#CFLAGS=-std=c++11 -g -lm -I/opt/X11/include -L/opt/X11/lib -lX11 -ljpeg -lpthread -w -ltbb
#CFLAGS= -O3 -std=c++11 -g -lm -I/opt/X11/include -L/opt/X11/lib -lX11 -ljpeg -lpthread -w -ltbb
CFLAGS= -std=c++11 -g -lm -lX11 -ljpeg -lpthread -w -ltbb
CXX=g++ -m64
#ISPCFLAGS= -O2 --arch=x86-64 --target=sse2 --math-lib=system
ISPCFLAGS=--arch=x86-64 --target=avx --math-lib=system

default: wavetable-ispc CONVERT CONVERTTBB CONVERT2 CONVERTTBB2 CONVERTTBBISPC

CONVERT: serialConversion.cpp WAV.c Wavetable.cpp Wavetable.h
	$(CXX) $(CFLAGS) serialConversion.cpp WAV.c Wavetable.cpp Wavetable_ispc.o -o CONVERT 

CONVERT2: serialConversion.cpp WAV.c Wavetable.cpp Wavetable.h
	$(CXX) $(CFLAGS) serialConversion2.cpp WAV.c Wavetable2.cpp Wavetable_ispc.o -o CONVERT2

CONVERTTBB: tbb_parallel_for_conversion.cpp WAV.c Wavetable.cpp Wavetable.h
	$(CXX) $(CFLAGS) tbb_parallel_for_conversion.cpp WAV.c Wavetable.cpp Wavetable_ispc.o -o CONVERTTBB

CONVERTTBB2: tbb_parallel_for_conversion.cpp WAV.c Wavetable.cpp Wavetable.h
	$(CXX) $(CFLAGS) tbb_parallel_for_conversion2.cpp WAV.c Wavetable2.cpp Wavetable_ispc.o -o CONVERTTBB2

CONVERTTBBISPC: wavetable-ispc tbb_parallel_for_conversion.cpp WAV.c Wavetable.cpp Wavetable.h Wavetable_ispc.o
	$(CXX) $(CFLAGS) tbb_parallel_for_conversion_ispc.cpp WAV.c Wavetable.cpp Wavetable_ispc.o -o CONVERTTBBISPC

wavetable-ispc: Wavetable.ispc
	ispc $(ISPCFLAGS) -o Wavetable_ispc.o -h Wavetable_ispc.h Wavetable.ispc

clean:
	rm -rf CONVERT CONVERT.dSYM CONVERTTBB CONVERTTBB.dSYM CONVERT2 CONVERT2.dSYM CONVERTTBB2 CONVERTTBB2.dSYM CONVERTTBBISPC CONVERTTBBISPC.dSYM
