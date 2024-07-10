/*****************************************************************************
** File Name:      ctrlcombox.c                                              *
** Author:                                                                   *
** Date:           07/18/2012                                                *
** Copyright:      2004 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 07/2012      xiyuan.ma & nan.ji              Creat
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
#include "guictrl_api.h"
#include "ctrlcombox.h"
#include "ctrldropdownlist_export.h"
#include "ctrlbutton_export.h"
#include "mmitheme_im.h"
#include "mmitheme_combox.h"

#include "ctrltextedit_export.h"
//#include "ctrldigitaledit_export.h"
//#include "ctrlphonenumedit_export.h"
//#include "ctrlpswedit_export.h"
//#include "ctrllistedit_export.h"

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
//  Description : init combox class
//  Global resource dependence :
//  Author: xiyuan.ma & nan.ji
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ComboxCtrlInitVtbl (
    CTRLCOMBOX_VTBL_T        *combox_vtbl_ptr
);

/*****************************************************************************/
//  Description : combox construct
//  Global resource dependence :
//  Author: xiyuan.ma & nan.ji
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ComboxCtrlConstruct (
    CTRLBASE_OBJ_T           *base_ctrl_ptr,
    CTRLCOMBOX_INIT_PARAM_T   *combox_param_ptr
);

/*****************************************************************************/
//  Description : combox destruct
//  Global resource dependence :
//  Author: xiyuan.ma & nan.ji
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ComboxCtrlDestruct (
    CTRLBASE_OBJ_T        *base_ctrl_ptr
);

/*****************************************************************************/
//  Description : convert init param
//  Global resource dependence :
//  Author: xiyuan.ma & nan.ji
//  Note:
/*****************************************************************************/
LOCAL void ComboxCtrlPackInitParam (
    CTRLCOMBOX_INIT_DATA_T     *combox_init_ptr,   //in
    CTRLCOMBOX_INIT_PARAM_T   *combox_param_ptr   //out
);

/*****************************************************************************/
//  Description : handle combox msg function
//  Global resource dependence :
//  Author: xiyuan.ma & nan.ji
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E ComboxCtrlHandleMsg (
    CTRLBASE_OBJ_T        *base_ctrl_ptr,   //control pointer
    MMI_MESSAGE_ID_E      msg_id,           //message
    DPARAM                param             //add data
);

/*****************************************************************************/
//  Description : display combox control
//  Global resource dependence :
//  Author: xiyuan.ma & nan.ji
//  Note:
/*****************************************************************************/
LOCAL void ComboxDraw (
    CTRLCOMBOX_OBJ_T     *combox_ctrl_ptr
);

/*****************************************************************************/
//  Description : get combox pointer by control id
//  Global resource dependence :
//  Author: xiyuan.ma & nan.ji
//  Note:
/*****************************************************************************/
LOCAL CTRLCOMBOX_OBJ_T* GetComboxPtr (
    MMI_HANDLE_T     ctrl_handle
);

/*****************************************************************************/
//  Description : ext type of
//  Global resource dependence :
//  Author:xiyuan.ma & nan.ji
//  Note: judge control type of
/*****************************************************************************/
LOCAL BOOLEAN ComboxTypeOf (
    CTRLBASE_OBJ_T     *ctrl_ptr
);

/*****************************************************************************/
// 	Description : get control height by width,include content,border etc.
//	Global resource dependence :
//  Author: xiyuan.ma & nan.ji
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN ComboxCtrlGetHeightByWidth (
    CTRLBASE_OBJ_T      *base_ctrl_ptr,  //in:
    uint16              width,          //in:
    uint16              *height_ptr     //in/out:
);

/*****************************************************************************/
// 	Description : set ctrl display rect
//	Global resource dependence :
//  Author: xiyuan.ma & nan.ji
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN ComboxCtrlSetDisplayRect (
    CTRLBASE_OBJ_T      *base_ctrl_ptr, //in:
    const GUI_RECT_T    *rect_ptr,      //in:
    BOOLEAN             is_update       //in:
);

/*****************************************************************************/
//  Description : Calc Expand Button Rect
//  Global resource dependence :
//  Author: nan ji & xiyuan ma
//  Note:
/*****************************************************************************/
LOCAL GUI_RECT_T CalcExpandButtonRect (
    CTRLCOMBOX_OBJ_T     * combox_ctrl_ptr
);

/*****************************************************************************/
//  Description : Calc Func Button Rect
//  Global resource dependence :
//  Author: nan ji & xiyuan ma
//  Note:
/*****************************************************************************/
LOCAL GUI_RECT_T CalcFuncButtonRect (
    CTRLCOMBOX_OBJ_T     * combox_ctrl_ptr
);

/*****************************************************************************/
//  Description : Calc icon Rect
//  Global resource dependence :
//  Author: nan ji & xiyuan ma
//  Note:
/*****************************************************************************/
LOCAL GUI_RECT_T CalcIconRect (
    CTRLCOMBOX_OBJ_T     * combox_ctrl_ptr
);

/*****************************************************************************/
//  Description : Calc List Rect
//  Global resource dependence :
//  Author: xiyuan.ma & nan.ji
//  Note:
/*****************************************************************************/
LOCAL GUI_RECT_T CalcListRect (
    CTRLCOMBOX_OBJ_T     * combox_ctrl_ptr
);

/*****************************************************************************/
// 	Description : set ctrl display rect
//	Global resource dependence :
//  Author: xiyuan.ma & nan.ji
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN ComboxCtrlSetRect (
    CTRLBASE_OBJ_T      *base_ctrl_ptr, //in:
    const GUI_RECT_T    *rect_ptr       //in:
);

/*****************************************************************************/
// 	Description : set ctrl rect
//	Global resource dependence :
//  Author: xiyuan.ma & nan.ji
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN ComboxCtrlSetBothRect (
    CTRLBASE_OBJ_T      *base_ctrl_ptr, //in:
    const GUI_BOTH_RECT_T    *both_rect_ptr
);

/*****************************************************************************/
//  Description : display bg
//  Global resource dependence :
//  Author: xiyuan.ma & nan.ji
//  Note:
/*****************************************************************************/
LOCAL void ComboxDrawBg (
    CTRLCOMBOX_OBJ_T     * combox_ctrl_ptr
);

/*****************************************************************************/
//  Description : Draw ExpandButton
//  Global resource dependence :
//  Author: xiyuan.ma & nan.ji
//  Note:
/*****************************************************************************/
LOCAL void ComboxDrawExpandButton (
    CTRLCOMBOX_OBJ_T     * combox_ctrl_ptr
);

/*****************************************************************************/
//  Description : Draw Function Button
//  Global resource dependence :
//  Author: xiyuan.ma & nan.ji
//  Note:
/*****************************************************************************/
LOCAL void ComboxDrawFunctionButton (
    CTRLCOMBOX_OBJ_T     * combox_ctrl_ptr
);

/*****************************************************************************/
//  Description : display bg
//  Global resource dependence :
//  Author: xiyuan.ma & nan.ji
//  Note:
/*****************************************************************************/
LOCAL void ComboxDrawIcon (
    CTRLCOMBOX_OBJ_T     * combox_ctrl_ptr
);

/*****************************************************************************/
//  Description : Draw Function Button
//  Global resource dependence :
//  Author: xiyuan.ma & nan.ji
//  Note:
/*****************************************************************************/
LOCAL void     ComboxDrawList (
    CTRLCOMBOX_OBJ_T *combox_ctrl_ptr
);

/*****************************************************************************/
//  Description : Draw Function Button
//  Global resource dependence :
//  Author: xiyuan.ma & nan.ji
//  Note:
/*****************************************************************************/
LOCAL void ComboxDrawText (
    CTRLCOMBOX_OBJ_T     * combox_ctrl_ptr
);

/*****************************************************************************/
//  Description : Create Expand Button
//  Global resource dependence :
//  Author: nan ji & xiyuan ma
//  Note:
/*****************************************************************************/
LOCAL void CreateExpandButton (
    CTRLBASE_OBJ_T *base_ctrl_ptr
);

/*****************************************************************************/
//  Description : Create Func Button
//  Global resource dependence :
//  Author: nan ji & xiyuan ma
//  Note:
/*****************************************************************************/
LOCAL void CreateFuncButton (
    CTRLBASE_OBJ_T *base_ctrl_ptr
);

/*****************************************************************************/
//  Description : create dynamic List
//  Global resource dependence :
//  Author: xiyuan.ma & nan.ji
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CreateListBox (
    CTRLBASE_OBJ_T *base_ctrl_ptr
);

/*****************************************************************************/
//  Description : Create Text Ctrl
//  Global resource dependence :
//  Author: nan ji & xiyuan ma
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CreateTextCtrl (
    CTRLBASE_OBJ_T *base_ctrl_ptr
);

/*****************************************************************************/
//  Description : Create Expand Button
//  Global resource dependence :
//  Author: nan ji & xiyuan ma
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E ExpandButtonCallBack (
    MMI_HANDLE_T ctrl_handle
);

/*****************************************************************************/
// 	Description : expand the list
//	Global resource dependence :
//  Author: xiyuan.ma & nan.ji
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E ExpandList (
    CTRLBASE_OBJ_T *base_ctrl_ptr
);

/*****************************************************************************/
//  Description : 计算下拉框，下拉时的区域和原始区域的合并区域
//  Global resource dependence :
//  Author: xiyuan.ma & nan.ji
//  Note:
/*****************************************************************************/
LOCAL void GetWholeRect (
    CTRLBASE_OBJ_T *obj_ptr ,   // [in]
    GUI_RECT_T *whole_rect_ptr,                 // [out] whole rect
    GUI_RECT_T *whole_display_rect_ptr          // [out] that not include invisible rect
);

/*****************************************************************************/
//  Description : send notify
//  Global resource dependence :
//  Author: xiyuan.ma & nan.ji
//  Note:
/*****************************************************************************/
LOCAL void NotifyResize (
    CTRLBASE_OBJ_T *obj_ptr
);

/*****************************************************************************/
// 	Description : select one from drop down list
//	Global resource dependence :
//  Author: xiyuan.ma & nan.ji
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E SelectFromList (
    CTRLBASE_OBJ_T *obj_ptr
);

/*****************************************************************************/
// 	Description : select text to edit
//	Global resource dependence :
//  Author: xiyuan.ma & nan.ji
//	Note:
/*****************************************************************************/
LOCAL void SetTextToEditCtrl (
    CTRLBASE_OBJ_T *obj_ptr
);

/*****************************************************************************/
//  Description : Calc Text Rect
//  Global resource dependence :
//  Author: nan ji & xiyuan ma
//  Note:
/*****************************************************************************/
LOCAL GUI_RECT_T CalcTextRect (
    CTRLCOMBOX_OBJ_T     * combox_ctrl_ptr
);

/*****************************************************************************/
//  Description : handle combox up msg function
//  Global resource dependence :
//  Author: xiyuan.ma & nan.ji
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ComboxCtrlHandleTpUp (
    CTRLCOMBOX_OBJ_T *combox_ctrl_ptr,   //control pointer
    DPARAM           param             //add data
);
/*****************************************************************************/
// 	Description : Set List Visiable
//	Global resource dependence :
//  Author: xiyuan.ma & nan.ji
//	Note:
/*****************************************************************************/
LOCAL void SetListVisiable (
    CTRLCOMBOX_OBJ_T * combox_ctrl_ptr,
    BOOLEAN is_visiable
);

/*****************************************************************************/
// 	Description : Set List border
//	Global resource dependence :
//  Author: xiyuan.ma & nan.ji
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN ComboxSetBorder (
    CTRLBASE_OBJ_T *base_ctrl_ptr,
    GUI_BORDER_T  *border_ptr
);

/*****************************************************************************/
//  Description : display bg
//  Global resource dependence :
//  Author: xiyuan.ma & nan.ji
//  Note:
/*****************************************************************************/
LOCAL void ComboxDrawBorder (
    CTRLCOMBOX_OBJ_T     * combox_ctrl_ptr
);

/*****************************************************************************/
// 	Description : select one from drop down list
//	Global resource dependence :
//  Author: xiyuan.ma & nan.ji
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E ShrinkList (
    CTRLBASE_OBJ_T *obj_ptr
);

/*****************************************************************************/
// 	Description : Set the back ground color.
//	Global resource dependence :
//  Author:
//	Note:设置控件的背景,FALSE,设置错误,TRUE设置成功
/*****************************************************************************/
LOCAL BOOLEAN ComboxSetBackGround (
    CTRLBASE_OBJ_T *base_ctrl_ptr,
    GUI_BG_T *bg_ptr
);


/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : get combox type
//  Global resource dependence :
//  Author: xiyuan.ma & nan.ji
//  Note:
/*****************************************************************************/
PUBLIC TYPE_TAG_T COMBOX_GetType()
{
    static TYPE_TAG_T    obj_type = 0;

    if (0 == obj_type)
    {
        obj_type = TYPEMNG_Register (
                       CTRL_BASE_TYPE,                                  // parent class type
                       "combox",                                         // control name
                       sizeof (CTRLCOMBOX_OBJ_T),                        // size of control object struct
                       (OBJECT_CONSTRUCTOR_FUNC) ComboxCtrlConstruct,    // control constructor function
                       (OBJECT_DESTRUCTOR_FUNC) ComboxCtrlDestruct,      // control destructor function
                       sizeof (CTRLCOMBOX_INIT_PARAM_T),                 // size of init param struct
                       (PARAM_PACKING_FUNC) ComboxCtrlPackInitParam,     // convert init param
                       sizeof (CTRLCOMBOX_VTBL_T),                       // size of virtual table
                       (VTBL_INIT_FUNC) ComboxCtrlInitVtbl);             // init virtual table function
    }

    return obj_type;
}

/*****************************************************************************/
// 	Description : Set the back ground color.
//	Global resource dependence :
//  Author:
//	Note:设置控件的背景,FALSE,设置错误,TRUE设置成功
/*****************************************************************************/
LOCAL BOOLEAN ComboxSetBackGround (
    CTRLBASE_OBJ_T *base_ctrl_ptr,
    GUI_BG_T *bg_ptr
)
{
    BOOLEAN result = FALSE;
    CTRLCOMBOX_OBJ_T *   combox_ctrl_ptr = (CTRLCOMBOX_OBJ_T*) base_ctrl_ptr;

    if (PNULL != bg_ptr && PNULL != combox_ctrl_ptr)
    {
        base_ctrl_ptr->bg = *bg_ptr;
        result = TRUE;
    }

    return result;
}

/*****************************************************************************/
// 	Description : Set the back ground color.
//	Global resource dependence :
//  Author:
//	Note:设置控件的背景,FALSE,设置错误,TRUE设置成功
/*****************************************************************************/
LOCAL BOOLEAN ComboxSetBorder (
    CTRLBASE_OBJ_T *base_ctrl_ptr,
    GUI_BORDER_T  *border_ptr
)
{
    BOOLEAN result = FALSE;
    CTRLCOMBOX_OBJ_T *   combox_ctrl_ptr = (CTRLCOMBOX_OBJ_T*) base_ctrl_ptr;

    if (PNULL != border_ptr && PNULL != combox_ctrl_ptr)
    {
        base_ctrl_ptr->border = *border_ptr;

        CTRLBASE_SetBorder (combox_ctrl_ptr->list_handle, border_ptr);

        result = TRUE;
    }

    return result;
}
/*****************************************************************************/
//  Description : init combox class
//  Global resource dependence :
//  Author: xiyuan.ma & nan.ji
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ComboxCtrlInitVtbl (
    CTRLCOMBOX_VTBL_T        *combox_vtbl_ptr
)
{
    CTRLBASE_VTBL_T *  base_ctrl_vtbl_ptr = (CTRLBASE_VTBL_T*) combox_vtbl_ptr;

    if (PNULL == base_ctrl_vtbl_ptr)
    {
        return FALSE;
    }

    base_ctrl_vtbl_ptr->HandleEvent = ComboxCtrlHandleMsg;

    // 以下接口用于实现内嵌form
    base_ctrl_vtbl_ptr->GetHeightByWidth = ComboxCtrlGetHeightByWidth;
    base_ctrl_vtbl_ptr->SetDisplayRect = ComboxCtrlSetDisplayRect;
    base_ctrl_vtbl_ptr->SetRect = ComboxCtrlSetRect;
    base_ctrl_vtbl_ptr->SetBothRect = ComboxCtrlSetBothRect;
    base_ctrl_vtbl_ptr->SetBorder = ComboxSetBorder;

    return TRUE;
}
/*****************************************************************************/
// 	Description : set ctrl rect
//	Global resource dependence :
//  Author: xiyuan.ma & nan.ji
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN ComboxCtrlSetBothRect (
    CTRLBASE_OBJ_T      *base_ctrl_ptr, //in:
    const GUI_BOTH_RECT_T    *both_rect_ptr
)
{
    BOOLEAN              result = FALSE;
    CTRLCOMBOX_OBJ_T *   combox_ctrl_ptr = (CTRLCOMBOX_OBJ_T*) base_ctrl_ptr;
    CTRLBASE_VTBL_T *    parent_vtbl_ptr = (CTRLBASE_VTBL_T*) TYPEMNG_GetParentVtbl (CTRL_COMBOX_TYPE, (OBJECT_TYPE_PTR) base_ctrl_ptr);

    if (PNULL != combox_ctrl_ptr
            && PNULL != parent_vtbl_ptr)
    {
        combox_ctrl_ptr->is_dirty = TRUE;
        parent_vtbl_ptr->SetBothRect (base_ctrl_ptr, both_rect_ptr);
    }

    return (result);
}
/*****************************************************************************/
//  Description : convert init param
//  Global resource dependence :
//  Author: xiyuan.ma & nan.ji
//  Note:
// 该方法用于将控件的对外初始化参数结构转换为内部的初始化参数结构
// 由于控件有继承关系，控件初始化时又带有参数，所以内部的初始化参数也必须有继承关系
/*****************************************************************************/
LOCAL void ComboxCtrlPackInitParam (
    CTRLCOMBOX_INIT_DATA_T     *combox_init_ptr,   //in
    CTRLCOMBOX_INIT_PARAM_T   *combox_param_ptr   //out
)
{
    CTRLBASE_INIT_PARAM_T *  base_ctrl_param_ptr = (CTRLBASE_INIT_PARAM_T*) combox_param_ptr;

    if (PNULL == combox_init_ptr || PNULL == combox_param_ptr)
    {
        return;
    }

    base_ctrl_param_ptr->both_rect = combox_init_ptr->both_rect;
    combox_param_ptr->page_item_num = combox_init_ptr->page_item_num;
    combox_param_ptr->expand_direction = combox_init_ptr->expand_direction;
}

/*****************************************************************************/
//  Description : combox construct
//  Global resource dependence :
//  Author: xiyuan.ma & nan.ji
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ComboxCtrlConstruct (
    CTRLBASE_OBJ_T           *base_ctrl_ptr,
    CTRLCOMBOX_INIT_PARAM_T   *combox_param_ptr
)
{
    BOOLEAN             result = TRUE;
    MMI_COMBOX_STYLE_T  style = {0};
    CTRLCOMBOX_OBJ_T *  combox_ctrl_ptr = (CTRLCOMBOX_OBJ_T*) base_ctrl_ptr;

    if (PNULL == base_ctrl_ptr || PNULL == combox_param_ptr)
    {
        return FALSE;
    }

    MMITHEME_GetComBoxStyle (&style);

    if (0 == combox_param_ptr->page_item_num)
    {
        combox_ctrl_ptr->page_item_num = style.default_item_num;
    }

    if (CTRLCOMBOX_SHOW_MAX <= combox_param_ptr->expand_direction)
    {
        combox_ctrl_ptr->expand_direction = CTRLCOMBOX_SHOW_AUTOSET;
    }

    base_ctrl_ptr->border = style.border;
    base_ctrl_ptr->bg     = style.bg_info;

    combox_ctrl_ptr->left_space = style.left_space;
    combox_ctrl_ptr->unpressed_arrow_icon = style.unpressed_arrow_icon;
    combox_ctrl_ptr->pressed_arrow_icon = style.pressed_arrow_icon;
    combox_ctrl_ptr->has_icon = style.has_icon;
    combox_ctrl_ptr->has_expand_button = style.has_expand_button;
    combox_ctrl_ptr->has_function_button = style.has_function_button;
    combox_ctrl_ptr->is_list_expand = style.is_list_expand;
    combox_ctrl_ptr->item_height = style.item_height;
    combox_ctrl_ptr->page_item_num = style.default_item_num;
    combox_ctrl_ptr->is_dirty = TRUE;

    combox_ctrl_ptr->is_mid_softkey_open_list = TRUE;
    combox_ctrl_ptr->is_web_key_open_list = TRUE;

    //直接创建listbox，现在的listbox会一直存在。
    result = CreateListBox (base_ctrl_ptr);

    return (result);
}

/*****************************************************************************/
//  Description : combox destruct
//  Global resource dependence :
//  Author: xiyuan.ma & nan.ji
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ComboxCtrlDestruct (
    CTRLBASE_OBJ_T    *base_ctrl_ptr
)
{
    BOOLEAN             result = TRUE;

    return (result);
}
/*****************************************************************************/
//  Description : send notify
//  Global resource dependence :
//  Author: xiyuan.ma & nan.ji
//  Note:
/*****************************************************************************/
LOCAL void NotifyResize (
    CTRLBASE_OBJ_T *obj_ptr
)
{
    MMI_HANDLE_T handle = 0;

    if (PNULL != obj_ptr)
    {
        handle = MMK_GetParentWinHandle (MMK_GetWinHandleByCtrl (obj_ptr->handle));

        if (0 == handle)
        {
            handle = MMK_GetWinHandleByCtrl (obj_ptr->handle);
        }

        MMK_DefNotifyProc (handle, obj_ptr->handle, MSG_NOTIFY_RESIZE, &obj_ptr->rect);
    }
}
/*****************************************************************************/
//  Description : 计算下拉框，下拉时的区域和原始区域的合并区域
//  Global resource dependence :
//  Author: xiyuan.ma & nan.ji
//  Note:
/*****************************************************************************/
LOCAL void GetWholeRect (
    CTRLBASE_OBJ_T *obj_ptr ,   // [in]
    GUI_RECT_T *whole_rect_ptr,                 // [out] whole rect
    GUI_RECT_T *whole_display_rect_ptr          // [out] that not include invisible rect
)
{
    GUI_RECT_T          list_rect = {0};
    CTRLCOMBOX_OBJ_T *  combox_ctrl_ptr = (CTRLCOMBOX_OBJ_T*) obj_ptr;

    if (PNULL == combox_ctrl_ptr || PNULL == whole_rect_ptr || PNULL == whole_display_rect_ptr)
    {
        return;
    }

    if (PNULL != combox_ctrl_ptr->list_handle)
    {
        // get listbox's rect
        GUIAPICTRL_GetRect (combox_ctrl_ptr->list_handle, &list_rect);

        *whole_rect_ptr = list_rect;
        *whole_display_rect_ptr = list_rect;

        whole_rect_ptr->left = combox_ctrl_ptr->box_rect.left;
        whole_rect_ptr->right = combox_ctrl_ptr->box_rect.right;

        whole_display_rect_ptr->left = obj_ptr->display_rect.left;
        whole_display_rect_ptr->right = obj_ptr->display_rect.right;

        if (list_rect.top >= combox_ctrl_ptr->box_rect.bottom) // 列表在下
        {
            whole_rect_ptr->top = combox_ctrl_ptr->box_rect.top;
            whole_rect_ptr->bottom += obj_ptr->border.width;

            whole_display_rect_ptr->top = obj_ptr->display_rect.top;
            whole_display_rect_ptr->bottom += obj_ptr->border.width;
        }
        else if (list_rect.bottom <= combox_ctrl_ptr->box_rect.top) // 列表在上
        {
            whole_rect_ptr->top -= obj_ptr->border.width;
            whole_rect_ptr->bottom = combox_ctrl_ptr->box_rect.bottom;

            whole_display_rect_ptr->top -= obj_ptr->border.width;
            whole_display_rect_ptr->bottom = obj_ptr->display_rect.bottom;
        }
    }
    else
    {
        *whole_rect_ptr = combox_ctrl_ptr->box_rect;
        *whole_display_rect_ptr = obj_ptr->display_rect;
    }
}

/*****************************************************************************/
// 	Description : Set Text To EditCtrl
//	Global resource dependence :
//  Author: xiyuan.ma & nan.ji
//	Note:
/*****************************************************************************/
LOCAL void SetTextToEditCtrl (
    CTRLBASE_OBJ_T *obj_ptr
)
{
    const CTRLLIST_ITEM_T *  item_info_ptr = PNULL;
    GUIEDIT_LIST_ITEM_T     list_item_info  = {0};
    CTRLCOMBOX_OBJ_T *      combox_ctrl_ptr = (CTRLCOMBOX_OBJ_T *) obj_ptr;

    if (PNULL != combox_ctrl_ptr)
    {
        GUIEDIT_TYPE_E  edit_type = CTRLBASEEDIT_GetType (combox_ctrl_ptr->text_handle);

        //get list item info from list
         item_info_ptr = CTRLLIST_GetItem(combox_ctrl_ptr->list_handle, CTRLLIST_GetCurItemIndex(combox_ctrl_ptr->list_handle));

        if (PNULL == item_info_ptr
                || PNULL == item_info_ptr->data_ptr)
        {
            return;
        }

        list_item_info.display_str_ptr = item_info_ptr->data_ptr->item_content[0].item_data.text_buffer.wstr_ptr;
        list_item_info.display_str_len = item_info_ptr->data_ptr->item_content[0].item_data.text_buffer.wstr_len;

        list_item_info.user_str_ptr = item_info_ptr->data_ptr->item_content[1].item_data.text_buffer.wstr_ptr;
        list_item_info.user_str_len = item_info_ptr->data_ptr->item_content[1].item_data.text_buffer.wstr_len;

        list_item_info.is_object = TRUE;
        list_item_info.is_valid  = TRUE;

        //set list item info to edit or string
        if (0 != list_item_info.display_str_len && PNULL != list_item_info.display_str_ptr)
        {
            //set edit
            if (0 != combox_ctrl_ptr->text_handle)
            {
                if (GUIEDIT_TYPE_LIST == edit_type)
                {
                    GUIEDIT_ReplaceCurListItem (combox_ctrl_ptr->text_handle, &list_item_info, 1);
                }
                else
                {
                    GUIEDIT_SetString (combox_ctrl_ptr->text_handle, list_item_info.display_str_ptr, list_item_info.display_str_len);
                }

                IGUICTRL_HandleEvent ( (IGUICTRL_T*) MMK_GetCtrlPtr (combox_ctrl_ptr->text_handle), MSG_CTL_PAINT, PNULL);
            }
            //set string
            else
            {
                ComboxDrawText (combox_ctrl_ptr);
            }
        }
    }

    return;
}

/*****************************************************************************/
// 	Description : select one from drop down list
//	Global resource dependence :
//  Author: xiyuan.ma & nan.ji
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E ShrinkList (
    CTRLBASE_OBJ_T *obj_ptr
)
{
    MMI_RESULT_E     recode = MMI_RESULT_FALSE;
    CTRLCOMBOX_OBJ_T *combox_ctrl_ptr = (CTRLCOMBOX_OBJ_T *) obj_ptr;

    //get selected item
    if (PNULL != combox_ctrl_ptr
            && 0 != combox_ctrl_ptr->list_handle
            && combox_ctrl_ptr->is_list_expand)
    {
        SetListVisiable (combox_ctrl_ptr, FALSE);

        //resize
        SCI_MEMSET (&combox_ctrl_ptr->box_rect, 0, sizeof (combox_ctrl_ptr->box_rect));

        NotifyResize (obj_ptr);

        recode = MMI_RESULT_TRUE;
    }

    return recode;
}

/*****************************************************************************/
// 	Description : select one from drop down list
//	Global resource dependence :
//  Author: xiyuan.ma & nan.ji
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E SelectFromList (
    CTRLBASE_OBJ_T *obj_ptr
)
{
    MMI_RESULT_E     recode = MMI_RESULT_FALSE;
    CTRLCOMBOX_OBJ_T *combox_ctrl_ptr = (CTRLCOMBOX_OBJ_T *) obj_ptr;

    //get selected item
    if (PNULL != combox_ctrl_ptr
            && 0 != combox_ctrl_ptr->list_handle
            && combox_ctrl_ptr->is_list_expand)
    {
        // 如果内嵌edit，则要设置字符串
        SetTextToEditCtrl (obj_ptr);

        SetListVisiable (combox_ctrl_ptr, FALSE);
        //resize
        SCI_MEMSET (&combox_ctrl_ptr->box_rect, 0, sizeof (combox_ctrl_ptr->box_rect));

        NotifyResize (obj_ptr);

        //send msg to parent window
        if (GUICTRL_PostNotify (obj_ptr->handle, MSG_NOTIFY_DROPDOWNLIST_SELECTED))
        {
            recode = MMI_RESULT_TRUE;
        }
    }

    return recode;
}
/*****************************************************************************/
// 	Description : Set List Visiable
//	Global resource dependence :
//  Author: xiyuan.ma & nan.ji
//	Note:
/*****************************************************************************/
LOCAL void SetListVisiable (
    CTRLCOMBOX_OBJ_T * combox_ctrl_ptr,
    BOOLEAN is_visiable
)
{
    CTRLBASE_OBJ_T *     ctrl_ptr  = PNULL;
    CTRLBASE_OBJ_T *     base_ctrl_ptr = PNULL;

    if (PNULL != combox_ctrl_ptr)
    {
        combox_ctrl_ptr->is_list_expand = is_visiable;

        ctrl_ptr = (CTRLBASE_OBJ_T *) MMK_GetCtrlPtr (combox_ctrl_ptr->list_handle);

        VTLBASE_SetVisible (ctrl_ptr, is_visiable);

        //当设置list为非active时，设置combox为active
        if (!is_visiable)
        {
            base_ctrl_ptr = (CTRLBASE_OBJ_T*) combox_ctrl_ptr;
            MMK_SetActiveCtrl (base_ctrl_ptr->handle, TRUE);
        }
    }
}
/*****************************************************************************/
// 	Description : expand the list
//	Global resource dependence :
//  Author: xiyuan.ma & nan.ji
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E ExpandList (
    CTRLBASE_OBJ_T *base_ctrl_ptr
)
{
    GUI_RECT_T              cross_rect   = {0};
    MMI_RESULT_E            recode       = MMI_RESULT_FALSE;
    CTRLCOMBOX_OBJ_T *      combox_ctrl_ptr = (CTRLCOMBOX_OBJ_T*) base_ctrl_ptr;

    if (PNULL != combox_ctrl_ptr
            && !combox_ctrl_ptr->is_list_expand)
    {
        SetListVisiable (combox_ctrl_ptr, TRUE);

        if ( (base_ctrl_ptr->is_active)
                && GUI_IntersectRect (&cross_rect, base_ctrl_ptr->display_rect, base_ctrl_ptr->rect))
        {
            // 通知app
            MMK_SendMsg (base_ctrl_ptr->win_handle, MSG_CTL_DROPDOWNLIST_PRE_PROCESS, PNULL);

            CTRLLIST_SetDisable3D (combox_ctrl_ptr->list_handle, TRUE);

            ComboxDrawList (combox_ctrl_ptr);

            // 重新刷屏
            NotifyResize (base_ctrl_ptr);

            GUIAPICTRL_HandleEvent (combox_ctrl_ptr->list_handle, MSG_CTL_PAINT, PNULL);

        }
    }

    return recode;
}

/*****************************************************************************/
//  Description : handle combox up msg function
//  Global resource dependence :
//  Author: xiyuan.ma & nan.ji
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ComboxCtrlHandleTpUp (
    CTRLCOMBOX_OBJ_T *combox_ctrl_ptr,   //control pointer
    DPARAM           param             //add data
)
{
    BOOLEAN           result = FALSE;
    GUI_POINT_T       cur_point = {0};
    GUI_RECT_T        text_rect = {0};
    CTRLBASE_OBJ_T *  base_ctrl_ptr = (CTRLBASE_OBJ_T*) combox_ctrl_ptr;

    if (PNULL != combox_ctrl_ptr
            && !MMK_GET_TP_SLIDE (param))
    {
        cur_point.x = MMK_GET_TP_X (param);
        cur_point.y = MMK_GET_TP_Y (param);

        text_rect = CalcTextRect (combox_ctrl_ptr);

        if (GUI_PointIsInRect (cur_point, text_rect))
        {
            if (!combox_ctrl_ptr->is_list_expand)
            {
                ExpandList (base_ctrl_ptr);
            }
            else if (combox_ctrl_ptr->is_list_expand)
            {
                SelectFromList (base_ctrl_ptr);
            }
        }

        result = TRUE;
    }

    return result;
}

/*****************************************************************************/
//  Description : handle combox msg function
//  Global resource dependence :
//  Author: xiyuan.ma & nan.ji
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E ComboxCtrlHandleMsg (
    CTRLBASE_OBJ_T        *base_ctrl_ptr,   //control pointer
    MMI_MESSAGE_ID_E      msg_id,           //message
    DPARAM                param             //add data
)
{
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    CTRLCOMBOX_OBJ_T *  combox_ctrl_ptr = (CTRLCOMBOX_OBJ_T*) base_ctrl_ptr;
    CTRLBASE_VTBL_T *   parent_vtbl_ptr = (CTRLBASE_VTBL_T*) TYPEMNG_GetParentVtbl (CTRL_COMBOX_TYPE, (OBJECT_TYPE_PTR) base_ctrl_ptr);

    if (PNULL == combox_ctrl_ptr)
    {
        return MMI_RESULT_FALSE;
    }

    switch (msg_id)
    {
    case MSG_CTL_OPEN:
        break;

    case MSG_CTL_PAINT:
        ComboxDraw (combox_ctrl_ptr);
        break;

    case MSG_CTL_GET_ACTIVE:
    case MSG_NOTIFY_GET_ACTIVE:
        base_ctrl_ptr->is_active = TRUE;
        GUICTRL_SendNotify (base_ctrl_ptr->handle, MSG_NOTIFY_GET_ACTIVE);

        if (0 != combox_ctrl_ptr->text_handle)
        {
            MMK_SetAtvCtrl (base_ctrl_ptr->win_handle, combox_ctrl_ptr->text_handle);

            if (param)
            {
                MMI_NOTIFY_T* notify_ptr = (MMI_NOTIFY_T*) param;

                if (notify_ptr->src_handle == combox_ctrl_ptr->text_handle)
                {
                    GUIAPICTRL_HandleEvent (combox_ctrl_ptr->text_handle, MSG_CTL_PAINT, PNULL);
                }
            }
        }

        break;

    case MSG_CTL_LOSE_ACTIVE:
        base_ctrl_ptr->is_active = FALSE;
        ShrinkList (base_ctrl_ptr);
        GUICTRL_SendNotify (base_ctrl_ptr->handle, MSG_NOTIFY_LOSE_ACTIVE);
        break;

    case MSG_CTL_LOSE_FOCUS:
        combox_ctrl_ptr->is_list_expand = FALSE;
        break;

    case MSG_NOTIFY_LOSE_ACTIVE:
        // hide listbox when listbox lost focus.
        //ShrinkList(base_ctrl_ptr);
        //CTRLBASE_SetCanActive(combox_ctrl_ptr->text_handle, FALSE);
        // 查看dropdownlist是否是当前的焦点控件
        base_ctrl_ptr->is_active = MMK_IsActiveCtrl (base_ctrl_ptr->handle);

        if (!base_ctrl_ptr->is_active)
        {
            GUICTRL_SendNotify (base_ctrl_ptr->handle, MSG_NOTIFY_LOSE_ACTIVE);
        }

        break;

    case MSG_TIMER:
        break;

    case MSG_NOTIFY_MIDSK:

        if (!combox_ctrl_ptr->is_list_expand && combox_ctrl_ptr->is_mid_softkey_open_list)
        {
            ExpandList (base_ctrl_ptr);
        }
        else if (combox_ctrl_ptr->is_list_expand)
        {
            SelectFromList (base_ctrl_ptr);
        }
        else
        {
            result = MMI_RESULT_FALSE;
        }

        break;


    case MSG_APP_OK:

        if (combox_ctrl_ptr->is_list_expand)
        {
            SelectFromList (base_ctrl_ptr);
        }
        else
        {
            result = MMI_RESULT_FALSE;
        }

        break;


    case MSG_NOTIFY_OK:
#ifdef TOUCH_PANEL_SUPPORT
    case MSG_NOTIFY_PENOK:
#endif

        if (combox_ctrl_ptr->is_list_expand)
        {
            SelectFromList (base_ctrl_ptr);
        }
        else
        {
            result = MMI_RESULT_FALSE;
        }

        break;


    case MSG_APP_WEB:

        if (!combox_ctrl_ptr->is_list_expand && combox_ctrl_ptr->is_web_key_open_list)
        {
            ExpandList (base_ctrl_ptr);
        }
        else if (combox_ctrl_ptr->is_list_expand)
        {
            SelectFromList (base_ctrl_ptr);
        }
        else
        {
            result = MMI_RESULT_FALSE;
        }

        break;

    case MSG_APP_CANCEL:

        if (combox_ctrl_ptr->is_list_expand)
        {
            result = ShrinkList (base_ctrl_ptr);
        }
        else
        {
            result = MMI_RESULT_FALSE;
        }

        break;

#ifdef TOUCH_PANEL_SUPPORT
    case MSG_TP_PRESS_DOWN:
        break;

    case MSG_TP_PRESS_UP:
        ComboxCtrlHandleTpUp (combox_ctrl_ptr, param);
        break;

    case MSG_TP_PRESS_MOVE:

        if (combox_ctrl_ptr->is_list_expand)
        {
            result = ShrinkList (base_ctrl_ptr);
        }
        else
        {
            result = MMI_RESULT_FALSE;
        }

        break;

#endif

    default:
        result = parent_vtbl_ptr->HandleEvent (base_ctrl_ptr, msg_id, param);
        break;
    }

    return (result);
}

/*****************************************************************************/
//  Description : Calc icon Rect
//  Global resource dependence :
//  Author: nan ji & xiyuan ma
//  Note:
/*****************************************************************************/
LOCAL GUI_RECT_T CalcIconRect (
    CTRLCOMBOX_OBJ_T     * combox_ctrl_ptr
)
{

    uint16              icon_width = 0;
    GUI_RECT_T          icon_rect = {0};
    CTRLBASE_OBJ_T *    base_ctrl_ptr = (CTRLBASE_OBJ_T*) combox_ctrl_ptr;

    if (PNULL != combox_ctrl_ptr
            && combox_ctrl_ptr->is_dirty)
    {
        if (combox_ctrl_ptr->has_icon)
        {
            GUIRES_GetImgWidthHeight (&icon_width, PNULL, combox_ctrl_ptr->icon_id, base_ctrl_ptr->win_handle);
        }

        icon_rect.top = base_ctrl_ptr->rect.top + base_ctrl_ptr->border.width;
        icon_rect.bottom = base_ctrl_ptr->rect.bottom - base_ctrl_ptr->border.width;
        icon_rect.left = base_ctrl_ptr->rect.left + base_ctrl_ptr->border.width;
        icon_rect.right = icon_rect.left + icon_width;
    }

    return (icon_rect);
}

/*****************************************************************************/
//  Description : Calc Func Button Rect
//  Global resource dependence :
//  Author: nan ji & xiyuan ma
//  Note:
/*****************************************************************************/
LOCAL GUI_RECT_T CalcFuncButtonRect (
    CTRLCOMBOX_OBJ_T     * combox_ctrl_ptr
)
{

    uint16              func_button_width = 0;
    GUI_RECT_T          func_button_rect = {0};
    CTRLBASE_OBJ_T *    base_ctrl_ptr = (CTRLBASE_OBJ_T*) combox_ctrl_ptr;

    if (PNULL != combox_ctrl_ptr
            && combox_ctrl_ptr->is_dirty)
    {
        if (combox_ctrl_ptr->has_function_button)
        {
            GUIRES_GetImgWidthHeight (&func_button_width, PNULL, combox_ctrl_ptr->func_button_icon, base_ctrl_ptr->win_handle);
        }

        func_button_rect.top = base_ctrl_ptr->rect.top + base_ctrl_ptr->border.width;
        func_button_rect.bottom = base_ctrl_ptr->rect.bottom - base_ctrl_ptr->border.width;
        func_button_rect.right = base_ctrl_ptr->rect.right - base_ctrl_ptr->border.width;
        func_button_rect.left = func_button_rect.right - func_button_width;
    }

    return (func_button_rect);
}

/*****************************************************************************/
//  Description : Calc Expand Button Rect
//  Global resource dependence :
//  Author: nan ji & xiyuan ma
//  Note:
/*****************************************************************************/
LOCAL GUI_RECT_T CalcExpandButtonRect (
    CTRLCOMBOX_OBJ_T     * combox_ctrl_ptr
)
{

    uint16              expand_button_width = 0;
    GUI_RECT_T          expand_button_rect = {0};
    GUI_RECT_T          func_button_rect = {0};
    CTRLBASE_OBJ_T *    base_ctrl_ptr = (CTRLBASE_OBJ_T*) combox_ctrl_ptr;

    if (PNULL != combox_ctrl_ptr
            && combox_ctrl_ptr->is_dirty)
    {
        if (combox_ctrl_ptr->has_expand_button)
        {
            GUIRES_GetImgWidthHeight (&expand_button_width, PNULL, combox_ctrl_ptr->unpressed_arrow_icon, base_ctrl_ptr->win_handle);
        }

        expand_button_rect.top = base_ctrl_ptr->rect.top + base_ctrl_ptr->border.width;
        expand_button_rect.bottom = base_ctrl_ptr->rect.bottom - base_ctrl_ptr->border.width;

        func_button_rect = CalcFuncButtonRect (combox_ctrl_ptr);
        expand_button_rect.right = func_button_rect.left - 1;
        expand_button_rect.left = expand_button_rect.right - expand_button_width;
    }

    return (expand_button_rect);
}

/*****************************************************************************/
//  Description : Calc Text Rect
//  Global resource dependence :
//  Author: nan ji & xiyuan ma
//  Note:
/*****************************************************************************/
LOCAL GUI_RECT_T CalcTextRect (
    CTRLCOMBOX_OBJ_T     * combox_ctrl_ptr
)
{
    GUI_RECT_T          text_rect = {0};
    GUI_RECT_T          icon_rect = {0};
    GUI_RECT_T          expand_button_rect = {0};
    CTRLBASE_OBJ_T *    base_ctrl_ptr = (CTRLBASE_OBJ_T*) combox_ctrl_ptr;

    if (PNULL != combox_ctrl_ptr
            && combox_ctrl_ptr->is_dirty)
    {
        if (base_ctrl_ptr->rect.top < base_ctrl_ptr->rect.bottom)
        {
            text_rect.top = base_ctrl_ptr->rect.top + base_ctrl_ptr->border.width;
            text_rect.bottom = base_ctrl_ptr->rect.bottom - base_ctrl_ptr->border.width;

            icon_rect = CalcIconRect (combox_ctrl_ptr);
            text_rect.left = icon_rect.right + 1;

            expand_button_rect = CalcExpandButtonRect (combox_ctrl_ptr);
            text_rect.right = expand_button_rect.left - 1;
        }
    }

    return (text_rect);
}

/*****************************************************************************/
//  Description : Calc List Rect
//  Global resource dependence :
//  Author: xiyuan.ma & nan.ji
//  Note:
/*****************************************************************************/
LOCAL GUI_RECT_T CalcListRect (
    CTRLCOMBOX_OBJ_T     * combox_ctrl_ptr
)
{
    uint16      rect_height = 0;
    uint16      item_height = 0;
    uint16      item_num  = 0;
    uint16      total_num = 0;
    GUI_RECT_T  rect = {0};
    GUI_RECT_T  client_rect = {0};
    CTRLBASE_OBJ_T      *base_ctrl_ptr = (CTRLBASE_OBJ_T*) combox_ctrl_ptr;

    if (PNULL == combox_ctrl_ptr
            || !combox_ctrl_ptr->is_dirty
            || !combox_ctrl_ptr->is_list_expand)
    {
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, GUIDROPDOWNLIST_1725_112_2_18_3_15_30_101, (uint8*) "");
        return rect;
    }

    total_num = CTRLLIST_GetTotalItemNum (combox_ctrl_ptr->list_handle);
    combox_ctrl_ptr->cur_page_item_num = (uint16) (MIN (combox_ctrl_ptr->page_item_num, total_num));

    rect = base_ctrl_ptr->rect;//combox_ctrl_ptr->box_rect;//base_ctrl_ptr->display_rect;

    client_rect = MMITHEME_GetClientRectEx (base_ctrl_ptr->win_handle);

    rect_height = client_rect.bottom - client_rect.top + 1;
    item_height = combox_ctrl_ptr->item_height;

    if ( (CTRLCOMBOX_SHOW_AUTOSET == combox_ctrl_ptr->expand_direction && rect.top > rect_height / 2)
            || CTRLCOMBOX_SHOW_UPWARD == combox_ctrl_ptr->expand_direction)    // 向上显示
    {
        if (0 < item_height)
        {
            // max item in such region
            // rect.top 应理解为rect.top - 1 + 1
            // -1是因为listbox要从dropdownlist的上一个象素开始显示
            // +1是因为rect算到height时要加1
            item_num = (rect.top - client_rect.top) / item_height;
        }
        else
        {
            item_num = 0;
        }

        combox_ctrl_ptr->cur_page_item_num = MIN (item_num, combox_ctrl_ptr->cur_page_item_num);

        rect.bottom = (int16) (base_ctrl_ptr->display_rect.top - 1);
        rect.top = (int16) (rect.bottom - item_height * combox_ctrl_ptr->cur_page_item_num - base_ctrl_ptr->border.width * 2 + 1);
    }
    else if (CTRLCOMBOX_SHOW_DOWNWARD == combox_ctrl_ptr->expand_direction
             || CTRLCOMBOX_SHOW_AUTOSET == combox_ctrl_ptr->expand_direction)   // 向下显示
    {
        if (0 < item_height)
        {
            // max item in such region
            // 之所以减2，是因为(rect.bottom + 1 + 1)，
            // 前一个+1是因为listbox要从dropdownlist下一个象素显示，
            // 后一个+1是因为rect算到高度时要加1
            item_num = (client_rect.bottom - rect.bottom - 2) / item_height;
        }
        else
        {
            item_num = 0;
        }

        combox_ctrl_ptr->cur_page_item_num = MIN (item_num, combox_ctrl_ptr->cur_page_item_num);

        rect.top = (int16) (base_ctrl_ptr->display_rect.bottom + 1);
        rect.bottom = (int16) (rect.top + item_height * combox_ctrl_ptr->cur_page_item_num + base_ctrl_ptr->border.width * 2 - 1);
    }
    else
    {
        //SCI_TRACE_LOW:"CalListRect show type = %d error!"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, GUIDROPDOWNLIST_1782_112_2_18_3_15_30_102, (uint8*) "d", combox_ctrl_ptr->expand_direction);
    }

    return (rect);
}

/*****************************************************************************/
//  Description : Create Expand Button
//  Global resource dependence :
//  Author: nan ji & xiyuan ma
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E ExpandButtonCallBack (MMI_HANDLE_T ctrl_handle)
{
    MMI_RESULT_E       result = MMI_RESULT_FALSE;
    CTRLBASE_OBJ_T *   ctrl_ptr = PNULL;
    CTRLCOMBOX_OBJ_T * combox_ctrl_ptr = PNULL;

    combox_ctrl_ptr = (CTRLCOMBOX_OBJ_T *) CTRLBASE_GetUserData (ctrl_handle);
    ctrl_ptr = (CTRLBASE_OBJ_T *) combox_ctrl_ptr;

    if (PNULL != combox_ctrl_ptr)
    {
        //当list展开时会shrink list
        if (!combox_ctrl_ptr->is_list_expand)
        {
            MMK_SetActiveCtrl (combox_ctrl_ptr->list_handle, TRUE);
            ExpandList (ctrl_ptr);
        }
        else
        {
            MMK_SetActiveCtrl (combox_ctrl_ptr->list_handle, FALSE);
            ShrinkList (ctrl_ptr);
        }

        result = MMI_RESULT_TRUE;
    }

    return result;
}

/*****************************************************************************/
//  Description : Create Expand Button
//  Global resource dependence :
//  Author: nan ji & xiyuan ma
//  Note:
/*****************************************************************************/
LOCAL void CreateExpandButton (
    CTRLBASE_OBJ_T *base_ctrl_ptr
)
{
    uint16              icon_width  = 0;
    uint16              icon_height = 0;
    GUI_RECT_T          button_rect = {0};
    CTRLBASE_OBJ_T *    ctrl_ptr = PNULL;
    CTRLCOMBOX_OBJ_T *  combox_ctrl_ptr = (CTRLCOMBOX_OBJ_T *) base_ctrl_ptr;

    if (PNULL != combox_ctrl_ptr
            && 0 == combox_ctrl_ptr->expand_button_handle)
    {
        GUIBUTTON_INIT_DATA_T   init_data = {0};
        MMI_CONTROL_CREATE_T    ctrl_create = {0};
        GUI_BG_T                bg = {0};

        GUIRES_GetImgWidthHeight (&icon_width, &icon_height, combox_ctrl_ptr->unpressed_arrow_icon, base_ctrl_ptr->win_handle);
        button_rect = CalcExpandButtonRect (combox_ctrl_ptr);

        //init
        ctrl_create.ctrl_id            = 0;
        ctrl_create.guid               = SPRD_GUI_BUTTON_ID;
        ctrl_create.init_data_ptr      = &init_data;
        ctrl_create.parent_ctrl_handle = base_ctrl_ptr->handle;
        ctrl_create.parent_win_handle  = base_ctrl_ptr->win_handle;

        //creat control
        ctrl_ptr = (CTRLBASE_OBJ_T*) MMK_CreateControl (&ctrl_create);
        combox_ctrl_ptr->expand_button_handle = ctrl_ptr->handle;

        CTRLBASE_SetUserData (combox_ctrl_ptr->expand_button_handle, (uint32) combox_ctrl_ptr);

        //set release bg && fg
        bg.bg_type = GUI_BG_IMG;
        bg.img_id = combox_ctrl_ptr->unpressed_arrow_icon;
        CTRLBUTTON_SetFg (combox_ctrl_ptr->expand_button_handle, &bg);

        //set press bg && fg
        bg.img_id = combox_ctrl_ptr->pressed_arrow_icon;
        CTRLBUTTON_SetPressedFg (combox_ctrl_ptr->expand_button_handle, &bg);

        CTRLBUTTON_SetRunSheen (combox_ctrl_ptr->expand_button_handle, FALSE);

        CTRLBUTTON_SetCallBackFuncExt (combox_ctrl_ptr->expand_button_handle, ExpandButtonCallBack);
        CTRLBASE_SetRect (combox_ctrl_ptr->expand_button_handle, &button_rect);
    }

    return;
}

/*****************************************************************************/
//  Description : Create Func Button
//  Global resource dependence :
//  Author: nan ji & xiyuan ma
//  Note:
/*****************************************************************************/
LOCAL void CreateFuncButton (
    CTRLBASE_OBJ_T *base_ctrl_ptr
)
{
    uint16                 icon_width = 0;
    uint16                 icon_height = 0;
    GUI_RECT_T             button_rect = {0};
    CTRLBASE_OBJ_T *       ctrl_ptr = PNULL;
    GUIBUTTON_INIT_DATA_T  init_data = {0};
    MMI_CONTROL_CREATE_T   ctrl_create = {0};
    CTRLCOMBOX_OBJ_T *     combox_ctrl_ptr = (CTRLCOMBOX_OBJ_T *) base_ctrl_ptr;

    if (PNULL != combox_ctrl_ptr
            && 0 == combox_ctrl_ptr->function_button_handle)
    {
        GUIRES_GetImgWidthHeight (&icon_width, &icon_height, combox_ctrl_ptr->func_button_icon, base_ctrl_ptr->win_handle);

        button_rect = CalcFuncButtonRect (combox_ctrl_ptr);

        //init
        init_data.bg.bg_type = GUI_BG_IMG;
        init_data.bg.img_id  = combox_ctrl_ptr->func_button_icon;
        ctrl_create.ctrl_id            = 0;
        ctrl_create.guid               = SPRD_GUI_BUTTON_ID;
        ctrl_create.init_data_ptr      = &init_data;
        ctrl_create.parent_ctrl_handle = base_ctrl_ptr->handle;
        ctrl_create.parent_win_handle  = base_ctrl_ptr->win_handle;

        //creat control
        ctrl_ptr = (CTRLBASE_OBJ_T*) MMK_CreateControl (&ctrl_create);
        combox_ctrl_ptr->function_button_handle = ctrl_ptr->handle;
        CTRLBASE_SetRect (combox_ctrl_ptr->function_button_handle, &button_rect);
    }

    return;
}
/*****************************************************************************/
//  Description : Create Text Ctrl
//  Global resource dependence :
//  Author: nan ji & xiyuan ma
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CreateTextCtrl (
    CTRLBASE_OBJ_T *base_ctrl_ptr
)
{
    BOOLEAN                 result = FALSE;
    CTRLBASE_OBJ_T *        ctrl_ptr = PNULL;
    GUIEDIT_INIT_DATA_T     init_data = {0};
    MMI_CONTROL_CREATE_T    ctrl_create = {0};
    CTRLCOMBOX_OBJ_T *      combox_ctrl_ptr = (CTRLCOMBOX_OBJ_T *) base_ctrl_ptr;

    if (PNULL != combox_ctrl_ptr
            && 0 == combox_ctrl_ptr->text_handle)
    {
        //set init data
        init_data.both_rect.h_rect = init_data.both_rect.v_rect = CalcTextRect (combox_ctrl_ptr);

        if (GUIEDIT_TYPE_NULL == combox_ctrl_ptr->edit_info.edit_type)
        {
            combox_ctrl_ptr->edit_info.edit_type = GUIEDIT_TYPE_TEXT;
        }

        init_data.type = combox_ctrl_ptr->edit_info.edit_type;
        init_data.str_max_len = combox_ctrl_ptr->edit_info.editable_max_text_num;
        init_data.add_data.list.item_max_len = combox_ctrl_ptr->edit_info.editable_max_text_num;
        init_data.add_data.list.item_num = combox_ctrl_ptr->edit_info.editable_max_listitem_num;

        //set control param
        ctrl_create.ctrl_id            = 0;
        ctrl_create.parent_ctrl_handle = base_ctrl_ptr->handle;
        ctrl_create.guid               = SPRD_GUI_EDITBOX_ID;
        ctrl_create.init_data_ptr      = &init_data;
        ctrl_create.parent_win_handle  = base_ctrl_ptr->win_handle;

        //creat edit control
        ctrl_ptr = (CTRLBASE_OBJ_T*) MMK_CreateControl (&ctrl_create);
        combox_ctrl_ptr->text_handle = ctrl_ptr->handle;

        if (GUIEDIT_TYPE_LIST == combox_ctrl_ptr->edit_info.edit_type)
        {
            GUIEDIT_SetIm (
                combox_ctrl_ptr->text_handle,
                GUIIM_TYPE_SET_ALL,
                GUIIM_TYPE_ABC);
        }

        //set state
        MMK_SetAtvCtrl (base_ctrl_ptr->win_handle, combox_ctrl_ptr->text_handle);
        IGUICTRL_SetState (MMK_GetCtrlPtr (base_ctrl_ptr->handle), GUICTRL_STATE_DISABLE_ACTIVE, TRUE);
        IGUICTRL_SetCircularHandleUpDown (MMK_GetCtrlPtr (combox_ctrl_ptr->text_handle), FALSE);

        CTRLLIST_SetListState (combox_ctrl_ptr->list_handle, GUILIST_STATE_DROPDOWNLIST, FALSE);
        result = TRUE;

        SCI_TRACE_LOW ("CreateTextCtrl result = %d, ctrl_handle = %d", result, base_ctrl_ptr->handle);
    }
    else
    {
        SCI_TRACE_LOW ("CreateTextCtrl IN PARAM PNULL!");
    }

    return result;
}
/*****************************************************************************/
//  Description : create dynamic List
//  Global resource dependence :
//  Author: xiyuan.ma & nan.ji
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CreateListBox (
    CTRLBASE_OBJ_T *base_ctrl_ptr
)
{
    BOOLEAN               result = FALSE;
    CTRLBASE_OBJ_T *      ctrl_ptr  = PNULL;
    GUILIST_INIT_DATA_T   init_data = {0};
    MMI_CONTROL_CREATE_T  create = {0};
    uint16                total_num = 0;
    MMI_COMBOX_STYLE_T    style = {0};
    CTRLCOMBOX_OBJ_T *    combox_ctrl_ptr = (CTRLCOMBOX_OBJ_T *) base_ctrl_ptr;

    if (PNULL != combox_ctrl_ptr
            && 0 == combox_ctrl_ptr->list_handle)
    {
        GUI_RECT_T rect = CalcListRect (combox_ctrl_ptr);

        init_data.both_rect.h_rect = init_data.both_rect.v_rect = rect;
        init_data.type = GUILIST_TEXTLIST_E;
        init_data.max_item_num = (uint16) (total_num + 1);

        create.ctrl_id = 0;
        create.guid    = SPRD_GUI_LIST_ID;
        create.parent_win_handle = base_ctrl_ptr->win_handle;
        create.parent_ctrl_handle = base_ctrl_ptr->handle;
        create.init_data_ptr = &init_data;

        //create list ctrl
        ctrl_ptr = (CTRLBASE_OBJ_T*) MMK_CreateControl (&create);
        combox_ctrl_ptr->list_handle = ctrl_ptr->handle;

        //set list's topmost ,so list may get tp message firstly
        SetListVisiable (combox_ctrl_ptr, FALSE);

        //set list's state
        GUIAPICTRL_SetState (combox_ctrl_ptr->list_handle, GUICTRL_STATE_TOPMOST_1, TRUE);
        CTRLLIST_SetListState (combox_ctrl_ptr->list_handle, GUILIST_STATE_NEED_TITLE | GUILIST_STATE_NEED_SOFTKEY, FALSE);
        CTRLLIST_SetListState (combox_ctrl_ptr->list_handle, GUILIST_STATE_DROPDOWNLIST, TRUE);

        //set list's border && bg
        MMITHEME_GetComBoxStyle (&style);
        GUIAPICTRL_SetBorder (combox_ctrl_ptr->list_handle, &style.border);
        GUIAPICTRL_SetBg (combox_ctrl_ptr->list_handle, &style.bg_info);

        //set list's 非active
        MMK_SetActiveCtrl (combox_ctrl_ptr->list_handle, FALSE);

        result = TRUE;
    }

    return result;
}
/*****************************************************************************/
//  Description : display bg
//  Global resource dependence :
//  Author: xiyuan.ma & nan.ji
//  Note:
/*****************************************************************************/
LOCAL void ComboxDrawBg (CTRLCOMBOX_OBJ_T     * combox_ctrl_ptr)
{
    GUI_RECT_T          image_rect = {0};
    GUI_RECT_T          cross_rect = {0};
    GUI_BG_DISPLAY_T    bg_display = {0};
    CTRLBASE_OBJ_T *    base_ctrl_ptr = (CTRLBASE_OBJ_T*) combox_ctrl_ptr;

    if (PNULL != base_ctrl_ptr
            && ComboxTypeOf ( (CTRLBASE_OBJ_T*) base_ctrl_ptr)
            && GUI_IntersectRect (&cross_rect, base_ctrl_ptr->display_rect, base_ctrl_ptr->rect)
            && MMITHEME_CheckImageID (base_ctrl_ptr->bg.img_id))
    {
        //calc bg image
        image_rect.bottom = image_rect.top + (cross_rect.bottom - cross_rect.top);
        image_rect.right  = image_rect.left + (cross_rect.right - cross_rect.left);

        //set bg display info
        bg_display.is_transparent = FALSE;
        bg_display.rect           = base_ctrl_ptr->rect;
        bg_display.display_rect   = cross_rect;
        bg_display.img_rect       = image_rect;
        bg_display.ctrl_handle    = base_ctrl_ptr->handle;
        bg_display.win_handle     = base_ctrl_ptr->win_handle;

        //display bg image
        GUI_DisplayBg (&base_ctrl_ptr->bg,
                       &bg_display,
                       & (base_ctrl_ptr->lcd_dev_info));
    }

    return;
}

/*****************************************************************************/
//  Description : display bg
//  Global resource dependence :
//  Author: xiyuan.ma & nan.ji
//  Note:
/*****************************************************************************/
LOCAL void ComboxDrawBorder (CTRLCOMBOX_OBJ_T *  combox_ctrl_ptr)
{
    CTRLBASE_OBJ_T *    base_ctrl_ptr = (CTRLBASE_OBJ_T*) combox_ctrl_ptr;

    if (PNULL != base_ctrl_ptr
            && GUI_BORDER_NONE != base_ctrl_ptr->border.type)
    {
        GUI_DisplayBorder (base_ctrl_ptr->rect,
                           base_ctrl_ptr->display_rect,
                           &base_ctrl_ptr->border,
                           &base_ctrl_ptr->lcd_dev_info);
    }

    return;
}
/*****************************************************************************/
//  Description : display bg
//  Global resource dependence :
//  Author: xiyuan.ma & nan.ji
//  Note:
/*****************************************************************************/
LOCAL void ComboxDrawIcon (CTRLCOMBOX_OBJ_T *  combox_ctrl_ptr)
{
    GUI_RECT_T          image_rect = {0};
    GUI_RECT_T          cross_rect = {0};
    GUI_RECT_T          rect = {0};
    CTRLBASE_OBJ_T *    base_ctrl_ptr = (CTRLBASE_OBJ_T*) combox_ctrl_ptr;

    if (PNULL != combox_ctrl_ptr
            && combox_ctrl_ptr->has_icon
            && GUI_IntersectRect (&cross_rect, base_ctrl_ptr->display_rect, CalcIconRect (combox_ctrl_ptr)))
    {
        //set image rect
        image_rect.left   = (int16) (cross_rect.left - rect.left);
        image_rect.top    = (int16) (cross_rect.top - rect.top);
        image_rect.right  = (int16) (image_rect.left + cross_rect.right - cross_rect.left);
        image_rect.bottom = (int16) (image_rect.top + cross_rect.bottom - cross_rect.top);

        //display image
        GUIRES_DisplayImg (PNULL, &cross_rect, PNULL, base_ctrl_ptr->win_handle, combox_ctrl_ptr->icon_id, &base_ctrl_ptr->lcd_dev_info);
    }

    return;
}
/*****************************************************************************/
//  Description : Draw ExpandButton
//  Global resource dependence :
//  Author: xiyuan.ma & nan.ji
//  Note:
/*****************************************************************************/
LOCAL void ComboxDrawExpandButton (CTRLCOMBOX_OBJ_T     * combox_ctrl_ptr)
{
    GUI_RECT_T          rect = {0};
    CTRLBASE_OBJ_T *    expand_button_ptr = PNULL;

    if (PNULL != combox_ctrl_ptr
            && combox_ctrl_ptr->has_expand_button
            && 0 != combox_ctrl_ptr->expand_button_handle)
    {
        //calc rect
        rect = CalcExpandButtonRect (combox_ctrl_ptr);

        //set rect
        expand_button_ptr = (CTRLBASE_OBJ_T*) MMK_GetCtrlPtr (combox_ctrl_ptr->expand_button_handle);
        VTLBASE_SetRect (expand_button_ptr, &rect);
    }

    return;
}

/*****************************************************************************/
//  Description : Draw Function Button
//  Global resource dependence :
//  Author: xiyuan.ma & nan.ji
//  Note:
/*****************************************************************************/
LOCAL void ComboxDrawFunctionButton (CTRLCOMBOX_OBJ_T *  combox_ctrl_ptr)
{
    GUI_RECT_T          rect = {0};
    CTRLBASE_OBJ_T *    function_button_ptr = PNULL;

    if (PNULL != combox_ctrl_ptr
            && combox_ctrl_ptr->has_function_button
            && 0 != combox_ctrl_ptr->function_button_handle)
    {
        //calc rect
        rect = CalcFuncButtonRect (combox_ctrl_ptr);

        //set rect
        function_button_ptr = (CTRLBASE_OBJ_T*) MMK_GetCtrlPtr (combox_ctrl_ptr->function_button_handle);
        VTLBASE_SetRect (function_button_ptr, &rect);

        //dispaly button
        CTRLBASE_Update (combox_ctrl_ptr->function_button_handle);
    }

    return;
}
/*****************************************************************************/
//  Description : Draw Function Button
//  Global resource dependence :
//  Author: xiyuan.ma & nan.ji
//  Note:
/*****************************************************************************/
LOCAL void ComboxDrawText (CTRLCOMBOX_OBJ_T     * combox_ctrl_ptr)
{
    GUI_RECT_T          rect = {0};

    if (PNULL != combox_ctrl_ptr)
    {
        //get edit or string rect
        rect = CalcTextRect (combox_ctrl_ptr);

        // draw edit
        if (0 != combox_ctrl_ptr->text_handle)
        {
            CTRLBASE_OBJ_T *    text_ctrl_ptr = PNULL;

            text_ctrl_ptr = (CTRLBASE_OBJ_T*) MMK_GetCtrlPtr (combox_ctrl_ptr->text_handle);

            VTLBASE_SetRect (text_ctrl_ptr, &rect);

            CTRLBASE_Update (combox_ctrl_ptr->text_handle);
        }
        // draw string
        else
        {
            uint16   index = 0xffff;
            uint16   total_num = 0;
            CTRLBASE_OBJ_T *        base_ctrl_ptr = (CTRLBASE_OBJ_T*) combox_ctrl_ptr;
            const CTRLLIST_ITEM_T *  item_ptr   = PNULL;

            index     = CTRLCOMBOX_GetSelectIndex (base_ctrl_ptr->handle);
            item_ptr  = CTRLCOMBOX_GetItem (base_ctrl_ptr->handle, index);
            total_num = CTRLLIST_GetTotalItemNum (combox_ctrl_ptr->list_handle);

            if (0xffff != index
                    && 0 != total_num
                    && PNULL != item_ptr
                    && PNULL != item_ptr->data_ptr)
            {
                uint16          text_rect_width = 0;
                GUI_COLOR_T     font_color = 0;
                MMI_STRING_T    text_info = {0};
                GUISTR_STYLE_T  str_style = {0};
                GUISTR_STATE_T  str_state = GUISTR_STATE_ALIGN | GUISTR_STATE_SINGLE_LINE | GUISTR_STATE_NOT_HALF_CHAR | GUISTR_STATE_ALIGN_BY_DIR;
                MMI_COMBOX_STYLE_T  style = {0};

                // get color
                MMITHEME_GetComBoxStyle (&style);
                font_color = style.font_color;

                // set text param
                str_style.align = ALIGN_LVMIDDLE;
                str_style.font = style.font;
                str_style.font_color = font_color;
                str_style.char_space = style.font_space;

                if (style.is_ellipsis)
                {
                    str_state |= GUISTR_STATE_ELLIPSIS;
                }

                // set string
                text_info.wstr_ptr = item_ptr->data_ptr->item_content[0].item_data.text_buffer.wstr_ptr;
                text_info.wstr_len = item_ptr->data_ptr->item_content[0].item_data.text_buffer.wstr_len;

                // set text width
                text_rect_width = rect.right - rect.left + 1;

                GUI_ReplaceChar (&text_info, LF_CHAR, BL_CHAR);
                GUI_ReplaceChar (&text_info, CR_CHAR, BL_CHAR);

                // display string
                GUISTR_DrawTextToLCDInRect (&base_ctrl_ptr->lcd_dev_info,
                                            (const GUI_RECT_T*) &rect,
                                            (const GUI_RECT_T*) &base_ctrl_ptr->display_rect,
                                            (const MMI_STRING_T*) &text_info,
                                            &str_style,
                                            str_state,
                                            GUISTR_TEXT_DIR_AUTO);

            }
        }
    }

    return;
}
/*****************************************************************************/
//  Description : Draw Function Button
//  Global resource dependence :
//  Author: xiyuan.ma & nan.ji
//  Note:
/*****************************************************************************/
LOCAL void ComboxDrawList (CTRLCOMBOX_OBJ_T *combox_ctrl_ptr)
{
    GUI_RECT_T          rect = {0};
    CTRLBASE_OBJ_T *    list_ctrl_ptr = PNULL;

    if (PNULL != combox_ctrl_ptr
            && 0 != combox_ctrl_ptr->list_handle
            && combox_ctrl_ptr->is_list_expand)
    {
        //calc rect
        list_ctrl_ptr = (CTRLBASE_OBJ_T*) MMK_GetCtrlPtr (combox_ctrl_ptr->list_handle);
        rect = CalcListRect (combox_ctrl_ptr);

        //set rect
        VTLBASE_SetRect (list_ctrl_ptr, &rect);

        //display list
        CTRLBASE_Update (combox_ctrl_ptr->list_handle);
    }

    return;
}
/*****************************************************************************/
//  Description : display combox control
//  Global resource dependence :
//  Author: xiyuan.ma & nan.ji
//  Note:
/*****************************************************************************/
LOCAL void ComboxDraw (
    CTRLCOMBOX_OBJ_T     *combox_ctrl_ptr
)
{
    ComboxDrawBg (combox_ctrl_ptr);

    ComboxDrawBorder (combox_ctrl_ptr);

    ComboxDrawIcon (combox_ctrl_ptr);

    ComboxDrawExpandButton (combox_ctrl_ptr);

    ComboxDrawFunctionButton (combox_ctrl_ptr);

    ComboxDrawText (combox_ctrl_ptr);

    ComboxDrawList (combox_ctrl_ptr);

    return;
}

/*****************************************************************************/
//  Description : get combox pointer by control id
//  Global resource dependence :
//  Author: xiyuan.ma & nan.ji
//  Note:
/*****************************************************************************/
LOCAL CTRLCOMBOX_OBJ_T* GetComboxPtr (
    MMI_HANDLE_T     ctrl_handle
)
{
    CTRLBASE_OBJ_T  *ctrl_ptr = (CTRLBASE_OBJ_T*) MMK_GetCtrlPtr (ctrl_handle);

    if (PNULL != ctrl_ptr)
    {
        SCI_ASSERT (ComboxTypeOf (ctrl_ptr)); /*assert verified*/
    }

    return (CTRLCOMBOX_OBJ_T*) ctrl_ptr;
}

/*****************************************************************************/
//  Description : ext type of
//  Global resource dependence :
//  Author:xiyuan.ma & nan.ji
//  Note: judge control type of
/*****************************************************************************/
LOCAL BOOLEAN ComboxTypeOf (
    CTRLBASE_OBJ_T     *ctrl_ptr
)
{
    return TYPEMNG_IsTypeOf ( (OBJECT_TYPE_PTR) ctrl_ptr, CTRL_COMBOX_TYPE);
}

/*****************************************************************************/
// 	Description : get control height by width,include content,border etc.
//	Global resource dependence :
//  Author: xiyuan.ma & nan.ji
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN ComboxCtrlGetHeightByWidth (
    CTRLBASE_OBJ_T      *base_ctrl_ptr,  //in:
    uint16              width,          //in:
    uint16              *height_ptr     //in/out:
)
{
    BOOLEAN             result = TRUE;
    uint16              font_height = 0;
    uint16              line_height = 0;
    uint16              image_height = 0;
    uint16              edit_width = 0;
    MMI_COMBOX_STYLE_T  style = {0};
    CTRLCOMBOX_OBJ_T *  combox_ctrl_ptr = (CTRLCOMBOX_OBJ_T*) base_ctrl_ptr;

    if (PNULL == base_ctrl_ptr
            || PNULL == height_ptr)
    {
        return FALSE;
    }

    MMITHEME_GetComBoxStyle (&style);
    //get font height
    font_height = GUI_GetFontHeight (style.font, UNICODE_HANZI);

    if (0 != combox_ctrl_ptr->text_handle)
    {
        edit_width = width;

        if (GUI_BORDER_NONE != base_ctrl_ptr->border.type)
        {
            edit_width -= base_ctrl_ptr->border.width * 2;
        }

        IGUICTRL_GetHeightByWidth (MMK_GetCtrlPtr (combox_ctrl_ptr->text_handle), edit_width, &font_height);
    }

    //add border
    if (GUI_BORDER_NONE == base_ctrl_ptr->border.type)
    {
        line_height = (uint16) (font_height + (base_ctrl_ptr->border.width * 2) + 4);
    }

    GUIRES_GetImgWidthHeight (PNULL, &image_height, combox_ctrl_ptr->unpressed_arrow_icon, base_ctrl_ptr->win_handle);

    *height_ptr = MAX (line_height, image_height + (base_ctrl_ptr->border.width * 2));

    GUI_INVALID_PARAM (width);/*lint !e522*/

    return (result);
}
/*****************************************************************************/
// 	Description : set ctrl display rect
//	Global resource dependence :
//  Author: xiyuan.ma & nan.ji
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN ComboxCtrlSetRect (
    CTRLBASE_OBJ_T      *base_ctrl_ptr, //in:
    const GUI_RECT_T    *rect_ptr       //in:
)
{
    BOOLEAN             result = FALSE;
    GUI_RECT_T          rect = {0};
    CTRLCOMBOX_OBJ_T *  combox_ctrl_ptr = (CTRLCOMBOX_OBJ_T*) base_ctrl_ptr;

    if ( (PNULL != rect_ptr) &&
            (PNULL != combox_ctrl_ptr) &&
            (!GUI_EqualRect (base_ctrl_ptr->rect, *rect_ptr)))
    {
        // 记录之前的区域
        rect = base_ctrl_ptr->rect;

        base_ctrl_ptr->rect = *rect_ptr;
        base_ctrl_ptr->display_rect = *rect_ptr;

        GUI_SetVOrHRect (base_ctrl_ptr->handle, (GUI_RECT_T *) rect_ptr, & (base_ctrl_ptr->both_rect));

        result = TRUE;
    }

    return result;
}
/*****************************************************************************/
// 	Description : set ctrl display rect
//	Global resource dependence :
//  Author: xiyuan.ma & nan.ji
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN ComboxCtrlSetDisplayRect (
    CTRLBASE_OBJ_T      *base_ctrl_ptr, //in:
    const GUI_RECT_T    *rect_ptr,      //in:
    BOOLEAN             is_update       //in:
)
{
    BOOLEAN             result = FALSE;
    GUI_RECT_T          rect = {0};
    CTRLCOMBOX_OBJ_T *  combox_ctrl_ptr = (CTRLCOMBOX_OBJ_T*) base_ctrl_ptr;

    if (PNULL != base_ctrl_ptr
            && PNULL != rect_ptr
            && ComboxTypeOf (base_ctrl_ptr))
    {
        // 记录之前的区域
        rect = base_ctrl_ptr->rect;

        base_ctrl_ptr->display_rect = *rect_ptr;

        if (is_update)
        {
            ComboxDraw (combox_ctrl_ptr);
        }

        GUI_SetVOrHRect (base_ctrl_ptr->handle, (GUI_RECT_T *) rect_ptr, & (base_ctrl_ptr->both_rect));

        result = TRUE;
    }

    return (result);
}
/*****************************************************************************/
// 	Description : Get Edit String
//	Global resource dependence :
//  Author: xiyuan.ma & nan.ji
//	Note:
/*****************************************************************************/
PUBLIC void CTRLCOMBOX_GetEditString (
    MMI_CTRL_ID_T ctrl_id,
    MMI_STRING_T *str_ptr
)
{
    CTRLCOMBOX_OBJ_T *combox_ctrl_ptr = GetComboxPtr (ctrl_id);

    if (PNULL != combox_ctrl_ptr)
    {
        GUIEDIT_GetString (combox_ctrl_ptr->text_handle, str_ptr);
    }

    return;
}


/*****************************************************************************/
// 	Description : Clear Edit All String
//	Global resource dependence :
//  Author: xiyuan.ma & nan.ji
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLCOMBOX_ClearEditAllStr (
    MMI_CTRL_ID_T    ctrl_id
)
{
    CTRLCOMBOX_OBJ_T *combox_ctrl_ptr = GetComboxPtr (ctrl_id);

    if (PNULL == combox_ctrl_ptr)
    {
        return FALSE;
    }

    return GUIEDIT_ClearAllStr (combox_ctrl_ptr->text_handle);
}


/*****************************************************************************/
// 	Description : Get Edit List Item Number
//	Global resource dependence :
//  Author: xiyuan.ma & nan.ji
//	Note:
/*****************************************************************************/
PUBLIC uint16 CTRLCOMBOX_GetEditListItemNum (
    MMI_CTRL_ID_T    ctrl_id
)
{
    CTRLCOMBOX_OBJ_T *combox_ctrl_ptr = GetComboxPtr (ctrl_id);

    if (PNULL == combox_ctrl_ptr)
    {
        return 0;
    }

    return GUIEDIT_GetListItemNum (combox_ctrl_ptr->text_handle);
}


/*****************************************************************************/
// 	Description : Get Edit Current sor Current Position
//	Global resource dependence :
//  Author: xiyuan.ma & nan.ji
//	Note:
/*****************************************************************************/
PUBLIC uint16 CTRLCOMBOX_GetEditCursorCurPos (
    MMI_CTRL_ID_T    ctrl_id
)
{
    CTRLCOMBOX_OBJ_T *combox_ctrl_ptr = GetComboxPtr (ctrl_id);

    if (PNULL == combox_ctrl_ptr)
    {
        return 0;
    }

    return GUIEDIT_GetCursorCurPos (combox_ctrl_ptr->text_handle);
}


/*****************************************************************************/
// 	Description : Get Edit List Item String
//	Global resource dependence :
//  Author: xiyuan.ma & nan.ji
//	Note:
/*****************************************************************************/
PUBLIC void CTRLCOMBOX_GetEditListItemString (
    MMI_CTRL_ID_T ctrl_id,
    uint16 array_len,
    GUIEDIT_LIST_ITEM_STR_T *item_array_ptr
)
{
    CTRLCOMBOX_OBJ_T *combox_ctrl_ptr = GetComboxPtr (ctrl_id);

    if (PNULL != combox_ctrl_ptr)
    {
        GUIEDIT_GetListItemString (combox_ctrl_ptr->text_handle, array_len, item_array_ptr);
    }

    return;
}


/*****************************************************************************/
// 	Description : Is Edit TP Panel Visible
//	Global resource dependence :
//  Author: xiyuan.ma & nan.ji
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLCOMBOX_IsEditTPPanelVisible (
    MMI_CTRL_ID_T    ctrl_id
)
{
    CTRLCOMBOX_OBJ_T *combox_ctrl_ptr = GetComboxPtr (ctrl_id);

    if (PNULL == combox_ctrl_ptr)
    {
        return FALSE;
    }

    return GUIEDIT_IsTPPanelVisible (combox_ctrl_ptr->text_handle);
}


/*****************************************************************************/
// 	Description : Set Edit Softkey
//	Global resource dependence :
//  Author: xiyuan.ma & nan.ji
//	Note:
/*****************************************************************************/
PUBLIC void CTRLCOMBOX_SetEditSoftkey (
    MMI_CTRL_ID_T ctrl_id,
    uint16 index,
    uint16 str_len,
    MMI_TEXT_ID_T text_1,
    MMI_TEXT_ID_T text_2,
    MMITHEME_EDIT_SET_BUTTON_PFUNC setsoftkey
)
{
    CTRLCOMBOX_OBJ_T *combox_ctrl_ptr = GetComboxPtr (ctrl_id);

    if (PNULL != combox_ctrl_ptr)
    {
        GUIEDIT_SetSoftkey (combox_ctrl_ptr->text_handle, index, str_len, text_1, text_2, setsoftkey);
    }

    return;
}


/*****************************************************************************/
// 	Description : Set Edit Default String By Id
//	Global resource dependence :
//  Author: xiyuan.ma & nan.ji
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLCOMBOX_SetEditDefaultStringById (
    MMI_CTRL_ID_T ctrl_id,
    MMI_TEXT_ID_T default_string_id
)
{
    CTRLCOMBOX_OBJ_T *combox_ctrl_ptr = GetComboxPtr (ctrl_id);

    if (PNULL == combox_ctrl_ptr)
    {
        return FALSE;
    }

    return GUIEDIT_SetDefaultStringById (combox_ctrl_ptr->text_handle, default_string_id);
}


/*****************************************************************************/
// 	Description : Set Edit Font
//	Global resource dependence :
//  Author: xiyuan.ma & nan.ji
//	Note:
/*****************************************************************************/
PUBLIC void CTRLCOMBOX_SetEditFont (
    MMI_CTRL_ID_T ctrl_id,
    GUI_FONT_T font
)
{
    CTRLCOMBOX_OBJ_T *combox_ctrl_ptr = GetComboxPtr (ctrl_id);

    if (PNULL != combox_ctrl_ptr)
    {
        GUIEDIT_SetFont (combox_ctrl_ptr->text_handle, font);
    }

    return;
}


/*****************************************************************************/
// 	Description : Add Edit List Item Array
//	Global resource dependence :
//  Author: xiyuan.ma & nan.ji
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLCOMBOX_AddEditListItemArray (
    MMI_CTRL_ID_T ctrl_id,
    GUIEDIT_LIST_ITEM_T *list_item_ptr,
    uint16 list_item_count
)
{
    CTRLCOMBOX_OBJ_T *combox_ctrl_ptr = GetComboxPtr (ctrl_id);

    if (PNULL == combox_ctrl_ptr)
    {
        return FALSE;
    }

    return GUIEDIT_AddListItemArray (combox_ctrl_ptr->text_handle, list_item_ptr, list_item_count);
}

/*****************************************************************************/
// 	Description : Is Edit Active
//	Global resource dependence :
//    Author: xiyuan.ma & nan.ji
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLCOMBOX_IsEditActive (
    MMI_CTRL_ID_T ctrl_id
)
{
    CTRLCOMBOX_OBJ_T *combox_ctrl_ptr = GetComboxPtr (ctrl_id);

    if (PNULL == combox_ctrl_ptr)
    {
        return FALSE;
    }

    return MMK_IsActiveCtrl (combox_ctrl_ptr->text_handle);
}

/*****************************************************************************/
// 	Description : Is Edit Active
//	Global resource dependence :
//    Author: xiyuan.ma & nan.ji
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLCOMBOX_SetEditActive (
    MMI_HANDLE_T win_handle,
    MMI_CTRL_ID_T ctrl_id
)
{
    CTRLCOMBOX_OBJ_T *combox_ctrl_ptr = GetComboxPtr (ctrl_id);

    if (PNULL == combox_ctrl_ptr)
    {
        return FALSE;
    }

    return MMK_SetAtvCtrl (win_handle, combox_ctrl_ptr->text_handle);
}
/*****************************************************************************/
// 	Description : Set Edit Bg
//	Global resource dependence :
//    Author: xiyuan.ma & nan.ji
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLCOMBOX_SetEditBg (
    MMI_CTRL_ID_T ctrl_id,
    GUI_BG_T *bg_ptr
)
{
    CTRLCOMBOX_OBJ_T *combox_ctrl_ptr = GetComboxPtr (ctrl_id);

    if (PNULL == combox_ctrl_ptr)
    {
        return FALSE;
    }

    return CTRLBASE_SetBg (combox_ctrl_ptr->text_handle, bg_ptr);
}


/*****************************************************************************/
// 	Description : Get Edit List Item Array
//	Global resource dependence :
//    Author: xiyuan.ma & nan.ji
//	Note:
/*****************************************************************************/
PUBLIC void CTRLCOMBOX_GetEditListItemArray (
    MMI_CTRL_ID_T ctrl_id,
    uint16 array_len,
    GUIEDIT_LIST_ITEM_T *item_array_ptr //out
)
{
    CTRLCOMBOX_OBJ_T *combox_ctrl_ptr = GetComboxPtr (ctrl_id);

    if (PNULL != combox_ctrl_ptr)
    {
        GUIEDIT_GetListItemArray (combox_ctrl_ptr->text_handle, array_len, item_array_ptr);
    }

    return;
}


/*****************************************************************************/
// 	Description : Set Expand Direction
//	Global resource dependence :
//  Author: xiyuan.ma & nan.ji
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLCOMBOX_SetExpandDirection (
    MMI_CTRL_ID_T    ctrl_id,
    CTRLCOMBOX_SHOW_TYPE_E expand_direction
)
{
    BOOLEAN                 result = FALSE;
    CTRLCOMBOX_OBJ_T *      combox_ctrl_ptr = PNULL;

    //get combox pointer by control id
    combox_ctrl_ptr = GetComboxPtr (ctrl_id);

    if (PNULL != combox_ctrl_ptr)
    {
        combox_ctrl_ptr->expand_direction = expand_direction;
        result = TRUE;
    }

    return (result);
}
/*****************************************************************************/
// 	Description : Set BoxList Space
//	Global resource dependence :
//  Author: xiyuan.ma & nan.ji
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLCOMBOX_SetBoxListSpace (
    MMI_CTRL_ID_T    ctrl_id,
    uint16      box_list_space
)
{
    BOOLEAN              result = FALSE;
    CTRLCOMBOX_OBJ_T *   combox_ctrl_ptr = PNULL;

    //get combox pointer by control id
    combox_ctrl_ptr = GetComboxPtr (ctrl_id);

    if (PNULL != combox_ctrl_ptr)
    {
        combox_ctrl_ptr->box_list_space = box_list_space;
        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
// 	Description : Set RightAligned
//	Global resource dependence :
//  Author: xiyuan.ma & nan.ji
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLCOMBOX_SetRightAligned (
    MMI_CTRL_ID_T    ctrl_id,
    BOOLEAN     is_right_aligned
)
{
    BOOLEAN              result = FALSE;
    CTRLCOMBOX_OBJ_T *   combox_ctrl_ptr = PNULL;

    //get combox pointer by control id
    combox_ctrl_ptr = GetComboxPtr (ctrl_id);

    if (PNULL != combox_ctrl_ptr)
    {
        combox_ctrl_ptr->is_right_aligned = is_right_aligned;
        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
// 	Description : Set Fixed List
//	Global resource dependence :
//  Author: xiyuan.ma & nan.ji
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLCOMBOX_SetFixedList (
    MMI_CTRL_ID_T    ctrl_id,
    BOOLEAN     is_list_fixed
)
{
    BOOLEAN             result = FALSE;
    CTRLCOMBOX_OBJ_T *  combox_ctrl_ptr = PNULL;

    //get combox pointer by control id
    combox_ctrl_ptr = GetComboxPtr (ctrl_id);

    if (PNULL != combox_ctrl_ptr)
    {
        combox_ctrl_ptr->is_list_fixed = is_list_fixed;
        result = TRUE;
    }

    return (result);
}
/*****************************************************************************/
// 	Description : Disable WebKey
//	Global resource dependence :
//  Author: xiyuan.ma & nan.ji
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLCOMBOX_DisableWebKey (
    MMI_CTRL_ID_T    ctrl_id,
    BOOLEAN      is_web_key_open_list
)
{
    BOOLEAN              result = FALSE;
    CTRLCOMBOX_OBJ_T *   combox_ctrl_ptr = PNULL;

    //get combox pointer by control id
    combox_ctrl_ptr = GetComboxPtr (ctrl_id);

    if (PNULL != combox_ctrl_ptr)
    {
        combox_ctrl_ptr->is_web_key_open_list = is_web_key_open_list;
        result = TRUE;
    }

    return (result);
}
/*****************************************************************************/
// 	Description : Disable Mid Softkey
//	Global resource dependence :
//  Author: xiyuan.ma & nan.ji
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLCOMBOX_DisableMidSoftkey (
    MMI_CTRL_ID_T    ctrl_id,
    BOOLEAN      is_mid_softkey_open_list
)
{
    BOOLEAN              result = FALSE;
    CTRLCOMBOX_OBJ_T *   combox_ctrl_ptr = PNULL;

    //get combox pointer by control id
    combox_ctrl_ptr = GetComboxPtr (ctrl_id);

    if (PNULL != combox_ctrl_ptr)
    {
        combox_ctrl_ptr->is_mid_softkey_open_list = is_mid_softkey_open_list;
        result = TRUE;
    }

    return (result);
}


/*****************************************************************************/
// 	Description : Set Has Icon
//	Global resource dependence :
//  Author: xiyuan.ma & nan.ji
//	Note:
/*****************************************************************************/

PUBLIC BOOLEAN CTRLCOMBOX_SetHasIcon (
    MMI_CTRL_ID_T    ctrl_id,
    BOOLEAN      has_icon
)
{
    BOOLEAN                 result = FALSE;
    CTRLCOMBOX_OBJ_T        *combox_ctrl_ptr = PNULL;

    //get combox pointer by control id
    combox_ctrl_ptr = GetComboxPtr (ctrl_id);

    if (PNULL != combox_ctrl_ptr)
    {
        combox_ctrl_ptr->has_icon = has_icon;
        result = TRUE;
    }

    return (result);
}


/*****************************************************************************/
// 	Description : Set Icon Id
//	Global resource dependence :
//  Author: xiyuan.ma & nan.ji
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLCOMBOX_SetIconId (
    MMI_CTRL_ID_T    ctrl_id,
    uint32      icon_id
)
{
    BOOLEAN                 result = FALSE;
    CTRLCOMBOX_OBJ_T        *combox_ctrl_ptr = PNULL;

    //get combox pointer by control id
    combox_ctrl_ptr = GetComboxPtr (ctrl_id);

    if (PNULL != combox_ctrl_ptr)
    {
        combox_ctrl_ptr->icon_id = icon_id;
        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
// 	Description : Set combox text
//	Global resource dependence :
//  Author: nan.ji & xiyuan.ma
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLCOMBOX_SetText (
    MMI_CTRL_ID_T  ctrl_id,
    MMI_STRING_T	*str_ptr
)
{
    BOOLEAN                 result = FALSE;
    CTRLCOMBOX_OBJ_T        *combox_ctrl_ptr = PNULL;

    //get combox pointer by control id
    combox_ctrl_ptr = GetComboxPtr (ctrl_id);

    if (PNULL != combox_ctrl_ptr
            && PNULL != str_ptr)
    {
        result = CTRLBASEFLEX_SetString (combox_ctrl_ptr->text_handle, str_ptr->wstr_ptr, str_ptr->wstr_len);
    }

    return (result);
}
/*****************************************************************************/
// 	Description : Get combox text
//	Global resource dependence :
//  Author: nan.ji & xiyuan.ma
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLCOMBOX_GetText (
    MMI_CTRL_ID_T    ctrl_id,
    MMI_STRING_T     *str_ptr    //in:/out
)
{
    BOOLEAN                 result = FALSE;
    CTRLCOMBOX_OBJ_T        *combox_ctrl_ptr = PNULL;

    //get combox pointer by control id
    combox_ctrl_ptr = GetComboxPtr (ctrl_id);

    if (PNULL != combox_ctrl_ptr)
    {
        CTRLBASEFLEX_GetString (combox_ctrl_ptr->text_handle, str_ptr);
        result = TRUE;
    }

    return result;
}

/*****************************************************************************/
// 	Description : Set Function Button Callback
//	Global resource dependence :
//  Author: nan.ji & xiyuan.ma
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLCOMBOX_SetFuncButtonCallBack (
    MMI_CTRL_ID_T               ctrl_id,
    GUIBUTTON_CALLBACK_FUNC_EXT func,
    void*                       user_data //
)
{
    BOOLEAN result = FALSE;
    CTRLCOMBOX_OBJ_T *combox_ctrl_ptr = PNULL;

    //get combox pointer by control id
    combox_ctrl_ptr = GetComboxPtr (ctrl_id);

    if (PNULL != combox_ctrl_ptr)
    {
        CTRLBASE_SetUserData (combox_ctrl_ptr->function_button_handle, (uint32) user_data);
        result = CTRLBUTTON_SetCallBackFuncExt (combox_ctrl_ptr->function_button_handle, func);
    }

    return (result);
}
/*****************************************************************************/
// 	Description : Add Item Detail
//	Global resource dependence :
//  Author: nan.ji & xiyuan.ma
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLCOMBOX_AddItemDetail (
    MMI_CTRL_ID_T ctrl_id,
    const CTRLCOMBOX_DETAIL_STR_DATA_T *data_ptr//item info
)
{
    BOOLEAN             result = FALSE;
    CTRLLIST_ITEM_T      item = {0};
    CTRLCOMBOX_OBJ_T *  combox_ctrl_ptr = PNULL;
    GUILIST_ITEM_DATA_T item_data = {0};/*lint !e64*/

    //get combox pointer by control id
    combox_ctrl_ptr = GetComboxPtr (ctrl_id);

    if (PNULL != combox_ctrl_ptr
            && PNULL != data_ptr)
    {
        item.style_ptr = THEMELIST_GetStyle (GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT);
        item.data_ptr = &item_data;

        item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
        item_data.item_content[0].item_data.text_buffer.wstr_ptr = data_ptr->str.wstr_ptr;
        item_data.item_content[0].item_data.text_buffer.wstr_len = data_ptr->str.wstr_len;

        item_data.item_content[1].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
        item_data.item_content[1].item_data.text_buffer.wstr_ptr = data_ptr->detail_str.wstr_ptr;
        item_data.item_content[1].item_data.text_buffer.wstr_len = data_ptr->detail_str.wstr_len;

        result = CTRLLIST_AppendItem (combox_ctrl_ptr->list_handle, &item);
    }

    return (result);
}

/*****************************************************************************/
// 	Description : Add Item
//	Global resource dependence :
//  Author: nan.ji & xiyuan.ma
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLCOMBOX_AddItemDetailArray (
    MMI_CTRL_ID_T ctrl_id,
    const CTRLCOMBOX_DETAIL_STR_DATA_T *data_ptr,
    uint16 array_size
)
{
    uint16            i = 0;
    BOOLEAN           result = FALSE;
    CTRLCOMBOX_OBJ_T  *combox_ctrl_ptr = PNULL;
    CTRLBASE_OBJ_T    *base_ctrl_ptr   = PNULL;

    //get combox pointer by control id
    combox_ctrl_ptr = GetComboxPtr (ctrl_id);
    base_ctrl_ptr = (CTRLBASE_OBJ_T *) combox_ctrl_ptr;

    if (PNULL != combox_ctrl_ptr
            && array_size > 0
            && PNULL != data_ptr)
    {
        for (i = 0; i < array_size; i++)
        {
            result = CTRLCOMBOX_AddItemDetail (base_ctrl_ptr->handle, &data_ptr[i]);

            if (!result)
            {
                SCI_TRACE_LOW ("CTRLCOMBOX_AddItemArray  AppendItem failed");
                break;
            }
        }
    }

    return (result);
}
/*****************************************************************************/
// 	Description : Add Item
//	Global resource dependence :
//  Author: nan.ji & xiyuan.ma
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLCOMBOX_AddItem (
    MMI_CTRL_ID_T ctrl_id,
    const MMI_STRING_T *str_ptr //item info
)
{
    BOOLEAN             result = FALSE;
    CTRLCOMBOX_OBJ_T    *combox_ctrl_ptr = PNULL;
    CTRLLIST_ITEM_T      item = {0};
    GUILIST_ITEM_DATA_T item_data = {0};/*lint !e64*/

    //get combox pointer by control id
    combox_ctrl_ptr = GetComboxPtr (ctrl_id);

    if (PNULL != combox_ctrl_ptr
            && PNULL != str_ptr)
    {
        item.style_ptr = THEMELIST_GetStyle (GUIITEM_STYLE_ONE_LINE_TEXT);
        item.data_ptr = &item_data;

        item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
        item_data.item_content[0].item_data.text_buffer.wstr_ptr = str_ptr->wstr_ptr;
        item_data.item_content[0].item_data.text_buffer.wstr_len = str_ptr->wstr_len;

        result = CTRLLIST_AppendItem (combox_ctrl_ptr->list_handle, &item);
    }

    return (result);
}

/*****************************************************************************/
// 	Description : Add Item
//	Global resource dependence :
//  Author: nan.ji & xiyuan.ma
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLCOMBOX_AddItemArray (
    MMI_CTRL_ID_T ctrl_id,
    const MMI_STRING_T *str_ptr,	// appointed Item
    uint16 array_size
)
{
    uint16            i = 0;
    BOOLEAN           result = FALSE;
    CTRLCOMBOX_OBJ_T  *combox_ctrl_ptr = PNULL;
    CTRLBASE_OBJ_T    *base_ctrl_ptr   = PNULL;

    //get combox pointer by control id
    combox_ctrl_ptr = GetComboxPtr (ctrl_id);
    base_ctrl_ptr = (CTRLBASE_OBJ_T *) combox_ctrl_ptr;

    if (PNULL != combox_ctrl_ptr
            && array_size > 0
            && PNULL != str_ptr)
    {
        for (i = 0; i < array_size; i++)
        {
            result = CTRLCOMBOX_AddItem (base_ctrl_ptr->handle, &str_ptr[i]);

            if (!result)
            {
                SCI_TRACE_LOW ("CTRLCOMBOX_AddItemArray  AppendItem failed");
                break;
            }
        }
    }

    return (result);
}

/*****************************************************************************/
// 	Description : Insert Item
//	Global resource dependence :
//  Author: nan.ji & xiyuan.ma
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLCOMBOX_InsertItem (MMI_CTRL_ID_T ctrl_id,
                                      const CTRLLIST_ITEM_T*   item_ptr,// appointed Item
                                      uint16 pos
                                     )
{
    BOOLEAN           result = FALSE;
    CTRLCOMBOX_OBJ_T  *combox_ctrl_ptr = PNULL;

    //get combox pointer by control id
    combox_ctrl_ptr = GetComboxPtr (ctrl_id);

    if (PNULL != combox_ctrl_ptr)
    {
        result = CTRLLIST_InsertItem (combox_ctrl_ptr->list_handle, item_ptr, pos);
    }

    return (result);
}
/*****************************************************************************/
// 	Description : Remove Item
//	Global resource dependence :
//  Author: nan.ji & xiyuan.ma
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLCOMBOX_RemoveItem (
    MMI_CTRL_ID_T ctrl_id,
    uint16 pos
)
{
    BOOLEAN           result = FALSE;
    CTRLCOMBOX_OBJ_T  *combox_ctrl_ptr = PNULL;

    //get combox pointer by control id
    combox_ctrl_ptr = GetComboxPtr (ctrl_id);

    if (PNULL != combox_ctrl_ptr)
    {
        result = CTRLLIST_RemoveItem (combox_ctrl_ptr->list_handle, pos);
    }

    return (result);
}

/*****************************************************************************/
// 	Description : Remove All Items
//	Global resource dependence :
//  Author: nan.ji & xiyuan.ma
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLCOMBOX_RemoveAllItems (
    MMI_CTRL_ID_T ctrl_id
)
{
    BOOLEAN           result = FALSE;
    CTRLCOMBOX_OBJ_T  *combox_ctrl_ptr = PNULL;

    //get combox pointer by control id
    combox_ctrl_ptr = GetComboxPtr (ctrl_id);

    if (PNULL != combox_ctrl_ptr)
    {
        result = CTRLLIST_RemoveAllItems (combox_ctrl_ptr->list_handle);
    }

    return (result);
}

/*****************************************************************************/
// 	Description : Get Item Count
//	Global resource dependence :
//  Author: nan.ji & xiyuan.ma
//	Note:
/*****************************************************************************/
PUBLIC uint16 CTRLCOMBOX_GetItemCount (
    MMI_CTRL_ID_T ctrl_id
)
{
    uint16           item_count = 0;
    CTRLCOMBOX_OBJ_T  *combox_ctrl_ptr = PNULL;

    //get combox pointer by control id
    combox_ctrl_ptr = GetComboxPtr (ctrl_id);

    if (PNULL != combox_ctrl_ptr)
    {
        item_count = CTRLLIST_GetTotalItemNum (combox_ctrl_ptr->list_handle);
    }

    return (item_count);
}

/*****************************************************************************/
// 	Description : Get Item
//	Global resource dependence :
//  Author: nan.ji & xiyuan.ma
//	Note:
/*****************************************************************************/
PUBLIC const CTRLLIST_ITEM_T* CTRLCOMBOX_GetItem (
    MMI_CTRL_ID_T ctrl_id,
    uint16 index
)
{
    const CTRLLIST_ITEM_T *  item_ptr = PNULL;
    CTRLCOMBOX_OBJ_T *      combox_ctrl_ptr = PNULL;

    //get combox pointer by control id
    combox_ctrl_ptr = GetComboxPtr (ctrl_id);

    if (PNULL != combox_ctrl_ptr)
    {
        item_ptr = CTRLLIST_GetItem(combox_ctrl_ptr->list_handle,index);
    }

    return (item_ptr);
}

/*****************************************************************************/
// 	Description : Get Select Index
//	Global resource dependence :
//  Author: nan.ji & xiyuan.ma
//	Note:
/*****************************************************************************/
PUBLIC uint16 CTRLCOMBOX_SetSelectIndex (
    MMI_CTRL_ID_T ctrl_id,
    uint16 index
)
{
    BOOLEAN           result = FALSE;
    CTRLCOMBOX_OBJ_T  *combox_ctrl_ptr = PNULL;

    //get combox pointer by control id
    combox_ctrl_ptr = GetComboxPtr (ctrl_id);

    if (PNULL != combox_ctrl_ptr)
    {
        result = CTRLLIST_SetCurItemIndex (combox_ctrl_ptr->list_handle, index);
    }

    return (result);
}

/*****************************************************************************/
// 	Description : Get Select Index
//	Global resource dependence :
//  Author: nan.ji & xiyuan.ma
//	Note:
/*****************************************************************************/
PUBLIC uint16 CTRLCOMBOX_GetSelectIndex (
    MMI_CTRL_ID_T ctrl_id
)
{
    uint16 index = 0;
    CTRLCOMBOX_OBJ_T  *combox_ctrl_ptr = PNULL;

    //get combox pointer by control id
    combox_ctrl_ptr = GetComboxPtr (ctrl_id);

    if (PNULL != combox_ctrl_ptr)
    {
        index = CTRLLIST_GetCurItemIndex (combox_ctrl_ptr->list_handle);
    }

    return (index);
}

/*****************************************************************************/
// 	Description : Set Edit Type
//	Global resource dependence :
//  Author: nan.ji & xiyuan.ma
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLCOMBOX_SetEditType (
    MMI_CTRL_ID_T ctrl_id,
    const CTRLCOMBOX_TEXT_TYPE_T *edit_type_ptr
)
{
    BOOLEAN               result = FALSE;
    CTRLBASE_OBJ_T *      base_ctrl_ptr = PNULL;
    CTRLCOMBOX_OBJ_T *    combox_ctrl_ptr = PNULL;
    CTRLCOMBOX_TEXT_TYPE_T edit_info = {0};

    //get combox pointer by control id
    combox_ctrl_ptr = GetComboxPtr (ctrl_id);
    base_ctrl_ptr = (CTRLBASE_OBJ_T *) combox_ctrl_ptr;

    if (PNULL != combox_ctrl_ptr
            && PNULL != edit_type_ptr)
    {
        edit_info.edit_type = edit_type_ptr->edit_type;
        edit_info.editable_max_listitem_num = edit_type_ptr->editable_max_listitem_num;
        edit_info.editable_max_text_num = edit_type_ptr->editable_max_text_num;

        combox_ctrl_ptr->edit_info = edit_info;

        result = CreateTextCtrl (base_ctrl_ptr);
    }

    return result;
}

/*****************************************************************************/
// 	Description : Get Edit Type
//	Global resource dependence :
//  Author: nan.ji & xiyuan.ma
//	Note:
/*****************************************************************************/
PUBLIC GUIEDIT_TYPE_E CTRLCOMBOX_GetEditType (
    MMI_CTRL_ID_T ctrl_id
)
{
    CTRLCOMBOX_OBJ_T  *combox_ctrl_ptr = PNULL;
    GUIEDIT_TYPE_E     edit_type = GUIEDIT_TYPE_NULL;

    //get combox pointer by control id
    combox_ctrl_ptr = GetComboxPtr (ctrl_id);

    if (PNULL != combox_ctrl_ptr)
    {
        edit_type = combox_ctrl_ptr->edit_info.edit_type;
    }

    return edit_type;
}

/*****************************************************************************/
// 	Description : Set List Item Max Num
//	Global resource dependence :
//  Author: nan.ji & xiyuan.ma
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLCOMBOX_SetListItemMaxNum (
    MMI_CTRL_ID_T ctrl_id,
    uint16        max_item_num,
    BOOLEAN       is_dynamic_item_data
)
{
    BOOLEAN            result = FALSE;
    CTRLCOMBOX_OBJ_T * combox_ctrl_ptr = PNULL;

    //get combox pointer by control id
    combox_ctrl_ptr = GetComboxPtr (ctrl_id);

    if (PNULL != combox_ctrl_ptr)
    {
        result = GUILIST_SetMaxItem (combox_ctrl_ptr->list_handle, max_item_num, is_dynamic_item_data);
    }

    return result;
}

/*****************************************************************************/
// 	Description : Set Icon
//	Global resource dependence :
//  Author: nan.ji & xiyuan.ma
//	Note:
/*****************************************************************************/
PUBLIC void CTRLCOMBOX_SetIcon (
    MMI_CTRL_ID_T ctrl_id,
    BOOLEAN is_show_icon,
    MMI_IMAGE_ID_T img_id
)
{
    CTRLCOMBOX_OBJ_T  *combox_ctrl_ptr = PNULL;

    //get combox pointer by control id
    combox_ctrl_ptr = GetComboxPtr (ctrl_id);

    if (PNULL != combox_ctrl_ptr
            && is_show_icon)
    {
        combox_ctrl_ptr->has_icon = is_show_icon;
        combox_ctrl_ptr->icon_id = img_id;
    }

    return;
}

/*****************************************************************************/
// 	Description : Set Expand Button
//	Global resource dependence :
//  Author: nan.ji & xiyuan.ma
//	Note:
/*****************************************************************************/
PUBLIC void CTRLCOMBOX_SetExpandButton (
    MMI_CTRL_ID_T  ctrl_id,
    BOOLEAN is_show_expand_button
)
{
    CTRLCOMBOX_OBJ_T  *combox_ctrl_ptr = PNULL;
    CTRLBASE_OBJ_T    *base_ctrl_ptr   = PNULL;

    //get combox pointer by control id
    combox_ctrl_ptr = GetComboxPtr (ctrl_id);

    if (PNULL != combox_ctrl_ptr
            && is_show_expand_button)
    {
        base_ctrl_ptr = (CTRLBASE_OBJ_T *) combox_ctrl_ptr;

        combox_ctrl_ptr->has_expand_button = is_show_expand_button;

        CreateExpandButton (base_ctrl_ptr);
    }

    return;
}

/*****************************************************************************/
// 	Description : Set Func Button
//	Global resource dependence :
//  Author: nan.ji & xiyuan.ma
//	Note:
/*****************************************************************************/
PUBLIC void CTRLCOMBOX_SetFuncButton (
    MMI_CTRL_ID_T  ctrl_id,
    BOOLEAN is_show_fucn_button
)
{
    CTRLCOMBOX_OBJ_T  *combox_ctrl_ptr = PNULL;
    CTRLBASE_OBJ_T    *base_ctrl_ptr   = PNULL;
    //get combox pointer by control id
    combox_ctrl_ptr = GetComboxPtr (ctrl_id);

    if (PNULL != combox_ctrl_ptr
            && is_show_fucn_button)
    {
        base_ctrl_ptr = (CTRLBASE_OBJ_T *) combox_ctrl_ptr;

        combox_ctrl_ptr->has_function_button = is_show_fucn_button;

        CreateFuncButton (base_ctrl_ptr);
    }

    return;
}
/*****************************************************************************/
// 	Description : set rect
//	Global resource dependence :
//  Author: xiyuan.ma & nan.ji
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLCOMBOX_SetRect (
    MMI_HANDLE_T ctrl_handle,
    const GUI_RECT_T* rect_ptr
)
{
    return ComboxCtrlSetRect ( (CTRLBASE_OBJ_T*) GetComboxPtr (ctrl_handle), rect_ptr);
}
/*****************************************************************************/
//  Description : 传入text_id,添加dropdownlist
//  Global resource dependence :
//  Author: xiyuan.ma & nan.ji
//  Note:添加成功返回TRUE，失败返回FALSE
/*****************************************************************************/
PUBLIC BOOLEAN CTRLCOMBOX_AppendItemById (
    MMI_CTRL_ID_T ctrl_id, // control id
    MMI_TEXT_ID_T text_id //text id
)
{
    MMI_STRING_T        item_string = {0};
    CTRLBASE_OBJ_T *    base_ctrl_ptr = PNULL;
    CTRLCOMBOX_OBJ_T *  combox_ctrl_ptr = (CTRLCOMBOX_OBJ_T *) GetComboxPtr (ctrl_id);

    if (PNULL == combox_ctrl_ptr)
    {
        return FALSE;
    }

    base_ctrl_ptr = (CTRLBASE_OBJ_T *) combox_ctrl_ptr;

    MMITHEME_GetResText (text_id, base_ctrl_ptr->win_handle, &item_string);

    return CTRLCOMBOX_AddItem (ctrl_id, &item_string);
}
/*****************************************************************************/
//  Description : get edit handle
//  Global resource dependence :
//  Author: xiyuan.ma & nan.ji
//  Note:添加成功返回TRUE，失败返回FALSE
/*****************************************************************************/
PUBLIC MMI_HANDLE_T CTRLCOMBOX_GetInlineEditHandle (
    MMI_HANDLE_T  win_handle,    //window id
    MMI_CTRL_ID_T ctrl_id // control id
)
{
    MMI_HANDLE_T        edit_handle = 0;
    CTRLCOMBOX_OBJ_T *  combox_ctrl_ptr = (CTRLCOMBOX_OBJ_T *) GetComboxPtr (ctrl_id);

    if (PNULL != combox_ctrl_ptr)
    {
        edit_handle = combox_ctrl_ptr->text_handle;
    }

    return edit_handle;
}
/*****************************************************************************/
//  Description : get list handle
//  Global resource dependence :
//  Author: xiyuan.ma & nan.ji
//  Note:添加成功返回TRUE，失败返回FALSE
/*****************************************************************************/
PUBLIC MMI_HANDLE_T CTRLCOMBOX_GetInlineListHandle (
    MMI_CTRL_ID_T ctrl_id // control id
)
{
    MMI_HANDLE_T              list_handle = 0;
    CTRLCOMBOX_OBJ_T *  combox_ctrl_ptr = (CTRLCOMBOX_OBJ_T *) GetComboxPtr (ctrl_id);

    if (PNULL != combox_ctrl_ptr)
    {
        list_handle = combox_ctrl_ptr->list_handle;
    }

    return list_handle;
}
/*****************************************************************************/
//  Description : 获得edit的type
//  Global resource dependence :
//  Author: xiyuan.ma & nan.ji
//  Note:添加成功返回TRUE，失败返回FALSE
/*****************************************************************************/
PUBLIC uint16 CTRLCOMBOX_GetTotalItemNum (
    MMI_CTRL_ID_T ctrl_id // control id
)
{
    uint16              total = 0;
    CTRLCOMBOX_OBJ_T *  combox_ctrl_ptr = (CTRLCOMBOX_OBJ_T *) GetComboxPtr (ctrl_id);

    if (PNULL == combox_ctrl_ptr)
    {
        return total;
    }

    return CTRLLIST_GetTotalItemNum (combox_ctrl_ptr->list_handle);
}

/*****************************************************************************/
// 	Description : open dropdown list
//	Global resource dependence :
//  Author: xiyuan.ma & nan.ji
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLCOMBOX_OpenList (
    MMI_HANDLE_T  win_handle,    //window id
    MMI_CTRL_ID_T ctrl_id
)
{
    BOOLEAN             recode   = FALSE;
    CTRLCOMBOX_OBJ_T *  combox_ctrl_ptr = (CTRLCOMBOX_OBJ_T *) GetComboxPtr (ctrl_id);
    CTRLBASE_OBJ_T *    base_ctrl_ptr   = (CTRLBASE_OBJ_T *) combox_ctrl_ptr;

    if (MMI_RESULT_TRUE == ExpandList (base_ctrl_ptr))
    {
        recode   = TRUE;
    }

    return recode;
}
/*****************************************************************************/
// 	Description : open dropdown list
//	Global resource dependence :
//  Author: xiyuan.ma & nan.ji
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLCOMBOX_CloseList (
    MMI_HANDLE_T  win_handle,    //window id
    MMI_CTRL_ID_T ctrl_id
)
{
    BOOLEAN             recode       = FALSE;
    CTRLCOMBOX_OBJ_T *  combox_ctrl_ptr = (CTRLCOMBOX_OBJ_T *) GetComboxPtr (ctrl_id);
    CTRLBASE_OBJ_T *    base_ctrl_ptr   = (CTRLBASE_OBJ_T *) combox_ctrl_ptr;

    if (MMI_RESULT_TRUE == ShrinkList (base_ctrl_ptr))
    {
        recode = TRUE;
    }

    return recode;
}
/*****************************************************************************/
// 	Description : set function icon
//	Global resource dependence :
//  Author: xiyuan.ma & nan.ji
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLCOMBOX_SetFuncIcon (
    MMI_CTRL_ID_T ctrl_id,
    MMI_IMAGE_ID_T icon_id
)
{
    BOOLEAN             recode          = TRUE;
    GUI_RECT_T          button_rect     = {0};
    GUI_BG_T            bg              = {0};
    CTRLCOMBOX_OBJ_T *  combox_ctrl_ptr = (CTRLCOMBOX_OBJ_T *) GetComboxPtr (ctrl_id);

    if (PNULL == combox_ctrl_ptr
            || !combox_ctrl_ptr->has_function_button
            || 0 == combox_ctrl_ptr->function_button_handle)
    {
        return FALSE;
    }

    //set icon
    combox_ctrl_ptr->func_button_icon = icon_id;

    //set rect
    button_rect = CalcFuncButtonRect (combox_ctrl_ptr);

    if (!CTRLBASE_SetRect (combox_ctrl_ptr->function_button_handle, &button_rect))
    {
        recode = FALSE;
    }

    //set bg
    bg.bg_type = GUI_BG_IMG;
    bg.img_id = icon_id;

    if (!CTRLBUTTON_SetBg (combox_ctrl_ptr->function_button_handle, &bg))
    {
        recode = FALSE;
    }

    return recode;
}
