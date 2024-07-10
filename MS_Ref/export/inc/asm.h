/******************************************************************************
 ** File Name:      asm.h                                                *
 ** Author:         Benjamin.Wang                                             *
 ** DATE:           11/11/2005                                                *
 ** Copyright:      2005 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the basic operation interfaces of Audio Service Manager.*
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 11/11/2005     Benjamin.Wang     Create.                                  *
 ******************************************************************************/

 /*! \file asm.h
  *  \author Benjamin Wang
  *  \date Nov 11, 2005
  *  \brief This file defines the basic operation interfaces of Audio Service Manager.
  */

#ifndef _ASM_H
#define _ASM_H

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "os_api.h"
#include "audio_api.h"

#ifdef __cplusplus
    extern   "C"
    {
#endif

/**---------------------------------------------------------------------------*
 **                         MACRO Definations                                     *
 **---------------------------------------------------------------------------*/

//#define ASM_ASSERT_MODE
#ifdef ASM_ASSERT_MODE
	#define ASM_ASSERT_WARNING SCI_ASSERT /*assert verified*/
	#define ASM_PASSERT_WARNING SCI_PASSERT /*assert verified*/
#else
	#define ASM_ASSERT_WARNING(__cond)  /*assert verified*/
	#define ASM_PASSERT_WARNING(__cond, __msg) /*assert verified*/
#endif

#ifdef EXPRESS_PROCESS_IN_DSP
#define	CODEC_DEMUX_PRIORITY_OFFSET		(1)
#else
#define	CODEC_DEMUX_PRIORITY_OFFSET		(0)
#endif

#define AUDIO_DUMP_PCM_DATA_FILE_COUNT_MAX 8

 /*! \def AUDIO_FFS_PATH_MAX_LEN
 *  max length of string of ffs path, which include dir and file_name.
 */
 #define AUDIO_FFS_PATH_MAX_LEN  0xFFFF     

 #define AUDIO_ALLOC(_SIZE)  (uint8 *)SCI_ALLOCA(_SIZE)

 #define AUDIO_SFS_DEVICE_LEN (SFS_MAX_PATH+1)

#define AUDIO_SRCDATA_INFO_OFFSET_TIME_IN_MS(tpOffsetTimeInfo) \
    (\
    (AUDIO_SRCDATA_INFO_OFFSET_INVALID==((AUDIO_SRCDATA_INFO_OFFSET_TIME_T *)tpOffsetTimeInfo)->eOffsetType)?0:\
    (\
      (AUDIO_SRCDATA_INFO_OFFSET_TIME==((AUDIO_SRCDATA_INFO_OFFSET_TIME_T *)tpOffsetTimeInfo)->eOffsetType)?\
      ((AUDIO_SRCDATA_INFO_OFFSET_TIME_T *)tpOffsetTimeInfo)->uOffsetTime.uiCurTime:\
      (\
      ((uint64)(((AUDIO_SRCDATA_INFO_OFFSET_TIME_T *)tpOffsetTimeInfo)->uOffsetTime.tPts.uiPts))*1000/\
      ((AUDIO_SRCDATA_INFO_OFFSET_TIME_T *)tpOffsetTimeInfo)->uOffsetTime.tPts.uiTimeScale\
      )\
      )\
    )
/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*! Indication and Response:
 * When some event occured, audio service will send indication initiatively to upper layer. After received
 * the indication, upper layer perhaps will send a response to audio service.*/
/*----------------------------------------------------------------------------*/
/*! \struct AUDIO_ERROR_IND_MSG_T
 *  \brief Error indication.
 *  When some error occured, audio service will send the error indication to upper layer to inform the 
 *  error type.
 */
typedef struct
{
    SIGNAL_VARS     /*!<Signal Header.*/
    HAUDIO hAudioHandle;    /*!<Audio handle.*/
    uint32 uiError;     /*!<Error type.*/
} AUDIO_ERROR_IND_MSG_T;

/*! \struct AUDIO_ERROR_RSP_MSG_T
 *  \brief Error indication response.
 *  After received the indication, it is recommend that upper layer response to audio servcie.
 */
typedef struct
{
    SIGNAL_VARS     /*!<Signal Header.*/
    HAUDIO hAudioHandle;    /*!<Audio handle.*/
} AUDIO_ERROR_RSP_MSG_T;

/*! \struct AUDIO_PLAYEND_IND_MSG_T
 *  \brief Play end indication.
 *  When an audio plays end, audio service will send an indication to upper laye to inform the audioplay 
 *  finished.
 */
typedef struct
{
    SIGNAL_VARS     /*!<Signal Header.*/
    HAUDIO hAudioHandle;    /*!<Audio handle.*/
    AUDIO_RESULT_E eResult; /*!< Stop result.*/
} AUDIO_PLAYEND_IND_MSG_T;

/*! \struct AUDIO_PLAYEND_RSP_MSG_T
 *  \brief Playend indication response.
 *  After received the indication, it is recommend that upper layer response to audio servcie.
 */
typedef struct
{
    SIGNAL_VARS     /*!<Signal Header.*/
    HAUDIO hAudioHandle;    /*!<Audio handle.*/
} AUDIO_PLAYEND_RSP_MSG_T;

/*! \struct AUDIO_RECORD_FINISH_IND_MSG_T
 *  \brief Record end indication.
 *  When an audio record ends, audio service will send an indication to upper laye to inform the audio 
 *  record finished.
 */
typedef struct
{
    SIGNAL_VARS     /*!<Signal Header.*/
    HAUDIO hAudioHandle;    /*!<Audio handle.*/
} AUDIO_RECORD_FINISH_IND_MSG_T;

typedef struct
{
    SIGNAL_VARS     /*!<Signal Header.*/
    HAUDIO hAudioHandle;    /*!<Audio handle.*/
    AUDIO_RESULT_E * peResult; /*!<result.*/	
    AUDIO_PLAYINFO_T *playinginfo_p; /*!<result.*/	
} AUDIO_GETPLAYINGINFO_REQ_MSG_T;

/*! \struct AUDIO_RECORD_FINISH_RSP_MSG_T
 *  \brief Record finish indication response.
 * It is not necessitous.
 */
typedef struct
{
    SIGNAL_VARS     /*!<Signal Header.*/
    HAUDIO hAudioHandle;    /*!<Audio handle.*/
} AUDIO_RECORD_FINISH_RSP_MSG_T;

/*! \struct AUDIO_RECORD_WRITE_ERR_IND_MSG_T
 *  \brief Record write error indication.
 *  When some errors heppened, audio service will send an indication to upper laye to inform 
 *  the error.
 */
typedef struct
{
    SIGNAL_VARS     /*!<Signal Header.*/
    HAUDIO hAudioHandle;    /*!<Audio handle.*/
    uint32 uiError;     /*!<Error type. Error report message's signal code.*/
} AUDIO_RECORD_WRITE_ERR_IND_MSG_T;

/*! \struct AUDIO_RECORD_WRITE_ERR_RSP_MSG_T
 *  \brief Record error indication response.
 * It is not necessitous.
 */
typedef struct
{
    SIGNAL_VARS     /*!<Signal Header.*/
    HAUDIO hAudioHandle;    /*!<Audio handle.*/
} AUDIO_RECORD_WRITE_ERR_RSP_MSG_T;

/*! \struct AUDIO_STOPPED_IND_MSG_T
 *  \brief Audio stopped indication.
 *  When one audio is stopped, audio service will send this indication to upper layer to inform the 
 *  stop reason.
 */
typedef struct
{
    SIGNAL_VARS     /*!<Signal Header.*/
    HAUDIO hAudioHandle;    /*!<Audio handle.*/
    uint32 uiReason;     /*!<Stop reason type.*/
} AUDIO_STOPPED_IND_MSG_T;

/*! \struct AUDIO_STOPPED_RSP_MSG_T
 *  \brief Audio stopped indication response.
 *  After received the indication, it is recommend that upper layer response to audio servcie.
 */
typedef struct
{
    SIGNAL_VARS     /*!<Signal Header.*/
    HAUDIO hAudioHandle;    /*!<Audio handle.*/
} AUDIO_STOPPED_RSP_MSG_T;

/*! \struct AUDIO_TRACKBUF_STATUS_REP_MSG_T
 *  \brief Trackbuf Status report.
 */
typedef struct
{
    SIGNAL_VARS        /*!<Signal Header.*/
    uint32 uiAudioObj; /*!<Audio Object.*/
    AUDIO_TRACKBUF_STATUS_E eStatus; /*!< TrackBuf Status.*/
} AUDIO_TRACKBUF_STATUS_REP_MSG_T;

/*! \struct AUDIO_TRACKBUF_STATUS_IND_MSG_T
 *  \brief Trackbuf Status indication.
 */
typedef struct
{
    SIGNAL_VARS     /*!<Signal Header.*/
    HAUDIO hAudioHandle;    /*!<Audio handle.*/
    AUDIO_TRACKBUF_STATUS_E eStatus; /*!< TrackBuf Status.*/
} AUDIO_TRACKBUF_STATUS_IND_MSG_T;

/*! \struct AUDIO_TRACKBUF_STATUS_RSP_MSG_T
 *  \brief Trackbuf Status indication response.
 */
typedef struct
{
    SIGNAL_VARS     /*!<Signal Header.*/
    HAUDIO hAudioHandle;    /*!<Audio handle.*/
} AUDIO_TRACKBUF_STATUS_RSP_MSG_T;

/*! \struct AUDIO_PAUSEEND_IND_MSG_T
 *  \brief Pause end indication.
 *  When an audio pauses end, audio service will send an indication to upper laye to inform the audio pause 
 *  finished.
 */
typedef struct
{
    SIGNAL_VARS     /*!<Signal Header.*/
    HAUDIO hAudioHandle;    /*!<Audio handle.*/
    AUDIO_RESULT_E eResult; /*!< Pause result.*/
} AUDIO_PAUSEEND_IND_MSG_T;

/*! \struct AUDIO_PAUSEEND_RSP_MSG_T
 *  \brief Pauseend indication response.
 *  After received the indication, it is recommend that upper layer response to audio servcie.
 */
typedef struct
{
    SIGNAL_VARS     /*!<Signal Header.*/
    HAUDIO hAudioHandle;    /*!<Audio handle.*/
} AUDIO_PAUSEEND_RSP_MSG_T;
/*----------------------------------------------------------------------------*/
/*! Request:
 *  Requestion is launched by upper layer to inform audio service to do something.
 */
/*----------------------------------------------------------------------------*/
 /*! \struct AUDIO_CREATE_FILE_HANDLE_REQ_MSG_T
 *  \brief Request to create an file audio.
 *          corresponding signal: AUDIO_CREATE_FILE_REQ
 */
typedef struct 
{
    SIGNAL_VARS     /*!<Signal Header.*/
    HAUDIOCODEC hAudioCodec;  
    const uint16* pusCodecName;
    HAUDIODEV hDevice;
    const uint16* pusDevName;
    const int16* psFileName;
    uint32* puiTrackBuffer;     // decode data output buffer.
                                        // This buffer is created by caller but managed by voiceband 
                                        // controller.
    uint32 uiTrackBufferSize;   // the track buffer size.
    AUDIO_NOTIFY_CALLBACK_PFUNC pNotifyCallbackFunc;/*!< callback func when as notify mmi or other audio_api caller.*/
    HAUDIO *phAudio;//[OUT]when asynchronous
} AUDIO_CREATE_FILE_HANDLE_REQ_MSG_T;

 /*! \struct AUDIO_CREATE_BUFFER_HANDLE_REQ_MSG_T
 *  \brief Request to create an buffer audio.
 *          corresponding signal: AUDIO_CREATE_BUFFER_REQ
 */
typedef struct 
{
    SIGNAL_VARS     /*!<Signal Header.*/
    HAUDIOCODEC hAudioCodec;  
    const uint16* pusCodecName;
    HAUDIODEV hDevice;
    const uint16* pusDevName;
    const uint8* pucData;  // the music data pointer
    uint32 uiDataLength;    // the music data length
    uint32* puiTrackBuffer;     // decode data output buffer.
                                        // This buffer is created by caller but managed by voiceband 
                                        // controller.
    uint32 uiTrackBufferSize;   // the track buffer size.
    AUDIO_NOTIFY_CALLBACK_PFUNC pNotifyCallbackFunc; /*!< callback func when as notify mmi or other audio_api caller.*/
    HAUDIO *phAudio;//[OUT]when asynchronous
} AUDIO_CREATE_BUFFER_HANDLE_REQ_MSG_T;

 /*! \struct AUDIO_CREATE_STREAMBUFFER_HANDLE_REQ_MSG_T
 *  \brief Request to create an stream buffer audio.
 *          corresponding signal: AUDIO_CREATE_STREAMBUFFER_REQ
 */
typedef struct 
{
    SIGNAL_VARS     /*!<Signal Header.*/
    HAUDIOCODEC hAudioCodec;  
    const uint16* pusCodecName;
    HAUDIODEV hDevice;
    const uint16* pusDevName;
    const uint8* pucData;  // the music data pointer
    uint32 uiDataLength;   // the music data length
    uint32* puiTrackBuffer;     // decode data output buffer.
                                        // This buffer is created by caller but managed by voiceband 
                                        // controller.
    uint32 uiTrackBufferSize;   // the track buffer size.	
    AUDIO_NOTIFY_CALLBACK_PFUNC pNotifyCallbackFunc; /*!< callback func when as notify mmi or other audio_api caller.*/
    AUDIO_GETSRCDATA_CALLBACK_PFUNC pGetSrcDataCallbackFunc;      // callback function. It is called when the 
    										//codec need new src data.	
    HAUDIO *phAudio;//[OUT]when asynchronous
} AUDIO_CREATE_STREAMBUFFER_HANDLE_REQ_MSG_T;

 /*! \struct AUDIO_CREATE_RECORD_FILE_HANDLE_REQ_MSG_T
 *  \brief Request to create an record file audio.
 *          corresponding signal: AUDIO_CREATE_RECORD_FILE_REQ
 */
typedef struct 
{
    SIGNAL_VARS     /*!<Signal Header.*/
    HAUDIOCODEC hAudioCodec;
    const uint16* pusCodecName;
    HAUDIODEV hDevice;
    const uint16* pusDevName;   
    BOOLEAN bIsFileExisted;
    const int16* psFileName;
    SFS_HANDLE hFileHandle;
    uint32* puiBuffer;
    uint32 uiBufferSize;
    AUDIO_NOTIFY_CALLBACK_PFUNC pNotifyCallbackFunc; /*!< callback func when as notify mmi or other audio_api caller.*/
    HAUDIO *phAudio;//[OUT]when asynchronous 
} AUDIO_CREATE_RECORD_FILE_HANDLE_REQ_MSG_T;

 /*! \struct AUDIO_CREATE_RECORD_BUFFER_HANDLE_REQ_MSG_T
 *  \brief Request to create an record buffer audio.
 *          corresponding signal: AUDIO_CREATE_RECORD_BUFFER_REQ
 */
typedef struct 
{
    SIGNAL_VARS     /*!<Signal Header.*/
    HAUDIOCODEC hAudioCodec;
    const uint16* pusCodecName;
    HAUDIODEV hDevice;
    const uint16* pusDevName;
    uint8* pucData;
    uint32 uiDataLength;
    uint32* puiBuffer;
    uint32 uiBufferSize;
    AUDIO_NOTIFY_CALLBACK_PFUNC pNotifyCallbackFunc; /*!< callback func when as notify mmi or other audio_api caller.*/
    HAUDIO *phAudio;//[OUT]when asynchronous
} AUDIO_CREATE_RECORD_BUFFER_HANDLE_REQ_MSG_T;

 /*! \struct AUDIO_CREATE_NORMAL_HANDLE_REQ_MSG_T
 *  \brief Request to create an record buffer audio.
 *          corresponding signal: AUDIO_CREATE_NORMAL_REQ
 */
typedef struct 
{
    SIGNAL_VARS     /*!<Signal Header.*/
    HAUDIOCODEC hAudioCodec;
    const uint16* pusCodecName;
    HAUDIODEV hDevice;
    const uint16* pusDevName;
    AUDIO_NOTIFY_CALLBACK_PFUNC pNotifyCallbackFunc; 
    HAUDIO *phAudio;//[OUT]when asynchronous
} AUDIO_CREATE_NORMAL_HANDLE_REQ_MSG_T;

 /*! \struct AUDIO_CREATE_CLONE_HANDLE_REQ_MSG_T
 *  \brief Request to create an record buffer audio.
 *          corresponding signal: AUDIO_CREATE_NORMAL_REQ
 */
typedef struct 
{
    SIGNAL_VARS     /*!<Signal Header.*/
    HAUDIO hAudioToBeCloned;
    HAUDIODEV hDevice;
    const uint16* pusDevName;
    uint32* puiTrackBuffer;     // decode data output buffer.
                                        // This buffer is created by caller but managed by voiceband 
                                        // controller.
    uint32 uiTrackBufferSize;   // the track buffer size.
    AUDIO_NOTIFY_CALLBACK_PFUNC pNotifyCallbackFunc; 
    HAUDIO *phAudio;//[OUT]when asynchronous
} AUDIO_CREATE_CLONE_HANDLE_REQ_MSG_T;
 
/*! \struct AUDIO_PLAY_REQ_MSG_T
 *  \brief Request to play an audio.
 */
typedef struct 
{
    SIGNAL_VARS     /*!<Signal Header.*/
    uint32	uiOffset;   /*!<Audio beginning offset.*/
    HAUDIO hAudioHandle;    /*!<Audio handle.*/    
    AUDIO_RESULT_E * peResult; /*!<result.*/	
} AUDIO_PLAY_REQ_MSG_T;

/*! \struct AUDIO_STOP_REQ_MSG_T
 *  \brief Request to stop an audio.
 */
typedef struct
{
    SIGNAL_VARS     /*!<Signal Header.*/
    HAUDIO hAudioHandle;    /*!<Audio handle.*/ 
    BOOLEAN b_syn_flag;     /*!<syn or asyn flag, true:syn, false:asyn.*/
     AUDIO_RESULT_E * peResult; /*!<result.*/
} AUDIO_STOP_REQ_MSG_T;

/*! \struct AUDIO_PAUSE_REQ_MSG_T
 *  \brief Request to pause an audio.
 */
typedef struct
{
    SIGNAL_VARS     /*!<Signal Header.*/
    HAUDIO hAudioHandle;    /*!<Audio handle.*/
    BOOLEAN b_syn_flag;     /*!<syn or asyn flag, true:syn, false:asyn.*/
    AUDIO_RESULT_E * peResult; /*!<result.*/
} AUDIO_PAUSE_REQ_MSG_T;

/*! \struct AUDIO_RESUME_REQ_MSG_T
 *  \brief Request to resume an audio.
 */
typedef struct
{
    SIGNAL_VARS     /*!<Signal Header.*/
    HAUDIO hAudioHandle;    /*!<Audio handle.*/
    AUDIO_RESULT_E * peResult; /*!<result.*/	
} AUDIO_RESUME_REQ_MSG_T;

/*! \struct AUDIO_SEEK_REQ_MSG_T
 *  \brief Request to seek the new audio position.
 */
typedef struct
{
    SIGNAL_VARS     /*!<Signal Header.*/
    HAUDIO hAudioHandle;    /*!<Audio handle.*/
    uint32 uiOffset;    /*!<Destination audio position*/
    AUDIO_RESULT_E * peResult; /*!<result.*/	
} AUDIO_SEEK_REQ_MSG_T;

/*! \struct AUDIO_TIMESEEK_REQ_MSG_T
 *  \brief Request to seek the new audio time.
 */
typedef struct
{
    SIGNAL_VARS     /*!<Signal Header.*/
    HAUDIO hAudioHandle;    /*!<Audio handle.*/
    uint32 uiTime;    /*!<Destination audio time*/
    AUDIO_RESULT_E * peResult; /*!<result.*/	
} AUDIO_TIMESEEK_REQ_MSG_T;

/*! \struct AUDIO_SETSPEED_REQ_MSG_T
 *  \brief Request to set play speed.
 */
typedef struct
{
    SIGNAL_VARS     /*!<Signal Header.*/
    HAUDIO hAudioHandle;    /*!<Audio handle.*/
    int32 uiSpeedX;    /*!<Playing speed. 2X, 4X, 8X ...*/
    AUDIO_RESULT_E * peResult; /*!<result.*/	
} AUDIO_SETSPEED_REQ_MSG_T;

/*! \struct AUDIO_SETDIGIGAIN_REQ_MSG_T
 *  \brief Request to set digital gain.
 */
typedef struct
{
    SIGNAL_VARS     /*!<Signal Header.*/
    HAUDIO hAudioHandle;    /*!<Audio handle.*/
    uint32 uiGain;  /*!<Gain value. Unit: dB*/
    AUDIO_RESULT_E * peResult; /*!<result.*/	
} AUDIO_SETDIGIGAIN_REQ_MSG_T;

/*! \struct AUDIO_GETDIGIGAIN_REQ_MSG_T
 *  \brief Request to get the playing digital gain.
 */
typedef struct
{
    SIGNAL_VARS     /*!<Signal Header.*/
    HAUDIO hAudioHandle;    /*!<Audio handle.*/
    AUDIO_RESULT_E * peResult; /*!<result.*/	
    uint32 *puiGain; /*!<result.*/	
} AUDIO_GETDIGIGAIN_REQ_MSG_T;


/*! \struct AUDIO_TURNON_EXP_REQ_MSG_T
 *  \brief Request to turn on an expression plugger.
 */
typedef struct
{
    SIGNAL_VARS     /*!<Signal Header.*/
    HAUDIO hAudioHandle;    /*!<Audio handle.*/
    HAUDIOEXP hExpressHandle;   /*!<Expression handle.*/
    AUDIO_RESULT_E * peResult; /*!<result.*/	
} AUDIO_TURNON_EXP_REQ_MSG_T;

/*! \struct AUDIO_SHUTDOWN_EXP_REQ_MSG_T
 *  \brief Request to shut down an expression plugger.
 */
typedef struct
{
    SIGNAL_VARS     /*!<Signal Header.*/
    HAUDIO hAudioHandle;    /*!<Audio handle.*/
    HAUDIOEXP hExpressHandle;   /*!<Expression handle.*/
    AUDIO_RESULT_E * peResult; /*!<result.*/	
} AUDIO_SHUTDOWN_EXP_REQ_MSG_T;

/*! \struct AUDIO_CLOSE_HANDLE_REQ_MSG_T
 *  \brief Request to close an audio handle.
 *
 *  Because audio stop operation is asynchronous, we have to design the 
 *  handle close operation is asynchronous too. Otherwise, when we close 
 *  an audio handle, the stop operation has not been finished perhaps. 
 */
typedef struct
{
    SIGNAL_VARS     /*!<Signal Header.*/
    HAUDIO hAudioHandle;    /*!<Audio handle.*/
    AUDIO_RESULT_E * peResult; /*!<result.*/
} AUDIO_CLOSE_HANDLE_REQ_MSG_T;

/*! \struct AUDIO_SEND_STREAMSRC_DATA_REQ_MSG_T
 *  \brief notify as src_data be ready in stream_play.
 *          corresponding signal: AUDIO_SEND_STREAM_SRC_DATA_REQ
 */
typedef struct
{
    SIGNAL_VARS     /*!<Signal Header.*/
    HAUDIO hAudioHandle;    /*!<Audio handle.*/
    AUDIO_STREAM_CURSRCDATA_INFO_T *tpStreamCurSrcDataInfo;
    AUDIO_RESULT_E * peResult; /*!<result.*/	
} AUDIO_SEND_STREAMSRC_DATA_REQ_MSG_T;

 /*! \struct AUDIO_SWITCH_DEVICE_REQ_MSG_T
 *  \brief Request to switch device.
 *          corresponding signal: AUDIO_SWITCH_DEVICE_REQ
 */
typedef struct 
{
    SIGNAL_VARS     /*!<Signal Header.*/
    HAUDIO hAudioHandle;    /*!<Audio handle.*/
    HAUDIODEV hDevice;
    uint16* pusDevName;
    AUDIO_RESULT_E *peResult;
} AUDIO_SWITCH_DEVICE_REQ_MSG_T;

  /*! \struct AUDIO_SET_VOL_REQ_MSG_T
 *  \brief Request to set volume.
 *          corresponding signal: AUDIO_SET_VOL_REQ
 */
typedef struct 
{
    SIGNAL_VARS     /*!<Signal Header.*/
    uint32 uiVol;   /*!<volume.*/
    AUDIO_ARM_APP_TYPE_E eAppType;/*!<volume type.*/
    AUDIO_RESULT_E *peResult;  /*!<result.*/
} AUDIO_SET_VOL_REQ_MSG_T;

    /*! \struct AUDIO_SET_VOL_EX_REQ_MSG_T
 *  \brief Request to set volume.
 *          corresponding signal: AUDIO_SET_VOL_REQ
 */
typedef struct 
{
    SIGNAL_VARS     /*!<Signal Header.*/
    HAUDIO hAudioHandle;		
    uint32 uiVol;   /*!<volume.*/
    AUDIO_ARM_APP_TYPE_E eAppType;/*!<volume type.*/
    AUDIO_RESULT_E *peResult;  /*!<result.*/
} AUDIO_SET_VOL_EX_REQ_MSG_T;

#ifdef BT_DIALER_SET_VOLGAIN  
  /*! \struct AUDIO_SET_VOL_gain_REQ_MSG_T
 *  \brief Request to set gain value for volume.
 *          corresponding signal: AUDIO_SET_EXTRA_VOL_REQ
 */
typedef struct 
{
    SIGNAL_VARS     /*!<Signal Header.*/
    AUDIO_ARM_APP_TYPE_E eAppType;/*!<volume type.*/
    AUDIO_VOLUME_CONFIG_T *param;
    AUDIO_RESULT_E *peResult;  /*!<result.*/
} AUDIO_SET_VOL_gain_REQ_MSG_T; 
#endif

typedef struct 
{
    SIGNAL_VARS     /*!<Signal Header.*/
    BOOLEAN bExtraVolFlag;   /*!<extra volume flag, enable or disable.*/
    uint16 usExtraVolGain;/*!<extra volume gain.*/
    AUDIO_RESULT_E *peResult;  /*!<result.*/
} AUDIO_SET_EXTRA_VOL_REQ_MSG_T;

    /*! \struct AUDIO_SET_EXTRA_VOL_EX_REQ_MSG_T
 *  \brief Request to set extra volume even if the config is the same with the older one.
 *          corresponding signal: AUDIO_SET_EXTRA_VOL_EX_REQ
 */
typedef struct 
{
    SIGNAL_VARS     /*!<Signal Header.*/
    HAUDIO hAudioHandle;		
    BOOLEAN bExtraVolFlag;   /*!<extra volume flag, enable or disable.*/
    uint16 usExtraVolGain;/*!<extra volume gain.*/
    AUDIO_RESULT_E *peResult;  /*!<result.*/
} AUDIO_SET_EXTRA_VOL_EX_REQ_MSG_T;

    /*! \struct AUDIO_SET_MODE_REQ_MSG_T
 *  \brief Request to set mode.
 *          corresponding signal: AUDIO_SET_MODE_REQ
 */
typedef struct 
{
    SIGNAL_VARS     /*!<Signal Header.*/
    AUDIO_DEVICE_MODE_TYPE_E eDevMode;   /*!<device mode.*/
    AUDIO_RESULT_E *peResult;  /*!<result.*/
} AUDIO_SET_MODE_REQ_MSG_T;

     /*! \struct AUDIO_SET_MODE_EX_REQ_MSG_T
 *  \brief Request to set mode mandatorily.
 *          corresponding signal: AUDIO_SET_MODE_EX_REQ
 */
typedef struct 
{
    SIGNAL_VARS     /*!<Signal Header.*/
    AUDIO_DEVICE_MODE_TYPE_E eDevMode;   /*!<device mode.*/
    HAUDIO hAudioHandle;
    AUDIO_RESULT_E *peResult;  /*!<result.*/
} AUDIO_SET_MODE_EX_REQ_MSG_T;
    
    /*! \struct AUDIO_SET_UPLINKMUTE_REQ_MSG_T
 *  \brief Request to set uplink mute.
 *          corresponding signal: AUDIO_SET_UPLINKMUTE_REQ
 */
typedef struct 
{
    SIGNAL_VARS     /*!<Signal Header.*/
    BOOLEAN  bUpLinkMute;   /*!<SCI_TRUE: Mute, SCI_FALSE: Don't mute.*/
    AUDIO_RESULT_E *peResult;  /*!<result.*/
} AUDIO_SET_UPLINKMUTE_REQ_MSG_T;

    /*! \struct AUDIO_SET_DOWNLINKMUTE_REQ_MSG_T
 *  \brief Request to set downlink mute.
 *          corresponding signal: AUDIO_SET_DOWNLINKMUTE_REQ
 */
typedef struct 
{
    SIGNAL_VARS     /*!<Signal Header.*/
    BOOLEAN  bDownLinkMute;   /*!<SCI_TRUE: Mute, SCI_FALSE: Don't mute.*/
    AUDIO_RESULT_E *peResult;  /*!<result.*/
} AUDIO_SET_DOWNLINKMUTE_REQ_MSG_T;


/*! \struct AUDIO_SET_DOWNLINKLOCALNETMUTE_REQ_MSG_T
 *  \brief Request to set downlink mute.
 *          corresponding signal: AUDIO_SET_DOWNLINKLOCALNETMUTE_REQ
 */
typedef struct 
{
    SIGNAL_VARS     /*!<Signal Header.*/
    BOOLEAN  bDownLinkLocalMute;   /*!<SCI_TRUE: Mute, SCI_FALSE: Don't mute.*/
	BOOLEAN  bDownLinkNetMute;   /*!<SCI_TRUE: Mute, SCI_FALSE: Don't mute.*/
    AUDIO_RESULT_E *peResult;  /*!<result.*/
} AUDIO_SET_DOWNLINKLOCALNETMUTE_REQ_MSG_T;
 
    /*! \struct AUDIO_SET_DEVDIGGAIN_REQ_MSG_T
 *  \brief Request to set device digital gain..
 *          corresponding signal: AUDIO_SET_DEVDIGGAIN_REQ
 */
typedef struct 
{
    SIGNAL_VARS     /*!<Signal Header.*/
    uint32 uiDevDigGain;   /*!<device digital gain.*/
    AUDIO_RESULT_E *peResult;  /*!<result.*/
} AUDIO_SET_DEVDIGGAIN_REQ_MSG_T;

    /*! \struct AUDIO_SET_DATA_OUTPUT_CHANNEL_CHOICE_REQ_MSG_T
 *  \brief Request to set device digital gain..
 *          corresponding signal: AUDIO_SET_DATA_OUTPUT_CHANNEL_CHOICE_REQ
 */
typedef struct 
{
    SIGNAL_VARS     /*!<Signal Header.*/
    DEV_DATA_OUTPUT_CHANNEL_CHOICE_E eChannelChoice;   /*!<channel choice.*/
    AUDIO_RESULT_E *peResult;  /*!<result.*/
} AUDIO_SET_DATA_OUTPUT_CHANNEL_CHOICE_REQ_MSG_T;

/*! \struct AUDIO_CHANGE_CODEC_TASK_PRI_REQ_MSG_T
 *  \brief Request to change priority of codec task.
 */
typedef struct
{
    SIGNAL_VARS     /*!<Signal Header.*/
    uint32 uiAudioObj; /*!<Audio Object.*/
    BOOLEAN bHighPriority;     /*!<SCI_TRUE: high, SCI_FALSE: low.*/
} AUDIO_CHANGE_CODEC_TASK_PRI_REQ_MSG_T;

  /*! \struct AUDIO_SET_VOL_REQ_MSG_T
 *  \brief Request to set track on or off.
 *          corresponding signal: AUDIO_SET_TRACK_ON_REQ
 */
typedef struct 
{
    SIGNAL_VARS     /*!<Signal Header.*/
    uint32 uiAudioObj; /*!<Audio Object.*/
    BOOLEAN bIsTrackOn;  /*!<bIsTrackOn.*/
    AUDIO_RESULT_E *peResult;  /*!<result.*/
} AUDIO_SET_TRACK_ON_MSG_T;

 /*! \struct AUDIO_EXE_CODEC_EXT_OPE_REQ_MSG_T
 *  \brief Request to execute extend operation with collision of codec.
 *          corresponding signal: AUDIO_EXE_CODEC_EXT_OPE_REQ
 */
typedef struct 
{
    SIGNAL_VARS     /*!<Signal Header.*/
    HAUDIO hAudioHandle;
	void *ptOneExtOpe;
	uint32 uiArgc;
	void *pvArgv; 
	AUDIO_RESULT_E *peResult;
} AUDIO_EXE_CODEC_EXT_OPE_REQ_MSG_T;
/*----------------------------------------------------------------------------*/
/*! Report:
 * Report is sent from sub audio task to audio service task.*/
/*----------------------------------------------------------------------------*/ 
/*! \struct AUDIO_ERROR_REP_MSG_T
 *  \brief Play error report.
 *  When an audio play error occured, the codec task will send an report to audio service to inform 
 *  the event.
 */
typedef struct
{
    SIGNAL_VARS     /*!<Signal Header.*/
    HAUDIO hAudioHandle;    /*!<Audio handle.*/
    AUDIO_RESULT_E eErrorType; /*!< error type.*/
} AUDIO_ERROR_REP_MSG_T;

/*! \struct AUDIO_PLAYEND_REP_MSG_T
 *  \brief Playend report.
 *  When an audio play end, the codec task will send an report to audio service to inform the event.
 */
typedef struct
{
    SIGNAL_VARS     /*!<Signal Header.*/
    HAUDIO hAudioHandle;    /*!<Audio handle.*/
    AUDIO_RESULT_E eResult; /*!< Stop result.*/
} AUDIO_PLAYEND_REP_MSG_T;

/*! \struct AUDIO_RECORD_SUCCESS_REP_MSG_T
 *  \brief Record success report.
 *  Indicate the record success.
 */
typedef struct
{
    SIGNAL_VARS     /*!<Signal Header.*/
    HAUDIO hAudioHandle;    /*!<Audio handle.*/
} AUDIO_RECORD_SUCCESS_REP_MSG_T;

/*! \struct AUDIO_RECORD_WRITE_FAILED_REP_MSG_T
 *  \brief Write failed report.
 *  Indicate write fail.
 */
typedef struct
{
    SIGNAL_VARS     /*!<Signal Header.*/
    HAUDIO hAudioHandle;    /*!<Audio handle.*/
} AUDIO_RECORD_WRITE_FAILED_REP_MSG_T;

/*! \struct AUDIO_RECORD_WRITE_HEADER_FAILED_REP_MSG_T
 *  \brief Write header error report.
 *  Indicate write file header failed.
 */
typedef struct
{
    SIGNAL_VARS     /*!<Signal Header.*/
    HAUDIO hAudioHandle;    /*!<Audio handle.*/
} AUDIO_RECORD_WRITE_HEADER_FAILED_REP_MSG_T;

/*! \struct AUDIO_PAUSE_REP_MSG_T
 *  \brief Play result report.
 *  After codec manage the pause signal from as, the codec task will send a report to audio service to inform 
 *  the event codec has response the pause signal .
 */
typedef struct
{
    SIGNAL_VARS     /*!<Signal Header.*/
    HAUDIO hAudioHandle;    /*!<Audio handle.*/
    AUDIO_RESULT_E eResult; /*!< Stop result.*/
} AUDIO_PAUSEEND_REP_MSG_T;


typedef struct
{
    SIGNAL_VARS     /*!<Signal Header.*/
    HAUDIO hAudioHandle;    /*!<Audio handle.*/
} AUDIO_STOPSELF_MSG_T;

/*===========================================================*/
/*! \struct AUDIO_FileStream_struct
 *  \brief Audio stream sturcture which is based on file system.
 */
struct AUDIO_FileStream_struct
{
    SFS_HANDLE fFileHandle;    /*!< File handle.*/
    uint32 uiFileLength;        /*!< File length. Unit:Byte.*/
    uint16 disk_denotation[AUDIO_SFS_DEVICE_LEN]; /*!< The denotation of disk in which the file lies.*/
};

/*! \struct AUDIO_BufferStream_struct
 *  \brief Audio stream sturcture which is based on data buffer.
 */
struct AUDIO_BufferStream_struct
{
    uint8* pucDataBuffer;   /*!< Data buffer's beginning pointer.*/
    uint32 uiBufferLength;  /*!< Data length. Unit:Byte*/
};

/*! \union AUDIO_Stream_union
 *  \brief Audio stream union which includes file stream and buffer stream.
 */
union AUDIO_Stream_union
{
    struct AUDIO_FileStream_struct tAudioFileStream;    /*!< File stream.*/
    struct AUDIO_BufferStream_struct tAudioBufferStream;    /*!< Data buffer stream.*/
};

typedef enum
{
	AUDIO_PLAYING_FLAG_STOPPED,
	AUDIO_PLAYING_FLAG_PLAYING,
	AUDIO_PLAYING_FLAG_STARTPAUSE,	
	AUDIO_PLAYING_FLAG_PAUSING,	
	AUDIO_PLAYING_FLAG_STARTSTOP,
	AUDIO_PLAYING_FLAG_SELFSTARTSTOP
}AUDIO_PLAYING_FLAG_E;

typedef struct
{
    int16 *left_exp_data_ptr[2];
    int16 *right_exp_data_ptr[2];
    uint32 exp_data_max_len;
    uint32 exp_count[2];
    uint8 * src_data_ptr;//pointer of buffer space to save src data.	
    void *stream_head_info_ptr;
    void *other_info_ptr;	
}AUDIO_CODEC_PROCESS_RESOUCE_T;

/*! \struct AUDIO_CODEC_EXT_CFGINFO_T
 *  \brief definition of the structure of external config info.
 */
typedef struct AUDIO_Codec_Ext_CfgInfo_struct
{    
    uint32 uiHighPriority; /*!<high priority of codec.*/
    uint32 uiLowPriority; /*!<low priority of codec.*/
    uint32 uiBufFullWatermark; /*!<Track Buffer Full Watermark.*/
    uint32 uiBufEmptyWatermark; /*!<Track Buffer Empty Watermark.*/
    uint32 uiBufEmptyWindows; /*!<Track Buffer Empty Windows.*/
    uint32 uiBufTrackOnPoint; /*!<Track on point.percent.*/
    BOOLEAN bTrackBufStatusNotify; /*!<flag for support trackbuf status notification.*/
    uint8 pad0;
    uint16 pad1;
}AUDIO_CODEC_EXT_CFGINFO_T;

/*! \struct AUDIO_CALC_SPEC_MEM_T
 *  \brief definition of the structure of memory info for spec calculation.
 */
typedef struct  
{
   uint16*  inversedIndex; /*!<store inversed index.*/
   int32* Real; /*!<store the real part of fft results.*/
   int32* Imag; /*!<store the imag part of fft results.*/
} AUDIO_SPEC_CALC_MEM_T;

/*! \struct AUDIO_CALC_SPEC_MEM_T
 *  \brief definition of the structure of memory info for spec calculation.
 */
typedef struct AUDIO_EXP_IF_WORKED_IN_THIS_OBJ_struct 
{
   HAUDIOEXP hExp; 
   BOOLEAN bWorking;
   BOOLEAN bInitialized;
   uint16  res1;
   struct AUDIO_EXP_IF_WORKED_IN_THIS_OBJ_struct *pNext;
} AUDIO_EXP_IF_WORKED_IN_THIS_OBJ;

/*! \struct AUDIO_OBJECT_T
 *  \brief Audio object structure which saves all necessary information about one audio's 
 *           paly and record.
 */
typedef struct AUDIO_OBJECT_struct
{
    HAUDIO uiHandleID;  /*!< Audio Handle.*/
    HAUDIOCODEC hAudioCodec;    /*!< The codec which is used for this audio playing.*/
    HAUDIODEV hDevice;      /*!< Output device.*/
    BLOCK_ID tThreadId;   /*!< Audio subtask ID.*/
    BLOCK_ID tAssiThreadId; /*!<Audio assistant task for substask >*/
    BLOCK_ID tSender;    /*!< The caller of audio play.*/
    void** ppvSigIn;    /*!< Signal_in pointer that audio task received. We should free it when audio task is deleted.*/
    void** ppvSigOut;   /*!< Signal_out pinter that audio task prepared to send out when audio task is deleted. We should free it.*/
    union AUDIO_Stream_union unAudioStream; /*!< Audio stream.*/
    uint32 uiTrackNum;  /*!< This audio's track number which is assigned by voiceband controller.*/
    uint32* puiTrackBuffer; /*!< The track buffer this audio used. It should be alloced by caller.*/
    uint32 uiTrackBufferSize;   /*!< Track buffer size.*/
    uint32 uiUsedTrackBufferSize;   /*!< Track buffer size.*/
    uint32 uiOutputSamplerate;  /*!< Output samplerate.*/
    uint32 uiCodecOutputSamplerate; /*!< Output samplerate determined by codec, used for the different device to get appropriate output samplerate.*/
    AUDIO_PLAYING_FLAG_E ePlayingFlag;   /*!< This flag is used to indicate this audio is playing. It should be cleared after stop operation.*/

    uint32 uiCurSrcOffset; //current position of source file/buffer to read.
    
    AUDIO_CODEC_PROCESS_RESOUCE_T tCodecProcessResouce;  //info related about codec.	
    AUDIO_FORMATINFO_T tAudioFormatInfo;
    AUDIO_PLAYINFO_T tPlayingInfo;

    AUDIO_CODEC_EXT_CFGINFO_T tCodecExtCfgInfo; /*!< Codec External Config Info Pointer.*/
    AUDIO_TRACKBUF_STATUS_E  eTrackBufStatus; /*!< Track Buffer Status feedback to AS.*/
    SCI_TIMER_PTR ptTrackBufTimer; /*!< The Timer which is used for check buf status.*/
    SCI_TIMER_PTR ptTrackOnTimer; /*!< The Timer which is used to check track status.*/    
    uint32 uiTrackOnTimeLength; /*!< The time length from play/resume to the time track is on.*/    

    BLOCK_ID tStoporpauseSynCallerThreadId;   /*!< Audio subtask ID.*/
    AUDIO_RESULT_E eStoporpauseSynInterfaceResult;
    BOOLEAN bCurrentStoporpauseOpeSynFlag;  
    BOOLEAN bDevicePaused;
    BOOLEAN bTrackBufStatusNotifyHasClosed; /*!< when bTrackBufStatusNotify is SCI_TRUE, if data count is less than bufempty_watermark set this be SCI_TRUE, default:SCI_FALSE.*/
    BOOLEAN bHasEnableDumpPcmFunction;

    AUDIO_GETSRCDATA_CALLBACK_PFUNC pGetSrcCallbackFunc;	
    AUDIO_NOTIFY_CALLBACK_PFUNC pNotifyCallbackFunc; /*!<callback func when as notify mmi or other audio_api caller.*/

    BOOLEAN bIsSpecCaculatorOn;
    BOOLEAN bEnableDebugDataOutput;/*!<default:SCI_FALSE; SCI_FALSE: disable output debug data to file, SCI_TRUE:if pbEnableDebugDataOutput[index] is SCI_TRUE, the debug data in index positon can be output to file.*/
    uint8 ucDebugFileHandleCount;
    uint8 ucDebugFileLeftRightChannel;//reserve[62] bit8==1: enable left channel output; bit9==1: enable right channel output;
    
    BOOLEAN *pbEnableDebugDataOutput; /*!<if bEnableDebugDataOutput is SCI_TRUE and here is true, the debug data can be output to file.*/
    SFS_HANDLE *phDebugFileHandleList; /*!< To save file handle list these file is used to store the debug data after decoding or processing in every express plugin.*/
    SCI_MUTEX_PTR playing_info_mutex;

    SCI_MUTEX_PTR expIfWorkingInfoMutex;
    AUDIO_EXP_IF_WORKED_IN_THIS_OBJ *expIfWorkingInfo; /*!< for example: HAUDPROCExpºÍHAUDENHAExp should not work for bt output. */

	SCI_MUTEX_PTR writeTrackBufferMutex;
    BOOLEAN bJustFinishInitExp;//set be true if just finish express init but has not executed express plugger in playing 
    HAUDIO hClonedHandle;
    uint8 res0;
    uint16 res1;
}AUDIO_OBJECT_T;

/*! \struct AUDIO_OBJECT_LIST_T
 *  \brief Audio object structure which saves all necessary information about one audio's 
 *           paly and record.
 */
typedef struct  AUDIO_OBJECT_List_struct
{
    AUDIO_OBJECT_T tAudioObject;
    struct  AUDIO_OBJECT_List_struct * ptNextHandle;/*!< LInt pointer. Point to next element.*/
    struct  AUDIO_OBJECT_List_struct * ptPreHandle;/*!< Link pointer. Point to the element before current one.*/    
}AUDIO_OBJECT_LIST_T;

/**---------------------------------------------------------------------------*
 **                                                   Global Variables                                                         *
 **---------------------------------------------------------------------------*/
 
/**---------------------------------------------------------------------------*
 **                                                 Constant Variables                                                        *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                                                Function Prototypes                                                       *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//! \fn PUBLIC AUDIO_RESULT_E AUDIO_SM_InitServer(
//!    uint32 uiTaskPriority
//!    )
//! \param uiTaskPriority Audio service task's priority. It should be higher than any sub 
//!            audio tasks'.
//! \return Operation result. \sa AUDIO_RESULT_E
//! \brief  Description: This function is used to initialize audio service. Its task is : \par
//!                             1.Create an audio server for event sending; \par
//!                             2.Create an audio service task to receive and send signals; \par
//!                             3.Initialize Collision Manager, Codec Plugger Manager and Expression 
//!                                Plugger Manager;
//!                             4.Initialize Subtask Scheduler(This part will be realized later).
//! \author Author: Benjamin.Wang
//! \note   Note: Note.      
/*****************************************************************************/
PUBLIC AUDIO_RESULT_E AUDIO_SM_InitServer(
    void
    );

/*****************************************************************************/
//! \fn PUBLIC HAUDIO AUDIO_SM_CreateFileHandle(
//!    HAUDIOCODEC hAudioCodec, 
//!    const uint16* pusCodecName, 
//!    HAUDIODEV hDevice, 
//!    const uint16* pusDevName, 
//!    const int16* psFileName,  
//!    uint32* puiTrackBuffer, 
//!    uint32 uiTrackBufferSize,
//!    AUDIO_NOTIFY_CALLBACK_PFUNC pNotifyCallbackFunc, 
//!    BLOCK_ID idSender
//!    )
//! \param hAudioCodec the codec handle which will be used.
//! \param pusCodecName the codec name which will be used.
//! \param hDevice the device handle which will be used.
//! \param pusDevName the device name which will be used.
//! \param psFileName the file's name. 
//! \param puiTrackBuffer decode data output buffer. This buffer is created by caller but 
//!                                  managed by voiceband controller.
//! \param uiTrackBufferSize the track buffer size.
//! \param pNotifyCallbackFunc callback func when as notify mmi or other audio_api caller.
//! \param idSender caller id.
//! \return SUCCESS - return the audio handle created \pa
//!            FAILURE - return INVALID_HANDLE
//! \brief  Description: This function is used to create an audio handle which is based on FFS. 
//!                             Not all codec support file system.
//! \author Author: Benjamin.Wang
//! \note   Note: None.    
/*****************************************************************************/
PUBLIC HAUDIO AUDIO_SM_CreateFileHandle(
    HAUDIOCODEC hAudioCodec, 
    const uint16* pusCodecName, 
    HAUDIODEV hDevice, 
    const uint16* pusDevName, 
    const int16* psFileName, 
    uint32* puiTrackBuffer, 
    uint32 uiTrackBufferSize,
    AUDIO_NOTIFY_CALLBACK_PFUNC pNotifyCallbackFunc,
    BLOCK_ID idSender
    );

/*****************************************************************************/
//! \fn PUBLIC HAUDIO AUDIO_SM_CreateBufferHandle(
//!    HAUDIOCODEC hAudioCodec, 
//!    const uint16* pusCodecName, 
//!    HAUDIODEV hDevice, 
//!    const uint16* pusDevName, 
//!    const uint8* pucData, 
//!    uint32 uiDataLength, 
//!    uint32* puiTrackBuffer, 
//!    uint32 uiTrackBufferSize ,
//!    AUDIO_NOTIFY_CALLBACK_PFUNC pNotifyCallbackFunc, 
//!    BLOCK_ID idSender
//!    )
//! \param hAudioCodec the codec handle which will be used.
//! \param pusCodecName the codec name which will be used.
//! \param hDevice the device handle which will be used.
//! \param pusDevName the device name which will be used.
//! \param pucData the music data pointer.
//! \param uiDataLength the music data length.
//! \param puiTrackBuffer decode data output buffer. This buffer is created by caller but 
//!                                  managed by voiceband controller.
//! \param uiTrackBufferSize the track buffer size.
//! \param pNotifyCallbackFunc callback func when as notify mmi or other audio_api caller.
//! \param idSender caller id.
//! \return SUCCESS - return the audio handle created
//!            FAILURE - return INVALID_HANDLE
//! \brief  Description: This function is used to create an audio handle which is based on data 
//!                             buffer. Not all codec support buffer controller.
//! \author Author: Benjamin.Wang
//! \note   Note: None.     
/*****************************************************************************/
PUBLIC HAUDIO AUDIO_SM_CreateBufferHandle(
    HAUDIOCODEC hAudioCodec, 
    const uint16* pusCodecName, 
    HAUDIODEV hDevice, 
    const uint16* pusDevName, 
    const uint8* pucData, 
    uint32 uiDataLength, 
    uint32* puiTrackBuffer, 
    uint32 uiTrackBufferSize,
    AUDIO_NOTIFY_CALLBACK_PFUNC pNotifyCallbackFunc,
    BLOCK_ID idSender
    );

/*****************************************************************************/
//! \fn PUBLIC HAUDIO AUDIO_SM_CreateStreamBufferHandle(
//!    HAUDIOCODEC hAudioCodec, 
//!    const uint16* pusCodecName, 
//!    HAUDIODEV hDevice, 
//!    const uint16* pusDevName, 
//!    const uint8* pucData, 
//!    uint32 uiDataLength, 
//!    uint32* puiTrackBuffer, 
//!    uint32 uiTrackBufferSize ,
//!    AUDIO_NOTIFY_CALLBACK_PFUNC pNotifyCallbackFunc, 
//!    AUDIO_GETSRCDATA_CALLBACK_PFUNC pGetSrcDataCallbackFunc,
//!    BLOCK_ID idSender
//!    )
//! \param hAudioCodec the codec handle which will be used.
//! \param pusCodecName the codec name which will be used.
//! \param hDevice the device handle which will be used.
//! \param pusDevName the device name which will be used.
//! \param pucData the music data header pointer.
//! \param uiDataLength the music data header length.
//! \param puiTrackBuffer decode data output buffer. This buffer is created by caller but 
//!                                  managed by voiceband controller.
//! \param uiTrackBufferSize the track buffer size.
//! \param pNotifyCallbackFunc callback func when as notify mmi or other audio_api caller.
//! \param pGetSrcDataCallbackFunc callback function. It is called when the codec need new src data.
//! \param idSender caller id.
//! \return SUCCESS - return the audio handle created
//!            FAILURE - return INVALID_HANDLE
//! \brief  Description: This function is used to create an audio handle which is based on data 
//!                             stream. Not all codec support buffer controller.
//! \author Author: Benjamin.Wang
//! \note   Note: None.     
/*****************************************************************************/
PUBLIC HAUDIO AUDIO_SM_CreateStreamBufferHandle(
    HAUDIOCODEC hAudioCodec, 
    const uint16* pusCodecName, 
    HAUDIODEV hDevice, 
    const uint16* pusDevName, 
    const uint8* pucData, 
    uint32 uiDataLength, 
    uint32* puiTrackBuffer, 
    uint32 uiTrackBufferSize,
    AUDIO_NOTIFY_CALLBACK_PFUNC pNotifyCallbackFunc,
    AUDIO_GETSRCDATA_CALLBACK_PFUNC pGetSrcDataCallbackFunc,
    BLOCK_ID idSender
    );    

/*****************************************************************************/
//! \fn PUBLIC AUDIO_RESULT_E AUDIO_SM_CloseHandle(
//!    HAUDIO hAudioHandle
//!    )
//! \param hAudioHandle Audio handle which to be closed.
//! \return operation results. \sa AUDIO_RESULT_E
//! \brief  Description: This function is used to close an audio handle.
//! \author Author: Benjamin.Wang
//! \note   Note: None     
/*****************************************************************************/
PUBLIC AUDIO_RESULT_E AUDIO_SM_CloseHandle(
    HAUDIO hAudioHandle
    );

/*****************************************************************************/
//! \fn PUBLIC AUDIO_RESULT_E AUDIO_SM_Play(
//!    HAUDIO hAudioHandle, 
//!    uint32 uiOffset
//!    )
//! \param hAudioHandle Audio handle which will be played.
//! \param uiOffset Audio beginning offset.
//! \param pCallbackFunc Play end callback.
//! \return operation results. \sa AUDIO_RESULT_E
//! \brief  Description: This function is used to play an audio.
//! \author Author: Benjamin.Wang
//! \note   Note: None     
/*****************************************************************************/
PUBLIC AUDIO_RESULT_E AUDIO_SM_Play(
    HAUDIO hAudioHandle, 
    uint32 uiOffset
    );

/*****************************************************************************/
//! \fn PUBLIC AUDIO_RESULT_E AUDIO_SM_Stop(
//!    HAUDIO hAudioHandle,
//!    BOOLEAN b_syn_flag
//!    )
//! \param hAudioHandle Audio handle which will be stopped.
//! \param b_syn_flag if this func is synchronous. TRUE:syn, FALSE:asyn.
//! \return operation results. \sa AUDIO_RESULT_E
//! \brief  Description: This function is used to stop an audio.
//! \author Author: Benjamin.Wang
//! \note   Note: None     
/*****************************************************************************/
PUBLIC AUDIO_RESULT_E AUDIO_SM_Stop(
    HAUDIO hAudioHandle,
    BOOLEAN b_syn_flag
    );

/*****************************************************************************/
//! \fn PUBLIC AUDIO_RESULT_E AUDIO_SM_Pause(
//!    HAUDIO hAudioHandle,
//!    BOOLEAN b_syn_flag
//!    )
//! \param hAudioHandle Audio handle which will be paused.
//! \param b_syn_flag if this func is synchronous. TRUE:syn, FALSE:asyn.
//! \return operation results. \sa AUDIO_RESULT_E
//! \brief  Description: This function is used to pause an audio.
//! \author Author: Benjamin.Wang
//! \note   Note: None     
/*****************************************************************************/
PUBLIC AUDIO_RESULT_E AUDIO_SM_Pause(
    HAUDIO hAudioHandle,
    BOOLEAN b_syn_flag
    );

/*****************************************************************************/
//! \fn PUBLIC AUDIO_RESULT_E AUDIO_SM_Resume(
//!    HAUDIO hAudioHandle
//!    )
//! \param hAudioHandle Audio handle which will be resumed.
//! \return operation results. \sa AUDIO_RESULT_E
//! \brief  Description: This function is used to resume an paused audio.
//! \author Author: Benjamin.Wang
//! \note   Note: None     
/*****************************************************************************/
PUBLIC AUDIO_RESULT_E AUDIO_SM_Resume(
    HAUDIO hAudioHandle
    );

/*****************************************************************************/
//! \fn PUBLIC AUDIO_RESULT_E AUDIO_SM_Seek(
//!    HAUDIO hAudioHandle, 
//!    uint32 uiOffset
//!    )
//! \param hAudioHandle the audio handle.
//! \param uiOffset the destination position.
//! \return return operation result. \sa AUDIO_RESULT_E
//! \brief  Description: This function is used to seek random position of an audio.
//! \author Author: Benjamin.Wang
//! \note   Note: None     
/*****************************************************************************/
PUBLIC AUDIO_RESULT_E AUDIO_SM_Seek(
    HAUDIO hAudioHandle, 
    uint32 uiOffset
    );

/*****************************************************************************/
//! \fn PUBLIC AUDIO_RESULT_E AUDIO_SM_SetSpeed(
//!    HAUDIO hAudioHandle, 
//!    uint32 uiSpeedX
//!    )
//! \param hAudioHandle the audio handle.
//! \param uiSpeedX Speed multiple. For example, 2X, 4X and 8X, the uiSpeedX should 
//!                                                  be 2, 4 and 8. It can't be negative!
//! \return return operation result. \sa AUDIO_RESULT_E
//! \brief  Description: This function is used to set the playing speed of an audio.
//! \author Author: Benjamin.Wang
//! \note   Note: None.       
/*****************************************************************************/
PUBLIC AUDIO_RESULT_E AUDIO_SM_SetSpeed(
    HAUDIO hAudioHandle, 
    uint32 uiSpeedX
    );

/*****************************************************************************/
//! \fn PUBLIC AUDIO_RESULT_E AUDIO_SM_SetDigitalGain(
//!    HAUDIO hAudioHandle, 
//!    uint32 uiDigitalGain
//!    )
//! \param hAudioHandle the audio handle.
//! \param uiDigitalGain Digital Gain. Unit: dB
//! \return return operation result. \sa AUDIO_RESULT_E
//! \brief  Description: This function is used to set the codec digital gain.
//! \author Author: Benjamin.Wang
//! \note   Note: None.       
/*****************************************************************************/
PUBLIC AUDIO_RESULT_E AUDIO_SM_SetDigitalGain(
    HAUDIO hAudioHandle, 
    uint32 uiDigitalGain
    );

/*****************************************************************************/
//! \fn PUBLIC uint32 AUDIO_SM_GetDigitalGain(
//!    HAUDIO hAudioHandle
//!    )
//! \param hAudioHandle the audio handle.
//! \return return this audio's digital gain. If failed, return 0xFFFFFFFF.
//! \brief  Description: This function is used to get the codec digital gain. The gain value will 
//!                             be feedback in the AUDIO_GETDIGIGAIN_CNF message.
//! \author Author: Benjamin.Wang
//! \note   Note: None.       
/*****************************************************************************/
PUBLIC uint32 AUDIO_SM_GetDigitalGain(
    HAUDIO hAudioHandle
    );

/*****************************************************************************/
//! \fn PUBLIC AUDIO_RESULT_E AUDIO_SM_SendStreamSrcData(
//!    HAUDIO hAudioHandle, 
//!    AUDIO_STREAM_CURSRCDATA_INFO_T *tpStreamCurSrcDataInfo	
//!    )
//! \param hAudioHandle the audio handle.
//! \param tpStreamCurSrcDataInfo data info
//! \return return operation result. \sa AUDIO_RESULT_E
//! \brief  Description: This function is used to send stream src data to codec.
//! \author Author: Shujing.dong
//! \note   Note: None.       
/*****************************************************************************/
PUBLIC AUDIO_RESULT_E AUDIO_SM_SendStreamSrcData(   
    HAUDIO hAudioHandle, 
    AUDIO_STREAM_CURSRCDATA_INFO_T *tpStreamCurSrcDataInfo
    );

/*****************************************************************************/
//! \fn PUBLIC AUDIO_RESULT_E AUDIO_SM_IdentifyAudioFormat(
//!    HAUDIO hAudioHandle, 
//!    AUDIO_FORMATINFO_T* ptFileInfo
//!    )
//! \param hAudioHandle the audio handle.
//! \param ptFileInfo The data buffer pointer which is used to store the file information. 
//!                          This parameter is an OUT parameter.
//! \return return operation result. \sa AUDIO_RESULT_E
//! \brief  Description: This function is used to get all file information provided by codec.
//! \author Author: Benjamin.Wang
//! \note   Note: None.       
/*****************************************************************************/
PUBLIC AUDIO_RESULT_E AUDIO_SM_IdentifyAudioFormat(
    HAUDIO hAudioHandle, 
    AUDIO_FORMATINFO_T* ptFileInfo            // OUT parameter
    );

/*****************************************************************************/
//! \fn PUBLIC AUDIO_RESULT_E AUDIO_SM_GetPlayingInfo(
//!    HAUDIO hAudioHandle, 
//!    AUDIO_PLAYINFO_T* ptPlayInfo
//!    )
//! \param hAudioHandle the audio handle.
//! \param ptFileInfo The data buffer pointer which is used to store the playing information. 
//!                          This parameter is an OUT parameter.
//! \return return operation result. \sa AUDIO_RESULT_E
//! \brief  Description: This function is used to get the playing information. For example, the 
//!                             playing position, time, and so on.
//! \author Author: Benjamin.Wang
//! \note   Note: None.       
/*****************************************************************************/
PUBLIC AUDIO_RESULT_E AUDIO_SM_GetPlayingInfo(
    HAUDIO hAudioHandle, 
    AUDIO_PLAYINFO_T* ptPlayInfo
    );

/*****************************************************************************/
//! \fn PUBLIC AUDIO_OBJECT_T* AUDIO_SM_GetAudioObject(
//!    HAUDIO hAudioHandle
//!    )
//! \param hAudioHandle the audio handle.
//! \return return the audio object's pointer.
//! \brief  Description: This function is used to get specified audio handle's object.
//! \author Author: Benjamin.Wang
//! \note   Note: None.       
/*****************************************************************************/
PUBLIC AUDIO_OBJECT_T* AUDIO_SM_GetAudioObject(
    HAUDIO hAudioHandle
    );

/*****************************************************************************/
//! \fn PUBLIC HAUDIOCODEC AUDIO_SM_GetCodecHandle(
//!    HAUDIO hAudioHandle
//!    )
//! \param hAudioHandle the audio handle.
//! \return return specified audio's codec handle.
//! \brief  Description: This function is used to get specified audio's codec handle.
//! \author Author: Benjamin.Wang
//! \note   Note: None.       
/*****************************************************************************/
PUBLIC HAUDIOCODEC AUDIO_SM_GetCodecHandle(
    HAUDIO hAudioHandle
    );

/*****************************************************************************/
//! \fn PUBLIC HAUDIODEV AUDIO_SM_GetDevHandle(
//!    HAUDIO hAudioHandle
//!    )
//! \param hAudioHandle The audio handle.
//! \return return the storage medium. \sa HAUDIODEV
//! \brief  Description: This function is used to get audio's play device. 
//! \author Author: Benjamin.Wang
//! \note   Note: None.       
/*****************************************************************************/
PUBLIC HAUDIODEV AUDIO_SM_GetDevHandle(
    HAUDIO hAudioHandle
    );
    
/*****************************************************************************/
//! \fn PUBLIC AUDIO_RESULT_E AUDIO_SM_SetCodecHandle(
//!    HAUDIO hAudioHandle, 
//!    HAUDIOCODEC hCodecHandle
//!    )
//! \param hAudioHandle The audio handle.
//! \param hCodecHandle The codec handle will be set as.
//! \return return operation results. \sa AUDIO_RESULT_E
//! \brief  Description: This function is used to set audio's codec.
//! \author Author: Benjamin.Wang
//! \note   Note: None.       
/*****************************************************************************/
PUBLIC AUDIO_RESULT_E AUDIO_SM_SetCodecHandle(
    HAUDIO hAudioHandle, 
    HAUDIOCODEC hCodecHandle
    );

/*****************************************************************************/
//! \fn PUBLIC uint32 AUDIO_SM_GetFileSize(
//!    HAUDIO hAudioHandle
//!    )
//! \param hAudioHandle The audio handle.
//! \return return the filesize. If failed, return zero.
//! \brief  Description: This function is used to get audio's size.
//! \author Author: Benjamin.Wang
//! \note   Note: None.       
/*****************************************************************************/
PUBLIC uint32 AUDIO_SM_GetFileSize(
    HAUDIO hAudioHandle
    );

/*****************************************************************************/
//! \fn PUBLIC SFS_HANDLE AUDIO_SM_GetFileHandle(
//!    HAUDIO hAudioHandle
//!    )
//! \param hAudioHandle The audio handle.
//! \return return the file handle. If failed, return INVALID_HANDLE.
//! \brief  Description: This function is used to get audio's file handle.
//! \author Author: Benjamin.Wang
//! \note   Note: None.       
/*****************************************************************************/
PUBLIC SFS_HANDLE AUDIO_SM_GetFileHandle(
    HAUDIO hAudioHandle
    );
 
/*****************************************************************************/
//! \fn PUBLIC uint32 AUDIO_SM_GetOutputSampleRate(
//!    HAUDIO hAudioHandle
//!    )
//! \param hAudioHandle The audio handle.
//! \return return the samplerate. If failed, return zero.
//! \brief  Description: This function is used to get audio's samplerate. This samplerate is 
//!                             the actual playing samplerate. If you want to get the samplerate 
//!                             recorded in audio files, you should call AUDIO_SM_IdentifyAudioFormat
//!                              to find it.
//! \author Author: Benjamin.Wang
//! \note   Note: None.       
/*****************************************************************************/
PUBLIC uint32 AUDIO_SM_GetOutputSampleRate(
    HAUDIO hAudioHandle
    );
    
/*****************************************************************************/
//! \fn PUBLIC AUDIO_RESULT_E AUDIO_SM_SetOutputSampleRate(
//!    HAUDIO hAudioHandle, 
//!    uint32 uiSamplerate
//!    )
//! \param hAudioHandle The audio handle.
//! \param uiSamplerate The output samplerate. 0 is the default samplerate.
//! \return return the samplerate. If failed, return zero.
//! \brief  Description: This function is used to set audio's output samplerate. 
//!                     This samplerate is the actual playing samplerate.
//! \author Author: Benjamin.Wang
//! \note   Note: None.       
/*****************************************************************************/
PUBLIC AUDIO_RESULT_E AUDIO_SM_SetOutputSampleRate(
    HAUDIO hAudioHandle, 
    uint32 uiSamplerate
    );

/*****************************************************************************/
//! \fn PUBLIC AUDIO_RESULT_E AUDIO_SM_VerifyHandle(
//!    uint32 uiHandle, 
//!    uint32 uiHandleType, 
//!    uint32 uiSubHandleType
//!    )
//! \param uiHandle The handle will be checked.
//! \param uiHandleType The main handle type you want to make sure.
//! \param uiHandleType The sub handle type you want to make sure.
//! \return return operation results. \sa AUDIO_RESULT_E
//! \brief  Description: This function is used to verify the validity of specified handle.
//! \author Author: Benjamin.Wang
//! \note   Note: None.       
/*****************************************************************************/
PUBLIC AUDIO_RESULT_E AUDIO_SM_VerifyHandle(
    uint32 uiHandle, 
    uint32 uiHandleType, 
    uint32 uiSubHandleType
    );

/*****************************************************************************/
//! \fn PUBLIC uint32 AUDIO_SM_GetModuleType(
//!    uint32 uiHandle
//!    )
//! \param uiHandle The handle will be operated.
//! \return return handle's modult type. If the type is 0, the input handle is an invalid handle.
//! \brief  Description: This function is used to pick-up handle's module type.
//! \author Author: Benjamin.Wang
//! \note   Note: None.       
/*****************************************************************************/
PUBLIC uint32 AUDIO_SM_GetModuleType(
    uint32 uiHandle
    );

/*****************************************************************************/
//! \fn PUBLIC uint32 AUDIO_SM_GetHandleType(
//!    uint32 uiHandle
//!    )
//! \param uiHandle The handle will be operated.
//! \return return handle's main handle type. If the type is 0, the input handle is an invalid handle.
//! \brief  Description: This function is used to pick-up handle's main handle type.
//! \author Author: Benjamin.Wang
//! \note   Note: None.       
/*****************************************************************************/
PUBLIC uint32 AUDIO_SM_GetHandleType(
    uint32 uiHandle
    );

/*****************************************************************************/
//! \fn PUBLIC uint32 AUDIO_SM_GetSubHandleType(
//!    uint32 uiHandle
//!    )
//! \param uiHandle The handle will be operated.
//! \return return handle's sub handle type. If the type is 0, the input handle is an invalid handle.
//! \brief  Description: This function is used to pick-up handle's sub handle type.
//! \author Author: Benjamin.Wang
//! \note   Note: None.       
/*****************************************************************************/
PUBLIC uint32 AUDIO_SM_GetSubHandleType(
    uint32 uiHandle
    );

/*****************************************************************************/
//! \fn PUBLIC HAUDIO AUDIO_SM_CreateRecordFileHandle(
//!    HAUDIOCODEC hAudioCodec, 
//!    const uint16* pusCodecName, 
//!    HAUDIODEV hDevice, 
//!    const uint16* pusDevName, 
//!    const int16* psFileName,
//!    SFS_HANDLE hFileHandle,
//!    uint32* puiBuffer, 
//!    uint32 uiBufferSize,   
//!    AUDIO_NOTIFY_CALLBACK_PFUNC pNotifyCallbackFunc,
//!    BLOCK_ID idSender,
//!    BOOLEAN bIsFileExisted
//!    )
//! \param hAudioCodec the codec handle which will be used.
//! \param pusCodecName the codec name which will be used.
//! \param hDevice the device handle which will be used.
//! \param pusDevName the device name which will be used.
//! \param psFileName the file's name.
//! \param hFileHandle the file's handle,not null if file is exited.
//! \param puiBuffer buffer used in recording.
//!                   This buffer is created by caller but managed by record codec.
//! \param uiBufferSize the track buffer size, unit:double word(32bit)
//! \param pNotifyCallbackFunc callback func when as notify mmi or other audio_api caller
//! \param idSender caller id.
//! \param bIsFileExisted file exited flag.
//! \return SUCCESS - return the audio handle created. \par
//!            FAILURE - return INVALID_HANDLE
//! \brief  Description: This function is used to create a record handle which is based on FFS. 
//! \author Author: Benjamin.Wang
//! \note   Note: None     
/*****************************************************************************/
PUBLIC HAUDIO AUDIO_SM_CreateRecordFileHandle(
    HAUDIOCODEC hAudioCodec, 
    const uint16* pusCodecName, 
    HAUDIODEV hDevice, 
    const uint16* pusDevName, 
    const int16* psFileName,
    SFS_HANDLE hFileHandle,
    uint32* puiBuffer,     // buffer used in recording.
                                            // This buffer is created by caller but managed by record codec.
    uint32 uiBufferSize,    // the track buffer size, unit:double word(32bit)
    AUDIO_NOTIFY_CALLBACK_PFUNC pNotifyCallbackFunc,
    BLOCK_ID idSender,
    BOOLEAN bIsFileExisted
    );
/*****************************************************************************/
//! \fn PUBLIC HAUDIO AUDIO_SM_CreateRecordBufferHandle(
//!    HAUDIOCODEC hAudioCodec, 
//!    const uint16* pusCodecName, 
//!    HAUDIODEV hDevice, 
//!    const uint16* pusDevName, 
//!    const uint8* pucData, 
//!    uint32 uiDataLength, 
//!    uint32* puiBuffer, 
//!    uint32 uiBufferSize,   
//!    AUDIO_NOTIFY_CALLBACK_PFUNC pNotifyCallbackFunc,
//!    BLOCK_ID idSender
//!    )
//! \param hAudioCodec the codec handle which will be used.
//! \param pusCodecName the codec name which will be used.
//! \param hDevice the device handle which will be used.
//! \param pusDevName the device name which will be used.
//! \param pucData Data buffer pointer which is used to save the record data.
//! \param uiDataLength The buffer size. It is the max record data size.
//! \param puiBuffer buffer used in recording.
//!                   This buffer is created by caller but managed by record codec.
//! \param uiBufferSize the track buffer size, unit:double word(32bit)
//! \param pNotifyCallbackFunc callback func when as notify mmi or other audio_api caller
//! \param idSender caller id.
//! \return SUCCESS - return the audio handle created. \par
//!            FAILURE - return INVALID_HANDLE
//! \brief  Description: This function is used to create a record handle which is based on 
//!                             data buffer. 
//! \author Author: Benjamin.Wang
//! \note   Note: None     
/*****************************************************************************/
PUBLIC HAUDIO AUDIO_SM_CreateRecordBufferHandle(
    HAUDIOCODEC hAudioCodec, 
    const uint16* pusCodecName, 
    HAUDIODEV hDevice, 
    const uint16* pusDevName, 
    const uint8* pucData, 
    uint32 uiDataLength,
    uint32* puiBuffer,     // buffer used in recording.
                                            // This buffer is created by caller but managed by record codec.
    uint32 uiBufferSize,    // the track buffer size, unit:double word(32bit)
    AUDIO_NOTIFY_CALLBACK_PFUNC pNotifyCallbackFunc,
    BLOCK_ID idSender
    );

/*****************************************************************************/
//! \fn PUBLIC HAUDIO AUDIO_SM_CreateNormalHandle(
//!    HAUDIOCODEC hAudioCodec, 
//!    const uint16* pusCodecName, 
//!    HAUDIODEV hDevice, 
//!    const uint16* pusDevName,
//!    AUDIO_NOTIFY_CALLBACK_PFUNC pNotifyCallbackFunc,
//!    BLOCK_ID idSender
//!    )
//! \param hAudioCodec the codec handle which will be used.
//! \param pusCodecName the codec name which will be used.
//! \param hDevice the device handle which will be used.
//! \param pusDevName the device name which will be used.
//! \param pNotifyCallbackFunc callback func when as notify mmi or other audio_api caller
//! \param idSender caller id.
//! \return SUCCESS - return the audio handle created. \par
//!            FAILURE - return INVALID_HANDLE
//! \brief  Description: This function is used to create a normal handle. This kind of handle 
//!                             is used to play control a source audio such as voice and tone, etc.
//! \author Author: Benjamin.Wang
//! \note   Note: None     
/*****************************************************************************/
PUBLIC HAUDIO AUDIO_SM_CreateNormalHandle(
    HAUDIOCODEC hAudioCodec, 
    const uint16* pusCodecName, 
    HAUDIODEV hDevice, 
    const uint16* pusDevName,
    AUDIO_NOTIFY_CALLBACK_PFUNC pNotifyCallbackFunc,
    BLOCK_ID idSender
    );

/*****************************************************************************/
//! \fn PUBLIC AUDIO_RESULT_E AUDIO_SM_StartRecord(
//!    HAUDIO hAudioHandle
//!    )
//! \param hAudioHandle the audio handle.
//! \return return operation result. \sa AUDIO_RESULT_E
//! \brief  Description: This function is used to start a record operation.
//! \author Author: Benjamin.Wang
//! \note   Note: None.       
/*****************************************************************************/
PUBLIC AUDIO_RESULT_E AUDIO_SM_StartRecord(
    HAUDIO hAudioHandle
    );

/*****************************************************************************/
//! \fn PUBLIC AUDIO_RESULT_E AUDIO_SM_StopRecord(
//!    HAUDIO hAudioHandle,
//!    BOOLEAN b_syn_flag
//!    )
//! \param hAudioHandle the audio handle.
//! \param b_syn_flag if this func is synchronous. TRUE:syn, FALSE:asyn.
//! \return return operation result. \sa AUDIO_RESULT_E
//! \brief  Description: This function is used to stop a record operation.
//! \author Author: Benjamin.Wang
//! \note   Note: None.       
/*****************************************************************************/
PUBLIC AUDIO_RESULT_E AUDIO_SM_StopRecord(
    HAUDIO hAudioHandle,
    BOOLEAN b_syn_flag
    );

/*****************************************************************************/
//! \fn PUBLIC AUDIO_RESULT_E AUDIO_SM_PauseRecord(
//!    HAUDIO hAudioHandle,
//!    BOOLEAN b_syn_flag
//!    )
//! \param hAudioHandle the audio handle.
//! \param b_syn_flag if this func is synchronous. TRUE:syn, FALSE:asyn.
//! \return return operation result. \sa AUDIO_RESULT_E
//! \brief  Description: This function is used to pause a record operation.
//! \author Author: Benjamin.Wang
//! \note   Note: None.       
/*****************************************************************************/
PUBLIC AUDIO_RESULT_E AUDIO_SM_PauseRecord(
    HAUDIO hAudioHandle,
    BOOLEAN b_syn_flag    
    );

/*****************************************************************************/
//! \fn PUBLIC AUDIO_RESULT_E AUDIO_SM_ResumeRecord(
//!    HAUDIO hAudioHandle
//!    )
//! \param hAudioHandle the audio handle.
//! \return return operation result. \sa AUDIO_RESULT_E
//! \brief  Description: This function is used to resume a record operation.
//! \author Author: Benjamin.Wang
//! \note   Note: None.       
/*****************************************************************************/
PUBLIC AUDIO_RESULT_E AUDIO_SM_ResumeRecord(
    HAUDIO hAudioHandle
    );

/*****************************************************************************/
//! \fn PUBLIC AUDIO_RESULT_E AUDIO_SM_SwitchDevice(
//!    HAUDIO hAudioHandle, 
//!    HAUDIODEV hDevice, 
//!    const uint16* pusDevName, 
//!    )
//! \param hAudioHandle Audio handle.
//! \param hDevice New device's handle.
//! \param pusDevName New device's name.
//! \return The operation result. \sa AUDIO_RESULT_E
//! \brief  Description: This function is used to switch audio's output device.
//! \author Author: Benjamin.Wang
//! \note   Note: None.      
/*****************************************************************************/
PUBLIC AUDIO_RESULT_E AUDIO_SM_SwitchDevice(
    HAUDIO hAudioHandle, 
    HAUDIODEV hDevice, 
    const uint16* pusDevName
    );

/*****************************************************************************/
//! \fn PUBLIC uint32 AUDIO_SM_GetDecoderDataLen(
//!    HAUDIO hAudioHandle 
//!    )
//! \param hAudioHandle Audio handle.
//! \return The operation result. \sa uint32
//! \brief  Description: This function is used to get the length of the data
//!           decode but not output.
//! \author Author: Benjamin.Wang
//! \note   Note: None.      
/*****************************************************************************/
PUBLIC uint32 AUDIO_SM_GetDecoderDataLen(
    HAUDIO hAudioHandle
    );

/*****************************************************************************/
//! \fn PUBLIC uint32 AUDIO_SM_GetDecoderDataTime(
//!    HAUDIO hAudioHandle 
//!    )
//! \param hAudioHandle Audio handle.
//! \return The operation result. \sa uint32
//! \brief  Description: This function is used to get the play_time of the data
//!           decode but not output. unit:ms.
//! \author Author: Benjamin.Wang
//! \note   Note: None.      
/*****************************************************************************/
PUBLIC uint32 AUDIO_SM_GetDecoderDataTime(
    HAUDIO hAudioHandle
    );
    
/*****************************************************************************/
//! \fn PUBLIC AUDIO_RESULT_E AUDIO_SM_GetEvent(BLOCK_ID thread_id)
//! \param thread_id 
//! \return return operation result \sa AUDIO_RESULT_E
//! \brief  Description:  This function is used to get an audio task's event.
//! \author Author:  Benjamin.Wang
//! \note   Note:Nothing.  
/*****************************************************************************/
PUBLIC AUDIO_RESULT_E AUDIO_SM_GetEvent(BLOCK_ID thread_id);

/*****************************************************************************/
//! \fn PUBLIC AUDIO_RESULT_E AUDIO_SM_SetEvent(BLOCK_ID thread_id)	
//! \param thread_id 
//! \return return operation result \sa AUDIO_RESULT_E
//! \brief  Description:  This function is used to set an audio task's event.
//! \author Author:  Benjamin.Wang
//! \note   Note:Nothing.  
/*****************************************************************************/
PUBLIC AUDIO_RESULT_E AUDIO_SM_SetEvent(BLOCK_ID thread_id);

/*****************************************************************************/
//! \fn PUBLIC AUDIO_RESULT_E AUDIO_SM_PlayEnd(
//!    HAUDIO hAudioHandle
//!    AUDIO_RESULT_E e_result
//!    )
//! \param hAudioHandle the audio handle.
//! \param e_result operation result.
//! \return return operation result. \sa AUDIO_RESULT_E
//! \brief  Description: This function is used to notify audio service that codec stop 
//!                    operation is over  or codec_play is over when codec has own task.
//! \author Author: Shujing.Dong
//! \note   Note: None.       
/*****************************************************************************/
PUBLIC AUDIO_RESULT_E AUDIO_SM_PlayEnd(
	HAUDIO hAudioHandle, 
	AUDIO_RESULT_E e_result
	);

/*****************************************************************************/
//! \fn PUBLIC AUDIO_RESULT_E AUDIO_SM_PauseEnd(
//!    HAUDIO hAudioHandle
//!    AUDIO_RESULT_E e_result
//!    )
//! \param hAudioHandle the audio handle.
//! \param e_result operation result.
//! \return return operation result. \sa AUDIO_RESULT_E
//! \brief  Description: This function is used to notify audio service 
//!                    that codec pause operation is over when codec has own task.
//! \author Author: Shujing.Dong
//! \note   Note: None.       
/*****************************************************************************/
PUBLIC AUDIO_RESULT_E AUDIO_SM_PauseEnd(
	HAUDIO hAudioHandle, 
	AUDIO_RESULT_E e_result
	);

/*****************************************************************************/
//! \fn PUBLIC AUDIO_RESULT_E AUDIO_SM_SendErrorRep(
//!    HAUDIO hAudioHandle
//!    AUDIO_RESULT_E e_result
//!    )
//! \param hAudioHandle the audio handle.
//! \param e_result note which unconventionality.
//! \return return operation result. \sa AUDIO_RESULT_E
//! \brief  Description: This function is used to notify audio service that some   
//!           unconventionality occur in codec task. 
//! \author Author: Shujing.Dong
//! \note   Note: None.       
/*****************************************************************************/
PUBLIC AUDIO_RESULT_E AUDIO_SM_SendErrorRep(
	HAUDIO hAudioHandle, 
	AUDIO_RESULT_E e_result
	); 

/*****************************************************************************/
//! \fn PUBLIC AUDIO_OBJECT_T * AUDIO_SM_GetAudioObjectOfLastPlay(
//!    void
//!    )
//! \param void.
//! \return return the pointer of audio object try to play at the last.
//!            \sa AUDIO_OBJECT_T* 
//! \brief  Description: This function is used to the pointer of audio object 
//!            try to play at the last 
//! \author Author: Shujing.Dong
//! \note   Note: None.       
/*****************************************************************************/
PUBLIC AUDIO_OBJECT_T * AUDIO_SM_GetAudioObjectOfLastPlay(
	void
	);
	
/*****************************************************************************/
//! \fn PUBLIC AUDIO_RESULT_E AUDIO_SM_InitExpPara(
//!    AUDIO_OBJECT_T *audio_obj
//!    )
//! \param hAudioHandle the audio handle.
//! \return return operation results. \sa AUDIO_RESULT_E
//! \brief  Description: This function is used to init param of all express 
//!      included in codec of audio object.
//! \author Author: Shujing.Dong
//! \note   Note: None     
/*****************************************************************************/
PUBLIC AUDIO_RESULT_E AUDIO_SM_InitExpPara(
    AUDIO_OBJECT_T *audio_obj
    );

/*****************************************************************************/
//! \fn PUBLIC AUDIO_RESULT_E AUDIO_SM_DeInitExpPara(
//!    AUDIO_OBJECT_T *audio_obj
//!    )
//! \param hAudioHandle the audio handle.
//! \return return operation results. \sa AUDIO_RESULT_E
//! \brief  Description: This function is used to deinit param of all express 
//!      included in codec of audio object.
//! \author Author: Binggo.Zhou
//! \note   Note: None     
/*****************************************************************************/
PUBLIC AUDIO_RESULT_E AUDIO_SM_DeInitExpPara(
    AUDIO_OBJECT_T *audio_obj
    );    
    
/*****************************************************************************/
//! \fn PUBLIC uint32 AUDIO_SM_GetRemainedPcmCount(HAUDIO hAudioHandle)
//! \param hAudioHandle the audio handle.
//! \return return  pcm count in byte unit
//! \brief  Description: This function is used to get the count of pcm data remained in the device
//!          buffer in byte unit
//! \author Author: peter.huang
//! \note   Note: None     
/*****************************************************************************/
PUBLIC uint32 AUDIO_SM_GetRemainedPcmCount(HAUDIO hAudioHandle);

/*****************************************************************************/
//! \fn PUBLIC uint32 AUDIO_SM_GetRemainedPcmTime(HAUDIO hAudioHandle)
//! \param hAudioHandle the audio handle.
//! \return return  play time of pcm count. unit:ms.
//! \brief  Description: This function is used to get the play time of pcm data 
//!         remained in the device buffer.
//! \author Author: shujing.dong
//! \note   Note: None     
/*****************************************************************************/
PUBLIC uint32 AUDIO_SM_GetRemainedPcmTime(HAUDIO hAudioHandle);

/*****************************************************************************/
//! \fn PUBLIC uint8 * AUDIO_SM_AllocMemory(uint32 ui_len)
//!    uint32 ui_alloc_len
//!    )
//! \param ui_alloc_len length to alloc, unit:bytes.
//! \return addaress of space. 
 //! \brief  Description: This function is used to alloc space used in codec.
//! \author Author: Shujing.Dong
//! \note   Note: None     
/*****************************************************************************/
//PUBLIC uint8 * AUDIO_SM_AllocMemory(uint32 ui_alloc_len);

 /*****************************************************************************/
//! \fn PUBLIC AUDIO_RESULT_E AUDIO_SM_ChangeCodecPriority(
//!    HAUDIO hAudioHandle
//!    BOOLEAN bHighPriority
//!    )
//! \param hAudioHandle The handle which will be operated.
//! \param bHighPriority   The high or low priority which will be set for codec task.
//! \return process result. \sa AUDIO_RESULT_E
//! \brief  Description: This function is used to change codec task prority 
//! \                           between ext defined low or high priority.  
//! \author Author: Binggo.Zhou
//! \note   Note: None. 
/*****************************************************************************/
PUBLIC AUDIO_RESULT_E AUDIO_SM_ChangeCodecPriority(
    HAUDIO hAudioHandle,
    BOOLEAN bHighPriority
    );

/*****************************************************************************/
//! \fn PUBLIC AUDIO_RESULT_E AUDIO_SM_GetCodecPriorityBeHigh(
//!    HAUDIO hAudioHandle
//!    BOOLEAN bHighPriority
//!    )
//! \param hAudioHandle The handle which will be operated.
//! \param bHighPriority   The high or low priority which will be set for codec task.
//! \return process result. \sa AUDIO_RESULT_E
//! \brief  Description: This function is used to change codec task prority 
//! \                           between ext defined low or high priority.  
//! \author Author: Binggo.Zhou
//! \note   Note: None. 
/*****************************************************************************/
PUBLIC AUDIO_RESULT_E AUDIO_SM_GetCodecPriorityBeHigh(
    HAUDIO hAudioHandle,
    BOOLEAN * bHighPriorityptr
    );

/*****************************************************************************/
//! \fn PUBLIC AUDIO_RESULT_E AUDIO_SM_PauseOutputData(
//!    HAUDIO hAudioHandle
//!    )
//! \param hAudioHandle The handle which will be operated.
//! \param bHighPriority   The high or low priority which will be set for codec task.
//! \return process result. \sa AUDIO_RESULT_E
//! \brief  Description: This function is used to change codec task prority 
//! \                           between ext defined low or high priority.  
//! \author Author: Binggo.Zhou
//! \note   Note: None. 
/*****************************************************************************/
PUBLIC AUDIO_RESULT_E AUDIO_SM_PauseOutputData(
    AUDIO_OBJECT_T* audio_obj
    );

/*****************************************************************************/
//! \fn PUBLIC AUDIO_RESULT_E AUDIO_SM_GetDebugOutputDataFileCount(
//!    HAUDIO hAudioHandle,
//!    uint16 *pusFileCount
//!    )
//! \param hAudioHandle The handle which will be operated.
//! \param pusFileCount  Debug output file count, it is the increase of count of express of codec and one.
//! \return process result. \sa AUDIO_RESULT_E
//! \brief  Description: This function is used to get file count to store output debug data  
//!              after decoding or processing of every express plugin.
//! \author Author: Shujing.Dong
//! \note   Note: None. 
/*****************************************************************************/
PUBLIC AUDIO_RESULT_E AUDIO_SM_GetDebugOutputDataFileCount(
    HAUDIO hAudioHandle,
    uint16 *pusFileCount
    );
    
/*****************************************************************************/
//! \fn PUBLIC AUDIO_RESULT_E AUDIO_SM_EnableSomePosDebugOutputDataToFile(
//!    HAUDIO hAudioHandle,
//!    BOOLEAN bEnableDebugDataOutput,
//!    uint16 usPos
//!    )
//! \param hAudioHandle The handle which will be operated.
//! \param bEnableDebugDataOutput   SCI_TRUE:enable, SCI_FALSE:disable.
//! \param usPos enable or disable output debug data in some postion: 
//!                           0-after decoding, 
//!                           1-after the processing of the first express,
//!                           2-after the processing of the second express
//!                           3-...
//! \return process result. \sa AUDIO_RESULT_E
//! \brief  Description: This function is used to Enable output debug data  
//!              after decoding or processing of every express plugin to file.
//! \author Author: Shujing.Dong
//! \note   Note: None. 
/*****************************************************************************/
PUBLIC AUDIO_RESULT_E AUDIO_SM_EnableSomePosDebugOutputDataToFile(
    HAUDIO hAudioHandle,
    BOOLEAN bEnableDebugDataOutput,
    uint16 usPos
    );
    
/*****************************************************************************/
//! \fn PUBLIC AUDIO_RESULT_E AUDIO_SM_EnableOutputDataToFile(
//!    HAUDIO hAudioHandle
//!    BOOLEAN bEnableDebugDataOutput
//!    )
//! \param hAudioHandle The handle which will be operated.
//! \param bEnableDebugDataOutput   SCI_TRUE:enable, SCI_FALSE:disable.
//! \return process result. \sa AUDIO_RESULT_E
//! \brief  Description: This function is used to Enable output debug data  
//!              after decoding or processing of every express plugin to file.
//! \author Author: Shujing.Dong
//! \note   Note: None. 
/*****************************************************************************/
PUBLIC AUDIO_RESULT_E AUDIO_SM_EnableOutputDataToFile(
    HAUDIO hAudioHandle,
    BOOLEAN bEnableDebugDataOutput
    );
    
/*****************************************************************************/
//! \fn PUBLIC AUDIO_RESULT_E AUDIO_SM_OfferDebugDataFileName(
//!    HAUDIO hAudioHandle
//!    uint16 *pusDebugDataFileName
//!    )
//! \param hAudioHandle The handle which will be operated.
//! \param pusDebugDataFileName   file name.
//! \return process result. \sa AUDIO_RESULT_E
//! \brief  Description: This function is used to set file name to store data
//!              after decoding or processing of every express plugin.
//! \author Author: Shujing.Dong
//! \note   Note: None. 
/*****************************************************************************/
PUBLIC AUDIO_RESULT_E AUDIO_SM_OfferDebugDataFileName(
    HAUDIO hAudioHandle,
    uint16 *pusDebugDataFileName
    );   

/*****************************************************************************/
//! \fn PUBLIC BOOLEAN AUDIO_SM_IfExpWorking(
//!    AUDIO_OBJECT_T *ptAudioObject,
//!    HAUDIOEXP hExp
//!    )
//! \param ptAudioObject 
//! \param hExp   
//! \return process result. \sa AUDIO_RESULT_E
//! \brief  Description: check if some exp is working in the audio object.
//! \author Author: Shujing.Dong
//! \note   Note: None. 
/*****************************************************************************/
PUBLIC BOOLEAN AUDIO_SM_IfExpWorking(
	AUDIO_OBJECT_T *ptAudioObject,
    HAUDIOEXP hExp
    ); 

/*****************************************************************************/
//! \fn PUBLIC AUDIO_RESULT_E AUDIO_SM_ConfigIfExpWorking(
//!    AUDIO_OBJECT_T *audio_obj,
//!    HAUDIODEV hDev
//!    )
//! \param audio_obj 
//! \param hDev
//! \return process result. \sa AUDIO_RESULT_E
//! \brief  Description: config: if some exp is working in the audio object for the dev.
//! \author Author: Shujing.Dong
//! \note   Note: None. 
/*****************************************************************************/
PUBLIC AUDIO_RESULT_E AUDIO_SM_ConfigIfExpWorking(
	AUDIO_OBJECT_T *audio_obj,
	HAUDIODEV hDev
    );    

/*****************************************************************************/
//! \fn PUBLIC AUDIO_RESULT_E AUDIO_SM_StopSelf(
//!    AUDIO_OBJECT_T* audio_obj
//!    )
//! \param audio_obj 
//! \param pusDebugDataFileName   file name.
//! \return process result. \sa AUDIO_RESULT_E
//! \note   Note: None. 
/*****************************************************************************/
PUBLIC AUDIO_RESULT_E AUDIO_SM_StopSelf(AUDIO_OBJECT_T* audio_obj);    


PUBLIC AUDIO_RESULT_E AUDIO_SM_SetExpInitialized(
	AUDIO_OBJECT_T *audio_obj,
	HAUDIOEXP hExp,
	BOOLEAN bInitialized
    );

PUBLIC AUDIO_RESULT_E AUDIO_SM_IsExpInitialized(
	AUDIO_OBJECT_T *audio_obj,
	HAUDIOEXP hExp,
	BOOLEAN *pbInitialized
    );    
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/    
#ifdef __cplusplus
}
#endif

#endif  // _ASM_H

// End of asm.h

