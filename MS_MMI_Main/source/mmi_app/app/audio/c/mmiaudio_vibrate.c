#if 0
/****************************************************************************
** File Name:      mmiaudio_vibrate.c                                       *
** Author:                                                                  *
** Date:           01/06/2010                                               *
** Copyright:      2003 Spreadtrum, Incorporated. All Rights Reserved.      *
** Description:    the file define and handle vibrate                       *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 06/2010        xingdong.li      Create
******************************************************************************/

#define _MMIAUDIO_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/

#ifdef WIN32
#include "std_header.h"
#endif
#include "mmiaudio.h"
#include "mmiaudio_ctrl.h"
#include "gpio_prod_api.h"
#include "mmk_app.h"
#include "mmk_timer.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/

LOCAL uint8 s_vibrate_timer_id = 0;
LOCAL BOOLEAN s_vibrate_is_period = FALSE;  //振动是否是周期性的振动
LOCAL BOOLEAN s_is_vibrating = FALSE;   //是否正在振动
LOCAL uint32 s_vibrate_duration = 0;    //振动周期
LOCAL MMIAUDIO_VIBRATE_CALLBACK s_vibrate_callback = PNULL;

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : start vibrator
//  Global resource dependence :
//  Author: bruce.chi
//  Note: when is_period seting TRUE, u must call MMIAUDIO_StopVibrate to stop vibrate
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUDIO_Vibrate(uint32 duaration, BOOLEAN is_period, MMIAUDIO_VIBRATE_CALLBACK callback)
{
    if (0 == duaration)
    {
        return FALSE;
    }

    GPIO_SetVibrator(TRUE);
    s_is_vibrating = TRUE;

    if (s_vibrate_timer_id == 0)
    {
        s_vibrate_timer_id = MMK_CreateTimerCallback(duaration, MMIAUDIO_HandleVibratorTimer, NULL, FALSE);
    }
    else
    {
        MMK_StopTimer(s_vibrate_timer_id);
        s_vibrate_timer_id = MMK_CreateTimerCallback(duaration, MMIAUDIO_HandleVibratorTimer, NULL, FALSE);
    }

    s_vibrate_is_period = is_period;
    s_vibrate_duration = duaration;
    s_vibrate_callback = callback;

//  if (0 != s_vibrate_timer_id)
//  {
//      s_is_vibrating = TRUE;
//  }

    return TRUE;
}

/*****************************************************************************/
//  Description : handle vibrtor timer over message
//  Global resource dependence :
//  Author: bruce.chi
//  Note:
/*****************************************************************************/
PUBLIC void MMIAUDIO_HandleVibratorTimer( uint8 timer_id, uint32 param)
{
    if (s_vibrate_timer_id == timer_id)
    {
        s_vibrate_timer_id = 0;
        if (s_vibrate_is_period)
        {
            s_vibrate_timer_id = MMK_CreateTimerCallback(s_vibrate_duration, MMIAUDIO_HandleVibratorTimer, NULL, FALSE);
            if (!s_is_vibrating)
            {
                GPIO_SetVibrator(TRUE);
                s_is_vibrating = TRUE;
            }
            else
            {
                GPIO_SetVibrator(FALSE);
                s_is_vibrating = FALSE;                
            }
        }
        else
        {
            GPIO_SetVibrator(FALSE);
            s_is_vibrating = FALSE;  
            if(PNULL != s_vibrate_callback)
            {
                s_vibrate_callback();
                s_vibrate_callback = PNULL;
            }
        }
    }
}

/*****************************************************************************/
//  Description : stop vibrate
//  Global resource dependence :
//  Author: bruce.chi
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUDIO_StopVibrate(void)
{
//    if (0 != s_vibrate_timer_id)
    {
        GPIO_SetVibrator(FALSE);
        s_is_vibrating = FALSE;

        if(s_vibrate_timer_id != 0)     //MS177479
        {
            MMK_StopTimer(s_vibrate_timer_id);
            s_vibrate_timer_id = 0;
        }
        s_vibrate_is_period = FALSE;
        s_vibrate_duration = 0;
        s_vibrate_callback = PNULL;
//      s_is_vibrating = FALSE;
        return TRUE;
    }
//    else
//    {
//        return FALSE;
//    }
}
#endif
