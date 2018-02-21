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
	return (img(i, j, 0) + img(i, j, 1) + img(i, j, 2));
}

int main(int argc, char **argv){
	int Z = 0;
	if (argc < 3){
		cout << "Usage: ./CONVERT [image path] [output name]"<<endl;
		exit(2);
	}

	CImg<double> input(argv[1]);

	double *amplitudes = new double[input.height()];
	double *frequencies = new double[input.height()];

	double freq = 20.0f;
	double step = 1000.0f * exp(1.0f / double(input.height()));


	for (int i=0; i<input.height(); i++){
		amplitudes[i] = 0.0f;
		frequencies[i] = freq;
		freq *= step;
	}

	int audioLength = input.width() * SAMPLERATE;
	double len = double(audioLength);

	short audio[audioLength];
	int index = 0;

	for(int j=0; j<input.width(); j++){
		for (int k=0; k<input.height(); k++){
			amplitudes[k] = brightness(input, j, k);
		}
		for (int A=0; A<SAMPLERATE; A++){
			double amp = 0.0f;
			double cycle = double(index) / double(SAMPLERATE);
			for (int l=0; l<input.height(); l++){
				amp += amplitudes[l] * sin(2*PI*frequencies[l]*double(A)*cycle/len);
			}
			audio[index++] = amp;
			//cout<< amp << endl;
		}

	}
	FILE * f = wavfile_open(argv[2]);
	wavfile_write(f, audio, audioLength);
	wavfile_close(f);

	delete amplitudes;
	delete frequencies;
	//delete audio; 
	return 0;
}