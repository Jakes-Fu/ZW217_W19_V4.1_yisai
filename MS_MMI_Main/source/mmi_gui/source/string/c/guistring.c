/*****************************************************************************
** File Name:      guistring.c                                               *
** Author:                                                                   *
** Date:           07/24/2009                                                *
** Copyright:      2009 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe call log                    *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 07/2009        xiaoqing.lu       Create                                   *
******************************************************************************/
#ifndef _GUI_STRING_C_
#define _GUI_STRING_C_
#ifndef SCRIPTEASY_SUPPORT
/**--------------------------------------------------------------------------*/
/**                         Include Files                                    */
/**--------------------------------------------------------------------------*/
#include "mmi_gui_trc.h"
#include "guilcd.h"
#include "guicommon.h"
#include "guires.h"
#include "mmi_default.h"
#include "guistring.h"
#include "guifont.h"
#include "spml_api.h"
#include "mmi_theme.h"
#include "mmi_autotest.h"
#include "guigraph.h"
#include "mmi_string.h"

/**--------------------------------------------------------------------------*/
/**                         MACRO DEFINITION                                 */
/**--------------------------------------------------------------------------*/
#if defined MMI_GUI_STYLE_MINISCREEN ||defined FONT_TYPE_SUPPORT_NONE
    #define USE_STRING_ELLIPSIS
#endif
/*---------------------------------------------------------------------------*/
/*                          CONSTANT ARRAY                                   */
/*---------------------------------------------------------------------------*/
//建立双向链表来构造临时特效显示结点
typedef struct _region_node_tag GUISTRING_REGION_NODE_T;
struct _region_node_tag
{
    GUISTRING_REGION_NODE_T *prev;
    GUISTRING_REGION_NODE_T *next;
    GUIFONT_REGION_STYLE_T region;
};

//下面是双向链表的常规操作，insert、append和remove
LOCAL __inline void InsertNodePtr(
    GUISTRING_REGION_NODE_T *new_ptr,
    GUISTRING_REGION_NODE_T *org_ptr
)
{
    if (PNULL != new_ptr && PNULL != org_ptr)
    {
        if (PNULL != org_ptr->prev) org_ptr->prev->next = new_ptr;
        
        new_ptr->prev = org_ptr->prev;
        new_ptr->next = org_ptr;
        
        org_ptr->prev = new_ptr;
    }

    return;
}

LOCAL __inline void AppendNodePtr(
    GUISTRING_REGION_NODE_T *org_ptr,
    GUISTRING_REGION_NODE_T *new_ptr
)
{
    if(PNULL != new_ptr && PNULL != org_ptr)
    {
        if (PNULL != org_ptr->next) org_ptr->next->prev = new_ptr;
        
        new_ptr->prev = org_ptr;
        new_ptr->next = org_ptr->next;
        
        org_ptr->next = new_ptr;
    }

    return;
}

LOCAL __inline void RemoveNodePtr(GUISTRING_REGION_NODE_T *node_ptr)
{
    if(PNULL != node_ptr)
    {
        if (PNULL != node_ptr->prev) node_ptr->prev->next = node_ptr->next;
        if (PNULL != node_ptr->next) node_ptr->next->prev = node_ptr->prev;

        SCI_FREE(node_ptr);
    }

    return;
}



/**--------------------------------------------------------------------------*/
/**                         EXTERNAL DECLARE                                 */
/**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*/
/**                         STATIC DEFINITION                                */
/**--------------------------------------------------------------------------*/

/*******************************************************************************************/
// 	Description : calculate the x position of new line
//	Global resource dependence :
//  Author:xiaoqing.lu
//	Note:
/*********************************************************************************************/
// LOCAL int16 GetXPosition(
//                          GUI_RECT_T  rect, 		    //the fixed display area
//                          GUI_ALIGN_E align,
//                          uint16      pixel_num
//                          );

/*******************************************************************************************/
// 	Description : 检查区域是否合法
//	Global resource dependence :
//  Author: xiaoqing.lu 
//	Note:
/*********************************************************************************************/
LOCAL BOOLEAN CheckRectValid(
                             GUI_RECT_T rect
                             );

/*******************************************************************************************/
// 	Description : 该函数用于断行
//	Global resource dependence :
//  Author: James.Zhang
//	Note: 传入文字的方向, 在有双向文字时断行才能更加准确
/*********************************************************************************************/
LOCAL BOOLEAN GetStringInfoInLineWithDir(
                                         const GUISTR_STYLE_T   *style_ptr,    
                                         const wchar     *str_ptr,
                                         uint16          length,
                                         GUISTR_STATE_T         state,
                                         uint16      		    width,
                                         uint16			        *char_num,
                                         uint16			        *pixel_num,
                                         BOOLEAN          is_byword
                                         );

/*******************************************************************************************/
// 	Description : calculate the ucs2 string by piexl number
//	Global resource dependence :
//  Author: bin.ji
//	Note:
/*********************************************************************************************/
LOCAL uint16 CalculateStringLinesByPixelNum(
                                            uint16    pixelnum,    //[IN] 像素个数
                                            const GUISTR_STYLE_T *style_ptr,    
                                            const wchar* str_ptr,  //[IN] String指针
                                            uint16     str_len,    //[IN] String长度(Byte长度)
                                            GUISTR_STATE_T       state
                                            );

/*******************************************************************************************/
// 	Description : calculate the y position of new line
//	Global resource dependence :
//  Author:xiaoqing.lu
//	Note:
/*********************************************************************************************/
LOCAL int16 GetYPosition(
                         GUI_RECT_T  rect,         // the fixed display area
                         GUISTR_STYLE_T     *style_ptr,
                         const MMI_STRING_T *str_ptr,
                         GUISTR_STATE_T     state,
                         uint16      font_height,   // space between two chars.
                         BOOLEAN     is_single_line // 
                         );

/*****************************************************************************/
//  Description : convert font effect
//  Global resource dependence : 
//  Author:James.zhang
//  Note:
/*****************************************************************************/
LOCAL void ConvertFontEffect( 
                             GUIFONT_EFFECT_T    effect,
                             GUI_COLOR_T 	     color,
                             GUI_COLOR_T         edge_color,
                             SPML_TEXT_PARAM_T   *param //out
                             );

/*****************************************************************************/
// 	Description : 计算剪切区域, 得到两个区域的共同区域
//	Global resource dependence : none
//  Author: xiaoqing.lu
//	Note: 
/*****************************************************************************/
LOCAL GUI_RECT_T GetTrimRect(
                             GUI_RECT_T rect1, 
                             GUI_RECT_T rect2
                             );

/*****************************************************************************/
// 	Description : 获取文本的方向
//	Global resource dependence : none
//  Author: James.zhang
//	Note: 
/*****************************************************************************/
LOCAL SPML_TEXT_DIR_T GetNextLineTextDir( 
                                         SPML_TEXT_DIR_T dir, //当前行的文字方向
                                         const wchar   *text_ptr,
                                         uint16        len,
                                         uint16        pos
                                         );

/*****************************************************************************/
// 	Description : 显示字符串
//	Global resource dependence : none
//  Author: xiaoqing.lu
//	Note: 
/*****************************************************************************/
LOCAL GUI_RECT_T DrawTextInRect(
                                GUISTR_STYLE_T     *style_ptr,
                                GUISTR_BUF_T       *des_buf_ptr,
                                const GUI_RECT_T   *disp_rect_ptr,       //the fixed display area
                                const GUI_RECT_T   *clip_rect_ptr,       //用户要剪切的实际区域
                                const MMI_STRING_T *str_ptr,
                                GUISTR_STATE_T     state,
                                int8               dir
                                );

/*****************************************************************************/
// 	Description : 显示字符串
//	Global resource dependence : none
//  Author: xiaoqing.lu
//	Note: 
/*****************************************************************************/
LOCAL GUI_RECT_T DrawTextInLine(
                                GUISTR_STYLE_T     *style_ptr,
                                GUISTR_BUF_T       *des_buf_ptr,
                                const GUI_RECT_T   *disp_rect_ptr,       //the fixed display area
                                const GUI_RECT_T   *clip_rect_ptr,       //用户要剪切的实际区域
                                const MMI_STRING_T *str_ptr,
                                GUISTR_STATE_T     state,
                                int8               dir,
                                BOOLEAN            is_change_align       //是否需要根据dir改变alignment
                                );

/*****************************************************************************/
// 	Description : 根据偏移量，计算剩余字符串的特殊效果字段数
//	Global resource dependence : none
//  Author: xiaoqing.lu
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN CreateRegionStyle(
                               const MMI_STRING_T *str_ptr, 
                               GUISTR_STYLE_T *style_ptr, 
                               uint16 offset,
                               GUIFONT_REGION_STYLE_T **region_style_pptr, 
                               uint16 *region_num_ptr
                               );

/*****************************************************************************/
// 	Description : 根据原始区域坐标和旋转角度，计算旋转后的坐标
//	Global resource dependence : none
//  Author: xiaoqing.lu
//	Note: 起始点的位置相对于buffer的原点
/*****************************************************************************/
LOCAL BOOLEAN CalculateRotateRect(
                                  const GUI_RECT_T *orig_rect_ptr,  // 原始区域坐标
                                  int16 x_offset,             // 坐标原点在x方向上的位移
                                  int16 y_offset,             // 坐标原点在y方向上的位移
                                  GUI_ANGLE_E angle,          // 旋转角度
                                  GUI_RECT_T *result_rect_ptr // 输出参数
                                  );

/*****************************************************************************/
// 	Description : copy buffer, 原始buffer和目标buffer的大小一定相同，否则会出现异常
//	Global resource dependence : none
//  Author: xiaoqing.lu
//	Note: 按顺时针方向旋转
/*****************************************************************************/
LOCAL BOOLEAN RotateBufferData(
                               GUISTR_BUF_TYPE_T type,
                               const void *src_data_ptr, 
                               int16  src_x,      // 原始起始点x坐标
                               int16  src_y,      // 原始起始点y坐标
                               uint16 src_width,  // 原始buffer的宽度
                               uint16 src_heigth, // 原始buffer的高度
                               void *dst_data_ptr,
                               int16  dst_x,      // 目标起始点x坐标
                               int16  dst_y,      // 目标起始点y坐标
                               uint16 dst_width,  // 目标buffer的宽度
                               uint16 dst_heigth, // 目标buffer的高度
                               GUI_ANGLE_E angle,  // 旋转角度
                               uint16 cp_width,  // 被copy的宽度
                               uint16 cp_heigth // 被copy的高度
                               );

/*******************************************************************************************/
// 	Description : 转化对齐方式
//	Global resource dependence :
//  Author: xiaoqing.lu
//	Note:
/*********************************************************************************************/
LOCAL SPML_TEXT_FORMAT_T ConvertAlignment(
                                          GUISTR_STATE_T state,
                                          int8           dir,
                                          BOOLEAN        is_change_align,
                                          GUI_ALIGN_E    align
                                          );

/*****************************************************************************/
// 	Description : 判断状态是否设置
//	Global resource dependence : none
//  Author: xiaoqing.lu
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsSetState(
                         GUISTR_STATE_T src_state, 
                         GUISTR_STATE_T dst_state
                         );

/*****************************************************************************/
// 	Description : 根据传入的文本gui状态，转化成底层显示时能够识别的状态
//	Global resource dependence : none
//  Author: xiaoqing.lu
//	Note: 
/*****************************************************************************/
LOCAL void ConvertTextState(
                            GUISTR_STATE_T     state, 
                            SPML_TEXT_FORMAT_T *format_ptr
                            );

/*****************************************************************************/
// 	Description : 转为spml的buffer类型
//	Global resource dependence : none
//  Author: James.Zhang
//	Note: 
/*****************************************************************************/
LOCAL SPML_BUF_TYPE_T ConvertSpmlBufferType(
                                            GUISTR_BUF_TYPE_T type
                                            );

/*****************************************************************************/
// 	Description : 转为string的buffer类型
//	Global resource dependence : none
//  Author: James.Zhang
//	Note: 
/*****************************************************************************/
LOCAL GUISTR_BUF_TYPE_T ConvertStringBufferType(
                                                DISPLAY_DATA_TYPE_E type
                                                );

/*******************************************************************************************/
// 	Description : 计算节点数，创建数组
//	Global resource dependence :
//  Author: xiaoqing.lu
//	Note: 
/*********************************************************************************************/
LOCAL void DestoryPtr(
                      void *node_ptr
                      );

#if 0
/*******************************************************************************************/
// 	Description : 计算节点数，创建数组
//	Global resource dependence :
//  Author: xiaoqing.lu
//	Note: 
/*********************************************************************************************/
LOCAL int32 CreateNodeNumberPtr(
                                 GUIFONT_REGION_STYLE_T *region_ptr, 
                                 int32 region_num,
                                 int32 **node_ptr
                                 );

/*******************************************************************************************/
// 	Description : 对数组进行排序
//	Global resource dependence :
//  Author: xiaoqing.lu
//	Note: 
/*********************************************************************************************/
LOCAL void BubbleSortNumberPtr(
                               int32 *node_ptr,
                               int32 node_num
                               );
/*******************************************************************************************/
// 	Description : 删除相同的节点
//	Global resource dependence :
//  Author: xiaoqing.lu
//	Note: 
/*********************************************************************************************/
LOCAL int32 DeleteSameNode(
                           int32 *node_ptr,
                           int32 node_num
                           );

/*******************************************************************************************/
// 	Description : 设置特殊区域的值，除了start位置和length长度之外的其他值
//	Global resource dependence :
//  Author: xiaoqing.lu
//	Note: 
/*********************************************************************************************/
LOCAL void SetRegionInfo(
                         GUIFONT_REGION_STYLE_T *in_region_ptr, 
                         int32 region_num,
                         GUIFONT_REGION_STYLE_T *out_region_ptr
                         );
#endif

/*==============================================================================
Description: CombineNode
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
LOCAL int32 CombineNode(GUISTRING_REGION_NODE_T **head_pptr);

/*==============================================================================
Description: InsertNode
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
LOCAL int32 InsertNode(
    GUISTRING_REGION_NODE_T **head_pptr,
    GUIFONT_REGION_STYLE_T const *region_ptr
);

/*******************************************************************************************/
// 	Description : 计算节点数，创建数组
//	Global resource dependence :
//  Author: xiaoqing.lu
//	Note: 
/*********************************************************************************************/
LOCAL int32 CreateRegionPtr(
                            GUIFONT_REGION_STYLE_T const *in_region_ptr, 
                            int32 region_num,
                            GUIFONT_REGION_STYLE_T **out_region_ptr
                            );


/**--------------------------------------------------------------------------*/
/**                         GLOBAL DEFINITION                                */
/**--------------------------------------------------------------------------*/

/*******************************************************************************************/
// 	Description : calculate the x position of new line
//	Global resource dependence :
//  Author:xiaoqing.lu
//	Note:
/*********************************************************************************************/
// LOCAL int16 GetXPosition(
//                          GUI_RECT_T  rect, 		    //the fixed display area
//                          GUI_ALIGN_E align,
//                          uint16      pixel_num
//                          )
// {
//     int16 x = 0;
//   
//     if ((int16)pixel_num < (rect.right - rect.left))
//     {
//         switch (align)
//         {
//         case ALIGN_RIGHT:
//         case ALIGN_RVMIDDLE:
//             if(0 == pixel_num)
//             {
//                 x = (rect.right);
//             }
//             else
//             {
//                 x = (rect.right - (int16)pixel_num + 1);
//             }
//             break;
// 
//         case ALIGN_HMIDDLE:
//         case ALIGN_HVMIDDLE:
//             if(0 == pixel_num)
//             {
//                 x = ((rect.right + rect.left)>>1);
//             }
//             else
//             {
//                 x = ((rect.right + rect.left - (int16)pixel_num + 1)>>1);
//             }
//             break;
// 
//         default:
//             x = rect.left;
//             break;
//         }    
//     }
//     else
//     {
//         x = rect.left;
//     }
// 
//     return x;
// }

LOCAL uint16 GetFontHeight(
                           const GUISTR_STYLE_T *style_ptr,    
                           const MMI_STRING_T   *str_ptr,
                           GUISTR_STATE_T       state
                           )
{
    uint16 height = 0;

    if (GUISTR_STATE_BOLD&state || GUISTR_STATE_UNDERLINE&state)
    {
        GUISTR_INFO_T font_info = {0};
        GUISTR_GetStringInfo(style_ptr, str_ptr, state, &font_info);
        
        height = font_info.height;
    }
    else
    {
        height = GUI_GetStringHeight(style_ptr->font, str_ptr->wstr_ptr, str_ptr->wstr_len);
    }

    return height;
}

/*******************************************************************************************/
// 	Description : 转化对齐方式
//	Global resource dependence :
//  Author: xiaoqing.lu
//	Note:
/*********************************************************************************************/
LOCAL SPML_TEXT_FORMAT_T ConvertAlignment(
                                          GUISTR_STATE_T state,
                                          int8           dir,
                                          BOOLEAN        is_change_align,
                                          GUI_ALIGN_E    align
                                          )
{
    SPML_TEXT_FORMAT_T format = TEXT_FORMAT_NORMAL;
  
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
    case ALIGN_TOP:	                                    // Align on top
        format |= TEXT_FORMAT_TOP;
        break;
    case ALIGN_BOTTOM:                                  // Align at bottom
        format |= TEXT_FORMAT_BOTTOM;
        break;
    case ALIGN_VMIDDLE:	                                // vertical and mediacy
        format |= TEXT_FORMAT_MIDDLE;
        break;
    case ALIGN_LVMIDDLE:                                // left and mediacy
        format |= TEXT_FORMAT_LEFT|TEXT_FORMAT_MIDDLE;
        break;
    case ALIGN_RVMIDDLE:                                // right and mediacy
        format |= TEXT_FORMAT_RIGHT|TEXT_FORMAT_MIDDLE;
        break;
    case ALIGN_HVMIDDLE:
        format |= TEXT_FORMAT_CENTER|TEXT_FORMAT_MIDDLE;
        break;
    default:
        SCI_ASSERT(0);/*assert verified*/
        break;
    }
    
    if ( !IsSetState( state, GUISTR_STATE_SINGLE_LINE ) 
        || IsSetState( state, GUISTR_STATE_ALIGN_BY_DIR ) )
    {
        if ( !IsSetState( state, GUISTR_STATE_SINGLE_LINE )  )
        {
            //多行忽略垂直方向align
            format &= ~(TEXT_FORMAT_MIDDLE|TEXT_FORMAT_BOTTOM);
            
            if ( is_change_align )
            {
                //RTL靠右显示
                if ( TEXT_DIR_RTL == dir )
                {
                    format |= TEXT_FORMAT_RIGHT;
                }
                //否则不靠右显示
                else
                {
                    format &= ~(TEXT_FORMAT_RIGHT);
                }
            }
        }
        else
        {
            //RTL靠右显示
            if ( TEXT_DIR_RTL == dir )
            {
                format |= TEXT_FORMAT_RIGHT;
            }
            //否则不靠右显示
            else
            {
                if ((MMI_DIRECTION_RIGHT == MMITHEME_GetDisDirection()) &&
                    (TEXT_DIR_AUTO == dir) && 
                    (!(format & TEXT_FORMAT_CENTER)))
                {
                    format |= TEXT_FORMAT_RIGHT;
                }
                else
                {
                    format &= ~(TEXT_FORMAT_RIGHT);
                }
            }
        }
    }

    return format;
}

/*******************************************************************************************/
// 	Description : 检查区域是否合法
//	Global resource dependence :
//  Author: xiaoqing.lu 
//	Note:
/*********************************************************************************************/
LOCAL BOOLEAN CheckRectValid(
                             GUI_RECT_T rect
                             )
{
    return (BOOLEAN)(((rect.right  >= rect.left ) && (rect.bottom  >= rect.top)));
}

/*******************************************************************************************/
// 	Description : 该函数用于断行
//	Global resource dependence :
//  Author: James.Zhang
//	Note: 传入文字的方向, 在有双向文字时断行才能更加准确
/*********************************************************************************************/
LOCAL BOOLEAN GetStringInfoInLineWithDir(
                                         const GUISTR_STYLE_T   *style_ptr,    
                                         const wchar     *str_ptr,
                                         uint16          length,
                                         GUISTR_STATE_T         state,
                                         uint16      		    width,
                                         uint16			        *char_num,
                                         uint16			        *pixel_num,
                                         BOOLEAN          is_byword
                                         )
{
    if( (PNULL == style_ptr) || (PNULL == str_ptr) || (0 == width) || (PNULL == char_num) || (PNULL == pixel_num))
    {
        return FALSE;
    }
    else
    {
    //存在连字和叠加文字时, 必须精确计算, 否则可以逐字进行估算
#if (defined COMPLEX_TEXT_SCRIPT || defined VECTOR_FONT_SUPPORT_ETRUMP)

        SPML_TEXT_PARAM_T param = {0};
        
        param.glyph_space = style_ptr->char_space;
        param.word_space  = style_ptr->word_space;
        param.dir = TEXT_DIR_AUTO;        
        param.size = GUI_ConvertGUIFontToSize( style_ptr->font );
        
        if (IsSetState(state, GUISTR_STATE_EFFECT)) // 设置了effect状态时，才转化响应状态
        {
            ConvertFontEffect( style_ptr->effect, style_ptr->font_color, style_ptr->edge_color, &param );
        }
        
        // 设置文本其他状态
        ConvertTextState(state, &param.format);
        
        *char_num = SPMLAPI_LineBreak( &param, str_ptr, length, width, pixel_num, is_byword);
#else
        uint16  w_char = 0;
        uint16  i = 0;
        uint8   step = 0;
        uint8   char_width = 0;
        uint16  pre_char_num  = 0;
        uint16  pre_pixel_num = 0;
        BOOLEAN is_exist_word_bread_ch = FALSE;
                
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
                
                if(i+1 < length)
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
                char_width = GUI_GetFontWidth( style_ptr->font, w_char );
                
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
                    if (i+1 == length)
                    {
                        *pixel_num += char_width;
                    }
                    else
                    {
                        *pixel_num += char_width + style_ptr->char_space;
                    }
                    *char_num = (uint16)(i + step);
                    
                    //按词换行
                    if( (GUISTR_STATE_WORDBREAK&state) && SPMLAPI_IsWordBreakChar( w_char ) )
                    {
                        pre_pixel_num = *pixel_num;
                        pre_char_num  = *char_num;
                        
                        is_exist_word_bread_ch = TRUE;
                    }
                }
            }
            
            i += step;        
        } while(i < length);
        
        //可以显示的下最后一个字符,但是无法显示的下最后一个字间距
        if (*pixel_num > width)
        {
            *pixel_num = width;
        }
        
#endif
        return TRUE;
    }
}


/*******************************************************************************************/
// 	Description : calculate the ucs2 string by piexl number
//	Global resource dependence :
//  Author: bin.ji
//	Note:
/*********************************************************************************************/
LOCAL uint16 CalculateStringLinesByPixelNum(
                                            uint16    pixelnum,    //[IN] 像素个数
                                            const GUISTR_STYLE_T *style_ptr,    
                                            const wchar* str_ptr,  //[IN] String指针
                                            uint16     str_len,    //[IN] String长度(Byte长度)
                                            GUISTR_STATE_T       state
                                            )
{
    uint16 i = 0;
    uint16 line_length = 0;
    uint16 cur_len = 0;

    uint16 pixel_num_line=0;

    if ((PNULL == style_ptr) || (PNULL == str_ptr))
    {
        return (0);
    }

    if (0 == pixelnum)
    {
        return (0);
    }

    while(cur_len < str_len)
    {
        if( GetStringInfoInLineWithDir(style_ptr, 
            &str_ptr[cur_len],
            (uint16)(str_len - cur_len),
            state,
            pixelnum,
            &line_length,
            &pixel_num_line,
            GUISTR_STATE_WORDBREAK&state
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
// 	Description : calculate the y position of new line
//	Global resource dependence :
//  Author: xiaoqing.lu
//	Note:
/*********************************************************************************************/
LOCAL int16 GetYPosition(
                         GUI_RECT_T  rect,         // the fixed display area
                         GUISTR_STYLE_T     *style_ptr,
                         const MMI_STRING_T *str_ptr,
                         GUISTR_STATE_T     state,
                         uint16      font_height,   // space between two chars.
                         BOOLEAN     is_single_line // 
                         )
{
    int16 y = 0; // 一定要支持负区域
    uint16 line_num = 0;
    uint16 line_num_max = 0;
    uint16 str_height = 0;
    uint16 height = 0;

	if (PNULL == style_ptr || str_ptr == PNULL || !CheckRectValid(rect))
    {
        return y;
    }

    if (ALIGN_TOP == style_ptr->align 
        || ALIGN_LEFT == style_ptr->align 
        || ALIGN_RIGHT == style_ptr->align 
        || ALIGN_HMIDDLE == style_ptr->align 
        || (int16)font_height >= (rect.bottom - rect.top + 1))
    {
        y = rect.top;
    }
    else if (is_single_line)
    {
        str_height = font_height;
        switch (style_ptr->align)
        {
        case ALIGN_BOTTOM:
            y = (rect.bottom - (int16)str_height + 1);
            break;
            
        case ALIGN_VMIDDLE:
        case ALIGN_LVMIDDLE:
        case ALIGN_RVMIDDLE:
        case ALIGN_HVMIDDLE:
            y = ((rect.bottom + rect.top - (int16)str_height + 1)>>1);
            break;
        default:
            y = rect.top;
            break;
        }   
    }
    else
    {
        // 多行显示并且align是其他的状态时
        line_num = CalculateStringLinesByPixelNum((uint16)(rect.right-rect.left+1), 
            style_ptr,
            str_ptr->wstr_ptr,
            str_ptr->wstr_len,
            state
            );

        height = font_height + style_ptr->line_space;
        if (0 != height)
        {
            line_num_max = (uint16)((rect.bottom - rect.top + 1 + (int16)style_ptr->line_space)/(int16)height);
        }
        
        if (line_num > line_num_max)
        {
            line_num = line_num_max;
        }

        str_height = (uint16)(line_num * (font_height+style_ptr->line_space) - style_ptr->line_space);

        if ( (int16)str_height < (rect.bottom - rect.top + 1))
        {
            switch (style_ptr->align)
            {
            case ALIGN_BOTTOM:
                y = (rect.bottom - (int16)str_height + 1);
                break;
                
            case ALIGN_VMIDDLE:
            case ALIGN_LVMIDDLE:
            case ALIGN_RVMIDDLE:
            case ALIGN_HVMIDDLE:
                y = ((rect.bottom + rect.top - (int16)str_height + 1)>>1);
                break;
            default:
                y = rect.top;
                break;
            }    
        }
        else
        {
            y = rect.top;
        }
    }

    return y; 
}

/*****************************************************************************/
//  Description : convert font effect
//  Global resource dependence : 
//  Author:James.zhang
//  Note:
/*****************************************************************************/
LOCAL void ConvertFontEffect( 
                             GUIFONT_EFFECT_T    effect,
                             GUI_COLOR_T 	     color,
                             GUI_COLOR_T         edge_color,
                             SPML_TEXT_PARAM_T   *param //out
                             )
{
    if (PNULL != param && FONT_EFFECT_NONE != effect )
    {
        if(FONT_EFFECT_REVERSE_EDGE == effect )
        {
            param->edge_color = (GUI_COLOR_T)(~color);
        }
        else if(FONT_EFFECT_WIHTE_EDGE == effect )
        {
            param->edge_color = 0xFFFF;
        }
        else if(FONT_EFFECT_CUSTOM == effect )
        {
            param->edge_color = edge_color;
        }
        
        param->format |= TEXT_FORMAT_EDGE;
    }
}

/*****************************************************************************/
// 	Description : 计算剪切区域, 得到两个区域的共同区域
//	Global resource dependence : none
//  Author: xiaoqing.lu
//	Note: 
/*****************************************************************************/
LOCAL GUI_RECT_T GetTrimRect(
                             GUI_RECT_T rect1, 
                             GUI_RECT_T rect2
                             )
{
    GUI_RECT_T trim_rect = {0};

    trim_rect.left = MAX(rect1.left, rect2.left);
    trim_rect.top = MAX(rect1.top, rect2.top);
    trim_rect.right = MIN(rect1.right, rect2.right);
    trim_rect.bottom = MIN(rect1.bottom, rect2.bottom);

    return (trim_rect);
}

/*****************************************************************************/
// 	Description : 获取文本的方向
//	Global resource dependence : none
//  Author: James.zhang
//	Note: 
/*****************************************************************************/
LOCAL SPML_TEXT_DIR_T GetNextLineTextDir( 
                                         SPML_TEXT_DIR_T dir, //当前行的文字方向
                                         const wchar   *text_ptr,
                                         uint16        len,
                                         uint16        pos
                                         )
{
    SPML_TEXT_DIR_T line_dir  = dir;
    wchar         last_char = 0;

    if ( len > pos )
    {
        if ( pos > 0 )
        {
            last_char = text_ptr[pos - 1];
        }
        
        if ( 0 == pos || '\r' == last_char || '\n' == last_char )
        {
            line_dir = SPMLAPI_GetTextDirection( text_ptr + pos, len - pos );
        }
    }

    return line_dir;
}

#ifndef GUI_STR_MINI_SUPPORT
/*****************************************************************************/
// 	Description : 根据偏移量，计算剩余字符串的特殊效果字段数
//	Global resource dependence : none
//  Author: xiaoqing.lu
//	Note: 注意：传入的region的start必须是有序的，从小到大。
/*****************************************************************************/
PUBLIC BOOLEAN GUISTR_CreateRegionStyleByPos(
                                             const MMI_STRING_T *str_ptr, 
                                             GUIFONT_REGION_STYLE_T *src_region_style_ptr, 
                                             uint16 src_region_num,
                                             uint16 pos,
                                             GUIFONT_REGION_STYLE_T **des_region_style_pptr, 
                                             uint16 *des_region_num_ptr
                                             )
{
    GUISTR_STYLE_T style_info = {0}; /*lint !e64*/

    if (PNULL == str_ptr || PNULL == src_region_style_ptr 
        || 0 == src_region_num || PNULL == des_region_num_ptr || PNULL == des_region_style_pptr)
    {
        return FALSE;
    }

    if ( PNULL == str_ptr->wstr_ptr
        || 0 == str_ptr->wstr_len )
    {
        return FALSE;
    }

    style_info.region_num = src_region_num;
    style_info.region_ptr = src_region_style_ptr;

    return CreateRegionStyle(str_ptr, &style_info, pos, des_region_style_pptr, des_region_num_ptr);
}
#endif

/*****************************************************************************/
// 	Description : 释放特效字段内存
//	Global resource dependence : none
//  Author: xiaoqing.lu
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN GUISTR_DestroyRegionStyle(
                                         GUIFONT_REGION_STYLE_T *region_style_ptr
                                         )
{
    DestoryPtr((void*)region_style_ptr);
    return TRUE;
}

/*****************************************************************************/
// 	Description : 根据偏移量，计算剩余字符串的特殊效果字段数
//	Global resource dependence : none
//  Author: xiaoqing.lu
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN CreateRegionStyle(
                               const MMI_STRING_T *str_ptr, 
                               GUISTR_STYLE_T *style_ptr, 
                               uint16 offset,
                               GUIFONT_REGION_STYLE_T **region_style_pptr, 
                               uint16 *region_num_ptr
                               )
{
    BOOLEAN result = FALSE;
    uint16 i = 0;
    uint16 index = 0;
    GUIFONT_REGION_STYLE_T *region_style_ptr = PNULL;
    uint16 region_num = 0;

    
    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != region_num_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != str_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != style_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != region_style_pptr); /*assert verified*/
    if ((PNULL == region_num_ptr) ||(PNULL == str_ptr)||(PNULL == style_ptr)||(PNULL == region_style_pptr))
    {
        return FALSE;
    }

    if (0 == style_ptr->region_num || PNULL == style_ptr->region_ptr)
    {
        return result;
    }

    for (i = 0; i < style_ptr->region_num; i++)
    {
        // 找到位置
        if (offset <= style_ptr->region_ptr[i].start)
        {
            result = TRUE; // 说明找到了
            break;
        }
    }

    if (result && (0 == i)) // 
    {
        result = TRUE; // 这个设置是为了保持原来的不变
        region_style_ptr = SCI_ALLOC_APP(sizeof(GUIFONT_REGION_STYLE_T)*style_ptr->region_num);
        SCI_MEMSET(region_style_ptr, 0, (sizeof(GUIFONT_REGION_STYLE_T)*style_ptr->region_num));
        SCI_MEMCPY(region_style_ptr, style_ptr->region_ptr, (sizeof(GUIFONT_REGION_STYLE_T)*style_ptr->region_num));
        *region_style_pptr = region_style_ptr;
        *region_num_ptr = style_ptr->region_num;
    }
    else if (!result && (style_ptr->region_num == i) 
        && (offset <= style_ptr->region_ptr[i - 1].start + style_ptr->region_ptr[i - 1].length))
    {
        // 这个说明找到了一个, 在最后一个的区域内
        region_num = 1;
        region_style_ptr = SCI_ALLOC_APP(sizeof(GUIFONT_REGION_STYLE_T)*region_num);
        SCI_MEMSET(region_style_ptr, 0, (sizeof(GUIFONT_REGION_STYLE_T)*region_num));
        region_style_ptr->start = offset;
        region_style_ptr->font_color = style_ptr->region_ptr[i - 1].font_color;
        region_style_ptr->state = style_ptr->region_ptr[i - 1].state;
        region_style_ptr->length = (style_ptr->region_ptr[i - 1].length) + (style_ptr->region_ptr[i - 1].start) - offset;
        *region_style_pptr = region_style_ptr;
        *region_num_ptr = region_num;
        result = TRUE;
        for (index = 0; index < region_num; index++)
        {
            region_style_ptr[index].start -= offset;
        }
    }
    else if (result && ((offset >= style_ptr->region_ptr[i - 1].start + style_ptr->region_ptr[i - 1].length) 
        && (offset <= style_ptr->region_ptr[i].start)))
    {
        // 这种情况说明在第i和第i-1之间，不包括i-1的区域
        region_num = style_ptr->region_num - i;
        region_style_ptr = SCI_ALLOC_APP(sizeof(GUIFONT_REGION_STYLE_T)*region_num);
        SCI_MEMSET(region_style_ptr, 0, (sizeof(GUIFONT_REGION_STYLE_T)*region_num));
        SCI_MEMCPY(region_style_ptr, &style_ptr->region_ptr[i], sizeof(GUIFONT_REGION_STYLE_T)*region_num);
        *region_style_pptr = region_style_ptr;
        *region_num_ptr = region_num;
        for (index = 0; index < region_num; index++)
        {
            region_style_ptr[index].start -= offset;
        }
    }
    else if (result && ((offset >= style_ptr->region_ptr[i-1].start) 
        && (offset < style_ptr->region_ptr[i - 1].start + style_ptr->region_ptr[i - 1].length)))
    {
        // 这种情况说明在第i和第i-1之间，包括i-1的部分区域
        region_num = style_ptr->region_num - i + 1;
        region_style_ptr = SCI_ALLOC_APP(sizeof(GUIFONT_REGION_STYLE_T)*region_num);
        SCI_MEMSET(region_style_ptr, 0, (sizeof(GUIFONT_REGION_STYLE_T)*region_num));
        // 从第1位置开始拷贝
        SCI_MEMCPY(&region_style_ptr[1], &style_ptr->region_ptr[i], (sizeof(GUIFONT_REGION_STYLE_T)*(region_num-1)));
        region_style_ptr[0].state = style_ptr->region_ptr[i-1].state;
        region_style_ptr[0].start = offset;
        region_style_ptr[0].font_color = style_ptr->region_ptr[i-1].font_color;
        region_style_ptr[0].length = (style_ptr->region_ptr[i-1].length) + (style_ptr->region_ptr[i-1].start) - offset;
        *region_style_pptr = region_style_ptr;
        *region_num_ptr = region_num;
        for (index = 0; index < region_num; index++)
        {
            region_style_ptr[index].start -= offset;
        }
    }

    return result;
}

/*****************************************************************************/
// 	Description : 判断状态是否设置
//	Global resource dependence : none
//  Author: xiaoqing.lu
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsSetState(
                         GUISTR_STATE_T src_state, 
                         GUISTR_STATE_T dst_state
                         )
{
    return (BOOLEAN)(((src_state&dst_state) == dst_state) ? TRUE : FALSE);
}

/*****************************************************************************/
// 	Description : 显示字符串
//	Global resource dependence : none
//  Author: xiaoqing.lu
//	Note: 
/*****************************************************************************/
LOCAL GUI_RECT_T DrawTextInRect(
                                GUISTR_STYLE_T     *style_ptr,
                                GUISTR_BUF_T       *des_buf_ptr,
                                const GUI_RECT_T   *disp_rect_ptr,       //the fixed display area
                                const GUI_RECT_T   *clip_rect_ptr,       //用户要剪切的实际区域
                                const MMI_STRING_T *str_ptr,
                                GUISTR_STATE_T     state,
                                int8               dir
                                )
{
    int16       y           = 0;    
    uint16      font_height = 0;
    uint16      char_num    = 0;
    uint16      pixel_num   = 0;
    uint16      pos         = 0;
    GUI_RECT_T   act_rect = {0};
    GUI_RECT_T   buf_rect = {0};
    GUI_RECT_T   clip_rect = {0};
    GUI_RECT_T   line_act_rect = {0};
    MMI_STRING_T str_data = {0};
    uint16      line_index  = 0;
    GUI_ALIGN_E align = ALIGN_LEFT;
    GUISTR_STYLE_T *tmp_style_ptr = PNULL;
    GUI_RECT_T  text_rect   = {0};
    BOOLEAN is_change_align = ( TEXT_DIR_AUTO == dir ) ? TRUE : FALSE;
    BOOLEAN is_single_line = IsSetState(state, GUISTR_STATE_SINGLE_LINE); // 是否单行显示
    BOOLEAN is_ellipsis = IsSetState(state, GUISTR_STATE_ELLIPSIS_EX);    //是否显示。。。
    wchar  *temp_str = PNULL;
    // 保存
    uint16 orig_region_num = 0;
    GUIFONT_REGION_STYLE_T *orig_region_style_ptr = PNULL;
    uint16 tmp_region_num = 0;
    GUIFONT_REGION_STYLE_T *tmp_region_style_ptr = PNULL;

    // 入参检测
    if (PNULL == des_buf_ptr || PNULL == disp_rect_ptr || PNULL == clip_rect_ptr
        || PNULL == str_ptr || PNULL == style_ptr)
    {
        return act_rect;
    }

    if ( PNULL == str_ptr->wstr_ptr
        || 0 == str_ptr->wstr_len )
    {
        return act_rect;
    }
	orig_region_num = style_ptr->region_num;
    orig_region_style_ptr = style_ptr->region_ptr;

    font_height = GetFontHeight(style_ptr, str_ptr, state);
    if (0 == font_height)
    {
        //SCI_TRACE_LOW:"error! [DrawTextInRect] len = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUISTRING_929_112_2_18_3_21_43_348,(uint8*)"d", str_ptr->wstr_len);
        return act_rect;
    }
    buf_rect.left = 0;
    buf_rect.top = 0;
    buf_rect.right = des_buf_ptr->width - 1;
    buf_rect.bottom = des_buf_ptr->height - 1;

    clip_rect = GetTrimRect(buf_rect, *disp_rect_ptr);
    clip_rect = GetTrimRect(clip_rect, *clip_rect_ptr);

    // 获取Y的坐标
    y = GetYPosition(*disp_rect_ptr, style_ptr, str_ptr, state, font_height, is_single_line);

    align = style_ptr->align;

    // 初始化文本区域
    text_rect   = *disp_rect_ptr;
    tmp_style_ptr = style_ptr; 

    // 这里需要根据输入的特殊区域重新创建一个区域，因为传进来的区域有可能会有重和的区域，而底层是不能重合的
    tmp_region_num = CreateRegionPtr(orig_region_style_ptr, orig_region_num, &tmp_region_style_ptr);
    style_ptr->region_num = tmp_region_num;
    style_ptr->region_ptr = tmp_region_style_ptr;
    if (is_single_line) // 单行显示的时候
    {
        BOOLEAN is_not_half_char = IsSetState(state, GUISTR_STATE_NOT_HALF_CHAR);
		
        str_data.wstr_len = str_ptr->wstr_len;
        str_data.wstr_ptr = str_ptr->wstr_ptr;
		
        //spml底层实现ellipsis
        // 显示字符串，但行显示的时候首先判断是否显示半字的情况
        if ( is_ellipsis && GUISTR_LineBreak(
            dir,
            str_ptr->wstr_ptr,
            (uint16)(str_ptr->wstr_len), 
            style_ptr, 
            state & (~GUISTR_STATE_WORDBREAK),
            (uint16)(disp_rect_ptr->right - disp_rect_ptr->left + 1), 
            &char_num, 
            &pixel_num
            ))
        {            //显示...
            if (GUISTR_GetStringWidth(style_ptr, str_ptr, state) > text_rect.right - text_rect.left + 1)
            {
                // 计算一次,不需要断行函数,直接截取字符串
                wchar   ellipsis_str[] = { '.', '.', '.', 0};
                uint16  ellipsis_len   = MMIAPICOM_Wstrlen( ellipsis_str );
                uint16  ellipsis_width = 0;
                uint16  temp_char_num  = 0;
                uint16  temp_pixel_num = 0;
                MMI_STRING_T string = {0};
				
                string.wstr_ptr = ellipsis_str;
                string.wstr_len = ellipsis_len;
				
                ellipsis_width = GUISTR_GetStringWidth( style_ptr, &string, state );
				
                temp_str = SCI_ALLOC_APP( (str_ptr->wstr_len + ellipsis_len + 1) * sizeof(wchar));
                
                GUISTR_LineBreak( dir, str_ptr->wstr_ptr, str_ptr->wstr_len,
                    style_ptr, state & (~GUISTR_STATE_WORDBREAK),
                    (text_rect.right - text_rect.left - ellipsis_width),
                    &temp_char_num, &temp_pixel_num );
                
                MMIAPICOM_Wstrncpy( temp_str, str_ptr->wstr_ptr, temp_char_num );
                MMIAPICOM_Wstrncpy( temp_str + temp_char_num, ellipsis_str, ellipsis_len );
                
                str_data.wstr_len = temp_char_num + ellipsis_len;
                str_data.wstr_ptr = temp_str;
            }
        }
        else if( is_not_half_char && GUISTR_LineBreak(
            dir,
            str_ptr->wstr_ptr,
            (uint16)(str_ptr->wstr_len), 
            style_ptr, 
            state & (~GUISTR_STATE_WORDBREAK),
            (uint16)(disp_rect_ptr->right - disp_rect_ptr->left + 1), 
            &char_num, 
            &pixel_num
            ))
        {
            str_data.wstr_ptr = str_ptr->wstr_ptr;
            str_data.wstr_len = char_num;
        }
		
        act_rect = DrawTextInLine(tmp_style_ptr, des_buf_ptr, &text_rect, &clip_rect, (const MMI_STRING_T*)&str_data, state, dir, FALSE);
		
        DestoryPtr((void*)temp_str);
    }
    else // 多行显示的时候
    {
        while (pos < str_ptr->wstr_len)
        {
            if( GUISTR_LineBreak(
                dir,
                str_ptr->wstr_ptr + pos,
                (uint16)(str_ptr->wstr_len-pos), 
                style_ptr, 
                state,
                (uint16)(disp_rect_ptr->right - disp_rect_ptr->left + 1), 
                &char_num, 
                &pixel_num
                ))
            {
                BOOLEAN is_true = FALSE;
                uint16 region_num = 0;
                GUIFONT_REGION_STYLE_T *region_style_ptr = PNULL;
                
                line_index++;
                //多行的align由文字方向决定, 计算第一行文字的dir, 并后面文字排版做准备
                if ( char_num < str_ptr->wstr_len && 1 == line_index )
                {
                    dir = SPMLAPI_GetTextDirection( str_ptr->wstr_ptr + pos, str_ptr->wstr_len - pos );
                }
                
                //只有单行显示,不改变alignment
                if ( char_num >= str_ptr->wstr_len && 1 == line_index )
                {
                    is_change_align = FALSE;
                }

                // 给top赋值,因为多行显示的时候，y值是改变的
                text_rect.top = y;
                
                // 关于特效问题的代码:计算特殊效果的指针,这里计算是为了换行时，传给底层的字符串起始位置改变难了
                if(style_ptr->region_num > 0 
                    && CreateRegionStyle(str_ptr, style_ptr, pos, &region_style_ptr, &region_num))
                {
                    if (PNULL != region_style_ptr && 0 != region_num) // 将计算后的特殊区域传给底层
                    {
                        style_ptr->region_num = region_num;
                        style_ptr->region_ptr = region_style_ptr;
                    }
                }
                else
                {
                    // 没找到的话置为空
                    style_ptr->region_num = 0;
                    style_ptr->region_ptr = PNULL;
                }
                
                // 显示字符串
                str_data.wstr_ptr = str_ptr->wstr_ptr + pos;
                str_data.wstr_len = char_num;//str_ptr->wstr_len - pos;
                
                //如果需要显示... 则在最后一行显示...
                if ( is_ellipsis )
                {
                    if (pos + char_num <= str_ptr->wstr_len
                        && text_rect.bottom - text_rect.top + 1 < font_height) // 如果当前行区域高度不够
                    {
                        is_true = TRUE;
                    }
                    else if (y + font_height + style_ptr->line_space >= disp_rect_ptr->bottom
                        && pos + char_num < str_ptr->wstr_len) // 如果下一行超过显示区域的bottom
                    {
                        is_true = TRUE;
                    }
                    else if (y + font_height + style_ptr->line_space >= disp_rect_ptr->bottom - font_height
                        && pos + char_num < str_ptr->wstr_len) // 如果下一行的区域高度不够
                    {
                        is_true = TRUE;
                    }
                    else
                    {
                        is_true = FALSE;
                    }


                    //spml底层实现ellipsis
                    if ( is_true )
                    {
                        // 计算一次,不需要断行函数,直接截取字符串
                        wchar   ellipsis_str[] = { '.', '.', '.', 0};
                        uint16  ellipsis_len   = MMIAPICOM_Wstrlen( ellipsis_str );
                        uint16  ellipsis_width = 0;
                        uint16  temp_char_num  = 0;
                        uint16  temp_pixel_num = 0;
                        MMI_STRING_T string = {0};
                        
                        string.wstr_ptr = ellipsis_str;
                        string.wstr_len = ellipsis_len;
                        
                        ellipsis_width = GUISTR_GetStringWidth( style_ptr, &string, state );
                        
                        temp_str = SCI_ALLOC_APP( (str_data.wstr_len + ellipsis_len + 1) * sizeof(wchar));
                        
                        GUISTR_LineBreak( dir, str_data.wstr_ptr, str_data.wstr_len,
                            style_ptr, state & (~GUISTR_STATE_WORDBREAK),
                            (text_rect.right - text_rect.left - ellipsis_width),
                            &temp_char_num, &temp_pixel_num );
                        
                        MMIAPICOM_Wstrncpy( temp_str, str_data.wstr_ptr, temp_char_num );
                        MMIAPICOM_Wstrncpy( temp_str + temp_char_num, ellipsis_str, ellipsis_len );
                        
                        str_data.wstr_len = temp_char_num + ellipsis_len;
                        str_data.wstr_ptr = temp_str;
                    }
                    
                    if ( is_true )
                    {
                        state |= GUISTR_STATE_ELLIPSIS;
                    }
                    else
                    {
                        state &= ~GUISTR_STATE_ELLIPSIS; 
                    }
                }


                // 显示字符
                line_act_rect = DrawTextInLine(tmp_style_ptr, des_buf_ptr, &text_rect, &clip_rect, &str_data, state, dir, is_change_align);
                
                DestoryPtr((void*)temp_str);
                temp_str = PNULL;

                // 关于特效问题的代码：释放内存
                if (region_num > 0 && PNULL != region_style_ptr)
                {
                    // 释放内存
                    DestoryPtr((void*)region_style_ptr);
                    region_style_ptr = PNULL;
                    region_num = 0;
                    // 恢复，下次计算的时候还是从开始计算的。
                    style_ptr->region_num = tmp_region_num;
                    style_ptr->region_ptr = tmp_region_style_ptr;
                }
                
                // 计算下一行
                pos += char_num;
                y += (font_height + style_ptr->line_space);

                if ( 1 == line_index )
                {
                    act_rect = line_act_rect;
                }
                else
                {
                    act_rect = clip_rect;

                    act_rect.bottom = line_act_rect.bottom;
                }

                // 判断下一行的y是否超出区域
                if (y >= disp_rect_ptr->bottom || is_true) 
                {
                    //act_rect = line_act_rect;
                    break;
                }
            }
            else
            {
                break;
            }
            
            // 获取下一行的显示方向
            if ( pos < str_ptr->wstr_len )
            {
                dir = GetNextLineTextDir( dir, str_ptr->wstr_ptr, str_ptr->wstr_len, pos );
            }
        }
    }

    // 创建时申请的内存要释放
    DestoryPtr((void*)tmp_region_style_ptr);
    // 恢复，为了不改变用户的输入参数，这里需要恢复
    style_ptr->region_num = orig_region_num;
    style_ptr->region_ptr = orig_region_style_ptr;

    return act_rect;
}

/*****************************************************************************/
// 	Description : 根据传入的文本gui状态，转化成底层显示时能够识别的状态
//	Global resource dependence : none
//  Author: xiaoqing.lu
//	Note: 
/*****************************************************************************/
LOCAL void ConvertTextState(
                            GUISTR_STATE_T     state, 
                            SPML_TEXT_FORMAT_T *format_ptr
                            )
{
    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != format_ptr); /*assert verified*/
    if (PNULL == format_ptr)
    {
        return;
    }
    
    //从头部显示, 用于向左滚动
    if ( state & GUISTR_STATE_FROM_HEAD )
    {
        *format_ptr |= TEXT_FORMAT_FROM_HEAD;
    }
    
    //从尾部显示, 用于向右滚动
    if ( state & GUISTR_STATE_FROM_TAIL )
    {
        *format_ptr |= TEXT_FORMAT_FROM_TAIL;
    }
    //粗体
    if ( state & GUISTR_STATE_BOLD )
    {
        *format_ptr |= TEXT_FORMAT_BOLD;
    }
    //斜体
    if ( state & GUISTR_STATE_ITALIC )
    {
        *format_ptr |= TEXT_FORMAT_ITALIC;
    }
    //下划线
    if ( state & GUISTR_STATE_UNDERLINE )
    {
        *format_ptr |= TEXT_FORMAT_UNDERLINE;
    }
    
    //背景
    if ( state & GUISTR_STATE_BACKGROUND )
    {
        *format_ptr |= TEXT_FORMAT_BACKGROUND;
    }

    // 反色
    if ( state & GUISTR_STATE_INVERSE )
    {
        *format_ptr |= TEXT_FORMAT_INVERSE;
    }

    //词间距
    if ( state & GUISTR_STATE_WORDSPACE )
    {
        *format_ptr |= TEXT_FORMAT_WORDSPACE;
    }
    
    //显示渐变
    if ( state & (GUISTR_STATE_ELLIPSIS|GUISTR_STATE_NOT_HALF_CHAR) )
    {
        *format_ptr |= TEXT_FORMAT_ELLIPSIS;
    }
}


/*****************************************************************************/
// 	Description : 转为spml的buffer类型
//	Global resource dependence : none
//  Author: James.Zhang
//	Note: 
/*****************************************************************************/
LOCAL SPML_BUF_TYPE_T ConvertSpmlBufferType(
                                            GUISTR_BUF_TYPE_T type
                                            )
{
    SPML_BUF_TYPE_T result = SPML_BUF_RGB565;
    
    if ( GUISTR_BUF_ARGB888 == type )
    {
        result = SPML_BUF_ARGB888;
    }
    
    return result;
}


/*****************************************************************************/
// 	Description : 转为string的buffer类型
//	Global resource dependence : none
//  Author: James.Zhang
//	Note: 
/*****************************************************************************/
LOCAL GUISTR_BUF_TYPE_T ConvertStringBufferType(
                                                DISPLAY_DATA_TYPE_E type
                                                )
{
    GUISTR_BUF_TYPE_T result = GUISTR_BUF_RGB565;
    
    if ( DATA_TYPE_ARGB888 == type )
    {
        result = GUISTR_BUF_ARGB888;
    }
    
    return result;
}

/*****************************************************************************/
// 	Description : 显示字符串
//	Global resource dependence : none
//  Author: xiaoqing.lu
//	Note: 
/*****************************************************************************/
LOCAL GUI_RECT_T DrawTextInLine(
                                GUISTR_STYLE_T     *style_ptr,
                                GUISTR_BUF_T       *des_buf_ptr,
                                const GUI_RECT_T   *disp_rect_ptr,       //the fixed display area
                                const GUI_RECT_T   *clip_rect_ptr,       //用户要剪切的实际区域
                                const MMI_STRING_T *str_ptr,
                                GUISTR_STATE_T     state,
                                int8               dir,
                                BOOLEAN            is_change_align
                                )
{
    SPML_TEXT_PARAM_T  param            = {0};
    SPML_TEXT_REGION_T *text_region_ptr = PNULL;
    GUI_RECT_T         act_rect         = {0};
    uint16             font_height      = 0;
    MMI_STRING_T string = {0};
#ifdef USE_STRING_ELLIPSIS
    wchar  *temp_str = PNULL;
#endif

    // 输入参数检查，均不能为NULL
    
    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != style_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != des_buf_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != disp_rect_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != clip_rect_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != str_ptr); /*assert verified*/
    if ((PNULL == style_ptr)||(PNULL == des_buf_ptr)||(PNULL == disp_rect_ptr)||(PNULL == clip_rect_ptr)||(PNULL == str_ptr))
    {
        return act_rect;
    }

    font_height = GUI_GetStringHeight(style_ptr->font, str_ptr->wstr_ptr, str_ptr->wstr_len);

    // 判断最后一行是否超出区域并且不显示半行
    if (IsSetState(state, GUISTR_STATE_NO_HALF_LINE)
        && (disp_rect_ptr->bottom - disp_rect_ptr->top + 1 < font_height))
    {
        return act_rect;
    }

    string = *str_ptr;

#ifdef USE_STRING_ELLIPSIS
    //显示...
    if( IsSetState(state, GUISTR_STATE_ELLIPSIS) )
    {
        if (GUISTR_GetStringWidth(style_ptr, str_ptr, state) > disp_rect_ptr->right - disp_rect_ptr->left + 1)
        {
            // 计算一次,不需要断行函数,直接截取字符串
            wchar   ellipsis_str[] = { '.', '.', '.', 0};
            uint16  ellipsis_len   = MMIAPICOM_Wstrlen( ellipsis_str );
            uint16  ellipsis_width = 0;
            uint16  temp_char_num  = 0;
            uint16  temp_pixel_num = 0;
            MMI_STRING_T temp_string = {0};
            
            
            temp_string.wstr_ptr = ellipsis_str;
            temp_string.wstr_len = ellipsis_len;
            
            ellipsis_width = GUISTR_GetStringWidth( style_ptr, &temp_string, state );
            
            temp_str = SCI_ALLOC_APP( (str_ptr->wstr_len + ellipsis_len + 1) * sizeof(wchar));
            
            GUISTR_LineBreak( dir, str_ptr->wstr_ptr, str_ptr->wstr_len,
                style_ptr, state & (~GUISTR_STATE_WORDBREAK),
                (disp_rect_ptr->right - disp_rect_ptr->left - ellipsis_width),
                &temp_char_num, &temp_pixel_num );
            
            MMIAPICOM_Wstrncpy( temp_str, str_ptr->wstr_ptr, temp_char_num );
            MMIAPICOM_Wstrncpy( temp_str + temp_char_num, ellipsis_str, ellipsis_len );
            
            string.wstr_len = temp_char_num + ellipsis_len;
            string.wstr_ptr = temp_str;
        }
    }
#endif

    // 判断文本是否为空
    if (PNULL != string.wstr_ptr && 0 != string.wstr_len) 
    {
        SPML_TEXT_METRICS_T  metrics_data = {0};

        param.buffer_type = ConvertSpmlBufferType( des_buf_ptr->type );

        param.buffer_ptr = des_buf_ptr->buffer;
//        param.buffer_width = des_buf_ptr->width;
        param.buffer_width = des_buf_ptr->mem_width;
        param.buffer_height = des_buf_ptr->height;
        
        param.font_color = style_ptr->font_color;
        param.bg_color   = style_ptr->bg_color;
        param.line_color = style_ptr->line_color;

        param.dir = dir;

        param.format |= ConvertAlignment( state, dir, is_change_align, style_ptr->align );

        if (IsSetState(state, GUISTR_STATE_EFFECT)) // 设置了effect状态时，才转化响应状态
        {
            ConvertFontEffect( style_ptr->effect, style_ptr->font_color, style_ptr->edge_color, &param );
        }

        // 设置文本其他状态
        ConvertTextState(state, &param.format);

        param.display_rect = *disp_rect_ptr;
        param.clip_rect = *clip_rect_ptr;

        param.size = GUI_ConvertGUIFontToSize(style_ptr->font);

        param.glyph_space = style_ptr->char_space;
        param.word_space  = style_ptr->word_space;
        param.edge_alpha  = style_ptr->edge_alpha;

        if ( style_ptr->region_num > 0 )
        {
            int32 i = 0;
            
            text_region_ptr = SCI_ALLOC_APP( style_ptr->region_num * sizeof(SPML_TEXT_REGION_T) );
            
            SCI_MEMSET( text_region_ptr, 0, (style_ptr->region_num * sizeof(SPML_TEXT_REGION_T)) );
            
            for ( i = 0; i < style_ptr->region_num; i++ )
            {
                text_region_ptr[i].start = style_ptr->region_ptr[i].start;
                text_region_ptr[i].end   = style_ptr->region_ptr[i].start + style_ptr->region_ptr[i].length;

                // 颜色, 大小, format根据需求填写, 暂时支持不同字体
                text_region_ptr[i].style.color = param.font_color;
                text_region_ptr[i].style.size  = param.size;
                ConvertTextState(style_ptr->region_ptr[i].state, &text_region_ptr[i].style.format);
                if ((style_ptr->region_ptr[i].state&GUISTR_STATE_SPEC_FONTCOLOR)
                    || (style_ptr->region_ptr[i].state&GUISTR_STATE_INVERSE))
                {
                    text_region_ptr[i].style.color = style_ptr->region_ptr[i].font_color;
                }
            }

            param.region_num = style_ptr->region_num;
            param.region_ptr = text_region_ptr;
        }

        MMI_TraceAutoTestOfText(string.wstr_ptr, string.wstr_len);

#if 0
        //for huawei
        //不需要自动调整bearing y的功能
        param.format |= TEXT_FORMAT_NO_AUTOBY;

        //字号小于14调节亮度
        if ( param.size <= 14 )
        {
            param.brightness = 185;
            param.format |= TEXT_FORMAT_BRIGHTNESS;
        }
        //for huawei end
#endif

#ifdef USE_STRING_ELLIPSIS 
        param.format &= ~TEXT_FORMAT_ELLIPSIS;
#endif
        SPMLAPI_DrawText( &param, string.wstr_ptr, string.wstr_len, &metrics_data );

        if ( 0 == metrics_data.height )
        {
            metrics_data.height = font_height;
        }

        if ( 0 == metrics_data.width )
        {
            metrics_data.width = clip_rect_ptr->right - clip_rect_ptr->left + 1;
        }

        //回车等字符会导致rect为0
        //if (0 != metrics_data.height && 0 != metrics_data.width)
        {
            int32 width = 0;
            int32 height = 0;
            act_rect = *disp_rect_ptr;

            //根据对齐方式调整起始点的坐标
            width  = act_rect.right - act_rect.left + 1;
            height = act_rect.bottom - act_rect.top + 1;

            if ( metrics_data.width < width )
            {
                if ( param.format & TEXT_FORMAT_CENTER ) 
                {
                    act_rect.left += ( width - metrics_data.width ) / 2;
                }
                else if ( param.format & TEXT_FORMAT_RIGHT )
                {
                    act_rect.left += ( width - metrics_data.width );
                }
            }

            act_rect.right = act_rect.left + metrics_data.width;

            if ( metrics_data.height < height )
            {
                if ( param.format & TEXT_FORMAT_MIDDLE ) 
                {
                    act_rect.top += ( height - metrics_data.height ) / 2;
                }
                else if ( param.format & TEXT_FORMAT_BOTTOM ) 
                {
                    act_rect.top += ( height - metrics_data.height );
                }
            }

            act_rect.bottom = act_rect.top + metrics_data.height;

            act_rect = GetTrimRect(act_rect, *clip_rect_ptr);
            
            if ( GUI_IsInvalidRect( act_rect ) )
            {
                SCI_MEMSET( &act_rect, 0, sizeof(GUI_RECT_T) );
            }
        }

        if ( PNULL != text_region_ptr )
        {
            SCI_FREE( text_region_ptr );
            text_region_ptr = PNULL;
        }
    }
    
#ifdef USE_STRING_ELLIPSIS
    if ( PNULL != temp_str )
    {
        SCI_FREE(temp_str);
    }
#endif

    return act_rect;
}

/*****************************************************************************/
// 	Description : 往LCD上画字符串
//	Global resource dependence : none
//  Author: xiaoqing.lu
//	Note: 
/*****************************************************************************/
LOCAL void GUISTR_DrawTextToLCDSingleLineInternal( 
                                           const GUI_LCD_DEV_INFO *dev_info_ptr,
                                           const GUI_POINT_T      *point_ptr,
                                           const MMI_STRING_T     *str_ptr,
                                           GUISTR_STYLE_T         *style_ptr,
                                           GUISTR_STATE_T         state
                                           )
{
    GUI_RECT_T      box = {0};
    GUISTR_BUF_T    des_buf_data = {0};
#ifdef UI_MULTILAYER_SUPPORT
    GUI_RECT_T      blt_rect = {0};
#endif
    GUI_POINT_T     point = {0};
    GUILCD_DRAW_DC_T draw_dc = {0};

    // 入参检测
    if (PNULL == dev_info_ptr || PNULL == point_ptr || PNULL == str_ptr || PNULL == style_ptr)
    {
        return;
    }

    if ( PNULL == str_ptr->wstr_ptr
        || 0 == str_ptr->wstr_len )
    {
        return;
    }

    if (!LCD_PrepareDrawParam(dev_info_ptr, &draw_dc))
    {
        return;
    }
    des_buf_data.buffer = draw_dc.lcd_buffer_ptr;
    des_buf_data.mem_width = draw_dc.mem_width;
    des_buf_data.width = draw_dc.width;
    des_buf_data.height = draw_dc.height;
    des_buf_data.type = ConvertStringBufferType( draw_dc.data_type );

#ifdef UI_MULTILAYER_SUPPORT
    point_ptr = LCD_ConvertDispPoint(GUILCD_CONVERT_POS_LCD2DISP, (GUI_LCD_DEV_INFO *)dev_info_ptr, point_ptr, &point);

    blt_rect.left = point_ptr->x;
    blt_rect.top = point_ptr->y;
    blt_rect.right = (draw_dc.width-1);
    blt_rect.bottom = (draw_dc.height-1);
    LCD_ConvertDispRect(GUILCD_CONVERT_POS_DISP2LCD, (GUI_LCD_DEV_INFO *)dev_info_ptr, &blt_rect, &blt_rect);
    UILAYER_BltLowerLayerWhenTransparent((GUI_LCD_DEV_INFO *)dev_info_ptr, &blt_rect);
#endif
	
    box = GUISTR_DrawTextSingleLine(&des_buf_data, point_ptr, str_ptr, style_ptr, state);

    MMITHEME_StoreUpdateRect(dev_info_ptr, box);

    return;
}

/*****************************************************************************/
// 	Description : 往LCD上画字符串,带渐变效果
//	Global resource dependence : none
//  Author: 
//	Note: 
/*****************************************************************************/
PUBLIC void GUISTR_DrawSingleLineGradualInRect( 
                                           const GUI_LCD_DEV_INFO *dev_info_ptr,
                                           const GUI_RECT_T      *rect_ptr,
                                           const MMI_STRING_T     *str_ptr,
                                           GUISTR_STYLE_T         *style_ptr,
                                           GUISTR_STATE_T         state
                                           )
{
#ifndef UI_MULTILAYER_SUPPORT
    uint16 fh, fw;
    GUI_POINT_T start_point = {0};

    fh = GUI_GetStringHeight(style_ptr->font, str_ptr->wstr_ptr, str_ptr->wstr_len);
    fw = GUISTR_GetStringWidth(style_ptr, str_ptr, state);

    switch (style_ptr->align)
    {
        case ALIGN_LEFT:
            start_point.x = rect_ptr->left;
            start_point.y = rect_ptr->top;
            break;
        case ALIGN_RIGHT:
            start_point.x = rect_ptr->right - fw;
            start_point.y = rect_ptr->top;
            break;
        case ALIGN_HMIDDLE:
            start_point.x = rect_ptr->left + (rect_ptr->right - rect_ptr->left -fw) / 2;
            start_point.y = rect_ptr->top;
            break;
        case ALIGN_TOP:
            start_point.x = rect_ptr->left;
            start_point.y = rect_ptr->top;
            break;
        case ALIGN_BOTTOM:
            start_point.x = rect_ptr->left;
            start_point.y = rect_ptr->bottom - fh;
            break;
        case ALIGN_VMIDDLE:
            start_point.x = rect_ptr->left;
            start_point.y = rect_ptr->top + (rect_ptr->bottom - rect_ptr->top - fh) / 2;
            break;
        case ALIGN_LVMIDDLE:                /*!< 水平方向左对齐，垂直方向居中 */
            start_point.x = rect_ptr->left;
            start_point.y = rect_ptr->top + (rect_ptr->bottom - rect_ptr->top - fh) / 2;
            break;
        case ALIGN_RVMIDDLE:                 /*!< 水平方向右对齐，垂直方向居中 */
            start_point.x = rect_ptr->right - fw;
            start_point.y = rect_ptr->top + (rect_ptr->bottom - rect_ptr->top - fh) / 2;
            break;
        case ALIGN_HVMIDDLE:                  /*!< 水平居中，垂直居中 */
            start_point.x = rect_ptr->left + (rect_ptr->right - rect_ptr->left -fw) / 2;
            start_point.y = rect_ptr->top + (rect_ptr->bottom - rect_ptr->top - fh) / 2;
            break;
	 default:
            start_point.x = rect_ptr->left;
            start_point.y = rect_ptr->top;
            break;
    }

    style_ptr->align = 0;
    GUISTR_DrawTextToLCDSingleLine(dev_info_ptr, &start_point, str_ptr, style_ptr, state);
	
#else

    uint8 begin_alpha = 0, end_alpha = 0;
    GUI_RECT_T      box = {0};
    GUISTR_BUF_T    des_buf_data = {0};
    GUI_RECT_T      blt_rect = {0};
    GUI_POINT_T     point = {0};
    GUILCD_DRAW_DC_T draw_dc = {0};
    UILAYER_CREATE_T    create_info = {0};
    GUI_LCD_DEV_INFO     layer = {0};
    GUISTR_INFO_T str_info = {0};
    GUI_LCD_DEV_INFO lcd_dev_info = {GUI_LCD_0, GUI_BLOCK_0};
    UILAYER_APPEND_BLT_T append_layer;
    GUI_POINT_T      point_ptr={0};
    int16            offset_x_ptr = 0;
    int16            offset_y_ptr = 0;
    // 入参检测
    if (PNULL == dev_info_ptr || PNULL == rect_ptr || PNULL == str_ptr || PNULL == style_ptr)
    {
        return;
    }

    if ( PNULL == str_ptr->wstr_ptr
        || 0 == str_ptr->wstr_len )
    {
        return;
    }

    if (!LCD_PrepareDrawParam(dev_info_ptr, &draw_dc))
    {
        return;
    }
    UILAYER_GetLayerPosition(dev_info_ptr, &offset_x_ptr, &offset_y_ptr);
    draw_dc.height += offset_y_ptr;

  //   state |=GUISTR_STATE_BOLD;
    begin_alpha = style_ptr->begin_alpha;
    end_alpha = style_ptr->end_alpha;
    GUISTR_GetStringInfo(style_ptr, str_ptr, state, &str_info);

    point_ptr.x = rect_ptr->left;
    point_ptr.y = rect_ptr->top;

    blt_rect.left = rect_ptr->left;
    blt_rect.top = rect_ptr->top;
    blt_rect.right = (int16)( rect_ptr->right- 1);
	//Bug 465383 修改设置list item字体透明度后，正确显示字符位置
    blt_rect.bottom = (int16)(MIN( rect_ptr->bottom/*rect_ptr->top + (int16)str_info.height*/, (int16)draw_dc.height) - 1);

    /* create a new 888 layer */
    create_info.lcd_id = dev_info_ptr->lcd_id;
    create_info.owner_handle = 0;
    create_info.offset_x = blt_rect.left;
    create_info.offset_y = blt_rect.top;
    create_info.width = (blt_rect.right + 1 - blt_rect.left);
    create_info.height = (blt_rect.bottom + 1 - blt_rect.top);
    create_info.is_bg_layer = FALSE;    /* FALSE is 888 layer */
    create_info.is_static_layer = TRUE; /* If TRUE, will alloc memory automaticlly */
    UILAYER_CreateLayer(
        &create_info,  
        &layer
        );
    
    UILAYER_SetLayerColorKey( &layer, TRUE, UILAYER_TRANSPARENT_COLOR );
    
    /* fill color key */
    UILAYER_Clear( &layer );

    append_layer.lcd_dev_info = layer;
    append_layer.layer_level = UILAYER_LEVEL_NORMAL;
    UILAYER_AppendBltLayer(&append_layer); 

    UILAYER_SetLayerPosition(&layer, blt_rect.left, blt_rect.top );

    if (LCD_PrepareDrawParam(&layer, &draw_dc))
    {
        uint8 *line_head_ptr = PNULL;
        uint8 *buff_ptr = PNULL;
        uint alpha = begin_alpha;
        float falpha = (float)alpha;
        //int8 step = 0;
        float step = 0.0;
        uint i = 0, j = 0;

        des_buf_data.buffer = draw_dc.lcd_buffer_ptr;
        des_buf_data.mem_width = draw_dc.mem_width;
        des_buf_data.width = draw_dc.width;
        des_buf_data.height = draw_dc.height;
        des_buf_data.type = ConvertStringBufferType( draw_dc.data_type );
        
        box = GUISTR_DrawTextSingleLine(&des_buf_data, &point, str_ptr, style_ptr, state);
        
        /* change argb888 layer alpha */
#if defined(__BIG_ENDIAN) || defined(__BigEndian)
        buff_ptr = (uint8 *)des_buf_data.buffer;
#else
        buff_ptr = (uint8 *)des_buf_data.buffer+3;
#endif
        if (state & GUISTR_STATE_GRADUAL_VER) 
        {
            step = ((float)(end_alpha - begin_alpha)) / draw_dc.height;
            for ( i = 0; i < des_buf_data.height; i++)
            {
                falpha += step;
                alpha = (uint)(falpha);
                line_head_ptr = buff_ptr;
                
                for(j = 0; j < des_buf_data.width; j++)
                {
                    if( 0 != *buff_ptr )
                    {
                        *buff_ptr = (*buff_ptr * alpha) >> 8;
                    }
                    buff_ptr += sizeof(uint32);
                }

                /* Here should be draw_dc.mem_width * sizeof(uint32), just for saving time */
                buff_ptr = line_head_ptr + (draw_dc.mem_width << 2); 
            }
        }
        else if(state & GUISTR_STATE_GRADUAL_HOR)
        {
            step = ((float)(end_alpha - begin_alpha)) / draw_dc.width;
            for ( i = 0; i < des_buf_data.width; i++)
            {
                falpha += step;
                alpha = (uint)(falpha);
                line_head_ptr = buff_ptr;
                
                for(j = 0; j < des_buf_data.height; j++)
                {
                    if( 0 != *buff_ptr )
                    {
                        *buff_ptr = (*buff_ptr * alpha) >> 8;
                    }
                    buff_ptr += (des_buf_data.width << 2);
                }

                buff_ptr = line_head_ptr + sizeof(uint32); 
            }

        }
        else
        {
            /* To be done later */
        }

        lcd_dev_info.lcd_id    = GUI_MAIN_LCD_ID;
        lcd_dev_info.block_id = dev_info_ptr->block_id;
        UILAYER_BltLayerToLayer(
                                    &lcd_dev_info,
                                    &layer,
                                    &blt_rect,
                                    TRUE
                                    );
    }

    UILAYER_RELEASELAYER(&layer);   /*lint !e506 !e774*/
//    UILAYER_ReleaseLayer(&layer);
    return;
#endif
}

#ifdef UI_MULTILAYER_SUPPORT
/*****************************************************************************/
// 	Description : 往LCD上画字符串,带渐变效果
//	Global resource dependence : none
//  Author: Yintang.Ren
//	Note: 
/*****************************************************************************/
LOCAL void GUISTR_DrawSingleLineGradual( 
                                           const GUI_LCD_DEV_INFO *dev_info_ptr,
                                           const GUI_POINT_T      *point_ptr,
                                           const MMI_STRING_T     *str_ptr,
                                           GUISTR_STYLE_T         *style_ptr,
                                           GUISTR_STATE_T         state
                                           )
{
    uint8 begin_alpha = 0, end_alpha = 0;
    GUI_RECT_T      box = {0};
    GUISTR_BUF_T    des_buf_data = {0};
    GUI_RECT_T      blt_rect = {0};
    GUI_POINT_T     point = {0};
    GUILCD_DRAW_DC_T draw_dc = {0};
    UILAYER_CREATE_T    create_info = {0};
    GUI_LCD_DEV_INFO     layer = {0};
    GUISTR_INFO_T str_info = {0};
    GUI_LCD_DEV_INFO lcd_dev_info = {GUI_LCD_0, GUI_BLOCK_0};
    UILAYER_APPEND_BLT_T append_layer;
   
    // 入参检测
    if (PNULL == dev_info_ptr || PNULL == point_ptr || PNULL == str_ptr || PNULL == style_ptr)
    {
        return;
    }

    if ( PNULL == str_ptr->wstr_ptr
        || 0 == str_ptr->wstr_len )
    {
        return;
    }

    if (!LCD_PrepareDrawParam(dev_info_ptr, &draw_dc))
    {
        return;
    }
    begin_alpha = style_ptr->begin_alpha;
    end_alpha = style_ptr->end_alpha;
    GUISTR_GetStringInfo(style_ptr, str_ptr, state, &str_info);

    blt_rect.left = point_ptr->x;
    blt_rect.top = point_ptr->y;
#if 0    
    blt_rect.right = (int16)(MIN(point_ptr->x + (int16)str_info.width , (int16)draw_dc.width) - 1);
#else
    blt_rect.right = (int16)(draw_dc.width - 1);
#endif
    blt_rect.bottom = (int16)(MIN( point_ptr->y + (int16)str_info.height, (int16)draw_dc.height) - 1);

    /* create a new 888 layer */
    create_info.lcd_id = dev_info_ptr->lcd_id;
    create_info.owner_handle = 0;
    create_info.offset_x = blt_rect.left;
    create_info.offset_y = blt_rect.top;
    create_info.width = (blt_rect.right + 1 - blt_rect.left);
    create_info.height = (blt_rect.bottom + 1 - blt_rect.top);
    create_info.is_bg_layer = FALSE;    /* FALSE is 888 layer */
    create_info.is_static_layer = TRUE; /* If TRUE, will alloc memory automaticlly */
    UILAYER_CreateLayer(
        &create_info,  
        &layer
        );
    
    UILAYER_SetLayerColorKey( &layer, TRUE, UILAYER_TRANSPARENT_COLOR );
    
    /* fill color key */
    UILAYER_Clear( &layer );

    append_layer.lcd_dev_info = layer;
    append_layer.layer_level = UILAYER_LEVEL_NORMAL;
    UILAYER_AppendBltLayer(&append_layer); 

    UILAYER_SetLayerPosition(&layer, blt_rect.left, blt_rect.top );

    if (LCD_PrepareDrawParam(&layer, &draw_dc))
    {
        uint8 *line_head_ptr = PNULL;
        uint8 *buff_ptr = PNULL;
        uint alpha = begin_alpha;
        float falpha = (float)alpha;
        //int8 step = 0;
        float step = 0.0;
        uint i = 0, j = 0;

        des_buf_data.buffer = draw_dc.lcd_buffer_ptr;
        des_buf_data.mem_width = draw_dc.mem_width;
        des_buf_data.width = draw_dc.width;
        des_buf_data.height = draw_dc.height;
        des_buf_data.type = ConvertStringBufferType( draw_dc.data_type );
        
        box = GUISTR_DrawTextSingleLine(&des_buf_data, &point, str_ptr, style_ptr, state);
        
        /* change argb888 layer alpha */
        #if defined(__BIG_ENDIAN) || defined(__BigEndian)
        buff_ptr = (uint8 *)des_buf_data.buffer;
        #else
        buff_ptr = (uint8 *)des_buf_data.buffer+3;
        #endif
        if (state & GUISTR_STATE_GRADUAL_VER) 
        {
            step = ((float)(end_alpha - begin_alpha)) / draw_dc.height;
            for ( i = 0; i < des_buf_data.height; i++)
            {
                falpha += step;
                alpha = (uint)(falpha);
                line_head_ptr = buff_ptr;
                
                for(j = 0; j < des_buf_data.width; j++)
                {
                    if( 0 != *buff_ptr )
                    {
                        *buff_ptr = (*buff_ptr * alpha) >> 8;
                    }
                    buff_ptr += sizeof(uint32);
                }

                /* Here should be draw_dc.mem_width * sizeof(uint32), just for saving time */
                buff_ptr = line_head_ptr + (draw_dc.mem_width << 2); 
            }
        }
        else if(state & GUISTR_STATE_GRADUAL_HOR)
        {
            step = ((float)(end_alpha - begin_alpha)) / draw_dc.width;
            for ( i = 0; i < des_buf_data.width; i++)
            {
                falpha += step;
                alpha = (uint)(falpha);
                line_head_ptr = buff_ptr;
                
                for(j = 0; j < des_buf_data.height; j++)
                {
                    if( 0 != *buff_ptr )
                    {
                        *buff_ptr = (*buff_ptr * alpha) >> 8;
                    }
                    buff_ptr += (des_buf_data.width << 2);
                }

                buff_ptr = line_head_ptr + sizeof(uint32); 
            }

        }
        else
        {
            /* To be done later */
        }

        lcd_dev_info.lcd_id    = GUI_MAIN_LCD_ID;
        lcd_dev_info.block_id = GUI_BLOCK_MAIN;
        UILAYER_BltLayerToLayer(
                                    &lcd_dev_info,
                                    &layer,
                                    &blt_rect,
                                    TRUE
                                    );
    }

    UILAYER_RELEASELAYER(&layer);   /*lint !e506 !e774*/
//    UILAYER_ReleaseLayer(&layer);
    return;
}
#endif //end of UI_MULTILAYER_SUPPORT
    
/*****************************************************************************/
// 	Description : 往LCD上画字符串
//	Global resource dependence : none
//  Author: xiaoqing.lu
//	Note: 
/*****************************************************************************/
PUBLIC void GUISTR_DrawTextToLCDSingleLine( 
                                           const GUI_LCD_DEV_INFO *dev_info_ptr,
                                           const GUI_POINT_T      *point_ptr,
                                           const MMI_STRING_T     *str_ptr,
                                           GUISTR_STYLE_T         *style_ptr,
                                           GUISTR_STATE_T         state
                                           )
{
#ifdef UI_MULTILAYER_SUPPORT
    if ( (state & GUISTR_STATE_GRADUAL_HOR) || (state & GUISTR_STATE_GRADUAL_VER))
    {
        GUISTR_DrawSingleLineGradual(
                                           dev_info_ptr,
                                           point_ptr,
                                           str_ptr,
                                           style_ptr,
                                           state
                                           );
    }
    else
#endif
    {
        GUISTR_DrawTextToLCDSingleLineInternal(
                                           dev_info_ptr,
                                           point_ptr,
                                           str_ptr,
                                           style_ptr,
                                           state
                                           );
    }
}

/*****************************************************************************/
// 	Description : 往buffer上画文本信息，以起始点为基准
//	Global resource dependence : none
//  Author: xiaoqing.lu
//	Note: 起始点的位置相对于buffer的原点
/*****************************************************************************/
PUBLIC GUI_RECT_T GUISTR_DrawTextSingleLine( 
                                            GUISTR_BUF_T           *des_buf_ptr,
                                            const GUI_POINT_T      *point_ptr,      //start point to display the string relative des_buf
                                            const MMI_STRING_T     *str_ptr,
                                            GUISTR_STYLE_T         *style_ptr,
                                            GUISTR_STATE_T         state
                                            )
{
    GUI_RECT_T   disp_rect = {0};
    GUI_RECT_T   act_rect = {0};
    GUI_RECT_T   buf_rect = {0};
    GUI_RECT_T   clip_rect = {0};
    //BOOLEAN is_not_half_char = IsSetState(state, GUISTR_STATE_NOT_HALF_CHAR);    // 是否按字断行，只对多行显示的时候才有用
    MMI_STRING_T str_data = {0};
    //uint16      char_num    = 0;
    //uint16      pixel_num   = 0;
    // 保存
    uint16 orig_region_num = 0;
    GUIFONT_REGION_STYLE_T *orig_region_style_ptr = PNULL;
    uint16 tmp_region_num = 0;
    GUIFONT_REGION_STYLE_T *tmp_region_style_ptr = PNULL;

    // 入参检测
    if (PNULL == des_buf_ptr || PNULL == point_ptr || PNULL == str_ptr || PNULL == style_ptr)
    {
        return act_rect;
    }

    if ( PNULL == str_ptr->wstr_ptr
        || 0 == str_ptr->wstr_len )
    {
        return act_rect;
    }
	orig_region_num = style_ptr->region_num;
    orig_region_style_ptr = style_ptr->region_ptr;
	
    // 计算显示的区域，相对于des buffer的原点
    disp_rect.left = point_ptr->x;
    disp_rect.top = point_ptr->y;
    disp_rect.right = des_buf_ptr->width - 1;
    disp_rect.bottom = des_buf_ptr->height - 1;
    // 计算buffer的原始区域
    buf_rect.left = 0;
    buf_rect.top = 0;
    buf_rect.right = des_buf_ptr->width - 1;
    buf_rect.bottom = des_buf_ptr->height - 1;

    // 检查现实的区域是否有效
    if (GUI_IsInvalidRect(disp_rect))
    {
        return act_rect;
    }

    // 计算剪切区域
    clip_rect = GetTrimRect(disp_rect, buf_rect);
    
    str_data.wstr_len = str_ptr->wstr_len;
    str_data.wstr_ptr = str_ptr->wstr_ptr;
    
    // 显示字符串，但行显示的时候首先判断是否显示半字的情况
//     if(is_not_half_char && GUISTR_LineBreak(
//         GUISTR_TEXT_DIR_AUTO,
//         str_ptr->wstr_ptr,
//         (uint16)(str_ptr->wstr_len), 
//         style_ptr, 
//         state & (~GUISTR_STATE_WORDBREAK),
//         (uint16)(clip_rect.right - clip_rect.left + 1), 
//         &char_num, 
//         &pixel_num
//         ))
//     {
//         str_data.wstr_ptr = str_ptr->wstr_ptr;
//         str_data.wstr_len = char_num;
//     }

    // 这里需要根据输入的特殊区域重新创建一个区域，因为传进来的区域有可能会有重和的区域，而底层是不能重合的
    tmp_region_num = CreateRegionPtr(orig_region_style_ptr, orig_region_num, &tmp_region_style_ptr);
    style_ptr->region_num = tmp_region_num;
    style_ptr->region_ptr = tmp_region_style_ptr;
    act_rect = DrawTextInLine(style_ptr, des_buf_ptr, &disp_rect, &clip_rect, str_ptr, state, TEXT_DIR_AUTO, FALSE);

    // 创建时申请的内存要释放
    DestoryPtr((void*)tmp_region_style_ptr);
    // 恢复，为了不改变用户的输入参数，这里需要恢复
    style_ptr->region_num = orig_region_num;
    style_ptr->region_ptr = orig_region_style_ptr;

    return act_rect;
}

#ifndef GUI_STR_MINI_SUPPORT
/*****************************************************************************/
// 	Description : 往LCD上画字符串
//	Global resource dependence : none
//  Author: xiaoqing.lu
//	Note: 
/*****************************************************************************/
PUBLIC void GUISTR_DrawTextToLCDMultiLine( 
                                          const GUI_LCD_DEV_INFO *dev_info_ptr,
                                          const GUI_POINT_T      *point_ptr,
                                          const MMI_STRING_T     *str_ptr,
                                          GUISTR_STYLE_T         *style_ptr,
                                          GUISTR_STATE_T         state
                                          )
{
    GUI_RECT_T      box = {0};
    GUISTR_BUF_T    des_buf_data = {0};
#ifdef UI_MULTILAYER_SUPPORT
    GUI_RECT_T      blt_rect = {0};
#endif
    GUI_POINT_T     point = {0};
    GUILCD_DRAW_DC_T draw_dc = {0};

    // 入参检测
    if (PNULL == dev_info_ptr || PNULL == point_ptr || PNULL == str_ptr || PNULL == style_ptr)
    {
        return;
    }

    if ( PNULL == str_ptr->wstr_ptr
        || 0 == str_ptr->wstr_len )
    {
        return;
    }

    if (!LCD_PrepareDrawParam(dev_info_ptr, &draw_dc))
    {
        return;
    }
    des_buf_data.buffer = draw_dc.lcd_buffer_ptr;
    des_buf_data.mem_width = draw_dc.mem_width;
    des_buf_data.width = draw_dc.width;
    des_buf_data.height = draw_dc.height;
    des_buf_data.type = ConvertStringBufferType( draw_dc.data_type );

#ifdef UI_MULTILAYER_SUPPORT
    point_ptr = LCD_ConvertDispPoint(GUILCD_CONVERT_POS_LCD2DISP, (GUI_LCD_DEV_INFO *)dev_info_ptr, point_ptr, &point);
    blt_rect.left = point_ptr->x;
    blt_rect.top = point_ptr->y;
    blt_rect.right = (draw_dc.width-1);
    blt_rect.bottom = (draw_dc.height-1);
    LCD_ConvertDispRect(GUILCD_CONVERT_POS_DISP2LCD, (GUI_LCD_DEV_INFO *)dev_info_ptr, &blt_rect, &blt_rect);
    UILAYER_BltLowerLayerWhenTransparent((GUI_LCD_DEV_INFO *)dev_info_ptr, &blt_rect);
#endif

    box = GUISTR_DrawTextMultiLine(&des_buf_data, point_ptr, str_ptr, style_ptr, state);

    MMITHEME_StoreUpdateRect(dev_info_ptr, box);
}

/*****************************************************************************/
// 	Description : 往buffer上画文本信息，以起始点为基准，以buffer的宽度和高度为界限
//	Global resource dependence : none
//  Author: xiaoqing.lu
//	Note: 起始点的位置相对于buffer的原点
/*****************************************************************************/
PUBLIC GUI_RECT_T GUISTR_DrawTextMultiLine( 
                                           GUISTR_BUF_T           *des_buf_ptr,
                                           const GUI_POINT_T      *point_ptr,      //start point to display the string relative des_buf
                                           const MMI_STRING_T     *str_ptr,
                                           GUISTR_STYLE_T         *style_ptr,
                                           GUISTR_STATE_T         state
                                           )
{
    GUI_RECT_T   disp_rect  = {0};
    GUI_RECT_T   act_rect   = {0};

    // 入参检测
    if (PNULL == des_buf_ptr || PNULL == point_ptr || PNULL == str_ptr || PNULL == style_ptr)
    {
        return act_rect;
    }

    if ( PNULL == str_ptr->wstr_ptr
        || 0 == str_ptr->wstr_len )
    {
        return act_rect;
    }

    // 计算显示的区域，相对于des buffer的原点
    disp_rect.left = point_ptr->x;
    disp_rect.top = point_ptr->y;
    disp_rect.right = des_buf_ptr->width - 1;
    disp_rect.bottom = des_buf_ptr->height - 1;

    return GUISTR_DrawTextInRect( des_buf_ptr, &disp_rect, &disp_rect, str_ptr, style_ptr, state, TEXT_DIR_AUTO );
}
#endif

/*****************************************************************************/
// 	Description : 往LCD上画字符串
//	Global resource dependence : none
//  Author: xiaoqing.lu
//	Note: 
/*****************************************************************************/
PUBLIC void GUISTR_DrawTextToLCDInRect( 
                                       const GUI_LCD_DEV_INFO *dev_info_ptr,
                                       const GUI_RECT_T      *disp_rect_ptr,       //the fixed display area
                                       const GUI_RECT_T      *clip_rect_ptr,       //用户要剪切的实际区域
                                       const MMI_STRING_T    *str_ptr,
                                       GUISTR_STYLE_T        *style_ptr,
                                       GUISTR_STATE_T        state,
                                       int8                  dir
                                       )
{
    GUI_RECT_T      box = {0};
    GUI_RECT_T      disp_rect = {0};
    GUI_RECT_T      clip_rect = {0};
    GUISTR_BUF_T    des_buf_data = {0};
    GUILCD_DRAW_DC_T draw_dc = {0};

    // 入参检测
    if ( PNULL == str_ptr || PNULL == style_ptr )
    {
        return;
    }

    if ( PNULL == str_ptr->wstr_ptr
        || 0 == str_ptr->wstr_len )
    {
        return;
    }

    if (PNULL == dev_info_ptr || !LCD_PrepareDrawParam(dev_info_ptr, &draw_dc))
    {
        return;
    }

    des_buf_data.buffer = draw_dc.lcd_buffer_ptr;
    des_buf_data.mem_width = draw_dc.mem_width;
    des_buf_data.width = draw_dc.width;
    des_buf_data.height = draw_dc.height;
    des_buf_data.type = ConvertStringBufferType( draw_dc.data_type );

#ifdef UI_MULTILAYER_SUPPORT
    UILAYER_BltLowerLayerWhenTransparent((GUI_LCD_DEV_INFO *)dev_info_ptr, disp_rect_ptr);
    disp_rect_ptr = LCD_ConvertDispRect(GUILCD_CONVERT_POS_LCD2DISP, (GUI_LCD_DEV_INFO *)dev_info_ptr, disp_rect_ptr, &disp_rect);
    clip_rect_ptr = LCD_ConvertDispRect(GUILCD_CONVERT_POS_LCD2DISP, (GUI_LCD_DEV_INFO*)dev_info_ptr, clip_rect_ptr, &clip_rect);
#endif

    box = GUISTR_DrawTextInRect(&des_buf_data, disp_rect_ptr, clip_rect_ptr, str_ptr,style_ptr, state, dir);
    
    MMITHEME_StoreUpdateRect(dev_info_ptr, box);
}


/*****************************************************************************/
// 	Description : 根据原始区域坐标和旋转角度，计算旋转后的坐标
//	Global resource dependence : none
//  Author: xiaoqing.lu
//	Note: 起始点的位置相对于buffer的原点
/*****************************************************************************/
LOCAL BOOLEAN CalculateRotateRect(
                                  const GUI_RECT_T *orig_rect_ptr,  // 原始区域坐标
                                  int16 x_offset,             // 坐标原点在x方向上的位移
                                  int16 y_offset,             // 坐标原点在y方向上的位移
                                  GUI_ANGLE_E angle,          // 旋转角度
                                  GUI_RECT_T *result_rect_ptr // 输出参数
                                  )
{
    BOOLEAN reslut = FALSE;
    GUI_POINT_T orig_point1 = {0}; // 对应于原始区域的左上角坐标
    GUI_POINT_T orig_point2 = {0}; // 对应于原始区域的右上角坐标
    GUI_POINT_T orig_point3 = {0}; // 对应于原始区域的右下角坐标
    GUI_POINT_T orig_point4 = {0}; // 对应于原始区域的左下角坐标
    GUI_POINT_T tmp_point = {0};   // 临时点
    int16 orig_rect_width = 0;
    int16 orig_rect_height = 0;

    if(PNULL != orig_rect_ptr && PNULL != result_rect_ptr && CheckRectValid(*orig_rect_ptr))
    {
        // 初始化宽度和高度，目的是为了计算四个点的原始坐标
        orig_rect_width = orig_rect_ptr->right - orig_rect_ptr->left + 1;
        orig_rect_height = orig_rect_ptr->bottom - orig_rect_ptr->top + 1;
        // 初始化坐标值
        orig_point1.x = orig_rect_ptr->left;
        orig_point1.y = orig_rect_ptr->top;
        orig_point2.x = orig_rect_ptr->right;
        orig_point2.y = orig_rect_ptr->top;
        orig_point3.x = orig_rect_ptr->right;
        orig_point3.y = orig_rect_ptr->bottom;
        orig_point4.x = orig_rect_ptr->left;
        orig_point4.y = orig_rect_ptr->bottom;

        // 在x和y方向上平移之后的坐标
        orig_point1.x = orig_point1.x - x_offset;
        orig_point1.y = orig_point1.y - y_offset;
        orig_point2.x = orig_point2.x - x_offset;
        orig_point2.y = orig_point2.y - y_offset;
        orig_point3.x = orig_point3.x - x_offset;
        orig_point3.y = orig_point3.y - y_offset;
        orig_point4.x = orig_point4.x - x_offset;
        orig_point4.y = orig_point4.y - y_offset;
        // 按角度旋转之后的坐标
        switch(angle)
        {
        case ANGLE_0:
            // 旋转角度为0的时候坐标不变
            result_rect_ptr->left = orig_point1.x;
            result_rect_ptr->top = orig_point1.y;
            result_rect_ptr->right = orig_point3.x;
            result_rect_ptr->bottom = orig_point3.y;
            break;
            
        case ANGLE_90:
            // 旋转90度时，区域的变成了由2、4两点确定
            tmp_point = orig_point2;
            orig_point2.x = tmp_point.y;
            orig_point2.y = -tmp_point.x;
            tmp_point = orig_point4;
            orig_point4.x = tmp_point.y;
            orig_point4.y = -tmp_point.x;
            result_rect_ptr->left = orig_point2.x;
            result_rect_ptr->top = orig_point2.y;
            result_rect_ptr->right = orig_point4.x;
            result_rect_ptr->bottom = orig_point4.y;
            break;
            
        case ANGLE_180:
            // 旋转180度，区域变为由3、1确定
            tmp_point = orig_point1;
            orig_point1.x = -tmp_point.x;
            orig_point1.y = -tmp_point.y;
            tmp_point = orig_point3;
            orig_point3.x = -tmp_point.x;
            orig_point3.y = -tmp_point.y;
            result_rect_ptr->left = orig_point3.x;
            result_rect_ptr->top = orig_point3.y;
            result_rect_ptr->right = orig_point1.x;
            result_rect_ptr->bottom = orig_point1.y;
            break;
            
        case ANGLE_270:
            // 旋转270度，区域由4、2确定
            tmp_point = orig_point4;
            orig_point4.x = -tmp_point.y;
            orig_point4.y = tmp_point.x;
            tmp_point = orig_point2;
            orig_point2.x = -tmp_point.y;
            orig_point2.y = tmp_point.x;
            result_rect_ptr->left = orig_point4.x;
            result_rect_ptr->top = orig_point4.y;
            result_rect_ptr->right = orig_point2.x;
            result_rect_ptr->bottom = orig_point2.y;
            break;
            
        default:
            SCI_ASSERT(0); /*assert verified*/ // 不支持其他角度的旋转
            break;
        }
        reslut = TRUE;
    }

    return reslut;
}

/*****************************************************************************/
// 	Description : copy buffer, 原始buffer和目标buffer的大小一定相同，否则会出现异常
//	Global resource dependence : none
//  Author: xiaoqing.lu
//	Note: 按顺时针方向旋转
/*****************************************************************************/
LOCAL BOOLEAN RotateBufferData(
                               GUISTR_BUF_TYPE_T type,
                               const void *src_data_ptr, 
                               int16  src_x,      // 原始起始点x坐标
                               int16  src_y,      // 原始起始点y坐标
                               uint16 src_width,  // 原始buffer的宽度
                               uint16 src_heigth, // 原始buffer的高度
                               void *dst_data_ptr,
                               int16  dst_x,      // 目标起始点x坐标
                               int16  dst_y,      // 目标起始点y坐标
                               uint16 dst_width,  // 目标buffer的宽度
                               uint16 dst_heigth, // 目标buffer的高度
                               GUI_ANGLE_E angle,  // 旋转角度
                               uint16 cp_width,  // 被copy的宽度
                                uint16 cp_heigth // 被copy的高度
                               )
{
    BOOLEAN result = FALSE;
    int16 i = 0, j = 0;
    
    //SCI_ASSERT(PNULL != src_data_ptr && PNULL != dst_data_ptr); /*assert verified*/
    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != src_data_ptr && PNULL != dst_data_ptr); /*assert verified*/
    if ((PNULL == src_data_ptr) || (PNULL == dst_data_ptr))
    {
        return FALSE;
    }
    
    //只支持565和888的buffer旋转
    if ( GUISTR_BUF_RGB565 != type
        && GUISTR_BUF_ARGB888 != type )
    {
        return result;
    }

    //只支持565和888的buffer旋转
    if ( GUISTR_BUF_RGB565 == type )
    {
        const GUI_COLOR_T *tmp_src_ptr = PNULL;
        const GUI_COLOR_T *src_ptr = (const GUI_COLOR_T *)src_data_ptr;
        GUI_COLOR_T* dst_ptr = (GUI_COLOR_T*)dst_data_ptr;
        
        // 源buffer的起始位置
        tmp_src_ptr = src_ptr + src_y*src_width + src_x;
        
        switch(angle)
        {
        case ANGLE_0:
            for(i = 0; i < cp_heigth; i++)
            {
                for (j = 0; j < cp_width; j++)
                {
                    //if ( tmp_src_ptr[j] )
                    {
                        dst_ptr [( dst_x + j ) + ( dst_y + i ) * dst_width] = tmp_src_ptr[j];
                    }
                }
                
                tmp_src_ptr += (src_width);
            }
            break;
            
        case ANGLE_90:
            // 旋转之后，目标起始点x坐标，y坐标不变
            dst_x +=cp_heigth - 1;
            for(i = 0; i < cp_heigth; i++)
            {
                for (j = 0; j < cp_width; j++)
                {
                    dst_ptr [( dst_x - i ) + ( dst_y + j ) * dst_width] = tmp_src_ptr[j];
                }
                
                tmp_src_ptr += (src_width);
            }
            break;
            
        case ANGLE_180:
            // 旋转之后，x，y坐标均发生变化
            dst_x += cp_width - 1;
            dst_y += cp_heigth - 1;
            for(i = 0; i < cp_heigth; i++)
            {
                for (j = 0; j < cp_width; j++)
                {
                    //if ( tmp_src_ptr[j] )
                    {
                        dst_ptr [( dst_x - j ) + ( dst_y - i ) * dst_width] = tmp_src_ptr[j];
                    }
                }
                
                tmp_src_ptr += (src_width);
            }
            break;
            
        case ANGLE_270:
            // 旋转之后，y坐标发生变化，x坐标不变
            dst_y += cp_width - 1;
            for(i = 0; i < cp_heigth; i++)
            {
                for (j = 0; j < cp_width; j++)
                {
                    //if ( tmp_src_ptr[j] )
                    {
                        dst_ptr [( dst_x + i ) + ( dst_y - j ) * dst_width] = tmp_src_ptr[j];
                    }
                }
                
                tmp_src_ptr += (src_width);
            }
            break;
            
        default:
            SCI_ASSERT(0);  /*assert verified*/// 不支持其他角度旋转
            break;
        }
    }
    else
    {
        const uint32 *tmp_src_ptr = PNULL;
        const uint32 *src_ptr = (const uint32 *)src_data_ptr;
        uint32* dst_ptr = (uint32*)dst_data_ptr;
        
        // 源buffer的起始位置
        tmp_src_ptr = src_ptr + src_y*src_width + src_x;
        
        switch(angle)
        {
        case ANGLE_0:
            for(i = 0; i < cp_heigth; i++)
            {
                for (j = 0; j < cp_width; j++)
                {
                    //if ( tmp_src_ptr[j] )
                    {
                        dst_ptr [( dst_x + j ) + ( dst_y + i ) * dst_width] = tmp_src_ptr[j];
                    }
                }
                
                tmp_src_ptr += (src_width);
            }
            break;
            
        case ANGLE_90:
            // 旋转之后，目标起始点x坐标，y坐标不变
            dst_x +=cp_heigth - 1;
            for(i = 0; i < cp_heigth; i++)
            {
                for (j = 0; j < cp_width; j++)
                {
                    //if ( tmp_src_ptr[j] )
                    {
                        dst_ptr [( dst_x - i ) + ( dst_y + j ) * dst_width] = tmp_src_ptr[j];
                    }
                    
                }
                
                tmp_src_ptr += (src_width);
            }
            break;
            
        case ANGLE_180:
            // 旋转之后，x，y坐标均发生变化
            dst_x += cp_width - 1;
            dst_y += cp_heigth - 1;
            for(i = 0; i < cp_heigth; i++)
            {
                for (j = 0; j < cp_width; j++)
                {
                    //if ( tmp_src_ptr[j] )
                    {
                        dst_ptr [( dst_x - j ) + ( dst_y - i ) * dst_width] = tmp_src_ptr[j];
                    }
                }
                
                tmp_src_ptr += (src_width);
            }
            break;
            
        case ANGLE_270:
            // 旋转之后，y坐标发生变化，x坐标不变
            dst_y += cp_width - 1;
            for(i = 0; i < cp_heigth; i++)
            {
                for (j = 0; j < cp_width; j++)
                {
                    //if ( tmp_src_ptr[j] )
                    {
                        dst_ptr [( dst_x + i ) + ( dst_y - j ) * dst_width] = tmp_src_ptr[j];
                    }
                }
                
                tmp_src_ptr += (src_width);
            }
            break;
            
        default:
            SCI_ASSERT(0);  /*assert verified*/ // 不支持其他角度旋转
            break;
        }
    }
    
    return result;
}

/*****************************************************************************/
// 	Description : 往buffer上画文本信息，所画的区域为
//	Global resource dependence : none
//  Author: xiaoqing.lu
//	Note: 起始点的位置相对于buffer的原点
/*****************************************************************************/
PUBLIC GUI_RECT_T GUISTR_DrawTextInRect( 
                                        GUISTR_BUF_T          *des_buf_ptr,
                                        const GUI_RECT_T      *disp_rect_ptr,       //the fixed display area
                                        const GUI_RECT_T      *clip_rect_ptr,       //用户要剪切的实际区域
                                        const MMI_STRING_T    *str_ptr,
                                        GUISTR_STYLE_T        *style_ptr,
                                        GUISTR_STATE_T        state,
                                        int8                  dir
                                        )
{
    GUI_RECT_T  box = {0};
    GUI_RECT_T  buff_rect = {0};
    GUI_RECT_T  orig_clip_rect = {0};

    // 入参检测
    if (PNULL == des_buf_ptr || PNULL == disp_rect_ptr 
        || PNULL == str_ptr || PNULL == style_ptr 
        || PNULL == clip_rect_ptr)
    {
        return orig_clip_rect;
    }

    if ( PNULL == str_ptr->wstr_ptr
        || 0 == str_ptr->wstr_len )
    {
        return orig_clip_rect;
    }

    // 检查区域是否有效，如果传入的区域无效，则会直接返回
    if (GUI_IsInvalidRect(*disp_rect_ptr) || GUI_IsInvalidRect(*clip_rect_ptr))
    {
        return orig_clip_rect;
    }

    buff_rect.left = buff_rect.top = 0;
    buff_rect.right = des_buf_ptr->width - 1;
    buff_rect.bottom = des_buf_ptr->height - 1;
    
    orig_clip_rect = GetTrimRect(*disp_rect_ptr, *clip_rect_ptr);
    orig_clip_rect = GetTrimRect(orig_clip_rect, buff_rect);

    if ((state&GUISTR_STATE_ANGLE) && ANGLE_0 != style_ptr->angle)
    {
        // 对于0度的，不旋转，则不会走该分支
        void *tmp_buffer = PNULL;
        GUI_RECT_T  disp_rect = {0};
        GUI_RECT_T  new_buff_rect = {0};
        GUISTR_BUF_T new_des_buf = {0};
        uint16 temp_width = 0;
        uint16 temp_height = 0;
        uint32 temp_size = 0;

        //只支持565和888的buffer旋转
        if ( GUISTR_BUF_RGB565 != des_buf_ptr->type
            && GUISTR_BUF_ARGB888 != des_buf_ptr->type )
        {
            return orig_clip_rect;
        }

        if ( GUISTR_BUF_RGB565 == des_buf_ptr->type )
        {
            temp_size = (orig_clip_rect.right - orig_clip_rect.left + 1)*(orig_clip_rect.bottom - orig_clip_rect.top + 1)*(int16)sizeof(GUI_COLOR_T);
            
        }
        else
        {
            temp_size = (orig_clip_rect.right - orig_clip_rect.left + 1)*(orig_clip_rect.bottom - orig_clip_rect.top + 1)*(int16)sizeof(uint32);
        }

        // 申请一段新的buffer
        tmp_buffer = SCI_ALLOC_APP( temp_size );
        SCI_MEMSET( tmp_buffer, 0, temp_size );

        new_des_buf.buffer = tmp_buffer;
        new_des_buf.type = des_buf_ptr->type;

        // 这里计算旋转之后的显示区域
        switch(style_ptr->angle)
        {
        case ANGLE_0: // 本case代码测试所用，因为当角度为0的时候，不会走这个分支
            
            new_des_buf.mem_width = new_des_buf.width = orig_clip_rect.right - orig_clip_rect.left + 1;
            new_des_buf.height = orig_clip_rect.bottom - orig_clip_rect.top + 1;
            // buffer rect
            new_buff_rect.left = new_buff_rect.right = 0;
            new_buff_rect.right = orig_clip_rect.right - orig_clip_rect.left;
            new_buff_rect.bottom = orig_clip_rect.bottom - orig_clip_rect.top;
            // display rect
            CalculateRotateRect(disp_rect_ptr, orig_clip_rect.left, orig_clip_rect.top, ANGLE_0, &disp_rect);

            if ( GUISTR_BUF_RGB565 == des_buf_ptr->type )
            {
                // 这里应该先把背景拷贝到buffer上
                RotateBufferData( des_buf_ptr->type, des_buf_ptr->buffer, orig_clip_rect.left, orig_clip_rect.top, des_buf_ptr->width, des_buf_ptr->height,
                    new_des_buf.buffer, 0, 0, new_des_buf.width, new_des_buf.height, ANGLE_0, new_des_buf.width, new_des_buf.height);
            }
            break;
            
        case ANGLE_90:
            
            new_des_buf.mem_width = new_des_buf.width = orig_clip_rect.bottom - orig_clip_rect.top + 1;
            new_des_buf.height = orig_clip_rect.right - orig_clip_rect.left + 1;
            // buffer rect
            new_buff_rect.left = new_buff_rect.right = 0;
            new_buff_rect.right = orig_clip_rect.bottom - orig_clip_rect.top;
            new_buff_rect.bottom = orig_clip_rect.right - orig_clip_rect.left;
            // display rect
            CalculateRotateRect(disp_rect_ptr, orig_clip_rect.right, orig_clip_rect.top, ANGLE_90, &disp_rect);

            if ( GUISTR_BUF_RGB565 == des_buf_ptr->type )
            {
                // 这里应该先把背景拷贝到buffer上, 旋转角度为270，因为拷贝回来的时候的旋转角度为90度
                RotateBufferData( des_buf_ptr->type, des_buf_ptr->buffer, orig_clip_rect.left, orig_clip_rect.top, des_buf_ptr->width, des_buf_ptr->height,
                    new_des_buf.buffer, 0, 0, new_des_buf.width, new_des_buf.height, ANGLE_270, new_des_buf.height, new_des_buf.width);
            }
            break;
            
        case ANGLE_180:
            
            new_des_buf.mem_width = new_des_buf.width = orig_clip_rect.right - orig_clip_rect.left + 1;
            new_des_buf.height = orig_clip_rect.bottom - orig_clip_rect.top + 1;
            // buffer rect
            new_buff_rect.left = new_buff_rect.right = 0;
            new_buff_rect.right = orig_clip_rect.right - orig_clip_rect.left;
            new_buff_rect.bottom = orig_clip_rect.bottom - orig_clip_rect.top;
            CalculateRotateRect(disp_rect_ptr, orig_clip_rect.right, orig_clip_rect.bottom, ANGLE_180, &disp_rect);

            if ( GUISTR_BUF_RGB565 == des_buf_ptr->type )
            {
                // 这里应该先把背景拷贝到buffer上, 旋转角度为180，因为拷贝回来的时候的旋转角度为180度
                RotateBufferData( des_buf_ptr->type, des_buf_ptr->buffer, orig_clip_rect.left, orig_clip_rect.top, des_buf_ptr->width, des_buf_ptr->height,
                    new_des_buf.buffer, 0, 0, new_des_buf.width, new_des_buf.height, ANGLE_180, new_des_buf.width, new_des_buf.height);
            }
            break;
            
        case ANGLE_270:
            
            new_des_buf.mem_width = new_des_buf.width = orig_clip_rect.bottom - orig_clip_rect.top + 1;
            new_des_buf.height = orig_clip_rect.right - orig_clip_rect.left + 1;
            // buffer rect
            new_buff_rect.left = new_buff_rect.right = 0;
            new_buff_rect.right = orig_clip_rect.bottom - orig_clip_rect.top;
            new_buff_rect.bottom = orig_clip_rect.right - orig_clip_rect.left;
            CalculateRotateRect(disp_rect_ptr, orig_clip_rect.left, orig_clip_rect.bottom, ANGLE_270, &disp_rect);

            if ( GUISTR_BUF_RGB565 == des_buf_ptr->type )
            {
                // 这里应该先把背景拷贝到buffer上, 旋转角度为90，因为拷贝回来的时候的旋转角度为270度
                RotateBufferData( des_buf_ptr->type, des_buf_ptr->buffer, orig_clip_rect.left, orig_clip_rect.top, des_buf_ptr->width, des_buf_ptr->height,
                    new_des_buf.buffer, 0, 0, new_des_buf.width, new_des_buf.height, ANGLE_90, new_des_buf.height, new_des_buf.width);
            }
            break;
            
        default:
            SCI_ASSERT(0); /*assert verified*/ // 不支持其他角度
            break;
        }

        // 将数据画到临时的buffer上
        box = DrawTextInRect(style_ptr, &new_des_buf, &disp_rect, &new_buff_rect, str_ptr, state, dir);

        // z这里需要从临时buffer上copy数据到目的buffer
        RotateBufferData( des_buf_ptr->type, new_des_buf.buffer, 0, 0, new_des_buf.width, new_des_buf.height, 
            des_buf_ptr->buffer, orig_clip_rect.left, orig_clip_rect.top, des_buf_ptr->width, des_buf_ptr->height, 
            style_ptr->angle, new_des_buf.width, new_des_buf.height);

        temp_width  = box.right - box.left;
        temp_height = box.bottom - box.top;

        switch(style_ptr->angle)
        {
        case ANGLE_0: // 本case代码测试所用，因为当角度为0的时候，不会走这个分支
            box.left   = disp_rect_ptr->left;
            box.top    = disp_rect_ptr->top;
            box.right  = box.left + temp_width;
            box.bottom = box.top + temp_height;
            break;
            
        case ANGLE_90:
            box.left   = disp_rect_ptr->right - temp_height;
            box.top    = disp_rect_ptr->top;
            box.right  = box.left + temp_height;
            box.bottom = box.top + temp_width;
            break;
            
        case ANGLE_180:
            box.left   = disp_rect_ptr->right - temp_width;
            box.top    = disp_rect_ptr->bottom - temp_height;
            box.right  = box.left + temp_width;
            box.bottom = box.top + temp_height;
            break;
            
        case ANGLE_270:
            box.left   = disp_rect_ptr->left;
            box.top    = disp_rect_ptr->bottom - temp_width;
            box.right  = box.left + temp_height;
            box.bottom = box.top + temp_width;
            break;
            
        default:
            SCI_ASSERT(0); /*assert verified*/ // 不支持其他角度
            break;
        }

        // 释放内存
        SCI_FREE(tmp_buffer);
    }
    else
    {
        box = DrawTextInRect(style_ptr, des_buf_ptr,disp_rect_ptr, clip_rect_ptr, str_ptr, state, dir);
    }

    orig_clip_rect = GetTrimRect(orig_clip_rect, box);

    if ( GUI_IsInvalidRect( orig_clip_rect ) )
    {
        SCI_MEMSET( &orig_clip_rect, 0, sizeof(GUI_RECT_T) );
    }

    return orig_clip_rect;
}


/*****************************************************************************/
// 	Description : 往LCD上画字符串
//	Global resource dependence : none
//  Author: xiaoqing.lu
//	Note: 
/*****************************************************************************/
PUBLIC void GUISTR_DrawTextToLCDByOffset( 
                                         const GUI_LCD_DEV_INFO *dev_info_ptr,
                                         const GUI_RECT_T      *disp_rect_ptr,       //the fixed display area
                                         const GUI_RECT_T      *clip_rect_ptr,       //the fixed display area
                                         int16                 x_offset, // x方向向左移动还是向右移动由系统决定
                                         int16                 y_offset, // y方向向上移动为负值,向下移动为正值
                                         const MMI_STRING_T    *str_ptr,
                                         GUISTR_STYLE_T        *style_ptr,
                                         GUISTR_STATE_T        state
                                         )
{
    GUI_RECT_T      box = {0};
    GUI_RECT_T      disp_rect = {0};
    GUI_RECT_T      clip_rect = {0};
    GUISTR_BUF_T    des_buf_data = {0};
    GUILCD_DRAW_DC_T draw_dc = {0};

    // 入参检测
    if (PNULL == dev_info_ptr || PNULL == disp_rect_ptr || PNULL == str_ptr || PNULL == style_ptr)
    {
        return;
    }

    if ( PNULL == str_ptr->wstr_ptr
        || 0 == str_ptr->wstr_len )
    {
        return;
    }

    if (!LCD_PrepareDrawParam(dev_info_ptr, &draw_dc))
    {
        return;
    }
    des_buf_data.buffer = draw_dc.lcd_buffer_ptr;
    des_buf_data.mem_width = draw_dc.mem_width;
    des_buf_data.width = draw_dc.width;
    des_buf_data.height = draw_dc.height;
    des_buf_data.type = ConvertStringBufferType( draw_dc.data_type );

#ifdef UI_MULTILAYER_SUPPORT
    UILAYER_BltLowerLayerWhenTransparent((GUI_LCD_DEV_INFO *)dev_info_ptr, disp_rect_ptr);
    disp_rect_ptr = LCD_ConvertDispRect(GUILCD_CONVERT_POS_LCD2DISP, (GUI_LCD_DEV_INFO *)dev_info_ptr, disp_rect_ptr, &disp_rect);
    if (PNULL != clip_rect_ptr)
    {
        clip_rect_ptr = LCD_ConvertDispRect(GUILCD_CONVERT_POS_LCD2DISP, (GUI_LCD_DEV_INFO *)dev_info_ptr, clip_rect_ptr, &clip_rect);
    }
#endif

    // 将字符刷新到buffer内
    box = GUISTR_DrawTextByOffset(&des_buf_data, disp_rect_ptr, clip_rect_ptr, x_offset, y_offset, str_ptr, style_ptr, state);
    
    // 刷新屏幕
    MMITHEME_StoreUpdateRect(dev_info_ptr, box);
}

/*****************************************************************************/
// 	Description : 往buffer上画文本信息，所画的区域为
//	Global resource dependence : none
//  Author: xiaoqing.lu
//	Note: 起始点的位置相对于buffer的原点
/*****************************************************************************/
PUBLIC GUI_RECT_T GUISTR_DrawTextByOffset( 
                                          GUISTR_BUF_T          *des_buf_ptr,
                                          const GUI_RECT_T      *disp_rect_ptr,       //the fixed display area
                                          const GUI_RECT_T      *clip_rect_ptr,       //the fixed display area
                                          int16                 x_offset, // x方向向左移动还是向右移动由系统决定
                                          int16                 y_offset, // y方向向上移动为负值,向下移动为正值
                                          const MMI_STRING_T    *str_ptr,
                                          GUISTR_STYLE_T        *style_ptr,
                                          GUISTR_STATE_T        state
                                          )
{
    GUI_RECT_T  box = {0};
    MMI_DIRECTION_SET_E direction = MMITHEME_GetDisDirection();
    GUI_RECT_T clip_rect = {0};
    GUI_RECT_T buf_rect = {0};
    GUI_RECT_T disp_rect = {0};

    // 入参检测
    if (PNULL == des_buf_ptr || PNULL == disp_rect_ptr || PNULL == str_ptr || PNULL == style_ptr)
    {
        return clip_rect;
    }

    if ( PNULL == str_ptr->wstr_ptr
        || 0 == str_ptr->wstr_len )
    {
        return clip_rect;
    }

    buf_rect.left = 0;
    buf_rect.top = 0;
    buf_rect.right = des_buf_ptr->width - 1;
    buf_rect.bottom = des_buf_ptr->height - 1;

    disp_rect = *disp_rect_ptr;
    clip_rect = GetTrimRect(disp_rect, buf_rect);

    if (PNULL != clip_rect_ptr)
    {
        clip_rect = GetTrimRect(clip_rect, *clip_rect_ptr);
    }
    
    if (PNULL != str_ptr->wstr_ptr && 0 != str_ptr->wstr_len) 
    {   
        //向右滚动, 从尾显示
        if ( MMI_DIRECTION_RIGHT == direction )
        {
            disp_rect.right += x_offset;

            state |= GUISTR_STATE_FROM_TAIL;
        }
        else
        {
            disp_rect.left -= x_offset;

            state |= GUISTR_STATE_FROM_HEAD;
        }
        
        disp_rect.top += y_offset;

        if (IsSetState(state, GUISTR_STATE_SINGLE_LINE))
        {
            // 保存
            uint16 orig_region_num = style_ptr->region_num;
            GUIFONT_REGION_STYLE_T *orig_region_style_ptr = style_ptr->region_ptr;
            uint16 tmp_region_num = 0;
            GUIFONT_REGION_STYLE_T *tmp_region_style_ptr = PNULL;
            
            // 这里需要根据输入的特殊区域重新创建一个区域，因为传进来的区域有可能会有重和的区域，而底层是不能重合的
            tmp_region_num = CreateRegionPtr(orig_region_style_ptr, orig_region_num, &tmp_region_style_ptr);
            style_ptr->region_num = tmp_region_num;
            style_ptr->region_ptr = tmp_region_style_ptr;

            box = DrawTextInLine(style_ptr, des_buf_ptr, &disp_rect, &clip_rect, str_ptr, state, TEXT_DIR_AUTO, FALSE);
            
            // 创建时申请的内存要释放
            DestoryPtr((void*)tmp_region_style_ptr);
            // 恢复，为了不改变用户的输入参数，这里需要恢复
            style_ptr->region_num = orig_region_num;
            style_ptr->region_ptr = orig_region_style_ptr;
        }
        else
        {
            box = DrawTextInRect(style_ptr, des_buf_ptr, &disp_rect, &clip_rect, str_ptr, state, TEXT_DIR_AUTO);
        }
    }

    return box;
}

/*******************************************************************************************/
// 	Description : 获得字符串的宽度
//	Global resource dependence :
//  Author: xiaoqing.lu
//	Note: 注意该接口与guifont中的GUI_GetStringWidth及GUI_GetStringWidthEx的区别为该接口的参数
/*********************************************************************************************/
PUBLIC uint16 GUISTR_GetStringWidth(
                                    const GUISTR_STYLE_T *style_ptr,    
                                    const MMI_STRING_T   *str_ptr,
                                    GUISTR_STATE_T       state
                                    )
{
    GUISTR_INFO_T str_info = {0};

    GUISTR_GetStringInfo( style_ptr, str_ptr, state, &str_info );

    return str_info.width;
}

/*******************************************************************************************/
// 	Description : 获得字符串的宽高
//	Global resource dependence :
//  Author: xiaoqing.lu
//	Note: 注意该接口与guifont中的GUI_GetStringWidth及GUI_GetStringWidthEx的区别为该接口的参数
/*********************************************************************************************/
PUBLIC BOOLEAN GUISTR_GetStringInfo(
                                    const GUISTR_STYLE_T *style_ptr,    
                                    const MMI_STRING_T   *str_ptr,
                                    GUISTR_STATE_T       state,
                                    GUISTR_INFO_T        *info_ptr //out
                                    )
{
    SPML_TEXT_METRICS_T metrics = {0};
    SPML_TEXT_PARAM_T param = {0};
    SPML_TEXT_REGION_T *text_region_ptr = PNULL;
    // 参数检查
    if ( PNULL == style_ptr 
        || PNULL == str_ptr 
        || PNULL == str_ptr->wstr_ptr 
        || 0 == str_ptr->wstr_len
        || PNULL == info_ptr ) 
    {
        return FALSE;
    }

    param.glyph_space = style_ptr->char_space;
    param.word_space  = style_ptr->word_space;
    param.dir = TEXT_DIR_AUTO;
    param.size = GUI_ConvertGUIFontToSize( style_ptr->font );
    
    if (IsSetState(state, GUISTR_STATE_EFFECT)) // 设置了effect状态时，才转化响应状态
    {
        ConvertFontEffect( style_ptr->effect, style_ptr->font_color, style_ptr->edge_color, &param );
    }
    
    // 设置文本其他状态
    ConvertTextState(state, &param.format);
    
    if ( param.format & ( TEXT_FORMAT_EDGE | TEXT_FORMAT_ITALIC | TEXT_FORMAT_BOLD | TEXT_FORMAT_WORDSPACE )
        || style_ptr->region_num > 0)
    {
        // 设置特殊字符，特殊字符串能影响字符串的宽度
        if ( style_ptr->region_num > 0 )
        {
            int32 i = 0;
            
            text_region_ptr = SCI_ALLOC_APP( style_ptr->region_num * sizeof(SPML_TEXT_REGION_T) );
            
            SCI_MEMSET( text_region_ptr, 0, (style_ptr->region_num * sizeof(SPML_TEXT_REGION_T)) );
            
            for ( i = 0; i < style_ptr->region_num; i++ )
            {
                text_region_ptr[i].start = style_ptr->region_ptr[i].start;
                text_region_ptr[i].end   = style_ptr->region_ptr[i].start + style_ptr->region_ptr[i].length;
                
                // 颜色, 大小, format根据需求填写, 暂时支持不同字体
                // text_region_ptr[i].style.color = param.font_color;
                text_region_ptr[i].style.size  = param.size;
                ConvertTextState(style_ptr->region_ptr[i].state, &text_region_ptr[i].style.format);
                
                text_region_ptr[i].style.color = style_ptr->region_ptr[i].font_color;
            }
            
            param.region_num = style_ptr->region_num;
            param.region_ptr = text_region_ptr;
        }
        // 获取字符串的宽度
        SPMLAPI_GetTextMetrics( &param, str_ptr->wstr_ptr, str_ptr->wstr_len, &metrics );
        
        // 释放特殊字符内存
        if ( PNULL != text_region_ptr )
        {
            SCI_FREE( text_region_ptr );
        }

        info_ptr->width  = metrics.width;
        info_ptr->height = metrics.height;
    }
    else
    {
        info_ptr->width = GUI_GetStringWidthEx( style_ptr->font, str_ptr->wstr_ptr, str_ptr->wstr_len, style_ptr->char_space );
        info_ptr->height = GUI_GetStringHeightEx ( style_ptr->font, str_ptr->wstr_ptr, str_ptr->wstr_len);//param.size;
    }
    
    return TRUE;
}

/*******************************************************************************************/
// 	Description : 计算节点数，创建数组
//	Global resource dependence :
//  Author: xiaoqing.lu
//	Note: 
/*********************************************************************************************/
LOCAL void DestoryPtr(
                      void *node_ptr
                      )
{
    if (PNULL != node_ptr)
    {
        SCI_FREE(node_ptr);
    }

    return;
}

#if 0
/*******************************************************************************************/
// 	Description : 计算节点数，创建数组
//	Global resource dependence :
//  Author: xiaoqing.lu
//	Note: 
/*********************************************************************************************/
LOCAL int32 CreateNodeNumberPtr(
                                 GUIFONT_REGION_STYLE_T *region_ptr, 
                                 int32 region_num,
                                 int32 **node_ptr
                                 )
{
    int32 i = 0;
    int32 index = 0;
    int32 tmp_node_num = 0;
    int32 *tmp_node_ptr = PNULL;

    if (PNULL != region_ptr && 0 < region_num)
    {
        // 一共2n个节点
        tmp_node_num = region_num*2;

        tmp_node_ptr = (int32 *)SCI_ALLOC_APP((sizeof(int32)*(uint32)region_num*2));
        SCI_ASSERT(PNULL != tmp_node_ptr); /*assert verified*/
        SCI_MEMSET(tmp_node_ptr, 0, (sizeof(int32)*(uint32)region_num*2));
        for (i = 0; i < region_num; i++)
        {
            tmp_node_ptr[index] = region_ptr[i].start;
            index++;
            tmp_node_ptr[index] = region_ptr[i].start + region_ptr[i].length - 1;
            index++;
        }
    }
    *node_ptr = tmp_node_ptr;

    return tmp_node_num;
}

/*******************************************************************************************/
// 	Description : 对数组进行排序
//	Global resource dependence :
//  Author: xiaoqing.lu
//	Note: 
/*********************************************************************************************/
LOCAL void BubbleSortNumberPtr(
                               int32 *node_ptr,
                               int32 node_num
                               )
{
    int32 j = 0;
    int32 k = 0;
    int32 h = 0;
    int32 t = 0;

    if (PNULL != node_ptr && 0 != node_num)
    {
        for (h = node_num - 1; h > 0; h = k) // 循环到没有比较的范围
        {
            for (j = 0, k = 0; j < h; j++) // 每次预置k = 0，循环扫描后更新k
            {
                if (node_ptr[j] > node_ptr[j+1]) // 大的放在后面，小的放在前面
                {
                    t = node_ptr[j];
                    node_ptr[j] = node_ptr[j+1];
                    node_ptr[j+1] = t; // 完成交换
                }
                k = j; // 保存最后下沉的位置。这样k后面的都是排序好了的
            }
        }
    }

    return;
}


/*******************************************************************************************/
// 	Description : 删除相同的节点
//	Global resource dependence :
//  Author: xiaoqing.lu
//	Note: 
/*********************************************************************************************/
LOCAL int32 DeleteSameNode(
                           int32 *node_ptr,
                           int32 node_num
                           )
{
    int32 i = 0;
    int32 j = 0;
    int32 tmp_node_num = 0;

    if (PNULL != node_ptr && 0 != node_num)
    {
        tmp_node_num = node_num;
        for (i = 0; i < node_num - 1; i++)
        {
            for (j = i + 1; j < node_num; j++)
            {
                if (node_ptr[i] == node_ptr[j])
                {
                    // 向前移动
                    SCI_MEMCPY(&node_ptr[j], &node_ptr[j+1], ((uint32)(node_num - j - 1)*sizeof(int32)));
                    node_num--;
                }
            }
        }
    }

    return node_num;
}


/*******************************************************************************************/
// 	Description : 设置特殊区域的值,除了start位置和length长度之外的其他值
//	Global resource dependence :
//  Author: xiaoqing.lu
//	Note: 
/*********************************************************************************************/
LOCAL void SetRegionInfo(
                         GUIFONT_REGION_STYLE_T *in_region_ptr, 
                         int32 region_num,
                         GUIFONT_REGION_STYLE_T *out_region_ptr
                         )
{
    int32 j = 0;

    if (PNULL != in_region_ptr && 0 != region_num && PNULL != out_region_ptr)
    {
        for (j = 0; j < region_num; j++)
        {
            // 设置状态，在哪个区域就“|”哪个状态
            if (out_region_ptr->start >= in_region_ptr[j].start 
                && out_region_ptr->start < in_region_ptr[j].start + in_region_ptr[j].length)
            {
                out_region_ptr->font_color = in_region_ptr[j].font_color; // 后一个颜色覆盖前一个颜色
                out_region_ptr->state |= in_region_ptr[j].state;
            }
        }
    }

    return;
}

/*******************************************************************************************/
// 	Description : 计算节点数，创建数组
//	Global resource dependence :
//  Author: xiaoqing.lu
//	Note: 
/*********************************************************************************************/
LOCAL int32 CreateRegionPtr(
                            GUIFONT_REGION_STYLE_T *in_region_ptr, 
                            int32 region_num,
                            GUIFONT_REGION_STYLE_T **out_region_ptr
                            )
{
    int32 i = 0;
    int32 *node_ptr = PNULL;
    int32 node_num = 0;
    int32 tmp_node_num = 0;
    GUIFONT_REGION_STYLE_T *tmp_region_ptr = PNULL;

    // 1创建node_ptr
    node_num = CreateNodeNumberPtr(in_region_ptr, region_num, &node_ptr);
    
    // 2对node 进行排序
    BubbleSortNumberPtr(node_ptr, node_num);
    
    // 3删除相同节点
    node_num = DeleteSameNode(node_ptr, node_num);

    // 创建region ptr
    if (PNULL != in_region_ptr && 0 < region_num && PNULL != node_ptr && node_num > 0)
    {
        // n个节点，n个区域，tmp_node_ptr记录每个点的状态
        tmp_node_num = node_num; 

        tmp_region_ptr = (GUIFONT_REGION_STYLE_T*)SCI_ALLOC_APP(sizeof(GUIFONT_REGION_STYLE_T)*(uint32)tmp_node_num);
        SCI_ASSERT(PNULL != tmp_region_ptr); /*assert verified*/
        SCI_MEMSET(tmp_region_ptr, 0, (sizeof(GUIFONT_REGION_STYLE_T)*(uint32)tmp_node_num));

        // 记录每个点的状态和其他信息，除了长度
        for ( i = 0; i < tmp_node_num; i++)
        {
            tmp_region_ptr[i].start = node_ptr[i];
            SetRegionInfo(in_region_ptr, region_num, &tmp_region_ptr[i]);
        }

        if (1 == node_num)
        {
            tmp_region_ptr[0].length = 1;
        }
        else
        {
            // 当多于一点的时候，计算长度，根据每个点的状态
            for ( i = 0; i < node_num - 1; i++)
            {
                if (tmp_region_ptr[i].state == tmp_region_ptr[i+1].state)
                {
                    // 如果状态相同
                    tmp_region_ptr[i].length = tmp_region_ptr[i + 1].start + 1 - tmp_region_ptr[i].start;
                    // 修改节点起始值
                    tmp_region_ptr[i+1].start = tmp_region_ptr[i + 1].start + 1;
                    // 恢复初值
                    tmp_region_ptr[i+1].font_color = 0xffff;
                    tmp_region_ptr[i+1].state = 0;
                    // 重新赋值
                    SetRegionInfo(in_region_ptr, region_num, &tmp_region_ptr[i+1]);
                    // 当最后一个节点相同的时候，区域就少一个
                    if (i == node_num - 2)
                    {
                        tmp_node_num = node_num - 1;
                    }
                }
                else
                {
                    // 如果状态不相同
                    tmp_region_ptr[i].length = tmp_region_ptr[i + 1].start - tmp_region_ptr[i].start;
                    // 倒数第二个不等于最后一个的时候
                    if (i == node_num - 2)
                    {
                        tmp_region_ptr[i + 1].start = tmp_region_ptr[i].start + tmp_region_ptr[i].length;
                        // 恢复初值
                        tmp_region_ptr[i+1].font_color = 0xffff;
                        tmp_region_ptr[i+1].state = 0;
                        // 重新赋值
                        SetRegionInfo(in_region_ptr, region_num, &tmp_region_ptr[i+1]);
                        tmp_region_ptr[i + 1].length = 1;
                    }
                }
            }
        }
    }

    DestoryPtr((void*)node_ptr);
    node_ptr = PNULL;
    *out_region_ptr = tmp_region_ptr;

    return tmp_node_num;
}
#endif


/*==============================================================================
Description: CombineNode
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:合并相同的连续的特效区域
==============================================================================*/
LOCAL int32 CombineNode(GUISTRING_REGION_NODE_T **head_pptr)
{
    GUISTRING_REGION_NODE_T *cur_node_ptr = PNULL;
    BOOLEAN is_same_state = FALSE;
    BOOLEAN is_series = FALSE;
    BOOLEAN is_same_color = FALSE;
    int16 cur_next = 0;
    int32 node_cnt = 0;

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != head_pptr); /*assert verified*/
    if (PNULL == head_pptr)
    {
        return 0;
    }

    if (PNULL == *head_pptr) return 0;

    cur_node_ptr = *head_pptr;
    node_cnt = 1;

    while (PNULL != cur_node_ptr)
    {
        if (PNULL == cur_node_ptr->next) break;

        is_same_state = (cur_node_ptr->region.state == cur_node_ptr->next->region.state);
        cur_next = cur_node_ptr->region.start + cur_node_ptr->region.length;
        //force check
        SCI_ASSERT(cur_next <= cur_node_ptr->next->region.start); /*assert verified*/
        is_series = (cur_next == cur_node_ptr->next->region.start);
        is_same_color = (cur_node_ptr->region.font_color== cur_node_ptr->next->region.font_color);

        if (is_same_state && is_series && is_same_color)
        {
            cur_node_ptr->region.length += cur_node_ptr->next->region.length;
            
            RemoveNodePtr(cur_node_ptr->next);
        }
        else
        {
            cur_node_ptr = cur_node_ptr->next;
            node_cnt++;
        }
    }

    return node_cnt;
}


/*==============================================================================
Description: InsertNode
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:向特效链表中插入一个新的特效结点
==============================================================================*/
LOCAL int32 InsertNode(
    GUISTRING_REGION_NODE_T **head_pptr,
    GUIFONT_REGION_STYLE_T const *region_ptr
)
{
    GUISTRING_REGION_NODE_T *cur_node_ptr = PNULL;
    GUISTRING_REGION_NODE_T *prev_ptr = PNULL;
    GUIFONT_REGION_STYLE_T region = {0};
    
    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != head_pptr); /*assert verified*/
    if (PNULL == head_pptr)
    {
        return 0;
    }

    cur_node_ptr = *head_pptr;
    region = *region_ptr;

    for(;;)
    {
        GUISTRING_REGION_NODE_T *new_node_ptr = PNULL;
        GUISTRING_REGION_NODE_T *cover_node_ptr = PNULL;
        GUISTRING_REGION_NODE_T *remain_node_ptr = PNULL;
        uint16 cur_end = 0;
        uint16 new_end = 0;

        if (PNULL == cur_node_ptr)
        {
            new_node_ptr = SCI_ALLOC_APP(sizeof(GUISTRING_REGION_NODE_T));

            new_node_ptr->region = region;
            new_node_ptr->prev = prev_ptr;
            new_node_ptr->next = PNULL;

            if (PNULL == prev_ptr)
            {
                *head_pptr = new_node_ptr;
            }
            else
            {
                prev_ptr->next = new_node_ptr;
            }

            break;
        }

        cur_end = cur_node_ptr->region.start + cur_node_ptr->region.length - 1;
        new_end = region.start + region.length - 1;
        
        if (region.start < cur_node_ptr->region.start)
        {
            //NNN????????????
            //      CCCCCC
            new_node_ptr = SCI_ALLOC_APP(sizeof(GUISTRING_REGION_NODE_T));
            new_node_ptr->region.start = region.start;
            new_node_ptr->region.state = region.state;
            new_node_ptr->region.font_color = region.font_color;

            InsertNodePtr(new_node_ptr, cur_node_ptr);
            
            if (PNULL == prev_ptr)
            {
                *head_pptr = new_node_ptr;
            }
            else
            {
                prev_ptr->next = new_node_ptr;
            }

            if (new_end < cur_node_ptr->region.start)
            {
                //NNN---
                //      CCCCCC
                new_node_ptr->region.length = region.length;
            }
            else
            {
                //NNNNNNN????????
                //      CCCCCC
                new_node_ptr->region.length = cur_node_ptr->region.start - region.start;

                if (new_end < cur_end)
                {
                    //NNNNNNN----
                    //      CCCCCC
                    cover_node_ptr = SCI_ALLOC_APP(sizeof(GUISTRING_REGION_NODE_T));
                    cover_node_ptr->region.start = cur_node_ptr->region.start;
                    cover_node_ptr->region.length = new_end - cur_node_ptr->region.start + 1;
                    cover_node_ptr->region.state = cur_node_ptr->region.state | region.state;
                    cover_node_ptr->region.font_color = region.font_color;

                    cur_node_ptr->region.start = new_end + 1;
                    cur_node_ptr->region.length = cur_end - new_end;

                    InsertNodePtr(cover_node_ptr, cur_node_ptr);
                }
                else
                {
                    //NNNNNNNNNNNN???
                    //      CCCCCC
                    cur_node_ptr->region.state |= region.state;

                    if (cur_end < new_end)
                    {
                        //NNNNNNNNNNNNN--
                        //      CCCCCC
                        region.start = cur_end + 1;
                        region.length = new_end - cur_end;

                        prev_ptr = cur_node_ptr;
                        cur_node_ptr = cur_node_ptr->next;
                        continue;
                    }
                }
            }
        }
        else if (region.start <= cur_end)
        {
            //?NN??????
            //CCCCCC
            if (region.start > cur_node_ptr->region.start)
            {
                // NN??????
                //CCCCCC
                new_node_ptr = SCI_ALLOC_APP(sizeof(GUISTRING_REGION_NODE_T));
                new_node_ptr->region.start = cur_node_ptr->region.start;
                new_node_ptr->region.state = cur_node_ptr->region.state;
                new_node_ptr->region.length = region.start - cur_node_ptr->region.start;
                new_node_ptr->region.font_color = cur_node_ptr->region.font_color;

                InsertNodePtr(new_node_ptr, cur_node_ptr);
                
                if (PNULL == prev_ptr)
                {
                    *head_pptr = new_node_ptr;
                }
                else
                {
                    prev_ptr->next = new_node_ptr;
                }

                cur_node_ptr->region.start = region.start;
            }

            if (new_end < cur_end)
            {
                //?NN??
                //CCCCCC
                remain_node_ptr = SCI_ALLOC_APP(sizeof(GUISTRING_REGION_NODE_T));
                remain_node_ptr->region.start = new_end + 1;
                remain_node_ptr->region.length = cur_end - new_end;
                remain_node_ptr->region.state = cur_node_ptr->region.state;
                remain_node_ptr->region.font_color = region.font_color;

                cur_node_ptr->region.state |= region.state;
                cur_node_ptr->region.length = region.length;

                AppendNodePtr(cur_node_ptr, remain_node_ptr);
            }
            else
            {
                //?NNNNN???
                //CCCCCC
                cur_node_ptr->region.state |= region.state;
                cur_node_ptr->region.length = cur_end - cur_node_ptr->region.start + 1;

                if (cur_end < new_end)
                {
                    //?NNNNNNNN?
                    //CCCCCC
                    region.start = cur_end + 1;
                    region.length = new_end - cur_end;

                    prev_ptr = cur_node_ptr;
                    cur_node_ptr = cur_node_ptr->next;
                    continue;
                }
            }
        }
        else
        {
            //      NNN???
            //CCC???
            prev_ptr = cur_node_ptr;
            cur_node_ptr = cur_node_ptr->next;
            continue;
        }

        break;
    }


    return CombineNode(head_pptr);
}

/*==============================================================================
Description: CreateRegionPtr
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
LOCAL int32 CreateRegionPtr(
                            GUIFONT_REGION_STYLE_T const *in_region_ptr, 
                            int32 region_num,
                            GUIFONT_REGION_STYLE_T **out_region_ptr
                            )
{
    GUISTRING_REGION_NODE_T *head_ptr = PNULL;
    GUISTRING_REGION_NODE_T *next_ptr = PNULL;
    int32 i = 0;
    int32 node_cnt = 0;
    uint32 node_arr_size = 0;

    for (i = 0; i < region_num; i++)
    {
        if (0 < in_region_ptr[i].length)
        {
            node_cnt = InsertNode(&(head_ptr), in_region_ptr + i);
        }
    }

    if (0 == node_cnt) return 0;

    //force check
    SCI_ASSERT(PNULL != head_ptr); /*assert verified*/

    node_arr_size = sizeof(GUIFONT_REGION_STYLE_T)*(uint32)node_cnt;
    *out_region_ptr = SCI_ALLOC_APP(node_arr_size);

    for (i = 0; i < node_cnt; i++)
    {
        (*out_region_ptr)[i] = head_ptr->region;
        
        next_ptr = head_ptr->next;
        SCI_FREE(head_ptr);
        head_ptr = next_ptr;
    }

    //force check
    SCI_ASSERT(PNULL == head_ptr); /*assert verified*/

    return node_cnt;
}


/*******************************************************************************************/
// 	Description : GUI_GetStringInfoInLine的扩展, 支持所有特效
//	Global resource dependence :
//  Author: James.Zhang
//	Note: 
/*******************************************************************************************/
PUBLIC BOOLEAN GUISTR_LineBreak(
                                int8                 dir,
                                const wchar	         *str_ptr,
                                uint16      	     str_len,
                                const GUISTR_STYLE_T *style_ptr,
                                GUISTR_STATE_T       state,
                                uint16      	     width,
                                uint16			     *char_num,
                                uint16			     *pixel_num
                                )
{
    if (PNULL == char_num 
        || PNULL == pixel_num
        || PNULL == str_ptr 
        || 0 == *str_ptr 
        || 0 == str_len 
        || 0 == width 
        || PNULL == style_ptr )
    {
        return FALSE;
    }
    else
    {
        SPML_TEXT_PARAM_T param = {0};
        BOOLEAN is_byword = IsSetState(state, GUISTR_STATE_WORDBREAK);

        if (IsSetState(state, GUISTR_STATE_EFFECT)) // 设置了effect状态时，才转化响应状态
        {
            ConvertFontEffect( style_ptr->effect, style_ptr->font_color, style_ptr->edge_color, &param );
        }
        
        // 设置文本其他状态
        ConvertTextState(state, &param.format);

        if ( param.format & ( TEXT_FORMAT_EDGE | TEXT_FORMAT_ITALIC | TEXT_FORMAT_BOLD | TEXT_FORMAT_WORDSPACE )
            || style_ptr->region_num > 0 )
        {
            SPML_TEXT_REGION_T *text_region_ptr = PNULL;

            // 设置特殊字符，特殊字符串能影响字符串的宽度
            if ( style_ptr->region_num > 0 )
            {
                int32 i = 0;
                
                text_region_ptr = SCI_ALLOC_APP( style_ptr->region_num * sizeof(SPML_TEXT_REGION_T) );
                
                SCI_MEMSET( text_region_ptr, 0, (style_ptr->region_num * sizeof(SPML_TEXT_REGION_T)) );
                
                for ( i = 0; i < style_ptr->region_num; i++ )
                {
                    text_region_ptr[i].start = style_ptr->region_ptr[i].start;
                    text_region_ptr[i].end   = style_ptr->region_ptr[i].start + style_ptr->region_ptr[i].length;
                    
                    // 颜色, 大小, format根据需求填写, 暂时支持不同字体
                    // text_region_ptr[i].style.color = param.font_color;
                    text_region_ptr[i].style.size  = param.size;
                    ConvertTextState(style_ptr->region_ptr[i].state, &text_region_ptr[i].style.format);
                    
                    text_region_ptr[i].style.color = style_ptr->region_ptr[i].font_color;
                }
                
                param.region_num = style_ptr->region_num;
                param.region_ptr = text_region_ptr;
            }
            
            param.glyph_space = style_ptr->char_space;
            param.word_space  = style_ptr->word_space;
            param.dir = dir;
            param.size = GUI_ConvertGUIFontToSize( style_ptr->font );
            
            *char_num = SPMLAPI_LineBreak( &param, str_ptr, str_len, width, pixel_num, is_byword );

            // 释放特殊字符内存
            if ( PNULL != text_region_ptr )
            {
                SCI_FREE( text_region_ptr );
            }
            
            return TRUE;
        }
        else
        {
            return GUI_GetStringInfoInLineWithDir( dir, str_ptr, str_len, 
                style_ptr->font, style_ptr->char_space, width, char_num, pixel_num, is_byword );
        }
    }
}

#endif // _GUI_STRING_C_ 
#endif 
