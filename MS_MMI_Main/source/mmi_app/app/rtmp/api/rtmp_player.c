#include "rtmp_player.h"
#include "minirtmp.h"
#include "librtmp/log.h"
#include "rtmp_mp3_info.h"

#include "mmisrv.h"
#include "mmisrvaud.h"
#include "mmisrvaud_api.h"
#include "mmi_ring.h"
#include "mmiset_export.h"
#include "mmisrvrecord_export.h"
#include "dsp_codec_adp.h"
#include "aud_gen.h"
#include "mp3_adp.h"
#include "sig_code.h"
#include "aac_lc_adp.h"

typedef enum
{
    RTMP_AUDIO_TYPE_MP3 = 0,
    RTMP_AUDIO_TYPE_AAC,		
} RTMP_AUDIO_TYPE_T;


static BLOCK_ID _threadId = SCI_INVALID_BLOCK_ID;
static SCI_MUTEX_PTR _mutexPtr = NULL;

static int _quitThread = 0; // 0 : running, 1 : to quit
static int _stoped = 1;     // 0 : running, 1 : stopped

static mrtmp_packet_t * _packets = NULL;
static mrtmp_packet_t * _tail = NULL;
static int _packetsTotal = 0;

static uint32 _audioHandle = 0;
static int _currentVolume = MMISET_VOL_MAX;
static MP3_STREAM_BUFFER_T  mp3_header_info = { 0 };
extern uint32 VBDecodeOuputBuf[];
unsigned char _mp3Cache[16 * 1024] = {0};

static int _isPlaying = 0;
static RTMP_AUDIO_TYPE_T _audioType = RTMP_AUDIO_TYPE_MP3;
static int _cacheCount = 100;

static rtmp_player_status_t _playerStatus = RTMP_STATUS_STOP;

MINIRTMP rtmp_interface = {0};
char * rtmp_url = PNULL;

static void rtmp_player_thread_proc(uint32 param, void* data);
static void rtmp_player_on_packet(RTMPPacket * pkt);

static void rtmp_dec_dummy_callbak(HAUDIO hAudio,	uint32 notify_info,	uint32 affix_info);
static AUDIO_STREAM_GET_SRC_RESULT_E rtmp_player_stream_data_callback(HAUDIO hAudio,	AUDIO_GETSRCDATA_INFO_T * const ptSrcDataInfo,	void *pvOtherInfo);

static int rtmp_player_audio_data_callback(unsigned int from_pos, unsigned char *data, unsigned int size);

static int rtmp_player_streaming_start();
static int rtmp_player_streaming_stop();

static int rtmp_player_mp3_stream_start();
static int rtmp_player_aac_stream_start();

static int rtmp_player_quit_callback(void * rtmp);

int rtmp_player_play(const char *url)
{
	int len = url != NULL ? strlen(url) : 0;

	if(len <= 0) {
		RTMP_Log(RTMP_LOGERROR, "invalid url");
		return -1;
	}
	rtmp_player_stop(); // delete old thread

	RTMP_Log(RTMP_LOGINFO, "rtmp: %s\n", url);
    
	if(rtmp_url != PNULL)
	{
	   	rtmp_free(rtmp_url);
	}

	rtmp_url = rtmp_malloc(len + 1);
	strncpy(rtmp_url, url, len);
	rtmp_url[len] = '\0';
    
	if(_mutexPtr == NULL) {
		_mutexPtr = SCI_CreateMutex("rtmp-mutex", SCI_INHERIT);
	}

	_threadId = SCI_CreateAppThread("rtmp thread",
		"rtmp thread",
		rtmp_player_thread_proc,
		0,
		rtmp_url,
		1024 * 20,			//stack_size,
		10, 				// queue_num
		TX_SYSTEM_NORMALE,
		SCI_NO_PREEMPT,
		SCI_AUTO_START);

	if ( _threadId == SCI_INVALID_BLOCK_ID) {
        if(rtmp_url != PNULL)
        {
			rtmp_free(rtmp_url);
            rtmp_url = PNULL;
        }
     	if(_mutexPtr != NULL) {
    		SCI_DeleteMutex(_mutexPtr);
    		_mutexPtr = NULL;
        }
		return -1;
	}

    return 0;
}

int rtmp_player_stop()
{
	int retryCount = 5;
	mrtmp_packet_t * pkt = NULL;

	RTMP_Log(RTMP_LOGINFO, "rtmp: stop\n");

	_quitThread = 1;
    
	// Todo : waiting for the thread exit
	while(_stoped == 0 && retryCount > 0) {		
		SCI_Sleep(50);
		--retryCount;
	}
	if(_stoped == 0) {
		RTMP_Log(RTMP_LOGERROR, "The thread is still running");
        if(_threadId != SCI_INVALID_BLOCK_ID)
        {
            SCI_TerminateThread(_threadId);
            SCI_DeleteThread(_threadId);
            _threadId = SCI_INVALID_BLOCK_ID;      
        }
        minirtmp_close(&rtmp_interface);
        _playerStatus = RTMP_STATUS_STOP;
        _stoped = 1;
	}
	else {
        if(_threadId != SCI_INVALID_BLOCK_ID)
        {
            SCI_TerminateThread(_threadId);
            SCI_DeleteThread(_threadId);
            _threadId = SCI_INVALID_BLOCK_ID;      
        }
		_playerStatus = RTMP_STATUS_STOP;
	}

	rtmp_player_streaming_stop();

	// Todo : release packets
    while (_packets && _packetsTotal > 0)
    {
        if (_packets->data)
            rtmp_free((char *)_packets->data - RTMP_MAX_HEADER_SIZE);
		
        pkt = _packets->next;
        rtmp_free(_packets);
        _packets = pkt;
		_packetsTotal--;
    }

	if(_packetsTotal > 0) {
		RTMP_Log(RTMP_LOGERROR, "packets (%d) not released", _packetsTotal);
	}
    
	if(rtmp_url != PNULL)
	{
	   	rtmp_free(rtmp_url);
	    rtmp_url = PNULL;
	}

	if(_mutexPtr != NULL) {
		SCI_DeleteMutex(_mutexPtr);
		_mutexPtr = NULL;
	}

    RTMP_Log(RTMP_LOGINFO, "rtmp: stoped \n");

	return (_playerStatus == RTMP_STATUS_STOP) ? 0 : -1;
}

void rtmp_player_set_cache_packet_count(const int count)
{
	_cacheCount = count;
}

int rtmp_player_get_status()
{
	return _playerStatus;
}

static int rtmp_player_quit_callback(void * rtmp)
{
	(void)rtmp;

	// 0 : running, other : stop rtmp
	return _quitThread;
}

static void rtmp_player_thread_proc(uint32 param, void* data)
{
	int res = 0;
	_stoped = 0;
	_quitThread = 0;
	_isPlaying = 0;
	_threadId=SCI_IdentifyThread();
	_playerStatus = RTMP_STATUS_CONNECTING;
    if (minirtmp_init(&rtmp_interface, rtmp_url, 0, rtmp_player_quit_callback)) {
        RTMP_Log(RTMP_LOGDEBUG, "error: can't open RTMP url\n");
		rtmp_free(rtmp_url);
        rtmp_url = PNULL;
		_stoped = 1;
		_playerStatus = RTMP_STATUS_ERROR;
        return;
    }

	RTMP_Log(RTMP_LOGDEBUG, "read data begin\n");
	_playerStatus = RTMP_STATUS_CACHED;
	
 	while(_quitThread == 0) {
        res = minirtmp_read(&rtmp_interface);

		if(MINIRTMP_MORE_DATA == res)
			continue;

		if (MINIRTMP_EOF == res) {
			RTMP_Log(RTMP_LOGDEBUG, "MINIRTMP_EOF\n");
            break;
		}
		
        if (MINIRTMP_OK == res)
        {
            RTMPPacket *pkt = &rtmp_interface.rtmpPacket;
			// only audio
			if (pkt->m_packetType == RTMP_PACKET_TYPE_AUDIO){
	            rtmp_player_on_packet(pkt);
			}
        }

		 // Todo : If the received data is less than a certain amount, pause and wait
		 if(_packetsTotal < _cacheCount / 2) {
		 }

		// Todo : Too much, discard or pause
 //       if(_quitThread == 0 && _packetsTotal > 100)
 //           SCI_Sleep(100);
    }

	RTMP_Log(RTMP_LOGDEBUG, "read data end, thread exit\n");
	
    minirtmp_close(&rtmp_interface);
    if(rtmp_url != PNULL)
    {
		rtmp_free(rtmp_url);
        rtmp_url = PNULL;
    }
	rtmp_player_streaming_stop();
	_playerStatus = RTMP_STATUS_STOP;
    _stoped = 1;
}


static void rtmp_player_on_packet(RTMPPacket * pkt)
{
    static int packet = 0;
    if (pkt->m_packetType == RTMP_PACKET_TYPE_AUDIO) {
        RTMP_Log(RTMP_LOGDEBUG, "packet %d %s size=%d\n", packet++, "audio", pkt->m_nBodySize);

		if(pkt->m_nBodySize > 2) {
	
			mrtmp_packet_t * p = rtmp_malloc(sizeof(mrtmp_packet_t));
			memset(p, 0, sizeof(mrtmp_packet_t));
			p->data = pkt->m_body;
            p->size = pkt->m_nBodySize;
            p->type = pkt->m_packetType;
            p->pts  = pkt->m_nTimeStamp;

			SCI_GetMutex(_mutexPtr, SCI_WAIT_FOREVER);

            if (_packets) {
                _tail->next = p;
            } else
                _packets = p;
			
            _tail = p;
            _packetsTotal++;
			
			SCI_PutMutex(_mutexPtr);
			
            if (pkt->m_body) {
                pkt->m_body = NULL;
                pkt->m_nBodySize  = 0;
                pkt->m_nBytesRead = 0;
            }

			if(_packetsTotal > _cacheCount && _isPlaying == 0) {
				_isPlaying = 1;
				rtmp_player_streaming_start();
			}

		}	
    }
	else if (pkt->m_packetType == RTMP_PACKET_TYPE_VIDEO) {
		RTMP_Log(RTMP_LOGDEBUG, "packet %d %s size=%d\n", packet++, "video", pkt->m_nBodySize);
	}
	else
        RTMP_Log(RTMP_LOGDEBUG, "packet %d type=%d size=%d\n", packet++, pkt->m_packetType, pkt->m_nBodySize);
}


static void rtmp_dec_dummy_callbak(
	HAUDIO hAudio,
	uint32 notify_info,
	uint32 affix_info)
{
	return;
}

static AUDIO_STREAM_GET_SRC_RESULT_E rtmp_player_stream_data_callback(
	HAUDIO hAudio,
	AUDIO_GETSRCDATA_INFO_T * const ptSrcDataInfo,
	void *pvOtherInfo)
{
	if (_packets == NULL || _audioHandle == 0)
		return AUDIO_STREAM_GET_SRC_ERROR;
	else {
		ptSrcDataInfo->tStreamCurSrcDataInfo.pucDataAddr = (uint8 *)_mp3Cache;
		ptSrcDataInfo->tStreamCurSrcDataInfo.uiDataLength = rtmp_player_audio_data_callback(0, _mp3Cache, 512);
		
		if (ptSrcDataInfo->tStreamCurSrcDataInfo.uiDataLength < 0)
			return AUDIO_STREAM_GET_SRC_ERROR;
		else if(ptSrcDataInfo->tStreamCurSrcDataInfo.uiDataLength == 0)
			return AUDIO_STREAM_GET_SRC_WAIT;
	}

	return AUDIO_STREAM_GET_SRC_SUCCESS;
}	

static int rtmp_player_audio_data_callback(unsigned int from_pos, unsigned char *data, unsigned int size)
{
	int wantCopy = 0;
	int hasCopied = 0;
	mrtmp_packet_t * pkt = NULL;

	if (_packets == NULL || data == NULL || size <= 0)
		return 0;

	wantCopy = size - hasCopied;
	SCI_GetMutex(_mutexPtr, SCI_WAIT_FOREVER);
	while(_stoped == 0 && _packetsTotal > 0 && wantCopy > 0) {

		pkt = _packets;
		
		if(_audioType == RTMP_AUDIO_TYPE_AAC) {		
			memcpy(data + hasCopied, (char *)pkt->data + 2, pkt->size - 2);
			hasCopied += (pkt->size - 2);
			wantCopy = size - hasCopied;
		}
		else {
			memcpy(data + hasCopied, (char *)pkt->data + 1, pkt->size - 1);
			hasCopied += (pkt->size - 1);
			wantCopy = size - hasCopied;
		}

		_packetsTotal--;
	    if (_packets)
        	_packets = _packets->next;

        if (pkt->data)
            rtmp_free((char*)pkt->data - RTMP_MAX_HEADER_SIZE);
        rtmp_free(pkt);

		// The next frame is larger than the buffer size
		if(_packets && _packets->size > wantCopy)
			break;
	}
	SCI_PutMutex(_mutexPtr);

	return hasCopied;
}

static int rtmp_player_streaming_start()
{
	_audioType = RTMP_AUDIO_TYPE_MP3;
	SCI_GetMutex(_mutexPtr, SCI_WAIT_FOREVER);
	if (_packets != NULL) {
		uint8_t *nal = (uint8_t *)_packets->data;
		if((nal[0] & 0xF0) == 0xA0) {
			_audioType = RTMP_AUDIO_TYPE_AAC;
		}
	}
	SCI_PutMutex(_mutexPtr);

	if(_audioType == RTMP_AUDIO_TYPE_AAC)
		return rtmp_player_aac_stream_start();
	else
		return rtmp_player_mp3_stream_start();
}

static int rtmp_player_streaming_stop()
{
	_isPlaying = 0;

	if(_audioHandle != 0) {		
		AUDIO_Stop(_audioHandle);
		AUDIO_CloseHandle(_audioHandle);
		_audioHandle = 0;
	}
	return 0;
}

static int rtmp_player_mp3_stream_start() 
{
	rtmp_mp3_frame_info_t info = {0};

	AUDIO_RESULT_E result = AUDIO_NO_ERROR;
	uint32 track_buf_size = (80 * 150); // VB_DECODEOUTPUTBUFSIZ

	rtmp_player_streaming_stop();
	if (_packets == NULL || _packetsTotal <= 0) {
		_playerStatus = RTMP_STATUS_ERROR;
		return -1;
	}

	SCI_GetMutex(_mutexPtr, SCI_WAIT_FOREVER);
	if (_packets != NULL) {
		uint8_t *nal = (uint8_t *)_packets->data;
		rtmp_mp3_decode_frame_info(nal + 1, _packets->size - 1, &info);
		RTMP_Log(RTMP_LOGDEBUG, "mp3 info : channel %d, sampleRate %d\n", info.channels, info.hz);
	}
	SCI_PutMutex(_mutexPtr);
	 
	if (info.hz == 0) {
		info.channels = 1;
		info.hz = 44100;
	}

	mp3_header_info.bType = TRUE;
	mp3_header_info.unMp3StreamData.tMp3FormatInfo.uiChannelNum = info.channels;
	mp3_header_info.unMp3StreamData.tMp3FormatInfo.uiSampleRate = info.hz;

	_audioHandle = AUDIO_CreateStreamBufferHandle(
		hMP3DSPCodec,
		PNULL,
		hARMVB,
		PNULL,
		(uint8 *)&mp3_header_info,
		sizeof(MP3_STREAM_BUFFER_T),
		(uint32 *)(VBDecodeOuputBuf),
		track_buf_size,
		rtmp_dec_dummy_callbak,
		rtmp_player_stream_data_callback
	);

	if (_audioHandle == 0) {
		_playerStatus = RTMP_STATUS_ERROR;
		return -1;
	}

	AUDIO_SetDevMode(AUDIO_DEVICE_MODE_HANDFREE);
	AUDIO_SetVolume(_currentVolume);

	result = AUDIO_Play(_audioHandle, 0);
	_playerStatus = (result == AUDIO_NO_ERROR || result == AUDIO_PLAYING) ? RTMP_STATUS_PLAYING : RTMP_STATUS_ERROR;
	return 0; //(result == AUDIO_NO_ERROR || result == AUDIO_PLAYING)
}


static int rtmp_player_aac_stream_start()
{
    const uint32 sample_rates[] = {96000,88200,64000,48000,44100,32000,24000,22050,16000,12000,11025,8000,7350,0,0,0};
	AAC_LC_HEADER_T header_info = {44100, 2, 2, 0, 0};

	AUDIO_RESULT_E result = AUDIO_NO_ERROR;
	uint32 track_buf_size = (80 * 150); // VB_DECODEOUTPUTBUFSIZ

	rtmp_player_streaming_stop();
	if (_packets == NULL || _packetsTotal <= 0) {
		_playerStatus = RTMP_STATUS_ERROR;
		return -1;
	}

	SCI_GetMutex(_mutexPtr, SCI_WAIT_FOREVER);
	if (_packets != NULL && _packets->size >= 4) {
		uint8_t *nal = (uint8_t *)_packets->data;

		// AAC sequence header
		if(nal[1] == 0) {
			mrtmp_packet_t * pkt = _packets;

			_packetsTotal--;
   	    	_packets = _packets->next;
            rtmp_free((char*)pkt->data - RTMP_MAX_HEADER_SIZE);
        	rtmp_free(pkt);
		}
	}
	SCI_PutMutex(_mutexPtr);
	 
	_audioHandle = AUDIO_CreateStreamBufferHandle(
		hAACLCCodec,
		PNULL,
		hARMVB,
		PNULL,
		(uint8 *)&header_info,
		sizeof(AAC_LC_HEADER_T),
		(uint32 *)(VBDecodeOuputBuf),
		track_buf_size,
		rtmp_dec_dummy_callbak,
		rtmp_player_stream_data_callback
	);

	if (_audioHandle == 0) {
		_playerStatus = RTMP_STATUS_ERROR;
		return -1;
	}

	AUDIO_SetDevMode(AUDIO_DEVICE_MODE_HANDFREE);
	AUDIO_SetVolume(_currentVolume);
	
	result = AUDIO_Play(_audioHandle, 0);
	_playerStatus = (result == AUDIO_NO_ERROR || result == AUDIO_PLAYING) ? RTMP_STATUS_PLAYING : RTMP_STATUS_ERROR;
	return 0; //(result == AUDIO_NO_ERROR || result == AUDIO_PLAYING)

}



