#include "Wavetable.h"

using namespace std;
using namespace cimg_library;

//Pixel Brightness, in range 0-1000
double Wavetable::brightness(CImg<unsigned int> img, int i, int j){
	return (img(i, j, 0) + img(i, j, 1) + img(i, j, 2))*1.3f;
}

//Exponential Scaling between 1 and 1000 for startpoint 0 and endpoint h-1
double Wavetable::efScale(int h, int i){
	return double(i*i) * 1000.0f / double(h*h) + 1.0f;
}

double Wavetable::lfScale(int h, int i){
	return 999.0f/h*i+1;
}

Wavetable::Wavetable(int timestep, double gain, CImg<unsigned int> image) {
    this->cycle_length = SAMPLERATE;
    this->timestep = timestep;
    this->image = image;
    this->gain = gain;
    this->numSamples = (cycle_length * timestep) / 1000;
    this->numParts = image.height();
    //Optimizations: -----------------------------------------------
	//Floating point division is slow; so I'll do these upfront
	this->invH = 1.0f / numParts;
	this->invC = 1.0f / cycle_length;	

	//math.sin() is also slow
	sinu = new double[cycle_length];
	for (int b=0; b<cycle_length; b++){
		sinu[b] = sin(2*M_PI*b*invC);
	}
	//--------------------------------------------------------------
	//Intermediate audio-write buffers
	this->audioBuffer = new short[numSamples];
	amplitudes = new double[numParts];
	frequencies = new double[numParts]; // each value in [20, 20000]
	double fundamental = 20.0f;
	double freq;
	for (int i=0; i<numParts; i++){
		freq = fundamental * efScale(numParts, i);
		amplitudes[i] = 0.0f;
		frequencies[numParts-i-1] = freq;
	}
}

Wavetable::~Wavetable() {
	delete [] sinu;
	delete [] frequencies;
	delete [] amplitudes;
	delete [] audioBuffer;
}

void Wavetable::generateFromImage(char *path) {
	FILE *f = openWav(path); //prepares .wav format header 
    
    int progress_thresh=image.width()/100+1;
    cout << "[";
    cout << "]\b";
    cout.flush(); 

 	for(int j=0; j<(image.width()); j++){
        if (j % progress_thresh == 0) {
            cout << ".]\b";
            cout.flush();
        }
		for (int k=0; k<numParts; k++){
			amplitudes[k] = brightness(image, j, k)*gain*invH;
		}
		for (int A=0; A<numSamples; A++){
			double spl = 0.0f;
			long pos = j * numSamples + A;
			for (int l=0; l<numParts; l++){
				spl += amplitudes[l]*sinu[long(frequencies[l]*pos)%cycle_length]; 
			}
			audioBuffer[A] = spl;
		}
		//write intermediate results
		writeWav(f, audioBuffer, numSamples);
	}
    cout << endl;
	closeWav(f);
}