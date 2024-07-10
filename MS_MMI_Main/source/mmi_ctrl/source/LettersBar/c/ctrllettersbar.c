/*****************************************************************************
** File Name:      ctrllettersbar.c                                          *
** Author:                                                                   *
** Date:           08/18/2012                                                *
** Copyright:      2012 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 08/2012      hua.fang              Creat
******************************************************************************/

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "mmi_gui_trc.h"
#include "mmk_type.h"
#include "mmk_app.h"
#include "mmk_timer.h"
#include "guires.h"
#include "mmi_theme.h"
#include "mmitheme_lettersbar.h"
#include "ctrllettersbar.h"
#include "guistring.h"
#include "guiprgbox.h"

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                 */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         EXTERNAL DEFINITION                              *
**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : init lettersbar class
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN LettersbarCtrlInitVtbl(
                                     CTRLLETTERSBAR_VTBL_T        *lettersbar_vtbl_ptr
                                     );

/*****************************************************************************/
//  Description : lettersbar construct
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN LettersbarCtrlConstruct (
                                       CTRLBASE_OBJ_T           *base_ctrl_ptr,
                                       CTRLLETTERSBAR_INIT_PARAM_T   *lettersbar_param_ptr
                                       );

/*****************************************************************************/
//  Description : lettersbar destruct
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN LettersbarCtrlDestruct (
                                      CTRLBASE_OBJ_T        *base_ctrl_ptr
                                      );

/*****************************************************************************/
//  Description : convert init param
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void LettersbarCtrlPackInitParam (
                                        GUILETTERSBAR_INIT_DATA_T     *lettersbar_init_ptr,   //in
                                        CTRLLETTERSBAR_INIT_PARAM_T   *lettersbar_param_ptr   //out
                                        );

/*****************************************************************************/
//  Description : handle lettersbar msg function
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E LettersbarCtrlHandleMsg (
                                            CTRLBASE_OBJ_T        *base_ctrl_ptr,   //control pointer
                                            MMI_MESSAGE_ID_E      msg_id,           //message
                                            DPARAM                param             //add data
                                            );

/*****************************************************************************/
//  Description : display lettersbar control
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void LettersbarDraw (
                           CTRLLETTERSBAR_OBJ_T     *lettersbar_ctrl_ptr
                           );

/*****************************************************************************/
//  Description : get lettersbar pointer by control id
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL CTRLLETTERSBAR_OBJ_T* GetLettersbarPtr (
                                              MMI_HANDLE_T     ctrl_handle
                                              );

/*****************************************************************************/
//  Description : ext type of
//  Global resource dependence :
//  Author:hua.fang
//  Note: judge control type of
/*****************************************************************************/
LOCAL BOOLEAN LettersbarTypeOf (
                                CTRLBASE_OBJ_T     *ctrl_ptr
                                );

/*****************************************************************************/
// 	Description : get control height by width,include content,border etc.
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN LettersbarCtrlGetHeightByWidth (
                                              CTRLBASE_OBJ_T      *base_ctrl_ptr,  //in:
                                              uint16              width,          //in:
                                              uint16              *height_ptr     //in/out:
                                              );

/*****************************************************************************/
// 	Description : set ctrl display rect
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN LettersbarCtrlSetDisplayRect (
                                            CTRLBASE_OBJ_T      *base_ctrl_ptr, //in:
                                            const GUI_RECT_T    *rect_ptr,      //in:
                                            BOOLEAN             is_update       //in:
                                            );

/*****************************************************************************/
// 	Description : set ctrl rect
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN LettersbarCtrlSetRect (
                                     CTRLBASE_OBJ_T      *base_ctrl_ptr, //in:
                                     const GUI_RECT_T    *rect_ptr
                                     );

/*****************************************************************************/
// 	Description : set ctrl rect
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN LettersbarCtrlSetBothRect (
                                         CTRLBASE_OBJ_T      *base_ctrl_ptr, //in:
                                         const GUI_BOTH_RECT_T    *both_rect_ptr
                                         );

/*****************************************************************************/
//  Description : 初始化字母表
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void LettersbarInitLettersTable (
                                       CTRLLETTERSBAR_OBJ_T *lettersbar_ctrl_ptr
                                       );

/*****************************************************************************/
//  Description : display lettersbar background
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void LettersbarDrawBg (
                             CTRLLETTERSBAR_OBJ_T *lettersbar_ctrl_ptr
                             );

/*****************************************************************************/
// 	Description : display horizontal scroll box
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN LettersbarDrawLetters(
                                    CTRLLETTERSBAR_OBJ_T  *lettersbar_ctrl_ptr
                                    );

/*****************************************************************************/
//  Description : letters bar control reset
//  Global resource dependence :
//  Author: xiaoqing.lu & nan.ji
//  Note:
/*****************************************************************************/
LOCAL void LettersbarReset(
                           CTRLLETTERSBAR_OBJ_T *lettersbar_ctrl_ptr
                           );

/*****************************************************************************/
//  Description :  get current item index according to point position
//  Global resource dependence : none
//  Author: xiaoqing.lu & nan.ji
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN LettersbarGetCurrentItem(
                                       CTRLLETTERSBAR_OBJ_T * lettersbar_ctrl_ptr, // in
                                       GUI_POINT_T cur_point, // in
                                       uint16 *index_ptr // in/out
                                       );
#ifdef TOUCH_PANEL_SUPPORT
/*****************************************************************************/
//  Description : handle lettersbar msg function
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void LettersbarHandleTp(
                              CTRLLETTERSBAR_OBJ_T  *lettersbar_ctrl_ptr, 
                              MMI_MESSAGE_ID_E      msg_id,           //message
                              DPARAM                param             //add data
                              );
#endif

/*==============================================================================
Description: 创建显示的层
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note: 在初始化及屏幕旋转的时候，都需要建立显示层
==============================================================================*/
LOCAL BOOLEAN LetterBarCreateLayer (
                                    CTRLLETTERSBAR_OBJ_T *lettersbar_ctrl_ptr
                                    );

/*==============================================================================
Description: 释放显示层
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
LOCAL void LetterBarReleaseLayer (
                                  CTRLLETTERSBAR_OBJ_T *lettersbar_ctrl_ptr
                                  );

/**--------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : get lettersbar type
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC TYPE_TAG_T LETTERSBAR_GetType()
{
    static TYPE_TAG_T    obj_type = 0;
    
    if (0 == obj_type)
    {
        obj_type = TYPEMNG_Register (
            CTRL_BASE_TYPE,                                  // parent class type
            "lettersbar",                                         // control name
            sizeof (CTRLLETTERSBAR_OBJ_T),                        // size of control object struct
            (OBJECT_CONSTRUCTOR_FUNC) LettersbarCtrlConstruct,    // control constructor function
            (OBJECT_DESTRUCTOR_FUNC) LettersbarCtrlDestruct,      // control destructor function
            sizeof (CTRLLETTERSBAR_INIT_PARAM_T),                 // size of init param struct
            (PARAM_PACKING_FUNC) LettersbarCtrlPackInitParam,     // convert init param
            sizeof (CTRLLETTERSBAR_VTBL_T),                       // size of virtual table
            (VTBL_INIT_FUNC) LettersbarCtrlInitVtbl);             // init virtual table function
    }
    
    return obj_type;
}

/*****************************************************************************/
//  Description : init lettersbar class
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN LettersbarCtrlInitVtbl (
                                      CTRLLETTERSBAR_VTBL_T        *lettersbar_vtbl_ptr
                                      )
{
    CTRLBASE_VTBL_T  *base_ctrl_vtbl_ptr = (CTRLBASE_VTBL_T*) lettersbar_vtbl_ptr;
    
    if (PNULL == base_ctrl_vtbl_ptr)
    {
        return FALSE;
    }
    
    base_ctrl_vtbl_ptr->HandleEvent = LettersbarCtrlHandleMsg;
    
    // 以下接口用于实现内嵌form
    base_ctrl_vtbl_ptr->GetHeightByWidth = LettersbarCtrlGetHeightByWidth;
    base_ctrl_vtbl_ptr->SetDisplayRect = LettersbarCtrlSetDisplayRect;
    base_ctrl_vtbl_ptr->SetRect = LettersbarCtrlSetRect;
    base_ctrl_vtbl_ptr->SetBothRect = LettersbarCtrlSetBothRect;
    
    return TRUE;
}

/*****************************************************************************/
//  Description : convert init param
//  Global resource dependence :
//  Author: hua.fang
//  Note:
// 该方法用于将控件的对外初始化参数结构转换为内部的初始化参数结构
// 由于控件有继承关系，控件初始化时又带有参数，所以内部的初始化参数也必须有继承关系
/*****************************************************************************/
LOCAL void LettersbarCtrlPackInitParam (
                                        GUILETTERSBAR_INIT_DATA_T     *lettersbar_init_ptr,   //in
                                        CTRLLETTERSBAR_INIT_PARAM_T   *lettersbar_param_ptr   //out
                                        )
{
    CTRLBASE_INIT_PARAM_T   *base_ctrl_param_ptr = (CTRLBASE_INIT_PARAM_T*) lettersbar_param_ptr;
    
    if (PNULL == lettersbar_init_ptr || PNULL == lettersbar_param_ptr)
    {
        return;
    }
    
    base_ctrl_param_ptr->both_rect = lettersbar_init_ptr->both_rect;
}

/*****************************************************************************/
//  Description : 初始化字母表
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void LettersbarInitLettersTable (
                                       CTRLLETTERSBAR_OBJ_T *lettersbar_ctrl_ptr
                                       )
{
    uint32 i = 0;

    if (lettersbar_ctrl_ptr != PNULL)
    {
        lettersbar_ctrl_ptr->letters_ptr = SCI_ALLOC_APPZ (sizeof (uint16) * ('Z' - 'A' + 2));
        //符号
        *lettersbar_ctrl_ptr->letters_ptr = '#';
        lettersbar_ctrl_ptr->letters_num = 1;

        //字母
        for (i = 0; i <= ('Z' - 'A'); i++)
        {
            *(lettersbar_ctrl_ptr->letters_ptr + i + 1) = 'A' + i;
        }

        lettersbar_ctrl_ptr->letters_num += ('Z' - 'A') + 1;

        lettersbar_ctrl_ptr->letter_postion_table = SCI_ALLOC_APPZ (sizeof (int16) * lettersbar_ctrl_ptr->letters_num);
    }
}

/*****************************************************************************/
//  Description : lettersbar construct
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN LettersbarCtrlConstruct (
                                       CTRLBASE_OBJ_T *base_ctrl_ptr,
                                       CTRLLETTERSBAR_INIT_PARAM_T *lettersbar_param_ptr
                                       )
{
    BOOLEAN             result = TRUE;
    MMI_LETTERS_STYLE_T letters_style = {0};
    CTRLLETTERSBAR_OBJ_T     *lettersbar_ctrl_ptr = (CTRLLETTERSBAR_OBJ_T*) base_ctrl_ptr;
    
    if (PNULL == base_ctrl_ptr || PNULL == lettersbar_param_ptr)
    {
        return FALSE;
    }

    MMITHEME_GetLettersStyle(&letters_style);

    lettersbar_ctrl_ptr->is_dirty = TRUE;

    // about letters
    lettersbar_ctrl_ptr->letter_font = letters_style.letter_font;
    lettersbar_ctrl_ptr->letter_font_color = letters_style.letter_font_color;
    lettersbar_ctrl_ptr->letter_scale = letters_style.letter_scale;

    lettersbar_ctrl_ptr->is_disp_bg = FALSE;

    lettersbar_ctrl_ptr->is_disp_own = TRUE;
   
    LettersbarInitLettersTable(lettersbar_ctrl_ptr);
    
    MMITHEME_GetLettersBg(&base_ctrl_ptr->bg);

    base_ctrl_ptr->lcd_dev_info.block_id = UILAYER_NULL_HANDLE;
    // LetterBarCreateLayer(lettersbar_ctrl_ptr);

    return (result);
}

/*****************************************************************************/
//  Description : lettersbar destruct
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN LettersbarCtrlDestruct (
                                      CTRLBASE_OBJ_T    *base_ctrl_ptr
                                      )
{
    BOOLEAN             result = TRUE;
    CTRLLETTERSBAR_OBJ_T     *lettersbar_ctrl_ptr = (CTRLLETTERSBAR_OBJ_T*) base_ctrl_ptr;
    
    if (PNULL == lettersbar_ctrl_ptr)
    {
        return FALSE;
    }

    LetterBarReleaseLayer(lettersbar_ctrl_ptr);
    
    SCI_FREE(lettersbar_ctrl_ptr->letters_ptr);
    SCI_FREE(lettersbar_ctrl_ptr->letter_postion_table);
    
    return (result);
}

/*****************************************************************************/
//  Description :  get current item index according to point position
//  Global resource dependence : none
//  Author: xiaoqing.lu & nan.ji
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN LettersbarGetCurrentItem(
                                       CTRLLETTERSBAR_OBJ_T * lettersbar_ctrl_ptr, // in
                                       GUI_POINT_T cur_point, // in
                                       uint16 *index_ptr // in/out
                                       )
{
    int16           i = 0;
    int16           ret = 0;
    int16           start = 0;
    int16           end = 0;
    int16           position = cur_point.y;
    GUI_RECT_T      bar_rect = {0};
    BOOLEAN         result = TRUE;
    BOOLEAN         is_found = FALSE;
    CTRLBASE_OBJ_T *base_ctrl_ptr = (CTRLBASE_OBJ_T *)lettersbar_ctrl_ptr;

    if (PNULL == lettersbar_ctrl_ptr || PNULL == index_ptr)
    {
        return FALSE;
    }

    ret = lettersbar_ctrl_ptr->max_disp_num - 1;
    start = end = 0;
    
    for (i = 0; i < lettersbar_ctrl_ptr->max_disp_num; i++)
    {
        bar_rect = base_ctrl_ptr->rect; //

        bar_rect.top += base_ctrl_ptr->padding.top;
        bar_rect.bottom -= base_ctrl_ptr->padding.bottom;        
        
        start = lettersbar_ctrl_ptr->letter_postion_table[i];
        
        if (i + 1 < lettersbar_ctrl_ptr->max_disp_num)
        {
            end =  lettersbar_ctrl_ptr->letter_postion_table[i + 1] - 1;
        }
        else
        {
            end = bar_rect.bottom;
        }
        
        if ( (position >= start) && (position <= end))
        {
            ret = i;
            is_found = TRUE;
            lettersbar_ctrl_ptr->cur_index = ret;
            break;
        }
    }
    
    if (is_found)
    {
        *index_ptr = ret;
    }
    else
    {
        result = FALSE;
    }

    return result;
}
#ifdef TOUCH_PANEL_SUPPORT
/*****************************************************************************/
//  Description : handle lettersbar msg function
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void LettersbarHandleTp(
                              CTRLLETTERSBAR_OBJ_T  *lettersbar_ctrl_ptr, 
                              MMI_MESSAGE_ID_E      msg_id,           //message
                              DPARAM                param             //add data
                              )
{
    GUI_POINT_T point = {0};
    uint16     index = 0;
    CTRLBASE_OBJ_T     *base_ctrl_ptr = (CTRLBASE_OBJ_T*) lettersbar_ctrl_ptr;
        
    point.x = MMK_GET_TP_X(param);
    point.y = MMK_GET_TP_Y(param);
    
    if (LettersbarGetCurrentItem(lettersbar_ctrl_ptr, point, &index))
    {
        CTRLMSG_PostNotifyEx (base_ctrl_ptr->handle, MSG_NOTIFY_LETTER_CHANGED, &index, sizeof (uint16));
    }
    
    LettersbarDraw(lettersbar_ctrl_ptr);
}
#endif

/*****************************************************************************/
//  Description : handle lettersbar msg function
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E LettersbarCtrlHandleMsg (
                                            CTRLBASE_OBJ_T        *base_ctrl_ptr,   //control pointer
                                            MMI_MESSAGE_ID_E      msg_id,           //message
                                            DPARAM                param             //add data
                                            )
{
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    CTRLLETTERSBAR_OBJ_T     *lettersbar_ctrl_ptr = (CTRLLETTERSBAR_OBJ_T*) base_ctrl_ptr;
    CTRLBASE_VTBL_T     *parent_vtbl_ptr = (CTRLBASE_VTBL_T*)TYPEMNG_GetParentVtbl(CTRL_LETTERSBAR_TYPE, (OBJECT_TYPE_PTR)base_ctrl_ptr);
    
    if (PNULL == lettersbar_ctrl_ptr)
    {
        return MMI_RESULT_FALSE;
    }
    
    switch (msg_id)
    {
    case MSG_CTL_PAINT:
        if (!lettersbar_ctrl_ptr->is_disp_own)
        {
            LettersbarDraw (lettersbar_ctrl_ptr);
        }
        break;
        
    case MSG_CTL_GET_ACTIVE:
        break;
        
    case MSG_CTL_LOSE_ACTIVE:
    case MSG_CTL_LOSE_FOCUS:
    case MSG_CTL_CLOSE:
        break;
        
    case MSG_NOTIFY_LOSE_ACTIVE:
        break;
        
#ifdef TOUCH_PANEL_SUPPORT
    case MSG_TP_PRESS_DOWN:
        
        lettersbar_ctrl_ptr->is_tp_down = TRUE;
        LettersbarHandleTp(lettersbar_ctrl_ptr, msg_id, param);
        break;

    case MSG_TP_PRESS_MOVE:
        LettersbarHandleTp(lettersbar_ctrl_ptr, msg_id, param);
        break;

    case MSG_TP_PRESS_UP:
        lettersbar_ctrl_ptr->is_tp_down = FALSE;
        LettersbarHandleTp(lettersbar_ctrl_ptr, msg_id, param);
        break;
#endif
        
    default:
        result = parent_vtbl_ptr->HandleEvent(base_ctrl_ptr, msg_id, param);
        break;
    }
    
    return (result);
}

/*****************************************************************************/
// 	Description : draw letters
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN LettersbarDrawLetters(
                                    CTRLLETTERSBAR_OBJ_T  *lettersbar_ctrl_ptr
                                    )
{
    BOOLEAN         result = TRUE;
    uint16          i = 0;
    uint32          letter_count = 0;
    uint16          scale = 0;
    uint16          font_height = 0;
    uint16          font_width = GUI_GetFontWidth (GUIPRGBOX_LETTER_FONT, 'A');
    uint16          all_space_height = 0;
    uint16          one_space_height = 0;
    uint16          unicode = 0;
    GUISTR_STYLE_T   text_style      = {0};
    uint16          compensation = 0;
    GUI_RECT_T      text_rect = {0};
    GUI_RECT_T      letters_rect;
    MMI_STRING_T    string = {0};
    wchar           str_buf[2] = {0};
    CTRLBASE_OBJ_T *base_ctrl_ptr = (CTRLBASE_OBJ_T *) lettersbar_ctrl_ptr;
    
    if (PNULL == lettersbar_ctrl_ptr)
    {
        return FALSE;
    }
    
    IMG_EnableTransparentColor (TRUE);
    
    //draw letters  bar
    font_height = GUIFONT_GetHeight (GUIPRGBOX_LETTER_FONT);
    text_style.align = ALIGN_HVMIDDLE;
    
    if (MMK_IsWindowLandscape (base_ctrl_ptr->win_handle))
    {
        //竖屏
        text_rect = base_ctrl_ptr->both_rect.h_rect;
    }
    else
    {
        text_rect = base_ctrl_ptr->both_rect.v_rect;
    }
    
    if (lettersbar_ctrl_ptr->max_disp_num < lettersbar_ctrl_ptr->letters_num)
    {
        //缩略显示，字母中间要用一个符号间隔
        letter_count = (lettersbar_ctrl_ptr->max_disp_num << 1) - 1;
        scale = lettersbar_ctrl_ptr->letter_scale;
    }
    else
    {
        //全字母显示，字母中间不需要用符号间隔
        letter_count = lettersbar_ctrl_ptr->max_disp_num;
        scale = 1;
    }
    
    if ( (text_rect.bottom - text_rect.top) > (font_height * letter_count)) /*lint !e574 !e737*/
    {
        text_rect.top = text_rect.top + base_ctrl_ptr->padding.top;
        text_rect.bottom = text_rect.bottom - base_ctrl_ptr->padding.bottom;
        
        letters_rect = text_rect;

        if (lettersbar_ctrl_ptr->max_disp_num < lettersbar_ctrl_ptr->letters_num)
        {
            //缩略显示，字母中间要用一个符号间隔
            letter_count = (lettersbar_ctrl_ptr->max_disp_num << 1) - 1;
            scale = lettersbar_ctrl_ptr->letter_scale;
        }
        else
        {
            //全字母显示，字母中间不需要用符号间隔
            letter_count = lettersbar_ctrl_ptr->max_disp_num;
            scale = 1;
        }
        
        all_space_height = (text_rect.bottom - text_rect.top) - (font_height * letter_count);   /*lint !e737*/
        
        if (letter_count > 1)
        {
            one_space_height = (all_space_height) / (letter_count - 1);//间隔
            compensation = all_space_height - one_space_height * (letter_count - 1);//像素补偿值
        }
        
        text_style.font = lettersbar_ctrl_ptr->letter_font;
        text_style.font_color = lettersbar_ctrl_ptr->letter_font_color;
        string.wstr_ptr = str_buf;
        string.wstr_len = 1;
        
        for (i = 0; i < lettersbar_ctrl_ptr->max_disp_num; i++)
        {
            if (i * scale > lettersbar_ctrl_ptr->letters_num)
            {
                unicode = lettersbar_ctrl_ptr->letters_ptr[lettersbar_ctrl_ptr->letters_num - 1];
            }
            else
            {
                unicode = lettersbar_ctrl_ptr->letters_ptr[i * scale];
            }
            
            if (i > 0)
            {
                if (compensation > 0)
                {
                    compensation--;
                    text_rect.top = text_rect.top + one_space_height + font_height + 1;
                }
                else
                {
                    text_rect.top = text_rect.top + one_space_height + font_height;
                }
            }
                        
            if (PNULL != lettersbar_ctrl_ptr->letter_postion_table)
            {
                lettersbar_ctrl_ptr->letter_postion_table[i] = text_rect.top;
            }

            text_rect.bottom = text_rect.top + font_height;
            
            //draw letters
            string.wstr_ptr[0] = unicode;
            GUISTR_DrawTextToLCDInRect (
                &base_ctrl_ptr->lcd_dev_info,
                &text_rect,       //the fixed display area
                &text_rect,       //用户要剪切的实际区域
                &string,
                &text_style,
                GUISTR_STATE_SINGLE_LINE | GUISTR_STATE_ALIGN,
                GUISTR_TEXT_DIR_AUTO
                );
            
            if (scale > 1)
            {
                //draw mark
                if (compensation > 0)
                {
                    compensation--;
                    text_rect.top = text_rect.top + one_space_height + font_height + 1;
                }
                else
                {
                    text_rect.top = text_rect.top + one_space_height + font_height;
                }
                
                text_rect.bottom = text_rect.top + font_height;
                
                if (text_rect.bottom <= letters_rect.bottom) // ??
                {
                    int16 x = 0;
                    int16 y = 0;
                    uint16 radius = 0;
                    
                    x = text_rect.left + ( (text_rect.right - text_rect.left) >> 1);
                    y = text_rect.top + (font_height >> 1);
                    radius = MIN ( (text_rect.right - text_rect.left), font_width);
                    radius = radius >> 1;
                    LCD_FillCircle (&base_ctrl_ptr->lcd_dev_info, &text_rect, x, y, radius, 0);
                }
            }
        }
        
        IMG_EnableTransparentColor (FALSE);

        return (result);
    }

    return (result);
}

/*==============================================================================
Description: 创建显示的层
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note: 在初始化及屏幕旋转的时候，都需要建立显示层
==============================================================================*/
LOCAL BOOLEAN LetterBarCreateLayer (
                                    CTRLLETTERSBAR_OBJ_T *lettersbar_ctrl_ptr
                                    )
{
    UILAYER_CREATE_T create_info = {0};
    GUI_RECT_T disp_rect = {0};
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*) lettersbar_ctrl_ptr;

    if (PNULL == lettersbar_ctrl_ptr)
    {
        return FALSE;
    }
#ifdef UI_MULTILAYER_SUPPORT	//支持多图层
    if (!UILAYER_IsMultiLayerEnable())
    {
        SCI_PASSERT (0, ("[LetterBarCreateLayer].Init: MultiLayer Disabled!")); /*assert verified*/
    }

    disp_rect = base_ctrl_ptr->rect;

    if (GUI_IsInvalidRect(disp_rect))
    {
        return FALSE;
    }

    if (lettersbar_ctrl_ptr->is_dirty || UILAYER_NULL_HANDLE == base_ctrl_ptr->lcd_dev_info.block_id)
    {
        UILAYER_RELEASELAYER(&base_ctrl_ptr->lcd_dev_info);

        create_info.lcd_id = MAIN_LCD_ID;
        create_info.owner_handle = base_ctrl_ptr->win_handle;
        create_info.offset_x = disp_rect.left;
        create_info.offset_y = disp_rect.top;
        create_info.width = disp_rect.right - disp_rect.left;
        create_info.height = (disp_rect.bottom - disp_rect.top + 1);
        create_info.is_bg_layer = FALSE;
        create_info.is_static_layer = FALSE;
        create_info.format = UILAYER_MEM_DOUBLE_COPY;
        
        UILAYER_CreateLayer (&create_info, & (base_ctrl_ptr->lcd_dev_info));
        UILAYER_SetLayerColorKey (& (base_ctrl_ptr->lcd_dev_info), TRUE, UILAYER_TRANSPARENT_COLOR);
    }
#endif

    return TRUE;
}

/*==============================================================================
Description: 释放显示层
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
LOCAL void LetterBarReleaseLayer (
                                  CTRLLETTERSBAR_OBJ_T *lettersbar_ctrl_ptr
                                  )
{
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*) lettersbar_ctrl_ptr;

    if (PNULL != base_ctrl_ptr)
    {
#ifdef UI_MULTILAYER_SUPPORT	//支持多图层
        UILAYER_RELEASELAYER (& (base_ctrl_ptr->lcd_dev_info)); /*lint !e506 !e774*/
#endif
    }

    return;
}

/*****************************************************************************/
//  Description : display lettersbar background
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void LettersbarDrawBg (
                             CTRLLETTERSBAR_OBJ_T *lettersbar_ctrl_ptr
                             )
{
    if (PNULL != lettersbar_ctrl_ptr)
    {
        GUI_RECT_T image_rect = {0};
        GUI_BG_DISPLAY_T display_info = {0};
        UILAYER_APPEND_BLT_T append_layer = {0};
        CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*) lettersbar_ctrl_ptr;

#ifdef UI_MULTILAYER_SUPPORT	//支持多图层
        if (UILAYER_IsLayerActive(&base_ctrl_ptr->lcd_dev_info))
        {
            //disp_layer每次都会重画，使用双缓冲切换的方式
            UILAYER_SwitchDoubleMem (&base_ctrl_ptr->lcd_dev_info, TRUE);
            
            //ClearDispInfo(lettersbar_ctrl_ptr);
            UILAYER_Clear (& (base_ctrl_ptr->lcd_dev_info));
            
            append_layer.lcd_dev_info = base_ctrl_ptr->lcd_dev_info;
            append_layer.layer_level = UILAYER_LEVEL_HIGH;
            
            UILAYER_AppendBltLayer (&append_layer);
        }
#endif        

        if (lettersbar_ctrl_ptr->is_tp_down || lettersbar_ctrl_ptr->is_disp_bg)
        {
            image_rect.right = base_ctrl_ptr->rect.right - base_ctrl_ptr->rect.left;
            image_rect.bottom = base_ctrl_ptr->rect.bottom - base_ctrl_ptr->rect.top;

            display_info.img_rect = image_rect;
            display_info.display_rect = base_ctrl_ptr->display_rect;
            display_info.rect = base_ctrl_ptr->rect;
            display_info.ctrl_handle    = base_ctrl_ptr->handle;
            display_info.win_handle     = base_ctrl_ptr->win_handle;
                        
            if (lettersbar_ctrl_ptr->is_tp_down)
            {
                GUI_SetAlphaOverwrite (TRUE);
            }

            GUI_DisplayBg(&base_ctrl_ptr->bg, &display_info, &(base_ctrl_ptr->lcd_dev_info)); // ??
                        
            if (lettersbar_ctrl_ptr->is_tp_down)
            {
                GUI_SetAlphaOverwrite (FALSE);
            }
        }
    }
}

/*****************************************************************************/
//  Description : letters bar control reset
//  Global resource dependence :
//  Author: xiaoqing.lu & nan.ji
//  Note:
/*****************************************************************************/
LOCAL void LettersbarReset(
                           CTRLLETTERSBAR_OBJ_T *lettersbar_ctrl_ptr
                           )
{
    uint16 font_height = 0;
    GUI_RECT_T letters_rect = {0};

    if (PNULL != lettersbar_ctrl_ptr && lettersbar_ctrl_ptr->is_dirty)
    {
        CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*)lettersbar_ctrl_ptr;

        //letters scrollbar, its item count is fixed
        font_height = GUIFONT_GetHeight (MMI_LETTER_FONT);
        
        if (MMK_IsWindowLandscape (base_ctrl_ptr->win_handle))
        {
            //竖屏
            letters_rect = base_ctrl_ptr->both_rect.h_rect;
        }
        else
        {
            letters_rect = base_ctrl_ptr->both_rect.v_rect;
        }
        letters_rect.top += base_ctrl_ptr->padding.top;
        letters_rect.bottom -= base_ctrl_ptr->padding.top;
        
        if ( (letters_rect.bottom - letters_rect.top) > (font_height* lettersbar_ctrl_ptr->letters_num))
        {
            //可以显示全字母表
            lettersbar_ctrl_ptr->max_disp_num = lettersbar_ctrl_ptr->letters_num;
        }
        else
        {
            //不可以显示全字母表，按比例显示
            lettersbar_ctrl_ptr->max_disp_num = (lettersbar_ctrl_ptr->letters_num) / lettersbar_ctrl_ptr->letter_scale;
            
            if (lettersbar_ctrl_ptr->max_disp_num * lettersbar_ctrl_ptr->letter_scale < (lettersbar_ctrl_ptr->letters_num))
            {
                lettersbar_ctrl_ptr->max_disp_num++;
            }
        }
    }

    return;
}

/*****************************************************************************/
//  Description : display lettersbar control
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void LettersbarDraw (
                           CTRLLETTERSBAR_OBJ_T     *lettersbar_ctrl_ptr
                           )
{
    LettersbarReset(lettersbar_ctrl_ptr);

    LetterBarCreateLayer(lettersbar_ctrl_ptr);

    lettersbar_ctrl_ptr->is_dirty = FALSE;

    LettersbarDrawBg(lettersbar_ctrl_ptr);

    LettersbarDrawLetters(lettersbar_ctrl_ptr);
}

/*****************************************************************************/
//  Description : get lettersbar pointer by control id
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL CTRLLETTERSBAR_OBJ_T* GetLettersbarPtr (
                                              MMI_HANDLE_T     ctrl_handle
                                              )
{
    CTRLBASE_OBJ_T  *ctrl_ptr = (CTRLBASE_OBJ_T*)MMK_GetCtrlPtr (ctrl_handle);
    
    if (PNULL != ctrl_ptr)
    {
        SCI_ASSERT (LettersbarTypeOf (ctrl_ptr)); /*assert verified*/
    }
    
    return (CTRLLETTERSBAR_OBJ_T*) ctrl_ptr;
}

/*****************************************************************************/
//  Description : ext type of
//  Global resource dependence :
//  Author:hua.fang
//  Note: judge control type of
/*****************************************************************************/
LOCAL BOOLEAN LettersbarTypeOf (
                                CTRLBASE_OBJ_T     *ctrl_ptr
                                )
{
    return TYPEMNG_IsTypeOf((OBJECT_TYPE_PTR)ctrl_ptr, CTRL_LETTERSBAR_TYPE);
}

/*****************************************************************************/
// 	Description : get control height by width,include content,border etc.
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN LettersbarCtrlGetHeightByWidth (
                                              CTRLBASE_OBJ_T      *base_ctrl_ptr,  //in:
                                              uint16              width,          //in:
                                              uint16              *height_ptr     //in/out:
                                              )
{
    BOOLEAN     result = FALSE;
    
    if (PNULL != base_ctrl_ptr && PNULL != height_ptr)
    {
        result = TRUE;
        *height_ptr = base_ctrl_ptr->rect.right - base_ctrl_ptr->rect.left + 1;
    }
    
    return (result);
}

/*****************************************************************************/
// 	Description : set ctrl display rect
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN LettersbarCtrlSetDisplayRect (
                                            CTRLBASE_OBJ_T      *base_ctrl_ptr, //in:
                                            const GUI_RECT_T    *rect_ptr,      //in:
                                            BOOLEAN             is_update       //in:
                                            )
{
    BOOLEAN     result = FALSE;
    CTRLLETTERSBAR_OBJ_T *lettersbar_ctrl_ptr = (CTRLLETTERSBAR_OBJ_T*)base_ctrl_ptr;
    CTRLBASE_VTBL_T     *parent_vtbl_ptr = (CTRLBASE_VTBL_T*)TYPEMNG_GetParentVtbl(CTRL_LETTERSBAR_TYPE, (OBJECT_TYPE_PTR)base_ctrl_ptr);

    if (PNULL != lettersbar_ctrl_ptr)
    {
        lettersbar_ctrl_ptr->is_dirty = TRUE;
        parent_vtbl_ptr->SetDisplayRect(base_ctrl_ptr, rect_ptr, is_update);
    }

    return (result);
}

/*****************************************************************************/
// 	Description : set ctrl rect
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN LettersbarCtrlSetRect (
                                     CTRLBASE_OBJ_T      *base_ctrl_ptr, //in:
                                     const GUI_RECT_T    *rect_ptr
                                     )
{
    BOOLEAN     result = FALSE;
    CTRLLETTERSBAR_OBJ_T *lettersbar_ctrl_ptr = (CTRLLETTERSBAR_OBJ_T*)base_ctrl_ptr;
    CTRLBASE_VTBL_T     *parent_vtbl_ptr = (CTRLBASE_VTBL_T*)TYPEMNG_GetParentVtbl(CTRL_LETTERSBAR_TYPE, (OBJECT_TYPE_PTR)base_ctrl_ptr);

    if (PNULL != lettersbar_ctrl_ptr)
    {
        lettersbar_ctrl_ptr->is_dirty = TRUE;
        parent_vtbl_ptr->SetRect(base_ctrl_ptr, rect_ptr);
    }

    return (result);
}

/*****************************************************************************/
// 	Description : set ctrl rect
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN LettersbarCtrlSetBothRect (
                                         CTRLBASE_OBJ_T      *base_ctrl_ptr, //in:
                                         const GUI_BOTH_RECT_T    *both_rect_ptr
                                         )
{
    BOOLEAN     result = FALSE;
    CTRLLETTERSBAR_OBJ_T *lettersbar_ctrl_ptr = (CTRLLETTERSBAR_OBJ_T*)base_ctrl_ptr;
    CTRLBASE_VTBL_T     *parent_vtbl_ptr = (CTRLBASE_VTBL_T*)TYPEMNG_GetParentVtbl(CTRL_LETTERSBAR_TYPE, (OBJECT_TYPE_PTR)base_ctrl_ptr);

    if (PNULL != lettersbar_ctrl_ptr)
    {
        lettersbar_ctrl_ptr->is_dirty = TRUE;
        parent_vtbl_ptr->SetBothRect(base_ctrl_ptr, both_rect_ptr);
    }

    return (result);
}

/***************************************************************************//*!
@brief 获取当前滚动条的位置所对应字母的unicode
@author xiaoqing.lu nan.ji
@param      ctrl_id：［IN］滚动条的控件ID
@param      item_pos：滚动条的当前位置
@return 返回item_pos所对应的字母的unicode
@note
*******************************************************************************/
PUBLIC wchar  CTRLLETTERS_GetWchar(
                                   MMI_HANDLE_T ctrl_handle,
                                   uint16       item_pos
                                   )
{
    wchar  ret_value = 0;
    uint16  scale = 1;
    CTRLLETTERSBAR_OBJ_T* lettersbar_ctrl_ptr = (CTRLLETTERSBAR_OBJ_T*)GetLettersbarPtr (ctrl_handle);

    if (PNULL == lettersbar_ctrl_ptr)
    {
        return ret_value;
    }
    
    if (PNULL != lettersbar_ctrl_ptr->letters_ptr)
    {
        if (lettersbar_ctrl_ptr->letters_num > lettersbar_ctrl_ptr->max_disp_num)
        {
            scale = lettersbar_ctrl_ptr->letter_scale;
        }
        
        if (item_pos < lettersbar_ctrl_ptr->max_disp_num)
        {
            if (scale*item_pos + 1 < lettersbar_ctrl_ptr->letters_num)
            {
                ret_value = lettersbar_ctrl_ptr->letters_ptr[scale*item_pos];
            }
            else
            {
                ret_value = lettersbar_ctrl_ptr->letters_ptr[lettersbar_ctrl_ptr->letters_num -1];
            }
        }
    }
    
    return ret_value;
}

/*****************************************************************************/
// 	Description : set rect
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLLETTERS_SetRect(
                                   MMI_HANDLE_T ctrl_handle, 
                                   const GUI_RECT_T* rect_ptr
                                   )
{
    return LettersbarCtrlSetRect((CTRLBASE_OBJ_T*)GetLettersbarPtr(ctrl_handle), rect_ptr);
}

/*****************************************************************************/
// 	Description : set rect
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLLETTERS_SetBothRect(
                                       MMI_HANDLE_T ctrl_handle, 
                                       const GUI_BOTH_RECT_T* both_rect_ptr
                                       )
{
    return LettersbarCtrlSetBothRect((CTRLBASE_OBJ_T*)GetLettersbarPtr(ctrl_handle), both_rect_ptr);
}

/*****************************************************************************/
//  Description : display slider control
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLLETTERS_Update(
                                  MMI_HANDLE_T ctrl_handle
                                  )
{
    LettersbarDraw(GetLettersbarPtr(ctrl_handle));
    
    return TRUE;
}

/*****************************************************************************/
// 	Description : create the progress control
//	Global resource dependence : 
//  Author: xiaoqing.lu
//	Note: return ctrl handle
/*****************************************************************************/
PUBLIC MMI_HANDLE_T CTRLLETTERS_Create(
                                       MMI_HANDLE_T win_handle,
                                       MMI_HANDLE_T parent_ctrl_handle,
                                       MMI_CTRL_ID_T ctrl_id
                                       )
{
    MMI_HANDLE_T ctrl_handle = 0;
    CTRLBASE_OBJ_T *base_ctrl_ptr = PNULL;
    MMI_CONTROL_CREATE_T    create = {0};
    GUILETTERSBAR_INIT_DATA_T   init_data = {0};

    create.ctrl_id           = ctrl_id;
    create.guid              = SPRD_GUI_LETTERS_ID;
    create.parent_win_handle = win_handle;
    create.parent_ctrl_handle = parent_ctrl_handle;
    create.init_data_ptr     = &init_data;

    base_ctrl_ptr = (CTRLBASE_OBJ_T *)MMK_CreateControl( &create );
        
    if (PNULL != base_ctrl_ptr)
    {
        ctrl_handle = base_ctrl_ptr->handle;
    }

    return ctrl_handle;
}

