CFLAGS=-std=c++11 -g -lm -I/opt/X11/include -L/opt/X11/lib -lX11 -ljpeg -lpthread -w
CXX=g++

default: CONVERT

CONVERT: convert.cpp WAV.c Wavetable.cpp Wavetable.h
	$(CXX) $(CFLAGS) convert.cpp WAV.c Wavetable.cpp -o CONVERT 

clean:
	rm -rf CONVERT CONVERT.dSYM
