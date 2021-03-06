#define cimg_use_jpeg
#include "CImg.h"
#include "WAV.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cmath>
#include <math.h>
#include <time.h>
#include <chrono>
#include <string.h>
#include <errno.h>
#include "Wavetable.h"

using namespace std;
using namespace cimg_library;

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
	CImg<unsigned int> input(argv[1]);

	auto start = chrono::system_clock::now();
    Wavetable w(timestep, input);
    w.writeAudio(argv[2]);
    auto stop = chrono::system_clock::now();
	auto elapsed = stop - start;
	cout << chrono::duration <double, milli> (elapsed).count() << " ms" << endl;
	
	return 0;
}
