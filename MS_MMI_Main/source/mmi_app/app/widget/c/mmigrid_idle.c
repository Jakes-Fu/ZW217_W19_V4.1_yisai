/*****************************************************************************
** File Name:      mmigrid_idle.c                                            *
** Author:                                                                   *
** Date:           06/2011                                                   *
** Copyright:      2011 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe widget control              *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME               DESCRIPTION                             *
** 23/06/2011     hongbo.lan     Create
******************************************************************************/

#ifndef _MMIWIDGET_C_
#define _MMIWIDGET_C_

/*---------------------------------------------------------------------------*/
/*                         Include Files                                     */
/*---------------------------------------------------------------------------*/
#include "mmi_app_widget_trc.h"
#include "mmiwidget.h"
#include "mmi_image.h"
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
#include "mmiset_export.h"
#include "mmiset_display.h"
#include "mmidisplay_data.h"
#include "mmiset_wintab.h"
#include "mmi_mainmenu_export.h"
#include "mmiwidget_id.h"
#include "mmi_id.h"
#include "mmimp3_export.h"
#include "Mmi_wallpaper_export.h"
#include "mmk_tp.h"
#include "Guistatusbar.h"
#include "mmiwidget_image.h"
#include "mmiwidget_position.h"
#include "mmiwidget_text.h"
#include "guimenu.h"
#include "mmitheme_menu.h"
#include "mmitheme_mainmenu.h"
#include "graphics_draw.h"

#include "mmiwidget_export.h"
#include "mmiwidget_internal.h"
#include "mmiwidget_item.h"
#include "mmiwidget_grid.h"
#include "mmiwidget_page.h"
#include "mmiwidget_virtualwin.h"
#include "mmiwidget_wallpaper.h"

#ifdef WEATHER_SUPPORT
#include "mmiweather.h"
#endif
#include "mmiidle_export.h"
#ifdef DYNAMIC_MAINMENU_SUPPORT
#include "mmi_mainmenu_synchronize.h"
#endif
#ifdef LIVE_WALLPAPER_SUPPORT
#include "mmi_wallpaper_export.h"
#endif
#include "guictrl_api.h"
#include "mmi_menutable.h"
//#include "mmiss_export.h"
#include "mmiidle_statusbar.h"
#include "mmidisplay_data.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmiset_text.h"
#include "mmiset_display.h"
#ifdef PDA_UI_SUPPORT_U_IDLE
#include "mmigrid_uidle_se.h"
#include "u_launcher_export.h"
#endif
#ifdef PDA_UI_DROPDOWN_WIN
#include "mmidropdownwin_id.h"
#endif
#ifdef LIVE_WALLPAPER_FRAMEWORK_SUPPORT
#include "mmilivewallpaper_export.h"
#endif
#ifdef MMI_VIDEOWALLPAPER_SUPPORT
#include "mmiset_videowallpaper.h"
#endif
#include "mmistk_export.h"
#include "lcd_cfg.h"
#include "mmicc_export.h"
#ifdef UPB_IDLE_SUPPORT
#include "mmiidle_upb_export.h"
#endif

#include "mmiwidget_special_effect.h"
#include "mmiqbtheme_menuwin.h"

/*---------------------------------------------------------------------------*/
/*                         Macro Declaration                                 */
/*---------------------------------------------------------------------------*/

#define MMIWIDGET_LONG_TP_PEN_WIDTH     2 
#define MMIWIDGET_LONG_TP_PEN_RANGE     5
#define MMIWIDGET_MOVE_X_MIN_PIX        10
#define MMIWIDGET_MOVE_Y_MIN_PIX        10
#define MMIWIDGET_INT16_MIN_VALUE      -0x8000
#define MMIWIDGET_INVALID_BAR_POS       0xFF



#define MMIWIDGET_AUTO_SLIDE_TIMEOUT                1000 

#define MMIWIDGET_MAX_COUNT_LIMIT                   100     //超时计数

#define MMIWIDGET_EDGE_DIST                         30      //widget移到边缘留的空隙

#ifdef MAINLCD_SIZE_320X480
#ifdef UPB_IDLE_SUPPORT
#define MMIWIDGET_IDLE_UPB_SOFTKEY_COUNT               5     //IDLE界面softkey 个数
#define MMIWIDGET_IDLE_SOFTKEY_COUNT                   4     //IDLE界面softkey 个数
#else
#define MMIWIDGET_IDLE_SOFTKEY_COUNT                   4     //IDLE界面softkey 个数
#endif

#else
#define MMIWIDGET_IDLE_SOFTKEY_COUNT                   3     //IDLE界面softkey 个数
#define MMIWIDGET_IDLE_UPB_SOFTKEY_COUNT               MMIWIDGET_IDLE_SOFTKEY_COUNT     //IDLE界面softkey 个数

#endif

#ifdef MMI_SAM_THEME
#define MMIWIDGET_SAM_IDLE_SOFTKEY_COUNT               4     //sam style idle softkey count
#endif

//按比例缩放
#define ZOOM_OFFSET(offset, lcd_width, lcd_height)    (((offset) * (lcd_width) * 2 + (lcd_height)) / ((lcd_height) * 2))

#define MMIWIDGET_THUMBVIEW_ALPHA              80
//绝对值宏定义
/*lint -e767*/
//#define abs(x) (((x) >= 0) ? (x) : (-(x)))   

/*lint +e767*/

#ifdef MMI_SAM_THEME
#define MMIGRID_IDLE_SAMSUNG_SOFTKEY_HEIGHT         85
#define MMIGRID_IDLE_SAMSUNG_FONT_HEIGHT            20
#define MMIGRID_IDLE_SAMSUNG_RECYCLE_HEIGHT         80
#endif

/*---------------------------------------------------------------------------*/
/*                         Constant Variables                                */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         EXTERNAL DECLARE                                  */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         STATIC DEFINITION                                 */
/*---------------------------------------------------------------------------*/

//目前只支持idle界面widget
LOCAL MMI_IDLE_GRID_DATA_T *s_idle_grid_data_ptr = PNULL;


//记录当前ticks
LOCAL uint8  s_auto_slide_timer_id      = 0;

LOCAL BOOLEAN s_is_auto_slide          = FALSE;


LOCAL const  GUI_RECT_T s_thumb_rect_grp[] = 
{
    MMI_IDLE_GRID_THUMB1_RECT,
    MMI_IDLE_GRID_THUMB2_RECT,
    MMI_IDLE_GRID_THUMB3_RECT,
#if MMI_GRID_PAGE_NUM > 3   
    MMI_IDLE_GRID_THUMB4_RECT,
    MMI_IDLE_GRID_THUMB5_RECT,
#if MMI_GRID_PAGE_NUM > 5      
    MMI_IDLE_GRID_THUMB6_RECT,
    MMI_IDLE_GRID_THUMB7_RECT,    
#endif
#endif
};

//display rect
LOCAL GUI_RECT_T        s_display_rect[MMIGRID_SOFTKEY_MAX] = {0};
LOCAL MMIGRID_SOFTKEY_TYPE_E s_tp_down_type = MMIGRID_SOFTKEY_BG;

/*---------------------------------------------------------------------------*/
/*                         GLOBAL DEFINITION                                 */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         LOCAL FUNCTION DECLARE                            */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : rotate rect
//  Global resource dependence : 
//  Author:hongbo.lan
//  Note: 
/*****************************************************************************/
LOCAL GUI_RECT_T RotateRect(
                            GUI_RECT_T rect,
                            BOOLEAN is_h_to_v    //横屏切竖屏
                            );

/*****************************************************************************/
//  Description : get widget page width
//  Global resource dependence : 
//  Author:hongbo.lan
//  Note: 
/*****************************************************************************/
LOCAL uint16 GetWidgetPageWidth(
                               MMI_IDLE_GRID_DATA_T *idle_grid_ptr
                               );

LOCAL GUI_RECT_T GetWidgetPageRect(
                               MMI_IDLE_GRID_DATA_T *idle_grid_ptr
                               );


/*****************************************************************************/
//  Description : rotate point
//  Global resource dependence : 
//  Author:hongbo.lan
//  Note: 
/*****************************************************************************/
LOCAL GUI_POINT_T RotatePoint(
                            GUI_POINT_T point,
                            BOOLEAN is_h_to_v    //横屏切竖屏
                            );

/*****************************************************************************/
//  Description : handle widget tp move
//  Global resource dependence : 
//  Author:hongbo.lan
//  Note: 
/*****************************************************************************/
LOCAL void DisplaySoftkey(
                         MMI_IDLE_GRID_DATA_T *idle_grid_ptr
                         );

/*****************************************************************************/
//  Description : handle widget tp move with timer
//  Global resource dependence : 
//  Author:hongbo.lan
//  Note: 
/*****************************************************************************/
LOCAL void PlaySlideAnim(
                         MMI_IDLE_GRID_DATA_T *idle_grid_ptr,
                         MMIWIDGET_PEN_MOVE_DIRECT_T move_direct,
                         int32 dst_page_index,
                         int32  slide_flag
                         );
LOCAL void SlideAnimCallback(MMI_IDLE_GRID_DATA_T *idle_grid_ptr);

/*****************************************************************************/
//  Description : create widget layer
//  Global resource dependence : 
//  Author:hongbo.lan
//  Note: 
/*****************************************************************************/
LOCAL void CreatePageLayer(
                             MMI_IDLE_GRID_DATA_T *idle_grid_ptr
                             );

/*****************************************************************************/
//  Description : handle widget tp move
//  Global resource dependence : 
//  Author:hongbo.lan
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN HandleSlidePageTpUp(
                                 MMI_IDLE_GRID_DATA_T *idle_grid_ptr,
                                 GUI_POINT_T point
                                 );

/*****************************************************************************/
//  Description : handle widget tp move
//  Global resource dependence : 
//  Author:hongbo.lan
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN HandleSlidePageTpMove(
                                 MMI_IDLE_GRID_DATA_T *idle_grid_ptr,
                                 GUI_POINT_T point
                                 );

LOCAL BOOLEAN SlidePage(
                                 MMI_IDLE_GRID_DATA_T *idle_grid_ptr,
                                 int32 dest_page_index
                                 );
LOCAL BOOLEAN SlideOnePage(
                          MMI_IDLE_GRID_DATA_T *idle_grid_ptr,
                          int32 dest_page_index, //for now ,only support sliding for 1 page
                          MMIWIDGET_PEN_MOVE_DIRECT_T direction,
                          int32 frame
                          );    

/*****************************************************************************/
//  Description : save tp move point
//  Global resource dependence : 
//  Author:hongbo.lan
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN SaveTpMovePoint(
                              MMI_IDLE_GRID_DATA_T *idle_grid_ptr,
                              GUI_POINT_T point
                              );

/*****************************************************************************/
//  Description : widget子窗口的消息处理函数
//  Global resource dependence : 
//  Author:  hongbo.lan
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleChildWidgetWinMsg(
                                MMI_WIN_ID_T      win_id,
                                MMI_MESSAGE_ID_E  msg_id,
                                DPARAM            param
                                );

/*****************************************************************************/
//  Description : destroy page layer
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void DestroyWidgetPageLayer(
                              MMI_IDLE_GRID_DATA_T *idle_grid_ptr
                              );

/*****************************************************************************/
//  Description : handle widget tp up
//  Global resource dependence : 
//  Author:hongbo.lan
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN HandleGridTpUp(
                               MMI_IDLE_GRID_DATA_T *idle_grid_ptr,
                               DPARAM            param
                               );

/*****************************************************************************/
//  Description : handle widget tp down
//  Global resource dependence : 
//  Author:hongbo.lan
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN HandleGridTpDown(
                                 MMI_IDLE_GRID_DATA_T *idle_grid_ptr,
                                 DPARAM            param
                                 );

/*****************************************************************************/
//  Description : handle widget tp move
//  Global resource dependence : 
//  Author:hongbo.lan
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN HandleGridTpMove(
                                 MMI_IDLE_GRID_DATA_T *idle_grid_ptr,
                                 DPARAM            param
                                 );

/*****************************************************************************/
//  Description : handle widget tp msg
//  Global resource dependence : 
//  Author:  hongbo.lan
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleGridTPMsg(
                                     MMI_IDLE_GRID_DATA_T* idle_grid_ptr,
                                     MMI_MESSAGE_ID_E  msg_id,
                                     DPARAM            param
                                     );

/*****************************************************************************/
//  Description : handle widget tp long down
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN HandleGridTpLongDown(
                                 MMI_IDLE_GRID_DATA_T *idle_grid_ptr,
                                 DPARAM                      param
                                 );

/*****************************************************************************/
//  Description : create softkey layer
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note: 
/*****************************************************************************/
LOCAL void CreateSoftkeyLayer(MMI_IDLE_GRID_DATA_T *idle_grid_ptr);

LOCAL void CreateTitlebarLayer(MMI_IDLE_GRID_DATA_T *idle_grid_ptr);
LOCAL void CreateRecycleLayer(MMI_IDLE_GRID_DATA_T *idle_grid_ptr);

/*****************************************************************************/
//  Description : destroy desktop elve layer
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note: 
/*****************************************************************************/
LOCAL void DestroySoftkeyLayer(
                              MMI_IDLE_GRID_DATA_T *idle_grid_ptr
                              );

LOCAL void DestroyTitlebarLayer(MMI_IDLE_GRID_DATA_T *idle_grid_ptr);
LOCAL void DestroyRecycleLayer(MMI_IDLE_GRID_DATA_T *idle_grid_ptr);


/*****************************************************************************/
//  Description : tp position for moving along X direction is valid or not.
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsTpXMoveValid(MMI_IDLE_GRID_DATA_T *idle_grid_ptr, GUI_POINT_T point);

/*****************************************************************************/
//  Description : tp position for moving along Y direction is valid or not.
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsTpYMoveValid(MMI_IDLE_GRID_DATA_T *idle_grid_ptr, GUI_POINT_T point);

/*****************************************************************************/
//  Description : reset widget tp state.
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note: 
/*****************************************************************************/
LOCAL void ResetWidgetTpState(MMI_IDLE_GRID_DATA_T *idle_grid_ptr);

/*****************************************************************************/
//  Description : try to reset moving child win postion.
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN ResetChildWinPosition(MMI_IDLE_GRID_DATA_T *idle_grid_ptr, GUI_POINT_T point);

/*****************************************************************************/
//  Description : initial shortcut idle grid rect information.
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note:
/*****************************************************************************/
LOCAL void InitScGridRect(MMI_IDLE_GRID_DATA_T *idle_grid_ptr);

/*****************************************************************************/
//  Description : GridSaveOldCellInfo
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note:
/*****************************************************************************/
LOCAL void ClearOldGridArea(MMI_IDLE_GRID_DATA_T *idle_grid_ptr);

/*****************************************************************************/
//  Description : handle widget tp down
//  Global resource dependence : 
//  Author:hongbo.lan
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN SetGridTPClickType(
                                 MMI_IDLE_GRID_DATA_T *idle_grid_ptr,
                                 MMI_GRID_TP_CLICK_TYPE_E click_type
                                 );

/*****************************************************************************/
//  Description : judge auto slide or not.
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note: 
/*****************************************************************************/
LOCAL void HandleGridAutoSlideEvent(MMI_IDLE_GRID_DATA_T *idle_grid_ptr, GUI_POINT_T point);

/*****************************************************************************/
//  Description : reset softkey status.
//  Global resource dependence : 
//  Author:  hongbo.lan
//  Note:
/*****************************************************************************/
LOCAL void ResetSoftkeyState(MMI_IDLE_GRID_DATA_T *idle_grid_ptr);

/*****************************************************************************/
//  Description : initial grid idle softkey.
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note:
/*****************************************************************************/
LOCAL void InitSoftkey(MMI_IDLE_GRID_DATA_T *idle_grid_ptr);
LOCAL void InitAndroidSoftkey(MMI_IDLE_GRID_DATA_T *idle_grid_ptr);

/*****************************************************************************/
//  Description : DrawSoftKeyButton
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note:
/*****************************************************************************/
LOCAL void DrawSoftKeyButton(MMI_IDLE_GRID_DATA_T *idle_grid_ptr);
LOCAL void PlaySoftkeyEnterAnim(MMI_IDLE_GRID_DATA_T *idle_grid_ptr);
LOCAL void PlaySoftkeyLeaveAnim(MMI_IDLE_GRID_DATA_T *idle_grid_ptr);
LOCAL void PlayRecycleEnterAnim(MMI_IDLE_GRID_DATA_T *idle_grid_ptr);
LOCAL void PlayRecycleLeaveAnim(MMI_IDLE_GRID_DATA_T *idle_grid_ptr);
LOCAL void DrawRecycle(MMI_IDLE_GRID_DATA_T *idle_grid_ptr);
LOCAL GUI_RECT_T GetRecycleRect(MMI_IDLE_GRID_DATA_T *idle_grid_ptr);

LOCAL uint16 GetPageTitleBarHeight(MMI_IDLE_GRID_DATA_T *idle_grid_ptr);

LOCAL GUI_RECT_T GetPageTitleBarRect(MMI_IDLE_GRID_DATA_T *idle_grid_ptr);

/*****************************************************************************/
//  Description : draw page title bar.
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note:
/*****************************************************************************/
LOCAL void DrawPageTitleBar(MMI_IDLE_GRID_DATA_T *idle_grid_ptr, int32 current_page_index);

/*****************************************************************************/
//  Description : Check the moving item and recycle bin.
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CheckGridItemRecycleStat(MMI_IDLE_GRID_DATA_T *idle_grid_ptr);

/*****************************************************************************/
//  Description : redraw top layer
//  Global resource dependence : 
//  Author:hongbo.lan
//  Note: 
/*****************************************************************************/
LOCAL void DrawAll(MMI_IDLE_GRID_DATA_T *idle_grid_ptr); /*lint !e401*/

/*****************************************************************************/
//  Description : Display item on recycle bin with red color.
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note:
/*****************************************************************************/
LOCAL void DisplayMoveItemOnRecycle(MMI_IDLE_GRID_DATA_T *idle_grid_ptr);

/*****************************************************************************/
//  Description : display page thumbnail view.
//  Global resource dependence : 
//  Author:  hongbo.lan
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN DisplayGridPageThumbView(MMI_IDLE_GRID_DATA_T *idle_grid_ptr);
LOCAL BOOLEAN EnterThumbviewState(MMI_IDLE_GRID_DATA_T *idle_grid_ptr);
LOCAL BOOLEAN ExitThumbviewState(MMI_IDLE_GRID_DATA_T *idle_grid_ptr);

LOCAL void SetBufferAlpha(
                     uint8 *dst_buf_ptr,
                     uint32 dst_buf_width,
                     DISPLAY_DATA_TYPE_E dst_data_type,
                     uint32 blt_width,
                     uint32 blt_height
                     );
// LOCAL void SetBufferAlphaEx(GUI_LCD_DEV_INFO *layer_ptr, GUI_RECT_T *rect_ptr, uint32 alpha);

/*****************************************************************************/
//  Description : handle page grid thumbview tp up.
//  Global resource dependence : 
//  Author:hongbo.lan
//  Note: 
/*****************************************************************************/
LOCAL void HandlePageThumbViewTpUp(MMI_IDLE_GRID_DATA_T *idle_grid_ptr, GUI_POINT_T point);


/*****************************************************************************/
//  Description : Display item on recycle bin with red color.
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note:
/*****************************************************************************/
LOCAL void FillLayerWithMaskColorEx(GUI_LCD_DEV_INFO *layer_ptr, GUI_RECT_T *rect_ptr, uint32 mask_color);


/*****************************************************************************/
//  Description : reset movable mode action.
//  Global resource dependence : 
//  Author: hongbo.lan  
//  Note:
/*****************************************************************************/
LOCAL void ResetGridMovableAction(MMI_IDLE_GRID_DATA_T *idle_grid_ptr);

/*****************************************************************************/
//  Description : handle grid item on movable mode tp down action.
//  Global resource dependence : 
//  Author:hongbo.lan
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN HandleMoveModeTpDown(
                               MMI_IDLE_GRID_DATA_T *idle_grid_ptr,
                               DPARAM            param
                               );


/*****************************************************************************/
//  Description : destroy thumbview layer
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note: 
/*****************************************************************************/
LOCAL void DestroyGridThumbViewLayer(MMI_IDLE_GRID_DATA_T *idle_grid_ptr);

/*****************************************************************************/
//  Description : Draw Icon EditRect
//  Global resource dependence : 
//  Author: nan.ji
//  Note:
/*****************************************************************************/
LOCAL void DrawIconDeleteRect(MMI_IDLE_GRID_DATA_T *idle_grid_ptr, 
                              GUI_RECT_T display_rc, 
                              GUI_LCD_DEV_INFO *dev_info_ptr);


/*****************************************************************************/
//  Description : Set idle edit able
//  Global resource dependence : 
//  Author: nan.ji
//  Note:
/*****************************************************************************/
LOCAL void SetIsEditable(BOOLEAN is_edit);

LOCAL BOOLEAN PointIsInEditIconRect(
                               MMI_IDLE_GRID_DATA_T *idle_grid_ptr,
                               MMI_GRID_TP_MOVE_INFO_T      *tp_move_ptr,
							   GUI_POINT_T point
                               );

#if defined BACKGROUND_GRID_LINE_LAYER

/*****************************************************************************/
//  Description : draw edit bag
//  Global resource dependence : 
//  Author:nan,ji
//  Note: 
/*****************************************************************************/
LOCAL void DrawEditBag(GUI_LCD_DEV_INFO *idle_grid_ptr);

/*****************************************************************************/
//  Description : destroy thumbview layer
//  Global resource dependence : 
//  Author: nan.ji
//  Note: 
/*****************************************************************************/
LOCAL void DestroyEditBgLayer(
                              MMI_IDLE_GRID_DATA_T *idle_grid_ptr
                               );

/*****************************************************************************/
//  Description : create edit bg layer
//  Global resource dependence : 
//  Author: nan.ji
//  Note: 
/*****************************************************************************/
LOCAL void CreateEditBgLayer(
                             MMI_IDLE_GRID_DATA_T *idle_grid_ptr
                             );
                         
/*****************************************************************************/
//  Description : Display GridPage EditBg 
//  Global resource dependence : 
//  Author:  nan.ji
//  Note:
/*****************************************************************************/
LOCAL void DisplayEditBgLayer(
                             MMI_IDLE_GRID_DATA_T *idle_grid_ptr
                             );
#endif

/*****************************************************************************/
//  Description : Set idle edit able
//  Global resource dependence : 
//  Author: nan.ji
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetIsEditable(void);

/*****************************************************************************/
//  Description : get tp type
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMIGRID_SOFTKEY_TYPE_E GetTpType(
                                  GUI_POINT_T   tp_point
                                  );

/*****************************************************************************/
//  Description : set display rect
//  Global resource dependence : 
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL void SetDisplayRect(
                          GUI_RECT_T           rect,
                          MMIGRID_SOFTKEY_TYPE_E    tp_type
                          );
						  
/*****************************************************************************/
//  Description : display number icon in idle 
//  Global resource dependence : 
//  Author: xinhe.yan
//  Note:在有未接来电或短信时，显示其数目
/*****************************************************************************/
// LOCAL MMI_RESULT_E DisplayIdleNumberIcon(  
//                                          MMI_IDLE_GRID_DATA_T *idle_grid_ptr,
//                                          MMI_MENU_ID_T menu_id,
//                                          GUI_RECT_T          disp_rect,  //number icon rect
//                                          GUI_LCD_DEV_INFO    lcd_dev_info //icon layer handle
//                                          );

LOCAL uint16 GetWidgetMaxGridNum(MMI_IDLE_GRID_DATA_T *idle_grid_ptr);

LOCAL uint16 GetPageGridNum(MMI_IDLE_GRID_DATA_T *idle_grid_ptr);

LOCAL void GetPageGridRowCol(int16 *prow, int16 *pcol, MMI_IDLE_GRID_DATA_T *idle_grid_ptr);

/*****************************************************************************/
//  Description : HandleKeyDownMsg
//  Global resource dependence : 
//  Author: juan.wu
//  Note:处理keydown消息
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleKeyDownMsg(MMI_IDLE_GRID_DATA_T * idle_grid_ptr,MMI_MESSAGE_ID_E  msg_id);

/*****************************************************************************/
//  Description : GetCurPageGridType
//  Global resource dependence : 
//  Author: juan.wu
//  Note:获得grid_idle每个格子的类型，包括index,widget_id,shortcut_id...
/*****************************************************************************/
LOCAL void GetCurPageGridType(
                                MMI_IDLE_GRID_DATA_T * idle_grid_ptr,
                                MMIGRID_GRID_INDEX_TYPE_T *grid_type
                                );
                                
/*****************************************************************************/
//  Description : GetNextValidGrid
//  Global resource dependence : 
//  Author: juan.wu
//  Note:获得下一个格子的索引值
/*****************************************************************************/
LOCAL int32   GetNextValidGrid(MMI_IDLE_GRID_DATA_T * idle_grid_ptr,
                                int32 first_grid_index,
                                MMIGRID_GRID_INDEX_TYPE_T *grid_type
                                );

/*****************************************************************************/
//  Description : GetPreValidGrid
//  Global resource dependence : 
//  Author: juan.wu
//  Note:获得上一个有效格子的索引值
/*****************************************************************************/
LOCAL int32   GetPreValidGrid(MMI_IDLE_GRID_DATA_T * idle_grid_ptr,
                                int32   last_grid_index,
                                MMIGRID_GRID_INDEX_TYPE_T *grid_type
                                );

/*****************************************************************************/
//  Description : SetItemFocus
//  Global resource dependence : 
//  Author: juan.wu
//  Note:设置focus状态
/*****************************************************************************/                                
LOCAL MMI_RESULT_E SetItemFocus(
                        MMI_IDLE_GRID_DATA_T * idle_grid_ptr,
                        uint32 index, 
                        MMIGRID_GRID_INDEX_TYPE_T  *grid_type,
                        MMI_MESSAGE_ID_E  msg_id
                        );
/*****************************************************************************/
//  Description : HandleWebKeyDown
//  Global resource dependence : 
//  Author: juan.wu
//  Note:处理WEB消息
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleWebKeyMsg(MMI_IDLE_GRID_DATA_T * idle_grid_ptr,MMI_MESSAGE_ID_E  msg_id);

/*****************************************************************************/
//  Description : HandleKeyUpMsg
//  Global resource dependence : 
//  Author: juan.wu
//  Note:处理up消息
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleKeyUpMsg(MMI_IDLE_GRID_DATA_T * idle_grid_ptr,MMI_MESSAGE_ID_E  msg_id);

/*****************************************************************************/
//  Description : HandleSlidePageMove
//  Global resource dependence : 
//  Author: juan.wu
//  Note:处理LEFT RIGHT消息
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSlidePageMove(MMI_IDLE_GRID_DATA_T * idle_grid_ptr,MMI_MESSAGE_ID_E  msg_id);
#ifdef MMI_IDLE_THUMBVIEW_SLIDE
/*****************************************************************************/
//  Description :  
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsInThumbviewSlideRange(  GUI_POINT_T point);

/*****************************************************************************/
//  Description :  
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
LOCAL uint8 GetCurThumbviewPage(GUI_POINT_T point);

/*****************************************************************************/
//  Description :  
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
LOCAL void CalcCurThumbviewPageCenterPosition(GUI_POINT_T point,GUI_POINT_T*out_point);

/*****************************************************************************/
//  Description :  
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
LOCAL void GetCurThumbviewPageSize(GUI_POINT_T point,uint16* width,uint16* height);

/*****************************************************************************/
//  Description :  
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
LOCAL void GetPreThumbviewPageSize(GUI_POINT_T point,uint16* width,uint16* height);

/*****************************************************************************/
//  Description :  
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
LOCAL void GetNextThumbviewPageSize(GUI_POINT_T point,uint16* width,uint16* height);


/*****************************************************************************/
//  Description :  
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
LOCAL void CalcCurThumbviewPagePosition(GUI_POINT_T point,GUI_POINT_T*out_point);

/*****************************************************************************/
//  Description :  
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
LOCAL void CalcPreThumbviewPagePosition(GUI_POINT_T point,GUI_POINT_T*out_point);

/*****************************************************************************/
//  Description :  
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
LOCAL void CalcNextThumbviewPagePosition(GUI_POINT_T point,GUI_POINT_T*out_point);

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void CreateThumbViewSlideLayer(MMI_IDLE_GRID_DATA_T *idle_grid_ptr);

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void DestroyGridThumbViewSlideLayer(MMI_IDLE_GRID_DATA_T *idle_grid_ptr);

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:  
//  Note:
/*****************************************************************************/
LOCAL void DisplayGridSlidePageThumbView(MMI_IDLE_GRID_DATA_T *idle_grid_ptr,GUI_POINT_T point)
;
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void DrawSlidePageTitleBar(GUI_POINT_T point);

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL GUI_LCD_DEV_INFO CreateSinglePageThumbLayer(MMI_IDLE_GRID_DATA_T *idle_grid_ptr,int16 width,int16 height);

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN HandleSlideThumbViewPageTpMove(
                                 MMI_IDLE_GRID_DATA_T *idle_grid_ptr,
                                 GUI_POINT_T point
                                 );

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void BltBufferWithAlpha(
                     uint8 *src_buf_ptr,
                     uint32 src_buf_width,
                     DISPLAY_DATA_TYPE_E src_data_type,
                     uint8 *dst_buf_ptr,
                     uint32 dst_buf_width,
                     DISPLAY_DATA_TYPE_E dst_data_type,
                     uint32 blt_width,
                     uint32 blt_height,
                     uint32 dst_alpha
                     );

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL GUI_LCD_DEV_INFO CreateThumbViewSlideBgLayer(MMI_IDLE_GRID_DATA_T *idle_grid_ptr);

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void DestroyThumbViewSlideBgLayer(MMI_IDLE_GRID_DATA_T *idle_grid_ptr);

#endif//MMI_IDLE_THUMBVIEW_SLIDE


#ifdef BAR_PHONE_WIDGET 
LOCAL void UpdateSoftKeyText(MMI_IDLE_GRID_DATA_T * idle_grid_ptr);

LOCAL BOOLEAN GetCurPageWidgetIdAndIndex(
                                MMI_IDLE_GRID_DATA_T * idle_grid_ptr,
                                uint32 *widget_id,
                                uint32 *index
                                );
                                
#endif

#ifdef PDA_UI_SUPPORT_U_IDLE
/*****************************************************************************/
//  Description : draw last frame for uidle effect.
//  Global resource dependence : 
//  Author:hongbo.lan
//  Note: 
/*****************************************************************************/
LOCAL void DrawUIdleLastFrame(MMI_IDLE_GRID_DATA_T *idle_grid_ptr, uint16 dst_page_idx);
#endif

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

#if defined BACKGROUND_GRID_LINE_LAYER
/*****************************************************************************/
//  Description : draw edit bag
//  Global resource dependence : 
//  Author:nan.ji
//  Note: 
/*****************************************************************************/
LOCAL void DrawEditBag(GUI_LCD_DEV_INFO *dev_info_ptr)
{
    uint32 grid_idx = 0;
    uint16 page_width = 0;
    GUI_RECT_T rect = {0};
    GUI_RECT_T temp_rect = {0};
	GUI_RECT_T layer_rect = {0};
    MMI_IDLE_GRID_DATA_T *idle_grid_ptr = (MMI_IDLE_GRID_DATA_T *)MMIAPIGRID_GetGridPtr();
    uint32 page_grid_num = GetPageGridNum(idle_grid_ptr);
    page_width = GetWidgetPageWidth(idle_grid_ptr);

	layer_rect = UILAYER_GetLayerRect(dev_info_ptr);
				
	UILAYER_ClearRect(dev_info_ptr, layer_rect);
    for (grid_idx = 0; grid_idx < page_grid_num; grid_idx++)
    {
		rect = idle_grid_ptr->page_grid_rect[grid_idx];
			temp_rect = rect;	
			rect.bottom = rect.top + 1;      //row
			temp_rect.right = temp_rect.left + 1; //col

			//draw grid
			if (rect.top != MMI_STATUSBAR_HEIGHT)
			{
				FillLayerWithMaskColor(dev_info_ptr, &rect, RGB5652ARGB8888(MMI_GRAY_WHITE_COLOR, 255));/*lint !e648 !e572 !e778*/
			}
			if (temp_rect.left != 0
				&& temp_rect.left != MMI_MAINSCREEN_WIDTH)
			{
				FillLayerWithMaskColor(dev_info_ptr, &temp_rect, RGB5652ARGB8888(MMI_GRAY_WHITE_COLOR, 255));/*lint !e648 !e572 !e778*/
			}
	}
}

/*****************************************************************************/
//  Description : Display GridPage EditBg 
//  Global resource dependence : 
//  Author:  nan.ji
//  Note:
/*****************************************************************************/
LOCAL void DisplayEditBgLayer(MMI_IDLE_GRID_DATA_T *idle_grid_ptr)
{    
    uint32      page_layer_size = MMIWIDGET_GetPageLayerSize(idle_grid_ptr);

    if(page_layer_size < 3)
    {
        return;
    }
	if(idle_grid_ptr->is_editable)
	{  
        DrawEditBag(&idle_grid_ptr->editbg_layer);
    }
}

/*****************************************************************************/
//  Description : create edit bg layer
//  Global resource dependence : 
//  Author: nan.ji
//  Note: 
/*****************************************************************************/
LOCAL void CreateEditBgLayer(MMI_IDLE_GRID_DATA_T *idle_grid_ptr)
{
    uint16           lcd_width           = 0;
    uint16           lcd_height          = 0;
    GUI_LCD_DEV_INFO widget_layer_handle = {0};
    UILAYER_CREATE_T create_info         = {0}; 
    int32           error = 0;
    uint32      page_layer_size = MMIWIDGET_GetPageLayerSize(idle_grid_ptr);

    if(page_layer_size == 1)
    {
        idle_grid_ptr->editbg_layer.block_id = UILAYER_NULL_HANDLE;        
        return;
    }
    //SetIsEditable(TRUE);
    
	GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &lcd_width, &lcd_height); 

    if (UILAYER_HANDLE_MULTI != UILAYER_GetHandleType(&idle_grid_ptr->editbg_layer))
    {
        create_info.lcd_id          = LCD_ID_0;
        create_info.owner_handle    = idle_grid_ptr->win_id;
        create_info.offset_x        = 0;
        create_info.offset_y        = 0;//MMI_STATUSBAR_HEIGHT + 1;
        create_info.width           = lcd_width;
        create_info.height          = lcd_height;// - MMI_STATUSBAR_HEIGHT - MMIGRID_IDLE_SOFTKEY_HEIGHT;
        create_info.is_bg_layer     = FALSE;
        create_info.is_static_layer = FALSE;   
        
        error = UILAYER_CreateLayer(&create_info, &widget_layer_handle);
            
        UILAYER_SetLayerColorKey(&widget_layer_handle, TRUE, UILAYER_TRANSPARENT_COLOR);
    
        idle_grid_ptr->editbg_layer = widget_layer_handle;
    }
    
    if (UILAYER_IsLayerActive(&widget_layer_handle))
    {
        UILAYER_Clear(&widget_layer_handle);   
        //SCI_TRACE_LOW:"MMIGRID_IDLE CreateEditBgLayer error = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIGRID_IDLE_1548_112_2_18_3_7_0_8,(uint8*)"d", error);
    }
}
/*****************************************************************************/
//  Description : destroy Editbg layer
//  Global resource dependence : 
//  Author: nan.ji
//  Note: 
/*****************************************************************************/
LOCAL void DestroyEditBgLayer(MMI_IDLE_GRID_DATA_T *idle_grid_ptr)
{
    if (PNULL == idle_grid_ptr)
    {
        //SCI_TRACE_LOW:"DestroyMoveLayer() PNULL == idle_grid_ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIGRID_IDLE_1559_112_2_18_3_7_0_9,(uint8*)"");
        return;
    }
 
    if (UILAYER_HANDLE_MULTI == UILAYER_GetHandleType(&idle_grid_ptr->editbg_layer))    
    {
        UILAYER_RELEASELAYER(&(idle_grid_ptr->editbg_layer));   /*lint !e506 !e774*/
    }
}
#endif

/*****************************************************************************/
//  Description : Draw Icon EditRect
//  Global resource dependence : 
//  Author: nan.ji
//  Note:
/*****************************************************************************/
LOCAL void DrawIconDeleteRect(MMI_IDLE_GRID_DATA_T *idle_grid_ptr, 
                              GUI_RECT_T display_rc, 
                              GUI_LCD_DEV_INFO *dev_info_ptr)
{	
	GUIRES_DisplayImg(PNULL,
		&display_rc,
		PNULL,
		idle_grid_ptr->win_id,
		idle_grid_ptr->delect_icon_id, 
		(const GUI_LCD_DEV_INFO*)dev_info_ptr);
}


/*****************************************************************************/
//  Description : Set idle edit able
//  Global resource dependence : 
//  Author: nan.ji
//  Note:
/*****************************************************************************/
LOCAL void SetIsEditable(BOOLEAN is_edit)
{
    MMI_IDLE_GRID_DATA_T *idle_grid_ptr = (MMI_IDLE_GRID_DATA_T *)MMIAPIGRID_GetGridPtr();

	idle_grid_ptr->is_editable = is_edit;
}

/*****************************************************************************/
//  Description : Set idle edit able
//  Global resource dependence : 
//  Author: nan.ji
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetIsEditable(void)
{
    MMI_IDLE_GRID_DATA_T *idle_grid_ptr = (MMI_IDLE_GRID_DATA_T *)MMIAPIGRID_GetGridPtr();

	return idle_grid_ptr->is_editable;
}

/*****************************************************************************/
//  Description : PointIsInEditIconRect
//  Global resource dependence : 
//  Author:nan.ji
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN PointIsInEditIconRect(
                               MMI_IDLE_GRID_DATA_T *idle_grid_ptr,
                               MMI_GRID_TP_MOVE_INFO_T      *tp_move_ptr,
							   GUI_POINT_T point
                               )
{
    GUI_RECT_T rect = {0};
    GUI_RECT_T	win_rect = {0};
    uint16 img_width = 0;
    uint16 img_height = 0;
    uint16 grid_idx = 0;
    MMIGRID_ITEM_TYPE_E press_type = 0;    
    BOOLEAN result =  FALSE;
    
	GUIRES_GetImgWidthHeight(&img_width, &img_height, idle_grid_ptr->delect_icon_id,idle_grid_ptr->win_id);
	
    press_type = MMIWIDGET_GetGridTpPressedItem(idle_grid_ptr, point, &grid_idx);
	if (MMIGRID_TP_CLICK_WIDGET == tp_move_ptr->click_type)
	{
	    if(MMIGRID_ITEM_TYPE_WIDGET != press_type)
	    {
	        return result;
	    }
        if(grid_idx != tp_move_ptr->click_id)
        {
            return result;
        }

		//MMK_GetWinRect(item_ptr->item_info.win_id, &win_rect);
		MMIWIDGET_GetWinRect(idle_grid_ptr, tp_move_ptr->click_id,  &win_rect);
	    rect = win_rect;	    
        rect.right = rect.left + img_width*2;
        rect.bottom = rect.top + img_height*2;
	}
	else if (MMIGRID_TP_CLICK_SHORTCUT == tp_move_ptr->click_type)
	{
	    if(MMIGRID_ITEM_TYPE_ICON != press_type)
	    {
	        return result;
	    }
	    if(tp_move_ptr->click_id != GET_NODE_ID(idle_grid_ptr->grid_data_tab[idle_grid_ptr->cur_page_index][grid_idx]))
	    {
	        return result;
	    }
        rect = idle_grid_ptr->page_grid_rect[grid_idx];        
        rect.right = rect.left + img_width*1;
        rect.bottom = rect.top + img_height*1;
	}	
	
	if(GUI_PointIsInRect( point, rect))
	{
	    result =  TRUE;
	}

    return result;
}

/*****************************************************************************/
//  Description : create auto slide timer
//  Global resource dependence : s_auto_slide_timer_id
//  Author: hongbo.lan 
//  Note:
/*****************************************************************************/
LOCAL void CreateAutoSlideTimer(MMI_HANDLE_T win_id, uint32 time_out)
{     
    if(s_auto_slide_timer_id == 0)
    {
        s_auto_slide_timer_id = MMK_CreateWinTimer(win_id, time_out, FALSE);
    }
}

/*****************************************************************************/
//  Description : stop auto slide timer
//  Global resource dependence : s_auto_slide_timer_id
//  Author:  hongbo.lan
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
//  Description : handle widget tp msg
//  Global resource dependence : 
//  Author:  hongbo.lan
//  Note:
/*****************************************************************************/
#ifdef TOUCH_PANEL_SUPPORT
LOCAL MMI_RESULT_E HandleGridTPMsg(
                                     MMI_IDLE_GRID_DATA_T* idle_grid_ptr,
                                     MMI_MESSAGE_ID_E  msg_id,
                                     DPARAM            param
                                     )
{
    GUI_POINT_T        tp_point = {0};
    MMI_RESULT_E       result = MMI_RESULT_TRUE;
#ifdef SCH_ASSISTANT_WIDGET
    BOOLEAN is_send=FALSE;
#endif
    MMIGRID_SOFTKEY_TYPE_E tp_up_type = MMIGRID_SOFTKEY_BG;
    tp_point.x = MMK_GET_TP_X(param);
    tp_point.y = MMK_GET_TP_Y(param);
    
    switch (msg_id)
    {
    case MSG_TP_PRESS_DOWN:       
        if(MMIGRID_TP_CLICK_INVALID == idle_grid_ptr->movable_tp_info.click_type)
        {
            s_tp_down_type = GetTpType(tp_point);            
            
            if (   MMIGRID_SOFTKEY_LEFT == s_tp_down_type
                || MMIGRID_SOFTKEY_MIDDLE == s_tp_down_type
                || MMIGRID_SOFTKEY_RIGHT == s_tp_down_type
                || MMIGRID_SOFTKEY_ADD_1== s_tp_down_type
                #ifdef UPB_IDLE_SUPPORT
                || MMIGRID_SOFTKEY_ADD_2 == s_tp_down_type
                #endif
                
                )
            {  
                idle_grid_ptr->softkey_info[s_tp_down_type].is_pressed = TRUE;
                DisplaySoftkey(idle_grid_ptr);
                break;
            }
        }
        if(s_tp_down_type == 0)
        {
            HandleGridTpDown(idle_grid_ptr, param);            
        }    
        break;
        
    case MSG_TP_PRESS_UP:
#ifdef SCH_ASSISTANT_WIDGET//for NEWMS00179399
        if(!MMIAPIWIDGET_IsScheduleOnTop())
        {
            is_send=TRUE;
        }
#endif
        
        tp_up_type = GetTpType(tp_point);    
        if (   0 != s_tp_down_type)
        {              
            if(s_tp_down_type == tp_up_type)
            {
                idle_grid_ptr->previous_win_state = MSG_LOSE_FOCUS;
                
                switch(idle_grid_ptr->softkey_info[s_tp_down_type].normal_img_id)
                {
                    case IMAGE_MAINMENU_ICON_DIAL:
                        MMIAPIIDLE_EnterDialWin();
                        break;
                    case IMAGE_MAINMENU_ICON_MESSAGE:
                        MMIAPISMS_OpenMessageMainMenuWin();//MMIAPIPB_OpenListAllGroupWin();
                        break;
                    case IMAGE_IDLE_SOFTKEY_MENU:
#ifdef MMI_SAM_THEME                    
                    case IMAGE_IDLE_ICON_APP:
#endif                    
                        MMIAPIMENU_CreatMainMenu();                        
                        break;
                    case IMAGE_MAINMENU_ICON_PB:
                        MMIAPIPB_OpenListAllGroupWin();                        
                        break;

                        
                    #ifdef UPB_IDLE_SUPPORT
                    #ifdef MAINLCD_SIZE_320X480
                    case IMAGE_UPB_ICON_DIAL:
                        MMIAPIIDLE_EnterDialWin();
                        break;
                    case IMAGE_UPB_ICON_MESSAGE:
                        MMIAPISMS_OpenMessageMainMenuWin();//MMIAPIPB_OpenListAllGroupWin();
                        break;
                    case IMAGE_UPB_ICON_MAINMENU:
                        MMIAPIMENU_CreatMainMenu();                        
                        break;
                    case IMAGE_UPB_ICON_PB:
                        MMIAPIPB_OpenListAllGroupWin();                        
                        break;
                    #endif
                    case IMAGE_UPB_ICON_ENTER:
                        MMIIDLE_UPB_Start();                        
                        break;
                    #endif
                    default:
                        break;
                }
       	    }
            ResetSoftkeyState(idle_grid_ptr);
            DisplaySoftkey(idle_grid_ptr);
        }
        
        StopAutoSlideTimer();
        if(s_tp_down_type == 0)
        {
            HandleGridTpUp(idle_grid_ptr, param);
        }        
        s_tp_down_type = 0;        

#ifdef SCH_ASSISTANT_WIDGET//for NEWMS00179399
        if(is_send)
        {
            MMK_PostMsg(WIDGET_SCHEDULE_WIN_ID, MSG_SCH_WIDGET_UPDATE, PNULL, 0);
        }
#endif
        break;        

     case MSG_TP_PRESS_LONG:     
        HandleGridTpLongDown(idle_grid_ptr,param);
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }/*lint !e764*/
    return result;
}
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

/*****************************************************************************/
//  Description : widget子窗口的消息处理函数
//  Global resource dependence : 
//  Author:  hongbo.lan
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleChildWidgetWinMsg(
                                MMI_WIN_ID_T      win_id,
                                MMI_MESSAGE_ID_E  msg_id,
                                DPARAM            param
                                )
{
    MMI_RESULT_E	    	result = MMI_RESULT_TRUE;
	MMI_IDLE_GRID_DATA_T        *idle_grid_ptr = PNULL;
    uint32                  widget_id = MMIWIDGET_INVALID_ID;

    idle_grid_ptr = (MMI_IDLE_GRID_DATA_T *)MMIAPIGRID_GetGridPtr();
    if (PNULL == idle_grid_ptr)
    {
        return result;
    }

    widget_id = MMIWIDGET_GetWidgetIdByWinHandle(idle_grid_ptr, win_id);
    
	switch (msg_id)
    {
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_LONG:
    case MSG_TP_PRESS_DOWN:
     case MSG_TP_PRESS_MOVE:
    case MSG_TP_PRESS_UP:
        //MMIDLE_RestartSsTimer(idle_grid_ptr->win_id);
        HandleGridTPMsg(idle_grid_ptr, msg_id, param);
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        
    default:
    	result = MMIWIDGET_RunChildWidgetProcEx(idle_grid_ptr, widget_id, msg_id, param, FALSE);
        break;
    }/*lint !e764*/

	return(result);
}

/*****************************************************************************/
//  Description : get current tp status is moveing page or not.
//  Global resource dependence : 
//  Author:  hongbo.lan 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIGRID_IsForbidPaint(uint32 widget_id)
{
    BOOLEAN ret = FALSE;
    
    if(PNULL != s_idle_grid_data_ptr)
    {
		    ret = ((MMIGRID_TP_MOVE_NONE != s_idle_grid_data_ptr->widget_mov_type)
		           || s_idle_grid_data_ptr->is_editable
		           || MMIGRID_RUN_MODE_THUMBVIEW == s_idle_grid_data_ptr->run_mode
		           || (MMIGRID_RUN_MODE_MOVABLE == s_idle_grid_data_ptr->run_mode));    
    }
    
    return ret;
}

/*****************************************************************************/
//  Description : reset movable mode action.
//  Global resource dependence : 
//  Author:  hongbo.lan 
//  Note:
/*****************************************************************************/
LOCAL void ResetGridMovableAction(MMI_IDLE_GRID_DATA_T *idle_grid_ptr)
{
    MMIWIDGET_ITEM_T *item_ptr = PNULL;
    MMIWIDGET_DEBUG(LOG_STR("movable_tp_info click_type = %d"), idle_grid_ptr->movable_tp_info.click_type);
    if (MMIGRID_TP_CLICK_LONG_WIDGET == idle_grid_ptr->movable_tp_info.click_type)
    {
        MMIWIDGET_DEBUG(LOG_STR("old_grid is_valid = %d"), idle_grid_ptr->old_grid.is_valid);
        if (idle_grid_ptr->old_grid.is_valid)
        {
            MMIWIDGET_FillGridTbl(idle_grid_ptr,
                        &idle_grid_ptr->old_grid.filled_grid_cell,
                        idle_grid_ptr->old_grid.page,
                        idle_grid_ptr->movable_tp_info.click_id,
                        MMIGRID_ITEM_TYPE_WIDGET);           
        }
        else
        {
            MMIWIDGET_DEBUG(LOG_STR("movable_tp_info click_id = %d"), idle_grid_ptr->movable_tp_info.click_id);

            if(MMIWIDGET_DeactiveWidgetItem(idle_grid_ptr, idle_grid_ptr->movable_tp_info.click_id))
            {
                item_ptr = MMIWIDGET_FindWidgetItem(idle_grid_ptr, idle_grid_ptr->movable_tp_info.click_id);
                
                if (PNULL != item_ptr)
                {
                    MMIWIDGET_InsertWidget2AddingList(idle_grid_ptr, idle_grid_ptr->movable_tp_info.click_id, 0);  
                }
            }            
        }
    }
    else
    if (MMIGRID_TP_CLICK_LONG_SHORTCUT == idle_grid_ptr->movable_tp_info.click_type)
    {
        if (idle_grid_ptr->old_grid.is_valid)
        {
            MMIWIDGET_FillGridTbl(idle_grid_ptr,
                        &idle_grid_ptr->old_grid.filled_grid_cell,
                        idle_grid_ptr->old_grid.page,
                        idle_grid_ptr->movable_tp_info.click_id,
                        MMIGRID_ITEM_TYPE_ICON);
        }      
    }
    
    ClearOldGridArea(idle_grid_ptr);
    //begin added by liuxiaohua for NEWMS00127216 at 20111011
    idle_grid_ptr->tp_move_is_valid = FALSE;
    //end added by liuxiaohua for NEWMS00127216 at 20111011
    idle_grid_ptr->run_mode = MMIGRID_RUN_MODE_NORMAL;
    DestroyMoveLayer(idle_grid_ptr);
     //reset to main layer
    MMIWIDGET_ResetHitWidgetLayer(idle_grid_ptr);        
    ResetWidgetTpState(idle_grid_ptr);
    MMIGRID_SaveWidgetInfoToNV(idle_grid_ptr);
    //DrawAll(idle_grid_ptr);   
}

/*****************************************************************************/
//  Description : handle widget msg
//  Global resource dependence : 
//  Author:  hongbo.lan
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIAPIGRID_HandleMsg(
                                MMI_WIN_ID_T      win_id,
                                MMI_MESSAGE_ID_E  msg_id,
                                DPARAM            param
                                )
{
    MMI_RESULT_E	             result = MMI_RESULT_TRUE;
	MMI_IDLE_GRID_DATA_T        *idle_grid_ptr = PNULL;
    MMIWIDGET_ITEM_T            *item_ptr = PNULL;
    int32                        default_page_idx = MMIWIDGET_DEFAULT_PAGE_INDEX;   /*lint !e529 !e830 */

    idle_grid_ptr = MMIAPIGRID_GetGridPtr();
    if (PNULL == idle_grid_ptr)
    {
        if (MSG_GET_FOCUS == msg_id || MSG_OPEN_WINDOW == msg_id)
        {
            MMIAPIGRID_CreateIdle();
        }
        idle_grid_ptr = MMIAPIGRID_GetGridPtr();
        if (PNULL == idle_grid_ptr)
        {
            return result;
        }
    }


	switch (msg_id)
    {
        case MSG_APP_WEB:
        {
            MMI_RESULT_E res = MMI_RESULT_TRUE;
            res = HandleWebKeyMsg(idle_grid_ptr,msg_id);
            if(MMI_RESULT_FALSE == res)
            {
                MMIAPIMENU_CreatMainMenu();  
            }
        }
            break;

#ifndef BAR_PHONE_WIDGET
        case MSG_KEYREPEAT_UP:
        case MSG_APP_UP:
            HandleKeyUpMsg(idle_grid_ptr,msg_id);
            break;

        case MSG_KEYREPEAT_DOWN:
        case MSG_APP_DOWN:
            HandleKeyDownMsg(idle_grid_ptr,msg_id);
            break;
#endif

        case MSG_KEYREPEAT_LEFT:
        case MSG_APP_LEFT:
            HandleSlidePageMove(idle_grid_ptr,msg_id);
            break;

        case MSG_KEYREPEAT_RIGHT:
        case MSG_APP_RIGHT:
            HandleSlidePageMove(idle_grid_ptr,msg_id);
            break;

#ifdef BAR_PHONE_WIDGET 
        case MSG_KEYLONG_UP:
        case MSG_KEYLONG_DOWN: 
        case MSG_KEYPRESSUP_DOWN:
        case MSG_KEYPRESSUP_UP:
        case MSG_KEYUP_UP:
        case MSG_KEYUP_DOWN:
        case MSG_KEYREPEAT_UP:
        case MSG_APP_UP:
        case MSG_KEYREPEAT_DOWN:
        case MSG_APP_DOWN:
            {
                uint32 widget_id = 0;
                GetCurPageWidgetIdAndIndex(idle_grid_ptr,&widget_id,0);
                MMIWIDGET_RunChildWidgetProcEx(idle_grid_ptr,widget_id, msg_id,   param,FALSE);
            }
            break;
#endif

        case MSG_APP_OK:        
#ifdef BAR_PHONE_WIDGET 
            MMIAPIMENU_CreatMainMenu();  
#else
            if (MMIGRID_RUN_MODE_NORMAL == idle_grid_ptr->run_mode)
            {
                MMIWIDGET_CreateGridMenuWin(idle_grid_ptr);
            }
#endif
            break;
            
#if defined(KEYPAD_TYPE_FOUR_KEY_UNIVERSE)
        case MSG_APP_MENU:
   		    if (MMIGRID_RUN_MODE_NORMAL == idle_grid_ptr->run_mode)
			{
                MMIWIDGET_CreateGridMenuWin(idle_grid_ptr);
		    }
            break;

        case MSG_KEYLONG_RED:      
            if(MMIGRID_RUN_MODE_NORMAL == idle_grid_ptr->run_mode)
            {
                EnterThumbviewState( idle_grid_ptr);                
                break;
            }
        break;

        case MSG_KEYUP_RED:  
#endif
#ifndef BAR_PHONE_WIDGET 
        case MSG_APP_CANCEL:
            if (!MMK_IsFocusWin(idle_grid_ptr->win_id))
            {
                break;
            }           
            
//            if (MMIGRID_TP_MOVE_IDLE_TO_IDLE == idle_grid_ptr->widget_mov_type
//                || MMIGRID_TP_MOVE_PAGE == idle_grid_ptr->widget_mov_type)
//            {
//                return result;
//            }

            if (MMIGRID_RUN_MODE_THUMBVIEW == idle_grid_ptr->run_mode)
            {
                ExitThumbviewState(idle_grid_ptr);
                UILAYER_Clear(&idle_grid_ptr->page_layer);
                DrawAll(idle_grid_ptr);
                break;
            }
            
            if (MMIGRID_RUN_MODE_MOVABLE == idle_grid_ptr->run_mode)
            {
                if (GetIsEditable())
                {
                    SetIsEditable(FALSE);
#if defined BACKGROUND_GRID_LINE_LAYER
                    DestroyEditBgLayer(idle_grid_ptr);
#endif					
                }                
                DestroyMoveLayer(idle_grid_ptr);        
                DestroyRecycleLayer(idle_grid_ptr);
                ResetGridMovableAction(idle_grid_ptr);      
                MMIWIDGET_ReCalcAllWidgetWinRect(idle_grid_ptr);  
				DrawAll(idle_grid_ptr);
                break;
            }
			if (GetIsEditable())
			{
				SetIsEditable(FALSE);
#if defined BACKGROUND_GRID_LINE_LAYER
				DestroyEditBgLayer(idle_grid_ptr);
#endif
                MMIGRID_SaveWidgetInfoToNV(idle_grid_ptr);
				DrawAll(idle_grid_ptr);
				break;
			} 
            
#ifdef MMI_SAM_THEME
            if (MMIAPISET_GetIdleSubStyle() == MMISET_IDLE_SUBSTYLE_SAM)
            {    
                default_page_idx = MMIWIDGET_SAM_DEFAULT_PAGE_INDEX;
            }
#endif            
            if(default_page_idx !=  idle_grid_ptr->cur_page_index)
            {    
     
                if (0 != idle_grid_ptr->slide_anim.slide_flag)
                {
                    break;
                }
                SlidePage(idle_grid_ptr, default_page_idx); 
                MMIGRID_SetCurPageIndex(default_page_idx);

                SetGridPageLayerCacheInvalid(idle_grid_ptr, 0);
                SetGridPageLayerCacheInvalid(idle_grid_ptr, 1);
                SetGridPageLayerCacheInvalid(idle_grid_ptr, 2);
                MMIGRID_SaveWidgetInfoToNV(idle_grid_ptr);
                DrawAll(idle_grid_ptr); 
            }               
            break;
        
#endif
#ifdef BAR_PHONE_WIDGET 
        case MSG_APP_CANCEL:
    {
        MMISET_IDLE_STYLE_E     pre_idle_style = MMIAPISET_GetPreIdleStyle();
        MMK_SendMsg(win_id,MSG_LOSE_FOCUS,PNULL);
        if(pre_idle_style == MMISET_IDLE_STYLE_COMMON)
        {  
            MMISET_SetIdleStyle( MMISET_IDLE_STYLE_COMMON );
        }
        else
        {
            MMISET_SetIdleStyle( MMISET_IDLE_STYLE_SMART );
        }
        MMK_SendMsg(win_id,MSG_GET_FOCUS,PNULL);
        MMK_SendMsg(win_id,MSG_FULL_PAINT,PNULL);
    }
            break;
#endif           

    	case MSG_FULL_PAINT:
       	    if (idle_grid_ptr->tp_move_is_valid)
       	    {
       	        break;
       	    }

            if (   MMIGRID_RUN_MODE_THUMBVIEW == idle_grid_ptr->run_mode
                && idle_grid_ptr->is_thumb_showed)
            {
                break;
            }
            
            DrawAll(idle_grid_ptr);                
            if( idle_grid_ptr->previous_win_state == MSG_GET_FOCUS)
            {
                UILAYER_Clear(&idle_grid_ptr->softkey_layer);
                MMK_PostMsg( win_id,  MSG_GRID_PLAY_SOFTKEY_ENTER_ANIM, 0 , 0 );
            }
            idle_grid_ptr->previous_win_state = msg_id;
            break;


#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_TP_PRESS_LONG:
        case MSG_TP_PRESS_DOWN:       
        case MSG_TP_PRESS_MOVE:
        case MSG_TP_PRESS_UP:
            HandleGridTPMsg(idle_grid_ptr, msg_id, param);
            break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    
        case MSG_BACKLIGHT_TURN_OFF:
            if (MMIGRID_RUN_MODE_NORMAL == idle_grid_ptr->run_mode)
            {
                MMIGRID_SetBltLayer(idle_grid_ptr);
            }
            break;
    
        case MSG_LOSE_FOCUS:               
            //主层内容：shortcut，状态栏，softkey, 墙纸，widget应用  
            ResetWallpaperLayer(idle_grid_ptr);
            
            StopTpMoveTimer(idle_grid_ptr);
            StopAutoSlideTimer();
            if(idle_grid_ptr->previous_win_state == msg_id)
            {
                PlaySoftkeyLeaveAnim(idle_grid_ptr);
            }
            if( MMK_GetWinDisplayStyleState(MMK_GetFocusWinHandle(), WS_HAS_TRANSPARENT))
            {
                idle_grid_ptr->previous_win_state = 0;
            }
			if (GetIsEditable())
			{
				SetIsEditable(FALSE);
#if defined BACKGROUND_GRID_LINE_LAYER
				DestroyEditBgLayer(idle_grid_ptr);
#endif
			}  
            DestroyRecycleLayer(idle_grid_ptr);
            ExitThumbviewState(idle_grid_ptr);
            if (MMIGRID_TP_CLICK_LONG_WIDGET == idle_grid_ptr->tp_move_info.click_type)
            {
                item_ptr = MMIWIDGET_FindWidgetItem(idle_grid_ptr, idle_grid_ptr->tp_move_info.click_id);  
                if (PNULL != item_ptr)
                {
                    if(PNULL !=  item_ptr->item_info.ResetChildWinItem)
                    {                        
                        item_ptr->item_info.ResetChildWinItem();
                    }
                }
            }           

            ResetGridMovableAction(idle_grid_ptr);			
            ResetGridPageLayerPos(idle_grid_ptr);
            SetGridPageLayerCacheInvalid(idle_grid_ptr, 0);
            SetGridPageLayerCacheInvalid(idle_grid_ptr, 1);
            SetGridPageLayerCacheInvalid(idle_grid_ptr, 2);
            MMIWIDGET_ReCalcAllWidgetWinRect(idle_grid_ptr);

            idle_grid_ptr->current_gird = MMI_IDLE_GRID_PAGE_CELL_NUM;  
			idle_grid_ptr->widget_mov_type = MMIGRID_TP_MOVE_NONE;
            //protection: reset layer to avoid bug.

            MMIWIDGET_DispatchMsgToActiveChildWin(idle_grid_ptr, msg_id);
            //MMIWIDGET_DispatchMsgToChildInOnePage(idle_grid_ptr,
            //            idle_grid_ptr->cur_page_index,  msg_id ,param);    
			ResetSoftkeyState(idle_grid_ptr);
	
#ifdef PDA_UI_SUPPORT_U_IDLE
            MMIUIDLE_FreeUIdleAction(idle_grid_ptr); 
#endif
#ifdef MMI_IDLE_WIDGET_CACHE
            MMIWIDGET_ReleaseWidgetCacheInAllPage(idle_grid_ptr);            
#endif
            break;

        case MSG_GET_FOCUS:            
            MMK_SetWinSupportAngle(idle_grid_ptr->win_id, WIN_SUPPORT_ANGLE_INIT);

            MMIWIDGET_ResetHitWidgetLayer(idle_grid_ptr);  
            ResetWallpaperLayer(idle_grid_ptr);
               
            //MMIWIDGET_DispatchMsgToActiveChildWin(idle_grid_ptr, MSG_GET_FOCUS);
            MMIWIDGET_DispatchMsgToChildInOnePage(idle_grid_ptr,
                        idle_grid_ptr->cur_page_index,  msg_id ,param);    
#ifdef DYNAMIC_MAINMENU_SUPPORT
            if (MMIMENU_DyCheckIsSyn())
            {
                MMIMENU_DyGetAllItemData();
            }
#endif            
            if(MSG_LOSE_FOCUS == idle_grid_ptr->previous_win_state)
            {
                idle_grid_ptr->previous_win_state = msg_id;
            }

            if(MMIGRID_TP_CLICK_LONG_SHORTCUT == idle_grid_ptr->tp_move_info.click_type)
            {
                HandleIconTpLongDown(idle_grid_ptr, idle_grid_ptr->down_point);
                SaveTpMovePoint(idle_grid_ptr, idle_grid_ptr->down_point);
                CreateRecycleLayer(idle_grid_ptr);  
                DrawRecycle( idle_grid_ptr);                  
                idle_grid_ptr->previous_win_state = 0;
            } 
            else   if (MMIGRID_TP_CLICK_LONG_WIDGET == idle_grid_ptr->tp_move_info.click_type)
            {
                HandleWidgetTpLongDown(idle_grid_ptr);
                CreateRecycleLayer(idle_grid_ptr);  
                DrawRecycle( idle_grid_ptr);  
                idle_grid_ptr->previous_win_state = 0;
            }
            CreateTpMoveTimer(idle_grid_ptr, win_id);
            break;

        case MSG_OPEN_WINDOW: 
            {  
                GUI_BOTH_RECT_T both_rect = {0};
                uint16  wallpaper_layer_size = MMIWIDGET_GetWallpaperLayerSize(idle_grid_ptr);
                     
                both_rect  = MMITHEME_GetFullScreenBothRect();               
                
                MMK_SetWinSupportAngle(idle_grid_ptr->win_id, WIN_SUPPORT_ANGLE_INIT);
                
                both_rect.v_rect.right = (int16)((both_rect.v_rect.right - 
                            both_rect.v_rect.left + 1) * wallpaper_layer_size);
                both_rect.h_rect.right = (int16)((both_rect.v_rect.right - 
                            both_rect.v_rect.left + 1) * wallpaper_layer_size);

                idle_grid_ptr->run_mode = MMIGRID_RUN_MODE_NORMAL;    
                
                ResetWallpaperLayer(idle_grid_ptr);
                CreateTpMoveTimer(idle_grid_ptr, win_id);
            }
            break;
            
        case MSG_TIMER:   
            if(!MMK_IsFocusWin(win_id))
            {
                break;
            }
            if (idle_grid_ptr->slide_anim.timer_id == *( GUI_TIMER_ID_T*)param)
            {
             //    MMIWIDGET_DEBUG(LOG_STR("MSG_TIMER start time = %d"), SCI_GetTickCount());
#ifdef LIVE_WALLPAPER_FRAMEWORK_SUPPORT
            MMIAPILIVEWALLPAPER_SaveTimerId(idle_grid_ptr->slide_anim.timer_id);
#endif    
                 RestartTpMoveTimer(idle_grid_ptr, win_id);
                 SlideAnimCallback(idle_grid_ptr);
#ifdef TOUCH_PANEL_SUPPORT
                 {
                     MMI_TP_STATUS_E tp_status = MMI_TP_NONE;
                     MMI_MULTI_KEY_TP_MSG_PARAM_T   para = {0};
                     GUI_POINT_T point = {0};
                     MMK_GetLogicTPMsg(&tp_status, &point);
                     
                     para.cur_tp_point.x = point.x;
                     para.cur_tp_point.y = point.y;
                     if(MMI_TP_NONE != tp_status)
                     {
                         HandleGridTpMove(idle_grid_ptr, (DPARAM)&para);                         
                     }                   
                 }
#endif
            //     MMIWIDGET_DEBUG(LOG_STR("MSG_TIMER end time = %d"), SCI_GetTickCount());
            }
#ifdef TOUCH_PANEL_SUPPORT
            else  if (s_auto_slide_timer_id == *( GUI_TIMER_ID_T*)param)
            {
                //CreateAutoSlideTimer(idle_grid_ptr->win_id, MMIWIDGET_AUTO_SLIDE_TIMEOUT);
                
                if (s_is_auto_slide)
                {
                    int32 src_page_index = idle_grid_ptr->cur_page_index;
                    int32 dst_page_index = 0;
                    MMIWIDGET_PEN_MOVE_DIRECT_T move_direct = idle_grid_ptr->tp_move_info.pen_move_dir;                  
                    
                    if (move_direct == MMIWIDGET_PEN_MOVE_LEFT)   
                    {
                        //向左
                        dst_page_index = (src_page_index<(idle_grid_ptr->page_num-1)) ? 
                                    (src_page_index+1) : (idle_grid_ptr->page_num-1);
                    }
                    else if (move_direct == MMIWIDGET_PEN_MOVE_RIGHT)
                    {
                        //向右
                        dst_page_index = (src_page_index>0) ? (src_page_index-1) : 0;
                    }
                    if(dst_page_index == src_page_index)
                    {
                        break;
                    }                                       
                    PlaySlideAnim(idle_grid_ptr, move_direct, dst_page_index, 1); 
                }
            }
#endif
            break;

#if defined  GRID_THUMBVIEW_SUPPORT
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_TP_GESTURE:
            if (!MMK_IsFocusWin(idle_grid_ptr->win_id) || MMIGRID_TP_MOVE_NONE != idle_grid_ptr->widget_mov_type)
            {
                break;
            }
            
            if (MMIGRID_RUN_MODE_NORMAL == idle_grid_ptr->run_mode)
            {
                if (MMI_GESTURE_ZOOM_OUT == MMK_GET_TP_GESTURE(param))
                {
                    EnterThumbviewState(idle_grid_ptr);
                }                
            }
/*            
            else if(MMIGRID_RUN_MODE_THUMBVIEW == idle_grid_ptr->run_mode)
            {
                if (MMI_GESTURE_ZOOM_IN == MMK_GET_TP_GESTURE(param))
                {
                    ExitThumbviewState(idle_grid_ptr);
                }
            }
*/            
            break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#endif
         case MSG_GRID_PLAY_SOFTKEY_ENTER_ANIM:
            DrawPageTitleBar(idle_grid_ptr, idle_grid_ptr->cur_page_index);
            DisplaySoftkey(idle_grid_ptr);
            PlaySoftkeyEnterAnim(idle_grid_ptr);
            break;

         //case MSG_IDLE_UPDATE_NETWORK:       
         //    MMITHEME_SetUpdateDelayCount(1);
         case MSG_IDLE_UPDATE_MP3:
         case MSG_IDLE_UPDATE_DATETIME:
             //MMIWIDGET_DispatchMsgToActiveChildWin(idle_grid_ptr, msg_id);
             //这些消息会引起widget画屏动作，forbid paint的时候不发
             if(MMIGRID_IsForbidPaint(0))
             {
                 break;
             }
             DrawSpecificPage(idle_grid_ptr, idle_grid_ptr->cur_page_index);
             //MMIWIDGET_DispatchMsgToChildInOnePage(idle_grid_ptr,
            //     idle_grid_ptr->cur_page_index,  msg_id ,param);    
             break;

         case MSG_WIDGET_MP3_SWITCH:
              MMIWIDGET_DispatchMsgToChildInOnePage(idle_grid_ptr,
                 idle_grid_ptr->cur_page_index,  msg_id ,param);    
         break;
         
         default:
             result = MMI_RESULT_FALSE;
             break;
    }

	return(result);     
}

/*****************************************************************************/
//  Description : dispatch msg to all child win
//  Global resource dependence : 
//  Author:  hongbo.lan
//  Note:
/*****************************************************************************/
/*****************************************************************************/
//  Description : destroy the widget control.
//  Global resource dependence : 
//  Author:  hongbo.lan
//  Note:
/*****************************************************************************/
PUBLIC void MMIGRID_Destroy(
                             MMIWIDGET_CTRL_E  ctrl_id
                             )
{    
    //目前只支持idle界面widget
    if (MMIWIDGET_CTRL_IDLE_ID != ctrl_id)
    {
        return;
    }
    
    if (PNULL != s_idle_grid_data_ptr)
    {        
        MMK_SetWinDisplayStyleState(MAIN_IDLE_WIN_ID, WS_DISABLE_REDRAW_CHILDWIN, FALSE);       
        DestroyWallpaperLayer(s_idle_grid_data_ptr);
        DestroySoftkeyLayer(s_idle_grid_data_ptr);
        DestroyWidgetPageLayer(s_idle_grid_data_ptr);
#if defined BACKGROUND_GRID_LINE_LAYER
        DestroyEditBgLayer(s_idle_grid_data_ptr);
#endif		
        DestroyTitlebarLayer(s_idle_grid_data_ptr);        
        DestroyRecycleLayer(s_idle_grid_data_ptr);

#ifdef MMI_IDLE_WIDGET_CACHE
        //clean widget cache
        MMIWIDGET_ReleaseWidgetCacheInAllPage(s_idle_grid_data_ptr);
        MMIWIDGET_ClearWidgetCacheControlData(s_idle_grid_data_ptr);
#endif

        MMIWIDGET_DestroyAllWidgetItemData(s_idle_grid_data_ptr);
#ifdef DYNAMIC_MAINMENU_SUPPORT
        MMIMENU_DyFreeData();
#endif
        SCI_FREE(s_idle_grid_data_ptr->item_ptr);
        SCI_FREE(s_idle_grid_data_ptr->icon_ptr);        
        SCI_FREE(s_idle_grid_data_ptr);
        s_idle_grid_data_ptr = PNULL;
    }
}

LOCAL uint16 GetWidgetMaxGridNum(MMI_IDLE_GRID_DATA_T *idle_grid_ptr)
{
    if(!MMK_IsWindowLandscape( idle_grid_ptr->win_id))   
    {   
        return MMI_IDLE_SC_ITEM_MAX;
    }
    else
    {
        return MMI_IDLE_SC_ITEM_MAX_LANDSCAPE;
    }
}

LOCAL uint16 GetPageGridNum(MMI_IDLE_GRID_DATA_T *idle_grid_ptr)
{
    if(!MMK_IsWindowLandscape( idle_grid_ptr->win_id))   
    {   
        return MMI_IDLE_GRID_PAGE_CELL_NUM;
    }
    else
    {
        return MMI_IDLE_GRID_PAGE_CELL_NUM_LANDSCAPE;
    }
}

LOCAL void GetPageGridRowCol(int16 *prow, int16 *pcol, MMI_IDLE_GRID_DATA_T *idle_grid_ptr)
{
    int16 col = 0;
    int16 row = 0;
    if(!MMK_IsWindowLandscape( idle_grid_ptr->win_id))   
    {   
       col = MMI_IDLE_GRID_COL_NUM;
       row = MMI_IDLE_GRID_ROW_NUM;    
    }
    else
    {
        col = MMI_IDLE_GRID_COL_NUM_LANDSCAPE;
        row = MMI_IDLE_GRID_ROW_NUM_LANDSCAPE;
    }
    *prow = row;
    *pcol = col;
}
/*****************************************************************************/
//  Description : initial shortcut idle grid rect information.
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note:
/*****************************************************************************/
LOCAL void InitScGridRect(MMI_IDLE_GRID_DATA_T *idle_grid_ptr)
{
    uint16 j = 0;
    uint16 k = 0;
    int16 col = 0;
    int16 row = 0; 
    uint16 width = 0;   /*lint !e529 !e830 */
    uint16 height = 0;  /*lint !e529 !e830 */
    GUI_RECT_T  page_layer_rect = GetWidgetPageRect(idle_grid_ptr);

    GetPageGridRowCol(&row, &col, idle_grid_ptr);
    
    width = page_layer_rect.right - page_layer_rect.left + 1;
    height = page_layer_rect.bottom - page_layer_rect.top + 1;
    idle_grid_ptr->display_rect = page_layer_rect;
    idle_grid_ptr->grid_width = width/col;
    idle_grid_ptr->grid_height = height/row;
    idle_grid_ptr->grid_row = row;
    idle_grid_ptr->grid_col = col;
    
    for(j = 0;j < row;j++ )
    {
        for(k = 0; k < col;k++)
        {
            idle_grid_ptr->page_grid_rect[j*col + k].top    = page_layer_rect.top + j*idle_grid_ptr->grid_height;
            idle_grid_ptr->page_grid_rect[j*col + k].bottom = page_layer_rect.top + j*idle_grid_ptr->grid_height + idle_grid_ptr->grid_height - 1;
            idle_grid_ptr->page_grid_rect[j*col + k].left   = k*idle_grid_ptr->grid_width;
            idle_grid_ptr->page_grid_rect[j*col + k].right  = k*idle_grid_ptr->grid_width + idle_grid_ptr->grid_width - 1;
        }
    }
}

/*****************************************************************************/
//  Description : create the widget control.
//  Global resource dependence : 
//  Author:  hongbo.lan
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIGRID_Create(
                             MMI_WIN_ID_T win_id,
                             MMIWIDGET_CTRL_E  ctrl_id,
                             GUI_BOTH_RECT_T *both_rect_ptr
                             )
{
    BOOLEAN result = FALSE;

    if (MMIWIDGET_CTRL_IDLE_ID != ctrl_id)
    {
        return FALSE;
    }
    if (PNULL == s_idle_grid_data_ptr)
    {
        s_idle_grid_data_ptr = (MMI_IDLE_GRID_DATA_T*)SCI_ALLOC_APP(sizeof(MMI_IDLE_GRID_DATA_T));

        SCI_MEMSET(s_idle_grid_data_ptr, 0x00, sizeof(MMI_IDLE_GRID_DATA_T));
        MMK_SetWinDisplayStyleState(win_id, WS_DISABLE_REDRAW_CHILDWIN, TRUE);
        s_idle_grid_data_ptr->win_id = win_id;
        s_idle_grid_data_ptr->ctrl_id = ctrl_id;
        s_idle_grid_data_ptr->cur_idle_type = MMI_IDLE_GRID;
        
        if (PNULL != both_rect_ptr)
        {
            s_idle_grid_data_ptr->both_rect = *both_rect_ptr;
        }
        s_idle_grid_data_ptr->item_ptr = PNULL;
        s_idle_grid_data_ptr->move_layer.lcd_id = GUI_MAIN_LCD_ID;
        s_idle_grid_data_ptr->move_layer.block_id = UILAYER_NULL_HANDLE;
        s_idle_grid_data_ptr->thumb_view_layer.lcd_id   = GUI_MAIN_LCD_ID;
        s_idle_grid_data_ptr->thumb_view_layer.block_id = UILAYER_NULL_HANDLE;        
        s_idle_grid_data_ptr->recycle_layer.lcd_id = GUI_MAIN_LCD_ID;
        s_idle_grid_data_ptr->recycle_layer.block_id = UILAYER_NULL_HANDLE;        
        s_idle_grid_data_ptr->is_main_screen_landscape = MMK_IsWindowLandscape(win_id);  
        s_idle_grid_data_ptr->tp_move_is_valid = FALSE;
	  s_idle_grid_data_ptr->delect_icon_id = IMAGE_ANDROIDMENU_DELETE_ICON;

#ifdef PDA_UI_SUPPORT_U_IDLE
        s_idle_grid_data_ptr->is_Uidle_1st_mov = TRUE;         
#endif        
        
        MMITHEME_GetWidgetTheme(&s_idle_grid_data_ptr->display_theme);
        InitScGridRect(s_idle_grid_data_ptr);
        InitSoftkey(s_idle_grid_data_ptr);  

        CreatePageLayer(s_idle_grid_data_ptr);
        CreateWallpaperLayer(s_idle_grid_data_ptr);
        CreateSoftkeyLayer(s_idle_grid_data_ptr);
        CreateTitlebarLayer( s_idle_grid_data_ptr);
        //DestroyRecycleLayer( s_idle_grid_data_ptr);
        
#ifdef MMI_IDLE_WIDGET_CACHE        
        MMIWIDGET_InitWidgetCacheControlData(s_idle_grid_data_ptr);
#endif
        
#ifdef DYNAMIC_MAINMENU_SUPPORT
        MMIMENU_DyGetAllItemData();
#endif

        //read all main menus.
        MMIMAINMENU_ReadAllMenuItemInfo(s_idle_grid_data_ptr->grid_menu_info.grid_menu, &s_idle_grid_data_ptr->grid_menu_info.menu_num);
        result = TRUE;
    }
    else
    {
        result = FALSE;
    }
    return result;
}

/*****************************************************************************/
//  Description : reset the filled grid cell data.
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note:
/*****************************************************************************/
LOCAL void ClearOldGridArea(MMI_IDLE_GRID_DATA_T *idle_grid_ptr)
{
    if(PNULL == idle_grid_ptr)
    {
        return;
    }
    idle_grid_ptr->old_grid.is_valid = FALSE;
    idle_grid_ptr->old_grid.page = 0;
    idle_grid_ptr->old_grid.filled_grid_cell.valid_cell_array[0]= GRID_END_FLAG;
}

/*****************************************************************************/
//  Description :  return widget control ptr
//  Global resource dependence : 
//  Author:  hongbo.lan
//  Note:
/*****************************************************************************/
PUBLIC MMI_IDLE_GRID_DATA_T *MMIAPIGRID_GetGridPtr(void)
{
    return s_idle_grid_data_ptr;
}


/*****************************************************************************/
//  Description :  set max item num, must be called before append item
//  Global resource dependence : 
//  Author:  hongbo.lan
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIGRID_SetMaxItemNum(
                                    MMIWIDGET_CTRL_E ctrl_id, // control id
                                    uint32        max_item_num
                                    )
{
    MMI_IDLE_GRID_DATA_T *idle_grid_ptr = (MMI_IDLE_GRID_DATA_T *)MMIAPIGRID_GetGridPtr();
    uint32 all_page_grid_num = GetWidgetMaxGridNum(idle_grid_ptr);

    if (PNULL == idle_grid_ptr)
    {
        return FALSE;
    }
    
    if (max_item_num > MMIWIDGET_MAX_ITEM_NUM)
    {
        max_item_num = MMIWIDGET_MAX_ITEM_NUM;
    }

    if (0 == max_item_num)
    {
        return FALSE;
    }

    if (PNULL != idle_grid_ptr->item_ptr)
    {
        SCI_FREE(idle_grid_ptr->item_ptr);
    }
    
    idle_grid_ptr->item_ptr = (MMIWIDGET_ITEM_T*)SCI_ALLOC_APP(max_item_num*sizeof(MMIWIDGET_ITEM_T));
    SCI_MEMSET(idle_grid_ptr->item_ptr, 0x00, max_item_num*sizeof(MMIWIDGET_ITEM_T));
    idle_grid_ptr->total_item_num = 0;
    idle_grid_ptr->max_item_num = max_item_num;

    idle_grid_ptr->icon_ptr = (MMI_IDLE_SC_ICON_T*)SCI_ALLOC_APP( all_page_grid_num * sizeof(MMI_IDLE_SC_ICON_T));
    SCI_MEMSET(idle_grid_ptr->icon_ptr, 0x00, all_page_grid_num * sizeof(MMI_IDLE_SC_ICON_T));
    idle_grid_ptr->total_icon_num = 0;
    
	return TRUE;     
}

/*****************************************************************************/
//  Description : reset widget tp state.
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note: 
/*****************************************************************************/
LOCAL void ResetWidgetTpState(MMI_IDLE_GRID_DATA_T *idle_grid_ptr)
{
    idle_grid_ptr->tp_move_is_valid          = FALSE;
    idle_grid_ptr->down_point.x              = MMIWIDGET_INT16_MIN_VALUE;
    idle_grid_ptr->tp_move_info.click_id     = MMIWIDGET_INVALID_ID;
    idle_grid_ptr->tp_move_info.click_type   = MMIGRID_TP_CLICK_INVALID;
    idle_grid_ptr->tp_move_info.pen_move_dir = MMIWIDGET_PEN_MOVE_INVALID;
    idle_grid_ptr->widget_mov_type           = MMIGRID_TP_MOVE_NONE;
    if(MMIGRID_RUN_MODE_MOVABLE != idle_grid_ptr->run_mode)
    {
        idle_grid_ptr->movable_tp_info.click_id     = MMIWIDGET_INVALID_ID;
        idle_grid_ptr->movable_tp_info.click_type   = MMIGRID_TP_CLICK_INVALID;
        idle_grid_ptr->movable_tp_info.pen_move_dir = MMIWIDGET_PEN_MOVE_INVALID;
    }
    idle_grid_ptr->showed_rect_info.is_valid = FALSE;
    idle_grid_ptr->is_child_win_btn          = FALSE;
    idle_grid_ptr->is_child_win_tp_move      = FALSE;    
}

/*****************************************************************************/
//  Description : try to reset moving child win postion.
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN ResetChildWinPosition(MMI_IDLE_GRID_DATA_T *idle_grid_ptr, GUI_POINT_T point)
{
    BOOLEAN ret = FALSE;
    GUI_RECT_T win_rect = {0};
    MMIWIDGET_ITEM_T *item_ptr = PNULL;
    GUI_RECT_T old_win_rect = {0}; 
    MMI_HANDLE_T win_handle = 0;
    uint16  bg_width  = 0;
    uint16  bg_height = 0;

    MMI_GRID_CELL_T filled_cell = {0};
    GUI_RECT_T new_rc = {0};
    uint16 lcd_width  = 0;
    uint16 lcd_height = 0;
    GUI_RECT_T dst_rect = {0};

    //if widget item is on recycle bin, remove it and add to shortcut list.
    if(idle_grid_ptr->is_item_upon_recycle)
    {
        if(MMIWIDGET_DeactiveWidgetItem(idle_grid_ptr, idle_grid_ptr->tp_move_info.click_id))
        {
            item_ptr = MMIWIDGET_FindWidgetItem(idle_grid_ptr, idle_grid_ptr->tp_move_info.click_id);
            
            if (PNULL != item_ptr)
            {
                ret = MMIWIDGET_InsertWidget2AddingList(idle_grid_ptr, idle_grid_ptr->tp_move_info.click_id, 0);  

            }
            return TRUE;
        }
    }



    win_handle = MMIWIDGET_GetWinHandleByWidgetId(idle_grid_ptr, idle_grid_ptr->tp_move_info.click_id);
    MMK_GetWinRect(win_handle, &old_win_rect);
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &lcd_width, &lcd_height);
    bg_width = old_win_rect.right - old_win_rect.left + 1;
    bg_height = old_win_rect.bottom - old_win_rect.top + 1;
    

    if (idle_grid_ptr->showed_rect_info.is_valid)
    {
        dst_rect = idle_grid_ptr->showed_rect_info.old_rect;
    }
    else
    {
        return ret;
    }

    if(MMIWIDGET_LayoutGridTblByRect(idle_grid_ptr,idle_grid_ptr->cur_page_index,dst_rect,&filled_cell,&new_rc))
    {
        bg_width = old_win_rect.right - old_win_rect.left + 1;
        bg_height = old_win_rect.bottom - old_win_rect.top + 1;

        win_rect.left = (new_rc.right + new_rc.left - bg_width)/2 ;
        win_rect.top = (new_rc.top + new_rc.bottom - bg_height)/2 ;
        win_rect.bottom = win_rect.top + bg_height - 1;
        win_rect.right = win_rect.left + bg_width - 1;
        MMIWIDGET_SetChildWinRect(win_handle, win_rect, idle_grid_ptr->is_main_screen_landscape);
        
        MMIWIDGET_FillGridTbl(idle_grid_ptr,&filled_cell,idle_grid_ptr->cur_page_index,idle_grid_ptr->tp_move_info.click_id, MMIGRID_ITEM_TYPE_WIDGET);
        ret = TRUE;
    }

    return ret;
}

/*****************************************************************************/
//  Description : layout the shortcut item.
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN LayoutShortCutItem(MMI_IDLE_GRID_DATA_T *idle_grid_ptr)
{
    BOOLEAN  ret = FALSE;
    GUI_RECT_T rc = {0};
    MMI_GRID_CELL_T valid_grid = {0};
    GUI_RECT_T dst_rc = {0};
    uint16 index = 0;
    MMI_IDLE_SC_ICON_T shortcut_item = {0};

    if(PNULL == idle_grid_ptr)
    {
        return FALSE;
    }
    
    shortcut_item = idle_grid_ptr->icon_ptr[idle_grid_ptr->tp_move_info.click_id];

    if (idle_grid_ptr->showed_rect_info.is_valid)    
    {
        rc = idle_grid_ptr->showed_rect_info.old_rect;
    }
    else
    {
        return ret;
    }

    if (MMIWIDGET_LayoutGridTblByRect(idle_grid_ptr, idle_grid_ptr->cur_page_index, rc, &valid_grid, &dst_rc))
    {
        index = MMIWIDGET_GetShortCutIdx(idle_grid_ptr,shortcut_item.icon_info);
        MMIWIDGET_FillGridTbl(idle_grid_ptr,&valid_grid,idle_grid_ptr->cur_page_index,index,MMIGRID_ITEM_TYPE_ICON); 
        ret = TRUE;
    }
    else
    {
        ret = FALSE;
    }

    return ret;
}

/*****************************************************************************/
//  Description : handle page grid thumbview tp up.
//  Global resource dependence : 
//  Author:hongbo.lan
//  Note: 
/*****************************************************************************/
LOCAL void HandlePageThumbViewTpUp(MMI_IDLE_GRID_DATA_T *idle_grid_ptr, GUI_POINT_T point)
{
    int32 i = 0;
    int32 idx = 0;
    int32 start_idx = 0;
    uint16 lcd_width  = 0;
    uint16 lcd_height = 0; 
    int32 click_thumb = -1;
    int16 offset_x = 0;
    int16 offset_y = 0;
    uint16 rect_width = 0;
    uint16 rect_height = 0;
    GUI_RECT_T      fullscreen_rect = MMITHEME_GetFullScreenRect();
    GUI_RECT_T      thumb_rect = {0};
    GUI_LCD_DEV_INFO    thumb_layer = {0};    
    GUI_LCD_DEV_INFO    main_layer = {0};    
    BOOLEAN         result = TRUE;
    int32       page_num = (int32)ARR_SIZE(s_thumb_rect_grp);    
    DISPLAY_DATA_TYPE_E data_type = DATA_TYPE_RGB565;
    uint16*   lcd_buf_ptr = GUILCD_GetMainLcdBufPtr();
    uint16*   dst_buf_ptr = 0;
    uint16*   src_buf_ptr = 0;
    int16     delta_width = 0;
    int16     delta_height = 0;
    SCALE_IMAGE_OUT_T   scale_out = {0};
    UILAYER_COPYBUF_PARAM_T copy_param = {0};
    UILAYER_COPYBUF_T copybuf = {0};


    if (MMIGRID_RUN_MODE_THUMBVIEW != idle_grid_ptr->run_mode)
    {
        return;
    }

    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height); 

    for (i = 0; i < page_num; i++)
    {
        if (GUI_PointIsInRect(point, s_thumb_rect_grp[i]))
        {
            click_thumb = i;
            break;
        }
    }
    if(click_thumb < 0 || click_thumb >= page_num)
    {
        return;
    }

    idle_grid_ptr->cur_page_index = click_thumb;
    UILAYER_Clear(&idle_grid_ptr->page_layer);
    ExitThumbviewState(idle_grid_ptr); 
    
    idle_grid_ptr->thumb_view_layer = main_layer;
    DrawSpecificPageEx(idle_grid_ptr, idle_grid_ptr->thumb_view_layer, click_thumb,  0, 0 );
    
    rect_width = (s_thumb_rect_grp[click_thumb].right - s_thumb_rect_grp[click_thumb].left + 1);
    rect_height = (s_thumb_rect_grp[click_thumb].bottom - s_thumb_rect_grp[click_thumb].top + 1);
    delta_width = lcd_width / 10;
    delta_height = lcd_height/ 10;
    start_idx = rect_width / delta_width;
    if(rect_width > idx* delta_width)
    {
        start_idx++;
    }
    offset_x = s_thumb_rect_grp[click_thumb].left / (10 - start_idx);
    offset_y = s_thumb_rect_grp[click_thumb].top / (10 - start_idx);
    //zoom animation.
    for (idx = start_idx; idx < 10; idx++)   //最后一帧有内存越界的风险 i=7， offset_y = 4,但是rect_height = lcd_width
                                    //再去往lcd buffer上拷贝，就超过了lcd buffer的限制
    {        
        //UILAYER_SetMainLayerImmutable(TRUE);    
        //UILAYER_SetLayerMemFormat(&idle_grid_ptr->page_layer, UILAYER_MEM_FPS_ACCELERATE);   
        //UILAYER_SetDirectDraw(TRUE);        
   
        rect_width   =  delta_width*idx ;
        rect_height  =  delta_height*idx;
        thumb_rect.left =  s_thumb_rect_grp[click_thumb].left - offset_x *(idx - start_idx + 1);
        thumb_rect.top = s_thumb_rect_grp[click_thumb].top - offset_y *(idx - start_idx + 1);
        thumb_rect.right = thumb_rect.left + rect_width - 1;            
        thumb_rect.bottom = thumb_rect.top + rect_height - 1;
        
        MMIWIDGET_DEBUG(LOG_STR("i = %d, time = %d"), idx, SCI_GetTickCount());
        
        thumb_layer = MMIWIDGET_CreateLayer(idle_grid_ptr->win_id, &thumb_rect, data_type);
        MMIWIDGET_DEBUG(LOG_STR("CreatePageThumbLayer finished, time = %d"), SCI_GetTickCount());
        
        if(FALSE == ThumbLayer(&idle_grid_ptr->thumb_view_layer, &fullscreen_rect, 
                            &thumb_layer, &thumb_rect, data_type, &scale_out))
        {
            MMIWIDGET_DEBUG(LOG_STR("ThumbLayer FALSE"));
            result =  FALSE;
            goto HandlePageThumbViewTpUp_ERROR_HANDLEMENT;
        }

        MMIWIDGET_DEBUG(LOG_STR("ThumbLayer finished, time = %d"), SCI_GetTickCount());       
        
        //UILAYER_BltLayerToLayer(&main_layer, &thumb_layer, &thumb_rect, FALSE);
        src_buf_ptr = (uint16*)UILAYER_GetLayerBufferPtr(&thumb_layer);
        
       //not copy stb display buffer
    
        dst_buf_ptr = lcd_buf_ptr + thumb_rect.left + 
                thumb_rect.top*(fullscreen_rect.right - fullscreen_rect.left + 1);

        
        for(i = 0; i <scale_out.output_size.h; i++)
        {
            SCI_MEMCPY(dst_buf_ptr, src_buf_ptr, scale_out.output_size.w*sizeof(uint16));
            dst_buf_ptr += (fullscreen_rect.right - fullscreen_rect.left + 1);
            src_buf_ptr += scale_out.output_size.w;
        }
        MMIWIDGET_DEBUG(LOG_STR("copy scale out buffer finished, time = %d"), SCI_GetTickCount());       
        GUILCD_InvalidateRectOnly(GUI_MAIN_LCD_ID,fullscreen_rect,0);            
        
        MMIWIDGET_DEBUG(LOG_STR("GUILCD_InvalidateRectOnly finished, time = %d"), SCI_GetTickCount());

        
#ifdef WIN32        
        SCI_SLEEP(30);
#endif                
        UILAYER_RELEASELAYER(&thumb_layer);      /*lint !e506 !e774*/
    }  
   
   copy_param.clip_rect_ptr = &fullscreen_rect;
   copy_param.rect = fullscreen_rect;
   copybuf.buf_ptr = (uint8*)lcd_buf_ptr;
   copybuf.data_type = DATA_TYPE_RGB565;
   copybuf.height  =   lcd_height;
   copybuf.width = lcd_width;
   UILAYER_CopyLayerToBuffer(&idle_grid_ptr->thumb_view_layer, &copy_param, &copybuf);
   GUILCD_InvalidateRectOnly(GUI_MAIN_LCD_ID,fullscreen_rect,0);  
   
   idle_grid_ptr->previous_win_state = MSG_GET_FOCUS;
   MMK_SendMsg(idle_grid_ptr->win_id, MSG_FULL_PAINT , 0 );

HandlePageThumbViewTpUp_ERROR_HANDLEMENT:
    UILAYER_RELEASELAYER(&thumb_layer);      /*lint !e506 !e774*/        
    //DestroyGridThumbViewLayer(idle_grid_ptr);    
}

/*****************************************************************************/
//  Description : handle grid item on movable mode tp up action.
//  Global resource dependence : 
//  Author:hongbo.lan
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN HandleMoveModeTpUp(
                               MMI_IDLE_GRID_DATA_T *idle_grid_ptr,
                               DPARAM            param
                               )
{
    MMIWIDGET_ITEM_T *item_ptr = PNULL;
    GUI_POINT_T point = {0};
    int16 offsetX = 0;
    int16 offsetY = 0;

    point.x = MMK_GET_TP_X(param);
    point.y = MMK_GET_TP_Y(param);

    if (MMIGRID_TP_MOVE_PAGE == idle_grid_ptr->widget_mov_type)
    {        
        if (MMIGRID_TP_CLICK_WIDGET == idle_grid_ptr->tp_move_info.click_type)
        {
            item_ptr = MMIWIDGET_FindWidgetItem(idle_grid_ptr, idle_grid_ptr->tp_move_info.click_id);

            if (PNULL != item_ptr && PNULL != item_ptr->item_info.ResetChildWinItem)
            {
               item_ptr->item_info.ResetChildWinItem();
            }
        }
        HandleSlidePageTpUp(idle_grid_ptr, point);
        idle_grid_ptr->tp_move_info = idle_grid_ptr->movable_tp_info;   
        if(MMIGRID_TP_CLICK_LONG_WIDGET == idle_grid_ptr->tp_move_info.click_type
            || MMIGRID_TP_CLICK_LONG_SHORTCUT == idle_grid_ptr->tp_move_info.click_type)
        {
            idle_grid_ptr->widget_mov_type |= MMIGRID_TP_MOVE_IDLE_TO_IDLE; /*lint !e655 */
        }

        return TRUE;
    }
    else if(MMIGRID_TP_CLICK_LONG_SHORTCUT == idle_grid_ptr->tp_move_info.click_type)
    {
        if (!idle_grid_ptr->is_item_upon_recycle)
        {        
            if (!LayoutShortCutItem(idle_grid_ptr))
            {
                idle_grid_ptr->run_mode = MMIGRID_RUN_MODE_MOVABLE;                   
                idle_grid_ptr->movable_tp_info = idle_grid_ptr->tp_move_info;
                return TRUE;
            }
        }
        else
        {
            MMIWIDGET_RemoveShortcutInAddingList(idle_grid_ptr,idle_grid_ptr->tp_move_info.click_id);
        }
        PlayRecycleLeaveAnim(idle_grid_ptr);
        idle_grid_ptr->run_mode = MMIGRID_RUN_MODE_NORMAL;
        ClearOldGridArea(idle_grid_ptr);
        DestroyMoveLayer(idle_grid_ptr);        
        DestroyRecycleLayer(idle_grid_ptr);
        s_is_auto_slide = FALSE;
        idle_grid_ptr->is_item_upon_recycle = FALSE;
        idle_grid_ptr->widget_mov_type = MMIGRID_TP_MOVE_NONE;
        //reset to main layer
        MMIWIDGET_ResetHitWidgetLayer(idle_grid_ptr);        
        ResetWidgetTpState(idle_grid_ptr);
        MMIGRID_SaveWidgetInfoToNV(idle_grid_ptr);              
        DrawAll(idle_grid_ptr);
        return TRUE;
    }
    else  if (MMIGRID_TP_CLICK_LONG_WIDGET == idle_grid_ptr->tp_move_info.click_type)
    {   
        point.x = offsetX;
        point.y = offsetY;
        
        if (!ResetChildWinPosition(idle_grid_ptr, point))
        {
            idle_grid_ptr->run_mode = MMIGRID_RUN_MODE_MOVABLE;            
            idle_grid_ptr->movable_tp_info = idle_grid_ptr->tp_move_info;
            return TRUE;
        }

        if (MMIGRID_TP_MOVE_IDLE_TO_IDLE == idle_grid_ptr->widget_mov_type)
        { 
            MMIWIDGET_RunChildWidgetProc(idle_grid_ptr, idle_grid_ptr->tp_move_info.click_id,
                                                MSG_WIDGET_TP_PRESS_LONG_UP, 0); 
        }
        PlayRecycleLeaveAnim(idle_grid_ptr);
        ClearOldGridArea(idle_grid_ptr);
        idle_grid_ptr->run_mode = MMIGRID_RUN_MODE_NORMAL;
        DestroyMoveLayer(idle_grid_ptr);
        DestroyRecycleLayer(idle_grid_ptr);
        idle_grid_ptr->is_item_upon_recycle = FALSE;
        s_is_auto_slide = FALSE;
        idle_grid_ptr->widget_mov_type = MMIGRID_TP_MOVE_NONE;
        //reset to main layer
        MMIWIDGET_ResetHitWidgetLayer(idle_grid_ptr);        
        ResetWidgetTpState(idle_grid_ptr);
        MMIGRID_SaveWidgetInfoToNV(idle_grid_ptr);            
        DrawAll(idle_grid_ptr);        
    } 

    return TRUE;
}                               

/*****************************************************************************/
//  Description : handle widget tp up
//  Global resource dependence : 
//  Author:hongbo.lan
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN HandleGridTpUp(
                               MMI_IDLE_GRID_DATA_T *idle_grid_ptr,
                               DPARAM            param
                               )
{
    MMIWIDGET_ITEM_T *item_ptr = PNULL;
    GUI_POINT_T point = {0};
    int16 offsetX = 0;
    int16 offsetY = 0;
	uint16 grid_index;
	
    point.x = MMK_GET_TP_X(param);
    point.y = MMK_GET_TP_Y(param);

    idle_grid_ptr->tp_move_is_valid          = FALSE;
    
    if (MMIGRID_RUN_MODE_THUMBVIEW == idle_grid_ptr->run_mode) 
    {   
        if (MMIGRID_TP_MOVE_THUMBVIEW != idle_grid_ptr->widget_mov_type
            && MMIGRID_TP_CLICK_THUMBVIEW == idle_grid_ptr->tp_move_info.click_type)
        {
            HandlePageThumbViewTpUp(idle_grid_ptr, point);
        }        
        ResetWidgetTpState(idle_grid_ptr);
        return TRUE;
    }

    CheckGridItemRecycleStat(idle_grid_ptr);
    if (MMIGRID_RUN_MODE_MOVABLE == idle_grid_ptr->run_mode)
    {
        HandleMoveModeTpUp(idle_grid_ptr, param);
        return TRUE;            
    }
#ifdef MMI_IDLE_THUMBVIEW_SLIDE
    if (MMIGRID_RUN_MODE_THUMBVIEW_SLIDE == idle_grid_ptr->run_mode)
    {
        uint8 cur_page=0;
        uint16 lcd_width=0;
        uint16 lcd_height=0;
        int16 wp_offset_x=0;
        GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height); 
        idle_grid_ptr->run_mode = MMIGRID_RUN_MODE_NORMAL;
        MMIWIDGET_ResetHitWidgetLayer(idle_grid_ptr); 
        ResetWidgetTpState(idle_grid_ptr);
        DestroyGridThumbViewSlideLayer(idle_grid_ptr);
        MMIGRID_SetBltLayer(idle_grid_ptr);
        cur_page=GetCurThumbviewPage(point);
        if(idle_grid_ptr->cur_page_index!=cur_page)
        {
            idle_grid_ptr->cur_page_index=cur_page;
            SetPageLayerPosition( idle_grid_ptr, &idle_grid_ptr->page_layer, 0, 0);
        }
        wp_offset_x = -idle_grid_ptr->cur_page_index * GetWidgetPageWidth(idle_grid_ptr);          
        SetWallpaperPosition(idle_grid_ptr, wp_offset_x, 0, FALSE);
        DrawAll(idle_grid_ptr);               
        return TRUE;
    }
#endif

	if(idle_grid_ptr->is_child_win_btn)
    {        
        if (!GetIsEditable())
        {
            MMIWIDGET_RunChildWidgetProc(idle_grid_ptr, 
                        idle_grid_ptr->tp_move_info.click_id, 
                        MSG_WIDGET_TP_PRESS_UP, param);
        }
    }
    //else
    if (MMIGRID_TP_MOVE_PAGE == idle_grid_ptr->widget_mov_type)
    {        
        if (MMIGRID_TP_CLICK_WIDGET == idle_grid_ptr->tp_move_info.click_type)
        {
            item_ptr = MMIWIDGET_FindWidgetItem(idle_grid_ptr, idle_grid_ptr->tp_move_info.click_id);

            if (PNULL != item_ptr && PNULL != item_ptr->item_info.ResetChildWinItem)
            {
               item_ptr->item_info.ResetChildWinItem();
            }
        }

#ifdef PDA_UI_SUPPORT_U_IDLE
        if (   MMIGRID_RUN_MODE_MOVABLE != idle_grid_ptr->run_mode
            && MMIUIdle_IsOpen())
        {
            uint16 dst_page_idx = 0;
            
            dst_page_idx = MMIUIdle_HandleUIdleSlideTpUp(idle_grid_ptr, point); 
            DrawUIdleLastFrame(idle_grid_ptr, dst_page_idx);
        }
        else
#endif        
        HandleSlidePageTpUp(idle_grid_ptr, point);

        if (MMIGRID_RUN_MODE_MOVABLE == idle_grid_ptr->run_mode)
        {
            return TRUE;            
        }

        ResetWidgetTpState(idle_grid_ptr);
        MMIGRID_SaveWidgetInfoToNV(idle_grid_ptr);        
        return TRUE;
    }
    else if(MMIGRID_TP_CLICK_LONG_SHORTCUT == idle_grid_ptr->tp_move_info.click_type)
    {
        if (!idle_grid_ptr->is_item_upon_recycle)
        {        
            if (!LayoutShortCutItem(idle_grid_ptr))
            {
                idle_grid_ptr->run_mode = MMIGRID_RUN_MODE_MOVABLE;
                idle_grid_ptr->movable_tp_info = idle_grid_ptr->tp_move_info;
                return TRUE;
            }
        }
        else
        {
            MMIWIDGET_RemoveShortcutInAddingList(idle_grid_ptr,idle_grid_ptr->tp_move_info.click_id);
        }

        idle_grid_ptr->run_mode = MMIGRID_RUN_MODE_NORMAL;
        ClearOldGridArea(idle_grid_ptr);
        DestroyMoveLayer(idle_grid_ptr);
        
        //reset to main layer
        MMIWIDGET_ResetHitWidgetLayer(idle_grid_ptr);        
        ResetWidgetTpState(idle_grid_ptr);
        MMIGRID_SaveWidgetInfoToNV(idle_grid_ptr);        
        DrawAll(idle_grid_ptr);
        return TRUE;
    }
    else if(   MMIGRID_TP_MOVE_PAGE != idle_grid_ptr->widget_mov_type
            && MMIGRID_TP_CLICK_SHORTCUT == idle_grid_ptr->tp_move_info.click_type)
    {
        
        if (GetIsEditable())
        {           
			//if(GUI_PointIsInRect(point, GetEditRect(item_ptr,&idle_grid_ptr->tp_move_info,point)))
            if(PointIsInEditIconRect(idle_grid_ptr, &idle_grid_ptr->tp_move_info, point))
			{
                MMIWIDGET_RemoveShortcutInAddingList(idle_grid_ptr,idle_grid_ptr->tp_move_info.click_id);
                grid_index = MMIWIDGET_GetGridIndexByPos(idle_grid_ptr, point);
				MMIWIDGET_ResetGridTblByScId(idle_grid_ptr, (uint16)idle_grid_ptr->cur_page_index, grid_index, MMIGRID_ITEM_TYPE_ICON);  
				ClearOldGridArea(idle_grid_ptr);
				//reset to main layer
				MMIWIDGET_ResetHitWidgetLayer(idle_grid_ptr); 
				ResetWidgetTpState(idle_grid_ptr);
				MMIGRID_SaveWidgetInfoToNV(idle_grid_ptr);    
				DrawAll(idle_grid_ptr);
				return TRUE;
			}   
        }
        else
        {
			//在无sim卡时，不弹出弹出框，同时恢复显示下按下的透明效果。
			if (ID_COMMUNICATION_STK == idle_grid_ptr->icon_ptr[idle_grid_ptr->tp_move_info.click_id].icon_info.menu_id
				&& 0 == MMIAPISTK_GetStkCardNum( PNULL, 0 ))
			{
				DrawAll(idle_grid_ptr);
				return TRUE;
			}
            MMIAPIMENU_HandleIdleMenuItem(idle_grid_ptr->icon_ptr[idle_grid_ptr->tp_move_info.click_id].icon_info.group_id, idle_grid_ptr->icon_ptr[idle_grid_ptr->tp_move_info.click_id].icon_info.menu_id);
            idle_grid_ptr = (MMI_IDLE_GRID_DATA_T *)MMIAPIGRID_GetGridPtr();
            if (PNULL != idle_grid_ptr)
		    {
		    	ResetWidgetTpState(idle_grid_ptr);
            }
            return TRUE;    
        }
    }    
#ifdef MMI_IDLE_THUMBVIEW_SLIDE
    else if(   MMIGRID_TP_MOVE_PAGE != idle_grid_ptr->widget_mov_type
            && MMIGRID_TP_CLICK_THUMBVIEW_SLIDE_BAR == idle_grid_ptr->tp_move_info.click_type)
    {
        uint8 cur_page=0;
        uint16 lcd_width=0;
        uint16 lcd_height=0;
        int16 wp_offset_x=0;
        GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height); 
        idle_grid_ptr->run_mode = MMIGRID_RUN_MODE_NORMAL;
        MMIWIDGET_ResetHitWidgetLayer(idle_grid_ptr); 
        ResetWidgetTpState(idle_grid_ptr);
        DestroyGridThumbViewSlideLayer(idle_grid_ptr);
        MMIGRID_SetBltLayer(idle_grid_ptr);
        cur_page=GetCurThumbviewPage(point);
        if(idle_grid_ptr->cur_page_index!=cur_page)
        {
            idle_grid_ptr->cur_page_index=cur_page;
            SetPageLayerPosition( idle_grid_ptr, &idle_grid_ptr->page_layer, 0 , 0);
        }
        wp_offset_x = -idle_grid_ptr->cur_page_index * GetWidgetPageWidth(idle_grid_ptr);          
        SetWallpaperPosition(idle_grid_ptr, wp_offset_x, 0, FALSE);
        DrawAll(idle_grid_ptr);                
        return TRUE;    
    } 
#endif
    else
    {  
        if (MMIGRID_TP_CLICK_LONG_WIDGET == idle_grid_ptr->tp_move_info.click_type)
        {   
            point.x = offsetX;
            point.y = offsetY;
            
            if (!ResetChildWinPosition(idle_grid_ptr, point))
            {
                idle_grid_ptr->run_mode = MMIGRID_RUN_MODE_MOVABLE;
                idle_grid_ptr->movable_tp_info = idle_grid_ptr->tp_move_info;
                return TRUE;
            }

            ClearOldGridArea(idle_grid_ptr);
            idle_grid_ptr->run_mode = MMIGRID_RUN_MODE_NORMAL;
        } 


        UILAYER_GetLayerPosition(&idle_grid_ptr->move_layer, &offsetX, &offsetY);

        DestroyMoveLayer(idle_grid_ptr);
        
        //reset to main layer
        MMIWIDGET_ResetHitWidgetLayer(idle_grid_ptr);

        if (MMIGRID_TP_MOVE_IDLE_TO_IDLE == idle_grid_ptr->widget_mov_type)
        {
            MMIWIDGET_DispatchMsgToActiveChildWin(idle_grid_ptr, MSG_WIDGET_TP_PRESS_LONG_UP);
        }
        else
        if (   MMIGRID_TP_CLICK_IDLE_EMPTY == idle_grid_ptr->tp_move_info.click_type
            && MMIWIDGET_INVALID_ID == idle_grid_ptr->tp_move_info.click_id)
        {
            DestroyMoveLayer(idle_grid_ptr);
              
            ResetWidgetTpState(idle_grid_ptr);
            //MMIGRID_SaveWidgetInfoToNV(idle_grid_ptr);//NEWMS00182546 
            return TRUE;
        }
        else
        if (   (MMIGRID_TP_MOVE_PAGE != idle_grid_ptr->widget_mov_type)
            && (MMIGRID_TP_CLICK_LONG_WIDGET != idle_grid_ptr->tp_move_info.click_type))
        {
            item_ptr = MMIWIDGET_FindWidgetItem(idle_grid_ptr, idle_grid_ptr->tp_move_info.click_id);
            if(item_ptr != PNULL)
            {       
                if(GetIsEditable())
                {                
			    //if(GUI_PointIsInRect(point, GetEditRect(item_ptr,&idle_grid_ptr->tp_move_info,point)))			    
                if(PointIsInEditIconRect(idle_grid_ptr, &idle_grid_ptr->tp_move_info, point))
			    {
				    MMIWIDGET_DeactiveWidgetItem(idle_grid_ptr, idle_grid_ptr->tp_move_info.click_id);
				    MMIWIDGET_ResetGridTblByScId(idle_grid_ptr, idle_grid_ptr->cur_page_index, item_ptr->item_info.widget_id, MMIGRID_ITEM_TYPE_WIDGET);
				    ClearOldGridArea(idle_grid_ptr);
				    MMIWIDGET_InsertWidget2AddingList(idle_grid_ptr, idle_grid_ptr->tp_move_info.click_id, 0);  
				    //reset to main layer
				    MMIWIDGET_ResetHitWidgetLayer(idle_grid_ptr); 
				    ResetWidgetTpState(idle_grid_ptr);
				    MMIGRID_SaveWidgetInfoToNV(idle_grid_ptr);    
				    DrawAll(idle_grid_ptr); 
				    return TRUE;
			    }
            }
            }
            
            if(!GetIsEditable())
            {
                if(!idle_grid_ptr->is_child_win_btn)
                {   
                    MMIWIDGET_RunChildWidgetProc(idle_grid_ptr, idle_grid_ptr->tp_move_info.click_id,
                        MSG_WIDGET_TP_PRESS_UP, param);
                }
            }
        }              
    }

    ResetWidgetTpState(idle_grid_ptr);
    MMIGRID_SaveWidgetInfoToNV(idle_grid_ptr);    
    DrawAll(idle_grid_ptr);    
    return TRUE;
}

/*****************************************************************************/
//  Description : save widget info to NV
//  Global resource dependence : 
//  Author:hongbo.lan
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIGRID_SaveWidgetInfoToNV(
                                 MMI_IDLE_GRID_DATA_T *idle_grid_ptr
                                 )
{
    MMI_GRID_SAVE_INFO_T *save_info = PNULL;
    uint32 i = 0;
    uint32 widget_id = MMIWIDGET_INVALID_ID;
    uint32 index = 0;
    uint32 all_page_grid_num = GetWidgetMaxGridNum(idle_grid_ptr);

    if (PNULL == idle_grid_ptr)
    {
        //SCI_TRACE_LOW:"SaveWidgetInfoToNV() PNULL == idle_grid_ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIGRID_IDLE_4436_112_2_18_3_7_6_16,(uint8*)"");
        return FALSE;
    }

    save_info = (MMI_GRID_SAVE_INFO_T*)SCI_ALLOC_APP(sizeof(MMI_GRID_SAVE_INFO_T));
   
    save_info->widget_shortcut_list.item_num = idle_grid_ptr->shortcut.shortcut_num;
    
    for (i = 0; i < idle_grid_ptr->shortcut.shortcut_num; i++)
    {
        widget_id = idle_grid_ptr->shortcut.shortcut_item[i].widget_id;
        if (MMIWIDGET_IsValidWidgetId(widget_id))
        {
            save_info->widget_shortcut_list.item_info[index++].widget_id = widget_id;
        }
    }
   
    save_info->page_num = idle_grid_ptr->page_num;
    save_info->cur_page_index = idle_grid_ptr->cur_page_index;
    save_info->is_nv_set = TRUE;
    SCI_MEMCPY(save_info->icon_item_info, idle_grid_ptr->icon_ptr, all_page_grid_num*sizeof(MMI_IDLE_SC_ICON_T));
    SCI_MEMCPY(save_info->sc_grid_data_tab, idle_grid_ptr->grid_data_tab, all_page_grid_num*sizeof(uint16));   

#ifdef MMI_SAM_THEME	
    if (MMIAPISET_GetIdleSubStyle() == MMISET_IDLE_SUBSTYLE_SAM)
    {
        MMINV_WRITE(MMI_SAM_SAVE_INFO, save_info);    
    }
    else
#endif
    {
        MMINV_WRITE(MMI_GRID_SAVE_INFO, save_info);
    }

    SCI_FREE(save_info);  
    save_info = PNULL;
          
    return TRUE;
}

/*****************************************************************************/
//  Description : handle widget tp down
//  Global resource dependence : 
//  Author:hongbo.lan
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN SetGridTPClickType(
                                 MMI_IDLE_GRID_DATA_T *idle_grid_ptr,
                                 MMI_GRID_TP_CLICK_TYPE_E click_type
                                 )
{
    if (PNULL == idle_grid_ptr)
    {
        //SCI_TRACE_LOW:"SetWidgetTPHitType() PNULL == idle_grid_ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIGRID_IDLE_4531_112_2_18_3_7_6_18,(uint8*)"");
        return FALSE;
    }

    idle_grid_ptr->tp_move_info.click_type = click_type;
    return TRUE;
}

/*****************************************************************************/
//  Description : handle all widget move action.
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note: 
/*****************************************************************************/
LOCAL void ProcessWidgetMove(MMI_IDLE_GRID_DATA_T *idle_grid_ptr, GUI_POINT_T point, MMI_GRID_TP_MOVE_TYPE_E move_type)
{
    switch(move_type)
    {
        case MMIGRID_TP_MOVE_IDLE_TO_IDLE:
            MoveWidgetIdle2Idle(idle_grid_ptr, point);
            break;

        case MMIGRID_TP_MOVE_PAGE:
#ifdef  PDA_UI_SUPPORT_U_IDLE
            if (   MMIGRID_RUN_MODE_MOVABLE != idle_grid_ptr->run_mode
                && MMIUIdle_IsOpen())
            {
                if (!MMIUIdle_HandleUIdleSlideTpMov(idle_grid_ptr, point))
                {
                    DrawPageLayer(idle_grid_ptr);
                    HandleSlidePageTpMove(idle_grid_ptr, point); 
                }
            }
            else
#endif
            HandleSlidePageTpMove(idle_grid_ptr, point); 
            break;
            
        default:
            break;
    }
}

/*****************************************************************************/
//  Description : handle widget tp long down
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN HandleGridTpLongDown(
                                 MMI_IDLE_GRID_DATA_T *idle_grid_ptr,
                                 DPARAM                      param
                                 )
{
    uint16 click_widget_id = MMIWIDGET_INVALID_ID;
    GUI_POINT_T point = {0};
    GUI_RECT_T cur_rect = {0};

    MMIGRID_ITEM_TYPE_E press_type = 0;
    
    point.x = MMK_GET_TP_X(param);
    point.y = MMK_GET_TP_Y(param);   


    if (   MMIGRID_RUN_MODE_THUMBVIEW == idle_grid_ptr->run_mode
        || MMIGRID_RUN_MODE_MOVABLE == idle_grid_ptr->run_mode)
    {
        return FALSE;
    }

    if (   (MMIGRID_TP_MOVE_PAGE == idle_grid_ptr->widget_mov_type)
        || (MMIGRID_TP_MOVE_IDLE_TO_IDLE== idle_grid_ptr->widget_mov_type))
    {
        return FALSE;
    }
    
    press_type = MMIWIDGET_GetGridTpPressedItem(idle_grid_ptr, point, &click_widget_id);
    
        if (MMIGRID_ITEM_TYPE_WIDGET == press_type)
        {       
            if (MMIWIDGET_INVALID_ID != click_widget_id)
            {
                if (MMI_RESULT_TRUE == MMIWIDGET_RunChildWidgetProc(idle_grid_ptr, click_widget_id, MSG_WIDGET_TP_PRESS_LONG_DOWN, param))
                {
                    return FALSE;
                }
                SetGridTPClickType(idle_grid_ptr, MMIGRID_TP_CLICK_LONG_WIDGET);  
                idle_grid_ptr->run_mode = MMIGRID_RUN_MODE_MOVABLE;
                idle_grid_ptr->tp_move_info.click_id = click_widget_id;     
                idle_grid_ptr->movable_tp_info.click_type = MMIGRID_TP_CLICK_LONG_WIDGET;
                idle_grid_ptr->movable_tp_info.click_id = click_widget_id;     
                MMIWIDGET_ResetGridTblByScId(idle_grid_ptr, (uint16)idle_grid_ptr->cur_page_index, idle_grid_ptr->tp_move_info.click_id, MMIGRID_ITEM_TYPE_WIDGET);
                //DrawSpecificPage(idle_grid_ptr, idle_grid_ptr->cur_page_index);
                DrawPageLayer( idle_grid_ptr);
                HandleWidgetTpLongDown(idle_grid_ptr); 
                
                SaveTpMovePoint(idle_grid_ptr, point);
                GetHightlightItemRect(idle_grid_ptr, &idle_grid_ptr->old_grid.filled_grid_cell, &cur_rect);
                DrawRect(idle_grid_ptr, cur_rect, MMI_GREEN_COLOR);                 
                    
                CreateRecycleLayer(idle_grid_ptr);  
                DrawRecycle( idle_grid_ptr);                
                PlayRecycleEnterAnim(idle_grid_ptr);
            }
            else
            {
                idle_grid_ptr->tp_move_info.click_id = MMIWIDGET_INVALID_ID;
                SetGridTPClickType(idle_grid_ptr, MMIGRID_TP_CLICK_INVALID);
                SaveTpMovePoint(idle_grid_ptr, point);
            }
        }
        else if(MMIGRID_ITEM_TYPE_ICON == press_type)
        {
            SetGridTPClickType(idle_grid_ptr, MMIGRID_TP_CLICK_LONG_SHORTCUT);
            idle_grid_ptr->run_mode = MMIGRID_RUN_MODE_MOVABLE;
            idle_grid_ptr->tp_move_info.click_id = GET_NODE_ID(idle_grid_ptr->grid_data_tab[idle_grid_ptr->cur_page_index][click_widget_id]);
            idle_grid_ptr->movable_tp_info.click_type = MMIGRID_TP_CLICK_LONG_SHORTCUT;
            idle_grid_ptr->movable_tp_info.click_id = idle_grid_ptr->tp_move_info.click_id;     
            
            MMIWIDGET_ResetGridTblByScId(idle_grid_ptr, (uint16)idle_grid_ptr->cur_page_index, click_widget_id, MMIGRID_ITEM_TYPE_ICON);            
            //DrawSpecificPage(idle_grid_ptr, idle_grid_ptr->cur_page_index);           
            DrawPageLayer( idle_grid_ptr);
            HandleIconTpLongDown(idle_grid_ptr,point);            
            //DrawAll(idle_grid_ptr);
            SaveTpMovePoint(idle_grid_ptr, point);

            GetHightlightItemRect(idle_grid_ptr, &idle_grid_ptr->old_grid.filled_grid_cell, &cur_rect);
            DrawRect(idle_grid_ptr, cur_rect, MMI_GREEN_COLOR);
                
            CreateRecycleLayer(idle_grid_ptr);  
            DrawRecycle( idle_grid_ptr);                
            PlayRecycleEnterAnim(idle_grid_ptr);
        }        
#ifdef MMI_IDLE_THUMBVIEW_SLIDE
        else 
        if(MMIGRID_ITEM_TYPE_PAGE_BAR == press_type)
        {
            UILAYER_APPEND_BLT_T append_layer={0};
            idle_grid_ptr->run_mode = MMIGRID_RUN_MODE_THUMBVIEW_SLIDE;
            DestroyGridThumbViewSlideLayer(idle_grid_ptr);
            CreateThumbViewSlideLayer(idle_grid_ptr);
            UILAYER_SetLayerPosition(&idle_grid_ptr->thumb_view_slide_layer, 0, 0);
            //UILAYER_RemoveBltLayer(&idle_grid_ptr->page_layer);
            //UILAYER_RemoveBltLayer(&idle_grid_ptr->softkey_layer);
            append_layer.lcd_dev_info = idle_grid_ptr->thumb_view_slide_layer;
            append_layer.layer_level = UILAYER_LEVEL_NORMAL;
            UILAYER_AppendBltLayer(&append_layer);
            //DisplayGridSlidePageThumbView(idle_grid_ptr,point);
            DrawSlidePageTitleBar(point);
            SaveTpMovePoint(idle_grid_ptr, point);                
        } 
#endif
        else
        {
            MMIWIDGET_CreatePopupAddWin(idle_grid_ptr);
            ResetWidgetTpState(idle_grid_ptr);
        }
    return TRUE;
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIGRID_AddMenu2Idle(MMIMAINMENU_ITEM_INFO_T shortcut_info, GUI_POINT_T start_pos)
{
    uint32 i = 0;
    MMI_IDLE_GRID_DATA_T *idle_grid_ptr = (MMI_IDLE_GRID_DATA_T *)MMIAPIGRID_GetGridPtr(); 
    uint32 all_page_grid_num = GetWidgetMaxGridNum(idle_grid_ptr);
    if (PNULL == idle_grid_ptr)
    {
        return;
    }
    idle_grid_ptr->down_point = start_pos;

    //SCI_TRACE_LOW:"from main menu: group id = %d, menu id = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIGRID_IDLE_4776_112_2_18_3_7_7_19,(uint8*)"dd", shortcut_info.group_id, shortcut_info.menu_id);
     
    for (i = 0; i < all_page_grid_num; i++)
    {
        if (   0 == idle_grid_ptr->icon_ptr[i].icon_info.group_id
            && 0 == idle_grid_ptr->icon_ptr[i].icon_info.menu_id)
        {
            idle_grid_ptr->icon_ptr[i].icon_info = shortcut_info;
            idle_grid_ptr->icon_ptr[i].icon_num++;
            break;
        }    
        else
        if (   shortcut_info.group_id == idle_grid_ptr->icon_ptr[i].icon_info.group_id
            && shortcut_info.menu_id == idle_grid_ptr->icon_ptr[i].icon_info.menu_id)
        {
            idle_grid_ptr->icon_ptr[i].icon_num++;
            break;
        }
    }

    //SCI_TRACE_LOW:"idle_grid_ptr->icon_ptr[%d].icon_info. group_id = %d, menu_id = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIGRID_IDLE_4796_112_2_18_3_7_7_20,(uint8*)"ddd", idle_grid_ptr->icon_ptr[i].icon_info.group_id, idle_grid_ptr->icon_ptr[i].icon_info.menu_id);
    SetGridTPClickType(idle_grid_ptr, MMIGRID_TP_CLICK_LONG_SHORTCUT);
    idle_grid_ptr->tp_move_info.click_id = i;  
    idle_grid_ptr->run_mode = MMIGRID_RUN_MODE_MOVABLE; 
    idle_grid_ptr->movable_tp_info.click_id = i;
    idle_grid_ptr->movable_tp_info.click_type = MMIGRID_TP_CLICK_LONG_SHORTCUT;   
}

/*****************************************************************************/
//  Description : add new widget to idle.
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIGRID_AddNewWidget(uint32 widget_id)
{
    MMI_IDLE_GRID_DATA_T *idle_grid_ptr = PNULL;
    MMI_GRID_CELL_T       valid_grid    = {0};
    GUI_RECT_T            rect          = {0};
    GUI_RECT_T            dst_rect      = {0};
    MMIWIDGET_ITEM_T      item_info     = {0};
    uint16                img_width     = 0;
    uint16                img_height    = 0;
#if defined MMI_GRID_CONTACT || defined MMI_WIDGET_MEMO
    int32                 i             = 0; 
#endif
    MMI_IMAGE_ID_T        bg_img        = 0;
    idle_grid_ptr = (MMI_IDLE_GRID_DATA_T *)MMIAPIGRID_GetGridPtr(); 
    
    if(PNULL == idle_grid_ptr )
    {
        return;
    }  
    if(widget_id == 0 || widget_id >= MMIWIDGET_WIDGET_MAX)
    {   
        return ;
    }
    item_info = idle_grid_ptr->item_ptr[widget_id - 1];

    MMIWIDGET_GetWidgetImage(&item_info.item_info, &bg_img, PNULL);

    GUIRES_GetImgWidthHeight(&img_width, &img_height, bg_img, idle_grid_ptr->win_id);

    rect.left   = 0;
    rect.top    = MMIGRID_IDLE_RECYCLE_HEIGHT;
    rect.right  = rect.left + img_width - 1;
    rect.bottom = rect.top + img_height - 1; 

#ifdef MMI_GRID_CONTACT
	if ((MMIWIDGET_CONTACT_ADD_ID == widget_id))
	{
		for (i = 0;i < MMIGRID_PB_WIDGET_NUM; i++)
		{
			if (CONTACT_INIT == MMIAPIGRIDPB_GetContactisNull(i))
			{
				widget_id = i + MMIWIDGET_CONTACT_ID0;
				break;
			}
		}
	}
#endif

#ifdef MMI_WIDGET_MEMO 
	if ((MMIWIDGET_MEMO_ID == widget_id))
    {
        for(i = 0;i < MMIGRID_MEMO_WIDGET_NUM; i++)
        {
            if(!MMIGRID_IsMemoStatusFull(i))
            {
                widget_id = i + MMIWIDGET_MEMO_ID0;
                break;
            }
        }
    }
#endif

#ifdef SCH_ASSISTANT_WIDGET
    MMIAPIWIDGET_SetScheduleWidgetFlag(TRUE);
#endif
	MMIWIDGET_RemoveWidgetInAddingList(idle_grid_ptr, widget_id);
    
    if(MMIWIDGET_LayoutGridTblByRect(idle_grid_ptr, idle_grid_ptr->cur_page_index, rect, &valid_grid, &dst_rect))
    {
        MMIWIDGET_FillGridTbl(idle_grid_ptr,&valid_grid,idle_grid_ptr->cur_page_index,widget_id,MMIGRID_ITEM_TYPE_WIDGET);
    }
    else
    {
        SetGridTPClickType(idle_grid_ptr, MMIGRID_TP_CLICK_LONG_WIDGET);
        idle_grid_ptr->tp_move_info.click_id = widget_id;
        idle_grid_ptr->run_mode = MMIGRID_RUN_MODE_MOVABLE;
        idle_grid_ptr->movable_tp_info.click_id = widget_id;
        idle_grid_ptr->movable_tp_info.click_type = MMIGRID_TP_CLICK_LONG_WIDGET;
    }
    
	MMIWIDGET_ActiveWidgetItem(idle_grid_ptr, widget_id);
#if defined BACKGROUND_GRID_LINE_LAYER
	CreateEditBgLayer(idle_grid_ptr);
#endif
	SetIsEditable(TRUE);
    return;
}

/*****************************************************************************/
//  Description : add new shortcut item to idle.
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note: 
/*****************************************************************************/
PUBLIC void MMIGRID_AddNewShortcut(MMIMAINMENU_ITEM_INFO_T *menu_info_ptr)
{
    uint32 i = 0;
    uint16                img_width     = 0;
    uint16                img_height    = 0; 
    CTRLMAINMENU_ITEM_T   menuitem_info = {0};
    MMI_GRID_CELL_T       valid_grid    = {0};
    GUI_RECT_T            rect          = {0};
    GUI_RECT_T            dst_rect      = {0};
    MMI_IMAGE_ID_T        img_id        = 0;  
    MMI_IDLE_GRID_DATA_T *idle_grid_ptr = (MMI_IDLE_GRID_DATA_T *)MMIAPIGRID_GetGridPtr(); 
    uint32 all_page_grid_num = 0;
    if (PNULL == idle_grid_ptr)
    {
        return;
    }
    
    all_page_grid_num = GetWidgetMaxGridNum(idle_grid_ptr);
    
    for (i = 0; i < all_page_grid_num; i++)
    {
        if (   0 == idle_grid_ptr->icon_ptr[i].icon_info.group_id
            && 0 == idle_grid_ptr->icon_ptr[i].icon_info.menu_id)
        {
            idle_grid_ptr->icon_ptr[i].icon_info = *menu_info_ptr;
            idle_grid_ptr->icon_ptr[i].icon_num++;
            break;
        }    
        else
        if (   menu_info_ptr->group_id == idle_grid_ptr->icon_ptr[i].icon_info.group_id
            && menu_info_ptr->menu_id == idle_grid_ptr->icon_ptr[i].icon_info.menu_id)
        {
            idle_grid_ptr->icon_ptr[i].icon_num++;
            break;
        }
    }

    if (all_page_grid_num == i)
    {
        return;
    }
    
    MMIMAINMENU_GetItemData(
        menu_info_ptr->menu_id,
        menu_info_ptr->group_id,
        &menuitem_info);

    if (0 == menuitem_info.group_id)
	//if (GUIMENU_MAINMENU_DATA_TYPE_STATIC == menuitem_info.menu_data_type)
    {
        img_id = menuitem_info.icon.data.icon_id;//menuitem_info.menu_data.static_menu_ptr->unselect_icon_id;
    }
#ifdef DYNAMIC_MAINMENU_SUPPORT    
    else
    //if (GUIMENU_MAINMENU_DATA_TYPE_DYNAMIC == menuitem_info.menu_data_type)
    {
        img_id  = menuitem_info.active_icon.data.icon_id;//menuitem_info.menu_data.dynamic_menu.ori_data_ptr->dynamic_menu_info.select_icon_id;
    }
//     if ((GUIMENU_MAINMENU_DATA_TYPE_DYNAMIC == menu_item_ptr->menu_data_type)
//         && (PNULL != menu_item_ptr->menu_data.dynamic_menu_ptr)
//         && (PNULL != menu_item_ptr->menu_data.dynamic_menu.ori_data_ptr))
//     {
// 
//         SCI_FREE(menu_item_ptr->menu_data.dynamic_menu_ptr);
//     }
#endif

    //SCI_FREE(menu_item_ptr);
    // MMITHEME_DestroyMenuDataInfo( menu_item_ptr );

    GUIRES_GetImgWidthHeight(&img_width, &img_height, img_id, idle_grid_ptr->win_id);

    rect.left = 0;
    rect.top  = MMIGRID_IDLE_RECYCLE_HEIGHT;
    rect.right = rect.left + img_width - 1;
    rect.bottom = rect.top + img_height - 1;
    
    if(MMIWIDGET_LayoutGridTblByRect(idle_grid_ptr, idle_grid_ptr->cur_page_index, rect, &valid_grid, &dst_rect))
    {
        MMIWIDGET_FillGridTbl(idle_grid_ptr,&valid_grid,idle_grid_ptr->cur_page_index, i, MMIGRID_ITEM_TYPE_ICON);
    }
    else
    {
        SetGridTPClickType(idle_grid_ptr, MMIGRID_TP_CLICK_LONG_SHORTCUT);
        idle_grid_ptr->tp_move_info.click_id = i; 
        idle_grid_ptr->run_mode = MMIGRID_RUN_MODE_MOVABLE;
        idle_grid_ptr->movable_tp_info.click_id = i;
        idle_grid_ptr->movable_tp_info.click_type = MMIGRID_TP_CLICK_LONG_SHORTCUT;   
    }
#if defined BACKGROUND_GRID_LINE_LAYER
	CreateEditBgLayer(idle_grid_ptr);
#endif
	SetIsEditable(TRUE);
}

/*****************************************************************************/
//  Description : handle grid item on movable mode tp down action.
//  Global resource dependence : 
//  Author:hongbo.lan
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN HandleMoveModeTpDown(
                               MMI_IDLE_GRID_DATA_T *idle_grid_ptr,
                               DPARAM            param
                               )
{
    uint16 click_id = MMIWIDGET_INVALID_ID;
    GUI_POINT_T point = {0};
    GUI_RECT_T layer_rect = {0};
    MMIGRID_ITEM_TYPE_E press_type = 0;
    
    point.x = MMK_GET_TP_X(param);
    point.y = MMK_GET_TP_Y(param);
    
    if (PNULL == idle_grid_ptr)
    {
        return FALSE;
    }
    idle_grid_ptr->down_point = point;
    idle_grid_ptr->save_point = point;

    layer_rect = UILAYER_GetLayerRect(&idle_grid_ptr->move_layer);

    if (GUI_PointIsInRect(point, layer_rect))
    {
        idle_grid_ptr->tp_move_info = idle_grid_ptr->movable_tp_info;
        SaveTpMovePoint(idle_grid_ptr, point);
        return TRUE; 
    }
    else
    {

        press_type = MMIWIDGET_GetGridTpPressedItem(idle_grid_ptr, point, &click_id);
        
        if (MMIGRID_ITEM_TYPE_WIDGET == press_type)
        {
            idle_grid_ptr->tp_move_info.click_id = click_id;
            SetGridTPClickType(idle_grid_ptr, MMIGRID_TP_CLICK_WIDGET); 
        }
        else
        if(MMIGRID_ITEM_TYPE_ICON ==  press_type)
        {
            idle_grid_ptr->tp_move_info.click_id = GET_NODE_ID(idle_grid_ptr->grid_data_tab[idle_grid_ptr->cur_page_index][click_id]);
            SetGridTPClickType(idle_grid_ptr, MMIGRID_TP_CLICK_SHORTCUT); 
        }
        else
        {
            idle_grid_ptr->tp_move_info.click_id = MMIWIDGET_INVALID_ID;
            SetGridTPClickType(idle_grid_ptr, MMIGRID_TP_CLICK_IDLE_EMPTY);
        }
        SaveTpMovePoint(idle_grid_ptr, point); 
        return TRUE;
    }
}                               

/*****************************************************************************/
//  Description : handle widget tp down
//  Global resource dependence : 
//  Author:hongbo.lan
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN HandleGridTpDown(
                                 MMI_IDLE_GRID_DATA_T *idle_grid_ptr,
                                 DPARAM            param
                                 )
{
    uint16 click_id = MMIWIDGET_INVALID_ID;
    MMIGRID_ITEM_TYPE_E press_type = MMIGRID_ITEM_TYPE_INVALID;
    GUI_POINT_T point = {0};
    MMI_HANDLE_T child_win_handle = 0;

    point.x = MMK_GET_TP_X(param);
    point.y = MMK_GET_TP_Y(param);

    if (MMIGRID_RUN_MODE_MOVABLE == idle_grid_ptr->run_mode)
    {
        HandleMoveModeTpDown(idle_grid_ptr, param);
        return TRUE;
    }
    
    ResetWidgetTpState(idle_grid_ptr);  
        idle_grid_ptr->down_point = point;
        idle_grid_ptr->save_point = point;
        if (MMIGRID_RUN_MODE_THUMBVIEW == idle_grid_ptr->run_mode)
        {
            SetGridTPClickType(idle_grid_ptr, MMIGRID_TP_CLICK_THUMBVIEW);
            SaveTpMovePoint(idle_grid_ptr, point);
            return TRUE;
        }
       
        press_type = MMIWIDGET_GetGridTpPressedItem(idle_grid_ptr, point, &click_id);
        if (MMIGRID_ITEM_TYPE_WIDGET == press_type)
        {       
            if (MMIWIDGET_INVALID_ID != click_id)
            { 
                child_win_handle = MMIWIDGET_GetWinHandleByWidgetId(idle_grid_ptr, click_id);
                //MMK_SetChildWinFocus(idle_grid_ptr->win_id, child_win_handle);

                if(!GetIsEditable())
                {
                    if (MMI_RESULT_TRUE == MMIWIDGET_RunChildWidgetProc(idle_grid_ptr, click_id, MSG_WIDGET_TP_PRESS_DOWN, param))
                    {                       
                         idle_grid_ptr->is_child_win_btn           = TRUE; 
                    }
                } 
                idle_grid_ptr->tp_move_info.click_id = click_id;
                SetGridTPClickType(idle_grid_ptr, MMIGRID_TP_CLICK_WIDGET); 
                SaveTpMovePoint(idle_grid_ptr, point);
            }
            else
            {
                idle_grid_ptr->tp_move_info.click_id = MMIWIDGET_INVALID_ID;
                SetGridTPClickType(idle_grid_ptr, MMIGRID_TP_CLICK_INVALID);
                SaveTpMovePoint(idle_grid_ptr, point);
            }
        }
        else if(MMIGRID_ITEM_TYPE_ICON ==  press_type)
        {        
            GUI_RECT_T          display_rc = {0};
            idle_grid_ptr->tp_move_info.click_id = GET_NODE_ID(idle_grid_ptr->grid_data_tab[idle_grid_ptr->cur_page_index][click_id]);
            SetGridTPClickType(idle_grid_ptr, MMIGRID_TP_CLICK_SHORTCUT); 
            SaveTpMovePoint(idle_grid_ptr, point);
            display_rc = idle_grid_ptr->page_grid_rect[click_id];
#ifndef UI_LAYER_SUPPORT_LOW_MEMORY
            GUI_WeekLayer(&idle_grid_ptr->page_layer, &display_rc, MMIMENU_WEAKLAYERALPHA);
            GUILCD_InvalidateRect(0, display_rc, 0);
#endif
        }            
#ifdef MMI_IDLE_THUMBVIEW_SLIDE
        else
        if(MMIGRID_ITEM_TYPE_PAGE_BAR ==  press_type)
        {
            idle_grid_ptr->tp_move_info.click_id = MMIWIDGET_INVALID_ID;
            SetGridTPClickType(idle_grid_ptr, MMIGRID_TP_CLICK_THUMBVIEW_SLIDE_BAR); 
            SaveTpMovePoint(idle_grid_ptr, point);
        } 
#endif
        else
        {
            idle_grid_ptr->tp_move_info.click_id = MMIWIDGET_INVALID_ID;
            SetGridTPClickType(idle_grid_ptr, MMIGRID_TP_CLICK_IDLE_EMPTY); 
            SaveTpMovePoint(idle_grid_ptr, point);
        }
    return TRUE;
}

/*****************************************************************************/
//  Description : tp position for moving along X direction is valid or not.
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsTpXMoveValid(MMI_IDLE_GRID_DATA_T *idle_grid_ptr, GUI_POINT_T point)
{
    if (abs(idle_grid_ptr->down_point.x - point.x) >= MMIWIDGET_MOVE_X_MIN_PIX)
    {
       return TRUE;
    }

    return FALSE;
}

/*****************************************************************************/
//  Description : tp position for moving along Y direction is valid or not.
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsTpYMoveValid(MMI_IDLE_GRID_DATA_T *idle_grid_ptr, GUI_POINT_T point)
{
    if (abs(idle_grid_ptr->down_point.y - point.y) >= MMIWIDGET_MOVE_Y_MIN_PIX)
    {
       return TRUE;
    }

    return FALSE;
}

/*****************************************************************************/
//  Description : judge auto slide or not.
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note: 
/*****************************************************************************/
LOCAL void HandleGridAutoSlideEvent(MMI_IDLE_GRID_DATA_T *idle_grid_ptr, GUI_POINT_T point)
{
    GUI_RECT_T widget_rect = {0};
    GUI_RECT_T layer_rect  = {0};
    int16      posX        = 0;

    widget_rect = idle_grid_ptr->display_rect;
    
    s_is_auto_slide = FALSE;
    
    layer_rect = UILAYER_GetLayerRect(&idle_grid_ptr->move_layer);
    posX = (layer_rect.right - layer_rect.left) / 4;
    
    if (layer_rect.right > widget_rect.right + posX)
    {
        s_is_auto_slide = TRUE;   
        idle_grid_ptr->tp_move_info.pen_move_dir = MMIWIDGET_PEN_MOVE_LEFT;
    }
    else if (layer_rect.left + posX < 0)
    {
        s_is_auto_slide = TRUE;   
        idle_grid_ptr->tp_move_info.pen_move_dir = MMIWIDGET_PEN_MOVE_RIGHT;
    }    
    if(TRUE == s_is_auto_slide)
    {
        //StopAutoSlideTimer();
        CreateAutoSlideTimer(idle_grid_ptr->win_id, MMIWIDGET_AUTO_SLIDE_TIMEOUT);
    }
    else
    {
        StopAutoSlideTimer();
    }    
}

/*****************************************************************************/
//  Description : Display item on recycle bin with red color.
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note:
/*****************************************************************************/
/*****************************************************************************/
//  Description : Display item on recycle bin with red color.
//  Global resource dependence : 
//  Author: ying.xu
//  Note:
/*****************************************************************************/
PUBLIC void FillLayerWithMaskColor(GUI_LCD_DEV_INFO *layer_ptr, GUI_RECT_T *rect_ptr, uint32 mask_color)
{
    UILAYER_INFO_T    layer_info = {0};
    uint8 *layer_buf_ptr = PNULL;
    uint32 *bk_buf_ptr = PNULL;    
    GUI_RECT_T mask_rect = {0};
    GUI_RECT_T layer_rect = {0};
    uint32 mask_width    = 0;
    uint32 mask_height   = 0;
    uint32 color = 0;
    uint32 i = 0;
    uint32 j = 0;
    uint32 mask_a = 0;
    uint32 mask_r = 0;
    uint32 mask_g = 0;
    uint32 mask_b = 0;
    uint8 a = 0;
    uint32 r = 0;
    uint32 g = 0;
    uint32 b = 0;
    uint32 blt_alpha = 192;
    uint32 max_alpha = 255;

    UILAYER_GetLayerInfo(layer_ptr, &layer_info);
    if(DATA_TYPE_ARGB888 != layer_info.data_type)
    {
        return;
    }
    layer_buf_ptr = UILAYER_GetLayerBufferPtr(layer_ptr);

    if (PNULL == layer_buf_ptr/*layer_info.layer_buf_ptr*/)
    {
        return;
    }    
    layer_rect = UILAYER_GetLayerRect(layer_ptr);        
    if (PNULL == rect_ptr)
    {
        mask_rect  = layer_rect;
    }
    else
    {
        if (!GUI_IntersectRect(&mask_rect, *rect_ptr, layer_rect))
        {
            return;
        }
    }
    LCD_ConvertDispRect(GUILCD_CONVERT_POS_LCD2DISP , layer_ptr, &mask_rect, &mask_rect);
    
    mask_height = mask_rect.bottom - mask_rect.top + 1;
    mask_width  = mask_rect.right - mask_rect.left + 1;
    mask_a = (mask_color>>24) & 0xFF;
    mask_r = (mask_color>>16) & 0xFF;
    mask_g = (mask_color>>8)  & 0xFF;
    mask_b = (mask_color)     & 0xFF;

 

    bk_buf_ptr = ((uint32*)layer_buf_ptr/*layer_info.layer_buf_ptr*/)
                 + mask_rect.top * layer_info.mem_width
                 + mask_rect.left; 
    
    for (i = 0; i < mask_height; i++)
    {    
        for (j = 0; j < mask_width; j++)
        {
            color = *(bk_buf_ptr + j);
            a = (color>>24) & 0xFF;
            r = (color>>16) & 0xFF;
            g = (color>>8)  & 0xFF;
            b = (color)     & 0xFF;

            color =   ((((a * (max_alpha - blt_alpha) + mask_a * blt_alpha) /max_alpha) << 24) & 0xFF000000)
                    | ((((r * (max_alpha - blt_alpha) + mask_r * blt_alpha) /max_alpha) << 16) & 0x00FF0000)
                    | ((((g * (max_alpha - blt_alpha) + mask_g * blt_alpha) /max_alpha) << 8) & 0x0000FF00)
                    | ((b * (max_alpha - blt_alpha) + mask_b * blt_alpha) /max_alpha & 0x000000FF);

            *(bk_buf_ptr + j) = color;
        }
        
        bk_buf_ptr += layer_info.mem_width;
    }   
}

LOCAL void FillLayerWithMaskColorEx(GUI_LCD_DEV_INFO *layer_ptr, GUI_RECT_T *rect_ptr, uint32 mask_color)
{
        UILAYER_INFO_T    layer_info = {0};
        uint8 *layer_buf_ptr = PNULL;
        uint32 *bk_buf_ptr = PNULL;    
        GUI_RECT_T mask_rect = {0};
        GUI_RECT_T layer_rect = {0};
        uint32 mask_width    = 0;
        uint32 mask_height   = 0;
        uint32 color = 0;
        uint32 i = 0;
        uint32 j = 0;
        //uint32 mask_a = 0;  /*lint !e529 !e830 */
        uint32 mask_r = 0;
        uint32 mask_g = 0;
        uint32 mask_b = 0;
        //uint8 a = 0;        /*lint !e529 !e830 */
        uint32 r = 0;
        uint32 g = 0;
        uint32 b = 0;
    
        UILAYER_GetLayerInfo(layer_ptr, &layer_info);
        layer_buf_ptr = UILAYER_GetLayerBufferPtr(layer_ptr);
    
        if (PNULL == layer_buf_ptr/*layer_info.layer_buf_ptr*/)
        {
            return;
        }    
        layer_rect = UILAYER_GetLayerRect(layer_ptr);        
    
        if (PNULL == rect_ptr)
        {
            mask_rect  = layer_rect;
        }
        else
        {
            if (!GUI_IntersectRect(&mask_rect, *rect_ptr, layer_rect))
            {
                return;
            }
        }
    
        //mask_a = (mask_color>>24) & 0xFF;
        mask_r = (mask_color>>16) & 0xFF;
        mask_g = (mask_color>>8)  & 0xFF;
        mask_b = (mask_color)     & 0xFF;
        LCD_ConvertDispRect(GUILCD_CONVERT_POS_LCD2DISP , layer_ptr, &mask_rect, &mask_rect);
        mask_height = mask_rect.bottom - mask_rect.top + 1;
        mask_width  = mask_rect.right - mask_rect.left + 1;
    
        bk_buf_ptr = ((uint32*)layer_buf_ptr/*layer_info.layer_buf_ptr*/) + mask_rect.top * layer_info.mem_width + mask_rect.left; 
        
        for (i = 0; i < mask_height; i++)
        {    
            for (j = 0; j < mask_width; j++)
            {
                color = *(bk_buf_ptr + j);
                if((color & 0xffffff) == 0)
                {
                    continue;
                }
                //a = (color>>24) & 0xFF;
                r = (color>>16) & 0xFF;
                g = (color>>8)  & 0xFF;
                b = (color)     & 0xFF;
    
                color =   (color & 0xFF000000)
                        | ((((r * 6  + mask_r * 2 ) >>3 ) << 16) & 0x00FF0000)
                        | ((((g * 6  + mask_g * 2 ) >>3) << 8) & 0x0000FF00)
                        | ((((b * 6  + mask_b * 2)) >>3) & 0x000000FF);
    
                *(bk_buf_ptr + j) = color;
            }
            
            bk_buf_ptr += layer_info.mem_width;
        }   
}


/*****************************************************************************/
//  Description : Display item on recycle bin with red color.
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note:
/*****************************************************************************/
LOCAL void DisplayMoveItemOnRecycle(MMI_IDLE_GRID_DATA_T *idle_grid_ptr)
{    
    if (MMIGRID_TP_CLICK_LONG_WIDGET == idle_grid_ptr->tp_move_info.click_type)
    {
        MMIWIDGET_DisplayWidgetHighlight(idle_grid_ptr, idle_grid_ptr->is_item_upon_recycle);
    }
    else
    if (MMIGRID_TP_CLICK_LONG_SHORTCUT == idle_grid_ptr->tp_move_info.click_type)
    {
        MMIWIDGET_DispShortCutHightlight(idle_grid_ptr, idle_grid_ptr->is_item_upon_recycle);
    }
}


/*****************************************************************************/
//  Description : Check the moving item and recycle bin.
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CheckGridItemRecycleStat(MMI_IDLE_GRID_DATA_T *idle_grid_ptr)
{
    GUI_RECT_T move_layer_rc = {0};
    UILAYER_INFO_T uilayer_info = {0};
    BOOLEAN is_need_draw = FALSE;
    GUI_RECT_T recycle_rect = GetRecycleRect( idle_grid_ptr);
    if (UILAYER_IsLayerActive(&idle_grid_ptr->move_layer))
    {
        UILAYER_GetLayerPosition(&idle_grid_ptr->move_layer, &move_layer_rc.left, &move_layer_rc.top);
        UILAYER_GetLayerInfo(&idle_grid_ptr->move_layer, &uilayer_info);
        move_layer_rc.right = move_layer_rc.left + uilayer_info.layer_width;
        move_layer_rc.bottom= move_layer_rc.top + uilayer_info.layer_height;
        
        if(GUI_IntersectRect(&move_layer_rc, move_layer_rc, recycle_rect))
        {
            if(!idle_grid_ptr->is_item_upon_recycle)
            {
                is_need_draw = TRUE;                
            }            
            idle_grid_ptr->is_item_upon_recycle = TRUE;
        }
        else
        {
            if(idle_grid_ptr->is_item_upon_recycle)
            {
                is_need_draw = TRUE;
            }
            idle_grid_ptr->is_item_upon_recycle = FALSE;
        }
    }
    return is_need_draw;       
}

LOCAL void DisplayGridItemRecycleStat(MMI_IDLE_GRID_DATA_T *idle_grid_ptr)
{
    BOOLEAN is_need_draw = FALSE;
    is_need_draw = CheckGridItemRecycleStat(idle_grid_ptr);
    if(is_need_draw)
    {
        DisplayMoveItemOnRecycle(idle_grid_ptr);     
        DrawRecycle(idle_grid_ptr);
    }
}


/*****************************************************************************/
//  Description : handle widget tp move
//  Global resource dependence : 
//  Author:hongbo.lan
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN HandleGridTpMove(
                                 MMI_IDLE_GRID_DATA_T *idle_grid_ptr,
                                 DPARAM            param
                                 )
{
    GUI_POINT_T point = {0};
    GUI_RECT_T  rect = {0}; 

    point.x = MMK_GET_TP_X(param);
    point.y = MMK_GET_TP_Y(param);
  
    if (   MMIGRID_RUN_MODE_NORMAL == idle_grid_ptr->run_mode
        && MMIGRID_TP_CLICK_WIDGET == idle_grid_ptr->tp_move_info.click_type
        && MMIGRID_TP_MOVE_PAGE != idle_grid_ptr->widget_mov_type)
    {
        idle_grid_ptr->is_child_win_tp_move = FALSE;
       
    	if (MMIWIDGET_RunChildWidgetProc(idle_grid_ptr, idle_grid_ptr->tp_move_info.click_id, MSG_WIDGET_TP_PRESS_MOVE, param))
        {
            //MMITHEME_UpdateRectEx();
        	idle_grid_ptr->is_child_win_tp_move = TRUE;
        	return FALSE;
        }
    }  

        if (!IsTpXMoveValid(idle_grid_ptr, point) && !IsTpYMoveValid(idle_grid_ptr, point))
        {
#ifdef MMI_IDLE_THUMBVIEW_SLIDE
            if (MMIGRID_RUN_MODE_THUMBVIEW_SLIDE == idle_grid_ptr->run_mode
                &&(abs(idle_grid_ptr->down_point.x - point.x) >= MMI_THUMBVIEW_MOVE_X_MIN_PIX))
            {
                idle_grid_ptr->tp_move_is_valid = TRUE;
            }
            else
            {
                return FALSE;
            }
#else
            return FALSE;
#endif
        }
/*
        if(idle_grid_ptr->is_child_win_btn && !idle_grid_ptr->is_child_win_tp_move)
        {
           return FALSE;
        }
*/
        idle_grid_ptr->tp_move_is_valid = TRUE;

    if (MMIGRID_RUN_MODE_THUMBVIEW == idle_grid_ptr->run_mode)
    {
        idle_grid_ptr->widget_mov_type = MMIGRID_TP_MOVE_THUMBVIEW;
        return FALSE;
    }
    

    DisplayGridItemRecycleStat(idle_grid_ptr);

    if (MMIGRID_TP_MOVE_IDLE_TO_IDLE == idle_grid_ptr->widget_mov_type)
    {
        if (!idle_grid_ptr->is_item_upon_recycle && CheckMovingEmptyGrid(idle_grid_ptr, &rect))
        {
            DrawRect(idle_grid_ptr, rect, MMI_GREEN_COLOR);
        }
        else
        if (idle_grid_ptr->showed_rect_info.is_valid)
        {
            ClearRect(idle_grid_ptr);
            idle_grid_ptr->showed_rect_info.is_valid = FALSE;
        }
    }
    if (   MMIGRID_TP_CLICK_WIDGET  == idle_grid_ptr->tp_move_info.click_type
        || MMIGRID_TP_CLICK_SHORTCUT == idle_grid_ptr->tp_move_info.click_type
        || MMIGRID_TP_CLICK_IDLE_EMPTY == idle_grid_ptr->tp_move_info.click_type)
    {
#ifdef PDA_UI_SUPPORT_U_IDLE
        if (   MMIGRID_TP_MOVE_PAGE != idle_grid_ptr->widget_mov_type
            && MMIGRID_RUN_MODE_MOVABLE != idle_grid_ptr->run_mode
            && MMIUIdle_IsOpen())
        {        
            idle_grid_ptr->page_num         = MMI_GRID_PAGE_NUM;
            idle_grid_ptr->is_Uidle_1st_mov = TRUE;
        }
#endif
        if(0 == (idle_grid_ptr->widget_mov_type & MMIGRID_TP_MOVE_PAGE))    /*lint !e655 */
        {
            idle_grid_ptr->widget_mov_type = MMIGRID_TP_MOVE_PAGE;            
            idle_grid_ptr->current_gird = MMI_IDLE_GRID_PAGE_CELL_NUM;  
            MMIWIDGET_DispatchMsgToActiveChildWin(idle_grid_ptr, MSG_WIDGET_PAGE_SLIDE_START);
        }
        ProcessWidgetMove(idle_grid_ptr, point, idle_grid_ptr->widget_mov_type);           
    }
#ifdef MMI_IDLE_THUMBVIEW_SLIDE
    else if(MMIGRID_TP_CLICK_THUMBVIEW_SLIDE_BAR  == idle_grid_ptr->tp_move_info.click_type
             &&MMIGRID_RUN_MODE_THUMBVIEW_SLIDE == idle_grid_ptr->run_mode)
    {
        HandleSlideThumbViewPageTpMove(idle_grid_ptr, point); 
    }
#endif
    else 
    {
            if(MMIGRID_TP_CLICK_LONG_SHORTCUT == idle_grid_ptr->tp_move_info.click_type)
            {
                idle_grid_ptr->widget_mov_type = MMIGRID_TP_MOVE_IDLE_TO_IDLE;
                
                HandleGridAutoSlideEvent(idle_grid_ptr, point);
                ProcessWidgetMove(idle_grid_ptr, point, idle_grid_ptr->widget_mov_type);                
            }

            //move from idle widget to idle widget.
            if (MMIGRID_TP_CLICK_LONG_WIDGET == idle_grid_ptr->tp_move_info.click_type)
            {
                //if (!idle_grid_ptr->is_child_win_btn) 
                {                    
                    HandleGridAutoSlideEvent(idle_grid_ptr, point);
                    idle_grid_ptr->widget_mov_type = MMIGRID_TP_MOVE_IDLE_TO_IDLE;
                    ProcessWidgetMove(idle_grid_ptr, point, idle_grid_ptr->widget_mov_type);
                }
            }          
    }
    return TRUE;
}

/*****************************************************************************/
//  Description : set widget layer blt
//  Global resource dependence : 
//  Author:hongbo.lan
//  Note: 
/*****************************************************************************/
PUBLIC void MMIGRID_SetBltLayer(
                          MMI_IDLE_GRID_DATA_T *idle_grid_ptr
                          )
{
    UILAYER_APPEND_BLT_T append_layer = {0};

	if (UILAYER_IsLayerActive(&idle_grid_ptr->editbg_layer))
	{
		append_layer.lcd_dev_info = idle_grid_ptr->editbg_layer;
		append_layer.layer_level = UILAYER_LEVEL_NORMAL;
		UILAYER_AppendBltLayer(&append_layer);    
	}

    if (UILAYER_IsLayerActive(&idle_grid_ptr->page_layer))
    {
        append_layer.lcd_dev_info = idle_grid_ptr->page_layer;
        append_layer.layer_level = UILAYER_LEVEL_NORMAL;
        UILAYER_AppendBltLayer(&append_layer);   
    }

#ifdef MMI_SAM_THEME
    if (MMIAPISET_GetIdleSubStyle() == MMISET_IDLE_SUBSTYLE_SAM)
    {
        if (UILAYER_IsLayerActive(&idle_grid_ptr->recycle_layer))
        {
            UILAYER_RemoveBltLayer(&idle_grid_ptr->softkey_layer);        
            append_layer.lcd_dev_info = idle_grid_ptr->recycle_layer;
            append_layer.layer_level = UILAYER_LEVEL_NORMAL;
            UILAYER_AppendBltLayer(&append_layer);    
        }
        else
        if (UILAYER_IsLayerActive(&idle_grid_ptr->softkey_layer))
        {
            append_layer.lcd_dev_info = idle_grid_ptr->softkey_layer;
            append_layer.layer_level = UILAYER_LEVEL_NORMAL;
            UILAYER_AppendBltLayer(&append_layer);    
        }    
    }
    else
#endif
    {
        if (UILAYER_IsLayerActive(&idle_grid_ptr->recycle_layer))
        {  
            //UILAYER_RemoveBltLayer(&idle_grid_ptr->stb_layer);
            append_layer.lcd_dev_info = idle_grid_ptr->recycle_layer;
            append_layer.layer_level = UILAYER_LEVEL_NORMAL;
            UILAYER_AppendBltLayer(&append_layer);    
        }
    
        if (UILAYER_IsLayerActive(&idle_grid_ptr->softkey_layer))
        {
            append_layer.lcd_dev_info = idle_grid_ptr->softkey_layer;
            append_layer.layer_level = UILAYER_LEVEL_NORMAL;
            UILAYER_AppendBltLayer(&append_layer);    
        }
    }

    if( MMIGRID_TP_MOVE_NONE != idle_grid_ptr->widget_mov_type)
    {
        uint32 page_layer_size = MMIWIDGET_GetPageLayerSize(idle_grid_ptr);
        if(3 <= page_layer_size)
        {
            //right
            append_layer.lcd_dev_info = idle_grid_ptr->page_cache[1];            /*lint !e415 !e416 !e661 !e662 !e831 */
            append_layer.layer_level = UILAYER_LEVEL_NORMAL;
            UILAYER_AppendBltLayer(&append_layer);   

            //left
            append_layer.lcd_dev_info = idle_grid_ptr->page_cache[(page_layer_size - 1)%page_layer_size];    /*lint !e415 !e416 !e661 !e662 !e831 */
            append_layer.layer_level = UILAYER_LEVEL_NORMAL;
            UILAYER_AppendBltLayer(&append_layer);   
        }
    }
    
    if (UILAYER_IsLayerActive(&idle_grid_ptr->titlebar_layer))
    {
        append_layer.lcd_dev_info = idle_grid_ptr->titlebar_layer;
        append_layer.layer_level = UILAYER_LEVEL_NORMAL;
        UILAYER_AppendBltLayer(&append_layer);    
    }
     
    if (UILAYER_IsLayerActive(&idle_grid_ptr->move_layer))
    {
        append_layer.lcd_dev_info = idle_grid_ptr->move_layer;
        append_layer.layer_level = UILAYER_LEVEL_HIGH;
        UILAYER_AppendBltLayer(&append_layer);    
    }

}



/*****************************************************************************/
//  Description : destroy softkey layer
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note: 
/*****************************************************************************/
LOCAL void DestroySoftkeyLayer(
                              MMI_IDLE_GRID_DATA_T *idle_grid_ptr
                              )
{
    if (PNULL == idle_grid_ptr)
    {
        //SCI_TRACE_LOW:"DestroyElveLayer() PNULL == idle_grid_ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIGRID_IDLE_5897_112_2_18_3_7_9_25,(uint8*)"");
        return;
    }
   
    if (UILAYER_HANDLE_MULTI == UILAYER_GetHandleType(&idle_grid_ptr->softkey_layer))
    {
        UILAYER_RELEASELAYER(&(idle_grid_ptr->softkey_layer));   /*lint !e506 !e774*/
    }
}

LOCAL void DestroyTitlebarLayer(MMI_IDLE_GRID_DATA_T *idle_grid_ptr)
{
    UILAYER_RELEASELAYER(&idle_grid_ptr->titlebar_layer);   /*lint !e506 !e774*/
}

LOCAL void DestroyRecycleLayer(MMI_IDLE_GRID_DATA_T *idle_grid_ptr)
{
    UILAYER_RELEASELAYER(&idle_grid_ptr->recycle_layer);   /*lint !e506 !e774*/
    
    GUIWIN_CreateStbDyn(idle_grid_ptr->win_id); // 动态创建状态栏
    GUIWIN_SetStbState(idle_grid_ptr->win_id, GUISTATUSBAR_STATE_EVENT, TRUE);
    GUIWIN_SetStbBgIsDesplay(idle_grid_ptr->win_id, TRUE);//需求状态栏加半透黑色背景，idle统一处理了    
#ifndef MMI_ONLY_IDLE_DISP_STATUSBAR
    GUIWIN_SetStbBgImageId(idle_grid_ptr->win_id, IMAGE_IDLE_STATUS_BAR);
#endif
    GUIWIN_SetStbState(idle_grid_ptr->win_id, GUISTATUSBAR_STATE_USE_LAYER, TRUE);
    MAIN_SetStbTimeVisible(TRUE);  
    GUIWIN_UpdateStb();
}

/*****************************************************************************/
//  Description : destroy page layer
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note: 
/*****************************************************************************/
LOCAL void DestroyWidgetPageLayer(
                           MMI_IDLE_GRID_DATA_T *idle_grid_ptr
                              )
{
    uint32      page_layer_size = MMIWIDGET_GetPageLayerSize(idle_grid_ptr);
    uint32 i = 0;
    
    for(i = 0; i < page_layer_size; i++)
    {
        UILAYER_RELEASELAYER(&(idle_grid_ptr->page_cache[i]));   /*lint !e506 !e774*/
    }
    
    if (UILAYER_HANDLE_MULTI == UILAYER_GetHandleType(&idle_grid_ptr->page_layer))    
    {
        UILAYER_RELEASELAYER(&(idle_grid_ptr->page_layer));   /*lint !e506 !e774*/
    }
}

/*****************************************************************************/
//  Description : destroy thumbview layer
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note: 
/*****************************************************************************/
LOCAL void DestroyGridThumbViewLayer(MMI_IDLE_GRID_DATA_T *idle_grid_ptr)
{ 
    if (UILAYER_HANDLE_MULTI == UILAYER_GetHandleType(&idle_grid_ptr->thumb_view_layer))    
    {
        UILAYER_RELEASELAYER(&(idle_grid_ptr->thumb_view_layer));   /*lint !e506 !e774*/
    }
}

/*****************************************************************************/
//  Description : create softkey layer
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note: 
/*****************************************************************************/
LOCAL void CreateSoftkeyLayer(MMI_IDLE_GRID_DATA_T *idle_grid_ptr)
{
    GUI_LCD_DEV_INFO widget_layer_handle = {0};
    uint16 width = 0;
    uint16 height = 0;
    UILAYER_CREATE_T create_info = {0};  
    GUI_RECT_T main_rect =  idle_grid_ptr->softkey_info[MMIGRID_SOFTKEY_BG].rc  ;
    if(!MMK_IsWindowLandscape(idle_grid_ptr->win_id))
    {
        main_rect.top -= GetPageTitleBarHeight(idle_grid_ptr);
    }
    if (UILAYER_HANDLE_MULTI != UILAYER_GetHandleType(&idle_grid_ptr->softkey_layer))    
    {
        width  = main_rect.right - main_rect.left + 1;
        height = main_rect.bottom - main_rect.top + 1;
        create_info.lcd_id          = LCD_ID_0;
        create_info.owner_handle    = idle_grid_ptr->win_id;
        create_info.offset_x        = main_rect.left;
        create_info.offset_y        = main_rect.top;
        create_info.width           = width;
        create_info.height          = height;
        create_info.is_bg_layer     = FALSE;
        create_info.is_static_layer = FALSE;
        create_info.format          = UILAYER_MEM_IMMUTABLE;
        
        UILAYER_CreateLayer(&create_info, &widget_layer_handle);
            
        UILAYER_SetLayerColorKey(&widget_layer_handle, TRUE, UILAYER_TRANSPARENT_COLOR);
        idle_grid_ptr->softkey_layer = widget_layer_handle;
    }
    
    if (UILAYER_IsLayerActive(&widget_layer_handle))
    {
        UILAYER_Clear(&widget_layer_handle);
    }
}
LOCAL void CreateRecycleLayer(MMI_IDLE_GRID_DATA_T *idle_grid_ptr)
{
    GUI_LCD_DEV_INFO widget_layer_handle = {0};    
    uint16 width = 0;
    uint16 height = 0;
    UILAYER_CREATE_T create_info = {0};  
    GUI_RECT_T main_rect =  GetRecycleRect(idle_grid_ptr);
    
    if (UILAYER_HANDLE_MULTI != UILAYER_GetHandleType(&idle_grid_ptr->recycle_layer))    
    {
        width  = main_rect.right - main_rect.left + 1;
        height = main_rect.bottom - main_rect.top + 1;
        create_info.lcd_id          = LCD_ID_0;
        create_info.owner_handle    = idle_grid_ptr->win_id;
        create_info.offset_x        = main_rect.left;
        create_info.offset_y        = main_rect.top;
        create_info.width           = width;
        create_info.height          = height;
        create_info.is_bg_layer     = FALSE;
        create_info.is_static_layer = FALSE;
        create_info.format          = UILAYER_MEM_IMMUTABLE;
        
        UILAYER_CreateLayer(&create_info, &widget_layer_handle);
            
        idle_grid_ptr->recycle_layer = widget_layer_handle;
    }
    
    if (UILAYER_IsLayerActive(&widget_layer_handle))
    {
        UILAYER_Clear(&widget_layer_handle);
    }

#ifdef MMI_SAM_THEME
    if (MMIAPISET_GetIdleSubStyle() != MMISET_IDLE_SUBSTYLE_SAM)
#endif
    {    
        MMK_DestroyDynaCtrl(MMK_GetCtrlHandleByWin(idle_grid_ptr->win_id, MMITHEME_GetStatusBarCtrlId()));
}
}

LOCAL void CreateTitlebarLayer(MMI_IDLE_GRID_DATA_T *idle_grid_ptr)
{
    if(!MMK_IsWindowLandscape(idle_grid_ptr->win_id))
    {
        idle_grid_ptr->titlebar_layer = idle_grid_ptr->softkey_layer;
    }
    else
    {
        GUI_LCD_DEV_INFO widget_layer_handle = {0};
        uint16 width = 0;
        uint16 height = 0;        
        
        UILAYER_CREATE_T create_info = {0};  
        GUI_RECT_T main_rect = GetPageTitleBarRect(idle_grid_ptr);        
        
        if (UILAYER_HANDLE_MULTI != UILAYER_GetHandleType(&idle_grid_ptr->titlebar_layer))    
        {
            width  = main_rect.right - main_rect.left + 1;
            height = GetPageTitleBarHeight(idle_grid_ptr);
            create_info.lcd_id          = LCD_ID_0;
            create_info.owner_handle    = idle_grid_ptr->win_id;
            create_info.offset_x        = main_rect.left;
            create_info.offset_y        = main_rect.top;
            create_info.width           = width;
            create_info.height          = height;
            create_info.is_bg_layer     = FALSE;
            create_info.is_static_layer = FALSE;
            create_info.format          = UILAYER_MEM_FPS_ACCELERATE;
            
            UILAYER_CreateLayer(&create_info, &widget_layer_handle);
                
            UILAYER_SetLayerColorKey(&widget_layer_handle, TRUE, UILAYER_TRANSPARENT_COLOR);
            idle_grid_ptr->titlebar_layer = widget_layer_handle;
        }
        
        if (UILAYER_IsLayerActive(&widget_layer_handle))
        {
            UILAYER_Clear(&widget_layer_handle);
        }
    }
}


/*****************************************************************************/
//  Description : update all screen
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note: 
/*****************************************************************************/
PUBLIC void MMIGRID_UpdateAllScreen(void)
{
    MMI_IDLE_GRID_DATA_T *idle_grid_ptr = (MMI_IDLE_GRID_DATA_T *)MMIAPIGRID_GetGridPtr();
    DrawAll(idle_grid_ptr);
}

/*****************************************************************************/
//  Description : save tp move point
//  Global resource dependence : 
//  Author:hongbo.lan
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN SaveTpMovePoint(
                              MMI_IDLE_GRID_DATA_T *idle_grid_ptr,
                              GUI_POINT_T point
                              )
{
    if (PNULL == idle_grid_ptr)
    {
        return FALSE;
    }
    idle_grid_ptr->save_point = point;

    return TRUE;
}

/*****************************************************************************/
//  Description : handle widget tp move
//  Global resource dependence : 
//  Author:hongbo.lan
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN HandleSlidePageTpMove(
                                 MMI_IDLE_GRID_DATA_T *idle_grid_ptr,
                                 GUI_POINT_T point
                                 )
{
    GUI_LCD_DEV_INFO    lcd_dev_info = {0};
    int16 offset_x = 0;
    int16 offset_y = 0;    
    int32  dest_page_index = 0;
    MMIWIDGET_PEN_MOVE_DIRECT_T last_move_direction = 0;
    
    if (0 != idle_grid_ptr->slide_anim.slide_flag)
    {
        return FALSE;
    }
    if(!GUI_PointIsInRect( point,  idle_grid_ptr->display_rect))
    {
        return FALSE;
    } 
    MMIGRID_SetBltLayer(idle_grid_ptr);
    
    last_move_direction = idle_grid_ptr->tp_move_info.pen_move_dir;
    
    if (point.x - idle_grid_ptr->down_point.x > 0)
    {
        idle_grid_ptr->tp_move_info.pen_move_dir = MMIWIDGET_PEN_MOVE_RIGHT;
        dest_page_index = idle_grid_ptr->cur_page_index - 1;
    }
    else
    {
        idle_grid_ptr->tp_move_info.pen_move_dir = MMIWIDGET_PEN_MOVE_LEFT;  
        dest_page_index = idle_grid_ptr->cur_page_index + 1;
    }   
    
    if(last_move_direction != idle_grid_ptr->tp_move_info.pen_move_dir)
    {   
        //send lose foucs msg to page that is not to be displayed
        if(last_move_direction == MMIWIDGET_PEN_MOVE_LEFT)
        {
            MMIWIDGET_DispatchMsgToChildInOnePage(idle_grid_ptr,idle_grid_ptr->cur_page_index + 1,  MSG_LOSE_FOCUS ,0); 
        }
        else if(last_move_direction == MMIWIDGET_PEN_MOVE_RIGHT)
        {
            MMIWIDGET_DispatchMsgToChildInOnePage(idle_grid_ptr,idle_grid_ptr->cur_page_index - 1,  MSG_LOSE_FOCUS ,0); 
        }
        //send foucs msg to page that is to be displayed
        MMIWIDGET_DispatchMsgToChildInOnePage(idle_grid_ptr,dest_page_index,  MSG_GET_FOCUS ,0); 
    }

    //计算新位置
    lcd_dev_info = idle_grid_ptr->page_layer;
    GetPageLayerPosition( idle_grid_ptr, &lcd_dev_info, &offset_x, &offset_y);
    offset_x += (point.x - idle_grid_ptr->save_point.x);
    
        UILAYER_SetDirectDraw(TRUE);        
        
        SetPageLayerPosition( idle_grid_ptr, &lcd_dev_info, offset_x, offset_y);

        SetWallpaperDeltaPosition(idle_grid_ptr, (point.x - idle_grid_ptr->save_point.x), TRUE);
    
    
//    MMITHEME_UpdateRectEx();

    SaveTpMovePoint(idle_grid_ptr, point);

    return TRUE;
}

/*****************************************************************************/
//  Description : handle widget tp move
//  Global resource dependence : 
//  Author:ying.xu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN SlidePage(
                        MMI_IDLE_GRID_DATA_T *idle_grid_ptr,
                        int32 dest_page_index
                        )
{
    int32 current_page_index = idle_grid_ptr->cur_page_index;
    int32 direction = 0;
    MMIWIDGET_PEN_MOVE_DIRECT_T left_right = 0;
    uint32      page_layer_size = MMIWIDGET_GetPageLayerSize(idle_grid_ptr);    
    
    if(dest_page_index == current_page_index)
    {
        return FALSE;            
    }
    
    if(0 == (idle_grid_ptr->widget_mov_type & MMIGRID_TP_MOVE_PAGE))  /*lint !e655 */
    {
        idle_grid_ptr->widget_mov_type |= MMIGRID_TP_MOVE_PAGE;   /*lint !e655 */         
        idle_grid_ptr->current_gird = MMI_IDLE_GRID_PAGE_CELL_NUM;  
        MMIWIDGET_DispatchMsgToActiveChildWin(idle_grid_ptr, MSG_WIDGET_PAGE_SLIDE_START);
    }

    //page index增减的方向
    if((dest_page_index - current_page_index) > 0)
    {
        direction = 1;
        left_right = MMIWIDGET_PEN_MOVE_LEFT; 
    }
    else
    {
        direction = -1;
        left_right = MMIWIDGET_PEN_MOVE_RIGHT; 
    }
    
    while(current_page_index != dest_page_index)
    {
        current_page_index += direction;
        SlideOnePage(idle_grid_ptr,current_page_index,left_right,4);
        if(page_layer_size >= 3)        
        {
            ResetGridPageLayerPos( idle_grid_ptr);
            DrawPageLayer(idle_grid_ptr);
            DrawPageTitleBar(idle_grid_ptr, idle_grid_ptr->cur_page_index);         
        }
    } 
    
#ifdef MMI_IDLE_WIDGET_CACHE
        MMIWIDGET_ReleaseWidgetCacheInOnePage(idle_grid_ptr, current_page_index);
#endif        
    //end page
    UILAYER_SetLayerMemFormat(&idle_grid_ptr->page_layer, UILAYER_MEM_FPS_ACCELERATE);

    MMIWIDGET_DispatchMsgToActiveChildWin(idle_grid_ptr, MSG_WIDGET_PAGE_SLIDE_END);

    idle_grid_ptr->widget_mov_type &= ~MMIGRID_TP_MOVE_PAGE;            
    
    MMIGRID_SetCurPageIndex( idle_grid_ptr->cur_page_index);
    MMIWIDGET_ReCalcAllWidgetWinRect(idle_grid_ptr);  
    
    ResetGridPageLayerPos( idle_grid_ptr);
    DrawPageLayer(idle_grid_ptr);
    DrawPageTitleBar(idle_grid_ptr, idle_grid_ptr->cur_page_index);         
 
    return TRUE;
}

LOCAL BOOLEAN SlideOnePage(
                          MMI_IDLE_GRID_DATA_T *idle_grid_ptr,
                          int32 dest_page_index, //for now ,only support sliding for 1 page
                          MMIWIDGET_PEN_MOVE_DIRECT_T direction,
                          int32 frame
                          )
{
    int16 offset_x = 0;
    int16 offset_y = 0;
    int16 delta_x = 0;
    int32 current_page_index = idle_grid_ptr->cur_page_index;
    int32 page_slide_num = 0;
    int32 i = 0;
    int32 page_width = GetWidgetPageWidth( idle_grid_ptr);        
    uint32      page_layer_size = MMIWIDGET_GetPageLayerSize(idle_grid_ptr);    
    
    if(dest_page_index == current_page_index)
    {
        return FALSE;            
    }
    
    if (!UILAYER_IsLayerActive(&idle_grid_ptr->page_layer))
    {
        //UILAYER_ClearRect(&idle_grid_ptr->page_layer, main_rect);    
        return FALSE;
    }        
    
    if (MMIWIDGET_PEN_MOVE_LEFT == direction)   
    {
        page_slide_num = 1;  
        delta_x = -page_width/frame;     
    }
    else if (MMIWIDGET_PEN_MOVE_RIGHT == direction)   
    {
        page_slide_num = -1;        
        delta_x = page_width/frame;    
    }
    else
    {
        return FALSE;
    }   

   MMIWIDGET_DEBUG(LOG_STR_ENTER("timer = %d"), SCI_GetTickCount());
   
    //send  foucs msg to page that will be displayed
    MMIWIDGET_DispatchMsgToChildInOnePage(idle_grid_ptr,dest_page_index, 
                             MSG_GET_FOCUS ,0); 
                             
    MMIGRID_SetBltLayer(idle_grid_ptr);    
    if(1 == page_layer_size)
    {        
        for(i = 0; i < frame ; i++)
        {             
        	MMIWIDGET_DEBUG(LOG_STR("index = %d, timer = %d"), i ,  SCI_GetTickCount());
        	
            UILAYER_SetLayerMemFormat(&idle_grid_ptr->page_layer, UILAYER_MEM_FPS_ACCELERATE);
            UILAYER_SwitchDoubleMem( &idle_grid_ptr->page_layer, TRUE );
            UILAYER_SetDirectDraw(TRUE);    
            
            offset_x += delta_x;
            UILAYER_Clear(&idle_grid_ptr->page_layer);
            DrawSpecificPageEx(idle_grid_ptr, idle_grid_ptr->page_layer, 
                                current_page_index, 0, offset_x);    
            DrawSpecificPageEx(idle_grid_ptr, idle_grid_ptr->page_layer,
                                dest_page_index, page_slide_num,offset_x);  
            SetWallpaperDeltaPosition(idle_grid_ptr, delta_x, TRUE);  
            MMITHEME_UpdateRect();
#ifdef WIN32        
            SCI_SLEEP(10);
#endif    
       }
    }
    else
    {
        for(i = 0; i < frame ; i++)
        { 
        	MMIWIDGET_DEBUG(LOG_STR("index = %d, timer = %d"), i ,  SCI_GetTickCount());

            UILAYER_SetDirectDraw(TRUE);            
                
        	GetPageLayerPosition( idle_grid_ptr, &idle_grid_ptr->page_layer, &offset_x, &offset_y);      
        	offset_x += delta_x;
        	SetPageLayerPosition( idle_grid_ptr, &idle_grid_ptr->page_layer, offset_x, offset_y);
                
            SetWallpaperDeltaPosition(idle_grid_ptr, delta_x, TRUE);  
            //GUILCD_InvalidateRect( 0,  display_rect, 0 );
            MMITHEME_UpdateRectEx();
#ifdef WIN32        
            SCI_SLEEP(10);
#endif    
        }
    }    
   MMIWIDGET_DEBUG(LOG_STR("loop end time = %d"), SCI_GetTickCount());
   
#ifdef MMI_IDLE_WIDGET_CACHE
    MMIWIDGET_ReleaseWidgetCacheInOnePage(idle_grid_ptr, current_page_index);
#endif    
    //send lose foucs msg to page that will not be displayed    
    MMIWIDGET_DispatchMsgToChildInOnePage(idle_grid_ptr,current_page_index, 
                             MSG_LOSE_FOCUS ,0); 
                             
                            
    //draw new buffer
    //UILAYER_SetLayerMemFormat(&idle_grid_ptr->page_layer, UILAYER_MEM_IMMUTABLE);
    idle_grid_ptr->cur_page_index = dest_page_index;
    DrawPageTitleBar(idle_grid_ptr, idle_grid_ptr->cur_page_index);
    return TRUE;
}

/*****************************************************************************/
//  Description : redraw top layer
//  Global resource dependence : 
//  Author:hongbo.lan
//  Note: 
/*****************************************************************************/
LOCAL void DrawAll(MMI_IDLE_GRID_DATA_T *idle_grid_ptr)
{
    MMIGRID_SetBltLayer(idle_grid_ptr);    
    CopyWallpaper2MainLayer(idle_grid_ptr);  
#if defined BACKGROUND_GRID_LINE_LAYER
	DisplayEditBgLayer(idle_grid_ptr);
#endif	
    if (   MMK_IsFocusWin(idle_grid_ptr->win_id)
        && (MMIGRID_RUN_MODE_THUMBVIEW == idle_grid_ptr->run_mode))
    {
        DisplayGridPageThumbView(idle_grid_ptr);
    }
    else
    {
        DrawPageLayer(idle_grid_ptr);        
    }

    DrawPageTitleBar(idle_grid_ptr, idle_grid_ptr->cur_page_index);
    DisplaySoftkey(idle_grid_ptr);
}

/*****************************************************************************/
//  Description : handle widget tp move
//  Global resource dependence : 
//  Author:hongbo.lan
//  Note: 
/*****************************************************************************/
LOCAL void DisplaySoftkey(
                         MMI_IDLE_GRID_DATA_T *idle_grid_ptr
                         )
{
#ifdef BAR_PHONE_WIDGET 
    UpdateSoftKeyText(idle_grid_ptr);
#else
    DrawSoftKeyButton(idle_grid_ptr);
#endif    
}

/*****************************************************************************/
//  Description : callback the  move timer
//  Global resource dependence : 
//  Author:hongbo.lan
//  Note: 
/*****************************************************************************/
LOCAL void SlideAnimCallback(MMI_IDLE_GRID_DATA_T *idle_grid_ptr)
{
        int16 run_steps = idle_grid_ptr->display_theme.slidepage_run_steps;
        int16 offset_x = idle_grid_ptr->slide_anim.offset_x;
        int16 offset_y = idle_grid_ptr->slide_anim.offset_y;
        
        int16 end_offset_x = idle_grid_ptr->slide_anim.old_offset_x;
        
        MMIWIDGET_PEN_MOVE_DIRECT_T move_direct = idle_grid_ptr->slide_anim.move_direct;
         
        if (0 == idle_grid_ptr->slide_anim.slide_flag)
        {
            return;
        }
        MMIWIDGET_DEBUG(LOG_STR("timer = %d, offset_x = %d"), SCI_GetTickCount(), offset_x);
         
        if(MMIWIDGET_PEN_MOVE_LEFT == move_direct)
        {
            run_steps = -run_steps;
        }
        offset_x += run_steps;
        if (((MMIWIDGET_PEN_MOVE_LEFT == move_direct) && (offset_x >= end_offset_x ))
            ||((MMIWIDGET_PEN_MOVE_RIGHT == move_direct) && (offset_x  <= end_offset_x )))
        {   
            idle_grid_ptr->slide_anim.offset_x = offset_x;
            UILAYER_SetDirectDraw(TRUE);   
            SetPageLayerPosition( idle_grid_ptr, &idle_grid_ptr->page_layer, offset_x, offset_y);
            SetWallpaperDeltaPosition(idle_grid_ptr, run_steps, TRUE);                    
        }
        else
        {
            UILAYER_SetDirectDraw(TRUE);   
            SetPageLayerPosition( idle_grid_ptr, &idle_grid_ptr->page_layer, end_offset_x, offset_y);
            //SetWallpaperDeltaPosition(idle_grid_ptr, run_steps, TRUE);           
            MMITHEME_UpdateRect();
            
#ifdef MMI_IDLE_WIDGET_CACHE        
            MMIWIDGET_ReleaseWidgetCacheInAllPage(idle_grid_ptr);
#endif 
            UpdateGridPageLayerCache(idle_grid_ptr);

            //send lose foucs msg to page that will not be displayed    
            if(idle_grid_ptr->cur_page_index != idle_grid_ptr->slide_anim.dst_page_index)
            {
                MMIWIDGET_DispatchMsgToChildInOnePage(idle_grid_ptr,idle_grid_ptr->cur_page_index, 
                                     MSG_LOSE_FOCUS ,0); 
            }
            else
            {
                int32 page_offset = (MMIWIDGET_PEN_MOVE_LEFT == move_direct)? 1:-1;
                MMIWIDGET_DispatchMsgToChildInOnePage(idle_grid_ptr,
                              (idle_grid_ptr->cur_page_index + page_offset
                              + idle_grid_ptr->page_num) % idle_grid_ptr->page_num , 
                                     MSG_LOSE_FOCUS ,0); 
            }
            idle_grid_ptr->slide_anim.slide_flag = 0;
            idle_grid_ptr->widget_mov_type &= ~MMIGRID_TP_MOVE_PAGE; 
                        
            //end page
            UILAYER_SetLayerMemFormat(&idle_grid_ptr->page_layer, UILAYER_MEM_FPS_ACCELERATE);
            idle_grid_ptr->cur_page_index = idle_grid_ptr->slide_anim.dst_page_index ;
            
            MMIWIDGET_ReCalcAllWidgetWinRect(idle_grid_ptr);  
            MMIWIDGET_DispatchMsgToActiveChildWin(idle_grid_ptr, MSG_WIDGET_PAGE_SLIDE_END);
            ResetGridPageLayerPos(idle_grid_ptr); 
            DrawPageLayer(idle_grid_ptr);
            DrawPageTitleBar(idle_grid_ptr, idle_grid_ptr->cur_page_index);
            DisplaySoftkey(idle_grid_ptr);
            StopAutoSlideTimer();
            //CreateAutoSlideTimer(idle_grid_ptr->win_id, MMIWIDGET_AUTO_SLIDE_TIMEOUT);
        }
}

/*****************************************************************************/
//  Description : handle widget tp move with timer
//  Global resource dependence : 
//  Author:hongbo.lan
//  Note: 
/*****************************************************************************/
LOCAL void PlaySlideAnim(
                         MMI_IDLE_GRID_DATA_T *idle_grid_ptr,
                         MMIWIDGET_PEN_MOVE_DIRECT_T move_direct,
                         int32 dst_page_index,
                         int32  slide_flag          ////0:no slide 1:tp up slide; 2: direction key slide
                         )
{
    int32 src_page_index = 0;
    int16 offset_x = 0;
    int16 offset_y = 0; 

    if (0 != idle_grid_ptr->slide_anim.slide_flag)
    {
        return;
    }
    if(0 == (idle_grid_ptr->widget_mov_type & MMIGRID_TP_MOVE_PAGE)) /*lint !e655 */
    {
        idle_grid_ptr->widget_mov_type |= MMIGRID_TP_MOVE_PAGE; /*lint !e655 */           
        idle_grid_ptr->current_gird = -1;
        MMIWIDGET_DispatchMsgToActiveChildWin(idle_grid_ptr, MSG_WIDGET_PAGE_SLIDE_START);
    }                
    UILAYER_SetLayerMemFormat(&idle_grid_ptr->page_layer, UILAYER_MEM_FPS_ACCELERATE);    
    src_page_index = idle_grid_ptr->cur_page_index;
    GetPageLayerPosition( idle_grid_ptr, &idle_grid_ptr->page_layer, &offset_x, &offset_y);    
    MMIWIDGET_DEBUG(LOG_STR("timer = %d, offset_x = %d"), SCI_GetTickCount(), offset_x);

        idle_grid_ptr->slide_anim.slide_flag = slide_flag;
        idle_grid_ptr->slide_anim.offset_x = offset_x;
        idle_grid_ptr->slide_anim.offset_y = offset_y;
        idle_grid_ptr->slide_anim.page_width = GetWidgetPageWidth(idle_grid_ptr);
        
        idle_grid_ptr->slide_anim.dst_page_index = dst_page_index;
        idle_grid_ptr->slide_anim.move_direct = move_direct;

        if(dst_page_index == src_page_index)
        {
            idle_grid_ptr->slide_anim.old_offset_x = 0;
        }
        else
        {
            if(move_direct == MMIWIDGET_PEN_MOVE_LEFT )
            {            
                idle_grid_ptr->slide_anim.old_offset_x = -idle_grid_ptr->slide_anim.page_width;
            }
            else
            {
                idle_grid_ptr->slide_anim.old_offset_x = idle_grid_ptr->slide_anim.page_width;
            }
        }
        
        MMIGRID_SetBltLayer( idle_grid_ptr);
        //send  foucs msg to page that will be displayed
        MMIWIDGET_DispatchMsgToChildInOnePage(idle_grid_ptr,dst_page_index,
                                 MSG_GET_FOCUS ,0); 

}

/*****************************************************************************/
//  Description : handle widget tp move
//  Global resource dependence : 
//  Author:hongbo.lan
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN HandleSlidePageTpUp(
                                 MMI_IDLE_GRID_DATA_T *idle_grid_ptr,
                                 GUI_POINT_T point
                                 )
{
    uint16      lcd_width = 0;
    int16 offset_y = 0;
    int16 offset_x = 0;
    int32   page_offset = 0;
    int32   dest_page_index = 0;
    MMIWIDGET_PEN_MOVE_DIRECT_T move_direct = {0};

    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,PNULL);    
    GetPageLayerPosition( idle_grid_ptr, &idle_grid_ptr->page_layer, &offset_x, &offset_y);
    
    if(abs(offset_x) >= (lcd_width/4) )
    {       
        move_direct = (offset_x < 0) ? MMIWIDGET_PEN_MOVE_LEFT : MMIWIDGET_PEN_MOVE_RIGHT;
        page_offset = (offset_x < 0) ? 1 : -1;
        dest_page_index = (idle_grid_ptr->cur_page_index + page_offset 
                    + idle_grid_ptr->page_num) % idle_grid_ptr->page_num;
    }
    /*
    else if(idle_grid_ptr->cur_page_index == 0 
        && offset_x < -(lcd_width/4))        
    {
        move_direct = (offset_x < 0) ? MMIWIDGET_PEN_MOVE_LEFT : MMIWIDGET_PEN_MOVE_RIGHT;
        page_offset = (offset_x < 0) ? 1 : -1;
        dest_page_index = idle_grid_ptr->cur_page_index + page_offset;
    }
    else if(idle_grid_ptr->cur_page_index == idle_grid_ptr->page_num - 1
        && offset_x > (lcd_width/4))        
    {
        move_direct = (offset_x < 0) ? MMIWIDGET_PEN_MOVE_LEFT : MMIWIDGET_PEN_MOVE_RIGHT;
        page_offset = (offset_x < 0) ? 1 : -1;
        dest_page_index = idle_grid_ptr->cur_page_index + page_offset;
    }
    */
    else //弹回原来的位置
    {
        dest_page_index = idle_grid_ptr->cur_page_index;
        move_direct = (offset_x > 0) ? MMIWIDGET_PEN_MOVE_LEFT : MMIWIDGET_PEN_MOVE_RIGHT;
    }       

    PlaySlideAnim(idle_grid_ptr, move_direct, dest_page_index, 1);
    ResetWidgetTpState(idle_grid_ptr);

    return TRUE;
}


/*****************************************************************************/
//  Description : create widget layer
//  Global resource dependence : 
//  Author:hongbo.lan
//  Note: 
/*****************************************************************************/
LOCAL void CreatePageLayer(
                             MMI_IDLE_GRID_DATA_T *idle_grid_ptr
                             )
{
    GUI_LCD_DEV_INFO widget_layer_handle = {0};
    uint16 width = 0;
    uint16 height = 0;
    GUI_RECT_T widget_rect = {0};    
    GUI_RECT_T screen_rect = MMITHEME_GetFullScreenRect();
    UILAYER_CREATE_T create_info = {0};
    uint16      page_layer_size = (uint16)MMIWIDGET_GetPageLayerSize(idle_grid_ptr);   
    uint16 	    i = 0;

    for(i=0; i < page_layer_size;  i++)
    {
    if (UILAYER_HANDLE_MULTI != UILAYER_GetHandleType(&idle_grid_ptr->page_cache[i]))    
    {
        widget_rect = idle_grid_ptr->display_rect;
        widget_rect.bottom = screen_rect.bottom;
        
        width = (uint16)((widget_rect.right+1-widget_rect.left)); 
        height = (widget_rect.bottom+1-widget_rect.top);
        
        create_info.lcd_id = LCD_ID_0;
        create_info.owner_handle = idle_grid_ptr->win_id;
        create_info.offset_x = widget_rect.left;
        create_info.offset_y = widget_rect.top;
        create_info.width = width;
        create_info.height = height;
        create_info.is_bg_layer = FALSE;
        create_info.is_static_layer = FALSE; 
        create_info.format = UILAYER_MEM_FPS_ACCELERATE;
        
        UILAYER_CreateLayer(
            &create_info,
            &widget_layer_handle
            );
    	}
	idle_grid_ptr->page_cache[i] = widget_layer_handle;
    }

    idle_grid_ptr->page_layer = idle_grid_ptr->page_cache[0];

 
}

/*****************************************************************************/
//  Description :  set shortcut offset
//  Global resource dependence : 
//  Author:  hongbo.lan
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIGRID_SetCurPageIndex(
                                    int16 page_index
                                    )
{
    MMI_IDLE_GRID_DATA_T *idle_grid_ptr = MMIAPIGRID_GetGridPtr();

    idle_grid_ptr->cur_page_index = page_index;

    //cur_page_index变了，需要调整page_layer的位置
    ResetGridPageLayerPos(idle_grid_ptr);
    MMIGRID_SetBltLayer(idle_grid_ptr);
    
    return TRUE;
}

/*****************************************************************************/
//  Description : rotate point
//  Global resource dependence : 
//  Author:hongbo.lan
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
//  Description : get widget page width
//  Global resource dependence : 
//  Author:hongbo.lan
//  Note: 
/*****************************************************************************/
LOCAL uint16 GetWidgetPageWidth(
                               MMI_IDLE_GRID_DATA_T *idle_grid_ptr
                               )
{
    GUI_RECT_T widget_rect = {0};
    uint16 page_width = 0;

    widget_rect =  MMITHEME_GetFullScreenRect();
    page_width = (widget_rect.right + 1 - widget_rect.left);
    return page_width;
}

/*****************************************************************************/
//  Description : get grid idle softkey height
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
LOCAL uint32 GetGridIdleSoftkeyHeight(void)
{
    uint32 height = 0;

#ifdef MMI_SAM_THEME
    if (MMIAPISET_GetIdleSubStyle() == MMISET_IDLE_SUBSTYLE_SAM)
    {
        height = MMIGRID_IDLE_SAMSUNG_SOFTKEY_HEIGHT;
    }
    else
#endif	
    {
        height = MMIGRID_IDLE_SOFTKEY_HEIGHT;
    }
    return height;
}

/*****************************************************************************/
//  Description : get widget page layer rect
//  Global resource dependence : 
//  Author:hongbo.lan
//  Note: 
/*****************************************************************************/
LOCAL GUI_RECT_T GetWidgetPageRect(
                               MMI_IDLE_GRID_DATA_T *idle_grid_ptr
                               )
{
    GUI_RECT_T page_rect = MMITHEME_GetFullScreenRect();

    if(!MMK_IsWindowLandscape( idle_grid_ptr->win_id))   
    {   
#ifdef BAR_PHONE_WIDGET   
#if !defined MMIWIDGET_SPECIAL_EFFECT
        page_rect.top += MMI_STATUSBAR_HEIGHT;/*lint !e737*/
#endif        
        page_rect.bottom -= (int16)MMITHEME_GetWinSoftkeyHeight(idle_grid_ptr->win_id) + GetPageTitleBarHeight(idle_grid_ptr); 
#else        
#ifdef MMI_SAM_THEME
        if (MMIAPISET_GetIdleSubStyle() == MMISET_IDLE_SUBSTYLE_SAM)
        {
            page_rect.top    += MMI_STATUSBAR_HEIGHT;          
        }
        else
#endif
        {
        page_rect.top += MMIGRID_IDLE_RECYCLE_HEIGHT;/*lint !e737*/
        }
        page_rect.bottom -= GetGridIdleSoftkeyHeight() + GetPageTitleBarHeight(idle_grid_ptr);  /*lint !e737*/
#endif
        
    }
    else
    {
#ifdef BAR_PHONE_WIDGET    
        page_rect.top += MMI_STATUSBAR_HEIGHT;/*lint !e737*/
        page_rect.bottom -= (int16)MMITHEME_GetWinSoftkeyHeight(idle_grid_ptr->win_id) + GetPageTitleBarHeight(idle_grid_ptr);       
#else        
#ifdef MMI_SAM_THEME
        if (MMIAPISET_GetIdleSubStyle() == MMISET_IDLE_SUBSTYLE_SAM)
        {
            page_rect.top    += MMI_STATUSBAR_HEIGHT; 
            page_rect.bottom -= GetGridIdleSoftkeyHeight() + GetPageTitleBarHeight(idle_grid_ptr); /*lint !e737*/       
        }
        else
#endif        
        {
        page_rect.top += MMIGRID_IDLE_RECYCLE_HEIGHT;/*lint !e737*/
        page_rect.bottom -= GetPageTitleBarHeight(idle_grid_ptr); /*lint !e737*/      
        page_rect.right -= GetGridIdleSoftkeyHeight();/*lint !e737*/
    }
#endif        
    }

    return page_rect;
}

/*****************************************************************************/
//  Description : rotate rect
//  Global resource dependence : 
//  Author:hongbo.lan
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
//  Description : grid idle softkey count
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
LOCAL uint32 GetWidgetIdleSoftkeyCount(void)
{
    uint32 count = 0;

#ifdef MMI_SAM_THEME
    if (MMIAPISET_GetIdleSubStyle() == MMISET_IDLE_SUBSTYLE_SAM)
    {
        count = MMIWIDGET_SAM_IDLE_SOFTKEY_COUNT;
        return count;
    }
#endif

#ifdef UPB_IDLE_SUPPORT
        count = MMIWIDGET_IDLE_UPB_SOFTKEY_COUNT;
#else
    count = MMIWIDGET_IDLE_SOFTKEY_COUNT;
#endif

    return count;
}

#ifdef BAR_PHONE_WIDGET  
/*****************************************************************************/
//  Description : initial grid idle softkey.
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note:
/*****************************************************************************/
LOCAL void InitBarPhoneWidgetSoftkey(MMI_IDLE_GRID_DATA_T *idle_grid_ptr)
{
    MMI_IDLE_GRID_DATA_T *mmiwidget_ctrl = idle_grid_ptr;

    GUI_RECT_T lsoft_rc = {0};
    GUI_RECT_T rsoft_rc = {0};
    GUI_RECT_T msoft_rc = {0};
    GUI_RECT_T full_rc = MMITHEME_GetFullScreenRect();
    GUI_RECT_T soft_bg = {0};
    uint16 width = 0;
    uint16 height = 0;   
    //uint16 soft_bg_width = 0;   /*lint !e529 !e830 */
    //uint16 soft_bg_height = 0;   /*lint !e529 !e830 */
    //uint32 softkey_count = GetWidgetIdleSoftkeyCount(); //MMIWIDGET_IDLE_SOFTKEY_COUNT;
    
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&width,&height);    
    full_rc.top = full_rc.bottom - (int16)MMITHEME_GetWinSoftkeyHeight(idle_grid_ptr->win_id) + 1;
    soft_bg = full_rc;
    full_rc.top = full_rc.top + 2;
    lsoft_rc = full_rc;
    lsoft_rc.right = lsoft_rc.left + width/3;
    lsoft_rc.left = lsoft_rc.left +2;
    mmiwidget_ctrl->softkey_info[MMIGRID_SOFTKEY_LEFT].rc = lsoft_rc;
    mmiwidget_ctrl->softkey_info[MMIGRID_SOFTKEY_LEFT].normal_text_id = TXT_COMMON_MAINMENU;

    msoft_rc = lsoft_rc;
    msoft_rc.left = lsoft_rc.right + 1;
    msoft_rc.right = msoft_rc.left + width/3;
    mmiwidget_ctrl->softkey_info[MMIGRID_SOFTKEY_MIDDLE].rc = msoft_rc;
    mmiwidget_ctrl->softkey_info[MMIGRID_SOFTKEY_MIDDLE].normal_text_id = TXT_NULL;
    
    rsoft_rc = msoft_rc;
    rsoft_rc.left = msoft_rc.right + 1;
    rsoft_rc.right = full_rc.right -2;
    mmiwidget_ctrl->softkey_info[MMIGRID_SOFTKEY_RIGHT].rc = rsoft_rc;
    mmiwidget_ctrl->softkey_info[MMIGRID_SOFTKEY_RIGHT].normal_text_id = TXT_IDLE_SHORTCUT_MODE;
    
    mmiwidget_ctrl->softkey_info[MMIGRID_SOFTKEY_BG].rc = soft_bg;
}
#endif

#ifdef UPB_IDLE_SUPPORT
/*****************************************************************************/
//  Description : initial grid idle softkey.
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note:
/*****************************************************************************/
LOCAL void InitUPBSoftkey(MMI_IDLE_GRID_DATA_T *idle_grid_ptr)
{
    MMI_IDLE_GRID_DATA_T *mmiwidget_ctrl = idle_grid_ptr;

    GUI_RECT_T lsoft_rc = {0};
    GUI_RECT_T rsoft_rc = {0};
    GUI_RECT_T msoft_rc = {0};
    GUI_RECT_T full_rc = MMITHEME_GetFullScreenRect();
    GUI_RECT_T soft_bg = {0};
    uint16 width = 0;
    uint16 height = 0;   
    uint16 soft_bg_width = 0;   /*lint !e529 !e830 */
    uint16 soft_bg_height = 0;   /*lint !e529 !e830 */
    uint32 softkey_count = GetWidgetIdleSoftkeyCount(); //MMIWIDGET_IDLE_SOFTKEY_COUNT;
    
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&width,&height);    
    
    if(!MMK_IsWindowLandscape(idle_grid_ptr->win_id))
    {
        #ifdef MAINLCD_SIZE_320X480
        soft_bg = full_rc;
        soft_bg_height = MMIGRID_IDLE_SOFTKEY_HEIGHT;
        soft_bg_width = width;
        soft_bg.top = height - soft_bg_height;
        mmiwidget_ctrl->softkey_info[MMIGRID_SOFTKEY_BG].rc = soft_bg;
        //手动调节
        mmiwidget_ctrl->softkey_info[MMIGRID_SOFTKEY_BG].rc.top += 6;
        mmiwidget_ctrl->softkey_info[MMIGRID_SOFTKEY_BG].rc.bottom += 6;
        mmiwidget_ctrl->softkey_info[MMIGRID_SOFTKEY_BG].normal_img_id = IMAGE_UPB_SOFTKEY;
        mmiwidget_ctrl->softkey_info[MMIGRID_SOFTKEY_BG].pressed_img_id = 0;
        GUIRES_GetImgWidthHeight(&width,&height, IMAGE_UPB_ICON_DIAL,mmiwidget_ctrl->win_id);
        lsoft_rc.top = soft_bg.top + (soft_bg_height - height)/2;/*lint !e737*/
        lsoft_rc.left = soft_bg.left + (soft_bg_width/softkey_count - width)/2;/*lint !e737*/
        lsoft_rc.bottom = lsoft_rc.top + height;
        lsoft_rc.right = lsoft_rc.left + width;
        mmiwidget_ctrl->softkey_info[MMIGRID_SOFTKEY_LEFT].rc = lsoft_rc;
        mmiwidget_ctrl->softkey_info[MMIGRID_SOFTKEY_LEFT].normal_img_id = IMAGE_UPB_ICON_DIAL;
        mmiwidget_ctrl->softkey_info[MMIGRID_SOFTKEY_LEFT].pressed_img_id = IMAGE_UPB_ICON_DIAL;
        
        GUIRES_GetImgWidthHeight(&width,&height, IMAGE_UPB_ICON_PB,mmiwidget_ctrl->win_id);
        msoft_rc = lsoft_rc;/*lint !e737*/
        msoft_rc.left = soft_bg.left + soft_bg_width/softkey_count + (soft_bg_width/softkey_count - width)/2;/*lint !e737*/
        msoft_rc.right = msoft_rc.left + width;/*lint !e737*/
        mmiwidget_ctrl->softkey_info[MMIGRID_SOFTKEY_RIGHT].rc = msoft_rc;
        mmiwidget_ctrl->softkey_info[MMIGRID_SOFTKEY_RIGHT].normal_img_id = IMAGE_UPB_ICON_PB;
        mmiwidget_ctrl->softkey_info[MMIGRID_SOFTKEY_RIGHT].pressed_img_id = IMAGE_UPB_ICON_PB;
        

        GUIRES_GetImgWidthHeight(&width,&height, IMAGE_UPB_ICON_MESSAGE,mmiwidget_ctrl->win_id);
        rsoft_rc = msoft_rc;/*lint !e737*/
        rsoft_rc.left = soft_bg.left + soft_bg_width*3/softkey_count + (soft_bg_width/softkey_count - width)/2;/*lint !e737*/
        rsoft_rc.right = rsoft_rc.left + width;/*lint !e737*/
        mmiwidget_ctrl->softkey_info[MMIGRID_SOFTKEY_MIDDLE].rc = rsoft_rc;
        mmiwidget_ctrl->softkey_info[MMIGRID_SOFTKEY_MIDDLE].normal_img_id = IMAGE_UPB_ICON_MESSAGE;
        mmiwidget_ctrl->softkey_info[MMIGRID_SOFTKEY_MIDDLE].pressed_img_id = IMAGE_UPB_ICON_MESSAGE;
        
        
        if(softkey_count == 5)      /*lint !e506 !e774  !e831 */
        {
            GUIRES_GetImgWidthHeight(&width,&height, IMAGE_UPB_ICON_MAINMENU,mmiwidget_ctrl->win_id);
            rsoft_rc = msoft_rc;/*lint !e737*/
            rsoft_rc.left = soft_bg.left + soft_bg_width*4/softkey_count + (soft_bg_width/softkey_count - width)/2;/*lint !e737*/
            rsoft_rc.right = rsoft_rc.left + width;/*lint !e737*/
            mmiwidget_ctrl->softkey_info[MMIGRID_SOFTKEY_ADD_1].rc = rsoft_rc;
            mmiwidget_ctrl->softkey_info[MMIGRID_SOFTKEY_ADD_1].normal_img_id = IMAGE_UPB_ICON_MAINMENU;
            mmiwidget_ctrl->softkey_info[MMIGRID_SOFTKEY_ADD_1].pressed_img_id = IMAGE_UPB_ICON_MAINMENU;

            GUIRES_GetImgWidthHeight(&width,&height, IMAGE_UPB_ICON_ENTER,mmiwidget_ctrl->win_id);
            rsoft_rc = msoft_rc;/*lint !e737*/
            rsoft_rc.left = soft_bg.left + soft_bg_width*2/softkey_count + (soft_bg_width/softkey_count - width)/2; /*lint !e737*/
            rsoft_rc.right = rsoft_rc.left + width;
            mmiwidget_ctrl->softkey_info[MMIGRID_SOFTKEY_ADD_2].rc = rsoft_rc;
            mmiwidget_ctrl->softkey_info[MMIGRID_SOFTKEY_ADD_2].normal_img_id = IMAGE_UPB_ICON_ENTER;
            mmiwidget_ctrl->softkey_info[MMIGRID_SOFTKEY_ADD_2].pressed_img_id = IMAGE_UPB_ICON_ENTER_PRESSED;
            
        }
        #else
        soft_bg = full_rc;
        soft_bg_height = MMIGRID_IDLE_SOFTKEY_HEIGHT;
        soft_bg_width = width;
        soft_bg.top = height - soft_bg_height;
        mmiwidget_ctrl->softkey_info[MMIGRID_SOFTKEY_BG].rc = soft_bg;
        //手动调节
        mmiwidget_ctrl->softkey_info[MMIGRID_SOFTKEY_BG].normal_img_id = IMAGE_UPB_SOFTKEY;
        mmiwidget_ctrl->softkey_info[MMIGRID_SOFTKEY_BG].pressed_img_id = 0;
        GUIRES_GetImgWidthHeight(&width,&height, IMAGE_MAINMENU_ICON_DIAL,mmiwidget_ctrl->win_id);
        lsoft_rc.top = soft_bg.top + (soft_bg_height - height)/2;
        lsoft_rc.left = soft_bg.left + (soft_bg_width/softkey_count - width)/2;
        lsoft_rc.bottom = lsoft_rc.top + height;
        lsoft_rc.right = lsoft_rc.left + width;
        mmiwidget_ctrl->softkey_info[MMIGRID_SOFTKEY_LEFT].rc = lsoft_rc;
        mmiwidget_ctrl->softkey_info[MMIGRID_SOFTKEY_LEFT].normal_img_id = IMAGE_MAINMENU_ICON_DIAL;
        mmiwidget_ctrl->softkey_info[MMIGRID_SOFTKEY_LEFT].pressed_img_id = IMAGE_MAINMENU_ICON_DIAL;

        GUIRES_GetImgWidthHeight(&width,&height, IMAGE_UPB_ICON_ENTER,mmiwidget_ctrl->win_id);
        rsoft_rc = lsoft_rc;
        rsoft_rc.left = soft_bg.left + soft_bg_width/softkey_count + (soft_bg_width/softkey_count - width)/2;
        rsoft_rc.right = rsoft_rc.left + width;
        mmiwidget_ctrl->softkey_info[MMIGRID_SOFTKEY_MIDDLE].rc = rsoft_rc;
        mmiwidget_ctrl->softkey_info[MMIGRID_SOFTKEY_MIDDLE].normal_img_id = IMAGE_UPB_ICON_ENTER;
        mmiwidget_ctrl->softkey_info[MMIGRID_SOFTKEY_MIDDLE].pressed_img_id = IMAGE_UPB_ICON_ENTER_PRESSED;

        GUIRES_GetImgWidthHeight(&width,&height, IMAGE_IDLE_SOFTKEY_MENU,mmiwidget_ctrl->win_id);
        msoft_rc = lsoft_rc;
        msoft_rc.left = soft_bg.left + soft_bg_width*2/softkey_count + (soft_bg_width/softkey_count - width)/2;
        msoft_rc.right = msoft_rc.left + width;
        mmiwidget_ctrl->softkey_info[MMIGRID_SOFTKEY_RIGHT].rc = msoft_rc;
        mmiwidget_ctrl->softkey_info[MMIGRID_SOFTKEY_RIGHT].normal_img_id = IMAGE_IDLE_SOFTKEY_MENU;
        mmiwidget_ctrl->softkey_info[MMIGRID_SOFTKEY_RIGHT].pressed_img_id = IMAGE_IDLE_SOFTKEY_MENU;
        

        #endif
        SetDisplayRect(mmiwidget_ctrl->softkey_info[MMIGRID_SOFTKEY_LEFT].rc,MMIGRID_SOFTKEY_LEFT);
        SetDisplayRect(mmiwidget_ctrl->softkey_info[MMIGRID_SOFTKEY_MIDDLE].rc,MMIGRID_SOFTKEY_MIDDLE);
        SetDisplayRect(mmiwidget_ctrl->softkey_info[MMIGRID_SOFTKEY_RIGHT].rc,MMIGRID_SOFTKEY_RIGHT);
        SetDisplayRect(mmiwidget_ctrl->softkey_info[MMIGRID_SOFTKEY_ADD_1].rc,MMIGRID_SOFTKEY_ADD_1);
        SetDisplayRect(mmiwidget_ctrl->softkey_info[MMIGRID_SOFTKEY_ADD_2].rc,MMIGRID_SOFTKEY_ADD_2);
        
    }
    else
    {
        InitAndroidSoftkey(idle_grid_ptr);
    }
}
#endif

/*****************************************************************************/
//  Description : initial grid idle softkey.
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note:
/*****************************************************************************/
LOCAL void InitAndroidSoftkey(MMI_IDLE_GRID_DATA_T *idle_grid_ptr)
{
    MMI_IDLE_GRID_DATA_T *mmiwidget_ctrl = idle_grid_ptr;

    GUI_RECT_T lsoft_rc = {0};
    GUI_RECT_T rsoft_rc = {0};
    GUI_RECT_T msoft_rc = {0};
    GUI_RECT_T full_rc = MMITHEME_GetFullScreenRect();
    GUI_RECT_T soft_bg = {0};
    uint16 width = 0;
    uint16 height = 0;   
    uint16 soft_bg_width = 0;   /*lint !e529 !e830 */
    uint16 soft_bg_height = 0;   /*lint !e529 !e830 */
    uint32 softkey_count = GetWidgetIdleSoftkeyCount(); //MMIWIDGET_IDLE_SOFTKEY_COUNT;
    
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&width,&height);    
 
    if(!MMK_IsWindowLandscape(idle_grid_ptr->win_id))
    {
        soft_bg = full_rc;
        soft_bg_height = MMIGRID_IDLE_SOFTKEY_HEIGHT;
        soft_bg_width = width;
        soft_bg.top = height - soft_bg_height;
        mmiwidget_ctrl->softkey_info[MMIGRID_SOFTKEY_BG].rc = soft_bg;
        mmiwidget_ctrl->softkey_info[MMIGRID_SOFTKEY_BG].normal_img_id = IMAGE_IDLE_SOFTKEY_BG;
        mmiwidget_ctrl->softkey_info[MMIGRID_SOFTKEY_BG].pressed_img_id = 0;

        GUIRES_GetImgWidthHeight(&width,&height, IMAGE_MAINMENU_ICON_DIAL,mmiwidget_ctrl->win_id);
        lsoft_rc.top = soft_bg.top + (soft_bg_height - height)/2;
        lsoft_rc.left = soft_bg.left + (soft_bg_width/softkey_count - width)/2;/*lint !e737*/
        lsoft_rc.bottom = lsoft_rc.top + height;
        lsoft_rc.right = lsoft_rc.left + width;
        mmiwidget_ctrl->softkey_info[MMIGRID_SOFTKEY_LEFT].rc = lsoft_rc;
        mmiwidget_ctrl->softkey_info[MMIGRID_SOFTKEY_LEFT].normal_img_id = IMAGE_MAINMENU_ICON_DIAL;
        mmiwidget_ctrl->softkey_info[MMIGRID_SOFTKEY_LEFT].pressed_img_id = IMAGE_MAINMENU_ICON_DIAL;
        
        GUIRES_GetImgWidthHeight(&width,&height, IMAGE_MAINMENU_ICON_MESSAGE,mmiwidget_ctrl->win_id);
        msoft_rc = lsoft_rc;
        msoft_rc.left = soft_bg.left + soft_bg_width/softkey_count + (soft_bg_width/softkey_count - width)/2;/*lint !e737*/
        msoft_rc.right = msoft_rc.left + width;
        mmiwidget_ctrl->softkey_info[MMIGRID_SOFTKEY_MIDDLE].rc = msoft_rc;
        mmiwidget_ctrl->softkey_info[MMIGRID_SOFTKEY_MIDDLE].normal_img_id = IMAGE_MAINMENU_ICON_MESSAGE;
        mmiwidget_ctrl->softkey_info[MMIGRID_SOFTKEY_MIDDLE].pressed_img_id = IMAGE_MAINMENU_ICON_MESSAGE;

        if(softkey_count == 3)
        {
            GUIRES_GetImgWidthHeight(&width,&height, IMAGE_IDLE_SOFTKEY_MENU,mmiwidget_ctrl->win_id);
            rsoft_rc = msoft_rc;
            rsoft_rc.left = soft_bg.left + soft_bg_width*2/softkey_count + (soft_bg_width/softkey_count - width)/2;/*lint !e737*/
            rsoft_rc.right = rsoft_rc.left + width;
            mmiwidget_ctrl->softkey_info[MMIGRID_SOFTKEY_RIGHT].rc = rsoft_rc;
            mmiwidget_ctrl->softkey_info[MMIGRID_SOFTKEY_RIGHT].normal_img_id = IMAGE_IDLE_SOFTKEY_MENU;
            mmiwidget_ctrl->softkey_info[MMIGRID_SOFTKEY_RIGHT].pressed_img_id = IMAGE_IDLE_SOFTKEY_MENU;
        }
        else      /*lint !e506 !e774  !e831 */
        {
            
            GUIRES_GetImgWidthHeight(&width,&height, IMAGE_MAINMENU_ICON_PB,mmiwidget_ctrl->win_id);
            rsoft_rc = msoft_rc;
            rsoft_rc.left = soft_bg.left + soft_bg_width*2/softkey_count + (soft_bg_width/softkey_count - width)/2;/*lint !e737*/
            rsoft_rc.right = rsoft_rc.left + width;
            mmiwidget_ctrl->softkey_info[MMIGRID_SOFTKEY_RIGHT].rc = rsoft_rc;
            mmiwidget_ctrl->softkey_info[MMIGRID_SOFTKEY_RIGHT].normal_img_id = IMAGE_MAINMENU_ICON_PB;
            mmiwidget_ctrl->softkey_info[MMIGRID_SOFTKEY_RIGHT].pressed_img_id = IMAGE_MAINMENU_ICON_PB;

            GUIRES_GetImgWidthHeight(&width,&height, IMAGE_MAINMENU_ICON_PB,mmiwidget_ctrl->win_id);
            rsoft_rc = msoft_rc;
            rsoft_rc.left = soft_bg.left + soft_bg_width*3/softkey_count + (soft_bg_width/softkey_count - width)/2;/*lint !e737*/
            rsoft_rc.right = rsoft_rc.left + width;
            mmiwidget_ctrl->softkey_info[MMIGRID_SOFTKEY_ADD_1].rc = rsoft_rc;
            mmiwidget_ctrl->softkey_info[MMIGRID_SOFTKEY_ADD_1].normal_img_id = IMAGE_IDLE_SOFTKEY_MENU;
            mmiwidget_ctrl->softkey_info[MMIGRID_SOFTKEY_ADD_1].pressed_img_id = IMAGE_IDLE_SOFTKEY_MENU;
        }
    }
    else
    {
        soft_bg = full_rc;        
        soft_bg_height = height -2*MMITHEME_GetStatusBarHeight();     
        soft_bg_width = MMIGRID_IDLE_SOFTKEY_HEIGHT;
        
        soft_bg.left = width - soft_bg_width;
        soft_bg.top += (int16)MMITHEME_GetStatusBarHeight();
        soft_bg.bottom -= (int16)MMITHEME_GetStatusBarHeight();       
        
        mmiwidget_ctrl->softkey_info[MMIGRID_SOFTKEY_BG].rc = soft_bg;
        mmiwidget_ctrl->softkey_info[MMIGRID_SOFTKEY_BG].normal_img_id = IMAGE_IDLE_SOFTKEY_BG;//;
        mmiwidget_ctrl->softkey_info[MMIGRID_SOFTKEY_BG].pressed_img_id = 0;

        GUIRES_GetImgWidthHeight(&width,&height, IMAGE_MAINMENU_ICON_DIAL,mmiwidget_ctrl->win_id);
        lsoft_rc = soft_bg;
        lsoft_rc.top = soft_bg.top + soft_bg_height*(0)/softkey_count + (soft_bg_height/softkey_count - height)/2;/*lint !e737*/
        lsoft_rc.left = soft_bg.left + (soft_bg_width - width)/2;
        lsoft_rc.bottom = lsoft_rc.top + height;
        mmiwidget_ctrl->softkey_info[MMIGRID_SOFTKEY_LEFT].rc = lsoft_rc;
        mmiwidget_ctrl->softkey_info[MMIGRID_SOFTKEY_LEFT].normal_img_id = IMAGE_MAINMENU_ICON_DIAL;
        mmiwidget_ctrl->softkey_info[MMIGRID_SOFTKEY_LEFT].pressed_img_id = IMAGE_MAINMENU_ICON_DIAL;
        
        GUIRES_GetImgWidthHeight(&width,&height, IMAGE_MAINMENU_ICON_MESSAGE,mmiwidget_ctrl->win_id);
        msoft_rc = soft_bg;
        msoft_rc.top = soft_bg.top + soft_bg_height*(1)/softkey_count + (soft_bg_height/softkey_count - height)/2;/*lint !e737*/
        msoft_rc.bottom= msoft_rc.top + height;        
        msoft_rc.left = soft_bg.left + (soft_bg_width - width)/2;
        mmiwidget_ctrl->softkey_info[MMIGRID_SOFTKEY_MIDDLE].rc = msoft_rc;
        mmiwidget_ctrl->softkey_info[MMIGRID_SOFTKEY_MIDDLE].normal_img_id = IMAGE_MAINMENU_ICON_MESSAGE;
        mmiwidget_ctrl->softkey_info[MMIGRID_SOFTKEY_MIDDLE].pressed_img_id = IMAGE_MAINMENU_ICON_MESSAGE;


        if(softkey_count == 3)    /*lint !e506 !e774  !e831 */
        {
            GUIRES_GetImgWidthHeight(&width,&height, IMAGE_IDLE_SOFTKEY_MENU,mmiwidget_ctrl->win_id);
            rsoft_rc = soft_bg;        
            rsoft_rc.top = soft_bg.top  + soft_bg_height*(2)/softkey_count + (soft_bg_height/softkey_count - height)/2;/*lint !e737*/
            rsoft_rc.bottom= rsoft_rc.top + height;        
            rsoft_rc.left = soft_bg.left + (soft_bg_width - width)/2;
            mmiwidget_ctrl->softkey_info[MMIGRID_SOFTKEY_RIGHT].rc = rsoft_rc;
            mmiwidget_ctrl->softkey_info[MMIGRID_SOFTKEY_RIGHT].normal_img_id = IMAGE_IDLE_SOFTKEY_MENU;
            mmiwidget_ctrl->softkey_info[MMIGRID_SOFTKEY_RIGHT].pressed_img_id = IMAGE_IDLE_SOFTKEY_MENU;
        }
        else
        {
            
            GUIRES_GetImgWidthHeight(&width,&height, IMAGE_MAINMENU_ICON_PB,mmiwidget_ctrl->win_id);
            rsoft_rc = soft_bg;        
            rsoft_rc.top = soft_bg.top  + soft_bg_height*(2)/softkey_count + (soft_bg_height/softkey_count - height)/2;/*lint !e737*/
            rsoft_rc.bottom= rsoft_rc.top + height;        
            rsoft_rc.left = soft_bg.left + (soft_bg_width - width)/2;
            mmiwidget_ctrl->softkey_info[MMIGRID_SOFTKEY_RIGHT].rc = rsoft_rc;
            mmiwidget_ctrl->softkey_info[MMIGRID_SOFTKEY_RIGHT].normal_img_id = IMAGE_MAINMENU_ICON_PB;
            mmiwidget_ctrl->softkey_info[MMIGRID_SOFTKEY_RIGHT].pressed_img_id = IMAGE_MAINMENU_ICON_PB;

            GUIRES_GetImgWidthHeight(&width,&height, IMAGE_IDLE_SOFTKEY_MENU,mmiwidget_ctrl->win_id);
            rsoft_rc = soft_bg;
            rsoft_rc.top = soft_bg.top  + soft_bg_height*(3)/softkey_count + (soft_bg_height/softkey_count - height)/2;/*lint !e737*/
            rsoft_rc.bottom= rsoft_rc.top + height;
            rsoft_rc.left = soft_bg.left + (soft_bg_width - width)/2;
            mmiwidget_ctrl->softkey_info[MMIGRID_SOFTKEY_ADD_1].rc = rsoft_rc;
            mmiwidget_ctrl->softkey_info[MMIGRID_SOFTKEY_ADD_1].normal_img_id = IMAGE_IDLE_SOFTKEY_MENU;
            mmiwidget_ctrl->softkey_info[MMIGRID_SOFTKEY_ADD_1].pressed_img_id = IMAGE_IDLE_SOFTKEY_MENU;
        }
    }
    SetDisplayRect(mmiwidget_ctrl->softkey_info[MMIGRID_SOFTKEY_LEFT].rc,MMIGRID_SOFTKEY_LEFT);
    SetDisplayRect(mmiwidget_ctrl->softkey_info[MMIGRID_SOFTKEY_MIDDLE].rc,MMIGRID_SOFTKEY_MIDDLE);
    SetDisplayRect(mmiwidget_ctrl->softkey_info[MMIGRID_SOFTKEY_RIGHT].rc,MMIGRID_SOFTKEY_RIGHT);
    SetDisplayRect(mmiwidget_ctrl->softkey_info[MMIGRID_SOFTKEY_ADD_1].rc,MMIGRID_SOFTKEY_ADD_1);
}

#ifdef MMI_SAM_THEME
/*****************************************************************************/
//  Description : initial grid idle softkey.
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note:
/*****************************************************************************/
LOCAL void InitSamsungSoftkey(MMI_IDLE_GRID_DATA_T *idle_grid_ptr)
{
    MMI_IDLE_GRID_DATA_T *mmiwidget_ctrl = idle_grid_ptr;

    GUI_RECT_T lsoft_rc = {0};
    GUI_RECT_T rsoft_rc = {0};
    GUI_RECT_T msoft_rc = {0};
    GUI_RECT_T full_rc = MMITHEME_GetFullScreenRect();
    GUI_RECT_T soft_bg = {0};
    uint16 width = 0;
    uint16 height = 0;   
    uint16 soft_bg_width = 0;   /*lint !e529 !e830 */
    uint16 soft_bg_height = 0;   /*lint !e529 !e830 */
    uint32 softkey_count = GetWidgetIdleSoftkeyCount(); //MMIWIDGET_IDLE_SOFTKEY_COUNT;
    
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&width,&height);    
    
    if(!MMK_IsWindowLandscape(idle_grid_ptr->win_id))
    {
        soft_bg = full_rc;
        soft_bg_height = GetGridIdleSoftkeyHeight();//MMIGRID_IDLE_SOFTKEY_HEIGHT;
        soft_bg_width = width;
        soft_bg.top = height - soft_bg_height;
        mmiwidget_ctrl->softkey_info[MMIGRID_SOFTKEY_BG].rc = soft_bg;
        mmiwidget_ctrl->softkey_info[MMIGRID_SOFTKEY_BG].normal_img_id = IMAGE_IDLE_SOFTKEY_BG;
        mmiwidget_ctrl->softkey_info[MMIGRID_SOFTKEY_BG].pressed_img_id = 0;
        
        GUIRES_GetImgWidthHeight(&width,&height, IMAGE_MAINMENU_ICON_DIAL,mmiwidget_ctrl->win_id);
 
            lsoft_rc.top = soft_bg.top;
        lsoft_rc.left = soft_bg.left + (soft_bg_width/softkey_count - width)/2;/*lint !e737*/
        lsoft_rc.bottom = lsoft_rc.top + height;
        lsoft_rc.right = lsoft_rc.left + width;
        mmiwidget_ctrl->softkey_info[MMIGRID_SOFTKEY_LEFT].rc = lsoft_rc;
        mmiwidget_ctrl->softkey_info[MMIGRID_SOFTKEY_LEFT].normal_img_id = IMAGE_MAINMENU_ICON_DIAL;
        mmiwidget_ctrl->softkey_info[MMIGRID_SOFTKEY_LEFT].pressed_img_id = IMAGE_MAINMENU_ICON_DIAL;
        mmiwidget_ctrl->softkey_info[MMIGRID_SOFTKEY_LEFT].normal_text_id = TXT_COMM_DIALPAD;
        
        GUIRES_GetImgWidthHeight(&width,&height, IMAGE_MAINMENU_ICON_PB,mmiwidget_ctrl->win_id);
        msoft_rc = lsoft_rc;
        msoft_rc.left = soft_bg.left + soft_bg_width/softkey_count + (soft_bg_width/softkey_count - width)/2;/*lint !e737*/
        msoft_rc.right = msoft_rc.left + width;
        mmiwidget_ctrl->softkey_info[MMIGRID_SOFTKEY_MIDDLE].rc = msoft_rc;
        mmiwidget_ctrl->softkey_info[MMIGRID_SOFTKEY_MIDDLE].normal_img_id =  IMAGE_MAINMENU_ICON_PB;
        mmiwidget_ctrl->softkey_info[MMIGRID_SOFTKEY_MIDDLE].pressed_img_id = IMAGE_MAINMENU_ICON_PB;
        mmiwidget_ctrl->softkey_info[MMIGRID_SOFTKEY_MIDDLE].normal_text_id = STXT_MAIN_PB;
        
        GUIRES_GetImgWidthHeight(&width,&height, IMAGE_MAINMENU_ICON_MESSAGE,mmiwidget_ctrl->win_id);
        rsoft_rc = msoft_rc;/*lint !e737*/
        rsoft_rc.left = soft_bg.left + soft_bg_width*2/softkey_count + (soft_bg_width/softkey_count - width)/2;/*lint !e737*/
        rsoft_rc.right = rsoft_rc.left + width;
        mmiwidget_ctrl->softkey_info[MMIGRID_SOFTKEY_RIGHT].rc = rsoft_rc;
        mmiwidget_ctrl->softkey_info[MMIGRID_SOFTKEY_RIGHT].normal_img_id = IMAGE_MAINMENU_ICON_MESSAGE;
        mmiwidget_ctrl->softkey_info[MMIGRID_SOFTKEY_RIGHT].pressed_img_id = IMAGE_MAINMENU_ICON_MESSAGE;
        mmiwidget_ctrl->softkey_info[MMIGRID_SOFTKEY_RIGHT].normal_text_id = TXT_COMMUNICATION_MESSAGE;
        
        if(softkey_count == 4)      /*lint !e506 !e774  !e831 */
        {
            GUIRES_GetImgWidthHeight(&width,&height, IMAGE_IDLE_ICON_APP,mmiwidget_ctrl->win_id);
            rsoft_rc = msoft_rc;/*lint !e737*/
            rsoft_rc.left = soft_bg.left + soft_bg_width*3/softkey_count + (soft_bg_width/softkey_count - width)/2;/*lint !e737*/
            rsoft_rc.right = rsoft_rc.left + width;
            mmiwidget_ctrl->softkey_info[MMIGRID_SOFTKEY_ADD_1].rc = rsoft_rc;
            mmiwidget_ctrl->softkey_info[MMIGRID_SOFTKEY_ADD_1].normal_img_id = IMAGE_IDLE_ICON_APP;
            mmiwidget_ctrl->softkey_info[MMIGRID_SOFTKEY_ADD_1].pressed_img_id = IMAGE_IDLE_ICON_APP;
            mmiwidget_ctrl->softkey_info[MMIGRID_SOFTKEY_ADD_1].normal_text_id = TXT_APPLICATION;
        }
    }
    else
    {
        soft_bg = full_rc;        
        soft_bg_height = height -2*MMITHEME_GetStatusBarHeight();     
        soft_bg_width = MMIGRID_IDLE_SOFTKEY_HEIGHT;
        
        soft_bg.left = width - soft_bg_width;
        soft_bg.top += (int16)MMITHEME_GetStatusBarHeight();
        soft_bg.bottom -= (int16)MMITHEME_GetStatusBarHeight();       
        
        mmiwidget_ctrl->softkey_info[MMIGRID_SOFTKEY_BG].rc = soft_bg;
        mmiwidget_ctrl->softkey_info[MMIGRID_SOFTKEY_BG].normal_img_id = IMAGE_IDLE_SOFTKEY_BG;//;
        mmiwidget_ctrl->softkey_info[MMIGRID_SOFTKEY_BG].pressed_img_id = 0;

        GUIRES_GetImgWidthHeight(&width,&height, IMAGE_MAINMENU_ICON_DIAL,mmiwidget_ctrl->win_id);
        lsoft_rc = soft_bg;
        lsoft_rc.top = soft_bg.top + soft_bg_height*(0)/softkey_count + (soft_bg_height/softkey_count - height)/2;/*lint !e737*/
        lsoft_rc.left = soft_bg.left + (soft_bg_width - width)/2;
        lsoft_rc.bottom = lsoft_rc.top + height;
        mmiwidget_ctrl->softkey_info[MMIGRID_SOFTKEY_LEFT].rc = lsoft_rc;
        mmiwidget_ctrl->softkey_info[MMIGRID_SOFTKEY_LEFT].normal_img_id = IMAGE_MAINMENU_ICON_DIAL;
        mmiwidget_ctrl->softkey_info[MMIGRID_SOFTKEY_LEFT].pressed_img_id = 0;
        
        GUIRES_GetImgWidthHeight(&width,&height, IMAGE_IDLE_SOFTKEY_MENU,mmiwidget_ctrl->win_id);
        msoft_rc = soft_bg;
        msoft_rc.top = soft_bg.top + soft_bg_height*(1)/softkey_count + (soft_bg_height/softkey_count - height)/2;/*lint !e737*/
        msoft_rc.bottom= msoft_rc.top + height;        
        msoft_rc.left = soft_bg.left + (soft_bg_width - width)/2;
        mmiwidget_ctrl->softkey_info[MMIGRID_SOFTKEY_MIDDLE].rc = msoft_rc;
        mmiwidget_ctrl->softkey_info[MMIGRID_SOFTKEY_MIDDLE].normal_img_id = IMAGE_IDLE_SOFTKEY_MENU;
        mmiwidget_ctrl->softkey_info[MMIGRID_SOFTKEY_MIDDLE].pressed_img_id = 0;
        
        GUIRES_GetImgWidthHeight(&width,&height, IMAGE_MAINMENU_ICON_MESSAGE,mmiwidget_ctrl->win_id);
        rsoft_rc = soft_bg;        
        rsoft_rc.top = soft_bg.top  + soft_bg_height*(2)/softkey_count + (soft_bg_height/softkey_count - height)/2;/*lint !e737*/
        rsoft_rc.bottom= rsoft_rc.top + height;        
        rsoft_rc.left = soft_bg.left + (soft_bg_width - width)/2;
        mmiwidget_ctrl->softkey_info[MMIGRID_SOFTKEY_RIGHT].rc = rsoft_rc;
        mmiwidget_ctrl->softkey_info[MMIGRID_SOFTKEY_RIGHT].normal_img_id = IMAGE_MAINMENU_ICON_MESSAGE;
        mmiwidget_ctrl->softkey_info[MMIGRID_SOFTKEY_RIGHT].pressed_img_id = 0;
        
        if(softkey_count == 4)    /*lint !e506 !e774  !e831 */
        {
            GUIRES_GetImgWidthHeight(&width,&height, IMAGE_MAINMENU_ICON_PB,mmiwidget_ctrl->win_id);
            rsoft_rc = soft_bg;
            rsoft_rc.top = soft_bg.top  + soft_bg_height*(3)/softkey_count + (soft_bg_height/softkey_count - height)/2;/*lint !e737*/
            rsoft_rc.bottom= rsoft_rc.top + height;
            rsoft_rc.left = soft_bg.left + (soft_bg_width - width)/2;
            mmiwidget_ctrl->softkey_info[MMIGRID_SOFTKEY_ADD_1].rc = rsoft_rc;
            mmiwidget_ctrl->softkey_info[MMIGRID_SOFTKEY_ADD_1].normal_img_id = IMAGE_MAINMENU_ICON_PB;
            mmiwidget_ctrl->softkey_info[MMIGRID_SOFTKEY_ADD_1].pressed_img_id = 0;
    }
    }
        SetDisplayRect(mmiwidget_ctrl->softkey_info[MMIGRID_SOFTKEY_LEFT].rc,MMIGRID_SOFTKEY_LEFT);
        SetDisplayRect(mmiwidget_ctrl->softkey_info[MMIGRID_SOFTKEY_MIDDLE].rc,MMIGRID_SOFTKEY_MIDDLE);
        SetDisplayRect(mmiwidget_ctrl->softkey_info[MMIGRID_SOFTKEY_RIGHT].rc,MMIGRID_SOFTKEY_RIGHT);
        SetDisplayRect(mmiwidget_ctrl->softkey_info[MMIGRID_SOFTKEY_ADD_1].rc,MMIGRID_SOFTKEY_ADD_1);
}
#endif

/*****************************************************************************/
//  Description : initial grid idle softkey.
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note:
/*****************************************************************************/
LOCAL void InitSoftkey(MMI_IDLE_GRID_DATA_T *idle_grid_ptr)
{   
#ifdef BAR_PHONE_WIDGET     
    InitBarPhoneWidgetSoftkey(idle_grid_ptr);
#else
#ifdef MMI_SAM_THEME
    if (MMIAPISET_GetIdleSubStyle() == MMISET_IDLE_SUBSTYLE_SAM)
    {
        InitSamsungSoftkey(idle_grid_ptr);
    }
    else
#endif	
    {
#ifdef UPB_IDLE_SUPPORT
        InitUPBSoftkey(idle_grid_ptr);
#else
        InitAndroidSoftkey(idle_grid_ptr);
#endif
    }
#endif
}

/*****************************************************************************/
//  Description : get menu icon number
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL uint32 GetMenuIconNumber(  
                               MMI_IMAGE_ID_T image_id
                               )
{
    uint32  count_num = 0; 

    //get number of item
    if( (IMAGE_MAINMENU_ICON_DIAL == image_id)
#ifdef UPB_IDLE_SUPPORT
#ifdef MAINLCD_SIZE_320X480
	||(IMAGE_UPB_ICON_DIAL== image_id)
#endif	
#endif
	 )
    {
        count_num = MMIAPICC_GetMissedCallNumber();
    }
    else if( (IMAGE_MAINMENU_ICON_MESSAGE == image_id)
#ifdef UPB_IDLE_SUPPORT	
#ifdef MAINLCD_SIZE_320X480
	||(IMAGE_UPB_ICON_MESSAGE== image_id) 
#endif	
#endif
	)
    {
        count_num = MMIAPISMS_GetAllUnreadMsgCount()
            + MMIAPISMS_GetUnDownloadMsgCount(MN_DUAL_SYS_MAX)
            ;
    }

    return count_num;
}

/*****************************************************************************/
//  Description : display number icon in menu
//  Global resource dependence : 
//  Author: xinhe.yan
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN DrawMenuNumberIcon(  
                                 MMI_WIN_ID_T        win_handle,
                                 uint32              count_num,
                                 GUI_RECT_T          disp_rect,  //number icon rect
                                 GUI_LCD_DEV_INFO    lcd_dev_info //icon layer handle
                                 )
{ 
    char    count_str[10] = {0};
    wchar   count_wstr[10]  = {0}; 
    uint32  str_len = 0;
    MMI_STRING_T    text_str  = {0};
    GUISTR_STYLE_T  text_style = {0}; 
    GUISTR_STATE_T  text_state = GUISTR_STATE_ALIGN;

    if( 0 == count_num )
    {
        return FALSE;
    }

    count_num = MIN( count_num, 999 );

    //convert uint32 num to wchar
    sprintf(count_str,"%d",count_num);
    str_len = strlen(count_str);
    MMI_STRNTOWSTR(count_wstr,10,(const uint8*)count_str,10,str_len);
    text_str.wstr_ptr = count_wstr ;
    text_str.wstr_len = str_len;    
    
    //set number icon string state and style
    text_style.font_color = MMI_WHITE_COLOR;  
    
    if( count_num <10)
    {
        text_style.font=SONG_FONT_14;
    }
    else
    {
        text_style.font=SONG_FONT_12;
    }    
    
    text_style.align=ALIGN_HVMIDDLE;

    disp_rect.left   = disp_rect.right - MENUICON_WIDTH/2;
    disp_rect.bottom = disp_rect.top + MENUICON_WIDTH/2;

    GUIRES_DisplaySabmImg(&disp_rect,PNULL,PNULL,win_handle,IMAGE_MAINMENU_NUMICON,&lcd_dev_info);
    GUISTR_DrawTextToLCDInRect(&lcd_dev_info,&disp_rect,&disp_rect,&text_str,&text_style,text_state,GUISTR_TEXT_DIR_AUTO);    

    return TRUE;
}

/*****************************************************************************/
//  Description : DrawSoftKeyButton
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note:
/*****************************************************************************/
LOCAL void DrawSoftKeyButton(MMI_IDLE_GRID_DATA_T *idle_grid_ptr)
{
    uint16 i = MMIGRID_SOFTKEY_LEFT;
    GUI_LCD_DEV_INFO  lcd_dev_info = {0};    
    uint32 count_num = 0;
    uint32 softkey_count = GetWidgetIdleSoftkeyCount();
    
#ifdef MMI_SAM_THEME	
    MMI_STRING_T     item_str    = {0};
    GUISTR_STYLE_T   str_style   = {0};
    GUISTR_STATE_T   str_state   = GUISTR_STATE_ALIGN | GUISTR_STATE_SINGLE_LINE | GUISTR_STATE_ELLIPSIS | GUISTR_STATE_EFFECT;
    GUI_RECT_T       text_rect   = {0};

    if (   (MMISET_IDLE_SUBSTYLE_SAM == MMIAPISET_GetIdleSubStyle())
        && UILAYER_IsLayerActive(&idle_grid_ptr->recycle_layer))
    {
        return;
    }    
#endif	
    
    if(UILAYER_IsLayerActive(&idle_grid_ptr->softkey_layer))
    {
        lcd_dev_info = idle_grid_ptr->softkey_layer;
        UILAYER_ClearRect(&lcd_dev_info,idle_grid_ptr->softkey_info[MMIGRID_SOFTKEY_BG].rc);    
       #ifdef UPB_IDLE_SUPPORT

        GUIRES_DisplayImg(PNULL, 
                  &idle_grid_ptr->softkey_info[MMIGRID_SOFTKEY_BG].rc, 
                  PNULL, 
                  idle_grid_ptr->win_id,
                  idle_grid_ptr->softkey_info[MMIGRID_SOFTKEY_BG].normal_img_id,
                  &lcd_dev_info); 

       #endif
    }
    else
    {
        lcd_dev_info = *MMITHEME_GetDefaultLcdDev();
    }
    
    while(i <= softkey_count)
    {
       if(idle_grid_ptr->softkey_info[i].is_pressed)
       {
        GUIRES_DisplayImg(PNULL, 
                  &idle_grid_ptr->softkey_info[i].rc, 
                  PNULL, 
                  idle_grid_ptr->win_id,
                  idle_grid_ptr->softkey_info[i].pressed_img_id,
                  &lcd_dev_info); 
#ifndef UI_LAYER_SUPPORT_LOW_MEMORY
            GUI_WeekLayer(&lcd_dev_info, &idle_grid_ptr->softkey_info[i].rc, MMIMENU_WEAKLAYERALPHA);
            GUILCD_InvalidateRect(0, idle_grid_ptr->softkey_info[i].rc, 0);
#endif
       }
       else
       {
        GUIRES_DisplayImg(PNULL, 
                  &idle_grid_ptr->softkey_info[i].rc, 
                  PNULL, 
                  idle_grid_ptr->win_id,
                  idle_grid_ptr->softkey_info[i].normal_img_id,
                  &lcd_dev_info); 
        }
                  
#ifdef MMI_SAM_THEME		
        if (MMIAPISET_GetIdleSubStyle() == MMISET_IDLE_SUBSTYLE_SAM)
        {
            GUI_RECT_T full_rc = MMITHEME_GetFullScreenRect();
            
            text_rect.left = MAX(full_rc.left, (idle_grid_ptr->softkey_info[i].rc.left-12));
            text_rect.top = idle_grid_ptr->softkey_info[i].rc.bottom;
            text_rect.right = MIN(full_rc.right, idle_grid_ptr->softkey_info[i].rc.right+12);
            text_rect.bottom = text_rect.top + MMIGRID_IDLE_SAMSUNG_FONT_HEIGHT;
            
            str_style.align = ALIGN_HVMIDDLE;
            str_style.font = MMITHEME_MAINMENU_TEXT_FONT;
            str_style.font_color = ~MMITHEME_GetCurThemeFontColor(MMI_THEME_MAINMENU_FONT);
            str_style.effect = FONT_EFFECT_REVERSE_EDGE;
            str_style.edge_alpha = 0x40;
            MMI_GetLabelTextByLang( idle_grid_ptr->softkey_info[i].normal_text_id, &item_str );
            //display text
            GUISTR_DrawTextToLCDInRect((const GUI_LCD_DEV_INFO*)&lcd_dev_info,
                (const GUI_RECT_T*)&text_rect,
                (const GUI_RECT_T*)&text_rect,
                (const MMI_STRING_T*)&item_str, 
                &str_style, 
                str_state,
                GUISTR_TEXT_DIR_AUTO);
        }
#endif
        
        count_num = GetMenuIconNumber(idle_grid_ptr->softkey_info[i].normal_img_id);
        DrawMenuNumberIcon(idle_grid_ptr->win_id, count_num, 
            idle_grid_ptr->softkey_info[i].rc, lcd_dev_info);
        i++;
    }
}

/*****************************************************************************/
//  Description : draw softkey anim of entering window
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note: 
/*****************************************************************************/
LOCAL void PlaySoftkeyEnterAnim(MMI_IDLE_GRID_DATA_T *idle_grid_ptr)
{
    int16 dy = 0;    
    int16 offset_y = 0;    
    int16 offset_x = 0;
    int16 offset_y_start = 0;    
    int16 offset_y_end = 0;    
    GUI_RECT_T  fullscreen_rect = MMITHEME_GetFullScreenRect();
    UILAYER_APPEND_BLT_T append_layer = {0};
    
#ifdef UPB_IDLE_SUPPORT
#ifdef MMI_SAM_THEME
    if (MMIAPISET_GetIdleSubStyle() != MMISET_IDLE_SUBSTYLE_SAM)
#endif
    {
        return;
    }
#endif
    
    if(MMK_IsWindowLandscape(idle_grid_ptr->win_id))
    {
        return;     //横屏暂时什么也不干
    }

    append_layer.lcd_dev_info = idle_grid_ptr->softkey_layer;
    append_layer.layer_level = UILAYER_LEVEL_NORMAL;
    UILAYER_AppendBltLayer(&append_layer);    
    
    UILAYER_GetLayerPosition(&idle_grid_ptr->softkey_layer, &offset_x, &offset_y);
   
    offset_y_start = fullscreen_rect.bottom;        
    offset_y_end = offset_y;        
    offset_y =  offset_y_start;        
    dy = (offset_y_end - offset_y_start + 1) >> 3;
    while(offset_y > offset_y_end)
    {        
        offset_y += dy;
		if (offset_y > offset_y_end) //Bug 125219 
		{
			break;   // PlaySoftkeyEnterAnim 测试要求不能抖，如果需要该特效会抖一下，请注掉此句
		}
        UILAYER_SetLayerPosition(&idle_grid_ptr->softkey_layer, offset_x,offset_y);
        MMITHEME_UpdateRect();  
        //SCI_SLEEP(10);
    }
    
    UILAYER_SetLayerPosition(&idle_grid_ptr->softkey_layer, offset_x, offset_y_end);
    MMITHEME_UpdateRect();  
}

/*****************************************************************************/
//  Description : draw softkey anim of entering window
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note: 
/*****************************************************************************/
LOCAL void PlaySoftkeyLeaveAnim(MMI_IDLE_GRID_DATA_T *idle_grid_ptr)
{
    int16 dy = 0;    
    int16 offset_y = 0;    
    int16 offset_x = 0;
    int16 offset_y_start = 0;    
    int16 offset_y_end = 0;    
    GUI_RECT_T  fullscreen_rect = MMITHEME_GetFullScreenRect();

#ifdef UPB_IDLE_SUPPORT
#ifdef MMI_SAM_THEME
    if (MMIAPISET_GetIdleSubStyle() != MMISET_IDLE_SUBSTYLE_SAM)
#endif
    {
        return;
    }
#endif
    
    if(MMK_IsWindowLandscape(idle_grid_ptr->win_id))
    {
        return;     //横屏暂时什么也不干
    }

    //这个是坑，窗口在收到lose focus的时候，
    //如果新开窗口的角度和当前窗口的角度不一致，
    //这个时候当前窗口的角度已经变为新窗口的角度，同时坐标系也跟着变化
    if(GUILCD_GetInitAngle(0) != GUILCD_GetLogicAngle(0))
    {
        return;
    }
   UILAYER_GetLayerPosition(&idle_grid_ptr->softkey_layer, &offset_x, &offset_y);
    offset_y_start = offset_y;        
    offset_y_end = fullscreen_rect.bottom;        
    offset_y =  offset_y_start;        
    dy = (offset_y_end - offset_y_start + 1) >> 3;
    while(offset_y < offset_y_end)
    {        
        offset_y += dy;
        UILAYER_SetLayerPosition(&idle_grid_ptr->softkey_layer, offset_x,offset_y);
        MMITHEME_UpdateRect();  
        //SCI_SLEEP(10);
    }
    UILAYER_SetLayerPosition(&idle_grid_ptr->softkey_layer, offset_x,offset_y_start);
}

/*****************************************************************************/
//  Description : draw softkey anim of entering window
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note: 
/*****************************************************************************/
LOCAL void PlayRecycleEnterAnim(MMI_IDLE_GRID_DATA_T *idle_grid_ptr)
{
    int16 dy = 0;    
    int16 offset_y = 0;    
    int16 offset_x = 0;
    int16 offset_y_start = 0;    
    int16 offset_y_end = 0;    
    GUI_RECT_T  fullscreen_rect = {0};
    GUI_RECT_T  recycle_rect = GetRecycleRect(idle_grid_ptr);
    fullscreen_rect = MMITHEME_GetFullScreenRect();

#ifdef MMI_SAM_THEME
    if (MMIAPISET_GetIdleSubStyle() == MMISET_IDLE_SUBSTYLE_SAM)
    {
        offset_y_start = fullscreen_rect.bottom;        
        offset_y_end = fullscreen_rect.bottom - (recycle_rect.bottom - recycle_rect.top) + 1;        
        offset_y =  offset_y_start;        
        dy = (offset_y_start - offset_y_end + 1) >> 3;
      
        MMIGRID_SetBltLayer( idle_grid_ptr);

        if(!MMK_IsWindowLandscape(idle_grid_ptr->win_id))
        {
            UILAYER_ClearRect(&idle_grid_ptr->softkey_layer, idle_grid_ptr->softkey_info[MMIGRID_SOFTKEY_BG].rc);
        }
        
        while(offset_y > offset_y_end)
        {
            UILAYER_SetLayerPosition(&idle_grid_ptr->recycle_layer, offset_x,offset_y);
            MMITHEME_UpdateRect();
            //SCI_SLEEP(10);        
            offset_y -= dy;
        }    
    }
    else
#endif    
    {    
        offset_y_start = fullscreen_rect.top - (recycle_rect.bottom - recycle_rect.top + 1);        
        offset_y_end = fullscreen_rect.top;        
        offset_y =  offset_y_start;        
        dy = (offset_y_end - offset_y_start + 1) >> 3;
      
        MMIGRID_SetBltLayer( idle_grid_ptr);
        while(offset_y < offset_y_end)
        {
            UILAYER_SetLayerPosition(&idle_grid_ptr->recycle_layer, offset_x,offset_y);
            MMITHEME_UpdateRect();
            //SCI_SLEEP(10);        
            offset_y += dy;
        }
    }
    
    UILAYER_SetLayerPosition(&idle_grid_ptr->recycle_layer, offset_x, offset_y_end);
    MMITHEME_UpdateRectEx();  
}

/*****************************************************************************/
//  Description : draw softkey anim of leaving window
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note: 
/*****************************************************************************/
LOCAL void PlayRecycleLeaveAnim(MMI_IDLE_GRID_DATA_T *idle_grid_ptr)
{
    int16 dy = 0;    
    int16 offset_y = 0;    
    int16 move_offset_x = 0;    
    int16 recycle_offset_x = 0;
    int16 offset_y_start = 0;    
    int16 offset_y_end = 0;    
   // uint8 alpha = 255;
   // uint8 d_alpha = 0;
    
    GUI_RECT_T  movelayer_rect = MMITHEME_GetFullScreenRect();
    GUI_RECT_T  recycle_rect = GetRecycleRect(idle_grid_ptr);
    
    if (   MMIGRID_TP_CLICK_LONG_WIDGET != idle_grid_ptr->tp_move_info.click_type
        && MMIGRID_TP_CLICK_LONG_SHORTCUT != idle_grid_ptr->tp_move_info.click_type)
    {
        return;
    }
    
#ifdef MMI_SAM_THEME
    if (MMIAPISET_GetIdleSubStyle() == MMISET_IDLE_SUBSTYLE_SAM)
    {
        if(!idle_grid_ptr->is_item_upon_recycle)
        {
            offset_y_start = recycle_rect.top;        
            offset_y_end   = recycle_rect.bottom; 
        }
        else
        {        
            movelayer_rect = UILAYER_GetLayerRect(&idle_grid_ptr->move_layer);    
            offset_y_start = movelayer_rect.top;        
            offset_y_end   = recycle_rect.bottom;   
        } 

        UILAYER_GetLayerPosition(&idle_grid_ptr->move_layer, &move_offset_x,  &offset_y);    
        UILAYER_GetLayerPosition(&idle_grid_ptr->recycle_layer, &recycle_offset_x,  &offset_y);
        offset_y =  offset_y_start;        
        dy = (offset_y_end - offset_y_start + 1) >> 3;
        //d_alpha = 255 >> 3;
        while(offset_y < offset_y_end)
        {               
            offset_y += dy;
            UILAYER_SetLayerPosition(&idle_grid_ptr->recycle_layer, recycle_offset_x,offset_y);
            if(idle_grid_ptr->is_item_upon_recycle)
            {
            UILAYER_SetLayerPosition(&idle_grid_ptr->move_layer, move_offset_x,offset_y);            
            }
            MMITHEME_UpdateRect();
            //SCI_SLEEP(10);             
        }          
    }
    else
#endif
    {
        if(!idle_grid_ptr->is_item_upon_recycle)
        {
            offset_y_start = recycle_rect.top;        
            offset_y_end = recycle_rect.top - recycle_rect.bottom;   
            //DestroyMoveLayer(idle_grid_ptr); 
        }
        else
        {        
            movelayer_rect = UILAYER_GetLayerRect(&idle_grid_ptr->move_layer);    
            offset_y_start = movelayer_rect.top;        
            offset_y_end = movelayer_rect.top - movelayer_rect.bottom;   
        }
    
        UILAYER_GetLayerPosition(&idle_grid_ptr->move_layer, &move_offset_x,  &offset_y);    
        UILAYER_GetLayerPosition(&idle_grid_ptr->recycle_layer, &recycle_offset_x,  &offset_y);
        offset_y =  offset_y_start;        
        dy = (offset_y_end - offset_y_start + 1) >> 3;
        //d_alpha = 255 >> 3;
        while(offset_y > offset_y_end)
        {               
            offset_y += dy;
            UILAYER_SetLayerPosition(&idle_grid_ptr->recycle_layer, recycle_offset_x,offset_y);
            if(idle_grid_ptr->is_item_upon_recycle)
            {
            UILAYER_SetLayerPosition(&idle_grid_ptr->move_layer, move_offset_x,offset_y);            
            }
            MMITHEME_UpdateRect();
            //SCI_SLEEP(10);             
        }    
    }
}

LOCAL GUI_RECT_T GetRecycleRect(MMI_IDLE_GRID_DATA_T *idle_grid_ptr)
{
    GUI_RECT_T rect =  MMITHEME_GetFullScreenRect();

#ifdef MMI_SAM_THEME
    if (MMIAPISET_GetIdleSubStyle() == MMISET_IDLE_SUBSTYLE_SAM)
    {
        rect.top =  rect.bottom - MMIGRID_IDLE_SAMSUNG_RECYCLE_HEIGHT;    
    }
    else
#endif
    {
    rect.bottom =  rect.top + MMIGRID_IDLE_RECYCLE_HEIGHT - 1;
    }
    
    return rect;
}

LOCAL void DrawRecycle(MMI_IDLE_GRID_DATA_T *idle_grid_ptr)
{
    MMI_IMAGE_ID_T bg_image_id = 0;   
    GUI_LCD_DEV_INFO  lcd_dev_info = {0};    
    GUI_RECT_T rect =  GetRecycleRect(idle_grid_ptr);
    MMI_IMAGE_ID_T del_img = IMAGE_IDLE_DELETE;
    uint16 width = 0;
    uint16 height = 0;     

    if (   MMIGRID_TP_CLICK_LONG_WIDGET != idle_grid_ptr->tp_move_info.click_type
        && MMIGRID_TP_CLICK_LONG_SHORTCUT != idle_grid_ptr->tp_move_info.click_type)
    {
        return;
    }

#ifdef MMI_SAM_THEME
    if (MMIAPISET_GetIdleSubStyle() == MMISET_IDLE_SUBSTYLE_SAM)
    {
        bg_image_id = IMAGE_SAM_IDLE_RECYLE_NORMAL;            
    }
    else
#endif
    {    
        if(idle_grid_ptr->is_item_upon_recycle)
        {
            bg_image_id = IMAGE_IDLE_RECYLE_OPEN;
        }
        else
        {
            bg_image_id = IMAGE_IDLE_RECYLE_NORMAL;            
        }
    }
    
    if(UILAYER_IsLayerActive(&idle_grid_ptr->recycle_layer))
    {
        lcd_dev_info = idle_grid_ptr->recycle_layer;
        UILAYER_Clear(&lcd_dev_info);    
    }
    else
    {
        lcd_dev_info = *MMITHEME_GetDefaultLcdDev();
    }
    
    GUIRES_DisplayImg(PNULL, 
                      &rect, 
                      PNULL, 
                      idle_grid_ptr->win_id,
                      bg_image_id, 
                      &lcd_dev_info);

    
#ifdef MMI_SAM_THEME
    if (MMIAPISET_GetIdleSubStyle() == MMISET_IDLE_SUBSTYLE_SAM)
    {
        return;
    }
#endif

    GUIRES_GetImgWidthHeight(&width,&height, del_img,idle_grid_ptr->win_id);
    rect = GUI_GetCenterRect(rect, width,  height);

    GUIRES_DisplayImg(PNULL, 
              &rect, 
              PNULL, 
              idle_grid_ptr->win_id,
              del_img,
              &lcd_dev_info); 
    return;                      
}

LOCAL uint16 GetPageTitleBarHeight(MMI_IDLE_GRID_DATA_T *idle_grid_ptr)
{    
    uint16 img_w = 0;
    uint16 img_h = 0;
    GUIRES_GetImgWidthHeight(&img_w, &img_h,IMAGE_GRID_PAGE_IDX,idle_grid_ptr->win_id);
#ifdef MMI_IDLE_THUMBVIEW_SLIDE
    return img_h + 12;
#else
    return img_h + 2;
#endif
}

LOCAL GUI_RECT_T GetPageTitleBarRect(MMI_IDLE_GRID_DATA_T *idle_grid_ptr)
{
    GUI_RECT_T rect = {0};
    if(!MMK_IsWindowLandscape(idle_grid_ptr->win_id))   
    {   
        rect = idle_grid_ptr->softkey_info[MMIGRID_SOFTKEY_BG].rc;
        rect.bottom = rect.top - 1;
        rect.top = rect.bottom - GetPageTitleBarHeight(idle_grid_ptr);        
    }
    else
    {
#ifdef BAR_PHONE_WIDGET
        rect = idle_grid_ptr->softkey_info[MMIGRID_SOFTKEY_BG].rc;
        rect.bottom = rect.top + 1;
        rect.top = rect.bottom - GetPageTitleBarHeight(idle_grid_ptr);        
#else
        rect = MMITHEME_GetFullScreenRect();
        rect.top = rect.bottom - GetPageTitleBarHeight(idle_grid_ptr);
#endif        
    }
    return rect;
}

/*****************************************************************************/
//  Description : draw page title bar.
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note:
/*****************************************************************************/
LOCAL void DrawPageTitleBar(MMI_IDLE_GRID_DATA_T *idle_grid_ptr, int32 cur_page_index)
{
    int32 i = 0;
    GUI_POINT_T start_point = {0};
    uint16 img_w = 0;
    uint16 img_h = 0;
    uint16 lcd_width = 0;
    uint16 lcd_height = 0;
    MMI_IMAGE_ID_T img_id = 0;
    GUI_LCD_DEV_INFO lcd_dev_info = {0};
    GUI_RECT_T     title_rect = GetPageTitleBarRect(idle_grid_ptr);
    
    
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);    
    GUIRES_GetImgWidthHeight(&img_w, &img_h,IMAGE_GRID_PAGE_IDX,idle_grid_ptr->win_id);

    if (UILAYER_IsLayerActive(&idle_grid_ptr->titlebar_layer))
    {
        lcd_dev_info = idle_grid_ptr->titlebar_layer;
        UILAYER_ClearRect(&lcd_dev_info, title_rect);        
    }
    else
    {
        lcd_dev_info = *MMITHEME_GetDefaultLcdDev();
    }   
 
    for (i = 0; i < MMI_GRID_PAGE_NUM; i++)
    {
        img_id = IMAGE_GRID_PAGE_IDX;
        
        if (i == cur_page_index)
        {
            img_id = IMAGE_GRID_PAGE_IDX_HL_1 + cur_page_index;
        }
        
        //start_point.x = title_rect.left + i * (img_w + 5);
        start_point.x = (lcd_width - MMI_GRID_PAGE_NUM * img_w)/2 + i*img_w;            
        start_point.y = title_rect.top+(title_rect.bottom-title_rect.top-img_h)/2;
        
        GUIRES_DisplayImg(&start_point, 
            PNULL, 
            PNULL, 
            idle_grid_ptr->win_id,
            img_id, 
            &lcd_dev_info);            
    }       
   
}

/*****************************************************************************/
//  Description : reset softkey status.
//  Global resource dependence : 
//  Author:  hongbo.lan
//  Note:
/*****************************************************************************/
LOCAL void ResetSoftkeyState(MMI_IDLE_GRID_DATA_T *idle_grid_ptr)
{
    uint16 _soft_index_ = MMIGRID_SOFTKEY_LEFT;
    BOOLEAN is_need_draw = FALSE;
    
    while(_soft_index_ < MMIGRID_SOFTKEY_MAX)
    {
        if(idle_grid_ptr->softkey_info[_soft_index_].is_pressed)
        {
            is_need_draw = TRUE;
        }
        idle_grid_ptr->softkey_info[_soft_index_++].is_pressed = FALSE;
    }
    if( MMIGRID_TP_MOVE_IDLE_TO_IDLE == idle_grid_ptr->widget_mov_type)
   // if(idle_grid_ptr->is_item_upon_recycle || MMIGRID_TP_MOVE_IDLE_TO_IDLE == idle_grid_ptr->widget_mov_type)
    {
        is_need_draw = TRUE;
    }

  //  idle_grid_ptr->is_item_upon_recycle = FALSE;/
}

/*****************************************************************************/
//  Description : get bytes number per pixel according to data type.
//  Global resource dependence : 
//  Author:   hongbo.lan
//   Note:
/*****************************************************************************/
LOCAL uint32 GetBytesPerPixel(
                              DISPLAY_DATA_TYPE_E data_type
                              )
{ 
    uint32 bytes_per_pixel = 0;

    switch(data_type)
    {
    case DATA_TYPE_ARGB888:
        bytes_per_pixel = 4;
        break;
    
    case DATA_TYPE_RGB565:
    default:
        bytes_per_pixel = 2;
        break;
    }
    return bytes_per_pixel;
}

/*****************************************************************************/
//  Description : blt one data buffer to another data buffer.
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note:
/*****************************************************************************/
LOCAL void BltBuffer(
                     uint8 *src_buf_ptr,
                     uint32 src_buf_width,
                     DISPLAY_DATA_TYPE_E src_data_type,
                     uint8 *dst_buf_ptr,
                     uint32 dst_buf_width,
                     DISPLAY_DATA_TYPE_E dst_data_type,
                     uint32 blt_width,
                     uint32 blt_height
                     )
{
    uint8 *src_buf = PNULL;
    uint8 *dst_buf = PNULL;
    uint32 xcount = 0;
    uint32 ycount = 0;
    uint32 src_bytes_per_pixel = 0;
    uint32 dst_bytes_per_pixel = 0;
    uint32 srcjumpcount = 0;
    uint32 dstjumpcount = 0;
    uint32 srcbltlinecount = 0;
    uint32 dstbltlinecount = 0;
    uint32 alpha  = 0;

    BOOLEAN is_same_type = FALSE;

    if ((src_buf_width < blt_width) || (dst_buf_width < blt_width))
    {
        return;
    }
    if ((PNULL == src_buf_ptr) || (PNULL == dst_buf_ptr))
    {
        return;
    }
    src_buf = src_buf_ptr;
    dst_buf = dst_buf_ptr;
    ycount = blt_height;
    src_bytes_per_pixel = GetBytesPerPixel(src_data_type);
    dst_bytes_per_pixel = GetBytesPerPixel(dst_data_type);

    is_same_type = (src_data_type == dst_data_type);
    srcjumpcount = (src_buf_width-blt_width)*src_bytes_per_pixel;
    dstjumpcount = (dst_buf_width-blt_width)*dst_bytes_per_pixel;
    srcbltlinecount = blt_width*src_bytes_per_pixel;
    dstbltlinecount = blt_width*dst_bytes_per_pixel;
    
    while(ycount)
    {
        if (is_same_type)
        {
            xcount = blt_width;
            
		    while(xcount--)
		    {
                if (0 == *((uint32*)src_buf))
                {
                    alpha = MMIWIDGET_THUMBVIEW_ALPHA;
                    *((uint32*)dst_buf) = (uint32)RGB5652ARGB8888(MMI_GRAY_WHITE_COLOR, 0);
                    alpha = alpha << 24;
                    *((uint32*)dst_buf) = ((*((uint32*)dst_buf)) | (alpha & 0xff000000));
                }
                else
                {
                    *((uint32*)dst_buf) = *((uint32*)src_buf);                    
                }
 
    		    src_buf+=src_bytes_per_pixel;
    		    dst_buf+=dst_bytes_per_pixel;
            }
        }

		src_buf += srcjumpcount;
		dst_buf += dstjumpcount;
		ycount--;
    }
}

LOCAL void SetBufferAlpha(
                     uint8 *dst_buf_ptr,
                     uint32 dst_buf_width,
                     DISPLAY_DATA_TYPE_E dst_data_type,
                     uint32 blt_width,
                     uint32 blt_height
                     )
{
    uint32 *dst_buf = PNULL;
    uint32 xcount = 0;
    uint32 ycount = 0;
    uint32 dstjumpcount = 0;
    uint32 color = 0;
    
    dst_buf = (uint32*)dst_buf_ptr;
    ycount = blt_height;
    
    dstjumpcount = dst_buf_width-blt_width;
    
    color = (uint32)RGB5652ARGB8888(MMI_GRAY_WHITE_COLOR, MMIWIDGET_THUMBVIEW_ALPHA);
    
    while(ycount)
    {     
            xcount = blt_width;
            
            while(xcount--)
            {
                if (0 == *dst_buf)
                {
                    *dst_buf = color ;
                }  
                dst_buf++;
            }
       
        dst_buf += dstjumpcount;
        ycount--;
    }
}
                          
/*****************************************************************************/
//  Description : zoom in page to thumb on main layer.
//  Global resource dependence : 
//  Author:  hongbo.lan
//  Note:
/*****************************************************************************/
LOCAL void CopyPage2Buffer(MMI_IDLE_GRID_DATA_T *idle_grid_ptr, uint8 *dst_buf, uint32 page_idx)
{
    UILAYER_INFO_T      layer_info = {0};
    UILAYER_COPYBUF_PARAM_T     copy_param = {0};
    UILAYER_COPYBUF_T           copy_buf = {0};
    uint16 page_width = 0;
    
    UILAYER_GetLayerInfo(&idle_grid_ptr->page_layer, &layer_info);
    page_width = layer_info.layer_width / idle_grid_ptr->page_layer_size;
    copy_param.rect = UILAYER_GetLayerRect(&idle_grid_ptr->page_layer);
    copy_param.rect.left += (int16)page_idx*page_width;
    copy_param.rect.right = (copy_param.rect.left+page_width-1);
    copy_param.rect.bottom = (copy_param.rect.top+layer_info.layer_height-1);
    
    copy_buf.buf_ptr = (uint8*)dst_buf;
    copy_buf.data_type = DATA_TYPE_ARGB888;
    copy_buf.width = page_width;
    copy_buf.height = layer_info.layer_height;
    
    UILAYER_CopyLayerToBuffer(&idle_grid_ptr->page_layer, &copy_param, &copy_buf);        

}
                          

/*****************************************************************************/
//  Description : zoom in page to thumb on main layer.
//  Global resource dependence : 
//  Author:  ying.xu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN DisplayGridPageThumbView(MMI_IDLE_GRID_DATA_T *idle_grid_ptr)
{
    UILAYER_INFO_T      layer_info = {0};
    GUI_LCD_DEV_INFO    thumb_layer = {0};
    GUI_RECT_T          blt_rect   = {0};
    UILAYER_APPEND_BLT_T append_layer = {0};
    uint16 lcd_width  = 0;
    uint16 lcd_height = 0;
    uint8 *tar_buf   = PNULL; 
    uint32 i          = 0;
    uint32 k          = 0;
    GUI_RECT_T      screen_rect = MMITHEME_GetFullScreenRect();
    GUI_RECT_T      thumb_rect = {0};
    BOOLEAN    result = TRUE;
    DISPLAY_DATA_TYPE_E data_type = DATA_TYPE_ARGB888;
    
	GUIAPICTRL_SetState(MMIWIDGET_COUNTEDTIME_TIMEEDITBOX_CTRL_ID, GUICTRL_STATE_DISABLE_TP, TRUE );     
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height); 

    idle_grid_ptr->thumb_view_layer =  MMIWIDGET_CreateLayer(idle_grid_ptr->win_id, &screen_rect, data_type);
    
    if(!UILAYER_IsLayerActive(&idle_grid_ptr->thumb_view_layer))
    {
        //SCI_TRACE_LOW:"MMIGRID CreateThumbViewLayer  == FALSE"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIGRID_IDLE_9091_112_2_18_3_7_16_39,(uint8*)"");
        result =  FALSE;
        goto DisplayGridPageThumbView_ERROR_HANDLEMENT;
    }

    thumb_rect = GUI_CreateRect(0 , 0 , MMI_IDLE_GRID_THUMB_WIDTH - 1 ,
                        MMI_IDLE_GRID_THUMB_HEIGHT - 1 );
    thumb_layer = MMIWIDGET_CreateLayer(idle_grid_ptr->win_id, &thumb_rect, data_type);
    tar_buf = UILAYER_GetLayerBufferPtr(&thumb_layer);    
    if(PNULL == tar_buf)
    {
        //SCI_TRACE_LOW:"MMIGRID DisplayGridPageThumbView tar_buf == 0"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIGRID_IDLE_9102_112_2_18_3_7_16_40,(uint8*)"");
        result =  FALSE;
        goto DisplayGridPageThumbView_ERROR_HANDLEMENT;
    }
    UILAYER_GetLayerInfo(&thumb_layer, &layer_info);
    
    UILAYER_Clear(&idle_grid_ptr->page_layer);
#ifndef BAR_PHONE_WIDGET 
    UILAYER_RemoveBltLayer(&idle_grid_ptr->softkey_layer);
#endif	

    for (i = 0; i < MMI_GRID_PAGE_NUM; i++)
    {
        UILAYER_Clear( &idle_grid_ptr->thumb_view_layer);
        DrawSpecificPageEx(idle_grid_ptr, idle_grid_ptr->thumb_view_layer, i,  0, 0 );
        if(FALSE == ThumbLayer(&idle_grid_ptr->thumb_view_layer, &screen_rect,
                                 &thumb_layer, &thumb_rect, data_type, 0))
        {
            //SCI_TRACE_LOW:"MMIGRID ThumbLayer  == FALSE"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIGRID_IDLE_9118_112_2_18_3_7_16_41,(uint8*)"");
            result =  FALSE;
            goto DisplayGridPageThumbView_ERROR_HANDLEMENT;
        }

        SetBufferAlpha(tar_buf, layer_info.mem_width,data_type,
                    thumb_rect.right - thumb_rect.left + 1,
                      thumb_rect.bottom - thumb_rect.top + 1);
        blt_rect = s_thumb_rect_grp[i];       

    
        append_layer.lcd_dev_info = thumb_layer;
        append_layer.layer_level = UILAYER_LEVEL_NORMAL;
        UILAYER_AppendBltLayer(&append_layer);
        
        k = 0;
        while (k < blt_rect.left)         /*lint !e574 !e737*/
        {
            UILAYER_SetMainLayerImmutable(TRUE);    
            UILAYER_SetLayerMemFormat(&idle_grid_ptr->page_layer, UILAYER_MEM_IMMUTABLE);   
            //UILAYER_SetDirectDraw(TRUE);        
            UILAYER_SetLayerPosition(&thumb_layer, k, blt_rect.top);
            k += 10;
            MMITHEME_UpdateRect();
        }

        UILAYER_SetLayerPosition(&thumb_layer, blt_rect.left, blt_rect.top);
        
        UILAYER_BltLayerToLayer(&idle_grid_ptr->page_layer,
                                &thumb_layer,
                                &blt_rect,
                                FALSE);   
        UILAYER_RemoveBltLayer(&thumb_layer);
        MMITHEME_UpdateRect();
    }     
    result =  TRUE;    
DisplayGridPageThumbView_ERROR_HANDLEMENT:
    UILAYER_RELEASELAYER(&thumb_layer);      /*lint !e506 !e774*/
    UILAYER_RELEASELAYER(&idle_grid_ptr->thumb_view_layer);      /*lint !e506 !e774*/
    return result;
}

/*****************************************************************************/
//  Description : Enter Thumbview State
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN EnterThumbviewState(MMI_IDLE_GRID_DATA_T *idle_grid_ptr)
{
    if (MMIGRID_RUN_MODE_NORMAL == idle_grid_ptr->run_mode)
    {
#ifdef LIVE_WALLPAPER_FRAMEWORK_SUPPORT
        if(MMIAPIIDLE_IsLiveWallpaper())
        {
            MMIAPILIVEWALLPAPER_Stop();
            //MMIAPILIVEWALLPAPER_Destroy(TRUE);
            MMK_SendMsg(idle_grid_ptr->win_id, MSG_FULL_PAINT, PNULL);
        }
#endif
#ifdef MMI_VIDEOWALLPAPER_SUPPORT
        if(MMIAPISETVWP_IsOpen())
        {
            MMIAPISETVWP_StopVideoWallpaper();
            MMK_SendMsg(idle_grid_ptr->win_id, MSG_FULL_PAINT, PNULL);
        }
#endif
        if(DisplayGridPageThumbView(idle_grid_ptr))
        {        
            idle_grid_ptr->run_mode = MMIGRID_RUN_MODE_THUMBVIEW;
            idle_grid_ptr->is_thumb_showed = TRUE;
            return TRUE;
        }
        else
        {
            MMIPUB_OpenAlertFailWin( TXT_IDLE_OUT_OF_MEMORY);
    }
    }
    return FALSE;
}

/*****************************************************************************/
//  Description : Exit Thumbview State
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN ExitThumbviewState(MMI_IDLE_GRID_DATA_T *idle_grid_ptr)
{
    if (MMIGRID_RUN_MODE_THUMBVIEW == idle_grid_ptr->run_mode)
    {
        idle_grid_ptr->run_mode = MMIGRID_RUN_MODE_NORMAL; 
        idle_grid_ptr->is_thumb_showed = FALSE;
        //DestroyGridThumbViewLayer(idle_grid_ptr);
#ifdef LIVE_WALLPAPER_FRAMEWORK_SUPPORT
        if(MMIAPIIDLE_IsLiveWallpaper())
        {
            //MMIAPILIVEWALLPAPER_Init();
            MMIAPILIVEWALLPAPER_Play(idle_grid_ptr->win_id); 
        }
#endif
#ifdef	MMI_VIDEOWALLPAPER_SUPPORT
        if (MMIAPISETVWP_IsOpen())
        {
            MMIAPISETVWP_SwitchToNext();
        }
#endif
        MMIGRID_SetCurPageIndex(idle_grid_ptr->cur_page_index);
        MMIWIDGET_ReCalcAllWidgetWinRect(idle_grid_ptr);  
        return TRUE;
    }
    return FALSE;
}

/*****************************************************************************/
//  Description : Reset Factory Setting
//  Global resource dependence : none
//  Author: nan.ji
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIGRID_ResetFactorySetting(void)
{
    MMI_GRID_SAVE_INFO_T  set_nv = {0};
    MMI_IDLE_GRID_DATA_T *idle_grid_ptr = MMIAPIGRID_GetGridPtr();
    if(0 != idle_grid_ptr)
    {
        MMIWIDGET_UnLoadAllWidget(idle_grid_ptr);
        MMIAPIGRID_DestroyIdle();
    }

    SCI_MEMSET(&set_nv, 0, sizeof(MMI_GRID_SAVE_INFO_T));    	
    
#ifdef MMI_SAM_THEME	
    MMINV_WRITE(MMI_SAM_SAVE_INFO, &set_nv);    
#endif
    
    MMINV_WRITE(MMI_GRID_SAVE_INFO, &set_nv);    

#ifdef MMI_WIDGET_MEMO 
    MMIWIDGET_MEMO_ResetFactorySettings();    
#endif
#if defined MMI_GRID_CONTACT
    MMIWIDGET_PB_ResetFactorySettings();
#endif

}

/*****************************************************************************/
//  Description : get tp type
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMIGRID_SOFTKEY_TYPE_E GetTpType(
                                  GUI_POINT_T   tp_point
                                  )
{
    uint32                  i               = 0;
    MMIGRID_SOFTKEY_TYPE_E       tp_type    = 0;

    for (i=0; i<MMIGRID_SOFTKEY_MAX; i++)
    {
        if (GUI_PointIsInRect(tp_point,s_display_rect[i]))
        {
            tp_type = (MMIGRID_SOFTKEY_TYPE_E)i;
            break;
        }
    }

    return (tp_type);
}

/*****************************************************************************/
//  Description : set display rect
//  Global resource dependence : 
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL void SetDisplayRect(
                          GUI_RECT_T           rect,
                          MMIGRID_SOFTKEY_TYPE_E    tp_type
                          )
{
    if (MMIGRID_SOFTKEY_MAX <= tp_type)
    {
        //SCI_TRACE_LOW:"SetDisplayRect ERROR! tp_type = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIGRID_IDLE_9425_112_2_18_3_7_17_44,(uint8*)"d",tp_type);
        return;
    }
    else
    {
        //set rect
        s_display_rect[tp_type].left   = rect.left;
        s_display_rect[tp_type].top    = rect.top;
        s_display_rect[tp_type].right  = rect.right;
        s_display_rect[tp_type].bottom = rect.bottom;
    }
}

/*****************************************************************************/
//  Description : display number icon in idle 
//  Global resource dependence : 
//  Author: xinhe.yan
//  Note:在有未接来电或短信时，显示其数目
/*****************************************************************************/
// LOCAL MMI_RESULT_E DisplayIdleNumberIcon(  
//                                       MMI_IDLE_GRID_DATA_T *idle_grid_ptr,
//                                       MMI_MENU_ID_T menu_id,
//                                       GUI_RECT_T          disp_rect,  //number icon rect
//                                       GUI_LCD_DEV_INFO    lcd_dev_info //icon layer handle
//                                       )
// {     
//     char    count_str[10] = {0};
//     wchar   count_wstr[10]  = {0}; 
//     uint32  count_num = 0; 
//     uint16  piexl_number=0;
//     uint32  str_len = 0;  
//     MMI_STRING_T    text_str  = {0}; 
//     GUISTR_STYLE_T  text_style = {0}; 
//     GUISTR_STATE_T  text_state = GUISTR_STATE_ALIGN;
//     MMI_RESULT_E    result = MMI_RESULT_FALSE;
// 
//     if (PNULL == idle_grid_ptr)
//     {
//         return result;
//     }
// 
//     //get number of item
//     if( ID_COMMUNICATION_DIAL == menu_id)
//     {
//         count_num = MMIAPICC_GetMissedCallNumber();
//     }
//     else if( ID_COMMUNICATION_MESSAGE == menu_id)
//     {
//         count_num = MMIAPISMS_GetTobeReadMessageCount(MN_DUAL_SYS_MAX) + MMIAPISMS_GetUnDownloadMsgCount(MN_DUAL_SYS_MAX);
//     }  
//     
//     if(count_num == 0 )
//     {
//         return result;
//     }
// 
//     //convert uint32 num to wchar
//     sprintf(count_str,"%d", count_num);
//     str_len = strlen(count_str);
//     MMI_STRNTOWSTR(count_wstr,10,(const uint8*)count_str,10,str_len);
//     text_str.wstr_ptr = count_wstr;
//     text_str.wstr_len = str_len;    
//     
//     //set number icon string state and style
//     text_style.font_color = MMI_WHITE_COLOR;   
//     
//     if( count_num < 10)
//     {
//         text_style.font = SONG_FONT_14;
//     }
//     else
//     {
//         text_style.font = SONG_FONT_12;
//     } 
//     
//     text_style.align = ALIGN_HVMIDDLE;
//     
//     piexl_number = GUI_CalculateStringPiexlNum(text_str.wstr_ptr,text_str.wstr_len,SONG_FONT_12,0);
//     
//     //stretch picture when count_num greater than 999
//     if (count_num > 999)
//     {
//         disp_rect.left = disp_rect.right - (piexl_number+10);
//     }
//     
//     //draw number icon to page layer
//     GUIRES_DisplayImg(PNULL,&disp_rect,PNULL,idle_grid_ptr->win_id,IMAGE_MAINMENU_NUMICON,&lcd_dev_info);
//     
//     GUISTR_DrawTextToLCDInRect(&lcd_dev_info,&disp_rect,&disp_rect,&text_str,&text_style,text_state,GUISTR_TEXT_DIR_AUTO);    
//        
//     result = MMI_RESULT_TRUE;
// 
//     return result;
// }
/*****************************************************************************/
//  Description : HandleKeyDownMsg
//  Global resource dependence : 
//  Author: juan.wu
//  Note:处理keydown消息
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleKeyDownMsg(MMI_IDLE_GRID_DATA_T * idle_grid_ptr,MMI_MESSAGE_ID_E  msg_id)
{
    MMIGRID_GRID_INDEX_TYPE_T   grid_type[MMI_IDLE_GRID_PAGE_CELL_NUM] = {0};
    MMI_RESULT_E         result = MMI_RESULT_FALSE;
    uint32               cur_index = 0;
    uint32               next_index = 0;

    if(GetIsEditable())
    {
        return result; 
    }
#ifdef BAR_PHONE_WIDGET 
    GetCurPageWidgetIdAndIndex(idle_grid_ptr,PNULL,&idle_grid_ptr->current_gird);
#endif
    cur_index = idle_grid_ptr->current_gird;
    GetCurPageGridType(idle_grid_ptr,grid_type);
    result = SetItemFocus(idle_grid_ptr, cur_index, grid_type, msg_id);
    if(MMI_RESULT_FALSE == result)
    {
        next_index = GetNextValidGrid(idle_grid_ptr,cur_index,grid_type);
        if(next_index < MMI_IDLE_GRID_PAGE_CELL_NUM)
        {
            cur_index = next_index;
            result = SetItemFocus(idle_grid_ptr, cur_index, grid_type, msg_id);
        }
        else
        {
            result = MMIWIDGET_RunChildWidgetProc(idle_grid_ptr, grid_type[cur_index].widget_id, MSG_WIDGET_ACTIVE, PNULL);
        }
        
    }
    return result;
}
/*****************************************************************************/
//  Description : GetGridIndexIsinvalid
//  Global resource dependence : 
//  Author: juan.wu
//  Note:遍历每个格子，看是否有效，如果几个格子放的是一个widget，则认为后面的格子无效
/*****************************************************************************/
LOCAL  void GetGridIndexIsinvalid(MMIGRID_GRID_INDEX_TYPE_T *grid_type)
{
    uint8 i = 0;
    uint8 j = 0;
    
    for(i = 0; i < MMI_IDLE_GRID_PAGE_CELL_NUM; i++)
    {
        if(MMIGRID_ITEM_TYPE_WIDGET == grid_type[i].type)
        {
            for(j = i+1; j < MMI_IDLE_GRID_PAGE_CELL_NUM; j++)
            {
                if(grid_type[i].widget_id == grid_type[j].widget_id)
                {
                    grid_type[j].is_invalid = TRUE;//说明j这个格子无效
                }
            }
        }

    }
}

/*****************************************************************************/
//  Description : GetCurPageGridType
//  Global resource dependence : 
//  Author: juan.wu
//  Note:获得grid_idle每个格子的类型，包括index,widget_id,shortcut_id...
/*****************************************************************************/
LOCAL void GetCurPageGridType(
                                MMI_IDLE_GRID_DATA_T * idle_grid_ptr,
                                MMIGRID_GRID_INDEX_TYPE_T *grid_type
                                )
{
    int16 grid_index = 0;
    int16 grid_row = 0;
    int16 grid_col = 0;
    int16 total_grid = 0;
    
    GetPageGridRowCol(&grid_row, &grid_col, idle_grid_ptr);
    total_grid = grid_row * grid_col;
    
    for(grid_index = 0; grid_index < total_grid; grid_index++)
    {
        if (IS_NODE_WIDGET(idle_grid_ptr->grid_data_tab[idle_grid_ptr->cur_page_index][grid_index]))
        {
            grid_type[grid_index].index = grid_index;
            grid_type[grid_index].type = MMIGRID_ITEM_TYPE_WIDGET;
            grid_type[grid_index].widget_id = GET_NODE_ID(idle_grid_ptr->grid_data_tab[idle_grid_ptr->cur_page_index][grid_index]);
        }
        else
        if (IS_NODE_ICON(idle_grid_ptr->grid_data_tab[idle_grid_ptr->cur_page_index][grid_index]))
        {
            grid_type[grid_index].index = grid_index;
            grid_type[grid_index].type = MMIGRID_ITEM_TYPE_ICON;
            grid_type[grid_index].shortcut_id = grid_index;
        }
        else
        {
            grid_type[grid_index].index = grid_index;
            grid_type[grid_index].type = MMIGRID_ITEM_TYPE_INVALID;
        }
    }
    GetGridIndexIsinvalid(grid_type);
}
/*****************************************************************************/
//  Description : GetNextValidGrid
//  Global resource dependence : 
//  Author: juan.wu
//  Note:获得下一个格子的索引值
/*****************************************************************************/
LOCAL int32   GetNextValidGrid(MMI_IDLE_GRID_DATA_T * idle_grid_ptr,
                                int32 first_grid_index,
                                MMIGRID_GRID_INDEX_TYPE_T *grid_type
                                )
{
    int32 grid_index = 0;
    int32 next_index = 0xffff;
    int16 total_grid = 0;
    int16 grid_row = 0;
    int16 grid_col = 0;
      
    GetPageGridRowCol(&grid_row, &grid_col, idle_grid_ptr);
    total_grid = grid_row * grid_col;


    if(0xffff == first_grid_index)//非法
    {
        for(grid_index = 0; grid_index < total_grid; grid_index++)
        {
            if(MMIGRID_ITEM_TYPE_INVALID != grid_type[grid_index].type)
            {
                next_index = grid_index;
                return  next_index;
            }
        }

    }
    for(grid_index = first_grid_index; grid_index < total_grid; grid_index++)
    {
        if(grid_index >= MMI_IDLE_GRID_PAGE_CELL_NUM ||first_grid_index >= MMI_IDLE_GRID_PAGE_CELL_NUM)
        {
            break;
        }
        if(MMIGRID_ITEM_TYPE_INVALID != grid_type[grid_index].type)
        {
            if(MMIGRID_ITEM_TYPE_WIDGET == grid_type[grid_index].type && !grid_type[grid_index].is_invalid)
            {
                if(grid_type[first_grid_index].widget_id != grid_type[grid_index].widget_id)
                {
                    next_index = grid_index;
                    break;
                }
            }
            else if(MMIGRID_ITEM_TYPE_ICON == grid_type[grid_index].type)
            {
                if(grid_index != first_grid_index)
                {
                    next_index = grid_index;
                    break;
                }
            }
        }
    }
    return next_index;
}

/*****************************************************************************/
//  Description : GetPreValidGrid
//  Global resource dependence : 
//  Author: juan.wu
//  Note:获得上一个有效格子的索引值
/*****************************************************************************/
LOCAL int32   GetPreValidGrid(MMI_IDLE_GRID_DATA_T * idle_grid_ptr,
                                int32   last_grid_index,
                                MMIGRID_GRID_INDEX_TYPE_T *grid_type
                                )
{
    int32 grid_index = 0;
    int32 pre_index = 0xffff;
    
    if(0xffff == last_grid_index)
    {
        for(grid_index = MMI_IDLE_GRID_PAGE_CELL_NUM - 1; grid_index >=0; grid_index--)
        {
            if(grid_index >= MMI_IDLE_GRID_PAGE_CELL_NUM)
            {
                return 0;
            }
            if(MMIGRID_ITEM_TYPE_INVALID != grid_type[grid_index].type)
            {
                pre_index = grid_index;
                return  pre_index;
            }
        }
    }

    for(grid_index = last_grid_index; grid_index >= 0; grid_index--)
    {
        if(grid_index >= MMI_IDLE_GRID_PAGE_CELL_NUM)
        {
            break;
        }
        if(MMIGRID_ITEM_TYPE_INVALID != grid_type[grid_index].type)
        {
            if(MMIGRID_ITEM_TYPE_WIDGET == grid_type[grid_index].type && !grid_type[grid_index].is_invalid)
            {
                if(grid_type[last_grid_index].widget_id != grid_type[grid_index].widget_id)
                {
                    pre_index = grid_index;
                    break;
                }
            }
            else if(MMIGRID_ITEM_TYPE_ICON == grid_type[grid_index].type)
            {
                if(grid_index != last_grid_index)
                {
                    pre_index = grid_index;
                    break;
                }
            }
        }
    }
    return pre_index;
}

/*****************************************************************************/
//  Description : SetItemFocus
//  Global resource dependence : 
//  Author: juan.wu
//  Note:设置focus状态
/*****************************************************************************/
LOCAL MMI_RESULT_E SetItemFocus(
                        MMI_IDLE_GRID_DATA_T * idle_grid_ptr,
                        uint32 index, 
                        MMIGRID_GRID_INDEX_TYPE_T  *grid_type,
                        MMI_MESSAGE_ID_E  msg_id
                        )
{
    MMI_RESULT_E         result      = MMI_RESULT_FALSE;
    MMI_HANDLE_T         win_handle  = 0;
    GUI_RECT_T           dis_rect    = {0};
    GUI_RECT_T              win_rect = {0};
    GUI_LCD_DEV_INFO        lcd_dev_info = {0, 0};

    if(index >= MMI_IDLE_GRID_PAGE_CELL_NUM || PNULL == idle_grid_ptr)
    {
        return MMI_RESULT_FALSE;
    }
    if(MMIGRID_ITEM_TYPE_WIDGET == grid_type[index].type)//如果是widget则把该widget设为focus
    {        
        win_handle = MMIWIDGET_GetWinHandleByWidgetId(idle_grid_ptr,grid_type[index].widget_id);
        //MMK_SetChildWinFocus(idle_grid_ptr->win_id, win_handle);
        MMK_GetWinRect(win_handle, &win_rect);
        win_rect.left--;
        win_rect.top--;
        win_rect.right++;
        win_rect.bottom++;  
        win_rect.left = MAX(0,win_rect.left);
        win_rect.right = MIN(win_rect.right,LCD_WIDTH-1);
        MMK_GetWinLcdDevInfo(win_handle, &lcd_dev_info);
        result = MMIWIDGET_RunChildWidgetProc(idle_grid_ptr, grid_type[index].widget_id, msg_id, PNULL);
        if(MMI_RESULT_FALSE == result)
        {
            if(grid_type[idle_grid_ptr->current_gird].widget_id != grid_type[index].widget_id )
            {
                DrawSpecificPage(idle_grid_ptr, idle_grid_ptr->cur_page_index);
                LCD_DrawRect(&lcd_dev_info, win_rect, MMI_GREEN_COLOR);   
                MMIWIDGET_RunChildWidgetProc(idle_grid_ptr, grid_type[idle_grid_ptr->current_gird].widget_id,MSG_WIDGET_DEACTIVE , PNULL);
            }
        }
        else
        {
            DrawSpecificPage(idle_grid_ptr, idle_grid_ptr->cur_page_index);
        }
        idle_grid_ptr->current_gird = index;
    }
    else if(MMIGRID_ITEM_TYPE_ICON == grid_type[index].type)
    {
        if(idle_grid_ptr->current_gird == index)    /*lint !e737 */
        {
            return MMI_RESULT_FALSE;
        }        
        idle_grid_ptr->current_gird = index;
        DrawSpecificPage(idle_grid_ptr, idle_grid_ptr->cur_page_index);
        dis_rect = idle_grid_ptr->page_grid_rect[index];
#ifndef UI_LAYER_SUPPORT_LOW_MEMORY
        GUI_WeekLayer(&idle_grid_ptr->page_layer, &dis_rect, MMIMENU_WEAKLAYERALPHA);
        GUILCD_InvalidateRect(0, dis_rect, 0);      
#endif
    }
    return result;
}
/*****************************************************************************/
//  Description : HandleWebKeyMsg
//  Global resource dependence : 
//  Author: juan.wu
//  Note:处理WEB消息
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleWebKeyMsg(MMI_IDLE_GRID_DATA_T * idle_grid_ptr,MMI_MESSAGE_ID_E  msg_id)
{
    MMI_RESULT_E result = MMI_RESULT_FALSE;
    MMIGRID_GRID_INDEX_TYPE_T   grid_type[MMI_IDLE_GRID_PAGE_CELL_NUM] = {0};   /* lint !e529  !e830 */

    if(GetIsEditable() || PNULL == idle_grid_ptr)
    {
        return result; 
    }    
#ifdef BAR_PHONE_WIDGET 
    {
        uint32 widget_id = 0;
        GetCurPageWidgetIdAndIndex(idle_grid_ptr,&widget_id,0);
        MMIWIDGET_RunChildWidgetProcEx(idle_grid_ptr,widget_id, msg_id,  0,FALSE);   
        result = MMI_RESULT_TRUE;
    }
#else
    GetCurPageGridType(idle_grid_ptr,grid_type);
    if(MMIGRID_ITEM_TYPE_WIDGET == grid_type[idle_grid_ptr->current_gird].type)
    {
        result = MMIWIDGET_RunChildWidgetProc(idle_grid_ptr, grid_type[idle_grid_ptr->current_gird].widget_id, msg_id, PNULL);
    }
    else if(MMIGRID_ITEM_TYPE_ICON == grid_type[idle_grid_ptr->current_gird].type)
    {
        uint32               item_id = 0;
        MMI_MENU_GROUP_ID_T group_id = 0;
        MMI_MENU_ID_T       menu_id  = 0;
        item_id  = GET_NODE_ID(idle_grid_ptr->grid_data_tab[idle_grid_ptr->cur_page_index][idle_grid_ptr->current_gird]);
        group_id = idle_grid_ptr->icon_ptr[item_id].icon_info.group_id;
        menu_id  = idle_grid_ptr->icon_ptr[item_id].icon_info.menu_id;

        MMIAPIMENU_HandleIdleMenuItem(group_id, menu_id);
        result = MMI_RESULT_TRUE;
    }
#endif    
    return result;
}
/*****************************************************************************/
//  Description : HandleKeyUpMsg
//  Global resource dependence : 
//  Author: juan.wu
//  Note:处理up消息
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleKeyUpMsg(MMI_IDLE_GRID_DATA_T * idle_grid_ptr,MMI_MESSAGE_ID_E  msg_id)
{
    MMIGRID_GRID_INDEX_TYPE_T   grid_type[MMI_IDLE_GRID_PAGE_CELL_NUM] = {0};
    MMI_RESULT_E         result = MMI_RESULT_FALSE;
    uint32               cur_index = 0;
    uint32               pre_index = 0;
    
    if(GetIsEditable() || PNULL == idle_grid_ptr)
    {
        return result; 
    }    
#ifdef BAR_PHONE_WIDGET 
    GetCurPageWidgetIdAndIndex(idle_grid_ptr,PNULL,&idle_grid_ptr->current_gird);
#endif
    cur_index = idle_grid_ptr->current_gird;
    GetCurPageGridType(idle_grid_ptr,grid_type);
    result = SetItemFocus(idle_grid_ptr, cur_index, grid_type, msg_id);
    if(MMI_RESULT_FALSE == result)
    {
        pre_index = GetPreValidGrid(idle_grid_ptr,cur_index,grid_type);
        if(pre_index < MMI_IDLE_GRID_PAGE_CELL_NUM)
        {
            cur_index = pre_index;
            result = SetItemFocus(idle_grid_ptr, cur_index, grid_type, msg_id);
        }
        else
        {
            result = MMIWIDGET_RunChildWidgetProc(idle_grid_ptr, grid_type[cur_index].widget_id, MSG_WIDGET_ACTIVE, PNULL);
        }
        
    }
    return result;
}

/*****************************************************************************/
//  Description : HandleSlidePageMove
//  Global resource dependence : 
//  Author: juan.wu
//  Note:处理LEFT RIGHT消息
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSlidePageMove(MMI_IDLE_GRID_DATA_T * idle_grid_ptr,MMI_MESSAGE_ID_E  msg_id)
{
    MMI_RESULT_E result = MMI_RESULT_FALSE;
    int32 dest_page_index = 0;   //for now ,only support sliding for 1 page
    MMIWIDGET_PEN_MOVE_DIRECT_T move_direct = 0;
    
    if(0 != idle_grid_ptr->slide_anim.slide_flag)
    {
        return result;
    }
  MMIWIDGET_DEBUG(LOG_STR_ENTER("timer = %d"), SCI_GetTickCount());
    
    if (MSG_APP_RIGHT == msg_id || MSG_KEYREPEAT_RIGHT == msg_id)
    {  
        dest_page_index = idle_grid_ptr->cur_page_index + 1;
        if(dest_page_index >= MMI_GRID_PAGE_NUM)
        {
            dest_page_index = 0;
        }
        move_direct = MMIWIDGET_PEN_MOVE_LEFT;
        result = MMI_RESULT_TRUE;
    }
    else if(MSG_APP_LEFT== msg_id || MSG_KEYREPEAT_LEFT== msg_id)
    {
        dest_page_index = idle_grid_ptr->cur_page_index - 1;
        if(dest_page_index < 0)
        {
            dest_page_index = MMI_GRID_PAGE_NUM - 1;
        }
        move_direct = MMIWIDGET_PEN_MOVE_RIGHT;
        result = MMI_RESULT_TRUE;
    }
    PlaySlideAnim(idle_grid_ptr, move_direct, dest_page_index, 2);                    


    return result;
}



#ifdef LIVE_WALLPAPER_FRAMEWORK_SUPPORT
/*****************************************************************************/
//  Description : MMIAPIGRID_GetSlideTimerID
//  Global resource dependence : 
//  Author:arvin.wu
//  Note:  return timer id
/*****************************************************************************/
PUBLIC uint8 MMIAPIGRID_GetSlideTimerID(void)
{
    MMI_IDLE_GRID_DATA_T *idle_grid_ptr = MMIAPIGRID_GetGridPtr();

    if(0 == idle_grid_ptr)
    {
        return 0;
    }
    return idle_grid_ptr->slide_anim.timer_id;
}

/*****************************************************************************/
//  Description : MMIAPIGRID_GetMainAppFgInfo
//  Global resource dependence : 
//  Author:arvin.wu
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIGRID_GetMainAppFgInfo(LW_FG_INFO *lw_fg_info_ptr)
{
    MMI_IDLE_GRID_DATA_T *idle_grid_ptr = PNULL;
    uint16 lcd_width = 0;
    uint16 lcd_height = 0;
    int16 offset_x = 0;
    int16 offset_y = 0;
    
    idle_grid_ptr = MMIAPIGRID_GetGridPtr();


    if(idle_grid_ptr && lw_fg_info_ptr)
    {
        lw_fg_info_ptr->disp_dev_info.lcd_id = 0;
        lw_fg_info_ptr->disp_dev_info.block_id = 0;

        GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &lcd_width, &lcd_height); 
        
        lw_fg_info_ptr->lw_fg_pos_info.rc.left = 0;
        lw_fg_info_ptr->lw_fg_pos_info.rc.right = (MMI_GRID_PAGE_NUM*lcd_width);
        lw_fg_info_ptr->lw_fg_pos_info.rc.top = 0;
        lw_fg_info_ptr->lw_fg_pos_info.rc.bottom = lcd_height ;
        GetPageLayerPosition( idle_grid_ptr,  &idle_grid_ptr->page_layer, &offset_x,&offset_y);
		offset_x = -offset_x +  idle_grid_ptr->cur_page_index * GetWidgetPageWidth(idle_grid_ptr);
        lw_fg_info_ptr->lw_fg_pos_info.x = offset_x;
        lw_fg_info_ptr->lw_fg_pos_info.y = 0;
    }
    
}
#endif
#ifdef MMI_IDLE_THUMBVIEW_SLIDE
/*****************************************************************************/
//  Description :  
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsInThumbviewSlideRange(  GUI_POINT_T point)
{
    uint16 img_w = 0;
    uint16 img_h = 0;
    uint16 lcd_width = 0;
    uint16 lcd_height = 0;
    MMI_IDLE_GRID_DATA_T *idle_grid_ptr = MMIAPIGRID_GetGridPtr();
    GUI_RECT_T     title_rect = GetPageTitleBarRect(idle_grid_ptr);
    
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);
    GUIRES_GetImgWidthHeight(&img_w, &img_h,IMAGE_GRID_PAGE_IDX,idle_grid_ptr->win_id);
    
    title_rect.left=(lcd_width - MMI_GRID_PAGE_NUM * img_w)/2;
    title_rect.right=title_rect.left+MMI_GRID_PAGE_NUM * img_w;
    
    if(GUI_PointIsInRect(point,title_rect))
    {
        return TRUE;
    }
    return FALSE;
}

/*****************************************************************************/
//  Description :  
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
LOCAL uint8 GetCurThumbviewPage(GUI_POINT_T point)
{
    GUI_RECT_T     title_rect = MMI_GRID_PAGE_TITLE_RECT;
    uint16 img_w = 0;
    uint16 img_h = 0;
    uint16 lcd_width = 0;
    uint16 lcd_height = 0;
    uint8 cur_thumbview_page=0;
    MMI_IDLE_GRID_DATA_T *idle_grid_ptr = MMIAPIGRID_GetGridPtr();
    
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);
    GUIRES_GetImgWidthHeight(&img_w, &img_h,IMAGE_GRID_PAGE_IDX,idle_grid_ptr->win_id);
    
    title_rect.left=(lcd_width - MMI_GRID_PAGE_NUM * img_w)/2;
    title_rect.right=title_rect.left+MMI_GRID_PAGE_NUM * img_w;
    
    if(point.x<=title_rect.left)
    {
        cur_thumbview_page=0;
    }
    else if(point.x>=title_rect.right)
    {
        cur_thumbview_page=MMI_GRID_PAGE_NUM-1;
    }
    else
    {
        cur_thumbview_page=(point.x-title_rect.left)/img_w;
    }
    return cur_thumbview_page;
}

/*****************************************************************************/
//  Description :  
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
LOCAL void CalcCurThumbviewPageCenterPosition(GUI_POINT_T point,GUI_POINT_T*out_point)
{
    GUI_RECT_T     title_rect = MMI_GRID_PAGE_TITLE_RECT;
    uint16 img_w = 0;
    uint16 img_h = 0;
    uint16 lcd_width = 0;
    uint16 lcd_height = 0;
    MMI_IDLE_GRID_DATA_T *idle_grid_ptr = MMIAPIGRID_GetGridPtr();
    int32 offset=0;
    int32 offset_2=0;
    uint8 cur_page=0;
    
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);
    GUIRES_GetImgWidthHeight(&img_w, &img_h,IMAGE_GRID_PAGE_IDX,idle_grid_ptr->win_id);
    cur_page=GetCurThumbviewPage(point);
    
    title_rect.left=(lcd_width - MMI_GRID_PAGE_NUM * img_w)/2;
    title_rect.right=title_rect.left+MMI_GRID_PAGE_NUM * img_w;
    
    if(point.x<(title_rect.left+img_w/2))
    {
        point.x=(title_rect.left+img_w/2);
    }
    else if(point.x>(title_rect.right-img_w/2))
    {
        point.x=(title_rect.right-img_w/2);
    }
    
    offset=(point.x-title_rect.left-img_w/2)*MMI_THUMBVIEW_CENTER_INTERNAL/img_w;
    
    offset_2=(MMI_GRID_PAGE_NUM-cur_page)*MMI_THUMBVIEW_CENTER_INTERNAL+offset;
    if(offset_2>=MMI_GRID_PAGE_NUM*MMI_THUMBVIEW_CENTER_INTERNAL)
    {
        offset_2=2*MMI_GRID_PAGE_NUM*MMI_THUMBVIEW_CENTER_INTERNAL-offset_2;
        out_point->x=lcd_width/2-MMI_THUMBVIEW_TOTAL_LEN*(MMI_THUMBVIEW_TOTAL_LEN-offset_2)/(2*MMI_THUMBVIEW_TOTAL_LEN-offset_2);
    }
    else
    {
        out_point->x=lcd_width/2+MMI_THUMBVIEW_TOTAL_LEN*(MMI_THUMBVIEW_TOTAL_LEN-offset_2)/(2*MMI_THUMBVIEW_TOTAL_LEN-offset_2);
    }

    out_point->y=MMI_THUMBVIEW_CENTER_Y;
}

/*****************************************************************************/
//  Description :  
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
LOCAL void CalcPreThumbviewPageCenterPosition(GUI_POINT_T point,GUI_POINT_T*out_point)
{
    GUI_RECT_T     title_rect = MMI_GRID_PAGE_TITLE_RECT;
    uint16 img_w = 0;
    uint16 img_h = 0;
    uint16 lcd_width = 0;
    uint16 lcd_height = 0;
    MMI_IDLE_GRID_DATA_T *idle_grid_ptr = MMIAPIGRID_GetGridPtr();
    int16 offset=0;
    int16 offset_2=0;
    uint8 cur_page=0;

    cur_page=GetCurThumbviewPage(point);
    if(cur_page!=0)
    {
        GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);
        GUIRES_GetImgWidthHeight(&img_w, &img_h,IMAGE_GRID_PAGE_IDX,idle_grid_ptr->win_id);
        
        
        title_rect.left=(lcd_width - MMI_GRID_PAGE_NUM * img_w)/2;
        title_rect.right=title_rect.left+MMI_GRID_PAGE_NUM * img_w;
        
        if(point.x<(title_rect.left+img_w/2))
        {
            point.x=(title_rect.left+img_w/2);
        }
        else if(point.x>(title_rect.right-img_w/2))
        {
            point.x=(title_rect.right-img_w/2);
        }
        
        offset=(point.x-title_rect.left-img_w/2)*MMI_THUMBVIEW_CENTER_INTERNAL/img_w;
        
        offset_2=(MMI_GRID_PAGE_NUM-(cur_page-1))*MMI_THUMBVIEW_CENTER_INTERNAL+offset;
        if(offset_2>=MMI_GRID_PAGE_NUM*MMI_THUMBVIEW_CENTER_INTERNAL)
        {
            offset_2=2*MMI_GRID_PAGE_NUM*MMI_THUMBVIEW_CENTER_INTERNAL-offset_2;
            out_point->x=lcd_width/2-MMI_THUMBVIEW_TOTAL_LEN*(MMI_THUMBVIEW_TOTAL_LEN-offset_2)/(2*MMI_THUMBVIEW_TOTAL_LEN-offset_2);
        }
        else
        {
            out_point->x=lcd_width/2+MMI_THUMBVIEW_TOTAL_LEN*(MMI_THUMBVIEW_TOTAL_LEN-offset_2)/(2*MMI_THUMBVIEW_TOTAL_LEN-offset_2);
        }

        out_point->y=MMI_THUMBVIEW_CENTER_Y;
    }
    else
    {
       out_point->x=-0xffff; 
       out_point->y=MMI_THUMBVIEW_CENTER_Y;
    }
}

/*****************************************************************************/
//  Description :  
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
LOCAL void CalcNextThumbviewPageCenterPosition(GUI_POINT_T point,GUI_POINT_T*out_point)
{
    GUI_RECT_T     title_rect = MMI_GRID_PAGE_TITLE_RECT;
    uint16 img_w = 0;
    uint16 img_h = 0;
    uint16 lcd_width = 0;
    uint16 lcd_height = 0;
    MMI_IDLE_GRID_DATA_T *idle_grid_ptr = MMIAPIGRID_GetGridPtr();
    int16 offset=0;
    int16 offset_2=0;
    uint8 cur_page=0;

    cur_page=GetCurThumbviewPage(point);
    if(cur_page!=MMI_GRID_PAGE_NUM-1)
    {
        GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);
        GUIRES_GetImgWidthHeight(&img_w, &img_h,IMAGE_GRID_PAGE_IDX,idle_grid_ptr->win_id);
        
        
        title_rect.left=(lcd_width - MMI_GRID_PAGE_NUM * img_w)/2;
        title_rect.right=title_rect.left+MMI_GRID_PAGE_NUM * img_w;
        
        if(point.x<(title_rect.left+img_w/2))
        {
            point.x=(title_rect.left+img_w/2);
        }
        else if(point.x>(title_rect.right-img_w/2))
        {
            point.x=(title_rect.right-img_w/2);
        }
        
        offset=(point.x-title_rect.left-img_w/2)*MMI_THUMBVIEW_CENTER_INTERNAL/img_w;
        
        offset_2=(MMI_GRID_PAGE_NUM-(cur_page+1))*MMI_THUMBVIEW_CENTER_INTERNAL+offset;
        if(offset_2>=MMI_GRID_PAGE_NUM*MMI_THUMBVIEW_CENTER_INTERNAL)
        {
            offset_2=2*MMI_GRID_PAGE_NUM*MMI_THUMBVIEW_CENTER_INTERNAL-offset_2;
            out_point->x=lcd_width/2-MMI_THUMBVIEW_TOTAL_LEN*(MMI_THUMBVIEW_TOTAL_LEN-offset_2)/(2*MMI_THUMBVIEW_TOTAL_LEN-offset_2);
        }
        else
        {
            out_point->x=lcd_width/2+MMI_THUMBVIEW_TOTAL_LEN*(MMI_THUMBVIEW_TOTAL_LEN-offset_2)/(2*MMI_THUMBVIEW_TOTAL_LEN-offset_2);
        }

        out_point->y=MMI_THUMBVIEW_CENTER_Y;
    }
    else
    {
       out_point->x=0xffff; 
       out_point->y=MMI_THUMBVIEW_CENTER_Y;
    }
}

/*****************************************************************************/
//  Description :  
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
LOCAL void GetCurThumbviewPageSize(GUI_POINT_T point,uint16* width,uint16* height)
{
    GUI_RECT_T     title_rect = MMI_GRID_PAGE_TITLE_RECT;
    uint16 img_w = 0;
    uint16 img_h = 0;
    uint16 lcd_width = 0;
    uint16 lcd_height = 0;
    MMI_IDLE_GRID_DATA_T *idle_grid_ptr = MMIAPIGRID_GetGridPtr();
    int32 offset=0;
    int32 offset_2=0;
    uint8 cur_page=0;
    
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);
    GUIRES_GetImgWidthHeight(&img_w, &img_h,IMAGE_GRID_PAGE_IDX,idle_grid_ptr->win_id);
    cur_page=GetCurThumbviewPage(point);
    
    title_rect.left=(lcd_width - MMI_GRID_PAGE_NUM * img_w)/2;
    title_rect.right=title_rect.left+MMI_GRID_PAGE_NUM * img_w;
    
    if(point.x<(title_rect.left+img_w/2))
    {
        point.x=(title_rect.left+img_w/2);
    }
    else if(point.x>(title_rect.right-img_w/2))
    {
        point.x=(title_rect.right-img_w/2);
    }
    
    offset=(point.x-title_rect.left-img_w/2)*MMI_THUMBVIEW_CENTER_INTERNAL/img_w;
    
    offset_2=(MMI_GRID_PAGE_NUM-cur_page)*MMI_THUMBVIEW_CENTER_INTERNAL+offset;
    if(offset_2>=MMI_GRID_PAGE_NUM*MMI_THUMBVIEW_CENTER_INTERNAL)
    {
        offset_2=2*MMI_GRID_PAGE_NUM*MMI_THUMBVIEW_CENTER_INTERNAL-offset_2;
        *width=MMI_THUMBVIEW_MIN_W+ (((MMI_THUMBVIEW_MAX_W-MMI_THUMBVIEW_MIN_W)*offset_2)/MMI_THUMBVIEW_TOTAL_LEN);
        *height=MMI_THUMBVIEW_MIN_H+ (((MMI_THUMBVIEW_MAX_H-MMI_THUMBVIEW_MIN_H)*offset_2)/MMI_THUMBVIEW_TOTAL_LEN);
    }
    else
    {
        *width=MMI_THUMBVIEW_MIN_W+ (((MMI_THUMBVIEW_MAX_W-MMI_THUMBVIEW_MIN_W)*offset_2)/MMI_THUMBVIEW_TOTAL_LEN);
        *height=MMI_THUMBVIEW_MIN_H+ (((MMI_THUMBVIEW_MAX_H-MMI_THUMBVIEW_MIN_H)*offset_2)/MMI_THUMBVIEW_TOTAL_LEN);
    }

}

/*****************************************************************************/
//  Description :  
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
LOCAL void GetPreThumbviewPageSize(GUI_POINT_T point,uint16* width,uint16* height)
{
    GUI_RECT_T     title_rect = MMI_GRID_PAGE_TITLE_RECT;
    uint16 img_w = 0;
    uint16 img_h = 0;
    uint16 lcd_width = 0;
    uint16 lcd_height = 0;
    MMI_IDLE_GRID_DATA_T *idle_grid_ptr = MMIAPIGRID_GetGridPtr();
    int32 offset=0;
    int32 offset_2=0;
    uint8 cur_page=0;
    
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);
    GUIRES_GetImgWidthHeight(&img_w, &img_h,IMAGE_GRID_PAGE_IDX,idle_grid_ptr->win_id);
    cur_page=GetCurThumbviewPage(point);
    if(cur_page!=0)
    {
        title_rect.left=(lcd_width - MMI_GRID_PAGE_NUM * img_w)/2;
        title_rect.right=title_rect.left+MMI_GRID_PAGE_NUM * img_w;
        
        if(point.x<(title_rect.left+img_w/2))
        {
            point.x=(title_rect.left+img_w/2);
        }
        else if(point.x>(title_rect.right-img_w/2))
        {
            point.x=(title_rect.right-img_w/2);
        }
        
        offset=(point.x-title_rect.left-img_w/2)*MMI_THUMBVIEW_CENTER_INTERNAL/img_w;
        
        offset_2=(MMI_GRID_PAGE_NUM-cur_page+1)*MMI_THUMBVIEW_CENTER_INTERNAL+offset;
        if(offset_2>=MMI_GRID_PAGE_NUM*MMI_THUMBVIEW_CENTER_INTERNAL)
        {
            offset_2=2*MMI_GRID_PAGE_NUM*MMI_THUMBVIEW_CENTER_INTERNAL-offset_2;
            *width=MMI_THUMBVIEW_MIN_W+ (((MMI_THUMBVIEW_MAX_W-MMI_THUMBVIEW_MIN_W)*offset_2)/MMI_THUMBVIEW_TOTAL_LEN);
            *height=MMI_THUMBVIEW_MIN_H+ (((MMI_THUMBVIEW_MAX_H-MMI_THUMBVIEW_MIN_H)*offset_2)/MMI_THUMBVIEW_TOTAL_LEN);
        }
        else
        {
            *width=MMI_THUMBVIEW_MIN_W+ (((MMI_THUMBVIEW_MAX_W-MMI_THUMBVIEW_MIN_W)*offset_2)/MMI_THUMBVIEW_TOTAL_LEN);
            *height=MMI_THUMBVIEW_MIN_H+ (((MMI_THUMBVIEW_MAX_H-MMI_THUMBVIEW_MIN_H)*offset_2)/MMI_THUMBVIEW_TOTAL_LEN);
        }
    }
    else
    {
        *width=MMI_THUMBVIEW_MIN_W;
        *height=MMI_THUMBVIEW_MIN_H;
    }

}

/*****************************************************************************/
//  Description :  
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
LOCAL void GetNextThumbviewPageSize(GUI_POINT_T point,uint16* width,uint16* height)
{
    GUI_RECT_T     title_rect = MMI_GRID_PAGE_TITLE_RECT;
    uint16 img_w = 0;
    uint16 img_h = 0;
    uint16 lcd_width = 0;
    uint16 lcd_height = 0;
    MMI_IDLE_GRID_DATA_T *idle_grid_ptr = MMIAPIGRID_GetGridPtr();
    int32 offset=0;
    int32 offset_2=0;
    uint8 cur_page=0;
    
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);
    GUIRES_GetImgWidthHeight(&img_w, &img_h,IMAGE_GRID_PAGE_IDX,idle_grid_ptr->win_id);
    cur_page=GetCurThumbviewPage(point);
    if(cur_page!=MMI_GRID_PAGE_NUM-1)
    {
        title_rect.left=(lcd_width - MMI_GRID_PAGE_NUM * img_w)/2;
        title_rect.right=title_rect.left+MMI_GRID_PAGE_NUM * img_w;
        
        if(point.x<(title_rect.left+img_w/2))
        {
            point.x=(title_rect.left+img_w/2);
        }
        else if(point.x>(title_rect.right-img_w/2))
        {
            point.x=(title_rect.right-img_w/2);
        }
        
        offset=(point.x-title_rect.left-img_w/2)*MMI_THUMBVIEW_CENTER_INTERNAL/img_w;
        
        offset_2=(MMI_GRID_PAGE_NUM-cur_page-1)*MMI_THUMBVIEW_CENTER_INTERNAL+offset;
        if(offset_2>=MMI_GRID_PAGE_NUM*MMI_THUMBVIEW_CENTER_INTERNAL)
        {
            offset_2=2*MMI_GRID_PAGE_NUM*MMI_THUMBVIEW_CENTER_INTERNAL-offset_2;
            *width=MMI_THUMBVIEW_MIN_W+ (((MMI_THUMBVIEW_MAX_W-MMI_THUMBVIEW_MIN_W)*offset_2)/MMI_THUMBVIEW_TOTAL_LEN);
            *height=MMI_THUMBVIEW_MIN_H+ (((MMI_THUMBVIEW_MAX_H-MMI_THUMBVIEW_MIN_H)*offset_2)/MMI_THUMBVIEW_TOTAL_LEN);
        }
        else
        {
            *width=MMI_THUMBVIEW_MIN_W+ (((MMI_THUMBVIEW_MAX_W-MMI_THUMBVIEW_MIN_W)*offset_2)/MMI_THUMBVIEW_TOTAL_LEN);
            *height=MMI_THUMBVIEW_MIN_H+ (((MMI_THUMBVIEW_MAX_H-MMI_THUMBVIEW_MIN_H)*offset_2)/MMI_THUMBVIEW_TOTAL_LEN);
        }
    }
    else
    {
        *width=MMI_THUMBVIEW_MIN_W;
        *height=MMI_THUMBVIEW_MIN_H;
    }

}


/*****************************************************************************/
//  Description :  
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
LOCAL void CalcCurThumbviewPagePosition(GUI_POINT_T point,GUI_POINT_T*out_point)
{
    GUI_POINT_T cur_center_point={0};
    uint16 w=0;
    uint16 h=0;
    CalcCurThumbviewPageCenterPosition(point,&cur_center_point);
    GetCurThumbviewPageSize(point,&w,&h);
    out_point->x=cur_center_point.x-w/2;
    out_point->y=cur_center_point.y-h/2;
}

/*****************************************************************************/
//  Description :  
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
LOCAL void CalcPreThumbviewPagePosition(GUI_POINT_T point,GUI_POINT_T*out_point)
{
    GUI_POINT_T center_point={0};
    uint16 w=0;
    uint16 h=0;
    uint16 pre_w=0;
    uint16 pre_h=0;
    CalcPreThumbviewPageCenterPosition(point,&center_point);
    GetPreThumbviewPageSize(point,&pre_w,&pre_h);
    out_point->x=center_point.x-pre_w/2;
    out_point->y=center_point.y-pre_h/2;
}

/*****************************************************************************/
//  Description :  
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
LOCAL void CalcNextThumbviewPagePosition(GUI_POINT_T point,GUI_POINT_T*out_point)
{
    GUI_POINT_T center_point={0};
    uint16 w=0;
    uint16 h=0;
    uint16 next_w=0;
    uint16 next_h=0;
    CalcNextThumbviewPageCenterPosition(point,&center_point);
    GetNextThumbviewPageSize(point,&next_w,&next_h);
    out_point->x=center_point.x-next_w/2;
    out_point->y=center_point.y-next_h/2;
}

/*****************************************************************************/
//  Description :  
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
LOCAL uint32 CalcCurThumbviewAlpha(GUI_POINT_T point)
{
    GUI_POINT_T cur_center_point={0};
    uint16 lcd_width = 0;
    uint16 lcd_height = 0;
    int32  offset=0;
    uint32 alpha=0;
    
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);
    CalcCurThumbviewPageCenterPosition(point,&cur_center_point);
    
    offset=MMI_THUMBVIEW_MAX_ALPHA-abs(cur_center_point.x-lcd_width/2)*(MMI_THUMBVIEW_MAX_ALPHA-MMI_THUMBVIEW_MIN_ALPHA)/lcd_width;
    if(offset<MMI_THUMBVIEW_MIN_ALPHA)
    {
        offset=MMI_THUMBVIEW_MIN_ALPHA;
    }
    alpha=offset;
    return alpha;
}

/*****************************************************************************/
//  Description :  
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
LOCAL uint32 CalcPreThumbviewAlpha(GUI_POINT_T point)
{
    GUI_POINT_T cur_center_point={0};
    uint16 lcd_width = 0;
    uint16 lcd_height = 0;
    int32  offset=0;
    uint32 alpha=0;
    
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);
    CalcPreThumbviewPageCenterPosition(point,&cur_center_point);
    
    offset=MMI_THUMBVIEW_MAX_ALPHA-abs(cur_center_point.x-lcd_width/2)*(MMI_THUMBVIEW_MAX_ALPHA-MMI_THUMBVIEW_MIN_ALPHA)/lcd_width;
    if(offset<MMI_THUMBVIEW_MIN_ALPHA)
    {
        offset=MMI_THUMBVIEW_MIN_ALPHA;
    }
    alpha=offset;
    return alpha;
}

/*****************************************************************************/
//  Description :  
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
LOCAL uint32 CalcNextThumbviewAlpha(GUI_POINT_T point)
{
    GUI_POINT_T cur_center_point={0};
    uint16 lcd_width = 0;
    uint16 lcd_height = 0;
    int32  offset=0;
    uint32 alpha=0;
    
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);
    CalcNextThumbviewPageCenterPosition(point,&cur_center_point);
    
    offset=MMI_THUMBVIEW_MAX_ALPHA-abs(cur_center_point.x-lcd_width/2)*(MMI_THUMBVIEW_MAX_ALPHA-MMI_THUMBVIEW_MIN_ALPHA)/lcd_width;
    if(offset<MMI_THUMBVIEW_MIN_ALPHA)
    {
        offset=MMI_THUMBVIEW_MIN_ALPHA;
    }
    alpha=offset;
    return alpha;
}
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void CreateThumbViewSlideLayer(MMI_IDLE_GRID_DATA_T *idle_grid_ptr)
{
    uint16           lcd_width           = 0;
    uint16           lcd_height          = 0;
    GUI_LCD_DEV_INFO widget_layer_handle = {0};
    UILAYER_CREATE_T create_info         = {0}; 

    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &lcd_width, &lcd_height); 

    if (UILAYER_HANDLE_MULTI != UILAYER_GetHandleType(&idle_grid_ptr->thumb_view_slide_layer))
    {
        create_info.lcd_id          = LCD_ID_0;
        create_info.owner_handle    = idle_grid_ptr->win_id;
        create_info.offset_x        = 0;
        create_info.offset_y        = 0;
        create_info.width           = lcd_width;
        create_info.height          = lcd_height;
        create_info.is_bg_layer     = FALSE;
        create_info.is_static_layer = FALSE;   
        
        UILAYER_CreateLayer(&create_info, &widget_layer_handle);
            
        UILAYER_SetLayerColorKey(&widget_layer_handle, TRUE, UILAYER_TRANSPARENT_COLOR);
    
        idle_grid_ptr->thumb_view_slide_layer = widget_layer_handle;
    }
    
    if (UILAYER_IsLayerActive(&widget_layer_handle))
    {
        UILAYER_Clear(&widget_layer_handle);   
    }
}
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void DestroyGridThumbViewSlideLayer(MMI_IDLE_GRID_DATA_T *idle_grid_ptr)
{
    if (PNULL == idle_grid_ptr)
    {
        //SCI_TRACE_LOW:"DestroyMoveLayer() PNULL == idle_grid_ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIGRID_IDLE_10859_112_2_18_3_7_21_45,(uint8*)"");
        return;
    }
 
    if (UILAYER_HANDLE_MULTI == UILAYER_GetHandleType(&idle_grid_ptr->thumb_view_slide_layer))    
    {
        UILAYER_RELEASELAYER(&(idle_grid_ptr->thumb_view_slide_layer));   /*lint !e506 !e774*/
    }
}


/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:  
//  Note:
/*****************************************************************************/
LOCAL void DisplayGridSlidePageThumbView(MMI_IDLE_GRID_DATA_T *idle_grid_ptr,GUI_POINT_T point)
{
    SCALE_IMAGE_IN_T    scale_in  = {0};
    SCALE_IMAGE_OUT_T   scale_out = {0};
    UILAYER_INFO_T      layer_info = {0};
    uint8 *layer_buf_ptr = PNULL;
    GUI_LCD_DEV_INFO    thumb_layer = {0};
    GUI_RECT_T          blt_rect   = {0};
    UILAYER_APPEND_BLT_T append_layer = {0};
    uint8 *temp_buf   = PNULL;
    uint16 lcd_width  = 0;
    uint16 lcd_height = 0;
    uint8 *for_ptr   = PNULL;
    uint8 *tar_buf   = PNULL; 
    uint8 cur_page=0;
#ifndef LOW_MEMORY_SUPPORT
    uint32 cur_index=0;
    uint32 pre_index=0;
    uint32 next_index=0;
#endif
    uint16 width=0;
    uint16 height=0;
    GUI_POINT_T out_point={0};
    uint8 *bg_layer_buf_ptr = PNULL;
    UILAYER_COPYBUF_PARAM_T     copy_param = {0};
    UILAYER_COPYBUF_T           copy_buf = {0};
    GUI_POINT_T start_point={0};
#if defined LOW_MEMORY_SUPPORT
    uint8 old_page=idle_grid_ptr->cur_page_index;
#endif
    UILAYER_RemoveBltLayer(&idle_grid_ptr->page_layer);

    if(point.x==idle_grid_ptr->save_point.x)
    {
        return ;
    }

    if(!UILAYER_IsLayerActive(&idle_grid_ptr->thumb_view_slide_layer))
    {
        return ;
    }
    UILAYER_Clear(&idle_grid_ptr->thumb_view_slide_layer);
    DrawSlidePageTitleBar(point);
    

    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height); 
    
    temp_buf = SCI_ALLOCA(lcd_width * lcd_height * 4);
    if (PNULL == temp_buf)
    {
        return;
    }
    

    CreateThumbViewSlideBgLayer(idle_grid_ptr);
    GUIRES_DisplayImg(&start_point, 
                      PNULL, 
                      PNULL, 
                      idle_grid_ptr->win_id,
                      IMGGE_THUMBVIEW_BG, 
                      &idle_grid_ptr->thumb_view_slide_bg_layer); 

    bg_layer_buf_ptr = UILAYER_GetLayerBufferPtr(&idle_grid_ptr->thumb_view_slide_bg_layer);      

    cur_page=GetCurThumbviewPage(point);
#ifdef LOW_MEMORY_SUPPORT
    idle_grid_ptr->cur_page_index=cur_page;
    DrawPageLayer(idle_grid_ptr);
#else
    if(idle_grid_ptr->cur_page_index!=cur_page)
    {
        idle_grid_ptr->cur_page_index=cur_page;
        SetPageLayerPosition( idle_grid_ptr, &idle_grid_ptr->page_layer, 0, 0);
        DrawPageLayer(idle_grid_ptr);
    }
    if(0==idle_grid_ptr->cur_page_index)
    {
        cur_index=0;
        next_index=cur_index+1;
    }
    else if((MMI_GRID_PAGE_NUM - 1)==idle_grid_ptr->cur_page_index)
    {
        cur_index=MMI_GRID_PAGE_LAYER_NUM-1;
        pre_index=cur_index-1;
    }
    else
    {
        cur_index=MMI_GRID_PAGE_LAYER_NUM/2;
        next_index=cur_index+1;
        pre_index=cur_index-1;
    }
#endif
    GetCurThumbviewPageSize(point,&width,&height);
    tar_buf = SCI_ALLOCA(width * height * 4 + 255);
    if (PNULL == tar_buf)
    {
        SCI_FREE(temp_buf); 
        return;
    }
    SCI_MEMSET(tar_buf, 0, width * height * 4 + 255);

    scale_in.src_format           = IMGREF_FORMAT_ARGB888;
    scale_in.src_chn.chn0.ptr     = bg_layer_buf_ptr;
    scale_in.src_chn.chn0.size    = lcd_width * lcd_height * 4 ;
    scale_in.src_size.w           = lcd_width;
    scale_in.src_size.h           = lcd_height;        
    scale_in.src_trim_rect.x      = 0;
    scale_in.src_trim_rect.y      = 0;
    scale_in.src_trim_rect.w      = lcd_width;
    scale_in.src_trim_rect.h      = lcd_height;
    scale_in.target_buf.size      = width * height * 4 + 255;          
    scale_in.target_buf.ptr       = tar_buf;  
    scale_in.target_format        = IMGREF_FORMAT_ARGB888;
    scale_in.target_size.w        = width;
    scale_in.target_size.h        = height;
    scale_in.scale_mode           =SCALE_MODE_DISTORT;

    if (SCI_SUCCESS == GRAPH_ScaleImage(&scale_in,&scale_out))
    {    
        for_ptr  = (uint8*)scale_out.output_chn.chn0.ptr;
           
        CalcCurThumbviewPagePosition(point,&out_point); 
        blt_rect.left=out_point.x;
        blt_rect.right=blt_rect.left+width-1;
        blt_rect.top=out_point.y;
        blt_rect.bottom=blt_rect.top+height-1;
        
        copy_param.rect = blt_rect;
        copy_buf.buf_ptr = (uint8*)for_ptr;
        copy_buf.data_type = DATA_TYPE_ARGB888;
        copy_buf.width = width;
        copy_buf.height = height;
        UILAYER_CopyBufferToLayer(&idle_grid_ptr->thumb_view_slide_layer, &copy_param, &copy_buf);
#ifdef LOW_MEMORY_SUPPORT
        CopyPage2Buffer(idle_grid_ptr, temp_buf, 0);
#else
        CopyPage2Buffer(idle_grid_ptr, temp_buf, cur_index);
#endif
        thumb_layer=CreateSinglePageThumbLayer(idle_grid_ptr,width,height);
        UILAYER_GetLayerInfo(&thumb_layer, &layer_info); 
        layer_buf_ptr = UILAYER_GetLayerBufferPtr(&thumb_layer);
        
        scale_in.src_chn.chn0.ptr     = temp_buf;
        SCI_MEMSET(tar_buf, 0, width * height * 4 + 255);
        GRAPH_ScaleImage(&scale_in,&scale_out);
        for_ptr  = (uint8*)scale_out.output_chn.chn0.ptr;
        BltBufferWithAlpha(for_ptr, 
                  scale_out.output_size.w, 
                  DATA_TYPE_ARGB888, 
                  layer_buf_ptr, 
                  layer_info.mem_width,
                  DATA_TYPE_ARGB888,
                  scale_out.output_size.w, 
                  scale_out.output_size.h,
                  CalcCurThumbviewAlpha(point));
        UILAYER_SetLayerPosition(&thumb_layer, out_point.x, out_point.y);
        UILAYER_BltLayerToLayer(&idle_grid_ptr->thumb_view_slide_layer,
                                &thumb_layer,
                                &blt_rect,
                                TRUE);            
    }
    
    SCI_FREE(tar_buf);
    UILAYER_RELEASELAYER(&thumb_layer);
        
    if(0!=idle_grid_ptr->cur_page_index)
    {
        GetPreThumbviewPageSize(point,&width,&height);
        tar_buf = SCI_ALLOCA(width * height * 4 + 255);
        if (PNULL == tar_buf)
        {
            SCI_FREE(temp_buf);
            return;
        }
        SCI_MEMSET(tar_buf, 0, width * height * 4 + 255);

        scale_in.src_chn.chn0.ptr     = bg_layer_buf_ptr;
        scale_in.target_buf.size      = width * height * 4 + 255;          
        scale_in.target_buf.ptr       = tar_buf;  
        scale_in.target_size.w        = width;
        scale_in.target_size.h        = height;

        if (SCI_SUCCESS == GRAPH_ScaleImage(&scale_in,&scale_out))
        {
            for_ptr  = (uint8*)scale_out.output_chn.chn0.ptr;
            
            CalcPreThumbviewPagePosition(point,&out_point); 
            blt_rect.left=out_point.x;
            blt_rect.right=blt_rect.left+width-1;
            blt_rect.top=out_point.y;
            blt_rect.bottom=blt_rect.top+height-1;

            copy_param.rect = blt_rect;
            copy_buf.buf_ptr = (uint8*)for_ptr;
            copy_buf.data_type = DATA_TYPE_ARGB888;
            copy_buf.width = width;
            copy_buf.height = height;
            UILAYER_CopyBufferToLayer(&idle_grid_ptr->thumb_view_slide_layer, &copy_param, &copy_buf);
#ifdef LOW_MEMORY_SUPPORT
            old_page=idle_grid_ptr->cur_page_index;
            idle_grid_ptr->cur_page_index-=1;
            DrawPageLayer(idle_grid_ptr);
            CopyPage2Buffer(idle_grid_ptr, temp_buf, 0);
            idle_grid_ptr->cur_page_index=old_page;
#else
            CopyPage2Buffer(idle_grid_ptr, temp_buf, pre_index);
#endif
            thumb_layer=CreateSinglePageThumbLayer(idle_grid_ptr,width,height);
            UILAYER_GetLayerInfo(&thumb_layer, &layer_info); 
            layer_buf_ptr = UILAYER_GetLayerBufferPtr(&thumb_layer);
            scale_in.src_chn.chn0.ptr     = temp_buf;
            SCI_MEMSET(tar_buf, 0, width * height * 4 + 255);
            GRAPH_ScaleImage(&scale_in,&scale_out);
            for_ptr  = (uint8*)scale_out.output_chn.chn0.ptr;

            BltBufferWithAlpha(for_ptr, 
                      scale_out.output_size.w, 
                      DATA_TYPE_ARGB888, 
                      layer_buf_ptr, 
                      layer_info.mem_width,
                      DATA_TYPE_ARGB888,
                      scale_out.output_size.w, 
                      scale_out.output_size.h,
                      CalcPreThumbviewAlpha(point));

            UILAYER_SetLayerPosition(&thumb_layer, out_point.x, out_point.y);
            UILAYER_BltLayerToLayer(&idle_grid_ptr->thumb_view_slide_layer,
                                    &thumb_layer,
                                    &blt_rect,
                                    TRUE);            
        }
        
        SCI_FREE(tar_buf);
        UILAYER_RELEASELAYER(&thumb_layer);
    }

    if((MMI_GRID_PAGE_NUM - 1)!=idle_grid_ptr->cur_page_index)
    {
        GetNextThumbviewPageSize(point,&width,&height);
        tar_buf = SCI_ALLOCA(width * height * 4 + 255);
        if (PNULL == tar_buf)
        {
            SCI_FREE(temp_buf);
            return;
        }
        SCI_MEMSET(tar_buf, 0, width * height * 4 + 255);

        scale_in.src_chn.chn0.ptr     = bg_layer_buf_ptr;
        scale_in.target_buf.size      = width * height * 4 + 255;          
        scale_in.target_buf.ptr       = tar_buf;  
        scale_in.target_size.w        = width;
        scale_in.target_size.h        = height;

        if (SCI_SUCCESS == GRAPH_ScaleImage(&scale_in,&scale_out))
        {
            for_ptr  = (uint8*)scale_out.output_chn.chn0.ptr;
            
            CalcNextThumbviewPagePosition(point,&out_point); 
            blt_rect.left=out_point.x;
            blt_rect.right=blt_rect.left+width-1;
            blt_rect.top=out_point.y;
            blt_rect.bottom=blt_rect.top+height-1;

            copy_param.rect = blt_rect;
            copy_buf.buf_ptr = (uint8*)for_ptr;
            copy_buf.data_type = DATA_TYPE_ARGB888;
            copy_buf.width = width;
            copy_buf.height = height;
            UILAYER_CopyBufferToLayer(&idle_grid_ptr->thumb_view_slide_layer, &copy_param, &copy_buf);
#ifdef LOW_MEMORY_SUPPORT
            old_page=idle_grid_ptr->cur_page_index;
            idle_grid_ptr->cur_page_index+=1;
            DrawPageLayer(idle_grid_ptr);
            CopyPage2Buffer(idle_grid_ptr, temp_buf, 0);
            idle_grid_ptr->cur_page_index=old_page;
#else
            CopyPage2Buffer(idle_grid_ptr, temp_buf, next_index);
#endif
            thumb_layer=CreateSinglePageThumbLayer(idle_grid_ptr,width,height);
            UILAYER_GetLayerInfo(&thumb_layer, &layer_info); 
            layer_buf_ptr = UILAYER_GetLayerBufferPtr(&thumb_layer);
            scale_in.src_chn.chn0.ptr     = temp_buf;
            SCI_MEMSET(tar_buf, 0, width * height * 4 + 255);
            GRAPH_ScaleImage(&scale_in,&scale_out);
            for_ptr  = (uint8*)scale_out.output_chn.chn0.ptr;

            BltBufferWithAlpha(for_ptr, 
                      scale_out.output_size.w, 
                      DATA_TYPE_ARGB888, 
                      layer_buf_ptr, 
                      layer_info.mem_width,
                      DATA_TYPE_ARGB888,
                      scale_out.output_size.w, 
                      scale_out.output_size.h,
                      CalcNextThumbviewAlpha(point));

            UILAYER_SetLayerPosition(&thumb_layer, out_point.x, out_point.y);
            UILAYER_BltLayerToLayer(&idle_grid_ptr->thumb_view_slide_layer,
                                    &thumb_layer,
                                    &blt_rect,
                                    TRUE);            
        }
        
        SCI_FREE(tar_buf);
        UILAYER_RELEASELAYER(&thumb_layer);
    }

    SCI_FREE(temp_buf);
    DestroyThumbViewSlideBgLayer(idle_grid_ptr);
 
}
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void DrawSlidePageTitleBar(GUI_POINT_T point)
{
    uint32 i = 0;
    MMI_IDLE_GRID_DATA_T *idle_grid_ptr = MMIAPIGRID_GetGridPtr();
    GUI_POINT_T start_point = {0};
    uint16 img_w = 0;
    uint16 img_h = 0;
    uint16 num_img_w = 0;
    uint16 num_img_h = 0;
    uint16 lcd_width = 0;
    uint16 lcd_height = 0;
    MMI_IMAGE_ID_T img_id = 0;
    GUI_LCD_DEV_INFO lcd_dev_info = {0};
    GUI_RECT_T     title_rect = GetPageTitleBarRect(idle_grid_ptr);
    MMI_IMAGE_ID_T number[MMI_THUMBVIEW_MAX_DIS_PAGE]=
    {
        IMAGE_NUMBER_1,
        IMAGE_NUMBER_2,
        IMAGE_NUMBER_3,
        IMAGE_NUMBER_4,
        IMAGE_NUMBER_5,
        IMAGE_NUMBER_6,
        IMAGE_NUMBER_7,
        IMAGE_NUMBER_8,
        IMAGE_NUMBER_9,
        IMAGE_NUMBER_10,
    };
    uint8 cur_page=0;
    
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);    

    if (UILAYER_IsLayerActive(&idle_grid_ptr->thumb_view_slide_layer))
    {
        lcd_dev_info = idle_grid_ptr->thumb_view_slide_layer;
        
        UILAYER_ClearRect(&lcd_dev_info, title_rect);        
    }

    if(PNULL != idle_grid_ptr)
    {        
        GUIRES_GetImgWidthHeight(&img_w, &img_h,IMAGE_GRID_PAGE_IDX,idle_grid_ptr->win_id);
        GUIRES_GetImgWidthHeight(&num_img_w, &num_img_h,IMAGE_NUMBER_1,idle_grid_ptr->win_id);
        
        for (i = 0; i < MMI_GRID_PAGE_NUM; i++)
        {
            img_id = IMAGE_GRID_PAGE_IDX;
            start_point.x = (lcd_width - MMI_GRID_PAGE_NUM * img_w)/2 + i*img_w;            
            start_point.y = title_rect.top+(title_rect.bottom-title_rect.top-img_h)/2;
            
            GUIRES_DisplayImg(&start_point, 
                              PNULL, 
                              PNULL, 
                              idle_grid_ptr->win_id,
                              img_id, 
                              &lcd_dev_info);            
        }

        img_id = IMAGE_GRID_PAGE_IDX_HL;
        start_point.x = point.x-img_w/2;            
        start_point.y = title_rect.top+(title_rect.bottom-title_rect.top-img_h)/2;
        if(start_point.x<(lcd_width - MMI_GRID_PAGE_NUM * img_w)/2)
        {
           start_point.x=(lcd_width - MMI_GRID_PAGE_NUM * img_w)/2;
        }
        if(start_point.x>(lcd_width - MMI_GRID_PAGE_NUM * img_w)/2+(MMI_GRID_PAGE_NUM-1)*img_w)
        {
            start_point.x=(lcd_width - MMI_GRID_PAGE_NUM * img_w)/2+(MMI_GRID_PAGE_NUM-1)*img_w;
        }
        GUIRES_DisplayImg(&start_point, 
                          PNULL, 
                          PNULL, 
                          idle_grid_ptr->win_id,
                          img_id, 
                          &lcd_dev_info); 
        
        cur_page=GetCurThumbviewPage(point);
        if(cur_page<MMI_THUMBVIEW_MAX_DIS_PAGE)
        {
            start_point.x = title_rect.left+(title_rect.right-title_rect.left)/2-num_img_w/2;            
            start_point.y = title_rect.top-num_img_h-5;
            GUIRES_DisplayImg(&start_point, 
                          PNULL, 
                          PNULL, 
                          idle_grid_ptr->win_id,
                          number[cur_page], 
                          &lcd_dev_info); 
        }
        
        
    }
}
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL GUI_LCD_DEV_INFO CreateSinglePageThumbLayer(MMI_IDLE_GRID_DATA_T *idle_grid_ptr,int16 width,int16 height)
{
    GUI_LCD_DEV_INFO widget_layer_handle = {0, UILAYER_NULL_HANDLE};
    UILAYER_CREATE_T create_info = {0}; 
    GUI_RECT_T       rect = {0};  

    if (PNULL == idle_grid_ptr)
    {
        //SCI_TRACE_LOW:"CreateMoveLayer() PNULL == idle_grid_ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIGRID_IDLE_11293_112_2_18_3_7_22_46,(uint8*)"");
        return widget_layer_handle;
    }
        
    create_info.lcd_id          = LCD_ID_0;
    create_info.owner_handle    = idle_grid_ptr->win_id;
    create_info.offset_x        = 0;
    create_info.offset_y        = 0;
    create_info.width           = width;
    create_info.height          = height;
    create_info.is_bg_layer     = FALSE;
    create_info.is_static_layer = FALSE;   
    
    UILAYER_CreateLayer(&create_info, &widget_layer_handle);
        
    UILAYER_SetLayerColorKey(&widget_layer_handle, TRUE, UILAYER_TRANSPARENT_COLOR);

    
    if (UILAYER_IsLayerActive(&widget_layer_handle))
    {
        UILAYER_Clear(&widget_layer_handle);

        //rect.left = 0;
        //rect.top  = 0;
        //rect.right = rect.left + width - 1;
        //rect.bottom = rect.top + height - 1;
        //GUI_FillRect(&widget_layer_handle, rect, MMI_GRAY_COLOR);
    }

    return widget_layer_handle; 
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN HandleSlideThumbViewPageTpMove(
                                 MMI_IDLE_GRID_DATA_T *idle_grid_ptr,
                                 GUI_POINT_T point
                                 )
{
    
    if (PNULL == idle_grid_ptr)
    {
        return FALSE;
    }
    
    DisplayGridSlidePageThumbView(idle_grid_ptr,point);
    SaveTpMovePoint(idle_grid_ptr, point);

    return TRUE;
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void BltBufferWithAlpha(
                     uint8 *src_buf_ptr,
                     uint32 src_buf_width,
                     DISPLAY_DATA_TYPE_E src_data_type,
                     uint8 *dst_buf_ptr,
                     uint32 dst_buf_width,
                     DISPLAY_DATA_TYPE_E dst_data_type,
                     uint32 blt_width,
                     uint32 blt_height,
                     uint32 dst_alpha
                     )
{
    uint8 *src_buf = PNULL;
    uint8 *dst_buf = PNULL;
    uint32 xcount = 0;
    uint32 ycount = 0;
    uint32 src_bytes_per_pixel = 0;
    uint32 dst_bytes_per_pixel = 0;
    uint32 srcjumpcount = 0;
    uint32 dstjumpcount = 0;
    uint32 srcbltlinecount = 0;
    uint32 dstbltlinecount = 0;
    uint32 alpha  = 0;

    BOOLEAN is_same_type = FALSE;

    if ((src_buf_width < blt_width) || (dst_buf_width < blt_width))
    {
        return;
    }
    if ((PNULL == src_buf_ptr) || (PNULL == dst_buf_ptr))
    {
        return;
    }
    src_buf = src_buf_ptr;
    dst_buf = dst_buf_ptr;
    ycount = blt_height;
    src_bytes_per_pixel = GetBytesPerPixel(src_data_type);
    dst_bytes_per_pixel = GetBytesPerPixel(dst_data_type);

    is_same_type = (src_data_type == dst_data_type);
    srcjumpcount = (src_buf_width-blt_width)*src_bytes_per_pixel;
    dstjumpcount = (dst_buf_width-blt_width)*dst_bytes_per_pixel;
    srcbltlinecount = blt_width*src_bytes_per_pixel;
    dstbltlinecount = blt_width*dst_bytes_per_pixel;
    
    while(ycount)
    {
        if (is_same_type)
        {
            xcount = blt_width;
            
		    while(xcount--)
		    {
                if (0 == *((uint32*)src_buf))
                {
                    //alpha = 50;
                    //alpha = alpha << 24;
                    //*((uint32*)dst_buf) = ((*((uint32*)src_buf)) | (alpha & 0xff000000));
                    *((uint32*)dst_buf) = *((uint32*)src_buf);
                }
                else
                {
                    alpha = dst_alpha;
                    alpha = alpha << 24;
                    *((uint32*)dst_buf) = (((*((uint32*)src_buf))&0x00ffffff) | (alpha & 0xff000000));
                }

    		    src_buf+=src_bytes_per_pixel;
    		    dst_buf+=dst_bytes_per_pixel;
            }
        }

		src_buf += srcjumpcount;
		dst_buf += dstjumpcount;
		ycount--;
    }
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL GUI_LCD_DEV_INFO CreateThumbViewSlideBgLayer(MMI_IDLE_GRID_DATA_T *idle_grid_ptr)
{
    uint16           lcd_width           = 0;
    uint16           lcd_height          = 0;
    GUI_LCD_DEV_INFO widget_layer_handle = {0};
    UILAYER_CREATE_T create_info         = {0}; 

    if (PNULL == idle_grid_ptr)
    {
        //SCI_TRACE_LOW:"CreateMoveLayer() PNULL == idle_grid_ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIGRID_IDLE_11442_112_2_18_3_7_22_47,(uint8*)"");
        return;
    }

    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &lcd_width, &lcd_height); 

    if (UILAYER_HANDLE_MULTI != UILAYER_GetHandleType(&idle_grid_ptr->thumb_view_slide_bg_layer))
    {
        create_info.lcd_id          = LCD_ID_0;
        create_info.owner_handle    = idle_grid_ptr->win_id;
        create_info.offset_x        = 0;
        create_info.offset_y        = 0;
        create_info.width           = lcd_width;
        create_info.height          = lcd_height;
        create_info.is_bg_layer     = FALSE;
        create_info.is_static_layer = FALSE;   
        
        UILAYER_CreateLayer(&create_info, &widget_layer_handle);
            
        UILAYER_SetLayerColorKey(&widget_layer_handle, TRUE, UILAYER_TRANSPARENT_COLOR);
    
        idle_grid_ptr->thumb_view_slide_bg_layer = widget_layer_handle;
    }
    
    if (UILAYER_IsLayerActive(&widget_layer_handle))
    {
        UILAYER_Clear(&widget_layer_handle);   
    }
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void DestroyThumbViewSlideBgLayer(MMI_IDLE_GRID_DATA_T *idle_grid_ptr)
{
    if (PNULL == idle_grid_ptr)
    {
        //SCI_TRACE_LOW:"DestroyMoveLayer() PNULL == idle_grid_ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIGRID_IDLE_11480_112_2_18_3_7_22_48,(uint8*)"");
        return;
    }
 
    if (UILAYER_HANDLE_MULTI == UILAYER_GetHandleType(&idle_grid_ptr->thumb_view_slide_bg_layer))    
    {
        UILAYER_RELEASELAYER(&(idle_grid_ptr->thumb_view_slide_bg_layer));   /*lint !e506 !e774*/
    }
}

#endif//MMI_IDLE_THUMBVIEW_SLIDE

//**************************************************************************//
//********************* U IDLE EFFECT CODES BEGIN **************************//
//**************************************************************************//
#ifdef PDA_UI_SUPPORT_U_IDLE
/*****************************************************************************/
//  Description : draw last frame for uidle effect.
//  Global resource dependence : 
//  Author:hongbo.lan
//  Note: 
/*****************************************************************************/
LOCAL void DrawUIdleLastFrame(MMI_IDLE_GRID_DATA_T *idle_grid_ptr, uint16 dst_page_idx)
{
    idle_grid_ptr->widget_mov_type &= ~MMIGRID_TP_MOVE_PAGE; 

    ResetWidgetTpState(idle_grid_ptr);

    MMIGRID_SetBltLayer(idle_grid_ptr);
    
    UILAYER_SetLayerMemFormat(&idle_grid_ptr->page_layer, UILAYER_MEM_FPS_ACCELERATE);
    //send  foucs msg to page that will be displayed
    MMIWIDGET_DispatchMsgToChildInOnePage(idle_grid_ptr, idle_grid_ptr->cur_page_index,
                             MSG_GET_FOCUS ,0); 

    MMIWIDGET_ReCalcAllWidgetWinRect(idle_grid_ptr);  
    MMIWIDGET_DispatchMsgToActiveChildWin(idle_grid_ptr, MSG_WIDGET_PAGE_SLIDE_END);
    ResetGridPageLayerPos( idle_grid_ptr);
    DrawPageTitleBar(idle_grid_ptr, idle_grid_ptr->cur_page_index);        
    DrawSpecificPage(idle_grid_ptr, idle_grid_ptr->cur_page_index);
#ifdef BAR_PHONE_WIDGET 
    DisplaySoftkey(idle_grid_ptr);                  	    
#endif
}
#endif
//**************************************************************************//
//********************* U IDLE EFFECT CODES END ****************************//
//**************************************************************************//

#ifdef BAR_PHONE_WIDGET 

LOCAL BOOLEAN GetCurPageWidgetIdAndIndex(
                                MMI_IDLE_GRID_DATA_T * idle_grid_ptr,
                                uint32 *widget_id,
                                uint32 *index
                                )
{
    BOOLEAN has_widget = FALSE;
    uint32 page_grid_num = GetPageGridNum(idle_grid_ptr);
    uint32 j = 0;
    uint32 node_id = 0;
    
    for (j = 0; j < page_grid_num; j++)
    {         
            if (IS_NODE_WIDGET(idle_grid_ptr->grid_data_tab[idle_grid_ptr->cur_page_index][j]) && !IS_NODE_AUX(idle_grid_ptr->grid_data_tab[idle_grid_ptr->cur_page_index][j]))
            {
                node_id  = GET_NODE_ID(idle_grid_ptr->grid_data_tab[idle_grid_ptr->cur_page_index][j]);

                if (!MMIWIDGET_IsValidWidgetId(node_id))
                {
                    continue;
                }
                else
                {
                    if(widget_id != PNULL)
                    {
                        *widget_id = node_id;
                    }
                    if(index != PNULL)
                    {
                        *index = j;
                    }
                    has_widget = TRUE;
                    break; 
                }
            }
        }

    return has_widget;
}

 
LOCAL void UpdateSoftKeyText(MMI_IDLE_GRID_DATA_T * idle_grid_ptr)
{
    MMI_TEXT_ID_T softkey_text    = TXT_NULL;
    uint32 i = 0;
    GUI_LCD_DEV_INFO  lcd_dev_info = {0};    
    uint8                   font_height     = 0;
    uint16                  str_width       = 0;
    GUI_RECT_T              dis_rect        = {0};
    GUI_POINT_T             dis_point       = {0};
    MMI_STRING_T            sk_str          = {0};
    MMI_STRING_T            full_str_t = {0};
    uint16                  display_max_len = 0;
    wchar                   *full_str = PNULL;
    uint16                  full_len = MMI_MAINSCREEN_RIGHT_MAX_PIXEL;
    GUISTR_STYLE_T          text_style = {0};/*lint !e64*/
    GUISTR_STATE_T          text_state = GUISTR_STATE_ALIGN|GUISTR_STATE_EFFECT|GUISTR_STATE_ELLIPSIS|GUISTR_STATE_SINGLE_LINE;
    uint32                  widget_id = 0;
    MMI_WIN_ID_T            win_id = 0;    

    if(0 != idle_grid_ptr->slide_anim.slide_flag)
    {
        return;
    }

    GetCurPageWidgetIdAndIndex(idle_grid_ptr, &widget_id,0);
    win_id = MMIWIDGET_GetWinHandleByWidgetId(idle_grid_ptr, widget_id);    
    MMK_SendMsg( win_id, MSG_WIDGET_UPDATE_SOFTKEY,  0);
    
    if(UILAYER_IsLayerActive(&idle_grid_ptr->softkey_layer))
    {
        lcd_dev_info = idle_grid_ptr->softkey_layer;
        UILAYER_ClearRect(&lcd_dev_info,idle_grid_ptr->softkey_info[MMIGRID_SOFTKEY_BG].rc);    
    }
    else
    {
        lcd_dev_info = *MMITHEME_GetDefaultLcdDev();
    }
    
    idle_grid_ptr->softkey_info[MMIGRID_SOFTKEY_MIDDLE].normal_text_id = 
                    idle_grid_ptr->widget_softkey.normal_text_id;


    font_height = GUI_GetFontHeight(MMI_FULLSCREEN_SOFTKEY_TEXT_FONT,UNICODE_HANZI);
    full_str = SCI_ALLOC_APP(sizeof(wchar)*MMI_MAINSCREEN_RIGHT_MAX_PIXEL);

    for(i=MMIGRID_SOFTKEY_LEFT;i<(MMIGRID_SOFTKEY_RIGHT+1);i++)
    {
    
        softkey_text = idle_grid_ptr->softkey_info[i].normal_text_id;
        if(softkey_text !=TXT_NULL && softkey_text != 0)
        {
            //get softkey area
            dis_rect = idle_grid_ptr->softkey_info[i].rc;
            display_max_len = dis_rect.right - dis_rect.left;
             
            //get softkey text
            MMI_GetLabelTextByLang(softkey_text,&sk_str);

            MMIIDLE_GetIdleTextStyle(&text_style, MMI_FULLSCREEN_SOFTKEY_TEXT_FONT, MMITHEME_GetCurThemeFontColor(MMI_THEME_IDLE_SOFTKEY));

            SCI_MEMSET(full_str,0,(sizeof(wchar)*MMI_MAINSCREEN_RIGHT_MAX_PIXEL));
            full_len = MMI_MAINSCREEN_RIGHT_MAX_PIXEL;
            MMIIDLE_GetDisPlayString(sk_str.wstr_ptr,sk_str.wstr_len,display_max_len,MMI_FULLSCREEN_SOFTKEY_TEXT_FONT,full_str,&full_len);

            //get softkey text length
            full_str_t.wstr_ptr = full_str;
            full_str_t.wstr_len = full_len;
            str_width = GUISTR_GetStringWidth(&text_style,&full_str_t,text_state);

            //set softkey display point
            if(i==MMIGRID_SOFTKEY_LEFT)
            {
                dis_point.x = dis_rect.left;
            }
            else if(i==MMIGRID_SOFTKEY_RIGHT)
            {
                dis_point.x = MAX((dis_rect.right - str_width - 1),(dis_rect.left));
            }
            else
            {  
                dis_point.x =  (dis_rect.left +dis_rect.right - str_width)/2;
            }
            dis_point.y = (dis_rect.top +dis_rect.bottom - font_height)/2;
            
            //display string
            MMIIDLE_DisplayIdleText(
                (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
                (const GUI_POINT_T *)&dis_point,
                MMI_FULLSCREEN_SOFTKEY_TEXT_FONT,
                MMITHEME_GetCurThemeFontColor(MMI_THEME_IDLE_SOFTKEY),
                full_str, 
                full_len
                );
        }
    }
    SCI_FREE(full_str);
    full_str = PNULL;                
}


PUBLIC BOOLEAN MMIWIDGET_SoftkeySetTextId(
                                   MMI_WIN_ID_T    win_id,
                                   MMI_TEXT_ID_T    text_id,
                                   BOOLEAN is_update
                                   )
{
    BOOLEAN result = FALSE;
    MMI_IDLE_GRID_DATA_T *idle_grid_ptr = MMIAPIGRID_GetGridPtr();

    idle_grid_ptr->widget_softkey.normal_text_id = text_id;
    if(is_update)
    {
        UpdateSoftKeyText(idle_grid_ptr);
    }
    
    return result;
}

#endif

//get log file name from path string
PUBLIC char* MMIWIDGET_LogGetFile( const char* file )
{
    char*  Log_file = (char*)file;
    char*  Log_current = Log_file;

    while ( *Log_current != '\0' )
    {
        if ( *Log_current == '\\'  || *Log_current == '/' )
        {
            Log_file = Log_current + 1;
        }

        Log_current++;
    }

    return Log_file;
}

#endif


/*Edit by script, ignore 13 case. Thu Apr 26 19:01:10 2012*/ //IGNORE9527
