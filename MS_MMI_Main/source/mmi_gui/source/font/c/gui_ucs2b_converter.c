/*****************************************************************************
** File Name:      gui_ucs2b_converter.c                                       *
** Author:                                                                   *
** Date:           08/07/2006                                                *
** Copyright:      2006 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe conversion function  for ucs2  big endian        *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 08/07/2006    songbin.zeng           Create                                  *
******************************************************************************/

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_gui_trc.h"
#include "sci_types.h"
#include "os_api.h"
#include "guifont.h"
#include "gui_ucs2b_converter.h"
#include "caf.h"

#ifdef JUNIVERSALCHARDET_SUPPORT
#include "universalchardet_export.h"
#endif

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/*---------------------------------------------------------------------------*/
/*                          PUBLIC FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
#define SEMICOLON   ';'
#define BACKSLASH   '\\'

#define	GB_OFFSET_NUMBER	94		//the number of one GB zone.
#define	GB_MIN_ZONE			0xA0	//min value of the zone number of GB code
#define	GB_MAX_ZONE			0xF8	//max value of the zone number of GB code
#define	GB_MIN_OFFSET		0xA1	//min value of the offset number of GB code
#define	GB_MAX_OFFSET		0xFF	//max value of the offset number of GB code
#define	MAX_OFFSET_IN_TABLE	8272	//the max offset of the conversion table.

#define UCS_CHAR_UNDEF  0x3F

#if defined (IM_SIMP_CHINESE_SUPPORT) || defined (MMI_DISPLAY_SIMP_CHINESE_SUPPORT)
	extern const unsigned short sci_gb2312_to_ucs2_table[];
#endif
/*---------------------------------------------------------------------------*/
/*                          PUBLIC FUNCTION DEFINE                           */
/*---------------------------------------------------------------------------*/

#ifndef GBK_SUPPORT
/*****************************************************************************/
// 	Description : 计算一段ANSI编码中非ANSI的字符个数
//	Global resource dependence : 必须用在某一段已经确定为ANSI编码的数据中
//   Author: Ying.Xu
//	Note: 返回非ANSI字符的个数
/*****************************************************************************/
LOCAL int32 CaculateNotAnsiCodeCount(uint8 *data_ptr, int32 length);
#endif
/*****************************************************************************/
// 	Description : wstr to UTF8
//	Global resource dependence : 
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC uint32 GUI_WstrToUTF8(//utf8 len
                             uint8 *utf8_ptr,//out
                             uint32 utf8_buf_len,//in
                             const wchar *wstr_ptr,//in
                             uint32 wstr_len//in
                             )
{
	wchar	ucs2_char = 0;	//here, we only consider UCS2. UCS4 is not considered.
    uint32  i = 0;
	uint32	j = 0;

	if (PNULL == utf8_ptr || PNULL == wstr_ptr)
	{
        return j;
	}
	
	for ( ; i < wstr_len; i++ )
	{
		//get unicode character

        ucs2_char = wstr_ptr[i];

		if (ucs2_char <= 0x7F)
		{
			//0xxx xxxx 
			//0111 1111 == 0x7F
            if ( j >= utf8_buf_len )
            {
                break;
            }

			utf8_ptr[j++] = (uint8)(ucs2_char & 0x7F);
		}
		else if (ucs2_char <= 0x7FF)
		{
            if ( j + 1 >= utf8_buf_len )
            {
                break;
            }

			//110x xxxx 10xx xxxx
			//11位拆成5，6位

			//0000 0111 1100 0000 == 0x7C0	110 == 0x6	//取出高5位＋(110 << 5)
			utf8_ptr[j++] = (uint8)(((ucs2_char & 0x7C0) >> 6) | (0x6 << 5));
			
			//0000 0000 0011 1111 = 0x3F	10 == 0x2	//取出低6位＋(10 << 6)
			utf8_ptr[j++] = (uint8)((ucs2_char & 0x3F) | (0x2 << 6));
		}
		else
		{
            if ( j + 2 >= utf8_buf_len )
            {
                break;
            }

			//1110 xxxx 10xx xxxxxx 10xx xxxxxx
			//16位拆成4, 6, 6

			//1111 0000 0000 0000 == 0xF000		1110 == 0xE	//取出高4位 ＋ (1110 << 4)
			utf8_ptr[j++] = (uint8)(((ucs2_char & 0xF000) >> 12) | (0xE << 4));

			//0000 1111 1100 0000 == 0xFC0		10 = 0x2	//中间6位 ＋ (10 << 6)
			utf8_ptr[j++] = (uint8)(((ucs2_char & 0xFC0 ) >> 6) | (0x2 << 6));

			//0000 0000 0011 1111 == 0x3F		10 = 0x2	//最后6位 ＋ (10 << 6)
			utf8_ptr[j++] = (uint8)((ucs2_char & 0x3F) | (0x2 << 6));
		}
	}

	return j;
}

/*****************************************************************************/
// 	Description : wstr to UTF8 only for vcard
//	Global resource dependence : 
//  Author: 
//	Note: need change ";" to "\;"
/*****************************************************************************/
PUBLIC uint32 GUI_WstrToUTF8_vCard(//utf8 len
                             uint8 *utf8_ptr,//out
                             uint32 utf8_buf_len,//in
                             const wchar *wstr_ptr,//in
                             uint32 wstr_len//in
                             )
{
	wchar	ucs2_char = 0;	//here, we only consider UCS2. UCS4 is not considered.
    uint32  i = 0;
	uint32	j = 0;

	if (PNULL == utf8_ptr || PNULL == wstr_ptr)
	{
        return j;
	}
	
	for ( ; i < wstr_len; i++ )
	{
		//get unicode character

        ucs2_char = wstr_ptr[i];
        if(SEMICOLON == ucs2_char)
		{
			//change ";" -->"\;"
            utf8_ptr[j++] = BACKSLASH;
			utf8_ptr[j++] = SEMICOLON;
		}
		else if (ucs2_char <= 0x7F)
		{
			//0xxx xxxx 
			//0111 1111 == 0x7F
            if ( j >= utf8_buf_len )
            {
                break;
            }

			utf8_ptr[j++] = (uint8)(ucs2_char & 0x7F);
		}
		else if (ucs2_char <= 0x7FF)
		{
            if ( j + 1 >= utf8_buf_len )
            {
                break;
            }

			//110x xxxx 10xx xxxx
			//11位拆成5，6位

			//0000 0111 1100 0000 == 0x7C0	110 == 0x6	//取出高5位＋(110 << 5)
			utf8_ptr[j++] = (uint8)(((ucs2_char & 0x7C0) >> 6) | (0x6 << 5));
			
			//0000 0000 0011 1111 = 0x3F	10 == 0x2	//取出低6位＋(10 << 6)
			utf8_ptr[j++] = (uint8)((ucs2_char & 0x3F) | (0x2 << 6));
		}
		else
		{
            if ( j + 2 >= utf8_buf_len )
            {
                break;
            }

			//1110 xxxx 10xx xxxxxx 10xx xxxxxx
			//16位拆成4, 6, 6

			//1111 0000 0000 0000 == 0xF000		1110 == 0xE	//取出高4位 ＋ (1110 << 4)
			utf8_ptr[j++] = (uint8)(((ucs2_char & 0xF000) >> 12) | (0xE << 4));

			//0000 1111 1100 0000 == 0xFC0		10 = 0x2	//中间6位 ＋ (10 << 6)
			utf8_ptr[j++] = (uint8)(((ucs2_char & 0xFC0 ) >> 6) | (0x2 << 6));

			//0000 0000 0011 1111 == 0x3F		10 = 0x2	//最后6位 ＋ (10 << 6)
			utf8_ptr[j++] = (uint8)((ucs2_char & 0x3F) | (0x2 << 6));
		}
	}

	return j;
}
/*****************************************************************************/
// 	Description : UTF8 to wstr 
//	Global resource dependence : 
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC uint32 GUI_UTF8ToWstr(//ucs2b len
                             wchar *wstr_ptr,//out
                             uint32 wstr_len,//in
                             const uint8 *utf8_ptr,//in
                             uint32 utf8_len//in
                             )
{
    return GUI_UTF8ToWstrWithEndPos(wstr_ptr, wstr_len, utf8_ptr, utf8_len, PNULL);
}

/*****************************************************************************/
// 	Description : UTF8 to wstr, with returning the end position of src postion.
//	Global resource dependence : 
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC uint32 GUI_UTF8ToWstrWithEndPos(//ucs2b len
                             wchar *wstr_ptr,//out
                             uint32 wstr_len,//in
                             const uint8 *utf8_ptr,//in
                             uint32 utf8_len,//in
                             uint32 *src_end_pos
                             )
{
	uint32	i = 0;
	uint32	j = 0;
      uint32     k = 0;
	uint16	ucs2_char = 0;
	uint8	head_char = 0;
	
	if (PNULL == wstr_ptr || PNULL == utf8_ptr)
	{
        return j;
	}

	while(i < utf8_len && j < wstr_len )
	{
        k = i;
             
		head_char = utf8_ptr[i++];

		if (head_char <= 0x7F)
		{
			//0xxx xxxx
			//0111 1111 = 0X7F
			wstr_ptr[j ++] = head_char;
		}
		else if (head_char <= 0xDF)
		{
			if (i < utf8_len)
			{
				//110x xxxx 10xx xxxx
				//1101 1111	== 0xDF

				//0001 1111 == 0x1F		0011 1111 == 0x3F
				ucs2_char = (uint16)(((head_char & 0x1F) << 6) | ((utf8_ptr[i++] & 0x3F)));

				wstr_ptr[j++] = ucs2_char;
			}
			else
			{
				break;
			}						
		}
		else if (head_char <= 0xEF)
		{
			if (i + 1 < utf8_len)
			{
				//1110 xxxx 10xx xxxx 10xx xxxx
				//1110 1111 == 0xEF

				//0000 1111 == 0xF	0011 1111 == 0x3F
				ucs2_char = (uint16)(((head_char & 0xF) << 12) | ((utf8_ptr[i] & 0x3F) << 6) | (utf8_ptr[i + 1] & 0x3F));
				i += 2;
				wstr_ptr[j++] = ucs2_char;
			}
			else
			{
				break;
			}
			
		}
		else if(head_char <= 0xF7)
		{
            //1111 0xxx 10xx xxxx 10xx xxxx 10xx xxxx
			//1111 0111 == 0xF7

			//0000 0111 == 0x7	0011 1111 == 0x3F
            if (i + 1 < utf8_len)
            {
                ucs2_char = (uint16)(((head_char & 0x7) << 18) | ((utf8_ptr[i] & 0x3F) << 12) | ((utf8_ptr[i+1] & 0x3F) << 6) | (utf8_ptr[i + 2] & 0x3F));
			    i += 3;
			    wstr_ptr[j++] = ucs2_char;
            }
            else
            {
                break;
            }
		}
        else if(head_char <= 0xFB)
		{
            //1111 10xx 10xx xxxx 10xx xxxx 10xx xxxx 10xx xxxx
			//1111 1011 == 0xFB

			//0000 0011 == 0x3	0011 1111 == 0x3F
            if (i + 1 < utf8_len)
            {
                ucs2_char = (uint16)(((head_char & 0x3) << 24) | ((utf8_ptr[i] & 0x3F) << 18) | ((utf8_ptr[i+1] & 0x3F) << 12) 
                    | ((utf8_ptr[i+2] & 0x3F) << 6) | (utf8_ptr[i + 3] & 0x3F));
			    i += 4;

                wstr_ptr[j++] = ucs2_char;
            }
            else
            {
                break;
            }

		}
        else if(head_char <= 0xFD)
		{
            //1111 110x 10xx xxxx 10xx xxxx 10xx xxxx 10xx xxxx 10xx xxxx
			//1111 1101 == 0xFD

			//0000 0001 == 0x1	0011 1111 == 0x3F
            if (i + 1 < utf8_len)
            {
                ucs2_char = (uint16)(((head_char & 0x3) << 30) | ((utf8_ptr[i] & 0x3F) << 24) | ((utf8_ptr[i+1] & 0x3F) << 18) 
                    | ((utf8_ptr[i+2] & 0x3F) << 12) | ((utf8_ptr[i+3] & 0x3F) << 6) | (utf8_ptr[i + 4] & 0x3F));
			    i += 5;

                wstr_ptr[j++] = ucs2_char;
            }
            else
            {
                break;
            }
		}
        else
        {
            //SCI_TRACE_LOW:"GUI_UTF82UCS2B: i %d char %x"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUI_UCS2B_CONVERTER_338_112_2_18_3_16_57_143,(uint8*)"dd",i,head_char);
            wstr_ptr[j++] = UCS_CHAR_UNDEF;
            //break;
        }

        k = i;
	}

    if(src_end_pos != PNULL)
    {
        *src_end_pos = (uint32)(utf8_ptr + k);
    }
    return j;
}
/*****************************************************************************/
// 	Description : convert ucs big endian to ucs little endian
//	Global resource dependence : 
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC uint32 GUI_UCS2B2UCS2L(//ucs2l len
                              uint8 *ucs2l_ptr,//out
                              uint32 ucs2l_buf_len,//in
                              const uint8 *ucs2b_ptr,//in
                              uint32 ucs2b_len//in
                              )
{
    return GUI_UCS2B2UCS2LWithEndPos(ucs2l_ptr, ucs2l_buf_len, ucs2b_ptr, ucs2b_len, PNULL);
}

/*****************************************************************************/
// 	Description : convert ucs big endian to ucs little endian, with returning the end position of src postion. 
//	Global resource dependence : 
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC uint32 GUI_UCS2B2UCS2LWithEndPos(//ucs2l len
                              uint8 *ucs2l_ptr,//out
                              uint32 ucs2l_buf_len,//in
                              const uint8 *ucs2b_ptr,//in
                              uint32 ucs2b_len,//in
                              uint32 *src_end_pos
                              )
{
    uint32 ucs2l_len = 0;

    if (PNULL == ucs2l_ptr || PNULL == ucs2b_ptr || ucs2l_buf_len < ucs2b_len)
    {
        return ucs2l_len;
    }

    ucs2l_len = ucs2b_len / 2 * 2;

    while(ucs2b_len > 1)
    {
        *(ucs2l_ptr + 1) = *ucs2b_ptr++;
        *ucs2l_ptr = *ucs2b_ptr++;
        ucs2l_ptr += 2;
        ucs2b_len -= 2;
    }

    if(src_end_pos != PNULL)
    {
        *src_end_pos = (uint32)(ucs2b_ptr);
    }
    return ucs2l_len;
}

/*****************************************************************************/
// 	Description : convert ucs little endian to ucs big endian
//	Global resource dependence : 
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC uint32 GUI_UCS2L2UCS2B(//ucs2b len
                              uint8 *ucs2b_ptr,//out
                              uint32 ucs2b_buf_len,//in
                              const uint8 *ucs2l_ptr,//in
                              uint32 ucs2l_len//in
                              )
{
    return GUI_UCS2L2UCS2BWithEndPos(ucs2b_ptr, ucs2b_buf_len, ucs2l_ptr, ucs2l_len, PNULL);
}

/*****************************************************************************/
// 	Description : convert ucs little endian to ucs big endian, , with returning the end position of src postion. 
//	Global resource dependence : 
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC uint32 GUI_UCS2L2UCS2BWithEndPos(//ucs2b len
                              uint8 *ucs2b_ptr,//out
                              uint32 ucs2b_buf_len,//in
                              const uint8 *ucs2l_ptr,//in
                              uint32 ucs2l_len,//in
                              uint32 *src_end_pos
                              )
{
    uint32 ucs2b_len = 0;

    if (PNULL == ucs2l_ptr || PNULL == ucs2b_ptr || ucs2b_buf_len < ucs2l_len)
    {
        return ucs2b_len;
    }

    ucs2b_len = ucs2l_len / 2 * 2;

    while(ucs2l_len > 1)
    {
        *(ucs2b_ptr + 1)= *ucs2l_ptr++;
        *ucs2b_ptr = *ucs2l_ptr++;
        ucs2b_ptr += 2;
        ucs2l_len -= 2;
    }

    if(src_end_pos != PNULL)
    {
        *src_end_pos = (uint32)(ucs2l_ptr);
    }
    return ucs2b_len;
}

/*****************************************************************************/
// 	Description : wstr to other code
//	Global resource dependence : 
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC uint32 GUI_WstrToOtherCode(//othercode len
                                  GUI_CHAR_CODE_TYPE_E code_type,//in
                                  uint8 *code_ptr,//out
                                  uint32 code_buf_len,//in
                                  const wchar *wstr_ptr,//in
                                  uint32 wstr_len//in
                                  )
{
	uint32 len = 0;
	
	switch(code_type)
		{
		case GUI_CHAR_CODE_ANSI:
            len = GUI_WstrToGB( code_ptr, wstr_ptr, wstr_len );
			break;

        case GUI_CHAR_CODE_ANSI_BIG5:
#ifdef BIG5_SUPPORT
            len = GUI_WstrToBig5( code_ptr, wstr_ptr, wstr_len );
#endif
            break;

		case GUI_CHAR_CODE_UCS2_LE:
#if !defined(__BIG_ENDIAN) && !defined(__BigEndian)
            if ( code_buf_len < wstr_len * sizeof(wchar) )
            {
                break;
            }

            SCI_MEMCPY(
                code_ptr, 
                wstr_ptr, 
                wstr_len * sizeof(wchar)
                );

#else
            GUI_UCS2B2UCS2L(
                code_ptr, 
                code_buf_len, 
                (uint8*)wstr_ptr, 
                wstr_len * sizeof(wchar)
                );
#endif
            len = wstr_len * sizeof(wchar);
			break;
			
		case GUI_CHAR_CODE_UTF8:
            len = GUI_WstrToUTF8(
                code_ptr, 
                code_buf_len, 
                wstr_ptr, 
                wstr_len
                );
			break;
			
		case GUI_CHAR_CODE_UCS2_BE:
#if defined(__BIG_ENDIAN) || defined(__BigEndian)
            if ( code_buf_len < wstr_len * sizeof(wchar) )
            {
                break;
            }

            SCI_MEMCPY(
                code_ptr, 
                wstr_ptr, 
                wstr_len * sizeof(wchar)
                );
#else
            GUI_UCS2L2UCS2B(
                code_ptr, 
                code_buf_len, 
                (uint8*)wstr_ptr, 
                wstr_len * sizeof(wchar)
                );
#endif
            len = wstr_len * sizeof(wchar);
			break;
			
		default:
            //SCI_TRACE_LOW:"== GUI_WstrToOtherCode == code_type %d is error!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUI_UCS2B_CONVERTER_535_112_2_18_3_16_57_144,(uint8*)"d", code_type);
			break;
		}
	
	return len;
}

/*****************************************************************************/
// 	Description : other code to wstr
//	Global resource dependence : 
//  Author: 
//	Note:返回unicode字符个数
/*****************************************************************************/
PUBLIC uint32 GUI_OtherCodeToWstr(//ucs2b len
                                  wchar *wstr_ptr,//out
                                  uint32 wstr_len,//in
                                  GUI_CHAR_CODE_TYPE_E code_type,//in
                                  const uint8 *code_ptr,//in
                                  uint32 code_len//in
                                  )
{
    return GUI_OtherCodeToWstrWithEndPos(wstr_ptr, wstr_len, code_type, code_ptr, code_len, PNULL);
}

/*****************************************************************************/
// 	Description : other code to wstr, with returning the end position of src postion. 
//	Global resource dependence : 
//  Author: 
//	Note:返回unicode字符个数
/*****************************************************************************/
PUBLIC uint32 GUI_OtherCodeToWstrWithEndPos(//ucs2b len
											wchar *wstr_ptr,//out
											uint32 wstr_len,//in
											GUI_CHAR_CODE_TYPE_E code_type,//in
											const uint8 *code_ptr,//in
											uint32 code_len, //in
											uint32 *src_end_pos
											)
{
	uint32 len = 0;
	
	switch(code_type)
		{
#ifdef IM_RUSSIAN_SUPPORT
        case GUI_CHAR_CODE_WINDOW1251:
            len = GUI_CP1251ToWstrWithEndPos(wstr_ptr,
                                             code_ptr,
                                             code_len,
                                             src_end_pos);
            break;
        case GUI_CHAR_CODE_CP855:
            len = GUI_CP855ToWstrWithEndPos(wstr_ptr,
                                             code_ptr,
                                             code_len,
                                             src_end_pos);
            break;
        case GUI_CHAR_CODE_CP866:
            len = GUI_CP866ToWstrWithEndPos(wstr_ptr,
                                             code_ptr,
                                             code_len,
                                             src_end_pos);
            break;
        case GUI_CHAR_CODE_KOI8R:
            len = GUI_KOI8RToWstrWithEndPos(wstr_ptr,
                                             code_ptr,
                                             code_len,
                                             src_end_pos);
            break;
        case GUI_CHAR_CODE_MACCYRILLIC:
            len = GUI_MAC_CYRILLICToWstrWithEndPos(wstr_ptr,
                                             code_ptr,
                                             code_len,
                                             src_end_pos);
            break;
#endif
		case GUI_CHAR_CODE_ANSI:
            len = GUI_GBToWstrWithEndPos(
                wstr_ptr, 
                code_ptr, 
                code_len,
                src_end_pos
                );
			break;

        case GUI_CHAR_CODE_ANSI_BIG5:
#ifdef BIG5_SUPPORT            
            len = GUI_Big5ToWstrWithEndPos(
                wstr_ptr, 
                code_ptr, 
                code_len,
                src_end_pos
                );
#endif
            break;

		case GUI_CHAR_CODE_UCS2_LE:
#if !defined(__BIG_ENDIAN) && !defined(__BigEndian)
            if ( wstr_len * sizeof(wchar) < code_len )
            {
                break;
            }

            SCI_MEMCPY(
                wstr_ptr, 
                code_ptr, 
                code_len
                );
            if(src_end_pos != PNULL)
            {
                *src_end_pos = (uint32)(code_ptr + (code_len /sizeof(wchar)) * 2);
            }
#else
            GUI_UCS2L2UCS2BWithEndPos(
                (uint8*)wstr_ptr, 
                wstr_len * sizeof(wchar), 
                code_ptr, 
                code_len,
                src_end_pos
                );
#endif
            len = code_len /sizeof(wchar);
            break;
						
		case GUI_CHAR_CODE_UTF8:
            len = GUI_UTF8ToWstrWithEndPos(
                wstr_ptr, 
                wstr_len, 
                code_ptr, 
                code_len,
                src_end_pos
                );
			break;

		case GUI_CHAR_CODE_UCS2_BE:
#if defined(__BIG_ENDIAN) || defined(__BigEndian)
            if ( wstr_len * sizeof(wchar) < code_len )
            {
                break;
            }

            SCI_MEMCPY(
                wstr_ptr, 
                code_ptr, 
                code_len
                );
            if(src_end_pos != PNULL)
            {
                *src_end_pos = (uint32)(code_ptr + (code_len /sizeof(wchar)) * 2);            
            }
#else
            GUI_UCS2B2UCS2LWithEndPos(
                (uint8*)wstr_ptr, 
                wstr_len * sizeof(wchar), 
                code_ptr, 
                code_len,
                src_end_pos
                );
#endif
            len = code_len /sizeof(wchar);        
            break;
			
		default:
			//SCI_TRACE_LOW:"== GUI_OtherCodeToWstr == code_type %d is error!"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUI_UCS2B_CONVERTER_693_112_2_18_3_16_58_145,(uint8*)"d",code_type);
			break;
		}
	return len;
}

BOOLEAN is_utf8_special_byte(uint8 c)
{
    uint16 special_byte = 0X02; //binary 00000010    
    if (c >> 6 == special_byte) {
        return 1;
    } else {
        return 0;
    }    
}

BOOLEAN is_utf8_code(uint8 *str)
{
    uint16 one_byte = 0X00; //binary 00000000
    uint16 two_byte = 0X06; //binary 00000110
    uint16 three_byte = 0X0E; //binary 00001110  
    uint16 four_byte = 0X1E; //binary 00011110
    uint16 five_byte = 0X3E; //binary 00111110
    uint16 six_byte = 0X7E; //binary 01111110
    
    int utf8_yes = 0;
    int utf8_no = 0;
    int utf8_total = 0;
    
    uint8 k = 0;
    uint8 m = 0;
    uint8 n = 0;
    uint8 p = 0;
    uint8 q = 0;
    
    uint8 c = 0; 
    
    uint32 str_len = strlen((char *)str);
    uint32 i = 0;
    uint16 ret = 0;

    for (i=0; i<str_len;) {
        c = str[i];        
        if (c>>7 == one_byte) {
            i++;    
            continue;
        } else if (c>>5 == two_byte) {
            k = str[i+1];
            if ( is_utf8_special_byte(k) ) {
                utf8_yes++;            
                i += 2;
                continue;
            }
        } else if (c>>4 == three_byte) {
            m = str[i+1];
            n = str[i+2];
            if ( is_utf8_special_byte(m) 
                    && is_utf8_special_byte(n) ) {
                utf8_yes++;
                i += 3;
                continue;
            }
        } else if (c>>3 == four_byte) {
            k = str[i+1];
            m = str[i+2];
            n = str[i+3];
            if ( is_utf8_special_byte(k)
                    && is_utf8_special_byte(m)
                    && is_utf8_special_byte(n) ) {
                utf8_yes++;
                i += 4;
                continue;
            }
        } else if (c>>2 == five_byte) {
             k = str[i+1];
             m = str[i+2];
             n = str[i+3];
             p = str[i+4];

            if ( is_utf8_special_byte(k) 
                    && is_utf8_special_byte(m)
                    && is_utf8_special_byte(n)
                    && is_utf8_special_byte(p) ) {
                utf8_yes++;
                i += 5;
                continue;
            }
        } else if (c>>1 == six_byte) {
            k = str[i+1];
            m = str[i+2];
            n = str[i+3];
            p = str[i+4];
            q = str[i+5];
            if ( is_utf8_special_byte(k) 
                    && is_utf8_special_byte(m)
                    && is_utf8_special_byte(n)
                    && is_utf8_special_byte(p) 
                    && is_utf8_special_byte(q) ) {
                utf8_yes++;
                i += 6;
                continue;
            }
        }            

        utf8_no++;
        i++;                    
    }        

    utf8_total = utf8_yes + utf8_no;
    printf("%d %d\n", utf8_yes, utf8_no);
    if (0 == utf8_total)
    {
        return 0;
    } 
    else
    {
        ret = 100*utf8_yes/utf8_total;
    }
    if (ret > 90) 
    {
        return 1;    
    } 
    else 
    {
        return 0;
    }
}


/*****************************************************************************/
// 	Description : Is string big5 coding?
//	Global resource dependence : 
//   Author: Yintang.Ren
//	Note: 返回big5 编码字符的个数
/*****************************************************************************/
LOCAL BOOLEAN IsBig5Str(
                            uint8 *code_ptr,//in
                            uint32 code_len, //in
                            uint32 *big5_len //out
                            )

{
    #define IS_BIG5_FIRST_BYTE(code)          ((code) >= 0x81 && (code) <= 0xFE) 
    #define IS_BIG5_SECOND_BYTE(code)      (((code) >= 0x40 && (code) <= 0x7E) || ((code) >= 0xA1 && (code) <= 0xFE) )

    uint8 *p = code_ptr;
    uint32 i = 0;
    uint32 len = 0;

    if(PNULL == code_ptr || 0 == code_len)
    {
        *big5_len = 0;
        return FALSE;
    }

    if (is_utf8_code(code_ptr))
    {
        return FALSE;
    }
    
    while( i < code_len && 0 != *p)
    {
        if(*p < 0x80)
        {
            len++;
            p++;
            i++;
        }
        else if(IS_BIG5_FIRST_BYTE(*p))
        {
            p++;
            if(IS_BIG5_SECOND_BYTE(*p))
            {
                len++;
                p++;
                i += 2;
            }
            else
            {
                break;
            }
        }
        else
        {
            break;
        }
    }
    
    if(PNULL != big5_len)
    {
        *big5_len = len;
    }
    if(i >= code_len)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*****************************************************************************/
// 	Description : Is string GBK coding?
//	Global resource dependence : 
//   Author: Yintang.Ren
//	Note: 返回GBK 编码字符的个数
//            GBK First bytes: 0x81 - 0xFE        Second bytes: 0x40 - 0x7E, 0x80 - 0xFE
/*****************************************************************************/
LOCAL BOOLEAN IsGbkStr(
                            uint8 *code_ptr,//in
                            uint32 code_len, //in
                            uint32 *gbk_len //out
                            )

{
    #define IS_GBK_FIRST_BYTE(code)          ((code) >= 0x81 && (code) <= 0xFE) 
    #define IS_GBK_SECOND_BYTE(code)      (((code) >= 0x40 && (code) <= 0x7E) || ((code) >= 0x80 && (code) <= 0xFE) )

    uint8 *p = code_ptr;
    uint32 i = 0;
    uint32 len = 0;

    if(PNULL == code_ptr || 0 == code_len)
    {
        *gbk_len = 0;
        return FALSE;
    }

    if (is_utf8_code(code_ptr))
    {
        return FALSE;
    }
    
    while( i < code_len && 0 != *p)
    {
        if(*p < 0x80)
        {
            len++;
            p++;
            i++;
        }
        else if(IS_GBK_FIRST_BYTE(*p))
        {
            p++;
            if(IS_GBK_SECOND_BYTE(*p))
            {
                len++;
                p++;
                i += 2;
            }
            else
            {
                break;
            }
        }
        else
        {
            break;
        }
    }
    
    if(PNULL != gbk_len)
    {
        *gbk_len = len;
    }
    if(i >= code_len)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
/*****************************************************************************/
// 	Description : Is string big5 coding?
//	Global resource dependence : 
//   Author: Yintang.Ren
//	Note: 返回ANSI 编码字符的个数
/*****************************************************************************/
LOCAL BOOLEAN IsAnsiStr(
                            uint8 *code_ptr,//in
                            uint32 code_len, //in
                            uint32 *ansi_len //out
                            )

{
    #define IS_GB_FIRST_BYTE(code)          ((code) >= 0x81 && (code) <= 0xFE) 
    #define IS_GB_SECOND_BYTE(code)       ((code) >= 0xA1 && (code) <= 0xFE) 

    uint8 *p = code_ptr;
    uint32 i = 0;
    uint32 len = 0;

    if(PNULL == code_ptr || 0 == code_len)
    {
        *ansi_len = 0;
        return FALSE;
    }

    if (is_utf8_code(code_ptr))
    {
        return FALSE;
    }
    
    while( i < code_len && 0 != *p)
    {
        if(*p < 0x80)
        {
            len++;
            p++;
            i++;
        }
        else if(IS_GB_FIRST_BYTE(*p))
        {
            p++;
            if(IS_GB_SECOND_BYTE(*p))
            {
                len++;
                p++;
                i += 2;
            }
            else
            {
                break;
            }
        }
        else
        {
            break;
        }        
    }

    if(PNULL != ansi_len)
    {
        *ansi_len = len;
    }
    if(i >= code_len)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*****************************************************************************/
// 	Description : 寻找一段UTF8编码中第1个UTF8字符的起始偏移
//	Global resource dependence : 必须用在某一段已经确定为UTF8编码的数据中
//   Author: Ying.Xu
//	Note: 返回第1个完整的UTF8编码的起始偏移
/*****************************************************************************/
PUBLIC int32 GUI_FindFirstUTF8CodeStartPosition(
                                            uint8 *p_src,//in
                                            int32 src_length //in
                                            )                                            
{
    int32 i = 0;
    
    for(i = 0; i < src_length; i++)
    {
        if(p_src[i] < 0x80 || p_src[i] >=  0xc0)
        {
            break;
        }
    }

    return i;	
}      
#ifndef GBK_SUPPORT	
/*****************************************************************************/
// 	Description : 计算一段ANSI编码中非ANSI的字符个数
//	Global resource dependence : 必须用在某一段已经确定为ANSI编码的数据中
//   Author: Ying.Xu
//	Note: 返回非ANSI字符的个数
/*****************************************************************************/
LOCAL int32 CaculateNotAnsiCodeCount(uint8 *data_ptr, int32 length)
{
	uint16 count = 0;
	uint16 gb_code = 0;
	int32 len = 0;
		
	if ( PNULL == data_ptr || 0 == length )
	{
		return 0;
	}
	
	len = length;  
	while(len > 0)
	{
		len--;
	        
		gb_code = (uint16)(*data_ptr++);

		if (GUI_IsAsciiCode(gb_code))
		{
			//该字符为Ascii, 属于ANSI
		}
#if defined (IM_SIMP_CHINESE_SUPPORT) || defined (MMI_DISPLAY_SIMP_CHINESE_SUPPORT)
		else if ((gb_code >= GB_MIN_ZONE) && (gb_code < GB_MAX_ZONE))
		{
			uint16 offset = 0;

			/* if len is 0 in this time, it means it's not a full GB2312 code, 
				we should stop and ignore the last byte */
			if(len == 0)
			{
				break;
			}
			offset = (uint16)(*data_ptr++);
			len--;

			if ((offset >= GB_MIN_OFFSET) && (offset < GB_MAX_OFFSET))
			{
				uint16 ucs2_code = 0;

				offset = (uint16)((offset - GB_MIN_OFFSET) + (gb_code - GB_MIN_ZONE) * GB_OFFSET_NUMBER);

				ucs2_code = (uint16)((offset < MAX_OFFSET_IN_TABLE) ? sci_gb2312_to_ucs2_table[offset] : UCS_CHAR_UNDEF);//songbin,2006.7.25

				if ( 0 == ucs2_code)
				{
					count++;
				}
			}
			else
			{
				//GB2312不支持的显示中文全角空格
				count++;
			}
		}
#endif          
		else
		{
			//An invalidate code.
			count++;

			//出现GBK编码时计算字符串长度错误,start
			data_ptr++;
			len--;
		}
	}

	return count;
}
/*****************************************************************************/
// 	Description : 寻找一段ANSI编码中第1个ANSI字符的起始偏移
//	Global resource dependence : 必须用在某一段已经确定为ANSI编码的数据中
//   Author: Ying.Xu
//	Note: 返回第1个完整的ANSI编码的起始偏移
/*****************************************************************************/
PUBLIC int32 GUI_FindFirstANSICodeStartPosition(
												uint8 *p_src,//in
												int32 src_length //in
												)                                            
{
    	int32 i = 0;
	uint16 gb_code = 0;
	uint16 src_count = 0;
	uint16 with_offset_count = 0;
	uint8* data_str_ptr_bak1 = p_src;
	uint8* data_str_ptr_bak2 = p_src + 1;

	if ( PNULL == p_src || 0 == src_length )
	{
		return 0;
	}
	
	//caculate scr data count
	src_count = CaculateNotAnsiCodeCount(data_str_ptr_bak1, src_length);

	//caculate src data count after offset
	with_offset_count = CaculateNotAnsiCodeCount(data_str_ptr_bak2, src_length);
		
	if (src_count > with_offset_count)
	{
		i = 1;
	}
	
	return i;	
}    
#endif
/*****************************************************************************/
// 	Description : Is string big5 coding?
//	Global resource dependence : 
//   Author: Yintang.Ren
//	Note: 返回UTF8 编码字符的个数
//　　0000        0000-0000 007F          0xxxxxxx
//　　0000        0080-0000 07FF          110xxxxx 10xxxxxx
//　　0000        0800-0000 FFFF          1110xxxx 10xxxxxx 10xxxxxx
//　　0001        0000-001F FFFF          11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
//　　0020        0000-03FF FFFF          111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
//　　0400        0000-7FFF FFFF          1111110x 10xxxxxx ... 10xxxxxx

/*****************************************************************************/
LOCAL BOOLEAN IsUtf8Str(
                            uint8 *code_ptr,//in
                            uint32 code_len, //in
                            uint32 *utf8_len
                            )

{
    uint32 i = 0;
    uint16 bytes = 0;
    uint8 chr = 0;
    BOOLEAN is_all_ascii = TRUE;
    uint32 len = 0;

    if(PNULL == code_ptr || 0 == code_len)
    {
        *utf8_len = 0;
        return FALSE;
    }

    if (is_utf8_code(code_ptr))
    {
        return TRUE;
    }
    
    for(i = 0; i < code_len; i++)
    {
        chr = *(code_ptr + i);

         /*  判断是否ASCII编码,如果不是,说明有可能是UTF-8,ASCII用7位编码,但用一个字节存,最高位标记为0,o0xxxxxxx*/
        if( (chr & 0x80) != 0 )
        {
            is_all_ascii = FALSE;
        }
        else
        {
            len++;
            continue;
        }
        /* 如果不是ASCII码,应该是多字节符,计算字节数*/
        if(bytes == 0) 
        {
            if(chr >= 0x80)
            {
                if(chr >= 0xFC && chr <= 0xFD)
                {
                    bytes = 6;
                }
                else if(chr >= 0xF8)
                {
                    bytes = 5;
                }
                else if(chr >= 0xF0)
                {
                    bytes = 4;
                }
                else if(chr >= 0xE0)
                {
                    bytes = 3;
                }
                else if(chr >= 0xC0)
                {
                    bytes = 2;
                }
                else
                {
                    break;
                }
                
                bytes--;
            }
        }
        /* 多字节符的非首字节,应为 10xxxxxx */
        else 
        {
            if( (chr & 0xC0) != 0x80 )
            {
                break;
            }
            bytes--;
            if(0 == bytes)
            {
                len++;
            }
        }
    }

    *utf8_len = len;
    
    /* 违返规则*/
    if( bytes > 0 ) 
    {
        return FALSE;
    }
    /* 如果全部都是ASCII, 说明不是UTF-8 */
    if( is_all_ascii ) 
    {
        return FALSE;
    }

    if(i >= code_len)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*****************************************************************************/
// 	Description : GetRussianEncodingType
//	Global resource dependence : 
//  Author: Dafeng.Xu
//	Note: 
/*****************************************************************************/
#ifdef JUNIVERSALCHARDET_SUPPORT
LOCAL GUI_CHAR_CODE_TYPE_E GetRussianEncodingType(const uint8 *code_ptr, uint32 code_len)
{
    uint8 encodingName[CHARDET_MAX_ENCODING_NAME];
    CHAR_DET_ENCODING_RESULT result;

    result = CharDetEncoding(code_ptr, code_len, encodingName, sizeof(encodingName));
    if (CHAR_DET_RESULT_OK == result) {
        if (0 == strcmp(CHARDET_ENCODING_IBM855, (char*)encodingName)) {
            return GUI_CHAR_CODE_CP855;
        }
        else if (0 == strcmp(CHARDET_ENCODING_IBM866, (char*)encodingName)) {
            return GUI_CHAR_CODE_CP866;
        }
        else if (0 == strcmp(CHARDET_ENCODING_KOI8_R, (char*)encodingName)) {
            return GUI_CHAR_CODE_KOI8R;
        }
        else if (0 == strcmp(CHARDET_ENCODING_MACCYRILLIC, (char*)encodingName)) {
            return GUI_CHAR_CODE_MACCYRILLIC;
        }
        else if (0 == strcmp(CHARDET_ENCODING_WINDOWS_1251, (char*)encodingName)) {
            return GUI_CHAR_CODE_WINDOW1251;
        }
        else if (0 == strcmp(CHARDET_ENCODING_UTF_8, (char*)encodingName)) {
            return GUI_CHAR_CODE_UTF8;
        }
        else {
            return GUI_CHAR_CODE_TYPE_MIN;
        }
    }
    return GUI_CHAR_CODE_TYPE_MIN;
}
#endif
/*****************************************************************************/
// 	Description : Get  a string's coding type, Big5, GB2312, or UTF8
//	Global resource dependence : 
//  Author:  Yintang.Ren
//	Note: We support getting the coding type by TXT file format, that means if the code_ptr including 
//          TXT file's heading, we can return the coding type by the heading code. For example, Unicode
//          formating TXT file is begin with the code 0xFF,0xFE.
//          Big5: 
//              First bytes: 0x81 - 0xFE        Second bytes: 0x40 - 0x7E, 0xA1 - 0xFE
//          GB2312:
//              First bytes: 0x81 - 0xFE        Second bytes: 0xA1 - 0xFE 
//          GBK:
//              First bytes: 0x81 - 0xFE        Second bytes: 0x40 - 0x7E, 0x80 - 0xFE
//
//          Big5 and GB2312 have the same first byte range.
/*****************************************************************************/
PUBLIC GUI_CHAR_CODE_TYPE_E GUI_GetCodeType(
                                            uint8 *code_ptr,//in
                                            uint32 code_len //in
                                            )
{    
    #define IS_MAX(a,b,c,d) ((a) >= (b) && (a) >= (c) && (a) >= (d))    
    
    uint32 big5_len = 0;
#ifndef BIG5_SUPPORT    
    uint32 ansi_len = 0;
#endif
    uint32 utf8_len = 0;
#ifdef GBK_SUPPORT
    uint32 gbk_len = 0;
#endif
#ifdef JUNIVERSALCHARDET_SUPPORT
    GUI_CHAR_CODE_TYPE_E ret;
#endif
    if(PNULL == code_ptr || 0 == code_len)
    {
        return GUI_CHAR_CODE_TYPE_MIN;
    }
    /* If the first two bytes is 0xFF,0xFE, we return it as UCS2 */
    if(0xFF == *code_ptr && 0xFE == *(code_ptr + 1))
    {
        return GUI_CHAR_CODE_UCS2_LE; 
    }
    else if(0xFE == *code_ptr && 0xFF == *(code_ptr + 1))
    {
        return GUI_CHAR_CODE_UCS2_BE;
    }
    else if(0xEF == *code_ptr && 0xBB == *(code_ptr + 1) && 0xBF == *(code_ptr + 2))
    {
        return GUI_CHAR_CODE_UTF8;
    }

#ifdef GBK_SUPPORT//CR161453
    if(IsGbkStr(code_ptr, code_len, &gbk_len))
    {
        return GUI_CHAR_CODE_ANSI;
    }    
#endif

#ifdef BIG5_SUPPORT            
    if(IsBig5Str(code_ptr, code_len, &big5_len))
    {
        return GUI_CHAR_CODE_ANSI_BIG5;
    }
#else
	//CR161453
//     if(IsBig5Str(code_ptr, code_len, &big5_len))
//     {
//         return GUI_CHAR_CODE_ANSI_BIG5;
//     }
    if(IsAnsiStr(code_ptr, code_len, &ansi_len))
    {
        return GUI_CHAR_CODE_ANSI;
    }    
#endif

    if(IsUtf8Str(code_ptr, code_len, &utf8_len))
    {
        return GUI_CHAR_CODE_UTF8;
    }

#ifdef JUNIVERSALCHARDET_SUPPORT
    ret = GetRussianEncodingType(code_ptr, code_len);
    if (GUI_CHAR_CODE_TYPE_MIN != ret) {
        return ret;
    }
#endif

#ifdef BIG5_SUPPORT         
#ifdef GBK_SUPPORT
    if(IS_MAX(big5_len, utf8_len, gbk_len, 0))//lint !e685 !e568
    {
        return GUI_CHAR_CODE_ANSI_BIG5;
    }
    else if(IS_MAX(gbk_len, utf8_len, big5_len, 0))//lint !e685 !e568
    {
        return GUI_CHAR_CODE_ANSI;
    }
    else
    {
        return GUI_CHAR_CODE_UTF8;
    }
#else
    if(IS_MAX(big5_len, utf8_len, 0, 0))//lint !e685 !e568
    {
        return GUI_CHAR_CODE_ANSI_BIG5;
    }
    else
    {
        return GUI_CHAR_CODE_UTF8;
    }
#endif    
#else
#ifdef GBK_SUPPORT
    if(IS_MAX(ansi_len, utf8_len, big5_len, gbk_len))
    {
        return GUI_CHAR_CODE_ANSI;
    }
    else if(IS_MAX(utf8_len, ansi_len, big5_len, gbk_len))
    {
        return GUI_CHAR_CODE_UTF8;
    }
    else if(IS_MAX(big5_len, utf8_len, ansi_len, gbk_len))
    {
        return GUI_CHAR_CODE_ANSI_BIG5;
    }
    else
    {
        return GUI_CHAR_CODE_ANSI;
    }
#else
    if(IS_MAX(ansi_len, utf8_len, big5_len, 0))//lint !e685 !e568
    {
        return GUI_CHAR_CODE_ANSI;
    }
    else if(IS_MAX(utf8_len, ansi_len, big5_len, 0))//lint !e685 !e568
    {
        return GUI_CHAR_CODE_UTF8;
    }
    else
    {
        return GUI_CHAR_CODE_ANSI_BIG5;
    }
#endif    
#endif
}


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif

