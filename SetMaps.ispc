export void setAmps(uniform double amplitudes[],
					uniform unsigned int imgr[],
					uniform unsigned int imgg[],
					uniform unsigned int imgb[],
					uniform double invH,
					uniform int width, uniform int height){

	foreach(i=0 ... width*height){
		amplitudes[i] = ((imgr[i]*imgr[i] + imgg[i]*imgg[i] + imgb[i]*imgb[i])/3.0f)*invH;
	}
}