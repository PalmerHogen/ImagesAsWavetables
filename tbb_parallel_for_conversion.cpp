#define cimg_use_jpeg
#include "CImg.h"
#include "WAV.h"
#include <chrono>
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <iostream>
#include <cmath>
#include <math.h>
#include <time.h>
#include <string.h>
#include <errno.h>
#include "Wavetable.h"
//TBB includes
#include "tbb/task_group.h"
#include "tbb/task_scheduler_init.h"

using namespace std;
using namespace cimg_library;
typedef chrono::high_resolution_clock Clock;

int main(int argc, char **argv){

	int Z = 0;
	if (argc < 4){
		cout << "Usage: ./CONVERT [image path] [output name] [timestep(ms)]"<<endl;
		exit(2);
	}
	int timestep;
	if((timestep = atoi(argv[3])) < 10){
		cout << "Must enter a timestep greater than 10 ms" <<endl;
		exit(1);
	}
	

/* Unused, I'm going to implement parallel for first before tasking 
    int numthreads = tbb::task_scheduler_init::default_num_threads();
    tbb::task_scheduler_init init(numthreads);
    printf("Number of threads: %d\n", numthreads);
    tbb::task_group g;
*/

	CImg<unsigned char> input(argv[1]);

    auto precomp = Clock::now();
    Wavetable w(timestep, input);
    w.writeAudioParallelFor(argv[2]);
    auto postcomp = Clock::now();
    chrono::milliseconds mscomp = chrono::duration_cast<chrono::milliseconds>(postcomp-precomp);
    cout << "TBB Time: " << mscomp.count() <<" ms"<< endl;
	
	return 0;
}
