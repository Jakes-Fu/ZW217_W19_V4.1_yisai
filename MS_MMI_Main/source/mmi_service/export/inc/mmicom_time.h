/****************************************************************************
** File Name:      mmicom_time.h                                           *
** Author:                                                                 *
** Date:           1/4/2008                                              *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.       *
** Description:    This file is used to describe the data struct of        *
**                 system, applicaiton, window and control                 *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 4/2008        jian.ma       Create
** 
****************************************************************************/
#ifndef MMICOM_TIME_H
#define MMICOM_TIME_H

/*-------------------------------------------------------------------------*/
/*                         DEPENDANCY                                      */
/*-------------------------------------------------------------------------*/


#include "sci_types.h"
#include "mn_type.h"
#include "sfs.h"
#include "dal_time.h"

/**---------------------------------------------------------------------------*
 **                         Macro Declaration                                 *
 **---------------------------------------------------------------------------*/
#define     MMICOM_DATE_MAX_YEAR                2099   //时间设置最大年份
#define     MMICOM_DATE_MIN_YEAR                1980   //时间设置最小年份
#define     MMICOM_SEC_PER_DAY                  86400
#define     MMICOM_SEC_PER_HOUR                 3600
#define     MMICOM_SEC_PER_MIN                  60
#define     MMICOM_DAY_PER_YEAR                 365

#define     MMICOM_DATE_MAX_YEAR_CALENDAR       2100   //日历阴历查询最大年份
#define     MMICOM_DATE_MIN_YEAR_CALENDAR       1900   //日历阴历查询最小年份

#define     MMICOM_QUERY_MIN_YEAR               1950    //可查询的最早年份
#define     MMICOM_QUERY_MAX_YEAR               2099    //可查询的最晚年份

#define   MMICOM_FIRSTDAY_WDAY  2//系统起始日期的星期值(eg，1980.1.1是星期二)

#define     MMICOM_DATE_MAX_MONTH               12
#define     MMICOM_DATE_BIG_MONTH_DAY           31
#define     MMICOM_DATE_SMALL_MONTH_DAY         30
#define     MMICOM_DATE_LEAP_MONTH_DAY          29
#define     MMICOM_DATE_NOT_LEAP_MONTH_DAY      28
#define     MMICOM_24HOUR_MAX_HOUR              24
#define     MMICOM_TIME_MAX_MINUTE              59

#define     MMICOM_RTC_DATE_STRING_LENGTH               10      // XXXX-XX-XX
#define     MMICOM_RTC_TIME_STRING_LENGTH               8       // XX:XX:XX

#define     MILLISECONDS_TO_SECONDS(count)              ((count) / 1000)
/*-------------------------------------------------------------------------*/
/*                         TYPES/CONSTANTS                                 */
/*-------------------------------------------------------------------------*/
typedef struct 
{
    uint8 tm_sec;     // secondsafter the minute - [0,59] 
    uint8 tm_min;     // minutesafter the hour - [0,59] 
    uint8 tm_hour;    // hours since midnight - [0,23] 
    uint8 tm_mday;    // day of the month - [1,31] 
    uint8 tm_mon;     // months since January - [0,11] 
    int   tm_year;    // years since 1980
    uint8 tm_wday;    // days since Sunday - [0,6] 
    int   tm_yday;    // days since January 1 - [0,365] 
    int   tm_isdst;   // daylight savings time flag 
}MMI_TM_T;

typedef enum
{
    MMISET_DATE_JAN = 1,    //January
        MMISET_DATE_FEB,    //February
        MMISET_DATE_MAR,    //March
        MMISET_DATE_APR,    //April
        MMISET_DATE_MAY,    //May
        MMISET_DATE_JUN,    //June
        MMISET_DATE_JUL,    //July
        MMISET_DATE_AUG,    //August
        MMISET_DATE_SEP,    //September
        MMISET_DATE_OCT,    //October
        MMISET_DATE_NOV,    //November
        MMISET_DATE_DEC     //December
} MMICOM_DATE_MONTH_E;

//date的信息，包括年，月，日
typedef struct
{
    uint8   date_day;   // day of the month
    uint8   date_month; // month
    uint16  date_year;  //year
}MMISET_DATE_T;


//time的信息，包括时，分
typedef struct
{
    uint8  time_hour;  //hour
    uint8  time_minute;//minute
} MMISET_TIME_T;

typedef uint8 MMIACC_RTC_DATE_T[MMICOM_RTC_DATE_STRING_LENGTH + 1];
typedef uint8 MMIACC_RTC_TIME_T[MMICOM_RTC_TIME_STRING_LENGTH + 1];
/*-------------------------------------------------------------------------*/
/*                         FUNCTIONS                                       */
/*-------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : to translate time to the formate
//  Global resource dependence : 
//  Author:louis.wei
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMIAPICOM_TranslateTime(
                          MN_SMS_TIME_STAMP_T   *time_stamp_ptr,    //IN:
                          uint8                 *time_ptr           //OUT:
                          );
                    
/*****************************************************************************/
//  Description : convert SFS time to uint32
//  Global resource dependence : 
//  Author:liqing.peng
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMIAPICOM_TansferSFSTime(
                          SFS_DATE_T    date,
                          SFS_TIME_T    time
                          );

/*****************************************************************************/
//  Description : divert fat file system time to tm structure
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC MMI_TM_T MMIAPICOM_FatData2Tm(
                        uint32 fatdata
                        );
                     

/*****************************************************************************/
//  Description :to get the current time 
//  Global resource dependence : none
//  Author:louis.wei
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIAPICOM_GetCurTime(void);

/*****************************************************************************/
//  Description : Is it leap year
//  Global resource dependence : none
//  Author:Louis wei
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICOM_IsLeapYear(uint32 year);

/*****************************************************************************/
//  Description : 获得文件系统当前的系统时间
//  Global resource dependence : 
//  Author: Jassmine.Meng
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMIAPICOM_GetFfsTime(void);

/*****************************************************************************/
//  Description : turn second into tm struct
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC MMI_TM_T MMIAPICOM_Second2Tm(uint32 second);

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
                      );

/*****************************************************************************/
//  Description : check the date if is valid
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICOM_CheckDateIsValid(
                                          MMISET_DATE_T            date    //the date
                                          );



/*****************************************************************************/
//  Description : check the date if is valid for calendar
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICOM_CheckDateIsValidForCalendar(
                                                     MMISET_DATE_T            date    //the date
                                                     );

/*****************************************************************************/
//  Description : check the time if is valid
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICOM_CheckTimeIsValid(
                                          MMISET_TIME_T       time        //the time
                                          );


/*****************************************************************************/
//  Description : get rtc time by string
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC MMIACC_RTC_TIME_T *MMIAPICOM_GetRTCTime( void );

/*****************************************************************************/
//  Description :   get current date by string
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC MMIACC_RTC_DATE_T *MMIAPICOM_GetRTCDate( void );

/*****************************************************************************/
//  Description : Interrogate Status on background 
//  Global resource dependence : 
//  Author:shengjie.liu
//  Note:
/*****************************************************************************/
PUBLIC  uint32 MMIAPICOM_GetRandomTimer(void);


/*****************************************************************************/
//  Description : count the lapsed year from 2000
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIAPICOM_DayToYear(uint32 *day_in);

/*****************************************************************************/
//  Description : count the date
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIAPICOM_DayToMonth( //return month
                       BOOLEAN leap_year,
                       uint32 *day
                       );

/*****************************************************************************/
//  Description : 毫秒到秒的转换
//  Global resource dependence : none
//  Author: 
//  Note:做四舍五入
/*****************************************************************************/
PUBLIC uint32 MMIAPICOM_GetSecondFromMillisecond(uint32 millisecond);

/*******************************************************************/
//  Interface:		MMIAPICOM_SetSysTime
//  Description : 	MMIAPICOM_SetSysTime
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
PUBLIC BOOLEAN MMIAPICOM_SetSysTime(
									SCI_TIME_T  sys_time
									);

/*******************************************************************/
//  Interface:		MMIAPICOM_SetSysData
//  Description : 	MMIAPICOM_SetSysData
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
PUBLIC BOOLEAN MMIAPICOM_SetSysData(
									SCI_DATE_T  sys_date
									);

#endif  /* FILENAME_H */
