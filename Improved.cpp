#include "Improved.h"
using namespace std;
using namespace cv;

struct FileData {
	map<int, Mat>* mappointer;
	int vector_at;
	char Path[256];
	FileData() {
		vector_at = 0;
	}
};
typedef struct FileData FileData;

void printFileStruct(FileData* input) {
	printf("index of map is [%d]", input->vector_at);
  //string str(input->Path->d_name);
  printf("File path is [%s]\n",input->Path);
}

//****************R*************************************************************
int Improved_than_Original() {
	printf("This is the improved function\n");
	//**********************Init the Path*****************************************
	char cwd[1024];
	if (getcwd(cwd, sizeof(cwd)) != NULL)
		fprintf(stdout, "Current working dir: %s\n", cwd);
	else
		perror("getcwd() error");
	string str(cwd);
	// string dir_source = str + "/source";
	// string dir_target = str + "/targets";
	string dir_source = str + "/source_Small";
	string dir_target = str + "/target_Small";
	string fp_source, fp_target;
	DIR *dp_source, *dp_target;
	struct dirent *dirp_source, *dirp_target;
	//*********************End of Init the path***********************************
	double min_distance;
	string str_source;
	map<string, string> answer;
	vector<string> filename;

	map<int, Mat> targetMat;
	map<int, Mat> SourceMat;

	vector<FileData*> targetStruct;
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
	int minHessian = HESSIAN;
	SurfFeatureDetector detector(minHessian);
	SurfDescriptorExtractor extractor;
	//******************************************************************************
	while (dirp_target = readdir(dp_target)) {
		str_source.clear();
		fp_target = dir_target + "/" + dirp_target->d_name;
		FileData* tempFileData = (FileData*)malloc(sizeof(FileData));

		if ((strcmp(dirp_target->d_name, ".") == 0) ||
		    (strcmp(dirp_target->d_name, "..") == 0)) {
			continue;
		} else {
			cout << "Start processing Target " << cnt2 << " image ......" << endl;
			Mat img_2 = imread(fp_target, CV_LOAD_IMAGE_GRAYSCALE);
			if (!img_2.data) {
				printf(" --(!) Error reading images \n");
				return -1;
			}
			std::vector<KeyPoint> keypoints_2;
			Mat descriptors_2;
			detector.detect(img_2, keypoints_2);
			extractor.compute(img_2, keypoints_2, descriptors_2);
			// push to data structure
			FileData* tempFileData = (FileData*)malloc(sizeof(FileData));
			SourceMat[cnt2] = descriptors_2;
			tempFileData->vector_at = cnt2;
      memcpy ( tempFileData->Path, dirp_target->d_name, strlen(dirp_target->d_name)+1 );
			tempFileData->mappointer = &targetMat;
			targetStruct.push_back(tempFileData);
      cnt2++;
      //printFileStruct(tempFileData);
			//cout << " Size of Target vertor " << targetStruct.size() << endl;
		}
	}
	while ((dirp_source = readdir(dp_source))) {
		fp_source = dir_source + "/" + dirp_source->d_name;
		if ((strcmp(dirp_source->d_name, ".") == 0) ||
		    (strcmp(dirp_source->d_name, "..") == 0)) {
			continue;
		} else {
			cout << "Start processing Source " << cnt1 << " image ......" << endl;
			Mat img_1 = imread(fp_source.c_str(), CV_LOAD_IMAGE_GRAYSCALE);
			if (!img_1.data) {
				printf(" --(!) Error reading images \n");
				return -1;
			}
			std::vector<KeyPoint> keypoints_1;
			Mat descriptors_1;
			detector.detect(img_1, keypoints_1);
			extractor.compute(img_1, keypoints_1, descriptors_1);
			// push to data structure
			FileData* tempFileData = (FileData*)malloc(sizeof(FileData));
			targetMat[cnt1] = descriptors_1;
			tempFileData->vector_at = cnt1;
      memcpy ( tempFileData->Path, dirp_source->d_name, strlen(dirp_source->d_name)+1 );
			tempFileData->mappointer = &SourceMat;
			sourceStruct.push_back(tempFileData);
      cnt1++;
		}
	}
  int targetSize = (int)targetMat.size();
  int sourcesize = (int)SourceMat.size();
	printf("sizeof the target files [%d]\n", targetSize);
	printf("sizeof the source files [%d]\n", sourcesize);
  double average[targetSize][sourcesize];
  for (int sourceid = 0; sourceid < sourcesize; sourceid++){
    FileData* SourceTemp = sourceStruct.at(sourceid);
    //printFileStruct(targetTemp);
		for (int targetid = 0; targetid < targetSize; targetid++)
		{
      FileData* targetTemp = targetStruct.at(targetid);
		}
	}


	/*
	        // -- Step 3: Matching descriptor vectors using FLANN matcher
	        FlannBasedMatcher matcher;
	        std::vector<DMatch> matches;
	        matcher.match(descriptors_1, descriptors_2, matches);
	 */

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
	printf("total micro seconds is --->[%ld]\n", total_micro_seconds);
	//******************************************************************************

	closedir(dp_source);
	closedir(dp_target);
}
