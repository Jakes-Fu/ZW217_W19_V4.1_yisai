/******************************************************************************
 ** File Name:      rtc_phy_v5.c                                              *
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
 ** 01/09/2007     jiexia.yu        modify for CR88036                        *
 ** 17/06/2010     Mingwei.Zhang    modify for SC8800G                        *
 ******************************************************************************/


/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "os_api.h"
#include "chip_plf_export.h"
#include "adi_hal_internal.h"
#include "task_monitor.h"
#include "rtc_reg_v5.h"
#include "rtc_drvapi.h"
#include "clock_drvapi.h"

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

typedef enum
{
    RTC_ACK_TIME = 0,
    RTC_ACK_ALARM
} RTC_ACK_E;

/**---------------------------------------------------------------------------*
 **                         Global Variables                                  *
 **---------------------------------------------------------------------------*/
LOCAL uint32        s_enable_update_reg_flag;
LOCAL uint32        s_updated_rtc_time;/*lint -esym(551,s_updated_rtc_time)*/  
LOCAL uint32        s_updated_rtc = 0;
LOCAL DEVICE_HANDLE s_dev_rtc = SCI_NULL;

// A local variable to save the RTC time updated latest.
LOCAL TB_RTC_TIME_T s_rtc_time_update;

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
LOCAL BOOLEAN RTC_CheckIfRtcUpdating (RTC_ACK_E rtc_ack, BOOLEAN wait_option);
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

INPUT_BUFFER_INIT (int32,16)



#define CLEAR_RTC_INT(msk) \
    do{ \
        uint32 cnt = 0;\
        SCI_DisableIRQ();\
        SCI_DisableFIQ();\
        ANA_REG_SET(ANA_RTC_INT_CLR, (msk));\
        while((ANA_REG_GET(ANA_RTC_INT_RSTS)&(msk))&&(cnt < 10)){cnt++;}\
        if(cnt >= 10){TM_SendTestPointRequest(0x15170004,cnt);}\
        SCI_RestoreFIQ();\
        SCI_RestoreIRQ();\
    }while(0)


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
    uint32          status    = TB_SUCCESS;
   // uint32	reg_val;

#if defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
    CLOCK_STATUS s = DEVICE_Find ("RTC", &s_dev_rtc);
#else
    CLOCK_STATUS s = DEVICE_Find ("ADC", &s_dev_rtc);
#endif
    SCI_ASSERT (s == CLOCK_SUCCESS); /*assert verified*/

    s_updated_rtc_time = 0;

    s_rtc_time_update.sec = 0;
    s_rtc_time_update.min = 0;
    s_rtc_time_update.hour = 0;
    s_rtc_time_update.day = 0;
	

    // Disable RTC INT.
    ANA_REG_AND (ANA_RTC_INT_EN, ~RTC_INT_ALL_MSK);

    // Enable RTC device.
    DEVICE_Open (s_dev_rtc);

    // Clear INT status.
    CLEAR_RTC_INT (RTC_INT_ALL_MSK);

    s_enable_update_reg_flag = (RTC_SEC_BIT|RTC_MIN_BIT|RTC_HOUR_BIT|RTC_DAY_BIT|RTC_ALARM_BIT);

    //ANA_REG_OR(ANA_RTC_INT_EN, RTC_MIN_INT_BIT);
    // Register ISR of RTC.
    status = ISR_RegHandler_Ex (TB_RTC_INT, (TB_ISR) RTC_Handler, RTCHisrFunc, CHIPDRV_HISR_PRIO_2, NULL);

    // If register successfully, Enable RTC interrupt.
    if (TB_SUCCESS == status)
    {
        // Enable RTC interrupt.
        //CHIPDRV_EnableIRQINT_(TB_RTC_INT);
        ANA_REG_OR (ANA_RTC_INT_EN, RTC_MIN_BIT|
                    RTC_SEC_ACK_BIT|RTC_MIN_ACK_BIT|RTC_HOUR_ACK_BIT|RTC_DAY_ACK_BIT|
                    RTC_SEC_ALM_ACK_BIT|RTC_MIN_ALM_ACK_BIT|RTC_HOUR_ALM_ACK_BIT|RTC_DAY_ALM_ACK_BIT);

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
    uint16  status;
    ISR_EXE_T result = ISR_DONE;

    // Get status of RTC INT.
    //status = rtc_ptr->sts & rtc_ptr->ctl;
    status = ANA_REG_GET (ANA_RTC_INT_RSTS) & ANA_REG_GET (ANA_RTC_INT_EN);

    if (status & RTC_DAY_BIT)
    {
        IsrWriteBuffer (TB_RTC_INT_DAY);
        result = CALL_HISR;
    }

    if (status & RTC_MIN_BIT)
    {
        IsrWriteBuffer (TB_RTC_INT_MIN);
        result = CALL_HISR;
    }

    if (status & RTC_ALARM_BIT)
    {
        // Handle Alarm.
        // Disable Alarm INT.
        ANA_REG_AND (ANA_RTC_INT_EN, ~RTC_ALARM_BIT);
        IsrWriteBuffer (TB_RTC_INT_ALARM);
        result = CALL_HISR;
    }

    if (status & RTC_SEC_BIT)
    {

    }

    if (status & (RTC_SEC_ACK_BIT|RTC_MIN_ACK_BIT|RTC_HOUR_ACK_BIT|RTC_DAY_ACK_BIT|
                  RTC_SEC_ALM_ACK_BIT|RTC_MIN_ALM_ACK_BIT|RTC_HOUR_ALM_ACK_BIT|RTC_DAY_ALM_ACK_BIT))
    {
        s_updated_rtc &= ~status;
    }

    // Clear INT status.
    CLEAR_RTC_INT (status);

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

    RTC_CheckIfRtcUpdating (RTC_ACK_TIME, 1);

    if ( (time.sec < RTC_SEC_MAX) && (time.min < RTC_MIN_MAX)
            && (time.hour < RTC_HOUR_MAX) && (time.day < RTC_DAY_MAX))
    {
        // Set time value to RTC register.
        while ( (s_enable_update_reg_flag & (RTC_SEC_BIT|RTC_MIN_BIT|RTC_HOUR_BIT|RTC_DAY_BIT))
                != (RTC_SEC_BIT|RTC_MIN_BIT|RTC_HOUR_BIT|RTC_DAY_BIT))
        {
            /* @CR88036 jiexiayu */
            //we replace this sentence because RTOS task scheduler may not work at this time
            //SCI_Sleep(20);
            RTC_Wait (POLL_INTERVAL_TIME);
            /* @CR88036 end*/
        }

        SCI_DisableIRQ();
        s_enable_update_reg_flag &= ~ (RTC_SEC_BIT|RTC_MIN_BIT|RTC_HOUR_BIT|RTC_DAY_BIT);
        // Disable RTC MIN_INT.
        /* @Xueliang.Wang Modified; CR10690; (2004-07-16) */
        //rtc_ptr->ctl &= ~(RTCCTL_DAY_IE | RTCCTL_MIN_IE);
        ANA_REG_AND (ANA_RTC_INT_EN, ~RTC_MIN_BIT);

        // Update local RTC time value.
        s_rtc_time_update.sec = time.sec;
        s_rtc_time_update.min = time.min;
        s_rtc_time_update.hour = time.hour;
        s_rtc_time_update.day = time.day;

        // Set new time.
        ANA_REG_SET (ANA_RTC_SEC_UPDATE, (uint16) time.sec);
        ANA_REG_SET (ANA_RTC_MIN_UPDATE, (uint16) time.min);
        ANA_REG_SET (ANA_RTC_HOUR_UPDATE, (uint16) time.hour);
        ANA_REG_SET (ANA_RTC_DAY_UPDATE, (uint16) time.day);

        s_updated_rtc |= RTC_SEC_ACK_BIT|RTC_MIN_ACK_BIT|RTC_HOUR_ACK_BIT|RTC_DAY_ACK_BIT;
        SCI_RestoreIRQ();

        /*WaitAfterSetRtc();
        {
            uint32 rtc_sec;
            uint32 tmp_sec;

            rtc_sec = ((rtc_ptr->day_cnt * RTC_HOUR_MAX + rtc_ptr->hour_cnt) * RTC_MIN_MAX + rtc_ptr->min_cnt) * RTC_SEC_MAX + rtc_ptr->sec_cnt;
            tmp_sec = ((time.day * RTC_HOUR_MAX + time.hour) * RTC_MIN_MAX + time.min) * RTC_SEC_MAX + time.sec;

            if ((rtc_sec < tmp_sec ) || (rtc_sec > (tmp_sec + 3)))
            {
                SCI_ASSERT(0);
            }
        }*/
        s_updated_rtc_time = SCI_GetTickCount();

        SCI_DisableIRQ();
        s_enable_update_reg_flag |= (RTC_SEC_BIT|RTC_MIN_BIT|RTC_HOUR_BIT|RTC_DAY_BIT);
        // Enable RTC MIN_INT.
        /* @Xueliang.Wang Modified; CR10690; (2004-07-16) */
        //rtc_ptr->ctl |= (RTCCTL_DAY_IE | RTCCTL_MIN_IE);
        ///rtc_ptr->ctl |= RTCCTL_MIN_IE;
        ANA_REG_OR (ANA_RTC_INT_EN, RTC_MIN_BIT);
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
    TB_RTC_TIME_T time;
    RTCDRV_PRINT ( ("RTC_SetDay: Day = %d", day));

    if (day <= RTC_DAY_MAX)
    {
        // Set Day value to RTC register.

        RTC_CheckIfRtcUpdating (RTC_ACK_TIME, 1);

        while ( (s_enable_update_reg_flag & RTC_DAY_BIT) != RTC_DAY_BIT)
        {
            /* @CR88036 jiexiayu */
            //we replace this sentence because RTOS task scheduler may not work at this time
            //SCI_Sleep(20);
            RTC_Wait (POLL_INTERVAL_TIME);
            /* @CR88036 end*/
        }

        SCI_DisableIRQ();
        s_enable_update_reg_flag &= ~ (RTC_DAY_BIT);
        // Disable RTC DAY_INT.
        /* @Xueliang.Wang Modified; CR10690; (2004-07-16) */
        //rtc_ptr->ctl &= ~(RTCCTL_DAY_IE);

        RTC_Get (&time);

        // Update local RTC time value.
        s_rtc_time_update.sec = time.sec;
        s_rtc_time_update.min = time.min;
        s_rtc_time_update.hour = time.hour;
        s_rtc_time_update.day = day;

        // Set new time.
        ANA_REG_SET (ANA_RTC_DAY_UPDATE, (uint16) day);

        s_updated_rtc |= RTC_DAY_ACK_BIT;
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
        s_enable_update_reg_flag |= (RTC_DAY_BIT);
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
    SCI_ASSERT (TB_NULL != time_ptr);/*assert verified*/

    // Check whether the RTC updating is done, if not, return the local value
    // saved at the latest RTC_Set/RTC_SetDay function.
    if (RTC_CheckIfRtcUpdating (RTC_ACK_TIME, 0))
    {
        time_ptr->sec   = s_rtc_time_update.sec;
        time_ptr->min   = s_rtc_time_update.min;
        time_ptr->hour  = s_rtc_time_update.hour;
        time_ptr->day   = s_rtc_time_update.day;
    }
    else
    {
        time_ptr->sec   = (uint8) (ANA_REG_GET (ANA_RTC_SEC_CNT) & 0x3F);
        time_ptr->min   = (uint8) (ANA_REG_GET (ANA_RTC_MIN_CNT)  & 0x3F);
        time_ptr->hour  = (uint8) (ANA_REG_GET (ANA_RTC_HOUR_CNT) & 0x1F);
        time_ptr->day   = (uint16) (ANA_REG_GET (ANA_RTC_DAY_CNT)  & 0xFFFF);
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
    uint32 reg_val;	
    RTCDRV_PRINT ( ("RTC_SetAlarm: Day = %2d, %02d:%02d:%02d", time.day, time.hour, time.min, time.sec));

    // Check if the time values range is valid.
    if ( (time.sec < RTC_SEC_MAX) && (time.min < RTC_MIN_MAX)
            && (time.hour < RTC_HOUR_MAX) && (time.day < RTC_DAY_MAX))
    {
        // Set time value to Alarm register.

        RTC_CheckIfRtcUpdating (RTC_ACK_ALARM, 1);

        while ( (s_enable_update_reg_flag & RTC_ALARM_BIT) != RTC_ALARM_BIT)
        {
            /* @CR88036 jiexiayu */
            //we replace this sentence because RTOS task scheduler may not work at this time
            //SCI_Sleep(20);
            RTC_Wait (POLL_INTERVAL_TIME);
            /* @CR88036 end*/
        }

        SCI_DisableIRQ();
        s_enable_update_reg_flag &= ~ (RTC_ALARM_BIT);

        // Disable Alarm INT.
        ANA_REG_AND (ANA_RTC_INT_EN, ~RTC_ALARM_BIT);

        // Set new time.
        ANA_REG_SET (ANA_RTC_SEC_ALM, (uint16) time.sec);
        ANA_REG_SET (ANA_RTC_MIN_ALM, (uint16) time.min);
        ANA_REG_SET (ANA_RTC_HOUR_ALM, (uint16) time.hour);
        ANA_REG_SET (ANA_RTC_DAY_ALM, (uint16) time.day);

        s_updated_rtc |= RTC_SEC_ALM_ACK_BIT|RTC_MIN_ALM_ACK_BIT|RTC_HOUR_ALM_ACK_BIT|RTC_DAY_ALM_ACK_BIT;
        SCI_RestoreIRQ();

        //WaitAfterSetRtc();
        s_updated_rtc_time = SCI_GetTickCount();

        SCI_DisableIRQ();
        s_enable_update_reg_flag |= (RTC_ALARM_BIT);

        // Enable Alarm INT.
        ANA_REG_OR (ANA_RTC_INT_EN, RTC_ALARM_BIT);

       if (CHIP_GetADIEChipID() == CHIP_ID_SR1131BA)
       {	
	       /*unlock the alarm function*/
	    reg_val = ANA_REG_GET(ANA_RTC_SPG_UPD);
	    reg_val &= ~0xff;
	    reg_val |= SPRD_RTC_UNLOCK & 0xff;
	    ANA_REG_SET(ANA_RTC_SPG_UPD, reg_val);		   
       }
       else if(CHIP_GetADIEChipID() == CHIP_ID_SC1161)
       {
       		/*unlock the alarm function*/
	    reg_val = ANA_REG_GET(ANA_RTC_SPG_VALUE);
	    reg_val &= ~0xff;
	    reg_val |= SPRD_RTC_UNLOCK & 0xff;
	    ANA_REG_SET(ANA_RTC_SPG_UPD, reg_val);
       }
		
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
    uint32 reg_val;
    RTCDRV_PRINT ( ("RTC_DisableAlarm: Disable !"));

    RTC_CheckIfRtcUpdating (RTC_ACK_TIME, 1);
    RTC_CheckIfRtcUpdating (RTC_ACK_ALARM, 1);

    while ( (s_enable_update_reg_flag & RTC_ALARM_BIT) != RTC_ALARM_BIT)
    {
        /* @CR88036 jiexiayu */
        //we replace this sentence because RTOS task scheduler may not work at this time
        //SCI_Sleep(20);
        RTC_Wait (POLL_INTERVAL_TIME);
        /* @CR88036 end*/
    }

    SCI_DisableIRQ();
    s_enable_update_reg_flag &= ~ (RTC_ALARM_BIT);

    // Disable Alarm INT.
    ANA_REG_AND (ANA_RTC_INT_EN, ~RTC_ALARM_BIT);

    if (CHIP_GetADIEChipID() == CHIP_ID_SR1131BA)
    {	 
   	    /*lock the alarm function*/
	    reg_val = ANA_REG_GET(ANA_RTC_SPG_UPD);
	    reg_val &= ~0xff;
	    reg_val |= SPRD_RTC_LOCK & 0xff;
	    ANA_REG_SET(ANA_RTC_SPG_UPD, reg_val);		
    }
    else if(CHIP_GetADIEChipID() == CHIP_ID_SC1161)
    {
            /*lock the alarm function*/
	    reg_val = ANA_REG_GET(ANA_RTC_SPG_VALUE);
	    reg_val &= ~0xff;
	    reg_val |= SPRD_RTC_LOCK & 0xff;
	    ANA_REG_SET(ANA_RTC_SPG_UPD, reg_val);	
    }

    // Save time in struct.
    ANA_REG_SET (ANA_RTC_SEC_ALM, (uint16) 1);
    ANA_REG_SET (ANA_RTC_MIN_ALM, (uint16) 1);
    ANA_REG_SET (ANA_RTC_HOUR_ALM, (uint16) 1);
    ANA_REG_SET (ANA_RTC_DAY_ALM, (uint16) 31);
    s_updated_rtc |= RTC_SEC_ALM_ACK_BIT|RTC_MIN_ALM_ACK_BIT|RTC_HOUR_ALM_ACK_BIT|RTC_DAY_ALM_ACK_BIT;
    SCI_RestoreIRQ();

    //WaitAfterSetRtc();
    s_updated_rtc_time = SCI_GetTickCount();

    SCI_DisableIRQ();
    s_enable_update_reg_flag |= (RTC_ALARM_BIT);
    SCI_RestoreIRQ();

}

/*****************************************************************************/
//  Description:    Set Rtc interrupt Type.
//  Author:         Weihua.Wang
//  Note:
/*****************************************************************************/
PUBLIC uint32 RTC_SetRtcInt (RTC_INTTYPE_E type, BOOLEAN value)
{
    uint32    rtc_int = 0;

    if ( (type >= RTC_INT_MAX) || ( (value != TRUE) && (value != FALSE)))
    {
        return TB_PARAM_ERROR;
    }

    RTC_CheckIfRtcUpdating (RTC_ACK_TIME, 1);
    RTC_CheckIfRtcUpdating (RTC_ACK_ALARM, 1);

    //wait for the signal
    while ( (s_enable_update_reg_flag & (RTC_SEC_BIT|RTC_MIN_BIT|RTC_HOUR_BIT|RTC_DAY_BIT))
            != (RTC_SEC_BIT|RTC_MIN_BIT|RTC_HOUR_BIT|RTC_DAY_BIT))
    {
        /* @CR88036 jiexiayu */
        //we replace this sentence because RTOS task scheduler may not work at this time
        //SCI_Sleep(20);
        RTC_Wait (POLL_INTERVAL_TIME);
        /* @CR88036 end*/
    }

    SCI_DisableIRQ();

    rtc_int = 0x0;
    rtc_int = (1<<type) &RTC_INT_ALL_MSK;
    s_enable_update_reg_flag &= ~ (rtc_int);

    if (value)
    {
        // enable Alarm INT.
        ANA_REG_OR (ANA_RTC_INT_EN, rtc_int);
    }
    else
    {
        // Disable Alarm INT.
        ANA_REG_AND (ANA_RTC_INT_EN, ~rtc_int);
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
LOCAL BOOLEAN RTC_CheckIfRtcUpdating (RTC_ACK_E rtc_ack, BOOLEAN wait_option)
{
    uint32  time_out =0;

    if (RTC_ACK_TIME == rtc_ack)
    {
        // Wait or not according to the wait_option.
        if (wait_option)
        {
            while (s_updated_rtc &
                    (RTC_SEC_ACK_BIT|
                     RTC_MIN_ACK_BIT|RTC_HOUR_ACK_BIT|RTC_DAY_ACK_BIT))
            {
                RTC_Wait (20);
                time_out ++;

                if (time_out > RTC_TIME_OUT)
                {
					SCI_PASSERT(0,("%x %x %x %x %x %x %x %x %x %x %x %x %x",s_updated_rtc,ANA_REG_GET(ANA_RTC_SEC_CNT),/*assert to do*/
														 ANA_REG_GET(ANA_RTC_MIN_CNT),
														 ANA_REG_GET(ANA_RTC_HOUR_CNT),
														 ANA_REG_GET(ANA_RTC_DAY_CNT),
														 ANA_REG_GET(ANA_RTC_SEC_UPDATE),
														 ANA_REG_GET(ANA_RTC_MIN_UPDATE),
														 ANA_REG_GET(ANA_RTC_HOUR_UPDATE),
														 ANA_REG_GET(ANA_RTC_DAY_UPDATE),
														 ANA_REG_GET(ANA_RTC_INT_EN),
														 ANA_REG_GET(ANA_RTC_INT_RSTS),
														 ANA_REG_GET(ANA_RTC_INT_CLR),
														 ANA_REG_GET(ANA_RTC_INT_MSK)
														 ));
                    //SCI_PASSERT (0, ("RTC_CheckIfRtcUpdating time out!"));/*assert verified*/
                }
            }

            return SCI_FALSE;
        }
        else
        {
            // Just check the flag and return directly.
            if (s_updated_rtc &
                    (RTC_SEC_ACK_BIT|
                     RTC_MIN_ACK_BIT|RTC_HOUR_ACK_BIT|RTC_DAY_ACK_BIT))
            {
                return SCI_TRUE;
            }
            else
            {
                return SCI_FALSE;
            }
        }
    }
    else if (RTC_ACK_ALARM == rtc_ack)
    {
        // Wait or not according to the wait_option.
        if (wait_option)
        {
            while (s_updated_rtc &
                    (RTC_SEC_ALM_ACK_BIT|
                     RTC_MIN_ALM_ACK_BIT|RTC_HOUR_ALM_ACK_BIT|RTC_DAY_ALM_ACK_BIT))
            {
                RTC_Wait (20);
                time_out ++;

                if (time_out > RTC_TIME_OUT)
                {
                    SCI_PASSERT (0, ("RTC_CheckIfRtcUpdating time out!"));/*assert verified*/
                }
            }

            return SCI_FALSE;
        }
        else
        {
            // Just check the flag and return directly.
            if (s_updated_rtc &
                    (RTC_SEC_ALM_ACK_BIT|
                     RTC_MIN_ALM_ACK_BIT|RTC_HOUR_ALM_ACK_BIT|RTC_DAY_ALM_ACK_BIT))
            {
                return SCI_TRUE;
            }
            else
            {
                return SCI_FALSE;
            }
        }
    }
    else
    {
        RTCDRV_PRINT ( ("RTC_CheckIfRtcUpdating : Error Input! rtc_ack:%d, wait_option:%d",
                        rtc_ack, wait_option));

        return SCI_TRUE;
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
    CLEAR_RTC_INT (RTC_ALARM_BIT);
}

/*below 1 functions are removed from driver_export.c, wait clean up*/
uint32 CheckRTCState (void)
{
    return ANA_REG_GET (ANA_RTC_INT_RSTS);
}

/*******************************************************************************/
// Description: This function used to set spare value
// Global resource dependence: none
// Author:
// Note:
/********************************************************************************/
PUBLIC void RTC_SetSpareReg()
{
    uint32 reg_val;	

    if(CHIP_GetADIEChipID() == CHIP_ID_SC1161)
    {
	    reg_val = ANA_REG_GET(ANA_RTC_SPG_VALUE);
	    reg_val &= 0xff;
	    reg_val |= (SPRD_RTC_SPARE << 8) & 0xff00;
	    ANA_REG_SET(ANA_RTC_SPG_UPD, reg_val);
    }
}

/*******************************************************************************/
// Description: This function used to compare spare value
// Global resource dependence: none
// Author: yuhua.shi
// Note:
/********************************************************************************/
PUBLIC BOOLEAN RTC_CompareSpareReg()
{
    uint32 reg_val;
    BOOLEAN tmp = SCI_TRUE;

    if(CHIP_GetADIEChipID() == CHIP_ID_SC1161)
    {
            reg_val = ANA_REG_GET(ANA_RTC_SPG_VALUE);

            reg_val = (reg_val >> 8) & 0xff;

            tmp = (SPRD_RTC_SPARE == reg_val)?1:0;
    }

        return tmp;
}

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif  // End of rtc_drv.c
