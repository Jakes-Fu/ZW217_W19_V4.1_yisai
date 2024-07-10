/*****************************************************************************
** File Name:      mmiwidget_item.c                                          *
** Author:                                                                   *
** Date:           08/10/2011                                                *
** Copyright:      2011 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe widge element,              *
**                    such as widget/menuicon                                *   
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 10/8/2012     ying.xu              Creat
******************************************************************************/

#ifndef _MMIWIDGET_ITEM_H_
#define _MMIWIDGET_ITEM_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
 
#ifdef __cplusplus
         extern   "C"
         {
#endif

/*****************************************************************************/
//  Description : is valid widget
//  Global resource dependence :
//  Author: hongbo.lan
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIWIDGET_IsValidWidgetId(
                              uint32 widget_id
                              );

/*****************************************************************************/
//  Description : find widget item
//  Global resource dependence :
//  Author: hongbo.lan
//  Note: 
/*****************************************************************************/
PUBLIC MMIWIDGET_ITEM_T *MMIWIDGET_FindWidgetItem(
                                       MMI_IDLE_GRID_DATA_T *idle_grid_ptr,
                                       uint32 widget_id
                                       );

/*****************************************************************************/
//  Description : redraw widget item
//  Global resource dependence : 
//  Author:hongbo.lan
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIWIDGET_RedrawWidgetItem( 
                                        MMIWIDGET_ITEM_T *item_ptr
                                      );

/*****************************************************************************/
//  Description : widget子窗口的消息处理函数
//  Global resource dependence : 
//  Author:  hongbo.lan
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIWIDGET_RunChildWidgetProc(
                                      MMI_IDLE_GRID_DATA_T  *idle_grid_ptr,
                                      uint32            widget_id,
                                      MMI_MESSAGE_ID_E  msg_id,
                                      DPARAM            param
                                      );

PUBLIC MMI_RESULT_E MMIWIDGET_RunChildWidgetProcEx(
                                      MMI_IDLE_GRID_DATA_T  *idle_grid_ptr,
                                      uint32            widget_id,
                                      MMI_MESSAGE_ID_E  msg_id,
                                      DPARAM            param,                                      
                                      BOOLEAN           is_handle_focus
                                      );

PUBLIC BOOLEAN MMIWIDGET_CreateWidgetItem(
                                   MMI_IDLE_GRID_DATA_T *idle_grid_ptr,                                     
                                   MMI_WIN_ID_T win_id,
                                   const MMIWIDGET_ITEM_T* add_item_ptr,                                   
                                   PROCESSMSG_FUNC          func               //窗口的回调函数
                                   );

/*****************************************************************************/
//  Description : active widget
//  Global resource dependence :
//  Author: hongbo.lan
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIWIDGET_ActiveWidgetItem(
                               MMI_IDLE_GRID_DATA_T *idle_grid_ptr,
                               uint32 widget_id
                               );

/*****************************************************************************/
//  Description : DE-active widget item
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIWIDGET_DeactiveWidgetItem(
                                 MMI_IDLE_GRID_DATA_T *idle_grid_ptr,
                                 int32 widget_id
                                 );

/*****************************************************************************/
//  Description : destroy item data
//  Global resource dependence : 
//  Author:hongbo.lan
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIWIDGET_DestroyWidgetItem(
                                MMI_IDLE_GRID_DATA_T * idle_grid_ptr,  // the list control pointer
                                MMIWIDGET_ITEM_T *item_ptr
                                );
PUBLIC BOOLEAN MMIWIDGET_DestroyAllWidgetItemData(
                                       MMI_IDLE_GRID_DATA_T*       idle_grid_ptr  // the list control pointer
                                       );

/*****************************************************************************/
//  Description : set child win rect
//  Global resource dependence : 
//  Author:hongbo.lan
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIWIDGET_SetChildWinRect(
                              MMI_HANDLE_T win_handle,
                              GUI_RECT_T rect,
                              BOOLEAN is_main_screen_landscape
                              );


#ifdef MMI_IDLE_WIDGET_CACHE
PUBLIC int32 MMIWIDGET_UpdateWidgetCache(MMIWIDGET_ITEM_T *p_item_info);  //write to cache
PUBLIC int32 MMIWIDGET_DrawWidget(MMIWIDGET_ITEM_T *p_item_info, GUI_LCD_DEV_INFO *p_lcd_dev, GUI_RECT_T *p_rect);
PUBLIC int32 MMIWIDGET_ReleaseWidgetCacheInAllPage(MMI_IDLE_GRID_DATA_T *idle_grid_ptr);
PUBLIC int32 MMIWIDGET_ReleaseWidgetCacheInOnePage(MMI_IDLE_GRID_DATA_T *idle_grid_ptr, int32 page_idx);
PUBLIC int32 MMIWIDGET_InitWidgetCacheControlData(MMI_IDLE_GRID_DATA_T *idle_grid_ptr);
PUBLIC int32 MMIWIDGET_ClearWidgetCacheControlData(MMI_IDLE_GRID_DATA_T *idle_grid_ptr);
PUBLIC int32 MMIWIDGET_CreateWidgetCache(MMIWIDGET_ITEM_T *p_item_info);
PUBLIC int32 MMIWIDGET_IsWidgetCacheValid(MMIWIDGET_ITEM_T *p_item_info);
 /*****************************************************************************/
 //  Description : release one page widget cache
 //  Global resource dependence : 
 //  Author:
 //  Note: 
 /*****************************************************************************/
 PUBLIC int32 MMIWIDGET_ReleaseAllWidgetsCache(MMI_IDLE_GRID_DATA_T *idle_grid_ptr);
#endif

/*****************************************************************************/
//  Description : 重新恢复到主层上（跟SetHitWidgetTopmost对应）
//  Global resource dependence : 
//  Author:hongbo.lan
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWIDGET_ResetHitWidgetLayer(
                               MMI_IDLE_GRID_DATA_T *idle_grid_ptr
                               );
                               
PUBLIC GUI_LCD_DEV_INFO * MMIWIDGET_ResetOneWidgetLayer(
                                             MMI_IDLE_GRID_DATA_T *idle_grid_ptr,
                                             uint32 widget_id
                                             );
                                             
PUBLIC int32 MMIWIDGET_ShortCutDrawMenuIcon(MMI_WIN_ID_T win_id,
                                    CTRLMAINMENU_ITEM_T* menu_item_ptr, 
                                    GUI_RECT_T display_rc,
                                    GUI_LCD_DEV_INFO* lcd_dev_info,
                                    MMI_IMAGE_ID_T delete_image_id,
                                    BOOLEAN is_editing_mode);

PUBLIC void MMIWIDGET_DispShortCutHightlight(MMI_IDLE_GRID_DATA_T *idle_grid_ptr, BOOLEAN is_on_recyclebin);
PUBLIC void MMIWIDGET_DisplayWidgetHighlight(MMI_IDLE_GRID_DATA_T *idle_grid_ptr, BOOLEAN is_on_recyclebin);

/*****************************************************************************/
PUBLIC MMI_HANDLE_T MMIWIDGET_GetWinHandleByWidgetId(
                                          MMI_IDLE_GRID_DATA_T *idle_grid_ptr,
                                          uint32 widget_id
                                          );

/*****************************************************************************/
//  Description : get widget id by win handle
//  Global resource dependence :
//  Author: hongbo.lan
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMIWIDGET_GetWidgetIdByWinHandle(
                                    MMI_IDLE_GRID_DATA_T *idle_grid_ptr,
                                    MMI_HANDLE_T win_handle
                                    );

/*****************************************************************************/   
//  Description : get widget bg image and shortcut image.
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIWIDGET_GetWidgetImage(MMIWIDGET_ITEM_INFO_T *item_ptr, 
                                        MMI_IMAGE_ID_T *bg_img, MMI_IMAGE_ID_T *sc_img);

#ifdef   __cplusplus
    }
#endif
#endif
