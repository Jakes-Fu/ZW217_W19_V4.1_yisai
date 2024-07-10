/*****************************************************************************
** File Name:      ctrlownerdraw.c                                            *
** Author:                                                                   *
** Date:           07/23/2012                                                *
** Copyright:      2004 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 07/2012     xiyuan.am             Creat
******************************************************************************/

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmk_app.h"
#include "mmi_theme.h"

#include "ctrlownerdraw.h"
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
 *                         TYPE AND CONSTANT                                 *
 *---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DEFINITION                              *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                                                                                 */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : init ownerdraw class
//  Global resource dependence :
//  Author: xiyuan.ma
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN OwnerDrawInitVtbl (
    CTRLOWNERDRAW_VTBL_T *owndraw_vtbl_ptr
);

/*****************************************************************************/
//  Description : ownerdraw construct
//  Global resource dependence :
//  Author: xiyuan.ma
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN OwnerDrawConstruct (
    CTRLBASE_OBJ_T *obj_ptr,
    void *param_ptr
);

/*****************************************************************************/
//  Description : ownerdraw destruct
//  Global resource dependence :
//  Author: xiyuan.ma
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN OwnerDrawDestruct (
    CTRLBASE_OBJ_T *obj_ptr
);

/*****************************************************************************/
//  Description : handle owner draw msg function
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E OwnerDrawHandleMsg (
    CTRLBASE_OBJ_T *obj_ptr, //control pointer
    MMI_MESSAGE_ID_E msg_id, //message
    DPARAM param //add data
);

/*****************************************************************************/
//  Description : display owner draw control
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void OwnerDrawDraw (
    CTRLOWNERDRAW_OBJ_T *owndraw_ctrl_ptr
);

/*****************************************************************************/
//  Description : display text background
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void OwnerDrawDrawBg (
    GUI_RECT_T bg_rect,
    CTRLOWNERDRAW_OBJ_T *owndraw_ctrl_ptr
);

#ifdef TOUCH_PANEL_SUPPORT
/*****************************************************************************/
//  Description : handle owner draw tp up message
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleOwnDrawTpUp (
    CTRLOWNERDRAW_OBJ_T *owndraw_ctrl_ptr,
    DPARAM param
);
#endif

/*****************************************************************************/
//  Description : get owner draw rect,except border
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL GUI_RECT_T OwnDrawGetRect (
    CTRLOWNERDRAW_OBJ_T *owndraw_ctrl_ptr
);

/*****************************************************************************/
//  Description : set owner draw rect
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN OwnDrawSetRect (
    CTRLBASE_OBJ_T *ctrl_ptr,
    const CAF_RECT_T   *rect_ptr
);

/*****************************************************************************/
//  Description : set owner draw control border
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN OwnDrawSetBorder (
    CTRLBASE_OBJ_T       *ctrl_ptr,
    GUI_BORDER_T     *border_ptr
);

/*****************************************************************************/
//  Description : set owner draw control bg
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN OwnDrawSetBg (
    CTRLBASE_OBJ_T   *ctrl_ptr,
    GUI_BG_T     *bg_ptr
);

/*****************************************************************************/
//  Description : get owner draw height by width,include border etc
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN OwnDrawGetHeightByWidth (
    CTRLBASE_OBJ_T    *ctrl_ptr,  //in:
    uint16        width,      //in:
    uint16        *height_ptr //in/out:
);

/*****************************************************************************/
//  Description : get owner draw display rect
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN OwnDrawGetDisplayRect (
    CTRLBASE_OBJ_T      *ctrl_ptr,  //in:
    GUI_RECT_T      *rect_ptr   //in/out:
);

/*****************************************************************************/
//  Description : set  owner draw display rect
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN OwnDrawSetDisplayRect (
    CTRLBASE_OBJ_T          *ctrl_ptr,  //in:
    const GUI_RECT_T    *rect_ptr,  //in:
    BOOLEAN             is_update   //in:
);

/*****************************************************************************/
//  Description : get owner draw pointer by control id
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL CTRLOWNERDRAW_OBJ_T* GetOwnDrawPtr (
    MMI_HANDLE_T     ctrl_handle
);

/*****************************************************************************/
//  Description : ext type of
//  Global resource dependence :
//  Author:Jassmine
//  Note: judge control type of
/*****************************************************************************/
LOCAL BOOLEAN OwnDrawTypeOf (
    CTRLBASE_OBJ_T  *ctrl_ptr
);

/*****************************************************************************/
//  Description : convert init param
//  Global resource dependence :
//  Author: xiyuan.ma
//  Note:
/*****************************************************************************/
LOCAL void OwnerDrawCtrlPackInitParam (
    GUIOWNDRAW_INIT_DATA_T *ownerdraw_init_ptr, //in
    CTRLOWNERDRAW_INIT_PARAM_T *ownerdraw_param_ptr //out
);

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                                                                                       *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : get ownerdraw type
//  Global resource dependence :
//  Author: xiyuan.ma
//  Note:
/*****************************************************************************/
PUBLIC TYPE_TAG_T CTRLOWNERDRAW_GetType()
{
    static TYPE_TAG_T obj_type = 0;

    if (0 == obj_type)
    {
        obj_type = TYPEMNG_Register (
                       CTRL_BASE_TYPE,
                       "ownerdraw",
                       sizeof (CTRLOWNERDRAW_OBJ_T),
                       (OBJECT_CONSTRUCTOR_FUNC) OwnerDrawConstruct,
                       (OBJECT_DESTRUCTOR_FUNC) OwnerDrawDestruct,
                       sizeof(CTRLOWNERDRAW_INIT_PARAM_T),
                       (PARAM_PACKING_FUNC)OwnerDrawCtrlPackInitParam,
                       sizeof (CTRLOWNERDRAW_VTBL_T),
                       (VTBL_INIT_FUNC) OwnerDrawInitVtbl);
    }

    return obj_type;
}

/*****************************************************************************/
//  Description : init ownerdraw class
//  Global resource dependence :
//  Author: xiyuan.ma
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN OwnerDrawInitVtbl (
    CTRLOWNERDRAW_VTBL_T *ownerdraw_vtbl_ptr
)
{
    CTRLBASE_VTBL_T *base_ctrl_vtbl_ptr = PNULL;

    base_ctrl_vtbl_ptr = (CTRLBASE_VTBL_T*) ownerdraw_vtbl_ptr;

    if (PNULL == base_ctrl_vtbl_ptr)
    {
        return FALSE;
    }

    base_ctrl_vtbl_ptr->HandleEvent = OwnerDrawHandleMsg;
    base_ctrl_vtbl_ptr->SetRect = OwnDrawSetRect;
    base_ctrl_vtbl_ptr->SetBorder = OwnDrawSetBorder;
    base_ctrl_vtbl_ptr->SetBg = OwnDrawSetBg;
    base_ctrl_vtbl_ptr->GetHeightByWidth = OwnDrawGetHeightByWidth;
    base_ctrl_vtbl_ptr->GetDisplayRect = OwnDrawGetDisplayRect;
    base_ctrl_vtbl_ptr->SetDisplayRect = OwnDrawSetDisplayRect;

    return TRUE;
}

/*****************************************************************************/
//  Description : convert init param
//  Global resource dependence :
//  Author: xiyuan.ma
//  Note:
/*****************************************************************************/
LOCAL void OwnerDrawCtrlPackInitParam (
    GUIOWNDRAW_INIT_DATA_T *ownerdraw_init_ptr, //in
    CTRLOWNERDRAW_INIT_PARAM_T *ownerdraw_param_ptr //out
)
{
    CTRLBASE_INIT_PARAM_T   *base_ctrl_param_ptr = (CTRLBASE_INIT_PARAM_T*)ownerdraw_param_ptr;

    if ( (PNULL == ownerdraw_init_ptr) || (PNULL == ownerdraw_param_ptr))
    {
        return;
    }

    base_ctrl_param_ptr->both_rect = ownerdraw_init_ptr->both_rect;

    ownerdraw_param_ptr->OwnDrawFunc = ownerdraw_init_ptr->OwnDrawFunc;
}

/*****************************************************************************/
//  Description : owner draw control construct
//  Global resource dependence :
//  Author: xiyuan.ma
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN OwnerDrawConstruct (
    CTRLBASE_OBJ_T *obj_ptr,
    void *param_ptr
)
{
    BOOLEAN result = TRUE;
    CTRLOWNERDRAW_OBJ_T *ownerdraw_ptr = PNULL;
    CTRLOWNERDRAW_INIT_PARAM_T *ownerdraw_param_ptr = PNULL;

    ownerdraw_ptr = (CTRLOWNERDRAW_OBJ_T*) obj_ptr;
    ownerdraw_param_ptr = (CTRLOWNERDRAW_INIT_PARAM_T*) param_ptr;

    if (PNULL == ownerdraw_ptr || PNULL == ownerdraw_param_ptr)
    {
        return FALSE;
    }

    //set permit display border
    ownerdraw_ptr->is_permit_border = TRUE;

    //default height
    ownerdraw_ptr->height = MMITHEME_GetOwnDrawDefaultHeight();

    //set call back
    ownerdraw_ptr->OwnDrawFunc = ownerdraw_param_ptr->OwnDrawFunc;

    //set win handle
    obj_ptr->win_handle = MMK_GetWinHandleByCtrl (obj_ptr->handle);

    return result;
}

/*****************************************************************************/
//  Description : owner draw control destruct
//  Global resource dependence :
//  Author: xiyuan.ma
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN OwnerDrawDestruct (
    CTRLBASE_OBJ_T *obj_ptr
)
{
    return TRUE;
}

/*****************************************************************************/
//  Description : handle owner draw msg function
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E OwnerDrawHandleMsg (
    CTRLBASE_OBJ_T *obj_ptr, //control pointer
    MMI_MESSAGE_ID_E msg_id, //message
    DPARAM param //add data
)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    CTRLOWNERDRAW_OBJ_T *owndraw_ctrl_ptr = PNULL;
#ifdef TOUCH_PANEL_SUPPORT //xiyuan edit for pclint
    GUI_POINT_T             tp_point = {0};
#endif

    if (PNULL == obj_ptr)
    {
        return MMI_RESULT_FALSE;
    }

    owndraw_ctrl_ptr = (CTRLOWNERDRAW_OBJ_T*) obj_ptr;

    switch (msg_id)
    {
    case MSG_CTL_PAINT:
        OwnerDrawDraw (owndraw_ctrl_ptr);
        break;

    case MSG_CTL_GET_ACTIVE:
        //notify parent control
        result = CTRLMSG_SendNotify (obj_ptr->handle, MSG_NOTIFY_GET_ACTIVE);
        break;

    case MSG_CTL_LOSE_ACTIVE:
        //notify parent control
        result = CTRLMSG_SendNotify (obj_ptr->handle, MSG_NOTIFY_LOSE_ACTIVE);
        break;

    case MSG_CTL_GET_FOCUS:

        if (PNULL != owndraw_ctrl_ptr->MsgCallback.get_focus)
        {
            result = owndraw_ctrl_ptr->MsgCallback.get_focus (obj_ptr->handle);
        }
        else
        {
            result = MMI_RESULT_FALSE;
        }

        break;

    case MSG_CTL_LOSE_FOCUS:

        if (PNULL != owndraw_ctrl_ptr->MsgCallback.lose_focus)
        {
            result = owndraw_ctrl_ptr->MsgCallback.lose_focus (obj_ptr->handle);
        }
        else
        {
            result = MMI_RESULT_FALSE;
        }

        break;

    case MSG_APP_OK:
        //notify parent control
        result = CTRLMSG_PostNotify (obj_ptr->handle, MSG_NOTIFY_OK);
        break;

    case MSG_APP_WEB:
        //notify parent control
        result = CTRLMSG_PostNotify (obj_ptr->handle, MSG_NOTIFY_MIDSK);
        break;

    case MSG_APP_CANCEL:
        //notify parent control
        result = CTRLMSG_PostNotify (obj_ptr->handle, MSG_NOTIFY_CANCEL);
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_DOWN:

        if (PNULL != owndraw_ctrl_ptr->MsgCallback.tp_down)
        {
            tp_point.x = MMK_GET_TP_X (param);
            tp_point.y = MMK_GET_TP_Y (param);

            result = owndraw_ctrl_ptr->MsgCallback.tp_down (obj_ptr->handle, tp_point);
        }
        else
        {
            result = MMI_RESULT_FALSE;
        }

        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_SHORT:

        if (PNULL != owndraw_ctrl_ptr->MsgCallback.tp_short)
        {
            tp_point.x = MMK_GET_TP_X (param);
            tp_point.y = MMK_GET_TP_Y (param);

            result = owndraw_ctrl_ptr->MsgCallback.tp_short (obj_ptr->handle, tp_point);
        }
        else
        {
            result = MMI_RESULT_FALSE;
        }

        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_MOVE:

        if (PNULL != owndraw_ctrl_ptr->MsgCallback.tp_move)
        {
            tp_point.x = MMK_GET_TP_X (param);
            tp_point.y = MMK_GET_TP_Y (param);

            result = owndraw_ctrl_ptr->MsgCallback.tp_move (obj_ptr->handle, tp_point);
        }
        else
        {
            result = MMI_RESULT_FALSE;
        }

        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_LONG:

        if (PNULL != owndraw_ctrl_ptr->MsgCallback.tp_long)
        {
            tp_point.x = MMK_GET_TP_X (param);
            tp_point.y = MMK_GET_TP_Y (param);

            result = owndraw_ctrl_ptr->MsgCallback.tp_long (obj_ptr->handle, tp_point);
        }
        else
        {
            result = MMI_RESULT_FALSE;
        }

        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_UP:

        if (PNULL != owndraw_ctrl_ptr->MsgCallback.tp_up)
        {
            tp_point.x = MMK_GET_TP_X (param);
            tp_point.y = MMK_GET_TP_Y (param);

            result = owndraw_ctrl_ptr->MsgCallback.tp_up (obj_ptr->handle, tp_point);
        }
        else
        {
            result = HandleOwnDrawTpUp (owndraw_ctrl_ptr, param);
        }

        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

    default:
        result = MMI_RESULT_FALSE;
        break;
    }

    return (result);
}

/*****************************************************************************/
//  Description : display owner draw control
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void OwnerDrawDraw (
    CTRLOWNERDRAW_OBJ_T *owndraw_ctrl_ptr
)
{
    GUI_RECT_T cross_rect = {0};
    GUIOWNDRAW_INFO_T owner_draw = {0};
    CTRLBASE_OBJ_T *base_obj_ptr = (CTRLBASE_OBJ_T*) owndraw_ctrl_ptr;

    if ( (VTLBASE_GetVisible(base_obj_ptr)) &&
            GUI_IntersectRect (&cross_rect, base_obj_ptr->display_rect, base_obj_ptr->rect))
    {
        //display bg
        OwnerDrawDrawBg (base_obj_ptr->display_rect, owndraw_ctrl_ptr);

        if (owndraw_ctrl_ptr->is_permit_border)
        {
            //display border
            GUI_DisplayBorder (base_obj_ptr->rect,
                               base_obj_ptr->display_rect,
                               &base_obj_ptr->border,
                               &base_obj_ptr->lcd_dev_info);
        }

        //call back
        if (PNULL != owndraw_ctrl_ptr->OwnDrawFunc)
        {
            //set control rect,except border
            owner_draw.ctrl_rect = OwnDrawGetRect (owndraw_ctrl_ptr);

            if (GUI_IntersectRect (&cross_rect, base_obj_ptr->display_rect, owner_draw.ctrl_rect))
            {
                //set output param
                owner_draw.display_rect = cross_rect;
                owner_draw.ctrl_handle  = base_obj_ptr->handle;
                owner_draw.win_handle   = base_obj_ptr->win_handle;

                //call back
                owndraw_ctrl_ptr->OwnDrawFunc (&owner_draw);
            }
        }
    }
}

/*****************************************************************************/
//  Description : display text background
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void OwnerDrawDrawBg (
    GUI_RECT_T bg_rect,
    CTRLOWNERDRAW_OBJ_T *owndraw_ctrl_ptr
)
{
    GUI_RECT_T          img_rect = {0};
    GUI_BG_DISPLAY_T    bg_display = {0};
    CTRLBASE_OBJ_T *base_obj_ptr = (CTRLBASE_OBJ_T*) owndraw_ctrl_ptr;

    //set image rect
    img_rect.left   = (int16) (bg_rect.left - base_obj_ptr->rect.left);
    img_rect.top    = (int16) (bg_rect.top - base_obj_ptr->rect.top);
    img_rect.right  = (int16) (img_rect.left + bg_rect.right - bg_rect.left);
    img_rect.bottom = (int16) (img_rect.top + bg_rect.bottom - bg_rect.top);

    //set bg display info
    bg_display.is_transparent = FALSE;
    bg_display.rect           = base_obj_ptr->rect;
    bg_display.display_rect   = bg_rect;
    bg_display.img_rect       = img_rect;
    bg_display.ctrl_handle    = base_obj_ptr->handle;
    bg_display.win_handle     = base_obj_ptr->win_handle;

    //display bg
    GUI_DisplayBg (&base_obj_ptr->bg,
                   &bg_display,
                   &base_obj_ptr->lcd_dev_info);
}

#ifdef TOUCH_PANEL_SUPPORT
/*****************************************************************************/
//  Description : handle owner draw tp up message
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleOwnDrawTpUp (
    CTRLOWNERDRAW_OBJ_T *owndraw_ctrl_ptr,
    DPARAM             param
)
{
    MMI_RESULT_E    result = MMI_RESULT_FALSE;
    GUI_RECT_T      cross_rect = {0};
    GUI_POINT_T     tp_point = {0};

    //not slide
    if (!MMK_GET_TP_SLIDE (param))
    {
        //set tp press point
        tp_point.x = MMK_GET_TP_X (param);
        tp_point.y = MMK_GET_TP_Y (param);

        if ( (GUI_IntersectRect (&cross_rect, owndraw_ctrl_ptr->base_ctrl.display_rect, owndraw_ctrl_ptr->base_ctrl.rect)) &&
                (GUI_PointIsInRect (tp_point, cross_rect)))
        {
            //notify parent control
            CTRLMSG_PostNotify (owndraw_ctrl_ptr->base_ctrl.handle, MSG_NOTIFY_PENOK);
        }
    }

    return (result);
}
#endif

/*****************************************************************************/
//  Description : get owner draw rect,except border
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL GUI_RECT_T OwnDrawGetRect (
    CTRLOWNERDRAW_OBJ_T *owndraw_ctrl_ptr
)
{
    BOOLEAN         is_border = TRUE;
    GUI_RECT_T      owndraw_rect = {0};
    CTRLBASE_OBJ_T *base_obj_ptr = (CTRLBASE_OBJ_T*) owndraw_ctrl_ptr;

    //owner draw is exist border
    if ( (!owndraw_ctrl_ptr->is_permit_border) ||
            (GUI_BORDER_NONE == base_obj_ptr->border.type))
    {
        is_border = FALSE;
    }

    if (is_border)
    {
        owndraw_rect.left   = (int16) (base_obj_ptr->rect.left + base_obj_ptr->border.width);
        owndraw_rect.right  = (int16) (base_obj_ptr->rect.right - base_obj_ptr->border.width);
        owndraw_rect.top    = (int16) (base_obj_ptr->rect.top + base_obj_ptr->border.width);
        owndraw_rect.bottom = (int16) (base_obj_ptr->rect.bottom - base_obj_ptr->border.width);
    }
    else
    {
        owndraw_rect = base_obj_ptr->rect;
    }

    return (owndraw_rect);
}

/*****************************************************************************/
//  Description : set owner draw rect
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN OwnDrawSetRect (
    CTRLBASE_OBJ_T *obj_ptr,
    const CAF_RECT_T *rect_ptr
)
{
    BOOLEAN             result = TRUE;

    if (!GUI_EqualRect (obj_ptr->rect, *rect_ptr))
    {
        obj_ptr->rect         = *rect_ptr;
        obj_ptr->display_rect = *rect_ptr;

        GUI_SetVOrHRect (obj_ptr->handle, (GUI_RECT_T *) rect_ptr, & (obj_ptr->both_rect));
    }

    return (result);
}

/*****************************************************************************/
//  Description : set owner draw control border
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN OwnDrawSetBorder (
    CTRLBASE_OBJ_T       *ctrl_ptr,
    GUI_BORDER_T     *border_ptr
)
{
    BOOLEAN result = TRUE;

    if ( (PNULL == ctrl_ptr) || (PNULL == border_ptr))
    {
        return FALSE;
    }

    //set border
    ctrl_ptr->border = *border_ptr;

    return result;
}

/*****************************************************************************/
//  Description : set owner draw control bg
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN OwnDrawSetBg (
    CTRLBASE_OBJ_T   *ctrl_ptr,
    GUI_BG_T     *bg_ptr
)
{
    BOOLEAN             result = TRUE;

    if ( (PNULL == ctrl_ptr) || (PNULL == bg_ptr))
    {
        return FALSE;
    }

    //set bg
    ctrl_ptr->bg = *bg_ptr;

    return result;
}

/*****************************************************************************/
//  Description : get owner draw height by width,include border etc
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN OwnDrawGetHeightByWidth (
    CTRLBASE_OBJ_T    *ctrl_ptr,  //in:
    uint16        width,      //in:
    uint16        *height_ptr //in/out:
)
{
    BOOLEAN result = TRUE;
    BOOLEAN is_border = TRUE;
    CTRLOWNERDRAW_OBJ_T *owndraw_ctrl_ptr = PNULL;
    CTRLBASE_OBJ_T *base_obj_ptr = PNULL;

    owndraw_ctrl_ptr = (CTRLOWNERDRAW_OBJ_T *) ctrl_ptr;
    base_obj_ptr = (CTRLBASE_OBJ_T*) owndraw_ctrl_ptr;

    if (0 < owndraw_ctrl_ptr->height)
    {
        //owner draw is exist border
        if ( (!owndraw_ctrl_ptr->is_permit_border) ||
                (GUI_BORDER_NONE == base_obj_ptr->border.type))
        {
            is_border = FALSE;
        }

        *height_ptr = owndraw_ctrl_ptr->height;

        //add border
        if (is_border)
        {
            *height_ptr = (uint16) (*height_ptr + (base_obj_ptr->border.width << 1));
        }

        result = TRUE;
    }

    GUI_INVALID_PARAM (width);/*lint !e522*/

    return (result);
}

/*****************************************************************************/
//  Description : get owner draw display rect
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN OwnDrawGetDisplayRect (
    CTRLBASE_OBJ_T *ctrl_ptr,  //in:
    GUI_RECT_T *rect_ptr   //in/out:
)
{
    if ( PNULL == ctrl_ptr || PNULL == rect_ptr)
    {
        return FALSE;
    }

    *rect_ptr = ctrl_ptr->display_rect;

    return TRUE;
}

/*****************************************************************************/
//  Description : set  owner draw display rect
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN OwnDrawSetDisplayRect (
    CTRLBASE_OBJ_T  *ctrl_ptr,  //in:
    const GUI_RECT_T    *rect_ptr,  //in:
    BOOLEAN             is_update   //in:
)
{
    BOOLEAN result = TRUE;
    CTRLOWNERDRAW_OBJ_T *owndraw_ctrl_ptr = PNULL;
    owndraw_ctrl_ptr = (CTRLOWNERDRAW_OBJ_T *) ctrl_ptr;

    if ( (PNULL == ctrl_ptr) || (PNULL == rect_ptr))
    {
        return FALSE;
    }

    //set display rect
    ctrl_ptr->display_rect = *rect_ptr;

    if (is_update)
    {
        //update owner draw
        OwnerDrawDraw (owndraw_ctrl_ptr);
    }

    return (result);
}

/*****************************************************************************/
//  Description : get owner draw pointer by control id
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL CTRLOWNERDRAW_OBJ_T* GetOwnDrawPtr (
    MMI_HANDLE_T     ctrl_handle
)
{
    CTRLBASE_OBJ_T *ctrl_ptr = (CTRLBASE_OBJ_T*)MMK_GetCtrlPtr (ctrl_handle);

    if ( (PNULL == ctrl_ptr) || (!OwnDrawTypeOf (ctrl_ptr)))
    {
        return PNULL;
    }

    return (CTRLOWNERDRAW_OBJ_T*) ctrl_ptr;
}

/*****************************************************************************/
//  Description : ext type of
//  Global resource dependence :
//  Author:Jassmine
//  Note: judge control type of
/*****************************************************************************/
LOCAL BOOLEAN OwnDrawTypeOf (
    CTRLBASE_OBJ_T *ctrl_ptr
)
{
    return TYPEMNG_IsTypeOf(ctrl_ptr, CTRL_OWNERDRAW_TYPE);
}

/*****************************************************************************/
//  Description : 设置TP处理函数
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void CTRLOWNERDRAW_SetCallback (
    MMI_CTRL_ID_T   ctrl_id,//in
    GUIOWNDRAW_TP_HANDLER_T *tp_handler_ptr
)
{
    CTRLOWNERDRAW_OBJ_T *owndraw_ctrl_ptr = PNULL;

    //get owner draw pointer by control id
    owndraw_ctrl_ptr = GetOwnDrawPtr (ctrl_id);

    if (PNULL != owndraw_ctrl_ptr && PNULL != tp_handler_ptr)
    {
        owndraw_ctrl_ptr->MsgCallback = *tp_handler_ptr;
    }
}

/*****************************************************************************/
//  Description : 设置用户数据
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void CTRLOWNERDRAW_SetAddData (
    MMI_CTRL_ID_T   ctrl_id,//in
    ADD_DATA        add_data
)
{
    CTRLOWNERDRAW_OBJ_T *owndraw_ctrl_ptr = PNULL;

    //get owner draw pointer by control id
    owndraw_ctrl_ptr = GetOwnDrawPtr (ctrl_id);

    if (PNULL != owndraw_ctrl_ptr)
    {
        owndraw_ctrl_ptr->add_data = add_data;
    }
}

/*****************************************************************************/
//  Description : 获取用户数据
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLOWNERDRAW_GetAddData (
    MMI_CTRL_ID_T   ctrl_id,//in
    ADD_DATA        *add_data_ptr
)
{
    BOOLEAN result = FALSE;
    CTRLOWNERDRAW_OBJ_T *owndraw_ctrl_ptr = PNULL;

    //get owner draw pointer by control id
    owndraw_ctrl_ptr = GetOwnDrawPtr (ctrl_id);

    if (PNULL != owndraw_ctrl_ptr && PNULL != add_data_ptr)
    {
        *add_data_ptr = owndraw_ctrl_ptr->add_data;
        result = TRUE;
    }

    return result;
}

/*****************************************************************************/
// 	Description : set owner draw background
//	Global resource dependence :
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLOWNERDRAW_SetBg (
    MMI_CTRL_ID_T   ctrl_id,//in
    GUI_BG_T        *bg_ptr //in
)
{
    CTRLOWNERDRAW_OBJ_T *owndraw_ctrl_ptr = PNULL;
    CTRLBASE_OBJ_T *base_obj_ptr = PNULL;

    //get owner draw pointer by control id
    owndraw_ctrl_ptr = GetOwnDrawPtr (ctrl_id);

    base_obj_ptr = (CTRLBASE_OBJ_T*) owndraw_ctrl_ptr;

    if (PNULL == base_obj_ptr || PNULL == bg_ptr)
    {
        return FALSE;
    }

    //set bg
    base_obj_ptr->bg = *bg_ptr;

    return TRUE;
}

/*****************************************************************************/
//  Description : set owner draw height
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLOWNERDRAW_SetHeight (
    MMI_HANDLE_T    ctrl_id,
    uint16          height
)
{
    BOOLEAN result = FALSE;
    CTRLOWNERDRAW_OBJ_T *owndraw_ctrl_ptr = PNULL;

    //get owner draw pointer by control id
    owndraw_ctrl_ptr = GetOwnDrawPtr (ctrl_id);

    if (PNULL != owndraw_ctrl_ptr)
    {
        owndraw_ctrl_ptr->height = height;
    }

    return (result);
}

/*****************************************************************************/
//  Description : update owner draw
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLOWNERDRAW_Update (
    MMI_HANDLE_T   ctrl_id
)
{
    BOOLEAN result = FALSE;
    CTRLOWNERDRAW_OBJ_T*owndraw_ctrl_ptr = PNULL;

    //get owner draw pointer by control id
    owndraw_ctrl_ptr = GetOwnDrawPtr (ctrl_id);

    if (PNULL != owndraw_ctrl_ptr)
    {
        //display owner draw
        OwnerDrawDraw (owndraw_ctrl_ptr);
    }

    return (result);
}

/*****************************************************************************/
//  Description : get owner draw rect
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLOWNERDRAW_GetRect (
    MMI_CTRL_ID_T ctrl_id,    //in:
    GUI_RECT_T *rect_ptr   //in/out:
)
{
    BOOLEAN             result = FALSE;
    CTRLOWNERDRAW_OBJ_T   *owndraw_ctrl_ptr = PNULL;
    CTRLBASE_OBJ_T *base_obj_ptr = PNULL;

    //get owner draw pointer by control id
    owndraw_ctrl_ptr = GetOwnDrawPtr (ctrl_id);
    base_obj_ptr = (CTRLBASE_OBJ_T*) owndraw_ctrl_ptr;

    if (PNULL != base_obj_ptr)
    {
        //set rect
        *rect_ptr = base_obj_ptr->rect;

        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : set owner draw rect
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLOWNERDRAW_SetRect (
    MMI_HANDLE_T    ctrl_id,
    GUI_RECT_T      *rect_ptr
)
{
    CTRLOWNERDRAW_OBJ_T *owndraw_ctrl_ptr = PNULL;

    //get owner draw pointer by control id
    owndraw_ctrl_ptr = GetOwnDrawPtr (ctrl_id);

    if (PNULL != owndraw_ctrl_ptr)
    {
        OwnDrawSetRect ( (CTRLBASE_OBJ_T *) owndraw_ctrl_ptr, rect_ptr);
    }
}

/*****************************************************************************/
//  Description : get owner draw display rect
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLOWNERDRAW_GetDisplayRect (
    MMI_CTRL_ID_T  ctrl_id,    //in:
    GUI_RECT_T     *rect_ptr   //in/out:
)
{
    BOOLEAN             result = FALSE;
    CTRLOWNERDRAW_OBJ_T *owndraw_ctrl_ptr = PNULL;
    CTRLBASE_OBJ_T *base_obj_ptr = PNULL;

    //get owner draw pointer by control id
    owndraw_ctrl_ptr = GetOwnDrawPtr (ctrl_id);
    base_obj_ptr = (CTRLBASE_OBJ_T*) owndraw_ctrl_ptr;

    if (PNULL != base_obj_ptr)
    {
        //set rect
        *rect_ptr = base_obj_ptr->display_rect;

        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
// 	Description : set owner draw is permit display border
//	Global resource dependence :
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLOWNERDRAW_PermitBorder (
    MMI_CTRL_ID_T    ctrl_id,
    BOOLEAN          is_permit
)
{
    BOOLEAN             result = FALSE;
    CTRLOWNERDRAW_OBJ_T *owndraw_ctrl_ptr = PNULL;

    //get owner draw pointer by control id
    owndraw_ctrl_ptr = GetOwnDrawPtr (ctrl_id);

    if (PNULL != owndraw_ctrl_ptr)
    {
        owndraw_ctrl_ptr->is_permit_border = is_permit;

        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : set ownerdraw is visible
//  Global resource dependence :
//  Author: hongjun.jia
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLOWNERDRAW_SetVisible (
    MMI_CTRL_ID_T   ctrl_id,
    BOOLEAN         is_visible,
    BOOLEAN         is_update
)
{
    BOOLEAN             result = FALSE;
    CTRLOWNERDRAW_OBJ_T *owndraw_ctrl_ptr = PNULL;
    CTRLBASE_OBJ_T *base_obj_ptr = PNULL;

    //get ownerdraw pointer by control id
    owndraw_ctrl_ptr = GetOwnDrawPtr (MMK_ConvertIdToHandle (ctrl_id));

    base_obj_ptr = (CTRLBASE_OBJ_T*) owndraw_ctrl_ptr;

    if (PNULL == base_obj_ptr)
    {
        return FALSE;
    }

    //set visible
    VTLBASE_SetVisible(base_obj_ptr, is_visible);

    if (is_update)
    {
        if (is_visible)
        {
            //update display
            OwnerDrawDraw (owndraw_ctrl_ptr);
        }
        else
        {
            //notify parent update bg
            CTRLMSG_SendNotifyEx (base_obj_ptr->handle, MSG_NOTIFY_UPDATE, &base_obj_ptr->rect);
        }
    }

    result = TRUE;

    return (result);
}

