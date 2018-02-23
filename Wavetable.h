
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

#define FUNDAMENTAL 20.0f

using namespace std;
using namespace cimg_library;

class Wavetable {
    double efScale(int h, int i);
    double lfScale(int h, int i);
    double brightness(int i, int j);

	int cycle_length, timestep, bandCount;
    long buffer_length;
    double gain, invH, invC;
    CImg<unsigned char> image;
    unsigned char *img_data;
    int size, w, h, r_offset, b_offset, g_offset;
    double *sinu;
    short *audioBuffer;
    double *amplitudes;
    double *frequencies;
    
public:
    Wavetable(int timestep, double gain, CImg<unsigned char> image);
    ~Wavetable();
    void writeAudio(char *path);
};
