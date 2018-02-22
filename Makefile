CFLAGS=-std=c++11 -g -lm -I/opt/X11/include -L/opt/X11/lib -lX11 -ljpeg -lpthread -w
CXX=g++
GCC=gcc 

default: CONVERT

CONVERT: serialConversion.cpp WAV.c
	$(CXX) $(CFLAGS) serialConversion.cpp WAV.c -o CONVERT

clean:
	rm -rf CONVERT CONVERT.dSYM
