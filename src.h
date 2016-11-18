#ifndef _SRC_H
#define _SRC_H

#include "ProjectHelper.h"
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


#else

# include "opencv2/core/core.hpp"
# include "opencv2/features2d/features2d.hpp"
# include "opencv2/highgui/highgui.hpp"
# include "opencv2/nonfree/features2d.hpp"

using namespace cv;
using namespace std;



#endif
#endif
