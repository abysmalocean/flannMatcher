
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
        if(WORKSET) cout<<
        "********[ Working on the **Big** Data Set ]**********************************"<<endl<<endl;
        else cout<<
        "********[ Working on the **Small** Data Set ]**********************************"<< endl<<endl;

        // original is the code we programed during the RAM Hack
        //TODO this is rung the original program
        long Original_counter = 1;
        long Improved_counter = 1;
        Original_counter = original();
        Improved_counter = Improved_than_Original();
        double speedupImproved = double(Original_counter)/double(Improved_counter);
        cout << "Speed up is [" << speedupImproved <<"]" <<endl;


}

/**
 * @function readme
 */
void readme() {
        cout << " Usage: ./SURF_FlannMatcher <img1> <img2>" << endl;
}
#endif  // ifndef HAVE_OPENCV_NONFRE
