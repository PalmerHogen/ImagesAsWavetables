# Images As Wavetables

This Project explores relationships between images and sound and was inspired in part by the visionary work of musician Richard David James.

Images (.jpg) are transformed into playable audio (.wav) files by interpreting their horizontal axis as the time domain, and their vertical axis as the frequency domain.

The relative volume of a frequency "y" at time "x" is determined by the brightness value of the pixel at location (x,y).

### Dependencies

This program depends on the Cool Image library in order to read images, the Intel SPMD program compiler for vectorization, and Thread Building Blocks for multithreading.

To install these dependencies using Homebrew,

```
brew install cimg
brew install ispc
brew install tbb
``` 

## Getting Started

To compile on a MacOS or Linux machine, use the provided makefile

Once compiled, the program can be executed with

```
./CONVERT [image] [audio] [time-step]
```

The program takes 3 arguments; the input filepath, output filepath, and a "time-step" value.

Input filenames must end in ".jpg", outputs must end in ".wav" – more filetypes may be supported in the future.

Time-step determines how many milliseconds of audio will be devoted to each column of the input image; in order to avoid writing an incorrect amount of samples, I have constrained this value to be larger than 10 ms per column.

