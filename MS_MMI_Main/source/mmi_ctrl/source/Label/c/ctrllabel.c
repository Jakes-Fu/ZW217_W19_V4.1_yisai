/*****************************************************************************
** File Name:      ctrllabel.c                                               *
** Author:                                                                   *
** Date:           07/18/2012                                                *
** Copyright:      2004 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 07/2012      Jassmine              Creat
******************************************************************************/

#define _CTRLLABEL_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_gui_trc.h"
#include "cafctrllabel.h"
#include "mmk_type.h"
#include "mmk_app.h"
#include "mmk_timer.h"
#include "guires.h"
#include "mmi_theme.h"
#include "mmitheme_label.h"

#include "ctrllabel.h"
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
//  Description : init label class
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN LabelInitVtbl (
    CTRLLABEL_VTBL_T        *label_vtbl_ptr
);

/*****************************************************************************/
//  Description : label construct
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN LabelCtrlConstruct (
    CTRLBASE_OBJ_T           *base_ctrl_ptr,
    CTRLLABEL_INIT_PARAM_T   *label_param_ptr
);

/*****************************************************************************/
//  Description : label destruct
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN LabelCtrlDestruct (
    CTRLBASE_OBJ_T        *base_ctrl_ptr
);

/*****************************************************************************/
//  Description : convert init param
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void LabelCtrlPackInitParam (
    GUILABEL_INIT_DATA_T     *label_init_ptr,   //in
    CTRLLABEL_INIT_PARAM_T   *label_param_ptr   //out
);

/*****************************************************************************/
//  Description : handle label msg function
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E LabelCtrlHandleMsg (
    CTRLBASE_OBJ_T        *base_ctrl_ptr,  //control pointer
    MMI_MESSAGE_ID_E      msg_id,     //message
    DPARAM                param       //add data
);

/*****************************************************************************/
//  Description : display label control
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void LabelDraw (
    CTRLLABEL_OBJ_T     *label_ctrl_ptr
);

/*****************************************************************************/
//  Description : display label background
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void LabelDrawBg (
    GUI_RECT_T        bg_rect,
    CTRLLABEL_OBJ_T   *label_ctrl_ptr
);

/*****************************************************************************/
//  Description : display label background buffer
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void LabelDrawBgBuf (
    GUI_RECT_T         bg_rect,
    CTRLLABEL_OBJ_T    *label_ctrl_ptr
);

/*****************************************************************************/
// 	Description : display label icon
//	Global resource dependence :
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL void LabelDrawIcon (
    CTRLLABEL_OBJ_T     *label_ctrl_ptr  //in:
);

/*****************************************************************************/
// 	Description : copy label background
//	Global resource dependence :
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN CopyLabelBg (
    CTRLLABEL_OBJ_T   *label_ctrl_ptr
);

/*****************************************************************************/
// 	Description : display label string
//	Global resource dependence :
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL void LabelDrawString (
    CTRLLABEL_OBJ_T   *label_ctrl_ptr  //in:
);

/*****************************************************************************/
//  Description : get label icon display rect
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetLabelIconDisplayRect (
    uint16            *display_x_ptr, //in/out:
    uint16            *display_y_ptr, //in/out:
    GUI_RECT_T        *rect_ptr,      //in/out:
    CTRLLABEL_OBJ_T   *label_ctrl_ptr //in:
);

/*****************************************************************************/
//  Description : get label rect,except border
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL GUI_RECT_T GetLabelRect (
    CTRLLABEL_OBJ_T   *label_ctrl_ptr
);

/*****************************************************************************/
//  Description : get label string rect
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL GUI_RECT_T GetLabelStrRect (
    CTRLLABEL_OBJ_T    *label_ctrl_ptr //in:
);

/*****************************************************************************/
// 	Description : get label align
//	Global resource dependence :
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL GUI_ALIGN_E GetLabelAlign (
    GUILABEL_ALIGN_E    label_align
);


/*****************************************************************************/
// 	Description : label string is need scroll
//	Global resource dependence :
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN IsLabelTextScroll (
    GUI_RECT_T      str_rect,       //in:
    CTRLLABEL_OBJ_T *label_ctrl_ptr //in:
);

/*****************************************************************************/
// 	Description : start label time scroll timer
//	Global resource dependence :
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL void LabelStartScrollTimer (
    CTRLLABEL_OBJ_T    *label_ctrl_ptr
);

/*****************************************************************************/
// 	Description : stop label time scroll timer
//	Global resource dependence :
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL void LabelStopScrollTimer (
    CTRLLABEL_OBJ_T     *label_ctrl_ptr
);

/*****************************************************************************/
//  Description : handle label scroll timer
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void LabelHandleScrollTimer (
    CTRLLABEL_OBJ_T   *label_ctrl_ptr
);

/*****************************************************************************/
//  Description : handle label tp up message
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E LabelHandleTpUp (
    CTRLLABEL_OBJ_T  *label_ctrl_ptr,
    DPARAM           param
);

/*****************************************************************************/
//  Description : get label pointer by control id
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL CTRLLABEL_OBJ_T* GetLabelPtr (
    MMI_HANDLE_T     ctrl_handle
);

/*****************************************************************************/
//  Description : ext type of
//  Global resource dependence :
//  Author:Jassmine
//  Note: judge control type of
/*****************************************************************************/
LOCAL BOOLEAN LabelTypeOf (
    CTRLBASE_OBJ_T     *ctrl_ptr
);

/*****************************************************************************/
// 	Description : set label string by pointer
//	Global resource dependence :
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN SetLabelTextByPtr (
    CTRLLABEL_OBJ_T     *label_ctrl_ptr,//in
    MMI_STRING_T        *text_ptr,      //in
    BOOLEAN             is_update       //in
);

/*****************************************************************************/
//  Description : clear label string
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void ClearLabelString (
    CTRLLABEL_OBJ_T     *label_ctrl_ptr
);

/*****************************************************************************/
//  Description : add label string
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void AddLabelString (
    CTRLLABEL_OBJ_T   *label_ctrl_ptr,//in
    MMI_STRING_T      *text_ptr       //in
);

/*****************************************************************************/
//  Description : set label rect
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN LabelCtrlSetRect (
    CTRLBASE_OBJ_T       *base_ctrl_ptr,
    const CAF_RECT_T     *rect_ptr
);

/*****************************************************************************/
//  Description : set label border
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN LabelCtrlSetBorder (
    CTRLBASE_OBJ_T     *base_ctrl_ptr,
    GUI_BORDER_T       *border_ptr
);

/*****************************************************************************/
//  Description : set label bg
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN LabelCtrlSetBg (
    CTRLBASE_OBJ_T     *base_ctrl_ptr,
    GUI_BG_T           *bg_ptr
);

/*****************************************************************************/
//  Description : set label font
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN LabelCtrlSetFont (
    CTRLBASE_OBJ_T       *base_ctrl_ptr,
    GUI_FONT_ALL_T       *font_ptr
);

/*****************************************************************************/
//  Description : get label font
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN LabelCtrlGetFont (
    CTRLBASE_OBJ_T       *base_ctrl_ptr,
    GUI_FONT_ALL_T       *font_ptr
);

/*****************************************************************************/
//  Description : get label height by width,include border,text etc
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN LabelCtrlGetHeightByWidth (
    CTRLBASE_OBJ_T  *base_ctrl_ptr,  //in:
    uint16          width,      //in:
    uint16          *height_ptr //in/out:
);

/*****************************************************************************/
//  Description : get label display rect
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN LabelCtrlGetDisplayRect (
    CTRLBASE_OBJ_T    *base_ctrl_ptr,  //in:
    GUI_RECT_T        *rect_ptr   //in/out:
);

/*****************************************************************************/
//  Description : set label display rect
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN LabelCtrlSetDisplayRect (
    CTRLBASE_OBJ_T        *base_ctrl_ptr,  //in:
    const GUI_RECT_T      *rect_ptr,  //in:
    BOOLEAN               is_update   //in:
);

/*****************************************************************************/
//  Description : get label valid rect
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL GUI_RECT_T GetLabelValidRect (
    CTRLLABEL_OBJ_T *label_ctrl_ptr
);

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : get label type
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC TYPE_TAG_T LABEL_GetType()
{
    static TYPE_TAG_T    obj_type = 0;

    if (0 == obj_type)
    {
        obj_type = TYPEMNG_Register (
                       CTRL_BASE_TYPE,
                       "label",
                       sizeof (CTRLLABEL_OBJ_T),
                       (OBJECT_CONSTRUCTOR_FUNC) LabelCtrlConstruct,
                       (OBJECT_DESTRUCTOR_FUNC) LabelCtrlDestruct,
                       sizeof(CTRLLABEL_INIT_PARAM_T),
                       (PARAM_PACKING_FUNC)LabelCtrlPackInitParam,
                       sizeof (CTRLLABEL_VTBL_T),
                       (VTBL_INIT_FUNC) LabelInitVtbl);
    }

    return obj_type;
}

/*****************************************************************************/
//  Description : init label class
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN LabelInitVtbl (
    CTRLLABEL_VTBL_T        *label_vtbl_ptr
)
{
    CTRLBASE_VTBL_T  *base_ctrl_vtbl_ptr = (CTRLBASE_VTBL_T*) label_vtbl_ptr;

    base_ctrl_vtbl_ptr->HandleEvent = LabelCtrlHandleMsg;
    base_ctrl_vtbl_ptr->SetRect =  LabelCtrlSetRect;
    base_ctrl_vtbl_ptr->SetBorder =  LabelCtrlSetBorder;
    base_ctrl_vtbl_ptr->SetBg =  LabelCtrlSetBg;
    base_ctrl_vtbl_ptr->SetFont =  LabelCtrlSetFont;
    base_ctrl_vtbl_ptr->GetFont =  LabelCtrlGetFont;
    base_ctrl_vtbl_ptr->GetHeightByWidth = LabelCtrlGetHeightByWidth;
    base_ctrl_vtbl_ptr->GetDisplayRect = LabelCtrlGetDisplayRect;
    base_ctrl_vtbl_ptr->SetDisplayRect = LabelCtrlSetDisplayRect;

    return TRUE;
}

/*****************************************************************************/
//  Description : convert init param
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void LabelCtrlPackInitParam (
    GUILABEL_INIT_DATA_T     *label_init_ptr,   //in
    CTRLLABEL_INIT_PARAM_T   *label_param_ptr   //out
)
{
    CTRLBASE_INIT_PARAM_T   *base_ctrl_param_ptr = (CTRLBASE_INIT_PARAM_T*)label_param_ptr;

    base_ctrl_param_ptr->both_rect = label_init_ptr->both_rect;

    label_param_ptr->align = label_init_ptr->align;
}

/*****************************************************************************/
//  Description : label construct
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN LabelCtrlConstruct (
    CTRLBASE_OBJ_T           *base_ctrl_ptr,
    CTRLLABEL_INIT_PARAM_T   *label_param_ptr
)
{
    BOOLEAN             result = TRUE;
    CTRLLABEL_OBJ_T     *label_ctrl_ptr = (CTRLLABEL_OBJ_T*) base_ctrl_ptr;

    //set align
    label_ctrl_ptr->align        = label_param_ptr->align;

    //set win handle
    label_ctrl_ptr->win_handle = MMK_GetWinHandleByCtrl (base_ctrl_ptr->handle);

    //get label theme
    MMITHEME_GetLabelTheme (&label_ctrl_ptr->theme);

    return (result);
}

/*****************************************************************************/
//  Description : label destruct
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN LabelCtrlDestruct (
    CTRLBASE_OBJ_T    *base_ctrl_ptr
)
{
    BOOLEAN             result = TRUE;
    CTRLLABEL_OBJ_T     *label_ctrl_ptr = (CTRLLABEL_OBJ_T*) base_ctrl_ptr;

    if (PNULL != label_ctrl_ptr)
    {
        //stop timer
        LabelStopScrollTimer (label_ctrl_ptr);

        //free text string memory
        if (PNULL != label_ctrl_ptr->str_ptr)
        {
            SCI_FREE (label_ctrl_ptr->str_ptr);
        }

        //free bg buffer
        if (PNULL != label_ctrl_ptr->bg_buf_ptr)
        {
            SCI_FREE (label_ctrl_ptr->bg_buf_ptr);
        }

        //free region
        if (PNULL != label_ctrl_ptr->region_ptr)
        {
            SCI_FREE (label_ctrl_ptr->region_ptr);
            label_ctrl_ptr->region_num = 0;
        }

        //free img buffer
        if (PNULL != label_ctrl_ptr->img_data.buf.buf_ptr)
        {
            SCI_FREE (label_ctrl_ptr->img_data.buf.buf_ptr);
        }
    }

    return (result);
}

/*****************************************************************************/
//  Description : handle label msg function
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E LabelCtrlHandleMsg (
    CTRLBASE_OBJ_T        *base_ctrl_ptr,  //control pointer
    MMI_MESSAGE_ID_E      msg_id,     //message
    DPARAM                param       //add data
)
{
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    CTRLLABEL_OBJ_T     *label_ctrl_ptr = (CTRLLABEL_OBJ_T*) base_ctrl_ptr;

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != label_ctrl_ptr); /*assert verified*/
    if (PNULL == label_ctrl_ptr)
    {
        return MMI_RESULT_FALSE;
    }

    switch (msg_id)
    {
    case MSG_CTL_PAINT:
        LabelDraw (label_ctrl_ptr);
        break;

    case MSG_CTL_GET_ACTIVE:
        label_ctrl_ptr->is_active = TRUE;
        //notify parent control
        result = CTRLMSG_SendNotify (base_ctrl_ptr->handle, MSG_NOTIFY_GET_ACTIVE);
        break;

    case MSG_CTL_LOSE_ACTIVE:
    case MSG_CTL_LOSE_FOCUS:
    case MSG_CTL_CLOSE:
        //stop timer
        LabelStopScrollTimer (label_ctrl_ptr);

        if (MSG_CTL_LOSE_ACTIVE == msg_id)
        {
            label_ctrl_ptr->is_active = FALSE;

            label_ctrl_ptr->str_index = 0;

            //notify parent control
            result = CTRLMSG_SendNotify (base_ctrl_ptr->handle, MSG_NOTIFY_LOSE_ACTIVE);
        }

        break;

    case MSG_NOTIFY_LOSE_ACTIVE:

        if (!MMK_IsActiveCtrl (base_ctrl_ptr->handle))
        {
            //stop timer
            LabelStopScrollTimer (label_ctrl_ptr);
        }

        break;

    case MSG_TIMER:

        if (label_ctrl_ptr->scroll_timer_id == * (uint8*) param)
        {
            LabelHandleScrollTimer (label_ctrl_ptr);
        }
        else
        {
            result = MMI_RESULT_FALSE;
        }

        break;

    case MSG_APP_OK:
        //notify parent control
        result = CTRLMSG_PostNotify (base_ctrl_ptr->handle, MSG_NOTIFY_OK);
        break;

    case MSG_APP_WEB:
        //notify parent control
        result = CTRLMSG_PostNotify (base_ctrl_ptr->handle, MSG_NOTIFY_MIDSK);
        break;

    case MSG_APP_CANCEL:
        //notify parent control
        result = CTRLMSG_PostNotify (base_ctrl_ptr->handle, MSG_NOTIFY_CANCEL);
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_UP:
        result = LabelHandleTpUp (label_ctrl_ptr, param);
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

    default:
        result = MMI_RESULT_FALSE;
        break;
    }

    return (result);
}

/*****************************************************************************/
//  Description : display label control
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void LabelDraw (
    CTRLLABEL_OBJ_T     *label_ctrl_ptr
)
{
    GUI_RECT_T      cross_rect = {0};
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*) label_ctrl_ptr;

    if ( (PNULL != label_ctrl_ptr) &&
            (VTLBASE_GetVisible( (CTRLBASE_OBJ_T*) label_ctrl_ptr)) &&
            (GUI_IntersectRect (&cross_rect, base_ctrl_ptr->display_rect, base_ctrl_ptr->rect)))
    {
        //display bg
#ifdef MAINLCD_SIZE_128X64 //@ alex added in2010.12.02
        label_ctrl_ptr->theme.bg.bg_type = GUI_BG_NONE;
#endif
        LabelDrawBg (cross_rect, label_ctrl_ptr);

        //display border
        GUI_DisplayBorder (base_ctrl_ptr->rect,
                           base_ctrl_ptr->display_rect,
                           &label_ctrl_ptr->theme.border,
                           &base_ctrl_ptr->lcd_dev_info);

        //display label icon
        LabelDrawIcon (label_ctrl_ptr);

        //display string
        LabelDrawString (label_ctrl_ptr);
    }
}

/*****************************************************************************/
//  Description : display label background
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void LabelDrawBg (
    GUI_RECT_T        bg_rect,
    CTRLLABEL_OBJ_T   *label_ctrl_ptr
)
{
    GUI_RECT_T          img_rect = {0};
    GUI_BG_DISPLAY_T    bg_display = {0};
    CTRLBASE_OBJ_T      *base_ctrl_ptr = (CTRLBASE_OBJ_T*) label_ctrl_ptr;

    //no parent control
    if ( (GUI_BG_NONE == label_ctrl_ptr->theme.bg.bg_type) &&
            (0 == MMK_GetParentCtrlHandle (base_ctrl_ptr->handle)))
    {
        //copy label bg
        CopyLabelBg (label_ctrl_ptr);

        LabelDrawBgBuf (bg_rect, label_ctrl_ptr);
    }
#if 0
    else if ( (GUI_BG_COLOR == label_ctrl_ptr->theme.bg.bg_type) &&
              (MMITHEME_GetTransparenceColor() == label_ctrl_ptr->theme.bg.color))
    {
        //do nothing
    }
#endif
    else
    {
        //set image rect
        img_rect.left   = (int16) (bg_rect.left - base_ctrl_ptr->rect.left);
        img_rect.top    = (int16) (bg_rect.top - base_ctrl_ptr->rect.top);
        img_rect.right  = (int16) (img_rect.left + bg_rect.right - bg_rect.left);
        img_rect.bottom = (int16) (img_rect.top + bg_rect.bottom - bg_rect.top);

        //set bg display info
        bg_display.is_transparent = FALSE;
        bg_display.rect           = base_ctrl_ptr->rect;
        bg_display.display_rect   = bg_rect;
        bg_display.img_rect       = img_rect;
        bg_display.ctrl_handle    = base_ctrl_ptr->handle;
        bg_display.win_handle     = label_ctrl_ptr->win_handle;

        GUI_DisplayBg (&label_ctrl_ptr->theme.bg,
                       &bg_display,
                       &base_ctrl_ptr->lcd_dev_info);
    }
}

/*****************************************************************************/
// 	Description : copy label background
//	Global resource dependence :
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN CopyLabelBg (
    CTRLLABEL_OBJ_T   *label_ctrl_ptr
)
{
    BOOLEAN         result = FALSE;
    uint16          label_width = 0;
    uint16          label_height = 0;
    uint32          label_size = 0;
    GUI_RECT_T      cross_rect = {0};
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*) label_ctrl_ptr;

    if ( (MMITHEME_IsBacklightOn()) &&
            (GUI_BG_NONE == label_ctrl_ptr->theme.bg.bg_type) &&
            (PNULL == label_ctrl_ptr->bg_buf_ptr) &&
            (MMK_IsWinOnPaint (label_ctrl_ptr->win_handle))) //app update in open win,rect is error
    {
        cross_rect = GetLabelValidRect (label_ctrl_ptr);

        if (GUI_IsInvalidRectEx (cross_rect))
        {
            return result;
        }

        //get label width,height and size
        label_width  = (uint16) (cross_rect.right - cross_rect.left + 1);
        label_height = (uint16) (cross_rect.bottom - cross_rect.top + 1);
        label_size   = label_width * label_height;

        //alloc bg buffer
        label_ctrl_ptr->bg_buf_ptr  = SCI_ALLOCA (label_size * sizeof (GUI_COLOR_T));

        if (PNULL != label_ctrl_ptr->bg_buf_ptr)
        {
            SCI_MEMSET (label_ctrl_ptr->bg_buf_ptr, 0, (label_size*sizeof (GUI_COLOR_T)));

            //copy buffer
            LCD_CopyLayerBufToMem (&base_ctrl_ptr->lcd_dev_info,
                                   label_ctrl_ptr->bg_buf_ptr,
                                   label_width,
                                   label_height,
                                   cross_rect.left,
                                   cross_rect.top);

            result = TRUE;
        }
    }

    return (result);
}

/*****************************************************************************/
//  Description : display label background buffer
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void LabelDrawBgBuf (
    GUI_RECT_T         bg_rect,
    CTRLLABEL_OBJ_T    *label_ctrl_ptr
)
{
    uint16          i = 0;
    uint16          bg_buf_width = 0;
    uint16          cross_width = 0;
    uint16          cross_height = 0;
    uint16          lcd_buf_width = 0;
    GUI_RECT_T      cross_rect = {0};
    GUI_RECT_T      valid_rect = {0};
    GUI_COLOR_T     *bg_buf_ptr = PNULL;
    GUI_COLOR_T     *lcd_buf_ptr = PNULL;
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*) label_ctrl_ptr;

    if (PNULL != label_ctrl_ptr->bg_buf_ptr)
    {
        valid_rect = GetLabelValidRect (label_ctrl_ptr);

        //get crossed rect is lcd rect
        if (GUI_IntersectRect (&cross_rect, valid_rect, bg_rect) && !GUI_IsInvalidRectEx (cross_rect))
        {
            //set bg buffer and width
            bg_buf_ptr   = label_ctrl_ptr->bg_buf_ptr;
            bg_buf_width = (uint16) (valid_rect.right - valid_rect.left + 1);

            //get cross width and height
            cross_width  = (uint16) (cross_rect.right - cross_rect.left + 1);
            cross_height = (uint16) (cross_rect.bottom - cross_rect.top + 1);

            //get lcd buffer
            lcd_buf_ptr   = (GUI_COLOR_T*) GUIBLOCK_GetBuf (&base_ctrl_ptr->lcd_dev_info);
            GUILCD_GetLogicWidthHeight (base_ctrl_ptr->lcd_dev_info.lcd_id, &lcd_buf_width, PNULL);

            //set lcd buffer start
            lcd_buf_ptr += cross_rect.top * lcd_buf_width + cross_rect.left;  /*lint !e737*/

            //set bg buffer start
            bg_buf_ptr += (cross_rect.top - valid_rect.top) * bg_buf_width + (cross_rect.left - valid_rect.left);

            //copy buffer
            for (i = 0; i < cross_height; i++)
            {
                SCI_MEMCPY (lcd_buf_ptr, bg_buf_ptr, (cross_width) *sizeof (GUI_COLOR_T));
                lcd_buf_ptr += lcd_buf_width;
                bg_buf_ptr  += bg_buf_width;

            }

            //store rect
            MMITHEME_StoreUpdateRect ( (const GUI_LCD_DEV_INFO*) &base_ctrl_ptr->lcd_dev_info, cross_rect);
        }
    }
}

/*****************************************************************************/
// 	Description : display label icon
//	Global resource dependence :
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL void LabelDrawIcon (
    CTRLLABEL_OBJ_T *label_ctrl_ptr  //in:
)
{
    uint16          display_x = 0;
    uint16          display_y = 0;
    GUI_RECT_T      icon_rect = {0};
    GUI_RECT_T      cross_rect = {0};
    GUI_RECT_T      img_rect = {0};
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*) label_ctrl_ptr;

    //get icon display rect
    if (GetLabelIconDisplayRect (&display_x, &display_y, &icon_rect, label_ctrl_ptr))
    {
        //get crossed rect is lcd rect
        if (GUI_IntersectRect (&cross_rect, base_ctrl_ptr->display_rect, icon_rect))
        {
            //image rect
            img_rect.left   = (int16) (cross_rect.left - display_x);
            img_rect.top    = (int16) (cross_rect.top - display_y);
            img_rect.right  = (int16) (cross_rect.right - display_x);
            img_rect.bottom = (int16) (cross_rect.bottom - display_y);

            //display icon
            IMG_EnableTransparentColor (TRUE);

            if (MMITHEME_CheckImageID (label_ctrl_ptr->icon_id))
            {
                GUIRES_DisplayImg (PNULL,
                                   &cross_rect,
                                   &img_rect,
                                   label_ctrl_ptr->win_handle,
                                   label_ctrl_ptr->icon_id,
                                   (const GUI_LCD_DEV_INFO*) &base_ctrl_ptr->lcd_dev_info);
            }
            else if (GUILABEL_IMAGE_RES == label_ctrl_ptr->img_type
                     && MMITHEME_CheckImageID (label_ctrl_ptr->img_data.res.id))
            {
                GUIRES_DisplayImg (PNULL,
                                   &cross_rect,
                                   &img_rect,
                                   label_ctrl_ptr->win_handle,
                                   label_ctrl_ptr->img_data.res.id,
                                   (const GUI_LCD_DEV_INFO*) &base_ctrl_ptr->lcd_dev_info);
            }
            else if (GUILABEL_IMAGE_BUF == label_ctrl_ptr->img_type)
            {
                if (label_ctrl_ptr->is_bit_map)
                {
                    GUIIMG_BITMAP_T bit_map = {0};
                    GUI_POINT_T dis_point = {0};

                    dis_point.x = cross_rect.left;
                    dis_point.y = cross_rect.top;

                    bit_map.bit_ptr = label_ctrl_ptr->img_data.buf.buf_ptr;
                    bit_map.img_width = label_ctrl_ptr->img_data.buf.width;
                    bit_map.img_height = label_ctrl_ptr->img_data.buf.height;
                    GUIIMG_DisplayBmp (FALSE, &cross_rect, &dis_point, &bit_map, (const GUI_LCD_DEV_INFO*) &base_ctrl_ptr->lcd_dev_info);
                }
                else
                {
                    GUIRES_DisplayImgByPtr (label_ctrl_ptr->img_data.buf.buf_ptr,
                                            label_ctrl_ptr->img_data.buf.size,
                                            PNULL,
                                            &cross_rect,
                                            &img_rect,
                                            (const GUI_LCD_DEV_INFO*) &base_ctrl_ptr->lcd_dev_info);
                }
            }

            IMG_EnableTransparentColor (FALSE);
        }
    }
}

/*****************************************************************************/
//  Description : get label icon display rect
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetLabelIconDisplayRect (
    uint16            *display_x_ptr, //in/out:
    uint16            *display_y_ptr, //in/out:
    GUI_RECT_T        *rect_ptr,      //in/out:
    CTRLLABEL_OBJ_T   *label_ctrl_ptr //in:
)
{
    BOOLEAN     result = FALSE;
    uint16      display_x = 0;
    uint16      display_y = 0;
    uint16      shrect_height = 0;
    uint16      pic_width = 0;
    uint16      pic_height = 0;
    GUI_RECT_T  label_rect = {0};
    GUI_RECT_T  icon_rect = {0};
    MMI_IMAGE_ID_T icon_id = 0;

    if (PNULL == label_ctrl_ptr)
    {
        return result;
    }

    icon_id = label_ctrl_ptr->icon_id;

    if (!MMITHEME_CheckImageID (icon_id)
            && GUILABEL_IMAGE_RES == label_ctrl_ptr->img_type)
    {
        icon_id = label_ctrl_ptr->img_data.res.id;
    }

    //get label rect
    label_rect = GetLabelRect (label_ctrl_ptr);

    if (MMITHEME_CheckImageID (icon_id))
    {
        //get icon width and height
        GUIRES_GetImgWidthHeight (&pic_width, &pic_height, icon_id, label_ctrl_ptr->win_handle);

        result = TRUE;
    }
    else if (GUILABEL_IMAGE_BUF == label_ctrl_ptr->img_type)
    {
        if (label_ctrl_ptr->is_bit_map)
        {
            pic_width = label_ctrl_ptr->img_data.buf.width;
            pic_height = label_ctrl_ptr->img_data.buf.height;
        }
        else
        {
            GUIRES_GetImgWidthHeightByPtr (label_ctrl_ptr->img_data.buf.buf_ptr,
                                           &pic_width, &pic_height,
                                           label_ctrl_ptr->img_data.buf.size);
        }

        result = TRUE;
    }

    if (result)
    {
        //get image display x and y
        display_x = (uint16) (label_rect.left + label_ctrl_ptr->theme.margin_lr_space);
        display_y = label_rect.top;

        //set show rect height
        shrect_height = (uint16) (label_rect.bottom - label_rect.top + 1);

        //set start y
        if (shrect_height > pic_height + 1)
        {
            display_y = (uint16) ( ( (shrect_height - pic_height) >> 1) + display_y);
        }

        //set icon display rect
        icon_rect.left   = display_x;
        icon_rect.top    = display_y;
        icon_rect.right  = (int16) (icon_rect.left + pic_width - 1);
        icon_rect.bottom = (int16) (icon_rect.top + pic_height - 1);

        //check rect by direction
        if (MMI_DIRECTION_RIGHT == MMITHEME_GetDisDirection())
        {
            icon_rect.right = (int16) (label_rect.right + label_rect.left - icon_rect.left);
            icon_rect.left = (int16) (icon_rect.right - pic_width + 1);

            display_x = icon_rect.left;
        }

        //set output
        if (PNULL != display_x_ptr)
        {
            *display_x_ptr = display_x;
        }

        if (PNULL != display_y_ptr)
        {
            *display_y_ptr = display_y;
        }

        if (PNULL != rect_ptr)
        {
            *rect_ptr = icon_rect;
        }
    }

    return (result);
}

/*****************************************************************************/
//  Description : get label rect,except border
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL GUI_RECT_T GetLabelRect (
    CTRLLABEL_OBJ_T   *label_ctrl_ptr
)
{
    BOOLEAN         is_border = TRUE;
    GUI_RECT_T      label_rect = {0};
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*) label_ctrl_ptr;

    //label is exist border
    if (GUI_BORDER_NONE == label_ctrl_ptr->theme.border.type)
    {
        is_border = FALSE;
    }

    if (is_border)
    {
        label_rect.left   = (int16) (base_ctrl_ptr->rect.left + label_ctrl_ptr->theme.border.width);
        label_rect.right  = (int16) (base_ctrl_ptr->rect.right - label_ctrl_ptr->theme.border.width);
        label_rect.top    = (int16) (base_ctrl_ptr->rect.top + label_ctrl_ptr->theme.border.width);
        label_rect.bottom = (int16) (base_ctrl_ptr->rect.bottom - label_ctrl_ptr->theme.border.width);
    }
    else
    {
        label_rect = base_ctrl_ptr->rect;
    }

    return (label_rect);
}

/*****************************************************************************/
//  Description : get label valid rect
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL GUI_RECT_T GetLabelValidRect (
    CTRLLABEL_OBJ_T *label_ctrl_ptr
)
{
    GUI_RECT_T      full_rect = {0};
    GUI_RECT_T      cross_rect = {0};
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*) label_ctrl_ptr;

    full_rect = MMITHEME_GetFullScreenRect();

    GUI_IntersectRect (&cross_rect, base_ctrl_ptr->display_rect, base_ctrl_ptr->rect);
    GUI_IntersectRect (&cross_rect, cross_rect, full_rect);

    return cross_rect;
}

/*****************************************************************************/
//  Description : get label string rect
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL GUI_RECT_T GetLabelStrRect (
    CTRLLABEL_OBJ_T    *label_ctrl_ptr //in:
)
{
    uint16      icon_width = 0;
    uint16      icon_height = 0;
    GUI_RECT_T  str_rect = {0};
    MMI_IMAGE_ID_T icon_id = 0;
    BOOLEAN result = FALSE;

    icon_id = label_ctrl_ptr->icon_id;

    if (!MMITHEME_CheckImageID (icon_id)
            && GUILABEL_IMAGE_RES == label_ctrl_ptr->img_type)
    {
        icon_id = label_ctrl_ptr->img_data.res.id;
    }

    //set label string rect
    str_rect = GetLabelRect (label_ctrl_ptr);

    //except margin
    str_rect.left  = (uint16) (str_rect.left + label_ctrl_ptr->theme.margin_lr_space);
    str_rect.right = (uint16) (str_rect.right - label_ctrl_ptr->theme.margin_lr_space);

    if (MMITHEME_CheckImageID (icon_id))
    {
        //get icon width and height
        GUIRES_GetImgWidthHeight (&icon_width, PNULL, icon_id, label_ctrl_ptr->win_handle);

        result = TRUE;
    }
    else if (GUILABEL_IMAGE_BUF == label_ctrl_ptr->img_type)
    {
        if (label_ctrl_ptr->is_bit_map)
        {
            icon_width = label_ctrl_ptr->img_data.buf.width;
            icon_height = label_ctrl_ptr->img_data.buf.height;
        }
        else
        {
            GUIRES_GetImgWidthHeightByPtr (label_ctrl_ptr->img_data.buf.buf_ptr,
                                           &icon_width, &icon_height,
                                           label_ctrl_ptr->img_data.buf.size);
        }

        result = TRUE;
    }

    if (result)
    {
        //check rect by direction
        if (MMI_DIRECTION_RIGHT == MMITHEME_GetDisDirection())
        {
            //set string rect
            str_rect.right = (int16) (str_rect.right - icon_width - label_ctrl_ptr->theme.icon_space);
        }
        else
        {
            //set string rect
            str_rect.left = (int16) (str_rect.left + icon_width + label_ctrl_ptr->theme.icon_space);
        }
    }

    return (str_rect);
}


/*****************************************************************************/
// 	Description : display label string
//	Global resource dependence :
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL void LabelDrawString (
    CTRLLABEL_OBJ_T   *label_ctrl_ptr  //in:
)
{
    BOOLEAN         is_scroll = FALSE;
    GUI_RECT_T      str_rect = {0};
    GUI_RECT_T      cross_rect = {0};
    GUI_ALIGN_E     align = ALIGN_LVMIDDLE;
    MMI_STRING_T    label_str = {0};
    GUISTR_STYLE_T  str_style = {0};
    GUISTR_STATE_T  str_state = GUISTR_STATE_ALIGN | GUISTR_STATE_SINGLE_LINE;
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*) label_ctrl_ptr;

    if (0 < label_ctrl_ptr->str_len)
    {
        //get label string rect
        str_rect = GetLabelStrRect (label_ctrl_ptr);

        //get crossed rect
        if (GUI_IntersectRect (&cross_rect, base_ctrl_ptr->display_rect, str_rect))
        {
            //get string is need scroll or ellipsis
            is_scroll = IsLabelTextScroll (str_rect, label_ctrl_ptr);
			//set state
            if (is_scroll)
            {
				/*
				 *	如果label的字符串需要滚动显示，则强制将字符串的对齐方式设置为 GUILABEL_ALIGN_LEFT
				 *  防止如果字符串对齐方式为GUILABEL_ALIGN_MIDDLE时，字符串滚动显示但前面部分无法显示
				 */
				if (GUILABEL_ALIGN_MIDDLE == label_ctrl_ptr->align)
				{
					label_ctrl_ptr->align = GUILABEL_ALIGN_LEFT;
				}
				//start string scroll timer
                LabelStartScrollTimer (label_ctrl_ptr);
                }
            else if(TRUE == label_ctrl_ptr->is_ellipsis_ex)
            {
                str_state = str_state | GUISTR_STATE_ELLIPSIS_EX;
             }
            else
            {
                str_state = str_state | GUISTR_STATE_ELLIPSIS;
            }

            if (GUILABEL_ALIGN_LEFT == label_ctrl_ptr->align)
            {
                str_state = str_state | GUISTR_STATE_ALIGN_BY_DIR;
            }

			//set align
            align = GetLabelAlign (label_ctrl_ptr->align);

            //set string style
            str_style.font       = label_ctrl_ptr->theme.font.font;
            str_style.font_color = label_ctrl_ptr->theme.font.color;
            str_style.align      = align;
            str_style.line_color = label_ctrl_ptr->theme.font.color;
            str_style.char_space = label_ctrl_ptr->theme.char_space;
            str_style.region_ptr = label_ctrl_ptr->region_ptr;
            str_style.region_num = label_ctrl_ptr->region_num;

            //display string
            if ( (is_scroll) &&
                    (MMITHEME_GetIsScrollAsPixel()))
            {
                //set label string
                label_str.wstr_ptr = label_ctrl_ptr->str_ptr;
                label_str.wstr_len = label_ctrl_ptr->str_len;

                GUISTR_DrawTextToLCDByOffset ( (const GUI_LCD_DEV_INFO*) &base_ctrl_ptr->lcd_dev_info,
                                               (const GUI_RECT_T*) &cross_rect,
                                               PNULL,
                                               label_ctrl_ptr->str_index,
                                               0,
                                               &label_str,
                                               &str_style,
                                               str_state);
            }
            else
            {
                //set label string
                label_str.wstr_ptr = label_ctrl_ptr->str_ptr + label_ctrl_ptr->str_index;
                label_str.wstr_len = (uint16) (label_ctrl_ptr->str_len - label_ctrl_ptr->str_index);

                GUISTR_DrawTextToLCDInRect ( (const GUI_LCD_DEV_INFO*) &base_ctrl_ptr->lcd_dev_info,
                                             (const GUI_RECT_T*) &str_rect,
                                             (const GUI_RECT_T*) &cross_rect,
                                             &label_str,
                                             &str_style,
                                             str_state,
                                             GUISTR_TEXT_DIR_AUTO);
            }
        }
    }
}

/*****************************************************************************/
// 	Description : get label align
//	Global resource dependence :
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL GUI_ALIGN_E GetLabelAlign (
    GUILABEL_ALIGN_E    label_align
)
{
    GUI_ALIGN_E     align = ALIGN_LVMIDDLE;

    switch (label_align)
    {
    case GUILABEL_ALIGN_LEFT:
        align = ALIGN_LVMIDDLE;
        break;

    case GUILABEL_ALIGN_MIDDLE:
        align = ALIGN_HVMIDDLE;
        break;

    case GUILABEL_ALIGN_RIGHT:
        align = ALIGN_RVMIDDLE;
        break;

    default:
        //SCI_TRACE_LOW:"GetLabelAlign:label_align %d is error!"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, GUILABEL_1208_112_2_18_3_19_35_202, (uint8*) "d", label_align);
        break;
    }

    return (align);
}

/*****************************************************************************/
// 	Description : label string is need scroll
//	Global resource dependence :
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN IsLabelTextScroll (
    GUI_RECT_T      str_rect,       //in:
    CTRLLABEL_OBJ_T *label_ctrl_ptr //in:
)
{
    BOOLEAN     result = FALSE;
    uint16      str_width = 0;
    uint16      display_width = 0;

    //is active
    if ((label_ctrl_ptr->is_active) ||(label_ctrl_ptr->is_auto_scroll))
    {
        //get string width
        str_width = GUI_CalculateStringPiexlNum (label_ctrl_ptr->str_ptr,
                    label_ctrl_ptr->str_len,
                    label_ctrl_ptr->theme.font.font,
                    (uint8) label_ctrl_ptr->theme.char_space);

        //get display width
        display_width = (uint16) (str_rect.right - str_rect.left + 1);

        if (str_width > display_width)
        {
            result = TRUE;
        }
    }

    return (result);
}

/*****************************************************************************/
// 	Description : start label time scroll timer
//	Global resource dependence :
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL void LabelStartScrollTimer (
    CTRLLABEL_OBJ_T    *label_ctrl_ptr
)
{
    if ( (MMK_IsFocusWin (label_ctrl_ptr->win_handle)) &&
            (0 == label_ctrl_ptr->scroll_timer_id))
    {
        CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*) label_ctrl_ptr;

        label_ctrl_ptr->scroll_timer_id = MMK_CreateWinTimer (base_ctrl_ptr->handle,
                                          label_ctrl_ptr->theme.scroll_period,
                                          FALSE);
    }
}

/*****************************************************************************/
// 	Description : stop label time scroll timer
//	Global resource dependence :
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL void LabelStopScrollTimer (
    CTRLLABEL_OBJ_T     *label_ctrl_ptr
)
{
    if (0 < label_ctrl_ptr->scroll_timer_id)
    {
        MMK_StopTimer (label_ctrl_ptr->scroll_timer_id);
        label_ctrl_ptr->scroll_timer_id = 0;
    }
}

/*****************************************************************************/
//  Description : handle label scroll timer
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void LabelHandleScrollTimer (
    CTRLLABEL_OBJ_T   *label_ctrl_ptr
)
{
    uint16      str_width = 0;
    uint16      display_width = 0;
    GUI_RECT_T  str_rect = {0};

    //stop timer
    LabelStopScrollTimer (label_ctrl_ptr);

    //get label string rect
    str_rect = GetLabelStrRect (label_ctrl_ptr);

    //get display width
    display_width = (uint16) (str_rect.right - str_rect.left + 1);

    //set string index
    if (MMITHEME_GetIsScrollAsPixel())
    {
        //get item text len
        str_width = GUI_CalculateStringPiexlNum (label_ctrl_ptr->str_ptr,
                    label_ctrl_ptr->str_len,
                    label_ctrl_ptr->theme.font.font,
                    (uint8) label_ctrl_ptr->theme.char_space);

        if ( (str_width - 1 - label_ctrl_ptr->str_index) > display_width)
        {
            label_ctrl_ptr->str_index = (uint16) (label_ctrl_ptr->str_index + label_ctrl_ptr->theme.scroll_pixel);
        }
        else
        {
            label_ctrl_ptr->str_index = 0;
        }
    }
    else
    {
        //get label string len
        str_width = GUI_CalculateStringPiexlNum ( (label_ctrl_ptr->str_ptr + label_ctrl_ptr->str_index),
                    (uint16) (label_ctrl_ptr->str_len - label_ctrl_ptr->str_index),
                    label_ctrl_ptr->theme.font.font,
                    (uint8) label_ctrl_ptr->theme.char_space);

        //string has display
        if (str_width > display_width)
        {
            label_ctrl_ptr->str_index++;
        }
        else
        {
            label_ctrl_ptr->str_index = 0;
        }
    }

    //display label
    LabelDraw (label_ctrl_ptr);
}

/*****************************************************************************/
//  Description : handle label tp up message
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E LabelHandleTpUp (
    CTRLLABEL_OBJ_T  *label_ctrl_ptr,
    DPARAM           param
)
{
    MMI_RESULT_E    result = MMI_RESULT_FALSE;
    GUI_RECT_T      cross_rect = {0};
    GUI_POINT_T     tp_point = {0};
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*) label_ctrl_ptr;

    //not slide
    if (!MMK_GET_TP_SLIDE (param))
    {
        //set tp press point
        tp_point.x = MMK_GET_TP_X (param);
        tp_point.y = MMK_GET_TP_Y (param);

        if ( (GUI_IntersectRect (&cross_rect, base_ctrl_ptr->display_rect, base_ctrl_ptr->rect)) &&
                (GUI_PointIsInRect (tp_point, cross_rect)))
        {
            //notify parent control
            result = CTRLMSG_PostNotify (base_ctrl_ptr->handle, MSG_NOTIFY_PENOK);
        }
        else if ( (!GUI_IsRectEmpty (label_ctrl_ptr->tp_rect)) &&
                  (GUI_PointIsInRect (tp_point, label_ctrl_ptr->tp_rect)))
        {
            //notify parent control
            result = CTRLMSG_PostNotify (base_ctrl_ptr->handle, MSG_NOTIFY_PENOK);
        }
    }

    return (result);
}

/*****************************************************************************/
//  Description : get label pointer by control id
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL CTRLLABEL_OBJ_T* GetLabelPtr (
    MMI_HANDLE_T     ctrl_handle
)
{
    CTRLBASE_OBJ_T *ctrl_ptr = (CTRLBASE_OBJ_T*)MMK_GetCtrlPtr (ctrl_handle);

    if (PNULL != ctrl_ptr)
    {
        SCI_ASSERT (LabelTypeOf (ctrl_ptr)); /*assert verified*/
    }

    return (CTRLLABEL_OBJ_T*)ctrl_ptr;
}

/*****************************************************************************/
//  Description : ext type of
//  Global resource dependence :
//  Author:Jassmine
//  Note: judge control type of
/*****************************************************************************/
LOCAL BOOLEAN LabelTypeOf (
    CTRLBASE_OBJ_T     *ctrl_ptr
)
{
    return TYPEMNG_IsTypeOf(ctrl_ptr, CTRL_LABEL_TYPE);
}

/*****************************************************************************/
// 	Description : set label string by pointer
//	Global resource dependence :
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN SetLabelTextByPtr (
    CTRLLABEL_OBJ_T     *label_ctrl_ptr,//in
    MMI_STRING_T        *text_ptr,      //in
    BOOLEAN             is_update       //in
)
{
    BOOLEAN     result = TRUE;

    if (PNULL != text_ptr)
    {
        //stop timer
        LabelStopScrollTimer (label_ctrl_ptr);

        //clear all string
        ClearLabelString (label_ctrl_ptr);

        if (0 < text_ptr->wstr_len)
        {
            //add string
            AddLabelString (label_ctrl_ptr, text_ptr);
        }

        if ( (is_update) &&
                (MMK_IsFocusWin (label_ctrl_ptr->win_handle)))
        {
            //update display
            LabelDraw (label_ctrl_ptr);
        }
    }

    return (result);
}

/*****************************************************************************/
//  Description : clear label string
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void ClearLabelString (
    CTRLLABEL_OBJ_T     *label_ctrl_ptr
)
{
    if (0 < label_ctrl_ptr->str_len)
    {
        //free label string memory
        if (PNULL != label_ctrl_ptr->str_ptr)
        {
            SCI_FREE (label_ctrl_ptr->str_ptr);
        }

        label_ctrl_ptr->str_len = 0;

        //set start index
        label_ctrl_ptr->str_index = 0;
    }
}

/*****************************************************************************/
//  Description : add label string
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void AddLabelString (
    CTRLLABEL_OBJ_T   *label_ctrl_ptr,//in
    MMI_STRING_T      *text_ptr       //in
)
{
    uint16  i = 0;

    //alloc string memory
    SCI_ASSERT (PNULL == label_ctrl_ptr->str_ptr); /*assert verified*/
    label_ctrl_ptr->str_ptr = SCI_ALLOC_APP ( (text_ptr->wstr_len + 1) * sizeof (wchar));
    SCI_MEMSET (label_ctrl_ptr->str_ptr, 0, ( (text_ptr->wstr_len + 1) *sizeof (wchar)));

    //copy string
    MMI_MEMCPY (label_ctrl_ptr->str_ptr, (text_ptr->wstr_len*sizeof (wchar)),
                text_ptr->wstr_ptr, (text_ptr->wstr_len*sizeof (wchar)),
                (text_ptr->wstr_len*sizeof (wchar)));
    label_ctrl_ptr->str_len = text_ptr->wstr_len;

    //replace LF/CR
    for (i = 0; i < label_ctrl_ptr->str_len; i++)
    {
        if ( (LF_CHAR == label_ctrl_ptr->str_ptr[i]) ||
                (CR_CHAR == label_ctrl_ptr->str_ptr[i]))
        {
            label_ctrl_ptr->str_ptr[i] = BL_CHAR;
        }
    }
}

/*****************************************************************************/
// 	Description : get label string
//	Global resource dependence :
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLLABEL_GetText (
    MMI_CTRL_ID_T   ctrl_id,    //in
    MMI_STRING_T    *text_ptr   //in/out
)
{
    BOOLEAN             result = FALSE;
    CTRLLABEL_OBJ_T		*label_ctrl_ptr = PNULL;

    //get label pointer by control id
    label_ctrl_ptr = GetLabelPtr (ctrl_id);

    if (PNULL != label_ctrl_ptr)
    {
        text_ptr->wstr_ptr = label_ctrl_ptr->str_ptr;
        text_ptr->wstr_len = label_ctrl_ptr->str_len;
    }

    return (result);
}

/*****************************************************************************/
// 	Description : set label string
//	Global resource dependence :
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLLABEL_SetText (
    MMI_CTRL_ID_T   ctrl_id,    //in
    MMI_STRING_T    *text_ptr,  //in
    BOOLEAN         is_update   //in
)
{
    BOOLEAN             result = FALSE;
    CTRLLABEL_OBJ_T		*label_ctrl_ptr = PNULL;

    //get label pointer by control id
    label_ctrl_ptr = GetLabelPtr (ctrl_id);

    if (PNULL != label_ctrl_ptr)
    {
        //set text string by pointer
        result = SetLabelTextByPtr (label_ctrl_ptr, text_ptr, is_update);
    }

    return (result);
}

/*****************************************************************************/
// 	Description : set label string by id
//	Global resource dependence :
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLLABEL_SetTextById (
    MMI_CTRL_ID_T   ctrl_id,    //in
    MMI_TEXT_ID_T   text_id,    //in
    BOOLEAN         is_update   //in
)
{
    BOOLEAN             result = FALSE;
    MMI_STRING_T        label_str = {0};
    CTRLLABEL_OBJ_T		*label_ctrl_ptr = PNULL;

    //get label pointer by control id
    label_ctrl_ptr = GetLabelPtr (ctrl_id);

    if (PNULL != label_ctrl_ptr)
    {
        MMITHEME_GetResText (text_id, label_ctrl_ptr->win_handle, &label_str);

        //set text string by pointer
        result = SetLabelTextByPtr (label_ctrl_ptr, &label_str, is_update);
    }

    return (result);
}

/*****************************************************************************/
// 	Description : set label string by id
//	Global resource dependence :
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLLABEL_SetTextRegion (
    MMI_CTRL_ID_T             ctrl_id,    //in:
    GUIFONT_REGION_STYLE_T    *region_ptr,//in:
    uint16                    region_num  //in:
)
{
    BOOLEAN             result = FALSE;
    uint32              region_size = 0;
    CTRLLABEL_OBJ_T		*label_ctrl_ptr = PNULL;

    //get label pointer by control id
    label_ctrl_ptr = GetLabelPtr (ctrl_id);

    if ( (PNULL != label_ctrl_ptr) &&
            (PNULL != region_ptr) &&
            (0 < region_num))
    {
        if (PNULL != label_ctrl_ptr->region_ptr)
        {
            SCI_FREE (label_ctrl_ptr->region_ptr);
            label_ctrl_ptr->region_num = 0;
        }

        region_size = region_num * sizeof (GUIFONT_REGION_STYLE_T);
        label_ctrl_ptr->region_ptr = SCI_ALLOCA (region_size);

        if (PNULL != label_ctrl_ptr->region_ptr)
        {
            SCI_MEMSET (label_ctrl_ptr->region_ptr, 0, region_size);
            MMI_MEMCPY (label_ctrl_ptr->region_ptr, region_size,
                        region_ptr, region_size,
                        region_size);

            label_ctrl_ptr->region_num = region_num;

            result = TRUE;
        }
    }

    return (result);
}

/*****************************************************************************/
// 	Description : set label icon
//	Global resource dependence :
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLLABEL_SetIcon (
    MMI_CTRL_ID_T   ctrl_id,//in
    MMI_IMAGE_ID_T  icon_id //in
)
{
    BOOLEAN             result = FALSE;
    CTRLLABEL_OBJ_T		*label_ctrl_ptr = PNULL;

    //get label pointer by control id
    label_ctrl_ptr = GetLabelPtr (ctrl_id);

    if (PNULL != label_ctrl_ptr)
    {
        //set icon
        label_ctrl_ptr->icon_id = icon_id;

        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
// 	Description : set label icon Ex
//	Global resource dependence :
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLLABEL_SetIconEx (
    MMI_CTRL_ID_T   ctrl_id,//in
    BOOLEAN is_bit_map,//IN
    GUILABEL_IMAGE_TYPE_E img_type,//IN
    GUILABEL_DATA_IMAGE_U img_data//IN
)
{
    BOOLEAN result = FALSE;
    CTRLLABEL_OBJ_T *label_ctrl_ptr = PNULL;

    //get label pointer by control id
    label_ctrl_ptr = GetLabelPtr (ctrl_id);

    if (PNULL != label_ctrl_ptr)
    {
        //set icon
        label_ctrl_ptr->is_bit_map = is_bit_map;
        label_ctrl_ptr->img_type = img_type;

        if (GUILABEL_IMAGE_RES == img_type)
        {
            label_ctrl_ptr->img_data.res.id = img_data.res.id;
        }
        else if (GUILABEL_IMAGE_BUF == img_type)
        {
            label_ctrl_ptr->img_data.buf.size = img_data.buf.size;
            label_ctrl_ptr->img_data.buf.buf_ptr = SCI_ALLOCA (img_data.buf.size);

            if (PNULL == label_ctrl_ptr->img_data.buf.buf_ptr)
            {
                return FALSE;
            }

            SCI_MEMSET (label_ctrl_ptr->img_data.buf.buf_ptr, 0x00, img_data.buf.size);
            SCI_MEMCPY (label_ctrl_ptr->img_data.buf.buf_ptr, img_data.buf.buf_ptr, img_data.buf.size);

            label_ctrl_ptr->img_data.buf.width = img_data.buf.width;
            label_ctrl_ptr->img_data.buf.height = img_data.buf.height;
        }

        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
// 	Description : set label margin
//	Global resource dependence :
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLLABEL_SetMargin (
    MMI_CTRL_ID_T ctrl_id,    //in
    uint16        margin_space//in
)
{
    BOOLEAN             result = FALSE;
    CTRLLABEL_OBJ_T		*label_ctrl_ptr = PNULL;

    //get label pointer by control id
    label_ctrl_ptr = GetLabelPtr (ctrl_id);

    if (PNULL != label_ctrl_ptr)
    {
        //set bg
        label_ctrl_ptr->theme.margin_lr_space = margin_space;
        label_ctrl_ptr->theme.margin_tb_space = margin_space;

        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
// 	Description : set label margin
//	Global resource dependence :
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLLABEL_SetMarginEx (
    MMI_CTRL_ID_T   ctrl_id,    //in
    uint16          margin_lr,  //in:left and right margin
    uint16          margin_tb   //in:top and bottom margin
)
{
    BOOLEAN             result = FALSE;
    CTRLLABEL_OBJ_T		*label_ctrl_ptr = PNULL;

    //get label pointer by control id
    label_ctrl_ptr = GetLabelPtr (ctrl_id);

    if (PNULL != label_ctrl_ptr)
    {
        //set bg
        label_ctrl_ptr->theme.margin_lr_space = margin_lr;
        label_ctrl_ptr->theme.margin_tb_space = margin_tb;

        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
// 	Description : set label background
//	Global resource dependence :
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLLABEL_SetBg (
    MMI_CTRL_ID_T ctrl_id,//in
    GUI_BG_T      *bg_ptr //in
)
{
    BOOLEAN             result = FALSE;
    CTRLLABEL_OBJ_T		*label_ctrl_ptr = PNULL;

    //get label pointer by control id
    label_ctrl_ptr = GetLabelPtr (ctrl_id);

    if (PNULL != label_ctrl_ptr)
    {
        //set bg
        label_ctrl_ptr->theme.bg = *bg_ptr;

        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
// 	Description : set label background color
//	Global resource dependence :
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLLABEL_SetBackgroundColor (
    MMI_CTRL_ID_T    ctrl_id,
    GUI_COLOR_T      bg_color
)
{
    BOOLEAN             result = FALSE;
    CTRLLABEL_OBJ_T		*label_ctrl_ptr = PNULL;

    //get label pointer by control id
    label_ctrl_ptr = GetLabelPtr (ctrl_id);

    if (PNULL != label_ctrl_ptr)
    {
        //set bg color
        label_ctrl_ptr->theme.bg.bg_type = GUI_BG_COLOR;
        label_ctrl_ptr->theme.bg.color   = bg_color;

        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
// 	Description : set label font
//	Global resource dependence :
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLLABEL_SetFont (
    MMI_CTRL_ID_T   ctrl_id,    //in:
    GUI_FONT_T      font,       //in:
    GUI_COLOR_T     font_color  //in:
)
{
    BOOLEAN             result = FALSE;
    CTRLLABEL_OBJ_T		*label_ctrl_ptr = PNULL;

    //get label pointer by control id
    label_ctrl_ptr = GetLabelPtr (ctrl_id);

    if (PNULL != label_ctrl_ptr)
    {
        //set font
        label_ctrl_ptr->theme.font.font  = font;
        label_ctrl_ptr->theme.font.color = font_color;

        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
// 	Description : get label rect
//	Global resource dependence :
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLLABEL_GetRect (
    MMI_CTRL_ID_T   ctrl_id,    //in:
    GUI_RECT_T      *rect_ptr   //in/out:
)
{
    BOOLEAN             result = FALSE;
    CTRLLABEL_OBJ_T		*label_ctrl_ptr = PNULL;
    CTRLBASE_OBJ_T      *base_ctrl_ptr = PNULL;

    //get label pointer by control id
    label_ctrl_ptr = GetLabelPtr (ctrl_id);

    if (PNULL != label_ctrl_ptr)
    {
        base_ctrl_ptr = (CTRLBASE_OBJ_T*) label_ctrl_ptr;

        //set rect
        *rect_ptr = base_ctrl_ptr->rect;

        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
// 	Description : set label rect
//	Global resource dependence :
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLLABEL_SetRect (
    MMI_CTRL_ID_T   ctrl_id,    //in:
    GUI_RECT_T      *rect_ptr,  //in:
    BOOLEAN         is_update   //in
)
{
    BOOLEAN             result = FALSE;
    CTRLLABEL_OBJ_T		*label_ctrl_ptr = PNULL;

    //get label pointer by control id
    label_ctrl_ptr = GetLabelPtr (ctrl_id);

    if (PNULL != label_ctrl_ptr)
    {
        result = LabelCtrlSetRect (label_ctrl_ptr, rect_ptr);

        if ( (is_update) &&
                (MMK_IsFocusWin (label_ctrl_ptr->win_handle)))
        {
            //update display
            LabelDraw (label_ctrl_ptr);
        }

        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
// 	Description : set label tp rect
//	Global resource dependence :
//  Author: Jassmine
//	Note: only for form
/*****************************************************************************/
PUBLIC BOOLEAN CTRLLABEL_SetTpRect (
    MMI_CTRL_ID_T ctrl_id,    //in:
    GUI_RECT_T    *rect_ptr   //in/out:
)
{
    BOOLEAN             result = FALSE;
    CTRLLABEL_OBJ_T		*label_ctrl_ptr = PNULL;

    //get label pointer by control id
    label_ctrl_ptr = GetLabelPtr (ctrl_id);

    if ( (PNULL != label_ctrl_ptr) &&
            (PNULL != rect_ptr))
    {
        label_ctrl_ptr->tp_rect = *rect_ptr;
        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
// 	Description : set label is visible
//	Global resource dependence :
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLLABEL_SetVisible (
    MMI_CTRL_ID_T    ctrl_id,
    BOOLEAN          is_visible,
    BOOLEAN          is_update
)
{
    BOOLEAN             result = FALSE;
    CTRLLABEL_OBJ_T		*label_ctrl_ptr = PNULL;
    CTRLBASE_OBJ_T      *base_ctrl_ptr = PNULL;

    //get label pointer by control id
    label_ctrl_ptr = GetLabelPtr (ctrl_id);

    if (PNULL != label_ctrl_ptr)
    {
        base_ctrl_ptr = (CTRLBASE_OBJ_T*) label_ctrl_ptr;

        //set visible
        IGUICTRL_SetState ( (IGUICTRL_T *) label_ctrl_ptr, GUICTRL_STATE_INVISIBLE, (BOOLEAN) (!is_visible));

        if ( (is_update) &&
                (MMK_IsFocusWin (label_ctrl_ptr->win_handle)))
        {
            if (is_visible)
            {
                //update display
                LabelDraw (label_ctrl_ptr);
            }
            else
            {
                //notify parent update bg
                CTRLMSG_SendNotifyEx (base_ctrl_ptr->handle, MSG_NOTIFY_UPDATE, &base_ctrl_ptr->rect);
            }
        }

        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
// 	Description : flush label backgroud buffer
//	Global resource dependence :
//  Author: feng.xiao
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLLABEL_FlushBg (
    MMI_CTRL_ID_T   ctrl_id
)
{
    CTRLLABEL_OBJ_T*    label_ptr = (CTRLLABEL_OBJ_T*) GetLabelPtr (ctrl_id);

    if (label_ptr)
    {
        //free bg buffer
        if (label_ptr->bg_buf_ptr)
        {
            SCI_FREE (label_ptr->bg_buf_ptr);
            label_ptr->bg_buf_ptr = PNULL;
            return TRUE;
        }
    }

    return FALSE;
}

/*****************************************************************************/
//  Description : set label align
//  Parameter: [In] ctrl_id : ctrl id
//             [In] align : align
//             [Out] None
//             [Return] result :TRUE(set success) FALSE(set failed)
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLLABEL_SetAlign (
        MMI_CTRL_ID_T   ctrl_id,
        GUILABEL_ALIGN_E align
)
{
    BOOLEAN             result = FALSE;
    CTRLLABEL_OBJ_T		*label_ctrl_ptr = PNULL;

    //get label pointer by control id
    label_ctrl_ptr = GetLabelPtr (ctrl_id);

    if (PNULL != label_ctrl_ptr)
    {
        //set bg
        label_ctrl_ptr->align= align;

        result = TRUE;
    }

    return FALSE;
}

/*****************************************************************************/
//  Description : string is auto scroll if string width > diaplay width whether label is active or not
//  Parameter: [In] ctrl_id : ctrl id
//             [In] is_auto_scroll : is_auto_scroll
//             [Out] None
//  Author: miao.liu2
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLLABEL_SetIsAutoScroll (
        MMI_CTRL_ID_T   ctrl_id,
        BOOLEAN         is_auto_scroll
)
{
    BOOLEAN             result = FALSE;
    CTRLLABEL_OBJ_T*  p_label_ctrl = GetLabelPtr (ctrl_id);
    if (PNULL != p_label_ctrl)
    {
        p_label_ctrl->is_auto_scroll= is_auto_scroll;
        result = TRUE;
    }

    return result;
}

/*****************************************************************************/
//  Description : label string is need Ellipsis
//  Parameter: [In] ctrl_id : ctrl id
//             [In] is_ellipsis_ex : is_ellipsis_ex
//             [Out] None
//  Author: miao.liu2
//  Note:当字符串宽度超过显示区域时，设置是否显示省略点
/*****************************************************************************/
PUBLIC BOOLEAN CTRLLABEL_SetIsEllipsisEx (
        MMI_CTRL_ID_T   ctrl_id,
        BOOLEAN           is_ellipsis_ex
)
{
    BOOLEAN             result = FALSE;
    CTRLLABEL_OBJ_T* p_label_ctrl = GetLabelPtr (ctrl_id);
    if (PNULL != p_label_ctrl)
    {
        p_label_ctrl->is_ellipsis_ex = is_ellipsis_ex;
        result = TRUE;
    }

    return result;
}

/*****************************************************************************/
// 	Description : set label text to icon margin
//	Global resource dependence :
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLLABEL_SetTextMargin (
    MMI_CTRL_ID_T ctrl_id,    //in
    uint16        margin_space//in
)
{
    BOOLEAN             result = FALSE;
    CTRLLABEL_OBJ_T		*label_ctrl_ptr = PNULL;

    //get label pointer by control id
    label_ctrl_ptr = GetLabelPtr (ctrl_id);

    if (PNULL != label_ctrl_ptr)
    {
        label_ctrl_ptr->theme.icon_space = margin_space;
        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : set label rect
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN LabelCtrlSetRect (
    CTRLBASE_OBJ_T       *base_ctrl_ptr,
    const CAF_RECT_T     *rect_ptr
)
{
    BOOLEAN             result = TRUE;
    CTRLLABEL_OBJ_T     *label_ctrl_ptr = PNULL;

    label_ctrl_ptr = (CTRLLABEL_OBJ_T *) base_ctrl_ptr;

    if (!GUI_EqualRect (base_ctrl_ptr->rect, *rect_ptr))
    {
        base_ctrl_ptr->rect         = *rect_ptr;
        base_ctrl_ptr->display_rect = *rect_ptr;

        GUI_SetVOrHRect (base_ctrl_ptr->handle, (GUI_RECT_T *) rect_ptr, & (base_ctrl_ptr->both_rect));

        //stop timer
        LabelStopScrollTimer (label_ctrl_ptr);

        //set start index
        label_ctrl_ptr->str_index = 0;
    }

    //free bg buffer
    if (PNULL != label_ctrl_ptr->bg_buf_ptr)
    {
        SCI_FREE (label_ctrl_ptr->bg_buf_ptr);
    }

    return (result);
}

/*****************************************************************************/
//  Description : set label border
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN LabelCtrlSetBorder (
    CTRLBASE_OBJ_T     *base_ctrl_ptr,
    GUI_BORDER_T       *border_ptr
)
{
    BOOLEAN             result = TRUE;
    CTRLLABEL_OBJ_T     *label_ctrl_ptr = PNULL;

    label_ctrl_ptr = (CTRLLABEL_OBJ_T *) base_ctrl_ptr;

    //set border
    label_ctrl_ptr->theme.border = *border_ptr;

    return (result);
}

/*****************************************************************************/
//  Description : set label bg
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN LabelCtrlSetBg (
    CTRLBASE_OBJ_T     *base_ctrl_ptr,
    GUI_BG_T           *bg_ptr
)
{
    BOOLEAN             result = TRUE;
    CTRLLABEL_OBJ_T     *label_ctrl_ptr = PNULL;

    label_ctrl_ptr = (CTRLLABEL_OBJ_T *) base_ctrl_ptr;

    //set bg
    label_ctrl_ptr->theme.bg = *bg_ptr;

    return (result);
}

/*****************************************************************************/
//  Description : set label font
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN LabelCtrlSetFont (
    CTRLBASE_OBJ_T       *base_ctrl_ptr,
    GUI_FONT_ALL_T       *font_ptr
)
{
    BOOLEAN             result = TRUE;
    CTRLLABEL_OBJ_T     *label_ctrl_ptr = PNULL;

    label_ctrl_ptr = (CTRLLABEL_OBJ_T *) base_ctrl_ptr;

    //set font
    label_ctrl_ptr->theme.font = *font_ptr;

    return (result);
}

/*****************************************************************************/
//  Description : get label font
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN LabelCtrlGetFont (
    CTRLBASE_OBJ_T       *base_ctrl_ptr,
    GUI_FONT_ALL_T       *font_ptr
)
{
    BOOLEAN             result = TRUE;
    CTRLLABEL_OBJ_T     *label_ctrl_ptr = PNULL;

    label_ctrl_ptr = (CTRLLABEL_OBJ_T *) base_ctrl_ptr;

    //set font
    *font_ptr = label_ctrl_ptr->theme.font;

    return (result);
}

/*****************************************************************************/
//  Description : get label height by width,include border,text etc
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN LabelCtrlGetHeightByWidth (
    CTRLBASE_OBJ_T  *base_ctrl_ptr,  //in:
    uint16          width,      //in:
    uint16          *height_ptr //in/out:
)
{
    BOOLEAN             result = TRUE;
    BOOLEAN             is_border = TRUE;
    uint16              font_height = 0;
    uint16              img_width = 0;
    uint16              img_height = 0;
    uint16              line_height = 0;
    CTRLLABEL_OBJ_T     *label_ctrl_ptr = PNULL;
    MMI_IMAGE_ID_T icon_id = 0;

    label_ctrl_ptr = (CTRLLABEL_OBJ_T *) base_ctrl_ptr;

    //label is exist border
    if (GUI_BORDER_NONE == label_ctrl_ptr->theme.border.type)
    {
        is_border = FALSE;
    }

    icon_id = label_ctrl_ptr->icon_id;

    if (!MMITHEME_CheckImageID (icon_id)
            && GUILABEL_IMAGE_RES == label_ctrl_ptr->img_type)
    {
        icon_id = label_ctrl_ptr->img_data.res.id;
    }

    //get font height
    font_height = GUI_GetFontHeight (label_ctrl_ptr->theme.font.font, UNICODE_HANZI);

    if (MMITHEME_CheckImageID (icon_id))
    {
        //get image height
        GUIRES_GetImgWidthHeight (PNULL, &img_height, icon_id, label_ctrl_ptr->win_handle);
    }
    else if (GUILABEL_IMAGE_BUF == label_ctrl_ptr->img_type)
    {
        if (label_ctrl_ptr->is_bit_map)
        {
            img_width = label_ctrl_ptr->img_data.buf.width;
            img_height = label_ctrl_ptr->img_data.buf.height;
        }
        else
        {
            GUIRES_GetImgWidthHeightByPtr (label_ctrl_ptr->img_data.buf.buf_ptr,
                                           &img_width, &img_height,
                                           label_ctrl_ptr->img_data.buf.size);
        }
    }

    font_height = (uint16) MAX (font_height, img_height);

    //get line height
    line_height = (uint16) (font_height + (label_ctrl_ptr->theme.margin_tb_space << 1));
    *height_ptr = line_height;

    //add border
    if (is_border)
    {
        *height_ptr = (uint16) (*height_ptr + (label_ctrl_ptr->theme.border.width << 1));
    }

    GUI_INVALID_PARAM (width);/*lint !e522*/

    return (result);
}

/*****************************************************************************/
//  Description : get label display rect
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN LabelCtrlGetDisplayRect (
    CTRLBASE_OBJ_T    *base_ctrl_ptr,  //in:
    GUI_RECT_T        *rect_ptr   //in/out:
)
{
    BOOLEAN             result = FALSE;
    CTRLLABEL_OBJ_T     *label_ctrl_ptr = PNULL;

    label_ctrl_ptr = (CTRLLABEL_OBJ_T *) base_ctrl_ptr;

    if (PNULL != rect_ptr)
    {
        if (GUI_IsRectEmpty (label_ctrl_ptr->tp_rect))
        {
            *rect_ptr = base_ctrl_ptr->display_rect;
        }
        else
        {
            *rect_ptr = label_ctrl_ptr->tp_rect;
        }

        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : set label display rect
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN LabelCtrlSetDisplayRect (
    CTRLBASE_OBJ_T        *base_ctrl_ptr,  //in:
    const GUI_RECT_T      *rect_ptr,  //in:
    BOOLEAN               is_update   //in:
)
{
    BOOLEAN             result = TRUE;
    CTRLLABEL_OBJ_T     *label_ctrl_ptr = PNULL;

    label_ctrl_ptr = (CTRLLABEL_OBJ_T *) base_ctrl_ptr;

    //set display rect
    base_ctrl_ptr->display_rect = *rect_ptr;

    if (is_update)
    {
        //update label
        LabelDraw (label_ctrl_ptr);
    }

    return (result);
}

/*Edit by script, ignore 1 case. Thu Apr 26 19:01:14 2012*/ //IGNORE9527
