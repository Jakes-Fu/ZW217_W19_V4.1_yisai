/*****************************************************************************
** File Name:      mmi_live_wallpaper.c                                    *
** Author:         kyle.jin                                                *
** Date:           21/07/2011                                                *
** Copyright:      2011 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe runninglight wallpaer       *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
**  07/2011   kyle.jin            Create                                  *
** 08/2011       Arvin.wu          Modify                                  *
******************************************************************************/

#ifndef _MMI_LIVEWALLPAPER_BEE_C_
#define _MMI_LIVEWALLPAPER_BEE_C_

#ifdef LIVE_WALLPAPER_SUPPORT_BEE

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "mmilivewallpaper_export.h"

#include "mmi_text.h"
#include "mmi_image.h"
#include "mmiset_text.h"
#include "mmiset_image.h"
#include "mmi_livewallpaper_text.h"
#include "mmi_livewallpaper_image.h"
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#define BG_POP_NUM	10

#define BEE_POP_NUM	3                               //POP的数目
#define BEE_FRAME_NUM 8                              //蜜蜂的帧数

#ifndef LOW_MEMORY_SUPPORT
#define BEE_NUM 2                                    //蜜蜂的数目 //Whether has big bees; Yes;
#else
#define BEE_NUM 1                                    //蜜蜂的数目 //Whether has big bees; No;.
#endif

#define WING_RATE 2                                  //翅膀扇动的速度
#define BEE_HEAD_MOVE_NUM 3                       //互动时蜜蜂伸头的次数
#define BIG_BEE_WIDTH  120                            //大蜜蜂的宽度
#define SMALL_BEE_WIDTH 60                          //小蜜蜂的宽度
#ifdef LOW_MEMORY_SUPPORT
#define BLOCK_MEM_POOL_SIZE_BEE (300 * 1024)   // Set a  MEM POOL for different livewallpaper respectively;
#else
#define BLOCK_MEM_POOL_SIZE_BEE (700 * 1024)   // Set a  MEM POOL for different livewallpaper respectively;
#endif

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         LOCAL DEFINITION                                 *
**--------------------------------------------------------------------------*/
LOCAL BOOLEAN s_is_calculate_first_frame = FALSE;

LOCAL POP_INFO_T g_POP[BG_POP_NUM];
LOCAL UILAYER_INFO_T s_pop_layer_info_ptr[BG_POP_NUM] ;
LOCAL GUI_LCD_DEV_INFO s_pop_layer_handle[BG_POP_NUM]; //图片层
LOCAL POP_INFO_T g_BEE_POP[BEE_POP_NUM];//蜜蜂后面泡泡的运动属性数组
LOCAL LIVEWALLPAPER_UILAYER_INFO_T s_bee_pop_layer_info_ptr[BEE_POP_NUM] ;//泡泡层BUF
LOCAL GUI_LCD_DEV_INFO s_bee_pop_layer_handle[BEE_POP_NUM]; 
LOCAL LIVEWALLPAPER_UILAYER_INFO_T s_bee_layer_info_ptr[BEE_NUM][BEE_FRAME_NUM] ;//蜜蜂层BUF
LOCAL GUI_LCD_DEV_INFO s_bee_layer_handle[BEE_NUM][BEE_FRAME_NUM]; 

LOCAL int16 current_bee_frame_index[BEE_NUM] = {0};//现在蜜蜂显示哪一帧
LOCAL IMGREF_POINT_T	bee_current_pose[BEE_NUM];//现在蜜蜂的位置

LOCAL uint8 g_bee_flex_head_timer_id[BEE_NUM] = {0};//蜜蜂伸头的周期

LOCAL const MMI_IMAGE_ID_T bee_image_id[BEE_NUM][BEE_FRAME_NUM] = {
#ifndef LOW_MEMORY_SUPPORT
    IMAGE_WALLPAPER_BIG_BEE_0,
        IMAGE_WALLPAPER_BIG_BEE_1,
        IMAGE_WALLPAPER_BIG_BEE_2,
        IMAGE_WALLPAPER_BIG_BEE_3,//mifeng
        IMAGE_WALLPAPER_BIG_BEE_4,
        IMAGE_WALLPAPER_BIG_BEE_5,
        IMAGE_WALLPAPER_BIG_BEE_6,
        IMAGE_WALLPAPER_BIG_BEE_7,//mifeng
#endif
        IMAGE_WALLPAPER_SMALL_BEE_0,
        IMAGE_WALLPAPER_SMALL_BEE_1,
        IMAGE_WALLPAPER_SMALL_BEE_2,
        IMAGE_WALLPAPER_SMALL_BEE_3,
        IMAGE_WALLPAPER_SMALL_BEE_4,
        IMAGE_WALLPAPER_SMALL_BEE_5,
        IMAGE_WALLPAPER_SMALL_BEE_6,
        IMAGE_WALLPAPER_SMALL_BEE_7,
}; 

/**--------------------------------------------------------------------------*
**                         EXTERNAL DECLARE                                 *
**--------------------------------------------------------------------------*/
extern   int s2d_sin(int angle);
extern  int s2d_cos(int angle);

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : 蜜蜂刷新
//  Global resource dependence : 
//  Author:Gaily.Wang
//  Note:
/*****************************************************************************/
LOCAL void BeePopUpdate(COLOR16 *target_buf_ptr,uint16 scr_width, uint16 scr_height);
/*****************************************************************************/
//  Description : 小蜜蜂刷新
//  Global resource dependence : 
//  Author:Gaily.Wang
//  Note:
/*****************************************************************************/
LOCAL void BeeUpdate(COLOR16 *target_buf_ptr,uint16 scr_width, uint16 scr_height);

/*****************************************************************************/
//  Description : 蜜蜂互动判断
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL void BeeHandleTpPressUp(uint16 point_x, uint16 point_y);
/*****************************************************************************/
//  Description : 小蜜蜂初始化
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN BeeInit(void);
/*****************************************************************************/
//  Description : 小蜜蜂构造函数
//  Global resource dependence : 
//  Author:arvin.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN BeeConstruct(MMI_WIN_ID_T win_id);

/*****************************************************************************/
//  Description : 小蜜蜂准备背景数据
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN BeeDrawBg(MMI_WIN_ID_T win_id,const GUI_LCD_DEV_INFO *dev_info_ptr);

/*****************************************************************************/
//  Description : 小蜜蜂算法
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL void BeeCalculate(LW_CAL_PARAM *lw_cal_param);
/*****************************************************************************/
//  Description : 小蜜蜂释放
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:销毁临时申请的变量
/*****************************************************************************/
LOCAL void BeeDestruct(void);
/*****************************************************************************/
//  Description : 小蜜蜂交互事件处理
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN BeeHandleEvent(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
/*****************************************************************************/
//  Description : 获得动态壁纸类型
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL uint32 BeeGetType(void);
/*****************************************************************************/
//  Description : 获得动态壁纸ThumbNail信息
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL void BeeGetThumbNailInfo(LW_THUMBNAIL_INFO *thumbnail_info_ptr);
/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/
PUBLIC const MMI_LIVE_WALLPAPER_ITEM_INFO_T g_bee_Instance =
{
    BeeInit,
    BeeConstruct,
    BeeDrawBg,
    BeeCalculate,
    BeeDestruct,
    BeeHandleEvent,
    PNULL,
    BeeGetType,
    BeeGetThumbNailInfo,
    PNULL,
    PNULL,
    PNULL,
    PNULL
};
/*---------------------------------------------------------------------------*/
/*                          FUNCTION DEFINITION                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : 小蜜蜂刷新
//  Global resource dependence : 
//  Author:Gaily.Wang
//  Note:
/*****************************************************************************/
LOCAL void BeeUpdate(COLOR16 *target_buf_ptr,uint16 scr_width, uint16 scr_height)
{ 	
    IMGREF_SIZE_T	bee_bg_window = {0,0};
    IMGREF_SIZE_T	bee_up_window = {0,0};
    FIX16_POINT_T	offset = {0};
    
    int32 bee_index = 0;	 
    int32 x = 0;
    int32 y = 0;
    
    if (PNULL == target_buf_ptr)
    {
        return;
    }
    bee_bg_window.w = scr_width;
    bee_bg_window.h = scr_height;	
    
    for(bee_index=0;bee_index<BEE_NUM;bee_index++)
    {
        //current_bee_frame_index[bee_index] = current_bee_frame_index[bee_index]%BEE_FRAME_NUM;
        
        bee_up_window.w = s_bee_layer_info_ptr[bee_index][current_bee_frame_index[bee_index]].layer_width;
        bee_up_window.h = s_bee_layer_info_ptr[bee_index][current_bee_frame_index[bee_index]].layer_height;	
        
        if(bee_current_pose[bee_index].y<-bee_up_window.h)
        {
            bee_current_pose[bee_index].y = MMI_MAINSCREEN_HEIGHT-50;
#if BEE_NUM > 1
            do
            {
                bee_current_pose[bee_index].x = (rand()%(MMI_MAINSCREEN_WIDTH-bee_up_window.w));
            }while((bee_current_pose[BEE_NUM - 1].x <(bee_current_pose[BEE_NUM - 2].x + BIG_BEE_WIDTH))&&((bee_current_pose[BEE_NUM - 1].x + SMALL_BEE_WIDTH)>(bee_current_pose[BEE_NUM - 2].x)));
#endif
        }
        
        x = bee_current_pose[bee_index].x;
        y = bee_current_pose[bee_index].y;
        
        offset.x = x;
        offset.x_dec = 0;
        offset.y = y;
        offset.y_dec = 0;
        
        if (PNULL == s_bee_layer_info_ptr[bee_index][current_bee_frame_index[bee_index]].layer_buf_ptr)
        {
            return;
        }      
        S2D_Blending(bee_bg_window ,target_buf_ptr,bee_up_window ,offset,(uint32*)s_bee_layer_info_ptr[bee_index][current_bee_frame_index[bee_index]].layer_buf_ptr,target_buf_ptr);
        
        if(bee_index == 0)
        {
            bee_current_pose[bee_index].y -=3;
        }
        else
        {
            bee_current_pose[bee_index].y -=1;
        }
        //如果没有互动，则只循环播放前四帧                
        if(0== g_bee_flex_head_timer_id[bee_index])
        {
            if(0 == bee_current_pose[bee_index].y%WING_RATE)
            {
                current_bee_frame_index[bee_index]++;
            }
            current_bee_frame_index[bee_index] = current_bee_frame_index[bee_index]%(BEE_FRAME_NUM/2);
        }
        //如果是互动，则循环播放八帧
        else
        {
            //WING_RATE 是指移动两次走一帧翅膀，也就是走大的走六个像素，小的走两个
            if(0 == bee_current_pose[bee_index].y%WING_RATE)
            {
                current_bee_frame_index[bee_index]++;
            }
            current_bee_frame_index[bee_index] = current_bee_frame_index[bee_index]%BEE_FRAME_NUM;
            
            //总共播放三个循环，也就是伸三次头。
            if(g_bee_flex_head_timer_id[bee_index]<BEE_HEAD_MOVE_NUM*BEE_FRAME_NUM*WING_RATE)
            {
                g_bee_flex_head_timer_id[bee_index]++;
            }
            else
            {
                g_bee_flex_head_timer_id[bee_index] = 0;
            }
        }
    }
}

/*****************************************************************************/
//  Description : 蜜蜂刷新
//  Global resource dependence : 
//  Author:Gaily.Wang
//  Note:
/*****************************************************************************/
LOCAL void BeePopUpdate(COLOR16 *target_buf_ptr,uint16 scr_width, uint16 scr_height)
{ 	
    IMGREF_SIZE_T	pop_bg_window = {0,0};
    IMGREF_SIZE_T	pop_up_window = {0,0};
    FIX16_POINT_T	offset = {0};
    
    int32 pop_index = 0;	 
    int32 x = 0;
    int32 y = 0;
    
    if (PNULL == target_buf_ptr)
    {
        return;
    }
    pop_bg_window.w = scr_width;
    pop_bg_window.h = scr_height;
    
    for( pop_index = 0 ; pop_index < BEE_POP_NUM ; pop_index++)
    {
        pop_up_window.w = s_bee_pop_layer_info_ptr[pop_index].layer_width;
        pop_up_window.h = s_bee_pop_layer_info_ptr[pop_index].layer_height;
        g_BEE_POP[pop_index].a0 += g_BEE_POP[pop_index].p;
        g_BEE_POP[pop_index].a0 = (g_BEE_POP[pop_index].a0 %3600);
        x = g_BEE_POP[pop_index].r1*((s2d_sin(g_BEE_POP[pop_index].a0)>>8)*(s2d_cos(g_BEE_POP[pop_index].a)>>8))+g_BEE_POP[pop_index].r2*((s2d_cos(g_BEE_POP[pop_index].a0)>>8)*(s2d_sin(g_BEE_POP[pop_index].a)>>8)) + g_BEE_POP[pop_index].x0;
        y = g_BEE_POP[pop_index].r2*((s2d_cos(g_BEE_POP[pop_index].a0)>>8)*(s2d_cos(g_BEE_POP[pop_index].a)>>8))-g_BEE_POP[pop_index].r1*((s2d_sin(g_BEE_POP[pop_index].a0)>>8)*(s2d_sin(g_BEE_POP[pop_index].a)>>8)) + g_BEE_POP[pop_index].y0;
        offset.x = (int16)(x>>16) ;
        offset.x_dec = (uint16)(x&0xffff);
        offset.y = (int16)(y>>16) ;
        offset.y_dec = (uint16)(y&0xffff);
        if (PNULL == s_bee_pop_layer_info_ptr[pop_index].layer_buf_ptr)
        {
            return;
        }   
        S2D_Blending(pop_bg_window ,target_buf_ptr,pop_up_window ,offset,(uint32*)s_bee_pop_layer_info_ptr[pop_index].layer_buf_ptr,target_buf_ptr);
    }
    
    BeeUpdate(target_buf_ptr,scr_width, scr_height);    
}

/*****************************************************************************/
//  Description : 蜜蜂互动判断
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL void BeeHandleTpPressUp(uint16 point_x, uint16 point_y)
{
    int32 bee_index = 0;
    int32 x = 0;
    int32 y = 0;
    int32 w = 0;
    int32 h = 0;
    
    for(bee_index=0;bee_index<BEE_NUM;bee_index++)
    {
        x = bee_current_pose[bee_index].x;
        y = bee_current_pose[bee_index].y;
        w = s_bee_layer_info_ptr[bee_index][current_bee_frame_index[bee_index]].layer_width;
        h = s_bee_layer_info_ptr[bee_index][current_bee_frame_index[bee_index]].layer_height;
        if((point_x>x)&&(point_x<x+w)&&(point_y>y)&&(point_y<y+h))
        {
            g_bee_flex_head_timer_id[bee_index] = 1;
        }        
    }
}
/*****************************************************************************/
//  Description : 小蜜蜂初始化
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN BeeInit(void)
{
    BOOLEAN b_result = TRUE;
    int32 pop_index = 0;	 

    for( pop_index = 0 ; pop_index < BEE_POP_NUM ; pop_index++)
    {
        g_BEE_POP[pop_index].a = (rand()%3600);
        g_BEE_POP[pop_index].r1 = (rand()%40)+30;
        g_BEE_POP[pop_index].r2 = (rand()%100)+50;
        g_BEE_POP[pop_index].x0 = ((rand()%40)+(pop_index+5)*18)<<16;
        g_BEE_POP[pop_index].y0 = ((rand()%140)+(pop_index+5)*22)<<16;
        g_BEE_POP[pop_index].p = (rand()%5)+8;
        g_BEE_POP[pop_index].a0 = (rand()%3600);
    }
    //随机第一支蜜蜂的位置。30代表偏移
    bee_current_pose[0].y = (rand()%(MMI_MAINSCREEN_HEIGHT-30));
    bee_current_pose[0].x = (rand()%(MMI_MAINSCREEN_WIDTH-BIG_BEE_WIDTH));

#if BEE_NUM > 1
    //如果第二只蜜蜂的位置与第一支蜜蜂重叠，则重新随机位置
    do
    {
        bee_current_pose[BEE_NUM - 1].y = (rand()%(MMI_MAINSCREEN_HEIGHT-20));
        bee_current_pose[BEE_NUM - 1].x = (rand()%(MMI_MAINSCREEN_WIDTH-SMALL_BEE_WIDTH));
    }while((bee_current_pose[BEE_NUM - 1].x <(bee_current_pose[BEE_NUM - 2].x + BIG_BEE_WIDTH))&&((bee_current_pose[BEE_NUM - 1].x + SMALL_BEE_WIDTH)>(bee_current_pose[BEE_NUM - 2].x)));
#endif
    
    return b_result;
}

/*****************************************************************************/
//  Description : 小蜜蜂构造函数
//  Global resource dependence : 
//  Author:arvin.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN BeeConstruct(MMI_WIN_ID_T win_id)
{
    BOOLEAN b_result = FALSE;
    UILAYER_CREATE_T create_info = {0}; 
    int16 pop_index = 0;
    int16 bee_index = 0;
    int16 bee_frame_index = 0;
    GUI_POINT_T  display_point = {0,0};
    UILAYER_INFO_T layer_info = {0};

    const MMI_IMAGE_ID_T  bee_pop_image_id[BEE_POP_NUM] = 
    {
        IMAGE_WALLPAPER_BEE_0,
        IMAGE_WALLPAPER_BEE_1,
        IMAGE_WALLPAPER_BEE_2
    }; 
    
    for(pop_index=0;pop_index<BEE_POP_NUM;pop_index++)
    {
        /*先创建一个层*/
        s_bee_pop_layer_handle[pop_index].lcd_id = 0;
        s_bee_pop_layer_handle[pop_index].block_id = UILAYER_NULL_HANDLE;

        create_info.lcd_id = GUI_MAIN_LCD_ID;
        create_info.owner_handle = (MMI_HANDLE_T)win_id;
        create_info.offset_x = 0;
        create_info.offset_y = 0;		
        GUIRES_GetImgWidthHeight((uint16*)&create_info.width, (uint16*)&create_info.height, bee_pop_image_id[pop_index], win_id);
        create_info.is_bg_layer = FALSE;
        create_info.is_static_layer = FALSE; 
    	create_info.func_ptr = MMIAPILIVEWALLPAPER_UilayerMemPoolFunc(); 
        if(UILAYER_RESULT_SUCCESS != UILAYER_CreateLayer(
            &create_info,
            &s_bee_pop_layer_handle[pop_index]
            ))
        {
              b_result = FALSE;
              return b_result;
        }
        // 设置color key
        UILAYER_SetLayerColorKey(&s_bee_pop_layer_handle[pop_index], TRUE, UILAYER_TRANSPARENT_COLOR);

        // 使用color清除层
        UILAYER_Clear(&s_bee_pop_layer_handle[pop_index]);

        //把图片画到层上
        GUIRES_DisplayImg(&display_point,
            PNULL,
            PNULL,
            win_id,
            bee_pop_image_id[pop_index],
            (const GUI_LCD_DEV_INFO*)&s_bee_pop_layer_handle[pop_index]);
        //获取图片的信息
        UILAYER_GetLayerInfo(&s_bee_pop_layer_handle[pop_index], &layer_info);
        s_bee_pop_layer_info_ptr[pop_index].layer_width = layer_info.layer_width;
        s_bee_pop_layer_info_ptr[pop_index].layer_height = layer_info.layer_height;
        s_bee_pop_layer_info_ptr[pop_index].mem_width = layer_info.mem_width;
        s_bee_pop_layer_info_ptr[pop_index].data_type = layer_info.data_type;
        s_bee_pop_layer_info_ptr[pop_index].layer_buf_ptr = UILAYER_GetLayerBufferPtr(&s_bee_pop_layer_handle[pop_index]);       
    }
    
    for(bee_index = 0;bee_index<BEE_NUM;bee_index++)
    {
        for(bee_frame_index=0;bee_frame_index<BEE_FRAME_NUM;bee_frame_index++)
        {
            /*先创建一个层*/
            s_bee_layer_handle[bee_index][bee_frame_index].lcd_id = 0;
            s_bee_layer_handle[bee_index][bee_frame_index].block_id = UILAYER_NULL_HANDLE;

            create_info.lcd_id = GUI_MAIN_LCD_ID;
            create_info.owner_handle = win_id;
            create_info.offset_x = 0;
            create_info.offset_y = 0;
            GUIRES_GetImgWidthHeight((uint16*)&create_info.width, (uint16*)&create_info.height, bee_image_id[bee_index][bee_frame_index], win_id);
            create_info.is_bg_layer = FALSE;
            create_info.is_static_layer = FALSE; 
    	    create_info.func_ptr = MMIAPILIVEWALLPAPER_UilayerMemPoolFunc(); 
            if(UILAYER_RESULT_SUCCESS != UILAYER_CreateLayer(
                &create_info,
                &s_bee_layer_handle[bee_index][bee_frame_index]
                ))	
            {
                  b_result = FALSE;
                  return b_result;
            }
            // 设置color key
            UILAYER_SetLayerColorKey(&s_bee_layer_handle[bee_index][bee_frame_index], TRUE, UILAYER_TRANSPARENT_COLOR);

            // 使用color清除层
            UILAYER_Clear(&s_bee_layer_handle[bee_index][bee_frame_index]);

            //把图片画到层上
            GUIRES_DisplayImg(&display_point,
                PNULL,
                PNULL,
                win_id,
                bee_image_id[bee_index][bee_frame_index],
                (const GUI_LCD_DEV_INFO*)&s_bee_layer_handle[bee_index][bee_frame_index]);
            //获取图片的信息
            UILAYER_GetLayerInfo(&s_bee_layer_handle[bee_index][bee_frame_index], &layer_info);
            s_bee_layer_info_ptr[bee_index][bee_frame_index].layer_width = layer_info.layer_width;
            s_bee_layer_info_ptr[bee_index][bee_frame_index].layer_height = layer_info.layer_height;
            s_bee_layer_info_ptr[bee_index][bee_frame_index].mem_width = layer_info.mem_width;
            s_bee_layer_info_ptr[bee_index][bee_frame_index].data_type = layer_info.data_type;
            s_bee_layer_info_ptr[bee_index][bee_frame_index].layer_buf_ptr = UILAYER_GetLayerBufferPtr(&s_bee_layer_handle[bee_index][bee_frame_index]);
        }        
    }    
    b_result = TRUE;
    return b_result;
}

/*****************************************************************************/
//  Description : 小蜜蜂准备背景数据
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN BeeDrawBg(MMI_WIN_ID_T win_id,const GUI_LCD_DEV_INFO *dev_info_ptr)
{
    BOOLEAN b_result = FALSE;
    GUI_POINT_T display_point = {0,0};

    if (PNULL != dev_info_ptr)
    {
        b_result = GUIRES_DisplayImg(&display_point, 
            PNULL, 
            PNULL, 
            win_id, 
            IMAGE_WALLPAPER_BEE_BG, 
            dev_info_ptr);
    }
    return b_result;
}
/*****************************************************************************/
//  Description : 小蜜蜂算法
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL void BeeCalculate(LW_CAL_PARAM *lw_cal_param)
{

    if(PNULL == lw_cal_param || PNULL == lw_cal_param->dest_buf_info.buf_ptr || PNULL == lw_cal_param->src_buf_info.buf_ptr)
    {
        return;
    }
    SCI_MEMCPY((GUI_COLOR_T*)lw_cal_param->dest_buf_info.buf_ptr, lw_cal_param->src_buf_info.buf_ptr,MMI_MAINSCREEN_WIDTH*MMI_MAINSCREEN_HEIGHT*sizeof(uint16));
    
    BeePopUpdate((COLOR16*)lw_cal_param->dest_buf_info.buf_ptr, lw_cal_param->src_buf_info.w,lw_cal_param->src_buf_info.h);
}
/*****************************************************************************/
//  Description : 小蜜蜂释放
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:销毁临时申请的变量
/*****************************************************************************/
LOCAL void BeeDestruct(void)
{	
    int16 pop_index = 0;
    int16 bee_index = 0;
    int16 bee_frame_index = 0;
    
    for(pop_index=0;pop_index<BEE_POP_NUM;pop_index++)
    {
        UILAYER_RELEASELAYER(&s_bee_pop_layer_handle[pop_index]);   /*lint !e506 !e774*/
//        UILAYER_ReleaseLayer(&s_bee_pop_layer_handle[pop_index]);
//        s_bee_pop_layer_handle[pop_index].block_id = UILAYER_NULL_HANDLE;
    }
    
    for(bee_index = 0;bee_index<BEE_NUM;bee_index++)
    {
        for(bee_frame_index=0;bee_frame_index<BEE_FRAME_NUM;bee_frame_index++)
        {
            UILAYER_RELEASELAYER(&s_bee_layer_handle[bee_index][bee_frame_index]);   /*lint !e506 !e774*/
//            UILAYER_ReleaseLayer(&s_bee_layer_handle[bee_index][bee_frame_index]);
//            s_bee_layer_handle[bee_index][bee_frame_index].block_id = UILAYER_NULL_HANDLE;
        }        
    }
}
/*****************************************************************************/
//  Description : 小蜜蜂交互事件处理
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN BeeHandleEvent(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    GUI_POINT_T   point       = {0}; 
    
    switch(msg_id)
    {
    case MSG_TP_PRESS_UP:
        point.x = MMK_GET_TP_X(param);
        point.y = MMK_GET_TP_Y(param);
        BeeHandleTpPressUp(point.x,point.y);
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
LOCAL uint32 BeeGetType(void)
{
    return MMI_LIVE_WALLPAPER_TYPE_BEE;
}
/*****************************************************************************/
//  Description : 获得动态壁纸ThumbNail信息
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL void BeeGetThumbNailInfo(LW_THUMBNAIL_INFO *thumbnail_info_ptr)
{
    if(thumbnail_info_ptr)
    {
        thumbnail_info_ptr->is_image_file = FALSE;
        thumbnail_info_ptr->image_id = IMAGE_WALLPAPER_BEE_THUMBNAIL;
        thumbnail_info_ptr->text_id = TXT_SET_WALLPAPER_BEE;

        thumbnail_info_ptr->is_not_string = FALSE;
        thumbnail_info_ptr->instance_mem_pool_size = BLOCK_MEM_POOL_SIZE_BEE;
    }
}
#endif

#endif /*_MMI_LIVEWALLPAPER_BEE_C_*/


/*Edit by script, ignore 1 case. Thu Apr 26 19:00:57 2012*/ //IGNORE9527
