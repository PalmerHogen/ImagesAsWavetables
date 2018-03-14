CFLAGS=-std=c++11 -g -lm -I/opt/X11/include -L/opt/X11/lib -lX11 -ljpeg -lpthread -w -ltbb
CXX=g++

default: CONVERT

CONVERT: convert.cpp WAV.c Wavetable.cpp Wavetable.h SetMaps.o
	$(CXX) $(CFLAGS) convert.cpp WAV.c Wavetable.cpp SetMaps.o -o CONVERT 

SetMaps.o: SetMaps.ispc
	./ispc -o SetMaps.o -h SetMaps.h SetMaps.ispc --quiet

clean:
	rm -rf CONVERT CONVERT.dSYM SetMaps.o SetMaps.h
