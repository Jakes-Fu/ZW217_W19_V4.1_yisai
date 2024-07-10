#include "heroEngineFm.h"
#include "audio_api.h"
#include "audio_config.h"
#include "heroEngineApi.h"
#include "mp3_adp.h"
#include "mmisrvaud_api.h"
#include "mmisrvmgr.h"

#define TRACK_BUF_SIZE_PLAYER         (80 * 150)
#define TRACK_BUF_SIZE_APP              (0 * 150)
#define DECODE_OUTPUT_BUF_SIZE      (TRACK_BUF_SIZE_APP + TRACK_BUF_SIZE_PLAYER)

#define HERO_FM_DECODE_STREAM_BUF_SIZE  (10 * 1024)
#define HERO_FM_PER_FRAME_COUNT	(50)

#define HERO_FM_DECODE_STREAM_BUF_COUNT (2)

typedef struct tagHeroFmPlayBufSt{
	int audioSize;
	char *audioBuf; //HERO_FM_DECODE_STREAM_BUF_SIZE
}heroFmPlayBufSt;

typedef struct tagHeroFmAudioContextSt{
	unsigned char amrHeadIsSend;
	unsigned char isRecording;

	unsigned char isPlaying;

	unsigned char volume;

	AUDIO_DEVICE_MODE_TYPE_E audioCurDevMode;

	SCI_SEMAPHORE_PTR audioPlayMutex;

	///////////////////
	short playStreamReadIndex;
	short playStreamWriteIndex;
	heroFmPlayBufSt playBufMap[HERO_FM_DECODE_STREAM_BUF_COUNT];

	HAUDIO audioPlayHandle;
}HeroFmAudioContextSt;

typedef struct tagHeroFmAudioPlayRetSt{
	int pushAudioSize;
}HeroFmAudioPlayRetSt;

typedef struct tagHeroFmAudioStreamSt{
	int sampleRate;
	short channel;
	short volume;
	char *stream;
	int streamSize;
}HeroFmAudioStreamSt;

typedef struct tagHeroFmAudioVolumeSt{
	int volume;
}HeroFmAudioVolumeSt;

typedef struct tagHeroFmCacheSizeSt{
	int cacheSize;
}HeroFmCacheSizeSt;

typedef struct tagHeroFmCurrentTimeSt{
	uint32 currentTime;
}HeroFmCurrentTimeSt;

extern uint32 VBDecodeOuputBuf[];

static HeroFmAudioPlayRetSt gHeroFmAudioPlayRet = {0};

static HeroFmAudioContextSt gHeroFmAudioContext = {0};

static HeroFmCacheSizeSt gHeroFmCacheSize = {0};

static HeroFmCurrentTimeSt gHeroFmCurTime = {0};

static int heroFmPushData(char *data, int dataSize);

static void hero_fm_free(void *ptr)
{
	if(ptr != NULL)
	{
		SCI_FREE(ptr);
	}
}

static void *hero_fm_malloc(int size)
{
	return (void*)SCI_ALLOCA(size);
}

static AUDIO_STREAM_GET_SRC_RESULT_E hero_fm_play_amr_string_callback(HAUDIO hAudio, AUDIO_GETSRCDATA_INFO_T * const ptSrcDataInfo, void *pvOtherInfo)
{
	AUDIO_STREAM_GET_SRC_RESULT_E ret = AUDIO_STREAM_GET_SRC_ERROR;
	do
	{
		heroFmPlayBufSt *pPlayBuf = NULL;
		heroFmPlayBufSt *pPlayWriteBuf = NULL;
		HeroFmAudioContextSt *pContext = &gHeroFmAudioContext;

		SCI_ASSERT(ptSrcDataInfo!=PNULL);

		ptSrcDataInfo->tStreamCurSrcDataInfo.pucDataAddr  = PNULL;

		if(!pContext->isPlaying)
		{
			hr_printf("hero_fm_play_amr_string_callback audio is stop ...");
			break;
		}

	SCI_GetSemaphore(pContext->audioPlayMutex, SCI_WAIT_FOREVER);
		pPlayBuf = &pContext->playBufMap[pContext->playStreamReadIndex];
		pContext->playStreamReadIndex = (++pContext->playStreamReadIndex)%HERO_FM_DECODE_STREAM_BUF_COUNT;

		pContext->playStreamWriteIndex = (++pContext->playStreamWriteIndex)%HERO_FM_DECODE_STREAM_BUF_COUNT;
		pPlayWriteBuf = &pContext->playBufMap[pContext->playStreamWriteIndex];
		pPlayWriteBuf->audioSize = 0;
	SCI_PutSemaphore(pContext->audioPlayMutex);

		ptSrcDataInfo->tStreamCurSrcDataInfo.pucDataAddr = pPlayBuf->audioBuf;
		ptSrcDataInfo->tStreamCurSrcDataInfo.uiDataLength = pPlayBuf->audioSize;
		hr_printf("hero_fm_play_amr_string_callback pPlayBuf->audioSize = %d", pPlayBuf->audioSize);

		ret = AUDIO_STREAM_GET_SRC_SUCCESS;
	}while(0);
	return ret;
}

static BOOLEAN heroFmSetVolume(HAUDIO handle, int volume)
{
	return MMISRVAUD_SetVolume(handle, volume);
}

static BOOLEAN heroFmHandleNotify(MMISRV_HANDLE_T handle, MMISRVMGR_NOTIFY_PARAM_T *param)
{
	MMISRVAUD_REPORT_T *report_ptr = PNULL;
	if(param != PNULL && handle > 0)
	{
		report_ptr = (MMISRVAUD_REPORT_T *)param->data;
		if(report_ptr != PNULL)
		{            
			switch(report_ptr->report)
			{
				case MMISRVAUD_REPORT_END:
				break;
				default:
				break;
			}
		}
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

LOCAL MP3_STREAM_BUFFER_T heroMp3StreamHeaderInfo = {0};
int heroFmAudioOpen(char* input, int input_len, char** output, int* output_len)
{
	int ret = 0;
	do
	{
		int volume = 0;
		int playRet = 0;
		int pushDataSize = 0;
		int index = 0;
		MMISRVAUD_TYPE_T audio_srv = {0};
		MMISRVMGR_SERVICE_REQ_T req = {0};
		heroFmPlayBufSt *pPlayBuf = NULL;
		HeroFmAudioStreamSt *pStream = (HeroFmAudioStreamSt*)input;
		HeroFmAudioContextSt *pContext = &gHeroFmAudioContext;

		hr_printf("heroFmAudioOpen pContext->isPlaying = %d", pContext->isPlaying);
		if(pContext->isPlaying)
		{
			break;
		}

		if(pStream == NULL || input_len != sizeof(HeroFmAudioStreamSt) || pStream->stream == NULL || pStream->streamSize == 0 || output == NULL || output_len == NULL)
		{
			break;
		}

		if(pContext->audioPlayMutex == 0)
		{
			pContext->audioPlayMutex = SCI_CreateSemaphore("fmstream", 1);
		}

		for(index = 0; index < HERO_FM_DECODE_STREAM_BUF_COUNT; index++)
		{
			pPlayBuf = &pContext->playBufMap[index];
			if(pPlayBuf->audioBuf == NULL)
			{
				pPlayBuf->audioBuf = (char*)hero_fm_malloc(HERO_FM_DECODE_STREAM_BUF_SIZE);
				hr_printf("heroFmAudioOpen pPlayBuf->audioBuf = %d", pPlayBuf->audioBuf);
				if(pPlayBuf->audioBuf == NULL)
				{
					break;
				}
				pPlayBuf->audioSize = 0;
			}
		}
		if(index != HERO_FM_DECODE_STREAM_BUF_COUNT)
		{
			break;
		}
		pContext->playStreamReadIndex = 0;
		pContext->playStreamWriteIndex = 0;

		pPlayBuf = &pContext->playBufMap[pContext->playStreamWriteIndex];

		pContext->isPlaying = 1;

		pushDataSize = heroFmPushData(pStream->stream, pStream->streamSize);
		if(pushDataSize <= 0)
		{
			pContext->isPlaying = 0;
			pPlayBuf->audioSize = 0;
			break;
		}

		AUDIO_GetDevMode(&pContext->audioCurDevMode);

		volume = ((pStream->volume)*MMISRVAUD_VOLUME_LEVEL_MAX)/5;
		if(volume < 0 || volume > MMISRVAUD_VOLUME_LEVEL_MAX)
		{
			volume = MMISRVAUD_VOLUME_LEVEL_MAX;
		}

		pContext->volume = volume;

		#ifdef BLUETOOTH_SUPPORT
	    if(BT_GetState())
	    {
	        hr_printf("BLUETOOTH_SUPPORT AUDIO_SetDevMode");
		    AUDIO_SetDevMode(AUDIO_DEVICE_MODE_BLUEPHONE);
		}
		else
		#endif
		AUDIO_SetDevMode(AUDIO_DEVICE_MODE_HANDFREE);

		heroMp3StreamHeaderInfo.bType = TRUE;
		heroMp3StreamHeaderInfo.unMp3StreamData.tMp3FormatInfo.uiChannelNum = pStream->channel;
		heroMp3StreamHeaderInfo.unMp3StreamData.tMp3FormatInfo.uiSampleRate  = pStream->sampleRate; //44100;

		req.notify	 = heroFmHandleNotify;
		req.pri 	 = MMISRVAUD_PRI_NORMAL;
		audio_srv.volume  = volume;
		audio_srv.volume_type = MMISRVAUD_VOLUME_TYPE_NONE;
		audio_srv.duation = 0;
		audio_srv.eq_mode = 0;
		audio_srv.play_times = 1;
		audio_srv.is_mixing_enable = FALSE;
		audio_srv.info.streaming.type = MMISRVAUD_TYPE_STREAMING;
		audio_srv.info.streaming.data = (uint8 *)(&heroMp3StreamHeaderInfo);
		audio_srv.info.streaming.data_len = sizeof(heroMp3StreamHeaderInfo);
		audio_srv.info.streaming.fmt = MMISRVAUD_RING_FMT_MP3;
		audio_srv.info.streaming.cb = (void *)hero_fm_play_amr_string_callback; /*lint !e611*/
		audio_srv.all_support_route = MMISRVAUD_ROUTE_AUTO;

		pContext->audioPlayHandle = MMISRVMGR_Request(STR_SRV_AUD_NAME, &req, &audio_srv);
		if(pContext->audioPlayHandle == INVALID_HANDLE)
		{
			pContext->isPlaying = 0;
			pPlayBuf->audioSize = 0;
			break;
		}

	#ifdef BLUETOOTH_SUPPORT
		if(BT_GetState())
		{
			HAUDIODEV hDevice = GetAudioDevice(TRUE, audio_srv.info.streaming.fmt);
			playRet = AUDIO_SwitchDevice(pContext->audioPlayHandle, hDevice, PNULL);
		}
	#endif

		playRet = MMISRVAUD_Play(pContext->audioPlayHandle, 0);
		if(playRet != MMISRVAUD_RET_OK && playRet != MMISRVAUD_RET_PENDING)
		{
			MMISRVMGR_Free(pContext->audioPlayHandle);
			pContext->audioPlayHandle = INVALID_HANDLE;

			pContext->isPlaying = 0;
			pPlayBuf->audioSize = 0;
			break;
		}

		heroFmSetVolume(pContext->audioPlayHandle, volume);

		gHeroFmAudioPlayRet.pushAudioSize = pushDataSize;
		*output = &gHeroFmAudioPlayRet;
		*output_len = sizeof(gHeroFmAudioPlayRet);

		ret = 1;
	}while(0);
	hr_printf("heroFmAudioOpen END heroVideoChatAudioOpen ret = %d", ret);

	return ret == 1 ? HR_SUCCESS : HR_FAILED;
}

int heroFmAudioPause()
{
	int ret = 0;
	do
	{
		int index = 0;
		HeroFmAudioContextSt *pContext = &gHeroFmAudioContext;

		hr_printf("heroFmAudioPause pContext->isPlaying = %d", pContext->isPlaying);
		if(!pContext->isPlaying)
		{
			break;
		}

		if(pContext->audioPlayHandle != INVALID_HANDLE)
		{
			MMISRVAUD_Pause(pContext->audioPlayHandle);
		}

		ret = 1;
	}while(0);
	hr_printf("heroFmAudioPause ret = %d", ret);
	return ret == 1 ? HR_SUCCESS : HR_FAILED;
}

int heroFmAudioResume()
{
	int ret = 0;
	do
	{
		int index = 0;
		HeroFmAudioContextSt *pContext = &gHeroFmAudioContext;

		hr_printf("heroFmAudioResume pContext->isPlaying = %d", pContext->isPlaying);
		if(!pContext->isPlaying)
		{
			break;
		}

		if (pContext->audioPlayHandle != INVALID_HANDLE)
		{
			MMISRVAUD_Resume(pContext->audioPlayHandle);

			heroFmSetVolume(pContext->audioPlayHandle, pContext->volume);
		}

		ret = 1;
	}while(0);
	hr_printf("heroFmAudioResume ret = %d", ret);
	return ret == 1 ? HR_SUCCESS : HR_FAILED;
}

int heroFmAudioClose()
{
	int ret = 0;
	do
	{
		int index = 0;
		HeroFmAudioContextSt *pContext = &gHeroFmAudioContext;

		hr_printf("heroFmAudioClose pContext->isPlaying = %d", pContext->isPlaying);
		if(!pContext->isPlaying)
		{
			break;
		}
		pContext->isPlaying = 0;

		hr_printf("heroFmAudioClose step1 xx");
		AUDIO_SetDevMode(pContext->audioCurDevMode);
		
		hr_printf("heroFmAudioClose step1");
		if(pContext->audioPlayMutex != NULL)
		{
			SCI_DeleteSemaphore(pContext->audioPlayMutex);
			pContext->audioPlayMutex = NULL;
		}

		if (pContext->audioPlayHandle != INVALID_HANDLE)
		{
			MMISRVAUD_Stop(pContext->audioPlayHandle);

			MMISRVMGR_Free(pContext->audioPlayHandle);

			pContext->audioPlayHandle = INVALID_HANDLE;
		}

		hr_printf("heroFmAudioClose step2");
		for(index = 0; index < HERO_FM_DECODE_STREAM_BUF_COUNT; index++)
		{
			heroFmPlayBufSt *pPlayBuf = &pContext->playBufMap[index];
			if(pPlayBuf->audioBuf != NULL)
			{
				hero_fm_free((void *)pPlayBuf->audioBuf);
				pPlayBuf->audioBuf = NULL;
			}
			pPlayBuf->audioSize = 0;
		}

		hr_printf("heroFmAudioClose step3");
		ret = 1;
	}while(0);
	hr_printf("heroFmAudioClose ret = %d", ret);
	return ret == 1 ? HR_SUCCESS : HR_FAILED;
}

static int heroFmPushData(char *data, int dataSize)
{
	int ret = 0;
	HeroFmAudioContextSt *pContext = &gHeroFmAudioContext;
	do
	{
		int copySize = 0;
		heroFmPlayBufSt *pPlayBuf = NULL;

		hr_printf("heroFmPushData data = %d, dataSize = %d, pContext->isPlaying = %d", data, dataSize, pContext->isPlaying);
		if(data == NULL || dataSize == 0 || !pContext->isPlaying)
		{
			break;
		}

	SCI_GetSemaphore(pContext->audioPlayMutex, SCI_WAIT_FOREVER);
		pPlayBuf = &pContext->playBufMap[pContext->playStreamWriteIndex];
	SCI_PutSemaphore(pContext->audioPlayMutex);

		if(pPlayBuf->audioBuf == NULL)
		{
			break;
		}

		copySize = MIN(HERO_FM_DECODE_STREAM_BUF_SIZE - pPlayBuf->audioSize, dataSize);
		hr_printf("heroFmPushData copySize = %d", copySize);

		memcpy(pPlayBuf->audioBuf + pPlayBuf->audioSize, data, copySize);
		pPlayBuf->audioSize += copySize;

		ret = copySize;
	}while(0);
	hr_printf("heroFmPushData 2222 copySize = %d", ret);
	return ret;
}

int heroFmAudioAppendData(char* input, int input_len, char** output, int* output_len)
{
	int ret = 0;
	do
	{
		int size = 0;		
		HeroFmAudioStreamSt *pStream = (HeroFmAudioStreamSt*)input;

		hr_printf("heroFmAudioAppendData pStream = %d, inputSize = %d", pStream, input_len);
		if(pStream == NULL || input_len != sizeof(HeroFmAudioStreamSt) || output == NULL || output_len == NULL)
		{
			break;
		}

		size = heroFmPushData(pStream->stream, pStream->streamSize);
		hr_printf("heroFmAudioAppendData size = %d", size);

		gHeroFmAudioPlayRet.pushAudioSize = size;

		*output = (char*)&gHeroFmAudioPlayRet;
		*output_len = sizeof(HeroFmAudioPlayRetSt);

		if(size <= 0)
		{
			break;
		}
		ret = 1;
	}while(0);
	return ret == 1 ? HR_SUCCESS : HR_FAILED;
}

int heroFmAudioClear()
{
	int ret = HR_FAILED;
	do
	{
		int index = 0;
		HeroFmAudioContextSt *pContext = &gHeroFmAudioContext;

		pContext->playStreamReadIndex = 0;
		pContext->playStreamWriteIndex = 0;

		for(index = 0; index < HERO_FM_DECODE_STREAM_BUF_COUNT; index++)
		{
			heroFmPlayBufSt *pPlayBuf = &pContext->playBufMap[index];
			pPlayBuf->audioSize = 0;
		}
		ret = HR_SUCCESS;
	}while(0);
	
	return ret;
}

int heroFmAudioGetCacheSize(char** output, int *output_len)
{
	int ret = HR_FAILED;

	do
	{
		int cacheSize = 0;
		heroFmPlayBufSt *pPlayBuf = NULL;	
		HeroFmAudioContextSt *pContext = &gHeroFmAudioContext;

		hr_printf("heroFmAudioGetCacheSize output = %d, output_len = %d", output, output_len);
		if(output == NULL || output_len == NULL || !pContext->isPlaying)
		{
			break;
		}

	SCI_GetSemaphore(pContext->audioPlayMutex, SCI_WAIT_FOREVER);
		pPlayBuf = &pContext->playBufMap[pContext->playStreamWriteIndex];
	SCI_PutSemaphore(pContext->audioPlayMutex);

		cacheSize = pPlayBuf->audioSize;

		gHeroFmCacheSize.cacheSize = cacheSize;

		*output = &gHeroFmCacheSize;
		*output_len = sizeof(gHeroFmCacheSize);
		hr_printf("heroFmAudioGetCacheSize cacheSize = %d", gHeroFmCacheSize.cacheSize);

		ret = HR_SUCCESS;
	}while(0);
	return ret;
}

int heroFmAudioSetVolume(char* input, int input_len)
{
	int ret = HR_FAILED;
	do
	{
		int volume = 0;
		HeroFmAudioContextSt *pContext = &gHeroFmAudioContext;
		HeroFmAudioVolumeSt *pVolume = (HeroFmAudioVolumeSt*)input;
		volume = pVolume->volume;

		if(pVolume == NULL || input_len != sizeof(HeroFmAudioVolumeSt))
		{
			break;
		}

		if(volume > 5){
			volume = 5;
		}

		volume = (volume*MMISRVAUD_VOLUME_LEVEL_MAX)/5;
		if(volume < 0 || volume > MMISRVAUD_VOLUME_LEVEL_MAX) {
			volume = MMISRVAUD_VOLUME_LEVEL_MAX;
		}

		heroFmSetVolume(pContext->audioPlayHandle, volume);
		pContext->volume = volume;

		ret = HR_SUCCESS;
	}while(0);

	return ret;
}

int heroFmAudioGetVolume(char* input, int input_len)
{
	int ret = HR_FAILED;
	do
	{
		int volume = 0;
		HeroFmAudioContextSt *pContext = &gHeroFmAudioContext;
		HeroFmAudioVolumeSt *pVolume = (HeroFmAudioVolumeSt*)input;

		if(pVolume == NULL || input_len != sizeof(HeroFmAudioVolumeSt)) {
			break;
		}

		if(pContext->volume == 0) {
			pContext->volume = MMISRVAUD_VOLUME_LEVEL_MAX/2;
		}
		pVolume->volume = (pContext->volume * 5)/MMISRVAUD_VOLUME_LEVEL_MAX;
		if(pVolume->volume > 5){
			pVolume->volume = 5;
		}

		ret = HR_SUCCESS;
	}while(0);

	return ret;
}

int heroFmAudioGetMaxCacheSize()
{
	return HERO_FM_DECODE_STREAM_BUF_SIZE;
}

int heroFmAudioGetInfo(char** output, int *output_len)
{
	int ret = HR_FAILED;
	do
	{
		heroFmPlayBufSt *pPlayBuf = NULL;
		MMISRVAUD_PLAY_INFO_T ptPlayInfo = {0};
		HeroFmAudioContextSt *pContext = &gHeroFmAudioContext;

		hr_printf("heroFmAudioGetInfo output = %d, output_len = %d", output, output_len);
		if(output == NULL || output_len == NULL || pContext->audioPlayHandle == INVALID_HANDLE)
		{
			break;
		}

		if(!MMISRVAUD_GetPlayingInfo(pContext->audioPlayHandle, &ptPlayInfo))
		{
			break;
		}

		gHeroFmCurTime.currentTime = ptPlayInfo.cur_time;

		*output = &gHeroFmCurTime;
		*output_len = sizeof(gHeroFmCurTime);
		hr_printf("heroFmAudioGetInfo ptPlayInfo.uiCurrentTime = %d", ptPlayInfo.cur_time);

		ret = HR_SUCCESS;
	}while(0);

	hr_printf("heroFmAudioGetInfo ret = %d", ret);
	return ret;	
}

