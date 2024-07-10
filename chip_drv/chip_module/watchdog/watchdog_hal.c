/******************************************************************************
 ** File Name:      watchdog_hal.c                                                 *
 ** Author:         Jimmy.Jia                                                 *
 ** DATE:           10/28/2004                                                *
 ** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    This file defines the basic operation interfaces of       *
 **                 serrial device. It manages create, send, receive          *
 **                 interface of serrial port.                                *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 04/02/2002     Jimmy.Jia        Create.                                   *
**  01/03/2008     Yi.Qiu        For SC6600L                                  *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "chip_drv_trc.h"
#include "sci_types.h"
#include "os_api.h"
#include "chip_plf_export.h"
#include "wdg_drvapi.h"
//#include "fiq_drvapi.h"
#include "watchdog_phy.h"
#include "ref_outport.h"

#define  WDG_TRACE  SCI_TRACE_LOW
/**---------------------------------------------------------------------------*
 **                      Function Declaration                                 *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    This function for save space
//  Author:         Tianyu.Yang
//  Note:           
//
/*****************************************************************************/
static void WDG_PHY_Config_Public(WDG_CONFIG_T *cfg)
{
	int32 ret;
	ret = WDG_PHY_CONFIG (cfg);
	SCI_PASSERT ( (WDG_RESULT_OK == ret),  /*assert verified*/
                  ("%s, %d, WDG Config: %d", __MODULE__, __LINE__, ret));
}

/*****************************************************************************/
//  Description:    This function disable WDG timer
//  Author:         Jimmy.Jia
//  Note:           no rtc_clk to drive the counter
//
/*****************************************************************************/
PUBLIC void WDG_TimerStop
(
    void
)
{
    WDG_CONFIG_T config;
    
    //WDG_TRACE:"Watch Dog Trace: WDG_TimerStop"
//    SCI_TRACE_ID(TRACE_TOOL_CONVERT,WATCHDOG_HAL_44_112_2_17_23_8_6_1493,(uint8*)"");
    WDG_PHY_INT_CLR();

    config.state    = WDG_TIMER_STATE_STOP  ;
    config.mode     = WDG_TIMEOUT_MODE_RESET;
    config.val      = WDG_MAX_COUNT;

    WDG_PHY_Config_Public (&config);

    OS_TickDelay (1);

    return;
}

/*****************************************************************************/
//  Description:    This function initialize and setup timer with given value
//  Author:         Jimmy.Jia
//  Note:           rtc_clk drive the counter
//                  load regiter is 32 bits wide so that don't check if param's valid
/*****************************************************************************/
PUBLIC void WDG_TimerStart
(
    uint32 init_val
)
{
    WDG_CONFIG_T config;

    //WDG_TRACE:"Watch Dog Trace: WDG_TimerStart"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,WATCHDOG_HAL_70_112_2_17_23_8_6_1494,(uint8*)"");

    config.state    = WDG_TIMER_STATE_START ;
    config.mode     = WDG_TIMEOUT_MODE_KEEP;
    config.val      = init_val;

    WDG_PHY_Config_Public (&config);

    OS_TickDelay (1);
}

/*****************************************************************************/
//  Description:    This function handle WatchDog Interrupt
//  Author:         Binggo Zhou
//  Note:
/*****************************************************************************/
PUBLIC void WDG_TimerHandler (uint32 int_num)
{
    WDG_PHY_INT_CLR();

//    SCI_TRACE_LOW("WDG_TimerHandler");
    SCI_PASSERT ( 0, ("WDG int Assert Before Watchdog Reset!"));/*assert verified*/
}

/*****************************************************************************/
//  Description:    This function init WatchDog
//  Author:         Binggo Zhou
//  Note:
/*****************************************************************************/
PUBLIC void WDG_TimerInit (void)
{
    WDG_CONFIG_T config;

    //WDG_TRACE:"Watch Dog Trace: WDG_TimerInit"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,WATCHDOG_HAL_101_112_2_17_23_8_6_1495,(uint8*)"");

    config.state    = WDG_TIMER_STATE_START ;
    config.mode     = WDG_TIMEOUT_MODE_RESET;
    config.val      = WDG_INIT_COUNT;
        
    WDG_PHY_Config_Public(&config);
}

PUBLIC void WDG_TimerSleepEnter (uint32 time_ms)
{
    WDG_CONFIG_T config;

    config.state    = WDG_TIMER_STATE_START ;
    config.mode     = WDG_TIMEOUT_MODE_RESET;
    config.val      = (time_ms * 32768) / 1000;;
    WDG_TimerStop();
    WDG_PHY_Config_Public (&config);
}

PUBLIC void WDG_TimerSleepExit (void)
{
    WDG_CONFIG_T config;

    config.state    = WDG_TIMER_STATE_START ;
    config.mode     = WDG_TIMEOUT_MODE_RESET;
    config.val      = WDG_INIT_COUNT;
        
    WDG_PHY_Config_Public (&config);
}

/*****************************************************************************/
//  Description:    This function Reset MCU
//  Author:         Younger.Yang
//  Note:
/*****************************************************************************/
PUBLIC void WDG_ResetMCU (void)
{
    WDG_CONFIG_T config;

    //WDG_TRACE:"Watch Dog Trace: WDG_ResetMCU"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,WATCHDOG_HAL_121_112_2_17_23_8_6_1496,(uint8*)"");
    config.state    = WDG_TIMER_STATE_START ;
    config.mode     = WDG_TIMEOUT_MODE_RESET;
    config.val      = 0x50;

    WDG_PHY_Config_Public (&config);
}

/*****************************************************************************/
//  Description:    This function Load WatchDog Timer Value
//  Author:         Binggo Zhou
//  Note:
/*****************************************************************************/
PUBLIC void WDG_TimerLoad (uint32 time_ms)
{
    WDG_CONFIG_T config;

    //WDG_TRACE:"Watch Dog Trace: WDG_TimerLoad %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,WATCHDOG_HAL_139_112_2_17_23_8_6_1497,(uint8*)"d", time_ms);

    config.state    = WDG_TIMER_STATE_KEEP  ;
    config.mode     = WDG_TIMEOUT_MODE_KEEP ;
    config.val      = (time_ms * 32768) / 1000;

    WDG_PHY_Config_Public (&config);
}
