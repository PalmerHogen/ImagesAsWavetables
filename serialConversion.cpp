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
#define FUNDAMENTAL 20.0f

using namespace std;
using namespace cimg_library;

//Pixel Brightness, in range 0-1000
double brightness(CImg<double> img, int i, int j){
	return (img(i, j, 0) + img(i, j, 1) + img(i, j, 2))*1.3f;
}

//Exponential Scaling between 1 and 1000 for startpoint 0 and endpoint h-1
int eScale(int h, int i){
	return i*i *1000 /(h*h) + 1;
}

//Linear Scaling between 1 and 1000 for startpoint 0 and endpoint h-1
double lScale(int h, int i){
	return double(i)*(1000.0f)/double(h) + 1.0f;
}

int main(int argc, char **argv){
	int CYCLELENGTH = SAMPLERATE;

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
		cout << "Must enter a positive gain" << endl;
		exit(1);
	}	

	//Number of samples devoted to each time step
	int stepLength = int(double(SAMPLERATE * timestep) * 0.001f);

	CImg<double> input(argv[1]);
	//Optimizations: -----------------------------------------------
	//Floating point division is slow; so I'll do these upfront
	double invH = 1.0f / double(input.height());
	double invS = 1.0f / double(SAMPLERATE);	
	double invC = 1.0f / double(CYCLELENGTH);

	//math.sin() is also slow
	double *sine = new double[CYCLELENGTH];
	for (int b=0; b<CYCLELENGTH; b++){
		sine[b] = sin(2*PI*b*invC);
	}
	//--------------------------------------------------------------
	double *amplitudes = new double[input.height()];
	double *frequencies = new double[input.height()];
	
	double freq;

	for (int i=0; i<input.height(); i++){
		freq = FUNDAMENTAL * lScale(input.height(), i);
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
			double pos = j * stepLength + A;
			for (int l=0; l<input.height(); l++){
				spl += amplitudes[l]*sine[int(frequencies[l]*pos)%CYCLELENGTH]; 
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
