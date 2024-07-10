

/*****************************************************************************
** File Name:      mmi_livewallpaper_papertown.c                                    *
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
#ifndef _MMI_LIVEWALLPAPER_PAPERTOWN_C_
#define _MMI_LIVEWALLPAPER_PAPERTOWN_C_

#ifdef LIVE_WALLPAPER_SUPPORT_PAPERTOWN

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
#include "Mmiaudio_ctrl.h"
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#if 0
#define DMA_DONE_EVENT_TIMEOUT   0x32 //50ms
#define DMA_DONE_EVENT    (1<<0)
#endif

#define PAPERTOWN_CLOND_NUM   4
#define PAPERTOWN_BIRD_A_NUM   2
#define PAPERTOWN_BIRD_B_NUM   2
#define PAPERTOWN_CAT_NUM   6
#define PAPERTOWN_DOG_NUM   2
#define PAPERTOWN_TRUNK_NUM   2
#define PAPERTOWN_BUS_NUM   2
#define PAPERTOWN_CAR_NUM   2

#define PAPERTOWN_MOVE_STEP 4
#define BLOCK_MEM_POOL_SIZE_PAPERTOWN (460 * 1024)   // Set a  MEM POOL for different livewallpaper respectively;
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
}PAPERTOWN_INFO_T;

/**--------------------------------------------------------------------------*
 **                         LOCAL DEFINITION                                 *
 **--------------------------------------------------------------------------*/
LOCAL PAPERTOWN_INFO_T s_papertown_sun_info = {
	IMAGE_PAPERTOWN_SUN, {213 ,35}, {0,0}, {0,0}, {0,0}, PNULL
};

LOCAL PAPERTOWN_INFO_T s_papertown_cloud_info[PAPERTOWN_CLOND_NUM] = {
	{IMAGE_PAPERTOWN_CLOUD0, {41*256/320 , 82*256/480}, {0,0}, {0,0}, {0,0}, PNULL},
	{IMAGE_PAPERTOWN_CLOUD1, {320*256/320, 102*256/480}, {0,0}, {0,0}, {0,0}, PNULL},
	{IMAGE_PAPERTOWN_CLOUD2, {259*256/320, 58*256/480}, {0,0}, {0,0}, {0,0}, PNULL},
	{IMAGE_PAPERTOWN_CLOUD3, {286*256/320, 77*256/480}, {0,0}, {0,0}, {0,0}, PNULL}
};

LOCAL PAPERTOWN_INFO_T s_papertown_birda_info[PAPERTOWN_BIRD_A_NUM] = {
	{IMAGE_PAPERTOWN_BIRDA0, {192 , 57}, {0,0}, {0,0}, {0,0}, PNULL},
	{IMAGE_PAPERTOWN_BIRDA1, {192 , 57}, {0,0}, {0,0}, {0,0}, PNULL}
};
LOCAL PAPERTOWN_INFO_T s_papertown_birdb_info[PAPERTOWN_BIRD_B_NUM] = {
	{IMAGE_PAPERTOWN_BIRDB0, {256 , 66}, {0,0}, {0,0}, {0,0}, PNULL},
	{IMAGE_PAPERTOWN_BIRDB1, {256 , 66}, {0,0}, {0,0}, {0,0}, PNULL}
};

LOCAL PAPERTOWN_INFO_T  s_papertown_cat_info[PAPERTOWN_CAT_NUM] = {
	{IMAGE_PAPERTOWN_CAT0, {130*256/320 , 172*256/480}, {0,0}, {0,0}, {0,0},PNULL},
	{IMAGE_PAPERTOWN_CAT1, {130*256/320 , 172*256/480}, {0,0}, {0,0}, {0,0}, PNULL},
	{IMAGE_PAPERTOWN_CAT2, {130*256/320 , 172*256/480}, {0,0}, {0,0}, {0,0}, PNULL},
	{IMAGE_PAPERTOWN_CAT3, {130*256/320 , 172*256/480}, {0,0}, {0,0}, {0,0}, PNULL},
	{IMAGE_PAPERTOWN_CAT4, {130*256/320 , 172*256/480}, {0,0}, {0,0}, {0,0}, PNULL},
	{IMAGE_PAPERTOWN_CAT5, {130*256/320 , 172*256/480}, {0,0}, {0,0}, {0,0}, PNULL}
}; 

LOCAL PAPERTOWN_INFO_T  s_papertown_dog_info[PAPERTOWN_DOG_NUM] = {
	{IMAGE_PAPERTOWN_DOG0, {0 , 322*256/480}, {0,0}, {0,0}, {0,0}, PNULL},
	{IMAGE_PAPERTOWN_DOG1, {0 , 322*256/480}, {0,0}, {0,0}, {0,0}, PNULL}
}; 

LOCAL PAPERTOWN_INFO_T  s_papertown_truck_info[PAPERTOWN_TRUNK_NUM] = {
	{IMAGE_PAPERTOWN_TRUCK0, {-160 , 325*256/480}, {0,0}, {0,0}, {0,0}, PNULL},
	{IMAGE_PAPERTOWN_TRUCK1, {-160 , 325*256/480}, {0,0}, {0,0}, {0,0}, PNULL}
}; 

LOCAL PAPERTOWN_INFO_T  s_papertown_bus_info[PAPERTOWN_BUS_NUM] = {
	{IMAGE_PAPERTOWN_BUS0, {0 , 325*256/480}, {0,0}, {0,0}, {0,0}, PNULL},
	{IMAGE_PAPERTOWN_BUS1, {0 , 325*256/480}, {0,0}, {0,0}, {0,0}, PNULL}
}; 

LOCAL PAPERTOWN_INFO_T  s_papertown_car_info[PAPERTOWN_CAR_NUM] = {
	{IMAGE_PAPERTOWN_CAR0, {320 , 362*256/480}, {0,0}, {0,0}, {0,0}, PNULL},
	{IMAGE_PAPERTOWN_CAR1, {320 , 362*256/480}, {0,0}, {0,0}, {0,0}, PNULL}
}; 

LOCAL int16 s_papertown_cloud_move_step[PAPERTOWN_MOVE_STEP] = {1,1,1,1};


LOCAL int16 s_papertown_birda_move_step = 2;
LOCAL int16 s_papertown_birdb_move_step = 2;

LOCAL uint8 s_papertown_cat_state  = 0;
LOCAL int16 s_papertown_cat_move_step = 2;

LOCAL int16 s_papertown_dog_move_step = 1;

LOCAL int16 s_papertown_truck_move_step = 3;

LOCAL int16 s_papertown_bus_move_step = 3;
LOCAL int16 s_papertown_car_move_step = 3;

LOCAL GUI_LCD_DEV_INFO s_papertown_sun_layer_handle ;
LOCAL GUI_LCD_DEV_INFO s_papertown_cloud_layer_handle[PAPERTOWN_CLOND_NUM];
LOCAL GUI_LCD_DEV_INFO s_papertown_birda_layer_handle[PAPERTOWN_BIRD_A_NUM] ;
LOCAL GUI_LCD_DEV_INFO s_papertown_birdb_layer_handle[PAPERTOWN_BIRD_B_NUM] ;
LOCAL GUI_LCD_DEV_INFO s_papertown_cat_layer_handle[PAPERTOWN_CAT_NUM] ;
LOCAL GUI_LCD_DEV_INFO s_papertown_dog_layer_handle[PAPERTOWN_DOG_NUM] ;
LOCAL GUI_LCD_DEV_INFO s_papertown_truck_layer_handle[PAPERTOWN_TRUNK_NUM] ;
LOCAL GUI_LCD_DEV_INFO s_papertown_bus_layer_handle[PAPERTOWN_BUS_NUM] ;
LOCAL GUI_LCD_DEV_INFO s_papertown_car_layer_handle[PAPERTOWN_CAR_NUM] ;
LOCAL FIX16_POINT_T *s_point_add = PNULL;
    
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
LOCAL BOOLEAN papertownInit(void);
/*****************************************************************************/
//  Description : 初始化
//  Global resource dependence : 
//  Author:arvin.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN papertownConstruct(MMI_WIN_ID_T win_id);

/*****************************************************************************/
//  Description : 准备背景数据
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN papertownDrawBg(MMI_WIN_ID_T win_id,const GUI_LCD_DEV_INFO *dev_info_ptr);

/*****************************************************************************/
//  Description : 算法
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL void papertownCalculate(LW_CAL_PARAM *lw_cal_param);
/*****************************************************************************/
//  Description : 释放
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:销毁临时申请的变量
/*****************************************************************************/
LOCAL void papertownDestruct(void);
/*****************************************************************************/
//  Description : 交互事件处理
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN papertownHandleEvent(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/*****************************************************************************/
//  Description : 获得动态壁纸类型
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL uint32 papertownGetType(void);
/*****************************************************************************/
//  Description : 获得动态壁纸ThumbNail信息
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL void papertownGetThumbNailInfo(LW_THUMBNAIL_INFO *thumbnail_info_ptr);

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/
PUBLIC const MMI_LIVE_WALLPAPER_ITEM_INFO_T g_papertown_Instance =
{
    papertownInit,
    papertownConstruct,
    papertownDrawBg,
    papertownCalculate,
    papertownDestruct,
    papertownHandleEvent,
    PNULL,
    papertownGetType,
    papertownGetThumbNailInfo,
    PNULL,
    PNULL,
    PNULL,
    PNULL
};


LOCAL BOOLEAN papertown_CreateImageLayer(PAPERTOWN_INFO_T *image_info, GUI_LCD_DEV_INFO *layer_handle_ptr, MMI_HANDLE_T win_id)
{
    UILAYER_INFO_T   layer_info;
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

        if(PNULL != wallpaper_layer_buf_ptr)
        {
            //加速
            for (i=0; i<layer_info.layer_width*layer_info.layer_height; i++)
            {
                if(*((uint32*)(wallpaper_layer_buf_ptr)+i) < 0x08000000)
                {
                    *((uint32*)(wallpaper_layer_buf_ptr)+i) = 0x00000000;
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
    }
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

LOCAL BOOLEAN  papertown_is_in_region(int16 x, int16 y, PAPERTOWN_INFO_T *theme_info_ptr)
{
    if (x>=theme_info_ptr->theme_offset_actual.x &&
        x<=theme_info_ptr->theme_offset_actual.x+theme_info_ptr->theme_size.w &&
        y>=theme_info_ptr->theme_offset_actual.y &&
        y<=theme_info_ptr->theme_offset_actual.y+theme_info_ptr->theme_size.h)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }

}
extern PUBLIC void MMI_APISET_LivePaperPlayRing(
                         MMI_RING_ID_T     label,
                         uint8             ring_vol, // the ring volume
                         uint32            play_times
                         );
LOCAL void  papertown_PointProc(int16 x, int16 y)
{
    if (papertown_is_in_region(x, y, &s_papertown_birda_info[0]))
    {
        s_papertown_birda_move_step = 3;
   //     if (!MMIAUDIO_IsExistBgPlay())
        {
            MMI_APISET_LivePaperPlayRing(R_LIVEPAPER_BIRD1, 4, 1);
        }
    }

    if (papertown_is_in_region(x, y, &s_papertown_birdb_info[0]))
    {
        s_papertown_birdb_move_step = 3;
     //   if (!MMIAUDIO_IsExistBgPlay())
        {
            MMI_APISET_LivePaperPlayRing(R_LIVEPAPER_BIRD2, 3, 1);
        }
    }
    
    if (papertown_is_in_region(x, y, &s_papertown_cat_info[0]))
    {
        if (0 == s_papertown_cat_state || 3 == s_papertown_cat_state)
        {
            s_papertown_cat_state ++;
        }
      //  if (!MMIAUDIO_IsExistBgPlay())
        {
            MMI_APISET_LivePaperPlayRing(R_LIVEPAPER_CAT, 5, 1);
        }
    }

    if (papertown_is_in_region(x, y, &s_papertown_dog_info[0]))
    {
        s_papertown_dog_move_step = 3;
     //   if (!MMIAUDIO_IsExistBgPlay())
        {
            MMI_APISET_LivePaperPlayRing(R_LIVEPAPER_DOG, 6, 1);
        }
    }
    
    if (papertown_is_in_region(x, y, &s_papertown_bus_info[0]) ||
        papertown_is_in_region(x, y, &s_papertown_car_info[0]) ||
        papertown_is_in_region(x, y, &s_papertown_truck_info[0]))
    {
    //    if (!MMIAUDIO_IsExistBgPlay())
        {
            MMI_APISET_LivePaperPlayRing(R_LIVEPAPER_TRAFFIC, 6, 1);
        }
    }

}


/*---------------------------------------------------------------------------*/
/*                          FUNCTION DEFINITION                              */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : 静态数据初始化
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN papertownInit(void)
{
    BOOLEAN b_result = TRUE;
    return b_result;
}

/*****************************************************************************/
//  Description : 贴画小镇初始化
//  Global resource dependence : 
//  Author:arvin.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN papertownConstruct(MMI_WIN_ID_T win_id)
{
    BOOLEAN b_result = FALSE;
    uint32 i = 0;
    
    if(PNULL != s_point_add)
    {
        SCI_TRACE_LOW("papertownConstruct, have a exception before Construct: PNULL != s_point_add; papertown ");
        MMILIVEWALLPAPER_FREE(s_point_add);
        s_point_add = PNULL;
    }
    s_point_add=MMILIVEWALLPAPER_ALLOC((sizeof(FIX16_POINT_T))*2048);
    if(PNULL == s_point_add)
    {
        return FALSE;
    }

    if(!papertown_CreateImageLayer(&s_papertown_sun_info, &s_papertown_sun_layer_handle, win_id))
    {
        return b_result;
    }
    
    for(i=0; i<PAPERTOWN_CLOND_NUM; i++)
    {
        if(!papertown_CreateImageLayer(&s_papertown_cloud_info[i], &s_papertown_cloud_layer_handle[i], win_id))
        {
            return b_result;
        }
    }
    
    for(i=0; i<PAPERTOWN_CAT_NUM; i++)
    {
        if(!papertown_CreateImageLayer(&s_papertown_cat_info[i], &s_papertown_cat_layer_handle[i], win_id))
        {
            return b_result;
        }
    }
    
    for(i=0; i<PAPERTOWN_BIRD_A_NUM; i++)
    {
        if(!papertown_CreateImageLayer(&s_papertown_birda_info[i], &s_papertown_birda_layer_handle[i], win_id))
        {
            return b_result;
        }
    }
    for(i=0; i<PAPERTOWN_BIRD_B_NUM; i++)
    {
        if(!papertown_CreateImageLayer(&s_papertown_birdb_info[i], &s_papertown_birdb_layer_handle[i], win_id))
        {
            return b_result;
        }
    }
    for(i=0; i<PAPERTOWN_DOG_NUM; i++)
    {
        if(!papertown_CreateImageLayer(&s_papertown_dog_info[i], &s_papertown_dog_layer_handle[i], win_id))
        {
            return b_result;
        }
    }
    for(i=0; i<PAPERTOWN_TRUNK_NUM; i++)
    {
        if(!papertown_CreateImageLayer(&s_papertown_truck_info[i], &s_papertown_truck_layer_handle[i], win_id))
        {
            return b_result;
        }
    }
    for(i=0; i<PAPERTOWN_BUS_NUM; i++)
    {
        if(!papertown_CreateImageLayer(&s_papertown_bus_info[i], &s_papertown_bus_layer_handle[i], win_id))
        {
            return b_result;
        }
    }
    for(i=0; i<PAPERTOWN_CAR_NUM; i++)
    {
        if(!papertown_CreateImageLayer(&s_papertown_car_info[i], &s_papertown_car_layer_handle[i], win_id))
        {
            return b_result;
    }        
    }
    b_result = TRUE;
    return b_result;
}

/*****************************************************************************/
//  Description : 贴画小镇准备背景数据
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN papertownDrawBg(MMI_WIN_ID_T win_id,const GUI_LCD_DEV_INFO *dev_info_ptr)
{
    BOOLEAN b_result = FALSE;
    GUI_POINT_T     display_point   = {0,0};

    if(PNULL != dev_info_ptr && UILAYER_IsLayerActive(dev_info_ptr))
    {
        b_result = GUIRES_DisplayImg(&display_point, 
                        PNULL, 
                        PNULL, 
                        win_id, 
                        IMAGE_PAPERTOWN_BG, 
                        dev_info_ptr); 
    }
    return b_result;
}

/*****************************************************************************/
//  Description : 贴画小镇算法
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL void papertownCalculate(LW_CAL_PARAM *lw_cal_param)
{
    static uint32 s_cost_time = 0;
    uint32 i = 0;
    static int32 s_papertown_sun_angle  = 0;
    static int32 s_papertown_sun_cnt    = 0;

    static uint8 s_papertown_birda_state = 0;
    static uint8 s_papertown_birdb_state = 0;
    static uint8 s_papertown_dog_state  = 0;
    static uint8 s_papertown_truck_state  = 0;
    static uint8 s_papertown_bus_state  = 0;
    static uint8 s_papertown_car_state  = 0;

    static int32 s_papertown_dither = 1;

    IMGREF_SIZE_T  bg_window_size = {0};
    FIX16_POINT_T  up_offset = {0};
    uint32 *papertown_buf_ptr = PNULL;
    uint16 scr_width = 0;
    uint16 scr_height = 0;
    

    s_cost_time = SCI_GetTickCount();

    if(PNULL == lw_cal_param || PNULL == lw_cal_param->dest_buf_info.buf_ptr || PNULL == lw_cal_param->src_buf_info.buf_ptr)
    {
        return;
    }
    scr_width = lw_cal_param->src_buf_info.w;
    scr_height = lw_cal_param->src_buf_info.h;
    bg_window_size.w = scr_width;
    bg_window_size.h = scr_height;
    SCI_MEMCPY(lw_cal_param->dest_buf_info.buf_ptr,lw_cal_param->src_buf_info.buf_ptr,sizeof(uint16)*scr_width*scr_height);

    s_cost_time = SCI_GetTickCount() - s_cost_time;				
    SCI_TRACE_LOW("harry, s_cost_time papertown SCI_MEMCPY cost time = %d ms",s_cost_time); //about 50ms

    //画太阳
    s_cost_time = SCI_GetTickCount();
    papertown_buf_ptr = s_papertown_sun_info.theme_image_buf_ptr;

    s_papertown_sun_cnt ++;
    if (s_papertown_sun_cnt >= 6)
    {    
        s_papertown_sun_angle += 275;
        s_papertown_sun_cnt = 0;
    }
    if(s_papertown_sun_angle >=3600)
    {
        s_papertown_sun_angle-=3600;
    }
       
    S2d_rotation_slim(	bg_window_size,
    					s_papertown_sun_info.theme_size,
    					s_papertown_sun_info.theme_offset_actual,
    					s_papertown_sun_info.theme_mid,
    					s_papertown_sun_angle,
    					papertown_buf_ptr,
    					lw_cal_param->dest_buf_info.buf_ptr,
    					4096,
    					s_point_add);

    s_cost_time = SCI_GetTickCount() - s_cost_time;				
    SCI_TRACE_LOW("harry, s_cost_time papertown_city sun cost time = %d ms", s_cost_time); //about 7ms

    //画云
    s_cost_time = SCI_GetTickCount();
    for (i=0; i<PAPERTOWN_CLOND_NUM; i++)   
    {
        papertown_buf_ptr = s_papertown_cloud_info[i].theme_image_buf_ptr;

        s_papertown_cloud_info[i].theme_offset_actual.x -= s_papertown_cloud_move_step[i];
        if ( s_papertown_cloud_info[i].theme_offset_actual.x + s_papertown_cloud_info[i].theme_size.w < 0)
        {
            s_papertown_cloud_info[i].theme_offset_actual.x += scr_width + s_papertown_cloud_info[i].theme_size.w;
            //s_papertown_cloud_info[i].theme_offset_actual.y = rand()%30+80;
        }
        up_offset.x = s_papertown_cloud_info[i].theme_offset_actual.x;
        up_offset.y = s_papertown_cloud_info[i].theme_offset_actual.y;
        S2D_Blending(bg_window_size ,lw_cal_param->dest_buf_info.buf_ptr, s_papertown_cloud_info[i].theme_size, up_offset, (uint32*)papertown_buf_ptr, lw_cal_param->dest_buf_info.buf_ptr);

    }
    
    s_cost_time = SCI_GetTickCount() - s_cost_time;				
    SCI_TRACE_LOW("harry, s_cost_time papertown_city cloud cost time = %d ms", s_cost_time); //about 7ms


    //画bird
    s_cost_time = SCI_GetTickCount();

    if ((16/s_papertown_birda_move_step) > abs(s_papertown_birda_info[0].theme_offset_actual.x)%(32/s_papertown_birda_move_step))
    {
        s_papertown_birda_state = 1;
    }
    else
    {
        s_papertown_birda_state = 0;
    }
    papertown_buf_ptr = s_papertown_birda_info[s_papertown_birda_state].theme_image_buf_ptr;

    s_papertown_birda_info[0].theme_offset_actual.x -= s_papertown_birda_move_step;
    if ( s_papertown_birda_info[0].theme_offset_actual.x + s_papertown_birda_info[0].theme_size.w < 0)
    {
        s_papertown_birda_info[0].theme_offset_actual.x += scr_width + s_papertown_birda_info[0].theme_size.w;
        //s_papertown_birda_info[0].theme_offset_actual.y = rand()%30+80;
        s_papertown_birda_move_step = 2;
    }
    up_offset.x = s_papertown_birda_info[0].theme_offset_actual.x;
    up_offset.y = s_papertown_birda_info[0].theme_offset_actual.y;
    S2D_Blending(bg_window_size ,lw_cal_param->dest_buf_info.buf_ptr, s_papertown_birda_info[0].theme_size, up_offset, (uint32*)papertown_buf_ptr, lw_cal_param->dest_buf_info.buf_ptr);


    if ((16/s_papertown_birdb_move_step) > abs(s_papertown_birdb_info[0].theme_offset_actual.x)%(32/s_papertown_birdb_move_step))
    {
        s_papertown_birdb_state = 1;
    }
    else
    {
        s_papertown_birdb_state = 0;
    }
    papertown_buf_ptr = s_papertown_birdb_info[s_papertown_birdb_state].theme_image_buf_ptr;

    s_papertown_birdb_info[0].theme_offset_actual.x -= s_papertown_birdb_move_step;
    if ( s_papertown_birdb_info[0].theme_offset_actual.x + s_papertown_birdb_info[0].theme_size.w < 0)
    {
        s_papertown_birdb_info[0].theme_offset_actual.x += scr_width + s_papertown_birdb_info[0].theme_size.w;
        //s_papertown_birdb_info[0].theme_offset_actual.y = rand()%30+100;
        s_papertown_birdb_move_step = 2;
    }
    up_offset.x = s_papertown_birdb_info[0].theme_offset_actual.x;
    up_offset.y = s_papertown_birdb_info[0].theme_offset_actual.y;
    S2D_Blending(bg_window_size ,lw_cal_param->dest_buf_info.buf_ptr, s_papertown_birdb_info[0].theme_size, up_offset, (uint32*)papertown_buf_ptr, lw_cal_param->dest_buf_info.buf_ptr);
    s_cost_time = SCI_GetTickCount() - s_cost_time;				
    SCI_TRACE_LOW("harry, s_cost_time papertown_city bird cost time = %d ms", s_cost_time); //about 7ms

    //画猫
    s_cost_time = SCI_GetTickCount();

    if (s_papertown_cat_state >= 6)
    {
        s_papertown_cat_state = 0;
    }

    if (1 == s_papertown_cat_state)
    {
        if ( s_papertown_cat_info[0].theme_offset_actual.x > (int16)((44*scr_width)>>8))
        {
            s_papertown_cat_info[0].theme_offset_actual.x -= s_papertown_cat_move_step;
            s_papertown_cat_state = 2;

        }
        else
        {
            s_papertown_cat_state = 3;
        }
    }
    else if (2 == s_papertown_cat_state)
    {
        if ( s_papertown_cat_info[0].theme_offset_actual.x > (int16)((44*scr_width)>>8))
        {
            s_papertown_cat_info[0].theme_offset_actual.x -= s_papertown_cat_move_step;
            s_papertown_cat_state = 1;
        }
        else
        {
            s_papertown_cat_state = 3;
        }
    }
    else if (4 == s_papertown_cat_state)
    {
        if ( s_papertown_cat_info[0].theme_offset_actual.x < (int16)((104*scr_width)>>8))
        {
            s_papertown_cat_info[0].theme_offset_actual.x += s_papertown_cat_move_step;
            s_papertown_cat_state = 5;
        }
        else
        {
            s_papertown_cat_state = 0;
        }
    }
    else if (5 == s_papertown_cat_state)
    {
        if ( s_papertown_cat_info[0].theme_offset_actual.x < (int16)((104*scr_width)>>8))
        {
            s_papertown_cat_info[0].theme_offset_actual.x += s_papertown_cat_move_step;
            s_papertown_cat_state = 4;
        }
        else
        {
            s_papertown_cat_state = 0;
        }
    }
    papertown_buf_ptr = s_papertown_cat_info[s_papertown_cat_state].theme_image_buf_ptr;

    up_offset.x = s_papertown_cat_info[0].theme_offset_actual.x;
    up_offset.y = s_papertown_cat_info[0].theme_offset_actual.y;
    S2D_Blending(bg_window_size ,lw_cal_param->dest_buf_info.buf_ptr, s_papertown_cat_info[s_papertown_cat_state].theme_size, up_offset, (uint32*)papertown_buf_ptr, (uint16*)lw_cal_param->dest_buf_info.buf_ptr);

    s_cost_time = SCI_GetTickCount() - s_cost_time;				
    SCI_TRACE_LOW("harry, s_cost_time papertown_city cat cost time = %d ms", s_cost_time); //about 7ms


    //画狗
    s_cost_time = SCI_GetTickCount();

    if (8/s_papertown_dog_move_step > abs(s_papertown_dog_info[0].theme_offset_actual.x)%(16/s_papertown_dog_move_step))
    {
        s_papertown_dog_state = 1;
    }
    else
    {
        s_papertown_dog_state = 0;
    }
    papertown_buf_ptr = s_papertown_dog_info[s_papertown_dog_state].theme_image_buf_ptr;

    s_papertown_dog_info[0].theme_offset_actual.x += s_papertown_dog_move_step;
    if ( s_papertown_dog_info[0].theme_offset_actual.x > scr_width + s_papertown_dog_info[0].theme_size.w)
    {
        s_papertown_dog_info[0].theme_offset_actual.x -= scr_width + 2*s_papertown_dog_info[0].theme_size.w;
        s_papertown_dog_move_step = 1;
    }
    up_offset.x = s_papertown_dog_info[0].theme_offset_actual.x;
    up_offset.y = s_papertown_dog_info[0].theme_offset_actual.y;
    S2D_Blending(bg_window_size ,lw_cal_param->dest_buf_info.buf_ptr, s_papertown_dog_info[0].theme_size, up_offset, (uint32*)papertown_buf_ptr, lw_cal_param->dest_buf_info.buf_ptr);

    s_cost_time = SCI_GetTickCount() - s_cost_time;				
    SCI_TRACE_LOW("harry, s_cost_time papertown_city dog cost time = %d ms", s_cost_time); //about 7ms

    //车抖动
    if (s_papertown_dither > 0)
    {
        s_papertown_dither = -1;
    }
    else
    {
        s_papertown_dither = 1;
    }

    //画卡车
    s_cost_time = SCI_GetTickCount();
    
    if (48 > s_papertown_truck_info[0].theme_offset_actual.x%96)
    {
        s_papertown_truck_state = 1;
    }
    else
    {
        s_papertown_truck_state = 0;
    }
    papertown_buf_ptr = s_papertown_truck_info[s_papertown_truck_state].theme_image_buf_ptr;
    
    s_papertown_truck_info[0].theme_offset_actual.x += s_papertown_truck_move_step;
    if ( s_papertown_truck_info[0].theme_offset_actual.x > scr_width + s_papertown_bus_info[0].theme_size.w)
    {
        s_papertown_truck_info[0].theme_offset_actual.x -= scr_width + 2*s_papertown_bus_info[0].theme_size.w;
    }

    s_papertown_truck_info[0].theme_offset_actual.y += s_papertown_dither;
    up_offset.x = s_papertown_truck_info[0].theme_offset_actual.x;
    up_offset.y = s_papertown_truck_info[0].theme_offset_actual.y;

    S2D_Blending(bg_window_size ,lw_cal_param->dest_buf_info.buf_ptr, s_papertown_truck_info[0].theme_size, up_offset, (uint32*)papertown_buf_ptr, lw_cal_param->dest_buf_info.buf_ptr);

    s_cost_time = SCI_GetTickCount() - s_cost_time;				
    SCI_TRACE_LOW("harry, s_cost_time papertown_city truck cost time = %d ms", s_cost_time); //about 7ms

    //画bus
    s_cost_time = SCI_GetTickCount();

    if (48 > s_papertown_bus_info[0].theme_offset_actual.x%96)
    {
        s_papertown_bus_state = 1;
    }
    else
    {
        s_papertown_bus_state = 0;
    }
    papertown_buf_ptr = s_papertown_bus_info[s_papertown_truck_state].theme_image_buf_ptr;

    s_papertown_bus_info[0].theme_offset_actual.x += s_papertown_bus_move_step;
    if ( s_papertown_bus_info[0].theme_offset_actual.x > scr_width + s_papertown_bus_info[0].theme_size.w)
    {
        s_papertown_bus_info[0].theme_offset_actual.x -= scr_width + 2*s_papertown_bus_info[0].theme_size.w;
    }

    s_papertown_bus_info[0].theme_offset_actual.y += s_papertown_dither;
    up_offset.x = s_papertown_bus_info[0].theme_offset_actual.x;
    up_offset.y = s_papertown_bus_info[0].theme_offset_actual.y;

    S2D_Blending(bg_window_size ,lw_cal_param->dest_buf_info.buf_ptr, s_papertown_bus_info[0].theme_size, up_offset, (uint32*)papertown_buf_ptr, lw_cal_param->dest_buf_info.buf_ptr);

    s_cost_time = SCI_GetTickCount() - s_cost_time;				
    SCI_TRACE_LOW("harry, s_cost_time papertown_city bus cost time = %d ms", s_cost_time); //about 7ms

    //画car
    s_cost_time = SCI_GetTickCount();

    if (48 > s_papertown_car_info[0].theme_offset_actual.x%96)
    {
        s_papertown_car_state = 1;
    }
    else
    {
        s_papertown_car_state = 0;
    }
    papertown_buf_ptr = s_papertown_car_info[s_papertown_truck_state].theme_image_buf_ptr;

    s_papertown_car_info[0].theme_offset_actual.x -= s_papertown_bus_move_step;
    if ( s_papertown_car_info[0].theme_offset_actual.x < - s_papertown_car_info[0].theme_size.w)
    {
        s_papertown_car_info[0].theme_offset_actual.x += scr_width + s_papertown_car_info[0].theme_size.w;
    }

    s_papertown_car_info[0].theme_offset_actual.y += s_papertown_dither;
    up_offset.x = s_papertown_car_info[0].theme_offset_actual.x;
    up_offset.y = s_papertown_car_info[0].theme_offset_actual.y;

    S2D_Blending(bg_window_size ,lw_cal_param->dest_buf_info.buf_ptr, s_papertown_car_info[0].theme_size, up_offset, (uint32*)papertown_buf_ptr, lw_cal_param->dest_buf_info.buf_ptr);

    s_cost_time = SCI_GetTickCount() - s_cost_time;				
    SCI_TRACE_LOW("harry, s_cost_time papertown_city car cost time = %d ms", s_cost_time); //about 7ms

}
/*****************************************************************************/
//  Description : 贴画小镇释放
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL void papertownDestruct(void)
{
    uint32 i = 0;

    
    UILAYER_ReleaseLayerExt(&s_papertown_sun_layer_handle);
    s_papertown_sun_info.theme_image_buf_ptr = PNULL;
    
    for(i=0; i<PAPERTOWN_CLOND_NUM; i++)
    {
        UILAYER_ReleaseLayerExt(&s_papertown_cloud_layer_handle[i]);
        s_papertown_cloud_info[i].theme_image_buf_ptr = PNULL;
    }
    
    for(i=0; i<PAPERTOWN_CAT_NUM; i++)
    {
        UILAYER_ReleaseLayerExt(&s_papertown_cat_layer_handle[i]);
        s_papertown_cat_info[i].theme_image_buf_ptr = PNULL;
    }
    
    for(i=0; i<PAPERTOWN_BIRD_A_NUM; i++)
    {
        UILAYER_ReleaseLayerExt(&s_papertown_birda_layer_handle[i]);
        s_papertown_birda_info[i].theme_image_buf_ptr = PNULL;
    }
    for(i=0; i<PAPERTOWN_BIRD_B_NUM; i++)
    {
        UILAYER_ReleaseLayerExt(&s_papertown_birdb_layer_handle[i]);
        s_papertown_birdb_info[i].theme_image_buf_ptr = PNULL;
    }
    for(i=0; i<PAPERTOWN_DOG_NUM; i++)
    {
        UILAYER_ReleaseLayerExt(&s_papertown_dog_layer_handle[i]);
        s_papertown_dog_info[i].theme_image_buf_ptr = PNULL;
    }
    for(i=0; i<PAPERTOWN_TRUNK_NUM; i++)
    {
        UILAYER_ReleaseLayerExt(&s_papertown_truck_layer_handle[i]);
        s_papertown_truck_info[i].theme_image_buf_ptr = PNULL;
    }
    for(i=0; i<PAPERTOWN_BUS_NUM; i++)
    {
        UILAYER_ReleaseLayerExt(&s_papertown_bus_layer_handle[i]);
        s_papertown_bus_info[i].theme_image_buf_ptr = PNULL;
    }
    for(i=0; i<PAPERTOWN_CAR_NUM; i++)
    {
        UILAYER_ReleaseLayerExt(&s_papertown_car_layer_handle[i]);
        s_papertown_car_info[i].theme_image_buf_ptr = PNULL;
    }     

    //Release memory of point_add_ptr.
    if(PNULL != s_point_add)
    {
        MMILIVEWALLPAPER_FREE(s_point_add);
        s_point_add = PNULL;
    }   
}

/*****************************************************************************/
//  Description : 贴画小镇释交互事件处理
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN papertownHandleEvent(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    GUI_POINT_T   point_down      = {0};
    
    switch(msg_id)
    {
    case MSG_TP_PRESS_DOWN:
        point_down.x = MMK_GET_TP_X(param);
        point_down.y = MMK_GET_TP_Y(param);
        papertown_PointProc(point_down.x, point_down.y);
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
LOCAL uint32 papertownGetType(void)
{
    return MMI_LIVE_WALLPAPER_TYPE_PAPERTOWN;
}
/*****************************************************************************/
//  Description : 获得动态壁纸ThumbNail信息
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL void papertownGetThumbNailInfo(LW_THUMBNAIL_INFO *thumbnail_info_ptr)
{
    if(thumbnail_info_ptr)
    {
        thumbnail_info_ptr->is_image_file = FALSE;
        thumbnail_info_ptr->image_id = IMAGE_WALLPAPER_PAPERTOWN_THUMBNAIL;

        thumbnail_info_ptr->is_not_string = FALSE;
        thumbnail_info_ptr->text_id = TXT_SET_WALLPAPER_PAPERTOWN;
        thumbnail_info_ptr->instance_mem_pool_size = BLOCK_MEM_POOL_SIZE_PAPERTOWN;
    }
}
#endif// LIVE_WALLPAPER_SUPPORT_LINEAR


#endif //_MMI_LIVEWALLPAPER_LINEAR_C_


/*Edit by script, ignore 1 case. Thu Apr 26 19:00:58 2012*/ //IGNORE9527
