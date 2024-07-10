/*
* <audio_codec_es83xx.c> - <This file define the basic hw interfaces of audio codec of es8374/es8311>
*
* Copyright (c) 2019 Unisoc Communications Inc.
* History
*      <2022/07/21> <jen.li> <refactoring>
*/

/*----------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **------------------------------------------------------------------------- */
#include "os_api.h"
#include "audio_codec.h"
#include "audio_drvapi.h"

#include "es83xx_i2c.h"
#include "extpa_drv.h"

#include "i2c_drvapi.h"

/**---------------------------------------------------------------------------*
 **                             Compiler Flag                                 *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif

/**---------------------------------------------------------------------------*
**                               Micro Define                                **
**---------------------------------------------------------------------------*/
#define CODEC_I2S_SLAVER 1
#define EXTPA_OPEN_DELAY    15      //if pop, please modify there

/**---------------------------------------------------------------------------*
 **                         Data Structure                                    *
 **---------------------------------------------------------------------------*/
typedef struct
{
    AUDIO_CODEC_PATH_T dl;
    AUDIO_CODEC_PATH_T ul;
    uint32 pa_state;
    uint32 fun_flg;
    uint32 fs;
} AUDIO_CODEC_CFG_T;

/**---------------------------------------------------------------------------*
 **                         Global variables                                  *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         LOCAL Data                                        *
 **---------------------------------------------------------------------------*/
LOCAL AUDIO_CODEC_CFG_T s_codec_cfg = {
    /*AUDIO_CODEC_PATH_T dl;            */ {0},
    /*AUDIO_CODEC_PATH_T ul;            */ {0},
    /*uint32 pa_state;                  */ FALSE,
    /*uint32 fun_flg;                   */ 0,
    /*uint32 fs;                        */ 0
};
LOCAL SCI_TIMER_PTR  s_extpa_timer = PNULL;
LOCAL uint32 s_pa_gain = 0;

/**----------------------------------------------------------------------------*
**                         Local Function Prototype                           **
**----------------------------------------------------------------------------*/
// update dl/ul cfg and update pa state
LOCAL void __codec_cfg_update(AUDIO_CODEC_PATH_T *new_path, uint32 direct)
{
    if (NULL != new_path) {
        if(AUDIO_PLAYBACK == direct){
            SCI_MEMCPY(&(s_codec_cfg.dl), new_path, sizeof(AUDIO_CODEC_PATH_T));
        } else {
            SCI_MEMCPY(&(s_codec_cfg.ul), new_path, sizeof(AUDIO_CODEC_PATH_T));
        }
    }

    if (AUDIO_PLAYBACK == direct) {
        AUDDRV_DEBUG("codec output={0x%x, 0x%x, 0x%x} mute={%d} pa={%d}",
                     s_codec_cfg.dl.device,
                     s_codec_cfg.dl.gain1,
                     s_codec_cfg.dl.gain2,
                     s_codec_cfg.dl.mute,
                     s_codec_cfg.pa_state);
    } else {
        AUDDRV_DEBUG("codec inpit={0x%x, 0x%x, 0x%x} mute={%d}",
                     s_codec_cfg.ul.device,
                     s_codec_cfg.ul.gain1,
                     s_codec_cfg.ul.gain2,
                     s_codec_cfg.ul.mute);
    }
}

LOCAL void Extpa_Timer_Callback(uint32 para)
{
    if(s_extpa_timer) {
       SCI_DeactiveTimer(s_extpa_timer);
    }
    SCI_TRACE_LOW("[extpa][codec_ext_pa_ctl] enable (%d)", SCI_GetTickCount());
    __extpa_enable(TRUE);
    __extpa_gain_set(s_pa_gain);
}

LOCAL void __extpa_enable_with_timer(BOOLEAN b_enable)
{
    if (b_enable) {
        if(PNULL == s_extpa_timer) {
            s_extpa_timer = SCI_CreateTimer("Extpa_Timer",
            Extpa_Timer_Callback, NULL, EXTPA_OPEN_DELAY, SCI_AUTO_ACTIVATE);
        } else {
            SCI_ChangeTimer(s_extpa_timer, Extpa_Timer_Callback, EXTPA_OPEN_DELAY);
            SCI_ActiveTimer(s_extpa_timer);
        }
    } else {
        if(s_extpa_timer) {
            SCI_DeactiveTimer(s_extpa_timer);
        }
        __extpa_enable(FALSE);
    }
}

LOCAL void __codec_cfg_reset()
{
    //__extpa_enable(FALSE);
    __extpa_enable_with_timer(FALSE);
    s_codec_cfg.pa_state = FALSE;
    SCI_MEMSET(&s_codec_cfg, 0, sizeof(AUDIO_CODEC_CFG_T));
}

LOCAL int32 _CODEC_Init(uint32 param) //param: i2c handle
{
    static uint32 inited = 0;
    if (!inited) {
        // reset s_codec_cfg
        __codec_cfg_reset();
        inited = 1;
    }
    AUDDRV_TRACE("ok");
    return AUDIO_HAL_SUCCESS;
}

LOCAL int32 _CODEC_Open(uint32 fun_flg, uint32 fs)
{
    int32 mode;
    int32 ret = AUDIO_HAL_SUCCESS;

     // fun_flg check
     if ((AUDIO_FUN_PLAYBACK != fun_flg) &&
         (AUDIO_FUN_CAPTRUE != fun_flg)) {
        AUDDRV_ERROR("codec open {fun_flg=0x%x (0x%x)} err",
            fun_flg, s_codec_cfg.fun_flg);
        return AUDIO_HAL_ERROR;
    }

    // s_codec_cfg.fun_flg check
    if (AUDIO_FUN_NONE != s_codec_cfg.fun_flg) {
        if (fs != s_codec_cfg.fs) {
            AUDDRV_ERROR("codec open {fs=%d (%d)} err", fs, s_codec_cfg.fs);
            return AUDIO_HAL_ERROR;
        } else if (fun_flg & s_codec_cfg.fun_flg) {
            AUDDRV_TRACE("codec open {fun_flg=0x%x (0x%x) fs=%d} already",
                fun_flg, s_codec_cfg.fun_flg, fs);
            return AUDIO_HAL_SUCCESS;
        }
    }

    // codec power up
    if (AUDIO_FUN_NONE == s_codec_cfg.fun_flg) {
        GPIO_EnableCodec(1);
        AUDDRV_TRACE("codec power up");
    }

    AUDDRV_DEBUG("codec open {fun_flg=0x%x (0x%x) fs=%d} entry",
        fun_flg, s_codec_cfg.fun_flg, fs);

    if (AUDIO_FUN_NONE == s_codec_cfg.fun_flg) {
        // mode
        if (AUDIO_FUN_CAPTRUE == fun_flg) {
            mode = 0;
        } else if (AUDIO_FUN_PLAYBACK == fun_flg) {
            mode = 1;
        } else {
            AUDDRV_ERROR("unsupport fun_flg");
            return AUDIO_HAL_ERROR;
        }

        // init codec i2c
        ret = __es83xx_i2c_init();
        if (AUDIO_HAL_SUCCESS != ret) {
            AUDDRV_ERROR("cann't open for i2c request failed");
            return AUDIO_HAL_ERROR;
        }

        // init codec
        s_codec_cfg.fs = __es83xx_cfg_init(fs, mode, CODEC_I2S_SLAVER);
        s_codec_cfg.fun_flg = fun_flg;

    } else {
        // mode
        if ((AUDIO_FUN_CAPTRUE == fun_flg) &&
            (AUDIO_FUN_PLAYBACK == s_codec_cfg.fun_flg)) {
            mode = 0;
        } else if ((AUDIO_FUN_PLAYBACK == fun_flg) &&
            (AUDIO_FUN_CAPTRUE == s_codec_cfg.fun_flg)) {
            mode = 1;
        } else {
            AUDDRV_ERROR("unsupport fun_flg");
            return AUDIO_HAL_ERROR;
        }

        // switch codec
        __es83xx_cfg_swicth(mode, CODEC_I2S_SLAVER);
        __es83xx_cfg_fs(s_codec_cfg.fs);
        s_codec_cfg.fun_flg |= fun_flg;
    }

    AUDDRV_TRACE("codec open {fun_flg=0x%x (0x%x) fs=%d} exit",
        fun_flg, s_codec_cfg.fun_flg, fs);

    return AUDIO_HAL_SUCCESS;
}

LOCAL int32 _CODEC_Close(uint32 fun_flg)
{
    int32 mode;

    // fun_flg check
     if ((AUDIO_FUN_PLAYBACK != fun_flg) &&
         (AUDIO_FUN_CAPTRUE != fun_flg)) {
        AUDDRV_ERROR("codec close {fun_flg=0x%x (0x%x)} err",
            fun_flg, s_codec_cfg.fun_flg);
        return AUDIO_HAL_ERROR;
    }

    // s_codec_cfg.fun_flg check
    if (!(fun_flg & s_codec_cfg.fun_flg)) {
        AUDDRV_TRACE("codec close {fun_flg=0x%x (0x%x)} already",
            fun_flg, s_codec_cfg.fun_flg);
        return AUDIO_HAL_SUCCESS;
    }

    AUDDRV_DEBUG("codec close {fun_flg=0x%x (0x%x)} entry",
        fun_flg, s_codec_cfg.fun_flg);

    s_codec_cfg.fun_flg &= ~fun_flg;

    if (AUDIO_FUN_NONE == s_codec_cfg.fun_flg) {
        // close pa
        //__extpa_enable(FALSE);
        __extpa_enable_with_timer(FALSE);
        s_codec_cfg.pa_state = FALSE;

        // close codec
        __es83xx_cfg_close();

        // codec release i2c handle
        __es83xx_i2c_deinit();

        // cfg reset
        __codec_cfg_reset();

    } else {
        if (AUDIO_FUN_PLAYBACK == fun_flg) {
            // close pa
            //__extpa_enable(FALSE);
            __extpa_enable_with_timer(FALSE);
            s_codec_cfg.pa_state = FALSE;
        }

        // micbias power up
        if (AUDIO_FUN_CAPTRUE == fun_flg) {
            GPIO_EnableMicBias(0);
            AUDDRV_TRACE("micbias power down");
        }

        // mode
        if (AUDIO_FUN_PLAYBACK == fun_flg) {
            mode = 2;
        } else if (AUDIO_FUN_CAPTRUE == fun_flg) {
            mode = 3;
        } else {
            AUDDRV_ERROR("unsupport fun_flg");
            return AUDIO_HAL_ERROR;
        }

        // switch codec
        __es83xx_cfg_swicth(mode, CODEC_I2S_SLAVER);
        __es83xx_cfg_fs(s_codec_cfg.fs);

    }

    AUDDRV_TRACE("codec close {fun_flg=0x%x (0x%x)} exit",
        fun_flg, s_codec_cfg.fun_flg);

    // codec power down
    if (AUDIO_FUN_NONE == s_codec_cfg.fun_flg) {
        GPIO_EnableCodec(0);
        AUDDRV_TRACE("codec power down");
    }

    return AUDIO_HAL_SUCCESS;
}

/*
check support path
set input path
set output path
set mute
*/
LOCAL int32 _CODEC_Ioctl(uint32 cmd, uint32 arg)
{
    int32 ret = AUDIO_HAL_SUCCESS;
    AUDIO_CODEC_PATH_T *path = NULL;
    uint32 is_hp, tmp;
    AUDIO_CTL_ARG_U *ioctl_arg = (AUDIO_CTL_ARG_U *)arg;

    AUDDRV_TRACE("ioctl codec {cmd %d arg_val 0x%x, %d}", cmd, ioctl_arg->dwVal, SCI_GetTickCount());

    switch (cmd)
    {
    case AUDIO_CODEC_CMD_FS_CHECK:
        tmp = __es83xx_fs_check(ioctl_arg->fs_check);
        AUDDRV_TRACE("ioctl dai fs check %d result %d", ioctl_arg->fs_check, tmp);
        ioctl_arg->fs_check  = tmp;
        break;

    case AUDIO_CODEC_CMD_SUPPORT_PATH_GET:
        ioctl_arg->support_path = __es83xx_support_device();
        AUDDRV_TRACE("support codec path {0x%x}", ioctl_arg->dwVal);
        break;

    case AUDIO_CODEC_CMD_OUTPUT_PATH_SET:
        if (NULL == ioctl_arg->addr_path) {
            AUDDRV_ERROR("codec addr_path NULL");
            return AUDIO_HAL_ERROR;
        } else {
            path = (AUDIO_CODEC_PATH_T *)(ioctl_arg->addr_path);
        }

        AUDDRV_TRACE("codec output path {0x%x 0x%x 0x%x %d}",
            path->device, path->gain1, path->gain2, path->mute);

        __es83xx_modify_device(&(path->device), AUDIO_PLAYBACK);
        // pa close the first
        if (((!(AUDIO_SPK & path->device)) ||
            (TRUE == path->mute)) &&
            (TRUE == s_codec_cfg.pa_state)) {
            //__extpa_enable(FALSE);
            __extpa_enable_with_timer(FALSE);
            s_codec_cfg.pa_state = FALSE;
        }
        // path and gain
        is_hp = (path->device == AUDIO_HP) ? 1 : 0;
        __es83xx_cfg_output(is_hp, path->gain1, path->gain2);
        //__extpa_gain_set(path->gain2);
        s_pa_gain = path->gain2;

        // mute
        __es83xx_cfg_output_mute(path->mute);

        // pa open the last
        if ((AUDIO_SPK & path->device) &&
            (FALSE == path->mute) &&
            (FALSE == s_codec_cfg.pa_state)) {
            //__extpa_enable(TRUE);
            __extpa_enable_with_timer(TRUE);
            s_codec_cfg.pa_state = TRUE;
        }
        // update dl cfg
        __codec_cfg_update(path, AUDIO_PLAYBACK);

        break;

    case AUDIO_CODEC_CMD_INPUT_PATH_SET:
        if (NULL == ioctl_arg->addr_path) {
            AUDDRV_ERROR("codec addr_path NULL");
            return AUDIO_HAL_ERROR;
        } else {
            path = (AUDIO_CODEC_PATH_T *)(ioctl_arg->addr_path);
        }

        AUDDRV_TRACE("codec input path {0x%x 0x%x 0x%x %d}",
            path->device, path->gain1, path->gain2, path->mute);

        __es83xx_modify_device(&(path->device), AUDIO_CAPTRUE);

        // micbias power down
        if ((AUDIO_MIC != path->device) || (1 == path->mute)) {
            GPIO_EnableMicBias(0);
            AUDDRV_TRACE("micbias power down");
        }

        is_hp = (path->device == AUDIO_HP_MIC) ? 1 : 0;
        __es83xx_cfg_input(is_hp, path->gain1, path->gain2);

        // mute
        __es83xx_cfg_input_mute(path->mute);

        // micbias power up
        if ((AUDIO_MIC == path->device) && (0 == path->mute)) {
            SCI_Sleep(1);
            GPIO_EnableMicBias(1);
            AUDDRV_TRACE("micbias power up");
        }

        // update ul cfg
        __codec_cfg_update(path, AUDIO_CAPTRUE);

        break;

    case AUDIO_CODEC_CMD_OUTPUT_MUTE_SET:
        s_codec_cfg.dl.mute = (ioctl_arg->mute > 0) ? TRUE : FALSE;

        // pa close the first
        if (((!(AUDIO_SPK & s_codec_cfg.dl.device)) ||
            (TRUE == s_codec_cfg.dl.mute)) &&
            (TRUE == s_codec_cfg.pa_state)) {
            //__extpa_enable(FALSE);
            __extpa_enable_with_timer(FALSE);
            s_codec_cfg.pa_state = FALSE;
        }

        __es83xx_cfg_output_mute(s_codec_cfg.dl.mute);

        // pa open the last
        if ((AUDIO_SPK & s_codec_cfg.dl.device) &&
            (FALSE == s_codec_cfg.dl.mute) &&
            (FALSE == s_codec_cfg.pa_state)) {
            //__extpa_enable(TRUE);
            __extpa_enable_with_timer(TRUE);
            s_codec_cfg.pa_state = TRUE;
        }

        // update dl cfg
        __codec_cfg_update(NULL, AUDIO_PLAYBACK);

        break;

    case AUDIO_CODEC_CMD_INPUT_MUTE_SET:
        s_codec_cfg.ul.mute = (ioctl_arg->mute > 0) ? 1 : 0;

        // micbias power down
        if ((AUDIO_MIC != s_codec_cfg.ul.device) || (1 == s_codec_cfg.ul.mute)) {
            //GPIO_EnableMicBias(0);
            AUDDRV_TRACE("micbias power down");
        }

        __es83xx_cfg_input_mute(s_codec_cfg.ul.mute);

        // micbias power up
        if ((AUDIO_MIC == s_codec_cfg.ul.device) && (0 == s_codec_cfg.ul.mute)) {
            //SCI_Sleep(1);
            //GPIO_EnableMicBias(1);
            AUDDRV_TRACE("micbias power up");
        }

        // update ul cfg
        __codec_cfg_update(NULL, AUDIO_CAPTRUE);

        break;

    default:
        ret = AUDIO_HAL_ERROR;
        AUDDRV_ERROR("ioctl codec unknow cmd %d", cmd);
        break;
    }

    return ret;
}

/**----------------------------------------------------------------------------*
**                           Function Prototype                               **
**----------------------------------------------------------------------------*/
LOCAL AUD_CODEC_OPS_T s_audio_codec_es83xx_ops =
    {
        _CODEC_Init,
        _CODEC_Open,
        _CODEC_Close,
        _CODEC_Ioctl,
};

PUBLIC AUD_CODEC_OPS_T *AUDIO_EXT_CODEC_Get_OPS(uint32 param)
{
    return (AUD_CODEC_OPS_T *)(&(s_audio_codec_es83xx_ops));
}

/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif
/**---------------------------------------------------------------------------*/

// End
