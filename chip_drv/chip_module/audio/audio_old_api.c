/******************************************************************************
 ** File Name:      audio_old_api.c                                           *
 ** Author:         ken.kuang                                                 *
 ** DATE:           12/12/2011                                                *
 ** Copyright:      2011 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file define the basic hw interfaces of audio device. *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 12/12/2011     ken.kuang        Create.                                   *
 ******************************************************************************/

/*----------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **------------------------------------------------------------------------- */

#include "chip_drv_trc.h"
#include "sci_types.h"
#include "sci_api.h"

#include "audio_drvapi.h"
#include "vb_drvapi.h"
#include "audio_hal.h"


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
#define AUDIO_OLD_API_PRT       SCI_TRACE_LOW

/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         LOCAL Data                                        *
 **---------------------------------------------------------------------------*/

/**----------------------------------------------------------------------------*
**                   LOCAL  Function Prototype                                **
**----------------------------------------------------------------------------*/

/**----------------------------------------------------------------------------*
**                           PUBLIC Function Prototype                        **
**----------------------------------------------------------------------------*/
volatile PUBLIC BOOLEAN b_is_vb_playing    = FALSE;

/*****************************************************************************/
//  Description:    This function is used to add some operation with VB when
//                  headset has been detectted in/out.
//  Author:         Jeff.Li
//  input:          status:  1--Headset detectted in
//                           0--Headset detectted out
//  Note:
//****************************************************************************/
PUBLIC void AUDDEV_HPDetectRoutine (BOOLEAN status)
{
#if 0 // use current system voltage change the LDO LEVEL.
    if (status) // Headset detectted in
    {
        // Set VBOLDO to 2.9V, to restrain RF noise in headset mode while the call
        // has just been connected.
        LDO_SetVoltLevel (LDO_LDO_AVBO, LDO_VOLT_LEVEL3);
    }
    else        // Headset detectted out
    {
        // Restore VBO LDO to 3.3V, when headset out detectted.
        LDO_SetVoltLevel (LDO_LDO_AVBO, LDO_VOLT_LEVEL0);
    }
#endif
}

/*****************************************************************************/
// Description :
// Author :         Jeff.Li
// Note :
/*****************************************************************************/
PUBLIC void VB_Hold_by_DSP (uint32 sample_rate)
{
    SCI_ASSERT(0 && "old api. do not use!");/*assert to do*/
}

/*****************************************************************************/
//  Description:    This function set VB CPG
//  Author:         Jimmy.Jia
//  Note:           speaker volume level need TBD
//                  Now we provide this function in l1_midi.c for convenience
/*****************************************************************************/
PUBLIC void VB_ARM_SetVolume (uint32  speaker_vol)
{
    SCI_ASSERT(0 && "old api. do not use!");/*assert to do*/
}

/*****************************************************************************/
//  Description:    This function is used to open or close PA.
//  Author:         Benjamin.Wang
//  Note:
/*****************************************************************************/
PUBLIC void VB_OpenPA (BOOLEAN is_enable)
{//NEED CHECK!!
    //USED BY LAYER1 AUDIO:::SCI_ASSERT(0 && "old api. do not use!");
}

/*****************************************************************************/
//  Description:    This function set VB sound device
//  Author:         Jimmy.Jia
//  Note:           Now we provide this function in l1_midi.c for convenience
/*****************************************************************************/
PUBLIC void VB_ARM_SetSoundDevice (
    BOOLEAN  b_aux_mic,         // TRUE: use mic2,    FALSE: use mic1
    BOOLEAN  b_aux_speaker,     // TRUE: use speaker, FALSE: use receiver
    uint16   mode
)
{
    SCI_ASSERT(0 && "old api. do not use!");/*assert to do*/
}

/*****************************************************************************/
//  Description:    This function is used to know which is the controller of
//                  VB, ARM or DSP.
//  Author:         Benjamin.Wang
//  Note:
//****************************************************************************/
PUBLIC BOOLEAN VB_IsControlledbyDSP (void)
{
    return 1;
}

/*****************************************************************************/
// Description :    get sm status.
// Author :         
// Note : !!! Called by atc_eng.c, Dummy it just for compilinig.
/*****************************************************************************/
PUBLIC BOOLEAN AUDDEV_GetSMStatus (void)
{
    return SCI_FALSE;
}


/*****************************************************************************/
// Description :
// Author :         ken.kuang
// Note :
/*****************************************************************************/
PUBLIC void VB_SwitchHpf (
    BOOLEAN is_enable // 1:open ;0: close
)
{
    //AUDIO_OLD_API_PRT:"[VB_SwitchHpf]:is_enable:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_OLD_API_128_112_2_17_23_2_37_50,(uint8*)"d",is_enable);
    VB_PHY_HPFSwitch (is_enable);
   /* if (is_enable)
    {
        VB_PHY_HPFSwitch (is_enable);
    }
    else
    {
        VB_PHY_HPFSwitch (is_enable);
    }*/    //coverity warnning:the same branch
}

/*****************************************************************************/
// Description :
// Author :         Jeff.Li
// Note :
/*****************************************************************************/
PUBLIC void VB_SetHpfMode (
    BOOLEAN is_reset_mode
)
{
    //AUDIO_OLD_API_PRT:"[VB_SetHpfMode]:is_reset_mode:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_OLD_API_146_112_2_17_23_2_37_51,(uint8*)"d",is_reset_mode);
    VB_PHY_SetHPFMode (is_reset_mode);
}

/*****************************************************************************/
// Description :
// Author :         Jeff.Li
// Note :  cherry add
// for 6800h,8800s4,6606l,6610  ,filter index:1,2
// for 8800G  ,                  filter index:1,2,3,4,5,6
/*****************************************************************************/
PUBLIC void VB_SetHpfParas (
    int16 filter_index,
    int16 B0,
    int16 B1,
    int16 B2,
    int16 A0,
    int16 minusA1,
    int16 minusA2
)
{
    VB_PHY_SetHPFParas (
        filter_index,
        B0,
        B1,
        B2,
        A0,
        minusA1,
        minusA2);
}

/*****************************************************************************/
// Description :
// Author :         Jeff.Li
// Note :  cherry add
// for 6800h,8800s4,6606l,6610  ,gain index:0,1,2
// for 8800G  ,                  gain index:0,1,2,3,4,5,6
/*****************************************************************************/
PUBLIC void VB_SetHpfGain (
    int16 gain_index,
    int16 gain_value
)
{
    //AUDIO_OLD_API_PRT:"[VB_SetHpfGain]:gain_index:%d,gain_value:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_OLD_API_186_112_2_17_23_2_37_52,(uint8*)"dd",gain_index, gain_value);
    VB_PHY_SetHPFGain (gain_index, gain_value);
}

/*****************************************************************************/
// Description :
// Author :         Jeff.Li
// Note :  cherry add
// for 6800h,8800s4,6606l,6610  ,gain index:0,1,2
// for 8800G  ,                  gain index:0,1,2,3,4,5,6
/*****************************************************************************/
PUBLIC uint32 VB_GetHpfGain (
    int16 gain_index
)
{
    uint32 gain = 0;
    //AUDIO_OLD_API_PRT:"[VB_GetHpfGain]:gain_index:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_OLD_API_200_112_2_17_23_2_37_53,(uint8*)"d",gain_index);
    gain = VB_PHY_GetHPFGain (gain_index);

    return gain;
}

/*****************************************************************************/
// Description :
// Author :         Jeff.Li
// Note :
/*****************************************************************************/
PUBLIC void VB_SetHpfLimit (
    int8 rLimit //0~127
)
{
    //AUDIO_OLD_API_PRT:"[VB_SetHpfLimit]:rLimit:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_OLD_API_213_112_2_17_23_2_37_54,(uint8*)"d",rLimit);
    VB_PHY_SetHPFLimit (rLimit);
}

/*****************************************************************************/
// Description :
// Author :         Jeff.Li
// Note :
/*****************************************************************************/
PUBLIC void VB_SetHpfWidth (
    uint16  width //width = 16 or 24 (bits)
)
{
    //AUDIO_OLD_API_PRT:"[VB_SetHpfWidth]:width:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_OLD_API_224_112_2_17_23_2_37_55,(uint8*)"d",width);
    VB_PHY_SetHPFWidth (width);
}

/*****************************************************************************/
// Description :
// Author :         cherry.liu
// Note :
/*****************************************************************************/
PUBLIC void VB_ALCSwitch (
    BOOLEAN is_enable
)
{
    //AUDIO_OLD_API_PRT:"[VB_ALCSwitch]:is_enable:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_OLD_API_235_112_2_17_23_2_37_56,(uint8*)"d",is_enable);
    VB_PHY_ALCSwitch (is_enable);
}

/*****************************************************************************/
// Description :
// Author :         cherry.liu
// Note :
/*****************************************************************************/
PUBLIC void VB_SetALCParas (
    VBC_ALC_PARAS_T *ptAlcPara
)
{
    SCI_ASSERT (SCI_NULL != ptAlcPara);/*assert verified*/
    //AUDIO_OLD_API_PRT:"[VB_SetALCParas]:hold:%d,rise:%d,fall:%d,limit:%d,threshold:%d,ratio:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_OLD_API_247_112_2_17_23_2_37_57,(uint8*)"dddddd",ptAlcPara->hold,ptAlcPara->rise,ptAlcPara->fall,ptAlcPara->limit,ptAlcPara->threshold,ptAlcPara->ratio);
    //AUDIO_OLD_API_PRT:"[VB_SetALCParas]:cg_var:%d,release_rate:%d,attack_rate:%d,release_rate_ex:%d,attack_rate_ex:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_OLD_API_249_112_2_17_23_2_37_58,(uint8*)"ddddd",ptAlcPara->cg_var,ptAlcPara->release_rate,ptAlcPara->attack_rate,ptAlcPara->release_rate_ex,ptAlcPara->attack_rate_ex);


    VB_PHY_SetALCParas (ptAlcPara);
}

/*****************************************************************************/
// Description :
// Author :         cherry.liu
// Note :
/*****************************************************************************/
PUBLIC void VB_SetFMMixMode (
    VBC_DA_CHANNEL_E da_index,
    VBC_DA_MIX_MODE_E mix_mode
)
{
    //AUDIO_OLD_API_PRT:"[VB_SetFMMixMode]:da_index:%d,mix_mode:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_OLD_API_264_112_2_17_23_2_37_59,(uint8*)"dd",da_index,mix_mode);
    VB_PHY_SetFMMixMode (da_index, mix_mode);
}

/*****************************************************************************/
// Description :
// Author :         cherry.liu
// Note :
/*****************************************************************************/
PUBLIC void VB_DGSwitch (
    VBC_DA_CHANNEL_E da_index,
    BOOLEAN is_enable
)
{
    //AUDIO_OLD_API_PRT:"[VB_DGSwitch]:da_index:%d,is_enable:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_OLD_API_276_112_2_17_23_2_37_60,(uint8*)"dd",da_index,is_enable);
    VB_PHY_DGSwitch (da_index, is_enable);
}

/*****************************************************************************/
// Description :
// Author :         cherry.liu
// Note :
/*****************************************************************************/
PUBLIC void VB_SetDG (
    VBC_DA_CHANNEL_E da_index,
    int16 dg_value
)
{
    //AUDIO_OLD_API_PRT:"[VB_SetDG]:da_index:%d,dg_value:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_OLD_API_288_112_2_17_23_2_37_61,(uint8*)"dd",da_index,dg_value);
    VB_PHY_SetDG (da_index, dg_value);
}

/**----------------------------------------------------------------------------*
**                           LOCAL Function Prototype                         **
**----------------------------------------------------------------------------*/



/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif
/**---------------------------------------------------------------------------*/

// End
