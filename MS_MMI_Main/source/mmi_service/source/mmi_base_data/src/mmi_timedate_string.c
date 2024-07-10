/****************************************************************************
** File Name:      mmi_timedate_string.c                                    *
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
#include "mmi_app_common_trc.h"
#include "mmk_type.h"
#include "dal_time.h"
#include "mmi_string.h"
#include "mmisrv_adapt.h"
#include "mmi_timedate_string.h"

//#define LONG_DATE_FORMAT_SUPPORT

#ifdef WIN32
#define SCI_TRACE_LOW(a) printf((a));printf("\n")
#endif

#define MMI_TIME_PM_HOUR            12
#define MMI_TIME_1_HOUR             1
#define MMI_TIME_13_HOUR            13        

#define TIME_HOUR_MAX               23
#define TIME_MIN_MAX                59
#define TIME_SEC_MAX                59
#define DATE_MON_MAX                11
#define DATE_MDAY_MAX               30
#define DATE_WDAY_MAX               6
#define TIMEDATE_MIN                0

#define YEAR_CENTURY                100

#define TIMEDATE_MAX_BUF_SIZE       64
#define TIMEDATE_DELIMITER_MAX_LEN  4

#define MMI_TIMEDATE_ERROR_MASK(err) ((err) | __LINE__ << 8)

typedef struct TIME_STRING_FORMAT_T_
{
    TIME_FORMAT_T           time_format[TIME_MAX_SLOT];
    BOOLEAN                 is_ampm;
    uint8                   time_slots;
    TIME_STRING_ORDER_T     time_string_order;
} TIME_STRING_FORMAT_T;

#define MMI_SET_TIMESTRING_FORMAT(time_string_format, i, value) ((time_string_format)->time_format[i] = (value))

typedef struct TIME_STRING_T_
{
    TIME_STRING_FORMAT_T    time_string_format;
    wchar                   wstr_buf[TIMEDATE_MAX_BUF_SIZE + 1];
    wchar                   delimiter[TIMEDATE_DELIMITER_MAX_LEN];
    wchar                   ampm_delimiter[TIMEDATE_DELIMITER_MAX_LEN];
    uint32                  error;
} TIME_STRING_T;

typedef struct TIME_DATA_T_
{
    SCI_TIME_T              sys_time;
    BOOLEAN                 is_am;
} TIME_DATA_T;

typedef struct DATE_STRING_FORMAT_T_
{
    DATE_FORMAT_T           date_format[DATE_MAX_SLOT];
    uint8                   date_slots;
    DATE_STRING_ORDER_T     date_string_order;
} DATE_STRING_FORMAT_T;

#define MMI_SET_DATESTRING_FORMAT(date_string_format, i, value) ((date_string_format)->date_format[i] = (value))

typedef struct DATE_STRING_T_
{
    DATE_STRING_FORMAT_T    date_string_format;
    wchar                   wstr_buf[TIMEDATE_MAX_BUF_SIZE + 1];
    wchar                   delimiter[TIMEDATE_DELIMITER_MAX_LEN];
#ifdef LONG_DATE_FORMAT_SUPPORT
    wchar                   wday_delimiter[TIMEDATE_DELIMITER_MAX_LEN];
#endif
    uint32                  error;
} DATE_STRING_T; 

/*******************************************************************************************/
//     Description : convert interger to wchar string 
//    Global resource dependence :
//  Author: hongbing.ju
//    Note:
/*********************************************************************************************/
LOCAL uint16 my_itow(wchar* buf, uint16 count, uint32 value, BOOLEAN is_zero_fill);

/*******************************************************************************************/
//     Description : set time string format according to time string order, 
//    Global resource dependence :
//  Author: hongbing.ju
//    Note:
/*********************************************************************************************/
LOCAL void SetTimeStringFormat(TIMESTRING_HANDLE_T time_string, TIME_FORMAT_T *time_format, TIME_STRING_ORDER_T time_string_order);

/*******************************************************************************************/
//     Description : set default time format according to current language setting, 
//    Global resource dependence :
//  Author: hongbing.ju
//    Note:
/*********************************************************************************************/
LOCAL void SetDefaultTimeFormat(TIMESTRING_HANDLE_T time_string);

/*******************************************************************************************/
//     Description : set default AMPM delimiter 
//    Global resource dependence :
//  Author: hongbing.ju
//    Note:
/*********************************************************************************************/
LOCAL void SetTimeStringAmpmDelimiter(TIMESTRING_HANDLE_T time_string);

/*******************************************************************************************/
//     Description : set default date string delimiter according to langauge setting
//    Global resource dependence :
//  Author: hongbing.ju
//    Note:
/*********************************************************************************************/
LOCAL void SetDefaultTimeStringDelimiter(TIMESTRING_HANDLE_T time_string, MMISET_LANGUAGE_TYPE_E  language_type);

/*******************************************************************************************/
//     Description : set default date format according to current language setting 
//    Global resource dependence :
//  Author: hongbing.ju
//    Note:
/*********************************************************************************************/
LOCAL void SetDefaultDateFormat(DATESTRING_HANDLE_T date_string);

#ifdef STANDALONE_TEST
#include "mmi_standalone_patch.c"
#endif

/*******************************************************************************************/
//     Description : set default date format according to current language setting 
//    Global resource dependence :
//  Author: hongbing.ju
//    Note:
/*********************************************************************************************/
LOCAL uint16 my_itow(wchar* buf, uint16 count, uint32 value, BOOLEAN is_zero_fill)
{
    int32 i = 0;

    if(is_zero_fill)
    {
        i = count;
    }
    else
    {
        while(value)
        {
            i++;
            value /= 10;
        }

    }

    count = i;
    while (value)
    {
        buf[--count] = value % 10 + L'0';
        value /= 10;
    }

    if(is_zero_fill)
    {
        while (count)
        {
            buf[--count] = L'0';
        }
    }
    return i;
}

/*******************************************************************************************/
//     Description : create default time format
//    Global resource dependence :
//  Author: hongbing.ju
//    Note:
/*********************************************************************************************/
PUBLIC TIMESTRING_HANDLE_T MMI_CreateDefaultTimeFormat(void)
{
    TIMESTRING_HANDLE_T time_string = SCI_ALLOCA(sizeof(TIME_STRING_T));
    
    if(PNULL != time_string)
    {
    	SCI_TRACE_LOW("MMI_CreateDefaultTimeFormat");
        SCI_MEMSET(time_string, 0x00, sizeof(TIME_STRING_T));
        SetDefaultTimeFormat(time_string);
    }
    return time_string;
}

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
        )
{
    TIMESTRING_HANDLE_T time_string = SCI_ALLOCA(sizeof(TIME_STRING_T));
    
    if(PNULL != time_string)
    {
    	SCI_TRACE_LOW("MMI_CreateTimeFormat: time_format = %x delimiter = %s order = %d", (uint32)time_format, delimiter, time_string_order);
        SCI_MEMSET(time_string, 0x00, sizeof(TIME_STRING_T));
        SetTimeStringFormat(
                time_string, 
                time_format,
                time_string_order
        );
        MMI_SetTimeStringDelimiter(time_string, delimiter);
    }
    return time_string;
}

/*******************************************************************************************/
//     Description : destroy time string
//    Global resource dependence :
//  Author: hongbing.ju
//    Note:
/*********************************************************************************************/
PUBLIC void MMI_DestroyTimeString(TIMESTRING_HANDLE_T* time_string)
{
    MMIBD_ASSERT_DEBUG(time_string && *time_string);

    if(PNULL != time_string && PNULL != *time_string)
    {
        SCI_FREE(*time_string);
        *time_string = PNULL;
    }
}

PUBLIC void MMI_SetTimeStringDelimiter(TIMESTRING_HANDLE_T time_string, wchar* delimiter)
{
    MMIBD_ASSERT_DEBUG(time_string && delimiter);

    if(PNULL != time_string && PNULL != delimiter)
    {
        time_string->delimiter[0] = delimiter[0];
        time_string->delimiter[1] = 0;
        time_string->ampm_delimiter[0] = ' ';
        time_string->ampm_delimiter[1] = 0;
    }
}

/*******************************************************************************************/
//     Description : set time string format according to time string order, 
//    Global resource dependence :
//  Author: hongbing.ju
//    Note:
/*********************************************************************************************/
LOCAL void SetTimeStringFormat(TIMESTRING_HANDLE_T time_string, TIME_FORMAT_T *time_format, TIME_STRING_ORDER_T time_string_order)
{
    TIME_STRING_FORMAT_T*   time_string_format = PNULL;
   
    MMIBD_ASSERT_DEBUG(time_string && time_format );

    if(PNULL != time_string && PNULL != time_format)
    {
        SCI_TRACE_LOW("SetTimeStringFormat: time_format = %x order = %d", (uint32)time_format, time_string_order);

        time_string_format = &time_string->time_string_format;
        time_string_format->is_ampm = FALSE;
        switch(time_string_order)
        {
        case HOUR_ONLY:
        case MINUTE_ONLY:
        case SECOND_ONLY:
        case AMPM_ONLY:
            MMI_SET_TIMESTRING_FORMAT(time_string_format, 0,time_format[0]);
            MMI_SET_TIMESTRING_FORMAT(time_string_format, 1,TIME_NONE);
            MMI_SET_TIMESTRING_FORMAT(time_string_format, 2,TIME_NONE);
            MMI_SET_TIMESTRING_FORMAT(time_string_format, 3,TIME_NONE);
            time_string_format->time_slots = 1;
            break;
        case HOUR_AMPM:
        case AMPM_HOUR:
            time_string_format->is_ampm = TRUE;
            MMI_SET_TIMESTRING_FORMAT(time_string_format, 0,time_format[0]);
            MMI_SET_TIMESTRING_FORMAT(time_string_format, 1,time_format[1]);
            MMI_SET_TIMESTRING_FORMAT(time_string_format, 2,TIME_NONE);
            MMI_SET_TIMESTRING_FORMAT(time_string_format, 3,TIME_NONE);
            time_string_format->time_slots = 2;
            break;
        case MINUTE_SECOND:
        case HOUR_MINUTE:
            MMI_SET_TIMESTRING_FORMAT(time_string_format, 0,time_format[0]);
            MMI_SET_TIMESTRING_FORMAT(time_string_format, 1,time_format[1]);
            MMI_SET_TIMESTRING_FORMAT(time_string_format, 2,TIME_NONE);
            MMI_SET_TIMESTRING_FORMAT(time_string_format, 3,TIME_NONE);
            time_string_format->time_slots = 2;
            break;
        case HOUR_MINUTE_AMPM:
        case AMPM_HOUR_MINUTE:
            time_string_format->is_ampm = TRUE;
            MMI_SET_TIMESTRING_FORMAT(time_string_format, 0,time_format[0]);
            MMI_SET_TIMESTRING_FORMAT(time_string_format, 1,time_format[1]);
            MMI_SET_TIMESTRING_FORMAT(time_string_format, 2,time_format[2]);
            MMI_SET_TIMESTRING_FORMAT(time_string_format, 3,TIME_NONE);
            time_string_format->time_slots = 3;
            break;
        case HOUR_MINUTE_SECOND:
            MMI_SET_TIMESTRING_FORMAT(time_string_format, 0,time_format[0]);
            MMI_SET_TIMESTRING_FORMAT(time_string_format, 1,time_format[1]);
            MMI_SET_TIMESTRING_FORMAT(time_string_format, 2,time_format[2]);
            MMI_SET_TIMESTRING_FORMAT(time_string_format, 3,TIME_NONE);
            time_string_format->time_slots = 3;
            break;
        case HOUR_MINUTE_SECOND_AMPM:
        case AMPM_HOUR_MINUTE_SECOND:
            time_string_format->is_ampm = TRUE;
            MMI_SET_TIMESTRING_FORMAT(time_string_format, 0,time_format[0]);
            MMI_SET_TIMESTRING_FORMAT(time_string_format, 1,time_format[1]);
            MMI_SET_TIMESTRING_FORMAT(time_string_format, 2,time_format[2]);
            MMI_SET_TIMESTRING_FORMAT(time_string_format, 3,time_format[3]);
            time_string_format->time_slots = 4;
            break;
        default:
            SCI_TRACE_LOW("SetTimeStringFormat: case no need to process here");
            break;
        }
        time_string_format->time_string_order = time_string_order;
    }
}

/*******************************************************************************************/
//     Description : set time slot value according to time format 
//    Global resource dependence :
//  Author: hongbing.ju
//    Note:
/*********************************************************************************************/
LOCAL void SetTimeStringValue(TIMESTRING_HANDLE_T time_string, uint8 *time_value, TIME_DATA_T time_data)
{
    TIME_STRING_FORMAT_T*   time_string_format = PNULL;
    uint32  i = 0;

    MMIBD_ASSERT_DEBUG(time_string && time_value );

    if(PNULL != time_string && PNULL != time_value)
    {
        SCI_TRACE_LOW("SetTimeStringValue: time_value = %x hour = %d min = %d sec = %d", (uint32)time_value, time_data.sys_time.hour, time_data.sys_time.min, time_data.sys_time.sec);

        time_string_format = &time_string->time_string_format;

        for(i = 0; i < time_string_format->time_slots; i++)
        {
            switch (time_string_format->time_format[i])
            {
            case TIME_HH:
            case TIME_XH:
            case TIME_hh:
                time_value[i] = time_data.sys_time.hour;
                break;
            case TIME_MM:
            case TIME_XM:
            case TIME_mm:
                time_value[i] = time_data.sys_time.min;
                break;
            case TIME_SS:
            case TIME_XS:
            case TIME_ss:
                time_value[i] = time_data.sys_time.sec;
                break;
            case TIME_AMPM:
            case TIME_ampm:
                time_value[i] = time_data.is_am;
                break;
            default:
                SCI_TRACE_LOW("SetTimeStringValue: case no need to process here");
                break;
            }
        }
    }
}

/*******************************************************************************************/
//     Description : set default date string delimiter according to langauge setting
//    Global resource dependence :
//  Author: hongbing.ju
//    Note:
/*********************************************************************************************/
LOCAL void SetDefaultTimeStringDelimiter(TIMESTRING_HANDLE_T time_string, MMISET_LANGUAGE_TYPE_E  language_type)
{
    MMIBD_ASSERT_DEBUG(time_string);

    if(PNULL != time_string)
    {
        SCI_TRACE_LOW("SetDefaultTimeStringDelimiter: language = %d", language_type);

        time_string->ampm_delimiter[1] = 0;
        time_string->ampm_delimiter[0] = ' ';
        time_string->delimiter[1] = 0;
        switch(language_type)
        {
        #ifdef IM_MALAYALAM_SUPPORT
        case MMISET_LANGUAGE_MALAYALAM:
            time_string->delimiter[0] = '.';
            break;
        #endif
        default:
            time_string->delimiter[0] = ':';
            break;
        }
    }
}

LOCAL void SetTimeStringAmpmDelimiter(TIMESTRING_HANDLE_T time_string)
{
    MMIBD_ASSERT_DEBUG(time_string);

    if(PNULL != time_string)
    {
        time_string->ampm_delimiter[1] = 0;
        time_string->ampm_delimiter[0] = ' ';
    }
}

/*******************************************************************************************/
//     Description : get time string default delimiter depending on language setting
//    Global resource dependence :
//  Author: hongbing.ju
//    Note:
/*********************************************************************************************/
PUBLIC void MMI_GetTimeStringDefaultDelimiter(wchar* delimiter)
{
    if(PNULL != delimiter)
    {
        MMISET_LANGUAGE_TYPE_E  language_type = MMISET_LANGUAGE_ENGLISH;

        MMIAPISET_GetLanguageType(&language_type);

        delimiter[1] = 0;
        switch(language_type)
        {
        #ifdef IM_MALAYALAM_SUPPORT
        case MMISET_LANGUAGE_MALAYALAM:
            delimiter[0] = '.';
            break;
        #endif
        default:
            delimiter[0] = ':';
            break;
        }
    }
}

/*******************************************************************************************/
//     Description : set default date string format according to language setting
//    Global resource dependence :
//  Author: hongbing.ju
//    Note:
/*********************************************************************************************/
LOCAL void SetDefaultTimeStringFormat(TIMESTRING_HANDLE_T time_string, MMISET_LANGUAGE_TYPE_E  language_type)
{
    TIME_FORMAT_T time_format[4] = {0};
    TIME_STRING_ORDER_T time_string_order = HOUR_MINUTE_SECOND;

    SCI_TRACE_LOW("SetDefaultTimeStringFormat: language = %d", language_type);

    switch(language_type)
    {
    case MMISET_LANGUAGE_ENGLISH:
#ifdef IM_GREEK_SUPPORT
    case MMISET_LANGUAGE_GREEK:
#endif
#ifdef IM_VIETNAMESE_SUPPORT
    case MMISET_LANGUAGE_VIETNAMESE:
#endif
        time_format[0] = TIME_XH;
        time_format[1] = TIME_MM;
        time_format[2] = TIME_SS;
        time_format[3] = TIME_ampm;
        time_string_order = HOUR_MINUTE_SECOND_AMPM;
        break;
#ifdef IM_SWAHILI_SUPPORT
    case MMISET_LANGUAGE_SWAHILI:
#endif
#ifdef IM_FILIPINO_SUPPORT
    case MMISET_LANGUAGE_FILIPINO:
#endif
#ifdef IM_URDU_SUPPORT
    case MMISET_LANGUAGE_URDU:
#endif
#if (defined IM_SWAHILI_SUPPORT) || (defined IM_FILIPINO_SUPPORT) || (defined IM_URDU_SUPPORT) //Bug 2114177
    {
        time_format[0] = TIME_XH;
        time_format[1] = TIME_MM;
        time_format[2] = TIME_SS;
        time_format[3] = TIME_AMPM;
        time_string_order = HOUR_MINUTE_SECOND_AMPM;
     }
        break;
#endif
#if (defined IM_ARABIC_SUPPORT) || (defined IM_AFRIKAANS_SUPPORT) || (defined IM_ALBANIAN_SUPPORT) //Bug 2124776
#ifdef IM_ARABIC_SUPPORT
    case MMISET_LANGUAGE_ARABIC:
#endif
#ifdef IM_AFRIKAANS_SUPPORT
    case MMISET_LANGUAGE_AFRIKAANS:
#endif
#ifdef IM_ALBANIAN_SUPPORT
    case MMISET_LANGUAGE_ALBANIAN:
#endif
    {
        time_format[0] = TIME_HH;
        time_format[1] = TIME_MM;
        time_format[2] = TIME_SS;
        time_format[3] = TIME_ampm;
        time_string_order = HOUR_MINUTE_SECOND_AMPM;
    }
        break;
#endif
#if (defined IM_TRAD_CHINESE_SUPPORT) || (defined MMI_DISPLAY_TRAD_CHINESE_SUPPORT)
    case MMISET_LANGUAGE_TRAD_CHINESE:
#endif
#ifdef IM_PUNJABI_SUPPORT
    case MMISET_LANGUAGE_PUNJABI:
#endif
    {
        time_format[0] = TIME_ampm;
        time_format[1] = TIME_HH;
        time_format[2] = TIME_MM;
        time_format[3] = TIME_SS;
        time_string_order = AMPM_HOUR_MINUTE_SECOND;
    }
        break;
    default:
        time_format[0] = TIME_HH;
        time_format[1] = TIME_MM;
        time_format[2] = TIME_SS;
        time_format[3] = TIME_NONE;
        time_string_order = HOUR_MINUTE_SECOND;
        break;
    }
    SetTimeStringFormat(time_string, time_format, time_string_order);
}

/*******************************************************************************************/
//     Description : set default time format according to current language setting, 
//    Global resource dependence :
//  Author: hongbing.ju
//    Note:
/*********************************************************************************************/
LOCAL void SetDefaultTimeFormat(TIMESTRING_HANDLE_T time_string)
{
    MMISET_LANGUAGE_TYPE_E  language_type = MMISET_LANGUAGE_ENGLISH;

    MMIAPISET_GetLanguageType(&language_type);
    SetDefaultTimeStringFormat(time_string, language_type);
    SetDefaultTimeStringDelimiter(time_string, language_type);
}

/*******************************************************************************************/
//     Description : set 12-hour time value
//    Global resource dependence :
//  Author: hongbing.ju
//    Note:
/*********************************************************************************************/
LOCAL TIME_DATA_T Get12HourSysTime(SCI_TIME_T sys_time)
{
    TIME_DATA_T time_data = {0};

    time_data.sys_time = sys_time;

    if ((MMI_TIME_PM_HOUR > time_data.sys_time.hour))//AM
    {
        time_data.is_am = TRUE;
        if ((MMI_TIME_1_HOUR > time_data.sys_time.hour))//00:00-1:00
        {
            time_data.sys_time.hour = (uint8)(time_data.sys_time.hour + MMI_TIME_PM_HOUR);
        }
    }
    else
    {
        time_data.is_am = FALSE;
        if ((MMI_TIME_PM_HOUR <= time_data.sys_time.hour) && (MMI_TIME_13_HOUR > time_data.sys_time.hour))//12:00-13:00
        {
        }
        else
        {
            time_data.sys_time.hour = (uint8)(time_data.sys_time.hour-MMI_TIME_PM_HOUR);
        }
    }
    return time_data;
}

/*******************************************************************************************/
//     Description : set time string for each slot
//    Global resource dependence :
//  Author: hongbing.ju
//    Note:
/*********************************************************************************************/
LOCAL uint16 SetSlotTimeString(wchar* buf, TIME_FORMAT_T time_format, uint8 value)
{
    MMIBD_ASSERT_DEBUG(buf);

    SCI_TRACE_LOW("SetSlotTimeString: time_format = %d value = %d", time_format, value);

    if(PNULL != buf)
    {
        switch (time_format)
        {
        case TIME_HH:
            return my_itow(buf, 2, value, TRUE);
        case TIME_XH:
            return my_itow(buf, 2, value, FALSE);
        case TIME_MM:
            return my_itow(buf, 2, value, TRUE);
        case TIME_XM:
            return my_itow(buf, 2, value, FALSE);
        case TIME_SS:
            return my_itow(buf, 2, value, TRUE);
        case TIME_XS:
            return my_itow(buf, 2, value, FALSE);
        case TIME_AMPM:
            MMIAPICOM_Wstrncpy(buf, ((value == 1) ? L"AM" : L"PM"), 2);
            return 2;
        case TIME_hh:
        case TIME_mm:
        case TIME_ss:
        case TIME_ampm:
            return MMI_TIME_GetTextString(buf, time_format, value);
        default:
            SCI_TRACE_LOW("SetSlotTimeString: case no need to process here");
            break;
        }
    }
    return 0;
}

LOCAL BOOLEAN IsAmpm(TIME_FORMAT_T time_format)
{
    return ((time_format == TIME_ampm) || (time_format == TIME_AMPM));
}

//time_string[] = {
//    {
//1.        2:12:30     AM/PM       //English (US/Canada/Australia)/Swahili/Filipino/Urdu
//2.        2:09:23     a.m./p.m.   //English New Zealand
//3.        02:10:29    AM/PM       //English S Africa
//4.        上午/下午   02:14:30    //Chinese Taiwan
//5.        ???/???     02:25:56    //Punjabi
//6.        2:29:06     pai u/u u   //Greek
//7.        03:46:32    ?/?         //Arabic
//8.        3:46:50     SA/CH       //Vietnamese
//9.        14.09.38                //Malayalam
//10.       02:38:56    /nm         //Afrikaans
//11.       2:32:43.PD/MD           //Albanian
//12.       14:08:38                //English (England/Ireland)/France/Chinese/Hungarian/Indonesia/Malaysia/Portuguese/Russian/Spain
//                                  //Italian/Turkish/German/Czech/Romania/Slovenian/Dutch/Polish/Bulgarian/Hindi/Thai/Hebrew
//                                  //Bengali/Telugu/Marathi/Tamil/Gujarati/Kannada/Armenian/Azerbaijan/Basque/Catalan/Croatian
//                                  //Danish/Estonian/Finnish/Galician/Georgian/Icelandic/Irish/Kazakh/Latvian/Lithuanian
//                                  //Macedonian/Norwegian/Serbian/Slovak/Swedish/Ukrainian
//    }
// because we are not able to get language profile for country information, case 2 3 and partial of 12 is not feasible right now.
//}

/*******************************************************************************************/
//    Description : Get time string according to time string format
//    Global resource dependence :
//    Author: hongbing.ju
//    Note: please call SetTimeStringFormat(...) first before call this API
/*********************************************************************************************/
PUBLIC wchar* MMI_GetCurrTimeString(TIMESTRING_HANDLE_T time_string)
{
    SCI_TIME_T  sys_time = {0};

    //get system time
    TM_GetSysTime(&sys_time);

    return MMI_GetTimeString(time_string, sys_time);
}

PUBLIC wchar* MMI_GetTimeString(TIMESTRING_HANDLE_T time_string, SCI_TIME_T sys_time)
{
    TIME_DATA_T time_data = {0};

    uint16      len = 0;
    uint16      i = 0;

    wchar       time_wstr[TIMEDATE_MAX_BUF_SIZE];
    uint16      time_wstr_len = 0;
    uint8       time_value[TIME_MAX_SLOT] = {0};

    MMIBD_ASSERT_DEBUG(time_string);

    if(PNULL != time_string)
    {
        SCI_TRACE_LOW("MMI_GetTimeString: hour = %d min = %d sec = %d", sys_time.hour, sys_time.min, sys_time.sec);
        
        if(sys_time.hour > TIME_HOUR_MAX || sys_time.min > TIME_MIN_MAX || sys_time.sec > TIME_SEC_MAX)
        {
            time_string->error = MMI_TIMEDATE_ERROR_INVALID_PARAM;
            return PNULL;
        }
        
        if(1 == time_string->time_string_format.is_ampm)
        {
            time_data = Get12HourSysTime(sys_time);
        }
        else
        {
            time_data.sys_time = sys_time;
            time_data.is_am = FALSE;
        }

        SetTimeStringValue(time_string, time_value, time_data);

        for(i = 0; i < time_string->time_string_format.time_slots; i++)
        {
            time_wstr_len = SetSlotTimeString(time_wstr, time_string->time_string_format.time_format[i], time_value[i]);
            if(time_wstr_len)
            {
                if(TIMEDATE_MAX_BUF_SIZE <len + time_wstr_len)
                {
                    time_string->error = MMI_TIMEDATE_ERROR_OVERFLOW;
                    break;
                }

                MMIAPICOM_Wstrncpy(time_string->wstr_buf+len, time_wstr, time_wstr_len);
                len += time_wstr_len;
                if(i != time_string->time_string_format.time_slots - 1)
                {
                    if(IsAmpm(time_string->time_string_format.time_format[i]) ||
                        (i == time_string->time_string_format.time_slots - 2 
                        && IsAmpm(time_string->time_string_format.time_format[i+1])))
                    {
                        if(TIMEDATE_MAX_BUF_SIZE < len + 1)
                        {
                            time_string->error = MMI_TIMEDATE_ERROR_OVERFLOW;
                            break;
                        }
                        MMIAPICOM_Wstrncpy(time_string->wstr_buf+len, time_string->ampm_delimiter, 1);
                    }
                    else
                    {
                        if(TIMEDATE_MAX_BUF_SIZE < len + 1)
                        {
                            time_string->error = MMI_TIMEDATE_ERROR_OVERFLOW;
                            break;
                        }
                        MMIAPICOM_Wstrncpy(time_string->wstr_buf+len, time_string->delimiter, 1);
                    }
                    len++;

                }            
            }
        }

        time_string->wstr_buf[len] = 0;
        return time_string->wstr_buf;
    }
    return PNULL;
}

/********************************************************************************
** Description: This function is used to get the error code of action           *
** Global Resource Dependency:                                                  *
** Author: Hongbing Ju                                                          *
** Note:                                                                        *
*********************************************************************************/
int32 MMI_TIME_GetCurrErrorCode(TIMESTRING_HANDLE_T timestring)
{
    MMIBD_ASSERT_DEBUG(timestring);

    if (timestring)
    {
        return (timestring->error);
    }

    return (0);    
}

/*******************************************************************************************/
//     Description : create default date format
//    Global resource dependence :
//  Author: hongbing.ju
//    Note:
/*********************************************************************************************/
PUBLIC DATESTRING_HANDLE_T MMI_CreateDefaultDateFormat(void)
{
    DATESTRING_HANDLE_T date_string = SCI_ALLOCA(sizeof(DATE_STRING_T));

    if(PNULL != date_string)
    {
        SCI_TRACE_LOW("MMI_CreateDefaultDateFormat");
        SCI_MEMSET(date_string, 0x00, sizeof(DATE_STRING_T));
        SetDefaultDateFormat(date_string);
    }
    return date_string;
}

/*******************************************************************************************/
//     Description : create date format
//    Global resource dependence :
//  Author: hongbing.ju
//    Note:
/*********************************************************************************************/
PUBLIC DATESTRING_HANDLE_T MMI_CreateDateFormat(
    DATE_FORMAT_T*  date_format,
    wchar*          delimiter,
    DATE_STRING_ORDER_T date_string_order
    )
{
    DATESTRING_HANDLE_T date_string = SCI_ALLOCA(sizeof(DATE_STRING_T));

    if(PNULL != date_string)
    {
        SCI_TRACE_LOW("MMI_CreateTimeFormat: date_format = %x delimiter = %s order = %d", (uint32)date_format, delimiter, date_string_order);
        SCI_MEMSET(date_string, 0x00, sizeof(DATE_STRING_T));
        MMI_SetDateStringFormat(
                date_string,
                date_format,
                date_string_order

                );
        MMI_SetDateStringDelimiter(date_string, delimiter);
    }
    return date_string;
}

PUBLIC void MMI_SetDateStringDelimiter(DATESTRING_HANDLE_T date_string, wchar* delimiter)
{
    MMIBD_ASSERT_DEBUG(date_string && delimiter);

    if(PNULL != date_string && PNULL != delimiter)
    {
        date_string->delimiter[0] = *delimiter;
        date_string->delimiter[1] = 0;
    }
}

/*******************************************************************************************/
//     Description : destroy date string
//    Global resource dependence :
//  Author: hongbing.ju
//    Note:
/*********************************************************************************************/
PUBLIC void MMI_DestroyDateString(DATESTRING_HANDLE_T* date_string)
{
    MMIBD_ASSERT_DEBUG(date_string && *date_string);

    if(PNULL != date_string && PNULL != *date_string)
    {
        SCI_FREE(*date_string);
        *date_string = PNULL;
    }
}

/*******************************************************************************************/
//     Description : set default week day string format according to langauge setting
//    Global resource dependence :
//  Author: hongbing.ju
//    Note:
/*********************************************************************************************/
#ifdef LONG_DATE_FORMAT_SUPPORT
LOCAL void SetDefaultWdayStringFormat(DATESTRING_HANDLE_T date_string, MMISET_LANGUAGE_TYPE_E  language_type)
{
    DATE_STRING_FORMAT_T* date_string_format = PNULL;

    MMIBD_ASSERT_DEBUG(date_string);

    if(PNULL != date_string)
    {
        date_string_format = &date_string->date_string_format;

        switch(language_type)
        {
        case MMISET_LANGUAGE_ENGLISH:
#ifdef IM_SWAHILI_SUPPORT
        case MMISET_LANGUAGE_SWAHILI:
#endif
#ifdef IM_HINDI_SUPPORT
        case MMISET_LANGUAGE_FRENCH:
#endif
#ifdef IM_PORTUGUESE_SUPPORT
        case MMISET_LANGUAGE_PORTUGUESE:
#endif
#ifdef IM_SPANISH_SUPPORT
        case MMISET_LANGUAGE_SPANISH:
#endif
#ifdef IM_TURKISH_SUPPORT
        case MMISET_LANGUAGE_TURKISH:
#endif
#ifdef IM_GERMAN_SUPPORT
        case MMISET_LANGUAGE_GERMAN:
#endif
#ifdef IM_HEBREW_SUPPORT
        case MMISET_LANGUAGE_HEBREW:
#endif
#ifdef IM_DUTCH_SUPPORT
        case MMISET_LANGUAGE_DUTCH:
#endif
            MMI_SetWdayStringFormat(date_string_format, DATE_ww);
        default:
            MMI_SetWdayStringFormat(date_string_format, DATE_WW);    //means no weekday
            break;
        }
    }
}
#endif

/*******************************************************************************************/
//     Description : set default date string order according to langauge setting
//    Global resource dependence :
//  Author: hongbing.ju
//    Note:
/*********************************************************************************************/
LOCAL void SetDefaultDateStringFormat(DATESTRING_HANDLE_T date_string, MMISET_LANGUAGE_TYPE_E  language_type)
{
    DATE_STRING_ORDER_T date_string_order = MONTH_MDAY_YEAR;
    DATE_FORMAT_T date_format[4] = {0};

    SCI_TRACE_LOW("SetDefaultDateStringFormat: language = %d", language_type);

    switch(language_type)
    {
    case MMISET_LANGUAGE_ENGLISH:
#ifdef IM_SWAHILI_SUPPORT
    case MMISET_LANGUAGE_SWAHILI:
#endif
#ifdef IM_FILIPINO_SUPPORT
    case MMISET_LANGUAGE_FILIPINO:
#endif
        date_format[0] = DATE_XM;
        date_format[1] = DATE_XD;
        date_format[2] = DATE_YYYY;
        date_string_order = MONTH_MDAY_YEAR;
        break;
#if (defined IM_SIMP_CHINESE_SUPPORT) || (defined MMI_DISPLAY_SIMP_CHINESE_SUPPORT)
    case MMISET_LANGUAGE_SIMP_CHINESE:
#endif
#if (defined IM_TRAD_CHINESE_SUPPORT) || (defined MMI_DISPLAY_TRAD_CHINESE_SUPPORT)
    case MMISET_LANGUAGE_TRAD_CHINESE:
#endif
    {
        date_format[0] = DATE_YYYY;
        date_format[1] = DATE_XM;
        date_format[2] = DATE_XD;
        date_string_order = YEAR_MONTH_MDAY;
    }
        break;
#ifdef IM_HUNGARIAN_SUPPORT
    case MMISET_LANGUAGE_HUNGARIAN:
#endif
#ifdef IM_URDU_SUPPORT
    case MMISET_LANGUAGE_URDU:
#endif
#ifdef IM_POLISH_SUPPORT
    case MMISET_LANGUAGE_POLISH:
#endif
#ifdef IM_AFRIKAANS_SUPPORT
    case MMISET_LANGUAGE_AFRIKAANS:
#endif
#ifdef IM_BASQUE_SUPPORT
    case MMISET_LANGUAGE_BASQUE:
#endif
#ifdef IM_ZULU_SUPPORT
    case MMISET_LANGUAGE_ZULU:
#endif
#ifdef IM_ALBANIAN_SUPPORT
    case MMISET_LANGUAGE_ALBANIAN:
#endif
#ifdef IM_SWEDISH_SUPPORT
    case MMISET_LANGUAGE_SWEDISH:
#endif
#ifdef IM_LATVIAN_SUPPORT
    case MMISET_LANGUAGE_LATVIAN:
#endif
#ifdef IM_LITHUANIAN_SUPPORT
    case MMISET_LANGUAGE_LITHUANIAN:
#endif
#if (defined IM_HUNGARIAN_SUPPORT) || (defined IM_URDU_SUPPORT) || (defined IM_POLISH_SUPPORT) || (defined IM_AFRIKAANS_SUPPORT) || (defined IM_BASQUE_SUPPORT) || (defined IM_ZULU_SUPPORT) || (defined IM_ALBANIAN_SUPPORT) || (defined IM_SWEDISH_SUPPORT) || (defined IM_LATVIAN_SUPPORT) || (defined IM_LITHUANIAN_SUPPORT)//Bug 2114177
    {
        date_format[0] = DATE_YYYY;
        date_format[1] = DATE_MM;
        date_format[2] = DATE_DD;
        date_string_order = YEAR_MONTH_MDAY;
    }
        break;
#endif
#ifdef IM_ARABIC_SUPPORT
    case MMISET_LANGUAGE_ARABIC:
#endif
    {
        date_format[0] = DATE_dd;
        date_format[1] = DATE_mm;
        date_format[2] = DATE_yyyy;
        date_string_order = MDAY_MONTH_YEAR;
    }
       break;
#ifdef IM_HINDI_SUPPORT
    case MMISET_LANGUAGE_HINDI:
#endif
#ifdef IM_PORTUGUESE_SUPPORT
    case MMISET_LANGUAGE_PORTUGUESE:
#endif
#ifdef IM_MARATHI_SUPPORT
    case MMISET_LANGUAGE_MARATHI:
#endif
#ifdef IM_TAMIL_SUPPORT
    case MMISET_LANGUAGE_TAMIL:
#endif
#ifdef IM_GUJARATI_SUPPORT
    case MMISET_LANGUAGE_GUJARATI:
#endif
#ifdef IM_KANNADA_SUPPORT
    case MMISET_LANGUAGE_KANNADA:
#endif
#ifdef IM_MALAYALAM_SUPPORT
    case MMISET_LANGUAGE_MALAYALAM:
#endif
#ifdef IM_DANISH_SUPPORT
    case MMISET_LANGUAGE_DANISH:
#endif
#ifdef IM_FRENCH_SUPPORT
    case MMISET_LANGUAGE_FRENCH:
#endif
#ifdef IM_INDONESIAN_SUPPORT
    case MMISET_LANGUAGE_INDONESIAN:
#endif
#ifdef IM_MALAY_SUPPORT
    case MMISET_LANGUAGE_MALAY:
#endif
#ifdef IM_SPANISH_SUPPORT
    case MMISET_LANGUAGE_SPANISH:
#endif
#ifdef IM_ITALIAN_SUPPORT
    case MMISET_LANGUAGE_ITALIAN:
#endif
#ifdef IM_CATALAN_SUPPORT
    case MMISET_LANGUAGE_CATALAN:
#endif
#ifdef IM_IRISH_SUPPORT
    case MMISET_LANGUAGE_IRISH:
#endif
#ifdef IM_RUSSIAN_SUPPORT
    case MMISET_LANGUAGE_RUSSIAN:
#endif
#ifdef IM_TURKISH_SUPPORT
    case MMISET_LANGUAGE_TURKISH:
#endif
#ifdef IM_GERMAN_SUPPORT
    case MMISET_LANGUAGE_GERMAN:
#endif
#ifdef IM_ROMANIAN_SUPPORT
    case MMISET_LANGUAGE_ROMANIAN:
#endif
#ifdef IM_ARMENIAN_SUPPORT
    case MMISET_LANGUAGE_ARMENIAN:
#endif
#ifdef IM_AZERBAIJANI_SUPPORT
    case MMISET_LANGUAGE_AZERBAIJANI:
#endif
#ifdef IM_GEORGIAN_SUPPORT
    case MMISET_LANGUAGE_GEORGIAN:
#endif
#ifdef IM_KAZAKH_SUPPORT
    case MMISET_LANGUAGE_KAZAKH:
#endif
#ifdef IM_VIETNAMESE_SUPPORT
    case MMISET_LANGUAGE_VIETNAMESE:
#endif
#ifdef IM_HEBREW_SUPPORT
    case MMISET_LANGUAGE_HEBREW:
#endif
#ifdef IM_BENGALI_SUPPORT
    case MMISET_LANGUAGE_BENGALI:
#endif
#ifdef IM_MACEDONIAN_SUPPORT
    case MMISET_LANGUAGE_MACEDONIAN:
#endif
#ifdef IM_NORWEGIAN_SUPPORT
    case MMISET_LANGUAGE_NORWEGIAN:
#endif
#ifdef IM_UKRAINIAN_SUPPORT
    case MMISET_LANGUAGE_UKRAINIAN:
#endif
    {
        date_format[0] = DATE_DD;
        date_format[1] = DATE_MM;
        date_format[2] = DATE_YYYY;
        date_string_order = MDAY_MONTH_YEAR;
    }
        break;
#ifdef IM_GREEK_SUPPORT
    case MMISET_LANGUAGE_GREEK:
#endif
#ifdef IM_THAI_SUPPORT
    case MMISET_LANGUAGE_THAI:
#endif
#ifdef IM_CZECH_SUPPORT
    case MMISET_LANGUAGE_CZECH:
#endif
#ifdef IM_SLOVENIAN_SUPPORT
    case MMISET_LANGUAGE_SLOVENIAN:
#endif
#ifdef IM_CROATIAN_SUPPORT
    case MMISET_LANGUAGE_CROATIAN:
#endif
#ifdef IM_FINNISH_SUPPORT
    case MMISET_LANGUAGE_FINNISH:
#endif
#ifdef IM_ICELANDIC_SUPPORT
    case MMISET_LANGUAGE_ICELANDIC:
#endif
#ifdef IM_SERBIAN_SUPPORT
    case MMISET_LANGUAGE_SERBIAN:
#endif
#ifdef IM_SLOVAK_SUPPORT
    case MMISET_LANGUAGE_SLOVAK:
#endif
#ifdef IM_DUTCH_SUPPORT
    case MMISET_LANGUAGE_DUTCH:
#endif
    {
        date_format[0] = DATE_XD;
        date_format[1] = DATE_XM;
        date_format[2] = DATE_YYYY;
        date_string_order = MDAY_MONTH_YEAR;
    }
        break;
#ifdef IM_ESTONIAN_SUPPORT
    case MMISET_LANGUAGE_ESTONIAN:
        date_format[0] = DATE_XD;
        date_format[1] = DATE_MM;
        date_format[2] = DATE_YYYY;
        date_string_order = MDAY_MONTH_YEAR;
        break;
#endif
#ifdef IM_BULGARIAN_SUPPORT
    case MMISET_LANGUAGE_BULGARIAN:
        date_format[0] = DATE_DD;
        date_format[1] = DATE_XM;
        date_format[2] = DATE_YYYY;
        date_string_order = MDAY_MONTH_YEAR;
        break;
#endif
#ifdef IM_PUNJABI_SUPPORT
    case MMISET_LANGUAGE_PUNJABI:
#endif
#ifdef IM_TELUGU_SUPPORT
    case MMISET_LANGUAGE_TELUGU:
#endif
#ifdef IM_GALICIAN_SUPPORT
    case MMISET_LANGUAGE_GALICIAN:
#endif
    {
        date_format[0] = DATE_DD;
        date_format[1] = DATE_MM;
        date_format[2] = DATE_YY;
        date_string_order = MDAY_MONTH_YEAR;
    }
        break;
    default:
        date_format[0] = DATE_DD;
        date_format[1] = DATE_MM;
        date_format[2] = DATE_YYYY;
        date_string_order = MDAY_MONTH_YEAR;
        break;
    }
    MMI_SetDateStringFormat(date_string, date_format, date_string_order);
}

/*******************************************************************************************/
//     Description : set default date string delimiter according to langauge setting
//    Global resource dependence :
//  Author: hongbing.ju
//    Note:
/*********************************************************************************************/
LOCAL void SetDefaultDateStringDelimiter(wchar* delimiter, MMISET_LANGUAGE_TYPE_E  language_type)
{
    MMIBD_ASSERT_DEBUG(delimiter);

    if(PNULL != delimiter)
    {
        SCI_TRACE_LOW("SetDefaultDateStringDelimiter: language = %d", language_type);

        delimiter[1] = 0;
        switch(language_type)
        {
        case MMISET_LANGUAGE_ENGLISH:
#ifdef IM_SWAHILI_SUPPORT
        case MMISET_LANGUAGE_SWAHILI:
#endif
#ifdef IM_FILIPINO_SUPPORT
        case MMISET_LANGUAGE_FILIPINO:
#endif
#ifdef IM_ARABIC_SUPPORT
        case MMISET_LANGUAGE_ARABIC:
#endif
#ifdef IM_FRENCH_SUPPORT
        case MMISET_LANGUAGE_FRENCH:
#endif
#ifdef IM_INDONESIAN_SUPPORT
        case MMISET_LANGUAGE_INDONESIAN:
#endif
#ifdef IM_MALAY_SUPPORT
        case MMISET_LANGUAGE_MALAY:
#endif
#ifdef IM_SPANISH_SUPPORT
        case MMISET_LANGUAGE_SPANISH:
#endif
#ifdef IM_ITALIAN_SUPPORT
        case MMISET_LANGUAGE_ITALIAN:
#endif
#ifdef IM_CATALAN_SUPPORT
        case MMISET_LANGUAGE_CATALAN:
#endif
#ifdef IM_IRISH_SUPPORT
        case MMISET_LANGUAGE_IRISH:
#endif
#if (defined IM_TRAD_CHINESE_SUPPORT) || (defined MMI_DISPLAY_TRAD_CHINESE_SUPPORT)
        case MMISET_LANGUAGE_TRAD_CHINESE:
#endif
#ifdef IM_THAI_SUPPORT
        case MMISET_LANGUAGE_THAI:
#endif
#ifdef IM_VIETNAMESE_SUPPORT
        case MMISET_LANGUAGE_VIETNAMESE:
#endif
#ifdef IM_AFRIKAANS_SUPPORT
        case MMISET_LANGUAGE_AFRIKAANS:
#endif
#ifdef IM_BASQUE_SUPPORT
        case MMISET_LANGUAGE_BASQUE:
#endif
#ifdef IM_URDU_SUPPORT
        case MMISET_LANGUAGE_URDU:
#endif
#ifdef IM_ASSAMESE_SUPPORT
        case MMISET_LANGUAGE_ZULU:
#endif
#ifdef IM_GREEK_SUPPORT
        case MMISET_LANGUAGE_GREEK:
#endif
#ifdef IM_HEBREW_SUPPORT
        case MMISET_LANGUAGE_HEBREW:
#endif
#ifdef IM_GALICIAN_SUPPORT
        case MMISET_LANGUAGE_GALICIAN:
#endif
            delimiter[0] = '/';
            break;
#if (defined IM_SIMP_CHINESE_SUPPORT) || (defined MMI_DISPLAY_SIMP_CHINESE_SUPPORT)
        case MMISET_LANGUAGE_SIMP_CHINESE:
#endif
#ifdef IM_HINDI_SUPPORT
        case MMISET_LANGUAGE_HINDI:
#endif
#ifdef IM_PORTUGUESE_SUPPORT
        case MMISET_LANGUAGE_PORTUGUESE:
#endif
#ifdef IM_MARATHI_SUPPORT
        case MMISET_LANGUAGE_MARATHI:
#endif
#ifdef IM_TAMIL_SUPPORT
        case MMISET_LANGUAGE_TAMIL:
#endif
#ifdef IM_GUJARATI_SUPPORT
        case MMISET_LANGUAGE_GUJARATI:
#endif
#ifdef IM_KANNADA_SUPPORT
        case MMISET_LANGUAGE_KANNADA:
#endif
#ifdef IM_MALAYALAM_SUPPORT
        case MMISET_LANGUAGE_MALAYALAM:
#endif
#ifdef IM_DANISH_SUPPORT
        case MMISET_LANGUAGE_DANISH:
#endif
#ifdef IM_DUTCH_SUPPORT
        case MMISET_LANGUAGE_DUTCH:
#endif
#ifdef IM_POLISH_SUPPORT
        case MMISET_LANGUAGE_POLISH:
#endif
#ifdef IM_ALBANIAN_SUPPORT
        case MMISET_LANGUAGE_ALBANIAN:
#endif
#ifdef IM_SWEDISH_SUPPORT
        case MMISET_LANGUAGE_SWEDISH:
#endif
#ifdef IM_PUNJABI_SUPPORT
        case MMISET_LANGUAGE_PUNJABI:
#endif
#ifdef IM_BENGALI_SUPPORT
        case MMISET_LANGUAGE_BENGALI:
#endif
#ifdef IM_TELUGU_SUPPORT
        case MMISET_LANGUAGE_TELUGU:
#endif
            delimiter[0] = '-';
            break;
#if 0
        case MMISET_LANGUAGE_HUNGARIAN:
        case MMISET_LANGUAGE_RUSSIAN:
        case MMISET_LANGUAGE_TURKISH:
        case MMISET_LANGUAGE_GERMAN:
        case MMISET_LANGUAGE_ROMANIAN:
        case MMISET_LANGUAGE_ARMENIAN:
        case MMISET_LANGUAGE_AZERBAIJANI:
        case MMISET_LANGUAGE_GEORGIAN:
        case MMISET_LANGUAGE_KAZAKH:
        case MMISET_LANGUAGE_CZECH:
        case MMISET_LANGUAGE_SLOVENIAN:
        case MMISET_LANGUAGE_CROATIAN:
        case MMISET_LANGUAGE_FINNISH:
        case MMISET_LANGUAGE_ICELANDIC:
        case MMISET_LANGUAGE_SERBIAN:
        case MMISET_LANGUAGE_SLOVAK:
        case MMISET_LANGUAGE_BULGARIAN:
        case MMISET_LANGUAGE_ESTONIAN:
        case MMISET_LANGUAGE_LATVIAN:
        case MMISET_LANGUAGE_LITHUANIAN:
        case MMISET_LANGUAGE_MACEDONIAN:
        case MMISET_LANGUAGE_NORWEGIAN:
        case MMISET_LANGUAGE_UKRAINIAN:
#endif
        default:
            delimiter[0] = '.';
            break;
        }
    }
}

/*******************************************************************************************/
//     Description : get date string default delimiter depending on language setting
//    Global resource dependence :
//  Author: hongbing.ju
//    Note:
/*********************************************************************************************/
PUBLIC void MMI_GetDateStringDefaultDelimiter(wchar* delimiter)
{
    if(PNULL != delimiter)
    {
        MMISET_LANGUAGE_TYPE_E  language_type = MMISET_LANGUAGE_ENGLISH;

        MMIAPISET_GetLanguageType(&language_type);

        SetDefaultDateStringDelimiter(delimiter, language_type);
    }
}

/*******************************************************************************************/
//     Description : set default date format according to current language setting 
//    Global resource dependence :
//  Author: hongbing.ju
//    Note:
/*********************************************************************************************/
LOCAL void SetDefaultDateFormat(DATESTRING_HANDLE_T date_string)
{
    MMISET_LANGUAGE_TYPE_E  language_type = MMISET_LANGUAGE_ENGLISH;

    MMIAPISET_GetLanguageType(&language_type);
    SetDefaultDateStringFormat(date_string, language_type);
    SetDefaultDateStringDelimiter(date_string->delimiter, language_type);
}

PUBLIC void MMI_SetDateStringFormat(DATESTRING_HANDLE_T date_string, DATE_FORMAT_T *date_format, DATE_STRING_ORDER_T date_string_order)
{
    DATE_STRING_FORMAT_T *date_string_format = PNULL;

    MMIBD_ASSERT_DEBUG(date_string && date_format);

    if(PNULL != date_string && PNULL != date_format)
    {
        date_string_format = &date_string->date_string_format;

        SCI_TRACE_LOW("MMI_SetDateStringFormat: date_format = %x order = %d", (uint32)date_format, date_string_order);
        switch(date_string_order)
        {
        case YEAR_ONLY:
        case MONTH_ONLY:
        case MDAY_ONLY:
#ifdef LONG_DATE_FORMAT_SUPPORT
        case WDAY_ONLY:
#endif
            MMI_SET_DATESTRING_FORMAT(date_string_format, 0, date_format[0]);
            MMI_SET_DATESTRING_FORMAT(date_string_format, 1, DATE_NONE);
            MMI_SET_DATESTRING_FORMAT(date_string_format, 2, DATE_NONE);
#ifdef LONG_DATE_FORMAT_SUPPORT
            MMI_SET_DATESTRING_FORMAT(date_string_format, 3, DATE_NONE);
#endif
            date_string_format->date_slots = 1;
            break;
        case MDAY_MONTH:
        case MONTH_MDAY:
        case MONTH_YEAR:
        case YEAR_MONTH:
            MMI_SET_DATESTRING_FORMAT(date_string_format, 0, date_format[0]);
            MMI_SET_DATESTRING_FORMAT(date_string_format, 1, date_format[1]);
            MMI_SET_DATESTRING_FORMAT(date_string_format, 2, DATE_NONE);
#ifdef LONG_DATE_FORMAT_SUPPORT
            MMI_SET_DATESTRING_FORMAT(date_string_format, 3, DATE_NONE);
#endif
            date_string_format->date_slots = 2;
            break;
#ifdef LONG_DATE_FORMAT_SUPPORT
        case WDAY_MDAY_MONTH:
        case MONTH_MDAY_WDAY:
#endif
        case MDAY_MONTH_YEAR:
        case MONTH_MDAY_YEAR:
        case YEAR_MDAY_MONTH:
        case YEAR_MONTH_MDAY:
            MMI_SET_DATESTRING_FORMAT(date_string_format, 0, date_format[0]);
            MMI_SET_DATESTRING_FORMAT(date_string_format, 1, date_format[1]);
            MMI_SET_DATESTRING_FORMAT(date_string_format, 2, date_format[2]);
#ifdef LONG_DATE_FORMAT_SUPPORT
            MMI_SET_DATESTRING_FORMAT(date_string_format, 3, DATE_NONE);
#endif
            date_string_format->date_slots = 3;
            break;
#ifdef LONG_DATE_FORMAT_SUPPORT
        case WDAY_MDAY_MONTH_YEAR:
        case WDAY_MONTH_MDAY_YEAR:
        case YEAR_MDAY_MONTH_WDAY:
        case YEAR_MONTH_MDAY_WDAY:
            MMI_SET_DATESTRING_FORMAT(date_string_format, 0, date_format[0]);
            MMI_SET_DATESTRING_FORMAT(date_string_format, 1, date_format[1]);
            MMI_SET_DATESTRING_FORMAT(date_string_format, 2, date_format[2]);
            MMI_SET_DATESTRING_FORMAT(date_string_format, 3, date_format[3]);
            date_string_format->date_slots = 4;
            break;
#endif
        default:
            SCI_TRACE_LOW("MMI_SetDateStringFormat: case no need to process here");
            break;
        }
        date_string_format->date_string_order = date_string_order;
    }
}

/*******************************************************************************************/
//     Description : set date slot value according to date format 
//    Global resource dependence :
//  Author: hongbing.ju
//    Note:
/*********************************************************************************************/
LOCAL void SetDateStringValue(DATESTRING_HANDLE_T date_string, uint16 *date_value, SCI_DATE_T sys_date)
{
    DATE_STRING_FORMAT_T*   date_string_format = PNULL;
    uint32  i = 0;

    MMIBD_ASSERT_DEBUG(date_string && date_value);

    if(PNULL != date_string && PNULL != date_value)
    {
        date_string_format = &date_string->date_string_format;

        SCI_TRACE_LOW("SetDateStringValue: date_value = %x date = %d mon = %d year = %d", (uint32)date_value, sys_date.mday, sys_date.mon, sys_date.year);

        for(i = 0; i < date_string_format->date_slots; i++)
        {
            switch (date_string_format->date_format[i])
            {
            case DATE_DD:
            case DATE_XD:
            case DATE_dd:
                date_value[i] = sys_date.mday;
                break;
            case DATE_ww:
            case DATE_xw:
                date_value[i] = sys_date.wday;
                break;
            case DATE_MM:
            case DATE_XM:
            case DATE_mm:
                date_value[i] = sys_date.mon;
                break;
            case DATE_YY:
                date_value[i] = sys_date.year % YEAR_CENTURY;
                break;
            case DATE_YYYY:
            case DATE_yyyy:
                date_value[i] = sys_date.year;
                break;
            default:
                SCI_TRACE_LOW("SetDateStringValue: case no need to process here");
                break;
            }
        }
    }
}

LOCAL uint16 SetSlotDateString(wchar* buf, DATE_FORMAT_T date_format, uint16 value)
{
    MMIBD_ASSERT_DEBUG(buf);

    if(PNULL != buf)
    {
        SCI_TRACE_LOW("SetSlotDateString: buf = %x date_format = %d value = %d", (uint32)buf, date_format, value);
        switch (date_format)
        {
        case DATE_DD:
            return my_itow(buf, 2, value, TRUE);
        case DATE_XD:
            return my_itow(buf, 2, value, FALSE);
        case DATE_MM:
            return my_itow(buf, 2, value, TRUE);
        case DATE_XM:
            return my_itow(buf, 2, value, FALSE);
        case DATE_YY:
            return my_itow(buf, 2,value%YEAR_CENTURY, TRUE);
        case DATE_YYYY:
            return my_itow(buf, 4, value, TRUE);
        case DATE_dd:
        case DATE_ww:
        case DATE_xw:
        case DATE_mm:
        case DATE_xm:
        case DATE_yyyy:
            return MMI_DATE_GetTextString(buf, date_format, value);
        default:
            SCI_TRACE_LOW("SetSlotDateString: case no need to process here");
            break;
        }
    }
    return 0;
}

//date_string[] = {               // short expression
//{
//XM_XD_YYYY        7/24(1)/2012            //English (US)/Swahili/Filipino
//DD_MM_YYYY        24(01)/07/2012          //English (England/Canada/Ireland)/French/Indonesian/Malay/Spain
                                            //Italian/Catalan/Irish
//XD_MM_YYYY        24(1)/07/2012           //English (Australia/New Zealand)
//XD_XM_YYYY        24(1)/7/2012            //Greek
//YYYY_MM_DD        2012/07/24(01)          //English (S Africa)/Afrikaans/Basque
//YYYY_XM_XD        2012-7-24(1)            //Chinese China/
//YYYY_XM_XD        2012/7/24(1)            //Chinese Taiwan/
//YYYY_MM_DD        2012.07.24(01)          //Hungarian
//DD_MM_YYYY        24(01)-07-2012          //Portuguese/Hindi/Marathi/Tamil/Gujarati/Kannada/Malayalam/Danish
//DD_MM_YY          24(01)-07-12            //Punjabi
//DD_MM_YYYY        24(01).07.2012          //Russian/Turkish/German/Romanian/Armenian/Azerbaijan/Georgian/Kazakh
//XD_XM_YYYY        26(1)/7/2555            //Thai
//DD_MM_YYYY        26(01)/07/2012          //Vietnamese
//YYYY_MM_DD        2012/07/26(01)          //Urdu/Zulu
//XD_XM_YYYY        24(1).7.2012            //Czech/Slovenian/Croatian/Finnish/Icelandic/Serbian/Slovak
//DD_MM_YYYY        26(01)/?07/?2012        //Hebrew
//DD_MM_YYYY        26(01)-07-12            //Bengali
//XD_XM_YYYY        24(1)-7-2012            //Dutch
//YYYY_MM_DD        2012-07-24(01)          //Polish/Albanian/Swedish
//DD_XM_YYYY        24(01).7.2012 r .       //Bulgarian
//DD_MM_YYYY        ?26(01)/?09/?1433       //Arabic
//DD_MM_YY          26(01)-07-12            //Telugu
//XD_MM_YYYY        26(1).07.2012           //Estonian
//DD_MM_YY          01/07/12                //Galician
//YYYY_MM_DD        2012.07.01              //Latvian/Lithuanian
//DD_MM_YYYY        01.07.2012              //Macedonian/Norwegian/Ukrainian
//}

//date_string[] = {               // long expression
//ww_mm_DD_YYYY     Tuesday, July 24(01), 2012  //English (US)/Swahili
//DD_mm_YYYY        24(01) July 2012        //English (England/Ireland/S Africa)/
//mm_xD_YYYY        July 24(1), 2012        //English (Canada)/
//ww_xD_mm_YYYY     Tuesday, 24(1) July 2012    //English (Australia/New Zealand)
//YYYY_XM_XD        2012年7月24(1)日        //Chinese (China/Taiwan)
//ww_xD_mm_YYYY     ?(26)01 ??????, ?1433       //Arabic
//ww_xD_mm_YYYY     mardi 24(1) juillet 2012    //French
//DD_mm_YYYY        24(01) ????? 2012       //Hindi
//YYYY_mm_XD        2012. juius 24(1).      //Hungarian
//DD_mm_YYYY        24(01) Juli 2012        //Indonesian
//DD_mm_YYYY        24(01) Julai 2012       //Malay
//ww_XD_mm_YYYY     terca-feira, 24(1) de Julho de 2012 //Portuguese
//XD_mm_YYYY        24(1) ???? 2012 r.      //Russian
//ww_DD_mm_YYYY     martes, 24(01) de julio de 2012 //Spain
//XD_mm_YYYY        26(1) ??????? 2555          //Thai
//DD_mm_YYYY        26(01) tha磏g ba?y 2012     //Vietnamese
//DD_mm_YYYY        26(01) ??????, ?2012        //Urdu
//XD_mm_YYYY        martedl 24(1) luglio 2012   //Italian
//DD_mm_YYYY_ww     24(01) Temmuz 2012 Sali //Turkish
//ww_XD_mm_YYYY     Dienstag, 24(1). Juli 2012 //German
//XD_mm_YYYY        ???t?, 24(1) ??????? 2012   //Greek
//XD_mm_YYYY        24(1). cervence 2012    //Czech
//XD_mm_YYYY        24(1) iulie 2012        //Romanian
//XD_mm_YYYY        24(1). julij 2012       //Slovenian
//ww_DD_mm_YYYY     ????????? ?26(01) ????? ?2012  //Hebrew
//DD_mm_YYYY        26(01) ????? 2012       //Bengali
//ww_XD_mm_YYYY     dinsdag 24(1) juli 2012 //Dutch
//XD_mm_YYYY        24(1) lipca 2012        //Polish
//DD_mm_YYYY        24(01) ??? 2012 r .     //Bulgarian
//DD_mm_YYYY        26(01) ???? 2012        //Telugu
//DD_mm_YYYY        26(01) ???? 2012        //Marathi
//DD_mm_YYYY        26(01) ???? 2012        //Tamil
//DD_mm_YYYY        26(01) ????? 2012       //Gujarati
//DD_mm_YYYY        26(01) ???? 2012        //Kannada
//DD_mm_YYYY_ww     01 ????? 2012 ?????     //Punjabi
//DD_mm_YYYY        01 Julie 2012           //Afrikaans
//YYYY_MM_DD        2012-07-01              //Albanian
//DD_mm_YYYY        1 ??????, 2012          //Armenian
//DD_mm_YYYY        1 ???? 2012             //Azerbaijani
//ww_YYYY_mm_XD     igandea, 2012.eko uztailak 1    //Basque
//ww_XD_mm_YYYY     diumenge, 1 / juliol / 2012 //Catalan
//XD_mm_YYYY        1. srpanj 2012          //Croatian
//XD_mm_YYYY        1. juli 2012            //Danish
//XD_mm_YYYY        1. juuli 2012. a.       //Estonian
//ww_mm_DD_YYYY     Linggo, Hulyo 01, 2012  //Filipino
//XD_mm_YYYY        1. heinakuuta 2012      //Finnish
//ww_DD_mm_YYYY     domingo, 01 de xullo de 2012    //Galician
//YYYY_DD_MM_ww     2012 ???? 01 07, ?????  //Georgian
//XD_mm_YYYY        1. jul| 2012            //Icelandic       
//XD_mm_YYYY        1 Iuil 2012             //Irish
//XD_mm_YYYY_ww     1 ????? 2012 ?.         //Kazakh
//ww_YYYY_XD_mm     svetdiena, 2012. gada 1. julija //Latvian
//YYYY_mm_XD        2012 m. liepos 1 d.     //Lithuanian
//ww_DD_mm_YYYY     ??????, 01 ???? 2012    //Macedonian    
//XD_mm_YYYY        1. juli 2012            //Norwegian
//XD_mm_YYYY        1. ??? 2012             //Serbian
//XD_mm_YYYY        1. jula 2012            //Slovak
//ww_XD_mm_YYYY     den 1 juli 2012         //Swedish
//XD_mm_YYYY_ww     1 ????? 2012 ?.         //Ukrainian
//XD_mm_YYYY        01 uJulayi 2012         //Zulu
//}

/*******************************************************************************************/
//     Description : get date string according to date format 
//    Global resource dependence :
//  Author: hongbing.ju
//    Note:
/*********************************************************************************************/
PUBLIC wchar* MMI_GetCurrDateString(DATESTRING_HANDLE_T date_string)
{
    SCI_DATE_T  sys_date = {0};

    //get system time
    TM_GetSysDate(&sys_date);

    return MMI_GetDateString(date_string, sys_date);
}

PUBLIC wchar* MMI_GetDateString(DATESTRING_HANDLE_T date_string, SCI_DATE_T  sys_date)
{
    uint16      len = 0;
    uint16      i = 0;

    wchar       date_wstr[TIMEDATE_MAX_BUF_SIZE] = {0};
    uint16      date_wstr_len = 0;
    uint16      date_value[DATE_MAX_SLOT] = {0};

    if(sys_date.mon > DATE_MON_MAX || sys_date.mday > DATE_MDAY_MAX || sys_date.wday > DATE_WDAY_MAX)
    {
        date_string->error = MMI_TIMEDATE_ERROR_MASK(MMI_TIMEDATE_ERROR_INVALID_PARAM);
        return PNULL;
    }
    SetDateStringValue(date_string, date_value, sys_date);

    SCI_TRACE_LOW("MMI_GetDateString: date_string = %x date = %d mon = %d year = %d", (uint32)date_string, sys_date.mday, sys_date.mon, sys_date.year);

    MMIBD_ASSERT_DEBUG(date_string);

    if(PNULL != date_string)
    {
        for(i = 0; i < date_string->date_string_format.date_slots; i++)
        {
            date_wstr_len = SetSlotDateString(date_wstr, date_string->date_string_format.date_format[i], date_value[i]);
            if(date_wstr_len)
            {
                if(TIMEDATE_MAX_BUF_SIZE < len + date_wstr_len)
                {
                    date_string->error = MMI_TIMEDATE_ERROR_OVERFLOW;
                    break;
                }
                MMIAPICOM_Wstrncpy(date_string->wstr_buf+len, date_wstr, date_wstr_len);
                len += date_wstr_len;
                if(i != date_string->date_string_format.date_slots - 1)
                {
                    if(TIMEDATE_MAX_BUF_SIZE < len + 1)
                    {
                        date_string->error = MMI_TIMEDATE_ERROR_OVERFLOW;
                        break;
                    }
                    MMIAPICOM_Wstrncpy(date_string->wstr_buf+len, date_string->delimiter, 1);
                    len++;
                }            
            }
        }

        date_string->wstr_buf[len] = 0;
        return date_string->wstr_buf;
    }
    return PNULL;
}

/********************************************************************************
** Description: This function is used to get the error code of action           *
** Global Resource Dependency:                                                  *
** Author: Hongbing Ju                                                          *
** Note:                                                                        *
*********************************************************************************/
int32 MMI_DATE_GetCurrErrorCode(DATESTRING_HANDLE_T datestring)
{
    MMIBD_ASSERT_DEBUG(datestring);

    if (datestring)
    {
        return (datestring->error);
    }

    return (0);    
}

#ifdef STANDALONE_TEST
#ifdef _MMI_TIMEDATE_STRING_UNIT_TEST_
void main(void)
{
    int i;
    TIMESTRING_HANDLE_T time_string;
    wchar    *time_string_buf = PNULL;
    DATESTRING_HANDLE_T date_string;
    wchar    *date_string_buf = PNULL;
    uint32   date_string_format = 0;
    uint32   date_string_format_le = 0;
    TIME_FORMAT_T   time_format[4];
    DATE_FORMAT_T   date_format[4];
    wchar           delimiter[2];

    for(i = MMISET_LANGUAGE_ENGLISH; i < MMISET_MAX_LANGUAGE; i++)
    {
        MMIAPISET_SetLanguageType(i);
        
        time_string = MMI_CreateDefaultTimeFormat();

        time_string_buf = MMI_GetCurrTimeString(time_string);
        wprintf(L"%s\r\n", time_string_buf);
        free(time_string);

        time_format[0] = TIME_HH;
        time_format[1] = TIME_ampm;
        MMI_GetTimeStringDefaultDelimiter(delimiter);
        time_string = MMI_CreateTimeFormat(time_format, delimiter, HOUR_AMPM);
        time_string_buf = MMI_GetCurrTimeString(time_string);
        wprintf(L"%s\r\n", time_string_buf);
        free(time_string);

        time_format[0] = TIME_ampm;
        time_format[1] = TIME_HH;
        time_format[2] = TIME_MM;
        time_string = MMI_CreateTimeFormat(time_format, ":", AMPM_HOUR_MINUTE);
        time_string_buf = MMI_GetCurrTimeString(time_string);
        wprintf(L"%s\r\n", time_string_buf);
        free(time_string);
    }

    for(i = MMISET_LANGUAGE_ENGLISH; i < MMISET_MAX_LANGUAGE; i++)
    {
        MMIAPISET_SetLanguageType(i);

        date_string = MMI_CreateDefaultDateFormat();

        date_string_buf = MMI_GetCurrDateString(date_string);
        wprintf(L"%s\r\n", date_string_buf);
        free(date_string);

        date_format[0] = DATE_MM;
        date_format[1] = DATE_YYYY;
        MMI_GetDateStringDefaultDelimiter(delimiter);
        date_string = MMI_CreateDateFormat(date_format, delimiter, MDAY_MONTH, DATE_DD);
        date_string_buf = MMI_GetCurrDateString(date_string);
        wprintf(L"%s\r\n", date_string_buf);
        free(date_string);

    }

}
#endif
#endif
