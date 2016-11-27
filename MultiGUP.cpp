#include "MultiGUP.h"

//****************R*************************************************************

void* ReadDataGPU(void * arg)
{
	ThreadStruct* inputStruct = (ThreadStruct *) arg;
	int i = *inputStruct->thread_ID;
	cv::gpu::setDevice(i+1);
	//cv::gpu::printShortCudaDeviceInfo(cv::gpu::getDevice());

	SURF_GPU surf;
	surf.hessianThreshold = HESSIAN;
	surf.nOctaves = 2 ;

	string fp_target;
	//target
	for( i; i < totalImageNumer; i = i + n_threads)
	{
		struct dirent *tempdrip = (inputStruct->dirp_map)->find(i)->second;
		fp_target = dir_target + "/" + tempdrip->d_name;
		GpuMat img_2;
		img_2.upload(imread(fp_target, CV_LOAD_IMAGE_GRAYSCALE));
		GpuMat keypoints2GPU ;
		GpuMat descriptors2GPU ;
		surf(img_2, GpuMat(), keypoints2GPU, descriptors2GPU);

		std::vector<KeyPoint> keypoints_2;
		vector<float> descriptors2;
		Mat descriptorsMat2;

		descriptors2GPU.download(descriptorsMat2);

		FileData* tempFileData = (FileData*)malloc(sizeof(FileData));
		tempFileData->vector_at = i;
		memcpy ( tempFileData->Path, tempdrip->d_name, strlen(tempdrip->d_name)+1 );
		tempFileData->mappointer = inputStruct->MatMap;
		// not sure to use the mutex
		//pthread_mutex_lock(&mute_lock);
		(*(inputStruct->MatMap))[i] = descriptorsMat2;
		(*(inputStruct->FileDataVector)).push_back(tempFileData);

		//pthread_mutex_unlock(&mute_lock);
		//cout << "THis is [ " << i << " ] Target Image,From Thread [ " << *inputStruct->thread_ID << " ]" << endl;
		//cout << "File path is [ " << fp_target << " ]" << endl ;
	}
	//pthread_barrier_wait(&barrier);

	//Source
	surf.hessianThreshold = HESSIAN;
	surf.nOctaves = 5 ;
	i = *inputStruct->thread_ID;
	for( i; i < totalImageNumer; i = i + n_threads)
	{
		struct dirent *tempdrip = (inputStruct->Source_dirp_map)->find(i)->second;
		fp_target = dir_source + "/" + tempdrip->d_name;
		//cout << "Source path is " << fp_target << endl;
		GpuMat img_1;

		Mat temMat = imread(fp_target, CV_LOAD_IMAGE_GRAYSCALE);
		if (!temMat.data) {
			printf(" --(!) Error reading images \n");
			pthread_exit(0);
		}
		img_1.upload(temMat);

		GpuMat keypoints1GPU ;
		GpuMat descriptors1GPU ;
		surf(img_1, GpuMat(), keypoints1GPU, descriptors1GPU);

		//init the keypoints and descriptors2
		std::vector<KeyPoint> keypoints_1;
		vector<float> descriptors1;
		Mat descriptorsMat1;

		//surf.downloadKeypoints(keypoints1GPU, keypoints_1);
		descriptors1GPU.download(descriptorsMat1);
		FileData* tempFileData = (FileData*)malloc(sizeof(FileData));
		tempFileData->vector_at = i;
		memcpy ( tempFileData->Path, tempdrip->d_name, strlen(tempdrip->d_name)+1 );
		tempFileData->mappointer = inputStruct->Source_MatMap;
		(*(inputStruct->Source_MatMap))[i] = descriptorsMat1;
		(*(inputStruct->Source_FileDataVector)).push_back(tempFileData);
		//pthread_mutex_unlock(&mute_lock);
	}
	cout << "Thread ["<< *inputStruct->thread_ID << " ] Finish uploading the pic \n";
	//cout << "Total Image Number is " << totalImageNumer << endl;

	pthread_barrier_wait(&barrier);
	// Get the Result

	double min_distance = MAX_DISTANCE;
	i = *inputStruct->thread_ID;
	string src_name, tar_name;

	BFMatcher_GPU matcher(NORM_L2);
	std::vector<DMatch> matches;
	for( i; i < totalImageNumer; i = i + n_threads)
	{
		min_distance = MAX_DISTANCE;
		GpuMat img1;
		Mat descriptors_1;
		FileData* SourceTemp = (*(inputStruct->Source_FileDataVector)).at(i);
		//cout << "sourced id is " << SourceTemp->vector_at << " at Thread " << *inputStruct->thread_ID << endl;
		descriptors_1 =  (SourceTemp->mappointer)->find(SourceTemp->vector_at)->second;
		img1.upload(descriptors_1);
		src_name = SourceTemp -> Path;
		for (int targetid = 0; targetid < totalImageNumer; targetid++)
		{
			GpuMat img2;
			Mat descriptors_2;
			FileData* targetTemp = (*(inputStruct->FileDataVector)).at(targetid);
			descriptors_2 =  (targetTemp->mappointer)->find(targetTemp->vector_at)->second;
			img2.upload(descriptors_2);
			GpuMat trainIdx, distance;

			matcher.matchSingle(img1, img2, trainIdx, distance);
			BFMatcher_GPU::matchDownload(trainIdx, distance, matches);

			double max_dist = 0;
			double min_dist = 100;
			//cout <<  "descriptors_2.rows " <<  descriptors_2.rows << endl;
			//cout <<  "descriptors_1.rows " <<  descriptors_1.rows << endl;
			for (int i = 0; i < descriptors_1.rows; i++) {
				double dist = matches[i].distance;
				if (dist < min_dist) min_dist = dist;
				if (dist > max_dist) max_dist = dist;
			}

			//cout << "Min Distache is " << min_dist << endl ;

			std::vector<DMatch> good_matches;
			for (int i = 0; i < descriptors_1.rows; i++) {
				if (matches[i].distance <= max(2 * min_dist, 0.02)) {
					good_matches.push_back(matches[i]);
				}
			}

			double sum = 0;
			double ave_sum = 0;
			for (int i = 0; i < (int)good_matches.size(); i++) {
				sum += good_matches[i].distance;
			}
			ave_sum = sum / good_matches.size();
			if(isnan(ave_sum))
			{
				printf("Change Hessan Value\n" );
				//return 0;
			}
			//cout << "best match is " << targetTemp->Path << " and " << src_name << " Avg " << ave_sum << endl;
			//cout << "ave_sum is " << ave_sum << endl;
			//cout << "min_distance is" << min_distance << endl;
			if (ave_sum < min_distance) {
				min_distance = ave_sum;
				tar_name = targetTemp->Path;
			}
		}//end loop inner
		(*(inputStruct->answer)).insert(pair<string, string>(tar_name, src_name));
	}//end loop outer

}


long MultiGUP() {
	cout << "****************************************************************************"<<endl;
	printf("This is the MultiGUP Version of this Project \n");
	//**********************Init the Path*****************************************
	char cwd[1024];

	if (getcwd(cwd, sizeof(cwd)) != NULL)
		fprintf(stdout, "Current working dir: %s\n", cwd);
	else
		perror("getcwd() error");
	string str(cwd);

	int workpath = WORKSET;
	if(workpath)
	{
		dir_source = str + "/source";
		dir_target = str + "/targets";
	}
	else
	{
		dir_source = str + "/source_Small";
		dir_target = str + "/target_Small";
	}
	cout << " Source Data Path is " << dir_source << endl;
	cout << " Target Data Path is " << dir_target << endl;

	DIR *dp_source, *dp_target;
	//struct dirent *dirp_source, *dirp_target;
	struct dirent *dirp_source, *dirp_target;
	//***************iR*************************************************************
	n_threads = THREADSGPU;
	int thread_ID[n_threads];
	for (int i = 0; i < n_threads; i++) {
		thread_ID[i]=i;
	}
	pthread_t *threads;
	threads = (pthread_t*)malloc(n_threads * sizeof(pthread_t));

	//*********************End of Init the path***********************************
	string str_source;
	map<string, string> answer;
	vector<string> filename;

	map<int, Mat> targetMat;
	map<int, Mat> SourceMat;
	map<int, struct dirent*> dirp_map;
	map<int, struct dirent*> Source_dirp_map;

	vector<FileData*> targetStruct;
	vector<FileData*> sourceStruct;


	//**********************Open the Target Folder********************************
	//************************************************Target**********************
	dp_target = opendir(dir_target.c_str());
	if (dp_target == NULL) {
		cout << "Error opening " << endl;
		return 0;
	}
	//************Source**********************
	dp_source = opendir(dir_source.c_str());
	if (dp_source == NULL) {
		cout << "Error opening " << endl;
		return 0;
	}
	// cnt is the the courting variable
	int cnt1 = 0;
	int cnt2 = 0;
	//****************Record the time********************************************
	struct timeval start, end;
	long seconds, useconds;
	gettimeofday(&start, NULL);

	// TODO
	// -- Step 1: Detect the keypoints using SURF Detector
	int minHessian = HESSIAN;
	cout<<"\n";
	printf("Hessian distance is ----> [%d]\n", minHessian);
	printf("Number of Thread is ----> [%d]\n", n_threads);
//	SurfFeatureDetector detector(minHessian);
//	SurfDescriptorExtractor extractor;
//	FlannBasedMatcher matcher;

	//***************************************************************************
	while (dirp_target = readdir(dp_target)) {
		if ((strcmp(dirp_target->d_name, ".") == 0) ||
		    (strcmp(dirp_target->d_name, "..") == 0)) {
			continue;
		} else {
			dirp_map[totalImageNumer]=dirp_target;
			totalImageNumer = totalImageNumer + 1;
		}
	}
	int countSource = 0;
	while ((dirp_source = readdir(dp_source))) {
		if ((strcmp(dirp_source->d_name, ".") == 0) ||
		    (strcmp(dirp_source->d_name, "..") == 0)) {
			continue;
		} else {
			Source_dirp_map[countSource]=dirp_source;
			countSource = countSource + 1;
		}
	}
	int s = pthread_barrier_init(&barrier, NULL, n_threads);
	if (s != 0)
	{
		cout << "Printf have Issue Quit the Program \n";
		exit(0);
	}
/* Create 'numThreads' threads */
	//cv::gpu::setDevice(0);
	//cv::gpu::printShortCudaDeviceInfo(cv::gpu::getDevice());
	for(int i = 0; i < n_threads; i++)
	{
		ThreadStruct* ThreadStruct_pointer    = (ThreadStruct*)malloc(sizeof(ThreadStruct));
		ThreadStruct_pointer->thread_ID        = &thread_ID[i];
		// target
		ThreadStruct_pointer->dirp_map              = &dirp_map;
		ThreadStruct_pointer->MatMap                = &targetMat;
		ThreadStruct_pointer->FileDataVector        = &targetStruct;
		// Source
		// target
		ThreadStruct_pointer->Source_dirp_map       = &Source_dirp_map;
		ThreadStruct_pointer->Source_MatMap         = &SourceMat;
		ThreadStruct_pointer->Source_FileDataVector = &sourceStruct;

		//ThreadStruct_pointer-> detector       = &detector;
		//ThreadStruct_pointer-> extractor      = &extractor;

		ThreadStruct_pointer->answer = &answer;
		int err = pthread_create(&threads[i],NULL,ReadDataGPU,(void *)ThreadStruct_pointer);
		if (err != 0) printf("cannot create thread");
	}

	for(int i = 0; i < n_threads; i++)
	{
		pthread_join(threads[i],NULL);
	}

	// Calcuate the Match
	int targetSize = (int)targetMat.size();
	int sourcesize = (int)SourceMat.size();
	printf("sizeof the target files [%d]\n", targetSize);
	printf("sizeof the source files [%d]\n", sourcesize);

	//****************Record the
	// time***********************************************
	gettimeofday(&end, NULL);
	seconds = end.tv_sec - start.tv_sec;
	useconds = end.tv_usec - start.tv_usec;

	if (useconds < 0) {
		seconds -= 1;
	}
	long total_micro_seconds = (seconds * 1000000) + abs(useconds);
	printf(
	        "This Program is the MultiGUP Version  \n");
	printf("total micro seconds is ----->[%ld]\n", total_micro_seconds);
	//******************************************************************************

	closedir(dp_source);
	closedir(dp_target);
	std::cout<<"{\"env\":\"dev\",\"answers\":{";
	int size = answer.size()-1;
	for(std::map<std::string, std::string>::iterator iter = answer.begin(); iter != answer.end(); iter++) {
		std::cout<<"\"" << iter->first<< "\"" <<":"<<iter->second;
		if(size != 0)
			std::cout<<",";
		size--;
	}
	std::cout<<"}}"<<std::endl;
	return total_micro_seconds;
}
