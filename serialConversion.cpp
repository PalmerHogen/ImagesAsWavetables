#define cimg_use_jpeg
#include "CImg.h"
#include "WAV.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cmath>
#include <math.h>
#include <time.h>
#include <string.h>
#include <errno.h>
#include <chrono>
#include "Wavetable.h"

using namespace std;
using namespace cimg_library;

int main(int argc, char **argv){

	int Z = 0;
	if (argc < 5){
		cout << "Usage: ./CONVERT [image path] [output name] [timestep(ms)] [gain]"<<endl;
		exit(2);
	}
	int timestep;
	double gain;
	if((timestep = atoi(argv[3])) < 10){
		cout << "Must enter a timestep greater than 10 ms" <<endl;
		exit(1);
	}
	if((gain = atof(argv[4])) <= 0.0f){
		cout << "Must enter a non-zero gain" << endl;
		exit(1);
	}	
	CImg<unsigned int> input(argv[1]);

    Wavetable w(timestep, gain, input);
    auto t1 = std::chrono::high_resolution_clock::now();
    w.writeAudio(argv[2]);
    auto t2 = std::chrono::high_resolution_clock::now();
    std::cout << "Audio Write took: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(t2-t1).count()
              << " milliseconds\n";
    
	
	return 0;
}
