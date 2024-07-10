/****************************************************************************
** File Name:  watch_slidewin.h                                                                                    *
** Author:                                                                                                                  *
** Date:          02/26/2020                                                                                          *
** Copyright:   2020 Spreadtrum, Incoporated. All Rights Reserved.                                   *
** Description: slidewin common win                                                                             *
*****************************************************************************
**                         Important Edit History                                                                       *
** -------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                                                                   *
** 03/26/2020 zhikun.lv        Create                                                                              *
** 05/24/2021 zhikun.lv        Update                                                                              *
******************************************************************************/
#ifndef WATCH_SLIDEWIN_H
#define WATCH_SLIDEWIN_H

#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmidisplay_data.h"
#include "window_parse.h"
#include "guires.h"
#include "mmi_appmsg.h"
#include "mmi_adapt_screen.h"

#define SLIDEWIN_MOTION_THRES DP2PX_VALUE(5) //horzontal / vertial motion thres pixel
#define SLIDEWIN_AUTOSLIDE_DEFAULT_STEP DP2PX_VALUE(40) //pixel
#define SLIDEWIN_AUTOSLIDE_INTERVAL 40 //ms
#define SLIDEWIN_TPUP_THRES DP2PX_VALUE(240 >> 1) //pixel
#define SLIDEWIN_TPMOVE_INTERVAL 40 //ms

typedef enum eSlideWinDirection
{
    SLIDEWIN_DIRECTION_HORZ,
    SLIDEWIN_DIRECTION_VERT,
    SLIDEWIN_DIRECTION_MAX
}eSlideWinDirection;

typedef enum eSlideWinStartUpMode
{
    SLIDEWIN_STARTUPMODE_TP,
    SLIDEWIN_STARTUPMODE_AUTO,
    SLIDEWIN_STARTUPMODE_MAX
}eSlideWinStartUpMode;

#define SLIDEWIN_STYLE_ENABLE_ROI 0X0001
#define SLIDEWIN_STYLE_ENABLE_RANGE 0X0002
#define SLIDEWIN_STYLE_ENABLE_FREE_WHEN_WINCLOSE 0x0004

#define SLIDEWIN_STATE_IDLE 0X0000
#define SLIDEWIN_STATE_TP_PRESSED 0X0001
#define SLIDEWIN_STATE_TP_MOVING  0X0002
#define SLIDEWIN_STATE_TP_UPED       0X0004
#define SLIDEWIN_STATE_TP_AUTO_SLIDING 0X0008
#define SLIDEWIN_STATE_DRAPING 0X0010


#define T_SLIDEWIN_INIT_DATA \
    MMI_WIN_ID_T win_id;  \
    MMI_WIN_ID_T win_before_id; \
    eSlideWinDirection direction; \
    eSlideWinStartUpMode mode; \
    uint32 style; \
    int16 init_pos; \
    int16 stop_pos; \
    int16 close_pos;  \
    int16 range_min; \
    int16 range_max; \
    GUI_RECT_T roi_region;

#define T_SLIDEWIN_RUN_DATA_BASE \
    GUI_LCD_DEV_INFO slide_layer; \
    uint32 state; \
    uint8 tp_drap_timer; \
    uint32 tp_drap_interval; \
    uint8 auto_slide_timer; \
    uint16 auto_slide_step; \
    uint32 auto_slide_interval; \
    int16 cur_pos;  \
    GUI_POINT_T tp_press_pos; \
    GUI_POINT_T tp_move_pos; \
    GUI_POINT_T tp_up_pos;


typedef struct tSlideWinInitData
{
    T_SLIDEWIN_INIT_DATA
}tSlideWinInitData;

typedef struct tSlideWinInfo
{
    T_SLIDEWIN_INIT_DATA
    T_SLIDEWIN_RUN_DATA_BASE
}tSlideWinInfo;


/*****************************************************************************/
//  Description : create slide win entity, which return reference slide handle,if error occur, null will be return.
//  Parameter: [In]init_data : initilize information
//            [Return] slide win handle, in heap memory
//  Author: zhikun.lv
//  Note: must invoke this function after mmk_createwin.
//           also, the win userdata ptr must be null !!!!!!
/*****************************************************************************/
PUBLIC uint32 WatchSLIDEWIN_Create(tSlideWinInitData* init_data);



/*****************************************************************************/
//  Description : destory slide win entity
//  Parameter: [In]handle:slide win handle
//            [Return] TRUE/FALSE
//  Author: zhikun.lv
//  Note: must call this function with WatchSLIDEWIN_Create on pair,otherwise,forbid leak memory.
/*****************************************************************************/
PUBLIC BOOLEAN WatchSLIDEWIN_Destory(uint32 handle);



/*****************************************************************************/
//  Description : start slide win entity
//  Parameter: [In]handle:slide win handle
//            [Return] TRUE/FALSE
//  Author: zhikun.lv
//  Note: must call this function after WatchSLIDEWIN_Create().use handle return by WatchSLIDEWIN_Create
/*****************************************************************************/
PUBLIC BOOLEAN WatchSLIDEWIN_Start(uint32 handle);



#endif
