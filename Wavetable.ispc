export void computeAmplitudes(  uniform double amplitudes[],
                                uniform unsigned int8 imgr[],
                                uniform unsigned int8 imgg[],
                                uniform unsigned int8 imgb[],
                                uniform double gain, uniform double invH,
                                uniform int full_amplitudes_offset, uniform int j, //j is this columns index
                                uniform int width, uniform int bandCount) { //bandCount is height

        foreach (k = 0 ... bandCount) {
            int index = width * k + j;
			fullAmplitudes[k+full_amplitudes_offset] = (imgr[index] + imgg[index] + imgb[index])*1.3*gain*invH;
		}
}
