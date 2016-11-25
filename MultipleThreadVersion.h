#ifndef _MULTIPLETHREAD_H
#define _MULTIPLETHREAD_H
#include <pthread.h>

pthread_mutex_t mute_lock;

struct ThreadStruct{
	int* thread_ID;
	//target
	map<int, struct dirent*>* dirp_map;
	map<int, Mat>* MatMap;
	vector<FileData*>* FileDataVector;
	//source
	map<int, struct dirent*>* Source_dirp_map;
	map<int, Mat>* Source_MatMap;
	vector<FileData*>* Source_FileDataVector;

//	SurfFeatureDetector* detector;
//	SurfDescriptorExtractor* extractor;

};
typedef struct ThreadStruct ThreadStruct;

long MultiThreadVersion();

# include "MultipleThreadVersion.cpp"


#endif
