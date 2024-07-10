/******************************************************************************
 ** File Name:      vbc_alg_phy_v3.c                                          *
 ** Author:         ken.kuang                                                 *
 ** DATE:           12/12/2011                                                *
 ** Copyright:      2011 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    Physical layer of voiceband                               *
 **                                                                           *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 12/12/2011     ken.kuang          Create.                                 *
 *****************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "chip_plf_export.h"
#include "vb_drvapi.h"
#include "vbc_phy_v0.h"
#include "audio_hal.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C"
{
#endif

/**---------------------------------------------------------------------------*
 **                         Macro defination                                  *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Global variables                                  *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Local variables                                   *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                     Local Function Prototypes                             *
 **---------------------------------------------------------------------------*/

#define VBC_HPF_ENABLE   1

/*****************************************************************************/
// Description :
// Author :         cherry.liu
// Note :
/*****************************************************************************/
PUBLIC void VB_PHY_HPFSwitch (BOOLEAN is_enable)
{
#if (VBC_HPF_ENABLE)
    if (is_enable)
    {
        VB_WRITE_BITS (HPCTL,
                       VBHP_EN_ENABLE,
                       VBHP_EN_MASK);
    }
    else
    {
        VB_WRITE_BITS (HPCTL,
                       VBHP_EN_DISABLE,
                       VBHP_EN_MASK);
    }
#endif
}

/*****************************************************************************/
// Description :
// Author :         cherry.liu
// Note :
/*****************************************************************************/
PUBLIC void VB_PHY_SetHPFMode (BOOLEAN is_reset_mode)
{
#if (VBC_HPF_ENABLE)
    if (is_reset_mode)
    {
        VB_WRITE_BITS (HPCTL,
                       VBHP_REG_CLR_ON,
                       VBHP_REG_CLR_MASK);
    }
    else
    {
        VB_WRITE_BITS (HPCTL,
                       VBHP_REG_CLR_OFF,
                       VBHP_REG_CLR_MASK);
    }
#endif
}

/*****************************************************************************/
// Description :
// Author :         cherry.liu
// Note :
/*****************************************************************************/
PUBLIC void VB_PHY_SetHPFParas (
    int16 filter_index,
    int16 B0,
    int16 B1,
    int16 B2,
    int16 A0,
    int16 minusA1,
    int16 minusA2
)
{
#if (VBC_HPF_ENABLE)
    switch (filter_index)
    {
        case 1:
            {
                REG32 (HPCOEF1) = B0;
                REG32 (HPCOEF3) = B1;
                REG32 (HPCOEF5) = B2;
                REG32 (HPCOEF2) = A0;
                REG32 (HPCOEF4) = minusA1;
                REG32 (HPCOEF6) = minusA2;
            }
            break;
        case 2:
            {
                REG32 (HPCOEF8) = B0;
                REG32 (HPCOEF10) = B1;
                REG32 (HPCOEF12) = B2;
                REG32 (HPCOEF9) = A0;
                REG32 (HPCOEF11) = minusA1;
                REG32 (HPCOEF13) = minusA2;
            }
            break;
        default:
            break;
    }
#endif
}


/*****************************************************************************/
// Description :
// Author :         cherry.liu
// Note :
/*****************************************************************************/
PUBLIC void VB_PHY_SetHPFGain (
    int16 gain_index,//gain index:0,1,2,3,4,5,6
    int16 gain_value
)
{
#if (VBC_HPF_ENABLE)
    switch (gain_index)
    {
        case 0://S0  Q=12
            {
                REG32 (HPCOEF0) = gain_value;
            }
            break;
        case 1://S1 Q=15
            {
                REG32 (HPCOEF7) = gain_value;
            }
            break;
        case 2://S2 Q=15
            {
                REG32 (HPCOEF14) = gain_value;
            }
            break;
        default:
            break;
    }
#endif
}

/*****************************************************************************/
// Description :
// Author :         cherry.liu
// Note :
/*****************************************************************************/
PUBLIC uint32 VB_PHY_GetHPFGain (
    int16 gain_index
)
{
    uint32 gain = 0;
#if (VBC_HPF_ENABLE)
    switch (gain_index)
    {
        case 0://S0  Q=12
            {
                gain = REG32 (HPCOEF0);
            }
            break;
        case 1://S1 Q=15
            {
                gain = REG32 (HPCOEF7);
            }
            break;
        case 2://S2 Q=15
            {
                gain = REG32 (HPCOEF14);
            }
            break;
        default:
            {
                gain = 0;
            }
            break;
    }
#endif

    return gain;
}

/*****************************************************************************/
// Description :
// Author :         cherry.liu
// Note :
/*****************************************************************************/
PUBLIC void VB_PHY_SetHPFLimit (
    int8 rLimit //0~127
)
{
#if (VBC_HPF_ENABLE)
    int16 limit_value = rLimit;

    if (limit_value < 0)
    {
        limit_value = 0;
    }

    VB_WRITE_BITS (HPCTL,
                   limit_value << VBHP_LIMIT_SHIFT,
                   VBHP_LIMIT_MASK);
#endif
}

/*****************************************************************************/
// Description :
// Author :         cherry.liu
// Note :
/*****************************************************************************/
PUBLIC void VB_PHY_SetHPFWidth (
    uint16  width //width = 16 or 24 (bits)
)
{
#if (VBC_HPF_ENABLE)
    uint16 data_width_value = 0;

    switch (width)
    {
        case 16:
            data_width_value = VBHP_WID_SEL_16BITS;
            break;
        case 24:
            data_width_value = VBHP_WID_SEL_24BITS;
            break;
        default:
            data_width_value = VBHP_WID_SEL_16BITS;
            break;
    }

    if ( (REG32 (HPCTL) & VBHP_WID_SEL_MASK) != data_width_value)
    {
        VB_WRITE_BITS (HPCTL,
                       data_width_value,
                       VBHP_WID_SEL_MASK);
    }
#endif
}

/*****************************************************************************/
// Description :
// Author :         cherry.liu
// Note :
/*****************************************************************************/
PUBLIC void VB_PHY_ALCSwitch (
    BOOLEAN is_enable
)
{
#if (VBC_HPF_ENABLE)
#endif
}

/*****************************************************************************/
// Description :
// Author :         cherry.liu
// Note :
/*****************************************************************************/
PUBLIC void VB_PHY_SetALCParas (
    VBC_ALC_PARAS_T *ptAlcPara
)
{
#if (VBC_HPF_ENABLE)
#endif
}


/*****************************************************************************/
// Description :
// Author :         cherry.liu
// Note :
/*****************************************************************************/
PUBLIC void VB_PHY_SetFMMixMode (
    int16 da_index,//0,1
    int16 mix_mode  //0,1,2
)
{
#if (VBC_HPF_ENABLE)
#endif
}

/*****************************************************************************/
// Description :
// Author :         cherry.liu
// Note :
/*****************************************************************************/
PUBLIC void VB_PHY_DGSwitch (
    int16 da_index,//0,1
    BOOLEAN is_enable
)
{
#if (VBC_HPF_ENABLE)
#endif
}

/*****************************************************************************/
// Description :
// Author :         cherry.liu
// Note :
/*****************************************************************************/
PUBLIC void VB_PHY_SetDG (
    int16 da_index,//0,1
    int16 dg_value
)
{
#if (VBC_HPF_ENABLE)
#endif
}


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif


