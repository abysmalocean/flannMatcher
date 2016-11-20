#ifndef _IMPROVED_H
#define _IMPROVED_H

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
#include <vector>
#include <sys/time.h>
//#include <direct.h>
#define MAX_DISTANCE 1

# include "opencv2/core/core.hpp"
# include "opencv2/features2d/features2d.hpp"
# include "opencv2/highgui/highgui.hpp"
# include "opencv2/nonfree/features2d.hpp"


//typedef struct FileData FileData;
/*
struct FileData {
        int vector_at;
        struct dirent * Path;
        FileData()
        {
          vector_at = 0;
        }

};
};
*/
int Improved_than_Original();
# include "Improved.cpp"

#endif
