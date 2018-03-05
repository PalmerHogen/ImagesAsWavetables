CFLAGS=-std=c++11 -g -lm -I/opt/X11/include -L/opt/X11/lib -lX11 -ljpeg -lpthread -w -ltbb
CXX=g++

default: CONVERT CONVERTTBB

CONVERT: serialConversion.cpp WAV.c Wavetable.cpp Wavetable.h
	$(CXX) $(CFLAGS) serialConversion.cpp WAV.c Wavetable.cpp -o CONVERT 

CONVERTTBB: tbb_parallel_for_conversion.cpp WAV.c Wavetable.cpp Wavetable.h
	$(CXX) $(CFLAGS) tbb_parallel_for_conversion.cpp WAV.c Wavetable.cpp -o CONVERTTBB

clean:
	rm -rf CONVERT CONVERT.dSYM CONVERTTBB CONVERTTBB.dSYM
