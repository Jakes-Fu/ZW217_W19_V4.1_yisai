/******************************************************************************
 ** File Name:      uix8910_sleep_cfg.c                                  *
 ** Author:         bo.tang                                                   *
 ** DATE:           07/24/2018                                                *
 ** Copyright:      2007 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the basic configuration for ldo management.*
 ******************************************************************************/

/******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 07/24/2018     bo.tang          Create.                                   *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "os_api.h"
#include "chip_plf_export.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif
/**---------------------------------------------------------------------------*
 **                         Global variables                                  *
 **---------------------------------------------------------------------------*/

CONST SLP_CTL_T sleep_cfg_data[] =
{

    {
        {
            // Sleep ahb status
            {  //  id                				ahb_dev_reg,       	mask,                     	value,          valid, reserved
                //{SLP_AHB_DCAM,    				0x20500000,     	AHB_CTL_DCAM_EB,      	SLP_BIT_CLR,  TRUE, NULL},
                {SLP_AHB_DEV_MAX,               NULL,               NULL,                   SLP_BIT_CLR,  FALSE,NULL},
            },
            //sleep apb status
            {  // id,                				apb_dev_reg,      		mask,       				value,          valid, reserved
                {SLP_APB_DEV_MAX,               NULL,               NULL,                   SLP_BIT_CLR,  FALSE,NULL},
            },
            //sleep ctrl bit status
            {	//id,                             	slp_bit_reg,     		mask,          			value,          valid,reserved
                //{SLP_CTRL_MCU_FORCE_STOP,       AHB_SLP_CTL0,       MCU_FORCE_SYS_SLP,      SLP_BIT_CLR,  TRUE, NULL},
                //{SLP_CTRL_MCU_SYS_SLEEP_EN,     AHB_SLP_CTL1,     MCU_SYS_SLP_EN,   SLP_BIT_SET,  TRUE, NULL},
                //{SLP_CTRL_MCU_DMA_WAKEUP_EN,    AHB_SLP_CTL1,       MCU_DMA_WAKE_UP_EN,     SLP_BIT_SET,  TRUE, NULL},
                //{SLP_CTRL_MCU_DEEP_SLEEP_EN, 	  AHB_SLP_CTL1,    MCU_DEEP_SLP_EN,   SLP_BIT_SET,  TRUE, NULL},
                // Actually, it's the APB_EN bit
                //{SLP_CTRL_APB_STOP,             AHB_SLP_CTL0,       APB_STOP,               SLP_BIT_CLR,  TRUE, NULL},

                //{SLP_CTRL_APB_FORCE_ON,         APB_SLP_CTL0,       APB_FORCE_ON,           SLP_BIT_CLR,  TRUE, NULL},
                //{SLP_CTRL_APB_FORCE_SLEEP,      APB_SLP_CTL0,       APB_FORCE_SLEEP,        SLP_BIT_CLR,  TRUE, NULL},
                //{SLP_CTRL_XTLEN,                APB_XTL_CTL0,       MCU_XTLEN_AUTOPD_EN,    SLP_BIT_SET,  TRUE, NULL},//remove for sc6531efm
                //{SLP_CTRL_DMA_SLEEP_MOD,        AHB_SLP_CTL1,       DMA_SLP_MODE,           SLP_BIT_CLR,  TRUE, NULL},
                //{SLP_CTRL_MCU_PLL_EN,           APB_PLL_CTL0,       MCU_PLL_EN,             SLP_BIT_SET,  TRUE, NULL},//remove for sc6531efm
                {SLP_CTRL_MAX,                  NULL,               NULL,                   SLP_BIT_SET,  FALSE,NULL},
            },/*lint !e785 comfirmed by xuepeng*/
            (SLP_ENTER_FUNC) tx_enter_deep_sleep
        }
    }

};

/**---------------------------------------------------------------------------*
 **                         Function Definitions                                                                               *
 **---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Function name:  Slp_Get_Cfg
//  Description  :  this function get sleep table entry according to chip type
//  Global resource dependence:
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC SLPCTL_PTR Slp_Get_Cfg (void)
{
    SLPCTL_PTR    slp_ctl_tab = NULL;

    slp_ctl_tab = (SLPCTL_PTR) sleep_cfg_data;

    return slp_ctl_tab;

}


