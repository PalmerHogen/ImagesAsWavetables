CFLAGS=-std=c++11 -g -lm -I/opt/X11/include -L/opt/X11/lib
CLIBS=-lpthread -w -lX11 -ljpeg
CXX=g++
ICC=ispc

default: CONVERT


ISPC_Maps.o: ISPC_Maps.ispc
	$(ICC) -o ISPC_Maps.o -h ISPC_Maps.h ISPC_Maps.ispc --quiet

CONVERT: serialConversion.cpp WAV.c Wavetable.cpp Wavetable.h
	$(CXX) $(CFLAGS) serialConversion.cpp WAV.c Wavetable.cpp -o CONVERT $(CLIBS)

clean:
	rm -rf CONVERT CONVERT.dSYM ISPC_Maps.o ISPC_Maps.h
