#ifdef __cplusplus
extern "C" {
#endif

int dspDownmix(short* pwSamples, int iNumOfSamples);
int dspLeftChannelMono(short* pwSamples, int iNumOfSamples);
int dspRightChannelMono(short* pwSamples, int iNumOfSamples);
int dspRightChannelStereo(short* pwSamples, int iNumOfSamples);
int dspLeftChannelStereo(short* pwSamples, int iNumOfSamples);

#ifdef __cplusplus
}
#endif
