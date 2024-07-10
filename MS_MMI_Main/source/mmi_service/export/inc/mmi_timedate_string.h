/****************************************************************************
** File Name:      mmi_timedate_string.h                                    *
** Author:         hongbing.ju                                              *
** Date:           27/07/2012                                               *
** Copyright:      2008 Spreadtrum, Incorporated. All Rights Reserved.      *
** Description:    This file is used to describe the time and date string.  *
*****************************************************************************
**                         Important Edit History                           *
** -------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                              *
** 7/2012        hongbing.ju        Create                                  *
**                                                                          *
****************************************************************************/
/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#ifndef _MMI_TIMEDATE_STRING_H__
#define _MMI_TIMEDATE_STRING_H__

#include "mmk_type.h"
#include "caf.h"

#define TIME_MAX_SLOT   4

#ifdef LONG_DATE_FORMAT_SUPPORT
#define DATE_MAX_SLOT   4
#else
#define DATE_MAX_SLOT   3
#endif

#define MMI_TIMEDATE_ERROR_NUMBER(err)  ((err) & 0xFF)
#define MMI_TIMEDATE_ERROR_LOCATION(err)(((err) & 0xFFFFFF00) >> 8)

enum
{
    MMI_TIMEDATE_ERROR_INVALID_PARAM = 1,
    MMI_TIMEDATE_ERROR_OVERFLOW,
    MMI_TIMEDATE_ERROR_OUT_OF_MEMORY
};

typedef enum 
{
    TIME_NONE,
    TIME_HH,
    TIME_XH,
    TIME_hh,
    TIME_MM,
    TIME_XM,
    TIME_mm,
    TIME_SS,
    TIME_XS,
    TIME_ss,
    TIME_AMPM,
    TIME_ampm,
    TIME_MAX_FORMAT
} TIME_FORMAT_T;

typedef enum
{
    HOUR_ONLY,
    MINUTE_ONLY,
    SECOND_ONLY,
    AMPM_ONLY,
    MINUTE_SECOND,
    HOUR_MINUTE,
    HOUR_AMPM,
    AMPM_HOUR,
    HOUR_MINUTE_SECOND,
    HOUR_MINUTE_AMPM,
    AMPM_HOUR_MINUTE,
    HOUR_MINUTE_SECOND_AMPM,
    AMPM_HOUR_MINUTE_SECOND
} TIME_STRING_ORDER_T;

typedef struct TIME_STRING_T_* TIMESTRING_HANDLE_T;

typedef enum
{
    DATE_NONE,
    DATE_DD,            //0-filled 2-digit mday, like 03, 15
    DATE_XD,            //normal digits for mday, like 3, 15
    DATE_dd,            //full word for month day name, for arabic
    DATE_xd,            //abbreviated name, like 1st
    DATE_ww,            //full word for week day name, like Monday
    DATE_xw,            //abbreviated word for week day name, like Mon 
    DATE_MM,            //0-filled 2-digit month, like 03, 11
    DATE_mm,            //full word for month name, like April
    DATE_XM,            //normal digits for month, like 3, 11
    DATE_xm,            //abbreviated word for month name, like Jan 
    DATE_YY,            //0-filled 2-digit year, like 08, 11
    DATE_YYYY,          //0-filled 4-digit year, like 2008,1995
    DATE_yyyy,          //full word for year, for arabic
    DATE_MAX_FORMAT
} DATE_FORMAT_T;

typedef enum
{
    MDAY_ONLY,
    WDAY_ONLY,
    MONTH_ONLY,
    YEAR_ONLY,
    MDAY_MONTH,
    MONTH_MDAY,
    MONTH_YEAR,
    YEAR_MONTH,
    WDAY_MDAY_MONTH,
    MONTH_MDAY_WDAY,
    MDAY_MONTH_YEAR,
    MONTH_MDAY_YEAR,
    YEAR_MDAY_MONTH,
    YEAR_MONTH_MDAY,
    WDAY_MDAY_MONTH_YEAR,
    WDAY_MONTH_MDAY_YEAR,
    YEAR_MDAY_MONTH_WDAY,
    YEAR_MONTH_MDAY_WDAY
} DATE_STRING_ORDER_T;

typedef struct DATE_STRING_T_* DATESTRING_HANDLE_T;

/*******************************************************************************************/
//     Description : create default time format
//    Global resource dependence :
//  Author: hongbing.ju
//    Note:
/*********************************************************************************************/
PUBLIC TIMESTRING_HANDLE_T MMI_CreateDefaultTimeFormat(void);

/*******************************************************************************************/
//     Description : create time format
//    Global resource dependence :
//  Author: hongbing.ju
//    Note:
/*********************************************************************************************/
PUBLIC TIMESTRING_HANDLE_T MMI_CreateTimeFormat(
            TIME_FORMAT_T*      time_format,
            wchar*              delimiter,   
            TIME_STRING_ORDER_T time_string_order
            );

/*******************************************************************************************/
//     Description : destroy time string
//    Global resource dependence :
//  Author: hongbing.ju
//    Note:
/*********************************************************************************************/
PUBLIC void MMI_DestroyTimeString(TIMESTRING_HANDLE_T* time_string);

/*******************************************************************************************/
//     Description : get time string default delimiter depending on language setting
//    Global resource dependence :
//  Author: hongbing.ju
//    Note:
/*********************************************************************************************/
PUBLIC void MMI_GetTimeStringDefaultDelimiter(wchar* delimiter);

/*******************************************************************************************/
//     Description : set time format delimiter
//    Global resource dependence :
//  Author: hongbing.ju
//    Note:
/*********************************************************************************************/
PUBLIC void MMI_SetTimeStringDelimiter(TIMESTRING_HANDLE_T time_string, wchar* delimiter);

/*******************************************************************************************/
//     Description : set time string format according to time string order, 
//    Global resource dependence :
//  Author: hongbing.ju
//    Note:
/*********************************************************************************************/
PUBLIC void MMI_SetTimeStringFormat(
        TIMESTRING_HANDLE_T time_string, 
        TIME_FORMAT_T*      time_format,
        TIME_STRING_ORDER_T time_string_order
        );
/*******************************************************************************************/
//     Description : Get time string according to time format
//    Global resource dependence :
//  Author: hongbing.ju
//    Note:
/*********************************************************************************************/
PUBLIC wchar* MMI_GetCurrTimeString(TIMESTRING_HANDLE_T time_string);

/*******************************************************************************************/
//     Description : Get time string according to time format
//    Global resource dependence :
//  Author: hongbing.ju
//    Note:
/*********************************************************************************************/
PUBLIC wchar* MMI_GetTimeString(TIMESTRING_HANDLE_T time_string, SCI_TIME_T  sys_time);

/*******************************************************************************************/
//     Description : get time text string 
//    Global resource dependence :
//  Author: hongbing.ju
//    Note:
/*********************************************************************************************/
PUBLIC uint16 MMI_TIME_GetTextString(wchar* buf, TIME_FORMAT_T time_format, uint16 value);

/*******************************************************************************************/
//     Description : create default date format
//    Global resource dependence :
//  Author: hongbing.ju
//    Note:
/*********************************************************************************************/
PUBLIC DATESTRING_HANDLE_T MMI_CreateDefaultDateFormat(void);

/*******************************************************************************************/
//     Description : create date format
//    Global resource dependence :
//  Author: hongbing.ju
//    Note:
/*********************************************************************************************/
PUBLIC DATESTRING_HANDLE_T MMI_CreateDateFormat(
    DATE_FORMAT_T*      date_format,
    wchar*              delimiter,
    DATE_STRING_ORDER_T date_string_order
    );

/*******************************************************************************************/
//     Description : destroy date string
//    Global resource dependence :
//  Author: hongbing.ju
//    Note:
/*********************************************************************************************/
PUBLIC void MMI_DestroyDateString(DATESTRING_HANDLE_T* date_string);

/*******************************************************************************************/
//     Description : set date string format
//    Global resource dependence :
//  Author: hongbing.ju
//    Note:
/*********************************************************************************************/
PUBLIC void MMI_SetDateStringFormat(
    DATESTRING_HANDLE_T date_string, 
    DATE_FORMAT_T* date_format,
    DATE_STRING_ORDER_T date_string_order
    );

/*******************************************************************************************/
//     Description : set date string delimiter
//    Global resource dependence :
//  Author: hongbing.ju
//    Note:
/*********************************************************************************************/
PUBLIC void MMI_SetDateStringDelimiter(DATESTRING_HANDLE_T date_string, wchar* delimiter);

/*******************************************************************************************/
//     Description : get date string default delimiter depending on language setting
//    Global resource dependence :
//  Author: hongbing.ju
//    Note:
/*********************************************************************************************/
PUBLIC void MMI_GetDateStringDefaultDelimiter(wchar* delimiter);

/*******************************************************************************************/
//     Description : get date string according to its format
//    Global resource dependence :
//  Author: hongbing.ju
//    Note:
/*********************************************************************************************/
PUBLIC wchar* MMI_GetCurrDateString(DATESTRING_HANDLE_T date_string);

/*******************************************************************************************/
//     Description : get date string according to its format
//    Global resource dependence :
//  Author: hongbing.ju
//    Note:
/*********************************************************************************************/
PUBLIC wchar* MMI_GetDateString(DATESTRING_HANDLE_T date_string, SCI_DATE_T  sys_date);

/*******************************************************************************************/
//     Description : get date text string 
//    Global resource dependence :
//  Author: hongbing.ju
//    Note:
/*********************************************************************************************/
PUBLIC uint16 MMI_DATE_GetTextString(wchar* buf, DATE_FORMAT_T date_format, uint16 value);
#endif  // end of _MMI_TIMEDATE_STRING_H__