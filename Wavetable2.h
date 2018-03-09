
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
//TBB includes
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
    double brightness(CImg<unsigned char> img, int i, int j);

	int cycle_length, timestep, bandCount;
    long buffer_length, full_buffer_length;
    double gain, invH, invC;
    CImg<unsigned char> image;
    double *sine;
    short *audioBuffer;
    double *amplitudes;
    double *fullAmplitudes;
    double *frequencies;
    short *fullAudioBuffer;
    
public:
    Wavetable(int timestep, double gain, CImg<unsigned char> image);
    ~Wavetable();
    void writeAudio(char *path);
    void writeAudioParallelFor(char *path);
};
