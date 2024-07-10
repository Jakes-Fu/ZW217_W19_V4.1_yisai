/******************************************************************************
 ** File Name:      vb_drvapi.h                                               *
 ** Author:         Jimmy.Jia                                                 *
 ** DATE:           10/01/2003                                                *
 ** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    VB related initialization and VB isr handler.             *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 10/01/2003     Jimmy.Jia        Create.                                   *
 ** 01/07/2006     Benjamin.Wang    Modify for code nesting                   *
 ** 12/12/2011     ken.kuang        Just vbc alg API. move to audio_drvapi.h  *
 ******************************************************************************/

#ifndef _VB_DRVAPI_H_
#define _VB_DRVAPI_H_

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C"
{
#endif

/**---------------------------------------------------------------------------*
 **                         MACRO Definations                                 *
 **---------------------------------------------------------------------------*/

// Controller of audio device
#define VBC_HOST ((VB_IsControlledbyDSP () == 1)? VBC_HOST_DSP:VBC_HOST_ARM)

/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/
typedef enum
{
    VBC_HOST_ARM = 0,
    VBC_HOST_DSP
}
VBC_HOST_E;

typedef enum
{
    VBC_DA_RIGHT = 0,//DAC0
    VBC_DA_LEFT,    //DAC1
    VBC_ST_RIGHT = 2,//ST0
    VBC_ST_LEFT,    //ST1
    VBC_DA_MUX
} VBC_DA_CHANNEL_E;

typedef enum
{
    VBC_DA_MIX_BYPASS = 0,//st or fm is bypassed
    VBC_DA_MIX_ADD,          //st or fm added to da path
    VBC_DA_MIX_SUBTRACT,    //st or fm subtracted from da path
    VBC_DA_MIX_MUX
} VBC_DA_MIX_MODE_E; //VBC_DA_MIX_MODE_E;

typedef struct
{
    int16  hold;
    int16  rise;
    int16  fall;
    int16  limit;
    int16  threshold;
    int16  ratio;
    int16  cg_var;
    int16  release_rate;
    int16  attack_rate;
    int16  release_rate_ex;
    int16  attack_rate_ex;
} VBC_ALC_PARAS_T;


/**---------------------------------------------------------------------------*
**                         Global Variables                                   *
**----------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               *
 **---------------------------------------------------------------------------*/

PUBLIC void VB_SwitchHpf (
    BOOLEAN is_enable // 1:open ;0: close
);

PUBLIC void VB_SetHpfMode (
    BOOLEAN is_reset_mode
);
PUBLIC void VB_SetHpfParas (
    int16 filter_index,//filter index:1,2
    int16 B0,
    int16 B1,
    int16 B2,
    int16 A0,
    int16 minusA1,
    int16 minusA2
);
PUBLIC void VB_SetHpfGain (
    int16 gain_index,//gain index:0,1,2
    int16 gain_value
);
PUBLIC uint32 VB_GetHpfGain (
    int16 gain_index//gain index:0,1,2
);
PUBLIC void VB_SetHpfLimit (
    int8 rLimit //0~127
);
PUBLIC void VB_SetHpfWidth (
    uint16  width //width = 16 or 24 (bits)
);
/*****************************************************************************/
// Description :
// Author :         cherry.liu
// Note :
/*****************************************************************************/
PUBLIC void VB_ALCSwitch (
    BOOLEAN is_enable
);
/*****************************************************************************/
// Description :
// Author :         cherry.liu
// Note :
/*****************************************************************************/
PUBLIC void VB_SetALCParas (
    VBC_ALC_PARAS_T *ptAlcPara
);
/*****************************************************************************/
// Description :
// Author :         cherry.liu
// Note :
/*****************************************************************************/
PUBLIC void VB_SetFMMixMode (
    VBC_DA_CHANNEL_E da_index,
    VBC_DA_MIX_MODE_E mix_mode
);

/*****************************************************************************/
// Description :
// Author :         cherry.liu
// Note :
/*****************************************************************************/
PUBLIC void VB_DGSwitch (
    VBC_DA_CHANNEL_E da_index,
    BOOLEAN is_enable
);

/*****************************************************************************/
// Description :
// Author :         cherry.liu
// Note :
/*****************************************************************************/
PUBLIC void VB_SetDG (
    VBC_DA_CHANNEL_E da_index,
    int16 dg_value
);
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif  // _VB_DRVAPI_H_


