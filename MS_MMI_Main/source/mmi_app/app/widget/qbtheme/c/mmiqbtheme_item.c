/*****************************************************************************
** File Name:      mmiqbtheme_item.c                                         *
** Author:                                                                   *
** Date:           08/10/2011                                                *
** Copyright:      2011 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe dockbar item                *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 10/23/2011     ying.xu              Creat
******************************************************************************/


/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "mmi_app_widget_trc.h"
#include "mmk_kbd.h"
#include "guicommon.h"
#include "mmidisplay_data.h"
#include "mmiwidget_id.h"
#include "mmiwidget_image.h"
#include "mmi_theme.h"
#include "mmiwidget.h"
#include "mmi_nv.h"
#include "mmiwidget_image.h"
#include "guires.h"
#include "mmi_modu_main.h"
//#include "mmiset.h"
#include "mmiset_export.h"
#include "mmiset_display.h"
//#include "mmi_wallpaper_export.h"
//#include "guictrl_api.h"

#include "mmk_type.h"
#include "mmk_app.h"
#include "mmk_kbd.h"
#include "guilcd.h"
#include "window_parse.h"
#include "guimenu.h"
#include "mmitheme_menu.h"
#include "mmitheme_mainmenu.h"
#include "mmiset_wintab.h"
#include "Mmiwidget_text.h"
#include "mmiwidget_export.h"
#include "mmi_mainmenu_export.h"
#ifdef PDA_UI_DROPDOWN_WIN
#include "mmidropdownwin_export.h"
#endif
#ifdef SEARCH_SUPPORT 
#include "mmisearch_export.h"
#endif
#include "mmi_menutable.h"
#include "mmiset_text.h"
//#include "mmiset.h"
#include "mmiwidget_internal.h"
#include "mmiwidget_item.h"
#include "mmiwidget_grid.h"
#include "mmiwidget_page.h"
#include "mmiwidget_virtualwin.h"

#include "mmiqbtheme_text.h"

#include "mmiqbtheme_config.h"
#include "mmiqbtheme_item.h"
#include "mmiqbtheme_grid.h"
#include "mmiqbtheme_page.h"
#include "mmiqbtheme_virtualwin.h"
#include "mmiqbtheme_dockbar.h"
#include "mmiqbtheme_menuwin.h"
#include "mmiqbtheme_idlewin.h"
#include "mmiqbtheme_idleexport.h"

//状态转换矩阵
//lost focus: QUIT moving/sliding state
//cancal_key: all_state is setted back to normal
//all state count in 4, so row = 4
//input msg  tp_down /tp_up/ tp_move/tp_long  count in 4,so col = 4
//the only deffient is array[0][3] with page_state_tranfer_array
const  MMIQBTHEME_FSM_STATE_E item_state_tranfer_array[4][4]= {
    MMIQBTHEME_NORMAL, MMIQBTHEME_NORMAL, MMIQBTHEME_NORMAL, MMIQBTHEME_ITEM_MOVING,// MMIQBTHEME_NORMAL    
    MMIQBTHEME_EDITING, MMIQBTHEME_EDITING, MMIQBTHEME_ITEM_MOVING, MMIQBTHEME_ITEM_MOVING, //MMIQBTHEME_EDITING
    MMIQBTHEME_PAGE_SLIDING, MMIQBTHEME_EDITING, MMIQBTHEME_PAGE_SLIDING, MMIQBTHEME_PAGE_SLIDING,//MMIQBTHEME_PAGE_SLIDING
    MMIQBTHEME_ITEM_MOVING, MMIQBTHEME_EDITING, MMIQBTHEME_ITEM_MOVING, MMIQBTHEME_ITEM_MOVING,//MMIQBTHEME_ITEM_MOVING
};

//dockbar 自身功能item的图标文字操作定义数组
const MMIQBTHEME_ITEM_FUNCTION_DATA function_data[] = {
#if defined MMI_WALLPAPER_SUPPORT
    {TXT_COMMON_SET_WALLPAPER, IMAGE_THEME_SET_WALLPAPER, MMIAPISET_SetWallpaper},
#endif
    {TXT_WIDGET_ADD_SHORT,IMAGE_THEME_ADD_SHORTCUT,MMIQBIDLE_CreateAddShortCutWin},
    {TXT_WIDGET_WIDGET,IMAGE_THEME_ADD_WIDGET,MMIQBIDLE_CreateAddWidgetWin},
    {TXT_COMMON_MAINMENU,IMAGE_MAINMENU_ICON_MAINMENU,MMIAPIMENU_CreatMainMenu},
    {0,IMAGE_QBTHEME_DOCKBAR_ADD_ICON,MMIQBTHEME_AddWidgetPagePreviewItemEx} /*lint !e64 */
};

LOCAL int32 DrawItem(MMIQBTHEME_ITEM_T *p_item
                                      );

LOCAL MMI_RESULT_E ItemHandleTPMSG(MMIQBTHEME_ITEM_T *p_item,
                                   MMI_WIN_ID_T      win_id,
                                   MMI_MESSAGE_ID_E  msg_id,                                                
                                   DPARAM            param
                                   );

//执行每个item对应的功能
LOCAL MMI_RESULT_E ItemExecuteFuncion(MMIQBTHEME_ITEM_T *p_item
                                      );

//fsm 状态/ tp消息——转换函数
LOCAL int32 ItemHandleFSM(MMIQBTHEME_ITEM_T *p_item,
                          MMI_MESSAGE_ID_E  msg_id,                             
                          MMI_RESULT_E      result
                          );

//get delete icon rect
LOCAL int32 ItemGetDeleteIconRect(MMIQBTHEME_ITEM_T *p_item,
                                  GUI_RECT_T *rect
                                  );
//handlement after click up                              
LOCAL MMI_RESULT_E ItemHandleClick(MMIQBTHEME_ITEM_T *p_item, GUI_POINT_T point);

//handlment after tp up                                  
LOCAL int32 ResetItemRectAfterTPUp(MMIQBTHEME_ITEM_T *p_item, GUI_POINT_T point);

//设置成员变量的值，申请内存
PUBLIC int32 MMIQBTHEME_ItemCreate(MMIQBTHEME_ITEM_T   **p_item, MMIQBTHEME_ITEM_TPYE_E type)
{
    MMIQBTHEME_ITEM_T   *p_temp_item = 0;
    
    if(p_item == PNULL)
    {
        return -1;
    }
    
    p_temp_item = (MMIQBTHEME_ITEM_T *)SCI_ALLOC_APP(sizeof(MMIQBTHEME_ITEM_T));
    SCI_MEMSET(p_temp_item, 0, sizeof(MMIQBTHEME_ITEM_T));
    p_temp_item->type = type;               
    p_temp_item->fsm_state = MMIQBTHEME_NORMAL;
    *p_item = p_temp_item;
    return 0;    
}

//destroy an virtual win objcet
PUBLIC int32 MMIQBTHEME_ItemDestroy(MMIQBTHEME_ITEM_T *p_item)
{
    if(MMIQBTHEME_ITEM_WIDGET_PAGE_PREVIEW_ICON == p_item->type )
    {
        MMIQBTHEME_DestroyDisplay(&p_item->display);
    }
    SCI_FREE(p_item);
    return 0;    
}

//窗口处理函数
PUBLIC MMI_RESULT_E MMIQBTHEME_ItemHandleMsg(MMIQBTHEME_ITEM_T *p_item,
                                             MMI_WIN_ID_T      win_id,
                                             MMI_MESSAGE_ID_E  msg_id,
                                             DPARAM            param
                                             )
{
    MMI_RESULT_E     result = MMI_RESULT_TRUE;
    
    switch (msg_id)
    {        
    case MSG_OPEN_WINDOW: 
        {                      
        }
        break;
        
    case MSG_FULL_PAINT:
        {   
            DrawItem(p_item);
        }
        break;
        
    case MSG_APP_RED:        
    case MSG_APP_CANCEL:
        //return to normal state
        if(p_item->fsm_state != MMIQBTHEME_NORMAL)
        {
            p_item->fsm_state = MMIQBTHEME_NORMAL;  
            if(MMIQBTHEME_ITEM_WIDGET_PAGE_PREVIEW_ICON == p_item->type)
            {
                MMIQBTHEME_RemoveDisplay( &p_item->display);      
            }
            else if(MMIQBTHEME_ITEM_MENU == p_item->type)
            {
                MMIQBTHEME_DestroyDisplay( &p_item->display);      
            }
            //DrawItem(p_item);  
        }       
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_LONG:
    case MSG_TP_PRESS_DOWN:
    case MSG_TP_PRESS_MOVE:              
    case MSG_TP_PRESS_UP:
        //check area
        if(p_item->fsm_state == MMIQBTHEME_ITEM_MOVING 
                && p_item->type == MMIQBTHEME_ITEM_WIDGET_PAGE_PREVIEW_ICON)
        {            
            GUI_POINT_T point = {0};
            point.x = MMK_GET_TP_X(param);
            point.y = MMK_GET_TP_Y(param);        
            //MMIQBIDLE_DEBUG:"pitem state = moving , type = preview index = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIQBTHEME_ITEM_197_112_2_18_3_9_16_310,(uint8*)"d", p_item->item_index);
            //MMIQBIDLE_DEBUG:"tp point.x =%d , point.y =%d, pitem display area %d,%d,%d,%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIQBTHEME_ITEM_204_112_2_18_3_9_16_311,(uint8*)"dddddd",point.x, point.y,p_item->display.display_rect.left, p_item->display.display_rect.top,                     p_item->display.display_rect.right, p_item->display.display_rect.bottom);
        }
        if(p_item->fsm_state != MMIQBTHEME_ITEM_MOVING )
        {        
            GUI_POINT_T point = {0};
            point.x = MMK_GET_TP_X(param);
            point.y = MMK_GET_TP_Y(param);        
            if(!GUI_PointIsInRect( point, p_item->display.display_rect))
            {                        
                result = MMI_RESULT_FALSE;
                break;
            }
        }
        result = ItemHandleTPMSG( p_item,  win_id, msg_id, param );
        break;       
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        
        
    case MSG_LOSE_FOCUS:   
        if(p_item->fsm_state == MMIQBTHEME_ITEM_MOVING )
        {            
            if(MMIQBTHEME_ITEM_WIDGET_PAGE_PREVIEW_ICON == p_item->type)
            {
                MMIQBTHEME_RemoveDisplay( &p_item->display);      
            }
            else if(MMIQBTHEME_ITEM_MENU == p_item->type)
            {
                MMIQBTHEME_DestroyDisplay( &p_item->display);      
            }
            
            MMIQBTHEME_ItemSetRect(p_item, p_item->display.layout_rect);   
            
            if(p_item->type == MMIQBTHEME_ITEM_WIDGET_PAGE_PREVIEW_ICON)
            { 
                MMIQBTHEME_SetWidgetPreviewPageFocusedItem(p_item->item_index); 
                MMIQBIDLE_SetCurPageIndex(p_item->item_index);
            }
            p_item->fsm_state = MMIQBTHEME_EDITING ;            
        }
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return(result);     
}

//set状态机状态
PUBLIC int32  MMIQBTHEME_ItemSetFSMState(MMIQBTHEME_ITEM_T* p_item,
                                         MMIQBTHEME_FSM_STATE_E      fsm_state
                                         )
{    
    if(p_item->fsm_state == MMIQBTHEME_ITEM_MOVING)
    {
        return -1;
    }    
    
    if(p_item->type == MMIQBTHEME_ITEM_WIDGET_PAGE_PREVIEW_ICON)
    {
        if(p_item->fsm_state != MMIQBTHEME_NORMAL && fsm_state == MMIQBTHEME_NORMAL)
        {       
            MMIQBTHEME_DestroyDisplay(&p_item->display);       
        }  
        else if(p_item->fsm_state == MMIQBTHEME_NORMAL && fsm_state != MMIQBTHEME_NORMAL)
        {
            MMIQBTHEME_CreateDisplayEx(&p_item->display);       
        }        
    }

    p_item->fsm_state = fsm_state;
    
    return 0;
}                                        

//get状态机状态
PUBLIC MMIQBTHEME_FSM_STATE_E  MMIQBTHEME_ItemGetFSMState(MMIQBTHEME_ITEM_T* p_item )
{
    return  p_item->fsm_state ;    
}                              

//set display object
PUBLIC int32 MMIQBTHEME_ItemSetDisplayObject(MMIQBTHEME_ITEM_T   *p_item , 
                                             MMIQBTHEME_DISPLAY_T      *display
                                             )
                                             
{
    p_item->display = *display;    
    return 0;
}   

//delete one item，in current page
PUBLIC int32 MMIQBTHEME_ItemDelete(MMIQBTHEME_ITEM_T *p_item)
{
    return MMIQBTHEME_OpenDeletingItemQueryWin(p_item);
}

//draw item
PUBLIC int32 MMIQBTHEME_ItemDraw(MMIQBTHEME_ITEM_T *p_item)
{
    return DrawItem(p_item);
}

LOCAL int32 DrawItem(MMIQBTHEME_ITEM_T *p_item
                                      )
{
    MMI_RESULT_E   result = MMI_RESULT_TRUE; 
    MMI_STRING_T   item_string  = {0};
    GUISTR_STYLE_T str_style = {0};
    GUISTR_STATE_T str_state = GUISTR_STATE_ALIGN | GUISTR_STATE_SINGLE_LINE | GUISTR_STATE_ELLIPSIS | GUISTR_STATE_EFFECT;
    GUI_RECT_T     str_rect = {0};
    uint16         img_width = 0;
    uint16         img_height = 0;
    uint16         font_height = 0;   
    GUI_RECT_T     icon_rect = {0};
    GUI_RECT_T     layer_rect = {0};
    GUI_LCD_DEV_INFO* lcd_dev_info_ptr = {0};

    if (UILAYER_IsLayerActive(&p_item->display.lcd_dev_info))
    {        
        lcd_dev_info_ptr = &p_item->display.lcd_dev_info;
    }   
    else
    {
         lcd_dev_info_ptr =  MMITHEME_GetDefaultLcdDev();     
    }

    layer_rect = UILAYER_GetLayerRect(lcd_dev_info_ptr);
    if(!GUI_IntersectRect(&layer_rect, layer_rect , p_item->display.display_rect))
    {
        return -1;
    }
    switch (p_item->type)
    {    
    case MMIQBTHEME_ITEM_FUNC:     
        icon_rect = p_item->display.display_rect;
      
        GUIRES_GetImgWidthHeight(&img_width, &img_height, function_data[p_item->id.qbtheme_func_id].icon , p_item->display.win_id);
 
        MMI_GetLabelTextByLang(function_data[p_item->id.qbtheme_func_id].text, &item_string);
        if(item_string.wstr_len != 0)
        {
            font_height = MMIQBTHEME_DOCKBAR_MENU_ITEM_TEXT_HEIGHT;
            //和主菜单的显示保持一致
            icon_rect.bottom = icon_rect.top + MMIQBTHEME_DOCKBAR_MENU_ITEM_ICON_SIZE - 1;
            icon_rect = GUI_GetCenterRect(icon_rect, img_width, img_height);
            //draw icon
            GUIRES_DisplayImg(PNULL, 
                &icon_rect, 
                PNULL, 
                p_item->display.win_id,
                function_data[p_item->id.qbtheme_func_id].icon , 
                lcd_dev_info_ptr); 
            
            //draw text
            str_style.align = ALIGN_HVMIDDLE;
            str_style.font = MMITHEME_MAINMENU_TEXT_FONT;
            str_style.font_color = ~MMITHEME_GetCurThemeFontColor(MMI_THEME_MAINMENU_FONT);        
            str_style.effect = FONT_EFFECT_REVERSE_EDGE;
            str_style.edge_alpha = 0x40;       
            str_rect.left   = p_item->display.display_rect.left;
            str_rect.right  = p_item->display.display_rect.right;
            str_rect.top    = p_item->display.display_rect.top + MMIQBTHEME_DOCKBAR_MENU_ITEM_ICON_SIZE ;
            str_rect.bottom = p_item->display.display_rect.bottom;
            
            GUISTR_DrawTextToLCDInRect(lcd_dev_info_ptr,
                &str_rect,
                &str_rect,
                &item_string, 
                &str_style, 
                str_state,
                GUISTR_TEXT_DIR_AUTO);
         }
        else
        {
            font_height = 0;
            icon_rect = GUI_GetCenterRect(icon_rect, img_width, img_height);
            //draw icon
            GUIRES_DisplayImg(PNULL, 
                &icon_rect, 
                PNULL, 
                p_item->display.win_id,
                function_data[p_item->id.qbtheme_func_id].icon , 
                lcd_dev_info_ptr); 
            if(MMIQBTHEME_DOCKBAR_PAGE_MAX_ITEM <= MMIQBTHEME_GetWidgetPagePreviewItemNum())
            {                   
                uint32 color = (uint32)RGB5652ARGB8888(MMI_BLACK_COLOR, 255);
                FillLayerWithMaskColor(&p_item->display.lcd_dev_info, &icon_rect, color);
            }                
        }        
  
        break;
        
    case MMIQBTHEME_ITEM_MENU:  
        {
            CTRLMAINMENU_ITEM_T menuitem_info = {0};
            BOOLEAN is_editing_mode = 0;
            is_editing_mode = (MMIQBTHEME_NORMAL != p_item->fsm_state)?1:0;
            MMIMAINMENU_GetItemData(
                p_item->id.menu_info.menu_id,
                p_item->id.menu_info.group_id,
                &menuitem_info);    
            
            MMIWIDGET_ShortCutDrawMenuIcon( p_item->display.win_id, &menuitem_info,  
                                             p_item->display.display_rect, 
                                             lcd_dev_info_ptr,
                                             IMAGE_QBTHEME_DOCKBAR_DELETE_ICON,is_editing_mode);
                                             
            if(MMIQBTHEME_ITEM_MOVING == p_item->fsm_state)
            {                
                UILAYER_WeakLayerAlpha(lcd_dev_info_ptr,  172   );
            }
            
            // MMITHEME_DestroyMenuDataInfo( menu_item_ptr );
        }
        break;    
        
    case MMIQBTHEME_ITEM_WIDGET_PAGE_PREVIEW_ICON:  
        {   
            //只有进入编辑状态后，才会刷新缩略图
            if(MMIQBTHEME_NORMAL == p_item->fsm_state)
            {
                break;
            }
            //缩略图没有更新
            if(0 != MMIQBIDLE_DisplayGridPageThumbView(p_item->item_index))
            {
                break;
            }
            //display delete icon
            if(1 < MMIQBTHEME_GetWidgetPagePreviewItemNum())
            {
                GUIRES_DisplayImg(PNULL, &p_item->display.display_rect, PNULL, p_item->display.win_id, 
                    IMAGE_QBTHEME_DOCKBAR_DELETE_ICON, lcd_dev_info_ptr);
            }
            if(MMIQBTHEME_ITEM_MOVING == p_item->fsm_state)
            {                
                UILAYER_WeakLayerAlpha(lcd_dev_info_ptr,  172 );
            }
            
        }
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return result;
}


//tp msg handle function of item
LOCAL MMI_RESULT_E ItemHandleTPMSG(MMIQBTHEME_ITEM_T *p_item,
                                   MMI_WIN_ID_T      win_id,
                                   MMI_MESSAGE_ID_E  msg_id,
                                   DPARAM            param
                                   )
{
    MMI_RESULT_E       result = MMI_RESULT_TRUE;  
    GUI_POINT_T        tp_point = {0};    
    
    if(p_item->type == MMIQBTHEME_ITEM_NULL)
    {
        return MMI_RESULT_FALSE;
    }
    
    tp_point.x = MMK_GET_TP_X(param);
    tp_point.y = MMK_GET_TP_Y(param); 
    
    switch (msg_id)
    {        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_DOWN:  
        p_item->tp_state = MSG_TP_PRESS_DOWN;           
        //DrawItem(p_item);
        if( (p_item->type == MMIQBTHEME_ITEM_MENU 
                && p_item->fsm_state == MMIQBTHEME_NORMAL)
            || p_item->type == MMIQBTHEME_ITEM_FUNC )
        {
            MMIQBTHEME_SetBufferAlpha(&p_item->display.lcd_dev_info, 
              &p_item->display.display_rect, 165);
            GUILCD_InvalidateRect(0, p_item->display.display_rect, 0);
        }
        ItemHandleFSM(p_item, msg_id,  MMI_RESULT_TRUE);        
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_UP:    
        if(p_item->fsm_state == MMIQBTHEME_ITEM_MOVING)
        {                
            ResetItemRectAfterTPUp(p_item,tp_point);
            ItemHandleFSM(p_item, msg_id,  result);            
            //DrawItem(p_item);
        }
        else if(p_item->fsm_state == MMIQBTHEME_NORMAL || p_item->fsm_state == MMIQBTHEME_EDITING)
        {
            if(p_item->tp_state != MSG_TP_PRESS_DOWN)   
            {                
                result = MMI_RESULT_FALSE;
                break;
            }
            result = ItemHandleClick( p_item, tp_point);
        }
        else
        {        
            result = MMI_RESULT_FALSE;
        }
        p_item->tp_state = MSG_TP_PRESS_UP;          
        
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_LONG: 
        if(p_item->type == MMIQBTHEME_ITEM_FUNC || 
            p_item->type == MMIQBTHEME_ITEM_NULL)
        {
            result = MMI_RESULT_FALSE;
            break;  
        }        
        p_item->prevous_tp_point.x = MMK_GET_TP_X(param);
        p_item->prevous_tp_point.y = MMK_GET_TP_Y(param);     
        if(p_item->type == MMIQBTHEME_ITEM_MENU)
        {
            //MMIQBTHEME_ItemSetDisplayObject( p_item,&p_item->display);   
            MMIQBTHEME_CreateDisplay(&p_item->display);       
        }
        MMIQBTHEME_AppendDisplay(&p_item->display); 
        ItemHandleFSM(p_item, msg_id,  result);
        p_item->tp_state = MSG_TP_PRESS_LONG;          
        DrawItem(p_item);

        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_MOVE:     
        
        if(p_item->type == MMIQBTHEME_ITEM_FUNC || 
            p_item->type == MMIQBTHEME_ITEM_NULL)
        {
            result = MMI_RESULT_FALSE;            
            break;  
        }
        
        if(p_item->fsm_state == MMIQBTHEME_ITEM_MOVING)
        {
            GUI_POINT_T current_point = {0};
            int32 offset_x = 0;
            int32 offset_y = 0;
            
            current_point.x = MMK_GET_TP_X(param);
            current_point.y = MMK_GET_TP_Y(param);        
            offset_x = current_point.x - p_item->prevous_tp_point.x;
            offset_y = current_point.y - p_item->prevous_tp_point.y;
            MMIQBTHEME_ItemMove(p_item, offset_x, offset_y); 
            p_item->prevous_tp_point = current_point;
            p_item->tp_state = MSG_TP_PRESS_MOVE;   
        }
        else
        {
            if(p_item->tp_state != MSG_TP_PRESS_DOWN
                || p_item->type == MMIQBTHEME_ITEM_WIDGET_PAGE_PREVIEW_ICON)      
            {
                p_item->tp_state = MSG_TP_PRESS_MOVE;   
            }
            result = MMI_RESULT_FALSE;
        }
        ItemHandleFSM(p_item, msg_id,  result);
        break;  
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        
    default:
        result = MMI_RESULT_FALSE;
        break;
     }
     
     return(result);     
}

//execute the specific function after click item
LOCAL MMI_RESULT_E ItemExecuteFuncion(MMIQBTHEME_ITEM_T *p_item
                                      )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    switch (p_item->type)
    {                
    case MMIQBTHEME_ITEM_FUNC:  //dockbar第一页中的功能
        if(PNULL != function_data[p_item->id.qbtheme_func_id].func)
        {
            function_data[p_item->id.qbtheme_func_id].func();   /*lint !e746 */
        }
        break;
        
    case MMIQBTHEME_ITEM_MENU:  //主菜单功能
        if(p_item->fsm_state == MMIQBTHEME_NORMAL)
        {            
            MMIAPIMENU_HandleIdleMenuItem(p_item->id.menu_info.group_id, p_item->id.menu_info.menu_id);
        }
        break;
        
    case MMIQBTHEME_ITEM_WIDGET_PAGE_PREVIEW_ICON ://widget页面缩略图    
        { 
            //MMIQBIDLE_SetCurPageIndex(p_item->item_index);
            MMIQBIDLE_SlidePage(p_item->item_index);            
            MMIQBTHEME_SetWidgetPreviewPageFocusedItem(p_item->item_index);   
        }
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return(result);    
}

//fsm 状态/ tp消息——转换函数
LOCAL int32 ItemHandleFSM(MMIQBTHEME_ITEM_T *p_item,
                          MMI_MESSAGE_ID_E  msg_id, 
                          MMI_RESULT_E      result
                          )
{
    //CHANGE FSM state
    if(result == MMI_RESULT_TRUE)
    {
        p_item->fsm_state = MMIQBTHEME_FSMTranfer(item_state_tranfer_array,p_item->fsm_state, msg_id);  /*lint !e64*/
    }
    return 0;
}                          

//return moving item current position 
PUBLIC int32 MMIQBTHEME_ItemGetDisplayRect(MMIQBTHEME_ITEM_T *p_item, GUI_RECT_T *rect)
{
    if(p_item->fsm_state != MMIQBTHEME_ITEM_MOVING) 
    {
        SCI_ASSERT(0);  /*assert verified*/
    }
    *rect = p_item->display.display_rect;   
    return 0;
}

//move item,only change display rect
PUBLIC int32 MMIQBTHEME_ItemMove(MMIQBTHEME_ITEM_T   *p_item , 
                                 int32 offset_x,
                                 int32 offset_y
                                 )
                                 
{
    p_item->display.display_rect.left += offset_x;
    p_item->display.display_rect.right += offset_x;
    p_item->display.display_rect.top +=  offset_y;
    p_item->display.display_rect.bottom += offset_y;
    if(p_item->fsm_state == MMIQBTHEME_ITEM_MOVING)
    {
        UILAYER_SetLayerPosition(&p_item->display.lcd_dev_info, 
            p_item->display.display_rect.left , 
            p_item->display.display_rect.top);
    }
    return 0;
} 

//set moving item current position
PUBLIC int32 MMIQBTHEME_ItemSetDisplayRect(MMIQBTHEME_ITEM_T *p_item, GUI_RECT_T rect)
{    
    p_item->display.display_rect = rect; 
    if(p_item->type == MMIQBTHEME_ITEM_WIDGET_PAGE_PREVIEW_ICON) 
    {
        UILAYER_SetLayerPosition(&p_item->display.lcd_dev_info,  rect.left, rect.top);
    }
    return 0;
}

//return  item layout position
PUBLIC int32 MMIQBTHEME_ItemGetLayoutRect(MMIQBTHEME_ITEM_T *p_item, GUI_RECT_T *rect)
{
    *rect = p_item->display.layout_rect;    
    return 0;
}

//set layout and display rect
PUBLIC int32 MMIQBTHEME_ItemSetRect(MMIQBTHEME_ITEM_T *p_item, GUI_RECT_T rect)
{
    p_item->display.layout_rect = rect ;  
    p_item->display.display_rect = rect ;  
    
    if(p_item->type == MMIQBTHEME_ITEM_WIDGET_PAGE_PREVIEW_ICON) 
    {
        UILAYER_SetLayerPosition(&p_item->display.lcd_dev_info, 
            p_item->display.display_rect.left, p_item->display.display_rect.top);
    }
    return 0;
}

//set item focused state
PUBLIC int32 MMIQBTHEME_ItemSetFocus(MMIQBTHEME_ITEM_T *p_item, int32 fucous_flag)
{
    p_item->focus_flag = fucous_flag;    
    //refresh
    //DrawItem(p_item);
    return 0;
}

//get delete icon rect
LOCAL int32 ItemGetDeleteIconRect(MMIQBTHEME_ITEM_T *p_item,
                                  GUI_RECT_T *rect
                                  )
{
    GUI_RECT_T         delete_icon_rect = {0};
    uint16             img_height  = 0;
    uint16             img_width   = 0;    
    uint16             added_height  = 0;
    uint16             added_width   = 0;    

    GUIRES_GetImgWidthHeight(&img_width, &img_height, 
        IMAGE_QBTHEME_DOCKBAR_DELETE_ICON, p_item->display.win_id);
#if defined MAINLCD_SIZE_320X480
    added_height  = (uint16)(1.5*img_height);
    added_width   = (uint16)(1.5*img_width);    
#elif defined MAINLCD_SIZE_240X320
    added_height  = img_height;
    added_width   = img_width;    
#else
    added_height  = img_height;
    added_width   = img_width;  
#endif

    //点击的区域实际范围是删除图标的9倍
    delete_icon_rect.left   = p_item->display.display_rect.left;
    delete_icon_rect.top    = p_item->display.display_rect.top ;
    delete_icon_rect.right  = p_item->display.display_rect.left + (int16)(added_width);
    delete_icon_rect.bottom = p_item->display.display_rect.top + (int16)(added_height);
    *rect = delete_icon_rect;
    return 0;
}

//tp up handle function of item 
LOCAL MMI_RESULT_E ItemHandleClick(MMIQBTHEME_ITEM_T *p_item, GUI_POINT_T point)
{
    MMI_RESULT_E result = MMI_RESULT_FALSE;    
    GUI_RECT_T         delete_icon_rect = {0};
    MMI_MESSAGE_ID_E    msg_id = MSG_TP_PRESS_UP;

    if(MMIQBTHEME_ITEM_MENU == p_item->type)
    {
            if(p_item->fsm_state == MMIQBTHEME_NORMAL)
            {
                result = ItemExecuteFuncion(p_item);
                ItemHandleFSM(p_item, msg_id,  result);              
            }
            else if(p_item->fsm_state == MMIQBTHEME_EDITING)            
            {
                ItemGetDeleteIconRect(p_item,&delete_icon_rect);
                if(GUI_PointIsInRect(point, delete_icon_rect))
                {
                    MMIQBTHEME_ItemDelete(p_item);
                }
            }                  
    }
    else if(MMIQBTHEME_ITEM_FUNC == p_item->type)
    {
            result = ItemExecuteFuncion(p_item);
            ItemHandleFSM(p_item, msg_id,  result);              
    }
    else if(MMIQBTHEME_ITEM_WIDGET_PAGE_PREVIEW_ICON == p_item->type)
    {
            if(p_item->fsm_state == MMIQBTHEME_EDITING)  
            {                        
                ItemGetDeleteIconRect(p_item,&delete_icon_rect);
                if(GUI_PointIsInRect(point, delete_icon_rect))
                {                    
                    if(1 < MMIQBTHEME_GetWidgetPagePreviewItemNum())
                    {
                        MMIQBTHEME_ItemDelete(p_item);
                    }
                }
                else
                {
                    result = ItemExecuteFuncion(p_item);
                    ItemHandleFSM(p_item, msg_id,  result);              
                }
            }
    }
    return result;
}

//handle function for moving item
LOCAL int32 ResetItemRectAfterTPUp(MMIQBTHEME_ITEM_T *p_item, GUI_POINT_T point)
{
    //moving 的后续滑动处理      
    int32 step_x = 10;  //10 pixels
    int32 step_y = 10;
    int32 frame = 0;
    int32 frame_index = 0;
    MMIQBTHEME_CalculateMovingParameter( &step_x , &step_y, &frame  ,
        p_item->display.display_rect, 
        p_item->display.layout_rect
        );
    
    MMIQBTHEME_AppendDisplay(&p_item->display);
    for(frame_index= 1; frame_index <= frame; frame_index++)
    {        
        MMIQBTHEME_ItemMove(p_item, step_x, step_y);   
        SCI_SLEEP(MMIQBTHEME_SLIDING_DELEY_TICKS);                   
        MMITHEME_UpdateRect();
    }                                    
    MMIQBTHEME_ItemSetDisplayRect(p_item, p_item->display.layout_rect);   
    MMITHEME_UpdateRect();    
       
    if(p_item->type == MMIQBTHEME_ITEM_WIDGET_PAGE_PREVIEW_ICON)
    {         
        MMIQBTHEME_RemoveDisplay(&p_item->display);
        if(p_item->focus_flag)
        {            
            MMIQBIDLE_SetCurPageIndex(p_item->item_index);  
            MMIQBIDLE_UpdateAllScreen();
        }        
        else
        {
            MMIQBIDLE_SlidePage(p_item->item_index);  
        }
        
        MMIQBTHEME_SetWidgetPreviewPageFocusedItem(p_item->item_index);   
        MMIQBTHEME_RemoveDisplay(&p_item->display);
    }     
    else if(p_item->type == MMIQBTHEME_ITEM_MENU)
    {        
        MMIQBTHEME_DestroyDisplay(&p_item->display);
    }

   return 0;
}


/*Edit by script, ignore 8 case. Thu Apr 26 19:01:11 2012*/ //IGNORE9527
