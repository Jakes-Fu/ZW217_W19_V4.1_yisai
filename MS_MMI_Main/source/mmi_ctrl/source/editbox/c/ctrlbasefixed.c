/*****************************************************************************
** File Name:      ctrlbasefixed.c                                               *
** Author:                                                                   *
** Date:           07/18/2012                                                *
** Copyright:      2004 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 07/2012      hua.fang              Creat
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

#include "ctrlbasefixed.h"

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
//  Description : init basefixed class
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseFixedCtrlInitVtbl (
    CTRLBASEFIXED_VTBL_T        *basefixed_vtbl_ptr
);

/*****************************************************************************/
//  Description : basefixed construct
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseFixedCtrlConstruct (
    CTRLBASE_OBJ_T           *base_ctrl_ptr,
    CTRLBASEFIXED_INIT_PARAM_T   *basefixed_param_ptr
);

/*****************************************************************************/
//  Description : basefixed destruct
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseFixedCtrlDestruct (
    CTRLBASE_OBJ_T        *base_ctrl_ptr
);

///*****************************************************************************/
////  Description : convert init param
////  Global resource dependence :
////  Author: hua.fang
////  Note:
///*****************************************************************************/
//LOCAL void BaseFixedCtrlPackInitParam (
//    GUIBASEFIXED_INIT_DATA_T     *basefixed_init_ptr,   //in
//    CTRLBASEFIXED_INIT_PARAM_T   *basefixed_param_ptr   //out
//);

/*****************************************************************************/
//  Description : handle basefixed msg function
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E BaseFixedCtrlHandleMsg (
    CTRLBASE_OBJ_T        *base_ctrl_ptr,   //control pointer
    MMI_MESSAGE_ID_E      msg_id,           //message
    DPARAM                param             //add data
);

/*****************************************************************************/
//  Description : get basefixed pointer by control id
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL CTRLBASEFIXED_OBJ_T* GetBaseFixedPtr (
    MMI_HANDLE_T     ctrl_handle
);

/*****************************************************************************/
//  Description : ext type of
//  Global resource dependence :
//  Author:hua.fang
//  Note: judge control type of
/*****************************************************************************/
LOCAL BOOLEAN BaseFixedTypeOf (
    CTRLBASE_OBJ_T     *ctrl_ptr
);

/*****************************************************************************/
// 	Description : config im default key
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL void BaseFixedCtrlConfigImKey (
    CTRLBASEEDIT_OBJ_T     *baseedit_ctrl_ptr
);

/*****************************************************************************/
//  Description : 获取总行数
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL uint16 BaseFixedCtrlGetLineNum (
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr,     // [in] ctrl ptr
    uint16              display_width           // [in] width
);

/*****************************************************************************/
//  Description : 显示所有内容
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void BaseFixedCtrlDisplayAll (
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr,
    BOOLEAN             is_update_prgbox
);

#ifdef TOUCH_PANEL_SUPPORT
/*****************************************************************************/
//  Description : handle fixed format edit tp down msg
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void HandleFixedFormatEditTpDown (
    CTRLBASEFIXED_OBJ_T *basefixed_ctrl_ptr, //in:
    DPARAM              param           //in:
);

/*****************************************************************************/
//  Description : get fixed format edit tp char offset
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetFixedFormatTpCharOffset (
    CTRLBASEFIXED_OBJ_T *basefixed_ctrl_ptr,
    GUI_POINT_T         tp_point,           //in:
    uint16              *char_offset_ptr,   //in/out:
    uint16              *highlight_len_ptr  //in/out:
);
#endif

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : get basefixed type
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC TYPE_TAG_T BASEFIXED_GetType()
{
    static TYPE_TAG_T    obj_type = 0;

    if (0 == obj_type)
    {
        obj_type = TYPEMNG_Register (
                       CTRL_BASEEDIT_TYPE,                                  // parent class type
                       "basefixed",                                         // control name
                       sizeof (CTRLBASEFIXED_OBJ_T),                        // size of control object struct
                       (OBJECT_CONSTRUCTOR_FUNC) BaseFixedCtrlConstruct,    // control constructor function
                       (OBJECT_DESTRUCTOR_FUNC) BaseFixedCtrlDestruct,      // control destructor function
                       0,                                                   // size of init param struct
                       PNULL,                                               // convert init param
                       sizeof (CTRLBASEFIXED_VTBL_T),                       // size of virtual table
                       (VTBL_INIT_FUNC) BaseFixedCtrlInitVtbl);             // init virtual table function
    }

    return obj_type;
}

/*****************************************************************************/
//  Description : init basefixed class
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseFixedCtrlInitVtbl (
    CTRLBASEFIXED_VTBL_T        *basefixed_vtbl_ptr
)
{
    CTRLBASE_VTBL_T  *base_ctrl_vtbl_ptr = (CTRLBASE_VTBL_T*) basefixed_vtbl_ptr;
    CTRLBASEEDIT_VTBL_T *baseedit_ctrl_vtbl_ptr = (CTRLBASEEDIT_VTBL_T*) basefixed_vtbl_ptr;

    if (PNULL == base_ctrl_vtbl_ptr)
    {
        return FALSE;
    }

    base_ctrl_vtbl_ptr->HandleEvent = BaseFixedCtrlHandleMsg;

    baseedit_ctrl_vtbl_ptr->ConfigImKey = BaseFixedCtrlConfigImKey;
    baseedit_ctrl_vtbl_ptr->GetLineNum = BaseFixedCtrlGetLineNum;
    baseedit_ctrl_vtbl_ptr->DisplayAll = BaseFixedCtrlDisplayAll;

    return TRUE;
}

/*****************************************************************************/
//  Description : basefixed construct
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseFixedCtrlConstruct (
    CTRLBASE_OBJ_T           *base_ctrl_ptr,
    CTRLBASEFIXED_INIT_PARAM_T   *basefixed_param_ptr
)
{
    BOOLEAN result = TRUE;

    return (result);
}

/*****************************************************************************/
//  Description : basefixed destruct
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseFixedCtrlDestruct (
    CTRLBASE_OBJ_T    *base_ctrl_ptr
)
{
    BOOLEAN                 result = TRUE;
    CTRLBASEFIXED_OBJ_T     *basefixed_ctrl_ptr = (CTRLBASEFIXED_OBJ_T*) base_ctrl_ptr;

    if (PNULL == basefixed_ctrl_ptr)
    {
        return FALSE;
    }

    // TODO
    // ...

    return (result);
}

/*****************************************************************************/
//  Description : handle basefixed msg function
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E BaseFixedCtrlHandleMsg (
    CTRLBASE_OBJ_T        *base_ctrl_ptr,   //control pointer
    MMI_MESSAGE_ID_E      msg_id,           //message
    DPARAM                param             //add data
)
{
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    CTRLBASE_VTBL_T     *parent_vtbl_ptr = PNULL;
    CTRLBASEFIXED_OBJ_T *basefixed_ctrl_ptr = (CTRLBASEFIXED_OBJ_T*) base_ctrl_ptr;

    if (PNULL == basefixed_ctrl_ptr)
    {
        return MMI_RESULT_FALSE;
    }

    switch (msg_id)
    {
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_DOWN:
        HandleFixedFormatEditTpDown (basefixed_ctrl_ptr, param);
        break;
#endif

    case MSG_APP_RED:
        result = MMI_RESULT_FALSE;
        break;

    default:
        parent_vtbl_ptr = (CTRLBASE_VTBL_T*) TYPEMNG_GetParentVtbl (CTRL_BASEFIXED_TYPE, (OBJECT_TYPE_PTR) base_ctrl_ptr);
        result = parent_vtbl_ptr->HandleEvent (base_ctrl_ptr, msg_id, param);
        break;
    }

    return (result);
}

/*****************************************************************************/
// 	Description : config im default key
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL void BaseFixedCtrlConfigImKey (
    CTRLBASEEDIT_OBJ_T     *baseedit_ctrl_ptr
)
{
    wchar       *star_value_ptr = PNULL;
    uint32      star_value_count = 0;
    wchar       hash_value_array[1] = {'#'};
    uint32      hash_value_count = 0;

    if (0 != baseedit_ctrl_ptr->im_ctrl_handle)
    {
        //config im *
        GUIIM_SetCustomKey (
            baseedit_ctrl_ptr->im_ctrl_handle,
            GUIIM_TYPE_DIGITAL,
            GUIIM_CUSTOM_KEY_STAR,
            star_value_ptr,
            star_value_count);

        //config im #
        GUIIM_SetCustomKey (
            baseedit_ctrl_ptr->im_ctrl_handle,
            GUIIM_TYPE_DIGITAL,
            GUIIM_CUSTOM_KEY_HASH,
            hash_value_array,
            hash_value_count);

    }
}

/*****************************************************************************/
//  Description : 获取总行数
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL uint16 BaseFixedCtrlGetLineNum (
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr,     // [in] ctrl ptr
    uint16              display_width           // [in] width
)
{
    return 1;
}

/*****************************************************************************/
//  Description : 显示所有内容
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void BaseFixedCtrlDisplayAll (
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr,
    BOOLEAN             is_update_prgbox
)
{
    GUI_RECT_T      cross_rect = {0};
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*) baseedit_ctrl_ptr;

    //get crossed rect
    if (GUI_IntersectRect (&cross_rect, base_ctrl_ptr->display_rect, base_ctrl_ptr->rect))
    {
        //display bg
        BASEEDIT_DisplayBg (baseedit_ctrl_ptr, cross_rect);

        if (baseedit_ctrl_ptr->is_permit_border)
        {
            //display border
            GUI_DisplayBorder (
                base_ctrl_ptr->rect,
                cross_rect,
                &baseedit_ctrl_ptr->common_theme.border,
                &base_ctrl_ptr->lcd_dev_info);
        }

        VTLBASEFIXED_ConvertToStr((CTRLBASEFIXED_OBJ_T*)baseedit_ctrl_ptr);

        //display highlight
        BASEEDIT_DisplayHighlight (baseedit_ctrl_ptr, TRUE);

        //set display cursor
        baseedit_ctrl_ptr->cursor.is_rect_valid = FALSE;
    }
}

/*****************************************************************************/
//  Description : get basefixed pointer by control id
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL CTRLBASEFIXED_OBJ_T* GetBaseFixedPtr (
    MMI_HANDLE_T     ctrl_handle
)
{
    CTRLBASE_OBJ_T  *ctrl_ptr = (CTRLBASE_OBJ_T*) MMK_GetCtrlPtr (ctrl_handle);

    if (PNULL != ctrl_ptr)
    {
        SCI_ASSERT (BaseFixedTypeOf (ctrl_ptr)); /*assert verified*/
    }

    return (CTRLBASEFIXED_OBJ_T*) ctrl_ptr;
}

/*****************************************************************************/
//  Description : ext type of
//  Global resource dependence :
//  Author:hua.fang
//  Note: judge control type of
/*****************************************************************************/
LOCAL BOOLEAN BaseFixedTypeOf (
    CTRLBASE_OBJ_T     *ctrl_ptr
)
{
    return TYPEMNG_IsTypeOf ( (OBJECT_TYPE_PTR) ctrl_ptr, CTRL_BASEFIXED_TYPE);
}

#ifdef TOUCH_PANEL_SUPPORT
/*****************************************************************************/
//  Description : handle fixed format edit tp down msg
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void HandleFixedFormatEditTpDown (
    CTRLBASEFIXED_OBJ_T *basefixed_ctrl_ptr, //in:
    DPARAM              param           //in:
)
{
    uint16          char_offset = 0;
    uint16          highlight_len = 0;
    GUI_POINT_T     tp_point = {0};
    CTRLBASEEDIT_OBJ_T *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*)basefixed_ctrl_ptr;

    //set tp press point
    tp_point.x = MMK_GET_TP_X (param);
    tp_point.y = MMK_GET_TP_Y (param);

    //get tp char offset
    if (GetFixedFormatTpCharOffset (basefixed_ctrl_ptr, tp_point, &char_offset, &highlight_len))
    {
        //set cursor position
        VTLBASEEDIT_SetCursorPos (baseedit_ctrl_ptr, char_offset);
        baseedit_ctrl_ptr->cursor.x_coordinate  = GUIEDIT_CURSOR_INVALID_COORDINATE;

        //set highlight
        VTLBASEEDIT_SetHighlight (
            baseedit_ctrl_ptr,
            char_offset,
            (uint16) (char_offset + highlight_len),
            SETHIGHLIGHT_BOTH);

        VTLBASEEDIT_DisplayAll(baseedit_ctrl_ptr, FALSE);
    }
}

/*****************************************************************************/
//  Description : get fixed format edit tp char offset
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetFixedFormatTpCharOffset (
    CTRLBASEFIXED_OBJ_T *basefixed_ctrl_ptr,
    GUI_POINT_T         tp_point,           //in:
    uint16              *char_offset_ptr,   //in/out:
    uint16              *highlight_len_ptr  //in/out:
)
{
    BOOLEAN         result = FALSE;
    uint8           font_width = 0;
    uint16          i = 0;
    uint16          char_offset = 0;
    GUI_RECT_T      text_rect = {0};
    CTRLBASEEDIT_OBJ_T *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*)basefixed_ctrl_ptr;

    //get text rect
    text_rect = BASEEDIT_GetTextRect (baseedit_ctrl_ptr, 0);

    if ((PNULL != baseedit_ctrl_ptr->str_ptr) &&
        (0 < baseedit_ctrl_ptr->str_len) &&
        (tp_point.y >= text_rect.top) &&
        (tp_point.y <= text_rect.bottom))
    {
        //Adjust rect left position by align
        BASEEDIT_AdjustLeftByAlign (baseedit_ctrl_ptr, 0, &text_rect);

        for (i = 0; i < baseedit_ctrl_ptr->str_len; i++)
        {
            font_width = GUI_GetFontWidth (baseedit_ctrl_ptr->common_theme.font.font, baseedit_ctrl_ptr->str_ptr[i]);

            if ((tp_point.x >= text_rect.left) &&
                (tp_point.x < text_rect.left + font_width + baseedit_ctrl_ptr->common_theme.char_space))
            {
                char_offset = i;
                result = TRUE;
                break;
            }

            text_rect.left = (int16) (text_rect.left + font_width + baseedit_ctrl_ptr->common_theme.char_space);
        }

        if (result)
        {
            // check whether the char offset is valid
            result = VTLBASEFIXED_IsValidOffset (
                         basefixed_ctrl_ptr,
                         &char_offset, highlight_len_ptr);
        }
    }

    *char_offset_ptr = char_offset;

    return (result);
}

#endif

/*****************************************************************************/
//  Description : get bit value
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint8 BASEFIXED_CalculateBitValue (
    BOOLEAN  is_add,     //in
    uint8    bit_value,  //in
    uint8    bit_min,    //in
    uint8    bit_max
)
{
    uint8   new_bit_value = bit_value;

    if (is_add)
    {
        if (new_bit_value < bit_max)
        {
            new_bit_value++;
        }
        else
        {
            new_bit_value = bit_min;
        }
    }
    else
    {
        if (new_bit_value > bit_min)
        {
            new_bit_value--;
        }
        else
        {
            new_bit_value = bit_max;
        }
    }

    return (new_bit_value);
}

/*****************************************************************************/
//  Description : convert number key to number
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN BASEFIXED_GetNumValueByIm (
    uint8                *num_value_ptr, //in/out:
    GUIEDIT_ADD_STR_T    *add_str_ptr    //in:
)
{
    BOOLEAN     result = FALSE;

    if ((PNULL != num_value_ptr) &&
        (PNULL != add_str_ptr) &&
        (1 == add_str_ptr->commit_len) &&
        (0 == add_str_ptr->underline_len))
    {
        if (('0' <= add_str_ptr->commit_ptr[0]) &&
            ('9' >= add_str_ptr->commit_ptr[0]))
        {
            *num_value_ptr = (uint8) (add_str_ptr->commit_ptr[0] - '0');

            result = TRUE;
        }
    }

    return (result);
}

