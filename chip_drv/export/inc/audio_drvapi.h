/******************************************************************************
 ** File Name:      audio_drvapi.h                                            *
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

#ifndef _AUDIO_DRVAPI_H_
#define _AUDIO_DRVAPI_H_
/*----------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **------------------------------------------------------------------------- */

#include "sci_types.h"

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
#define AUDIO_DRV_DEBUG
#ifdef AUDIO_DRV_DEBUG
#define AUDDRV_DEBUG(A, ...)    SCI_TRACE_LOW("[AUDDRV][%s] " A "\n", __FUNCTION__, ##__VA_ARGS__)
#define AUDDRV_PASSERT          SCI_PASSERT
#else
#define AUDDRV_DEBUG(...)
#define AUDDRV_PASSERT(...)
#endif

#define AUDDRV_TRACE(A, ...)    SCI_TRACE_LOW("[AUDDRV][%s] " A "\n", __FUNCTION__, ##__VA_ARGS__)
#define AUDDRV_ERROR(A, ...)    SCI_TRACE_LOW("[AUDDRV][%s][%d]{E} " A "\n", __FUNCTION__, __LINE__, ##__VA_ARGS__)

//enum CODEC_MODE_TYPE_E{
#define AUDIO_SPK       (0x0001)
#define AUDIO_EAR       (0x0002)
#define AUDIO_HP        (0x0004)
#define AUDIO_MIC       (0x0008)
#define AUDIO_LINEIN    (0x0010)
#define AUDIO_HP_MIC    (0x0020)
//};

/**
When play MP3, just use AUDIO_FUN_PLAYBACK .
When use for call, need AUDIO_FUN_VOICE
the AUDIO_FUN_VOICE special tell DAI/CODEC open both dl and ul.
Yet, AUDIO_FUN_PLAYBACK stand for DAC path except open HP/PA/EAR what open by IOCTL.
Yet, AUDIO_FUN_CAPTRUE stand for ADC path except select MIC/AI  what select by IOCTL.
only AUDIO_FUN_TONE can start when HAL is running in other mode (bypass data from other mode)
**/
//enum AUDIO_FUN_E{
#define AUDIO_FUN_NONE      (0x00000000)
#define AUDIO_FUN_PLAYBACK  (0x00000001)
#define AUDIO_FUN_CAPTRUE   (0x00000002)
//};
#define AUDIO_FUN_MAX_CNT 28

enum AUDIO_ERR_E
{
    AUDIO_HAL_SUCCESS = 0,
    AUDIO_HAL_ERROR = -1,
    AUDIO_ERR_ENUM_MAX = 0xFFFFFFFF //force this enum use uint32
};

typedef enum
{
    AUDIO_CH_MONO = 1,
    AUDIO_CH_STEREO = 2,
    AUDIO_CH_MAX
} AUDIO_CH_E;

enum AUDIO_DIRECTION_E
{
    AUDIO_CAPTRUE = 0,
    AUDIO_PLAYBACK,
};


/******************************************************************************/
// for TONE
/******************************************************************************/
// tone
#define TONE_CMD_START 0
#define TONE_CMD_STOP 1
#define TONE_CMD_PAUSE 2
#define TONE_CMD_RESUME 3

typedef enum
{
    AUDIO_AIF_TONE_GAIN_0_DB,
    AUDIO_AIF_TONE_GAIN_M3_DB,
    AUDIO_AIF_TONE_GAIN_M9_DB,
    AUDIO_AIF_TONE_GAIN_M15_DB,
    AUDIO_AIF_TONE_GAIN_MAX
}AUDIO_AIF_TONE_GAIN_E;

// =============================================================================
// AUDIO_AIF_TONE_TYPE_T
// -----------------------------------------------------------------------------
/// Tone types.
/// The DTMF Tones are used to inform the user that the number is being
/// composed. All the standard DTMF are available: 0 to 9, A to D,
/// pound and star. \n
/// The Comfort Tones are used to inform the user on the current state of
/// the call: Ringing, Busy, Unavailable... All frequencies needed to do
/// the standard Comfort Tones are available: 425 Hz, 950 Hz, 1400 Hz and
/// 1800 Hz. \n
// =============================================================================
typedef enum
{
    /// Tone when the '0' key
    AUDIO_AIF_DTMF_0,
    /// Tone when the '1' key
    AUDIO_AIF_DTMF_1,
    /// Tone when the '2' key
    AUDIO_AIF_DTMF_2,
    /// Tone when the '3' key
    AUDIO_AIF_DTMF_3,
    /// Tone when the '4' key
    AUDIO_AIF_DTMF_4,
    /// Tone when the '5' key
    AUDIO_AIF_DTMF_5,
    /// Tone when the '6' key
    AUDIO_AIF_DTMF_6,
    /// Tone when the '7' key
    AUDIO_AIF_DTMF_7,
    /// Tone when the '8' key
    AUDIO_AIF_DTMF_8,
    /// Tone when the '9' key
    AUDIO_AIF_DTMF_9,
    AUDIO_AIF_DTMF_A,
    AUDIO_AIF_DTMF_B,
    AUDIO_AIF_DTMF_C,
    AUDIO_AIF_DTMF_D,
    /// Tone when the * key
    AUDIO_AIF_DTMF_S,
    /// Tone when the # key
    AUDIO_AIF_DTMF_P,
    /// Comfort tone at 425 Hz
    AUDIO_AIF_COMFORT_425,
    /// Comfort tone at 950 Hz
    AUDIO_AIF_COMFORT_950,
    /// Comfort tone at 1400 Hz
    AUDIO_AIF_COMFORT_1400,
    /// Confort tone at 1800 Hz
    AUDIO_AIF_COMFORT_1800,
    ///
    AUDIO_AIF_TONE_SILENCE
} AUDIO_AIF_TONE_TYPE_E;// releated to AUD_DTMF_TONE_ID_E(audio_config.h)


/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/
typedef union {
    uint32 dwVal;
    struct
    {
        uint32 gain : 27;
        uint32 type : 2; // 4 level
        uint32 cmd : 2;  // 4 cmd
        uint32 reserve :1;
    } mBits;
} AUDIO_CTL_TONE_U;

typedef struct {
    uint32 device;
    uint16 gain1; // dac/adc
    uint16 gain2; // mixgain/pga
    uint32 mute;
} AUDIO_CODEC_PATH_T;

// union
typedef union {
    uint32 dwVal;

    uint32 fs_check;

    // dai
    AUDIO_CTL_TONE_U tone;
    uint32 channel;

    //codec
    uint32 support_path;
    uint32 addr_path; // the addr of AUDIO_CODEC_PATH_T
    uint32 mute;
} AUDIO_CTL_ARG_U;

// hal cmd enum
//// hal related
enum AUDIO_HAL_CMD_E
{
    AUDIO_HAL_CMD_BEGIN = 0,

    // get current fun_flg
    AUDIO_HAL_CMD_FUN_FLG_GET,

    //TODO: add here.
    AUDIO_HAL_CMD_END,
    AUDIO_HAL_CMD_MAX = 0xFFFFFFFF //force this enum use uint32
};

// dai cmd enum
//// aif dai related
enum AUDIO_DAI_CMD_E
{
    AUDIO_DAI_CMD_BEGIN = AUDIO_HAL_CMD_END,

    // check_fs
    AUDIO_DAI_CMD_FS_CHECK,
    // tone cmd
    AUDIO_DAI_CMD_TONE,
    // set channel
    AUDIO_DAI_CMD_CHANNEL_SET,
    // get channel
    AUDIO_DAI_CMD_CHANNEL_GET,

    //TODO: add here.
    AUDIO_DAI_CMD_END,
    AUDIO_DAI_CMD_MAX = 0xFFFFFFFF //force this enum use uint32
};

// codec cmd enum
//// codec related
enum AUDIO_CODEC_CMD_E
{
    AUDIO_CODEC_CMD_BEGIN = AUDIO_DAI_CMD_END,

    // check_fs
    AUDIO_CODEC_CMD_FS_CHECK,
    // get support path
    AUDIO_CODEC_CMD_SUPPORT_PATH_GET,
    // set new output path (path and gain)
    AUDIO_CODEC_CMD_OUTPUT_PATH_SET,
    // set new input path (path and gain)
    AUDIO_CODEC_CMD_INPUT_PATH_SET,
    // set output mute
    AUDIO_CODEC_CMD_OUTPUT_MUTE_SET,
    // set input mute
    AUDIO_CODEC_CMD_INPUT_MUTE_SET,

    //TODO: add here.
    AUDIO_CODEC_CMD_END,
    AUDIO_CODEC_CMD_MAX = 0xFFFFFFFF //force this enum use uint32
};

typedef int32 (*AUDIO_HAL_CALLBACK_FUN)(uint32);

/**----------------------------------------------------------------------------*
**                           Function Prototype                               **
**----------------------------------------------------------------------------*/

/*****************************************************************************/
// Description :    Audio Driver Init.
//                  include Codec Init, DAI Init etc.
// Return :         AUDIO_HAL_SUCCESS
// Author :         ken.kuang
// Note :
/*****************************************************************************/
PUBLIC int32 AUDIO_HAL_Init(uint32 param);

/*****************************************************************************/
// Description :    Audio Driver Open.
//                  include Codec Power On, DAI Enable.
// Return :         AUDIO_HAL_SUCCESS,AUDIO_HAL_ERROR
// Param :          fun_flg : from AUDIO_FUN_E
//                  fs : frame sample rate.
// Author :         ken.kuang
// Note :
/*****************************************************************************/
PUBLIC int32 AUDIO_HAL_Open(uint32 fun_flg, uint32 fs);

/*****************************************************************************/
// Description :    Audio Driver Close.
//                  include Codec Power Off, DAI Disable.
// Return :         AUDIO_HAL_SUCCESS
// Param :          fun_flg : from AUDIO_FUN_E
// Author :         ken.kuang
// Note :
/*****************************************************************************/
PUBLIC int32 AUDIO_HAL_Close(uint32 fun_flg);

/*****************************************************************************/
// Description :    Audio Driver Ioctl.
//                  include Codec Ioctl, DAI Ioctl.
// Return :         AUDIO_HAL_SUCCESS,AUDIO_HAL_ERROR
// Param :          cmd : from CODEC_CMD_E and DAI_CMD_E
//                  arg : some cmd maybe use NULL if no param
//                      if the cmd have param,please give a non-NULL point!
// Author :         ken.kuang
// Note :
/*****************************************************************************/
PUBLIC int32 AUDIO_HAL_Ioctl(uint32 cmd, uint32 arg);

/*****************************************************************************/
// Description :    Audio Driver Set Playback Buffer Info.
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
//                  size : the max size of pLHead.
//                      well, pRHead MUST the same size with pLHead if exist.
// Author :         ken.kuang
// Note :
/*****************************************************************************/
PUBLIC int32 AUDIO_HAL_DAC_Set_Buf_Info(void *pLHead, void *pRHead, uint32 size);
PUBLIC int32 AUDIO_HAL_DAC_Set_Write_Point(uint32 pWrite);
PUBLIC int32 AUDIO_HAL_DAC_Get_Read_Point(uint32 *pRead);

/*****************************************************************************/
// Description :    Audio Driver Will Trigger Playback immediately.
//
// Return :         AUDIO_HAL_SUCCESS,AUDIO_HAL_ERROR
// Param :
//
// Author :         ken.kuang
// Note :           do nothing when already trigger.
/*****************************************************************************/
PUBLIC int32 AUDIO_HAL_DAC_Trigger(void);

/*****************************************************************************/
// Description :    Audio Driver Capture Buffer Info.
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
//                  size : the max size of pLHead.
//                      well, pRHead MUST the same size with pLHead if exist.
// Author :         ken.kuang
// Note :
/*****************************************************************************/
PUBLIC int32 AUDIO_HAL_ADC_Set_Buf_Info(void *pLHead, void *pRHead, uint32 size);
PUBLIC int32 AUDIO_HAL_ADC_Set_Read_Point(uint32 pRead);
PUBLIC int32 AUDIO_HAL_ADC_Get_Write_Point(uint32 *pWrite);

/*****************************************************************************/
// Description :    Audio Driver Will Trigger Captrue immediately.
//
// Return :         AUDIO_HAL_SUCCESS,AUDIO_HAL_ERROR
// Param :
//
// Author :         ken.kuang
// Note :           do nothing when already trigger.
/*****************************************************************************/
PUBLIC int32 AUDIO_HAL_ADC_Trigger(void);

/*****************************************************************************/
// Description :    Audio Driver Register Playback Callback.
//                  Use to Notify User the Driver fetch data gone.
//                  User can do something like start decode audio data.
// Return :         AUDIO_HAL_SUCCESS,AUDIO_HAL_ERROR
// Param :          pFun : this function.
//
// Author :         ken.kuang
// Note :  This Callback DO NOT call deep tree.
//          should be send message or set flag etc.
/*****************************************************************************/
PUBLIC int32 AUDIO_HAL_Register_Playback(AUDIO_HAL_CALLBACK_FUN pFun);

/*****************************************************************************/
// Description :    Audio Driver Register Capture Callback.
//                  Use to Notify User for fetch data please.
//                  User should be move audio data ASAP.
// Return :         AUDIO_HAL_SUCCESS,AUDIO_HAL_ERROR
// Param :          pFun : this function.
//
// Author :         ken.kuang
// Note :  This Callback DO NOT call deep tree.
//          should be send message or set flag etc.
/*****************************************************************************/
PUBLIC int32 AUDIO_HAL_Register_Capture(AUDIO_HAL_CALLBACK_FUN pFun);

/*****************************************************************************/
// Description :    Audio Driver Check fs if dai and codec support.
// Return :         AUDIO_HAL_SUCCESS,AUDIO_HAL_ERROR
// Param :          pFun : fs.
//
// Author :         rui.huang
// Note :
/*****************************************************************************/
PUBLIC int32 AUDIO_HAL_FS_CHECK(uint32 fs);

/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif
/**---------------------------------------------------------------------------*/
#endif //_AUDIO_DRVAPI_H_
// End
