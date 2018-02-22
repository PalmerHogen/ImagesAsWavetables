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

#define PI 3.14159265358979323846264338327f

using namespace std;
using namespace cimg_library;

//Pixel Brightness, in range 0-1000
double brightness(CImg<double> img, int i, int j){
	return (img(i, j, 0) + img(i, j, 1) + img(i, j, 2))*1.3f;
}

//Exponential Scaling between 1 and 1000 for startpoint 0 and endpoint h-1
double eScale(int h, int i){
	return 1000.0f/double(h*h) * double(i*i) + 1.0f;
}

int main(int argc, char **argv){
	int SAMPLERATE = SAMPLES_PER_SECOND;

	//Retrieve user input
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

	//Number of samples devoted to each time step
	int stepLength = int(SAMPLERATE * double(timestep) * 0.001f);

	CImg<double> input(argv[1]);
	//Optimizations: -----------------------------------------------
	//Floating point division is slow; so I'll do these upfront
	double invH = 1.0f / double(input.height());
	double invS = 1.0f / double(SAMPLERATE);	

	//math.sin() is also slow
	double *sine = new double[SAMPLERATE];
	for (int b=0; b<SAMPLERATE; b++){
		sine[b] = sin(2*PI*b*invS);
	}
	//--------------------------------------------------------------
	double *amplitudes = new double[input.height()];
	double *frequencies = new double[input.height()];
	
	double fundamental = 20.0f;
	double freq;

	for (int i=0; i<input.height(); i++){
		freq = fundamental * eScale(input.height(), i);
		amplitudes[i] = 0.0f;
		frequencies[input.height() - 1 - i] = freq;
	}

	//Intermediate audio-write buffer
	short *audioBuffer = new short[stepLength];

	FILE *f = openWav(argv[2]);

	for(int j=0; j<(input.width()); j++){
		for (int k=0; k<input.height(); k++){
			amplitudes[k] = brightness(input, j, k)*gain*invH;
		}
		for (int A=0; A<stepLength; A++){
			double spl = 0.0f;
			long pos = j * stepLength + A;
			for (int l=0; l<input.height(); l++){
				spl += amplitudes[l]*sine[long(frequencies[l])*pos%SAMPLERATE]; 
			}
			audioBuffer[A] = spl;
		}
		//write intermediate results
		writeWav(f, audioBuffer, stepLength);
	}
	closeWav(f);

	delete amplitudes;
	delete frequencies;
	delete audioBuffer;
	delete sine;
	return 0;
}
