/*****************************************************************************
** File Name:      MMI_wintab.c                                       *
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
#ifdef EBOOK_SUPPORT
#include "mmk_kbd.h"
#include "guicommon.h"
#include "mmi_theme.h"
#include "mmi_nv.h"
#include "mmi_modu_main.h"
#include "mmiset_export.h"
#include "mmk_type.h"
#include "mmk_app.h"
#include "guilcd.h"
#include "window_parse.h"
#include "guimenu.h"
#include "mmitheme_menu.h"
#include "mmiset_wintab.h"
#include "mmi_mainmenu_export.h"
#include "mmigrid.h"
#include "mmiebook_bookshelf.h"

LOCAL int32 AddPointTrack(MMIGRID_T * p_grid, GUI_POINT_T point)
{
    int32 i = 0;
    if(p_grid->track.point_num == p_grid->track.max_num)
    {  
        for(i = 1; i < p_grid->track.max_num; i++)
        {
            p_grid->track.point[i - 1] = p_grid->track.point[i];
            p_grid->track.time[i - 1] = p_grid->track.time[i];
        }
        p_grid->track.point_num--;
    }    
    p_grid->track.point[p_grid->track.point_num] = point;
    p_grid->track.time[p_grid->track.point_num] = SCI_GetTickCount();
    p_grid->track.point_num++;
    return p_grid->track.point_num;
}

LOCAL int32 ResetPointTrack(MMIGRID_T * p_grid)
{    
    SCI_MEMSET(&p_grid->track, 0 , sizeof(p_grid->track));
    p_grid->track.max_num = sizeof(p_grid->track.time)/sizeof(p_grid->track.time[0]);
    return 0;
}

LOCAL int32 CalculateYAccelerationVelocity(MMIGRID_T * p_grid)
{
    int32 time = p_grid->track.time[p_grid->track.point_num - 1] - p_grid->track.time[0];
    p_grid->track.velocity = 0;
    p_grid->track.acceleration  =  0;    
    
    if(p_grid->track.point_num <= 1)
    {        
        return -1;
    }
    else if(0 == time)
    {
        return -2;
    }
    else
    {
        p_grid->track.velocity = 2*1000*( p_grid->track.point[p_grid->track.point_num - 1].y 
            - p_grid->track.point[0].y) / time;
        p_grid->track.acceleration  =    p_grid->track.velocity*1000/ time;           
        
        return 0;
    }
}

LOCAL int32 CalculateDistance(MMIGRID_T * p_grid, int32 sample_time, int32 step)
{
    int32 time = sample_time*step;
    int32 acceleration = - p_grid->track.acceleration/MMIGRID_SLIDE_ACCELERATION_DAMP;
    int32 velocity = p_grid->track.velocity + acceleration*time/1000;
    if(p_grid->track.point_num <=1)
    {        
        return 0;
    }
    else if(p_grid->track.velocity*velocity <= 0)    //velocity decreasing to zero or negetive
    {   
        return 0;
    }
    else
    {  
        return p_grid->track.velocity*time/1000 + acceleration*time*time/1000/1000/2;
    }
}

//用于代码在内存中画图
LOCAL int32 SetDisplayBuffer2LogicalRect(MMIGRID_DISPLAY_T  *display)
{
#ifdef UI_MULTILAYER_SUPPORT
    //reset layer position
    UILAYER_SetLayerPosition(&display->lcd_dev_info,
        display->display_rect.left, display->display_rect.top);
#endif
    return 0;
}

//用于将内存buffer送到lcd buffer显示
LOCAL int32 SetDisplayBuffer2PhsicalRect(MMIGRID_DISPLAY_T  *display)
{
#ifdef UI_MULTILAYER_SUPPORT
    //reset layer position
    UILAYER_SetLayerPosition(&display->lcd_dev_info,
        display->layout_rect.left, display->layout_rect.top);
#endif
    return 0;
}

//将屏幕上的坐标点转换为逻辑上的坐标点
LOCAL GUI_POINT_T ConvertPhisicalPoint2Logical(MMIGRID_T * p_grid, GUI_POINT_T point)
{
    GUI_POINT_T temp_point = {0};
    temp_point = point;
    temp_point.x += p_grid->display.display_rect.left - p_grid->display.layout_rect.left;    
    temp_point.y += p_grid->display.display_rect.top - p_grid->display.layout_rect.top;
    return  temp_point;
}

//将逻辑的坐标转换为屏幕上的坐标
LOCAL GUI_RECT_T ConvertLogicalRect2Phisical(MMIGRID_T * p_grid, GUI_RECT_T *p_logical_rect)
{
    GUI_RECT_T temp_rect = *p_logical_rect;
    int32 offset_y = 0;
    int32 display_rect_height  = p_grid->display.display_rect.bottom - p_grid->display.display_rect.top + 1;
    
    offset_y = (p_logical_rect->top - p_grid->display.display_rect.top) % display_rect_height;
    temp_rect.top = p_grid->display.layout_rect.top + offset_y;
    temp_rect.bottom = temp_rect.top + (p_logical_rect->bottom - p_logical_rect->top );
    return temp_rect;
}

LOCAL GUI_RECT_T GetOneGridRect(MMIGRID_T * p_grid, int32 grid_index)
{
    int32 row = 0;
    int32 col = 0;
    GUI_RECT_T rect = {0};
    row = grid_index / p_grid->col;
    col = grid_index%p_grid->col;    
    rect.top = (p_grid->rect.top  + p_grid->grid_y_interval*(row*2+1) + p_grid->grid_height*row);    
    rect.left = (p_grid->rect.left + p_grid->grid_x_interval*(col*2 + 1) + p_grid->grid_width*col);
    rect.bottom = rect.top + p_grid->grid_height - 1;    
    rect.right = rect.left  + p_grid->grid_width -1;    
    return rect;
}

LOCAL int32 GetPointLocation(MMIGRID_T * p_grid, GUI_POINT_T point)
{
    GUI_RECT_T rect = {0};
    int32 temp_index = 0;
    
    //效率低，有更好的算法
    for(temp_index = 0; temp_index < p_grid->row * p_grid->col; temp_index++)
    {
        rect = GetOneGridRect(p_grid, temp_index);
        if(GUI_PointIsInRect( point,  rect))
        {
            return temp_index;
        }
    }
    //not in any grid area
    return -1;
}

LOCAL int32 DrawOneGrid(MMIGRID_T *p_grid, int32 grid_index)
{
    GUI_RECT_T  rect = {0};   
    rect = MMIGRID_GetOneGridRect(p_grid,  grid_index);    
    return p_grid->display.draw(grid_index, &rect, &p_grid->display.lcd_dev_info);
}

LOCAL int32 DrawAllGrid(MMIGRID_T *p_grid)
{
    int32 i = 0;
    GUI_POINT_T point = {0};  
    GUI_RECT_T  rect1 = {0};  
    GUI_RECT_T  rect4 = {0};  
    GUI_RECT_T  rect = {0};  
    int32 start_index = 0;
    int16 per_screen=0;
    if(!MMITHEME_IsMainScreenLandscape ())
    {
        per_screen=MMIEBOOK_BOOKSHELF_LAYER_PER_SCREEN_V;
    }
    else
    {
        per_screen=MMIEBOOK_BOOKSHELF_LAYER_PER_SCREEN_H;
    }
#ifdef UI_MULTILAYER_SUPPORT    
    UILAYER_Clear(&p_grid->display.lcd_dev_info);
#endif
    //显示区域与layout区域不重合时，通常是item数目发生了变化，减少
    //更改了layout区域，却没有改显示区域。
    //因而如果检测到这种情况，display rect重设一下
    if(!GUI_RectIsCovered( p_grid->display.layout_rect,p_grid->display.display_rect))
    {
        MMIGRID_MoveDisplayRect( &p_grid->display, 0 ,-per_screen*p_grid->grid_height);
    }
    
    point.x = p_grid->display.display_rect.left;
    point.y = p_grid->display.display_rect.top;
    
    start_index = GetPointLocation( p_grid,  point);
    if(start_index < 0)
    {
        return -1;
    }
    for(i = 0; i < per_screen; i++)
    {
        DrawOneGrid(p_grid, start_index + i);
    }
    //check area when draw grid 4, in case area are overlapped
    rect1 = MMIGRID_GetOneGridRect(p_grid,  start_index);
    rect4 = MMIGRID_GetOneGridRect(p_grid,  start_index + i);    
    if(GUI_IntersectRect(&rect, rect1, rect4))
    {
        return -2;
    }
    
    DrawOneGrid(p_grid, start_index + i);
    return 0;
}

LOCAL int32 CreateSlideTimer(MMIGRID_T *p_grid)
{
    if(p_grid->slide.slide_timer_id != 0)
    {
        MMK_StopTimer(p_grid->slide.slide_timer_id);
    }
    p_grid->slide.slide_timer_id = MMK_CreateWinTimer( p_grid->display.win_id, MMIGRID_SLIDE_TIME_INTERVAL,  FALSE);  
    
    return p_grid->slide.slide_timer_id;
}

LOCAL int32 DestroySlideTimer(MMIGRID_T * p_grid)
{
    int32 result = 0;
    result = MMK_StopTimer(p_grid->slide.slide_timer_id);
    p_grid->slide.slide_timer_id = 0;
    return result;
}

PUBLIC GUI_RECT_T MMIGRID_GetOneGridRect(MMIGRID_T * p_grid, int32 grid_index)
{
    GUI_RECT_T  logical_rect = {0};    
    GUI_RECT_T  physical_rect = {0};
    logical_rect = GetOneGridRect(p_grid, grid_index);
    physical_rect = ConvertLogicalRect2Phisical(p_grid, &logical_rect);
    return physical_rect;
}

PUBLIC int32 MMIGRID_GetPointLocation(MMIGRID_T * p_grid, GUI_POINT_T point)
{
    GUI_POINT_T logical_point = {0};
    logical_point = ConvertPhisicalPoint2Logical(p_grid, point);
    return GetPointLocation(p_grid, logical_point);    
}


PUBLIC int32 MMIGRID_SetData(MMIGRID_T * p_grid,int32 grid_index, MMIGRID_DATA_T *p_data)
{    
    if(p_grid->p_grid_data[grid_index].type == 0)
    {
        p_grid->p_grid_data[grid_index] = *p_data;
    }
    
    return 0;
}

PUBLIC int32 MMIGRID_ClearData(MMIGRID_T * p_grid,int32 grid_index)
{       
    SCI_MEMSET(&p_grid->p_grid_data[grid_index], 0, sizeof(MMIGRID_DATA_T));
    p_grid->p_grid_data[grid_index].type = 0;
    
    return 0;
}

PUBLIC int32 MMIGRID_ClearAllData(MMIGRID_T * p_grid)
{       
    int32 grid_num = p_grid->row*p_grid->col;
    SCI_MEMSET(&p_grid->p_grid_data, 0, sizeof(MMIGRID_DATA_T)*(uint32)grid_num);    
    return 0;
}

//get  offset of grids that their index offset = grid_offset
PUBLIC int32  MMIGRID_GetOneGridPosition(MMIGRID_T* p_grid ,
                                         int32*   offset_x,   int32*   offset_y,
                                         int32  grid_offset
                                         )
{
    if(offset_x == 0 || offset_y == 0)
    {
        return -1;
    }
    *offset_x = (p_grid->grid_width +  p_grid->grid_x_interval*2)* grid_offset ;
    *offset_y = (p_grid->grid_height +  p_grid->grid_y_interval*2)* grid_offset ;
    return 0;
}      

//窗口处理函数,只处理tp msg
PUBLIC MMI_RESULT_E MMIGRID_HandleGridMsg(MMIGRID_T *p_grid,
                                          MMI_WIN_ID_T      win_id,
                                          MMI_MESSAGE_ID_E  msg_id,
                                          DPARAM            param
                                          )
{
    MMI_RESULT_E  result = MMI_RESULT_TRUE;
    
    switch (msg_id)
    {          
    case MSG_FULL_PAINT:
        
        if(p_grid->slide.slide_state != MMK_TP_SLIDE_NONE)
        {
            break;
        }
        DrawAllGrid(p_grid);            
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_DOWN:
        //case MSG_TP_PRESS_LONG:
        {
            GUI_POINT_T point = {0};            
            point.x = MMK_GET_TP_X(param);
            point.y = MMK_GET_TP_Y(param);  
            p_grid->slide.prevous_tp_point = point;
            p_grid->slide.tp_state = msg_id;
            DestroySlideTimer(p_grid);
            ResetPointTrack(p_grid);
            AddPointTrack(p_grid, point);
            if( p_grid->slide.slide_state != MMK_TP_SLIDE_FLING)
            {
                p_grid->slide.slide_state = MMK_TP_SLIDE_NONE;
            }

        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_MOVE:            
        {
            GUI_POINT_T physical_point = {0};  
            int32 offset_x = 0;
            int32 offset_y = 0;
            
            physical_point.x = MMK_GET_TP_X(param);
            physical_point.y = MMK_GET_TP_Y(param);  
            
            offset_x = physical_point.x - p_grid->slide.prevous_tp_point.x;
            offset_y = physical_point.y - p_grid->slide.prevous_tp_point.y;
            AddPointTrack(p_grid, physical_point);    
            
            if((p_grid->slide.allowed_slide_direction & MMI_SLIDE_DIRECTION_X)  /*lint !e655 */
                != MMI_SLIDE_DIRECTION_X)
            {
                offset_x = 0;
                
            }
            if((p_grid->slide.allowed_slide_direction & MMI_SLIDE_DIRECTION_Y)  /*lint !e655 */
                != MMI_SLIDE_DIRECTION_Y)
            {
                offset_y = 0;
            }
            if(0 != offset_x || 0 != offset_y)
            {
                MMIGRID_MoveDisplayRect(&p_grid->display, offset_x, offset_y);  
                DrawAllGrid(p_grid);
                p_grid->slide.slide_state = MMK_TP_SLIDE_TP_SCROLL;
            }
            
            p_grid->slide.prevous_tp_point = physical_point;
            p_grid->slide.tp_state = msg_id;
        }        
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_UP:          
        {
            GUI_POINT_T physical_point = {0};                  
            int32 offset_x = 0;
            int32 offset_y = 0;
            
            physical_point.x = MMK_GET_TP_X(param);
            physical_point.y = MMK_GET_TP_Y(param);  
            //AddPointTrack(p_grid, physical_point);
            
            offset_x = physical_point.x - p_grid->slide.prevous_tp_point.x;
            offset_y = physical_point.y - p_grid->slide.prevous_tp_point.y;
            
            p_grid->slide.prevous_tp_point = physical_point;
            p_grid->slide.tp_state = msg_id;
            if(p_grid->slide.slide_state == MMK_TP_SLIDE_TP_SCROLL)
            {                             
                CreateSlideTimer(p_grid);
                p_grid->slide.slide_state = MMK_TP_SLIDE_FLING;
                CalculateYAccelerationVelocity(p_grid);
            }
            else
            {
                p_grid->slide.slide_state = MMK_TP_SLIDE_NONE;
            }
        }
        break;       
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        
    case MSG_TIMER:    
        if (p_grid->slide.slide_timer_id !=  *( GUI_TIMER_ID_T*)param)
        {
            break;
        }
        {   
            int32 offset1 = 0;
            int32 offset2 = 0;
            int32 offset = 0;
            p_grid->track.step++;
            offset1 = CalculateDistance(p_grid,MMIGRID_SLIDE_TIME_INTERVAL, p_grid->track.step) ; 
            offset2 = CalculateDistance(p_grid,MMIGRID_SLIDE_TIME_INTERVAL, p_grid->track.step - 1);
            //滑动完成
            if(0 == offset1)
            {
                DestroySlideTimer(p_grid);
                p_grid->slide.slide_state = MMK_TP_SLIDE_NONE;
                break;
            }
            offset = offset1 - offset2;
            if(0 == MMIGRID_MoveDisplayRect(&p_grid->display, 0, offset))
            {
                CreateSlideTimer(p_grid);
            }
            else
            {
                DestroySlideTimer(p_grid);
                p_grid->slide.slide_state = MMK_TP_SLIDE_NONE;
            }
            DrawAllGrid(p_grid);
        }
        break;   
        
    case MSG_LOSE_FOCUS:    
    case MSG_CLOSE_WINDOW:    
        DestroySlideTimer(p_grid);
        p_grid->slide.slide_state = MMK_TP_SLIDE_NONE;
        break;

    case MSG_APP_UP:    
        MMIGRID_MoveDisplayRect(&p_grid->display, 0, p_grid->grid_height);        
        DrawAllGrid(p_grid);            
        break;
        
    case MSG_APP_DOWN: 
        MMIGRID_MoveDisplayRect(&p_grid->display, 0, -p_grid->grid_height);        
        DrawAllGrid(p_grid);            
        break;

    case MSG_APP_LEFT: 
    case MSG_APP_RIGHT: 
        DrawAllGrid(p_grid);            
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
        }
        
        return(result);     
        
}                                             

PUBLIC int32 MMIGRID_CreateDisplayLayer( MMIGRID_DISPLAY_T  *display )
{
#ifdef UI_MULTILAYER_SUPPORT
    GUI_LCD_DEV_INFO widget_layer_handle = {0};
    int16 width = 0;
    int16 height = 0;
    UILAYER_CREATE_T create_info = {0};  
    GUI_RECT_T rect = display->display_rect;    
    
    
    if (UILAYER_HANDLE_MULTI != UILAYER_GetHandleType(&display->lcd_dev_info))    
    {
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
        
        UILAYER_SetLayerColorKey(&widget_layer_handle, TRUE, UILAYER_TRANSPARENT_COLOR);
        display->lcd_dev_info  = widget_layer_handle;
    }
    
    if (UILAYER_IsLayerActive(&widget_layer_handle))
    {
        UILAYER_Clear(&widget_layer_handle);
        return 0;
    }
    else
#endif
    {
        return -1;
    }
}

PUBLIC int32 MMIGRID_DestroyDisplayLayer( MMIGRID_DISPLAY_T  *display)
{
#ifdef UI_MULTILAYER_SUPPORT
    UILAYER_ReleaseLayer(&display->lcd_dev_info);   
#endif
    return 0;
}

PUBLIC int32 MMIGRID_AppendDisplayLayer( MMIGRID_DISPLAY_T  *display)
{
#ifdef UI_MULTILAYER_SUPPORT
    if (UILAYER_IsBltLayer(&display->lcd_dev_info))
    {
        return -1;        
    } 
    if (!UILAYER_IsLayerActive(&display->lcd_dev_info))
    {
        return -2;        
    }  
    {
        UILAYER_APPEND_BLT_T append_layer = {0};
        append_layer.lcd_dev_info = display->lcd_dev_info;
        append_layer.layer_level = UILAYER_LEVEL_NORMAL;
        UILAYER_AppendBltLayer(&append_layer);   
        return 0;
    }
#else
    return 0;
#endif
}

PUBLIC int32 MMIGRID_RemoveDisplay( MMIGRID_DISPLAY_T  *display)
{
#ifdef UI_MULTILAYER_SUPPORT
    UILAYER_RemoveBltLayer(&display->lcd_dev_info); 
#endif    
    return 0;
}

PUBLIC int32 MMIGRID_MoveDisplayRect( MMIGRID_DISPLAY_T  *display, int32 offset_x, int32 offset_y)
{
    int32 result  = 0; 
    if(0 != offset_x)
    {        
        if(display->display_rect.left + offset_x <= display->layout_rect.left)
        {
            offset_x = display->layout_rect.left - display->display_rect.left;
            result = -1;
        }
        else if(display->display_rect.right + offset_x >= display->layout_rect.right)                    
        {
            offset_x = display->layout_rect.right - display->display_rect.right;            
            result = -2;
        }   
    }
    if(0 != offset_y)
    {
        if(display->display_rect.top - offset_y <= display->layout_rect.top)
        {
            offset_y = - display->layout_rect.top + display->display_rect.top;            
            result = -3;
        }
        if(display->display_rect.bottom - offset_y >= display->layout_rect.bottom)                    
        {
            offset_y = -display->layout_rect.bottom + display->display_rect.bottom;            
            result = -4;
        }
    }
    display->display_rect.left -= offset_x;
    display->display_rect.right -= offset_x;
    display->display_rect.top -=  offset_y;
    display->display_rect.bottom -= offset_y;
    
    return result;
}

PUBLIC int32 MMIGRID_SetDisplayDisplayRect( MMIGRID_DISPLAY_T  *display, GUI_RECT_T *rect)
{
    display->display_rect = *rect;
    return 0;
}

PUBLIC int32 MMIGRID_SetDisplayLayoutRect( MMIGRID_DISPLAY_T  *display, GUI_RECT_T *rect)
{
    display->layout_rect = *rect;
    return 0;
}
#endif


/*Edit by script, ignore 3 case. Thu Apr 26 19:00:53 2012*/ //IGNORE9527
