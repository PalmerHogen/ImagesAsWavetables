
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

class Wavetable {
    double efScale(int h, int i);
    double lfScale(int h, int i);
    double brightness(CImg<unsigned int> img, int i, int j);

	int cycle_length, timestep, numParts;
    long numSamples;
    double gain, invH, invC;
    CImg<unsigned int> image;
    double *sinu;
    short *audioBuffer;
    double *amplitudes;
    double *frequencies;
    
public:
    Wavetable(int timestep, double gain, CImg<unsigned int> image);
    ~Wavetable();
    void generateFromImage(char *path);
};
