/*****************************************************************************
** File Name:      mmi_gettextstring.c                                       *
** Author:                                                                   *
** Date:           08/16/2012                                                *
** Copyright:      2003 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to control mmi module                   *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 08/2012       hongbing.ju       Creat
******************************************************************************/

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_app_common_trc.h"
#include "mmk_type.h"
#include "dal_time.h"
#include "mmi_resource.h"
#include "mmi_resource_def.h"
#include "mmi_module.h" 
#include "mmi_text.h" 
#include "mmi_string.h"
#include "mmi_timedate_string.h"

PUBLIC MMI_TEXT_ID_T wday_text_id[] = {
    TXT_SUNDAY,
    TXT_MONDAY,
    TXT_TUESDAY,
    TXT_WEDNESDAY,
    TXT_THURSDAY,
    TXT_FRIDAY,
    TXT_SATURDAY
};

PUBLIC MMI_TEXT_ID_T wday_short_text_id[] = {
    TXT_SHORT_SUN,
    TXT_SHORT_MON,
    TXT_SHORT_TUE,
    TXT_SHORT_WED,
    TXT_SHORT_THU,
    TXT_SHORT_FRI,
    TXT_SHORT_SAT
};

PUBLIC MMI_TEXT_ID_T month_text_id[] = {
	TXT_JANUARY,
	TXT_FEBRARY,
	TXT_MARCH,
	TXT_APRIL,
	TXT_MAY,
	TXT_JUNE,
	TXT_JULY,
	TXT_AUGUST,
	TXT_SEPTEMBER,
	TXT_OCTORBER,
	TXT_NOVEMBER,
	TXT_DECEMBER
};

PUBLIC MMI_TEXT_ID_T month_short_text_id[] = {
	TXT_SHORT_JANUARY,
	TXT_SHORT_FEBRARY,
	TXT_SHORT_MARCH,
	TXT_SHORT_APRIL,
	TXT_SHORT_MAY,
	TXT_SHORT_JUNE,
	TXT_SHORT_JULY,
	TXT_SHORT_AUGUST,
	TXT_SHORT_SEPTEMBER,
	TXT_SHORT_OCTORBER,
	TXT_SHORT_NOVEMBER,
	TXT_SHORT_DECEMBER
};

/*******************************************************************************************/
//     Description : get time text string 
//    Global resource dependence :
//  Author: hongbing.ju
//    Note:
/*********************************************************************************************/
PUBLIC uint16 MMI_TIME_GetTextString(wchar* buf, TIME_FORMAT_T time_format, uint16 value)
{
    MMI_STRING_T string_prefix = {0};

    MMIBD_ASSERT_DEBUG(buf);

    if(PNULL != buf)
    {
        switch (time_format)
        {
        case TIME_hh:
            // FIXME how to get arabic number string
            return my_itow(buf, 2, value, TRUE);
        case TIME_mm:
            // FIXME how to get arabic number string
            return my_itow(buf, 2, value, TRUE);
        case TIME_ss:
            // FIXME how to get arabic number string
            return my_itow(buf, 2, value, TRUE);
        case TIME_ampm:
            MMI_GetLabelTextByLang(((value == 1) ? TXT_AM : TXT_PM), &string_prefix);    //don't need to free string_prefix.str_ptr
            MMIAPICOM_Wstrncpy(buf, string_prefix.wstr_ptr, string_prefix.wstr_len);
            return string_prefix.wstr_len;
        default:
            SCI_TRACE_LOW("MMI_TIME_GetTextString: case no need to process here");
            break;
        }
    }
    return 0;
}

/*******************************************************************************************/
//     Description : get date text string 
//    Global resource dependence :
//  Author: hongbing.ju
//    Note:
/*********************************************************************************************/
PUBLIC uint16 MMI_DATE_GetTextString(wchar* buf, DATE_FORMAT_T date_format, uint16 value)
{
    MMI_STRING_T string_prefix = {0};

    MMIBD_ASSERT_DEBUG(buf);

    if(PNULL != buf)
    {
        switch (date_format)
        {
        case DATE_dd:
            // FIXME how to get arabic number string
            return my_itow(buf, 2, value, TRUE);
        case DATE_ww:
            MMI_GetLabelTextByLang(wday_text_id[value], &string_prefix);    //don't need to free string_prefix.str_ptr
            MMIAPICOM_Wstrncpy(buf, string_prefix.wstr_ptr, string_prefix.wstr_len);
            return string_prefix.wstr_len;
        case DATE_xw:
            MMI_GetLabelTextByLang(wday_short_text_id[value], &string_prefix);    //don't need to free string_prefix.str_ptr
            MMIAPICOM_Wstrncpy(buf, string_prefix.wstr_ptr, string_prefix.wstr_len);
            return string_prefix.wstr_len;
        case DATE_mm:
            MMI_GetLabelTextByLang(month_text_id[value], &string_prefix);    //don't need to free string_prefix.str_ptr
            MMIAPICOM_Wstrncpy(buf, string_prefix.wstr_ptr, string_prefix.wstr_len);
            return string_prefix.wstr_len;
        case DATE_xm:
            MMI_GetLabelTextByLang(month_short_text_id[value], &string_prefix);    //don't need to free string_prefix.str_ptr
            MMIAPICOM_Wstrncpy(buf, string_prefix.wstr_ptr, string_prefix.wstr_len);
            return string_prefix.wstr_len;
        case DATE_yyyy:
            // FIXME how to get arabic number string
            return my_itow(buf, 4, value, TRUE);
        default:
            SCI_TRACE_LOW("MMI_DATE_GetTextString: case no need to process here");
            break;
        }
    }
    return 0;
}

