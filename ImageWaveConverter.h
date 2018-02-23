#define _USE_MATH_DEFINES
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

using namespace std;
using namespace cimg_library;

class ImageWaveConverter {
    double expInterp(int h, int i);
    double linInterp(int h, int i);
    double brightness(CImg<unsigned int> img, int i, int j);

	int samplerate, timestep, numParts;
    long numSamples;
    double gain, invH, invS;
    CImg<unsigned int> image;
    double *sinu;
    short *audioBuffer;
    double *amplitudes;
    int *frequencies;
    
public:
    ImageWaveConverter(int timestep, double gain, CImg<unsigned int> image);
    ~ImageWaveConverter();
    void convert(char *path);
};


