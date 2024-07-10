/******************************************************************************
 ** File Name:      sleep_phy_v5.h                                               *
 ** Author:         Richard Yang                                              *
 ** DATE:           08/25/2003                                                *
 ** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the basic operation interfaces of       *
 **                 DSP and MCU for deep sleep.                               *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 08/25/2003     Richard.Yang     Create.                                   *
 ******************************************************************************/
#ifndef _SLEEP_PHY_V5_H_
#define _SLEEP_PHY_V5_H_

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "com_drvapi.h"
#include "chip.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif

#ifndef WIN32
#include "tx_api_thumb.h"
/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/
#define MILLISECOND         180


//AHB_CTL0 bit define bit5,bit6 reserved
#define AHB_CTL0_DCAM_EB    BIT_1
#define AHB_CTL0_USBD_EB    BIT_2
#define AHB_CTL0_DMA_EB     BIT_3
#define AHB_DMA_SLEEP_MODE  BIT_4
#define AHB_CTL0_TIC_EB     BIT_7
#define AHB_CTL0_MON_EB     BIT_8
#define AHB_CTL0_EMC_EB     BIT_9
//APB REG related bit definition
#define MISC0_APB_FORCE_SLP  BIT_8
#define MISC0_APB_FORCE_ON   BIT_20
//Mcu_misc_en reg related bit definition
#define DSP_CLK_OUT_EN      BIT_15
#define MCU_MISC_HPASS      BIT_31



#endif
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif //
#endif //



