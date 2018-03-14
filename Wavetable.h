
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

#include "tbb/tbb.h"
#include "tbb/parallel_for.h"
#include "tbb/blocked_range.h"

#define FUNDAMENTAL 20.0f

using namespace std;
using namespace cimg_library;

class Wavetable {
    double efScale(int h, int i);
    double lfScale(int h, int i);
    double nfScale(int h, int i);
    double brightness(CImg<unsigned int> img, int i, int j);
    double aLerp(int col, int h, int t);

	int cycle_length, timestep, bandCount;
    int step_length;
    double invH, invC;
    CImg<unsigned int> image;
    unsigned int* imageData;
    double *sine;
    short *audioBuffer;
    double *amplitudes;
    double *frequencies;
    
public:
    Wavetable(int timestep, CImg<unsigned int> image);
    ~Wavetable();
    void writeAudio(char *path);
};
