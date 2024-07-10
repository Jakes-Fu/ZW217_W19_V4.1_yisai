/*****************************************************************************
** File Name:      ctrlbaseflex.c                                               *
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

#include "ctrlbaseflex.h"
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
//  Description : init baseflex class
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseFlexCtrlInitVtbl (
    CTRLBASEFLEX_VTBL_T        *baseflex_vtbl_ptr
);

/*****************************************************************************/
//  Description : baseflex construct
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseFlexCtrlConstruct (
    CTRLBASE_OBJ_T           *base_ctrl_ptr,
    CTRLBASEFLEX_INIT_PARAM_T   *baseflex_param_ptr
);

/*****************************************************************************/
//  Description : baseflex destruct
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseFlexCtrlDestruct (
    CTRLBASE_OBJ_T        *base_ctrl_ptr
);

///*****************************************************************************/
////  Description : convert init param
////  Global resource dependence :
////  Author: hua.fang
////  Note:
///*****************************************************************************/
//LOCAL void BaseFlexCtrlPackInitParam (
//    GUIBASEFLEX_INIT_DATA_T     *baseflex_init_ptr,   //in
//    CTRLBASEFLEX_INIT_PARAM_T   *baseflex_param_ptr   //out
//);

/*****************************************************************************/
//  Description : handle baseflex msg function
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E BaseFlexCtrlHandleMsg (
    CTRLBASE_OBJ_T        *base_ctrl_ptr,   //control pointer
    MMI_MESSAGE_ID_E      msg_id,           //message
    DPARAM                param             //add data
);

/*****************************************************************************/
//  Description : ext type of
//  Global resource dependence :
//  Author:hua.fang
//  Note: judge control type of
/*****************************************************************************/
LOCAL BOOLEAN BaseFlexTypeOf (
    CTRLBASE_OBJ_T     *ctrl_ptr
);

/*****************************************************************************/
// 	Description : set control whether display progress
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseFlexCtrlSetScrollbarEnabled (
    CTRLBASE_OBJ_T       *base_ctrl_ptr,
    BOOLEAN              is_display
);

/*****************************************************************************/
// 	Description : set ctrl display rect
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseFlexCtrlSetDisplayRect (
    CTRLBASE_OBJ_T      *base_ctrl_ptr, //in:
    const GUI_RECT_T    *rect_ptr,      //in:
    BOOLEAN             is_update       //in:
);

/*****************************************************************************/
// 	Description : set gui control state
//	Global resource dependence :
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseFlexCtrlSetVisible (
    CTRLBASE_OBJ_T     *base_ctrl_ptr,
    BOOLEAN            is_true
);

/*****************************************************************************/
//  Description : set edit font
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseFlexCtrlSetFont (
    CTRLBASE_OBJ_T    *base_ctrl_ptr,
    GUI_FONT_ALL_T    *font_ptr
);

/*****************************************************************************/
//  Description : on rect changed
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void BaseFlexCtrlOnRectChanged (
    CTRLBASEEDIT_OBJ_T*     baseedit_ctrl_ptr
);

/*****************************************************************************/
//  Description : 获取总行数
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL uint16 BaseFlexCtrlGetLineNum (
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr,     // [in] ctrl ptr
    uint16              display_width           // [in] width
);

/*****************************************************************************/
//  Description : 检查行区域
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void BaseFlexCtrlCheckLineRect (
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr,
    GUI_RECT_T          *line_rect_ptr          // [in/out]
);

/*****************************************************************************/
//  Description : 检查被删字符
//  Global resource dependence :
//  Author: hua.fang
//  Note:
// 此方法在删除时用到，因为在删除动作时，在确保当前有字符被选中
/*****************************************************************************/
LOCAL void BaseFlexCtrlCheckDelStr (
    CTRLBASEFLEX_OBJ_T     *baseflex_ctrl_ptr
);

/*****************************************************************************/
//  Description : 检查添加的字符
//  Global resource dependence :
//  Author: hua.fang
// TRUE: commit的字符已经拷贝到actual_str_ptr;
// FALSE: 不拷贝字符，需要后续处理
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseFlexCtrlCheckCommitStr (    //[ret] TRUE: commit的字符已经拷贝到actual_str_ptr; FALSE: 不拷贝字符，需要后续处理
    CTRLBASEFLEX_OBJ_T          *baseflex_ctrl_ptr,     //in:
    GUIEDIT_ADD_STR_T           *add_str_ptr,           //in/out:
    GUIEDIT_ADD_ACTUAL_STR_T    *actual_str_ptr,        //out
    uint16                      *delete_len_ptr         //out:
);

/*****************************************************************************/
//  Description : handle edit text cancel key
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleTextCancelKey (
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr,
    BOOLEAN             is_cancel_key
);

/*****************************************************************************/
//  Description : handle edit text cancel long
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleTextCancelLong (
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr
);

/*****************************************************************************/
//  Description : handle edit tp progress bar msg
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void HandleEditTpPrg (
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr,
    uint16              top_line_index
);

/*****************************************************************************/
//  Description : 设置无内容时，显示的默认文字
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void SetDefaultString (
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr,         //in:
    wchar               *default_string_ptr,    //in:
    uint16              default_string_len      //in:
);

/*****************************************************************************/
//  Description : handle edit lose active msg
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleEditLoseActive (
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr, //in:
    BOOLEAN             is_notify           //in
);

/*****************************************************************************/
//  Description : create progress control
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL IGUICTRL_T* CreatePrgCtrl (
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr
);

/*****************************************************************************/
//  Description : handle text edit key and tp msg
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDirectionKey (
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr,
    MMI_MESSAGE_ID_E    msg_id,
    DPARAM              param
);

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

#ifdef TOUCH_PANEL_SUPPORT
/*****************************************************************************/
//  Description : set editbox whether to respond to tp msg
//  Global resource dependence :
//  Author: yuming.yang
//  Note: default handle tp msg
/*****************************************************************************/
PUBLIC void CTRLBASEFLEX_SetHandleTpMsg (
    MMI_CTRL_ID_T    ctrl_id,
    BOOLEAN          is_handle
)
{
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr = PNULL;

    baseflex_ctrl_ptr = BASEFLEX_GetPtr (ctrl_id);
    if (PNULL != baseflex_ctrl_ptr)
    {
        baseflex_ctrl_ptr->flex_theme.is_handle_tp = is_handle;
    }
}

#endif

/*****************************************************************************/
//  Description : get edit current cursor position
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint16 CTRLBASEFLEX_GetCursorCurPos (
    MMI_CTRL_ID_T ctrl_id
)
{
    uint16              cur_pos = 0;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = PNULL;

    baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*)BASEFLEX_GetPtr (ctrl_id);
    if (PNULL != baseedit_ctrl_ptr)
    {
        cur_pos = baseedit_ctrl_ptr->cursor.cur_pos;
    }

    return (cur_pos);
}

/*****************************************************************************/
//  Description : set edit current cursor position
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBASEFLEX_SetCursorCurPos (
    MMI_CTRL_ID_T    ctrl_id,    //in:
    uint16           cur_pos     //in:from 0
)
{
    BOOLEAN             result = FALSE;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = PNULL;

    baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*)BASEFLEX_GetPtr (ctrl_id);
    if ((PNULL != baseedit_ctrl_ptr) &&
        (cur_pos <= baseedit_ctrl_ptr->str_len))
    {
        VTLBASEEDIT_SetCursorPos (baseedit_ctrl_ptr, cur_pos);
        VTLBASEEDIT_SetHighlight (baseedit_ctrl_ptr, cur_pos, cur_pos, SETHIGHLIGHT_BOTH);

        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : set the hide state
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLBASEFLEX_SetCursorHideState (
    MMI_CTRL_ID_T    ctrl_id,
    BOOLEAN          is_hide
)
{
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = PNULL;

    baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*)BASEFLEX_GetPtr (ctrl_id);
    if (PNULL != baseedit_ctrl_ptr)
    {
        baseedit_ctrl_ptr->cursor.is_hide = is_hide;
    }
}

/*****************************************************************************/
//  Description : get edit string,include text,digital,phone number etc.
//  Global resource dependence :
//  Author: Jassmine
//  Note:  对于GUIEDIT_TYPE_PHONENUM类型的editbox，如果输入的电话号码首位为“+”，
//     则该位不计算在已输入字符个数之内。那么在editbox内部会多开辟一个宽字节的内存来存储“+”。
//     此时用户输入的字符数长度可能会超过给定最大值；以应用给定的字符串最大值截取editbox前面的部分。
/*****************************************************************************/
PUBLIC void CTRLBASEFLEX_GetString (
    MMI_CTRL_ID_T     ctrl_id,    //in:
    MMI_STRING_T      *str_ptr    //in:/out
)
{
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = PNULL;

    baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*)BASEFLEX_GetPtr (ctrl_id);
    if ((PNULL != baseedit_ctrl_ptr) && (PNULL != str_ptr))
    {
        //set string
		if ( GUIEDIT_TYPE_PHONENUM == baseedit_ctrl_ptr->type )
		{						
			str_ptr->wstr_ptr = baseedit_ctrl_ptr->str_ptr;
            str_ptr->wstr_len = MIN(baseedit_ctrl_ptr->str_len, baseedit_ctrl_ptr->str_max_len - 1);
		}
		else
		{
			//set string
			str_ptr->wstr_ptr = baseedit_ctrl_ptr->str_ptr;
			str_ptr->wstr_len = baseedit_ctrl_ptr->str_len;
		}
    }
}

/*****************************************************************************/
//  Description : get edit string,include text,digital,phone number etc.
//  Global resource dependence :
//  Author: Hongjun Jia
//  Note:  该函数获取editbox中字符串的所有信息；仅供GUIEDIT_TYPE_PHONE
//         类型的editbox使用。一般用户请使用CTRLBASEFLEX_GetString()
/*****************************************************************************/
PUBLIC void CTRLBASEFLEX_GetStringEx (
									MMI_CTRL_ID_T     ctrl_id,    //in:
									MMI_STRING_T      *str_ptr    //in:/out
									)
{
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = PNULL;
	
    baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*)BASEFLEX_GetPtr (ctrl_id);
    if ((PNULL != baseedit_ctrl_ptr) && (PNULL != str_ptr))
    {
        //set string
        str_ptr->wstr_ptr = baseedit_ctrl_ptr->str_ptr;
        str_ptr->wstr_len = baseedit_ctrl_ptr->str_len;
    }
}

/*****************************************************************************/
//  Description : set edit string,include text,digital,phone number etc.
//  Global resource dependence :
//  Author: Jassmine
//  Note:超出最大范围则进行截取
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBASEFLEX_SetString (
    MMI_CTRL_ID_T      ctrl_id,    //in:
    wchar              *str_ptr,   //in:
    uint16             str_len     //in:
)
{
    BOOLEAN             result = FALSE;
    GUIEDIT_UPDATE_T    update_info = {0};
    GUIEDIT_ADD_STR_T   add_str = {0};
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr = PNULL;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = PNULL;

    if ( (PNULL != str_ptr) && (0 < str_len))
    {
        //get edit pointer by control id
        baseflex_ctrl_ptr = BASEFLEX_GetPtr (ctrl_id);
        baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*)baseflex_ctrl_ptr;

        //check edit string
        if ((PNULL != baseflex_ctrl_ptr) &&
            (BASEFLEX_CheckEditString (baseflex_ctrl_ptr, str_ptr, &str_len)))
        {
#ifdef GUIF_CLIPBOARD
            BASEFLEX_CloseClipboardMenu (baseflex_ctrl_ptr);
#endif

            //clear all string
            if (0 < baseedit_ctrl_ptr->str_len)
            {
                //set highlight
                VTLBASEEDIT_SetHighlight (
                    baseedit_ctrl_ptr,
                    0, baseedit_ctrl_ptr->str_len,
                    SETHIGHLIGHT_BOTH);

                //delete all chars before cursor
                BASEFLEX_DelString (baseflex_ctrl_ptr, &update_info);
            }

            //set add string info
            add_str.commit_ptr = str_ptr;
            add_str.commit_len = str_len;

            if (GUIEDIT_TYPE_LIST == baseedit_ctrl_ptr->type)
            {
                add_str.is_list_item = TRUE;
            }

            //add string
            result = BASEFLEX_AddString (baseflex_ctrl_ptr, &update_info, &add_str, PNULL, 0);
        }
    }

    return (result);
}

/*****************************************************************************/
//  Description : insert text string before current cursor,include text,digital,phone number etc.
//  Global resource dependence :
//  Author: Jassmine
//  Note: 超出最大范围则返回FALSE
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBASEFLEX_InsertString (
    MMI_CTRL_ID_T      ctrl_id,    //in:
    wchar              *str_ptr,   //in:
    uint16             str_len     //in:
)
{
    BOOLEAN             result = FALSE;
    uint16              highlight_len = 0;
    uint16              cur_str_len = 0;
    GUIEDIT_UPDATE_T    update_info = {0};
    GUIEDIT_ADD_STR_T   add_str = {0};
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr = PNULL;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = PNULL;

    if ((PNULL != str_ptr) && (0 < str_len))
    {
        //get edit pointer by control id
        baseflex_ctrl_ptr = BASEFLEX_GetPtr (ctrl_id);
        baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*)baseflex_ctrl_ptr;

        //check edit string and if insert string
        if ((PNULL != baseflex_ctrl_ptr) &&
            (BASEFLEX_CheckEditString (baseflex_ctrl_ptr, str_ptr, &str_len)))
        {
            //get highlight len
            highlight_len = BASEEDIT_GetHighlightLen (baseedit_ctrl_ptr);

            //set current str len
            cur_str_len = (uint16) (baseedit_ctrl_ptr->str_len - highlight_len);

            //set add string info
            add_str.commit_ptr = str_ptr;
            add_str.commit_len = str_len;

            //add string
            if (BASEFLEX_AddString (baseflex_ctrl_ptr, &update_info, &add_str, PNULL, 0))
            {
                //if add str len too long
                if ((baseedit_ctrl_ptr->str_len - cur_str_len) == str_len)
                {
                    result = TRUE;
                }
            }
        }
    }

    return (result);
}

/*****************************************************************************/
//  Description : clear edit all string
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBASEFLEX_ClearAllStr (
    MMI_CTRL_ID_T    ctrl_id
)
{
    BOOLEAN             result = FALSE;
    GUIEDIT_UPDATE_T    update_info = {0};
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr = PNULL;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = PNULL;

    //get edit pointer by control id
    baseflex_ctrl_ptr = BASEFLEX_GetPtr (ctrl_id);
    baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*)baseflex_ctrl_ptr;

    if (PNULL != baseflex_ctrl_ptr)
    {
        //set highlight
        VTLBASEEDIT_SetHighlight (
                baseedit_ctrl_ptr,
                0, baseedit_ctrl_ptr->str_len,
                SETHIGHLIGHT_BOTH);

        //delete all chars before cursor
        if (BASEFLEX_DelString (baseflex_ctrl_ptr, &update_info))
        {
            //update display
            BASEFLEX_UpdateDisplay (baseflex_ctrl_ptr, &update_info);
        }

        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : clear edit all string
//  Global resource dependence :
//  Author: tao.xue
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBASEFLEX_ClearAllStrEx (
    MMI_CTRL_ID_T       ctrl_id,
    BOOLEAN             is_update
)
{
    BOOLEAN             result = FALSE;
    GUIEDIT_UPDATE_T    update_info = {0};
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr = PNULL;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = PNULL;

    //get edit pointer by control id
    baseflex_ctrl_ptr = BASEFLEX_GetPtr (ctrl_id);
    baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*)baseflex_ctrl_ptr;

    if (PNULL != baseflex_ctrl_ptr)
    {
        //set highlight
        VTLBASEEDIT_SetHighlight (
                baseedit_ctrl_ptr,
                0, baseedit_ctrl_ptr->str_len,
                SETHIGHLIGHT_BOTH);

        //delete all chars before cursor
        if (BASEFLEX_DelString (baseflex_ctrl_ptr, &update_info) && is_update)
        {
            //update display
            BASEFLEX_UpdateDisplay (baseflex_ctrl_ptr, &update_info);
        }

        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : set edit max length
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLBASEFLEX_SetMaxLen (
    MMI_CTRL_ID_T     ctrl_id,
    uint16            str_max_len
)
{
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr = PNULL;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = PNULL;

    //get edit pointer by control id
    baseflex_ctrl_ptr = BASEFLEX_GetPtr (ctrl_id);
    baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*)baseflex_ctrl_ptr;

    if ((PNULL != baseflex_ctrl_ptr) &&
        (str_max_len != baseedit_ctrl_ptr->str_max_len))
    {
        if (baseedit_ctrl_ptr->str_len > 0)
        {
            CTRLBASEFLEX_ClearAllStrEx(ctrl_id, FALSE);
        }

        //set max len
		if (GUIEDIT_TYPE_PHONENUM == baseedit_ctrl_ptr->type)
		{
			baseedit_ctrl_ptr->str_max_len = str_max_len + 1;
		}
		else
		{
			baseedit_ctrl_ptr->str_max_len = str_max_len;
		}
        SCI_ASSERT (0 < baseedit_ctrl_ptr->str_max_len); /*assert verified*/

        //free sting memory and alloc
        if (PNULL != baseedit_ctrl_ptr->str_ptr)
        {
            SCI_FREE (baseedit_ctrl_ptr->str_ptr);
            baseedit_ctrl_ptr->str_len = 0;
        }

        baseedit_ctrl_ptr->str_ptr = BASEEDIT_AllocStrMemory (baseedit_ctrl_ptr->str_max_len);

        //free line info memory and alloc
        if (PNULL != baseedit_ctrl_ptr->line_info_ptr)
        {
            SCI_FREE (baseedit_ctrl_ptr->line_info_ptr);
            baseedit_ctrl_ptr->total_line_num = 1;
            BASEEDIT_GetTopLineOffsetRange(baseedit_ctrl_ptr, &baseedit_ctrl_ptr->top_line_offset, PNULL);
        }

        baseedit_ctrl_ptr->line_info_ptr = BASEEDIT_AllocLineMemory (baseedit_ctrl_ptr->str_max_len);
        VTLBASEEDIT_SetCursorPos (baseedit_ctrl_ptr, 0);
        VTLBASEEDIT_SetHighlight (baseedit_ctrl_ptr, 0, 0, SETHIGHLIGHT_BOTH);

        VTLBASEFLEX_OnMaxLenChanged(baseflex_ctrl_ptr);
    }
}

/*****************************************************************************/
//  Description : config edit disable input char
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLBASEFLEX_ConfigDisableChar (
    MMI_CTRL_ID_T     ctrl_id,    //in:
    wchar             *value_ptr, //in:
    uint16            value_count //in:
)
{
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr = PNULL;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = PNULL;

    //get edit pointer by control id
    baseflex_ctrl_ptr = BASEFLEX_GetPtr (ctrl_id);
    baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*)baseflex_ctrl_ptr;

    if (PNULL != baseflex_ctrl_ptr)
    {
        //free disable char memory
        if (PNULL != baseflex_ctrl_ptr->disable_char_ptr)
        {
            SCI_FREE (baseflex_ctrl_ptr->disable_char_ptr);
            baseflex_ctrl_ptr->disable_char_len = 0;
        }

        if (0 < value_count)
        {
            //alloc memory
            baseflex_ctrl_ptr->disable_char_ptr = SCI_ALLOC_APP ( (value_count + 1) * sizeof (wchar));

            SCI_MEMSET (baseflex_ctrl_ptr->disable_char_ptr, 0, ( (value_count + 1) *sizeof (wchar)));

            //copy string
            MMI_WSTRNCPY (baseflex_ctrl_ptr->disable_char_ptr, (value_count + 1),
                          value_ptr, value_count,
                          value_count);
            baseflex_ctrl_ptr->disable_char_len = value_count;
        }
    }
}

/*****************************************************************************/
//  Description : config edit add enable input char
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLBASEFLEX_ConfigAddEnableChar (
    MMI_CTRL_ID_T	ctrl_id,    //in:
    wchar			*value_ptr, //in:
    uint16			value_count //in:
)
{
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr = PNULL;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = PNULL;

    //get edit pointer by control id
    baseflex_ctrl_ptr = BASEFLEX_GetPtr (ctrl_id);
    baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*)baseflex_ctrl_ptr;

    if (PNULL != baseflex_ctrl_ptr)
    {
        //free add enable char memory
        if (PNULL != baseflex_ctrl_ptr->add_enable_char_ptr)
        {
            SCI_FREE (baseflex_ctrl_ptr->add_enable_char_ptr);
            baseflex_ctrl_ptr->add_enable_char_len = 0;
        }

        if (0 < value_count)
        {
            //alloc memory
            baseflex_ctrl_ptr->add_enable_char_ptr = SCI_ALLOC_APP ( (value_count + 1) * sizeof (wchar));

            SCI_MEMSET (baseflex_ctrl_ptr->add_enable_char_ptr, 0, ( (value_count + 1) *sizeof (wchar)));

            //copy string
            MMI_WSTRNCPY (baseflex_ctrl_ptr->add_enable_char_ptr, (value_count + 1),
                          value_ptr, value_count,
                          value_count);
            baseflex_ctrl_ptr->add_enable_char_len = value_count;
        }
    }
}

/*****************************************************************************/
//  Description : set edit is display dividing line
//  Global resource dependence :
//  Author: Jassmine
//  Note: default don't display
/*****************************************************************************/
PUBLIC void CTRLBASEFLEX_SetDividingLine (
    MMI_CTRL_ID_T   ctrl_id,    //in:
    uint16          line_width, //in:0 don't display dividing line
    GUI_COLOR_T     line_color  //in:
)
{
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr = PNULL;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = PNULL;

    //get edit pointer by control id
    baseflex_ctrl_ptr = BASEFLEX_GetPtr (ctrl_id);
    baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*)baseflex_ctrl_ptr;

    if ((PNULL != baseflex_ctrl_ptr) &&
        (line_width <= baseedit_ctrl_ptr->common_theme.line_space))
    {
        //set dividing line
        baseedit_ctrl_ptr->common_theme.dividing_line_width = line_width;
        baseedit_ctrl_ptr->common_theme.dividing_line_color = line_color;
    }
}

/*****************************************************************************/
//  Description : set edit is handle red key,default handle,close current window
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLBASEFLEX_SetHandleRedKey (
    BOOLEAN         is_handle,
    MMI_CTRL_ID_T   ctrl_id
)
{
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr = PNULL;

    //get edit pointer by control id
    baseflex_ctrl_ptr = BASEFLEX_GetPtr (ctrl_id);

    if (PNULL != baseflex_ctrl_ptr)
    {
        baseflex_ctrl_ptr->flex_theme.is_handle_red_key = is_handle;
    }
}

/*****************************************************************************/
//  Description : set edit long cancel is delete all chars or delete one by one
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLBASEFLEX_SetLongCancelDelAll (
    BOOLEAN         is_del_all,
    MMI_CTRL_ID_T   ctrl_id
)
{
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr = PNULL;

    //get edit pointer by control id
    baseflex_ctrl_ptr = BASEFLEX_GetPtr (ctrl_id);

    if (PNULL != baseflex_ctrl_ptr)
    {
        //set is delete all
        baseflex_ctrl_ptr->flex_theme.is_delete_all = is_del_all;
    }
}

/*****************************************************************************/
//  Description : get cursor rect,include multi line
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBASEFLEX_GetPosInfo (
    MMI_CTRL_ID_T     ctrl_id,            //in:
    uint16            pos,                //in:
    GUI_RECT_T        *cursor_rect_ptr    //in/out:
)
{
    BOOLEAN             result = FALSE;
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr = PNULL;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = PNULL;

    //get edit pointer by control id
    baseflex_ctrl_ptr = BASEFLEX_GetPtr (ctrl_id);
    baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*)baseflex_ctrl_ptr;

    if (PNULL != baseflex_ctrl_ptr)
    {
        result = BASEEDIT_GetPosRect (baseedit_ctrl_ptr, pos, cursor_rect_ptr);
    }

    return (result);
}

/*****************************************************************************/
//  Description : get cursor rect,include multi line
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBASEFLEX_GetHighlightInfo (
    MMI_CTRL_ID_T   ctrl_id,
    uint16          *highlight_min_ptr,
    uint16          *highlight_max_ptr
)
{
    BOOLEAN             result = FALSE;
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr = PNULL;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = PNULL;

    //get edit pointer by control id
    baseflex_ctrl_ptr = BASEFLEX_GetPtr (ctrl_id);
    baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*)baseflex_ctrl_ptr;

    if (PNULL != baseflex_ctrl_ptr)
    {
        if (PNULL != highlight_min_ptr)
        {
            *highlight_min_ptr = BASEEDIT_GetHighlightMinPos (baseedit_ctrl_ptr);
        }

        if (PNULL != highlight_max_ptr)
        {
            *highlight_max_ptr = BASEEDIT_GetHighlightMaxPos (baseedit_ctrl_ptr);
        }
    }

    return (result);
}

/*****************************************************************************/
//  Description : get cursor rect,include multi line
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBASEFLEX_GetHighlightData (
    MMI_CTRL_ID_T   ctrl_id,
    GUIEDIT_HIGHLIGHT_T *highlight_data_ptr
)
{
    BOOLEAN             result = FALSE;
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr = PNULL;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = PNULL;
    
    //get edit pointer by control id
    baseflex_ctrl_ptr = BASEFLEX_GetPtr (ctrl_id);
    baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*)baseflex_ctrl_ptr;

    if (PNULL != baseflex_ctrl_ptr)
    {
        highlight_data_ptr->is_underline = baseedit_ctrl_ptr->highlight.is_underline;
        highlight_data_ptr->start_pos = baseedit_ctrl_ptr->highlight.start_pos;
        highlight_data_ptr->end_pos = baseedit_ctrl_ptr->highlight.end_pos;
        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : 设置无内容时，显示的默认文字
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBASEFLEX_SetDefaultString (
    MMI_CTRL_ID_T       ctrl_id,                //in:
    wchar               *default_string_ptr,    //in:
    uint16              default_string_len      //in:
)
{
    BOOLEAN             result = FALSE;
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr = PNULL;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = PNULL;

    //get edit pointer by control id
    baseflex_ctrl_ptr = BASEFLEX_GetPtr (ctrl_id);
    baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*)baseflex_ctrl_ptr;

    if (PNULL != baseflex_ctrl_ptr)
    {
        SetDefaultString (baseflex_ctrl_ptr, default_string_ptr, default_string_len);

        result = TRUE;
    }

    return result;
}

/*****************************************************************************/
//  Description : 设置无内容时，显示的默认文字
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBASEFLEX_SetDefaultStringById (
    MMI_CTRL_ID_T       ctrl_id,            //in:
    MMI_TEXT_ID_T       default_string_id   //in:
)
{
    BOOLEAN             result = FALSE;
    MMI_STRING_T        default_str = {0};
    CTRLBASE_OBJ_T      *base_ctrl_ptr = PNULL;
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr = PNULL;

    //get edit pointer by control id
    baseflex_ctrl_ptr = BASEFLEX_GetPtr (ctrl_id);

    if (PNULL != baseflex_ctrl_ptr)
    {
        base_ctrl_ptr = (CTRLBASE_OBJ_T*) baseflex_ctrl_ptr;

        MMITHEME_GetResText (
            default_string_id,
            base_ctrl_ptr->win_handle,
            &default_str);

        SetDefaultString (baseflex_ctrl_ptr, default_str.wstr_ptr, default_str.wstr_len);

        result = TRUE;
    }

    return result;
}

/***************************************************************************//*!
@brief 设置是否支持剪贴板
@author hua.fang
@param ctrl_id [in] 控件id
@param is_support  [in] 是否支持剪贴板，默认为支持
@note 必须在平台支持剪贴板的情况下才有效
*******************************************************************************/
PUBLIC BOOLEAN CTRLBASEFLEX_SetClipboardEnabled (
    MMI_CTRL_ID_T    ctrl_id,
    BOOLEAN          is_support
)
{
    BOOLEAN             result = FALSE;
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr = PNULL;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = PNULL;

    //get edit pointer by control id
    baseflex_ctrl_ptr = BASEFLEX_GetPtr (ctrl_id);
    baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*)baseflex_ctrl_ptr;

    if (PNULL != baseflex_ctrl_ptr)
    {
#ifdef GUIF_CLIPBOARD
        // 恢复到NORMAL状态
        if (!is_support)
        {
            BASEFLEX_ChangeDragState (baseflex_ctrl_ptr, GUIEDIT_DRAGSTATE_NORMAL);
        }

        baseflex_ctrl_ptr->is_clipbrd_support = is_support;
#endif

        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : set edit softkey,left,middle,right button
//  Global resource dependence :
//  Author: Jassmine
//  Note:
// 如果id为0，不修改原内容
/*****************************************************************************/
PUBLIC void CTRLBASEFLEX_SetSoftkey (
    MMI_CTRL_ID_T                    ctrl_id,    //in:
    uint16                           index,      //in:softkey index
    uint16                           str_len,    //in:0,不变
    MMI_TEXT_ID_T                    text_1,     //in:<str_len display text_1
    MMI_TEXT_ID_T                    text_2,     //in:>=str_len display text_2
    MMITHEME_EDIT_SET_BUTTON_PFUNC   setsoftkey  //in:优先通过函数设置softkey[in:字符串指针in:字符串长度,out:softkey值]
)
{
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr = PNULL;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = PNULL;

    //get edit pointer by control id
    baseflex_ctrl_ptr = BASEFLEX_GetPtr (ctrl_id);
    baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*)baseflex_ctrl_ptr;

    if ((PNULL != baseflex_ctrl_ptr) &&
        (index < GUISOFTKEY_BUTTON_NUM))
    {
        baseflex_ctrl_ptr->is_set_skb = TRUE;

        //set softkey
        baseedit_ctrl_ptr->common_theme.softkey.button[index].is_modify  = TRUE;
        baseedit_ctrl_ptr->common_theme.softkey.button[index].str_len    = str_len;
        baseedit_ctrl_ptr->common_theme.softkey.button[index].setsoftkey = setsoftkey;

        if (0 != text_1)
        {
            baseedit_ctrl_ptr->common_theme.softkey.button[index].text_1 = text_1;
        }

        if (0 != text_2)
        {
            baseedit_ctrl_ptr->common_theme.softkey.button[index].text_2 = text_2;
        }
    }
}

/*****************************************************************************/
//  Description : set edit softkey,left,middle,right button
//  Global resource dependence : 
//  Author: hongjun.jia
//  Note: 对GUIEDIT_SetSoftkey扩展，支持editbox 的softkey显示图片
//  如果id为0，不修改原内容
/*****************************************************************************/
PUBLIC void CTRLBASEFLEX_SetSoftkeyEx(
                               MMI_CTRL_ID_T                    ctrl_id,    //in:
                               uint16                           index,      //in:softkey index
                               uint16                           str_len,    //in:0,不变
                               MMI_TEXT_ID_T                    text_1,     //in:<str_len display text_1
                               MMI_TEXT_ID_T                    text_2,     //in:>=str_len display text_2
							   MMI_IMAGE_ID_T                   image_id,
							   GUIEDIT_BUTTON_TYPE_T            style,
                               MMITHEME_EDIT_SET_BUTTON_PFUNC   setsoftkey  //in:优先通过函数设置softkey[in:字符串指针in:字符串长度,out:softkey值]
                               )
{
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr = PNULL;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = PNULL;
	
    //get edit pointer by control id
    baseflex_ctrl_ptr = BASEFLEX_GetPtr (ctrl_id);
    baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*)baseflex_ctrl_ptr;

    if (PNULL != baseflex_ctrl_ptr &&
        (index < GUISOFTKEY_BUTTON_NUM))
    {
        baseflex_ctrl_ptr->is_set_skb = TRUE;

		if (EDIT_BUTTON_TEXT_ID == style)
		{
			baseedit_ctrl_ptr->common_theme.softkey.button[index].button_type = EDIT_BUTTON_TEXT_ID;
			//set softkey text
            CTRLBASEFLEX_SetSoftkey(ctrl_id,index,str_len,text_1,text_2,setsoftkey);
 
		} 
		else
		{
			baseedit_ctrl_ptr->common_theme.softkey.button[index].button_type = EDIT_BUTTON_IMAGE_ID;
			//set softkey button icon
			baseedit_ctrl_ptr->common_theme.softkey.button[index].is_modify  = TRUE;
			baseedit_ctrl_ptr->common_theme.softkey.button[index].image_id = image_id;
			baseedit_ctrl_ptr->common_theme.softkey.button[index].setsoftkey = setsoftkey;
		}
    }
}

/*****************************************************************************/
//  Description : set edit style,single line,multi line or adaptive
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLBASEFLEX_SetStyle (
    MMI_CTRL_ID_T      ctrl_id,
    GUIEDIT_STYLE_E    style
)
{
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr = PNULL;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = PNULL;

    //get edit pointer by control id
    baseflex_ctrl_ptr = BASEFLEX_GetPtr (ctrl_id);
    baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*)baseflex_ctrl_ptr;

    if (PNULL != baseflex_ctrl_ptr)
    {
        //SCI_ASSERT ( (GUIEDIT_TYPE_TEXT == edit_ctrl_ptr->type) || /*assert verified*/
        //             (GUIEDIT_TYPE_PHONENUM == edit_ctrl_ptr->type) ||
        //             (GUIEDIT_TYPE_DIGITAL == edit_ctrl_ptr->type) ||
        //             (GUIEDIT_TYPE_PASSWORD == edit_ctrl_ptr->type));

        if (style != baseedit_ctrl_ptr->style)
        {
            //set style
            baseedit_ctrl_ptr->style = style;

            //re-parse edit text
            BASEFLEX_ReparseEditText (baseflex_ctrl_ptr);
        }
    }
}

/*****************************************************************************/
//  Description : set edit is display im icon and number info
//  Global resource dependence :
//  Author: Jassmine
//  Note: Default:text and list edit display,others not display
/*****************************************************************************/
PUBLIC void CTRLBASEFLEX_SetDispLeftNum (
    MMI_CTRL_ID_T     ctrl_id,                //in:
    BOOLEAN           is_disp_num_info        //in:
)
{
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr = PNULL;

    //get edit pointer by control id
    baseflex_ctrl_ptr = BASEFLEX_GetPtr (ctrl_id);

    if (PNULL != baseflex_ctrl_ptr)
    {
        baseflex_ctrl_ptr->is_disp_num_info = is_disp_num_info;
    }
}

/*****************************************************************************/
//  Description : del highlight chars
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBASEFLEX_DelHighlightChars(
                                        MMI_CTRL_ID_T     ctrl_id    //in:
                                        )
{
    BOOLEAN is_ret = FALSE;
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr = PNULL;

    //get edit pointer by control id
    baseflex_ctrl_ptr = BASEFLEX_GetPtr (ctrl_id);

    if (PNULL != baseflex_ctrl_ptr)
    {
        is_ret = HandleTextCancelKey(baseflex_ctrl_ptr, FALSE);
    }

    return is_ret;
}

/*****************************************************************************/
//  Description : display title bar
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBASEFLEX_DisplayTitleInfo(
                                        MMI_CTRL_ID_T     ctrl_id    //in:
                                        )
{
    BOOLEAN is_ret = FALSE;
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr = PNULL;

    //get edit pointer by control id
    baseflex_ctrl_ptr = BASEFLEX_GetPtr (ctrl_id);

    if (PNULL != baseflex_ctrl_ptr)
    {
        BASEFLEX_DisplayInfoBar(baseflex_ctrl_ptr);
        is_ret = TRUE;
    }

    return is_ret;
}

/*****************************************************************************/
//  Description : get baseflex type
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC TYPE_TAG_T BASEFLEX_GetType()
{
    static TYPE_TAG_T    obj_type = 0;

    if (0 == obj_type)
    {
        obj_type = TYPEMNG_Register (
                       CTRL_BASEEDIT_TYPE,                                 // parent class type
                       "baseflex",                                         // control name
                       sizeof (CTRLBASEFLEX_OBJ_T),                        // size of control object struct
                       (OBJECT_CONSTRUCTOR_FUNC) BaseFlexCtrlConstruct,    // control constructor function
                       (OBJECT_DESTRUCTOR_FUNC) BaseFlexCtrlDestruct,      // control destructor function
                       0,                                                  // size of init param struct
                       PNULL,                                              // convert init param
                       sizeof (CTRLBASEFLEX_VTBL_T),                       // size of virtual table
                       (VTBL_INIT_FUNC) BaseFlexCtrlInitVtbl);             // init virtual table function
    }

    return obj_type;
}

/*****************************************************************************/
//  Description : Re-parse edit text
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN BASEFLEX_ReparseEditText (
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr  //in:
)
{
    BOOLEAN     result = FALSE;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*)baseflex_ctrl_ptr;

    if (0 < baseedit_ctrl_ptr->str_len)
    {
        //destroy progress
        if (PNULL != baseedit_ctrl_ptr->prgbox_ctrl_ptr)
        {
            //destroy progress bar control
            MMK_DestroyControl (IGUICTRL_GetCtrlHandle (baseedit_ctrl_ptr->prgbox_ctrl_ptr));
            baseedit_ctrl_ptr->prgbox_ctrl_ptr = PNULL;
        }

        //progress is change,creat or destroy progress control
        BASEFLEX_CreateOrDestroyPrgCtrl (baseflex_ctrl_ptr);

        //parse all text
        VTLBASEFLEX_ParseText (baseflex_ctrl_ptr, 0);

        //reset cursor x coordinate
        baseedit_ctrl_ptr->cursor.x_coordinate = GUIEDIT_CURSOR_INVALID_COORDINATE;

        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : init baseflex class
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseFlexCtrlInitVtbl (
    CTRLBASEFLEX_VTBL_T        *baseflex_vtbl_ptr
)
{
    CTRLBASE_VTBL_T     *base_ctrl_vtbl_ptr = (CTRLBASE_VTBL_T*) baseflex_vtbl_ptr;
    CTRLBASEEDIT_VTBL_T *baseedit_vtbl_ptr = (CTRLBASEEDIT_VTBL_T*) baseflex_vtbl_ptr;

    if (PNULL == base_ctrl_vtbl_ptr)
    {
        return FALSE;
    }

    base_ctrl_vtbl_ptr->HandleEvent = BaseFlexCtrlHandleMsg;
    base_ctrl_vtbl_ptr->SetVisible = BaseFlexCtrlSetVisible;
    base_ctrl_vtbl_ptr->SetFont = BaseFlexCtrlSetFont;

    // 以下接口用于实现内嵌form
    base_ctrl_vtbl_ptr->SetDisplayRect = BaseFlexCtrlSetDisplayRect;
    base_ctrl_vtbl_ptr->SetScrollbarEnabled = BaseFlexCtrlSetScrollbarEnabled; //有些控件不必实现

    // base edit virtual method
    baseedit_vtbl_ptr->OnRectChanged = BaseFlexCtrlOnRectChanged;
    baseedit_vtbl_ptr->GetLineNum = BaseFlexCtrlGetLineNum;
    baseedit_vtbl_ptr->DisplayAll = BaseFlexCtrlDisplayAll;
    baseedit_vtbl_ptr->CheckLineRect = BaseFlexCtrlCheckLineRect;

    // base flex virtual method
    baseflex_vtbl_ptr->CheckDelStr = BaseFlexCtrlCheckDelStr;
    baseflex_vtbl_ptr->CheckCommitStr = BaseFlexCtrlCheckCommitStr;
    baseflex_vtbl_ptr->DisplayLeftNum = BaseFlexCtrlDisplayLeftNum;
    baseflex_vtbl_ptr->GetLeftNumInfo = BaseFlexCtrlGetLeftNumInfo;
    baseflex_vtbl_ptr->ParseText = BaseFlexCtrlParseText;

    return TRUE;
}

/*****************************************************************************/
//  Description : baseflex construct
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseFlexCtrlConstruct (
    CTRLBASE_OBJ_T              *base_ctrl_ptr,
    CTRLBASEFLEX_INIT_PARAM_T   *baseflex_param_ptr
)
{
    BOOLEAN             result = TRUE;
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr = (CTRLBASEFLEX_OBJ_T*) base_ctrl_ptr;

    if (PNULL == base_ctrl_ptr || PNULL == baseflex_param_ptr)
    {
        return FALSE;
    }

    //set edit display progress
    baseflex_ctrl_ptr->is_display_prg = TRUE;

#ifdef GUIF_CLIPBOARD
    baseflex_ctrl_ptr->is_clipbrd_support = TRUE;
#endif

    MMITHEME_GetBaseFlexTheme(&baseflex_ctrl_ptr->flex_theme);

    return (result);
}

/*****************************************************************************/
//  Description : baseflex destruct
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseFlexCtrlDestruct (
    CTRLBASE_OBJ_T    *base_ctrl_ptr
)
{
    BOOLEAN             result = TRUE;
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr = (CTRLBASEFLEX_OBJ_T*) base_ctrl_ptr;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*) base_ctrl_ptr;

    if (PNULL == baseflex_ctrl_ptr)
    {
        return FALSE;
    }

#ifdef GUIF_CLIPBOARD
    //set not display clip board menu
    BASEFLEX_CloseClipboardMenu (baseflex_ctrl_ptr);

    // 销毁slider对象
    BASEFLEX_DestroyEditSlider (baseflex_ctrl_ptr);

    //stop slider timer
    BASEFLEX_StopSliderTimer (baseflex_ctrl_ptr);
#endif

    //stop cursor timer
    BASEFLEX_StopCursorTimer (baseflex_ctrl_ptr);

#ifdef TOUCH_PANEL_SUPPORT
    //stop tp scroll timer
    BASEFLEX_StopTpScrollTimer (baseflex_ctrl_ptr);

    // STOP SLIDE TIMER
    BASEFLEX_StopTpSlideTimer(baseflex_ctrl_ptr);
#endif

    if (PNULL != baseflex_ctrl_ptr->default_str_ptr)
    {
        SCI_FREE (baseflex_ctrl_ptr->default_str_ptr);
        baseflex_ctrl_ptr->default_str_len = 0;
    }

    //free disable char memory
    if (PNULL != baseflex_ctrl_ptr->disable_char_ptr)
    {
        SCI_FREE (baseflex_ctrl_ptr->disable_char_ptr);
        baseflex_ctrl_ptr->disable_char_len = 0;
    }

    //free add enable char memory
    if (PNULL != baseflex_ctrl_ptr->add_enable_char_ptr)
    {
        SCI_FREE (baseflex_ctrl_ptr->add_enable_char_ptr);
        baseflex_ctrl_ptr->add_enable_char_len = 0;
    }

    //free cursor memory
    if (PNULL != baseedit_ctrl_ptr->cursor.bg_buf_ptr)
    {
        SCI_FREE (baseedit_ctrl_ptr->cursor.bg_buf_ptr);
        baseedit_ctrl_ptr->cursor.bg_buf_size = 0;
    }

    //free prgbox control memory
    if (PNULL != baseedit_ctrl_ptr->prgbox_ctrl_ptr)
    {
        baseedit_ctrl_ptr->prgbox_ctrl_ptr = PNULL;
    }

    return (result);
}

/*****************************************************************************/
//  Description : handle baseflex msg function
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E BaseFlexCtrlHandleMsg (
    CTRLBASE_OBJ_T        *base_ctrl_ptr,   //control pointer
    MMI_MESSAGE_ID_E      msg_id,           //message
    DPARAM                param             //add data
)
{
    BOOLEAN             need_parent_handle = FALSE;
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    CTRLBASE_VTBL_T     *parent_vtbl_ptr = PNULL;
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr = (CTRLBASEFLEX_OBJ_T*) base_ctrl_ptr;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*) base_ctrl_ptr;

    if (PNULL == baseflex_ctrl_ptr)
    {
        return MMI_RESULT_FALSE;
    }

    switch (msg_id)
    {
    case MSG_CTL_LOSE_FOCUS:
        need_parent_handle = TRUE;

        //stop cursor timer
        BASEFLEX_StopCursorTimer (baseflex_ctrl_ptr);

#ifdef TOUCH_PANEL_SUPPORT
        //stop tp scroll timer
        BASEFLEX_StopTpScrollTimer (baseflex_ctrl_ptr);
#endif

#ifdef GUIF_CLIPBOARD
        // 非剪贴板下关闭滑块
        if (!baseflex_ctrl_ptr->is_show_clipbrd)
        {
            BASEFLEX_ChangeDragState (baseflex_ctrl_ptr, GUIEDIT_DRAGSTATE_NORMAL);
        }
#endif
        break;

    case MSG_CTL_GET_ACTIVE:
        need_parent_handle = TRUE;

        //for TP down active edit,discard tp move/up msg
        baseflex_ctrl_ptr->is_tp_rect_change = TRUE;

        // make cursor visible
        BASEFLEX_MakeCursorVisible(baseflex_ctrl_ptr);

        //start cursor timer
        BASEFLEX_StartCursorTimer (baseflex_ctrl_ptr);
        break;

    case MSG_CTL_LOSE_ACTIVE:
        need_parent_handle = TRUE;

        result = HandleEditLoseActive (baseflex_ctrl_ptr, FALSE);
        break;

    case MSG_NOTIFY_LOSE_ACTIVE:
        need_parent_handle = TRUE;

        result = HandleEditLoseActive (baseflex_ctrl_ptr, TRUE);
        break;

    case MSG_TIMER:
        if (baseedit_ctrl_ptr->cursor.timer_id == * (uint8*) param)
        {
            BASEFLEX_HandleCursorTimer (baseflex_ctrl_ptr);
        }
#ifdef TOUCH_PANEL_SUPPORT
        else if (baseflex_ctrl_ptr->tp_scroll_timer_id == * (uint8 *) param)
        {
            BASEFLEX_HandleTpScrollTimer (baseflex_ctrl_ptr);
        }
        else if (baseflex_ctrl_ptr->slide_timer_id == * (uint8 *) param)
        {
            BASEFLEX_HandleTpSlideTimer(baseflex_ctrl_ptr);
        }
#endif
#ifdef GUIF_CLIPBOARD
        else if (baseflex_ctrl_ptr->slider_dis_timer_id == * (uint8 *) param)
        {
            BASEFLEX_HandleSliderTimer (baseflex_ctrl_ptr);
        }
#endif
        else
        {
            need_parent_handle = TRUE;
        }
        break;

#ifdef GUIF_CLIPBOARD
    case MSG_LCD_SWITCH:
        need_parent_handle = TRUE;

        //set not display clip board menu
        BASEFLEX_CloseClipboardMenu (baseflex_ctrl_ptr);
        break;
#endif

    case MSG_APP_UP:
    case MSG_KEYREPEAT_UP:
    case MSG_APP_DOWN:
    case MSG_KEYREPEAT_DOWN:
    case MSG_APP_LEFT:
    case MSG_KEYREPEAT_LEFT:
    case MSG_APP_RIGHT:
    case MSG_KEYREPEAT_RIGHT:
        result = HandleDirectionKey (baseflex_ctrl_ptr, msg_id, param);
        break;

    case MSG_APP_CANCEL:
        if (!HandleTextCancelKey (baseflex_ctrl_ptr, TRUE))
        {
            //notify parent control
            result = BASEEDIT_NotifyParentMsg (base_ctrl_ptr->handle, MSG_NOTIFY_CANCEL);
        }
        break;

    case MSG_KEYREPEAT_CANCEL:
        if (!baseflex_ctrl_ptr->flex_theme.is_delete_all)
        {
            //delete chars one by one before cursor
            if (!HandleTextCancelKey (baseflex_ctrl_ptr, TRUE))
            {
                //notify parent control
                result = BASEEDIT_NotifyParentMsg (base_ctrl_ptr->handle, MSG_NOTIFY_CANCEL);
            }
        }
        break;

    case MSG_KEYLONG_CANCEL:
        if ((baseflex_ctrl_ptr->flex_theme.is_delete_all) &&
            (baseedit_ctrl_ptr->common_theme.is_cancel_delete))
        {
            //delete all chars before cursor
            result = HandleTextCancelLong (baseflex_ctrl_ptr);
        }
        break;

    case MSG_APP_RED:
        if (baseflex_ctrl_ptr->flex_theme.is_handle_red_key)
        {
            //notify parent control
            result = BASEEDIT_NotifyParentMsg (base_ctrl_ptr->handle, MSG_NOTIFY_CANCEL);
        }
        else
        {
            result = MMI_RESULT_FALSE;
        }
        break;

    case MSG_NOTIFY_IM_BACKSPACE:
        HandleTextCancelKey (baseflex_ctrl_ptr, FALSE);
        break;

    case MSG_NOTIFY_IM_LONG_DEL:
        if (baseflex_ctrl_ptr->flex_theme.is_delete_all)
        {
            //delete all chars before cursor
            result = HandleTextCancelLong (baseflex_ctrl_ptr);
        }
        break;

#ifdef GUIF_CLIPBOARD
    case MSG_NOTIFY_IM_HIDE:
        parent_vtbl_ptr = (CTRLBASE_VTBL_T*) TYPEMNG_GetParentVtbl (CTRL_BASEFLEX_TYPE, (OBJECT_TYPE_PTR) base_ctrl_ptr);
        parent_vtbl_ptr->HandleEvent (base_ctrl_ptr, msg_id, param);

        // TODO: bugbug. may not
        // fix NEWMS00174274
        if (baseedit_ctrl_ptr->is_rect_adaptive)
        {
            BASEFLEX_ShowSlider (baseflex_ctrl_ptr);
        }
        break;
#endif


    case MSG_NOTIFY_IM_SETCURSOR:
        {
            BOOLEAN is_update_str = FALSE;
            BOOLEAN is_set_cursor = FALSE;
            uint16  old_cursor_value = 0;
            uint16  cursor_value = 0;
            int16   cursor_offset = (int16)SETCURSOR_VALUE(param);
            uint16  prev_first_char_index = 0;
            uint16  prev_top_line_index = 0;

            switch (SETCURSOR_TYPE(param))
            {
            case GUIIM_SETCURSOR_TYPE_CUR:
                if (baseedit_ctrl_ptr->cursor.cur_pos + cursor_offset >= 0
                    && baseedit_ctrl_ptr->cursor.cur_pos + cursor_offset <= baseedit_ctrl_ptr->str_len)
                {
                    cursor_value = baseedit_ctrl_ptr->cursor.cur_pos + cursor_offset;
                    is_set_cursor = TRUE;
                }
                break;

            case GUIIM_SETCURSOR_TYPE_START:
                if ((cursor_offset >= 0 && cursor_offset <= baseedit_ctrl_ptr->str_len)
#ifdef _SNMC_SC6530_BUG_FIX_
                    &&(GUIEDIT_TYPE_TIME != baseedit_ctrl_ptr->type)
#endif   
                    )
                {
                    cursor_value = cursor_offset;
                    is_set_cursor = TRUE;
                }
                break;

            case GUIIM_SETCURSOR_TYPE_END:
                if ((cursor_offset <= 0 && cursor_offset >= -baseedit_ctrl_ptr->str_len) 
#ifdef _SNMC_SC6530_BUG_FIX_
                    &&(GUIEDIT_TYPE_TIME != baseedit_ctrl_ptr->type)
#endif                    
                    )
                {
                    cursor_value = baseedit_ctrl_ptr->str_len + cursor_offset;
                    is_set_cursor = TRUE;
                }
                break;

            default:
                break;
            }

            if (is_set_cursor)
            {
        		//get previous cursor position
                old_cursor_value = baseedit_ctrl_ptr->cursor.cur_pos;

                //get previous top line index and first char index
                prev_top_line_index = baseedit_ctrl_ptr->top_line_offset;
                prev_first_char_index = baseedit_ctrl_ptr->line_info_ptr[0].start_pos;

                //reset date cursor not display
                BASEFLEX_ResetCursor(baseflex_ctrl_ptr);

                if (baseedit_ctrl_ptr->highlight.start_pos != baseedit_ctrl_ptr->highlight.end_pos)
                {
                    //reset highlight not display
                    BASEEDIT_DisplayHighlight(baseedit_ctrl_ptr, FALSE);
                }

                VTLBASEEDIT_SetCursorPos(baseedit_ctrl_ptr, cursor_value);

                //set first char index
                if (GUIEDIT_STYLE_SINGLE == baseedit_ctrl_ptr->style)
                {
                    if (baseedit_ctrl_ptr->line_info_ptr[0].start_pos > baseedit_ctrl_ptr->cursor.cur_pos)
                    {
                        baseedit_ctrl_ptr->line_info_ptr[0].start_pos = baseedit_ctrl_ptr->cursor.cur_pos;
                    }

                    BASEFLEX_GetFirstCharIndex(baseflex_ctrl_ptr);

                    if (prev_first_char_index != baseedit_ctrl_ptr->line_info_ptr[0].start_pos)
                    {
                        //need update edit all string
                        is_update_str = TRUE;
                    }
                }
                else
                {
                    //set top line index
                    BASEFLEX_MakeCursorVisible (baseflex_ctrl_ptr);

                    if (prev_top_line_index != baseedit_ctrl_ptr->top_line_offset)
                    {
                        //need update edit all string
                        is_update_str = TRUE;
                    }
                }

                //set no highlight
                BASEEDIT_SetNoHighlight(baseedit_ctrl_ptr, cursor_value);

                // when on list item
                if (baseedit_ctrl_ptr->highlight.start_pos != baseedit_ctrl_ptr->highlight.end_pos)
                {
                    baseedit_ctrl_ptr->cursor.cur_pos = baseedit_ctrl_ptr->highlight.start_pos;
                    BASEFLEX_MakeCursorVisible (baseflex_ctrl_ptr);

                    baseedit_ctrl_ptr->cursor.cur_pos = baseedit_ctrl_ptr->highlight.end_pos;
                    BASEFLEX_MakeCursorVisible (baseflex_ctrl_ptr);

                    if (prev_top_line_index != baseedit_ctrl_ptr->top_line_offset)
                    {
                        //need update edit all string
                        is_update_str = TRUE;
                    }
                }

                //update text display for move cursor
                BASEFLEX_UpdateForMoveCursor(baseflex_ctrl_ptr, is_update_str, old_cursor_value);

                baseedit_ctrl_ptr->cursor.x_coordinate = GUIEDIT_CURSOR_INVALID_COORDINATE;
            }
        }
        break;

    case MSG_NOTIFY_IM_GETCURSOR:
        *((uint16*)param) = baseedit_ctrl_ptr->cursor.cur_pos;
        break;

    case MSG_NOTIFY_IM_SET_HIGHLIGHT:
        {
            uint16  start_index = SET_HIGHLIGHT_START(param);
            uint16  end_index = SET_HIGHLIGHT_END(param);

            if (start_index <= baseedit_ctrl_ptr->str_len
                && end_index <= baseedit_ctrl_ptr->str_len)
            {
                //reset date cursor not display
                BASEFLEX_ResetCursor(baseflex_ctrl_ptr);

                VTLBASEEDIT_SetCursorPos(baseedit_ctrl_ptr, end_index);

                baseedit_ctrl_ptr->cursor.x_coordinate  = GUIEDIT_CURSOR_INVALID_COORDINATE;

                //send move cursor msg
                BASEEDIT_NotifyCursorMsg(baseedit_ctrl_ptr);

                //reset highlight not display
                BASEEDIT_DisplayHighlight(baseedit_ctrl_ptr, FALSE);

                VTLBASEEDIT_SetHighlight(baseedit_ctrl_ptr,
                    start_index,
                    end_index,
                    SETHIGHLIGHT_BOTH);

                // cursor will be between min_pos and max_pos while moving upword,
                // so check cursor and make it at the beginning of the highlight.
                // must call after GUIEDIT_SetHighlight, otherwise it will be error.
                BASEFLEX_CheckCursor (baseflex_ctrl_ptr);

                //display highlight
                BASEEDIT_DisplayHighlight(baseedit_ctrl_ptr, TRUE);

                baseedit_ctrl_ptr->cursor.is_display    = TRUE;
                baseedit_ctrl_ptr->cursor.is_rect_valid = FALSE;
                BASEFLEX_DisplayCursor(baseflex_ctrl_ptr);
            }
        }
        break;

    case MSG_NOTIFY_IM_SWICTH_MODE:
        //only update info bar
        BASEFLEX_DisplayInfoBar (baseflex_ctrl_ptr);
        break;

    case MSG_NOTIFY_IM_COMMIT:
        {
            GUIEDIT_UPDATE_T    update_info = {0};
            GUIEDIT_ADD_STR_T   add_str = {0};

            BASEEDIT_ConvertImCommitMsg(param, &add_str);

            if (BASEFLEX_AddString (baseflex_ctrl_ptr, &update_info, &add_str, PNULL, 0))
            {
                //update display
                BASEFLEX_UpdateDisplay (baseflex_ctrl_ptr, &update_info);
            }
        }
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_DOWN:
        if (baseflex_ctrl_ptr->flex_theme.is_handle_tp)
        {
            BASEFLEX_HandleEditTpDown (baseflex_ctrl_ptr, param);
        }
        else
        {
            result = MMI_RESULT_FALSE;
        }
        break;

    case MSG_TP_PRESS_SHORT:
        if (baseflex_ctrl_ptr->flex_theme.is_handle_tp)
        {
            result = BASEFLEX_HandleTpShort (baseflex_ctrl_ptr);
        }
        else
        {
            result = MMI_RESULT_FALSE;
        }
        break;

    case MSG_TP_PRESS_MOVE:
        if (baseflex_ctrl_ptr->flex_theme.is_handle_tp)
        {
            BASEFLEX_HandleTpMove (baseflex_ctrl_ptr, param);
        }
        else
        {
            result = MMI_RESULT_FALSE;
        }
        break;

    case MSG_TP_PRESS_UP:
        if (baseflex_ctrl_ptr->flex_theme.is_handle_tp)
        {
            // 父类在UP消息里起输入法
            need_parent_handle = TRUE;

            BASEFLEX_HandleEditTpUp (baseflex_ctrl_ptr, param);
        }
        else
        {
            result = MMI_RESULT_FALSE;
        }
        break;

    case MSG_TP_PRESS_LONG:
        if (baseflex_ctrl_ptr->flex_theme.is_handle_tp)
        {
            BASEFLEX_HandleTpLong (baseflex_ctrl_ptr, param);
        }
        else
        {
            result = MMI_RESULT_FALSE;
        }
        break;
#endif

#ifdef GUIF_CLIPBOARD
    case MSG_NOTIFY_CLIPBRD_COPY:
        BASEFLEX_HandleEditCopyMsg (baseflex_ctrl_ptr);
        break;

    case MSG_NOTIFY_CLIPBRD_CUT:
        BASEFLEX_HandleEditCutMsg (baseflex_ctrl_ptr);
        break;

    case MSG_NOTIFY_CLIPBRD_PASTE:
        BASEFLEX_HandleEditPasetMsg (baseflex_ctrl_ptr);
        break;

    case MSG_NOTIFY_CLIPBRD_SELECTALL:
        BASEFLEX_HandleEditSelectAllMsg (baseflex_ctrl_ptr);
        break;

    case MSG_NOTIFY_CLIPBRD_SELECT:
        BASEFLEX_HandleSelectClipbrdMsg (baseflex_ctrl_ptr);
        break;

    case MSG_NOTIFY_CLIPBRD_MENU_CLOSED:
        baseflex_ctrl_ptr->clipbrd_win_handle = 0;
        baseflex_ctrl_ptr->is_show_clipbrd = FALSE;

        // NEWMS00173721: 剪贴板menu关闭后，将滑块也隐藏
        if (!MMK_IsFocusWin (base_ctrl_ptr->win_handle))
        {
            BASEFLEX_ChangeDragState (baseflex_ctrl_ptr, GUIEDIT_DRAGSTATE_NORMAL);
        }
        break;

    case MSG_NOTIFY_BEGIN_SLIDER:
        BASEFLEX_HandleBeginSlider (baseflex_ctrl_ptr, param);
        break;

    case MSG_NOTIFY_MOVE_SLIDER:
        BASEFLEX_HandleMoveSlider (baseflex_ctrl_ptr, param);
        break;

    case MSG_NOTIFY_END_SLIDER:
        BASEFLEX_HandleEndSlider (baseflex_ctrl_ptr, param);
        break;
#endif

    case MSG_CTL_PRGBOX_MOVEUP_ONE:
    case MSG_CTL_PRGBOX_MOVEDOWN_ONE:
    case MSG_CTL_PRGBOX_MOVEUP_PAGE:
    case MSG_CTL_PRGBOX_MOVEDOWN_PAGE:
    case MSG_CTL_PRGBOX_MOVE:
        HandleEditTpPrg ( baseflex_ctrl_ptr, (uint16) (* (uint32*) param));
        break;

    default:
        need_parent_handle = TRUE;
        break;
    }

    if (need_parent_handle)
    {
        parent_vtbl_ptr = (CTRLBASE_VTBL_T*) TYPEMNG_GetParentVtbl (CTRL_BASEFLEX_TYPE, (OBJECT_TYPE_PTR) base_ctrl_ptr);
        result = parent_vtbl_ptr->HandleEvent (base_ctrl_ptr, msg_id, param);
    }

    return (result);
}

/*****************************************************************************/
// 	Description : set gui control state
//	Global resource dependence :
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseFlexCtrlSetVisible (
    CTRLBASE_OBJ_T     *base_ctrl_ptr,
    BOOLEAN            is_true
)
{
    BOOLEAN             result = FALSE;
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr = (CTRLBASEFLEX_OBJ_T*)base_ctrl_ptr;
    CTRLBASE_VTBL_T     *parent_vtbl_ptr = PNULL;

    if (PNULL != baseflex_ctrl_ptr)
    {
#ifdef GUIF_CLIPBOARD

        // 显隐控件时，要将剪贴板关闭
        if ((base_ctrl_ptr->is_visible && !is_true) ||
            (!base_ctrl_ptr->is_visible && is_true))
        {
            BASEFLEX_ChangeDragState (baseflex_ctrl_ptr, GUIEDIT_DRAGSTATE_NORMAL);
        }

#endif

        parent_vtbl_ptr = (CTRLBASE_VTBL_T*) TYPEMNG_GetParentVtbl (CTRL_BASEFLEX_TYPE, base_ctrl_ptr);
        result = parent_vtbl_ptr->SetVisible (base_ctrl_ptr, is_true);
    }

    return result;
}

/*****************************************************************************/
// 	Description : set control whether display progress
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseFlexCtrlSetScrollbarEnabled (
    CTRLBASE_OBJ_T       *base_ctrl_ptr,
    BOOLEAN              is_display
)
{
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr = (CTRLBASEFLEX_OBJ_T*)base_ctrl_ptr;

    //set is display progress
    baseflex_ctrl_ptr->is_display_prg = is_display;

    return TRUE;
}

/*****************************************************************************/
// 	Description : set ctrl display rect
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseFlexCtrlSetDisplayRect (
    CTRLBASE_OBJ_T      *base_ctrl_ptr, //in:
    const GUI_RECT_T    *rect_ptr,      //in:
    BOOLEAN             is_update       //in:
)
{
    CTRLBASE_VTBL_T* base_vtbl_ptr = (CTRLBASE_VTBL_T*) TYPEMNG_GetParentVtbl (CTRL_BASEFLEX_TYPE, base_ctrl_ptr);

#ifdef GUIF_CLIPBOARD
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr = (CTRLBASEFLEX_OBJ_T*)base_ctrl_ptr;

    if (!GUI_EqualRect (base_ctrl_ptr->display_rect, *rect_ptr))
    {
        BASEFLEX_ChangeDragState (baseflex_ctrl_ptr, GUIEDIT_DRAGSTATE_NORMAL);
    }
#endif

    return base_vtbl_ptr->SetDisplayRect (base_ctrl_ptr, rect_ptr, is_update);
}

/*****************************************************************************/
//  Description : set edit font
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseFlexCtrlSetFont (
    CTRLBASE_OBJ_T    *base_ctrl_ptr,
    GUI_FONT_ALL_T    *font_ptr
)
{
    BOOLEAN             result = TRUE;
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr = (CTRLBASEFLEX_OBJ_T*)base_ctrl_ptr;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T *) base_ctrl_ptr;

    // font color
    baseedit_ctrl_ptr->common_theme.font.color = font_ptr->color;

    // font size
    if (baseedit_ctrl_ptr->common_theme.font.font != font_ptr->font)
    {
        baseedit_ctrl_ptr->common_theme.font.font = font_ptr->font;

        //re-parse edit text
        BASEFLEX_ReparseEditText (baseflex_ctrl_ptr);

        //notify edit change
        BASEEDIT_NotifyLineMsg (baseedit_ctrl_ptr, 0);
    }

    return (result);
}

/*****************************************************************************/
//  Description : on rect changed
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void BaseFlexCtrlOnRectChanged (
    CTRLBASEEDIT_OBJ_T*     baseedit_ctrl_ptr
)
{
    CTRLBASEFLEX_OBJ_T*     baseflex_ctrl_ptr = (CTRLBASEFLEX_OBJ_T*) baseedit_ctrl_ptr;

#ifdef GUIF_CLIPBOARD
    BASEFLEX_ChangeDragState (baseflex_ctrl_ptr, GUIEDIT_DRAGSTATE_NORMAL);
#endif

    //re-parse edit text
    BASEFLEX_ReparseEditText (baseflex_ctrl_ptr);

    //set top line index
    BASEFLEX_MakeCursorVisible (baseflex_ctrl_ptr);
}

/*****************************************************************************/
//  Description : 获取总行数
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL uint16 BaseFlexCtrlGetLineNum (
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr,     // [in] ctrl ptr
    uint16              display_width           // [in] width
)
{
    uint16              total_line_num = 0;
    wchar               *str_ptr = baseedit_ctrl_ptr->str_ptr;
    wchar               str_len = baseedit_ctrl_ptr->str_len;
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr = (CTRLBASEFLEX_OBJ_T*) baseedit_ctrl_ptr;

    if (GUIEDIT_STYLE_SINGLE == baseedit_ctrl_ptr->style)
    {
        total_line_num = 1;
    }
    else
    {
        total_line_num = GUI_CalculateStringLinesByPixelNum (
                             display_width,
                             str_ptr,
                             str_len,
                             baseedit_ctrl_ptr->common_theme.font.font,
                             (uint8) baseedit_ctrl_ptr->common_theme.char_space,
                             baseflex_ctrl_ptr->is_byword);

        //is carriage return or line feed
        if (BASEEDIT_IsNewLineChar (baseedit_ctrl_ptr, str_len))
        {
            //cursor display the next line
            total_line_num++;
        }

        //must one line
        if (0 == total_line_num)
        {
            total_line_num = 1;
        }
    }

    return (total_line_num);
}

/*****************************************************************************/
//  Description : 检查行区域
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void BaseFlexCtrlCheckLineRect (
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr,
    GUI_RECT_T          *line_rect_ptr          // [in/out]
)
{
    uint16          prg_width = 0;
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr = (CTRLBASEFLEX_OBJ_T*) baseedit_ctrl_ptr;

    //need scroll bar
    if (PNULL != line_rect_ptr &&
        PNULL != baseedit_ctrl_ptr->prgbox_ctrl_ptr)
    {
        prg_width = MMITHEME_GetScrollBarWidthByState(baseflex_ctrl_ptr->flex_theme.is_support_slide);
        SCI_ASSERT ((line_rect_ptr->right - line_rect_ptr->left + 1) > prg_width); /*assert verified*/

        if (MMI_DIRECTION_RIGHT == MMITHEME_GetDisDirection())
        {
            line_rect_ptr->left = (int16) (line_rect_ptr->left + prg_width);
        }
        else
        {
            line_rect_ptr->right = (int16) (line_rect_ptr->right - prg_width);
        }
    }
}

/*****************************************************************************/
//  Description : 检查被删字符
//  Global resource dependence :
//  Author: hua.fang
//  Note:
// 此方法在删除时用到，因为在删除动作时，要确保当前有字符被选中
/*****************************************************************************/
LOCAL void BaseFlexCtrlCheckDelStr (
    CTRLBASEFLEX_OBJ_T     *baseflex_ctrl_ptr
)
{
    uint16              highlight_len = 0;
    uint16		        cursor_pos = 0;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*)baseflex_ctrl_ptr;

    //get highlight len
    highlight_len = BASEEDIT_GetHighlightLen (baseedit_ctrl_ptr);

    if (0 == highlight_len)
    {
        //set highlight
        SCI_ASSERT (0 < baseedit_ctrl_ptr->cursor.cur_pos); /*assert verified*/

        //adjust cursor position
        cursor_pos = (uint16) (baseedit_ctrl_ptr->cursor.cur_pos - 1);
        //BASEFLEX_AdjustCursorPos (baseflex_ctrl_ptr, TRUE, &cursor_pos);

        //set highlight
        VTLBASEEDIT_SetHighlight (
            baseedit_ctrl_ptr,
            cursor_pos,
            baseedit_ctrl_ptr->cursor.cur_pos,
            SETHIGHLIGHT_BOTH);
    }
}

/*****************************************************************************/
//  Description : 检查添加的字符
//  Global resource dependence :
//  Author: hua.fang
// TRUE: commit的字符已经拷贝到actual_str_ptr;
// FALSE: 不拷贝字符，需要后续处理
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseFlexCtrlCheckCommitStr (    //[ret] TRUE: commit的字符已经拷贝到actual_str_ptr; FALSE: 不拷贝字符，需要后续处理
    CTRLBASEFLEX_OBJ_T          *baseflex_ctrl_ptr,     //in:
    GUIEDIT_ADD_STR_T           *add_str_ptr,           //in/out:
    GUIEDIT_ADD_ACTUAL_STR_T    *actual_str_ptr,        //out
    uint16                      *delete_len_ptr         //out:
)
{
    uint16      highlight_len = 0;
    uint16      str_max_len = 0;
    uint16      current_str_len = 0;
    uint16      str_commit_len = 0;
    uint16      str_underline_len = 0;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*)baseflex_ctrl_ptr;

    SCI_ASSERT (PNULL != delete_len_ptr); /*assert verified*/

    //get highlight len
    highlight_len = BASEEDIT_GetHighlightLen (baseedit_ctrl_ptr);
    SCI_ASSERT (highlight_len <= baseedit_ctrl_ptr->str_len); /*assert verified*/

    //get string max length
    str_max_len = baseedit_ctrl_ptr->str_max_len;

    //get current string length except underline
    current_str_len = (uint16) (baseedit_ctrl_ptr->str_len - highlight_len);

    //get commit length and underline length
    str_commit_len    = add_str_ptr->commit_len;
    str_underline_len = add_str_ptr->underline_len;

    //adjust commit and underline length by string max len
    if ((current_str_len + str_commit_len) > str_max_len)
    {
        //edit full,is delete
        if ((baseflex_ctrl_ptr->is_del_after_full) &&
            (0 < baseedit_ctrl_ptr->cursor.cur_pos))
        {
            *delete_len_ptr = (uint16) (current_str_len + str_commit_len - str_max_len);

            if (str_commit_len > highlight_len)
            {
                *delete_len_ptr = (uint16) (*delete_len_ptr + highlight_len);
            }
        }
        else
        {
            str_commit_len    = (uint16) (str_max_len - current_str_len);
            str_underline_len = 0;
        }
    }
    else if ( (current_str_len + str_commit_len + str_underline_len) > str_max_len)
    {
        //edit full,is delete
        if ((baseflex_ctrl_ptr->is_del_after_full) &&
            (0 < baseedit_ctrl_ptr->cursor.cur_pos))
        {
            SCI_ASSERT (0 == highlight_len); /*assert verified*/
            *delete_len_ptr = (uint16) (current_str_len + str_commit_len + str_underline_len - str_max_len);
        }
        else
        {
            str_underline_len = (uint16) (str_max_len - current_str_len - str_commit_len);
        }
    }
	if (GUIEDIT_TYPE_PHONENUM == baseedit_ctrl_ptr->type)
	{
		if ((1 == str_max_len - current_str_len) && (!baseedit_ctrl_ptr->is_first_plus))
		{
			str_commit_len = 0;
			str_underline_len = 0;
		}
	}
    //set commit and underline length
    add_str_ptr->commit_len    = str_commit_len;
    add_str_ptr->underline_len = str_underline_len;

    // 必须返回FALSE，因为这里不拷贝字符，需要后续处理
    return FALSE;
}

/*****************************************************************************/
//  Description : get baseflex pointer by control id
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC CTRLBASEFLEX_OBJ_T* BASEFLEX_GetPtr (
    MMI_HANDLE_T     ctrl_handle
)
{
    CTRLBASE_OBJ_T  *ctrl_ptr = (CTRLBASE_OBJ_T*) MMK_GetCtrlPtr (ctrl_handle);

    if (PNULL != ctrl_ptr)
    {
        SCI_ASSERT (BaseFlexTypeOf (ctrl_ptr)); /*assert verified*/
    }

    return (CTRLBASEFLEX_OBJ_T*) ctrl_ptr;
}

/*****************************************************************************/
//  Description : ext type of
//  Global resource dependence :
//  Author:hua.fang
//  Note: judge control type of
/*****************************************************************************/
LOCAL BOOLEAN BaseFlexTypeOf (
    CTRLBASE_OBJ_T     *ctrl_ptr
)
{
    return TYPEMNG_IsTypeOf ( (OBJECT_TYPE_PTR) ctrl_ptr, CTRL_BASEFLEX_TYPE);
}

/*****************************************************************************/
//  Description : handle text edit key and tp msg
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDirectionKey (
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr,
    MMI_MESSAGE_ID_E    msg_id,
    DPARAM              param
)
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*)baseflex_ctrl_ptr;

    switch (msg_id)
    {
    case MSG_APP_UP:
    case MSG_KEYREPEAT_UP:
        if ((GUIEDIT_STYLE_SINGLE != baseedit_ctrl_ptr->style) &&
            (BASEFLEX_MoveCursor (baseflex_ctrl_ptr, GUIEDIT_CURSOR_MOVE_UP)))
        {
            result = MMI_RESULT_TRUE;
        }
        else
        {
            result = MMI_RESULT_FALSE;
        }

        break;

    case MSG_APP_DOWN:
    case MSG_KEYREPEAT_DOWN:
        if ((GUIEDIT_STYLE_SINGLE != baseedit_ctrl_ptr->style) &&
            (BASEFLEX_MoveCursor (baseflex_ctrl_ptr, GUIEDIT_CURSOR_MOVE_DOWN)))
        {
            result = MMI_RESULT_TRUE;
        }
        else
        {
            result = MMI_RESULT_FALSE;
        }

        break;

    case MSG_APP_LEFT:
    case MSG_KEYREPEAT_LEFT:
        if (BASEFLEX_MoveCursor (baseflex_ctrl_ptr, GUIEDIT_CURSOR_MOVE_LEFT))
        {
            result = MMI_RESULT_TRUE;
        }
        else
        {
            result = MMI_RESULT_FALSE;
        }

        break;

    case MSG_APP_RIGHT:
    case MSG_KEYREPEAT_RIGHT:
        if (BASEFLEX_MoveCursor (baseflex_ctrl_ptr, GUIEDIT_CURSOR_MOVE_RIGHT))
        {
            result = MMI_RESULT_TRUE;
        }
        else
        {
            result = MMI_RESULT_FALSE;
        }

        break;

    default:
        GUI_INVALID_PARAM (param);/*lint !e522*/
        result = MMI_RESULT_FALSE;
        break;
    }

    if (MMI_RESULT_TRUE == result)
    {
        //config im
        VTLBASEEDIT_ConfigImKey (baseedit_ctrl_ptr);
    }

    return (result);
}

/*****************************************************************************/
//  Description : handle edit text cancel key
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleTextCancelKey (
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr,
    BOOLEAN             is_cancel_key
)
{
    BOOLEAN             result = FALSE;
    GUIEDIT_UPDATE_T    update_info = {0};
    CTRLBASE_OBJ_T      *base_ctrl_ptr = (CTRLBASE_OBJ_T*) baseflex_ctrl_ptr;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*)baseflex_ctrl_ptr;

    if (0 < baseedit_ctrl_ptr->str_len)
    {
        //if window intercept control msg
        if ((PNULL != baseflex_ctrl_ptr->WinInterceptCtrlMsgFunc) &&
            (baseflex_ctrl_ptr->WinInterceptCtrlMsgFunc (base_ctrl_ptr->win_handle,
                        MSG_APP_CANCEL,
                        baseedit_ctrl_ptr->str_ptr,
                        baseedit_ctrl_ptr->str_len)))
        {
            result = TRUE;
            //SCI_TRACE_LOW:"HandleTextCancelKey:window intercept the cancel msg!"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, GUIEDIT_4020_112_2_18_3_15_42_122, (uint8*) "");
        }
        else
        {
            //is delete
            if (is_cancel_key)
            {
                if (baseedit_ctrl_ptr->common_theme.is_cancel_delete)
                {
                    result = TRUE;
                }
            }
            else
            {
                result = TRUE;
            }

            if (result)
            {
                if (BASEFLEX_DelString (baseflex_ctrl_ptr, &update_info))
                {
                    //update display
                    BASEFLEX_UpdateDisplay (baseflex_ctrl_ptr, &update_info);
                }
            }
        }
    }

    return (result);
}

/*****************************************************************************/
//  Description : handle edit text cancel long
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleTextCancelLong (
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr
)
{
    uint16              cur_pos = 0;
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    GUIEDIT_UPDATE_T    update_info = {0};
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*)baseflex_ctrl_ptr;

    if (0 < baseedit_ctrl_ptr->cursor.cur_pos)
    {
        cur_pos = (uint16) MAX (baseedit_ctrl_ptr->highlight.start_pos, baseedit_ctrl_ptr->highlight.end_pos);
        cur_pos = (uint16) MAX (cur_pos, baseedit_ctrl_ptr->cursor.cur_pos);

        //set highlight
        VTLBASEEDIT_SetHighlight (baseedit_ctrl_ptr, 0, cur_pos, SETHIGHLIGHT_BOTH);

        //delete all chars before cursor
        if (BASEFLEX_DelString (baseflex_ctrl_ptr, &update_info))
        {
            //update display
            BASEFLEX_UpdateDisplay (baseflex_ctrl_ptr, &update_info);
        }
    }

    return (result);
}

/*****************************************************************************/
//  Description : handle edit tp progress bar msg
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void HandleEditTpPrg (
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr,
    uint16              top_line_offset
)
{
    uint16              display_height = 0;
    uint32              total_height = 0;
    GUIEDIT_UPDATE_T    update_info = {0};
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*)baseflex_ctrl_ptr;

    total_height = BASEEDIT_GetTotalHeight(baseedit_ctrl_ptr);

    if ((top_line_offset < total_height) &&
        (top_line_offset != baseedit_ctrl_ptr->top_line_offset))
    {
        display_height = BASEEDIT_GetDispHeight(baseedit_ctrl_ptr);

        //set top line index
        if (top_line_offset <= total_height - display_height)
        {
            baseedit_ctrl_ptr->top_line_offset = top_line_offset;
        }
        else
        {
            baseedit_ctrl_ptr->top_line_offset = (int32) (total_height - display_height);
        }

        //update display
        update_info.update_mode = GUIEDIT_UPDATE_ALL;
        BASEFLEX_UpdateDisplay (baseflex_ctrl_ptr, &update_info);
    }
}

/*****************************************************************************/
//  Description : handle edit lose active msg
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleEditLoseActive (
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr, //in:
    BOOLEAN             is_notify           //in
)
{
    BOOLEAN             is_handle = FALSE;
    MMI_RESULT_E        result = MMI_RESULT_FALSE;
    CTRLBASE_OBJ_T      *base_ctrl_ptr = (CTRLBASE_OBJ_T*) baseflex_ctrl_ptr;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*)baseflex_ctrl_ptr;

#ifdef GUIF_CLIPBOARD
    BASEFLEX_ChangeDragState (baseflex_ctrl_ptr, GUIEDIT_DRAGSTATE_NORMAL);
#endif

    if (is_notify)
    {
        if (!MMK_IsActiveCtrl (base_ctrl_ptr->handle))
        {
            is_handle = TRUE;
        }
    }
    else
    {
        is_handle = TRUE;
    }

    if (is_handle)
    {
        //reset the display of cursor
		BASEFLEX_ResetCursor (baseflex_ctrl_ptr);

#ifdef TOUCH_PANEL_SUPPORT
        //stop tp scroll timer
        BASEFLEX_StopTpScrollTimer (baseflex_ctrl_ptr);
#endif

        BASEEDIT_SetNoHighlight (baseedit_ctrl_ptr, baseedit_ctrl_ptr->cursor.cur_pos);
    }

    return (result);
}

/*****************************************************************************/
//  Description : progress is change,creat or destroy progress control
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN BASEFLEX_CreateOrDestroyPrgCtrl (//is update all
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr  //in:
)
{
    BOOLEAN     result = FALSE;
    uint16      edit_width = 0;
    uint16      total_line_num = 0;
    GUI_RECT_T  edit_rect = {0};
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*)baseflex_ctrl_ptr;

    if ((baseflex_ctrl_ptr->is_display_prg) &&
        (GUIEDIT_STYLE_MULTI == baseedit_ctrl_ptr->style))
    {
        //get edit box rect
        edit_rect = BASEEDIT_GetClientRect (baseedit_ctrl_ptr);

        //get edit box width
        edit_width = (uint16) (edit_rect.right - edit_rect.left + 1);

        //get total number
        total_line_num = VTLBASEEDIT_GetLineNum (baseedit_ctrl_ptr, edit_width);

        //is need progress
        if (total_line_num > BASEEDIT_GetPageLineNum(baseedit_ctrl_ptr))
        {
            if (PNULL == baseedit_ctrl_ptr->prgbox_ctrl_ptr)
            {
                //create progress bar control
                baseedit_ctrl_ptr->prgbox_ctrl_ptr = CreatePrgCtrl (baseflex_ctrl_ptr);

                result = TRUE;
            }
        }
        else
        {
            if (PNULL != baseedit_ctrl_ptr->prgbox_ctrl_ptr)
            {
                //destroy progress bar control
                MMK_DestroyControl (IGUICTRL_GetCtrlHandle (baseedit_ctrl_ptr->prgbox_ctrl_ptr));
                baseedit_ctrl_ptr->prgbox_ctrl_ptr = PNULL;

                result = TRUE;
            }
        }
    }

    return (result);
}

/*****************************************************************************/
//  Description : get scroll bar rect
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC GUI_RECT_T BASEFLEX_GetScrollBarRect (
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr
)
{
    uint16          prg_width = 0;
    GUI_RECT_T      prg_rect = {0};
    GUI_RECT_T      edit_rect = {0};
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*)baseflex_ctrl_ptr;

    //get edit rect
    edit_rect = BASEEDIT_GetClientRect (baseedit_ctrl_ptr);

    //set prg rect
    prg_rect = edit_rect;

    prg_width = MMITHEME_GetScrollBarWidthByState(baseflex_ctrl_ptr->flex_theme.is_support_slide);

    if (MMI_DIRECTION_RIGHT == MMITHEME_GetDisDirection())
    {
        prg_rect.right = (int16) (prg_rect.left + prg_width - 1);
    }
    else
    {
        prg_rect.left = (int16) (prg_rect.right - prg_width + 1);
    }

    //NEWMS00193624 1.改变了edit下的prgbox的rect（原来计算left240，right239）。
    if (prg_rect.left >= prg_rect.right)
    {
        prg_rect.left = prg_rect.right - 2;
    }

    return (prg_rect);
}

/*****************************************************************************/
//  Description : create progress control
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL IGUICTRL_T* CreatePrgCtrl (
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr
)
{
    GUI_RECT_T              prg_rect = {0};
    IGUICTRL_T              *prgbox_ctrl_ptr = PNULL;
    MMI_CONTROL_CREATE_T    ctrl_create = {0};
    GUIPRGBOX_INIT_DATA_T   init_data = {0};
    CTRLBASE_OBJ_T          *base_ctrl_ptr = (CTRLBASE_OBJ_T*) baseflex_ctrl_ptr;

    //get progress bar rect
    prg_rect = BASEFLEX_GetScrollBarRect (baseflex_ctrl_ptr);

    //init data
    init_data.lcd_id = base_ctrl_ptr->lcd_dev_info.lcd_id;
    init_data.both_rect.h_rect = init_data.both_rect.v_rect   = prg_rect;
    init_data.style  = GUIPRGBOX_STYLE_VERTICAL_WINDOWS;

    //creat progress
    ctrl_create.guid               = SPRD_GUI_PRGBOX_ID;
    ctrl_create.init_data_ptr      = &init_data;
    ctrl_create.parent_ctrl_handle = base_ctrl_ptr->handle;
    ctrl_create.parent_win_handle  = base_ctrl_ptr->win_handle;

    //creat progress bar control
    prgbox_ctrl_ptr = MMK_CreateControl (&ctrl_create);

    return (prgbox_ctrl_ptr);
}

/*****************************************************************************/
//  Description : 设置无内容时，显示的默认文字
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void SetDefaultString (
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr,         //in:
    wchar               *default_string_ptr,    //in:
    uint16              default_string_len      //in:
)
{
    if (PNULL != baseflex_ctrl_ptr->default_str_ptr)
    {
        SCI_FREE (baseflex_ctrl_ptr->default_str_ptr);
        baseflex_ctrl_ptr->default_str_len = 0;
    }

    if (default_string_len > 0)
    {
        baseflex_ctrl_ptr->default_str_ptr = SCI_ALLOC_APP (sizeof (wchar) * (default_string_len + 1));
        SCI_MEMSET (baseflex_ctrl_ptr->default_str_ptr, 0, sizeof (wchar) * (default_string_len + 1));

        MMI_WSTRNCPY (
            baseflex_ctrl_ptr->default_str_ptr,
            default_string_len,
            default_string_ptr,
            default_string_len,
            default_string_len);

        baseflex_ctrl_ptr->default_str_len = default_string_len;
    }
}

