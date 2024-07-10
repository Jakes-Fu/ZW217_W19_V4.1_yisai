/*****************************************************************************
** File Name:      mmiqbtheme_wintab.c                                       *
** Author:                                                                   *
** Date:           08/10/2011                                                *
** Copyright:      2011 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe                             *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 08/10/2011     ying.xu              Creat
******************************************************************************/

/*---------------------------------------------------------------------------*/
/*                         Include Files                                     */
/*---------------------------------------------------------------------------*/
#include "mmi_app_widget_trc.h"
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
//#include "mmiset.h"
#include "mmiset_export.h"
#include "mmidisplay_data.h"
#include "mmiset_wintab.h"
#include "mmi_mainmenu_export.h"
#include "mmiwidget_id.h"
#include "mmi_id.h"
#include "chng_freq.h"
#include "mmimp3_export.h"
#include "mmi_wallpaper_export.h"
#include "mmk_tp.h"
#include "Guistatusbar.h"
#include "mmiwidget_image.h"
#include "mmi_position.h"
#include "chng_freq.h"
#include "mmiwidget_text.h"
#include "guimenu.h"
#include "graphics_draw.h"
#include "mmiwidget_export.h"
#ifdef WEATHER_SUPPORT
#include "mmiweather.h"
#endif
#include "mmiidle_export.h"
#include "mmiidle_statusbar.h"
//#include "mmiss_export.h"
#include "guictrl_api.h"
#ifdef DYNAMIC_MAINMENU_SUPPORT
#include "mmi_mainmenu_synchronize.h"
#endif
#ifdef LIVE_WALLPAPER_SUPPORT
#include "mmi_wallpaper_export.h"
#endif
#ifdef LIVE_WALLPAPER_FRAMEWORK_SUPPORT
#include "mmilivewallpaper_export.h"
#endif

#include "mmiwidget.h"
#include "mmiwidget_internal.h"

#include "mmiwidget_item.h"
#include "mmiwidget_grid.h"
#include "mmiwidget_page.h"
#include "mmiwidget_virtualwin.h"
#include "mmiwidget_wallpaper.h"

#include "mmiqbtheme_export.h"
#include "mmiqbtheme_config.h"
#include "mmiqbtheme_item.h"
#include "mmiqbtheme_grid.h"
#include "mmiqbtheme_page.h"
#include "mmiqbtheme_virtualwin.h"
#include "mmiqbtheme_dockbar.h"
#include "mmiqbtheme_idlewin.h"
#include "mmiqbtheme_menuwin.h"
#include "mmiqbtheme_idleexport.h"
#ifdef PDA_UI_SUPPORT_U_IDLE
#include  "mmigrid_uidle_se.h"
#endif
/*---------------------------------------------------------------------------*/
/*                         Macro Declaration                                 */
/*---------------------------------------------------------------------------*/

#define MMIWIDGET_LONG_TP_PEN_WIDTH     2 
#define MMIWIDGET_LONG_TP_PEN_RANGE     5
#define MMIWIDGET_MOVE_X_MIN_PIX        10
#define MMIWIDGET_MOVE_Y_MIN_PIX        10
#define MMIWIDGET_INT16_MIN_VALUE      -0x8000
#define MMIWIDGET_INVALID_BAR_POS       0xFF

#define MMIWIDGET_TP_MOVE_PAGE_TIMEOUT              20

#define MMIWIDGET_TP_MOVE_TIMEOUT                   10


#define MMIWIDGET_MAX_COUNT_LIMIT                   100     //超时计数

#define MMIWIDGET_EDGE_DIST                         30      //widget移到边缘留的空隙

//按比例缩放
#define ZOOM_OFFSET(offset, lcd_width, lcd_height)    (((offset) * (lcd_width) * 2 + (lcd_height)) / ((lcd_height) * 2))

#define MMIWIDGET_THUMBVIEW_ALPHA              80



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

/*---------------------------------------------------------------------------*/
/*                         GLOBAL DEFINITION                                 */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         LOCAL FUNCTION DECLARE                            */
/*---------------------------------------------------------------------------*/


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
//  Description : handle widget tp move
//  Global resource dependence : 
//  Author:ying.xu
//  Note: 
/*****************************************************************************/
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
//  Description : create widget layer
//  Global resource dependence : 
//  Author:hongbo.lan
//  Note: 
/*****************************************************************************/
LOCAL void CreatePageLayer(
                           MMI_IDLE_GRID_DATA_T *idle_grid_ptr
                           );

LOCAL BOOLEAN CreatePreviewIconPageLayer(  MMI_WIN_ID_T    win_id, 
                                      GUI_LCD_DEV_INFO *lcd_dev_info_ptr
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
//  Description : destroy page layer
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void DestroyWidgetPageLayer(
                                  MMI_IDLE_GRID_DATA_T *idle_grid_ptr
                                  );


LOCAL void CreateThumbMoveLayer(MMI_IDLE_GRID_DATA_T *idle_grid_ptr, 
                                MMIGRID_ITEM_TYPE_E elem_type);

LOCAL void DestroyThumbMoveLayer(
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
//  Description : draw page title bar.
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note:
/*****************************************************************************/
LOCAL void DrawPageTitleBar(MMI_IDLE_GRID_DATA_T *idle_grid_ptr);


/*****************************************************************************/
//  Description : redraw top layer
//  Global resource dependence : 
//  Author:hongbo.lan
//  Note: 
/*****************************************************************************/
LOCAL void DrawAll(MMI_IDLE_GRID_DATA_T *idle_grid_ptr);
LOCAL void DrawCurrentPage(MMI_IDLE_GRID_DATA_T *idle_grid_ptr);

/*****************************************************************************/
//  Description : reset movable mode action.
//  Global resource dependence : 
//  Author: hongbo.lan  
//  Note:
/*****************************************************************************/
LOCAL void ResetGridMovableAction(MMI_IDLE_GRID_DATA_T *idle_grid_ptr);


LOCAL void SetBufferAlpha(
                     uint8 *dst_buf_ptr,
                     uint32 dst_buf_width,
                     DISPLAY_DATA_TYPE_E dst_data_type,
                     uint32 blt_width,
                     uint32 blt_height
                        );
LOCAL void SetBufferAlphaEx(GUI_LCD_DEV_INFO *layer_ptr, GUI_RECT_T *rect_ptr, uint32 alpha);

//page title layer
LOCAL void CreatePageTitleLayer(MMI_IDLE_GRID_DATA_T *idle_grid_ptr);
LOCAL void DestroyPageTitleLayer( MMI_IDLE_GRID_DATA_T *idle_grid_ptr);
LOCAL GUI_RECT_T GetWidgetPageTitleRect(MMI_IDLE_GRID_DATA_T *idle_grid_ptr );
LOCAL GUI_RECT_T GetWidgetPageDisplayRect(MMI_IDLE_GRID_DATA_T *idle_grid_ptr) ;
LOCAL void InitScGridRect(MMI_IDLE_GRID_DATA_T *idle_grid_ptr);

LOCAL int32 ThumbMoveLayer(void);
LOCAL int32 MMIQBTHEME_MoveMenu2DockbarPage2(MMI_IDLE_GRID_DATA_T *idle_grid_ptr, GUI_POINT_T point);
LOCAL int32 MMIQBTHEME_MoveIcon2DockbarPage3(MMI_IDLE_GRID_DATA_T *idle_grid_ptr, GUI_POINT_T point, 
                                             MMIGRID_ITEM_TYPE_E item_type);

#if defined BACKGROUND_GRID_LINE_LAYER

LOCAL void DrawAllBackground(MMI_IDLE_GRID_DATA_T *idle_grid_ptr);

LOCAL void DrawFocusedGrid(MMI_IDLE_GRID_DATA_T *idle_grid_ptr);

LOCAL void DrawPageGridLine(MMI_IDLE_GRID_DATA_T *idle_grid_ptr);
LOCAL int32 DrawLine(GUI_LCD_DEV_INFO    *dev_info_ptr,  // block information
                    uint16                 x1,             //x coordinate of first point
                    uint16                 y1,             //y coordinate of first point
                    uint16                 x2,             //x coordinate of second point
                    uint16                 y2,             //y coordinate of second point
                    GUI_COLOR_T  color ,          //color to draw.
                    int32       alpha);
LOCAL  uint32 RGB565TO888(uint32 rgb565);   /*lint !e401 */

/*****************************************************************************/
//  Description : Draw Focused grid frame
//  Global resource dependence : 
//  Author: ying.xu
//  Note:
/*****************************************************************************/
LOCAL int32 DrawFocusedGridFrame(GUI_RECT_T rect, GUI_LCD_DEV_INFO *dev_info_ptr);
#endif
                    
LOCAL BOOLEAN MMIQBTHEME_SetMaxItemNum(
                                     MMIWIDGET_CTRL_E ctrl_id, // control id
                                     uint32        max_item_num
                                     );
            
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
                                   )
{
    GUI_POINT_T        tp_point = {0};
    MMI_RESULT_E       result = MMI_RESULT_TRUE;
    GUI_RECT_T         dockbar_rect = {0};
    MMIQBTHEME_VIRTUALWIN_T * p_dockbar = MMIQBTHEME_GetDockbarPtr();
    if (PNULL == idle_grid_ptr)
    {
        return MMI_RESULT_FALSE;
    }
    
    tp_point.x = MMK_GET_TP_X(param);
    tp_point.y = MMK_GET_TP_Y(param);
    
    MMIQBTHEME_VirtualWinGetRect(p_dockbar,   &dockbar_rect );
    switch (msg_id)
    {
    case MSG_TP_PRESS_DOWN:       
        if(GUI_PointIsInRect(tp_point ,  dockbar_rect))
        {
            return MMI_RESULT_FALSE;
        }
        
        if (!HandleGridTpDown(idle_grid_ptr, param))
        {
            result = MMI_RESULT_FALSE;
        }        
        
        break;
        
    case MSG_TP_PRESS_UP:        
        result = HandleGridTpUp(idle_grid_ptr, param);
        break;     
        
        
    case MSG_TP_PRESS_LONG:        
        if(GUI_PointIsInRect(tp_point ,  dockbar_rect))
        {
            return MMI_RESULT_FALSE;
        }
        result = HandleGridTpLongDown(idle_grid_ptr, param);
        break;     
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return result;
}



/*****************************************************************************/
//  Description : get current tp status is moveing page or not.
//  Global resource dependence : 
//  Author:  hongbo.lan 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIQBIDLE_IsForbidPaint(uint32 widget_id)
{
    BOOLEAN ret = FALSE;
    
    ret = (s_idle_grid_data_ptr->tp_move_is_valid
        || s_idle_grid_data_ptr->widget_mov_type != MMIGRID_TP_MOVE_NONE
        || (MMIGRID_RUN_MODE_EDITING == s_idle_grid_data_ptr->run_mode)
        );    
    
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
    if (MMIGRID_TP_CLICK_LONG_WIDGET == idle_grid_ptr->tp_move_info.click_type)
    {
        if (idle_grid_ptr->old_grid.is_valid)
        {
            MMIWIDGET_FillGridTbl(idle_grid_ptr,
                &idle_grid_ptr->old_grid.filled_grid_cell,
                idle_grid_ptr->old_grid.page,
                idle_grid_ptr->tp_move_info.click_id,
                MMIGRID_ITEM_TYPE_WIDGET);           
        }
        else
        {
            MMIWIDGET_RemoveWidgetIcon(idle_grid_ptr, idle_grid_ptr->cur_page_index, 
                idle_grid_ptr->tp_move_info.click_id, MMIGRID_ITEM_TYPE_WIDGET );
        }
    }
    else  if (MMIGRID_TP_CLICK_LONG_SHORTCUT == idle_grid_ptr->tp_move_info.click_type)
    {
        if (idle_grid_ptr->old_grid.is_valid)
        {
            MMIWIDGET_FillGridTbl(idle_grid_ptr,
                &idle_grid_ptr->old_grid.filled_grid_cell,
                idle_grid_ptr->old_grid.page,
                idle_grid_ptr->tp_move_info.click_id,
                MMIGRID_ITEM_TYPE_ICON);                
        }      
        else
        {
            //MMIWIDGET_RemoveWidgetIcon(idle_grid_ptr, idle_grid_ptr->cur_page_index, 
             //   idle_grid_ptr->old_grid.filled_grid_cell.valid_cell_array[0], MMIGRID_ITEM_TYPE_ICON);
             
            MMIWIDGET_RemoveShortcutInAddingList(idle_grid_ptr,idle_grid_ptr->tp_move_info.click_id);
        }
    }
    idle_grid_ptr->tp_move_is_valid = FALSE;
    ClearOldGridArea(idle_grid_ptr);
    DestroyMoveLayer(idle_grid_ptr);        
    //reset to main layer
    MMIWIDGET_ResetHitWidgetLayer(idle_grid_ptr);        
    ResetWidgetTpState(idle_grid_ptr);
}

/*****************************************************************************/
//  Description : handle widget msg
//  Global resource dependence : 
//  Author:  hongbo.lan
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIAPIQBIDLE_HandleMsg(
                                      MMI_WIN_ID_T      win_id,
                                      MMI_MESSAGE_ID_E  msg_id,
                                      DPARAM            param
                                      )
{
    MMI_RESULT_E	             result = MMI_RESULT_FALSE;
    MMI_IDLE_GRID_DATA_T        *idle_grid_ptr = PNULL;
    GUI_POINT_T	                 point = {0, 0};
    MMIWIDGET_ITEM_T            *item_ptr = PNULL;
    MMIQBTHEME_VIRTUALWIN_T     *p_dockbar =    MMIQBTHEME_GetDockbarPtr();
    MMI_RESULT_E	             dockbar_result = MMI_RESULT_FALSE;
    
    idle_grid_ptr = MMIAPIQBIDLE_GetGridPtr();
    if (PNULL == idle_grid_ptr)
    {
        if (MSG_GET_FOCUS == msg_id || MSG_OPEN_WINDOW == msg_id)
        {
            MMIAPIQBTHEME_CreateIdle();
        }
        idle_grid_ptr = MMIAPIQBIDLE_GetGridPtr();
        if (PNULL == idle_grid_ptr)
        {
            return result;
        }
    }
       

    //处理快的放在前面
    dockbar_result = MMIQBTHEME_HandleDockbarMsg(idle_grid_ptr, msg_id, param);
   
    
    switch (msg_id)
    {
    case MSG_APP_WEB:
        MMIAPIMENU_CreatMainMenu();       
        break;
  
    case MSG_APP_MENU: 
        MMIWIDGET_CreateGridMenuWin(idle_grid_ptr);
        break;
        
    case MSG_APP_RED:        
    case MSG_APP_CANCEL:
        if (!MMK_IsFocusWin(idle_grid_ptr->win_id))
        {
            break;
        }

        if (MMIGRID_TP_MOVE_IDLE_TO_IDLE == idle_grid_ptr->widget_mov_type
            || MMIGRID_TP_MOVE_PAGE == idle_grid_ptr->widget_mov_type)
        {
            break;
        }
        if (MMIGRID_RUN_MODE_EDITING == idle_grid_ptr->run_mode)
        {
            MMIQBIDLE_SetWidgetWinMode(idle_grid_ptr, MMIGRID_RUN_MODE_NORMAL);

            //set dockbar in editing mode    
            MMIQBTHEME_VirtualWinSetFSMState(p_dockbar, MMIQBTHEME_NORMAL);             
            MMIQBTHEME_VirtualWinSetCurrentPage( p_dockbar,1);
            MMIQBTHEME_VirtualWinHandleMsg( p_dockbar,  p_dockbar->display.win_id, MSG_FULL_PAINT , 0);
            DrawAll(idle_grid_ptr);   
            MMIQBIDLE_SaveWidgetInfoToNV(idle_grid_ptr);    
        }
        break;
        
    case MSG_FULL_PAINT:
        if (idle_grid_ptr->tp_move_is_valid)
        {
            break;
        }
        DrawAll(idle_grid_ptr);   
        break;
        
        
    case MSG_TP_PRESS_LONG:
    case MSG_TP_PRESS_DOWN:
//        MMIAPIIDLESS_StopTimer();
        if(MMI_RESULT_TRUE == dockbar_result)
        {
            break;
        }
        HandleGridTPMsg(idle_grid_ptr, msg_id, param);
        break;
        
    case MSG_TP_PRESS_MOVE:    
        if(MMI_RESULT_TRUE == dockbar_result)
        {
            break;
        }
        HandleGridTPMsg(idle_grid_ptr, msg_id, param);          
        break;
        
    case MSG_TP_PRESS_UP:
        //MMIDLE_RestartSsTimer(win_id);     
        HandleGridTPMsg(idle_grid_ptr, msg_id, param);
        break;
                
    case MSG_LOSE_FOCUS:   
        //主层内容：shortcut，状态栏，softkey, 墙纸，widget应用     
        ResetWallpaperLayer(idle_grid_ptr);
        StopTpMoveTimer(idle_grid_ptr);
        
        if (MMIGRID_TP_CLICK_LONG_WIDGET == idle_grid_ptr->tp_move_info.click_type)
        {
            item_ptr = MMIWIDGET_FindWidgetItem(idle_grid_ptr, idle_grid_ptr->tp_move_info.click_id);            
            if (PNULL != item_ptr && PNULL != item_ptr->item_info.ResetChildWinItem)
            {
                item_ptr->item_info.ResetChildWinItem();
            }
        }        
        
        ResetGridMovableAction(idle_grid_ptr);
        
        DestroyThumbMoveLayer(idle_grid_ptr);
        ResetGridPageLayerPos(idle_grid_ptr);        
        MMIWIDGET_ReCalcAllWidgetWinRect(idle_grid_ptr);
        if (MMIGRID_RUN_MODE_EDITING == idle_grid_ptr->run_mode)
        {
            MMIQBIDLE_SaveWidgetInfoToNV(idle_grid_ptr);
        }
        idle_grid_ptr->widget_mov_type = MMIGRID_TP_MOVE_NONE;
        
        
        MMIWIDGET_DispatchMsgToActiveChildWin(idle_grid_ptr, msg_id);
        //MMIWIDGET_DispatchMsgToChildInOnePage(idle_grid_ptr,
        //            idle_grid_ptr->cur_page_index,  msg_id ,param);    
#ifdef PDA_UI_SUPPORT_U_IDLE
        MMIUIDLE_FreeUIdleAction(idle_grid_ptr); 
#endif   
#ifdef MMI_IDLE_WIDGET_CACHE
        MMIWIDGET_ReleaseWidgetCacheInAllPage(idle_grid_ptr);            
#endif   
        break;
        
    case MSG_GET_FOCUS:                      
        MMK_SetWinSupportAngle(idle_grid_ptr->win_id, WIN_SUPPORT_ANGLE_INIT);
        ResetWallpaperLayer(idle_grid_ptr);


        //ResetGridPageLayerPos(idle_grid_ptr);
        //MMIWIDGET_ResetHitWidgetLayer(idle_grid_ptr);  
        
        //MMIWIDGET_DispatchMsgToAllChildWin(idle_grid_ptr, MSG_GET_FOCUS);
        MMIWIDGET_DispatchMsgToChildInOnePage(idle_grid_ptr,
                    idle_grid_ptr->cur_page_index,  msg_id ,param);     
                    
#ifdef DYNAMIC_MAINMENU_SUPPORT
        if (MMIMENU_DyCheckIsSyn())
        {
            MMIMENU_DyGetAllItemData();
        }
#endif
        if(MMIGRID_TP_CLICK_LONG_SHORTCUT == idle_grid_ptr->tp_move_info.click_type)
        {
            HandleIconTpLongDown(idle_grid_ptr, idle_grid_ptr->down_point);
            SaveTpMovePoint(idle_grid_ptr, idle_grid_ptr->down_point);
        } 
        else
            if (MMIGRID_TP_CLICK_LONG_WIDGET == idle_grid_ptr->tp_move_info.click_type)
            {
                HandleWidgetTpLongDown(idle_grid_ptr);
            }
            
            CreateTpMoveTimer(idle_grid_ptr, idle_grid_ptr->win_id);
            break;
            
    case MSG_OPEN_WINDOW: 
        {  
            MMK_SetWinSupportAngle(idle_grid_ptr->win_id, WIN_SUPPORT_ANGLE_INIT);
            
            MMIQBIDLE_SetWidgetWinMode(idle_grid_ptr, MMIGRID_RUN_MODE_NORMAL);
            
            MMIQBIDLE_SetBltLayer(idle_grid_ptr);
            ResetWallpaperLayer(idle_grid_ptr);
            CreateTpMoveTimer(idle_grid_ptr, idle_grid_ptr->win_id);
        }
        break;
        
    case MSG_TIMER:
        if(!MMK_IsFocusWin(win_id))
        {
            break;
        }
        if (idle_grid_ptr->slide_anim.timer_id == *( GUI_TIMER_ID_T*)param)
        {
#ifdef LIVE_WALLPAPER_FRAMEWORK_SUPPORT
             MMIAPILIVEWALLPAPER_SaveTimerId(idle_grid_ptr->slide_anim.timer_id);
#endif    
             RestartTpMoveTimer(idle_grid_ptr, win_id);
             SlideAnimCallback(idle_grid_ptr);
#ifdef TOUCH_PANEL_SUPPORT
             {
                 MMI_TP_STATUS_E tp_status = MMI_TP_NONE;
                 MMI_MULTI_KEY_TP_MSG_PARAM_T   para = {0};
                 MMK_GetLogicTPMsg(&tp_status, &point);
                 
                 para.cur_tp_point.x = point.x;
                 para.cur_tp_point.y = point.y;
                 if(MMI_TP_NONE != tp_status)
                 {
                     HandleGridTpMove(idle_grid_ptr, (DPARAM)&para);                         
                 }                   
             }
       }
#endif

            break;

    case MSG_IDLE_UPDATE_MP3:
    case MSG_IDLE_UPDATE_NETWORK:
    case MSG_IDLE_UPDATE_TIME_SECOND:    
    case MSG_IDLE_UPDATE_DATETIME:
        //MMIWIDGET_DispatchMsgToAllChildWin(idle_grid_ptr, msg_id);   
        //这些消息会引起widget画屏动作，forbid paint的时候不发
        if(MMIQBIDLE_IsForbidPaint(0))
        {
            break;
        }
        DrawSpecificPage(idle_grid_ptr, idle_grid_ptr->cur_page_index);
        //MMIWIDGET_DispatchMsgToChildInOnePage(idle_grid_ptr,
        //            idle_grid_ptr->cur_page_index,  msg_id ,param);    
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }    
    
    return(result);     
}

/*****************************************************************************/
//  Description : destroy the widget control.
//  Global resource dependence : 
//  Author:  hongbo.lan
//  Note:
/*****************************************************************************/
PUBLIC void MMIQBIDLE_Destroy(
                            MMIWIDGET_CTRL_E  ctrl_id
                            )
{    
    
    MMIQBTHEME_VIRTUALWIN_T *p_dockbar =  MMIQBTHEME_GetDockbarPtr();
    
    //目前只支持idle界面widget
    if (MMIWIDGET_CTRL_IDLE_ID != ctrl_id)
    {
        return;
    }
    
    if (PNULL != s_idle_grid_data_ptr)
    {        
#ifdef MMI_IDLE_WIDGET_CACHE
        //clean widget cache
        MMIWIDGET_ReleaseWidgetCacheInAllPage(s_idle_grid_data_ptr);
        MMIWIDGET_ClearWidgetCacheControlData(s_idle_grid_data_ptr);
#endif
        MMK_SetWinDisplayStyleState(MAIN_IDLE_WIN_ID, WS_DISABLE_REDRAW_CHILDWIN, FALSE);       
        DestroyWallpaperLayer(s_idle_grid_data_ptr);
        DestroyWidgetPageLayer(s_idle_grid_data_ptr);
        MMIWIDGET_DestroyAllWidgetItemData(s_idle_grid_data_ptr);
        DestroyPageTitleLayer(s_idle_grid_data_ptr);
        DestroyMoveLayer(s_idle_grid_data_ptr);
        DestroyThumbMoveLayer(s_idle_grid_data_ptr);
#ifdef DYNAMIC_MAINMENU_SUPPORT
        MMIMENU_DyFreeData();
#endif
        SCI_FREE(s_idle_grid_data_ptr->item_ptr);
        SCI_FREE(s_idle_grid_data_ptr->icon_ptr); 
        SCI_FREE(s_idle_grid_data_ptr);
        MMIQBTHEME_VirtualWinDestroy (p_dockbar);
        
        
        s_idle_grid_data_ptr = PNULL;
    }
}


/*****************************************************************************/
//  Description : create the widget control.
//  Global resource dependence : 
//  Author:  hongbo.lan
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIQBIDLE_Create(
                              MMI_WIN_ID_T win_id,
                              MMIWIDGET_CTRL_E  ctrl_id,
                              GUI_BOTH_RECT_T *both_rect_ptr
                              )
{
    BOOLEAN result = FALSE;
    MMI_GRID_SAVE_INFO_T save_info = {0};
    
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
        s_idle_grid_data_ptr->cur_idle_type = MMI_IDLE_QBTHEME;
        
        if (PNULL != both_rect_ptr)
        {
            s_idle_grid_data_ptr->both_rect = *both_rect_ptr;
        }
        s_idle_grid_data_ptr->item_ptr = PNULL;
        s_idle_grid_data_ptr->move_layer.lcd_id = GUI_MAIN_LCD_ID;
        s_idle_grid_data_ptr->move_layer.block_id = UILAYER_NULL_HANDLE;
        s_idle_grid_data_ptr->thumb_move_layer.lcd_id = GUI_MAIN_LCD_ID;
        s_idle_grid_data_ptr->thumb_move_layer.block_id = UILAYER_NULL_HANDLE;
        s_idle_grid_data_ptr->is_main_screen_landscape = MMK_IsWindowLandscape(win_id);  
        s_idle_grid_data_ptr->tp_move_is_valid = FALSE;

#ifdef PDA_UI_SUPPORT_U_IDLE
        s_idle_grid_data_ptr->is_Uidle_1st_mov = TRUE;        
#endif     
        
        s_idle_grid_data_ptr->delect_icon_id = IMAGE_QBTHEME_DOCKBAR_DELETE_ICON;

#ifdef DYNAMIC_MAINMENU_SUPPORT
        MMIMENU_DyGetAllItemData();
#endif
        //read all main menus.
        MMIAPIMENU_ReadQbthemeMenuItemInfo(s_idle_grid_data_ptr->grid_menu_info.grid_menu,
                        &s_idle_grid_data_ptr->grid_menu_info.menu_num);   
        
        MMITHEME_GetWidgetTheme(&s_idle_grid_data_ptr->display_theme);
        InitScGridRect(s_idle_grid_data_ptr);
        

        //初始化widget shortcut数据
        MMIQBTHEME_SetMaxItemNum(MMIWIDGET_CTRL_IDLE_ID, MMIWIDGET_MAX_ITEM_NUM);
        //读NV
        MMIQBIDLE_ReadWidgetNV(&save_info);

        MMIQBTHEME_InitDockbar(s_idle_grid_data_ptr);

        CreatePageLayer(s_idle_grid_data_ptr);
        CreateWallpaperLayer(s_idle_grid_data_ptr);  
        CreatePageTitleLayer(s_idle_grid_data_ptr);        
        
        SCI_MEMSET(s_idle_grid_data_ptr->previous_grid_data_tab, 0xff, 
            sizeof(s_idle_grid_data_ptr->previous_grid_data_tab));

#ifdef MMI_IDLE_WIDGET_CACHE        
        MMIWIDGET_InitWidgetCacheControlData(s_idle_grid_data_ptr);
#endif

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
PUBLIC MMI_IDLE_GRID_DATA_T *MMIAPIQBIDLE_GetGridPtr(void)
{
    return s_idle_grid_data_ptr;
}


/*****************************************************************************/
//  Description :  set max item num, must be called before append item
//  Global resource dependence : 
//  Author:  hongbo.lan
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIQBTHEME_SetMaxItemNum(
                                     MMIWIDGET_CTRL_E ctrl_id, // control id
                                     uint32        max_item_num
                                     )
{
    MMI_IDLE_GRID_DATA_T *idle_grid_ptr = MMIAPIQBIDLE_GetGridPtr();
    
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
    
    idle_grid_ptr->icon_ptr = (MMI_IDLE_SC_ICON_T*)SCI_ALLOC_APP(
            MMIQBTHEME_MAX_WIDGET_PAGE_NUM*MMI_IDLE_GRID_PAGE_CELL_NUM * sizeof(MMI_IDLE_SC_ICON_T));
    SCI_MEMSET(idle_grid_ptr->icon_ptr, 0x00, 
        MMIQBTHEME_MAX_WIDGET_PAGE_NUM*MMI_IDLE_GRID_PAGE_CELL_NUM * sizeof(MMI_IDLE_SC_ICON_T));
    idle_grid_ptr->icon_ptr->icon_num = 0;
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
    MMI_GRID_CELL_T filled_cell = {0};
    GUI_RECT_T new_rc = {0};
    uint32 temp_widget_id = idle_grid_ptr->tp_move_info.click_id;
    GUI_RECT_T dst_rect = {0};
    
    dst_rect = idle_grid_ptr->showed_rect_info.old_rect;
    
    if(MMIWIDGET_LayoutGridTblByRect(idle_grid_ptr,idle_grid_ptr->cur_page_index,dst_rect,&filled_cell,&new_rc))
    {            
        MMIWIDGET_FillGridTbl(idle_grid_ptr,&filled_cell,idle_grid_ptr->cur_page_index,
                    idle_grid_ptr->tp_move_info.click_id, MMIGRID_ITEM_TYPE_WIDGET);
        ret = TRUE;
    }    
    else if(idle_grid_ptr->old_grid.is_valid)
    {
        MMIWIDGET_FillGridTbl(idle_grid_ptr,&idle_grid_ptr->old_grid.filled_grid_cell,
            idle_grid_ptr->old_grid.page, temp_widget_id, MMIGRID_ITEM_TYPE_WIDGET); 
        ret = TRUE;
    }
    else
    {
        ret = FALSE;
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
    
    shortcut_item = idle_grid_ptr->icon_ptr[idle_grid_ptr->tp_move_info.click_id];
    index = MMIWIDGET_GetShortCutIdx(idle_grid_ptr,shortcut_item.icon_info);
    
    rc = idle_grid_ptr->showed_rect_info.old_rect;
    
    if (MMIWIDGET_LayoutGridTblByRect(idle_grid_ptr, idle_grid_ptr->cur_page_index, rc, &valid_grid, &dst_rc))
    {
        MMIWIDGET_FillGridTbl(idle_grid_ptr,&valid_grid,idle_grid_ptr->cur_page_index,index,MMIGRID_ITEM_TYPE_ICON); 
        ret = TRUE;
    }
    else if(idle_grid_ptr->old_grid.is_valid)
    {
        MMIWIDGET_FillGridTbl(idle_grid_ptr,&idle_grid_ptr->old_grid.filled_grid_cell,
            idle_grid_ptr->old_grid.page, index, MMIGRID_ITEM_TYPE_ICON); 
        ret = TRUE;
    }
    else
    {
        ret = FALSE;
    }
    
    
    return ret;
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
    GUI_RECT_T dockbar_rect = {0};
    MMIQBTHEME_VIRTUALWIN_T *p_dockbar =    MMIQBTHEME_GetDockbarPtr();
    point.x = MMK_GET_TP_X(param);
    point.y = MMK_GET_TP_Y(param);    
    
    idle_grid_ptr->tp_move_is_valid  = FALSE;
    
    if(MMIGRID_RUN_MODE_NORMAL == idle_grid_ptr->run_mode)
    {    
        if (idle_grid_ptr->is_child_win_btn)
        {        
            MMIWIDGET_RunChildWidgetProc(idle_grid_ptr, idle_grid_ptr->tp_move_info.click_id,
                MSG_WIDGET_TP_PRESS_UP, param);  
        }
    }
    MMIQBTHEME_VirtualWinGetRect(p_dockbar,  &dockbar_rect );
    
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
            idle_grid_ptr->widget_mov_type = MMIGRID_TP_MOVE_NONE ;
            ResetWidgetTpState(idle_grid_ptr);
        }
        else
#endif         
        HandleSlidePageTpUp(idle_grid_ptr, point);
        return TRUE;
    }
    else if(MMIGRID_TP_MOVE_IDLE_TO_IDLE == idle_grid_ptr->widget_mov_type)
    {
        if(MMIGRID_TP_CLICK_LONG_SHORTCUT == idle_grid_ptr->tp_move_info.click_type)
        {
            if(!GUI_PointIsInRect( point, dockbar_rect))
            {        
                if (!LayoutShortCutItem(idle_grid_ptr))
                {
                    if(idle_grid_ptr->cur_page_index != idle_grid_ptr->old_grid.page)
                    {
                        if(!MMIWIDGET_ResetShortCutItem(idle_grid_ptr))
                        {
                            MMIWIDGET_RemoveShortcutInAddingList(idle_grid_ptr,idle_grid_ptr->tp_move_info.click_id);
                        }
                    }
                }
            }            
            else
            {
                MMIMAINMENU_ITEM_INFO_T menu_info = {0};
                
                menu_info.group_id= idle_grid_ptr->icon_ptr[idle_grid_ptr->tp_move_info.click_id].icon_info.group_id ; 
                menu_info.menu_id = idle_grid_ptr->icon_ptr[idle_grid_ptr->tp_move_info.click_id].icon_info.menu_id;
                
                if(p_dockbar->current_page == 2)//dockbar page 3,preview icon page
                {
                    if(GUI_PointIsInRect( point, p_dockbar->p_page[2]->display.display_rect))
                    {
                        if(!MMIQBIDLE_AddNewShortcut(&menu_info))
                        {
                            if(idle_grid_ptr->cur_page_index != idle_grid_ptr->old_grid.page)
                            {
                                if(!MMIWIDGET_ResetShortCutItem(idle_grid_ptr))
                                {
                                    MMIWIDGET_RemoveShortcutInAddingList(idle_grid_ptr,idle_grid_ptr->tp_move_info.click_id);
                                }
                            }
                        }                          
                    }
                }
                else //if(p_dockbar->current_page == 0)
                {
                    if (!LayoutShortCutItem(idle_grid_ptr))
                    {
                        if(!MMIWIDGET_ResetShortCutItem(idle_grid_ptr))
                        {
                            MMIWIDGET_RemoveShortcutInAddingList(idle_grid_ptr,idle_grid_ptr->tp_move_info.click_id);
                        }
                    }
                }                    
            }
        }
        else  if (MMIGRID_TP_CLICK_LONG_WIDGET == idle_grid_ptr->tp_move_info.click_type)
        {                   
            if (!ResetChildWinPosition(idle_grid_ptr, point))
            {
                MMIWIDGET_RemoveWidgetIcon(idle_grid_ptr, idle_grid_ptr->cur_page_index, 
                    idle_grid_ptr->tp_move_info.click_id, MMIGRID_ITEM_TYPE_WIDGET );     
                
            }   
            MMIWIDGET_RunChildWidgetProc(idle_grid_ptr, idle_grid_ptr->tp_move_info.click_id,
                MSG_WIDGET_TP_PRESS_LONG_UP, param);            
        }         
        
        DestroyMoveLayer(idle_grid_ptr);
        DestroyThumbMoveLayer(idle_grid_ptr);
        idle_grid_ptr->widget_mov_type = MMIGRID_TP_MOVE_NONE ;
        //reset to main layer
        MMIWIDGET_ResetHitWidgetLayer(idle_grid_ptr); 
        ResetWidgetTpState(idle_grid_ptr);
        //MMIQBIDLE_SaveWidgetInfoToNV(idle_grid_ptr);    
        DrawCurrentPage(idle_grid_ptr);
        MMIQBTHEME_VirtualWinDraw( p_dockbar);
        return TRUE;
    }
    else    //if(MMIGRID_TP_MOVE_NONE == idle_grid_ptr->widget_mov_type)
    {
        if( MMIGRID_TP_CLICK_SHORTCUT == idle_grid_ptr->tp_move_info.click_type)
        {   
            if(MMIGRID_RUN_MODE_EDITING == idle_grid_ptr->run_mode)
            {
                if(MMIWIDGET_IsPointInDeleteIconRect( idle_grid_ptr, point))
                {
                    int32 grid_index = 0;
                    grid_index = MMIWIDGET_GetGridIndexByPos(idle_grid_ptr, point);
                    MMIWIDGET_RemoveWidgetIcon(idle_grid_ptr, idle_grid_ptr->cur_page_index, 
                        grid_index, MMIGRID_ITEM_TYPE_ICON );
                    ResetWidgetTpState(idle_grid_ptr);
                    //MMIQBIDLE_SaveWidgetInfoToNV(idle_grid_ptr);    
                    MMIQBTHEME_VirtualWinDraw( p_dockbar);
                }
                ResetWidgetTpState(idle_grid_ptr);
                DrawCurrentPage(idle_grid_ptr);
                return TRUE;
            }
            else
            {
                MMIAPIMENU_HandleIdleMenuItem(idle_grid_ptr->icon_ptr[idle_grid_ptr->tp_move_info.click_id].icon_info.group_id, 
                        idle_grid_ptr->icon_ptr[idle_grid_ptr->tp_move_info.click_id].icon_info.menu_id);
                ResetWidgetTpState(idle_grid_ptr);
                return TRUE;
            }
        }    
        else if(MMIGRID_TP_CLICK_WIDGET == idle_grid_ptr->tp_move_info.click_type)
        {
            if(MMIGRID_RUN_MODE_EDITING == idle_grid_ptr->run_mode)
            {
                if (MMIWIDGET_IsPointInDeleteIconRect( idle_grid_ptr, point))
                {
                    MMIWIDGET_RemoveWidgetIcon(idle_grid_ptr, idle_grid_ptr->cur_page_index, 
                        idle_grid_ptr->tp_move_info.click_id, MMIGRID_ITEM_TYPE_WIDGET );
                    ResetWidgetTpState(idle_grid_ptr);
                    //MMIQBIDLE_SaveWidgetInfoToNV(idle_grid_ptr);    
                    MMIQBTHEME_VirtualWinDraw( p_dockbar);
                }  
            }
            else
            {    
                if(!idle_grid_ptr->is_child_win_btn)
                {                    
                    MMIWIDGET_RunChildWidgetProc(idle_grid_ptr, idle_grid_ptr->tp_move_info.click_id,
                        MSG_WIDGET_TP_PRESS_UP, param);
                }
            }
            ResetWidgetTpState(idle_grid_ptr);
            DrawCurrentPage(idle_grid_ptr);
            return TRUE;
        }
        else if(MMIGRID_TP_CLICK_IDLE_EMPTY == idle_grid_ptr->tp_move_info.click_type)
        {            
            if(MMIGRID_RUN_MODE_EDITING == idle_grid_ptr->run_mode)
            {
                idle_grid_ptr->current_gird = MMIWIDGET_GetGridIndexByPos( idle_grid_ptr, point); 
                if(idle_grid_ptr->current_gird != -1 
                    && idle_grid_ptr->current_gird < MMIQBTHEME_GRID_PAGE_CELL_NUM
                    && p_dockbar->current_page != 0
                    )
                {
                    MMIQBTHEME_VirtualWinSetCurrentPage(p_dockbar,  0);
                    MMIQBTHEME_VirtualWinDraw(p_dockbar);
                }
            }            
            ResetWidgetTpState(idle_grid_ptr);
            DrawCurrentPage(idle_grid_ptr);
            return TRUE;
        }
        else
        {
            ResetWidgetTpState(idle_grid_ptr);            
            //MMIQBIDLE_SaveWidgetInfoToNV(idle_grid_ptr);    
            //DrawAll(idle_grid_ptr);
            return FALSE;
        }
    }       
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
    idle_grid_ptr->tp_move_info.click_type = click_type;
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
    if (ABS(idle_grid_ptr->down_point.x - point.x) >= MMIWIDGET_MOVE_X_MIN_PIX)
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
    if (ABS(idle_grid_ptr->down_point.y - point.y) >= MMIWIDGET_MOVE_Y_MIN_PIX)
    {
        return TRUE;
    }
    
    return FALSE;
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
    MMIQBTHEME_VIRTUALWIN_T *p_dockbar =    MMIQBTHEME_GetDockbarPtr();
    
    point.x = MMK_GET_TP_X(param);
    point.y = MMK_GET_TP_Y(param);    
    
    if (   (MMIGRID_TP_MOVE_PAGE == idle_grid_ptr->widget_mov_type)
        || (MMIGRID_TP_MOVE_IDLE_TO_IDLE== idle_grid_ptr->widget_mov_type))
    {
        return FALSE;
    }
    
    //MMIAPISET_SetVibratorEx(TRUE,MMISET_RING_TYPE_KEY_LOCK, FALSE, 0);

    if(MMIGRID_RUN_MODE_NORMAL == idle_grid_ptr->run_mode)
    {
        //enter eding mode
        MMIQBIDLE_SetWidgetWinMode(idle_grid_ptr,MMIGRID_RUN_MODE_EDITING);
        
        //set dockbar in editing mode    
        MMIQBTHEME_VirtualWinSetFSMState(p_dockbar, MMIQBTHEME_EDITING);
        MMIQBTHEME_VirtualWinHandleMsg( p_dockbar,  p_dockbar->display.win_id, MSG_FULL_PAINT , 0);
        
        MMIQBTHEME_SetWidgetPreviewPageFocusedItem(idle_grid_ptr->cur_page_index)  ;  
        MMIQBTHEME_SlideWidgetPreviewPage2FocusedItem();       
        
        DrawAll(idle_grid_ptr);
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

            idle_grid_ptr->widget_mov_type = MMIGRID_TP_MOVE_IDLE_TO_IDLE;
            SetGridTPClickType(idle_grid_ptr, MMIGRID_TP_CLICK_LONG_WIDGET);  
            idle_grid_ptr->tp_move_info.click_id = click_widget_id;                
            MMIWIDGET_ResetGridTblByScId(idle_grid_ptr, (uint16)idle_grid_ptr->cur_page_index,
                idle_grid_ptr->tp_move_info.click_id, MMIGRID_ITEM_TYPE_WIDGET);
            HandleWidgetTpLongDown(idle_grid_ptr);               
            SaveTpMovePoint(idle_grid_ptr, point);
            DrawCurrentPage(idle_grid_ptr);
            
            GetHightlightItemRect(idle_grid_ptr, &idle_grid_ptr->old_grid.filled_grid_cell, &cur_rect);
            DrawRect(idle_grid_ptr, cur_rect, MMI_GREEN_COLOR);   
            MMIQBTHEME_VirtualWinDraw( p_dockbar);
        }
        else
        {
            idle_grid_ptr->tp_move_info.click_id = MMIWIDGET_INVALID_ID;
            SetGridTPClickType(idle_grid_ptr, MMIGRID_TP_CLICK_INVALID);
        }
    }
    else  if (MMIGRID_ITEM_TYPE_ICON == press_type)
    {
        idle_grid_ptr->widget_mov_type = MMIGRID_TP_MOVE_IDLE_TO_IDLE;
        SetGridTPClickType(idle_grid_ptr, MMIGRID_TP_CLICK_LONG_SHORTCUT);
        idle_grid_ptr->tp_move_info.click_id = GET_NODE_ID(idle_grid_ptr->grid_data_tab[idle_grid_ptr->cur_page_index][click_widget_id]);        
        MMIWIDGET_ResetGridTblByScId(idle_grid_ptr, (uint16)idle_grid_ptr->cur_page_index, 
                click_widget_id,  MMIGRID_ITEM_TYPE_ICON);            
        HandleIconTpLongDown(idle_grid_ptr,point);      
        DrawCurrentPage(idle_grid_ptr);
        SaveTpMovePoint(idle_grid_ptr, point);
        
        GetHightlightItemRect(idle_grid_ptr, &idle_grid_ptr->old_grid.filled_grid_cell, &cur_rect);
        DrawRect(idle_grid_ptr, cur_rect, MMI_GREEN_COLOR);
        MMIQBTHEME_VirtualWinDraw( p_dockbar);
    }   
    else
    {    
        if(idle_grid_ptr->run_mode == MMIGRID_RUN_MODE_EDITING)
        {
            idle_grid_ptr->current_gird = MMIWIDGET_GetGridIndexByPos( idle_grid_ptr, point);            
            if(p_dockbar->current_page != 0)
            {
                MMIQBTHEME_VirtualWinSetCurrentPage(p_dockbar,  0);
                MMIQBTHEME_VirtualWinDraw(p_dockbar);
            }
            DrawCurrentPage(idle_grid_ptr);
        }
        idle_grid_ptr->tp_move_info.click_id = MMIWIDGET_INVALID_ID;
        SetGridTPClickType(idle_grid_ptr, MMIGRID_TP_CLICK_IDLE_EMPTY);
    }
    
    return TRUE;
}



/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIQBIDLE_AddMenu2Idle(MMIMAINMENU_ITEM_INFO_T shortcut_info, GUI_POINT_T start_pos)
{
    uint32 i = 0;
    MMI_IDLE_GRID_DATA_T *idle_grid_ptr = MMIAPIQBIDLE_GetGridPtr(); 
    CTRLMAINMENU_ITEM_T     menuitem_info   = {0};    
    
    uint16                img_width     = 0;
    uint16                img_height    = 0; 
    MMI_GRID_FILLED_INFO_T       valid_grid    = {0};
    GUI_RECT_T            rect          = {0};
    GUI_RECT_T            dst_rect      = {0};
    MMI_IMAGE_ID_T        img_id        = 0;  
    MMIQBTHEME_VIRTUALWIN_T *p_dockbar =    MMIQBTHEME_GetDockbarPtr();
    
    //SCI_TRACE_LOW:"from main menu: group id = %d, menu id = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIQBTHEME_IDLEWIN_2395_112_2_18_3_9_6_285,(uint8*)"dd", shortcut_info.group_id, shortcut_info.menu_id);
        
    if (PNULL == idle_grid_ptr)
    {
        goto MAINMENU_ADD_MENU_ERROR_HANDLEMENT;
    }        
    
    idle_grid_ptr->down_point = start_pos;
    
    MMIMAINMENU_GetItemData(shortcut_info.menu_id, shortcut_info.group_id, &menuitem_info);
    
    if (0 == menuitem_info.group_id)
    {
        img_id = menuitem_info.icon.data.icon_id;
    }
#ifdef DYNAMIC_MAINMENU_SUPPORT    
    else
        //if (GUIMENU_MAINMENU_DATA_TYPE_DYNAMIC == menuitem_info.menu_data_type)
        {
            img_id  = menuitem_info.active_icon.data.icon_id;//menuitem_info.menu_data.dynamic_menu.ori_data_ptr->dynamic_menu_info.select_icon_id;
        }
#endif

        GUIRES_GetImgWidthHeight(&img_width, &img_height, img_id, idle_grid_ptr->win_id);
        
        rect.left = 0;
        rect.top  = MMI_STATUSBAR_HEIGHT;
        rect.right = rect.left + img_width - 1;
        rect.bottom = rect.top + img_height - 1;
        
        if(MMIWIDGET_LayoutGridTblByRectEx(idle_grid_ptr, idle_grid_ptr->cur_page_index, rect, &valid_grid, &dst_rect))
        {                        
            for (i = 0; i < MMIQBTHEME_MAX_WIDGET_PAGE_NUM*MMI_IDLE_GRID_PAGE_CELL_NUM; i++)
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
            
            if (MMIQBTHEME_MAX_WIDGET_PAGE_NUM*MMI_IDLE_GRID_PAGE_CELL_NUM == i)
            {
                //MMIQBIDLE_DEBUG:"find  icon_ptr falied"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIQBTHEME_IDLEWIN_2461_112_2_18_3_9_6_287,(uint8*)"");
                goto MAINMENU_ADD_MENU_ERROR_HANDLEMENT;
            }
            //SCI_TRACE_LOW:"idle_grid_ptr->icon_ptr[%d].icon_info. group_id = %d, menu_id = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIQBTHEME_IDLEWIN_2464_112_2_18_3_9_6_288,(uint8*)"ddd", idle_grid_ptr->icon_ptr[i].icon_info.group_id, idle_grid_ptr->icon_ptr[i].icon_info.menu_id);
            idle_grid_ptr->widget_mov_type = MMIGRID_TP_MOVE_IDLE_TO_IDLE;
            SetGridTPClickType(idle_grid_ptr, MMIGRID_TP_CLICK_LONG_SHORTCUT);
            idle_grid_ptr->tp_move_info.click_id = i;  
            SaveTpMovePoint(idle_grid_ptr, start_pos);            
            if(idle_grid_ptr->run_mode != MMIGRID_RUN_MODE_EDITING)
            {
                MMIQBIDLE_SetWidgetWinMode(idle_grid_ptr, MMIGRID_RUN_MODE_EDITING);
                //set dockbar in editing mode    
                MMIQBTHEME_VirtualWinSetFSMState(p_dockbar, MMIQBTHEME_EDITING);            
                //MMIQBTHEME_VirtualWinHandleMsg( p_dockbar,  p_dockbar->display.win_id, MSG_FULL_PAINT , 0);
            }
            MMIQBIDLE_SetNewPageIndex(valid_grid.page);
            return TRUE;
        }
        else
        {
            //MMIQBIDLE_DEBUG:"MMIWIDGET_LayoutGridTblByRect falied"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIQBTHEME_IDLEWIN_2480_112_2_18_3_9_6_289,(uint8*)"");
        }
MAINMENU_ADD_MENU_ERROR_HANDLEMENT:            
        MMIPUB_OpenAlertFailWin(TXT_QB_NO_MORE_ROOM_ON_WIDGET_SCREEN);
        return FALSE;
        
        
}

/*****************************************************************************/
//  Description : add new widget to idle.
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIQBIDLE_AddNewWidget(uint32 widget_id)
{
    MMI_IDLE_GRID_DATA_T *idle_grid_ptr = PNULL;
    MMI_GRID_FILLED_INFO_T       valid_grid    = {0};
    GUI_RECT_T            rect          = {0};
    GUI_RECT_T            dst_rect      = {0};
    MMIWIDGET_ITEM_T      item_info     = {0};
    uint16                img_width     = 0;
    uint16                img_height    = 0;
    MMI_IMAGE_ID_T        bg_img        = 0;
    idle_grid_ptr = MMIAPIQBIDLE_GetGridPtr(); 
    
    if(PNULL == idle_grid_ptr )
    {
        return FALSE;
    }  

    if(widget_id == 0 || widget_id >= MMIWIDGET_WIDGET_MAX)
    {   
        return FALSE;
    }
    item_info = idle_grid_ptr->item_ptr[widget_id - 1];

    if (PNULL != item_info.item_info.GetWidgetImage)
    {
        item_info.item_info.GetWidgetImage(&bg_img, PNULL);
    }
    else
    {
        bg_img = item_info.item_info.large_bg_img;
    }     
    GUIRES_GetImgWidthHeight(&img_width, &img_height, item_info.item_info.large_bg_img, idle_grid_ptr->win_id);
    

    if(idle_grid_ptr->current_gird != -1
            &&   idle_grid_ptr->current_gird < MMIQBTHEME_GRID_PAGE_CELL_NUM)
    {     
        if(!IS_NODE_EMPTY(idle_grid_ptr->grid_data_tab[idle_grid_ptr->cur_page_index][idle_grid_ptr->current_gird]))
        {
            idle_grid_ptr->current_gird = MMIWIDGET_GetFirtstValidGrid(idle_grid_ptr, 
                idle_grid_ptr->cur_page_index);
        }
        if(idle_grid_ptr->current_gird == -1 
            || idle_grid_ptr->current_gird >= MMIQBTHEME_GRID_PAGE_CELL_NUM)
        {
            idle_grid_ptr->current_gird  = 0;
        }   
    }

    if((idle_grid_ptr->current_gird != -1) && (  idle_grid_ptr->current_gird < MMIQBTHEME_GRID_PAGE_CELL_NUM))
    {
        rect = idle_grid_ptr->page_grid_rect[idle_grid_ptr->current_gird];
    }
    rect.right = rect.left + img_width - 1;
    rect.bottom = rect.top + img_height - 1;
 
#ifdef MMI_GRID_CONTACT	
    if ((MMIWIDGET_CONTACT_ADD_ID == widget_id))
    {
        int32 i = 0;
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
        int32 i = 0;
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

    //if(MMIWIDGET_LayoutGridTblByRect(idle_grid_ptr, idle_grid_ptr->cur_page_index, rect, &valid_grid, &dst_rect))    
    if(MMIWIDGET_LayoutGridTblByRectEx(idle_grid_ptr, idle_grid_ptr->cur_page_index, rect, &valid_grid, &dst_rect))
    {
        //当前页有空间可以放下widget时，再完成下面的操作
        MMIWIDGET_FillGridTbl(idle_grid_ptr,&valid_grid.filled_grid_cell, valid_grid.page,widget_id,MMIGRID_ITEM_TYPE_WIDGET);       

        MMIWIDGET_RemoveWidgetInAddingList(idle_grid_ptr, widget_id);
        MMIWIDGET_ActiveWidgetItem(idle_grid_ptr, widget_id);
        //MMIQBIDLE_SaveWidgetInfoToNV(MMIAPIQBIDLE_GetGridPtr());         
        MMIQBIDLE_SetNewPageIndex(valid_grid.page);
        return TRUE;
    }
    else
    {
        //MMIQBIDLE_DEBUG:"MMIWIDGET_LayoutGridTblByRect falied"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIQBTHEME_IDLEWIN_2564_112_2_18_3_9_7_290,(uint8*)"");
    }
    MMIPUB_OpenAlertFailWin(TXT_QB_NO_MORE_ROOM_ON_WIDGET_SCREEN);
    return FALSE;
}

/*****************************************************************************/
//  Description : add new shortcut item to idle.
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIQBIDLE_AddNewShortcut(MMIMAINMENU_ITEM_INFO_T *menu_info_ptr)
{
    uint32 i = 0;
    MMI_GRID_FILLED_INFO_T       valid_grid    = {0};
    GUI_RECT_T            rect          = {0};
    GUI_RECT_T            dst_rect      = {0};
    MMI_IDLE_GRID_DATA_T *idle_grid_ptr = MMIAPIQBIDLE_GetGridPtr(); 
    
    if (PNULL == idle_grid_ptr)
    {
        return FALSE ;
    }    
    
    if (PNULL == menu_info_ptr)
    {
        goto ADD_MENU_ERROR_HANDLEMENT ;
    } 
    
    if(idle_grid_ptr->current_gird == -1 
        || idle_grid_ptr->current_gird >= MMIQBTHEME_GRID_PAGE_CELL_NUM)
    {
        idle_grid_ptr->current_gird  = 0;
    }
    if(!IS_NODE_EMPTY(idle_grid_ptr->grid_data_tab[idle_grid_ptr->cur_page_index][idle_grid_ptr->current_gird]))
    {
        idle_grid_ptr->current_gird = MMIWIDGET_GetFirtstValidGrid(idle_grid_ptr, 
                                                idle_grid_ptr->cur_page_index);
    }
    if(idle_grid_ptr->current_gird == -1 
        || idle_grid_ptr->current_gird >= MMIQBTHEME_GRID_PAGE_CELL_NUM)
    {
        //goto ADD_MENU_ERROR_HANDLEMENT ;
        idle_grid_ptr->current_gird  = 0;
    }
    rect = idle_grid_ptr->page_grid_rect[idle_grid_ptr->current_gird];
     

    //if(MMIWIDGET_LayoutGridTblByRect(idle_grid_ptr, idle_grid_ptr->cur_page_index, rect, &valid_grid, &dst_rect))    
    if(MMIWIDGET_LayoutGridTblByRectEx(idle_grid_ptr, idle_grid_ptr->cur_page_index, rect, &valid_grid, &dst_rect))
    {            
        for (i = 0; i < MMIQBTHEME_MAX_WIDGET_PAGE_NUM*MMI_IDLE_GRID_PAGE_CELL_NUM; i++)
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
        
        if (MMIQBTHEME_MAX_WIDGET_PAGE_NUM*MMI_IDLE_GRID_PAGE_CELL_NUM == i)
        {
            //MMIQBIDLE_DEBUG:"find icon ptr falied"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIQBTHEME_IDLEWIN_2651_112_2_18_3_9_7_292,(uint8*)"");
            goto ADD_MENU_ERROR_HANDLEMENT ;
        }
        MMIWIDGET_FillGridTbl(idle_grid_ptr,&valid_grid.filled_grid_cell, valid_grid.page, i, MMIGRID_ITEM_TYPE_ICON);
        //MMIQBIDLE_SaveWidgetInfoToNV(MMIAPIQBIDLE_GetGridPtr());   
        MMIQBIDLE_SetNewPageIndex(valid_grid.page);
        return TRUE;
    }
    else
    {
        //MMIQBIDLE_DEBUG:"MMIWIDGET_LayoutGridTblByRect falied"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIQBTHEME_IDLEWIN_2660_112_2_18_3_9_7_293,(uint8*)"");
    }
ADD_MENU_ERROR_HANDLEMENT:            
    MMIPUB_OpenAlertFailWin(TXT_QB_NO_MORE_ROOM_ON_WIDGET_SCREEN);
    return FALSE;
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
    int32        grid_index = 0;
    point.x = MMK_GET_TP_X(param);
    point.y = MMK_GET_TP_Y(param);

    ResetWidgetTpState(idle_grid_ptr);    
    idle_grid_ptr->down_point = point;
    press_type = MMIWIDGET_GetGridTpPressedItem(idle_grid_ptr, point, &click_id);

    if (MMIGRID_ITEM_TYPE_WIDGET == press_type)
    {       
        if (MMIWIDGET_INVALID_ID != click_id)
        { 
            child_win_handle = MMIWIDGET_GetWinHandleByWidgetId(idle_grid_ptr, click_id);
            if(MMIGRID_RUN_MODE_NORMAL != idle_grid_ptr->run_mode)
            {
                goto HANDLE_WIDGET_TPDOWN_LABEL;
            }            
            //MMK_SetChildWinFocus(idle_grid_ptr->win_id, child_win_handle);
            if (MMI_RESULT_TRUE == MMIWIDGET_RunChildWidgetProc(idle_grid_ptr, click_id, MSG_WIDGET_TP_PRESS_DOWN, param))
            {
                idle_grid_ptr->is_child_win_btn  = TRUE;   
                //DrawCurrentPage(idle_grid_ptr);.
                DrawSpecificPage(idle_grid_ptr, idle_grid_ptr->cur_page_index);
            }       
HANDLE_WIDGET_TPDOWN_LABEL:     
            idle_grid_ptr->tp_move_info.click_id = click_id;
            SetGridTPClickType(idle_grid_ptr, MMIGRID_TP_CLICK_WIDGET);       
        }
    }
    else if(MMIGRID_ITEM_TYPE_ICON == press_type)
    {  
        idle_grid_ptr->tp_move_info.click_id = GET_NODE_ID(idle_grid_ptr->grid_data_tab[idle_grid_ptr->cur_page_index][click_id]);
        SetGridTPClickType(idle_grid_ptr, MMIGRID_TP_CLICK_SHORTCUT); 

        if(MMIGRID_RUN_MODE_NORMAL == idle_grid_ptr->run_mode )
        {
            GUI_RECT_T          display_rc = {0};
            grid_index = MMIWIDGET_GetGridIndexByPos(idle_grid_ptr, point);
            display_rc = idle_grid_ptr->page_grid_rect[grid_index];
            SetBufferAlphaEx(&idle_grid_ptr->page_layer, &display_rc, MMIMENU_WEAKLAYERALPHA);
            GUILCD_InvalidateRect(0, display_rc, 0);
        }    
    }
    else
    {    
        idle_grid_ptr->tp_move_info.click_id = MMIWIDGET_INVALID_ID;
        SetGridTPClickType(idle_grid_ptr, MMIGRID_TP_CLICK_IDLE_EMPTY); 
    }
    SaveTpMovePoint(idle_grid_ptr, point);
    return TRUE;
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
    
    GUI_RECT_T dockbar_rect = {0};
    MMIQBTHEME_VIRTUALWIN_T *p_dockbar =    MMIQBTHEME_GetDockbarPtr();
    
    if (   MMIGRID_RUN_MODE_NORMAL == idle_grid_ptr->run_mode
        && MMIGRID_TP_MOVE_NONE == idle_grid_ptr->widget_mov_type        
         && MMIGRID_TP_CLICK_WIDGET == idle_grid_ptr->tp_move_info.click_type
        )
    {
        idle_grid_ptr->is_child_win_tp_move = FALSE;
        
        if (MMIWIDGET_RunChildWidgetProc(idle_grid_ptr, idle_grid_ptr->tp_move_info.click_id, MSG_WIDGET_TP_PRESS_MOVE, param))
        {
            idle_grid_ptr->is_child_win_tp_move = TRUE;
            return FALSE;
        }
    }  
    
    point.x = MMK_GET_TP_X(param);
    point.y = MMK_GET_TP_Y(param);
    
    if (!idle_grid_ptr->tp_move_is_valid)
    {
        if (!IsTpXMoveValid(idle_grid_ptr, point) && !IsTpYMoveValid(idle_grid_ptr, point))
        {
            return FALSE;
        }
/*
        if(idle_grid_ptr->is_child_win_btn && !idle_grid_ptr->is_child_win_tp_move)
        {
           return FALSE;
        }
*/
        idle_grid_ptr->tp_move_is_valid = TRUE;
    }
    
    MMIQBTHEME_VirtualWinGetRect(p_dockbar,  &dockbar_rect );
    if (MMIGRID_TP_MOVE_IDLE_TO_IDLE == idle_grid_ptr->widget_mov_type)
    {        
        if( !GUI_PointIsInRect( point, dockbar_rect))         
        {
            if(CheckMovingEmptyGrid(idle_grid_ptr, &rect))
            {              
                DrawRect(idle_grid_ptr, rect, MMI_GREEN_COLOR);
            }            
        }
        else
        {
            if (idle_grid_ptr->showed_rect_info.is_valid)
            {
                ClearRect(idle_grid_ptr);    
                idle_grid_ptr->showed_rect_info.is_valid = FALSE;
            }          
        }
    }
    
    if (   MMIGRID_TP_CLICK_WIDGET  == idle_grid_ptr->tp_move_info.click_type
        || MMIGRID_TP_CLICK_SHORTCUT == idle_grid_ptr->tp_move_info.click_type
        || MMIGRID_TP_CLICK_IDLE_EMPTY == idle_grid_ptr->tp_move_info.click_type)
    {
        if( !GUI_PointIsInRect( point, dockbar_rect))     
        {
#ifdef PDA_UI_SUPPORT_U_IDLE
            if (   MMIGRID_TP_MOVE_PAGE != idle_grid_ptr->widget_mov_type
                && MMIUIdle_IsOpen())
            { 
                idle_grid_ptr->is_Uidle_1st_mov = TRUE;
            }
#endif        
            
            if (idle_grid_ptr->widget_mov_type != MMIGRID_TP_MOVE_PAGE)
            {
                idle_grid_ptr->widget_mov_type = MMIGRID_TP_MOVE_PAGE;
                MMIWIDGET_DispatchMsgToActiveChildWin(idle_grid_ptr, MSG_WIDGET_PAGE_SLIDE_START);
            }          
            
#ifdef  PDA_UI_SUPPORT_U_IDLE
            if (MMIUIdle_IsOpen())
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
        }
        return TRUE;
    }
    
    if(MMIGRID_TP_CLICK_LONG_SHORTCUT == idle_grid_ptr->tp_move_info.click_type)
    {
        if( !GUI_PointIsInRect( point, dockbar_rect))
        {                
            idle_grid_ptr->widget_mov_type = MMIGRID_TP_MOVE_IDLE_TO_IDLE;
            MoveWidgetIdle2Idle(idle_grid_ptr, point);      
        }
        else
        {                    
            if(0 != MMIQBTHEME_MoveMenu2DockbarPage2(idle_grid_ptr, point))
            {
                MMIQBTHEME_MoveIcon2DockbarPage3(idle_grid_ptr,  point, MMIGRID_ITEM_TYPE_ICON);
            }                    
        }
        
    }
    
    
    if (MMIGRID_TP_CLICK_LONG_WIDGET == idle_grid_ptr->tp_move_info.click_type)
    {            
            if( !GUI_PointIsInRect( point, dockbar_rect))
            {
                if( GUI_PointIsInRect( idle_grid_ptr->save_point, dockbar_rect))
                {                               
                    MMIQBTHEME_PAGE_T*      p_page = p_dockbar->p_page[2];
                    if(p_page->type == MMIQBTHEME_PAGE_DOCKBAR_PAGE3)
                    {
                        //交还给widget页面的代码控制     
                        if (UILAYER_IsBltLayer(&idle_grid_ptr->thumb_move_layer)
                            && !UILAYER_IsBltLayer(&idle_grid_ptr->move_layer)
                            )    
                        {
                            UILAYER_APPEND_BLT_T append_layer = {0};
                            UILAYER_INFO_T layer_info= {0};
                            GUI_POINT_T offset_point= {0};
                            DestroyThumbMoveLayer(idle_grid_ptr);  
                            //set layer position
                            UILAYER_GetLayerInfo(&idle_grid_ptr->move_layer, &layer_info);
                            offset_point.x =  point.x - layer_info.layer_width / 2;
                            offset_point.y =  point.y - layer_info.layer_height / 2;
                            UILAYER_SetLayerPosition(&idle_grid_ptr->move_layer, offset_point.x, offset_point.y);
                            append_layer.lcd_dev_info = idle_grid_ptr->move_layer;
                            append_layer.layer_level = UILAYER_LEVEL_NORMAL;
                            UILAYER_AppendBltLayer(&append_layer); 
                            idle_grid_ptr->save_point = point;
                        }      
                    }
                }
                idle_grid_ptr->widget_mov_type = MMIGRID_TP_MOVE_IDLE_TO_IDLE;
                //ProcessWidgetMove(idle_grid_ptr, point, idle_grid_ptr->widget_mov_type);                
                MoveWidgetIdle2Idle(idle_grid_ptr, point);      
            }
            else
            {
                MMIQBTHEME_MoveIcon2DockbarPage3(idle_grid_ptr,  point, MMIGRID_ITEM_TYPE_WIDGET);
            }
    } 
    
    idle_grid_ptr->save_point = point;
    
    
    
    return TRUE;
}

/*****************************************************************************/
//  Description : set widget layer blt
//  Global resource dependence : 
//  Author:hongbo.lan
//  Note: 
/*****************************************************************************/
PUBLIC void MMIQBIDLE_SetBltLayer(
                       MMI_IDLE_GRID_DATA_T *idle_grid_ptr
                       )
{
    UILAYER_APPEND_BLT_T append_layer = {0};
    MMIQBTHEME_VIRTUALWIN_T *p_dockbar =    MMIQBTHEME_GetDockbarPtr();
    
    if(UILAYER_IsLayerActive(&idle_grid_ptr->wallpaper_layer))
    {
        append_layer.lcd_dev_info = idle_grid_ptr->wallpaper_layer;
        append_layer.layer_level = UILAYER_LEVEL_NORMAL;
        UILAYER_AppendBltLayer(&append_layer);    
    }
    
    if (UILAYER_IsLayerActive(&idle_grid_ptr->background_layer))
    {
        append_layer.lcd_dev_info = idle_grid_ptr->background_layer;
        append_layer.layer_level = UILAYER_LEVEL_NORMAL;
        UILAYER_AppendBltLayer(&append_layer);    
    }

    if (UILAYER_IsLayerActive(&idle_grid_ptr->page_layer))
    {
        append_layer.lcd_dev_info = idle_grid_ptr->page_layer;
        append_layer.layer_level = UILAYER_LEVEL_NORMAL;
        UILAYER_AppendBltLayer(&append_layer);   
    }   
    
    if (UILAYER_IsLayerActive(&idle_grid_ptr->thumb_move_layer))
    {
        append_layer.lcd_dev_info = idle_grid_ptr->thumb_move_layer;
        append_layer.layer_level = UILAYER_LEVEL_NORMAL;
        UILAYER_AppendBltLayer(&append_layer);    
    }
    else
    {
        if (UILAYER_IsLayerActive(&idle_grid_ptr->move_layer))
        {
            append_layer.lcd_dev_info = idle_grid_ptr->move_layer;
            append_layer.layer_level = UILAYER_LEVEL_NORMAL;
            UILAYER_AppendBltLayer(&append_layer);    
        }    
    }    
    
    MMIQBTHEME_AppendDisplay(&p_dockbar->display);
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
    if (PNULL == idle_grid_ptr)
    {
        //SCI_TRACE_LOW:"DestroyMoveLayer() PNULL == idle_grid_ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIQBTHEME_IDLEWIN_3293_112_2_18_3_9_8_298,(uint8*)"");
        return;
    }
    
    if (UILAYER_HANDLE_MULTI == UILAYER_GetHandleType(&idle_grid_ptr->page_layer))    
    {
        UILAYER_RELEASELAYER(&(idle_grid_ptr->page_layer));   /*lint !e506 !e774*/
        //        UILAYER_ReleaseLayer(&(idle_grid_ptr->page_layer));
        //        idle_grid_ptr->page_layer.block_id = UILAYER_NULL_HANDLE;
    }
}


/*****************************************************************************/
//  Description : create widget layer
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note: 
/*****************************************************************************/
LOCAL void CreateThumbMoveLayer(MMI_IDLE_GRID_DATA_T *idle_grid_ptr, MMIGRID_ITEM_TYPE_E elem_type)
{
    GUI_LCD_DEV_INFO widget_layer_handle = {0};
    UILAYER_CREATE_T create_info = {0};
    MMI_TP_STATUS_E tp_status = MMI_TP_UP;
    GUI_POINT_T point = {0};
    
    if (PNULL == idle_grid_ptr)
    {
        //SCI_TRACE_LOW:"CreateMoveLayer() PNULL == idle_grid_ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIQBTHEME_IDLEWIN_3435_112_2_18_3_9_8_300,(uint8*)"");
        return;
    }
    
    if (UILAYER_HANDLE_MULTI != UILAYER_GetHandleType(&idle_grid_ptr->thumb_move_layer))    
    {
        if (MMIGRID_ITEM_TYPE_WIDGET == elem_type)
        {   
            MMK_GetLogicTPMsg(&tp_status, &point);            
            
            
            create_info.width = MMIQBTHEME_THUMB_WIDGET_THUMB_SIZE;
            create_info.height = MMIQBTHEME_THUMB_WIDGET_THUMB_SIZE  ;       
            
            create_info.offset_x = point.x - MMIQBTHEME_THUMB_WIDGET_THUMB_SIZE / 2;
            create_info.offset_y = point.y - MMIQBTHEME_THUMB_WIDGET_THUMB_SIZE / 2;
            
            
            create_info.lcd_id = LCD_ID_0;
            create_info.owner_handle = idle_grid_ptr->win_id;
            create_info.is_bg_layer = FALSE;
            create_info.is_static_layer = FALSE;  
            create_info.format = UILAYER_MEM_IMMUTABLE;
            UILAYER_CreateLayer(  &create_info, &widget_layer_handle );
            UILAYER_SetLayerColorKey(&widget_layer_handle, TRUE, UILAYER_TRANSPARENT_COLOR);
            idle_grid_ptr->thumb_move_layer = widget_layer_handle;
        }
    }
    
    if (UILAYER_IsLayerActive(&widget_layer_handle))
    {
        UILAYER_Clear(&widget_layer_handle);
    }  
}

LOCAL void DestroyThumbMoveLayer(
                                 MMI_IDLE_GRID_DATA_T *idle_grid_ptr
                                 )
{
    if (PNULL == idle_grid_ptr)
    {
        //SCI_TRACE_LOW:"DestroyMoveLayer() PNULL == idle_grid_ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIQBTHEME_IDLEWIN_3476_112_2_18_3_9_8_301,(uint8*)"");
        return;
    }
    
    if (UILAYER_HANDLE_MULTI == UILAYER_GetHandleType(&idle_grid_ptr->thumb_move_layer))    
    {
        UILAYER_RELEASELAYER(&(idle_grid_ptr->thumb_move_layer));   /*lint !e506 !e774*/
    }
}



/*****************************************************************************/
//  Description : update all screen
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note: 
/*****************************************************************************/
PUBLIC void MMIQBIDLE_UpdateAllScreen(void)
{
    MMI_IDLE_GRID_DATA_T *idle_grid_ptr = MMIAPIQBIDLE_GetGridPtr();
    
    // UILAYER_Clear(&idle_grid_ptr->move_layer);
    
    //widget应用如果打开新窗口则不用刷新
    if (!MMK_IsFocusWin(idle_grid_ptr->win_id))
    {
        return ;
    }
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

    //condition check before sliding page
    if (idle_grid_ptr->widget_mov_type != MMIGRID_TP_MOVE_PAGE)
    {
        idle_grid_ptr->widget_mov_type = MMIGRID_TP_MOVE_PAGE;
        MMIWIDGET_DispatchMsgToActiveChildWin(idle_grid_ptr, MSG_WIDGET_PAGE_SLIDE_START);
    }           
   

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
    GetPageLayerPosition( idle_grid_ptr,&lcd_dev_info, &offset_x, &offset_y);
    offset_x += (point.x - idle_grid_ptr->save_point.x);
    
    
        //改变page_layer的位置          
        UILAYER_SetDirectDraw(TRUE);        
        
        SetPageLayerPosition( idle_grid_ptr,&lcd_dev_info, offset_x, offset_y);

        SetWallpaperDeltaPosition(idle_grid_ptr, (point.x - idle_grid_ptr->save_point.x), TRUE);
    

    
    //    MMITHEME_UpdateRect();
    
    SaveTpMovePoint(idle_grid_ptr, point);
    
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
    MMIQBIDLE_SetBltLayer(idle_grid_ptr);
#if 0
    GUILCD_Invalidate(MAIN_LCD_ID,0);
    SCI_SLEEP(500);
#endif
    if(idle_grid_ptr->cur_page_index == idle_grid_ptr->new_page_index)
    {
        DrawPageLayer(idle_grid_ptr);        
    }
    else
    {
        MMIQBTHEME_SetWidgetPreviewPageFocusedItem(idle_grid_ptr->new_page_index)  ;  
        MMIQBTHEME_SlideWidgetPreviewPage2FocusedItem();
        MMIQBTHEME_HandleDockbarMsg(idle_grid_ptr, MSG_FULL_PAINT,0);        
        SlidePage(idle_grid_ptr, idle_grid_ptr->new_page_index);
        idle_grid_ptr->cur_page_index = idle_grid_ptr->new_page_index;
    }
    
    CopyWallpaper2MainLayer(idle_grid_ptr);  
#if defined BACKGROUND_GRID_LINE_LAYER
    DrawAllBackground(idle_grid_ptr);
#endif    
    DrawPageTitleBar(idle_grid_ptr);    
}

LOCAL void DrawCurrentPage(MMI_IDLE_GRID_DATA_T *idle_grid_ptr)
{
    MMIQBIDLE_SetBltLayer(idle_grid_ptr);
    CopyWallpaper2MainLayer(idle_grid_ptr); 
#if defined BACKGROUND_GRID_LINE_LAYER
    DrawAllBackground(idle_grid_ptr);
#endif
    DrawPageTitleBar(idle_grid_ptr);
    DrawSpecificPage(idle_grid_ptr, idle_grid_ptr->cur_page_index);
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
            MMIQBIDLE_SetCurPageIndex(idle_grid_ptr->slide_anim.dst_page_index);
            MMIWIDGET_ReCalcAllWidgetWinRect(idle_grid_ptr);  
            MMIWIDGET_DispatchMsgToActiveChildWin(idle_grid_ptr, MSG_WIDGET_PAGE_SLIDE_END);
            ResetGridPageLayerPos(idle_grid_ptr); 
            DrawPageLayer(idle_grid_ptr);
            DrawPageTitleBar(idle_grid_ptr);
            
            //set dockbar preview page focused item
            MMIQBTHEME_SetWidgetPreviewPageFocusedItem(idle_grid_ptr->cur_page_index)  ;  
            MMIQBTHEME_SlideWidgetPreviewPage2FocusedItem();
            MMIQBTHEME_HandleDockbarMsg(idle_grid_ptr, MSG_FULL_PAINT,0);
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
    if(0 == (idle_grid_ptr->widget_mov_type & MMIGRID_TP_MOVE_PAGE))    /*lint !e655 */
    {
        idle_grid_ptr->widget_mov_type |= MMIGRID_TP_MOVE_PAGE; /*lint !e655 */            
        //s_focus_index = -1;
        MMIWIDGET_DispatchMsgToActiveChildWin(idle_grid_ptr, MSG_WIDGET_PAGE_SLIDE_START);
    }                
    UILAYER_SetLayerMemFormat(&idle_grid_ptr->page_layer, UILAYER_MEM_FPS_ACCELERATE);    
    src_page_index = idle_grid_ptr->cur_page_index;
    GetPageLayerPosition( idle_grid_ptr, &idle_grid_ptr->page_layer, &offset_x, &offset_y);    
    MMIWIDGET_DEBUG(LOG_STR("timer = %d, offset_x = %d"), SCI_GetTickCount(), offset_x);

        idle_grid_ptr->slide_anim.slide_flag = slide_flag;
        idle_grid_ptr->slide_anim.offset_x = offset_x;
        idle_grid_ptr->slide_anim.offset_y = offset_y;
        idle_grid_ptr->slide_anim.page_width = MMIWIDGET_GetWidgetPageWidth(idle_grid_ptr);
        
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
//  Author:ying.xu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN SlidePage(
                        MMI_IDLE_GRID_DATA_T *idle_grid_ptr,
                        int32 dest_page_index
                        )
{
    int32 current_page_index = idle_grid_ptr->cur_page_index;
    int16 direction = 0;
    int32 delta_page_num = 0;
    MMIWIDGET_PEN_MOVE_DIRECT_T left_right = 0;
    int32 frame = 4;
    
    if(dest_page_index == current_page_index)
    {
        return FALSE;            
    }
    
    if (idle_grid_ptr->widget_mov_type != MMIGRID_TP_MOVE_PAGE)
    {
        idle_grid_ptr->widget_mov_type = MMIGRID_TP_MOVE_PAGE;
        MMIWIDGET_DispatchMsgToActiveChildWin(idle_grid_ptr, MSG_WIDGET_PAGE_SLIDE_START);
    }           
    
    //page index增减的方向
    delta_page_num = dest_page_index - current_page_index;

    if((delta_page_num) > 0)
    {
        direction = 1;
        left_right = MMIWIDGET_PEN_MOVE_LEFT; 
    }
    else
    {
        direction = -1;
        left_right = MMIWIDGET_PEN_MOVE_RIGHT; 
    }

    if(abs(delta_page_num) >= 4)
    {
        frame = 1;
    }
    else if(abs(delta_page_num) >= 2)
    {
        frame = 2;
    }
    while(current_page_index != dest_page_index)
    {
        current_page_index += direction;
        SlideOnePage(idle_grid_ptr,current_page_index,left_right,frame);
        //GUILCD_Invalidate(0,0);
    } 

#ifdef MMI_IDLE_WIDGET_CACHE
    MMIWIDGET_ReleaseWidgetCacheInOnePage(idle_grid_ptr, dest_page_index);
#endif
   
    //idle_grid_ptr->widget_mov_type = MMIGRID_TP_MOVE_NONE;
    idle_grid_ptr->widget_mov_type = MMIGRID_TP_MOVE_IDLE_TO_IDLE;
    MMIQBIDLE_SetCurPageIndex(dest_page_index);  
    MMIWIDGET_ReCalcAllWidgetWinRect(idle_grid_ptr);
    MMIWIDGET_DispatchMsgToActiveChildWin(idle_grid_ptr, MSG_WIDGET_PAGE_SLIDE_END);
    
	DrawSpecificPageEx(idle_grid_ptr, idle_grid_ptr->page_layer, 
                  idle_grid_ptr->cur_page_index, 0, 0);     
    DrawPageTitleBar(idle_grid_ptr);    
    
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
    int16 delta_x = 0;
    int32 current_page_index = idle_grid_ptr->cur_page_index;
    int32 page_slide_num = 0;
    int32 i = 0;
    int32 page_width = MMIWIDGET_GetWidgetPageWidth( idle_grid_ptr);        
    GUI_RECT_T display_rect = idle_grid_ptr->display_rect;
    
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
        page_slide_num = ( idle_grid_ptr->page_num + dest_page_index - current_page_index) % idle_grid_ptr->page_num;   
        delta_x = -page_slide_num*page_width/frame;     //16 frames
    }
    else if (MMIWIDGET_PEN_MOVE_RIGHT == direction)   
    {
        page_slide_num = -( idle_grid_ptr->page_num - dest_page_index + current_page_index) % idle_grid_ptr->page_num;           
        delta_x = -page_slide_num*page_width/frame;     //16 frames
    }
    else
    {
        return FALSE;
    }        
   
    
    //send  foucs msg to page that will be displayed
    MMIWIDGET_DispatchMsgToChildInOnePage(idle_grid_ptr,dest_page_index, 
                             MSG_GET_FOCUS ,0); 
                             
    MMIQBIDLE_SetBltLayer(idle_grid_ptr);
    for(i = 0; i < frame ; i++)
    { 
        UILAYER_SetLayerMemFormat(&idle_grid_ptr->page_layer, UILAYER_MEM_FPS_ACCELERATE);
        UILAYER_SwitchDoubleMem( &idle_grid_ptr->page_layer, TRUE );
        UILAYER_SetDirectDraw(TRUE);    
        
        offset_x += delta_x;
        UILAYER_ClearRect(&idle_grid_ptr->page_layer, display_rect);
        DrawSpecificPageEx(idle_grid_ptr, idle_grid_ptr->page_layer, 
                            current_page_index, 0, offset_x);    
        DrawSpecificPageEx(idle_grid_ptr, idle_grid_ptr->page_layer,
                            dest_page_index, page_slide_num,offset_x);
        SetWallpaperDeltaPosition(idle_grid_ptr, delta_x, TRUE);  
        //GUILCD_InvalidateRect( 0,  display_rect, 0 );
        MMITHEME_UpdateRect();
#ifdef WIN32        
        SCI_SLEEP(10);
#endif
    }   

#ifdef MMI_IDLE_WIDGET_CACHE
    MMIWIDGET_ReleaseWidgetCacheInOnePage(idle_grid_ptr, current_page_index);
#endif
    
    //send lose foucs msg to page that will not be displayed    
    MMIWIDGET_DispatchMsgToChildInOnePage(idle_grid_ptr,current_page_index, 
                             MSG_LOSE_FOCUS ,0); 
   
        //draw new buffer
    UILAYER_SetLayerMemFormat(&idle_grid_ptr->page_layer, UILAYER_MEM_FPS_ACCELERATE);
    MMIQBIDLE_SetCurPageIndex(dest_page_index);
    DrawPageTitleBar(idle_grid_ptr);

    return TRUE;
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
    
    //page 1 - 6
    if(abs(offset_x) >= (lcd_width/4) )
    {       
        move_direct = (offset_x < 0) ? MMIWIDGET_PEN_MOVE_LEFT : MMIWIDGET_PEN_MOVE_RIGHT;
        page_offset = (offset_x < 0) ? 1 : -1;
        dest_page_index = (idle_grid_ptr->cur_page_index + page_offset 
                    + idle_grid_ptr->page_num) % idle_grid_ptr->page_num;
    }
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
    UILAYER_CREATE_T create_info = {0};
    GUI_RECT_T rect = {0};
    uint32      page_layer_size = MMIWIDGET_GetPageLayerSize(idle_grid_ptr);   

    if(1 == page_layer_size)
    {        
        rect = MMITHEME_GetFullScreenRect();
        rect.top = idle_grid_ptr->display_rect.top;
    }
    
    if (UILAYER_HANDLE_MULTI != UILAYER_GetHandleType(&idle_grid_ptr->page_layer))    
    {        
        width = (uint16)(rect.right+1-rect.left)*page_layer_size;  

        height = (rect.bottom+1-rect.top );
        create_info.lcd_id = LCD_ID_0;
        create_info.owner_handle = idle_grid_ptr->win_id;
        create_info.offset_x = rect.left;
        create_info.offset_y = rect.top;
        create_info.width = width;
        create_info.height = height;
        create_info.is_bg_layer = FALSE;
        create_info.is_static_layer = FALSE; 
        create_info.format = UILAYER_MEM_FPS_ACCELERATE;
        
        UILAYER_CreateLayer(
            &create_info,
            &widget_layer_handle
            );
        idle_grid_ptr->page_layer = widget_layer_handle;

    }
    if (UILAYER_IsLayerActive(&widget_layer_handle))
    {
        UILAYER_Clear(&widget_layer_handle);     
    }    
}



/*****************************************************************************/
//  Description :  set current page
//  Global resource dependence : 
//  Author:  hongbo.lan
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIQBIDLE_SetCurPageIndex(
                                       int32 page_index
                                       )
{
    MMI_IDLE_GRID_DATA_T *idle_grid_ptr = MMIAPIQBIDLE_GetGridPtr();
    
    idle_grid_ptr->cur_page_index = page_index;
    idle_grid_ptr->new_page_index = page_index;
    
    return TRUE;
}

PUBLIC BOOLEAN MMIQBIDLE_SetNewPageIndex(
                                       int32 page_index
                                       )
{
    MMI_IDLE_GRID_DATA_T *idle_grid_ptr = MMIAPIQBIDLE_GetGridPtr();
    
    idle_grid_ptr->new_page_index = page_index;
    return TRUE;
}

/*****************************************************************************/
//  Description :  slide page
//  Global resource dependence : 
//  Author:  hongbo.lan
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIQBIDLE_SlidePage(
                                       int32 page_index
                                       )
{
    MMI_IDLE_GRID_DATA_T *idle_grid_ptr = MMIAPIQBIDLE_GetGridPtr();    
    SlidePage(idle_grid_ptr, page_index);
    return TRUE;
}

/*****************************************************************************/
//  Description : draw page title bar.
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note:
/*****************************************************************************/
LOCAL void DrawPageTitleBar(MMI_IDLE_GRID_DATA_T *idle_grid_ptr)
{
    
    int32 i = 0;
    GUI_POINT_T start_point = {0};
    uint16 img_w = 0;
    uint16 temp_img_w = 0;
    uint16 img_h = 0;
    MMI_IMAGE_ID_T img_id = 0;
    GUI_LCD_DEV_INFO  lcd_dev_info = {0};
    GUI_RECT_T     title_rect = {0};      
    int32 interval_x = 0;
    int32 start_x = 0;
    
    if(PNULL == idle_grid_ptr)
    {
        return;
    }
    title_rect = GetWidgetPageTitleRect(idle_grid_ptr);    

    if (UILAYER_IsLayerActive(&idle_grid_ptr->background_layer))
    {        
        lcd_dev_info = idle_grid_ptr->background_layer;
        UILAYER_ClearRect(&lcd_dev_info, title_rect);        
    }  
    else
    {        
        lcd_dev_info =  *MMITHEME_GetDefaultLcdDev();           
    }       
    
    GUIRES_GetImgWidthHeight(&img_w, &img_h,IMAGE_THEME_TAG_DEACTIVE, idle_grid_ptr->win_id);
    
    //为了和主菜单统一，主菜单最大可以有10页，widget page只有9页
    interval_x = (title_rect.right - title_rect.left + 1 
        - (MMIQBTHEME_MAX_WIDGET_PAGE_NUM + 1) * img_w) / (MMIQBTHEME_MAX_WIDGET_PAGE_NUM + 2);
    
    
    start_x = (title_rect.right + title_rect.left - idle_grid_ptr->page_num * (img_w+interval_x) + interval_x)/2;
    
    for (i = 0; i < idle_grid_ptr->page_num; i++)
    {        
        if (i == idle_grid_ptr->cur_page_index)
        {                
            img_id = IMAGE_THEME_TAG_PAGE1 + i;
        }
        else
        {            
            img_id = IMAGE_THEME_TAG_DEACTIVE;
        }
        
        GUIRES_GetImgWidthHeight(&temp_img_w, &img_h,img_id, idle_grid_ptr->win_id);
        
        start_point.x =  (int16)(start_x + (interval_x + (int32)img_w) * i) - (int16)(temp_img_w - img_w)/2 ;   
        start_point.y = (title_rect.top + title_rect.bottom - img_h)/2 ;
        
        
        GUIRES_DisplayImg(&start_point, 
            PNULL, 
            PNULL, 
            idle_grid_ptr->win_id,
            img_id, 
            &lcd_dev_info);            
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

LOCAL void SetBufferAlphaEx(GUI_LCD_DEV_INFO *layer_ptr, GUI_RECT_T *rect_ptr, uint32 alpha)
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
            layer_rect = UILAYER_GetLayerRect(layer_ptr);        
            if (!GUI_IntersectRect(&mask_rect, *rect_ptr, layer_rect))
            {
                return;
            }
        } 
        LCD_ConvertDispRect(GUILCD_CONVERT_POS_LCD2DISP , layer_ptr, &mask_rect, &mask_rect);
        mask_height = mask_rect.bottom - mask_rect.top + 1;
        mask_width  = mask_rect.right - mask_rect.left + 1;
    
        bk_buf_ptr = ((uint32*)layer_buf_ptr/*layer_info.layer_buf_ptr*/) + mask_rect.top * layer_info.mem_width + mask_rect.left; 
        
        for (i = 0; i < mask_height; i++)
        {    
            for (j = 0; j < mask_width; j++)
            {         
                color = *(bk_buf_ptr + j);
                //color &=  0xffffff;
                *(bk_buf_ptr + j) = (color)?\
                    (((((color >> 24) * alpha) >> 8) << 24) | (color & 0xffffff)) : color ; 
            }
            
            bk_buf_ptr += layer_info.mem_width;
        }   
}

/*****************************************************************************/
//  Description : create widget layer
//  Global resource dependence : 
//  Author:hongbo.lan
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN CreatePreviewIconPageLayer(
                                        MMI_WIN_ID_T    win_id,
                                      GUI_LCD_DEV_INFO *lcd_dev_info_ptr                                      
                                      )
{
    uint16 width = 0;
    uint16 height = 0;
    UILAYER_CREATE_T create_info = {0};
 
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&width,&height);  
 
    create_info.lcd_id = LCD_ID_0;
    create_info.owner_handle = win_id;
    create_info.offset_x = 0;
    create_info.offset_y = 0;
    create_info.width = width;
    create_info.height = height;
    create_info.is_bg_layer = FALSE;
    create_info.is_static_layer = FALSE; 
    create_info.format = 0;
    
    UILAYER_CreateLayer(&create_info, lcd_dev_info_ptr);
    if(UILAYER_IsLayerActive(lcd_dev_info_ptr))
    {
        UILAYER_Clear(lcd_dev_info_ptr);
        return TRUE;
    }
    else
    {        
         //MMIQBIDLE_DEBUG:"failed"
         SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIQBTHEME_IDLEWIN_5027_112_2_18_3_9_12_303,(uint8*)"");
         return FALSE;
    }

}

/*****************************************************************************/
//  Description : zoom in page to thumb on main layer.
//  Global resource dependence : 
//  Author:  hongbo.lan
//  Note:
/*****************************************************************************/
PUBLIC int32 MMIQBIDLE_DisplayGridPageThumbView(int32 page_index)
{
    UILAYER_INFO_T      layer_info = {0};
    GUI_LCD_DEV_INFO    thumb_layer = {0};
    uint8 *temp_buf   = PNULL;
    uint16 lcd_width  = 0;
    uint16 lcd_height = 0;
    GUI_RECT_T screen_rect = MMITHEME_GetFullScreenRect();
    uint8 *tar_buf   = PNULL;
    int32 tar_buf_size = 0;
    GUI_RECT_T  dest_rect = {0};
    MMI_IDLE_GRID_DATA_T *idle_grid_ptr = MMIAPIQBIDLE_GetGridPtr();   
    GUI_LCD_DEV_INFO lcd_dev_info = {0};    
    MMIQBTHEME_VIRTUALWIN_T *p_dockbar =  MMIQBTHEME_GetDockbarPtr();
    int32 result = 0;
    DISPLAY_DATA_TYPE_E dst_data_type = DATA_TYPE_ARGB888;
    if(0 == memcmp(idle_grid_ptr->grid_data_tab[page_index],
                idle_grid_ptr->previous_grid_data_tab[page_index],
                sizeof(idle_grid_ptr->grid_data_tab[page_index])))
    {
        //same, on need update preview icon
        return -1;        
    }
    SCI_MEMCPY(idle_grid_ptr->previous_grid_data_tab[page_index],
                idle_grid_ptr->grid_data_tab[page_index],
                sizeof(idle_grid_ptr->grid_data_tab[page_index]));

    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);    
    
    lcd_dev_info = MMIWIDGET_CreateLayer(idle_grid_ptr->win_id, &screen_rect, dst_data_type);    
    temp_buf = UILAYER_GetLayerBufferPtr(&lcd_dev_info);
    if(0 == temp_buf)
    {    
        result = -2;
        goto RETURN_LABEL;
    }
    
    thumb_layer = p_dockbar->p_page[2]->p_item[page_index]->display.lcd_dev_info;    
    tar_buf = UILAYER_GetLayerBufferPtr(&thumb_layer);
    if(0 == tar_buf)
    {    
        //MMIQBIDLE_DEBUG:"p_item[%d].lcd_dev_info invalid"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIQBTHEME_IDLEWIN_5088_112_2_18_3_9_12_304,(uint8*)"d",page_index);        
        result = -3;
        goto RETURN_LABEL;
    }
    UILAYER_Clear(&thumb_layer);
    UILAYER_GetLayerInfo(&thumb_layer, &layer_info); 
    
    dest_rect = UILAYER_GetLayerRect(&thumb_layer);
     
    tar_buf_size =   layer_info.layer_width * layer_info.layer_height * 4 ;  
    
    DrawSpecificPageEx(idle_grid_ptr,  lcd_dev_info,  page_index,  0, 0 );
    ThumbLayer(&lcd_dev_info,  &screen_rect, &thumb_layer, &dest_rect,  dst_data_type,  0);
    /*        
    {   
    char path[64] = "d:\\page_layer.raw";
    FILE *fp = 0;
    
      sprintf(path, "d:\\page_layer%d.raw", page_index);
      
        fp = fopen(path, "w+b");
        
          fwrite(temp_buf, 1, lcd_width*lcd_height*4, fp);
          
            fclose(fp);
            }
    */     
    
    if (!ThumbLayer(&lcd_dev_info,  &screen_rect, &thumb_layer, &dest_rect,  dst_data_type,  0))
    {
        //MMIQBIDLE_DEBUG:"GRAPH_ScaleImage failed"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIQBTHEME_IDLEWIN_5139_112_2_18_3_9_12_305,(uint8*)"");        
        result = -4;
    }
    
    //set background color  
    SetBufferAlpha(tar_buf, 
        layer_info.mem_width,
        DATA_TYPE_ARGB888,
        layer_info.layer_width, 
        layer_info.layer_height);  
RETURN_LABEL:    
    UILAYER_RELEASELAYER(&lcd_dev_info);     /*lint !e506 !e774*/   
    MMIWIDGET_ReCalcWidgetWinRectInOnePage(idle_grid_ptr, page_index);    
    return result;
}

//
LOCAL int32 ThumbMoveLayer(void)
{
    SCALE_IMAGE_IN_T    scale_in  = {0};
    SCALE_IMAGE_OUT_T   scale_out = {0};
    UILAYER_INFO_T      src_layer_info = {0};
    UILAYER_INFO_T      dst_layer_info = {0};
    uint8 *src_buf   = PNULL;
    uint16 src_width  = 0;
    uint16 src_height = 0;
    uint8 *tar_buf   = PNULL; 
    int32 tar_buf_size = 0;
    
    
    int32 target_width = 0;
    int32 target_height = 0;
    MMI_IDLE_GRID_DATA_T *idle_grid_ptr = MMIAPIQBIDLE_GetGridPtr();   
    
    src_buf = UILAYER_GetLayerBufferPtr(&idle_grid_ptr->move_layer);   
    UILAYER_GetLayerInfo(&idle_grid_ptr->move_layer, &src_layer_info);
    src_width  = src_layer_info.layer_width;
    src_height = src_layer_info.layer_height;    
    
    tar_buf = UILAYER_GetLayerBufferPtr(&idle_grid_ptr->thumb_move_layer);     
    UILAYER_GetLayerInfo(&idle_grid_ptr->thumb_move_layer, &dst_layer_info);
    target_width = dst_layer_info.layer_width;
    target_height = dst_layer_info.layer_height;    
    tar_buf_size =   target_width * target_height * 4;         
    
    scale_in.src_format           = IMGREF_FORMAT_ARGB888;
    scale_in.src_chn.chn0.ptr     = src_buf;
    scale_in.src_chn.chn0.size    = src_width * src_height * 4 ;
    scale_in.src_size.w           = src_width;
    scale_in.src_size.h           = src_height;        
    scale_in.src_trim_rect.x      = 0;
    scale_in.src_trim_rect.y      = 0;
    scale_in.src_trim_rect.w      = src_width;
    scale_in.src_trim_rect.h      = src_height;
    scale_in.target_buf.size      = tar_buf_size;          
    scale_in.target_buf.ptr       = tar_buf;  
    scale_in.scale_mode           = SCALE_MODE_DISTORT; 
    
    scale_in.target_format        = IMGREF_FORMAT_ARGB888;
    scale_in.target_size.w        = target_width;
    scale_in.target_size.h        = target_height;
    
    if (SCI_SUCCESS != GRAPH_ScaleImage(&scale_in,&scale_out))
    {
        //MMIQBIDLE_DEBUG:"GRAPH_ScaleImage failed"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIQBTHEME_IDLEWIN_5193_112_2_18_3_9_12_306,(uint8*)"");
        return -3;
    }    
    
    SetBufferAlpha(tar_buf,
        dst_layer_info.mem_width,
        DATA_TYPE_ARGB888,
        target_width, 
        target_height);  

    return 0;
}

#ifdef PDA_UI_SUPPORT_U_IDLE
/*****************************************************************************/
//  Description : create page NO. layer
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void CreatePageNoLayer(MMI_IDLE_GRID_DATA_T *idle_grid_ptr)
{
    GUI_LCD_DEV_INFO widget_layer_handle = {0}; 
    GUI_RECT_T rect = {0};
    UILAYER_CREATE_T create_info = {0};  
    uint16 width  = 0;
    uint16 height = 0;    
    uint16 img_width  = 0;
    uint16 img_height = 0; 
    
    rect = GetWidgetPageTitleRect(idle_grid_ptr);
    
    GUIRES_GetImgWidthHeight(&img_width, &img_height, IMAGE_THEME_DESKTOP_DOCKBAR, idle_grid_ptr->win_id);

    width  = rect.right - rect.left + 1;
    height = rect.bottom - rect.top + 1 + img_height;  

    if (UILAYER_HANDLE_MULTI != UILAYER_GetHandleType(&idle_grid_ptr->background_layer))    
    {
        create_info.lcd_id          = LCD_ID_0;
        create_info.owner_handle    = idle_grid_ptr->win_id;
        create_info.offset_x        = rect.left;
        create_info.offset_y        = rect.top;
        create_info.width           = width;
        create_info.height          = height;
        create_info.is_bg_layer     = FALSE;
        create_info.is_static_layer = FALSE;
        create_info.format          = UILAYER_MEM_IMMUTABLE;
        
        UILAYER_CreateLayer(&create_info, &widget_layer_handle);
        
        idle_grid_ptr->background_layer = widget_layer_handle;
    }
    
    if (UILAYER_IsLayerActive(&widget_layer_handle))
    {
        UILAYER_SetLayerPosition(&widget_layer_handle, rect.left, rect.top);
        UILAYER_Clear(&widget_layer_handle);
    }
   
}
#endif

/*****************************************************************************/
//  Description : create page title layer
//  Global resource dependence : 
//  Author: ying.xu
//  Note: 
/*****************************************************************************/
LOCAL void CreatePageTitleLayer(MMI_IDLE_GRID_DATA_T *idle_grid_ptr)
{
    GUI_LCD_DEV_INFO widget_layer_handle = {0};
    uint16 width = 0;
    uint16 height = 0;
    GUI_RECT_T rect = {0};
    UILAYER_CREATE_T create_info = {0};  
    
    uint32      page_layer_size = MMIWIDGET_GetPageLayerSize(idle_grid_ptr); 

    if(1 == page_layer_size)
    {
#ifndef  PDA_UI_SUPPORT_U_IDLE   
        idle_grid_ptr->background_layer = idle_grid_ptr->page_layer;         
#else
        CreatePageNoLayer(idle_grid_ptr);
#endif
        idle_grid_ptr->stb_layer = idle_grid_ptr->page_layer; 
        return;
    }

    rect = MMITHEME_GetFullScreenRect();
    width = rect.right - rect.left + 1;
    height = rect.bottom - rect.top + 1;
    
    if (UILAYER_HANDLE_MULTI != UILAYER_GetHandleType(&idle_grid_ptr->background_layer))    
    {
        create_info.lcd_id          = LCD_ID_0;
        create_info.owner_handle    = idle_grid_ptr->win_id;
        create_info.offset_x        = rect.left;
        create_info.offset_y        = rect.top;
        create_info.width           = width;
        create_info.height          = height;
        create_info.is_bg_layer     = FALSE;
        create_info.is_static_layer = FALSE;
        create_info.format          = UILAYER_MEM_FPS_ACCELERATE;
        
        UILAYER_CreateLayer(&create_info, &widget_layer_handle);
        
        idle_grid_ptr->background_layer = widget_layer_handle; //使用softkey的layer指针        
        idle_grid_ptr->stb_layer = widget_layer_handle; //使用softkey的layer指针
    }
    
    if (UILAYER_IsLayerActive(&widget_layer_handle))
    {
        UILAYER_Clear(&widget_layer_handle);
    }
   
}

/*****************************************************************************/
//  Description : destroy page title layer
//  Global resource dependence : 
//  Author: ying.xu
//  Note: 
/*****************************************************************************/
LOCAL void DestroyPageTitleLayer(
                                 MMI_IDLE_GRID_DATA_T *idle_grid_ptr
                                 )
{
    if (PNULL == idle_grid_ptr)
    {
        //SCI_TRACE_LOW:"DestroyElveLayer() PNULL == idle_grid_ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIQBTHEME_IDLEWIN_5268_112_2_18_3_9_12_307,(uint8*)"");
        return;
    }
    
    if (UILAYER_HANDLE_MULTI == UILAYER_GetHandleType(&idle_grid_ptr->background_layer))
    {
        UILAYER_RELEASELAYER(&(idle_grid_ptr->background_layer));   /*lint !e506 !e774*/
    }
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
    uint16 lcd_width = 0;
    uint16 lcd_height = 0;    

    
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);
    
    idle_grid_ptr->display_rect = GetWidgetPageDisplayRect(idle_grid_ptr);
    
    idle_grid_ptr->grid_row = MMIQBTHEME_GRID_ROW_NUM;
    idle_grid_ptr->grid_col = MMIQBTHEME_GRID_COL_NUM;
    
    idle_grid_ptr->grid_width  = lcd_width/MMIQBTHEME_GRID_COL_NUM;
    idle_grid_ptr->grid_height = (lcd_height - MMI_STATUSBAR_HEIGHT 
        - MMIQBTHEME_PAGE_TITLE_HEIGHT - MMIQBTHEME_DOCKBAR_HEIGHT)/MMIQBTHEME_GRID_ROW_NUM;
    
    for(j = 0;j < MMIQBTHEME_GRID_ROW_NUM;j++ )
    {
        for(k = 0; k < MMIQBTHEME_GRID_COL_NUM;k++)
        {
            idle_grid_ptr->page_grid_rect[j*MMIQBTHEME_GRID_COL_NUM + k].top    = MMI_STATUSBAR_HEIGHT + j*idle_grid_ptr->grid_height;
            idle_grid_ptr->page_grid_rect[j*MMIQBTHEME_GRID_COL_NUM + k].bottom = MMI_STATUSBAR_HEIGHT + j*idle_grid_ptr->grid_height + idle_grid_ptr->grid_height - 1;
            idle_grid_ptr->page_grid_rect[j*MMIQBTHEME_GRID_COL_NUM + k].left   = k*idle_grid_ptr->grid_width;
            idle_grid_ptr->page_grid_rect[j*MMIQBTHEME_GRID_COL_NUM + k].right  = k*idle_grid_ptr->grid_width + idle_grid_ptr->grid_width - 1;
        }
    }
}

LOCAL GUI_RECT_T GetWidgetPageTitleRect(
                                        MMI_IDLE_GRID_DATA_T *idle_grid_ptr                       
                                        )
{
    GUI_RECT_T     title_rect = MMITHEME_GetFullScreenRect();        
    
    title_rect.bottom -= MMIQBTHEME_DOCKBAR_HEIGHT ;
    title_rect.top = title_rect.bottom - MMIQBTHEME_PAGE_TITLE_HEIGHT + 1;
    return title_rect;
}

/*****************************************************************************/
//  Description : get widget page display rect.
//  Global resource dependence : 
//  Author: ying.xu
//  Note: 
/*****************************************************************************/
LOCAL GUI_RECT_T GetWidgetPageDisplayRect(MMI_IDLE_GRID_DATA_T *idle_grid_ptr) 
{
    GUI_RECT_T rect = {0};
    rect = MMITHEME_GetFullScreenRect();        
    
    rect.bottom -= MMIQBTHEME_DOCKBAR_HEIGHT + MMIQBTHEME_PAGE_TITLE_HEIGHT ;
    rect.top += MMI_STATUSBAR_HEIGHT;
    return rect;
}

PUBLIC int32 MMIQBIDLE_SetWidgetWinMode(MMI_IDLE_GRID_DATA_T *idle_grid_ptr,
                                         MMIGRID_RUN_MODE_E          run_mode
                                         )
{

    int32 i = 0;

    if(idle_grid_ptr->run_mode == MMIGRID_RUN_MODE_NORMAL
        && MMIGRID_RUN_MODE_EDITING == run_mode)
    {
        //设置上一次位置数据为垃圾数据，保证进入编辑状态时每一页的缩略图生成
        for(i = 0; i < (int32)idle_grid_ptr->page_num ; i++ )
        {
            SCI_MEMSET(idle_grid_ptr->previous_grid_data_tab[i] ,0xff,
                sizeof(uint16)*MMIQBTHEME_GRID_PAGE_CELL_NUM);
        }  
    }
    idle_grid_ptr->run_mode = run_mode;
    idle_grid_ptr->is_editable = (idle_grid_ptr->run_mode != MMIGRID_RUN_MODE_NORMAL)?1:0;
    return 0;
}

PUBLIC int32 MMIQBIDLE_ResetStateAfterMovingMenu2WidgetWin(GUI_POINT_T point)
{    
    MMI_IDLE_GRID_DATA_T        *idle_grid_ptr =MMIAPIQBIDLE_GetGridPtr();
       
    CreateMoveLayer(idle_grid_ptr, MMIGRID_ITEM_TYPE_ICON);    
    
    UILAYER_SetLayerPosition(&idle_grid_ptr->move_layer, point.x, point.y);
    MMIQBIDLE_SetBltLayer(idle_grid_ptr);
    //DrawSpecificPage (idle_grid_ptr, idle_grid_ptr->cur_page_index);
    MMIWIDGET_DispShortCutHightlight(idle_grid_ptr, FALSE);
    
    return 0;
}



LOCAL int32 MMIQBTHEME_MoveMenu2DockbarPage2(MMI_IDLE_GRID_DATA_T *idle_grid_ptr, GUI_POINT_T point)
{
    GUI_POINT_T offset_point = {0};
    MMIQBTHEME_VIRTUALWIN_T *p_dockbar =    MMIQBTHEME_GetDockbarPtr();
    int32 i = 0;
    MMIMAINMENU_ITEM_INFO_T menu_id = {0};
    MMIQBTHEME_ITEM_T *p_item = 0;
    MMI_MULTI_KEY_TP_MSG_PARAM_T tp_param = {0};
    
    if(p_dockbar->current_page == 1 //dockbar page1    
        && p_dockbar->p_page[1]->item_num < MMIQBTHEME_DOCKBAR_PAGE2_MAX_ITEM
        && GUI_PointIsInRect( point, p_dockbar->display.display_rect)
        )
    {                
        menu_id =  idle_grid_ptr->icon_ptr[idle_grid_ptr->tp_move_info.click_id].icon_info; 
        MMIWIDGET_RemoveShortcutInAddingList(idle_grid_ptr,idle_grid_ptr->tp_move_info.click_id);        
        UILAYER_GetLayerPosition(&idle_grid_ptr->move_layer, &offset_point.x,  &offset_point.y);
        DestroyMoveLayer(idle_grid_ptr);
        ResetWidgetTpState(idle_grid_ptr);        
        idle_grid_ptr->widget_mov_type = MMIGRID_TP_MOVE_NONE ;
        if(point.x < p_dockbar->p_page[1]->p_item[0]->display.display_rect.left)
        {
            i =0;
        }
        else if(point.x > p_dockbar->p_page[1]->p_item[p_dockbar->p_page[1]->item_num - 1]->display.display_rect.right)
        {
            i = p_dockbar->p_page[1]->item_num;
        }
        else
        {            
            for(i=0;i < p_dockbar->p_page[1]->item_num - 1; i++)
            {
                if(point.x >= p_dockbar->p_page[1]->p_item[i]->display.display_rect.left
                    && point.x <= p_dockbar->p_page[1]->p_item[i+1]->display.display_rect.right
                    )
                {
                    break;
                }
            }
            i++;
        }
        
        //recreate dockbar page2
        MMIQBTHEME_InsertMenuIconItem(i,  menu_id    );
        p_item = p_dockbar->p_page[1]->p_item[i];
        
        tp_param.cur_tp_point.x = (p_item->display.display_rect.right + p_item->display.display_rect.left)/2;
        tp_param.cur_tp_point.y = (p_item->display.display_rect.bottom + p_item->display.display_rect.top)/2;
        MMIQBTHEME_VirtualWinHandleMsg(p_dockbar,  0,  MSG_TP_PRESS_LONG, &tp_param);
        
        MMIQBTHEME_ItemMove( p_item, offset_point.x - p_item->display.display_rect.left,
            offset_point.y - p_item->display.display_rect.top);
        
        p_item->prevous_tp_point = idle_grid_ptr->save_point;
        tp_param.cur_tp_point = point;
        MMIQBTHEME_VirtualWinHandleMsg(p_dockbar,  0,  MSG_TP_PRESS_MOVE, &tp_param);
        
        return 0;
    }
    return -1;
    
}

//move widget/menu in dockbar page3
LOCAL int32 MMIQBTHEME_MoveIcon2DockbarPage3(MMI_IDLE_GRID_DATA_T *idle_grid_ptr, GUI_POINT_T point, 
                                             MMIGRID_ITEM_TYPE_E item_type)
{
    GUI_POINT_T offset_point = {0};
    GUI_POINT_T old_offset = {0};
    UILAYER_INFO_T layer_info = {0};
    MMIQBTHEME_VIRTUALWIN_T *p_dockbar =    MMIQBTHEME_GetDockbarPtr();
    GUI_LCD_DEV_INFO lcd_dev_info = {0};
    int32 i = 0;
    UILAYER_APPEND_BLT_T append_layer = {0};    
    
    
    if(item_type == MMIGRID_ITEM_TYPE_ICON)
    {
        lcd_dev_info = idle_grid_ptr->move_layer;
    }
    else
    {
        if (!UILAYER_IsBltLayer(&idle_grid_ptr->thumb_move_layer))    
        {
            CreateThumbMoveLayer( idle_grid_ptr,MMIGRID_ITEM_TYPE_WIDGET);
            ThumbMoveLayer();
            
            UILAYER_WeakLayerAlpha(&idle_grid_ptr->thumb_move_layer,MMIMENU_WEAKLAYERALPHA);

            UILAYER_RemoveBltLayer(&idle_grid_ptr->move_layer);  
            append_layer.lcd_dev_info = idle_grid_ptr->thumb_move_layer;
            append_layer.layer_level = UILAYER_LEVEL_NORMAL;
            UILAYER_AppendBltLayer(&append_layer);   
        }   
        lcd_dev_info = idle_grid_ptr->thumb_move_layer;
    }
    
    UILAYER_GetLayerPosition(&lcd_dev_info, &offset_point.x, &offset_point.y);
    UILAYER_GetLayerInfo(&lcd_dev_info, &layer_info);
    
    old_offset = offset_point;
    offset_point.x += (point.x - idle_grid_ptr->save_point.x);
    offset_point.y += (point.y - idle_grid_ptr->save_point.y);
    
    UILAYER_SetLayerPosition(&lcd_dev_info, offset_point.x, offset_point.y);    
    idle_grid_ptr->save_point = point;
    
    if(p_dockbar->current_page != 2)//dockbar page 3,preview icon page
    {
        return -1;        
    }
    if(!GUI_PointIsInRect( point, p_dockbar->p_page[2]->display.display_rect))
    {
        return -2;
    }
    for(i=0; i < p_dockbar->p_page[2]->item_num; i++)
    {
        if(GUI_PointIsInRect( point, p_dockbar->p_page[2]->p_item[i]->display.display_rect))
        {
            if(i != idle_grid_ptr->cur_page_index)
            {
                MMIQBTHEME_SetWidgetPreviewPageFocusedItem( i)  ;             
                MMIQBTHEME_HandleDockbarMsg(idle_grid_ptr, MSG_FULL_PAINT,0);                
                SlidePage(idle_grid_ptr, i);
            }
        }
    }
    
    return TRUE;
}

#if defined BACKGROUND_GRID_LINE_LAYER

LOCAL void DrawAllBackground(MMI_IDLE_GRID_DATA_T *idle_grid_ptr)
{    
    uint32      page_layer_size = MMIWIDGET_GetPageLayerSize(idle_grid_ptr);   
    if(1 == page_layer_size)
    {
        return;
    }
    DrawPageGridLine(idle_grid_ptr);
    DrawFocusedGrid(idle_grid_ptr);
}

/*****************************************************************************/
//  Description : DrawPageGridLine.
//  Global resource dependence : 
//  Author:hongbo.lan
//  Note: 
/*****************************************************************************/
LOCAL void DrawPageGridLine(MMI_IDLE_GRID_DATA_T *idle_grid_ptr)
{    
    int32 j = 0;
    GUI_LCD_DEV_INFO lcd_dev_info = {0};
    GUI_RECT_T main_rect = idle_grid_ptr->display_rect;    
    int32 offset_x = 0;
    
    if (PNULL == idle_grid_ptr)
    {
        //SCI_TRACE_LOW:"DrawSpecificPage() PNULL == idle_grid_ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIQBTHEME_IDLEWIN_5316_112_2_18_3_9_12_308,(uint8*)"");
        return;
    }   
    
    if (UILAYER_IsLayerActive(&idle_grid_ptr->background_layer))
    {        
        lcd_dev_info = idle_grid_ptr->background_layer;
        UILAYER_ClearRect(&lcd_dev_info, main_rect);
    }
    else
    {        
        lcd_dev_info =  *MMITHEME_GetDefaultLcdDev();           
    }
    
    if(idle_grid_ptr->run_mode != MMIGRID_RUN_MODE_EDITING)
    {
        return;
    }
    
    for(j = 1;j < MMIQBTHEME_GRID_ROW_NUM;j++ )
    {
        DrawLine(&lcd_dev_info, 
            idle_grid_ptr->page_grid_rect[j*MMIQBTHEME_GRID_COL_NUM].left + offset_x,
            idle_grid_ptr->page_grid_rect[j*MMIQBTHEME_GRID_COL_NUM].top,
            idle_grid_ptr->page_grid_rect[j*MMIQBTHEME_GRID_COL_NUM 
            + MMIQBTHEME_GRID_COL_NUM - 1].right + offset_x ,
            idle_grid_ptr->page_grid_rect[j*MMIQBTHEME_GRID_COL_NUM + MMIQBTHEME_GRID_COL_NUM - 1].top,
            MMI_WHITE_COLOR,128);
    }
    for(j = 1; j < MMIQBTHEME_GRID_COL_NUM; j++)
    {
        DrawLine(&lcd_dev_info, 
            idle_grid_ptr->page_grid_rect[j].left + offset_x,
            idle_grid_ptr->page_grid_rect[j].top,
            idle_grid_ptr->page_grid_rect[j + (MMIQBTHEME_GRID_ROW_NUM - 1)*MMIQBTHEME_GRID_COL_NUM ].left + offset_x ,
            idle_grid_ptr->page_grid_rect[j + (MMIQBTHEME_GRID_ROW_NUM - 1)*MMIQBTHEME_GRID_COL_NUM ].bottom,
            MMI_WHITE_COLOR,128);
    }     
    // UILAYER_WeakLayerAlpha(lcd_dev_info_ptr, 127);
}

LOCAL void DrawFocusedGrid(MMI_IDLE_GRID_DATA_T *idle_grid_ptr)
{
    MMIQBTHEME_VIRTUALWIN_T *p_dockbar = MMIQBTHEME_GetDockbarPtr();
    GUI_RECT_T      rect = {0};
    GUI_LCD_DEV_INFO lcd_dev_info = {0};       
        
    if(idle_grid_ptr->run_mode != MMIGRID_RUN_MODE_EDITING)
    {
        return;
    }
    if(p_dockbar->current_page != 0)
    {
        idle_grid_ptr->current_gird = -1 ;
        return;
    }
    
    if(idle_grid_ptr->current_gird == -1 
        || idle_grid_ptr->current_gird >= MMIQBTHEME_GRID_PAGE_CELL_NUM)
    {
        idle_grid_ptr->current_gird = 0;
    }
    if(!IS_NODE_EMPTY(idle_grid_ptr->grid_data_tab[idle_grid_ptr->cur_page_index][idle_grid_ptr->current_gird]))
    {
        idle_grid_ptr->current_gird = MMIWIDGET_GetFirtstValidGrid(idle_grid_ptr, 
            idle_grid_ptr->cur_page_index);
    }
    if(idle_grid_ptr->current_gird == -1 
        || idle_grid_ptr->current_gird >= MMIQBTHEME_GRID_PAGE_CELL_NUM)
    {
        return ;
    }
    rect = idle_grid_ptr->page_grid_rect[idle_grid_ptr->current_gird];

    if (UILAYER_IsLayerActive(&idle_grid_ptr->background_layer))
    {        
        lcd_dev_info = idle_grid_ptr->background_layer;
    } 
    else
    {        
        lcd_dev_info =  *MMITHEME_GetDefaultLcdDev();           
    }
    
    DrawFocusedGridFrame( rect, &lcd_dev_info);
    rect.left ++;
    rect.top ++;
    rect.right --;
    rect.bottom --;
    DrawFocusedGridFrame( rect, &lcd_dev_info);
}


LOCAL  uint32 RGB565TO888(uint32 rgb565)
{
    unsigned int r565 = (rgb565 >> 11) & 0x1f;
    unsigned int g565 = (rgb565 >> 5) & 0x3f;
    unsigned int b565 = rgb565 & 0x1f;
    unsigned int r888 = r565 << 3;
    unsigned int g888 = g565 << 2;
    unsigned int b888 = b565 << 3;
    unsigned int rgb888 = (r888 << 16) | (g888 << 8) | b888;

    return (rgb888 | 0xff000000); //set max alpha
}

LOCAL void DrawPoint( GUI_POINT_T pos,  uint32 color,
                                GUI_LCD_DEV_INFO  *dev_info_ptr)
{
    int16 dot_x = 0;
    int16 dot_y = 0;
    GUI_POINT_T offset_point = {0};
    uint8 *mem_ptr = 0;
    UILAYER_INFO_T layer_info = {0};
    uint16  bytes_per_dot = 0;
    
    UILAYER_GetLayerPosition(dev_info_ptr, &offset_point.x, &offset_point.y);
    UILAYER_GetLayerInfo(dev_info_ptr, &layer_info);
    mem_ptr = UILAYER_GetLayerBufferPtr( dev_info_ptr);
    dot_x = pos.x - offset_point.x ;
    if(dot_x < 0 || dot_x >=  layer_info.layer_width)
    {
        return;
    }
    dot_y = pos.y - offset_point.y ;
    if(dot_y < 0 || dot_y >=  layer_info.layer_height)
    {
        return;
    }

    if(layer_info.data_type == DATA_TYPE_ARGB888)
    {
        bytes_per_dot = 4;
        *(uint32*)(mem_ptr + ((layer_info.mem_width*dot_y 
                    + dot_x)*bytes_per_dot)) = color;
    }
    else if(layer_info.data_type == DATA_TYPE_RGB565)
    {
        bytes_per_dot = 2;
        color = RGB8882RGB565(color);
        *(uint16*)(mem_ptr + (layer_info.mem_width* (uint16)dot_y 
                    + (uint16)dot_x)*bytes_per_dot) = (uint16)color;
    }
    else
    {
        return;
    }

}
/*****************************************************************************/
//  Description : Draw LINE WITH ALPHA
//  Global resource dependence : 
//  Author: ying.xu
//  Note:
/*****************************************************************************/
LOCAL int32 DrawLine(GUI_LCD_DEV_INFO      *dev_info_ptr,  // block information
                    uint16                 x1,             //x coordinate of first point
                    uint16                 y1,             //y coordinate of first point
                    uint16                 x2,             //x coordinate of second point
                    uint16                 y2,             //y coordinate of second point
                    GUI_COLOR_T  color ,          //color to draw.
                    int32       alpha)
{
    int32 length = 0;
    int32 i = 0;
    int32 dx = 0;
    int32 dy = 0;
    GUI_POINT_T point = {0};
    uint32 temp_color = color;
    if(x1 == x2)
    {
        length = y2 - y1 + 1;
        dx = 0;
        dy = 1;
        if(length < 0)
        {
            length = 0 - length;
            dy= -1;
        }
    }
    else if(y1 == y2)
    {        
        length = x2 - x1 + 1;        
        dx = 1;
        dy = 0;
        if(length < 0)
        {
            length = 0 - length;
            dx = -1;
        }
    }
    else
    {
        return -1;
    }
  for(i = 0; i < length; i++)
  {
      temp_color = RGB565TO888(color);        
      temp_color = (uint32)((alpha << 24) | (temp_color & 0x00ffffff));
      point.x = x1 + dx*i;
      point.y = y1 + dy*i;
      DrawPoint(point, temp_color, dev_info_ptr);
  }
      return 0;
}             

/*****************************************************************************/
//  Description : Draw Focused grid frame
//  Global resource dependence : 
//  Author: ying.xu
//  Note:
/*****************************************************************************/
LOCAL int32 DrawGradientLine(GUI_POINT_T start_pos, GUI_POINT_T end_pos, 
                                uint32 color, 
                                GUI_LCD_DEV_INFO  *dev_info_ptr)
{
    int32 length = 0;
    int32 i = 0;
    int32 dx = 0;
    int32 dy = 0;
    GUI_POINT_T point = {0};
    uint32 temp_color = color;
    //LCD_DrawLine( dev_info_ptr, start_pos.x, start_pos.y,
     //           end_pos.x, end_pos.y, MMI_WHITE_COLOR );

    
    if(start_pos.x == end_pos.x)
    {
        length = end_pos.y - start_pos.y + 1;
        dx = 0;
        dy = 1;
        if(length < 0)
        {
            length = 0 - length;
            dy= -1;
        }
    }
    else if(start_pos.y == end_pos.y)
    {        
        length = end_pos.x - start_pos.x + 1;        
        dx = 1;
        dy = 0;
        if(length < 0)
        {
            length = 0 - length;
            dx = -1;
        }
    }
    else
    {
        return -1;
    }
    for(i = 0; i < length; i++)
    {
        temp_color = RGB565TO888(color);        
        temp_color = (uint32)(((64 * i / length + 192) << 24) | (temp_color & 0x00ffffff));
        point.x = start_pos.x + dx*i;
        point.y = start_pos.y + dy*i;
        DrawPoint(point, temp_color, dev_info_ptr);
    }
  //  LCD_DrawLine( dev_info_ptr, start_pos.x +  dx*i, start_pos.y + dy*i,
  //              end_pos.x, end_pos.y, color );
    return 0;
    
}

/*****************************************************************************/
//  Description : Draw Focused grid frame
//  Global resource dependence : 
//  Author: ying.xu
//  Note:
/*****************************************************************************/
LOCAL int32 DrawFocusedGridLine(GUI_POINT_T start_pos, GUI_POINT_T end_pos, 
                                uint32 color,
                                GUI_LCD_DEV_INFO  *dev_info_ptr)
{
    GUI_POINT_T middle_pos1 = {0};
    GUI_POINT_T middle_pos2 = {0};
    if(start_pos.x == end_pos.x)  
    {
        middle_pos1.x = start_pos.x ;
        middle_pos1.y = (start_pos.y + end_pos.y)/2;  
        middle_pos2.x = start_pos.x ;
        middle_pos2.y =  middle_pos1.y;  
    }
    else if(start_pos.y == end_pos.y)
    {        
        middle_pos1.y = start_pos.y ;
        middle_pos1.x = (start_pos.x + end_pos.x)/2;      
        middle_pos2.y = start_pos.y ;
        middle_pos2.x = middle_pos1.x ;  
    }
    else
    {
        return -1;
    }   
    DrawGradientLine(start_pos, middle_pos1, color, dev_info_ptr);
    DrawGradientLine(end_pos, middle_pos2, color,dev_info_ptr);    
    
    return 0;
}

/*****************************************************************************/
//  Description : Draw Focused grid frame
//  Global resource dependence : 
//  Author: ying.xu
//  Note:
/*****************************************************************************/
LOCAL int32 DrawFocusedGridFrame(GUI_RECT_T rect, GUI_LCD_DEV_INFO *dev_info_ptr)
{
    uint32 color = MMI_WHITE_COLOR;
    GUI_POINT_T start_pos = {0};
    GUI_POINT_T end_pos = {0};

    if(!UILAYER_IsLayerActive(dev_info_ptr))
    {
        return -1;
    }
    //left line
    start_pos.x = rect.left;
    start_pos.y = rect.top;
    end_pos.x = rect.left;
    end_pos.y = rect.bottom;
    DrawFocusedGridLine(start_pos, end_pos,color , dev_info_ptr);  

    //top line
    start_pos.x = rect.left;
    start_pos.y = rect.top;
    end_pos.x = rect.right;
    end_pos.y = rect.top;
    DrawFocusedGridLine(start_pos, end_pos,color, dev_info_ptr);    

    
    //right line
    start_pos.x = rect.right;
    start_pos.y = rect.top;
    end_pos.x = rect.right;
    end_pos.y = rect.bottom;
    DrawFocusedGridLine(start_pos, end_pos, color, dev_info_ptr);    


    //bottom line
    start_pos.x = rect.left;
    start_pos.y = rect.bottom;
    end_pos.x = rect.right;
    end_pos.y = rect.bottom;
    DrawFocusedGridLine(start_pos, end_pos, color, dev_info_ptr); 
        
    return 0;
}
#endif
#ifdef PDA_UI_SUPPORT_U_IDLE
/*****************************************************************************/
//  Description : draw last frame for uidle effect.
//  Global resource dependence : 
//  Author:hongbo.lan
//  Note: 
/*****************************************************************************/
LOCAL void DrawUIdleLastFrame(MMI_IDLE_GRID_DATA_T *idle_grid_ptr, uint16 dst_page_idx)
{

    idle_grid_ptr->widget_mov_type = MMIGRID_TP_MOVE_NONE;
    ResetWidgetTpState(idle_grid_ptr);
    
    MMIQBIDLE_SetBltLayer(idle_grid_ptr);

    UILAYER_SetLayerMemFormat(&idle_grid_ptr->page_layer, UILAYER_MEM_FPS_ACCELERATE);  

    MMIQBIDLE_SetCurPageIndex(idle_grid_ptr->cur_page_index);
    
    //send  foucs msg to page that will be displayed
    MMIWIDGET_DispatchMsgToChildInOnePage(idle_grid_ptr, idle_grid_ptr->cur_page_index,
                             MSG_GET_FOCUS ,0); 
    MMIWIDGET_ReCalcAllWidgetWinRect(idle_grid_ptr); 
    MMIWIDGET_DispatchMsgToActiveChildWin(idle_grid_ptr, MSG_WIDGET_PAGE_SLIDE_END);
    DrawSpecificPage(idle_grid_ptr, idle_grid_ptr->cur_page_index);
    DrawPageTitleBar(idle_grid_ptr);
    //set dockbar preview page focused item
    MMIQBTHEME_SetWidgetPreviewPageFocusedItem(idle_grid_ptr->cur_page_index)  ;  
    MMIQBTHEME_SlideWidgetPreviewPage2FocusedItem();
    MMIQBTHEME_HandleDockbarMsg(idle_grid_ptr, MSG_FULL_PAINT,0);    
}
#endif 

#ifdef LIVE_WALLPAPER_FRAMEWORK_SUPPORT
/*****************************************************************************/
//  Description : MMIAPIGRID_GetMainAppFgInfo
//  Global resource dependence : 
//  Author:arvin.wu
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIQBTHEME_GetMainAppFgInfo(LW_FG_INFO *lw_fg_info_ptr)
{
    MMI_IDLE_GRID_DATA_T *idle_grid_ptr = PNULL;
    uint16 lcd_width = 0;
    uint16 lcd_height = 0;
    int16 offset_x = 0;
    int16 offset_y = 0;
    
    idle_grid_ptr = MMIAPIQBIDLE_GetGridPtr();


    if(idle_grid_ptr && lw_fg_info_ptr)
    {
        lw_fg_info_ptr->disp_dev_info.lcd_id = 0;
        lw_fg_info_ptr->disp_dev_info.block_id = 0;

        GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &lcd_width, &lcd_height); 
        
        lw_fg_info_ptr->lw_fg_pos_info.rc.left = 0;
        lw_fg_info_ptr->lw_fg_pos_info.rc.right = (idle_grid_ptr->page_num*lcd_width);
        lw_fg_info_ptr->lw_fg_pos_info.rc.top = 0;
        lw_fg_info_ptr->lw_fg_pos_info.rc.bottom = lcd_height ;
		GetPageLayerPosition( idle_grid_ptr,  &idle_grid_ptr->page_layer, &offset_x,&offset_y);
		offset_x = -offset_x +  idle_grid_ptr->cur_page_index * MMIWIDGET_GetWidgetPageWidth(idle_grid_ptr);
        lw_fg_info_ptr->lw_fg_pos_info.y = 0;
    }
    
}
#endif

