/******************************************************************************
 ** File Name:      sp7100g_cfg.c                                             *
 ** Author:         Xueliang.Wang                                             *
 ** DATE:           08/23/2004                                                *
 ** Copyright:      2004 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the config information of SP7100G       *
 **                 project.                                                  *
 **                                                                           *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE            NAME            DESCRIPTION                               *
 ** 08/23/2004      Xueliang.Wang   Create.                                   *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "arm_reg.h"
#include "sc_reg.h"
#include "sc_reg.h"
#include "chng_freq.h"
//#include "emc_drv.h"
#include "chip.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif

//-----------------------------------------------------------------------------
//
//  The initialized frequency when system start up.
//

//-----------------------------------------------------------------------------
//Global variable contain system clk config
LOCAL const SYS_CLK_CFG_INFO g_system_clk_cfg =
{

#if  defined PLATFORM_SC6800H
    MPLL_MN_192M_EX26M,
    UPLL_MN_192M_EX26M,
    P192M_ARM192M_AHB96M
    
#else
    0,
    0,
    0
#endif
};

/*****************************************************************************/
//  Description:    This function get system clk config pointer
//  Author:         Younger.yang
//  Note:
/*****************************************************************************/
SYS_CLK_CFG_INFO *Get_system_clk_cfg (void)
{

    return (SYS_CLK_CFG_INFO *) &g_system_clk_cfg;
}

uint32 FDL_GetBootMode (void)
{
    uint32 bootMode = 0;
#ifdef  PLATFORM_SC6800H
    bootMode = (* (volatile uint32 *) (0x8b00003C) >> 8) & 0xff; //read HWRST reg bit[15:8]
#endif
#ifdef  PLATFORM_SC8800G
    bootMode = (* (volatile uint32 *) (0x8b000020) >> 8) & 0xff; //read HWRST reg bit[15:8]
#endif
#ifdef  PLATFORM_SC6600L
    bootMode = (* (volatile uint32 *) (0x8b000020) >> 8) & 0xff; //read HWRST reg bit[15:8]
#endif
#if defined(PLATFORM_SC6530)||defined(PLATFORM_SC6531)
    bootMode = (* (volatile uint32 *) (0x8b000228) >> 8) & 0xff; //read HWRST reg bit[15:8]
#endif
#if defined(PLATFORM_SC6531EFM)
    bootMode = (* (volatile uint32 *) (0x8b000228) >> 8) & 0xff; //read HWRST reg bit[15:8]
#endif
#if defined(PLATFORM_UWS6121E)
    bootMode = 0x8A; //#define BOOT_FLAG_AUTODOWNLOAD (0x8A)
#endif

    rprintf ("FDL_GetBootMode 0x%x\n", bootMode);

    return bootMode;
}
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif
