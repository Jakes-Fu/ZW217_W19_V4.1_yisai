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


#include "mmk_kbd.h"
#include "guicommon.h"
#include "mmidisplay_data.h"
#include "mmiwidget_id.h"
#include "mmiwidget_image.h"
#include "mmi_theme.h"
#include "mmiwidget.h"
#include "mmi_nv.h"
#include "mmiwidget_image.h"
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
#include "mmitheme_mainmenu.h"
#include "mmiset_wintab.h"
#include "mmiwidget_export.h"
#include "mmi_mainmenu_export.h"

#ifdef PDA_UI_DROPDOWN_WIN
#include "mmidropdownwin_export.h"
#endif
#ifdef SEARCH_SUPPORT 
#include "mmisearch_export.h"
#endif
#include "mmiqbtheme_idleexport.h"
#include "mmiwidget_page.h"
#include "mmiqbtheme_config.h"
#include "mmiqbtheme_internal.h"
#include "mmiqbtheme_item.h"
#include "mmiqbtheme_grid.h"
#include "mmiqbtheme_page.h"
#include "mmiqbtheme_virtualwin.h"

#include "mmiqbtheme_dockbar.h"

LOCAL int32 DrawPage(MMIQBTHEME_PAGE_T* p_page );
LOCAL int32 FindMovingItem(MMIQBTHEME_PAGE_T* p_page);


//return item index
LOCAL int32 FindExchangeItemWithMovingItem(MMIQBTHEME_PAGE_T* p_page,int32 moving_item_index);

LOCAL int32 PlayExchangingAnimation(MMIQBTHEME_PAGE_T* p_page,
                                    int32   moving_item_index,
                                    int32   exchaning_item_index
                                    );
//return item index
LOCAL int32 ExchangeItem(MMIQBTHEME_PAGE_T* p_page,  int32   old_item_id, int32   new_item_id );

//reset item initial position
LOCAL int32 ResetPageItemInitialPosition(MMIQBTHEME_PAGE_T* p_page) ;  

//draw rect in buffer
LOCAL void DrawRectFrame( 
                         GUI_LCD_DEV_INFO  *dev_info_ptr,  // block information
                         GUI_RECT_T                 *buffer_rect,
                         GUI_RECT_T                 *display_rect,      //rectangle to draw
                         GUI_COLOR_T                color           //color to draw
                         );

//在一页内对item进行滑动
LOCAL int32 SlidePageItem(MMIQBTHEME_PAGE_T* p_page,int32 offset_x, int32 offset_y);


//设置成员变量的值，申请内存
PUBLIC int32 MMIQBTHEME_PageCreate(MMIQBTHEME_PAGE_T   ** p_page, MMIQBTHEME_PAGE_TYPE_E type)
{
    MMIQBTHEME_PAGE_T * p_temp_page = 0;
    MMIQBTHEME_GRID_T *p_temp_grid = 0;
    /*
    if(p_page == PNULL)
    {
    return -1;
    }
    */   
    p_temp_page = (MMIQBTHEME_PAGE_T *)SCI_ALLOC_APP(sizeof(MMIQBTHEME_PAGE_T));
    SCI_MEMSET(p_temp_page, 0, sizeof(MMIQBTHEME_PAGE_T));
    
    //set display object    
    
    //create grid object            
    MMIQBTHEME_GridCreate(&p_temp_grid);
    
    p_temp_page->p_grid = p_temp_grid;
    p_temp_page->type = type;
    *p_page = p_temp_page;
    return 0;
}

//destroy an page  objcet
PUBLIC int32 MMIQBTHEME_PageDestroy(MMIQBTHEME_PAGE_T* p_page)
{
    int32 i = 0;
    MMIQBTHEME_ITEM_LIST_T *p_temp_item_list = 0;
    MMIQBTHEME_ITEM_LIST_T *p_tofree_item_list = 0;
    
    for(i = 0; i< p_page->item_num; i++)
    {
        MMIQBTHEME_ItemDestroy(p_page->p_item[i]);
    }

    p_temp_item_list = p_page->p_special_item_head;    
    while(p_temp_item_list != 0)
    {
        if(p_temp_item_list->p_item != 0)
        {            
            MMIQBTHEME_ItemDestroy(p_temp_item_list->p_item);            
        }
        p_temp_item_list = p_temp_item_list->p_next_list_item;    
    }

    p_temp_item_list = p_page->p_special_item_head;    
    while(p_temp_item_list != 0)
    {
        p_tofree_item_list = p_temp_item_list; 
        p_temp_item_list = p_temp_item_list->p_next_list_item;            
        SCI_FREE(p_tofree_item_list);
    }
    
    MMIQBTHEME_DestroyDisplay(&p_page->display);
    MMIQBTHEME_GridDestroy(p_page->p_grid);
    SCI_FREE(p_page);
    
    return 0;
}


//窗口处理函数
PUBLIC MMI_RESULT_E MMIQBTHEME_PageHandleMsg(MMIQBTHEME_PAGE_T* p_page,
                                             MMI_WIN_ID_T      win_id,
                                             MMI_MESSAGE_ID_E  msg_id,
                                             DPARAM            param
                                             )
{
    MMI_RESULT_E  result = MMI_RESULT_TRUE;
    MMI_RESULT_E  temp_result = MMI_RESULT_FALSE;
    int32 item_index = 0;
    MMIQBTHEME_ITEM_LIST_T *p_temp_item_list = 0;

    if((MSG_TP_PRESS_DOWN == msg_id )       //except tp up / tp move msg
        || (MSG_TP_PRESS_SHORT <= msg_id && msg_id <= MSG_TP_GESTURE))
    {
        GUI_POINT_T point = {0};
        point.x = MMK_GET_TP_X(param);
        point.y = MMK_GET_TP_Y(param);     
        if(GUI_PointIsInRect( point, p_page->display.display_rect))
        { 
            for(item_index = 0 ; item_index < p_page->item_num  ; item_index++)
            {   
                if(MMI_RESULT_TRUE == MMIQBTHEME_ItemHandleMsg(p_page->p_item[item_index],win_id,
                    msg_id,  param ) )
                {
                    temp_result = MMI_RESULT_TRUE;  
                    //only one item can handle tp message, once msg was handled, 
                    //break and exit the loop
                    break;  
                }
            }
        }
    }
    else if(MSG_FULL_PAINT != msg_id )
    {        
        for(item_index = 0 ; item_index < p_page->item_num  ; item_index++)
        {   
            if(MMI_RESULT_TRUE == MMIQBTHEME_ItemHandleMsg(p_page->p_item[item_index],win_id,            
                msg_id,  param ) )
            {
                temp_result = MMI_RESULT_TRUE;  
            }
        }        
    }
    
    //特殊处理的item
    if(MSG_FULL_PAINT != msg_id )
    {
        p_temp_item_list = p_page->p_special_item_head;    
        while(p_temp_item_list != 0)
        {
            if(p_temp_item_list->p_item != 0)
            {            
                if(MMI_RESULT_TRUE == MMIQBTHEME_ItemHandleMsg(p_temp_item_list->p_item, win_id,            
                    msg_id,  param ) )
                {
                    temp_result = MMI_RESULT_TRUE;  
                }
            }
            p_temp_item_list = p_temp_item_list->p_next_list_item;    
        }    
    }
    
    
    switch (msg_id)
    {        
    case MSG_OPEN_WINDOW: 
        {                      
        }
        break;
        
    case MSG_FULL_PAINT:
        {      
            DrawPage(p_page);
        }        
        break;
        
    case MSG_APP_RED:        
    case MSG_APP_CANCEL:
        //return to normal state
        if(p_page->fsm_state != MMIQBTHEME_NORMAL)
        {
            p_page->fsm_state = MMIQBTHEME_NORMAL; 
            if(p_page->type == MMIQBTHEME_PAGE_DOCKBAR_PAGE2)
            {
                //save dockbar page2 data
                MMIQBTHEME_SaveDockbarPage2Data(p_page);
            }
        }
        break;
        
    case MSG_GET_FOCUS:    
        if(p_page->type == MMIQBTHEME_PAGE_DOCKBAR_PAGE2)
        {
            //read dockbar page2 data
            MMIQBTHEME_InitDockbarPage2();
        }
        break;
        
    case MSG_LOSE_FOCUS:      
        if(p_page->fsm_state != MMIQBTHEME_NORMAL)
        {
            if(p_page->type == MMIQBTHEME_PAGE_DOCKBAR_PAGE2)
            {
                //save dockbar page2 data
                MMIQBTHEME_SaveDockbarPage2Data(p_page);
            }
        }
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_LONG:
        {
            GUI_POINT_T point = {0};            
            point.x = MMK_GET_TP_X(param);
            point.y = MMK_GET_TP_Y(param);  
            p_page->prevous_tp_point = point;
            p_page->tp_state = msg_id;  
            if(temp_result == MMI_RESULT_TRUE)
            {
                DrawPage(p_page);
            }
            result = temp_result;            
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_DOWN:
        {
            GUI_POINT_T point = {0};            
            point.x = MMK_GET_TP_X(param);
            point.y = MMK_GET_TP_Y(param);  
            p_page->prevous_tp_point = point;              
            p_page->tp_state = msg_id;
            result = temp_result;
        }      
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_MOVE:
        if(MMI_RESULT_TRUE == temp_result)  
        {              
            int32 moving_item_index =0;                
            int32 exchanging_item_index =0;
            GUI_POINT_T point = {0};
            
            point.x = MMK_GET_TP_X(param);
            point.y = MMK_GET_TP_Y(param);  
            
            //第一页的item不允许拖动
            if(p_page->type == MMIQBTHEME_PAGE_DOCKBAR_PAGE1)
            {
                result = MMI_RESULT_FALSE;
                break;
            }
            
            moving_item_index = FindMovingItem(p_page);
            if(moving_item_index == -1) 
            {
                break;
            }

            //move menu icon to widget win
            if(!GUI_PointIsInRect( point, p_page->display.layout_rect))
            {
                if(p_page->type == MMIQBTHEME_PAGE_DOCKBAR_PAGE2)
                {
                    MMIQBTHEME_MoveMenu2WidgetWin(p_page, moving_item_index , point);
                    break;
                }
            }
            exchanging_item_index = FindExchangeItemWithMovingItem(p_page,moving_item_index);
            if(exchanging_item_index == -1)
            {
                break;
            }
            
            PlayExchangingAnimation(p_page, moving_item_index, exchanging_item_index);       
           
            
            MMIQBTHEME_GridExchangeGridData(p_page->p_grid,p_page->p_item[moving_item_index]->grid_index,
                p_page->p_item[exchanging_item_index]->grid_index  );
            ExchangeItem( p_page, moving_item_index, exchanging_item_index);
            //if(p_page->type == MMIQBTHEME_PAGE_DOCKBAR_PAGE2)
            //{
                //save dockbar page2 data
            //    MMIQBTHEME_SaveDockbarPage2Data(p_page);
            //}
            //else
            if(p_page->type == MMIQBTHEME_PAGE_DOCKBAR_PAGE3)
            {
                MMIWIDGET_ExchangeWidgetPageData(MMIAPIQBIDLE_GetGridPtr(), moving_item_index, exchanging_item_index);
            }
            p_page->tp_state = msg_id;
            p_page->fsm_state = MMIQBTHEME_ITEM_MOVING;
            //index means widget page index
            DrawPage(p_page);  
        }
        else
        {            
            //item的显示范围是否超过了1页的范围            
            if(p_page->type != MMIQBTHEME_PAGE_DOCKBAR_PAGE3) 
            {
                result = MMI_RESULT_FALSE;    //向上传递,进行页滑动处理 
                break;
            }
            if(p_page->fsm_state == MMIQBTHEME_PAGE_SLIDING) 
            {
                result = MMI_RESULT_FALSE;    //向上传递,进行页滑动处理          
                break;
            }
            if(p_page->item_num < MMIQBTHEME_DOCKBAR_PAGE_MAX_DISPLAY_ITEM )
            {
                result = MMI_RESULT_FALSE;    //向上传递,进行页滑动处理   
                break;
            }
            if(MSG_TP_PRESS_MOVE != p_page->tp_state
                && MSG_TP_PRESS_DOWN != p_page->tp_state
                )
            {
                result = MMI_RESULT_FALSE;    //向上传递,进行页滑动处理   
                break;
            }         
            {
                GUI_POINT_T point = {0};
                point.x = MMK_GET_TP_X(param);
                point.y = MMK_GET_TP_Y(param);
                //进行页内滑动处理  

                if(!GUI_PointIsInRect( point, p_page->display.display_rect))
                {
                    break;
                }
                p_page->tp_state = msg_id;
                if(0 != SlidePageItem(p_page, point.x - p_page->prevous_tp_point.x, 0 ))
                {
                    result = MMI_RESULT_FALSE;    //向上传递,进行页滑动处理          
                }
                p_page->fsm_state = MMIQBTHEME_ITEM_MOVING;
                DrawPage(p_page);  
                p_page->prevous_tp_point = point;
            }
        }        
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_UP:          
        if(MMI_RESULT_TRUE == temp_result)  
        {        
            if(p_page->type == MMIQBTHEME_PAGE_DOCKBAR_PAGE2)
            {                 
                int32 i = 0;
                MMIQBTHEME_DISPLAY_T temp_display = {0};
                for(i = 0; i < p_page->item_num; i++)
                {
                    temp_display = p_page->p_item[i]->display;
                    temp_display.lcd_dev_info = p_page->display.lcd_dev_info;
                    MMIQBTHEME_ItemSetDisplayObject( p_page->p_item[i],&temp_display); 
                }
            }
        }                
        DrawPage(p_page );
        p_page->tp_state = msg_id;
        result = temp_result;
        break;       
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return(result);     
    
}                                           

//set状态机状态
PUBLIC int32  MMIQBTHEME_PageSetFSMState(MMIQBTHEME_PAGE_T* p_page,
                                         MMIQBTHEME_FSM_STATE_E      fsm_state
                                         )
{
    int32 i = 0;
    MMIQBTHEME_ITEM_LIST_T *p_special_item_head = 0; 
    
    if(p_page->type == MMIQBTHEME_PAGE_DOCKBAR_PAGE2)
    {            
        if(p_page->fsm_state != MMIQBTHEME_NORMAL
          && fsm_state == MMIQBTHEME_NORMAL)
        {
            //save dockbar page2 data
            MMIQBTHEME_SaveDockbarPage2Data(p_page);
        }
    }   
    
    p_page->fsm_state = fsm_state;
    p_special_item_head = p_page->p_special_item_head;
    //set item state
    for(i=0; i < p_page->item_num; i++)
    {
        MMIQBTHEME_ItemSetFSMState(p_page->p_item[i], fsm_state);
    }
    while(p_special_item_head != PNULL)
    {
        if(p_special_item_head->p_item != PNULL)
        {
            MMIQBTHEME_ItemSetFSMState(p_special_item_head->p_item, fsm_state);
        }
        p_special_item_head = p_special_item_head->p_next_list_item;
    }
    return 0;
}                                        

//get状态机状态
PUBLIC MMIQBTHEME_FSM_STATE_E  MMIQBTHEME_PageGetFSMState(MMIQBTHEME_PAGE_T* p_page )
{
    return p_page->fsm_state;
}


//set display 
PUBLIC int32  MMIQBTHEME_PageSetDisplayObject(MMIQBTHEME_PAGE_T* p_page,
                                              MMIQBTHEME_DISPLAY_T      *display
                                              )
{
/*
if(p_page == PNULL || display == PNULL)
{
return ;
}   
    */
    p_page->display = *display;    
    return 0;
}

PUBLIC int32  MMIQBTHEME_PageAddItem(MMIQBTHEME_PAGE_T* p_page,
                                     MMIQBTHEME_ITEM_T *p_item
                                     )
{
    int32 max_page_num = 0;
    MMIQBTHEME_DISPLAY_T    temp_display = {0};
    int32 grid_index = 0;
    GUI_RECT_T rect = {0};
    MMIQBTHEME_GRID_DATA_T grid_data = {0};

    if(p_page->type >= MMIQBTHEME_PAGE_DOCKBAR && p_page->type <= MMIQBTHEME_PAGE_DOCKBAR_PAGE3)
    {
        max_page_num = MMIQBTHEME_DOCKBAR_PAGE_MAX_ITEM;
    }
    else
    {
        max_page_num = MMIQBTHEME_WIDGET_PAGE_MAX_ITEM;
    }
    
    if(p_page->item_num >= max_page_num )
    {
        return -2;
    } 
    
    p_item->item_index = p_page->item_num;
    p_page->p_item[p_page->item_num]= p_item;
    p_page->item_num ++;      
    
    grid_data.type = p_item->type;
    grid_data.data.qbtheme_func_id = p_item->item_index;
    
    //分配空间
    MMIQBTHEME_GridGetFirstValidGrid(p_page->p_grid, &grid_index);       
    MMIQBTHEME_GridGetOneGridRect(p_page->p_grid,  grid_index, &rect);   
    p_item->grid_index = grid_index;
    MMIQBTHEME_GridSetData(p_page->p_grid ,grid_index,&grid_data);
    
    temp_display.win_id = p_page->display.win_id;
    temp_display.display_rect = rect;
    temp_display.layout_rect = rect;
    temp_display.lcd_dev_info = p_page->display.lcd_dev_info;

    if(p_item->type == MMIQBTHEME_ITEM_WIDGET_PAGE_PREVIEW_ICON)
    {        
        //MMIQBTHEME_CreateDisplayEx(&p_item->display);  
        temp_display.lcd_dev_info.block_id = UILAYER_NULL_HANDLE;
    }
    
    MMIQBTHEME_ItemSetDisplayObject( p_item,&temp_display); 

    return 0;
}                                 
//特殊处理的item
PUBLIC int32  MMIQBTHEME_PageAddSpecialItem(MMIQBTHEME_PAGE_T* p_page,
                                            MMIQBTHEME_ITEM_T *p_item
                                            )
{
    MMIQBTHEME_ITEM_LIST_T *p_temp_item_list = 0;
    if(0 == p_page->p_special_item_head)
    {
        p_page->p_special_item_head = (MMIQBTHEME_ITEM_LIST_T*)SCI_ALLOC_APP(sizeof(MMIQBTHEME_ITEM_LIST_T));
        SCI_MEMSET(p_page->p_special_item_head,0, sizeof(MMIQBTHEME_ITEM_LIST_T));
    }   
    p_temp_item_list = p_page->p_special_item_head;
    
    while(p_temp_item_list->p_item != 0)
    {
        if(p_temp_item_list->p_next_list_item == 0)
        {
            p_temp_item_list->p_next_list_item = (MMIQBTHEME_ITEM_LIST_T*)SCI_ALLOC_APP(sizeof(MMIQBTHEME_ITEM_LIST_T));
            SCI_MEMSET(p_temp_item_list->p_next_list_item,0, sizeof(MMIQBTHEME_ITEM_LIST_T));
        }        
        p_temp_item_list = p_temp_item_list->p_next_list_item;
        
    }
    p_temp_item_list->p_item = p_item;
    p_temp_item_list->p_next_list_item = 0;
    
    //分配空间
    p_item->item_index = -1;        
    p_item->grid_index = -1;     
    
    return 0;
}   

//delete one item in item array
PUBLIC int32  MMIQBTHEME_PageDeleteItem(MMIQBTHEME_PAGE_T* p_page,
                                        int32 item_index 
                                        )                                    
{
    int32 i = 0;
    MMIQBTHEME_GRID_T *p_grid = p_page->p_grid;
    GUI_RECT_T          rect = {0};
    MMIQBTHEME_GRID_DATA_T grid_data = {0};    
    int32 offset_x      = 0;
    int32 offset_y      = 0;
    
    if(p_page->type != MMIQBTHEME_PAGE_DOCKBAR_PAGE3
        && p_page->type != MMIQBTHEME_PAGE_DOCKBAR_PAGE2
        )
    {
        return -1;
    }
    
    //at least ,there is one item in page
    if(p_page->item_num == 1) 
    {
        return -2;
    }
    
    //获得ｐａｇｅ业内ｉｔｅｍ整体滑动的偏移值
    //page业内item数目少于6个，
    if(p_page->item_num - 1 >= MMIQBTHEME_DOCKBAR_PAGE_MAX_DISPLAY_ITEM)
    {
        for(i = 0;  i <  p_page->item_num; i++)
        {
            if(p_page->p_item[i]->fsm_state != MMIQBTHEME_ITEM_MOVING)
            {            
                MMIQBTHEME_GridGetOneGridRect(p_grid, i, &rect);        
                offset_x = p_page->p_item[i]->display.display_rect.left -  rect.left;
                offset_y = p_page->p_item[i]->display.display_rect.top -  rect.top;
                break;
            }
        }
    }
    if(i == p_page->item_num)
    {
        return -3;
    }    
    
    MMIQBTHEME_ItemDestroy(p_page->p_item[item_index]);  
    for(i = item_index; i < p_page->item_num - 1; i++ )
    {
        p_page->p_item[i] = p_page->p_item[i+1];
    }            
    p_page->p_item[p_page->item_num - 1] = 0;    
    p_page->item_num --;
    
    
    //page2 需要设定格子个数，计算区域
    if(p_page->type == MMIQBTHEME_PAGE_DOCKBAR_PAGE2)
    {
        MMIQBTHEME_GridSetRowCol( p_grid, 1,p_page->item_num);    
    }   
    
    for(i=0;i < p_page->item_num; i++)
    {   
        p_page->p_item[i]->item_index = i;
        p_page->p_item[i]->grid_index = i;
        p_page->p_item[i]->focus_flag = 0;
        grid_data.type = p_page->p_item[i]->type;
        grid_data.data = p_page->p_item[i]->id;
        MMIQBTHEME_GridSetData( p_grid, i,&grid_data);
        
        MMIQBTHEME_GridGetOneGridRect(p_grid, i, &rect);        
        
        rect.left += offset_x;
        rect.right += offset_x;
        rect.top += offset_y;
        rect.bottom += offset_y;
        
        MMIQBTHEME_ItemSetRect(p_page->p_item[i], rect);       
    }
    
    
    //set focus item
    //if the last item was deleted and it was foucsed , 
    //move focused item index to the previous one
    if(p_page->type == MMIQBTHEME_PAGE_DOCKBAR_PAGE3)
    {
        if(p_page->focused_item_index >= p_page->item_num)
        {
            p_page->focused_item_index = p_page->item_num - 1;
        }    
    }    
    return 0;
}
                                        
//insert one item in item array, and items after item_index will be moved to the followed position
PUBLIC int32  MMIQBTHEME_PageInsertItem(MMIQBTHEME_PAGE_T* p_page,
                                        MMIQBTHEME_ITEM_T *p_item,                                    
                                        int32  item_position
                                        )               
{
    int32 i = 0;
    MMIQBTHEME_GRID_T *p_grid = p_page->p_grid;
    GUI_RECT_T          rect = {0};
    MMIQBTHEME_GRID_DATA_T grid_data = {0};    
    MMIQBTHEME_DISPLAY_T    temp_display = {0};
    int32 offset_x      = 0;
    int32 offset_y      = 0;
    
    if(p_page->type != MMIQBTHEME_PAGE_DOCKBAR_PAGE3
        && p_page->type != MMIQBTHEME_PAGE_DOCKBAR_PAGE2
        )
    {
        return -1;
    }
    
    //check item num
    if(p_page->type == MMIQBTHEME_PAGE_DOCKBAR_PAGE3)
    {
        if(p_page->item_num >= MMIQBTHEME_DOCKBAR_PAGE_MAX_ITEM) 
        {
            return -2;
        }
    }    
    else if(p_page->type == MMIQBTHEME_PAGE_DOCKBAR_PAGE2)
    {
        if(p_page->item_num >= MMIQBTHEME_DOCKBAR_PAGE2_MAX_ITEM) 
        {
            return -2;
        }
    }
    
    //获得ｐａｇｅ业内ｉｔｅｍ整体滑动的偏移值
    //page页内item数目不少于6个，
    if(p_page->item_num - 1 >= MMIQBTHEME_DOCKBAR_PAGE_MAX_DISPLAY_ITEM)
    {
        for(i = 0;  i <  p_page->item_num; i++)
        {
            if(p_page->p_item[i]->fsm_state != MMIQBTHEME_ITEM_MOVING)
            {            
                MMIQBTHEME_GridGetOneGridRect(p_grid, i, &rect);        
                offset_x = p_page->p_item[i]->display.display_rect.left -  rect.left;
                offset_y = p_page->p_item[i]->display.display_rect.top -  rect.top;
                break;
            }
        }
    }
    if(i == p_page->item_num)
    {
        return -3;
    }    
    
    p_page->item_num++;
    for(i = p_page->item_num -1 ; i > item_position; i--)
    {
        p_page->p_item[i ] =  p_page->p_item[i - 1];
    }
    p_page->p_item[item_position] = p_item;
    p_item->fsm_state = p_page->fsm_state;
    
    if(p_page->type == MMIQBTHEME_PAGE_DOCKBAR_PAGE2)
    {
        MMIQBTHEME_GridSetRowCol( p_grid, 1,p_page->item_num);    
    }
    temp_display.win_id = p_page->display.win_id;
    
    for(i=0;i < p_page->item_num; i++)
    {       
        p_page->p_item[i]->item_index = i;
        p_page->p_item[i]->grid_index = i;
        p_page->p_item[i]->focus_flag = 0;
        
        grid_data.type = p_page->p_item[i]->type;
        grid_data.data = p_page->p_item[i]->id;
        MMIQBTHEME_GridSetData( p_grid, i,&grid_data);
        
        MMIQBTHEME_GridGetOneGridRect(p_grid, i, &rect);      
        rect.left += offset_x;
        rect.right += offset_x;
        rect.top += offset_y;
        rect.bottom += offset_y;
        temp_display.display_rect = rect;
        temp_display.layout_rect = rect;
        
        if(i == item_position)
        {            
            MMIQBTHEME_ItemSetDisplayObject( p_page->p_item[i ],&temp_display);
            if(p_item->type == MMIQBTHEME_ITEM_WIDGET_PAGE_PREVIEW_ICON)
            {        
                MMIQBTHEME_CreateDisplayEx(&p_item->display);       
            }
        }              

        MMIQBTHEME_ItemSetRect(p_page->p_item[i ], rect);       
    }
    return 0;
}

//delete all items in one page
PUBLIC int32  MMIQBTHEME_PageDeleteAllItems(MMIQBTHEME_PAGE_T* p_page
                                         )
{
    int32 i = 0;
    MMIQBTHEME_ITEM_LIST_T *p_special_item_head = 0; 
    p_special_item_head = p_page->p_special_item_head;
    
    //del item 
    for(i=0; i < p_page->item_num; i++)
    {
        MMIQBTHEME_ItemDestroy(p_page->p_item[i]);        
    }
    while(p_special_item_head != PNULL)
    {
        if(p_special_item_head->p_item != PNULL)
        {
            MMIQBTHEME_ItemDestroy(p_special_item_head->p_item);            
        }
        p_special_item_head = p_special_item_head->p_next_list_item;
    }
    p_page->item_num = 0;
    MMIQBTHEME_GridClearAllData(p_page->p_grid);
    return 0;
}                                        

//set focus item
PUBLIC int32  MMIQBTHEME_PageSetFocusItem(MMIQBTHEME_PAGE_T* p_page,
                                  int32  item_index
                                        )               
{
    // only dock bar page3 has focused item
    if(p_page->type != MMIQBTHEME_PAGE_DOCKBAR_PAGE3)
    {
        return -1;
    }
    //cancel previous focused item
    if(item_index >= p_page->item_num )
    {  
        return -2;
    }

    //CANCEL OLD ONE
    MMIQBTHEME_ItemSetFocus(p_page->p_item[p_page->focused_item_index], 0);
    //set new one
    p_page->focused_item_index = item_index;
    MMIQBTHEME_ItemSetFocus(p_page->p_item[p_page->focused_item_index], 1);
    //DrawPage(p_page);
    return 0;
}

//slide page to  focus item position
PUBLIC int32  MMIQBTHEME_PageSlide2FocusedItem(MMIQBTHEME_PAGE_T* p_page)
{
    GUI_RECT_T rect = {0};
    int32 offset_x = 0    ;
    int32 offset_y = 0    ;
    int32 grid_offset = 0;
    
    if(p_page->item_num < MMIQBTHEME_DOCKBAR_PAGE_MAX_DISPLAY_ITEM )
    {
        return 0;    //不需要处理焦点item的位置      
    }
    
    //判断边界
    MMIQBTHEME_ItemGetLayoutRect (p_page->p_item[p_page->focused_item_index],  &rect);
    
    if(rect.left >= p_page->display.display_rect.left
        && rect.right <= p_page->display.display_rect.right)
    {
        return 0;    //不需要处理焦点item的位置      
    }
    
    ResetPageItemInitialPosition(p_page);
    
    //focused item will display in the fifth grid area
    if(p_page->focused_item_index >= MMIQBTHEME_DOCKBAR_PAGE_MAX_DISPLAY_ITEM - 1)
    {
        grid_offset =  0 - (p_page->focused_item_index + 1 - (MMIQBTHEME_DOCKBAR_PAGE_MAX_DISPLAY_ITEM - 1));
        
        MMIQBTHEME_GridGetOneGridPosition(p_page->p_grid ,
            &offset_x,  &offset_y, grid_offset
            );
    }
    SlidePageItem(p_page,offset_x, 0 );
    //DrawPage(p_page);
    return 0;
}


PUBLIC int32 MMIQBTHEME_PageDraw(MMIQBTHEME_PAGE_T* p_page )
{
    DrawPage(p_page);
    
    return 0;
}

PUBLIC int32 MMIQBTHEME_PageSetRectByOffset(MMIQBTHEME_PAGE_T* p_page ,int32 offset_x, int32 offset_y)
{
    int32 i= 0;
    GUI_RECT_T rect = {0};
    MMIQBTHEME_ITEM_LIST_T *p_temp_item_list = 0;
    
    p_page->display.display_rect.left += offset_x;
    p_page->display.display_rect.right += offset_x;
    p_page->display.layout_rect.left += offset_x;
    p_page->display.layout_rect.right += offset_x;
    
    MMIQBTHEME_GridSetRect(p_page->p_grid,  p_page->display.layout_rect);
    for(i= 0; i < p_page->item_num; i++)
    {
        MMIQBTHEME_ItemGetLayoutRect(p_page->p_item[i], &rect);
        rect.left += offset_x;
        rect.right += offset_x;
        //set  rect
        MMIQBTHEME_ItemSetRect(p_page->p_item[i], rect);
    }
    
    p_temp_item_list = p_page->p_special_item_head;    
    while(p_temp_item_list != 0)
    {
        if(p_temp_item_list->p_item != 0)
        {            
            MMIQBTHEME_ItemGetLayoutRect(p_temp_item_list->p_item, &rect);
            rect.left += offset_x;
            rect.right += offset_x;
            //set  rect
            MMIQBTHEME_ItemSetRect(p_temp_item_list->p_item, rect);
        }
        p_temp_item_list = p_temp_item_list->p_next_list_item;    
    }
    return 0;
}

PUBLIC int32 MMIQBTHEME_PageSetRect(MMIQBTHEME_PAGE_T* p_page ,GUI_RECT_T   rect)
{
    int32 offset_x = {0};
    
    offset_x = rect.left - p_page->display.layout_rect.left;
    
    MMIQBTHEME_PageSetRectByOffset(p_page,offset_x ,0 );
    return 0;
}


LOCAL int32 DrawPage(MMIQBTHEME_PAGE_T* p_page )
{        
    int32 item_index =0;
    MMIQBTHEME_ITEM_LIST_T *p_temp_item_list = 0;
    GUI_LCD_DEV_INFO    lcd_dev_info = {0};
    BOOLEAN     is_compound = FALSE;
    
    if (UILAYER_IsLayerActive(&p_page->display.lcd_dev_info))
    {        
        lcd_dev_info = p_page->display.lcd_dev_info;
        UILAYER_ClearRect(&lcd_dev_info, p_page->display.layout_rect);
    }
    else
    {        
        lcd_dev_info =  *MMITHEME_GetDefaultLcdDev();      
        is_compound = TRUE;
    }
    if(p_page->type == MMIQBTHEME_PAGE_DOCKBAR_PAGE1 
        || p_page->type == MMIQBTHEME_PAGE_DOCKBAR_PAGE2
        || (p_page->type == MMIQBTHEME_PAGE_DOCKBAR_PAGE3 
            && p_page->fsm_state != MMIQBTHEME_PAGE_SLIDING))
    {
        for(item_index = 0 ; item_index < p_page->item_num  ; item_index++)
        {               
            //moving item will not be drawed on page layer            
            if(p_page->p_item[item_index]->fsm_state == MMIQBTHEME_ITEM_MOVING)
            {
                continue;
            } 
            MMIQBTHEME_ItemHandleMsg(p_page->p_item[item_index],
                p_page->p_item[item_index]->display.win_id,            
                MSG_FULL_PAINT,  0 );
        }        
    }
    
    //特殊处理的item
    p_temp_item_list = p_page->p_special_item_head;    
    while(p_temp_item_list != 0)
    {
        if(p_temp_item_list->p_item != 0)
        {            
            MMIQBTHEME_ItemHandleMsg(p_temp_item_list->p_item,
                p_temp_item_list->p_item->display.win_id,            
                MSG_FULL_PAINT,  0 );
            
            p_temp_item_list = p_temp_item_list->p_next_list_item;    
        }    
    }    
    if(p_page->type == MMIQBTHEME_PAGE_DOCKBAR_PAGE3)
    {
        for(item_index =0; item_index <p_page->item_num ; item_index++)
        {                
            //moving item will not be drawed on page layer            
            if(p_page->p_item[item_index]->fsm_state == MMIQBTHEME_ITEM_MOVING)
            {
                continue;
            } 
            UILAYER_BltLayerToLayer(&lcd_dev_info,
                &p_page->p_item[item_index]->display.lcd_dev_info,
                //&p_page->p_item[i]->display.display_rect, 
                &p_page->display.display_rect, 
                is_compound);        
        }   
        
        if(p_page->focused_item_index < p_page->item_num)
        {    
            //draw focus frame
            DrawRectFrame(&lcd_dev_info, 
            &p_page->display.display_rect,
            &p_page->p_item[p_page->focused_item_index]->display.display_rect,
              MMI_YELLOW_COLOR);
        }
    }
    return 0;
}

//find item in moving state, return item index
LOCAL int32 FindMovingItem(MMIQBTHEME_PAGE_T* p_page)
{
    int32 i=0;
    for(i=0; i < p_page->item_num; i++)
    {
        if(p_page->p_item[i]->fsm_state == MMIQBTHEME_ITEM_MOVING)
        {
            return i;   //found
        }
    }
    return -1; //not found
}

//find item to be exchanged with moving state, return item index
LOCAL int32 FindExchangeItemWithMovingItem(MMIQBTHEME_PAGE_T* p_page,int32 moving_item_index)
{
    MMIQBTHEME_ITEM_T *p_moving_item = p_page->p_item[moving_item_index];
    GUI_RECT_T moving_rect = {0};
    GUI_RECT_T old_rect = {0};
    GUI_RECT_T find_rect = {0};
    GUI_RECT_T dest_rect = {0};
    int32 i = 0;
    
    MMIQBTHEME_ItemGetDisplayRect(p_moving_item, &moving_rect);
    MMIQBTHEME_ItemGetLayoutRect(p_moving_item, &old_rect);    
    
    if(moving_rect.left < old_rect.left)    //moving to left
    {
        if(moving_item_index == 0)  //first item 
        {
            return -1;
        }
        for(i= moving_item_index - 1; i >=0; i--)
        {
            MMIQBTHEME_ItemGetLayoutRect(p_page->p_item[i],&find_rect);
            if(moving_rect.left < find_rect.left)
            {
                if(p_page->p_item[i]->type != MMIQBTHEME_ITEM_NULL 
                    && GUI_IntersectRect(&dest_rect, find_rect, p_page->display.display_rect)
                    )
                {
                    return  i;
                }
            }
        }
        
    }
    else if(moving_rect.right > old_rect.right) //moving to right
    {
        if(moving_item_index == p_page->item_num - 1)   //last item
        {
            return -1;
        }
        for(i= moving_item_index + 1; i < p_page->item_num; i++)
        {
            MMIQBTHEME_ItemGetLayoutRect(p_page->p_item[i],&find_rect);
            if(moving_rect.right > find_rect.right)
            {
                if(p_page->p_item[i]->type != MMIQBTHEME_ITEM_NULL
                    && GUI_IntersectRect(&dest_rect, find_rect, p_page->display.display_rect)
                    )           
                {
                    return  i;
                }
            }
        }   
    }
    
    return -1;
    
}

LOCAL int32 PlayExchangingAnimation(MMIQBTHEME_PAGE_T* p_page,
    int32   moving_item_index,
    int32   exchanging_item_index
    )
{    
    GUI_RECT_T start_rect = {0};
    GUI_RECT_T end_rect = {0};
    int32 frame = 1;
    int32 step_x = 10;
    int32 step_y = 10;
    int32 frame_index = 0;
    
    MMIQBTHEME_ItemGetLayoutRect(p_page->p_item[exchanging_item_index], &start_rect);    
    MMIQBTHEME_ItemGetLayoutRect(p_page->p_item[moving_item_index], &end_rect);
    MMIQBTHEME_CalculateMovingParameter( &step_x , &step_y, &frame  ,
        start_rect,  end_rect
        );    
    
    for(frame_index= 1; frame_index <= frame; frame_index++)
    {        
        MMIQBTHEME_ItemMove(p_page->p_item[exchanging_item_index],
            step_x, step_y);   
        
        UILAYER_ClearRect(&p_page->display.lcd_dev_info,p_page->display.display_rect);
        DrawPage(p_page );     
        MMITHEME_UpdateRect();
        SCI_SLEEP(MMIQBTHEME_SLIDING_DELEY_TICKS);
    }
    
    MMIQBTHEME_ItemSetDisplayRect(p_page->p_item[exchanging_item_index], end_rect);      
    //DrawPage(p_page );     
    //MMITHEME_UpdateRect();
    return 0;
}        


//exchange item
LOCAL int32 ExchangeItem(MMIQBTHEME_PAGE_T* p_page,
    int32   old_item_id,
    int32   new_item_id
    )
{
    MMIQBTHEME_ITEM_T temp_item = {0};
    MMIQBTHEME_ITEM_T *p_temp_item = {0};
    
    //CANCEL OLD ONE
    MMIQBTHEME_ItemSetFocus(p_page->p_item[p_page->focused_item_index], 0);
    
    SCI_MEMCPY(&temp_item, p_page->p_item[new_item_id],sizeof(MMIQBTHEME_ITEM_T));
    
    //交换位置信息
    p_page->p_item[new_item_id]->item_index = p_page->p_item[old_item_id]->item_index;
    
    p_page->p_item[new_item_id]->grid_index = p_page->p_item[old_item_id]->grid_index;
    
    p_page->p_item[new_item_id]->display.layout_rect = p_page->p_item[old_item_id]->display.layout_rect;
    
    p_page->p_item[old_item_id]->item_index = temp_item.item_index;
    
    p_page->p_item[old_item_id]->grid_index = temp_item.grid_index;
    
    p_page->p_item[old_item_id]->display.layout_rect = temp_item.display.layout_rect;

//    if(p_page->p_item[old_item_id]->focus_flag == 1)
//    {
//        p_page->focused_item_index = p_page->item_num;
//     }

    //交换指针
    p_temp_item = p_page->p_item[old_item_id];
    p_page->p_item[old_item_id] = p_page->p_item[new_item_id];
    p_page->p_item[new_item_id] = p_temp_item;    

    MMIQBTHEME_PageSetFocusItem(p_page, p_page->focused_item_index);
    return 0;
}




//在一页内对item进行滑动
LOCAL int32 SlidePageItem(MMIQBTHEME_PAGE_T* p_page,int32 offset_x, int32 offset_y)                       
{
    int32 i= 0;
    GUI_RECT_T rect = {0};
    int32 result = 0;
    
    //判断左边界
    MMIQBTHEME_ItemGetLayoutRect (p_page->p_item[0],  &rect);
    if(rect.left + offset_x > p_page->display.display_rect.left)
    {
        offset_x = p_page->display.display_rect.left - rect.left;
        result = -1;        
    }
    
    //判断右边界
    MMIQBTHEME_ItemGetLayoutRect (p_page->p_item[p_page->item_num - 1],  &rect);
    if(rect.right + offset_x < p_page->display.display_rect.right)
    {
        offset_x = p_page->display.display_rect.right - rect.right;
        result = -2;        
    }
    
    //滑动
    for(i=0; i<p_page->item_num ; i++)
    {  
        MMIQBTHEME_ItemGetLayoutRect (p_page->p_item[i],  &rect);
        rect.left += offset_x;
        rect.right += offset_x;
        rect.top += offset_y;
        rect.bottom += offset_y;
        //set  rect
        MMIQBTHEME_ItemSetRect(p_page->p_item[i], rect);    
    }        
    return result;
}

//reset item initial position
LOCAL int32 ResetPageItemInitialPosition(MMIQBTHEME_PAGE_T* p_page)                       
{
    int32 i= 0;
    GUI_RECT_T rect = {0};
    
    for(i=0; i < p_page->item_num; i++)
    {    
        MMIQBTHEME_GridGetOneGridRect(p_page->p_grid,  i, &rect);   
        //set  rect
        MMIQBTHEME_ItemSetRect(p_page->p_item[i], rect);    
    }
    return 0;
}

//draw rect in buffer
LOCAL void DrawRectFrame( 
                         GUI_LCD_DEV_INFO  *dev_info_ptr,  // block information
                         GUI_RECT_T                 *buffer_rect,
                         GUI_RECT_T                 *display_rect,      //rectangle to draw
                         GUI_COLOR_T                color           //color to draw
                         )
{
    GUI_RECT_T rect = {0};
    GUI_POINT_T point1 = {0};
    GUI_POINT_T point2 = {0};
    
   if(! GUI_IntersectRect(&rect, *buffer_rect, *display_rect))
   {
        return ;
   }
   //draw focus frame  

   point1.x = display_rect->left;
   point1.y = display_rect->top;   
   point2.x = display_rect->right;
   point2.y = display_rect->top;
   if(GUI_PointIsInRect(point1, *buffer_rect) || GUI_PointIsInRect(point2, *buffer_rect))
   {
        LCD_DrawHLine(dev_info_ptr, rect.left, rect.top, rect.right, color);
   }
   
   point1.x = display_rect->left;
   point1.y = display_rect->bottom;   
   point2.x = display_rect->right;
   point2.y = display_rect->bottom;
   if(GUI_PointIsInRect(point1, *buffer_rect) || GUI_PointIsInRect(point2, *buffer_rect))
   {
        LCD_DrawHLine(dev_info_ptr, rect.left, rect.bottom, rect.right, color);
   }
   
   point1.x = display_rect->left;
   point1.y = display_rect->top;   
   point2.x = display_rect->left;
   point2.y = display_rect->bottom;
   if(GUI_PointIsInRect(point1, *buffer_rect) || GUI_PointIsInRect(point2, *buffer_rect))
   {
        LCD_DrawVLine(dev_info_ptr, rect.left, rect.top, rect.bottom, color);
   }
   
   point1.x = display_rect->right;
   point1.y = display_rect->top;   
   point2.x = display_rect->right;
   point2.y = display_rect->bottom;
   if(GUI_PointIsInRect(point1, *buffer_rect) || GUI_PointIsInRect(point2, *buffer_rect))
   {
        LCD_DrawVLine(dev_info_ptr, rect.right, rect.top, rect.bottom, color);
   }
}




/*Edit by script, ignore 4 case. Thu Apr 26 19:01:11 2012*/ //IGNORE9527
