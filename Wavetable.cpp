#include "Wavetable.h"
#include "Wavetable_ispc.h"
#include <pthread.h>

using namespace std;
using namespace cimg_library;

//Pixel Brightness, in range 0-1000
double Wavetable::brightness(CImg<unsigned char> img, int i, int j){
	return (img(i, j, 0)*img(i, j, 0) + img(i, j, 1)*img(i, j, 1) + img(i, j, 2)*img(i, j, 2))/3;
}

//Exponential Scaling between 1 and 1000 for startpoint 0 and endpoint h-1
double Wavetable::efScale(int h, int i){
	return double(i*i) * 999.0f / double(h*h) + 1.0f;
}
//Linear Scaling between 1 and 1000 for startpoint 0 and end endpoint h-1
double Wavetable::lfScale(int h, int i){
	return 999.0f/h*i+1;
}

//non-linear anti-logarithmic scale between 1 and 1000 for startpoint 0 and endpoint h-1
double Wavetable::nfScale(int h, int i){
	double n = pow(1000.0f, (1.0f/double(h-1)));
	return pow(n, double(i));
}

Wavetable::Wavetable(int timestep, CImg<unsigned char> image) {
    this->cycle_length = SAMPLERATE;
    this->timestep = timestep;
    this->image = image;
    this->imageData = (this->image).data();
    this->buffer_length = (SAMPLERATE * timestep) / 1000;
    this->full_buffer_length = buffer_length * image.width();
    this->bandCount = image.height();
    //Optimizations: -----------------------------------------------
	//Floating point division is slow; so I'll do these upfront
	this->invH = 1.0f / bandCount;
	this->invC = 1.0f / cycle_length;	

	//math.sin() is also slow
	this->sine = new double[cycle_length];
	for (int b=0; b<cycle_length; b++){
		this->sine[b] = sin(2*M_PI*b*invC);
	}
	//--------------------------------------------------------------
	//Intermediate audio-write buffers
	this->audioBuffer = new short[buffer_length];
	this->fullAudioBuffer = new short[full_buffer_length];
	this->amplitudes = new double[bandCount];
	this->fullAmplitudes = new double[bandCount*image.width()];
	this->frequencies = new double[bandCount]; // each value in [20, 20000]

    for(int i=0; i < full_buffer_length; i++) {
        fullAudioBuffer[i] = 0;
    }
    for(int i=0; i < bandCount*image.width(); i++) {
        fullAmplitudes[i] = 0;
    }

	double freq;
	for (int i=0; i<bandCount; i++){
		freq = FUNDAMENTAL * nfScale(bandCount, i);
		this->amplitudes[i] = 0.0f;
		this->frequencies[bandCount-i-1] = freq;
	}
}

Wavetable::~Wavetable() {
	delete [] sine;
	delete [] frequencies;
	delete [] amplitudes;
	delete [] audioBuffer;
}

void Wavetable::writeAudio(char *path) {
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
		for (int k=0; k<bandCount; k++){
			amplitudes[k] = brightness(image, j, k)*invH;
		}
		for (int A=0; A<buffer_length; A++){
			double spl = 0.0f;
			long pos = j * buffer_length + A;
			for (int l=0; l<bandCount; l++){
				spl += amplitudes[l]*sine[long(frequencies[l]*pos)%cycle_length]; 
			}
			audioBuffer[A] = spl;
		}
		//write intermediate results
		writeWav(f, audioBuffer, buffer_length);
	}
    cout << endl;
	closeWav(f);
}

void Wavetable::writeAudioParallelFor(char *path) {
	FILE *f = openWav(path); //prepares .wav format header 
    
    tbb::parallel_for(size_t(0), size_t(image.width()), [&](size_t j) {
        int full_buffer_offset = buffer_length * (int)j;
        int full_amplitudes_offset = bandCount * (int)j;
		for (int k=0; k<bandCount; k++){
			fullAmplitudes[k+full_amplitudes_offset] = brightness(image, j, k)*invH;
		}
		for (int A=0; A<buffer_length; A++){
			double spl = 0.0f;
			long pos = (int)j * buffer_length + A;
			for (int l=0; l<bandCount; l++){
				spl += fullAmplitudes[l+full_amplitudes_offset]*sine[long(frequencies[l]*pos)%cycle_length]; 
			}
			fullAudioBuffer[A+full_buffer_offset] = spl;
		}
	});
    writeWav(f, fullAudioBuffer, full_buffer_length);
    cout << endl;
	closeWav(f);
}

void Wavetable::writeAudioParallelForIspc(char *path) {
	FILE *f = openWav(path); //prepares .wav format header 
    
    tbb::parallel_for(size_t(0), size_t(image.width()), [&](size_t j) {
        int full_buffer_offset = buffer_length * (int)j;
        int full_amplitudes_offset = bandCount * (int)j;

        int r_offset = 0;
        int g_offset = image.width()*bandCount;
        int b_offset = 2*g_offset;
        unsigned char* imgr = imageData + r_offset;
        unsigned char* imgg = imageData + g_offset;
        unsigned char* imgb = imageData + b_offset;
        ispc::computeAmplitudes(fullAmplitudes, imgr, imgg, imgb, 
				
				
				
				, invH, full_amplitudes_offset, (int)j, (int)image.width(), bandCount); 
		for (int A=0; A<buffer_length; A++){
			double spl = 0.0f;
			long pos = (int)j * buffer_length + A;
			for (int l=0; l<bandCount; l++){
				spl += fullAmplitudes[l+full_amplitudes_offset]*sine[long(frequencies[l]*pos)%cycle_length]; 
			}
			fullAudioBuffer[A+full_buffer_offset] = spl;
		}
	});
    writeWav(f, fullAudioBuffer, full_buffer_length);
    cout << endl;
	closeWav(f);
}


