/******************************************************************************
 ** File Name:      rtc_drv.c                                                 *
 ** Author:         Xueliang.Wang                                             *
 ** DATE:           10/10/2002                                                *
 ** Copyright:      2002 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the basic operation interfaces of       *
 **                 Real Timer Counter.                                       *
 **                 Alarm also defined here.                                  *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 10/10/2002     Xueliang.Wang    Create.                                   *
 ** 18/08/2003     Lin.liu          Use second int for watchdog.              *
 ** 28/09/2004     Jimmy.Jia        rewrite for sc6600d.                      *
 ** 29/08/2005     weihua.wang      modify for  CR29225                       *
 ** 01/09/2007     jiexia.yu            modify for CR88036
 ******************************************************************************/


/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "tb_hal.h"
#include "rtc_drvapi.h"

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

#ifdef RTCDRV_DEBUG
#define RTCDRV_PRINT(log)   SCI_TRACE_LOW log
#else
#define RTCDRV_PRINT(log)      ((void)0)
#endif  // RTCDRV_DEBUG


/**---------------------------------------------------------------------------*
 **                         Global Variables                                  *
 **---------------------------------------------------------------------------*/


/**---------------------------------------------------------------------------*
 **                     Local Function Prototypes                             *
 **---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description:    This function be called by interrupt handler when is
//                  RTC interrupt.
//  Author:         Xueliang.Wang
//  Note:
/*****************************************************************************/


#define RTC_DAY_MAX   (65535)
#define RTC_HOUR_MAX  (24)
#define RTC_MIN_MAX   (60)
#define RTC_SEC_MAX   (60)

#ifdef WIN32
#define X_RTC_SIMULATE
#endif

#ifdef X_RTC_SIMULATE
#define RTC_INTERNAL 20000
LOCAL BOOLEAN        s_rtc_init =  FALSE;
LOCAL BOOLEAN        s_rtc_alarm_enable=FALSE;
LOCAL TB_RTC_TIME_T  s_rtc_alarm;
LOCAL TB_RTC_TIME_T  s_rtc_time;
LOCAL TB_CALLBACK    s_rtc_callback = NULL;
LOCAL void RTCHisrFunc (uint32 i);
LOCAL void HandleRTCHisr (int int_type);
#endif


/**---------------------------------------------------------------------------*
 **                         Function Definitions                              *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    Init RTC device. All interrupts of RTC was disabled.
//  Author:         Lin.Liu
//  Note:           If initialize successfully, return TB_SUCCESS;
//                  else return error code.
/*****************************************************************************/
PUBLIC uint32 RTC_Init (void)
{
#ifdef X_RTC_SIMULATE
	SYSTEMTIME system_time;
	GetLocalTime (&system_time);
	s_rtc_time.sec  = system_time.wSecond;
	s_rtc_time.min  = system_time.wMinute;
	s_rtc_time.hour = system_time.wHour;
	s_rtc_time.day  = system_time.wDay;	
	s_rtc_alarm_enable = FALSE;
	s_rtc_init = TRUE;
	SCI_CreatePeriodTimer("RTC_INT", RTCHisrFunc, 0, RTC_INTERNAL, 1);
#endif //RTC_SIMULATE
    return 0;
}

/*****************************************************************************/
//  Description:    Set RTC value.
//                  This function check the parameter first.
//                  If it is valid, set the time value to RTC register;
//                  Else return error code.
//  Author:         Xueliang.Wang
//  Note:
/*****************************************************************************/
PUBLIC uint32 RTC_Set ( // If set successfully, return TB_SUCCESS;
    // else return error code.
    TB_RTC_TIME_T time  // Time value to be set to RTC register.
)
{	
    SYSTEMTIME system_time;
	WORD status;    	

    // Check if the time values range is valid.
    if ( (time.sec < RTC_SEC_MAX) && (time.min < RTC_MIN_MAX)
            && (time.hour < RTC_HOUR_MAX) && (time.day <= RTC_DAY_MAX))
    {
        // Set new time.
#ifdef X_RTC_SIMULATE
		s_rtc_time.sec  = (WORD) time.sec;
		s_rtc_time.min  = (WORD) time.min;
		s_rtc_time.hour = (WORD) time.hour;
		s_rtc_time.day  = (WORD) time.day;
		status = TRUE;
#else
		GetLocalTime (&system_time);
		system_time.wSecond  = (WORD) time.sec;
		system_time.wMinute  = (WORD) time.min;
		system_time.wHour = (WORD) time.hour;
		system_time.wDay  = (WORD) time.day;
		status = SetLocalTime (&system_time);
#endif //X_RTC_SIMULATE

        if (status)
        {
            return TB_SUCCESS;
        }
        else
        {
            return TB_NULL;
        }
    }
    else
    {
        return TB_RTC_TIME_RANGE_ERROR;
    }

}

/*****************************************************************************/
//  Description:    Set RTC's day value.
//                  This function check the parameter first.
//                  If it is valid, set the time value to RTC register;
//                  Else return error code.
//  Author:         Eric.zhou
//  Note:
/*****************************************************************************/
PUBLIC uint32 RTC_SetDay (  // If set successfully, return TB_SUCCESS;
    // else return error code.
    uint32  day     // Day value to be set.
)
{
    RTCDRV_PRINT ( ("RTC_SetDay: Day = %d", day));
    return TB_SUCCESS;
}

/*****************************************************************************/
//  Description:    Get RTC value.
//  Author:         Xueliang.Wang
//  Note:
/*****************************************************************************/
PUBLIC void RTC_Get (
    TB_RTC_TIME_T *time_ptr // Pointer to a TB_RTC_TIME_T struct,
    // used to save RTC value.
)
{
#ifndef X_RTC_SIMULATE
	SYSTEMTIME system_time;
	GetLocalTime (&system_time);
	time_ptr->sec   = (uint8) (system_time.wSecond & 0x3F);
	time_ptr->min   = (uint8) (system_time.wMinute & 0x3F);
	time_ptr->day   = (uint16) (system_time.wDay & 0xFFFF);
	time_ptr->hour  = (uint8) (system_time.wHour &  0x1F);
#else
	time_ptr->sec   = s_rtc_time.sec;
    time_ptr->min   = s_rtc_time.min;
    time_ptr->day   = s_rtc_time.day;
    time_ptr->hour  = s_rtc_time.hour;
#endif // X_RTC_SIMULATE

    RTCDRV_PRINT ( ("RTC_Get: Day = %2d, %02d:%02d:%02d", time_ptr->day, time_ptr->hour, time_ptr->min, time_ptr->sec));
}


/*****************************************************************************/
//  Description:    Set ALARM time.
//                  This function check the parameter first.
//                  If it is valid, set the time value to alarm register;
//                  Else return error code.
//  Author:         Xueliang.Wang
//  Note:
/*****************************************************************************/
PUBLIC uint32 RTC_SetAlarm (// If set successfully, return TB_SUCCESS;
    // else return error code.
    TB_RTC_TIME_T time      // Time value to be set to alarm register.
)
{
    RTCDRV_PRINT ( ("RTC_SetAlarm: Day = %2d, %02d:%02d:%02d", time.day, time.hour, time.min, time.sec));

#ifdef X_RTC_SIMULATE
	// Check if the time values range is valid.
    if ( (time.sec < RTC_SEC_MAX) && (time.min < RTC_MIN_MAX)
		&& (time.hour < RTC_HOUR_MAX) && (time.day < RTC_DAY_MAX))
    {
		s_rtc_alarm.day  = time.day;
		s_rtc_alarm.hour = time.hour;
		s_rtc_alarm.min  = time.min;
		s_rtc_alarm.sec  = time.sec;   
		
		s_rtc_alarm_enable = TRUE;
		
        return TB_SUCCESS;
    }
    else
    {
        return TB_RTC_TIME_RANGE_ERROR;
    }
#else
	return TB_SUCCESS;
#endif // X_RTC_SIMULATE
}

/*****************************************************************************/
//  Description:    Get Alarm time.
//  Author:         Xueliang.Wang
//  Note:
/*****************************************************************************/
PUBLIC void RTC_DisableAlarm (void)
{
#ifdef X_RTC_SIMULATE
	s_rtc_alarm_enable = FALSE;
#endif // X_RTC_SIMULATE
}

/*****************************************************************************/
//  Description:    Set Rtc interrupt Type.
//  Author:         Weihua.Wang
//  Note:
/*****************************************************************************/
PUBLIC uint32 RTC_SetRtcInt (RTC_INTTYPE_E type, BOOLEAN value)
{
    return TB_SUCCESS;
}


/*******************************************************************************/
// Description: This function used to clear alarm int status value
// Global resource dependence: none
// Author: yuhua.shi
// Note:
/********************************************************************************/
PUBLIC void RTC_ClearAlarmInt (void)
{

}

#ifdef X_RTC_SIMULATE
LOCAL void RTCHisrFunc (uint32 i)
{
	s_rtc_time.sec += RTC_INTERNAL;
	if(s_rtc_time.sec >= 60)
	{
		s_rtc_time.sec = 0;
		s_rtc_time.min += 1;
		HandleRTCHisr(TB_RTC_INT_MIN);
		if(s_rtc_time.min >= 60)
		{
			s_rtc_time.min = 0;
			s_rtc_time.hour += 1;
			HandleRTCHisr(TB_RTC_INT_HOUR);

			if(s_rtc_time.hour >= 24)
			{
				HandleRTCHisr(TB_RTC_INT_DAY);
				s_rtc_time.hour = 0;
				s_rtc_time.day += 1;
			}
		}
	}

	if( s_rtc_alarm_enable && 
		/*s_rtc_time.day == s_rtc_alarm.day && */ 
		s_rtc_time.hour == s_rtc_alarm.hour &&
		s_rtc_time.min  == s_rtc_alarm.min)
	{
		HandleRTCHisr(TB_RTC_INT_ALARM);
	}

}

LOCAL void HandleRTCHisr (int int_type)
{
    TB_MSG  rtc_msg;
	
    if ( (int_type != TB_RTC_INT_MIN) && (int_type != TB_RTC_INT_ALARM)
		&& (int_type != TB_RTC_INT_DAY) && (int_type != TB_RTC_INT_HOUR))
    {
        SCI_PASSERT (SCI_FALSE, ("Error rtc irq")); /*assert verified*/
    }
	
    rtc_msg.message = int_type;
    ISR_Callback (TB_RTC_INT,&rtc_msg);
    return ;
}

PUBLIC uint32 ISR_RegCallback(
							  uint32      int_num,    // Interrupt number
							  TB_CALLBACK handler     // Callback handler
)
{
	if(int_num != TB_RTC_INT)
	{
		return TB_INT_ERROR;
	}
	if(handler == NULL)
	{
		return TB_PTR_ERROR;
	}

	s_rtc_callback = handler;

	return TB_SUCCESS;
}

PUBLIC uint32 ISR_Callback(
						   uint32 int_num, // Interrupt number
						   TB_MSG *msg     // Message pointer
)
{
	if(msg == NULL)
	{
		return TB_PTR_ERROR;
	}
	if(s_rtc_callback)
	{
		 s_rtc_callback (msg);
	}
	else
	{
		return TB_NO_CALLBACK;
	}

	return TB_SUCCESS;
}
#endif // X_RTC_SIMULATE



/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif  // End of rtc_drv.c

