CFLAGS=-std=c++11 -g -lm -I/opt/X11/include -L/opt/X11/lib
CLIBS=-lpthread -w -lX11 -ljpeg
CXX=g++
GCC=gcc 

default: CONVERT

CONVERT: serialConversion.cpp WAV.c Wavetable.cpp Wavetable.h
	$(CXX) $(CFLAGS) serialConversion.cpp WAV.c Wavetable.cpp -o CONVERT $(CLIBS)

clean:
	rm -rf CONVERT CONVERT.dSYM
