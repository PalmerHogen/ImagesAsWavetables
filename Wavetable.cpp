#include "Wavetable.h"
#include <chrono>

using namespace std;
using namespace cimg_library;

//Pixel Brightness, in range 0-1000
double Wavetable::brightness(int i, int j){
    int r_index = w * j + i + r_offset;
    int g_index = w * j + i + g_offset;
    int b_index = w * j + i + b_offset;
	return (img_data[r_index] + img_data[b_index] + img_data[g_index])*1.3f;
	//return (img_data[r_index])*3.9f;
}

//Exponential Scaling between 1 and 1000 for startpoint 0 and endpoint h-1
double Wavetable::efScale(int h, int i){
	return double(i*i) * 1000.0f / double(h*h) + 1.0f;
}

double Wavetable::lfScale(int h, int i){
	return 999.0f/h*i+1;
}

Wavetable::Wavetable(int timestep, double gain, CImg<unsigned char> image) {
    this->cycle_length = SAMPLERATE;
    this->timestep = timestep;
    this->image = image;
    this->gain = gain;
    this->buffer_length = (SAMPLERATE * timestep) / 1000;
    this->bandCount = image.height();

    this->img_data = image.data();
    this->w = image.width();
    this->h = image.height();
    this->size = image.size();
    this->r_offset = 0;
    this->g_offset = w*h;
    this->b_offset = 2*w*h;

    //Optimizations: -----------------------------------------------
	//Floating point division is slow; so I'll do these upfront
	this->invH = 1.0f / bandCount;
	this->invC = 1.0f / cycle_length;	

	//math.sin() is also slow
	sinu = new double[cycle_length];
	for (int b=0; b<cycle_length; b++){
		sinu[b] = sin(2*M_PI*b*invC);
	}
	//--------------------------------------------------------------
	//Intermediate audio-write buffers
	this->audioBuffer = new short[buffer_length];
	amplitudes = new double[bandCount];
	frequencies = new double[bandCount]; // each value in [20, 20000]

	double freq;
	for (int i=0; i<bandCount; i++){
		freq = FUNDAMENTAL * efScale(bandCount, i);
		amplitudes[i] = 0.0f;
		frequencies[bandCount-i-1] = freq;
	}
}

Wavetable::~Wavetable() {
	delete [] sinu;
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
  
    double audioTime = 0; 
    double writeTime = 0;
 	for(int j=0; j<(image.width()); j++){

        if (j % progress_thresh == 0) {
            cout << ".]\b";
            //cout << j << " ]\b";
            cout.flush();
        }
		for (int k=0; k<bandCount; k++){
			amplitudes[k] = brightness(j, k)*gain*invH;
		}
		
		auto t1 = std::chrono::high_resolution_clock::now();
		for (int A=0; A<buffer_length; A++){
			double spl = 0.0f;
			long pos = j * buffer_length + A;
			for (int l=0; l<bandCount; l++){
				spl += amplitudes[l]*sinu[long(frequencies[l]*pos)%cycle_length]; 
			}
			audioBuffer[A] = spl;
		}
		auto t2 = std::chrono::high_resolution_clock::now();
		audioTime += std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
		//write intermediate results
		t1 = std::chrono::high_resolution_clock::now();
		writeWav(f, audioBuffer, buffer_length);
		t2 = std::chrono::high_resolution_clock::now();
		writeTime += std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
		 
	}
    
    cout << endl;
    std::cout << "Audio loop took: "
              << audioTime
              << " milliseconds\n";
    std::cout << "Write took: "
              << writeTime
              << " milliseconds\n"; 
       
	closeWav(f);

}
