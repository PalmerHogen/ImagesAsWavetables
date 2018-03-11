export void computeAmplitudes(  uniform double amplitudes[],
                                uniform unsigned int8 imgr[],
                                uniform unsigned int8 imgg[],
                                uniform unsigned int8 imgb[],
                                uniform double invH,
                                uniform int full_amplitudes_offset, uniform int j, //j is this columns index
                                uniform int width, uniform int bandCount) { //bandCount is height

        foreach (k = 0 ... bandCount) {
            int index = width * k + j;
			amplitudes[(int)k+full_amplitudes_offset] = ((double)imgr[index]*(double)imgr[index] + (double)imgg[index]*(double)imgg[index] + (double)imgb[index]*(double)imgb[index])/3*invH;
		}
}
