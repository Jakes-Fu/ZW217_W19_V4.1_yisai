/******************************************************************************
 ** File Name:      audio_dai_aif.c                                           *
 ** Author:         jen.li                                                 *
 ** DATE:           6/21/2022                                                *
 ** Copyright:      Copyright (c) 2022 Unisoc Communications Inc.         *
 ** Description:    This file define the basic hw interfaces of audio device. *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 6/21/2022      jen.li           Create.                                   *
 ******************************************************************************/
#ifndef _AUDIO_DAI_AIF_H_
#define _AUDIO_DAI_AIF_H_

/*----------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **------------------------------------------------------------------------- */
#include "os_api.h"
#include "sci_types.h"
#include "uws6121e_int_cfg.h"
#include "chip_plf_export.h"

#include "audio_dai.h"
#include "audio_drvapi.h"

#include "audio_ifc_phy.h"
#include "audio_aif_phy.h"
#include "mmu_drvapi.h"
/**---------------------------------------------------------------------------*
 **                             Compiler Flag                                 *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif
/**---------------------------------------------------------------------------*
**                               Micro Define                                **
**---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/
typedef struct
{
    uint32                  addr;
    uint32                  size;
    uint32                  pWrite;
    uint32                  pRead;
    uint32                  cnt;
    uint32                  max_cnt;
}AIF_BUFFER_T;

typedef struct
{
    uint32                    play:1;
    uint32                    record:1;
    uint32                    loop:1;
    uint32                    i2s:1;
    uint32                    voice:1;
    uint32                    da_trigger:1;
    uint32                    ad_trigger:1;
    uint32                    send_ready:1;
    uint32                    rsve:24;
} AIF_OPENED_T;

typedef struct
{
    //char                    magic_str[AUDIO_MAGIC_STR_MAX_LEN];

    AIF_OPENED_T            opened;
    //uint32                  support_data_fmt;
    //uint32                  data_fmt[2];
    AIF_BUFFER_T            playbuffer;
    AIF_BUFFER_T            recordbuffer;
    AUDIO_HAL_CALLBACK_FUN  cb[2];

    uint32                  fs;
    uint32                  ch;
    //VBC_DMA_T               dma_L[2];
    //VBC_DMA_T               dma_R[2];

    //uint32                  save_pos[2];
    uint32                  need_call_cb[2];

    //DEVICE_HANDLE           dev_audio;

    //uint32                  buffer_div[2];


    //uint32                  magic_num;
    uint32 fun_flg;

} AIF_DAI_T;

/**---------------------------------------------------------------------------*
 **                         Local variables                                   *
 **---------------------------------------------------------------------------*/
LOCAL AIF_DAI_T s_aif_cfg;

// empty buffer for tone
#define STREAM_BUFFER_LEN (1024)
char streambuffer[STREAM_BUFFER_LEN];

/**----------------------------------------------------------------------------*
**                         Local Function Prototype                           **
**----------------------------------------------------------------------------*/
LOCAL void _AIF_PLAY_I2S_Callback(uint32 param);
LOCAL void _AIF_RECORD_I2S_Callback(uint32 param);

/*****************************************************************************/
// Description :    AIF Init.
//                  this is DAI Init etc.
// Return :         AUDIO_HAL_SUCCESS
// Author :         jen.li
// Note :
/*****************************************************************************/
LOCAL int32 _AIF_Init(uint32 param)
{
    static uint32 inited = 0;
    if (!inited) {
        s_aif_cfg.fs = 0;
        s_aif_cfg.ch = 0;
        s_aif_cfg.fun_flg = AUDIO_FUN_NONE;
        s_aif_cfg.opened.play = FALSE;
        s_aif_cfg.opened.record = FALSE;

        ISR_RegHandler(TB_AIF_APB1_INT, _AIF_PLAY_I2S_Callback);
        ISR_RegHandler(TB_AIF_APB0_INT, _AIF_RECORD_I2S_Callback);

        inited = 1;
    }
    AUDDRV_TRACE("ok");
    return AUDIO_HAL_SUCCESS;
}

/*****************************************************************************/
// Description :    AIF  Open.
//                  this is DAI Enable.
// Return :         AUDIO_HAL_SUCCESS,AUDIO_HAL_ERROR
// Param :          fun_flg : from AUDIO_FUN_E
//                  fs : frame sample rate.
// Author :         jen.li
// Note :
/*****************************************************************************/
LOCAL int32 _AIF_Open(uint32 fun_flg,uint32 fs)
{
    uint32 channel;

    // fun_flg check
    if ((AUDIO_FUN_PLAYBACK != fun_flg) &&
        (AUDIO_FUN_CAPTRUE != fun_flg)) {
       AUDDRV_ERROR("dai open {fun_flg=0x%x (0x%x)} err", fun_flg, s_aif_cfg.fun_flg);
       return AUDIO_HAL_ERROR;
   }

    // s_aif_cfg.fun_flg check
    if (AUDIO_FUN_NONE != s_aif_cfg.fun_flg) {
        if ((fs == s_aif_cfg.fs) && !(fun_flg & s_aif_cfg.fun_flg)) {
            s_aif_cfg.fun_flg |= fun_flg;
            AUDDRV_TRACE("dai open {fun_flg=0x%x (0x%x) fs=%d ch=%d} directly",
                fun_flg, s_aif_cfg.fun_flg, s_aif_cfg.fs, s_aif_cfg.ch);
            return AUDIO_HAL_SUCCESS;
        } else {
            AUDDRV_ERROR("dai open {fun_flg=0x%x (0x%x) fs=%d (%d)} err",
                fun_flg, s_aif_cfg.fun_flg, fs, s_aif_cfg.fs);
            return AUDIO_HAL_ERROR;
        }
    }

    AUDDRV_DEBUG("dai open {fun_flg=0x%x (0x%x) fs=%d ch=%d} entry",
        fun_flg, s_aif_cfg.fun_flg, fs, s_aif_cfg.ch);

    // open aif
    if(AUDIO_HAL_SUCCESS != __aif_open(fs, s_aif_cfg.ch)) {
        AUDDRV_ERROR("__aif_open {fs=%d ch=%d} err", fs, s_aif_cfg.ch);
        return AUDIO_HAL_ERROR;
    }

    // update param
    s_aif_cfg.fs = fs;
    s_aif_cfg.fun_flg |= fun_flg;

    AUDDRV_TRACE("dai open {fun_flg=0x%x (0x%x) fs=%d ch=%d} exit",
        fun_flg, s_aif_cfg.fun_flg, fs, s_aif_cfg.ch);

    return AUDIO_HAL_SUCCESS;
}

/*****************************************************************************/
// Description :    AIF  Close.
//                  this is  DAI Disable.
// Return :         AUDIO_HAL_SUCCESS
// Param :          fun_flg : from AUDIO_FUN_E
// Author :         jen.li
// Note :
/*****************************************************************************/
LOCAL int32 _AIF_Close(uint32 fun_flg)
{
    int32 ret = AUDIO_HAL_SUCCESS;

    // fun_flg check
    if ((AUDIO_FUN_PLAYBACK != fun_flg) &&
        (AUDIO_FUN_CAPTRUE != fun_flg)) {
        AUDDRV_ERROR("dai close {fun_flg=0x%x} err", fun_flg);
        return AUDIO_HAL_ERROR;
    }

    // s_aif_cfg.fun_flg check
    if (!(fun_flg & s_aif_cfg.fun_flg)) {
        AUDDRV_TRACE("dai close {fun_flg=0x%x (0x%x)} already",
            fun_flg, s_aif_cfg.fun_flg);
        return AUDIO_HAL_SUCCESS;
    }

    AUDDRV_DEBUG("dai close {fun_flg=0x%x (0x%x)} entry",
        fun_flg, s_aif_cfg.fun_flg);

    if (AUDIO_FUN_PLAYBACK == fun_flg) {

        // clr irq
        CHIPDRV_DisableIRQINT(TB_AIF_APB1_INT);

        __aif_playback_stop();
        ret = __ifc_StopPlay();
        s_aif_cfg.opened.play = FALSE;

        s_aif_cfg.playbuffer.addr  = 0;
        s_aif_cfg.playbuffer.size = 0;
        s_aif_cfg.playbuffer.pWrite  = 0;
        s_aif_cfg.playbuffer.pRead   = 0;

    } else {    // if (AUDIO_FUN_CAPTRUE == fun_flg)

        // clr irq
        CHIPDRV_DisableIRQINT(TB_AIF_APB0_INT);

        __aif_capture_stop();
        ret = __ifc_StopRecord();
        s_aif_cfg.opened.record = FALSE;

        s_aif_cfg.recordbuffer.addr  = 0;
        s_aif_cfg.recordbuffer.size = 0;
        s_aif_cfg.recordbuffer.pWrite  = 0;
        s_aif_cfg.recordbuffer.pRead   = 0;
    }

    if (AUDIO_HAL_SUCCESS != ret) {
        AUDDRV_TRACE("__ifc_StopPlay/__ifc_StopRecord(%d) failed", ret);
    }

    s_aif_cfg.fun_flg &= ~fun_flg;

    if (__aif_can_close() || (AUDIO_FUN_NONE == s_aif_cfg.fun_flg)) {
        AUDDRV_TRACE("__aif_close");
        __aif_close();
        s_aif_cfg.fs = 0;
        s_aif_cfg.ch = 0;
        s_aif_cfg.fun_flg = AUDIO_FUN_NONE;
    }

    AUDDRV_TRACE("dai close {fun_flg=0x%x (0x%x)} exit",
        fun_flg, s_aif_cfg.fun_flg);
    return AUDIO_HAL_SUCCESS;
}

/*****************************************************************************/
// Description :    AIF  Ioctl.
//                  this is  DAI Ioctl.
// Return :         AUDIO_HAL_SUCCESS,AUDIO_HAL_ERROR
// Param :          cmd : from CODEC_CMD_E and DAI_CMD_E
//                  arg : some cmd maybe use NULL if no param
//                      if the cmd have param,please give a non-NULL point!
// Author :         jen.li
// Note :
/*****************************************************************************/
LOCAL int32 _AIF_Ioctl(uint32 cmd, uint32 arg)
{
    int32 ret = AUDIO_HAL_SUCCESS;
    AUDIO_CTL_ARG_U* ioctl_arg = (AUDIO_CTL_ARG_U*)arg;
    uint32 tmp=0;

    AUDDRV_TRACE("ioctl aif {cmd %d arg 0x%x}", cmd, arg);

    switch(cmd) {
    case AUDIO_DAI_CMD_FS_CHECK:
        tmp = __aif_fs_check(ioctl_arg->fs_check);
        AUDDRV_TRACE("ioctl dai fs check %d result %d", ioctl_arg->fs_check, tmp);
        ioctl_arg->fs_check  = tmp;
        break;

    case AUDIO_DAI_CMD_TONE:
        // tone play need __aif_playback_start
        if (TONE_CMD_START == ioctl_arg->tone.mBits.cmd) {
            ret = __aif_tone_start(ioctl_arg->tone.mBits.type,
                                    ioctl_arg->tone.mBits.gain);
        } else if (TONE_CMD_STOP == ioctl_arg->tone.mBits.cmd) {
            ret = __aif_tone_stop();
        } else if (TONE_CMD_PAUSE == ioctl_arg->tone.mBits.cmd) {
            ret = __aif_tone_pasue();
        } else if (TONE_CMD_RESUME == ioctl_arg->tone.mBits.cmd) {
            ret = __aif_tone_resume();
        } else {
            ret = AUDIO_HAL_ERROR;
            AUDDRV_ERROR("ioctl dai tone unknow cmd %d", ioctl_arg->tone.mBits.cmd);
        }
        break;

    case AUDIO_DAI_CMD_CHANNEL_SET:
        if ((AUDIO_CH_MONO > ioctl_arg->channel) ||
            (AUDIO_CH_MAX <= ioctl_arg->channel)) {
            AUDDRV_ERROR("ioctl dai set invalid channel %d", ioctl_arg->channel);
            ret = AUDIO_HAL_ERROR;
        } else if ((AUDIO_FUN_NONE != s_aif_cfg.fun_flg) &&
            (s_aif_cfg.ch != ioctl_arg->channel)) {
            AUDDRV_ERROR("ioctl dai set different channel %d!=%d",
                s_aif_cfg.ch, ioctl_arg->channel);
            ret = AUDIO_HAL_ERROR;
        } else {
            s_aif_cfg.ch = ioctl_arg->channel;
            AUDDRV_DEBUG("ioctl dai set channel %d", s_aif_cfg.ch);
        }
        break;

    case AUDIO_DAI_CMD_CHANNEL_GET:
        ioctl_arg->channel = s_aif_cfg.ch;
        break;

    default:
        ret = AUDIO_HAL_ERROR;
        AUDDRV_TRACE("ioctl dai unknow cmd %d", cmd);
        break;
    }

    return ret;
}


/*****************************************************************************/
// Description :    AIF  Set Playback Buffer Info.
//
// Return :         AUDIO_HAL_SUCCESS
// Param :          pLHead : if AUDIO_DATA_FMT_LL_RR ,
//                      the Left channel data start point.
//                      if AUDIO_DATA_FMT_LR_LR or AUDIO_DATA_FMT_MONO
//                      , the data start point.
//                  pRHead : if AUDIO_DATA_FMT_LL_RR ,
//                      the Right channel data start point.
//                      if AUDIO_DATA_FMT_LR_LR or AUDIO_DATA_FMT_MONO
//                      , will ignore.
//                  cnt : the max size of pLHead.
//                      well, pRHead MUST the same size with pLHead if exist.
// Author :         jen.li
// Note :
/*****************************************************************************/
LOCAL int32 _AIF_DAC_Set_Buf_Info(void *pLHead,void *pRHead,uint32 size)
{
    s_aif_cfg.playbuffer.addr  = (uint32)pLHead;
    s_aif_cfg.playbuffer.size = size;
    s_aif_cfg.playbuffer.pWrite  = 0;
    s_aif_cfg.playbuffer.pRead   = 0;

    AUDDRV_TRACE("set {addr=0x%x size=%d}",
        s_aif_cfg.playbuffer.addr, s_aif_cfg.playbuffer.size);

    return AUDIO_HAL_SUCCESS;
}

LOCAL int32 _AIF_DAC_Set_Write_Point(uint32 pWrite)
{
    s_aif_cfg.playbuffer.pWrite = pWrite;
    return AUDIO_HAL_SUCCESS;
}

LOCAL int32 _AIF_DAC_Set_Read_Point(uint32 pRead)
{
    s_aif_cfg.playbuffer.pRead = pRead;
    return AUDIO_HAL_SUCCESS;
}

LOCAL int32 _AIF_DAC_Get_Read_Point(uint32* pRead)
{
    *pRead = s_aif_cfg.playbuffer.pRead;
    return AUDIO_HAL_SUCCESS;
}

/*****************************************************************************/
// Description :    AIF  Will Trigger Playback immediately.
//
// Return :         AUDIO_HAL_SUCCESS,AUDIO_HAL_ERROR
// Param :
//
// Author :         jen.li
// Note :           do nothing when already trigger.
/*****************************************************************************/
LOCAL int32 _AIF_DAC_Trigger(void)
{
    int32 ret1, ret2;
    if ((FALSE == s_aif_cfg.opened.play) &&
        (AUDIO_FUN_PLAYBACK & s_aif_cfg.fun_flg)) {

        // enabel irq
        CHIPDRV_EnableIRQINT (TB_AIF_APB1_INT);

        if (s_aif_cfg.playbuffer.addr) {
            ret1 = __ifc_start(s_aif_cfg.playbuffer.addr,
                                s_aif_cfg.playbuffer.size,
                                AUDIO_IFC_PLAYBACK,
                                s_aif_cfg.opened.record);
            AUDDRV_TRACE("dac trigger with playbuffer buffer {0x%x %d}",
                s_aif_cfg.playbuffer.addr, s_aif_cfg.playbuffer.size);
        } else {
            /* empty buffer for tone */
            SCI_MEMSET(streambuffer, 0, STREAM_BUFFER_LEN);
            ret1 = __ifc_start(streambuffer,
                                STREAM_BUFFER_LEN,
                                AUDIO_IFC_PLAYBACK,
                                s_aif_cfg.opened.record);
            AUDDRV_TRACE("dac trigger with streambuffer {0x%x %d}",
                streambuffer, STREAM_BUFFER_LEN);
        }

        ret2 = __aif_playback_start();
        if (AUDIO_HAL_SUCCESS != ret1 || AUDIO_HAL_SUCCESS != ret2) {
            AUDDRV_ERROR("failed %d %d", ret1, ret2);
            // clr irq

            CHIPDRV_DisableIRQINT(TB_AIF_APB1_INT);

            return AUDIO_HAL_ERROR;
        }
        s_aif_cfg.opened.play = TRUE;
        AUDDRV_TRACE("DAC start succsee");
    }
    return AUDIO_HAL_SUCCESS;
}

/*****************************************************************************/
// Description :    AIF  Capture Buffer Info.
//
// Return :         AUDIO_HAL_SUCCESS
// Param :          pLHead : if AUDIO_DATA_FMT_LL_RR ,
//                      the Left channel data start point.
//                      if AUDIO_DATA_FMT_LR_LR or AUDIO_DATA_FMT_MONO
//                      , the data start point.
//                  pRHead : if AUDIO_DATA_FMT_LL_RR ,
//                      the Right channel data start point.
//                      if AUDIO_DATA_FMT_LR_LR or AUDIO_DATA_FMT_MONO
//                      , will ignore.
//                  cnt : the max size of pLHead.
//                      well, pRHead MUST the same size with pLHead if exist.
// Author :         jen.li
// Note :
/*****************************************************************************/
LOCAL int32 _AIF_ADC_Set_Buf_Info(void *pLHead,void *pRHead,uint32 size)
{
    s_aif_cfg.recordbuffer.addr  = (uint32)pLHead;
    s_aif_cfg.recordbuffer.size = size;
    s_aif_cfg.recordbuffer.pWrite  = 0;
    s_aif_cfg.recordbuffer.pRead   = 0;
    return AUDIO_HAL_SUCCESS;
}

LOCAL int32 _AIF_ADC_Set_Read_Point(uint32 pRead)
{
    s_aif_cfg.recordbuffer.pRead = pRead;
    return AUDIO_HAL_SUCCESS;
}

LOCAL int32 _AIF_ADC_Set_Write_Point(uint32 pWrite)
{
    s_aif_cfg.recordbuffer.pWrite = pWrite;
    return AUDIO_HAL_SUCCESS;
}

LOCAL int32 _AIF_ADC_Get_Write_Point(uint32* pWrite)
{
   *pWrite = s_aif_cfg.recordbuffer.pWrite;
    return AUDIO_HAL_SUCCESS;
}

/*****************************************************************************/
// Description :    AIF  Will Trigger Captrue immediately.
//
// Return :         AUDIO_HAL_SUCCESS,AUDIO_HAL_ERROR
// Param :
//
// Author :         jen.li
// Note :           do nothing when already trigger.
/*****************************************************************************/
LOCAL int32 _AIF_ADC_Trigger(void)
{
    int32 ret1, ret2;
    if ((FALSE == s_aif_cfg.opened.record) &&
        (AUDIO_FUN_CAPTRUE & s_aif_cfg.fun_flg)) {

        // enable irq
        CHIPDRV_EnableIRQINT (TB_AIF_APB0_INT);

        ret1 = __ifc_start(s_aif_cfg.recordbuffer.addr,
                            s_aif_cfg.recordbuffer.size,
                            AUDIO_IFC_CAPTURE,
                            s_aif_cfg.opened.play);
        ret2 = __aif_capture_start();
        if (AUDIO_HAL_SUCCESS != ret1 || AUDIO_HAL_SUCCESS != ret2) {
            AUDDRV_TRACE("failed %d %d", ret1, ret2);
            // clr irq

            CHIPDRV_DisableIRQINT(TB_AIF_APB0_INT);

            return AUDIO_HAL_ERROR;
        }
        s_aif_cfg.opened.record= TRUE;
        AUDDRV_TRACE("ADC start success");
    }
    return AUDIO_HAL_SUCCESS;
}

/*****************************************************************************/
// Description :    AIF  Register Playback Callback.
//                  Use to Notify User the Driver fetch data gone.
//                  User can do something like start decode audio data.
// Return :         AUDIO_HAL_SUCCESS,AUDIO_HAL_ERROR
// Param :          pFun : this function.
//
// Author :         jen.li
// Note :  This Callback DO NOT call deep tree.
//          should be send message or set flag etc.
/*****************************************************************************/
LOCAL int32 _AIF_Register_Playback(AUDIO_HAL_CALLBACK_FUN pFun)
{
    AUDDRV_TRACE("entry {Playback cb=0x%x}",pFun);
    s_aif_cfg.cb[AUDIO_PLAYBACK]  = pFun;

    return AUDIO_HAL_SUCCESS;
}


/*****************************************************************************/
// Description :    AIF  Register Capture Callback.
//                  Use to Notify User for fetch data please.
//                  User should be move audio data ASAP.
// Return :         AUDIO_HAL_SUCCESS,AUDIO_HAL_ERROR
// Param :          pFun : this function.
//
// Author :         jen.li
// Note :  This Callback DO NOT call deep tree.
//          should be send message or set flag etc.
/*****************************************************************************/
LOCAL int32 _AIF_Register_Capture(AUDIO_HAL_CALLBACK_FUN pFun)
{
    AUDDRV_TRACE("entry {Capture cb=0x%x}",pFun);
    s_aif_cfg.cb[AUDIO_CAPTRUE] = pFun;

    return AUDIO_HAL_SUCCESS;
}

LOCAL void _AIF_PLAY_I2S_Callback(uint32 param)
{
    if(__ifc_get_cause_ihf_status(AUDIO_FUN_PLAYBACK)){
        AUDDRV_DEBUG("in half %d",SCI_GetTickCount());
        _AIF_DAC_Set_Read_Point(s_aif_cfg.playbuffer.size/2);
    } else if(__ifc_get_cause_ief_status(AUDIO_FUN_PLAYBACK)){
        AUDDRV_DEBUG("in end %d",SCI_GetTickCount());
        _AIF_DAC_Set_Read_Point(0);
    }

    if( s_aif_cfg.cb[AUDIO_PLAYBACK] )
    {
        s_aif_cfg.cb[AUDIO_PLAYBACK](s_aif_cfg.playbuffer.size/2);
    }

    // sync
    MMU_DmaCacheSync(
        (uint32)s_aif_cfg.playbuffer.addr,
        s_aif_cfg.playbuffer.size,
        DMABUFFER_TO_DEVICE);

    //__aif_print_reg();

    // clear irq
    __ifc_set_int_clear_end_fifo(AUDIO_FUN_PLAYBACK);
    __ifc_set_int_clear_half_fifo(AUDIO_FUN_PLAYBACK);
}

LOCAL void _AIF_RECORD_I2S_Callback(uint32 param)
{

    if(__ifc_get_cause_ihf_status(AUDIO_FUN_CAPTRUE)){
        AUDDRV_DEBUG("in half %d",SCI_GetTickCount());
        _AIF_ADC_Set_Write_Point(s_aif_cfg.recordbuffer.size/2);
    } else if(__ifc_get_cause_ief_status(AUDIO_FUN_CAPTRUE)){
        AUDDRV_DEBUG("in end %d",SCI_GetTickCount());
        _AIF_ADC_Set_Write_Point(0);
    }

    if( s_aif_cfg.cb[AUDIO_CAPTRUE] )
    {
        s_aif_cfg.cb[AUDIO_CAPTRUE](s_aif_cfg.recordbuffer.size/2);
    }

    // sync
    MMU_DmaCacheSync(
        (uint32)s_aif_cfg.recordbuffer.addr,
        s_aif_cfg.recordbuffer.size,
        DMABUFFER_FROM_DEVICE);

    //__aif_print_reg();

    // clear irq
    __ifc_set_int_clear_end_fifo(AUDIO_FUN_CAPTRUE);
    __ifc_set_int_clear_half_fifo(AUDIO_FUN_CAPTRUE);
}

/**---------------------------------------------------------------------------*
 **                         LOCAL Data                                        *
 **---------------------------------------------------------------------------*/

LOCAL AUD_DAI_OPS_T s_audio_dai_aif_ops=
{
    _AIF_Init,
    _AIF_Open,
    _AIF_Close,
    _AIF_Ioctl,
    _AIF_DAC_Set_Buf_Info,
    _AIF_DAC_Set_Write_Point,
    _AIF_DAC_Get_Read_Point,
    _AIF_DAC_Trigger,
    _AIF_ADC_Set_Buf_Info,
    _AIF_ADC_Set_Read_Point,
    _AIF_ADC_Get_Write_Point,
    _AIF_ADC_Trigger,
    _AIF_Register_Playback,
    _AIF_Register_Capture,
};

/**----------------------------------------------------------------------------*
**                           Function Prototype                               **
**----------------------------------------------------------------------------*/
PUBLIC AUD_DAI_OPS_T* AUDIO_DAI_Get_OPS(uint32 param)
{
    return (AUD_DAI_OPS_T*)(&(s_audio_dai_aif_ops));
}

/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif
/**---------------------------------------------------------------------------*/
#endif //_AUDIO_DAI_H_
// End
