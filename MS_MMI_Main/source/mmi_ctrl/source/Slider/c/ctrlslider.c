/*****************************************************************************
** File Name:      ctrlslider.c                                              *
** Author:                                                                   *
** Date:           08/13/2012                                                *
** Copyright:      2004 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 08/2012      xiaoqing.lu              Creat
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
#include "mmitheme_slider.h"
#include "ctrlslider.h"
#include "guicommon.h"
#include "mmk_tp.h"
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#define CTRL_SLIDER_TIMEOUT 100

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
//  Description : init slider class
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SliderCtrlInitVtbl(CTRLSLIDER_VTBL_T *slider_vtbl_ptr);

/*****************************************************************************/
//  Description : slider construct
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SliderCtrlConstruct(CTRLBASE_OBJ_T *base_ctrl_ptr,
                                  CTRLSLIDER_INIT_PARAM_T *slider_param_ptr);

/*****************************************************************************/
//  Description : slider destruct
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SliderCtrlDestruct(CTRLBASE_OBJ_T *base_ctrl_ptr);

/*****************************************************************************/
//  Description : convert init param
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void
SliderCtrlPackInitParam(GUISLIDER_INIT_DATA_T *slider_init_ptr,   // in
                        CTRLSLIDER_INIT_PARAM_T *slider_param_ptr // out
);

/*****************************************************************************/
//  Description : handle slider msg function
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E SliderCtrlHandleMsg(
    CTRLBASE_OBJ_T *base_ctrl_ptr, // control pointer
    MMI_MESSAGE_ID_E msg_id,       // message
    DPARAM param                   // add data
);

/*****************************************************************************/
//  Description : get slider pointer by control id
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL CTRLSLIDER_OBJ_T *GetSliderPtr(MMI_HANDLE_T ctrl_handle);

/*****************************************************************************/
//  Description : ext type of
//  Global resource dependence :
//  Author:hua.fang
//  Note: judge control type of
/*****************************************************************************/
LOCAL BOOLEAN SliderTypeOf(CTRLBASE_OBJ_T *ctrl_ptr);

/*****************************************************************************/
//  Description : update channel & thumb rect
//  Global resource dependence :
//  Author: xiaoqing.lu & nan.ji
//  Note:
/*****************************************************************************/
LOCAL void UpdateSliderRect(CTRLSLIDER_OBJ_T *slider_ctrl_ptr);

/*****************************************************************************/
//  Description : display slider image
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void SliderDrawImage(CTRLSLIDER_OBJ_T *slider_ctrl_ptr,
                           CTRLSLIDER_IMAGE_T image_info,
                           GUI_RECT_T *disp_rect_ptr);

/*****************************************************************************/
//  Description : display slider bg
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void SliderDrawBg(CTRLSLIDER_OBJ_T *slider_ctrl_ptr);

/*****************************************************************************/
//  Description : display slider thumb
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void SliderDrawThumb(CTRLSLIDER_OBJ_T *slider_ctrl_ptr);

/*****************************************************************************/
//  Description : display slider fg
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void SliderDrawChannelFg(CTRLSLIDER_OBJ_T *slider_ctrl_ptr);

/*****************************************************************************/
//  Description : display slider fg
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void SliderDrawChannelBg(CTRLSLIDER_OBJ_T *slider_ctrl_ptr);

/*****************************************************************************/
//  Description : display slider control
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void SliderDraw(CTRLSLIDER_OBJ_T *slider_ctrl_ptr);

/*****************************************************************************/
// 	Description : get control height by width,include content,border etc.
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN SliderCtrlGetHeightByWidth(CTRLBASE_OBJ_T *base_ctrl_ptr, // in:
                                         uint16 width,                  // in:
                                         uint16 *height_ptr // in/out:
);

/*****************************************************************************/
// 	Description : set rect
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN SliderCtrlSetRect(CTRLBASE_OBJ_T *base_ctrl_ptr,
                                const GUI_RECT_T *rect_ptr);

/*****************************************************************************/
// 	Description : set rect
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN SliderCtrlSetDispRect(CTRLBASE_OBJ_T *base_ctrl_ptr,
                                    const GUI_RECT_T *rect_ptr,
                                    BOOLEAN is_update);

/*****************************************************************************/
//  Description : set cur by size (line_size or page_size)
//  Global resource dependence :
//  Author: xiaoqing.lu & nan.ji
//  Note: return to head or to tail
/*****************************************************************************/
LOCAL BOOLEAN SliderSetCurBySize(CTRLSLIDER_OBJ_T *slider_ctrl_ptr, int16 size);

/*****************************************************************************/
//  Description : handle slider msg function
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SliderCtrlHandleTpDown(
    CTRLSLIDER_OBJ_T *slider_ctrl_ptr, // control pointer
    DPARAM param                       // add data
);

/*****************************************************************************/
//  Description : handle slider msg function
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SliderCtrlHandleTpMove(
    CTRLSLIDER_OBJ_T *slider_ctrl_ptr, // control pointer
    DPARAM param                       // add data
);

/*****************************************************************************/
//  Description : handle slider msg function
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SliderCtrlHandleTpUp(
    CTRLSLIDER_OBJ_T *slider_ctrl_ptr, // control pointer
    DPARAM param                       // add data
);

/*****************************************************************************/
//  Description : handle slider msg function
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E SliderCtrlHandleTimer(
    CTRLSLIDER_OBJ_T *slider_ctrl_ptr, // control pointer
    DPARAM param                       // add data
);

/*****************************************************************************/
//  Description : SliderCtrlAdjuestCur
//  Global resource dependence :
//  Author: xiaoqing.lu & nan.ji
//  Note: return to head or to tail
/*****************************************************************************/
LOCAL void SliderCtrlAdjuestCur(CTRLSLIDER_OBJ_T *slider_ctrl_ptr);

/**--------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/

LOCAL void SLIDER_PostNotifyMsg(const CTRLSLIDER_OBJ_T *slider_ptr,
                                MMI_MESSAGE_ID_E notify_code,
                                CTRLSLIDER_MODE_E slider_mdoe);

/*****************************************************************************/
//  Description : send slider msg
//  Global resource dependence :
//  Author:ningbiao.huang
//  Note:
/*****************************************************************************/
LOCAL void SLIDER_PostNotifyMsg(const CTRLSLIDER_OBJ_T *slider_ptr,
                                MMI_MESSAGE_ID_E notify_code,
                                CTRLSLIDER_MODE_E slider_mdoe)
{
    CTRLBASE_OBJ_T *base_ctrl_ptr = (CTRLBASE_OBJ_T *)slider_ptr;
    CTRLSLIDER_NOTIFY_T notify = {0};

    if (PNULL == slider_ptr)
    {
        return;
    }
    notify.notify.src_handle = base_ctrl_ptr->handle;
    notify.notify.src_id = MMK_GetCtrlId(base_ctrl_ptr->handle);
    notify.notify.code = notify_code;
    notify.cur_position = slider_ptr->cur;
    notify.slider_mode = slider_mdoe;

    CTRLMSG_PostNotifyEx(base_ctrl_ptr->handle, notify_code, &notify,
                         sizeof(notify));
}

/*****************************************************************************/
//  Description : get slider type
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC TYPE_TAG_T SLIDER_GetType()
{
    static TYPE_TAG_T obj_type = 0;

    if (0 == obj_type)
    {
        obj_type = TYPEMNG_Register(
            CTRL_BASE_TYPE, "slider", sizeof(CTRLSLIDER_OBJ_T),
            (OBJECT_CONSTRUCTOR_FUNC)SliderCtrlConstruct,
            (OBJECT_DESTRUCTOR_FUNC)SliderCtrlDestruct,
            sizeof(CTRLSLIDER_INIT_PARAM_T),
            (PARAM_PACKING_FUNC)SliderCtrlPackInitParam,
            sizeof(CTRLSLIDER_VTBL_T), (VTBL_INIT_FUNC)SliderCtrlInitVtbl);
    }

    return obj_type;
}

/*****************************************************************************/
//  Description : init slider class
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SliderCtrlInitVtbl(CTRLSLIDER_VTBL_T *slider_vtbl_ptr)
{
    CTRLBASE_VTBL_T *base_ctrl_vtbl_ptr = (CTRLBASE_VTBL_T *)slider_vtbl_ptr;

    if (PNULL == base_ctrl_vtbl_ptr)
    {
        return FALSE;
    }

    base_ctrl_vtbl_ptr->HandleEvent = SliderCtrlHandleMsg;

    // 以下接口用于实现内嵌form
    base_ctrl_vtbl_ptr->SetRect = SliderCtrlSetRect;
    base_ctrl_vtbl_ptr->SetDisplayRect = SliderCtrlSetDispRect;
    base_ctrl_vtbl_ptr->GetHeightByWidth = SliderCtrlGetHeightByWidth;

    return TRUE;
}

/*****************************************************************************/
//  Description : convert init param
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void
SliderCtrlPackInitParam(GUISLIDER_INIT_DATA_T *slider_init_ptr,   // in
                        CTRLSLIDER_INIT_PARAM_T *slider_param_ptr // out
)
{
    CTRLBASE_INIT_PARAM_T *base_ctrl_param_ptr =
        (CTRLBASE_INIT_PARAM_T *)slider_param_ptr;

    if (PNULL != slider_init_ptr && PNULL != slider_param_ptr)
    {
        base_ctrl_param_ptr->both_rect = slider_init_ptr->both_rect;
    }
}

/*****************************************************************************/
//  Description : slider construct
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SliderCtrlConstruct(CTRLBASE_OBJ_T *base_ctrl_ptr,
                                  CTRLSLIDER_INIT_PARAM_T *slider_param_ptr)
{
    BOOLEAN result = TRUE;
    MMI_SLIDER_STYLE_T slider_style = {0};
    CTRLSLIDER_OBJ_T *slider_ctrl_ptr = (CTRLSLIDER_OBJ_T *)base_ctrl_ptr;

    if (PNULL != slider_ctrl_ptr)
    {
        slider_ctrl_ptr->is_dirty = TRUE;

        MMITHEME_GetSliderStyle(&slider_style);

        slider_ctrl_ptr->is_disp_bg = slider_style.is_has_bg;

        slider_ctrl_ptr->channel_bg_info.color = slider_style.channel_bg_color;
        slider_ctrl_ptr->channel_bg_info.image_id =
            slider_style.channel_bg_image_id;
        slider_ctrl_ptr->channel_bg_info.disable_color =
            slider_style.channel_disable_bg_color;
        slider_ctrl_ptr->channel_bg_info.disable_image_id =
            slider_style.channel_disable_bg_image_id;

        slider_ctrl_ptr->channel_fg_info.is_true = slider_style.is_disp_fg;
        slider_ctrl_ptr->channel_fg_info.color = slider_style.channel_fg_color;
        slider_ctrl_ptr->channel_fg_info.image_id =
            slider_style.channel_fg_image_id;
        slider_ctrl_ptr->channel_fg_info.disable_color =
            slider_style.channel_disable_fg_color;
        slider_ctrl_ptr->channel_fg_info.disable_image_id =
            slider_style.channel_disable_fg_image_id;

        slider_ctrl_ptr->thumb_info.is_true = slider_style.is_has_thumb;
        slider_ctrl_ptr->thumb_info.color = slider_style.thumb_color;
        slider_ctrl_ptr->thumb_info.image_id = slider_style.thumb_image_id;
        slider_ctrl_ptr->thumb_info.disable_color =
            slider_style.disable_thumb_color;
        slider_ctrl_ptr->thumb_info.disable_image_id =
            slider_style.disable_thumb_image_id;

        slider_ctrl_ptr->is_vertical = FALSE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : slider destruct
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SliderCtrlDestruct(CTRLBASE_OBJ_T *base_ctrl_ptr)
{
    BOOLEAN result = TRUE;
    CTRLSLIDER_OBJ_T *slider_ctrl_ptr = (CTRLSLIDER_OBJ_T *)base_ctrl_ptr;

    if (PNULL != slider_ctrl_ptr)
    {
        SCI_TRACE_LOW("CTRLSLIDER SliderCtrlDestruct!");
    }

    return (result);
}

/*****************************************************************************/
//  Description : set cur by size (line_size or page_size)
//  Global resource dependence :
//  Author: xiaoqing.lu & nan.ji
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SliderSetCurBySize(CTRLSLIDER_OBJ_T *slider_ctrl_ptr, int16 size)
{
    BOOLEAN result = FALSE;

    if (PNULL != slider_ctrl_ptr)
    {
        int16 pre_cur = slider_ctrl_ptr->cur;

        slider_ctrl_ptr->is_dirty = TRUE;
        slider_ctrl_ptr->cur += size;

        // 纠正
        // min cur is 0
        if (slider_ctrl_ptr->cur < 0)
        {
            slider_ctrl_ptr->cur = 0;
            result = TRUE;
        }

        // 纠正
        if (slider_ctrl_ptr->cur > slider_ctrl_ptr->max)
        {
            slider_ctrl_ptr->cur = slider_ctrl_ptr->max;
            result = TRUE;
        }

        // 如果变化，需要发送消息给应用
        if (pre_cur != slider_ctrl_ptr->cur)
        {
            // post msg to win
            // MMK_PostMsg(base_ctrl_ptr->win_handle, MSG_NOTIFY_SLIDER_CHANGED,
            // PNULL, 0);
            SLIDER_PostNotifyMsg(slider_ctrl_ptr, MSG_NOTIFY_SLIDER_CHANGED,
                                 CTRLSLIDER_MODE_VALUE_CHANGED);
        }
        else
        {
            result = TRUE;
        }
    }

    return result;
}

/*****************************************************************************/
//  Description : SliderCtrlAdjuestCur
//  Global resource dependence :
//  Author: xiaoqing.lu & nan.ji
//  Note: return to head or to tail
/*****************************************************************************/
LOCAL void SliderCtrlAdjuestCur(CTRLSLIDER_OBJ_T *slider_ctrl_ptr)
{
    if (PNULL != slider_ctrl_ptr)
    {
        // min cur is 0
        if (slider_ctrl_ptr->cur < 0)
        {
            slider_ctrl_ptr->cur = 0;
            slider_ctrl_ptr->is_dirty = TRUE;
        }

        if (slider_ctrl_ptr->cur > slider_ctrl_ptr->max)
        {
            slider_ctrl_ptr->cur = slider_ctrl_ptr->max;
            slider_ctrl_ptr->is_dirty = TRUE;
        }
    }

    return;
}

/*****************************************************************************/
//  Description : handle slider msg function
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL int16 SliderCtrlGetMoveSize(CTRLSLIDER_OBJ_T *slider_ctrl_ptr)
{
    int16 size = 0;

    if (PNULL != slider_ctrl_ptr)
    {
        // size = slider_ctrl_ptr->page_size; // page size
        size = slider_ctrl_ptr->tp_pos; // tp pos

        if (slider_ctrl_ptr->is_vertical)
        {
            if (slider_ctrl_ptr->is_slide_negative) // 判断正负
            {
                // size = -size;
                size = -(slider_ctrl_ptr->cur - size);
            }
        }
        else
        {
            if (slider_ctrl_ptr->is_slide_negative) // 判断正负
            {
                // size = -size;
                size = -(slider_ctrl_ptr->cur - size);
            }
        }

        // 校正，防止移动过了
        if (slider_ctrl_ptr->is_slide_negative)
        {
            if (size + slider_ctrl_ptr->cur < slider_ctrl_ptr->tp_pos)
            {
                size = slider_ctrl_ptr->tp_pos - slider_ctrl_ptr->cur;
            }
        }
        else
        {
            if (size + slider_ctrl_ptr->cur > slider_ctrl_ptr->tp_pos)
            {
                size = slider_ctrl_ptr->tp_pos - slider_ctrl_ptr->cur;
            }
        }
    }

    return size;
}

/*****************************************************************************/
//  Description : handle slider msg function
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SliderCtrlHandleTpDown(
    CTRLSLIDER_OBJ_T *slider_ctrl_ptr, // control pointer
    DPARAM param                       // add data
)
{
    BOOLEAN result = FALSE;

    if (PNULL != slider_ctrl_ptr)
    {
        int16 channel_width = 0;
        int16 range_size = 0;
        int16 space_size = 0;
        CTRLBASE_OBJ_T *base_ctrl_ptr = (CTRLBASE_OBJ_T *)slider_ctrl_ptr;

        result = TRUE;

        slider_ctrl_ptr->tp_down_point.x = MMK_GET_TP_X(param);
        slider_ctrl_ptr->tp_down_point.y = MMK_GET_TP_Y(param);
        slider_ctrl_ptr->tp_pre_point.x = MMK_GET_TP_X(param);
        slider_ctrl_ptr->tp_pre_point.y = MMK_GET_TP_Y(param);

        UpdateSliderRect(slider_ctrl_ptr);

        if (GUI_PointIsInRect(slider_ctrl_ptr->tp_down_point,
                              slider_ctrl_ptr->thumb_rect)) // 滑块上
        {
            slider_ctrl_ptr->is_thumb_pressed = TRUE;
            // send tp down msg
            SLIDER_PostNotifyMsg(slider_ctrl_ptr, MSG_NOTIFY_SLIDER_CHANGED,
                                 CTRLSLIDER_MODE_START);
        }
        else if (GUI_PointIsInRect(slider_ctrl_ptr->tp_down_point,
                                   slider_ctrl_ptr->channel_rect)
                 && 0 != slider_ctrl_ptr->channel_width
                 && 0 != slider_ctrl_ptr->channel_height) // 槽内，但不在滑块
        {
            int16 move_size = 0;

            slider_ctrl_ptr->is_thumb_pressed = FALSE;

            slider_ctrl_ptr->is_slide_negative = FALSE;

            range_size = slider_ctrl_ptr->max - slider_ctrl_ptr->min;

            // 计算tp点击的位置
            if (slider_ctrl_ptr->is_vertical)
            {
                channel_width = slider_ctrl_ptr->channel_height;
                space_size = slider_ctrl_ptr->tp_down_point.y
                             - slider_ctrl_ptr->channel_rect.top;

                // slider_ctrl_ptr->tp_pos =
                //    slider_ctrl_ptr->min
                //    + space_size * range_size / channel_width;
                slider_ctrl_ptr->tp_pos =
                    space_size * range_size / channel_width;

                if (slider_ctrl_ptr->tp_down_point.y
                    < slider_ctrl_ptr->thumb_rect.top)
                {
                    slider_ctrl_ptr->is_slide_negative = TRUE;
                }
            }
            else
            {
                channel_width = slider_ctrl_ptr->channel_width;
                space_size = slider_ctrl_ptr->tp_down_point.x
                             - slider_ctrl_ptr->channel_rect.left;
                // slider_ctrl_ptr->tp_pos =
                //    slider_ctrl_ptr->min
                //    + space_size * range_size / channel_width;
                slider_ctrl_ptr->tp_pos =
                    space_size * range_size / channel_width;

                if (slider_ctrl_ptr->tp_down_point.x
                    < slider_ctrl_ptr->thumb_rect.left)
                {
                    slider_ctrl_ptr->is_slide_negative = TRUE;
                }
            }

            move_size =
                SliderCtrlGetMoveSize(slider_ctrl_ptr); // 计算移动的size

            if (!SliderSetCurBySize(slider_ctrl_ptr,
                                    move_size)) // 有变化，启动timer
            {
                slider_ctrl_ptr->slider_timer_id = MMK_CreateWinTimer(
                    base_ctrl_ptr->handle, CTRL_SLIDER_TIMEOUT, FALSE);
            }

            SliderDraw(slider_ctrl_ptr);
        }
        else
        {
            slider_ctrl_ptr->is_thumb_pressed = FALSE;
        }
    }

    return result;
}

/*****************************************************************************/
//  Description : handle slider msg function
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SliderCtrlHandleTpMove(
    CTRLSLIDER_OBJ_T *slider_ctrl_ptr, // control pointer
    DPARAM param                       // add data
)
{
    BOOLEAN result = FALSE;
    GUI_POINT_T cur_point = {0};
    int16 move_space = 0;

    if (PNULL != slider_ctrl_ptr)
    {
        MMI_TP_STATUS_E state = MMI_TP_NONE;
        CTRLBASE_OBJ_T *base_ctrl_ptr = (CTRLBASE_OBJ_T *)slider_ctrl_ptr;

        result = TRUE;

        MMK_StopTimer(slider_ctrl_ptr->slider_timer_id);
        slider_ctrl_ptr->slider_timer_id = 0;

        cur_point.x = MMK_GET_TP_X(param);
        cur_point.y = MMK_GET_TP_Y(param);

        MMK_GetLogicTPMsg(&state, &cur_point);

        if (slider_ctrl_ptr->is_thumb_pressed
            && GUI_PointIsInRect(cur_point, base_ctrl_ptr->rect))
        {

            if (slider_ctrl_ptr->is_vertical)
            {
                move_space = cur_point.y - slider_ctrl_ptr->tp_pre_point.y;
                slider_ctrl_ptr->thumb_rect.top += move_space;
                slider_ctrl_ptr->thumb_rect.bottom += move_space;

                if (slider_ctrl_ptr->thumb_rect.top
                    < slider_ctrl_ptr->channel_rect.top
                          - slider_ctrl_ptr->thumb_height / 2)
                {
                    slider_ctrl_ptr->thumb_rect.top =
                        slider_ctrl_ptr->channel_rect.top
                        - slider_ctrl_ptr->thumb_height / 2;
                    slider_ctrl_ptr->thumb_rect.bottom =
                        slider_ctrl_ptr->thumb_rect.top
                        + slider_ctrl_ptr->thumb_height - 1;
                }

                if (slider_ctrl_ptr->thumb_rect.bottom
                    > slider_ctrl_ptr->channel_rect.bottom
                          + slider_ctrl_ptr->thumb_height / 2)
                {
                    slider_ctrl_ptr->thumb_rect.bottom =
                        slider_ctrl_ptr->channel_rect.bottom
                        + slider_ctrl_ptr->thumb_height / 2;
                    slider_ctrl_ptr->thumb_rect.top =
                        slider_ctrl_ptr->thumb_rect.bottom
                        - slider_ctrl_ptr->thumb_height + 1;
                }
            }
            else
            {
                move_space = cur_point.x - slider_ctrl_ptr->tp_pre_point.x;

                slider_ctrl_ptr->thumb_rect.left += move_space;
                slider_ctrl_ptr->thumb_rect.right += move_space;

                if (slider_ctrl_ptr->thumb_rect.left
                    < slider_ctrl_ptr->channel_rect.left
                          - slider_ctrl_ptr->thumb_width / 2)
                {
                    slider_ctrl_ptr->thumb_rect.left =
                        slider_ctrl_ptr->channel_rect.left
                        - slider_ctrl_ptr->thumb_width / 2;
                    slider_ctrl_ptr->thumb_rect.right =
                        slider_ctrl_ptr->thumb_rect.left
                        + slider_ctrl_ptr->thumb_width - 1;
                }

                if (slider_ctrl_ptr->thumb_rect.right
                    > slider_ctrl_ptr->channel_rect.right
                          + slider_ctrl_ptr->thumb_width / 2)
                {
                    slider_ctrl_ptr->thumb_rect.right =
                        slider_ctrl_ptr->channel_rect.right
                        + slider_ctrl_ptr->thumb_width / 2;
                    slider_ctrl_ptr->thumb_rect.left =
                        slider_ctrl_ptr->thumb_rect.right
                        - slider_ctrl_ptr->thumb_width + 1;
                }
            }

            if (0 != slider_ctrl_ptr->max - slider_ctrl_ptr->min)
            {
                SliderSetCurBySize(
                    slider_ctrl_ptr,
                    move_space * (slider_ctrl_ptr->max - slider_ctrl_ptr->min)
                        / (slider_ctrl_ptr->channel_width));
                slider_ctrl_ptr->is_dirty = FALSE;
            }

            // send tp move msg
            SLIDER_PostNotifyMsg(slider_ctrl_ptr, MSG_NOTIFY_SLIDER_CHANGED,
                                 CTRLSLIDER_MODE_MOVE);

            SliderDraw(slider_ctrl_ptr);
        }
        else
        {
            slider_ctrl_ptr->is_thumb_pressed = FALSE;
        }
        // 记录pre point
        slider_ctrl_ptr->tp_pre_point.x = cur_point.x;
        slider_ctrl_ptr->tp_pre_point.y = cur_point.y;
    }

    return result;
}

/*****************************************************************************/
//  Description : handle slider msg function
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SliderCtrlHandleTpUp(
    CTRLSLIDER_OBJ_T *slider_ctrl_ptr, // control pointer
    DPARAM param                       // add data
)
{
    BOOLEAN result = FALSE;

    if (PNULL != slider_ctrl_ptr)
    {
        result = TRUE;

        slider_ctrl_ptr->is_thumb_pressed = FALSE;
        MMK_StopTimer(slider_ctrl_ptr->slider_timer_id);
        slider_ctrl_ptr->slider_timer_id = 0;

        // send tp up msg
        SLIDER_PostNotifyMsg(slider_ctrl_ptr, MSG_NOTIFY_SLIDER_CHANGED,
                             CTRLSLIDER_MODE_END);

        SliderDraw(slider_ctrl_ptr);
    }

    return result;
}

/*****************************************************************************/
//  Description : handle slider msg function
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E SliderCtrlHandleTimer(
    CTRLSLIDER_OBJ_T *slider_ctrl_ptr, // control pointer
    DPARAM param                       // add data
)
{
    int16 size = 0;
    MMI_RESULT_E result = MMI_RESULT_FALSE;

    if (*(uint8 *)param == slider_ctrl_ptr->slider_timer_id)
    {
        CTRLBASE_OBJ_T *base_ctrl_ptr = (CTRLBASE_OBJ_T *)slider_ctrl_ptr;

        MMK_StopTimer(slider_ctrl_ptr->slider_timer_id);
        slider_ctrl_ptr->slider_timer_id = 0;

        size = SliderCtrlGetMoveSize(slider_ctrl_ptr);

        if (!SliderSetCurBySize(slider_ctrl_ptr, size))
        {
            slider_ctrl_ptr->slider_timer_id = MMK_CreateWinTimer(
                base_ctrl_ptr->handle, CTRL_SLIDER_TIMEOUT, FALSE);
        }

        SliderDraw(slider_ctrl_ptr);

        result = MMI_RESULT_TRUE;
    }

    return result;
}

/*****************************************************************************/
//  Description : handle slider msg function
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E SliderCtrlHandleMsg(
    CTRLBASE_OBJ_T *base_ctrl_ptr, // control pointer
    MMI_MESSAGE_ID_E msg_id,       // message
    DPARAM param                   // add data
)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    CTRLSLIDER_OBJ_T *slider_ctrl_ptr = (CTRLSLIDER_OBJ_T *)base_ctrl_ptr;
    CTRLBASE_VTBL_T *parent_vtbl_ptr = (CTRLBASE_VTBL_T *)TYPEMNG_GetParentVtbl(
        CTRL_SLIDER_TYPE, (OBJECT_TYPE_PTR)base_ctrl_ptr);

    if (PNULL == slider_ctrl_ptr)
    {
        return MMI_RESULT_FALSE;
    }

    switch (msg_id)
    {
    case MSG_CTL_PAINT:
        SliderDraw(slider_ctrl_ptr);
        break;

    case MSG_CTL_GET_ACTIVE:
        result = MMI_RESULT_FALSE;
        break;

    case MSG_CTL_LOSE_ACTIVE:
    case MSG_CTL_LOSE_FOCUS:
    case MSG_CTL_CLOSE:
        MMK_StopTimer(slider_ctrl_ptr->slider_timer_id);
        slider_ctrl_ptr->slider_timer_id = 0;
        slider_ctrl_ptr->is_thumb_pressed = FALSE;
        break;

    case MSG_NOTIFY_LOSE_ACTIVE:
        MMK_StopTimer(slider_ctrl_ptr->slider_timer_id);
        slider_ctrl_ptr->slider_timer_id = 0;
        slider_ctrl_ptr->is_thumb_pressed = FALSE;
        break;

    case MSG_APP_LEFT:
    case MSG_APP_UP:
    case MSG_KEYREPEAT_LEFT:
    case MSG_KEYREPEAT_UP:
        SliderSetCurBySize(slider_ctrl_ptr, -(slider_ctrl_ptr->line_size));
        SliderDraw(slider_ctrl_ptr); // ??
        break;

    case MSG_APP_RIGHT:
    case MSG_APP_DOWN:
    case MSG_KEYREPEAT_RIGHT:
    case MSG_KEYREPEAT_DOWN:
        SliderSetCurBySize(slider_ctrl_ptr, slider_ctrl_ptr->line_size);
        SliderDraw(slider_ctrl_ptr); // ??
        break;

    case MSG_TIMER:
        SliderCtrlHandleTimer(slider_ctrl_ptr, param);
        break;

#ifdef TOUCH_PANEL_SUPPORT
    case MSG_TP_PRESS_DOWN:
        SliderCtrlHandleTpDown(slider_ctrl_ptr, param);
        break;

    case MSG_TP_PRESS_MOVE:
        SliderCtrlHandleTpMove(slider_ctrl_ptr, param);
        break;

    case MSG_TP_PRESS_UP:
        SliderCtrlHandleTpUp(slider_ctrl_ptr, param);
        break;
#endif

    default:
        result = parent_vtbl_ptr->HandleEvent(base_ctrl_ptr, msg_id, param);
        break;
    }

    return (result);
}

/*****************************************************************************/
//  Description : update channel & thumb rect
//  Global resource dependence :
//  Author: xiaoqing.lu & nan.ji
//  Note:
/*****************************************************************************/
LOCAL void UpdateSliderRect(CTRLSLIDER_OBJ_T *slider_ctrl_ptr)
{
    GUI_RECT_T channel_rect = {0};
    GUI_RECT_T thumb_rect = {0};
    GUI_POINT_T thumb_pos = {0};
    CTRLBASE_OBJ_T *base_ctrl_ptr = PNULL;

    base_ctrl_ptr = (CTRLBASE_OBJ_T *)slider_ctrl_ptr;

    if (PNULL != slider_ctrl_ptr)
    {
        SliderCtrlAdjuestCur(slider_ctrl_ptr);

        if (slider_ctrl_ptr->is_dirty)
        {
            // cal channel rect
            channel_rect = base_ctrl_ptr->rect;

            if (!GUI_IsInvalidRect(channel_rect))
            {
                channel_rect = GUI_GetCenterRect(
                    channel_rect, slider_ctrl_ptr->channel_width,
                    slider_ctrl_ptr->channel_height);
            }

            slider_ctrl_ptr->channel_rect = channel_rect;

            if (slider_ctrl_ptr->is_vertical)
            {
                // cal thumb rect
                if (0 != slider_ctrl_ptr->max - slider_ctrl_ptr->min)
                {
                    thumb_pos.y =
                        channel_rect.top
                        + (slider_ctrl_ptr->channel_height
                           * slider_ctrl_ptr->cur
                           / (slider_ctrl_ptr->max - slider_ctrl_ptr->min));
                }
                else
                {
                    thumb_pos.y = channel_rect.top;
                }

                thumb_pos.x =
                    channel_rect.left + (slider_ctrl_ptr->channel_width) / 2;
            }
            else
            {
                // cal thumb rect
                if (0 != slider_ctrl_ptr->max - slider_ctrl_ptr->min)
                {
                    thumb_pos.x =
                        channel_rect.left
                        + (slider_ctrl_ptr->channel_width * slider_ctrl_ptr->cur
                           / (slider_ctrl_ptr->max - slider_ctrl_ptr->min));
                }
                else
                {
                    thumb_pos.x = channel_rect.left;
                }

                thumb_pos.y =
                    channel_rect.top + (slider_ctrl_ptr->channel_height) / 2;
            }

            thumb_rect.left = thumb_rect.right = thumb_pos.x;
            thumb_rect.top = thumb_rect.bottom = thumb_pos.y;
            thumb_rect =
                GUI_GetCenterRectExt(thumb_rect, slider_ctrl_ptr->thumb_width,
                                     slider_ctrl_ptr->thumb_height);

            slider_ctrl_ptr->thumb_rect = thumb_rect;
        }

        slider_ctrl_ptr->is_dirty = FALSE;
    }

    return;
}

/*****************************************************************************/
//  Description : display slider image
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void SliderDrawImage(CTRLSLIDER_OBJ_T *slider_ctrl_ptr,
                           CTRLSLIDER_IMAGE_T image_info,
                           GUI_RECT_T *disp_rect_ptr)
{
    GUI_COLOR_T color = 0;
    MMI_IMAGE_ID_T image_id = 0;
    // GUI_RECT_T disp_rect = {0};
    GUI_RECT_T image_rect = {0};
    uint16 image_width = 0;
    uint16 image_height = 0;
    CTRLBASE_OBJ_T *base_ctrl_ptr = PNULL;
    GUI_RECT_T cross_rect = {0};
    GUI_BG_DISPLAY_T bg_display = {0};
    GUI_BG_T bg_info = {0};

    base_ctrl_ptr = (CTRLBASE_OBJ_T *)slider_ctrl_ptr;

    if (PNULL != slider_ctrl_ptr
        && GUI_IntersectRect(&cross_rect, base_ctrl_ptr->display_rect,
                             *disp_rect_ptr)
        && !GUI_IsInvalidRect(cross_rect))
    {
        if (base_ctrl_ptr->is_enabled)
        {
            color = image_info.color;
            image_id = image_info.image_id;
        }
        else
        {
            color = image_info.disable_color;
            image_id = image_info.disable_image_id;
        }

        if (MMITHEME_CheckImageID(image_id))
        {
            // 计算背景图片显示的区域
            GUIRES_GetImgWidthHeight(&image_width, &image_height, image_id,
                                     base_ctrl_ptr->win_handle);
            image_rect.bottom =
                image_height
                - 1; // image_rect.top + (cross_rect.bottom - cross_rect.top);
            image_rect.right =
                image_width
                - 1; // image_rect.left + (cross_rect.right - cross_rect.left);

            if (disp_rect_ptr->top < cross_rect.top)
            {
                image_rect.top = (cross_rect.top - disp_rect_ptr->top);
                image_rect.bottom = image_height - 1;
            }
            else if (disp_rect_ptr->bottom > cross_rect.bottom)
            {
                image_rect.top = 0;
                image_rect.bottom =
                    image_height - (disp_rect_ptr->bottom - cross_rect.bottom);
            }
            else
            {
                image_rect.top = 0;
                image_rect.bottom = image_height - 1;
            }
            // set bg display info
            bg_display.is_transparent = FALSE;
            bg_display.rect = *disp_rect_ptr;
            bg_display.display_rect = cross_rect;
            bg_display.img_rect = image_rect;
            bg_display.ctrl_handle = base_ctrl_ptr->handle;
            bg_display.win_handle = base_ctrl_ptr->win_handle;

            bg_info.bg_type = GUI_BG_IMG;
            bg_info.img_id = image_id;
            GUI_DisplayBg(&bg_info, &bg_display,
                          &(base_ctrl_ptr->lcd_dev_info));
        }
        else
        {
            GUI_FillRect(&base_ctrl_ptr->lcd_dev_info, cross_rect, color);
        }
    }

    return;
}

/*****************************************************************************/
//  Description : display slider bg
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void SliderDrawBg(CTRLSLIDER_OBJ_T *slider_ctrl_ptr)
{
    GUI_RECT_T disp_rect = {0};
    CTRLBASE_OBJ_T *base_ctrl_ptr = PNULL;
    CTRLSLIDER_IMAGE_T bg_info = {0};

    base_ctrl_ptr = (CTRLBASE_OBJ_T *)slider_ctrl_ptr;

    if (PNULL != slider_ctrl_ptr && slider_ctrl_ptr->is_disp_bg)
    {
        disp_rect = base_ctrl_ptr->display_rect;

        bg_info.color = base_ctrl_ptr->bg.color;
        bg_info.image_id = base_ctrl_ptr->bg.img_id;
        bg_info.disable_color = base_ctrl_ptr->bg.color;
        bg_info.disable_image_id = base_ctrl_ptr->bg.img_id;

        SliderDrawImage(slider_ctrl_ptr, bg_info, &disp_rect);
    }
}

/*****************************************************************************/
//  Description : display slider thumb
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void SliderDrawThumb(CTRLSLIDER_OBJ_T *slider_ctrl_ptr)
{

    CTRLBASE_OBJ_T *base_ctrl_ptr = PNULL;

    base_ctrl_ptr = (CTRLBASE_OBJ_T *)slider_ctrl_ptr;

    if (PNULL != slider_ctrl_ptr && slider_ctrl_ptr->thumb_info.is_true)
    {
        SliderDrawImage(slider_ctrl_ptr, slider_ctrl_ptr->thumb_info,
                        &slider_ctrl_ptr->thumb_rect);
    }
}

/*****************************************************************************/
//  Description : display slider fg
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void SliderDrawChannelFg(CTRLSLIDER_OBJ_T *slider_ctrl_ptr)
{
    GUI_RECT_T channel_fg_rect = {0};
    CTRLBASE_OBJ_T *base_ctrl_ptr = PNULL;

    base_ctrl_ptr = (CTRLBASE_OBJ_T *)slider_ctrl_ptr;

    if (PNULL != slider_ctrl_ptr && slider_ctrl_ptr->channel_fg_info.is_true)
    {
        channel_fg_rect = slider_ctrl_ptr->channel_rect;

        if (slider_ctrl_ptr->is_vertical)
        {
            channel_fg_rect.bottom = slider_ctrl_ptr->thumb_rect.top;
            // thumb reach max,channel_fg's bottom draw to thumb's bottom.
            if (slider_ctrl_ptr->channel_rect.bottom
                <= slider_ctrl_ptr->thumb_rect.bottom)
            {
                channel_fg_rect.bottom = slider_ctrl_ptr->thumb_rect.bottom;
            }
        }
        else
        {
            channel_fg_rect.right = slider_ctrl_ptr->thumb_rect.left;
            // thumb reach max,channel_fg's right draw to thumb's right.
            if (slider_ctrl_ptr->channel_rect.right
                <= slider_ctrl_ptr->thumb_rect.right)
            {
                channel_fg_rect.right = slider_ctrl_ptr->thumb_rect.right;
            }
        }

        SliderDrawImage(slider_ctrl_ptr, slider_ctrl_ptr->channel_fg_info,
                        &channel_fg_rect);
    }
}

/*****************************************************************************/
//  Description : display slider fg
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void SliderDrawChannelBg(CTRLSLIDER_OBJ_T *slider_ctrl_ptr)
{

    CTRLBASE_OBJ_T *base_ctrl_ptr = PNULL;

    base_ctrl_ptr = (CTRLBASE_OBJ_T *)slider_ctrl_ptr;

    if (PNULL != slider_ctrl_ptr)
    {
        SliderDrawImage(slider_ctrl_ptr, slider_ctrl_ptr->channel_bg_info,
                        &slider_ctrl_ptr->channel_rect);
    }
}

/*****************************************************************************/
//  Description : display slider control
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void SliderDraw(CTRLSLIDER_OBJ_T *slider_ctrl_ptr)
{
    UpdateSliderRect(slider_ctrl_ptr);

    SliderDrawBg(slider_ctrl_ptr);
    SliderDrawChannelBg(slider_ctrl_ptr);
    SliderDrawChannelFg(slider_ctrl_ptr);
    SliderDrawThumb(slider_ctrl_ptr);

    return;
}

/*****************************************************************************/
//  Description : get slider pointer by control id
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL CTRLSLIDER_OBJ_T *GetSliderPtr(MMI_HANDLE_T ctrl_handle)
{
    CTRLBASE_OBJ_T *ctrl_ptr = (CTRLBASE_OBJ_T *)MMK_GetCtrlPtr(ctrl_handle);

    if (PNULL != ctrl_ptr)
    {
        SCI_ASSERT(SliderTypeOf(ctrl_ptr)); /*assert verified*/
    }

    return (CTRLSLIDER_OBJ_T *)ctrl_ptr;
}

/*****************************************************************************/
//  Description : ext type of
//  Global resource dependence :
//  Author:hua.fang
//  Note: judge control type of
/*****************************************************************************/
LOCAL BOOLEAN SliderTypeOf(CTRLBASE_OBJ_T *ctrl_ptr)
{
    return TYPEMNG_IsTypeOf((OBJECT_TYPE_PTR)ctrl_ptr, CTRL_SLIDER_TYPE);
}

/*****************************************************************************/
// 	Description : get control height by width,include content,border etc.
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN SliderCtrlGetHeightByWidth(CTRLBASE_OBJ_T *base_ctrl_ptr, // in:
                                         uint16 width,                  // in:
                                         uint16 *height_ptr // in/out:
)
{
    BOOLEAN result = TRUE;

    if (PNULL != base_ctrl_ptr && PNULL != height_ptr)
    {
        *height_ptr = base_ctrl_ptr->rect.bottom - base_ctrl_ptr->rect.top + 1;
    }

    return (result);
}

/*****************************************************************************/
// 	Description : set rect
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN SliderCtrlSetDispRect(CTRLBASE_OBJ_T *base_ctrl_ptr,
                                    const GUI_RECT_T *rect_ptr,
                                    BOOLEAN is_update)
{
    CTRLSLIDER_OBJ_T *slider_ctrl_ptr = (CTRLSLIDER_OBJ_T *)base_ctrl_ptr;
    CTRLBASE_VTBL_T *parent_vtbl_ptr = PNULL;

    if ((PNULL == base_ctrl_ptr) || (PNULL == rect_ptr))
    {
        return FALSE;
    }

    parent_vtbl_ptr = (CTRLBASE_VTBL_T *)TYPEMNG_GetParentVtbl(
        CTRL_SLIDER_TYPE, (OBJECT_TYPE_PTR)base_ctrl_ptr);

    slider_ctrl_ptr->is_dirty = TRUE;

    if (PNULL != parent_vtbl_ptr)
    {
        parent_vtbl_ptr->SetDisplayRect(base_ctrl_ptr, rect_ptr, is_update);
    }

    return TRUE;
}

/*****************************************************************************/
// 	Description : set rect
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN SliderCtrlSetRect(CTRLBASE_OBJ_T *base_ctrl_ptr,
                                const GUI_RECT_T *rect_ptr)
{
    CTRLSLIDER_OBJ_T *slider_ctrl_ptr = (CTRLSLIDER_OBJ_T *)base_ctrl_ptr;
    CTRLBASE_VTBL_T *parent_vtbl_ptr = PNULL;

    if ((PNULL == base_ctrl_ptr) || (PNULL == rect_ptr))
    {
        return FALSE;
    }

    parent_vtbl_ptr = (CTRLBASE_VTBL_T *)TYPEMNG_GetParentVtbl(
        CTRL_SLIDER_TYPE, (OBJECT_TYPE_PTR)base_ctrl_ptr);

    slider_ctrl_ptr->is_dirty = TRUE;

    if (PNULL != parent_vtbl_ptr)
    {
        parent_vtbl_ptr->SetRect(base_ctrl_ptr, rect_ptr);
    }

    return TRUE;
}

/*****************************************************************************/
//  Description : get line size
//  Global resource dependence :
//  Author: xiaoqing.lu & nan.ji
//  Note:
/*****************************************************************************/
PUBLIC int16 CTRLSLIDER_GetLineSize(MMI_HANDLE_T ctrl_handle)
{
    int16 line_size = 0;
    CTRLSLIDER_OBJ_T *slider_ctrl_ptr = GetSliderPtr(ctrl_handle);

    if (PNULL != slider_ctrl_ptr)
    {
        line_size = slider_ctrl_ptr->line_size;
    }

    return line_size;
}

/*****************************************************************************/
//  Description : get page size
//  Global resource dependence :
//  Author: xiaoqing.lu & nan.ji
//  Note:
/*****************************************************************************/
PUBLIC int16 CTRLSLIDER_GetPageSize(MMI_HANDLE_T ctrl_handle)
{
    int16 page_size = 0;
    CTRLSLIDER_OBJ_T *slider_ctrl_ptr = GetSliderPtr(ctrl_handle);

    if (PNULL != slider_ctrl_ptr)
    {
        page_size = slider_ctrl_ptr->page_size;
    }

    return page_size;
}

/*****************************************************************************/
//  Description : get range max
//  Global resource dependence :
//  Author: xiaoqing.lu & nan.ji
//  Note:
/*****************************************************************************/
PUBLIC int16 CTRLSLIDER_GetRangeMax(MMI_HANDLE_T ctrl_handle)
{
    int16 max = 0;
    CTRLSLIDER_OBJ_T *slider_ctrl_ptr = GetSliderPtr(ctrl_handle);

    if (PNULL != slider_ctrl_ptr)
    {
        max = slider_ctrl_ptr->max;
    }

    return max;
}

/*****************************************************************************/
//  Description : get range min
//  Global resource dependence :
//  Author: xiaoqing.lu & nan.ji
//  Note:
/*****************************************************************************/
PUBLIC int16 CTRLSLIDER_GetRangeMin(MMI_HANDLE_T ctrl_handle)
{
    int16 min = 0;
    CTRLSLIDER_OBJ_T *slider_ctrl_ptr = GetSliderPtr(ctrl_handle);

    if (PNULL != slider_ctrl_ptr)
    {
        min = slider_ctrl_ptr->min;
    }

    return min;
}

/*****************************************************************************/
//  Description : get current position
//  Global resource dependence :
//  Author: xiaoqing.lu & nan.ji
//  Note:
/*****************************************************************************/
PUBLIC int16 CTRLSLIDER_GetSelection(MMI_HANDLE_T ctrl_handle)
{
    int16 cur = 0;
    CTRLSLIDER_OBJ_T *slider_ctrl_ptr = GetSliderPtr(ctrl_handle);

    if (PNULL != slider_ctrl_ptr)
    {
        cur = slider_ctrl_ptr->cur;
    }

    return cur;
}

/*****************************************************************************/
//  Description : set line size
//  Global resource dependence :
//  Author: xiaoqing.lu & nan.ji
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLSLIDER_SetLineSize(MMI_HANDLE_T ctrl_handle, int16 line_size)
{
    BOOLEAN result = FALSE;
    CTRLSLIDER_OBJ_T *slider_ctrl_ptr = GetSliderPtr(ctrl_handle);

    if (PNULL != slider_ctrl_ptr)
    {
        result = TRUE;
        slider_ctrl_ptr->line_size = line_size;
    }

    return result;
}

/*****************************************************************************/
//  Description : set page size
//  Global resource dependence :
//  Author: xiaoqing.lu & nan.ji
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLSLIDER_SetPageSize(MMI_HANDLE_T ctrl_handle, int16 page_size)
{
    BOOLEAN result = FALSE;
    CTRLSLIDER_OBJ_T *slider_ctrl_ptr = GetSliderPtr(ctrl_handle);

    if (PNULL != slider_ctrl_ptr)
    {
        result = TRUE;
        slider_ctrl_ptr->page_size = page_size;
    }

    return result;
}

/*****************************************************************************/
//  Description : set need thumb or not
//  Global resource dependence :
//  Author: xiaoqing.lu & nan.ji
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLSLIDER_SetNeedThumb(MMI_HANDLE_T ctrl_handle,
                                       BOOLEAN is_true)
{
    BOOLEAN result = FALSE;
    CTRLSLIDER_OBJ_T *slider_ctrl_ptr = GetSliderPtr(ctrl_handle);

    if (PNULL != slider_ctrl_ptr)
    {
        result = TRUE;
        slider_ctrl_ptr->thumb_info.is_true = is_true;
    }

    return result;
}

/*****************************************************************************/
//  Description : get if need thumb
//  Global resource dependence :
//  Author: ningbiao.huang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLSLIDER_GetIsNeedThumb(MMI_HANDLE_T ctrl_handle)
{
    CTRLSLIDER_OBJ_T *slider_ctrl_ptr = GetSliderPtr(ctrl_handle);

    if (PNULL != slider_ctrl_ptr)
    {
        return slider_ctrl_ptr->thumb_info.is_true;
    }

    return FALSE;
}

/*****************************************************************************/
//  Description : set need channel foreground or not
//  Global resource dependence :
//  Author: xiaoqing.lu & nan.ji
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLSLIDER_SetNeedChannelFg(MMI_HANDLE_T ctrl_handle,
                                           BOOLEAN is_true)
{
    BOOLEAN result = FALSE;
    CTRLSLIDER_OBJ_T *slider_ctrl_ptr = GetSliderPtr(ctrl_handle);

    if (PNULL != slider_ctrl_ptr)
    {
        result = TRUE;
        slider_ctrl_ptr->channel_fg_info.is_true = is_true;
    }

    return result;
}

/*****************************************************************************/
//  Description : set need foreground or not
//  Global resource dependence :
//  Author: xiaoqing.lu & nan.ji
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLSLIDER_SetNeedBg(MMI_HANDLE_T ctrl_handle, BOOLEAN is_true)
{
    BOOLEAN result = FALSE;
    CTRLSLIDER_OBJ_T *slider_ctrl_ptr = GetSliderPtr(ctrl_handle);

    if (PNULL != slider_ctrl_ptr)
    {
        result = TRUE;
        slider_ctrl_ptr->is_disp_bg = is_true;
    }

    return result;
}

/*****************************************************************************/
//  Description : set max
//  Global resource dependence :
//  Author: xiaoqing.lu & nan.ji
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLSLIDER_SetRangeMax(MMI_HANDLE_T ctrl_handle, int16 max)
{
    BOOLEAN result = FALSE;
    CTRLSLIDER_OBJ_T *slider_ctrl_ptr = GetSliderPtr(ctrl_handle);

    if (PNULL != slider_ctrl_ptr)
    {
        result = TRUE;
        slider_ctrl_ptr->max = max;
    }

    return result;
}

/*****************************************************************************/
//  Description : set min
//  Global resource dependence :
//  Author: xiaoqing.lu & nan.ji
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLSLIDER_SetRangeMin(MMI_HANDLE_T ctrl_handle, int16 min)
{
    BOOLEAN result = FALSE;
    CTRLSLIDER_OBJ_T *slider_ctrl_ptr = GetSliderPtr(ctrl_handle);

    if (PNULL != slider_ctrl_ptr)
    {
        result = TRUE;
        slider_ctrl_ptr->min = min;
    }

    return result;
}

/*****************************************************************************/
//  Description : set current position
//  Global resource dependence :
//  Author: xiaoqing.lu & nan.ji
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLSLIDER_SetSelection(MMI_HANDLE_T ctrl_handle, int16 cur)
{
    BOOLEAN result = FALSE;
    CTRLSLIDER_OBJ_T *slider_ctrl_ptr = GetSliderPtr(ctrl_handle);

    if (PNULL != slider_ctrl_ptr)
    {
        result = TRUE;
        slider_ctrl_ptr->cur = cur;
        slider_ctrl_ptr->is_dirty = TRUE;
        MMK_UpdateScreen();
    }

    return result;
}

/*****************************************************************************/
//  Description : set display style, vertical or horizontal
//  Global resource dependence :
//  Author: xiaoqing.lu & nan.ji
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLSLIDER_SetVerDisp(MMI_HANDLE_T ctrl_handle,
                                     BOOLEAN is_vertical)
{
    BOOLEAN result = FALSE;
    CTRLSLIDER_OBJ_T *slider_ctrl_ptr = GetSliderPtr(ctrl_handle);

    if (PNULL != slider_ctrl_ptr)
    {
        result = TRUE;
        slider_ctrl_ptr->is_vertical = is_vertical;
    }

    return result;
}

/*****************************************************************************/
//  Description : set channel size
//  Global resource dependence :
//  Author: xiaoqing.lu & nan.ji
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLSLIDER_SetChannelSize(MMI_HANDLE_T ctrl_handle,
                                         uint16 channel_width,
                                         uint16 channel_height)
{
    BOOLEAN result = FALSE;
    CTRLSLIDER_OBJ_T *slider_ctrl_ptr = GetSliderPtr(ctrl_handle);

    if (PNULL != slider_ctrl_ptr)
    {
        result = TRUE;
        slider_ctrl_ptr->channel_width = channel_width;
        slider_ctrl_ptr->channel_height = channel_height;
    }

    return result;
}

/*****************************************************************************/
//  Description : Get channel size
//  Global resource dependence :
//  Author: ningbiao.huang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLSLIDER_GetChannelSize(MMI_HANDLE_T ctrl_handle,
                                         uint16 *channel_width,
                                         uint16 *channel_height)
{
    BOOLEAN result = FALSE;
    CTRLSLIDER_OBJ_T *slider_ctrl_ptr = GetSliderPtr(ctrl_handle);

    if (channel_width == NULL || channel_height == NULL)
    {
        return FALSE;
    }

    if (PNULL != slider_ctrl_ptr)
    {
        result = TRUE;
        *channel_width = slider_ctrl_ptr->channel_width;
        *channel_height = slider_ctrl_ptr->channel_height;
    }

    return result;
}

/*****************************************************************************/
//  Description : set thumb size
//  Global resource dependence :
//  Author: xiaoqing.lu & nan.ji
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLSLIDER_SetThumbSize(MMI_HANDLE_T ctrl_handle,
                                       uint16 thumb_width, uint16 thumb_height)
{
    BOOLEAN result = FALSE;
    CTRLSLIDER_OBJ_T *slider_ctrl_ptr = GetSliderPtr(ctrl_handle);

    if (PNULL != slider_ctrl_ptr)
    {
        result = TRUE;
        slider_ctrl_ptr->thumb_width = thumb_width;
        slider_ctrl_ptr->thumb_height = thumb_height;
    }

    return result;
}

/*****************************************************************************/
//  Description : get thumb size
//  Global resource dependence :
//  Author: ningbiao.huang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLSLIDER_GetThumbSize(MMI_HANDLE_T ctrl_handle,
                                       uint16 *thumb_width,
                                       uint16 *thumb_height)
{
    BOOLEAN result = FALSE;
    CTRLSLIDER_OBJ_T *slider_ctrl_ptr = GetSliderPtr(ctrl_handle);
    if (PNULL == thumb_width || PNULL == thumb_height)
    {
        return FALSE;
    }
    if (PNULL != slider_ctrl_ptr)
    {
        result = TRUE;
        *thumb_width = slider_ctrl_ptr->thumb_width;
        *thumb_height = slider_ctrl_ptr->thumb_height;
    }

    return result;
}

/*****************************************************************************/
// 	Description : set rect
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLSLIDER_SliderSetRect(MMI_HANDLE_T ctrl_handle,
                                        const GUI_RECT_T *rect_ptr)
{
    return SliderCtrlSetRect(GetSliderPtr(ctrl_handle), rect_ptr);
}

/*****************************************************************************/
//  Description : display slider control
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLSLIDER_UpdateSlider(MMI_HANDLE_T ctrl_handle)
{
    SliderDraw(GetSliderPtr(ctrl_handle));

    return TRUE;
}

/*****************************************************************************/
// 	Description : create the slider control
//	Global resource dependence :
//  Author: xiaoqing.lu
//	Note: return ctrl handle
/*****************************************************************************/
PUBLIC MMI_HANDLE_T CTRLSLIDER_Create(MMI_HANDLE_T win_handle,
                                      MMI_HANDLE_T parent_ctrl_handle,
                                      MMI_CTRL_ID_T ctrl_id)
{
    MMI_HANDLE_T ctrl_handle = 0;
    CTRLBASE_OBJ_T *base_ctrl_ptr = PNULL;
    MMI_CONTROL_CREATE_T create = {0};
    GUISLIDER_INIT_DATA_T init_data = {0};

    create.ctrl_id = ctrl_id;
    create.guid = SPRD_GUI_SLIDER_ID;
    create.parent_win_handle = win_handle;
    create.parent_ctrl_handle = parent_ctrl_handle;
    create.init_data_ptr = &init_data;

    base_ctrl_ptr = (CTRLBASE_OBJ_T *)MMK_CreateControl(&create);

    if (PNULL != base_ctrl_ptr)
    {
        ctrl_handle = base_ctrl_ptr->handle;
    }

    return ctrl_handle;
}

/*****************************************************************************/
//  Description : slider channel bg color
//  Global resource dependence :
//  Author: ningbiao.huang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLSLIDER_SetChannelBgColor(MMI_HANDLE_T ctrl_handle,
                                            GUI_COLOR_T channel_bg_color)
{
    BOOLEAN result = FALSE;
    CTRLBASE_OBJ_T *base_ctrl_ptr = (CTRLBASE_OBJ_T *)GetSliderPtr(ctrl_handle);
    CTRLSLIDER_OBJ_T *slider_ctrl_ptr = (CTRLSLIDER_OBJ_T *)base_ctrl_ptr;

    if (PNULL != slider_ctrl_ptr)
    {
        slider_ctrl_ptr->channel_bg_info.color = channel_bg_color;
        // disable image id
        slider_ctrl_ptr->channel_bg_info.image_id = 0;
        // draw
        SliderDrawChannelBg(base_ctrl_ptr);
        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : get slider channel bg color
//  Global resource dependence :
//  Author: ningbiao.huang
//  Note:
/*****************************************************************************/
PUBLIC GUI_COLOR_T CTRLSLIDER_GetChannelBgColor(MMI_HANDLE_T ctrl_handle)
{
    CTRLBASE_OBJ_T *base_ctrl_ptr = (CTRLBASE_OBJ_T *)GetSliderPtr(ctrl_handle);
    CTRLSLIDER_OBJ_T *slider_ctrl_ptr = (CTRLSLIDER_OBJ_T *)base_ctrl_ptr;

    if (PNULL != slider_ctrl_ptr)
    {
        return slider_ctrl_ptr->channel_bg_info.color;
    }

    return 0;
}

/*****************************************************************************/
//  Description : slider channel fg color
//  Global resource dependence :
//  Author: ningbiao.huang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLSLIDER_SetChannelFgColor(MMI_HANDLE_T ctrl_handle,
                                            GUI_COLOR_T channel_fg_color)
{
    BOOLEAN result = FALSE;
    CTRLBASE_OBJ_T *base_ctrl_ptr = (CTRLBASE_OBJ_T *)GetSliderPtr(ctrl_handle);
    CTRLSLIDER_OBJ_T *slider_ctrl_ptr = (CTRLSLIDER_OBJ_T *)base_ctrl_ptr;

    if (PNULL != slider_ctrl_ptr)
    {
        slider_ctrl_ptr->channel_fg_info.color = channel_fg_color;
        // disable image id
        slider_ctrl_ptr->channel_fg_info.image_id = 0;
        // draw
        SliderDrawChannelFg(base_ctrl_ptr);
        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : get slider channel fg color
//  Global resource dependence :
//  Author: ningbiao.huang
//  Note:
/*****************************************************************************/
PUBLIC GUI_COLOR_T CTRLSLIDER_GetChannelFgColor(MMI_HANDLE_T ctrl_handle)
{
    CTRLBASE_OBJ_T *base_ctrl_ptr = (CTRLBASE_OBJ_T *)GetSliderPtr(ctrl_handle);
    CTRLSLIDER_OBJ_T *slider_ctrl_ptr = (CTRLSLIDER_OBJ_T *)base_ctrl_ptr;

    if (PNULL != slider_ctrl_ptr)
    {
        return slider_ctrl_ptr->channel_fg_info.color;
    }

    return 0;
}

/*****************************************************************************/
//  Description : slider thumb color
//  Global resource dependence :
//  Author: ningbiao.huang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLSLIDER_SetThumbColor(MMI_HANDLE_T ctrl_handle,
                                        GUI_COLOR_T thumb_color)
{
    BOOLEAN result = FALSE;
    CTRLBASE_OBJ_T *base_ctrl_ptr = (CTRLBASE_OBJ_T *)GetSliderPtr(ctrl_handle);
    CTRLSLIDER_OBJ_T *slider_ctrl_ptr = (CTRLSLIDER_OBJ_T *)base_ctrl_ptr;

    if (PNULL != slider_ctrl_ptr)
    {
        slider_ctrl_ptr->thumb_info.color = thumb_color;
        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : get slider thumb color
//  Global resource dependence :
//  Author: ningbiao.huang
//  Note:
/*****************************************************************************/
PUBLIC GUI_COLOR_T CTRLSLIDER_GetThumbColor(MMI_HANDLE_T ctrl_handle)
{
    CTRLBASE_OBJ_T *base_ctrl_ptr = (CTRLBASE_OBJ_T *)GetSliderPtr(ctrl_handle);
    CTRLSLIDER_OBJ_T *slider_ctrl_ptr = (CTRLSLIDER_OBJ_T *)base_ctrl_ptr;

    if (PNULL != slider_ctrl_ptr)
    {
        return slider_ctrl_ptr->thumb_info.color;
    }

    return 0;
}

/*****************************************************************************/
//  Description : slider VerDisp
//  Global resource dependence :
//  Author: ningbiao.huang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLSLIDER_GetVerDisp(MMI_HANDLE_T ctrl_handle)
{
    CTRLSLIDER_OBJ_T *slider_ctrl_ptr = GetSliderPtr(ctrl_handle);

    if (PNULL != slider_ctrl_ptr)
    {
        return slider_ctrl_ptr->is_vertical;
    }

    return FALSE;
}
