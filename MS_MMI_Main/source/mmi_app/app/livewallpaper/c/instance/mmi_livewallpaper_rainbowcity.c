

/*****************************************************************************
** File Name:      mmi_livewallpaper_rainbowcity.c                                    *
** Author:         Arvin.wu                                                       *
** Date:           18/06/2011                                                *
** Copyright:      2011 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe runninglight wallpaer       *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 08/2011       Arvin.wu          Modify                                  *
******************************************************************************/
#ifndef _MMI_LIVEWALLPAPER_RAINBOWCITY_C_
#define _MMI_LIVEWALLPAPER_RAINBOWCITY_C_

#include "mmi_app_livewallpaper_trc.h"
#ifdef LIVE_WALLPAPER_SUPPORT_RAINBOWCITY

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmilivewallpaper_export.h"

#include "mmi_text.h"
#include "mmi_image.h"
#include "mmiset_text.h"
#include "mmiset_image.h"
#include "mmi_livewallpaper_image.h"
#include "mmi_livewallpaper_text.h"
#include "graphics_3d_rotate.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#if 0
#define DMA_DONE_EVENT_TIMEOUT   0x32 //50ms
#define DMA_DONE_EVENT    (1<<0)
#endif

#define RAINBOWCITY_THEME_NUM    8
#define BLOCK_MEM_POOL_SIZE_RAINBOWCITY (350 * 1024)   // Set a  MEM POOL for different livewallpaper respectively;

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/


typedef struct
{
	MMI_IMAGE_ID_T theme_image_id;
	IMGREF_POINT_T theme_offset_relative;
	IMGREF_POINT_T theme_offset_actual;	
	IMGREF_SIZE_T  theme_size;
	IMGREF_POINT_T theme_mid;
	void *theme_image_buf_ptr;
}RAINBOWCITY_INFO_T;

/**--------------------------------------------------------------------------*
 **                         LOCAL DEFINITION                                 *
 **--------------------------------------------------------------------------*/

LOCAL RAINBOWCITY_INFO_T s_rainbowcity_theme_info[] = {
{IMAGE_RAINBOWCITY_THEME_01, {(int16)0xffff, (int16)0xffff}, {(int16)0xffff, (int16)0xffff}, {60  , 60}, {30 , 30},PNULL},
{IMAGE_RAINBOWCITY_THEME_02, {(int16)0xffff, (int16)0xffff}, {(int16)0xffff, (int16)0xffff}, {60  , 60}, {30 , 30},PNULL},
{IMAGE_RAINBOWCITY_THEME_03, {(int16)0xffff, (int16)0xffff}, {(int16)0xffff, (int16)0xffff}, {60  , 60}, {30 , 30},PNULL},
{IMAGE_RAINBOWCITY_THEME_04, {(int16)0xffff, (int16)0xffff}, {(int16)0xffff, (int16)0xffff}, {60  , 60}, {30 , 30},PNULL},
{IMAGE_RAINBOWCITY_THEME_05, {(int16)0xffff, (int16)0xffff}, {(int16)0xffff, (int16)0xffff}, {60  , 60}, {30 , 30},PNULL},
{IMAGE_RAINBOWCITY_THEME_06, {(int16)0xffff, (int16)0xffff}, {(int16)0xffff, (int16)0xffff}, {60  , 60}, {30 , 30},PNULL},
{IMAGE_RAINBOWCITY_THEME_07, {(int16)0xffff, (int16)0xffff}, {(int16)0xffff, (int16)0xffff}, {60  , 60}, {30 , 30},PNULL},
{IMAGE_RAINBOWCITY_THEME_08, {(int16)0xffff, (int16)0xffff}, {(int16)0xffff, (int16)0xffff}, {60  , 60}, {30 , 30},PNULL}
};
LOCAL RAINBOWCITY_INFO_T s_rainbowcity_city_info = {
IMAGE_RAINBOWCITY_CITY, {(int16)0xffff, (int16)0xffff}, {(int16)0xffff, (int16)0xffff}, {0, 0}, {0, 0},PNULL
};
LOCAL GUI_LCD_DEV_INFO s_rainbowcity_theme_layer_handle[RAINBOWCITY_THEME_NUM] ;
LOCAL GUI_LCD_DEV_INFO s_rainbowcity_city_layer_handle ;
LOCAL FIX16_POINT_T *s_point_add = PNULL;
    
uint32 s_rainbow_move_step = 10;

int32  s_rainbow_angle_step[RAINBOWCITY_THEME_NUM] = {0};
int32  s_rot_direction[RAINBOWCITY_THEME_NUM] = {0};

int16 s_rainbowcity_theme_x[RAINBOWCITY_THEME_NUM] = {0};
int16 s_rainbowcity_theme_x_end[RAINBOWCITY_THEME_NUM] = {0};
int16 s_rainbowcity_theme_y[RAINBOWCITY_THEME_NUM] = {0};
int16 s_rainbowcity_theme_x_top[RAINBOWCITY_THEME_NUM] = {0};
int16 s_rainbowcity_theme_y_top[RAINBOWCITY_THEME_NUM] = {0};

uint32 s_rainbowcity_theme_index = 0;

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/
 
/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                                                         */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : 静态数据初始化
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN rainbowcityInit(void);
/*****************************************************************************/
//  Description : 初始化
//  Global resource dependence : 
//  Author:arvin.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN rainbowcityConstruct(MMI_WIN_ID_T win_id);

/*****************************************************************************/
//  Description : 准备背景数据
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN rainbowcityDrawBg(MMI_WIN_ID_T win_id,const GUI_LCD_DEV_INFO *dev_info_ptr);

/*****************************************************************************/
//  Description : 算法
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL void rainbowcityCalculate(LW_CAL_PARAM *lw_cal_param);
/*****************************************************************************/
//  Description : 释放
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:销毁临时申请的变量
/*****************************************************************************/
LOCAL void rainbowcityDestruct(void);
/*****************************************************************************/
//  Description : 交互事件处理
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN rainbowcityHandleEvent(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/*****************************************************************************/
//  Description : 获得动态壁纸类型
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL uint32 rainbowcityGetType(void);
/*****************************************************************************/
//  Description : 获得动态壁纸ThumbNail信息
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL void rainbowcityGetThumbNailInfo(LW_THUMBNAIL_INFO *thumbnail_info_ptr);

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/
PUBLIC const MMI_LIVE_WALLPAPER_ITEM_INFO_T g_rainbowcity_Instance =
{
    rainbowcityInit,
    rainbowcityConstruct,
    rainbowcityDrawBg,
    rainbowcityCalculate,
    rainbowcityDestruct,
    rainbowcityHandleEvent,
    PNULL,
    rainbowcityGetType,
    rainbowcityGetThumbNailInfo,
    PNULL,
    PNULL,
    PNULL,
    PNULL
};

LOCAL BOOLEAN rainbowcity_CreateImageLayer(RAINBOWCITY_INFO_T *image_info, GUI_LCD_DEV_INFO *layer_handle_ptr, MMI_HANDLE_T win_id)
{
    UILAYER_INFO_T   layer_info = {0};
    UILAYER_CREATE_T create_info = {0}; 
    GUI_POINT_T      display_point       = {0,0};

    uint32 i = 0;

    uint8 * wallpaper_layer_buf_ptr = PNULL;
   
    /*先创建一个层*/
    layer_handle_ptr->lcd_id = 0;
    layer_handle_ptr->block_id = UILAYER_NULL_HANDLE;

    create_info.lcd_id = GUI_MAIN_LCD_ID;
    create_info.owner_handle = (MMI_HANDLE_T)win_id;
    create_info.offset_x = 0;
    create_info.offset_y = 0;
    GUIRES_GetImgWidthHeight((uint16*)&create_info.width, (uint16*)&create_info.height, image_info->theme_image_id, win_id);
    create_info.is_bg_layer = FALSE;
    create_info.is_static_layer = TRUE; 
    create_info.func_ptr = MMIAPILIVEWALLPAPER_UilayerMemPoolFunc(); 
      
    if(UILAYER_RESULT_SUCCESS == UILAYER_CreateLayer(
    					&create_info,
    					layer_handle_ptr
    					))
    {
        // 设置color key
        UILAYER_SetLayerColorKey(layer_handle_ptr, TRUE, UILAYER_TRANSPARENT_COLOR);

        // 使用color清除层
        UILAYER_Clear(layer_handle_ptr);

        //把图片画到层上
        GUIRES_DisplayImg(&display_point,
        					PNULL,
        					PNULL,
        					win_id,
        					image_info->theme_image_id,
        					(const GUI_LCD_DEV_INFO*)layer_handle_ptr);
        //获取图片的信息
        UILAYER_GetLayerInfo(layer_handle_ptr, &layer_info);

        wallpaper_layer_buf_ptr = UILAYER_GetLayerBufferPtr(layer_handle_ptr);  
        
        //加速
        if(PNULL != wallpaper_layer_buf_ptr)
        {
            for (i=0; i<layer_info.layer_width*layer_info.layer_height; i++)
            {
                if(*((uint32*)(wallpaper_layer_buf_ptr)+i) < 0x08000000)
                {
                    *((uint32*)(wallpaper_layer_buf_ptr)+i) = 0x00000000;
                }
            }
        }
        image_info->theme_image_buf_ptr = wallpaper_layer_buf_ptr;
        image_info->theme_size.w = layer_info.layer_width;
        image_info->theme_size.h = layer_info.layer_height;
        image_info->theme_mid.x = image_info->theme_size.w/2+image_info->theme_size.w%2;
        image_info->theme_mid.y = image_info->theme_size.h/2+image_info->theme_size.w%2;
        image_info->theme_offset_actual.x = (int16)(((image_info->theme_offset_relative.x)*MMI_MAINSCREEN_WIDTH)>>8);
        image_info->theme_offset_actual.y = (int16)(((image_info->theme_offset_relative.y)*MMI_MAINSCREEN_HEIGHT)>>8);
        return TRUE;
    }

    //SCI_TRACE_LOW:"Livewallpaper:rainbowcity_CreateImageLayer Faild!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_LIVEWALLPAPER_RAINBOWCITY_218_112_2_18_2_32_18_30,(uint8*)"");
    return FALSE;
}

#if 0

LOCAL SCI_EVENT_GROUP_PTR  s_dma_done_event = PNULL;

LOCAL void _DMA_Done_SetEvent(uint32 dma_set_flags)
{
    if(PNULL!=s_dma_done_event)
    {   
        SCI_SetEvent(s_dma_done_event, dma_set_flags, SCI_OR);
    }
}

LOCAL uint32 _DMA_Done_GetEvent(uint32 dma_requested_flags, uint32 dma_wait_option)
{
    uint32 actual_event = 0;
    uint32 status = SCI_ERROR;

    if(PNULL!=s_dma_done_event)
    {
        status = SCI_GetEvent(s_dma_done_event, 
                                dma_requested_flags, 
                                SCI_OR_CLEAR,
                                &actual_event, 
                                dma_wait_option);
    }

    return status;
}

typedef void (*NOTIFY_CB)(void);

PUBLIC uint32 DMA_HAL_Memcpy_Asy(void *des, void *src, uint32 count, NOTIFY_CB callback);

LOCAL void dma_hal_memcpy_done_callback(void)
{
    _DMA_Done_SetEvent(DMA_DONE_EVENT);
    //SCI_TRACE_LOW("harry, dma_hal_memcpy_done_callback");
}
#endif

LOCAL void  rainbowcity_PointProc(int16 x, int16 y)
{
    uint32 i = 0;
    int16 temp = 0;

    temp = GDI_sqrt(MMI_MAINSCREEN_HEIGHT-1-y)*2;

    for (i=0; i<RAINBOWCITY_THEME_NUM; i++)
    {
        if (s_rainbowcity_theme_x[s_rainbowcity_theme_index] == (int16)0xffff)
        {
            s_rainbowcity_theme_x_top[s_rainbowcity_theme_index] = x;
            s_rainbowcity_theme_y_top[s_rainbowcity_theme_index] = y;

            s_rainbowcity_theme_x[s_rainbowcity_theme_index] = (int16)(x - temp + 2) ;
            if (s_rainbowcity_theme_x[s_rainbowcity_theme_index] < 0)
            {
                s_rainbowcity_theme_x[s_rainbowcity_theme_index] = 0;
            }
            s_rainbowcity_theme_x_end[s_rainbowcity_theme_index] = (int16)(x + temp - 2);
            if (s_rainbowcity_theme_x_end[s_rainbowcity_theme_index] > (MMI_MAINSCREEN_HEIGHT-1))
            {
                s_rainbowcity_theme_x_end[s_rainbowcity_theme_index] = (MMI_MAINSCREEN_HEIGHT-1);
            }
            s_rainbowcity_theme_y[s_rainbowcity_theme_index] = 0;

            s_rainbow_angle_step[s_rainbowcity_theme_index] = (rand()%16 + 2)*16;
            s_rot_direction[s_rainbowcity_theme_index] = rand()%2;

            s_rainbowcity_theme_index++;
            if (s_rainbowcity_theme_index >= RAINBOWCITY_THEME_NUM)
            {
                s_rainbowcity_theme_index = 0;
            }
            break;
        }
    }

}


/*---------------------------------------------------------------------------*/
/*                          FUNCTION DEFINITION                                                              */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : 静态数据初始化
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN rainbowcityInit(void)
{
    BOOLEAN b_result = TRUE;
    uint32 i = 0;

    for(i=0; i<RAINBOWCITY_THEME_NUM; i++)
    {
        s_rainbowcity_theme_x[i] = (int16)0xffff;
        s_rainbowcity_theme_x_end[i] = (int16)0xffff;
        s_rainbowcity_theme_y[i] = (int16)0xffff;
        s_rainbowcity_theme_x_top[i] = (int16)0xffff;
        s_rainbowcity_theme_y_top[i] = (int16)0xffff;
    }
    return b_result;
}

/*****************************************************************************/
//  Description : 彩虹小镇初始化
//  Global resource dependence : 
//  Author:arvin.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN rainbowcityConstruct(MMI_WIN_ID_T win_id)
{
    BOOLEAN b_result = FALSE;
    uint32 i = 0;

    if(PNULL != s_point_add)
    {
        SCI_TRACE_LOW("rainbowcityConstruct, have a exception before Construct: PNULL != s_point_add; rainbowcity ");
        MMILIVEWALLPAPER_FREE(s_point_add);
        s_point_add = PNULL;
    }
    s_point_add=MMILIVEWALLPAPER_ALLOC((sizeof(FIX16_POINT_T))*2048);
    if(PNULL == s_point_add)
    {
        return FALSE;
    }
        
    for(i=0; i<RAINBOWCITY_THEME_NUM; i++)
    {
        if(!rainbowcity_CreateImageLayer(&s_rainbowcity_theme_info[i], &s_rainbowcity_theme_layer_handle[i], win_id))
        {
            return b_result;
        }
    }

    if(!rainbowcity_CreateImageLayer(&s_rainbowcity_city_info, &s_rainbowcity_city_layer_handle, win_id))
    {
        return b_result;
    }

    b_result = TRUE;
    return b_result;
}

/*****************************************************************************/
//  Description : 彩虹小镇准备背景数据
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN rainbowcityDrawBg(MMI_WIN_ID_T win_id,const GUI_LCD_DEV_INFO *dev_info_ptr)
{
    BOOLEAN b_result = FALSE;
    GUI_POINT_T     display_point   = {0,0};

    if(PNULL != dev_info_ptr)
    {
        
        if(UILAYER_IsLayerActive(dev_info_ptr))
        {
            b_result = GUIRES_DisplayImg(&display_point, 
                                        PNULL, 
                                        PNULL, 
                                        win_id, 
                                        IMAGE_RAINBOWCITY_BG, 
                                        dev_info_ptr); 
        }
    }
    else
    {
        //SCI_TRACE_LOW:"Livewallpaper:rainbowcityDrawBg Faild"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_LIVEWALLPAPER_RAINBOWCITY_363_112_2_18_2_32_18_31,(uint8*)"");
    }
    return b_result;
}

/*****************************************************************************/
//  Description : 彩虹小镇算法
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL void rainbowcityCalculate(LW_CAL_PARAM *lw_cal_param)
{
    LOCAL uint32 s_cost_time = 0;
    uint32 i = 0;

    IMGREF_SIZE_T  bg_window_size = {0};
    FIX16_POINT_T  up_offset = {0};
    IMGREF_SIZE_T  up_size   = {0};
    uint32 *rainbowcity_buf_ptr = PNULL;

    static int32 rainbowcity_theme_angle[RAINBOWCITY_THEME_NUM] = {0};
    uint16 *rainbowcity_src_start_ptr = PNULL;
    uint16 *rainbowcity_dst_start_ptr = PNULL;
    uint16 *target_buf_ptr = PNULL;
    uint16 *source_buf_ptr = PNULL;
    uint16 scr_width = 0;
    uint16 scr_height = 0;

    

    s_cost_time = SCI_GetTickCount();
    
    if(PNULL == lw_cal_param || PNULL == lw_cal_param->dest_buf_info.buf_ptr|| PNULL == lw_cal_param->src_buf_info.buf_ptr)
    {
        return;
    }
    target_buf_ptr = lw_cal_param->dest_buf_info.buf_ptr;
    source_buf_ptr = lw_cal_param->src_buf_info.buf_ptr;
    scr_width =  lw_cal_param->src_buf_info.w;
    scr_height =  lw_cal_param->src_buf_info.h;
    rainbowcity_src_start_ptr = source_buf_ptr;
    rainbowcity_dst_start_ptr = target_buf_ptr;
    
    bg_window_size.w = scr_width;
    bg_window_size.h = scr_height;
    for (i = 0; i < scr_height; i++)
    {
        SCI_MEMCPY(rainbowcity_dst_start_ptr+i*scr_width,rainbowcity_src_start_ptr+i*scr_width+s_rainbow_move_step,sizeof(uint16)*(scr_width-s_rainbow_move_step));
        SCI_MEMCPY(rainbowcity_dst_start_ptr+i*scr_width+scr_width-s_rainbow_move_step,rainbowcity_src_start_ptr+i*scr_width,sizeof(uint16)*(s_rainbow_move_step));
    }
    s_rainbow_move_step += 1;
    if (s_rainbow_move_step >= scr_width)
    {
        s_rainbow_move_step -= scr_width;
    }

    s_cost_time = SCI_GetTickCount() - s_cost_time;				
    SCI_TRACE_LOW("harry, cost_time rainbowcity SCI_MEMCPY cost time = %d ms",s_cost_time); //about 50ms


	
    for (i = 0; i < RAINBOWCITY_THEME_NUM; i++)
    {
        if (s_rainbowcity_theme_x[i] == (int16)0xffff)
        {
            continue;
        }

        s_rainbowcity_theme_x[i] += 1;
        s_rainbowcity_theme_y[i] = (s_rainbowcity_theme_x[i]-s_rainbowcity_theme_x_top[i])*(s_rainbowcity_theme_x[i]-s_rainbowcity_theme_x_top[i])/4 + s_rainbowcity_theme_y_top[i];


        if (s_rainbowcity_theme_x[i] > s_rainbowcity_theme_x_end[i])
        {
            s_rainbowcity_theme_x[i] = (int16)0xffff;
            continue;
        }

        rainbowcity_buf_ptr = s_rainbowcity_theme_info[i].theme_image_buf_ptr;
        s_rainbowcity_theme_info[i].theme_offset_actual.x = s_rainbowcity_theme_x[i];
        s_rainbowcity_theme_info[i].theme_offset_actual.y = s_rainbowcity_theme_y[i];
        
        S2d_rotation_slim(	bg_window_size,
                                    s_rainbowcity_theme_info[i].theme_size,
                                    s_rainbowcity_theme_info[i].theme_offset_actual,
                                    s_rainbowcity_theme_info[i].theme_mid,
                                    rainbowcity_theme_angle[i],
                                    rainbowcity_buf_ptr,
                                    target_buf_ptr,
                                    4096,
                                    s_point_add);

        if (0 == s_rot_direction[i])
        {
            rainbowcity_theme_angle[i] -= s_rainbow_angle_step[i];
            if(rainbowcity_theme_angle[i] <= 0)
            {
                rainbowcity_theme_angle[i] += 3600;
            }
        }
        else
        {
            rainbowcity_theme_angle[i] += s_rainbow_angle_step[i];
            if(rainbowcity_theme_angle[i] >= 3600)
            {
                rainbowcity_theme_angle[i] -= 3600;
            }
        }

    }	
#if 1

    //画city
    s_cost_time = SCI_GetTickCount();
    rainbowcity_buf_ptr = s_rainbowcity_city_info.theme_image_buf_ptr;
    up_offset.x = 0;
    up_offset.y = scr_height-s_rainbowcity_city_info.theme_size.h;
    up_size.w   = scr_width;
    up_size.h   = s_rainbowcity_city_info.theme_size.h;
    S2D_Blending(bg_window_size ,target_buf_ptr, up_size, up_offset ,(uint32*)rainbowcity_buf_ptr, target_buf_ptr);
    s_cost_time = SCI_GetTickCount() - s_cost_time;				
    SCI_TRACE_LOW("harry, cost_time rainbowcity_city cost time = %d ms", s_cost_time); //about 7ms

#endif
}
/*****************************************************************************/
//  Description : 彩虹小镇释放
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL void rainbowcityDestruct(void)
{
    uint32 i = 0;

    for(i=0; i<RAINBOWCITY_THEME_NUM; i++)
    {
        UILAYER_ReleaseLayerExt(&s_rainbowcity_theme_layer_handle[i]);
        s_rainbowcity_theme_info[i].theme_image_buf_ptr = PNULL;
    }
    UILAYER_ReleaseLayerExt(&s_rainbowcity_city_layer_handle);
    s_rainbowcity_city_info.theme_image_buf_ptr = PNULL;
     
    //Release memory of point_add_ptr.
    if(PNULL != s_point_add)
    {
        MMILIVEWALLPAPER_FREE(s_point_add);
        s_point_add = PNULL;
    }
}

/*****************************************************************************/
//  Description : 彩虹小镇释交互事件处理
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN rainbowcityHandleEvent(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    GUI_POINT_T   point_down      = {0};
    
    switch(msg_id)
    {
        case MSG_TP_PRESS_DOWN:
            point_down.x = MMK_GET_TP_X(param);
            point_down.y = MMK_GET_TP_Y(param);
            rainbowcity_PointProc(point_down.x, point_down.y);
        break;

        default:
            return FALSE;
    }
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    
    return TRUE;
}

/*****************************************************************************/
//  Description : 获得动态壁纸类型
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL uint32 rainbowcityGetType(void)
{
    return MMI_LIVE_WALLPAPER_TYPE_RAINBOWCITY;
}
/*****************************************************************************/
//  Description : 获得动态壁纸ThumbNail信息
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL void rainbowcityGetThumbNailInfo(LW_THUMBNAIL_INFO *thumbnail_info_ptr)
{
    if(thumbnail_info_ptr)
    {
        thumbnail_info_ptr->is_image_file = FALSE;
        thumbnail_info_ptr->image_id = IMAGE_WALLPAPER_RAINBOWCITY_THUMBNAIL;

        thumbnail_info_ptr->is_not_string = FALSE;
        thumbnail_info_ptr->text_id = TXT_SET_WALLPAPER_RAINBOWCITY;
        thumbnail_info_ptr->instance_mem_pool_size = BLOCK_MEM_POOL_SIZE_RAINBOWCITY;
    }
}
#endif// LIVE_WALLPAPER_SUPPORT_LINEAR


#endif //_MMI_LIVEWALLPAPER_LINEAR_C_


/*Edit by script, ignore 1 case. Thu Apr 26 19:00:58 2012*/ //IGNORE9527
