/******************************************************************************
 ** File Name:      freq_phy.h                                                 *
 ** Author:         Yuhua.Shi                                                   *
 ** DATE:           08/17/2010                                                *
 ** Copyright:      2010 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file define the basic hw interfaces of FREQ module.      *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 08/17/2010     Yuhua.Shi     Create.                                   *
 ******************************************************************************/

#ifndef _FREQ_PHY_H_
#define _FREQ_PHY_H_
/*----------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **------------------------------------------------------------------------- */

#include "sci_types.h"
#include "chng_freq.h"

/**---------------------------------------------------------------------------*
 **                             Compiler Flag                                 *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif
/**---------------------------------------------------------------------------*
**                               Macro Define                                **
**---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                 *
 **---------------------------------------------------------------------------*/
//the callback function for send freqency change request
typedef BOOLEAN (* FREQ_SendReq_CALLBACK) (uint32 handler, CHNG_FREQ_MESSAGE_T *freq_param);

/**----------------------------------------------------------------------------*
**                           Function Prototype                               **
**----------------------------------------------------------------------------*/
/*****************************************************************************/
// Description :    This function is used to set the customized freq config(base address)
// Global resource dependence :
// Author :
// Note :
/*****************************************************************************/
PUBLIC void FREQ_PHY_SetCustomFreqCfg (
    uint32 *p_customized_freq_ptr//pointer of customized freqency config table
);

/*****************************************************************************/
// Description :    This function is used to detect validity of arm/ahb clk level
// Global resource dependence :
// Author :         Yuhua.Shi
// Note :
//
/*****************************************************************************/
PUBLIC BOOLEAN FREQ_PHY_DetectClkValidity (
    uint32 arm_ahb_clk_level//the arm/ahb clk level would be set
);

/*****************************************************************************/
// Description :    This function is used to get the min clk level of arm/ahb clock
// Global resource dependence :
// Author :
// Note :
/*****************************************************************************/
PUBLIC uint32 FREQ_PHY_GetMinClkLevel (
    void
);

/*****************************************************************************/
// Description :    This function is used to get the max clk level of arm/ahb clock
// Global resource dependence :
// Author :
// Note :
/*****************************************************************************/
PUBLIC uint32 FREQ_PHY_GetMaxClkLevel (
    void
);

/*****************************************************************************/
// Description :    This function is used to get the clock level
// Global resource dependence :
// Author :         Yuhua.Shi
// Note :
/*****************************************************************************/
PUBLIC uint32 FREQ_PHY_GetClkLevel (
    CHNG_FREQ_INDEX_E freq_type_index//index of frequency type
);

/*********************************************************************************************/
//Description:   This function is used to config Ahb device timing befor change arm/ahb clock
// Global resource dependence :
//Author:        Yuhua.Shi
//Note:
/*********************************************************************************************/
PUBLIC void FREQ_PHY_BeforeChangeClk (
    uint32 handler, //device handler
    uint32 arm_ahb_clk_level,//the arm/ahb clk level would be set
    FREQ_SendReq_CALLBACK SendReq_Callback_Func//callback function to send freq request
);

/*********************************************************************************************/
//Description:   This function is used to change clk(e.g. arm/ahb/emc clock)
// Global resource dependence :
//Author:        Yuhua.Shi
//Note:
/*********************************************************************************************/
PUBLIC void FREQ_PHY_ChangeClk (
    CHNG_FREQ_MESSAGE_T *freq_param//parameter of freq
);

/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif
/**---------------------------------------------------------------------------*/
#endif
// End
