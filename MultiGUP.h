#ifndef _MULTIGUP_H
#define _MULTIGUP_H
/*
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
	map<string, string>* answer;

};
typedef struct ThreadStruct ThreadStruct;

long MultiThreadVersion();
*/
# include "MultiGUP.cpp"
long MultiGUP();

#endif
