#include "Improved.h"
using namespace std;
using namespace cv;

struct FileData {
        map< int, Mat> * mappointer;
        int vector_at;
        struct dirent * Path;
        FileData()
        {
          vector_at = 0;
        }

};
typedef struct FileData FileData;

void printFileStruct(FileData* input)
{
    printf("index of map is [%d]", input -> vector_at);
}

//****************R*************************************************************
int Improved_than_Original()
{
        printf("This is the improved function\n" );
        //**********************Init the Path*****************************************
        char cwd[1024];
        if (getcwd(cwd, sizeof(cwd)) != NULL)
                fprintf(stdout, "Current working dir: %s\n", cwd);
        else
                perror("getcwd() error");
        string str(cwd);
        string dir_source = str + "/source";
        string dir_target = str + "/targets";
        string fp_source, fp_target;
        DIR *dp_source, *dp_target;
        struct dirent *dirp_source, *dirp_target;
        //*********************End of Init the path***********************************
        double min_distance;
        string str_source;
        map<string, string> answer;
        vector<string> filename;

        map< int, Mat> targetMat;
        map< int, Mat> SourceMat;
        vector<FileData*> targetStruct;
        vector<FileData*> sourceStruct;

        min_distance = MAX_DISTANCE;

//**********************Open the Target Folder**********************************
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
        int cnt = 0;
        int cnt2 = 0;
//****************Record the time***********************************************
        struct timeval start, end;
        long seconds, useconds;
        gettimeofday(&start, NULL);
//******************************************************************************
        while (dirp_target = readdir(dp_target)) {
                str_source.clear();
                fp_target = dir_target + "/" + dirp_target->d_name;
                FileData* tempFileData = (FileData*)malloc(sizeof(FileData));

                if ((strcmp(dirp_target->d_name, ".") == 0) ||
                    (strcmp(dirp_target->d_name, "..") == 0)) {
                        continue;
                } else {
                        cout << "Start processing Target " << ++cnt << " image ......" << endl;
                        Mat img_2 = imread(fp_target, CV_LOAD_IMAGE_GRAYSCALE);
                        if (!img_2.data) {
                                printf(" --(!) Error reading images \n");
                                return -1;
                        }
                        targetMat[cnt]            = img_2;
                        tempFileData -> vector_at = cnt ;
                        tempFileData -> Path      = dirp_target;
                        tempFileData -> mappointer= &targetMat;
                        targetStruct.push_back(tempFileData);
                        //cout<<" Size of Target vertor " <<targetStruct.size()<<endl;
                }
        }
        while ((dirp_source = readdir(dp_source))) {
                FileData* tempFileData = (FileData*)malloc(sizeof(FileData));
                fp_source = dir_source + "/" + dirp_source->d_name;
                if ((strcmp(dirp_source->d_name, ".") == 0) ||
                    (strcmp(dirp_source->d_name, "..") == 0)) {
                        continue;
                } else {
                        cout << "Start processing Source " << ++cnt2 << " image ......" << endl;
                        Mat img_1 = imread(fp_source.c_str(), CV_LOAD_IMAGE_GRAYSCALE);
                        if (!img_1.data) {
                                printf(" --(!) Error reading images \n");
                                return -1;
                        }
                        SourceMat[cnt]            = img_1;
                        tempFileData -> vector_at = cnt2 ;
                        tempFileData -> Path      = dirp_source;
                        tempFileData -> mappointer= &SourceMat;
                        sourceStruct.push_back(tempFileData);
                }
        }



//****************Record the time***********************************************
gettimeofday(&end, NULL);
seconds = end.tv_sec - start.tv_sec;
useconds = end.tv_usec - start.tv_usec;

if (useconds < 0) {
        seconds -= 1;
}
long total_micro_seconds = (seconds * 1000000) + abs(useconds);
printf("This Program is the Improved version Improved the Reading and compare part\n");
printf("total micro seconds is --->[%ld]\n", total_micro_seconds);
//******************************************************************************

closedir(dp_source);
closedir(dp_target);

}
