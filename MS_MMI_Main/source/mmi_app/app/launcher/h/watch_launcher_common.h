/*****************************************************************************
** File Name:      watch_launcher_common.h                                   *
** Author:                                                                   *
** Date:           02/03/2020                                                *
** Copyright:      2020 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is for draw common clock
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 02/03/2020  zhikun.lv        Create
******************************************************************************/
#ifndef WATCH_LAUNCHER_COMMON_H
#define WATCH_LAUNCHER_COMMON_H
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmidisplay_data.h"
#include "window_parse.h"
#include "guires.h"
#include "mmi_appmsg.h"
#ifdef ADULT_WATCH_SUPPORT
#include "mmi_adapt_screen.h"
#endif

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                  *
**---------------------------------------------------------------------------*/
#define LAUNCHER_WIDTH DP2PX_VALUE(MMI_MAINSCREEN_WIDTH)
#define LAUNCHER_HEIGHT DP2PX_VALUE(MMI_MAINSCREEN_HEIGHT)

#define DIGITAL_CLOCK_NUMBER_HEIGHT DP2PX_VALUE(84)
#define DIGITAL_CLOCK_NUMBER_WIDTH DP2PX_VALUE(56)
#define DIGITAL_CLOCK_WEEK_WIDTH_OFFSET DP2PX_VALUE(48)
#define DIGITAL_CLOCK_WEEK_HEIGHT_OFFSET DP2PX_VALUE(12)

#define LAUNCHER_PAGE_NUM 8
#define LAUNCHER_TOP_MARGIN DP2PX_VALUE(0)
#define LAUNCHER_BOTTOM_MARGIN DP2PX_VALUE(30)
#define LAUNCHER_TITLE_LEFT_MARGIN DP2PX_VALUE(48)
#define LAUNCHER_CLIENT_RECT DP2PX_RECT(0,0,MMI_MAINSCREEN_WIDTH,MMI_MAINSCREEN_HEIGHT)
#ifdef SCREEN_SHAPE_CIRCULAR
#define LAUNCHER_TITLE_RECT DP2PX_RECT(0,192,240,214)
#else
#define LAUNCHER_TITLE_RECT DP2PX_RECT(0,0,240,54)
#endif
#define LAUNCHER_INDICATOR_RECT DP2PX_RECT(32,220,MMI_MAINSCREEN_WIDTH-1,MMI_MAINSCREEN_HEIGHT)
#define LAUNCHER_NOTIFY_AREA DP2PX_RECT(0, 0, MMI_MAINSCREEN_WIDTH, 30)
#define LAUNCHER_SOFTKEY_AREA DP2PX_RECT(0, MMI_MAINSCREEN_HEIGHT-LAUNCHER_BOTTOM_MARGIN, MMI_MAINSCREEN_WIDTH, MMI_MAINSCREEN_HEIGHT)
#define LAUNCHER_PAGE_TITLE_RECT DP2PX_RECT(LAUNCHER_TITLE_LEFT_MARGIN, LAUNCHER_TOP_MARGIN, DP2PX_VALUE(192), LAUNCHER_TOP_MARGIN + DP2PX_VALUE(30))
#define LAUNCHER_NOTIFY_UNREAD_MESSAGE_RECT DP2PX_RECT(156, 60, 180, 84)
#define LAUNCHER_NOTIFY_UNREAD_MESSAGE_MAX_RECT DP2PX_RECT(156, 60, 192, 84)
#define LAUNCHER_NOTIFICATION_NUM_MAX_LEN 3
#define LAUNCHER_NOTIFICATION_MAX_NUM 99
#define LAUNCHER_CLOCK_NOTIFICATION_RECT DP2PX_RECT(118,209,122,213)
#define LAUNCHER_CLOCK_NOTIFICATION_RECT1 DP2PX_RECT(115,4,124,13)



#define LAUNCHER_CLOCK_DEFAULT_FONT DP2PX_FONT(SONG_FONT_14)

#define LAUNCHER_BACKGROUND_COLOR MMI_BLACK_COLOR

#ifdef ADULT_WATCH_SUPPORT
#define LAUNCHER_ELEM_COUNT 6
#else
#if defined(JAVASCRIPT_ECO_ENVIRONMENT_SUPPORT) || defined(APP_STORE_SUPPORT) || defined(DYNAMIC_WATCHFACE_SUPPORT)
#define LAUNCHER_ELEM_COUNT 10 //新增了一个extralauncher用于js,appstore,watchface
#else
#define LAUNCHER_ELEM_COUNT 9
#endif
#endif

/*****************************************************************************
//  Description : Display Analog Clock on win, the display_rect is paint area.
//  Author: zhikun.lv
//  Param:N/A
//  Return:
//  Note:
*****************************************************************************/
PUBLIC void WatchLAUNCHER_DrawAnalogClock(MMI_WIN_ID_T win_id,
                                                                           GUI_RECT_T display_rect);

/*****************************************************************************
//  Description : Display Digital Clock on win, the display_rect is paint area.
//  Author: zhikun.lv
//  Param:N/A
//  Return:
//  Note:
*****************************************************************************/
PUBLIC void WatchLAUNCHER_DrawDigitalClock(MMI_WIN_ID_T win_id,
                                                                                           GUI_RECT_T display_rect,
                                                                                           BOOLEAN is_12hour);
#ifdef ADULT_WATCH_SUPPORT
/*****************************************************************************
//  Description : Display MTF Clock on win, the display_rect is paint area.
//  Author: haosheng.cui
//  Param:N/A
//  Return:
//  Note:
*****************************************************************************/

PUBLIC void WatchLAUNCHER_DrawMTFClock(MMI_WIN_ID_T win_id,
                                                                                            GUI_RECT_T display_rect);
/*****************************************************************************
//  Description : get week string
//  Author: haosheng.cui
//  Param:N/A
//  Return:
//  Note:
*****************************************************************************/
PUBLIC MMI_STRING_T WatchLAUNCHER_GetWeekString(uint8 week_id);
#endif

/*****************************************************************************
//  Description : Display image by id to dst buffer whit angle.
//  Author: zhikun.lv
//  Param:N/A
//  Return:
//  Note:
*****************************************************************************/
PUBLIC BOOLEAN DisplayImageToBufferWithAngle(uint8 *dst_buf,
    uint16 dst_width,
    uint16 dst_height,
    MMI_WIN_ID_T win_id,
    MMI_IMAGE_ID_T image_id,
    uint32 angle);


#endif
