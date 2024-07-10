// LvProto.h: Function prototypes of project

//-----------------------------------------------------------------------------
// Function prototypes
//-----------------------------------------------------------------------------
#define USE_FM_IRQ


#define _rom const
/* Radio.c */
BYTE AdjustVolume(BYTE byVolume);
BYTE SetFrequency(WORD wDisplayFreq);
WORD GetDisplayFrequency(WORD wSpacing);
BYTE SetRegion(BYTE byRegion);
BYTE ScanStation(BYTE byFlag);

/* RadioSub.c */
BYTE MeasureFrequency(WORD wSource, DWORD dwMeasureTimeUs, PWORD pwFreq);
WORD DisplayFreqToRf(WORD wDisp);
WORD RfToDisplayFreq(WORD wRf);
BYTE CheckRfLimit(PWORD pwCurFreq, WORD wRfBandLow, WORD wRfBandHigh, int iAdjust);
BYTE ScanFmRoutine(int iDir, WORD wFmBandLow, WORD wFmBandHigh);
BOOL FindFmStation(void);

/* HardwareIo.c */
void Init3wHardwareIo(void);
void Init3wHardwareIo_PowerOn(void);
void IoWrite3W(BYTE byLow, BYTE byHigh);
BYTE IoRead3W(BYTE byReg);
DWORD Pulse3wNRW(WORD wStart, WORD wStop, DWORD dwTimeUs);

#if defined(USE_FM_IRQ)
void Enable3wIrq(BOOL bState);//enable by Eric.wei
#endif

void IoWriteByte3W(BYTE byData);
void DelayUs(WORD wT0Cnt);
void _putbit(BYTE byValue, BYTE byFun, WORD byPin);
BYTE _getbit(BYTE byFun, WORD byPin);
void ConfigGpio(BYTE gpio_Num, DWORD gpRegister, BYTE shiftSt, BYTE dir, BYTE defaultLeve,BYTE dataUse);

BYTE GetFeatureValueType(BYTE byFeatureId);

BYTE CallBack(BYTE byCbReason);

/* LV24001.c */
BYTE InitLv2400xChip_PowerOn(void) ;
BYTE InitLv2400xChip(void);
void DeInitLv2400xChip(void);
BYTE GetFmFieldStrength(void);
BYTE GetStereoState(void);
BYTE GetHwFeatureLimit(BYTE byFeatureId);
BYTE GetHwFeatureValue(BYTE byFeatureId);
BYTE SetHwFeatureValue(BYTE byFeatureId, BYTE byValue);

#if defined(USE_FM_IRQ)
void EnableLv2400xIrq(void);
void DisableLv2400xIrq(void);
void HandleLv2400xIrq(void);
void AudioFeedback(BYTE byType);
#endif

BYTE Set_IF_Freq(WORD wInputIF);
BYTE Set_SD_Freq(WORD wInputSC);
BYTE GetOutputSelect(void);
WORD SetOutputSelect(WORD wOutputCfg);
WORD SetUpChipMode(WORD wCfgIn);
BYTE WriteIfOsc(WORD wIfOscValue); 
BYTE WriteSdOsc(WORD wSdOscValue);
BYTE WriteRfOsc(WORD wRfOscValue);
BYTE WriteRfCap(WORD wCapValue);
BYTE GetRfCapValue(void);
BYTE SetAFCMode(BYTE byEnable);
BYTE SetDeemphasis(BYTE byDeemp);
BYTE GetChipVolume(void);
void SetChipVolume(BYTE byLevel);
BYTE SetStereoMono(BOOL bStereo);
BYTE SetChipStereo(BYTE byStereo);
BYTE SetChipAudioMute(BYTE byMute, BYTE bySource);
WORD GetDividerFactor(void);
BYTE CountPulse(DWORD dwMeasureTimeUs, PWORD pwFreq);
BYTE CountPulseCnt1(DWORD dwMeasureTimeUs, PWORD pwFreq);

// 2005-03-03
void cTabSetting(DWORD dwMeasureTimeUs ,PDWORD pdwPulseCnt2,PWORD pwRegValue);
BYTE CountPulseCnt2(DWORD dwMeasureTimeUs, PWORD pwFreq);
DWORD PulseToFrequency2(DWORD dwCntA, DWORD dwCntB, DWORD dwFext);
/*
#ifdef USE_EXTCLK
BYTE CountPulseCnt2(PWORD pwFreq);
WORD PulseToFrequency2(WORD wCntA, WORD wCntB);
#endif //USE_EXTCLK
*/

void ShadowMeasuredFrequency(DWORD dwFreqHz);
WORD PulseToFrequency1(WORD wPulseCnt, DWORD dwMeasureTimeUs);
BYTE SetRfFrequency(WORD wRfFreq);
void AdjustHwToneControl(void);
void ShadowReg(WORD wRegAddress, BYTE byValue);
BYTE GetSwRegValue(WORD wRegAddress);
void SelRegBlock(BYTE byBlock);
int IsFrequencyOk(WORD wCurFreq, WORD wExpFreq, DWORD dwPrecise);

/* DgtParser.c */
void WriteReg(WORD wRegAddress, BYTE byValue);
BYTE ReadReg(WORD wRegAddress);
BYTE DriveBit(WORD wRegAddress, BYTE byBitMask, BYTE byState);
BYTE DriveBitNeg(WORD wRegAddress, BYTE byBitMask, BYTE byState);
BYTE SetRegBits(WORD wRegAddress, BYTE byValueMask, BYTE bySetValue);

/* TuneAlg.c */
BYTE SearchInByteArray(BYTE byToFind, BYTE _rom pbyArray[], BYTE byArraySize);
BYTE LinTuneDac(WORD wExpFreq, DWORD dwMeasureTimeUs, FPTR_WRITE_OSC fpWriteOsc, WORD wX1, WORD wX2, BYTE bPrecision);
WORD CalculateCoeff(WORD wFreq);
WORD InterpolateY(WORD wX, WORD wX1, WORD wX2, WORD wY1, WORD wY2);
WORD InterpolateX(WORD dwY, WORD wX1, WORD wX2, WORD wY1, WORD wY2);
int InterpolateOscStep(WORD dwY, WORD wX1, WORD wX2, WORD wY1, WORD wY2);
BYTE InitTuningRfCapOsc(void);
BYTE GetRfAtCapOsc(WORD wCapValue, WORD wOscValue, DWORD dwMsrTimeUs, PWORD pwFreq);
BYTE ScanSetFreq(WORD wTuneFreq, BYTE byPrecisionLevel);
BYTE FineTuneRfOsc(WORD wRfFreq, DWORD dwMeasureTimeUs, WORD wExpOscValue, BYTE byNeedPrecision);
BYTE CalculateRfCapOsc(WORD wFreq, PWORD pwCap, PWORD pwOsc, BYTE byPrecisionLevel);

/* -------------------------------EOF------------------------------------------ */
