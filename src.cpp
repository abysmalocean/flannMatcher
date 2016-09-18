/**
 * @file SURF_FlannMatcher
 * @brief SURF detector + descriptor + FLANN Matcher
 * @author A. Huaman
 */

#include "opencv2/opencv_modules.hpp"
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <string>
#include <iostream>

#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#ifndef HAVE_OPENCV_NONFREE

int main(int, char**)
{
	printf("The sample requires nonfree module that is not available in your OpenCV distribution.\n");
	return -1;
}

#else

# include "opencv2/core/core.hpp"
# include "opencv2/features2d/features2d.hpp"
# include "opencv2/highgui/highgui.hpp"
# include "opencv2/nonfree/features2d.hpp"

using namespace cv;
using namespace std;

void readme();

/**
 * @function main
 * @brief Main function
 */
int main( int argc, char** argv )
{
	if( argc != 2 ){
		readme();
		return -1;
	}

	string dir("/home/wangd7/Desktop/flannMatcher/image/source");
	cout<<dir<<endl;
	string filepath;
	DIR *dp;
	struct dirent *dirp;
	//cout<<"Input the file directory!"<<endl;
	//cin>>dir;

	dp = opendir(dir.c_str());

	if(dp == NULL){
		cout<<"Error opening "<<dir<<endl;
		return 0;
	}

	Mat img_2 = imread( argv[1], CV_LOAD_IMAGE_GRAYSCALE );
	while((dirp = readdir(dp))){
		filepath = dir + "/" + dirp->d_name;
				if(strcmp(dirp->d_name, ".") == 0 || strcmp(dirp->d_name, "..") == 0 ){
			continue;
		}
		else{
			cout<<"*****"<<filepath<<"*****"<<endl;
			Mat img_1 = imread( filepath.c_str(), CV_LOAD_IMAGE_GRAYSCALE );

			if( !img_1.data ){
				printf(" --(!) Error reading images \n");
				return -1;
			}

			//-- Step 1: Detect the keypoints using SURF Detector
			int minHessian = 400;
			SurfFeatureDetector detector( minHessian );
			std::vector<KeyPoint> keypoints_1, keypoints_2;
			detector.detect( img_1, keypoints_1 );
			detector.detect( img_2, keypoints_2 );

			//-- Step 2: Calculate descriptors (feature vectors)
			SurfDescriptorExtractor extractor;
			Mat descriptors_1, descriptors_2;
			extractor.compute( img_1, keypoints_1, descriptors_1 );
			extractor.compute( img_2, keypoints_2, descriptors_2 );

			//-- Step 3: Matching descriptor vectors using FLANN matcher
			FlannBasedMatcher matcher;
			std::vector< DMatch > matches;
			matcher.match( descriptors_1, descriptors_2, matches );

			double max_dist = 0; double min_dist = 100;

			//-- Quick calculation of max and min distances between keypoints
			for( int i = 0; i < descriptors_1.rows; i++ )
			{
				double dist = matches[i].distance;
				if( dist < min_dist )
					min_dist = dist;
				if( dist > max_dist )
					max_dist = dist;
			}

		//	printf("-- Max dist : %f \n", max_dist );
		//	printf("-- Min dist : %f \n", min_dist );

			//-- Draw only "good" matches (i.e. whose distance is less than 2*min_dist,
			//-- or a small arbitary value ( 0.02 ) in the event that min_dist is very
			//-- small)
			//-- PS.- radiusMatch can also be used here.
			std::vector< DMatch > good_matches;

			for( int i = 0; i < descriptors_1.rows; i++ )
			{
				if( matches[i].distance <= max(2*min_dist, 0.02) ){
					good_matches.push_back( matches[i]);
				}
			}

			//-- Draw only "good" matches
			Mat img_matches;
			drawMatches( img_1, keypoints_1, img_2, keypoints_2,
					good_matches, img_matches, Scalar::all(-1), Scalar::all(-1),
				vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );

			//-- Show detected matches
			imshow( "Good Matches", img_matches );

			double sum = 0;
			for( int i = 0; i < (int)good_matches.size(); i++ ){
				sum += good_matches[i].distance;
				// printf( "-- Good Match [%d] Keypoint 1: %4d  -- Keypoint 2: %4d  good distance: %.4f\n",
				//         i, good_matches[i].queryIdx, good_matches[i].trainIdx, good_matches[i].distance );
			}
			printf("average value is %.4f\n\n", sum/good_matches.size());
		//	waitKey(0);
		}
	}

	closedir(dp);
}

/**
 * @function readme
 */
void readme(){
	cout<<" Usage: ./SURF_FlannMatcher <img1> <img2>"<<endl;
}

#endif

