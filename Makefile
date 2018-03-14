CFLAGS=-std=c++11 -lm -I/opt/X11/include -L/opt/X11/lib -lX11 -ljpeg -lpthread -ltbb -w
CXX=g++
ICC=ispc

default: CONVERT

CONVERT: convert.cpp WAV.c Wavetable.cpp Wavetable.h ISPC_Maps.o
	$(CXX) convert.cpp WAV.c Wavetable.cpp ISPC_Maps.o -o CONVERT $(CFLAGS)

ISPC_Maps.o: ISPC_Maps.ispc
	$(ICC) -o ISPC_Maps.o -h ISPC_Maps.h ISPC_Maps.ispc --quiet

clean:
	rm -rf CONVERT CONVERT.dSYM ISPC_Maps.o ISPC_Maps.h
