#include "Wavetable.h"
#include "ISPC_Maps.h"

using namespace std;
using namespace cimg_library;

//Linear interpolation between amplitudes to prevent missed zero-crossings
double Wavetable::aLerp(int col, int h, int t){
	double fract = (double) t / 100.0f;
	if (t < 100){
		if (col ==0){
			return (fract) * amplitudes[this->image.width()*h + col];
		}
		else{
			return (fract) * amplitudes[this->image.width()*h + col] + (1.0f - fract) * amplitudes[this->image.width()*h + col - 1];
		}
	}
	else{
		return amplitudes[this->image.width()*h + col];
	}
}

//Pixel Brightness, in range 0-1000
double Wavetable::brightness(CImg<unsigned int> img, int i, int j){
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
	return pow(pow(1000.0f, (1.0f/double(h-1))), double(i));
}

Wavetable::Wavetable(int timestep, CImg<unsigned int> image) {
    this->cycle_length = SAMPLERATE;
    this->timestep = timestep;
    this->image = image;
    this->step_length = (SAMPLERATE * timestep) / 1000;
    this->bandCount = image.height();
    //Optimizations: -----------------------------------------------
	//Floating point division is slow; so I'll create these constants upfront
	this->invH = 1.0f / bandCount;
	this->invC = 1.0f / cycle_length;	

	this->imageData = (this->image).data();

	int r_off, g_off, b_off;
	r_off = 0;
	g_off = image.width() * bandCount;
	b_off = 2*g_off;
	//Extract RGB channels from image data
	unsigned int *imgr, *imgg, *imgb;
	imgr = imageData + r_off;
	imgg = imageData + g_off;
	imgb = imageData + b_off;

	//math sin() is also relatively slow for large input value; 
	//I would rather index into an arbitrarily accurate array
	this->sine = new double[cycle_length];
	for (int b=0; b<cycle_length; b++){
		this->sine[b] = sin(2*M_PI*b*invC);
	}
	//--------------------------------------------------------------
	//Audio-write buffers
	this->audioBuffer = new short[step_length*image.width()];
	this->amplitudes = new double[image.width()*bandCount];
	this->frequencies = new double[bandCount]; // each value in [20, 20000]

	//Vectorized upfront Maps
	ispc::setFreqs(frequencies, FUNDAMENTAL, bandCount);
	ispc::setAmps(amplitudes, imgr, imgg, imgb, invH, image.width(), bandCount);
}

Wavetable::~Wavetable() {
	delete [] sine;
	delete [] frequencies;
	delete [] amplitudes;
	delete [] audioBuffer;
}

void Wavetable::writeAudio(char *path) {
	//Threading across Columns
	tbb::parallel_for(size_t(0), size_t(image.width()), [&](size_t j) {
		for (int A=0; A<step_length; A++){
			double spl = 0.0f;
			long pos = j * step_length + A;
			for (int l=0; l<bandCount; l++){
				spl += aLerp(j, l, A)*sine[long(frequencies[l]*pos)%cycle_length]; 
			}
			audioBuffer[pos] = spl;
		}
	});
	FILE *f = openWav(path);
    writeWav(f, audioBuffer, step_length*image.width());
	closeWav(f);
}