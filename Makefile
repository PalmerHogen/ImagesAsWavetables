CFLAGS=-std=c++11 -lm -I/opt/X11/include -L/opt/X11/lib -lX11 -ljpeg -lpthread -ltbb -w
CXX=g++
ICC=ispc

default: CONVERTTBBISPC2

CONVERTTBBISPC2: convert.cpp WAV.c Wavetable.cpp Wavetable.h ISPC_Maps.o
	$(CXX) convert.cpp WAV.c Wavetable.cpp ISPC_Maps.o -o CONVERTTBBISPC2 $(CFLAGS)

ISPC_Maps.o: ISPC_Maps.ispc
	$(ICC) -o ISPC_Maps.o -h ISPC_Maps.h ISPC_Maps.ispc --quiet

clean:
	rm -rf CONVERTTBBISPC2 CONVERTTBBISPC2.dSYM ISPC_Maps.o ISPC_Maps.h
