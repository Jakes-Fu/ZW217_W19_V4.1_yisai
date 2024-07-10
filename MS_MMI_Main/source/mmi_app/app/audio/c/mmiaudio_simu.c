/******************************************************************************
 ** File Name:      audio_api.c                                                                *
 ** Author:         Benjamin.Wang                                             *
 ** DATE:           11/11/2005                                                *
 ** Copyright:      2005 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines audio general operation interface.   *
 **                                                                           *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 11/11/2005     Benjamin.Wang    Create.                                   *
 ******************************************************************************/

 /*! \file audio_api.c
  *  \author Benjamin Wang
  *  \date Nov 11, 2005
  *  \brief This file defines audio general operation interface.
  */

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/ 
#include "std_header.h"
#include "os_api.h"
#include "tasks_id.h"
#include "mmiaudio_simu.h"
#include "audio_api.h"
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif
/**---------------------------------------------------------------------------*
 **                         Macro Definition                                   *
 **---------------------------------------------------------------------------*/
#define MMI_AUDIO_SIMU_HANDLE (1)
/**---------------------------------------------------------------------------*
 **                         Global Variables                                  *
 **---------------------------------------------------------------------------*/
HAUDIOCODEC hMIDICodec;
HAUDIOCODEC hAACCodec;
HAUDIOCODEC hAMRCodec;
HAUDIOCODEC hWAVCodec;
HAUDIOCODEC hMP3Codec;
HAUDIOCODEC hMP3DSPCodec;
#ifdef WMA_SUPPORT
HAUDIOCODEC hWMACodec;
#endif
HAUDIOCODEC hVoiceCodec;
HAUDIOCODEC hCustomCodec;
HAUDIOCODEC hDtmfCodec;
HAUDIOCODEC hGenericCodec;
HAUDIOCODEC hRecordAdpcmCodec;
HAUDIOCODEC hRecordCodec;
HAUDIOCODEC hDSPCodec;
HAUDIOCODEC hAACLCCodec;
HAUDIOCODEC hNullCodec;

HAUDIOCODEC hBlueCodec;
HAUDIOCODEC hBlueRecordCodec;

HAUDIODEV hARMVB;
HAUDIODEV hLAYER1;
HAUDIODEV hRECORDVB;
HAUDIODEV hMP4RECORDVB;
HAUDIODEV hDSPDATADEVICE;
HAUDIODEV hPCMDev;
HAUDIODEV hL1Rec;
HAUDIODEV hLINEINDev;
HAUDIODEV hLINEINADDev;

#ifdef BT_A2DP_SUPPORT
HAUDIODEV hA2DPDev;
#endif

HAUDIOEXP hTransSamExp;
HAUDIOEXP hDGExp;
HAUDIOEXP hEQExp;
HAUDIO    hMyRing;
/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/
 
/**---------------------------------------------------------------------------*
 **                     Local Function Prototypes                             *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Function Definitions                              *
 **---------------------------------------------------------------------------*/

/*****************************************************************************/
//! \fn PUBLIC void AUDIO_Init(void)
//! \param void
//! \return void No return.
//! \brief  Description: This Function is used to initialize the audio server.
//! \author Author: Benjamin.Wang
//! \note   Note:Nothing.           
/*****************************************************************************/
PUBLIC void AUDIO_Init(void)
{
}

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
    )
{
    HAUDIO hHandle = MMI_AUDIO_SIMU_HANDLE;

    return hHandle;
}

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
    )
{
    return AUDIO_NO_ERROR;
}
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
    )
{
    HAUDIO hHandle = MMI_AUDIO_SIMU_HANDLE;

    return hHandle;
}

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
    )
{
    return AUDIO_NO_ERROR;
}

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
    )
{
    HAUDIO hHandle = MMI_AUDIO_SIMU_HANDLE;

    return hHandle;
}

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
    )
{
    return AUDIO_NO_ERROR;
}

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
    )
{
    return AUDIO_NO_ERROR;
}

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
    )
{
    return AUDIO_NO_ERROR;
}

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
    )
{
    return AUDIO_NO_ERROR;
}

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
    )
{
    return AUDIO_NO_ERROR;
}

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
    )
{
    return AUDIO_NO_ERROR;
}

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
    )
{
    return AUDIO_NO_ERROR;
}

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
    )
{
    return AUDIO_NO_ERROR;
}

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
    )
{
    return AUDIO_NO_ERROR;
}

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
    )
{
    return AUDIO_NO_ERROR;
}

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
    )
{
    return AUDIO_NO_ERROR;
}

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
    )
{
    return AUDIO_NO_ERROR;
}

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
    )
{
    return AUDIO_NO_ERROR;
}

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
    )
{
    return AUDIO_NO_ERROR;
}

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
    )
{
    return AUDIO_NO_ERROR;
}

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
    )
{
    return AUDIO_NO_ERROR;
}

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
    )
{
    return AUDIO_NO_ERROR;
}
    
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
    )
{
    return AUDIO_NO_ERROR;
}

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
    )
{
    return AUDIO_NO_ERROR;
}

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
    )
{
    return AUDIO_NO_ERROR;
}

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
    )
{
    return AUDIO_NO_ERROR;
}

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
    )
{
    return AUDIO_NO_ERROR;
}

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
    )
{
    return 0;
}

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
    )
{
    return AUDIO_NO_ERROR;
}
    
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
    )
{
    HAUDIO hHandle = MMI_AUDIO_SIMU_HANDLE;

    return hHandle;
}


/*****************************************************************************/
//! \fn PUBLIC HAUDIO AUDIO_CreateRecordFileHandle_FileCreated(
//!    HAUDIOCODEC hAudioCodec, 
//!    const uint16* pusCodecName, 
//!    HAUDIODEV hDevice, 
//!    const uint16* pusDevName, 
//!    const int16* psFileName,
//!    SFS_HANDLE hFileHandle,
//!    uint32* puiBuffer, 
//!    uint32 uiBufferSize,   
//!    AUDIO_NOTIFY_CALLBACK_PFUNC pNotifyCallbackFunc 
//!    )
//! \param hAudioCodec the codec handle which will be used.
//! \param pusCodecName the codec name which will be used.
//! \param hDevice the device handle which will be used.
//! \param pusDevName the device name which will be used.
//! \param psFileName the file's name.
//! \param hFileHandle the file's handle,not null if file exited.
//! \param puiBuffer buffer used in recording.
//!                   This buffer is created by caller but managed by record codec.
//! \param uiBufferSize the track buffer size, unit:double word(32bit)
//! \param pNotifyCallbackFunc callback func when as notify mmi or other audio_api caller
//! \return SUCCESS - return the audio handle created. \par
//!            FAILURE - return INVALID_HANDLE
//! \brief  Description: This function is used to create a record handle which is based on FFS. 
//!                              and based on existed file. 
//! \author Author: Benjamin.Wang
//! \note   Note: This operation is synchronous.     
/*****************************************************************************/
PUBLIC HAUDIO AUDIO_CreateRecordFileHandle_FileCreated(
    HAUDIOCODEC hAudioCodec, 
    const uint16* pusCodecName, 
    HAUDIODEV hDevice, 
    const uint16* pusDevName, 
    const int16* psFileName,
    SFS_HANDLE hFileHandle, 
    uint32* puiBuffer,                                            
    uint32 uiBufferSize,   
    AUDIO_NOTIFY_CALLBACK_PFUNC pNotifyCallbackFunc 
    )
{
    HAUDIO hHandle = MMI_AUDIO_SIMU_HANDLE;

    return hHandle;
}

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
    )
{
    return AUDIO_NO_ERROR;
}

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
    )
{
    HAUDIO hHandle = MMI_AUDIO_SIMU_HANDLE;

    return hHandle;
}

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
    )
{
    return AUDIO_NO_ERROR;
}

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
    )
{
    HAUDIO hHandle = MMI_AUDIO_SIMU_HANDLE;

    return hHandle;
}

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
    )
{
    return AUDIO_NO_ERROR;
}

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
    )
{
    return AUDIO_NO_ERROR;
}

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
    )
{
    return 0;
}
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
    )
{
    return INVALID_HANDLE;
}

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
    )
{
    return 0;
}
    
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
    )
{
    return 0;
}
    
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
    )
    
{
    return AUDIO_NO_ERROR;
}

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
    )
{
    return AUDIO_NO_ERROR;
}
    

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
    )
{
    return 0;
}

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
    )

{
    return 0;
}

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
    )
{
    return AUDIO_NO_ERROR;
}   

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
    )
{
    return AUDIO_NO_ERROR;
}

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
    )
{
    return AUDIO_NO_ERROR;
}

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
    )
{
    return AUDIO_NO_ERROR;
}
    
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
    )
{
    return AUDIO_NO_ERROR;
}

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
    )
{
    return AUDIO_NO_ERROR;
}

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
    )
{
    return AUDIO_NO_ERROR;
}

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
    )
{
    return AUDIO_NO_ERROR;
}

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
    )
{
    return AUDIO_NO_ERROR;
}

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
    )
{
    return AUDIO_NO_ERROR;
}

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
    )
{
    return AUDIO_NO_ERROR;
}

/*****************************************************************************/
//! \fn PUBLIC AUDIO_RESULT_E AUDIO_ChangeCodecPriority(
//!   uint32 * puiDigitalGain
//!    )
//! \param puiDigitalGain:  digital gain value
//! \return return operation result \sa AUDIO_RESULT_E.
//! \brief  Description: This function is used to get digital gain, 
//!                             dest value is ui_digital_gain percent of original value.
//! \author Author: Shujing.Dong
//! \note   Note: This operation is synchronous.    
/*****************************************************************************/ 
PUBLIC AUDIO_RESULT_E AUDIO_ChangeCodecPriority(
    HAUDIO hAudioHandle,
    BOOLEAN bHighPriority
    )
{
    return AUDIO_NO_ERROR;
}

/*****************************************************************************/
//! \fn PUBLIC AUDIO_RESULT_E AUDIO_OpenSpectrum(
//!      HAUDIO hAudioHandle, 
//!    )
//! \param hAudioHandle Audio Handle.
//! \return return operation result \sa AUDIO_RESULT_E.
//! \brief  Description: This function is used to open the func for spectrum calculating.
//! \author Author: Cherry.liu&Hyy
//! \note   Note: This operation is synchronous.    
/*****************************************************************************/ 
PUBLIC AUDIO_RESULT_E AUDIO_OpenSpectrum(
    HAUDIO hAudioHandle
    )
{
    return AUDIO_NO_ERROR;
}
/*****************************************************************************/
//! \fn PUBLIC AUDIO_RESULT_E AUDIO_CloseSpectrum(
//!      HAUDIO hAudioHandle, 
//!    )
//! \param hAudioHandle Audio Handle.
//! \return return operation result \sa AUDIO_RESULT_E.
//! \brief  Description: This function is used to close the func for spectrum calculating.
//! \author Author: Cherry.liu &Hyy
//! \note   Note: This operation is synchronous.    
/*****************************************************************************/ 
PUBLIC AUDIO_RESULT_E AUDIO_CloseSpectrum(
    HAUDIO hAudioHandle
    )
{
    return AUDIO_NO_ERROR;
}
/*****************************************************************************/
//! \fn PUBLIC AUDIO_RESULT_E AUDIO_GetSpectrum(
//!      HAUDIO hAudioHandle, 
//!   	uint16*  puiSpecBuf
//! 		uint16* pusSpecNum,
//! 		uint16* pusSpecValueMax
//!    )
//! \param hAudioHandle Audio Handle.
//! \param puiSpecBuf stores spectrum.<out pointer>
//! \param pusSpecNum stores spectrum num.<out num>
//! \param pusSpecValueMax stores max value of spectrum.<out value>
//! \return return operation result \sa AUDIO_RESULT_E.
//! \brief  Description: This function is used to get current spectrum info.
//! \author Author: Cherry.liu&Hyy
//! \note   Note: This operation is synchronous.    
/*****************************************************************************/ 
PUBLIC AUDIO_RESULT_E AUDIO_GetSpectrum(
	HAUDIO hAudioHandle,
	uint16*  puiSpecBuf,
	uint16* pusSpecNum,
	uint16* pusSpecValueMax
    )
{
    return AUDIO_NO_ERROR;
}

/**---------------------------------------------------------------------------*
 **                         Local Function Definitions                              *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
    
#endif  // End of audio_api.c
