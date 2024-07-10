#if 0
/******************************************************************************
 ** File Name:      audio_api.h                                                *
 ** Author:         Benjamin.Wang                                             *
 ** DATE:           11/11/2005                                                *
 ** Copyright:      2005 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the basic operation interfaces and data structure of Audio *
 **                     Service.                                                        *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 11/11/2005     Benjamin.Wang     Create.                                  *
 ******************************************************************************/

 /*! \file audio_api.h
  *  \author Benjamin Wang
  *  \date Nov 11, 2005
  *  \brief This file defines the basic operation interfaces and data structure of Audio Service. 
  */
  
#ifndef _AUDIO_API_H
#define _AUDIO_API_H

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "os_api.h"
#include "sci_service.h"
#include "mmifmm_export.h"
//#include "ffs.h"
#include "audio_config.h"
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**---------------------------------------------------------------------------*
 **                         MACRO Definations                                     *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/
 /*! \def INVALID_HANDLE 
 *  \brief Reserved to express the invalid handle.*/
#define INVALID_HANDLE (0)

/*! \def AUDIO_FORMAT_INFO_LEN 
 *  \brief length of string to describe format info, e.g."IMA-ADPCM" or "mp3PRO".*/
#define AUDIO_FORMAT_INFO_LEN (32)

/*! \typedef HAUDIO
 *  \brief Audio Handle
 *  Audio handle is equivalently equal to music handle.
 */
typedef uint32 HAUDIO;      // audio handle == music handle
/*! \typedef HAUDIOCODEC
 *  \brief Codec Plugger Handle
 */
typedef uint32 HAUDIOCODEC;     // codec plugger handle
/*! \typedef HAUDIOEXP
 *  \brief Expression Plugger Handle
 */
typedef uint32 HAUDIOEXP;   // expression plugger handle
/*! \typedef HCOLLISION
 *  \brief Collision Handle ID
 */
typedef uint32 HCOLLISION;  // collision handle ID
/*! \typedef HCOLLISIONCLASS
 *  \brief Collision Class ID
 *
 *  We regard it as a kind of handle.
 *  \sa AUDIO_CM_CLASS_T
 */
typedef uint32 HCOLLISIONCLASS; // class ID. We think it as HANDLE.
typedef uint32 HAUDIOEXP;   // expression plugger handle
/*! \typedef HAUDIODEV
 *  \brief Audio Device Handle ID
 */
typedef uint32 HAUDIODEV;

//count of param to set in recording
#define RECORD_PARAM_SET_COUNT 5
#define RECORD_SET_ADDR_COUNT 2
#define RECORD_GET_RECORD_TIME_COUNT 1

//EXT_OPE_NAME: ANSI2UINT16("SET_RECORDADDR");
//note: this para is need in normal recording, not used in recording dsp test data
typedef struct
{
    uint8 *uc_record_buffer_ptr;// addr pointer of buffer
    uint32 ui_record_buffer_len; // buffer length , min:32000(byte)
}RECORD_BUFFER_INFO_T;



/*! \enum EQ_MODE_E
 *  \brief eq mode type. PASS is last one.
 */
typedef enum
{    
    BASS    = 0,
    TREBLE  = 1,
    VOCAL   = 2,
    ROCK    = 3,
    POP     = 4,
    CLASSIC = 5,
    POWER   = 6,
    PASS    = 7,
    MODE_MAX = 0xFF  
}EQ_MODE_E;

typedef enum
{
    S_8000  = 0,  //8000hz
    S_11025 = 1,   //11025
    S_12000 = 2,   //12000
    S_16000 = 3,   //16000
    S_22050 = 4,   //22050
    S_24000 = 5,   //24000
    S_32000 = 6,   //32000
    S_44100 = 7,   //44100
    S_48000 = 8   //48000  
}EQ_SAMPLERATE_E;

/*! \typedef AUDIO_CALLBACK_PFUNC
 *  \brief Audio callback function prototype. This function is used to support stream media.
 */
typedef AUDIO_RESULT_E (*AUDIO_CALLBACK_PFUNC)(HAUDIO hAudio, uint8** ppucDataAddr, uint32* puiDataLength);

#define AUDIO_CM_OPERATION_DONE AUDIO_NO_ERROR


/*! \enum AUDIO_RESULT_E
 *  \brief Audio operation result list.
 */
typedef enum
{
    AUDIO_NO_ERROR = SCI_SUCCESS,       /*!<Operation success.*/
    AUDIO_LINKLIST_NULL,    /*!<The link list is null.*/
    AUDIO_UNOPENED_HANDLE,  /*!<This handle has not been opened.*/
    AUDIO_PLAYING,  /*!<This audio is being played.*/
    AUDIO_PAUSING, /*!<This audio has been paused.*/
    AUDIO_NOT_PAUSING, /*!<This audio has not been paused.*/
    AUDIO_HANDLE_IS_INVALID,    /*!<The handle is invalid.*/
    AUDIO_HANDLE_NOT_EXIST,     /*!<This handle is not exist.*/
    AUDIO_FILE_ERROR,   /*!<The file operation is error.*/
    AUDIO_FORMAT_ERROR, /*!<Audio format can't be supported.*/
    AUDIO_MULTI_STOP,   /*!<The destination audio has been stopped.*/
    AUDIO_ERROR_MODULE_HANDLE,  /*!<This handle's module type is unconformity.*/
    AUDIO_ERROR_HANDLE_TYPE,    /*!<This handle's handle type is unconformity.*/
    AUDIO_ERROR_SUB_HANDLE_TYPE,    /*!<This handle's sub-handle typeis unconformity.*/
    AUDIO_NO_ENOUGH_MEMORY,     /*!<System can't provide enough memory space.*/
    AUDIO_CONFLICT,     /*!<Audio play conflict.*/
    AUDIO_NO_OPERATION,     /*!<Don't exist this operation.*/
    AUDIO_NO_CODECHANDLE,   /*!<Codec handle is not exist.*/
    AUDIO_VB_TRACK_FULL,    /*!<Voiceband track number is full.*/
    AUDIO_PARAM_ERROR,  /*!<Parameter error.*/
    AUDIO_CM_OPERATION_FAILURE, /*!<Collision management operation failure.*/
    AUDIO_CM_CAN_PLAY,  /*!<Collision arbitration pass. */
    AUDIO_CM_CANNOT_PLAY,   /*!<Collision arbitration reject.*/
    AUDIO_CM_PARA_ERROR,    /*!<Collision management operation parameter error.*/
    AUDIO_PM_NO_EXT_OPE_SYMBOL, /*!<This extend operation symbol is not exist.*/
    AUDIO_NO_EXPHANDLE,    /*!<This handle is not exist.*/
    AUDIO_INVALID_GAIN,     /*!<The digital gain from codec is invalid.*/
    AUDIO_INVALID_SAMPLERATE, /*!<The samplerate from codec is invalid.*/
    AUDIO_SEEK_ERROR,/*!<The seek operation in codec is wrong.*/
    
    AUDIO_ERROR = SCI_ERROR /*!<General ERROR.*/
}AUDIO_RESULT_E;

/*! \enum AUDIO_SUB_TYPE_E
 *  \brief Audio sub type list.
 */
typedef enum
{
    AUDIO_FILE_HANDLE = 1,                   //=1, Audio handle sub type which is based on file system. 
    AUDIO_BUFFER_HANDLE,                   //=2, Audio handle sub type which is based on data buffer.
    AUDIO_STREAM_BUFFER_HANDLE,    //=3, Audio handle sub type which is based on stream data.
    AUDIO_RECORD_FILE_HANDLE    ,          //=4, Record handle sub type which is based on file system.
    AUDIO_RECORD_BUFFER_HANDLE,    //=5, Record handle sub type which is based on data buffer.
    AUDIO_NORMAL_HANDLE,                   //=6, Audio handle sub type which is based on no source audio player(codec).
    AUDIO_SUB_TYPE_ERROR
}AUDIO_SUB_TYPE_E;

/*! \enum AUDIO_STREAM_GET_SRC_RESULT_E
 *  \brief Audio get stream src data operation result list.
 */
typedef enum
{
    AUDIO_STREAM_GET_SRC_SUCCESS,   
    AUDIO_STREAM_GET_SRC_WAIT,
    AUDIO_STREAM_GET_SRC_ASY,
    AUDIO_STREAM_GET_SRC_ERROR
}AUDIO_STREAM_GET_SRC_RESULT_E;

/*! \enum AUDIO_DEVICE_MODE_TYPE_E
 *  \brief Audio device mode type list.
 */
typedef enum
{
    AUDIO_DEVICE_MODE_HANDHOLD,
    AUDIO_DEVICE_MODE_HANDFREE,
    AUDIO_DEVICE_MODE_EARPHONE,
    AUDIO_DEVICE_MODE_EARFREE,
    AUDIO_DEVICE_MODE_TVOUT,
    AUDIO_DEVICE_MODE_MAX
}AUDIO_DEVICE_MODE_TYPE_E;

/*! \enum AUDIO_CHANNEL_MODE_E
 *  \brief Audio channel mode list.
 */
typedef enum
{
    AUDIO_CHANNEL_MONO      = 0,    // single channel 
    AUDIO_CHANNEL_DUAL_CHANNEL  = 1,    // dual channel 
    AUDIO_CHANNEL_JOINT_STEREO  = 2,    // joint (MS/intensity) stereo 
    AUDIO_CHANNEL_STEREO        = 3     // normal LR stereo 
}AUDIO_CHANNEL_MODE_E;

/*! \enum AUDIO_TRACKBUF_STATUS_E
 *  Indicate Trackbuf Status.
 */
typedef enum
{
    AUDIO_TRACKBUF_EMPTY = 0,
    AUDIO_TRACKBUF_FULL
}AUDIO_TRACKBUF_STATUS_E;


/*! \typedef AUDIO_GETSRCDATA_CALLBACK_PFUNC
 *  \brief Audio get_stream_src_data callback function prototype. This function is used to get stream media.
 */
typedef AUDIO_STREAM_GET_SRC_RESULT_E (*AUDIO_GETSRCDATA_CALLBACK_PFUNC)(
    HAUDIO hAudio, 
    uint8 ** const ppucDataAddr, 
    uint32* puiDataLength,
    int32 offset
);

/*! \typedef AUDIO_GETSRCDATA_CALLBACK_PFUNC
 *  \brief Audio notify function prototype. This function is used to notify event to mmi/app from as.
 */ 
typedef void (*AUDIO_NOTIFY_CALLBACK_PFUNC)(
    HAUDIO hAudio, 
    uint32 notify_info, 
    uint32 affix_info
);

#define AUDIO_CM_OPERATION_DONE AUDIO_NO_ERROR

/*===========================================================*/
/*!AUDIO SERVER EVENTID DEFINITION*/
/*! \enum AUDIO_SVR_EventID_enum
 *  \brief Audio Event ID list.
 *  When event happened, it will be sent to all registered clients.
 */
enum AUDIO_SVR_EventID_enum
{
    AUDIO_EVENT_IND = ((AUDIO_SERVICE << 8) | 1),   /*!<Indication Event ID*/

    AUDIO_MSG_MAX_NUM         /*!<Max Event ID*/
} ;

/*! \struct AUDIO_ID3_TAG_T
 *  \brief audio id3 tag info. structure.
 */
typedef struct AUDIO_tag_id3 
{
    char tag_code_type;            //0:ascii;    1:unicode; 
    char year[5];               //year published    4bytes
    char res0;
    char res1;
    char tag[4];                // "TAG"        3 bytes
    char title[32];             //music title   30bytes
    char artist[32];            //music artist      30bytes
    char album[32];             //music album   30bytes
    char comment[32];           // comment      30bytes
    char genre[32];             //genre     30bytes
    char content[32];
    char track[32];
} AUDIO_ID3_TAG_T;

/*! \struct AUDIO_FORMATINFO_T
 *  \brief Audio File Information.
 */
typedef struct AUDIO_FormatInfo_struct
{
    HAUDIOCODEC hCodecHandle;/*!<get these info by what codec*/
    uint32 uiTotalDataLength;    /*!<File length in bytes*/
    uint32 uiTotalTime; /*!<File length in seconds*/
    uint32 uiSamplerate;    /*!<Audio data's samplerate*/
    uint32 uiChannelNum;    /*!<Audio data's channel number, mono or stereo or some others.*/
    uint32 uiBitrate; /*!<bitrate*/
    uint32 uiLayer; /*!<layer*/
    AUDIO_CHANNEL_MODE_E  eChannelMode; /*!<channel mode*/   
    AUDIO_ID3_TAG_T tAudioId3Tag;/*!<ID3 tag info */
    char ucFormatInfo[AUDIO_FORMAT_INFO_LEN];/*!<string to describe format info, e.g."IMA-ADPCM" or "mp3PRO"*/
    void* pvOtherInfo;  /*!<Extend audio information. It should point to an infor structure defined by codec.*/
}AUDIO_FORMATINFO_T;

/*! \struct AUDIO_PLAYINFO_T
 *  \brief Audio Playing Information.
 */
typedef struct AUDIO_PlayInfo_struct
{
    uint32 uiTotalTime;    /*!< Total audio time.*/
    uint32 uiCurrentTime;   /*!< Current audio played time.*/
    uint32 uiTotalDataLength;   /*!< Total audio data length.*/
    uint32 uiCurrentDataOffset; /*!< Current audio data offset.*/
    void* pvOtherInfo;  /*!< Extend audio playing information. It should point to an infor struc*/
}AUDIO_PLAYINFO_T;

typedef struct
{
    AUD_REC_FORMAT_E            format;//ADPCM OR SOME MODE OF DSP_DATA_TO_TEST
    RECORD_OUTPUT_CHANNEL_E     output_channel;
    uint32                      time_limit;//unit:s,save file when recording time_limit(s)
    uint16                      frame_length;//the length of data from dsp to arm in once time.   
    uint16                      block_count;//the count of block. 
/*[START] Jerry Liang for IQ MODE 2008/07/31(accroding to REF code)*/     
    uint16                      arfcn;
    uint32                      reserved;
/*[END] Jerry Liang for IQ MODE 2008/07/31(accroding to REF code)*/            
}RECORD_PARAM_T;

/*! \enum AUDIO_Priority_enum
 *  \brief Audio priority enum.
 *  0 is the lowest. 15 is the highest.
 */
enum AUDIO_Priority_enum
{
    AUDIO_PRIORITY_0 = 0,   /*!<The lowest priority*/
    AUDIO_PRIORITY_1, 
    AUDIO_PRIORITY_2, 
    AUDIO_PRIORITY_3, 
    AUDIO_PRIORITY_4, 
    AUDIO_PRIORITY_5, 
    AUDIO_PRIORITY_6, 
    AUDIO_PRIORITY_7,   /*!<The middle priority*/
    AUDIO_PRIORITY_8, 
    AUDIO_PRIORITY_9, 
    AUDIO_PRIORITY_10, 
    AUDIO_PRIORITY_11, 
    AUDIO_PRIORITY_12, 
    AUDIO_PRIORITY_13, 
    AUDIO_PRIORITY_14, 
    AUDIO_PRIORITY_15   /*!<The highest priority*/
};

/*! \def AUDIO_PRIORITY_LOWEST
 *  Lowest audio priority.
 */
#define AUDIO_PRIORITY_LOWEST AUDIO_PRIORITY_0
/*! \def AUDIO_PRIORITY_MIDDLE
 *  Middle audio priority.
 */
#define AUDIO_PRIORITY_MIDDLE AUDIO_PRIORITY_7
/*! \def AUDIO_PRIORITY_HIGHEST
 *  Highest audio priority.
 */
#define AUDIO_PRIORITY_HIGHEST AUDIO_PRIORITY_15

/**---------------------------------------------------------------------------*
 **                         Global Variables                                  *
 **---------------------------------------------------------------------------*/

extern HAUDIOCODEC hMIDICodec;
extern HAUDIOCODEC hAACCodec;
extern HAUDIOCODEC hAMRCodec;
extern HAUDIOCODEC hWAVCodec;
extern HAUDIOCODEC hMP3Codec;
#ifdef WMA_SUPPORT
extern HAUDIOCODEC hWMACodec;
#endif
extern HAUDIOCODEC hVoiceCodec;
extern HAUDIOCODEC hCustomCodec;
extern HAUDIOCODEC hDtmfCodec;
extern HAUDIOCODEC hGenericCodec;
extern HAUDIOCODEC hRecordAdpcmCodec;
extern HAUDIOCODEC hDSPCodec;
extern HAUDIOCODEC hAACLCCodec;

extern HAUDIOCODEC hBlueCodec;
extern HAUDIOCODEC hBlueRecordCodec;

extern HAUDIODEV hARMVB;
extern HAUDIODEV hLAYER1;
extern HAUDIODEV hRECORDVB;
extern HAUDIODEV hMP4RECORDVB;
extern HAUDIODEV hDSPDATADEVICE;
extern HAUDIODEV hPCMDev;
extern HAUDIODEV hL1Rec;
extern HAUDIODEV hA2DPDev;

extern HAUDIOEXP hTransSamExp;
extern HAUDIOEXP hDGExp;
extern HAUDIOEXP hEQExp;
extern HAUDIO    hMyRing;

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//! \fn PUBLIC void AUDIO_Init(void)
//! \param void
//! \return void No return.
//! \brief  Description: This Function is used to initialize the audio server.
//! \author Author: Benjamin.Wang
//! \note   Note:Nothing.           
/*****************************************************************************/
PUBLIC void AUDIO_Init(void);

/*****************************************************************************/
//! \fn PUBLIC HAUDIO AUDIO_CreateFileHandle(
//!    HAUDIOCODEC hAudioCodec, 
//!    const uint16* pusCodecName, 
//!    HAUDIODEV hDevice, 
//!    const uint16* pusDevName, 
//!    const int16* psFileName, 
//!    uint32* puiTrackBuffer, 
//!    uint32 uiTrackBufferSize,
//!    AUDIO_NOTIFY_CALLBACK_PFUNC pNotifyCallbackFunc 
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
//! \return SUCCESS - return the audio handle created
//!            FAILURE - return INVALID_HANDLE
//! \brief  Description: This function is used to create an audio handle which is based on FFS. 
//!                             Not all codec support file system.
//! \author Author: Benjamin.Wang
//! \note   Note: This operation is synchronous.         
/*****************************************************************************/
PUBLIC HAUDIO AUDIO_CreateFileHandle(   // SUCCESS - return the audio handle created
                                                                            // FAILURE - return SCI_NULL
    HAUDIOCODEC hAudioCodec,    // the codec handle which will be used.
    const uint16* pusCodecName,     // the codec name which will be used.
    HAUDIODEV hDevice, 
    const uint16* pusDevName,     
    const int16* psFileName, 
    uint32* puiTrackBuffer,     // decode data output buffer.
                                            // This buffer is created by caller but managed by voiceband 
                                            // controller.
    uint32 uiTrackBufferSize,    // the track buffer size.
    AUDIO_NOTIFY_CALLBACK_PFUNC pNotifyCallbackFunc /*!< callback func when as notify mmi or other audio_api caller.*/
    );

/*****************************************************************************/
//! \fn PUBLIC AUDIO_RESULT_E AUDIO_ASYCreateFileHandle(
//!    HAUDIOCODEC hAudioCodec, 
//!    const uint16* pusCodecName, 
//!    HAUDIODEV hDevice, 
//!    const uint16* pusDevName, 
//!    const int16* psFileName, 
//!    uint32* puiTrackBuffer, 
//!    uint32 uiTrackBufferSize,
//!    AUDIO_NOTIFY_CALLBACK_PFUNC pNotifyCallbackFunc 
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
//! \return SUCCESS - return AUDIO_NO_ERROR
//!            FAILURE - return error reason
//! \brief  Description: This function is used to create an audio handle which is based on FFS. 
//!                             Not all codec support file system.
//! \author Author: Benjamin.Wang
//! \note   Note: This operation is asynchronous.         
/*****************************************************************************/
PUBLIC AUDIO_RESULT_E AUDIO_ASYCreateFileHandle(   // SUCCESS - return the audio handle created
                                                                            // FAILURE - return SCI_NULL
    HAUDIOCODEC hAudioCodec,    // the codec handle which will be used.
    const uint16* pusCodecName,     // the codec name which will be used.
    HAUDIODEV hDevice, 
    const uint16* pusDevName,     
    const int16* psFileName, 
    uint32* puiTrackBuffer,     // decode data output buffer.
                                            // This buffer is created by caller but managed by voiceband 
                                            // controller.
    uint32 uiTrackBufferSize,    // the track buffer size.
    AUDIO_NOTIFY_CALLBACK_PFUNC pNotifyCallbackFunc /*!< callback func when as notify mmi or other audio_api caller.*/
    );

/*****************************************************************************/
//! \fn PUBLIC HAUDIO AUDIO_CreateBufferHandle(
//!    HAUDIOCODEC hAudioCodec, 
//!    const uint16* pusCodecName, 
//!    HAUDIODEV hDevice, 
//!    const uint16* pusDevName, 
//!    const uint8* pucData, 
//!    uint32 uiDataLength, 
//!    uint32* puiTrackBuffer, 
//!    uint32 uiTrackBufferSize,
//!    AUDIO_NOTIFY_CALLBACK_PFUNC pNotifyCallbackFunc 
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
//! \return SUCCESS - return the audio handle created
//!            FAILURE - return INVALID_HANDLE
//! \brief  Description: This function is used to create an audio handle which is based on data 
//!                             buffer. Not all codec support buffer controller.
//! \author Author: Benjamin.Wang
//! \note   Note: This operation is synchronous.         
/*****************************************************************************/
PUBLIC HAUDIO AUDIO_CreateBufferHandle( // SUCCESS - return the audio handle created
                                                                            // FAILURE - return SCI_NULL
    HAUDIOCODEC hAudioCodec,    // the codec handle which will be used.
    const uint16* pusCodecName,     // the codec name which will be used.
    HAUDIODEV hDevice, 
    const uint16* pusDevName,     
    const uint8* pucData,   // the music data pointer
    uint32 uiDataLength,    // the music data length
    uint32* puiTrackBuffer,     // decode data output buffer.
                                            // This buffer is created by caller but managed by voiceband 
                                            // controller.
    uint32 uiTrackBufferSize,    // the track buffer size.
    AUDIO_NOTIFY_CALLBACK_PFUNC pNotifyCallbackFunc /*!< callback func when as notify mmi or other audio_api caller.*/
    );

/*****************************************************************************/
//! \fn PUBLIC AUDIO_RESULT_E AUDIO_ASYCreateBufferHandle(
//!    HAUDIOCODEC hAudioCodec, 
//!    const uint16* pusCodecName, 
//!    HAUDIODEV hDevice, 
//!    const uint16* pusDevName, 
//!    const uint8* pucData, 
//!    uint32 uiDataLength, 
//!    uint32* puiTrackBuffer, 
//!    uint32 uiTrackBufferSize,
//!    AUDIO_NOTIFY_CALLBACK_PFUNC pNotifyCallbackFunc 
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
//! \return SUCCESS - return AUDIO_NO_ERROR
//!            FAILURE - return error reason
//! \brief  Description: This function is used to create an audio handle which is based on data 
//!                             buffer. Not all codec support buffer controller.
//! \author Author: Benjamin.Wang
//! \note   Note: This operation is asynchronous.         
/*****************************************************************************/
PUBLIC AUDIO_RESULT_E AUDIO_ASYCreateBufferHandle( // SUCCESS - return the audio handle created
                                                                            // FAILURE - return SCI_NULL
    HAUDIOCODEC hAudioCodec,    // the codec handle which will be used.
    const uint16* pusCodecName,     // the codec name which will be used.
    HAUDIODEV hDevice, 
    const uint16* pusDevName,     
    const uint8* pucData,   // the music data pointer
    uint32 uiDataLength,    // the music data length
    uint32* puiTrackBuffer,     // decode data output buffer.
                                            // This buffer is created by caller but managed by voiceband 
                                            // controller.
    uint32 uiTrackBufferSize,    // the track buffer size.
    AUDIO_NOTIFY_CALLBACK_PFUNC pNotifyCallbackFunc /*!< callback func when as notify mmi or other audio_api caller.*/
    );

/*****************************************************************************/
//! \fn PUBLIC HAUDIO AUDIO_CreateStreamBufferHandle(
//!    HAUDIOCODEC hAudioCodec, 
//!    const uint16* pusCodecName, 
//!    HAUDIODEV hDevice, 
//!    const uint16* pusDevName, 
//!    const uint8* pucData, 
//!    uint32 uiDataLength, 
//!    uint32* puiTrackBuffer, 
//!    uint32 uiTrackBufferSize,
//!    AUDIO_NOTIFY_CALLBACK_PFUNC pNotifyCallbackFunc, 
//!   AUDIO_GETSRCDATA_CALLBACK_PFUNC pGetSrcDataCallbackFunc
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
//! \param pGetSrcDataCallbackFunc callback function. It is called when the codec need new src data.
//! \return SUCCESS - return the audio handle created
//!            FAILURE - return INVALID_HANDLE
//! \brief  Description: This function is used to create an audio handle which is based on data 
//!                             buffer. Not all codec support buffer controller.
//! \author Author: Benjamin.Wang
//! \note   Note: This operation is synchronous.         
/*****************************************************************************/
PUBLIC HAUDIO AUDIO_CreateStreamBufferHandle( // SUCCESS - return the audio handle created
                                                                            // FAILURE - return SCI_NULL
    HAUDIOCODEC hAudioCodec,    // the codec handle which will be used.
    const uint16* pusCodecName,     // the codec name which will be used.
    HAUDIODEV hDevice, 
    const uint16* pusDevName,     
    const uint8* pucData,   // the music data pointer
    uint32 uiDataLength,    // the music data length
    uint32* puiTrackBuffer,     // decode data output buffer.
                                            // This buffer is created by caller but managed by voiceband 
                                            // controller.
    uint32 uiTrackBufferSize,    // the track buffer size.
    AUDIO_NOTIFY_CALLBACK_PFUNC pNotifyCallbackFunc, /*!< callback func when as notify mmi or other audio_api caller.*/
    AUDIO_GETSRCDATA_CALLBACK_PFUNC pGetSrcDataCallbackFunc      // callback function. It is called when the 
                                                //codec need new src data.      
    );

/*****************************************************************************/
//! \fn PUBLIC AUDIO_RESULT_E AUDIO_ASYCreateStreamBufferHandle(
//!    HAUDIOCODEC hAudioCodec, 
//!    const uint16* pusCodecName, 
//!    HAUDIODEV hDevice, 
//!    const uint16* pusDevName, 
//!    const uint8* pucData, 
//!    uint32 uiDataLength, 
//!    uint32* puiTrackBuffer, 
//!    uint32 uiTrackBufferSize,
//!    AUDIO_NOTIFY_CALLBACK_PFUNC pNotifyCallbackFunc ,
//!    AUDIO_GETSRCDATA_CALLBACK_PFUNC pGetSrcDataCallbackFunc 
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
//! \param pGetSrcDataCallbackFunc callback function. It is called when the codec need new src data.
//! \return SUCCESS - return AUDIO_NO_ERROR
//!            FAILURE - return error reason
//! \brief  Description: This function is used to create an audio handle which is based on data 
//!                             buffer. Not all codec support buffer controller.
//! \author Author: Benjamin.Wang
//! \note   Note: This operation is asynchronous.         
/*****************************************************************************/
PUBLIC AUDIO_RESULT_E AUDIO_ASYCreateStreamBufferHandle( // SUCCESS - return the audio handle created
                                                                            // FAILURE - return SCI_NULL
    HAUDIOCODEC hAudioCodec,    // the codec handle which will be used.
    const uint16* pusCodecName,     // the codec name which will be used.
    HAUDIODEV hDevice, 
    const uint16* pusDevName,     
    const uint8* pucData,   // the music data pointer
    uint32 uiDataLength,    // the music data length
    uint32* puiTrackBuffer,     // decode data output buffer.
                                            // This buffer is created by caller but managed by voiceband 
                                            // controller.
    uint32 uiTrackBufferSize,    // the track buffer size.
    AUDIO_NOTIFY_CALLBACK_PFUNC pNotifyCallbackFunc, /*!< callback func when as notify mmi or other audio_api caller.*/
    AUDIO_GETSRCDATA_CALLBACK_PFUNC pGetSrcDataCallbackFunc      // callback function. It is called when the 
                                                //codec need new src data.      
    );

/*****************************************************************************/
//! \fn PUBLIC AUDIO_RESULT_E AUDIO_CloseHandle( 
//!    HAUDIO hAudioHandle  
//!    )
//! \param hAudioHandle the audio handle.
//! \return return operation result. \sa AUDIO_RESULT_E
//! \brief  Description: This function is used to close the audio handle.
//! \author Author: Benjamin.Wang
//! \note   Note: This operation is synchronous.       
/*****************************************************************************/
PUBLIC AUDIO_RESULT_E AUDIO_CloseHandle(    // return operation result
    HAUDIO hAudioHandle     // the audio handle
    );

/*****************************************************************************/
//! \fn PUBLIC AUDIO_RESULT_E AUDIO_ASYCloseHandle( 
//!    HAUDIO hAudioHandle  
//!    )
//! \param hAudioHandle the audio handle.
//! \return return operation result. \sa AUDIO_RESULT_E
//! \brief  Description: This function is used to close the audio handle.
//! \author Author: Benjamin.Wang
//! \note   Note: This operation is asynchronous.       
/*****************************************************************************/
PUBLIC AUDIO_RESULT_E AUDIO_ASYCloseHandle(    // return operation result
    HAUDIO hAudioHandle     // the audio handle
    );

/*****************************************************************************/
//! \fn PUBLIC AUDIO_RESULT_E AUDIO_Play(  
//!    HAUDIO hAudioHandle,   
//!    uint32 uiOffset,    
//!    AUDIO_GETSRCDATA_CALLBACK_PFUNC pCallbackFunc 
//!    )
//! \param hAudioHandle the audio handle.
//! \param uiOffset the beginning offset.
//! \param pCallbackFunc Callback Function. It is called when the stop event occured.
//! \return return operation result. \sa AUDIO_RESULT_E
//! \brief  Description: This function is used to play the audio.
//! \author Author: Benjamin.Wang
//! \note   Note: This operation is synchronous.       
/*****************************************************************************/
PUBLIC AUDIO_RESULT_E AUDIO_Play(   // return operation result
    HAUDIO hAudioHandle,    // the audio handle
    uint32 uiOffset    // the beginning offset
    );

/*****************************************************************************/
//! \fn PUBLIC AUDIO_RESULT_E AUDIO_ASYPlay(  
//!    HAUDIO hAudioHandle,   
//!    uint32 uiOffset,    
//!    AUDIO_GETSRCDATA_CALLBACK_PFUNC pCallbackFunc 
//!    )
//! \param hAudioHandle the audio handle.
//! \param uiOffset the beginning offset.
//! \param pCallbackFunc Callback Function. It is called when the stop event occured.
//! \return return operation result. \sa AUDIO_RESULT_E
//! \brief  Description: This function is used to play the audio.
//! \author Author: Benjamin.Wang
//! \note   Note: This operation is asynchronous.       
/*****************************************************************************/
PUBLIC AUDIO_RESULT_E AUDIO_ASYPlay(   // return operation result
    HAUDIO hAudioHandle,    // the audio handle
    uint32 uiOffset    // the beginning offset
    );

/*****************************************************************************/
//! \fn PUBLIC AUDIO_RESULT_E AUDIO_Stop(
//!     HAUDIO hAudioHandle
//!     )
//! \param hAudioHandle the audio handle.
//! \return return operation result. \sa AUDIO_RESULT_E
//! \brief  Description: This function is used to stop an audio play.
//! \author Author: Benjamin.Wang
//! \note   Note: This operation is synchronous.       
/*****************************************************************************/
PUBLIC AUDIO_RESULT_E AUDIO_Stop(
    HAUDIO hAudioHandle
    );

/*****************************************************************************/
//! \fn PUBLIC AUDIO_RESULT_E AUDIO_ASYStop(
//!     HAUDIO hAudioHandle
//!     )
//! \param hAudioHandle the audio handle.
//! \return return operation result. \sa AUDIO_RESULT_E
//! \brief  Description: This function is used to stop an audio play.
//! \author Author: Benjamin.Wang
//! \note   Note: This operation is asynchronous.       
/*****************************************************************************/
PUBLIC AUDIO_RESULT_E AUDIO_ASYStop(
    HAUDIO hAudioHandle
    );

/*****************************************************************************/
//! \fn PUBLIC AUDIO_RESULT_E AUDIO_Pause(
//!    HAUDIO hAudioHandle
//!    )
//! \param hAudioHandle the audio handle.
//! \return return operation result. \sa AUDIO_RESULT_E
//! \brief  Description: This function is used to pause an audio play.
//! \author Author: Benjamin.Wang
//! \note   Note: This operation is synchronous.       
/*****************************************************************************/
PUBLIC AUDIO_RESULT_E AUDIO_Pause(
    HAUDIO hAudioHandle
    );

/*****************************************************************************/
//! \fn PUBLIC AUDIO_RESULT_E AUDIO_ASYPause(
//!    HAUDIO hAudioHandle
//!    )
//! \param hAudioHandle the audio handle.
//! \return return operation result. \sa AUDIO_RESULT_E
//! \brief  Description: This function is used to pause an audio play.
//! \author Author: Benjamin.Wang
//! \note   Note: This operation is asynchronous.       
/*****************************************************************************/
PUBLIC AUDIO_RESULT_E AUDIO_ASYPause(
    HAUDIO hAudioHandle
    );

/*****************************************************************************/
//! \fn PUBLIC AUDIO_RESULT_E AUDIO_Resume(
//!    HAUDIO hAudioHandle
//!    )
//! \param hAudioHandle the audio handle.
//! \return return operation result. \sa AUDIO_RESULT_E
//! \brief  Description: This function is used to resume a paused audio play.
//! \author Author: Benjamin.Wang
//! \note   Note: This operation is synchronous.       
/*****************************************************************************/
PUBLIC AUDIO_RESULT_E AUDIO_Resume(
    HAUDIO hAudioHandle
    );

/*****************************************************************************/
//! \fn PUBLIC AUDIO_RESULT_E AUDIO_ASYResume(
//!    HAUDIO hAudioHandle
//!    )
//! \param hAudioHandle the audio handle.
//! \return return operation result. \sa AUDIO_RESULT_E
//! \brief  Description: This function is used to resume a paused audio play.
//! \author Author: Benjamin.Wang
//! \note   Note: This operation is asynchronous.       
/*****************************************************************************/
PUBLIC AUDIO_RESULT_E AUDIO_ASYResume(
    HAUDIO hAudioHandle
    );

/*****************************************************************************/
//! \fn PUBLIC AUDIO_RESULT_E AUDIO_Seek(
//!    HAUDIO hAudioHandle, 
//!    uint32 uiOffset
//!    )
//! \param hAudioHandle the audio handle.
//! \param uiOffset the destination position.
//! \return return operation result. \sa AUDIO_RESULT_E
//! \brief  Description: This function is used to seek random position of an audio.
//! \author Author: Benjamin.Wang
//! \note   Note: This operation is synchronous.       
/*****************************************************************************/
PUBLIC AUDIO_RESULT_E AUDIO_Seek(
    HAUDIO hAudioHandle, 
    uint32 uiOffset
    );

/*****************************************************************************/
//! \fn PUBLIC AUDIO_RESULT_E AUDIO_ASYSeek(
//!    HAUDIO hAudioHandle, 
//!    uint32 uiOffset
//!    )
//! \param hAudioHandle the audio handle.
//! \param uiOffset the destination position.
//! \return return operation result. \sa AUDIO_RESULT_E
//! \brief  Description: This function is used to seek random position of an audio.
//! \author Author: Benjamin.Wang
//! \note   Note: This operation is asynchronous.       
/*****************************************************************************/
PUBLIC AUDIO_RESULT_E AUDIO_ASYSeek(
    HAUDIO hAudioHandle, 
    uint32 uiOffset
    );

/*****************************************************************************/
//! \fn PUBLIC AUDIO_RESULT_E AUDIO_SetSpeed(
//!    HAUDIO hAudioHandle, 
//!    uint32 uiSpeedX
//!    )
//! \param hAudioHandle the audio handle.
//! \param uiSpeedX Speed multiple. For example, 2X, 4X and 8X, the uiSpeedX should 
//!                                                  be 2, 4 and 8. It can't be negative!
//! \return return operation result. \sa AUDIO_RESULT_E
//! \brief  Description: This function is used to set the playing speed of an audio.
//! \author Author: Benjamin.Wang
//! \note   Note: This operation is synchronous.       
/*****************************************************************************/
PUBLIC AUDIO_RESULT_E AUDIO_SetSpeed(
    HAUDIO hAudioHandle, 
    uint32 uiSpeedX
    );

/*****************************************************************************/
//! \fn PUBLIC AUDIO_RESULT_E AUDIO_ASYSetSpeed(
//!    HAUDIO hAudioHandle, 
//!    uint32 uiSpeedX
//!    )
//! \param hAudioHandle the audio handle.
//! \param uiSpeedX Speed multiple. For example, 2X, 4X and 8X, the uiSpeedX should 
//!                                                  be 2, 4 and 8. It can't be negative!
//! \return return operation result. \sa AUDIO_RESULT_E
//! \brief  Description: This function is used to set the playing speed of an audio.
//! \author Author: Benjamin.Wang
//! \note   Note: This operation is asynchronous.       
/*****************************************************************************/
PUBLIC AUDIO_RESULT_E AUDIO_ASYSetSpeed(
    HAUDIO hAudioHandle, 
    uint32 uiSpeedX
    );

/*****************************************************************************/
//! \fn PUBLIC AUDIO_RESULT_E AUDIO_SetDigitalGain(
//!    HAUDIO hAudioHandle, 
//!    uint32 uiDigitalGain
//!    )
//! \param hAudioHandle the audio handle.
//! \param uiDigitalGain Digital Gain. Unit: dB
//! \return return operation result. \sa AUDIO_RESULT_E
//! \brief  Description: This function is used to set the codec digital gain.
//! \author Author: Benjamin.Wang
//! \note   Note: This operation is synchronous.       
/*****************************************************************************/
PUBLIC AUDIO_RESULT_E AUDIO_SetDigitalGain(
    HAUDIO hAudioHandle, 
    int32 uiDigitalGain
    );

/*****************************************************************************/
//! \fn PUBLIC AUDIO_RESULT_E AUDIO_ASYSetDigitalGain(
//!    HAUDIO hAudioHandle, 
//!    uint32 uiDigitalGain
//!    )
//! \param hAudioHandle the audio handle.
//! \param uiDigitalGain Digital Gain. Unit: dB
//! \return return operation result. \sa AUDIO_RESULT_E
//! \brief  Description: This function is used to set the codec digital gain.
//! \author Author: Benjamin.Wang
//! \note   Note: This operation is asynchronous.       
/*****************************************************************************/
PUBLIC AUDIO_RESULT_E AUDIO_ASYSetDigitalGain(
    HAUDIO hAudioHandle, 
    int32 uiDigitalGain
    );
    
/*****************************************************************************/
//! \fn PUBLIC AUDIO_RESULT_E AUDIO_GetDigitalGain(
//!    HAUDIO hAudioHandle,
//!    uint32 * pui_digital_gain
//!    )
//! \param hAudioHandle the audio handle.
//! \param pui_digital_gain the digital gain.
//! \return return result, SUCCESS:AUDIO_NO_ERROR.
//!                               FAILED:AUDIO_ERROR
//! \brief  Description: This function is used to get the codec digital gain. 
//! \author Author: Benjamin.Wang
//! \note   Note: This operation is synchronous.       
/*****************************************************************************/
PUBLIC AUDIO_RESULT_E AUDIO_GetDigitalGain(
    HAUDIO hAudioHandle,
    uint32 *pui_digital_gain
    );

/*****************************************************************************/
//! \fn PUBLIC AUDIO_RESULT_E AUDIO_ASYGetDigitalGain(
//!    HAUDIO hAudioHandle
//!    )
//! \param hAudioHandle the audio handle.
//! \return return this audio's digital gain.
//! \brief  Description: This function is used to get the codec digital gain. The gain value will 
//!                             be feedback in the AUDIO_GETDIGIGAIN_CNF message.
//! \author Author: Benjamin.Wang
//! \note   Note: This operation is asynchronous.       
/*****************************************************************************/
PUBLIC AUDIO_RESULT_E AUDIO_ASYGetDigitalGain(
    HAUDIO hAudioHandle
    );

/*****************************************************************************/
//! \fn PUBLIC AUDIO_RESULT_E AUDIO_SendStreamSrcData(
//!    HAUDIO hAudioHandle, 
//!    uint8 * pucDataAddr, 
//!    uint32 uiDataLength  
//!    )
//! \param hAudioHandle the audio handle.
//! \param ppucDataAddr data address
//! \param uiDataLength data length
//! \return return operation result. \sa AUDIO_RESULT_E
//! \brief  Description: This function is used to send src data when streambuffer play.
//! \author Author: Benjamin.Wang
//! \note   Note: This operation is synchronous.       
/*****************************************************************************/
PUBLIC AUDIO_RESULT_E AUDIO_SendStreamSrcData(   // return operation result
    HAUDIO hAudioHandle, 
    uint8 const * pucDataAddr,     
    uint32 uiDataLength 
    );

/*****************************************************************************/
//! \fn PUBLIC AUDIO_RESULT_E AUDIO_IdentifyAudioFormat(
//!    HAUDIO hAudioHandle, 
//!    AUDIO_FORMATINFO_T* ptFormatInfo
//!    )
//! \param hAudioHandle the audio handle.
//! \param ptFormatInfo The data buffer pointer which is used to store the file information. 
//!                          This parameter is an OUT parameter.
//! \return return operation result. \sa AUDIO_RESULT_E
//! \brief  Description: This function is used to get all file information provided by codec.
//! \author Author: Benjamin.Wang
//! \note   Note: None.       
/*****************************************************************************/
PUBLIC AUDIO_RESULT_E AUDIO_IdentifyAudioFormat(
    HAUDIO hAudioHandle, 
    AUDIO_FORMATINFO_T* ptFormatInfo            // OUT parameter
    );

/*****************************************************************************/
//! \fn PUBLIC AUDIO_RESULT_E AUDIO_GetPlayingInfo(
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
//! \note   Note: This operation is synchronous.       
/*****************************************************************************/
PUBLIC AUDIO_RESULT_E AUDIO_GetPlayingInfo(
    HAUDIO hAudioHandle, 
    AUDIO_PLAYINFO_T* ptPlayInfo
    );

/*****************************************************************************/
//! \fn PUBLIC uint32 AUDIO_GetOutputSampleRate(
//!    HAUDIO hAudioHandle
//!    )
//! \param hAudioHandle The audio handle.
//! \return return the samplerate. If failed, return zero.
//! \brief  Description: This function is used to get audio's samplerate. This samplerate is 
//!                             the actual playing samplerate. If you want to get the samplerate 
//!                             recorded in audio files, you should call AUDIO_SM_IdentifyAudioFormat
//!                              to find it.
//! \author Author: Benjamin.Wang
//! \note   Note: This operation is synchronous.    
/*****************************************************************************/
PUBLIC uint32 AUDIO_GetOutputSampleRate(
    HAUDIO hAudioHandle
    );

/*****************************************************************************/
//! \fn PUBLIC AUDIO_RESULT_E AUDIO_SetOutputSampleRate(
//!    HAUDIO hAudioHandle, 
//!    uint32 uiSamplerate
//!    )
//! \param hAudioHandle The audio handle.
//! \param uiSamplerate The output samplerate. 0 is the default samplerate.
//! \return return the samplerate. If failed, return zero.
//! \brief  Description: This function is used to set audio's output samplerate. 
//!                     This samplerate is the actual playing samplerate.
//! \author Author: Benjamin.Wang
//! \note   Note: This operation is synchronous.  
/*****************************************************************************/
PUBLIC AUDIO_RESULT_E AUDIO_SetOutputSampleRate(
    HAUDIO hAudioHandle, 
    uint32 uiSamplerate
    );
    
/*****************************************************************************/
//! \fn PUBLIC HAUDIO AUDIO_CreateRecordFileHandle(
//!    HAUDIOCODEC hAudioCodec, 
//!    const uint16* pusCodecName, 
//!    HAUDIODEV hDevice, 
//!    const uint16* pusDevName, 
//!    const int16* psFileName,
//!    uint32* puiBuffer, 
//!    uint32 uiBufferSize,   
//!    AUDIO_NOTIFY_CALLBACK_PFUNC pNotifyCallbackFunc 
//!    )
//! \param hAudioCodec the codec handle which will be used.
//! \param pusCodecName the codec name which will be used.
//! \param hDevice the device handle which will be used.
//! \param pusDevName the device name which will be used.
//! \param psFileName the file's name.
//! \param puiBuffer buffer used in recording.
//!                   This buffer is created by caller but managed by record codec.
//! \param uiBufferSize the track buffer size, unit:double word(32bit)
//! \param pNotifyCallbackFunc callback func when as notify mmi or other audio_api caller
//! \return SUCCESS - return the audio handle created. \par
//!            FAILURE - return INVALID_HANDLE
//! \brief  Description: This function is used to create a record handle which is based on FFS. 
//! \author Author: Benjamin.Wang
//! \note   Note: This operation is synchronous.     
/*****************************************************************************/
PUBLIC HAUDIO AUDIO_CreateRecordFileHandle(
    HAUDIOCODEC hAudioCodec, 
    const uint16* pusCodecName, 
    HAUDIODEV hDevice, 
    const uint16* pusDevName,     
    const int16* psFileName, 
    uint32* puiBuffer,                                            
    uint32 uiBufferSize,   
    AUDIO_NOTIFY_CALLBACK_PFUNC pNotifyCallbackFunc 
    );

/*****************************************************************************/
//! \fn AUDIO_RESULT_E  AUDIOASY_CreateRecordFileHandle(
//!    HAUDIOCODEC hAudioCodec, 
//!    const uint16* pusCodecName, 
//!    HAUDIODEV hDevice, 
//!    const uint16* pusDevName, 
//!    const int16* psFileName,
//!    uint32* puiBuffer, 
//!    uint32 uiBufferSize,   
//!    AUDIO_NOTIFY_CALLBACK_PFUNC pNotifyCallbackFunc 
//!    )
//! \param hAudioCodec the codec handle which will be used.
//! \param pusCodecName the codec name which will be used.
//! \param hDevice the device handle which will be used.
//! \param pusDevName the device name which will be used.
//! \param psFileName the file's name.
//! \param puiBuffer buffer used in recording.
//!                   This buffer is created by caller but managed by record codec.
//! \param uiBufferSize the track buffer size, unit:double word(32bit)
//! \param pNotifyCallbackFunc callback func when as notify mmi or other audio_api caller
//! \return SUCCESS - return AUDIO_NO_ERROR. \par
//!            FAILURE - return eror reason
//! \brief  Description: This function is used to create a record handle which is based on FFS. 
//! \author Author: Benjamin.Wang
//! \note   Note: This operation is asynchronous.     
/*****************************************************************************/
PUBLIC AUDIO_RESULT_E AUDIOASY_CreateRecordFileHandle(
    HAUDIOCODEC hAudioCodec, 
    const uint16* pusCodecName, 
    HAUDIODEV hDevice, 
    const uint16* pusDevName,     
    const int16* psFileName, 
    uint32* puiBuffer,     // buffer used in recording.
                                            // This buffer is created by caller but managed by record codec.
    uint32 uiBufferSize,    // the track buffer size, unit:double word(32bit)
    AUDIO_NOTIFY_CALLBACK_PFUNC pNotifyCallbackFunc /*!< callback func when as notify mmi or other audio_api caller.*/
    );

/*****************************************************************************/
//! \fn PUBLIC HAUDIO AUDIO_CreateRecordBufferHandle(
//!    HAUDIOCODEC hAudioCodec, 
//!    const uint16* pusCodecName, 
//!    HAUDIODEV hDevice, 
//!    const uint16* pusDevName, 
//!    uint8* pucData, 
//!    uint32 uiDataLength, 
//!    uint32* puiBuffer, 
//!    uint32 uiBufferSize,   
//!    AUDIO_NOTIFY_CALLBACK_PFUNC pNotifyCallbackFunc 
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
//! \return SUCCESS - return the audio handle created. \par
//!            FAILURE - return INVALID_HANDLE
//! \brief  Description: This function is used to create a record handle which is based on 
//!                             data buffer. 
//! \author Author: Benjamin.Wang
//! \note   Note: This operation is synchronous.       
/*****************************************************************************/
PUBLIC HAUDIO AUDIO_CreateRecordBufferHandle(
    HAUDIOCODEC hAudioCodec, 
    const uint16* pusCodecName, 
    HAUDIODEV hDevice, 
    const uint16* pusDevName, 
    uint8*  pucData, 
    uint32 uiDataLength,
    uint32* puiBuffer,     // buffer used in recording.
                                            // This buffer is created by caller but managed by record codec.
    uint32 uiBufferSize,    // the track buffer size, unit:double word(32bit)
    AUDIO_NOTIFY_CALLBACK_PFUNC pNotifyCallbackFunc /*!< callback func when as notify mmi or other audio_api caller.*/
    );

/*****************************************************************************/
//! \fn PUBLIC AUDIO_RESULT_E AUDIOASY_CreateRecordBufferHandle(
//!    HAUDIOCODEC hAudioCodec, 
//!    const uint16* pusCodecName, 
//!    HAUDIODEV hDevice, 
//!    const uint16* pusDevName, 
//!    uint8* pucData, 
//!    uint32 uiDataLength, 
//!    uint32* puiBuffer, 
//!    uint32 uiBufferSize,   
//!    AUDIO_NOTIFY_CALLBACK_PFUNC pNotifyCallbackFunc 
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
//! \return SUCCESS - return the audio handle created. \par
//!            FAILURE - return INVALID_HANDLE
//! \brief  Description: This function is used to create a record handle which is based on 
//!                             data buffer. 
//! \author Author: Benjamin.Wang
//! \note   Note: This operation is asynchronous.       
/*****************************************************************************/
PUBLIC AUDIO_RESULT_E AUDIOASY_CreateRecordBufferHandle(
    HAUDIOCODEC hAudioCodec, 
    const uint16* pusCodecName, 
    HAUDIODEV hDevice, 
    const uint16* pusDevName, 
    uint8* pucData, 
    uint32 uiDataLength,
    uint32* puiBuffer,     // buffer used in recording.
                                            // This buffer is created by caller but managed by record codec.
    uint32 uiBufferSize,    // the track buffer size, unit:double word(32bit)
    AUDIO_NOTIFY_CALLBACK_PFUNC pNotifyCallbackFunc /*!< callback func when as notify mmi or other audio_api caller.*/
    );

/*****************************************************************************/
//! \fn PUBLIC HAUDIO AUDIO_CreateNormalHandle(
//!    HAUDIOCODEC hAudioCodec, 
//!    const uint16* pusCodecName, 
//!    HAUDIODEV hDevice, 
//!    const uint16* pusDevName,
//!    AUDIO_NOTIFY_CALLBACK_PFUNC pNotifyCallbackFunc 
//!    )
//! \param hAudioCodec the codec handle which will be used.
//! \param pusCodecName the codec name which will be used.
//! \param hDevice the device handle which will be used.
//! \param pusDevName the device name which will be used.
//! \param pNotifyCallbackFunc callback func when as notify mmi or other audio_api caller
//! \return SUCCESS - return the audio handle created. \par
//!            FAILURE - return INVALID_HANDLE
//! \brief  Description: This function is used to create a normal handle. This kind of handle 
//!                             is used to play control a source audio such as voice and tone, etc.
//! \author Author: Benjamin.Wang
//! \note   Note: This operation is synchronous.    
/*****************************************************************************/
PUBLIC HAUDIO AUDIO_CreateNormalHandle(
    HAUDIOCODEC hAudioCodec, 
    const uint16* pusCodecName, 
    HAUDIODEV hDevice, 
    const uint16* pusDevName,
    AUDIO_NOTIFY_CALLBACK_PFUNC pNotifyCallbackFunc /*!< callback func when as notify mmi or other audio_api caller.*/
    );

/*****************************************************************************/
//! \fn PUBLIC AUDIO_RESULT_E AUDIOASY_CreateNormalHandle(
//!    HAUDIOCODEC hAudioCodec, 
//!    const uint16* pusCodecName, 
//!    HAUDIODEV hDevice, 
//!    const uint16* pusDevName,
//!    AUDIO_NOTIFY_CALLBACK_PFUNC pNotifyCallbackFunc 
//!    )
//! \param hAudioCodec the codec handle which will be used.
//! \param pusCodecName the codec name which will be used.
//! \param hDevice the device handle which will be used.
//! \param pusDevName the device name which will be used.
//! \param pNotifyCallbackFunc callback func when as notify mmi or other audio_api caller
//! \return SUCCESS - return AUDIO_NO_ERROR. \par
//!            FAILURE - return error reason
//! \brief  Description: This function is used to create a normal handle. This kind of handle 
//!                             is used to play control a source audio such as voice and tone, etc.
//! \author Author: Benjamin.Wang
//! \note   Note: This operation is asynchronous.    
/*****************************************************************************/
PUBLIC AUDIO_RESULT_E AUDIOASY_CreateNormalHandle(
    HAUDIOCODEC hAudioCodec, 
    const uint16* pusCodecName, 
    HAUDIODEV hDevice, 
    const uint16* pusDevName,
    AUDIO_NOTIFY_CALLBACK_PFUNC pNotifyCallbackFunc /*!< callback func when as notify mmi or other audio_api caller.*/
    );

/*****************************************************************************/
//! \fn AUDIO_RESULT_E AUDIO_ExeCodecExtOpe(
//!    HAUDIO hAudioHandle, 
//!    uint16 *pusExtOpeSymbol, 
//!    uint32 uiArgc, 
//!    void *pvArgv
//!    )
//! \param hAudioHandle Audio Handle.
//! \param pusExtOpeSymbol Symbol of every extend operation.
//! \param uiArgc Number of parameters of the extend operation.
//! \param pvArgv The parameter list.
//! \param pvResult the real return value of execute an extend operation.
//! \return Operation results. \sa AUDIO_RESULT_E
//! \brief  Description: This function is to exe Codec Extend Oper.
//! \author Author: Benjamin.Wang
//! \note   Note: This operation is synchronous.    
/*****************************************************************************/
PUBLIC AUDIO_RESULT_E AUDIO_ExeCodecExtOpe(
    HAUDIO hAudioHandle, 
    uint16 *pusExtOpeSymbol, 
    uint32 uiArgc, 
    void *pvArgv,
    void *pvResult
    );

/*****************************************************************************/
//! \fn PUBLIC uint32 AUDIO_GetSubHandleType(
//!    uint32 uiHandle
//!    )
//! \param uiHandle The handle which will be operated.
//! \return return the subhandle type of a handle 
//! \brief  Description: This function is used to get the subhandle type of a handle.
//! \author Author: Benjamin.Wang
//! \note   Note: None. 
/*****************************************************************************/
PUBLIC uint32 AUDIO_GetSubHandleType(
    uint32 uiHandle
    );

/*****************************************************************************/
//! \fn PUBLIC SFS_HANDLE AUDIO_GetFileHandle(
//!    HAUDIO hAudioHandle
//!    )
//! \param hAudioHandle The handle which will be operated.
//! \return SUCCESS - return the file handle . \par
//!            FAILURE - return INVALID_HANDLE.
//! \brief  Description: This function is used to get the file type.
//! \author Author: Benjamin.Wang
//! \note   Note: None. 
/*****************************************************************************/
PUBLIC SFS_HANDLE AUDIO_GetFileHandle(
    HAUDIO hAudioHandle
    );

/*****************************************************************************/
//! \fn PUBLIC HAUDIOCODEC AUDIO_GetCodecHandle(
//!    HAUDIO hAudioHandle
//!    )
//! \param hAudioHandle The handle which will be operated.
//! \return return the storage medium. \sa HAUDIOCODEC
//! \brief  Description: This function is used to get audio's codec handle. 
//! \author Author: Benjamin.Wang
//! \note   Note: None. 
/*****************************************************************************/
PUBLIC HAUDIOCODEC AUDIO_GetCodecHandle(
    HAUDIO hAudioHandle
    );
    
/*****************************************************************************/
//! \fn PUBLIC HAUDIODEV AUDIO_GetDeviceHandle(
//!    HAUDIO hAudioHandle
//!    )
//! \param hAudioHandle The handle which will be operated.
//! \return return the storage medium. \sa AUDIO_SM_GetDevHandle
//! \brief  Description: This function is used to get audio's play device.  
//! \author Author: Benjamin.Wang
//! \note   Note: None. 
/*****************************************************************************/
PUBLIC HAUDIODEV AUDIO_GetDeviceHandle(
    HAUDIO hAudioHandle
    );
    
/*****************************************************************************/
//! \fn PUBLIC AUDIO_RESULT_E AUDIO_SwitchDevice(
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
//! \note   Note: This operation is synchronous.    
/*****************************************************************************/
PUBLIC AUDIO_RESULT_E AUDIO_SwitchDevice(
    HAUDIO hAudioHandle, 
    HAUDIODEV hDevice, 
    const uint16* pusDevName
    );

/*****************************************************************************/
//! \fn PUBLIC AUDIO_RESULT_E AUDIOASY_SwitchDevice(
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
//! \note   Note: This operation is asynchronous.      
/*****************************************************************************/
PUBLIC AUDIO_RESULT_E AUDIOASY_SwitchDevice(
    HAUDIO hAudioHandle, 
    HAUDIODEV hDevice, 
    const uint16* pusDevName
    );

/*****************************************************************************/
//! \fn PUBLIC uint32 AUDIO_GetDecoderDataLen(
//!    HAUDIO hAudioHandle 
//!    )
//! \param hAudioHandle Audio handle.
//! \return The operation result. \sa uint32
//! \brief  Description: This function is used to get the length of the data
//!           decode but not output.
//! \author Author: Benjamin.Wang
//! \note   Note: None.      
/*****************************************************************************/
PUBLIC uint32 AUDIO_GetDecoderDataLen(
    HAUDIO hAudioHandle
    );

/*****************************************************************************/
//! \fn PUBLIC uint32 AUDIO_GetDecoderDataTime(
//!    HAUDIO hAudioHandle 
//!    )
//! \param hAudioHandle Audio handle.
//! \return The operation result. \sa uint32
//! \brief  Description: This function is used to get the play_time of the data
//!           decode but not output. unit: ms.
//! \author Author: Benjamin.Wang
//! \note   Note: None.      
/*****************************************************************************/
PUBLIC uint32 AUDIO_GetDecoderDataTime(
    HAUDIO hAudioHandle
    );

/*****************************************************************************/
//! \fn PUBLIC AUDIO_RESULT_E AUDIO_ExeExpSetParaOpe(
//!    HAUDIOEXP hExpHandle,
//!    uint16 *pusExpName,
//!    uint32 argc,
//!    void *argv
//! )   
//! \param hExpHandle express handle.
//! \param pusExpName express name.
//! \param argc number of parameters.
//! \param argv content of parameters.
//! \return return operation result \sa AUDIO_RESULT_E
//! \brief  Description:  This function is to exe express set parameter operation
//! \author Author:  Shujing.Dong 
//! \note   Note:Nothing.  
/*****************************************************************************/
PUBLIC AUDIO_RESULT_E AUDIO_ExeExpSetParaOpe(
    HAUDIOEXP hExpHandle, 
    uint16 *pusExpName,  
    uint32 argc,   
    void *argv    
    );       

/*****************************************************************************/
//! \fn PUBLIC AUDIO_RESULT_E AUDIO_SetVolume(
//!    uint32 uiVol
//!    )
//! \param uiVol is Volume value of speaker,1= speaker_vol <=7.
//! \return return operation result \sa AUDIO_RESULT_E.
//! \brief  Description: This function is used to set volume level.
//! \author Author: Shujing.Dong
//! \note   Note: This operation is synchronous.    
/*****************************************************************************/
PUBLIC AUDIO_RESULT_E AUDIO_SetVolume(
    uint32 uiVol
    );

/*****************************************************************************/
//! \fn PUBLIC AUDIO_RESULT_E AUDIO_GetVolume(
//!    uint32  *  puiVol 
//!    )
//! \param puiVol is address of Volume, 1= *speaker_vol <=7.
//! \return return operation result \sa AUDIO_RESULT_E.
//! \brief  Description: This function is used to get volume level.
//! \author Author: Shujing.Dong
//! \note   Note: This operation is synchronous.    
/*****************************************************************************/
PUBLIC AUDIO_RESULT_E AUDIO_GetVolume(
    uint32  * puiVol 
    );

/*****************************************************************************/
//! \fn PUBLIC AUDIO_RESULT_E AUDIO_SetDevMode(
//!    AUDIO_DEVICE_MODE_TYPE_E  eDevMode
//!    )
//! \param eMode: device mode be setted.
//! \return return operation result \sa AUDIO_RESULT_E.
//! \brief  Description: This function is used to set device mode.
//! \author Author: Shujing.Dong
//! \note   Note: This operation is synchronous.    
/*****************************************************************************/
PUBLIC AUDIO_RESULT_E AUDIO_SetDevMode(
    AUDIO_DEVICE_MODE_TYPE_E  eDevMode
    );
    
/*****************************************************************************/
//! \fn PUBLIC AUDIO_RESULT_E AUDIO_GetDevMode(
//!    AUDIO_DEVICE_MODE_TYPE_E *  peMode
//!    )
//! \param peMode: device mode be getted.
//! \return return operation result \sa AUDIO_RESULT_E.
//! \brief  Description: This function is used to get device mode.
//! \author Author: Shujing.Dong
//! \note   Note: This operation is synchronous.    
/*****************************************************************************/
PUBLIC AUDIO_RESULT_E AUDIO_GetDevMode(
    AUDIO_DEVICE_MODE_TYPE_E *  peMode
    );

/*****************************************************************************/
//! \fn PUBLIC AUDIO_RESULT_E AUDIO_SetUpLinkMute(
//!   BOOLEAN bEnable 
//!    )
//! \param bEnable: SCI_TRUE: Mute, SCI_FALSE: Don't mute
//! \return return operation result \sa AUDIO_RESULT_E.
//! \brief  Description: This function is used to enable or disable uplink mute.
//! \author Author: Shujing.Dong
//! \note   Note: This operation is synchronous.    
/*****************************************************************************/ 
PUBLIC AUDIO_RESULT_E AUDIO_SetUpLinkMute(
    BOOLEAN bEnable               
    );

/*****************************************************************************/
//! \fn PUBLIC AUDIO_RESULT_E AUDIO_GetUpLinkMute(
//!   BOOLEAN * pbEnable 
//!    )
//! \param pbEnable: SCI_TRUE: Mute, SCI_FALSE: Don't mute
//! \return return operation result \sa AUDIO_RESULT_E.
//! \brief  Description: This function is used to get state of uplink mute.
//! \author Author: Shujing.Dong
//! \note   Note: This operation is synchronous.    
/*****************************************************************************/ 
PUBLIC AUDIO_RESULT_E AUDIO_GetUpLinkMute(
    BOOLEAN * pbEnable               
    );

/*****************************************************************************/
//! \fn PUBLIC AUDIO_RESULT_E AUDIO_SetDownLinkMute(
//!   BOOLEAN bEnable 
//!    )
//! \param bEnable: SCI_TRUE: Mute,  SCI_FALSE: Don't mute
//! \return return operation result \sa AUDIO_RESULT_E.
//! \brief  Description: This function is used to enable or disable downlink mute.
//! \author Author: Shujing.Dong
//! \note   Note: This operation is synchronous.    
/*****************************************************************************/ 
PUBLIC AUDIO_RESULT_E AUDIO_SetDownLinkMute(
    BOOLEAN bEnable   
    );

/*****************************************************************************/
//! \fn PUBLIC AUDIO_RESULT_E AUDIO_GetDownLinkMute(
//!   BOOLEAN * pbEnable 
//!    )
//! \param pbEnable: SCI_TRUE: Mute, SCI_FALSE: Don't mute
//! \return return operation result \sa AUDIO_RESULT_E.
//! \brief  Description: This function is used to get state of downlink mute.
//! \author Author: Shujing.Dong
//! \note   Note: This operation is synchronous.    
/*****************************************************************************/ 
PUBLIC AUDIO_RESULT_E AUDIO_GetDownLinkMute(
    BOOLEAN * pbEnable               
    );

/*****************************************************************************/
//! \fn PUBLIC AUDIO_RESULT_E AUDIO_SetDevDigitalGain(
//!   uint32 uiDigitalGain
//!    )
//! \param uiDigitalGain:   default:100
//! \return return operation result \sa AUDIO_RESULT_E.
//! \brief  Description: This function is used to set digital gain, 
//!                             dest value is ui_digital_gain percent of original value.
//! \author Author: Shujing.Dong
//! \note   Note: This operation is synchronous.    
/*****************************************************************************/ 
PUBLIC AUDIO_RESULT_E AUDIO_SetDevDigitalGain(
    uint32 uiDigitalGain
    );

/*****************************************************************************/
//! \fn PUBLIC AUDIO_RESULT_E AUDIO_SetDevDigitalGain(
//!   uint32 * puiDigitalGain
//!    )
//! \param puiDigitalGain:  digital gain value
//! \return return operation result \sa AUDIO_RESULT_E.
//! \brief  Description: This function is used to get digital gain, 
//!                             dest value is ui_digital_gain percent of original value.
//! \author Author: Shujing.Dong
//! \note   Note: This operation is synchronous.    
/*****************************************************************************/ 
PUBLIC AUDIO_RESULT_E AUDIO_GetDevDigitalGain(
    uint32 * puiDigitalGain
    );

/*****************************************************************************/
//! \fn PUBLIC HAUDIO AUDIO_CreateStreamBufferHandle(
//!    HAUDIOCODEC hAudioCodec, 
//!    const uint16* pusCodecName, 
//!    HAUDIODEV hDevice, 
//!    const uint16* pusDevName, 
//!    const uint8* pucData, 
//!    uint32 uiDataLength, 
//!    uint32* puiTrackBuffer, 
//!    uint32 uiTrackBufferSize 
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
//! \return SUCCESS - return the audio handle created
//!            FAILURE - return INVALID_HANDLE
//! \brief  Description: This function is used to create an audio handle which is based on data 
//!                             buffer. Not all codec support buffer controller.
//! \author Author: Benjamin.Wang
//! \note   Note: This operation is synchronous.         
/*****************************************************************************/
PUBLIC HAUDIO AUDIO_CreateStreamBufferHandle( // SUCCESS - return the audio handle created
                                                                            // FAILURE - return SCI_NULL
    HAUDIOCODEC hAudioCodec,    // the codec handle which will be used.
    const uint16* pusCodecName,     // the codec name which will be used.
    HAUDIODEV hDevice, 
    const uint16* pusDevName,     
    const uint8* pucData,   // the music data pointer
    uint32 uiDataLength,    // the music data length
    uint32* puiTrackBuffer,     // decode data output buffer.
                                            // This buffer is created by caller but managed by voiceband 
                                            // controller.
    uint32 uiTrackBufferSize    // the track buffer size.
    );

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/    
#ifdef __cplusplus
}
#endif

#endif  // _AUDIO_API_H

// End of audio_api.h


#endif