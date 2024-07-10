/************************************************************************
* ��Ȩ���� (C)2010,��Ѷ�Ƽ���
* 
* �ļ����ƣ� // MexLib_Mdi.h
* �ļ���ʶ��
* ����ժҪ�� //mexƽ̨������ؿ�
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
    E_MAM_MODE_IDLE,                                    // ������
    E_MAM_MODE_NORMAL,                                  // ��ͨģʽ��SOUND PLAY
    E_MAM_MODE_PCMPLAY,                                 // PCM ������ģʽ
    E_MAM_MODE_RECORD,                                  // PCM ��¼��ģʽ
    E_MAM_MODE_PCMPLAY_AND_RECORD                       // ���Ų�¼��
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
	E_MSS_SOUND_IDLE,									//����
	E_MSS_SOUND_PLAYING,                                //������ǰ���ڲ���
	E_MSS_SOUND_STOPED,                                 //������ǰ�Ѿ�ֹͣ
	E_MSS_SOUND_PAUSED,                                 //������ǰ�Ѿ���ͣ
	E_MSS_SOUND_RESTORE									//�ؽ��ļ� �����²���
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
	wchar_t				m_sPath[MEX_DIR_BUFFER_LENGTH];			//�����ļ�·��
	uint32  			m_iLength;								//�������ݳ���
	int8 				m_iType;								//������ʽ����
	void* 				m_pData;								//������Ϣ����
} Mex_Sound_Data;

typedef Mex_Sound_Data * MEX_MDI_HANDLE;

typedef void (*audio_cb)(void *);

// mex audio struct
typedef struct  
{
	wchar_t				sPath[MEX_DIR_BUFFER_LENGTH];			//���������ļ�·��
	uint32				uDuration;								//����������ʱ�� ��λms
	uint32				uNowPlayedTime;							//��ǰ�Ѳ���ʱ��	��λms ��Ĭ�ϳ�ʼ��Ϊ0. ��Play Ϊ����ļ���ƫ����
	int8				eType;									//������������
	uint32				uVolume;								//��ǰ����
	funcptr				fpPlayNotify;							//���Ź����еĻص�  ������� ���Ŵ��� ������ ����ʱ�����
	int32				iAppid;									//������id
}Mex_Audio_Struct;


/* mex record state */
typedef enum
{
	E_MRS_RECORD_RECORDING,                              /* ����¼�� */
	E_MRS_RECORD_STOPED,                                 /* ¼����ֹͣ */
	E_MRS_RECORD_PAUSED                                  /* ¼����ͣ */
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
    E_MRT_RECORD_FILE,                                   // FILE ��ʽ¼��
    E_MRT_RECORD_STREAM                                  // PCM ��ʽ¼��
} MEX_RECORD_TYPE;

/* mex record request struct */
typedef struct
{
    const wchar_t      *path;                            // ¼���ļ�·��
    uint32              pathLen;                         // ¼���ļ�·������
    MEX_RECORD_FORMAT   format;                          // 
    MEX_RECORD_TYPE     type;                            // 
} Mex_Record_Req_Struct;

// ¼���ӿ�
// ��ʼ¼��
// voice_record_start
// ֹͣ¼��
// voice_record_stop
// ��ͣ��ָ�¼��
// voice_record_suspend
// ��ȡ��¼���ݻ�����
// voice_record_get_read_buffer
// ��ȡ��¼�������
// voice_record_read_data_done
// ���MEX_RECORD_STATE
// get_voiceRecordState

/* MEX PCM STATE */
typedef enum
{
    E_MPS_PCM_PLAYING = 0,       /* ���ڲ��� */
    E_MPS_PCM_STOPED  = 1,       /* ��ֹͣ */
    E_MPS_PCM_PAUSED  = 2        /* ����ͣ */
} MEX_PCM_STATE;

/* MEX PCM PLAY REQ STRUCT */
typedef struct
{
    /* PCM Stream Param */
    bool       isStereo;        /* ��������־ */
    int8       bitPerSample;    /* ÿһ���������������ı����� */
    uint16     sampleFreq;      /* ����Ƶ�� */

    /* Read pcm data */
    bool (*read_pcm_data) (uint8 *data_p, uint32 *len_p);
} Mex_Pcm_Play_Req_Struct;

// PCM ���Žӿ�
// PCM ��ʼ����
// pcm_play
// PCM ֹͣ����
// pcm_stop
// PCM ��ͣ��ָ�
// pcm_suspend
// ���MEX_PCM_STATE
// get_pcmState

// ���MEX AUDIO MODE
// get_audioMode
// ����MEX AUDIO MODE
// set_audioMode

// MEX SOUND INPUT SOURCE
typedef enum
{
    E_MSIS_MICROPHONE1,         // �ֻ���Microphone
    E_MSIS_MICROPHONE2,         // ������Microphone
    E_MSIS_DEFAULT,             // Ĭ�ϵ�Microphone
    
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

// ��ȡ��Ƶ���ݵĻص�����
// ����1: pAudioStream ��Ƶ��ָ��
// ����2: pDataBuf ��Ƶ���ݻ�����
// ����3: iBufLen ��Ƶ���ݻ�������С�����ֽڼ���
// ����: �������Ƶ���ݵĴ�С��������ʾ��Ƶ������
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
    uint8      isStartMix;          // ��ʼ������־
    uint8      aud_volume;         //����ʱ��Ƶ������
    uint8      mic_volume;         //����ʱ��Ͳ������
    uint8      is_aud_muse;        //��Ƶ�Ƿ���
    uint8      is_mic_muse;        //��Ͳ�Ƿ���
    uint8      is_right_channel_on;   //����ѡ��������Ϊ����������������Ϊԭ��
    uint8      is_discard_mic_data;  //ָʾ�Ƿ���MIC�������е�����
    uint8      is_discard_track_data;  //ָʾ�Ƿ���DA�������е�����    
} Mex_Mix_Control_Para_Struct;

typedef enum
{
    MEX_MIX_NOTIFY_TYPE_HEADSET_STATE,
    MEX_MIX_NOTIFY_TYPE_NUM,    
}MEX_MIX_NOTIFY_TYPE_E;

typedef struct
{    
    int        iNotifyType;         //֪ͨ����Ϣ����

    union
    {
        uint8  isPlugIn;          // ��ʼ������־
    }para;
    
} Mex_Mix_Notify_Para_Struct;

// ��ȡ��Ƶ���ݵĻص�����
// ����1: pAudioStream ��Ƶ��ָ��
// ����2: pDataBuf ��Ƶ���ݻ�����
// ����3: iBufLen ��Ƶ���ݻ�������С�����ֽڼ���
// ����: �������Ƶ���ݵĴ�С��������ʾ��Ƶ������
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
    /*��Ͳ���ƽӿ�*/
    Mex_Mic_Open_fp              mic_open;               // ��ʼ������־
    Mex_Mic_Close_fp             mic_close;              //�ر�mic
    Mex_Mic_Set_SampleRate_fp    mic_set_samplerate;     //���ò�����
    Mex_Mic_Set_InputDevice_fp   mic_set_input_device;   //���������豸
    Mex_Mic_Clean_Queue_fp       mic_clean_queue;        //����������ݻ�����

    /*��������*/    
    Mex_Mix_Open_fp              mix_open;    //�ر�mic
    Mex_Mix_Close_fp             mix_close;     // ��ʼ������־
    Mex_Mix_Get_Para_fp          mix_get_para;    //�ر�mic
    Mex_Mix_Set_Para_fp          mix_set_para;     // ��ʼ������־    
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

//����WAV,MIDI,mp3���ָ�ʽ��Ƶ��������Ƶ�����
//����1����Ƶ�ļ�·������Դ����Դ·����
//����2����Ƶ��ʽ���μ�enum MEX_SOUND_TYPE��
//���� ����Ƶ���
MEX_MDI_HANDLE sound_create (wchar_t* filePath, MEX_SOUND_TYPE type);

//˵��������һ����Ƶ����ȫ��Ӧ�ü�飬��ǰ��æ���ܲ����Լ�������
//����1����Ƶ�������sound_create ������
//����2���Ƿ�һ�β��ţ������ѭ�����š�
//���� �������Ƿ�ɹ�
bool sound_play (MEX_MDI_HANDLE sound , bool once);

//˵����ֹͣһ����Ƶ��
//����1����Ƶ�������sound_create ������
//���� ��ֹͣ�Ƿ�ɹ�
bool sound_stop (MEX_MDI_HANDLE sound);

//˵������ͣ�����һ����Ƶ��
//����1����Ƶ�������sound_create ������
//����2���Ƿ���ͣ���ţ�����ͼ������š�
//���� ����ͣ������Ƿ�ɹ�
bool sound_suspend (MEX_MDI_HANDLE sound , bool pause);

//˵�����ر�һ����Ƶ��
//����1����Ƶ�������sound_create ������
//���� ����
void sound_close (MEX_MDI_HANDLE sound);

//˵����ȡ�õ�ǰ����״̬��
//�������ޡ�
//���� ������״̬���μ�enum MEX_SOUND_STATE��
MEX_SOUND_STATE get_soundState (void);

//˵����ȡ�õ�ǰ����������
//�������ޡ�
//���� �������������μ�enum MEX_SOUND_VOLUME��
MEX_SOUND_VOLUME get_soundVolume (void);

//˵�������õ�ǰ����������
//�����������������μ�enum MEX_SOUND_VOLUME��
//���� ���ޡ�
void set_soundVolume (MEX_SOUND_VOLUME volume);

//˵������õ�ǰ�������·��
//��������
//���� ����ǰ�������·��
MEX_SOUND_OUTPUT_PATH get_soundOutputPath(void);

//˵�������õ�ǰ�������·��
//�������µ��������·��
//���� ����
void set_soundOutputPath(MEX_SOUND_OUTPUT_PATH path);

/* mex record */
//˵������ʼ¼��
//����1��¼������
//���أ��Ƿ�ɹ�
bool voice_record_start(Mex_Record_Req_Struct* req);

//˵��������¼��
//��������
//���أ���
void voice_record_stop(void);

//˵������ͣ��ָ�¼��
//����1��������ͣ¼��������ָ�¼��
//���أ��Ƿ�ɹ�
bool voice_record_suspend(bool pause);

//˵������ȡ��¼���ݻ�����
//����1��[OUT] ָ����¼���ݻ�����
//����2��[OUT] ��¼�����ݵĳ���,  ���ֽڼ���
//���أ���
void voice_record_get_read_buffer(uint8 **dataBuf, uint32 *dataLen);

//˵������ȡ��¼�������
//����1���Ѵ����¼�����ݵĳ���
//���أ���
void voice_record_read_data_done(uint32 dataLen);

//˵�������MEX_RECORD_STATE
//����1����
//���أ���ǰ¼��״̬
MEX_RECORD_STATE get_voiceRecordState(void);

//˵��������PCM ��
//����1��Pcm Play request
//���أ��Ƿ�ɹ�
bool pcm_play (Mex_Pcm_Play_Req_Struct* req);

//˵����ֹͣ����PCM ��
//����1����
//���أ���
void pcm_stop(void);

//˵������ͣ��ָ�����PCM ��
//����1��������ͣ���ţ�����ָ�����
//���أ��Ƿ�ɹ�
bool pcm_suspend(bool pause);

//˵�������MEX_PCM_STATE
//����1����
//���أ�PCM ����״̬
MEX_PCM_STATE get_pcmState(void);

//˵�������MEX AUDIO MODE
//����1����
//���أ���ǰAUDIO MODE
MEX_AUDIO_MODE get_audioMode(void);

//˵��������MEX AUDIO MODE
//����1���µ�AUDIO MODE
//���أ��Ƿ�ɹ�
bool set_audioMode(MEX_AUDIO_MODE audioMode);

//����һ����Ƶ�ṹ 
//���� filePath ��Ƶ�ļ���·��
//fpAudioNofity ���Ź����еĻص����� ����audio_cb  ����Ϊint32  ��Ϊ-1ʱ˵�����Ŵ��� ��Ϊ0ʱ ������� ��Ϊ1ʱ˵����Ҫ�ؽ������ļ�
//������id
Mex_Audio_Struct *audio_init(wchar_t* filePath,audio_cb fpAudioNofity, int32 app_id );

//����һ����Ƶ�ṹ
//���� audio_struct ��Ҫ���ٵĽṹ
//������id
void audio_deInit(Mex_Audio_Struct * audio_struct,int32 app_id );

//��ȡ��ǰ��Ƶ������ʱ�� ��λms
//���� ��ǰ���ŵ���Ƶ�ṹ
uint32 audio_getduration(Mex_Audio_Struct * audio_struct);

//��ȡ��ǰ�����Ѳ��ŵ�ʱ�� ��λms
//���� ��ǰ���ŵ���Ƶ�ṹ
uint32 audio_get_nowplayedtime(Mex_Audio_Struct *audio_struct);

//������ת��ĳ��ʱ��ȥ���� 
//����audio_struct��ǰ���ŵ���Ƶ�ṹ
//����target_time Ŀǰʱ��� ��λms
//ע�⣺ֻ���ڲ���״̬�²���Ч
//�˺�����������������У�����ڵ��õ�ʱ�������һ����ʱ����
int8 audio_seek_to_target(Mex_Audio_Struct * audio_struct,uint32 target_time);

//��ȡ��ǰ����
uint32 audio_get_volume(void);

//���õ�ǰ���� �ο�MEX_SOUND_VOLUME
void audio_set_volume(uint32 volume);

//��������
//����0 ˵��ʧ��
//����1 �ɹ�
int8 audio_play(Mex_Audio_Struct * audio_struct);

//ֹͣ����
//����0 ˵��ʧ��
//����1 �ɹ�
int8 audio_stop(Mex_Audio_Struct * audio_struct);

//��ͣ����
//����0 ˵��ʧ��
//����1 �ɹ�
int8 audio_pause(Mex_Audio_Struct * audio_struct);

//�ָ�����  ��ͣ����ÿɻָ����ֲ���
//����0 ˵��ʧ��
//����1 �ɹ�
int8 audio_resume(Mex_Audio_Struct * audio_struct);

//�ؽ������ļ��� ��������
//����0 ˵��ʧ��
//����1 �ɹ�
int8 audio_restore(Mex_Audio_Struct * audio_struct);

//����Ƶ�׷�������
//����0 ˵��ʧ��
//����1 �ɹ�
//ע�⣺���øú���ǰӦʹ��GetPhoneConfig(E_MPC_AudPly_Spectrum_Display) ���ƽ̨�Ƿ�֧��Ƶ�׷�������
int8 audio_start_calc_spectrum(void);

//����Ƶ�׷�������
//����0 ˵��ʧ��
//����1 �ɹ�
int8 audio_stop_calc_spectrum(void);

//��ȡ��ǰƵ�׷��������ֵ
//top_p ��СΪ16
//val_p ��СΪ16
int8 audio_get_spectrum(uint8 *top_p,uint8 *val_p);

// ��ȡ¼������Դ
// ����1: ��
// ����MEX_SOUND_INPUT_SOURCE
MEX_SOUND_INPUT_SOURCE get_soundInputSource(void);

// ����¼������Դ
// ����1: eSource ¼������Դ
// ����: ��
void set_soundInputSource(MEX_SOUND_INPUT_SOURCE eSource);

// ��ȡ��˵�����
// ����1: ��
// ����: ��˵�������С
uint8 get_microphoneVolume(void);

// ������˵�����
// ����1: volume �������ֵ
// ����: ��
void set_microphoneVolume(uint8 volume);

//���ϵͳ�Ƿ����ڲ�������
//��ȫ��Ӧ�ü�飬��ǰ��æ���ܲ����Լ�������
bool sound_is_busy(void);

//��ȡ¼�������������ݳ���
//����1: ��
//����: ¼�������������ݳ���(���ֽڼ���)
uint32 voice_record_get_data_count(void);

// ��ʼ����Ƶ��
// ����1: pAudioData ָ����Ƶ������
// ����2: nDataLen ��Ƶ���ݵĳ���
// ����3: eFormat ��Ƶ��ʽ
// ����4: ePlayType ����ģʽ
// ����5: pPlayNotify ���Ź����еĻص�  ������� ���Ŵ��� �ȵ�
// ����: �ɹ�������Ƶ��ָ�룬ʧ���򷵻�NULL
Mex_Audio_Stream audio_stream_init(void *pAudioData, uint16 nDataLen, E_MEX_Audio_Stream_Format eFormat, MEX_SOUND_PLAY_TYPE ePlayType, Mex_Audio_Stream_CB pPlayNotify);

// �ͷ���Ƶ��
// ����1: pAudioStream ��Ƶ��ָ��
// ����: ��
void audio_stream_deInit(Mex_Audio_Stream pAudioStream);

// ������Ƶ��
// ����1: pAudioStream ��Ƶ��ָ��
// ����: �Ƿ�ɹ�
bool audio_stream_play(Mex_Audio_Stream pAudioStream);

// ֹͣ������Ƶ��
// ����1: pAudioStream ��Ƶ��ָ��
// ����: �Ƿ�ɹ�
bool audio_stream_stop(Mex_Audio_Stream pAudioStream);

// ���ô�Ӧ�û�ȡ��Ƶ���ݵĻص�����
// ����1: pAudioStream ��Ƶ��ָ��
// ����2: pProvider ��Ӧ�û�ȡ��Ƶ���ݵĻص�����
// ����: �Ƿ����óɹ�
bool set_audio_stream_provider (Mex_Audio_Stream pAudioStream, Mex_Audio_Stream_Provider pProvider);

//ֹͣ��ǰ���ŵ�����
//��������
//����;��
void audio_stop_background_play (void)

#endif

#endif

