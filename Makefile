CFLAGS=-std=c++11 -g -lm -I/opt/X11/include -L/opt/X11/lib -lX11 -ljpeg -lpthread -w
CXX=g++
GCC=gcc 

default: CONVERT

CONVERT: serialConversion.cpp wavfile.c
	$(CXX) $(CFLAGS) serialConversion.cpp wavfile.c -o CONVERT

clean:
	rm -rf CONVERT CONVERT.dSYM
