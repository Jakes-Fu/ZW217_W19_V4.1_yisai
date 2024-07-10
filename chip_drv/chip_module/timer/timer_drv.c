/******************************************************************************
 ** File Name:      timer_drv.c                                               *
 ** Author:         Richard Yang                                              *
 ** DATE:           04/02/2002                                                *
 ** Copyright:      2002 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the basic operation interfaces of       *
 **                 timer count .                                             *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 04/02/2002     Richard.Yang     Create.                                   *
 ** 04/25/2002     Lin.liu          Added interface to support RTC and time   *
 ** 10/08/2002     Xueliang.Wang    Modified.                                 *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "os_api.h"
#include "timer_drvapi.h"

/**---------------------------------------------------------------------------*
 **                         Debugging Flag                                    *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif

/**---------------------------------------------------------------------------*
 **                         Function Definitions                              *
 **---------------------------------------------------------------------------*/

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
    TB_TIMER_CALLBACK func_ptr // When timer is expired, call this function.
)
{
    uint32 status = 0;
    TIMER_CONFIG_T config;

    // Check if input parameter is valid.
    SCI_ASSERT (NULL != func_ptr);/*assert verified*/
    SCI_ASSERT (timer_id < TIMER_MAX);/*assert verified*/
    SCI_ASSERT ( (TIMER_PERIOD_MODE == mode) || (TIMER_ONESHOT_MODE == mode));/*assert verified*/

    SCI_ASSERT (mil_sec != 0);/*assert verified*/
    //SCI_ASSERT (mil_sec <= (0xFFFFFFFF/TIMER_MICSEC_PER_MILSEC));

    config.mode = mode;

    //config.us = mil_sec * TIMER_MICSEC_PER_MILSEC;
    config.unit_number = mil_sec;
    config.unit_type = TIMER_UNIT_ms;

    config.call_back = func_ptr;

    status = (uint32) TIMER_HAL_Open (timer_id);

    if (status)
    {
        return status;
    }

    status = (uint32) TIMER_HAL_IOctl (timer_id, TIMER_IOCTL_CMD_CONFIG, (void *) &config);

    if (status)
    {
        return status;
    }

    status = (uint32) TIMER_HAL_IOctl (timer_id, TIMER_IOCTL_CMD_START, NULL);

    if (status)
    {
        return status;
    }

    return status;
}

/*****************************************************************************/
//  Description:    Get TIMER remaining value.
//  Author:         Xueliang.Wang
//  Note:
/*****************************************************************************/
PUBLIC uint32 TIMER_GetRemaining (  // Return remaining time in millisecond.
    uint32 timer_id                 // Indicate which timer to be get.
)
{
    TIMER_REMAINING_T remaining_result = {TIMER_UNIT_ms, 0};

    SCI_ASSERT (timer_id < TIMER_MAX);/*assert verified*/

    TIMER_HAL_Open (timer_id);

    TIMER_HAL_IOctl (timer_id, TIMER_IOCTL_CMD_GET_REMAINING, (void *) &remaining_result);

    return (remaining_result.unit_number);
}

/*****************************************************************************/
//  Description:    Get the number of ticks that have elapsed since the
//                  system was started.
//  Author:         Lin.Liu
//  Note:
/*****************************************************************************/
PUBLIC uint32 TIMER_GetTickCount (void) // return the number of tick count.
{
    return (uint32) (TIMER_GetSystemCounterReg());
}

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif  // End of timer_drv.c
