This code is using the cmake and opencv package

src.cpp ---------------> main file
*************************
cmake .
make
./exe
*************************

this program has 5 modules.
1. Origianl   -------------------> 18191416089 micro seconds
   < original.cpp , original.h >
2. Improved programming  --------> 1315246048  micro seconds speedup 13.8312
   < Improved.cpp , Improved.h >
3. Multi Thread  ----------------> 137352134   micro seconds speedup 132.444
   < MultipleThreadVersion.cpp, MultipleThreadVersion.h >
4. Single GPU  ------------------> 28178377    micro seconds speedup 645.581
   < ImprovedGPU.cpp, ImprovedGPU.h >
5. Multiple GPU -----------------> 16492464    micro seconds speedup 1103.013
   < ImprovedGPU.cpp, ImprovedGPU.h >

Result based on 20 thread and 8 GUP

```
< src.h >

HESSIAN Value 400
WORKSET 1   1 ---> Big data
            0 ---> Small Data

THREADS 20  Number of Thread in multi thread
THREADSGPU 8  Number of GUP

```
```
uncommon which you want to run
< src.cpp >
Original_counter = original();
Improved_counter = Improved_than_Original();
MultiThraed_counter = MultiThreadVersion();
Improved_GPU  = Improved_GPUfucntion();
MultiGUp_Counter = MultiGUP();
```

result in result/

Note: Do not try to run the origianl version it take about 5.05 hours
