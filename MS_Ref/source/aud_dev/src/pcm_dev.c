/******************************************************************************
 ** File Name:      pcm_dev.c                                                                *
 ** Author:         Benjamin.Wang                                             *
 ** DATE:           10/04/2006                                                *
 ** Copyright:      2006 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:    PCM audioservice adapter layer.* 
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
#include "ms_ref_aud_dev_trc.h"
#include "os_api.h"
#include "audio_config.h"
#include "aud_gen.h"
#include "adm.h"
#include "adm_dp.h"
#include "chng_freq.h"
#include "pcm_drv.h"
#include "pcm_dev.h"
#include "deep_sleep.h"
#include "tasks_id.h"
#include "chip.h"
#include "gpio_drv.h"
#include "sci_api.h"
#include "deepsleep_drvapi.h"
#include "apm_express.h"
#include "track_mixer.h"

#ifdef DSP_AUDIO_III
#include "admm_III.h"
#else
#include "admm_II.h"
#endif
#include "asm.h"

#define _PCM_DEV_DEBUG_  0

#if _PCM_DEV_DEBUG_
const uint8 g_pcm_audio_dat[] =
#include "美丽的神话_mono_8khz_16bit.dat"
//#include "test_data.dat"
static uint32 g_pcm_data_len = sizeof(g_pcm_audio_dat)/sizeof(uint8);
#endif

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
#define PCM_MAX_VALUE (32767)
#define PCM_MIN_VALUE (-32767)

#define PCM_OUTPUT_SMAPLE_RATE 8000//unit:hz.

#define PCM_MAX_CARRYING (2)

#define HFG_MAX_BUF_SIZE 6400  /*for bt mp3 over hfg*/

#if !defined(PLATFORM_SC6530) && !defined(PLATFORM_SC6531EFM) && !defined(PLATFORM_UWS6121E)
#ifdef FIFO_WATER_MARK_LOW
#define FIFO_FULL_WATERMARK  0x0A
#define FIFO_EMPTY_WATERMARK  0x06
#else    
#define FIFO_FULL_WATERMARK  24
#define FIFO_EMPTY_WATERMARK  8
#endif 
#endif

#define PCM_FETCH_LEN_MIN 160
#define OUTPUT_SAMPLE_MAX (PCM_FETCH_LEN_MIN*2)

#define PCMDEV_RESERVER_VOL_LEVEL  15

/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/
 
/**---------------------------------------------------------------------------*
 **                         Global Variables                                  *
 **---------------------------------------------------------------------------*/
LOCAL AUDIO_DEVICE_HANDLE_T* g_pcm_device_ptr;
uint32 s_pcm_on_count = 0;
LOCAL ADM_RECCALLBACK s_pcm_callback_list[PCM_MAX_CARRYING];  // default NULL
LOCAL uint32 s_pcm_chng_freq_req_handler = CHNG_FREQ_REQ_HANDLER_NULL;


LOCAL BOOLEAN sbIfAddDigitalGainExp = SCI_FALSE;
LOCAL AUDIO_OBJECT_T* ptLastPlayAudioObject = PNULL;

LOCAL int32 *s_pcm_da0_buffer_multitrack;
LOCAL int32 *s_pcm_da1_buffer_multitrack;
LOCAL int16 *s_pcm_da0_buffer;
#if defined(BLUETOOTH_SUPPORT) && defined(BLUETOOTH_SHARE_MEMORY) 
extern void BLUE_SendData (uint8 *data_ptr,uint32 data_len);
extern void  BLUE_DevOpen(PCM_PARA_T *paramer);
extern uint8 BLUE_DevClose(void);
extern int32 BLUE_ReceiveData(uint8* buff_ptr, uint32 data_len);
#endif

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/
 
/**---------------------------------------------------------------------------*
 **                     Local Function Prototypes                             *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    Send data to PCM output fifo.
//  Author:         Benjamin.Wang
//  Note:           
//****************************************************************************/
LOCAL ERR_PCM_E PCMDEV_Put_Data_To_FiFo(CALL_RET_T ret);

/*****************************************************************************/
//  Description:    Open pcm device.
//  Author:         Thomas.Chai
//  Note:
//****************************************************************************/
LOCAL AUDIO_RESULT_E PCMDEV_Play(uint32 uiSampleRate, DEV_SETTING_T *ptDevSetting);

/*****************************************************************************/
//  Description:    Close pcm device.
//  Author:         Benjamin.Wang
//  Note:
//****************************************************************************/
LOCAL AUDIO_RESULT_E PCMDEV_Stop(DEV_SETTING_T *ptDevSetting);

/*****************************************************************************/
//  Description:    Set device's samplerate.
//  Author:         Benjamin.Wang
//  Note:
//****************************************************************************/
LOCAL void PCMDEV_SetSampleRate(uint32 uiSampleRate);

/*****************************************************************************/
//  Description:    Start record operation.
//  Author:         Benjamin.Wang
//  Note:
//****************************************************************************/
LOCAL void PCMDEV_StartRecord(
    uint32 uiSampleRate 
    );

/*****************************************************************************/
//  Description:    Stop record operation.
//  Author:         Benjamin.Wang
//  Note:
//****************************************************************************/
LOCAL void PCMDEV_StopRecord(void);

/*****************************************************************************/
//  Description:    Register record callback function. When the device got enough data, it will 
//                       call the registered callback function to notify clients to do data operation.
//  Author:         Benjamin.Wang
//  Note:
//****************************************************************************/
LOCAL uint32 PCMDEV_RegRecCallback(
    ADM_RECCALLBACK pCallback
    );

/*****************************************************************************/
//  Description:    Unregister record callback function. When the device got enough data, it will 
//                       call the registered callback function to notify clients to do data operation.
//  Author:         Benjamin.Wang
//  Note:
//****************************************************************************/
LOCAL void PCMDEV_UnRegRecCallback(
    uint32 uiRegVector
    );


/*****************************************************************************/
//  Description:    Read data from pcm input buffer.
//  Author:         Benjamin.Wang
//  Note:
//****************************************************************************/
LOCAL void PCMDEV_ReadData(
    uint16* puiDestData, 
    uint32 uiCount,
    uint32 *puiDataLen
    );
 
/*****************************************************************************/
//  Description:    PCM device (Bluetooth) Set device mode.
//  Author:         Johnson.sun
//  Note:  
//****************************************************************************/
LOCAL void PCMDEV_SetDevMode(AUDIO_DEVICE_MODE_TYPE_E eMode);

/*****************************************************************************/
//  Description:    get appropriate output samplerate.
//  Author:         Shujing.Dong
//  Note:
//****************************************************************************/
LOCAL uint32 PCMDEV_GetAppropriateOutputSamplerate(uint32 uiSampleRate);
    
/**---------------------------------------------------------------------------*
 **                         Function Definitions                              *
 **---------------------------------------------------------------------------*/
LOCAL void PcmDev_SetVolumeType (uint32  uiVol, AUDIO_ARM_APP_TYPE_E eAppType);
LOCAL void PcmDev_SetDownlinkMute (BOOLEAN is_mute);

LOCAL char * _Pcm_ArmvbNv_GetName(AUDIO_DEVICE_MODE_TYPE_E aud_dev_mode)
{
    NV_AUDIO_ARM_E arm_nv_id = NV_AUDIO_ARM_HANDSET_E;
    switch(aud_dev_mode)
    {
        case      AUDIO_DEVICE_MODE_HANDHOLD:
        arm_nv_id = NV_AUDIO_ARM_HANDSET_E;
        break;
        case      AUDIO_DEVICE_MODE_HANDFREE:
        arm_nv_id = NV_AUDIO_ARM_HANDSFREE_E;
        break;
        case      AUDIO_DEVICE_MODE_EARPHONE:
        arm_nv_id = NV_AUDIO_ARM_HEADSET_E;
        break;
        case      AUDIO_DEVICE_MODE_EARFREE:
        arm_nv_id = NV_AUDIO_ARM_EARFREE_E;
        break;
        case      AUDIO_DEVICE_MODE_TVOUT:
        arm_nv_id = NV_AUDIO_ARM_HEADSET_E;
        break;
        case      AUDIO_DEVICE_MODE_BLUEPHONE:
        arm_nv_id = NV_AUDIO_ARM_BTHS_E;
        break;
        default:
        arm_nv_id = NV_AUDIO_ARM_HANDSFREE_E;
        break;
    }
    return AUDIONVARM_GetAudioModeNameById(arm_nv_id);
} 
 
/*****************************************************************************/
//  Description:    Register pcm device to audio service.
//  Author:         Benjamin.Wang
//  Note:
//****************************************************************************/
PUBLIC HAUDIODEV PCMDEV_RegAudioDevice(void)
{
    AUDIO_DEVICE_T tDeviceInfo = {0};
    HAUDIODEV hDevice = INVALID_HANDLE;
    
    PCM_Init();
    
    SCI_MEM16CPY((uint16*)tDeviceInfo.usDeviceName, ANSI2UINT16("PCMDEV"), strlen("PCMDEV") * 2 + 2);
    tDeviceInfo.bIsNeedTrack = SCI_TRUE;
    tDeviceInfo.uiMinFetchLength = PCM_FETCH_LEN_MIN;
    tDeviceInfo.ucDevOpenCnt = 0;
    tDeviceInfo.tAudioDevOpe.pOpen = PCMDEV_Play;
    tDeviceInfo.tAudioDevOpe.pClose = PCMDEV_Stop;
    tDeviceInfo.tAudioDevOpe.pPause = PCMDEV_Stop;
    tDeviceInfo.tAudioDevOpe.pResume = PCMDEV_Play;
    tDeviceInfo.tAudioDevOpe.pSetSamplerate = PCMDEV_SetSampleRate;
    tDeviceInfo.tAudioDevOpe.pStartRecord = PCMDEV_StartRecord;
    tDeviceInfo.tAudioDevOpe.pStopRecord = PCMDEV_StopRecord;
    tDeviceInfo.tAudioDevOpe.pRegRecCallback = PCMDEV_RegRecCallback;
    tDeviceInfo.tAudioDevOpe.pUnRegRecCallback = PCMDEV_UnRegRecCallback;
    tDeviceInfo.tAudioDevOpe.pReadData = PCMDEV_ReadData;
    tDeviceInfo.tAudioDevOpe.pSetDevMode = PCMDEV_SetDevMode;
    tDeviceInfo.tAudioDevOpe.pSetVolume = PcmDev_SetVolumeType;
    tDeviceInfo.tAudioDevOpe.pSetUplinkMute = SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pSetDownlinkMute = PcmDev_SetDownlinkMute;
    tDeviceInfo.tAudioDevOpe.pSetDepopMute=SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pSetDigitalGain = SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pGetRemainedPcmCount=SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pGetRemainedPcmTime=SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pGetAppropriateOutputSamplerate = PCMDEV_GetAppropriateOutputSamplerate;

    hDevice = AUDIO_DM_RegAudioDevice(&tDeviceInfo);
    g_pcm_device_ptr = AUDIO_DM_GetDeviceHandleStruct(hDevice);   // Record device structure
    g_pcm_device_ptr->uiCurrentSamplerate = PCM_OUTPUT_SMAPLE_RATE;

    return hDevice;
}

/**---------------------------------------------------------------------------*
 **                         Local Function Definitions                              *
 **---------------------------------------------------------------------------*/


/*****************************************************************************/
//  Description:    add digital gain express to codecs or delete digital gain 
//                  express from codecs. The codecs are the ones included in the
//                  audio objects using the current logic device: a2dp device.
//                  SCI_TRUE: add, SCI_FALSE: delete.
//  Author:         shujing.dong
//  Note:
//****************************************************************************/ 
LOCAL void _PcmDev_EnableDigitalGain(BOOLEAN bEnable)
{
#ifdef  DIGITAL_GAIN_SUPPORT
    HAUDIOCODEC hCurCodec = INVALID_HANDLE;
    
    if(PNULL!=g_pcm_device_ptr)
    {
        if(bEnable)
        {
            ptLastPlayAudioObject = AUDIO_SM_GetAudioObjectOfLastPlay();
            if(PNULL!=ptLastPlayAudioObject)
            {
                hCurCodec = ptLastPlayAudioObject->hAudioCodec;
                if(!AUDIO_PM_IsCodecIncludeExp(hCurCodec, hDigitalGainExp))
                        {
                            AUDIO_PM_AddCodecExpress (hCurCodec, hDigitalGainExp);
                            //SCI_TRACE_LOW:"_PcmDev_EnableDigitalGain add express:0x%x to codec:0x%x."
                            SCI_TRACE_ID(TRACE_TOOL_CONVERT,PCM_DEV_255_112_2_18_1_2_19_182,(uint8*)"dd",hDigitalGainExp, hCurCodec);
                            sbIfAddDigitalGainExp = SCI_TRUE;
                        }
            }
        }
        else
        {
            if(PNULL!=ptLastPlayAudioObject)
            {
                hCurCodec = ptLastPlayAudioObject->hAudioCodec;
                if(sbIfAddDigitalGainExp && (AUDIO_PM_IsCodecIncludeExp(hCurCodec, hDigitalGainExp)))
                        {
                            AUDIO_PM_DeleteCodecExpress (hCurCodec, hDigitalGainExp);
                            //SCI_TRACE_LOW:"_PcmDev_EnableDigitalGain delete express:0x%x from codec:0x%x."
                            SCI_TRACE_ID(TRACE_TOOL_CONVERT,PCM_DEV_269_112_2_18_1_2_19_183,(uint8*)"dd",hDigitalGainExp, hCurCodec);
                            sbIfAddDigitalGainExp = SCI_FALSE;
                        }                
            }            
        }        
    }  
#endif    
}



/*****************************************************************************/
//  Description:    a2dp logic device set device volume. 
//  Author:         Shujing.Dong
//  Note:           adjust volume by adjust digital gain
//****************************************************************************/
LOCAL void PcmDev_SetVolumeType (uint32  uiVol, AUDIO_ARM_APP_TYPE_E eAppType)
{
#ifdef DIGITAL_GAIN_SUPPORT
    uint32 gain = 0;
    int32  dg_level = 0;
    AUDIO_DEVICE_MODE_TYPE_E    cur_dev_mode = AUDIO_DEVICE_MODE_BLUEPHONE;
    char *mode_name = NULL;
    AUDIO_NV_ARM_MODE_INFO_T *ptAudioInfo = PNULL;
    uint32 index_base = 0;

    //SCI_TRACE_LOW:"PcmDev_SetVolumeType vol:%d, appType:%d."
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,PCM_DEV_294_112_2_18_1_2_19_184,(uint8*)"dd", uiVol, eAppType);
    
    if(uiVol>PCMDEV_RESERVER_VOL_LEVEL)
    {
        //SCI_TRACE_LOW:"PcmDev_SetVolumeType volume level is too big(%d), will be adjusted to be:%d."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,PCM_DEV_299_112_2_18_1_2_19_185,(uint8*)"dd", uiVol, (PCMDEV_RESERVER_VOL_LEVEL));
        uiVol = PCMDEV_RESERVER_VOL_LEVEL;//max volume level: PCMDEV_RESERVER_VOL_LEVEL-1
    }
    else if(uiVol<1)
    {
        uiVol = 1;
    }

     mode_name = _Pcm_ArmvbNv_GetName(cur_dev_mode);
    if(PNULL==mode_name)
    {
        //SCI_TRACE_LOW:"PcmDev_SetVolumeType filed to get modeName:%d."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,PCM_DEV_306_112_2_18_1_2_19_186,(uint8*)"d", cur_dev_mode);
        return;
    }
    ptAudioInfo = (AUDIO_NV_ARM_MODE_INFO_T *)AUDIO_ALLOC(sizeof(AUDIO_NV_ARM_MODE_INFO_T));
    if(PNULL==ptAudioInfo)
    {
        //SCI_TRACE_LOW:"PcmDev_SetVolumeType, alloc fail, size: %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,PCM_DEV_312_112_2_18_1_2_19_187,(uint8*)"d",sizeof(AUDIO_NV_ARM_MODE_INFO_T));
        return;
    }
    
    if(AUDIO_NV_ARM_NO_ERROR != AUDIONVARM_GetModeParam((const char *) mode_name, (AUDIO_NV_ARM_MODE_INFO_T *)(ptAudioInfo)))
    {
        //SCI_TRACE_LOW:"PcmDev_SetVolumeType failed to get mode param, mode:%d, modeName:%s."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,PCM_DEV_319_112_2_18_1_2_19_188,(uint8*)"ds",cur_dev_mode, mode_name);
        SCI_FREE(ptAudioInfo);
        return; 
    }  

    //SCI_TRACE_LOW:"PcmDev_SetVolumeType uiVol=%d, mode:%d, appType:%d, modeName:%s."
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,PCM_DEV_325_112_2_18_1_2_19_189,(uint8*)"ddds", uiVol, cur_dev_mode, eAppType, mode_name);

    index_base = AUDIO_NV_ARM_PARA_RESERVE - PCMDEV_RESERVER_VOL_LEVEL;

    SCI_PASSERT(((index_base+uiVol)<=AUDIO_NV_ARM_PARA_RESERVE),    /*assert verified*/
        ("PCMDEV_SetVolumeType overflow, indexbase:%d, vol:%d, indexMax:%d",
        index_base, uiVol, (AUDIO_NV_ARM_PARA_RESERVE)));
    gain = ptAudioInfo->tAudioNvArmModeStruct.reserve[index_base+uiVol-1]; // 0:mute, 1-15 corresponding to volume level 1-15;

    gain =  gain & 0x7F; //Last 7Bit
    
    dg_level = (int32) ( ( (int32) gain << 25) >> 25); // 7Bit is the flag bit: signed

    //SCI_TRACE_LOW:"PcmDev_SetVolumeType gain:0x%4x, dg_level= 0x%4x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,PCM_DEV_338_112_2_18_1_2_19_190,(uint8*)"dd", gain, dg_level);

    {
        AUDIO_PM_ExeExpSetParaOpe(hDigitalGainExp, PNULL, dg_level, PNULL);
    }

    SCI_FREE(ptAudioInfo);
#endif
    return;
}

LOCAL void PcmDev_SetDownlinkMute (
    BOOLEAN is_mute
)
{
#ifdef DIGITAL_GAIN_SUPPORT
    uint32  uiVol = 0;
    AUDIO_ARM_APP_TYPE_E eAppType = AUDIO_ARM_APP_TYPE_0;
    AUDIO_GetVolumeAndType((uint32 *)&uiVol, (AUDIO_ARM_APP_TYPE_E *)&eAppType);

    //SCI_TRACE_LOW:"PcmDev_SetDownlinkMute, mute:%d, vol:%d, appType:%d."
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,PCM_DEV_359_112_2_18_1_2_20_191,(uint8*)"ddd",is_mute, uiVol, eAppType);
        
    if(is_mute)
    {
        PcmDev_SetVolumeType(0, eAppType);
    }
    else
    {
        PcmDev_SetVolumeType(uiVol, eAppType);
    }
#endif    
}

 
/*****************************************************************************/
//  Description:    Send data to PCM , PCM will output these data to bt codec
//  Author:         thomas.chai
//  Note:           The data transmitting speed of lay1 is faster than pcm, the 
//                  global buffer between lay1 and pcm will be always full, so
//                  when the whole buffer is full should throw away some data.
//****************************************************************************/
LOCAL ERR_PCM_E PCMDEV_Put_Data_To_FiFo(CALL_RET_T ret)
{    
#if _PCM_DEV_DEBUG_
   PCM_SendData((uint8*)g_pcm_audio_dat,g_pcm_data_len);
#else
        BOOLEAN bEnable = FALSE;
        AUDIO_GetDownLinkMute(&bEnable);
    _Track_Mixer(g_pcm_device_ptr->ptTrackList,
                  s_pcm_da0_buffer_multitrack,
                  s_pcm_da1_buffer_multitrack,
                  s_pcm_da0_buffer,
                  SCI_NULL,
                  PCM_FETCH_LEN_MIN,
                  OUTPUT_SAMPLE_MAX/PCM_FETCH_LEN_MIN,
                      SCI_TRUE,
                      bEnable);
#if defined(BLUETOOTH_SUPPORT) && defined(BLUETOOTH_SHARE_MEMORY) 
    BLUE_SendData((uint8 *)s_pcm_da0_buffer, (OUTPUT_SAMPLE_MAX)*sizeof(uint16));
#else
    PCM_SendData((uint8 *)s_pcm_da0_buffer, (OUTPUT_SAMPLE_MAX)*sizeof(uint16)); 
#endif

#endif
   return ERR_PCM_SUCCESS;
}

/*****************************************************************************/
//  Description:    PCM Tx call back function
//  Author:         Thomas.Chai
//  Note:
//****************************************************************************/
LOCAL ERR_PCM_E PCM_Tx_Notify(CALL_RET_T ret)  
{
   return PCMDEV_Put_Data_To_FiFo(ret);
}

/*****************************************************************************/
//  Description:    PCM Rx call back function
//  Author:         Thomas.Chai
//  Note:
//****************************************************************************/
LOCAL ERR_PCM_E PCM_Rx_Notify(CALL_RET_T ret)
{
   if(s_pcm_callback_list[0] != SCI_NULL)
   {
     s_pcm_callback_list[0](ret.value);
   }
   return ERR_PCM_SUCCESS;
}

LOCAL void PCM_SetPinFunc(BOOLEAN isIIS)
{
    
}


LOCAL void PCM_SetPinDriver(BOOLEAN isStrong)
{

}


/*****************************************************************************/
//  Description:    Open pcm device.
//  Author:         Thomas.Chai
//  Note:
//****************************************************************************/
LOCAL AUDIO_RESULT_E PCMDEV_Play(uint32 uiSampleRate, DEV_SETTING_T *ptDevSetting)
{
    PCM_PARA_T pcm_param = {
        SHORT_FRAME_SYNC_MODE,//pcm operate mode.(sc6600m only support short frame sync mode
        SLAVE_MODE, //Master/Slave mode.0:master 1:slave
        TX_16BITS_PER_FRAME,//TX_16BITS_PER_WORD, //transmit bit number per channel
        PCM_SLOT0,              
        FIFO_EMPTY_WATERMARK,
        FIFO_FULL_WATERMARK,
        PCM_Tx_Notify,
        PCM_Rx_Notify,
    };
    
    if(s_pcm_on_count > 0)    
    {
        SCI_ASSERT(0);/*assert verified*/
    }
    //SCI_TRACE_LOW:"====PCM_Open====="
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,PCM_DEV_535_112_2_18_1_2_20_192,(uint8*)"");
    s_pcm_da0_buffer_multitrack = SCI_ALLOC_APP(PCM_FETCH_LEN_MIN*sizeof(int32));
    s_pcm_da1_buffer_multitrack = SCI_ALLOC_APP(PCM_FETCH_LEN_MIN*sizeof(int32));
    s_pcm_da0_buffer = SCI_ALLOC_APP(OUTPUT_SAMPLE_MAX*sizeof(int16));


    //SCI_TRACE_LOW:"[PCMDEV_Play] IN (s_pcm_on_count = %d)"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,PCM_DEV_537_112_2_18_1_2_20_193,(uint8*)"d", s_pcm_on_count);

    if (CHNG_FREQ_REQ_HANDLER_NULL == s_pcm_chng_freq_req_handler)
    {
        s_pcm_chng_freq_req_handler =  CHNG_FREQ_GetReqHandler("pcm");
        SCI_ASSERT (CHNG_FREQ_REQ_HANDLER_NULL != s_pcm_chng_freq_req_handler);/*assert verified*/
    }
    PCM_SetPinFunc(TRUE);
    PCM_SetPinDriver(TRUE);
    //Disable  PCM Sleep
    SCI_PCM_EnableDeepSleep(DISABLE_DEEP_SLEEP);

    CHNG_FREQ_SetArmClk(s_pcm_chng_freq_req_handler,FREQ_INDEX_PCM_BLUE_PLAY);  

    AUD_SetOutputDataType(AUDIO_TRANPCM_TYPE);       
    AUD_Cfg_PcmTrans_Param(PCM_OUTPUT_SMAPLE_RATE);
            
#if defined(BLUETOOTH_SUPPORT) && defined(BLUETOOTH_SHARE_MEMORY) 
    BLUE_DevOpen(&pcm_param);
#else
    PCM_Open(&pcm_param);
#endif
    
    s_pcm_on_count++;
    _PcmDev_EnableDigitalGain(SCI_TRUE);
    PCMDEV_SetDevMode(ptDevSetting->eDev_mode);

     if(ptDevSetting->bDownlink_mute)
    {
        PcmDev_SetVolumeType(0, ptDevSetting->eAppType);
    }
    else
    {
        PcmDev_SetVolumeType(ptDevSetting->uiVol, ptDevSetting->eAppType);
    }

    return AUDIO_NO_ERROR;
}

/*****************************************************************************/
//  Description:    Close pcm device.
//  Author:         Benjamin.Wang
//  Note:
//****************************************************************************/
LOCAL AUDIO_RESULT_E PCMDEV_Stop(DEV_SETTING_T *ptDevSetting)
{
#if defined(BLUETOOTH_SUPPORT) && defined(BLUETOOTH_SHARE_MEMORY) 
     uint8 result = 0;
#endif
    if(s_pcm_on_count > 0)
    {
        s_pcm_on_count--;    
    }
    
    if(s_pcm_on_count == 0)
    {

#if defined(BLUETOOTH_SUPPORT) && defined(BLUETOOTH_SHARE_MEMORY) 

         result = BLUE_DevClose();/*lint -e746*/
#else
         //PCM Close

         PCM_Close();

         PCM_SetPinFunc(FALSE);
         PCM_SetPinDriver(FALSE);



#endif //if defined(BLUETOOTH_SUPPORT_SPRD_V0)
         //Enable  PCM Sleep
         SCI_PCM_EnableDeepSleep(ENABLE_DEEP_SLEEP);
         //Recover Arm Clk
         CHNG_FREQ_RestoreARMClk(s_pcm_chng_freq_req_handler); 
         CHNG_FREQ_DeleteReqHandler(s_pcm_chng_freq_req_handler);
         s_pcm_chng_freq_req_handler = CHNG_FREQ_REQ_HANDLER_NULL;


    }
    
    _PcmDev_EnableDigitalGain(SCI_FALSE);  
    SCI_FREE(s_pcm_da0_buffer_multitrack);
    SCI_FREE(s_pcm_da1_buffer_multitrack);
    SCI_FREE(s_pcm_da0_buffer);
    return AUDIO_NO_ERROR;
}

/*****************************************************************************/
//  Description:    Set device's samplerate.
//  Author:         Benjamin.Wang
//  Note:
//****************************************************************************/
LOCAL void PCMDEV_SetSampleRate(uint32 uiSampleRate)
{
    
}

/*****************************************************************************/
//  Description:    Start record operation.
//  Author:         Benjamin.Wang
//  Note:
//****************************************************************************/
LOCAL void PCMDEV_StartRecord(
    uint32 uiSampleRate 
    )
{
    //PCMDEV_Play(uiSampleRate);
}

/*****************************************************************************/
//  Description:    Stop record operation.
//  Author:         Benjamin.Wang
//  Note:
//****************************************************************************/
LOCAL void PCMDEV_StopRecord(void)
{
    //PCMDEV_Stop();
}

/*****************************************************************************/
//  Description:    Register record callback function. When the device got enough data, it will 
//                       call the registered callback function to notify clients to do data operation.
//  Author:         Benjamin.Wang
//  Note:
//****************************************************************************/
LOCAL uint32 PCMDEV_RegRecCallback(
    ADM_RECCALLBACK pCallback
    )
{
    int32 i = 0;

    if(SCI_NULL == pCallback)
    {
        ASM_PASSERT_WARNING((pCallback != SCI_NULL),   
            ("PCM: Input Parameter(pCallback) should not be NULL!"));
        return (uint32)SCI_NULL;
    }

    // Find the vacant position and save it.
    for(i = 0; i < PCM_MAX_CARRYING; i++)
    {
        if(s_pcm_callback_list[i] == SCI_NULL)
        {
            s_pcm_callback_list[i] = pCallback;
            return (uint32)&s_pcm_callback_list[i];
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
LOCAL void PCMDEV_UnRegRecCallback(
    uint32 uiRegVector
    )
{
    int32 i = 0;

    // Find the occupied position by callback function and clear it.
    for(i = 0; i < PCM_MAX_CARRYING; i++)
    {
        if((uint32)&s_pcm_callback_list[i] == uiRegVector)
        {
            s_pcm_callback_list[i] = SCI_NULL;
            break;
        }
    }

    // Can't find the occupied position.
    return;
}

/*****************************************************************************/
//  Description:    Read data from pcm input buffer.
//  Author:         Benjamin.Wang
//  Note:
//****************************************************************************/
LOCAL void PCMDEV_ReadData(uint16* puiDestData, uint32 uiCount, uint32 *puiDataLen )
{
#if defined(BLUETOOTH_SUPPORT) && defined(BLUETOOTH_SHARE_MEMORY) 
  *puiDataLen =  BLUE_ReceiveData((uint8*)puiDestData, uiCount*sizeof(uint16));
#else
  *puiDataLen =  PCM_ReceiveData((uint8*)puiDestData, uiCount*sizeof(uint16));
#endif
  return;
}

/*****************************************************************************/
//  Description:    PCM device (Bluetooth) Set device mode.
//  Author:         Johnson.sun
//  Note:  
//****************************************************************************/
LOCAL void PCMDEV_SetDevMode(AUDIO_DEVICE_MODE_TYPE_E eMode)
{    
    //SCI_TRACE_LOW:"PCMDEV_SetDevMode:%d."
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,PCM_DEV_709_112_2_18_1_2_20_194,(uint8*)"d", eMode);
}

/*****************************************************************************/
//  Description:    get appropriate output samplerate.
//  Author:         Shujing.Dong
//  Note:
//****************************************************************************/
LOCAL uint32 PCMDEV_GetAppropriateOutputSamplerate(uint32 uiSampleRate)
{
    return PCM_OUTPUT_SMAPLE_RATE;
}

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
    
#endif  // End of pcm_dev.c


