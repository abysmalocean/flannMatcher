#ifndef _SRC_H
#define _SRC_H

#include "opencv2/opencv_modules.hpp"
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <string>
#include <iostream>

#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <map>
#include <unistd.h>
//#include <direct.h>
#define MAX_DISTANCE 1

#ifndef HAVE_OPENCV_NONFREE

int main(int, char**)
{
	printf("The sample requires nonfree module that is not available in your OpenCV distribution.\n");
	printf("sudo add-apt-repository --yes ppa:xqms/opencv-nonfree \
					sudo apt-get update \
					sudo apt-get install libopencv-nonfree-dev")
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
 * @Auther Liang Xu
 * @ This code based on the Ram Hack challenge 2016
 */

 #include "currentfoler.h"

#endif
