/*****************************************************************************
** File Name:      guicommon.c                                               *
** Author:                                                                   *
** Date:           16/04/2004                                                *
** Copyright:      2003 Spreadtrum,  Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe gui common function.contain *
**                     1 the font                                            *
**                     2 the border                                          *
**                     3 title                                               *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 04/2004       panda.luo          Create                                   *
******************************************************************************/

#define _GUICOMMON_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_gui_trc.h"
#include "guicommon.h"
#include "mmk_timer.h"
#include "guilcd.h"
#include "guifont.h"
#include "mmi_imagefun.h"
#include "mmi_theme.h"
#include "mmk_app.h"
#include "guilistbox.h"
#include "guimenu.h"
#include "guilabel.h"
//#include "guimsgbox_internal.h"
#include "guianim.h"
#include "guires.h"
#ifndef WIN32
#include "dal_jpeg.h"
#include "chng_freq.h"
#endif
#include "gui_unicode.h"

#include "spml_api.h"
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
#ifndef  WIN32
LOCAL uint32 s_jpg_display_chng_freq_req_handler = CHNG_FREQ_REQ_HANDLER_NULL;
#endif

LOCAL uint32 s_cur_tick_count = 0;
                            
LOCAL BOOLEAN s_is_alpha_overwrite = FALSE;
/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/

 /**-------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*******************************************************************************************/
// 	Description : is rect valid
//	Global resource dependence :
//  Author: Jassmine
//	Note:   
/*********************************************************************************************/
LOCAL BOOLEAN IsValidRect( 
                          GUI_RECT_T    rect
                          );

/*****************************************************************************/
//  Description : display sheen
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void DisplaySheen(
                        GUI_SHEEN_T     *sheen_ptr
                        );

/*****************************************************************************/
//  Description : display sheen rect,width = 1
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void DisplaySheenRect(
                            GUI_RECT_T          *rect_ptr,  //in:
                            GUI_SHEEN_T         *sheen_ptr, //in/out:
                            GUI_SHEEN_DEPTH_E   depth       //in:
                            );

/*****************************************************************************/
//  Description : translate every pixel sheen effect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void TranslateSheenPixel(
                               GUI_COLOR_T          src_pixel,          //in:
                               GUI_COLOR_T          *sheen_pixel_ptr,   //in/out:
                               GUI_SHEEN_DEPTH_E    depth               //in:
                               );

/*****************************************************************************/
//  Description : translate every pixel sheen effect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void TranslateSheenPixel32(
                                 uint32             src_pixel,          //in:
                                 uint32             *sheen_pixel_ptr,   //in/out:
                                 GUI_SHEEN_DEPTH_E  depth               //in:
                                 );

/*****************************************************************************/
//  Description : recover sheen rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void RecoverSheenRect(
                            GUI_SHEEN_T *sheen_ptr
                            );

/**-------------------------------------------------------------------------------------------*
 **                                     FUNCTION DEFINITION                                   *
 **-------------------------------------------------------------------------------------------*/
/*******************************************************************************************/
// 	Description : 创建rect
//	Global resource dependence :
//  Author: panda.luo
//	Note:
/*********************************************************************************************/
PUBLIC GUI_RECT_T GUI_CreateRect(
                                 uint16 left,
                                 uint16 top,
                                 uint16 right,
                                 uint16 bottom
                                 )
{
    GUI_RECT_T rect = {0};

    rect.left = left;
    rect.top = top;
    rect.right = right;
    rect.bottom = bottom;
    return rect;
}

/*******************************************************************************************/
// 	Description : 根据输入的原始区域及中间区域的高度和宽度，求一个区域的中间区域
//	Global resource dependence :
//  Author: xiaoqing.lu
//	Note:   返回值一个区域,返回的区域不会超过原始区域
/*********************************************************************************************/
PUBLIC GUI_RECT_T GUI_GetCenterRect(
                                    GUI_RECT_T src_rect,
                                    uint16     width,
                                    uint16     height
                                    )
{
    uint16     src_width = 0;
    uint16     src_height = 0;
    GUI_RECT_T ret_rect = {0};

    if (!GUI_IsInvalidRect(src_rect))
    {
        ret_rect = src_rect;
        src_width = src_rect.right - src_rect.left + 1;
        src_height = src_rect.bottom - src_rect.top + 1;

        if (width < src_width)
        {
            ret_rect.left = (src_rect.left + src_rect.right + 1 - width) / 2;
            ret_rect.right = ret_rect.left + width - 1;
        }

        if (height < src_height)
        {
            ret_rect.top = (src_rect.top + src_rect.bottom + 1 - height) / 2;
            ret_rect.bottom = ret_rect.top + height - 1;
        }
    }
    
    return (ret_rect);
}

/*******************************************************************************************/
// 	Description : 根据输入的原始区域及中间区域的高度和宽度，求一个区域的中间区域
//	Global resource dependence :
//  Author: xiaoqing.lu
//	Note:   返回值一个区域，返回的区域可以超过原始区域
/*********************************************************************************************/
PUBLIC GUI_RECT_T GUI_GetCenterRectExt(
                                       GUI_RECT_T src_rect,
                                       uint16     width,
                                       uint16     height
                                       )
{
    uint16     src_width = 0;
    uint16     src_height = 0;
    GUI_RECT_T ret_rect = {0};

    if (!GUI_IsInvalidRectEx(src_rect))
    {
        ret_rect = src_rect;
        src_width = src_rect.right - src_rect.left + 1;
        src_height = src_rect.bottom - src_rect.top + 1;
        
        
        ret_rect.left = (src_rect.left + src_rect.right + 1 - width) / 2;
        ret_rect.right = ret_rect.left + width - 1;
        ret_rect.top = (src_rect.top + src_rect.bottom + 1 - height) / 2;
        ret_rect.bottom = ret_rect.top + height - 1;
    }
    
    return (ret_rect);
}


/*******************************************************************************************/
// 	Description : 求两个rect的相交部分
//	Global resource dependence :
//  Author: bin.ji
//	Note:   返回值FALSE没有相交部分TRUE有相交部分
/*********************************************************************************************/
PUBLIC BOOLEAN GUI_IntersectRect(
                                 GUI_RECT_T    *rect_dest_ptr,
                                 GUI_RECT_T    rect1,
                                 GUI_RECT_T    rect2
                                 )
{
    BOOLEAN     result = TRUE;

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != rect_dest_ptr);/*assert verified*/
    if (PNULL == rect_dest_ptr)
    {
        return FALSE;
    }
    
    rect_dest_ptr->left   = (int16)(MAX(rect1.left, rect2.left));
    rect_dest_ptr->top    = (int16)(MAX(rect1.top, rect2.top));
    rect_dest_ptr->right  = (int16)(MIN(rect1.right, rect2.right));
    rect_dest_ptr->bottom = (int16)(MIN(rect1.bottom, rect2.bottom));
    
    if ((rect_dest_ptr->left > rect_dest_ptr->right) ||
        (rect_dest_ptr->top > rect_dest_ptr->bottom))
    {
        result = FALSE;
    }
    else if ((0 == rect_dest_ptr->left) &&
             (0 == rect_dest_ptr->top) &&
             (0 == rect_dest_ptr->right) &&
             (0 == rect_dest_ptr->bottom))
    {
        result = FALSE;
    }
    
    return (result);
}

/*******************************************************************************************/
// 	Description : 一个区域完全在另一个区域内
//	Global resource dependence :
//  Author: xiaoqing.lu
//	Note:   返回值FALSE或TRUE，rect_dest_ptr获取交集
/*********************************************************************************************/
PUBLIC BOOLEAN GUI_RectIsCovered(
                                 GUI_RECT_T    orig_rect,
                                 GUI_RECT_T    rect
                                 )
{
    if (rect.left < orig_rect.left || rect.top < orig_rect.top 
        || rect.right > orig_rect.right || rect.bottom > orig_rect.bottom)
    {
        return FALSE;
    }
    return TRUE;
}

/*******************************************************************************************/
// 	Description : is rect empty
//	Global resource dependence :
//  Author: James.Zhang
//	Note:   
/*********************************************************************************************/
PUBLIC BOOLEAN GUI_IsRectEmpty( 
                               GUI_RECT_T rect 
                               )
{
    return (BOOLEAN)( rect.right == rect.left || rect.bottom == rect.top );
}

/*******************************************************************************************/
// 	Description : is zero rect
//	Global resource dependence :
//  Author: James.Zhang
//	Note:   
/*********************************************************************************************/
PUBLIC BOOLEAN GUI_IsZeroRect( 
                              GUI_RECT_T rect 
                              )
{
    return (BOOLEAN)( 0 == rect.right && 0 == rect.left && 0 == rect.bottom && 0 == rect.top );
}

/*******************************************************************************************/
// 	Description : is rect invalid
//	Global resource dependence :
//  Author: xiaoqing.lu
//	Note:   
/*********************************************************************************************/
PUBLIC BOOLEAN GUI_IsInvalidRect( 
                                 GUI_RECT_T rect 
                                 )
{
    return (BOOLEAN)((rect.right <= rect.left) || rect.bottom <= rect.top);
}

/*******************************************************************************************/
// 	Description : is rect invalid
//	Global resource dependence :
//  Author: xiaoqing.lu
//	Note:   
// 判断矩形区是否有效，这里会把右、下点包括进来，当成有效点，而GUI_IsInvalidRect
// 则不包括。MOCOR平台的RECT其实是包括右下点的，理论上来说，GUI_IsInvalidRect
// 的实现有问题，但现在rect={0,0,0,0}被视为无效，很多用GUI_IsInvalidRect判断，
// 这里调用的地方很多，暂时不动GUI_IsInvalidRect这个函数，新加一个Ex类型
/*********************************************************************************************/
PUBLIC BOOLEAN GUI_IsInvalidRectEx( 
                                   GUI_RECT_T rect 
                                   )
{
    return (BOOLEAN)((rect.right < rect.left) || (rect.bottom < rect.top));
}

/*******************************************************************************************/
// 	Description : 用指定颜色填充矩形框
//	Global resource dependence :
//  Author: panda.luo
//	Note:
/*********************************************************************************************/
PUBLIC void GUI_FillRect(
                         GUI_LCD_DEV_INFO const	*dev_info_ptr,	// block information
                         GUI_RECT_T				rect,   //要填充的矩形框
                         GUI_COLOR_T			color   //颜色
                         )
{
   //GUI_CHECK_RECT(rect);
   
   LCD_FillRect(dev_info_ptr, rect, color);
}
                           
/*******************************************************************************************/
// 	Description : 检查给定的矩形是否可以在屏幕上显示
//	Global resource dependence :
//  Author: panda.luo
//	Note:
/*********************************************************************************************/
PUBLIC BOOLEAN GUI_CheckRectValid(
                                  GUI_RECT_T rect
                                  )
{
    SCI_ASSERT(rect.right  >= rect.left ); /*assert verified*/
    SCI_ASSERT(rect.bottom  >= rect.top); /*assert verified*/

    return TRUE;
}

/*****************************************************************************/
// 	Description : 得到a除以b的商
//	Global resource dependence : 
//  Author: Jibin
//	Note:
/*****************************************************************************/
PUBLIC uint16 GUI_Divide(
                         uint16 a,
                         uint16 b
                         ) 
{
    if (0 != b)
    {
        return (uint16)(a / b); 
    }
    else
    {
        return 0;
    }
}

/*******************************************************************************************/
// 	Description : compare string, 
//                if str1 > str2, return >0; 
//                   str1 = str2, return =0; 
//                   str1 < str2, return <0
//	Global resource dependence :
//  Author: lin.lin
//	Note:
/*********************************************************************************************/
PUBLIC int32 GUI_StringCompare(
                               uint8* str1,         //compared string
                               uint16 str1_len,     //string length
                               uint8* str2,         //compared string
                               uint16 str2_len      //string length
                               )
{
     uint16 num = 0;
     int32  res = 0;
     
    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != str1);/*assert verified*/
    //SCI_ASSERT(PNULL != str2);/*assert verified*/
    if ((PNULL == str1)||(PNULL == str2))
    {
        return res;
    }

    num = (uint16)(MIN(str1_len, str2_len));
    res = memcmp((void *)str1, (void *)str2, num);
    
    if ((0 == res) && (str1_len != str2_len))
    {
        res = (str1_len > str2_len)? 1 : -1;
    }
    
    return res;
} 

/*******************************************************************************************/
// 	Description : 多行显示时计算第一行第一个字符所在位置
//	Global resource dependence :
//  Author: bin.ji
//	Note:
/*********************************************************************************************/
PUBLIC uint16 GUI_CalculateStringFirstPosByLines(
                                                 uint16   line_num,    //[IN] 行数
                                                 uint16    pixelnum,   //[IN] 像素个数
                                                 const wchar*     str_ptr,    //[IN] String指针
                                                 uint16     str_len,    //[IN] String长度(Byte长度)
                                                 //BOOLEAN    is_ucs2,    //[IN] 是否为ucs2编码
                                                 GUI_FONT_T font,       //[IN] String的字体
                                                 uint8      char_space  //[IN] 字间距
                                                 )
{
    uint16 i =0;
    uint16 line_length = 0;
    uint16 cur_len = 0;

    uint16 pixel_num_line = 0;
    
    for (i = 0; i < line_num; i++)
    {
        if( GUI_GetStringInfoInLine(&str_ptr[cur_len], 
            (uint16)(str_len - cur_len),
            font,
            char_space,
            pixelnum,
            &line_length,
            &pixel_num_line,
            TRUE
            ) )
        {
            
            if (0 == line_length)
            {
                return (0);
            }
            cur_len += line_length;
            if (cur_len >= str_len)
            {
                cur_len = 0;  //@Lin.Lin, 2004-7-16
                break;
            }
        }
        else
        {
            break;
        }
    }
    return cur_len;
}

/*******************************************************************************************/
// 	Description : calculate the ucs2 string by piexl number
//	Global resource dependence :
//  Author: bin.ji
//	Note:
/*********************************************************************************************/
PUBLIC uint16 GUI_CalculateStringLinesByPixelNum(
                                                 uint16    pixelnum,    //[IN] 像素个数
                                                 const wchar* str_ptr,  //[IN] String指针
                                                 uint16     str_len,    //[IN] String长度(Byte长度)
                                                 GUI_FONT_T font,       //[IN] String的字体
                                                 uint8      char_space, //[IN] 字间距
                                                 BOOLEAN    is_byword   //[IN] 是否按词换行
                                                 )
{
    uint16 i =0;
    uint16 line_length = 0;
    uint16 cur_len = 0;

    uint16 pixel_num_line=0;

    if ((PNULL == str_ptr) || (0 == str_len))
    {
        return (0);
    }

    if (0 == pixelnum)
    {
        return (0);
    }

    while(cur_len < str_len)
    {
        if( GUI_GetStringInfoInLine(&str_ptr[cur_len], 
            (uint16)(str_len - cur_len),
            font,
            char_space,
            pixelnum,
            &line_length,
            &pixel_num_line,
            is_byword
            ) )
        {
            if (0 == line_length)
            {
                return (0);
            }
            cur_len += line_length;
            i++;
        }
        else
        {
            break;
        }
    }
    return i;
}

/*******************************************************************************************/
// 	Description : calculate the char lines by pixel number
//	Global resource dependence :
//  Author: Jassmine
//	Note:
/*********************************************************************************************/
PUBLIC uint16 GUI_CalculateCharLinesByPixelNum(
                                               uint16       pixelnum,   //[IN] 像素个数
                                               wchar        w_char,     //[IN] String指针
                                               uint16       char_len,    //[IN] String长度(Byte长度)
                                               GUI_FONT_T   font,       //[IN] String的字体
                                               uint8        char_space  //[IN] 字间距
                                               )
{
    uint16  cur_char_len = 0;
    uint16  line_num = 0;
    uint16  char_num = 0;
    uint16  char_pixel = 0;

    if ((0 < pixelnum) &&
        (0 < char_len))
    {
        while (cur_char_len < char_len)
        {
            GUI_GetCharInfoInLine(w_char,
                char_len,
                font,
                char_space,
                pixelnum,
                &char_num,
                &char_pixel);

            if (0 == char_num)
            {
                break;
            }
            else
            {
                cur_char_len += char_num;
                line_num++;
            }
        }
    }

    return (line_num);
}

/*******************************************************************************************/
// 	Description : calculate the ucs2 string piexl number
//	Global resource dependence :
//  Author: Great.Tian
//	Note:
/*********************************************************************************************/
PUBLIC uint16 GUI_CalculateStringPiexlNum(
                                          const wchar* str_ptr,    //[IN] String指针
                                          uint16       str_len,    //[IN] String长度(Byte长度)
                                          GUI_FONT_T   font,       //[IN] String的字体
                                          uint8        char_space  //[IN] 字间距
                                          )
{
    uint16 str_piexl_num = 0;

    str_piexl_num = GUI_GetStringWidthEx(font, str_ptr, str_len, char_space);

    return (str_piexl_num);

}

#ifndef SCRIPTEASY_SUPPORT  /* 暂时没有用到 */
/*******************************************************************************************/
// 	Description : word bread char
//	Global resource dependence :
//  Author: James Zhang
//	Note: 按词换行的字符
/*********************************************************************************************/
PUBLIC BOOLEAN GUI_IsWordBreakChar(
                                   wchar ch 
                                   )
{
	return SPMLAPI_IsWordBreakChar(ch);
}
#endif

/*******************************************************************************************/
// 	Description : 该函数用于断行
//	Global resource dependence :
//  Author: James.Zhang
//	Note: 传入文字的方向, 在有双向文字时断行才能更加准确
/*********************************************************************************************/
PUBLIC BOOLEAN GUI_GetStringInfoInLineWithDir(
                                               int8             dir,
                                               const wchar		*str_ptr,
                                               uint16      		str_len,
                                               GUI_FONT_T  		font,
                                               uint8       		char_space,
                                               uint16      		width,
                                               uint16			*char_num,
                                               uint16			*pixel_num,
                                               BOOLEAN          is_byword
                                               )
{
    //kevin.lou modified:delete assert
    //SCI_ASSERT( PNULL != char_num && PNULL != pixel_num );/*assert verified*/
    if( (PNULL == str_ptr) || (0 == *str_ptr) || (0 == str_len) || (0 == width) || (PNULL == char_num) || (PNULL == pixel_num))
    {
        return FALSE;
    }
    else
    {
#ifdef SCRIPTEASY_SUPPORT
        GUI_AnalysisNextLine(str_ptr, str_len, 0, font, width, char_num, pixel_num); /*lint !e718 !e746*/
        if (*pixel_num > width)
        {
            *pixel_num = width;
        }
#else    	
    //存在连字和叠加文字时, 必须精确计算, 否则可以逐字进行估算
#if (defined COMPLEX_TEXT_SCRIPT || defined VECTOR_FONT_SUPPORT_ETRUMP)

        SPML_TEXT_PARAM_T param = {0};
        
        param.glyph_space = char_space;
        
        param.dir = dir;
        
        param.size = GUI_ConvertGUIFontToSize( font );
        
        *char_num = SPMLAPI_LineBreak( &param, str_ptr, str_len, width, pixel_num, is_byword );

#else
        
        uint16  w_char = 0;
        uint16  i = 0;
        uint8   step = 0;
        uint8   char_width = 0;
        uint16  pre_char_num  = 0;
        uint16  pre_pixel_num = 0;
        BOOLEAN is_exist_word_bread_ch = FALSE;
        
        SCI_ASSERT(PNULL != str_ptr);/*assert verified*/
        
        step = 1;
        
        *char_num = 0;
        *pixel_num = 0;
        
        do {
            w_char = str_ptr[i];
            
            if( 0 == w_char )//james, \0返回
            {
                break;
            }
            if (CR_CHAR == w_char)  //回车
            {
                //CR28985, @Lin.Lin, 2005-8-8, 当字符串中包含0D0A时,计算行数不正确, start
                uint16 next_char = 0;
                
                if(i+1 < str_len)
                {
                    next_char = str_ptr[i+1];
                }
                
                if(LF_CHAR == next_char)
                {
                    *char_num += 2*step;
                }
                else
                    //CR28985, @Lin.Lin, 2005-8-8, 当字符串中包含0D0A时,计算行数不正确, end
                {
                    *char_num += step;
                }
                break;
            }
            else if(LF_CHAR == w_char)  //换行
            {
                //if (0 == *char_num)
                {
                    *char_num += step;
                }
                break;
            }
            else
            {
                char_width = GUI_GetFontWidth( font, w_char );
                
                if ((*pixel_num + char_width) > width)
                {
                    //存在按词换行字符
                    if( is_exist_word_bread_ch )
                    {
                        *pixel_num = pre_pixel_num;
                        *char_num  = pre_char_num;
                    }
                    break;
                }
                else    
                {
                    if (i+1 == str_len)
                    {
                        *pixel_num += char_width;
                    }
                    else
                    {
                        *pixel_num += char_width + char_space;
                    }
                    *char_num = (uint16)(i + step);
                    
                    //按词换行
                    if( is_byword && SPMLAPI_IsWordBreakChar( w_char ) )
                    {
                        pre_pixel_num = *pixel_num;
                        pre_char_num  = *char_num;
                        
                        is_exist_word_bread_ch = TRUE;
                    }
                }
            }
            
            i += step;        
        } while(i < str_len);
        
        //可以显示的下最后一个字符,但是无法显示的下最后一个字间距
        if (*pixel_num > width)
        {
            *pixel_num = width;
        }
        
#endif
#endif  /* SCRIPTEASY_SUPPORT */
        return TRUE;
    }
}

/*******************************************************************************************/
// 	Description : Get string info in one lines, include pixel num and char num
//	Global resource dependence :
//  Author: Lin.Lin
//	Note: note: \0 需要立即返回
/*********************************************************************************************/
PUBLIC BOOLEAN GUI_GetStringInfoInLine(
                                       const wchar* str_ptr,    //[IN] String指针
                                       uint16       str_len,    //[IN] String长度(Byte长度)
                                       GUI_FONT_T   font,       //[IN] String的字体
                                       uint8        char_space,  //[IN] 字间距
                                       uint16       width,
                                       uint16*      char_num,
                                       uint16*      pixel_num,
                                       BOOLEAN      is_byword
                                       )
{
    return GUI_GetStringInfoInLineWithDir( TEXT_DIR_AUTO, str_ptr, str_len, font, char_space, width, char_num, pixel_num, is_byword );
}

/*******************************************************************************************/
// 	Description : get char info in one lines, include pixel num and char num
//	Global resource dependence :
//  Author: Jassmine
//	Note: note:
/*********************************************************************************************/
PUBLIC void GUI_GetCharInfoInLine(
                                  wchar         w_char,
                                  uint16        char_len,
                                  GUI_FONT_T    font,
                                  uint8         char_space,
                                  uint16        width,
                                  uint16        *char_num_ptr,
                                  uint16        *pixel_num_tr
                                  )
{
    uint8   char_width = 0;
    uint16  char_num = 0;
    uint16  pixel_num = 0;

    if ((0 != w_char) &&
        (0 < char_len))
    {
        if ((CR_CHAR == w_char) || (LF_CHAR == w_char))
        {
            char_num = 1;
            pixel_num = 0;
        }
        else
        {
            do 
            {
            	char_width = GUI_GetFontWidth( font, w_char );
                if ((pixel_num + char_width) < width)
                {
                    pixel_num += char_width + char_space;
                    char_num++;
                }
                else
                {
                    break;
                }
                
            } while(char_num < char_len);

            //可以显示的下最后一个字符,但是无法显示的下最后一个字间距
            if (pixel_num > width)
            {
                pixel_num = width;
            }
        }
    }

    //set char number and pixel number
    *char_num_ptr = char_num;
    *pixel_num_tr = pixel_num;
}

/*******************************************************************************************/
// 	Description : Whether the point is in the rectangle, not including the border.
//	Global resource dependence :
//  Author: Robert.Lu
//	Note:   返回值FALSE not, TRUE is in
/*********************************************************************************************/
PUBLIC BOOLEAN GUI_PointIsInRect(
                                 GUI_POINT_T    point,
                                 GUI_RECT_T     rect 
                                 )
{
    if (point.x >= rect.left && point.x <= rect.right && point.y >= rect.top && point.y <= rect.bottom)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*****************************************************************************/
//  Description : check two rect whether is equal
//  Global resource dependence : 
//  Author: Hui.Zhang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUI_EqualRect(
                             GUI_RECT_T    rect_1,
                             GUI_RECT_T    rect_2
                             )
{
    if(rect_1.left == rect_2.left
        && rect_1.right == rect_2.right
        && rect_1.top == rect_2.top
        && rect_1.bottom == rect_2.bottom)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*****************************************************************************/
//  Description : rect width and height is equal
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN GUI_IsRectSizeEqual(
                                   GUI_RECT_T    rect1,  //in
                                   GUI_RECT_T    rect2   //in
                                   )
{
    BOOLEAN     result = FALSE;
    
    if (((rect1.right - rect1.left) == (rect2.right - rect2.left)) &&
        ((rect1.bottom - rect1.top) == (rect2.bottom - rect2.top)))
    {
        result = TRUE;
    }
    
    return (result);
}

/*****************************************************************************/
//    Description : check whether year is leap year
//    Global resource dependence : 
//    Author: liqing.peng
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUI_IsLeapYear(uint32 year)
{
	BOOLEAN ret_val = FALSE;

	ret_val = (BOOLEAN)((!(year % 4) && (year % 100)) || !(year % 400));

	return ret_val;
}

/*****************************************************************************/
//  Description : 创建一个矩阵中每个Item的区域
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note: 
/*****************************************************************************/
PUBLIC void GUI_CreateMatrixRect(
                                 GUI_RECT_T *total_rect_ptr, // 整个区域
                                 int16 h_side_margin,        // 水平边距
                                 int16 v_side_margin,        // 垂直边距
                                 int16 h_item_space,         // 水平间距
                                 int16 v_item_space,         // 垂直间距
                                 int16 row_num,             // 行数
                                 int16 col_num,             // 列数
                                 GUI_RECT_T *item_rect_ptr   // 输出的item 区域，是一个二位数组，行和列由line_num、row_num定义
                                 )
{
    int16       total_width     = 0;
    int16       total_height    = 0;
    int16       item_width      = 0;
    int16       item_height     = 0;
    int16       width_rem       = 0;
    int16       height_rem      = 0;
    int16       i               = 0;
    int16       j               = 0;
    GUI_RECT_T  total_rect      = {0};

    // 检测条件
    if (PNULL != total_rect_ptr && PNULL != item_rect_ptr && col_num > 0 && row_num > 0)
    {
        // 计算总宽度
        total_width = total_rect_ptr->right - total_rect_ptr->left + 1 - 2*h_side_margin - (col_num - 1)*h_item_space;

        // 计算每个项的宽度
        item_width = total_width/col_num;
        width_rem = total_width%col_num;

        // 计算总高度
        total_height = total_rect_ptr->bottom - total_rect_ptr->top + 1 - 2*v_side_margin - (row_num - 1)*v_item_space;

        // 计算每个项高度
        item_height = total_height/row_num;
        height_rem = total_height%row_num;

        // 计算总区域，去除了边距
        total_rect.left = total_rect_ptr->left + h_side_margin + width_rem/2 + width_rem%2;
        total_rect.top = total_rect_ptr->top + v_side_margin + height_rem/2 + height_rem%2;
        total_rect.right = total_rect_ptr->right - h_side_margin - width_rem/2;
        total_rect.bottom = total_rect_ptr->bottom - v_side_margin - height_rem/2;

        // 计算每个项的区域，用2维空间表示
        for (j = 0; j < row_num; j++)
        {
            for (i = 0; i < col_num; i++)
            {
                ((GUI_RECT_T *)(item_rect_ptr + col_num*j + i))->left = total_rect.left + i*h_item_space + i*item_width;
                ((GUI_RECT_T *)(item_rect_ptr + col_num*j + i))->top = total_rect.top + j*v_item_space + j*item_height;
                ((GUI_RECT_T *)(item_rect_ptr + col_num*j + i))->right = ((GUI_RECT_T *)(item_rect_ptr + col_num*j + i))->left + item_width - 1;
                ((GUI_RECT_T *)(item_rect_ptr + col_num*j + i))->bottom = ((GUI_RECT_T *)(item_rect_ptr + col_num*j + i))->top + item_height - 1;
            }
        }
    }

    return;
}


/*****************************************************************************/
//  Description : display border
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void GUI_DisplayBorder(
                              GUI_RECT_T        rect,           //in:
                              GUI_RECT_T        display_rect,   //in:
                              GUI_BORDER_T      *border_ptr,    //in:
                              GUI_LCD_DEV_INFO  *lcd_dev_ptr    //in:
                              )
{
    GUI_RECT_T      border_rect = {0};
    GUI_RECT_T      cross_rect = {0};

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != border_ptr);/*assert verified*/
    if (PNULL == border_ptr)
    {
        return;
    }

    switch (border_ptr->type)
    {
    case GUI_BORDER_NONE:
        break;

    case GUI_BORDER_SOLID:
        //set top border rect
        border_rect        = rect;
        border_rect.bottom = (int16)(border_rect.top + border_ptr->width - 1);

        //get crossed rect
        if (GUI_IntersectRect(&cross_rect,display_rect,border_rect))
        {
            //display top border
            GUI_FillRect((const GUI_LCD_DEV_INFO*)lcd_dev_ptr,
                cross_rect,
                border_ptr->color);
        }

        //set bottom border rect
        border_rect     = rect;
        border_rect.top = (int16)(border_rect.bottom - border_ptr->width + 1);

        //get crossed rect
        if (GUI_IntersectRect(&cross_rect,display_rect,border_rect))
        {
            //display bottom border
            GUI_FillRect((const GUI_LCD_DEV_INFO*)lcd_dev_ptr,
                cross_rect,
                border_ptr->color);
        }

        //set left border rect
        border_rect       = rect;
        border_rect.right = (int16)(border_rect.left + border_ptr->width - 1);

        //get crossed rect
        if (GUI_IntersectRect(&cross_rect,display_rect,border_rect))
        {
            //display left border
            GUI_FillRect((const GUI_LCD_DEV_INFO*)lcd_dev_ptr,
                cross_rect,
                border_ptr->color);
        }

        //set right border rect
        border_rect      = rect;
        border_rect.left = (int16)(border_rect.right - border_ptr->width + 1);

        //get crossed rect
        if (GUI_IntersectRect(&cross_rect,display_rect,border_rect))
        {
            //display right border
            GUI_FillRect((const GUI_LCD_DEV_INFO*)lcd_dev_ptr,
                cross_rect,
                border_ptr->color);
        }
        break;
        
    case GUI_BORDER_ROUNDED:
        LCD_DrawRoundedRect((const GUI_LCD_DEV_INFO*)lcd_dev_ptr,
            rect,
            display_rect,
            border_ptr->color);
        break;

    case GUI_BORDER_SROUNDED:
        LCD_DrawShadowRoundedRect((const GUI_LCD_DEV_INFO*)lcd_dev_ptr,
            rect,
            display_rect,
            border_ptr->color);
        break;

    default:
        //SCI_TRACE_LOW:"GUI_DisplayBorder:border type %d is error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUICOMMON_1029_112_2_18_3_15_16_57,(uint8*)"d",border_ptr->type);
        break;
    }
}

/*****************************************************************************/
//  Description : display border
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void GUI_DisplayBorderToBuf(
                                   GUI_COLOR_T      *buf_ptr,   //in:
                                   uint16           buf_width,  //in:
                                   uint16           buf_height, //in:
                                   GUI_RECT_T       rect,       //in:相对于buf
                                   GUI_BORDER_T     *border_ptr //in:
                                   )
{
    uint32          buf_size = (uint32)(buf_width * buf_height);
    GUI_RECT_T      border_rect = {0};

    switch (border_ptr->type)
    {
    case GUI_BORDER_NONE:
        break;

    case GUI_BORDER_SOLID:
        //set top border rect
        border_rect        = rect;
        border_rect.bottom = (int16)(border_rect.top + border_ptr->width - 1);

        //display top border
        LCD_FillRectTobuffer(buf_ptr,
            buf_size,
            buf_width,
            buf_height,
            border_rect,
            border_ptr->color);

        //set bottom border rect
        border_rect     = rect;
        border_rect.top = (int16)(border_rect.bottom - border_ptr->width + 1);

        //display bottom border
        LCD_FillRectTobuffer(buf_ptr,
            buf_size,
            buf_width,
            buf_height,
            border_rect,
            border_ptr->color);

        //set left border rect
        border_rect       = rect;
        border_rect.right = (int16)(border_rect.left + border_ptr->width - 1);

        //display left border
        LCD_FillRectTobuffer(buf_ptr,
            buf_size,
            buf_width,
            buf_height,
            border_rect,
            border_ptr->color);

        //set right border rect
        border_rect      = rect;
        border_rect.left = (int16)(border_rect.right - border_ptr->width + 1);

        //display right border
        LCD_FillRectTobuffer(buf_ptr,
            buf_size,
            buf_width,
            buf_height,
            border_rect,
            border_ptr->color);
        break;
        
    case GUI_BORDER_ROUNDED:
        LCD_DrawRoundedRectToBuf(buf_ptr,
            buf_width,
            buf_height,
            rect,
            border_ptr->color);
        break;

    case GUI_BORDER_SROUNDED:
        LCD_DrawShadowRoundedRectToBuf(buf_ptr,
            buf_width,
            buf_height,
            rect,
            border_ptr->color);
        break;

    default:
        //SCI_TRACE_LOW:"GUI_DisplayBorderToBuf:border type %d is error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUICOMMON_1121_112_2_18_3_15_17_58,(uint8*)"d",border_ptr->type);
        break;
    }
}

/*****************************************************************************/
//  Description : display background
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void GuiComDisplayBg(
						   GUI_BG_T          *bg_ptr,        //in:
						   GUI_BG_DISPLAY_T  *display_ptr,   //in:
						   BOOLEAN			 is_notify_update, // 当背景为GUI_BG_NONE时，是否无条件给父亲发送MSG_NOTIFY_UPDATE
						   GUI_LCD_DEV_INFO  *lcd_dev_ptr    //in:
						   )
{
    GUI_RECT_T      cross_rect = {0};
    MMI_IMAGE_ID_T  common_bg_id = 0;
    MMI_HANDLE_T    handle = 0;

    switch (bg_ptr->bg_type)
    {
    case GUI_BG_NONE:
        if ( !MMK_IsWinOnPaint( display_ptr->win_handle ) ||is_notify_update )
        {
            if (display_ptr->is_form_bg)
            {
                //notify window update bg
                MMK_SendMsg(display_ptr->win_handle,MSG_NOTIFY_UPDATE,&display_ptr->display_rect);
            }
            else
            {
                //notify parent update bg
                GUICTRL_SendNotifyEx(display_ptr->ctrl_handle,MSG_NOTIFY_UPDATE,&display_ptr->display_rect);
            }
        }
        break;

    case GUI_BG_IMG:
        //get common bg
        common_bg_id = MMITHEME_GetCommonBg().img_id;

        if (display_ptr->is_transparent)
        {
            //display bg
            MMITHEME_DisplayCommonBg(&display_ptr->display_rect,display_ptr->win_handle,lcd_dev_ptr);
        }
        else
        {
            if ( !MMK_IsWinOnPaint( display_ptr->win_handle ) || is_notify_update)
            {
                if (GUIRES_IsArgb(bg_ptr->img_id,display_ptr->win_handle))
                {
                    if (display_ptr->is_form_bg)
                    {
                        //notify window update bg
                        MMK_SendMsg(display_ptr->win_handle,MSG_NOTIFY_UPDATE,&display_ptr->display_rect);
                    }
                    else
                    {
                        //notify parent update bg
                        GUICTRL_SendNotifyEx(display_ptr->ctrl_handle,MSG_NOTIFY_UPDATE,&display_ptr->display_rect);
                    }
                }
            }
        }

        //adjust image rect
        if (common_bg_id == bg_ptr->img_id)
        {
            display_ptr->img_rect = display_ptr->display_rect;
            handle = VIRTUAL_WIN_ID;
        }
        else
        {
            if (bg_ptr->is_screen_img)
            {
                display_ptr->img_rect = display_ptr->display_rect;
            }

            handle = display_ptr->win_handle;
        }

        GUIRES_DisplaySabmImg(
            &display_ptr->display_rect,
            &display_ptr->rect,
            &display_ptr->img_rect,
            handle,
            bg_ptr->img_id,
            (const GUI_LCD_DEV_INFO*)lcd_dev_ptr);
        break;

    case GUI_BG_COLOR:
        switch (bg_ptr->shape)
        {
        case GUI_SHAPE_RECT:
            if (GUI_IntersectRect(&cross_rect,display_ptr->rect,display_ptr->display_rect))
            {
                GUI_FillRect(lcd_dev_ptr,
                    cross_rect,
                    bg_ptr->color);
            }
            break;

        case GUI_SHAPE_ROUNDED_RECT:
            LCD_FillRoundedRect(lcd_dev_ptr,
                display_ptr->rect,
                display_ptr->display_rect,
                bg_ptr->color);
            break;

        case GUI_SHAPE_SROUNDED_RECT:
            LCD_FillShadowRoundedRect(lcd_dev_ptr,
                display_ptr->rect,
                display_ptr->display_rect,
                bg_ptr->color);
            break;

        default:
            //SCI_TRACE_LOW:"GUI_DisplayBg:shape %d is error !"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUICOMMON_1239_112_2_18_3_15_17_59,(uint8*)"d",bg_ptr->shape);
            break;
        }
        break;

    default:
        //SCI_TRACE_LOW:"GUI_DisplayBg:bg_type %d is error !"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUICOMMON_1245_112_2_18_3_15_17_60,(uint8*)"d",bg_ptr->bg_type);
        break;
    }
}

/*****************************************************************************/
//  Description : display background
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void GUI_DisplayBg(
                          GUI_BG_T          *bg_ptr,        //in:
                          GUI_BG_DISPLAY_T  *display_ptr,   //in:
                          GUI_LCD_DEV_INFO  *lcd_dev_ptr    //in:
                          )
{
	GuiComDisplayBg(bg_ptr, display_ptr, FALSE, lcd_dev_ptr);
}

/*****************************************************************************/
//  Description : display background
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void GUI_DisplayBgExt(
							 GUI_BG_T          *bg_ptr,        //in:
							 GUI_BG_DISPLAY_T  *display_ptr,   //in:
							 BOOLEAN			 is_notify_update, // 当背景为GUI_BG_NONE时，是否无条件给父亲发送MSG_NOTIFY_UPDATE
							 GUI_LCD_DEV_INFO  *lcd_dev_ptr    //in:
							 )
{
	GuiComDisplayBg(bg_ptr, display_ptr, is_notify_update, lcd_dev_ptr);
}

/*****************************************************************************/
//  Description : set vertical or horizontal both rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
PUBLIC void GUI_SetVOrHRect(
                            MMI_HANDLE_T        ctrl_handle,
                            GUI_RECT_T          *rect_ptr,      //in:
                            GUI_BOTH_RECT_T     *both_rect_ptr  //in/out:
                            )
{
    if ( MMK_IsWindowLandscape( MMK_GetWinHandleByCtrl( ctrl_handle ) ) )
    {
        both_rect_ptr->h_rect = *rect_ptr;
    }
    else
    {
        both_rect_ptr->v_rect = *rect_ptr;
    }
}

/*****************************************************************************/
//  Description : check the rect by direction
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN GUI_CheckRectDirection(
                                      const GUI_RECT_T *total_ptr, //in:out, 总rect
                                      GUI_RECT_T       *rect_ptr   //in:out
                                      )
{
    BOOLEAN     result = FALSE;

    if (MMI_DIRECTION_RIGHT == MMITHEME_GetDisDirection())
    {
        int16 temp_left = rect_ptr->left;

        if ( PNULL == total_ptr )
        {
            rect_ptr->left  = (int16)(MMITHEME_GetMainScreenRightPixel() - rect_ptr->right);
            rect_ptr->right = (int16)(MMITHEME_GetMainScreenRightPixel() - temp_left);
        }
        else
        {
            rect_ptr->left = total_ptr->left + total_ptr->right - rect_ptr->right;
            rect_ptr->right = total_ptr->left + total_ptr->right - temp_left;
        }

        result = TRUE;
    }

    return (result);
}

/*******************************************************************************************/
// 	Description : 取两个区域的合集
//	Global resource dependence :
//  Author: xiaoqing.lu
//	Note:
/*********************************************************************************************/
PUBLIC GUI_RECT_T GUI_AddupRect(
                                GUI_RECT_T *rect1_ptr,
                                GUI_RECT_T *rect2_ptr
                                )
{
    GUI_RECT_T rect = {0};
    
    if (PNULL != rect1_ptr && PNULL != rect2_ptr)
    {
        rect.left = MIN(rect1_ptr->left, rect2_ptr->left);
        rect.top = MIN(rect1_ptr->top, rect2_ptr->top);
        rect.right = MAX(rect1_ptr->right, rect2_ptr->right);
        rect.bottom = MAX(rect1_ptr->bottom, rect2_ptr->bottom);
    }
    else if (PNULL != rect1_ptr && PNULL == rect2_ptr)
    {
        rect = *rect1_ptr;
    }
    else if (PNULL == rect1_ptr && PNULL != rect2_ptr)
    {
        rect = *rect2_ptr;
    }
    else
    {
        rect = rect;
    }    

    return rect;
}

/*****************************************************************************/
//  Description : display sheen
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC GUI_SHEEN_T* GUI_DisplaySheen(
                                     GUI_SHEEN_INPUT_T  *sheen_input_ptr  //in:
                                     )
{
    uint16              i = 0;
    uint16              j = 0;
    uint16              rect_width = 0;
    uint16              rect_height = 0;
    GUI_RECT_T          show_rect = {0};
    GUI_SHEEN_T         *sheen_ptr = PNULL;
    GUILCD_DRAW_DC_T    draw_dc = {0};

    if ((PNULL != sheen_input_ptr) &&
        (IsValidRect(sheen_input_ptr->rect)))
    {
        //get buffer info
        if (LCD_PrepareDrawParam(&sheen_input_ptr->lcd_dev,&draw_dc))
        {
            //set show rect
            show_rect.right  = (uint16)(draw_dc.width - 1);
            show_rect.bottom = (uint16)(draw_dc.height - 1);

            //convert rect
            LCD_ConvertDispRect(GUILCD_CONVERT_POS_LCD2DISP,&sheen_input_ptr->lcd_dev,&sheen_input_ptr->rect,&sheen_input_ptr->rect);

            if (GUI_IntersectRect(&sheen_input_ptr->rect,show_rect,sheen_input_ptr->rect))
            {
                //get rect width and height
                rect_width  = (uint16)(sheen_input_ptr->rect.right - sheen_input_ptr->rect.left + 1);
                rect_height = (uint16)(sheen_input_ptr->rect.bottom - sheen_input_ptr->rect.top + 1);

                //alloc
                sheen_ptr = SCI_ALLOCA(sizeof(GUI_SHEEN_T));
                if ((PNULL != sheen_ptr) &&
                    (PNULL != draw_dc.lcd_buffer_ptr))
                {
                    //set dest buffer
                    sheen_ptr->dest_buf_width = draw_dc.mem_width;
                    sheen_ptr->dest_buf_ptr   = draw_dc.lcd_buffer_ptr;

#ifdef UI_MULTILAYER_SUPPORT
                    if (DATA_TYPE_ARGB888 == UILAYER_GetLayerColorType(&sheen_input_ptr->lcd_dev))
                    {
                        sheen_ptr->is_pixel_bit16 = FALSE;

                        //alloc copy buffer
                        sheen_ptr->copy_buf_ptr = SCI_ALLOCA(rect_width*rect_height*sizeof(uint32));
                        if (PNULL != sheen_ptr->copy_buf_ptr)
                        {
                            uint32  *copy_buf_ptr = sheen_ptr->copy_buf_ptr;
                            uint32  *dest_buf_ptr = sheen_ptr->dest_buf_ptr;

                            //copy src buffer
                            for (i=0; i<rect_height; i++)
                            {
                                for (j=0; j<rect_width; j++)
                                {
                                    *(copy_buf_ptr + i*rect_width + j) 
                                        = *(dest_buf_ptr + (sheen_input_ptr->rect.top + i)*(sheen_ptr->dest_buf_width) + (sheen_input_ptr->rect.left + j));
                                }
                            }

                            //set input param
                            sheen_ptr->input = *sheen_input_ptr;

                            //display sheen
                            DisplaySheen(sheen_ptr);
                        }
                        else
                        {
                            SCI_FREE(sheen_ptr);
                        }
                    }
                    else
#endif
                    {
                        sheen_ptr->is_pixel_bit16 = TRUE;

                        //alloc copy buffer
                        sheen_ptr->copy_buf_ptr = SCI_ALLOCA(rect_width*rect_height*sizeof(GUI_COLOR_T));
                        if (PNULL != sheen_ptr->copy_buf_ptr)
                        {
                            GUI_COLOR_T *copy_buf_ptr = sheen_ptr->copy_buf_ptr;
                            GUI_COLOR_T *dest_buf_ptr = sheen_ptr->dest_buf_ptr;

                            //copy src buffer
                            for (i=0; i<rect_height; i++)
                            {
                                for (j=0; j<rect_width; j++)
                                {
                                    *(copy_buf_ptr + i*rect_width + j) 
                                        = *(dest_buf_ptr + (sheen_input_ptr->rect.top + i)*(sheen_ptr->dest_buf_width) + (sheen_input_ptr->rect.left + j));
                                }
                            }

                            //set input param
                            sheen_ptr->input = *sheen_input_ptr;

                            //display sheen
                            DisplaySheen(sheen_ptr);
                        }
                        else
                        {
                            SCI_FREE(sheen_ptr);
                        }
                    }
                }
            }
        }
        else
        {
            //SCI_TRACE_LOW:"GUI_DisplaySheen:get buffer info fail!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUICOMMON_1449_112_2_18_3_15_17_61,(uint8*)"");
        }
    }

    return (sheen_ptr);
}

/*******************************************************************************************/
// 	Description : is rect valid
//	Global resource dependence :
//  Author: Jassmine
//	Note:   
/*********************************************************************************************/
LOCAL BOOLEAN IsValidRect( 
                          GUI_RECT_T    rect
                          )
{
    BOOLEAN     result = FALSE;

    if ((rect.left <= rect.right) &&
        (rect.top <= rect.bottom))
    {
        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : display sheen
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void DisplaySheen(
                        GUI_SHEEN_T     *sheen_ptr
                        )
{
    BOOLEAN             is_add = TRUE;
    GUI_RECT_T          sheen_rect = {0};
    GUI_RECT_T          lcd_rect = {0};
    GUI_SHEEN_DEPTH_E   i = GUI_SHEEN_DEPTH_0;

    if ((PNULL != sheen_ptr) &&
        (PNULL != sheen_ptr->copy_buf_ptr))
    {
        //根据边际和中间深度的设置,来决定显示特效的方向
        if (sheen_ptr->input.side_depth > sheen_ptr->input.center_depth)
        {
            is_add = FALSE;
        }

        //set sheen rect
        sheen_rect = sheen_ptr->input.rect;
        
        //display sheen rect
        for (i=sheen_ptr->input.side_depth; (is_add ? i<=sheen_ptr->input.center_depth : i>=sheen_ptr->input.center_depth); (is_add ? i++ : i--))
        {
            if (IsValidRect(sheen_rect))
            {
                //display sheen rect
                DisplaySheenRect(&sheen_rect,
                    sheen_ptr,
                    i);

                //modify sheen rect
                sheen_rect.left++;
                sheen_rect.top++;
                sheen_rect.right--;
                sheen_rect.bottom--;
            }
            else
            {
                break;
            }
        }

        //fill center rect
        while (IsValidRect(sheen_rect))
        {
            //display sheen rect
            DisplaySheenRect(&sheen_rect,
                sheen_ptr,
                sheen_ptr->input.center_depth);

            //modify sheen rect
            sheen_rect.left++;
            sheen_rect.top++;
            sheen_rect.right--;
            sheen_rect.bottom--;
        }

        LCD_ConvertDispRect(GUILCD_CONVERT_POS_DISP2LCD,&sheen_ptr->input.lcd_dev,&sheen_ptr->input.rect,&lcd_rect);
        GUILCD_InvalidateRect(sheen_ptr->input.lcd_dev.lcd_id,
            lcd_rect,
            0);
    }
}

/*****************************************************************************/
//  Description : display sheen rect,width = 1
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void DisplaySheenRect(
                            GUI_RECT_T          *rect_ptr,  //in:
                            GUI_SHEEN_T         *sheen_ptr, //in/out:
                            GUI_SHEEN_DEPTH_E   depth       //in:
                            )
{
    int16   i=0;
    uint16  copy_width = 0;
    uint16  copy_height = 0;
    
    //get copy width and height
    copy_width  = (uint16)(sheen_ptr->input.rect.right - sheen_ptr->input.rect.left + 1);
    copy_height = (uint16)(sheen_ptr->input.rect.bottom - sheen_ptr->input.rect.top + 1);

    if (sheen_ptr->is_pixel_bit16)
    {
        GUI_COLOR_T *copy_buf_ptr = sheen_ptr->copy_buf_ptr;
        GUI_COLOR_T *dest_buf_ptr = sheen_ptr->dest_buf_ptr;

        //left
        for (i=rect_ptr->top; i<=rect_ptr->bottom; i++)
        {
            TranslateSheenPixel(*(copy_buf_ptr + (i - sheen_ptr->input.rect.top)*copy_width + (rect_ptr->left - sheen_ptr->input.rect.left)),
                dest_buf_ptr + i*sheen_ptr->dest_buf_width + rect_ptr->left,
                depth);
        }

        //right
        for (i=rect_ptr->top; i<=rect_ptr->bottom; i++)
        {
            TranslateSheenPixel(*(copy_buf_ptr + (i - sheen_ptr->input.rect.top)*copy_width + (rect_ptr->right - sheen_ptr->input.rect.left)),
                dest_buf_ptr + i*sheen_ptr->dest_buf_width + rect_ptr->right,
                depth);
        }
    
        //top
        for (i=rect_ptr->left; i<=rect_ptr->right; i++)
        {
            TranslateSheenPixel(*(copy_buf_ptr + (rect_ptr->top - sheen_ptr->input.rect.top)*copy_width + (i - sheen_ptr->input.rect.left)),
                dest_buf_ptr + rect_ptr->top*sheen_ptr->dest_buf_width + i,
                depth);
        }

        //bottom
        for (i=rect_ptr->left; i<=rect_ptr->right; i++)
        {
            TranslateSheenPixel(*(copy_buf_ptr + (rect_ptr->bottom - sheen_ptr->input.rect.top)*copy_width + (i - sheen_ptr->input.rect.left)),
                dest_buf_ptr + rect_ptr->bottom*sheen_ptr->dest_buf_width + i,
                depth);
        }
    }
    else
    {
        uint32  *copy_buf_ptr = sheen_ptr->copy_buf_ptr;
        uint32  *dest_buf_ptr = sheen_ptr->dest_buf_ptr;

        //left
        for (i=rect_ptr->top; i<=rect_ptr->bottom; i++)
        {
            TranslateSheenPixel32(*(copy_buf_ptr + (i - sheen_ptr->input.rect.top)*copy_width + (rect_ptr->left - sheen_ptr->input.rect.left)),
                dest_buf_ptr + i*sheen_ptr->dest_buf_width + rect_ptr->left,
                depth);
        }

        //right
        for (i=rect_ptr->top; i<=rect_ptr->bottom; i++)
        {
            TranslateSheenPixel32(*(copy_buf_ptr + (i - sheen_ptr->input.rect.top)*copy_width + (rect_ptr->right - sheen_ptr->input.rect.left)),
                dest_buf_ptr + i*sheen_ptr->dest_buf_width + rect_ptr->right,
                depth);
        }
    
        //top
        for (i=rect_ptr->left; i<=rect_ptr->right; i++)
        {
            TranslateSheenPixel32(*(copy_buf_ptr + (rect_ptr->top - sheen_ptr->input.rect.top)*copy_width + (i - sheen_ptr->input.rect.left)),
                dest_buf_ptr + rect_ptr->top*sheen_ptr->dest_buf_width + i,
                depth);
        }

        //bottom
        for (i=rect_ptr->left; i<=rect_ptr->right; i++)
        {
            TranslateSheenPixel32(*(copy_buf_ptr + (rect_ptr->bottom - sheen_ptr->input.rect.top)*copy_width + (i - sheen_ptr->input.rect.left)),
                dest_buf_ptr + rect_ptr->bottom*sheen_ptr->dest_buf_width + i,
                depth);
        }
    }
}

/*****************************************************************************/
//  Description : translate every pixel sheen effect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void TranslateSheenPixel(
                               GUI_COLOR_T          src_pixel,          //in:
                               GUI_COLOR_T          *sheen_pixel_ptr,   //in/out:
                               GUI_SHEEN_DEPTH_E    depth               //in:
                               )
{
    uint16  left_value = 0;
    uint16  mask1p16 = 0x0861;
    uint16  mask1p8 = 0x18e3;
    uint16  mask1p4 = 0x39e7;
    uint16  mask1p2 = 0x7bef;

    left_value = (uint16)(0xffff - src_pixel);
    switch(depth)
    {               
    case GUI_SHEEN_DEPTH_0:
        left_value = (uint16)(((left_value>>1)&mask1p2) + ((left_value>>2)&mask1p4));
        break;

    case GUI_SHEEN_DEPTH_1:
        left_value = (uint16)(((left_value>>1)&mask1p2) + ((left_value>>3)&mask1p8) + ((left_value>>4)&mask1p16));
        break;

    case GUI_SHEEN_DEPTH_2:
        left_value = (uint16)(((left_value>>1)&mask1p2) + ((left_value>>3)&mask1p8));
        break;

    case GUI_SHEEN_DEPTH_3:
        left_value = (uint16)(((left_value>>1)&mask1p2) + ((left_value>>4)&mask1p16));
        break;

    case GUI_SHEEN_DEPTH_4:
        left_value = (uint16)(((left_value>>1)&mask1p2));
        break;

    case GUI_SHEEN_DEPTH_5:
        left_value = (uint16)(((left_value>>2)&mask1p4) + ((left_value>>3)&mask1p8) + ((left_value>>4)&mask1p16));
        break;

    case GUI_SHEEN_DEPTH_6:
        left_value = (uint16)(((left_value>>2)&mask1p4) + ((left_value>>3)&mask1p8));
        break;

    case GUI_SHEEN_DEPTH_7:
        left_value = (uint16)(((left_value>>2)&mask1p4) + ((left_value>>4)&mask1p16));
        break;

    case GUI_SHEEN_DEPTH_8:
        left_value = (uint16)(((left_value>>2)&mask1p4));
        break;

    case GUI_SHEEN_DEPTH_9:
        left_value = (uint16)(((left_value>>3)&mask1p8) + ((left_value>>4)&mask1p16));
        break;

//    case GUI_SHEEN_DEPTH_10:
//        left_value = ((left_value>>3)&mask1p8);
//        break;
//
//    case GUI_SHEEN_DEPTH_11:
//        left_value = ((left_value>>4)&mask1p16);
//        break;

    default:
        break;
    }
    
    *(uint16*)(sheen_pixel_ptr) = (uint16)(left_value + src_pixel);
}

/*****************************************************************************/
//  Description : translate every pixel sheen effect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void TranslateSheenPixel32(
                                 uint32             src_pixel,          //in:
                                 uint32             *sheen_pixel_ptr,   //in/out:
                                 GUI_SHEEN_DEPTH_E  depth               //in:
                                 )
{
    uint32 left_value = 0;
    uint32 mask1p16 = 0x0f0f0f;
    uint32 mask1p8 = 0x1f1f1f;
    uint32 mask1p4 = 0x3f3f3f;
    uint32 mask1p2 = 0x7f7f7f;

    left_value = (0xffffff - (src_pixel & 0xffffff));
    
    switch(depth)
    {               
    case GUI_SHEEN_DEPTH_0:
        left_value = (((left_value>>1)&mask1p2) + ((left_value>>2)&mask1p4));
        break;

    case GUI_SHEEN_DEPTH_1:
        left_value = (((left_value>>1)&mask1p2) + ((left_value>>3)&mask1p8) + ((left_value>>4)&mask1p16));
        break;

    case GUI_SHEEN_DEPTH_2:
        left_value = (((left_value>>1)&mask1p2) + ((left_value>>3)&mask1p8));
        break;

    case GUI_SHEEN_DEPTH_3:
        left_value = (((left_value>>1)&mask1p2) + ((left_value>>4)&mask1p16));
        break;

    case GUI_SHEEN_DEPTH_4:
        left_value = (((left_value>>1)&mask1p2));
        break;

    case GUI_SHEEN_DEPTH_5:
        left_value = (((left_value>>2)&mask1p4) + ((left_value>>3)&mask1p8) + ((left_value>>4)&mask1p16));
        break;

    case GUI_SHEEN_DEPTH_6:
        left_value = (((left_value>>2)&mask1p4) + ((left_value>>3)&mask1p8));
        break;

    case GUI_SHEEN_DEPTH_7:
        left_value = (((left_value>>2)&mask1p4) + ((left_value>>4)&mask1p16));
        break;

    case GUI_SHEEN_DEPTH_8:
        left_value = (((left_value>>2)&mask1p4));
        break;

    case GUI_SHEEN_DEPTH_9:
        left_value = (((left_value>>3)&mask1p8) + ((left_value>>4)&mask1p16));
        break;

    default:
        break;
    }
    
    *(uint32*)(sheen_pixel_ptr) = (left_value + (src_pixel & 0xffffff)) + (src_pixel & 0xff000000);
}

/*****************************************************************************/
//  Description : release sheen data
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void GUI_ReleaseSheen(
                             GUI_SHEEN_T    *sheen_ptr, //in
                             BOOLEAN        is_recover  //in:
                             )
{
    if (PNULL != sheen_ptr)
    {
        if (is_recover)
        {
            RecoverSheenRect(sheen_ptr);
        }

        //free memory
        if (PNULL != sheen_ptr->copy_buf_ptr)
        {
            SCI_FREE(sheen_ptr->copy_buf_ptr);
        }
        SCI_FREE(sheen_ptr);
    }
}

/*****************************************************************************/
//  Description : recover sheen rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void RecoverSheenRect(
                            GUI_SHEEN_T *sheen_ptr
                            )
{
    uint16          i = 0;
    uint16          j = 0;
    uint16          rect_width = 0;
    uint16          rect_height = 0;
    GUI_RECT_T      lcd_rect = {0};

    if ((PNULL != sheen_ptr) &&
        (PNULL != sheen_ptr->dest_buf_ptr) &&
        (IsValidRect(sheen_ptr->input.rect)))
    {
        //get rect width and height
        rect_width  = (uint16)(sheen_ptr->input.rect.right - sheen_ptr->input.rect.left + 1);
        rect_height = (uint16)(sheen_ptr->input.rect.bottom - sheen_ptr->input.rect.top + 1);

        if (sheen_ptr->is_pixel_bit16)
        {
            GUI_COLOR_T *copy_buf_ptr = sheen_ptr->copy_buf_ptr;
            GUI_COLOR_T *dest_buf_ptr = sheen_ptr->dest_buf_ptr;

            //recover src buffer
            for (i=0; i<rect_height; i++)
            {
                for (j=0; j<rect_width; j++)
                {
                    *(dest_buf_ptr + (sheen_ptr->input.rect.top + i)*sheen_ptr->dest_buf_width + (sheen_ptr->input.rect.left + j)) 
                        = *(copy_buf_ptr + i*rect_width + j);
                }
            }
        }
        else
        {
            uint32  *copy_buf_ptr = sheen_ptr->copy_buf_ptr;
            uint32  *dest_buf_ptr = sheen_ptr->dest_buf_ptr;

            //recover src buffer
            for (i=0; i<rect_height; i++)
            {
                for (j=0; j<rect_width; j++)
                {
                    *(dest_buf_ptr + (sheen_ptr->input.rect.top + i)*sheen_ptr->dest_buf_width + (sheen_ptr->input.rect.left + j)) 
                        = *(copy_buf_ptr + i*rect_width + j);
                }
            }
        }

        LCD_ConvertDispRect(GUILCD_CONVERT_POS_DISP2LCD,&sheen_ptr->input.lcd_dev,&sheen_ptr->input.rect,&lcd_rect);
        GUILCD_InvalidateRect(sheen_ptr->input.lcd_dev.lcd_id,
            lcd_rect,
            0);
    }
}

/*****************************************************************************/
//  Description : 开始计时
//  Global resource dependence : 
//  Author: hua.fang
//  Note: 
/*****************************************************************************/
PUBLIC void GUI_StartTiming(void)
{
    s_cur_tick_count = SCI_GetTickCount();

    //SCI_TRACE_LOW:"== GUICOM TIMING START == %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUICOMMON_1872_112_2_18_3_15_18_62,(uint8*)"d", s_cur_tick_count);
}

/*****************************************************************************/
//  Description : 打印计时
//  Global resource dependence : 
//  Author: hua.fang
//  Note: 
/*****************************************************************************/
PUBLIC void GUI_PrintTiming(
                            char    *tips
                            )
{
    uint32 pre_tick_count = s_cur_tick_count;

    s_cur_tick_count = SCI_GetTickCount();

    //SCI_TRACE_LOW:"== GUICOM TIMING ELAPSE == %s: %d, %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUICOMMON_1887_112_2_18_3_15_18_63,(uint8*)"sdd", tips, s_cur_tick_count - pre_tick_count, s_cur_tick_count);
}

/*****************************************************************************/
//  Description : dump memory data
//  Global resource dependence : 
//  Author: hua.fang
//  Note: 
// 打印内存数据，以16进制表示
// dump_len: 表示一行打印的个数
// dump_num: 表示总共要打印多少行
/*****************************************************************************/
PUBLIC void GUI_DumpMemData(
                            uint32  *data_ptr,
                            uint16  dump_len,
                            uint16  dump_num
                            )
{
    char*   data_str_ptr = PNULL;
    char*   temp_str_ptr = PNULL;
    uint16  i = 0;
    uint16  j = 0;
    int32   count = 0;

    if (PNULL == data_ptr || 0 == dump_len || 0 == dump_num)
    {
        return;
    }

    data_str_ptr = SCI_ALLOC(dump_len*9+1);

    if (PNULL != data_str_ptr)
    {
        SCI_MEMSET(data_str_ptr, 0, dump_len*9+1);

        for (i = 0; i < dump_num; i++)
        {
            temp_str_ptr = data_str_ptr;

            for (j = 0; j < dump_len; j++)
            {
                sprintf(temp_str_ptr, "%08x ", *(data_ptr + count));
                count++;
                temp_str_ptr += 9;
            }

            SCI_TRACE_LOW("%s", data_str_ptr);
        }

        SCI_FREE(data_str_ptr);
    }
}

/*****************************************************************************/
//  Description : set alpha overwrite
//  Global resource dependence : 
//  Author: James.Zhang
//  Note: 在带alpha的图层上绘制时,true为覆盖,否则为合成. 返回值为old值
//        若无背景,可设为覆盖,能够提高图片文字的渲染速度
/*****************************************************************************/
PUBLIC BOOLEAN GUI_SetAlphaOverwrite(
                                     BOOLEAN is_overwrite
                                     )
{
    BOOLEAN old = s_is_alpha_overwrite;

    s_is_alpha_overwrite = is_overwrite;

    return old;
}

/*****************************************************************************/
//  Description : is alpha overwrite
//  Global resource dependence : 
//  Author: James.Zhang
//  Note: 在带alpha的图层上绘制时,true为覆盖,否则为合成.
//        若无背景,可设为覆盖,能够提高图片文字的渲染速度
/*****************************************************************************/
PUBLIC BOOLEAN GUI_IsAlphaOverwrite(
                                    void
                                    )
{
    return s_is_alpha_overwrite;
}
#if defined(UI_MULTILAYER_SUPPORT) && !defined(UI_LAYER_SUPPORT_LOW_MEMORY) 
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: ying.xu
//  Note:
/*****************************************************************************/
PUBLIC void GUI_WeekLayer(
                          GUI_LCD_DEV_INFO *layer_ptr, 
                          GUI_RECT_T *rect_ptr, 
                          uint32 alpha
                          )
{
    UILAYER_INFO_T    layer_info = {0};
    uint8 *layer_buf_ptr = PNULL;
    uint32 *bk_buf_ptr = PNULL;    
    GUI_RECT_T mask_rect = {0};
    GUI_RECT_T layer_rect = {0};
    uint32 mask_width    = 0;
    uint32 mask_height   = 0;
    uint32 color = 0;
    uint32 i = 0;
    uint32 j = 0;
    
    UILAYER_GetLayerInfo(layer_ptr, &layer_info);
    layer_buf_ptr = UILAYER_GetLayerBufferPtr(layer_ptr);
    
    if (PNULL == layer_buf_ptr/*layer_info.layer_buf_ptr*/)
    {
        return;
    }    
	
    if(layer_info.data_type != DATA_TYPE_ARGB888)
    {
    	return;
    }
	
    layer_rect = UILAYER_GetLayerRect(layer_ptr);       
    
    if (PNULL == rect_ptr)
    {
        mask_rect  = layer_rect;
    }
    else
    {
        if (!GUI_IntersectRect(&mask_rect, *rect_ptr, layer_rect))
        {
            return;
        }
    }     
    
    LCD_ConvertDispRect(GUILCD_CONVERT_POS_LCD2DISP , layer_ptr, &mask_rect, &mask_rect);
    mask_height = mask_rect.bottom - mask_rect.top ;
    mask_width  = mask_rect.right - mask_rect.left ;
    
    bk_buf_ptr = ((uint32*)layer_buf_ptr) + mask_rect.top * layer_info.mem_width + mask_rect.left; 
    
    for (i = 0; i < mask_height; i++)
    {    
        for (j = 0; j < mask_width; j++)
        {         
            color = *(bk_buf_ptr + j);
            //color &=  0xffffff;
            *(bk_buf_ptr + j) = (color)?\
                (((((color >> 24) * alpha) >> 8) << 24) | (color & 0xffffff)) : color ; 
        }
        
        bk_buf_ptr += layer_info.mem_width;
    }   
}
#endif