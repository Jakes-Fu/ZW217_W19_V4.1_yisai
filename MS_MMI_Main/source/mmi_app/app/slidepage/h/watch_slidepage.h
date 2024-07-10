/****************************************************************************
** File Name:      watch_slidepage.h                                                                                *
** Author:                                                                                                                    *
** Date:             02/03/2020                                                                                         *
** Copyright:      2020 Spreadtrum, Incoporated. All Rights Reserved.                                  *
** Description:    This file is used to slidepage entity   (多页水平滑动页)
*****************************************************************************
**                      Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE            NAME             DESCRIPTION                               *
** 02/03/2020  zhikun.lv         Create
** 03/10/2020  zhikun.lv         Add Cycle attribute.
******************************************************************************/
#ifndef WATCH_SLIDEPAGE_H
#define WATCH_SLIDEPAGE_H

#include "mmi_common.h"
#include "mmi_appmsg.h"
#ifdef ADULT_WATCH_SUPPORT
#include "mmi_adapt_screen.h"
#endif
#include "mmi_position.h"

/*define the max count of slidepage entity*/
#define SLIDE_PAGE_ENTITY_MAX 4

/*define max page count in slidepage*/
#define SLIDE_PAGE_MAXNUM 10

/*define active page number in slidepage. there is 3 items, include previous/current/next*/
#define SLIDE_PAGE_NUM  3

/*orgin position*/
#define SLIDE_PAGE_ORI DP2PX_VALUE(0)

/*the width of slidepage. must equal to the sliding page width*/
#define SLIDE_PAGE_WIDTH DP2PX_VALUE(MMI_MAINSCREEN_WIDTH)

#define SLIDE_PAGE_HEIGHT DP2PX_VALUE(MMI_MAINSCREEN_HEIGHT)

#define SLIDE_CHANGE_PAGE_THR DP2PX_VALUE(30)

/*the animation interval of slidepage*/
#define SLIDE_PAGE_TPDRAP_INTERVAL 25

/*the thres of horzional slide startup*/
#define SLIDE_PAGE_TPDRAP_THRES DP2PX_VALUE(2)

/*the step value of slide animation*/
#define SLIDE_PAGE_AUTOSLIDE_THRES DP2PX_VALUE(5)


#define SLIDE_PAGE_AUTO_FRAMENUM 10

#define SLIDEPAGE_UPDATE_RECT DP2PX_RECT(0,0,MMI_MAINSCREEN_WIDTH,MMI_MAINSCREEN_HEIGHT)


/*define the direction of slidepage*/
typedef enum eWatchSlideDirection
{
    WATCH_SLIDE_DIRECTION_LEFT,
    WATCH_SLIDE_DIRECTION_RIGHT
}eWatchSlideDirection;

/*define active page number in slidepage. there is 3 items, include previous/current/next*/
typedef enum eWatchSlideOperType
{
    WATCH_SLIDE_OPER_PREVONE,
    WATCH_SLIDE_OPER_CURONE,
    WATCH_SLIDE_OPER_NEXTONE,

    WATCH_SLIDE_OPER_MAX
}eWatchSlideOperType;

/*typedef slidepage item enter function*/
typedef void (*PFUN_WatchSlidePageItemWin_Enter)();

/*declear slide page item*/
typedef struct tWatchSlidePageItem
{
    MMI_WIN_ID_T win_id;
    PFUN_WatchSlidePageItemWin_Enter fun_enter_win;
    GUI_RECT_T win_rect_tp_pressed;
    BOOLEAN is_disabled;
}tWatchSlidePageItem;

/*declear slide page*/
typedef struct tWatchSlidePage
{
    BOOLEAN is_used;

    //win id of slide page item
    tWatchSlidePageItem pages[SLIDE_PAGE_MAXNUM];

    //count of slide page items
    uint8 page_cnt;

    //page width
    uint16 page_width;

    //index of current page
    uint8 cur_page_index;

    //index of previous page
    uint8 prev_page_index;

    //index of next page
    uint8 next_page_index;

    //index of target page, used for draping && autosliding
    uint8 target_page_index;

    //index of another page, used for draping && autosliding
    uint8 second_page_index;

    //the total value for auto sliding
    int16 total_vaule_to_slide;

    //the current value for auto sliding
    int16 cur_value_in_slide;

    //point of touch press
    GUI_POINT_T tp_press_point;

    //point of touch move
    GUI_POINT_T tp_move_point;

    //point of touch up
    GUI_POINT_T tp_up_point;

    GUI_POINT_T tp_last_point;

    //whether touch pressed
    BOOLEAN is_tp_pressed;

    //whether touch moved
    BOOLEAN is_tp_moved;

    //the direction of slide action
    eWatchSlideDirection direction;

    //if slide is run, then this value is true;
    BOOLEAN is_draped;

    //the stage of touch up, for auto slidng
    BOOLEAN is_auto_sliding;

    BOOLEAN is_support_cycle;

    //the timer of auto sliding
    uint8 auto_sliding_timer;

    uint8 tp_drap_timer;

    PROCESSMSG_FUNC fun_cb;

    uint32 focus_win_id;
}tWatchSlidePage;


/*****************************************************************************
//  Description : 从系统中申请创建一个SlidePage 对象
//  Author: zhikun.lv
//  Param:N/A
//  Return: 返回申请到的Handle句柄，非0为有效值
//  Note:
*****************************************************************************/
PUBLIC MMI_HANDLE_T WatchSLIDEPAGE_CreateHandle(void);


/*****************************************************************************
//  Description : 释放SlidePage 对象.可能不需要显示的调用,
//                     因为当SlidePage的某个页面关闭时，会自动关闭SlidePage
//  Author: zhikun.lv
//  Param:N/A
//  Return:
//  Note:
*****************************************************************************/
PUBLIC BOOLEAN WatchSLIDEPAGE_DestoryHandle(MMI_HANDLE_T handle);


/*****************************************************************************
//  Description : 启动SlidePage
//  Author: zhikun.lv
//  Param:
      handle ---handle of slidepage
      page_arr --- slidepageItem info array
      page_cnt --- count of slidepageItem
      focus_index --- the default index of window to garb focus
      is_support_cycle --- is support cycle tranform in slidepage
      fun_cb --- reserved
//  Return:
//  Note:
*****************************************************************************/
PUBLIC BOOLEAN WatchSLIDEPAGE_Open(MMI_HANDLE_T handle,
                                                                            tWatchSlidePageItem *page_arr,
                                                                            uint8 page_cnt,
                                                                            uint8 focus_index,
                                                                            BOOLEAN is_support_cycle,
                                                                            PROCESSMSG_FUNC fun_cb);


/*****************************************************************************
//  Description : 关闭SlidePage 对象.目前不需要显示的调用，
//                     因为当SlidePage的某个页面关闭时，会自动关闭SlidePage
//  Author: zhikun.lv
//  Param:N/A
//  Return:
//  Note:
*****************************************************************************/
PUBLIC BOOLEAN WatchSLIDEAGE_Close(MMI_HANDLE_T handle);


/*****************************************************************************
//  Description : Get SlidePage Infomation
//  Author: zhikun.lv
//  Return: 
//  Note:
*****************************************************************************/
PUBLIC tWatchSlidePage* WatchSLIDEPAGE_GetSlideInfoByHandle(MMI_HANDLE_T handle);


/*****************************************************************************
//  Description : set current page index of slide entity
//  Author: zhikun.lv
//  Return:
//  Note:
*****************************************************************************/
PUBLIC void WatchSLIDEAGE_SetCurrentPageIndex(uint8 currentIndex);


/*****************************************************************************
//  Description : set page enable by winid.
//  Author: zhikun.lv
//  Return:
//  Note:
*****************************************************************************/
PUBLIC void WatchSLIDEAGE_SetPageEnable(MMI_WIN_ID_T win_id, BOOLEAN bEnable);

PUBLIC void WatchSLIDEPAGE_SetSlideBufferEnable(BOOLEAN bEnableSlideBuffer);

#endif
