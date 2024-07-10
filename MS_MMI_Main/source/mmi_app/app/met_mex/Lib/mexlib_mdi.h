/************************************************************************
* 版权所有 (C)2010,扬讯科技。
* 
* 文件名称： // MexLib_Mdi.h
* 文件标识：
* 内容摘要： //mex平台声音相关库
*
************************************************************************/


#if defined(MEX_TARGET_SERVER) && !defined(__MEXLIB_MDI_SERVER_H__) || defined(MEX_TARGET_CLIENT) && !defined(__MEXLIB_MDI_CLIENT_H__)

#if defined(MEX_TARGET_SERVER)
	#define __MEXLIB_MDI_SERVER_H__
#elif defined(MEX_TARGET_CLIENT)
	#define __MEXLIB_MDI_CLIENT_H__
#endif

//common macro definitions for MDI
#define MEX_H_LAUNCH_FUNC_MDI					MEX_H_LAUNCH_FUNC( Mdi )
#define MEX_C_LIB_FUNC_NAME_MDI					MEX_C_LIB_FUNC_NAME( Mdi )

#define MEX_H_GET_FUNC_DEFINE_MDI(x)				MEX_H_GET_FUNC_DEFINE( Mdi, x )

#define MEX_H_ENUM_FUNC_START_MDI				MEX_H_ENUM_FUNC_START( Mdi )
#define MEX_H_ENUM_FUNC_MDI(x)					MEX_H_ENUM_FUNC( Mdi, x )
#define MEX_H_ENUM_FUNC_END_MDI					MEX_H_ENUM_FUNC_END( Mdi )

#define MEX_H_NB_FUNC_MDI						MEX_H_NB_FUNC( Mdi )
//end common macro definitions

/* MEX MDI AUD MODE */
typedef enum
{
    E_MAM_MODE_IDLE,                                    // 空闲中
    E_MAM_MODE_NORMAL,                                  // 普通模式，SOUND PLAY
    E_MAM_MODE_PCMPLAY,                                 // PCM 流播放模式
    E_MAM_MODE_RECORD,                                  // PCM 流录音模式
    E_MAM_MODE_PCMPLAY_AND_RECORD                       // 播放并录音
} MEX_AUDIO_MODE;

#if defined(MEX_TARGET_SERVER)
	#define MEX_MDI_OUTPUT_SPEAKER							( mex_ConvertSysDef(E_M2MDC_MDI_DEVICE_SPEAKER) )
	#define MEX_MDI_OUTPUT_SPEAKER2							( mex_ConvertSysDef(E_M2MDC_MDI_DEVICE_SPEAKER2) )
	#define MEX_MDI_OUTPUT_LOUDSPEAKER						( mex_ConvertSysDef(E_M2MDC_MDI_DEVICE_LOUDSPEAKER) )
	#define MEX_MDI_OUTPUT_SPEAKER_BOTH						( mex_ConvertSysDef(E_M2MDC_MDI_DEVICE_SPEAKER_BOTH) )
	#define MEX_MDI_OUTPUT_BT_HEADSET						( mex_ConvertSysDef(E_M2MDC_MDI_DEVICE_BT_HEADSET) )
	#define MEX_MDI_OUTPUT_DEFAULT							( mex_ConvertSysDef(E_M2MDC_MDI_DEVICE_AUTO_SELECT) )
#elif defined(MEX_TARGET_CLIENT)
	#define MEX_MDI_OUTPUT_SPEAKER							( ConvertSysDef(E_M2MDC_MDI_DEVICE_SPEAKER) )
	#define MEX_MDI_OUTPUT_SPEAKER2							( ConvertSysDef(E_M2MDC_MDI_DEVICE_SPEAKER2) )
	#define MEX_MDI_OUTPUT_LOUDSPEAKER						( ConvertSysDef(E_M2MDC_MDI_DEVICE_LOUDSPEAKER) )
	#define MEX_MDI_OUTPUT_SPEAKER_BOTH						( ConvertSysDef(E_M2MDC_MDI_DEVICE_SPEAKER_BOTH) )
	#define MEX_MDI_OUTPUT_BT_HEADSET						( ConvertSysDef(E_M2MDC_MDI_DEVICE_BT_HEADSET) )
	#define MEX_MDI_OUTPUT_DEFAULT							( ConvertSysDef(E_M2MDC_MDI_DEVICE_AUTO_SELECT) )	
#endif


// MEX SOUND OUTPUT PATH
typedef enum
{
    E_MSOP_OUTPUT_SPEAKER,                              // Tone, Keypad sound to play on audio device
    E_MSOP_OUTPUT_SPEAKER2,                             // earphone, carkit
    E_MSOP_OUTPUT_LOUDSPEAKER,                          // loudspeaker for free sound
    E_MSOP_OUTPUT_SPEAKER_BOTH,                         // 
    E_MSOP_OUTPUT_BT_HEADSET,                           // not support yet, used to be OR bitwise with above audio output devices
    E_MSOP_OUTPUT_DEFAULT                               // default output path
} MEX_SOUND_OUTPUT_PATH;



typedef enum 
{
	E_MST_SOUND_MIDI,
	E_MST_SOUND_WAV,
	E_MST_SOUND_MP3,
	E_MST_SOUND_AMR,                                    // not support yet
	E_MST_SOUND_PCM 	                                // not support yet
} MEX_SOUND_TYPE;

typedef enum
{
	E_MSPT_PLAY_LOOP,									//loop play
	E_MSPT_PLAY_ONCE									//play only once
} MEX_SOUND_PLAY_TYPE;

//sound play state
typedef enum
{
	E_MSS_SOUND_IDLE,									//空闲
	E_MSS_SOUND_PLAYING,                                //声音当前正在播放
	E_MSS_SOUND_STOPED,                                 //声音当前已经停止
	E_MSS_SOUND_PAUSED,                                 //声音当前已经暂停
	E_MSS_SOUND_RESTORE									//重建文件 需重新播放
} MEX_SOUND_STATE;

//sound volume level
typedef enum
{
	E_MSV_LEVEL_0 = 0,		
	E_MSV_LEVEL_1,
	E_MSV_LEVEL_2,
	E_MSV_LEVEL_3,
	E_MSV_LEVEL_4,
	E_MSV_LEVEL_5,
	E_MSV_LEVEL_6,
	E_MSV_LEVEL_MAX
} MEX_SOUND_VOLUME;

#define MEX_DIR_BUFFER_LENGTH MEX_FILE_NAME_MAX_LENGTH

//mex sound struct
typedef struct
{
	wchar_t				m_sPath[MEX_DIR_BUFFER_LENGTH];			//声音文件路径
	uint32  			m_iLength;								//声音数据长度
	int8 				m_iType;								//声音格式类型
	void* 				m_pData;								//声音信息内容
} Mex_Sound_Data;

typedef Mex_Sound_Data * MEX_MDI_HANDLE;

typedef void (*audio_cb)(void *);

// mex audio struct
typedef struct  
{
	wchar_t				sPath[MEX_DIR_BUFFER_LENGTH];			//声音数据文件路径
	uint32				uDuration;								//声音播放总时间 单位ms
	uint32				uNowPlayedTime;							//当前已播放时间	单位ms ，默认初始化为0. 在Play 为相对文件的偏移量
	int8				eType;									//声音数据类型
	uint32				uVolume;								//当前类型
	funcptr				fpPlayNotify;							//播放过程中的回调  播放完成 播放错误 或者是 播放时间更新
	int32				iAppid;									//调用者id
}Mex_Audio_Struct;


/* mex record state */
typedef enum
{
	E_MRS_RECORD_RECORDING,                              /* 正在录音 */
	E_MRS_RECORD_STOPED,                                 /* 录音已停止 */
	E_MRS_RECORD_PAUSED                                  /* 录音暂停 */
} MEX_RECORD_STATE;

// mex record format
typedef enum
{
    E_MRF_RECORD_WAV_8K,                                 // for E_MRT_RECORD_FILE
    E_MRF_RECORD_WAV_16K,                                // for E_MRT_RECORD_FILE
    E_MRF_RECORD_PCM_8K,                                 // for E_MRT_RECORD_STREAM
    E_MRF_RECORD_PCM_16K,                                // for E_MRT_RECORD_STREAM

#if MEX_SDK_VER >= 1005
    E_MRF_RECORD_AMR                                     // for E_MRT_RECORD_STREAM
#endif /* MEX_SDK_VER */
    
} MEX_RECORD_FORMAT;

/* mex record type */
typedef enum
{
    E_MRT_RECORD_FILE,                                   // FILE 方式录音
    E_MRT_RECORD_STREAM                                  // PCM 流式录音
} MEX_RECORD_TYPE;

/* mex record request struct */
typedef struct
{
    const wchar_t      *path;                            // 录音文件路径
    uint32              pathLen;                         // 录音文件路径长度
    MEX_RECORD_FORMAT   format;                          // 
    MEX_RECORD_TYPE     type;                            // 
} Mex_Record_Req_Struct;

// 录音接口
// 开始录音
// voice_record_start
// 停止录音
// voice_record_stop
// 暂停与恢复录音
// voice_record_suspend
// 获取已录数据缓冲区
// voice_record_get_read_buffer
// 读取已录数据完成
// voice_record_read_data_done
// 获得MEX_RECORD_STATE
// get_voiceRecordState

/* MEX PCM STATE */
typedef enum
{
    E_MPS_PCM_PLAYING = 0,       /* 正在播放 */
    E_MPS_PCM_STOPED  = 1,       /* 已停止 */
    E_MPS_PCM_PAUSED  = 2        /* 已暂停 */
} MEX_PCM_STATE;

/* MEX PCM PLAY REQ STRUCT */
typedef struct
{
    /* PCM Stream Param */
    bool       isStereo;        /* 立体声标志 */
    int8       bitPerSample;    /* 每一个采样点所包含的比特数 */
    uint16     sampleFreq;      /* 采样频率 */

    /* Read pcm data */
    bool (*read_pcm_data) (uint8 *data_p, uint32 *len_p);
} Mex_Pcm_Play_Req_Struct;

// PCM 播放接口
// PCM 开始播放
// pcm_play
// PCM 停止播放
// pcm_stop
// PCM 暂停与恢复
// pcm_suspend
// 获得MEX_PCM_STATE
// get_pcmState

// 获得MEX AUDIO MODE
// get_audioMode
// 设置MEX AUDIO MODE
// set_audioMode

// MEX SOUND INPUT SOURCE
typedef enum
{
    E_MSIS_MICROPHONE1,         // 手机的Microphone
    E_MSIS_MICROPHONE2,         // 耳机的Microphone
    E_MSIS_DEFAULT,             // 默认的Microphone
    
    E_MSIS_MAX
} MEX_SOUND_INPUT_SOURCE;

#if MEX_SDK_VER >= 1005

typedef void* Mex_Audio_Stream;

typedef enum
{
    E_MASE_TERMINATED,
    E_MASE_END_OF_STREAM,
    E_MASE_BAD_FORMAT,
    E_MASE_UNKNOWN_ERROR,
    
    E_MASE_MAX
} E_MEX_Audio_Stream_Event;

typedef void (*Mex_Audio_Stream_CB) (E_MEX_Audio_Stream_Event eEvent);

// 获取音频数据的回调函数
// 参数1: pAudioStream 音频流指针
// 参数2: pDataBuf 音频数据缓冲区
// 参数3: iBufLen 音频数据缓冲区大小，按字节计算
// 返回: 已填充音频数据的大小，负数表示音频流结束
typedef int (*Mex_Audio_Stream_Provider) (Mex_Audio_Stream pAudioStream, uint8 *pDataBuf, uint32 iBufLen);

typedef enum
{
    E_MASF_MIDI,
    E_MASF_WAV,
    E_MASF_MP3,
    E_MASF_AMR,

    E_MASF_MAX
} E_MEX_Audio_Stream_Format;

#endif /* MEX_SDK_VER */

#if defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
#if MEX_SDK_VER >= 1012

typedef struct
{    
    uint8      isStartMix;          // 开始混音标志
    uint8      aud_volume;         //混音时音频的音量
    uint8      mic_volume;         //混音时话筒的音量
    uint8      is_aud_muse;        //音频是否静音
    uint8      is_mic_muse;        //话筒是否静音
    uint8      is_right_channel_on;   //声道选择，左声道为仅伴奏音，右声道为原唱
    uint8      is_discard_mic_data;  //指示是否丢弃MIC缓冲区中的数据
    uint8      is_discard_track_data;  //指示是否丢弃DA缓冲区中的数据    
} Mex_Mix_Control_Para_Struct;

typedef enum
{
    MEX_MIX_NOTIFY_TYPE_HEADSET_STATE,
    MEX_MIX_NOTIFY_TYPE_NUM,    
}MEX_MIX_NOTIFY_TYPE_E;

typedef struct
{    
    int        iNotifyType;         //通知的消息类型

    union
    {
        uint8  isPlugIn;          // 开始混音标志
    }para;
    
} Mex_Mix_Notify_Para_Struct;

// 获取音频数据的回调函数
// 参数1: pAudioStream 音频流指针
// 参数2: pDataBuf 音频数据缓冲区
// 参数3: iBufLen 音频数据缓冲区大小，按字节计算
// 返回: 已填充音频数据的大小，负数表示音频流结束
typedef int    (*Mex_Mic_Open_fp) (uint32 uiSampleRate,uint32 uiInputDevice);
typedef int    (*Mex_Mic_Close_fp)();
typedef void   (*Mex_Mic_Set_SampleRate_fp) (uint32 uiSampleRate);
typedef void   (*Mex_Mic_Set_InputDevice_fp) (uint32 uiInputDevice);
typedef void   (*Mex_Mic_Clean_Queue_fp) ();

typedef void   (*Mex_Mix_Notify_Callback_fp) (void *pNotifyPara);

typedef int    (*Mex_Mix_Open_fp) (Mex_Mix_Notify_Callback_fp callback);
typedef int    (*Mex_Mix_Close_fp) ();
typedef Mex_Mix_Control_Para_Struct *(*Mex_Mix_Get_Para_fp) (Mex_Mix_Control_Para_Struct *pMixPara);
typedef void   (*Mex_Mix_Set_Para_fp) (Mex_Mix_Control_Para_Struct *pMixPara);



typedef struct
{    
    /*话筒控制接口*/
    Mex_Mic_Open_fp              mic_open;               // 开始混音标志
    Mex_Mic_Close_fp             mic_close;              //关闭mic
    Mex_Mic_Set_SampleRate_fp    mic_set_samplerate;     //设置采样率
    Mex_Mic_Set_InputDevice_fp   mic_set_input_device;   //设置输入设备
    Mex_Mic_Clean_Queue_fp       mic_clean_queue;        //清除输入数据缓冲区

    /*混音控制*/    
    Mex_Mix_Open_fp              mix_open;    //关闭mic
    Mex_Mix_Close_fp             mix_close;     // 开始混音标志
    Mex_Mix_Get_Para_fp          mix_get_para;    //关闭mic
    Mex_Mix_Set_Para_fp          mix_set_para;     // 开始混音标志    
} Mex_Mix_Control_Interface_Struct;

#endif /* MEX_SDK_VER */
#endif

MEX_H_DECLARE_FUNC( MEX_MDI_HANDLE,		sound_create,		(wchar_t* filePath, MEX_SOUND_TYPE type) )
MEX_H_DECLARE_FUNC( bool, 				sound_play,			(MEX_MDI_HANDLE sound , bool once) )
MEX_H_DECLARE_FUNC( bool,				sound_stop, 		(MEX_MDI_HANDLE sound) )
MEX_H_DECLARE_FUNC( bool,				sound_suspend, 		(MEX_MDI_HANDLE sound , bool pause) )
MEX_H_DECLARE_FUNC( void,				sound_close, 		(MEX_MDI_HANDLE sound) )

MEX_H_DECLARE_FUNC( MEX_SOUND_STATE,        get_soundState,      (void) )
MEX_H_DECLARE_FUNC( MEX_SOUND_VOLUME,       get_soundVolume,     (void) )
MEX_H_DECLARE_FUNC( void,                   set_soundVolume,     (MEX_SOUND_VOLUME volume) )
MEX_H_DECLARE_FUNC( MEX_SOUND_OUTPUT_PATH,  get_soundOutputPath, (void) )
MEX_H_DECLARE_FUNC( void,                   set_soundOutputPath, (MEX_SOUND_OUTPUT_PATH path) )

/* mex record */
MEX_H_DECLARE_FUNC( bool,               voice_record_start,             (Mex_Record_Req_Struct* req) )
MEX_H_DECLARE_FUNC( void,               voice_record_stop,              (void) )
MEX_H_DECLARE_FUNC( bool,               voice_record_suspend,           (bool pause) )
MEX_H_DECLARE_FUNC( void,               voice_record_get_read_buffer,   (uint8 **dataBuf, uint32 *dataLen) )
MEX_H_DECLARE_FUNC( void,               voice_record_read_data_done,    (uint32 dataLen) )
MEX_H_DECLARE_FUNC( MEX_RECORD_STATE,   get_voiceRecordState,           (void) )

/* mex pcm play interface */
MEX_H_DECLARE_FUNC( bool,               pcm_play,                       (Mex_Pcm_Play_Req_Struct* req) )
MEX_H_DECLARE_FUNC( void,               pcm_stop,                       (void) )
MEX_H_DECLARE_FUNC( bool,               pcm_suspend,                    (bool pause) )
MEX_H_DECLARE_FUNC( MEX_PCM_STATE,      get_pcmState,                   (void) )

/* mex audio mode */
MEX_H_DECLARE_FUNC( MEX_AUDIO_MODE,     get_audioMode,                  (void) )
MEX_H_DECLARE_FUNC( bool,               set_audioMode,                  (MEX_AUDIO_MODE audioMode) )

/* mex audio player api*/
MEX_H_DECLARE_FUNC( Mex_Audio_Struct *,	audio_init,						(wchar_t* filePath,audio_cb fpAudioNofity, int32 app_id) )
MEX_H_DECLARE_FUNC( void,				audio_deInit,					(Mex_Audio_Struct * audio_struct,int32 app_id) )
MEX_H_DECLARE_FUNC( uint32,				audio_getduration,				(Mex_Audio_Struct * audio_struct) )
MEX_H_DECLARE_FUNC( uint32,				audio_get_nowplayedtime,		(Mex_Audio_Struct * audio_struct) )
MEX_H_DECLARE_FUNC( int8,				audio_seek_to_target,			(Mex_Audio_Struct * audio_struct,uint32 target_time) )
MEX_H_DECLARE_FUNC( uint32,				audio_get_volume,				(void) )
MEX_H_DECLARE_FUNC( void,				audio_set_volume,				(uint32 volume) )
MEX_H_DECLARE_FUNC( int8,				audio_play,						(Mex_Audio_Struct * audio_struct) )
MEX_H_DECLARE_FUNC( int8,				audio_stop,						(Mex_Audio_Struct * audio_struct) )
MEX_H_DECLARE_FUNC( int8,				audio_pause,					(Mex_Audio_Struct * audio_struct) )
MEX_H_DECLARE_FUNC( int8,				audio_resume,					(Mex_Audio_Struct * audio_struct) )
MEX_H_DECLARE_FUNC( int8,				audio_restore,					(Mex_Audio_Struct * audio_struct) )
MEX_H_DECLARE_FUNC( int8,				audio_start_calc_spectrum,		(void) )
MEX_H_DECLARE_FUNC( int8,				audio_stop_calc_spectrum,		(void) )
MEX_H_DECLARE_FUNC( int8,				audio_get_spectrum,				(uint8 *top_p,uint8 *val_p) )

/* mex sound input source , microphone volume */
MEX_H_DECLARE_FUNC( MEX_SOUND_INPUT_SOURCE, get_soundInputSource,       (void) )
MEX_H_DECLARE_FUNC( void,                   set_soundInputSource,       (MEX_SOUND_INPUT_SOURCE eSource) )
MEX_H_DECLARE_FUNC( uint8,                  get_microphoneVolume,       (void) )
MEX_H_DECLARE_FUNC( void,                   set_microphoneVolume,       (uint8 volume) )

MEX_H_DECLARE_FUNC( bool,					sound_is_busy,		(void) )
MEX_H_DECLARE_FUNC( uint32,                 voice_record_get_data_count, (void) )
#if MEX_SDK_VER >= 1005
MEX_H_DECLARE_FUNC( Mex_Audio_Stream,             audio_stream_init,            (void *pAudioData, uint16 nDataLen, E_MEX_Audio_Stream_Format eFormat, MEX_SOUND_PLAY_TYPE ePlayType, Mex_Audio_Stream_CB pPlayNotify) )
MEX_H_DECLARE_FUNC( void,                         audio_stream_deInit,          (Mex_Audio_Stream pAudioStream) )
MEX_H_DECLARE_FUNC( bool,                         audio_stream_play,            (Mex_Audio_Stream pAudioStream) )
MEX_H_DECLARE_FUNC( bool,                         audio_stream_stop,            (Mex_Audio_Stream pAudioStream) )
#endif /* MEX_SDK_VER */
#if MEX_SDK_VER >= 1010
MEX_H_DECLARE_FUNC( bool,                         set_audio_stream_provider,    (Mex_Audio_Stream pAudioStream, Mex_Audio_Stream_Provider pProvider) )
#endif
#if MEX_SDK_VER >= 1032
MEX_H_DECLARE_FUNC( void,					audio_stop_background_play,		(void) )
#endif


MEX_H_ENUM_FUNC_START_MDI
	MEX_H_ENUM_FUNC_MDI( sound_create )
	MEX_H_ENUM_FUNC_MDI( sound_play )
	MEX_H_ENUM_FUNC_MDI( sound_stop )
	MEX_H_ENUM_FUNC_MDI( sound_suspend )
	MEX_H_ENUM_FUNC_MDI( sound_close )

	MEX_H_ENUM_FUNC_MDI( get_soundState )
	MEX_H_ENUM_FUNC_MDI( get_soundVolume )
	MEX_H_ENUM_FUNC_MDI( set_soundVolume )
    MEX_H_ENUM_FUNC_MDI( get_soundOutputPath )
    MEX_H_ENUM_FUNC_MDI( set_soundOutputPath )

	/* mex record */
    MEX_H_ENUM_FUNC_MDI( voice_record_start )
    MEX_H_ENUM_FUNC_MDI( voice_record_stop )
    MEX_H_ENUM_FUNC_MDI( voice_record_suspend )
    MEX_H_ENUM_FUNC_MDI( voice_record_get_read_buffer )
    MEX_H_ENUM_FUNC_MDI( voice_record_read_data_done )
    MEX_H_ENUM_FUNC_MDI( get_voiceRecordState )

    /* mex pcm play */
    MEX_H_ENUM_FUNC_MDI( pcm_play )
    MEX_H_ENUM_FUNC_MDI( pcm_stop )
    MEX_H_ENUM_FUNC_MDI( pcm_suspend )
    MEX_H_ENUM_FUNC_MDI( get_pcmState )

    /* mex audio mode */
    MEX_H_ENUM_FUNC_MDI( get_audioMode)
    MEX_H_ENUM_FUNC_MDI( set_audioMode)

	/* mex audio player api */
	MEX_H_ENUM_FUNC_MDI( audio_init )
	MEX_H_ENUM_FUNC_MDI( audio_deInit )
	MEX_H_ENUM_FUNC_MDI( audio_getduration )
	MEX_H_ENUM_FUNC_MDI( audio_get_nowplayedtime )
	MEX_H_ENUM_FUNC_MDI( audio_seek_to_target )
	MEX_H_ENUM_FUNC_MDI( audio_get_volume )
	MEX_H_ENUM_FUNC_MDI( audio_set_volume )
	MEX_H_ENUM_FUNC_MDI( audio_play )
	MEX_H_ENUM_FUNC_MDI( audio_stop )
	MEX_H_ENUM_FUNC_MDI( audio_pause )
	MEX_H_ENUM_FUNC_MDI( audio_resume )
	MEX_H_ENUM_FUNC_MDI( audio_restore )
	MEX_H_ENUM_FUNC_MDI( audio_start_calc_spectrum )
	MEX_H_ENUM_FUNC_MDI( audio_stop_calc_spectrum )
	MEX_H_ENUM_FUNC_MDI( audio_get_spectrum )

    /* mex sound input source , microphone volume */
	MEX_H_ENUM_FUNC_MDI( get_soundInputSource )
	MEX_H_ENUM_FUNC_MDI( set_soundInputSource )
	MEX_H_ENUM_FUNC_MDI( get_microphoneVolume )
	MEX_H_ENUM_FUNC_MDI( set_microphoneVolume )
	
	MEX_H_ENUM_FUNC_MDI( sound_is_busy )
    MEX_H_ENUM_FUNC_MDI( voice_record_get_data_count )
#if MEX_SDK_VER >= 1005
    MEX_H_ENUM_FUNC_MDI( audio_stream_init )
    MEX_H_ENUM_FUNC_MDI( audio_stream_deInit )
    MEX_H_ENUM_FUNC_MDI( audio_stream_play )
    MEX_H_ENUM_FUNC_MDI( audio_stream_stop )
#endif /* MEX_SDK_VER */
#if MEX_SDK_VER >= 1010
    MEX_H_ENUM_FUNC_MDI( set_audio_stream_provider )
#endif
#if MEX_SDK_VER >= 1032
	 MEX_H_ENUM_FUNC_MDI( audio_stop_background_play )
#endif
MEX_H_ENUM_FUNC_END_MDI


#if defined (MEX_TARGET_CLIENT)
	#define sound_create			MEX_H_GET_FUNC_DEFINE_MDI( sound_create )		//(wchar_t* filePath, MEX_SOUND_TYPE type)
	#define sound_play				MEX_H_GET_FUNC_DEFINE_MDI( sound_play )			//(MEX_MDI_HANDLE sound , bool once)
	#define sound_stop				MEX_H_GET_FUNC_DEFINE_MDI( sound_stop )			//(MEX_MDI_HANDLE sound)
	#define sound_suspend			MEX_H_GET_FUNC_DEFINE_MDI( sound_suspend )		//(MEX_MDI_HANDLE sound , bool pause)
	#define sound_close				MEX_H_GET_FUNC_DEFINE_MDI( sound_close )		//(MEX_MDI_HANDLE sound)
	
	#define get_soundState			MEX_H_GET_FUNC_DEFINE_MDI( get_soundState ) 	//(void)
	#define get_soundVolume			MEX_H_GET_FUNC_DEFINE_MDI( get_soundVolume )	//(void)
	#define set_soundVolume 		MEX_H_GET_FUNC_DEFINE_MDI( set_soundVolume )	//(MEX_SOUND_VOLUME volume)
    #define get_soundOutputPath     MEX_H_GET_FUNC_DEFINE_MDI( get_soundOutputPath )
    #define set_soundOutputPath     MEX_H_GET_FUNC_DEFINE_MDI( set_soundOutputPath )
    
	/* mex record */
    #define voice_record_start              MEX_H_GET_FUNC_DEFINE_MDI( voice_record_start )
    #define voice_record_stop               MEX_H_GET_FUNC_DEFINE_MDI( voice_record_stop )
    #define voice_record_suspend            MEX_H_GET_FUNC_DEFINE_MDI( voice_record_suspend )
    #define voice_record_get_read_buffer    MEX_H_GET_FUNC_DEFINE_MDI( voice_record_get_read_buffer )
    #define voice_record_read_data_done     MEX_H_GET_FUNC_DEFINE_MDI( voice_record_read_data_done )
    #define get_voiceRecordState            MEX_H_GET_FUNC_DEFINE_MDI( get_voiceRecordState )

    /* mex pcm play */
    #define pcm_play                        MEX_H_GET_FUNC_DEFINE_MDI( pcm_play )
    #define pcm_stop                        MEX_H_GET_FUNC_DEFINE_MDI( pcm_stop )
    #define pcm_suspend                     MEX_H_GET_FUNC_DEFINE_MDI( pcm_suspend )
    #define get_pcmState                    MEX_H_GET_FUNC_DEFINE_MDI( get_pcmState )

    /* mex audio mode */
    #define get_audioMode                   MEX_H_GET_FUNC_DEFINE_MDI( get_audioMode )
    #define set_audioMode                   MEX_H_GET_FUNC_DEFINE_MDI( set_audioMode )

	#define audio_init						MEX_H_GET_FUNC_DEFINE_MDI( audio_init )
	#define audio_deInit					MEX_H_GET_FUNC_DEFINE_MDI( audio_deInit )
	#define audio_getduration				MEX_H_GET_FUNC_DEFINE_MDI( audio_getduration )
	#define audio_get_nowplayedtime			MEX_H_GET_FUNC_DEFINE_MDI( audio_get_nowplayedtime )
	#define audio_seek_to_target			MEX_H_GET_FUNC_DEFINE_MDI( audio_seek_to_target )
	#define audio_get_volume				MEX_H_GET_FUNC_DEFINE_MDI( audio_get_volume )
	#define audio_set_volume				MEX_H_GET_FUNC_DEFINE_MDI( audio_set_volume )
	#define audio_play						MEX_H_GET_FUNC_DEFINE_MDI( audio_play )
	#define audio_stop						MEX_H_GET_FUNC_DEFINE_MDI( audio_stop )
	#define audio_pause						MEX_H_GET_FUNC_DEFINE_MDI( audio_pause )
	#define audio_resume					MEX_H_GET_FUNC_DEFINE_MDI( audio_resume )
	#define audio_restore					MEX_H_GET_FUNC_DEFINE_MDI( audio_restore )
	#define audio_start_calc_spectrum		MEX_H_GET_FUNC_DEFINE_MDI( audio_start_calc_spectrum )
	#define audio_stop_calc_spectrum		MEX_H_GET_FUNC_DEFINE_MDI( audio_stop_calc_spectrum )
	#define audio_get_spectrum				MEX_H_GET_FUNC_DEFINE_MDI( audio_get_spectrum )

    /* mex sound input source , microphone volume */
	#define get_soundInputSource				MEX_H_GET_FUNC_DEFINE_MDI( get_soundInputSource )
	#define set_soundInputSource				MEX_H_GET_FUNC_DEFINE_MDI( set_soundInputSource )
	#define get_microphoneVolume				MEX_H_GET_FUNC_DEFINE_MDI( get_microphoneVolume )
	#define set_microphoneVolume				MEX_H_GET_FUNC_DEFINE_MDI( set_microphoneVolume )

	#define sound_is_busy				MEX_H_GET_FUNC_DEFINE_MDI( sound_is_busy )
	#define voice_record_get_data_count MEX_H_GET_FUNC_DEFINE_MDI( voice_record_get_data_count )
#if MEX_SDK_VER >= 1005
    #define audio_stream_init            MEX_H_GET_FUNC_DEFINE_MDI( audio_stream_init )
    #define audio_stream_deInit          MEX_H_GET_FUNC_DEFINE_MDI( audio_stream_deInit )
    #define audio_stream_play            MEX_H_GET_FUNC_DEFINE_MDI( audio_stream_play )
    #define audio_stream_stop            MEX_H_GET_FUNC_DEFINE_MDI( audio_stream_stop )
#endif /* MEX_SDK_VER */
#if MEX_SDK_VER >= 1010
    #define set_audio_stream_provider        MEX_H_GET_FUNC_DEFINE_MDI( set_audio_stream_provider )
#endif /* MEX_SDK_VER */
#if MEX_SDK_VER >= 1032
	#define audio_stop_background_play        MEX_H_GET_FUNC_DEFINE_MDI( audio_stop_background_play )	
#endif
#endif

#if defined(_MEX_COMMENTS_NEVER_DEFINE_THIS_)

//创建WAV,MIDI,mp3三种格式音频，返回音频句柄。
//参数1：音频文件路径，资源则传资源路径。
//参数2：音频格式，参见enum MEX_SOUND_TYPE。
//返回 ：音频句柄
MEX_MDI_HANDLE sound_create (wchar_t* filePath, MEX_SOUND_TYPE type);

//说明：播放一段音频。非全屏应用检查，当前繁忙则不能播放自己的声音
//参数1：音频句柄，由sound_create 创建。
//参数2：是否一次播放，否则就循环播放。
//返回 ：播放是否成功
bool sound_play (MEX_MDI_HANDLE sound , bool once);

//说明：停止一段音频。
//参数1：音频句柄，由sound_create 创建。
//返回 ：停止是否成功
bool sound_stop (MEX_MDI_HANDLE sound);

//说明：暂停或继续一段音频。
//参数1：音频句柄，由sound_create 创建。
//参数2：是否暂停播放，否则就继续播放。
//返回 ：暂停或继续是否成功
bool sound_suspend (MEX_MDI_HANDLE sound , bool pause);

//说明：关闭一段音频。
//参数1：音频句柄，由sound_create 创建。
//返回 ：无
void sound_close (MEX_MDI_HANDLE sound);

//说明：取得当前播放状态。
//参数：无。
//返回 ：播放状态，参见enum MEX_SOUND_STATE。
MEX_SOUND_STATE get_soundState (void);

//说明：取得当前播放音量。
//参数：无。
//返回 ：播放音量，参见enum MEX_SOUND_VOLUME。
MEX_SOUND_VOLUME get_soundVolume (void);

//说明：设置当前播放音量。
//参数：播放音量，参见enum MEX_SOUND_VOLUME。
//返回 ：无。
void set_soundVolume (MEX_SOUND_VOLUME volume);

//说明：获得当前声音输出路径
//参数：无
//返回 ：当前声音输出路径
MEX_SOUND_OUTPUT_PATH get_soundOutputPath(void);

//说明：设置当前声音输出路径
//参数：新的声音输出路径
//返回 ：无
void set_soundOutputPath(MEX_SOUND_OUTPUT_PATH path);

/* mex record */
//说明：开始录音
//参数1：录音请求
//返回：是否成功
bool voice_record_start(Mex_Record_Req_Struct* req);

//说明：结束录音
//参数：无
//返回：无
void voice_record_stop(void);

//说明：暂停与恢复录音
//参数1：是则暂停录音，否则恢复录音
//返回：是否成功
bool voice_record_suspend(bool pause);

//说明：获取已录数据缓冲区
//参数1：[OUT] 指向已录数据缓冲区
//参数2：[OUT] 已录音数据的长度,  按字节计数
//返回：无
void voice_record_get_read_buffer(uint8 **dataBuf, uint32 *dataLen);

//说明：读取已录数据完成
//参数1：已处理的录音数据的长度
//返回：无
void voice_record_read_data_done(uint32 dataLen);

//说明：获得MEX_RECORD_STATE
//参数1：无
//返回：当前录音状态
MEX_RECORD_STATE get_voiceRecordState(void);

//说明：播放PCM 流
//参数1：Pcm Play request
//返回：是否成功
bool pcm_play (Mex_Pcm_Play_Req_Struct* req);

//说明：停止播放PCM 流
//参数1：无
//返回：无
void pcm_stop(void);

//说明：暂停与恢复播放PCM 流
//参数1：是则暂停播放，否则恢复播放
//返回：是否成功
bool pcm_suspend(bool pause);

//说明：获得MEX_PCM_STATE
//参数1：无
//返回：PCM 播放状态
MEX_PCM_STATE get_pcmState(void);

//说明：获得MEX AUDIO MODE
//参数1：无
//返回：当前AUDIO MODE
MEX_AUDIO_MODE get_audioMode(void);

//说明：设置MEX AUDIO MODE
//参数1：新的AUDIO MODE
//返回：是否成功
bool set_audioMode(MEX_AUDIO_MODE audioMode);

//创建一个音频结构 
//参数 filePath 音频文件的路径
//fpAudioNofity 播放过程中的回调函数 类型audio_cb  参数为int32  当为-1时说明播放错误 当为0时 播放完成 当为1时说明需要重建播放文件
//调用者id
Mex_Audio_Struct *audio_init(wchar_t* filePath,audio_cb fpAudioNofity, int32 app_id );

//销毁一个音频结构
//参数 audio_struct 需要销毁的结构
//调用者id
void audio_deInit(Mex_Audio_Struct * audio_struct,int32 app_id );

//获取当前音频播放总时间 单位ms
//参数 当前播放的音频结构
uint32 audio_getduration(Mex_Audio_Struct * audio_struct);

//获取当前音乐已播放的时间 单位ms
//参数 当前播放的音频结构
uint32 audio_get_nowplayedtime(Mex_Audio_Struct *audio_struct);

//快速跳转到某个时间去播放 
//参数audio_struct当前播放的音频结构
//参数target_time 目前时间点 单位ms
//注意：只有在播放状态下才有效
//此函数会阻塞程序的运行，最好在调用的时候将其放入一个定时器内
int8 audio_seek_to_target(Mex_Audio_Struct * audio_struct,uint32 target_time);

//获取当前音量
uint32 audio_get_volume(void);

//设置当前音量 参考MEX_SOUND_VOLUME
void audio_set_volume(uint32 volume);

//播放音乐
//返回0 说明失败
//返回1 成功
int8 audio_play(Mex_Audio_Struct * audio_struct);

//停止音乐
//返回0 说明失败
//返回1 成功
int8 audio_stop(Mex_Audio_Struct * audio_struct);

//暂停音乐
//返回0 说明失败
//返回1 成功
int8 audio_pause(Mex_Audio_Struct * audio_struct);

//恢复音乐  暂停后调用可恢复音乐播放
//返回0 说明失败
//返回1 成功
int8 audio_resume(Mex_Audio_Struct * audio_struct);

//重建音乐文件后 播放音乐
//返回0 说明失败
//返回1 成功
int8 audio_restore(Mex_Audio_Struct * audio_struct);

//开启频谱分析功能
//返回0 说明失败
//返回1 成功
//注意：调用该函数前应使用GetPhoneConfig(E_MPC_AudPly_Spectrum_Display) 检测平台是否支持频谱分析功能
int8 audio_start_calc_spectrum(void);

//开启频谱分析功能
//返回0 说明失败
//返回1 成功
int8 audio_stop_calc_spectrum(void);

//获取当前频谱分析输出数值
//top_p 大小为16
//val_p 大小为16
int8 audio_get_spectrum(uint8 *top_p,uint8 *val_p);

// 获取录音的音源
// 参数1: 无
// 返回MEX_SOUND_INPUT_SOURCE
MEX_SOUND_INPUT_SOURCE get_soundInputSource(void);

// 设置录音的音源
// 参数1: eSource 录音的音源
// 返回: 无
void set_soundInputSource(MEX_SOUND_INPUT_SOURCE eSource);

// 获取麦克的音量
// 参数1: 无
// 返回: 麦克的音量大小
uint8 get_microphoneVolume(void);

// 设置麦克的音量
// 参数1: volume 麦克音量值
// 返回: 无
void set_microphoneVolume(uint8 volume);

//检测系统是否正在播放声音
//非全屏应用检查，当前繁忙则不能播放自己的声音
bool sound_is_busy(void);

//获取录音缓冲区的数据长度
//参数1: 无
//返回: 录音缓冲区的数据长度(按字节计算)
uint32 voice_record_get_data_count(void);

// 初始化音频流
// 参数1: pAudioData 指向音频数据流
// 参数2: nDataLen 音频数据的长度
// 参数3: eFormat 音频格式
// 参数4: ePlayType 播放模式
// 参数5: pPlayNotify 播放过程中的回调  播放完成 播放错误 等等
// 返回: 成功返回音频流指针，失败则返回NULL
Mex_Audio_Stream audio_stream_init(void *pAudioData, uint16 nDataLen, E_MEX_Audio_Stream_Format eFormat, MEX_SOUND_PLAY_TYPE ePlayType, Mex_Audio_Stream_CB pPlayNotify);

// 释放音频流
// 参数1: pAudioStream 音频流指针
// 返回: 无
void audio_stream_deInit(Mex_Audio_Stream pAudioStream);

// 播放音频流
// 参数1: pAudioStream 音频流指针
// 返回: 是否成功
bool audio_stream_play(Mex_Audio_Stream pAudioStream);

// 停止播放音频流
// 参数1: pAudioStream 音频流指针
// 返回: 是否成功
bool audio_stream_stop(Mex_Audio_Stream pAudioStream);

// 设置从应用获取音频数据的回调函数
// 参数1: pAudioStream 音频流指针
// 参数2: pProvider 从应用获取音频数据的回调函数
// 返回: 是否设置成功
bool set_audio_stream_provider (Mex_Audio_Stream pAudioStream, Mex_Audio_Stream_Provider pProvider);

//停止当前播放的音乐
//参数：无
//返回;无
void audio_stop_background_play (void)

#endif

#endif

