/*****************************************************************************
** File Name:      mmiwidget.c                                                  *
** Author:                                                                   *
** Date:           06/2007                                                   *
** Copyright:      2005 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe widget control                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME               DESCRIPTION                             *
** 04/2010        bin.ji     Create
******************************************************************************/

#ifndef _MMIWIDGET_C_
#define _MMIWIDGET_C_

/*---------------------------------------------------------------------------*/
/*                         Include Files                                     */
/*---------------------------------------------------------------------------*/
#include "mmi_app_widget_trc.h"
#include "mmiwidget.h"
#include "guilcd.h"
#include "guicommon.h"
#include "mmitheme_widget.h"
#include "mmitheme_pubwin.h"
#include "mmi_theme.h"
#include "guifont.h"
#include "mmk_timer.h"
#include "guires.h"
#include "guistring.h"
#include "mmi_appmsg.h"
#include "mmiwidget_nv.h"
#include "mmi_nv.h"
#include "lcd_cfg.h"
#include "mmiidle_export.h"
//#include "mmiset.h"
#include "mmiset_export.h"

#include "mmidisplay_data.h"
//#include "mmiset_wintab.h"
#include "mmi_mainmenu_export.h"
#include "mmiwidget_id.h"
#include "mmi_id.h"
#include "chng_freq.h"
#include "mmimp3_export.h"
#include "mmi_wallpaper_export.h"
//#include "guictrl_api.h"
#include "mmk_tp.h"
#include "Guistatusbar.h"
#include "mmiwidget_image.h"
#include "mmiwidget_position.h"
#include "mmi_position.h"
#include "chng_freq.h"
#include "mmiwidget_text.h"
#include "mmipub.h"
//#include "mmieng.h"
#include "mmieng_export.h"
#include "mmiwidget_export.h"
#include "mmiwidget_internal.h"
#ifdef MMI_IDLE_NO_SIM_SOS_SUPPORT
#include "mmiidle_cstyle.h"
#include "mmicc_export.h"
#endif
#ifdef MMIWIDGET_WEATHER_ANIM
#include "mmiweather.h"
#endif
/*---------------------------------------------------------------------------*/
/*                         Macro Declaration                                 */
/*---------------------------------------------------------------------------*/

#define MMIWIDGET_VIBRATE_STEP_COUNT    1//2
#define MMIWIDGET_VIBRATE_HSTEP         3
#define MMIWIDGET_VIBRATE_VSTEP         6
#define MMIWIDGET_VIBRATE_STEP_RANGE    (MMIWIDGET_VIBRATE_STEP_COUNT*MMIWIDGET_VIBRATE_HSTEP)

#define MMIWIDGET_VIBRATE_COUNT         15
#define MMIWIDGET_SLIDE_OBO_STEP        8
#define MMIWIDGET_ROLL_BACK_STEP        20
#define MMIWIDGET_SHORTCUT_ROLLBACK_NUM 3
#define MMIWIDGET_MOVE_MIN_NUM          1
#define MMIWIDGET_LONG_TP_PEN_WIDTH     2 
#define MMIWIDGET_LONG_TP_PEN_RANGE     5
#define MMIWIDGET_MOVE_X_MIN_PIX        7
#define MMIWIDGET_MOVE_Y_MIN_PIX        5
#define MMIWIDGET_INT16_MIN_VALUE      -0x8000
#define MMIWIDGET_INVALID_BAR_POS       0xFF

#define MMIWIDGET_TP_LONG_PRESS_TIMEOUT             300//200
#define MMIWIDGET_BAR_AUTO_SHRINK_TIMEOUT           3000

#ifdef MAINLCD_SIZE_320X480
#define MMIWIDGET_TP_MOVE_PAGE_TIMEOUT              10//60
#else
#define MMIWIDGET_TP_MOVE_PAGE_TIMEOUT              10//50
#endif
#define MMIWIDGET_TP_MOVE_TIMEOUT                   10

#define MMIWIDGET_AUTO_SLIDE_TIMEOUT                1000
#define MMIWIDGET_SLIDE_ANIM_INTERVAl               50

#define MMIWIDGET_MAX_COUNT_LIMIT                   100     //超时计数

#define MMIWIDGET_EDGE_DIST                         30      //widget移到边缘留的空隙

//按比例缩放
#define ZOOM_OFFSET(offset, lcd_width, lcd_height)    (((offset) * (lcd_width) * 2 + (lcd_height)) / ((lcd_height) * 2))

//绝对值宏定义
#define ABS(x) (((x) >= 0) ? (x) : (-(x))) /*lint !e767*/
/*---------------------------------------------------------------------------*/
/*                         Constant Variables                                */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         EXTERNAL DECLARE                                  */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         STATIC DEFINITION                                 */
/*---------------------------------------------------------------------------*/
LOCAL MMIIDLE_TP_TYPE_E s_tp_down_type = MMIIDLE_TP_NONE;

//目前只支持idle界面widget
LOCAL MMIWIDGET_CTRL_T *s_idlewidget_ctrl_ptr = PNULL;

LOCAL CONST GUI_RECT_T s_child_win_rec = {0, 74, 1, 75}; 

//记录当前ticks
LOCAL uint32 s_widget_cur_ticks         = 0;
LOCAL uint8  s_tp_long_press_timer_id   = 0;
LOCAL uint8  s_bar_auto_shrink_timer_id = 0;
LOCAL uint8	 s_tp_move_timer_id         = 0;   //tp move timer.
LOCAL uint8  s_auto_slide_timer_id      = 0;

LOCAL BOOLEAN s_is_restart_idle_screen = TRUE;
LOCAL BOOLEAN s_is_elve_first_start    = FALSE;
LOCAL BOOLEAN s_is_page_moving         = FALSE; 
LOCAL BOOLEAN s_is_auto_slide          = FALSE;
LOCAL BOOLEAN s_is_set_widget_style    = FALSE;

LOCAL GUI_LCD_DEV_INFO  s_widget_application_layer = {0};
LOCAL GUI_RECT_T s_widget_act_child_win_rect[MMIWIDGET_MAX_ITEM_NUM] = {0};

/*---------------------------------------------------------------------------*/
/*                         GLOBAL DEFINITION                                 */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         LOCAL FUNCTION DECLARE                            */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : change pagelayer pos
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
LOCAL void ChangePageLayerPos(
                              MMIWIDGET_CTRL_T *widget_ctrl_ptr,
                              int16 offset_x,
                              int16 offset_y,
                              BOOLEAN is_sync
                              );

/*****************************************************************************/
//  Description : copy pagelayer to mainlayer（将pagelayer的内容同步到mainlayer上）
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
LOCAL void CopyPageLayerToMainLayer(
                                    MMIWIDGET_CTRL_T *widget_ctrl_ptr
                                    );

/*****************************************************************************/
//  Description : reset wallpaper layer
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
LOCAL void ResetWallpaperLayer(
                                MMIWIDGET_CTRL_T *widget_ctrl_ptr
                                );

/*****************************************************************************/
//  Description : reset widget page layer pos
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
LOCAL void ResetWidgetPageLayerPos(
                                MMIWIDGET_CTRL_T *widget_ctrl_ptr
                                );

/*****************************************************************************/
//  Description : set widget layer blt
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
LOCAL void SetBltLayer(
                          MMIWIDGET_CTRL_T *widget_ctrl_ptr
                          );

/*****************************************************************************/
//  Description : destroy wallpaper layer
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
LOCAL void DestroyWallpaperLayer(
                              MMIWIDGET_CTRL_T *widget_ctrl_ptr
                              );

/*****************************************************************************/
//  Description : create wallpaper layer
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void CreateWallpaperLayer(MMIWIDGET_CTRL_T *widget_ctrl_ptr);

/*****************************************************************************/
//  Description : copy wallpapaer to pagelayer
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
LOCAL void CopyWallpaperToPageLayer(void);

#ifdef MMIWIDGET_VIBRATE_TICK_TEST
/*****************************************************************************/
//  Description : output text
//  Global resource dependence : 
//  Author:bin.ji
//  Note:
/*****************************************************************************/
LOCAL void OutputText(uint32 tick_count);
#endif

/*****************************************************************************/
//  Description : create tp move timer
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void CreateTpMoveTimer(MMI_HANDLE_T win_id);

/*****************************************************************************/
//  Description : stop tp move timer
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void StopTpMoveTimer(void);

/*****************************************************************************/
//  Description : restart tp move timer
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void RestartTpMoveTimer(MMI_HANDLE_T win_id);

/*****************************************************************************/
//  Description : is elve widget active
//  Global resource dependence : 
//  Author:bin.ji
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsElveWidgetActive(
                                 MMIWIDGET_CTRL_T *widget_ctrl_ptr
                                 );

/*****************************************************************************/
//  Description : dispatch msg to all child win
//  Global resource dependence : 
//  Author:  bin.ji
//  Note:
/*****************************************************************************/
LOCAL void DispatchMsgToAllChildWin(
                             MMIWIDGET_CTRL_T  *widget_ctrl_ptr,
                             MMI_MESSAGE_ID_E  msg_id
                             );

/*****************************************************************************/
//  Description : reset all widget layer
//  Global resource dependence : 
//  Author:  
//  Note:
/*****************************************************************************/
LOCAL void ResetAllWidgetLayer(
                           MMIWIDGET_CTRL_T *widget_ctrl_ptr
                           );

/*****************************************************************************/
//  Description : rotate rect
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
LOCAL GUI_RECT_T RotateRect(
                            GUI_RECT_T rect,
                            BOOLEAN is_h_to_v    //横屏切竖屏
                            );

/*****************************************************************************/
//  Description : set child win rect
//  Global resource dependence : 
//  Author:bin.ji
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SetChildWinRect(
                              MMI_HANDLE_T win_handle,
                              GUI_RECT_T rect,
                              BOOLEAN is_main_screen_landscape
                              );

/*****************************************************************************/
//  Description : get widget page width
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
LOCAL uint16 GetWidgetPageWidth(
                               MMIWIDGET_CTRL_T *widget_ctrl_ptr
                               );

/*****************************************************************************/
//  Description : get widget ctrl rect
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
LOCAL GUI_RECT_T GetWidgetCtrlRect(
                               MMIWIDGET_CTRL_T *widget_ctrl_ptr
                               );

/*****************************************************************************/
//  Description : rotate point
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
LOCAL GUI_POINT_T RotatePoint(
                            GUI_POINT_T point,
                            BOOLEAN is_h_to_v    //横屏切竖屏
                            );

/*****************************************************************************/
//  Description : switch lcd
//  Global resource dependence : 
//  Author:  
//  Note:
/*****************************************************************************/
LOCAL void SwitchLCD(
                     MMIWIDGET_CTRL_T *widget_ctrl_ptr
                     );

/*****************************************************************************/
//  Description : handle widget tp move
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
LOCAL void ChangeAllChildWinRect(
                                 MMIWIDGET_CTRL_T *widget_ctrl_ptr,
                                 int16 offset_x
                                 );

/*****************************************************************************/
//  Description : set high arm clock
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
LOCAL void SetArmClock(void);

/*****************************************************************************/
//  Description : restore arm clock
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
LOCAL void RestoreArmClock(void);

/*****************************************************************************/
//  Description : handle widget tp move
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsPageLayerPosInRange(
                                    MMIWIDGET_CTRL_T *widget_ctrl_ptr,
                                    int16 offset
                                    );

/*****************************************************************************/
//  Description : handle widget tp move
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
LOCAL int16 GetCurPageLayerOffset(
                                MMIWIDGET_CTRL_T *widget_ctrl_ptr
                                );

/*****************************************************************************/
//  Description : handle widget tp move
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
LOCAL void DisplayPageTitle(
                            MMIWIDGET_CTRL_T *widget_ctrl_ptr
                            );

/*****************************************************************************/
//  Description : redraw top layer
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
LOCAL void RedrawTopLayer(
                           MMIWIDGET_CTRL_T *widget_ctrl_ptr,
                           BOOLEAN is_redraw_bar
                           );

/*****************************************************************************/
//  Description : handle widget tp move
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
LOCAL void DisplaySoftkey(
                         MMIWIDGET_CTRL_T *widget_ctrl_ptr
                         );

/*****************************************************************************/
//  Description : handle widget tp move
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
LOCAL void ShowSlideAnim(
                         MMIWIDGET_CTRL_T *widget_ctrl_ptr,
                         GUI_POINT_T point,
                         BOOLEAN is_slide_by_point
                         );

/*****************************************************************************/
//  Description : handle widget tp move with timer
//  Global resource dependence : 
//  Author:cheng.luo
//  Note: 
/*****************************************************************************/
LOCAL void PlaySlideAnim(
                         MMIWIDGET_CTRL_T *widget_ctrl_ptr,
                         GUI_POINT_T point,
                         BOOLEAN is_slide_by_point
                         );

/*****************************************************************************/
//  Description : 设置某widget应用到widget_layer，page_layer或主层上
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
LOCAL void ResetOneWidgetLayer(
                            MMIWIDGET_CTRL_T *widget_ctrl_ptr,
                            uint32 widget_id
                            );

/*****************************************************************************/
//  Description : create widget layer
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
LOCAL void CreateWidgetPageLayer(
                             MMIWIDGET_CTRL_T *widget_ctrl_ptr
                             );

/*****************************************************************************/
//  Description : handle widget tp move
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN HandleSlidePageTpUp(
                                 MMIWIDGET_CTRL_T *widget_ctrl_ptr,
                                 GUI_POINT_T point
                                 );

/*****************************************************************************/
//  Description : handle widget tp move
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN HandleSlidePageTpMove(
                                 MMIWIDGET_CTRL_T *widget_ctrl_ptr,
                                 GUI_POINT_T point
                                 );

/*****************************************************************************/
//  Description : save tp move point
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN SaveTpMovePoint(
                              MMIWIDGET_CTRL_T *widget_ctrl_ptr,
                              GUI_POINT_T point
                              );

/*****************************************************************************/
//  Description : 重新恢复到主层上
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
LOCAL void ResetHitWidgetLayer(
                               MMIWIDGET_CTRL_T *widget_ctrl_ptr
                               );

/*****************************************************************************/
//  Description : update all screen
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN UpdateAllScreen(void);

/*****************************************************************************/
//  Description : handle widget tp up
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN ChangeWidgetWinRect(
                               MMIWIDGET_CTRL_T *widget_ctrl_ptr,
                               uint32 widget_id,
                               GUI_POINT_T old_point,
                               GUI_POINT_T point,
                               BOOLEAN is_change
                               );

#if 0
/*****************************************************************************/
//  Description : handle widget tp up
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN CheckTotalWidgetNum(
                               MMIWIDGET_CTRL_T *widget_ctrl_ptr
                               );
#endif

/*****************************************************************************/
//  Description : active widget
//  Global resource dependence :
//  Author: bin.ji
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN ActiveWidgetItem(
                            MMIWIDGET_CTRL_T *widget_ctrl_ptr,
                            uint32 widget_id
                            );

/*****************************************************************************/
//  Description : deactive widget
//  Global resource dependence :
//  Author: bin.ji
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN DeactiveWidgetItem(
                              MMIWIDGET_CTRL_T *widget_ctrl_ptr,
                              uint32 widget_id
                              );

/*****************************************************************************/
//  Description : handle widget tp down
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN ResetWidgetTPMoveInfo(
                                 MMIWIDGET_CTRL_T *widget_ctrl_ptr
                                 );

/*****************************************************************************/
//  Description : widget子窗口的消息处理函数
//  Global resource dependence : 
//  Author:  bin.ji
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E RunChildWidgetProc(
                                      MMIWIDGET_CTRL_T  *widget_ctrl_ptr,
                                      uint32            widget_id,
                                      MMI_MESSAGE_ID_E  msg_id,
                                      DPARAM            param
                                      );

/*****************************************************************************/
//  Description : 是否有效偏移（至少有一个item在可视范围内）
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsValidShortcutOffset(
                                 MMIWIDGET_CTRL_T *widget_ctrl_ptr,
                                 int16 shortcut_offset
                                 );

/*****************************************************************************/
//  Description : redraw widget item
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN RedrawWidgetItem(
                               MMIWIDGET_ITEM_T *item_ptr
                               );

/*****************************************************************************/
//  Description : 得到点中item
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN GetWidgetTpPressedItem(
                                 MMIWIDGET_CTRL_T *widget_ctrl_ptr,
                                 GUI_POINT_T point,
                                 uint32 *hit_widget_id_ptr
                                 );

/*****************************************************************************/
//  Description : widget子窗口的消息处理函数
//  Global resource dependence : 
//  Author:  bin.ji
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleChildWidgetWinMsg(
                                MMI_WIN_ID_T      win_id,
                                MMI_MESSAGE_ID_E  msg_id,
                                DPARAM            param
                                );

/*****************************************************************************/
//  Description : 创建widget子窗口
//  Global resource dependence : 
//  Author:bin.ji
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CreateChildWidgetWin(
                                   MMI_WIN_ID_T win_id,
                                   const MMIWIDGET_ITEM_T* add_item_ptr
                                   );

/*****************************************************************************/
//  Description : destroy item data
//  Global resource dependence : 
//  Author:bin.ji
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN DestroyWidgetItem(
                                MMIWIDGET_CTRL_T * widget_ctrl_ptr,  // the list control pointer
                                MMIWIDGET_ITEM_T *item_ptr
                                );

/*****************************************************************************/
//  Description : is valid widget
//  Global resource dependence :
//  Author: bin.ji
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsValidWidgetId(
                              uint32 widget_id
                              );

/*****************************************************************************/
//  Description : is point in shortcut switch region
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsPointInShortcutSwitchRegion(
                               MMIWIDGET_CTRL_T *widget_ctrl_ptr,
                               GUI_POINT_T point
                               );

/*****************************************************************************/
//  Description : is point in shortcut region
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsPointInShortcutRegion(
                               MMIWIDGET_CTRL_T *widget_ctrl_ptr,
                               GUI_POINT_T point
                               );

/*****************************************************************************/
//  Description : remove zorder item
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN RemoveZorderItem(
                            MMIWIDGET_CTRL_T *widget_ctrl_ptr,
                            MMIWIDGET_ITEM_T *item_ptr
                            );

/*****************************************************************************/
//  Description : add item topmost
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN AddZorderItemTopmost(
                          MMIWIDGET_CTRL_T *widget_ctrl_ptr,
                          MMIWIDGET_ITEM_T *item_ptr
                          );

/*****************************************************************************/
//  Description : set item topmost
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
LOCAL void SetZorderItemTopMostByWidgetId(
                          MMIWIDGET_CTRL_T *widget_ctrl_ptr,
                          uint32 widget_id
                          );

/*****************************************************************************/
//  Description : set item topmost
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
LOCAL void SetZorderItemTopMost(
                          MMIWIDGET_CTRL_T *widget_ctrl_ptr,
                          MMIWIDGET_ITEM_T *item_ptr
                          );

/*****************************************************************************/
//  Description : is rect in rect
//  Global resource dependence :
//  Author: bin.ji
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsRectInRect(
                           GUI_RECT_T rect1,
                           GUI_RECT_T rect2
                           );

/*****************************************************************************/
//  Description : insert shortcut
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN InsertShortcut(
                          MMIWIDGET_CTRL_T *widget_ctrl_ptr,
                          MMI_IMAGE_ID_T icon_id,
                          uint32 widget_id,
                          uint32 pos
                          );

/*****************************************************************************/
//  Description : destroy widget layer
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
LOCAL void DestroyWidgetLayer(
                              MMIWIDGET_CTRL_T *widget_ctrl_ptr
                              );

/*****************************************************************************/
//  Description : destroy page layer
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void DestroyWidgetPageLayer(
                              MMIWIDGET_CTRL_T *widget_ctrl_ptr
                              );

/*****************************************************************************/
//  Description : create widget layer
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
LOCAL void CreateWidgetLayer(
                             MMIWIDGET_CTRL_T *widget_ctrl_ptr
                             );

/*****************************************************************************/
//  Description : clear widget rect
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
LOCAL void ClearWidgetRect(
                           MMIWIDGET_CTRL_T *widget_ctrl_ptr,
                           GUI_RECT_T rect
                           );

/*****************************************************************************/
//  Description : redraw hit icon in shortcut bar
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
LOCAL void RedrawHitIcon(
                         MMIWIDGET_CTRL_T *widget_ctrl_ptr,
                         GUI_POINT_T point
                         );

/*****************************************************************************/
//  Description : set widget active
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
LOCAL void SetWidgetActive(
                           MMIWIDGET_CTRL_T *widget_ctrl_ptr,
                           uint32 widget_id,
                           BOOLEAN is_active
                           );

/*****************************************************************************/
//  Description : set widget layer topmost
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN SetClickWidgetTopmost(
                                  MMIWIDGET_CTRL_T *widget_ctrl_ptr
                                  );

/*****************************************************************************/
//  Description : handle widget tp up
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN HandleWidgetTpUp(
                               MMIWIDGET_CTRL_T *widget_ctrl_ptr,
                               DPARAM            param
                               );

/*****************************************************************************/
//  Description : handle widget tp down
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN HandleWidgetTpDown(
                                 MMIWIDGET_CTRL_T *widget_ctrl_ptr,
                                 DPARAM            param
                                 );

/*****************************************************************************/
//  Description : handle widget tp move
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN HandleWidgetTpMove(
                                 MMIWIDGET_CTRL_T *widget_ctrl_ptr,
                                 DPARAM            param
                                 );

/*****************************************************************************/
//  Description : get shortcut item rectm
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
LOCAL GUI_RECT_T GetShortcutItemRect(
                                     MMIWIDGET_CTRL_T *widget_ctrl_ptr,
                                     int16 shortcut_offset,
                                     uint32 index
                                     );

/*****************************************************************************/
//  Description : find shortcut insert pos by point
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN FindShortcutInsertPosByPoint(
                                     MMIWIDGET_CTRL_T *widget_ctrl_ptr,
                                     GUI_POINT_T point,
                                     uint32 *pos_ptr
                                     );

/*****************************************************************************/
//  Description : find shortcut pos by point
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN FindShortcutPosByPoint(
                                     MMIWIDGET_CTRL_T *widget_ctrl_ptr,
                                     GUI_POINT_T point,
                                     uint32 *pos_ptr
                                     );

/*****************************************************************************/
//  Description : remove shortcut item
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN RemoveShortcut(
                          MMIWIDGET_CTRL_T *widget_ctrl_ptr,
                          uint32 pos
                          );

/*****************************************************************************/
//  Description : get win handle by widget id
//  Global resource dependence :
//  Author: bin.ji
//  Note: 
/*****************************************************************************/
LOCAL MMI_HANDLE_T GetWinHandleByWidgetId(
                             MMIWIDGET_CTRL_T *widget_ctrl_ptr,
                             uint32 widget_id
                             );

/*****************************************************************************/
//  Description : get widget id by win handle
//  Global resource dependence :
//  Author: bin.ji
//  Note: 
/*****************************************************************************/
LOCAL uint32 GetWidgetIdByWinHandle(
                             MMIWIDGET_CTRL_T *widget_ctrl_ptr,
                             MMI_HANDLE_T win_handle
                             );

/*****************************************************************************/
//  Description : find widget item
//  Global resource dependence :
//  Author: bin.ji
//  Note: 
/*****************************************************************************/
LOCAL MMIWIDGET_ITEM_T *FindWidgetItem(
                                       MMIWIDGET_CTRL_T *widget_ctrl_ptr,
                                       uint32 widget_id
                                       );

/*****************************************************************************/
//  Description : update widget item child win
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
LOCAL void UpdateAllWidgetItem(
                               MMIWIDGET_CTRL_T *widget_ctrl_ptr
                               );

/*****************************************************************************/
//  Description : display widget shortcut
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
LOCAL void DisplayWidgetShortcut(
                                  MMIWIDGET_CTRL_T           *widget_ctrl_ptr
                                  );

/*****************************************************************************/
//  Description : destroy all item data
//  Global resource dependence : 
//  Author:bin.ji
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN DestroyAllWidgetItemData(
                                    MMIWIDGET_CTRL_T*       widget_ctrl_ptr  // the list control pointer
                                    );

/*****************************************************************************/
//  Description : get the appointed item pointer
//  Global resource dependence : 
//  Author:bin.ji
//  Note:
/*****************************************************************************/
LOCAL MMIWIDGET_ITEM_T* GetItemPtr(
                                 const MMIWIDGET_CTRL_T* widget_ctrl_ptr,
                                 uint32              index  //from 0 to total_item_num-1
                                 );

/*****************************************************************************/
//  Description : handle widget tp msg
//  Global resource dependence : 
//  Author:  bin.ji
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleWidgetTPMsg(
                                     MMIWIDGET_CTRL_T* widget_ctrl_ptr,
                                     MMI_MESSAGE_ID_E  msg_id,
                                     DPARAM            param
                                     );

/*****************************************************************************/
//  Description : roll back widget system when widgets move to up or down most.
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN RollbackWidget(MMIWIDGET_CTRL_T *widget_ctrl_ptr);

/*****************************************************************************/
//  Description : update all widget items status before draw them.
//  Global resource dependence : 
//  Author:  bin.ji
//  Note:
/*****************************************************************************/
LOCAL void UpdateCurWidgetStatus();

/*****************************************************************************/
//  Description : remove shortcut item
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN RemoveShortcutById(
                          MMIWIDGET_CTRL_T *widget_ctrl_ptr,
                          uint32 widget_id
                          );

/*****************************************************************************/
//  Description : remove one widget from the current mmi idle widget system.
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL uint32 GetShortcutItemPos(MMIWIDGET_CTRL_E ctrl_id,
                                       uint32 widget_id
                                       );

/*****************************************************************************/
//  Description : calculate all small icons postions on shortcut bar.
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void CalcAllShortcutItemPos(
                                     MMIWIDGET_CTRL_T *widget_ctrl_ptr,
                                     int16 shortcut_offset
                                     );

/*****************************************************************************/
//  Description : calculate small icons postions on shortcut bar in range.
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void CalcRangeShortcutItemPos(
                                 MMIWIDGET_CTRL_T *widget_ctrl_ptr,
                                 int16 shortcut_offset,
                                 uint32 min_index,
                                 uint32 max_index
                                 );

/*****************************************************************************/
//  Description : switch widget postion on shortcut bar.
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void SwitchWidgetPos(
                          MMIWIDGET_CTRL_T *widget_ctrl_ptr,
                          uint32 pos1,
                          uint32 pos2
                          );

/*****************************************************************************/
//  Description : create shortcut bar layer
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void CreateBarLayer(MMIWIDGET_CTRL_T *widget_ctrl_ptr);

/*****************************************************************************/
//  Description : move widget on idle screen.
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN MoveWidgetBarBtn(MMIWIDGET_CTRL_T *widget_ctrl_ptr, GUI_POINT_T point);

/*****************************************************************************/
//  Description : move widget on idle screen.
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN MoveWidgetBarTpUp(MMIWIDGET_CTRL_T *widget_ctrl_ptr, int16 offsetX, BOOLEAN is_open);

/*****************************************************************************/
//  Description : move widget on idle screen.
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN MoveWidgetBarBtnByPoint(MMIWIDGET_CTRL_T *widget_ctrl_ptr, GUI_POINT_T point);


/*****************************************************************************/
//  Description : update widget item child win
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void UpdatePageDynmicWidgetItem(MMIWIDGET_CTRL_T *widget_ctrl_ptr, uint32 widget_id);

/*****************************************************************************/
//  Description : destroy shortcut bar layer
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void DestroyBarLayer(MMIWIDGET_CTRL_T *widget_ctrl_ptr);

/*****************************************************************************/
//  Description : reset widget status.
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN ResetWidgetState(MMIWIDGET_CTRL_T *widget_ctrl_ptr);

/*****************************************************************************/
//  Description : side all widget items one by one.
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void SlideWidgetOneByOne(MMIWIDGET_CTRL_T *widget_ctrl_ptr);

/*****************************************************************************/
//  Description : vibrate widget item when place it on shortcut bar.
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void VibrateWidgetItem(MMIWIDGET_CTRL_T *widget_ctrl_ptr);  

/*****************************************************************************/
//  Description : handle widget tp long down
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN HandleWidgetTpLongDown(
                                 MMIWIDGET_CTRL_T *widget_ctrl_ptr,
                                 DPARAM                      param
                                 );


/*****************************************************************************/
//  Description : draw rect around child window.
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void DrawSelectedEdgeRect(
                                 MMIWIDGET_CTRL_T *widget_ctrl_ptr,
                                 GUI_RECT_T rect,
                                 GUI_LCD_DEV_INFO *lcd_dev_ptr
                                 );

/*****************************************************************************/
//  Description : check the widget id to add is exist in zorder system or not.
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsExistInZorderSys(MMIWIDGET_CTRL_T *widget_ctrl_ptr, uint32 widget_id);

/*****************************************************************************/
//  Description : check the widget id to add is exist in shorcut or not.
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsExistInShortcutSys(MMIWIDGET_CTRL_T *widget_ctrl_ptr, uint32 widget_id);

/*****************************************************************************/
//  Description : Add the dynamic item to widget
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN AppendDynItem(
                            MMIWIDGET_CTRL_T*       widget_ctrl_ptr,
                            const MMIWIDGET_ITEM_T* add_item_ptr
                            );

/*****************************************************************************/
//  Description : get child window rect.
//  Global resource dependence :
//  Author: bin.ji
//  Note: 
/*****************************************************************************/
LOCAL GUI_RECT_T GetChildWinRect(MMIWIDGET_CTRL_T *widget_ctrl_ptr, uint32 widget_id);

/*****************************************************************************/
//  Description : create desktop elve layer
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void CreateDeskElveLayer(MMIWIDGET_CTRL_T *widget_ctrl_ptr);

/*****************************************************************************/
//  Description : create softkey layer
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void CreateSoftkeyLayer(MMIWIDGET_CTRL_T *widget_ctrl_ptr);

/*****************************************************************************/
//  Description : create softkey layer
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void CreateStbLayer(MMIWIDGET_CTRL_T *widget_ctrl_ptr);

/*****************************************************************************/
//  Description : create softkey layer
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void CreatePageTitleLayer(MMIWIDGET_CTRL_T *widget_ctrl_ptr);

/*****************************************************************************/
//  Description : destroy desktop elve layer
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void DestroySoftkeyLayer(
                              MMIWIDGET_CTRL_T *widget_ctrl_ptr
                              );

/*****************************************************************************/
//  Description : destroy desktop elve layer
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void DestroyStbLayer(
                              MMIWIDGET_CTRL_T *widget_ctrl_ptr
                              );


/*****************************************************************************/
//  Description : destroy desktop elve layer
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void DestroyPageTitleLayer(
                              MMIWIDGET_CTRL_T *widget_ctrl_ptr
                              );


/*****************************************************************************/
//  Description : destroy desktop elve layer
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void DestroyElveLayer(
                              MMIWIDGET_CTRL_T *widget_ctrl_ptr
                              );

/*****************************************************************************/
//  Description : start elve animation.
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void StartElveAnim(MMIWIDGET_CTRL_T *widget_ctrl_ptr, BOOLEAN is_to_start);

/*****************************************************************************/
//  Description : set elve current status.
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void SetElveStatus(MMIWIDGET_CTRL_T *widget_ctrl_ptr, MMIWIDGET_ELVE_DISPLAY_E type);

/*****************************************************************************/
//  Description : stop elve animation.
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void StopElveAnim(MMIWIDGET_CTRL_T *widget_ctrl_ptr);

/*****************************************************************************/
//  Description : is elve widget or not.
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsElveWidget(uint32 widget_id);

/*****************************************************************************/
//  Description : force to draw elve.
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void DrawElve(MMIWIDGET_CTRL_T *widget_ctrl_ptr);

/*****************************************************************************/
//  Description : get tp area
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL MMIWIDGET_TP_AREA_E GetTpArea(MMIWIDGET_CTRL_T *widget_ctrl_ptr, GUI_POINT_T point);

/*****************************************************************************/
//  Description : tp position for moving along X direction is valid or not.
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsTpXMoveValid(MMIWIDGET_CTRL_T *widget_ctrl_ptr, GUI_POINT_T point);

/*****************************************************************************/
//  Description : tp position for moving along Y direction is valid or not.
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsTpYMoveValid(MMIWIDGET_CTRL_T *widget_ctrl_ptr, GUI_POINT_T point);

/*****************************************************************************/
//  Description : shirink widget bar.
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN ShrinkWidgetBar(MMIWIDGET_CTRL_T *widget_ctrl_ptr);

/*****************************************************************************/
//  Description : backup all active child window rect.
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void BakAllActiveChildWinRect(MMIWIDGET_CTRL_T *widget_ctrl_ptr);

/*****************************************************************************/
//  Description : restore all active child window rect.
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void RestoreAllActiveChildWinRect(MMIWIDGET_CTRL_T *widget_ctrl_ptr);

/*****************************************************************************/
//  Description : print all widget info, just for log trace.
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void PrintWidgetTraceInfo(MMIWIDGET_CTRL_T *widget_ctrl_ptr);

/*****************************************************************************/
//  Description : is the item the top item in zorder.
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsTopItemInZorder(MMIWIDGET_CTRL_T *widget_ctrl_ptr, MMIWIDGET_ITEM_T *item_ptr);

/*****************************************************************************/
//  Description : start mp3 progress bar timer.
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void StartMp3PrgbarTimer(MMIWIDGET_CTRL_T *widget_ctrl_ptr);

/*****************************************************************************/
//  Description : stop mp3 progress bar timer.
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void StopMp3PrgbarTimer(MMIWIDGET_CTRL_T *widget_ctrl_ptr);

/*****************************************************************************/
//  Description : reset widget tp state.
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void ResetWidgetTpState(MMIWIDGET_CTRL_T *widget_ctrl_ptr);

/*****************************************************************************/
//  Description : reset widget bar status.
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN ResetWidgetBarState(MMIWIDGET_CTRL_T *widget_ctrl_ptr);


/*****************************************************************************/
//  Description : active widget when loading all widget
//  Global resource dependence :
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN InitActiveWidgetItem(
                            MMIWIDGET_CTRL_T *widget_ctrl_ptr,
                            uint32 widget_id
                            );

/*****************************************************************************/
//  Description : is widget active
//  Global resource dependence : 
//  Author:arvin.wu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsWidgetActive(
                           MMIWIDGET_CTRL_T *widget_ctrl_ptr,
                           uint32 widget_id
                           );

/*****************************************************************************/
//  Description : try to reset moving child win postion.
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN ResetChildWinPosition(MMIWIDGET_CTRL_T *widget_ctrl_ptr, GUI_POINT_T point);

/*****************************************************************************/
//  Description : move widget from idle to shortcut bar by specified pos.
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN MoveWidgetIdle2BarByPos(MMIWIDGET_CTRL_T *widget_ctrl_ptr, uint32 pos);

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : print all widget info, just for log trace.
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void PrintWidgetTraceInfo(MMIWIDGET_CTRL_T *widget_ctrl_ptr)
{
    uint32 i = 0;
    GUI_RECT_T rect = {0};
    MMIWIDGET_ITEM_T *item_ptr = PNULL;
    
    //total num....
    SCI_TRACE_LOW("////////////////////////WIDGET TOTAL ITEM START//////////////////");
    //SCI_TRACE_LOW:"total_num = %d\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_1201_112_2_18_3_7_33_73,(uint8*)"d", widget_ctrl_ptr->total_item_num);
    SCI_TRACE_LOW("////////////////////////WIDGET BAR ITEM END//////////////////");

    //widget bar num .....
    SCI_TRACE_LOW("////////////////////////WIDGET BAR ITEM START//////////////////");
    for (i = 0; i < widget_ctrl_ptr->shortcut.shortcut_num; i++)
    {
        //SCI_TRACE_LOW:"shortcut[%d] = %d\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_1206_112_2_18_3_7_33_74,(uint8*)"dd", i, widget_ctrl_ptr->shortcut.shortcut_item[i].widget_id);
    }
    SCI_TRACE_LOW("////////////////////////WIDGET BAR ITEM END//////////////////");

    //widget active item .....
    item_ptr = widget_ctrl_ptr->zorder_item_ptr->zorder_next_ptr;

    SCI_TRACE_LOW("////////////////////////ACTIVE WIDGET ITEM START//////////////////");
    i = 0;
    
    while (PNULL != item_ptr)
    {       
        if (item_ptr->is_active)
        {            
            MMK_GetWinRect(item_ptr->item_info.win_id, &rect);
            //SCI_TRACE_LOW:"active item [%d] = %d, rect = {%d, %d, %d, %d}"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_1219_112_2_18_3_7_33_75,(uint8*)"dddddd", i, item_ptr->item_info.widget_id, rect.left, rect.top, rect.right, rect.bottom);
            i++;
        }
        
        item_ptr = item_ptr->zorder_next_ptr;
    }  
    SCI_TRACE_LOW("////////////////////////ACTIVE WIDGET ITEM END//////////////////");
}

/*****************************************************************************/
//  Description : create tp long press timer
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void CreateAutoShrinkTimer(MMI_HANDLE_T win_id)
{
    s_bar_auto_shrink_timer_id = MMK_CreateWinTimer(win_id, MMIWIDGET_BAR_AUTO_SHRINK_TIMEOUT, FALSE);
}

/*****************************************************************************/
//  Description : stop tp long press timer
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void StopAutoShrinkTimer(void)
{
    if (0 != s_bar_auto_shrink_timer_id)
    {
        MMK_StopTimer(s_bar_auto_shrink_timer_id);
        s_bar_auto_shrink_timer_id = 0;
    }
}

/*****************************************************************************/
//  Description : create tp long press timer
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void RestartAutoShrinkTimer(MMI_HANDLE_T win_id)
{
    StopAutoShrinkTimer();
    CreateAutoShrinkTimer(win_id);
}

/*****************************************************************************/
//  Description : create tp long press timer
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void CreateTpLongPressTimer(MMI_HANDLE_T win_id, uint32 time_out)
{
    s_tp_long_press_timer_id = MMK_CreateWinTimer(win_id, time_out, FALSE);
}

/*****************************************************************************/
//  Description : stop tp long press timer
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void StopTpLongPressTimer(void)
{
    if (0 != s_tp_long_press_timer_id)
    {
        MMK_StopTimer(s_tp_long_press_timer_id);
        s_tp_long_press_timer_id = 0;
    }
}

/*****************************************************************************/
//  Description : create tp long press timer
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void RestartTpLongPressTimer(MMI_HANDLE_T win_id, uint32 time_out)
{
    StopTpLongPressTimer();
    CreateTpLongPressTimer(win_id, time_out);
}

/*****************************************************************************/
//  Description : create tp move timer
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void CreateTpMoveTimer(MMI_HANDLE_T win_id)
{
	uint32  time_out = MMIWIDGET_TP_MOVE_PAGE_TIMEOUT;
    MMIWIDGET_CTRL_T *widget_ctrl_ptr = MMIAPIWIDGET_GetWidgetPtr(MMIWIDGET_CTRL_IDLE_ID);
	
    if (PNULL == widget_ctrl_ptr)
    {
        return;
    }
	
    if (MMIWIDGET_TP_MOVE_IDLE_TO_IDLE == widget_ctrl_ptr->widget_mov_type)
    {
        time_out = MMIWIDGET_TP_MOVE_TIMEOUT;
    }
	
	s_tp_move_timer_id = MMK_CreateWinTimer(win_id, time_out, FALSE);
}

/*****************************************************************************/
//  Description : stop tp move timer
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void StopTpMoveTimer(void)
{
    if (0 != s_tp_move_timer_id)
    {
        MMK_StopTimer(s_tp_move_timer_id);
        s_tp_move_timer_id = 0;
    }
}

/*****************************************************************************/
//  Description : restart tp move timer
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void RestartTpMoveTimer(MMI_HANDLE_T win_id)
{
    StopTpMoveTimer();
    CreateTpMoveTimer(win_id);
}

/*****************************************************************************/
//  Description : create auto slide timer
//  Global resource dependence : s_auto_slide_timer_id
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void CreateAutoSlideTimer(MMI_HANDLE_T win_id, uint32 time_out)
{
    s_auto_slide_timer_id = MMK_CreateWinTimer(win_id, time_out, FALSE);
}

/*****************************************************************************/
//  Description : stop auto slide timer
//  Global resource dependence : s_auto_slide_timer_id
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void StopAutoSlideTimer(void)
{
    if (0 != s_auto_slide_timer_id)
    {
        MMK_StopTimer(s_auto_slide_timer_id);
        s_auto_slide_timer_id = 0;
    }
}

/*****************************************************************************/
//  Description : restart auto slide timer
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void RestartAutoSlideTimer(MMI_HANDLE_T win_id, uint32 time_out)
{
    StopAutoSlideTimer();
    CreateAutoSlideTimer(win_id, time_out);
}

/*****************************************************************************/
//  Description : handle widget tp msg
//  Global resource dependence : 
//  Author:  bin.ji
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleWidgetTPMsg(
                                     MMIWIDGET_CTRL_T* widget_ctrl_ptr,
                                     MMI_MESSAGE_ID_E  msg_id,
                                     DPARAM            param
                                     )
{
    GUI_POINT_T        tp_point = {0};
    MMI_RESULT_E       result = MMI_RESULT_TRUE;
    
    if (PNULL == widget_ctrl_ptr)
    {
        return MMI_RESULT_FALSE;
    }
    MMI_CheckAllocatedMemInfo();
    switch (msg_id)
    {
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_DOWN:       
        StopMp3PrgbarTimer(widget_ctrl_ptr);

	    tp_point.x = MMK_GET_TP_X(param);
        tp_point.y = MMK_GET_TP_Y(param);
        s_tp_down_type = MMIAPIIDLE_GetWidgetTpType(tp_point);            
  
        if (   MMIIDLE_TP_LEFT_SK == s_tp_down_type
            || MMIIDLE_TP_RIGHT_SK == s_tp_down_type
            || MMIIDLE_TP_MIDDLE_SK == s_tp_down_type)
        {
            break;
        }

        CreateTpLongPressTimer(widget_ctrl_ptr->win_id, MMIWIDGET_TP_LONG_PRESS_TIMEOUT);
        CreateTpMoveTimer(widget_ctrl_ptr->win_id);
        CreateAutoSlideTimer(widget_ctrl_ptr->win_id, MMIWIDGET_AUTO_SLIDE_TIMEOUT);

        //变频
        SetArmClock();      
        if (!HandleWidgetTpDown(widget_ctrl_ptr, param))
        {
            result = MMI_RESULT_FALSE;
        }        
        
        //backup all active child window rect.
        BakAllActiveChildWinRect(widget_ctrl_ptr);        
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_UP:        
        if (MMIIDLE_TP_LEFT_SK == s_tp_down_type)
        {
#ifdef MMI_IDLE_NO_SIM_SOS_SUPPORT
            if (0 == MMIAPIPHONE_GetSimAvailableNum(PNULL, 0))
            {
                uint16 sim_sys[MMI_DUAL_SYS_MAX] = {0};
                uint32 sim_num = 0;
                const char* s_emergency_call = "112";

                sim_num = MMIAPIPHONE_GetPSReadyNum(sim_sys, MMI_DUAL_SYS_MAX);

                MMIAPICC_MakeCall((MN_DUAL_SYS_E)sim_sys[0],
                                                (uint8 *)s_emergency_call,
                                                strlen(s_emergency_call),
                                                PNULL,
                                                CC_CALL_SIM_MAX,CC_CALL_NORMAL_CALL,
                                                PNULL);
            }
            else
#endif
            {
                MMIAPISET_OpenFuncKeyWin(widget_ctrl_ptr->win_id,MMISET_FUNCKEY_LEFT);
            }

            s_tp_down_type =  MMIIDLE_TP_NONE;
            break;
        }
        else
        if (MMIIDLE_TP_RIGHT_SK == s_tp_down_type) 
        {
            MMIAPISET_OpenFuncKeyWin(widget_ctrl_ptr->win_id,MMISET_FUNCKEY_RIGHT);
            s_tp_down_type =  MMIIDLE_TP_NONE;
            break;
        }
        else
        if (MMIIDLE_TP_MIDDLE_SK == s_tp_down_type)
        {
            MMIAPIMENU_CreatMainMenu();
            s_tp_down_type =  MMIIDLE_TP_NONE;
            break;
        }

        StopTpLongPressTimer();
        StopTpMoveTimer();
        StopAutoSlideTimer();
        
        if (HandleWidgetTpUp(widget_ctrl_ptr, param))
        {
            ResetWidgetTPMoveInfo(widget_ctrl_ptr);
        }
        else
        {
            result = MMI_RESULT_FALSE;
        }

        RestoreArmClock(); 
        StartMp3PrgbarTimer(widget_ctrl_ptr);               
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

    default:
        result = MMI_RESULT_FALSE;
        break;
    }
	MMI_CheckAllocatedMemInfo();
    return result;
}

/*****************************************************************************/
//  Description : widget子窗口的消息处理函数
//  Global resource dependence : 
//  Author:  bin.ji
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E RunChildWidgetProc(
                                      MMIWIDGET_CTRL_T  *widget_ctrl_ptr,
                                      uint32            widget_id,
                                      MMI_MESSAGE_ID_E  msg_id,
                                      DPARAM            param
                                      )
{
    MMIWIDGET_ITEM_T  *item_ptr = PNULL;
    MMI_RESULT_E result = MMI_RESULT_FALSE;

    if (PNULL == widget_ctrl_ptr)
    {
        //SCI_TRACE_LOW:"RunChildWidgetProc() PNULL == widget_ctrl_ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_1500_112_2_18_3_7_34_76,(uint8*)"");
        return result;
    }
    item_ptr = FindWidgetItem(widget_ctrl_ptr, widget_id);
    //MSG_OPEN_WINDOW,MSG_CLOSE_WINDOW消息漏下去，其他的消息都是在widget item处于ACTIVE状态处理
    if ((PNULL != item_ptr) && (PNULL != item_ptr->item_info.HandleWidgetWinMsg) && ((MSG_CLOSE_WINDOW == msg_id) ||(MSG_OPEN_WINDOW == msg_id)) )
    {
        result = item_ptr->item_info.HandleWidgetWinMsg(item_ptr->item_info.win_id, msg_id, param);
    }
    else if((PNULL != item_ptr) && (PNULL != item_ptr->item_info.HandleWidgetWinMsg) && item_ptr->is_active)
    {
        result = item_ptr->item_info.HandleWidgetWinMsg(item_ptr->item_info.win_id, msg_id, param);    
    }

    return result;
}

/*****************************************************************************/
// Description : check and handle status bar tp event.
// Global resource dependence : 
// Author: 
// Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleStatusBarTpDown(
                                    MMI_HANDLE_T win_handle,
                                    MMI_MESSAGE_ID_E       msg_id,
                                    DPARAM                 param
                                    )
{
   uint32                   item_index     = 0;
   uint32                   item_total_num = 0;
   GUISTBDATA_ITEM_DATA_T   *item_data_ptr = PNULL;
   GUI_POINT_T             tp_point = {0};
   MMIWIDGET_CTRL_T        *widget_ctrl_ptr = PNULL;
   MMI_HANDLE_T             stbctrl_handle = 0;
   GUI_RECT_T               ctrl_rect = {0};

   stbctrl_handle = MMK_GetWinStatusBarHandle(win_handle);
   if (0 == stbctrl_handle)
   {
       return FALSE;
   }

    widget_ctrl_ptr = MMIAPIWIDGET_GetWidgetPtr(MMIWIDGET_CTRL_IDLE_ID);
    
    if (PNULL == widget_ctrl_ptr)
    {
        return FALSE;
    }

  tp_point.x = MMK_GET_TP_X(param);
  tp_point.y = MMK_GET_TP_Y(param);

   item_total_num = GUIWIN_GetStbItemTotalNum();

   GUICTRL_GetRect(MMK_GetCtrlPtr(stbctrl_handle), &ctrl_rect);
   //响应TP UP消息
   if(GUI_PointIsInRect(tp_point, ctrl_rect)) // 是否支持点击事件
   {
       for(item_index = 0; item_index < item_total_num; item_index++)
       {
           item_data_ptr = GUISTBDATA_GetItemData(item_index);

           if (PNULL != item_data_ptr
               && GUI_PointIsInRect(tp_point, item_data_ptr->item_rect)) // 点击位置是否在具体的item区域
           {
               // 点是否在某一项的区域内，执行条件为1、item可见；2、item有效；3、item允许执行应用回调；4、回调函数地址不为空
               if (GUIWIN_IsStbItemVisible(item_index))  
               {
                   return TRUE;
               }               
           }
       }
   }

   return  FALSE;
}

/*****************************************************************************/
// Description : check and handle status bar tp event.
// Global resource dependence : 
// Author: 
// Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleStatusBarTpUp(
                                 MMI_HANDLE_T win_handle,
                                 MMI_MESSAGE_ID_E       msg_id,
                                 DPARAM                 param
                                 )
{
   uint32                   item_index     = 0;
   uint32                   item_total_num = 0;
   GUISTBDATA_ITEM_DATA_T   *item_data_ptr = PNULL;
   GUI_POINT_T             tp_point = {0};
   MMIWIDGET_CTRL_T        *widget_ctrl_ptr = PNULL;
   MMI_HANDLE_T             ctrl_handle = 0;
   GUI_RECT_T               ctrl_rect = {0};

   ctrl_handle = MMK_GetWinStatusBarHandle(win_handle);

   if (PNULL == ctrl_handle)
   {
       return FALSE;
   }

    widget_ctrl_ptr = MMIAPIWIDGET_GetWidgetPtr(MMIWIDGET_CTRL_IDLE_ID);
    
    if (PNULL == widget_ctrl_ptr)
    {
        return FALSE;
    }

  tp_point.x = MMK_GET_TP_X(param);
  tp_point.y = MMK_GET_TP_Y(param);

   item_total_num = GUIWIN_GetStbItemTotalNum();

   GUICTRL_GetRect(MMK_GetCtrlPtr(ctrl_handle), &ctrl_rect);
   //响应TP UP消息
   if(GUI_PointIsInRect(tp_point, ctrl_rect)) // 是否支持点击事件
   {
       for(item_index = 0; item_index < item_total_num; item_index++)
       {
           item_data_ptr = GUISTBDATA_GetItemData(item_index);

           if (PNULL != item_data_ptr
               && GUI_PointIsInRect(tp_point, item_data_ptr->item_rect)) // 点击位置是否在具体的item区域
           {
               // 点是否在某一项的区域内，执行条件为1、item可见；2、item有效；3、item允许执行应用回调；4、回调函数地址不为空
               if (GUIWIN_IsStbItemVisible(item_index)
                   && !GUIWIN_IsStbItemDisable(item_index)
                   && GUIWIN_StbItemIsHandleFuncValid(item_index)
                   && PNULL != item_data_ptr->app_handle_func)  
               {
                   item_data_ptr->app_handle_func(); // 执行app回调函数
                   return TRUE;
               }
           }
       }
       HandleWidgetTPMsg(widget_ctrl_ptr, msg_id, param);
   }

   return  FALSE;
}

/*****************************************************************************/
//  Description : widget子窗口的消息处理函数
//  Global resource dependence : 
//  Author:  bin.ji
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleChildWidgetWinMsg(
                                           MMI_WIN_ID_T      win_id,
                                           MMI_MESSAGE_ID_E  msg_id,
                                           DPARAM            param
                                           )
{
    MMI_RESULT_E			result = MMI_RESULT_TRUE;
	MMIWIDGET_CTRL_T        *widget_ctrl_ptr = PNULL;
    uint32                  widget_id = MMIWIDGET_INVALID_ID;
    GUI_POINT_T             tp_point = {0};
    MMI_HANDLE_T             ctrl_handle = 0;
    GUI_RECT_T               ctrl_rect = {0};
MMI_CheckAllocatedMemInfo();
    widget_ctrl_ptr = MMIAPIWIDGET_GetWidgetPtr(MMIWIDGET_CTRL_IDLE_ID);
    if (PNULL == widget_ctrl_ptr)
    {
        return result;
    }

    widget_id = GetWidgetIdByWinHandle(widget_ctrl_ptr, win_id);
    if (PNULL == FindWidgetItem(widget_ctrl_ptr, widget_id))
    {
        return result;
    }

    ctrl_handle = MMK_GetWinStatusBarHandle(win_id);

	switch (msg_id)
	{
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_LONG:
    case MSG_TP_PRESS_DOWN:
        StopAutoShrinkTimer();
//        MMIAPIIDLESS_StopTimer();

        tp_point.x = MMK_GET_TP_X(param);
        tp_point.y = MMK_GET_TP_Y(param);

        if (!HandleStatusBarTpDown(win_id, msg_id, param))
        {
            HandleWidgetTPMsg(widget_ctrl_ptr, msg_id, param);
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
     case MSG_TP_PRESS_MOVE:
        HandleWidgetTPMsg(widget_ctrl_ptr, msg_id, param);
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_UP:
        RestartAutoShrinkTimer(widget_ctrl_ptr->win_id);
        //MMIDLE_RestartSsTimer(widget_ctrl_ptr->win_id);

        tp_point.x = MMK_GET_TP_X(param);
        tp_point.y = MMK_GET_TP_Y(param);

        GUICTRL_GetRect(MMK_GetCtrlPtr(ctrl_handle), &ctrl_rect);
                
        if((MMIWIDGET_TP_MOVE_IDLE_TO_IDLE != widget_ctrl_ptr->widget_mov_type) && GUI_PointIsInRect(tp_point, ctrl_rect))
        {
            HandleStatusBarTpUp(win_id, msg_id, param); 
        }
        else
        {
            HandleWidgetTPMsg(widget_ctrl_ptr, msg_id, param);
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

    //case MSG_LOSE_FOCUS:
    //case MSG_GET_FOCUS:
    //   break;
        
	default:
		result = RunChildWidgetProc(widget_ctrl_ptr, widget_id, msg_id, param);
		break;
	}
MMI_CheckAllocatedMemInfo();
	return(result);
}

/*****************************************************************************/
//  Description : update all widget items status before draw them.
//  Global resource dependence : 
//  Author:  bin.ji
//  Note:
/*****************************************************************************/
LOCAL void UpdateCurWidgetStatus()
{
    uint16 i = 0;
    MMIWIDGET_ITEM_INFO_T *item_info = PNULL;
    MMIWIDGET_CTRL_T *widget_ctrl_ptr = PNULL;
    MMIWIDGET_ITEM_T *item_ptr = PNULL;
   
    widget_ctrl_ptr = MMIAPIWIDGET_GetWidgetPtr(MMIWIDGET_CTRL_IDLE_ID);
    if (PNULL == widget_ctrl_ptr)
    {
        return;
    }
    
    for (i = 0; i < widget_ctrl_ptr->shortcut.shortcut_num; i++)
    {
         item_info = MMIWIDGET_FindWidgetItemInfo(widget_ctrl_ptr->shortcut.shortcut_item[i].widget_id);
         
         if (  (PNULL != item_info) 
 			&& (item_info->is_dynamic) 
 			&& ((PNULL != item_info->DisplayWidgetCondition)
 			&& (MMI_RESULT_FALSE == item_info->DisplayWidgetCondition())))
         {
             MMIAPIWIDGET_RemoveDynamicItem(MMIWIDGET_CTRL_IDLE_ID, item_info->widget_id);
         }        
    }
    
    if (PNULL != widget_ctrl_ptr->zorder_item_ptr)
    {
        item_ptr = widget_ctrl_ptr->zorder_item_ptr->zorder_next_ptr;  
        
        while ((PNULL != item_ptr) && item_ptr->is_active)
        {
            item_info = &item_ptr->item_info;
            
            if (   (item_info->is_dynamic) 
    			&& (PNULL != item_info->DisplayWidgetCondition)
    			&& (MMI_RESULT_FALSE == item_info->DisplayWidgetCondition()))
            {
                MMIAPIWIDGET_RemoveDynamicItem(MMIWIDGET_CTRL_IDLE_ID, item_info->widget_id);

                if (PNULL == widget_ctrl_ptr->zorder_item_ptr->zorder_next_ptr)                
                {
                    //zorder list is null after remove one widget.
                    break;
                }
            }

            item_ptr = item_ptr->zorder_next_ptr;
        }
    }  
}

/*****************************************************************************/
//  Description : get current tp status is moveing page or not.
//  Global resource dependence : 
//  Author:  
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIWIDGET_IsTpMovingPage(void)
{
    return s_is_page_moving;
}

/*****************************************************************************/
//  Description : handle widget msg
//  Global resource dependence : 
//  Author:  bin.ji
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIAPIWIDGET_HandleMsg(
                                MMI_WIN_ID_T      win_id,
                                MMI_MESSAGE_ID_E  msg_id,
                                DPARAM            param
                                )
{
    MMI_RESULT_E			     result = MMI_RESULT_TRUE;
	MMIWIDGET_CTRL_T            *widget_ctrl_ptr = PNULL;
	GUI_POINT_T				     point = {0, 0};
    MMI_MULTI_KEY_TP_MSG_PARAM_T tp_point = {0};
    MMIWIDGET_ITEM_T            *item_ptr = PNULL;
    IGUICTRL_T                  *stb_ctr_ptr  = PNULL;

    if (MMIWIDGET_IsIdleSetWidgetStyle())
    {
        MMIAPIWIDGET_SetIdleWidgetStyle(FALSE);
        MMIAPIWIDGET_CreateIdleWidget();
    }

    widget_ctrl_ptr = MMIAPIWIDGET_GetWidgetPtr(MMIWIDGET_CTRL_IDLE_ID);
    if (PNULL == widget_ctrl_ptr)
    {
        return result;
    }

	switch (msg_id)
	{
        case MSG_APP_WEB:
            MMIAPIMENU_CreatMainMenu();       
            break;

        case MSG_KEYREPEAT_UP:
        case MSG_APP_UP:
            MMIAPISET_OpenDirectionKeyWin(win_id,MMISET_DIRKEY_UP);
            break;

        case MSG_KEYREPEAT_DOWN:
        case MSG_APP_DOWN:
            MMIAPISET_OpenDirectionKeyWin(win_id,MMISET_DIRKEY_DOWN);
            break;

        case MSG_KEYREPEAT_LEFT:
        case MSG_APP_LEFT:
            MMIAPISET_OpenDirectionKeyWin(win_id,MMISET_DIRKEY_LEFT);
            break;

        case MSG_KEYREPEAT_RIGHT:
        case MSG_APP_RIGHT:
            MMIAPISET_OpenDirectionKeyWin(win_id,MMISET_DIRKEY_RIGHT);
            break;

    	case MSG_FULL_PAINT:
            RedrawTopLayer(widget_ctrl_ptr, TRUE);
       	    if (widget_ctrl_ptr->tp_move_is_valid)
       	    {
       	        break;
       	    }
       	    
    	    if (s_is_elve_first_start)
    	    {
    	        SetElveStatus(widget_ctrl_ptr, MMIWIDGET_ELVE_NORMAL_DISPLAY);
                StartElveAnim(widget_ctrl_ptr, TRUE);
                s_is_elve_first_start = FALSE;
    	    }

            //change arm frequece from normal level to multimedia level
	        SetArmClock();  

            //必须在wallpaper动画控件显示之后！
            //CopyWallpaperToPageLayer();
            UILAYER_Clear( &widget_ctrl_ptr->page_layer);
            if (1 == widget_ctrl_ptr->cur_page_index)
            {
                //display idle text
                MAIN_DrawStkIdleModeText(widget_ctrl_ptr->page_layer.block_id);
            }

            SetBltLayer(widget_ctrl_ptr);
            UpdateCurWidgetStatus();
            UpdateAllWidgetItem(widget_ctrl_ptr);   
            //CopyPageLayerToMainLayer(widget_ctrl_ptr);
            //restore arm frequence to normal level.
            RestoreArmClock();
            break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_TP_PRESS_LONG:
        case MSG_TP_PRESS_DOWN:
            StopAutoShrinkTimer();
//            MMIAPIIDLESS_StopTimer();
            HandleWidgetTPMsg(widget_ctrl_ptr, msg_id, param);
            break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_TP_PRESS_MOVE:
            HandleWidgetTPMsg(widget_ctrl_ptr, msg_id, param);          
            break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
            
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_TP_PRESS_UP:
            RestartAutoShrinkTimer(widget_ctrl_ptr->win_id);
            //MMIDLE_RestartSsTimer(win_id);
            HandleWidgetTPMsg(widget_ctrl_ptr, msg_id, param);
            break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    
        case MSG_BACKLIGHT_TURN_OFF:
            SetBltLayer(widget_ctrl_ptr);
            break;
    
        case MSG_LOSE_FOCUS:
            //restore main layer.
            UILAYER_RestoreMainLayer();
//            MMIAPIENG_RestoreScreenLogLayer();
        
            stb_ctr_ptr = (IGUICTRL_T*)MMK_GetCtrlPtrByWin(widget_ctrl_ptr->win_id, MMITHEME_GetStatusBarCtrlId());
            GUICTRL_SetLcdDevInfo(stb_ctr_ptr, MMITHEME_GetDefaultLcdDev());            
            
            item_ptr = FindWidgetItem(widget_ctrl_ptr, widget_ctrl_ptr->tp_move_info.click_widget_id);

            if (PNULL != item_ptr && PNULL != item_ptr->item_info.ResetChildWinItem)
            {
               item_ptr->item_info.ResetChildWinItem();
            }

            widget_ctrl_ptr->shortcut_offset = 0;
            ResetChildWinPosition(widget_ctrl_ptr, widget_ctrl_ptr->save_point);
            ResetWidgetPageLayerPos(widget_ctrl_ptr);
            ResetWidgetState(widget_ctrl_ptr);
            MMIAPIWIDGET_SetWallpaperLcdDevInfo((GUI_LCD_DEV_INFO *)MMITHEME_GetDefaultLcdDev());               
            //protection: reset layer to avoid bug.
            StopElveAnim(widget_ctrl_ptr);
            StopAutoShrinkTimer();
            DispatchMsgToAllChildWin(widget_ctrl_ptr, MSG_LOSE_FOCUS);

            if (s_is_page_moving)
            {
                RestoreAllActiveChildWinRect(widget_ctrl_ptr);
            }

            //restore arm frequence to normal level.
            RestoreArmClock();
			UILAYER_SetDirectDraw(FALSE);
            break;

        case MSG_BACKLIGHT_TURN_ON:         
            SetBltLayer(widget_ctrl_ptr);
            break;

        case MSG_GET_FOCUS:            
            //log trace: print all widget info.
            PrintWidgetTraceInfo(widget_ctrl_ptr);

            //remove main layer.
            UILAYER_RemoveMainLayer();
//            MMIAPIENG_ChangeScreenLogLayer();

            //set status bar dev as stb layer.
            stb_ctr_ptr = (IGUICTRL_T*)MMK_GetCtrlPtrByWin(widget_ctrl_ptr->win_id, MMITHEME_GetStatusBarCtrlId());
            GUICTRL_SetLcdDevInfo(stb_ctr_ptr, &widget_ctrl_ptr->stb_layer);
            MAIN_SetStbTimeVisible(TRUE);
            if (widget_ctrl_ptr->shortcut.is_shortcut_on)
            {
                s_is_restart_idle_screen = FALSE;
                RestartAutoShrinkTimer(widget_ctrl_ptr->win_id);
            }
            else
            {
                s_is_restart_idle_screen = TRUE;
            }         
            ResetWidgetPageLayerPos(widget_ctrl_ptr);
            ResetWidgetState(widget_ctrl_ptr);            
            ResetWidgetBarState(widget_ctrl_ptr);
            StartElveAnim(widget_ctrl_ptr, TRUE);
            SetElveStatus(widget_ctrl_ptr, MMIWIDGET_ELVE_NORMAL_DISPLAY);
            
            DispatchMsgToAllChildWin(widget_ctrl_ptr, MSG_GET_FOCUS);
            
            if (IsElveWidgetActive(widget_ctrl_ptr))
            {
                SetZorderItemTopMostByWidgetId(widget_ctrl_ptr, MMIWIDGET_ELVE_ID);
            } 
            break;

        case MSG_OPEN_WINDOW: 
            if (MMK_IsFocusWin(widget_ctrl_ptr->win_id))
            {
                //log trace: print all widget info.
                PrintWidgetTraceInfo(widget_ctrl_ptr);
    
                //remove main layer.
                UILAYER_RemoveMainLayer();
//                MMIAPIENG_ChangeScreenLogLayer();
    
                //set status bar to stb layer.
                stb_ctr_ptr = (IGUICTRL_T*)MMK_GetCtrlPtrByWin(widget_ctrl_ptr->win_id, MMITHEME_GetStatusBarCtrlId());
                GUICTRL_SetLcdDevInfo(stb_ctr_ptr, &widget_ctrl_ptr->stb_layer);
                
                s_is_restart_idle_screen = TRUE;
                SetBltLayer(widget_ctrl_ptr);
                ResetWallpaperLayer(widget_ctrl_ptr);
            }
            s_is_elve_first_start    = TRUE;
            if (IsElveWidgetActive(widget_ctrl_ptr))
            {
                SetZorderItemTopMostByWidgetId(widget_ctrl_ptr, MMIWIDGET_ELVE_ID);
            }
            break;

        case MSG_LCD_SWITCH:
            if (s_is_page_moving)
            {
                RestoreAllActiveChildWinRect(widget_ctrl_ptr);
            }
            
            ResetWidgetState(widget_ctrl_ptr);
            SetElveStatus(widget_ctrl_ptr, MMIWIDGET_ELVE_NORMAL_DISPLAY);
            SwitchLCD(widget_ctrl_ptr);
            break;

        case MSG_TIMER:
            if (s_tp_long_press_timer_id == *( GUI_TIMER_ID_T*)param)
            {
                tp_point.cur_tp_point = widget_ctrl_ptr->save_point;
                HandleWidgetTpLongDown(widget_ctrl_ptr, (DPARAM)&tp_point); 
                s_tp_long_press_timer_id = 0;
            }
            else
            if (s_bar_auto_shrink_timer_id == *( GUI_TIMER_ID_T*)param)
            {
                ShrinkWidgetBar(widget_ctrl_ptr);
                RestartAutoShrinkTimer(widget_ctrl_ptr->win_id);
            }
            else
            if (s_tp_move_timer_id == *( GUI_TIMER_ID_T*)param)
            {
                //if (!IsActionDoing())
                {
                    MMI_TP_STATUS_E tp_status = MMI_TP_NONE;
                    MMI_MULTI_KEY_TP_MSG_PARAM_T   para = {0};

                    MMK_GetLogicTPMsg(&tp_status, &point);

		      UILAYER_SetDirectDraw( TRUE );
			  
                    para.cur_tp_point.x = point.x;
	                para.cur_tp_point.y = point.y;
                    
                    RestartTpMoveTimer(widget_ctrl_ptr->win_id);
                    HandleWidgetTpMove(widget_ctrl_ptr, (DPARAM)&para);
                }
            }
            else
            if (s_auto_slide_timer_id == *( GUI_TIMER_ID_T*)param)
            {
                RestartAutoSlideTimer(widget_ctrl_ptr->win_id, MMIWIDGET_AUTO_SLIDE_TIMEOUT);

                if (s_is_auto_slide)
                {
                    MMI_TP_STATUS_E tp_status = MMI_TP_NONE;
                    MMK_GetLogicTPMsg(&tp_status, &point);

		      UILAYER_SetDirectDraw( TRUE );
                    
                  //ShowSlideAnim(widget_ctrl_ptr, point, TRUE);
                    PlaySlideAnim(widget_ctrl_ptr, point, TRUE);
                    
                    s_is_auto_slide = FALSE;
                }
            }
            break;

#ifdef MMIWIDGET_WEATHER_ANIM
        case MSG_WEATHER_NEED_ANIM:
        case MSG_WEATHER_FORBID_ANIM:
            DispatchMsgToAllChildWin(widget_ctrl_ptr, msg_id);
            break;
#endif

    	default:
    		result = MMI_RESULT_FALSE;
    		break;
	}

	return(result);     
}

/*****************************************************************************/
//  Description : dispatch msg to all child win
//  Global resource dependence : 
//  Author:  bin.ji
//  Note:
/*****************************************************************************/
LOCAL void DispatchMsgToAllChildWin(
                             MMIWIDGET_CTRL_T  *widget_ctrl_ptr,
                             MMI_MESSAGE_ID_E  msg_id
                             )
{
    uint32 i = 0;
    
    if (PNULL == widget_ctrl_ptr)
    {
        //SCI_TRACE_LOW:"DispatchMsgToAllChildWin() PNULL == widget_ctrl_ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_2088_112_2_18_3_7_35_77,(uint8*)"");
        return;
    }  
    if (PNULL != widget_ctrl_ptr->item_ptr)
    {
        for (i = 0; i < MMIWIDGET_MAX_ITEM_NUM; i++)
        {
            RunChildWidgetProc(widget_ctrl_ptr, widget_ctrl_ptr->item_ptr[i].item_info.widget_id, msg_id, PNULL);
//            MMK_SendMsg(widget_ctrl_ptr->item_ptr[i].item_info.win_id, msg_id, PNULL);
        }
    }    
}

/*****************************************************************************/
//  Description : destroy the widget control.
//  Global resource dependence : 
//  Author:  bin.ji
//  Note:
/*****************************************************************************/
PUBLIC void MMIWIDGET_Destroy(
                             MMIWIDGET_CTRL_E  ctrl_id
                             )
{
    //目前只支持idle界面widget
    if (MMIWIDGET_CTRL_IDLE_ID != ctrl_id)
    {
        //MMI_ASSERT_WARNING("MMIWIDGET_Destroy() MMIWIDGET_CTRL_IDLE_ID != ctrl_id!");
        return;
    }
    if (PNULL != s_idlewidget_ctrl_ptr)
    {
        MMK_SetWinDisplayStyleState(MAIN_IDLE_WIN_ID, WS_DISABLE_REDRAW_CHILDWIN, FALSE);       
        DestroyWallpaperLayer(s_idlewidget_ctrl_ptr);
        DestroyElveLayer(s_idlewidget_ctrl_ptr);
        DestroyBarLayer(s_idlewidget_ctrl_ptr);
        DestroyStbLayer(s_idlewidget_ctrl_ptr);
        DestroySoftkeyLayer(s_idlewidget_ctrl_ptr);
        DestroyPageTitleLayer(s_idlewidget_ctrl_ptr);
        DestroyWidgetPageLayer(s_idlewidget_ctrl_ptr);
        DestroyAllWidgetItemData(s_idlewidget_ctrl_ptr);

        SCI_FREE(s_idlewidget_ctrl_ptr);
        s_idlewidget_ctrl_ptr = PNULL;
    }
}

/*****************************************************************************/
//  Description : create the widget control.
//  Global resource dependence : 
//  Author:  bin.ji
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIWIDGET_Create(
                             MMI_WIN_ID_T win_id,
                             MMIWIDGET_CTRL_E  ctrl_id,
                             GUI_BOTH_RECT_T *both_rect_ptr
                             )
{
    BOOLEAN result = FALSE;

    //目前只支持idle界面widget
    if (MMIWIDGET_CTRL_IDLE_ID != ctrl_id)
    {
        //MMI_ASSERT_WARNING("MMIWIDGET_Create() MMIWIDGET_CTRL_IDLE_ID != ctrl_id!");
        return FALSE;
    }
    if (PNULL == s_idlewidget_ctrl_ptr)
    {
        s_idlewidget_ctrl_ptr = (MMIWIDGET_CTRL_T*)SCI_ALLOC_APP(sizeof(MMIWIDGET_CTRL_T));
        //初始化
        SCI_MEMSET(s_idlewidget_ctrl_ptr, 0x00, sizeof(MMIWIDGET_CTRL_T));
        MMK_SetWinDisplayStyleState(win_id, WS_DISABLE_REDRAW_CHILDWIN, TRUE);
        s_idlewidget_ctrl_ptr->win_id = win_id;
        s_idlewidget_ctrl_ptr->ctrl_id = ctrl_id;
        if (PNULL != both_rect_ptr)
        {
            s_idlewidget_ctrl_ptr->both_rect = *both_rect_ptr;
        }
        s_idlewidget_ctrl_ptr->item_ptr = PNULL;
        s_idlewidget_ctrl_ptr->zorder_item_ptr = PNULL;
        s_idlewidget_ctrl_ptr->is_hit_msg = FALSE;
        s_idlewidget_ctrl_ptr->widget_layer.lcd_id = GUI_MAIN_LCD_ID;
        s_idlewidget_ctrl_ptr->widget_layer.block_id = UILAYER_NULL_HANDLE;
        s_idlewidget_ctrl_ptr->is_main_screen_landscape = MMK_IsWindowLandscape(win_id);  
	    s_idlewidget_ctrl_ptr->tp_move_is_valid = FALSE;
        MMITHEME_GetWidgetTheme(&s_idlewidget_ctrl_ptr->display_theme);
        //创建3页wallpaper
        CreateWidgetPageLayer(s_idlewidget_ctrl_ptr);
        CreateWallpaperLayer(s_idlewidget_ctrl_ptr);
        CreateBarLayer(s_idlewidget_ctrl_ptr);
        CreateDeskElveLayer(s_idlewidget_ctrl_ptr);
        CreateSoftkeyLayer(s_idlewidget_ctrl_ptr);
        CreateStbLayer(s_idlewidget_ctrl_ptr);
        CreatePageTitleLayer(s_idlewidget_ctrl_ptr);
        MAIN_SetStbTimeVisible(TRUE);
        
        result = TRUE;
    }
    else
    {
        //SCI_TRACE_LOW:"mmiwidget.c MMIWIDGET_Create() ctrl_id = %d is exist!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_2184_112_2_18_3_7_35_78,(uint8*)"d", ctrl_id);
        result = FALSE;
    }
    return result;
}

/*****************************************************************************/
//  Description : Add the dynamic item to widget
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN AppendDynItem(
                            MMIWIDGET_CTRL_T*       widget_ctrl_ptr,
                            const MMIWIDGET_ITEM_T* add_item_ptr
                            )
{
    BOOLEAN             result        = FALSE;

    if ((PNULL != widget_ctrl_ptr) && (PNULL != widget_ctrl_ptr->item_ptr) && (PNULL != add_item_ptr))
    {       
        if (widget_ctrl_ptr->total_item_num < widget_ctrl_ptr->max_item_num)
        {
            if (IsExistInZorderSys(widget_ctrl_ptr, add_item_ptr->item_info.widget_id))
            {
                return FALSE;
            }

            widget_ctrl_ptr->item_ptr[add_item_ptr->item_info.widget_id - 1] = *add_item_ptr;
            widget_ctrl_ptr->total_item_num++;
            CreateChildWidgetWin(widget_ctrl_ptr->win_id, add_item_ptr);                      
            ActiveWidgetItem(widget_ctrl_ptr, add_item_ptr->item_info.widget_id);

            ResetOneWidgetLayer(widget_ctrl_ptr, add_item_ptr->item_info.widget_id);
            result = TRUE;
        }
    }
    //else
    //{
    //    MMI_ASSERT_WARNING("(PNULL != widget_ctrl_ptr) && (PNULL != widget_ctrl_ptr->item_ptr) && (PNULL != add_item_ptr)");
    //}

    return result;
}

/*****************************************************************************/
//  Description : Add the item to widget
//  Global resource dependence : 
//  Author:bin.ji
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIWIDGET_AppendItem(
                                 MMIWIDGET_CTRL_E         ctrl_id,
                                 const MMIWIDGET_ITEM_T   *item_ptr
                                 )
{
    return MMIWIDGET_AppendItemByPtr(MMIAPIWIDGET_GetWidgetPtr(ctrl_id), item_ptr);
}

/*****************************************************************************/
//  Description :  return widget control ptr
//  Global resource dependence : 
//  Author:  bin.ji
//  Note:
/*****************************************************************************/
PUBLIC MMIWIDGET_CTRL_T *MMIAPIWIDGET_GetWidgetPtr(
                                     MMIWIDGET_CTRL_E         ctrl_id
                                     )
{
    //目前只支持idle界面widget
    if (MMIWIDGET_CTRL_IDLE_ID != ctrl_id)
    {
        //MMI_ASSERT_WARNING("MMIAPIWIDGET_GetWidgetPtr() MMIWIDGET_CTRL_IDLE_ID != ctrl_id!");
        return PNULL;
    }
    return s_idlewidget_ctrl_ptr;
}

/*****************************************************************************/
//  Description : 创建widget子窗口
//  Global resource dependence : 
//  Author:bin.ji
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CreateChildWidgetWin(
                                   MMI_WIN_ID_T win_id,
                                   const MMIWIDGET_ITEM_T* add_item_ptr
                                   )
{
    MMI_WINDOW_CREATE_T win_create = {0};
    MMI_HANDLE_T win_handle = 0;
    BOOLEAN result = FALSE;
	GUI_RECT_T init_rect = s_child_win_rec;

    win_create.applet_handle = MMK_GetFirstAppletHandle();  //applet_handle;
    if (PNULL != add_item_ptr)
    {
        win_create.win_id = add_item_ptr->item_info.win_id;
    }
    win_create.func = HandleChildWidgetWinMsg;
    win_create.parent_win_handle = win_id;
    win_create.win_priority = WIN_ONE_LEVEL;//win_priority;
    win_create.add_data_ptr = PNULL;
    win_create.window_style = WS_DISABLE_COMMON_BG;
    if( 0 != ( win_handle = MMK_CreateWindow( &win_create ) ) )
    {
//		MMK_SetWinRectNotifyCtrl(win_handle, &init_rect);
        SetChildWinRect(win_handle, init_rect, FALSE);
        result = TRUE;
    }
    return result;
}

/*****************************************************************************/
//  Description : Add the item to widget list
//  Global resource dependence : 
//  Author:bin.ji
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIWIDGET_AppendItemByPtr(
                                      MMIWIDGET_CTRL_T*       widget_ctrl_ptr,
                                      const MMIWIDGET_ITEM_T* add_item_ptr
                                      )
{
    BOOLEAN             result        = FALSE;

    if (   (PNULL != widget_ctrl_ptr) && (PNULL != widget_ctrl_ptr->item_ptr) 
        && (PNULL != add_item_ptr) && IsValidWidgetId(add_item_ptr->item_info.widget_id))
    {
        if (widget_ctrl_ptr->total_item_num < widget_ctrl_ptr->max_item_num)
        {      
            if (   IsExistInShortcutSys(widget_ctrl_ptr, add_item_ptr->item_info.widget_id) 
                || IsExistInZorderSys(widget_ctrl_ptr, add_item_ptr->item_info.widget_id))
            {
                return FALSE;
            }

            widget_ctrl_ptr->item_ptr[add_item_ptr->item_info.widget_id - 1] = *add_item_ptr;
            widget_ctrl_ptr->total_item_num++;  
            CreateChildWidgetWin(widget_ctrl_ptr->win_id, add_item_ptr);
            
            if (add_item_ptr->is_active)
            {
                result = InitActiveWidgetItem(widget_ctrl_ptr, add_item_ptr->item_info.widget_id);
            }
            else
            {
                result  =  InsertShortcut(
                                         widget_ctrl_ptr, 
                                         add_item_ptr->item_info.small_icon_id, 
                                         add_item_ptr->item_info.widget_id, 
                                         widget_ctrl_ptr->shortcut.shortcut_num
                                         );
                SetWidgetActive(widget_ctrl_ptr, add_item_ptr->item_info.widget_id, FALSE);
            }
           
            ResetOneWidgetLayer(widget_ctrl_ptr, add_item_ptr->item_info.widget_id);
        }
    }
    //else
    //{
    //    MMI_ASSERT_WARNING("(PNULL != widget_ctrl_ptr) && (PNULL != widget_ctrl_ptr->item_ptr) && (PNULL != add_item_ptr)");
    //}

	return result;
}

/*****************************************************************************/
//  Description :  set max item num, must be called before append item
//  Global resource dependence : 
//  Author:  bin.ji
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIWIDGET_SetMaxItemNum(
                                    MMIWIDGET_CTRL_E ctrl_id, // control id
                                    uint32        max_item_num
                                    )
{
    if (max_item_num > MMIWIDGET_MAX_ITEM_NUM)
    {
        max_item_num = MMIWIDGET_MAX_ITEM_NUM;
    }

    return MMIWIDGET_SetMaxItemByPtr(MMIAPIWIDGET_GetWidgetPtr(ctrl_id), max_item_num);
}

/*****************************************************************************/
//  Description : Set the max item of list
//  Global resource dependence : 
//  Author:bin.ji
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIWIDGET_SetMaxItemByPtr(
                                       MMIWIDGET_CTRL_T *widget_ctrl_ptr,
                                       uint32        max_item_num
                                       )
{
    if (PNULL == widget_ctrl_ptr)
    {
        //SCI_TRACE_LOW:"MMIWIDGET_SetMaxItemByPtr() PNULL == widget_ctrl_ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_2369_112_2_18_3_7_35_79,(uint8*)"");
        return FALSE;
    }

    if (0 == max_item_num)
    {
        //MMI_ASSERT_WARNING("max_item_num can't be 0!");
        //SCI_TRACE_LOW:"MMIWIDGET_SetMaxItemByPtr() max_item_num is zero!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_2376_112_2_18_3_7_35_80,(uint8*)"");
        return FALSE;
    }
    
    if (PNULL != widget_ctrl_ptr->item_ptr)
    {
        SCI_FREE(widget_ctrl_ptr->item_ptr);
    }
    widget_ctrl_ptr->item_ptr = (MMIWIDGET_ITEM_T*)SCI_ALLOC_APP(max_item_num*sizeof(MMIWIDGET_ITEM_T));
    SCI_MEMSET(widget_ctrl_ptr->item_ptr, 0x00, max_item_num*sizeof(MMIWIDGET_ITEM_T));
    widget_ctrl_ptr->total_item_num = 0;
    widget_ctrl_ptr->max_item_num = max_item_num;

	return TRUE;  
}

/*****************************************************************************/
//  Description : destroy item data
//  Global resource dependence : 
//  Author:bin.ji
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN DestroyWidgetItem(
                                MMIWIDGET_CTRL_T * widget_ctrl_ptr,  // the list control pointer
                                MMIWIDGET_ITEM_T *item_ptr
                                )
{
    BOOLEAN result = FALSE;

    if (PNULL == widget_ctrl_ptr)
    {
        //SCI_TRACE_LOW:"DestroyWidgetItem() PNULL == widget_ctrl_ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_2405_112_2_18_3_7_36_81,(uint8*)"");
        return FALSE;
    }
    if (PNULL != item_ptr && IsValidWidgetId(item_ptr->item_info.widget_id))
    {
        MMK_CloseWin(item_ptr->item_info.win_id);
        result = TRUE;
    }
    return result;
}

/*****************************************************************************/
//  Description : destroy all item data
//  Global resource dependence : 
//  Author:bin.ji
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN DestroyAllWidgetItemData(
                                    MMIWIDGET_CTRL_T*       widget_ctrl_ptr  // the list control pointer
                                    )
{
    uint32 i = 0;
    BOOLEAN result = TRUE;
    
    if (PNULL == widget_ctrl_ptr)
    {
        //SCI_TRACE_LOW:"DestroyAllWidgetItemData() PNULL == widget_ctrl_ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_2429_112_2_18_3_7_36_82,(uint8*)"");
        return FALSE;
    }
    
    for ( i = 0; i < MMIWIDGET_MAX_ITEM_NUM; i++)
    {
        DestroyWidgetItem( widget_ctrl_ptr, GetItemPtr( widget_ctrl_ptr, i ) );
    }
    if (PNULL != widget_ctrl_ptr->item_ptr)
    {
        SCI_FREE(widget_ctrl_ptr->item_ptr);
        widget_ctrl_ptr->item_ptr = PNULL;
    }

    if (PNULL != widget_ctrl_ptr->zorder_item_ptr)
    {
        SCI_FREE(widget_ctrl_ptr->zorder_item_ptr);
        widget_ctrl_ptr->zorder_item_ptr = PNULL;
    }
    
    return result;
}

/*****************************************************************************/
//  Description : get the appointed item pointer
//  Global resource dependence : 
//  Author:bin.ji
//  Note:
/*****************************************************************************/
LOCAL MMIWIDGET_ITEM_T* GetItemPtr(
                                 const MMIWIDGET_CTRL_T* widget_ctrl_ptr,
                                 uint32              index  //from 0 to total_item_num-1
                                 )
{
    if (PNULL == widget_ctrl_ptr)
    {
        //SCI_TRACE_LOW:"GetItemPtr() PNULL == widget_ctrl_ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_2463_112_2_18_3_7_36_83,(uint8*)"");
        return PNULL;
    } 
    if (index >= widget_ctrl_ptr->max_item_num || 0 == widget_ctrl_ptr->total_item_num)
    {
        return PNULL;
    }
    else
    {
        return widget_ctrl_ptr->item_ptr + index;
    }
}

/*****************************************************************************/
//  Description : update widget item child win
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void UpdatePageDynmicWidgetItem(
                               MMIWIDGET_CTRL_T *widget_ctrl_ptr,
                               uint32 widget_id
                               )
{
    MMI_HANDLE_T child_win_handle = 0;
    MMIWIDGET_ITEM_T *item_ptr = PNULL;
    GUI_RECT_T old_win_rect = {0};
    GUI_POINT_T old_point = {0};
    GUI_POINT_T new_point = {0};
    //uint32 i = 0;   
    uint16 page_width = 0;
    uint16 lcd_width = 0;

    if (PNULL == widget_ctrl_ptr || PNULL == widget_ctrl_ptr->zorder_item_ptr)
    {
        //SCI_TRACE_LOW:"UpdateAllWidgetItem() PNULL == widget_ctrl_ptr || PNULL == widget_ctrl_ptr->zorder_item_ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_2496_112_2_18_3_7_36_84,(uint8*)"");
        return;
    }
    
    lcd_width = MMITHEME_GetMainScreenRightPixel()+1;
    page_width = GetWidgetPageWidth(widget_ctrl_ptr);    
    item_ptr = widget_ctrl_ptr->zorder_item_ptr->zorder_next_ptr;
    while (PNULL != item_ptr)
    {
        //SCI_ASSERT(i++ < widget_ctrl_ptr->total_item_num);  //死循环检测 
        
        if (    (item_ptr->item_info.widget_id == widget_id) 
            && item_ptr->is_active 
            && item_ptr->item_info.is_dynamic)
        {            
            SetZorderItemTopMost(widget_ctrl_ptr, item_ptr);
            child_win_handle = GetWinHandleByWidgetId(widget_ctrl_ptr, item_ptr->item_info.widget_id);               
            MMK_GetWinRect(child_win_handle, &old_win_rect);              

            old_point.x = old_win_rect.left;
            old_point.y = old_win_rect.top;

            if (item_ptr->item_info.start_pos.x < 0)
            {
                 new_point.x = lcd_width * widget_ctrl_ptr->cur_page_index + item_ptr->item_info.start_pos.x % lcd_width;                 
            }
            else
            {
                new_point.x = item_ptr->item_info.start_pos.x % lcd_width;
            }

            new_point.y = item_ptr->item_info.start_pos.y;
                        
            ChangeWidgetWinRect(widget_ctrl_ptr, item_ptr->item_info.widget_id, old_point, new_point, TRUE);   
        }
        
        item_ptr = item_ptr->zorder_next_ptr;
    }
}

/*****************************************************************************/   
//  Description : update widget item child win
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
LOCAL void UpdateAllWidgetItem(
                               MMIWIDGET_CTRL_T *widget_ctrl_ptr
                               )
{
    MMIWIDGET_ITEM_T *item_ptr = PNULL;
    //uint32 i = 0;

    if (PNULL == widget_ctrl_ptr || PNULL == widget_ctrl_ptr->zorder_item_ptr)
    {
        //SCI_TRACE_LOW:"UpdateAllWidgetItem() PNULL == widget_ctrl_ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_2549_112_2_18_3_7_36_85,(uint8*)"");
        return;
    }   
    
    ResetAllWidgetLayer(widget_ctrl_ptr);
    
    item_ptr = widget_ctrl_ptr->zorder_item_ptr->zorder_next_ptr;
    
    while (PNULL != item_ptr)
    {
        //SCI_ASSERT(i++ < widget_ctrl_ptr->total_item_num);  //死循环检测 
        
        if (item_ptr->is_active)
        {            
            RedrawWidgetItem(item_ptr);
        }
        
        item_ptr = item_ptr->zorder_next_ptr;
    }
}

/*****************************************************************************/
//  Description : calculate all small icons postion from idle to bar.
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void CalcIdle2BarIconsPos(MMIWIDGET_CTRL_T *widget_ctrl_ptr, GUI_POINT_T point)
{
    uint32 widget_id = MMIWIDGET_INVALID_ID;
    uint16 icon_width = 0;
    uint16 icon_height = 0;
    MMIWIDGET_ITEM_T *item_ptr = PNULL;
    GUI_POINT_T point_lt = {0};
    uint32 pos = 0;

    pos = GetShortcutItemPos(MMIWIDGET_CTRL_IDLE_ID, widget_ctrl_ptr->tp_move_info.click_widget_id);

    if (MMIWIDGET_INVALID_BAR_POS == pos)
    {
        return;
    }
    
    widget_id = widget_ctrl_ptr->shortcut.shortcut_item[pos].widget_id;

    item_ptr = FindWidgetItem(widget_ctrl_ptr, widget_id);

    if (PNULL == item_ptr)
    {
        return;
    }

    GUIRES_GetImgWidthHeight(&icon_width, &icon_height, item_ptr->item_info.small_icon_id, widget_ctrl_ptr->win_id);

    point_lt.x = point.x - icon_width/2;
    point_lt.y = point.y - icon_height/2;   

    widget_ctrl_ptr->shortcut.shortcut_item_pos[pos] = point_lt;

    if (pos == widget_ctrl_ptr->shortcut.shortcut_num - 1)
    {
        return;
    }
       
    if (0 == pos)
    {
        CalcRangeShortcutItemPos(widget_ctrl_ptr, widget_ctrl_ptr->shortcut_offset, pos + 1, widget_ctrl_ptr->shortcut.shortcut_num);      
        widget_ctrl_ptr->shortcut.shortcut_item_pos[pos +1].y = point_lt.y + icon_height;        
    }
    else
    {
        widget_ctrl_ptr->shortcut.shortcut_item_pos[0].y -= widget_ctrl_ptr->display_theme.shortcut_data.disp_interval;

        CalcRangeShortcutItemPos(widget_ctrl_ptr, widget_ctrl_ptr->shortcut_offset, 1, pos);

        CalcRangeShortcutItemPos(widget_ctrl_ptr, widget_ctrl_ptr->shortcut_offset, pos + 1, widget_ctrl_ptr->shortcut.shortcut_num);
    }
}

/*****************************************************************************/
//  Description : calculate all small icons postion when moving one icon on bar.
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void CalcDynamicIconsPosOnBar(MMIWIDGET_CTRL_T *widget_ctrl_ptr, GUI_POINT_T point)
{
    uint32 widget_id = MMIWIDGET_INVALID_ID;
    uint16 icon_width = 0;
    uint16 icon_height = 0;
    MMIWIDGET_ITEM_T *item_ptr = PNULL;
    GUI_POINT_T point_lt = {0};
    uint32 pos = 0;

    pos = GetShortcutItemPos(MMIWIDGET_CTRL_IDLE_ID, widget_ctrl_ptr->tp_move_info.click_widget_id);

    if (MMIWIDGET_INVALID_BAR_POS == pos)
    {
        return;
    }
    
    widget_id = widget_ctrl_ptr->shortcut.shortcut_item[pos].widget_id;

    item_ptr = FindWidgetItem(widget_ctrl_ptr, widget_id);

    if (PNULL == item_ptr)
    {
        return;
    }

    GUIRES_GetImgWidthHeight(&icon_width, &icon_height, item_ptr->item_info.small_icon_id, widget_ctrl_ptr->win_id);

    point_lt.x = point.x - icon_width/2;
    point_lt.y = point.y - icon_height/2;   

    widget_ctrl_ptr->shortcut.shortcut_item_pos[pos] = point_lt;        
   
    if ((pos > 0) && (point_lt.y < widget_ctrl_ptr->shortcut.shortcut_item_pos[pos - 1].y))
    {
        SwitchWidgetPos(widget_ctrl_ptr, pos - 1, pos);

        CalcRangeShortcutItemPos(widget_ctrl_ptr, widget_ctrl_ptr->shortcut_offset, 0, pos - 1);

        widget_ctrl_ptr->shortcut.shortcut_item_pos[pos - 1] = point_lt;  

        CalcRangeShortcutItemPos(widget_ctrl_ptr, widget_ctrl_ptr->shortcut_offset, pos, widget_ctrl_ptr->shortcut.shortcut_num);            
    }
    else
    if ((pos < widget_ctrl_ptr->shortcut.shortcut_num - 1) && (point_lt.y  > (widget_ctrl_ptr->shortcut.shortcut_item_pos[pos + 1].y)))
    {
        SwitchWidgetPos(widget_ctrl_ptr, pos, pos + 1);

        CalcRangeShortcutItemPos(widget_ctrl_ptr, widget_ctrl_ptr->shortcut_offset, 0, pos + 1);

        widget_ctrl_ptr->shortcut.shortcut_item_pos[pos + 1] = point_lt;  

        CalcRangeShortcutItemPos(widget_ctrl_ptr, widget_ctrl_ptr->shortcut_offset, pos + 1, widget_ctrl_ptr->shortcut.shortcut_num);        
    }
}

/*****************************************************************************/
//  Description : display icon on shortcut bar.
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void DisplayShortcutIcon(MMIWIDGET_CTRL_T *widget_ctrl_ptr, uint32 pos, BOOLEAN is_moving)
{  
    uint32 widget_id = MMIWIDGET_INVALID_ID;
    uint16 icon_width = 0;
    uint16 icon_height = 0;
    MMIWIDGET_ITEM_T *item_ptr = PNULL;
    GUI_POINT_T point_lt = {0};
    GUI_RECT_T item_disp_rect = {0};
    GUI_RECT_T img_disp_rect = {0};

    GUI_POINT_T *DispPoint    = PNULL;
    GUI_RECT_T  *ItemDispRect = PNULL;
    GUI_RECT_T  *ImgDispRect  = PNULL;
    BOOLEAN      is_need_to_draw = FALSE;
    GUI_LCD_DEV_INFO *lcd_dev_ptr = PNULL;

    widget_id = widget_ctrl_ptr->shortcut.shortcut_item[pos].widget_id;

    item_ptr = FindWidgetItem(widget_ctrl_ptr, widget_id);
    
    if (PNULL == item_ptr)
    {
        return;
    }

    point_lt = widget_ctrl_ptr->shortcut.shortcut_item_pos[pos];
    
    GUIRES_GetImgWidthHeight(&icon_width, &icon_height, item_ptr->item_info.small_icon_id, widget_ctrl_ptr->win_id);

    if (   point_lt.y < widget_ctrl_ptr->display_theme.shortcut_data.bar_data.shortcut_bar_rect.top
        && point_lt.y + icon_height > widget_ctrl_ptr->display_theme.shortcut_data.bar_data.shortcut_bar_rect.top)
    {
        img_disp_rect.left   = 0;
        img_disp_rect.top    = (widget_ctrl_ptr->display_theme.shortcut_data.bar_data.shortcut_bar_rect.top-point_lt.y);
        img_disp_rect.right  = (icon_width-1);
        img_disp_rect.bottom = (icon_height-1);  

		point_lt.y = widget_ctrl_ptr->display_theme.shortcut_data.bar_data.shortcut_bar_rect.top;

        DispPoint    = &point_lt;
        ItemDispRect = PNULL;
        ImgDispRect  = &img_disp_rect;   
        is_need_to_draw = TRUE;
    }
    else
    if (   point_lt.y >= widget_ctrl_ptr->display_theme.shortcut_data.bar_data.shortcut_bar_rect.top 
		&& point_lt.y + icon_height <= widget_ctrl_ptr->display_theme.shortcut_data.bar_data.shortcut_bar_rect.bottom)
	{
        item_disp_rect.left   = point_lt.x;
        item_disp_rect.top    = point_lt.y;
        item_disp_rect.right  = point_lt.x + icon_width;
        item_disp_rect.bottom = point_lt.y + icon_height;

        DispPoint    = PNULL;
        ItemDispRect = &item_disp_rect;
        ImgDispRect  = PNULL;
        is_need_to_draw = TRUE;
    }
    else
    if (    point_lt.y < widget_ctrl_ptr->display_theme.shortcut_data.bar_data.shortcut_bar_rect.bottom
		 && point_lt.y + icon_height > widget_ctrl_ptr->display_theme.shortcut_data.bar_data.shortcut_bar_rect.bottom)
    {
        item_disp_rect.left   = point_lt.x;
        item_disp_rect.top    = point_lt.y;
        item_disp_rect.right  = widget_ctrl_ptr->display_theme.shortcut_data.bar_data.shortcut_bar_rect.right;
        item_disp_rect.bottom = widget_ctrl_ptr->display_theme.shortcut_data.bar_data.shortcut_bar_rect.bottom;

        DispPoint    = PNULL;
        ItemDispRect = &item_disp_rect;
        ImgDispRect  = PNULL;
        is_need_to_draw = TRUE;
    }
    
    if (is_need_to_draw)
    {
        if (is_moving)
        {
            if (UILAYER_IsLayerActive(&widget_ctrl_ptr->widget_layer))
            {
                lcd_dev_ptr = &widget_ctrl_ptr->widget_layer;                
                UILAYER_SetLayerPosition(&widget_ctrl_ptr->widget_layer, point_lt.x, point_lt.y);
            }
            else
            {
                lcd_dev_ptr = MMITHEME_GetDefaultLcdDev(); /*lint !e605*/
            }
        }
        else
        {
            if (UILAYER_IsLayerActive(&widget_ctrl_ptr->bar_layer))
            {
                lcd_dev_ptr = &widget_ctrl_ptr->bar_layer;
            }
            else
            {
                lcd_dev_ptr = MMITHEME_GetDefaultLcdDev(); /*lint !e605*/
            }  
            LCD_ConvertDispPoint(GUILCD_CONVERT_POS_DISP2LCD, &widget_ctrl_ptr->bar_layer, DispPoint, DispPoint); /*lint !e605*/
            LCD_ConvertDispRect(GUILCD_CONVERT_POS_DISP2LCD, &widget_ctrl_ptr->bar_layer, ItemDispRect, ItemDispRect); /*lint !e605*/           
        }
        GUIRES_DisplayImg(
            DispPoint,
            ItemDispRect, 
            ImgDispRect,
            widget_ctrl_ptr->win_id,
            item_ptr->item_info.small_icon_id,
            lcd_dev_ptr
            );
        is_need_to_draw = FALSE;
    }
}

/*****************************************************************************/
//  Description : display icon on shortcut bar for moving widget icon.
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void DisplayCustomWidgetOnBar(MMIWIDGET_CTRL_T *widget_ctrl_ptr)
{
    uint32 i   = 0;
    uint32 pos = 0;
    BOOLEAN result = FALSE;

    //widget应用如果打开新窗口则不用刷新
    if (!MMK_IsFocusWin(widget_ctrl_ptr->win_id))
    {
        return;
    }

    pos = GetShortcutItemPos(MMIWIDGET_CTRL_IDLE_ID, widget_ctrl_ptr->tp_move_info.click_widget_id);

    for (i = 0; i < widget_ctrl_ptr->shortcut.shortcut_num; i++)
    {
        if (i == pos)
        {
            result = TRUE;
            continue;
        }

        DisplayShortcutIcon(widget_ctrl_ptr, i, FALSE);      
    }

    if (result)
    {
        DisplayShortcutIcon(widget_ctrl_ptr, pos, TRUE);    
    }
}


/*****************************************************************************/
//  Description : display dynamic widget shortcut
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void DisplayDynWidgetShortcut(MMIWIDGET_CTRL_T *widget_ctrl_ptr, GUI_POINT_T point)
{
    GUI_RECT_T bar_rect = {0};
    MMI_IMAGE_ID_T switch_img_id = 0;

    if (PNULL == widget_ctrl_ptr)
    {
        //SCI_TRACE_LOW:"DisplayWidgetShortcut() PNULL == widget_ctrl_ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_2848_112_2_18_3_7_36_86,(uint8*)"");
        return;
    }    

    //widget应用如果打开新窗口则不用刷新
    if (!MMK_IsFocusWin(widget_ctrl_ptr->win_id))
    {
        return;
    }

    UILAYER_Clear(&widget_ctrl_ptr->bar_layer);

    //shortcut bar
    GUIRES_DisplayImg(
        PNULL,
        &(widget_ctrl_ptr->display_theme.shortcut_data.bar_data.shortcut_bar_bg_rect),
        PNULL,
        widget_ctrl_ptr->win_id,
        widget_ctrl_ptr->display_theme.shortcut_data.bar_data.shortcut_bar_bg_icon,
        &widget_ctrl_ptr->bar_layer
        );

    if (widget_ctrl_ptr->shortcut.is_shortcut_on)
    {
        switch_img_id = widget_ctrl_ptr->display_theme.shortcut_data.switch_data.shortcut_bar_switch_close_off_icon; 
    }
    else
    {
        switch_img_id = widget_ctrl_ptr->display_theme.shortcut_data.switch_data.shortcut_bar_switch_open_off_icon;     
    }

    //shortcut bar switch button
    bar_rect = widget_ctrl_ptr->display_theme.shortcut_data.switch_data.shortcut_bar_switch_rect;
    
    GUIRES_DisplayImg(
        PNULL,
        &bar_rect,
        PNULL,
        widget_ctrl_ptr->win_id,
        switch_img_id,
        &widget_ctrl_ptr->bar_layer
        );

    DisplayCustomWidgetOnBar(widget_ctrl_ptr);   
}

/*****************************************************************************/
//  Description : display widget shortcut
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
LOCAL void DisplayWidgetShortcut(
                                  MMIWIDGET_CTRL_T           *widget_ctrl_ptr
                                  )
{
    uint32 index = 0;
    GUI_RECT_T bar_rect = {0};
    MMI_IMAGE_ID_T switch_img_id = 0;
    GUI_LCD_DEV_INFO *dev_info_ptr = PNULL;

    if (PNULL == widget_ctrl_ptr)
    {
        //SCI_TRACE_LOW:"DisplayWidgetShortcut() PNULL == widget_ctrl_ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_2909_112_2_18_3_7_37_87,(uint8*)"");
        return;
    }  

    if (UILAYER_IsLayerActive(&widget_ctrl_ptr->bar_layer))
    {
        dev_info_ptr = &widget_ctrl_ptr->bar_layer;
        UILAYER_Clear(dev_info_ptr);
    }
    else
    {
        dev_info_ptr = MMITHEME_GetDefaultLcdDev(); /*lint !e605*/
    }  
       
    bar_rect = widget_ctrl_ptr->display_theme.shortcut_data.bar_data.shortcut_bar_bg_rect;
    LCD_ConvertDispRect(GUILCD_CONVERT_POS_DISP2LCD, &widget_ctrl_ptr->bar_layer, &bar_rect, &bar_rect);
    //shortcut bar
    GUIRES_DisplayImg(
        PNULL,
        &bar_rect,
        PNULL,
        widget_ctrl_ptr->win_id,
        widget_ctrl_ptr->display_theme.shortcut_data.bar_data.shortcut_bar_bg_icon,
        dev_info_ptr
        );

    if (widget_ctrl_ptr->shortcut.is_shortcut_on)
    {
        switch_img_id = widget_ctrl_ptr->display_theme.shortcut_data.switch_data.shortcut_bar_switch_close_off_icon; 
    }
    else
    {
        switch_img_id = widget_ctrl_ptr->display_theme.shortcut_data.switch_data.shortcut_bar_switch_open_off_icon;     
    }
    
    //shortcut bar switch button
    bar_rect = widget_ctrl_ptr->display_theme.shortcut_data.switch_data.shortcut_bar_switch_rect;
    LCD_ConvertDispRect(GUILCD_CONVERT_POS_DISP2LCD, &widget_ctrl_ptr->bar_layer, &bar_rect, &bar_rect);
    GUIRES_DisplayImg(
        PNULL,
        &bar_rect,
        PNULL,
        widget_ctrl_ptr->win_id,
        switch_img_id,
        dev_info_ptr
        );

    if (s_is_restart_idle_screen)
    {
        return;
    }
    
    //display all shortcut icons.
    for (index=0; index<widget_ctrl_ptr->shortcut.shortcut_num; index++)
    {
        DisplayShortcutIcon(widget_ctrl_ptr, index, FALSE);          
    }
}

/*****************************************************************************/
//  Description : find widget item
//  Global resource dependence :
//  Author: bin.ji
//  Note: 
/*****************************************************************************/
LOCAL MMIWIDGET_ITEM_T *FindWidgetItem(
                                       MMIWIDGET_CTRL_T *widget_ctrl_ptr,
                                       uint32 widget_id
                                       )
{
    uint32 i = 0;
    MMIWIDGET_ITEM_T *item_ptr = PNULL;

    if (PNULL == widget_ctrl_ptr)
    {
        //SCI_TRACE_LOW:"FindWidgetItem() PNULL == widget_ctrl_ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_2982_112_2_18_3_7_37_88,(uint8*)"");
        return PNULL;
    }

    if (PNULL != widget_ctrl_ptr->item_ptr && IsValidWidgetId(widget_id))
    {
        for (i = 0; i < MMIWIDGET_MAX_ITEM_NUM; i++)
        {
            if (widget_id == widget_ctrl_ptr->item_ptr[widget_id - 1].item_info.widget_id)
            {
                item_ptr = &(widget_ctrl_ptr->item_ptr[widget_id - 1]);
                break;
            }
        }
    }
    return item_ptr;
}

/*****************************************************************************/
//  Description : is point in shortcut switch region
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsPointInShortcutSwitchRegion(
                               MMIWIDGET_CTRL_T *widget_ctrl_ptr,
                               GUI_POINT_T point
                               )
{
    GUI_RECT_T rect = {0};

    if (PNULL == widget_ctrl_ptr)
    {
        //SCI_TRACE_LOW:"IsPointInShortcutRegion() PNULL == widget_ctrl_ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_3013_112_2_18_3_7_37_89,(uint8*)"");
        return FALSE;
    }

    if (widget_ctrl_ptr->shortcut.is_shortcut_on)
    {
        rect = widget_ctrl_ptr->display_theme.shortcut_data.switch_data.shortcut_bar_switch_rect;
        if (GUI_PointIsInRect(point, rect))
        {
            return TRUE;
        }
    }
    else
    {
        rect = widget_ctrl_ptr->display_theme.shortcut_data.switch_data.shortcut_bar_switch_rect;
        rect.right -= rect.left;
        rect.left = 0;
        if (GUI_PointIsInRect(point, rect))
        {
            return TRUE;
        }
    }
    return FALSE;
}

/*****************************************************************************/
//  Description : is point in shortcut region
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsPointInShortcutRegion(
                               MMIWIDGET_CTRL_T *widget_ctrl_ptr,
                               GUI_POINT_T point
                               )
{
    if (PNULL == widget_ctrl_ptr)
    {
        //SCI_TRACE_LOW:"IsPointInShortcutRegion() PNULL == widget_ctrl_ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_3049_112_2_18_3_7_37_90,(uint8*)"");
        return FALSE;
    }

    if (widget_ctrl_ptr->shortcut.is_shortcut_on)
    {
        if (GUI_PointIsInRect(point, widget_ctrl_ptr->display_theme.shortcut_data.bar_data.shortcut_bar_rect))
        {
            return TRUE;
        }
    }
    return FALSE;
}

/*****************************************************************************/
//  Description : is point in shortcut extra region
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsPointInShortcutExtRegion(
                               MMIWIDGET_CTRL_T *widget_ctrl_ptr,
                               GUI_POINT_T point
                               )
{
    GUI_RECT_T rect_top = {0};
    GUI_RECT_T rect_bot = {0};
    GUI_RECT_T main_rect = MMITHEME_GetFullScreenRect();
    
    if (PNULL == widget_ctrl_ptr)
    {
        //SCI_TRACE_LOW:"IsPointInShortcutRegion() PNULL == widget_ctrl_ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_3078_112_2_18_3_7_37_91,(uint8*)"");
        return FALSE;
    }

    rect_top.left   = 0;
    rect_top.top    = 0;
    rect_top.right  = widget_ctrl_ptr->display_theme.shortcut_data.bar_data.shortcut_bar_rect.right;
    rect_top.bottom = widget_ctrl_ptr->display_theme.shortcut_data.bar_data.shortcut_bar_rect.top;

    rect_bot.left   = 0;
    rect_bot.top    = widget_ctrl_ptr->display_theme.shortcut_data.bar_data.shortcut_bar_rect.bottom;
    rect_bot.right  = widget_ctrl_ptr->display_theme.shortcut_data.bar_data.shortcut_bar_rect.right; 
    rect_bot.bottom = main_rect.bottom - main_rect.top; 

    if (widget_ctrl_ptr->shortcut.is_shortcut_on)
    {
        if (GUI_PointIsInRect(point, rect_top) || GUI_PointIsInRect(point, rect_bot))
        {
            return TRUE;
        }
    }
    return FALSE;
}

#if 0
/*****************************************************************************/
//  Description : handle widget tp up
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN CheckTotalWidgetNum(
                               MMIWIDGET_CTRL_T *widget_ctrl_ptr
                               )
{
    MMIWIDGET_ITEM_T *item_ptr = PNULL;
    uint32 active_num = 0;
    uint32 deactive_num = 0;

    if (PNULL == widget_ctrl_ptr || PNULL == widget_ctrl_ptr->zorder_item_ptr)
    {
        //SCI_TRACE_LOW:"CheckTotalWidgetNum() PNULL == widget_ctrl_ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_3117_112_2_18_3_7_37_92,(uint8*)"");
        return FALSE;
    }

    item_ptr = widget_ctrl_ptr->zorder_item_ptr->zorder_next_ptr;
    
    while (PNULL != item_ptr)
    {
        if (item_ptr->is_active)
        {
            active_num++;
        }
        
        item_ptr = item_ptr->zorder_next_ptr;
    }
    
    deactive_num = widget_ctrl_ptr->shortcut.shortcut_num;
    
    //SCI_ASSERT((active_num + deactive_num) == widget_ctrl_ptr->total_item_num); 
    
    return TRUE;
}
#endif

/*****************************************************************************/
//  Description : handle widget tp up
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN ChangeWidgetWinRect(
                               MMIWIDGET_CTRL_T *widget_ctrl_ptr,
                               uint32 widget_id,
                               GUI_POINT_T old_point,
                               GUI_POINT_T point,
                               BOOLEAN is_change
                               )
{
    MMIWIDGET_ITEM_T *item_ptr = PNULL;
    BOOLEAN result = FALSE;
    GUI_RECT_T old_win_rect = {0};
    GUI_RECT_T win_rect = {0};
    MMI_HANDLE_T win_handle = 0;
    uint16 bg_width = 0;
    uint16 bg_height = 0;
    if (PNULL == widget_ctrl_ptr)
    {
        //SCI_TRACE_LOW:"ChangeWidgetWinRect() PNULL == widget_ctrl_ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_3162_112_2_18_3_7_37_93,(uint8*)"");
        return result;
    }
    item_ptr = FindWidgetItem(widget_ctrl_ptr, widget_id);
    if (PNULL == item_ptr)
    {
        return result;
    }
    win_handle = GetWinHandleByWidgetId(widget_ctrl_ptr, widget_id);
    GUIRES_GetImgWidthHeight(&bg_width, &bg_height, item_ptr->item_info.large_bg_img, item_ptr->item_info.win_id);
    MMK_GetWinRect(win_handle, &old_win_rect);
    if (is_change)
    {
        win_rect.left = old_win_rect.left + (point.x - old_point.x);
        win_rect.top = old_win_rect.top + (point.y - old_point.y);
        win_rect.right = win_rect.left + bg_width -1;
        win_rect.bottom = win_rect.top + bg_height-1;                
    }
    else
    {
        win_rect.left = point.x - bg_width/2;
        win_rect.top = point.y - bg_height/2;
        win_rect.right = win_rect.left + bg_width -1;
        win_rect.bottom = win_rect.top + bg_height-1;  
    }

    SetChildWinRect(win_handle, win_rect, widget_ctrl_ptr->is_main_screen_landscape);
//    MMK_SetWinRectNotifyCtrl(win_handle, &win_rect);
    result = TRUE;
    
    return result;
}

/*****************************************************************************/
//  Description : 重新恢复到主层上（跟SetHitWidgetTopmost对应）
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
LOCAL void ResetHitWidgetLayer(
                               MMIWIDGET_CTRL_T *widget_ctrl_ptr
                               )
{
    uint32 click_widget_id = 0;
    if (PNULL == widget_ctrl_ptr)
    {
        //SCI_TRACE_LOW:"ResetHitWidgetLayer() PNULL == widget_ctrl_ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_3206_112_2_18_3_7_37_94,(uint8*)"");
        return;
    }
    click_widget_id = widget_ctrl_ptr->tp_move_info.click_widget_id;
    widget_ctrl_ptr->tp_move_info.click_widget_id = MMIWIDGET_INVALID_ID;
    ResetOneWidgetLayer(widget_ctrl_ptr, click_widget_id);
    widget_ctrl_ptr->tp_move_info.click_widget_id = click_widget_id;
}

/*****************************************************************************/
//  Description : 设置某widget应用到widget_layer，page_layer或主层上
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
LOCAL void ResetOneWidgetLayer(
                            MMIWIDGET_CTRL_T *widget_ctrl_ptr,
                            uint32 widget_id
                            )
{
    GUI_LCD_DEV_INFO lcd_dev_info = {0};
    MMI_HANDLE_T win_handle = 0;
    GUI_LCD_DEV_INFO *dev_info_ptr = PNULL;

    if (PNULL == widget_ctrl_ptr)
    {
        //SCI_TRACE_LOW:"ResetOneWidgetLayer() PNULL == widget_ctrl_ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_3230_112_2_18_3_7_37_95,(uint8*)"");
        return;
    }
    
    if (MMIWIDGET_INVALID_ID != widget_id)
    {
        win_handle = GetWinHandleByWidgetId(widget_ctrl_ptr, widget_id); /*lint !e605*/

        if ((widget_ctrl_ptr->tp_move_info.click_widget_id == widget_id)
            && UILAYER_IsLayerActive(&widget_ctrl_ptr->widget_layer))
        {
            dev_info_ptr = &widget_ctrl_ptr->widget_layer;
        }
        else
        if (IsElveWidget(widget_id)
            && UILAYER_IsLayerActive(&widget_ctrl_ptr->elve_layer))
        {
            dev_info_ptr = &widget_ctrl_ptr->elve_layer;
        }
        else if(UILAYER_IsLayerActive(&widget_ctrl_ptr->page_layer))
        {
            dev_info_ptr = &widget_ctrl_ptr->page_layer;
        }
        else
        {
            dev_info_ptr = MMITHEME_GetDefaultLcdDev(); /*lint !e605*/
        }

        MMK_GetWinLcdDevInfo(win_handle, &lcd_dev_info);

        if (dev_info_ptr->block_id != lcd_dev_info.block_id)
        {
            MMK_SetWinLcdDevInfoNotifyCtrl(
                win_handle,
                dev_info_ptr
                );
        }
    }
}

/*****************************************************************************/
//  Description : reset widget status.
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN ResetWidgetState(MMIWIDGET_CTRL_T *widget_ctrl_ptr)
{
    if (PNULL == widget_ctrl_ptr)
    {
        //SCI_TRACE_LOW:"HandleWidgetTpUp() PNULL == widget_ctrl_ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_3278_112_2_18_3_7_37_96,(uint8*)"");
        return FALSE;
    }

    //销毁widget图层
    DestroyWidgetLayer(widget_ctrl_ptr);

    //SCI_TRACE_LOW:"ResetHitWidgetLayer()00000 widget_id = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_3285_112_2_18_3_7_37_97,(uint8*)"d", widget_ctrl_ptr->tp_move_info.click_widget_id);
    ResetHitWidgetLayer(widget_ctrl_ptr);  
    SetBltLayer(widget_ctrl_ptr);
    ResetWallpaperLayer(widget_ctrl_ptr);
//    UpdateAllScreen();
    
    widget_ctrl_ptr->tp_move_info.click_widget_id = MMIWIDGET_INVALID_ID;
    widget_ctrl_ptr->tp_move_info.click_type      = MMIWIDGET_TP_CLICK_INVALID;
    widget_ctrl_ptr->tp_move_info.pen_move_dir    = MMIWIDGET_PEN_MOVE_INVALID;
    widget_ctrl_ptr->tp_move_is_valid             = FALSE;
    
    //CheckTotalWidgetNum(widget_ctrl_ptr);
    MMIWIDGET_SaveWidgetInfoToNV(widget_ctrl_ptr);

    //recovery shortcut position.
    CalcAllShortcutItemPos(widget_ctrl_ptr, widget_ctrl_ptr->shortcut_offset);
        
    return TRUE;
}

/*****************************************************************************/
//  Description : reset widget bar status.
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN ResetWidgetBarState(MMIWIDGET_CTRL_T *widget_ctrl_ptr)
{
    int16 offset_x = 0;
    int16 offset_y = 0;

    UILAYER_GetLayerPosition(&widget_ctrl_ptr->bar_layer, &offset_x, &offset_y);
    
    if (widget_ctrl_ptr->shortcut.is_shortcut_on)
    {
        offset_x = 0;    
    }
    else
    {
        offset_x = widget_ctrl_ptr->display_theme.shortcut_data.bar_data.shortcut_bar_rect.left - widget_ctrl_ptr->display_theme.shortcut_data.bar_data.shortcut_bar_rect.right - 1;
    }
    
    UILAYER_SetLayerPosition(&widget_ctrl_ptr->bar_layer, offset_x, offset_y);

    return TRUE;
}

/*****************************************************************************/
//  Description : vibrate widget item when place it on shortcut bar.
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void VibrateWidgetItem(MMIWIDGET_CTRL_T *widget_ctrl_ptr)
{
    uint32 i = 0;
    uint32 widget_id = 0;
    int16  widget_posX = 0; 
    uint16 vibrate_count = 0;
    uint32 pos = MMIWIDGET_INVALID_BAR_POS;
    MMIWIDGET_VIBRATE_DIRECT_T vibrate_direct = MMIWIDGET_SHORTCUT_VIBARTE_LEFT;    

    //SCI_TRACE_LOW:"VibrateWidgetItem() startup"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_3343_112_2_18_3_7_37_98,(uint8*)"");
    
    if (PNULL == widget_ctrl_ptr)
    {
        return;
    }

    widget_id   = widget_ctrl_ptr->tp_move_info.click_widget_id;
    pos         = GetShortcutItemPos(MMIWIDGET_CTRL_IDLE_ID, widget_id);
    widget_posX = widget_ctrl_ptr->shortcut.shortcut_item_pos[pos].x; 

    if (MMIWIDGET_INVALID_BAR_POS == pos)
    {
        return;
    }

    while (vibrate_count < MMIWIDGET_VIBRATE_COUNT)
    {
        if (ABS(widget_posX - widget_ctrl_ptr->shortcut.shortcut_item_pos[pos].x) <= MMIWIDGET_VIBRATE_STEP_RANGE)
        {
            if (MMIWIDGET_SHORTCUT_VIBARTE_LEFT == vibrate_direct)
            {
                 widget_ctrl_ptr->shortcut.shortcut_item_pos[pos].x -= MMIWIDGET_VIBRATE_HSTEP; 

                 for (i = 0; i < pos; i++)
                 {
                     widget_ctrl_ptr->shortcut.shortcut_item_pos[i].y -= MMIWIDGET_VIBRATE_VSTEP;
                 }
         
                 for (i = pos + 1; i < widget_ctrl_ptr->shortcut.shortcut_num; i++)
                 {
                     widget_ctrl_ptr->shortcut.shortcut_item_pos[i].y += MMIWIDGET_VIBRATE_VSTEP;
                 }

                 if (ABS(widget_posX - widget_ctrl_ptr->shortcut.shortcut_item_pos[pos].x) == MMIWIDGET_VIBRATE_STEP_RANGE)
                 {
                     vibrate_direct = MMIWIDGET_SHORTCUT_VIBARTE_RIGHT;
                 }
            }
            else
            if (MMIWIDGET_SHORTCUT_VIBARTE_RIGHT == vibrate_direct)
            {
                widget_ctrl_ptr->shortcut.shortcut_item_pos[pos].x += MMIWIDGET_VIBRATE_HSTEP; 

                for (i = 0; i < pos; i++)
                {
                    widget_ctrl_ptr->shortcut.shortcut_item_pos[i].y += MMIWIDGET_VIBRATE_VSTEP;
                }

                for (i = pos + 1; i < widget_ctrl_ptr->shortcut.shortcut_num; i++)
                {
                    widget_ctrl_ptr->shortcut.shortcut_item_pos[i].y -= MMIWIDGET_VIBRATE_VSTEP;
                } 
                
                if (ABS(widget_ctrl_ptr->shortcut.shortcut_item_pos[pos].x - widget_posX) == MMIWIDGET_VIBRATE_STEP_RANGE)
                {
                    vibrate_direct = MMIWIDGET_SHORTCUT_VIBARTE_LEFT;
                }            
            }
        }
        
        DisplayWidgetShortcut(widget_ctrl_ptr);
        MMITHEME_UpdateRect();
        vibrate_count++;
        
        SCI_SLEEP(1);
    }

    MMIWIDGET_SetShortcutOffset(MMIWIDGET_CTRL_IDLE_ID, widget_ctrl_ptr->shortcut_offset);
    DisplayWidgetShortcut(widget_ctrl_ptr);
    MMITHEME_UpdateRect();
}

/*****************************************************************************/
//  Description : reset widget tp state.
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void ResetWidgetTpState(MMIWIDGET_CTRL_T *widget_ctrl_ptr)
{
    widget_ctrl_ptr->tp_move_is_valid = FALSE;
    widget_ctrl_ptr->down_point.x = MMIWIDGET_INT16_MIN_VALUE;
    widget_ctrl_ptr->tp_move_info.click_widget_id = MMIWIDGET_INVALID_ID;
    widget_ctrl_ptr->tp_move_info.click_type      = MMIWIDGET_TP_CLICK_INVALID;
    //CheckTotalWidgetNum(widget_ctrl_ptr);
    MMIWIDGET_SaveWidgetInfoToNV(widget_ctrl_ptr);
}

/*****************************************************************************/
//  Description : try to reset moving child win postion.
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN ResetChildWinPosition(MMIWIDGET_CTRL_T *widget_ctrl_ptr, GUI_POINT_T point)
{
    BOOLEAN ret = FALSE;
    GUI_RECT_T win_rect = {0};
    MMIWIDGET_ITEM_T *item_ptr = PNULL;
    GUI_RECT_T old_win_rect = {0};
    GUI_RECT_T backup_old_win_rect = {0};    
    MMI_HANDLE_T hit_win_handle = 0;
	BOOLEAN is_need_reset = FALSE;
    uint16  bg_width  = 0;
    uint16  bg_height = 0;
    int16 max_page_width = 0;
    int16 max_page_height = 0;


    max_page_width = (MMITHEME_GetMainScreenRightPixel()+1)*MMIWIDGET_MAX_PAGE_NUM;
    max_page_height = (MMITHEME_GetMainScreenBottomPixel()+1);
    
    ChangeWidgetWinRect(widget_ctrl_ptr, widget_ctrl_ptr->tp_move_info.click_widget_id, widget_ctrl_ptr->save_point, point, TRUE);

	/* add these codes to reset the widget item positon */
    item_ptr = FindWidgetItem(widget_ctrl_ptr, widget_ctrl_ptr->tp_move_info.click_widget_id);
    hit_win_handle = GetWinHandleByWidgetId(widget_ctrl_ptr, widget_ctrl_ptr->tp_move_info.click_widget_id);
    MMK_SetChildWinFocus(widget_ctrl_ptr->win_id, hit_win_handle);
    MMK_GetWinRect(hit_win_handle, &old_win_rect);

	if (PNULL != item_ptr)
	{
        GUIRES_GetImgWidthHeight(&bg_width, &bg_height, item_ptr->item_info.large_bg_img, item_ptr->item_info.win_id);
                        
        backup_old_win_rect = old_win_rect;
        LCD_ConvertDispRect(GUILCD_CONVERT_POS_LCD2DISP, &widget_ctrl_ptr->page_layer, &old_win_rect, &old_win_rect);
        
        if (   (old_win_rect.right - bg_width/2 >= max_page_width)
            || (old_win_rect.left + bg_width/2 <= 0)
            || (old_win_rect.bottom - bg_height/2 >= max_page_height)
            || (old_win_rect.top + bg_height/2 <= 0))
        {
            is_need_reset = TRUE;
        }
        
	    if (is_need_reset)
	    { 
	        ClearWidgetRect(widget_ctrl_ptr, backup_old_win_rect);

            win_rect = old_win_rect;

            if (old_win_rect.right - bg_width/2 >= max_page_width)
            {
                win_rect.left    = max_page_width - bg_width;
                win_rect.right   = max_page_width - 1;
            }

            if (old_win_rect.left + bg_width/2 <= 0)
            {
                win_rect.left    = 0;
                win_rect.right   = bg_width - 1;              
            }

            if (old_win_rect.bottom - bg_height/2 >= max_page_height)
            {
                win_rect.top     = max_page_height - bg_height;
                win_rect.bottom  = max_page_height - 1;                 
            }

            if (old_win_rect.top + bg_height/2 <= 0)
            {
                win_rect.top     = 0;
                win_rect.bottom  = bg_height;                
            }

            LCD_ConvertDispRect(GUILCD_CONVERT_POS_DISP2LCD, &widget_ctrl_ptr->page_layer, &win_rect, &win_rect);    
            SetChildWinRect(hit_win_handle, win_rect, widget_ctrl_ptr->is_main_screen_landscape);
	        //RedrawWidgetItem(item_ptr);
            SaveTpMovePoint(widget_ctrl_ptr, point);
            ret = TRUE;
	    }       
    }

    return ret;
}

/**********************************************************************************/
//  Description : handle active widgets: check active num, compare with limitation.
//  Global resource dependence : 
//  Author:
//  Note: 
/**********************************************************************************/
LOCAL BOOLEAN HandleActiveWidgetNum(MMIWIDGET_CTRL_T *widget_ctrl_ptr)
{
    uint32 i = 0;
    uint32 dynamic_count = 0;
    MMIWIDGET_ITEM_T *item_ptr = PNULL;
    uint32 dynamic_widget[] = {MMIWIDGET_NEWSMS_ID,
#ifdef MMI_ENABLE_DCD
                               MMIWIDGET_DCD_ID,
#endif                               
                               MMIWIDGET_CALLLOG_ID,
                               MMIWIDGET_AUTOANSWER_LOG_ID,
                               MMIWIDGET_CUSTOM_WORD_ID}; 

    for (i = 0; i < sizeof(dynamic_widget)/sizeof(uint32); i++)
    {
        item_ptr = FindWidgetItem(widget_ctrl_ptr, dynamic_widget[i]);
 
        if((PNULL != item_ptr) && item_ptr->is_active)
        {
            dynamic_count++;
        }
    }

    if ((widget_ctrl_ptr->total_item_num - widget_ctrl_ptr->shortcut.shortcut_num - dynamic_count) > 10)
    {
        MoveWidgetIdle2BarByPos(widget_ctrl_ptr, widget_ctrl_ptr->tp_move_info.click_widget_bar_pos);
        //VibrateWidgetItem(widget_ctrl_ptr);
        MMIPUB_OpenAlertWinByTextId(PNULL, TXT_WIDGET_MAX_ACTIVE_NUM,TXT_NULL, IMAGE_PUBWIN_WARNING, PNULL, PNULL, MMIPUB_SOFTKEY_NONE, PNULL);
        return TRUE;
    }  
    
    return FALSE;
}

/*****************************************************************************/
//  Description : handle widget tp up
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN HandleWidgetTpUp(
                               MMIWIDGET_CTRL_T *widget_ctrl_ptr,
                               DPARAM            param
                               )
{
    uint32 pos = 0;
    MMIWIDGET_ITEM_T *item_ptr = PNULL;
    GUI_POINT_T point = {0};
    GUI_RECT_T old_win_rect = {0};
    int16 max_page_width = 0;
    int16 max_page_height = 0;
    int16 layer_offsetX = 0;
    int16 layer_offsetY = 0;
    int16 offset = 0;
MMI_CheckAllocatedMemInfo();
    point.x = MMK_GET_TP_X(param);
    point.y = MMK_GET_TP_Y(param);

    if (PNULL == widget_ctrl_ptr)
    {
        //SCI_TRACE_LOW:"HandleWidgetTpUp() PNULL == widget_ctrl_ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_3578_112_2_18_3_7_38_99,(uint8*)"");
        return FALSE;
    }
    
    max_page_width = (MMITHEME_GetMainScreenRightPixel()+1)*MMIWIDGET_MAX_PAGE_NUM;
    max_page_height = (MMITHEME_GetMainScreenBottomPixel()+1);

	if(widget_ctrl_ptr->is_child_win_btn)
    {
        RunChildWidgetProc(widget_ctrl_ptr, widget_ctrl_ptr->tp_move_info.click_widget_id, MSG_WIDGET_TP_PRESS_UP, param);
    }
    else
	if (   MMIWIDGET_TP_CLICK_SHORTCUT_BTN == widget_ctrl_ptr->tp_move_info.click_type
        && MMIWIDGET_TP_MOVE_BAR_BTN == widget_ctrl_ptr->widget_mov_type)
    {
        UILAYER_GetLayerPosition(&widget_ctrl_ptr->bar_layer, &layer_offsetX, &layer_offsetY);

        if (layer_offsetX >= (widget_ctrl_ptr->display_theme.shortcut_data.bar_data.shortcut_bar_rect.left - widget_ctrl_ptr->display_theme.shortcut_data.bar_data.shortcut_bar_rect.right - 1)/2)
        {
            MoveWidgetBarTpUp(widget_ctrl_ptr, layer_offsetX, TRUE);
            widget_ctrl_ptr->shortcut.is_shortcut_on = TRUE;
                     
            if (widget_ctrl_ptr->shortcut.is_shortcut_on && s_is_restart_idle_screen)
            {
                s_is_restart_idle_screen = FALSE;            
                offset = widget_ctrl_ptr->display_theme.shortcut_data.bar_data.shortcut_bar_rect.bottom - widget_ctrl_ptr->display_theme.shortcut_data.bar_data.shortcut_bar_rect.top;
                MMIWIDGET_SetShortcutOffset(MMIWIDGET_CTRL_IDLE_ID, offset);
                RollbackWidget(widget_ctrl_ptr);
                ResetWidgetTpState(widget_ctrl_ptr);
                return TRUE;
            }            
        }
        else
        {
            MoveWidgetBarTpUp(widget_ctrl_ptr, layer_offsetX, FALSE);
            widget_ctrl_ptr->shortcut.is_shortcut_on = FALSE;              
        }
        DisplayWidgetShortcut(widget_ctrl_ptr); 
    }
    else
    if (   MMIWIDGET_TP_CLICK_IDLE_EMPTY == widget_ctrl_ptr->tp_move_info.click_type
        && MMIWIDGET_TP_MOVE_PAGE == widget_ctrl_ptr->widget_mov_type)
    {
        if (IsElveWidget(widget_ctrl_ptr->tp_move_info.click_widget_id))
        {
            SetElveStatus(widget_ctrl_ptr, MMIWIDGET_ELVE_NORMAL_DISPLAY);
        }   
        
        item_ptr = FindWidgetItem(widget_ctrl_ptr, widget_ctrl_ptr->tp_move_info.click_widget_id);

        if (PNULL != item_ptr && PNULL != item_ptr->item_info.ResetChildWinItem)
        {
           item_ptr->item_info.ResetChildWinItem();
        }
        HandleSlidePageTpUp(widget_ctrl_ptr, point);
        ResetWidgetTpState(widget_ctrl_ptr);
        return TRUE;
    }
    else if (MMI_WIDGET_TP_BAR_AREA == GetTpArea(widget_ctrl_ptr, point))
    {   
        //销毁widget图层
        DestroyWidgetLayer(widget_ctrl_ptr);
        SetBltLayer(widget_ctrl_ptr);
        
        //move on shortcut bar, check rollback
        if (   MMIWIDGET_TP_CLICK_SHORTCUT_WIDGET == widget_ctrl_ptr->tp_move_info.click_type
            || MMIWIDGET_TP_CLICK_SHORTCUT_EMPTY  == widget_ctrl_ptr->tp_move_info.click_type)
        {
            if (RollbackWidget(widget_ctrl_ptr))
            {
                ResetWidgetTpState(widget_ctrl_ptr);
                return TRUE;                    
            }
        }

        //move dynamic small widget icon on shortcut bar.
        if (MMIWIDGET_TP_CLICK_SHORTCUT_DYN_WIDGET == widget_ctrl_ptr->tp_move_info.click_type)
        {
            CalcAllShortcutItemPos(widget_ctrl_ptr, widget_ctrl_ptr->shortcut_offset);
            DisplayWidgetShortcut(widget_ctrl_ptr);              
        }
        
        //move from idle to shortcut, reset main layer.
        if (MMIWIDGET_TP_CLICK_ACTIVE_WIDGET == widget_ctrl_ptr->tp_move_info.click_type)
        {
            //reset to main layer
            ResetHitWidgetLayer(widget_ctrl_ptr);  
            CalcAllShortcutItemPos(widget_ctrl_ptr, widget_ctrl_ptr->shortcut_offset);
            DisplayWidgetShortcut(widget_ctrl_ptr);        
        }
        //UpdateAllScreen(); 

        //move dynamic small widget icon on shortcut bar.
        if (MMIWIDGET_TP_CLICK_SHORTCUT_DYN_WIDGET == widget_ctrl_ptr->tp_move_info.click_type)
        {       
            pos = GetShortcutItemPos(MMIWIDGET_CTRL_IDLE_ID, widget_ctrl_ptr->tp_move_info.click_widget_id);

            if (MMIWIDGET_INVALID_BAR_POS != pos)
            {
                VibrateWidgetItem(widget_ctrl_ptr);
            }        
        }
    }
    else
    // click up on shortcut button.
    if (   MMI_WIDGET_TP_BAR_BTN_AREA == GetTpArea(widget_ctrl_ptr, point)
        && (MMIWIDGET_TP_CLICK_SHORTCUT_BTN == widget_ctrl_ptr->tp_move_info.click_type))
    {
        //销毁widget图层
        DestroyWidgetLayer(widget_ctrl_ptr);

        MoveWidgetBarBtn(widget_ctrl_ptr, point);
       
        if (widget_ctrl_ptr->shortcut.is_shortcut_on && s_is_restart_idle_screen)
        {
            MMIWIDGET_SetShortcutOffset(MMIWIDGET_CTRL_IDLE_ID, 0);
            s_is_restart_idle_screen = FALSE;
            SlideWidgetOneByOne(widget_ctrl_ptr);
        }
        
        ResetWidgetTpState(widget_ctrl_ptr);
        return TRUE;        
    }
    else
    if (MMI_WIDGET_TP_BAR_EX_AREA == GetTpArea(widget_ctrl_ptr, point)) 
    {
        DestroyWidgetLayer(widget_ctrl_ptr);
        SetBltLayer(widget_ctrl_ptr); 
          
        //move dynamic small widget icon on shortcut bar.
        if (MMIWIDGET_TP_CLICK_SHORTCUT_DYN_WIDGET == widget_ctrl_ptr->tp_move_info.click_type)
        { 
            CalcAllShortcutItemPos(widget_ctrl_ptr, widget_ctrl_ptr->shortcut_offset);
            DisplayWidgetShortcut(widget_ctrl_ptr); 
            
            pos = GetShortcutItemPos(MMIWIDGET_CTRL_IDLE_ID, widget_ctrl_ptr->tp_move_info.click_widget_id);

            if (MMIWIDGET_INVALID_BAR_POS != pos)
            {
                VibrateWidgetItem(widget_ctrl_ptr);
            }        
        }        
    }
    else
    {   
        if (   MMIWIDGET_TP_CLICK_ACTIVE_WIDGET == widget_ctrl_ptr->tp_move_info.click_type
            && MMIWIDGET_TP_MOVE_IDLE_TO_IDLE != widget_ctrl_ptr->widget_mov_type)
        {
            old_win_rect.left   = 0;
            old_win_rect.top    = 0;
            old_win_rect.right  = MMITHEME_GetMainScreenRightPixel();
            old_win_rect.bottom = MMITHEME_GetMainScreenBottomPixel();

            ClearWidgetRect(widget_ctrl_ptr, old_win_rect);
        }

        //销毁widget图层
        DestroyWidgetLayer(widget_ctrl_ptr);
        
        //reset to main layer
        ResetHitWidgetLayer(widget_ctrl_ptr);

        if (IsElveWidget(widget_ctrl_ptr->tp_move_info.click_widget_id))
        {
            SetElveStatus(widget_ctrl_ptr, MMIWIDGET_ELVE_NORMAL_DISPLAY);
        }        

        if (IsElveWidgetActive(widget_ctrl_ptr))
        {
            SetZorderItemTopMostByWidgetId(widget_ctrl_ptr, MMIWIDGET_ELVE_ID);
        }
 
        if (MMIWIDGET_TP_MOVE_IDLE_TO_IDLE == widget_ctrl_ptr->widget_mov_type)
        {
            //让widget应用处理
            RunChildWidgetProc(widget_ctrl_ptr, widget_ctrl_ptr->tp_move_info.click_widget_id, MSG_WIDGET_TP_PRESS_LONG_UP, param);
        }
        else if (MMIWIDGET_TP_CLICK_IDLE_EMPTY == widget_ctrl_ptr->tp_move_info.click_type
                 && MMIWIDGET_INVALID_ID == widget_ctrl_ptr->tp_move_info.click_widget_id)
        {
            //click empty on idle to shrink or expand widget bar.
            DestroyWidgetLayer(widget_ctrl_ptr);
    
            MoveWidgetBarBtn(widget_ctrl_ptr, point);
           
            if (widget_ctrl_ptr->shortcut.is_shortcut_on && s_is_restart_idle_screen)
            {
                MMIWIDGET_SetShortcutOffset(MMIWIDGET_CTRL_IDLE_ID, 0);
                s_is_restart_idle_screen = FALSE;
                SlideWidgetOneByOne(widget_ctrl_ptr);
            }

            ResetWidgetTpState(widget_ctrl_ptr);
            return TRUE;
        }
        else if ((MMIWIDGET_TP_MOVE_PAGE != widget_ctrl_ptr->widget_mov_type)
           && (MMIWIDGET_TP_CLICK_ACTIVE_WIDGET != widget_ctrl_ptr->tp_move_info.click_type))
        {
            //让widget应用处理
            RunChildWidgetProc(widget_ctrl_ptr, widget_ctrl_ptr->tp_move_info.click_widget_id, MSG_WIDGET_TP_PRESS_UP, param);
            UpdateAllScreen();
        }

        if (   MMIWIDGET_TP_CLICK_ACTIVE_WIDGET == widget_ctrl_ptr->tp_move_info.click_type
            && MMIWIDGET_TP_MOVE_IDLE_TO_IDLE == widget_ctrl_ptr->widget_mov_type)
        {   
            if (!HandleActiveWidgetNum(widget_ctrl_ptr))
            {
                item_ptr = FindWidgetItem(widget_ctrl_ptr, widget_ctrl_ptr->tp_move_info.click_widget_id);
                
                if (!ResetChildWinPosition(widget_ctrl_ptr, point))
                {
                    RedrawWidgetItem(item_ptr);
                }
            }
        }
    }

    if (!RollbackWidget(widget_ctrl_ptr))
    {
        CalcAllShortcutItemPos(widget_ctrl_ptr, widget_ctrl_ptr->shortcut_offset);
    }

    ResetWidgetTpState(widget_ctrl_ptr);    
    UpdateAllScreen();
    MMI_CheckAllocatedMemInfo();
    return TRUE;
}

/*****************************************************************************/
//  Description : save widget info to NV
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIWIDGET_SaveWidgetInfoToNV(
                                 MMIWIDGET_CTRL_T *widget_ctrl_ptr
                                 )
{
    MMIWIDGET_SAVE_INFO_T *save_info = PNULL;
    uint32 i = 0;
    uint32 widget_id = MMIWIDGET_INVALID_ID;
    uint32 index = 0;
    MMIWIDGET_ITEM_T *item_ptr = PNULL;
    GUI_RECT_T win_rect = {0};

    if (PNULL == widget_ctrl_ptr || PNULL == widget_ctrl_ptr->zorder_item_ptr)
    {
        //SCI_TRACE_LOW:"SaveWidgetInfoToNV() PNULL == widget_ctrl_ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_3824_112_2_18_3_7_38_100,(uint8*)"");
        return FALSE;
    }

    save_info = (MMIWIDGET_SAVE_INFO_T*)SCI_ALLOC_APP(sizeof(MMIWIDGET_SAVE_INFO_T));

    if (PNULL == save_info)
    {
        return FALSE;
    }
    save_info->shortcut_offset = widget_ctrl_ptr->shortcut_offset;    
    save_info->widget_shortcut_list.item_num = widget_ctrl_ptr->shortcut.shortcut_num;
    for (i = 0; i < widget_ctrl_ptr->shortcut.shortcut_num; i++)
    {
        widget_id = widget_ctrl_ptr->shortcut.shortcut_item[i].widget_id;
        if (IsValidWidgetId(widget_id))
        {
            save_info->widget_shortcut_list.item_info[index++].widget_id = widget_id;
        }
    }
    index = 0;

    item_ptr = widget_ctrl_ptr->zorder_item_ptr->zorder_next_ptr;

    while (PNULL != item_ptr)
    {
        widget_id = item_ptr->item_info.widget_id;
        if (IsValidWidgetId(widget_id) && item_ptr->is_active)
        {
            MMK_GetWinRect(item_ptr->item_info.win_id, &win_rect);
            save_info->widget_active_list.item_info[index].widget_id = item_ptr->item_info.widget_id;
            if (widget_ctrl_ptr->is_main_screen_landscape)
            {
                //从当前屏（横屏）转换竖屏
                win_rect = RotateRect(win_rect, TRUE);
            }
            save_info->widget_active_list.item_info[index].save_pos.x = win_rect.left;
            save_info->widget_active_list.item_info[index].save_pos.y = win_rect.top;
            index++;
        }
        item_ptr = item_ptr->zorder_next_ptr;
    }
    
    save_info->widget_active_list.item_num = index;

    save_info->cur_page_index = widget_ctrl_ptr->cur_page_index;

    MMINV_WRITE(MMIWIDGET_SAVE_INFO, save_info);

    SCI_FREE(save_info);  
    save_info = PNULL;
          
    return TRUE;
}

/*****************************************************************************/
//  Description : 得到点中item
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN GetWidgetTpPressedItem(
                                 MMIWIDGET_CTRL_T *widget_ctrl_ptr,
                                 GUI_POINT_T point,
                                 uint32 *hit_widget_id_ptr
                                 )
{
    BOOLEAN result = FALSE;
    MMIWIDGET_ITEM_T *item_ptr = PNULL;
    GUI_RECT_T rect = {0};
    
    if (PNULL == widget_ctrl_ptr || PNULL == widget_ctrl_ptr->zorder_item_ptr)
    {
        //SCI_TRACE_LOW:"GetWidgetTpPressedItem() PNULL == widget_ctrl_ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_3895_112_2_18_3_7_39_101,(uint8*)"");
        return FALSE;
    }

    if (PNULL == hit_widget_id_ptr)
    {
        return result;
    }
    
    item_ptr = widget_ctrl_ptr->zorder_item_ptr;

    while (PNULL != item_ptr->zorder_next_ptr)
    {
        item_ptr = item_ptr->zorder_next_ptr;
    }

    //check from top to bottom.
    while ((PNULL != item_ptr) && (item_ptr != widget_ctrl_ptr->zorder_item_ptr) && item_ptr->is_active)
    {
        MMK_GetWinRect(item_ptr->item_info.win_id, &rect);
        
        if (GUI_PointIsInRect(point, rect))
        {
            //find
            result = TRUE;
            *hit_widget_id_ptr = item_ptr->item_info.widget_id;
            break;
        }

        item_ptr = item_ptr->zorder_prev_ptr;
    }

    return result;
}

/*****************************************************************************/
//  Description : handle widget tp down
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN ResetWidgetTPMoveInfo(
                                 MMIWIDGET_CTRL_T *widget_ctrl_ptr
                                 )
{
    if (PNULL == widget_ctrl_ptr)
    {
        //SCI_TRACE_LOW:"ResetWidgetTPMoveInfo() PNULL == widget_ctrl_ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_3940_112_2_18_3_7_39_102,(uint8*)"");
        return FALSE;
    }
    widget_ctrl_ptr->tp_move_info.click_type = MMIWIDGET_TP_CLICK_INVALID;
    widget_ctrl_ptr->tp_move_info.click_widget_id = MMIWIDGET_INVALID_ID;
    return TRUE;
}

/*****************************************************************************/
//  Description : handle widget tp down
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN SetWidgetTPClickType(
                                 MMIWIDGET_CTRL_T *widget_ctrl_ptr,
                                 MMIWIDGET_TP_CLICK_TYPE_E click_type
                                 )
{
    if (PNULL == widget_ctrl_ptr)
    {
        //SCI_TRACE_LOW:"SetWidgetTPHitType() PNULL == widget_ctrl_ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_3959_112_2_18_3_7_39_103,(uint8*)"");
        return FALSE;
    }

    widget_ctrl_ptr->tp_move_info.click_type = click_type;
    return TRUE;
}

/*****************************************************************************/
//  Description : move widget from shortcut bar to idle.
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN MoveWidgetBar2Idle(MMIWIDGET_CTRL_T *widget_ctrl_ptr, GUI_POINT_T point)
{
    uint32 pos = 0;
    MMI_HANDLE_T hit_win_handle = 0;
    GUI_RECT_T old_win_rect = {0};
    MMIWIDGET_ITEM_T *item_ptr = PNULL;
    GUI_RECT_T win_rect = {0};
    
    if (widget_ctrl_ptr->shortcut.shortcut_num < MMIWIDGET_MOVE_MIN_NUM)
    {
        return FALSE;
    }

    pos = GetShortcutItemPos(MMIWIDGET_CTRL_IDLE_ID, widget_ctrl_ptr->tp_move_info.click_widget_id);

    if (MMIWIDGET_INVALID_BAR_POS == pos)
    {
        return FALSE;
    }
    
    old_win_rect = GetShortcutItemRect(widget_ctrl_ptr, widget_ctrl_ptr->shortcut_offset, pos);   
    ClearWidgetRect(widget_ctrl_ptr, old_win_rect);

    SetWidgetTPClickType(widget_ctrl_ptr, MMIWIDGET_TP_CLICK_ACTIVE_WIDGET);
    
    RemoveShortcutById(widget_ctrl_ptr, widget_ctrl_ptr->tp_move_info.click_widget_id); 

    MMIWIDGET_SetShortcutOffset(MMIWIDGET_CTRL_IDLE_ID, widget_ctrl_ptr->shortcut_offset);

    DisplayDynWidgetShortcut(widget_ctrl_ptr, point);    

    ActiveWidgetItem(widget_ctrl_ptr, widget_ctrl_ptr->tp_move_info.click_widget_id);
  
    hit_win_handle = GetWinHandleByWidgetId(widget_ctrl_ptr, widget_ctrl_ptr->tp_move_info.click_widget_id);
    //MMK_GetWinRect(hit_win_handle, &old_win_rect);
    old_win_rect = GetChildWinRect(widget_ctrl_ptr, widget_ctrl_ptr->tp_move_info.click_widget_id);
    item_ptr = FindWidgetItem(widget_ctrl_ptr, widget_ctrl_ptr->tp_move_info.click_widget_id);

    win_rect.left   = point.x - (old_win_rect.right - old_win_rect.left)/2;
    win_rect.top    = point.y - (old_win_rect.bottom - old_win_rect.top)/2;
    win_rect.right  = win_rect.left + (old_win_rect.right - old_win_rect.left);
    win_rect.bottom = win_rect.top + (old_win_rect.bottom - old_win_rect.top);

    if (!IsElveWidget(widget_ctrl_ptr->tp_move_info.click_widget_id))
    {
        UILAYER_SetLayerPosition(&widget_ctrl_ptr->widget_layer, win_rect.left, win_rect.top);
    }
   
    //ClearWidgetRect(widget_ctrl_ptr, old_win_rect);

    //ChangeWidgetWinRect(widget_ctrl_ptr, widget_ctrl_ptr->tp_move_info.click_widget_id, widget_ctrl_ptr->save_point, point, FALSE);
	MMK_WinGrabFocus(hit_win_handle);
	//MMK_SendMsg(hit_win_handle, MSG_GET_FOCUS, NULL);
    SetChildWinRect(hit_win_handle, win_rect, widget_ctrl_ptr->is_main_screen_landscape);
    RedrawWidgetItem(item_ptr);

    if (IsElveWidget(widget_ctrl_ptr->tp_move_info.click_widget_id))
    {
        SetElveStatus(widget_ctrl_ptr, MMIWIDGET_ELVE_MOVING_DISPLAY);
        StartElveAnim(widget_ctrl_ptr, TRUE); 
    }

    widget_ctrl_ptr->tp_move_info.click_widget_bar_pos = pos;
    widget_ctrl_ptr->save_point = point;
    
    return TRUE;
}

/*****************************************************************************/
//  Description : move widget from idle to shortcut bar.
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN MoveWidgetIdle2Bar(MMIWIDGET_CTRL_T *widget_ctrl_ptr, GUI_POINT_T point)
{
    uint32 pos = 0;
    BOOLEAN result = FALSE;
    MMIWIDGET_ITEM_T *item_ptr = PNULL;
    
    if (MMIWIDGET_INVALID_ID == widget_ctrl_ptr->tp_move_info.click_widget_id)
    {
        return FALSE;
    }

    if (IsElveWidget(widget_ctrl_ptr->tp_move_info.click_widget_id))
    {
        StopElveAnim(widget_ctrl_ptr);   
    }
   
    if (DeactiveWidgetItem(widget_ctrl_ptr, widget_ctrl_ptr->tp_move_info.click_widget_id))
    {
        //add shortcut
        FindShortcutInsertPosByPoint(widget_ctrl_ptr, point, &pos);
       
        item_ptr = FindWidgetItem(widget_ctrl_ptr, widget_ctrl_ptr->tp_move_info.click_widget_id);

        if (PNULL != item_ptr)
        {
            result = InsertShortcut(
                          widget_ctrl_ptr,
                          item_ptr->item_info.small_icon_id,
                          widget_ctrl_ptr->tp_move_info.click_widget_id,
                          pos
                          );  
    
            if (result)
            {              
			    UILAYER_Clear(&widget_ctrl_ptr->widget_layer);
                CalcIdle2BarIconsPos(widget_ctrl_ptr, point);
                DisplayDynWidgetShortcut(widget_ctrl_ptr, point);  
            }
        }
    }
    return TRUE;
}

/*****************************************************************************/
//  Description : move widget from idle to shortcut bar by specified pos.
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN MoveWidgetIdle2BarByPos(MMIWIDGET_CTRL_T *widget_ctrl_ptr, uint32 pos)
{
    BOOLEAN result = FALSE;
    MMIWIDGET_ITEM_T *item_ptr = PNULL;
    
    if (MMIWIDGET_INVALID_ID == widget_ctrl_ptr->tp_move_info.click_widget_id)
    {
        return FALSE;
    }

    if (IsElveWidget(widget_ctrl_ptr->tp_move_info.click_widget_id))
    {
        StopElveAnim(widget_ctrl_ptr);   
    }
   
    if (DeactiveWidgetItem(widget_ctrl_ptr, widget_ctrl_ptr->tp_move_info.click_widget_id))
    {       
        item_ptr = FindWidgetItem(widget_ctrl_ptr, widget_ctrl_ptr->tp_move_info.click_widget_id);

        if (PNULL != item_ptr)
        {
            result = InsertShortcut(
                          widget_ctrl_ptr,
                          item_ptr->item_info.small_icon_id,
                          widget_ctrl_ptr->tp_move_info.click_widget_id,
                          pos
                          );  
    
            if (result)
            {              
			    UILAYER_Clear(&widget_ctrl_ptr->widget_layer);
                CalcAllShortcutItemPos(widget_ctrl_ptr, widget_ctrl_ptr->shortcut_offset);
                DisplayWidgetShortcut(widget_ctrl_ptr);
            }
        }
    }
    return TRUE;
}

/*****************************************************************************/
//  Description : move widget on shortcut bar.
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN MoveWidgetBar2Bar(MMIWIDGET_CTRL_T *widget_ctrl_ptr, GUI_POINT_T point)
{
    uint16 icon_width  = 0;
    uint16 icon_height = 0;
    int16 shortcut_offset      = 0;
    MMIWIDGET_ITEM_T *item_ptr = PNULL;
  
    if (widget_ctrl_ptr->shortcut.shortcut_num < MMIWIDGET_MOVE_MIN_NUM)
    {
        return FALSE;
    }
   
    item_ptr = FindWidgetItem(widget_ctrl_ptr, widget_ctrl_ptr->shortcut.shortcut_item[0].widget_id);

    if (PNULL == item_ptr)
    {
        return FALSE;
    }
    
    GUIRES_GetImgWidthHeight(&icon_width, &icon_height,  item_ptr->item_info.small_icon_id, widget_ctrl_ptr->win_id);
   
    if (   (point.y > widget_ctrl_ptr->save_point.y) 
        && (widget_ctrl_ptr->shortcut.shortcut_item_pos[widget_ctrl_ptr->shortcut.shortcut_num - 1].y 
        <  widget_ctrl_ptr->display_theme.shortcut_data.bar_data.shortcut_bar_rect.top))
    {
        return TRUE;
    }

    if (    (point.y < widget_ctrl_ptr->save_point.y)   
         && (widget_ctrl_ptr->shortcut.shortcut_item_pos[0].y + icon_height
         >   widget_ctrl_ptr->display_theme.shortcut_data.bar_data.shortcut_bar_rect.bottom))              
    {
        return TRUE;
    }
    
    shortcut_offset = widget_ctrl_ptr->shortcut_offset + (point.y - widget_ctrl_ptr->save_point.y); 
    MMIWIDGET_SetShortcutOffset(MMIWIDGET_CTRL_IDLE_ID, shortcut_offset);   
    widget_ctrl_ptr->save_point = point;
    DisplayWidgetShortcut(widget_ctrl_ptr);
    return TRUE;
}

/*****************************************************************************/
//  Description : move widget on shortcut bar.
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN MoveWidgetIconOnBar(MMIWIDGET_CTRL_T *widget_ctrl_ptr, GUI_POINT_T point)
{
    uint32 pos;
    GUI_RECT_T old_rect = {0};
    MMIWIDGET_ITEM_T *item_ptr = PNULL;

    item_ptr = FindWidgetItem(widget_ctrl_ptr, widget_ctrl_ptr->tp_move_info.click_widget_id);
    
    if (PNULL == item_ptr)
    {
        return FALSE;
    }

    pos = GetShortcutItemPos(MMIWIDGET_CTRL_IDLE_ID, widget_ctrl_ptr->tp_move_info.click_widget_id);

    if (MMIWIDGET_INVALID_BAR_POS == pos)
    {
        return FALSE;
    }
    old_rect = GetShortcutItemRect(widget_ctrl_ptr, widget_ctrl_ptr->shortcut_offset, pos);  

    UILAYER_Clear(&widget_ctrl_ptr->widget_layer);
    
    CalcDynamicIconsPosOnBar(widget_ctrl_ptr, point); 
    DisplayDynWidgetShortcut(widget_ctrl_ptr, point);
    return TRUE;
}

/*****************************************************************************/
//  Description : move widget on idle screen.
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN MoveWidgetIdle2Idle(MMIWIDGET_CTRL_T *widget_ctrl_ptr, GUI_POINT_T point)
{
    MMI_HANDLE_T click_win_handle = 0;
    GUI_RECT_T old_win_rect = {0};
    GUI_RECT_T win_rect = {0};
    MMIWIDGET_ITEM_T *item_ptr = PNULL;
    uint16 bg_width = 0;
    uint16 bg_height = 0;
    GUI_RECT_T main_rect  = MMITHEME_GetFullScreenRect();
    GUI_POINT_T offset_point = {0};
    int16 top_interval = MMIWIDGET_EDGE_DIST;    //顶端边缘
    int16 bottom_interval = (main_rect.bottom - main_rect.top - MMIWIDGET_EDGE_DIST);    //底端边缘

    click_win_handle = GetWinHandleByWidgetId(widget_ctrl_ptr, widget_ctrl_ptr->tp_move_info.click_widget_id);
    MMK_GetWinRect(click_win_handle, &old_win_rect);
    item_ptr = FindWidgetItem(widget_ctrl_ptr, widget_ctrl_ptr->tp_move_info.click_widget_id);

    if (PNULL != item_ptr)
    {   
        if (IsElveWidget(widget_ctrl_ptr->tp_move_info.click_widget_id))  
        {
            GUIRES_GetImgWidthHeight(&bg_width, &bg_height, item_ptr->item_info.large_bg_img, item_ptr->item_info.win_id);        
            win_rect.left = old_win_rect.left + (point.x - widget_ctrl_ptr->save_point.x);
            win_rect.top = old_win_rect.top + (point.y - widget_ctrl_ptr->save_point.y);
            win_rect.right = win_rect.left + bg_width -1;
            win_rect.bottom = win_rect.top + bg_height-1;   
            
            //清除原来位置上的widget
            ClearWidgetRect(widget_ctrl_ptr, old_win_rect);
            
            if (win_rect.bottom <= top_interval)
            {
                win_rect.bottom = top_interval;
                win_rect.top    = win_rect.bottom - bg_height;
                SetChildWinRect(click_win_handle, win_rect, widget_ctrl_ptr->is_main_screen_landscape);
            }
            else if (win_rect.top >= bottom_interval)
            {
                win_rect.top      = bottom_interval;
                win_rect.bottom   = win_rect.top + bg_height;
                SetChildWinRect(click_win_handle, win_rect, widget_ctrl_ptr->is_main_screen_landscape);
            }
            else
            {                  
                ChangeWidgetWinRect(widget_ctrl_ptr, widget_ctrl_ptr->tp_move_info.click_widget_id, widget_ctrl_ptr->save_point, point, TRUE);
            }

            RedrawWidgetItem(item_ptr);
        }
        else
        {
            UILAYER_GetLayerPosition(&widget_ctrl_ptr->widget_layer, &offset_point.x, &offset_point.y);
            offset_point.x += (point.x - widget_ctrl_ptr->save_point.x);
            offset_point.y += (point.y - widget_ctrl_ptr->save_point.y);
            UILAYER_SetLayerPosition(&widget_ctrl_ptr->widget_layer, offset_point.x, offset_point.y);

            win_rect.left = offset_point.x;
            win_rect.top  = offset_point.y;
            win_rect.right = win_rect.left + (old_win_rect.right - old_win_rect.left);
            win_rect.bottom = win_rect.top + (old_win_rect.bottom - old_win_rect.top);  
            SetChildWinRect(click_win_handle, win_rect, widget_ctrl_ptr->is_main_screen_landscape);            
        }
        widget_ctrl_ptr->save_point = point;
    } 

    return TRUE;
}

/*****************************************************************************/
//  Description : move widget on idle screen.
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN MoveWidgetBarBtnByPoint(MMIWIDGET_CTRL_T *widget_ctrl_ptr, GUI_POINT_T point)
{
    int32 pos = 0;
    int16 new_pos = 0;    
    int16 offset_x = 0;
    int16 offset_y = 0;
    int16 layer_offsetX = 0;
    int16 layer_offsetY = 0;
    
    //销毁widget图层
    DestroyWidgetLayer(widget_ctrl_ptr);
    //shortcut switch按钮为切换展开/收起状态
   
    UILAYER_GetLayerPosition(&widget_ctrl_ptr->bar_layer, &offset_x, &offset_y);

    pos = widget_ctrl_ptr->display_theme.shortcut_data.bar_data.shortcut_bar_rect.left - widget_ctrl_ptr->display_theme.shortcut_data.bar_data.shortcut_bar_rect.right - 1;    

    new_pos = offset_x + (point.x - widget_ctrl_ptr->save_point.x);
    
    if (widget_ctrl_ptr->shortcut.is_shortcut_on)
    {       
        if (new_pos > pos && new_pos < 0)
        {
            UILAYER_SetLayerPosition(&widget_ctrl_ptr->bar_layer, new_pos, offset_y);
            DisplayWidgetShortcut(widget_ctrl_ptr);
            MMITHEME_UpdateRect();        
        }
    }
    else
    {
        if (new_pos < 0 && new_pos > pos)
        {
            UILAYER_SetLayerPosition(&widget_ctrl_ptr->bar_layer, new_pos, offset_y);
            DisplayWidgetShortcut(widget_ctrl_ptr);
            
            MMITHEME_UpdateRect();
        }
    }

    widget_ctrl_ptr->save_point = point;

    UILAYER_GetLayerPosition(&widget_ctrl_ptr->bar_layer, &layer_offsetX, &layer_offsetY);

    if (layer_offsetX >= (widget_ctrl_ptr->display_theme.shortcut_data.bar_data.shortcut_bar_rect.left - widget_ctrl_ptr->display_theme.shortcut_data.bar_data.shortcut_bar_rect.right - 1)/2)
    {
       widget_ctrl_ptr->shortcut.is_shortcut_on = TRUE;
    }    

    return TRUE;
}

/*****************************************************************************/
//  Description : move widget on idle screen.
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN MoveWidgetBarTpUp(MMIWIDGET_CTRL_T *widget_ctrl_ptr, int16 offsetX, BOOLEAN is_open)
{
    int i = 0;
    int32 pos = 0;
    int16 offset_x = 0;
    int16 offset_y = 0;
    int16 new_pos = 0;
    
    //销毁widget图层
    DestroyWidgetLayer(widget_ctrl_ptr);
    //shortcut switch按钮为切换展开/收起状态
   
    UILAYER_GetLayerPosition(&widget_ctrl_ptr->bar_layer, &offset_x, &offset_y);

    pos = widget_ctrl_ptr->display_theme.shortcut_data.bar_data.shortcut_bar_rect.left - widget_ctrl_ptr->display_theme.shortcut_data.bar_data.shortcut_bar_rect.right - 1;    

    if (!is_open)
    {
        new_pos = pos;
        for (i = offsetX; i > pos; i -= 10)
        { 
            UILAYER_SetLayerPosition(&widget_ctrl_ptr->bar_layer, i, offset_y);
            DisplayWidgetShortcut(widget_ctrl_ptr);

            MMITHEME_UpdateRect();
            SCI_Sleep(10);
        }
    }
    else
    {
        new_pos = 0;
        for (i = offsetX; i < 0; i += 10)
        {
            UILAYER_SetLayerPosition(&widget_ctrl_ptr->bar_layer, i, offset_y);
            DisplayWidgetShortcut(widget_ctrl_ptr);
            
            MMITHEME_UpdateRect();
            SCI_Sleep(10);
        }
    }
   
    UILAYER_SetLayerPosition(&widget_ctrl_ptr->bar_layer, new_pos, offset_y);  
    DisplayWidgetShortcut(widget_ctrl_ptr); 
    return TRUE;
}

/*****************************************************************************/
//  Description : shirink widget bar.
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN ShrinkWidgetBar(MMIWIDGET_CTRL_T *widget_ctrl_ptr)
{
    int i = 0;
    int32 pos = 0;
    int16 offset_x = 0;
    int16 offset_y = 0;
    int16 new_pos = 0;
    
    if (widget_ctrl_ptr->shortcut.is_shortcut_on)
    {
        //销毁widget图层
        DestroyWidgetLayer(widget_ctrl_ptr);
        //shortcut switch按钮为切换展开/收起状态
       
        UILAYER_GetLayerPosition(&widget_ctrl_ptr->bar_layer, &offset_x, &offset_y);
    
        pos = widget_ctrl_ptr->display_theme.shortcut_data.bar_data.shortcut_bar_rect.left - widget_ctrl_ptr->display_theme.shortcut_data.bar_data.shortcut_bar_rect.right - 1;    
        
        new_pos = pos;
        
        for (i = 0; i > pos; i -= 10)
        { 
            UILAYER_SetLayerPosition(&widget_ctrl_ptr->bar_layer, i, offset_y);
            DisplayWidgetShortcut(widget_ctrl_ptr);

            MMITHEME_UpdateRect();
            SCI_Sleep(10);
        }
        
        widget_ctrl_ptr->shortcut.is_shortcut_on = !widget_ctrl_ptr->shortcut.is_shortcut_on;
        UILAYER_SetLayerPosition(&widget_ctrl_ptr->bar_layer, new_pos, offset_y); 
        DisplayWidgetShortcut(widget_ctrl_ptr);        
    }

    return TRUE;
}

/*****************************************************************************/
//  Description : move widget on idle screen.
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN MoveWidgetBarBtn(MMIWIDGET_CTRL_T *widget_ctrl_ptr, GUI_POINT_T point)
{
    int i = 0;
    int32 pos = 0;
    int16 offset_x = 0;
    int16 offset_y = 0;
    int16 new_pos = 0;
    
    //销毁widget图层
    DestroyWidgetLayer(widget_ctrl_ptr);
    //shortcut switch按钮为切换展开/收起状态
   
    UILAYER_GetLayerPosition(&widget_ctrl_ptr->bar_layer, &offset_x, &offset_y);

    pos = widget_ctrl_ptr->display_theme.shortcut_data.bar_data.shortcut_bar_rect.left - widget_ctrl_ptr->display_theme.shortcut_data.bar_data.shortcut_bar_rect.right - 1;    

    if (widget_ctrl_ptr->shortcut.is_shortcut_on)
    {
        new_pos = pos;
        for (i = 0; i > pos; i -= 10)
        { 
            UILAYER_SetLayerPosition(&widget_ctrl_ptr->bar_layer, i, offset_y);
            DisplayWidgetShortcut(widget_ctrl_ptr);

            MMITHEME_UpdateRect();
            SCI_Sleep(10);
        }
    }
    else
    {
        new_pos = 0;
        for (i = pos; i < 0; i += 10)
        {
            UILAYER_SetLayerPosition(&widget_ctrl_ptr->bar_layer, i, offset_y);
            DisplayWidgetShortcut(widget_ctrl_ptr);
            
            MMITHEME_UpdateRect();
            SCI_Sleep(10);
        }
    }

    widget_ctrl_ptr->shortcut.is_shortcut_on = !widget_ctrl_ptr->shortcut.is_shortcut_on;
    UILAYER_SetLayerPosition(&widget_ctrl_ptr->bar_layer, new_pos, offset_y); 
    DisplayWidgetShortcut(widget_ctrl_ptr);

    MMITHEME_UpdateRect();

    return TRUE;
}

/*****************************************************************************/
//  Description : handle all widget move action.
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void ProcessWidgetMove(MMIWIDGET_CTRL_T *widget_ctrl_ptr, GUI_POINT_T point, MMIWIDGET_TP_MOVE_TYPE_E move_type)
{
    switch(move_type)
    {
        case MMIWIDGET_TP_MOVE_BAR_TO_BAR:
            MoveWidgetBar2Bar(widget_ctrl_ptr, point);
            break;

        case MMIWIDGET_TP_MOVE_BAR_TO_IDLE:
            MoveWidgetBar2Idle(widget_ctrl_ptr, point);
            RunChildWidgetProc(widget_ctrl_ptr, widget_ctrl_ptr->tp_move_info.click_widget_id, MSG_WIDGET_MOVE_BAR_TO_IDLE, PNULL);
            break;

        case MMIWIDGET_TP_MOVE_IDLE_TO_BAR:
            MoveWidgetIdle2Bar(widget_ctrl_ptr, point);
            RunChildWidgetProc(widget_ctrl_ptr, widget_ctrl_ptr->tp_move_info.click_widget_id, MSG_WIDGET_MOVE_IDLE_TO_BAR, PNULL);
            break;

        case MMIWIDGET_TP_MOVE_IDLE_TO_IDLE:
            MoveWidgetIdle2Idle(widget_ctrl_ptr, point);
            break;

        case MMIWIDGET_TP_MOVE_ICON_ON_BAR:
            MoveWidgetIconOnBar(widget_ctrl_ptr, point);
            break;

        case MMIWIDGET_TP_MOVE_BAR_BTN:
            MoveWidgetBarBtnByPoint(widget_ctrl_ptr, point);
            break;

        case MMIWIDGET_TP_MOVE_PAGE:
            HandleSlidePageTpMove(widget_ctrl_ptr, point); 
            break;
            
        default:
            break;
    }
}

/*****************************************************************************/
//  Description : draw rect around child window.
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void DrawSelectedEdgeRect(
                                 MMIWIDGET_CTRL_T *widget_ctrl_ptr,
                                 GUI_RECT_T rect,
                                 GUI_LCD_DEV_INFO *lcd_dev_ptr
                                 )
{
    GUI_POINT_T point[4] = {0};

    point[0].x = rect.left   - MMIWIDGET_LONG_TP_PEN_WIDTH;
    point[0].y = rect.top    - MMIWIDGET_LONG_TP_PEN_WIDTH;
    point[1].x = rect.right  + MMIWIDGET_LONG_TP_PEN_WIDTH;
    point[1].y = rect.top    - MMIWIDGET_LONG_TP_PEN_WIDTH;
    point[2].x = rect.right  + MMIWIDGET_LONG_TP_PEN_WIDTH;
    point[2].y = rect.bottom + MMIWIDGET_LONG_TP_PEN_WIDTH;
    point[3].x = rect.left   - MMIWIDGET_LONG_TP_PEN_WIDTH;
    point[3].y = rect.bottom + MMIWIDGET_LONG_TP_PEN_WIDTH;

    LCD_DrawThickLineEx(lcd_dev_ptr, point[0].x, point[0].y, point[1].x, point[1].y, 1, MMIWIDGET_LONG_TP_PEN_WIDTH, GUILCD_PEN_STYLE_DASH_DOT);
    LCD_DrawThickLineEx(lcd_dev_ptr, point[1].x, point[1].y, point[2].x, point[2].y, 1, MMIWIDGET_LONG_TP_PEN_WIDTH, GUILCD_PEN_STYLE_DASH_DOT);
    LCD_DrawThickLineEx(lcd_dev_ptr, point[2].x, point[2].y, point[3].x, point[3].y, 1, MMIWIDGET_LONG_TP_PEN_WIDTH, GUILCD_PEN_STYLE_DASH_DOT);
    LCD_DrawThickLineEx(lcd_dev_ptr, point[3].x, point[3].y, point[0].x, point[0].y, 1, MMIWIDGET_LONG_TP_PEN_WIDTH, GUILCD_PEN_STYLE_DASH_DOT);    
}

/*****************************************************************************/
//  Description : handle widget tp long down
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN HandleWidgetTpLongDown(
                                 MMIWIDGET_CTRL_T *widget_ctrl_ptr,
                                 DPARAM                      param
                                 )
{
    uint32 click_widget_id = MMIWIDGET_INVALID_ID;
    GUI_POINT_T point = {0};
    MMI_HANDLE_T click_win_handle = 0;
    GUI_RECT_T main_rect = MMITHEME_GetFullScreenRect(); 
    GUI_RECT_T cur_rect = {0};

    point.x = MMK_GET_TP_X(param);
    point.y = MMK_GET_TP_Y(param);
    
    if (PNULL == widget_ctrl_ptr)
    {
        //SCI_TRACE_LOW:"IsValidShortcutOffset() PNULL == widget_ctrl_ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_4569_112_2_18_3_7_40_104,(uint8*)"");
        return FALSE;
    }

    if (   (MMIWIDGET_TP_MOVE_PAGE == widget_ctrl_ptr->widget_mov_type)
        || (MMIWIDGET_TP_MOVE_IDLE_TO_IDLE== widget_ctrl_ptr->widget_mov_type)
        || (MMIWIDGET_TP_MOVE_BAR_TO_IDLE== widget_ctrl_ptr->widget_mov_type))
    {
        return FALSE;
    }
    
    //shortcut展开响应switch按钮为收起
    if (   !IsPointInShortcutSwitchRegion(widget_ctrl_ptr, point)
        && !IsPointInShortcutRegion(widget_ctrl_ptr, point)
        && (point.y < (main_rect.bottom - main_rect.top)))    
    {
        if (GetWidgetTpPressedItem(widget_ctrl_ptr, point, &click_widget_id))
        {       
            if (MMIWIDGET_INVALID_ID != click_widget_id)
            {
                if (MMI_RESULT_TRUE == RunChildWidgetProc(widget_ctrl_ptr, click_widget_id, MSG_WIDGET_TP_PRESS_LONG_DOWN, param))
                {
                    return FALSE;
                }

                if (widget_ctrl_ptr->is_child_win_btn)
                {
                    return FALSE;
                }

                if (IsElveWidget(click_widget_id))
                {
                    UILAYER_Clear(&widget_ctrl_ptr->elve_layer);
                }
                
                SetWidgetTPClickType(widget_ctrl_ptr, MMIWIDGET_TP_CLICK_ACTIVE_WIDGET);
                
                //设置点中widget为focus窗口
                click_win_handle = GetWinHandleByWidgetId(widget_ctrl_ptr, click_widget_id);
                
                MMK_SetChildWinFocus(widget_ctrl_ptr->win_id, click_win_handle);
    
                widget_ctrl_ptr->tp_move_info.click_widget_id = click_widget_id;
                
                //创建widget层
                CreateWidgetLayer(widget_ctrl_ptr);
                //将选中widget移到最上面
                SetClickWidgetTopmost(widget_ctrl_ptr);
                SetZorderItemTopMostByWidgetId(widget_ctrl_ptr, widget_ctrl_ptr->tp_move_info.click_widget_id);

                MMK_GetWinRect(click_win_handle, &cur_rect);  
              
//                DrawSelectedEdgeRect(widget_ctrl_ptr, cur_rect, &widget_ctrl_ptr->widget_layer);
                
                SaveTpMovePoint(widget_ctrl_ptr, point);
            }
            else
            {
                widget_ctrl_ptr->tp_move_info.click_widget_id = MMIWIDGET_INVALID_ID;
                SetWidgetTPClickType(widget_ctrl_ptr, MMIWIDGET_TP_CLICK_INVALID);
                SaveTpMovePoint(widget_ctrl_ptr, point);
            }
            UpdateAllScreen();
        }
        else
        {
            widget_ctrl_ptr->tp_move_info.click_widget_id = MMIWIDGET_INVALID_ID;
            SetWidgetTPClickType(widget_ctrl_ptr, MMIWIDGET_TP_CLICK_IDLE_EMPTY); 
            SaveTpMovePoint(widget_ctrl_ptr, point);
        }
    }       
    return TRUE;
}
    
/*****************************************************************************/
//  Description : handle widget tp down
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN HandleWidgetTpDown(
                                 MMIWIDGET_CTRL_T *widget_ctrl_ptr,
                                 DPARAM            param
                                 )
{
    uint32 pos = 0;
    uint32 click_widget_id = MMIWIDGET_INVALID_ID;
    GUI_POINT_T point = {0};
    MMI_HANDLE_T child_win_handle = 0;
MMI_CheckAllocatedMemInfo();
    point.x = MMK_GET_TP_X(param);
    point.y = MMK_GET_TP_Y(param);
    
    if (PNULL == widget_ctrl_ptr)
    {
        //SCI_TRACE_LOW:"IsValidShortcutOffset() PNULL == widget_ctrl_ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_4662_112_2_18_3_7_40_105,(uint8*)"");
        return FALSE;
    }
    ResetWidgetState(widget_ctrl_ptr);
    
    widget_ctrl_ptr->tp_move_info.click_widget_id = MMIWIDGET_INVALID_ID;
    widget_ctrl_ptr->widget_mov_type              = MMIWIDGET_TP_MOVE_NONE;
    widget_ctrl_ptr->is_child_win_btn             = FALSE;
    widget_ctrl_ptr->tp_move_is_valid             = FALSE;
    
    //记录down时page layer,用来恢复page layer 
    if(UILAYER_IsLayerActive(&widget_ctrl_ptr->page_layer))
    {
	    UILAYER_GetLayerPosition(&widget_ctrl_ptr->page_layer, &widget_ctrl_ptr->page_layer_offset_x, &widget_ctrl_ptr->page_layer_offset_y);   
	}
	
    //shortcut展开响应switch按钮为收起
    if (MMI_WIDGET_TP_BAR_BTN_AREA == GetTpArea(widget_ctrl_ptr, point))
    {
        widget_ctrl_ptr->down_point = point;
        SetWidgetTPClickType(widget_ctrl_ptr, MMIWIDGET_TP_CLICK_SHORTCUT_BTN);
        SaveTpMovePoint(widget_ctrl_ptr, point);        
    }
    else if (MMI_WIDGET_TP_BAR_AREA == GetTpArea(widget_ctrl_ptr, point))
    {
        if (FindShortcutPosByPoint(widget_ctrl_ptr, point, &pos))
        {
            click_widget_id = widget_ctrl_ptr->shortcut.shortcut_item[pos].widget_id;
            
            if (MMIWIDGET_INVALID_ID != click_widget_id)
            {
                //widget_ctrl_ptr->down_point.x = MMIWIDGET_INT16_MIN_VALUE;
                widget_ctrl_ptr->down_point = point;
                widget_ctrl_ptr->shortcut.click_pos = pos;
                widget_ctrl_ptr->tp_move_info.click_widget_id = click_widget_id;            
                SetWidgetTPClickType(widget_ctrl_ptr, MMIWIDGET_TP_CLICK_SHORTCUT_WIDGET);

                CreateWidgetLayer(widget_ctrl_ptr);
                //将选中widget移到最上面
                SetClickWidgetTopmost(widget_ctrl_ptr);
            }
            else
            {
                //SCI_TRACE_LOW:"HandleWidgetTpDown() hit_widget_id == MMIWIDGET_INVALID_ID!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_4705_112_2_18_3_7_40_106,(uint8*)"");
            }
        }
        else
        {
            SetWidgetTPClickType(widget_ctrl_ptr, MMIWIDGET_TP_CLICK_SHORTCUT_EMPTY);
        }
        SaveTpMovePoint(widget_ctrl_ptr, point);
    }
    else
    if (MMI_WIDGET_TP_IDLE_AREA == GetTpArea(widget_ctrl_ptr, point))        
    {
        widget_ctrl_ptr->down_point = point;
        
        if (GetWidgetTpPressedItem(widget_ctrl_ptr, point, &click_widget_id))
        {       
            if (MMIWIDGET_INVALID_ID != click_widget_id)
            { 
                child_win_handle = GetWinHandleByWidgetId(widget_ctrl_ptr, click_widget_id);
                MMK_SetChildWinFocus(widget_ctrl_ptr->win_id, child_win_handle);
                
                if (IsElveWidget(click_widget_id))
                {
                    SetElveStatus(widget_ctrl_ptr, MMIWIDGET_ELVE_CLICK_DISPLAY);    
                }
                
                if (MMI_RESULT_FALSE == RunChildWidgetProc(widget_ctrl_ptr, click_widget_id, MSG_WIDGET_TP_PRESS_DOWN, param))
                {
                    widget_ctrl_ptr->tp_move_info.click_widget_id = click_widget_id;
                    SetWidgetTPClickType(widget_ctrl_ptr, MMIWIDGET_TP_CLICK_IDLE_EMPTY); 
                }
                else
                {
                    widget_ctrl_ptr->tp_move_info.click_widget_id = click_widget_id;
                    widget_ctrl_ptr->is_child_win_btn           = TRUE;   
                    SetWidgetTPClickType(widget_ctrl_ptr, MMIWIDGET_TP_CLICK_INVALID); 
                    UpdateAllScreen();
                }
                SetZorderItemTopMostByWidgetId(widget_ctrl_ptr, click_widget_id);
                SaveTpMovePoint(widget_ctrl_ptr, point);
            }
            else
            {
                widget_ctrl_ptr->tp_move_info.click_widget_id = MMIWIDGET_INVALID_ID;
                SetWidgetTPClickType(widget_ctrl_ptr, MMIWIDGET_TP_CLICK_INVALID);
                SaveTpMovePoint(widget_ctrl_ptr, point);
            }
            //UpdateAllScreen();
        }
        else
        {
            widget_ctrl_ptr->tp_move_info.click_widget_id = MMIWIDGET_INVALID_ID;
            SetWidgetTPClickType(widget_ctrl_ptr, MMIWIDGET_TP_CLICK_IDLE_EMPTY); 
            SaveTpMovePoint(widget_ctrl_ptr, point);
        }
    }    
	MMI_CheckAllocatedMemInfo();
    return TRUE;
}

/*****************************************************************************/
//  Description : redraw widget item
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN RedrawWidgetItem(
                               MMIWIDGET_ITEM_T *item_ptr
                               )
{
    BOOLEAN result = FALSE;
    MMIWIDGET_CTRL_T *widget_ctrl_ptr = MMIAPIWIDGET_GetWidgetPtr(MMIWIDGET_CTRL_IDLE_ID);

    if (PNULL != item_ptr)
    {
        MMK_SendMsg(item_ptr->item_info.win_id, MSG_FULL_PAINT, PNULL);

        if (   UILAYER_IsLayerActive(&widget_ctrl_ptr->widget_layer) 
            && (widget_ctrl_ptr->tp_move_info.click_widget_id == item_ptr->item_info.widget_id)
            && MMIWIDGET_ELVE_ID != item_ptr->item_info.widget_id)
        {
            UILAYER_WeakLayerAlpha(&widget_ctrl_ptr->widget_layer, 127);
        }
        
        result = TRUE;
    }
    else
    {
        //SCI_TRACE_LOW:"RedrawWidgetItem() item_ptr == PNULL!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_4790_112_2_18_3_7_40_107,(uint8*)"");
    }
    return result;
}

/*****************************************************************************/
//  Description : 是否有效偏移（至少有一个item在可视范围内）
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsValidShortcutOffset(
                                 MMIWIDGET_CTRL_T *widget_ctrl_ptr,
                                 int16 shortcut_offset
                                 )
{
    BOOLEAN result = TRUE;
    GUI_RECT_T first_rect = {0};
    GUI_RECT_T last_rect = {0};

    if (PNULL == widget_ctrl_ptr)
    {
        //SCI_TRACE_LOW:"IsValidShortcutOffset() PNULL == widget_ctrl_ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_4810_112_2_18_3_7_40_108,(uint8*)"");
        return FALSE;
    }
    first_rect = GetShortcutItemRect(widget_ctrl_ptr, shortcut_offset, 0);
    last_rect = GetShortcutItemRect(widget_ctrl_ptr, shortcut_offset, (widget_ctrl_ptr->shortcut.shortcut_num-1));
    if ((first_rect.bottom >= widget_ctrl_ptr->display_theme.shortcut_data.bar_data.shortcut_bar_rect.bottom) 
        || (last_rect.top <= widget_ctrl_ptr->display_theme.shortcut_data.bar_data.shortcut_bar_rect.top))
    {
        result = FALSE;
    }
    return result;
}

/*****************************************************************************/
//  Description : get tp area
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL MMIWIDGET_TP_AREA_E GetTpArea(MMIWIDGET_CTRL_T *widget_ctrl_ptr, GUI_POINT_T point)
{
    MMIWIDGET_TP_AREA_E tp_area = MMI_WIDGET_INVALID_AREA;
    GUI_RECT_T main_rect        = MMITHEME_GetFullScreenRect();    
    
    if (IsPointInShortcutRegion(widget_ctrl_ptr, point))
    {
        tp_area = MMI_WIDGET_TP_BAR_AREA;
    }
    else
    if (IsPointInShortcutSwitchRegion(widget_ctrl_ptr, point))
    {
        tp_area = MMI_WIDGET_TP_BAR_BTN_AREA;
    }
    else
    if (IsPointInShortcutExtRegion(widget_ctrl_ptr, point))
    {
        tp_area = MMI_WIDGET_TP_BAR_EX_AREA;
    }
    else
    if (point.y < (main_rect.bottom - main_rect.top))
    {
        tp_area = MMI_WIDGET_TP_IDLE_AREA;    
    }

    return tp_area;
}

/*****************************************************************************/
//  Description : tp position for moving along X direction is valid or not.
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsTpXMoveValid(MMIWIDGET_CTRL_T *widget_ctrl_ptr, GUI_POINT_T point)
{
    if (ABS(widget_ctrl_ptr->down_point.x - point.x) >= MMIWIDGET_MOVE_X_MIN_PIX)
    {
       return TRUE;
    }

    return FALSE;
}

/*****************************************************************************/
//  Description : tp position for moving along Y direction is valid or not.
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsTpYMoveValid(MMIWIDGET_CTRL_T *widget_ctrl_ptr, GUI_POINT_T point)
{
    if (ABS(widget_ctrl_ptr->down_point.y - point.y) >= MMIWIDGET_MOVE_Y_MIN_PIX)
    {
       return TRUE;
    }

    return FALSE;
}

/*****************************************************************************/
//  Description : judge auto slide or not.
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void HandleWidgetAutoSlideEvent(MMIWIDGET_CTRL_T *widget_ctrl_ptr, GUI_POINT_T point)
{
    GUI_RECT_T widget_rect = {0};
    uint16 slide_edge = 0;
    
    //SCI_ASSERT(PNULL != widget_ctrl_ptr);

    if (PNULL == widget_ctrl_ptr)
    {
        //SCI_TRACE_LOW:"HandleWidgetAutoSlideEvent() PNULL == widget_ctrl_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_4896_112_2_18_3_7_41_109,(uint8*)"");
        return;
    }
   MMI_CheckAllocatedMemInfo();
    widget_rect = GetWidgetCtrlRect(widget_ctrl_ptr);
    slide_edge = widget_ctrl_ptr->display_theme.slide_edge;

    if (point.x < slide_edge)   // left
    {   
        s_is_auto_slide = TRUE;
    }
    else if(point.x + slide_edge > widget_rect.right)   // right
    {
        s_is_auto_slide = TRUE;
    }
    else
    {
        s_is_auto_slide = FALSE;
    }
	MMI_CheckAllocatedMemInfo();
}

/*****************************************************************************/
//  Description : handle widget tp move
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN HandleWidgetTpMove(
                                 MMIWIDGET_CTRL_T *widget_ctrl_ptr,
                                 DPARAM            param
                                 )
{
    GUI_POINT_T point = {0};

    if (PNULL == widget_ctrl_ptr)
    {
        //SCI_TRACE_LOW:"HandleWidgetTpMove() PNULL == widget_ctrl_ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_4930_112_2_18_3_7_41_110,(uint8*)"");
        return FALSE;
    }

    point.x = MMK_GET_TP_X(param);
    point.y = MMK_GET_TP_Y(param);
           
    if(widget_ctrl_ptr->is_child_win_btn)
    {
        widget_ctrl_ptr->is_child_win_tp_move = FALSE;
		
    	if (RunChildWidgetProc(widget_ctrl_ptr, widget_ctrl_ptr->tp_move_info.click_widget_id, MSG_WIDGET_TP_PRESS_MOVE, param))
        {
    	    MMITHEME_UpdateRect();
			widget_ctrl_ptr->is_child_win_tp_move = TRUE;
        }
    }  

    if (  (MMI_WIDGET_TP_IDLE_AREA == GetTpArea(widget_ctrl_ptr, point)
        || MMI_WIDGET_TP_BAR_BTN_AREA == GetTpArea(widget_ctrl_ptr, point))
        && !widget_ctrl_ptr->tp_move_is_valid)
    {
        if (!IsTpXMoveValid(widget_ctrl_ptr, point) && !IsTpYMoveValid(widget_ctrl_ptr, point))
        {
            return FALSE;
        }

        if(widget_ctrl_ptr->is_child_win_btn && !widget_ctrl_ptr->is_child_win_tp_move)
        {
           return FALSE;
        }

        widget_ctrl_ptr->tp_move_is_valid = TRUE;
    }
    
    //stop the long press timer to move something.
    StopTpLongPressTimer();
    
    if (MMIWIDGET_TP_CLICK_SHORTCUT_BTN == widget_ctrl_ptr->tp_move_info.click_type)
    {
        widget_ctrl_ptr->widget_mov_type = MMIWIDGET_TP_MOVE_BAR_BTN;
        ProcessWidgetMove(widget_ctrl_ptr, point, widget_ctrl_ptr->widget_mov_type);  
    }
    else if (MMIWIDGET_TP_CLICK_IDLE_EMPTY == widget_ctrl_ptr->tp_move_info.click_type)
    {
        //滑页效果
        widget_ctrl_ptr->widget_mov_type = MMIWIDGET_TP_MOVE_PAGE;
        ProcessWidgetMove(widget_ctrl_ptr, point, widget_ctrl_ptr->widget_mov_type);           
    }
    else 
    {
        if (MMI_WIDGET_TP_BAR_AREA == GetTpArea(widget_ctrl_ptr, point))
        {      
            //now you can move icon on bar directly, needn't to move it to idle then move back on bar.
            if (   MMIWIDGET_INVALID_ID != widget_ctrl_ptr->tp_move_info.click_widget_id   
                && MMIWIDGET_TP_MOVE_NONE == widget_ctrl_ptr->widget_mov_type)
            {
                if (IsTpYMoveValid(widget_ctrl_ptr, point))
                {
                   widget_ctrl_ptr->tp_move_info.click_type = MMIWIDGET_TP_CLICK_SHORTCUT_WIDGET;
                }
                else
                if (IsTpXMoveValid(widget_ctrl_ptr, point))
                {
                   widget_ctrl_ptr->tp_move_info.click_type = MMIWIDGET_TP_CLICK_SHORTCUT_DYN_WIDGET;
                }               
                else
                {
                   return FALSE;
                }
            }
            
            //move from shortcut bar to shortcut bar
            if (   MMIWIDGET_TP_CLICK_SHORTCUT_WIDGET == widget_ctrl_ptr->tp_move_info.click_type
                || MMIWIDGET_TP_CLICK_SHORTCUT_EMPTY  == widget_ctrl_ptr->tp_move_info.click_type)
            {                   
                 widget_ctrl_ptr->widget_mov_type = MMIWIDGET_TP_MOVE_BAR_TO_BAR;
                 ProcessWidgetMove(widget_ctrl_ptr, point, widget_ctrl_ptr->widget_mov_type);
            }

            if (   MMIWIDGET_TP_CLICK_SHORTCUT_DYN_WIDGET == widget_ctrl_ptr->tp_move_info.click_type)
            {
                 widget_ctrl_ptr->widget_mov_type = MMIWIDGET_TP_MOVE_ICON_ON_BAR;
                 ProcessWidgetMove(widget_ctrl_ptr, point, widget_ctrl_ptr->widget_mov_type);           
            }

            //move from idle widget to shortcut bar.
            if (MMIWIDGET_TP_CLICK_ACTIVE_WIDGET == widget_ctrl_ptr->tp_move_info.click_type)
            {                
                 widget_ctrl_ptr->widget_mov_type = MMIWIDGET_TP_MOVE_IDLE_TO_BAR;
                 ProcessWidgetMove(widget_ctrl_ptr, point, widget_ctrl_ptr->widget_mov_type);                 
                 SetWidgetTPClickType(widget_ctrl_ptr, MMIWIDGET_TP_CLICK_SHORTCUT_DYN_WIDGET);
            }
        }
        else if (MMI_WIDGET_TP_BAR_EX_AREA == GetTpArea(widget_ctrl_ptr, point))
        {
             //move from idle widget to idle widget.
            if (MMIWIDGET_TP_CLICK_ACTIVE_WIDGET == widget_ctrl_ptr->tp_move_info.click_type)
            {
                 if (!widget_ctrl_ptr->is_child_win_btn) 
                 {
                     widget_ctrl_ptr->widget_mov_type = MMIWIDGET_TP_MOVE_IDLE_TO_IDLE;
                     ProcessWidgetMove(widget_ctrl_ptr, point, widget_ctrl_ptr->widget_mov_type);   
                 }
            }        
        }
        else 
        if (MMI_WIDGET_TP_IDLE_AREA == GetTpArea(widget_ctrl_ptr, point)
            || MMI_WIDGET_TP_BAR_BTN_AREA == GetTpArea(widget_ctrl_ptr, point))
        {         
            //process child window moving.
            if (MMIWIDGET_INVALID_ID == widget_ctrl_ptr->tp_move_info.click_widget_id)
            {
                return FALSE;
            }

            //move from shortcut bar to idle.
            if ( /*  (MMIWIDGET_TP_MOVE_BAR_TO_BAR != widget_ctrl_ptr->widget_mov_type)
                && */(MMIWIDGET_TP_CLICK_SHORTCUT_WIDGET == widget_ctrl_ptr->tp_move_info.click_type
                || MMIWIDGET_TP_CLICK_SHORTCUT_DYN_WIDGET == widget_ctrl_ptr->tp_move_info.click_type))
            {
                widget_ctrl_ptr->widget_mov_type = MMIWIDGET_TP_MOVE_BAR_TO_IDLE;
                ProcessWidgetMove(widget_ctrl_ptr, point, widget_ctrl_ptr->widget_mov_type);  
                SetWidgetTPClickType(widget_ctrl_ptr, MMIWIDGET_TP_CLICK_ACTIVE_WIDGET);
            }

            //move from idle widget to idle widget.
            if (MMIWIDGET_TP_CLICK_ACTIVE_WIDGET == widget_ctrl_ptr->tp_move_info.click_type)
            {
                if (!widget_ctrl_ptr->is_child_win_btn) 
                {
                    if (IsElveWidget(widget_ctrl_ptr->tp_move_info.click_widget_id))
                    {
                        SetElveStatus(widget_ctrl_ptr, MMIWIDGET_ELVE_MOVING_DISPLAY);
                    }
                     
                    HandleWidgetAutoSlideEvent(widget_ctrl_ptr, point);
                    widget_ctrl_ptr->widget_mov_type = MMIWIDGET_TP_MOVE_IDLE_TO_IDLE;
                    ProcessWidgetMove(widget_ctrl_ptr, point, widget_ctrl_ptr->widget_mov_type);
                }
            }          
        }
    }
    
    return TRUE;
}

/*****************************************************************************/
//  Description : set widget active
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
LOCAL void SetWidgetActive(
                           MMIWIDGET_CTRL_T *widget_ctrl_ptr,
                           uint32 widget_id,
                           BOOLEAN is_active
                           )
{
    MMIWIDGET_ITEM_T *item_ptr = PNULL; 

    if (PNULL == widget_ctrl_ptr)
    {
        //SCI_TRACE_LOW:"SetWidgetActive() PNULL == widget_ctrl_ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_5091_112_2_18_3_7_41_111,(uint8*)"");
        return;
    }
    item_ptr = FindWidgetItem(widget_ctrl_ptr, widget_id);
    
    if (PNULL != item_ptr)
    {
        item_ptr->is_active = is_active;
    }
}

/*****************************************************************************/
//  Description : set widget layer blt
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
LOCAL void SetBltLayer(
                          MMIWIDGET_CTRL_T *widget_ctrl_ptr
                          )
{
    UILAYER_APPEND_BLT_T append_layer = {0};

    if (PNULL == widget_ctrl_ptr)
    {
        return;
    }

    if (UILAYER_IsLayerActive(&widget_ctrl_ptr->wallpaper_layer))
    {
        append_layer.lcd_dev_info = widget_ctrl_ptr->wallpaper_layer;
        append_layer.layer_level = UILAYER_LEVEL_LOW;
        UILAYER_AppendBltLayer(&append_layer);   
    }
    
    if (UILAYER_IsLayerActive(&widget_ctrl_ptr->page_layer))
    {
        append_layer.lcd_dev_info = widget_ctrl_ptr->page_layer;
        append_layer.layer_level = UILAYER_LEVEL_LOW;
        UILAYER_AppendBltLayer(&append_layer);   
    }
    
    if (IsElveWidgetActive(widget_ctrl_ptr))
    {
        append_layer.lcd_dev_info = widget_ctrl_ptr->elve_layer;
        append_layer.layer_level = UILAYER_LEVEL_NORMAL;
        UILAYER_AppendBltLayer(&append_layer);    
    }
    append_layer.lcd_dev_info = widget_ctrl_ptr->bar_layer;
    append_layer.layer_level = UILAYER_LEVEL_NORMAL;
    UILAYER_AppendBltLayer(&append_layer);    

    append_layer.lcd_dev_info = widget_ctrl_ptr->stb_layer;
    append_layer.layer_level = UILAYER_LEVEL_NORMAL;
    UILAYER_AppendBltLayer(&append_layer);  

    append_layer.lcd_dev_info = widget_ctrl_ptr->softkey_layer;
    append_layer.layer_level = UILAYER_LEVEL_NORMAL;
    UILAYER_AppendBltLayer(&append_layer);    
    append_layer.lcd_dev_info = widget_ctrl_ptr->title_layer;
    append_layer.layer_level = UILAYER_LEVEL_NORMAL;
    UILAYER_AppendBltLayer(&append_layer);    
    
    append_layer.lcd_dev_info = widget_ctrl_ptr->widget_layer;
    append_layer.layer_level = UILAYER_LEVEL_NORMAL;
    UILAYER_AppendBltLayer(&append_layer);    
    append_layer.lcd_dev_info = s_widget_application_layer;
    append_layer.layer_level = UILAYER_LEVEL_NORMAL;
    UILAYER_AppendBltLayer(&append_layer);
}

/*****************************************************************************/
//  Description : set widget layer topmost
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN SetClickWidgetTopmost(
                                  MMIWIDGET_CTRL_T *widget_ctrl_ptr
                                  )
{
    MMI_HANDLE_T hit_win_handle = 0;
    
    if (PNULL == widget_ctrl_ptr)
    {
        //SCI_TRACE_LOW:"SetHitWidgetTopmost() PNULL == widget_ctrl_ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_5172_112_2_18_3_7_41_112,(uint8*)"");
        return FALSE;
    }
    SetBltLayer(widget_ctrl_ptr);
    //设置选中widget对应子窗口所在层在widget_layer
    hit_win_handle = GetWinHandleByWidgetId(widget_ctrl_ptr, widget_ctrl_ptr->tp_move_info.click_widget_id);
    ResetOneWidgetLayer(widget_ctrl_ptr, widget_ctrl_ptr->tp_move_info.click_widget_id);
    
    return TRUE;
}

/*****************************************************************************/
//  Description : is the item the top item in zorder.
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsTopItemInZorder(MMIWIDGET_CTRL_T *widget_ctrl_ptr, MMIWIDGET_ITEM_T *item_ptr)
{
    BOOLEAN result = FALSE;
    MMIWIDGET_ITEM_T *cur_item_ptr = PNULL;

    if (PNULL == widget_ctrl_ptr || PNULL == item_ptr || PNULL == widget_ctrl_ptr->zorder_item_ptr)
    {
        return result;
    }
    
    cur_item_ptr = widget_ctrl_ptr->zorder_item_ptr;

    while (PNULL != cur_item_ptr->zorder_next_ptr)
    {
        cur_item_ptr = cur_item_ptr->zorder_next_ptr;
    }

    if (cur_item_ptr->item_info.widget_id == item_ptr->item_info.widget_id)
    {
        result = TRUE;
    }

    return result;
}   

/*****************************************************************************/
//  Description : remove zorder item
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN RemoveZorderItem(
                            MMIWIDGET_CTRL_T *widget_ctrl_ptr,
                            MMIWIDGET_ITEM_T *item_ptr
                            )
{
    BOOLEAN result = FALSE;
    MMIWIDGET_ITEM_T *cur_item_ptr = PNULL;

    if (PNULL == widget_ctrl_ptr || PNULL == item_ptr || PNULL == widget_ctrl_ptr->zorder_item_ptr)
    {
        return result;
    }
    
    cur_item_ptr = widget_ctrl_ptr->zorder_item_ptr->zorder_next_ptr;

    while (PNULL != cur_item_ptr)
    {
        if (cur_item_ptr->item_info.widget_id == item_ptr->item_info.widget_id)
        {
            result = TRUE;
            break;
        }

        cur_item_ptr = cur_item_ptr->zorder_next_ptr;
    }

    if (result && PNULL != cur_item_ptr)
    {
        if (PNULL == cur_item_ptr->zorder_next_ptr)
        {
            cur_item_ptr->zorder_prev_ptr->zorder_next_ptr = PNULL;
        }
        else
        {
            cur_item_ptr->zorder_prev_ptr->zorder_next_ptr = item_ptr->zorder_next_ptr;
            item_ptr->zorder_next_ptr->zorder_prev_ptr = cur_item_ptr->zorder_prev_ptr;        
        }

        item_ptr->zorder_next_ptr = PNULL;
        item_ptr->zorder_prev_ptr = PNULL;
    }

    return result;
}

/*****************************************************************************/
//  Description : check the widget id to add is exist in shorcut or not.
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsExistInShortcutSys(MMIWIDGET_CTRL_T *widget_ctrl_ptr, uint32 widget_id)
{
    uint32 i = 0;
    
    while (i < widget_ctrl_ptr->shortcut.shortcut_num)
    {
        if (widget_ctrl_ptr->shortcut.shortcut_item[i].widget_id == widget_id)
        {
            return TRUE;
        }
        i++;
    }

    return FALSE;
}

/*****************************************************************************/
//  Description : check the widget id to add is exist in zorder system or not.
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsExistInZorderSys(MMIWIDGET_CTRL_T *widget_ctrl_ptr, uint32 widget_id)
{
    MMIWIDGET_ITEM_T *item_ptr = PNULL;
    
    item_ptr = widget_ctrl_ptr->zorder_item_ptr->zorder_next_ptr;

    while (PNULL != item_ptr)
    {
        if (item_ptr->item_info.widget_id == widget_id)
        {
            return TRUE;
        }

        item_ptr = item_ptr->zorder_next_ptr;     
    }  

    return FALSE;
}

/*****************************************************************************/
//  Description : add item topmost
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN AddZorderItemTopmost(
                          MMIWIDGET_CTRL_T *widget_ctrl_ptr,
                          MMIWIDGET_ITEM_T *item_ptr
                          )
{
    BOOLEAN result = FALSE;
    MMIWIDGET_ITEM_T *widget_item_ptr = PNULL;

    if (PNULL == widget_ctrl_ptr)
    {
        //SCI_TRACE_LOW:"AddZorderItemTopmost() PNULL == widget_ctrl_ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_5318_112_2_18_3_7_41_113,(uint8*)"");
        return result;
    }
    
    if (PNULL == item_ptr)
    {
        return result;
    }

    if (PNULL == widget_ctrl_ptr->zorder_item_ptr->zorder_next_ptr)
    {
        widget_ctrl_ptr->zorder_item_ptr->zorder_next_ptr = item_ptr;
        item_ptr->zorder_prev_ptr = widget_ctrl_ptr->zorder_item_ptr;
        item_ptr->zorder_next_ptr = PNULL;
        result = TRUE;
    }
    else
    {
        if (IsExistInZorderSys(widget_ctrl_ptr, item_ptr->item_info.widget_id))
        {
            return FALSE;
        }

        widget_item_ptr = widget_ctrl_ptr->zorder_item_ptr;

        while (PNULL != widget_item_ptr->zorder_next_ptr)
        {
            widget_item_ptr =  widget_item_ptr->zorder_next_ptr;
        }

        widget_item_ptr->zorder_next_ptr = item_ptr;
        item_ptr->zorder_prev_ptr = widget_item_ptr;
        item_ptr->zorder_next_ptr = PNULL;

        result = TRUE;
    }
  
    return result;
}

/*****************************************************************************/
//  Description : set item topmost
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
LOCAL void SetZorderItemTopMostByWidgetId(
                          MMIWIDGET_CTRL_T *widget_ctrl_ptr,
                          uint32 widget_id
                          )
{
    MMIWIDGET_ITEM_T *item_ptr = PNULL; 

    if (PNULL == widget_ctrl_ptr)
    {
        //SCI_TRACE_LOW:"SetZorderItemTopMostByWidgetId() PNULL == widget_ctrl_ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_5371_112_2_18_3_7_41_114,(uint8*)"");
        return;
    }

    item_ptr = FindWidgetItem(widget_ctrl_ptr, widget_id);
    if (PNULL != item_ptr)
    {
        SetZorderItemTopMost(widget_ctrl_ptr, item_ptr);
    }
}

/*****************************************************************************/
//  Description : set item topmost
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
LOCAL void SetZorderItemTopMost(
                          MMIWIDGET_CTRL_T *widget_ctrl_ptr,
                          MMIWIDGET_ITEM_T *item_ptr
                          )
{
    if (PNULL == widget_ctrl_ptr || PNULL == item_ptr)
    {
        //SCI_TRACE_LOW:"SetZorderItemTopMost() PNULL == widget_ctrl_ptr || PNULL == item_ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_5393_112_2_18_3_7_42_115,(uint8*)"");
        return;
    }

    if (IsTopItemInZorder(widget_ctrl_ptr, item_ptr))
    {
        return;
    }
    //CheckTotalWidgetNum(widget_ctrl_ptr);
    RemoveZorderItem(widget_ctrl_ptr, item_ptr);
    AddZorderItemTopmost(widget_ctrl_ptr, item_ptr);
    //CheckTotalWidgetNum(widget_ctrl_ptr);
}

/*****************************************************************************/
//  Description : redraw hit icon in shortcut bar
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
LOCAL void RedrawHitIcon(
                         MMIWIDGET_CTRL_T *widget_ctrl_ptr,
                         GUI_POINT_T point
                         )
{    
    GUI_LCD_DEV_INFO lcd_dev_info = {0};
    GUI_RECT_T img_rect = {0};
    GUI_RECT_T disp_rect = {0};
    uint16 shortcut_item_width = 0;
    uint16 shortcut_item_height = 0;
    MMI_HANDLE_T hit_win_handle = 0;
    GUI_RECT_T win_rect = {0};
    MMIWIDGET_ITEM_T *widget_item_ptr = PNULL;     
    
    if (PNULL == widget_ctrl_ptr)
    {
        //SCI_TRACE_LOW:"RedrawHitIcon() PNULL == widget_ctrl_ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_5427_112_2_18_3_7_42_116,(uint8*)"");
        return;
    }

    //widget应用如果打开新窗口则不用刷新
    if (!MMK_IsFocusWin(widget_ctrl_ptr->win_id))
    {
        return;
    }

    widget_item_ptr = FindWidgetItem(widget_ctrl_ptr, widget_ctrl_ptr->tp_move_info.click_widget_id);   
    if (PNULL == widget_item_ptr)
    {
        return;
    }
    hit_win_handle = GetWinHandleByWidgetId(widget_ctrl_ptr, widget_ctrl_ptr->tp_move_info.click_widget_id);
    GUIRES_GetImgWidthHeight(&shortcut_item_width, &shortcut_item_height, widget_item_ptr->item_info.small_icon_id, hit_win_handle);
    if ((shortcut_item_width > 0) && (shortcut_item_height > 0))
    {
        MMK_GetWinLcdDevInfo(hit_win_handle, &lcd_dev_info);
    
        //重设选中widget对应子窗口区域，并设为隐藏属性
        win_rect.left = point.x-shortcut_item_width/2;
        win_rect.top = point.y-shortcut_item_height/2;
        win_rect.right = win_rect.left+shortcut_item_width-1;
        win_rect.bottom = win_rect.top+shortcut_item_height-1;
//        MMK_SetWinRectNotifyCtrl(hit_win_handle, &win_rect);
        SetChildWinRect(hit_win_handle, win_rect, widget_ctrl_ptr->is_main_screen_landscape);

        //重画选中widget小图标
        img_rect.left = 0;
        img_rect.top = 0;
        img_rect.right = shortcut_item_width-1;
        img_rect.bottom = shortcut_item_height-1;
        disp_rect.left = img_rect.left + win_rect.left;
        disp_rect.top = img_rect.top + win_rect.top;
        disp_rect.right = img_rect.right + win_rect.left;
        disp_rect.bottom = img_rect.bottom + win_rect.top;
        GUIRES_DisplayImg(
            PNULL,
            &disp_rect,
            &img_rect,
            hit_win_handle,
            widget_item_ptr->item_info.small_icon_id,
            &lcd_dev_info
            );
    }
}

/*****************************************************************************/
//  Description : clear widget rect
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
LOCAL void ClearWidgetRect(
                           MMIWIDGET_CTRL_T *widget_ctrl_ptr,
                           GUI_RECT_T rect
                           )
{
    if (PNULL == widget_ctrl_ptr)
    {
        //SCI_TRACE_LOW:"ClearWidgetRect() PNULL == widget_ctrl_ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_5487_112_2_18_3_7_42_117,(uint8*)"");
        return;
    }
    if (UILAYER_IsLayerActive(&(widget_ctrl_ptr->widget_layer)))
    {
        UILAYER_ClearRect(&(widget_ctrl_ptr->widget_layer), rect);
//        LCD_FillRect(&(widget_ctrl_ptr->widget_layer), rect, UILAYER_TRANSPARENT_COLOR);
    }
}

/*****************************************************************************/
//  Description : destroy shortcut bar layer
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void DestroyBarLayer(
                              MMIWIDGET_CTRL_T *widget_ctrl_ptr
                              )
{
    if (PNULL == widget_ctrl_ptr)
    {
        //SCI_TRACE_LOW:"DestroyWidgetLayer() PNULL == widget_ctrl_ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_5507_112_2_18_3_7_42_118,(uint8*)"");
        return;
    }

//    if ((UILAYER_NULL_HANDLE != widget_ctrl_ptr->bar_layer.block_id)
//        && (UILAYER_MAIN_HANDLE != widget_ctrl_ptr->bar_layer.block_id))  
    if (UILAYER_HANDLE_MULTI == UILAYER_GetHandleType(&widget_ctrl_ptr->bar_layer))
    {
        UILAYER_RELEASELAYER(&(widget_ctrl_ptr->bar_layer));   /*lint !e506 !e774*/
//        UILAYER_ReleaseLayer(&(widget_ctrl_ptr->bar_layer));
//        widget_ctrl_ptr->bar_layer.block_id = UILAYER_NULL_HANDLE;
    }
}

/*****************************************************************************/
//  Description : destroy softkey layer
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void DestroySoftkeyLayer(
                              MMIWIDGET_CTRL_T *widget_ctrl_ptr
                              )
{
    if (PNULL == widget_ctrl_ptr)
    {
        //SCI_TRACE_LOW:"DestroyElveLayer() PNULL == widget_ctrl_ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_5531_112_2_18_3_7_42_119,(uint8*)"");
        return;
    }

//    if ((UILAYER_NULL_HANDLE != widget_ctrl_ptr->softkey_layer.block_id)
//        && (UILAYER_MAIN_HANDLE != widget_ctrl_ptr->softkey_layer.block_id))    
    if (UILAYER_HANDLE_MULTI == UILAYER_GetHandleType(&widget_ctrl_ptr->softkey_layer))
    {
        UILAYER_RELEASELAYER(&(widget_ctrl_ptr->softkey_layer));   /*lint !e506 !e774*/
//        UILAYER_ReleaseLayer(&(widget_ctrl_ptr->softkey_layer));
//        widget_ctrl_ptr->softkey_layer.block_id = UILAYER_NULL_HANDLE;
    }
}

/*****************************************************************************/
//  Description : destroy status bar layer
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void DestroyStbLayer(
                              MMIWIDGET_CTRL_T *widget_ctrl_ptr
                              )
{
    if (PNULL == widget_ctrl_ptr)
    {
        //SCI_TRACE_LOW:"DestroyElveLayer() PNULL == widget_ctrl_ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_5555_112_2_18_3_7_42_120,(uint8*)"");
        return;
    }

//    if ((UILAYER_NULL_HANDLE != widget_ctrl_ptr->stb_layer.block_id)
//        && (UILAYER_MAIN_HANDLE != widget_ctrl_ptr->stb_layer.block_id)) 
    if (UILAYER_HANDLE_MULTI == UILAYER_GetHandleType(&widget_ctrl_ptr->stb_layer))    
    {
        UILAYER_RELEASELAYER(&(widget_ctrl_ptr->stb_layer));   /*lint !e506 !e774*/
//        UILAYER_ReleaseLayer(&(widget_ctrl_ptr->stb_layer));
//        widget_ctrl_ptr->stb_layer.block_id = UILAYER_NULL_HANDLE;
    }
}

/*****************************************************************************/
//  Description : destroy page title layer
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void DestroyPageTitleLayer(
                              MMIWIDGET_CTRL_T *widget_ctrl_ptr
                              )
{
    if (PNULL == widget_ctrl_ptr)
    {
        //SCI_TRACE_LOW:"DestroyElveLayer() PNULL == widget_ctrl_ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_5579_112_2_18_3_7_42_121,(uint8*)"");
        return;
    }

//    if ((UILAYER_NULL_HANDLE != widget_ctrl_ptr->title_layer.block_id)
//        && (UILAYER_MAIN_HANDLE != widget_ctrl_ptr->title_layer.block_id))    
    if (UILAYER_HANDLE_MULTI == UILAYER_GetHandleType(&widget_ctrl_ptr->title_layer))    
    {
        UILAYER_RELEASELAYER(&(widget_ctrl_ptr->title_layer));   /*lint !e506 !e774*/
//        UILAYER_ReleaseLayer(&(widget_ctrl_ptr->title_layer));
//        widget_ctrl_ptr->title_layer.block_id = UILAYER_NULL_HANDLE;
    }
}

/*****************************************************************************/
//  Description : destroy desktop elve layer
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void DestroyElveLayer(
                              MMIWIDGET_CTRL_T *widget_ctrl_ptr
                              )
{
    if (PNULL == widget_ctrl_ptr)
    {
        //SCI_TRACE_LOW:"DestroyElveLayer() PNULL == widget_ctrl_ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_5603_112_2_18_3_7_42_122,(uint8*)"");
        return;
    }

//    if ((UILAYER_NULL_HANDLE != widget_ctrl_ptr->elve_layer.block_id)
//        && (UILAYER_MAIN_HANDLE != widget_ctrl_ptr->elve_layer.block_id))    
    if (UILAYER_HANDLE_MULTI == UILAYER_GetHandleType(&widget_ctrl_ptr->elve_layer))    
    {
        UILAYER_RELEASELAYER(&(widget_ctrl_ptr->elve_layer));   /*lint !e506 !e774*/
//        UILAYER_ReleaseLayer(&(widget_ctrl_ptr->elve_layer));
//        widget_ctrl_ptr->elve_layer.block_id = UILAYER_NULL_HANDLE;
    }
}

/*****************************************************************************/
//  Description : destroy wallpaper layer
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
LOCAL void DestroyWallpaperLayer(
                              MMIWIDGET_CTRL_T *widget_ctrl_ptr
                              )
{
    if (PNULL == widget_ctrl_ptr)
    {
        //SCI_TRACE_LOW:"DestroyWallpaperLayer() PNULL == widget_ctrl_ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_5627_112_2_18_3_7_42_123,(uint8*)"");
        return;
    }

//    if ((UILAYER_NULL_HANDLE != widget_ctrl_ptr->wallpaper_layer.block_id)
//        && (UILAYER_MAIN_HANDLE != widget_ctrl_ptr->wallpaper_layer.block_id))    
    if (UILAYER_HANDLE_MULTI == UILAYER_GetHandleType(&widget_ctrl_ptr->wallpaper_layer))    
    {
        MMK_SetCtrlLcdDevInfo(MMIIDLE_WALLPAPER_ANIM_CTRL_ID, MMITHEME_GetDefaultLcdDev()); /*lint !e605*/
        UILAYER_RELEASELAYER(&(widget_ctrl_ptr->wallpaper_layer));   /*lint !e506 !e774*/
//        UILAYER_ReleaseLayer(&(widget_ctrl_ptr->wallpaper_layer));
//        widget_ctrl_ptr->wallpaper_layer.block_id = UILAYER_NULL_HANDLE;
    }
}

/*****************************************************************************/
//  Description : destroy widget layer
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
LOCAL void DestroyWidgetLayer(
                              MMIWIDGET_CTRL_T *widget_ctrl_ptr
                              )
{
    if (PNULL == widget_ctrl_ptr)
    {
        //SCI_TRACE_LOW:"DestroyWidgetLayer() PNULL == widget_ctrl_ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_5652_112_2_18_3_7_42_124,(uint8*)"");
        return;
    }

//    if ((UILAYER_NULL_HANDLE != widget_ctrl_ptr->widget_layer.block_id)
//        && (UILAYER_MAIN_HANDLE != widget_ctrl_ptr->widget_layer.block_id))    
    if (UILAYER_HANDLE_MULTI == UILAYER_GetHandleType(&widget_ctrl_ptr->widget_layer))    
    {
        UILAYER_RELEASELAYER(&(widget_ctrl_ptr->widget_layer));   /*lint !e506 !e774*/
//        UILAYER_ReleaseLayer(&(widget_ctrl_ptr->widget_layer));
//        widget_ctrl_ptr->widget_layer.block_id = UILAYER_NULL_HANDLE;
    }
}

/*****************************************************************************/
//  Description : destroy page layer
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void DestroyWidgetPageLayer(
                              MMIWIDGET_CTRL_T *widget_ctrl_ptr
                              )
{
    if (PNULL == widget_ctrl_ptr)
    {
        //SCI_TRACE_LOW:"DestroyWidgetLayer() PNULL == widget_ctrl_ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_5676_112_2_18_3_7_42_125,(uint8*)"");
        return;
    }

//    if ((UILAYER_NULL_HANDLE != widget_ctrl_ptr->page_layer.block_id)
//        && (UILAYER_MAIN_HANDLE != widget_ctrl_ptr->page_layer.block_id))    
    if (UILAYER_HANDLE_MULTI == UILAYER_GetHandleType(&widget_ctrl_ptr->page_layer))    
    {
//        MMK_SetCtrlLcdDevInfo(MMIIDLE_WALLPAPER_ANIM_CTRL_ID, MMITHEME_GetDefaultLcdDev());
        UILAYER_RELEASELAYER(&(widget_ctrl_ptr->page_layer));   /*lint !e506 !e774*/
//        UILAYER_ReleaseLayer(&(widget_ctrl_ptr->page_layer));
//        widget_ctrl_ptr->page_layer.block_id = UILAYER_NULL_HANDLE;
    }
}

/*****************************************************************************/
//  Description : create shortcut bar layer
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void CreateBarLayer(MMIWIDGET_CTRL_T *widget_ctrl_ptr)
{
    GUI_LCD_DEV_INFO widget_layer_handle = {0};
    uint16 width = 0;
    uint16 height = 0;
    int16 offsetX = 0;
    UILAYER_CREATE_T create_info = {0};

    if (PNULL == widget_ctrl_ptr)
    {
        //SCI_TRACE_LOW:"CreateWidgetLayer() PNULL == widget_ctrl_ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_5705_112_2_18_3_7_42_126,(uint8*)"");
        return;
    }

//    if ((UILAYER_NULL_HANDLE == widget_ctrl_ptr->bar_layer.block_id)
//        ||(UILAYER_MAIN_HANDLE == widget_ctrl_ptr->bar_layer.block_id))
    if (UILAYER_HANDLE_MULTI != UILAYER_GetHandleType(&widget_ctrl_ptr->bar_layer))    
    {
        width = (widget_ctrl_ptr->display_theme.shortcut_data.bar_data.shortcut_bar_rect.right 
                   - widget_ctrl_ptr->display_theme.shortcut_data.bar_data.shortcut_bar_rect.left) 
                   + (widget_ctrl_ptr->display_theme.shortcut_data.switch_data.shortcut_bar_switch_rect.right
                   - widget_ctrl_ptr->display_theme.shortcut_data.switch_data.shortcut_bar_switch_rect.left)
                   + 1;
        
        height = (MMITHEME_GetMainScreenBottomPixel()+1);

        if (widget_ctrl_ptr->shortcut.is_shortcut_on)
        {
            offsetX = 0;
        }
        else
        {
            offsetX = widget_ctrl_ptr->display_theme.shortcut_data.bar_data.shortcut_bar_rect.left 
                       - widget_ctrl_ptr->display_theme.shortcut_data.bar_data.shortcut_bar_rect.right - 1;
        }
        
        create_info.lcd_id = LCD_ID_0;
        create_info.owner_handle = widget_ctrl_ptr->win_id;
        create_info.offset_x = 0;
        create_info.offset_y = 0;
        create_info.width = width;
        create_info.height = height;
        create_info.is_bg_layer = FALSE;
        create_info.is_static_layer = FALSE;         
        UILAYER_CreateLayer(
            &create_info, 
            &widget_layer_handle
            );
        UILAYER_SetLayerColorKey(&widget_layer_handle, TRUE, UILAYER_TRANSPARENT_COLOR);
        widget_ctrl_ptr->bar_layer = widget_layer_handle;
        UILAYER_SetLayerPosition(&widget_layer_handle, offsetX, 0);
    }
    if (UILAYER_IsLayerActive(&widget_layer_handle))
    {
        UILAYER_Clear(&widget_layer_handle);
    }
}

/*****************************************************************************/
//  Description : is elve widget or not.
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsElveWidget(uint32 widget_id)
{
    BOOLEAN result = FALSE;
    
    if (MMIWIDGET_ELVE_ID == widget_id)
    {
        result = TRUE;
    }

    return result;
}

/*****************************************************************************/
//  Description : force to draw elve.
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void DrawElve(MMIWIDGET_CTRL_T *widget_ctrl_ptr)
{
    MMIWIDGET_ITEM_T  *item_ptr = PNULL;
    MMI_RESULT_E result = MMI_RESULT_FALSE;

    if (PNULL == widget_ctrl_ptr)
    {
        //SCI_TRACE_LOW:"RunChildWidgetProc() PNULL == widget_ctrl_ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_5780_112_2_18_3_7_42_127,(uint8*)"");
        return;
    }

    item_ptr = FindWidgetItem(widget_ctrl_ptr, MMIWIDGET_ELVE_ID);
 
    if((PNULL != item_ptr) && item_ptr->is_active && (PNULL != item_ptr->item_info.HandleWidgetWinMsg))
    {
        result = item_ptr->item_info.HandleWidgetWinMsg(item_ptr->item_info.win_id, MSG_FULL_PAINT, PNULL);    
    }
}

/*****************************************************************************/
//  Description : start elve animation.
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void StartElveAnim(MMIWIDGET_CTRL_T *widget_ctrl_ptr, BOOLEAN is_to_start)
{
    MMIWIDGET_ITEM_T  *item_ptr = PNULL;
    MMI_RESULT_E result = MMI_RESULT_FALSE;

    if (PNULL == widget_ctrl_ptr)
    {
        //SCI_TRACE_LOW:"RunChildWidgetProc() PNULL == widget_ctrl_ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_5803_112_2_18_3_7_42_128,(uint8*)"");
        return;
    }

    item_ptr = FindWidgetItem(widget_ctrl_ptr, MMIWIDGET_ELVE_ID);
 
    if((PNULL != item_ptr) && (PNULL != item_ptr->item_info.HandleWidgetWinMsg) && item_ptr->is_active)
    {
        result = item_ptr->item_info.HandleWidgetWinMsg(item_ptr->item_info.win_id, MSG_WIDGET_ELVE_START_ANIM, &is_to_start);    
    }
}

/*****************************************************************************/
//  Description : set elve current status.
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void SetElveStatus(MMIWIDGET_CTRL_T *widget_ctrl_ptr, MMIWIDGET_ELVE_DISPLAY_E type)
{
    MMIWIDGET_ITEM_T  *item_ptr = PNULL;
    MMI_RESULT_E result = MMI_RESULT_FALSE;

    if (PNULL == widget_ctrl_ptr)
    {
        //SCI_TRACE_LOW:"RunChildWidgetProc() PNULL == widget_ctrl_ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_5826_112_2_18_3_7_42_129,(uint8*)"");
        return;
    }

    item_ptr = FindWidgetItem(widget_ctrl_ptr, MMIWIDGET_ELVE_ID);
 
    if((PNULL != item_ptr) && (PNULL != item_ptr->item_info.HandleWidgetWinMsg) && item_ptr->is_active)
    {
        result = item_ptr->item_info.HandleWidgetWinMsg(item_ptr->item_info.win_id, MSG_WIDGET_ELVE_SET_STATUS, &type);    
    }
}

/*****************************************************************************/
//  Description : stop elve animation.
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void StopElveAnim(MMIWIDGET_CTRL_T *widget_ctrl_ptr)
{
    MMIWIDGET_ITEM_T  *item_ptr = PNULL;
    MMI_RESULT_E result = MMI_RESULT_FALSE;
    BOOLEAN type = FALSE;

    if (PNULL == widget_ctrl_ptr)
    {
        //SCI_TRACE_LOW:"RunChildWidgetProc() PNULL == widget_ctrl_ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_5850_112_2_18_3_7_42_130,(uint8*)"");
        return;
    }
    
    item_ptr = FindWidgetItem(widget_ctrl_ptr, MMIWIDGET_ELVE_ID);
 
    if((PNULL != item_ptr) && (PNULL != item_ptr->item_info.HandleWidgetWinMsg) && item_ptr->is_active)
    {
        result = item_ptr->item_info.HandleWidgetWinMsg(item_ptr->item_info.win_id, MSG_WIDGET_ELVE_START_ANIM, &type);    
    }
}

/*****************************************************************************/
//  Description : start mp3 progress bar timer.
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void StartMp3PrgbarTimer(MMIWIDGET_CTRL_T *widget_ctrl_ptr)
{
    MMIWIDGET_ITEM_T  *item_ptr = PNULL;
    MMI_RESULT_E result = MMI_RESULT_FALSE;
    BOOLEAN type = FALSE;

    if (PNULL == widget_ctrl_ptr)
    {
        //SCI_TRACE_LOW:"RunChildWidgetProc() PNULL == widget_ctrl_ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_5874_112_2_18_3_7_42_131,(uint8*)"");
        return;
    }
    
    item_ptr = FindWidgetItem(widget_ctrl_ptr, MMIWIDGET_MP3_ID);
 
    if((PNULL != item_ptr) && (PNULL != item_ptr->item_info.HandleWidgetWinMsg) && item_ptr->is_active)
    {
        result = item_ptr->item_info.HandleWidgetWinMsg(item_ptr->item_info.win_id, MSG_WIDGET_MP3_PRGBAR_STAR, &type);    
    }
}

/*****************************************************************************/
//  Description : stop mp3 progress bar timer.
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void StopMp3PrgbarTimer(MMIWIDGET_CTRL_T *widget_ctrl_ptr)
{
    MMIWIDGET_ITEM_T  *item_ptr = PNULL;
    MMI_RESULT_E result = MMI_RESULT_FALSE;
    BOOLEAN type = FALSE;

    if (PNULL == widget_ctrl_ptr)
    {
        //SCI_TRACE_LOW:"RunChildWidgetProc() PNULL == widget_ctrl_ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_5898_112_2_18_3_7_43_132,(uint8*)"");
        return;
    }
    
    item_ptr = FindWidgetItem(widget_ctrl_ptr, MMIWIDGET_MP3_ID);
 
    if((PNULL != item_ptr) && (PNULL != item_ptr->item_info.HandleWidgetWinMsg) && item_ptr->is_active)
    {
        result = item_ptr->item_info.HandleWidgetWinMsg(item_ptr->item_info.win_id, MSG_WIDGET_MP3_PRGBAR_STOP, &type);    
    }
}

/*****************************************************************************/
//  Description : create softkey layer
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void CreateSoftkeyLayer(MMIWIDGET_CTRL_T *widget_ctrl_ptr)
{
    GUI_LCD_DEV_INFO widget_layer_handle = {0};
    uint16 width = 0;
    uint16 height = 0;
    UILAYER_CREATE_T create_info = {0};  
    GUI_RECT_T main_rect = MMITHEME_GetFullScreenRect();    

    if (PNULL == widget_ctrl_ptr)
    {
        //SCI_TRACE_LOW:"CreateWidgetLayer() PNULL == widget_ctrl_ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_5924_112_2_18_3_7_43_133,(uint8*)"");
        return;
    }

//    if (  (UILAYER_NULL_HANDLE == widget_ctrl_ptr->softkey_layer.block_id)
//        ||(UILAYER_MAIN_HANDLE == widget_ctrl_ptr->softkey_layer.block_id))
    if (UILAYER_HANDLE_MULTI != UILAYER_GetHandleType(&widget_ctrl_ptr->softkey_layer))    
    {
        width  = main_rect.right - main_rect.left + 1;
        height = MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT; 
        
        create_info.lcd_id          = LCD_ID_0;
        create_info.owner_handle    = widget_ctrl_ptr->win_id;
        create_info.offset_x        = 0;
        create_info.offset_y        = main_rect.bottom - main_rect.top + 1 - height;
        create_info.width           = width;
        create_info.height          = height;
        create_info.is_bg_layer     = FALSE;
        create_info.is_static_layer = FALSE;   
        
        UILAYER_CreateLayer(&create_info, &widget_layer_handle);
            
        UILAYER_SetLayerColorKey(&widget_layer_handle, TRUE, UILAYER_TRANSPARENT_COLOR);
        widget_ctrl_ptr->softkey_layer = widget_layer_handle;
    }
    
    if (UILAYER_IsLayerActive(&widget_layer_handle))
    {
        UILAYER_Clear(&widget_layer_handle);
    }
}

/*****************************************************************************/
//  Description : create status bar layer
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void CreateStbLayer(MMIWIDGET_CTRL_T *widget_ctrl_ptr)
{
    GUI_LCD_DEV_INFO widget_layer_handle = {0};
    uint16 width = 0;
    uint16 height = 0;
    UILAYER_CREATE_T create_info = {0};  
    GUI_RECT_T main_rect = MMITHEME_GetFullScreenRect();    

    if (PNULL == widget_ctrl_ptr)
    {
        //SCI_TRACE_LOW:"CreateWidgetLayer() PNULL == widget_ctrl_ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_5970_112_2_18_3_7_43_134,(uint8*)"");
        return;
    }

//    if (  (UILAYER_NULL_HANDLE == widget_ctrl_ptr->stb_layer.block_id)
//        ||(UILAYER_MAIN_HANDLE == widget_ctrl_ptr->stb_layer.block_id))
    if (UILAYER_HANDLE_MULTI != UILAYER_GetHandleType(&widget_ctrl_ptr->stb_layer))    
    {
        //GUIRES_GetImgWidthHeight(&width, &height, IMAGE_WIDGET_ELVE_NOR_0, widget_ctrl_ptr->win_id); 
        width  = main_rect.right - main_rect.left + 1;
        height = MMI_STATUSBAR_HEIGHT;
          
        create_info.lcd_id          = LCD_ID_0;
        create_info.owner_handle    = widget_ctrl_ptr->win_id;
        create_info.offset_x        = 0;
        create_info.offset_y        = 0;
        create_info.width           = width;
        create_info.height          = height;
        create_info.is_bg_layer     = FALSE;
        create_info.is_static_layer = FALSE;   
        
        UILAYER_CreateLayer(&create_info, &widget_layer_handle);
            
        UILAYER_SetLayerColorKey(&widget_layer_handle, TRUE, UILAYER_TRANSPARENT_COLOR);
        widget_ctrl_ptr->stb_layer = widget_layer_handle;
    }
    
    if (UILAYER_IsLayerActive(&widget_layer_handle))
    {
        UILAYER_Clear(&widget_layer_handle);
    }
}

/*****************************************************************************/
//  Description : create status bar layer
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void CreatePageTitleLayer(MMIWIDGET_CTRL_T *widget_ctrl_ptr)
{
    GUI_LCD_DEV_INFO widget_layer_handle = {0};
    uint16 width = 0;
    uint16 height = 0;
    GUI_RECT_T title_rect = MMITHEME_WIDGET_TITLE_ICON_RECT;
    GUI_RECT_T tilte_h_rect = MMITHEME_H_WIDGET_TITLE_ICON_RECT;
    UILAYER_CREATE_T create_info = {0};  

    if (PNULL == widget_ctrl_ptr)
    {
        //SCI_TRACE_LOW:"CreateWidgetLayer() PNULL == widget_ctrl_ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_6018_112_2_18_3_7_43_135,(uint8*)"");
        return;
    }

//    if (  (UILAYER_NULL_HANDLE == widget_ctrl_ptr->title_layer.block_id)
//        ||(UILAYER_MAIN_HANDLE == widget_ctrl_ptr->title_layer.block_id))
    if (UILAYER_HANDLE_MULTI != UILAYER_GetHandleType(&widget_ctrl_ptr->title_layer))    
    {
        if (widget_ctrl_ptr->is_main_screen_landscape)    
        {
            title_rect = tilte_h_rect;
        }

        width  = title_rect.right - title_rect.left + 1;
        height = title_rect.bottom - title_rect.top + 1;

        create_info.lcd_id          = LCD_ID_0;
        create_info.owner_handle    = widget_ctrl_ptr->win_id;
        create_info.offset_x        = title_rect.left;
        create_info.offset_y        = title_rect.top;
        create_info.width           = width;
        create_info.height          = height;
        create_info.is_bg_layer     = FALSE;
        create_info.is_static_layer = FALSE;   
        
        UILAYER_CreateLayer(&create_info, &widget_layer_handle);
            
        UILAYER_SetLayerColorKey(&widget_layer_handle, TRUE, UILAYER_TRANSPARENT_COLOR);
        widget_ctrl_ptr->title_layer = widget_layer_handle;
    }
    
    if (UILAYER_IsLayerActive(&widget_layer_handle))
    {
        UILAYER_Clear(&widget_layer_handle);
    }
}

/*****************************************************************************/
//  Description : create desktop elve layer
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void CreateDeskElveLayer(MMIWIDGET_CTRL_T *widget_ctrl_ptr)
{
    GUI_LCD_DEV_INFO widget_layer_handle = {0};
    uint16 width = 0;
    uint16 height = 0;
    UILAYER_CREATE_T create_info = {0};  

    if (PNULL == widget_ctrl_ptr)
    {
        //SCI_TRACE_LOW:"CreateWidgetLayer() PNULL == widget_ctrl_ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_6068_112_2_18_3_7_43_136,(uint8*)"");
        return;
    }

//    if (  (UILAYER_NULL_HANDLE == widget_ctrl_ptr->elve_layer.block_id)
//        ||(UILAYER_MAIN_HANDLE == widget_ctrl_ptr->elve_layer.block_id))
    if (UILAYER_HANDLE_MULTI != UILAYER_GetHandleType(&widget_ctrl_ptr->elve_layer))    
    {
        GUIRES_GetImgWidthHeight(&width, &height, IMAGE_WIDGET_ELVE_NOR_0, widget_ctrl_ptr->win_id); 
          
        create_info.lcd_id          = LCD_ID_0;
        create_info.owner_handle    = widget_ctrl_ptr->win_id;
        create_info.offset_x        = 0;
        create_info.offset_y        = 0;
        create_info.width           = width;
        create_info.height          = height;
        create_info.is_bg_layer     = FALSE;
        create_info.is_static_layer = FALSE;   
        
        UILAYER_CreateLayer(&create_info, &widget_layer_handle);
            
        UILAYER_SetLayerColorKey(&widget_layer_handle, TRUE, UILAYER_TRANSPARENT_COLOR);
        widget_ctrl_ptr->elve_layer = widget_layer_handle;
    }
    
    if (UILAYER_IsLayerActive(&widget_layer_handle))
    {
        UILAYER_Clear(&widget_layer_handle);
    }
}

/*****************************************************************************/
//  Description : create wallpaper layer
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void CreateWallpaperLayer(MMIWIDGET_CTRL_T *widget_ctrl_ptr)
{
    GUI_LCD_DEV_INFO widget_layer_handle = {0};
    uint16 width = 0;
    uint16 height = 0;
    UILAYER_CREATE_T create_info = {0};  
    GUI_RECT_T main_rect = MMITHEME_GetFullScreenRect();    

    if (PNULL == widget_ctrl_ptr)
    {
        //SCI_TRACE_LOW:"CreateWallpaperLayer() PNULL == widget_ctrl_ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_6113_112_2_18_3_7_43_137,(uint8*)"");
        return;
    }

//    if (  (UILAYER_NULL_HANDLE == widget_ctrl_ptr->wallpaper_layer.block_id)
//        ||(UILAYER_MAIN_HANDLE == widget_ctrl_ptr->wallpaper_layer.block_id))
    if (UILAYER_HANDLE_MULTI != UILAYER_GetHandleType(&widget_ctrl_ptr->wallpaper_layer))    
    {
        width  = main_rect.right  - main_rect.left + 1;
        height = main_rect.bottom - main_rect.top  + 1;
            
        create_info.lcd_id          = LCD_ID_0;
        create_info.owner_handle    = widget_ctrl_ptr->win_id;
        create_info.offset_x        = 0;
        create_info.offset_y        = 0;
        create_info.width           = width;
        create_info.height          = height;
        create_info.is_bg_layer     = TRUE;//FALSE;
        create_info.is_static_layer = FALSE;   
#if 0
        if(!MMIAPIIDLE_IsLiveWallpaper())
        {
            width = 2*width;
        }
#endif        
        UILAYER_CreateLayer(&create_info, &widget_layer_handle);
            
        UILAYER_SetLayerColorKey(&widget_layer_handle, TRUE, UILAYER_TRANSPARENT_COLOR);
        widget_ctrl_ptr->wallpaper_layer = widget_layer_handle;
    }
    
    if (UILAYER_IsLayerActive(&widget_layer_handle))
    {
        UILAYER_Clear(&widget_layer_handle);
        //初始黑色背景
        //LCD_FillRect(&widget_layer_handle, main_rect, MMI_BLACK_COLOR);
        MMK_SetCtrlLcdDevInfo(MMIIDLE_WALLPAPER_ANIM_CTRL_ID, &widget_layer_handle);
    }
}

/*****************************************************************************/
//  Description : create widget layer
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
LOCAL void CreateWidgetLayer(
                             MMIWIDGET_CTRL_T *widget_ctrl_ptr
                             )
{
    GUI_LCD_DEV_INFO widget_layer_handle = {0};
    UILAYER_CREATE_T create_info = {0};
    MMI_HANDLE_T click_win_handle = 0;
    GUI_RECT_T win_rect = {0};
    MMIWIDGET_ITEM_T  *item_ptr = PNULL;

    if (PNULL == widget_ctrl_ptr)
    {
        //SCI_TRACE_LOW:"CreateWidgetLayer() PNULL == widget_ctrl_ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_6169_112_2_18_3_7_43_138,(uint8*)"");
        return;
    }

//    if ((UILAYER_NULL_HANDLE == widget_ctrl_ptr->widget_layer.block_id)
//        ||(UILAYER_MAIN_HANDLE == widget_ctrl_ptr->widget_layer.block_id))
    if (UILAYER_HANDLE_MULTI != UILAYER_GetHandleType(&widget_ctrl_ptr->widget_layer))    
    {
//        widget_rect = GetWidgetCtrlRect(widget_ctrl_ptr);
//        width = (widget_rect.right+1-widget_rect.left);
//        height = (widget_rect.bottom+1-widget_rect.top);

        click_win_handle = GetWinHandleByWidgetId(widget_ctrl_ptr, widget_ctrl_ptr->tp_move_info.click_widget_id);
        MMK_GetWinRect(click_win_handle, &win_rect);                   

        item_ptr = FindWidgetItem(widget_ctrl_ptr, widget_ctrl_ptr->tp_move_info.click_widget_id);

        if (PNULL == item_ptr)
        {
            return;
        }

        if (!item_ptr->is_active)
        {
            win_rect = GetChildWinRect(widget_ctrl_ptr, widget_ctrl_ptr->tp_move_info.click_widget_id);
        }
#if 0
        if (IsElveWidget(widget_ctrl_ptr->tp_move_info.click_widget_id))
        {
            win_rect = GetWidgetCtrlRect(widget_ctrl_ptr);    
        }
#endif        
        create_info.lcd_id = LCD_ID_0;
        create_info.owner_handle = widget_ctrl_ptr->win_id;
        create_info.offset_x = win_rect.left;
        create_info.offset_y = win_rect.top;
        create_info.width = (win_rect.right+1-win_rect.left);
        create_info.height = (win_rect.bottom+1-win_rect.top);
        create_info.is_bg_layer = FALSE;
        create_info.is_static_layer = FALSE;  
        UILAYER_CreateLayer(
            &create_info, 
            &widget_layer_handle
            );
        UILAYER_SetLayerColorKey(&widget_layer_handle, TRUE, UILAYER_TRANSPARENT_COLOR);

        widget_ctrl_ptr->widget_layer = widget_layer_handle;
    }
    if (UILAYER_IsLayerActive(&widget_layer_handle))
    {
        UILAYER_Clear(&widget_layer_handle);
    }
}

/*****************************************************************************/
//  Description : insert shortcut
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN InsertShortcut(
                          MMIWIDGET_CTRL_T *widget_ctrl_ptr,
                          MMI_IMAGE_ID_T icon_id,
                          uint32 widget_id,
                          uint32 pos
                          )
{
    uint32 i = 0;

    if (PNULL == widget_ctrl_ptr)
    {
        //SCI_TRACE_LOW:"InsertShortcut() PNULL == widget_ctrl_ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_6238_112_2_18_3_7_43_139,(uint8*)"");
        return FALSE;
    }

    while (i < widget_ctrl_ptr->shortcut.shortcut_num)
    {
        if (widget_ctrl_ptr->shortcut.shortcut_item[i].widget_id == widget_id)
        {
            return FALSE;
        }
        i++;
    }

    for (i = widget_ctrl_ptr->shortcut.shortcut_num; i > pos; i--)
    {
        widget_ctrl_ptr->shortcut.shortcut_item[i] = widget_ctrl_ptr->shortcut.shortcut_item[i-1];
    }
    widget_ctrl_ptr->shortcut.shortcut_item[pos].widget_id = widget_id;
    widget_ctrl_ptr->shortcut.shortcut_num++;

    return TRUE;
}

/*****************************************************************************/
//  Description : switch widget postion on shortcut bar.
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void SwitchWidgetPos(
                          MMIWIDGET_CTRL_T *widget_ctrl_ptr,
                          uint32 pos1,
                          uint32 pos2
                          )
{
    uint32 widget_id = 0;

    if (PNULL == widget_ctrl_ptr)
    {
        //SCI_TRACE_LOW:"InsertShortcut() PNULL == widget_ctrl_ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_6275_112_2_18_3_7_43_140,(uint8*)"");
        return;
    }

    widget_id = widget_ctrl_ptr->shortcut.shortcut_item[pos1].widget_id;
    widget_ctrl_ptr->shortcut.shortcut_item[pos1].widget_id = widget_ctrl_ptr->shortcut.shortcut_item[pos2].widget_id;
    widget_ctrl_ptr->shortcut.shortcut_item[pos2].widget_id = widget_id;
}

/*****************************************************************************/
//  Description : remove shortcut item by position.
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN RemoveShortcut(
                          MMIWIDGET_CTRL_T *widget_ctrl_ptr,
                          uint32 pos
                          )
{
    uint32 i = 0;
    
    if (PNULL == widget_ctrl_ptr)
    {
        //SCI_TRACE_LOW:"RemoveShortcut() PNULL == widget_ctrl_ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_6297_112_2_18_3_7_43_141,(uint8*)"");
        return FALSE;
    }

    if (pos >= widget_ctrl_ptr->shortcut.shortcut_num)
    {
        return FALSE;
    }
    
    if (widget_ctrl_ptr->shortcut.shortcut_num > 0)
    {
        if (i < widget_ctrl_ptr->shortcut.shortcut_num)
        {
            for (i = pos; i < widget_ctrl_ptr->shortcut.shortcut_num; i++)
            {
                widget_ctrl_ptr->shortcut.shortcut_item[i] = widget_ctrl_ptr->shortcut.shortcut_item[i+1];
            }
            //清空最后一项
            SCI_MEMSET(&(widget_ctrl_ptr->shortcut.shortcut_item[i]), 0x00, sizeof(MMIWIDGET_SHORTCUT_ITEM_T));
            widget_ctrl_ptr->shortcut.shortcut_num--;
        }
    }

    return TRUE;
}

/*****************************************************************************/
//  Description : remove shortcut item
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN RemoveShortcutById(
                          MMIWIDGET_CTRL_T *widget_ctrl_ptr,
                          uint32 widget_id
                          )
{
    uint32 i = 0;
    BOOLEAN is_find = FALSE;
    
    if (PNULL == widget_ctrl_ptr)
    {
        //SCI_TRACE_LOW:"RemoveShortcut() PNULL == widget_ctrl_ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_6337_112_2_18_3_7_43_142,(uint8*)"");
        return FALSE;
    }

    if (widget_ctrl_ptr->shortcut.shortcut_num > 0)
    {
        if (widget_id == widget_ctrl_ptr->shortcut.shortcut_item[widget_ctrl_ptr->shortcut.shortcut_num - 1].widget_id)
        {
            widget_ctrl_ptr->shortcut.shortcut_item[widget_ctrl_ptr->shortcut.shortcut_num - 1].widget_id = 0;
            widget_ctrl_ptr->shortcut.shortcut_num--;
            return TRUE;
        }
        
        for (i = 0; i < widget_ctrl_ptr->shortcut.shortcut_num; i++)
        {
            if (widget_id == widget_ctrl_ptr->shortcut.shortcut_item[i].widget_id)
            {
                is_find = TRUE;
                break;
            }
        }
        
        if (is_find)
        {
            for (; i < widget_ctrl_ptr->shortcut.shortcut_num; i++)
            {
                widget_ctrl_ptr->shortcut.shortcut_item[i] = widget_ctrl_ptr->shortcut.shortcut_item[i+1];
            }
            widget_ctrl_ptr->shortcut.shortcut_item[i].widget_id = 0;
            widget_ctrl_ptr->shortcut.shortcut_num--;
            return TRUE;
        }
    }

    return FALSE;
}

/*****************************************************************************/
//  Description : calculate all small icons postions on shortcut bar.
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void CalcAllShortcutItemPos(
                                 MMIWIDGET_CTRL_T *widget_ctrl_ptr,
                                 int16 shortcut_offset
                                 )
{
    uint16 i = 0;
    uint16 icon_width = 0;
    uint16 icon_height = 0;
    MMIWIDGET_ITEM_T *item_ptr = PNULL;

    if (PNULL == widget_ctrl_ptr)
    {
        //SCI_TRACE_LOW:"GetShortcutItemRect() PNULL == widget_ctrl_ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_6390_112_2_18_3_7_43_143,(uint8*)"");
        return;
    }

    for (i = 0; i < widget_ctrl_ptr->shortcut.shortcut_num; i++)
    {
        item_ptr = FindWidgetItem(widget_ctrl_ptr, widget_ctrl_ptr->shortcut.shortcut_item[i].widget_id);
       
        if (PNULL != item_ptr)
        {
             GUIRES_GetImgWidthHeight(&icon_width, &icon_height, item_ptr->item_info.small_icon_id, widget_ctrl_ptr->win_id);
             
             widget_ctrl_ptr->shortcut.shortcut_item_pos[i].x 
                         = (widget_ctrl_ptr->display_theme.shortcut_data.bar_data.shortcut_bar_rect.left 
                                           + widget_ctrl_ptr->display_theme.shortcut_data.bar_data.shortcut_bar_rect.right - icon_width)/2;
            if (0 == i)
            {
                 widget_ctrl_ptr->shortcut.shortcut_item_pos[i].y 
                             = widget_ctrl_ptr->display_theme.shortcut_data.bar_data.shortcut_bar_rect.top +  shortcut_offset;
            }
            else
            {
                item_ptr = FindWidgetItem(widget_ctrl_ptr, widget_ctrl_ptr->shortcut.shortcut_item[i - 1].widget_id);

                if (PNULL != item_ptr)
                {
                    GUIRES_GetImgWidthHeight(&icon_width, &icon_height, item_ptr->item_info.small_icon_id, widget_ctrl_ptr->win_id);
    
                    widget_ctrl_ptr->shortcut.shortcut_item_pos[i].y 
                                 = widget_ctrl_ptr->shortcut.shortcut_item_pos[i - 1].y + icon_height + widget_ctrl_ptr->display_theme.shortcut_data.disp_interval;
                }             
            }             
        }
    }
}

/*****************************************************************************/
//  Description : calculate small icons postions on shortcut bar in range.
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void CalcRangeShortcutItemPos(
                                 MMIWIDGET_CTRL_T *widget_ctrl_ptr,
                                 int16 shortcut_offset,
                                 uint32 min_index,
                                 uint32 max_index
                                 )
{
    uint16 i = 0;
    uint16 icon_width = 0;
    uint16 icon_height = 0;
    MMIWIDGET_ITEM_T *item_ptr = PNULL;

    if (PNULL == widget_ctrl_ptr)
    {
        //SCI_TRACE_LOW:"GetShortcutItemRect() PNULL == widget_ctrl_ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_6444_112_2_18_3_7_44_144,(uint8*)"");
        return;
    }

    for (i = min_index; i < max_index; i++)
    {
        item_ptr = FindWidgetItem(widget_ctrl_ptr, widget_ctrl_ptr->shortcut.shortcut_item[i].widget_id);
       
        if (PNULL != item_ptr)
        {
             GUIRES_GetImgWidthHeight(&icon_width, &icon_height, item_ptr->item_info.small_icon_id, widget_ctrl_ptr->win_id);

             widget_ctrl_ptr->shortcut.shortcut_item_pos[i].x 
                         = (widget_ctrl_ptr->display_theme.shortcut_data.bar_data.shortcut_bar_rect.left 
                                           + widget_ctrl_ptr->display_theme.shortcut_data.bar_data.shortcut_bar_rect.right - icon_width)/2;
            if (0 == i)
            {
                 widget_ctrl_ptr->shortcut.shortcut_item_pos[i].y 
                             = widget_ctrl_ptr->display_theme.shortcut_data.bar_data.shortcut_bar_rect.top +  shortcut_offset;
            }
            else
            {
                item_ptr = FindWidgetItem(widget_ctrl_ptr, widget_ctrl_ptr->shortcut.shortcut_item[i - 1].widget_id);

                if (PNULL != item_ptr)
                {
                    GUIRES_GetImgWidthHeight(&icon_width, &icon_height, item_ptr->item_info.small_icon_id, widget_ctrl_ptr->win_id);
    
                    widget_ctrl_ptr->shortcut.shortcut_item_pos[i].y 
                                 = widget_ctrl_ptr->shortcut.shortcut_item_pos[i - 1].y + icon_height + widget_ctrl_ptr->display_theme.shortcut_data.disp_interval;                
                }
            }             
        }
    }
}

/*****************************************************************************/
//  Description : get shortcut item rect
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
LOCAL GUI_RECT_T GetShortcutItemRect(
                                     MMIWIDGET_CTRL_T *widget_ctrl_ptr,
                                     int16 shortcut_offset,
                                     uint32 index
                                     )
{
    GUI_RECT_T rect = {0};
    uint16 icon_width = 0;
    uint16 icon_height = 0;
    MMIWIDGET_ITEM_T *item_ptr = PNULL;

    if (PNULL == widget_ctrl_ptr)
    {
        //SCI_TRACE_LOW:"GetShortcutItemRect() PNULL == widget_ctrl_ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_6497_112_2_18_3_7_44_145,(uint8*)"");
        return rect;
    }

    item_ptr = FindWidgetItem(widget_ctrl_ptr, widget_ctrl_ptr->shortcut.shortcut_item[index].widget_id);
    
    if (PNULL != item_ptr)
    {
        GUIRES_GetImgWidthHeight(&icon_width, &icon_height, item_ptr->item_info.small_icon_id, widget_ctrl_ptr->win_id);

        rect.left = widget_ctrl_ptr->shortcut.shortcut_item_pos[index].x;        
        rect.top = widget_ctrl_ptr->shortcut.shortcut_item_pos[index].y;
        rect.right = rect.left + icon_width - 1;
        rect.bottom = rect.top + icon_height - 1;             
    }

    return rect;
}

/*****************************************************************************/
//  Description : find shortcut insert pos by point
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN FindShortcutInsertPosByPoint(
                                     MMIWIDGET_CTRL_T *widget_ctrl_ptr,
                                     GUI_POINT_T point,
                                     uint32 *pos_ptr
                                     )
{
    uint32 i = 0;
    GUI_RECT_T rect = {0};

    if (PNULL == widget_ctrl_ptr)
    {
        //SCI_TRACE_LOW:"FindShortcutInsertPosByPoint() PNULL == widget_ctrl_ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_6531_112_2_18_3_7_44_146,(uint8*)"");
        return FALSE;
    }
    if (!IsPointInShortcutRegion(widget_ctrl_ptr, point))
    {
        return FALSE;
    }    
    for (i = 0; i < widget_ctrl_ptr->shortcut.shortcut_num; i++)
    {
        rect = GetShortcutItemRect(widget_ctrl_ptr, widget_ctrl_ptr->shortcut_offset, i);
        if (point.y <= rect.bottom)
        {
            break;
        }
    }
    if (PNULL != pos_ptr)
    {
        *pos_ptr = i;
    }
    return TRUE;
}

/*****************************************************************************/
//  Description : find shortcut pos by point
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN FindShortcutPosByPoint(
                                     MMIWIDGET_CTRL_T *widget_ctrl_ptr,
                                     GUI_POINT_T point,
                                     uint32 *pos_ptr
                                     )
{
    uint32 i = 0;
    BOOLEAN is_find = FALSE;
    GUI_RECT_T rect = {0};

    if (PNULL == widget_ctrl_ptr)
    {
        //SCI_TRACE_LOW:"FindShortcutPosByPoint() PNULL == widget_ctrl_ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_6569_112_2_18_3_7_44_147,(uint8*)"");
        return is_find;
    }
    for (i = 0; i < widget_ctrl_ptr->shortcut.shortcut_num; i++)
    {
        rect = GetShortcutItemRect(widget_ctrl_ptr, widget_ctrl_ptr->shortcut_offset, i);
        if (GUI_PointIsInRect(point, rect))
        {
            if (IsValidWidgetId(widget_ctrl_ptr->shortcut.shortcut_item[i].widget_id))
            {
                is_find = TRUE;
                if (PNULL != pos_ptr)
                {
                    *pos_ptr = i;
                }
            }
            break;
        }
    }
    return is_find;
}

/*****************************************************************************/
//  Description : get child window rect.
//  Global resource dependence :
//  Author: bin.ji
//  Note: 
/*****************************************************************************/
LOCAL GUI_RECT_T GetChildWinRect(MMIWIDGET_CTRL_T *widget_ctrl_ptr, uint32 widget_id)
{
    GUI_RECT_T window_rect = {0};
    MMIWIDGET_ITEM_T *item_ptr = PNULL;
    uint16 bg_width = 0;
    uint16 bg_height = 0;    
    
    item_ptr = FindWidgetItem(widget_ctrl_ptr, widget_id);
    
    if (PNULL != item_ptr)
    {
        window_rect.left = item_ptr->item_info.start_pos.x;
        window_rect.top = item_ptr->item_info.start_pos.y;
        GUIRES_GetImgWidthHeight(&bg_width, &bg_height, item_ptr->item_info.large_bg_img, item_ptr->item_info.win_id);
        window_rect.right = window_rect.left + bg_width - 1;
        window_rect.bottom = window_rect.top + bg_height - 1;

        if (widget_ctrl_ptr->is_main_screen_landscape)
        {
            //从竖屏转换当前屏（横屏）
            window_rect = RotateRect(window_rect, FALSE);
        }
    }

    return window_rect;
}

/*****************************************************************************/
//  Description : is widget active
//  Global resource dependence : 
//  Author:arvin.wu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsWidgetActive(
                           MMIWIDGET_CTRL_T *widget_ctrl_ptr,
                           uint32 widget_id
                           )
{
    MMIWIDGET_ITEM_T *item_ptr = PNULL; 

    if (PNULL == widget_ctrl_ptr)
    {
        //SCI_TRACE_LOW:"SetWidgetActive() PNULL == widget_ctrl_ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_6635_112_2_18_3_7_44_148,(uint8*)"");
        return FALSE;
    }
    item_ptr = FindWidgetItem(widget_ctrl_ptr, widget_id);
    
    if (PNULL != item_ptr)
    {
        return item_ptr->is_active;
    }

    return FALSE;
}

/*****************************************************************************/
//  Description : active widget when loading all widget
//  Global resource dependence :
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN InitActiveWidgetItem(
                            MMIWIDGET_CTRL_T *widget_ctrl_ptr,
                            uint32 widget_id
                            )
{
    GUI_RECT_T window_rect = {0};
    MMIWIDGET_ITEM_T *item_ptr = PNULL;
    uint16 bg_width = 0;
    uint16 bg_height = 0;

    if (PNULL == widget_ctrl_ptr)
    {
        //SCI_TRACE_LOW:"ActiveWidgetItem() PNULL == widget_ctrl_ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_6664_112_2_18_3_7_44_149,(uint8*)"");
        return FALSE;
    }
    
    item_ptr = FindWidgetItem(widget_ctrl_ptr, widget_id);
    if (PNULL != item_ptr)
    {
        //add action process only when the widget id isn't exist in zorder system.
        if (AddZorderItemTopmost(widget_ctrl_ptr, item_ptr))
        {
            SetWidgetActive(widget_ctrl_ptr, widget_id, TRUE);        
            window_rect.left = item_ptr->item_info.start_pos.x;
            window_rect.top = item_ptr->item_info.start_pos.y;
            GUIRES_GetImgWidthHeight(&bg_width, &bg_height, item_ptr->item_info.large_bg_img, item_ptr->item_info.win_id);
            window_rect.right = window_rect.left + bg_width - 1;
            window_rect.bottom = window_rect.top + bg_height - 1;
            
            SetChildWinRect(item_ptr->item_info.win_id, window_rect, widget_ctrl_ptr->is_main_screen_landscape);
            return TRUE;
        }
    }

    return FALSE;
}

/*****************************************************************************/
//  Description : active widget
//  Global resource dependence :
//  Author: bin.ji
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN ActiveWidgetItem(
                            MMIWIDGET_CTRL_T *widget_ctrl_ptr,
                            uint32 widget_id
                            )
{
    GUI_RECT_T window_rect = {0};
    MMIWIDGET_ITEM_T *item_ptr = PNULL;
    uint16 bg_width = 0;
    uint16 bg_height = 0;

    if (PNULL == widget_ctrl_ptr)
    {
        //SCI_TRACE_LOW:"ActiveWidgetItem() PNULL == widget_ctrl_ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_6705_112_2_18_3_7_44_150,(uint8*)"");
        return FALSE;
    }
    item_ptr = FindWidgetItem(widget_ctrl_ptr, widget_id);
    if (PNULL != item_ptr)
    {
        //add action process only when the widget id isn't exist in zorder system.
        if (AddZorderItemTopmost(widget_ctrl_ptr, item_ptr))
        {
            SetWidgetActive(widget_ctrl_ptr, widget_id, TRUE);        
            window_rect.left = item_ptr->item_info.start_pos.x;
            window_rect.top = item_ptr->item_info.start_pos.y;
            GUIRES_GetImgWidthHeight(&bg_width, &bg_height, item_ptr->item_info.large_bg_img, item_ptr->item_info.win_id);
            window_rect.right = window_rect.left + bg_width - 1;
            window_rect.bottom = window_rect.top + bg_height - 1;
    //        MMK_SetWinRectNotifyCtrl(item_ptr->item_info.win_id, &window_rect);
            if (widget_ctrl_ptr->is_main_screen_landscape)
            {
                //从竖屏转换当前屏（横屏）
                window_rect = RotateRect(window_rect, FALSE);
            }
            
            SetChildWinRect(item_ptr->item_info.win_id, window_rect, widget_ctrl_ptr->is_main_screen_landscape);
            return TRUE;
        }
    }

    return FALSE;
}

/*****************************************************************************/
//  Description : deactive widget
//  Global resource dependence :
//  Author: bin.ji
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN DeactiveWidgetItem(
                              MMIWIDGET_CTRL_T *widget_ctrl_ptr,
                              uint32 widget_id
                              )
{
    MMIWIDGET_ITEM_T *item_ptr = PNULL;

    if (PNULL == widget_ctrl_ptr)
    {
        //SCI_TRACE_LOW:"DeactiveWidgetItem() PNULL == widget_ctrl_ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_6748_112_2_18_3_7_44_151,(uint8*)"");
        return FALSE;
    }
    item_ptr = FindWidgetItem(widget_ctrl_ptr, widget_id);   
    if (PNULL != item_ptr)
    {
        //删除zorder中的窗口
        if (RemoveZorderItem(widget_ctrl_ptr, item_ptr))
        {
            SetWidgetActive(widget_ctrl_ptr, widget_id, FALSE); 
            return TRUE;
        }
    }

    return FALSE;
}

/*****************************************************************************/
//  Description : get win handle by widget id
//  Global resource dependence :
//  Author: bin.ji
//  Note: 
/*****************************************************************************/
LOCAL MMI_HANDLE_T GetWinHandleByWidgetId(
                             MMIWIDGET_CTRL_T *widget_ctrl_ptr,
                             uint32 widget_id
                             )
{
    MMI_HANDLE_T win_handle = 0;
    uint32 i = 0;

    if (PNULL == widget_ctrl_ptr)
    {
        //SCI_TRACE_LOW:"GetWinHandleByWidgetId() PNULL == widget_ctrl_ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_6779_112_2_18_3_7_44_152,(uint8*)"");
        return win_handle;
    }
    for (i = 0; i < MMIWIDGET_MAX_ITEM_NUM; i++)  
    {
        if (widget_id == widget_ctrl_ptr->item_ptr[i].item_info.widget_id)
        {
            win_handle = widget_ctrl_ptr->item_ptr[i].item_info.win_id;
            break;
        }
    }
    return win_handle;
}

/*****************************************************************************/
//  Description : get widget id by win handle
//  Global resource dependence :
//  Author: bin.ji
//  Note: 
/*****************************************************************************/
LOCAL uint32 GetWidgetIdByWinHandle(
                             MMIWIDGET_CTRL_T *widget_ctrl_ptr,
                             MMI_HANDLE_T win_handle
                             )
{
    uint32 widget_id = MMIWIDGET_INVALID_ID;
    uint32 i = 0;

    if (PNULL == widget_ctrl_ptr)
    {
        //SCI_TRACE_LOW:"GetWidgetIdByWinHandle() PNULL == widget_ctrl_ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_6807_112_2_18_3_7_44_153,(uint8*)"");
        return widget_id;
    }
    for (i = 0; i < MMIWIDGET_MAX_ITEM_NUM; i++)
    {
        if (MMK_ConvertIdToHandle(win_handle) == MMK_ConvertIdToHandle(widget_ctrl_ptr->item_ptr[i].item_info.win_id))
        {
            widget_id = widget_ctrl_ptr->item_ptr[i].item_info.widget_id;
            break;
        }
    }
    return widget_id;
}


/*****************************************************************************/
//  Description : is rect in rect
//  Global resource dependence :
//  Author: bin.ji
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsRectInRect(
                           GUI_RECT_T rect1,
                           GUI_RECT_T rect2
                           )
{
    GUI_POINT_T point1 = {0};
    GUI_POINT_T point2 = {0};

    point1.x = rect1.left;
    point1.y = rect1.top;
    point2.x = rect2.right;
    point2.y = rect2.bottom;

    if (GUI_PointIsInRect(point1, rect2) && GUI_PointIsInRect(point2, rect2))
    {
        return TRUE;
    }
    return FALSE;
}

/*****************************************************************************/
//  Description : is valid widget
//  Global resource dependence :
//  Author: bin.ji
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsValidWidgetId(
                              uint32 widget_id
                              )
{
    if (widget_id > 0 && widget_id < MMIWIDGET_WIDGET_MAX)
    {
        return TRUE;
    }
    
    return FALSE;
}

/*****************************************************************************/
//  Description :  set shortcut offset
//  Global resource dependence : 
//  Author:  bin.ji
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIWIDGET_SetShortcutOffset(
                                    MMIWIDGET_CTRL_E ctrl_id, // control id
                                    int16 shortcut_offset
                                    )
{
    MMIWIDGET_CTRL_T *widget_ctrl_ptr = MMIAPIWIDGET_GetWidgetPtr(ctrl_id);
    
    if (PNULL == widget_ctrl_ptr)
    {
        //SCI_TRACE_LOW:"MMIWIDGET_SetShortcutOffset() PNULL == widget_ctrl_ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_6875_112_2_18_3_7_44_154,(uint8*)"");
        return FALSE;
    }

    widget_ctrl_ptr->shortcut_offset = shortcut_offset;    

    CalcAllShortcutItemPos(widget_ctrl_ptr, widget_ctrl_ptr->shortcut_offset);
    
    return TRUE;
}

/*****************************************************************************/
//  Description : update all screen
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN UpdateAllScreen(void)
{
    MMIWIDGET_CTRL_T *widget_ctrl_ptr = MMIAPIWIDGET_GetWidgetPtr(MMIWIDGET_CTRL_IDLE_ID);
    
    //widget应用如果打开新窗口则不用刷新
    if (!MMK_IsFocusWin(widget_ctrl_ptr->win_id))
    {
        return FALSE;
    }

    MMK_UpdateScreen();
    
    return TRUE;
}

/*****************************************************************************/
//  Description : update all screen
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWIDGET_UpdateAllScreen(void)
{
   // MMIWIDGET_CTRL_T *widget_ctrl_ptr = MMIAPIWIDGET_GetWidgetPtr(MMIWIDGET_CTRL_IDLE_ID);
    
   // UILAYER_Clear(&widget_ctrl_ptr->widget_layer);
    
    UpdateAllScreen();
}

/*****************************************************************************/
//  Description : if the widget item is in the active list or shortcut list.
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL MMIWIDGET_LIST_E GetWidgetItemListId(
                                  uint32 widget_id
                                  )
{
    uint32 j = 0;
    MMIWIDGET_SAVE_INFO_T       save_info   = {0};
    MMIWIDGET_SAVE_ITEM_LIST_T *save_item   = PNULL;
    MMIWIDGET_LIST_E            widget_list = MMI_WIDGET_LIST_NONE;
    
    MMIWIDGET_ReadWidgetNV(&save_info);

    widget_list = MMI_WIDGET_ACTIVE_LIST;
    save_item   = &save_info.widget_active_list;

    for (j = 0; j < save_item->item_num; j++)
    {
        if (widget_id == save_item->item_info[j].widget_id)
        {
            return widget_list;
        }
    }

    widget_list = MMI_WIDGET_SHORTCUT_LIST;
    save_item   = &save_info.widget_shortcut_list;    

    for (j = 0; j < save_item->item_num; j++)
    {
        if (widget_id == save_item->item_info[j].widget_id)
        {
            return widget_list;
        }
    }

    return MMI_WIDGET_LIST_NONE;
}

/*****************************************************************************/
//  Description : set idle widget style.
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIWIDGET_SetIdleWidgetStyle(BOOLEAN set_value)
{
    s_is_set_widget_style = set_value;
}

/*****************************************************************************/
//  Description : get idle widget style.
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIWIDGET_IsIdleSetWidgetStyle(void)
{
    return s_is_set_widget_style;
}

/*****************************************************************************/
//  Description : add the new widget to the mmi idle widget.
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIAPIWIDGET_AddDynamicItem(MMIWIDGET_CTRL_E ctrl_id,
                                 uint32 widget_id,
                                 BOOLEAN is_on_shortcut,
                                 BOOLEAN is_to_update
                                  )
{
    uint32 i = 0;
    GUI_POINT_T point = {0};
    MMIWIDGET_ITEM_T widget_item    = {0};   
    MMIWIDGET_ITEM_T *widget_item_ptr = PNULL;   
    MMIWIDGET_ITEM_INFO_T *item_ptr = PNULL;
    MMIWIDGET_SAVE_INFO_T save_info = {0};
    MMIWIDGET_CTRL_T *widget_ctrl_ptr = MMIAPIWIDGET_GetWidgetPtr(ctrl_id);

    if (PNULL == widget_ctrl_ptr)
    {
        return MMI_RESULT_FALSE;
    }
    MMIWIDGET_ReadWidgetNV(&save_info);

    //already exist in shortcut list, remove it and add to zoder system.
    while (i < widget_ctrl_ptr->shortcut.shortcut_num)
    {
        if (widget_ctrl_ptr->shortcut.shortcut_item[i].widget_id == widget_id)
        {
            RemoveShortcutById(widget_ctrl_ptr, widget_id); 
        
            MMIWIDGET_SetShortcutOffset(MMIWIDGET_CTRL_IDLE_ID, widget_ctrl_ptr->shortcut_offset);
        
            DisplayDynWidgetShortcut(widget_ctrl_ptr, point);    
        
            ActiveWidgetItem(widget_ctrl_ptr, widget_id);
          
            UpdatePageDynmicWidgetItem(widget_ctrl_ptr, widget_id); 
           
            ResetOneWidgetLayer(widget_ctrl_ptr, widget_id);

            MMIWIDGET_SaveWidgetInfoToNV(MMIAPIWIDGET_GetWidgetPtr(ctrl_id));

            if (is_to_update)
            {
                UpdateAllScreen();
            }
            return MMI_RESULT_TRUE;
        }
        i++;
    }

    widget_item_ptr = FindWidgetItem(widget_ctrl_ptr, widget_id);
    
    //already exist in zoder system, update it.
    if (PNULL != widget_item_ptr && widget_item_ptr->is_active)
    {
        UpdatePageDynmicWidgetItem(widget_ctrl_ptr, widget_id); 
        
        MMIWIDGET_SaveWidgetInfoToNV(MMIAPIWIDGET_GetWidgetPtr(ctrl_id));
        ResetOneWidgetLayer(widget_ctrl_ptr, widget_id);  

        if (is_to_update)
        {
            UpdateAllScreen();
        }
       
        return MMI_RESULT_TRUE;        
    }

    //not in shortcut list and not in zoder system too, add new item to zoder system.
    if (save_info.widget_active_list.item_num < MMIWIDGET_MAX_SHORTCUT_NUM)
    {       
        item_ptr = MMIWIDGET_FindWidgetItemInfo(widget_id);
        if (PNULL != item_ptr)
        {
            widget_item.item_info = *item_ptr;
            widget_item.is_active = TRUE;
            widget_item.item_info.start_pos.x = item_ptr->start_pos.x;
            widget_item.item_info.start_pos.y = item_ptr->start_pos.y;
            widget_item.zorder_next_ptr = PNULL;
            widget_item.zorder_prev_ptr = PNULL;
            AppendDynItem(widget_ctrl_ptr, &widget_item);
        }
    }

    MMIWIDGET_SaveWidgetInfoToNV(MMIAPIWIDGET_GetWidgetPtr(ctrl_id));

    if (is_to_update)
    {
        UpdateAllScreen();
    }

    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
//  Description : remove one widget from the current mmi idle widget system.
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL uint32 GetShortcutItemPos(MMIWIDGET_CTRL_E ctrl_id,
                                       uint32 widget_id
                                       )
{
    uint32 i = 0;
    MMIWIDGET_CTRL_T *widget_ctrl_ptr = MMIAPIWIDGET_GetWidgetPtr(ctrl_id);

    if (PNULL == widget_ctrl_ptr)
    {
        return MMIWIDGET_INVALID_BAR_POS;
    }

    while (i < widget_ctrl_ptr->shortcut.shortcut_num) 
    {
        if (widget_id == widget_ctrl_ptr->shortcut.shortcut_item[i].widget_id) 
        {
            return i;
        }
        i++;
    }

    return MMIWIDGET_INVALID_BAR_POS;
}

/*****************************************************************************/
//  Description : remove one widget from the current mmi idle widget system.
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN RemoveWidgetItemPtr(MMIWIDGET_CTRL_E ctrl_id,
                                       uint32 widget_id
                                       )
{
    BOOLEAN result = FALSE;
    MMIWIDGET_CTRL_T * widget_ctrl_ptr = MMIAPIWIDGET_GetWidgetPtr(ctrl_id);
    MMIWIDGET_ITEM_T *item_ptr = PNULL;

    if (PNULL == widget_ctrl_ptr || 0 == widget_id)
    {
        //SCI_TRACE_LOW:"RemoveWidgetItemPtr() PNULL == widget_ctrl_ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_7114_112_2_18_3_7_45_155,(uint8*)"");
        return FALSE;
    }

    item_ptr = widget_ctrl_ptr->item_ptr + widget_id - 1;

    if (widget_ctrl_ptr->total_item_num > 0)
    {
        if (item_ptr->item_info.widget_id == widget_id)
        {
            SCI_MEMSET(widget_ctrl_ptr->item_ptr + widget_id - 1, 0x00, sizeof(MMIWIDGET_ITEM_T));
            widget_ctrl_ptr->total_item_num--;
            result = TRUE;
        }
    }

    return result;
}

/*****************************************************************************/
//  Description : remove dynamic widget from mmi idle widget system.
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIAPIWIDGET_RemoveDynamicItem(MMIWIDGET_CTRL_E ctrl_id,
                                           uint32 widget_id
                                           )
{   
	MMIWIDGET_LIST_E  list_id       = MMI_WIDGET_LIST_NONE;
    MMI_HANDLE_T  widget_handle = 0;
    GUI_RECT_T old_win_rect = {0};
    MMIWIDGET_CTRL_T * ctrl_ptr = PNULL;
    uint32 pos = MMIWIDGET_INVALID_BAR_POS;

    list_id = GetWidgetItemListId(widget_id);
    
    if (   (MMI_WIDGET_ACTIVE_LIST   != list_id)
        && (MMI_WIDGET_SHORTCUT_LIST != list_id))
	{
        return MMI_RESULT_FALSE;
	}

    ctrl_ptr = MMIAPIWIDGET_GetWidgetPtr(ctrl_id);

    if (PNULL == ctrl_ptr)
    {
        return MMI_RESULT_FALSE;
    }
    
    if (MMI_WIDGET_ACTIVE_LIST == list_id)
    {
        /*remove dynamic widget item on acitve idle.*/
        widget_handle = GetWinHandleByWidgetId(ctrl_ptr, widget_id);
        MMK_GetWinRect(widget_handle, &old_win_rect);
        ClearWidgetRect(ctrl_ptr, old_win_rect);
        
        if (DeactiveWidgetItem(ctrl_ptr, widget_id))
        {
    		DestroyWidgetItem(ctrl_ptr, FindWidgetItem(ctrl_ptr, widget_id)); 
            RemoveWidgetItemPtr(ctrl_id, widget_id);        
        }
    }
    else
    if (MMI_WIDGET_SHORTCUT_LIST == list_id )
    {
        //remove dynamic widget item on short cut bar.
        if (ctrl_ptr->shortcut.is_shortcut_on)
        {
            ResetHitWidgetLayer(ctrl_ptr);
        }     

        pos = GetShortcutItemPos(ctrl_id, widget_id);

        if (MMIWIDGET_INVALID_BAR_POS == pos)
        {
           return MMI_WIDGET_LIST_NONE;
        }
        
        if (RemoveShortcut(ctrl_ptr, pos))
        {
            /*destroy the dynamic child widget window.*/
            DestroyWidgetItem(ctrl_ptr, FindWidgetItem(ctrl_ptr, widget_id)); 
            RemoveWidgetItemPtr(ctrl_id, widget_id);          
        }       
    }
    MMIWIDGET_SaveWidgetInfoToNV(ctrl_ptr);

    //UpdateAllScreen();  

    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
//  Description : side all widget items one by one.
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void SlideWidgetOneByOne(MMIWIDGET_CTRL_T *widget_ctrl_ptr)
{
    uint32 i = 0;
    uint16 icon_width  = 0;
    uint16 icon_height = 0;
    int16  first_posX  = widget_ctrl_ptr->shortcut.shortcut_item_pos[0].x;
    MMIWIDGET_ITEM_T *item_ptr = PNULL;
    GUI_POINT_T widget_pos_bak[MMIWIDGET_MAX_SHORTCUT_NUM] = {0};

    if (PNULL == widget_ctrl_ptr)
    {
        //SCI_TRACE_LOW:"SlideWidgetOneByOne() PNULL == widget_ctrl_ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_7220_112_2_18_3_7_45_156,(uint8*)"");
        return;
    }

    if (0 == widget_ctrl_ptr->shortcut.shortcut_num)
    {
        return;
    }
    
    item_ptr = FindWidgetItem(widget_ctrl_ptr, widget_ctrl_ptr->shortcut.shortcut_item[0].widget_id);

    if (PNULL == item_ptr)
    {
        return;
    }
    
    GUIRES_GetImgWidthHeight(&icon_width, &icon_height, item_ptr->item_info.small_icon_id, widget_ctrl_ptr->win_id);

    for (i = 0; i < widget_ctrl_ptr->shortcut.shortcut_num; i++)
    {
        widget_pos_bak[i] = widget_ctrl_ptr->shortcut.shortcut_item_pos[i];
        widget_ctrl_ptr->shortcut.shortcut_item_pos[i].x -= (first_posX + icon_width);
    }
   
    for (i = 0; i < widget_ctrl_ptr->shortcut.shortcut_num; i++)
    {
        widget_ctrl_ptr->shortcut.shortcut_item_pos[i].x -= (int16)(i*MMIWIDGET_SLIDE_OBO_STEP);
    }

    DisplayWidgetShortcut(widget_ctrl_ptr);   

    while (widget_ctrl_ptr->shortcut.shortcut_item_pos[widget_ctrl_ptr->shortcut.shortcut_num - 1].x < widget_pos_bak[widget_ctrl_ptr->shortcut.shortcut_num - 1].x)   
    { 
        for (i = 0; i < widget_ctrl_ptr->shortcut.shortcut_num; i++)
        {
            if (widget_ctrl_ptr->shortcut.shortcut_item_pos[i].x < widget_pos_bak[i].x)
            {
                widget_ctrl_ptr->shortcut.shortcut_item_pos[i].x += MMIWIDGET_SLIDE_OBO_STEP;
            }

            if (widget_ctrl_ptr->shortcut.shortcut_item_pos[i].x > widget_pos_bak[i].x)
            {
                widget_ctrl_ptr->shortcut.shortcut_item_pos[i].x = widget_pos_bak[i].x;
            }
        }   
        
        DisplayWidgetShortcut(widget_ctrl_ptr);
        MMITHEME_UpdateRect();

        SCI_Sleep(10);
    } 

    MMIWIDGET_SetShortcutOffset(MMIWIDGET_CTRL_IDLE_ID, widget_ctrl_ptr->shortcut_offset);
    DisplayWidgetShortcut(widget_ctrl_ptr);
    MMITHEME_UpdateRect();   
}

/*****************************************************************************/
//  Description : roll back widget system when widgets move to up or down most.
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN RollbackWidget(MMIWIDGET_CTRL_T *widget_ctrl_ptr)
{
    int16 orgin_posY   = 0;
    int16 step_offset  = 0;
    MMIWIDGET_ITEM_T *item_ptr = PNULL;
    uint16 icon_width  = 0;
    uint16 icon_height = 0;
	BOOLEAN is_up   = FALSE;
    BOOLEAN is_down = FALSE;
    uint16  min_shortcut_num = 0;

    uint16   min_num    = 0;
    int16    step       = 0;
    uint16   roll_limit = 0; 
    MMIWIDGET_ROLLBACK_DIRECT_E direct = MMIWIDGET_ROLLBACK_UP;    

    if (0 == widget_ctrl_ptr->shortcut.shortcut_num)
    {
        return FALSE;
    }
    if (!widget_ctrl_ptr->shortcut.is_shortcut_on)
    {
        return FALSE;
    }
    if (!MMK_IsFocusWin(widget_ctrl_ptr->win_id))
    {
        return FALSE;
    }
    
    if (widget_ctrl_ptr->shortcut.shortcut_num >= MMIWIDGET_SHORTCUT_ROLLBACK_NUM)
    {
        min_shortcut_num = MMIWIDGET_SHORTCUT_ROLLBACK_NUM;
    }
    else
    {
        min_shortcut_num = widget_ctrl_ptr->shortcut.shortcut_num;
    }
   
    item_ptr = FindWidgetItem(widget_ctrl_ptr, widget_ctrl_ptr->shortcut.shortcut_item[0].widget_id);

    if (PNULL == item_ptr)
    {
        return FALSE;
    }
    
    GUIRES_GetImgWidthHeight(&icon_width, &icon_height, item_ptr->item_info.small_icon_id, widget_ctrl_ptr->win_id);
            
    if (   widget_ctrl_ptr->shortcut.shortcut_item_pos[widget_ctrl_ptr->shortcut.shortcut_num - min_shortcut_num].y 
        <= widget_ctrl_ptr->display_theme.shortcut_data.bar_data.shortcut_bar_rect.top)
    {
        is_up = TRUE;
        orgin_posY  = widget_ctrl_ptr->display_theme.shortcut_data.bar_data.shortcut_bar_rect.top;
        step_offset = MMIWIDGET_ROLL_BACK_STEP; 
    } 

    if (   widget_ctrl_ptr->shortcut.shortcut_item_pos[0].y 
        > widget_ctrl_ptr->display_theme.shortcut_data.bar_data.shortcut_bar_rect.top)
    {
        is_down = TRUE;
        orgin_posY  = widget_ctrl_ptr->display_theme.shortcut_data.bar_data.shortcut_bar_rect.top;
        step_offset = 0 - MMIWIDGET_ROLL_BACK_STEP; 
    }

    if (!is_up && !is_down)
    {
		return FALSE;
    }

    if ((   (is_up && widget_ctrl_ptr->shortcut.shortcut_item_pos[widget_ctrl_ptr->shortcut.shortcut_num - min_shortcut_num].y < orgin_posY)
           || (is_down && widget_ctrl_ptr->shortcut.shortcut_item_pos[0].y > orgin_posY)))

    {
        direct = is_up ? MMIWIDGET_ROLLBACK_UP: MMIWIDGET_ROLLBACK_DOWN;
        min_num = min_shortcut_num;
        roll_limit = orgin_posY;
        step = step_offset;    
        
        MMIWIDGET_SetShortcutOffset(MMIWIDGET_CTRL_IDLE_ID, (widget_ctrl_ptr->shortcut_offset + step_offset));

        if (is_down && (widget_ctrl_ptr->shortcut_offset < 0))
        {
            MMIWIDGET_SetShortcutOffset(MMIWIDGET_CTRL_IDLE_ID, 0);
        }

        DisplayWidgetShortcut(widget_ctrl_ptr);        
    }
    
    widget_ctrl_ptr->save_point.y = widget_ctrl_ptr->shortcut.shortcut_item_pos[0].y;  

    while ((   ((MMIWIDGET_ROLLBACK_UP == direct) && widget_ctrl_ptr->shortcut.shortcut_item_pos[widget_ctrl_ptr->shortcut.shortcut_num - min_num].y < roll_limit)
          || ((MMIWIDGET_ROLLBACK_DOWN == direct) && widget_ctrl_ptr->shortcut.shortcut_item_pos[0].y > roll_limit)))

    {
        MMIWIDGET_SetShortcutOffset(MMIWIDGET_CTRL_IDLE_ID, (widget_ctrl_ptr->shortcut_offset + step));

        if ((MMIWIDGET_ROLLBACK_DOWN == direct) && (widget_ctrl_ptr->shortcut_offset < 0))
        {
            MMIWIDGET_SetShortcutOffset(MMIWIDGET_CTRL_IDLE_ID, 0);
        }
 
        DisplayWidgetShortcut(widget_ctrl_ptr);
        MMITHEME_UpdateRect();
        SCI_SLEEP(10);
    }
	
    return TRUE;
}

/*****************************************************************************/
//  Description : save tp move point
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN SaveTpMovePoint(
                              MMIWIDGET_CTRL_T *widget_ctrl_ptr,
                              GUI_POINT_T point
                              )
{
    if (PNULL == widget_ctrl_ptr)
    {
        return FALSE;
    }
    widget_ctrl_ptr->save_point = point;

    return TRUE;
}

/*****************************************************************************/
//  Description : handle widget tp move
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsPageLayerPosInRange(
                                    MMIWIDGET_CTRL_T *widget_ctrl_ptr,
                                    int16 offset
                                    )
{
    uint16 page_width = 0;

    if (PNULL == widget_ctrl_ptr)
    {
        return FALSE;
    }
    page_width = GetWidgetPageWidth(widget_ctrl_ptr);    
    if ((offset <= 0) && (offset >= -page_width*(MMIWIDGET_MAX_PAGE_NUM-1)))
    {
        return TRUE;
    }
    return FALSE;
}

/*****************************************************************************/
//  Description : handle widget tp move
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
LOCAL void ChangeAllChildWinRect(
                                 MMIWIDGET_CTRL_T *widget_ctrl_ptr,
                                 int16 offset_x
                                 )
{
    MMIWIDGET_ITEM_T *item_ptr = PNULL;
    //uint32 i = 0;
    GUI_RECT_T win_rect = {0};
    if (PNULL == widget_ctrl_ptr || PNULL == widget_ctrl_ptr->zorder_item_ptr)
    {
        //SCI_TRACE_LOW:"ChangeAllChildWinRect() PNULL == widget_ctrl_ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_7444_112_2_18_3_7_46_157,(uint8*)"");
        return;
    }   

    item_ptr = widget_ctrl_ptr->zorder_item_ptr->zorder_next_ptr;
    
    while (PNULL != item_ptr)
    {
        //SCI_ASSERT(i++ < widget_ctrl_ptr->total_item_num);  //死循环检测        
        MMK_GetWinRect(item_ptr->item_info.win_id, &win_rect);
        win_rect.left += offset_x;
        win_rect.right += offset_x; 	

        SetChildWinRect(item_ptr->item_info.win_id, win_rect, widget_ctrl_ptr->is_main_screen_landscape);
        
        item_ptr = item_ptr->zorder_next_ptr;
    }
}

/*****************************************************************************/
//  Description : handle widget tp move
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN HandleSlidePageTpMove(
                                 MMIWIDGET_CTRL_T *widget_ctrl_ptr,
                                 GUI_POINT_T point
                                 )
{
    GUI_LCD_DEV_INFO    lcd_dev_info = {0};
    int16 offset_x = 0;
    int16 offset_y = 0;
#ifdef MMIWIDGET_VIBRATE_TICK_TEST    
    uint32 cur_tick_count = 0;
#endif
MMI_CheckAllocatedMemInfo();
    if (PNULL == widget_ctrl_ptr)
    {
        return FALSE;
    }
#ifdef MMIWIDGET_VIBRATE_TICK_TEST
    cur_tick_count = SCI_GetTickCount();
#endif
    s_is_page_moving = TRUE;
    SetBltLayer(widget_ctrl_ptr);
    //如果处于滑页状态，不让转屏
    //MMK_SetWinSupportAngle(widget_ctrl_ptr->win_id, WIN_SUPPORT_ANGLE_CUR);
    //计算新位置
    lcd_dev_info = widget_ctrl_ptr->page_layer;
    UILAYER_GetLayerPosition(&lcd_dev_info, &offset_x, &offset_y);
    offset_x += (point.x - widget_ctrl_ptr->save_point.x);
    if (IsPageLayerPosInRange(widget_ctrl_ptr, offset_x))
    {
        //改变page_layer的位置
//        UILAYER_SetLayerPosition(&lcd_dev_info, offset_x, offset_y);
        ChangePageLayerPos(widget_ctrl_ptr, offset_x, offset_y, TRUE);
        //重画top layer上的数据
        //RedrawTopLayer(widget_ctrl_ptr, FALSE);
        ChangeAllChildWinRect(widget_ctrl_ptr, (point.x - widget_ctrl_ptr->save_point.x));
    }

    if (point.x - widget_ctrl_ptr->save_point.x > 0)
    {
        widget_ctrl_ptr->tp_move_info.pen_move_dir = MMIWIDGET_PEN_MOVE_RIGHT;
    }
    else
    {
        widget_ctrl_ptr->tp_move_info.pen_move_dir = MMIWIDGET_PEN_MOVE_LEFT;    
    }
    
//    MMITHEME_UpdateRect();
#ifdef MMIWIDGET_VIBRATE_TICK_TEST
    OutputText(SCI_GetTickCount()-cur_tick_count);
#endif

    SaveTpMovePoint(widget_ctrl_ptr, point);
MMI_CheckAllocatedMemInfo();
    return TRUE;
}

/*****************************************************************************/
//  Description : redraw top layer
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
LOCAL void RedrawTopLayer(
                           MMIWIDGET_CTRL_T *widget_ctrl_ptr,
                           BOOLEAN is_redraw_bar
                           )
{
    IGUICTRL_T *stb_ctrl_ptr = PNULL;
    
    if (is_redraw_bar)
    {
        DisplayWidgetShortcut(widget_ctrl_ptr);    
    }
    
    DisplaySoftkey(widget_ctrl_ptr);
    DisplayPageTitle(widget_ctrl_ptr);

    stb_ctrl_ptr = (IGUICTRL_T*)MMK_GetCtrlPtrByWin(widget_ctrl_ptr->win_id, MMITHEME_GetStatusBarCtrlId());    
    
    if (UILAYER_IsLayerActive(&widget_ctrl_ptr->stb_layer))
    {
        GUICTRL_SetLcdDevInfo(stb_ctrl_ptr, &widget_ctrl_ptr->stb_layer);    
        UILAYER_Clear(&widget_ctrl_ptr->stb_layer);
        GUIWIN_ReDrawStbCtrl(widget_ctrl_ptr->win_id,FALSE);     
    }
    else
    {
        GUICTRL_SetLcdDevInfo(stb_ctrl_ptr, MMITHEME_GetDefaultLcdDev());         
        GUIWIN_ReDrawStbCtrl(widget_ctrl_ptr->win_id,FALSE); 
    }    
}

/*****************************************************************************/
//  Description : handle widget tp move
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
LOCAL void DisplayPageTitle(
                            MMIWIDGET_CTRL_T *widget_ctrl_ptr
                            )
{
    GUI_RECT_T      image_rect = {0};
    GUI_RECT_T      disp_rect = {0};
    uint16          image_width = 0;
    uint16          image_height = 0;
    uint16          page_index = 0;
    MMI_IMAGE_ID_T  title_img_id = 0;
    GUI_LCD_DEV_INFO *dev_info_ptr = PNULL; ;

    if (PNULL == widget_ctrl_ptr)
    {
        return;
    }

    if (UILAYER_IsLayerActive(&widget_ctrl_ptr->title_layer))
    {
        dev_info_ptr = &widget_ctrl_ptr->title_layer;
    }
    else
    {
        dev_info_ptr = MMITHEME_GetDefaultLcdDev(); /*lint !e605*/
    }
    
    for (page_index = 0; page_index < MMIWIDGET_MAX_PAGE_NUM; page_index++)
    {
        if (page_index == widget_ctrl_ptr->cur_page_index)
        {
            title_img_id = widget_ctrl_ptr->display_theme.page_data.sel_bg_image;
        }
        else
        {
            title_img_id = widget_ctrl_ptr->display_theme.page_data.unsel_bg_image;
        }
        GUIRES_GetImgWidthHeight(&image_width, &image_height, title_img_id, widget_ctrl_ptr->win_id);
        disp_rect = widget_ctrl_ptr->display_theme.page_data.img_disp_rect[page_index];
        disp_rect = GUI_GetCenterRect(disp_rect, image_width, image_height);
        image_rect = GUI_CreateRect(0, 0, image_width - 1, image_height - 1); 

        if (UILAYER_IsLayerActive(&widget_ctrl_ptr->page_layer))
        {
            UILAYER_ClearRect(dev_info_ptr, disp_rect);
        }
//        LCD_FillRect(dev_info_ptr, disp_rect, UILAYER_TRANSPARENT_COLOR);
        //display title image
        GUIRES_DisplayImg(PNULL,
            &disp_rect,
            &image_rect,
            widget_ctrl_ptr->win_id,
            title_img_id,
            dev_info_ptr
            );
    }
}

/*****************************************************************************/
//  Description : handle widget tp move
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
LOCAL void DisplaySoftkey(
                         MMIWIDGET_CTRL_T *widget_ctrl_ptr
                         )
{
    const GUI_LCD_DEV_INFO *lcd_dev_ptr = PNULL;
    GUI_RECT_T rect = MMITHEME_GetFullScreenRect();
    GUI_RECT_T client_rect = MMITHEME_GetClientRect();

    if (PNULL == widget_ctrl_ptr)
    {
        return;
    }

    if (UILAYER_IsLayerActive(&widget_ctrl_ptr->softkey_layer))
    {
        lcd_dev_ptr = &widget_ctrl_ptr->softkey_layer;
    }
    else
    {
        lcd_dev_ptr = MMITHEME_GetDefaultLcdDev();
    }
    
    rect.top = client_rect.bottom + 1;
    UILAYER_ClearRect(lcd_dev_ptr, rect); /*lint !e605*/
//    LCD_FillRect(&lcd_dev_info, rect, UILAYER_TRANSPARENT_COLOR);
    MMIAPIIDLE_DisplaySoftkey(widget_ctrl_ptr->win_id, lcd_dev_ptr);    /*lint !e605*/  
}

/*****************************************************************************/
//  Description : backup all active child window rect.
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void BakAllActiveChildWinRect(MMIWIDGET_CTRL_T *widget_ctrl_ptr)
{
    MMIWIDGET_ITEM_T *item_ptr = PNULL;
    GUI_RECT_T child_rect = {0};
    uint32 i = 0;

    if (PNULL == widget_ctrl_ptr || PNULL == widget_ctrl_ptr->zorder_item_ptr)
    {
        //SCI_TRACE_LOW:"BakAllActiveChildWinRect() PNULL == widget_ctrl_ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_7662_112_2_18_3_7_46_158,(uint8*)"");
        return;
    }   

    item_ptr = widget_ctrl_ptr->zorder_item_ptr->zorder_next_ptr;

    while (PNULL != item_ptr)
    {
        //SCI_ASSERT(i < widget_ctrl_ptr->total_item_num); 
        
        if (item_ptr->is_active)
        {            
            MMK_GetWinRect(item_ptr->item_info.win_id, &child_rect);
            s_widget_act_child_win_rect[i++] = child_rect;
        }
        
        item_ptr = item_ptr->zorder_next_ptr;
    }
}

/*****************************************************************************/
//  Description : restore all active child window rect.
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void RestoreAllActiveChildWinRect(MMIWIDGET_CTRL_T *widget_ctrl_ptr)
{
    MMIWIDGET_ITEM_T *item_ptr = PNULL;
    uint32 i = 0;

    if (PNULL == widget_ctrl_ptr || PNULL == widget_ctrl_ptr->zorder_item_ptr)
    {
        //SCI_TRACE_LOW:"RestoreAllActiveChildWinRect() PNULL == widget_ctrl_ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_7693_112_2_18_3_7_46_159,(uint8*)"");
        return;
    }   

    item_ptr = widget_ctrl_ptr->zorder_item_ptr->zorder_next_ptr;

    while (PNULL != item_ptr)
    {
        //SCI_ASSERT(i < widget_ctrl_ptr->total_item_num); 
        
        if (item_ptr->is_active)
        {            
            SetChildWinRect(item_ptr->item_info.win_id, s_widget_act_child_win_rect[i], widget_ctrl_ptr->is_main_screen_landscape);
            i++;
        }
        
        item_ptr = item_ptr->zorder_next_ptr;
    }
}

/*****************************************************************************/
//  Description : handle widget tp move
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
LOCAL void ShowSlideAnim(
                         MMIWIDGET_CTRL_T *widget_ctrl_ptr,
                         GUI_POINT_T point,
                         BOOLEAN is_slide_by_point
                         )
{
    uint16 src_page_index = 0;
    uint16 dst_page_index = 0;
    uint16 run_steps = 0;
    uint16 delay_ticks = 0;
    uint16 page_width = 0;
    uint16 slide_edge = 0;
    int16 offset_x = 0;
    int16 offset_y = 0;
    int16 old_offset_x = 0;
    int16 new_offset_x = 0;
    GUI_RECT_T widget_rect = {0};
    BOOLEAN is_change = FALSE;
    uint16 lcd_width  = 0;
    uint16 lcd_height = 0;

    if (PNULL == widget_ctrl_ptr)
    {
        return;
    }
    
    widget_rect = GetWidgetCtrlRect(widget_ctrl_ptr);
    run_steps = widget_ctrl_ptr->display_theme.slidepage_run_steps;
    delay_ticks = widget_ctrl_ptr->display_theme.slidepage_delay_ticks;
    slide_edge = widget_ctrl_ptr->display_theme.slide_edge;
    src_page_index = widget_ctrl_ptr->cur_page_index;
    page_width = GetWidgetPageWidth(widget_ctrl_ptr);    
    UILAYER_GetLayerPosition(&widget_ctrl_ptr->page_layer, &offset_x, &offset_y);
    old_offset_x = offset_x;

    if (is_slide_by_point)
    {
        //计算新的页index
        if ((point.x + slide_edge) > widget_rect.right)
        {
            //向左
            dst_page_index = (src_page_index<(MMIWIDGET_MAX_PAGE_NUM-1)) ? (src_page_index+1) : (MMIWIDGET_MAX_PAGE_NUM-1);
            while (offset_x > -dst_page_index*page_width)
            {
//                UILAYER_SetLayerPosition(&widget_ctrl_ptr->page_layer, offset_x, offset_y);    
                ChangePageLayerPos(widget_ctrl_ptr, offset_x, offset_y, TRUE);   
                ChangeAllChildWinRect(widget_ctrl_ptr, (offset_x - old_offset_x));
                //DrawElve(widget_ctrl_ptr);
                //RedrawTopLayer(widget_ctrl_ptr, FALSE);
                MMITHEME_UpdateRect();
                old_offset_x = offset_x;
                offset_x -= run_steps;
                //SCI_Sleep(delay_ticks);
            }

            is_change = TRUE;
        }
        else
        if (point.x < slide_edge)
        {
            //向右
            dst_page_index = (src_page_index>0) ? (src_page_index-1) : 0;
            while (offset_x < -dst_page_index*page_width)
            {
//                UILAYER_SetLayerPosition(&widget_ctrl_ptr->page_layer, offset_x, offset_y);  
                ChangePageLayerPos(widget_ctrl_ptr, offset_x, offset_y, TRUE);   
                ChangeAllChildWinRect(widget_ctrl_ptr, (offset_x - old_offset_x));
                //DrawElve(widget_ctrl_ptr);                
                //RedrawTopLayer(widget_ctrl_ptr, FALSE);                
                MMITHEME_UpdateRect();
                old_offset_x = offset_x;                
                offset_x += run_steps;
                //SCI_Sleep(delay_ticks);
            }

            is_change = TRUE;
        }  
    }
    else
    {
        GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);
        //计算新的页index
        if (MMIWIDGET_PEN_MOVE_LEFT == widget_ctrl_ptr->tp_move_info.pen_move_dir)
        {
            //向左
            if (offset_x < (-lcd_width * src_page_index))
            {
                dst_page_index = (src_page_index<(MMIWIDGET_MAX_PAGE_NUM-1)) ? (src_page_index+1) : (MMIWIDGET_MAX_PAGE_NUM-1);            
            }
            else
            {
                dst_page_index = src_page_index;
            }

            while (offset_x > -dst_page_index*page_width)
            {
//                UILAYER_SetLayerPosition(&widget_ctrl_ptr->page_layer, offset_x, offset_y);  
                ChangePageLayerPos(widget_ctrl_ptr, offset_x, offset_y, TRUE);   
                ChangeAllChildWinRect(widget_ctrl_ptr, (offset_x - old_offset_x));
                //DrawElve(widget_ctrl_ptr);                
                //RedrawTopLayer(widget_ctrl_ptr, FALSE);
                MMITHEME_UpdateRect();
                old_offset_x = offset_x;                
                offset_x -= run_steps;
                //SCI_Sleep(delay_ticks);
            }

            is_change = TRUE;
        }
        else
        if (MMIWIDGET_PEN_MOVE_RIGHT == widget_ctrl_ptr->tp_move_info.pen_move_dir)
        {
            //向右
            if (offset_x > (-lcd_width * src_page_index))
            {
                dst_page_index = (src_page_index>0) ? (src_page_index-1) : 0;
            }
            else
            {
                dst_page_index = src_page_index;
            }
        
            while (offset_x < -dst_page_index*page_width)
            {
//                UILAYER_SetLayerPosition(&widget_ctrl_ptr->page_layer, offset_x, offset_y);  
                ChangePageLayerPos(widget_ctrl_ptr, offset_x, offset_y, TRUE);   
                ChangeAllChildWinRect(widget_ctrl_ptr, (offset_x - old_offset_x));
                //DrawElve(widget_ctrl_ptr);                
                //RedrawTopLayer(widget_ctrl_ptr, FALSE);
                MMITHEME_UpdateRect();
                old_offset_x = offset_x;                
                offset_x += run_steps;
                //SCI_Sleep(delay_ticks);
            }

            is_change = TRUE;
        }
    }

    if (is_change)
    {
        new_offset_x = -dst_page_index*page_width;
        //end page
//        UILAYER_SetLayerPosition(&widget_ctrl_ptr->page_layer, new_offset_x, offset_y);  
        ChangePageLayerPos(widget_ctrl_ptr, new_offset_x, offset_y, TRUE);   
        old_offset_x = -page_width * widget_ctrl_ptr->cur_page_index;
        widget_ctrl_ptr->cur_page_index = dst_page_index;
        //RedrawTopLayer(widget_ctrl_ptr, FALSE);
        //通知所有子窗口改变位置
        RestoreAllActiveChildWinRect(widget_ctrl_ptr);
        ChangeAllChildWinRect(widget_ctrl_ptr, (new_offset_x - old_offset_x));   
        DisplayPageTitle(widget_ctrl_ptr);

        if (1 == widget_ctrl_ptr->cur_page_index)
        {
            //display idle text
            MAIN_DrawStkIdleModeText(widget_ctrl_ptr->page_layer.block_id);
        }

        MMITHEME_UpdateRect();
        //DrawElve(widget_ctrl_ptr);
    }
}

/*****************************************************************************/
//  Description : callback the  move timer
//  Global resource dependence : 
//  Author:cheng.luo
//  Note: 
/*****************************************************************************/
LOCAL void SlideAnimCallback(uint8 timer_id, uint32 param)
{
    MMIWIDGET_CTRL_T *widget_ctrl_ptr = (MMIWIDGET_CTRL_T *)param;
    
    if (timer_id == widget_ctrl_ptr->slide_anim.timer_id)
    {
        int16 run_steps = widget_ctrl_ptr->display_theme.slidepage_run_steps;
        int16 offset_x = widget_ctrl_ptr->slide_anim.offset_x;
        int16 offset_y = widget_ctrl_ptr->slide_anim.offset_y;
        uint16 page_width = widget_ctrl_ptr->slide_anim.page_width;
        int16 end_offset_x = -(widget_ctrl_ptr->slide_anim.dst_page_index * page_width);
        MMIWIDGET_PEN_MOVE_DIRECT_T move_direct = widget_ctrl_ptr->slide_anim.move_direct;
//        int16 old_offset_x = widget_ctrl_ptr->slide_anim.old_offset_x;
         
        if(MMIWIDGET_PEN_MOVE_LEFT == move_direct)
        {
            run_steps = -run_steps;
        }

        if (((MMIWIDGET_PEN_MOVE_LEFT == move_direct) && (offset_x > end_offset_x))
            ||((MMIWIDGET_PEN_MOVE_RIGHT == move_direct) && (offset_x < end_offset_x)))
        {        
//            UILAYER_SetLayerPosition(&widget_ctrl_ptr->page_layer, offset_x, offset_y);  
            ChangePageLayerPos(widget_ctrl_ptr, offset_x, offset_y, TRUE);   
            //DrawElve(widget_ctrl_ptr);                
            //RedrawTopLayer(widget_ctrl_ptr, FALSE);                
            MMITHEME_UpdateRect();
            widget_ctrl_ptr->slide_anim.old_offset_x = widget_ctrl_ptr->slide_anim.offset_x;
            widget_ctrl_ptr->slide_anim.offset_x += run_steps;
        }
        else
        {
            widget_ctrl_ptr->cur_page_index = widget_ctrl_ptr->slide_anim.dst_page_index;
            //end page
            UILAYER_GetLayerPosition(&widget_ctrl_ptr->page_layer, &offset_x, &offset_y); 
//            UILAYER_SetLayerPosition(&widget_ctrl_ptr->page_layer, end_offset_x, offset_y); 
            ChangePageLayerPos(widget_ctrl_ptr, end_offset_x, offset_y, TRUE);   
            ChangeAllChildWinRect(widget_ctrl_ptr, (end_offset_x - offset_x));
                  
            //通知所有子窗口改变位置
            //RestoreAllActiveChildWinRect(widget_ctrl_ptr);
            //ChangeAllChildWinRect(widget_ctrl_ptr, (page_width * page_offset));
            
            //DrawElve(widget_ctrl_ptr);
            DisplayPageTitle(widget_ctrl_ptr);
            //RedrawTopLayer(widget_ctrl_ptr, FALSE);         

            MMK_StopTimer(widget_ctrl_ptr->slide_anim.timer_id);
            widget_ctrl_ptr->slide_anim.timer_id = 0;
        }
    }
}

/*****************************************************************************/
//  Description : handle widget tp move with timer
//  Global resource dependence : 
//  Author:cheng.luo
//  Note: 
/*****************************************************************************/
LOCAL void PlaySlideAnim(
                         MMIWIDGET_CTRL_T *widget_ctrl_ptr,
                         GUI_POINT_T point,
                         BOOLEAN is_slide_by_point
                         )
{
    uint16 src_page_index = 0;
    uint16 dst_page_index = 0;
    uint16 slide_edge = 0;
    int16 offset_x = 0;
    int16 offset_y = 0; 
    GUI_RECT_T widget_rect = {0};
    uint16 lcd_width  = 0;
    uint16 lcd_height = 0;
    MMIWIDGET_PEN_MOVE_DIRECT_T move_direct = MMIWIDGET_PEN_MOVE_INVALID;

    if (PNULL == widget_ctrl_ptr
        || 0 != widget_ctrl_ptr->slide_anim.timer_id)
    {
        return;
    }

    slide_edge = widget_ctrl_ptr->display_theme.slide_edge;
    widget_rect = GetWidgetCtrlRect(widget_ctrl_ptr);
    src_page_index = widget_ctrl_ptr->cur_page_index;
    UILAYER_GetLayerPosition(&widget_ctrl_ptr->page_layer, &offset_x, &offset_y);
 
    if (is_slide_by_point)
    {
        if ((point.x + slide_edge) > widget_rect.right)
        {
            //向左
            dst_page_index = (src_page_index<(MMIWIDGET_MAX_PAGE_NUM-1)) ? (src_page_index+1) : (MMIWIDGET_MAX_PAGE_NUM-1);
            move_direct = MMIWIDGET_PEN_MOVE_LEFT;
        }
        else if (point.x < slide_edge)
        {
            //向右
            dst_page_index = (src_page_index>0) ? (src_page_index-1) : 0;
            move_direct = MMIWIDGET_PEN_MOVE_RIGHT;
        }
    }
    else
    {
        GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);
        //计算新的页index
        if (MMIWIDGET_PEN_MOVE_LEFT == widget_ctrl_ptr->tp_move_info.pen_move_dir)
        {
            //向左
            if (offset_x < (-lcd_width * src_page_index))
            {
                dst_page_index = (src_page_index<(MMIWIDGET_MAX_PAGE_NUM-1)) ? (src_page_index+1) : (MMIWIDGET_MAX_PAGE_NUM-1);            
            }
            else
            {
                dst_page_index = src_page_index;
            }
            move_direct = MMIWIDGET_PEN_MOVE_LEFT;
        }
       else if (MMIWIDGET_PEN_MOVE_RIGHT == widget_ctrl_ptr->tp_move_info.pen_move_dir)
       {
            //向右
            if (offset_x > (-lcd_width * src_page_index))
            {
                dst_page_index = (src_page_index>0) ? (src_page_index-1) : 0;
            }
            else
            {
                dst_page_index = src_page_index;
            }
            move_direct = MMIWIDGET_PEN_MOVE_RIGHT;
       }     
    }
    
    if (MMIWIDGET_PEN_MOVE_INVALID != move_direct)
    {
        widget_ctrl_ptr->slide_anim.move_point = point;
        widget_ctrl_ptr->slide_anim.offset_x = offset_x;
        widget_ctrl_ptr->slide_anim.offset_y = offset_y;
        widget_ctrl_ptr->slide_anim.page_width = GetWidgetPageWidth(widget_ctrl_ptr);
        widget_ctrl_ptr->slide_anim.dst_page_index = dst_page_index;
        widget_ctrl_ptr->slide_anim.move_direct = move_direct;
        widget_ctrl_ptr->slide_anim.old_offset_x = offset_x;

        if (0 != widget_ctrl_ptr->slide_anim.timer_id)
        {
            MMK_StopTimer(widget_ctrl_ptr->slide_anim.timer_id);
            widget_ctrl_ptr->slide_anim.timer_id = 0;
        }
        
        widget_ctrl_ptr->slide_anim.timer_id = MMK_CreateTimerCallback(
                                        MMIWIDGET_SLIDE_ANIM_INTERVAl,
                                        (MMI_TIMER_FUNC)SlideAnimCallback,
                                        (uint32)widget_ctrl_ptr,
                                        TRUE);
    }

}

/*****************************************************************************/
//  Description : change pagelayer pos
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
LOCAL void ChangePageLayerPos(
                              MMIWIDGET_CTRL_T *widget_ctrl_ptr,
                              int16 offset_x,
                              int16 offset_y,
                              BOOLEAN is_sync
                              )
{
    if (PNULL == widget_ctrl_ptr)
    {
        return;
    }
    if (UILAYER_HANDLE_MULTI == UILAYER_GetHandleType(&widget_ctrl_ptr->page_layer))
    {
        UILAYER_SetLayerPosition(&widget_ctrl_ptr->page_layer, offset_x, offset_y);   
        if (is_sync)
        {
            CopyPageLayerToMainLayer(widget_ctrl_ptr);
        }
    }
}

/*****************************************************************************/
//  Description : handle widget tp move
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN HandleSlidePageTpUp(
                                 MMIWIDGET_CTRL_T *widget_ctrl_ptr,
                                 GUI_POINT_T point
                                 )
{
    uint16      lcd_width = 0;
    int16 offset_y = 0;
    int16 offset_x = widget_ctrl_ptr->page_layer_offset_x;
    int16 old_offset_x = 0;
    uint16 run_steps = 0;
    uint16 delay_ticks = 0;
    uint16 slide_edge = 0;
    MMI_CheckAllocatedMemInfo();
    run_steps = widget_ctrl_ptr->display_theme.slidepage_run_steps;
    delay_ticks = widget_ctrl_ptr->display_theme.slidepage_delay_ticks;
    slide_edge = widget_ctrl_ptr->display_theme.slide_edge;

    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,PNULL);	

     //如果处于滑页状态，不让转屏,TP UP 现在恢复
   MMK_SetWinSupportAngle(widget_ctrl_ptr->win_id, WIN_SUPPORT_ANGLE_ALL);
    old_offset_x = -lcd_width * widget_ctrl_ptr->cur_page_index;
    if(ABS(widget_ctrl_ptr->down_point.x - point.x) >= (lcd_width/4))
    {       
	     ShowSlideAnim(widget_ctrl_ptr, point, FALSE);
         //PlaySlideAnim(widget_ctrl_ptr, point, FALSE);
    }
    else //弹回原来的位置
    {
         UILAYER_GetLayerPosition(&widget_ctrl_ptr->page_layer, &offset_x, &offset_y);

         if (offset_x < old_offset_x)
         {
             offset_x += run_steps;
    
             while (offset_x < old_offset_x)
             {
//                 UILAYER_SetLayerPosition(&widget_ctrl_ptr->page_layer, offset_x, offset_y);   
                 ChangePageLayerPos(widget_ctrl_ptr, offset_x, offset_y, TRUE);   
                 ChangeAllChildWinRect(widget_ctrl_ptr, run_steps);
                 //DrawElve(widget_ctrl_ptr);
                 //RedrawTopLayer(widget_ctrl_ptr, FALSE);
                 MMITHEME_UpdateRect();
                 offset_x += run_steps;
                 //SCI_Sleep(delay_ticks);
             } 
         }
         else
         {
             offset_x -= run_steps;
    
             while (offset_x > old_offset_x)
             {
//                 UILAYER_SetLayerPosition(&widget_ctrl_ptr->page_layer, offset_x, offset_y);   
                 ChangePageLayerPos(widget_ctrl_ptr, offset_x, offset_y, TRUE);   
                 ChangeAllChildWinRect(widget_ctrl_ptr, -run_steps);
                 //DrawElve(widget_ctrl_ptr);
                 //RedrawTopLayer(widget_ctrl_ptr, FALSE);
                 MMITHEME_UpdateRect();
                 offset_x -= run_steps;
                 //SCI_Sleep(delay_ticks);
             }          
         }

        //end page
        UILAYER_GetLayerPosition(&widget_ctrl_ptr->page_layer, &offset_x, &offset_y);
//        UILAYER_SetLayerPosition(&widget_ctrl_ptr->page_layer, old_offset_x, offset_y); 
        ChangePageLayerPos(widget_ctrl_ptr, old_offset_x, offset_y, TRUE);  
        //ChangeAllChildWinRect(widget_ctrl_ptr, old_offset_x - offset_x);
        //RedrawTopLayer(widget_ctrl_ptr, FALSE);
        //通知所有子窗口改变位置
        RestoreAllActiveChildWinRect(widget_ctrl_ptr);
        //ChangeAllChildWinRect(widget_ctrl_ptr, 0);        
        //DrawElve(widget_ctrl_ptr);          
    }

    s_is_page_moving = FALSE;
    widget_ctrl_ptr->down_point.x = MMIWIDGET_INT16_MIN_VALUE;
    MMI_CheckAllocatedMemInfo();
    return TRUE;
}

/*****************************************************************************/
//  Description : handle widget tp move
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
LOCAL int16 GetCurPageLayerOffset(
                                MMIWIDGET_CTRL_T *widget_ctrl_ptr
                                )
{
    int16 offset = 0;
    uint16 page_width = 0;

    if (PNULL == widget_ctrl_ptr)
    {
        return 0;
    }
    page_width = GetWidgetPageWidth(widget_ctrl_ptr);    
    offset = widget_ctrl_ptr->cur_page_index * page_width;
    return offset;
}

/*****************************************************************************/
//  Description : reset widget page layer pos
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
LOCAL void ResetWidgetPageLayerPos(
                                MMIWIDGET_CTRL_T *widget_ctrl_ptr
                                )
{
    int16               offset_x = 0;
    int16               offset_y = 0;
    
    if (PNULL == widget_ctrl_ptr)
    {
        return;
    }
    if (UILAYER_IsLayerActive(&widget_ctrl_ptr->page_layer))
    {
        offset_x = -GetCurPageLayerOffset(widget_ctrl_ptr);
        offset_y = 0;
        UILAYER_SetLayerPosition(&widget_ctrl_ptr->page_layer, offset_x, offset_y);
    }
}

/*****************************************************************************/
//  Description : reset wallpaper layer
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
LOCAL void ResetWallpaperLayer(
                                MMIWIDGET_CTRL_T *widget_ctrl_ptr
                                )
{

    if (PNULL == widget_ctrl_ptr)
    {
        return;
    }
    if (UILAYER_IsLayerActive(&widget_ctrl_ptr->wallpaper_layer))
    {
        MMIAPIWIDGET_SetWallpaperLcdDevInfo(&widget_ctrl_ptr->wallpaper_layer);
    }
    else
    {
    
        //主层内容：shortcut，状态栏，softkey, 墙纸，widget应用
        MMIAPIWIDGET_SetWallpaperLcdDevInfo((GUI_LCD_DEV_INFO *)MMITHEME_GetDefaultLcdDev());
  }
}

/*****************************************************************************/
//  Description : create widget layer
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
LOCAL void CreateWidgetPageLayer(
                             MMIWIDGET_CTRL_T *widget_ctrl_ptr
                             )
{
    GUI_LCD_DEV_INFO widget_layer_handle = {0};
    uint16 width = 0;
    uint16 height = 0;
    GUI_RECT_T widget_rect = {0};
    UILAYER_CREATE_T create_info = {0};
    GUI_RECT_T all_rect = {0};

    if (PNULL == widget_ctrl_ptr)
    {
        //SCI_TRACE_LOW:"CreateWidgetPageLayer() PNULL == widget_ctrl_ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_8237_112_2_18_3_7_47_160,(uint8*)"");
        return;
    }
//    if ((UILAYER_NULL_HANDLE == widget_ctrl_ptr->page_layer.block_id)
//        ||(UILAYER_MAIN_HANDLE == widget_ctrl_ptr->page_layer.block_id))
    if (UILAYER_HANDLE_MULTI != UILAYER_GetHandleType(&widget_ctrl_ptr->page_layer))    
    {
        widget_rect = GetWidgetCtrlRect(widget_ctrl_ptr);
        width = (widget_rect.right+1-widget_rect.left)*MMIWIDGET_MAX_PAGE_NUM;
        height = (widget_rect.bottom+1-widget_rect.top);
        create_info.lcd_id = LCD_ID_0;
        create_info.owner_handle = widget_ctrl_ptr->win_id;
        create_info.offset_x = 0;
        create_info.offset_y = 0;
        create_info.width = width;
        create_info.height = height;
        create_info.is_bg_layer = FALSE;
        create_info.is_static_layer = FALSE;  
        UILAYER_CreateLayer(
            &create_info,
            &widget_layer_handle
            );
        UILAYER_SetLayerColorKey(&widget_layer_handle, TRUE, UILAYER_TRANSPARENT_COLOR);
        widget_ctrl_ptr->page_layer = widget_layer_handle;
    }
    if (UILAYER_IsLayerActive(&widget_layer_handle))
    {
        UILAYER_Clear(&widget_layer_handle);
        //初始黑色背景
        all_rect.left = 0;
        all_rect.top = 0;
        all_rect.right = (width-1);
        all_rect.bottom = (height-1);
        LCD_FillRect(&widget_layer_handle, all_rect, MMI_BLACK_COLOR);        
        //初始化位置，后面会根据cur_page_index调整
        UILAYER_SetLayerPosition(&widget_layer_handle, 0, 0);
    }
}

/*****************************************************************************/
//  Description :  set shortcut offset
//  Global resource dependence : 
//  Author:  bin.ji
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIWIDGET_SetCurPageIndex(
                                    MMIWIDGET_CTRL_E ctrl_id, // control id
                                    int16 page_index
                                    )
{
    MMIWIDGET_CTRL_T *widget_ctrl_ptr = MMIAPIWIDGET_GetWidgetPtr(ctrl_id);
    
    if (PNULL == widget_ctrl_ptr)
    {
        //SCI_TRACE_LOW:"MMIWIDGET_SetCurPageIndex() PNULL == widget_ctrl_ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_8289_112_2_18_3_7_47_161,(uint8*)"");
        return FALSE;
    }

    widget_ctrl_ptr->cur_page_index = page_index;   
    //cur_page_index变了，需要调整page_layer的位置
    ResetWidgetPageLayerPos(widget_ctrl_ptr);
    SetBltLayer(widget_ctrl_ptr);
    
    return TRUE;
}

/*****************************************************************************/
//  Description : set high arm clock
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
LOCAL void SetArmClock(void)
{
    // 变频
    if(!MMIAPIMP3_IsPlayerActive())
    {
#ifndef WIN32
        CHNG_FREQ_SetArmClk(MMI_GetFreqHandler(), FREQ_INDEX_MP3);      
#endif
    }
}

/*****************************************************************************/
//  Description : restore arm clock
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
LOCAL void RestoreArmClock(void)
{
    if(!MMIAPIMP3_IsPlayerActive())
    {
#ifndef WIN32
        CHNG_FREQ_RestoreARMClk(MMI_GetFreqHandler());
#endif
    }     
}

/*****************************************************************************/
//  Description : switch lcd
//  Global resource dependence : 
//  Author:  
//  Note:
/*****************************************************************************/
LOCAL void SwitchLCD(
                     MMIWIDGET_CTRL_T *widget_ctrl_ptr
                     )
{
    if (PNULL == widget_ctrl_ptr)
    {
        return;
    }

    if (MMK_IsWindowLandscape(widget_ctrl_ptr->win_id) != widget_ctrl_ptr->is_main_screen_landscape)  
    {
        widget_ctrl_ptr->is_main_screen_landscape = !widget_ctrl_ptr->is_main_screen_landscape;
    }
	
    MMITHEME_GetWidgetTheme(&widget_ctrl_ptr->display_theme);

    //Rotate Page Layer
    DestroyWidgetPageLayer(widget_ctrl_ptr);
    CreateWidgetPageLayer(widget_ctrl_ptr);
    //重新设置page layer位置
    ResetWidgetPageLayerPos(widget_ctrl_ptr);
    //rotate all child win
    ResetAllWidgetLayer(widget_ctrl_ptr);
    //rotate bar layer
    DestroyBarLayer(widget_ctrl_ptr);
    CreateBarLayer(widget_ctrl_ptr);

    //rotate elve layer.
    DestroyElveLayer(widget_ctrl_ptr);
    CreateDeskElveLayer(widget_ctrl_ptr);

    //rotate softkey layer.
    DestroySoftkeyLayer(widget_ctrl_ptr);
    CreateSoftkeyLayer(s_idlewidget_ctrl_ptr);

    //rotate status bar layer.
    DestroyStbLayer(widget_ctrl_ptr);
    CreateStbLayer(s_idlewidget_ctrl_ptr);

    //rotate page title layer.
    DestroyPageTitleLayer(widget_ctrl_ptr);
    CreatePageTitleLayer(s_idlewidget_ctrl_ptr);

    //rotate wallpaper layer
    DestroyWallpaperLayer(widget_ctrl_ptr);
    CreateWallpaperLayer(widget_ctrl_ptr);
}

/*****************************************************************************/
//  Description : reset all widget layer
//  Global resource dependence : 
//  Author:  
//  Note:
/*****************************************************************************/
LOCAL void ResetAllWidgetLayer(
                           MMIWIDGET_CTRL_T *widget_ctrl_ptr
                           )
{
    uint32 i = 0;
    
    if (PNULL == widget_ctrl_ptr)
    {
        //SCI_TRACE_LOW:"ResetAllWidgetLayer() PNULL == widget_ctrl_ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_8394_112_2_18_3_7_47_162,(uint8*)"");
        return;
    }  
    if (PNULL != widget_ctrl_ptr->item_ptr)
    {
        for (i = 0; i < MMIWIDGET_MAX_ITEM_NUM; i++)
        {
            ResetOneWidgetLayer(widget_ctrl_ptr, widget_ctrl_ptr->item_ptr[i].item_info.widget_id);
        }
    }    
}

/*****************************************************************************/
//  Description : rotate point
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
LOCAL GUI_POINT_T RotatePoint(
                            GUI_POINT_T point,
                            BOOLEAN is_h_to_v    //横屏切竖屏
                            )
{
    GUI_POINT_T out_point = {0};
    uint16 lcd_width = MMI_MAINSCREEN_WIDTH;
    uint16 lcd_height = MMI_MAINSCREEN_HEIGHT;

    if (is_h_to_v)
    {
        //横屏切竖屏
        out_point.x = ZOOM_OFFSET(point.x, lcd_width, lcd_height);
        out_point.y = ZOOM_OFFSET(point.y, lcd_height, lcd_width);
    }
    else
    {
        //竖屏切横屏
        out_point.y = ZOOM_OFFSET(point.y, lcd_width, lcd_height);
        out_point.x = ZOOM_OFFSET(point.x, lcd_height, lcd_width);
    }

    return out_point;
}

/*****************************************************************************/
//  Description : get widget ctrl rect
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
LOCAL GUI_RECT_T GetWidgetCtrlRect(
                               MMIWIDGET_CTRL_T *widget_ctrl_ptr
                               )
{
    GUI_RECT_T rect = {0};

    if (PNULL == widget_ctrl_ptr)
    {
        return rect;
    }
    if (widget_ctrl_ptr->is_main_screen_landscape)
    {
        rect = widget_ctrl_ptr->both_rect.h_rect;
    }
    else
    {
        rect = widget_ctrl_ptr->both_rect.v_rect;
    }
    return rect;
}

/*****************************************************************************/
//  Description : get widget page width
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
LOCAL uint16 GetWidgetPageWidth(
                               MMIWIDGET_CTRL_T *widget_ctrl_ptr
                               )
{
    GUI_RECT_T widget_rect = {0};
    uint16 page_width = 0;

    if (PNULL == widget_ctrl_ptr)
    {
        return 0;
    }
    widget_rect = GetWidgetCtrlRect(widget_ctrl_ptr);
    page_width = (widget_rect.right + 1 - widget_rect.left);
    return page_width;
}

/*****************************************************************************/
//  Description : rotate rect
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
LOCAL GUI_RECT_T RotateRect(
                            GUI_RECT_T rect,
                            BOOLEAN is_h_to_v    //横屏切竖屏
                            )
{
    GUI_POINT_T old_point = {0};
    GUI_POINT_T point = {0};    
    GUI_RECT_T out_rect = {0};

    old_point.x = rect.left;
    old_point.y = rect.top;
    out_rect = rect;
    if (is_h_to_v)
    {
        //转换坐标
        point = RotatePoint(old_point, TRUE);    
    }
    else
    {
        point = RotatePoint(old_point, FALSE);    
    }
    out_rect.left += (point.x - old_point.x);
    out_rect.top += (point.y - old_point.y);
    out_rect.right += (point.x - old_point.x);
    out_rect.bottom += (point.y - old_point.y);        
    return out_rect;
}

/*****************************************************************************/
//  Description : set child win rect
//  Global resource dependence : 
//  Author:bin.ji
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SetChildWinRect(
                              MMI_HANDLE_T win_handle,
                              GUI_RECT_T rect,
                              BOOLEAN is_main_screen_landscape
                              )
{
    GUI_BOTH_RECT_T both_rect = {0};

    if ((rect.left > rect.right)
        || (rect.top > rect.bottom))
    {
        //MMI_ASSERT_WARNING("SetChildWinRect window_rect:(%d, %d, %d, %d) is error!", rect.left, rect.top, rect.right, rect.bottom);
        //SCI_TRACE_LOW:"SetChildWinRect window_rect:(%d, %d, %d, %d) is error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_8528_112_2_18_3_7_48_163,(uint8*)"dddd", rect.left, rect.top, rect.right, rect.bottom);
        return FALSE;        
    }
    
    if (is_main_screen_landscape)
    {        
        both_rect.h_rect = rect;
        both_rect.v_rect = RotateRect(rect, TRUE);      
    }
    else
    {
        both_rect.v_rect = rect;
        both_rect.h_rect = RotateRect(rect, FALSE);
    }
    MMK_SetBothRectNotifyCtrl(win_handle, &both_rect);
    MMK_PostMsg(win_handle, MSG_WIDGET_MODIFY_RECT, PNULL, NULL);
    return TRUE;
}
/*****************************************************************************/
//  Description : create widget应用层
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWIDGET_CreateApplicationLayer(void)
{
    uint16 lcd_width = 0;
    uint16 lcd_height = 0;
    UILAYER_CREATE_T create_info = {0};
    
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);	

//    if ((UILAYER_NULL_HANDLE == s_widget_application_layer.block_id)
//        ||(UILAYER_MAIN_HANDLE == s_widget_application_layer.block_id))
    if (UILAYER_HANDLE_MULTI != UILAYER_GetHandleType(&s_widget_application_layer))    
    {
        //MAIN_IDLE_WIN_ID 与 widget_ctrl_ptr->win_id 保持一致
        //用WIDGET的子窗口会有问题
        create_info.lcd_id = LCD_ID_0;
        create_info.owner_handle = MAIN_IDLE_WIN_ID;
        create_info.offset_x = 0;
        create_info.offset_y = 0;
        create_info.width = lcd_width;
        create_info.height = lcd_height;
        create_info.is_bg_layer = FALSE;
        create_info.is_static_layer = FALSE;        
        UILAYER_CreateLayer(
					            &create_info, 
					            &s_widget_application_layer
					            );
        UILAYER_SetLayerColorKey(&s_widget_application_layer, TRUE, UILAYER_TRANSPARENT_COLOR);
    }
    if (UILAYER_IsLayerActive(&s_widget_application_layer))
    {
        UILAYER_Clear(&s_widget_application_layer);
    }	
}
/*****************************************************************************/
//  Description : Destroy widget应用层
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWIDGET_DestroyApplicationLayer(void)
{
//    if ((UILAYER_NULL_HANDLE != s_widget_application_layer.block_id)
//        && (UILAYER_MAIN_HANDLE != s_widget_application_layer.block_id))    
    if (UILAYER_HANDLE_MULTI == UILAYER_GetHandleType(&s_widget_application_layer))    
    {
        UILAYER_RELEASELAYER(&s_widget_application_layer);   /*lint !e506 !e774*/
//        UILAYER_ReleaseLayer(&s_widget_application_layer);
//        s_widget_application_layer.block_id = UILAYER_NULL_HANDLE;
    }
}
/*****************************************************************************/
//  Description : 获取应用层(外用)
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIWIDGET_GetApplicationLayer(GUI_LCD_DEV_INFO *dev_info_ptr)
{
    BOOLEAN ret_result = FALSE;
//    if ((UILAYER_NULL_HANDLE != s_widget_application_layer.block_id)
//        && (UILAYER_MAIN_HANDLE != s_widget_application_layer.block_id))    
    if (UILAYER_HANDLE_MULTI == UILAYER_GetHandleType(&s_widget_application_layer))    
    {
		if(UILAYER_IsLayerActive(&s_widget_application_layer))
		{
			dev_info_ptr->lcd_id= s_widget_application_layer.lcd_id;
			dev_info_ptr->block_id= s_widget_application_layer.block_id;
			ret_result = TRUE;
		}
    }
    return ret_result;
}

/*****************************************************************************/
//  Description : is elve widget active
//  Global resource dependence : 
//  Author:bin.ji
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsElveWidgetActive(
                                 MMIWIDGET_CTRL_T *widget_ctrl_ptr
                                 )
{
    MMIWIDGET_ITEM_T  *item_ptr = PNULL;

    if (PNULL == widget_ctrl_ptr)
    {
        //SCI_TRACE_LOW:"IsElveWidgetActive() PNULL == widget_ctrl_ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_8631_112_2_18_3_7_48_164,(uint8*)"");
        return FALSE;
    }

    item_ptr = FindWidgetItem(widget_ctrl_ptr, MMIWIDGET_ELVE_ID);
 
    if((PNULL != item_ptr) && item_ptr->is_active)
    {
        return TRUE;
    }
    return FALSE;
}

/*****************************************************************************/
//  Description : copy pagelayer to mainlayer（将pagelayer的内容同步到mainlayer上）
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
LOCAL void CopyPageLayerToMainLayer(
                                    MMIWIDGET_CTRL_T *widget_ctrl_ptr
                                    )
{
#if 0
    GUI_LCD_DEV_INFO mainlayer = {0, 0};
    GUI_RECT_T rect = MMITHEME_GetFullScreenRect();
    uint32 cur_tick = SCI_GetTickCount();

    if (PNULL == widget_ctrl_ptr)
    {
        return;
    }
    if (UILAYER_HANDLE_MULTI == UILAYER_GetHandleType(&widget_ctrl_ptr->page_layer))
    {
        UILAYER_BltLayerToLayer(
            &mainlayer,
            &widget_ctrl_ptr->page_layer,
            &rect,
            FALSE
            );
        //SCI_TRACE_LOW:"CopyPageLayerToMainLayer() pass = %dms"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_8669_112_2_18_3_7_48_165,(uint8*)"d", (SCI_GetTickCount()-cur_tick));
    }
#endif
}

/*****************************************************************************/
//  Description : copy wallpapaer to pagelayer
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
LOCAL void CopyWallpaperToPageLayer(void)
{
#if 0
    MMIWIDGET_CTRL_T *widget_ctrl_ptr = MMIAPIWIDGET_GetWidgetPtr(MMIWIDGET_CTRL_IDLE_ID);
    GUI_RECT_T         display_rect = {0, 0, 239, 319};
    uint16             page_width = 0;
    GUI_POINT_T        start_point = {0};
    
    if (PNULL == widget_ctrl_ptr)
    {
        return;
    }

#if 0
    if(MMIAPIIDLE_IsLiveWallpaper())
    {
        return;
    }
#endif    
    if (UILAYER_IsLayerActive(&widget_ctrl_ptr->page_layer)
//        && UILAYER_IsMultiLayerHandle(&widget_ctrl_ptr->page_layer))
          && (UILAYER_HANDLE_MULTI == UILAYER_GetHandleType(&widget_ctrl_ptr->page_layer)))
    {
        
        if (UILAYER_IsLayerActive(&widget_ctrl_ptr->wallpaper_layer)
//            && UILAYER_IsMultiLayerHandle(&widget_ctrl_ptr->wallpaper_layer))
              && (UILAYER_HANDLE_MULTI == UILAYER_GetHandleType(&widget_ctrl_ptr->wallpaper_layer)))
        {
            UILAYER_GetLayerPosition(&widget_ctrl_ptr->page_layer, &start_point.x, &start_point.y);
            display_rect = GetWidgetCtrlRect(widget_ctrl_ptr);
            page_width = GetWidgetPageWidth(widget_ctrl_ptr);
            //page 1
            display_rect.left = start_point.x;
            display_rect.right = (display_rect.left+page_width-1);
            UILAYER_SetLayerPosition(
                &widget_ctrl_ptr->wallpaper_layer,
                display_rect.left,
                display_rect.top
                );
            UILAYER_BltLayerToLayer(
                &widget_ctrl_ptr->page_layer,
                &widget_ctrl_ptr->wallpaper_layer,
                &display_rect,
                FALSE
                );
            //page 2
            display_rect.left+=page_width;
            display_rect.right+=page_width;
            UILAYER_SetLayerPosition(
                &widget_ctrl_ptr->wallpaper_layer,
                display_rect.left,
                display_rect.top
                );        
            UILAYER_BltLayerToLayer(
                &widget_ctrl_ptr->page_layer,
                &widget_ctrl_ptr->wallpaper_layer,
                &display_rect,
                FALSE
                );        
            //page 3
            display_rect.left+=page_width;
            display_rect.right+=page_width;
            UILAYER_SetLayerPosition(
                &widget_ctrl_ptr->wallpaper_layer,
                display_rect.left,
                display_rect.top
                );
            UILAYER_BltLayerToLayer(
                &widget_ctrl_ptr->page_layer,
                &widget_ctrl_ptr->wallpaper_layer,
                &display_rect,
                FALSE
                );
            //reset wallpaper_layer
            UILAYER_SetLayerPosition(&widget_ctrl_ptr->wallpaper_layer, 0, 0);
        }
    }
#endif
}

/*****************************************************************************/
//  Description : MMIWIDGET_GetWinHandleByWidgetId
//  Global resource dependence : 
//  Author:xiaoming.ren
//  Note: 
/*****************************************************************************/
PUBLIC MMI_HANDLE_T MMIWIDGET_GetWinHandleByWidgetId(uint32 widget_id)
{
    MMIWIDGET_CTRL_E ctrl_id    = MMIWIDGET_CTRL_IDLE_ID;
    MMI_HANDLE_T  widget_handle = 0;
    MMIWIDGET_CTRL_T * ctrl_ptr = PNULL;
    MMIWIDGET_LIST_E list_id    = 0;

    list_id = GetWidgetItemListId(widget_id);
    
    if (   (MMI_WIDGET_ACTIVE_LIST   != list_id)
        && (MMI_WIDGET_SHORTCUT_LIST != list_id))
	{
        return MMI_RESULT_FALSE;
	}

    ctrl_ptr = MMIAPIWIDGET_GetWidgetPtr(ctrl_id);

    if (PNULL == ctrl_ptr)
    {
        return MMI_RESULT_FALSE;
    }
    
    if (MMI_WIDGET_ACTIVE_LIST == list_id)
    {
        widget_handle = GetWinHandleByWidgetId(ctrl_ptr, widget_id);
    }

    return widget_handle;
}


#ifdef MMIWIDGET_VIBRATE_TICK_TEST
/*****************************************************************************/
//  Description : output text
//  Global resource dependence : 
//  Author:bin.ji
//  Note:
/*****************************************************************************/
LOCAL void OutputText(uint32 tick_count)
{
    uint8 temp_str[50] = {0};
    wchar temp_wstr[50] = {0};
    const GUI_RECT_T rect = {100, 100, 200, 120};
    MMI_STRING_T temp_string = {0};
    GUISTR_STATE_T temp_state = GUISTR_STATE_INVERSE;
    GUISTR_STYLE_T temp_style = {0};
    uint16 str_len = 0;
    GUI_LCD_DEV_INFO lcd_dev_info = {0};
            
    temp_string.wstr_ptr = temp_wstr;

    temp_style.font = SONG_FONT_16;
    temp_style.font_color = 0xFFFF;

    sprintf( (char*)temp_str, "vibrate : %d", tick_count );
    str_len = strlen((char*)temp_str);
    MMI_STRNTOWSTR( temp_wstr, 50, temp_str, str_len, str_len );
    temp_string.wstr_len = str_len;
    LCD_FillRect(&lcd_dev_info, rect, 0xFFFF);
    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_RECT_T      *)&rect,       //the fixed display area
        (const GUI_RECT_T      *)&rect,       //用户要剪切的实际区域
        (const MMI_STRING_T    *)&temp_string,
        &temp_style,
        temp_state,
        GUISTR_TEXT_DIR_AUTO
        );
}
#endif

/*****************************************************************************/
//  Description : MMIWIDGET_UpdateWidgetByWidgetId
//  Global resource dependence : 
//  Author:arvin.wu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWIDGET_UpdateWidgetByWidgetId(uint16 widget_id)
{
    MMIWIDGET_CTRL_T *widget_ctrl_ptr = MMIAPIWIDGET_GetWidgetPtr(MMIWIDGET_CTRL_IDLE_ID);
    MMI_HANDLE_T  win_handle = 0;
    GUI_RECT_T   widget_rect = {0};
    GUI_RECT_T   full_rect = {0};

    
    if(widget_ctrl_ptr && IsWidgetActive(widget_ctrl_ptr,widget_id))
    {
        win_handle = GetWinHandleByWidgetId(widget_ctrl_ptr, widget_id); 
        
        full_rect = MMITHEME_GetFullScreenRect();
        MMK_GetWinRect(win_handle, &widget_rect);
        if(GUI_IntersectRect(&widget_rect, full_rect,widget_rect))
        {
            //UILAYER_ClearRect(&widget_ctrl_ptr->current_paper_layer,widget_rect);
            //UILAYER_BltLayerToLayer(&widget_ctrl_ptr->current_paper_layer, &widget_ctrl_ptr->page_layer,&widget_rect,FALSE);
        }
    }
}

#endif


/*Edit by script, ignore 10 case. Thu Apr 26 19:01:10 2012*/ //IGNORE9527
