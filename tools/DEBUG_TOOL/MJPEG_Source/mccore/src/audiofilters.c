/*************************************************************************
* MediaCoder - The universal audio/video transcoder
*
* Copyright (c) 2005-06 Stanley Huang <reachme@citiz.net>
* All rights reserved.
*************************************************************************/

#include <windows.h>

int dspDownmix(short* pwSamples, int iNumOfSamples)
{
	register int i;
	for (i = 0; i < iNumOfSamples; i += 2) {
		pwSamples[i >> 1] = (pwSamples[i] >> 1) + (pwSamples[i + 1] >> 1);
	}
	return iNumOfSamples >> 1;		//return number of samples in buffer
}

int dspLeftChannelMono(short* pwSamples, int iNumOfSamples)
{
	register int i;
	for (i = 0; i < iNumOfSamples / 2; i++) {
		pwSamples[i] = pwSamples[i * 2];
	}
	return iNumOfSamples >> 1;		//return number of samples in buffer
}

int dspRightChannelMono(short* pwSamples, int iNumOfSamples)
{
	register int i;
	for (i = 0; i < iNumOfSamples / 2; i++) {
		pwSamples[i] = pwSamples[i * 2 + 1];
	}
	return iNumOfSamples >> 1;		//return number of samples in buffer
}

int dspRightChannelStereo(short* pwSamples, int iNumOfSamples)
{
	register int i;
	for (i = 0; i < iNumOfSamples; i++) {
		pwSamples[i] = pwSamples[i + 1];
	}
	return iNumOfSamples;		//return number of samples in buffer
}

int dspLeftChannelStereo(short* pwSamples, int iNumOfSamples)
{
	register int i;
	for (i = 0; i < iNumOfSamples; i++) {
		pwSamples[i + 1] = pwSamples[i];
	}
	return iNumOfSamples;		//return number of samples in buffer
}
