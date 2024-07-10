/*****************************************************************************
** File Name:      ctrlbaseedit.c                                               *
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

#include "ctrlbaseedit.h"

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
//  Description : init baseedit class
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseEditCtrlInitVtbl (
    CTRLBASEEDIT_VTBL_T        *baseedit_vtbl_ptr
);

/*****************************************************************************/
//  Description : baseedit construct
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseEditCtrlConstruct (
    CTRLBASE_OBJ_T           *base_ctrl_ptr,
    CTRLBASEEDIT_INIT_PARAM_T   *baseedit_param_ptr
);

/*****************************************************************************/
//  Description : baseedit destruct
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseEditCtrlDestruct (
    CTRLBASE_OBJ_T        *base_ctrl_ptr
);

///*****************************************************************************/
////  Description : convert init param
////  Global resource dependence :
////  Author: hua.fang
////  Note:
///*****************************************************************************/
//LOCAL void BaseEditCtrlPackInitParam (
//    GUIBASEEDIT_INIT_DATA_T     *baseedit_init_ptr,   //in
//    CTRLBASEEDIT_INIT_PARAM_T   *baseedit_param_ptr   //out
//);

/*****************************************************************************/
//  Description : handle baseedit msg function
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E BaseEditCtrlHandleMsg (
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
LOCAL BOOLEAN BaseEditTypeOf (
    CTRLBASE_OBJ_T     *ctrl_ptr
);

/*****************************************************************************/
// 	Description : set control is circular handle up/down key
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseEditCtrlSetUpDownLoop (
    CTRLBASE_OBJ_T       *base_ctrl_ptr,
    BOOLEAN              is_circular
);

/*****************************************************************************/
// 	Description : get control height by width,include content,border etc.
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseEditCtrlGetHeightByWidth (
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
LOCAL BOOLEAN BaseEditCtrlSetDisplayRect (
    CTRLBASE_OBJ_T      *base_ctrl_ptr, //in:
    const GUI_RECT_T    *rect_ptr,      //in:
    BOOLEAN             is_update       //in:
);

/*****************************************************************************/
//  Description : set edit is circular handle left/right key msg
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseEditCtrlSetLeftRightLoop (
    CTRLBASE_OBJ_T     *base_ctrl_ptr,
    BOOLEAN            is_circular
);

/*****************************************************************************/
// 	Description : set control select rect
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseEditCtrlGetSelectRect (
    CTRLBASE_OBJ_T         *base_ctrl_ptr,  //in:
    GUI_RECT_T             *rect_ptr       //in/out:
);

/*****************************************************************************/
// 	Description : set control select index,is the first or last line
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseEditCtrlGetSelectIndex (
    CTRLBASE_OBJ_T        *base_ctrl_ptr,  //in:
    BOOLEAN               *is_first_ptr,  //in/out:
    BOOLEAN               *is_last_ptr    //in/out:
);

/*****************************************************************************/
// 	Description : set control select index,is the first or last line
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseEditCtrlSetSelectIndex (
    CTRLBASE_OBJ_T        *base_ctrl_ptr,   //in:
    BOOLEAN               is_first,         //in:
    BOOLEAN               is_last           //in:
);

/*****************************************************************************/
//  Description : set edit rect
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseEditCtrlSetRect (
    CTRLBASE_OBJ_T    *base_ctrl_ptr,
    const CAF_RECT_T  *rect_ptr
);

/*****************************************************************************/
//  Description : set edit border
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseEditCtrlSetBorder (
    CTRLBASE_OBJ_T  *base_ctrl_ptr,
    GUI_BORDER_T    *border_ptr
);

/*****************************************************************************/
//  Description : set edit bg
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseEditCtrlSetBg (
    CTRLBASE_OBJ_T      *base_ctrl_ptr,
    GUI_BG_T            *bg_ptr
);

/*****************************************************************************/
//  Description : set edit font
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseEditCtrlSetFont (
    CTRLBASE_OBJ_T    *base_ctrl_ptr,
    GUI_FONT_ALL_T    *font_ptr
);

/*****************************************************************************/
//  Description : get edit font
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseEditCtrlGetFont (
    CTRLBASE_OBJ_T    *base_ctrl_ptr,
    GUI_FONT_ALL_T    *font_ptr
);

/*****************************************************************************/
//  Description : 获取edit的字符串内容
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL wchar* BaseEditCtrlGetDispStr (
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr,
    uint16              start_pos,
    uint16              str_len
);

/*****************************************************************************/
//  Description : handle landscape edit
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleLandscapeEdit (// is create im
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr, //in:
    BOOLEAN             is_tp_up            //in:
);

/*****************************************************************************/
//  Description : handle edit lose active msg
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleBaseEditLoseActive (
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr, //in:
    BOOLEAN             is_notify       //in
);

/*****************************************************************************/
//  Description : reset display information bar,include im icon and remaining characters
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void ResetInfoBar (
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr
);

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : get edit type
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC GUIEDIT_TYPE_E CTRLBASEEDIT_GetType (
    MMI_CTRL_ID_T ctrl_id
)
{
    GUIEDIT_TYPE_E      type = GUIEDIT_TYPE_NULL;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = PNULL;

    baseedit_ctrl_ptr = BASEEDIT_GetPtr (ctrl_id);
    if (PNULL != baseedit_ctrl_ptr)
    {
        type = baseedit_ctrl_ptr->type;
    }

    return (type);
}

/*****************************************************************************/
//  Description : set edit is display im icon and number info
//  Global resource dependence :
//  Author: Jassmine
//  Note: Default:text and list edit display,others not display
/*****************************************************************************/
PUBLIC void CTRLBASEEDIT_SetDispImIcon (
    MMI_CTRL_ID_T     ctrl_id,                  //in:
    BOOLEAN           is_disp_im_icon           //in:
)
{
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = PNULL;

    baseedit_ctrl_ptr = BASEEDIT_GetPtr (ctrl_id);
    if (PNULL != baseedit_ctrl_ptr)
    {
        baseedit_ctrl_ptr->is_disp_im_icon = is_disp_im_icon;
    }
}

/*****************************************************************************/
// 	Description : set edit is permit display border
//	Global resource dependence :
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBASEEDIT_PermitBorder (
    MMI_CTRL_ID_T   ctrl_id,
    BOOLEAN         is_permit
)
{
    BOOLEAN             result = FALSE;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = PNULL;

    baseedit_ctrl_ptr = BASEEDIT_GetPtr (ctrl_id);
    if (PNULL != baseedit_ctrl_ptr)
    {
        baseedit_ctrl_ptr->is_permit_border = is_permit;

        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : set edit bg is transparent display image
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBASEEDIT_SetBgTransparent (
    MMI_CTRL_ID_T   ctrl_id,
    BOOLEAN         is_transparent
)
{
    BOOLEAN             result = FALSE;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = PNULL;

    baseedit_ctrl_ptr = BASEEDIT_GetPtr (ctrl_id);
    if (PNULL != baseedit_ctrl_ptr)
    {
        baseedit_ctrl_ptr->is_bg_transparent = is_transparent;

        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : set edit margin
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLBASEEDIT_SetMargin (
    MMI_CTRL_ID_T     ctrl_id,    //in:
    uint16            margin_lr,  //in:left and right margin
    uint16            margin_tb   //in:top and bottom margin
)
{
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = PNULL;

    baseedit_ctrl_ptr = BASEEDIT_GetPtr (ctrl_id);
    if (PNULL != baseedit_ctrl_ptr)
    {
        if (GUIEDIT_CURSOR_DEFAULT_WIDTH <= baseedit_ctrl_ptr->common_theme.margin_lr_space)
        {
            baseedit_ctrl_ptr->common_theme.margin_lr_space = margin_lr;
        }

        baseedit_ctrl_ptr->common_theme.margin_tb_space = margin_tb;
    }
}

/*****************************************************************************/
//  Description : set edit font
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLBASEEDIT_SetFont (
    MMI_CTRL_ID_T       ctrl_id,
    GUI_FONT_T          *font_size_ptr,
    GUI_COLOR_T         *font_color_ptr
)
{
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = PNULL;
    GUI_FONT_ALL_T      font_all = {0};

    baseedit_ctrl_ptr = BASEEDIT_GetPtr (ctrl_id);
    if (PNULL != baseedit_ctrl_ptr)
    {
        font_all = baseedit_ctrl_ptr->common_theme.font;

        if (PNULL != font_size_ptr)
        {
            font_all.font = *font_size_ptr;
        }

        if (PNULL != font_color_ptr)
        {
            font_all.color = *font_color_ptr;
        }

        VTLBASE_SetFont (baseedit_ctrl_ptr, &font_all);
    }
}

/*****************************************************************************/
//  Description : set edit align
//  Global resource dependence :
//  Author: Jassmine
//  Note: only for single line
/*****************************************************************************/
PUBLIC void CTRLBASEEDIT_SetAlign (
    MMI_CTRL_ID_T  ctrl_id,//in
    GUI_ALIGN_E    align   //in
)
{
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = PNULL;

    baseedit_ctrl_ptr = BASEEDIT_GetPtr (ctrl_id);
    if ((PNULL != baseedit_ctrl_ptr) &&
        (GUIEDIT_STYLE_SINGLE == baseedit_ctrl_ptr->style))
    {
        VTLBASEEDIT_SetAlign (baseedit_ctrl_ptr, align);
    }
}

/*****************************************************************************/
//  Description : set edit rect is adaptive
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLBASEEDIT_SetRectAdaptive(
                                         MMI_CTRL_ID_T  ctrl_id,//in
                                         BOOLEAN        is_true //in:
                                         )
{
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = PNULL;

    baseedit_ctrl_ptr = BASEEDIT_GetPtr (ctrl_id);
    if (PNULL != baseedit_ctrl_ptr)
    {
        baseedit_ctrl_ptr->is_rect_adaptive = is_true;
    }
}

/*****************************************************************************/
//  Description : get baseedit type
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC TYPE_TAG_T BASEEDIT_GetType()
{
    static TYPE_TAG_T    obj_type = 0;

    if (0 == obj_type)
    {
        obj_type = TYPEMNG_Register (
                       CTRL_BASE_TYPE,                                     // parent class type
                       "baseedit",                                         // control name
                       sizeof (CTRLBASEEDIT_OBJ_T),                        // size of control object struct
                       (OBJECT_CONSTRUCTOR_FUNC) BaseEditCtrlConstruct,    // control constructor function
                       (OBJECT_DESTRUCTOR_FUNC) BaseEditCtrlDestruct,      // control destructor function
                       0,                                                  // size of init param struct
                       PNULL,                                              // convert init param
                       sizeof (CTRLBASEEDIT_VTBL_T),                       // size of virtual table
                       (VTBL_INIT_FUNC) BaseEditCtrlInitVtbl);             // init virtual table function
    }

    return obj_type;
}

/*****************************************************************************/
//  Description : get baseedit pointer by control id
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC CTRLBASEEDIT_OBJ_T* BASEEDIT_GetPtr (
    MMI_HANDLE_T     ctrl_handle
)
{
    CTRLBASE_OBJ_T  *ctrl_ptr = (CTRLBASE_OBJ_T*) MMK_GetCtrlPtr (ctrl_handle);

    if (PNULL != ctrl_ptr)
    {
        SCI_ASSERT (BaseEditTypeOf (ctrl_ptr)); /*assert verified*/
    }

    return (CTRLBASEEDIT_OBJ_T*) ctrl_ptr;
}

/*****************************************************************************/
//  Description : resize edit rect by im rect
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN BASEEDIT_ResizeRect (//edit is change
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr  //in
)
{
    BOOLEAN         result = FALSE;
    GUI_RECT_T      edit_rect = {0};
    GUI_RECT_T      im_rect = {0};
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*) baseedit_ctrl_ptr;

    //set edit rect
    edit_rect = base_ctrl_ptr->rect;

    //reset rect
    base_ctrl_ptr->rect = baseedit_ctrl_ptr->original_rect;

    //get im rect
    if ((0 != baseedit_ctrl_ptr->im_ctrl_handle) &&
        (GUIIM_GetImRect (baseedit_ctrl_ptr->im_ctrl_handle, &im_rect)))
    {
        // 输入法给的区域，有可能im_rect.top == im_rect.bottom，对于输入法，它认为
        // 是无效区域，但给form后，会计算这个区域，导致错误
        if (GUI_IsInvalidRect(im_rect))
        {
            SCI_MEMSET(&im_rect, 0, sizeof(im_rect));
        }

        if (BASEEDIT_IsParentCtrl(baseedit_ctrl_ptr))
        {
            //im rect is not in edit
            CTRLMSG_SendNotifyEx (base_ctrl_ptr->handle, MSG_NOTIFY_RESIZE, &im_rect);
        }
        else
        {
            //modify edit rect
            if ((im_rect.top > baseedit_ctrl_ptr->original_rect.top) &&
                (im_rect.top <= baseedit_ctrl_ptr->original_rect.bottom))
            {
                base_ctrl_ptr->rect.bottom = (int16) (im_rect.top - 1);
            }
        }
    }

    //edit is change
    if (!GUI_EqualRect (edit_rect, base_ctrl_ptr->rect))
    {
        // virtual method to handle rect changed
        VTLBASEEDIT_OnRectChanged (baseedit_ctrl_ptr);

        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : init baseedit class
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseEditCtrlInitVtbl (
    CTRLBASEEDIT_VTBL_T        *baseedit_vtbl_ptr
)
{
    CTRLBASE_VTBL_T  *base_ctrl_vtbl_ptr = (CTRLBASE_VTBL_T*) baseedit_vtbl_ptr;

    if (PNULL == base_ctrl_vtbl_ptr)
    {
        return FALSE;
    }

    base_ctrl_vtbl_ptr->HandleEvent = BaseEditCtrlHandleMsg;
    base_ctrl_vtbl_ptr->SetRect = BaseEditCtrlSetRect;
    base_ctrl_vtbl_ptr->SetBorder = BaseEditCtrlSetBorder;
    base_ctrl_vtbl_ptr->SetBg = BaseEditCtrlSetBg;
    base_ctrl_vtbl_ptr->SetFont = BaseEditCtrlSetFont;
    base_ctrl_vtbl_ptr->GetFont = BaseEditCtrlGetFont;

    // 以下接口用于实现内嵌form
    base_ctrl_vtbl_ptr->GetHeightByWidth = BaseEditCtrlGetHeightByWidth;
    base_ctrl_vtbl_ptr->SetDisplayRect = BaseEditCtrlSetDisplayRect;
    base_ctrl_vtbl_ptr->SetLeftRightLoop = BaseEditCtrlSetLeftRightLoop;
    base_ctrl_vtbl_ptr->SetUpDownLoop = BaseEditCtrlSetUpDownLoop;
    base_ctrl_vtbl_ptr->GetSelectRect = BaseEditCtrlGetSelectRect;
    base_ctrl_vtbl_ptr->GetSelectIndex = BaseEditCtrlGetSelectIndex;
    base_ctrl_vtbl_ptr->SetSelectIndex = BaseEditCtrlSetSelectIndex;

    // 本类的虚方法
    baseedit_vtbl_ptr->SetCursorPos = BaseEditCtrlSetCursorPos;
    baseedit_vtbl_ptr->SetHighlight = BaseEditCtrlSetHighlight;
    baseedit_vtbl_ptr->GetDispStr = BaseEditCtrlGetDispStr;

    return TRUE;
}

/*****************************************************************************/
//  Description : baseedit construct
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseEditCtrlConstruct (
    CTRLBASE_OBJ_T              *base_ctrl_ptr,
    CTRLBASEEDIT_INIT_PARAM_T   *baseedit_param_ptr
)
{
    BOOLEAN             result = TRUE;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*) base_ctrl_ptr;

    if (PNULL == base_ctrl_ptr || PNULL == baseedit_param_ptr)
    {
        return FALSE;
    }

    //set edit style
    baseedit_ctrl_ptr->style = GUIEDIT_STYLE_MULTI;

    // TODO: this should not be here
    //set edit display direction
    baseedit_ctrl_ptr->display_dir = GUIEDIT_DISPLAY_DIR_LT;

    //set edit rect is adaptive
    baseedit_ctrl_ptr->is_rect_adaptive = baseedit_param_ptr->is_rect_adaptive;

    //set edit rect and type
    baseedit_ctrl_ptr->type          = baseedit_param_ptr->type;
    baseedit_ctrl_ptr->original_rect = base_ctrl_ptr->both_rect.v_rect;

    //set edit get active create im
    baseedit_ctrl_ptr->is_create_im_active = TRUE;

    //set edit permit display border
    baseedit_ctrl_ptr->is_permit_border = TRUE;

    //set string max len
	if (GUIEDIT_TYPE_PHONENUM == baseedit_ctrl_ptr->type)
	{
		baseedit_ctrl_ptr->str_max_len = (uint16)MAX(baseedit_param_ptr->str_max_len + 1,1);
	}
	else
	{
		baseedit_ctrl_ptr->str_max_len = (uint16)MAX(baseedit_param_ptr->str_max_len,1);
	}

    //set cursor
    baseedit_ctrl_ptr->cursor.x_coordinate = GUIEDIT_CURSOR_INVALID_COORDINATE;

    //set no highlight
    BASEEDIT_SetNoHighlight (baseedit_ctrl_ptr, baseedit_ctrl_ptr->cursor.cur_pos);

    //set line
    baseedit_ctrl_ptr->total_line_num = 1;

    //get edit theme
    MMITHEME_GetEditTheme (&baseedit_ctrl_ptr->common_theme);
    SCI_ASSERT (baseedit_ctrl_ptr->common_theme.dividing_line_width <= baseedit_ctrl_ptr->common_theme.line_space); /*assert verified*/
    SCI_ASSERT (GUIEDIT_CURSOR_DEFAULT_WIDTH <= baseedit_ctrl_ptr->common_theme.margin_lr_space); /*assert verified*/

    base_ctrl_ptr->lcd_dev_info = baseedit_ctrl_ptr->common_theme.lcd_dev;

    return (result);
}

/*****************************************************************************/
//  Description : baseedit destruct
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseEditCtrlDestruct (
    CTRLBASE_OBJ_T    *base_ctrl_ptr
)
{
    BOOLEAN                 result = TRUE;
    CTRLBASEEDIT_OBJ_T      *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*) base_ctrl_ptr;
    GUIEDIT_CONFIG_KEY_T    *im_config_ptr = PNULL;
    GUIEDIT_CONFIG_KEY_T    *im_config_next_ptr = PNULL;

    if (PNULL == baseedit_ctrl_ptr)
    {
        return FALSE;
    }

    //free edit string memory
    if (PNULL != baseedit_ctrl_ptr->str_ptr)
    {
        SCI_FREE (baseedit_ctrl_ptr->str_ptr);
    }

    //free line info memory
    if (PNULL != baseedit_ctrl_ptr->line_info_ptr)
    {
        SCI_FREE (baseedit_ctrl_ptr->line_info_ptr);
    }

    //free im config key memory
    im_config_ptr = baseedit_ctrl_ptr->im_config_key_ptr;

    while (PNULL != im_config_ptr)
    {
        im_config_next_ptr = im_config_ptr->next_ptr;

        //free
        SCI_FREE (im_config_ptr->value_ptr);
        SCI_FREE (im_config_ptr);

        im_config_ptr = im_config_next_ptr;
    }

    baseedit_ctrl_ptr->im_config_key_ptr = PNULL;

    //free im control memory
    if (0 != baseedit_ctrl_ptr->im_ctrl_handle)
    {
        baseedit_ctrl_ptr->im_ctrl_handle = 0;
    }

    return (result);
}

/*****************************************************************************/
//  Description : handle baseedit msg function
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E BaseEditCtrlHandleMsg (
    CTRLBASE_OBJ_T        *base_ctrl_ptr,   //control pointer
    MMI_MESSAGE_ID_E      msg_id,           //message
    DPARAM                param             //add data
)
{
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*) base_ctrl_ptr;
    CTRLBASE_VTBL_T     *parent_vtbl_ptr = PNULL;

    if (PNULL == baseedit_ctrl_ptr)
    {
        return MMI_RESULT_FALSE;
    }

    switch (msg_id)
    {
    case MSG_CTL_OPEN:
        //set edit handle right softkey long msg
        BASEEDIT_SetSoftkeyBtnState (baseedit_ctrl_ptr, 2, FALSE, TRUE);
        break;

    case MSG_CTL_LOSE_FOCUS:
    case MSG_CTL_GET_FOCUS:
        // 过滤此消息，不让父类处理
        break;

    case MSG_CTL_GET_ACTIVE:
        base_ctrl_ptr->is_active = TRUE;

        //landscape edit for PDA
        if ((HandleLandscapeEdit (baseedit_ctrl_ptr, FALSE)) &&
            (baseedit_ctrl_ptr->is_create_im_active))
        {
            //create im
            BASEEDIT_CreateImCtrl (baseedit_ctrl_ptr);
        }

        //notify parent control
        result = CTRLMSG_SendNotify (base_ctrl_ptr->handle, MSG_NOTIFY_GET_ACTIVE);

        //resize edit rect
        BASEEDIT_ResizeRect (baseedit_ctrl_ptr);
        break;

    case MSG_CTL_LOSE_ACTIVE:
        result = HandleBaseEditLoseActive (baseedit_ctrl_ptr, FALSE);
        break;

    case MSG_NOTIFY_LOSE_ACTIVE:
        result = HandleBaseEditLoseActive (baseedit_ctrl_ptr, TRUE);
        break;

    case MSG_CTL_PAINT:
        if (VTLBASE_GetVisible (base_ctrl_ptr))
        {
            VTLBASEEDIT_DisplayAll (
                    baseedit_ctrl_ptr,
                    MMK_IsWinOnPaint(base_ctrl_ptr->win_handle) ? FALSE : TRUE);
        }
        break;

#ifdef GUIF_EDIT_PDASTYLE
    case MSG_LCD_SWITCH:
        if (MMK_IsWindowLandscape (base_ctrl_ptr->win_handle))
        {
            BASEEDIT_DestroyImCtrl (baseedit_ctrl_ptr);
        }
#if defined(KEYPAD_TYPE_TYPICAL_KEYPAD) || defined(KEYPAD_TYPE_QWERTY_KEYPAD)
        else if (baseedit_ctrl_ptr->is_create_im_active)
        {
            BASEEDIT_CreateImCtrl (baseedit_ctrl_ptr);
        }

#endif
        break;
#endif

    case MSG_APP_1:
    case MSG_KEYUP_1:
    case MSG_APP_2:
    case MSG_KEYUP_2:
    case MSG_APP_3:
    case MSG_KEYUP_3:
    case MSG_APP_4:
    case MSG_KEYUP_4:
    case MSG_APP_5:
    case MSG_KEYUP_5:
    case MSG_APP_6:
    case MSG_KEYUP_6:
    case MSG_APP_7:
    case MSG_KEYUP_7:
    case MSG_APP_8:
    case MSG_KEYUP_8:
    case MSG_APP_9:
    case MSG_KEYUP_9:
    case MSG_APP_STAR:
    case MSG_KEYUP_STAR:
    case MSG_APP_0:
    case MSG_KEYUP_0:
    case MSG_APP_HASH:
    case MSG_KEYUP_HASH:
#if defined(KEYPAD_TYPE_QWERTY_KEYPAD)
    case MSG_APP_Q:
    case MSG_APP_W:
    case MSG_APP_E:
    case MSG_APP_R:
    case MSG_APP_T:
    case MSG_APP_Y:
    case MSG_APP_U:
    case MSG_APP_I:
    case MSG_APP_O:
    case MSG_APP_P:
    case MSG_APP_A:
    case MSG_APP_S:
    case MSG_APP_D:
    case MSG_APP_F:
    case MSG_APP_G:
    case MSG_APP_H:
    case MSG_APP_J:
    case MSG_APP_K:
    case MSG_APP_L:
    case MSG_APP_DEL:
    case MSG_APP_Z:
    case MSG_APP_X:
    case MSG_APP_C:
    case MSG_APP_V:
    case MSG_APP_B:
    case MSG_APP_N:
    case MSG_APP_M:
    case MSG_APP_COMMA:
    case MSG_APP_PERIOD:
    case MSG_APP_ENTER:
    case MSG_APP_FN:
    case MSG_APP_SHIFT:
    case MSG_APP_AT_QWERTY:
    case MSG_APP_SPACE:
    case MSG_APP_AND:
    case MSG_APP_QUESTION:
    case MSG_APP_CTRL:
    case MSG_APP_PLUS:
    case MSG_APP_LPARENTHESIS:
    case MSG_APP_RPARENTHESIS:
    case MSG_APP_MINUS:
    case MSG_APP_DQUOTES:
    case MSG_APP_SEMICOLON:
    case MSG_APP_COLON:
    case MSG_APP_SLASH:
    case MSG_APP_SAND:
    case MSG_APP_EXCLAMATION:
#endif
        if (0 == baseedit_ctrl_ptr->im_ctrl_handle &&
            !MMK_IsWindowLandscape (base_ctrl_ptr->win_handle))
        {
            BASEEDIT_CreateImCtrl (baseedit_ctrl_ptr);
        }

        if (0 != baseedit_ctrl_ptr->im_ctrl_handle)
        {
            result = GUIIM_HandleMsg (baseedit_ctrl_ptr->im_ctrl_handle, msg_id, PNULL);
        }
        break;

    case MSG_APP_OK:
    case MSG_APP_WEB:
        result = BASEEDIT_HandleSoftkeyMsg (baseedit_ctrl_ptr, msg_id, param);
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_UP:
        if ((!MMK_GET_TP_SLIDE (param)) &&
            (base_ctrl_ptr->is_active))
        {
            GUIIM_INPUT_TYPE_T default_input = {0};

            //landscape edit for PDA
            if ((0 == baseedit_ctrl_ptr->im_ctrl_handle) &&
                HandleLandscapeEdit (baseedit_ctrl_ptr, TRUE))
            {
                BASEEDIT_CreateImCtrl (baseedit_ctrl_ptr);
                BASEEDIT_HandleImSwitch (baseedit_ctrl_ptr);
            }

            //非触笔输入、可切换至触笔输入->切换至触笔输入
            if (GUIIM_CanSwithToTp (baseedit_ctrl_ptr->im_ctrl_handle, &default_input))
            {
                GUIIM_SetInput (baseedit_ctrl_ptr->im_ctrl_handle, default_input);
            }
        }
        break;
#endif

    case MSG_NOTIFY_IM_PAINT:
        //update window
        MMK_SendMsg (base_ctrl_ptr->win_handle, MSG_FULL_PAINT, PNULL);
        break;

    case MSG_NOTIFY_IM_SWICTH:
    case MSG_NOTIFY_IM_CHANGE_RECT:
        BASEEDIT_HandleImSwitch (baseedit_ctrl_ptr);
        break;

    case MSG_NOTIFY_IM_HIDE:
        BASEEDIT_DestroyImCtrl (baseedit_ctrl_ptr);

        if (! (baseedit_ctrl_ptr->is_rect_adaptive))
        {
            BASEEDIT_HandleImSwitch (baseedit_ctrl_ptr);
        }

//#ifdef GUIF_CLIPBOARD
//        else
//        {
//            // fix NEWMS00174274
//            BASEFLEX_ShowSlider(edit_ctrl_ptr);
//        }
//#endif
        break;

    case MSG_NOTIFY_IM_REFRESH_SOFTKEY:
        BASEEDIT_DisplaySoftkey (baseedit_ctrl_ptr);
        break;

    default:
        parent_vtbl_ptr = (CTRLBASE_VTBL_T*) TYPEMNG_GetParentVtbl (CTRL_BASEEDIT_TYPE, (OBJECT_TYPE_PTR) base_ctrl_ptr);
        result = parent_vtbl_ptr->HandleEvent (base_ctrl_ptr, msg_id, param);
        break;
    }

    return (result);
}

/*****************************************************************************/
//  Description : set edit rect
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseEditCtrlSetRect (
    CTRLBASE_OBJ_T    *base_ctrl_ptr,
    const CAF_RECT_T  *rect_ptr
)
{
    BOOLEAN             result = TRUE;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T *) base_ctrl_ptr;

    if ((!GUI_IsRectEmpty (*rect_ptr)) &&
        (!GUI_EqualRect (base_ctrl_ptr->rect, *rect_ptr)))
    {
        //set rect
        base_ctrl_ptr->rect          = *rect_ptr;
        base_ctrl_ptr->display_rect  = *rect_ptr;
        baseedit_ctrl_ptr->original_rect = *rect_ptr;

        // fix NEWMS00197533
        baseedit_ctrl_ptr->cursor.is_rect_valid = FALSE;

        GUI_SetVOrHRect (base_ctrl_ptr->handle, (GUI_RECT_T *) rect_ptr, & (base_ctrl_ptr->both_rect));

        // virtual method for rect changed
        VTLBASEEDIT_OnRectChanged (baseedit_ctrl_ptr);
    }

    return (result);
}

/*****************************************************************************/
//  Description : set edit border
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseEditCtrlSetBorder (
    CTRLBASE_OBJ_T  *base_ctrl_ptr,
    GUI_BORDER_T    *border_ptr
)
{
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T *) base_ctrl_ptr;

    baseedit_ctrl_ptr->common_theme.border = *border_ptr;

    return TRUE;
}

/*****************************************************************************/
//  Description : set edit bg
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseEditCtrlSetBg (
    CTRLBASE_OBJ_T      *base_ctrl_ptr,
    GUI_BG_T            *bg_ptr
)
{
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T *) base_ctrl_ptr;

    baseedit_ctrl_ptr->common_theme.bg = *bg_ptr;

    return TRUE;
}

/*****************************************************************************/
//  Description : set edit font
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseEditCtrlSetFont (
    CTRLBASE_OBJ_T    *base_ctrl_ptr,
    GUI_FONT_ALL_T    *font_ptr
)
{
    BOOLEAN             result = TRUE;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T *) base_ctrl_ptr;

    // font color
    baseedit_ctrl_ptr->common_theme.font.color = font_ptr->color;

    // font size
    if (baseedit_ctrl_ptr->common_theme.font.font != font_ptr->font)
    {
        baseedit_ctrl_ptr->common_theme.font.font = font_ptr->font;

        //notify edit change
        BASEEDIT_NotifyLineMsg (baseedit_ctrl_ptr, 0);
    }

    return (result);
}

/*****************************************************************************/
//  Description : get edit font
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseEditCtrlGetFont (
    CTRLBASE_OBJ_T    *base_ctrl_ptr,
    GUI_FONT_ALL_T    *font_ptr
)
{
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T *) base_ctrl_ptr;

    *font_ptr = baseedit_ctrl_ptr->common_theme.font;

    return TRUE;
}

/*****************************************************************************/
//  Description : set edit is circular handle left/right key msg
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseEditCtrlSetLeftRightLoop (
    CTRLBASE_OBJ_T     *base_ctrl_ptr,
    BOOLEAN            is_circular
)
{
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T *) base_ctrl_ptr;

    baseedit_ctrl_ptr->common_theme.is_cir_handle_lf = is_circular;

    return TRUE;
}

/*****************************************************************************/
// 	Description : set control is circular handle up/down key
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseEditCtrlSetUpDownLoop (
    CTRLBASE_OBJ_T       *base_ctrl_ptr,
    BOOLEAN              is_circular
)
{
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T *) base_ctrl_ptr;

    baseedit_ctrl_ptr->common_theme.is_cir_handle_ud = is_circular;

    return TRUE;
}

/*****************************************************************************/
// 	Description : get control height by width,include content,border etc.
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseEditCtrlGetHeightByWidth (
    CTRLBASE_OBJ_T      *base_ctrl_ptr,  //in:
    uint16              width,          //in:
    uint16              *height_ptr     //in/out:
)
{
    BOOLEAN         result = TRUE;
    BOOLEAN         is_border = TRUE;
    uint16          line_height = 0;
    uint16          edit_width = 0;
    uint16          total_line_num = 0;
    int32           real_height = 0;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T *) base_ctrl_ptr;

    //test whether exist border
    if ((!baseedit_ctrl_ptr->is_permit_border) ||
        (GUI_BORDER_NONE == baseedit_ctrl_ptr->common_theme.border.type))
    {
        is_border = FALSE;
    }

    //get line height
    line_height = BASEEDIT_GetLineHeight(baseedit_ctrl_ptr);

    //get text box width
    edit_width = (uint16) (width - (baseedit_ctrl_ptr->common_theme.margin_lr_space << 1));

    if (is_border)
    {
        edit_width = (uint16) (edit_width - (baseedit_ctrl_ptr->common_theme.border.width << 1));
    }

    //get edit string line number
    total_line_num = VTLBASEEDIT_GetLineNum (baseedit_ctrl_ptr, edit_width);

    //must one line
    if (0 == total_line_num)
    {
        total_line_num = 1;
    }

    //get text width and height
    real_height = (int32) line_height * total_line_num;

    //add margin
    real_height = real_height + (baseedit_ctrl_ptr->common_theme.margin_tb_space << 1);

    //add border
    if (is_border)
    {
        real_height = real_height + (baseedit_ctrl_ptr->common_theme.border.width << 1);
    }

    if (real_height < 0x7fff)
    {
        *height_ptr = (uint16) real_height;
    }
    else
    {
        result = FALSE;
    }

    return (result);
}

/*****************************************************************************/
// 	Description : set ctrl display rect
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseEditCtrlSetDisplayRect (
    CTRLBASE_OBJ_T      *base_ctrl_ptr, //in:
    const GUI_RECT_T    *rect_ptr,      //in:
    BOOLEAN             is_update       //in:
)
{
    BOOLEAN             result = TRUE;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T *) base_ctrl_ptr;

    if (!GUI_EqualRect (base_ctrl_ptr->display_rect, *rect_ptr))
    {
        //set display rect
        base_ctrl_ptr->display_rect = *rect_ptr;

        // fix NEWMS00197533
        baseedit_ctrl_ptr->cursor.is_rect_valid = FALSE;
    }

    if (is_update)
    {
        //update edit
        VTLBASEEDIT_DisplayAll (baseedit_ctrl_ptr, TRUE);
    }

    return (result);
}

/*****************************************************************************/
// 	Description : set control select rect
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseEditCtrlGetSelectRect (
    CTRLBASE_OBJ_T         *base_ctrl_ptr,  //in:
    GUI_RECT_T             *rect_ptr       //in/out:
)
{
    BOOLEAN             result = FALSE;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T *) base_ctrl_ptr;

    if ((base_ctrl_ptr->is_active) && (PNULL != rect_ptr))
    {
        //set cursor rect is invalid
        baseedit_ctrl_ptr->cursor.is_rect_valid = FALSE;

        result = BASEEDIT_GetCursorRect (baseedit_ctrl_ptr, PNULL, rect_ptr);
    }

    return (result);
}

/*****************************************************************************/
// 	Description : set control select index,is the first or last line
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseEditCtrlGetSelectIndex (
    CTRLBASE_OBJ_T        *base_ctrl_ptr,  //in:
    BOOLEAN               *is_first_ptr,  //in/out:
    BOOLEAN               *is_last_ptr    //in/out:
)
{
    BOOLEAN             result = FALSE;
    uint16              cursor_index = 0;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T *) base_ctrl_ptr;

    if (PNULL != baseedit_ctrl_ptr)
    {
        cursor_index = BASEEDIT_GetCursorLineIndex (baseedit_ctrl_ptr, baseedit_ctrl_ptr->cursor.cur_pos);

        if (0 == cursor_index)
        {
            if (PNULL != is_first_ptr)
            {
                *is_first_ptr = TRUE;
            }
        }

        if (cursor_index == (baseedit_ctrl_ptr->total_line_num - 1))
        {
            if (PNULL != is_last_ptr)
            {
                *is_last_ptr = TRUE;
            }
        }

        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
// 	Description : set control select index,is the first or last line
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseEditCtrlSetSelectIndex (
    CTRLBASE_OBJ_T        *base_ctrl_ptr,   //in:
    BOOLEAN               is_first,         //in:
    BOOLEAN               is_last           //in:
)
{
    BOOLEAN             result = FALSE;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T *) base_ctrl_ptr;

    if (is_first)
    {
        BaseEditCtrlSetCursorPos(baseedit_ctrl_ptr,0);
        result = TRUE;
    }
    else if (is_last)
    {
        BaseEditCtrlSetCursorPos(baseedit_ctrl_ptr,baseedit_ctrl_ptr->str_len);
        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : 获取edit的字符串内容
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL wchar* BaseEditCtrlGetDispStr (
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr,
    uint16              start_pos,
    uint16              str_len
)
{
    return baseedit_ctrl_ptr->str_ptr + start_pos;
}

/*****************************************************************************/
//  Description : ext type of
//  Global resource dependence :
//  Author:hua.fang
//  Note: judge control type of
/*****************************************************************************/
LOCAL BOOLEAN BaseEditTypeOf (
    CTRLBASE_OBJ_T     *ctrl_ptr
)
{
    return TYPEMNG_IsTypeOf ( (OBJECT_TYPE_PTR) ctrl_ptr, CTRL_BASEEDIT_TYPE);
}

/*****************************************************************************/
//  Description : handle landscape edit
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleLandscapeEdit (// is create im
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr, //in:
    BOOLEAN             is_tp_up            //in:
)
{
    BOOLEAN  result = TRUE;

#if defined GUIF_EDIT_PDASTYLE && defined MMI_IM_PDA_SUPPORT   //与IM相关，有可能IM 不支持使用PDA 版本的UI
    MMI_CTRL_ID_T           ctrl_id = 0;
    GUIEDIT_CONFIG_KEY_T    *im_config_ptr = PNULL;
    CTRLBASE_OBJ_T          *base_ctrl_ptr = (CTRLBASE_OBJ_T*) baseedit_ctrl_ptr;

    if (MMK_IsWindowLandscape (base_ctrl_ptr->win_handle))
    {
        if (is_tp_up)
        {
            if (MMITHEME_IsLandscapeEdit (base_ctrl_ptr->win_handle))
            {
                result = TRUE;
            }
            else
            {
                result = FALSE;

                //open edit win
                ctrl_id = MMITHEME_OpenLandscapeEdit (baseedit_ctrl_ptr->type, base_ctrl_ptr->handle);

                if (PNULL != ctrl_id)
                {
                    // call virtual method to set landscape edit by style edit
                    VTLBASEEDIT_SetLandscapeEdit (baseedit_ctrl_ptr, ctrl_id);

                    //config im
                    im_config_ptr = baseedit_ctrl_ptr->im_config_key_ptr;

                    while (PNULL != im_config_ptr)
                    {
                        CTRLBASEEDIT_ConfigImCustomKey (
                                ctrl_id,
                                im_config_ptr->im_type,
                                im_config_ptr->custom_key,
                                im_config_ptr->value_ptr,
                                im_config_ptr->value_count);

                        im_config_ptr = im_config_ptr->next_ptr;
                    }
                }
            }
        }
        else
        {
            //not landscape edit and disable im tp
            if ((!MMITHEME_IsLandscapeEdit (base_ctrl_ptr->win_handle)) &&
                (BASEEDIT_GetImTp (baseedit_ctrl_ptr)))
            {
                result = FALSE;
            }
        }
    }

#endif

    return (result);
}

/*****************************************************************************/
//  Description : handle edit lose active msg
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleBaseEditLoseActive (
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr, //in:
    BOOLEAN             is_notify       //in
)
{
    BOOLEAN         is_handle = FALSE;
    MMI_RESULT_E    result = MMI_RESULT_FALSE;
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*) baseedit_ctrl_ptr;

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
        base_ctrl_ptr->is_active = FALSE;

        //destroy im
        BASEEDIT_DestroyImCtrl (baseedit_ctrl_ptr);

        //reset display info bar
        ResetInfoBar (baseedit_ctrl_ptr);

        //reset softkey
        BASEEDIT_ResetSoftkey (baseedit_ctrl_ptr);

        //notify parent control
        result = CTRLMSG_SendNotify (base_ctrl_ptr->handle, MSG_NOTIFY_LOSE_ACTIVE);
    }
    else
    {
        // 当焦点在输入法上，点击edit后焦点落到edit上，此时输入法失去焦点发
        // MSG_NOTIFY_LOSE_ACTIVE，如果这里不将焦点设回到输入法上的话，那后续
        // 按键的处理都是给edit的，状态就不对了。所以，如果是notify_lose_active
        // 消息，且edit控件是有焦点的情况下，将焦点重设回输入法。
        BASEEDIT_SetImActive (baseedit_ctrl_ptr);
    }

    return (result);
}

/*****************************************************************************/
//  Description : reset display information bar,include im icon and remaining characters
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void ResetInfoBar (
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr
)
{
    MMI_IMAGE_ID_T  icon_id = 0;
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*) baseedit_ctrl_ptr;

    //set title icon
    GUIWIN_SetTitleIcon (base_ctrl_ptr->win_handle, icon_id);

    //reset sub string
    GUIWIN_SetTitleSubText (base_ctrl_ptr->win_handle, FALSE, PNULL, 0, FALSE);

    //update title
    GUIWIN_UpdateTitle (base_ctrl_ptr->win_handle);
}

