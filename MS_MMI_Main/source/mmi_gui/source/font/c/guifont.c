/*****************************************************************************
** File Name:      guifont.c                                                 *
** Author:                                                                   *
** Date:           21/05/2004                                                *
** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file provide functions about font liabrary and       *
**                  display string                                           *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 21/05/2004       Jim.Zhang           Create                               *
******************************************************************************/

#ifndef _GUI_FONT_C_
#define _GUI_FONT_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_gui_trc.h"
#include "sci_types.h"
#include "guilcd.h"
#include "guifont.h"
#include "guicommon.h"
#include "mmi_theme.h"
#include "mmi_textfun.h"
#include "mmi_resource_def.h"
#include "guiblock.h"
#include "gui_unicode.h"

#include "spml_api.h"

/**---------------------------------------------------------------------------*
 **                         Functions                                         *
 **---------------------------------------------------------------------------*/

/*******************************************************************************************/
// 	Description : 获取文字宽度
//	Global resource dependence :
//  Author: James.Zhang
//	Note: 
/*********************************************************************************************/
uint8 GUI_GetFontWidth(//
                        GUI_FONT_T	font_type,      //字库类型
                        uint16 unicode
                        )
{
    //尽量少使用该函数, 多国语言下计算会错误
    return GUI_GetStringWidthEx( font_type, &unicode, 1, 0 );
}


/*******************************************************************************************/
// 	Description : 获取光标对齐方式
//	Global resource dependence :
//  Author: James.Zhang
//	Note: 光标只支持水平方向的对齐
/*********************************************************************************************/
LOCAL SPML_TEXT_FORMAT_T ConvertCursorAlign(
                                            GUI_ALIGN_E       align
                                            )
{
    SPML_TEXT_FORMAT_T format = 0;

    switch(align)
    {
    case ALIGN_LEFT:                                    // Align in left
        format |= TEXT_FORMAT_LEFT;
        break;
    case ALIGN_RIGHT:                                   // Align in right
        format |= TEXT_FORMAT_RIGHT;
        break;
    case ALIGN_HMIDDLE:                                 // Align in mediacy
        format |= TEXT_FORMAT_CENTER;
        break;
    default:
        break;
    }

    return format;
}

#ifndef SCRIPTEASY_SUPPORT
/*******************************************************************************************/
// 	Description : 获得光标的x坐标
//	Global resource dependence :
//  Author: James.Zhang
//	Note: 
/*********************************************************************************************/
PUBLIC int16 GUI_GetCusorCoordinate(//绝对坐标
                                    GUI_ALIGN_E       align,
                                    const GUI_RECT_T* rect_ptr,
                                    int8              dir,
                                    const wchar*      str_ptr,
                                    uint16            str_len,
                                    uint16            cursor_pos,
                                    GUI_FONT_T	      font_type,
                                    uint16            char_space
                                    )
{
    int16 x = 0;
    SPML_TEXT_PARAM_T param = {0};

	if (PNULL == rect_ptr)
	{
        //SCI_TRACE_LOW:"== GUI_GetCusorCoordinate == param rect_ptr is PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIFONT_96_112_2_18_3_17_1_147,(uint8*)"");
        return x;
	}

    if (PNULL == str_ptr || 0 == str_len )
    {
        if (ALIGN_HMIDDLE == align)
        {
            return (rect_ptr->left + rect_ptr->right) / 2;
        }
        else if (MMI_DIRECTION_RIGHT == MMITHEME_GetDisDirection()
                    || ALIGN_RIGHT == align
                    || ALIGN_RVMIDDLE == align)
        {
            return (rect_ptr->right);
        }
        else
        {
            return (rect_ptr->left);
        }
    }

    param.format |= ConvertCursorAlign( align );
    param.display_rect = param.clip_rect = *rect_ptr;
    param.glyph_space = char_space;
    param.dir  = dir;
    param.size = GUI_ConvertGUIFontToSize( font_type );

    if ( TEXT_DIR_RTL == param.dir )
    {
        param.format |= TEXT_FORMAT_RIGHT;
    }

    x = (int16)SPMLAPI_GetCursorCoordinate( &param, str_ptr, str_len, cursor_pos );

    return x;
}

/*******************************************************************************************/
// 	Description : 获得光标的x坐标并微调光标位置
//	Global resource dependence :
//  Author: James.Zhang
//	Note: 
/*********************************************************************************************/
PUBLIC int16 GUI_GetCursorInfo(
                               GUI_ALIGN_E       align,
                               const GUI_RECT_T	*rect_ptr,
                               int8				 dir,
                               const wchar		*str_ptr,
                               uint16            str_len,
                               BOOLEAN           is_left,
                               GUI_FONT_T	     font_type,
                               uint16            char_space,
                               int32*            cursor_ptr
                               )
{
    int16 x = 0;
    SPML_TEXT_PARAM_T param = {0};
    
	if (PNULL == rect_ptr)
	{
        //SCI_TRACE_LOW:"== GUI_GetCursorInfo == param rect_ptr is PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIFONT_149_112_2_18_3_17_1_148,(uint8*)"");
        return x;
	}

    if (PNULL == str_ptr || 0 == str_len )
    {
        return (rect_ptr->left);
    }
    
    param.format |= ConvertCursorAlign( align );
    param.display_rect = param.clip_rect = *rect_ptr;
    param.glyph_space = char_space;
    param.dir  = dir;
    param.size = GUI_ConvertGUIFontToSize( font_type );
    
    if ( TEXT_DIR_RTL == param.dir )
    {
        param.format |= TEXT_FORMAT_RIGHT;
    }
    
     x = (int16)SPMLAPI_GetCursorInfo( &param, str_ptr, str_len, is_left, cursor_ptr );

    return x;
}

/*******************************************************************************************/
// 	Description : 获得光标索引
//	Global resource dependence :
//  Author: James.Zhang
//	Note: 
/*********************************************************************************************/
PUBLIC uint16 GUI_GetCursorPos(//相对位置
                               GUI_ALIGN_E      align,
							   const GUI_RECT_T	*rect_ptr,      //in:
							   int8				dir,            //in:
							   const wchar		*str_ptr,       //in:
							   uint16           str_len,        //in:
							   uint16           cursor_pos,	    //in:相对位置
							   int16            cursor_x,	    //in:绝对坐标
							   GUI_FONT_T	    font_type,      //in:
							   uint16           char_space,     //in:
                               BOOLEAN          *is_find_ptr    //in/out:may PNULL
							   )
{
    int32               pos = 0;
    SPML_TEXT_PARAM_T   param = {0};

    if ( PNULL == rect_ptr || PNULL == str_ptr || 0 == str_len )
    {
        if (PNULL != is_find_ptr)
        {
            *is_find_ptr = FALSE;
        }

        return 0;
    }

    param.format |= ConvertCursorAlign( align );
    param.display_rect = param.clip_rect = *rect_ptr;
    param.glyph_space = char_space;
    param.dir  = dir;
    param.size = GUI_ConvertGUIFontToSize( font_type );

    if ( TEXT_DIR_RTL == param.dir )
    {
        param.format |= TEXT_FORMAT_RIGHT;
    }

    pos = SPMLAPI_GetCursorPos( &param, str_ptr, str_len, cursor_pos, cursor_x,is_find_ptr);

    if ( pos >= 0 )
    {
        cursor_pos =(uint16)pos;
    }

    return cursor_pos;
}



/*==============================================================================
Description: 获得光标索引
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note: 增强型，同时获得光标与字符的相对位置关系
==============================================================================*/
PUBLIC BOOLEAN GUI_GetCursorPosEx(
                                  GUI_ALIGN_E      align,
                                  const GUI_RECT_T *rect_ptr,
                                  int8         dir,
                                  const wchar  *str_ptr,
                                  uint16       str_len,
                                  int16        cursor_x,
                                  GUI_FONT_T   font_type,
                                  uint16       char_space,
                                  int32        *nearest_chr_idx_ptr,
                                  int32        *under_chr_idx_ptr,
                                  int32        *under_len_ptr
                                  )
{
    BOOLEAN is_find = FALSE;
    SPML_TEXT_PARAM_T param = {0};

    if (PNULL == nearest_chr_idx_ptr
        || PNULL == under_chr_idx_ptr
        || PNULL == under_len_ptr)
    {
        return FALSE;
    }

    if (PNULL == rect_ptr || PNULL == str_ptr || 0 == str_len)
    {
        *nearest_chr_idx_ptr = -1;
        return is_find;
    }

    param.format |= ConvertCursorAlign(align);
    param.display_rect = param.clip_rect = *rect_ptr;
    param.glyph_space = char_space;
    param.dir  = dir;
    param.size = GUI_ConvertGUIFontToSize(font_type);

    if ( TEXT_DIR_RTL == param.dir )
    {
        param.format |= TEXT_FORMAT_RIGHT;
    }

    is_find = SPMLAPI_GetCursorPosEx(&param,
                                     str_ptr,
                                     str_len,
                                     cursor_x,
                                     nearest_chr_idx_ptr,
                                     under_chr_idx_ptr,
                                     under_len_ptr);

    return is_find;
}

#endif /* SCRIPTEASY_SUPPORT */

//增加缓冲区进行优化
#define FONT_CACHE_MASK 0xFF
#define FONT_CACHE_SIZE (FONT_CACHE_MASK + 1)

typedef struct
{
    uint16 ch;
    uint8  ch_width;
    uint8  font_size;
} GUI_FONT_CACHE_T;

LOCAL GUI_FONT_CACHE_T s_han_font_cache = {0};
LOCAL GUI_FONT_CACHE_T s_other_font_cache[FONT_CACHE_SIZE] = {0};


#ifndef SCRIPTEASY_SUPPORT
/*******************************************************************************************/
// 	Description : 获得字符串的宽度
//	Global resource dependence :
//  Author: James.Zhang
//	Note: 字形间距可理解为之前的字间距, 但不允许用户自己计算, 必须通过该函数
/*********************************************************************************************/
PUBLIC uint16 GUI_GetStringWidthEx(
                                   GUI_FONT_T	font_type,     
                                   const wchar* str_ptr,
                                   uint16 str_len,
                                   uint16 char_space
                                   )
{
    SPML_TEXT_METRICS_T metrics = {0};
    SPML_TEXT_PARAM_T param = {0};
    
    if ( str_ptr == PNULL || str_len == 0 ) 
    {
        return 0;
    }
    
    param.glyph_space = char_space;
    param.dir = TEXT_DIR_AUTO;
    param.size = GUI_ConvertGUIFontToSize( font_type );
    
    if ( 1 == str_len )
    {
        wchar ch = str_ptr[0];
        GUI_FONT_CACHE_T* cache_ptr = PNULL;
        
        //汉字宽高都一样
        if (  (ch >= 0x2e80 && ch <= 0x9fff)
            || (ch >= 0xf900 && ch <= 0xfaff)
            || (ch >= 0xff00 && ch <= 0xffef))
        {
            ch = UNICODE_HANZI;
            
            cache_ptr = &s_han_font_cache;
        }
        else
        {
            cache_ptr = &s_other_font_cache[ch & FONT_CACHE_MASK];
        }
        
        if ( cache_ptr->ch == ch
            && cache_ptr->font_size == param.size
            && 0 != ch )
        {
            return cache_ptr->ch_width;
        }
        else
        {
            SPMLAPI_GetTextMetrics( &param, &ch, str_len, &metrics );
            
            cache_ptr->ch        = ch;
            cache_ptr->font_size = param.size;
            cache_ptr->ch_width  = metrics.width;
            
            return metrics.width;
        }
    }
    else
    {
        SPMLAPI_GetTextMetrics( &param, str_ptr, str_len, &metrics );
        
        return metrics.width;
    }
}
#endif /* SCRIPTEASY_SUPPORT */

/*******************************************************************************************/
// 	Description : 获得字符串的宽度
//	Global resource dependence :
//  Author: yongwei.he
//	Note:   
/*********************************************************************************************/
PUBLIC uint16 GUI_GetStringWidth(
                                 GUI_FONT_T	font_type,     
                                 const wchar* str_ptr,
                                 uint16 str_len
                                 )

{
#ifdef SCRIPTEASY_SUPPORT
    return GUI_GetStringWidthEx( font_type, str_ptr, str_len, 0 );
#else	
    //存在连字和叠加文字时, 必须精确计算, 否则可以逐字进行估算
#ifdef COMPLEX_TEXT_SCRIPT
    return GUI_GetStringWidthEx( font_type, str_ptr, str_len, 0 );
#else

    uint16  w_char = 0;
    uint16  str_width = 0;
    uint32  i = 0;
    uint8   char_width = 0;
    
    if((PNULL == str_ptr) || (0 == str_len) || (font_type >= GUI_MAX_FONT_NUM))
    {
        return 0;
    }

    for( ; i < str_len; i++ )
    {
        w_char = str_ptr[i];

        //james, \0返回
        if( 0 == w_char )
        {
            break;
        }
        
        char_width = GUI_GetFontWidth( font_type, w_char );
        
        str_width += char_width;
    }
    
    return str_width;
#endif
#endif /* SCRIPTEASY_SUPPORT */
}

/*****************************************************************************/
//  Description : get font height by font type
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint16 GUIFONT_GetHeight(
                                GUI_FONT_T  font_type
                                )
{
    return GUI_ConvertGUIFontToSize( font_type );
}

/*******************************************************************************************/
// 	Description : 获得字体的高度
//	Global resource dependence :
//  Author: panda.luo
//	Note:   2007-09-27 replace second param is_ucs2 to unicode by yongwei.he
/*********************************************************************************************/
uint8 GUI_GetFontHeight(                        //返回指定字体的高度
                        GUI_FONT_T	font_type,   //字库类型
                        uint16 unicode
                        )
{
    //尽量少使用该函数, 多国语言下计算会错误
    return GUI_GetStringHeight( font_type, &unicode, 1 );
}

/*******************************************************************************************/
// 	Description : 获得字符串的高度
//	Global resource dependence :
//  Author: yongwei.he
//	Note:   字符串的高度为所有字符的最大高度
/*********************************************************************************************/
#ifndef SCRIPTEASY_SUPPORT
PUBLIC uint16 GUI_GetStringHeight(
                                  GUI_FONT_T   font_type,     
                                  const wchar* str_ptr,
                                  uint16       str_len
                                  )
{
//    SPML_TEXT_METRICS_T metrics = {0};
    SPML_TEXT_PARAM_T param = {0};
    
    if ( str_ptr == 0 || str_len == 0 ) 
    {
        return 0;
    }

    param.dir = TEXT_DIR_AUTO;
    param.size = GUI_ConvertGUIFontToSize( font_type );

    return param.size;
    
    //优化多语显示, 删除精确计算, 可以通过调整字体库让多语字体稍小一点
    //存在连字和叠加文字时, 必须精确计算, 否则可以逐字进行估算
// #ifdef COMPLEX_TEXT_SCRIPT
// 
//     SPMLAPI_GetTextMetrics( &param, str_ptr, str_len, &metrics);
//     
//     return metrics.height;
//     
// #else
// 
//     return param.size;
// 
// #endif
}
#endif /* #ifndef SCRIPTEASY_SUPPORT */

/*******************************************************************************************/
// 	Description : 获得字符串的高度扩展
//	Global resource dependence :
//  Author: zhewei.tang
//	Note:   字符串的高度为所有字符的最大高度
/*********************************************************************************************/
PUBLIC uint16 GUI_GetStringHeightEx(
                                  GUI_FONT_T   font_type,     
                                  const wchar* str_ptr,
                                  uint16       str_len
                                  )
{
    SPML_TEXT_METRICS_T metrics = {0};
    SPML_TEXT_PARAM_T param = {0};
    
    if ( str_ptr == PNULL || str_len == 0 ) 
    {
        return 0;
    }
    
    param.dir = TEXT_DIR_AUTO;
    param.size = GUI_ConvertGUIFontToSize( font_type );    

    SPMLAPI_GetTextMetrics( &param, str_ptr, str_len, &metrics );
    
    return metrics.height;

}

/*******************************************************************************************/
// 	Description : whether is the new line char
//	Global resource dependence :
//  Author: Great.Tian
//	Note:
/*********************************************************************************************/
PUBLIC BOOLEAN GUI_IsNewlineChar(
                                 uint16 char_code //[IN] the char code, 用uint16可以同时支持ASCII和UINCODE
                                 )
{
    // SCI_TRACE_LOW("guifont.c: enter function GUI_IsNewlineChar()");

    if (CR_CHAR == char_code || LF_CHAR == char_code)
    {
        return (TRUE);
    }
    else
    {
        return (FALSE);
    }
}

/*******************************************************************************************/
// 	Description :
//	Global resource dependence :
//  Author: 
//	Note:
/*********************************************************************************************/
uint16 GUI_ConvertGUIFontToSize( GUI_FONT_T font )
{    
    uint16 font_size = GUI_MAX_FONT_NUM - 1;
    
    if ( font < font_size )
    {
        font_size = font;
    }

    return font_size;
}

/*******************************************************************************************/
// 	Description :
//	Global resource dependence :
//  Author: 
//	Note:
/*********************************************************************************************/
void GUI_ReplaceChar(
                     MMI_STRING_T *src_str_ptr, 
                     uint16 src_char,
                     uint16 des_char
                     )
{    
    uint16 i = 0;
    wchar  *temp_str = PNULL;
    uint16 sum_len = 0;

    if (PNULL != src_str_ptr)
    {
        sum_len = src_str_ptr->wstr_len;
        temp_str = src_str_ptr->wstr_ptr;
        
        for ( i = 0; i < sum_len; i++ )
        {
            if ( src_char == *temp_str)
            {
                *temp_str = des_char; //替换为空格
            }
            
            temp_str++;
        }

    }

    return;
}

/*******************************************************************************************/
// 	Description :
//	Global resource dependence :
//  Author: 
//	Note:
/*********************************************************************************************/
GUI_FONT_T GUI_ConvertSizeToGUIFont( uint16 size )
{
    GUI_FONT_T font_size = GUI_MAX_FONT_NUM - 1;
    
    if ( size < font_size )
    {
        font_size = (GUI_FONT_T)size;
    }

    return font_size;
}

#endif  //_GUI_FONT_C_
