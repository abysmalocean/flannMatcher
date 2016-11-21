
#include "src.h"
#ifndef HAVE_OPENCV_NONFRE

int main(int, char **) {
        printf(
                "The sample requires nonfree module that is not available in your "
                "OpenCV distribution.\n");

        printf(
                "\n\
	               sudo add-apt-repository --yes ppa:xqms/opencv-nonfree \n\
	               sudo apt-get update \n\
	               sudo apt-get install libopencv-nonfree-dev\n"                                                                                                        );

        return -1;
}

#else  // ifndef HAVE_OPENCV_NONFRE

void readme();

/**
 * @Auther Liang Xu
 * @ This code based on the Ram Hack challenge 2016
 * @ During  the Ram Hack our group have Dongwei Wang and Mo Li
 */
using namespace std;
using namespace cv;

int main(int argc, char **argv) {
        if (argc != 1) {
                readme();
                return -1;
        }

        // original is the code we programed during the RAM Hack
        //TODO this is rung the original program
        original();

        //Improved_than_Original();


}

/**
 * @function readme
 */
void readme() {
        cout << " Usage: ./SURF_FlannMatcher <img1> <img2>" << endl;
}
#endif  // ifndef HAVE_OPENCV_NONFRE
