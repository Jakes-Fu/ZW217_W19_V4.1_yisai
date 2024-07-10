/*****************************************************************************
** File Name:      ctrlstatusbar.c                                               *
** Author:                                                                   *
** Date:              09/2012                                                *
** Copyright:      2004 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 09/2012     xiyuan.ma              Creat
******************************************************************************/

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_gui_trc.h"
#include "mmi_default.h"
#include "mmitheme_statusbar.h"
#include "mmk_type.h"
#include "mmk_app.h"
#include "mmk_timer.h"
#include "mmk_msg.h"
#include "mmi_theme.h"
#include "guires.h"
#include "guilcd.h"
#include "guistring.h"
#include "guicommon.h"

#include "ctrlbase.h"
#include "ctrlstatusbar.h"
#include "ctrlstatusbar_data.h"
#include "ctrlstatusbar_scrollmsg.h"
#include "ctrlstatusbar_export.h"
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define CTRLSTB_ITEM_LEFT_RIGHT_MARGEN  DP2PX_VALUE(2)//左右图片距离屏幕边界的左右距离
#define CTRLSTB_ITEM_TOP_BOTTON_MARGEN  DP2PX_VALUE(1)//图片距离屏幕上下边界的距离
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
//  Description : init statusbar class
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN StatusbarCtrlInitVtbl (
    CTRLSTATUSBAR_VTBL_T        *statusbar_vtbl_ptr
);


/*****************************************************************************/
//  Description : statusbar construct
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN StatusbarCtrlConstruct (
    CTRLBASE_OBJ_T           *base_ctrl_ptr,
    CTRLSTATUSBAR_INIT_PARAM_T   *statusbar_param_ptr
);


/*****************************************************************************/
//  Description : statusbar destruct
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN StatusbarCtrlDestruct (
    CTRLBASE_OBJ_T        *base_ctrl_ptr
);


/*****************************************************************************/
//  Description : convert init param
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void StatusbarCtrlPackInitParam (
    GUISTATUSBAR_INIT_DATA_T     *statusbar_init_ptr,   //in
    CTRLSTATUSBAR_INIT_PARAM_T   *statusbar_param_ptr   //out
);


/*****************************************************************************/
//  Description : handle statusbar msg function
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E StatusbarCtrlHandleMsg (
    CTRLBASE_OBJ_T        *base_ctrl_ptr,   //control pointer
    MMI_MESSAGE_ID_E      msg_id,           //message
    DPARAM                param             //add data
);


/*****************************************************************************/
//  Description : get statusbar pointer by control id
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL CTRLSTATUSBAR_OBJ_T* GetStatusbarPtr (
    MMI_HANDLE_T     ctrl_handle
);


/*****************************************************************************/
//  Description : ext type of
//  Global resource dependence :
//  Author:hua.fang
//  Note: judge control type of
/*****************************************************************************/
LOCAL BOOLEAN StatusbarTypeOf (
    CTRLBASE_OBJ_T     *ctrl_ptr
);


/*****************************************************************************/
// Description : 显示滚动消息
// Global resource dependence :
// Author: bin.ji
// Note:
/*****************************************************************************/
LOCAL void DrawStatusBarScrollItem (
    CTRLSTATUSBAR_OBJ_T     *statusbar_ctrl_ptr,
    GUI_RECT_T rect,
    BOOLEAN is_next_item
);


/*****************************************************************************/
// Description : 停止timer
// Global resource dependence :
// Author: bin.ji
// Note:
/*****************************************************************************/
LOCAL void StopStatusBarScrollMsgTimer (
    CTRLSTATUSBAR_OBJ_T     *statusbar_ctrl_ptr
);



/*****************************************************************************/
// Description : 处理控件的定时器
// Global resource dependence :
// Author: xiaoqing.lu
// Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleTimer (
    CTRLSTATUSBAR_OBJ_T     *statusbar_ctrl_ptr,
    uint8               timer_id
);


/*****************************************************************************/
// Description : 重启控件的定时器
// Global resource dependence :
// Author: xiaoqing.lu
// Note:
/*****************************************************************************/
LOCAL void StartStatusBarTimer (
    CTRLSTATUSBAR_OBJ_T     *statusbar_ctrl_ptr
);


#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
/*****************************************************************************/
// Description : 处理控件的案件消息
// Global resource dependence :
// Author: xiaoqing.lu
// Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleTPKeyUp (
    CTRLSTATUSBAR_OBJ_T     *statusbar_ctrl_ptr,
    GUI_POINT_T           point
);
#endif


/*****************************************************************************/
// Description : 显示控件
// Global resource dependence :
// Author: Xiaoqing.Lu
// Note:
/*****************************************************************************/
LOCAL void DrawStatusBarCtrl (
    CTRLSTATUSBAR_OBJ_T     *statusbar_ctrl_ptr
);


/*****************************************************************************/
// Description : 计算控件项item的区域，不需要显示的item不计算，最后一个默认显示在最后位置
// Global resource dependence :
// Author: Xiaoqing.lu
// Note:
/*****************************************************************************/
LOCAL BOOLEAN CalculateItemRect (
    CTRLSTATUSBAR_OBJ_T     *statusbar_ctrl_ptr
);


/*****************************************************************************/
// Description : 重新设置区域
// Global resource dependence :
// Author: Xiaoqing.lu
// Note:
/*****************************************************************************/
LOCAL void ResetAllItemRect (
    CTRLSTATUSBAR_OBJ_T     *statusbar_ctrl_ptr
);


/*****************************************************************************/
// Description : 计算Istyle控件项item的区域，不需要显示的item不计算，最后一个默认显示在最后位置
// Global resource dependence :
// Author: xiyuan.ma
// Note:
/*****************************************************************************/
LOCAL BOOLEAN CalculateIstyleItemRect (
    CTRLSTATUSBAR_OBJ_T     *statusbar_ctrl_ptr

);


/*****************************************************************************/
// Description : 画背景
// Global resource dependence :
// Author: Xiaoqing.lu
// Note: the return value means this function draw backgroung or not
/*****************************************************************************/
LOCAL BOOLEAN DrawStatusBarBg (
    CTRLSTATUSBAR_OBJ_T     *statusbar_ctrl_ptr,
    GUI_RECT_T bg_rect,
    BOOLEAN is_week_layer
);


/*****************************************************************************/
// Description : 显示控件和相关的图标
// Global resource dependence :
// Author: xiaoqing.lu
// Note:
/*****************************************************************************/
LOCAL void DrawStatusBarAllItem (
    CTRLSTATUSBAR_OBJ_T     *statusbar_ctrl_ptr,
    int16 offset_y
);


/*****************************************************************************/
// Description : 停止timer
// Global resource dependence :
// Author: xiaoqing.lu
// Note:
/*****************************************************************************/
LOCAL void StopStatusBarTimer (
    CTRLSTATUSBAR_OBJ_T     *statusbar_ctrl_ptr
);


/*****************************************************************************/
// Description : 处理状态栏的消息MSG_CTL_STATUSBAR_UPDATE
// Global resource dependence :
// Author: xiaoqing.lu
// Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E RefreshStatusBar (
    CTRLSTATUSBAR_OBJ_T     *statusbar_ctrl_ptr,
    BOOLEAN is_full_paint
);


/*****************************************************************************/
//  Description : 获取text的长度，计算到每个象素
//  Global resource dependence :
//  Author:  xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL uint32 GetTextLength (
    GUISTBDATA_TEXT_T *text_info_ptr
);


/*****************************************************************************/
//  Description : set status bar rect
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN StatusBarSetRect (
    CTRLBASE_OBJ_T    *base_ctrl_ptr,
    const CAF_RECT_T  *rect_ptr
);

/*****************************************************************************/
// Description : get lcd dev info
// Global resource dependence :
// Author: James.Zhang
// Note: get lcd dev info
/*****************************************************************************/
LOCAL GUI_LCD_DEV_INFO GetStatusBarLcdDevInfo (
    CTRLSTATUSBAR_OBJ_T     *statusbar_ctrl_ptr
);

#ifdef UI_MULTILAYER_SUPPORT
/****************************************************************************/
//  Description :create layer for stb
//  Global resource dependence :
//  Author: jixin.xu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CreateCtrlLayer (
    CTRLSTATUSBAR_OBJ_T* statusbar_ctrl_ptr
);


/****************************************************************************/
//  Description :release stb layer
//  Global resource dependence :
//  Author: jixin.xu
//  Note:
/*****************************************************************************/
LOCAL void ReleaseCtrlLayer (
    CTRLSTATUSBAR_OBJ_T* statusbar_ctrl_ptr
);
#endif

#ifdef GUIF_STATUSBAR_SCROLL
/*****************************************************************************/
// Description : 显示控件和相关的图标
// Global resource dependence :
// Author: bin.ji
// Note:
/*****************************************************************************/
LOCAL void DrawStatusBarScrollMsg (
    CTRLSTATUSBAR_OBJ_T            *statusbar_ctrl_ptr,
    GUISTBSCROLLMSG_SCROLL_STATE_E cur_scroll_state
);

#endif
/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/
LOCAL CTRLBASE_VTBL_T    *parent_vtbl_ptr = PNULL;


/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : 获取状态
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL __inline BOOLEAN GetState (
    uint32 src_state,
    uint32 aim_state
)
{
    if ( (src_state&aim_state) != 0x0)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}


/*****************************************************************************/
//  Description : 设置状态
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL __inline void SetState (
    uint32 *src_state_ptr,
    uint32 aim_state,
    BOOLEAN is_set
)
{
    if (is_set)
    {
        *src_state_ptr |= aim_state;
    }
    else
    {
        *src_state_ptr &= ~aim_state;
    }

    return;
}


/*****************************************************************************/
//  Description : init statusbar class
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN StatusbarCtrlInitVtbl (
    CTRLSTATUSBAR_VTBL_T        *statusbar_vtbl_ptr
)
{
    CTRLBASE_VTBL_T  *base_ctrl_vtbl_ptr = (CTRLBASE_VTBL_T*) statusbar_vtbl_ptr;

    base_ctrl_vtbl_ptr->HandleEvent = StatusbarCtrlHandleMsg;
    base_ctrl_vtbl_ptr->SetRect = StatusBarSetRect;

    return TRUE;
}

/*****************************************************************************/
//  Description : convert init param
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void StatusbarCtrlPackInitParam (
    GUISTATUSBAR_INIT_DATA_T     *statusbar_init_ptr,
    CTRLSTATUSBAR_INIT_PARAM_T   *statusbar_param_ptr
)
{
    CTRLBASE_INIT_PARAM_T   *base_ctrl_param_ptr = (CTRLBASE_INIT_PARAM_T*) statusbar_param_ptr;

    base_ctrl_param_ptr->both_rect = statusbar_init_ptr->both_rect;

    return;
}

/*****************************************************************************/
//  Description : statusbar construct
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN StatusbarCtrlConstruct (
    CTRLBASE_OBJ_T *base_ctrl_ptr,
    CTRLSTATUSBAR_INIT_PARAM_T *statusbar_param_ptr
)
{
    BOOLEAN result = TRUE;
    CTRLSTATUSBAR_OBJ_T *statusbar_ctrl_ptr = (CTRLSTATUSBAR_OBJ_T*) base_ctrl_ptr;
    GUISTATUSBAR_INIT_DATA_T *init_ptr = (GUISTATUSBAR_INIT_DATA_T *) statusbar_param_ptr;

    if (PNULL != base_ctrl_ptr && PNULL != statusbar_param_ptr)
    {
        MMITHEME_GetStatusBarStyle (&statusbar_ctrl_ptr->style_info);

        base_ctrl_ptr->lcd_dev_info = * (MMITHEME_GetDefaultLcdDev());
        base_ctrl_ptr->bg  = MMITHEME_GetCommonBg();
        base_ctrl_ptr->both_rect = init_ptr->both_rect;

        statusbar_ctrl_ptr->is_display_bg = TRUE;
        statusbar_ctrl_ptr->is_shown = FALSE;
        statusbar_ctrl_ptr->clear_customer_bg = FALSE;

        IGUICTRL_SetState (MMK_GetCtrlPtr(base_ctrl_ptr->handle), GUICTRL_STATE_DISABLE_ACTIVE, TRUE);
        SetState (&statusbar_ctrl_ptr->ctrl_state, GUISTATUSBAR_STATE_EVENT, FALSE);
    }
    else
    {
        result = FALSE;
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, GUISTATUSBAR_1900_112_2_18_3_21_36_341, (uint8*) "");
    }

    return (result);
}


/*****************************************************************************/
//  Description : statusbar destruct
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN StatusbarCtrlDestruct (
    CTRLBASE_OBJ_T    *base_ctrl_ptr
)
{
    BOOLEAN result = TRUE;
    CTRLSTATUSBAR_OBJ_T *statusbar_ctrl_ptr = (CTRLSTATUSBAR_OBJ_T*) base_ctrl_ptr;

    if (PNULL == base_ctrl_ptr)
    {
        return FALSE;
    }

#ifdef UI_MULTILAYER_SUPPORT
    ReleaseCtrlLayer (statusbar_ctrl_ptr);
#endif
    StopStatusBarTimer (statusbar_ctrl_ptr);
    StopStatusBarScrollMsgTimer (statusbar_ctrl_ptr);

    return (result);
}



/*****************************************************************************/
//  Description : get statusbar pointer by control id
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL CTRLSTATUSBAR_OBJ_T* GetStatusbarPtr (
    MMI_HANDLE_T     ctrl_handle
)
{
    CTRLBASE_OBJ_T  *ctrl_ptr = (CTRLBASE_OBJ_T*) MMK_GetCtrlPtr (ctrl_handle);

    if (PNULL != ctrl_ptr)
    {
        SCI_ASSERT (StatusbarTypeOf (ctrl_ptr)); /*assert verified*/
    }

    return (CTRLSTATUSBAR_OBJ_T*) ctrl_ptr;
}


/*****************************************************************************/
//  Description : ext type of
//  Global resource dependence :
//  Author:hua.fang
//  Note: judge control type of
/*****************************************************************************/
LOCAL BOOLEAN StatusbarTypeOf (
    CTRLBASE_OBJ_T     *ctrl_ptr
)
{
    return TYPEMNG_IsTypeOf (ctrl_ptr, CTRL_STATUSBAR_TYPE);
}


/*****************************************************************************/
// Description : 重启控件的定时器
// Global resource dependence :
// Author: xiaoqing.lu
// Note:
/*****************************************************************************/
LOCAL void StartStatusBarTimer (
    CTRLSTATUSBAR_OBJ_T* statusbar_ctrl_ptr
)
{
    uint32                                     item_index = 0;
    uint32                                     item_total_num = 0;
    GUISTBDATA_ICON_STYLE_E  show_type  = GUISTBDATA_ICON_NONE;
    GUISTBDATA_ITEM_DATA_T    *item_data_ptr = PNULL;

    if (PNULL != statusbar_ctrl_ptr && 0 == statusbar_ctrl_ptr->timer_id)
    {
        item_total_num = CTRLSTBDATA_GetItemTotalNum();

        for (item_index = 0; item_index < item_total_num; item_index++)
        {
            item_data_ptr = CTRLSTBDATA_GetItemData (item_index);

            if (PNULL != item_data_ptr)
            {
                show_type = item_data_ptr->icon_info.icon_type;

                if (CTRLSTBDATA_IsItemVisible (item_index)
                        && !CTRLSTBDATA_IsItemDisable (item_index)
                        && (GUISTBDATA_ICON_ANIM == show_type)
                        && MMIDEFAULT_GetBackLightState())
                {
                    statusbar_ctrl_ptr->timer_id = MMK_CreateWinTimer (statusbar_ctrl_ptr->base_ctrl.handle, statusbar_ctrl_ptr->style_info.timer_out, FALSE);
                    break;
                }
            }
        }
    }

    return;
}


/*****************************************************************************/
//  Description : Start statusbar scrollmsg timer
//  Global resource dependence : none
//  Author: bin.ji
//  Note:
/*****************************************************************************/
LOCAL void StartStatusBarScrollMsgTimer (
    CTRLSTATUSBAR_OBJ_T* statusbar_ctrl_ptr,
    BOOLEAN is_pause
)
{
    uint32 timeout = 0;

    if (PNULL != statusbar_ctrl_ptr)
    {	
		if (is_pause)
		{
			timeout = statusbar_ctrl_ptr->style_info.scrollmsg_pause_timer_out;
		}
		else
		{
			timeout = statusbar_ctrl_ptr->style_info.scrollmsg_timer_out;
		}

		if (0 == statusbar_ctrl_ptr->scroll_msg_timer_id)
		{
			statusbar_ctrl_ptr->scroll_msg_timer_id = MMK_CreateWinTimer (statusbar_ctrl_ptr->base_ctrl.handle, timeout/*200*/, FALSE);
		}
    }
    return;
}


/*****************************************************************************/
//  Description : 获取text的长度，计算到每个象素
//  Global resource dependence :
//  Author:  xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL uint32 GetTextLength (
    GUISTBDATA_TEXT_T *text_info_ptr
)
{
    uint32   text_len = 0;
    GUISTR_STYLE_T str_style = {0};
    MMI_STRING_T   str_info = {0};
#ifndef SCRIPTEASY_SUPPORT
    GUISTR_STATE_T str_state = GUISTR_STATE_ALIGN | GUISTR_STATE_SINGLE_LINE | GUISTR_STATE_EFFECT|GUISTR_STATE_ALIGN_BY_DIR;
#else
    GUISTR_STATE_T str_state = GUISTR_STATE_ALIGN | GUISTR_STATE_SINGLE_LINE | GUISTR_STATE_EFFECT|GUISTR_STATE_ALIGN_BY_DIR|GUISTR_STATE_BOLD;
#endif

    if (PNULL != text_info_ptr)
    {
        str_info.wstr_len = text_info_ptr->wstr_len;
        str_info.wstr_ptr = text_info_ptr->wstr_ptr;

        str_style.align = text_info_ptr->align;
        str_style.font = text_info_ptr->font_type;
        str_style.font_color = text_info_ptr->font_color;
        str_style.char_space = text_info_ptr->font_space;
        str_style.effect = FONT_EFFECT_REVERSE_EDGE;

        text_len = GUISTR_GetStringWidth (&str_style, &str_info, str_state);
    }

    return text_len;
}




/*****************************************************************************/
//  Description : set status bar rect
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN StatusBarSetRect (
    CTRLBASE_OBJ_T    *base_ctrl_ptr,
    const CAF_RECT_T  *rect_ptr
)
{
    BOOLEAN                       result = FALSE;
    CTRLSTATUSBAR_OBJ_T *statusbar_ctrl_ptr = (CTRLSTATUSBAR_OBJ_T*) base_ctrl_ptr;

    if (!GUI_EqualRect (base_ctrl_ptr->rect, *rect_ptr))
    {
        base_ctrl_ptr->rect = *rect_ptr;

        GUI_SetVOrHRect (base_ctrl_ptr->handle, (GUI_RECT_T *) rect_ptr, & (base_ctrl_ptr->both_rect));

#ifdef UI_MULTILAYER_SUPPORT
        /* add for stb-layer, jixin.xu */
        if (GetState (statusbar_ctrl_ptr->ctrl_state, GUISTATUSBAR_STATE_USE_LAYER))
        {
            ReleaseCtrlLayer (statusbar_ctrl_ptr);
        }
#endif

        if (MMITHEME_IsIstyle())
        {
            CalculateIstyleItemRect (statusbar_ctrl_ptr);
        }
        else
        {
            CalculateItemRect (statusbar_ctrl_ptr);
        }

        result = TRUE;
    }

    return (result);
}


/*****************************************************************************/
// Description : 重新设置区域
// Global resource dependence :
// Author: Xiaoqing.lu
// Note:
/*****************************************************************************/
LOCAL void ResetAllItemRect (
    CTRLSTATUSBAR_OBJ_T* statusbar_ctrl_ptr
)
{
    uint16 item_index = 0;
    uint16 item_total_num = 0;
    GUI_RECT_T item_rect = {0};

    if (PNULL == statusbar_ctrl_ptr)
    {
        return;
    }

    item_total_num = CTRLSTBDATA_GetItemTotalNum();

    for (item_index = 0; item_index < item_total_num; item_index++)
    {
        CTRLSTBDATA_SetItemRect (item_index, item_rect);
    }

    return;
}


/*****************************************************************************/
// Description : 计算控件项item的区域，不需要显示的item不计算，最后一个默认显示在最后位置
// Global resource dependence :
// Author: Xiaoqing.lu
// Note:
/*****************************************************************************/
LOCAL BOOLEAN CalculateItemRect (
    CTRLSTATUSBAR_OBJ_T* statusbar_ctrl_ptr
)
{
    int32               i           = 0;
    uint16              item_index  = 0;
    uint16              total_width = 0;
    uint16              item_width  = 0;
    uint16              item_total_num = 0;
    uint16              right_item_num = 0;
    BOOLEAN             result = TRUE;
    MMI_IMAGE_ID_T      image_id       = 0;
    GUI_RECT_T          rect        = {0};
    GUI_RECT_T          item_rect   = {0};
    MMI_WIN_ID_T        win_id      = 0;
    GUISTBDATA_ITEM_DATA_T *item_data_ptr   = PNULL;
    uint32              temp_width  = 0;

    item_total_num = CTRLSTBDATA_GetItemTotalNum();

    // 入参检查
    if (PNULL == statusbar_ctrl_ptr || 0 == item_total_num)
    {
        return FALSE;
    }

    ResetAllItemRect (statusbar_ctrl_ptr);

    // get parent's win id
    win_id = (MMI_WIN_ID_T) MMK_GetWinHandleByCtrl (statusbar_ctrl_ptr->base_ctrl.handle);

    // 重新调整区域，去除边界值
    rect.left = statusbar_ctrl_ptr->base_ctrl.rect.left + CTRLSTB_ITEM_LEFT_RIGHT_MARGEN;
    rect.top = statusbar_ctrl_ptr->base_ctrl.rect.top;
    rect.right = statusbar_ctrl_ptr->base_ctrl.rect.right - CTRLSTB_ITEM_LEFT_RIGHT_MARGEN;
    rect.bottom = statusbar_ctrl_ptr->base_ctrl.rect.bottom;
    total_width = rect.right - rect.left + 1;

    if (GUI_IsInvalidRect (rect)) // 检查区域是否有效
    {
        return FALSE;
    }

    //从最后一个item开始计算rect，右侧的Item优先显示
    for (i = (int32) (item_total_num - 1); i >= 0; i--)
    {
        uint16 img_width   = 0;
        uint16 text_width  = 0;
        item_index = i;

        if (!CTRLSTBDATA_IsItemValid(item_index))
        {
            continue;
        }

        // get item data
        item_data_ptr = CTRLSTBDATA_GetItemData (item_index);

        if (PNULL != item_data_ptr)
        {
            // 这里判断，从右侧开始，遇到不是靠右显示的item，就直接返回。其他都是靠左显示
            if (!CTRLSTBDATA_IsItemRightAlign (item_index))
            {
                break;
            }

            CTRLSTBDATA_SetItemDisable (item_index, FALSE);

            if (CTRLSTBDATA_IsItemVisible (item_index))
            {
                if (PNULL != item_data_ptr->owner_draw_func)
                {
                    //owner draw max width = max width
                    temp_width = MMISTATUSBAR_ITEM_WIDTH;
                }
                else
                {
                    // 这里计算文本的宽度
                    if (item_data_ptr->text_info.is_display)
                    {
                        text_width = GetTextLength (&item_data_ptr->text_info);
                    }

                    // 这里计算图标的宽度
                    if (item_data_ptr->icon_info.is_display)
                    {
                        image_id = item_data_ptr->icon_info.icon_array[item_data_ptr->cur_frame];
                        GUIRES_GetImgWidthHeight (&img_width, PNULL, image_id, win_id);
                    }

                    temp_width = text_width + img_width;
                }

                item_width += temp_width;

                //calculate & save item rect
                item_rect.top    = rect.top;
                item_rect.bottom = rect.bottom;

                if (item_width > total_width)
                {
                    // 显示不下的情况下该item为无效
                    item_width -= temp_width;
                    item_rect.left  = 0;
                    item_rect.right = 0;
                    CTRLSTBDATA_SetItemDisable (item_index, TRUE);
                    CTRLSTBDATA_SetItemRect (item_index, item_rect);
                    break;
                }
                else
                {
                    if (rect.right >= item_width)
                    {
                        item_rect.left  = rect.right - (int16) item_width + 1;
                    }

                    if (temp_width > 0)
                    {
                        item_rect.right = item_rect.left + (int16) temp_width - 1;
                    }
                    else
                    {
                        item_rect.right = item_rect.left + (int16) temp_width;
                    }

                    CTRLSTBDATA_SetItemRect (item_index, item_rect);
                    item_width += statusbar_ctrl_ptr->style_info.item_space;
                }

            }

            right_item_num++;
            result = TRUE;
        }
        else
        {
            //SCI_TRACE_LOW:"CalculateItemRect 1 item_data_ptr = PNULL, %d"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, GUISTATUSBAR_653_112_2_18_3_21_34_331, (uint8*) "d", item_index);
        }
    }

    //开始计算左侧的区域
    item_total_num -= right_item_num;

    //剩余的宽度
    total_width -= item_width;

    //从左向右开始计算的时候，该值初始化为0
    item_width = 0;

    // 计算从第0个到第item_total_num - 2个item的区域,并设置是否有效
    for (item_index = 0; item_index < item_total_num; item_index++)
    {
        uint16 img_width   = 0;
        uint16 text_width  = 0;

        item_data_ptr = CTRLSTBDATA_GetItemData (item_index);

        if (!CTRLSTBDATA_IsItemValid(item_index))
        {
            continue;
        }

        if (CTRLSTBDATA_IsItemRightAlign (item_index))
        {
            continue;
        }
        if (PNULL != item_data_ptr)
        {
            // 设置所有的item的状态都是有效的图标
            CTRLSTBDATA_SetItemDisable (item_index, FALSE);

            // item是否可见
            if (CTRLSTBDATA_IsItemVisible (item_index))
            {
                if (PNULL != item_data_ptr->owner_draw_func)
                {
                    //owner draw max width = max width
                    temp_width = MMISTATUSBAR_ITEM_WIDTH;
                }
                else
                {
                    // 这里计算文本的宽度
                    if (item_data_ptr->text_info.is_display)
                    {
                        text_width = GetTextLength (&item_data_ptr->text_info);
                    }

                    // 这里计算图标的宽度
                    if (item_data_ptr->icon_info.is_display)
                    {
                        image_id = item_data_ptr->icon_info.icon_array[item_data_ptr->cur_frame];
                        GUIRES_GetImgWidthHeight (&img_width, PNULL, image_id, win_id);
                    }

                    temp_width = text_width + img_width;
                }

                //从左开始计算的时候，先加上用于判断是否越界
                item_width += temp_width;

                //calculate & save item rect
                item_rect.top = rect.top;
                item_rect.bottom = rect.bottom;

                if (item_width > total_width)
                {
                    //显示不下的情况下该item为无效
                    item_width -= temp_width;
                    item_rect.left  = 0;
                    item_rect.right = 0;
                    CTRLSTBDATA_SetItemDisable (item_index, TRUE);
                    CTRLSTBDATA_SetItemRect (item_index, item_rect);
                    break;
                }
                else
                {
                    item_width         -= temp_width;
                    item_rect.left      = rect.left + (int16) item_width + 1;

                    if (temp_width > 0)
                    {
                        item_rect.right = item_rect.left + (int16) temp_width - 1;
                    }

                    CTRLSTBDATA_SetItemRect (item_index, item_rect);
                    item_width         += (temp_width + statusbar_ctrl_ptr->style_info.item_space);
                }

                result = TRUE;
            }
        }
        else
        {
            //SCI_TRACE_LOW:"CalculateItemRect 2 item_data_ptr = PNULL, %d"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, GUISTATUSBAR_740_112_2_18_3_21_34_332, (uint8*) "d", item_index);
        }


    }

    return result;
}


/*****************************************************************************/
// Description : 计算Istyle控件项item的区域，不需要显示的item不计算，最后一个默认显示在最后位置
// Global resource dependence :
// Author: Xiaoqing.lu
// Note:
/*****************************************************************************/
LOCAL BOOLEAN CalculateIstyleItemRect (
    CTRLSTATUSBAR_OBJ_T* statusbar_ctrl_ptr
)
{
    int32               i              = 0;
    uint32              item_index     = 0;
    uint32              total_width    = 0;
    uint32              item_width     = 0;
    uint32              center_width   = 0;
    uint32              item_total_num = 0;
    uint32              right_item_num = 0;
    BOOLEAN             result         = TRUE;
    MMI_IMAGE_ID_T      image_id       = 0;
    GUI_RECT_T          rect           = {0};
    GUI_RECT_T          item_rect      = {0};
    uint16              center_right   = 0;
    uint16              center_left    = 0;
    MMI_WIN_ID_T        win_id         = 0;
    GUISTBDATA_ITEM_DATA_T *item_data_ptr   = PNULL;
    uint32              temp_width  = 0;

    item_total_num = CTRLSTBDATA_GetItemTotalNum();

    // 入参检查
    if (PNULL == statusbar_ctrl_ptr || 0 == item_total_num)
    {
        return FALSE;
    }

    ResetAllItemRect (statusbar_ctrl_ptr);

    // get parent's win id
    win_id = (MMI_WIN_ID_T) MMK_GetWinHandleByCtrl (statusbar_ctrl_ptr->base_ctrl.handle);

    // 重新调整区域，去除边界值
    rect.left = statusbar_ctrl_ptr->base_ctrl.rect.left + CTRLSTB_ITEM_LEFT_RIGHT_MARGEN;
#ifdef MAINLCD_SIZE_320X480
    rect.top = statusbar_ctrl_ptr->base_ctrl.rect.top + CTRLSTB_ITEM_LEFT_RIGHT_MARGEN;
#else
    rect.top = statusbar_ctrl_ptr->base_ctrl.rect.top + CTRLSTB_ITEM_TOP_BOTTON_MARGEN;
#endif
    rect.right = statusbar_ctrl_ptr->base_ctrl.rect.right - CTRLSTB_ITEM_LEFT_RIGHT_MARGEN;
    rect.bottom = statusbar_ctrl_ptr->base_ctrl.rect.bottom - CTRLSTB_ITEM_TOP_BOTTON_MARGEN;
    total_width = rect.right - rect.left + 1;

    if (GUI_IsInvalidRect (rect)) // 检查区域是否有效
    {
        return FALSE;
    }

    //Istyle风格中间仅有time item
    for (i = (int32) (item_total_num - 1); i >= 0; i--)
    {
        uint16 text_width  = 0;
        //get item data
        item_data_ptr = CTRLSTBDATA_GetItemData (i);

        if (PNULL != item_data_ptr)
        {
            //是否是居中的
            if (CTRLSTBDATA_IsItemCenterAlign (i))
            {
                CTRLSTBDATA_SetItemDisable (i, FALSE);
                CTRLSTBDATA_SetItemVisible (i, TRUE);

                if (CTRLSTBDATA_IsItemVisible (i))
                {
                    if (item_data_ptr->text_info.is_display)
                    {
                        text_width = GetTextLength (&item_data_ptr->text_info);

                    }

                    center_width = text_width;

                    if (center_width > total_width)
                    {
                        break;
                    }

                    if (center_width > 0)
                    {
                        item_rect.left = ( (rect.right) / 2) - ( ( (int16) center_width) / 2) + 1;
                    }
                    else
                    {
                        item_rect.left = ( (rect.right) / 2) - ( ( (int16) center_width) / 2);
                    }

                    if ( (text_width) > 0)
                    {
                        item_rect.right = item_rect.left + (text_width) - 1;
                    }
                    else
                    {
                        item_rect.right = item_rect.left + (text_width);
                    }

                    item_rect.top = rect.top;
                    item_rect.bottom = rect.bottom;
                    //用于右侧长度判定
                    center_right = (uint16) (item_rect.right - statusbar_ctrl_ptr->base_ctrl.rect.left);
                    //xiyuan edit for CRNEWMS00128802
                    center_left  = (uint16) (item_rect.left - statusbar_ctrl_ptr->base_ctrl.rect.left);
                    CTRLSTBDATA_SetItemRect (i, item_rect);
                    center_width +=  statusbar_ctrl_ptr->style_info.item_space;
                }
            }
        }

    }

    // 从最后一个item开始计算rect，右侧的Item优先显示
    for (i = (int32) (item_total_num - 2); i >= 0; i--)
    {
        uint16 img_width   = 0;
        uint16 text_width  = 0;
        item_index = i;

        // get item data
        item_data_ptr = CTRLSTBDATA_GetItemData (item_index);

        if (PNULL != item_data_ptr)
        {
            // 这里判断，从右侧开始
            if (CTRLSTBDATA_IsItemRightAlign (item_index))
            {
                CTRLSTBDATA_SetItemDisable (item_index, FALSE);

                if (CTRLSTBDATA_IsItemVisible (item_index))
                {
                    if (PNULL != item_data_ptr->owner_draw_func)
                    {
                        //owner draw max width = max width
                        temp_width = MMISTATUSBAR_ITEM_WIDTH;
                    }
                    else
                    {
                        // 这里计算文本的宽度
                        if (item_data_ptr->text_info.is_display)
                        {
                            text_width = GetTextLength (&item_data_ptr->text_info);
                        }

                        // 这里计算图标的宽度
                        if (item_data_ptr->icon_info.is_display)
                        {
                            image_id = item_data_ptr->icon_info.icon_array[item_data_ptr->cur_frame];
                            GUIRES_GetImgWidthHeight (&img_width, PNULL, image_id, win_id);
                        }

                        temp_width = (text_width + img_width);
                    }

                    item_width += temp_width;

                    //calculate & save item rect
                    item_rect.top    = rect.top;
                    item_rect.bottom = rect.bottom;

                    if (item_width > (total_width - center_right))
                    {
                        // 显示不下的情况下该item为无效
                        item_width -= temp_width;
                        item_rect.left  = 0;
                        item_rect.right = 0;
                        CTRLSTBDATA_SetItemDisable (item_index, TRUE);
                        CTRLSTBDATA_SetItemRect (item_index, item_rect);
                        break;
                    }
                    else
                    {
                        if (rect.right >= (int16) item_width)
                        {
                            item_rect.left  = rect.right - (int16) item_width + 1;
                        }

                        if (temp_width > 0)
                        {
                            item_rect.right = item_rect.left + (int16) temp_width - 1;
                        }
                        else
                        {
                            item_rect.right = item_rect.left + (int16) temp_width;
                        }

                        CTRLSTBDATA_SetItemRect (item_index, item_rect);
                        item_width         += statusbar_ctrl_ptr->style_info.item_space;
                    }

                    right_item_num++;
                    result = TRUE;
                }
            }
        }
        else
        {
            //SCI_TRACE_LOW:"CalculateItemRect 1 item_data_ptr = PNULL, %d"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, GUISTATUSBAR_935_112_2_18_3_21_34_333, (uint8*) "d", item_index);
        }
    }

    // 开始计算左侧的区域
    item_total_num -= right_item_num;

    // 剩余的宽度
    total_width -= item_width;

    // 从左向右开始计算的时候，该值初始化为0
    item_width = 0;

    // 计算从第1个到第item_total_num - 2个item的区域,并设置是否有效
    for (item_index = 0; item_index < item_total_num; item_index++)
    {
        uint16 img_width   = 0;
        uint16 text_width  = 0;

        item_data_ptr = CTRLSTBDATA_GetItemData (item_index);

        if (PNULL != item_data_ptr)
        {
            // 这里判断，从右侧开始
            if (!CTRLSTBDATA_IsItemRightAlign (item_index))
            {
                CTRLSTBDATA_SetItemDisable (item_index, FALSE);

                // item是否可见
                if (CTRLSTBDATA_IsItemVisible (item_index))
                {
                    if (PNULL != item_data_ptr->owner_draw_func)
                    {
                        //owner draw max width = max width
                        temp_width = MMISTATUSBAR_ITEM_WIDTH;
                    }
                    else
                    {
                        // 这里计算文本的宽度
                        if (item_data_ptr->text_info.is_display)
                        {
                            text_width = GetTextLength (&item_data_ptr->text_info);
                        }

                        // 这里计算图标的宽度
                        if (item_data_ptr->icon_info.is_display)
                        {
                            image_id = item_data_ptr->icon_info.icon_array[item_data_ptr->cur_frame];
                            GUIRES_GetImgWidthHeight (&img_width, PNULL, image_id, win_id);
                        }

                        temp_width = text_width + img_width;
                    }

                    //从左开始计算的时候，先加上用于越界判断
                    item_width += temp_width;

                    //calculate & save item rect
                    item_rect.top = rect.top;
                    item_rect.bottom = rect.bottom;

                    if ( (int32) item_width > ( (int32) center_left))
                    {
                        // 显示不下的情况下该item为无效
                        item_width -= temp_width;
                        item_rect.left  = 0;
                        item_rect.right = 0;
                        CTRLSTBDATA_SetItemDisable (item_index, TRUE);
                        CTRLSTBDATA_SetItemRect (item_index, item_rect);
                        break;
                    }
                    else
                    {
                        item_width         -= temp_width;
                        item_rect.left      = rect.left + (int16) item_width + 1;

                        if (temp_width > 0)
                        {
                            item_rect.right = item_rect.left + (int16) temp_width - 1;
                        }
                        else
                        {
                            item_rect.right = item_rect.left;
                        }

                        CTRLSTBDATA_SetItemRect (item_index, item_rect);
                        item_width         += (temp_width + statusbar_ctrl_ptr->style_info.item_space);
                    }

                    result = TRUE;
                }
            }
        }
        else
        {
            //SCI_TRACE_LOW:"CalculateItemRect 2 item_data_ptr = PNULL, %d"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, GUISTATUSBAR_1028_112_2_18_3_21_35_334, (uint8*) "d", item_index);
        }


    }

    return result;
}


/*****************************************************************************/
// Description : get lcd dev info
// Global resource dependence :
// Author: James.Zhang
// Note: get lcd dev info
/*****************************************************************************/
LOCAL GUI_LCD_DEV_INFO GetStatusBarLcdDevInfo (
    CTRLSTATUSBAR_OBJ_T* statusbar_ctrl_ptr
)
{
    GUI_LCD_DEV_INFO lcd_dev_info = {0};
#ifdef UI_MULTILAYER_SUPPORT

    if (GetState (statusbar_ctrl_ptr->ctrl_state, GUISTATUSBAR_STATE_USE_LAYER))
    {
        lcd_dev_info = statusbar_ctrl_ptr->statusbar_layer.lcd_dev_info;
    }
    else
    {
        lcd_dev_info = statusbar_ctrl_ptr->base_ctrl.lcd_dev_info;
    }

    if (UILAYER_IsMultiLayerEnable() && !UILAYER_IsLayerActive (&lcd_dev_info))
    {
        lcd_dev_info.block_id = 0; // 支持多层，且层不active时，使用主层
    }

#else
    lcd_dev_info = statusbar_ctrl_ptr->base_ctrl.lcd_dev_info;
#endif

    return (lcd_dev_info);
}


/*****************************************************************************/
// Description : 画背景
// Global resource dependence :
// Author: Xiaoqing.lu
// Note: the return value means this function draw backgroung or not
/*****************************************************************************/
LOCAL BOOLEAN DrawStatusBarBgExt (
    CTRLSTATUSBAR_OBJ_T* statusbar_ctrl_ptr,
    GUI_RECT_T bg_rect
)
{
    MMI_WIN_ID_T        win_id      = 0;
    GUI_BG_DISPLAY_T    bg_display  = {0};
    GUI_LCD_DEV_INFO    lcd_dev_info = {0};
    BOOLEAN				is_draw_bg = TRUE;

    win_id = MMK_GetWinHandleByCtrl (statusbar_ctrl_ptr->base_ctrl.handle);

    lcd_dev_info = GetStatusBarLcdDevInfo (statusbar_ctrl_ptr);

    if (GUI_BG_IMG == statusbar_ctrl_ptr->style_info.bg_info.bg_type)
    {
        int16           image_width = bg_rect.right - bg_rect.left + 1;
        int16           image_height = bg_rect.bottom - bg_rect.top + 1;
        GUI_RECT_T      image_rect = {0};

        IMG_EnableTransparentColor (TRUE);

        if (statusbar_ctrl_ptr->style_info.is_transparent)
        {
            //set bg display info
            bg_display.is_transparent = FALSE;
            bg_display.rect           = bg_rect;
            bg_display.display_rect   = bg_rect;
            bg_display.img_rect       = bg_rect;
            bg_display.ctrl_handle    = statusbar_ctrl_ptr->base_ctrl.handle;
            bg_display.win_handle     = win_id;

            GUI_DisplayBg (&statusbar_ctrl_ptr->base_ctrl.bg,
                           &bg_display,
                           &lcd_dev_info);
        }

        if (0 < image_width && 0 < image_height)
        {
            image_rect = GUI_CreateRect (0, 0, image_width - 1, image_height - 1);
            //display bg
            GUIRES_DisplayImg (PNULL,
                               &bg_rect,
                               &image_rect,
                               win_id,
                               statusbar_ctrl_ptr->style_info.bg_info.img_id,
                               &lcd_dev_info);
        }

        IMG_EnableTransparentColor (FALSE);

    }
    else if (GUI_BG_COLOR == statusbar_ctrl_ptr->style_info.bg_info.bg_type)
    {
        // 画背景颜色
        GUI_FillRect (&lcd_dev_info, bg_rect, statusbar_ctrl_ptr->style_info.bg_info.color);
    }
    else
    {
        is_draw_bg = FALSE;
        // 此处不需要画背景，使用通用的背景
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, GUISTATUSBAR_1149_112_2_18_3_21_35_336, (uint8*) "d", statusbar_ctrl_ptr->style_info.bg_info.bg_type);
    }

    return is_draw_bg;
}


/*****************************************************************************/
// Description : 画背景
// Global resource dependence :
// Author: Xiaoqing.lu
// Note: the return value means this function draw backgroung or not
/*****************************************************************************/
LOCAL BOOLEAN DrawStatusBarBg (
    CTRLSTATUSBAR_OBJ_T* statusbar_ctrl_ptr,
    GUI_RECT_T bg_rect,
    BOOLEAN is_week_layer
)
{
    BOOLEAN             is_draw_bg  = TRUE;
    MMI_WIN_ID_T        win_id      = 0;
    GUI_LCD_DEV_INFO    lcd_dev_info = {0};

    if (PNULL == statusbar_ctrl_ptr)
    {
        //SCI_TRACE_LOW:"DrawStatusBarBg error input param!"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, GUISTATUSBAR_1079_112_2_18_3_21_35_335, (uint8*) "");
        return FALSE;
    }

    win_id = MMK_GetWinHandleByCtrl (statusbar_ctrl_ptr->base_ctrl.handle);

    lcd_dev_info = GetStatusBarLcdDevInfo (statusbar_ctrl_ptr);

#ifdef UI_MULTILAYER_SUPPORT

    //user layer draw bg
    if (GetState (statusbar_ctrl_ptr->ctrl_state, GUISTATUSBAR_STATE_USE_LAYER)
		&& 0 != lcd_dev_info.block_id)
    {
        UILAYER_ClearRect (&lcd_dev_info, bg_rect);

        if (statusbar_ctrl_ptr->is_display_bg)
        {
            DrawStatusBarBgExt (statusbar_ctrl_ptr, bg_rect);
        }
    }
    //main layer draw bg
    else
#endif
    {
#ifdef UI_MULTILAYER_SUPPORT

        if (statusbar_ctrl_ptr->clear_customer_bg)
        {
            if (UILAYER_HANDLE_MAIN != UILAYER_GetHandleType (&lcd_dev_info))
            {
                UILAYER_ClearRect (&lcd_dev_info, bg_rect);
            }
        }

#endif

        if (statusbar_ctrl_ptr->is_display_bg)
        {
            is_draw_bg = DrawStatusBarBgExt (statusbar_ctrl_ptr, bg_rect);
        }

#ifdef UI_MULTILAYER_SUPPORT
        else
        {
            // 此处不需要画背景，使用通用的背景
            // SCI_TRACE_LOW("DrawStatusBarBg status bar ctrl has no background image!");
            if (UILAYER_HANDLE_MAIN != UILAYER_GetHandleType (&lcd_dev_info))
            {
                UILAYER_ClearRect (&lcd_dev_info, bg_rect);
            }

            is_draw_bg = FALSE;
        }

#endif
    }

    return is_draw_bg;
}


/*****************************************************************************/
// Description : 显示控件和相关的图标
// Global resource dependence :
// Author: xiaoqing.lu
// Note:
/*****************************************************************************/
LOCAL void DrawStatusBarAllItem (
    CTRLSTATUSBAR_OBJ_T* statusbar_ctrl_ptr,
    int16 offset_y
)
{
    uint32          item_index      = 0;
    MMI_WIN_ID_T    win_id          = 0;
    uint32          item_total_num  = 0;
    GUI_RECT_T      item_rect       = {0};
    MMI_IMAGE_ID_T  image_id        = 0;
    GUISTBDATA_ITEM_DATA_T *item_data_ptr = PNULL;
    GUI_BG_DISPLAY_T       bg_display      = {0};
    GUI_LCD_DEV_INFO       lcd_dev_info    = {0};
    GUISTBDATA_OWNER_DRAW_T    owner_draw  = {0};
    GUI_RECT_T                 cross_rect  = {0};

    if (PNULL == statusbar_ctrl_ptr)
    {
        //SCI_TRACE_LOW:"DrawStatusBarAllItem error input param!"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, GUISTATUSBAR_1190_112_2_18_3_21_35_337, (uint8*) "");
        return;
    }

    lcd_dev_info = GetStatusBarLcdDevInfo (statusbar_ctrl_ptr);

    item_total_num = CTRLSTBDATA_GetItemTotalNum();
    win_id = MMK_GetWinHandleByCtrl (statusbar_ctrl_ptr->base_ctrl.handle);

    for (item_index = 0; item_index < item_total_num; item_index++)
    {
#ifdef PDA_UI_DROPDOWN_WIN
        //显示日期时，所有靠左的ITEM都不显示
        //目前MMI_WIN_TEXT_DATE为0，如果不为0，需要修改下面的0

        //Istyle风格下，显示日期时，所有的item，除了电池，全都不显示
        //目前MMI_WIN_TEXT_DATE为0，如果不为0，需要修改下面的0
        //目前MMI_WIN_ICON_BATTERY为item_total_num -2，如果不为item_total_num -2，需要修改
        //目前MMI_WIN_TEXT_TIME为item_total_num -1，如果不为item_total_num -1，需要修改
        if (CTRLSTBDATA_IsItemVisible (0/*MMI_WIN_TEXT_DATE*/))
        {
            if (MMITHEME_IsIstyle())
            {
#ifdef GUIF_STATUSBAR_DUAL_BATTERY //双电池

                if (item_index != 0 && item_index != (item_total_num - 1) && item_index != (item_total_num - 2) && item_index != (item_total_num - 3))
                {
                    continue;
                }

#else

                if (item_index != 0 && item_index != (item_total_num - 1) && item_index != (item_total_num - 2))
                {
                    continue;
                }

#endif
            }
            else
            {
                if (!CTRLSTBDATA_IsItemRightAlign (item_index) && item_index != 0/*MMI_WIN_TEXT_DATE*/)
                {
                    continue;
                }
            }

        }

#endif

        if (CTRLSTBDATA_IsItemVisible (item_index) 
            && !CTRLSTBDATA_IsItemDisable (item_index)
            && CTRLSTBDATA_IsItemValid(item_index)) // 可见的有效的
        {
            GUI_RECT_T  text_rect   = {0};
            GUI_RECT_T  clip_rect   = {0};
            uint16      img_width   = 0;
            uint16      img_height   = 0;
            GUI_RECT_T  disp_rect = {0};
            GUI_RECT_T  img_rect = {0};

            item_data_ptr = CTRLSTBDATA_GetItemData (item_index);

            SCI_ASSERT (PNULL != item_data_ptr); /*assert verified*/

            item_rect = CTRLSTBDATA_GetItemRect (item_index);

            item_rect.top += offset_y;
            item_rect.bottom += offset_y;

            //set bg display info
            bg_display.is_transparent = FALSE;
            bg_display.rect                  = statusbar_ctrl_ptr->base_ctrl.rect;
            bg_display.display_rect     = item_rect;
            bg_display.ctrl_handle      = statusbar_ctrl_ptr->base_ctrl.handle;
            bg_display.win_handle     = win_id;

            if (PNULL == item_data_ptr->owner_draw_func)
            {
                //画背景
                if (GUI_BG_IMG == item_data_ptr->bg_info.bg_type ||
                        GUI_BG_COLOR == item_data_ptr->bg_info.bg_type)
                {
                    GUI_DisplayBg (&item_data_ptr->bg_info, &bg_display, &lcd_dev_info);
                }

                if (item_data_ptr->icon_info.is_display)
                {
                    image_id = item_data_ptr->icon_info.icon_array[item_data_ptr->cur_frame];
                    GUIRES_GetImgWidthHeight (&img_width, &img_height, image_id, win_id);

                    //start_point.x = item_rect.left;
                    //start_point.y = item_rect.top;
                    IMG_EnableTransparentColor (TRUE);

                    item_rect = GUI_GetCenterRect (item_rect, img_width, img_height);

                    // 画图片
                    if (GUI_IntersectRect (&disp_rect, item_rect, statusbar_ctrl_ptr->base_ctrl.rect))
                    {
                        img_rect.left = disp_rect.left - item_rect.left;
                        img_rect.right = disp_rect.right - item_rect.left;
                        img_rect.top = disp_rect.top - item_rect.top;
                        img_rect.bottom = disp_rect.bottom - item_rect.top;

                        if (item_data_ptr->icon_info.is_color_exchange)
                        {
                            GUIRES_DisplayImgWitchColorExchange (PNULL,
                                                                 &disp_rect,
                                                                 &img_rect,
                                                                 win_id,
                                                                 image_id,
                                                                 &lcd_dev_info,
                                                                 item_data_ptr->icon_info.is_color_exchange,
                                                                 item_data_ptr->icon_info.color);
                        }
                        else
                        {
                            GUIRES_DisplayImg (PNULL,
                                               &disp_rect,
                                               &img_rect,
                                               win_id,
                                               image_id,
                                               &lcd_dev_info);
                        }
                    }

                    IMG_EnableTransparentColor (FALSE);
                }

                if (item_data_ptr->text_info.is_display)
                {
                    GUISTR_STYLE_T  str_style = {0};/*lint !e64*/
                    GUISTR_STATE_T  str_state = GUISTR_STATE_ALIGN | GUISTR_STATE_SINGLE_LINE | GUISTR_STATE_EFFECT | GUISTR_STATE_ALIGN_BY_DIR | GUISTR_STATE_BOLD;
                    MMI_STRING_T    str_info = {0};
                    // 设置文本风格
                    str_style.align = item_data_ptr->text_info.align;
                    str_style.font = item_data_ptr->text_info.font_type;
                    str_style.font_color = item_data_ptr->text_info.font_color;
                    str_style.char_space = item_data_ptr->text_info.font_space;
                    str_style.effect = FONT_EFFECT_NONE;//FONT_EFFECT_REVERSE_EDGE;
                    // 文本区域
                    text_rect = item_rect;
                    text_rect.left = item_rect.left + (int16) img_width;

                    if (GUI_IntersectRect (&clip_rect, text_rect, statusbar_ctrl_ptr->base_ctrl.rect))
                    {
                        str_info.wstr_len = item_data_ptr->text_info.wstr_len;
                        str_info.wstr_ptr = item_data_ptr->text_info.wstr_ptr;
                        //clip_rect.top += 2;
                        text_rect.top += DP2PX_VALUE(2);
                        // 画字
                        GUISTR_DrawTextToLCDInRect (&lcd_dev_info,
                                                    (const GUI_RECT_T*) &text_rect,
                                                    (const GUI_RECT_T*) &clip_rect/*text_rect*/,
                                                    (const MMI_STRING_T*) &str_info,
                                                    &str_style,
                                                    str_state,
                                                    GUISTR_TEXT_DIR_AUTO);
                    }
                }
            }
            else
            {
                //owner draw
                if (PNULL != item_data_ptr->owner_draw_func)
                {
                    //get item rect
                    item_rect = CTRLSTBDATA_GetItemRect (item_index);

                    if (GUI_IntersectRect (&cross_rect, statusbar_ctrl_ptr->base_ctrl.rect, item_rect))
                    {
                        owner_draw.display_rect = cross_rect;
                        owner_draw.ctrl_handle  = statusbar_ctrl_ptr->base_ctrl.handle;
                        owner_draw.lcd_dev      = lcd_dev_info;

                        item_data_ptr->owner_draw_func (&owner_draw);
                    }
                }
            }
        }
    }

    if (MMK_IsFocusWin (win_id))
    {
        StartStatusBarTimer (statusbar_ctrl_ptr);
    }
}





/*****************************************************************************/
// Description : 显示控件
// Global resource dependence :
// Author: Xiaoqing.Lu
// Note:
/*****************************************************************************/
LOCAL void DrawStatusBarCtrl (
    CTRLSTATUSBAR_OBJ_T* statusbar_ctrl_ptr
)
{
    GUISTBSCROLLMSG_SCROLL_STATE_E cur_scroll_state = GUISTBSCROLLMSG_SCROLL_NONE;

    if (PNULL == statusbar_ctrl_ptr)
    {
        return;
    }

#ifdef UI_MULTILAYER_SUPPORT

    /* add for stb-layer, jixin.xu */
    if (GetState (statusbar_ctrl_ptr->ctrl_state, GUISTATUSBAR_STATE_USE_LAYER))
    {
        CreateCtrlLayer (statusbar_ctrl_ptr);
    }
    else
    {
        ReleaseCtrlLayer (statusbar_ctrl_ptr);
    }

    if (GetState (statusbar_ctrl_ptr->ctrl_state, GUISTATUSBAR_STATE_USE_LAYER))
    {
        UILAYER_AppendBltLayer (&statusbar_ctrl_ptr->statusbar_layer);
    }

    //block_id 不是main layer，也不是user layer，则为customer layer
    if ( (statusbar_ctrl_ptr->base_ctrl.lcd_dev_info.block_id != 0) &&
            (!GetState (statusbar_ctrl_ptr->ctrl_state, GUISTATUSBAR_STATE_USE_LAYER)))
    {
        SetState (&statusbar_ctrl_ptr->ctrl_state, GUISTATUSBAR_STATE_CUSTOMER_LAYER, TRUE);
    }

#endif

    /* xiyuan.ma */
    if (MMITHEME_IsIstyle())
    {
        CalculateIstyleItemRect (statusbar_ctrl_ptr);
    }
    else
    {
        CalculateItemRect (statusbar_ctrl_ptr);
    }

    if (GetState (statusbar_ctrl_ptr->ctrl_state, GUISTATUSBAR_STATE_SCROLLMSG_PAUSE))
    {
        DrawStatusBarBg (statusbar_ctrl_ptr, statusbar_ctrl_ptr->base_ctrl.rect, TRUE);
        DrawStatusBarAllItem (statusbar_ctrl_ptr, 0);
    }
    else
    {
#ifdef GUIF_STATUSBAR_SCROLL
        cur_scroll_state = CTRLSTBSCROLLMSG_GetCurScrollState();
#else
        cur_scroll_state = GUISTBSCROLLMSG_SCROLL_NONE;
#endif

        switch (cur_scroll_state)
        {
#ifdef GUIF_STATUSBAR_SCROLL
        case GUISTBSCROLLMSG_SCROLL_START:
            DrawStatusBarScrollMsg (statusbar_ctrl_ptr, cur_scroll_state);
            break;

        case GUISTBSCROLLMSG_SCROLL_END:
            DrawStatusBarScrollMsg (statusbar_ctrl_ptr, cur_scroll_state);
            break;

        case GUISTBSCROLLMSG_SCROLL_RUNNING:
            DrawStatusBarScrollMsg (statusbar_ctrl_ptr, cur_scroll_state);
            break;
#endif
        case GUISTBSCROLLMSG_SCROLL_NONE:
        default:
            DrawStatusBarBg (statusbar_ctrl_ptr, statusbar_ctrl_ptr->base_ctrl.rect, TRUE);
            DrawStatusBarAllItem (statusbar_ctrl_ptr, 0);
            break;
        }
    }
}


/*****************************************************************************/
// Description : 处理控件的定时器
// Global resource dependence :
// Author: xiaoqing.lu
// Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleTimer (
    CTRLSTATUSBAR_OBJ_T* statusbar_ctrl_ptr,
    uint8               timer_id
)
{
    uint32          item_index  = 0;
    uint32          item_total_num  = 0;
    uint16			icon_width  = 0;
    uint16			icon_height = 0;
    GUI_RECT_T		item_rect   = {0};
    MMI_WIN_ID_T    win_id      = 0;
    MMI_IMAGE_ID_T  image_id    = 0;
    MMI_RESULT_E    result      = MMI_RESULT_FALSE;
    GUISTBDATA_ITEM_DATA_T      *item_data_ptr = PNULL;
    GUI_LCD_DEV_INFO lcd_dev_info = {0};

    if (PNULL != statusbar_ctrl_ptr && timer_id == statusbar_ctrl_ptr->timer_id)
    {
        lcd_dev_info = GetStatusBarLcdDevInfo (statusbar_ctrl_ptr);
        item_total_num = CTRLSTBDATA_GetItemTotalNum();
        win_id = MMK_GetWinHandleByCtrl (statusbar_ctrl_ptr->base_ctrl.handle);
        // 首先停止timer
        StopStatusBarTimer (statusbar_ctrl_ptr);
#if defined PDA_UI_DROPDOWN_WIN || defined DROPDOWN_NOTIFY_SHORTCUT
        if(!CTRLSTATUSBAR_IsDropDown(statusbar_ctrl_ptr->base_ctrl.win_handle))//bug1570975
#endif
        {
            // 判断空间是否可见
            if (!IGUICTRL_GetState ( (IGUICTRL_T*) statusbar_ctrl_ptr, GUICTRL_STATE_INVISIBLE)
                    && (0 == statusbar_ctrl_ptr->scroll_msg_timer_id))    //scrollmsg滚动的同时不处理
            {
                for (item_index = 0; item_index < item_total_num; item_index++)
                {
                    if (CTRLSTBDATA_IsItemVisible (item_index) && !CTRLSTBDATA_IsItemDisable (item_index))
                    {
                        item_data_ptr = CTRLSTBDATA_GetItemData (item_index);

                        if (PNULL != item_data_ptr && GUISTBDATA_ICON_ANIM == item_data_ptr->icon_info.icon_type)
                        {
                            //SCI_TRACE_LOW:"HandleTimer item_index = %d"
                            SCI_TRACE_ID (TRACE_TOOL_CONVERT, GUISTATUSBAR_1690_112_2_18_3_21_36_340, (uint8*) "d", item_index);

                            CTRLSTBDATA_GetItemIconNextFrame (item_index, &item_data_ptr->cur_frame);

                            image_id = item_data_ptr->icon_info.icon_array[item_data_ptr->cur_frame];

                            GUIRES_GetImgWidthHeight (&icon_width, &icon_height, image_id, win_id);

                            item_rect = GUI_GetCenterRect (item_data_ptr->item_rect, icon_width, icon_height);

                            DrawStatusBarBg (statusbar_ctrl_ptr, item_rect, FALSE);

                            IMG_EnableTransparentColor (TRUE);
                            // 画图片
                            GUIRES_DisplayImg (PNULL, &item_rect, PNULL, win_id, image_id, &lcd_dev_info);

                            IMG_EnableTransparentColor (FALSE);
                        }
                    }
                }
            }
        }
        if (MMK_IsFocusWin (win_id))
        {
            StartStatusBarTimer (statusbar_ctrl_ptr);
        }

        result = MMI_RESULT_TRUE;
    }
    else if (PNULL != statusbar_ctrl_ptr && timer_id == statusbar_ctrl_ptr->scroll_msg_timer_id)
    {
        // 首先停止timer
        StopStatusBarScrollMsgTimer (statusbar_ctrl_ptr);

        win_id = MMK_GetWinHandleByCtrl (statusbar_ctrl_ptr->base_ctrl.handle);

        if (!IGUICTRL_GetState ( (IGUICTRL_T*) statusbar_ctrl_ptr, GUICTRL_STATE_INVISIBLE)  //可见
                && !GetState (statusbar_ctrl_ptr->ctrl_state, GUISTATUSBAR_STATE_SCROLLMSG_PAUSE) //非pause
                && MMK_IsFocusWin (win_id)) //focus win
        {
            DrawStatusBarCtrl (statusbar_ctrl_ptr);
        }

        result = MMI_RESULT_TRUE;
    }

    return result;
}





/*****************************************************************************/
// Description : 停止timer
// Global resource dependence :
// Author: xiaoqing.lu
// Note:
/*****************************************************************************/
LOCAL void StopStatusBarTimer (
    CTRLSTATUSBAR_OBJ_T* statusbar_ctrl_ptr
)
{
    if (PNULL != statusbar_ctrl_ptr && 0 != statusbar_ctrl_ptr->timer_id)
    {
        // 销毁定时器
        MMK_StopTimer (statusbar_ctrl_ptr->timer_id);
        statusbar_ctrl_ptr->timer_id = 0;
    }

    return;
}


/*****************************************************************************/
// Description : 停止timer
// Global resource dependence :
// Author: bin.ji
// Note:
/*****************************************************************************/
LOCAL void StopStatusBarScrollMsgTimer (
    CTRLSTATUSBAR_OBJ_T* statusbar_ctrl_ptr
)
{
    if (PNULL != statusbar_ctrl_ptr && 0 != statusbar_ctrl_ptr->scroll_msg_timer_id)
    {
        // 销毁定时器
        MMK_StopTimer (statusbar_ctrl_ptr->scroll_msg_timer_id);
        statusbar_ctrl_ptr->scroll_msg_timer_id = 0;
    }

    return;
}


/*****************************************************************************/
// Description : 处理状态栏的消息MSG_CTL_STATUSBAR_UPDATE
// Global resource dependence :
// Author: xiaoqing.lu
// Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E RefreshStatusBar (
    CTRLSTATUSBAR_OBJ_T* statusbar_ctrl_ptr,
    BOOLEAN is_full_paint
)
{
    MMI_RESULT_E result = MMI_RESULT_FALSE;

    // check is_shown flag to avoid painting itself while window is not shown.
    if (PNULL != statusbar_ctrl_ptr
            && !IGUICTRL_GetState ( (IGUICTRL_T*) statusbar_ctrl_ptr, GUICTRL_STATE_INVISIBLE))
    {
        if ( (statusbar_ctrl_ptr->is_display_bg) ||
                (!is_full_paint) ||
                (GetState (statusbar_ctrl_ptr->ctrl_state, GUISTATUSBAR_STATE_USE_LAYER)) ||
                (GetState (statusbar_ctrl_ptr->ctrl_state, GUISTATUSBAR_STATE_CUSTOMER_LAYER)))
        {
            DrawStatusBarCtrl (statusbar_ctrl_ptr);
        }
        else
        {
            // 状态栏没有设置背景信息时,需要刷新整个窗口
            MMK_SendMsg (MMK_GetWinHandleByCtrl (statusbar_ctrl_ptr->base_ctrl.handle), MSG_FULL_PAINT, PNULL);
        }

        result = MMI_RESULT_TRUE;
    }

    return result;
}


/*****************************************************************************/
// Description : 状态栏的消息处理
// Global resource dependence :
// Author: xiaoqing.lu
// Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E StatusbarCtrlHandleMsg (
    CTRLBASE_OBJ_T *base_ctrl_ptr,   //control pointer
    MMI_MESSAGE_ID_E  msg_id,   //事件类型
    DPARAM            param     //附带的处理参数
)
{
#ifdef TOUCH_PANEL_SUPPORT //xiyuan edit for pclint
    GUI_POINT_T                   point           = {0};
#endif
    MMI_RESULT_E                result          = MMI_RESULT_TRUE;
    CTRLSTATUSBAR_OBJ_T   *statusbar_ctrl_ptr = (CTRLSTATUSBAR_OBJ_T*) base_ctrl_ptr;

    if (PNULL == base_ctrl_ptr)
    {
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, GUISTATUSBAR_1942_112_2_18_3_21_37_342, (uint8*) "");
        return MMI_RESULT_FALSE;
    }

    switch (msg_id)
    {
    case MSG_CTL_OPEN:
        break;

    case MSG_CTL_LOSE_FOCUS:    //失去焦点
        StopStatusBarTimer (statusbar_ctrl_ptr);
        StopStatusBarScrollMsgTimer (statusbar_ctrl_ptr);
        break;

    case MSG_CTL_GET_FOCUS:        //得到焦点
        break;

    case MSG_TIMER:
        result = HandleTimer (statusbar_ctrl_ptr, * (uint8*) param);
        break;

    case MSG_CTL_PAINT:

        if (!IGUICTRL_GetState ( (IGUICTRL_T*) statusbar_ctrl_ptr, GUICTRL_STATE_INVISIBLE))
        {
            MMI_HANDLE_T ctrl_handle = base_ctrl_ptr->handle;
            MMI_HANDLE_T win_handle = MMK_GetWinHandleByCtrl (base_ctrl_ptr->handle);

            GUICTRL_SendNotify (base_ctrl_ptr->handle, MSG_NOTIFY_STATUSBAR_PAINT);

            if (MMK_IsOpenWin (win_handle) && PNULL != MMK_GetCtrlPtr (ctrl_handle))
            {
                //Bugzilla Bug 68978: stb在非焦点窗口下，不滚动
                if(!MMK_IsFocusWin(win_handle))
                {
                    SetState(&(statusbar_ctrl_ptr->ctrl_state), GUISTATUSBAR_STATE_SCROLLMSG_PAUSE, TRUE);
                }
                else
                {
                    SetState(&(statusbar_ctrl_ptr->ctrl_state), GUISTATUSBAR_STATE_SCROLLMSG_PAUSE, FALSE);
                }    

                DrawStatusBarCtrl (statusbar_ctrl_ptr);
            }

            if (MMK_IsOpenWin (win_handle) && PNULL != MMK_GetCtrlPtr (ctrl_handle))
            {
                GUICTRL_SendNotify (base_ctrl_ptr->handle, MSG_NOTIFY_STATUSBAR_PAINT_END);

                if (MMK_IsFocusWin (MMK_GetWinHandleByCtrl (base_ctrl_ptr->handle)))
                {
                    StartStatusBarTimer (statusbar_ctrl_ptr);
                }
            }
        }

        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_DOWN:

        if (!GetState (statusbar_ctrl_ptr->ctrl_state, GUISTATUSBAR_STATE_INVALID))
        {
            result = MMITHEME_HandleStatusbarTPMsg (MMK_GetWinHandleByCtrl (base_ctrl_ptr->handle), msg_id, param);
        }

        // 不处理
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_MOVE:

        if (!GetState (statusbar_ctrl_ptr->ctrl_state, GUISTATUSBAR_STATE_INVALID))
        {
            result = MMITHEME_HandleStatusbarTPMsg (MMK_GetWinHandleByCtrl (base_ctrl_ptr->handle), msg_id, param);
        }

        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_UP: // TP UP时响应操作

        if (!GetState (statusbar_ctrl_ptr->ctrl_state, GUISTATUSBAR_STATE_INVALID))
        {
            result = MMITHEME_HandleStatusbarTPMsg (MMK_GetWinHandleByCtrl (base_ctrl_ptr->handle), msg_id, param);
        }

        if (MMI_RESULT_FALSE == result)
        {
            point.x = MMK_GET_TP_X (param);
            point.y = MMK_GET_TP_Y (param);
            result = HandleTPKeyUp (statusbar_ctrl_ptr, point);
        }

        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527


    case MSG_CTL_STATUSBAR_UPDATE:
        RefreshStatusBar (statusbar_ctrl_ptr, TRUE);
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }

    return result;
}


/*****************************************************************************/
//  Description : get statusbar type
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC TYPE_TAG_T STATUSBAR_GetType()
{
    static TYPE_TAG_T    obj_type = 0;

    if (0 == obj_type)
    {
        obj_type = TYPEMNG_Register (
                       CTRL_BASE_TYPE,
                       "statusbar",
                       sizeof (CTRLSTATUSBAR_OBJ_T),
                       (OBJECT_CONSTRUCTOR_FUNC) StatusbarCtrlConstruct,
                       (OBJECT_DESTRUCTOR_FUNC) StatusbarCtrlDestruct,
                       sizeof (CTRLSTATUSBAR_INIT_PARAM_T),
                       (PARAM_PACKING_FUNC) StatusbarCtrlPackInitParam,
                       sizeof (CTRLSTATUSBAR_VTBL_T),
                       (VTBL_INIT_FUNC) StatusbarCtrlInitVtbl);
    }

    return obj_type;
}



#ifdef TOUCH_PANEL_SUPPORT
/*****************************************************************************/
// Description : 处理控件的定时器
// Global resource dependence :
// Author: xiaoqing.lu
// Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleTPKeyUp (
    CTRLSTATUSBAR_OBJ_T* statusbar_ctrl_ptr,
    GUI_POINT_T           point
)
{
    uint32                   item_index  = 0;
    uint32                   item_total_num  = 0;
    MMI_WIN_ID_T             win_id      = 0;
    MMI_RESULT_E             result      = MMI_RESULT_TRUE;
    GUISTBDATA_ITEM_DATA_T   *item_data_ptr = PNULL;

    if (PNULL == statusbar_ctrl_ptr)
    {
        return MMI_RESULT_FALSE;
    }

    item_total_num = CTRLSTBDATA_GetItemTotalNum();

    win_id = MMK_GetWinHandleByCtrl (statusbar_ctrl_ptr->base_ctrl.handle);

    //响应TP UP消息
    if (GUI_PointIsInRect (point, statusbar_ctrl_ptr->base_ctrl.rect) // 点击位置是否在状态栏区域
            && GetState (statusbar_ctrl_ptr->ctrl_state, GUISTATUSBAR_STATE_EVENT)) // 是否支持点击事件
    {
        for (item_index = 0; item_index < item_total_num; item_index++)
        {
            item_data_ptr = CTRLSTBDATA_GetItemData (item_index);

            if (PNULL != item_data_ptr
                    && GUI_PointIsInRect (point, item_data_ptr->item_rect)) // 点击位置是否在具体的item区域
            {
                // 点是否在某一项的区域内，执行条件为1、item可见；2、item有效；3、item允许执行应用回调；4、回调函数地址不为空
                if (CTRLSTBDATA_IsItemVisible (item_index)
                        && !CTRLSTBDATA_IsItemDisable (item_index)
                        && CTRLSTBDATA_ItemIsHandleFuncValid (item_index)
                        && PNULL != item_data_ptr->app_handle_func)
                {
                    item_data_ptr->app_handle_func(); // 执行app回调函数
                    break;
                }
            }
        }
    }
    else
    {
        result = MMI_RESULT_FALSE;
    }

    return  result;
}
#endif


#ifdef UI_MULTILAYER_SUPPORT
/****************************************************************************/
//  Description :create layer for stb
//  Global resource dependence :
//  Author: jixin.xu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CreateCtrlLayer (
    CTRLSTATUSBAR_OBJ_T* statusbar_ctrl_ptr
)
{
    BOOLEAN result = FALSE;

    if (PNULL != statusbar_ctrl_ptr)
    {	
		if (UILAYER_IsMultiLayerEnable() &&
        GUI_BLOCK_MAIN == statusbar_ctrl_ptr->statusbar_layer.lcd_dev_info.block_id)
		{
			UILAYER_CREATE_T create_info = {0};

			/* Create Layer for Statusbar*/
			create_info.lcd_id = GUI_MAIN_LCD_ID;
			create_info.owner_handle = statusbar_ctrl_ptr->base_ctrl.handle;
			create_info.offset_x = statusbar_ctrl_ptr->base_ctrl.rect.left;
			create_info.offset_y = statusbar_ctrl_ptr->base_ctrl.rect.top;
			create_info.width = statusbar_ctrl_ptr->base_ctrl.rect.right - statusbar_ctrl_ptr->base_ctrl.rect.left + 1;
			create_info.height = statusbar_ctrl_ptr->base_ctrl.rect.bottom - statusbar_ctrl_ptr->base_ctrl.rect.top + 1;
			create_info.format = UILAYER_MEM_FPS_ACCELERATE;

			UILAYER_CreateLayer (&create_info, &statusbar_ctrl_ptr->statusbar_layer.lcd_dev_info);
			statusbar_ctrl_ptr->statusbar_layer.layer_level = UILAYER_LEVEL_HIGH;

			UILAYER_SetLayerColorKey (&statusbar_ctrl_ptr->statusbar_layer.lcd_dev_info, TRUE, UILAYER_TRANSPARENT_COLOR);

			result = TRUE;
		}
    }


    return result;
}


/****************************************************************************/
//  Description :release stb layer
//  Global resource dependence :
//  Author: jixin.xu
//  Note:
/*****************************************************************************/
LOCAL void ReleaseCtrlLayer (
    CTRLSTATUSBAR_OBJ_T* statusbar_ctrl_ptr
)
{
    if (UILAYER_IsMultiLayerEnable())
    {
        UILAYER_RELEASELAYER (&statusbar_ctrl_ptr->statusbar_layer.lcd_dev_info);  /*lint !e506 !e774*/

        statusbar_ctrl_ptr->statusbar_layer.lcd_dev_info.block_id = GUI_BLOCK_MAIN;
    }

    return;
}
#endif

#ifdef GUIF_STATUSBAR_SCROLL
/*****************************************************************************/
// Description : 显示滚动消息
// Global resource dependence :
// Author: bin.ji
// Note:
/*****************************************************************************/
LOCAL void DrawStatusBarScrollItem (
    CTRLSTATUSBAR_OBJ_T* statusbar_ctrl_ptr,
    GUI_RECT_T rect,
    BOOLEAN is_next_item
)
{
    GUISTBSCROLLMSG_SCROLL_ITEM_T scroll_item = {0};
    GUISTR_STYLE_T  str_style = {0};/*lint !e64*/
    GUISTR_STATE_T  str_state = GUISTR_STATE_ALIGN | GUISTR_STATE_SINGLE_LINE \
		                        | GUISTR_STATE_EFFECT | GUISTR_STATE_ALIGN_BY_DIR \
								| GUISTR_STATE_NOT_HALF_CHAR;
    MMI_STRING_T    str_info = {0};
    GUI_RECT_T      text_rect = {0};
    GUI_LCD_DEV_INFO lcd_dev_info   = {0};
    GUI_POINT_T start_point = {0};
    MMI_WIN_ID_T win_id = 0;
    GUI_RECT_T  clip_rect = {0};
    GUI_RECT_T  disp_rect = {0};
    GUI_RECT_T  img_rect = {0};
//    wchar test_string[] = L"1234567";
    MMI_IMAGE_ID_T image_id = 0;
    GUI_RECT_T item_rect = {0};
    uint16 img_width = 0;
    uint16 img_height = 0;

    if (PNULL == statusbar_ctrl_ptr)
    {
        //SCI_TRACE_LOW:"DrawStatusBarScrollItem error input param!"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, GUISTATUSBAR_1395_112_2_18_3_21_35_338, (uint8*) "");
        return;
    }

    if (is_next_item)
    {
        CTRLSTBSCROLLMSG_GetNextLine (&scroll_item);
    }
    else
    {
        CTRLSTBSCROLLMSG_GetCurLine (&scroll_item);
    }

    win_id = MMK_GetWinHandleByCtrl (statusbar_ctrl_ptr->base_ctrl.handle);
    lcd_dev_info = GetStatusBarLcdDevInfo (statusbar_ctrl_ptr);

    item_rect = rect;
    start_point.x = rect.left;
    start_point.y = rect.top;
    IMG_EnableTransparentColor (TRUE);
    // 画图片
    image_id = scroll_item.icon_id;
    GUI_IntersectRect (&disp_rect, item_rect, statusbar_ctrl_ptr->base_ctrl.rect);
    img_rect.left = disp_rect.left - item_rect.left;
    img_rect.right = disp_rect.right - item_rect.left;
    img_rect.top = disp_rect.top - item_rect.top;
    img_rect.bottom = disp_rect.bottom - item_rect.top;
    GUIRES_DisplayImg (PNULL,
                       &disp_rect,
                       &img_rect,
                       win_id,
                       image_id,
                       &lcd_dev_info);
    IMG_EnableTransparentColor (FALSE);

    //画文字
    // 设置文本风格
    str_style.effect 		= FONT_EFFECT_REVERSE_EDGE;
    str_style.angle 		= ANGLE_0;
    str_style.align 		= ALIGN_LVMIDDLE;
    str_style.font 		= DP_FONT_16;
    str_style.font_color 	= 0xffff;//MMI_WHITE_COLOR;

    // 文本区域
    text_rect = rect;

    // renwei modify cr00116016
    GUIRES_GetImgWidthHeight (&img_width,
                              &img_height,
                              image_id,
                              win_id);

    text_rect.left  = img_width + 1;//img_width;(int16)50
    // renwei modify cr00116016
    text_rect.right = text_rect.left + GUISTBSCROLLMSG_TEXT_WIDTH - 1;
    //            text_rect.left = item_rect.left + (int16)img_width;

    str_info.wstr_ptr = scroll_item.string_info.wstr_ptr;
    str_info.wstr_len = scroll_item.string_info.wstr_len;
    GUI_IntersectRect (&clip_rect, text_rect, statusbar_ctrl_ptr->base_ctrl.rect);
    // 画字
    GUISTR_DrawTextToLCDInRect (&lcd_dev_info,
                                (const GUI_RECT_T*) &text_rect,
                                (const GUI_RECT_T*) &clip_rect,
                                (const MMI_STRING_T*) &str_info,
                                &str_style,
                                str_state,
                                GUISTR_TEXT_DIR_AUTO);
}


/*****************************************************************************/
// Description : 计算滚动时2个区域
// Global resource dependence :
// Author: bin.ji
// Note:
/*****************************************************************************/
LOCAL void CalcScrollRect (
    BOOLEAN is_downtoup,
    GUI_RECT_T statusbar_rect,     //in
    uint32 frame_index,
    GUI_RECT_T *rect1_ptr,        //out
    GUI_RECT_T *rect2_ptr        //out
)
{
    int16 interval_y = 0;
    int16 statusbar_height = 0;

    statusbar_height = (statusbar_rect.bottom + 1 - statusbar_rect.top);
    interval_y = statusbar_height / (GUISTBSCROLLMSG_SCROLL_FRAME_NUM - 1);

    if ( (PNULL == rect1_ptr) || (PNULL == rect2_ptr))
    {
        return;
    }

    *rect1_ptr = statusbar_rect;

    if ( (GUISTBSCROLLMSG_SCROLL_FRAME_NUM - 1) == frame_index)
    {
        //处理不整除的情况
        if (is_downtoup)
        {
            rect1_ptr->top -= statusbar_height;
            rect1_ptr->bottom -= statusbar_height;
        }
    }
    else
    {
        if (is_downtoup)
        {
            rect1_ptr->top -= (interval_y * frame_index);       /*lint !e737*/
            rect1_ptr->bottom -= (interval_y * frame_index);    /*lint !e737*/
        }
        else
        {
            rect1_ptr->top += (interval_y * frame_index - statusbar_height);    /*lint !e737*/
            rect1_ptr->bottom += (interval_y * frame_index - statusbar_height); /*lint !e737*/
        }
    }

    *rect2_ptr = *rect1_ptr;
    rect2_ptr->top += statusbar_height;
    rect2_ptr->bottom += statusbar_height;
}


/*****************************************************************************/
// Description : 显示控件和相关的图标
// Global resource dependence :
// Author: bin.ji
// Note:
/*****************************************************************************/
LOCAL void DrawStatusBarScrollMsg (
    CTRLSTATUSBAR_OBJ_T* statusbar_ctrl_ptr,
    GUISTBSCROLLMSG_SCROLL_STATE_E cur_scroll_state
)
{
    GUI_RECT_T scoll_rect1 = {0};
    GUI_RECT_T scoll_rect2 = {0};
    uint32 cur_frame = 0;
    BOOLEAN is_pause = FALSE;

    if (PNULL == statusbar_ctrl_ptr)
    {
        return;
    }

    MMITHEME_SetUpdateDelayCount (0);
    cur_frame = CTRLSTBSCROLLMSG_GetCurFrame();

    if ( (GUISTBSCROLLMSG_SCROLL_FRAME_NUM - 1) == cur_frame)
    {
        is_pause = TRUE;
    }
    else
    {
        is_pause = FALSE;
    }

    //restart timer
    StartStatusBarScrollMsgTimer (statusbar_ctrl_ptr, is_pause);

    switch (cur_scroll_state)
    {
    case GUISTBSCROLLMSG_SCROLL_START:  //向上
        CalcScrollRect (TRUE, statusbar_ctrl_ptr->base_ctrl.rect, cur_frame, &scoll_rect1, &scoll_rect2);
        //改变位置，重新显示
        DrawStatusBarBg (statusbar_ctrl_ptr, statusbar_ctrl_ptr->base_ctrl.rect, TRUE);
        DrawStatusBarAllItem (statusbar_ctrl_ptr, scoll_rect1.top);
        DrawStatusBarScrollItem (statusbar_ctrl_ptr, scoll_rect2, FALSE);
        CTRLSTBSCROLLMSG_ConsumeOneFrame();
        break;

    case GUISTBSCROLLMSG_SCROLL_END:    //向下
        CalcScrollRect (FALSE, statusbar_ctrl_ptr->base_ctrl.rect, cur_frame, &scoll_rect1, &scoll_rect2);
        //改变位置，重新显示
        DrawStatusBarBg (statusbar_ctrl_ptr, statusbar_ctrl_ptr->base_ctrl.rect, TRUE);
        DrawStatusBarAllItem (statusbar_ctrl_ptr, scoll_rect1.top);
        DrawStatusBarScrollItem (statusbar_ctrl_ptr, scoll_rect2, FALSE);
        CTRLSTBSCROLLMSG_ConsumeOneFrame();
        break;

    case GUISTBSCROLLMSG_SCROLL_RUNNING:    //向上
        CalcScrollRect (TRUE, statusbar_ctrl_ptr->base_ctrl.rect, cur_frame, &scoll_rect1, &scoll_rect2);
        //改变位置，重新显示
        DrawStatusBarBg (statusbar_ctrl_ptr, statusbar_ctrl_ptr->base_ctrl.rect, TRUE);
        DrawStatusBarScrollItem (statusbar_ctrl_ptr, scoll_rect1, FALSE);
        DrawStatusBarScrollItem (statusbar_ctrl_ptr, scoll_rect2, TRUE);
        CTRLSTBSCROLLMSG_ConsumeOneFrame();
        break;

    case GUISTBSCROLLMSG_SCROLL_NONE:
    default:
        break;
    }
}
#endif

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC void CTRLSTATUSBAR_SetBgIsDesplay (
    MMI_HANDLE_T win_handle,
    BOOLEAN is_desplay_bg
)
{
    CTRLSTATUSBAR_OBJ_T *ctrl_ptr = GetStatusbarPtr(MMK_GetWinStatusBarHandle(win_handle));

    if (PNULL != ctrl_ptr)
    {
        ctrl_ptr->is_display_bg = is_desplay_bg;
    }

    return;
}


/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:xiyuan.ma
//  Note:
/*****************************************************************************/
PUBLIC void CTRLSTATUSBAR_ClearCustomerBg (
    MMI_HANDLE_T win_handle,
    BOOLEAN clear_customer_bg
)
{
    CTRLSTATUSBAR_OBJ_T *ctrl_ptr = GetStatusbarPtr(MMK_GetWinStatusBarHandle(win_handle));

    if (PNULL != ctrl_ptr)
    {
        ctrl_ptr->clear_customer_bg = clear_customer_bg;
    }

    return;
}


/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC void CTRLSTATUSBAR_SetBgImageId (
    MMI_HANDLE_T win_handle,
    MMI_IMAGE_ID_T image_id
)
{
    CTRLSTATUSBAR_OBJ_T *ctrl_ptr = GetStatusbarPtr(MMK_GetWinStatusBarHandle(win_handle));

    if (PNULL != ctrl_ptr)
    {
        ctrl_ptr->style_info.bg_info.bg_type = GUI_BG_IMG;
        ctrl_ptr->style_info.bg_info.img_id = image_id;
    }

    return;
}


/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC void CTRLSTATUSBAR_SetState (
    MMI_HANDLE_T win_handle,
    uint32 ctrl_state,
    BOOLEAN is_set
)
{
    CTRLSTATUSBAR_OBJ_T *ctrl_ptr = GetStatusbarPtr(MMK_GetWinStatusBarHandle(win_handle));

    if (PNULL != ctrl_ptr)
    {
        SetState (&ctrl_ptr->ctrl_state, ctrl_state, is_set);
    }

    return;
}


/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLSTATUSBAR_GetState (
    MMI_HANDLE_T win_handle,
    uint32 ctrl_state
)
{
    BOOLEAN result = FALSE;
    CTRLSTATUSBAR_OBJ_T *ctrl_ptr = GetStatusbarPtr(MMK_GetWinStatusBarHandle(win_handle));

    if (PNULL != ctrl_ptr)
    {
        result = GetState (ctrl_ptr->ctrl_state, ctrl_state);
    }

    return result;
}


/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void CTRLSTATUSBAR_SetBg (
    MMI_HANDLE_T win_handle,
    GUI_BG_T  bg_info
)
{
    CTRLSTATUSBAR_OBJ_T *ctrl_ptr = GetStatusbarPtr(MMK_GetWinStatusBarHandle(win_handle));

    if (PNULL != ctrl_ptr)
    {
        ctrl_ptr->style_info.bg_info = bg_info;
    }

    return;
}


/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void CTRLSTATUSBAR_SetCommonBg (
    MMI_HANDLE_T win_handle,
    GUI_BG_T  bg_info
)
{
    CTRLSTATUSBAR_OBJ_T *ctrl_ptr = GetStatusbarPtr(MMK_GetWinStatusBarHandle(win_handle));

    if (PNULL != ctrl_ptr)
    {
        ctrl_ptr->base_ctrl.bg = bg_info;
    }

    return;
}


/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E CTRLSTATUSBAR_UpdateCtrl (
    MMI_HANDLE_T win_handle,
    BOOLEAN is_full_paint
)
{
    CTRLSTATUSBAR_OBJ_T *ctrl_ptr = GetStatusbarPtr(MMK_GetWinStatusBarHandle(win_handle));

    if (PNULL != ctrl_ptr)
    {
        return RefreshStatusBar (ctrl_ptr, is_full_paint);
    }
    else
    {
        return MMI_RESULT_FALSE;
    }
}


/*****************************************************************************/
//  Description : 动态创建状态栏控件
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLSTATUSBAR_CreateCtrlDyn (
    MMI_HANDLE_T win_handle
)
{
    MMI_CONTROL_CREATE_T     create    = {0};
    GUISTATUSBAR_INIT_DATA_T init_data = {0};

    init_data.both_rect = MMITHEME_GetStatusBarBothRect();

    create.ctrl_id = MMITHEME_GetStatusBarCtrlId();
    create.guid    = SPRD_GUI_STATUSBAR_ID;
    create.parent_win_handle = win_handle;
    create.init_data_ptr = &init_data;

    MMK_CreateControl (&create);

    return TRUE;
}


/*****************************************************************************/
//  Description : 动态创建状态栏控件，用户可以自定义控件的区域
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLSTATUSBAR_CustomCreate (
    MMI_HANDLE_T win_handle,
    GUI_BOTH_RECT_T *rect_ptr
)
{
    BOOLEAN                  result    = FALSE;
    MMI_CONTROL_CREATE_T     create    = {0};
    GUISTATUSBAR_INIT_DATA_T init_data = {0};

    if (PNULL != rect_ptr)
    {
        init_data.both_rect = *rect_ptr;

        create.ctrl_id = MMITHEME_GetStatusBarCtrlId();
        create.guid    = SPRD_GUI_STATUSBAR_ID;
        create.parent_win_handle = win_handle;
        create.init_data_ptr = &init_data;

        MMK_CreateControl (&create);

        result = TRUE;
    }

    return result;
}


/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author: xiyuan.ma
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLSTATUSBAR_SetItemBg (
    uint32            item_index,
    GUI_BG_T          *bg_info_ptr
)
{
    BOOLEAN  result = FALSE;

    result = GUISTABATA_SetItemBgInter (item_index, bg_info_ptr);

    return result;
}


/*****************************************************************************/
//  Description : 用户获取statusbar的层信息
//  Global resource dependence :
//  Author:xiyuan.ma
//  Note:
/*****************************************************************************/
PUBLIC void CTRLSTATUSBAR_GetCtrlLayer (
    MMI_HANDLE_T win_handle,
    GUI_LCD_DEV_INFO *dev_info_ptr
)
{
    CTRLSTATUSBAR_OBJ_T *ctrl_ptr = GetStatusbarPtr(MMK_GetWinStatusBarHandle(win_handle));

    if (PNULL != ctrl_ptr)
    {
        *dev_info_ptr = GetStatusBarLcdDevInfo (ctrl_ptr);
    }

    return;
}


#if defined PDA_UI_DROPDOWN_WIN || defined DROPDOWN_NOTIFY_SHORTCUT
/*****************************************************************************/
//  Description : Set Status Bar DropDown State
//  Global resource dependence : none
//  Author: Paul.huang
//  Note:
/*****************************************************************************/
PUBLIC void CTRLSTATUSBAR_SetDropDownState (
    MMI_HANDLE_T win_handle,
    BOOLEAN enable
)
{
    CTRLSTATUSBAR_OBJ_T *ctrl_ptr = GetStatusbarPtr(MMK_GetWinStatusBarHandle(win_handle));

    if (PNULL != ctrl_ptr)
    {
        ctrl_ptr->dropdown_state = enable;
    }

    return;
}


/*****************************************************************************/
//  Description : get Status Bar DropDown State
//  Global resource dependence : none
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLSTATUSBAR_IsDropDown (
    MMI_HANDLE_T win_handle
)
{
    CTRLSTATUSBAR_OBJ_T *ctrl_ptr = GetStatusbarPtr(MMK_GetWinStatusBarHandle(win_handle));

    if (PNULL != ctrl_ptr)
    {
        return ctrl_ptr->dropdown_state;
    }

    return FALSE;
}


/*****************************************************************************/
//  Description : Set Status Bar DropDown State
//  Global resource dependence : none
//  Author: Paul.huang
//  Note:
/*****************************************************************************/
PUBLIC void CTRLSTATUSBAR_SetPressed (
    MMI_HANDLE_T win_handle,
    BOOLEAN is_true
)
{
    CTRLSTATUSBAR_OBJ_T *ctrl_ptr = GetStatusbarPtr(MMK_GetWinStatusBarHandle(win_handle));

    if (PNULL != ctrl_ptr)
    {
        ctrl_ptr->is_press_down = is_true;
    }

    return;
}


/*****************************************************************************/
//  Description : get Status Bar DropDown State
//  Global resource dependence : none
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLSTATUSBAR_IsPressed (
    MMI_HANDLE_T win_handle
)
{
    CTRLSTATUSBAR_OBJ_T *ctrl_ptr = GetStatusbarPtr(MMK_GetWinStatusBarHandle(win_handle));

    if (PNULL != ctrl_ptr)
    {
        return ctrl_ptr->is_press_down;
    }

    return FALSE;
}
#endif


