/*****************************************************************************
** File Name:      ctrlscroll.c                                               *
** Author:                                                                   *
** Date:           07/18/2012                                                *
** Copyright:      2004 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 07/2012      lianxiang.zhou & xiyuan.ma              Creat
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

#include "mmitheme_scroll.h"
#include "ctrlscroll.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define SCROLL_HIDE_ALPHA_INIT (255)
#define SCROLL_HIDE_ALPHA_CHANGE_STEP (25)
#define SCROLL_HIDE_START_TICK (1000)
#define SCROLL_HIDE_TICK (100)

#define SCROLL_TP_LONG_TICK (100)

#define SCROLL_AUTO_THIN_PAGE_COUNT (5)
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
//  Description : init scroll class
//  Global resource dependence :
//  Author: lianxiang.zhou & xiyuan.ma
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ScrollCtrlInitVtbl(
    CTRLSCROLL_VTBL_T        *scroll_vtbl_ptr
);

/*****************************************************************************/
//  Description : scroll construct
//  Global resource dependence :
//  Author: lianxiang.zhou & xiyuan.ma
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ScrollCtrlConstruct(
    CTRLBASE_OBJ_T           *base_ctrl_ptr,
    CTRLSCROLL_INIT_PARAM_T   *scroll_param_ptr
);

/*****************************************************************************/
//  Description : scroll destruct
//  Global resource dependence :
//  Author: lianxiang.zhou & xiyuan.ma
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ScrollCtrlDestruct(
    CTRLBASE_OBJ_T        *base_ctrl_ptr
);

/*****************************************************************************/
//  Description : convert init param
//  Global resource dependence :
//  Author: lianxiang.zhou & xiyuan.ma
//  Note:
/*****************************************************************************/
LOCAL void ScrollCtrlPackInitParam(
    GUISCROLL_INIT_DATA_T     *scroll_init_ptr,   //in
    CTRLSCROLL_INIT_PARAM_T   *scroll_param_ptr   //out
);

/*****************************************************************************/
//  Description : handle scroll msg function
//  Global resource dependence :
//  Author: lianxiang.zhou & xiyuan.ma
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E ScrollCtrlHandleMsg(
    CTRLBASE_OBJ_T        *base_ctrl_ptr,   //control pointer
    MMI_MESSAGE_ID_E      msg_id,           //message
    DPARAM                param             //add data
);


/*****************************************************************************/
//  Description : 判定点落在滚动条的那个区域
//  Global resource dependence :
//  Author: lianxiang.zhou & xiyuan.ma
//  Note:
/*****************************************************************************/
LOCAL CTRLSCROLL_REGION_T ScrollGetRegion(
    CTRLSCROLL_OBJ_T const *scroll_ctrl_ptr,
    GUI_POINT_T const *point
);


/*****************************************************************************/
//  Description : 向上滚动
//  Global resource dependence :
//  Author: lianxiang.zhou & xiyuan.ma
//  Note:
/*****************************************************************************/
LOCAL void ScrollMoveUp(
    CTRLSCROLL_OBJ_T     *scroll_ctrl_ptr,
    int32 move_size
);


/*****************************************************************************/
//  Description : 向下滚动
//  Global resource dependence :
//  Author: lianxiang.zhou & xiyuan.ma
//  Note:
/*****************************************************************************/
LOCAL void ScrollMoveDown(
    CTRLSCROLL_OBJ_T     *scroll_ctrl_ptr,
    int32 move_size
);




/*****************************************************************************/
//  Description : 向上滚动一行
//  Global resource dependence :
//  Author: lianxiang.zhou & xiyuan.ma
//  Note:
/*****************************************************************************/
LOCAL void ScrollLineUp(
    CTRLSCROLL_OBJ_T     *scroll_ctrl_ptr
);


/*****************************************************************************/
//  Description : 向下滚动一行
//  Global resource dependence :
//  Author: lianxiang.zhou & xiyuan.ma
//  Note:
/*****************************************************************************/
LOCAL void ScrollLineDown(
    CTRLSCROLL_OBJ_T     *scroll_ctrl_ptr
);


/*****************************************************************************/
//  Description : 向上滚动一页
//  Global resource dependence :
//  Author: lianxiang.zhou & xiyuan.ma
//  Note:
/*****************************************************************************/
LOCAL void ScrollPageUp(
    CTRLSCROLL_OBJ_T     *scroll_ctrl_ptr
);


/*****************************************************************************/
//  Description : 向下滚动一页
//  Global resource dependence :
//  Author: lianxiang.zhou & xiyuan.ma
//  Note:
/*****************************************************************************/
LOCAL void ScrollPageDown(
    CTRLSCROLL_OBJ_T     *scroll_ctrl_ptr
);

/*****************************************************************************/
//  Description : 滑块移动
//  Global resource dependence :
//  Author: lianxiang.zhou & xiyuan.ma
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ScrollMoveThumb(
    CTRLSCROLL_OBJ_T     *scroll_ctrl_ptr,
    GUI_POINT_T const *point
);

/*****************************************************************************/
//  Description : 启动长按定时器
//  Global resource dependence :
//  Author: lianxiang.zhou & xiyuan.ma
//  Note:
/*****************************************************************************/
LOCAL void ScrollStartTimerTpLong(
    CTRLSCROLL_OBJ_T *scroll_ctrl_ptr
);


/*****************************************************************************/
//  Description : 销毁长按定时器
//  Global resource dependence :
//  Author: lianxiang.zhou & xiyuan.ma
//  Note:
/*****************************************************************************/
LOCAL void ScrollStopTimerTpLong(
    CTRLSCROLL_OBJ_T *scroll_ctrl_ptr
);


/*****************************************************************************/
//  Description : 长按定时器callback
//  Global resource dependence :
//  Author: lianxiang.zhou & xiyuan.ma
//  Note:
/*****************************************************************************/
LOCAL void ScrollTimerCallbackTpLong(uint8 timer_id, uint32 param);

/*****************************************************************************/
//  Description : 启动自动隐藏开始定时器
//  Global resource dependence :
//  Author: lianxiang.zhou & xiyuan.ma
//  Note:
/*****************************************************************************/
LOCAL void ScrollStartTimerHideStart(
    CTRLSCROLL_OBJ_T *scroll_ctrl_ptr
);


/*****************************************************************************/
//  Description : 销毁自动隐藏定时器
//  Global resource dependence :
//  Author: lianxiang.zhou & xiyuan.ma
//  Note:
/*****************************************************************************/
LOCAL void ScrollStopTimerHide(
    CTRLSCROLL_OBJ_T *scroll_ctrl_ptr
);


/*****************************************************************************/
//  Description : 自动隐藏开始定时器callback
//  Global resource dependence :
//  Author: lianxiang.zhou & xiyuan.ma
//  Note:
/*****************************************************************************/
LOCAL void ScrollTimerCallbackHideStart(uint8 timer_id, uint32 param);

/*****************************************************************************/
//  Description : 启动自动隐藏的定时器
//  Global resource dependence :
//  Author: lianxiang.zhou & xiyuan.ma
//  Note:
/*****************************************************************************/
LOCAL void ScrollStartTimerHide(
    CTRLSCROLL_OBJ_T *scroll_ctrl_ptr
);


/*****************************************************************************/
//  Description : 自动隐藏定时器callback
//  Global resource dependence :
//  Author: lianxiang.zhou & xiyuan.ma
//  Note:
/*****************************************************************************/
LOCAL void ScrollTimerCallbackHide(uint8 timer_id, uint32 param);



/*****************************************************************************/
//  Description : 根据宽度和显示方向重新设置显示区域
//  Global resource dependence :
//  Author: lianxiang.zhou & xiyuan.ma
//  Note:
/*****************************************************************************/
LOCAL void ScrollResetDisplayRect(
    CTRLSCROLL_OBJ_T     *scroll_ctrl_ptr,
    int32 scroll_width
);

/*****************************************************************************/
//  Description : 计算滑块的高度
//  Global resource dependence :
//  Author: lianxiang.zhou & xiyuan.ma
//  Note:
/*****************************************************************************/
LOCAL int32 ScrollCalcThumbHeight(
    CTRLSCROLL_OBJ_T     *scroll_ctrl_ptr,
    GUI_RECT_T *region_rect_ptr
);


/*****************************************************************************/
//  Description : 计算full滚动条参数
//  Global resource dependence :
//  Author: lianxiang.zhou & xiyuan.ma
//  Note: 设置控制rect
/*****************************************************************************/
LOCAL void ScrollCalcDispParameterFull(
    CTRLSCROLL_OBJ_T     *scroll_ctrl_ptr
);


/*****************************************************************************/
//  Description : 计算full thin滚动条参数
//  Global resource dependence :
//  Author: lianxiang.zhou & xiyuan.ma
//  Note:
/*****************************************************************************/
LOCAL void ScrollCalcDispParameterFullThin(
    CTRLSCROLL_OBJ_T     *scroll_ctrl_ptr
);


/*****************************************************************************/
//  Description : 计算thumb滚动条参数
//  Global resource dependence :
//  Author: lianxiang.zhou & xiyuan.ma
//  Note:
/*****************************************************************************/
LOCAL void ScrollCalcDispParameterThumb(
    CTRLSCROLL_OBJ_T     *scroll_ctrl_ptr
);


/*****************************************************************************/
//  Description : 计算thumb thin滚动条参数
//  Global resource dependence :
//  Author: lianxiang.zhou & xiyuan.ma
//  Note:
/*****************************************************************************/
LOCAL void ScrollCalcDispParameterThumbThin(
    CTRLSCROLL_OBJ_T     *scroll_ctrl_ptr
);



/*****************************************************************************/
//  Description : 计算滚动条的各种显示区域
//  Global resource dependence :
//  Author: lianxiang.zhou & xiyuan.ma
//  Note: 设置普通滚动条的控制rect
/*****************************************************************************/
LOCAL void ScrollCalcDispParameter(
    CTRLSCROLL_OBJ_T     *scroll_ctrl_ptr
);


/*****************************************************************************/
//  Description : 计算滑块的位置
//  Global resource dependence :
//  Author: lianxiang.zhou & xiyuan.ma
//  Note: 动态设置控制rect
/*****************************************************************************/
LOCAL void ScrollCalcThumbPos(
    CTRLSCROLL_OBJ_T     *scroll_ctrl_ptr,
    GUI_RECT_T *region_rect_ptr
);

/*****************************************************************************/
//  Description : 计算滑块的位置
//  Global resource dependence :
//  Author: lianxiang.zhou & xiyuan.ma
//  Note: 动态设置控制rect
/*****************************************************************************/
LOCAL void ScrollCalcThumbPositionFull(
    CTRLSCROLL_OBJ_T     *scroll_ctrl_ptr
);


/*****************************************************************************/
//  Description : 计算滑块的位置
//  Global resource dependence :
//  Author: lianxiang.zhou & xiyuan.ma
//  Note: 动态设置控制rect
/*****************************************************************************/
LOCAL void ScrollCalcThumbPositionThumb(
    CTRLSCROLL_OBJ_T     *scroll_ctrl_ptr
);


/*****************************************************************************/
//  Description : 计算滑块的位置
//  Global resource dependence :
//  Author: lianxiang.zhou & xiyuan.ma
//  Note: 动态设置控制rect
/*****************************************************************************/
LOCAL void ScrollCalcThumbPositionThumbThin(
    CTRLSCROLL_OBJ_T     *scroll_ctrl_ptr
);

/*****************************************************************************/
//  Description : 计算滑块的位置
//  Global resource dependence :
//  Author: lianxiang.zhou & xiyuan.ma
//  Note: 动态设置控制rect
/*****************************************************************************/
LOCAL void ScrollCalcThumbPosition(
    CTRLSCROLL_OBJ_T     *scroll_ctrl_ptr
);


/*****************************************************************************/
//  Description : display scroll control
//  Global resource dependence :
//  Author: lianxiang.zhou & xiyuan.ma
//  Note:
/*****************************************************************************/
LOCAL void ScrollDrawFull(
    CTRLSCROLL_OBJ_T     *scroll_ctrl_ptr
);


/*****************************************************************************/
//  Description : display scroll control
//  Global resource dependence :
//  Author: lianxiang.zhou & xiyuan.ma
//  Note:
/*****************************************************************************/
LOCAL void ScrollDrawFullThin(
    CTRLSCROLL_OBJ_T     *scroll_ctrl_ptr
);


/*****************************************************************************/
//  Description : display scroll control
//  Global resource dependence :
//  Author: lianxiang.zhou & xiyuan.ma
//  Note:
/*****************************************************************************/
LOCAL void ScrollDrawThumb(
    CTRLSCROLL_OBJ_T     *scroll_ctrl_ptr
);


/*****************************************************************************/
//  Description : 设置自动隐藏时的显示层
//  Global resource dependence :
//  Author: lianxiang.zhou & xiyuan.ma
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ScrollHideDispLayerSetting(
    CTRLSCROLL_OBJ_T     *scroll_ctrl_ptr
);


/*****************************************************************************/
//  Description : 释放自动隐藏时的显示层
//  Global resource dependence :
//  Author: lianxiang.zhou & xiyuan.ma
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ScrollHideDispLayerRelease(
    CTRLSCROLL_OBJ_T     *scroll_ctrl_ptr
);




/*****************************************************************************/
//  Description : display scroll control
//  Global resource dependence :
//  Author: lianxiang.zhou & xiyuan.ma
//  Note:
/*****************************************************************************/
LOCAL void ScrollDraw(
    CTRLSCROLL_OBJ_T     *scroll_ctrl_ptr
);

/*****************************************************************************/
//  Description : get scroll pointer by control id
//  Global resource dependence :
//  Author: lianxiang.zhou & xiyuan.ma
//  Note:
/*****************************************************************************/
LOCAL CTRLSCROLL_OBJ_T *GetScrollPtr(
    MMI_HANDLE_T     ctrl_handle
);

/*****************************************************************************/
//  Description : ext type of
//  Global resource dependence :
//  Author:lianxiang.zhou & xiyuan.ma
//  Note: judge control type of
/*****************************************************************************/
LOCAL BOOLEAN ScrollTypeOf(
    CTRLBASE_OBJ_T     *ctrl_ptr
);

/*****************************************************************************/
//  Description : set control whether display progress
//  Global resource dependence :
//  Author: lianxiang.zhou & xiyuan.ma
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ScrollCtrlSetScrollbarEnabled(
    CTRLBASE_OBJ_T       *base_ctrl_ptr,
    BOOLEAN              is_display
);

/*****************************************************************************/
//  Description : set control is circular handle up/down key
//  Global resource dependence :
//  Author: lianxiang.zhou & xiyuan.ma
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ScrollCtrlSetUpDownLoop(
    CTRLBASE_OBJ_T       *base_ctrl_ptr,
    BOOLEAN              is_circular
);

/*****************************************************************************/
//  Description : get control height by width,include content,border etc.
//  Global resource dependence :
//  Author: lianxiang.zhou & xiyuan.ma
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ScrollCtrlGetHeightByWidth(
    CTRLBASE_OBJ_T      *base_ctrl_ptr,  //in:
    uint16              width,          //in:
    uint16              *height_ptr     //in/out:
);

/*****************************************************************************/
//  Description : set ctrl rect
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ScrollCtrlSetRect(
    CTRLBASE_OBJ_T      *base_ctrl_ptr, //in:
    const GUI_RECT_T    *rect_ptr
);

/*****************************************************************************/
//  Description : set ctrl display rect
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ScrollCtrlSetDisplayRect(
    CTRLBASE_OBJ_T      *base_ctrl_ptr, //in:
    const GUI_RECT_T    *rect_ptr,      //in:
    BOOLEAN             is_update       //in:
);

/*****************************************************************************/
//  Description : set control select rect
//  Global resource dependence :
//  Author: lianxiang.zhou & xiyuan.ma
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ScrollCtrlGetSelectRect(
    CTRLBASE_OBJ_T         *base_ctrl_ptr,  //in:
    GUI_RECT_T             *rect_ptr       //in/out:
);

/*****************************************************************************/
//  Description : set control select index,is the first or last line
//  Global resource dependence :
//  Author: lianxiang.zhou & xiyuan.ma
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ScrollCtrlGetSelectIndex(
    CTRLBASE_OBJ_T        *base_ctrl_ptr,  //in:
    BOOLEAN               *is_first_ptr,  //in/out:
    BOOLEAN               *is_last_ptr    //in/out:
);

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : get scroll type
//  Global resource dependence :
//  Author: lianxiang.zhou & xiyuan.ma
//  Note:
/*****************************************************************************/
PUBLIC TYPE_TAG_T SCROLL_GetType()
{
    static TYPE_TAG_T    obj_type = 0;

    if (0 == obj_type)
    {
        obj_type = TYPEMNG_Register(
                       CTRL_BASE_TYPE,                                  // parent class type
                       "scroll",                                         // control name
                       sizeof(CTRLSCROLL_OBJ_T),                         // size of control object struct
                       (OBJECT_CONSTRUCTOR_FUNC) ScrollCtrlConstruct,    // control constructor function
                       (OBJECT_DESTRUCTOR_FUNC) ScrollCtrlDestruct,      // control destructor function
                       sizeof(CTRLSCROLL_INIT_PARAM_T),                  // size of init param struct
                       (PARAM_PACKING_FUNC) ScrollCtrlPackInitParam,     // convert init param
                       sizeof(CTRLSCROLL_VTBL_T),                        // size of virtual table
                       (VTBL_INIT_FUNC) ScrollCtrlInitVtbl);             // init virtual table function
    }

    return obj_type;
}

/*****************************************************************************/
//  Description : init scroll class
//  Global resource dependence :
//  Author: lianxiang.zhou & xiyuan.ma
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ScrollCtrlInitVtbl(
    CTRLSCROLL_VTBL_T        *scroll_vtbl_ptr
)
{
    CTRLBASE_VTBL_T  *base_ctrl_vtbl_ptr = (CTRLBASE_VTBL_T *) scroll_vtbl_ptr;

    if (PNULL == base_ctrl_vtbl_ptr)
    {
        return FALSE;
    }

    base_ctrl_vtbl_ptr->HandleEvent = ScrollCtrlHandleMsg;

    // 以下接口用于实现内嵌form
    base_ctrl_vtbl_ptr->SetRect = ScrollCtrlSetRect;
    base_ctrl_vtbl_ptr->GetHeightByWidth = ScrollCtrlGetHeightByWidth;
    //base_ctrl_vtbl_ptr->SetDisplayRect = ScrollCtrlSetDisplayRect;
    base_ctrl_vtbl_ptr->SetScrollbarEnabled = ScrollCtrlSetScrollbarEnabled; //有些控件不必实现
    base_ctrl_vtbl_ptr->SetUpDownLoop = ScrollCtrlSetUpDownLoop; //有些控件不必实现
    base_ctrl_vtbl_ptr->GetSelectRect = ScrollCtrlGetSelectRect; //有些控件不必实现
    base_ctrl_vtbl_ptr->GetSelectIndex = ScrollCtrlGetSelectIndex; //有些控件不必实现
    base_ctrl_vtbl_ptr->GetDisplayRect = BaseCtrlGetDisplayRectNotImp;

    return TRUE;
}

/*****************************************************************************/
//  Description : convert init param
//  Global resource dependence :
//  Author: lianxiang.zhou & xiyuan.ma
//  Note:
// 该方法用于将控件的对外初始化参数结构转换为内部的初始化参数结构
// 由于控件有继承关系，控件初始化时又带有参数，所以内部的初始化参数也必须有继承关系
/*****************************************************************************/
LOCAL void ScrollCtrlPackInitParam(
    GUISCROLL_INIT_DATA_T     *scroll_init_ptr,   //in
    CTRLSCROLL_INIT_PARAM_T   *scroll_param_ptr   //out
)
{
    CTRLBASE_INIT_PARAM_T   *base_ctrl_param_ptr = (CTRLBASE_INIT_PARAM_T *) scroll_param_ptr;

    if (PNULL == scroll_init_ptr || PNULL == scroll_param_ptr)
    {
        return;
    }

    base_ctrl_param_ptr->both_rect = scroll_init_ptr->both_rect;
}

/*****************************************************************************/
//  Description : scroll construct
//  Global resource dependence :
//  Author: lianxiang.zhou & xiyuan.ma
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ScrollCtrlConstruct(
    CTRLBASE_OBJ_T           *base_ctrl_ptr,
    CTRLSCROLL_INIT_PARAM_T   *scroll_param_ptr
)
{
    BOOLEAN             result = TRUE;
    CTRLSCROLL_OBJ_T     *scroll_ctrl_ptr = (CTRLSCROLL_OBJ_T *) base_ctrl_ptr;

    if (PNULL == base_ctrl_ptr || PNULL == scroll_param_ptr)
    {
        return FALSE;
    }

    // TODO
    scroll_ctrl_ptr->thumb_min_size = MMITHEME_ScrollGetThumbMinSize();

    scroll_ctrl_ptr->is_dirty = TRUE;
    scroll_ctrl_ptr->is_moved = TRUE;

    scroll_ctrl_ptr->line_size = 1; //set "1" as default

    scroll_ctrl_ptr->is_thin = MMITHEME_ScrollIsThin();
    scroll_ctrl_ptr->is_thumb_only = MMITHEME_ScrollIsThumbOnly();

    if (scroll_ctrl_ptr->is_thumb_only)
    {
        scroll_ctrl_ptr->is_auto_hide = TRUE;
        scroll_ctrl_ptr->hide_layer.block_id = UILAYER_NULL_HANDLE;

        scroll_ctrl_ptr->alpha_change_step = SCROLL_HIDE_ALPHA_CHANGE_STEP;
        scroll_ctrl_ptr->hide_start_tick = SCROLL_HIDE_START_TICK;
        scroll_ctrl_ptr->hide_tick = SCROLL_HIDE_TICK;

        scroll_ctrl_ptr->auto_thin_page_count = SCROLL_AUTO_THIN_PAGE_COUNT;
    }
    else
    {
        scroll_ctrl_ptr->tp_long_tick = SCROLL_TP_LONG_TICK;
    }


    return (result);
}

/*****************************************************************************/
//  Description : scroll destruct
//  Global resource dependence :
//  Author: lianxiang.zhou & xiyuan.ma
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ScrollCtrlDestruct(
    CTRLBASE_OBJ_T    *base_ctrl_ptr
)
{
    BOOLEAN             result = TRUE;
    CTRLSCROLL_OBJ_T     *scroll_ctrl_ptr = (CTRLSCROLL_OBJ_T *) base_ctrl_ptr;

    if (PNULL == scroll_ctrl_ptr)
    {
        return FALSE;
    }

    // TODO
    // ...
    ScrollStopTimerTpLong(scroll_ctrl_ptr);

    if (scroll_ctrl_ptr->is_auto_hide)
    {
        ScrollStopTimerHide(scroll_ctrl_ptr);
        ScrollHideDispLayerRelease(scroll_ctrl_ptr);
    }


    return (result);
}

/*****************************************************************************/
//  Description : handle scroll msg function
//  Global resource dependence :
//  Author: lianxiang.zhou & xiyuan.ma
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E ScrollCtrlHandleMsg(
    CTRLBASE_OBJ_T        *base_ctrl_ptr,   //control pointer
    MMI_MESSAGE_ID_E      msg_id,           //message
    DPARAM                param             //add data
)
{
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    CTRLSCROLL_OBJ_T     *scroll_ctrl_ptr = (CTRLSCROLL_OBJ_T *) base_ctrl_ptr;
    CTRLBASE_VTBL_T     *parent_vtbl_ptr = (CTRLBASE_VTBL_T *)TYPEMNG_GetParentVtbl(CTRL_SCROLL_TYPE, (OBJECT_TYPE_PTR)base_ctrl_ptr);
#ifdef TOUCH_PANEL_SUPPORT
    GUI_POINT_T point = {0};
    CTRLSCROLL_REGION_T region = CTRLSCROLL_REGION_NONE;
#endif

    if (PNULL == scroll_ctrl_ptr)
    {
        return MMI_RESULT_FALSE;
    }

    switch (msg_id)
    {
        case MSG_CTL_PAINT:
            ScrollDraw(scroll_ctrl_ptr);
            break;

        case MSG_CTL_GET_ACTIVE:
            break;

        case MSG_CTL_LOSE_ACTIVE:
        case MSG_CTL_LOSE_FOCUS:
        case MSG_CTL_CLOSE:
            break;

        case MSG_NOTIFY_LOSE_ACTIVE:
            break;

        case MSG_TIMER:
            break;

        case MSG_APP_OK:
            break;

        case MSG_APP_WEB:
            break;

        case MSG_APP_CANCEL:
            break;

#ifdef TOUCH_PANEL_SUPPORT

        case MSG_TP_PRESS_DOWN:
            point.x = MMK_GET_TP_X(param);
            point.y = MMK_GET_TP_Y(param);

            region = ScrollGetRegion(scroll_ctrl_ptr, &point);

            scroll_ctrl_ptr->tp_down_region = region;
            scroll_ctrl_ptr->is_tp_move = FALSE;
            scroll_ctrl_ptr->is_tp_long = FALSE;

            scroll_ctrl_ptr->on_thumb_y = point.y - scroll_ctrl_ptr->thumb_rect.top;
            scroll_ctrl_ptr->last_tp_point = point;

            switch (region)
            {
                case CTRLSCROLL_REGION_TOP:
                    ScrollLineUp(scroll_ctrl_ptr);
                    break;

                case CTRLSCROLL_REGION_BOTTOM:
                    ScrollLineDown(scroll_ctrl_ptr);
                    break;

                case CTRLSCROLL_REGION_UP:
                    ScrollPageUp(scroll_ctrl_ptr);
                    break;

                case CTRLSCROLL_REGION_DOWN:
                    ScrollPageDown(scroll_ctrl_ptr);
                    break;

                case CTRLSCROLL_REGION_THUMB:
                    //do nothing
                    break;

                default:
                    break;
            }

            break;

        case MSG_TP_PRESS_MOVE:
            point.x = MMK_GET_TP_X(param);
            point.y = MMK_GET_TP_Y(param);
            region = ScrollGetRegion(scroll_ctrl_ptr, &point);

            scroll_ctrl_ptr->is_tp_move = TRUE;

            scroll_ctrl_ptr->last_tp_point = point;

            if (scroll_ctrl_ptr->is_tp_long)
            {
                scroll_ctrl_ptr->tp_long_region = region;
                break;
            }

            switch (scroll_ctrl_ptr->tp_down_region)
            {
                case CTRLSCROLL_REGION_TOP:
                case CTRLSCROLL_REGION_BOTTOM:
                case CTRLSCROLL_REGION_UP:
                case CTRLSCROLL_REGION_DOWN:
                    //do nothing
                    break;

                case CTRLSCROLL_REGION_THUMB:
                    ScrollMoveThumb(scroll_ctrl_ptr, &point);
                    break;

                default:
                    break;
            }

            break;

        case MSG_TP_PRESS_LONG:
            scroll_ctrl_ptr->is_tp_long = TRUE;
            scroll_ctrl_ptr->tp_long_region = ScrollGetRegion(
                                                  scroll_ctrl_ptr,
                                                  &(scroll_ctrl_ptr->last_tp_point));
            ScrollStartTimerTpLong(scroll_ctrl_ptr);
            break;

        case MSG_TP_PRESS_UP:
            point.x = MMK_GET_TP_X(param);
            point.y = MMK_GET_TP_Y(param);
            region = ScrollGetRegion(scroll_ctrl_ptr, &point);

            ScrollStopTimerTpLong(scroll_ctrl_ptr);

            if (CTRLSCROLL_REGION_THUMB == region
                && !(scroll_ctrl_ptr->is_tp_move)
                && !(scroll_ctrl_ptr->is_tp_long))
            {
                //send a click msg to parent
                CTRLMSG_PostNotify(base_ctrl_ptr->handle, MSG_NOTIFY_SCROLL_THUMB_CLICK);
            }

            scroll_ctrl_ptr->tp_down_region = CTRLSCROLL_REGION_NONE;
            scroll_ctrl_ptr->tp_long_region = CTRLSCROLL_REGION_NONE;
            scroll_ctrl_ptr->is_tp_move = FALSE;
            scroll_ctrl_ptr->is_tp_long = FALSE;

			ScrollDraw(scroll_ctrl_ptr);//xiyuan

            break;
#endif

        default:
            result = parent_vtbl_ptr->HandleEvent(base_ctrl_ptr, msg_id, param);
            break;
    }

    return (result);
}


/*****************************************************************************/
//  Description : 判定点落在滚动条的那个区域
//  Global resource dependence :
//  Author: lianxiang.zhou & xiyuan.ma
//  Note:
/*****************************************************************************/
LOCAL CTRLSCROLL_REGION_T ScrollGetRegion(
    CTRLSCROLL_OBJ_T const *scroll_ctrl_ptr,
    GUI_POINT_T const *point
)
{
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T *) scroll_ctrl_ptr;
    CTRLSCROLL_REGION_T region = CTRLSCROLL_REGION_NONE;

    if (PNULL == scroll_ctrl_ptr || PNULL == point)
    {
        return CTRLSCROLL_REGION_NONE;
    }

    if (point->x < base_ctrl_ptr->display_rect.left
        || point->x > base_ctrl_ptr->display_rect.right)
    {
        return CTRLSCROLL_REGION_NONE;
    }

    switch (scroll_ctrl_ptr->style)
    {
        case CTRLSCROLL_STYLE_FULL:

            if (scroll_ctrl_ptr->top_arrow_rect.top <= point->y
                && point->y <= scroll_ctrl_ptr->top_arrow_rect.bottom)
            {
                region = CTRLSCROLL_REGION_TOP;
            }
            else if (scroll_ctrl_ptr->region_rect.top <= point->y
                     && point->y < scroll_ctrl_ptr->thumb_rect.top)
            {
                region = CTRLSCROLL_REGION_UP;
            }
            else if (scroll_ctrl_ptr->thumb_rect.top <= point->y
                     && point->y <= scroll_ctrl_ptr->thumb_rect.bottom)
            {
                region = CTRLSCROLL_REGION_THUMB;
            }
            else if (scroll_ctrl_ptr->thumb_rect.bottom < point->y
                     && point->y <= scroll_ctrl_ptr->region_rect.bottom)
            {
                region = CTRLSCROLL_REGION_DOWN;
            }
            else if (scroll_ctrl_ptr->bottom_arrow_rect.top <= point->y
                     && point->y <= scroll_ctrl_ptr->bottom_arrow_rect.bottom)
            {
                region = CTRLSCROLL_REGION_BOTTOM;
            }

            break;

        case CTRLSCROLL_STYLE_THUMB:

            if (scroll_ctrl_ptr->thumb_rect.top <= point->y
                && point->y <= scroll_ctrl_ptr->thumb_rect.bottom)
            {
                region = CTRLSCROLL_REGION_THUMB;
            }

            break;

        default:
            //no region for "thin" style
            break;
    }

    return region;
}


/*****************************************************************************/
//  Description : 向上滚动
//  Global resource dependence :
//  Author: lianxiang.zhou & xiyuan.ma
//  Note:
/*****************************************************************************/
LOCAL void ScrollMoveUp(
    CTRLSCROLL_OBJ_T     *scroll_ctrl_ptr,
    int32 move_size
)
{
    if (PNULL == scroll_ctrl_ptr)
    {
        return;
    }

    if ((int32)scroll_ctrl_ptr->top_of_current_page < move_size)
    {
        scroll_ctrl_ptr->top_of_current_page = 0;
    }
    else
    {
        scroll_ctrl_ptr->top_of_current_page -= move_size;      /*lint !e737*/
    }

    scroll_ctrl_ptr->is_moved = TRUE;

    //draw...
    ScrollDraw(scroll_ctrl_ptr);

    return;
}


/*****************************************************************************/
//  Description : 向下滚动
//  Global resource dependence :
//  Author: lianxiang.zhou & xiyuan.ma
//  Note:
/*****************************************************************************/
LOCAL void ScrollMoveDown(
    CTRLSCROLL_OBJ_T     *scroll_ctrl_ptr,
    int32 move_size
)
{
    uint32 top_of_last_page = 0;

    if (PNULL == scroll_ctrl_ptr)
    {
        return;
    }

    top_of_last_page = scroll_ctrl_ptr->total_size - scroll_ctrl_ptr->page_size;

    if (scroll_ctrl_ptr->top_of_current_page + move_size > top_of_last_page)    /*lint !e737*/
    {
        scroll_ctrl_ptr->top_of_current_page = top_of_last_page;
    }
    else
    {
        scroll_ctrl_ptr->top_of_current_page += move_size;  /*lint !e737*/
    }

    scroll_ctrl_ptr->is_moved = TRUE;

    //draw...
    ScrollDraw(scroll_ctrl_ptr);

    return;
}




/*****************************************************************************/
//  Description : 向上滚动一行
//  Global resource dependence :
//  Author: lianxiang.zhou & xiyuan.ma
//  Note:
/*****************************************************************************/
LOCAL void ScrollLineUp(
    CTRLSCROLL_OBJ_T     *scroll_ctrl_ptr
)
{
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T *) scroll_ctrl_ptr;

    if (PNULL == scroll_ctrl_ptr)
    {
        return;
    }

    ScrollMoveUp(scroll_ctrl_ptr, scroll_ctrl_ptr->line_size);

    //send msg to parent
    CTRLMSG_PostNotify(base_ctrl_ptr->handle, MSG_NOTIFY_SCROLL_LINE_UP);


    return;
}


/*****************************************************************************/
//  Description : 向下滚动一行
//  Global resource dependence :
//  Author: lianxiang.zhou & xiyuan.ma
//  Note:
/*****************************************************************************/
LOCAL void ScrollLineDown(
    CTRLSCROLL_OBJ_T     *scroll_ctrl_ptr
)
{
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T *) scroll_ctrl_ptr;

    if (PNULL == scroll_ctrl_ptr)
    {
        return;
    }

    ScrollMoveDown(scroll_ctrl_ptr, scroll_ctrl_ptr->line_size);

    //send msg to parent
    CTRLMSG_PostNotify(base_ctrl_ptr->handle, MSG_NOTIFY_SCROLL_LINE_DOWN);


    return;
}


/*****************************************************************************/
//  Description : 向上滚动一页
//  Global resource dependence :
//  Author: lianxiang.zhou & xiyuan.ma
//  Note:
/*****************************************************************************/
LOCAL void ScrollPageUp(
    CTRLSCROLL_OBJ_T     *scroll_ctrl_ptr
)
{
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T *) scroll_ctrl_ptr;

    if (PNULL == scroll_ctrl_ptr)
    {
        return;
    }

    ScrollMoveUp(scroll_ctrl_ptr, scroll_ctrl_ptr->page_size);

    //send msg to parent
    CTRLMSG_PostNotify(base_ctrl_ptr->handle, MSG_NOTIFY_SCROLL_PAGE_UP);


    return;
}


/*****************************************************************************/
//  Description : 向下滚动一页
//  Global resource dependence :
//  Author: lianxiang.zhou & xiyuan.ma
//  Note:
/*****************************************************************************/
LOCAL void ScrollPageDown(
    CTRLSCROLL_OBJ_T     *scroll_ctrl_ptr
)
{
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T *) scroll_ctrl_ptr;

    if (PNULL == scroll_ctrl_ptr)
    {
        return;
    }

    ScrollMoveDown(scroll_ctrl_ptr, scroll_ctrl_ptr->page_size);

    //send msg to parent
    CTRLMSG_PostNotify(base_ctrl_ptr->handle, MSG_NOTIFY_SCROLL_PAGE_DOWN);


    return;
}

/*****************************************************************************/
//  Description : 滑块移动
//  Global resource dependence :
//  Author: lianxiang.zhou & xiyuan.ma
//  Note: move *TO* point
/*****************************************************************************/
LOCAL BOOLEAN ScrollMoveThumb(
    CTRLSCROLL_OBJ_T     *scroll_ctrl_ptr,
    GUI_POINT_T const *point)
{
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T *) scroll_ctrl_ptr;
    CTRLSCROLL_REGION_T region = CTRLSCROLL_REGION_NONE;
    GUI_RECT_T region_rect = {0};
    int16 new_thumb_top = 0;
    int16 region_height = 0;
    uint32 new_top = 0;
    int32 scroll_size = 0;
    BOOLEAN is_real_moved = FALSE;

    if (PNULL == scroll_ctrl_ptr || PNULL == point)
    {
        return FALSE;
    }

    region = ScrollGetRegion(scroll_ctrl_ptr, &(scroll_ctrl_ptr->last_tp_point));

    if (CTRLSCROLL_REGION_NONE == region)
    {
        return FALSE;
    }

    switch (scroll_ctrl_ptr->style)
    {
        case CTRLSCROLL_STYLE_FULL:
            region_rect = scroll_ctrl_ptr->region_rect;
            break;

        case CTRLSCROLL_STYLE_THUMB:
            region_rect = base_ctrl_ptr->display_rect;
            break;

        default:
            //no region for "thin" style
            break;
    }

    new_thumb_top = point->y - scroll_ctrl_ptr->on_thumb_y;

    if (new_thumb_top < region_rect.top)
    {
        new_thumb_top = region_rect.top;
    }
    else if (new_thumb_top > region_rect.bottom - scroll_ctrl_ptr->thumb_height + 1)    /*lint !e737 !e574*/
    {
        new_thumb_top = region_rect.bottom - scroll_ctrl_ptr->thumb_height + 1; /*lint !e737*/
    }

    region_height = region_rect.bottom - region_rect.top + 1;

    new_top = (new_thumb_top - region_rect.top)*scroll_ctrl_ptr->total_size/region_height ; /*lint !e737 !e573*/

    scroll_size = new_top - scroll_ctrl_ptr->top_of_current_page;

    if (scroll_size < 0) scroll_size *= (-1);

    if (scroll_size >= (int32)scroll_ctrl_ptr->line_size)
    {
        is_real_moved = TRUE;

        scroll_ctrl_ptr->top_of_current_page = new_top;
        scroll_ctrl_ptr->is_moved = TRUE;

        //draw...
        ScrollDraw(scroll_ctrl_ptr);

        //send msg to parent
        CTRLMSG_PostNotifyEx(base_ctrl_ptr->handle, MSG_NOTIFY_SCROLL_MOVE_TO, &new_top, sizeof(new_top));
    }
    

    return is_real_moved;
}

/*****************************************************************************/
//  Description : 启动长按定时器
//  Global resource dependence :
//  Author: lianxiang.zhou & xiyuan.ma
//  Note:
/*****************************************************************************/
LOCAL void ScrollStartTimerTpLong(
    CTRLSCROLL_OBJ_T *scroll_ctrl_ptr
)
{
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T *) scroll_ctrl_ptr;

    if (PNULL == scroll_ctrl_ptr)
    {
        return;
    }

    if (CTRLSCROLL_STYLE_FULL != scroll_ctrl_ptr->style)
    {
        return;
    }

    if (CTRLSCROLL_REGION_THUMB == scroll_ctrl_ptr->tp_long_region)
    {
        return;
    }

    scroll_ctrl_ptr->tp_long_timer = MMK_CreateTimerCallback(
                                         scroll_ctrl_ptr->tp_long_tick,
                                         ScrollTimerCallbackTpLong,
                                         (uint32)(base_ctrl_ptr->handle),
                                         TRUE);

    return;
}


/*****************************************************************************/
//  Description : 销毁长按定时器
//  Global resource dependence :
//  Author: lianxiang.zhou & xiyuan.ma
//  Note:
/*****************************************************************************/
LOCAL void ScrollStopTimerTpLong(
    CTRLSCROLL_OBJ_T *scroll_ctrl_ptr
)
{
    if (PNULL == scroll_ctrl_ptr)
    {
        return;
    }

    if (0 != scroll_ctrl_ptr->tp_long_timer)
    {
        MMK_StopTimer(scroll_ctrl_ptr->tp_long_timer);
        scroll_ctrl_ptr->tp_long_timer = 0;
    }

    return;
}


/*****************************************************************************/
//  Description : 长按定时器callback
//  Global resource dependence :
//  Author: lianxiang.zhou & xiyuan.ma
//  Note:
/*****************************************************************************/
LOCAL void ScrollTimerCallbackTpLong(uint8 timer_id, uint32 param)
{
    CTRLSCROLL_OBJ_T *scroll_ctrl_ptr = GetScrollPtr((MMI_HANDLE_T)param);

    if (PNULL == scroll_ctrl_ptr)
    {
        return;
    }

    if (timer_id != scroll_ctrl_ptr->tp_long_timer)
    {
        //assert?
        return;
    }

    if (CTRLSCROLL_STYLE_FULL != scroll_ctrl_ptr->style)
    {
        return;
    }

    scroll_ctrl_ptr->tp_long_region = ScrollGetRegion(
                                          scroll_ctrl_ptr,
                                          &(scroll_ctrl_ptr->last_tp_point));

    if (scroll_ctrl_ptr->tp_down_region != scroll_ctrl_ptr->tp_long_region)
    {
        return;
    }

    switch (scroll_ctrl_ptr->tp_long_region)
    {
        case CTRLSCROLL_REGION_TOP:
            ScrollLineUp(scroll_ctrl_ptr);
            break;

        case CTRLSCROLL_REGION_BOTTOM:
            ScrollLineDown(scroll_ctrl_ptr);
            break;

        case CTRLSCROLL_REGION_UP:
            ScrollPageUp(scroll_ctrl_ptr);
            break;

        case CTRLSCROLL_REGION_DOWN:
            ScrollPageDown(scroll_ctrl_ptr);
            break;

        default:
            break;

    }


    return;
}

/*****************************************************************************/
//  Description : 启动自动隐藏开始定时器
//  Global resource dependence :
//  Author: lianxiang.zhou & xiyuan.ma
//  Note:
/*****************************************************************************/
LOCAL void ScrollStartTimerHideStart(
    CTRLSCROLL_OBJ_T *scroll_ctrl_ptr
)
{
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T *) scroll_ctrl_ptr;

    if (PNULL == scroll_ctrl_ptr)
    {
        return;
    }

    if (!scroll_ctrl_ptr->is_auto_hide)
    {
        return;
    }

    scroll_ctrl_ptr->hide_timer = MMK_CreateTimerCallback(
                                      scroll_ctrl_ptr->hide_start_tick,
                                      ScrollTimerCallbackHideStart,
                                      (uint32)(base_ctrl_ptr->handle),
                                      FALSE);

    return;
}


/*****************************************************************************/
//  Description : 销毁自动隐藏定时器
//  Global resource dependence :
//  Author: lianxiang.zhou & xiyuan.ma
//  Note:
/*****************************************************************************/
LOCAL void ScrollStopTimerHide(
    CTRLSCROLL_OBJ_T *scroll_ctrl_ptr
)
{
    if (PNULL == scroll_ctrl_ptr)
    {
        return;
    }

    if (!scroll_ctrl_ptr->is_auto_hide)
    {
        return;
    }

    if (0 != scroll_ctrl_ptr->hide_timer)
    {
        MMK_StopTimer(scroll_ctrl_ptr->hide_timer);
        scroll_ctrl_ptr->hide_timer = 0;
    }

    return;
}


/*****************************************************************************/
//  Description : 自动隐藏开始定时器callback
//  Global resource dependence :
//  Author: lianxiang.zhou & xiyuan.ma
//  Note:
/*****************************************************************************/
LOCAL void ScrollTimerCallbackHideStart(uint8 timer_id, uint32 param)
{
    CTRLSCROLL_OBJ_T *scroll_ctrl_ptr = GetScrollPtr((MMI_HANDLE_T)param);

    if (PNULL == scroll_ctrl_ptr)
    {
        return;
    }

    if (timer_id != scroll_ctrl_ptr->hide_timer)
    {
        //assert?
        return;
    }

    if (!scroll_ctrl_ptr->is_auto_hide)
    {
        return;
    }

    ScrollStartTimerHide(scroll_ctrl_ptr);

    return;
}

/*****************************************************************************/
//  Description : 启动自动隐藏的定时器
//  Global resource dependence :
//  Author: lianxiang.zhou & xiyuan.ma
//  Note:
/*****************************************************************************/
LOCAL void ScrollStartTimerHide(
    CTRLSCROLL_OBJ_T *scroll_ctrl_ptr
)
{
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T *) scroll_ctrl_ptr;

    if (PNULL == scroll_ctrl_ptr)
    {
        return;
    }

    if (!scroll_ctrl_ptr->is_auto_hide)
    {
        return;
    }

    scroll_ctrl_ptr->hide_timer = MMK_CreateTimerCallback(
                                      scroll_ctrl_ptr->hide_tick,
                                      ScrollTimerCallbackHide,
                                      (uint32)(base_ctrl_ptr->handle),
                                      TRUE);

    return;
}


/*****************************************************************************/
//  Description : 自动隐藏定时器callback
//  Global resource dependence :
//  Author: lianxiang.zhou & xiyuan.ma
//  Note:
/*****************************************************************************/
LOCAL void ScrollTimerCallbackHide(uint8 timer_id, uint32 param)
{
    CTRLSCROLL_OBJ_T *scroll_ctrl_ptr = GetScrollPtr((MMI_HANDLE_T)param);
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T *) scroll_ctrl_ptr;

    if (PNULL == scroll_ctrl_ptr)
    {
        return;
    }

    if (timer_id != scroll_ctrl_ptr->hide_timer)
    {
        //assert?
        return;
    }

    if (!scroll_ctrl_ptr->is_auto_hide)
    {
        return;
    }

    //hide...
    if (scroll_ctrl_ptr->layer_alpha > scroll_ctrl_ptr->alpha_change_step)
    {
        scroll_ctrl_ptr->layer_alpha -= scroll_ctrl_ptr->alpha_change_step;

        UILAYER_WeakLayerAlpha(&(base_ctrl_ptr->lcd_dev_info), scroll_ctrl_ptr->layer_alpha);
        GUILCD_Invalidate(base_ctrl_ptr->lcd_dev_info.lcd_id, 0);
    }
    else
    {
        GUI_RECT_T null_rect = {0};

        VTLBASE_SetRect(base_ctrl_ptr, &null_rect);

        ScrollStopTimerHide(scroll_ctrl_ptr);
        ScrollHideDispLayerRelease(scroll_ctrl_ptr);
    }


    return;
}



/*****************************************************************************/
//  Description : 根据宽度和显示方向重新设置显示区域
//  Global resource dependence :
//  Author: lianxiang.zhou & xiyuan.ma
//  Note:
/*****************************************************************************/
LOCAL void ScrollResetDisplayRect(
    CTRLSCROLL_OBJ_T     *scroll_ctrl_ptr,
    int32 scroll_width
)
{
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T *) scroll_ctrl_ptr;
    GUI_RECT_T disp_rect = {0};

    if (PNULL == scroll_ctrl_ptr)
    {
        return;
    }

    VTLBASE_GetDisplayRect(base_ctrl_ptr, &disp_rect);

    //根据显示方向，决定滚动条靠给定区域的哪边。滚动条一定要靠边
    if (MMI_DIRECTION_LEFT == MMITHEME_GetDisDirection())
    {
        disp_rect.left = disp_rect.right - scroll_width + 1;
    }
    else
    {
        disp_rect.right = disp_rect.left + scroll_width - 1;
    }

    VTLBASE_SetDisplayRect(base_ctrl_ptr, &disp_rect, FALSE);

    return;
}

/*****************************************************************************/
//  Description : 计算滑块的高度
//  Global resource dependence :
//  Author: lianxiang.zhou & xiyuan.ma
//  Note:
/*****************************************************************************/
LOCAL int32 ScrollCalcThumbHeight(
    CTRLSCROLL_OBJ_T     *scroll_ctrl_ptr,
    GUI_RECT_T *region_rect_ptr
)
{
    int32 full_height = 0;
    int32 thumb_height = 0;

    if (PNULL == scroll_ctrl_ptr || PNULL == region_rect_ptr)
    {
        return 0;
    }

    full_height = region_rect_ptr->bottom - region_rect_ptr->top + 1;

    thumb_height = full_height * scroll_ctrl_ptr->page_size / scroll_ctrl_ptr->total_size;  /*lint !e737*/

    if (thumb_height < (int32)scroll_ctrl_ptr->thumb_min_size)
    {
        thumb_height = scroll_ctrl_ptr->thumb_min_size;
    }

    return thumb_height;
}


/*****************************************************************************/
//  Description : 计算full滚动条参数
//  Global resource dependence :
//  Author: lianxiang.zhou & xiyuan.ma
//  Note: 设置控制rect
/*****************************************************************************/
LOCAL void ScrollCalcDispParameterFull(
    CTRLSCROLL_OBJ_T     *scroll_ctrl_ptr
)
{
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T *) scroll_ctrl_ptr;
    int32 arrow_height = 0;

    if (PNULL == scroll_ctrl_ptr)
    {
        return;
    }

    ScrollResetDisplayRect(scroll_ctrl_ptr, MMITHEME_ScrollGetFullWidth());

    VTLBASE_SetRect(base_ctrl_ptr, &(base_ctrl_ptr->display_rect));

    arrow_height = MMITHEME_ScrollGetArrowHeight();

    scroll_ctrl_ptr->top_arrow_rect = base_ctrl_ptr->display_rect;
    scroll_ctrl_ptr->top_arrow_rect.bottom = base_ctrl_ptr->display_rect.top + arrow_height - 1;

    scroll_ctrl_ptr->bottom_arrow_rect = base_ctrl_ptr->display_rect;
    scroll_ctrl_ptr->bottom_arrow_rect.top = base_ctrl_ptr->display_rect.bottom - arrow_height + 1;

    scroll_ctrl_ptr->region_rect = base_ctrl_ptr->display_rect;
    scroll_ctrl_ptr->region_rect.top = scroll_ctrl_ptr->top_arrow_rect.bottom + 1;
    scroll_ctrl_ptr->region_rect.bottom = scroll_ctrl_ptr->bottom_arrow_rect.top - 1;

    scroll_ctrl_ptr->thumb_rect = scroll_ctrl_ptr->region_rect;

    scroll_ctrl_ptr->thumb_height = ScrollCalcThumbHeight(scroll_ctrl_ptr, &(scroll_ctrl_ptr->region_rect));

    MMITHEME_ScrollGetFullImg(&(scroll_ctrl_ptr->top_arrow_img),
                              &(scroll_ctrl_ptr->top_arrow_pressed_img),
                              &(scroll_ctrl_ptr->bottom_arrow_img),
                              &(scroll_ctrl_ptr->bottom_arrow_pressed_img),
                              &(scroll_ctrl_ptr->region_bg_img),
                              &(scroll_ctrl_ptr->thumb_img));


    return;
}


/*****************************************************************************/
//  Description : 计算full thin滚动条参数
//  Global resource dependence :
//  Author: lianxiang.zhou & xiyuan.ma
//  Note:
/*****************************************************************************/
LOCAL void ScrollCalcDispParameterFullThin(
    CTRLSCROLL_OBJ_T     *scroll_ctrl_ptr
)
{
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T *) scroll_ctrl_ptr;

    if (PNULL == scroll_ctrl_ptr)
    {
        return;
    }

    ScrollResetDisplayRect(scroll_ctrl_ptr, MMITHEME_ScrollGetFullThinWidth());

    scroll_ctrl_ptr->region_rect = base_ctrl_ptr->display_rect;
    scroll_ctrl_ptr->thumb_rect = scroll_ctrl_ptr->region_rect;

    scroll_ctrl_ptr->thumb_height = ScrollCalcThumbHeight(scroll_ctrl_ptr, &(scroll_ctrl_ptr->region_rect));

    MMITHEME_ScrollGetFullThinImg(&(scroll_ctrl_ptr->region_bg_img),
                                  &(scroll_ctrl_ptr->thumb_img));


    return;
}


/*****************************************************************************/
//  Description : 计算thumb滚动条参数
//  Global resource dependence :
//  Author: lianxiang.zhou & xiyuan.ma
//  Note:
/*****************************************************************************/
LOCAL void ScrollCalcDispParameterThumb(
    CTRLSCROLL_OBJ_T     *scroll_ctrl_ptr
)
{
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T *) scroll_ctrl_ptr;

    if (PNULL == scroll_ctrl_ptr)
    {
        return;
    }

    ScrollResetDisplayRect(scroll_ctrl_ptr, MMITHEME_ScrollGetThumbWidth(base_ctrl_ptr->win_handle));

    scroll_ctrl_ptr->thumb_rect = base_ctrl_ptr->display_rect;

    scroll_ctrl_ptr->thumb_height = MMITHEME_ScrollGetThumbHeight(base_ctrl_ptr->win_handle);

    MMITHEME_ScrollGetThumbImg(&(scroll_ctrl_ptr->thumb_img));


    return;
}


/*****************************************************************************/
//  Description : 计算thumb thin滚动条参数
//  Global resource dependence :
//  Author: lianxiang.zhou & xiyuan.ma
//  Note:
/*****************************************************************************/
LOCAL void ScrollCalcDispParameterThumbThin(
    CTRLSCROLL_OBJ_T     *scroll_ctrl_ptr
)
{
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T *) scroll_ctrl_ptr;

    if (PNULL == scroll_ctrl_ptr)
    {
        return;
    }

    ScrollResetDisplayRect(scroll_ctrl_ptr, MMITHEME_ScrollGetThumbThinWidth());

    scroll_ctrl_ptr->thumb_rect = base_ctrl_ptr->display_rect;

    scroll_ctrl_ptr->thumb_height = ScrollCalcThumbHeight(scroll_ctrl_ptr, &(base_ctrl_ptr->display_rect));

    MMITHEME_ScrollGetThumbThinImg(&(scroll_ctrl_ptr->thumb_img));


    return;
}



/*****************************************************************************/
//  Description : 计算滚动条的各种显示区域
//  Global resource dependence :
//  Author: lianxiang.zhou & xiyuan.ma
//  Note: 设置普通滚动条的控制rect
/*****************************************************************************/
LOCAL void ScrollCalcDispParameter(
    CTRLSCROLL_OBJ_T     *scroll_ctrl_ptr
)
{
    if (PNULL == scroll_ctrl_ptr)
    {
        return;
    }

    if (scroll_ctrl_ptr->is_thumb_only)
    {
        //is thin?
        if (scroll_ctrl_ptr->is_auto_thin_disable)
        {
            scroll_ctrl_ptr->is_thin = TRUE;
        }
        else
        {
            /*
            计算page count
            total size, m = kn + j, j >= 0, j < n
            page size, n
            那么page count, pc = (m - 1)/n + 1 = (kn + j - 1)/n + 1
            即，
            当j=0时，pc = k
            当j>0时，pc = k + 1
            */
            uint32 page_cnt = (scroll_ctrl_ptr->total_size - 1) / scroll_ctrl_ptr->page_size + 1;

            if (page_cnt < scroll_ctrl_ptr->auto_thin_page_count)
            {
                scroll_ctrl_ptr->is_thin = TRUE;
            }
            else
            {
                scroll_ctrl_ptr->is_thin = FALSE;
            }
        }

        if (scroll_ctrl_ptr->is_thin)
        {
            scroll_ctrl_ptr->style = CTRLSCROLL_STYLE_THUMB_THIN;
        }
        else
        {
            scroll_ctrl_ptr->style = CTRLSCROLL_STYLE_THUMB;
        }
    }
    else
    {
        if (scroll_ctrl_ptr->is_thin)
        {
            scroll_ctrl_ptr->style = CTRLSCROLL_STYLE_FULL_THIN;
        }
        else
        {
            scroll_ctrl_ptr->style = CTRLSCROLL_STYLE_FULL;
        }
    }

    switch (scroll_ctrl_ptr->style)
    {
        case CTRLSCROLL_STYLE_FULL:
            ScrollCalcDispParameterFull(scroll_ctrl_ptr);
            break;

        case CTRLSCROLL_STYLE_FULL_THIN:
            ScrollCalcDispParameterFullThin(scroll_ctrl_ptr);
            break;

        case CTRLSCROLL_STYLE_THUMB:
            ScrollCalcDispParameterThumb(scroll_ctrl_ptr);
            break;

        case CTRLSCROLL_STYLE_THUMB_THIN:
            ScrollCalcDispParameterThumbThin(scroll_ctrl_ptr);
            break;

        default:
            break;
    }

    scroll_ctrl_ptr->is_dirty = FALSE;


    return;
}


/*****************************************************************************/
//  Description : 计算滑块的位置
//  Global resource dependence :
//  Author: lianxiang.zhou & xiyuan.ma
//  Note: 动态设置控制rect
/*****************************************************************************/
LOCAL void ScrollCalcThumbPos(
    CTRLSCROLL_OBJ_T     *scroll_ctrl_ptr,
    GUI_RECT_T *region_rect_ptr
)
{
    if (PNULL == scroll_ctrl_ptr || PNULL == region_rect_ptr)
    {
        return;
    }

    //position
    //做一些末尾修正
    if (scroll_ctrl_ptr->top_of_current_page + scroll_ctrl_ptr->page_size >= scroll_ctrl_ptr->total_size)
    {
        scroll_ctrl_ptr->thumb_rect.bottom = region_rect_ptr->bottom;
        scroll_ctrl_ptr->thumb_rect.top = region_rect_ptr->bottom - scroll_ctrl_ptr->thumb_height + 1;  /*lint !e737*/
    }
    else
    {
        int32 full_height = 0;
        int32 thumb_top = 0;

        full_height = region_rect_ptr->bottom - region_rect_ptr->top + 1;
        thumb_top = full_height * scroll_ctrl_ptr->top_of_current_page / scroll_ctrl_ptr->total_size;   /*lint !e737*/

        scroll_ctrl_ptr->thumb_rect.top = region_rect_ptr->top + thumb_top;
        scroll_ctrl_ptr->thumb_rect.bottom = scroll_ctrl_ptr->thumb_rect.top + scroll_ctrl_ptr->thumb_height - 1;   /*lint !e737*/
    }


    return;
}

/*****************************************************************************/
//  Description : 计算滑块的位置
//  Global resource dependence :
//  Author: lianxiang.zhou & xiyuan.ma
//  Note: 动态设置控制rect
/*****************************************************************************/
LOCAL void ScrollCalcThumbPositionFull(
    CTRLSCROLL_OBJ_T     *scroll_ctrl_ptr
)
{
    if (PNULL == scroll_ctrl_ptr)
    {
        return;
    }

    ScrollCalcThumbPos(scroll_ctrl_ptr, &(scroll_ctrl_ptr->region_rect));

    return;
}


/*****************************************************************************/
//  Description : 计算滑块的位置
//  Global resource dependence :
//  Author: lianxiang.zhou & xiyuan.ma
//  Note: 动态设置控制rect
/*****************************************************************************/
LOCAL void ScrollCalcThumbPositionThumb(
    CTRLSCROLL_OBJ_T     *scroll_ctrl_ptr
)
{
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T *) scroll_ctrl_ptr;

    if (PNULL == scroll_ctrl_ptr)
    {
        return;
    }

    ScrollCalcThumbPos(scroll_ctrl_ptr, &(base_ctrl_ptr->display_rect));

    //rect reset
    if (CTRLSCROLL_STYLE_THUMB == scroll_ctrl_ptr->style)
    {
        VTLBASE_SetRect(base_ctrl_ptr, &(scroll_ctrl_ptr->thumb_rect));
    }

    return;
}


/*****************************************************************************/
//  Description : 计算滑块的位置
//  Global resource dependence :
//  Author: lianxiang.zhou & xiyuan.ma
//  Note: 动态设置控制rect
/*****************************************************************************/
LOCAL void ScrollCalcThumbPositionThumbThin(
    CTRLSCROLL_OBJ_T     *scroll_ctrl_ptr
)
{
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T *) scroll_ctrl_ptr;

    if (PNULL == scroll_ctrl_ptr)
    {
        return;
    }

    ScrollCalcThumbPos(scroll_ctrl_ptr, &(base_ctrl_ptr->display_rect));

    return;
}

/*****************************************************************************/
//  Description : 计算滑块的位置
//  Global resource dependence :
//  Author: lianxiang.zhou & xiyuan.ma
//  Note: 动态设置控制rect
/*****************************************************************************/
LOCAL void ScrollCalcThumbPosition(
    CTRLSCROLL_OBJ_T     *scroll_ctrl_ptr
)
{
    if (PNULL == scroll_ctrl_ptr)
    {
        return;
    }

    switch (scroll_ctrl_ptr->style)
    {
        case CTRLSCROLL_STYLE_FULL:
        case CTRLSCROLL_STYLE_FULL_THIN:
            ScrollCalcThumbPositionFull(scroll_ctrl_ptr);
            break;

        case CTRLSCROLL_STYLE_THUMB:
            ScrollCalcThumbPositionThumb(scroll_ctrl_ptr);
            break;

        case CTRLSCROLL_STYLE_THUMB_THIN:
            ScrollCalcThumbPositionThumbThin(scroll_ctrl_ptr);
            break;

        default:
            break;
    }

    return;
}


/*****************************************************************************/
//  Description : display scroll control
//  Global resource dependence :
//  Author: lianxiang.zhou & xiyuan.ma
//  Note:
/*****************************************************************************/
LOCAL void ScrollDrawFull(
    CTRLSCROLL_OBJ_T     *scroll_ctrl_ptr
)
{
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T *) scroll_ctrl_ptr;
    CTRLSCROLL_REGION_T last_region = CTRLSCROLL_REGION_NONE;
    MMI_IMAGE_ID_T top_arrow = 0;
    MMI_IMAGE_ID_T bottom_arrow = 0;
    

    if (PNULL == scroll_ctrl_ptr)
    {
        return;
    }

    last_region = ScrollGetRegion(scroll_ctrl_ptr, &(scroll_ctrl_ptr->last_tp_point));

    top_arrow = scroll_ctrl_ptr->top_arrow_img;
    bottom_arrow = scroll_ctrl_ptr->bottom_arrow_img;

    if (CTRLSCROLL_REGION_TOP == scroll_ctrl_ptr->tp_down_region
		&& CTRLSCROLL_REGION_TOP == last_region)
    {
        top_arrow = scroll_ctrl_ptr->top_arrow_pressed_img;
    }
    else if (CTRLSCROLL_REGION_BOTTOM == scroll_ctrl_ptr->tp_down_region
		&& CTRLSCROLL_REGION_BOTTOM == last_region)
    {
        bottom_arrow = scroll_ctrl_ptr->bottom_arrow_pressed_img;
    }

    GUIRES_DisplayImg(PNULL,
                      &scroll_ctrl_ptr->top_arrow_rect,
                      PNULL,
                      base_ctrl_ptr->win_handle,
                      top_arrow,
                      &base_ctrl_ptr->lcd_dev_info);

    GUIRES_DisplayImg(PNULL,
                      &scroll_ctrl_ptr->bottom_arrow_rect,
                      PNULL,
                      base_ctrl_ptr->win_handle,
                      bottom_arrow,
                      &base_ctrl_ptr->lcd_dev_info);

    GUIRES_DisplayImg(PNULL,
                      &scroll_ctrl_ptr->region_rect,
                      PNULL,
                      base_ctrl_ptr->win_handle,
                      scroll_ctrl_ptr->region_bg_img,
                      &base_ctrl_ptr->lcd_dev_info);

    GUIRES_DisplayImg(PNULL,
                      &scroll_ctrl_ptr->thumb_rect,
                      PNULL,
                      base_ctrl_ptr->win_handle,
                      scroll_ctrl_ptr->thumb_img,
                      &base_ctrl_ptr->lcd_dev_info);

    return;
}


/*****************************************************************************/
//  Description : display scroll control
//  Global resource dependence :
//  Author: lianxiang.zhou & xiyuan.ma
//  Note:
/*****************************************************************************/
LOCAL void ScrollDrawFullThin(
    CTRLSCROLL_OBJ_T     *scroll_ctrl_ptr
)
{
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T *) scroll_ctrl_ptr;

    if (PNULL == scroll_ctrl_ptr)
    {
        return;
    }

    GUIRES_DisplayImg(PNULL,
                      &scroll_ctrl_ptr->region_rect,
                      PNULL,
                      base_ctrl_ptr->win_handle,
                      scroll_ctrl_ptr->region_bg_img,
                      &base_ctrl_ptr->lcd_dev_info);

    GUIRES_DisplayImg(PNULL,
                      &scroll_ctrl_ptr->thumb_rect,
                      PNULL,
                      base_ctrl_ptr->win_handle,
                      scroll_ctrl_ptr->thumb_img,
                      &base_ctrl_ptr->lcd_dev_info);

    return;
}


/*****************************************************************************/
//  Description : display scroll control
//  Global resource dependence :
//  Author: lianxiang.zhou & xiyuan.ma
//  Note:
/*****************************************************************************/
LOCAL void ScrollDrawThumb(
    CTRLSCROLL_OBJ_T     *scroll_ctrl_ptr
)
{
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T *) scroll_ctrl_ptr;

    if (PNULL == scroll_ctrl_ptr)
    {
        return;
    }

    GUIRES_DisplayImg(PNULL,
                      &scroll_ctrl_ptr->thumb_rect,
                      PNULL,
                      base_ctrl_ptr->win_handle,
                      scroll_ctrl_ptr->thumb_img,
                      &base_ctrl_ptr->lcd_dev_info);

    return;
}


/*****************************************************************************/
//  Description : 设置自动隐藏时的显示层
//  Global resource dependence :
//  Author: lianxiang.zhou & xiyuan.ma
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ScrollHideDispLayerSetting(
    CTRLSCROLL_OBJ_T     *scroll_ctrl_ptr
)
{
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T *) scroll_ctrl_ptr;
    UILAYER_CREATE_T create_info = {0};
    GUI_RECT_T buffer_rect = {0};

    if (PNULL == scroll_ctrl_ptr)
    {
        return FALSE;
    }

    if (!UILAYER_IsMultiLayerEnable())
    {
        SCI_PASSERT(0, ("ScrollHideDispLayerSetting(): MultiLayer Disabled!"));  /*assert verified*/
    }

    //已经创建
    if (UILAYER_NULL_HANDLE != scroll_ctrl_ptr->hide_layer.block_id)
    {
        return TRUE;
    }

    //创建
    buffer_rect = base_ctrl_ptr->display_rect;

    create_info.lcd_id = MAIN_LCD_ID;
    create_info.owner_handle = base_ctrl_ptr->win_handle;
    create_info.offset_x = buffer_rect.left;
    create_info.offset_y = buffer_rect.top;
    create_info.width = buffer_rect.right - buffer_rect.left + 1;
    create_info.height = buffer_rect.bottom - buffer_rect.top + 1;
    create_info.is_bg_layer = FALSE;//TRUE;
    create_info.is_static_layer = FALSE;
    create_info.format = UILAYER_MEM_DOUBLE_COPY;

    UILAYER_CreateLayer(&create_info, &(scroll_ctrl_ptr->hide_layer));
    UILAYER_SetLayerColorKey(&(scroll_ctrl_ptr->hide_layer), TRUE, UILAYER_TRANSPARENT_COLOR);

    //加载
    {
        UILAYER_APPEND_BLT_T append_layer = {0};

        append_layer.lcd_dev_info = scroll_ctrl_ptr->hide_layer;
        append_layer.layer_level = UILAYER_LEVEL_HIGH;

        UILAYER_AppendBltLayer(&append_layer);
    }

    //设置基类的lcd info
    base_ctrl_ptr->lcd_dev_info = scroll_ctrl_ptr->hide_layer;

    return TRUE;
}


/*****************************************************************************/
//  Description : 释放自动隐藏时的显示层
//  Global resource dependence :
//  Author: lianxiang.zhou & xiyuan.ma
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ScrollHideDispLayerRelease(
    CTRLSCROLL_OBJ_T     *scroll_ctrl_ptr
)
{
    if (PNULL == scroll_ctrl_ptr)
    {
        return FALSE;
    }

    UILAYER_RemoveBltLayer(&(scroll_ctrl_ptr->hide_layer));
    UILAYER_RELEASELAYER(&(scroll_ctrl_ptr->hide_layer)); /*lint !e506 !e774*/

    return TRUE;
}


/*****************************************************************************/
//  Description : display scroll control
//  Global resource dependence :
//  Author: lianxiang.zhou & xiyuan.ma
//  Note:
/*****************************************************************************/
LOCAL void ScrollDraw(
    CTRLSCROLL_OBJ_T     *scroll_ctrl_ptr
)
{
    if (PNULL == scroll_ctrl_ptr)
    {
        return;
    }

    if (scroll_ctrl_ptr->is_dirty)
    {
        ScrollCalcDispParameter(scroll_ctrl_ptr);
    }

    if (scroll_ctrl_ptr->is_moved)
    {
        ScrollCalcThumbPosition(scroll_ctrl_ptr);
    }

    if (scroll_ctrl_ptr->is_auto_hide)
    {
        BOOLEAN is_layer_ok = ScrollHideDispLayerSetting(scroll_ctrl_ptr);

        ScrollStopTimerHide(scroll_ctrl_ptr);

        if (!is_layer_ok)
        {
            return;
        }

        //disp_layer每次都会重画，使用双缓冲切换的方式
        UILAYER_SwitchDoubleMem(&(scroll_ctrl_ptr->hide_layer), TRUE);

        UILAYER_Clear(&(scroll_ctrl_ptr->hide_layer));
    }

    IMG_EnableTransparentColor(TRUE);
    GUI_SetAlphaOverwrite(TRUE);

    switch (scroll_ctrl_ptr->style)
    {
        case CTRLSCROLL_STYLE_FULL:
            ScrollDrawFull(scroll_ctrl_ptr);
            break;

        case CTRLSCROLL_STYLE_FULL_THIN:
            ScrollDrawFullThin(scroll_ctrl_ptr);
            break;

        case CTRLSCROLL_STYLE_THUMB:
        case CTRLSCROLL_STYLE_THUMB_THIN:
            ScrollDrawThumb(scroll_ctrl_ptr);
            break;

        default:
            break;
    }

    GUI_SetAlphaOverwrite(FALSE);
    IMG_EnableTransparentColor(FALSE);

    if (scroll_ctrl_ptr->is_auto_hide)
    {
        scroll_ctrl_ptr->layer_alpha = SCROLL_HIDE_ALPHA_INIT;
        ScrollStartTimerHideStart(scroll_ctrl_ptr);
    }

    return;
}

/*****************************************************************************/
//  Description : get scroll pointer by control id
//  Global resource dependence :
//  Author: lianxiang.zhou & xiyuan.ma
//  Note:
/*****************************************************************************/
LOCAL CTRLSCROLL_OBJ_T *GetScrollPtr(
    MMI_HANDLE_T     ctrl_handle
)
{
    CTRLBASE_OBJ_T  *ctrl_ptr = (CTRLBASE_OBJ_T *)MMK_GetCtrlPtr(ctrl_handle);

    if (PNULL != ctrl_ptr)
    {
        SCI_ASSERT(ScrollTypeOf(ctrl_ptr));   /*assert verified*/
    }

    return (CTRLSCROLL_OBJ_T *) ctrl_ptr;
}

/*****************************************************************************/
//  Description : ext type of
//  Global resource dependence :
//  Author:lianxiang.zhou & xiyuan.ma
//  Note: judge control type of
/*****************************************************************************/
LOCAL BOOLEAN ScrollTypeOf(
    CTRLBASE_OBJ_T     *ctrl_ptr
)
{
    return TYPEMNG_IsTypeOf((OBJECT_TYPE_PTR)ctrl_ptr, CTRL_SCROLL_TYPE);
}

/*****************************************************************************/
//  Description : set control whether display progress
//  Global resource dependence :
//  Author: lianxiang.zhou & xiyuan.ma
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ScrollCtrlSetScrollbarEnabled(
    CTRLBASE_OBJ_T       *base_ctrl_ptr,
    BOOLEAN              is_display
)
{
    BOOLEAN     result = FALSE;

    // TODO
    // 注意: 有滚动条功能的控件需要实现这个接口
    // ...

    return result;
}

/*****************************************************************************/
//  Description : set control is circular handle up/down key
//  Global resource dependence :
//  Author: lianxiang.zhou & xiyuan.ma
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ScrollCtrlSetUpDownLoop(
    CTRLBASE_OBJ_T       *base_ctrl_ptr,
    BOOLEAN              is_circular
)
{
    BOOLEAN     result = FALSE;

    // TODO
    // 注意: 像list等这种有多个item项的控件，需要实现这个接口.

    // 其他单项控件，如button，不需要实现
    // ...

    return (result);
}

/*****************************************************************************/
//  Description : get control height by width,include content,border etc.
//  Global resource dependence :
//  Author: lianxiang.zhou & xiyuan.ma
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ScrollCtrlGetHeightByWidth(
    CTRLBASE_OBJ_T      *base_ctrl_ptr,  //in:
    uint16              width,          //in:
    uint16              *height_ptr     //in/out:
)
{
    BOOLEAN     result = FALSE;

    // TODO
    // ...

    return (result);
}


/*****************************************************************************/
//  Description : set ctrl rect
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ScrollCtrlSetRect(
    CTRLBASE_OBJ_T      *base_ctrl_ptr, //in:
    const GUI_RECT_T    *rect_ptr
)
{
    CTRLBASE_VTBL_T *parent_vtbl_ptr = (CTRLBASE_VTBL_T *) TYPEMNG_GetParentVtbl(CTRL_SCROLL_TYPE, (OBJECT_TYPE_PTR) base_ctrl_ptr);

    if (PNULL == base_ctrl_ptr || PNULL == rect_ptr || PNULL == parent_vtbl_ptr)
    {
        return FALSE;
    }

    // TODO
    parent_vtbl_ptr->SetRect(base_ctrl_ptr, rect_ptr);

    base_ctrl_ptr->display_rect = *rect_ptr;

    return (TRUE);
}


/*****************************************************************************/
//  Description : set ctrl display rect
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ScrollCtrlSetDisplayRect(
    CTRLBASE_OBJ_T      *base_ctrl_ptr, //in:
    const GUI_RECT_T    *rect_ptr,      //in:
    BOOLEAN             is_update       //in:
)
{
    BOOLEAN     result = FALSE;

    // TODO
    // ...

    return (result);
}

/*****************************************************************************/
//  Description : set control select rect
//  Global resource dependence :
//  Author: lianxiang.zhou & xiyuan.ma
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ScrollCtrlGetSelectRect(
    CTRLBASE_OBJ_T         *base_ctrl_ptr,  //in:
    GUI_RECT_T             *rect_ptr       //in/out:
)
{
    BOOLEAN     result = FALSE;

    // TODO
    // 注意: 像list等这种有多个item项的控件，需要实现这个接口
    // 其他单项控件，如button，不需要实现
    // ...

    return (result);
}

/*****************************************************************************/
//  Description : set control select index,is the first or last line
//  Global resource dependence :
//  Author: lianxiang.zhou & xiyuan.ma
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ScrollCtrlGetSelectIndex(
    CTRLBASE_OBJ_T        *base_ctrl_ptr,  //in:
    BOOLEAN               *is_first_ptr,  //in/out:
    BOOLEAN               *is_last_ptr    //in/out:
)
{
    BOOLEAN     result = FALSE;

    // TODO
    // 注意: 像list等这种有多个item项的控件，需要实现这个接口
    // 其他单项控件，如button，不需要实现
    // ...

    return (result);
}


/*****************************************************************************/
//  Description : set control total size
//  Global resource dependence :
//  Author: lianxiang.zhou & xiyuan.ma
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLSCROLL_SetTotalSize(
    MMI_HANDLE_T ctrl_handle, //in
    uint32 total_size //in
)
{
    CTRLSCROLL_OBJ_T *scroll_ctrl_ptr = GetScrollPtr(ctrl_handle);

    if (PNULL == scroll_ctrl_ptr)
    {
        return FALSE;
    }

    scroll_ctrl_ptr->total_size = total_size;

    scroll_ctrl_ptr->is_dirty = TRUE;

    return TRUE;
}


/*****************************************************************************/
//  Description : set control page size
//  Global resource dependence :
//  Author: lianxiang.zhou & xiyuan.ma
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLSCROLL_SetPageSize(
    MMI_HANDLE_T ctrl_handle, //in
    uint32 page_size //in
)
{
    CTRLSCROLL_OBJ_T *scroll_ctrl_ptr = GetScrollPtr(ctrl_handle);

    if (PNULL == scroll_ctrl_ptr)
    {
        return FALSE;
    }

    scroll_ctrl_ptr->page_size = page_size;

    scroll_ctrl_ptr->is_dirty = TRUE;

    return TRUE;
}


/*****************************************************************************/
//  Description : set control line size
//  Global resource dependence :
//  Author: lianxiang.zhou & xiyuan.ma
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLSCROLL_SetLineSize(
    MMI_HANDLE_T ctrl_handle, //in
    uint32 line_size //in
)
{
    CTRLSCROLL_OBJ_T *scroll_ctrl_ptr = GetScrollPtr(ctrl_handle);

    if (PNULL == scroll_ctrl_ptr)
    {
        return FALSE;
    }

    scroll_ctrl_ptr->line_size = line_size;
    scroll_ctrl_ptr->is_dirty = TRUE;

    return TRUE;
}


/*****************************************************************************/
//  Description : set control top of current page
//  Global resource dependence :
//  Author: lianxiang.zhou & xiyuan.ma
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLSCROLL_SetCurrentPostion(
    MMI_HANDLE_T ctrl_handle, //in
    uint32 top_of_current_page//in
)
{
    CTRLSCROLL_OBJ_T *scroll_ctrl_ptr = GetScrollPtr(ctrl_handle);

    if (PNULL == scroll_ctrl_ptr)
    {
        return FALSE;
    }

    scroll_ctrl_ptr->top_of_current_page = top_of_current_page;
    scroll_ctrl_ptr->is_dirty = TRUE;

    return TRUE;
}


/*****************************************************************************/
//  Description : control update
//  Global resource dependence :
//  Author: lianxiang.zhou & xiyuan.ma
//  Note:
/*****************************************************************************/
PUBLIC void CTRLSCROLL_Update(
    MMI_HANDLE_T ctrl_handle
)
{
    CTRLSCROLL_OBJ_T *scroll_ctrl_ptr = GetScrollPtr(ctrl_handle);

    if (PNULL == scroll_ctrl_ptr)
    {
        return;
    }

    ScrollDraw(scroll_ctrl_ptr);

    return;
}

/*****************************************************************************/
//  Description : control update
//  Global resource dependence :
//  Author: lianxiang.zhou & xiyuan.ma
//  Note:
/*****************************************************************************/
PUBLIC void CTRLSCROLL_DisableAutoThin(
    MMI_HANDLE_T ctrl_handle
)
{
    CTRLSCROLL_OBJ_T *scroll_ctrl_ptr = GetScrollPtr(ctrl_handle);

    if (PNULL == scroll_ctrl_ptr)
    {
        return;
    }

    scroll_ctrl_ptr->is_auto_thin_disable = TRUE;

    return;
}



