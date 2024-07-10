#define HAVE_LITTLE_ENDIAN
# ifdef HAVE_LITTLE_ENDIAN
#  define ntohl(x)    ( ((x) << 24) | (((x) >> 24) & 255) | (((x) << 8) & 0xff0000) | (((x) >> 8) & 0xff00) )
#  define htonl(x)    ntohl(x)
#  define ntohs(x)    ( (((x) << 8) & 0xff00) | (((x) >> 8) & 255) )
#  define htons(x)    ntohs(x)
# else
#  define ntohl(x)    (x)
#  define htonl(x)    (x)
#  define ntohs(x)    (x)
#  define htons(x)    (x)
# endif

#define TXT_SUNDAY                  0x1001
#define TXT_MONDAY                  0x1002
#define TXT_TUESDAY                 0x1003
#define TXT_WEDNESDAY               0x1004
#define TXT_THURSDAY                0x1005
#define TXT_FRIDAY                  0x1006
#define TXT_SATURDAY                0x1007
#define TXT_SHORT_SUNDAY       0x1008
#define TXT_SHORT_MONDAY       0x1009
#define TXT_SHORT_TUESDAY      0x100a
#define TXT_SHORT_WEDNESDAY    0x100b
#define TXT_SHORT_THURSDAY     0x100c
#define TXT_SHORT_FRIDAY       0x100d
#define TXT_SHORT_SATURDAY     0x100e
#define TXT_KL_TIME_AM              0x100f
#define TXT_KL_TIME_PM              0x1010
#define TXT_JANUARY                 0x1011
#define TXT_FEBRARY                 0x1012
#define TXT_MARCH                   0x1013
#define TXT_APRIL                   0x1014
#define TXT_MAY                     0x1015
#define TXT_JUNE                    0x1016
#define TXT_JULY                    0x1017
#define TXT_AUGUST                  0x1018
#define TXT_SEPTEMBER				0x1019
#define TXT_OCTORBER                0x101a
#define TXT_NOVEMBER                0x101b
#define TXT_DECEMBER				0x101c

#define TXT_SHORT_JANUARY           0x101d
#define TXT_SHORT_FEBRARY           0x101e
#define TXT_SHORT_MARCH             0x101f
#define TXT_SHORT_APRIL             0x1020
#define TXT_SHORT_MAY               0x1021
#define TXT_SHORT_JUNE              0x1022
#define TXT_SHORT_JULY              0x1023
#define TXT_SHORT_AUGUST            0x1024
#define TXT_SHORT_SEPTEMBER         0x1025
#define TXT_SHORT_OCTORBER          0x1026
#define TXT_SHORT_NOVEMBER          0x1027
#define TXT_SHORT_DECEMBER          0x1028

#define TXT_AM                      0
#define TXT_PM                      1

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
    TXT_SHORT_SUNDAY,
    TXT_SHORT_MONDAY,
    TXT_SHORT_TUESDAY,
    TXT_SHORT_WEDNESDAY,
    TXT_SHORT_THURSDAY,
    TXT_SHORT_FRIDAY,
    TXT_SHORT_SATURDAY
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

wchar s_suffix[16] = {0};

PUBLIC ERR_TM_E TM_GetSysTime(SCI_TIME_T  *time_ptr)
{
    SYSTEMTIME systime;

    GetSystemTime(&systime);
    time_ptr->hour = systime.wHour;
    time_ptr->min = systime.wMinute;
    time_ptr->sec = systime.wSecond;
}

PUBLIC ERR_TM_E TM_GetSysDate(SCI_DATE_T  *date_ptr)
{
    SYSTEMTIME systime;

    GetSystemTime(&systime);
    date_ptr->year = systime.wYear;
    date_ptr->mon = systime.wMonth;
    date_ptr->mday = systime.wDay;
    date_ptr->wday = systime.wDayOfWeek;
}

const wchar *MMIRES_GetText(
                            MMI_TEXT_ID_T      text_id,     //[in] the text id 
                            CAF_HANDLE_T       handle_id,
                            MMI_STRING_T       *str_ptr
                            )
{
    str_ptr->wstr_ptr = s_suffix;
    if(text_id == TXT_AM)
    {
        MMIAPICOM_Wstrncpy(str_ptr->wstr_ptr, L" AM", 3);
    }
    else
    {
        MMIAPICOM_Wstrncpy(str_ptr->wstr_ptr, L" PM", 3);
    }
    str_ptr->wstr_len = 3;
    return str_ptr->wstr_ptr;
}

MMISET_LANGUAGE_TYPE_E s_cur_language = MMISET_LANGUAGE_ENGLISH;

void MMIAPISET_GetLanguageType(MMISET_LANGUAGE_TYPE_E *language)
{
    *language = s_cur_language;
    //return MMISET_LANGUAGE_SIMP_CHINESE;
}

void MMIAPISET_SetLanguageType(MMISET_LANGUAGE_TYPE_E language)
{
    s_cur_language = language;
    //return MMISET_LANGUAGE_SIMP_CHINESE;
}

static wstr_ptr[4];
PUBLIC void MMI_GetLabelTextByLang( 
                                   MMI_TEXT_ID_T    label,     //[in] the label 
                                   MMI_STRING_T     *str_ptr   //[out] the data of text
                                   )
{
    str_ptr->wstr_ptr = wstr_ptr;
    MMIAPICOM_Wstrncpy(wstr_ptr, ((label == TXT_KL_TIME_AM) ? L"AM" : L"PM"), 2);
    str_ptr->wstr_len = 2;
}

#define MAX_TRACESTR_LEN 1024
#define SCI_DisableIRQ()
#define SCI_RestoreIRQ()
// ----------------------------------------------------------------------------
// FUNCTION NAME:   SCI_Trace
// AUTHOR:          Liu Jun
// INPUT:           ...
// RETURN:          None
// DESCRIPTION:     Internal trace functions for RTOS (DEBUG mode in VC)
// ----------------------------------------------------------------------------
void SCI_Trace(const char * x_format, ...)
{
    va_list     args;
    int         nBuf;
    char        format_str[MAX_TRACESTR_LEN] = {0};
    static  int flag=0;

    SCI_DisableIRQ();

    va_start(args, x_format);

    //apple:  support to outputing  trace to logel
    //[[
    nBuf = _vsnprintf(format_str, sizeof(format_str) - 1,x_format, args);
    // was there an error? was the  expanded string too long?
    //assert((nBuf >= 0) && (nBuf < MAX_TRACESTR_LEN));
    va_end(args);
    //]]
    SCI_RestoreIRQ();


}

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

