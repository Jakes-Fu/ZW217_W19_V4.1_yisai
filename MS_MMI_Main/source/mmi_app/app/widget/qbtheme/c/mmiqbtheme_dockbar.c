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
** 10/23/2009     ying.xu              Creat
******************************************************************************/


/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/

#include "mmi_app_widget_trc.h"
#include "mmk_kbd.h"
#include "guicommon.h"
#include "mmiwidget.h"
#include "mmiwidget_id.h"

#include "mmi_nv.h"

#include "mmi_modu_main.h"
//#include "mmiset.h"
#include "mmiset_export.h"

#include "mmk_type.h"
#include "mmk_app.h"
#include "mmk_kbd.h"
#include "guilcd.h"
#include "window_parse.h"
#include "guimenu.h"
#include "mmitheme_menu.h"
#include "mmiset_wintab.h"
#include "mmitheme_mainmenu.h"

#include "mmiwidget_export.h"
#include "mmi_mainmenu_export.h"
#ifdef PDA_UI_DROPDOWN_WIN
#include "mmidropdownwin_export.h"
#endif
#ifdef SEARCH_SUPPORT 
#include "mmisearch_export.h"
#endif

#include "mmiwidget_page.h"

#include "mmi_menutable.h"
#include "mmiqbtheme_config.h"
#include "mmiqbtheme_item.h"
#include "mmiqbtheme_grid.h"
#include "mmiqbtheme_page.h"
#include "mmiqbtheme_virtualwin.h"
#include "mmiqbtheme_dockbar.h"
#include "mmiqbtheme_idleexport.h"
#include "mmiqbtheme_idlewin.h"



LOCAL MMIQBTHEME_VIRTUALWIN_T *sp_dockbar = PNULL;     //dockbar 全局指针

/*
const LOCAL MMIMAINMENU_PAGE_ITEM_INFO_T s_init_menu_info = {4,
                                                    {
                                                    {MENU_QBTHEME_BAR,ID_COMMUNICATION_DIAL},
                                                    {MENU_QBTHEME_BAR,ID_COMMUNICATION_MESSAGE},
                                                    {MENU_QBTHEME_BAR,ID_COMMUNICATION_PB},
                                                    {MENU_QBTHEME_BAR,ID_MAINMENU_RETURNIDLE},
                                                    0}
                                                        };
                                                        
*/
//query whether to procced deleting item operation

//dockbar bar中删除某页的项时，提示窗口处理函数
LOCAL MMI_RESULT_E  HandleDeletingItemQueryWin(
                                               MMI_WIN_ID_T    win_id, 
                                               MMI_MESSAGE_ID_E   msg_id, 
                                               DPARAM             param
                                               );


/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
LOCAL int32 InitDockbarPage2(MMIQBTHEME_PAGE_T *p_page)
{
    MMIMAINMENU_PAGE_ITEM_INFO_T menu_info = {0};
    int32 i = 0;
    MMIQBTHEME_ITEM_T   *p_temp_item = 0;
    int32 item_num = 0;
    
    MMIAPIMENU_ReadQbThemeDockbar(&menu_info);

    item_num= MIN(MMIQBTHEME_DOCKBAR_PAGE2_MAX_ITEM, menu_info.item_num);
    MMIQBTHEME_GridSetRowCol(p_page->p_grid, 1, item_num);     
    MMIQBTHEME_GridSetOneGridWidthHeight (p_page->p_grid, 
        MMIQBTHEME_DOCKBAR_MENU_ITEM_WIDTH, MMIQBTHEME_DOCKBAR_MENU_ITEM_HEIGHT);     
        
    
    for(i = 0; i < item_num; i++)
    {
        if(menu_info.menu_item_info[i].menu_id == ID_MAINMENU_RETURNIDLE)
        {
            MMIQBTHEME_ItemCreate(&p_temp_item, MMIQBTHEME_ITEM_FUNC );
            p_temp_item->id.qbtheme_func_id = 3;    
        }
        else
        {
            MMIQBTHEME_ItemCreate(&p_temp_item, MMIQBTHEME_ITEM_MENU );
            p_temp_item->id.menu_info = menu_info.menu_item_info[i];    
        }        
        MMIQBTHEME_PageAddItem(p_page,  p_temp_item);
    }    
    return 0;
}

//reset factory settings
PUBLIC int32 MMIQBTHEME_ResetDockbarPage2Data(void)
{
    MMIMAINMENU_PAGE_ITEM_INFO_T menu_info = {0};
    MMIAPIMENU_ResetQbThemeDockbar(&menu_info);
    return 0;
}

//save dockbar page2 data
PUBLIC int32 MMIQBTHEME_SaveDockbarPage2Data(MMIQBTHEME_PAGE_T *p_page)
{
    MMIMAINMENU_PAGE_ITEM_INFO_T menu_info = {0};
    int32 i = 0;
    
    menu_info.item_num = p_page->item_num;
    for(i = 0; i < p_page->item_num; i++)
    {
        if(p_page->p_item[i]->type == MMIQBTHEME_ITEM_FUNC)
        {
            menu_info.menu_item_info[i].group_id = 0;//MENU_QBTHEME_BAR;
            menu_info.menu_item_info[i].menu_id = ID_MAINMENU_RETURNIDLE ;
        }
        else
        {           
            menu_info.menu_item_info[i] = p_page->p_item[i]->id.menu_info;
        }        
    }    
    MMI_WriteNVItem( MMINV_QBTHEME_DOCKBAR_INFO, &menu_info);
    return 0;
}

/*****************************************************************************/
//  Description : re-initial grid idle dockbar page2
//  Global resource dependence : 
//  Author: YING.XU
//  Note:
/*****************************************************************************/
PUBLIC void MMIQBTHEME_InitDockbarPage2(void)
{
    MMIQBTHEME_PAGE_T *p_page = 0;
    p_page = sp_dockbar->p_page[1];
    MMIQBTHEME_PageDeleteAllItems( p_page);
    InitDockbarPage2(p_page);
    MMIQBTHEME_PageSetFSMState( p_page,  p_page->fsm_state);
}

/*****************************************************************************/
//  Description : initial grid idle dockbar
//  Global resource dependence : 
//  Author: YING.XU
//  Note:
/*****************************************************************************/
PUBLIC void MMIQBTHEME_InitDockbar(MMI_IDLE_GRID_DATA_T *idle_grid_ptr)
{
   
    GUI_RECT_T full_rc = MMITHEME_GetFullScreenRect();
    GUI_RECT_T soft_bg = {0};
    MMIQBTHEME_PAGE_T *p_temp_page = 0;
    MMIQBTHEME_DISPLAY_T   temp_display = {0};
    MMIQBTHEME_ITEM_T   *p_temp_item = 0;
 
    int32 i = 0;
    
    soft_bg = full_rc;
    soft_bg.top = soft_bg.bottom - MMIQBTHEME_DOCKBAR_HEIGHT + 1;  
    
    MMIQBTHEME_VirtualWinCreate(&sp_dockbar,MMIQBTHEME_VIRTUALWIN_DOCKBAR);
    
    temp_display.display_rect = soft_bg;
    temp_display.layout_rect = soft_bg;
    temp_display.win_id = idle_grid_ptr->win_id;
    MMIQBTHEME_VirtualWinSetDisplayObject(sp_dockbar, &temp_display  );
    
    MMIQBTHEME_PageCreate(&p_temp_page, MMIQBTHEME_PAGE_DOCKBAR_PAGE1);         
    MMIQBTHEME_VirtualWinAddPage(sp_dockbar,   p_temp_page   );  
    
    //GUIRES_GetImgWidthHeight(&width, &height, IMAGE_THEME_ADD_WIDGET, idle_grid_ptr->win_id);
    MMIQBTHEME_GridSetRowCol(p_temp_page->p_grid, 1, 3);
    MMIQBTHEME_GridSetOneGridWidthHeight (p_temp_page->p_grid, 
            MMIQBTHEME_DOCKBAR_ITEM_WIDTH, MMIQBTHEME_DOCKBAR_ITEM_HEIGHT);
    
    //create item   
    MMIQBTHEME_ItemCreate(&p_temp_item, MMIQBTHEME_ITEM_FUNC ); 
    p_temp_item->id.qbtheme_func_id = 0;       
    //add to page    
    MMIQBTHEME_PageAddItem(p_temp_page,  p_temp_item);   
    
    MMIQBTHEME_ItemCreate(&p_temp_item, MMIQBTHEME_ITEM_FUNC );
    p_temp_item->id.qbtheme_func_id = 1;    
    MMIQBTHEME_PageAddItem(p_temp_page,  p_temp_item);
    
    MMIQBTHEME_ItemCreate(&p_temp_item, MMIQBTHEME_ITEM_FUNC );
    p_temp_item->id.qbtheme_func_id = 2;    
    MMIQBTHEME_PageAddItem(p_temp_page,  p_temp_item);    

    
    MMIQBTHEME_PageCreate(&p_temp_page, MMIQBTHEME_PAGE_DOCKBAR_PAGE2);    
    MMIQBTHEME_VirtualWinAddPage(sp_dockbar,   p_temp_page   );   
    InitDockbarPage2(p_temp_page);
    
    MMIQBTHEME_PageCreate(&p_temp_page, MMIQBTHEME_PAGE_DOCKBAR_PAGE3);    
    MMIQBTHEME_VirtualWinAddPage(sp_dockbar,   p_temp_page   );  
    p_temp_page->p_grid->rect.right  +=  p_temp_page->p_grid->rect.right 
        - p_temp_page->p_grid->rect.left + 1;
    MMIQBTHEME_GridSetRowCol(p_temp_page->p_grid, 1, 12);     
    
    MMIQBTHEME_GridSetOneGridWidthHeight (p_temp_page->p_grid,
            MMIQBTHEME_DOCKBAR_PREVIEW_ITEM_WIDTH, MMIQBTHEME_DOCKBAR_PREVIEW_ITEM_HEIGHT);     
 
    //add to page       
    for(i = 0; i < idle_grid_ptr->page_num; i++)
    {
        MMIQBTHEME_ItemCreate(&p_temp_item, MMIQBTHEME_ITEM_WIDGET_PAGE_PREVIEW_ICON );  
        p_temp_item->id.widget_page_index = i;
        MMIQBTHEME_PageAddItem(p_temp_page,  p_temp_item);  
    }
    
    MMIQBTHEME_ItemCreate(&p_temp_item, MMIQBTHEME_ITEM_FUNC );  
    p_temp_item->id.qbtheme_func_id = 4;
    MMIQBTHEME_PageAddSpecialItem(p_temp_page,p_temp_item);
    {
        GUI_RECT_T rect = {0};
        int32 grid_x_interval = 0;
        int32 grid_y_interval = 0;
        MMIQBTHEME_GridGetOneGridRect(p_temp_page->p_grid,  5, &rect);   
        
        temp_display.win_id = p_temp_page->display.win_id;
        temp_display.display_rect = rect;
        temp_display.layout_rect = rect;
        temp_display.lcd_dev_info = p_temp_page->display.lcd_dev_info;
        MMIQBTHEME_ItemSetDisplayObject( p_temp_item,&temp_display); 
        
        MMIQBTHEME_GridGetOneGridRect(p_temp_page->p_grid,  4, &rect); 
        MMIQBTHEME_GridGetGridInterval(p_temp_page->p_grid, &grid_x_interval, &grid_y_interval);
        //reset page display rect
        p_temp_page->display.display_rect.left +=  grid_x_interval;         
        p_temp_page->display.display_rect.right = rect.right + grid_x_interval;
        
    }         
    
    MMIQBTHEME_VirtualWinSetCurrentPage(sp_dockbar,1);    
    MMIQBTHEME_VirtualWinSetFSMState(sp_dockbar, MMIQBTHEME_NORMAL );
}

PUBLIC MMIQBTHEME_VIRTUALWIN_T ** MMIQBTHEME_GetDockbarPtrPtr()
{
    return &sp_dockbar;
}

PUBLIC MMIQBTHEME_VIRTUALWIN_T * MMIQBTHEME_GetDockbarPtr()
{
    return sp_dockbar;
}


PUBLIC GUI_RECT_T MMIQBIDLE_GetDockbarRect(
                                            MMI_IDLE_GRID_DATA_T *idle_grid_ptr                       
                                            )
{
    GUI_RECT_T     rect = MMITHEME_GetFullScreenRect();        
    
    rect.top = rect.bottom - MMIQBTHEME_DOCKBAR_HEIGHT + 1;
    return rect;
}


PUBLIC MMI_RESULT_E MMIQBTHEME_HandleDockbarMsg(                                
                                        MMI_IDLE_GRID_DATA_T        *idle_grid_ptr ,
                                        MMI_MESSAGE_ID_E  msg_id,
                                        DPARAM            param
)
{
    MMI_RESULT_E	             result = MMI_RESULT_FALSE;
    GUI_POINT_T	                 point = {0, 0};
    GUI_RECT_T                   dockbar_rect = {0};
    static uint32                last_tp_move_time = 0;         
     uint32                      current_time = 0;                     
    //退出编辑状态由widget页面统一处理,timer不处理
    if(msg_id == MSG_APP_RED 
        || msg_id == MSG_APP_CANCEL
        || msg_id == MSG_TIMER
        )
    {
         return result;
    }
    //tp msg 
if( MSG_TP_PRESS_DOWN <= msg_id && msg_id <= MSG_TP_GESTURE)
{    
    //widget页面的icon处于滑动状态时，dockbar不处理tp消息
    if (MMIGRID_TP_MOVE_IDLE_TO_IDLE == idle_grid_ptr->widget_mov_type
        || MMIGRID_TP_MOVE_PAGE == idle_grid_ptr->widget_mov_type)
    {
        result = MMI_RESULT_FALSE;
        return result;
    }
    
    point.x = MMK_GET_TP_X(param);
    point.y = MMK_GET_TP_Y(param);        
    MMIQBTHEME_VirtualWinGetRect(sp_dockbar,  &dockbar_rect );
        //check rect,当处于MMIQBTHEME_ITEM_MOVING 的状态时例外。
        //原因是item有可能移动到dockbar之外,tp move/up 消息除外
    if(GUI_PointIsInRect( point, dockbar_rect)
        || msg_id == MSG_TP_PRESS_UP        
        || sp_dockbar->fsm_state == MMIQBTHEME_ITEM_MOVING
        //|| msg_id == MSG_TP_PRESS_MOVE
        )
    {
        current_time = SCI_GetTickCount();
        if(msg_id == MSG_TP_PRESS_MOVE)
        {
            if((current_time - last_tp_move_time) < 20) //driver adc frequency > 10ms
            {                
                result = MMI_RESULT_FALSE;
                return result;
            }
            last_tp_move_time = current_time;
        }
        result = MMIQBTHEME_VirtualWinHandleMsg(sp_dockbar,  idle_grid_ptr->win_id, msg_id,param);        
    }
}
else
{
    result = MMIQBTHEME_VirtualWinHandleMsg(sp_dockbar,  idle_grid_ptr->win_id, msg_id,param);         
}

    return result;
}

PUBLIC MMIQBTHEME_FSM_STATE_E MMIQBTHEME_FSMTranfer( MMIQBTHEME_FSM_STATE_E array[],                                                    
                                                    MMIQBTHEME_FSM_STATE_E old_state,
                                                    MMI_MESSAGE_ID_E  msg_id                                                     
                                                    )
{
    int32 i = 0;
    if(MSG_TP_PRESS_DOWN <= msg_id && msg_id <= MSG_TP_PRESS_MOVE)
    {
        i = msg_id - MSG_TP_PRESS_DOWN;
    }
    else if(msg_id <= MSG_TP_PRESS_LONG)
    {
        i = 3;
    }
    return array[old_state*4 + i];    
}

/*****************************************************************************/
//  Description : create softkey layer
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note: 
/*****************************************************************************/
PUBLIC void MMIQBTHEME_CreateDisplay( MMIQBTHEME_DISPLAY_T  *display)
{
    GUI_LCD_DEV_INFO widget_layer_handle = {0};
    uint16 width = 0;
    uint16 height = 0;
    UILAYER_CREATE_T create_info = {0};  
    GUI_RECT_T rect = display->display_rect;  
    
    width  = rect.right - rect.left + 1;
    height = rect.bottom - rect.top + 1;
    create_info.lcd_id          = LCD_ID_0;
    create_info.owner_handle    = display->win_id;
    create_info.offset_x        = rect.left;
    create_info.offset_y        = rect.top;
    create_info.width           = width;
    create_info.height          = height;
    create_info.is_bg_layer     = FALSE;
    create_info.is_static_layer = FALSE;
    create_info.format          = 0;
    
    UILAYER_CreateLayer(&create_info, &widget_layer_handle);
    
    display->lcd_dev_info  = widget_layer_handle;
    
    if (UILAYER_IsLayerActive(&widget_layer_handle))
    {
        UILAYER_Clear(&widget_layer_handle);
    }
}

/*****************************************************************************/
//  Description : create softkey layer
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note: 
/*****************************************************************************/
PUBLIC void MMIQBTHEME_CreateDisplayEx( MMIQBTHEME_DISPLAY_T  *display)
{
    GUI_LCD_DEV_INFO widget_layer_handle = {0};
    uint16 width = 0;
    uint16 height = 0;
    UILAYER_CREATE_T create_info = {0};  
    GUI_RECT_T rect = display->display_rect;  

    width  = rect.right - rect.left + 1;
    height = rect.bottom - rect.top + 1;
    create_info.lcd_id          = LCD_ID_0;
    create_info.owner_handle    = display->win_id;
    create_info.offset_x        = rect.left;
    create_info.offset_y        = rect.top;
    create_info.width           = width;
    create_info.height          = height;
    create_info.is_bg_layer     = FALSE;
    create_info.is_static_layer = TRUE;
    create_info.format          = 0;
    
    UILAYER_CreateLayer(&create_info, &widget_layer_handle);        
    display->lcd_dev_info  = widget_layer_handle;
    
    if (UILAYER_IsLayerActive(&widget_layer_handle))
    {
        UILAYER_Clear(&widget_layer_handle);
    }
}

/*****************************************************************************/
//  Description : drwa page title
//  Global resource dependence : 
//  Author: YING.XU
//  Note: 
/*****************************************************************************/
PUBLIC void MMIQBTHEME_DestroyDisplay( MMIQBTHEME_DISPLAY_T  *display)
{
    UILAYER_RELEASELAYER(&display->lcd_dev_info);     /*lint !e506 !e774*/
}

/*****************************************************************************/
//  Description : drwa page title
//  Global resource dependence : 
//  Author: YING.XU
//  Note: 
/*****************************************************************************/
PUBLIC void MMIQBTHEME_AppendDisplay( MMIQBTHEME_DISPLAY_T  *display)
{
    if (UILAYER_IsBltLayer(&display->lcd_dev_info))
    {
        return;        
    } 
    if (!UILAYER_IsLayerActive(&display->lcd_dev_info))
    {
        return;        
    }     
    {
        UILAYER_APPEND_BLT_T append_layer = {0};
        append_layer.lcd_dev_info = display->lcd_dev_info;
        append_layer.layer_level = UILAYER_LEVEL_NORMAL;
        UILAYER_AppendBltLayer(&append_layer);   
    }
}

/*****************************************************************************/
//  Description : drwa page title
//  Global resource dependence : 
//  Author: YING.XU
//  Note: 
/*****************************************************************************/
PUBLIC int32 MMIQBTHEME_SetBufferAlpha(GUI_LCD_DEV_INFO *layer_ptr, GUI_RECT_T *rect_ptr, uint32 alpha)
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
        return -1;
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
            return -2;
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
    return (int32)mask_height;
}

/*****************************************************************************/
//  Description : drwa page title
//  Global resource dependence : 
//  Author: YING.XU
//  Note: 
/*****************************************************************************/
PUBLIC void MMIQBTHEME_RemoveDisplay( MMIQBTHEME_DISPLAY_T  *display)
{
    if (!UILAYER_IsBltLayer(&display->lcd_dev_info))
    {
        return;        
    }     
    {
        UILAYER_APPEND_BLT_T append_layer = {0};
        append_layer.lcd_dev_info = display->lcd_dev_info;
        append_layer.layer_level = UILAYER_LEVEL_NORMAL;
        UILAYER_RemoveBltLayer(&append_layer);   
    }
}

//get a widget page preview icon in dockbar page3
PUBLIC int32 MMIQBTHEME_GetWidgetPagePreviewItemNum(void)                       
{
    return sp_dockbar->p_page[2]->item_num;
}

//insert a widget page preview icon in dockbar page3
PUBLIC int32 MMIQBTHEME_AddWidgetPagePreviewItemEx(void)                       
{
    MMIQBTHEME_PAGE_T *p_page = sp_dockbar->p_page[2];
    int32 new_index = p_page->focused_item_index + 1;
    int32 result = 0;
    
    result = MMIQBTHEME_AddWidgetPagePreviewItem();
    if(0 == result)
    {
        MMIQBIDLE_SlidePage(new_index);        
        //MMIQBIDLE_SetCurPageIndex(i);  
        MMIQBTHEME_SetWidgetPreviewPageFocusedItem(new_index);
        MMIQBTHEME_PageSlide2FocusedItem(p_page);  
        MMIQBTHEME_PageDraw(p_page);  
    }
    return result;
}


//insert a widget page preview icon in dockbar page3
PUBLIC int32 MMIQBTHEME_AddWidgetPagePreviewItem(void)                       
{
    MMIQBTHEME_ITEM_T *p_temp_item = 0;
    MMIQBTHEME_PAGE_T *p_page = sp_dockbar->p_page[2];
    int32 result = 0;
    int32 new_index = p_page->focused_item_index + 1;
    MMI_IDLE_GRID_DATA_T *idle_grid_ptr = MMIAPIQBIDLE_GetGridPtr();
    
    //在制定widget页面位置加入一页
    if(MMIWIDGET_AddWidgetPage(idle_grid_ptr, new_index) != 0)
    {
        return -1;
    }
    
    //add to page         
    MMIQBTHEME_ItemCreate(&p_temp_item, MMIQBTHEME_ITEM_WIDGET_PAGE_PREVIEW_ICON );  

    
    if(0 != MMIQBTHEME_PageInsertItem(p_page,  p_temp_item, new_index))
    {
        //add error 
        MMIQBTHEME_ItemDestroy(p_temp_item); 
        return -2;
    }
    
    //MMIQBIDLE_SlidePage(new_index);        
    //MMIQBIDLE_SetCurPageIndex(i);  
    //MMIQBTHEME_SetWidgetPreviewPageFocusedItem(new_index);
    //result = MMIQBTHEME_PageSlide2FocusedItem(p_page);    
    return result;
}

//set  focued 
PUBLIC int32 MMIQBTHEME_SetWidgetPreviewPageFocusedItem(int32 item_index)                       
{
    MMIQBTHEME_PAGE_T* p_page = sp_dockbar->p_page[2];   
    int32 result = 0;
    //set focus item
    result =  MMIQBTHEME_PageSetFocusItem(p_page,item_index);

    return result;
    
}

//slide page to  focus item position
PUBLIC int32  MMIQBTHEME_SlideWidgetPreviewPage2FocusedItem(void)
{
    MMIQBTHEME_PAGE_T* p_page = sp_dockbar->p_page[2];    
    int32 result = 0;
    //set focus item
    result = MMIQBTHEME_PageSlide2FocusedItem(p_page);    

    return result;
}


//input start_rect -> end_rect
// input/output step_x/step_y //xy方向上的默认步长
//output frame      计算出来的帧数
PUBLIC int32 MMIQBTHEME_CalculateMovingParameter( int32 *step_x , int32 *step_y, int32 *frame  ,
                                                 GUI_RECT_T start_rect, GUI_RECT_T end_rect
                                                 )
{
    //moving 的后续滑动处理      
    int32 offset_x = 0;
    int32 offset_y = 0;
    int32 frame_x = 0;
    int32 frame_y = 0;
    
    offset_x = end_rect.left - start_rect.left ;
    offset_y = end_rect.top - start_rect.top ;
    if(*step_x != 0)
    {
        frame_x =  abs(offset_x / *step_x);
    }
    else
    {
        frame_x =  1;
    }
    if(*step_y != 0)
    {
        frame_y =  abs(offset_y / *step_y);
    }
    else
    {
        frame_y =  1;
    }    
    
    if(frame_x > frame_y)
    {
        *frame =  frame_x;
    }
    else
    {
        *frame = frame_y;
    }
    
    if(*frame != 0)
    {
        *step_y = offset_y / *frame;
        *step_x = offset_x / *frame;     
    }    
    return 0;
}

PUBLIC int32 MMIQBTHEME_InsertMenuIconItem(
                                           int32 item_position, 
                                           MMIMAINMENU_ITEM_INFO_T menu_id
                                           )
{
    MMIQBTHEME_ITEM_T *p_temp_item = 0;
    MMIQBTHEME_PAGE_T *p_page = sp_dockbar->p_page[1];
    int32 result = 0;
    //add to page         
    if(MMIQBTHEME_ItemCreate(&p_temp_item, MMIQBTHEME_ITEM_MENU )!= 0)
    {
        return -1;
    }
    p_temp_item->id.menu_info = menu_id;
    p_temp_item->item_index = item_position;
    p_temp_item->grid_index = item_position;    
    result = MMIQBTHEME_PageInsertItem(p_page, p_temp_item, item_position )  ;     
    //MMIQBTHEME_SaveDockbarPage2Data(p_page);
    return result;
}

//delete one item of dockbar 
PUBLIC int32 MMIQBTHEME_DeleteItem(
                                   MMIQBTHEME_ITEM_T *p_item 
                                   )
{
    int32 i = 0;    
    MMIQBTHEME_VIRTUALWIN_T *p_dockbar =    MMIQBTHEME_GetDockbarPtr();
    MMIQBTHEME_PAGE_T*  p_page = p_dockbar->p_page[p_dockbar->current_page];
    
    //find item
    for(i=0; i < p_page->item_num ; i++)
    {
        if(p_item == p_page->p_item[i])
        {
            MMIQBTHEME_PageDeleteItem(p_dockbar->p_page[p_dockbar->current_page],i );
            if(p_page->type == MMIQBTHEME_PAGE_DOCKBAR_PAGE3)
            {
                MMIQBTHEME_SlideWidgetPreviewPage2FocusedItem();
            }
            return 0;
        }
    }   
    return -1;
}

PUBLIC int32 MMIQBTHEME_OpenDeletingItemQueryWin(
                                                 MMIQBTHEME_ITEM_T *p_item 
                                                 )
{
    if(p_item->type == MMIQBTHEME_ITEM_MENU)
    {
        return MMIQBTHEME_DeleteItem(p_item);
    }
    MMIPUB_OpenQueryWinByTextIdEx(MMK_GetFirstAppletHandle(),
        TXT_DELETEQUERY, IMAGE_PUBWIN_QUERY,PNULL,
        HandleDeletingItemQueryWin, (uint32)p_item ); 
    return 0;
}

//query whether to procced deleting item operation
LOCAL MMI_RESULT_E  HandleDeletingItemQueryWin(
                                               MMI_WIN_ID_T    win_id, 
                                               MMI_MESSAGE_ID_E   msg_id, 
                                               DPARAM             param
                                               )
{
    MMI_RESULT_E   result =   MMI_RESULT_TRUE;
    switch (msg_id)
    {
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        {
            MMIPUB_INFO_T       *pub_win_ptr = PNULL;     
            MMIQBTHEME_ITEM_T   *p_item  = PNULL; 
            MMI_IDLE_GRID_DATA_T *idle_grid_ptr = MMIAPIQBIDLE_GetGridPtr();
            //get pubwin data
            pub_win_ptr = MMIPUB_GetWinAddDataPtr(win_id);
            if(pub_win_ptr == PNULL)
            {                
                //MMIQBIDLE_DEBUG:"pub_win_ptr == 0"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIQBTHEME_DOCKBAR_700_112_2_18_3_8_57_260,(uint8*)"");
                MMK_CloseWin(win_id);
                break;
            }
            //get file info
            p_item = (MMIQBTHEME_ITEM_T *)pub_win_ptr->user_data;
            if(MMIQBTHEME_ITEM_WIDGET_PAGE_PREVIEW_ICON == p_item->type)
            {
                MMIWIDGET_DeleteWidgetPage(idle_grid_ptr, p_item->item_index);
            }
            MMIQBTHEME_DeleteItem(p_item);
            MMK_CloseWin(win_id);
        }
        break;
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    default:
        result = MMIPUB_HandleQueryWinMsg(win_id,msg_id,param);
        break;
    }
    return result;
}                                        

PUBLIC int32 MMIQBTHEME_MoveMenu2WidgetWin(MMIQBTHEME_PAGE_T *p_page, int32 index, GUI_POINT_T point)
{
    MMIMAINMENU_ITEM_INFO_T shortcut_info = {0};
    GUI_RECT_T rect = {0};
    GUI_POINT_T offset_point= {0};
    MMI_IDLE_GRID_DATA_T *idle_grid_ptr = MMIAPIQBIDLE_GetGridPtr();
    
    shortcut_info = p_page->p_item[index]->id.menu_info;
    
    //add to idle
    if(MMIAPIQBIDLE_AddMenu2Idle( shortcut_info,  point))
    { 
        //交还给widget页面的代码控制  
        MMIQBTHEME_ItemGetDisplayRect(p_page->p_item[index],&rect);
        offset_point.x = rect.left;
        offset_point.y = rect.top;
        //delete one item        
        MMIQBTHEME_DestroyDisplay(&p_page->p_item[index]->display);
        MMIQBTHEME_PageDeleteItem( p_page,index);
        MMIQBTHEME_PageDraw(p_page);
        //MMIQBTHEME_SaveDockbarPage2Data(p_page);
        
        if(idle_grid_ptr->cur_page_index != idle_grid_ptr->new_page_index)
        {        
            MMIQBIDLE_SlidePage(idle_grid_ptr->new_page_index);
        }
        //MMIQBIDLE_SetCurPageIndex(i);          
        MMIQBTHEME_SetWidgetPreviewPageFocusedItem(idle_grid_ptr->new_page_index)  ;  
        MMIQBTHEME_SlideWidgetPreviewPage2FocusedItem();
        MMIQBIDLE_ResetStateAfterMovingMenu2WidgetWin(offset_point);
        return 0;
     }
     return -1;
}

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/



/*Edit by script, ignore 1 case. Fri Apr 27 09:38:56 2012*/ //IGNORE9527
