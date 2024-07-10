/****************************************************************************
** File Name:      mmicom_time.c                                               *
** Author:         jian.ma                                                   *
** Date:           03/04/2008                                                *
** Copyright:      2008 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe the application modual.   *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 4/2008        jian.ma            Create
** 
****************************************************************************/
#define MMICOM_TIME_C


/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "mmi_app_common_trc.h"
//#ifdef WIN32
//#include "std_header.h"
//#endif
#include "mmicom_time.h"
#include "mmk_type.h"
//#include "guicommon.h"
#include "mmisrv_adapt.h"
//#include "mmiset.h"
//#include "mmiset_export.h"
#include "mmi_event_api.h"

/**---------------------------------------------------------------------------*
 **                         Macro Declaration                                 *
 **---------------------------------------------------------------------------*/
#define     MONTH_START             1

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/

const uint16 dayfromjan1[13] =
{0,31,59,90,120,151,181,212,243,273,304,334,365};

const uint16 dayfromjan1run[13] =
{0,31,60,91,121,152,182,213,244,274,305,335,366};

const uint16 day_per_mon[MMICOM_DATE_MAX_MONTH] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

//const uint16 dayofleapyear[13] =
// {0,31,60,91,121,152,182,213,244,274,305,335,366};

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Functions                                         *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : check the day of date if is valid
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CheckDayOfDateIsValid(
                                    MMISET_DATE_T            date    //the date
                                    )
{
    BOOLEAN     result = TRUE;
    
    switch (date.date_month)
    {
    case MMISET_DATE_JAN:
    case MMISET_DATE_MAR:
    case MMISET_DATE_MAY:
    case MMISET_DATE_JUL:
    case MMISET_DATE_AUG:
    case MMISET_DATE_OCT:
    case MMISET_DATE_DEC:
        //check big month
        if ((MMICOM_DATE_BIG_MONTH_DAY < date.date_day))
        {
            result = FALSE;
        }
        break;
        
    case MMISET_DATE_APR:
    case MMISET_DATE_JUN:
    case MMISET_DATE_SEP:
    case MMISET_DATE_NOV:
        //check small month
        if ((MMICOM_DATE_SMALL_MONTH_DAY < date.date_day))
        {
            result = FALSE;
        }
        break;
        
    case MMISET_DATE_FEB:
        if ((!(date.date_year % 4) && (date.date_year % 100)) || !(date.date_year % 400))
        {
            //check leap month
            if ((MMICOM_DATE_LEAP_MONTH_DAY < date.date_day))
            {
                result = FALSE;
            }
        }
        else
        {
            //check not leap month
            if ((MMICOM_DATE_NOT_LEAP_MONTH_DAY < date.date_day))
            {
                result = FALSE;
            }
        }
        break;
        
    default:
        //SCI_TRACE_LOW:"CheckDayOfDateIsValid:date.date_month = %d is error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICOM_TIME_140_112_2_18_1_59_17_129,(uint8*)"d",date.date_month);
        break;
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : 获得文件系统当前的系统时间
//  Global resource dependence : 
//  Author: Jassmine.Meng
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMIAPICOM_GetFfsTime(void)
{
    return SCI_GetTickCount();
}


/*****************************************************************************/
//  Description :to get the current time 
//  Global resource dependence : none
//  Author:Harvey.Huang
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIAPICOM_GetCurTime(void)
{
    SCI_DATE_T sys_date = {0};
    SCI_TIME_T sys_time = {0};
    uint32      second = 0;
    
    TM_GetSysDate(&sys_date);
    TM_GetSysTime(&sys_time);
    second = MMIAPICOM_Tm2Second( sys_time.sec, 
        sys_time.min, 
        sys_time.hour,
        sys_date.mday,
        sys_date.mon, 
        sys_date.year );   
    return ( second );
}


/*****************************************************************************/
//  Description : convert SFS time to uint32
//  Global resource dependence : 
//  Author:liqing.peng
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMIAPICOM_TansferSFSTime(
                          SFS_DATE_T    date,
                          SFS_TIME_T    time
                          )
{
    uint16      comp_date = 0;
    uint16      comp_time = 0;
    uint16      curr_date = 0;
    uint16      curr_time = 0;
    uint32      result = 0;

    //SCI_TRACE_LOW:"year = %d, mon = %d, day = %d, hour = %d, min = %d, sec = %d "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICOM_TIME_194_112_2_18_1_59_17_130,(uint8*)"dddddd", date.year, date.mon, date.mday, time.hour, time.min, time.sec);

    //date
    curr_date = date.mday&0x1f;
    comp_date = comp_date | (curr_date);

    curr_date = date.mon&0x0f;
    comp_date = comp_date | (curr_date<<5);

    curr_date = (date.year-MMICOM_DATE_MIN_YEAR)&0x7f;
    comp_date = comp_date | (curr_date<<9);

    //time
    curr_time = (time.sec/2)&0x1f;
    comp_time = comp_time | (curr_time);

    curr_time = time.min&0x3f;
    comp_time = comp_time | (curr_time<<5);

    curr_time = time.hour&0x1f;
    comp_time = comp_time | (curr_time<<11);

    //result
    result = comp_time;
    result = (result<<16) | comp_date;

    return result;
}

/*****************************************************************************/
//  Description : divert fat file system time to tm structure
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC MMI_TM_T MMIAPICOM_FatData2Tm(
                        uint32 fatdata
                        )
{
    MMI_TM_T tm = {0};
    
    uint16 day = 0;
    uint16 time = 0;

    day=fatdata&0x0000ffff;
    time=(fatdata>>16)&0x0000ffff;

    tm.tm_mday=day&0x1f;
    tm.tm_mon=(day>>5)&0x0f;
    tm.tm_year=(day>>9)&0x007f;
    tm.tm_year=tm.tm_year+MMICOM_DATE_MIN_YEAR;
    
    tm.tm_sec=time&0x1f*2;
    tm.tm_min=(time>>5)&0x003f;
    tm.tm_hour=(time>>11)&0x1f;
    
    return tm;
}

/*****************************************************************************/
//  Description : to translate time to the formate
//  Global resource dependence : 
//  Author:louis.wei
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMIAPICOM_TranslateTime(
                          MN_SMS_TIME_STAMP_T   *time_stamp_ptr,    //IN:
                          uint8                 *time_ptr           //OUT:
                          )
{
    uint8   len = 0;
    
    if (PNULL == time_stamp_ptr ||PNULL == time_ptr)
    {
        return 0;
    }

    len = sprintf((char*)time_ptr, "%02d/%02d %02d:%02d",
                    time_stamp_ptr->month,
                    time_stamp_ptr->day,
                    time_stamp_ptr->hour,
                    time_stamp_ptr->minute);

    return (len);
}

/*****************************************************************************/
//  Description : get days from month,year
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL uint16 DaysPerMonth(uint8 month, uint16 year)
{
    if (month != 2)
    {
        return day_per_mon[month - 1];
    }
    else
    {
        return day_per_mon[1] + MMIAPICOM_IsLeapYear(year);
    }
}
/*****************************************************************************/
//  Description : turn second into tm struct
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL MMI_TM_T Second2Tm(uint32 second)
{
    MMI_TM_T tm = {0};
    int32 sec = 0;
    int32 day =0;
    uint16 y = 0;
    uint8 m = 0;
    uint16 d = 0;

    /* hour, min, sec */
    /* hour */
    sec = second % MMICOM_SEC_PER_DAY;
    tm.tm_hour = sec / MMICOM_SEC_PER_HOUR;

    /* min */
    sec %= MMICOM_SEC_PER_HOUR;
    tm.tm_min = sec / MMICOM_SEC_PER_MIN;

    /* sec */
    tm.tm_sec = sec % MMICOM_SEC_PER_MIN;

    /* year, month, day */
    /* year */
    /* year */
    day = second / MMICOM_SEC_PER_DAY;
    for (y = MMICOM_DATE_MIN_YEAR; day > 0; y++)
    {
        d = (MMICOM_DAY_PER_YEAR + MMIAPICOM_IsLeapYear(y));
        if (day >= d)
        {
            day -= d;
        }
        else
        {
            break;
        }
    }
    tm.tm_year = y;

    for (m = 1; m < MMICOM_DATE_MAX_MONTH; m++)
    {
        d = DaysPerMonth(m, y);
        if (day >= d)
        {
            day -= d;
        }
        else
        {
            break;
        }
    }
   
    tm.tm_mon = m;
    tm.tm_mday = (uint8)(day + 1);

    return tm;
}

/*****************************************************************************/
//  Description : turn second into tm struct
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC MMI_TM_T MMIAPICOM_Second2Tm(uint32 second)
{
    return Second2Tm(second);
}

/*****************************************************************************/
//    Description : check whether year is leap year
//    Global resource dependence : 
//    Author: liqing.peng
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICOM_IsLeapYear(uint32 year)
{
    return GUI_IsLeapYear(year);
}

/*****************************************************************************/
//    Description :  to translate the time to second
//    Global resource dependence : 
//    Author: 
//    Note:
/*****************************************************************************/
LOCAL uint32 Tm2Second(                      
                      uint32 tm_sec, 
                      uint32 tm_min, 
                      uint32 tm_hour,
                      uint32 tm_mday,
                      uint32 tm_mon, 
                      uint32 tm_year    
                      )
{
    uint16 i = 0;
    uint32 no_of_days = 0;
    uint32 utc_time = 0;

    //check if the date value range is valid
    if ( (tm_mday > 31) || (tm_mon > 12) 
        ||(tm_year < MMICOM_DATE_MIN_YEAR)
        ||(tm_mon < MONTH_START)
        )
    {
        //SCI_TRACE_LOW:"MMI_COMMON:tm_mday = %d,tm_mon = %d,tm_year = %dn"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICOM_TIME_340_112_2_18_1_59_18_131,(uint8*)"ddd",tm_mday ,tm_mon ,tm_year);
        return 0;
    }
    
    /* year */
    for (i = MMICOM_DATE_MIN_YEAR; i < tm_year; i++)
    {
        no_of_days += (MMICOM_DAY_PER_YEAR + MMIAPICOM_IsLeapYear(i));
    }

    if(MMIAPICOM_IsLeapYear(tm_year))
    {
        no_of_days += 
            dayfromjan1run[tm_mon-MONTH_START]
            +tm_mday - 1;
    }
    else
    {
        no_of_days += 
            dayfromjan1[tm_mon-MONTH_START]
            +tm_mday - 1;
    }

    /* sec */
    utc_time =
         no_of_days *MMICOM_SEC_PER_DAY +  tm_hour * MMICOM_SEC_PER_HOUR +
                                                                tm_min * MMICOM_SEC_PER_MIN + tm_sec;

    return utc_time;
}

/*****************************************************************************/
//  Description : to translate the time to second
//  Global resource dependence : none
//  Author:Louis wei
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIAPICOM_Tm2Second( 
                      uint32 tm_sec, 
                      uint32 tm_min, 
                      uint32 tm_hour,
                      uint32 tm_mday,
                      uint32 tm_mon, 
                      uint32 tm_year                      
                      )
{
    return Tm2Second(tm_sec,tm_min,tm_hour,tm_mday,tm_mon,tm_year);
}

/*****************************************************************************/
//  Description : count the lapsed year from 2000
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIAPICOM_DayToYear(uint32 *day_in)
{
    uint32 day = 0;
    uint32 year = 0;
    uint16 d = 0;
    day = *day_in;
    for (year = MMICOM_DATE_MIN_YEAR; day > 0; year++)
    {
        d = (MMICOM_DAY_PER_YEAR + MMIAPICOM_IsLeapYear(year));
        if (day >= d)
        {
            day -= d;
        }
        else
        {
            break;
        }
    }
    *day_in = day;
    return year;
}

/*****************************************************************************/
//  Description : count the date
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIAPICOM_DayToMonth( //return month
                       BOOLEAN leap_year,
                       uint32 *day
                       )
{
    uint8 month = 0;
    
    if(*day > 365 + leap_year || *day == 0)
    {
        //SCI_TRACE_MID:"MMI_COMMON:error day = %d\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICOM_TIME_438_112_2_18_1_59_18_133,(uint8*)"d",*day);
        return  1; // return month = 1
    }
    
    if(leap_year)
    {
        for(month = 1 ; month < 13; month++)
        {
            if(*day <= dayfromjan1run[month])
            {
                *day -= dayfromjan1run[month-1];
                break;
            }
        }
    }
    else
    {
        for(month = 1 ; month < 13; month++)
        {
            if(*day <= dayfromjan1[month])
            {
                *day -= dayfromjan1[month-1];
                break;
            }
        }
        
    }

/*
    if(month == 0)
    {
        //SCI_TRACE_MID:"MMI_COMMON:error month = %d\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICOM_TIME_468_112_2_18_1_59_18_134,(uint8*)"d",month);
        return 1;    //return month = 1 
    }
*/
    
    return month;
}

/*****************************************************************************/
//  Description : check the date if is valid
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICOM_CheckDateIsValid(
                                          MMISET_DATE_T            date    //the date
                                          )
{
    BOOLEAN     result = TRUE;
    
    //check year
    if ((MMICOM_DATE_MIN_YEAR > date.date_year) || (MMICOM_DATE_MAX_YEAR < date.date_year))
    {
        result = FALSE;
    }
    else if ((MMICOM_DATE_MAX_MONTH < date.date_month) || (0 == date.date_month))//check month
    {
        result = FALSE;
    }
    else if (0 == date.date_day)
    {
        result = FALSE;
    }
    else //check day
    {
        result = CheckDayOfDateIsValid(date);
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : check the time if is valid
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICOM_CheckTimeIsValid(
                                          MMISET_TIME_T       time        //the time
                                          )
{
    BOOLEAN     result = TRUE;
       
    //check the minute
    if ((MMICOM_TIME_MAX_MINUTE < time.time_minute))
    {
        result = FALSE;
    }
    else
    {
        //check the time of 24 Hour if is valid,check the hour
        if ((MMICOM_24HOUR_MAX_HOUR <= time.time_hour))
        {
            result = FALSE;
        }
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : check the date if is valid for calendar
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICOM_CheckDateIsValidForCalendar(
                                                     MMISET_DATE_T            date    //the date
                                                     )
{
    BOOLEAN     result = TRUE;
    
    //check year
    if ((MMICOM_DATE_MIN_YEAR_CALENDAR > date.date_year) || (MMICOM_DATE_MAX_YEAR_CALENDAR < date.date_year))
    {
        result = FALSE;
    }
    else if ((MMICOM_DATE_MAX_MONTH < date.date_month) || (0 == date.date_month))//check month
    {
        result = FALSE;
    }
    else if (0 == date.date_day)
    {
        result = FALSE;
    }
    else //check day
    {
        result = CheckDayOfDateIsValid(date);
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : get rtc time by string
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC MMIACC_RTC_TIME_T *MMIAPICOM_GetRTCTime( void )
{
    static  MMIACC_RTC_TIME_T time;
    SCI_TIME_T systime = {0};

    TM_GetSysTime(&systime);
    sprintf((char*)time, "%02d:%02d:%02d", systime.hour, systime.min,systime.sec);
    
    return( (MMIACC_RTC_TIME_T*)time );
}

/*****************************************************************************/
//  Description :   get current date by string
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC MMIACC_RTC_DATE_T *MMIAPICOM_GetRTCDate( void )
{
    static MMIACC_RTC_DATE_T date;
    SCI_DATE_T sysdate = {0};

    TM_GetSysDate(&sysdate);
    sprintf((char*)date, "%4d-%02d-%02d", sysdate.year, sysdate.mon, sysdate.mday);

    return( (MMIACC_RTC_DATE_T*)date );
}


/*****************************************************************************/
//  Description : Interrogate Status on background 
//  Global resource dependence : 
//  Author:shengjie.liu
//  Note:
/*****************************************************************************/
PUBLIC  uint32 MMIAPICOM_GetRandomTimer(void)
{
    uint16 random = 0;//随机数
    uint32 timeout = 0; // 定时器时长
    
    // 计算随机时间
    srand(SCI_GetTickCount()); // 设置随机数种子
    random = rand(); // 随机数
    timeout = random * 2 + 60000; // 时间在1分钟到3分钟之间
    
    //SCI_TRACE_LOW:"GetRandomTimer return timer = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICOM_TIME_610_112_2_18_1_59_18_135,(uint8*)"d", timeout);
    
    return timeout;
}

/*****************************************************************************/
//  Description : 毫秒到秒的转换
//  Global resource dependence : none
//  Author: 
//  Note:补足一秒
/*****************************************************************************/
PUBLIC uint32 MMIAPICOM_GetSecondFromMillisecond(uint32 millisecond)
{
	uint32 second = 0;

    second = millisecond / 1000;
    if(millisecond % 1000 >=500)
    {
        second++;
    }

    return second;
}

/*******************************************************************/
//  Interface:		MMIAPICOM_SetSysTime
//  Description : 	MMIAPICOM_SetSysTime
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
PUBLIC BOOLEAN MMIAPICOM_SetSysTime(
									SCI_TIME_T  sys_time
									)
{
    uint32 old_time = 0;
    uint32 new_time = 0;
	ERR_TM_E err_tm = ERR_TM_NONE;
	
	old_time = MMIAPICOM_GetCurTime();
	
	err_tm = TM_SetSysTime(sys_time);
	if (ERR_TM_NONE != err_tm)
	{
		return FALSE;
	}
	
#ifdef SFR_SUPPORT
    MMISFR_CheckEvent();
#endif
	new_time = MMIAPICOM_GetCurTime();
	
	return MMIFW_PumpTimerUpdateEvent(old_time, new_time);
}

/*******************************************************************/
//  Interface:		MMIAPICOM_SetSysData
//  Description : 	MMIAPICOM_SetSysData
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
PUBLIC BOOLEAN MMIAPICOM_SetSysData(
									SCI_DATE_T  sys_date
									)
{
	uint32 old_time = 0;
    uint32 new_time = 0;
	ERR_TM_E err_tm = ERR_TM_NONE;
	
	old_time = MMIAPICOM_GetCurTime();
	
	err_tm = TM_SetSysDate(sys_date);
	if (ERR_TM_NONE != err_tm)
	{
		return FALSE;
	}
	
#ifdef SFR_SUPPORT
    MMISFR_CheckEvent();
#endif
	new_time = MMIAPICOM_GetCurTime();
	
	return MMIFW_PumpTimerUpdateEvent(old_time, new_time);
}
