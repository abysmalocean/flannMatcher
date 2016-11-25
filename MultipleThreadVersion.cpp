#include "MultipleThreadVersion.h"

//****************R*************************************************************
string dir_source;
string dir_target;
string fp_source;
int totalImageNumer = 0;
int n_threads = THREADS ;
static pthread_barrier_t barrier;

void* ReadData(void * arg)
{

	ThreadStruct* inputStruct = (ThreadStruct *) arg;
	int i = *inputStruct -> thread_ID;
	int minHessianInTread = HESSIAN;
	SurfFeatureDetector detector(minHessianInTread);
	SurfDescriptorExtractor extractor;
	//cout<< "Thread Id is " << i << endl ;
	//cout << "Total Number of Image is " << totalImageNumer << endl;
	string fp_target;

	//target
	for( i ; i < totalImageNumer ; i = i + n_threads)
	{
		struct dirent *tempdrip = (inputStruct->dirp_map)->find(i)->second;
		fp_target = dir_target + "/" + tempdrip->d_name;
		Mat img_2 = imread(fp_target, CV_LOAD_IMAGE_GRAYSCALE);
		if (!img_2.data) {
			printf(" --(!) Error reading images \n");
			pthread_exit(NULL);
			exit (1);
		}
		std::vector<KeyPoint> keypoints_2;
		Mat descriptors_2;
		//inputStruct->detector->detect(img_2, keypoints_2);
		//inputStruct->extractor->compute(img_2, keypoints_2, descriptors_2);
		detector.detect(img_2, keypoints_2);
		extractor.compute(img_2, keypoints_2, descriptors_2);

		FileData* tempFileData = (FileData*)malloc(sizeof(FileData));
		tempFileData->vector_at = i;
		memcpy ( tempFileData->Path, tempdrip->d_name, strlen(tempdrip->d_name)+1 );
		tempFileData->mappointer = inputStruct->MatMap;
		// not sure to use the mutex
		//pthread_mutex_lock(&mute_lock);
		(*(inputStruct->MatMap))[i] = descriptors_2;
		(*(inputStruct->FileDataVector)).push_back(tempFileData);
		//pthread_mutex_unlock(&mute_lock);
		cout << "THis is [ " << i << " ] Target Image,From Thread [ " << *inputStruct -> thread_ID << " ]" << endl;
		//cout << "File path is [ " << fp_target << " ]" << endl ;
	}
	//pthread_barrier_wait(&barrier);
	//cout << "Finish Loading the Target \n";

	//Source
	i = *inputStruct -> thread_ID;
		for( i ; i < totalImageNumer ; i = i + n_threads)
		{
			struct dirent *tempdrip = (inputStruct->Source_dirp_map)->find(i)->second;
			fp_target = dir_source + "/" + tempdrip->d_name;
			Mat img_2 = imread(fp_target, CV_LOAD_IMAGE_GRAYSCALE);
			if (!img_2.data) {
				printf(" --(!) Error reading images \n");
				pthread_exit(0);
			}
			std::vector<KeyPoint> keypoints_2;
			Mat descriptors_2;
			detector.detect(img_2, keypoints_2);
			extractor.compute(img_2, keypoints_2, descriptors_2);
			//inputStruct->detector->detect(img_2, keypoints_2);
			//inputStruct->extractor->compute(img_2, keypoints_2, descriptors_2);
			FileData* tempFileData = (FileData*)malloc(sizeof(FileData));
			tempFileData->vector_at = i ;
			memcpy ( tempFileData->Path, tempdrip->d_name, strlen(tempdrip->d_name)+1 );
			tempFileData->mappointer = inputStruct->Source_MatMap;
			// not sure to use the mutex
			//pthread_mutex_lock(&mute_lock);
			(*(inputStruct->Source_MatMap))[i] = descriptors_2;
			(*(inputStruct->Source_FileDataVector)).push_back(tempFileData);
			//pthread_mutex_unlock(&mute_lock);
			cout << "THis is [ " << i << " ] Source Image,From Thread [ " << *inputStruct -> thread_ID << " ]" << endl;
		}

}

long MultiThreadVersion() {
cout << "****************************************************************************"<<endl;
	printf("This is the MultiThread Version of this Project \n");
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
	int thread_ID[n_threads];
	for (int i = 0; i < n_threads; i++) {
		thread_ID[i]=i;
	}
	pthread_t *threads;
	threads = (pthread_t*)malloc(n_threads * sizeof(pthread_t));

	//*********************End of Init the path***********************************
	double min_distance;
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
	//************Target**********************
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
	FlannBasedMatcher matcher;

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

	for(int i = 0; i < n_threads; i++)
	{
		ThreadStruct* ThreadStruct_pointer    = (ThreadStruct*)malloc(sizeof(ThreadStruct));
		ThreadStruct_pointer-> thread_ID      = &thread_ID[i];
		// target
		ThreadStruct_pointer-> dirp_map              = &dirp_map;
		ThreadStruct_pointer-> MatMap         			 = &targetMat;
		ThreadStruct_pointer-> FileDataVector 			 = &targetStruct;
		// Source
		// target
		ThreadStruct_pointer-> Source_dirp_map       = &Source_dirp_map;
		ThreadStruct_pointer-> Source_MatMap         = &SourceMat;
		ThreadStruct_pointer-> Source_FileDataVector = &sourceStruct;

		//ThreadStruct_pointer-> detector       = &detector;
		//ThreadStruct_pointer-> extractor      = &extractor;

		pthread_create(&threads[i],NULL,ReadData,(void *)ThreadStruct_pointer);
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
	double average[targetSize][sourcesize];

	std::vector<DMatch> matches;
	//std::map<int, Mat>::iterator it;
  string src_name, tar_name;
	for (int sourceid = 0; sourceid < sourcesize; sourceid++) {
		min_distance = MAX_DISTANCE;
		Mat descriptors_1;
		FileData* SourceTemp = sourceStruct.at(sourceid);
		//printFileStruct(SourceTemp);
		cout << "sourced id is " << SourceTemp->vector_at << endl;
		descriptors_1 =  (SourceTemp->mappointer)->find(SourceTemp->vector_at)->second;
    src_name = SourceTemp -> Path;
		//mymap.find('a')->second
		//printFileStruct(targetTemp);
		for (int targetid = 0; targetid < targetSize; targetid++)
		{
			Mat descriptors_2;
			FileData* targetTemp = targetStruct.at(targetid);
			//printFileStruct(targetTemp);
			descriptors_2 =  (targetTemp->mappointer)->find(targetTemp->vector_at)->second;
			matcher.match(descriptors_1, descriptors_2, matches);
			// -- Quick calculation of max and min distances between keypoints
			double max_dist = 0;
			double min_dist = 100;
			for (int i = 0; i < descriptors_1.rows; i++) {
				double dist = matches[i].distance;
				if (dist < min_dist) min_dist = dist;
				if (dist > max_dist) max_dist = dist;
			}
			//Calculate Good match
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
				return 0;
			}
			average[sourceid][targetid] = min_distance;
      //cout << "best match is " << targetTemp->Path << " and " << src_name << " Avg " << ave_sum << endl;
      //cout << "ave_sum is " << ave_sum << endl;
      //cout << "min_distance is" << min_distance << endl;
			if (ave_sum < min_distance) {
				min_distance = ave_sum;
				tar_name = targetTemp->Path;

			}
		}// end for inner
      answer.insert(pair<string, string>(tar_name, src_name));
	}

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
	        "This Program is the Improved version Improved the Reading and compare "
	        "part\n");
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
