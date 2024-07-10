/******************************************************************************
 ** File Name:      adm.h                                                *
 ** Author:         Benjamin.Wang                                             *
 ** DATE:           12/17/2005                                                *
 ** Copyright:      2005 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the basic operation interfaces of Audio Device Manager.*
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 12/17/2005     Benjamin.Wang     Create.                                  *
 ******************************************************************************/

 /*! \file adm.h
  *  \author Benjamin Wang
  *  \date Dec 17, 2005
  *  \brief This file defines the basic operation interfaces of Audio Device Manager.
  */

#ifndef _ADM_H
#define _ADM_H

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "os_api.h"
#include "audio_api.h"
#include "asm.h"

#ifdef __cplusplus
    extern   "C"
    {
#endif

/**---------------------------------------------------------------------------*
 **                         MACRO Definations                                     *
 **---------------------------------------------------------------------------*/
/*! \def ADM_MAX_TRACK_NUM
 *  The max track amount supported by system.
 */
#define ADM_MAX_TRACK_NUM (3)

/*! \def ADM_INVALID_TRACK
 *  Invalid track number.
 */
#define ADM_INVALID_TRACK (0xFFFFFFFF)

/*! \def ADM_DEFAULT_TRACKON_POINT
 *  Default trackon point. (15 * 160) indicates 8KSamplerate delays 300ms
 */
#define ADM_DEFAULT_TRACKON_POINT (15 * 160)       // 8KSamplerate delay 300ms
#define ADM_NAME_MAX_LEN (96)

/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/
/*! \typedef DEV_SETTING_T
 *  \brief This is the prototype of device setting.
 */
typedef struct DEV_SETTING_tag
{
    AUDIO_DEVICE_MODE_TYPE_E  eDev_mode;
    uint32  uiVol;
    AUDIO_ARM_APP_TYPE_E eAppType;
    uint32  uiDigital_gain;
    DEV_DATA_OUTPUT_CHANNEL_CHOICE_E eChannelChoice;
    BOOLEAN  bUplink_mute;
    BOOLEAN  bDownlink_mute;    
	BOOLEAN  bDownlinkLocal_mute;
	BOOLEAN  bDownlinkNet_mute;
    uint16 pad0;
    uint8   pad1;
    BOOLEAN  bExtraVolFlag;    
    uint16 usExtraVolGain;    
    uint16 usKeyTone;    
}DEV_SETTING_T;

/*! \typedef ADM_PLAY_PFUNC
 *  \brief This is the prototype of opening audio device function.
 *  \return void
 */
typedef AUDIO_RESULT_E (* ADM_OPEN_PFUNC)(
    uint32,  /*!<Sample rate.*/ 
    DEV_SETTING_T *  /* device setting */
    );

/*! \typedef ADM_CLOSE_PFUNC
 *  \brief This is the prototype of closing audio device function.
 *  \return void
 */
typedef AUDIO_RESULT_E (* ADM_CLOSE_PFUNC)(
    DEV_SETTING_T * /* device setting */
    );

typedef AUDIO_RESULT_E (* ADM_PRECLOSE_PFUNC)(
    DEV_SETTING_T * /* device setting */
    );   

/*! \typedef ADM_PAUSE_PFUNC
 *  \brief This is the prototype of pausing audio device function.
 *  \return void
 */
typedef AUDIO_RESULT_E (* ADM_PAUSE_PFUNC)(
    DEV_SETTING_T  * /* device setting */
    );

/*! \typedef ADM_RESUME_PFUNC
 *  \brief This is the prototype of resuming audio device function.
 *  \return void
 */
typedef AUDIO_RESULT_E (* ADM_RESUME_PFUNC)(
    uint32 ,  /*!<Sample rate.*/ 

    DEV_SETTING_T * /* device setting */
    );

/*! \typedef ADM_SET_SAMPLERATE_PFUNC
 *  \brief This is the prototype of setting device's samplerate.
 *  \return void
 */
typedef void (* ADM_SET_SAMPLERATE_PFUNC)(
    uint32              /*!<Sample rate.*/ 
    );

/*! \typedef ADM_RECCALLBACK
 *  \brief This is the prototype of record data callback. When 
 *          record device get enough data, it will call this callback 
 *          function to notify the client data's coming.
 *  \return void
 */
typedef void (* ADM_RECCALLBACK)(
    uint32              /*!<data count*/
    );

/*! \typedef ADM_PLAYCALLBACK
 *  \brief This is the prototype of play data callback..
 *  \return void
 */
typedef void (* ADM_PLAYCALLBACK)(
    void
    );

/*! \typedef ADM_REGRECCALLBACK_PFUNC
 *  \brief This is the prototype of registering ADM_RECCALLBACK.
 *  \return Register vector which is used to indicate this register operation.
 */
typedef uint32 (* ADM_REGRECCALLBACK_PFUNC)(
    ADM_RECCALLBACK /*!<Registed record data callback function*/
    );

/*! \typedef ADM_UNREGRECCALLBACK_PFUNC
 *  \brief This is the prototype of unregistering ADM_RECCALLBACK.
 *  \return void
 */
typedef void (* ADM_UNREGRECCALLBACK_PFUNC)(
    uint32  /*!<the register vector.*/
    );

/*! \typedef ADM_REGPLAYCALLBACK_PFUNC
 *  \brief This is the prototype of registering ADM_PLAYCALLBACK.
 *  \return Register vector which is used to indicate this register operation.
 */
typedef uint32 (* ADM_REGPLAYCALLBACK_PFUNC)(
    ADM_PLAYCALLBACK /*!<Registed record data callback function*/
    );

/*! \typedef ADM_UNREGPLAYCALLBACK_PFUNC
 *  \brief This is the prototype of unregistering ADM_PLAYCALLBACK.
 *  \return void
 */
typedef void (* ADM_UNREGPLAYCALLBACK_PFUNC)(
    uint32  /*!<the register vector.*/
    );


/*! \typedef ADM_READDATA_PFUNC
 *  \brief This is the prototype of reading device's data.
 *  \return Data length which is read.
 */
typedef void (* ADM_READDATA_PFUNC)( 
    uint16*,        /*!<dest data ptr*/
    uint32,         /*!<data length to get*/
    uint32*         /*!<real data length*/
    );


/*! \typedef ADM_WRITEDATA_PFUNC
 *  \brief This is the prototype of writing device's data.
 *  \return Data length which is Write.
 */
typedef void (* ADM_WRITEDATA_PFUNC)( 
    const uint16*,        /*!<dest data ptr*/
    uint32,         /*!<data length to write*/
    uint32*         /*!<real data length*/
    );

/*! \typedef ADM_STARTRECORD_PFUNC
 *  \brief This is the prototype of starting record.
 *  \return void
 */
typedef void (* ADM_STARTRECORD_PFUNC)(
    uint32      /*!<sample rate*/
    );

/*! \typedef ADM_STOPRECORD_PFUNC
 *  \brief This is the prototype of stopping record.
 *  \return void
 */
typedef void (* ADM_STOPRECORD_PFUNC)(
    void
    );

/*! \typedef ADM_SETDEVMODE_PFUNC
 * \brief This is the prototype of Setting device mode.
 * \return void
 */
typedef void (*ADM_SETDEVMODE_PFUNC)(
	AUDIO_DEVICE_MODE_TYPE_E
	);

/*! \typedef ADM_SETDEVMODE_PFUNC
 * \brief  This is the prototype of Setting volume.
 * \return
 */
typedef void (*ADM_SETVOLUME_PFUNC)(
	uint32,
	AUDIO_ARM_APP_TYPE_E
	);

/*! \typedef ADM_SETEXTRAVOLUME_PFUNC
 * \brief  This is the prototype of Setting volume.
 * \return
 */
typedef void (*ADM_SETEXTRAVOLUME_PFUNC)(
       BOOLEAN,
	uint16
	);
/*! \typedef ADM_SETUPLINKMUTE_PFUNC
 * \brief This is the prototype of Setting uplink mute.
 * \return void
 */
typedef void (*ADM_SETUPLINKMUTE_PFUNC)(
	BOOLEAN
	);

/*! \typedef ADM_SETDOWNLINKMUTE_PFUNC
 * \brief This is the prototype of Setting downlinkmute.
 * \return void
 */
typedef void (*ADM_SETDOWNLINKMUTE_PFUNC)(
	BOOLEAN
	);
typedef void (*ADM_SETMUTEDEPOP_PFUNC)(
	BOOLEAN
	);
/*! \typedef ADM_SETDOWNLINKLOCALNETMUTE_PFUNC
 * \brief This is the prototype of Setting downlink local and net mute.
 * \return void
 */
typedef void (*ADM_SETDOWNLINKLOCALNETMUTE_PFUNC)(
	BOOLEAN,
	BOOLEAN
	);

/*! \typedef ADM_SETDIGITALGAIN_pFUNC
 * \brief This is the prototype of Setting digital gain.
 * \return void
 */
typedef void (*ADM_SETDIGITALGAIN_pFUNC)(
	uint32
	);
	
/*! \typedef ADM_GETREMAINEDPCMCOUNT_pFUNC
 * \brief This is the prototype of getting remained pcm data count.
 * \return void
 */	
typedef uint32 (*ADM_GETREMAINEDPCMCOUNT_PFUNC)(
	void
	);

/*! \typedef ADM_GETREMAINEDPCMTIME_PFUNC
 * \brief This is the prototype of getting play time of remained pcm data, unit:ms.
 * \return void
 */	
typedef uint32 (*ADM_GETREMAINEDPCMTIME_PFUNC)(
	void
	);
	
/*! \typedef ADM_GETAPPOUTPUTSAMPLERATE_PFUNC
 * \brief This is the prototype of getting appropriate output samplerate.
 * \return void
 */	
typedef uint32 (*ADM_GETAPPOUTPUTSAMPLERATE_PFUNC)(
	uint32
	);
	
#ifdef BT_DIALER_SET_VOLGAIN    	
typedef void (*ADM_SETVOLGAIN_PFUNC)(
	AUDIO_ARM_APP_TYPE_E,
	void *
	);
#endif	
typedef AUDIO_RESULT_E (* ADM_SET_NOTCHTONEEN_PFUNC)(
    uint32              /*!<notch tone enable.*/ 
    );
typedef AUDIO_RESULT_E (* ADM_ENABLEOUTPUT_PFUNC)(void);

typedef struct
{
    uint32 ptsinM;   //0x8c
    uint32 ptsinL;   //0x90
    uint32 ptcosM;   //0x94
    uint32 ptcosL;   //0x98
    uint32 ptgl;     //0x9c
    uint32 ptgr;     //0xa0
    uint32 gd;       //0xa4
    uint32 gu;       //0xa8
    uint32 holdtime; //0xac
}NothPara; 
/*! \struct AUDIO_CODEC_GEN_OPERATION_T
 *  \brief save the address of every genOperation.
 */
typedef struct AUDIO_Device_Operation_struct
{
    ADM_OPEN_PFUNC pOpen;    /*!<Open audio device and start play.*/
    ADM_CLOSE_PFUNC pClose;    /*!<Close audio device.*/
    ADM_PAUSE_PFUNC pPause; /*!<We can think it as mute operation.*/
    ADM_RESUME_PFUNC pResume;   /*!< Resume from pause or mute.*/
    ADM_SETDEVMODE_PFUNC pSetDevMode;  /*!<Set Device Mode      */   
    ADM_SETVOLUME_PFUNC    pSetVolume;     /*!<Set Volume             */
    ADM_SETEXTRAVOLUME_PFUNC    pSetExtraVolume;     /*!<Set Volume             */
    ADM_SETUPLINKMUTE_PFUNC        pSetUplinkMute;      /*!<Set Mute, True,mute */
    ADM_SETDOWNLINKMUTE_PFUNC   pSetDownlinkMute; /*!<Set Mute, True,mute */
    ADM_SETDOWNLINKLOCALNETMUTE_PFUNC   pSetDownlinkLocalNetMute; /*!<Set Mute, True,mute */
    ADM_SETMUTEDEPOP_PFUNC pSetDepopMute;
    ADM_SETDIGITALGAIN_pFUNC        pSetDigitalGain;     /*!<Set Digital gain        */
    ADM_SET_SAMPLERATE_PFUNC  pSetSamplerate;   /*!<Set device's samplerate.*/
    ADM_REGRECCALLBACK_PFUNC pRegRecCallback; /*!<Register record callback.*/
    ADM_UNREGRECCALLBACK_PFUNC pUnRegRecCallback; /*!<Unregister record callback.*/
    ADM_REGPLAYCALLBACK_PFUNC pRegPlayCallback; /*!<Register play callback.*/
    ADM_UNREGPLAYCALLBACK_PFUNC pUnRegPlayCallback; /*!<Unregister play callback.*/
    ADM_READDATA_PFUNC pReadData; /*!<Reading data from device.*/
    ADM_WRITEDATA_PFUNC pWriteData; /*!<Writing data to device.*/
    ADM_STARTRECORD_PFUNC pStartRecord; /*!<Start record.*/
    ADM_STOPRECORD_PFUNC pStopRecord;   /*!<Stop record.*/
    ADM_GETREMAINEDPCMCOUNT_PFUNC pGetRemainedPcmCount; /*!<get remained pcm data count */
    ADM_GETREMAINEDPCMTIME_PFUNC pGetRemainedPcmTime; /*!<get play time of remained pcm data, unit:ms.*/
    ADM_GETAPPOUTPUTSAMPLERATE_PFUNC pGetAppropriateOutputSamplerate;
    ADM_PRECLOSE_PFUNC pPostOpen;
    ADM_PRECLOSE_PFUNC pPreClose;
    ADM_SET_NOTCHTONEEN_PFUNC pSetNotchton;
    ADM_ENABLEOUTPUT_PFUNC pEnableOutput;
#ifdef BT_DIALER_SET_VOLGAIN      
    ADM_SETVOLGAIN_PFUNC    pSetVolGain;     /* SET GAIN FOR SPECIAL VOL */
#endif
}AUDIO_DEVICE_OPERATION_T;

/*! \struct AUDIO_DEVICE_T
 *  \brief Device information.
 */
typedef struct AUDIO_Device_struct
{
    uint16 usDeviceName[ADM_NAME_MAX_LEN];    /*!< Device name. The name length should be less than 16 words.*/
    BOOLEAN bIsNeedTrack;   /*!<Flag whether this device need the track  mechanism.*/
    uint8 ucDevOpenCnt; /*!< Device Open count */
    AUDIO_DEVICE_OPERATION_T tAudioDevOpe;  /*!< Device's operation list.*/
    uint32 uiMinFetchLength;     /*!< The least data count fetched from buffer every ISR.*/
}AUDIO_DEVICE_T;

/*! \struct ADM_DecOutputBuf_T
 *  \brief Audio data output buffer control sturcture.
 */	
typedef struct
{
    uint32 data_in_pos;     /*!<Data in pointer.*/
    uint32 data_out_pos;    /*!<Data out pointer.*/
    uint32 data_count;  /*!<Total data count.*/
	
    uint32 buffer_size;	/*!<Buffer size. Unit: Half word(16bits)*/
    int16* data_buffer_ptr; /*!<Left channel buffer pointer.*/
    int16* data_buffer_ptr_r;   /*!<Right channel buffer pointer.*/
    uint32 buffull_watermark;	  /*!<Buffer full  watermark value.*/
    uint32 bufempty_watermark; /*!<Buffer empty watermark value.*/
    uint32 bufempty_windows;	  /*!<Buffer empty windows value.*/
    uint32 data_overflow;
    uint32 bufempty_watermark_count;    
    DEV_DATA_OUTPUT_CHANNEL_CHOICE_E channel_choise_e;
    BOOLEAN bDecodeEnd;
}ADM_DecOutputBuf_T;

/*! \struct ADM_TRACK_T
 *  \brief Track record sturcture.
 */	
typedef struct ADM_Track_struct
{
    uint32 uiTrackNum;  /*!<Track's serial number.*/
    
    struct ADM_Track_struct* ptNextTrack;   /*!<Next track.*/
    struct ADM_Track_struct* ptPreTrack;    /*!<Previous track.*/
}ADM_TRACK_T;

/*! \struct AUDIO_DEVICE_HANDLE_T
 *  \brief Audio Device Handle. Represent an audio device.
 */
typedef struct AUDIO_Device_Handle_struct
{
    HAUDIODEV hDeviceHandle;    /*!< Device Handle which is assigned in register operation.*/
    AUDIO_DEVICE_T tDeviceInfo; /*!< Device information includes device name and operation interface.*/
    ADM_TRACK_T* ptTrackList;   /*!< Track list records all tracks which will export to this device.*/
    uint32 uiTotalTrackon;  /*!< The total counts of all turned on tracks.*/
    uint32 uiTotalTrackOccupied;    /*!< The total counts of all occupied tracks.*/
    uint32 uiCurrentSamplerate; /*!< Current device samplerate.*/

    struct AUDIO_Device_Handle_struct* ptNextHandle;   /*!< Link pointer. Point to next element.*/
    struct AUDIO_Device_Handle_struct* ptPreHandle;    /*!< Link pointer. Point to previous element.*/
}AUDIO_DEVICE_HANDLE_T;

typedef struct tag_ADM_TRACK_AUDOBJ_LST
{
    struct tag_ADM_TRACK_AUDOBJ_LST *pNext;
    AUDIO_OBJECT_T * ptAudioObj;
}ADM_TRACK_AUDOBJ_LST;

/*! \struct ADM_TRACKCON_T
 *  \brief Track control sturcture.
 */	
typedef struct
{
    BOOLEAN track_on;   /*!<Flag this track has been turned on.*/
    BOOLEAN track_occupied; /*!<Flag this track has been occupied. But it is not always on.*/
    uint16  res0;
    uint32 sample_rate; /*!<The sample rate of the track.*/
    uint32 track_on_point;  /*!<The delayed buffer depth which is used to ensure decoder 
                                                has enough time to decode enough time.*/
    AUDIO_DEVICE_HANDLE_T* ptDevice;    /*!<Track's output device.*/
    AUDIO_OBJECT_T * ptAudioObj; /*!<Record Audio Object.*/                                            
    /*xuxin - 20181114*/
    ADM_TRACK_AUDOBJ_LST *pHead;
}ADM_TRACKCON_T;

/**---------------------------------------------------------------------------*
 **                         Global Variables                                  *
 **---------------------------------------------------------------------------*/
/*! \var g_adm_dec_output_buf
 *  \brief Decode output track array.
 */
extern ADM_DecOutputBuf_T g_adm_dec_output_buf[ADM_MAX_TRACK_NUM];
/*! \var g_adm_track_controller
 *  \brief Decode output track controller array.
 */
extern ADM_TRACKCON_T g_adm_track_controller[ADM_MAX_TRACK_NUM]; 

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//! \fn PUBLIC HAUDIODEV AUDIO_DM_RegAudioDevice(
//!    AUDIO_DEVICE_T* ptDeviceInfo 
//!    )
//! \param ptDeviceInfo Address used to save device's information. \sa AUDIO_DEVICE_T
//! \return The device handle.
//! \brief  Description: This function is used to register the audio device to device manager.
//! \author Author: Benjamin.Wang
//! \note   Note: None.      
/*****************************************************************************/
PUBLIC HAUDIODEV AUDIO_DM_RegAudioDevice(
    AUDIO_DEVICE_T* ptDeviceInfo 
    );

/*****************************************************************************/
//! \fn PUBLIC AUDIO_RESULT_E AUDIO_DM_UnRegAudioDevice(
//!    HAUDIODEV hDevice
//!    )
//! \param hDevice The operated device handle .
//! \return Operation results. \sa AUDIO_RESULT_E
//! \brief  Description: This function is used to unregister the audio device from ADM.
//! \author Author: Benjamin.Wang
//! \note   Note: None.      
/*****************************************************************************/
PUBLIC AUDIO_RESULT_E AUDIO_DM_UnRegAudioDevice(
    HAUDIODEV hDevice
    );

/*****************************************************************************/
//! \fn PUBLIC uint32 AUDIO_DM_Open(
//!    HAUDIODEV hDevice, 
//!    uint32* puiBufferStartAddr, 
//!    uint32 uiBufferSize, 
//!    uint32 uiSampleRate, 
//!    uint32 uiTrackOnPoint, 
//!    BOOLEAN bStereoOn, 
//!    )
//! \param hDevice The operated device handle .
//! \param puiBufferStartAddr Track's buffer.
//! \param uiBufferSize Track's buffer size.
//! \param uiSampleRate Audio's samplerate .
//! \param uiTrackOnPoint The delayed buffer depth which is used to ensure decoder 
//!                                   has enough time to decode enough time..
//! \param bStereoOn Whether the track buffer should be initialized as stereo mode.
//! \return The track number. If failed, return invalid track number: 0xFFFFFFFF.
//! \brief  Description: This function is used to open the audio device and assign a track 
//!                             to the audio(if supported).
//! \author Author: Benjamin.Wang
//! \note   Note: None.      
/*****************************************************************************/
PUBLIC uint32 AUDIO_DM_Open(
    HAUDIODEV hDevice, 
    uint32* puiBufferStartAddr, 
    uint32 uiBufferSize, 
    uint32 uiSampleRate, 
    uint32 uiTrackOnPoint, 
    BOOLEAN bStereoOn
    );

/*****************************************************************************/
//! \fn PUBLIC AUDIO_RESULT_E AUDIO_DM_Close(
//!    HAUDIODEV hDevice,
//!    uint32 uiTrackNum,
//!    BOOLEAN bHasPaused
//!    )
//! \param hDevice The operated device handle .
//! \param uiTrackNum The operated track number.
//! \param bHasPaused if it has paused, SCI_TRUE:paused. SCI_FALSE:not puase.
//! \return Operation results. \sa AUDIO_RESULT_E
//! \brief  Description: This function is used to close an track from the audio device. If the 
//!                             device doesn't support track mechanism, he will just close the device.
//! \author Author: Benjamin.Wang
//! \note   Note: None.      
/*****************************************************************************/
PUBLIC AUDIO_RESULT_E AUDIO_DM_Close(
    HAUDIODEV hDevice,
    uint32 uiTrackNum,
    BOOLEAN bHasPaused
    );
    
PUBLIC AUDIO_RESULT_E AUDIO_DM_PostOpen(
    HAUDIODEV hDevice,
    void *param
    );

PUBLIC AUDIO_RESULT_E AUDIO_DM_PreClose(
    void *param
    );

/*xuxin - 20181114 added*/    
PUBLIC AUDIO_OBJECT_T * AUDIO_DM_GetTrackCtrlAudioObj(ADM_TRACKCON_T *pAdmTrackCtrl,BOOLEAN bDevicePaused);
PUBLIC BOOLEAN AUDIO_DM_AddTrackCtrlAudioObj(ADM_TRACKCON_T *pAdmTrackCtrl, AUDIO_OBJECT_T * ptAudioObj);
PUBLIC BOOLEAN AUDIO_DM_DelTrackCtrlAudioObj(ADM_TRACKCON_T *pAdmTrackCtrl, AUDIO_OBJECT_T * ptAudioObj);


/*****************************************************************************/
//! \fn PUBLIC AUDIO_RESULT_E AUDIO_DM_Pause(
//!    HAUDIODEV hDevice, 
//!    uint32 uiTrackNum
//!    )
//! \param hDevice The operated device handle .
//! \param uiTrackNum The operated track number.
//! \return Operation results. \sa AUDIO_RESULT_E
//! \brief  Description: This function is used to pause(mute) an track or the whole device.
//! \author Author: Benjamin.Wang
//! \note   Note: None.      
/*****************************************************************************/
PUBLIC AUDIO_RESULT_E AUDIO_DM_Pause(
    HAUDIODEV hDevice, 
    uint32 uiTrackNum
    );

/*****************************************************************************/
//! \fn PUBLIC AUDIO_RESULT_E AUDIO_DM_Resume(
//!    HAUDIODEV hDevice, 
//!    uint32 uiTrackNum,
//!    uint32 uiSampleRate 
//!    )
//! \param hDevice The operated device handle .
//! \param uiTrackNum The operated track number.
//! \para  uiSampleRate  the aimed samplerate
//! \return Operation results. \sa AUDIO_RESULT_E
//! \brief  Description: This function is used to resume an track or the whole device.
//! \author Author: Benjamin.Wang
//! \note   Note: None.      
/*****************************************************************************/
PUBLIC AUDIO_RESULT_E AUDIO_DM_Resume(
    HAUDIODEV hDevice, 
    uint32 uiTrackNum,
    uint32 uiSampleRate    
    );

/*****************************************************************************/
//! \fn PUBLIC HAUDIODEV AUDIO_DM_GetFirstDevice(void)
//! \param void No parameter.
//! \return The first device handle. If the device list is empty, return INVALID_HANDLE.
//! \brief  Description: This function is used to get the first device in ADM.
//! \author Author: Benjamin.Wang
//! \note   Note: None.      
/*****************************************************************************/
PUBLIC HAUDIODEV AUDIO_DM_GetFirstDevice(void);

/*****************************************************************************/
//! \fn PUBLIC HAUDIODEV AUDIO_DM_GetNextDevice(
//!    HAUDIODEV hDevice
//!    )
//! \param hDevice The operated device handle .
//! \return The specified device handle's next handle. If it isn't exist, return INVALID_HANDLE.
//! \brief  Description: This function is used to get next device of specified device.
//! \author Author: Benjamin.Wang
//! \note   Note: None.      
/*****************************************************************************/
PUBLIC HAUDIODEV AUDIO_DM_GetNextDevice(
    HAUDIODEV hDevice
    );

/*****************************************************************************/
//! \fn PUBLIC AUDIO_RESULT_E AUDIO_DM_AddTrackIntoDevice(
//!    HAUDIODEV hDevice, 
//!    uint32 uiTrackNum
//!    )
//! \param hDevice The operated device handle .
//! \param uiTrackNum The operated track number .
//! \return Operation results. \sa AUDIO_RESULT_E
//! \brief  Description: This function is used to add a track into a device.
//! \author Author: Benjamin.Wang
//! \note   Note: None.      
/*****************************************************************************/
PUBLIC AUDIO_RESULT_E AUDIO_DM_AddTrackIntoDevice(
    HAUDIODEV hDevice, 
    uint32 uiTrackNum
    );

/*****************************************************************************/
//! \fn PUBLIC AUDIO_RESULT_E AUDIO_DM_RemoveTrackFromDevice(
//!    HAUDIODEV hDevice, 
//!    uint32 uiTrackNum
//!    )
//! \param hDevice The operated device handle .
//! \param uiTrackNum The operated track number .
//! \return Operation results. \sa AUDIO_RESULT_E
//! \brief  Description: This function is used to remove a track from a device.
//! \author Author: Benjamin.Wang
//! \note   Note: None.      
/*****************************************************************************/
PUBLIC AUDIO_RESULT_E AUDIO_DM_RemoveTrackFromDevice(
    HAUDIODEV hDevice, 
    uint32 uiTrackNum
    );

/*****************************************************************************/
//! \fn PUBLIC AUDIO_DEVICE_HANDLE_T* AUDIO_DM_GetDeviceHandleStruct(
//!    HAUDIODEV hDevice
//!    )
//! \param hDevice The operated device handle .
//! \return Specified device structure pointer.
//! \brief  Description: This function is used to get specified device handle's device 
//!                             handle struct.
//! \author Author: Benjamin.Wang
//! \note   Note: None.      
/*****************************************************************************/
PUBLIC AUDIO_DEVICE_HANDLE_T* AUDIO_DM_GetDeviceHandleStruct(
    HAUDIODEV hDevice
    );

/*****************************************************************************/
//! \fn PUBLIC HAUDIODEV AUDIO_DM_GetDeviceHandleFromName(
//!    const uint16* pusDevName
//!    )
//! \param pusDevName The device name.
//! \return The matching device handle.
//! \brief  Description: This function is used to get device handle matched to the name.
//! \author Author: Benjamin.Wang
//! \note   Note: None.      
/*****************************************************************************/
PUBLIC HAUDIODEV AUDIO_DM_GetDeviceHandleFromName(
    const uint16* pusDevName
    );

/*****************************************************************************/
//! \fn PUBLIC AUDIO_RESULT_E AUDIO_DM_SwitchDevice(
//!	        AUDIO_OBJECT_T* audio_obj,
//!	        HAUDIODEV hDestDev,
//!	        BOOLEAN bPlayingFlag
//!	)	
//! \param audio_obj audio object
//! \param hDestDev The dest device handle.
//! \param bPlayingFlag the flag of audio playing, 
//! \param              SCI_FALSE:stopped or pauseing, SCI_TRUE:other states.    
//! \return The operation result. \sa AUDIO_RESULT_E
//! \brief  Description: This function is used to switch audio's output device.
//! \author Author: Benjamin.Wang
//! \note   Note: None.      
/*****************************************************************************/
PUBLIC AUDIO_RESULT_E AUDIO_DM_SwitchDevice(
    AUDIO_OBJECT_T* audio_obj,
    HAUDIODEV hDestDev,
    BOOLEAN bPlayingFlag
    );
    
/*****************************************************************************/
//! \fn PUBLIC AUDIO_RESULT_E AUDIO_DM_TrackOn(
//!    uint32 uiTrackNum
//!    )
//! \param uiTrackNum Audio's track number.
//! \return The operation result. \sa AUDIO_RESULT_E
//! \brief  Description: This function is used to switch the track's track_on state.
//! \author Author: Benjamin.Wang
//! \note   Note: None.      
/*****************************************************************************/
PUBLIC AUDIO_RESULT_E AUDIO_DM_TrackOn(
    uint32 uiTrackNum
    );

/*****************************************************************************/
//! \fn PUBLIC AUDIO_RESULT_E AUDIO_DM_SwitchDevice(
//!    uint32 uiTrackNum
//!    )
//! \param uiTrackNum Audio's track number.
//! \return The operation result. \sa AUDIO_RESULT_E
//! \brief  Description: This function is used to switch the track's track_on state.
//! \author Author: Benjamin.Wang
//! \note   Note: None.      
/*****************************************************************************/
PUBLIC AUDIO_RESULT_E AUDIO_DM_TrackOff(
    uint32 uiTrackNum
    );
    
/*****************************************************************************/
//! \fn PUBLIC void AUDIO_DP_SetTrackOnPoint(
//!    uint32 uiTrackNum
//!    uint32 uiTrackOnPoint
//!    )
//! \param uiTrackNum Track number.
//! \param track_on_point  track_on_point value
//! \return None.
//! \brief  Description: This function is used to set track_on_point value.
//! \author Author: Johnson.Sun
//! \note   Note: None.      
/*****************************************************************************/
PUBLIC void AUDIO_DP_SetTrackOnPoint(
      uint32 uiTrackNum, 
      uint32 uiTrackOnPoint
      );    

/*****************************************************************************/
//! \fn PUBLIC void AUDIO_DM_InitDevInfo( 
//!    void 
//!    )
//! \param void
//! \return void
//! \brief  Description: This function is used to init device info.
//! \author Author: Johnson.sun
//! \note   Note: None.      
/*****************************************************************************/
PUBLIC void AUDIO_DM_InitDevInfo(
    void
    );

/*****************************************************************************/
//! \fn PUBLIC AUDIO_RESULT_E AUDIO_DM_GetTrackStatus(
//!    uint32 uiTrackNum
//!    )
//! \param uiTrackNum Audio's track number.
//! \return The operation result. \sa AUDIO_RESULT_E
//! \brief  Description: This function is used to get the track status: on or off.
//! \author Author: Benjamin.Wang
//! \note   Note: None.      
/*****************************************************************************/
PUBLIC AUDIO_RESULT_E AUDIO_DM_GetTrackStatus(
    uint32 uiTrackNum,
    BOOLEAN * b_track_on
    );
    
/*****************************************************************************/
//! \fn PUBLIC AUDIO_RESULT_E AUDIO_DM_SetDevMode( 
//!    AUDIO_DEVICE_MODE_TYPE_E    dev_mode 
//!    )
//! \param dev_mode, device mode to be set.
//! \return AUDIO_RESULT_E, generally return AUDIO_NO_ERROR
//! \brief  Description: This function is used to set device mode
//! \author Author: Johnson.sun
//! \note   Note: None.      
/*****************************************************************************/
PUBLIC AUDIO_RESULT_E AUDIO_DM_SetDevMode(
     AUDIO_DEVICE_MODE_TYPE_E    eDevMode
    );

/*****************************************************************************/
//! \fn PUBLIC AUDIO_RESULT_E AUDIO_DM_SetDevModeEx( 
//!    HAUDIO hAudioHandle,
//!    AUDIO_DEVICE_MODE_TYPE_E    eDevMode 
//!    )
//! \param eDev_mode, device mode to be set.
//! \param hAudioHandle, set mode to its device.
//! \return AUDIO_RESULT_E, generally return AUDIO_NO_ERROR
//! \brief  Description: This function is used to set device mode mandatorily
//! \author Author: TH, shujing.dong
//! \note   Note: 
//!         1) Special usage with special case.      
//!         2) hAudioHandle is permited to be INVALID_HANDLE. 
//!            If it is INVALID_HANDLE, this interface will set the new mode to
//!                all the logic devices playing in the current time mandatorily
//!                no matter whether the new mode is the same with original mode.
/*****************************************************************************/
PUBLIC AUDIO_RESULT_E AUDIO_DM_SetDevModeEx
(
    HAUDIO hAudioHandle,
    AUDIO_DEVICE_MODE_TYPE_E eDevMode 
);

/*****************************************************************************/
//! \fn PUBLIC AUDIO_RESULT_E AUDIO_DM_GetDevMode( 
//!    AUDIO_DEVICE_MODE_TYPE_E    *ptDev_mode 
//!    )
//! \param ptDev_mode, current device mode return value through the pointer.
//! \return AUDIO_RESULT_E, generally return AUDIO_NO_ERROR
//! \brief  Description: This function is used to get current device mode.
//! \author Author: Johnson.sun
//! \note   Note: None.      
/*****************************************************************************/
PUBLIC AUDIO_RESULT_E AUDIO_DM_GetDevMode(
     AUDIO_DEVICE_MODE_TYPE_E    *ptDevMode
    );

/*****************************************************************************/
//! \fn PUBLIC AUDIO_RESULT_E AUDIO_DM_SetVolumeAndTypeEx(
//!    HAUDIO hAudioHandle,
//!    BOOLEAN bExtraVolFlag,
//!    uint16 usExtraVolGain
//!    )
//! \param hAudioHandle: indicate which audio to be set volume and appType.
//! \param bExtraVolFlag  true: enable extra volume, false: disable extra volume
//! \param usExtraVolGain is the gain of extra volume 
//! \return return operation result \sa AUDIO_RESULT_E.
//! \brief  Description: This function is used to enable or disable extra volume setting
//!                             and to set the volume gain.
//! \author Author: shujing.dong
//! \note   Note: 
//!         1) Special usage with special case.      
//!         2) hAudioHandle is permited to be INVALID_HANDLE. 
//!            If it is INVALID_HANDLE, this interface will set the new bExtraVolFlag and usExtraVolGain  to
//!                all the logic devices playing in the current time mandatorily no matter 
//!                whether the new new bExtraVolFlag and usExtraVolGain  are the same with original ones.
/*****************************************************************************/
PUBLIC AUDIO_RESULT_E AUDIO_DM_SetExtraVolumeEx(
    HAUDIO hAudioHandle,
     BOOLEAN bExtraVolFlag,
     uint16 usExtraVolGain
    );    

/*****************************************************************************/
//! \fn PUBLIC AUDIO_RESULT_E AUDIO_DM_SetExtraVolume( 
//!    BOOLEAN bExtraVolFlag,
//!    uint16 usExtraVolGain
//!    )
//! \param bExtraVolFlag  true: enable extra volume, false: disable extra volume
//! \param usExtraVolGain is the gain of extra volume 
//! \return AUDIO_RESULT_E, generally return AUDIO_NO_ERROR
//! \brief  Description: This function is used to enable or disable extra volume setting
//!                             and to set the volume gain.
//! \author Author: Shujing.dong
//! \note   Note: None.      
/*****************************************************************************/
PUBLIC AUDIO_RESULT_E AUDIO_DM_SetExtraVolume(
     BOOLEAN bExtraVolFlag,
     uint16 usExtraVolGain
    );    

/*****************************************************************************/
//! \fn PUBLIC AUDIO_RESULT_E AUDIO_DM_SetVolume( 
//!    uint32 uiVol,
//!    AUDIO_ARM_APP_TYPE_E eAppType
//!    )
//! \param uiVol is level of volume 
//! \param eAppType is the type of application scheme .
//! \return AUDIO_RESULT_E, generally return AUDIO_NO_ERROR
//! \brief  Description: This function is used to set volume.
//! \author Author: Johnson.sun
//! \note   Note: None.      
/*****************************************************************************/
PUBLIC AUDIO_RESULT_E AUDIO_DM_SetVolume(
     uint32  uiVol,
     AUDIO_ARM_APP_TYPE_E eAppType
    );

/*****************************************************************************/
//! \fn PUBLIC AUDIO_RESULT_E AUDIO_DM_GetExtraVolume( 
//!    BOOLEAN *pbExtraVolFlag,
//!    uint16 *pusExtraVolGain
//!    )
//! \param pbExtraVolFlag the pointer of the extra volume flag
//! \param pusExtraVolGain the pointer of the gain of extra volume 
//! \return AUDIO_RESULT_E, generally return AUDIO_NO_ERROR
//! \brief  Description: This function is used to get current extra volume configure infomation.
//! \author Author: Shujing.dong
//! \note   Note: None.      
/*****************************************************************************/
PUBLIC AUDIO_RESULT_E AUDIO_DM_GetExtraVolume(
     BOOLEAN *pbExtraVolFlag,
     uint16 *pusExtraVolGain
    );    
/*****************************************************************************/
//! \fn PUBLIC AUDIO_RESULT_E AUDIO_DM_SetKeyTone(
//!    HAUDIO hAudioHandle,
//!    uint32 uiVol,
//!    AUDIO_ARM_APP_TYPE_E eAppType
//!    )
//! \param hAudioHandle: indicate which audio to be set volume and appType.
//! \param uiVol is level of volume 
//! \param eAppType is the type of application scheme .
//! \return return operation result \sa AUDIO_RESULT_E.
//! \brief  Description: This function is used to set volume level and appType.
//! \author Author: shujing.dong
//! \note   Note: 
//!         1) Special usage with special case.      
//!         2) hAudioHandle is permited to be INVALID_HANDLE. 
//!            If it is INVALID_HANDLE, this interface will set the new volume and appType  to
//!                all the logic devices playing in the current time mandatorily no matter 
//!                whether the new new volume and appType  are the same with original ones.
/*****************************************************************************/
PUBLIC AUDIO_RESULT_E AUDIO_DM_SetKeyTone(
    uint16 usKeyTone
    );

/*****************************************************************************/
//! \fn PUBLIC AUDIO_RESULT_E AUDIO_DM_SetVolumeAndTypeEx(
//!    HAUDIO hAudioHandle,
//!    uint32 uiVol,
//!    AUDIO_ARM_APP_TYPE_E eAppType
//!    )
//! \param hAudioHandle: indicate which audio to be set volume and appType.
//! \param uiVol is level of volume 
//! \param eAppType is the type of application scheme .
//! \return return operation result \sa AUDIO_RESULT_E.
//! \brief  Description: This function is used to set volume level and appType.
//! \author Author: shujing.dong
//! \note   Note: 
//!         1) Special usage with special case.      
//!         2) hAudioHandle is permited to be INVALID_HANDLE. 
//!            If it is INVALID_HANDLE, this interface will set the new volume and appType  to
//!                all the logic devices playing in the current time mandatorily no matter 
//!                whether the new new volume and appType  are the same with original ones.
/*****************************************************************************/
PUBLIC AUDIO_RESULT_E AUDIO_DM_SetVolumeAndTypeEx(
    HAUDIO hAudioHandle,
    uint32 uiVol,
    AUDIO_ARM_APP_TYPE_E eAppType
    );    

/*****************************************************************************/
//! \fn PUBLIC AUDIO_RESULT_E AUDIO_DM_GetVolume( 
//!    uint32  * puiVol ,
//!    AUDIO_ARM_APP_TYPE_E *peAppType
//!    )
//! \param puiVol is address of Volume level.
//! \param peAppType is address of application scheme.
//! \return AUDIO_RESULT_E, generally return AUDIO_NO_ERROR
//! \brief  Description: This function is used to get current volume value.
//! \author Author: Johnson.sun
//! \note   Note: None.      
/*****************************************************************************/
PUBLIC AUDIO_RESULT_E AUDIO_DM_GetVolume(
     uint32  *puiVol,
     AUDIO_ARM_APP_TYPE_E *eAppType
    );

/*****************************************************************************/
//! \fn PUBLIC AUDIO_RESULT_E  AUDIO_DM_SetUplinkMute( 
//!    BOOLEAN bUplinkMute 
//!    )
//! \param bUplinkMute, TRUE: MUTE
//! \return AUDIO_RESULT_E, generally return AUDIO_NO_ERROR
//! \brief  Description: This function is used to set uplink mute.
//! \author Author: Johnson.sun
//! \note   Note: None.      
/*****************************************************************************/
PUBLIC AUDIO_RESULT_E AUDIO_DM_SetUplinkMute(
     BOOLEAN bUplinkMute
    );

/*****************************************************************************/
//! \fn PUBLIC AUDIO_RESULT_E AUDIO_DM_GetUplinkMute( 
//!    BOOLEAN  *ptUplinkMute 
//!    )
//! \param ptUplinkMute, current uplink mute return value through the pointer.
//! \return AUDIO_RESULT_E, generally return AUDIO_NO_ERROR
//! \brief  Description: This function is used to get current uplink mute value.
//! \author Author: Johnson.sun
//! \note   Note: None.      
/*****************************************************************************/
PUBLIC AUDIO_RESULT_E AUDIO_DM_GetUplinkMute(
     BOOLEAN  *pbUplinkMute
    );


/*****************************************************************************/
//! \fn PUBLIC AUDIO_RESULT_E AUDIO_DM_SetDownlinkMute( 
//!    BOOLEAN bDownlinkMute 
//!    )
//! \param bDownlinkMute, TRUE:Mute
//! \return AUDIO_RESULT_E, generally return AUDIO_NO_ERROR
//! \brief  Description: This function is used to set downlink mute.
//! \author Author: Johnson.sun
//! \note   Note: None.      
/*****************************************************************************/
PUBLIC AUDIO_RESULT_E AUDIO_DM_SetDownlinkMute(
     BOOLEAN bDownlinkMute
    );

/*****************************************************************************/
//! \fn PUBLIC AUDIO_RESULT_E AUDIO_DM_GetDownlinkMute( 
//!    BOOLEAN  *ptDownlinkMute 
//!    )
//! \param ptDownlinkMute, current downlink mute return value through the pointer.
//! \return AUDIO_RESULT_E, generally return AUDIO_NO_ERROR
//! \brief  Description: This function is used to get current downlink mute value.
//! \author Author: Johnson.sun
//! \note   Note: None.      
/*****************************************************************************/
PUBLIC AUDIO_RESULT_E AUDIO_DM_GetDownlinkMute(
     BOOLEAN  *pbDownlinkMute
    );
    
/*****************************************************************************/
//! \fn PUBLIC AUDIO_RESULT_E AUDIO_DM_SetDownlinkLocalNetMute( 
//!   BOOLEAN bLocalEnable,
//!   BOOLEAN bNetEnable
//!    )
//! \param bLocalEnable: SCI_TRUE: Mute,  SCI_FALSE: Don't mute
//! \param bNetEnable: SCI_TRUE: Mute,  SCI_FALSE: Don't mute
//! \return return operation result \sa AUDIO_RESULT_E.
//! \brief  Description: This function is used to enable or disable downlink local and net mute.
//! \return AUDIO_RESULT_E, generally return AUDIO_NO_ERROR
//! \brief  Description: This function is used to set downlink mute.
//! \author Author: Johnson.sun
//! \note   Note: None.      
/*****************************************************************************/
PUBLIC AUDIO_RESULT_E AUDIO_DM_SetDownlinkLocalNetMute(
     BOOLEAN bLocalEnable,
     BOOLEAN bNetEnable
    );

/*****************************************************************************/
//! \fn PUBLIC AUDIO_RESULT_E AUDIO_DM_GetDownLinkLocalNetMute(
//!   BOOLEAN *pbLocalEnable,
//!   BOOLEAN *pbNetEnable
//!    )
//! \param pbLocalEnable: SCI_TRUE: Mute,  SCI_FALSE: Don't mute
//! \param pbNetEnable: SCI_TRUE: Mute,  SCI_FALSE: Don't mute
//! \return return operation result \sa AUDIO_RESULT_E.
//! \brief  Description: This function is used to get the state of downlink local and net mute.
//! \author Author: Shujing.Dong
//! \note   Note: This operation is synchronous.    
/*****************************************************************************/ 
PUBLIC AUDIO_RESULT_E AUDIO_DM_GetDownLinkLocalNetMute(
    BOOLEAN *pbLocalEnable,
    BOOLEAN *pbNetEnable
    );    

/*****************************************************************************/
//! \fn PUBLIC AUDIO_RESULT_E AUDIO_DM_SetDataOutputChannelChoice(
//!   DEV_DATA_OUTPUT_CHANNEL_CHOICE_E eChannelChoice
//!    )
//! \param  eChannelChoice:  channel choice
//! \return return operation result \sa AUDIO_RESULT_E.
//! \brief  Description: This function is used to set channel choice when device  
//!                         output data.
//! \author Author: Shujing.Dong
//! \note   Note: This operation is synchronous.    
/*****************************************************************************/ 
PUBLIC AUDIO_RESULT_E AUDIO_DM_SetDataOutputChannelChoice(
    DEV_DATA_OUTPUT_CHANNEL_CHOICE_E eChannelChoice
    );

/*****************************************************************************/
//! \fn PUBLIC AUDIO_RESULT_E AUDIO_DM_GetDataOutputChannelChoice(
//!   DEV_DATA_OUTPUT_CHANNEL_CHOICE_E *peChannelChoice
//!    )
//! \param  peChannelChoice:  the pointer of channel choice
//! \return return operation result \sa AUDIO_RESULT_E.
//! \brief  Description: This function is used to get channel choice when device  
//!                         output data.
//! \author Author: Shujing.Dong
//! \note   Note: This operation is synchronous.    
/*****************************************************************************/ 
PUBLIC AUDIO_RESULT_E AUDIO_DM_GetDataOutputChannelChoice(
    DEV_DATA_OUTPUT_CHANNEL_CHOICE_E *peChannelChoice
    );
    
/*****************************************************************************/
//! \fn PUBLIC uint32 AUDIO_DM_GetRemainedPcmCount(HAUDIODEV hDevice)
//!    
//! \param  hDevice The operated device.
//! \return the count of output pcm data which is still remained in the device in 
//!         byte unit .
//! \brief  Description: This function is used to get  the count of output pcm  
//!         data which is still remained in the device in byte unit ..
//! \author Author: peter.huang
//! \note   Note: None.      
/*****************************************************************************/
PUBLIC uint32 AUDIO_DM_GetRemainedPcmCount(HAUDIODEV hDevice);

/*****************************************************************************/
//! \fn PUBLIC uint32 AUDIO_DM_GetRemainedPcmTime(HAUDIODEV hDevice)
//!    
//! \param  hDevice The operated device.
//! \return the play time of output pcm data which is still remained in the 
//!         device in byte unit .
//! \brief  Description: This function is used to get the play time of output 
//!         pcm data which is still remained in the device.
//! \author Author: shujing.dong
//! \note   Note: None.      
/*****************************************************************************/
PUBLIC uint32 AUDIO_DM_GetRemainedPcmTime(HAUDIODEV hDevice);

/*****************************************************************************/
//! \fn PUBLIC AUDIO_RESULT_E AUDIO_DM_SetDigitalGain( 
//!    uint32  uiGain 
//!    )
//! \param uiGain, digital gain value to be set.
//! \return AUDIO_RESULT_E, generally return AUDIO_NO_ERROR
//! \brief  Description: This function is used to set digital gain value.
//! \author Author: Johnson.sun
//! \note   Note: None.      
/*****************************************************************************/
PUBLIC AUDIO_RESULT_E AUDIO_DM_SetDigitalGain(
     uint32  uiGain
    );

/*****************************************************************************/
//! \fn PUBLIC AUDIO_RESULT_E AUDIO_DM_GetDigitalGain( 
//!    uint32  *ptGain
//!    )
//! \param ptGain, current digital gain return value through the pointer.
//! \return AUDIO_RESULT_E, generally return AUDIO_NO_ERROR
//! \brief  Description: This function is used to get current digital gain value.
//! \author Author: Johnson.sun
//! \note   Note: None.      
/*****************************************************************************/
PUBLIC AUDIO_RESULT_E AUDIO_DM_GetDigitalGain(
     uint32  *puiGain
    );

/*****************************************************************************/
//! \fn PUBLIC uint32 AUDIO_DM_GetAppropriateOutputSamplerate(
//!       HAUDIODEV hDevice,
//!       uint32 uiSampleRate    
//!       )
//! \param  hDevice The operated device.
//! \param  uiSampleRate original output samplerate.
//! \return the appropriate output samplerate.
//! \brief  Description: This function is used to get the appropriate output samplerate.
//! \author Author: peter.huang
//! \note   Note: None.      
/*****************************************************************************/
PUBLIC uint32 AUDIO_DM_GetAppropriateOutputSamplerate(
    HAUDIODEV hDevice,
    uint32 uiSampleRate    
    );    

/*****************************************************************************/
//! \fn PUBLIC uint32 AUDIO_DM_GetOutputSamplerate(
//!       HAUDIODEV hDevice
//!       )
//! \param  hDevice The operated device.
//! \return the current output samplerate.
//! \brief  Description: This function is used to get the current output samplerate.
//! \author Author: peter.huang
//! \note   Note: None.      
/*****************************************************************************/
PUBLIC uint32 AUDIO_DM_GetOutputSamplerate(
    HAUDIODEV hDevice
    );

PUBLIC AUDIO_RESULT_E AUDIO_DM_SetNotchToneEn(uint32 enable);
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/    
#ifdef __cplusplus
}
#endif

#endif  // _ADM_H

// End of adm.h


