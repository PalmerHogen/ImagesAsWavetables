#ifndef WAV_H
#define WAV_H

#include <stdio.h>
#include <inttypes.h>

FILE *openWav(const char *filename);
void writeWav(FILE *file, short data[], int length);
void closeWav(FILE * file);

#define SAMPLERATE 44100
#define BITS_PER_SAMPLE 16

#endif