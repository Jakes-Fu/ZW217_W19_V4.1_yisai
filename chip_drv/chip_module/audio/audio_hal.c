/******************************************************************************
 ** File Name:      audio_hal.c                                               *
 ** Author:         rui.huang                                                 *
 ** DATE:           06/28/2022                                                *
 ** Copyright:      2011 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file define the basic hw interfaces of audio device. *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 06/28/2022     rui.huang        Create.                                   *
 ******************************************************************************/

/*----------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **------------------------------------------------------------------------- */
#include "chip_drv_trc.h"
#include "sci_types.h"
#include "sci_api.h"
#include "priority_system.h"
#include "chip_plf_export.h"

#include "chg_drvapi.h"
#include "audio_drvapi.h"

#include "audio_dai.h"
#include "audio_codec.h"
#include "audio_hal.h"
#include "deepsleep_drvapi.h"
#include "analog_drvapi.h"
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
#define AUDIO_HAL_MUTEX_NAME "AUDIO HAL MUTEX"

#ifdef AUDIO_HAL_DEBUG
#define AUDIO_HAL_DBG SCI_TRACE_LOW
#define AUDIO_HAL_DBG_ASS SCI_ASSERT
#else
#define AUDIO_HAL_DBG(...)
#define AUDIO_HAL_DBG_ASS(x)
#endif

#define AUDIO_HAL_CALL(cb, rt)               \
    do                                       \
    {                                        \
        if ((cb))                            \
        {                                    \
            if (AUDIO_HAL_SUCCESS != (cb)()) \
                rt = AUDIO_HAL_ERROR;        \
        }                                    \
        else                                 \
        {                                    \
            rt = AUDIO_HAL_ERROR;            \
        }                                    \
    } while (0)

#define AUDIO_HAL_CALL1(cb, param, rt)            \
    do                                            \
    {                                             \
        if ((cb))                                 \
        {                                         \
            if (AUDIO_HAL_SUCCESS != (cb)(param)) \
                rt = AUDIO_HAL_ERROR;             \
        }                                         \
        else                                      \
        {                                         \
            rt = AUDIO_HAL_ERROR;                 \
        }                                         \
    } while (0)

#define AUDIO_HAL_CALL2(cb, param1, parm2, rt)            \
    do                                                    \
    {                                                     \
        if ((cb))                                         \
        {                                                 \
            if (AUDIO_HAL_SUCCESS != (cb)(param1, parm2)) \
                rt = AUDIO_HAL_ERROR;                     \
        }                                                 \
        else                                              \
        {                                                 \
            rt = AUDIO_HAL_ERROR;                         \
        }                                                 \
    } while (0)

#define AUDIO_HAL_CALL3(cb, param1, parm2, parm3, rt)            \
    do                                                           \
    {                                                            \
        if ((cb))                                                \
        {                                                        \
            if (AUDIO_HAL_SUCCESS != (cb)(param1, parm2, parm3)) \
                rt = AUDIO_HAL_ERROR;                            \
        }                                                        \
        else                                                     \
        {                                                        \
            rt = AUDIO_HAL_ERROR;                                \
        }                                                        \
    } while (0)

#define AUDIO_HAL_IRQ_ENTRY() do{ SCI_DisableIRQ();}while(0)
#define AUDIO_HAL_IRQ_LEAVE() do{ SCI_RestoreIRQ();}while(0)

#define AUDIO_HAL_MUTEX_ENTRY(mutex) do{ \
        uint32 local_ret = SCI_ERROR; \
        if (SCI_InThreadContext()) { \
            if ( NULL != mutex ) {\
                local_ret = SCI_GetMutex(mutex, SCI_INVALID_BLOCK_ID != \
                SCI_IdentifyThread() ? SCI_WAIT_FOREVER : 0); \
            } \
        } \
        AUDIO_HAL_DBG_ASS( local_ret == SCI_SUCCESS );/*assert verified*/\
    }while(0)

#define AUDIO_HAL_MUTEX_LEAVE(mutex) do{ \
        uint32 local_ret = SCI_ERROR;\
        if (SCI_InThreadContext()) { \
            if ( NULL != mutex ) { \
                local_ret = SCI_PutMutex( mutex ); \
            } \
        } \
        AUDIO_HAL_DBG_ASS( local_ret == SCI_SUCCESS );/*assert verified*/\
    }while(0)


/**---------------------------------------------------------------------------*
 **                         Data Structure                                    *
 **---------------------------------------------------------------------------*/
typedef struct
{
    AUD_CODEC_OPS_T *codec_ops;
    AUD_DAI_OPS_T *dai_ops;
    AUDIO_HAL_CALLBACK_FUN cb[2];

    SCI_MUTEX_PTR mutex;
    //SCI_MUTEX_PTR sleep_mutex;

    /*
    only tone can play with playback or voice mode
    but samplerate and channel should set the same
    possible fun_flg:
    AUDIO_HAL_PLAYBACK                      only playback
    AUDIO_HAL_CAPTURE                       only captur
    AUDIO_HAL_TONE                          only tone (AUDIO_HAL_PLAYBACK for dai/codec)
*/
    uint32 fun_flg;

    uint32 fs;
    uint32 ch;
} AUDIO_HAL_T;

/**---------------------------------------------------------------------------*
 **                         Local variables                                    *
 **---------------------------------------------------------------------------*/
LOCAL AUDIO_HAL_T s_audio_hal;

/**---------------------------------------------------------------------------*
 **                         Local Function                                    *
 **---------------------------------------------------------------------------*/

LOCAL int32 _AUDIO_HAL_Parse_Fun_Flg(uint32 in_flg, uint32 now_flg, uint32 *out_flg_p, uint32 oc)
{
    uint32 tmp_flg = now_flg;

    // NONE means not operate the dai/codec
    *out_flg_p = AUDIO_HAL_NONE;

    // in_flg can only deal one single flg once
    AUDHAL_DEBUG("in_flg 0x%x, now_flg 0x%x, oc %d", in_flg, now_flg, oc);

    if (oc) {
        /* open */
        if (in_flg & now_flg) {
            AUDHAL_ERROR("reopen flg err");
            goto pares_failed;
        }

        switch (in_flg) {
        case AUDIO_HAL_PLAYBACK:
        case AUDIO_HAL_TONE:
            if ((AUDIO_HAL_PLAYBACK | AUDIO_HAL_TONE) & now_flg) {
                *out_flg_p = AUDIO_HAL_NONE;    // no need to open
            } else {
                *out_flg_p = AUDIO_HAL_PLAYBACK;
            }
            break;
        case AUDIO_HAL_CAPTURE:
            *out_flg_p = in_flg;
            break;
        case AUDIO_HAL_NONE:
        default:
            AUDHAL_ERROR("unknown in_flg 0x%x", in_flg);
            goto pares_failed;
            break;
        }
    } else {
        /* close */
        if (!((in_flg) & (now_flg))) {
            AUDHAL_ERROR("close flg err");
            goto pares_failed;
        }

        tmp_flg &= ~(in_flg);

        switch (in_flg) {
        case AUDIO_HAL_PLAYBACK:
        case AUDIO_HAL_TONE:
            if (!((AUDIO_HAL_PLAYBACK | AUDIO_HAL_TONE) & tmp_flg)) {
                *out_flg_p = AUDIO_HAL_PLAYBACK;
            } else {
                *out_flg_p = AUDIO_HAL_NONE;    // no need to close
            }
            break;
        case AUDIO_HAL_CAPTURE:
            *out_flg_p = AUDIO_HAL_CAPTURE;
            break;
        case AUDIO_HAL_NONE:
        default:
            AUDHAL_ERROR("unknown in_flg 0x%x", in_flg);
            goto pares_failed;
            break;
        }
    }

    AUDHAL_TRACE("in_flg 0x%x, now_flg 0x%x, out_flg 0x%x, oc %d",
        in_flg, now_flg,*out_flg_p, oc);
    return AUDIO_HAL_SUCCESS;

pares_failed:
    AUDHAL_ERROR("unsupport operation  for in_flg 0x%x, now_flg 0x%x, oc %d",
        in_flg, now_flg, oc);
    //*out_flg_p = AUDIO_HAL_NONE;
    return AUDIO_HAL_ERROR;
}

LOCAL int32 _AUDIO_HAL_Get_Channel(uint32 *channel)
{
    int32 ret = AUDIO_HAL_SUCCESS;

    if (0 == s_audio_hal.ch)
    {
        AUDIO_CTL_ARG_U ioctl_arg = {0};
        AUDIO_HAL_CALL2(s_audio_hal.dai_ops->dai_ioctl,
                        AUDIO_DAI_CMD_CHANNEL_GET,
                        (uint32)&ioctl_arg,
                        ret);
        if (AUDIO_HAL_SUCCESS == ret)
        {
            s_audio_hal.ch = ioctl_arg.channel;
        }
    }
    if (channel)
    {
        if (s_audio_hal.ch != (*channel))
        {
            *channel = s_audio_hal.ch;
        }
    }
    return ret;
}

/**
 * this function is the handler of da buffer.
 */
LOCAL int32 _AUDIO_HAL_Playback_CB(uint32 param)
{
    int32 ret = AUDIO_HAL_SUCCESS;

    if (s_audio_hal.cb[AUDIO_PLAYBACK])
    {
        ret = s_audio_hal.cb[AUDIO_PLAYBACK]((param));
    }

    return ret;
}

/**
 * this function is the handler of ad buffer.
 */
LOCAL int32 _AUDIO_HAL_Capture_CB(uint32 param)
{
    int32 ret = AUDIO_HAL_SUCCESS;

    if (s_audio_hal.cb[AUDIO_CAPTRUE])
    {
        ret = s_audio_hal.cb[AUDIO_CAPTRUE]((param));
    }

    return ret;
}

/**---------------------------------------------------------------------------*
 **                         Global Function                                    *
 **---------------------------------------------------------------------------*/

/**
 * Audio Driver Init. include Codec Init, DAI Init etc.
 */
PUBLIC int32 AUDIO_HAL_Init(uint32 param)
{
    int32 ret = AUDIO_HAL_SUCCESS;

    AUDHAL_TRACE("entry");

    SCI_MEMSET(&s_audio_hal, 0, sizeof(s_audio_hal));

    s_audio_hal.mutex = SCI_CreateMutex(AUDIO_HAL_MUTEX_NAME, SCI_INHERIT);
    AUDHAL_PASSERT(s_audio_hal.mutex, ("audio hal mutex"));   /*assert verified*/
/*
    s_audio_hal.sleep_mutex = SCI_CreateMutex("AUDIO_HAL_SLEEP_MUTEX", SCI_INHERIT);
    AUDHAL_PASSERT(s_audio_hal.sleep_mutex, "audio hal sleep mutex");//assert verified
*/
    s_audio_hal.dai_ops = AUDIO_DAI_Get_OPS(0);
    if (0 != (s_audio_hal.dai_ops)) {
        AUDIO_HAL_CALL1(s_audio_hal.dai_ops->dai_init, param, ret);
    }
    if (AUDIO_HAL_SUCCESS != ret) {
        AUDHAL_ERROR("Init DAI Failed!");
    }

    s_audio_hal.codec_ops = AUDIO_EXT_CODEC_Get_OPS(0);
    if (0 != (s_audio_hal.codec_ops)) {
        AUDIO_HAL_CALL1(s_audio_hal.codec_ops->codec_init, param, ret);
    }
    if (AUDIO_HAL_SUCCESS != ret) {
        AUDHAL_ERROR("Init CODEC Failed!");
    }

    return ret;
}

/**
 *.Audio Driver Open include Codec Power On, DAI Enable.
 * @parameter1 fun_flg : from AUDIO_FUN_E.
 * @parameter2 fs : frame sample rate.
 */
PUBLIC int32 AUDIO_HAL_Open(uint32 fun_flg, uint32 fs)
{
    int32 ret = AUDIO_HAL_SUCCESS;
    uint32 real_fun_flg;

    AUDHAL_DEBUG("[AUDTICK] entry %d", SCI_GetTickCount());
    AUDHAL_TRACE("open fun_flg=0x%x,fs=%d", fun_flg, fs);

    AUDIO_HAL_MUTEX_ENTRY(s_audio_hal.mutex);

    if ((fs != s_audio_hal.fs) && (0 != s_audio_hal.fs)) {
        AUDIO_HAL_MUTEX_LEAVE(s_audio_hal.mutex);
        AUDHAL_ERROR("cann't open defferent fs %d (%d)", fs, s_audio_hal.fs);
        return AUDIO_HAL_ERROR;
    }

    ret = _AUDIO_HAL_Parse_Fun_Flg(fun_flg,
                                   s_audio_hal.fun_flg,
                                   &real_fun_flg,
                                   AUDIO_OPEN);
    if (AUDIO_HAL_SUCCESS != ret) {
        AUDHAL_ERROR("cann't open 0x%x (0x%x)", fun_flg, s_audio_hal.fun_flg);
        real_fun_flg = AUDIO_HAL_NONE;
    }

    AUDHAL_TRACE("real open 0x%x", real_fun_flg);

    // update fun_flg
    s_audio_hal.fun_flg |= fun_flg;
    ANA_DecisionCodecTpFunc(ANA_DEV_ID_AUDIO, TRUE);
    if (AUDIO_HAL_NONE != real_fun_flg) {
        if (0 == s_audio_hal.dai_ops) {
            s_audio_hal.dai_ops = AUDIO_DAI_Get_OPS(0);
            AUDHAL_PASSERT(s_audio_hal.dai_ops, ("audio dai_ops")); /*assert verified*/
            AUDIO_HAL_CALL1(s_audio_hal.dai_ops->dai_init, 0, ret);
        }

        AUDIO_HAL_CALL2(s_audio_hal.dai_ops->dai_open, real_fun_flg, fs, ret);
        if (AUDIO_HAL_SUCCESS != ret) {
            AUDHAL_ERROR("open dai failed! %d", ret);
            goto audiohal_open_failed;
        }

        if (0 == s_audio_hal.codec_ops) {
            s_audio_hal.codec_ops = AUDIO_EXT_CODEC_Get_OPS(0);
            AUDHAL_PASSERT(s_audio_hal.codec_ops, ("audio codec_ops")); /*assert verified*/
            AUDIO_HAL_CALL1(s_audio_hal.codec_ops->codec_init, 0, ret);
        }

        AUDIO_HAL_CALL2(s_audio_hal.codec_ops->codec_open, real_fun_flg, fs, ret);
        if (AUDIO_HAL_SUCCESS != ret) {
            AUDHAL_ERROR("open codec failed! %d", ret);
            goto audiohal_open_failed;
        }

        //update hal cfg
        s_audio_hal.fs = fs;
        _AUDIO_HAL_Get_Channel(&(s_audio_hal.ch)); //s_audio_hal.ch
    }
    SCI_AUD_EnableDeepSleep(DISABLE_DEEP_SLEEP);
    AUDIO_HAL_MUTEX_LEAVE(s_audio_hal.mutex);
    AUDHAL_DEBUG("[AUDTICK] exit %d", SCI_GetTickCount());
    AUDHAL_TRACE("open: fun_flg=0x%x,fs=%d,ch=%d",
                 s_audio_hal.fun_flg, s_audio_hal.fs, s_audio_hal.ch);
    return ret;

audiohal_open_failed:
    AUDIO_HAL_MUTEX_LEAVE(s_audio_hal.mutex);
    AUDHAL_ERROR("open failed!");

    // close
    if (AUDIO_HAL_NONE != real_fun_flg) {
        AUDIO_HAL_Close(real_fun_flg);
    }
    return AUDIO_HAL_ERROR;
}

/**
 *.Audio Driver Close.
 * @parameter fun_flg : from AUDIO_FUN_E.
 */
PUBLIC int32 AUDIO_HAL_Close(uint32 fun_flg)
{
    int32 ret = AUDIO_HAL_SUCCESS;
    int32 t_ret = AUDIO_HAL_SUCCESS;
    uint32 real_fun_flg;

    AUDHAL_DEBUG("[AUDTICK] entry %d", SCI_GetTickCount());
    AUDHAL_TRACE("close fun_flg=0x%x", fun_flg);

    AUDIO_HAL_MUTEX_ENTRY(s_audio_hal.mutex);

    ret = _AUDIO_HAL_Parse_Fun_Flg(fun_flg,
                                   s_audio_hal.fun_flg,
                                   &real_fun_flg,
                                   AUDIO_CLOSE);
    if (AUDIO_HAL_SUCCESS != ret)
    {
        AUDHAL_ERROR("not opened 0x%x (when in 0x%x)", fun_flg, s_audio_hal.fun_flg);
        AUDIO_HAL_MUTEX_LEAVE(s_audio_hal.mutex);
        return AUDIO_HAL_SUCCESS;
//      goto audiohal_close_failed;
    }

    AUDHAL_TRACE("real close 0x%x", real_fun_flg);

    SCI_AUD_EnableDeepSleep(ENABLE_DEEP_SLEEP);
    // update fun_flg
    s_audio_hal.fun_flg &= ~fun_flg;

    if (AUDIO_HAL_NONE != real_fun_flg) {
        AUDIO_HAL_CALL1(s_audio_hal.codec_ops->codec_close, real_fun_flg, t_ret);
        if (AUDIO_HAL_SUCCESS != t_ret) {
            AUDHAL_ERROR("close codec failed! %d", ret);
//            goto audiohal_close_failed;
        }

        AUDIO_HAL_CALL1(s_audio_hal.dai_ops->dai_close, real_fun_flg, t_ret);
        if (AUDIO_HAL_SUCCESS != t_ret) {
            AUDHAL_ERROR("close dai failed! %d", ret);
//            goto audiohal_close_failed;
        }
    }

    // clear when all stream closed
    if(AUDIO_HAL_NONE == s_audio_hal.fun_flg){
        s_audio_hal.fs = 0;
        s_audio_hal.ch = 0;
	 ANA_DecisionCodecTpFunc(ANA_DEV_ID_AUDIO, FALSE);
    }

    AUDIO_HAL_MUTEX_LEAVE(s_audio_hal.mutex);
    AUDHAL_DEBUG("[AUDTICK] exit %d", SCI_GetTickCount());
    return AUDIO_HAL_SUCCESS;

//audiohal_close_failed:
//    AUDIO_HAL_MUTEX_LEAVE(s_audio_hal.mutex);
//    AUDHAL_ERROR("close failed");
//    return AUDIO_HAL_ERROR;
}

/**
 *.Audio Driver Ioctl, include Codec Ioctl, DAI Ioctl.
 * @parameter1 cmd :from CODEC_CMD_E and DAI_CMD_E.
 * @parameter2 arg :some cmd maybe use 0 if no param,
 *if the cmd have param,please give a non-NULL point!
 */
PUBLIC int32 AUDIO_HAL_Ioctl(uint32 cmd, uint32 arg)
{
    int32 ret = AUDIO_HAL_SUCCESS;
    AUDHAL_PASSERT(cmd, ("AUDIO_HAL_Ioctl cmd null")); /*assert verified*/
    AUDHAL_PASSERT(arg, ("AUDIO_HAL_Ioctl arg null")); /*assert verified*/
    AUDIO_HAL_MUTEX_ENTRY(s_audio_hal.mutex);

    AUDHAL_TRACE("ioctl cmd=%d,arg=0x%x", cmd, arg);

    if ((cmd > AUDIO_HAL_CMD_BEGIN) && (cmd < AUDIO_HAL_CMD_END)) {
        switch (cmd) {
            case AUDIO_HAL_CMD_FUN_FLG_GET:
                ((AUDIO_CTL_ARG_U *)(arg))->dwVal = s_audio_hal.fun_flg;
                AUDHAL_TRACE("ioctl hal current fun_flg=0x%x", s_audio_hal.fun_flg);
                break;

            default:
                AUDHAL_TRACE("ioctl hal invalid cmd=%d", cmd);
                ret = AUDIO_HAL_ERROR;
                break;
        }

    } else if ((cmd > AUDIO_CODEC_CMD_BEGIN) && (cmd < AUDIO_CODEC_CMD_END)) {
        AUDIO_HAL_CALL2(s_audio_hal.codec_ops->codec_ioctl, cmd, arg, ret);

    } else if ((cmd > AUDIO_DAI_CMD_BEGIN) && (cmd < AUDIO_DAI_CMD_END)) {
        AUDIO_HAL_CALL2(s_audio_hal.dai_ops->dai_ioctl, cmd, arg, ret);
        if ((AUDIO_DAI_CMD_CHANNEL_GET == cmd) && (AUDIO_HAL_SUCCESS == ret)) {
            s_audio_hal.ch = ((AUDIO_CTL_ARG_U *)(arg))->channel;
        }

    } else {
        AUDHAL_TRACE("ioctl cmd=%d,arg_ret=0x%x", cmd, ((AUDIO_CTL_ARG_U *)(arg))->dwVal);
        ret = AUDIO_HAL_ERROR;
    }

    AUDIO_HAL_MUTEX_LEAVE(s_audio_hal.mutex);
    return ret;
}

/**
 *.Audio Driver Set Playback Buffer Info.
 * @parameter1 pLHead :if AUDIO_DATA_FMT_LL_RR , the Left channel data start point.
 *                                 if AUDIO_DATA_FMT_LR_LR , the data start point.
 * @parameter2 pRHead :if AUDIO_DATA_FMT_LL_RR , the Right channel data start point.
 *                                 if AUDIO_DATA_FMT_LR_LR , will ignore.
 * @parameter3 cnt :the max size of pLHead.
* well, pRHead MUST the same size with pLHead if exist.
 */
 // update when trigger
PUBLIC int32 AUDIO_HAL_DAC_Set_Buf_Info(void *pLHead, void *pRHead, uint32 cnt)
{
    int32 ret = AUDIO_HAL_SUCCESS;

    AUDHAL_TRACE("dac_set_buf_info pLHead=0x%x,pRHead=0x%x,cnt=%d",
                 pLHead, pRHead, cnt);
    AUDHAL_PASSERT(pLHead, ("DAC pLHead")); /*assert verified*/
    AUDHAL_PASSERT(cnt, ("DAC cnt"));       /*assert verified*/
    // pRHead not used in w217
    AUDIO_HAL_CALL3(
        s_audio_hal.dai_ops->dai_dac_set_buf_info,
        pLHead, pRHead, cnt, ret);
    return ret;
}

PUBLIC int32 AUDIO_HAL_DAC_Set_Write_Point(uint32 pWrite)
{
    int32 ret = AUDIO_HAL_SUCCESS;

    AUDIO_HAL_CALL1(s_audio_hal.dai_ops->dai_dac_set_write_point, pWrite, ret);
    return ret;
}
PUBLIC int32 AUDIO_HAL_DAC_Get_Read_Point(uint32 *pRead)
{
    int32 ret = AUDIO_HAL_SUCCESS;

    AUDIO_HAL_CALL1(s_audio_hal.dai_ops->dai_dac_get_read_point, pRead, ret);
    return ret;
}

/**
 *.Audio Driver Will Trigger Playback immediately.
* do nothing when already trigger.
 */
PUBLIC int32 AUDIO_HAL_DAC_Trigger(void)
{
    int32 ret = AUDIO_HAL_SUCCESS;

    AUDHAL_DEBUG("dac_trigger");
    AUDIO_HAL_CALL(s_audio_hal.dai_ops->dai_dac_trigger, ret);
    return ret;
}

/**
 *.Audio Driver Capture Buffer Info.
 * @parameter1 pLHead :if AUDIO_DATA_FMT_LL_RR , the Left channel data start point.
 *                                 if AUDIO_DATA_FMT_LR_LR , the data start point.
 * @parameter2 pRHead :if AUDIO_DATA_FMT_LL_RR , the Right channel data start point.
 *                                 if AUDIO_DATA_FMT_LR_LR , will ignore.
 * @parameter3 cnt :the max size of pLHead.
* well, pRHead MUST the same size with pLHead if exist.
 */
 // update when trigger
PUBLIC int32 AUDIO_HAL_ADC_Set_Buf_Info(void *pLHead, void *pRHead, uint32 cnt)
{
    int32 ret = AUDIO_HAL_SUCCESS;

    AUDHAL_TRACE("adc_set_buf_info pLHead=0x%x,pRHead=0x%x,cnt=%d",
                 pLHead, pRHead, cnt);
    AUDHAL_PASSERT(pLHead, ("ADC pLHead")); /*assert verified*/
    AUDHAL_PASSERT(cnt, ("ADC cnt"));       /*assert verified*/
    // pRHead not used in w217
    AUDIO_HAL_CALL3(
        s_audio_hal.dai_ops->dai_adc_set_buf_info,
        pLHead, pRHead, cnt, ret);
    return ret;
}

PUBLIC int32 AUDIO_HAL_ADC_Set_Read_Point(uint32 pRead)
{
    int32 ret = AUDIO_HAL_SUCCESS;

    AUDIO_HAL_CALL1(s_audio_hal.dai_ops->dai_adc_set_read_point, pRead, ret);
    return ret;
}

PUBLIC int32 AUDIO_HAL_ADC_Get_Write_Point(uint32 *pWrite)
{
    int32 ret = AUDIO_HAL_SUCCESS;

    AUDIO_HAL_CALL1(s_audio_hal.dai_ops->dai_adc_get_write_point, pWrite, ret);
    return ret;
}

/**
 *.Audio Driver Will Trigger Captrue immediately.
* do nothing when already trigger.
 */
PUBLIC int32 AUDIO_HAL_ADC_Trigger(void)
{
    int32 ret = AUDIO_HAL_SUCCESS;

    AUDHAL_DEBUG("adc_trigger");
    AUDIO_HAL_CALL(s_audio_hal.dai_ops->dai_adc_trigger, ret);
    return ret;
}

/**
 *.Audio Driver Register Playback Callback, Use to Notify User the Driver fetch data gone,
 * User can do something like start decode audio data..
 * @parameter pFun :this function.
 * This Callback DO NOT call deep tree, should be send message or set flag etc..
 */
PUBLIC int32 AUDIO_HAL_Register_Playback(AUDIO_HAL_CALLBACK_FUN pFun)
{
    int32 ret = AUDIO_HAL_SUCCESS;

    if (pFun)
    {
        AUDHAL_DEBUG("register_playback 0x%x", pFun);
        s_audio_hal.cb[AUDIO_PLAYBACK] = pFun;

        AUDIO_HAL_CALL1(
            s_audio_hal.dai_ops->dai_register_playback,
            _AUDIO_HAL_Playback_CB, ret);
    }

    return ret;
}

/**
 *.Audio Driver Register Capture Callback, Use to Notify User  for fetch data please,
 * User should be move audio data ASAP..
 * @parameter pFun :this function.
 * This Callback DO NOT call deep tree, should be send message or set flag etc..
 */
PUBLIC int32 AUDIO_HAL_Register_Capture(AUDIO_HAL_CALLBACK_FUN pFun)
{
    int32 ret = AUDIO_HAL_SUCCESS;

    if (pFun)
    {
        AUDHAL_DEBUG("captire 0x%x", pFun);
        s_audio_hal.cb[AUDIO_CAPTRUE] = pFun;

        AUDIO_HAL_CALL1(
            s_audio_hal.dai_ops->dai_register_capture,
            _AUDIO_HAL_Capture_CB, ret);
    }

    return ret;
}

/**
 *.Audio Driver check support fs.
 * @parameter1 fs : frame sample rate.
 */
PUBLIC int32 AUDIO_HAL_FS_CHECK(uint32 fs)
{
    int32 ret = AUDIO_HAL_SUCCESS;
    uint32 cmd;
    AUDIO_CTL_ARG_U arg;

    AUDIO_HAL_MUTEX_ENTRY(s_audio_hal.mutex);

    // init dai if undef dai_ops
    if (0 == s_audio_hal.dai_ops) {
        s_audio_hal.dai_ops = AUDIO_DAI_Get_OPS(0);
        AUDHAL_PASSERT(s_audio_hal.dai_ops, ("audio dai_ops")); /*assert verified*/
        AUDIO_HAL_CALL1(s_audio_hal.dai_ops->dai_init, 0, ret);
    }

    // check dai fs
    cmd = AUDIO_DAI_CMD_FS_CHECK;
    arg.fs_check = fs;
    ret = AUDIO_HAL_Ioctl(cmd, (uint32)&arg);
    if ((fs != arg.fs_check) || (AUDIO_HAL_SUCCESS != ret)) {
        AUDHAL_ERROR("DAI not support fs %d", fs);
        AUDIO_HAL_MUTEX_LEAVE(s_audio_hal.mutex);
        return AUDIO_HAL_ERROR;
    }

    // init codec if undef dai_ops
    if (0 == s_audio_hal.codec_ops) {
        s_audio_hal.codec_ops = AUDIO_EXT_CODEC_Get_OPS(0);
        AUDHAL_PASSERT(s_audio_hal.codec_ops, ("audio codec_ops")); /*assert verified*/
        AUDIO_HAL_CALL1(s_audio_hal.codec_ops->codec_init, 0, ret);
    }

    // check codec fs
    cmd = AUDIO_CODEC_CMD_FS_CHECK;
    arg.fs_check = fs;
    ret = AUDIO_HAL_Ioctl(cmd, (uint32)&arg);
    if ((fs != arg.fs_check) || (AUDIO_HAL_SUCCESS != ret)) {
        AUDHAL_ERROR("CODEC not support fs %d", fs);
        AUDIO_HAL_MUTEX_LEAVE(s_audio_hal.mutex);
        return AUDIO_HAL_ERROR;
    }

    AUDHAL_TRACE("fs=%d",fs);
    AUDIO_HAL_MUTEX_LEAVE(s_audio_hal.mutex);
    return ret;
}

/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif
/**---------------------------------------------------------------------------*/

// End
