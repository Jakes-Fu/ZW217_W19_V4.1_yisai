#if 0
/*****************************************************************************
** File Name:      mmiaudio.h                                                  *
** Author:                                                                   *
** Date:           15/01/2005                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to declare audio function and define global               *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 01/2006       wancan.you       Create
******************************************************************************/

#ifndef _MMIAUDIO_H_
#define _MMIAUDIO_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "mmk_type.h"
#include "tb_dal.h"
//#include "ffs.h"

//#ifndef _WIN32
#include "mmifmm_export.h"   //audio_api.h中用到了ffs.h中的结构
#include "audio_config.h"
#include "audio_api.h"
#include "adpcmRecord_adp.h"
//#else
//#include "mmiaudio_simu.h"
//#endif

#include "eq_exp.h"

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define ADPCM_FRAME_LENGTH  160
#define DSP_DATA_FRAME_LENGTH  350//328
/*[START]Jerry Liang for IQ MODE 2008/07/31*/
#define DSP_DATA_FRAME_LENGTH_VOICE_DATA  480
/*[END]Jerry Liang for IQ MODE 2008/07/31*/
#define DSP_DATA_BLOCK_COUNT_2  2
#define DSP_DATA_BLOCK_COUNT_3  3
#define MMIAUDIO_TONE_DEFAULT_FREQ	2000

#define MMI_MIDI_LOW_POLY_NUM      8
#define MMI_MIDI_NORMAL_POLY_NUM   32
#ifdef MONOCHROME_LCD_SUPPORT 
#define MMI_AUDIO_VOLUME_MAX   7
#else
#define MMI_AUDIO_VOLUME_MAX   9
#endif 
/**--------------------------------------------------------------------------*
 **                         TYPE DEFINITION                                 *
 **--------------------------------------------------------------------------*/
//the type of audio service handle
typedef enum
{
    MMIAUDIO_NONE,  //None
    MMIAUDIO_APP,   //application
    MMIAUDIO_PLAYER,//player
    MMIAUDIO_MUSIC, //music play
    MMIAUDIO_KURO,  //kuro
#ifdef JAVA_SUPPORT
    MMIAUDIO_JAVA,
#endif
    MMIAUDIO_NORMAL,
#ifdef WRE_SUPPORT
    MMIAUDIO_WRE,
#endif    
    MMIAUDIO_MAX
}MMIAUDIO_HTYPE;

typedef enum
{
    MMIAUDIO_DEVICE_BT,
    MMIAUDIO_DEVICE_OTHER,
    MMIAUDIO_DEVICE_MAX
}MMIAUDIO_DEVICE_TYPE_E;

typedef enum
{
    MMIAUDIO_RECORD_NORMAL_APP,
    MMIAUDIO_RECORD_CC,
    MMIAUDIO_RECORD_FM,
    MMIAUDIO_RECORD_MAX
}MMIAUDIO_RECORD_TYPE_E;

// Audio ring data type.
typedef enum
{
    MMIAUDIO_MIDI,          // File format: 
                            //  General Midi file format
                                                        
    MMIAUDIO_GENERIC_TONE,   // File format(in BigEndian): 
                                            
    MMIAUDIO_CUSTOM_TONE,     // File format(in BigEndian):
                            
    MMIAUDIO_DTMF_TONE,   // File format(in BigEndian):

    MMIAUDIO_SMAF,          // File format:
                            //  Defined by Yamaha.
                            
    MMIAUDIO_WAVE,          // File format:
                            //  wave sound 16bit samples data ,sound sampling rate is 16k                            
                            
    MMIAUDIO_MP3,           // File format:
                            //  mp3
    MMIAUDIO_WMA,
    
    MMIAUDIO_AMR,           // File format

    MMIAUDIO_AAC,
    
    MMIAUDIO_M4A,

    MMIAUDIO_TYPE_MAX       // Reserved, user can't use it.
} MMIAUDIO_TYPE_E;

//the result of handle audio service(mmi handle)
typedef enum
{
    MMIAUDIO_RLT_SUCCESS = SCI_SUCCESS,//success
    MMIAUDIO_RLT_STOP,
    MMIAUDIO_RLT_NOT_SUPPORT,//not support
    MMIAUDIO_RLT_ERROR,//error
    MMIAUDIO_RLT_OTHER,//other reason
    MMIAUDIO_RLT_MAX
}MMIAUDIO_RESULT;

//record frame length
typedef enum
{
    MMIAUDIO_RECORD_DEFAULT_FRAME_LEN = 0,
    MMIAUDIO_RECORD_FRAME_LEN_32      = 0,
    MMIAUDIO_RECORD_FRAME_LEN_27,
    MMIAUDIO_RECORD_FRAME_LEN_21,
    MMIAUDIO_RECORD_FRAME_LEN_20,
    MMIAUDIO_RECORD_FRAME_LEN_18,
    MMIAUDIO_RECORD_FRAME_LEN_16,
    MMIAUDIO_RECORD_FRAME_LEN_14,
    MMIAUDIO_RECORD_FRAME_LEN_13,
    MMIAUDIO_RECORD_FRAME_LEN_MAX
}MMIAUDIO_RECORD_FRAME_LENGTH_T;

//record frame length
typedef enum
{
    MMIAUDIO_RECORD_FORMAT_ADPCM,
    MMIAUDIO_RECORD_FORMAT_AMR,
    MMIAUDIO_RECORD_FORMAT_MP3,//andy.he_record 
    MMIAUDIO_RECORD_FORMAT_MAX
}MMIAUDIO_RECORD_FORMAT_T;

//play rate
typedef enum
{
    MMIAUDIO_PLAY_RATE_SLOW_2,
    MMIAUDIO_PLAY_RATE_SLOW_1,
    MMIAUDIO_PLAY_RATE_NORMAL,
    MMIAUDIO_PLAY_RATE_FAST_1,
    MMIAUDIO_PLAY_RATE_FAST_2,
    MMIAUDIO_PLAY_RATE_MAX
}MMIAUDIO_PLAY_RATE_E;

typedef enum
{
    MMIAUDIO_EQ_REGULAR,      //常规
    MMIAUDIO_EQ_CLASSIC,      //古典
    MMIAUDIO_EQ_ODEUM,        //音乐厅
    MMIAUDIO_EQ_JAZZ,         //爵士
    MMIAUDIO_EQ_ROCK,         //摇滚
    MMIAUDIO_EQ_SOFT_ROCK,    //软摇滚
    MMIAUDIO_EQ_MAX_MODE
} MMIAUDIO_EQ_MODE_E;

//the callback info
typedef struct
{
    HAUDIO hAudio;
    uint32 notify_info;
    uint32 affix_info;
}MMIAUDIO_CALLBACK_INFO_T;

typedef struct
{
    uint32 time_len;    //sec
    uint32 data_len;    //byte
}MMIAUDIO_INFO_T;

//mmi audio callback function
typedef void (* MMIAUDIO_END_CALLBACK_PFUNC)(MMIAUDIO_RESULT result, DPARAM param);  //播放铃声自动结束后的回调函数
//#ifdef JAVA_SUPPORT
//typedef void (* MMIAUDIO_JAVA_END_CALLBACK_PFUNC)(HAUDIO handle);  //播放铃声自动结束后的回调函数
//#endif
extern HAUDIO hVoiceRing;
extern HAUDIO hVoiceLoopbackRing;
/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : audio init
//  Global resource dependence : none
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAUDIO_Init(void);

/*****************************************************************************/
//  Description : MMIAUDIO SwitchDevice
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUDIO_SwitchDeviceExt(
                    MMIAUDIO_HTYPE          audio_handle_type,
                    MMIAUDIO_DEVICE_TYPE_E  device
                    );
                    
/*****************************************************************************/
//  Description : API of create audio buffer play handle
//  Global resource dependence : none
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUDIO_CreateAudioBufHandle(
                     BOOLEAN is_a2dp,
                     MMIAUDIO_HTYPE audio_handle_type,   //the type of audio handle
                     MMIAUDIO_TYPE_E ring_type,    // the codec handle which will be used.
                     const uint8* pucData,   // the music data pointer
                     uint32 uiDataLength    // the music data length
                     );
/*****************************************************************************/
//  Description : API of create audio file handle
//  Global resource dependence : none
//  Author: 
//  Note: SUCCESS - return the audio handle created
//        FAILURE - return SCI_NULL
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUDIO_CreateAudioFileHandle( 
                        BOOLEAN is_a2dp,
                        MMIAUDIO_HTYPE audio_handle_type,
                        MMIAUDIO_TYPE_E ring_type,          // the codec handle which will be used.
                        const wchar *name_ptr               // file name ptr
                        );

/*****************************************************************************/
//  Description : API of create audio streaming buffer play handle
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUDIO_CreateAudioStreamingHandle(
                     BOOLEAN            is_a2dp,
                     MMIAUDIO_HTYPE     audio_handle_type,  //the type of audio handle
                     MMIAUDIO_TYPE_E    ring_type,          // the codec handle which will be used.
                     const uint8        *pucData,           // the music data pointer
                     uint32             uiDataLength,       // the music data length
                     AUDIO_GETSRCDATA_CALLBACK_PFUNC AudioGetSrcDataCallback
                     );

/*****************************************************************************/
//  Description : close audio handle 
//  Global resource dependence : none
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUDIO_CloseAudioHandle(MMIAUDIO_HTYPE audio_handle_type);


/*****************************************************************************/
//  Description : audio play 
//  Global resource dependence : none
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUDIO_AudioPlay(
                            MMIAUDIO_HTYPE audio_handle_type,
                            uint32 uiOffset,
                            uint32  play_times,
                            MMIAUDIO_END_CALLBACK_PFUNC end_callback_func
                            );


/*****************************************************************************/
//  Description : audio play 
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUDIO_AudioPlayAsy(
                            MMIAUDIO_HTYPE   audio_handle_type,
                            uint32           uiOffset,
                            uint32           play_times,
                            MMIAUDIO_END_CALLBACK_PFUNC end_callback_func
                            );
                            
/*****************************************************************************/
//  Description : stop audio play 
//  Global resource dependence : none
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAUDIO_AudioStop(MMIAUDIO_HTYPE audio_handle_type);

/*****************************************************************************/
//  Description : API of audio play pause
//  Global resource dependence : none
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUDIO_AudioPause(MMIAUDIO_HTYPE audio_handle_type);

/*****************************************************************************/
//  Description : API of audio play resume
//  Global resource dependence : none
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUDIO_AudioResume(MMIAUDIO_HTYPE audio_handle_type);

/*****************************************************************************/
//  Description : API of audio play seek
//  Global resource dependence : none
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUDIO_AudioSeek(
                                  MMIAUDIO_HTYPE    audio_handle_type,
                                  uint32            uiOffset
                                  );

/*****************************************************************************/
//  Description : API of audio play set speed
//  Global resource dependence : none
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAUDIO_AudioSetSpeed(MMIAUDIO_HTYPE audio_handle_type, uint32 uiSpeedX);

/*****************************************************************************/
//  Description : API of audio play get playing file info
//  Global resource dependence : none
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUDIO_AudioGetPlayingFileInfo(MMIAUDIO_HTYPE audio_handle_type, AUDIO_PLAYINFO_T* ptPlayInfo);

/*****************************************************************************/
//  Description : API of audio get file info(cann't callback during play, only use after create handle)
//  Global resource dependence : none
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUDIO_AudioGetFileInfo(MMIAUDIO_HTYPE audio_handle_type, AUDIO_FORMATINFO_T* ptfileInfo);

/*****************************************************************************/
//  Description : API of start record
//  Global resource dependence : none
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUDIO_StartRecord(
                    const wchar     *name_ptr,
                    MMIFILE_HANDLE  file_handle,    //file handle created, can be null
                    MMIAUDIO_RECORD_FORMAT_T format,
                    MMIAUDIO_RECORD_FRAME_LENGTH_T frame_length,
                    MMIAUDIO_RECORD_TYPE_E      record_type,
                    MMIAUDIO_END_CALLBACK_PFUNC end_callback_func
                    );

/*****************************************************************************/
//  Description : API of start record
//  Global resource dependence : none
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUDIO_StopRecord(void);

/*****************************************************************************/
//  Description : API of end record (close handle)
//  Global resource dependence : none
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUDIO_EndRecord(void);

/*****************************************************************************/
//  Description : API of pause record
//  Global resource dependence : none
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUDIO_PauseRecord(void);

/*****************************************************************************/
//  Description : API of resume record
//  Global resource dependence : none
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUDIO_ResumeRecord(void);

/*****************************************************************************/
//  Description : API of is is audio playing
//  Global resource dependence : none
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUDIO_GetIsPlaying(void);

/*****************************************************************************/
//  Description : API of is permit audio playing
//  Global resource dependence : none
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUDIO_GetIsPermitPlay(MMIAUDIO_HTYPE audio_htype);

/*****************************************************************************/
//  Description : API of is get current play audio handle type
//  Global resource dependence : none
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC MMIAUDIO_HTYPE MMIAUDIO_GetAudioHtype(void);

/*****************************************************************************/
//  Description : API of is get audio handle
//  Global resource dependence : none
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC HAUDIO MMIAUDIO_GetAudioHandle(MMIAUDIO_HTYPE audio_handle_type);

/*****************************************************************************/
//  Description : play tone 
//  Global resource dependence : none
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAUDIO_PlayTone(
                            MMIAUDIO_TYPE_E tone_type,
                            uint32 tone_id,
			                uint32 duration,
            			    uint32 freq
                            );

/*****************************************************************************/
//  Description : tone stop 
//  Global resource dependence : none
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAUDIO_StopTone(
                             MMIAUDIO_TYPE_E tone_type
                             );

/*****************************************************************************/
//  Description : set eq mode
//  Global resource dependence : hEQExp
//  Author: haiyang.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMIAUDIO_SetEQMode(MMIAUDIO_EQ_MODE_E eq_mode); //0—低音,1—高音,2—声乐,3—摇滚,4—流行,5—古典,6—强劲,7—常规

/*****************************************************************************/
//  Description : Set play rate.
//  Global resource dependence : 
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIAUDIO_SetPlayRate(
                MMIAUDIO_PLAY_RATE_E rate
                );
                
/*****************************************************************************/
//  Description : set volume 
//  Global resource dependence : none
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAUDIO_SetVolume(AUDIO_DEVICE_MODE_TYPE_E  mode, uint32 speaker_vol);

/*****************************************************************************/
//  Description : get current volume 
//  Global resource dependence : none
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMIAUDIO_GetCurVolume(void);

/*****************************************************************************/
//  Description : get current device mode 
//  Global resource dependence : none
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC AUDIO_DEVICE_MODE_TYPE_E MMIAUDIO_GetCurDevMode(void);

/*****************************************************************************/
//  Description : set mute 
//  Global resource dependence : none
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUDIO_SetMute(BOOLEAN is_mute);
/*****************************************************************************/
//  Description : Get mute 
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUDIO_GetMute(void);

/*****************************************************************************/
//  Description : Seek by time offset(ms).
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUDIO_AudioSeekByTime(
                        MMIAUDIO_HTYPE audio_handle_type, 
                        uint32 time_offset
                        );

/*****************************************************************************/
//  Description : To judge whether the music is supported for seeking.
//  Global resource dependence : 
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUDIO_IsSeekSupport(
                    MMIAUDIO_TYPE_E audio_type
                    );

/*****************************************************************************/
//  Description : set up link mute 
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUDIO_SetUpLinkMute(BOOLEAN is_mute);

/*****************************************************************************/
//  Description : set current device mode 
//  Global resource dependence : none
//  Author: ryan.xu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAUDIO_SetCurDevMode(AUDIO_DEVICE_MODE_TYPE_E mode);

/*****************************************************************************/
//  Description : API of start IQ Data Record
//  Global resource dependence : none
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUDIO_StartIQData(
                    const wchar *name_ptr, 
                    RECORD_PARAM_T *record_param
                    );

/*****************************************************************************/
//  Description : Get the record eclipse time (count in ms).
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMIAUDIO_GetRecordEclipseTime(
                    void
                    );

/*****************************************************************************/
//  Description : Set half samplerate.
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAUDIO_SetHalfSamplerate(
                BOOLEAN is_set_half
                );

/*****************************************************************************/
//  Description : Handle audio callback message.
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIAUDIO_HandleCallBackMsg(
                    MMIAUDIO_CALLBACK_INFO_T *callback_info_ptr
                    );

/*****************************************************************************/
//  Description : enable/disable voice codec.
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUDIO_EnableVoiceCodec(
                    BOOLEAN is_enable
                    );

/*****************************************************************************/
//  Description : Get whether eq mode is normal.
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUDIO_IsNormalEQMode(void);

/*****************************************************************************/
//  Description : audio handle is valid
//  Global resource dependence : none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUDIO_SM_GetAudioObject(
    HAUDIO hAudioHandle
    );
    
/*****************************************************************************/
//  Description : Reset the device mode and volume, regardless of the original device mode and volume.
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAUDIO_SetVolumeDeviceModeExt(AUDIO_DEVICE_MODE_TYPE_E  mode, uint32 speaker_vol);

/*****************************************************************************/
//  Description : enable/disable voice loop.
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAUDIO_EnableVoiceLoop(
                    BOOLEAN is_loop_enable,
                    BOOLEAN is_force_dev_mode,
                    AUDIO_DEVICE_MODE_TYPE_E force_dev_mode,
                    uint8   volume
                    );

/*****************************************************************************/
//  Description : is voice loop on.
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUDIO_IsInVoiceLoop(
                    void
                    );

/*****************************************************************************/
//  Description : Is Voice Loop Force Mode
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUDIO_IsVoiceLoopForceMode(
                    void
                    );
                    
/*****************************************************************************/
//  Description : Set midi param to low chord.
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAUDIO_SetMidiLowPolyNum(
                    MMIAUDIO_HTYPE audio_htype,
                    BOOLEAN is_low_poly_num
                    );

/*****************************************************************************/
//  Description : MMI AUDIO Is AGC Support
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUDIO_IsAGCSupport(
                        void
                        );

/*****************************************************************************/
//  Description : MMI AUDIO Is AGC Support
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAUDIO_StartAGC(
                        void
                        );

/*****************************************************************************/
//  Description : MMI AUDIO Is AGC Support
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAUDIO_StopAGC(
                        void
                        );

/*****************************************************************************/
//  Description : MMIAUDIO Enable EQ
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAUDIO_EnableEQ(
                        BOOLEAN is_enable
                        );

/*****************************************************************************/
//  Description : MMIAUDIO Enable AGC
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAUDIO_EnableAGC(
                        BOOLEAN is_enable
                        );

/*****************************************************************************/
//  Description : MMIAUDIO Enable EQ & AGC
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAUDIO_EnableEQAndAGC(
                        BOOLEAN is_enable
                        );


/*****************************************************************************/
//  Description : MMI AUDIO Open Spectrum
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUDIO_OpenSpectrum(MMIAUDIO_HTYPE audio_handle_type);

/*****************************************************************************/
//  Description : MMI AUDIO Get Spectrum Info
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUDIO_GetSpectrum(
                    MMIAUDIO_HTYPE  audio_handle_type,
                    uint16          * spec_buf_ptr,         //[OUT]
                    uint16          * spec_num_ptr,         //[OUT] 目前长度无法验证，约定为32，即buf长度需要大于 32*sizeof(uint16),否则可能覆盖；需要ref提供结构体形式
                    uint16          * max_spec_value_ptr    //[OUT]
                    );

/*****************************************************************************/
//  Description : MMI AUDIO Close Spectrum
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUDIO_CloseSpectrum(MMIAUDIO_HTYPE audio_handle_type);

/*****************************************************************************/
//  Description : set device mode for call
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUDIO_SerDeviceModeInCall(
                    AUDIO_DEVICE_MODE_TYPE_E mode
                    );

/*****************************************************************************/
//  Description : MMIAUDIO Open FM
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAUDIO_OpenFM(
                    void
                    );

/*****************************************************************************/
//  Description : MMIAUDIO Play FM
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUDIO_PlayFM(
                    void
                    );

/*****************************************************************************/
//  Description : MMIAUDIO Stop FM
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAUDIO_StopFM(
                    void
                    );

/*****************************************************************************/
//  Description : MMIAUDIO Close FM
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAUDIO_CloseFM(
                    void
                    );

/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC uint32 *MMIAUDIO_GetApplicationTrackBuf(void);
/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMIAUDIO_GetApplicationTrackBufSize(void);

/*****************************************************************************/
//  Description : MMIAUDIO_IsAudioPlayEnd
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUDIO_IsAudioPlayEnd(HAUDIO audio_handle);

/*****************************************************************************/
//  Description : Get the record voidce DB.
//  Global resource dependence : none
//  Author: chunyou.zhu
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMIAUDIO_GetRecordVoiceDB(
                void
                );

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
#endif
