/*****************************************************************************
** File Name:      ctrlbase.c                                                *
** Author:         hua.fang                                                  *
** Date:           08/01/2011                                                *
** Copyright:      2011 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:                                                              *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 08/2011        hua.fang          Create                                   *
******************************************************************************/

/**--------------------------------------------------------------------------*/
/**                         Include Files                                    */
/**--------------------------------------------------------------------------*/
#include "guicommon.h"
#include "ctrlbase.h"

/**--------------------------------------------------------------------------*/
/**                         MACRO DEFINITION                                 */
/**--------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                          TYPE AND STRUCT                                   */
/*----------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          CONSTANT ARRAY                                   */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*/
/**                         EXTERNAL DECLARE                                 */
/**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*/
/**                         STATIC DEFINITION                                */
/**--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : init base ctrl class
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void BaseCtrlInitVtbl (
    CTRLBASE_VTBL_T        *vtbl_ptr
);

/*****************************************************************************/
//  Description : get base ctrl pointer by control id
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL CTRLBASE_OBJ_T* GetBaseCtrlPtr (
    MMI_HANDLE_T     ctrl_handle
);

/*****************************************************************************/
// 	Description : construct
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN  BaseCtrlConstruct (
    CTRLBASE_OBJ_T         *base_ctrl_ptr,
    CTRLBASE_INIT_PARAM_T  *basectrl_param_ptr
);

/*****************************************************************************/
// 	Description : destruct
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN  BaseCtrlDestruct (
    CTRLBASE_OBJ_T      *base_ctrl_ptr
);

/*****************************************************************************/
// 	Description : handle msg
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E BaseCtrlHandleMsg (
    CTRLBASE_OBJ_T         *base_ctrl_ptr,
    CAF_MESSAGE_ID_E       msg_id,
    void*                  param_ptr
);

/*****************************************************************************/
// 	Description : set control both rect
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseCtrlSetBothRect (
    CTRLBASE_OBJ_T           *base_ctrl_ptr,  //in:
    const GUI_BOTH_RECT_T    *both_rect_ptr  //in:
);

/*****************************************************************************/
// 	Description : get gui control both rect
//	Global resource dependence :
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseCtrlGetBothRect (
    CTRLBASE_OBJ_T   *base_ctrl_ptr,
    GUI_BOTH_RECT_T  *both_rect_ptr
);

/*****************************************************************************/
// 	Description : set gui control rect
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseCtrlSetRect (
    CTRLBASE_OBJ_T       *base_ctrl_ptr,
    const GUI_RECT_T     *rect_ptr
);

/*****************************************************************************/
// 	Description : get gui control rect
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseCtrlGetRect (
    CTRLBASE_OBJ_T       *base_ctrl_ptr,
    GUI_RECT_T           *rect_ptr
);

/*****************************************************************************/
// 	Description : set control display rect
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseCtrlSetDisplayRect (
    CTRLBASE_OBJ_T        *base_ctrl_ptr,  //in:
    const GUI_RECT_T      *rect_ptr,       //in:
    BOOLEAN               is_update        //in:
);

/*****************************************************************************/
// 	Description : get control display rect
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseCtrlGetDisplayRect (
    CTRLBASE_OBJ_T        *base_ctrl_ptr,  //in:
    GUI_RECT_T            *rect_ptr        //out:
);

/*****************************************************************************/
// 	Description : set gui control padding
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseCtrlSetPadding (
    CTRLBASE_OBJ_T       *base_ctrl_ptr,
    const GUI_PADDING_T  *padding_ptr
);

/*****************************************************************************/
// 	Description : set gui control lcd device
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseCtrlSetLcdDevInfo (
    CTRLBASE_OBJ_T            *base_ctrl_ptr,
    const GUI_LCD_DEV_INFO    *lcd_dev_ptr
);

/*****************************************************************************/
// 	Description : get gui control lcd device
//	Global resource dependence :
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseCtrlGetLcdDevInfo (
    CTRLBASE_OBJ_T     *base_ctrl_ptr,
    GUI_LCD_DEV_INFO   *lcd_dev_ptr
);

/*****************************************************************************/
// 	Description : set gui control active
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseCtrlSetActive (
    CTRLBASE_OBJ_T     *base_ctrl_ptr,
    BOOLEAN            is_true
);

/*****************************************************************************/
// 	Description : get active
//	Global resource dependence :
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseCtrlGetActive (
    CTRLBASE_OBJ_T      *base_ctrl_ptr
);

/*****************************************************************************/
// 	Description : set gui control enabled
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseCtrlSetEnabled (
    CTRLBASE_OBJ_T     *base_ctrl_ptr,
    BOOLEAN            is_enabled
);

/*****************************************************************************/
// 	Description : get gui control enabled
//	Global resource dependence :
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseCtrlGetEnabled (
    CTRLBASE_OBJ_T      *base_ctrl_ptr
);

/*****************************************************************************/
// 	Description : set control boder info
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseCtrlSetBorder (
    CTRLBASE_OBJ_T     *base_ctrl_ptr,
    GUI_BORDER_T       *border_ptr
);

/*****************************************************************************/
// 	Description : set control background info
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseCtrlSetBg (
    CTRLBASE_OBJ_T         *base_ctrl_ptr,
    GUI_BG_T               *bg_ptr
);

/*****************************************************************************/
// 	Description : set control font info
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseCtrlSetFont (
    CTRLBASE_OBJ_T       *base_ctrl_ptr,
    GUI_FONT_ALL_T       *font_ptr
);

/*****************************************************************************/
// 	Description : get control font info
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseCtrlGetFont (
    CTRLBASE_OBJ_T       *base_ctrl_ptr,
    GUI_FONT_ALL_T       *font_ptr
);

/*****************************************************************************/
// 	Description : set slide state
//	Global resource dependence :
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseCtrlSetSlideState (
    CTRLBASE_OBJ_T     *base_ctrl_ptr,
    BOOLEAN            is_slide
);

/*****************************************************************************/
// 	Description : get slide state
//	Global resource dependence :
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseCtrlGetSlideState (
    CTRLBASE_OBJ_T     *base_ctrl_ptr
);

/*****************************************************************************/
// 	Description : set active mode
//	Global resource dependence :
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseCtrlSetCanActive (
    CTRLBASE_OBJ_T      *base_ctrl_ptr,
    BOOLEAN             can_active
);

/*****************************************************************************/
// 	Description : get active mode
//	Global resource dependence :
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseCtrlGetCanActive (
    CTRLBASE_OBJ_T      *base_ctrl_ptr
);

/*****************************************************************************/
// 	Description : set whether ctrl can active under kbd mode
//	Global resource dependence :
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseCtrlSetCanKbdActive (
    CTRLBASE_OBJ_T      *base_ctrl_ptr,
    BOOLEAN             can_kbd_active
);

/*****************************************************************************/
// 	Description : get whether ctrl can active under kbd mode
//	Global resource dependence :
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseCtrlGetCanKbdActive (
    CTRLBASE_OBJ_T      *base_ctrl_ptr
);

/*****************************************************************************/
// 	Description : set visible
//	Global resource dependence :
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseCtrlSetVisible (
    CTRLBASE_OBJ_T      *base_ctrl_ptr,
    BOOLEAN             is_visible
);

/*****************************************************************************/
// 	Description : Get visible
//	Global resource dependence :
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseCtrlGetVisible (
    CTRLBASE_OBJ_T      *base_ctrl_ptr
);

/*****************************************************************************/
// 	Description : set kbd enabled
//	Global resource dependence :
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseCtrlSetKbdEnabled (
    CTRLBASE_OBJ_T      *base_ctrl_ptr,
    BOOLEAN             is_kbd_enabled
);

/*****************************************************************************/
// 	Description : Get kbd enabled
//	Global resource dependence :
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseCtrlGetKbdEnabled (
    CTRLBASE_OBJ_T      *base_ctrl_ptr
);

/*****************************************************************************/
// 	Description : set tp enabled
//	Global resource dependence :
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseCtrlSetTpEnabled (
    CTRLBASE_OBJ_T      *base_ctrl_ptr,
    BOOLEAN             is_tp_enabled
);

/*****************************************************************************/
// 	Description : Get tp enabled
//	Global resource dependence :
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseCtrlGetTpEnabled (
    CTRLBASE_OBJ_T      *base_ctrl_ptr
);

/*****************************************************************************/
// 	Description : set tp priority
//	Global resource dependence :
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseCtrlSetTpPriority (
    CTRLBASE_OBJ_T      *base_ctrl_ptr,
    CTRL_TP_PRIORITY_E  tp_priority
);

/*****************************************************************************/
// 	Description : get tp priority
//	Global resource dependence :
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL CTRL_TP_PRIORITY_E BaseCtrlGetTpPriority (
    CTRLBASE_OBJ_T      *base_ctrl_ptr
);

/*****************************************************************************/
// 	Description : set control is display progress
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseCtrlSetScrollbarEnabled (
    CTRLBASE_OBJ_T   *base_ctrl_ptr,
    BOOLEAN          is_display
);

/*****************************************************************************/
// 	Description : set control is circular handle up/down key
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseCtrlSetUpDownLoop (
    CTRLBASE_OBJ_T       *base_ctrl_ptr,
    BOOLEAN              is_circular
);

/*****************************************************************************/
// 	Description : set control is circular handle left/right key
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseCtrlSetLeftRightLoop (
    CTRLBASE_OBJ_T    *base_ctrl_ptr,
    BOOLEAN           is_circular
);

/*****************************************************************************/
// 	Description : get control height by width,include content,border etc.
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseCtrlGetHeightByWidth (
    CTRLBASE_OBJ_T      *base_ctrl_ptr,  //in:
    uint16              width,          //in:
    uint16              *height_ptr     //in/out:
);

/*****************************************************************************/
// 	Description : set control select rect
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseCtrlGetSelectRect (
    CTRLBASE_OBJ_T         *base_ctrl_ptr,  //in:
    GUI_RECT_T             *rect_ptr       //in/out:
);

/*****************************************************************************/
// 	Description : set control select index,is the first or last line
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseCtrlGetSelectIndex (
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
LOCAL BOOLEAN BaseCtrlSetSelectIndex (
    CTRLBASE_OBJ_T        *base_ctrl_ptr,   //in:
    BOOLEAN               is_first,         //in:
    BOOLEAN               is_last           //in:
);


/**--------------------------------------------------------------------------*/
/**                         GLOBAL DEFINITION                                */
/**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : init base ctrl class
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void BaseCtrlInitVtbl (
    CTRLBASE_VTBL_T        *vtbl_ptr
)
{
    vtbl_ptr->HandleEvent = BaseCtrlHandleMsg;
    vtbl_ptr->SetBothRect = BaseCtrlSetBothRect;
    vtbl_ptr->GetBothRect = BaseCtrlGetBothRect;
    vtbl_ptr->SetRect = BaseCtrlSetRect;
    vtbl_ptr->GetRect = BaseCtrlGetRect;
    vtbl_ptr->SetDisplayRect = BaseCtrlSetDisplayRect;
    vtbl_ptr->GetDisplayRect = BaseCtrlGetDisplayRect;
    vtbl_ptr->SetLcdDevInfo = BaseCtrlSetLcdDevInfo;
    vtbl_ptr->GetLcdDevInfo = BaseCtrlGetLcdDevInfo;
    vtbl_ptr->SetBg = BaseCtrlSetBg;
    vtbl_ptr->SetBorder = BaseCtrlSetBorder;
    vtbl_ptr->SetFont = BaseCtrlSetFont;
    vtbl_ptr->GetFont = BaseCtrlGetFont;
    vtbl_ptr->SetSlideState = BaseCtrlSetSlideState;
    vtbl_ptr->GetSlideState = BaseCtrlGetSlideState;
    vtbl_ptr->SetCanActive = BaseCtrlSetCanActive;
    vtbl_ptr->GetCanActive = BaseCtrlGetCanActive;
    vtbl_ptr->SetCanKbdActive = BaseCtrlSetCanKbdActive;
    vtbl_ptr->GetCanKbdActive = BaseCtrlGetCanKbdActive;
    vtbl_ptr->SetPadding = BaseCtrlSetPadding;
    vtbl_ptr->SetActive = BaseCtrlSetActive;
    vtbl_ptr->GetActive = BaseCtrlGetActive;
    vtbl_ptr->SetEnabled = BaseCtrlSetEnabled;
    vtbl_ptr->GetEnabled = BaseCtrlGetEnabled;
    vtbl_ptr->SetVisible = BaseCtrlSetVisible;
    vtbl_ptr->GetVisible = BaseCtrlGetVisible;
    vtbl_ptr->SetKbdEnabled = BaseCtrlSetKbdEnabled;
    vtbl_ptr->GetKbdEnabled = BaseCtrlGetKbdEnabled;
    vtbl_ptr->SetTpEnabled = BaseCtrlSetTpEnabled;
    vtbl_ptr->GetTpEnabled = BaseCtrlGetTpEnabled;
    vtbl_ptr->SetTpPriority = BaseCtrlSetTpPriority;
    vtbl_ptr->GetTpPriority = BaseCtrlGetTpPriority;
    vtbl_ptr->SetScrollbarEnabled = BaseCtrlSetScrollbarEnabled;
    vtbl_ptr->SetUpDownLoop = BaseCtrlSetUpDownLoop;
    vtbl_ptr->SetLeftRightLoop = BaseCtrlSetLeftRightLoop;
    vtbl_ptr->GetHeightByWidth = BaseCtrlGetHeightByWidth;
    vtbl_ptr->GetSelectRect = BaseCtrlGetSelectRect;
    vtbl_ptr->GetSelectIndex = BaseCtrlGetSelectIndex;
    vtbl_ptr->SetSelectIndex = BaseCtrlSetSelectIndex;
}

/*****************************************************************************/
//  Description : get base control type
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC TYPE_TAG_T BASE_GetType(void)
{
    static TYPE_TAG_T    obj_type = 0;

    if (0 == obj_type)
    {
        obj_type = TYPEMNG_Register (
                       0,                                  // it's the root
                       "object",
                       sizeof (CTRLBASE_OBJ_T),
                       (OBJECT_CONSTRUCTOR_FUNC) BaseCtrlConstruct,
                       (OBJECT_DESTRUCTOR_FUNC) BaseCtrlDestruct,
                       sizeof (CTRLBASE_INIT_PARAM_T),
                       PNULL,
                       sizeof (CTRLBASE_VTBL_T),
                       (VTBL_INIT_FUNC) BaseCtrlInitVtbl);
    }

    return obj_type;
}

/*****************************************************************************/
// 	Description : update
//	Global resource dependence :
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC void BASE_Update (
    CTRLBASE_OBJ_T    *base_ctrl_ptr
)
{
    if (PNULL != base_ctrl_ptr)
    {
        if (MMK_IsHandleValid (base_ctrl_ptr->handle))
        {
            MMK_PostMsg (base_ctrl_ptr->handle, MSG_CTL_PAINT, 0, 0);
        }
    }
}

/*****************************************************************************/
// 	Description : get ctrl guid
//	Global resource dependence :
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC CAF_GUID_T BASE_GetCtrlGuid (
    CTRLBASE_OBJ_T    *base_ctrl_ptr
)
{
    if (PNULL != base_ctrl_ptr)
    {
        return base_ctrl_ptr->object.guid;
    }
    else
    {
        return 0;
    }
}

/*****************************************************************************/
// 	Description : set gui control handle
//	Global resource dependence :
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC void BASE_SetCtrlHandle (
    CTRLBASE_OBJ_T    *base_ctrl_ptr,
    CAF_HANDLE_T      ctrl_handle
)
{
    if (PNULL != base_ctrl_ptr)
    {
        base_ctrl_ptr->handle = ctrl_handle;
    }
}

/*****************************************************************************/
// 	Description : get gui control handle
//	Global resource dependence :
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC MMI_HANDLE_T BASE_GetCtrlHandle (
    CTRLBASE_OBJ_T    *base_ctrl_ptr
)
{
    if (PNULL != base_ctrl_ptr)
    {
        return base_ctrl_ptr->handle;
    }
    else
    {
        return 0;
    }
}

/*****************************************************************************/
//  Description : get base ctrl pointer by control id
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL CTRLBASE_OBJ_T* GetBaseCtrlPtr (
    MMI_HANDLE_T     ctrl_handle
)
{
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*) MMK_GetCtrlPtr (ctrl_handle);

    return base_ctrl_ptr;
}

/*****************************************************************************/
// 	Description : construct
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN  BaseCtrlConstruct (
    CTRLBASE_OBJ_T         *base_ctrl_ptr,
    CTRLBASE_INIT_PARAM_T  *basectrl_param_ptr
)
{
    base_ctrl_ptr->both_rect    = basectrl_param_ptr->both_rect;
    base_ctrl_ptr->rect         = basectrl_param_ptr->both_rect.v_rect;
    base_ctrl_ptr->display_rect = basectrl_param_ptr->both_rect.v_rect;

    base_ctrl_ptr->is_enabled = TRUE;
    base_ctrl_ptr->is_visible = TRUE;
    base_ctrl_ptr->is_kbd_enabled = TRUE;
    base_ctrl_ptr->is_tp_enabled = TRUE;
    base_ctrl_ptr->can_active = TRUE;

    return TRUE;
}

/*****************************************************************************/
// 	Description : destruct
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN  BaseCtrlDestruct (
    CTRLBASE_OBJ_T      *base_ctrl_ptr
)
{
    return TRUE;
}

/*****************************************************************************/
// 	Description : handle msg
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E BaseCtrlHandleMsg (
    CTRLBASE_OBJ_T     *base_ctrl_ptr,
    CAF_MESSAGE_ID_E   msg_id,
    void*              param_ptr
)
{
    return MMI_RESULT_FALSE;
}

/*****************************************************************************/
// 	Description : set control both rect
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseCtrlSetBothRect (
    CTRLBASE_OBJ_T           *base_ctrl_ptr,  //in:
    const GUI_BOTH_RECT_T    *both_rect_ptr  //in:
)
{
    BOOLEAN         result = FALSE;

    if ( (PNULL != base_ctrl_ptr) && (PNULL != both_rect_ptr))
    {
        base_ctrl_ptr->both_rect = *both_rect_ptr;
        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
// 	Description : get gui control both rect
//	Global resource dependence :
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseCtrlGetBothRect (
    CTRLBASE_OBJ_T   *base_ctrl_ptr,
    GUI_BOTH_RECT_T  *both_rect_ptr
)
{
    BOOLEAN         result = FALSE;

    if ( (PNULL != base_ctrl_ptr) && (PNULL != both_rect_ptr))
    {
        *both_rect_ptr = base_ctrl_ptr->both_rect;
        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
// 	Description : set gui control rect
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseCtrlSetRect (
    CTRLBASE_OBJ_T       *base_ctrl_ptr,
    const GUI_RECT_T     *rect_ptr
)
{
    BOOLEAN         result = FALSE;

    if ( (PNULL != base_ctrl_ptr) && (PNULL != rect_ptr))
    {
        base_ctrl_ptr->rect = *rect_ptr;
        base_ctrl_ptr->display_rect = *rect_ptr;

        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
// 	Description : get gui control rect
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseCtrlGetRect (
    CTRLBASE_OBJ_T       *base_ctrl_ptr,
    GUI_RECT_T           *rect_ptr
)
{
    BOOLEAN         result = FALSE;

    if ( (PNULL != base_ctrl_ptr) && (PNULL != rect_ptr))
    {
        *rect_ptr = base_ctrl_ptr->rect;

        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
// 	Description : set control display rect
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseCtrlSetDisplayRect (
    CTRLBASE_OBJ_T        *base_ctrl_ptr,  //in:
    const GUI_RECT_T      *rect_ptr,       //in:
    BOOLEAN               is_update        //in:
)
{
    BOOLEAN     result = FALSE;

    if (PNULL != base_ctrl_ptr && PNULL != rect_ptr)
    {
        base_ctrl_ptr->display_rect = *rect_ptr;
        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
// 	Description : get control display rect
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseCtrlGetDisplayRect (
    CTRLBASE_OBJ_T        *base_ctrl_ptr,  //in:
    GUI_RECT_T            *rect_ptr        //out:
)
{
    BOOLEAN     result = FALSE;

    if (PNULL != base_ctrl_ptr && PNULL != rect_ptr)
    {
        *rect_ptr = base_ctrl_ptr->display_rect;
        result = TRUE;
//        result = GUI_IsInvalidRectEx(*rect_ptr);
    }

    return (result);
}

/*****************************************************************************/
// 	Description : set gui control padding
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseCtrlSetPadding (
    CTRLBASE_OBJ_T       *base_ctrl_ptr,
    const GUI_PADDING_T  *padding_ptr
)
{
    BOOLEAN         result = FALSE;

    if ( (PNULL != base_ctrl_ptr) && (PNULL != padding_ptr))
    {
        base_ctrl_ptr->padding = *padding_ptr;

        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
// 	Description : set gui control lcd device
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseCtrlSetLcdDevInfo (
    CTRLBASE_OBJ_T            *base_ctrl_ptr,
    const GUI_LCD_DEV_INFO    *lcd_dev_ptr
)
{
    BOOLEAN result = FALSE;

    if ( (PNULL != base_ctrl_ptr) && (PNULL != lcd_dev_ptr))
    {
        base_ctrl_ptr->lcd_dev_info = *lcd_dev_ptr;
        result = TRUE;
    }

    return result;
}

/*****************************************************************************/
// 	Description : get gui control lcd device
//	Global resource dependence :
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseCtrlGetLcdDevInfo (
    CTRLBASE_OBJ_T     *base_ctrl_ptr,
    GUI_LCD_DEV_INFO   *lcd_dev_ptr
)
{
    BOOLEAN         result = FALSE;

    if ( (PNULL != base_ctrl_ptr) && (PNULL != lcd_dev_ptr))
    {
        *lcd_dev_ptr = base_ctrl_ptr->lcd_dev_info;

        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
// 	Description : set control boder info
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseCtrlSetBorder (
    CTRLBASE_OBJ_T     *base_ctrl_ptr,
    GUI_BORDER_T       *border_ptr
)
{
    BOOLEAN     result = FALSE;

    if (PNULL != border_ptr && PNULL != base_ctrl_ptr)
    {
        base_ctrl_ptr->border = *border_ptr;
        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
// 	Description : set control background info
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseCtrlSetBg (
    CTRLBASE_OBJ_T     *base_ctrl_ptr,
    GUI_BG_T           *bg_ptr
)
{
    BOOLEAN         result = FALSE;

    if ( (PNULL != base_ctrl_ptr) && (PNULL != bg_ptr))
    {
        base_ctrl_ptr->bg = *bg_ptr;
        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
// 	Description : set control font info
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseCtrlSetFont (
    CTRLBASE_OBJ_T       *base_ctrl_ptr,
    GUI_FONT_ALL_T       *font_ptr
)
{
    BOOLEAN     result = FALSE;

    GUI_INVALID_PARAM (base_ctrl_ptr);/*lint !e522*/
    GUI_INVALID_PARAM (font_ptr);/*lint !e522*/

    return (result);
}

/*****************************************************************************/
// 	Description : get control font info
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseCtrlGetFont (
    CTRLBASE_OBJ_T       *base_ctrl_ptr,
    GUI_FONT_ALL_T       *font_ptr
)
{
    BOOLEAN     result = FALSE;

    GUI_INVALID_PARAM (base_ctrl_ptr);/*lint !e522*/
    GUI_INVALID_PARAM (font_ptr);/*lint !e522*/

    return (result);
}

/*****************************************************************************/
// 	Description : set slide state
//	Global resource dependence :
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseCtrlSetSlideState (
    CTRLBASE_OBJ_T     *base_ctrl_ptr,
    BOOLEAN            is_slide
)
{
    BOOLEAN result = FALSE;

    if (PNULL != base_ctrl_ptr)
    {
        base_ctrl_ptr->is_slide = is_slide;
        result = TRUE;
    }

    return TRUE;
}

/*****************************************************************************/
// 	Description : get slide state
//	Global resource dependence :
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseCtrlGetSlideState (
    CTRLBASE_OBJ_T     *base_ctrl_ptr
)
{
    if (PNULL != base_ctrl_ptr)
    {
        return base_ctrl_ptr->is_slide;
    }
    else
    {
        return FALSE;
    }
}

/*****************************************************************************/
// 	Description : set whether ctrl can active
//	Global resource dependence :
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseCtrlSetCanActive (
    CTRLBASE_OBJ_T      *base_ctrl_ptr,
    BOOLEAN             can_active
)
{
    BOOLEAN result = FALSE;

    if (PNULL != base_ctrl_ptr)
    {
        base_ctrl_ptr->can_active = can_active;
        result = TRUE;
    }

    return result;
}

/*****************************************************************************/
// 	Description : get whether ctrl can active
//	Global resource dependence :
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseCtrlGetCanActive (
    CTRLBASE_OBJ_T      *base_ctrl_ptr
)
{
    if (PNULL != base_ctrl_ptr)
    {
        return base_ctrl_ptr->can_active;
    }
    else
    {
        return 0;
    }
}

/*****************************************************************************/
// 	Description : set whether ctrl can active under kbd mode
//	Global resource dependence :
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseCtrlSetCanKbdActive (
    CTRLBASE_OBJ_T      *base_ctrl_ptr,
    BOOLEAN             can_kbd_active
)
{
    BOOLEAN result = FALSE;

    if (PNULL != base_ctrl_ptr)
    {
        base_ctrl_ptr->can_kbd_active = can_kbd_active;
        result = TRUE;
    }

    return result;
}

/*****************************************************************************/
// 	Description : get whether ctrl can active under kbd mode
//	Global resource dependence :
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseCtrlGetCanKbdActive (
    CTRLBASE_OBJ_T      *base_ctrl_ptr
)
{
    if (PNULL != base_ctrl_ptr)
    {
        return base_ctrl_ptr->can_kbd_active;
    }
    else
    {
        return 0;
    }
}

/*****************************************************************************/
// 	Description : set gui control active
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseCtrlSetActive (
    CTRLBASE_OBJ_T     *base_ctrl_ptr,
    BOOLEAN            is_true
)
{
    BOOLEAN         result = FALSE;
    MMI_HANDLE_T    ctrl_handle = 0;

    if (PNULL != base_ctrl_ptr && MMK_IsHandleValid (base_ctrl_ptr->handle))
    {
        if (is_true)
        {
            result = MMK_SetActiveCtrl (base_ctrl_ptr->handle, TRUE);
        }
        else
        {
            ctrl_handle = MMK_ConvertIdToHandle (base_ctrl_ptr->handle);
            result = MMK_WinInactiveCtrl (MMK_GetWinHandleByCtrl (ctrl_handle), FALSE);
        }
    }

    return (result);
}

/*****************************************************************************/
// 	Description : get active
//	Global resource dependence :
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseCtrlGetActive (
    CTRLBASE_OBJ_T      *base_ctrl_ptr
)
{
    BOOLEAN         result = FALSE;
    MMI_HANDLE_T    ctrl_handle = 0;

    if (PNULL != base_ctrl_ptr)
    {
        //convert handle
        ctrl_handle = MMK_ConvertIdToHandle (base_ctrl_ptr->handle);

        //is active
        result = MMK_IsActiveCtrl (ctrl_handle);
    }

    return (result);
}

/*****************************************************************************/
// 	Description : set gui control enabled
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseCtrlSetEnabled (
    CTRLBASE_OBJ_T     *base_ctrl_ptr,
    BOOLEAN            is_enabled
)
{
    BOOLEAN         result = FALSE;

    if (PNULL != base_ctrl_ptr)
    {
        base_ctrl_ptr->is_enabled = is_enabled;
        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
// 	Description : get gui control enabled
//	Global resource dependence :
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseCtrlGetEnabled (
    CTRLBASE_OBJ_T      *base_ctrl_ptr
)
{
    if (PNULL != base_ctrl_ptr)
    {
        return base_ctrl_ptr->is_enabled;
    }
    else
    {
        return FALSE;
    }
}

/*****************************************************************************/
// 	Description : set visible
//	Global resource dependence :
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseCtrlSetVisible (
    CTRLBASE_OBJ_T      *base_ctrl_ptr,
    BOOLEAN             is_visible
)
{
    BOOLEAN result = FALSE;

    if (PNULL != base_ctrl_ptr)
    {
        if (base_ctrl_ptr->is_visible != is_visible)
        {
            base_ctrl_ptr->is_visible = is_visible;
            MMK_CheckActive (base_ctrl_ptr->handle, (BOOLEAN)!is_visible);
        }

        result = TRUE;
    }

    return result;
}

/*****************************************************************************/
// 	Description : Get visible
//	Global resource dependence :
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseCtrlGetVisible (
    CTRLBASE_OBJ_T      *base_ctrl_ptr
)
{
    if (PNULL != base_ctrl_ptr)
    {
        return base_ctrl_ptr->is_visible;
    }
    else
    {
        return FALSE;
    }
}

/*****************************************************************************/
// 	Description : set kbd enabled
//	Global resource dependence :
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseCtrlSetKbdEnabled (
    CTRLBASE_OBJ_T      *base_ctrl_ptr,
    BOOLEAN             is_kbd_enabled
)
{
    BOOLEAN result = FALSE;

    if (PNULL != base_ctrl_ptr)
    {
        base_ctrl_ptr->is_kbd_enabled = is_kbd_enabled;
        result = TRUE;
    }

    return result;
}

/*****************************************************************************/
// 	Description : Get kbd enabled
//	Global resource dependence :
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseCtrlGetKbdEnabled (
    CTRLBASE_OBJ_T      *base_ctrl_ptr
)
{
    if (PNULL != base_ctrl_ptr)
    {
        return base_ctrl_ptr->is_kbd_enabled;
    }
    else
    {
        return FALSE;
    }
}

/*****************************************************************************/
// 	Description : set tp enabled
//	Global resource dependence :
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseCtrlSetTpEnabled (
    CTRLBASE_OBJ_T      *base_ctrl_ptr,
    BOOLEAN             is_tp_enabled
)
{
    BOOLEAN result = FALSE;

    if (PNULL != base_ctrl_ptr)
    {
        base_ctrl_ptr->is_tp_enabled = is_tp_enabled;
        result = TRUE;
    }

    return result;
}

/*****************************************************************************/
// 	Description : Get tp enabled
//	Global resource dependence :
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseCtrlGetTpEnabled (
    CTRLBASE_OBJ_T      *base_ctrl_ptr
)
{
    if (PNULL != base_ctrl_ptr)
    {
        return base_ctrl_ptr->is_tp_enabled;
    }
    else
    {
        return FALSE;
    }
}

/*****************************************************************************/
// 	Description : set tp priority
//	Global resource dependence :
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseCtrlSetTpPriority (
    CTRLBASE_OBJ_T      *base_ctrl_ptr,
    CTRL_TP_PRIORITY_E  tp_priority
)
{
    BOOLEAN result = FALSE;

    if (PNULL != base_ctrl_ptr)
    {
        base_ctrl_ptr->tp_priority = tp_priority;
        result = TRUE;
    }

    return result;
}

/*****************************************************************************/
// 	Description : get tp priority
//	Global resource dependence :
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL CTRL_TP_PRIORITY_E BaseCtrlGetTpPriority (
    CTRLBASE_OBJ_T      *base_ctrl_ptr
)
{
    if (PNULL != base_ctrl_ptr)
    {
        return base_ctrl_ptr->tp_priority;
    }
    else
    {
        return 0;
    }
}

/*****************************************************************************/
// 	Description : set control is display progress
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseCtrlSetScrollbarEnabled (
    CTRLBASE_OBJ_T       *base_ctrl_ptr,
    BOOLEAN              is_display
)
{
    BOOLEAN     result = FALSE;

    GUI_INVALID_PARAM (base_ctrl_ptr);/*lint !e522*/
    GUI_INVALID_PARAM (is_display);/*lint !e522*/

    return (result);
}

/*****************************************************************************/
// 	Description : set control is circular handle up/down key
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseCtrlSetUpDownLoop (
    CTRLBASE_OBJ_T       *base_ctrl_ptr,
    BOOLEAN              is_circular
)
{
    BOOLEAN     result = FALSE;

    GUI_INVALID_PARAM (base_ctrl_ptr);/*lint !e522*/
    GUI_INVALID_PARAM (is_circular);/*lint !e522*/

    return (result);
}

/*****************************************************************************/
// 	Description : set control is circular handle left/right key
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseCtrlSetLeftRightLoop (
    CTRLBASE_OBJ_T    *base_ctrl_ptr,
    BOOLEAN           is_circular
)
{
    BOOLEAN     result = FALSE;

    GUI_INVALID_PARAM (base_ctrl_ptr);/*lint !e522*/
    GUI_INVALID_PARAM (is_circular);/*lint !e522*/

    return (result);
}

/*****************************************************************************/
// 	Description : get control height by width,include content,border etc.
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseCtrlGetHeightByWidth (
    CTRLBASE_OBJ_T      *base_ctrl_ptr,  //in:
    uint16              width,          //in:
    uint16              *height_ptr     //in/out:
)
{
    BOOLEAN     result = FALSE;

    GUI_INVALID_PARAM (base_ctrl_ptr);/*lint !e522*/
    GUI_INVALID_PARAM (width);/*lint !e522*/
    GUI_INVALID_PARAM (height_ptr);/*lint !e522*/

    return (result);
}

/*****************************************************************************/
// 	Description : set control select rect
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseCtrlGetSelectRect (
    CTRLBASE_OBJ_T         *base_ctrl_ptr,  //in:
    GUI_RECT_T             *rect_ptr       //in/out:
)
{
    BOOLEAN     result = FALSE;

    GUI_INVALID_PARAM (base_ctrl_ptr);/*lint !e522*/
    GUI_INVALID_PARAM (rect_ptr);/*lint !e522*/

    return (result);
}

/*****************************************************************************/
// 	Description : set control select index,is the first or last line
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseCtrlGetSelectIndex (
    CTRLBASE_OBJ_T        *base_ctrl_ptr,  //in:
    BOOLEAN               *is_first_ptr,  //in/out:
    BOOLEAN               *is_last_ptr    //in/out:
)
{
    BOOLEAN     result = FALSE;

    GUI_INVALID_PARAM (base_ctrl_ptr);/*lint !e522*/
    GUI_INVALID_PARAM (is_first_ptr);/*lint !e522*/
    GUI_INVALID_PARAM (is_last_ptr);/*lint !e522*/

    return (result);
}

/*****************************************************************************/
// 	Description : set control select index,is the first or last line
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseCtrlSetSelectIndex (
    CTRLBASE_OBJ_T        *base_ctrl_ptr,   //in:
    BOOLEAN               is_first,         //in:
    BOOLEAN               is_last           //in:
)
{
    BOOLEAN     result = FALSE;

    GUI_INVALID_PARAM (base_ctrl_ptr);/*lint !e522*/
    GUI_INVALID_PARAM (is_first);/*lint !e522*/
    GUI_INVALID_PARAM (is_last);/*lint !e522*/

    return (result);
}

/*****************************************************************************/
// 	Description : update control and all its children
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
PUBLIC void CTRLBASE_Update (
    MMI_HANDLE_T   ctrl_handle
)
{
    CTRLBASE_OBJ_T  *base_ctrl_ptr = GetBaseCtrlPtr (ctrl_handle);

    if (PNULL != base_ctrl_ptr)
    {
        BASE_Update (base_ctrl_ptr);
    }
}

/*****************************************************************************/
// 	Description : get both rect
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBASE_GetBothRect (
    MMI_HANDLE_T        ctrl_handle,
    GUI_BOTH_RECT_T     *both_rect_ptr
)
{
    BOOLEAN         result = FALSE;
    CTRLBASE_OBJ_T  *base_ctrl_ptr = GetBaseCtrlPtr (ctrl_handle);

    if (PNULL != both_rect_ptr && PNULL != base_ctrl_ptr)
    {
        result = VTLBASE_GetBothRect (base_ctrl_ptr, both_rect_ptr);
    }

    return result;
}

/*****************************************************************************/
// 	Description : set both rect
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBASE_SetBothRect (
    MMI_HANDLE_T            ctrl_handle,
    const GUI_BOTH_RECT_T   *both_rect_ptr
)
{
    BOOLEAN         result = FALSE;
    CTRLBASE_OBJ_T  *base_ctrl_ptr = GetBaseCtrlPtr (ctrl_handle);

    if (PNULL != both_rect_ptr && PNULL != base_ctrl_ptr)
    {
        result = VTLBASE_SetBothRect (base_ctrl_ptr, both_rect_ptr);
    }

    return result;
}

/*****************************************************************************/
// 	Description : get rect
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBASE_GetRect (
    MMI_HANDLE_T            ctrl_handle,
    GUI_RECT_T              *rect_ptr
)
{
    BOOLEAN         result = FALSE;
    CTRLBASE_OBJ_T  *base_ctrl_ptr = GetBaseCtrlPtr (ctrl_handle);

    if (PNULL != rect_ptr && PNULL != base_ctrl_ptr)
    {
        result = VTLBASE_GetRect (base_ctrl_ptr, rect_ptr);
    }

    return result;
}

/*****************************************************************************/
// 	Description : set rect
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBASE_SetRect (
    MMI_HANDLE_T            ctrl_handle,
    const GUI_RECT_T        *rect_ptr
)
{
    BOOLEAN         result = FALSE;
    CTRLBASE_OBJ_T  *base_ctrl_ptr = GetBaseCtrlPtr (ctrl_handle);

    if (PNULL != rect_ptr && PNULL != base_ctrl_ptr)
    {
        result = VTLBASE_SetRect (base_ctrl_ptr, rect_ptr);
    }

    return result;
}

/*****************************************************************************/
// 	Description : get lcd dev info
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBASE_GetLcdDevInfo (
    MMI_HANDLE_T              ctrl_handle,
    GUI_LCD_DEV_INFO          *lcd_dev_ptr
)
{
    BOOLEAN         result = FALSE;
    CTRLBASE_OBJ_T  *base_ctrl_ptr = GetBaseCtrlPtr (ctrl_handle);

    if (PNULL != lcd_dev_ptr && PNULL != base_ctrl_ptr)
    {
        result = VTLBASE_GetLcdDevInfo (base_ctrl_ptr, lcd_dev_ptr);
    }

    return result;
}

/*****************************************************************************/
// 	Description : set lcd dev info
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBASE_SetLcdDevInfo (
    MMI_HANDLE_T                 ctrl_handle,
    const GUI_LCD_DEV_INFO       *lcd_dev_ptr
)
{
    BOOLEAN         result = FALSE;
    CTRLBASE_OBJ_T  *base_ctrl_ptr = GetBaseCtrlPtr (ctrl_handle);

    if (PNULL != lcd_dev_ptr && PNULL != base_ctrl_ptr)
    {
        result = VTLBASE_SetLcdDevInfo (base_ctrl_ptr, lcd_dev_ptr);
    }

    return result;
}

/*****************************************************************************/
// 	Description : get bg
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBASE_GetBg (
    MMI_HANDLE_T          ctrl_handle,
    GUI_BG_T              *bg_ptr
)
{
    BOOLEAN         result = FALSE;
    CTRLBASE_OBJ_T  *base_ctrl_ptr = GetBaseCtrlPtr (ctrl_handle);

    if ( (PNULL != bg_ptr) && (PNULL != base_ctrl_ptr))
    {
        *bg_ptr = base_ctrl_ptr->bg;
        result = TRUE;
    }

    return result;
}

/*****************************************************************************/
// 	Description : set bg
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBASE_SetBg (
    MMI_HANDLE_T          ctrl_handle,
    GUI_BG_T              *bg_ptr
)
{
    BOOLEAN         result = FALSE;
    CTRLBASE_OBJ_T  *base_ctrl_ptr = GetBaseCtrlPtr (ctrl_handle);

    if (PNULL != bg_ptr && PNULL != base_ctrl_ptr)
    {
        result = VTLBASE_SetBg (base_ctrl_ptr, bg_ptr);
    }

    return result;
}

/*****************************************************************************/
// 	Description : get border
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBASE_GetBorder (
    MMI_HANDLE_T      ctrl_handle,
    GUI_BORDER_T      *border_ptr
)
{
    BOOLEAN         result = FALSE;
    CTRLBASE_OBJ_T  *base_ctrl_ptr = GetBaseCtrlPtr (ctrl_handle);

    if ( (PNULL != border_ptr) && (PNULL != base_ctrl_ptr))
    {
        *border_ptr = base_ctrl_ptr->border;
        result = TRUE;
    }

    return result;
}

/*****************************************************************************/
// 	Description : set border
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBASE_SetBorder (
    MMI_HANDLE_T      ctrl_handle,
    GUI_BORDER_T      *border_ptr
)
{
    BOOLEAN         result = FALSE;
    CTRLBASE_OBJ_T  *base_ctrl_ptr = GetBaseCtrlPtr (ctrl_handle);

    if (PNULL != border_ptr && PNULL != base_ctrl_ptr)
    {
        result = VTLBASE_SetBorder (base_ctrl_ptr, border_ptr);
    }

    return result;
}

/*****************************************************************************/
// 	Description : get padding
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBASE_GetPadding (
    MMI_HANDLE_T      ctrl_handle,
    GUI_PADDING_T     *padding_ptr
)
{
    BOOLEAN         result = FALSE;
    CTRLBASE_OBJ_T  *base_ctrl_ptr = GetBaseCtrlPtr (ctrl_handle);

    if ( (PNULL != padding_ptr) && (PNULL != base_ctrl_ptr))
    {
        *padding_ptr = base_ctrl_ptr->padding;
        result = TRUE;
    }

    return result;
}

/*****************************************************************************/
// 	Description : set padding
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBASE_SetPadding (
    MMI_HANDLE_T      ctrl_handle,
    GUI_PADDING_T     *padding_ptr
)
{
    BOOLEAN         result = FALSE;
    CTRLBASE_OBJ_T  *base_ctrl_ptr = GetBaseCtrlPtr (ctrl_handle);

    if (PNULL != padding_ptr && PNULL != base_ctrl_ptr)
    {
        result = VTLBASE_SetPadding (base_ctrl_ptr, padding_ptr);
    }

    return result;
}

/*****************************************************************************/
// 	Description : set whether ctrl can active
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBASE_SetCanActive (
    MMI_HANDLE_T            ctrl_handle,
    BOOLEAN                 can_active
)
{
    BOOLEAN         result = FALSE;
    CTRLBASE_OBJ_T  *base_ctrl_ptr = GetBaseCtrlPtr (ctrl_handle);

    if (PNULL != base_ctrl_ptr)
    {
        result = VTLBASE_SetCanActive (base_ctrl_ptr, can_active);
    }

    return result;
}

/*****************************************************************************/
// 	Description : set whether ctrl can active under kbd mode
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBASE_SetCanKbdActive (
    MMI_HANDLE_T            ctrl_handle,
    BOOLEAN                 can_kbd_active
)
{
    BOOLEAN         result = FALSE;
    CTRLBASE_OBJ_T  *base_ctrl_ptr = GetBaseCtrlPtr (ctrl_handle);

    if (PNULL != base_ctrl_ptr)
    {
        result = VTLBASE_SetCanKbdActive (base_ctrl_ptr, can_kbd_active);
    }

    return result;
}

/*****************************************************************************/
// 	Description : get active
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBASE_GetActive (
    MMI_HANDLE_T      ctrl_handle
)
{
    BOOLEAN         result = FALSE;
    CTRLBASE_OBJ_T  *base_ctrl_ptr = GetBaseCtrlPtr (ctrl_handle);

    if (PNULL != base_ctrl_ptr)
    {
        result = VTLBASE_GetActive (base_ctrl_ptr);
    }

    return result;
}

/*****************************************************************************/
// 	Description : set active
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBASE_SetActive (
    MMI_HANDLE_T      ctrl_handle
)
{
    BOOLEAN         result = FALSE;
    CTRLBASE_OBJ_T  *base_ctrl_ptr = GetBaseCtrlPtr (ctrl_handle);

    if (PNULL != base_ctrl_ptr)
    {
        result = VTLBASE_SetActive (base_ctrl_ptr, TRUE);
    }

    return result;
}

/*****************************************************************************/
// 	Description : get visible
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBASE_GetVisible (
    MMI_HANDLE_T      ctrl_handle
)
{
    BOOLEAN         result = FALSE;
    CTRLBASE_OBJ_T  *base_ctrl_ptr = GetBaseCtrlPtr (ctrl_handle);

    if (PNULL != base_ctrl_ptr)
    {
        result = VTLBASE_GetVisible (base_ctrl_ptr);
    }

    return result;
}

/*****************************************************************************/
// 	Description : set visible
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBASE_SetVisible (
    MMI_HANDLE_T      ctrl_handle,
    BOOLEAN           is_visible
)
{
    BOOLEAN         result = FALSE;
    CTRLBASE_OBJ_T  *base_ctrl_ptr = GetBaseCtrlPtr (ctrl_handle);

    if (PNULL != base_ctrl_ptr)
    {
        result = VTLBASE_SetVisible (base_ctrl_ptr, is_visible);
    }

    return result;
}

/*****************************************************************************/
// 	Description : get enabled
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBASE_GetEnabled (
    MMI_HANDLE_T      ctrl_handle
)
{
    BOOLEAN         result = FALSE;
    CTRLBASE_OBJ_T  *base_ctrl_ptr = GetBaseCtrlPtr (ctrl_handle);

    if (PNULL != base_ctrl_ptr)
    {
        result = VTLBASE_GetEnabled (base_ctrl_ptr);
    }

    return result;
}

/*****************************************************************************/
// 	Description : set enabled
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBASE_SetEnabled (
    MMI_HANDLE_T      ctrl_handle,
    BOOLEAN           is_enabled
)
{
    BOOLEAN         result = FALSE;
    CTRLBASE_OBJ_T  *base_ctrl_ptr = GetBaseCtrlPtr (ctrl_handle);

    if (PNULL != base_ctrl_ptr)
    {
        result = VTLBASE_SetEnabled (base_ctrl_ptr, is_enabled);
    }

    return result;
}

/*****************************************************************************/
// 	Description : set tp enabled
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBASE_SetTpEnabled (
    MMI_HANDLE_T      ctrl_handle,
    BOOLEAN           is_tp_enabled
)
{
    BOOLEAN         result = FALSE;
    CTRLBASE_OBJ_T  *base_ctrl_ptr = GetBaseCtrlPtr (ctrl_handle);

    if (PNULL != base_ctrl_ptr)
    {
        result = VTLBASE_SetTpEnabled (base_ctrl_ptr, is_tp_enabled);
    }

    return result;
}

/*****************************************************************************/
// 	Description : set kbd enabled
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBASE_SetKbdEnabled (
    MMI_HANDLE_T      ctrl_handle,
    BOOLEAN           is_kbd_enabled
)
{
    BOOLEAN         result = FALSE;
    CTRLBASE_OBJ_T  *base_ctrl_ptr = GetBaseCtrlPtr (ctrl_handle);

    if (PNULL != base_ctrl_ptr)
    {
        result = VTLBASE_SetKbdEnabled (base_ctrl_ptr, is_kbd_enabled);
    }

    return result;
}

/*****************************************************************************/
// 	Description : get user data
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
PUBLIC uint32 CTRLBASE_GetUserData (
    MMI_HANDLE_T      ctrl_handle
)
{
    uint32          user_data = FALSE;
    CTRLBASE_OBJ_T  *base_ctrl_ptr = GetBaseCtrlPtr (ctrl_handle);

    if (PNULL != base_ctrl_ptr)
    {
        user_data = base_ctrl_ptr->user_data;
    }

    return user_data;
}

/*****************************************************************************/
// 	Description : set user data
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBASE_SetUserData (
    MMI_HANDLE_T      ctrl_handle,
    uint32            user_data
)
{
    BOOLEAN         result = FALSE;
    CTRLBASE_OBJ_T  *base_ctrl_ptr = GetBaseCtrlPtr (ctrl_handle);

    if (PNULL != base_ctrl_ptr)
    {
        base_ctrl_ptr->user_data = user_data;
        result = TRUE;
    }

    return result;
}

/*****************************************************************************/
// 	Description : get control display rect
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN BaseCtrlGetDisplayRectNotImp (
    CTRLBASE_OBJ_T        *base_ctrl_ptr,  //in:
    GUI_RECT_T            *rect_ptr        //out:
)
{
    // 有些控件没有内嵌到form里，不会被设置display_rect，这个方法被用在这些控件里
    return FALSE;
}

