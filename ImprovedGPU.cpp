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

	map<int,  vector<float> > targetMat;
	map<int, Mat > SourceMat;

	vector<FileDataGPU*> targetStruct;
	vector<FileData*> sourceStruct;

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
	cv::gpu::printShortCudaDeviceInfo(cv::gpu::getDevice());

	//******************************************************************************
	while (dirp_target = readdir(dp_source)) {
		//dirp_target.clear();
		fp_target = dir_target + "/" + dirp_target->d_name;

		if ((strcmp(dirp_target->d_name, ".") == 0) ||
		    (strcmp(dirp_target->d_name, "..") == 0)) {
			continue;
		} else {
			SURF_GPU surf;
			cout << "[Improved version] Start processing Target "
			<< cnt2 << " image ......" << endl;
			GpuMat img_2;
			img_2.upload(imread(fp_target, CV_LOAD_IMAGE_GRAYSCALE));
			//CV_Assert(!img_2.empty());

			GpuMat keypoints2GPU ;
			GpuMat descriptors2GPU ;
			surf(img_2, GpuMat(), keypoints2GPU, descriptors2GPU);

			std::vector<KeyPoint> keypoints_2;
			vector<float> descriptors2;
			surf.downloadKeypoints(keypoints2GPU, keypoints_2);
			surf.downloadDescriptors(descriptors2GPU, descriptors2);

			FileDataGPU* tempFileData = (FileDataGPU*)malloc(sizeof(FileDataGPU));

			targetMat[cnt2] = descriptors2;
			tempFileData->vector_at = cnt2;
			memcpy ( tempFileData->Path, dirp_target->d_name, strlen(dirp_target->d_name)+1 );
			//cout << "File Path is " << fp_target << endl;
			//cout << "Structure Path is " << tempFileData->Path << endl;

			tempFileData->mappointer = &targetMat;
			targetStruct.push_back(tempFileData);
			cnt2++;
			//printFileStruct(tempFileData);
			//cout << " Size of Target vertor " << targetStruct.size() << endl;
		}
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
