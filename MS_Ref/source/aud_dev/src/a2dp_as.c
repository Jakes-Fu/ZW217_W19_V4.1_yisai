#include "ms_ref_bt_pal_csr_trc.h"
#if defined(BT_A2DP_SUPPORT)

/******************************************************************************
 ** File Name:      a2dp_as.c                                                 *
 ** Author:         yuhua.shi                                                 *
 ** DATE:           06/03/2006                                                *
 ** Copyright:      2006 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    This file contail the interface function for audio service* 
 **                                                                           *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 06/03/2006     yuhua.shi        Create.                                   *
 ******************************************************************************/
  
/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/ 
 #if 0
#include "audio_config.h"
#include "aud_gen.h"
#include "adm.h"
#include "adm_dp.h"
#include "os_api.h"

#include "a2dp_as.h"
#include "a2dp_api.h"
#include "track_mixer.h"

#include "apm_express.h"
#include "asm.h"
#include "aud_proc_exp.h"
#include "aud_enha_exp.h"
#include "audio_nv_arm.h"
#endif

//hch
#include "ms_ref_aud_dev_trc.h"
#include "os_api.h"
#include "audio_config.h"
#include "vb_drv.h"
#include "aud_gen.h"
#include "adm.h"
#include "armvb_as.h"

#include "sc_reg.h"

#include "agc_exp.h"
#include "aud_proc_exp.h"
#include "aud_enha_exp.h"
#include "audio_nv_arm.h"
#include "admm_III.h"
#include "audio_drvapi.h"
#include "track_mixer.h"
#include "fm_drv.h"
#include "deepsleep_drvapi.h"
#ifdef AUD_ARM_RECORD_EXP
#include "aud_arm_record_exp.h"
#endif
#include "pmic_26M_clock.h"

#include "bt_abs.h"
#include "rpc_aud.h"
#include "rpc_aud_api.h"
#include "load_modem.h"
#include "hal_aud_zsp.h"



#define A2DP_SBC_ONE_FRAME_LEN                  512
#define A2DP_MAX_OUTPUT_PCM_DATA_LENGTH       (15*A2DP_SBC_ONE_FRAME_LEN)

//this value is related with nv configuration, and is note permitted to change at random.
#define A2DP_RESERVER_VOL_LEVEL 15//(0->14, 0:MUTE, 1->14 volume level 1->14) 
//#define A2DP_FETCH_LEN 512
//#define A2DP_FETCH_BUFSIZE (A2DP_FETCH_LEN*1)
//#define A2DP_FETCH_LEN 32
//#define A2DP_FETCH_BUFSIZE (A2DP_FETCH_LEN)

extern void *SCI_Mem16Cpy(uint16*, const uint16*, uint16);  // defined in os_api.c
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Macro Definition                                  *
 **---------------------------------------------------------------------------*/
#define MULTITRACK_BUFFER_SIZE  (8*1024) 
/**---------------------------------------------------------------------------*
 **                        External types                                 *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/

typedef struct
{
    uint32 in;     /*!<Data in pointer.*/
    uint32 out;    /*!<Data out pointer.*/
    uint32 size;  /*!<Total data count.*/
    int16* data_buffer_ptr; /*!<Left channel buffer pointer.*/
    int16* data_buffer_ptr_r;   /*!<Right channel buffer pointer.*/
}VB_A2DP_PLAYBACK_BUF;
/**---------------------------------------------------------------------------*
 **                         Global Variables                                  *
 **---------------------------------------------------------------------------*/
PUBLIC uint32 g_sbc_src_samplerate=0;
extern uint8 *g_zsp_share_memory_addr;
extern  volatile BOOLEAN  s_is_vb_mute ;

extern SND_ITF_T g_cur_itf ;
extern HAL_AIF_STREAM_T g_aif_stream;
extern int32 g_audio_dev_used ;
extern uint32 g_uiCurrentSamplerate ;
extern HAL_AIF_CH_NB_T g_channelNb ;
extern AUD_LEVEL_T g_aud_level_cfg;
extern SND_SPK_LEVEL_T g_volbeforemute ;
extern uint32 g_shakingMode ;
extern SCI_SEMAPHORE_PTR audio_armvb_semaphore;
extern int32 s_vb_da0_buffer_multitrack[MULTITRACK_BUFFER_SIZE];
extern AUD_AP2CP_PARA_T g_zsp_para;


/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/
 AUDIO_DEVICE_HANDLE_T* s_a2dp_device_ptr;//hch
LOCAL uint32 s_OutputSamplerate=0;
//record if in local file digital gain is added to current codec.
//       SCI_TRUE: yes , SCI_FALSE:no
// if true, then when stop or pause, the codec should to remove it.
LOCAL BOOLEAN sbIfAddDigitalGainExp = SCI_FALSE;
LOCAL AUDIO_OBJECT_T* ptLastPlayAudioObject = PNULL;

VB_A2DP_PLAYBACK_BUF s_a2dp_output_buf;//hch

//LOCAL int32 *s_a2dp_da0_buffer_multitrack, *s_a2dp_da1_buffer_multitrack;



/**---------------------------------------------------------------------------*
 **                     Local Function Prototypes                             *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    a2dp logic device set device volume. 
//  Author:         Shujing.Dong
//  Note:           adjust volume by adjust digital gain
//****************************************************************************/
LOCAL void A2DP_SetVolumeType (uint32  uiVol, AUDIO_ARM_APP_TYPE_E eAppType); 
/**---------------------------------------------------------------------------*
 **                         Function Definitions                              *
 **---------------------------------------------------------------------------*/
PUBLIC void A2DP_GetAudioData(int16 *left_data, int16 *right_data, uint32 data_len);
PUBLIC uint32 A2DP_GetAudioDataCount(void);


/*****************************************************************************/
//  Description:    add digital gain express to codecs or delete digital gain 
//                  express from codecs. The codecs are the ones included in the
//                  audio objects using the current logic device: a2dp device.
//                  SCI_TRUE: add, SCI_FALSE: delete.
//  Author:         shujing.dong
//  Note:
//****************************************************************************/ 
LOCAL void _A2DP_EnableDigitalGain(BOOLEAN bEnable)
{
#ifdef DIGITAL_GAIN_SUPPORT
    HAUDIOCODEC hCurCodec = INVALID_HANDLE;
    
    if(PNULL!=s_a2dp_device_ptr)
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
                    //SCI_TRACE_LOW:"_A2DP_EnableDigitalGain add express:0x%x to codec:0x%x."
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,A2DP_AS_109_112_2_18_1_6_52_192,(uint8*)"dd",hDigitalGainExp, hCurCodec);
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
                    //SCI_TRACE_LOW:"_A2DP_EnableDigitalGain delete express:0x%x from codec:0x%x."
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,A2DP_AS_123_112_2_18_1_6_52_193,(uint8*)"dd",hDigitalGainExp, hCurCodec);
                    sbIfAddDigitalGainExp = SCI_FALSE;
                }                
            }            
        }        
    }  
#endif    
}

void A2DP_PLAYBACK_Callback (void)
{
    AUD_ZSP_SHAREMEM_T *zsp_sharemem = NULL;
    uint32 tick_cnt;
    uint32 data_count;
    uint32 copy_size = 0;
    int channelNb = 0;

    tick_cnt = SCI_GetTickCount();
    SCI_TRACE_LOW("[ljf] file[%s] func[%s] line[%d] 1time:[%d][%d]in", __FILE__, __FUNCTION__, __LINE__, tick_cnt/1000, tick_cnt%1000);
    
    zsp_sharemem = (AUD_ZSP_SHAREMEM_T *)g_zsp_share_memory_addr;
    zsp_sharemem->audInPara.inLenth = AUDIO_INPUT_BUF_SIZE; // 7K
    
    if (g_channelNb == HAL_AIF_MONO)
    {
        channelNb = 0;
    }
    else
    {
        channelNb = 1;
    }

    data_count = RingBufferGetWriteBufferLen(&zsp_sharemem->audInPara, zsp_sharemem->audInPara.inLenth);
    SCI_TRACE_LOW("[xx] file[%s] func[%s] line[%d] data_count = %d", __FILE__, __FUNCTION__, __LINE__,data_count);

    copy_size = _Track_Mixer(s_a2dp_device_ptr->ptTrackList,
                                            s_vb_da0_buffer_multitrack, SCI_NULL/*s_vb_da1_buffer_multitrack*/,
                                            SCI_NULL, SCI_NULL,  data_count/2,   1,  channelNb,  s_is_vb_mute);
    SCI_TRACE_LOW("[xx] file[%s] func[%s] line[%d] copy_size = %d", __FILE__, __FUNCTION__, __LINE__,copy_size);
    
    RingBufferWriteBuffer(&zsp_sharemem->audInPara, 
                                        zsp_sharemem->audInput, 
                                        s_vb_da0_buffer_multitrack, data_count, (copy_size * 2));

    s_a2dp_output_buf.in += copy_size*2;
    if (copy_size < (data_count/2))
    {
        if(_Track_Mixer_End(s_a2dp_device_ptr->ptTrackList))
        {
            zsp_sharemem->audInPara.fileEndFlag = 1;
        }
    }

    tick_cnt = SCI_GetTickCount();
}

/*****************************************************************************/
//  Description:    Open A2DP.
//  Author:         yuhua.shi
//  Note:
//****************************************************************************/
LOCAL AUDIO_RESULT_E A2DP_Play(uint32 uiSampleRate, DEV_SETTING_T *ptDevSetting )
{
	#if 0
    //SCI_TRACE_LOW:"A2DP_Play uiSamleRate = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,A2DP_AS_137_112_2_18_1_6_52_194,(uint8*)"d", uiSampleRate);
//    s_a2dp_da0_buffer_multitrack = SCI_ALLOC_APP(A2DP_FETCH_BUFSIZE*4);
//    s_a2dp_da1_buffer_multitrack = SCI_ALLOC_APP(A2DP_FETCH_BUFSIZE*4);
    s_OutputSamplerate=uiSampleRate;
    A2DP_audioStart(uiSampleRate, A2DP_GetAudioData, A2DP_GetAudioDataCount);
    AUD_SetOutputDataType(AUDIO_SBC_TYPE); 
    _A2DP_EnableDigitalGain(SCI_TRUE); 
    if (ptDevSetting->bDownlink_mute)
    {
        A2DP_SetVolumeType(0, ptDevSetting->eAppType);
    }
    else
    {
        A2DP_SetVolumeType(ptDevSetting->uiVol, ptDevSetting->eAppType);
    }
    
    return AUDIO_NO_ERROR;
	#endif
	AUD_ERR_T ret = 0;
    HAL_ERR_T hal_ret = 0;
    AUD_DEVICE_CFG_EXT_T device_cfg_ext;
    AUD_ZSP_SHAREMEM_T *zsp_sharemem = (AUD_ZSP_SHAREMEM_T *)g_zsp_share_memory_addr;
    uint32 tick_cnt;
    SND_ITF_T cur_itf = SND_ITF_LOUD_SPEAKER;
    SCI_TRACE_LOW("hch,A2DP_Play ARMVB_Play SCI_GetSemaphore....");
    if(SCI_SUCCESS != SCI_GetSemaphore(audio_armvb_semaphore, 10))
    {
        SCI_TRACE_LOW("A2DP_PLAY SCI_GetSemaphore return");
        return AUDIO_CONFLICT;
    }
    SCI_TRACE_LOW("A2DP_Play SCI_GetSemaphore OK");
    tick_cnt = SCI_GetTickCount();
    SCI_TRACE_LOW("[ljf] line[%d] file[%s] func[%s] ptDevSetting->eDev_mode:0x%x g_audio_dev_used:[%d] time[%d][%d]", __LINE__, __FILE__, __FUNCTION__, ptDevSetting->eDev_mode, g_audio_dev_used, tick_cnt/1000, tick_cnt%1000);
    SCI_TRACE_LOW("[zgt] fuc[%s] line[%d] uiSampleRate[%d] vol[%d] ", __FUNCTION__,__LINE__,uiSampleRate,ptDevSetting->uiVol);
    if (1 == g_audio_dev_used)
    {
        SCI_TRACE_LOW("[ljf] Because g_audio_dev_used is [%d], so ARMVB_Play return", g_audio_dev_used);
        return AUDIO_ERROR;
    }
    SCI_AUD_EnableDeepSleep(DISABLE_DEEP_SLEEP);

    switch(ptDevSetting->eDev_mode)
    {
        case AUDIO_DEVICE_MODE_HANDHOLD:
            g_cur_itf = SND_ITF_RECEIVER;
            break;
        case AUDIO_DEVICE_MODE_HANDFREE:
            g_cur_itf = SND_ITF_LOUD_SPEAKER;
            break;
        case AUDIO_DEVICE_MODE_EARPHONE:
            g_cur_itf = SND_ITF_EAR_PIECE;
            break;
        case AUDIO_DEVICE_MODE_EARFREE:
            g_cur_itf = SND_ITF_EAR_PIECE;
            break;
        case AUDIO_DEVICE_MODE_TVOUT:
            g_cur_itf = SND_ITF_TV;
            break;
        case AUDIO_DEVICE_MODE_BLUEPHONE:
            g_cur_itf = SND_ITF_BLUETOOTH;
            break;
        default:
            SCI_TRACE_LOW("[ljf] line[%d] ARMVB_Play eMode is [0x%x] WRONG!!!", __LINE__, ptDevSetting->eDev_mode);
            break;
    }
    g_aif_stream.startAddress = (uint32 *)zsp_sharemem->Output.audOutput;
    g_aif_stream.length = AUDIO_OUTPUT_BUF_SIZE;
    g_uiCurrentSamplerate = uiSampleRate;
    zsp_sharemem->audInPara.sampleRate = uiSampleRate;
    zsp_sharemem->audOutPara.samplerate = uiSampleRate;
    if (ptDevSetting->eChannelChoice == DEV_DATA_OUTPUT_CHANNEL_CHOICE_NORMAL)
    {
        zsp_sharemem->audInPara.channelNb = HAL_AIF_STEREO;
        zsp_sharemem->audOutPara.channelNb = HAL_AIF_STEREO;
    }
    else if ((ptDevSetting->eChannelChoice == DEV_DATA_OUTPUT_CHANNEL_CHOICE_LEFT) || (ptDevSetting->eChannelChoice == DEV_DATA_OUTPUT_CHANNEL_CHOICE_RIGHT))
    {
        zsp_sharemem->audInPara.channelNb = HAL_AIF_MONO;
        zsp_sharemem->audOutPara.channelNb = HAL_AIF_MONO;
    }
    else
    {
        SCI_TRACE_LOW("line[%d] ARMVB_Play ptDevSetting->eChannelChoice:%d", __LINE__, ptDevSetting->eChannelChoice);
    }
    zsp_sharemem->audInPara.bitsPerSample = 16;
#ifdef DECODE_IN_ZSP
#if 0 // AUDIO_CP_USE_ZSP_PARA
    zsp_sharemem->musicMode =2;// 1:PCM mode;2:mp3 stream
#endif
    g_zsp_para.musicMode = 2;
#else
#if 0 // AUDIO_CP_USE_ZSP_PARA
    zsp_sharemem->musicMode =1;
#endif
    g_zsp_para.musicMode = 1;
#endif
    zsp_sharemem->audOutPara.length = 1152*2;

    //!!!can not memset all of zsp sharemem to zero. some status value will be keep with zsp!!! sheen
    //  do not init this para will result in assert but  it should not initialize here for zsp used !zgt
    zsp_sharemem->updateParaInd=0;//sheen
    if (ptDevSetting->eChannelChoice == DEV_DATA_OUTPUT_CHANNEL_CHOICE_NORMAL)
    {
        g_aif_stream.channelNb = HAL_AIF_STEREO;
    }
    else if ((ptDevSetting->eChannelChoice == DEV_DATA_OUTPUT_CHANNEL_CHOICE_LEFT) || (ptDevSetting->eChannelChoice == DEV_DATA_OUTPUT_CHANNEL_CHOICE_RIGHT))
    {
        g_aif_stream.channelNb = HAL_AIF_MONO;
    }
    else
    {
        SCI_TRACE_LOW("line[%d] ARMVB_Play ptDevSetting->eChannelChoice:%d", __LINE__, ptDevSetting->eChannelChoice);
    }
    g_channelNb = g_aif_stream.channelNb;
    g_aif_stream.playSyncWithRecord = 0;
    g_aif_stream.sampleRate = uiSampleRate;
    g_aif_stream.voiceQuality = TRUE;
    g_aif_stream.halfHandler = NULL;
    g_aif_stream.endHandler = NULL;

    //for bug973551 973739 to fix noise in seek and stop.zgt 2018.12.10
    SCI_TRACE_LOW("line[%d] ARMVB_SetDownlinkMute ptDevSetting->bDownlink_mute:%d", __LINE__, ptDevSetting->bDownlink_mute);
    if(0==ptDevSetting->bDownlink_mute)
    {
        g_aud_level_cfg.spkLevel = (SND_SPK_LEVEL_T)ptDevSetting->uiVol;
    }
    else
    {
        g_aud_level_cfg.spkLevel = SND_SPK_MUTE;
    }
    s_is_vb_mute = ptDevSetting->bDownlink_mute;

    g_aud_level_cfg.micLevel = SND_MIC_ENABLE;
    g_aud_level_cfg.sideLevel = SND_SIDE_VOL_15;
    g_aud_level_cfg.toneLevel = SND_SIDE_VOL_15;
    g_aud_level_cfg.appMode = SND_APP_MODE_MUSIC;
    zsp_sharemem->audInPara.inLenth = AUDIO_INPUT_BUF_SIZE; // 7K
    zsp_sharemem->audInPara.fileEndFlag = 0;
    zsp_sharemem->audInPara.readOffset = 0;
    if (ptDevSetting->eDev_mode == AUDIO_DEVICE_MODE_BLUEPHONE)
    {
#if 0 // AUDIO_CP_USE_ZSP_PARA
        zsp_sharemem->audInPara.sbcOutFlag = 1;
#endif
        g_zsp_para.sbcOutFlag = 1;
    }
    else
    {
#if 0 // AUDIO_CP_USE_ZSP_PARA
        zsp_sharemem->audInPara.sbcOutFlag = 0;
#endif
        g_zsp_para.sbcOutFlag = 0;
    }
    SCI_MEMSET(zsp_sharemem->Output.audOutput, 0, (9 * 1024));
    SCI_MEMSET(zsp_sharemem->audInput, 0, (zsp_sharemem->audInPara.inLenth -4));
#ifdef DECODE_IN_ZSP
    zsp_sharemem->audInPara.writeOffset =zsp_sharemem->audInPara.inLenth -4; // Init buffer
#else
    zsp_sharemem->audInPara.writeOffset = 4608; //zsp_sharemem->audInPara.inLenth -4; // Init buffer
#endif
    ipc_register_music_notify(A2DP_PLAYBACK_Callback);

    if (ptDevSetting->eDev_mode == AUDIO_DEVICE_MODE_BLUEPHONE)
    {
        SCI_TRACE_LOW("[ljf] eDev_mode is bluephone, call A2DP_audioStart");
        A2DP_audioStart(uiSampleRate, A2DP_GetAudioData, A2DP_GetAudioDataCount);
    }
#if 0//ndef ARMVB_USE_CP_AUDIO_INTERFACE
#ifdef ARMVB_USE_PCIM_26M_CLOCK
	//PMIC_26M_open(PMIC_26M_USED_BY_AUDIO);
	AUDIO_PMIC_26M_open(AUDIO_26M_USED_BY_MUSIC);
#endif
#ifdef ARMVB_USE_DM_TONE_INTERFACE
    DM_ZspMusicPlayStart();
#else
    hal_ret = hal_zspMusicPlayStart();
    if (0 != hal_ret)
    {
        SCI_TRACE_LOW("[ljf] line[%d] ARMVB_Play call hal_zspMusicPlayStart faile ret:0x%x", __LINE__, hal_ret);
    }
#endif

#if 1 // vibration mode
    SCI_TRACE_LOW("[ljf] line[%d] ARMVB_Play g_shakingMode:0x%x", __LINE__, g_shakingMode);
    aud_SetSharkingMode(g_shakingMode);
#else
    SCI_TRACE_LOW("[ljf] line[%d] ARMVB_Play g_shakingMode:0x%x", __LINE__, g_shakingMode);
    DM_AudSetSharkingMode(g_shakingMode);
#endif
#ifndef ARMVB_USE_DM_TONE_INTERFACE
    ret = aud_StreamStart(g_cur_itf, &g_aif_stream, &g_aud_level_cfg);
    if (0 != ret)
    {
        SCI_TRACE_LOW("[ljf] line[%d] ARMVB_Play call aud_StreamStart faile ret:0x%x", __LINE__, ret);
    }
#else
    ret = DM_AudStreamStart(g_cur_itf, &g_aif_stream, &g_aud_level_cfg);
    if (0 != ret)
    {
        SCI_TRACE_LOW("[ljf] line[%d] ARMVB_Play call DM_AudStreamStart faile ret:0x%x", __LINE__, ret);
    }
#endif
#else
    ret =AUDIO_CP_START(AUDIO_DEV_MUSIC,g_cur_itf, &g_aif_stream, &g_aud_level_cfg,0,device_cfg_ext,0, &g_zsp_para);
    if (AUD_ERR_NO != ret)
    {
        SCI_TRACE_LOW("zgt line[%d] ARMVB_Play call AUDIO_CP_START faile ret:0x%x", __LINE__, ret);
        return ret;
    }
#endif
    if (ptDevSetting->eDev_mode == AUDIO_DEVICE_MODE_EARFREE)
    {
        g_cur_itf = SND_ITF_LOUD_SPEAKER;
        ret = AUDIO_CP_Setup(g_cur_itf, &g_aud_level_cfg);
        if (0 != ret)
        {
            SCI_TRACE_LOW("[ljf] line[%d] ARMVB_Play call AUDIO_CP_Setup faile ret:0x%x", __LINE__, ret);
        }
    }
    return 0;
}

/*****************************************************************************/
//  Description:    Close A2DP
//  Author:         yuhua.shi
//  Note:
//****************************************************************************/
LOCAL AUDIO_RESULT_E A2DP_Stop(DEV_SETTING_T *ptDevSetting)
{
#if 0
    //SCI_TRACE_LOW:"A2DP_Stop"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,A2DP_AS_160_112_2_18_1_6_52_195,(uint8*)"");
    AUD_SetOutputDataType(AUDIO_PCM_TYPE);
    A2DP_audioClose();  
    _A2DP_EnableDigitalGain(SCI_FALSE); 
//    SCI_FREE(s_a2dp_da0_buffer_multitrack);
//    SCI_FREE(s_a2dp_da1_buffer_multitrack);
    return AUDIO_NO_ERROR;
#endif
	AUD_ERR_T ret = 0;
    HAL_ERR_T hal_ret = 0;
    AUD_ZSP_SHAREMEM_T *zsp_sharemem = (AUD_ZSP_SHAREMEM_T *)g_zsp_share_memory_addr;
    uint32 tick_cnt;
    INT8 delay_cnt = 10;
    tick_cnt = SCI_GetTickCount();
    SCI_TRACE_LOW("hch,A2DP_Stop [ljf] line[%d] file[%s] func[%s] eDev_mode:0x%x g_audio_dev_used:[%d], time[%d][%d]", __LINE__, __FILE__, __FUNCTION__, ptDevSetting->eDev_mode, g_audio_dev_used, tick_cnt/1000, tick_cnt%1000);
    if (1 == g_audio_dev_used)
    {
        SCI_TRACE_LOW("[ljf] Because g_audio_dev_used is [%d], so ARMVB_Stop return", g_audio_dev_used);
        return AUDIO_ERROR;
    }
    switch(ptDevSetting->eDev_mode)
    {
        case AUDIO_DEVICE_MODE_HANDHOLD:
            g_cur_itf = SND_ITF_RECEIVER;
            break;
        case AUDIO_DEVICE_MODE_HANDFREE:
            g_cur_itf = SND_ITF_LOUD_SPEAKER;
            break;
        case AUDIO_DEVICE_MODE_EARPHONE:
            g_cur_itf = SND_ITF_EAR_PIECE;
            break;
        case AUDIO_DEVICE_MODE_EARFREE:
            g_cur_itf = SND_ITF_LOUD_SPEAKER; // close lasted device.
            break;
        case AUDIO_DEVICE_MODE_TVOUT:
            g_cur_itf = SND_ITF_TV;
            break;
        case AUDIO_DEVICE_MODE_BLUEPHONE:
            g_cur_itf = SND_ITF_BLUETOOTH;
            break;
        default:
            SCI_TRACE_LOW("[ljf] line[%d] ARMVB_Play eMode is [0x%x] WRONG!!!", __LINE__, ptDevSetting->eDev_mode);
            break;
    }
    g_uiCurrentSamplerate = 0;
    zsp_sharemem->audInPara.fileEndFlag = 1;
#if 0 // AUDIO_CP_USE_ZSP_PARA
    zsp_sharemem->audInPara.sbcOutFlag = 0;
    zsp_sharemem->musicMode = 0;
#endif
    g_zsp_para.musicMode = 0;
    g_zsp_para.sbcOutFlag = 0;
    if (ptDevSetting->eDev_mode == AUDIO_DEVICE_MODE_BLUEPHONE)
    {
        SCI_TRACE_LOW("[ljf] eDev_mode is bluephone, call A2DP_audioClose");
        A2DP_audioClose();
    }
#if 0///ndef ARMVB_USE_CP_AUDIO_INTERFACE
#ifndef ARMVB_USE_DM_TONE_INTERFACE
    ret = aud_StreamStop(g_cur_itf);
    if (0 != ret)
    {
        SCI_TRACE_LOW("[ljf] line[%d] ARMVB_Stop call aud_StreamStop faile", __LINE__);
    }
#else
    ret = DM_AudStreamStop(g_cur_itf);
    if (0 != ret)
    {
        SCI_TRACE_LOW("[ljf] line[%d] ARMVB_Stop call DM_AudStreamStop faile", __LINE__);
    }
#endif
#ifdef ARMVB_USE_DM_TONE_INTERFACE
    DM_ZspMusicPlayStop();
#else
    hal_ret = hal_zspMusicPlayStop();
    if (0 != hal_ret)
    {
        SCI_TRACE_LOW("[ljf] line[%d] ARMVB_Stop call hal_zspMusicPlayStop faile", __LINE__);
    }
#endif
#if 1
    // just for close 26M after close codec
    while (delay_cnt > 0)
    {
    ret = DM_VoisSetBtNRECFlag(0);
    if (1 == ret)
    {
        SCI_TRACE_LOW("[ljf] line[%d] ARMVB_Stop call DM_VoisSetBtNRECFlag sleep 50ms ret:0x%x", __LINE__, ret);
        SCI_SLEEP(50);
    delay_cnt--;
    }
    else
    {
        SCI_TRACE_LOW("[ljf] line[%d] ARMVB_Stop call DM_VoisSetBtNRECFlag succ and return ret:0x%x", __LINE__, ret);
        break;
    }
    }
#endif

#ifdef ARMVB_USE_PCIM_26M_CLOCK
	//PMIC_26M_close(PMIC_26M_USED_BY_AUDIO);
        AUDIO_PMIC_26M_close(AUDIO_26M_USED_BY_MUSIC);
#endif
#else
    ret = AUDIO_CP_STOP(AUDIO_DEV_MUSIC,g_cur_itf, &g_zsp_para);
    if (AUD_ERR_NO != ret)
    {
        SCI_TRACE_LOW("zgt line[%d] ARMVB_Stop call AUDIO_CP_STOP faile ret:0x%x", __LINE__, ret);
        return ret;
    }
#endif

    SCI_AUD_EnableDeepSleep(ENABLE_DEEP_SLEEP);
    SCI_TRACE_LOW("A2dp_Stop SCI_PutSemaphore....");
    SCI_PutSemaphore(audio_armvb_semaphore);
    SCI_TRACE_LOW("A2dp_Stop SCI_PutSemaphore OK");
    return 0;
}

/*****************************************************************************/
//  Description:    Pause A2DP.
//  Author:         yuhua.shi
//  Note:
//****************************************************************************/
LOCAL AUDIO_RESULT_E A2DP_Pause(DEV_SETTING_T *ptDevSetting)
{
    //SCI_TRACE_LOW:"A2DP_Pause"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,A2DP_AS_173_112_2_18_1_6_52_196,(uint8*)"");
    AUD_SetOutputDataType(AUDIO_PCM_TYPE);
    A2DP_audioClose();
    _A2DP_EnableDigitalGain(SCI_FALSE);
    return AUDIO_NO_ERROR;
}

/*****************************************************************************/
//  Description:    Resume A2DP.
//  Author:         yuhua.shi
//  Note:
//****************************************************************************/
LOCAL AUDIO_RESULT_E A2DP_Resume(uint32 uiSampleRate, DEV_SETTING_T *ptDevSetting )
{
    //SCI_TRACE_LOW:"A2DP_Resume uiSampleRate = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,A2DP_AS_186_112_2_18_1_6_52_197,(uint8*)"d", uiSampleRate);
    s_OutputSamplerate=uiSampleRate;
    A2DP_audioStart(uiSampleRate, A2DP_GetAudioData, A2DP_GetAudioDataCount);
    AUD_SetOutputDataType(AUDIO_SBC_TYPE);
    _A2DP_EnableDigitalGain(SCI_TRUE);
    if (ptDevSetting->bDownlink_mute)
    {
        A2DP_SetVolumeType(0, ptDevSetting->eAppType);
    }
    else
    {
        A2DP_SetVolumeType(ptDevSetting->uiVol, ptDevSetting->eAppType);
    }
    
    return AUDIO_NO_ERROR;
}

/*****************************************************************************/
//  Description:    This function set A2DP clock dividor according sample rate needed
//  Author:         yuhua.shi
//    Note:           
/*****************************************************************************/ 
LOCAL void A2DP_SetSampleRate (uint32 sample_rate)
{
    //nothing to do
    //SCI_TRACE_LOW:"A2DP_SetSampleRate samle_rate = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,A2DP_AS_209_112_2_18_1_6_52_198,(uint8*)"d", sample_rate);
}

/*****************************************************************************/
//  Description:    a2dp logic device set device mode. 
//  Author:         cherry.liu
//  Note:           to inform the audproc plugger about the device mode info
//****************************************************************************/
LOCAL void A2DP_SetDeviceMode (AUDIO_DEVICE_MODE_TYPE_E eMode)
{
	#if 0
    AUDIO_RESULT_E  result = AUDIO_NO_ERROR;
    char *mode_name = NULL;
    
#ifdef EQ_EXE_ON_CHIP
#ifndef EQ_HAS_MULTI_BANDS
    if(hAUDPROCExp)//6800h,8800s4,... 2-band asic eq
    {
        AUD_PROC_EXP_PARA_T  aud_para = {0};
        aud_para.eParaType = AUD_PROC_PARA_EQ_SET;
        result = AUDIO_ExeExpSetParaOpe (
                     hAUDPROCExp,
                     PNULL,
                     AUD_PROC_EXP_PARA_COUNT,
                     &aud_para
                 );

        //SCI_TRACE_LOW:"a2dp_as.c,[A2DP_SetDeviceMode] set aud proc explugger! hAUDPROCExp:0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,A2DP_AS_234_112_2_18_1_6_52_199,(uint8*)"d",hAUDPROCExp);
    }
#else
    mode_name = _ARMVB_ArmvbNv_GetName(eMode);
    if(PNULL==mode_name)
    {
        SCI_TRACE_LOW("a2dp_as.c A2DP_SetDeviceMode mode:%d.");
        return;
    }

    if(hAUDENHAExp)//8800g,... 5-band asic eq
    {
        AUD_ENHA_EXP_PARA_T  mode_para = {0};

        mode_para.eParaType = AUD_ENHA_PARA_DEV_MODE;
        mode_para.unAudProcPara.devModeName = mode_name;

        result = AUDIO_ExeExpSetParaOpe (
            hAUDENHAExp, 
            PNULL,
            AUD_ENHA_EXP_PARA_COUNT,
            &mode_para
        );

        //SCI_TRACE_LOW:"a2dp_as.c,[A2DP_SetDeviceMode] set aud enha explugger! hAUDENHAExp:0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,A2DP_AS_251_112_2_18_1_6_52_200,(uint8*)"d",hAUDENHAExp);
    }
#endif//EQ_HAS_MULTI_BANDS
   
#endif

    return ;
	#endif
	AUD_ERR_T ret = 0;
    HAL_ERR_T hal_ret = 0;
    AUD_ZSP_SHAREMEM_T *zsp_sharemem = (AUD_ZSP_SHAREMEM_T *)g_zsp_share_memory_addr;

    SCI_TRACE_LOW("hch,A2DP_SetDeviceMode [ljf] line[%d] ARMVB_SetDevMode eMode:0x%x", __LINE__, eMode);
    if (AUDIO_DEVICE_MODE_BLUEPHONE == g_cur_itf)
    {
        SCI_TRACE_LOW("[ljf] eDev_mode is bluephone, call A2DP_audioClose");
        A2DP_audioClose();
    }
#if 0
#ifndef ARMVB_USE_DM_TONE_INTERFACE
    ret = aud_StreamStop(g_cur_itf);
    if (0 != ret)
    {
        SCI_TRACE_LOW("[ljf] line[%d] ARMVB_SetDevMode call aud_StreamStop faile", __LINE__);
    }
#else
    ret = DM_AudStreamStop(g_cur_itf);
    if (0 != ret)
    {
        SCI_TRACE_LOW("[ljf] line[%d] ARMVB_SetDevMode call DM_AudStreamStop faile", __LINE__);
    }
#endif

#ifdef ARMVB_USE_DM_TONE_INTERFACE
    DM_ZspMusicPlayStop();
#else
    hal_ret = hal_zspMusicPlayStop();
    if (0 != hal_ret)
    {
        SCI_TRACE_LOW("line[%d] ARMVB_SetDevMode call hal_zspMusicPlayStop faile", __LINE__);
    }
#endif
#else
    ret = AUDIO_CP_StreamStop(g_cur_itf);
    if (0 != ret)
    {
        SCI_TRACE_LOW("[ljf] line[%d] ARMVB_SetDevMode call AUDIO_CP_StreamStop faile", __LINE__);
    }
    hal_ret = AUDIO_ZSP_STOP(&g_zsp_para);
    if (0 != hal_ret)
    {
        SCI_TRACE_LOW("line[%d] ARMVB_SetDevMode call hal_zspMusicPlayStop faile", __LINE__);
    }
#endif
    SCI_MEMSET(zsp_sharemem->Output.audOutput, 0, (9 * 1024));

    switch(eMode)
    {
        case AUDIO_DEVICE_MODE_HANDHOLD:
            g_cur_itf = SND_ITF_RECEIVER;
            break;
        case AUDIO_DEVICE_MODE_HANDFREE:
            g_cur_itf = SND_ITF_LOUD_SPEAKER;
            break;
        case AUDIO_DEVICE_MODE_EARPHONE:
            g_cur_itf = SND_ITF_EAR_PIECE;
            break;
        case AUDIO_DEVICE_MODE_EARFREE:
            g_cur_itf = SND_ITF_LOUD_SPEAKER;
            break;
        case AUDIO_DEVICE_MODE_TVOUT:
            g_cur_itf = SND_ITF_TV;
            break;
        case AUDIO_DEVICE_MODE_BLUEPHONE:
            g_cur_itf = SND_ITF_BLUETOOTH;
            break;

        default:
            break;
    }
    if (AUDIO_DEVICE_MODE_BLUEPHONE ==eMode)
    {
#if 0 // AUDIO_CP_USE_ZSP_PARA
        zsp_sharemem->audInPara.sbcOutFlag = 1;
#endif
        g_zsp_para.sbcOutFlag = 1;
    }
    else
    {
#if 0 // AUDIO_CP_USE_ZSP_PARA
        zsp_sharemem->audInPara.sbcOutFlag = 0;
#endif
        g_zsp_para.sbcOutFlag = 0;
    }
    if (AUDIO_DEVICE_MODE_BLUEPHONE ==eMode)
    {
        SCI_TRACE_LOW("[ljf] eDev_mode is bluephone, call A2DP_audioStart");
        A2DP_audioStart(g_uiCurrentSamplerate, A2DP_GetAudioData, A2DP_GetAudioDataCount);
    }
#if 0
#ifdef ARMVB_USE_DM_TONE_INTERFACE
    DM_ZspMusicPlayStart();
#else
    hal_ret = hal_zspMusicPlayStart();
    if (0 != hal_ret)
    {
        SCI_TRACE_LOW("[zgt] line[%d] ARMVB_SetDevMode call hal_zspMusicPlayStart faile ret:0x%x", __LINE__, hal_ret);
    }
#endif
#if 1 // vibration mode
    SCI_TRACE_LOW("[ljf] line[%d] ARMVB_SetDevMode g_shakingMode:0x%x", __LINE__, g_shakingMode);
    aud_SetSharkingMode(g_shakingMode);
#else
    SCI_TRACE_LOW("[ljf] line[%d] ARMVB_SetDevMode g_shakingMode:0x%x", __LINE__, g_shakingMode);
    DM_AudSetSharkingMode(g_shakingMode);
#endif
    g_aud_level_cfg.spkLevel = SND_SPK_MUTE;
#ifndef ARMVB_USE_DM_TONE_INTERFACE
    ret = aud_StreamStart(g_cur_itf, &g_aif_stream, &g_aud_level_cfg);
    if (0 != ret)
    {
        SCI_TRACE_LOW("[ljf] line[%d] ARMVB_SetDevMode call aud_StreamStart faile ret:0x%x", __LINE__, ret);
    }
#else
    ret = DM_AudStreamStart(g_cur_itf, &g_aif_stream, &g_aud_level_cfg);
    if (0 != ret)
    {
        SCI_TRACE_LOW("[ljf] line[%d] ARMVB_SetDevMode call DM_AudStreamStart faile ret:0x%x", __LINE__, ret);
    }
#endif
#else
    hal_ret = AUDIO_ZSP_START(&g_zsp_para);
    if (0 != hal_ret)
    {
        SCI_TRACE_LOW("[zgt] line[%d] ARMVB_SetDevMode call AUDIO_ZSP_START faile ret:0x%x", __LINE__, hal_ret);
    }
    AUDIO_CP_SetSharkingMode(g_shakingMode);
    ret = AUDIO_CP_StreamStart(g_cur_itf, &g_aif_stream, &g_aud_level_cfg);
    if (0 != ret)
    {
        SCI_TRACE_LOW("[ljf] line[%d] ARMVB_SetDevMode call AUDIO_CP_StreamStart faile ret:0x%x", __LINE__, ret);
    }
#endif
    return;
}

/*****************************************************************************/
//  Description:    a2dp logic device set device volume. 
//  Author:         Shujing.Dong
//  Note:           adjust volume by adjust digital gain
//****************************************************************************/
LOCAL void A2DP_SetVolumeType (uint32  uiVol, AUDIO_ARM_APP_TYPE_E eAppType)
{
#if 0
#ifdef DIGITAL_GAIN_SUPPORT
    uint32 gain = 0;
    int32  dg_level = 0;
    NV_AUDIO_ARM_E    cur_arm_nv_id = NV_AUDIO_ARM_BTHS_E;
    char *mode_name = NULL;
    AUDIO_NV_ARM_MODE_INFO_T *ptAudioInfo = PNULL;
    uint32 index_base = 0;

    //SCI_TRACE_LOW:"A2DP_SetVolumeType vol:%d, appType:%d."
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,A2DP_AS_273_112_2_18_1_6_52_201,(uint8*)"dd", uiVol, eAppType);
    
    if(uiVol>A2DP_RESERVER_VOL_LEVEL)
    {
        //SCI_TRACE_LOW:"A2DP_SetVolumeType volume level is too big(%d), will be adjusted to be:%d."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,A2DP_AS_278_112_2_18_1_6_52_202,(uint8*)"dd", uiVol, (A2DP_RESERVER_VOL_LEVEL));
        uiVol = A2DP_RESERVER_VOL_LEVEL;//max volume level: A2DP_RESERVER_VOL_LEVEL-1
    }
    else if(uiVol<1)
    {
        uiVol = 1;
    }
     
     mode_name = AUDIONVARM_GetAudioModeNameById(cur_arm_nv_id);
     
    if(PNULL==mode_name)
    {
        //SCI_TRACE_LOW:"A2DP_SetVolumeType filed to get modeName:%d."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,A2DP_AS_285_112_2_18_1_6_52_203,(uint8*)"d", cur_arm_nv_id);
        return;
    }
    ptAudioInfo = (AUDIO_NV_ARM_MODE_INFO_T *)AUDIO_ALLOC(sizeof(AUDIO_NV_ARM_MODE_INFO_T));
    if(PNULL==ptAudioInfo)
    {
        //SCI_TRACE_LOW:"A2DP_SetVolumeType, alloc fail, size: %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,A2DP_AS_291_112_2_18_1_6_52_204,(uint8*)"d",sizeof(AUDIO_NV_ARM_MODE_INFO_T));
        return;
    }
    
    if(AUDIO_NV_ARM_NO_ERROR != AUDIONVARM_GetModeParam((const char *) mode_name, (AUDIO_NV_ARM_MODE_INFO_T *)(ptAudioInfo)))
    {
        //SCI_TRACE_LOW:"A2DP_SetVolumeType failed to get mode param, mode:%d, modeName:%s."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,A2DP_AS_298_112_2_18_1_6_52_205,(uint8*)"ds",cur_arm_nv_id, mode_name);
        SCI_FREE(ptAudioInfo);
        return; 
    }  

    //SCI_TRACE_LOW:"A2DP_SetVolumeType uiVol=%d, mode:%d, appType:%d, modeName:%s."
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,A2DP_AS_304_112_2_18_1_6_52_206,(uint8*)"ddds", uiVol, cur_arm_nv_id, eAppType, mode_name);

    index_base = AUDIO_NV_ARM_PARA_RESERVE - A2DP_RESERVER_VOL_LEVEL;

    SCI_PASSERT(((index_base+uiVol)<=AUDIO_NV_ARM_PARA_RESERVE),    /*assert verified*/
        ("A2DP_SetVolumeType overflow, indexbase:%d, vol:%d, indexMax:%d",
        index_base, uiVol, (AUDIO_NV_ARM_PARA_RESERVE)));
    gain = ptAudioInfo->tAudioNvArmModeStruct.reserve[index_base+uiVol-1]; // 0:mute, 1-15 corresponding to volume level 1-15;

    gain =  gain & 0x7F; //Last 7Bit
    
    dg_level = (int32) ( ( (int32) gain << 25) >> 25); // 7Bit is the flag bit: signed

    //SCI_TRACE_LOW:"A2DP_SetVolumeType gain:0x%4x, dg_level= 0x%4x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,A2DP_AS_317_112_2_18_1_6_52_207,(uint8*)"dd", gain, dg_level);

    {
        AUDIO_PM_ExeExpSetParaOpe(hDigitalGainExp, PNULL, dg_level, PNULL);
    }

    SCI_FREE(ptAudioInfo);
#endif
    return;
#endif
	AUD_ERR_T aud_ret = 0;

    SCI_TRACE_LOW("hch,A2DP_SetVolumeType [ljf] line[%d] ARMVB_SetVolume uiVol:0x%x eAppType:0x%x", __LINE__, uiVol, eAppType);
    g_aud_level_cfg.spkLevel = uiVol;
    g_aud_level_cfg.micLevel = SND_MIC_ENABLE;
    g_aud_level_cfg.sideLevel = SND_SIDE_VOL_15;
    g_aud_level_cfg.toneLevel = SND_SIDE_VOL_15;
    g_aud_level_cfg.appMode = SND_APP_MODE_MUSIC;
#if 0
#ifndef ARMVB_USE_DM_TONE_INTERFACE
    aud_ret = aud_Setup(g_cur_itf, &g_aud_level_cfg);
    if (AUD_ERR_NO != aud_ret)
    {
        SCI_TRACE_LOW("[ljf] line[%d] ARMVB_SetVolume aud_ret is [0x%x] WRONG!!!", __LINE__, aud_ret);
    }
#else
    aud_ret = DM_AudSetup(g_cur_itf, &g_aud_level_cfg);
    if (AUD_ERR_NO != aud_ret)
    {
        SCI_TRACE_LOW("[ljf] line[%d] ARMVB_SetVolume aud_ret is [0x%x] WRONG!!!", __LINE__, aud_ret);
    }
#endif
#else
    aud_ret = AUDIO_CP_Setup(g_cur_itf, &g_aud_level_cfg);
    if (AUD_ERR_NO != aud_ret)
    {
        SCI_TRACE_LOW("[ljf] line[%d] ARMVB_SetVolume aud_ret is [0x%x] WRONG!!!", __LINE__, aud_ret);
    }
#endif
    return;
}

/*****************************************************************************/
//  Description:    Set a2dp mute or Cancel the mute_set.
//  Author:         shujing.dong
//  Note:
//****************************************************************************/
LOCAL void A2DP_SetDownlinkMute (
    BOOLEAN is_mute
)
{
#if 0
#ifdef DIGITAL_GAIN_SUPPORT
    uint32  uiVol = 0;
    AUDIO_ARM_APP_TYPE_E eAppType = AUDIO_ARM_APP_TYPE_0;
    AUDIO_GetVolumeAndType((uint32 *)&uiVol, (AUDIO_ARM_APP_TYPE_E *)&eAppType);

    //SCI_TRACE_LOW:"A2DP_SetDownlinkMute, mute:%d, vol:%d, appType:%d."
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,A2DP_AS_342_112_2_18_1_6_52_208,(uint8*)"ddd",is_mute, uiVol, eAppType);
        
    if(is_mute)
    {
        A2DP_SetVolumeType(0, eAppType);
    }
    else
    {
        A2DP_SetVolumeType(uiVol, eAppType);
    }
#endif    
#endif
AUD_ERR_T aud_ret = 0;
    s_is_vb_mute = is_mute;
    SCI_TRACE_LOW("hch,A2DP_SetDownlinkMute [ljf] line[%d] ARMVB_SetDownlinkMute is_mute:0x%x", __LINE__, is_mute);
    if (TRUE == is_mute)
    {
        g_volbeforemute = g_aud_level_cfg.spkLevel;
        g_aud_level_cfg.spkLevel = SND_SPK_MUTE;
    }
    else
    {
        g_aud_level_cfg.spkLevel = g_volbeforemute;
    }
    g_aud_level_cfg.micLevel = SND_MIC_ENABLE;
    g_aud_level_cfg.sideLevel = SND_SIDE_VOL_15;
    g_aud_level_cfg.toneLevel = SND_SIDE_VOL_15;
    g_aud_level_cfg.appMode = SND_APP_MODE_MUSIC;
#if 0
#ifndef ARMVB_USE_DM_TONE_INTERFACE
    aud_ret = aud_Setup(g_cur_itf, &g_aud_level_cfg);
    if (AUD_ERR_NO != aud_ret)
    {
        SCI_TRACE_LOW("[ljf] line[%d] ARMVB_SetDownlinkMute aud_ret is [0x%x] WRONG!!!", __LINE__, aud_ret);
    }
#else
    aud_ret = DM_AudSetup(g_cur_itf, &g_aud_level_cfg);
    if (AUD_ERR_NO != aud_ret)
    {
        SCI_TRACE_LOW("[ljf] line[%d] ARMVB_SetDownlinkMute aud_ret is [0x%x] WRONG!!!", __LINE__, aud_ret);
    }
#endif
#else
    aud_ret = AUDIO_CP_Setup(g_cur_itf, &g_aud_level_cfg);
    if (AUD_ERR_NO != aud_ret)
    {
        SCI_TRACE_LOW("[ljf] line[%d] ARMVB_SetDownlinkMute aud_ret is [0x%x] WRONG!!!", __LINE__, aud_ret);
    }
#endif
    return;
}

/*****************************************************************************/
//  Description:    This function get pcm data count remained in A2DP  device buffer
//                  in byte unit
//  Author:        peter.huang
//    Note:           
/*****************************************************************************/ 
LOCAL uint32  A2DP_GetRemainedPcmCount ()
{
#if 0
    uint32  sbc_samplerate=0;
    uint32 pcm_data_count=0;
    
       sbc_samplerate=A2DP_GetSampleRate();  

    if(sbc_samplerate)
    {    
        pcm_data_count=SBC_GetRemainedPcmCount();
        pcm_data_count=(uint32)(((uint64) pcm_data_count*s_OutputSamplerate)/sbc_samplerate);
    }
    else
    {
        pcm_data_count=0;
    }
    return pcm_data_count;
	#endif
	AUD_ZSP_SHAREMEM_T *zsp_sharemem = NULL;
    UINT16 readOffset;
    UINT16 writeOffset;
    UINT16 lenth;
    UINT32 remainedcount;

    zsp_sharemem = (AUD_ZSP_SHAREMEM_T *)g_zsp_share_memory_addr;
    readOffset = zsp_sharemem->audInPara.readOffset;
    writeOffset = zsp_sharemem->audInPara.writeOffset;
    lenth = zsp_sharemem->audInPara.inLenth;
    if (readOffset <= writeOffset)
    {
        remainedcount = (UINT32)(writeOffset - readOffset);
    }
    else
    {
        remainedcount = (UINT32)(writeOffset + lenth - readOffset);
    }
    SCI_TRACE_LOW("hch,A2DP_GetRemainedPcmCount [ljf] line[%d] ARMVB_GetRemainedPcmCount return remainedcount:%d",
         __LINE__, remainedcount);
    return (remainedcount);
    
}

/*****************************************************************************/
//  Description:    This function get pcm data play time remained in A2DP device buffer
//  Author:        shujing.dong
//    Note:           
/*****************************************************************************/ 
LOCAL uint32  A2DP_GetRemainedPcmTime ()
{
#if 0
    uint32  sbc_samplerate=0;
    uint32 pcm_data_count=0;    

        sbc_samplerate=A2DP_GetSampleRate();  
    
    if(sbc_samplerate)
    {    
        pcm_data_count=SBC_GetRemainedPcmCount();
        pcm_data_count= (uint32)((uint64)pcm_data_count*s_OutputSamplerate/sbc_samplerate);
    }
    else
    {
        pcm_data_count=0;
    }
    if(0==s_OutputSamplerate)
    {
        return 0;
    }
    else
    {
        return ((pcm_data_count>>2) * 1000)/s_OutputSamplerate;
    }
	#endif
	uint32 ret_time = 0;
    uint32 pcm_num = 0;
    if (0 == g_uiCurrentSamplerate)
    {
        SCI_TRACE_LOW("[ljf] line[%d] ARMVB_GetRemainedPcmTime return 0", __LINE__);
        return 0;
    }
    if (8000 == g_uiCurrentSamplerate)
    {
        pcm_num = 2304;
    }
    else
    {
        pcm_num = 1152;
    }
    ret_time = (((ARMVB_GetRemainedPcmCount() + pcm_num) / g_channelNb) * 1000 / g_uiCurrentSamplerate);
    SCI_TRACE_LOW("hch,A2DP_GetRemainedPcmTime [ljf] line[%d] ARMVB_GetRemainedPcmTime 1 g_channelNb:[%d] return:%d ", __LINE__, g_channelNb, ret_time);
    return ret_time;
}

/*****************************************************************************/
//  Description:    get appropriate output samplerate.
//  Author:         Shujing.Dong
//  Note:
//****************************************************************************/
LOCAL uint32 A2DP_GetAppropriateOutputSamplerate(uint32 uiSampleRate)
{
	uint32 tempSampleRate = A2DP_GetSampleRate();//hch
    g_sbc_src_samplerate =  tempSampleRate; 
    //SCI_TRACE_LOW:"peter:g_sbc_src_samplerate is %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,A2DP_AS_447_112_2_18_1_6_53_210,(uint8*)"d",g_sbc_src_samplerate);
#if 1//def BT_SBC_DSP_ENCODE
    if(tempSampleRate <8000)
    {
        return 8000;
    }
    else
    {
        return tempSampleRate;
    }
#else
    return A2DP_GetSampleRate();
#endif  

}

#ifdef BT_A2DP_SINK_AUDIO_SUPPORT

LOCAL void A2DP_Callback_Read_Data(uint16 ret)
{
   SCI_TRACE_LOW("[a2dp_as.c]A2DP_Callback_Read_Data----entry\n");
   return;
}

LOCAL uint32 A2DP_RegRecCallback(
    ADM_RECCALLBACK pCallback
    )
{
    return (uint32)SCI_NULL;
}

LOCAL void A2DP_UnRegRecCallback(
    uint32 uiRegVector
    )
{

   SCI_TRACE_LOW("A2DP_UnRegRecCallback----entry\n");
   return;
}

LOCAL void A2DP_ReadData(uint16* puiDestData, uint32 uiCount, uint32 *puiDataLen )
{
    SCI_TRACE_LOW("A2DP_ReadData----entry\n");
    return;    
}

#endif
/*****************************************************************************/
//  Description:    Register A2DP device to audio service.
//  Author:         yuhua.shi
//  Note:
//****************************************************************************/
PUBLIC HAUDIODEV A2DP_RegAudioDevice(void)
{
    AUDIO_DEVICE_T tDeviceInfo = {0};
    HAUDIODEV hDevice = INVALID_HANDLE;
    
    SCI_MEM16CPY((uint16*)tDeviceInfo.usDeviceName, ANSI2UINT16("A2DP"), strlen("A2DP") * 2 + 2);
    tDeviceInfo.bIsNeedTrack = SCI_TRUE;
    tDeviceInfo.uiMinFetchLength = A2DP_MAX_OUTPUT_PCM_DATA_LENGTH;
    tDeviceInfo.ucDevOpenCnt = 0;
    tDeviceInfo.tAudioDevOpe.pOpen = A2DP_Play; 
    tDeviceInfo.tAudioDevOpe.pClose = A2DP_Stop; 
    //tDeviceInfo.tAudioDevOpe.pPause = A2DP_Pause; //hch
    //tDeviceInfo.tAudioDevOpe.pResume = A2DP_Resume; //hch
	tDeviceInfo.tAudioDevOpe.pPause = A2DP_Stop; //hch
    tDeviceInfo.tAudioDevOpe.pResume = A2DP_Play; //hch
    tDeviceInfo.tAudioDevOpe.pSetSamplerate = A2DP_SetSampleRate;
    tDeviceInfo.tAudioDevOpe.pStartRecord = SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pStopRecord = SCI_NULL;
#ifdef BT_A2DP_SINK_AUDIO_SUPPORT
    tDeviceInfo.tAudioDevOpe.pRegRecCallback = A2DP_RegRecCallback; 
    tDeviceInfo.tAudioDevOpe.pUnRegRecCallback = A2DP_UnRegRecCallback; 
    tDeviceInfo.tAudioDevOpe.pReadData =  A2DP_ReadData ; 
#else
    tDeviceInfo.tAudioDevOpe.pRegRecCallback = SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pUnRegRecCallback = SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pReadData = SCI_NULL;
#endif
    tDeviceInfo.tAudioDevOpe.pSetDevMode = A2DP_SetDeviceMode;
    tDeviceInfo.tAudioDevOpe.pSetVolume = A2DP_SetVolumeType;
    tDeviceInfo.tAudioDevOpe.pSetUplinkMute = SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pSetDownlinkMute = A2DP_SetDownlinkMute;
    tDeviceInfo.tAudioDevOpe.pSetDepopMute=SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pSetDigitalGain = SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pGetRemainedPcmCount=A2DP_GetRemainedPcmCount;
    tDeviceInfo.tAudioDevOpe.pGetRemainedPcmTime=A2DP_GetRemainedPcmTime;
    tDeviceInfo.tAudioDevOpe.pGetAppropriateOutputSamplerate = 
        A2DP_GetAppropriateOutputSamplerate;
    
    hDevice = AUDIO_DM_RegAudioDevice(&tDeviceInfo);
    s_a2dp_device_ptr = AUDIO_DM_GetDeviceHandleStruct(hDevice);   // Record device structure
    if(SCI_NULL == audio_armvb_semaphore)
    {
        audio_armvb_semaphore = SCI_CreateSemaphore("ARMVB_AUDIO_SEMAPHORE",1);
    }

    return hDevice;
}
PUBLIC void A2DP_GetAudioData(int16 *left_data, int16 *right_data, uint32 data_len)
{
        /*SCI_ASSERT(data_len%32==0);
        SCI_ASSERT(data_len<=A2DP_FETCH_BUFSIZE);
        _Track_Mixer(s_a2dp_device_ptr->ptTrackList,
            s_a2dp_da0_buffer_multitrack,
            s_a2dp_da1_buffer_multitrack,
            left_data,
            right_data,
            32,   // data_len per block
            data_len/32,               // blocks
            SCI_FALSE, // stereo, disable downmix
            SCI_FALSE);*/
        BOOLEAN bEnable = FALSE;
        AUDIO_GetDownLinkMute(&bEnable);
        _Read_SingleTrack(s_a2dp_device_ptr->ptTrackList,
            left_data,
            right_data,
            data_len,
            bEnable
            );
}

PUBLIC uint32 A2DP_GetAudioDataCount(void)
{
        return _GetDataCount_SingleTrack(s_a2dp_device_ptr->ptTrackList);
}

#endif /* defined(BT_A2DP_SUPPORT) */
