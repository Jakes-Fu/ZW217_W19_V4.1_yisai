/*****************************************************************************
** File Name:      mmiwidget_special_effect.c                                        *
** Author:                                                                   *
** Date:           06/26/2012                                                 *
** Copyright:      2004 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe widget  special effect interface                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE                  NAME             DESCRIPTION                        *
** 06/26/2012         ying.xu              Creat
******************************************************************************/
#if defined MMIWIDGET_SPECIAL_EFFECT

#include "sci_types.h"
#include "os_api.h"
#include "img_ref_types.h"
#include "graphics_3d_rotate.h"
#include "mmk_msg.h"
#include "mmk_type.h"
#include "mmk_timer.h"
#include "mmi_appmsg.h"
#include "mmi_utility.h"
#include "mmiwidget.h"
#include "mmiwidget_special_effect.h"
#include "mmiwidget_internal.h "


LOCAL BOOLEAN CreateResource(MMI_WIN_ID_T win_id,MMIWIDGET_SPECIAL_EFFECT_T *p_widget_se)
{        
    if( p_widget_se->resource_state < 0)
    {     	    
   		MMIWIDGET_DEBUG(LOG_STR("p_widget_se =%x, state = %d,start time = %d"),  
            p_widget_se, p_widget_se->resource_state, SCI_GetTickCount());  
        if(0 != p_widget_se->CreateResourceBuffer(win_id, p_widget_se))
        {    
            MMIWIDGET_DEBUG(LOG_STR("CreateResourceBuffer failed"));   
            p_widget_se->DestroyResourceBuffer(win_id, p_widget_se);
            return FALSE;
        }
        p_widget_se->resource_state = 1;
        MMIWIDGET_DEBUG(LOG_STR(" end  time = %d"),  SCI_GetTickCount());	
    } 	
    return TRUE;
}

//widget 特效的窗口处理函数
PUBLIC MMI_RESULT_E  MMIWIDGET_HandleWidgetSpecialEffectWinMsg(
    MMIWIDGET_SPECIAL_EFFECT_T *p_widget_se,
    MMI_WIN_ID_T        win_id,        //IN:
    MMI_MESSAGE_ID_E    msg_id,        //IN:
    DPARAM            param        //IN:
)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
	
    switch (msg_id)
    {
    	case MSG_OPEN_WINDOW: 	
        case MSG_LOSE_FOCUS:        
        MMIWIDGET_ResetWidgetSpecialEffectState(win_id, p_widget_se);    
            break;
  
        case MSG_WIDGET_PAGE_SLIDE_START:
            MMIWIDGET_StopWidgetSpecialEffectDisplayTimer(p_widget_se);
            break;

        case MSG_WIDGET_PAGE_SLIDE_END:
            MMIWIDGET_DEBUG(LOG_STR("MSG_WIDGET_PAGE_SLIDE_END p_widget_se = %x,\
                     focus_flag = %d, time = %d,"), 
                    p_widget_se, p_widget_se->focus_flag, SCI_GetTickCount());
            if(1 == p_widget_se->focus_flag)
            {
                MMIWIDGET_StartWidgetSpecialEffectDisplayTimer( win_id,  
                        MMIWIDGET_SPECIAL_EFFECT_FRAME_INTERVAL, p_widget_se);
            }   
            break;
            
        case MSG_GET_FOCUS: 
            p_widget_se->focus_flag = 1;
        break;
      
        case MSG_FULL_PAINT:            
            MMIWIDGET_DEBUG(LOG_STR("MSG_FULL_PAINT p_widget_se =%x frame = %d, time = %d"), 
                           p_widget_se,p_widget_se->current_frame, SCI_GetTickCount());
                           
            if(-1 == p_widget_se->current_frame && 0 != p_widget_se->focus_flag)
            {
                MMIWIDGET_StartWidgetSpecialEffectDisplayTimer( win_id,  
                        MMIWIDGET_SPECIAL_EFFECT_FRAME_INTERVAL, p_widget_se);
            } 
		 
            if(!CreateResource(win_id, p_widget_se))
            {
                break;
            }
            p_widget_se->Display(win_id, p_widget_se);       
            //save memory, release resource when not focused
            if(0 == p_widget_se->focus_flag)
            {
                MMIWIDGET_ResetWidgetSpecialEffectState(win_id, p_widget_se);    
            }
            
	        MMIWIDGET_DEBUG(LOG_STR("MSG_FULL_PAINT Display time = %d"),  SCI_GetTickCount());	
			
            break;

        case MSG_TIMER:
	     MMIWIDGET_DEBUG(LOG_STR("MSG_TIMER p_widget_se =%x frame = %d, start time = %d"), 
	                    p_widget_se, p_widget_se->current_frame, SCI_GetTickCount());	
	                    
         if(0 == p_widget_se->focus_flag)
         {
             break;
         }
            if(p_widget_se->display_se_timer_id == *(uint8*)param)
            {
              MMIWIDGET_StartWidgetSpecialEffectDisplayTimer( win_id,  
                  MMIWIDGET_SPECIAL_EFFECT_FRAME_INTERVAL, p_widget_se); 

                if(0 != MMIWIDGET_DisplaySEForTimer(win_id, p_widget_se))
                {                    
                    MMIWIDGET_StopWidgetSpecialEffectDisplayTimer(p_widget_se);
                }                
         }
	     MMIWIDGET_DEBUG(LOG_STR("MSG_TIMER end time = %d"),  SCI_GetTickCount());
        break;

        default:
        break;
            
    }
    return recode;
}

//widget 特效显示定时器启动
PUBLIC int32 MMIWIDGET_StartWidgetSpecialEffectDisplayTimer(MMI_WIN_ID_T  win_id, uint32 time_interval,
                                                     MMIWIDGET_SPECIAL_EFFECT_T *p_widget_se)
{
    if(0 != p_widget_se->display_se_timer_id)
    {
        MMK_StopTimer(p_widget_se->display_se_timer_id);
    }    
    p_widget_se->display_se_timer_id = MMK_CreateWinTimer(win_id, time_interval, FALSE);
    return p_widget_se->display_se_timer_id;
}

//widget 特效显示定时器停止
PUBLIC int32 MMIWIDGET_StopWidgetSpecialEffectDisplayTimer(MMIWIDGET_SPECIAL_EFFECT_T *p_widget_se)
{
    if( MMK_StopTimer(p_widget_se->display_se_timer_id))
    {
        p_widget_se->display_se_timer_id = 0;
    }
    return (0 - p_widget_se->display_se_timer_id);
}

//widget reset特效的关键状态，定制定时器
PUBLIC int32 MMIWIDGET_ResetWidgetSpecialEffectState( MMI_WIN_ID_T  win_id,  MMIWIDGET_SPECIAL_EFFECT_T *p_widget_se)
{
    MMIWIDGET_DEBUG(LOG_STR("MSG_TIMER p_widget_se =%x, frame = %d,  resource_state= %d, focus_flag = %d,time = %d"), 
                   p_widget_se, p_widget_se->current_frame, p_widget_se->resource_state, p_widget_se->focus_flag, SCI_GetTickCount());   
    p_widget_se->current_frame = -1;    
    p_widget_se->resource_state = -1;
    p_widget_se->focus_flag = 0;
    MMIWIDGET_StopWidgetSpecialEffectDisplayTimer(p_widget_se);   
    p_widget_se->DestroyResourceBuffer(win_id, p_widget_se);
    return 0;
}

PUBLIC int32  MMIWIDGET_DisplaySEForTimer( 
                                    MMI_WIN_ID_T  win_id,  
                                    MMIWIDGET_SPECIAL_EFFECT_T *p_widget_se
                                    )
{
    GUI_RECT_T  scr_rect    = MMITHEME_GetFullScreenRect();
    GUI_RECT_T win_rect = {0};
    GUI_RECT_T rect = {0};
    
    
    if(!MMK_IsFocusWin(win_id) && !MMK_IsFocusWin(MMK_GetParentWinHandle(win_id))) 
    {
        return -1;
    }

#if defined MMI_GRID_IDLE_SUPPORT
    if(MMISET_IDLE_STYLE_GRID == MMIAPISET_GetIdleStyle())
    {
        if(MMIGRID_IsForbidPaint(0))
        {                
            return -2;
        }
    }
#endif
        
#if defined QBTHEME_SUPPORT
    if(MMISET_IDLE_QBTHEME == MMIAPISET_GetIdleStyle())
    {
        if(MMIQBIDLE_IsForbidPaint(0))
        {                
            return -3;
        }
    }
#endif
  
    MMK_GetWinRect(win_id, &win_rect);
    
    if(!GUI_IntersectRect(&rect, win_rect, scr_rect))
    {            
        return -4;
    }
    if(!CreateResource(win_id, p_widget_se))
    {
        return -5;
    } 
    p_widget_se->Display(win_id, p_widget_se);  
    return 0;
}

//create picture resource
PUBLIC uint32* MMIWIDGET_CreatePictureResource( MMI_WIN_ID_T  win_id,  MMI_IMAGE_ID_T image_id)
{
    uint16 image_width = 0;
    uint16 image_height = 0;
    UILAYER_CREATE_T create_info = {0};
    GUI_LCD_DEV_INFO layer_handle = {0};
    uint32* p_buffer = 0;
    uint8*      p_layer_buffer = 0;
    uint32      buffer_size = 0;
    MMI_WIN_ID_T parent_id = MAIN_IDLE_WIN_ID;  //MMK_GetParentWinHandle(win_id);
    GUI_RECT_T  rect = {0};

    GUIRES_GetImgWidthHeight(&image_width, &image_height, image_id,win_id);    
    
    //create layer    
    create_info.lcd_id          = GUI_MAIN_LCD_ID;
    create_info.owner_handle    = parent_id;
    create_info.offset_x        = 0; 
    create_info.offset_y        = 0;
    create_info.width           = image_width;
    create_info.height          = image_height;
    create_info.is_bg_layer     = FALSE;
    create_info.is_static_layer = FALSE;
    
    UILAYER_CreateLayer(&create_info, &layer_handle);
    p_layer_buffer = UILAYER_GetLayerBufferPtr(&layer_handle);
    if(0 == p_layer_buffer)
    {        
        UILAYER_RELEASELAYER(&layer_handle);
        return 0;
    }
    
    buffer_size = image_width*image_height*4 ;
    p_buffer = SCI_ALLOC_APP(buffer_size);
    if(0 == p_buffer)
    {
        //release layer        
        UILAYER_RELEASELAYER(&layer_handle);
        return 0;
    }
    
    //draw image
    UILAYER_Clear(&layer_handle);
    rect = GUI_CreateRect(0,0, image_width - 1, image_height - 1);
    GUIRES_DisplayImg(0, &rect,  0,  win_id,  image_id, &layer_handle);     
    
    //copy layer to new buffer    
    SCI_MEMCPY(p_buffer, p_layer_buffer, buffer_size);
    
    //tranform to PMA888
    MMI_ARGB888TOPMARGB888( (uint32*)p_buffer, buffer_size);
    
    UILAYER_RELEASELAYER(&layer_handle);

    return p_buffer;
}
#endif

