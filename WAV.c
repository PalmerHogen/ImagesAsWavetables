#include "WAV.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct wav_header {
	char	riff_tag[4];
	int		riff_length;
	char	wave_tag[4];
	char	fmt_tag[4];
	int		fmt_length;
	short	audio_format;
	short	num_channels;
	int		sample_rate;
	int		byte_rate;
	short	block_align;
	short	bits_per_sample;
	char	data_tag[4];
	int		data_length;
};

FILE *openWav(const char *filename){
	struct wav_header header;
	strncpy(header.riff_tag,"RIFF",4);
	strncpy(header.wave_tag,"WAVE",4);
	strncpy(header.fmt_tag,"fmt ",4);
	strncpy(header.data_tag,"data",4);
	header.riff_length = 0;
	header.fmt_length = 16;
	header.audio_format = 1;
	header.num_channels = 1;
	header.sample_rate = SAMPLERATE;
	header.byte_rate = SAMPLERATE*(BITS_PER_SAMPLE/8);
	header.block_align = BITS_PER_SAMPLE/8;
	header.bits_per_sample = BITS_PER_SAMPLE;
	header.data_length = 0;
	FILE * file = fopen(filename,"wb+");
	if(!file){
		return 0;
	}
	fwrite(&header,sizeof(header),1,file);
	fflush(file);
	return file;
}

void writeWav(FILE *file, short data[], int length){
	fwrite(data,sizeof(short),length,file);
}

void closeWav(FILE *file){
	int file_length = ftell(file);
	int data_length = file_length - sizeof(struct wav_header);
	fseek(file,sizeof(struct wav_header) - sizeof(int),SEEK_SET);
	fwrite(&data_length,sizeof(data_length),1,file);
	int riff_length = file_length - 8;
	fseek(file,4,SEEK_SET);
	fwrite(&riff_length,sizeof(riff_length),1,file);
	fclose(file);
}