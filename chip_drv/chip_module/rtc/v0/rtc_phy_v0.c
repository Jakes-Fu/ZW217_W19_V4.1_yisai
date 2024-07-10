/******************************************************************************
 ** File Name:      rtc_phy_v0.c                                                 *
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
#include "sci_types.h"
#include "os_api.h"
#include "chip_plf_export.h"
#include "rtc_reg_v0.h"
#include "rtc_drvapi.h"
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

#ifdef RTCDRV_DEBUG
#define RTCDRV_PRINT(log)   SCI_TRACE_LOW log
#else
#define RTCDRV_PRINT(log)      ((void)0)
#endif  // RTCDRV_DEBUG
#define RTC_TIME_OUT  0x64          // Totally 2s at 20ms per count.

/**---------------------------------------------------------------------------*
 **                         Global Variables                                  *
 **---------------------------------------------------------------------------*/
LOCAL TB_RTC_TIME_T s_alarm_reg_backup;
LOCAL uint32        s_enable_update_reg_flag;
LOCAL uint32        s_updated_rtc_time;
LOCAL uint32        s_updated_rtc = 0;


/**---------------------------------------------------------------------------*
 **                     Local Function Prototypes                             *
 **---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description:    This function be called by interrupt handler when is
//                  RTC interrupt.
//  Author:         Xueliang.Wang
//  Note:
/*****************************************************************************/
//@Daniel.Ding,CR:MS00004239,9/2/2003,begin
LOCAL ISR_EXE_T RTC_Handler (uint32 int_num);
LOCAL void RTC_CheckIfRtcUpdating (void);
//added by jiexia.yu 07.09.01
LOCAL void RTC_Wait (uint32 ms);
//Daniel.end.
LOCAL void RTCHisrFunc (uint32 i, void *d);
LOCAL void HandleRTCHisr (int int_type);


#define RTC_DAY_MAX   (65535)
#define RTC_HOUR_MAX  (24)
#define RTC_MIN_MAX   (60)
#define RTC_SEC_MAX   (60)

//added by jiexiayu
#define POLL_INTERVAL_TIME  20      //polling time in millisecond
#define WAIT_INTERVAL_TIME  800      //waiting time in millisecond

#define RTC_UPDATE_TIME_OUT 0xffff

INPUT_BUFFER_INIT (int32,16)



/**---------------------------------------------------------------------------*
 **                         Function Definitions                              *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    Get RTC value with double read.               
//  Author:          Yong.Li
//  Note:
/*****************************************************************************/
LOCAL void _GetRTCValue (    
    TB_RTC_TIME_T *timer_ptr
)
{
    uint32  time_out = 0;
    volatile uint32 sec_count1, min_count1, hour_count1, day_count1;
    volatile uint32 sec_count2, min_count2, hour_count2, day_count2;
    rtc_s    *rtc_ptr = (rtc_s *) (RTC_BASE);
    
    sec_count1 = rtc_ptr->sec_cnt;
    min_count1 = rtc_ptr->min_cnt;
    hour_count1 = rtc_ptr->hour_cnt;
    day_count1 = rtc_ptr->day_cnt;

    //double read and check rtc value
    sec_count2 = rtc_ptr->sec_cnt;
    min_count2 = rtc_ptr->min_cnt;
    hour_count2 = rtc_ptr->hour_cnt;
    day_count2 = rtc_ptr->day_cnt;
    
    while(!((sec_count1 == sec_count2) && (min_count1 == min_count2) && (hour_count1 == hour_count2) && (day_count1 == day_count2)))
    {
        time_out++;
        if(time_out > RTC_UPDATE_TIME_OUT)
        {
            SCI_PASSERT (0, ("RTC read value time out!"));/*assert verified*/
        }
        
        sec_count1 = sec_count2;
        min_count1 = min_count2;
        hour_count1 = hour_count2;
        day_count1 = day_count2;

        //double read and check rtc value
        sec_count2 = rtc_ptr->sec_cnt;
        min_count2 = rtc_ptr->min_cnt;
        hour_count2 = rtc_ptr->hour_cnt;
        day_count2 = rtc_ptr->day_cnt;
    }

    timer_ptr->sec = (uint8)(sec_count1 & 0x3F);
    timer_ptr->min = (uint8)(min_count1 & 0x3F);
    timer_ptr->hour = (uint8)(hour_count1 & 0x1F);
    timer_ptr->day = (uint16)(day_count1 & 0xFFFF);    
}

/*****************************************************************************/
//  Description:    Init RTC device. All interrupts of RTC was disabled.
//  Author:         Lin.Liu
//  Note:           If initialize successfully, return TB_SUCCESS;
//                  else return error code.
/*****************************************************************************/
PUBLIC uint32 RTC_Init (void)
{
    uint32          status    = TB_SUCCESS;
    rtc_s           *rtc_ptr  = (rtc_s *) (RTC_BASE);

    s_updated_rtc_time = 0;
    // Disable RTC INT.
    CHIPDRV_DisableIRQINT (TB_RTC_INT);

    // Clear INT status.
    rtc_ptr->iclr   = 0xFFFF;

    s_enable_update_reg_flag = (RTCCTL_SEC_IE | RTCCTL_MIN_IE | RTCCTL_HOUR_IE | RTCCTL_DAY_IE | RTCCTL_ALARM_IE);
    // Set alarm time.
    rtc_ptr->ctl = RTCCTL_MIN_IE|RTCCTL_SEC_ACK_IE|RTCCTL_MIN_ACK_IE|RTCCTL_HOUR_ACK_IE|RTCCTL_DAY_ACK_IE|
                   RTCCTL_SEC_ALM_ACK_IE|RTCCTL_MIN_ALM_ACK_IE|RTCCTL_HOUR_ALM_ACK_IE|RTCCTL_DAY_ALM_ACK_IE;
    // Register ISR of RTC.
    status = ISR_RegHandler_Ex (TB_RTC_INT, (TB_ISR) RTC_Handler, RTCHisrFunc, CHIPDRV_HISR_PRIO_2, NULL);

    // If register successfully, Enable RTC interrupt.
    if (TB_SUCCESS == status)
    {
        // Enable RTC interrupt.
        CHIPDRV_EnableIRQINT (TB_RTC_INT);
    }


    return status;
}

/*****************************************************************************/
//  Description:    This function be called by interrupt handler when is
//                  RTC interrupt.
//  Author:         Xueliang.Wang
//  Note:
/*****************************************************************************/
LOCAL ISR_EXE_T RTC_Handler (uint32 int_num)
{
    uint32  status;
    rtc_s   *rtc_ptr  = (rtc_s *) (RTC_BASE);
    //    TB_MSG  rtc_msg;
    ISR_EXE_T result = ISR_DONE;

    // Get status of RTC INT.
    status = rtc_ptr->sts & rtc_ptr->ctl;

    if (status & RTCSTS_DAY)
    {
        IsrWriteBuffer (TB_RTC_INT_DAY);
        result = CALL_HISR;
    }

    if (status & RTCSTS_MIN)
    {
        IsrWriteBuffer (TB_RTC_INT_MIN);
        result = CALL_HISR;
    }

    if (status & RTCSTS_ALARM)
    {
        // Disable Alarm INT.
        rtc_ptr->ctl &= ~ (RTCCTL_ALARM_IE);

        IsrWriteBuffer (TB_RTC_INT_ALARM);
        result = CALL_HISR;
    }

    if (status & RTCSTS_SEC)
    {

    }

    if (status & (RTCSTS_SEC_ACK|RTCSTS_MIN_ACK|RTCSTS_HOUR_ACK|RTCSTS_DAY_ACK|
                  RTCSTS_SEC_ALM_ACK|RTCSTS_MIN_ALM_ACK|RTCSTS_HOUR_ALM_ACK|RTCSTS_DAY_ALM_ACK))
    {
        s_updated_rtc &= ~status;
    }

    // Clear INT status.
    rtc_ptr->iclr = status;
    return result;
}

/*****************************************************************************/
//  Description:    This function be called by Hisr handler when is
//                  RTC interrupt.
//  Author:         Mingwei.Zhang
//  Note:
/*****************************************************************************/
LOCAL void RTCHisrFunc (uint32 i, void *d)
{
    int32  int_type = 0;

    while (!threadReadBuffer (&int_type))
    {
        HandleRTCHisr (int_type);
    }
}

/*****************************************************************************/
//  Description:    This function be called by RTCHisrFunc
//  Author:         Mingwei.Zhang
//  Note:
/*****************************************************************************/
LOCAL void HandleRTCHisr (int int_type)
{
    TB_MSG  rtc_msg;

    if ( (int_type != TB_RTC_INT_MIN) && (int_type != TB_RTC_INT_ALARM)
            && (int_type != TB_RTC_INT_DAY))
    {
        SCI_PASSERT (SCI_FALSE, ("Error rtc irq")); /*assert verified*/
    }

    rtc_msg.message = int_type;
    ISR_Callback (TB_RTC_INT, &rtc_msg);
    return ;
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
    RTCDRV_PRINT ( ("RTC_Set: Day = %2d, %02d:%02d:%02d", time.day, time.hour, time.min, time.sec));
    // Check if the time values range is valid.

    RTC_CheckIfRtcUpdating();

    if ( (time.sec < RTC_SEC_MAX) && (time.min < RTC_MIN_MAX)
            && (time.hour < RTC_HOUR_MAX) && (time.day < RTC_DAY_MAX))
    {
        // Set time value to RTC register.
        rtc_s   *rtc_ptr  = (rtc_s *) (RTC_BASE);

        while ( (s_enable_update_reg_flag & (RTCCTL_SEC_IE|RTCCTL_MIN_IE|RTCCTL_HOUR_IE|RTCCTL_DAY_IE))
                != (RTCCTL_SEC_IE|RTCCTL_MIN_IE|RTCCTL_HOUR_IE|RTCCTL_DAY_IE))
        {
            /* @CR88036 jiexiayu */
            //we replace this sentence because RTOS task scheduler may not work at this time
            //SCI_Sleep(20);
            RTC_Wait (POLL_INTERVAL_TIME);
            /* @CR88036 end*/
        }

        SCI_DisableIRQ();
        s_enable_update_reg_flag &= ~ (RTCCTL_SEC_IE|RTCCTL_MIN_IE|RTCCTL_HOUR_IE|RTCCTL_DAY_IE);
        // Disable RTC MIN_INT.
        /* @Xueliang.Wang Modified; CR10690; (2004-07-16) */
        //rtc_ptr->ctl &= ~(RTCCTL_DAY_IE | RTCCTL_MIN_IE);
        rtc_ptr->ctl &= ~RTCCTL_MIN_IE;

        // Set new time.
        rtc_ptr->sec_update  = (uint32) time.sec;
        rtc_ptr->min_update  = (uint32) time.min;
        rtc_ptr->hour_update = (uint32) time.hour;
        rtc_ptr->day_update  = (uint32) time.day;
        s_updated_rtc |= RTCSTS_SEC_ACK|RTCSTS_MIN_ACK|RTCSTS_HOUR_ACK|RTCSTS_DAY_ACK;
        SCI_RestoreIRQ();

        s_updated_rtc_time = SCI_GetTickCount();

        SCI_DisableIRQ();
        s_enable_update_reg_flag |= (RTCCTL_SEC_IE|RTCCTL_MIN_IE|RTCCTL_HOUR_IE|RTCCTL_DAY_IE);
        // Enable RTC MIN_INT.
        /* @Xueliang.Wang Modified; CR10690; (2004-07-16) */
        //rtc_ptr->ctl |= (RTCCTL_DAY_IE | RTCCTL_MIN_IE);
        rtc_ptr->ctl |= RTCCTL_MIN_IE;
        SCI_RestoreIRQ();

        return TB_SUCCESS;
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

    if (day <= RTC_DAY_MAX)
    {
        // Set Day value to RTC register.

        rtc_s   *rtc_ptr = (rtc_s *) (RTC_BASE);

        RTC_CheckIfRtcUpdating();

        while ( (s_enable_update_reg_flag & RTCCTL_DAY_IE) != RTCCTL_DAY_IE)
        {
            /* @CR88036 jiexiayu */
            //we replace this sentence because RTOS task scheduler may not work at this time
            //SCI_Sleep(20);
            RTC_Wait (POLL_INTERVAL_TIME);
            /* @CR88036 end*/
        }

        SCI_DisableIRQ();
        s_enable_update_reg_flag &= ~ (RTCCTL_DAY_IE);
        // Disable RTC DAY_INT.
        /* @Xueliang.Wang Modified; CR10690; (2004-07-16) */
        //rtc_ptr->ctl &= ~(RTCCTL_DAY_IE);

        // Set new time.
        rtc_ptr->day_update  = day;
        s_updated_rtc |= RTCSTS_DAY_ACK;
        SCI_RestoreIRQ();

        /*WaitAfterSetRtc();
        {
            if (day != rtc_ptr->day_cnt)
            {
                SCI_ASSERT(0);
            }
        }*/
        s_updated_rtc_time = SCI_GetTickCount();

        SCI_DisableIRQ();
        s_enable_update_reg_flag |= (RTCCTL_DAY_IE);
        // Enable RTC DAY_INT.
        /* @Xueliang.Wang Modified; CR10690; (2004-07-16) */
        //rtc_ptr->ctl |= (RTCCTL_DAY_IE);
        SCI_RestoreIRQ();

        return TB_SUCCESS;
    }
    else
    {
        return TB_RTC_TIME_RANGE_ERROR;
    }

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
    //c_s    *rtc_ptr = (rtc_s *) (RTC_BASE);
    uint32 sys_cnt = 0;     //over timer cnt
    SCI_ASSERT (TB_NULL != time_ptr);/*assert verified*/

    RTC_CheckIfRtcUpdating();

    //Judge if RTC reset
    if ( (REG32 (GR_HWRST) & ( (uint32) HWRST_RTC_BLIEVABLE)) == ( (uint32) HWRST_RTC_BLIEVABLE)) //bit16=0, bit17~bit23 are all "1", rtc beliavlable
    {
        //Save time in struct.
        _GetRTCValue(time_ptr);
    }
    else
    {
        //Return invalid time setting
        time_ptr->sec   = 0x3F;
        time_ptr->min   = 0x3F;
        time_ptr->hour  = 0x1F;
        time_ptr->day   = 0xFFFF;
        //Set RTC reset bit
        CHIP_REG_OR (GR_HWRST, HWRST_RTC_SET);   //will set HWRST bit[23:16]
        sys_cnt = TIMER_GetSystemCounterReg();

        while ( (* (volatile uint32 *) (GR_HWRST) & ( (uint32) (HWRST_RTC_POWER_UP|HWRST_RTC_BLIEVABLE))) != ( (uint32) (HWRST_RTC_POWER_UP|HWRST_RTC_BLIEVABLE)))
        {
            if (TIMER_GetSystemCounterReg() - sys_cnt > 5)  //over timer
            {
                SCI_ASSERT (0);/*assert verified*/
            }
        }

        CHIP_REG_AND (GR_HWRST,~HWRST_RTC_SET);
    }

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

    // Check if the time values range is valid.
    if ( (time.sec < RTC_SEC_MAX) && (time.min < RTC_MIN_MAX)
            && (time.hour < RTC_HOUR_MAX) && (time.day < RTC_DAY_MAX))
    {
        // Set time value to Alarm register.
        rtc_s   *rtc_ptr  = (rtc_s *) (RTC_BASE);

        RTC_CheckIfRtcUpdating();

        while ( (s_enable_update_reg_flag & RTCCTL_ALARM_IE) != RTCCTL_ALARM_IE)
        {
            /* @CR88036 jiexiayu */
            //we replace this sentence because RTOS task scheduler may not work at this time
            //SCI_Sleep(20);
            RTC_Wait (POLL_INTERVAL_TIME);
            /* @CR88036 end*/
        }

        SCI_DisableIRQ();
        s_enable_update_reg_flag &= ~ (RTCCTL_ALARM_IE);

        // Disable Alarm INT.
        rtc_ptr->ctl &= ~ (RTCCTL_ALARM_IE);

        // Set new time.
        rtc_ptr->sec_alm  = (uint32) time.sec;
        rtc_ptr->min_alm  = (uint32) time.min;
        rtc_ptr->hour_alm = (uint32) time.hour;
        rtc_ptr->day_alm  = (uint32) time.day;
        s_updated_rtc |= RTCSTS_SEC_ALM_ACK|RTCSTS_MIN_ALM_ACK|RTCSTS_HOUR_ALM_ACK|RTCSTS_DAY_ALM_ACK;
        // Save to local vars.
        s_alarm_reg_backup.sec  = (uint32) time.sec;
        s_alarm_reg_backup.min  = (uint32) time.min;
        s_alarm_reg_backup.hour = (uint32) time.hour;
        s_alarm_reg_backup.day  = (uint32) time.day;
        SCI_RestoreIRQ();

        //WaitAfterSetRtc();
        s_updated_rtc_time = SCI_GetTickCount();

        SCI_DisableIRQ();
        s_enable_update_reg_flag |= (RTCCTL_ALARM_IE);

        // Enable Alarm INT.
        rtc_ptr->ctl |= (RTCCTL_ALARM_IE);
        SCI_RestoreIRQ();

        return TB_SUCCESS;
    }
    else
    {
        return TB_RTC_TIME_RANGE_ERROR;
    }

}

/*****************************************************************************/
//  Description:    Get Alarm time.
//  Author:         Xueliang.Wang
//  Note:
/*****************************************************************************/
PUBLIC void RTC_DisableAlarm (void)
{
    rtc_s            *rtc_ptr = (rtc_s *) (RTC_BASE);

    RTCDRV_PRINT ( ("RTC_DisableAlarm: Disable !"));

    RTC_CheckIfRtcUpdating();

    while ( (s_enable_update_reg_flag & RTCCTL_ALARM_IE) != RTCCTL_ALARM_IE)
    {
        /* @CR88036 jiexiayu */
        //we replace this sentence because RTOS task scheduler may not work at this time
        //SCI_Sleep(20);
        RTC_Wait (POLL_INTERVAL_TIME);
        /* @CR88036 end*/
    }

    SCI_DisableIRQ();
    s_enable_update_reg_flag &= ~ (RTCCTL_ALARM_IE);

    // Disable Alarm INT.
    rtc_ptr->ctl &= ~ (RTCCTL_ALARM_IE);

    // Save time in struct.
    rtc_ptr->sec_alm    = 1;
    rtc_ptr->min_alm    = 1;
    rtc_ptr->hour_alm   = 1;
    rtc_ptr->day_alm    = 31;
    s_updated_rtc |= RTCSTS_SEC_ALM_ACK|RTCSTS_MIN_ALM_ACK|RTCSTS_HOUR_ALM_ACK|RTCSTS_DAY_ALM_ACK;
    s_alarm_reg_backup.sec  = 1;
    s_alarm_reg_backup.min  = 1;
    s_alarm_reg_backup.hour = 1;
    s_alarm_reg_backup.day  = 31;
    SCI_RestoreIRQ();

    //WaitAfterSetRtc();
    s_updated_rtc_time = SCI_GetTickCount();

    SCI_DisableIRQ();
    s_enable_update_reg_flag |= (RTCCTL_ALARM_IE);
    SCI_RestoreIRQ();

}

/*****************************************************************************/
//  Description:    Set Rtc interrupt Type.
//  Author:         Weihua.Wang
//  Note:
/*****************************************************************************/
PUBLIC uint32 RTC_SetRtcInt (RTC_INTTYPE_E type, BOOLEAN value)
{
    rtc_s      *rtc_ptr = (rtc_s *) (RTC_BASE);
    uint32    rtc_int = 0;

    if ( (type >= RTC_INT_MAX) || ( (value != TRUE) && (value != FALSE)))
    {
        return TB_PARAM_ERROR;
    }

    RTC_CheckIfRtcUpdating();

    //wait for the signal
    while ( (s_enable_update_reg_flag & (RTCCTL_SEC_IE|RTCCTL_MIN_IE|RTCCTL_HOUR_IE|RTCCTL_DAY_IE))
            != (RTCCTL_SEC_IE|RTCCTL_MIN_IE|RTCCTL_HOUR_IE|RTCCTL_DAY_IE))
    {
        /* @CR88036 jiexiayu */
        //we replace this sentence because RTOS task scheduler may not work at this time
        //SCI_Sleep(20);
        RTC_Wait (POLL_INTERVAL_TIME);
        /* @CR88036 end*/
    }

    SCI_DisableIRQ();

    rtc_int = 0x0;
    rtc_int = (1<<type) &0x1f;
    s_enable_update_reg_flag &= ~ (rtc_int);

    if (value)
    {
        // enable Alarm INT.
        rtc_ptr->ctl |= (rtc_int);
    }
    else
    {
        // Disable Alarm INT.
        rtc_ptr->ctl &= ~ (rtc_int);
    }

    SCI_RestoreIRQ();

    //WaitAfterSetRtc();
    s_updated_rtc_time = SCI_GetTickCount();

    SCI_DisableIRQ();
    s_enable_update_reg_flag |= (rtc_int);
    SCI_RestoreIRQ();

    RTCDRV_PRINT ( ("RTC_SetRtcInt: type = %d, value = %d", type,value));

    return TB_SUCCESS;
}

/*****************************************************************************/
// Description :    Check if RTC is update before,if it isnt long enough,wait it
// Global resource dependence :
// Author :         hanjun.liu
// Note :
/*****************************************************************************/
LOCAL void RTC_CheckIfRtcUpdating (void)
{
    //SCI_TRACE_LOW("RTC_CheckIfRtcUpdating:cur_time=%d",cur_time);
    uint32  time_out =0;

    while (s_updated_rtc)
    {
        SCI_Sleep (20);
        time_out ++;

        if (time_out > RTC_TIME_OUT)
        {
            SCI_PASSERT (0, ("RTC_CheckIfRtcUpdating time out!"));/*assert verified*/
        }
    }
}

/*******************************************************************************/
// Description: when a thread is executing and system is not in assert mode, we invoke
//              OS suspend function for sleep, otherwise, we poll system counter value for delay
// Global resource dependence: none
// Author: jiexia.yu
// Note:
/********************************************************************************/
LOCAL void RTC_Wait (uint32 ms)
{
    uint32 tick_start=SCI_GetTickCount();

    if (SCI_InThreadContext())
    {
        SCI_Sleep (ms);
    }
    else
    {
        while ( (SCI_GetTickCount() - tick_start) < ms)
        {
            ;/* loop */
        }

    }
}

/*******************************************************************************/
// Description: This function used to clear alarm int status value
// Global resource dependence: none
// Author: yuhua.shi
// Note:
/********************************************************************************/
PUBLIC void RTC_ClearAlarmInt (void)
{
    //clear alarm int status value
    REG32 (RTC_ICLR) |= RTCCTL_ALARM_IE;
}

/*below 1 functions are removed from driver_export.c, wait clean up*/
PUBLIC uint32 CheckRTCState (void)
{
    return CHIP_REG_GET (RTC_STS);
}
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif  // End of rtc_drv.c
