#include "ImageWaveConverter.h"

using namespace std;
using namespace cimg_library;

//Pixel Brightness, in range 0-1000
double ImageWaveConverter::brightness(CImg<unsigned int> img, int i, int j){
	return (img(i, j, 0) + img(i, j, 1) + img(i, j, 2))*1.3f;
}

//Exponential Scaling between 1 and 1000 for startpoint 0 and endpoint h-1
double ImageWaveConverter::expInterp(int h, int i){
	return 1000.0f/(h*h) * (i*i) + 1;
}

double ImageWaveConverter::linInterp(int h, int i){
	return 999.0f/h*i+1;
}

ImageWaveConverter::ImageWaveConverter(int timestep, double gain, CImg<unsigned int> image) {
    this->samplerate = SAMPLES_PER_SECOND;
    this->timestep = timestep;
    this->image = image;
    this->gain = gain;
    this->numSamples = (samplerate * timestep) / 1000;
    this->numParts = image.height();
    //Optimizations: -----------------------------------------------
	//Floating point division is slow; so I'll do these upfront
	this->invH = 1.0f / numParts;
	this->invS = 1.0f / samplerate;	

	//math.sin() is also slow
	sinu = new double[samplerate];
	for (int b=0; b<samplerate; b++){
		sinu[b] = sin(2*M_PI*b*invS);
	}
	//--------------------------------------------------------------
	//Intermediate audio-write buffers
	this->audioBuffer = new short[numSamples];
	amplitudes = new double[numParts];
	frequencies = new int[numParts]; // each value in [20, 20000]
	int fundamental = 20;
	int freq;
	for (int i=0; i<numParts; i++){
		freq = fundamental * expInterp(numParts, i);
		amplitudes[i] = 0.0f;
		frequencies[numParts-i-1] = freq;
	}
}

ImageWaveConverter::~ImageWaveConverter() {
	delete [] sinu;
	delete [] frequencies;
	delete [] amplitudes;
	delete [] audioBuffer;
}

void ImageWaveConverter::convert(char *path) {
	FILE *f = openWav(path); //prepares .wav format header 
    
    int progress_thresh=image.width()/100+1;
    cout << "[";
    for (int i = 0; i < 100; i++) {
        cout << ' ';
    }
    cout << "]\b";
    for (int i = 0; i < 100; i++) {
        cout << '\b';
    }
    cout.flush(); 

 	for(int j=0; j<(image.width()); j++){
        if (j % progress_thresh == 0) {
            cout << ".";
            cout.flush();
        }
		for (int k=0; k<numParts; k++){
			amplitudes[k] = brightness(image, j, k)*gain*invH;
		}
		for (int A=0; A<numSamples; A++){
			double spl = 0.0f;
			long pos = j * numSamples + A;
			for (int l=0; l<numParts; l++){
				spl += amplitudes[l]*sinu[(frequencies[l]*pos)%samplerate]; 
			}
			audioBuffer[A] = spl;
		}
		//write intermediate results
		writeWav(f, audioBuffer, numSamples);
	}
    cout << endl;
	closeWav(f);
}

