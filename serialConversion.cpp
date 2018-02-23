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
double T(int h, int i){
	//return 1000.0f/double(h*h) * double(i*i) + 1.0f;
	return 1000.0f/(h*h) * (i*i) + 1;
	//return 1000.0f/h * i + 1;
}

double linInterp(int h, int i){
	return 999.0f/h*i+1;
}

int main(int argc, char **argv){
	int SAMPLERATE = SAMPLES_PER_SECOND;

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

	int numSamples = (SAMPLERATE * timestep) / 1000;
    int numParts = input.height();

	//Optimizations: -----------------------------------------------
	//Floating point division is slow; so I'll do these upfront
	double invH = 1.0f / numParts;
	double invS = 1.0f / SAMPLERATE;	

	//math.sin() is also slow
	double *sinu = new double[SAMPLERATE];
	for (int b=0; b<SAMPLERATE; b++){
		sinu[b] = sin(2*PI*b*invS);
	}
	//--------------------------------------------------------------
	double *amplitudes = new double[numParts];
	int *frequencies = new int[numParts]; // each value in [20, 20000]
	
	int fundamental = 20;
	int freq;

	for (int i=0; i<numParts; i++){
		freq = fundamental * T(numParts, i);
		amplitudes[i] = 0.0f;
		frequencies[numParts - 1 - i] = freq;
        //cout << freq << endl;
	}

	//Intermediate audio-write buffer
	short *audioBuffer = new short[numSamples];

	FILE *f = openWav(argv[2]);
    int progress_thresh=input.width()/100+1;
    cout << "[";
    for (int i = 0; i < 100; i++) {
        cout << ' ';
    }
    cout << "]\b";
    for (int i = 0; i < 100; i++) {
        cout << '\b';
    }
    cout.flush(); 

	for(int j=0; j<(input.width()); j++){
        if (j % progress_thresh == 0) {
            cout << ".";
            cout.flush();
        }
		for (int k=0; k<numParts; k++){
			amplitudes[k] = brightness(input, j, k)*gain*invH;
            //cout << amplitudes[k] << endl;
		}
		for (int A=0; A<numSamples; A++){
			double spl = 0.0f;
			long pos = j * numSamples + A;
			for (int l=0; l<numParts; l++){
				spl += amplitudes[l]*sinu[(frequencies[l]*pos)%SAMPLERATE]; 
			}
			audioBuffer[A] = spl;
		}
		//write intermediate results
		writeWav(f, audioBuffer, numSamples);

	}
    cout << endl;
	closeWav(f);

	delete amplitudes;
	delete frequencies;
	delete audioBuffer;
	delete sinu;
	return 0;
}
