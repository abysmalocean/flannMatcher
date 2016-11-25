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

long printFileStruct(FileData* input) {
	printf("index of map is [%d] ", input->vector_at);
	//string str(input->Path->d_name);
	printf("File path is [%s]\n",input->Path);
}

//****************R*************************************************************
long Improved_than_Original() {
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
	printf("Hessian distance is ----> [%d]\n", minHessian);
	SurfFeatureDetector detector(minHessian);
	SurfDescriptorExtractor extractor;
	FlannBasedMatcher matcher;

	//******************************************************************************
	while (dirp_target = readdir(dp_target)) {
		//dirp_target.clear();
		fp_target = dir_target + "/" + dirp_target->d_name;

		if ((strcmp(dirp_target->d_name, ".") == 0) ||
		    (strcmp(dirp_target->d_name, "..") == 0)) {
			continue;
		} else {
			cout << "[Improved version] Start processing Target "
			<< cnt2 << " image ......" << endl;
			Mat img_2 = imread(fp_target, CV_LOAD_IMAGE_GRAYSCALE);
			// error handle
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

			targetMat[cnt2] = descriptors_2;
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
	while ((dirp_source = readdir(dp_source))) {
		fp_source = dir_source + "/" + dirp_source->d_name;
		if ((strcmp(dirp_source->d_name, ".") == 0) ||
		    (strcmp(dirp_source->d_name, "..") == 0)) {
			continue;
		} else {
			cout << "[Improved version] Start processing Source "
			<< cnt1 << " image ......" << endl;
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
			SourceMat[cnt1] = descriptors_1;
			tempFileData->vector_at = cnt1;
			memcpy ( tempFileData->Path, dirp_source->d_name, strlen(dirp_source->d_name)+1 );
			//cout << "File Path is " << fp_source << endl;
			//cout << "Structure Path is " << tempFileData->Path << endl;
			tempFileData->mappointer = &SourceMat;
			sourceStruct.push_back(tempFileData);
			cnt1++;
		}
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
