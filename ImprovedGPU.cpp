#include "ImprovedGPU.h"
using namespace std;
using namespace cv;
using namespace cv::gpu;


//****************R*************************************************************
long Improved_GPUfucntion() {
cout << "****************************************************************************"<<endl;
	printf("This is the improved function\n");
	//**********************Init the Path*****************************************
	char cwd[1024];
	if (getcwd(cwd, sizeof(cwd)) != NULL)
		fprintf(stdout, "Current working dir: %s\n", cwd);
	else
		perror("getcwd() error");
	string str(cwd);
	string dir_source;
	string dir_target;
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
	string fp_source, fp_target;
	DIR *dp_source, *dp_target;
	struct dirent *dirp_source, *dirp_target;
	//*********************End of Init the path***********************************
	double min_distance;
	string str_source;
	map<string, string> answer;
	vector<string> filename;

	map<int,  vector<float> > targetMatGPU;
	map<int,  vector<float> > SourceMatGPU;

	map<int,  Mat > targetMat;
	map<int,  Mat > SourceMat;

	vector<FileDataGPU*> targetStruct;
	vector<FileDataGPU*> sourceStruct;

	min_distance = MAX_DISTANCE;

	//**********************Open the Target
	// Folder**********************************
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
	//****************Record the
	// time***********************************************
	struct timeval start, end;
	long seconds, useconds;
	gettimeofday(&start, NULL);

	// TODO
	// -- Step 1: Detect the keypoints using SURF Detector
	//cv::gpu::printShortCudaDeviceInfo(cv::gpu::getDevice());

	//******************************************************************************
	SURF_GPU surf;
	surf.hessianThreshold = HESSIAN;
	surf.nOctaves = 2 ;
	while (dirp_target = readdir(dp_target)) {
		//dirp_target.clear();
		fp_target = dir_target + "/" + dirp_target->d_name;

		if ((strcmp(dirp_target->d_name, ".") == 0) ||
		    (strcmp(dirp_target->d_name, "..") == 0)) {
			continue;
		} else {
			//cout << "[GPU] Start processing Target "
			//<< cnt2 << " image ......" << endl;
			GpuMat img_2;
			img_2.upload(imread(fp_target, CV_LOAD_IMAGE_GRAYSCALE));
			GpuMat keypoints2GPU ;
			GpuMat descriptors2GPU ;
			surf(img_2, GpuMat(), keypoints2GPU, descriptors2GPU);

			//init the keypoints and descriptors2
			std::vector<KeyPoint> keypoints_2;
			vector<float> descriptors2;
			Mat descriptorsMat2;

			surf.downloadKeypoints(keypoints2GPU, keypoints_2);
			descriptors2GPU.download(descriptorsMat2);
			surf.downloadDescriptors(descriptors2GPU, descriptors2);

			FileDataGPU* tempFileData = (FileDataGPU*)malloc(sizeof(FileDataGPU));
			targetMat[cnt2]    = descriptorsMat2;
			targetMatGPU[cnt2] = descriptors2;
			tempFileData->vector_at = cnt2;
			memcpy ( tempFileData->Path, dirp_target->d_name, strlen(dirp_target->d_name)+1 );
			tempFileData->mappointer = &targetMatGPU;
			targetStruct.push_back(tempFileData);
			cnt2++;
		}
	}
	cout << "Finish Importing the Target Image \n";

	surf.hessianThreshold = HESSIAN;
	surf.nOctaves = 5 ;
	while (dirp_source = readdir(dp_source)) {
		fp_source = dir_source + "/" + dirp_source->d_name;
		if ((strcmp(dirp_source->d_name, ".") == 0) ||
		    (strcmp(dirp_source->d_name, "..") == 0)) {
			continue;
		} else{
			//cout << "[Improved version] Start processing Target "
			//<< cnt2 << " image ......" << endl;
			GpuMat img_1;
			img_1.upload(imread(fp_source, CV_LOAD_IMAGE_GRAYSCALE));
			GpuMat keypoints1GPU ;
			GpuMat descriptors1GPU ;
			surf(img_1, GpuMat(), keypoints1GPU, descriptors1GPU);

			//init the keypoints and descriptors2
			std::vector<KeyPoint> keypoints_1;
			vector<float> descriptors1;

			surf.downloadKeypoints(keypoints1GPU, keypoints_1);
			surf.downloadDescriptors(descriptors1GPU, descriptors1);

			FileDataGPU* tempFileData = (FileDataGPU*)malloc(sizeof(FileDataGPU));
			SourceMatGPU[cnt1] = descriptors1;
			tempFileData->vector_at = cnt1;
			memcpy ( tempFileData->Path, dirp_source->d_name, strlen(dirp_source->d_name)+1 );
			tempFileData->mappointer = &SourceMatGPU;
			sourceStruct.push_back(tempFileData);
			cnt1++;
		}
	}
	cout << "Finish Importing the Source Image \n";
	//TODO The following is to Use Match Function


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
