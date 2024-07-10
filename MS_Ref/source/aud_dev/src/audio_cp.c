/******************************************************************************
 ** File Name:      audio_cp.c                                                                *
 ** Author:         Guangtao.Zhi                                             *
 ** DATE:           03/11/2019                                                *
 ** Copyright:      2019 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:    We abstract CP codec and ZSP as an audio hal middle layer between audio dev and 
 **                    codec&zsp.It call codec&zsp with rpc channel.     * 
 **                                                                           *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 03/11/2019     Guangtao.Zhi    Create.                                   *
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
#include "rpc_aud.h"
#include "rpc_aud_api.h"
#include "load_modem.h"
#include "hal_aud_zsp.h"
#include "pmic_26M_clock.h"
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif
uint32    AUDIO_CP_status = AUDIO_CP_STOPPED;
AUD_CP_STATUS_E    AUD_CODEC_status = AUD_NULL;
LOCAL uint32    AUDIO_PMIC_26M_clock_status = 0;
extern uint8 *g_zsp_share_memory_addr ;
extern uint32 g_shakingMode;
extern uint32 g_toneMode;
#define AUDIO_CP_DM_TONE_INTERFACE
#define AUDIO_CP_Synchronous_ITF

PUBLIC void AUDIO_PMIC_26M_open (uint32 open_flag)
{
    if( ( AUDIO_26M_USED_BY_MUSIC    != open_flag )
      && ( AUDIO_26M_USED_BY_LAYER1  != open_flag )
      && ( AUDIO_26M_USED_BY_RECORD    != open_flag )
      && ( AUDIO_26M_USED_BY_FM    != open_flag )
      && ( AUDIO_26M_USED_BY_TONE    != open_flag ) 
      && ( AUDIO_26M_USED_BY_LocalTest    != open_flag ) )
    {
        SCI_PASSERT(0, ("AUDIO_PMIC_26M_open ERR OPEN FLAG =%d", open_flag));
    }
    SCI_TRACE_LOW("[zgt] fuc[%s] line[%d]open_flag[%d] ", __FUNCTION__,__LINE__,open_flag);
    PMIC_26M_open(PMIC_26M_USED_BY_AUDIO);
    AUDIO_PMIC_26M_clock_status |= (open_flag); 
}
PUBLIC void AUDIO_PMIC_26M_close (uint32 close_flag)
{
    if( ( AUDIO_26M_USED_BY_MUSIC    != close_flag )
      && ( AUDIO_26M_USED_BY_LAYER1  != close_flag )
      && ( AUDIO_26M_USED_BY_RECORD    != close_flag )
      && ( AUDIO_26M_USED_BY_FM    != close_flag ) 
      && ( AUDIO_26M_USED_BY_TONE    != close_flag )
      && ( AUDIO_26M_USED_BY_LocalTest    != close_flag ))
    {         
        SCI_PASSERT(0, ("AUDIO_PMIC_26M_close ERR close_flag =%d", close_flag));
    }
    AUDIO_PMIC_26M_clock_status &= (~close_flag);
    SCI_TRACE_LOW("[zgt] fuc[%s] line[%d]close_flag[%d]clock_status[%d] "
        ,__FUNCTION__,__LINE__,close_flag,AUDIO_PMIC_26M_clock_status);
    if  ( 0 == AUDIO_PMIC_26M_clock_status )
    {
        PMIC_26M_close(PMIC_26M_USED_BY_AUDIO);
    }	else
    {
        SCI_TRACE_LOW("line[%d] AUDIO_PMIC_26M is busy:%d", __LINE__,AUDIO_PMIC_26M_clock_status);
    }

}
PUBLIC BOOLEAN Aud_IS_CodecOperateDone(AUD_CP_STATUS_E flag)
{
    INT8 delay_cnt = 100;
    while (delay_cnt > 0)
    {
        if (flag == AUD_CODEC_status)
        {
            SCI_TRACE_LOW("zgt Aud_IS_CodecOperateDone  succ and  status:%d", AUD_CODEC_status);
            AUD_CODEC_status = AUD_NULL;
            return SCI_TRUE;
        }
        else
        {
            SCI_TRACE_LOW("zgt Aud_IS_CodecOperateDone wait codec flag sleep 15ms ret:%d", AUD_CODEC_status);
            SCI_SLEEP(5);
            delay_cnt--;
        }
    }
    SCI_TRACE_LOW("zgt Aud_IS_CodecOperateDone  wait codec operate timeout ret:%d", AUD_CODEC_status);
    return SCI_FALSE;
}
PUBLIC void AUD_SetCodecOpStatus(AUD_CP_STATUS_E Status)
{
    SCI_TRACE_LOW("zgt AUD_SetCodecOpStatus enter status:%d,AUD_CODEC_status:%d", Status,AUD_CODEC_status);
    AUD_CODEC_status = Status;
    SCI_TRACE_LOW("zgt AUD_SetCodecOpStatus exit AUD_CODEC_status:%d", AUD_CODEC_status);
}

PUBLIC AUDIO_RESULT_E AUDIO_ZSP_START(AUD_AP2CP_PARA_T *zsp_para)
{
    AUDIO_RESULT_E zsp_ret = AUDIO_ERROR;
    //AUD_SetCodecOpStatus(ZSP_START_BEGIN);
#ifdef AUDIO_CP_DM_TONE_INTERFACE
#if 0 // AUDIO_CP_USE_ZSP_PARA
    zsp_ret = DM_ZspMusicPlayStart();
#else
    zsp_ret = DM_ZspMusicPlayStartEx(zsp_para);
#endif
#ifdef AUDIO_CP_Synchronous_ITF
    if(SCI_FALSE==Aud_IS_CodecOperateDone(ZSP_START_DONE))
    {
        SCI_TRACE_LOW("zgt AUDIO_ZSP_START NOT DONE:%d", AUD_CODEC_status);
        return AUDIO_ERROR;
    }
#endif
#else
    zsp_ret = hal_zspMusicPlayStart();
#endif
    if (AUDIO_NO_ERROR != zsp_ret)
    {
        SCI_TRACE_LOW("zgt AUDIO_ZSP_START call zsp Start faile ret:%d", zsp_ret);
        return zsp_ret;
    }
    return AUDIO_NO_ERROR;
}
PUBLIC AUDIO_RESULT_E AUDIO_ZSP_STOP(AUD_AP2CP_PARA_T *zsp_para)
{
    AUDIO_RESULT_E zsp_ret = AUDIO_ERROR;
    //AUD_SetCodecOpStatus(ZSP_STOP_BEGIN);
#ifdef AUDIO_CP_DM_TONE_INTERFACE
#if 0 // AUDIO_CP_USE_ZSP_PARA
    zsp_ret = DM_ZspMusicPlayStop();
#else
    zsp_ret = DM_ZspMusicPlayStopEx(zsp_para);
#endif
#ifdef AUDIO_CP_Synchronous_ITF    
    if(SCI_FALSE==Aud_IS_CodecOperateDone(ZSP_STOP_DONE))
    {
        SCI_TRACE_LOW("zgt AUDIO_ZSP_STOP NOT DONE:%d", AUD_CODEC_status);
        return AUDIO_ERROR;
    }
#endif
#else
    zsp_ret = hal_zspMusicPlayStop();
#endif
    if (AUDIO_NO_ERROR != zsp_ret)
    {
        SCI_TRACE_LOW("zgt AUDIO_ZSP_STOP call zsp music Stop faile =%d", zsp_ret);
        return zsp_ret;
    }
    return AUDIO_NO_ERROR;
}
PUBLIC AUDIO_RESULT_E AUDIO_VOIS_START()
{
    AUDIO_RESULT_E vois_ret = AUDIO_ERROR;
    //AUD_SetCodecOpStatus(CODEC_VOIS_START_BEGIN);
    vois_ret = DM_StartAudioEx();
    if (AUDIO_NO_ERROR != vois_ret)
    {
        SCI_TRACE_LOW("zgt line[%d] AUDIO_VOIS_START call StartAudioEx dm_ret is [0x%x] WRONG!!!", __LINE__, vois_ret);
        return vois_ret;
    }
#ifdef AUDIO_CP_Synchronous_ITF
    SCI_TRACE_LOW("LYZ AUDIO_VOIS_START");
    if(SCI_FALSE==Aud_IS_CodecOperateDone(CODEC_VOIS_START_DONE))
    {
        SCI_TRACE_LOW("zgt AUDIO_VOIS_START NOT DONE:%d", AUD_CODEC_status);
        return AUDIO_ERROR;
    }
#endif
    return AUDIO_NO_ERROR;
}
PUBLIC AUDIO_RESULT_E AUDIO_VOIS_STOP()
{
    AUDIO_RESULT_E vois_ret = AUDIO_ERROR;
    //AUD_SetCodecOpStatus(CODEC_VOIS_STOP_BEGIN);
    vois_ret = DM_StopAudioEx();
    if (AUDIO_NO_ERROR != vois_ret)
    {
        SCI_TRACE_LOW("zgt line[%d] AUDIO_VOIS_STOP call StartAudioEx dm_ret is [0x%x] WRONG!!!", __LINE__, vois_ret);
        return vois_ret;
    }
#ifdef AUDIO_CP_Synchronous_ITF
    SCI_TRACE_LOW("LYZ AUDIO_VOIS_STOP");
    if(SCI_FALSE==Aud_IS_CodecOperateDone(CODEC_VOIS_STOP_DONE))
    {
        SCI_TRACE_LOW("zgt AUDIO_VOIS_STOP NOT DONE:%d", AUD_CODEC_status);
        return AUDIO_ERROR;
    }
#endif
    return AUDIO_NO_ERROR;
}
PUBLIC AUD_ERR_T AUDIO_CP_StreamStart(SND_ITF_T itf, HAL_AIF_STREAM_T *stream, AUD_LEVEL_T *cfg)
{
    AUD_ERR_T codec_result = AUD_ERR_UNKNOWN;
    //AUD_SetCodecOpStatus(CODEC_STREAM_START_BEGIN);
#ifndef AUDIO_CP_DM_TONE_INTERFACE
    codec_result = aud_StreamStart(itf, stream, cfg);
#else
    codec_result = DM_AudStreamStart(itf, stream, cfg);
#ifdef AUDIO_CP_Synchronous_ITF
    if(SCI_FALSE==Aud_IS_CodecOperateDone(CODEC_STREAM_START_DONE))
    {
        SCI_TRACE_LOW("zgt AUDIO_CP_StreamStart NOT DONE:%d", AUD_CODEC_status);
        return AUDIO_ERROR;
    }
#endif
#endif
    if (AUD_ERR_NO != codec_result)
    {
        SCI_TRACE_LOW("zgt AUDIO_CP_StreamStart call codec Start faile ret:%d", codec_result);
        return codec_result;
    }
    return AUD_ERR_NO;
}
PUBLIC AUD_ERR_T AUDIO_CP_StreamStop(SND_ITF_T itf)
{    
    AUD_ERR_T codec_result = AUD_ERR_UNKNOWN;
#ifndef AUDIO_CP_DM_TONE_INTERFACE
    codec_result = aud_StreamStop(itf);
#else
    //AUD_SetCodecOpStatus(CODEC_STREAM_STOP_BEGIN);
    codec_result = DM_AudStreamStop(itf);
#ifdef AUDIO_CP_Synchronous_ITF
    if(SCI_FALSE==Aud_IS_CodecOperateDone(CODEC_STREAM_STOP_DONE))
    {
        SCI_TRACE_LOW("zgt AUDIO_CP_StreamStop NOT DONE:%d", AUD_CODEC_status);
        return AUDIO_ERROR;
    }
#endif
#endif
    if (AUD_ERR_NO!= codec_result)
    {
        SCI_TRACE_LOW("zgt AUDIO_CP_StreamStop call codec Stop faile ret:0x%x", codec_result);
        return codec_result;
    }
    return AUD_ERR_NO;
}
PUBLIC AUD_ERR_T AUDIO_CP_RecordStart(SND_ITF_T itf, HAL_AIF_STREAM_T *stream, AUD_LEVEL_T *cfg)
{
    AUD_ERR_T codec_result = AUD_ERR_UNKNOWN;
#ifndef AUDIO_CP_DM_TONE_INTERFACE
    codec_result = aud_StreamRecord(itf, stream, cfg);
#else
    //AUD_SetCodecOpStatus(CODEC_RECORD_START_BEGIN);
    codec_result = DM_AudStreamRecord(itf, stream, cfg);
#ifdef AUDIO_CP_Synchronous_ITF
    if(SCI_FALSE==Aud_IS_CodecOperateDone(CODEC_RECORD_START_DONE))
    {
        SCI_TRACE_LOW("zgt AUDIO_CP_RecordStart NOT DONE:%d", AUD_CODEC_status);
        return AUDIO_ERROR;
    }
#endif
#endif
    if (AUD_ERR_NO != codec_result)
    {
        SCI_TRACE_LOW("zgt AUDIO_CP_RecordStart call codec Start faile ret:%d", codec_result);
        return codec_result;
    }
    return AUD_ERR_NO;
}
PUBLIC AUD_ERR_T AUDIO_CP_RecordStop(SND_ITF_T itf)
{    
    AUD_ERR_T codec_result = AUD_ERR_UNKNOWN;
#ifndef AUDIO_CP_DM_TONE_INTERFACE
    codec_result = aud_StreamStop(itf);
#else
    //AUD_SetCodecOpStatus(CODEC_STREAM_STOP_BEGIN);
    codec_result = DM_AudStreamStop(itf);
#ifdef AUDIO_CP_Synchronous_ITF
    if(SCI_FALSE==Aud_IS_CodecOperateDone(CODEC_STREAM_STOP_DONE))
    {
        SCI_TRACE_LOW("zgt AUDIO_CP_StreamStop NOT DONE:%d", AUD_CODEC_status);
        return AUDIO_ERROR;
    }
#endif
#endif
    if (AUD_ERR_NO!= codec_result)
    {
        SCI_TRACE_LOW("zgt AUDIO_CP_StreamStop call codec Stop faile ret:0x%x", codec_result);
        return codec_result;
    }
    return AUD_ERR_NO;
}
PUBLIC int32 AUDIO_CP_PlayTone(uint8 nToneType, uint8 nToneAttenuation, uint16 nDuration, DM_SpeakerGain volumn)
{ 
    AUD_ERR_T codec_result = AUD_ERR_UNKNOWN;
    if(AUDIO_CP_STOPPED == AUDIO_CP_status || AUDIO_CP_VOICE_PLAYING == AUDIO_CP_status)
    {
        AUDIO_PMIC_26M_open(AUDIO_26M_USED_BY_TONE);
        codec_result =DM_PlayToneEx( nToneType,nToneAttenuation,nDuration,volumn);
        if (AUD_ERR_NO != codec_result)
        {
            SCI_TRACE_LOW("zgt AUDIO_CP_Setup call setcfg faile vois_ret:0x%x", codec_result);
            return codec_result;
        }
        AUDIO_CP_status |=AUDIO_CP_TONE_PLAYING;
    }else
    {
        SCI_TRACE_LOW("zgt:line[%d] TONE call cp faile AUDIO_CP_status=0x%x", __LINE__, AUDIO_CP_status);
        return AUDIO_PLAYING;
    }
    return AUDIO_NO_ERROR;
}
PUBLIC int32 AUDIO_CP_StopTone()
{
    if(0!=(AUDIO_CP_status&AUDIO_CP_TONE_PLAYING ))
    {
        AUDIO_PMIC_26M_close(AUDIO_26M_USED_BY_TONE);
        AUDIO_CP_status &= (~AUDIO_CP_TONE_PLAYING);
    }else
    {
        SCI_TRACE_LOW("zgt line[%d] AUDIO_CP_STOP faile because TONE not in playing", __LINE__);
        return AUDIO_ERROR;
    }
    return AUDIO_NO_ERROR;
}
PUBLIC AUD_ERR_T AUDIO_CP_bbatPcmPlayStart(SND_ITF_T itf, BBAT_PCM_STREAM_T *playbuffer)
{
    AUD_ERR_T codec_result = AUD_ERR_UNKNOWN;
    AUDIO_PMIC_26M_open(AUDIO_26M_USED_BY_MUSIC);
    codec_result = aud_bbatPcmBufferPlayStart(itf, playbuffer);
    if (AUD_ERR_NO != codec_result)
    {
        SCI_TRACE_LOW("line[%d] AUDIO_CP_bbatPcmPlayStart call aud_bbatPcmBufferPlayStart faile ret:0x%x", __LINE__, codec_result);
        return codec_result;
    }
    return AUD_ERR_NO;
}
PUBLIC AUD_ERR_T AUDIO_CP_bbatPcmPlayStop(SND_ITF_T itf)
{
    AUD_ERR_T codec_result = AUD_ERR_UNKNOWN;
    codec_result = aud_bbatPcmBufferPlayStop(itf);
    if (AUD_ERR_NO != codec_result)
    {
        SCI_TRACE_LOW("line[%d] AUDIO_CP_bbatPcmPlayStop call aud_bbatPcmBufferPlayStop faile ret:0x%x", __LINE__, codec_result);
        return codec_result;
    }    
    AUDIO_PMIC_26M_close(AUDIO_26M_USED_BY_MUSIC);
    return AUD_ERR_NO;
}
PUBLIC HAL_ERR_T AUDIO_ZSP_VoiceRecordStart(void)
{
    AUDIO_RESULT_E zsp_ret = AUDIO_ERROR;
    zsp_ret = hal_zspVoiceRecordStart();
    if (AUDIO_NO_ERROR != zsp_ret)
    {
        SCI_TRACE_LOW("zgt AUDIO_ZSP_VoiceRecordStart call zsp Start faile ret:0x%x", zsp_ret);
        return zsp_ret;
    }
    return AUDIO_NO_ERROR;
}
PUBLIC HAL_ERR_T AUDIO_ZSP_VoiceRecordStop(void)
{
    AUDIO_RESULT_E zsp_ret = AUDIO_ERROR;
    zsp_ret = hal_zspVoiceRecordStop();
    if (AUDIO_NO_ERROR != zsp_ret)
    {
        SCI_TRACE_LOW("zgt AUDIO_ZSP_VoiceRecordStop call zsp Start faile ret:0x%x", zsp_ret);
        return zsp_ret;
    }
    return AUDIO_NO_ERROR;
}
PUBLIC AUD_ERR_T AUDIO_CP_Setup(SND_ITF_T itf, AUD_LEVEL_T *cfg)
{
    AUD_ERR_T codec_result = VOIS_ERR_UNKNOWN;
#ifndef AUDIO_CP_DM_TONE_INTERFACE
   // AUD_SetCodecOpStatus(AUD_CODEC_SETUP_BEGIN);
    codec_result =  aud_Setup(itf,cfg);
#else
    codec_result = DM_AudSetup(itf, cfg);
#ifdef AUDIO_CP_Synchronous_ITF
    if(SCI_FALSE==Aud_IS_CodecOperateDone(AUD_CODEC_SETUP_DONE))
    {
        SCI_TRACE_LOW("zgt AUDIO_CP_Setup NOT DONE:%d", AUD_CODEC_status);
        return AUDIO_ERROR;
    }
#endif
#endif
    if (AUD_ERR_NO != codec_result)
    {
        SCI_TRACE_LOW("zgt AUDIO_CP_Setup call setcfg faile vois_ret:%d", codec_result);
        return codec_result;
    }
    return AUD_ERR_NO;
}
PUBLIC void AUDIO_CP_SetDevCFG(AUD_DEVICE_CFG_EXT_T cfg)
{
    aud_SetAudDeviceCFG(cfg);
}
PUBLIC VOIS_ERR_T AUDIO_CP_VoisSetCfg(AUD_ITF_T itf, VOIS_AUDIO_CFG_T *cfg, AUD_DEVICE_CFG_EXT_T devicecfg, SND_BT_WORK_MODE_T btworkmode)
{
    VOIS_ERR_T vois_ret = VOIS_ERR_UNKNOWN;
#ifndef AUDIO_CP_DM_TONE_INTERFACE
    vois_ret = vois_SetCfg(itf, cfg, devicecfg, btworkmode);
#else
    //AUD_SetCodecOpStatus(VOIS_SETUP_BEGIN);
    vois_ret = DM_VoisSetCfg(itf, cfg, devicecfg, btworkmode);
#if 0//def AUDIO_CP_Synchronous_ITF
    if(SCI_FALSE==Aud_IS_CodecOperateDone(VOIS_SETUP_DONE))
    {
        SCI_TRACE_LOW("zgt AUDIO_CP_VoisSetCfg NOT DONE:%d", AUD_CODEC_status);
        return AUDIO_ERROR;
    }
#endif
#endif
    if (VOIS_ERR_NO != vois_ret)
    {
        SCI_TRACE_LOW("zgt AUDIO_CP_VoisSetCfg call vois setcfg faile vois_ret:%d", vois_ret);
        return vois_ret;
    }
    return VOIS_ERR_NO;
}
PUBLIC VOIS_ERR_T AUDIO_CP_TestModeSet(SND_ITF_T itf, HAL_AIF_STREAM_T *stream, AUD_LEVEL_T *cfg, AUD_TEST_T mode, UINT32 voismode)
{
    VOIS_ERR_T vois_ret = VOIS_ERR_UNKNOWN;
#ifndef AUDIO_CP_DM_TONE_INTERFACE
    vois_ret = vois_TestModeSetup(itf, stream, cfg, mode, voismode);
#else
    //AUD_SetCodecOpStatus(AUD_TESTMODE_SETUP_BEGIN);
    vois_ret = DM_VoisTestModeSetup(itf, stream, cfg, mode, voismode);
#ifdef AUDIO_CP_Synchronous_ITF
    if(SCI_FALSE==Aud_IS_CodecOperateDone(AUD_TESTMODE_SETUP_DONE))
    {
        SCI_TRACE_LOW("zgt AUDIO_CP_VoisSetCfg NOT DONE:%d", AUD_CODEC_status);
        return AUDIO_ERROR;
    }
#endif
#endif
    if (VOIS_ERR_NO != vois_ret)
    {
        SCI_TRACE_LOW("zgt AUDIO_CP_TestModeSet call tset mode setcfg faile vois_ret:0x%x", vois_ret);
        return vois_ret;
    }
    return VOIS_ERR_NO;
}
PUBLIC void AUDIO_CP_SetBtNRECFlag(uint32 flag)
{
    vois_SetBtNRECFlag(flag);
    return;
}
PUBLIC AUDIO_CODEC_STATUS_E AUDIO_CP_GetCodecStatus(UINT32 flag)
{
    AUDIO_CODEC_STATUS_E AUDIO_CODEC_status = CODEC_STOPPED;
    INT8 delay_cnt = 10;
    while (delay_cnt > 0)
    {
        AUDIO_CODEC_status = DM_VoisSetBtNRECFlag(flag);
        if (CODEC_PLAYING == AUDIO_CODEC_status)
        {
            SCI_TRACE_LOW("zgt AUDIO_CP_GetCodecStatus call DM_VoisSetBtNRECFlag sleep 50ms ret:0x%x", AUDIO_CODEC_status);
            SCI_SLEEP(50);
            delay_cnt--;
        }
        else
        {
            SCI_TRACE_LOW("zgt AUDIO_CP_GetCodecStatus call DM_VoisSetBtNRECFlag succ and return ret:0x%x", AUDIO_CODEC_status);
            break;
        }
    }
    return AUDIO_CODEC_status;
}

void AUDIO_CP_SetSharkingMode(UINT32 isSharkingMode)
{
    UINT32 CpSharkMode=0;
    CpSharkMode = isSharkingMode;
    SCI_TRACE_LOW("zgt line[%d] AUDIO_CP_SetSharkingMode CpSharkMode:0x%x", __LINE__, CpSharkMode);
#ifndef AUDIO_CP_DM_TONE_INTERFACE
    aud_SetSharkingMode(CpSharkMode);
#else
    DM_AudSetSharkingMode(CpSharkMode);
#endif
}

void AUDIO_CP_SetToneMode(UINT32 isToneMode)
{
    SCI_TRACE_LOW("zgt line[%d] AUDIO_CP_SetToneMode isToneMode:0x%x ", __LINE__, isToneMode);
    DM_AudSetToneMode(isToneMode);
}

PUBLIC AUDIO_RESULT_E AUDIO_CP_START(
    AUDIO_DEV_TYPE_E dev_type,
    SND_ITF_T itf, 
    HAL_AIF_STREAM_T *stream, 
    AUD_LEVEL_T *cfg,
    VOIS_AUDIO_CFG_T *Vois_cfg,
    AUD_DEVICE_CFG_EXT_T devicecfg, 
    SND_BT_WORK_MODE_T btworkmode,
    AUD_AP2CP_PARA_T *zsp_para
    )
{
    AUDIO_RESULT_E codec_result = AUDIO_ERROR;
    AUDIO_RESULT_E zsp_ret = AUDIO_ERROR;
    AUDIO_RESULT_E vois_ret = AUDIO_ERROR;
    AUDIO_DEV_TYPE_E type = 0;
    AUD_ZSP_SHAREMEM_T *zsp_sharemem = (AUD_ZSP_SHAREMEM_T *)g_zsp_share_memory_addr;

    type = dev_type;
    SCI_TRACE_LOW("zgt:line[%d] AUDIO_CP_START enter DEV type=0x%x,AUDIO_CP_status=0x%x", __LINE__, type,AUDIO_CP_status);
    switch(type)
    {
    case    AUDIO_DEV_MUSIC:
        if(AUDIO_CP_STOPPED == AUDIO_CP_status || (g_shakingMode&&(AUDIO_CP_VOICE_PLAYING == AUDIO_CP_status)))
        {
            //DM_AudSetToneMode(g_toneMode);
            AUDIO_CP_SetToneMode(g_toneMode);
            AUDIO_PMIC_26M_open(AUDIO_26M_USED_BY_MUSIC);
            AUDIO_CP_SetDevCFG(devicecfg);
            zsp_ret = AUDIO_ZSP_START(zsp_para);
            if (AUDIO_NO_ERROR != zsp_ret)
            {
                SCI_TRACE_LOW("zgt line[%d] AUDIO_CP_START call zsp Start faile ret:0x%x", __LINE__, zsp_ret);
                //return zsp_ret;
            }
            AUDIO_CP_SetSharkingMode(g_shakingMode);
            codec_result = AUDIO_CP_StreamStart(itf, stream, cfg);
            if (AUDIO_NO_ERROR != codec_result)
            {
                SCI_TRACE_LOW("zgt line[%d] AUDIO_CP_START call codec Start faile ret:0x%x", __LINE__, codec_result);
                //return codec_result;
            }
            AUDIO_CP_status |=AUDIO_CP_MUSIC_PLAYING;
        }else
        {
            SCI_TRACE_LOW("zgt:line[%d] music call cp faile AUDIO_CP_status=0x%x", __LINE__, AUDIO_CP_status);
            return AUDIO_PLAYING;
        }
        break;
        
    case    AUDIO_DEV_LAYER1:
         //if(AUDIO_CP_STOPPED == AUDIO_CP_status  || (g_shakingMode&&(AUDIO_CP_MUSIC_PLAYING == AUDIO_CP_status)))
        if(AUDIO_CP_STOPPED == AUDIO_CP_status )
         {
            AUDIO_PMIC_26M_open(AUDIO_26M_USED_BY_LAYER1);
            vois_ret = AUDIO_CP_VoisSetCfg(itf, Vois_cfg, devicecfg, btworkmode);
            if (VOIS_ERR_NO != vois_ret)
            {
                SCI_TRACE_LOW("zgt line[%d] AUDIO_CP_START call vois setcfg faile vois_ret:0x%x", __LINE__, vois_ret);
               // return vois_ret;
            }
            vois_ret = AUDIO_VOIS_START();
            if (0 != vois_ret)
            {
                SCI_TRACE_LOW("zgt line[%d] AUDIO_CP_START call StartAudioEx dm_ret is [0x%x] WRONG!!!", __LINE__, vois_ret);
                //return vois_ret;
            }
            AUDIO_CP_status |=AUDIO_CP_VOICE_PLAYING;
        }else
        {
            SCI_TRACE_LOW("zgt:line[%d] voice call cp faile AUDIO_CP_status=0x%x", __LINE__, AUDIO_CP_status);
            return AUDIO_PLAYING;
        }
        break;
    case    AUDIO_DEV_RECORD:
         if(AUDIO_CP_STOPPED == AUDIO_CP_status )
        {
            AUDIO_PMIC_26M_open(AUDIO_26M_USED_BY_RECORD);
            AUDIO_CP_SetDevCFG(devicecfg);
            zsp_ret = AUDIO_ZSP_START(zsp_para);
            if (0 != zsp_ret)
            {
                SCI_TRACE_LOW("[zgt] line[%d] AUDIO_CP_START call zsp Start faile", __LINE__);
                //return zsp_ret;
            }
            //start record stream
            codec_result = AUDIO_CP_RecordStart(itf, stream, cfg);
            if (0 != codec_result) 
            {
                SCI_TRACE_LOW("[zgt] line[%d] AUDIO_CP_START call codec Record faile", __LINE__);
                //return codec_result;
            }
            AUDIO_CP_status |=AUDIO_CP_RECORD_PLAYING;
        
        }else
        {
            SCI_TRACE_LOW("zgt:line[%d] record call cp faile AUDIO_CP_status=0x%x", __LINE__, AUDIO_CP_status);
            return AUDIO_PLAYING;
        }
        break;
    case    AUDIO_DEV_FM:
        if(AUDIO_CP_STOPPED == AUDIO_CP_status )
        {
            AUDIO_PMIC_26M_open(AUDIO_26M_USED_BY_FM);
            AUDIO_CP_SetDevCFG(devicecfg);
            zsp_ret = AUDIO_ZSP_START(zsp_para);
            if (0 != zsp_ret)
            {
                SCI_TRACE_LOW("[zgt] line[%d] AUDIO_CP_START fm call zsp Start faile", __LINE__);
            }
            AUDIO_CP_SetSharkingMode(g_shakingMode);
            codec_result = AUDIO_CP_StreamStart(itf, stream, cfg);
            if (0 != codec_result) 
            {
                SCI_TRACE_LOW("[zgt] line[%d] AUDIO_CP_START FM call codec  faile", __LINE__);
            }
            stream->startAddress = (uint32 *)zsp_sharemem->audInput;
            stream->length = AUDIO_INPUT_BUF_SIZE;
            cfg->appMode = SND_APP_MODE_FMRECORD;
            codec_result = AUDIO_CP_RecordStart(itf, stream, cfg);
            if (0 != codec_result) 
            {
                SCI_TRACE_LOW("[zgt] line[%d] AUDIO_CP_START FM call codec  faile", __LINE__);
            }
    
            AUDIO_CP_status |=AUDIO_CP_FM_PLAYING;
        }else
        {
            SCI_TRACE_LOW("zgt:line[%d] FM call cp faile AUDIO_CP_status=0x%x", __LINE__, AUDIO_CP_status);
            return AUDIO_PLAYING;
        }
        break;
    case    AUDIO_DEV_TONE:
        if(AUDIO_CP_STOPPED == AUDIO_CP_status || AUDIO_CP_VOICE_PLAYING == AUDIO_CP_status)
        {

            AUDIO_CP_status |=AUDIO_CP_TONE_PLAYING;
        }else
        {
            SCI_TRACE_LOW("zgt:line[%d] TONE call cp faile AUDIO_CP_status=0x%x", __LINE__, AUDIO_CP_status);
            return AUDIO_PLAYING;
        }
        break;
    case    AUDIO_DEV_MP4RECORD:
        if(AUDIO_CP_STOPPED == AUDIO_CP_status )
        {
            AUDIO_PMIC_26M_open(AUDIO_26M_USED_BY_RECORD);
            AUDIO_CP_SetDevCFG(devicecfg);
            zsp_ret = AUDIO_ZSP_START(zsp_para);
            if (0 != zsp_ret)
            {
                SCI_TRACE_LOW("[zgt] line[%d] AUDIO_CP_START call zsp Start faile", __LINE__);
                //return zsp_ret;
            }
            //start record stream
            codec_result = AUDIO_CP_RecordStart(itf, stream, cfg);
            if (0 != codec_result) 
            {
                SCI_TRACE_LOW("[zgt] line[%d] AUDIO_CP_START call codec Record faile", __LINE__);
                //return codec_result;
            }
            AUDIO_CP_status |=AUDIO_CP_MP4RECORD_PLAYING;
        }else
        {
            SCI_TRACE_LOW("zgt:line[%d] MP4RECORD call cp faile AUDIO_CP_status=0x%x", __LINE__, AUDIO_CP_status);
            return AUDIO_PLAYING;
        }
        break;
    case AUDIO_DEV_DUALCHN_PLAYRECORD:
        if(AUDIO_CP_STOPPED == AUDIO_CP_status || (g_shakingMode&&(AUDIO_CP_VOICE_PLAYING == AUDIO_CP_status)))
        {
            //DM_AudSetToneMode(g_toneMode);
            AUDIO_CP_SetToneMode(g_toneMode);
            AUDIO_PMIC_26M_open(AUDIO_26M_USED_BY_MUSIC);
            zsp_ret = AUDIO_ZSP_START(zsp_para);
            if (AUDIO_NO_ERROR != zsp_ret)
            {
                SCI_TRACE_LOW("zgt line[%d] AUDIO_CP_START call zsp Start faile ret:0x%x", __LINE__, zsp_ret);
                //return zsp_ret;
            }
            AUDIO_CP_SetSharkingMode(g_shakingMode);
            AUDIO_CP_VoisSetCfg(itf, Vois_cfg, devicecfg, btworkmode);
            codec_result = AUDIO_CP_StreamStart(itf, stream, cfg);
            if (AUDIO_NO_ERROR != codec_result)
            {
                SCI_TRACE_LOW("zgt line[%d] AUDIO_CP_START call codec Start faile ret:0x%x", __LINE__, codec_result);
                //return codec_result;
            }
            AUDIO_CP_status |=AUDIO_CP_MUSIC_PLAYING;
        }else
        {
            SCI_TRACE_LOW("zgt:line[%d] music call cp faile AUDIO_CP_status=0x%x", __LINE__, AUDIO_CP_status);
            return AUDIO_PLAYING;
        }
        break;
    default:
        break;
    }
    SCI_TRACE_LOW("zgt:line[%d] AUDIO_CP_START exit DEV type=0x%x,AUDIO_CP_status=0x%x", __LINE__, type,AUDIO_CP_status);
    return AUDIO_NO_ERROR;
}
PUBLIC AUDIO_RESULT_E AUDIO_CP_STOP(AUDIO_DEV_TYPE_E dev_type,SND_ITF_T itf, AUD_AP2CP_PARA_T *zsp_para)
{    
    AUDIO_RESULT_E codec_result = AUDIO_ERROR;
    AUDIO_RESULT_E zsp_ret = AUDIO_ERROR;
    AUDIO_RESULT_E vois_ret = AUDIO_ERROR;
    AUDIO_CODEC_STATUS_E CODEC_status = CODEC_STOPPED;
    AUDIO_DEV_TYPE_E type = 0;
    type = dev_type;
    SCI_TRACE_LOW("zgt:line[%d] AUDIO_CP_STOP DEV type=0x%x,AUDIO_CP_status=0x%x", __LINE__, type,AUDIO_CP_status);

    switch(dev_type)
    {
    case    AUDIO_DEV_MUSIC:
        if(AUDIO_CP_status&AUDIO_CP_MUSIC_PLAYING)
        {
            codec_result = AUDIO_CP_StreamStop(itf);
            if (0 != codec_result)
            {
                SCI_TRACE_LOW("zgt line[%d] AUDIO_CP_STOP call codec Stop faile", __LINE__);
                //return codec_result;
            }
            zsp_ret = AUDIO_ZSP_STOP(zsp_para);
            if (0 != zsp_ret)
            {
                SCI_TRACE_LOW("zgt line[%d] AUDIO_CP_STOP call zsp music Stop faile", __LINE__);
                //return zsp_ret;
            }
#ifndef AUDIO_CP_Synchronous_ITF
            CODEC_status = AUDIO_CP_GetCodecStatus(0);
            if( CODEC_PLAYING==CODEC_status)
            {
                SCI_TRACE_LOW("zgt AUDIO_CP_STOP CODEC ERROR =%d", CODEC_status);
            }
#endif
            AUDIO_PMIC_26M_close(AUDIO_26M_USED_BY_MUSIC);
            AUDIO_CP_status &= (~AUDIO_CP_MUSIC_PLAYING);
        }else
        {
            SCI_TRACE_LOW("zgt line[%d] AUDIO_CP_STOP faile because music not in playing", __LINE__);
            return AUDIO_ERROR;
        }
        break;
    case    AUDIO_DEV_LAYER1:
        if(0!= (AUDIO_CP_status&AUDIO_CP_VOICE_PLAYING))
        {
            vois_ret = AUDIO_VOIS_STOP();
            if (0 != vois_ret)
            {
            	SCI_TRACE_LOW("zgt line[%d] AUDIO_CP_STOP call DM_StopAudioEx dm_ret fail [0x%x] ", __LINE__, vois_ret);
                //return vois_ret;
            }
#ifndef AUDIO_CP_Synchronous_ITF
            CODEC_status = AUDIO_CP_GetCodecStatus(0);
            if( CODEC_PLAYING==CODEC_status)
            {
                SCI_TRACE_LOW("zgt AUDIO_CP_STOP CODEC ERROR =%d", CODEC_status);
            }
#endif
            AUDIO_PMIC_26M_close(AUDIO_26M_USED_BY_LAYER1);
            AUDIO_CP_status &= (~AUDIO_CP_VOICE_PLAYING);
        }else
        {
            SCI_TRACE_LOW("zgt line[%d] AUDIO_CP_STOP faile because voice not in playing", __LINE__);
            return AUDIO_ERROR;
        }
        break;
    case    AUDIO_DEV_RECORD:
        if(0!= (AUDIO_CP_status&AUDIO_CP_RECORD_PLAYING ))
        {
            codec_result = AUDIO_CP_RecordStop(itf);
            if (0 != codec_result)
            {
                SCI_TRACE_LOW("[zgt] line[%d] AUDIO_CP_STOP call codec Stop faile", __LINE__);
               // return codec_result;
            }
            zsp_ret = AUDIO_ZSP_STOP(zsp_para);
            if (0 != zsp_ret)
            {
                SCI_TRACE_LOW("[zgt] line[%d] AUDIO_CP_STOP call zsp Stop faile", __LINE__);
               // return zsp_ret;
            }
            AUDIO_PMIC_26M_close(AUDIO_26M_USED_BY_RECORD);
            AUDIO_CP_status &= (~AUDIO_CP_RECORD_PLAYING);
        }else
        {
            SCI_TRACE_LOW("zgt line[%d] AUDIO_CP_STOP faile because record not in playing", __LINE__);
            return AUDIO_ERROR;
        }
        break;
    case    AUDIO_DEV_FM:
        if(0!= (AUDIO_CP_status&AUDIO_CP_FM_PLAYING ))
        {
            codec_result = AUDIO_CP_StreamStop(itf);
            if (0 != codec_result)
            {
                SCI_TRACE_LOW("zgt line[%d] AUDIO_CP_STOP call codec Stop faile", __LINE__);
               // return codec_result;
            }
            codec_result = AUDIO_CP_RecordStop(itf);
            if (0 != codec_result)
            {
                SCI_TRACE_LOW("[zgt] line[%d] AUDIO_CP_STOP call codec Stop faile", __LINE__);
               // return codec_result;
            }
             zsp_ret = AUDIO_ZSP_STOP(zsp_para);
            if (0 != zsp_ret)
            {
                SCI_TRACE_LOW("[zgt] line[%d] AUDIO_CP_STOP call zsp Stop faile", __LINE__);
               // return zsp_ret;
            }
#ifndef AUDIO_CP_Synchronous_ITF
            CODEC_status = AUDIO_CP_GetCodecStatus(0);
            if( CODEC_PLAYING==CODEC_status)
            {
                SCI_TRACE_LOW("zgt AUDIO_CP_STOP CODEC ERROR =%d", CODEC_status);
            }
#endif
            AUDIO_PMIC_26M_close(AUDIO_26M_USED_BY_FM);
            AUDIO_CP_status &= (~AUDIO_CP_FM_PLAYING);
        }else
        {
            SCI_TRACE_LOW("zgt line[%d] AUDIO_CP_STOP faile because FM not in playing", __LINE__);
            return AUDIO_ERROR;
        }
        break;
    case    AUDIO_DEV_TONE:
        if(0!= (AUDIO_CP_status&AUDIO_CP_TONE_PLAYING ))
        {
        
            AUDIO_CP_status &= (~AUDIO_CP_TONE_PLAYING);
        }else
        {
            SCI_TRACE_LOW("zgt line[%d] AUDIO_CP_STOP faile because TONE not in playing", __LINE__);
            return AUDIO_ERROR;
        }
        break;
    case    AUDIO_DEV_MP4RECORD:
        if(0!= (AUDIO_CP_status&AUDIO_CP_MP4RECORD_PLAYING ))
        {
            codec_result = AUDIO_CP_RecordStop(itf);
            if (0 != codec_result)
            {
                SCI_TRACE_LOW("[zgt] line[%d] AUDIO_CP_STOP MP4 call codec Stop faile", __LINE__);
            }
             zsp_ret = AUDIO_ZSP_STOP(zsp_para);
            if (0 != zsp_ret)
            {
                SCI_TRACE_LOW("[zgt] line[%d] AUDIO_CP_STOP MP4 call zsp Stop faile", __LINE__);
            }
            AUDIO_PMIC_26M_close(AUDIO_26M_USED_BY_RECORD);
            AUDIO_CP_status &= (~AUDIO_CP_MP4RECORD_PLAYING);
        }else
        {
            SCI_TRACE_LOW("zgt line[%d] AUDIO_CP_STOP faile because MP4RECORD not in playing", __LINE__);
            return AUDIO_ERROR;
        }
        break;
    case AUDIO_DEV_DUALCHN_PLAYRECORD:
        if(AUDIO_CP_status&AUDIO_CP_MUSIC_PLAYING)
        {
            codec_result = AUDIO_CP_StreamStop(itf);
            if (0 != codec_result)
            {
                SCI_TRACE_LOW("zgt line[%d] AUDIO_CP_STOP call codec Stop faile", __LINE__);
                //return codec_result;
            }
            zsp_ret = AUDIO_ZSP_STOP(zsp_para);
            if (0 != zsp_ret)
            {
                SCI_TRACE_LOW("zgt line[%d] AUDIO_CP_STOP call zsp music Stop faile", __LINE__);
                //return zsp_ret;
            }
#ifndef AUDIO_CP_Synchronous_ITF
            CODEC_status = AUDIO_CP_GetCodecStatus(0);
            if( CODEC_PLAYING==CODEC_status)
            {
                SCI_TRACE_LOW("zgt AUDIO_CP_STOP CODEC ERROR =%d", CODEC_status);
            }
#endif
            AUDIO_PMIC_26M_close(AUDIO_26M_USED_BY_MUSIC);
            AUDIO_CP_status &= (~AUDIO_CP_MUSIC_PLAYING);
        }else
        {
            SCI_TRACE_LOW("zgt line[%d] AUDIO_CP_STOP faile because music not in playing", __LINE__);
            return AUDIO_ERROR;
        }
        break;
            default:
        break;
    }
    SCI_TRACE_LOW("zgt:line[%d] AUDIO_CP_STop exit DEV type=0x%x,AUDIO_CP_status=0x%x", __LINE__, type,AUDIO_CP_status);
    return AUDIO_NO_ERROR;
}

INT32 AUDIO_ZSP_VoiceVibrateStart(UINT32 time)
{
    SCI_TRACE_LOW("ljf line[%d] AUDIO_ZSP_VoiceVibrateStart start vibrate from zsp duation:%d", __LINE__, time);
    return DM_ZspVoiceVibrateStart(time);
}

INT32 AUDIO_ZSP_VoiceVibrateStop(void)
{
    SCI_TRACE_LOW("ljf line[%d] AUDIO_ZSP_VoiceVibrateStart stop vibrate from zsp", __LINE__);
    return DM_ZspVoiceVibrateStop();
}

INT32 AUDIO_ZSP_MusicVibrateStart(UINT32 time)
{
    SCI_TRACE_LOW("ljf line[%d] AUDIO_ZSP_MusicVibrateStart start vibrate from zsp duation:%d", __LINE__, time);
    return DM_ZspMusicVibrateStart(time);
}

INT32 AUDIO_ZSP_MusicVibrateStartAllTime(void)
{
    SCI_TRACE_LOW("ljf line[%d] AUDIO_ZSP_MusicVibrateStartAllTime start vibrate from zsp", __LINE__);
    return DM_ZspMusicVibrateStart(0x7FFFFFFF);
}

INT32 AUDIO_ZSP_MusicVibrateStop(void)
{
    SCI_TRACE_LOW("ljf line[%d] AUDIO_ZSP_MusicVibrateStop stop vibrate from zsp", __LINE__);
    return DM_ZspMusicVibrateStop();
}
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
    
#endif  // End of l1rec_dev.c

