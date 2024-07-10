/******************************************************************************
 ** File Name:      l1rec_dev.c                                                                *
 ** Author:         Benjamin.Wang                                             *
 ** DATE:           10/04/2006                                                *
 ** Copyright:      2006 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:    We abstract layer1(DSP) recorder as an record device and 
 **                     match it with audio service device manager.     * 
 **                                                                           *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 10/04/2006     Benjamin.Wang    Create.                                   *
 ******************************************************************************/
  
/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/ 
#include "sci_types.h"
#include "sci_api.h"
#include "aud_gen.h"
#include "audio_api.h"
#include "adm.h"
#include "layer1_audio.h"
#include "audio_config.h"
#include "l1rec_dev.h"

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
#define L1REC_MAX_CARRYING (2)
#define L1REC_DATA_FETCH_SIZE (160)

/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Global Variables                                  *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/
LOCAL ADM_RECCALLBACK s_l1rec_callback_list[L1REC_MAX_CARRYING];  // default NULL
LOCAL AUD_REC_FORMAT_E s_l1rec_format = AUD_REC_PCM;
LOCAL BOOLEAN s_l1rec_is_from_mic = SCI_TRUE;
LOCAL BOOLEAN s_l1rec_is_from_net = SCI_FALSE;

/**---------------------------------------------------------------------------*
 **                     Local Function Prototypes                             *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    Layer1 record data callback function.
//  Author:         Benjamin.Wang
//  Note:
//****************************************************************************/
LOCAL void L1REC_RecordCallback(uint16 usDataLength);

/*****************************************************************************/
//  Description:    Start record operation.
//  Author:         Benjamin.Wang
//  Note:
//****************************************************************************/
LOCAL void L1REC_Start(
    uint32 uiSampleRate 
    );

/*****************************************************************************/
//  Description:    Stop record operation.
//  Author:         Benjamin.Wang
//  Note:
//****************************************************************************/
LOCAL void L1REC_Stop(void);

/*****************************************************************************/
//  Description:    Register record callback function. When the device got enough data, it will 
//                       call the registered callback function to notify clients to do data operation.
//  Author:         Benjamin.Wang
//  Note:
//****************************************************************************/
LOCAL uint32 L1REC_RegRecCallback(
    ADM_RECCALLBACK pCallback
    );

/*****************************************************************************/
//  Description:    Unregister record callback function. When the device got enough data, it will 
//                       call the registered callback function to notify clients to do data operation.
//  Author:         Benjamin.Wang
//  Note:
//****************************************************************************/
LOCAL void L1REC_UnRegRecCallback(
    uint32 uiRegVector
    );

/*****************************************************************************/
//  Description:    Read data from Layer1.
//  Author:         Benjamin.Wang
//  Note:
//****************************************************************************/
LOCAL void L1REC_ReadData(
    uint16* puiDestData, 
    uint32 uiCount,
    uint32 *puiDataLen
    );

/**---------------------------------------------------------------------------*
 **                         Function Definitions                              *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    Register l1rec_dev to audio service.
//  Author:         Benjamin.Wang
//  Note:
//****************************************************************************/
PUBLIC HAUDIODEV L1REC_RegAudioDevice(void)
{
    AUDIO_DEVICE_T tDeviceInfo = {0};
    HAUDIODEV hDevice = INVALID_HANDLE;
    int32 i = 0;
    
    SCI_MEM16CPY((uint16*)tDeviceInfo.usDeviceName, ANSI2UINT16("L1REC"), strlen("L1REC") * 2 + 2);
    tDeviceInfo.bIsNeedTrack = SCI_FALSE;
    tDeviceInfo.uiMinFetchLength = L1REC_DATA_FETCH_SIZE;
    tDeviceInfo.ucDevOpenCnt = 0;
    tDeviceInfo.tAudioDevOpe.pOpen = SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pClose = SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pPause = SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pResume = SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pSetSamplerate = SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pStartRecord = L1REC_Start;
    tDeviceInfo.tAudioDevOpe.pStopRecord = L1REC_Stop;
    tDeviceInfo.tAudioDevOpe.pRegRecCallback = L1REC_RegRecCallback;
    tDeviceInfo.tAudioDevOpe.pUnRegRecCallback = L1REC_UnRegRecCallback;
    tDeviceInfo.tAudioDevOpe.pReadData = L1REC_ReadData;
    tDeviceInfo.tAudioDevOpe.pSetDevMode = SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pSetVolume = SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pSetUplinkMute = SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pSetDownlinkMute = SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pSetDepopMute=SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pSetDigitalGain = SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pGetRemainedPcmCount=SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pGetRemainedPcmTime=SCI_NULL;

    for(i = 0; i < L1REC_MAX_CARRYING; i++)
    {
        s_l1rec_callback_list[i] = SCI_NULL;
    }
    
    return hDevice;
}

/*****************************************************************************/
//  Description:    Config layer1(DSP) record parameters.
//  Author:         Benjamin.Wang
//  Note:
//****************************************************************************/
PUBLIC void L1REC_ConfigRecordDevice(
    AUD_REC_FORMAT_E eRecordFormat, 
    BOOLEAN bIsfromMic, 
    BOOLEAN bIsfromNet
    )
{
    s_l1rec_format = eRecordFormat;
    s_l1rec_is_from_mic = bIsfromMic;
    s_l1rec_is_from_net = bIsfromNet;
}

/**---------------------------------------------------------------------------*
 **                         Local Function Definitions                              *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    Layer1 record data callback function.
//  Author:         Benjamin.Wang
//  Note:
//****************************************************************************/
LOCAL void L1REC_RecordCallback(uint16 usDataLength)
{
    int16 i = 0;

    for(i = 0; i < L1REC_MAX_CARRYING; i++)
    {
        if(s_l1rec_callback_list[i] != SCI_NULL)
        {
            s_l1rec_callback_list[i](usDataLength);
        }
    }
}

/*****************************************************************************/
//  Description:    Start record operation.
//  Author:         Benjamin.Wang
//  Note:
//****************************************************************************/
LOCAL void L1REC_Start(
    uint32 uiSampleRate 
    )
{
    LAYER1_StartRecord(s_l1rec_format, 
        L1REC_RecordCallback, 
        s_l1rec_is_from_mic, 
        s_l1rec_is_from_net);
}

/*****************************************************************************/
//  Description:    Stop record operation.
//  Author:         Benjamin.Wang
//  Note:
//****************************************************************************/
LOCAL void L1REC_Stop(void)
{
    LAYER1_StopRecord();
}
/*****************************************************************************/
//  Description:    Register record callback function. When the device got enough data, it will 
//                       call the registered callback function to notify clients to do data operation.
//  Author:         Benjamin.Wang
//  Note:
//****************************************************************************/
LOCAL uint32 L1REC_RegRecCallback(
    ADM_RECCALLBACK pCallback
    )
{
    int32 i = 0;

    if(SCI_NULL == pCallback)
    {
        //SCI_PASSERT((pCallback != SCI_NULL), /*assert verified*/ //cancel
           // ("L1REC: Input Parameter(pCallback) should not be NULL!"));
        return (uint32)SCI_NULL;
    }

    // Find the vacant position and save it.
    for(i = 0; i < L1REC_MAX_CARRYING; i++)
    {
        if(s_l1rec_callback_list[i] == SCI_NULL)
        {
            s_l1rec_callback_list[i] = pCallback;
            return (uint32)&s_l1rec_callback_list[i];
        }
    }

    // Can't find vacant position.
    return (uint32)SCI_NULL;
}

/*****************************************************************************/
//  Description:    Unregister record callback function. When the device got enough data, it will 
//                       call the registered callback function to notify clients to do data operation.
//  Author:         Benjamin.Wang
//  Note:
//****************************************************************************/
LOCAL void L1REC_UnRegRecCallback(
    uint32 uiRegVector
    )
{
    int32 i = 0;

    // Find the occupied position by callback function and clear it.
    for(i = 0; i < L1REC_MAX_CARRYING; i++)
    {
        if((uint32)&s_l1rec_callback_list[i] == uiRegVector)
        {
            s_l1rec_callback_list[i] = SCI_NULL;
            break;
        }
    }

    // Can't find the occupied position.
    return;
}

/*****************************************************************************/
//  Description:    Read data from Layer1.
//  Author:         Benjamin.Wang
//  Note:
//****************************************************************************/
LOCAL void L1REC_ReadData(
    uint16* puiDestData, 
    uint32 uiCount,
    uint32 *puiDataLen
    )
{
    LAYER1_ReadVoiceData(puiDestData, uiCount);
    *puiDataLen = uiCount;

    return;
}

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
    
#endif  // End of l1rec_dev.c

