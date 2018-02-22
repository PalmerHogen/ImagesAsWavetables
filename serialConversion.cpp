#define cimg_use_jpeg
#include "CImg.h"
#include "wavfile.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cmath>
#include <math.h>
#include <time.h>
#include <string.h>
#include <errno.h>

#define SAMPLERATE 44100
#define PI 3.14159265358979323846264338327f

using namespace std;
using namespace cimg_library;

double brightness(CImg<double> img, int i, int j){
	return (img(i, j, 0) + img(i, j, 1) + img(i, j, 2))*1.3f;
}

double T(int h, int i){
	return 1000.0f/double(h*h) * double(i*i) + 1.0f;
}

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
	double fract = double(timestep) * 0.001f;	

	CImg<double> input(argv[1]);
	//Floating point division is slow; so I'll do these upfront
	double invH = 1.0f / double(input.height());
	double invS = 1.0f / double(SAMPLERATE);	

	double *amplitudes = new double[input.height()];
	double *frequencies = new double[input.height()];
	
	double fundamental = 20.0f;
	double freq;

	for (int i=0; i<input.height(); i++){
		freq = fundamental * T(input.height(), i);
		amplitudes[i] = 0.0f;
		frequencies[input.height() - 1 - i] = freq;
	}

	int audioLength = input.width() * SAMPLERATE * fract;
	double len = double(audioLength);

	short *audio = new short[audioLength];
	int index = 0;

	for(int j=0; j<(input.width()); j++){
		for (int k=0; k<input.height(); k++){
			amplitudes[k] = brightness(input, j, k)*gain*invH;
		}
		for (int A=0; A<(SAMPLERATE*fract); A++){
			double amp = 0.0f;
			double cycle = double(index) * invS;
			for (int l=0; l<input.height(); l++){
				amp += amplitudes[l] * sin(2.0f*PI*frequencies[l]*cycle);
			}
			audio[index++] = amp;
		}

	}
	FILE * f = wavfile_open(argv[2]);
	wavfile_write(f, audio, audioLength);
	wavfile_close(f);

	delete amplitudes;
	delete frequencies;
	delete audio;
	return 0;
}
