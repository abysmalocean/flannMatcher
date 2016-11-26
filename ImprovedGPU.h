#ifndef _IMPROVEDGPU_H
#define _IMPROVEDGPU_H
#include "opencv2/gpu/gpu.hpp"
#include "opencv2/nonfree/gpu.hpp"
struct FileDataGPU {
	map<int, vector<float> >* mappointer;
	int vector_at;
	char Path[256];
	FileDataGPU() {
		vector_at = 0;
	}
};
typedef struct FileDataGPU FileDataGPU;

long Improved_GPUfucntion();
# include "ImprovedGPU.cpp"

#endif
