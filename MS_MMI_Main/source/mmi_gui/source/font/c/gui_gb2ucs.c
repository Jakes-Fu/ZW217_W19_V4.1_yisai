/*****************************************************************************
** File Name:      gui_gb2ucs.c                                              *
** Author:                                                                   *
** Date:           06/22/2004                                                *
** Copyright:      2004 Spreadtrum, Incorporated. All Rights Reserved.         *
** Description:    This file is used to describe gb2312 to ucs2 convertion   *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 06/22/2004     Jim.Zhang           Create                                 *
******************************************************************************/

#ifndef _GUI_GB2UCS_C_
#define _GUI_GB2UCS_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "os_api.h"
#include "gui_unicode.h"
#include "caf.h"
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

#define	GB_OFFSET_NUMBER	94		//the number of one GB zone.
#define	GB_MIN_ZONE			0xA0	//min value of the zone number of GB code
#define	GB_MAX_ZONE			0xF8	//max value of the zone number of GB code
#define	GB_MIN_OFFSET		0xA1	//min value of the offset number of GB code
#define	GB_MAX_OFFSET		0xFF	//max value of the offset number of GB code
#define	MAX_OFFSET_IN_TABLE	8272	//the max offset of the conversion table.
#if 0
#define UCS_CHAR_UNDEF      0x3000  //中文全角空格
#else
#define UCS_CHAR_UNDEF      0xFF1F  //中文全角问号
#endif

#if defined (IM_SIMP_CHINESE_SUPPORT) || defined (MMI_DISPLAY_SIMP_CHINESE_SUPPORT)
extern const unsigned short sci_gb2312_to_ucs2_table[];
#endif

#ifdef GBK_SUPPORT
extern uint32 GUI_GBKToWstrWithEndPos(//return the number of UCS2 code that have been converted.
									  wchar *wstr_ptr, 	      //destination: string pointer to UCS2 string.
									  const uint8 *gbk_ptr,   //source: string pointer to GBK string
									  uint32 str_len,		  //the length to convert from GBK to UCS2.
									  uint32 *src_end_pos     //the end pos of src string
									  );
#endif

/*****************************************************************************/
// 	Description : gb to wstr, with returning the end of src string.
//	Global resource dependence : none
//  Author: Yintang.ren
//	Note: when this function change, must check GUI_GBToWstrEx
/*****************************************************************************/
PUBLIC uint32 GUI_GBToWstrWithEndPos(					//return the number of UCS2 code that have been converted.
                           wchar *wstr_ptr, 	//destination: string pointer to UCS2 string.
                           const uint8 *gb_ptr, //source: string pointer to GB string
                           uint32 str_len,			//the length to convert from GB to UCS2.
                           uint32 *src_end_pos      //the end pos of src string
                           );

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : gb to wstr
//	Global resource dependence : none
//  Author: Jim.zhang
//	Note: when this function change, must check GUI_GBToWstrEx
/*****************************************************************************/
PUBLIC uint32 GUI_GBToWstr(					//return the number of UCS2 code that have been converted.
                           wchar *wstr_ptr, 	//destination: string pointer to UCS2 string.
                           const uint8 *gb_ptr, //source: string pointer to GB string
                           uint32 str_len		//the length to convert from GB to UCS2.
                           )
{
    return GUI_GBToWstrWithEndPos(wstr_ptr, gb_ptr, str_len, PNULL);
} 

/*****************************************************************************/
// 	Description : gb to wstr, with returning the end of src string.
//	Global resource dependence : none
//  Author: Yintang.ren
//	Note: when this function change, must check GUI_GBToWstrEx
/*****************************************************************************/
PUBLIC uint32 GUI_GBToWstrWithEndPos(					  //return the number of UCS2 code that have been converted.
									 wchar *wstr_ptr, 	  //destination: string pointer to UCS2 string.
									 const uint8 *gb_ptr, //source: string pointer to GB string
									 uint32 str_len,	  //the length to convert from GB to UCS2.
									 uint32 *src_end_pos  //the end pos of src string
									 )
{
#ifdef GBK_SUPPORT
    return GUI_GBKToWstrWithEndPos( wstr_ptr, gb_ptr, str_len, src_end_pos);
#else
    int32 len = 0;
    uint32 count = 0;
    uint16 gb_code = 0;

    if ( PNULL == gb_ptr || PNULL == wstr_ptr || 0 == str_len )
    {
        if(src_end_pos != PNULL)
        {
            *src_end_pos = 0;
        }
        return 0;
    }

    len = str_len;
    while(len > 0)
    {
        len--;
        count += 1;
        gb_code = (uint16)(*gb_ptr++);

        if (GUI_IsAsciiCode(gb_code))
        {
            *wstr_ptr++ = gb_code;
        }
#if defined (IM_SIMP_CHINESE_SUPPORT) || defined (MMI_DISPLAY_SIMP_CHINESE_SUPPORT)//Bug 1592283
        else if ((gb_code >= GB_MIN_ZONE) && (gb_code < GB_MAX_ZONE))
        {
            uint16 offset = 0;

            /* if len is 0 in this time, it means it's not a full GB2312 code, 
                we should stop and ignore the last byte */
            if(len == 0)
            {
                count--;
                gb_ptr--;
                break;
            }
            offset = (uint16)(*gb_ptr++);
            len--;

            if ((offset >= GB_MIN_OFFSET) && (offset < GB_MAX_OFFSET))
            {
                uint16 ucs2_code = 0;

                offset = (uint16)((offset - GB_MIN_OFFSET) + (gb_code - GB_MIN_ZONE) * GB_OFFSET_NUMBER);

                ucs2_code = (uint16)((offset < MAX_OFFSET_IN_TABLE) ? sci_gb2312_to_ucs2_table[offset] : UCS_CHAR_UNDEF);//songbin,2006.7.25

                if ( 0 == ucs2_code )
                {
                    ucs2_code = UCS_CHAR_UNDEF;
                }
                
                *wstr_ptr++ = ucs2_code;
            }
            else
            {
                //GB2312不支持的显示中文全角空格
                *wstr_ptr++ = UCS_CHAR_UNDEF;
            }
        }
#endif          
        else
        {
            //An invalidate code.
            *wstr_ptr++ = UCS_CHAR_UNDEF;

            //出现GBK编码时计算字符串长度错误,start
            gb_ptr++;
            len--;
        }
    }

    if(src_end_pos != PNULL)
    {
        *src_end_pos = (uint32)(gb_ptr);
    }
  
    return count;
#endif
}

/******************************************************************************************
// DESCRIPTION:
//       GB2312 string to Unicode string, only for browser.
//       in gb2312 web page, "a&#160;c" will be convert to
//       GB2312 string "a\x00\xA0c", but "a\x00\xA0c" convert to
//       Unicode will be {0x0061, 0x0000, 0x00a0, 0x0063} by call GUI_GBToWstr()
//       the string was ended by Unicode char 0x0000. so must extend  GUI_GBToWstr()
//       for WAP browser to avoid this 0x0000 char
// AUTHOR:
//   jiqun.li
//
// PARAMETERS:
//   wstr_ptr:    [OUT] destnation unicode string
//   gb2312_ptr:  [IN] source GB2312 string
//   len:         [IN] number os uint8 bytes in gb2312_ptr
//
// RETURN:
//   the number of UCS2 code that have been converted.
//
// NOTE:
******************************************************************************************/
PUBLIC uint16 GUI_GBToWstrEx(
                             wchar *wstr_ptr, 
                             const uint8 *gb_ptr, 
                             uint16 str_len
                             )
{
    uint16 count = 0;

    uint16 gb_code = 0;
    int32 len = str_len;
    
    if( PNULL == wstr_ptr || PNULL == gb_ptr )
    {
        return 0;
    }
    
    while( len > 0)
    {
        len--;
        count++;
        gb_code = (uint16)(*gb_ptr++);

        if(0 == gb_code)
        {
            *wstr_ptr++ = (uint16)(*gb_ptr++);
            len--;
            continue;
        }

        if (GUI_IsAsciiCode(gb_code))
        {
            *wstr_ptr++ = gb_code;
        }
#ifdef IM_SIMP_CHINESE_SUPPORT        
        else
        if ((gb_code >= GB_MIN_ZONE) && (gb_code < GB_MAX_ZONE))
        {
            uint16 offset = (uint16)(*gb_ptr++);
            len--;
            
            if ((offset >= GB_MIN_OFFSET) && (offset < GB_MAX_OFFSET))
            {
                uint16 ucs2_code = 0;
                
                offset = (uint16)((offset - GB_MIN_OFFSET) + (gb_code - GB_MIN_ZONE) * GB_OFFSET_NUMBER);
                
                ucs2_code = (uint16)((offset < MAX_OFFSET_IN_TABLE) ? sci_gb2312_to_ucs2_table[offset] : UCS_CHAR_UNDEF);//songbin,2006.7.25
                
                if ( 0 == ucs2_code )
                {
                    ucs2_code = UCS_CHAR_UNDEF;
                }

                *wstr_ptr++ = ucs2_code;
            }
            else
            {
                //GB2312不支持的显示中文全角空格
                *wstr_ptr++ = UCS_CHAR_UNDEF;
            }
        }
#endif         
        else
        {
            //An invalidate code.
            *wstr_ptr++ = UCS_CHAR_UNDEF;
            
            //出现GBK编码时计算字符串长度错误,start
            gb_ptr++;
            len--;
        }
    }
   
    return count;
} 
/*****************************************************************************/
// 	Description : convert an GB2312 string to UCS2 string.
//	Global resource dependence : none
//  Author: Jim.zhang
//	Note: 
/*****************************************************************************/
PUBLIC uint16 GUI_GB2UCS(					//return the number of UCS2 code that have been converted.
                         uint16 *ucs2_ptr, 	//destination: string pointer to UCS2 string.
                         const uint8 *gb_ptr,//source: string pointer to GB string
                         uint16 len			//the length to convert from GB to UCS2.
                         )
{
    return GUI_GBToWstr( ucs2_ptr, gb_ptr, len );
} 

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif


#endif	//_GUI_GB2UCS_C_
