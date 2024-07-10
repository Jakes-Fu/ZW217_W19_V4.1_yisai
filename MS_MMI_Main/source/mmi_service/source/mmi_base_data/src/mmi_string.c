/****************************************************************************
** File Name:      mmi_string.c                                         *
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
/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "mmi_app_common_trc.h"
#include "mmk_type.h"
#include "ctype.h"
#include "mmisrv_adapt_gui.h"
#include "mmisrv_adapt.h"
#include "mmi_textfun.h"
#include "dal_time.h"
#include "mmi_string.h"

#define     MMICOM_CMD_START_CHAR 0x30  //命令串中的起始字符
#define     MMICOM_CMD_END_CHAR  0x7A  //命令串中的终止字符
#define     MMICOM_NUMBER_SYSTEM 75

# ifdef HAVE_LITTLE_ENDIAN
#  define NTOHL(x)    ( ((x) << 24) | (((x) >> 24) & 255) | (((x) << 8) & 0xff0000) | (((x) >> 8) & 0xff00) )
#  define HTONL(x)    NTOHL(x)
#  define NTOHS(x)    ( (((x) << 8) & 0xff00) | (((x) >> 8) & 255) )
#  define HTONS(x)    NTOHS(x)
# else
#  define NTOHL(x)    (x)
#  define HTONL(x)    (x)
#  define NTOHS(x)    (x)
#  define HTONS(x)    (x)
# endif

 /**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/
#if 0
/*#if defined(TCPIP_SUPPORT) && !defined(IPVERSION_SUPPORT_V4_V6)*/
/*fix the link error.*/
extern  int stricmp (const char *a, const char *b);
extern  int strnicmp (const char *a, const char *b, size_t c); /*lint !e526*/
//#else
int strnicmp(char * s1, char * s2, int len)
{
    char  c1,   c2;

    while (len--)
    {
        c1 = (char)tolower(*s1++);
        c2 = (char)tolower(*s2++);
        if (c1 == c2)
        {
            if (c1 == 0)   /* end of both strings */
                return 0;   /* they match */
            continue;   /* check next char */
        }
        else if(c1 < c2)
            return -1;
        else  /* c1 > c2 */
            return 1;
    }
    return 0;
}
int stricmp(char * s1, char * s2)
{
    int   l1,   l2;

    l1 = strlen(s1);  /* get both lengths */
    l2 = strlen(s2);

    /* call strnicmp with maximum length */
    return( strnicmp(s1, s2, ( l1 > l2 )?l1:l2 ));
}
#endif

/*****************************************************************************/
//  Description : 复制字符串
//  Global resource dependence : none
//  Parameter: out_len_ptr  : return the copied string
//             out_alpha_ptr: return the copied string length
//             in_len       : the length of string to be copied
//             max_len      : the max length of out_len_ptr
//             in_alpha_ptr : string of to be copied to out_len_ptr
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPICOM_CopyString(
                                 uint32      *out_len_ptr,       //OUT:
                                 uint8       *out_alpha_ptr,     //OUT:
                                 uint32      in_len,             //IN:
                                 uint32      max_len,            //IN:
                                 uint8       *in_alpha_ptr       //IN:
                                 )
{

    if (PNULL == out_len_ptr || PNULL == out_alpha_ptr || PNULL == in_alpha_ptr)
    {
        //SCI_TRACE_LOW:"MMIAPICOM_CopyString PNULL == out_len_ptr || PNULL == out_alpha_ptr || PNULL == in_alpha_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICOM_STRING_894_112_2_18_1_59_13_100,(uint8*)"");
        return;
    }
    if (0 < in_len)
    {
        *out_len_ptr = (in_len >= max_len) ? max_len : in_len;
        SCI_MEMCPY(
                    out_alpha_ptr,
                    in_alpha_ptr,
                    *out_len_ptr
                    );
    }
    else
    {
        *out_len_ptr = 0;
        SCI_MEMSET(
                    out_alpha_ptr,
                    0XFF,
                    max_len
                    );
    }
}


/*******************************************************************************************/
//     Description : compare string,
//                if str1 > str2, return >0;
//                   str1 = str2, return =0;
//                   str1 < str2, return <0
//    Global resource dependence :
//  Author: lin.lin
//    Note:
/*********************************************************************************************/
PUBLIC int32 MMIAPICOM_StrCmp(
                              uint8* str1,         //compared string
                              uint16 str1_len,     //string length
                              uint8* str2,         //compared string
                              uint16 str2_len      //string length
                              )
{
    uint16 num = 0;
    int32  res = 0;

    //kevin.lou modified:delete assert
    //MMIBD_ASSERT_DEBUG(PNULL != str1);/*assert verified*/
    //MMIBD_ASSERT_DEBUG(PNULL != str2);/*assert verified*/
    if(PNULL == str1 || PNULL == str2)
    {
        if(PNULL != str1)
        {
            return 1;
        }
        else if(PNULL != str2)
        {
            return -1;
        }
        return 0;
    }

    num = (uint16)(MIN(str1_len, str2_len));
    res = strncmp((char *)str1, (char *)str2, num);

    if ((0 == res) && (str1_len != str2_len))
    {
        res = (str1_len > str2_len)? 1 : -1;
    }

    return res;
}

/*****************************************************************************/
//  Description : compare string,
//                if str1 > str2, return >0;
//                   str1 = str2, return =0;
//                   str1 < str2, return <0
//  Global resource dependence :
//  Author:jassmine.meng
//  Note:
/*****************************************************************************/
PUBLIC int32 MMIAPICOM_CompareTwoUint8String(
                            uint8    *str1_ptr,  //compared string
                            uint16    str1_len,   //string length,字节数
                            uint8    *str2_ptr,  //compared string
                            uint16    str2_len    //string length,字节数
                            )
{
    return MMIAPICOM_StrCmp(str1_ptr, str1_len, str2_ptr, str2_len);
}

/*****************************************************************************/
//  Description : cmp two string
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC int32 MMIAPICOM_Stricmp(
                               const uint8 *str1,
                               const uint8 *str2
                               )
{
    if(PNULL == str1 || PNULL == str2)
    {
        if(PNULL != str1)
        {
            return 1;
        }
        else if(PNULL != str2)
        {
            return -1;
        }
        return 0;
    }

    return stricmp((const char*)str1, (const char*)str2);
}

/*****************************************************************************/
//  Description :
// This function searches for a given character in a string. If it is found
// a pointer to that character is returned. If it is not found a NULL
// pointer is returned. If the the given string is in unicode it is searched
// for the given char in unicode format. Only searching for default GSM
// alphabet characters is possible. The parameter i2_text is given in
// default GSM alphabet.
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC uint8 *MMIAPICOM_SearchChar(
            uint8  *i1_text_ptr,
                     int8  i2_char)
{
     /*-------------------------------------------------*/
     /* Definitions of variables local to this function */
     /*-------------------------------------------------*/
     int16 len = 0;
     int16 i =0;
     uint8 *pstr = i1_text_ptr;

    if (PNULL == i1_text_ptr)
    {
        //SCI_TRACE_LOW:"MMIAPICOM_CopyString PNULL == i1_text_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICOM_STRING_1035_112_2_18_1_59_13_101,(uint8*)"");
        return PNULL;
    }
     len = SCI_STRLEN((char*)pstr);
     for (i=0; i<len; i++)
     {
        if (*pstr== i2_char)
        {
            return(pstr);
        }
        else
        {
            pstr++;
        }
     }
     return(PNULL);      // NOT FOUND

} /* end MMIAPICOM_SearchChar */


/*****************************************************************************/
//  Description : stristr(str1, str2) - look for str2 (case insensitive) inside str1
//  Global resource dependence : none
//  Author: wancan.you
//  Note:
//  Return:
/*****************************************************************************/
PUBLIC char * MMIAPICOM_Stristr(char * str1, char * str2)
{
    char c1 = 0;
    char c2 = 0;
    uint32 len2 = 0;

    if (PNULL == str1 || PNULL == str2)
    {
        return (char *)PNULL;
    }

    c1 = (char)tolower(*str2); /* 1st char of sub-string in upper case */
    len2 = strlen(str2);
    while (*str1)
    {
        c2 = (char)tolower(*str1); /* c1=toupper(*str2) */
        if (c1 == c2)  /* found first char of str2 in str1 */
        {
            if (strnicmp(str1, str2, len2) == 0)
            {
                return str1;   /* found str2 at str1 */
            }
        }
        str1++;
    }
    return (char *)PNULL;
}

/*****************************************************************************/
//  Description : str lower
//  Global resource dependence :
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC char* MMIAPICOM_Strlower( char * string )
{
    char *start = string;

    if( PNULL != string )
    {
        while (NULL != (*string = tolower( *string )) )/*lint !e564 */
            string++;
    }

    return start;
}

/*****************************************************************************/
//  Description : str upper
//  Global resource dependence :
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC char* MMIAPICOM_Strupper( char * string )
{
    char *start = string;

    if( PNULL != string )
    {
        while (NULL != (*string = toupper( *string )) )/*lint !e564 */
            string++;
    }

    return start;
}

/*****************************************************************************/
//  Description : str to wstr
//  Global resource dependence :
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC wchar* MMIAPICOM_StrToWstr(
                                  const uint8* src,
                                  wchar* dst
                                  )
{
    wchar *return_wstr = dst;

    if( src == PNULL ||PNULL == dst)
    {
        return return_wstr;
    }

    while( '\0' != ( *dst++ = *src++ ) )    /* copy string */
    {
        ;
    }

    return return_wstr;
}

/*****************************************************************************/
//  Description : wstr to str
//  Global resource dependence :
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC uint8* MMIAPICOM_WstrToStr(
                                  const wchar* src,
                                  uint8* dst
                                  )
{
    uint8 *return_str = dst;

    if( src == PNULL || PNULL == dst )
    {
        return return_str;
    }

    while( !(*src & 0xFF00) && '\0' != (*dst++ = *src++) )
    {
        ;
    }

    return return_str;
}

/*****************************************************************************/
//  Description : str to wstr, for 5 param
//  Global resource dependence :
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC wchar* MMI_STRNTOWSTR(
                             wchar* dst,
                             size_t dst_max_count,
                             const uint8* src,
                             size_t src_max_count,
                             size_t count
                             )
{
    wchar *wstr = dst;

    if( src == PNULL || PNULL == dst)
    {
        return wstr;
    }

    if (count <= src_max_count && count <= dst_max_count)
    {
        while( count && '\0' != (*dst++ = *src++) )    /* copy string */
            count--;

        if( count )                            /* pad out with zeroes */
            while(--count)
                *dst++ = '\0';
    }
    else
    {
        //SCI_TRACE_LOW:"MMI_STRTOWSTR: the count is too big!count=%d,src_max_count=%d,dst_max_count=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_COMMON_3071_112_2_18_1_58_15_37,(uint8*)"ddd", count, src_max_count, dst_max_count);
    }
    return wstr;
}

/*****************************************************************************/
//  Description : 把string对应的字符串连接到一个字符串中
//  Global resource dependence :
//  Author:yongwei.he
//  Note: used to textbox
/*****************************************************************************/
PUBLIC void MMIAPICOM_StrcatFromStrToUCS2(
                                          uint16  *ucs2_str_ptr,    //in, out
                                          uint16  *ucs2_str_len_ptr,    //in, out, uint16 numbers, not bytes
                                          const uint8 *str_ptr,   //in
                                          uint16   str_len
                                          )
{
    if (PNULL == ucs2_str_ptr || PNULL == ucs2_str_len_ptr || PNULL == str_ptr)
    {
        //SCI_TRACE_LOW:"MMIAPICOM_StrcatFromStrToUCS2 PNULL == ucs2_str_ptr || PNULL == ucs2_str_len_ptr || PNULL == str_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICOM_STRING_669_112_2_18_1_59_12_95,(uint8*)"");
        return;
    }
    MMI_STRNTOWSTR( ucs2_str_ptr + (*ucs2_str_len_ptr), str_len, str_ptr, str_len, str_len );

    *ucs2_str_len_ptr += str_len;
}

/*****************************************************************************/
//  Description : wstr to str, for 5 param
//  Global resource dependence :
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC uint8* MMI_WSTRNTOSTR(
                             uint8* dst,
                             size_t dst_max_count,
                             const wchar* src,
                             size_t src_max_count,
                             size_t count
                             )
{
    uint8 *return_str = dst;

    if( src == PNULL || PNULL == dst )
    {
        return return_str;
    }
    if (count <= src_max_count && count <= dst_max_count)
    {
        while( count && !(*src & 0xFF00) && '\0' != (*dst++ = *src++) )
        {
            count--;
        }

        if( count )                            /* pad out with zeroes */
        {
            while(--count)
            {
                *dst++ = '\0';
            }
        }
    }
    else
    {
        //SCI_TRACE_LOW:"MMI_WSTRNTOSTR: the count is too big!count=%d,src_max_count=%d,dst_max_count=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_COMMON_3111_112_2_18_1_58_16_38,(uint8*)"ddd", count, src_max_count, dst_max_count);
    }
    return return_str;
}

/*****************************************************************************/
//  Description : convert ascii string to unicode string(UCS2)
//  Global resource dependence : none
//  Author: zhaohui
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPICOM_ConvertAlphabetToUnicode(
                                    uint8 *i1_default_text_ptr,         // alphabet
                                    uint8 *o1_unicode_text_ptr,     // unicode
                                    int16  i2_output_buf_size
                                    )       // length of unicode
{
     /*-------------------------------------------------*/
     /* Definitions of variables local to this function */
     /*-------------------------------------------------*/
     uint8 *p = PNULL;
     uint8 *out_buf =  o1_unicode_text_ptr;
     int16 counter = 0;

    if (PNULL == i1_default_text_ptr || PNULL == o1_unicode_text_ptr)
    {
        //SCI_TRACE_LOW:"MMIAPICOM_ConvertAlphabetToUnicode PNULL == i1_default_text_ptr || PNULL == o1_unicode_text_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICOM_STRING_1251_112_2_18_1_59_13_106,(uint8*)"");
        return ;
    }
     /*------*/
     /* Code */
     /*------*/
     counter = 0;
     for (p = i1_default_text_ptr;
          ( NULL != *p) && (counter < i2_output_buf_size);
           p++)
       {
        *out_buf++ = 0;
        *out_buf++ = *p;
            counter+=2;
       }
     /* create the end of the string */
} /* end ConvertAlphabetToUnicode */

/*****************************************************************************/
//  Description : wstr str cmp
//  Global resource dependence :
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC int MMIAPICOM_WstrStrncmp(
                                 const wchar* wstr_ptr,
                                 const uint8*  str_ptr,
                                 size_t count
                                 )
{
    int result = 0;
    if (!count)
        return(0);

    if( wstr_ptr == PNULL || str_ptr == PNULL )
    {
        if(str_ptr != PNULL )
        {
            return -1;
        }

        if( wstr_ptr != PNULL)
        {
            return 1;
        }
        return (0);
    }
/*lint -save -e613 -e794 */
    while ( --count && *wstr_ptr && *wstr_ptr == *str_ptr )
    {
        wstr_ptr++;
        str_ptr++;
    }

    result = ( *wstr_ptr - *str_ptr );
/*lint -restore */
    return ( result > 0 ? 1 : ( result < 0 ? -1 : 0 ) );
}

/*****************************************************************************/
//  Description : wstrcpy
//  Global resource dependence :
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC wchar* MMIAPICOM_Wstrcpy(
                                wchar* dst,
                                const wchar* src
                                )
{
    wchar* cp = dst;

    if( PNULL == src || PNULL == dst )
    {
        return dst;
    }

    while(PNULL != (*cp++ = *src++) )
        ; /* copy src to dst */

    return( dst );

}

/*****************************************************************************/
//  Description : wstrncpy
//  Global resource dependence :
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC wchar* MMIAPICOM_Wstrncpy(
                                 wchar* dst,
                                 const wchar* src,
                                 size_t count
                                 )
{
    wchar* start = dst;

    if( PNULL == src || PNULL == dst)
    {
        return start;
    }

    while( (0 != count) && ('\0' != (*dst++ = *src++)) )    /* copy string */
    {
        count--;
    }

    if( count )                            /* pad out with zeroes */
        while(--count)
            *dst++ = '\0';

    return(start);
}

/*****************************************************************************/
//  Description : wstrncpy, for 5 param
//  Global resource dependence :
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC wchar* MMI_WSTRNCPY(
                    wchar* dst,
                    size_t dst_max_count,
                    const wchar* src,
                    size_t src_max_count,
                    size_t count
                    )
{
    if (count <= src_max_count && count <= dst_max_count)
    {
        return MMIAPICOM_Wstrncpy( dst, src, count );
    }
    else
    {
        //SCI_TRACE_LOW:"MMI_WSTRNCPY: the count is too big!count=%d,src_max_count=%d,dst_max_count=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_COMMON_3036_112_2_18_1_58_15_36,(uint8*)"ddd", count, src_max_count, dst_max_count);
        return PNULL;
    }
}

/*****************************************************************************/
//  Description : wstrcat
//  Global resource dependence :
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC wchar* MMIAPICOM_Wstrcat(
                                wchar* dst,
                                const wchar* src
                                )
{
    wchar * cp = dst;

    if( PNULL == src || PNULL == dst )
    {
        return dst;
    }

    while( *cp )
        cp++;                   /* find end of dst */

    while(PNULL != (*cp++ = *src++) ) ;   /* copy src to end of dst */

    return( dst );              /* return dst */
}

/*****************************************************************************/
//  Description : wstrncat
//  Global resource dependence :
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC wchar* MMIAPICOM_Wstrncat(
                                 wchar* dst,
                                 const wchar* src,
                                 size_t count
                                 )
{
    wchar *start = dst;

    if( PNULL == src || PNULL == dst )
    {
        return start;
    }

    while ( *dst++ )
        ;

    dst--;

    while( count-- )
    {
        if('\0' == ( *dst++ = *src++ ) )
        {
            return(start);
        }
    }

    *dst = 0;

    return(start);
}

/*****************************************************************************/
//  Description : wstrcmp
//  Global resource dependence :
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC int MMIAPICOM_Wstrcmp(
                             const wchar * string1,
                             const wchar * string2
                             )
{
    int ret = 0 ;

    if( string1 == PNULL || string2 == PNULL )
    {
        if( string2 != PNULL )
        {
            return -1;
        }
        else if( string1 != PNULL )
        {
            return 1;
        }
        return 0;
    }
    else
    {
/*lint -save -e613 */
        ret = *(wchar *)string1 - *(wchar *)string2;/*lint !e613 */

        while( (!ret) && *string2 && *string1)/*lint !e613 */
        {
            ++string1;/*lint !e613 */
            ++string2;
            ret = *(wchar *)string1 - *(wchar *)string2;
        }
/*lint -restore */
        return ( ret > 0 ? 1 : ( ret < 0 ? -1 : 0 ) );
    }
}

/*****************************************************************************/
//  Description : wstrncmp
//  Global resource dependence :
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC int MMIAPICOM_Wstrncmp(
                              const wchar* string1,
                              const wchar* string2,
                              size_t count
                              )
{
    int result = 0;
    if (!count)
    {
        return(0);
    }

    if( string1 == PNULL || string2 == PNULL )
    {
        if( string1 == PNULL )
        {
            return -1;
        }
        else if( string2 != PNULL )
        {
            return 1;
        }
        return (0);
    }
    else
    {
/*lint -save -e613 -e794 */
        while ( --count && *string1 && *string1 == *string2 )
        {
            string1++;
            string2++;
        }

        result = ( *(wchar *)string1 - *(wchar *)string2 );
/*lint -restore */
        return ( result > 0 ? 1 : ( result < 0 ? -1 : 0 ) );
    }
}

/*****************************************************************************/
//  Description : compare string,
//                if str1 > str2, return >0;
//                   str1 = str2, return =0;
//                   str1 < str2, return <0
//  Global resource dependence :
//  Author:jassmine.meng
//  Note:
/*****************************************************************************/
PUBLIC int32 MMIAPICOM_CompareTwoWstr(
                                      const wchar    *wstr1_ptr,  //compared string
                                      uint16         wstr1_len,   //string length,字节数
                                      const wchar    *wstr2_ptr,  //compared string
                                      uint16         wstr2_len    //string length,字节数
                                      )
{
    return MMIAPICOM_CompareTwoWstrExt(wstr1_ptr,wstr1_len,wstr2_ptr,wstr2_len,TRUE);
}

/*****************************************************************************/
//  Description : compare string,
//                if str1 > str2, return >0;
//                   str1 = str2, return =0;
//                   str1 < str2, return <0
//  Global resource dependence :
//  Author:jian.ma
//  Note:
/*****************************************************************************/
PUBLIC int32 MMIAPICOM_CompareTwoWstrExt(
                                      const wchar    *wstr1_ptr,  //wchar string1
                                      uint16         wstr1_len,   //string length
                                      const wchar    *wstr2_ptr,  //wchar string2
                                      uint16         wstr2_len,   //string length
                                      BOOLEAN        str_flag     //是否字符串区分大小写
                                      )
{
    int32   result = 0;
    uint16  min_len = 0;
    wchar   *str1 = PNULL;
    wchar   *str2 = PNULL;

    if (PNULL == wstr1_ptr || PNULL == wstr2_ptr)
    {
        //SCI_TRACE_LOW:"MMIAPICOM_CompareTwoWstrExt PNULL == wstr1_ptr || PNULL == wstr2_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICOM_STRING_790_112_2_18_1_59_12_99,(uint8*)"");
        return result;
    }
    min_len = MIN( wstr1_len, wstr2_len);

    if (!str_flag)
    {
        str1 =  SCI_ALLOCA(sizeof(wchar)*(wstr1_len+1));
        SCI_MEMSET(str1,0x00,(sizeof(wchar)*(wstr1_len+1)));
        MMIAPICOM_Wstrncpy(str1,wstr1_ptr,wstr1_len);

        str2 =  SCI_ALLOCA(sizeof(wchar)*(wstr2_len+1));
        SCI_MEMSET(str2,0x00,(sizeof(wchar)*(wstr2_len+1)));
        MMIAPICOM_Wstrncpy(str2,wstr2_ptr,wstr2_len);

        MMIAPICOM_Wstrlower(str1);
        MMIAPICOM_Wstrlower(str2);

        result  = MMIAPICOM_Wstrncmp( str1, str2, min_len );
    }
    else
    {
        result  = MMIAPICOM_Wstrncmp( wstr1_ptr, wstr2_ptr, min_len );
    }

    if ((0 == result) && (wstr1_len != wstr2_len))
    {
        result = (wstr1_len > wstr2_len)? 1 : -1;
    }
    if (!str_flag)
    {
        SCI_FREE(str1);
        SCI_FREE(str2);
    }
    return (result);
}

int MMIAPICOM_Wstrzcmp(const wchar *s1, size_t n1, const wchar *s2, size_t n2)
{
    const wchar* e1 = s1+n1;
    const wchar* e2 = s2+n2;

    while (s1 < e1 && s2 < e2)
    {
        const int d = (int)*s1++ - (int)*s2++;
        if (d)
        {
            return (d > 0) ? 1 : ((d < 0) ? -1 : 0);
        }
    }
    return (n1 < n2) ? -1 : ((n1 > n2) ? 1 : 0);
}

int MMIAPICOM_Wstrzcmp_h_n(const wchar *s1H, size_t n1, const wchar *s2N, size_t n2)
{
    const wchar* e1 = s1H+n1;
    const wchar* e2 = s2N+n2;

    while (s1H < e1 && s2N < e2)
    {
        const wchar c2 = NTOHS(*s2N);
        const int d = (int)*s1H++ - (int)c2;
        s2N++;
        if (d)
        {
            return (d > 0) ? 1 : ((d < 0) ? -1 : 0);
        }
    }

    return (n1 < n2) ? -1 : ((n1 > n2) ? 1 : 0);
}

/*****************************************************************************/
//  Description : compair string by its pinyin string
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
#ifndef STANDALONE_TEST
PUBLIC int MMIAPICOM_WstrncmpByPinYin(
                              const wchar* string1,
                              const wchar* string2,
                              size_t count
                              )
{
    int result = 0;
	wchar  letter_buf1[40] = {0};
	wchar  letter_buf2[40] = {0};

    if (!count)
    {
        return(0);
    }

    if( string1 == PNULL || string2 == PNULL )
    {
        if( string2 != PNULL )
        {
            return -1;
        }
        else if( string1 != PNULL )
        {
            return 1;
        }
        return (0);
    }
    else
    {
/*lint -save -e613 -e794 */
        while ( --count && *string1 && tolower(*string1) == tolower(*string2))
        {
            string1++;
            string2++;
        }
		SCI_MEMSET(letter_buf1, 0x00, sizeof(wchar)*40);
		MMIAPIIM_GetTextLetters(*string1, letter_buf1);
		SCI_MEMSET(letter_buf2, 0x00, sizeof(wchar)*40);
		MMIAPIIM_GetTextLetters(*string2, letter_buf2);
		result = MMIAPICOM_Wstrcmp(letter_buf1, letter_buf2);
        if(!result)
		{
			//same pinstring string, compare unicode
			result = ( *(wchar *)string1 - *(wchar *)string2 );
            result = ( result > 0 ? 1 : ( result < 0 ? -1 : 0 ) );
		}
        //result = ( *(wchar *)string1 - *(wchar *)string2 );
/*lint -restore */
        return result;
    }
}
#endif
/*****************************************************************************/
//  Description : wstrlen
//  Global resource dependence :
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC size_t MMIAPICOM_Wstrlen( const wchar* str )
{
    const wchar* ustr = str;

    if( PNULL == str )
    {
        return 0;
    }

    while( *ustr++ )
        ;

    return( (size_t)(ustr - str - 1) );
}

/*****************************************************************************/
//  Description : wstrnlen
//  Global resource dependence :
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC size_t MMIAPICOM_Wstrnlen( const wchar* str, size_t max_len )
{
    const wchar* ustr = str;

    if( PNULL == str || max_len == 0)
    {
        return 0;
    }

    while( (max_len--) && *ustr )
    {
        ustr++;
    }

    return( (size_t)(ustr - str) );
}

/*****************************************************************************/
//  Description : check whether string1 contain string2
//  Global resource dependence :
//  Author:   liqing.peng 2006-7-16
//  RETRUN:
//  Note:
/*****************************************************************************/
wchar* MMIAPICOM_Wstrstr(wchar *str1, wchar *str2)
{
    int len1 = MMIAPICOM_Wstrlen(str1);
    int len2 = MMIAPICOM_Wstrlen(str2);

    if(!len2)
    {
        return str1;
    }

    if(len1 < len2)
    {
        return PNULL;
    }

    for(; *str1; str1++)
    {
        if((*str1 == *str2) && MMIAPICOM_Wstrncmp(str1, str2, len2) == 0)
        {
            return str1;
        }
    }
    return PNULL;
}

/*****************************************************************************/
//  Description : check whether string1 contain string2
//  Global resource dependence :
//  Author:   liqing.peng 2006-7-16
//  RETRUN:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICOM_Wstrnstr(
                                  const wchar  *str1_ptr,
                                  uint16       str1_len,
                                  const wchar  *str2_ptr,
                                  uint16       str2_len
                                  )
{
    BOOLEAN is_match = FALSE;
    int32   i = 0;

    if( str2_len <= str1_len)
    {
        for(i = 0; i < str1_len-str2_len+1; i++, str1_ptr++)
        {
            if((*str1_ptr == *str2_ptr) && MMIAPICOM_Wstrncmp(str1_ptr, str2_ptr, str2_len) == 0)
            {
                is_match = TRUE;
                break;
            }
        }
    }
    return is_match;
}

/*****************************************************************************/
//  Description : check whether string1 contain string2 ext
//  Global resource dependence :
//  Author:   tao.xue
//  RETRUN:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICOM_WstrnstrExt(
                                  const wchar  *str1_ptr,
                                  uint16       str1_len,
                                  const wchar  *str2_ptr,
                                  uint16       str2_len,
                                  BOOLEAN        str_flag     //是否字符串区分大小写
                                  )
{
    BOOLEAN result = FALSE;
    wchar   *str1 = PNULL;
    wchar   *str2 = PNULL;

    if (!str_flag)
    {
        str1 =  SCI_ALLOCA(sizeof(wchar)*(str1_len+1));
        SCI_MEMSET(str1,0x00,(sizeof(wchar)*(str1_len+1)));
        MMIAPICOM_Wstrncpy(str1,str1_ptr,str1_len);

        str2 =  SCI_ALLOCA(sizeof(wchar)*(str2_len+1));
        SCI_MEMSET(str2,0x00,(sizeof(wchar)*(str2_len+1)));
        MMIAPICOM_Wstrncpy(str2,str2_ptr,str2_len);

        MMIAPICOM_Wstrlower(str1);
        MMIAPICOM_Wstrlower(str2);

        result = MMIAPICOM_Wstrnstr(str1,str1_len,str2,str2_len);
    }
    else
    {
        result = MMIAPICOM_Wstrnstr(str1_ptr,str1_len,str2_ptr,str2_len);
    }

    if (!str_flag)
    {
        SCI_FREE(str1);
        SCI_FREE(str2);
    }

    return result;
}

/*****************************************************************************/
//  Description : wchar tolower
//  Global resource dependence :
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC wchar MMIAPICOM_Wchartolower(
                                    wchar wc
                                    )
{
    if ((wc >= 0x0041 && wc <= 0x005A) || /* Latin basic */
        (wc >= 0x00C0 && wc <= 0x00D6) || (wc >= 0x00D8 && wc <= 0x00DE) || /* Latin-1 */
        (wc >= 0x0410 && wc <= 0x042F) ) /* Cyrillic */
    {
        return (wc + 0x0020);
    }
    /* Greek */
    else if ((wc >= 0x0391 && wc <= 0x03A1) || (wc >= 0x03A3 && wc <= 0x03AB))
    {
        return (wc + 0x20);
    }
    else if (wc == 0x038F || wc == 0x038E)
    {
        return (wc + 0x3F);
    }
    else if (wc == 0x038C)
    {
        return (wc + 0x40);
    }
    else if (wc >= 0x0388 && wc <= 0x038A)
    {
        return (wc + 0x25);
    }
    else if (wc == 0x0386)
    {
        return (wc + 0x26);
    }
    /* Greek */
    else if (wc == 0x0102 || /* Latin extend-A */
        wc == 0x01A0 || wc == 0x01AF || wc == 0x0187 || wc == 0x018B ||  /* Latin extend-B */
        wc == 0x0191 || wc == 0x01AB || wc == 0x01B8 || wc == 0x01F2) /* Latin extend-B */
    {
        return (wc + 0x0001);
    }
    else if (wc >= 0x0400 && wc <= 0x040F) /* Cyrillic */
    {
        return (wc + 0x50);
    }
    else if (wc == 0x0490) /* Cyrillic */
    {
        return 0x0491;
    }
    else if (wc >= 0x0100 && wc <= 0x0137 || wc >= 0x014A && wc <= 0x0177 || /* Latin extend-A */
        wc >= 0x0182 && wc <= 0x0185 || wc >= 0x0198 && wc <= 0x01A5 || /* Latin extend-B */
        wc >= 0x01DE && wc <= 0x01EF || wc >= 0x01F8 && wc <= 0x01FF || /* Latin extend-B */
        wc >= 0x0222 && wc <= 0x0233 || wc >= 0x1E00 &&  wc <= 0x1EF8) /*Latin Extended Additional */
    {
        if ((wc & 0x0001) == 0) /* odd is small case */
            return wc + 1;
        else
            return wc;
    }
    else if ( (wc >= 0x0460) && (wc <= 0x0500) )
    {
        if ((wc & 1) == 1) /* odd is small case */
            return wc;
        else
            return wc +1;
    }
    else if (wc >= 0x0139 && wc <= 0x0148 ||  /* Latin extend-A */
        wc >= 0x0179 && wc <= 0x017E ||wc >= 0x01B3 && wc <= 0x01B6 ||  /* Latin extend-B */
        wc >= 0x01CD && wc <= 0x01DC) /* Latin extend-B*/
    {
        if ((wc & 0x0001) == 1) /* even is small case */
            return wc + 1;
        else
            return wc;
    }
    else if (wc >= 0x0531 && wc <= 0x0556) /* Armenian */
    {
        return (wc + 0x30);
    }
    else if (wc == 0x181) /* Hausa */
    {
        return 0x253;
    }
    else if (wc == 0x18A ) /* Hausa */
    {
        return 0x257;
    }
    else if (wc ==  0x18F) /* Azerbaijani */
    {
        return 0x259 ;
    }
    else
    {
        return wc;
    }
}

/*****************************************************************************/
//  Description : wchar toupper
//  Global resource dependence :
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC wchar MMIAPICOM_Wchartoupper(
                                    wchar wc
                                    )
{
    if ((wc >= 0x0061 && wc <= 0x007A) || /* Latin basic */
        (wc >= 0x00E0 && wc <= 0x00F6) || (wc >= 0x00F8 && wc <= 0x00FE) || /* Latin-1 */
        (wc >= 0x0430 && wc <= 0x044F)) /* Cyrillic */
    {
        return (wc - 0x0020);
    }
    /* Greek */
    else if ((wc >= 0x03B1 && wc <= 0x03C1) || (wc >= 0x03C3 && wc <= 0x03CB))
    {
        return (wc - 0x20);
    }
    else if (wc == 0x03CE || wc == 0x03CD)
    {
        return (wc - 0x3F);
    }
    else if (wc == 0x03CC)
    {
        return (wc - 0x40);
    }
    else if (wc >= 0x03AD && wc <= 0x03AF)
    {
        return (wc - 0x25);
    }
    else if (wc == 0x03AC)
    {
        return (wc - 0x26);
    }
    else if (wc == 0x0253)
    {
        return (wc - 0xD2);
    }
    else if (wc == 0x0257)
    {
        return (wc - 0xCD);
    }
    else if (wc == 0x0259)
    {
        return (wc - 0xCA);
    }
    /* Greek */
    else if (wc == 0x0103 || /* Latin extend-A */
        wc == 0x01A1 || wc == 0x01B0 || wc == 0x0188 || wc == 0x018C ||  /* Latin extend-B */
        wc == 0x0192 || wc == 0x01AC || wc == 0x01B9 || wc == 0x01F3) /* Latin extend-B */
    {
        return (wc - 0x0001);
    }
    else if (wc >= 0x0450 && wc <= 0x045f) /* Cyrillic */
    {
        return (wc - 0x50);
    }

    else if ( ( (wc >= 0x0461) && (wc <= 0x04f9) ) || ( (wc >= 0x1E01) && (wc <= 0x1EF9)) )
    {
        if ((wc & 1) == 1) /* odd is small case */
            return wc - 1;
        else
            return wc;
    }
    else if (wc >= 0x0100 && wc <= 0x0137 ||wc >= 0x014A && wc <= 0x0177 || /* Latin extend-A */
        wc >= 0x0182 && wc <= 0x0185 ||wc >= 0x0198 && wc <= 0x01A5 || /* Latin extend-B */
        wc >= 0x01DE && wc <= 0x01EF ||wc >= 0x01F8 && wc <= 0x01FF || /* Latin extend-B */
        wc >= 0x0222 && wc <= 0x0233) /* Latin extend-B */
    {
        if ((wc & 1) == 1) /* odd is small case */
            return wc - 1;
        else
            return wc;
    }
    else if (wc >= 0x0139 && wc <= 0x0148 || /* Latin extend-A */
        wc >= 0x0179 && wc <= 0x017E || wc >= 0x01B3 && wc <= 0x01B6 ||  /* Latin extend-B */
        wc >= 0x01CD && wc <= 0x01DC) /* Latin extend-B*/
    {
        if ((wc & 1) == 0) /* even is small case */
            return wc - 1;
        else
            return wc;
    }
    else if (wc >= 0x0561 && wc <= 0x0586) /* Armenian */
    {
        return (wc - 0x30);
    }
    else
    {
        return wc;
    }
}

/*****************************************************************************/
//  Description : wstr lower
//  Global resource dependence :
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC wchar* MMIAPICOM_Wstrlower( wchar * string )
{
    wchar *start = string;

    if( PNULL != string )
    {
        while (NULL != (*string = MMIAPICOM_Wchartolower( *string )) )/*lint !e564 */
            string++;
    }

    return start;
}

/*****************************************************************************/
//  Description : wstr upper
//  Global resource dependence :
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC wchar* MMIAPICOM_Wstrupper( wchar * string )
{
    wchar *start = string;

    if( PNULL != string )
    {
        while (NULL != (*string = MMIAPICOM_Wchartoupper( *string )) )/*lint !e564 */
            string++;
    }

    return start;
}

/*****************************************************************************/
//    Description : destroy num string
//    Global resource dependence :
//    Author: James.Zhang
//    Note:
/*****************************************************************************/
PUBLIC void MMIAPICOM_DestroyString(
                            MMI_STRING_T num_string
                            )
{
    if (PNULL != num_string.wstr_ptr)
    {
        SCI_FREE( num_string.wstr_ptr );
    }
    else
    {
        //SCI_TRACE_LOW:"PNULL != num_string.wstr_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICOM_STRING_1209_112_2_18_1_59_13_105,(uint8*)"");
    }
}

/*****************************************************************************/
//  Description : copy unicode string
//  Global resource dependence : none
//  Author: taul.zhu
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPICOM_UnicodeStrcpy(
                                    MMI_STRING_T  *o1_dest_text,        // DESTINATION
                                    MMI_STRING_T i1_src_text                // SOURCE
                                    )
{
      int16 len = 0;

      len = MIN( o1_dest_text->wstr_len, i1_src_text.wstr_len );

      MMI_WSTRNCPY( o1_dest_text->wstr_ptr, o1_dest_text->wstr_len, i1_src_text.wstr_ptr, i1_src_text.wstr_len, len );

      o1_dest_text->wstr_len = len;

} /* end UnicodeStrcpy */

/*****************************************************************************/
//  Description : connect some string
//  Global resource dependence :
//  Author: bin.ji
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICOM_ConnectSomeString(
                                           MMI_STRING_T        *string_in,     //in
                                           uint32              string_num,     //in
                                           MMI_STRING_T        *string_out     //out
                                           )
{
    wchar *str_ptr = PNULL;
    uint16 str_len = 0;
    uint32 i = 0;

    if ((PNULL == string_out) || (PNULL == string_out->wstr_ptr) || (PNULL == string_in))
    {
        return FALSE;
    }

    str_ptr = string_out->wstr_ptr;
    str_len = string_out->wstr_len;

    for (i = 0; i < string_num; i++)
    {
        if ((PNULL != string_in[i].wstr_ptr) && (string_in[i].wstr_len <= str_len))
        {
            MMI_WSTRNCPY( str_ptr, str_len, string_in[i].wstr_ptr, string_in[i].wstr_len, string_in[i].wstr_len );
            str_ptr += string_in[i].wstr_len;
            str_len -= string_in[i].wstr_len;
        }
    }
    return TRUE;
}

/*****************************************************************************/
//  Description : merge two string
//  Global resource dependence :
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICOM_MergeTwoString(
                                        MMI_STRING_T        *string_1,      //in
                                        MMI_STRING_T        *string_2,      //in
                                        MMI_STRING_T        *string_3,      //out
                                        uint16              str3_len        //in
                                        )
{
    uint16      str_len = 0;

    if (PNULL == string_1 || PNULL == string_2 || PNULL == string_3)
    {
        //SCI_TRACE_LOW:"MMIAPICOM_MergeTwoString PNULL == string_1 || PNULL == string_2 || PNULL == string_3"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICOM_STRING_553_112_2_18_1_59_12_93,(uint8*)"");
        return FALSE;
    }

    // check parameters
    if((NULL == string_1->wstr_ptr && NULL == string_2->wstr_ptr) || NULL == string_3->wstr_ptr || 0 == str3_len)
    {
        return FALSE;
    }

    string_3->wstr_len = 0;
    if(string_1->wstr_ptr)
    {
        // check if string_1 has content to copy
        if((str_len = MIN(str3_len, string_1->wstr_len)) > 0)
        {
            MMI_WSTRNCPY( string_3->wstr_ptr, str3_len, string_1->wstr_ptr, string_1->wstr_len, str_len );
            string_3->wstr_len = str_len;
        }
    }

    if(string_2->wstr_ptr)
    {
        // check if string_2 has content to copy and if string_3 has space to copy
        if((str_len = MIN((str3_len - string_3->wstr_len), string_2->wstr_len)) > 0)
        {
            MMI_WSTRNCPY( string_3->wstr_ptr + string_3->wstr_len, str3_len-string_3->wstr_len,\
                    string_2->wstr_ptr, string_2->wstr_len, str_len );
            string_3->wstr_len += str_len;
        }
    }

    return TRUE;
}

/*****************************************************************************/
//     Description : cat two string
//    Global resource dependence :
//  Author:
//    Note:    should free the dst_ptr->str_ptr
///*****************************************************************************/
PUBLIC BOOLEAN MMIAPICOM_CatTwoString(
                                      MMI_STRING_T *dst_ptr,        //out.
                                      MMI_STRING_T *src1_ptr,        //in.
                                      MMI_STRING_T *src2_ptr        //in.
                                      )
{
    uint32    str1_len = 0;
    wchar    *str1_text_ptr = PNULL;
    uint32    str2_len = 0;
    wchar    *str2_text_ptr = PNULL;

    if(PNULL==dst_ptr || PNULL==src1_ptr || PNULL==src2_ptr)
    {
        //SCI_TRACE_MID:"[MMICOM]:MMIAPICOM_CatTwoString fail, dst_ptr=%d,src1_ptr=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICOM_STRING_1815_112_2_18_1_59_14_119,(uint8*)"dd",dst_ptr,src1_ptr);
        return FALSE;
    }
    //check
    if (src1_ptr->wstr_len == 0 && src2_ptr->wstr_len == 0)
    {
        //SCI_TRACE_MID:"[MMICOM]:MMIAPICOM_CatTwoString fail, len1=%d,len2=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICOM_STRING_1821_112_2_18_1_59_14_120,(uint8*)"dd",src1_ptr->wstr_len,src2_ptr->wstr_len);
        return FALSE;
    }

    str1_text_ptr    = src1_ptr->wstr_ptr;
    str1_len        = src1_ptr->wstr_len;

    str2_text_ptr    = src2_ptr->wstr_ptr;
    str2_len        = src2_ptr->wstr_len;

    //merge
    dst_ptr->wstr_len = str1_len + str2_len;

    dst_ptr->wstr_ptr = SCI_ALLOCA( (dst_ptr->wstr_len + 1) * sizeof(wchar) );

    if (PNULL == dst_ptr->wstr_ptr)
    {
        //SCI_TRACE_LOW:"[MMICOM]:MMIAPICOM_CatTwoString fail, malloc for dst_ptr->str_ptr fail!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICOM_STRING_1838_112_2_18_1_59_15_121,(uint8*)"");
        return FALSE;
    }
    SCI_MEMSET(dst_ptr->wstr_ptr, 0, ((dst_ptr->wstr_len + 1) * sizeof(wchar)));

    MMIAPICOM_MergeTwoString(src1_ptr, src2_ptr, dst_ptr, dst_ptr->wstr_len);
    return TRUE;
}

/*****************************************************************************/
//  Description : 字符串比较函数
//  Global resource dependence : none
//  Author: maqianlin
//  Note:用于比较文字资源和编辑的文字
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICOM_StrCompare(
                                    const MMI_STRING_T          *str_ptr,//in 编辑的文字
                                    const MMI_STRING_T          *str_info_ptr//in文字资源ID
                                    )
{

    int32       result = 0;

    if (PNULL== str_ptr || PNULL == str_info_ptr)
    {
        //SCI_TRACE_LOW:"PNULL != str_ptr, PNULL != str_info_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICOM_STRING_1069_112_2_18_1_59_13_102,(uint8*)"");
        return FALSE;
    }
    //if( str_ptr->is_ucs2 != str_info_ptr->is_ucs2) return FALSE;
    if( str_ptr->wstr_len  != str_info_ptr->wstr_len ) return FALSE;

    result = MMIAPICOM_Wstrncmp( str_ptr->wstr_ptr, str_info_ptr->wstr_ptr, str_ptr->wstr_len );
    //MMIAPICOM_StrCmp(str_ptr->wstr_ptr,str_ptr->wstr_len,str_info_ptr->wstr_ptr,str_info_ptr->wstr_len);

    return (result == 0);
}

/*****************************************************************************/
//  Description : 把一个MMI_STRING_T对应的字符串连接到一个字符串中
//  Global resource dependence :
//  Author:yongwei.he
//  Note: used to textbox
/*****************************************************************************/
PUBLIC void MMIAPICOM_StrcatFromSTRINGToUCS2(
                                             uint16  *ucs2_str_ptr,    //in, out
                                             uint16  *ucs2_str_len_ptr,    //in, out, uint16 numbers, not bytes
                                             const MMI_STRING_T *string_ptr   //in
                                             )
{
    if (PNULL == ucs2_str_ptr || PNULL == ucs2_str_len_ptr || PNULL == string_ptr)
    {
        //SCI_TRACE_LOW:"MMIAPICOM_StrcatFromSTRINGToUCS2 PNULL == ucs2_str_ptr || PNULL == ucs2_str_len_ptr || PNULL == string_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICOM_STRING_646_112_2_18_1_59_12_94,(uint8*)"");
        return;
    }

    MMI_WSTRNCPY( ucs2_str_ptr + (*ucs2_str_len_ptr), string_ptr->wstr_len,
        string_ptr->wstr_ptr, string_ptr->wstr_len, string_ptr->wstr_len );

    *ucs2_str_len_ptr += string_ptr->wstr_len;
}

/*****************************************************************************/
//  Description : 把一个text id对应的字符串连接到一个字符串中
//  Global resource dependence :
//  Author:yongwei.he
//  Note: used to textbox
/*****************************************************************************/
PUBLIC void MMIAPICOM_StrcatFromTextIDToUCS2(
                                             uint16  *ucs2_str_ptr,    //in, out
                                             uint16  *ucs2_str_len_ptr,    //in, out, uint16 numbers, not bytes
                                             MMI_TEXT_ID_T text_id   //in
                                             )
{
    MMI_STRING_T text_string = {0};

    MMI_GetLabelTextByLang( text_id, &text_string );

    MMIAPICOM_StrcatFromSTRINGToUCS2( ucs2_str_ptr, ucs2_str_len_ptr, &text_string );
}

/*****************************************************************************/
//     Description : cat two string by a textid and a string
//    Global resource dependence :
//  Author:
//    Note:    should free the dst_ptr->str_ptr
///*****************************************************************************/
PUBLIC BOOLEAN MMIAPICOM_CatIDAndString(
                                        MMI_STRING_T    *dst_ptr,        //out.
                                        uint32          text_id,        //in.
                                        MMI_STRING_T    *src2_ptr        //in.
                                        )
{
    MMI_STRING_T string_prefix = {0};

    if(PNULL ==dst_ptr || PNULL ==src2_ptr)
    {
        //SCI_TRACE_LOW:"warning CatStringByTextID:dst_ptr=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICOM_STRING_1863_112_2_18_1_59_15_122,(uint8*)"d",dst_ptr);
        return FALSE;
    }
    MMI_GetLabelTextByLang(text_id, &string_prefix);    //don't need to free string_prefix.str_ptr
    return    MMIAPICOM_CatTwoString(dst_ptr, &string_prefix, src2_ptr);
}


/*****************************************************************************/
//     Description : cat two string by two textids
//    Global resource dependence :
//  Author:
//    Note:    should free the dst_ptr->str_ptr
///*****************************************************************************/
PUBLIC BOOLEAN MMIAPICOM_CatTwoID(
                                  MMI_STRING_T      *dst_ptr,        //out.
                                  uint32            text_id1,        //in.
                                  uint32            text_id2        //in.
                                  )
{
    MMI_STRING_T string_prefix = {0};
    MMI_STRING_T string_subfix = {0};

    if(PNULL==dst_ptr)
    {
        //SCI_TRACE_LOW:"warning CatStringByTextID2 dst_ptr NULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICOM_STRING_1887_112_2_18_1_59_15_123,(uint8*)"");
        return FALSE;
    }
    MMI_GetLabelTextByLang(text_id1, &string_prefix);    //don't need to free string_prefix.str_ptr
    MMI_GetLabelTextByLang(text_id2, &string_subfix);
    return    MMIAPICOM_CatTwoString(dst_ptr, &string_prefix, &string_subfix);
}

/*****************************************************************************/
//  Description : Get transmitter display string
//  Global resource dependence :
//  Author: Tracy Zhang
//  Note:
/*****************************************************************************/
PUBLIC wchar MMIAPICOM_GetCRCh( void )
{
    return 0x0d;
}

/*****************************************************************************/
//  Description : search the same name in album list
//  Global resource dependence :
//  Author: bruce.chi
//  Return: TRUE: contain invalid char. FALSE: do not contain invalid char
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICOM_IsIncludeInvalidChar(
                                              const wchar* name_ptr,
                                              uint16 len//,
                                              //BOOLEAN is_ucs2
                                              )
{
    const uint8 invalid_char[] = {'\\', '/', ':', '*', '?', '"', '<', '>', '|',0x0D, 0x0A};
    int32 i = 0;
    int32 j = 0;
    uint8 invalid_char_num  = sizeof(invalid_char) ;

    if (name_ptr == PNULL || len == 0)
    {
        return TRUE;
    }

    for(i=0; i<len; i++)
    {
        for (j=0; j< invalid_char_num; j++)
        {
            if (*(name_ptr+i) == invalid_char[j])
            {
                return TRUE;
            }
        }
    }

    return FALSE;
}

/*****************************************************************************/
//  Description : 判断是否为 UCS2 字符
//  Global resource dependence : none
//  Author: ZhangJuntao
//  Note:
/*****************************************************************************/
PUBLIC  BOOLEAN MMIAPICOM_IsUcs2Char(uint16 ch)
{
    return (ch & 0xFF80) != 0;
}

/*****************************************************************************/
//  Description : 判断wstring是否为 ASCII 字符串
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC  BOOLEAN MMIAPICOM_IsASCIIString(
                                        const wchar* wstr_ptr,
                                        uint16 wstr_len
                                        )
{
    BOOLEAN is_ascii = TRUE;
    uint32  i = 0;

    if( PNULL != wstr_ptr )
    {
        for ( ; i < wstr_len; i++ )
        {
            if( MMIAPICOM_IsUcs2Char( wstr_ptr[i] ) )
            {
                is_ascii = FALSE;

                break;
            }
        }
    }

    return is_ascii;
}

/*****************************************************************************/
//  Description : 获取wstr_ptr中以ASCII 字符串开头的长度
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC  uint16 MMIAPICOM_GetASCIIStringLen(
                                        const wchar* wstr_ptr,
                                        uint16 wstr_len
                                        )
{
    uint32  i = 0;
    uint16  len = 0;
    if( PNULL != wstr_ptr )
    {
        for ( ; i < wstr_len; i++ )
        {
            if( MMIAPICOM_IsUcs2Char( wstr_ptr[i] ) )
            {
                break;
            }
            len++;
        }
    }
    return len;
}

/*****************************************************************************/
//  Description : 是否是数字串
//  Global resource dependence :
//  Author:yelianna.zhao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICOM_IsNumberString(
                                  uint16* cmd_char,
                                  uint16 len
                                  )
{   BOOLEAN ret = TRUE;
    wchar  wchar_val = 0;
    int32 i =0;

    if (PNULL == cmd_char || len == 0)
    {
        //SCI_TRACE_LOW:"MMIAPICOM_IsNumberString PNULL == cmd_char"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICOM_STRING_1394_112_2_18_1_59_14_109,(uint8*)"");
        return ret;
    }

    for (i=0; i<len; i++)
    {
        wchar_val = *(cmd_char+i);
        if ((wchar_val < 0x30) || (wchar_val > 0x39))
        {
            ret = FALSE;
            break;
        }
    }
    return ret;
}

/*****************************************************************************/
//  Description : 计算字符串中ucs2字符的个数
//  Global resource dependence : none
//  Author: ZhangJuntao
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMIAPICOM_CountUcs2Char(uint16 *txt_ptr, uint16 len)
{
    uint16 i, count=0;

    if (PNULL == txt_ptr)
    {
        //SCI_TRACE_LOW:"MMIAPICOM_CountUcs2Char PNULL == txt_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICOM_STRING_1327_112_2_18_1_59_14_108,(uint8*)"");
        return count;
    }
    for (i = 0; i < len; i++)
    {
        if (MMIAPICOM_IsUcs2Char(txt_ptr[i]) )
        {
            count++;
        }
    }

    return count;
}

/*****************************************************************************/
//  Description : translate the string to number
//  Global resource dependence :
//  Author:louis.wei
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICOM_WstrToNum(
                                   const wchar* str,
                                   uint8   len,
                                   uint32*  num
                                   )
{
    int32       i = 0;
    BOOLEAN     result = FALSE;

    if (PNULL == str || PNULL == num)
    {
        //SCI_TRACE_LOW:"PNULL != str, PNULL != num"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICOM_STRING_1103_112_2_18_1_59_13_103,(uint8*)"");
        return FALSE;
    }
    //清零
    *num = 0;

    for( i = 0; i < len; i++ )
    {
        result = TRUE;
        if( ( '0' <= str[i] ) && ( '9' >= str[i] ) )
        {
            *num = ( *num ) * 10 + ( str[i] - '0');
        }
        else
        {
            result = FALSE;
            break;
        }
    }

    return result;
}

/********************************************************************************
 NAME:          Int2Str
 DESCRIPTION:
 PARAM IN:      value -
                buf -
                buf_len -
 PARAM OUT:     None
 AUTHOR:        allen
 DATE:          2004.09.14
********************************************************************************/
PUBLIC void MMIAPICOM_Int2Str(uint32 value, uint8* buf, uint8 buf_len)
{
    int32 i = 0, j = 0;
    int32 semi_len = 0;
    int32 offset = 0;
    uint8 tmp_chr = 0x00;

    if (PNULL == buf || buf_len <2)
    {
        //SCI_TRACE_LOW:"MMIAPICOM_Int2Str PNULL == buf || buf_len <2"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICOM_STRING_1287_112_2_18_1_59_13_107,(uint8*)"");
        return ;
    }
    if (value == 0)
    {
        buf[i] = (uint8)(0 + '0');
        buf[i + 1] = 0;
        return;
    }
    while (value != 0)
    {
        buf[i] = (uint8)((value % 10) + '0');
        value /= 10;
        i++;
        if (i >= buf_len)
            break;
    }
    if (i < buf_len)
        buf[i] = 0;
    semi_len = i / 2;
    offset = i % 2 - 1;
    for (j = 0; j < semi_len; j++)
    {
        tmp_chr = buf[j];
        buf[j] = buf[2 * semi_len + offset - j];
        buf[2 * semi_len + offset - j] = tmp_chr;
    }
}


/*****************************************************************************/
//  Description : 75进制转换成10进制
//  Global resource dependence :
//  Author:yelianna.zhao
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMIAPICOM_ConvertCmdChar2Num (
                                  uint16* cmd_char,
                                  uint16 len
                                  )
{   uint16 number = 0;
    uint8  u8_char;
    int32 i =0;

    if (PNULL == cmd_char)
    {
        //SCI_TRACE_LOW:"MMIAPICOM_ConvertCmdChar2Num PNULL == cmd_char"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICOM_STRING_1424_112_2_18_1_59_14_110,(uint8*)"");
        return number;
    }

    for (i=0; i<len; i++)
    {
        u8_char = *(cmd_char+i);
        if ((u8_char >= MMICOM_CMD_START_CHAR) && (u8_char <= MMICOM_CMD_END_CHAR))
        {
            number = number * MMICOM_NUMBER_SYSTEM + (u8_char - MMICOM_CMD_START_CHAR) ;
        }
        else
        {
            //SCI_TRACE_LOW:"MMIAPICOM_ConvertCmdChar2Num u8_char=%d,i=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICOM_STRING_1437_112_2_18_1_59_14_111,(uint8*)"dd",u8_char,i);
        }
    }

    return number;
}

/*****************************************************************************/
//  Description : Get phone number form string
//  Global resource dependence :
//  Author:jian.ma
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPICOM_GetPhoneNumberFormString(uint8* string,uint32 len,uint8* num)
{
    uint32 i = 0;
    uint32 j = 0;

    if(PNULL==string||PNULL==num)
    {
        return;
    }

    for (i= 0,j=0;i<len;i++)
    {
        if(string[i]>=0x30 && string[i]<=0x39)
        {
            num[j] = string[i];
            j++;
        }
    }
}

PUBLIC BOOLEAN  MMIAPICOM_CustomGenerateReplaceStr
(
const MMI_STRING_T *src_str,
const wchar* find_str,
const wchar* replace_str,
uint16 result_max_len,
MMI_STRING_T *result_str//out
)
{
       MMI_STRING_T text_str = {0};
       uint16 array_index = 0;
       wchar* str1 = PNULL;
       wchar* find_pos = PNULL;
       int src_str_remain_len = 0;
       int src_str_len = 0;
       int find_str_len = 0;
       int replace_str_len = 0;
       uint16 remain_len = result_max_len;
       uint16 min_len = 0;
       if(PNULL == src_str 
              || PNULL == src_str->wstr_ptr
              || PNULL == result_str 
              || PNULL == result_str->wstr_ptr
              || PNULL == find_str
              || PNULL == replace_str
              )
       {
              return FALSE;
       }
              
       src_str_remain_len = src_str_len = src_str->wstr_len;
       find_str_len = MMIAPICOM_Wstrlen(find_str);
       replace_str_len = MMIAPICOM_Wstrlen(replace_str);
       
       if(0 == src_str_len || 0 == find_str_len)
       {
              return FALSE;
       }
       
       str1 = src_str->wstr_ptr;
       find_pos = MMIAPICOM_Wstrstr(str1, find_str);
       if(PNULL == find_pos)
       {
              result_str->wstr_len = MIN(remain_len, src_str_len);
              MMIAPICOM_Wstrncpy(result_str->wstr_ptr, src_str->wstr_ptr, result_str->wstr_len);
              return FALSE;
       }
       str1 = src_str->wstr_ptr; 
       for(; *str1; str1++)
       {
              array_index++;
              if(str1 == find_pos) break;       
       }
       
       if(array_index > src_str_len)
       {
              result_str->wstr_len = MIN(remain_len, src_str_len);
              MMIAPICOM_Wstrncpy(result_str->wstr_ptr, src_str->wstr_ptr, result_str->wstr_len);
              return FALSE;
       }
       
       result_str->wstr_len = MIN(remain_len, array_index-1);
       MMIAPICOM_Wstrncpy(result_str->wstr_ptr, src_str->wstr_ptr, result_str->wstr_len);
       src_str_remain_len -= array_index-1;

       remain_len = remain_len - result_str->wstr_len;
       min_len = MIN(remain_len, replace_str_len);
       MMIAPICOM_Wstrncat(result_str->wstr_ptr, replace_str, min_len);
       result_str->wstr_len += min_len;
       src_str_remain_len -= find_str_len;
       
       str1 += find_str_len;
       remain_len = remain_len - min_len;
       min_len = MIN(remain_len, src_str_remain_len);
       MMIAPICOM_Wstrncat(result_str->wstr_ptr, str1, min_len);
       result_str->wstr_len += min_len;
       
       return TRUE;
       
}


PUBLIC BOOLEAN  MMIAPICOM_CustomGenerateNumReplaceStrByTextId
(
MMI_TEXT_ID_T text_id,
const wchar* find_str,
uint16 num,
uint16 result_max_len,
MMI_STRING_T *result_str
)
{
       MMI_STRING_T    src_str = {0};
       char num_str[10+1] = {0};
       wchar num_wstr[10+1] = {0};
       sprintf((char *)num_str,"%d",num);
       MMIAPICOM_StrToWstr(num_str, num_wstr);
       MMI_GetLabelTextByLang(text_id, &src_str);
       return MMIAPICOM_CustomGenerateReplaceStr(&src_str, find_str, num_wstr, result_max_len, result_str);
}

PUBLIC BOOLEAN  MMIAPICOM_CustomGenerateFloatNumReplaceStrByTextId
(
MMI_TEXT_ID_T text_id,
const wchar* find_str,
float num,
uint16 result_max_len,
MMI_STRING_T *result_str
)
{
	MMI_STRING_T    src_str = {0};
	char num_str[10+1] = {0};
	wchar num_wstr[10+1] = {0};
	sprintf((char *)num_str,"%.2f",num); 
  	MMIAPICOM_StrToWstr(num_str, num_wstr);
	MMI_GetLabelTextByLang(text_id, &src_str);
 	return MMIAPICOM_CustomGenerateReplaceStr(&src_str, find_str, num_wstr, result_max_len, result_str);
}

/***************************************************************************/
//  Description:
//  Global resource dependence
//  Auther:Bill.Ji
//  Note
/****************************************************************************/
#ifndef STANDALONE_TEST
PUBLIC void MMIAPICOM_GBToSTRING(
                                 const uint8   *src_ptr,
                                 MMI_STRING_T  *des_str
                                 )
{
    uint16    str_len = 0;
    wchar     *p_buf = PNULL;

    if (PNULL == src_ptr || PNULL == des_str)
    {
        //SCI_TRACE_LOW:"MMIAPICOM_GBToSTRING PNULL == src_ptr || PNULL == des_str"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICOM_STRING_1494_112_2_18_1_59_14_112,(uint8*)"");
        return ;
    }

    if (PNULL != des_str->wstr_ptr )
    {
        SCI_FREE( des_str->wstr_ptr);
    }
    //Check is GB code
    str_len = strlen((const char *)src_ptr);

    p_buf = SCI_ALLOCA( ( str_len + 1 ) * sizeof(wchar) );

    des_str->wstr_len = GUI_GBToWstr( p_buf, src_ptr, str_len );
    des_str->wstr_ptr = p_buf;

    return;
}
#endif

/*****************************************************************************/
// 	Description : copy string to buffer
//	Global resource dependence :
//  Author: Tracy Zhang
/*****************************************************************************/
PUBLIC uint16 MMIAPICOM_CopyStr2Buf(wchar *dst_buf_ptr, MMI_STRING_T const *src_ptr, uint16 max_copy_len)
{
    uint16 copy_length = 0;

    if (PNULL == dst_buf_ptr || PNULL == src_ptr )
    {
        //SCI_TRACE_LOW:"MMIAPICOM_CopyStr2Buf PNULL == dst_buf_ptr || PNULL == src_ptr "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICOM_STRING_1782_112_2_18_1_59_14_118,(uint8*)"");
        return copy_length;
    }
    copy_length = MIN(max_copy_len, src_ptr->wstr_len);

    MMI_WSTRNCPY(dst_buf_ptr,
                            max_copy_len,
                            src_ptr->wstr_ptr,
                            src_ptr->wstr_len,
                            copy_length);

    return copy_length;
}

/*****************************************************************************/
//  Description : to get the length according the param
//  Global resource dependence : none
//  Author: arthur.peng
//  Note:
/*****************************************************************************/
PUBLIC int32 MMIAPICOM_GetInt32ValueFromBuf(
                     uint8 * dataPtr,
                     uint16 len
                     )
{
    int32 result =0;
    int32 i = 0;

    if (PNULL == dataPtr || len == 0)
    {
        //SCI_TRACE_LOW:"MMIAPICOM_GetInt32ValueFromBuf PNULL == dataPtr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICOM_STRING_691_112_2_18_1_59_12_96,(uint8*)"");
        return result;
    }

    // only last 4 bytes are useful
    if(len > 4) i = len - 4;
    for(; i < len; i++)
    {
        result = (result <<8 ) + (dataPtr[i] & 0xFF );
    }
    return result;
}

/*****************************************************************************/
//  Description : to get the uint8 value of the buffer
//  Global resource dependence : none
//  Author: arthur.peng
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMIAPICOM_GetInt8ValueFromBuf(
                     uint8 * dataPtr,
                     uint16 len
                     )
{
    uint8 result =0;
    int32 i = 0;

    if (PNULL == dataPtr || len == 0)
    {
        //SCI_TRACE_LOW:"MMIAPICOM_GetInt8ValueFromBuf PNULL == dataPtr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICOM_STRING_716_112_2_18_1_59_12_97,(uint8*)"");
        return result;
    }
    // only last byte is useful
    if(len > 1) i = len - 1;
    result = dataPtr[i] & 0xFF;
    return result;
}

/*****************************************************************************/
//  Description : to get the length according the param
//  Global resource dependence : none
//  Author: arthur.peng
//  Note:
/*****************************************************************************/
PUBLIC int16 MMIAPICOM_GetInt16ValueFromBuf(
                     uint8 * dataPtr,
                     uint16 len
                     )
{
    int16 result =0;
    int32 i = 0;

    if (PNULL == dataPtr)
    {
        //SCI_TRACE_LOW:"MMIAPICOM_GetInt16ValueFromBuf PNULL == dataPtr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICOM_STRING_740_112_2_18_1_59_12_98,(uint8*)"");
        return result;
    }
    // only last 2 bytes is useful
    if(len > 2) i = len - 2;
    for(; i < len; i++)
    {
        result = (result << 8) + (dataPtr[i] & 0xFF);
    }
    return result;
}

/*******************************************************************************************/
//  Description : 获得在指定区域内符串的宽度和长度
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*********************************************************************************************/
#ifndef STANDALONE_TEST
PUBLIC uint16 MMIAPICOM_GetStrWidth(
                                    const wchar     *str_ptr,       //in:
                                    uint16          *str_len_ptr,   //in/out:
                                    uint16          str_space,      //in:
                                    uint16          display_width,  //in:
                                    GUI_FONT_T      font_type       //in:
                                    )
{
    uint16  str_width = 0;
    uint16  unicode = 0;
    uint16  font_width = 0;
    uint32  i = 0;

    if (PNULL == str_ptr || PNULL == str_len_ptr || font_type >= GUI_MAX_FONT_NUM )
    {
        //SCI_TRACE_LOW:"MMIAPICOM_GetStrWidth PNULL == str_ptr || PNULL == str_len_ptr || font_type >= GUI_MAX_FONT_NUM"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICOM_STRING_1549_112_2_18_1_59_14_113,(uint8*)"");
        return str_width;
    }
    for (i=0; i<*str_len_ptr; i++)
    {
        //add string space
        if (0 != i)
        {
            str_width += str_space;
        }

        //get unicode
        unicode = (uint16)(*(str_ptr+i));

        //string width
        font_width = GUI_GetFontWidth(font_type,unicode);
        str_width += font_width;
        if (str_width > display_width)
        {
            *str_len_ptr = i;
            if (str_width > (str_space + font_width))
            {
                str_width = str_width - str_space - font_width;
            }
            else
            {
                //SCI_TRACE_LOW:"MMIAPICOM_GetStrWidth str_width= %d,str_space=%d,font_width=%d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICOM_STRING_1575_112_2_18_1_59_14_114,(uint8*)"ddd",str_width,str_space,font_width);
                str_width = 0;
            }
            break;
        }
    }

    return (str_width);
}
#endif

#ifdef STANDALONE_TEST
#ifdef _MMI_STRING_UNIT_TEST_
const wchar wptr[32] = {0x0030, 0x0031, 0x0032};
void main(void)
{
    MMI_STRING_T *str1 = PNULL;
    MMI_STRING_T *str2 = PNULL;
    MMI_STRING_T *str3 = PNULL;
    MMI_STRING_T string1 = {0};
    MMI_STRING_T string2 = {0};
    MMI_STRING_T string3 = {0};
    wchar wchar1[64];
    wchar wchar2[64];
    wchar wchar3[129];
    char char1[64];
    char *char_ptr = PNULL;
    int i;

    memset(wchar3, 0x0, 258);
    MMIAPICOM_MergeTwoString(str1, str2, str3, 0);

    str1 = &string1;
    str2 = &string2;
    str3 = &string3;

    MMIAPICOM_MergeTwoString(str1, str2, str3, 0);

    string1.wstr_ptr = wchar1;
    string3.wstr_ptr = wchar3;

    MMIAPICOM_MergeTwoString(str1, str2, str3, 0);

    MMIAPICOM_MergeTwoString(str1, str2, str3, 64);

    for(i = 0; i < 64; i++)
    {
        wchar1[i] = i + '0';
    }
    //memcpy(string1.wstr_ptr, "1234567890abcdef", 16);
    string1.wstr_len = 64;
    MMIAPICOM_MergeTwoString(str1, str2, str3, 8);
    MMIAPICOM_MergeTwoString(str1, str2, str3, 16);
    MMIAPICOM_MergeTwoString(str1, str2, str3, 64);

    string1.wstr_ptr = 0;
    string2.wstr_ptr = wchar2;
    for(i = 0; i < 64; i++)
    {
        wchar2[i] = 64 + i + '0';
    }
    string2.wstr_len = 64;
    MMIAPICOM_MergeTwoString(str1, str2, str3, 8);
    MMIAPICOM_MergeTwoString(str1, str2, str3, 16);
    MMIAPICOM_MergeTwoString(str1, str2, str3, 64);

    string1.wstr_ptr = wchar1;
    memset(wchar3, 0x0, 258);
    MMIAPICOM_MergeTwoString(str1, str2, str3, 8);
    memset(wchar3, 0x0, 258);
    MMIAPICOM_MergeTwoString(str1, str2, str3, 16);
    memset(wchar3, 0x0, 258);
    MMIAPICOM_MergeTwoString(str1, str2, str3, 32);
    memset(wchar3, 0x0, 258);
    MMIAPICOM_MergeTwoString(str1, str2, str3, 64);
    memset(wchar3, 0x0, 258);
    MMIAPICOM_MergeTwoString(str1, str2, str3, 96);
    memset(wchar3, 0x0, 258);
    MMIAPICOM_MergeTwoString(str1, str2, str3, 128);

    for(i = 0; i < 64; i++)
    {
        char1[i] = i + '0';
    }
    char_ptr = MMIAPICOM_SearchChar(char1, '5');
}
#endif
#endif

/*****************************************************************************/
//  Description : check if need low
//  Global resource dependence : 
//  Author:CBK
//  Note:
/*****************************************************************************/
PUBLIC int MMIAPICOM_WstrnLowCmp(
                              const wchar* string1,
                              const wchar* string2,
                              size_t count
                              )
{
    int result = 0;
    if (!count)
    {
        return(0);
    }

    if( string1 == PNULL || string2 == PNULL )
    {
        if( string1 == PNULL )
        {
            return -1;
        }
        else 
        {
            return 1;
        }
    }
    else
    {
        while ( --count && *string1 && (MMIAPICOM_Wchartolower(*string1) == MMIAPICOM_Wchartolower(*string2)) )
        {
            string1++;
            string2++;
        }

        result = ( MMIAPICOM_Wchartolower(*string1) - MMIAPICOM_Wchartolower(*string2) );

        return ( result > 0 ? 1 : ( result < 0 ? -1 : 0 ) );    
    }
}


/*****************************************************************************/
//  Description : skip blank before and after string
//  Global resource dependence : none
//  Author: CBK
//  Note:skip blank before compare for smart compare
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICOM_StrSmartCompare(
                                         const MMI_STRING_T          *str_ptr,//in 编辑的文字
                                         const MMI_STRING_T          *str_info_ptr,//in文字资源ID
                                         BOOLEAN ifLow,  // if need low
                                         BOOLEAN ifNeedCheckTail
                                         )
{   
    
    int32       result = 0;
    MMI_STRING_T str1 = {0};
    MMI_STRING_T str2 = {0};
    wchar*    wTailPtr = PNULL;
    uint16      iTailPos = 0;
    const wchar wBlank[] = L" ";
    
    if (PNULL== str_ptr || PNULL == str_info_ptr)
    {
        //SCI_TRACE_LOW:"PNULL != str_ptr, PNULL != str_info_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICOM_STRING_1069_112_2_18_1_59_13_102,(uint8*)"");
        return FALSE;
    }
    if (!str_ptr->wstr_len && !str_info_ptr->wstr_len)
    {
        //SCI_TRACE_LOW:"PNULL != str_ptr, PNULL != str_info_ptr"
       SCI_TRACE_LOW("[MMIAPICOM_StrSmartCompare] PNULL != str_ptr_buf, PNULL != str_info_ptr_buf");   
        return TRUE;
    }

    str1.wstr_len = str_ptr->wstr_len;
    str1.wstr_ptr = str_ptr->wstr_ptr;
    str2.wstr_len = str_info_ptr->wstr_len;
    str2.wstr_ptr = str_info_ptr->wstr_ptr;

    // cut head blank
    while(str1.wstr_len && (*wBlank == *(str1.wstr_ptr)) )
    {
        str1.wstr_ptr++;
        str1.wstr_len--;
    }
    while(str2.wstr_len && (*wBlank == *(str2.wstr_ptr)) )
    {
        str2.wstr_ptr++;
        str2.wstr_len--;
    }

    // check real content
    if (ifLow)
    {  
        result  = MMIAPICOM_WstrnLowCmp( str1.wstr_ptr, str2.wstr_ptr, MIN(str1.wstr_len, str2.wstr_len) );
    }
    else
    {   
         result = MMIAPICOM_Wstrncmp( str1.wstr_ptr, str2.wstr_ptr, MIN(str1.wstr_len, str2.wstr_len) ); 
    }

    if(!ifNeedCheckTail) return (result == 0);
    
    // skip tail blank
    if((0 == result) &&  (str1.wstr_len != str2.wstr_len) )
    {
        if(str1.wstr_len > str2.wstr_len)
        {
            wTailPtr = str1.wstr_ptr;
            iTailPos = str2.wstr_len;
        }
        else
        {
            wTailPtr = str2.wstr_ptr;
            iTailPos = str1.wstr_len;;
        }
        while( (iTailPos < MAX(str1.wstr_len, str2.wstr_len)) )
        {
            if(*wBlank != wTailPtr[iTailPos++])
            {
                result++;
                break;
            }
        }
    }

    return (result == 0);
}

PUBLIC BOOLEAN MMIAPICON_ConvertStringtoNumber(uint8 *str_ptr, uint8 length, uint32 *num_ptr)
{
    uint8  i;
    uint32  numeric_value = 0;

    if((NULL == str_ptr) || (length < 1) || (length > 12))
    {
        return FALSE;
    }

    for(i = 0; i < length; ++i)
    {
        if(str_ptr[i] >= '0' && str_ptr[i] <= '9')
        {
            numeric_value = numeric_value * 10 + (int32)(str_ptr[i] - '0');
        }
    }

    SCI_TRACE_LOW("[szp] ConvertStringtoUint32,get the number: %d", numeric_value);
    
    *num_ptr = numeric_value;

    return TRUE;
}

