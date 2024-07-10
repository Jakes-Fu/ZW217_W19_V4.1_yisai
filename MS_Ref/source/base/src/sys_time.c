/*******************************************************************************
 ** File Name:      sys_time.c                                                 *
 ** Author:         Eric.zhou                                                  *
 ** Date:           06/01/2003                                                 *
 ** Copyright:      2001 Spreatrum, Incoporated. All Rights Reserved.          *
 ** Description:    This file is the main file of Time module. It contains     *
 **                 most of the functions which control the Time get,set and   *
 **                 others associated functions.                               *
 *******************************************************************************
                                                                               
 *******************************************************************************
 **                        Edit History                                        *
 ** -------------------------------------------------------------------------- *
 ** DATE            NAME            DESCRIPTION                                *
 ** 2003/07/07    Xueliang.Wang Modify                     *
 ** 29/08/2005     weihua.wang      Modify for  CR29225                        *
 ******************************************************************************/
#include "ms_ref_base_trc.h"
#include "sys_time_ext.h"
#include "nv_type.h"
#include "nvitem.h"
#include "os_api.h"
#include "tasks_id.h"
#include "misc_ext.h"
#include "ref_param.h"
#include "rtc_drv.h"

#ifndef WIN32
    #include "tb_hal.h"
#endif  // WIN32    

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


#define SYSTIME_DEBUG

#ifdef  SYSTIME_DEBUG
  #define SYSTIME_PRINT   SCI_TRACE_LOW
#else 
  #define SYSTIME_PRINT   
#endif  // SYSTIME_DEBUG

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/
LOCAL const uint16 day_of_month[2][TM_MAX_MONTHS_SUPPORTED+1] =
    {
        {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
      {0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}
    };

LOCAL const uint16 days_from_jan1[2][TM_MAX_MONTHS_SUPPORTED] =
    {
        {0, 31,  59,  90,  120, 151, 181, 212, 243, 273, 304, 334   },
        {0, 31,  60,  91,  121, 152, 182, 213, 244, 274, 305, 335   }
    };

LOCAL const SCI_DATE_T s_sys_begin_date = 
    {1,  1,  1980,  2};

LOCAL const SCI_DATE_T s_sys_end_date =
    {31, 12, 2099,  4};
   
/**---------------------------------------------------------------------------*
 **                         Local Variables                                  *
 **---------------------------------------------------------------------------*/
// Save system date, it is same as flash.
//LOCAL SCI_DATE_T    s_last_date;

// Save alarm date & time.
LOCAL SCI_ALARM_T   s_alarm_time;

//s_is_alarm_active is not accessed!
//LOCAL BOOLEAN       s_is_alarm_active;

LOCAL uint32  s_end_day_number;

static BOOLEAN s_is_rtc_abnormal = FALSE;  

/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               *
**---------------------------------------------------------------------------*/
/*****************************************************************************/

//  Description :   Get the week-day of the inputed date.
//  Global resource dependence : 
//  Author:         Xueliang.Wang
//  Note:           Sunday = 0
//                  Monday = 1
//                  ......
/*****************************************************************************/
LOCAL uint32 GetWeekDayOfDate(  // Return week day.
    SCI_DATE_T  sys_date        // Date value
    );

/*****************************************************************************/
//  Description :   Get number of day from 2000/01/01 to the inputed date.
//  Global resource dependence : 
//  Author:         Xueliang.Wang
//  Note: 
//                  2000/01/01  day_num = 1
//                  2000/01/02  day_num = 2
//                  ......
/*****************************************************************************/
LOCAL uint32 GetDayNumOfDate(   // Return number of day.
    SCI_DATE_T  sys_date        // Date value
    );
    /*****************************************************************************/
//  Description :   Get date from number of day 
//  Global resource dependence : 
//  Author:         weihua.wang
//  Note: 
//                  2000/01/01  day_num = 0
//                  2000/01/02  day_num = 1
//                  ......
/*****************************************************************************/
LOCAL void GetDateOfDaynum(   // Return number of day.
    SCI_DATE_T  *date_ptr,          // Used to save date, if the date is out of
                                    // range(2099/12/31), set it to s_sys_begin_date
    uint32      day_num
    );
/*****************************************************************************/
//  Description : Handle the abnormal RTC value 
//  Global resource dependence : 
//  Author: hanjun.liu
//  Note: 
/*****************************************************************************/
LOCAL  void TM_HandleAbnormRTC(void);
/*****************************************************************************/
//  Description : check if the RTC time is valid
//  Global resource dependence : 
//  Author: hanjun.liu
//  Note: 
/*****************************************************************************/
LOCAL  BOOLEAN TM_IsRtcValid(TB_RTC_TIME_T rtc_time);
/**---------------------------------------------------------------------------*
 **                         Function Definitions                              *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description :   Initialize system date & time. 
//  Global resource dependence : 
//  Author:         Eric.zhou
//  Note: 
/*****************************************************************************/
PUBLIC void TM_Init(void)
{
  TB_RTC_TIME_T   rtc_time;
  SCI_DATE_T   last_date;
    //SYSTIME_PRINT:"TM_Init: Init system date & time."
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SYS_TIME_139_112_2_18_1_5_40_119,(uint8*)"");

    REFPARAM_SetAlarmTime(0);
    
    // Read date from flash to s_last_date, if it is not exited,
    // Set it to begin_date(1980/01/01 Tuesday)
    RTC_Get(&rtc_time);

    //calculate the end daynumber from end date
    s_end_day_number = GetDayNumOfDate(s_sys_end_date);
    s_is_rtc_abnormal = FALSE; 

    if ((FALSE==TM_IsRtcValid(rtc_time)) 
#ifndef WIN32
            || (FALSE==RTC_CompareSpareReg())
#endif
    )
    {
 #ifndef WIN32      
    TM_HandleAbnormRTC();
#endif 
    s_is_rtc_abnormal = TRUE;
    }
    else
    {
      TM_GetDateOfDayNum(&last_date, rtc_time.day);     
        // Saved s_last_date to NV.
        if(NVERR_NONE != EFS_NvitemWrite(NV_ORIGINAL_DATE, sizeof(SCI_DATE_T), (uint8 *)&last_date, 1))
        {
            SCI_PASSERT(0,("TM_Init:write last_date error!"));/*assert verified*/
        }       
    }
}
/*****************************************************************************/
//  Description :   This API retrun the result if RTC is abnormal
//  Global resource dependence : 
//  Author:         hanjun.liu
//  Note:           
/*****************************************************************************/
PUBLIC BOOLEAN TM_IsRtcAbnormal(void)
{
  if(s_is_rtc_abnormal)
  {
    s_is_rtc_abnormal = FALSE;
    return SCI_TRUE;
  }
  else
  {
    return SCI_FALSE;
  }
}

/*****************************************************************************/
//  Description :   Set system date(year/mon/day)
//  Global resource dependence : 
//  Author:         Xueliang.Wang
//  Note:           The week_day of system date input is ignorled.
/*****************************************************************************/
PUBLIC ERR_TM_E TM_SetSysDate(  // If successful, return ERR_TM_NONE;
                                // else return ERR_TM_WRONG_DATE.
    SCI_DATE_T  sys_date        // Date value to be set
  )
{
  TB_RTC_TIME_T  rtc_time;
    SCI_DATE_T     cur_sys_date;
    uint32         day_num;

    rtc_time.day = 0;
    rtc_time.hour = 0;
    rtc_time.min = 0;
    rtc_time.sec = 0;
    
    //SYSTIME_PRINT:"TM_SetSysDate: Set system date %04d-%02d-%02d !"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SYS_TIME_198_112_2_18_1_5_41_120,(uint8*)"ddd", sys_date.year, sys_date.mon, sys_date.mday);
        
    if (TM_IsDateValid(sys_date))
    {
        SCI_DisableIRQ();
        
        //get current sys date
        RTC_Get(&rtc_time);
      TM_GetDateOfDayNum(&cur_sys_date, rtc_time.day);

        // Set date to local variable s_last_date.        
        cur_sys_date.mday    = sys_date.mday;
        cur_sys_date.mon     = sys_date.mon;
        cur_sys_date.year    = sys_date.year;
        // The week-day input maybe mismatch date, update it.
        cur_sys_date.wday    = GetWeekDayOfDate(sys_date);
        
        SCI_RestoreIRQ();
        
        SCI_UpdateRtcCalibration(1);
        
        // get the day num from begin.
        TM_GetDayNumOfDate(cur_sys_date,&day_num);

        //SYSTIME_PRINT:"TM_SetSysDate: day num %04d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SYS_TIME_222_112_2_18_1_5_41_121,(uint8*)"d", day_num);
        
        // Update RTC day.
        RTC_SetDay(day_num);
    
     // Saved s_last_date to NV.
          if(NVERR_NONE != EFS_NvitemWrite(NV_ORIGINAL_DATE, sizeof(SCI_DATE_T), (uint8 *)&sys_date, 1))
           {
                SCI_PASSERT(0,("TM_SetSysDate:write NV error!"));/*assert verified*/
            }
        return ERR_TM_NONE;                
    }
    else
    {
      s_is_rtc_abnormal = TRUE;
        //SYSTIME_PRINT:"TM_SetSysDate: System date is invalid !"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SYS_TIME_237_112_2_18_1_5_41_122,(uint8*)"");
        return ERR_TM_WRONG_DATE;
    }

}

/*****************************************************************************/
//  Description :   Get system date(year/mon/day, week_day)
//  Global resource dependence : 
//  Author:         Eric.zhou
//  Note: 
/*****************************************************************************/
PUBLIC ERR_TM_E TM_GetSysDate(  // If successful, return ERR_TM_NONE;
                                // else return ERR_TM_PNULL.
    SCI_DATE_T  *date_ptr       // Save system date gotten.
  )
{
#ifndef WIN32
  TB_RTC_TIME_T  rtc_time;
    SCI_DATE_T     cur_sys_date;

    rtc_time.day = 0;
    rtc_time.hour = 0;
    rtc_time.min = 0;
    rtc_time.sec = 0;
    
    if (PNULL != date_ptr)
    {
      // Get RTC time.
      RTC_Get(&rtc_time);
      
      //if current rtc day is more than the max value,then restore to default value
        if(rtc_time.day > s_end_day_number)
        {
        TM_SetSysDate(s_sys_begin_date);
        s_is_rtc_abnormal = TRUE;
      }

      // Get Date from rtc day number
      TM_GetDateOfDayNum(&cur_sys_date, rtc_time.day);

      SCI_DisableIRQ();
      
      // Get date saved in local variable s_last_date.
      date_ptr->mday = cur_sys_date.mday;
      date_ptr->mon  = cur_sys_date.mon;
      date_ptr->year = cur_sys_date.year;
      date_ptr->wday = cur_sys_date.wday;

        SCI_RestoreIRQ();

      //SYSTIME_PRINT("TM_GetSysDate: Get system date %4d-%2d-%2d (%d) !", date_ptr->year, date_ptr->mon, date_ptr->mday, date_ptr->wday);

      return ERR_TM_NONE;
    }
    else
    {
        SCI_ASSERT(PNULL != date_ptr);/*assert verified*/
        return ERR_TM_PNULL;  /*lint !e527 */
    }
#else
  SYSTEMTIME system_time;

  GetLocalTime(&system_time);
    date_ptr->mday = system_time.wDay;
    date_ptr->mon  = system_time.wMonth;
    date_ptr->year = system_time.wYear;
    date_ptr->wday = system_time.wDayOfWeek;
#endif

}

/*****************************************************************************/
//  Description :       Set system time (hour/min/sec)
//  Global resource dependence : 
//  Author:             Xueliang.Wang
//  Note: 
/*****************************************************************************/
PUBLIC ERR_TM_E TM_SetSysTime(  // If successful, return ERR_TM_NONE;
                                // else return ERR_TM_WRONG_TIME.
    SCI_TIME_T  sys_time        // Time value to be set.
    )
{
    //SYSTIME_PRINT:"TM_SetSysTime: Set system time %2d:%2d:%2d !"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SYS_TIME_311_112_2_18_1_5_41_123,(uint8*)"ddd", sys_time.hour, sys_time.min, sys_time.sec);
        
    if (TM_IsTimeValid(sys_time))
    {
      // Set RTC time.
      TB_RTC_TIME_T   rtc_time,rtc_cur_time;

      rtc_cur_time.day = 0;
      rtc_cur_time.hour = 0;
      rtc_cur_time.min = 0;
      rtc_cur_time.sec = 0;

      RTC_Get(&rtc_cur_time);

      rtc_time.sec    = sys_time.sec;
      rtc_time.min    = sys_time.min;
      rtc_time.hour   = sys_time.hour;
      rtc_time.day    = rtc_cur_time.day;        
        RTC_Set(rtc_time);

        #ifdef SIMULATE_MIN_INT
        SCI_DisableIRQ();
        SCI_AdjustMiniuteIntTimer(60 - sys_time.sec);
        SCI_RestoreIRQ();
        #endif
        
        SCI_UpdateRtcCalibration(1);    
        
        return ERR_TM_NONE;
    }
    else
    {
        //SYSTIME_PRINT:"TM_SetSysTime: System time is invalid !"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SYS_TIME_338_112_2_18_1_5_41_124,(uint8*)"");
        return ERR_TM_WRONG_TIME;
    }
}

/*****************************************************************************/
//  Description :       Get system time (hour/min/sec)
//  Global resource dependence : 
//  Author:             Xueliang.Wang
//  Note: 
/*****************************************************************************/
PUBLIC ERR_TM_E TM_GetSysTime(  // If successful, return ERR_TM_NONE;
                                // else return ERR_TM_PNULL.
    SCI_TIME_T  *time_ptr       // Save time value gotten.
    )
{
    TB_RTC_TIME_T   rtc_time;

    rtc_time.day = 0;
    rtc_time.hour = 0;
    rtc_time.min = 0;
    rtc_time.sec = 0;
    
    if (PNULL != time_ptr)
    {
      // Get RTC time.
      RTC_Get(&rtc_time);
    
      time_ptr->sec  = rtc_time.sec;
      time_ptr->min  = rtc_time.min;
      time_ptr->hour = rtc_time.hour;
      
      //CR89000 jiexia 07.07.18
      if(SCI_FALSE == TM_IsTimeValid(*time_ptr))
      {
          TM_HandleAbnormRTC();
          s_is_rtc_abnormal = TRUE;     
          //get RTC value again
          RTC_Get(&rtc_time);
          time_ptr->sec  = rtc_time.sec;
          time_ptr->min  = rtc_time.min;
          time_ptr->hour = rtc_time.hour;
          SCI_ASSERT(SCI_TRUE == TM_IsTimeValid(*time_ptr));   /*assert verified*/
      }
      //CR89000 jiexia end

    //SYSTIME_PRINT("TM_GetSysTime: Get system time %2d:%2d:%2d !", time_ptr->hour, time_ptr->min, time_ptr->sec);
      
      return ERR_TM_NONE;
    }
    else
    {
        SCI_ASSERT(PNULL != time_ptr);/*assert verified*/
        return ERR_TM_PNULL;  /*lint !e527 */
    }
}

static uint16 oldtick = 0;
static uint16 lasttick = 0;
static TB_RTC_TIME_T oldtime = {0};

/*****************************************************************************/
//  Description :       Get system timex (hour/min/sec/msec)
//  Global resource dependence : 
//  Author:             cheng.Fang
//  Note: 
/*****************************************************************************/
PUBLIC ERR_TM_E TM_GetSysTimeEx(  // If successful, return ERR_TM_NONE;
                                // else return ERR_TM_PNULL.
    SCI_TIMEX_T  *time_ptr       // Save time value gotten.
    )
{
    TB_RTC_TIME_T   rtc_time;
    uint16 newtick=0,mstick = 0;

    rtc_time.day = 0;
    rtc_time.hour = 0;
    rtc_time.min = 0;
    rtc_time.sec = 0;

    if (PNULL != time_ptr)
    {
    // Get RTC time.
    RTC_Get(&rtc_time);

        if((rtc_time.hour==oldtime.hour)&&(rtc_time.min==oldtime.min)&&(rtc_time.sec==oldtime.sec))
        {
            newtick = SCI_GetTickCount();
            
            mstick = newtick - oldtick;

            if(mstick>0)
                mstick  = mstick%1000;
            else //tick reach max
                mstick = (1000-oldtick%1000)+newtick%1000;
            
            lasttick +=mstick;
            
            time_ptr->msec = lasttick;

            oldtick = newtick;
        }
        else
        {
            oldtick = SCI_GetTickCount();
            oldtime = rtc_time;
            lasttick = 0;
            time_ptr->msec = 0;
         }
 
        time_ptr->sec  = rtc_time.sec;
        time_ptr->min  = rtc_time.min;
        time_ptr->hour = rtc_time.hour;

        //SYSTIME_PRINT("TM_GetSysTime: Get system time %2d:%2d:%2d !", time_ptr->hour, time_ptr->min, time_ptr->sec);

    return ERR_TM_NONE;
    }
    else
    {
        SCI_ASSERT(PNULL != time_ptr);/*assert verified*/
        return ERR_TM_PNULL;  /*lint !e527 */
    }
}

/*****************************************************************************/
//  Description :   Get number of seconds escaped from 2000/01/01/ 00:00:00.
//  Global resource dependence : 
//  Author:         Xueliang.Wang
//  Note:
/*****************************************************************************/
PUBLIC uint32 TM_GetTotalSeconds(void)  // Return number of seconds.
{
    uint32      sec;
    SCI_DATE_T  tmp_date;
    SCI_TIME_T  tmp_time={0,0,0};

#ifndef FPGA_VERIFICATION    
    TM_GetSysDate(&tmp_date);
    TM_GetSysTime(&tmp_time);

    sec = GetDayNumOfDate(tmp_date) * 24 * 3600
        + tmp_time.hour * 3600 + tmp_time.min * 60 + tmp_time.sec;
#else
    {
        static uint32 s_rtc_sec = 0;
        sec = s_rtc_sec++;
    }
#endif

    return sec;
}

/*****************************************************************************/
//  Description :   Get the week-day of the inputed date.
//          If date is between 2000/01/01/00:00:00 and 
//          2049/12/31/23:59:59, convert result(saved in week_day_ptr)
//          is correct; else it is 0.
//  Global resource dependence : 
//  Author:         Xueliang.Wang
//  Note:           Value: 0 - 6
//            Sunday = 0
//                    Monday = 1
//                    ......
/*****************************************************************************/
PUBLIC ERR_TM_E TM_GetWeekDayOfDate(    // If successful, return ERR_TM_NONE
                                        // else return ERR_TM_WRONG_DATE or ERR_TM_PNULL.
    SCI_DATE_T  date_val,               // Date value
    uint8       *week_day_ptr           // Save the week day if the date is correct;
                                        // else it is 0
    )
{
    if (PNULL != week_day_ptr)
    {
        // Check if the date is valid.
        if (TM_IsDateValid(date_val))
        {
            // Get the week day.
            // 2000/01/01 is Saturday(week_day = 6)
            *week_day_ptr = (uint8)GetWeekDayOfDate(date_val);            
            return ERR_TM_NONE;
        }
        else
        {
            *week_day_ptr = 0;            
            return ERR_TM_WRONG_DATE;
        }
    }
    else
    {
        SCI_ASSERT(PNULL != week_day_ptr);/*assert verified*/
        return ERR_TM_PNULL;  /*lint !e527 */
    }
}

/*****************************************************************************/
//  Description :   Get number of day from 1980/01/01 to the inputed date.
//          If date is between 1980/01/01/00:00:00 and 
//          2099/12/31/23:59:59, convert result(saved in day_num_ptr)
//          is correct; else it is 0.
//  Global resource dependence : 
//  Author:         Xueliang.Wang
//  Note: 
//                  2000/01/01  day_num = 0
//                  2000/01/02  day_num = 1
//                  ......
/*****************************************************************************/
PUBLIC ERR_TM_E TM_GetDayNumOfDate( // If successful, return ERR_TM_NONE
                                    // else return ERR_TM_WRONG_DATE or ERR_TM_PNULL.
    SCI_DATE_T  date_val,           // Date value
    uint32      *day_num_ptr        // Save number of day if the date is correct;
                                    // else it is 0
    )
{
    if (PNULL != day_num_ptr)
    {
        // Check if the date is valid.
        if (TM_IsDateValid(date_val))
        {
            // Get the day number.
            *day_num_ptr = GetDayNumOfDate(date_val);            
            return ERR_TM_NONE;
        }
        else
        {
            *day_num_ptr = 0;
            return ERR_TM_WRONG_DATE;
        }
    }
    else
    {
        SCI_ASSERT(PNULL != day_num_ptr);/*assert verified*/
        return ERR_TM_PNULL;  /*lint !e527 */
    }    
}

/*****************************************************************************/
//  Description :   Get date from number of day(since 2000/01/01).
//                  If the date is out of range(2049/12/31), return start date
//                  2000/01/01.
//  Global resource dependence : 
//  Author:         Xueliang.Wang
//  Note: 
//                  2000/01/01  day_num = 0
//                  2000/01/02  day_num = 1
//                  ......
/*****************************************************************************/
PUBLIC ERR_TM_E TM_GetDateOfDayNum( // If successful, return ERR_TM_NONE
                                    // else return ERR_TM_WRONG_DATE or ERR_TM_PNULL.
    SCI_DATE_T  *date_ptr,          // Used to save date, if the date is out of
                                    // range(2050/01/01), set it to s_sys_begin_date
    uint32      day_num             // Number of day from (2000/01/01)
    )
{ 
    // int32  i;
    //uint16  *days_ptr;
    
    if (date_ptr != PNULL)
    {
        if (TM_IsDayNumValid(day_num))
        {
            GetDateOfDaynum(date_ptr,day_num);          
            
            return ERR_TM_NONE;
        }   
        else
        {
            SCI_MEMCPY((void*)date_ptr, (void*)&s_sys_begin_date, sizeof(SCI_DATE_T));            
            return ERR_TM_WRONG_DATE;
        }
    }
    else
    {
        SCI_ASSERT(date_ptr != PNULL);/*assert verified*/
        return ERR_TM_PNULL;  /*lint !e527 */
    }
}

/*****************************************************************************/
//  Description :   Judge if the year is leap year.
//  Global resource dependence : 
//  Author:         Eric.zhou
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN TM_IsLeapYear(   // Return SCI_TRUE, if it is leap year;
                                // else, return SCI_FALSE.
    uint16  year                // The year to be judge
    )
{
  if (year % 400 == 0)
  {
    return SCI_TRUE;
  }
  else if (year % 100 == 0)
  {
    return SCI_FALSE;
  }
  else if (year % 4 == 0)
  {
    return SCI_TRUE;
  }
  else
  {
    return SCI_FALSE;
  }
}

/*****************************************************************************/
//  Description :   Check if the date is valid.
//  Global resource dependence : 
//  Author:         Xueliang.Wang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN TM_IsDateValid(  // Return SCI_TRUE, if it is valid;
                                // else, return SCI_FALSE.
    SCI_DATE_T  date_val        // Date value
  )
{
    BOOLEAN b_status = SCI_FALSE;
    
    if ((date_val.year >= s_sys_begin_date.year) && (date_val.year <= s_sys_end_date.year ))
    {
        if (date_val.mon <= TM_MAX_MONTHS_SUPPORTED)
        {
            uint32  year_type = (TM_IsLeapYear(date_val.year)) == SCI_TRUE ? 1 : 0;
            if (date_val.mday <= day_of_month[year_type][date_val.mon])
            {
                b_status = SCI_TRUE;
            }
        }        
    }
   // if(SCI_FALSE == b_status)
   // {
   //   s_is_rtc_abnormal = TRUE;
  //  }    
    return b_status;
}

/*****************************************************************************/
//  Description :   Check if the time is valid.
//  Global resource dependence : 
//  Author:         Xueliang.Wang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN TM_IsTimeValid(  // Return SCI_TRUE, if it is valid;
                                // else, return SCI_FALSE.
    SCI_TIME_T    time_val      // Time value
  )
{
#ifndef FPGA_VERIFICATION
    if ((time_val.hour < 24) && (time_val.min < 60) && (time_val.sec < 60))
    {
        return SCI_TRUE;
    }
    else
    {
        return SCI_FALSE;
    }    
#else
  return SCI_TRUE;
#endif
}

/*****************************************************************************/
//  Description :   Check if the timex is valid.
//  Global resource dependence : 
//  Author:         cheng.Fang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN TM_IsTimeXValid(  // Return SCI_TRUE, if it is valid;
                                // else, return SCI_FALSE.
    SCI_TIMEX_T    time_val      // Time value
  )
{
#ifndef FPGA_VERIFICATION
    if ((time_val.hour < 24) && (time_val.min < 60) && (time_val.sec < 60))
    {
        return SCI_TRUE;
    }
    else
    {
        return SCI_FALSE;
    }    
#else
  return SCI_TRUE;
#endif
}

/*****************************************************************************/
//  Description :   Check if the time is valid.
//  Global resource dependence : 
//  Author:         Xueliang.Wang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN TM_IsDayNumValid(    // Return SCI_TRUE, if it is valid;
                                    // else, return SCI_FALSE.
    uint32  day_num                 // Day number from 2000/01/01
  )
{   
    //s_end_day_number is calcuted in tm_int
    if (day_num > s_end_day_number)
    {
      s_is_rtc_abnormal = TRUE;
        return SCI_FALSE;         
    }
    else
    {
        return SCI_TRUE;
    }
}

/*****************************************************************************/
//  Description :   Get the week-day of the inputed date.
//  Global resource dependence : 
//  Author:         Xueliang.Wang
//  Note:           Sunday = 0
//                  Monday = 1
//                  ......
/*****************************************************************************/
LOCAL uint32 GetWeekDayOfDate(  // Return week day.
    SCI_DATE_T  date_val        // Date value
    )
{
    uint32  days;
    uint32  week_day;
    
    // Get the number of day from 2000/01/01 to date_val.
    days = GetDayNumOfDate(date_val);
    
    // Get the week day.
    // 2000/01/01 is Saturday(week_day = 6)
    week_day = (days + 6) % 7;
    
    return week_day;
}
   
/*****************************************************************************/
//  Description :   Get number of day escaped from 2000/01/01 to the inputed date.
//  Global resource dependence : 
//  Author:         Xueliang.Wang
//  Note: 
//                  2000/01/01  day_num = 0
//                  2000/01/02  day_num = 1
//                  ......
/*****************************************************************************/
LOCAL uint32 GetDayNumOfDate(   // Return number of day.
    SCI_DATE_T  date_val        // Date value
    )
{
    uint32  year_type;
  uint32 day_num;
  uint32 curYear,curmonth,curmday;

  day_num = 0;

  curYear = s_sys_begin_date.year;
    while(date_val.year > curYear)
  {
      year_type = (curYear%4==0 && curYear%100!=0 || curYear%400==0);

    day_num += (365 + year_type);
        curYear++;
  }

    year_type = (curYear%4==0 && curYear%100!=0 || curYear%400==0);
    
  curmonth = s_sys_begin_date.mon;
    if(curmonth < date_val.mon)
    {
      while(curmonth < date_val.mon)
      {
            day_num += day_of_month[year_type][curmonth];
        curmonth++;
      }
    }
    else//if s_sys_begin_date.mon is not January.
    {
        
        year_type = (s_sys_begin_date.year%4==0 && s_sys_begin_date.year%100!=0 || s_sys_begin_date.year%400==0);
        while(curmonth>date_val.mon)
      {
            day_num -= day_of_month[year_type][curmonth-1];
        curmonth--;
      }
    }
    
    curmday = s_sys_begin_date.mday;
    if(date_val.mday < curmday)
    {
        day_num -= (curmday - date_val.mday );
    }
    else
    {
      day_num += date_val.mday - curmday;//day_num += date_val.mday - 1;
    }
    return day_num;    
}    

/*****************************************************************************/
//  Description :   Get date from number of day 
//  Global resource dependence : 
//  Author:         weihua.wang
//  Note: 
//                  2000/01/01  day_num = 0
//                  2000/01/02  day_num = 1
//                  ......
/*****************************************************************************/
LOCAL void GetDateOfDaynum(   // Return number of day.
    SCI_DATE_T  *date_ptr,          // Used to save date, if the date is out of
                                    // range(2099/12/31), set it to s_sys_begin_date
    uint32      day_num
    )
{
    SCI_DATE_T   begin_date;
  uint32       i;                     // for loop
  uint32       curYear,curMonth = 1,curDay = 1,curweekday = 1; 
  uint32       days_from_begin = 0;       // add-up days, the key variable
    uint32       year_type;              // whether be leap year;

  days_from_begin = 0;

  SCI_MEMCPY(&begin_date,&s_sys_begin_date,sizeof(SCI_DATE_T));/*lint !e516 -e516*/
  year_type = (begin_date.year %4==0 && begin_date.year%100!=0 || begin_date.year%400==0);
  
  // get the days of the start year 
  for (i = 1; i < begin_date.mon; i++)
  {
    days_from_begin += day_of_month[year_type][i];
  }
  
  days_from_begin += begin_date.mday;
  
  // get the lasted days
  days_from_begin += day_num;
  
  // calculate the current year
  curYear = begin_date.year;

  //calculate current year and remain days
  //The more conventional form of
    //  infinite loop prefix is for(;;)
  //while(1)
  for(;;)
  {
      year_type = (curYear%4==0 && curYear%100!=0 || curYear%400==0);

    if(days_from_begin <= (365 + year_type))
      break;

    days_from_begin -= (365 + year_type);
    curYear += 1;
  }
  
  //current year is leep year?
  year_type = (curYear%4==0 && curYear%100!=0 || curYear%400==0);
  
  //calculate cur month and cur day info
  for (i = 1; i <= 12; i++)
  {
    if(days_from_begin <= day_of_month[year_type][i])
    {
      curDay   = days_from_begin;
      curMonth = i;
      break;
    }
    days_from_begin  -= day_of_month[year_type][i];
  }
 
  //calculate current week day info
  curweekday = (day_num + begin_date.wday)%7; 

  date_ptr->year       = curYear;
  date_ptr->mon   = curMonth;
  date_ptr->mday    = curDay;
  date_ptr->wday        = curweekday;
}

/*****************************************************************************/
//  Description :   Set alarm (year/mon/day hour:min:sec)
//  Global resource dependence : 
//  Author:         Xueliang.Wang
//  Note:           
/*****************************************************************************/
PUBLIC ERR_TM_E TM_SetAlarm(    // If successful, return ERR_TM_NONE;
                                // else return ERR_TM_ALARM_TIME_IS_EXPIRED 
                                // or ERR_TM_ALARM_WRONG_TIME.                                
    SCI_ALARM_T  alarm_time     // Alarm date and time value to be set
  )
{
    uint32      status;
    uint32      alarm_sec;
    uint32      sys_sec;
    uint32      alarm_days;
    SCI_DATE_T  tmp_date;
    SCI_TIME_T  tmp_time;
    TB_RTC_TIME_T   rtc_time;  
            
    //SYSTIME_PRINT:"TM_SetAlarm: Set Alarm %4d-%2d-%2d  %2d:%2d:%2d !"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SYS_TIME_815_112_2_18_1_5_42_125,(uint8*)"dddddd", alarm_time.year, alarm_time.mon, alarm_time.mday, alarm_time.hour, alarm_time.min, alarm_time.sec);
    
  tmp_time.sec    = alarm_time.sec ;
  tmp_time.min    = alarm_time.min ;
  tmp_time.hour   = alarm_time.hour;
                                     
  tmp_date.mday   = alarm_time.mday;
  tmp_date.mon    = alarm_time.mon ;
  tmp_date.year   = alarm_time.year;
    
    if ((TM_IsDateValid(tmp_date)) && (TM_IsTimeValid(tmp_time)))
    {
        // Calculate alarm days from 2000/01/01
        alarm_sec = GetDayNumOfDate(tmp_date) * 3600 * 24
                    + alarm_time.hour * 3600
                    + alarm_time.min  * 60
                    + alarm_time.sec;
        
        // Get current days from 2000/01/01
        TM_GetSysDate(&tmp_date);
        TM_GetSysTime(&tmp_time);
        sys_sec = GetDayNumOfDate(tmp_date) * 3600 * 24
                    + tmp_time.hour * 3600
                    + tmp_time.min  * 60
                    + tmp_time.sec;
        
        if (alarm_sec > sys_sec)
        {
          SCI_DisableIRQ();
      //s_is_alarm_active is not accessed!
          //s_is_alarm_active   = SCI_TRUE;
            SCI_MEMCPY(&s_alarm_time, &alarm_time, sizeof(SCI_ALARM_T));
            SCI_RestoreIRQ();
            
            REFPARAM_SetAlarmTime(alarm_sec);
            
            //get alarm day from date info 
          tmp_date.mday   = s_alarm_time.mday;
          tmp_date.mon    = s_alarm_time.mon ;
          tmp_date.year   = s_alarm_time.year;
          TM_GetDayNumOfDate(tmp_date, &alarm_days);

            rtc_time.sec  = s_alarm_time.sec;
            rtc_time.min  = s_alarm_time.min;
            rtc_time.hour = s_alarm_time.hour;
            rtc_time.day  = alarm_days; 

        //SYSTIME_PRINT:"TM_SetAlarm:rtc alarm value %4d %2d:%2d:%2d !"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SYS_TIME_864_112_2_18_1_5_42_126,(uint8*)"dddd", rtc_time.day , rtc_time.hour , rtc_time.min , rtc_time.sec );

            RTC_SetAlarm(rtc_time);
            status = ERR_TM_NONE;
        }
        else
        {
            // Alarm time is earlier than now.
            //SYSTIME_PRINT:"TM_SetAlarm: Alarm time is before current time !"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,SYS_TIME_872_112_2_18_1_5_42_127,(uint8*)"");
            
            TM_DisableAlarm();
            SCI_ASSERT(0);/*assert verified*/
            status = ERR_TM_ALARM_TIME_IS_EXPIRED;  /*lint !e527 */
        }
    }
    else
    {
        //SYSTIME_PRINT:"TM_SetAlarm: Alarm time is invalid !"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SYS_TIME_881_112_2_18_1_5_42_128,(uint8*)"");
        
      s_is_rtc_abnormal = TRUE;
        TM_DisableAlarm();
        SCI_ASSERT(0);/*assert verified*/
        status = ERR_TM_ALARM_WRONG_TIME;  /*lint !e527 */
    }
    
    return status;
}           

/*****************************************************************************/
//  Description :   Disable alarm.
//  Global resource dependence : 
//  Author:         Xueliang.Wang
//  Note:           
/*****************************************************************************/
PUBLIC void TM_DisableAlarm(void)
{
    //SYSTIME_PRINT:"TM_DisableAlarm: Disable alarm (%4d-%02d-%02d  %02d:%02d:%02d)!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SYS_TIME_900_112_2_18_1_5_42_129,(uint8*)"dddddd", s_alarm_time.year, s_alarm_time.mon, s_alarm_time.mday, s_alarm_time.hour, s_alarm_time.min, s_alarm_time.sec);
    
    SCI_DisableIRQ();
  //s_is_alarm_active is not accessed!
    //s_is_alarm_active   = SCI_FALSE;
    SCI_MEMSET(&s_alarm_time, 0, sizeof(SCI_ALARM_T));
    SCI_RestoreIRQ();
    
    // Disable Alarm.
    RTC_DisableAlarm();

    REFPARAM_SetAlarmTime(0);
}

/*****************************************************************************/
//  Description :   Check if alarm is expired.
//  Global resource dependence : 
//  Author:         Xueliang.Wang
//  Note:           This function is called after init RTC and before TM_Init.
/*****************************************************************************/
PUBLIC BOOLEAN TM_IsAlarmExpired(void)
{
    NVITEM_ERROR_E  status;
    uint32          alarm_time;
    SCI_DATE_T      last_date;
    
    // Get alarm time from NV.
    alarm_time = REFPARAM_GetAlarmTime();
    
    // Get last date from NV.
    status = EFS_NvitemRead(NV_ORIGINAL_DATE, sizeof(SCI_DATE_T),(uint8 *)&last_date);
  if(NVERR_NONE != status)
  {
      return SCI_FALSE;
    }
    else
    {
        //uint32          day_num;
        uint32          sec_num;
        TB_RTC_TIME_T   rtc_time;

        RTC_Get(&rtc_time);
        
        sec_num = ((rtc_time.day * 24 + rtc_time.hour) * 60 + rtc_time.min) * 60 + rtc_time.sec;
#ifdef SECURE_BOOT_SUPPORT
        if ((sec_num >= alarm_time) && (sec_num - alarm_time) <= 8)
#else
        if ((sec_num >= alarm_time) && (sec_num - alarm_time) <= 5)
#endif
        {
            return SCI_TRUE;
        }
        else
        {
            //invalid alarm int, clear it
            RTC_ClearAlarmInt();
            return SCI_FALSE;
        }
    }
}

/*****************************************************************************/
//  Description : Get time (year/mon/day/hour/min/sec) 
//  Global resource dependence : 
//  Author: Eric.zhou
//  Note: 
/*****************************************************************************/
PUBLIC void TM_GetTime(SCI_TM_T *realtime)
{
  SCI_TIME_T  tmp_time={0,0,0};
  SCI_DATE_T  tmp_date;


  TM_GetSysDate(&tmp_date);
  TM_GetSysTime(&tmp_time); 
  
  realtime->tm_sec    = tmp_time.sec;
  realtime->tm_min    = tmp_time.min;
  realtime->tm_hour   = tmp_time.hour;
  
  realtime->tm_mday   = tmp_date.mday;
  realtime->tm_mon    = tmp_date.mon;
  realtime->tm_year   = tmp_date.year;
  realtime->tm_wday   = tmp_date.wday;

    {
        uint32  i;
        uint16  *days_ptr;
        
        i = ((TM_IsLeapYear(tmp_date.year)) == SCI_TRUE) ? 1 : 0;
        days_ptr = (uint16  *)&(days_from_jan1[i][0]);
        
        realtime->tm_yday = *(days_ptr + tmp_date.mon - 1) + tmp_date.mday - 1;
    }
        
  realtime->tm_isdst  = 0;
}

/*****************************************************************************/
//  Description : Set time (year/mon/day/hour/min/sec) 
//  Global resource dependence : 
//  Author: Eric.zhou
//  Note: 
/*****************************************************************************/
PUBLIC uint32 TM_SetTime(SCI_TM_T realtime)
{
    uint32  status;
    SCI_TIME_T  tmp_time;
  SCI_DATE_T  tmp_date;

  tmp_time.sec    = realtime.tm_sec ;
  tmp_time.min    = realtime.tm_min ;
  tmp_time.hour   = realtime.tm_hour;
                                     
  tmp_date.mday   = realtime.tm_mday;
  tmp_date.mon    = realtime.tm_mon ;
  tmp_date.year   = realtime.tm_year;
  tmp_date.wday   = realtime.tm_wday;

  status = TM_SetSysDate(tmp_date);
  if (SCI_SUCCESS == status)
  {
      status = TM_SetSysTime(tmp_time);
  }
  return status;
}

/*****************************************************************************/
//  Description :   set rtc int type enable/disable
//  Global resource dependence : 
//  Input:          type:   the rtc interrupt type
//                          TM_INT_SEC 
//              TM_INT_MIN 
//              TM_INT_HOUR
//              TM_INT_DAY 
//              TM_INT_ALARM
//                  value  : interrupt enable/disable                      
//  Output          ERR_TM_NONE: success
//  Author:         Weihua.Wang
//  Note:           
/*****************************************************************************/
PUBLIC ERR_TM_E TM_SetRtcIntType(TM_INTTYPE_E type, BOOLEAN value)
{
    //SYSTIME_PRINT:"TM_SetRtcIntType: type %d- value%d!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SYS_TIME_1030_112_2_18_1_5_42_130,(uint8*)"dd", type, value);
    
    SCI_ASSERT(type < TM_INT_MAX);/*assert verified*/
    
    if((value != TRUE)&&(value != FALSE))
    {
        return ERR_TM_WRONG_DATE;
    }
        
    if(type < TM_INT_MAX)
    {
        RTC_SetRtcInt((RTC_INTTYPE_E)type,value);
        return ERR_TM_NONE;                
    }
    else
    {
        //SYSTIME_PRINT:"TM_SetSysDate: System date is invalid !"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SYS_TIME_1046_112_2_18_1_5_42_131,(uint8*)"");
        return ERR_TM_WRONG_DATE;
    }
    
} 
/*****************************************************************************/
//  Description : Handle the abnormal RTC value 
//  Global resource dependence : 
//  Author: hanjun.liu
//  Note: 
/*****************************************************************************/
LOCAL  void TM_HandleAbnormRTC(void)
{
  SCI_DATE_T   last_date;
  SCI_TIME_T  sys_time ;
  NVITEM_ERROR_E ret=0;
  ret=EFS_NvitemRead(NV_ORIGINAL_DATE, sizeof(SCI_DATE_T),(uint8 *)&last_date);
  SCI_TRACE_LOW("TM_HandleAbnormRTC  EFS_NvitemWrite  return value=%d",ret);
  if(NVERR_NONE != ret)
  {
        //SYSTIME_PRINT:"TM_GetSysDate: Read BeginDate from NV failed !"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SYS_TIME_1062_112_2_18_1_5_42_132,(uint8*)"");
        TM_SetSysDate(s_sys_begin_date);
        sys_time.sec = 0;
        sys_time.min = 0;
        sys_time.hour = 0;
        TM_SetSysTime(sys_time);  
        s_is_rtc_abnormal = TRUE;   

        // Saved s_last_date to NV.
        if(NVERR_NONE != EFS_NvitemWrite(NV_ORIGINAL_DATE, sizeof(SCI_DATE_T), (uint8 *)&s_sys_begin_date, 1))
        {
            SCI_PASSERT(0,("TM_Init:write s_sys_begin_date error!"));/*assert verified*/
        }     
    }
  else 
  {
    last_date.mon = 1;
    last_date.mday= 1;
    TM_SetSysDate(last_date);
    sys_time.sec = 0;
             sys_time.min = 0;
        sys_time.hour = 0;
    TM_SetSysTime(sys_time);
  }
}
/*****************************************************************************/
//  Description : check if the RTC time is valid
//  Global resource dependence : 
//  Author: hanjun.liu
//  Note: 
/*****************************************************************************/
LOCAL  BOOLEAN TM_IsRtcValid(TB_RTC_TIME_T rtc_time)
{
  BOOLEAN ret_val = TRUE;
  SCI_DATE_T   last_date;
  SCI_DATE_T   cur_date;
  
        if(rtc_time.day>s_end_day_number)
        {
          ret_val = FALSE;    
  }
  else
  {
    if(NVERR_NONE != EFS_NvitemRead(NV_ORIGINAL_DATE, sizeof(SCI_DATE_T),(uint8 *)&last_date))
         {
          ret_val = FALSE;  
    }
    else
    {
      TM_GetDateOfDayNum(&cur_date, rtc_time.day);
      if((cur_date.year < last_date.year) || ((cur_date.year - last_date.year) > 5))
      {
        ret_val = FALSE;    
      }
    }
  }
  return ret_val;
 }
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
