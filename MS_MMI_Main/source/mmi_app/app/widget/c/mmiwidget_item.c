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


/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_app_widget_trc.h"
#include "mmiwidget.h"
#include "mmitheme_widget.h"
#include "mmiwidget_nv.h"
#include "mmiwidget_id.h"
#include "mmiwidget_image.h"
#include "mmiwidget_position.h"
#include "mmiwidget_text.h"
#include "mmiwidget_export.h"
#include "mmiwidget_internal.h"
#include "mmiwidget_item.h"
#include "mmiwidget_grid.h"
#include "mmiwidget_page.h"
#include "mmiwidget_virtualwin.h"
#include "mmiwidget_wallpaper.h"
#include "mmi_appmsg.h"
#include "mmidisplay_data.h"
#ifdef PDA_UI_SUPPORT_U_IDLE
#include "u_launcher_export.h"
#endif
//用于内存回收的互斥量和标志位
//LOCAL uint32*           sp_widget_cahce_flag    = PNULL; 
//LOCAL SCI_MUTEX_PTR     sp_widget_cahce_mutex   = PNULL;      //互斥信号量

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#ifdef MMI_IDLE_WIDGET_CACHE
LOCAL int32 ReleaseWidgetCache(MMIWIDGET_ITEM_T *p_item_info);
LOCAL uint32 ReleaseWidgetCacheInAllPageForCallback(uint32 id, void *idle_grid_ptr);
#endif

/*****************************************************************************/
//  Description : is valid widget
//  Global resource dependence :
//  Author: hongbo.lan
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIWIDGET_IsValidWidgetId(
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
//  Description : find widget item
//  Global resource dependence :
//  Author: hongbo.lan
//  Note: 
/*****************************************************************************/
PUBLIC MMIWIDGET_ITEM_T *MMIWIDGET_FindWidgetItem(
                                                  MMI_IDLE_GRID_DATA_T *idle_grid_ptr,
                                                  uint32 widget_id
                                                  )
{
    MMIWIDGET_ITEM_T *item_ptr = PNULL;
    
    if (PNULL != idle_grid_ptr->item_ptr && MMIWIDGET_IsValidWidgetId(widget_id))
    {          
       item_ptr = &(idle_grid_ptr->item_ptr[widget_id - 1]);          
    }
    return item_ptr;
}

/*****************************************************************************/
//  Description : redraw widget item
//  Global resource dependence : 
//  Author:hongbo.lan
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIWIDGET_RedrawWidgetItem(
                               MMIWIDGET_ITEM_T *item_ptr
                               )
{
    BOOLEAN result = FALSE;    
    result = MMK_SendMsg(item_ptr->item_info.win_id, MSG_FULL_PAINT, PNULL);
    
    return result;
}

/*****************************************************************************/
//  Description : widget子窗口的消息处理函数
//  Global resource dependence : 
//  Author:  hongbo.lan
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIWIDGET_RunChildWidgetProcEx(
                                      MMI_IDLE_GRID_DATA_T  *idle_grid_ptr,
                                      uint32            widget_id,
                                      MMI_MESSAGE_ID_E  msg_id,
                                      DPARAM            param,                                      
                                      BOOLEAN           is_handle_focus
                                      )
{
    MMIWIDGET_ITEM_T  *item_ptr = PNULL;
    MMI_RESULT_E result = MMI_RESULT_FALSE;    

    item_ptr = MMIWIDGET_FindWidgetItem(idle_grid_ptr, widget_id);
    if(item_ptr == 0)
    {
        return result;
    }
    //MSG_OPEN_WINDOW,MSG_CLOSE_WINDOW消息漏下去，其他的消息都是在widget item处于ACTIVE状态处理
    if (MSG_CLOSE_WINDOW == msg_id || MSG_OPEN_WINDOW == msg_id )
    {
        result = item_ptr->item_info.HandleWidgetWinMsg(item_ptr->item_info.win_id, msg_id, param);
    }
    else if(item_ptr->is_active)
    {
        if( (MSG_LOSE_FOCUS == msg_id || MSG_GET_FOCUS == msg_id) && is_handle_focus)
        {        
            result = MMK_SendMsg(item_ptr->item_info.win_id,  msg_id, param );
        }
        else
        {
            result = item_ptr->item_info.HandleWidgetWinMsg(item_ptr->item_info.win_id, msg_id, param);   
        }
    }
    
    return result;
}

PUBLIC MMI_RESULT_E MMIWIDGET_RunChildWidgetProc(
                                      MMI_IDLE_GRID_DATA_T  *idle_grid_ptr,
                                      uint32            widget_id,
                                      MMI_MESSAGE_ID_E  msg_id,
                                      DPARAM            param
                                      )
{
    return MMIWIDGET_RunChildWidgetProcEx(idle_grid_ptr, widget_id, msg_id, param, TRUE);
}

 //widget related
 /*****************************************************************************/
 //  Description : 创建widget子窗口
 //  Global resource dependence : 
 //  Author:hongbo.lan
 //  Note:
 /*****************************************************************************/
 PUBLIC BOOLEAN MMIWIDGET_CreateWidgetItem(
                                    MMI_IDLE_GRID_DATA_T *idle_grid_ptr,                                     
                                    MMI_WIN_ID_T win_id,
                                    const MMIWIDGET_ITEM_T* add_item_ptr,                                    
                                    PROCESSMSG_FUNC          func              //窗口的回调函数
                                    )
 {
     MMI_WINDOW_CREATE_T win_create = {0};
     MMI_HANDLE_T win_handle = 0;
     BOOLEAN result = FALSE;
     win_create.applet_handle = MMK_GetFirstAppletHandle();  //applet_handle;
     if (PNULL != add_item_ptr)
     {
         win_create.win_id = add_item_ptr->item_info.win_id;
     }
     win_create.func = func;
     win_create.parent_win_handle = win_id;
     win_create.win_priority = WIN_ONE_LEVEL;//win_priority;
     win_create.add_data_ptr = (void *)add_item_ptr;
     win_create.window_style = WS_DISABLE_COMMON_BG | WS_WIDGET_WIN;
     if( 0 != ( win_handle = MMK_CreateWindow( &win_create ) ) )
     {
          result = TRUE;
     }
     return result;
 }
 
 /*****************************************************************************/
 //  Description : active widget
 //  Global resource dependence :
 //  Author: hongbo.lan
 //  Note: 
 /*****************************************************************************/
 PUBLIC BOOLEAN MMIWIDGET_ActiveWidgetItem(
                                MMI_IDLE_GRID_DATA_T *idle_grid_ptr,
                                uint32 widget_id
                                )
 {
    GUI_RECT_T window_rect = {0};
    MMIWIDGET_ITEM_T *item_ptr = PNULL;
    uint16 bg_width = 0;
    uint16 bg_height = 0;
    MMI_IMAGE_ID_T  bg_img = 0;
    GUI_RECT_T  screen_rect = MMITHEME_GetFullScreenRect();

    SCI_TRACE_LOW("MMIQBTHEME_IDLEFUNCTION MMIWIDGET_ActiveWidgetItem widget_id = %d", widget_id);    
    item_ptr = MMIWIDGET_FindWidgetItem(idle_grid_ptr, widget_id);
    if (PNULL != item_ptr)
    {        
      item_ptr->is_active = TRUE;

      MMIWIDGET_GetWidgetImage(&item_ptr->item_info, &bg_img, PNULL);
      
      
      if(!MMIWIDGET_GetWinRect(idle_grid_ptr, widget_id, &window_rect))
      {
          window_rect.left = item_ptr->item_info.start_pos.x;
          window_rect.top = item_ptr->item_info.start_pos.y;
          GUIRES_GetImgWidthHeight(&bg_width, &bg_height, bg_img, item_ptr->item_info.win_id);
          window_rect.right = window_rect.left + bg_width - 1;
          window_rect.bottom = window_rect.top + bg_height - 1; 
      }
           
      MMIWIDGET_SetChildWinRect(item_ptr->item_info.win_id, window_rect, idle_grid_ptr->is_main_screen_landscape);
      MMIWIDGET_RunChildWidgetProc(idle_grid_ptr, item_ptr->item_info.widget_id, MSG_GRID_ADD_WIDGET, PNULL);
     if(GUI_IntersectRect(&window_rect, window_rect, screen_rect))
     {
        MMIWIDGET_RunChildWidgetProc(idle_grid_ptr, item_ptr->item_info.widget_id, MSG_GET_FOCUS, PNULL);        
     }
      return TRUE;
    }

    return FALSE;
 }
 
 /*****************************************************************************/
 //  Description : deactive widget
 //  Global resource dependence :
 //  Author: hongbo.lan
 //  Note: 
 /*****************************************************************************/
 PUBLIC BOOLEAN MMIWIDGET_DeactiveWidgetItem(
                                  MMI_IDLE_GRID_DATA_T *idle_grid_ptr,
                                  int32 widget_id
                                  )
 {
     MMIWIDGET_ITEM_T *item_ptr = PNULL;
     
     SCI_TRACE_LOW("MMIQBTHEME_IDLEFUNCTION MMIWIDGET_DeactiveWidgetItem widget_id = %d", widget_id);    
 
     if(widget_id < 0)
     {
         //MMIQBIDLE_DEBUG:"widget_id = %d"
         SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIQBTHEME_IDLEFUNCTION_984_112_2_18_3_9_0_270,(uint8*)"d",widget_id);
         return  FALSE;
     }
     item_ptr = MMIWIDGET_FindWidgetItem(idle_grid_ptr, widget_id);   
     if (PNULL != item_ptr)
     {
         MMK_SetChildWinLostFocus(idle_grid_ptr->win_id, FALSE);
         MMIWIDGET_RunChildWidgetProc(idle_grid_ptr, item_ptr->item_info.widget_id, MSG_LOSE_FOCUS, PNULL);
         MMIWIDGET_RunChildWidgetProc(idle_grid_ptr, item_ptr->item_info.widget_id, MSG_GRID_REMOVE_WIDGET, PNULL);    
         item_ptr->is_active = FALSE;
         return TRUE;
     }
     
     return FALSE;
 }
 
 /*****************************************************************************/
 //  Description : destroy item data
 //  Global resource dependence : 
 //  Author:hongbo.lan
 //  Note:
 /*****************************************************************************/
 PUBLIC BOOLEAN MMIWIDGET_DestroyWidgetItem(
                                 MMI_IDLE_GRID_DATA_T * idle_grid_ptr,  // the list control pointer
                                 MMIWIDGET_ITEM_T *item_ptr
                                 )
 {
     BOOLEAN result = FALSE;
 
     if (PNULL != item_ptr && MMIWIDGET_IsValidWidgetId(item_ptr->item_info.widget_id))
     {     
         //为了区别系统切换和widget删除，系统切换时不发送MSG_GRID_REMOVE_WIDGET消息
         //MMIWIDGET_RunChildWidgetProc(idle_grid_ptr, item_ptr->item_info.widget_id,  MSG_GRID_REMOVE_WIDGET, 0 );
         MMK_CloseWin(item_ptr->item_info.win_id);
         result = TRUE;
     }
     return result;
 }
 
 /*****************************************************************************/
 //  Description : destroy all item data
 //  Global resource dependence : 
 //  Author:hongbo.lan
 //  Note:
 /*****************************************************************************/
 PUBLIC BOOLEAN MMIWIDGET_DestroyAllWidgetItemData(
                                        MMI_IDLE_GRID_DATA_T*       idle_grid_ptr  // the list control pointer
                                        )
 {
     uint32 i = 0;
     BOOLEAN result = TRUE;
     
     for ( i = 0; i < MMIWIDGET_MAX_ITEM_NUM; i++)
     {
         MMIWIDGET_DestroyWidgetItem( idle_grid_ptr, MMIWIDGET_FindWidgetItem( idle_grid_ptr, i ) );
     }
     
     if (PNULL != idle_grid_ptr->item_ptr)
     {
         SCI_FREE(idle_grid_ptr->item_ptr);
         idle_grid_ptr->item_ptr = PNULL;
     }
     
     return result;
 }

 
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
                               )
 {
     GUI_BOTH_RECT_T both_rect = {0};
     
     if ((rect.left > rect.right)
         || (rect.top > rect.bottom))
     {
         //MMI_ASSERT_WARNING("MMIWIDGET_SetChildWinRect window_rect:(%d, %d, %d, %d) is error!", rect.left, rect.top, rect.right, rect.bottom);
         //SCI_TRACE_LOW:"MMIWIDGET_SetChildWinRect window_rect:(%d, %d, %d, %d) is error!"
         SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIQBTHEME_IDLEFUNCTION_1240_112_2_18_3_9_1_274,(uint8*)"dddd", rect.left, rect.top, rect.right, rect.bottom);
         return FALSE;        
     }
     if (is_main_screen_landscape)
     {        
         both_rect.h_rect = rect;
     }
     else
     {
         both_rect.v_rect = rect;
     }
     win_handle = MMK_ConvertIdToHandle(win_handle);
     MMK_SetBothRectNotifyCtrl(win_handle, &both_rect);
     MMK_SendMsg(win_handle, MSG_WIDGET_MODIFY_RECT, PNULL);
     
     return TRUE;
 }
 
 
 /*****************************************************************************/
 //  Description : get win handle by widget id
 //  Global resource dependence :
 //  Author: hongbo.lan
 //  Note: 
 /*****************************************************************************/
 PUBLIC MMI_HANDLE_T MMIWIDGET_GetWinHandleByWidgetId(
                                           MMI_IDLE_GRID_DATA_T *idle_grid_ptr,
                                           uint32 widget_id
                                           )
 {
     MMI_HANDLE_T win_handle = 0;
     
     win_handle = idle_grid_ptr->item_ptr[widget_id - 1].item_info.win_id;

     return win_handle;
 }
 
 /*****************************************************************************/
 //  Description : get widget id by win handle
 //  Global resource dependence :
 //  Author: hongbo.lan
 //  Note: 
 /*****************************************************************************/
 PUBLIC uint32 MMIWIDGET_GetWidgetIdByWinHandle(
                                     MMI_IDLE_GRID_DATA_T *idle_grid_ptr,
                                     MMI_HANDLE_T win_handle
                                     )
 {
     uint32 widget_id = MMIWIDGET_INVALID_ID;
     int32 i = 0;
     
     win_handle = MMK_ConvertIdToHandle(win_handle);
     for (i = 0; i < idle_grid_ptr->max_item_num; i++)
     {
         if (win_handle == MMK_ConvertIdToHandle(idle_grid_ptr->item_ptr[i].item_info.win_id))
         {
             widget_id = idle_grid_ptr->item_ptr[i].item_info.widget_id;
             break;
         }
     }
     return widget_id;
 }

 
 /*****************************************************************************/   
 //  Description : get widget bg image and shortcut image.
 //  Global resource dependence : 
 //  Author: hongbo.lan
 //  Note:
 /*****************************************************************************/
 PUBLIC BOOLEAN MMIWIDGET_GetWidgetImage(MMIWIDGET_ITEM_INFO_T *item_ptr, MMI_IMAGE_ID_T *bg_img, MMI_IMAGE_ID_T *sc_img)
 {     
#ifdef MMI_SAM_THEME
     if (MMISET_IDLE_STYLE_GRID == MMIAPISET_GetIdleStyle()
            && MMISET_IDLE_SUBSTYLE_SAM == MMIAPISET_GetIdleSubStyle())
     {    
         if (PNULL != item_ptr->GetWidgetImage)
         {
             item_ptr->GetWidgetImage(bg_img, sc_img);
             return TRUE;
         }
     }
#endif            
 
     if (PNULL != bg_img)
     {
         *bg_img = item_ptr->large_bg_img;
     }
 
     if (PNULL != sc_img)
     {
         *sc_img = item_ptr->small_icon_id;
     }
 
     return TRUE;
 }
 
 #ifdef MMI_IDLE_WIDGET_CACHE
 /*****************************************************************************/
 //  Description : updatge widget contents to cache.
 //  Global resource dependence : 
 //  Author:hongbo.lan
 //  Note: 
 /*****************************************************************************/
 PUBLIC int32 MMIWIDGET_UpdateWidgetCache(MMIWIDGET_ITEM_T *p_item_info)  //write to cache
 {    
     GUI_RECT_T  rect = {0};    
     //SCI_GetMutex( sp_widget_cahce_mutex, SCI_WAIT_FOREVER);
     if(1 == MMIWIDGET_IsWidgetCacheValid(p_item_info))
     {
         MMK_GetWinRect(p_item_info->item_info.win_id, &rect);  
         UILAYER_SetLayerPosition(&p_item_info->lcd_dev_cache , rect.left, rect.top);
         MMK_SetWinLcdDevInfoNotifyCtrl(p_item_info->item_info.win_id, &p_item_info->lcd_dev_cache);        
         MMIWIDGET_RedrawWidgetItem(p_item_info);    
     }    
     
    // SCI_PutMutex( sp_widget_cahce_mutex );
     return 0;
 }
 
 //read from cache
 PUBLIC int32 MMIWIDGET_DrawWidget(MMIWIDGET_ITEM_T *p_item_info, GUI_LCD_DEV_INFO *p_lcd_dev, GUI_RECT_T *p_rect)
 {     
     int32 result = 0;
     //SCI_GetMutex( sp_widget_cahce_mutex, SCI_WAIT_FOREVER);
     if(0 == MMIWIDGET_IsWidgetCacheValid(p_item_info))
     {
         MMIWIDGET_CreateWidgetCache(p_item_info);  
         MMIWIDGET_UpdateWidgetCache(p_item_info);
     }
     
     if(1 == MMIWIDGET_IsWidgetCacheValid(p_item_info))
     {        
         if( UILAYER_GetLayerColorType( p_lcd_dev)
                  != UILAYER_GetLayerColorType(&p_item_info->lcd_dev_cache))
         {
             if(DATA_TYPE_PMARGB888 != UILAYER_GetLayerColorType( p_lcd_dev))
             {
                 UILAYER_SetPreMultiplyAlpha(p_lcd_dev, TRUE , TRUE );                
             }
             if(DATA_TYPE_PMARGB888 != UILAYER_GetLayerColorType( &p_item_info->lcd_dev_cache))
             {
                 UILAYER_SetPreMultiplyAlpha(&p_item_info->lcd_dev_cache, TRUE , TRUE );                
             }
         }         
         
         UILAYER_SetLayerPosition(&p_item_info->lcd_dev_cache , p_rect->left, p_rect->top);
         UILAYER_BltLayerToLayer(p_lcd_dev, &p_item_info->lcd_dev_cache, p_rect, FALSE);
     }
     else
     {    
         MMK_SetWinLcdDevInfoNotifyCtrl(p_item_info->item_info.win_id, p_lcd_dev);        
         MMIWIDGET_RedrawWidgetItem(p_item_info);    
         result = -1;
     }    
    //SCI_PutMutex( sp_widget_cahce_mutex );
    return result;
 }
 
 int32 ReleaseWidgetCache(MMIWIDGET_ITEM_T *p_item_info)
 {
     //if(0 !=  SCI_GetMutex( sp_widget_cahce_mutex, SCI_NO_WAIT))
     //{
     //    SCI_TRACE_LOW("MMIGRID_IDLE ReleaseWidgetCache GET MUTEX error");
     //    return -1;
     //}
     UILAYER_RELEASELAYER(&p_item_info->lcd_dev_cache);    /*lint !e506 !e774*/     
     //SCI_PutMutex( sp_widget_cahce_mutex);
     return 0;
 }
 
 uint32 ReleaseWidgetCacheInAllPageForCallback(uint32 id, void *idle_grid_ptr)
 {        
     static uint32 num = 0;
     MMI_IDLE_GRID_DATA_T *temp_idle_grid_ptr = 0;
 
     if(0 == idle_grid_ptr)
     {
         return (uint32)(-1);
     }
     temp_idle_grid_ptr = (MMI_IDLE_GRID_DATA_T*)idle_grid_ptr;
     num++;    
 //    if(0 !=  SCI_GetMutex( sp_widget_cahce_mutex, SCI_NO_WAIT))
 //    {
 //        SCI_TRACE_LOW("MMIGRID_IDLE ReleaseWidgetCache GET MUTEX error");
 //        return -1;
 //    }
 //    SCI_PutMutex( sp_widget_cahce_mutex);
     SCI_TRACE_LOW("MMIGRID ReleaseWidgetCacheInAllPageForCallback memory recycle num = %d", num);
     MMIWIDGET_ReleaseWidgetCacheInOnePage(temp_idle_grid_ptr, temp_idle_grid_ptr->cur_page_index - 1);
     MMIWIDGET_ReleaseWidgetCacheInOnePage(temp_idle_grid_ptr, temp_idle_grid_ptr->cur_page_index);
     MMIWIDGET_ReleaseWidgetCacheInOnePage(temp_idle_grid_ptr, temp_idle_grid_ptr->cur_page_index + 1);
     return num;
 }
 
 PUBLIC int32 MMIWIDGET_ReleaseWidgetCacheInAllPage(MMI_IDLE_GRID_DATA_T *idle_grid_ptr)
 {        
     MMIWIDGET_ReleaseWidgetCacheInOnePage(idle_grid_ptr, (idle_grid_ptr->cur_page_index - 1 
                                                                    + idle_grid_ptr->page_num)%idle_grid_ptr->page_num);
     MMIWIDGET_ReleaseWidgetCacheInOnePage(idle_grid_ptr, idle_grid_ptr->cur_page_index);
     MMIWIDGET_ReleaseWidgetCacheInOnePage(idle_grid_ptr, (idle_grid_ptr->cur_page_index + 1 
                                                                    + idle_grid_ptr->page_num)%idle_grid_ptr->page_num);
     return 0;
 }
 
 /*****************************************************************************/
 //  Description : release one page widget cache
 //  Global resource dependence : 
 //  Author:ying.xu
 //  Note: 
 /*****************************************************************************/
 PUBLIC int32 MMIWIDGET_ReleaseWidgetCacheInOnePage(MMI_IDLE_GRID_DATA_T *idle_grid_ptr, int32 page_idx)
 {
     int32           j = 0;    
     int32           page_grid_num = 0;
     MMIWIDGET_ITEM_T *item_ptr = 0;
     int32           widget_id = 0;    
     uint32         page_layer_size = MMIWIDGET_GetPageLayerSize(idle_grid_ptr);
     
     if(page_layer_size >= 3)
     {
         return -1;
     }
     
     if(0 > page_idx || page_idx >= idle_grid_ptr->page_num)
     {
          return -2;
     }
     
     page_grid_num = idle_grid_ptr->grid_row * idle_grid_ptr->grid_col;
     for (j = 0; j < page_grid_num; j++)
     {   
         if (IS_NODE_WIDGET(idle_grid_ptr->grid_data_tab[page_idx][j]) && !IS_NODE_AUX(idle_grid_ptr->grid_data_tab[page_idx][j]))
         {
             widget_id  = GET_NODE_ID(idle_grid_ptr->grid_data_tab[page_idx][j]);

             item_ptr = &idle_grid_ptr->item_ptr[widget_id - 1];
             ReleaseWidgetCache(item_ptr);
         }
     }
     return 0;
 }
 
 /*****************************************************************************/
 //  Description : release one page widget cache
 //  Global resource dependence : 
 //  Author:
 //  Note: 
 /*****************************************************************************/
 PUBLIC int32 MMIWIDGET_ReleaseAllWidgetsCache(MMI_IDLE_GRID_DATA_T *idle_grid_ptr)
 {
     int32           j = 0;    
     MMIWIDGET_ITEM_T *item_ptr = 0;
     uint32         page_layer_size = MMIWIDGET_GetPageLayerSize(idle_grid_ptr);
     
     if(page_layer_size >= 3)
     {
         return -1;
     }

     for (j = 0; j < MMIWIDGET_WIDGET_MAX; j++)
     {
         item_ptr = idle_grid_ptr->item_ptr + j;

         if(UILAYER_HANDLE_MULTI == UILAYER_GetHandleType(&item_ptr->lcd_dev_cache))
         {
             ReleaseWidgetCache(item_ptr);
         }
     }
     
     return 0;
 }

 
PUBLIC int32 MMIWIDGET_InitWidgetCacheControlData(MMI_IDLE_GRID_DATA_T *idle_grid_ptr)
 {
 /*
     if ( PNULL == sp_widget_cahce_mutex )
     {
         sp_widget_cahce_mutex = SCI_CreateMutex("mmigrid_idle widget cache", SCI_INHERIT);
     }
    
     //最后注册，避免申请大内存时被垃圾回收的回调
     if ( PNULL == sp_widget_cahce_flag )
     {
         BLKMEM_CONFIG_T config = {0};
 
         config.func_callback = ReleaseWidgetCacheInAllPageForCallback;
         config.param = idle_grid_ptr;
 
         BLKMEM_Register( (uint32)ReleaseWidgetCacheInAllPageForCallback, &config );
 
         sp_widget_cahce_flag = BLKMEM_Malloc( (uint32)ReleaseWidgetCacheInAllPageForCallback, sizeof(uint32));
     }
 
      */
     return 0;
 }
 
 PUBLIC int32 MMIWIDGET_ClearWidgetCacheControlData(MMI_IDLE_GRID_DATA_T *idle_grid_ptr)
 {          
 /* 
     BLKMEM_Free((uint32)ReleaseWidgetCacheInAllPageForCallback, sp_widget_cahce_flag);
     sp_widget_cahce_flag = 0;
     BLKMEM_UnRegister( (uint32)ReleaseWidgetCacheInAllPageForCallback);

     SCI_DeleteMutex(sp_widget_cahce_mutex);
     sp_widget_cahce_mutex = 0;  
     */
     return 0;
 }
 
 /*****************************************************************************/
 //  Description : create widget cache.
 //  Global resource dependence : 
 //  Author:hongbo.lan
 //  Note: 
 /*****************************************************************************/
 PUBLIC int32 MMIWIDGET_CreateWidgetCache(MMIWIDGET_ITEM_T *p_item_info)
 {
     GUI_RECT_T rect = {0};    
     uint16 width = 0;
     uint16 height = 0;
     UILAYER_CREATE_T create_info = {0};
     MMI_WIN_ID_T    win_id = 0;
 
     if(UILAYER_HANDLE_MULTI != UILAYER_GetHandleType(&p_item_info->lcd_dev_cache))
     {
         MMK_GetWinRect( p_item_info->item_info.win_id, &rect);
             
         width = (rect.right+1-rect.left);        
         height = (rect.bottom+1-rect.top);
         create_info.lcd_id = LCD_ID_0;
         win_id = MMK_GetParentWinHandle(p_item_info->item_info.win_id );
         create_info.owner_handle = win_id;
         create_info.offset_x = rect.left;
         create_info.offset_y = rect.top;
         create_info.width = width;
         create_info.height = height;
         create_info.is_bg_layer = FALSE;
         create_info.is_static_layer = TRUE; 
         create_info.format = UILAYER_MEM_FORMAT_IMMUTABLE;
         
         if(UILAYER_RESULT_SUCCESS == UILAYER_CreateLayer(&create_info,&p_item_info->lcd_dev_cache))
         {
             UILAYER_Clear(&p_item_info->lcd_dev_cache);
             return 0;
         }
         else
         {
             return -1;
         }
     }
     return -2;
 }
 
 /*****************************************************************************/
 //  Description : is widget cache valid or not?
 //  Global resource dependence : 
 //  Author:hongbo.lan
 //  Note: 
 /*****************************************************************************/
 PUBLIC int32 MMIWIDGET_IsWidgetCacheValid(MMIWIDGET_ITEM_T *p_item_info)
 { 
     if(UILAYER_HANDLE_MULTI == UILAYER_GetHandleType(&p_item_info->lcd_dev_cache))
     {
         return 1;
     }
     else
     {
         return 0;
     }    
 }
#endif


/*****************************************************************************/
//  Description : reset all widget layer
//  Global resource dependence : 
//  Author: hongbo.lan 
//  Note:
/*****************************************************************************/
PUBLIC void ResetAllGridLayer(
                           MMI_IDLE_GRID_DATA_T *idle_grid_ptr
                           )
{
    int32 i = 0;
    
    if (PNULL != idle_grid_ptr->item_ptr)
    {
        for (i = 0; i < idle_grid_ptr->max_item_num; i++)
        {
            if(idle_grid_ptr->item_ptr[i].is_active)
            {
                MMIWIDGET_ResetOneWidgetLayer(idle_grid_ptr, idle_grid_ptr->item_ptr[i].item_info.widget_id);
            }
        }
    }    
}

/*****************************************************************************/
//  Description : 设置某widget应用到widget_layer，page_layer或主层上
//  Global resource dependence : 
//  Author:hongbo.lan
//  Note: 
/*****************************************************************************/
PUBLIC GUI_LCD_DEV_INFO * MMIWIDGET_ResetOneWidgetLayer(
                                             MMI_IDLE_GRID_DATA_T *idle_grid_ptr,
                                             uint32 widget_id
                                             )
{
    GUI_LCD_DEV_INFO old_lcd_dev_info = {0};
    MMI_HANDLE_T win_handle = 0;
    GUI_LCD_DEV_INFO *dev_info_ptr = PNULL;    
 
    
    if (MMIWIDGET_INVALID_ID != widget_id)
    {
        win_handle = MMIWIDGET_GetWinHandleByWidgetId(idle_grid_ptr, widget_id); /*lint !e605*/
        
        if (   (MMIGRID_TP_CLICK_LONG_WIDGET == idle_grid_ptr->tp_move_info.click_type) 
            && (idle_grid_ptr->tp_move_info.click_id == widget_id)
            && UILAYER_IsLayerActive(&idle_grid_ptr->move_layer))
        {
            dev_info_ptr = &idle_grid_ptr->move_layer;
        }
        else if(UILAYER_IsLayerActive(&idle_grid_ptr->page_layer))
        {
            dev_info_ptr = &idle_grid_ptr->page_layer;
        }
        else
        {
            dev_info_ptr = MMITHEME_GetDefaultLcdDev(); /*lint !e605*/
        }
        
        MMK_GetWinLcdDevInfo(win_handle, &old_lcd_dev_info);
        
        if (dev_info_ptr->block_id != old_lcd_dev_info.block_id)
        {
            MMK_SetWinLcdDevInfoNotifyCtrl(
                win_handle,
                dev_info_ptr
                );
        }        
    }
    return dev_info_ptr;
}

/*****************************************************************************/
//  Description : 重新恢复到主层上（跟SetHitWidgetTopmost对应）
//  Global resource dependence : 
//  Author:hongbo.lan
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWIDGET_ResetHitWidgetLayer(
                               MMI_IDLE_GRID_DATA_T *idle_grid_ptr
                               )
{
    uint32 click_id = 0;

    if (MMIGRID_TP_CLICK_INVALID != idle_grid_ptr->tp_move_info.click_type)
    {
        click_id = idle_grid_ptr->tp_move_info.click_id;
        idle_grid_ptr->tp_move_info.click_id = MMIWIDGET_INVALID_ID;
        MMIWIDGET_ResetOneWidgetLayer(idle_grid_ptr, click_id);
        idle_grid_ptr->tp_move_info.click_id = click_id;
    }
  
}

/*****************************************************************************/
//  Description : Display item on recycle bin with red color.
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note:
/*****************************************************************************/
PUBLIC void MMIWIDGET_DisplayWidgetHighlight(MMI_IDLE_GRID_DATA_T *idle_grid_ptr, BOOLEAN is_on_recyclebin)
{
    MMIWIDGET_ITEM_T  item_info  = {0};
    GUI_RECT_T        rect       = {0}; 

    if(!UILAYER_IsLayerActive(&idle_grid_ptr->move_layer ))
    {
        return;
    }
    if (is_on_recyclebin)
    {
        FillLayerWithMaskColor(&idle_grid_ptr->move_layer, PNULL, RGB5652ARGB8888(MMI_RED_COLOR, 255)); 
        return;
    }
    
    item_info = idle_grid_ptr->item_ptr[idle_grid_ptr->tp_move_info.click_id - 1];

    rect = UILAYER_GetLayerRect(&idle_grid_ptr->move_layer);    
    MMIWIDGET_SetChildWinRect(item_info.item_info.win_id, rect,  idle_grid_ptr->is_main_screen_landscape);

    UILAYER_Clear(&idle_grid_ptr->move_layer);
    
    MMIWIDGET_RedrawWidgetItem(&item_info); 

    if(idle_grid_ptr->is_editable)
    {
        GUIRES_DisplayImg(PNULL, 
            &rect, 
            PNULL, 
            idle_grid_ptr->win_id, 
            idle_grid_ptr->delect_icon_id, 
            &idle_grid_ptr->move_layer);
    }
    UILAYER_WeakLayerAlpha(&idle_grid_ptr->move_layer, MMIMENU_WEAKLAYERALPHA);
}


/*****************************************************************************/
//  Description : ShortCutTryToMove
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWIDGET_DispShortCutHightlight(MMI_IDLE_GRID_DATA_T *idle_grid_ptr, BOOLEAN is_on_recyclebin)
{
    GUI_RECT_T display_rc = {0};
    CTRLMAINMENU_ITEM_T menuitem_info = {0};
    MMI_IDLE_SC_ICON_T shortcut_item = {0};


    if(!UILAYER_IsLayerActive(&idle_grid_ptr->move_layer ))
    {
        return;
    }
    if (is_on_recyclebin)
    {
        FillLayerWithMaskColor(&idle_grid_ptr->move_layer, PNULL, RGB5652ARGB8888(MMI_RED_COLOR, 255));/*lint !e648 !e778*/
        return;
    }
    UILAYER_Clear(&idle_grid_ptr->move_layer);
    
    shortcut_item = idle_grid_ptr->icon_ptr[idle_grid_ptr->tp_move_info.click_id];
    MMIMAINMENU_GetItemData(
        shortcut_item.icon_info.menu_id,
        shortcut_item.icon_info.group_id,
        &menuitem_info);

    display_rc = UILAYER_GetLayerRect(&idle_grid_ptr->move_layer);    
    
    MMIWIDGET_ShortCutDrawMenuIcon(idle_grid_ptr->win_id, &menuitem_info, 
                                    display_rc, &idle_grid_ptr->move_layer,
                                    idle_grid_ptr->delect_icon_id, idle_grid_ptr->is_editable);
                                    
    UILAYER_WeakLayerAlpha(&idle_grid_ptr->move_layer, MMIMENU_WEAKLAYERALPHA);
}

PUBLIC int32 MMIWIDGET_ShortCutDrawMenuIcon(MMI_WIN_ID_T win_id,
                                    CTRLMAINMENU_ITEM_T* menu_item_ptr, 
                                    GUI_RECT_T display_rc,
                                    GUI_LCD_DEV_INFO* lcd_dev_info,
                                    MMI_IMAGE_ID_T delete_image_id,
                                    BOOLEAN is_editing_mode)
{
    GUI_LCD_DEV_INFO item_layer = {0};
    
    if(menu_item_ptr == PNULL)
    {
        return -1;
    }    
    
    display_rc = MMIWIDGET_GetIconRectFromGridRect(display_rc);
    
    item_layer = MMITHEME_DrawMainmenuItem( win_id, menu_item_ptr, PNULL );
    UILAYER_SetLayerPosition( &item_layer, display_rc.left, display_rc.top );
    UILAYER_BltLayerToLayer( lcd_dev_info, &item_layer, &display_rc, 
        (UILAYER_HANDLE_MAIN == UILAYER_GetHandleType( lcd_dev_info )) ? TRUE : FALSE );

    if(is_editing_mode)//draw delete icon
    {
        GUIRES_DisplayImg(PNULL, 
            &display_rc, 
            PNULL, 
            win_id, 
            delete_image_id, 
            lcd_dev_info);
    } 
    return 0;    
}


