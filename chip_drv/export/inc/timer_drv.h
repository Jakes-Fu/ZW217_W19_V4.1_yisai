/******************************************************************************
 ** File Name:      timer_drv.h                                               *
 ** Author:         Richard Yang                                              *
 ** DATE:           04/02/2002                                                *
 ** Copyright:      2002 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the basic operation interfaces of       *
 **                 time count and rtc device. It manages init, get and set   *
 **                 new time interface.                                       *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 04/02/2002     Richard.Yang     Create.                                   *
 ** 04/25/2002     Lin.liu          Added rtc function prototype              *
 ** 10/08/2002     Xueliang.Wang    Modified.                                 *
 ******************************************************************************/

#ifndef _TIMER_DRV_H_
#define _TIMER_DRV_H_
#ifdef   __cplusplus
extern   "C"
{
#endif

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "tb_hal.h"
#include "timer_drvapi.h"

#define TB_TIMER_PERIODIC_MODE  TIMER_PERIOD_MODE
#define TB_TIMER_ONCE_MODE      TIMER_ONESHOT_MODE

// TIMER callback function prototype.
typedef TIMER_CALLBACK_FUNC TB_TIMER_CALLBACK;

/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/
/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    Initialize TIMER.
//                  1. Initialize loacal variables
//                  2. Enable System Counter device.
//                  3. Enable TIMER device.
//                  4. Disable TIMER interrupt and counting.
//                  5. Register ISR of TIMER.
//  Author:         Xueliang.Wang
//  Note:           If initialize successfully, return TB_SUCCESS;
//                  else return error code.
/*****************************************************************************/
PUBLIC uint32 TIMER_Init (void);

/*****************************************************************************/
//  Description:    Set TIMER value.
//                  Set reset_milsec according to the mode.
//                  Register callback function.
//  Author:         Xueliang.Wang
//  Note:
/*****************************************************************************/
PUBLIC uint32 TIMER_Set (   // If set successfully, return TB_SUCCESS;
    // else return error code.
    uint32 timer_id,        // Indicate which timer to be set.
    uint32 mil_sec,         // Time value to be set.
    uint32 mode,            // Indicate the mode of the timer.
    // TB_TIMER_PERIODIC_MODE
    // TB_TIMER_FREE_MODE
    TB_TIMER_CALLBACK func_ptr // When timer is expired, call this function.
);

/*****************************************************************************/
//  Description:    Get remaining time of the proper TIMER.
//  Author:         Xueliang.Wang
//  Note:
/*****************************************************************************/
PUBLIC uint32 TIMER_GetRemaining (  // Return remaining time in millisecond.
    uint32 timer_id                 // Indicate which timer to be get.
);

/*****************************************************************************/
//  Description:    Get the number of ticks that have elapsed since the
//                  system was started.
//  Author:         Lin.Liu
//  Note:
/*****************************************************************************/
PUBLIC uint32 TIMER_GetTickCount (void); // return the number of tick count.

/*****************************************************************************/
//  Description:    Disable TIMER .
//  Author:         Xueliang.Wang
//  Note:
/*****************************************************************************/
PUBLIC void TIMER_DisableTimer (
    uint32 timer_id         // Indicate which timer to be disabled.
);

#ifdef   __cplusplus
}
#endif
#endif  // _TIMER_DRV_H_


