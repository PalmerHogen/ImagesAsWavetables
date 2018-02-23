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

<<<<<<< HEAD
    ImageWaveConverter myConverter(timestep, gain, input);
    myConverter.convert(argv[2]);
=======
    Wavetable w(timestep, gain, input);
    w.writeAudio(argv[2]);
>>>>>>> master
	
	return 0;
}
