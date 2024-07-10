/*****************************************************************************
** File Name:      youngtong_tts_interface.c                                       *
** Author:                                                                   *
** Date:           2007.5.8                                                  *
** Copyright:      All Rights Reserved.                                      *
** Description:    Youngtong tts                                             *
*****************************************************************************/


#include <string.h>

#ifndef YT_TTS_TWO_RES_FILE
#include "youngtone_tts_interface.h"
#else
#include "youngtone_tts_interface_6800.h"
#endif

//////////////////////////////////////////
#define YTV_MAX_FRAME_SAMPLE_NUMBER_2 500
#define YT_TTS_FINAL_RELEASE

  

extern void YT_TTS_MTK_SET_STATE(int nState);
extern int YT_TTS_MTK_GET_STATE(void);

extern void YTV_ClearFrameNumber(void);
extern unsigned int CallYTV_GetLabelNumber(void);


extern void yt_global_start_synthesis(void);
extern char yt_global_get_synthesis_flag(void);
extern void yt_global_stop_synthesis(void);

extern void YT_SetCallbackFuncForDebug(long pFunc);
extern void YT_SetCallbackForOpenFile(long pFunc_OpenFile);
extern void YT_SetCallbackForReadFile(long pFunc_ReadFile);
extern void YT_SetCallbackForCloseFile(long pFunc_ReadFile);
extern void YT_SetCallbackForTellFile(long pFunc_ReadFile);
extern void YT_SetCallbackForSeekFile(long pFunc_ReadFile);


extern int YT_MM_SetMemoryBuffer(char *pMemoryBuffer, unsigned int nBufSize);
extern char *YT_MM_GetMemoryBuffer(unsigned int *pAllSize);
extern unsigned int YT_MM_GetOffsetForMisc(unsigned int *pBufSize_MISC);


extern void CallYTV_ResetFuncPointers(void);
extern void CallYTV_ResetFilterBuffer(void);
extern void CallYTV_SetRateFactor(unsigned int nFactor);
extern void CallYTV_SetPitchFactor(unsigned int nFactor);
extern int CallYTV_SynthesizeSpeechForLoop(int nStage, 
										   long pFunction, 
										   unsigned int *pFrameNumberForLoop, 
										   char *pFlagForUttFinal);
extern void CallYTV_GetSpeechFrame(int *pSpeechFrame_INT, 
								   int nFrameIndex,
								   int *pSampleNumber);





///////////////////////////////////////////////////////////////////////////////
//////////////////////// US ENGLISH SECTION //////////////////////////////////
#ifdef YT_LANG_ID_US_ENGLISH
	extern int YT_ENG_TTS_Initialize(char *strArg1, int nVoiceID,char *strArg2);						  						  
	extern int YT_ENG_TTS_InputText_Unicode(unsigned short *strText_Unicode,
												   unsigned int nCharNumber,
												   unsigned int DATE_FORMAT,
												   unsigned int TEXT_TYPE);
	extern int YT_ENG_TTS_PlayText(long pFunction, char charReserved);
						

	extern void YT_ENG_TTS_FreeResource(void);								
#endif 
//////////////////////////////////////////////////////////////////////////////




///////////////////////////////////////////////////////////////////////////////
//////////////////////// HINDI SECTION ///////////////////////////////////////

#ifdef YT_LANG_ID_HINDI
	extern int YT_HIN_TTS_Initialize(char *strArg1, unsigned int nVoiceID, char *strArg2);
	extern int YT_HIN_TTS_InputText_Unicode(unsigned short *strText_Unicode, 
											unsigned int nCharNumber,
											unsigned int DATE_FORMAT, 
											unsigned int TEXT_TYPE);
	extern int YT_HIN_TTS_PlayText(long pFunction,char charReserved);
	extern void YT_HIN_TTS_FreeResource(void);						
#endif
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
//////////////////////// BAHASA INDONESIAN SECTION ////////////////////////////
#ifdef YT_LANG_ID_INDONESIAN
	extern int YT_BAHASA_TTS_Initialize(char *strArg1, int nVoiceID,char *strArg2);						  
	extern int YT_BAHASA_TTS_InputText_Unicode(unsigned short *strText_Unicode,
												   unsigned int nCharNumber,
												   unsigned int DATE_FORMAT,
												   unsigned int TEXT_TYPE);


	extern int YT_BAHASA_TTS_PlayText(long pFunction,char charReserved);
extern void YT_BAHASA_TTS_FreeResource(void);	
#endif 
///////////////////////////////////////////////////////////////////////////////

													 						  


///////////////////////////////////////////////////////////////////////////////
//////////////////////// CHINESE MANDARIN SECTION ////////////////////////////
#ifdef YT_LANG_ID_MANDARIN
	extern int YT_MAN_TTS_Init(char *strArg1, int nVoiceID,char *strArg2);						  
	extern int YT_MAN_TTS_InputText_UTF16(unsigned short *strText_Unicode,
												   unsigned int nCharNumber,
												   unsigned int DATE_FORMAT,
												   unsigned int TEXT_TYPE);


	extern int YT_MAN_TTS_PlayText(long pFunction,char charReserved);
	extern void YT_MAN_TTS_FreeResource(void);						
#endif
///////////////////////////////////////////////////////////////////////////////
static int YT_CUR_LANG_ID = 0;
static int YT_CUR_VOICE_ID = 0;

static int YT_CUR_TTS_RATE = 100;
static int YT_CUR_TTS_PITCH = 100;
//static int YT_CUR_SAMPLING_RATE = 16000;
static int YT_CUR_TTS_MAX_VOLUME = 32768;

////////////////////////////////////////////////////////////
static  unsigned int YT_CUR_FRAME_NUMBER_LOOP = 0;
static  unsigned int YT_CUR_FRAME_INDEX_LOOP = 0;
static  unsigned int YT_CUR_FRAME_INDEX_UTT = 0;
static  char YT_FLAG_FOR_UTT_FINAL = 'N';
static  char YT_ENGINE_INITIALIZED = 'N';


#if 0
	static int yt_pSpeechFrame_INT[800];
#else
	static int *yt_pSpeechFrame_INT=NULL;
#endif







/*---------------------------------------------------------------------------------------
 |yt_tts_initialize(...)
 |PURPOSE
 |	This API intends to initialize TTS engine.
 |INPUT
 |	strReserved: reserved string variable...
 |	nLangID: language ID, which is defined in language ID section
 |	strArgOne: the first data argument, which can be a front-end file name or a data buffer
 |	nVoiceID: voice ID, which is defined in voice ID section
 |	strArgTwo: the second data argument, which can be a back-end file name or a data buffer
 |
 |OUTPUT
 |	none
 |
 |RETURN VALUE
 |	It returns 0 if successful; else it returns -1.
 |
 |CODING
 |	Shanghai YoungTone Tech.
 *--------------------------------------------------------------------------------------*/
int  yt_tts_initialize( const char *strReserved,
						  int nLangID,
						  char *strArgOne, 
						  int nVoiceID, 
						  char *strArgTwo)
{
	int nReturn = -1;
	//unsigned int  nVoiceID_IN = (unsigned int)nVoiceID;
	//unsigned int nTimeSnapshot = 0;
	int nResult = 0;

	yt_tts_start_play(); //TERENCE-2011/09/26
	CallYTV_ResetFuncPointers( );



	//YT_DispDebugInfo_TTS("before free_resource", &nTimeSnapshot);
	yt_tts_free_resource();
	nResult = yt_tts_set_rate(100);
	nResult = yt_tts_set_pitch(100); 
	nResult = yt_tts_set_volume(32768);
	YTV_ClearFrameNumber();
	yt_pSpeechFrame_INT=NULL; //TERENCE-2011/04/11

	nResult = 0;
	YT_TTS_MTK_SET_STATE(nResult);
	//nTimeSnapshot = 0;
	//YT_DispDebugInfo_TTS("after free_resource", &nTimeSnapshot);

	switch(nLangID)
	{
#ifdef YT_LANG_ID_MANDARIN
	case YT_LANG_ID_MANDARIN:
		nReturn = YT_MAN_TTS_Init(strArgOne,nVoiceID, strArgTwo);
		if(0 == nReturn)
		{
			YT_CUR_LANG_ID = YT_LANG_ID_MANDARIN;
			YT_CUR_VOICE_ID = nVoiceID;

			yt_tts_set_pitch(106);
			yt_tts_set_rate(105);

		}										
		break;
#endif


#ifdef YT_LANG_ID_US_ENGLISH
	case YT_LANG_ID_US_ENGLISH:
		nReturn = YT_ENG_TTS_Initialize(strArgOne, nVoiceID, strArgTwo);
		if(0 == nReturn)
		{
			YT_CUR_LANG_ID = YT_LANG_ID_US_ENGLISH;
			YT_CUR_VOICE_ID = nVoiceID;
			yt_tts_set_pitch(108);
			yt_tts_set_rate(105);
		}									
		break;
#endif










#ifdef YT_LANG_ID_HINDI
	case YT_LANG_ID_HINDI:
		nReturn = YT_HIN_TTS_Initialize(strArgOne,(unsigned int)nVoiceID, strArgTwo);
		if(0 == nReturn)
		{
			YT_CUR_LANG_ID = YT_LANG_ID_HINDI;
			YT_CUR_VOICE_ID = nVoiceID;
		}										
		break;
#endif


#ifdef YT_LANG_ID_INDONESIAN
	case YT_LANG_ID_INDONESIAN:
		nReturn = YT_BAHASA_TTS_Initialize(strArgOne,nVoiceID, strArgTwo);
		if(0 == nReturn)
		{
			YT_CUR_LANG_ID = YT_LANG_ID_INDONESIAN;
			YT_CUR_VOICE_ID = nVoiceID;
		}										
		break;
#endif










	default: break;
	}




	if(0 == nReturn)YT_ENGINE_INITIALIZED = 'Y';
	return nReturn;

}


static unsigned int yt_tts_flag_for_first_frame = 0;

/*-------------------------------------------------------------------------------
 |yt_tts_input_text_utf16(...)
 |PURPOSE
 |	This API intends to feed text in Unicode(UCS16/UCS2) to TTS engine.
 |INPUT
 |	strText_U16: buffer for holding UTF16 text
 |	nTextLen_U16: number of character in strText_U16
 |	DATE_FORMAT: date format, which is defined in date format section
 |	nTextType: text type,which is defined in date format section. 
 |			    In general, user can set it to the default value, 0
 |
 |
 |OUTPUT
 |	none
 |
 |RETURN VALUE
 |	It returns 0 if successful; else it returns -1.
 |
 |CODING
 |	Shanghai YoungTone Tech.
 *-------------------------------------------------------------------------------*/
int  yt_tts_input_text_utf16(unsigned short *strText_U16,//[in]
							  unsigned int nTextLen_U16,//[in]
							  unsigned int DATE_FORMAT,
							  unsigned int nTextType)
{

int nReturn = 0;
unsigned int nDateFormat = DATE_FORMAT;

if('N' == YT_ENGINE_INITIALIZED) return -1;

if(nDateFormat>2)nDateFormat=0;


YT_CUR_FRAME_NUMBER_LOOP = 0;
YT_CUR_FRAME_INDEX_LOOP = 0;
YT_CUR_FRAME_INDEX_UTT = 0;
YT_FLAG_FOR_UTT_FINAL = 'N';
CallYTV_ResetFilterBuffer();
YTV_ClearFrameNumber();
yt_global_start_synthesis();
yt_tts_flag_for_first_frame = 0;



#ifdef YT_LANG_ID_MANDARIN
	if(YT_CUR_LANG_ID == YT_LANG_ID_MANDARIN)	
	{

		nReturn = YT_MAN_TTS_InputText_UTF16(strText_U16,//[in]
											nTextLen_U16,//[in]
											1,
											nTextType);//[in]

	}

#endif

#ifdef YT_LANG_ID_US_ENGLISH
	if(YT_CUR_LANG_ID == YT_LANG_ID_US_ENGLISH)	
	{
		nReturn = YT_ENG_TTS_InputText_Unicode(strText_U16,
											 nTextLen_U16,
											 nDateFormat,
											 nTextType);
		
	}

#endif












#ifdef YT_LANG_ID_HINDI
	if(YT_CUR_LANG_ID == YT_LANG_ID_HINDI)	
	{

		nReturn = YT_HIN_TTS_InputText_Unicode(strText_U16,//[in]
												nTextLen_U16,//[in]
												2,
												0);//[in]
	}

#endif










#ifdef YT_LANG_ID_INDONESIAN
	if(YT_CUR_LANG_ID == YT_LANG_ID_INDONESIAN)	
	{

		nReturn = YT_BAHASA_TTS_InputText_Unicode(strText_U16,//[in]
													nTextLen_U16,//[in]
													2,
													0);//[in]

	}

#endif


	return  nReturn;
}

/*-------------------------------------------------------------------------------
 |yt_tts_play_text(...)
 |PURPOSE
 |	This API intends to synthesize speech for input text at sentence level 
 |	in most cases.
 |
 |INPUT
 |	yt_callback_function_for_tts: callback function. It adopts the following format:
 |
 |		int  yt_callback_function_for_tts(int nFlag, 
 |										  short *pSpeechData, 
 |										  unsigned int nSamples, 
 |										  int *pCommand)
 |
 |		where 
 |			- nFlag, which indicates TTS engine status,0 for female voice; 1 for male voice 
 |			- pSpeechData: buffer for holding synthesized speech samples
 |			- nSamples: sample number of pSpeechData 
 |
 |			- pCommand, pointer to a control variable, 
 |						user can set it to zero to interrupt synthesis.
 |		It always returns 0
 |
 |
 |	char_reserved: reserved variable
 |
 |OUTPUT
 |	none
 |
 |RETURN VALUE
 |	It returns 0 if all the text has been processed; otherwise it returns 1.
 |
 |CODING
 |	Shanghai YoungTone Tech.
 *-------------------------------------------------------------------------------*/
int yt_tts_play_text(long yt_callback_function_for_tts, char char_reserved)
{

	int nResult = 0;
//	short tempbuf[1000];
//	unsigned int nSampleNumber;
	//int nFlag = 0;
	//typedef int (*PFUNCTION)(int, short *pData, int nSamples, int *);
	//PFUNCTION play_buffer3 = (PFUNCTION)yt_callback_function_for_tts;
	//int nCommand = 1;
	static unsigned int TIMES_FOR_EVALUATION = 0;


	if('N' == YT_ENGINE_INITIALIZED) return -1;
	YT_CUR_FRAME_NUMBER_LOOP = 0;
	YT_CUR_FRAME_INDEX_LOOP = 0;
	YT_CUR_FRAME_INDEX_UTT = 0;
	YT_FLAG_FOR_UTT_FINAL = 'N';



//	if(2 == YT_CUR_VOICE_ID)nFlag = 1;

#ifdef YT_LANG_ID_MANDARIN
	if(YT_CUR_LANG_ID == YT_LANG_ID_MANDARIN)		
	{
		nResult = YT_MAN_TTS_PlayText(yt_callback_function_for_tts,char_reserved);
	}
#endif


#ifdef YT_LANG_ID_US_ENGLISH
	if(YT_CUR_LANG_ID == YT_LANG_ID_US_ENGLISH)		
	{
		nResult = YT_ENG_TTS_PlayText(yt_callback_function_for_tts,char_reserved);
	}
#endif







#ifdef YT_LANG_ID_HINDI
	if(YT_CUR_LANG_ID == YT_LANG_ID_HINDI)		
	{
		nResult = YT_HIN_TTS_PlayText(yt_callback_function_for_tts,char_reserved);
	}
#endif

#ifdef YT_LANG_ID_INDONESIAN
	if(YT_CUR_LANG_ID == YT_LANG_ID_INDONESIAN)		
	{
		nResult = YT_BAHASA_TTS_PlayText(yt_callback_function_for_tts,char_reserved);
	}
#endif




	return nResult;
}



/*-------------------------------------------------------------------------------
 |yt_tts_free_resource(...)
 |PURPOSE
 |	This API intends to free resources occupied by TTS engine
 |	in most cases.
 |
 |INPUT
 | none
 |
 |OUTPUT
 |	none
 |
 |RETURN VALUE
 |	none
 |
 |CODING
 |	Shanghai YoungTone Tech.
 *-------------------------------------------------------------------------------*/
void yt_tts_free_resource(void)
{

	YT_ENGINE_INITIALIZED = 'N';

	YT_CUR_FRAME_NUMBER_LOOP = 0;
	YT_CUR_FRAME_INDEX_LOOP = 0;
	YT_CUR_FRAME_INDEX_UTT = 0;
	YT_FLAG_FOR_UTT_FINAL = 'N';
	CallYTV_ResetFilterBuffer();
	YTV_ClearFrameNumber();


#ifdef YT_LANG_ID_MANDARIN
	if(YT_CUR_LANG_ID == YT_LANG_ID_MANDARIN)
	{
		YT_MAN_TTS_FreeResource();
	}
#endif

#ifdef YT_LANG_ID_US_ENGLISH
	if(YT_CUR_LANG_ID == YT_LANG_ID_US_ENGLISH)
	{
		YT_ENG_TTS_FreeResource();
	}
#endif


#ifdef YT_LANG_ID_HINDI
	if(YT_CUR_LANG_ID == YT_LANG_ID_HINDI)
	{
		YT_HIN_TTS_FreeResource();
	}
#endif



#ifdef YT_LANG_ID_INDONESIAN
	if(YT_CUR_LANG_ID == YT_LANG_ID_INDONESIAN)
	{
		YT_BAHASA_TTS_FreeResource();
	}
#endif
	



}






/*-------------------------------------------------------------------------------
 |yt_tts_set_rate(...)
 |PURPOSE
 |	This API intends to set speech rate(speed).
 |
 |INPUT
 | nRateFactor: the target speech rate, which ranges 50~200. 
 |				100 is the normal rate. 50 is the fastest speed while 200 is the slowest one.
 |
 |OUTPUT
 |	none
 |
 |RETURN VALUE
 |	the acutal speech rate.
 |
 |CODING
 |	Shanghai YoungTone Tech.
 *-------------------------------------------------------------------------------*/
int yt_tts_set_rate(int nRateFactor)
{

	YT_CUR_TTS_RATE = nRateFactor;
	if(YT_CUR_TTS_RATE < 50)YT_CUR_TTS_RATE = 50;
	if(YT_CUR_TTS_RATE > 200)YT_CUR_TTS_RATE = 200;


	CallYTV_SetRateFactor((unsigned int)YT_CUR_TTS_RATE);





	return YT_CUR_TTS_RATE;
}



/*-------------------------------------------------------------------------------
 |yt_tts_set_pitch(...)
 |PURPOSE
 |	This API intends to set pitch(speed).
 |
 |INPUT
 | nPitchFactor: the target pitch factor, which ranges 50~200. 
 |				100 is the normal pitch. 50 is the lowest pitch while 200 is the highest one.
 |
 |OUTPUT
 |	none
 |
 |RETURN VALUE
 |	the acutal pitch.
 |
 |CODING
 |	Shanghai YoungTone Tech.
 *-------------------------------------------------------------------------------*/
int yt_tts_set_pitch(int nPitchFactor)
{
	YT_CUR_TTS_PITCH = nPitchFactor;
	if(YT_CUR_TTS_PITCH < 50)YT_CUR_TTS_PITCH = 50;
	if(YT_CUR_TTS_PITCH > 200)YT_CUR_TTS_PITCH = 200;

	CallYTV_SetPitchFactor((unsigned int)YT_CUR_TTS_PITCH);




	return YT_CUR_TTS_PITCH;
}



/*-------------------------------------------------------------------------------
 |yt_tts_set_volume(...)
 |PURPOSE
 |	This API intends to set volume.
 |
 |INPUT
 |nMaxVolume: the target volume factor, which ranges 0~32768. 
 |				32768 means the loudest speech while 0 means silence
 |
 |OUTPUT
 |	none
 |
 |RETURN VALUE
 |	the acutal volume
 |
 |CODING
 |	Shanghai YoungTone Tech.
 *-------------------------------------------------------------------------------*/
int yt_tts_set_volume(int nMaxVolume)
{
	if(nMaxVolume>32768)YT_CUR_TTS_MAX_VOLUME = 32768;
	else if(nMaxVolume>0)YT_CUR_TTS_MAX_VOLUME = nMaxVolume;
	return YT_CUR_TTS_MAX_VOLUME;
}






static unsigned int yt_get_byte_shift_utf8(unsigned char charByte)
{
	unsigned char charOne = 0x80;
	unsigned int i = 0;
	while(i < 7)
	{
		if(!(charOne&charByte)) break;
		charOne >>= 1;
		i ++;
	}
	return i;
}

/*-------------------------------------------------------------------------------
 |yt_tts_utf8_to_utf16(...)
 |PURPOSE
 |	This API intends to conduct encoding conversion from UTF8 to UTF16
 |
 |INPUT
 |	strText_UTF8: buffer for the input text string in UTF8 encoding
 |	nByteNumber_UTF8: length of buffer, strText_UTF8 in byte
 |
 |OUTPUT
 |	strText_UTF16: buffer for holding text in UTF16 encoding,
 |				   Its length should be not less than nByteNumber_UTF8 in most cases.
 |	pTextLen_UTF16: pointer to character number after being converted.
 |
 |RETURN VALUE
 |	none
 |
 |CODING
 |	Shanghai YoungTone Tech.
 *-------------------------------------------------------------------------------*/
void  yt_tts_utf8_to_utf16(const char *strText_UTF8,
							unsigned int nByteNumber_UTF8,
							unsigned short *strText_UTF16,
							unsigned int  *pTextLen_UTF16)
{
	//UTF8 file header: 0xEF, 0xBB, 0xBF
	unsigned int nTextLen_UTF16 = 0, nShift, nNumber_BIT_1, i;
	unsigned char charByte;
	unsigned short nCode;
	unsigned char charArray[5]={0x01, 0x03,0x07,0x0f,0x1f};

	nShift = 0;
	if(nByteNumber_UTF8>3)
	{
		//in case file header exists...
		if(0xef == (unsigned char)strText_UTF8[0] && 0xbb == (unsigned char)strText_UTF8[1] &&0xbf == (unsigned char)strText_UTF8[2])nShift=3;		   		   
	}

	while(nShift < nByteNumber_UTF8)
	{
		charByte = (unsigned char)strText_UTF8[nShift++];
		nNumber_BIT_1 = yt_get_byte_shift_utf8(charByte);
		if(0 == nNumber_BIT_1)
		{
			if(NULL != strText_UTF16)strText_UTF16[nTextLen_UTF16] = charByte & 0x7f;
			nTextLen_UTF16 ++;
		}

		if(nNumber_BIT_1>1 && nNumber_BIT_1 <= 6)
		{
			nCode = charArray[6-nNumber_BIT_1] & charByte;
			for(i=1; i <nNumber_BIT_1; i++)
			{
				nCode <<= 6;
				if(nShift < nByteNumber_UTF8)nCode |= strText_UTF8[nShift++]&0x3f; //6-bit valid 
			}
			if(NULL != strText_UTF16)strText_UTF16[nTextLen_UTF16] = nCode;
			nTextLen_UTF16 ++;
		}

		//if nNumber_BIT_1==1, it's an error, just ignore it
	}

	if(NULL != strText_UTF16)strText_UTF16[nTextLen_UTF16] = '\0';
	*pTextLen_UTF16 = nTextLen_UTF16;
}









/*-------------------------------------------------------------------------------
 |yt_tts_get_speech_frame(...)
 |PURPOSE
 |	This API intends to get speech frame from TTS engine...
 |
 |BACKGROUND
 |	Several customers need to return speech frame by frame,  
 |	So we need to provide such kind of API.
 |
 |INPUT
 |	none
 |
 |OUTPUT
 |	pSpeechFrame: buffer for holding speech samples, which should be writable
 |				  Typical  pSpeechFrame contains 1000 short integer
 |	pSampleNumber: pointer to sample number of pSpeechFrame
 |
 |RETURN VALUE
 |	It returns a flag indicating TTS engine status.
 |		0: means the engine reaches text end, say all the text has been processed...
 |		1: a sentence has been completed
 |		2: need to continue
 |
 |CODING
 |	Shanghai YoungTone Tech.
 *-------------------------------------------------------------------------------*/


int yt_tts_get_speech_frame(short *pSpeechFrame, unsigned int *pSampleNumber)
												  
{

	//int nFlag = 0;
	int nResult, nReturn;
	int nSampleNumber, fperiod, nInt, k;
	char charFlag_TO_DO = 'N';
	unsigned int nModelNumber = 0;
	static int nStage = 3;



	static unsigned int TIMES_FOR_EVALUATION = 0;

	char *pMemoryBuffer, *pTemp;
	
	unsigned int nMemSize,nOffset_MISC,nBufSize_MISC;


	*pSampleNumber = 0;

	if('N' == YT_ENGINE_INITIALIZED) return 0;

	if(NULL == yt_pSpeechFrame_INT)
	{

		pMemoryBuffer = YT_MM_GetMemoryBuffer(&nMemSize);
		nOffset_MISC = YT_MM_GetOffsetForMisc(&nBufSize_MISC);

		if(NULL != pMemoryBuffer)
		{
			pTemp = &pMemoryBuffer[nOffset_MISC+YTV_MAX_FRAME_SAMPLE_NUMBER_2*8+80];
			yt_pSpeechFrame_INT = (int*)(void*)pTemp;

			#ifndef YT_TTS_FINAL_RELEASE
				assert(YTV_MAX_FRAME_SAMPLE_NUMBER_2*8 + 420 * sizeof(int) < nBufSize_MISC);/*assert verified*/
			#endif
		}
	}

	if(NULL == yt_pSpeechFrame_INT) return 2;
	if('Y' !=yt_global_get_synthesis_flag()) return -1; //TERENCE



	TIMES_FOR_EVALUATION ++;	





		if(YT_CUR_FRAME_INDEX_LOOP >= YT_CUR_FRAME_NUMBER_LOOP)
		{
			//if(1)
			{
				
				
				
				nStage = 0;
				nReturn = CallYTV_SynthesizeSpeechForLoop(nStage,(long)NULL, &YT_CUR_FRAME_NUMBER_LOOP,&YT_FLAG_FOR_UTT_FINAL);
								
				YT_CUR_FRAME_INDEX_LOOP = 0;	
				
				if(0 == nReturn || 0 == YT_CUR_FRAME_NUMBER_LOOP)
				{										
					nResult = 0;					

					#ifdef YT_LANG_ID_MANDARIN
							if(YT_CUR_LANG_ID == YT_LANG_ID_MANDARIN)nResult = YT_MAN_TTS_PlayText((long)NULL,'N');	
					#endif
					
										
					#ifdef YT_LANG_ID_US_ENGLISH
							if(YT_CUR_LANG_ID == YT_LANG_ID_US_ENGLISH)	nResult = YT_ENG_TTS_PlayText((long)NULL,'N');
					#endif					
										
										
										
										
					#ifdef YT_LANG_ID_HINDI
							if(YT_CUR_LANG_ID ==YT_LANG_ID_HINDI)nResult = YT_HIN_TTS_PlayText((long)NULL,'N');	
					#endif


										
					#ifdef YT_LANG_ID_INDONESIAN
						if(YT_CUR_LANG_ID ==YT_LANG_ID_INDONESIAN)nResult = YT_BAHASA_TTS_PlayText((long)NULL,'N');	
					#endif






					if(0 == nResult)return 0;
					YT_CUR_FRAME_INDEX_UTT = 0;
					YT_CUR_FRAME_NUMBER_LOOP = 0;
					
					return 10001;
				}
				
				
				nStage ++;
				return 10002;
			}
		}


		if(nStage < 3)
		{
			nReturn = CallYTV_SynthesizeSpeechForLoop(nStage,(long)NULL, &YT_CUR_FRAME_NUMBER_LOOP,&YT_FLAG_FOR_UTT_FINAL);
			nStage ++;
			return 10000 + nStage;
		}
		



		yt_tts_flag_for_first_frame ++;
		if(1 == yt_tts_flag_for_first_frame) return 2;
		nModelNumber = CallYTV_GetLabelNumber();
		if(0 == nModelNumber) return 1;

		CallYTV_GetSpeechFrame(yt_pSpeechFrame_INT, (int)YT_CUR_FRAME_INDEX_LOOP,&nSampleNumber);


		//need to conduct fade in/fade out...
		if(nSampleNumber > 0)
		{

			fperiod = nSampleNumber;
			charFlag_TO_DO = 'Y';






			for(k=0; k<nSampleNumber; k++)
			{
				nInt = yt_pSpeechFrame_INT[k];


				//if(nTotalFrameNumber < 200)nInt >>= 1;
				//assert(nInt < 32768 && nInt > -32768);
				//nInt <<= 2; nInt /= 6;
				if(nModelNumber <= 5)
				{
					nInt = nInt;
					//nInt <<= 2; nInt /= 5;
				}
				//nInt >>= 1;

				if(YT_CUR_FRAME_INDEX_UTT<6)
				{
				  nInt *= ((int)YT_CUR_FRAME_INDEX_UTT *fperiod+k+10);
				  nInt /= 6*fperiod+10;
				}


				yt_pSpeechFrame_INT[k] = nInt;
			}


			//if('Y' == charFlag_TO_DO)
			{
				for(k=0; k<nSampleNumber; k++)
				{
					nInt =  yt_pSpeechFrame_INT[k];
					//nInt *= YT_CUR_TTS_MAX_VOLUME;
					//nInt >>= 15;
					pSpeechFrame[k] = (short) nInt;

				}

				*pSampleNumber = (unsigned int)nSampleNumber;
			}


			YT_CUR_FRAME_INDEX_UTT ++;
		}



		YT_CUR_FRAME_INDEX_LOOP ++;
		if(YT_CUR_FRAME_INDEX_LOOP >= YT_CUR_FRAME_NUMBER_LOOP  )
		{
			if('Y' == YT_FLAG_FOR_UTT_FINAL)return 1; //for sentence final...
			else if(YT_CUR_FRAME_NUMBER_LOOP>0)return 10; //for break final...
			else return 2;
		}
		else return 2;
	



	//return nFlag;
}






int yt_tts_set_memory_buffer(char *pBufferForTTSEngine,unsigned int nMemSize)	
													 
{
	int nReturn = YT_MM_SetMemoryBuffer(pBufferForTTSEngine, nMemSize);

	return nReturn;
}



unsigned int yt_tts_get_cur_lang()
{
	return (unsigned int)YT_CUR_LANG_ID;
}


void yt_set_callback_func_for_debug(long pCallBack)
{

YT_SetCallbackFuncForDebug(pCallBack);

}

///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////

/*-------------------------------------------------------------------------------
 |yt_set_callback_func_for_fopen(...)
 |PURPOSE
 |	This API intends to set a callback function for opening file like fopen(...)
 |
 |INPUT
 |	pCallBack: callback function address, say function name. Here is its definition.
 |				long open_file(unsigned shoer *strFileName_U16);
 |OUTPUT
 |	none
 |
 |RETURN VALUE
 |	none
 |
 |CODING
 |	Shanghai YoungTone Tech.
 *-------------------------------------------------------------------------------*/

void yt_set_callback_func_for_fopen(long pCallBack)
{
	YT_SetCallbackForOpenFile(pCallBack);
}


/*-------------------------------------------------------------------------------
 |yt_set_callback_func_for_fread(...)
 |PURPOSE
 |	This API intends to set a callback function for reading file like fread(...)
 |
 |INPUT
 |	pCallBack: callback function address, say function name. Here is its definition.
 |				unsigned int read_file(void * ptr, unsigned int size, void *file_handle);
 |
 |OUTPUT
 |	none
 |
 |RETURN VALUE
 |	none
 |
 |CODING
 |	Shanghai YoungTone Tech.
 *-------------------------------------------------------------------------------*/
void yt_set_callback_func_for_fread(long pCallBack)
{
	YT_SetCallbackForReadFile(pCallBack);
}



/*-------------------------------------------------------------------------------
 |yt_set_callback_func_for_fclose(...)
 |PURPOSE
 |	This API intends to set a callback function for reading file like fclose(...)
 |
 |INPUT
 |	pCallBack: callback function address, say function name. Here is its definition.
 |				unsigned int close_file(void *file_handle);
 |
 |OUTPUT
 |	none
 |
 |RETURN VALUE
 |	none
 |
 |CODING
 |	Shanghai YoungTone Tech.
 *-------------------------------------------------------------------------------*/
void yt_set_callback_func_for_fclose(long pCallBack)
{
	YT_SetCallbackForCloseFile(pCallBack);
}



/*-------------------------------------------------------------------------------
 |yt_set_callback_func_for_ftell(...)
 |PURPOSE
 |	This API intends to set a callback function for telling file pos like ftell(...)
 |
 |INPUT
 |	pCallBack: callback function address, say function name. Here is its definition.
 |				unsigned int tell_file(void *file_handle);
 |
 |OUTPUT
 |	none
 |
 |RETURN VALUE
 |	none
 |
 |CODING
 |	Shanghai YoungTone Tech.
 *-------------------------------------------------------------------------------*/
void yt_set_callback_func_for_ftell(long pCallBack)
{
	YT_SetCallbackForTellFile(pCallBack);
}



/*-------------------------------------------------------------------------------
 |yt_set_callback_func_for_fseek(...)
 |PURPOSE
 |	This API intends to set a callback function for seeking file pos like fseek(...)
 |
 |INPUT
 |	pCallBack: callback function address, say function name. Here is its definition.
 |				unsigned int seek_file(void *file_handle, unsigned int nOffset, unsigned int whence)
 |
 |OUTPUT
 |	none
 |
 |RETURN VALUE
 |	none
 |
 |CODING
 |	Shanghai YoungTone Tech.
 *-------------------------------------------------------------------------------*/
void yt_set_callback_func_for_fseek(long pCallBack)
{
	YT_SetCallbackForSeekFile(pCallBack);
}


///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////








void yt_tts_stop_play()
{
	yt_global_stop_synthesis();
}



void yt_tts_start_play()
{
	yt_global_start_synthesis();
}

static const char   verStringBuildTime[40]   =   __DATE__   "   "     __TIME__; 
char *yt_tts_get_build_time()
{

	return (char *)verStringBuildTime;
}
