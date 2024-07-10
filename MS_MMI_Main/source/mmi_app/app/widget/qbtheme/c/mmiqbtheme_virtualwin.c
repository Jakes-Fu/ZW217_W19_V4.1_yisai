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
#include "mmi_modu_main.h"
#include "guires.h"
#include "mmiset_export.h"
#include "mmk_type.h"
#include "mmk_app.h"
#include "mmk_kbd.h"
#include "guilcd.h"
#include "window_parse.h"
#include "guimenu.h"
#include "mmitheme_menu.h"
#include "mmitheme_mainmenu.h"

#include "mmiwidget_export.h"
#include "mmi_mainmenu_export.h"

#include "mmi_menutable.h"

#include "mmiqbtheme_config.h"
#include "mmiqbtheme_internal.h"
#include "mmiqbtheme_item.h"
#include "mmiqbtheme_grid.h"
#include "mmiqbtheme_page.h"
#include "mmiqbtheme_virtualwin.h"
#include "mmiqbtheme_dockbar.h"

#include "mmiqbtheme_idlewin.h"
#include "mmiqbtheme_idleexport.h"
//input msg 
//lost focus   QUIT moving/sliding state
//cancal_key all_state is setted back to normal
//all state count in 4, so row = 4
//tp_down tp_up  tp_long tp_move count in 4,so col = 4
//the only deffient is array[0][3] with item_state_tranfer_array
const  MMIQBTHEME_FSM_STATE_E page_state_tranfer_array[4][4]= {
    MMIQBTHEME_NORMAL, MMIQBTHEME_NORMAL, MMIQBTHEME_NORMAL, MMIQBTHEME_EDITING,// MMIQBTHEME_NORMAL    
        MMIQBTHEME_EDITING, MMIQBTHEME_EDITING, MMIQBTHEME_PAGE_SLIDING, MMIQBTHEME_EDITING, //MMIQBTHEME_EDITING
        MMIQBTHEME_PAGE_SLIDING, MMIQBTHEME_EDITING, MMIQBTHEME_PAGE_SLIDING, MMIQBTHEME_PAGE_SLIDING,//MMIQBTHEME_PAGE_SLIDING
        MMIQBTHEME_ITEM_MOVING, MMIQBTHEME_EDITING, MMIQBTHEME_ITEM_MOVING, MMIQBTHEME_ITEM_MOVING,//MMIQBTHEME_ITEM_MOVING
};

LOCAL int32 SlidePage(MMIQBTHEME_VIRTUALWIN_T* p_virtualwin, int32 offset_x);
LOCAL int32 DrawWallPaper(  MMIQBTHEME_VIRTUALWIN_T* p_virtualwin);
LOCAL int32 DrawTurnOverIcon(  MMIQBTHEME_VIRTUALWIN_T* p_virtualwin);
LOCAL int32 DrawVirtualWin(  MMIQBTHEME_VIRTUALWIN_T* p_virtualwin);
LOCAL int32 SetVirtualWinState(  MMIQBTHEME_VIRTUALWIN_T* p_virtualwin,
                                MMIQBTHEME_FSM_STATE_E      fsm_state
                                );

//create an virtual win objcet 
//申请内存,赋初始值
PUBLIC int32 MMIQBTHEME_VirtualWinCreate(MMIQBTHEME_VIRTUALWIN_T** p_virtualwin,
                                         MMIQBTHEME_VIRTUALWIN_TYPE_E type     //widget/ dockbar
                                         )
{
    MMIQBTHEME_VIRTUALWIN_T * p_temp_virtualwin = 0;
    
    if(p_virtualwin == PNULL)
    {
        return -1;
    }
    
    p_temp_virtualwin = (MMIQBTHEME_VIRTUALWIN_T *)SCI_ALLOCA(sizeof(MMIQBTHEME_VIRTUALWIN_T));
    SCI_MEMSET(p_temp_virtualwin, 0, sizeof(MMIQBTHEME_VIRTUALWIN_T));
    
    p_temp_virtualwin->type = type;
    * p_virtualwin = p_temp_virtualwin;
    
    return 0;
}                                            


//destroy an virtual win objcet
PUBLIC int32 MMIQBTHEME_VirtualWinDestroy(MMIQBTHEME_VIRTUALWIN_T* p_virtualwin)
{
    int32 i = 0;
    for(i = 0; i< p_virtualwin->page_num; i++)
    {
        MMIQBTHEME_PageDestroy(p_virtualwin->p_page[i]);
    }
    MMIQBTHEME_DestroyDisplay(&p_virtualwin->display);
    SCI_FREE(p_virtualwin);
    return 0;
}

//窗口处理函数
PUBLIC MMI_RESULT_E MMIQBTHEME_VirtualWinHandleMsg(MMIQBTHEME_VIRTUALWIN_T* p_virtualwin,
                                                   MMI_WIN_ID_T      win_id,
                                                   MMI_MESSAGE_ID_E  msg_id,
                                                   DPARAM            param
                                                   )
{
    MMI_RESULT_E  result = MMI_RESULT_TRUE;
    MMI_RESULT_E  temp_result = MMI_RESULT_FALSE;
    
    //TP 
    if(MSG_TP_PRESS_DOWN <= msg_id && msg_id <= MSG_TP_GESTURE )
    {
        if(MMIQBTHEME_PAGE_SLIDING != p_virtualwin->fsm_state)
            //|| MMIQBTHEME_EDITING == p_virtualwin->fsm_state)
        {
            temp_result = MMIQBTHEME_PageHandleMsg( p_virtualwin->p_page[p_virtualwin->current_page],
                win_id,  msg_id,param);
        }
    }     
    else if(MSG_FULL_PAINT != msg_id)
    {
        int32 i= 0;
        for(i=0; i< p_virtualwin->page_num; i++)
        {
            if(MMIQBTHEME_PageHandleMsg( p_virtualwin->p_page[i], win_id,  msg_id,param))
            {
                temp_result = MMI_RESULT_TRUE;
            }
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
            DrawWallPaper(p_virtualwin);
            DrawTurnOverIcon(p_virtualwin);    
         if(MMIQBTHEME_EDITING == p_virtualwin->fsm_state)
         {
             int32 i= 0;
             for(i=0; i< p_virtualwin->page_num; i++)
             {
                 if(MMIQBTHEME_PageHandleMsg( p_virtualwin->p_page[i], win_id,  msg_id,param))
                 {
                     temp_result = MMI_RESULT_TRUE;
                 }
             }
        }
        else if(MMIQBTHEME_PAGE_SLIDING != p_virtualwin->fsm_state)
        {
            temp_result = MMIQBTHEME_PageHandleMsg( p_virtualwin->p_page[p_virtualwin->current_page],
                win_id,  msg_id,param);
        }
        }
        //DrawVirtualWin(p_virtualwin);
        break;
        
    case MSG_APP_RED:        
    case MSG_APP_CANCEL:
        //return to normal state
        p_virtualwin->fsm_state = MMIQBTHEME_NORMAL;            
        //set current page       
        MMIQBTHEME_VirtualWinSetCurrentPage( p_virtualwin,1);        
        DrawVirtualWin(p_virtualwin);        
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_DOWN:        
        //if(MMI_RESULT_TRUE == temp_result)break;
        p_virtualwin->tp_state = msg_id;
        p_virtualwin->prevous_tp_point.x = MMK_GET_TP_X(param);
        p_virtualwin->prevous_tp_point.y = MMK_GET_TP_Y(param);
        result =  temp_result;
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_LONG:
        {            
            //set state            
            if(MMI_RESULT_TRUE == temp_result)
            {
               if( p_virtualwin->fsm_state ==  MMIQBTHEME_NORMAL)
               {  
                   MMIQBTHEME_VirtualWinSetFSMState(p_virtualwin, MMIQBTHEME_EDITING);
                   MMIQBIDLE_SetWidgetWinMode(MMIAPIQBIDLE_GetGridPtr(),MMIGRID_RUN_MODE_EDITING);
                   //refresh
                   MMIQBTHEME_VirtualWinHandleMsg( p_virtualwin,  p_virtualwin->display.win_id, 
                                        MSG_FULL_PAINT , 0);
                    MMIQBIDLE_UpdateAllScreen();
               }  
            }       
            p_virtualwin->tp_state = msg_id;       
            p_virtualwin->prevous_tp_point.x = MMK_GET_TP_X(param);
            p_virtualwin->prevous_tp_point.y = MMK_GET_TP_Y(param);
            result = temp_result;
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_MOVE:    
        {           
            GUI_POINT_T tp_point = {0};   
        
            tp_point.x = p_virtualwin->prevous_tp_point.x;
            tp_point.y = p_virtualwin->prevous_tp_point.y;
            
            p_virtualwin->prevous_tp_point.x  = MMK_GET_TP_X(param) ;
            p_virtualwin->prevous_tp_point.y  = MMK_GET_TP_Y(param) ;      
            if(MMI_RESULT_TRUE == temp_result)
            {      
                p_virtualwin->fsm_state = MMIQBTHEME_ITEM_MOVING;
                break;            
            }
            if(MSG_TP_PRESS_DOWN != p_virtualwin->tp_state
                && MSG_TP_PRESS_MOVE != p_virtualwin->tp_state
                )
            {                
                result = MMI_RESULT_FALSE;
                break;
            }
            if(!GUI_PointIsInRect( p_virtualwin->prevous_tp_point, p_virtualwin->display.display_rect))
            {
                result = MMI_RESULT_FALSE;
                break;
            }    
            p_virtualwin->tp_state = msg_id;  
            {
                int32 offset =0 ;  
                int32 i = 0;
                offset = p_virtualwin->prevous_tp_point.x - tp_point.x ;
                /*   if (abs(offset) < 10)
                {
                break;
                }           
                */   
                if(p_virtualwin->fsm_state != MMIQBTHEME_NORMAL)
                {                    
                    p_virtualwin->fsm_state =  MMIQBTHEME_PAGE_SLIDING;
                    for(i=0; i < p_virtualwin->page_num ; i++)
                    {
                        MMIQBTHEME_PageSetFSMState(p_virtualwin->p_page[i] ,MMIQBTHEME_PAGE_SLIDING);
                    }
                }
                if(p_virtualwin->fsm_state ==  MMIQBTHEME_PAGE_SLIDING)
                {                             
                    UILAYER_SetDirectDraw( TRUE );
                    SlidePage( p_virtualwin,  offset);    
                    result = MMI_RESULT_TRUE;
                }          
            }
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_UP:
        if(MMI_RESULT_FALSE == temp_result)
        {     
            if(p_virtualwin->fsm_state ==  MMIQBTHEME_PAGE_SLIDING)
            {
                GUI_RECT_T start_rect = {0};
                GUI_RECT_T end_rect = {0};
                int32 offset = 0;
                int32 page_changed = 0;
                int32 page_width = 0;
                //moving 的后续滑动处理                      
                int32 step_x = 20;  //10 pixels
                int32 step_y = 10;
                int32 frame = 0;
                int32 frame_index = 0;
                int32 i = 0;
                int32 temp_current_page = 0;
                start_rect = p_virtualwin->p_page[p_virtualwin->current_page]->display.layout_rect;                 
                end_rect = start_rect;
                page_width = start_rect.right - start_rect.left + 1;
                offset = start_rect.left ;
                if(abs(offset) > page_width/4)
                {
                    if(offset > 0)
                    {
                        page_changed =  1;
                    }
                    else
                    {
                        page_changed =  -1;
                    }
                }
                else
                {
                    page_changed =  0;
                }
                end_rect.left = page_width*page_changed;
                end_rect.right = page_width*page_changed + page_width - 1;                
                
                MMIQBTHEME_CalculateMovingParameter( &step_x , &step_y, &frame  ,
                    start_rect, end_rect                                    
                    );
                
                for(frame_index= 1; frame_index <= frame; frame_index++)
                {        
                    UILAYER_SetDirectDraw( TRUE );
                    SlidePage(p_virtualwin,step_x);
                    SCI_SLEEP(MMIQBTHEME_SLIDING_DELEY_TICKS);                   
                    MMITHEME_UpdateRect();
                }   
                if(p_virtualwin->current_page - page_changed < 0)
                {
                    temp_current_page = 0;
                }
                else if(p_virtualwin->current_page - page_changed >= p_virtualwin->page_num)
                {
                    temp_current_page = p_virtualwin->page_num - 1;
                }
                else
                {
                    temp_current_page =  p_virtualwin->current_page - page_changed;
                }                
                MMIQBTHEME_VirtualWinSetCurrentPage( p_virtualwin,temp_current_page);
                DrawVirtualWin(p_virtualwin);
                MMITHEME_UpdateRect();
                p_virtualwin->fsm_state =  MMIQBTHEME_EDITING;
                for(i=0; i < p_virtualwin->page_num ; i++)
                {                    
                    MMIQBTHEME_PageSetFSMState(p_virtualwin->p_page[i] ,MMIQBTHEME_EDITING);
                }
            }
            else
            {
                 result = MMI_RESULT_FALSE;
            }
        }
        p_virtualwin->tp_state = msg_id;       
        break;       
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        
        
    case MSG_LOSE_FOCUS:       
        if(p_virtualwin->fsm_state ==  MMIQBTHEME_PAGE_SLIDING)
        {
            MMIQBTHEME_VirtualWinSetCurrentPage( p_virtualwin,p_virtualwin->current_page);
        }
        if(MMIQBTHEME_NORMAL != p_virtualwin->fsm_state)
        {
            SetVirtualWinState(p_virtualwin, MMIQBTHEME_EDITING);
        }
        break;
        
    case MSG_GET_FOCUS:        
        //MMIQBTHEME_AppendDisplay(&p_virtualwin->display);
        //MMIQBTHEME_InitDockbarPage2();
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
        }
        
        return(result);     
        
}

//add one page in page array tail position
PUBLIC int32  MMIQBTHEME_VirtualWinAddPage(MMIQBTHEME_VIRTUALWIN_T* p_virtualwin,
                                           MMIQBTHEME_PAGE_T* p_page
                                           )
{
    int32 max_page_num = 0;
    int32 offset = 0;
    MMIQBTHEME_DISPLAY_T    temp_display = {0};
    int32 page_width = 0;
    
    if(p_virtualwin == PNULL)
    {
        return -1;
    }
    if(p_page == PNULL)
    {
        return -2;
    }
    if(p_virtualwin->type == MMIQBTHEME_VIRTUALWIN_DOCKBAR)
    {
        max_page_num = MMIQBTHEME_MAX_DOCKBAR_PAGE_NUM;
    }
    else
    {
        max_page_num = MMIQBTHEME_MAX_WIDGET_PAGE_NUM;
    }
    
    if(p_virtualwin->page_num >= max_page_num )
    {
        return -3;
    } 
    
    p_page->page_index = p_virtualwin->page_num;
    p_virtualwin->p_page[p_virtualwin->page_num]= p_page;
    p_virtualwin->page_num ++;
    
    temp_display = p_virtualwin->display;
    
    page_width = p_virtualwin->display.display_rect.right 
        - p_virtualwin->display.display_rect.left + 1;
    
    offset = p_page->page_index - p_virtualwin->current_page;
    
    temp_display.display_rect.left += offset*page_width;
    temp_display.display_rect.right += offset*page_width;
    temp_display.layout_rect.left += offset*page_width;
    temp_display.layout_rect.right += offset*page_width;
    
    p_page->p_grid->rect = temp_display.layout_rect;
    
    MMIQBTHEME_PageSetDisplayObject( p_page,&temp_display); 
    /*
    if(1 == p_virtualwin->page_num)
    {
    MMIQBTHEME_VirtualWinSetCurrentPage(p_virtualwin, 0);                                            
    }
    */
    return 0;
}                                              

//set状态机状态
PUBLIC int32  MMIQBTHEME_VirtualWinSetFSMState(MMIQBTHEME_VIRTUALWIN_T* p_virtualwin,
                                              MMIQBTHEME_FSM_STATE_E      fsm_state
                                              )
{
    return SetVirtualWinState(p_virtualwin, fsm_state);
}

//get状态机状态
PUBLIC MMIQBTHEME_FSM_STATE_E  MMIQBTHEME_VirtualWinGetFSMState(
                                MMIQBTHEME_VIRTUALWIN_T* p_virtualwin
                                              )
{
    return p_virtualwin->fsm_state;
}

//get  page num
PUBLIC int32  MMIQBTHEME_VirtualWinGetPageNum(MMIQBTHEME_VIRTUALWIN_T* p_virtualwin
                                              )
{
    return p_virtualwin->page_num;
}

//get display and layout rect
PUBLIC void  MMIQBTHEME_VirtualWinGetRect(MMIQBTHEME_VIRTUALWIN_T* p_virtualwin,
                                          GUI_RECT_T *rect
                                          )
{
    *rect = p_virtualwin->display.display_rect;
}                                              

//set display object
PUBLIC int32  MMIQBTHEME_VirtualWinSetDisplayObject(MMIQBTHEME_VIRTUALWIN_T* p_virtualwin,                                            
                                                    MMIQBTHEME_DISPLAY_T     *display
                                                    )
{
    if(p_virtualwin == PNULL)
    {
        return -1;
    }
    if(display == PNULL)
    {
        return -2;
    }
    
    p_virtualwin->display = *display;       
    //create display object    
    MMIQBTHEME_CreateDisplay(&p_virtualwin->display);  
    
    return 0;
}

//set current page
PUBLIC int32  MMIQBTHEME_VirtualWinSetCurrentPage(MMIQBTHEME_VIRTUALWIN_T* p_virtualwin,
                                                  int32 page_index)
{
    int32 i = 0;
    int32 page_width = 0;
    GUI_RECT_T rect = {0};

    rect = p_virtualwin->display.display_rect;
    page_width = rect.right - rect.left + 1;    
    p_virtualwin->current_page = page_index;
    
    //调整page的坐标    
    for(i=0; i < p_virtualwin->page_num; i++)
    {
        rect = p_virtualwin->display.display_rect;
        rect.left += page_width*(i - p_virtualwin->current_page);        
        rect.right += page_width*(i - p_virtualwin->current_page);
        MMIQBTHEME_PageSetRect( p_virtualwin->p_page[i] , rect );
    }
    return 0;
} 

//refresh virtual window
PUBLIC int32 MMIQBTHEME_VirtualWinDraw(MMIQBTHEME_VIRTUALWIN_T* p_virtualwin)
{    
    DrawWallPaper(p_virtualwin);
    DrawTurnOverIcon(p_virtualwin);    
    return DrawVirtualWin(p_virtualwin);
}

//refresh virtual window
LOCAL int32 DrawVirtualWin(MMIQBTHEME_VIRTUALWIN_T* p_virtualwin)
{    
    DrawWallPaper(p_virtualwin);
    DrawTurnOverIcon(p_virtualwin);    
    return MMIQBTHEME_PageDraw(p_virtualwin->p_page[p_virtualwin->current_page]);
}

//slide page in virtual win
LOCAL int32 SlidePage(MMIQBTHEME_VIRTUALWIN_T* p_virtualwin, int32 offset_x)
{
    int32 result = 0;
    int32 i = 0;
    
    //判断左边界
    if(p_virtualwin->p_page[0]->display.layout_rect.left + offset_x >= 
        p_virtualwin->display.layout_rect.left)
    {
        offset_x = p_virtualwin->display.layout_rect.left 
            - p_virtualwin->p_page[0]->display.layout_rect.left;
        result = -1;        
        
    }
    
    //判断右边界
    
    if(p_virtualwin->p_page[p_virtualwin->page_num - 1]->display.layout_rect.right 
        + offset_x <= p_virtualwin->display.layout_rect.right)
    {
        offset_x = p_virtualwin->display.layout_rect.right
            - p_virtualwin->p_page[p_virtualwin->page_num - 1]->display.layout_rect.right;
        result = -2;        
    }
    
    if(offset_x == 0) return -3;
    
    UILAYER_ClearRect(&p_virtualwin->display.lcd_dev_info,p_virtualwin->display.display_rect);
        
    for(i=0; i < p_virtualwin->page_num ; i++)
    {        
        MMIQBTHEME_PageSetRectByOffset( p_virtualwin->p_page[i] , offset_x, 0 );
        MMIQBTHEME_PageDraw(p_virtualwin->p_page[i] );        
    }

    result = 0;
    return result;
}

//draw dockbar wallpaper, using widget page backgroudn layer
LOCAL int32 DrawWallPaper(  MMIQBTHEME_VIRTUALWIN_T* p_virtualwin)
{   
    //画图操作在单独的墙纸层上
    
    MMI_IDLE_GRID_DATA_T        *idle_grid_ptr = MMIAPIQBIDLE_GetGridPtr();
    GUI_LCD_DEV_INFO    lcd_dev_info = {0};
    GUI_RECT_T  rect = p_virtualwin->display.display_rect;
    uint16 width = 0;
    uint16 height = 0;
    MMI_IMAGE_ID_T img_id = IMAGE_THEME_DESKTOP_DOCKBAR;
    
    if (UILAYER_IsLayerActive(&idle_grid_ptr->background_layer))
    {        
        lcd_dev_info = idle_grid_ptr->background_layer;
        UILAYER_ClearRect(&lcd_dev_info, rect);
    }
    else
    {        
        lcd_dev_info =  *MMITHEME_GetDefaultLcdDev();           
    }    
    
    GUIRES_GetImgWidthHeight(&width, &height, img_id, p_virtualwin->display.win_id);
    rect.top = rect.bottom - height + 1;
    
    GUIRES_DisplayImg(PNULL, 
        &rect, 
        PNULL, 
        p_virtualwin->display.win_id,
        img_id, 
        &lcd_dev_info); 
    return 0;
}

//draw arrow icon of turning left/right , using widget page backgroudn layer
LOCAL int32 DrawTurnOverIcon(  MMIQBTHEME_VIRTUALWIN_T* p_virtualwin)
{
    uint16 width = 0;
    uint16 height = 0;
    GUI_POINT_T lefticon_start_point = {0};
    GUI_POINT_T righticon_start_point = {0};
    MMI_IMAGE_ID_T lefticon = IMAGE_THEME_ARROW_LEFT; 
    MMI_IMAGE_ID_T righticon = IMAGE_THEME_ARROW_RIGHT; 
    GUI_RECT_T page_rect = p_virtualwin->display.layout_rect;
    MMI_IDLE_GRID_DATA_T        *idle_grid_ptr = MMIAPIQBIDLE_GetGridPtr();
    GUI_LCD_DEV_INFO    lcd_dev_info = {0};

    if(p_virtualwin->fsm_state == MMIQBTHEME_NORMAL)
    {
        return -1;
    }

    if (UILAYER_IsLayerActive(&idle_grid_ptr->background_layer))
    {        
        lcd_dev_info = idle_grid_ptr->background_layer;
        //UILAYER_ClearRect(&lcd_dev_info, p_virtualwin->display.display_rect);
    }
    else
    {        
        lcd_dev_info =  *MMITHEME_GetDefaultLcdDev();           
    }       
    
    GUIRES_GetImgWidthHeight(&width, &height, lefticon, p_virtualwin->display.win_id);
    lefticon_start_point.y = (page_rect.bottom + page_rect.top  - height)/2;
    righticon_start_point.y = lefticon_start_point.y;
    lefticon_start_point.x = page_rect.left ;
    righticon_start_point.x = page_rect.right - width + 1;    

    
    //画图操作在单独的墙纸层上
    if(p_virtualwin->current_page == 0) //first page 
    {
        GUIRES_DisplayImg(&righticon_start_point, 
            PNULL, 
            PNULL, 
            p_virtualwin->display.win_id,
            righticon, 
            &lcd_dev_info); 
        
    }
    else if(p_virtualwin->current_page == MMIQBTHEME_MAX_DOCKBAR_PAGE_NUM - 1) //last page 
    {
        GUIRES_DisplayImg(&lefticon_start_point, 
            PNULL, 
            PNULL, 
            p_virtualwin->display.win_id,
            lefticon, 
            &lcd_dev_info); 
    }
    else
    {
        GUIRES_DisplayImg(&lefticon_start_point, 
            PNULL, 
            PNULL, 
            p_virtualwin->display.win_id,
            lefticon,
            &lcd_dev_info); 
        
        GUIRES_DisplayImg(&righticon_start_point, 
            PNULL, 
            PNULL, 
            p_virtualwin->display.win_id,
            righticon, 
            &lcd_dev_info); 
    }    
    return 0;
}

//set virtual win state
LOCAL int32 SetVirtualWinState(  MMIQBTHEME_VIRTUALWIN_T* p_virtualwin,
                                MMIQBTHEME_FSM_STATE_E      fsm_state
                                )
{
    int32 i= 0;
    p_virtualwin->fsm_state = fsm_state;
    
    //set page state
    for(i=0; i < p_virtualwin->page_num; i++)
    {
        MMIQBTHEME_PageSetFSMState(p_virtualwin->p_page[i], fsm_state);
    }
    return 0;
}

/*Edit by script, ignore 4 case. Thu Apr 26 19:01:11 2012*/ //IGNORE9527
