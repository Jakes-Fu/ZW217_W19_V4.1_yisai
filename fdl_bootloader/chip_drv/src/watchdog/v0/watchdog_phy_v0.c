/******************************************************************************
 ** File Name:      watchdog_phy_v0.c                                                 *
 ** Author:         Jie Dai                                                   *
 ** DATE:           08/02/2010                                                *
 ** Copyright:      2010 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file define the physical layer of I2C device.      *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 08/02/2010     Jie Dai            Create.                                 *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "fdl_bootloader_trc.h"
#include "os_api.h"
#include "sc_reg.h"
#include "..\watchdog_phy.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif

/**---------------------------------------------------------------------------*
 **                            Macro Define
 **---------------------------------------------------------------------------*/
#define  WDG_TRACE

/*****************************************************************************/
//  Description:    This function config the watch dog module.
//  Dependency:     No
//  Author:         Jie.Dai
//  Note:
/*****************************************************************************/
PUBLIC int32 WDG_PHY_CONFIG (WDG_CONFIG_T *cfg)
{
    //uint32 ctrl = 0;
    //uint32 val  = 0;

    //WDG_TRACE:"Watch Dog Trace: Watch Dog Value 0x%8.8x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,WATCHDOG_PHY_V0_41_112_2_18_1_0_26_3,(uint8*)"d", CHIP_REG_GET (WDG_VALUE));

    CHIP_REG_SET (WDG_LOCK, WDG_UNLOCK_KEY);

    switch (cfg->mode)
    {
        case WDG_TIMEOUT_MODE_RESET:
            CHIP_REG_AND (WDG_CTL, ~WDG_CTL_INT_EN);
            break;

        case WDG_TIMEOUT_MODE_INT:
            CHIP_REG_OR (WDG_CTL, WDG_CTL_INT_EN);
            break;

        default:
            break;  //No need to change
    }

    if (WDG_TIMER_STATE_STOP != cfg->state)
    {
        CHIP_REG_SET (WDG_LOAD, cfg->val);
    }

    switch (cfg->state)
    {
        case WDG_TIMER_STATE_STOP:
            CHIP_REG_AND (WDG_CTL, ~WDG_CTL_CNT_EN);
            break;

        case WDG_TIMER_STATE_START:
            CHIP_REG_OR (WDG_CTL, WDG_CTL_CNT_EN);
            break;

        default:
            break;  //No need to change
    }

    //WDG_TRACE:"Watch Dog Trace: Watch Dog Control 0x%8.8x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,WATCHDOG_PHY_V0_78_112_2_18_1_0_26_4,(uint8*)"d", CHIP_REG_GET (WDG_CTL));
    //WDG_TRACE:"Watch Dog Trace: Watch Dog LOAD    0x%8.8x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,WATCHDOG_PHY_V0_79_112_2_18_1_0_26_5,(uint8*)"d", CHIP_REG_GET (WDG_LOAD));

    CHIP_REG_SET (WDG_LOCK, WDG_LOCK_KEY);

    return 0;
}

/*****************************************************************************/
//  Description:    This function clear the watch dog interrupt
//  Dependency:     No
//  Author:         Jie.Dai
//  Note:
/*****************************************************************************/
PUBLIC int32 WDG_PHY_INT_CLR (void)
{
    CHIP_REG_SET (WDG_LOCK, WDG_UNLOCK_KEY);

    CHIP_REG_SET (WDG_INT_CLR, WDG_INT_CLR_BIT);

    CHIP_REG_SET (WDG_LOCK, WDG_LOCK_KEY);

    return 0;
}


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/

#ifdef   __cplusplus
}
#endif

/*  End Of File */


