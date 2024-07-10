

/*****************************************************************************
** File Name:      mmi_livewallpaper_ripple.c                                    *
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
#ifndef _MMI_LIVEWALLPAPER_RIPPLE_C_
#define _MMI_LIVEWALLPAPER_RIPPLE_C_

#ifdef LIVE_WALLPAPER_SUPPORT_RIPPLE
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
#define LEAF_TYPE_NUM 7  //叶子类型数目
#define LEAF_NUM 4       //叶子在屏幕上面的数目
#define Z_MAX 4096       //Z方向最大值
#define Z_DOWN   64      //z方向每一帧的变化值
#define Y_ADD   1        //y方向每一帧的变化值
#define LEAF_ANGLE_CHANGE 10    //叶子每一帧转动的角度
#define RIPPLE_BG_DIFF_SIZE  40  //算法需要背景高度增加40
#define MEMORY_FOR_ALGORITHM  2048    //分配2048大小的空间供算法计算使用,单位是sizeof(FIX16_POINT_T)
#define ANGLE_OF_LEAF  3600     //为了树叶旋转精密计算需要，将角度的度量衡扩大10倍。
#define BLOCK_MEM_POOL_SIZE_RIPPLE (350 * 1024)   // Set a  MEM POOL for different livewallpaper respectively;

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/
typedef struct
{
    int32 x;
    int32 y;
    int32 angle;
    int32 type;
    int32 z;
}LEAF_INFO_T;

/**--------------------------------------------------------------------------*
 **                         LOCAL DEFINITION                                 *
 **--------------------------------------------------------------------------*/
LOCAL LIVEWALLPAPER_UILAYER_INFO_T s_leaf_layer_info_ptr[LEAF_TYPE_NUM] = {0};
LOCAL GUI_LCD_DEV_INFO s_leaf_layer_handle[LEAF_TYPE_NUM] = {0}; //图片层
LOCAL LEAF_INFO_T s_leaf_param[LEAF_NUM] = {0};
LOCAL FIX16_POINT_T* s_point_ptr = PNULL;//分配2048大小的空间供算法计算使用

LOCAL MMI_IMAGE_ID_T  s_leaf_image_id[LEAF_TYPE_NUM] = {
IMAGE_WALLPAPER_LEAF_0,
IMAGE_WALLPAPER_LEAF_1,
IMAGE_WALLPAPER_LEAF_2,
IMAGE_WALLPAPER_LEAF_3,
IMAGE_WALLPAPER_LEAF_4,
IMAGE_WALLPAPER_LEAF_5,
IMAGE_WALLPAPER_LEAF_6,
 }; 
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
LOCAL BOOLEAN RippleInit(void);
/*****************************************************************************/
//  Description : 初始化
//  Global resource dependence : 
//  Author:arvin.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN RippleConstruct(MMI_WIN_ID_T win_id);

/*****************************************************************************/
//  Description : 准备背景数据
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN RippleDrawBg(MMI_WIN_ID_T win_id,const GUI_LCD_DEV_INFO *dev_info_ptr);

/*****************************************************************************/
//  Description : 算法
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL void RippleCalculate(LW_CAL_PARAM *lw_cal_param);
/*****************************************************************************/
//  Description : 释放
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:销毁临时申请的变量
/*****************************************************************************/
LOCAL void RippleDestruct(void);
/*****************************************************************************/
//  Description : 交互事件处理
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN RippleHandleEvent(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
/*****************************************************************************/
//  Description : TP UP事件处理
//  Global resource dependence : 
//  Author:Gaily.Wang
//  Note:
/*****************************************************************************/
LOCAL void Ripple_Tp_Up(DPARAM param);
/*****************************************************************************/
//  Description : 波纹更新
//  Global resource dependence : 
//  Author:Gaily.Wang
//  Note:
/*****************************************************************************/
LOCAL void Ripple_WaveUpdate(COLOR16 *target_buf_ptr, uint16 scr_width, uint16 scr_height,uint16*source_buf_ptr);
/*****************************************************************************/
//  Description : 根据Z排序
//  Global resource dependence : 
//  Author:Gaily.Wang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN Ripple_Leaf_Z_Sort(int32 *temp_array,int32 iLen);
/*****************************************************************************/
//  Description : 叶子更新
//  Global resource dependence : 
//  Author:Gaily.Wang
//  Note:
/*****************************************************************************/
LOCAL void Ripple_LeafUpdate(COLOR16 *target_buf_ptr, uint16 scr_width, uint16 scr_height);

/*****************************************************************************/
//  Description : 获得动态壁纸类型
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL uint32 RippleGetType(void);
/*****************************************************************************/
//  Description : 获得动态壁纸ThumbNail信息
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL void RippleGetThumbNailInfo(LW_THUMBNAIL_INFO *thumbnail_info_ptr);
/*****************************************************************************/
//  Description : 获得动态壁纸源数据宽高
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL void RippleGetWidthAndHeight(uint16 *width_ptr,uint16 *height_ptr);

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/
PUBLIC const MMI_LIVE_WALLPAPER_ITEM_INFO_T g_ripple_Instance =
{
    RippleInit,
    RippleConstruct,
    RippleDrawBg,
    RippleCalculate,
    RippleDestruct,
    RippleHandleEvent,
    PNULL,
    RippleGetType,
    RippleGetThumbNailInfo,
    RippleGetWidthAndHeight,
    PNULL,
    PNULL,
    PNULL
};
/*---------------------------------------------------------------------------*/
/*                          FUNCTION DEFINITION                                                              */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : 静态数据初始化
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN RippleInit(void)
{
    BOOLEAN b_result = TRUE;
    int32 leaf_index = 0;
    
        
    for(leaf_index = 0; leaf_index < LEAF_NUM; leaf_index++)
    {
        s_leaf_param[leaf_index].x = rand()%MMI_MAINSCREEN_WIDTH;
        s_leaf_param[leaf_index].y = (rand()%(MMI_MAINSCREEN_HEIGHT-RIPPLE_BG_DIFF_SIZE * 2))+RIPPLE_BG_DIFF_SIZE;
        s_leaf_param[leaf_index].angle = rand()%ANGLE_OF_LEAF;
        s_leaf_param[leaf_index].type = rand()%LEAF_TYPE_NUM;
        s_leaf_param[leaf_index].z = ((Z_MAX*4)*(leaf_index+1)/LEAF_NUM)+(Z_MAX*2)/LEAF_NUM - (rand()%((Z_MAX*4)/LEAF_NUM));
    }

   
    S2D_RippleInit();
    return b_result;
}

/*****************************************************************************/
//  Description :初始化
//  Global resource dependence : 
//  Author:arvin.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN RippleConstruct(MMI_WIN_ID_T win_id)
{
    UILAYER_CREATE_T create_info = {0}; 
    int16 leaf_index = 0;
    GUI_POINT_T display_point = {0,0};
    UILAYER_INFO_T layer_info = {0};
    BOOLEAN b_result = FALSE;

    for(leaf_index=0;leaf_index<LEAF_TYPE_NUM;leaf_index++)
    {

        /*先创建一个层*/
        s_leaf_layer_handle[leaf_index].lcd_id = 0;
        s_leaf_layer_handle[leaf_index].block_id = UILAYER_NULL_HANDLE;

        create_info.lcd_id = GUI_MAIN_LCD_ID;
        create_info.owner_handle = (MMI_HANDLE_T)win_id;
        create_info.offset_x = 0;
        create_info.offset_y = 0;
        GUIRES_GetImgWidthHeight((uint16*)&create_info.width, (uint16*)&create_info.height, s_leaf_image_id[leaf_index], win_id);
        create_info.is_bg_layer = FALSE;
        create_info.is_static_layer = FALSE; 
        create_info.func_ptr = MMIAPILIVEWALLPAPER_UilayerMemPoolFunc(); 
        if(UILAYER_RESULT_SUCCESS != UILAYER_CreateLayer(
        					&create_info,
        					&s_leaf_layer_handle[leaf_index]
        					))
        {
            return b_result;
        }
        // 设置color key
        UILAYER_SetLayerColorKey(&s_leaf_layer_handle[leaf_index], TRUE, UILAYER_TRANSPARENT_COLOR);

        // 使用color清除层
        UILAYER_Clear(&s_leaf_layer_handle[leaf_index]);

        //把图片画到层上
        GUIRES_DisplayImg(&display_point,
        				PNULL,
        				PNULL,
        				win_id,
        				s_leaf_image_id[leaf_index],
        				(const GUI_LCD_DEV_INFO*)&s_leaf_layer_handle[leaf_index]);
        //获取图片的信息
        UILAYER_GetLayerInfo(&s_leaf_layer_handle[leaf_index], &layer_info);
        s_leaf_layer_info_ptr[leaf_index].layer_width = layer_info.layer_width;
        s_leaf_layer_info_ptr[leaf_index].layer_height = layer_info.layer_height;
        s_leaf_layer_info_ptr[leaf_index].mem_width = layer_info.mem_width;
        s_leaf_layer_info_ptr[leaf_index].data_type = layer_info.data_type;
        s_leaf_layer_info_ptr[leaf_index].layer_buf_ptr = UILAYER_GetLayerBufferPtr(&s_leaf_layer_handle[leaf_index]);       

    }

    s_point_ptr = (FIX16_POINT_T*)MMILIVEWALLPAPER_ALLOC(MEMORY_FOR_ALGORITHM * sizeof(FIX16_POINT_T));
    if(PNULL == s_point_ptr)
    {
        return b_result;
    }
    
    b_result = TRUE;
    return b_result;
}

/*****************************************************************************/
//  Description : 准备背景数据
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN RippleDrawBg(MMI_WIN_ID_T win_id,const GUI_LCD_DEV_INFO *dev_info_ptr)
{
    BOOLEAN b_result = FALSE;
    GUI_POINT_T display_point = {0,0};

    if(PNULL == dev_info_ptr)
    {
        return FALSE;
    }
    else
    {
        b_result = GUIRES_DisplayImg(&display_point, 
                                            PNULL, 
                                            PNULL, 
                                            win_id, 
                                            IMAGE_WALLPAPER_RIPPLE_BG, 
                                            dev_info_ptr);
    }
    return b_result;
}

/*****************************************************************************/
//  Description : 算法
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL void RippleCalculate(LW_CAL_PARAM *lw_cal_param)
{
    uint16 *target_buf_ptr = PNULL;
    uint16 *src_buf_ptr = PNULL;
    
    if(PNULL == lw_cal_param || PNULL == lw_cal_param->dest_buf_info.buf_ptr || PNULL == lw_cal_param->src_buf_info.buf_ptr)
    {
        return;
    }
    src_buf_ptr = (uint16*)lw_cal_param->src_buf_info.buf_ptr;
    target_buf_ptr = (uint16*)lw_cal_param->dest_buf_info.buf_ptr;
    lw_cal_param->src_buf_info.h = lw_cal_param->src_buf_info.h -RIPPLE_BG_DIFF_SIZE;
    src_buf_ptr = (uint16*)(src_buf_ptr + (RIPPLE_BG_DIFF_SIZE/2) *lw_cal_param->src_buf_info.w);

    
    SCI_MEMCPY(target_buf_ptr,src_buf_ptr,sizeof(uint16)*lw_cal_param->src_buf_info.w*lw_cal_param->src_buf_info.h);
    
    Ripple_WaveUpdate((GUI_COLOR_T*)target_buf_ptr,lw_cal_param->src_buf_info.w,lw_cal_param->src_buf_info.h,src_buf_ptr);

    Ripple_LeafUpdate((GUI_COLOR_T*)target_buf_ptr,lw_cal_param->src_buf_info.w,lw_cal_param->src_buf_info.h);   
}
/*****************************************************************************/
//  Description : 释放
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL void RippleDestruct(void)
{
    int16 leaf_index = 0;

    for(leaf_index=0;leaf_index<LEAF_TYPE_NUM;leaf_index++)
    {
        UILAYER_RELEASELAYER(&s_leaf_layer_handle[leaf_index]);   /*lint !e506 !e774*/
//        UILAYER_ReleaseLayer(&s_leaf_layer_handle[leaf_index]);
//        s_leaf_layer_handle[leaf_index].block_id = UILAYER_NULL_HANDLE;
    }
    if(PNULL != s_point_ptr)
    {
        MMILIVEWALLPAPER_FREE(s_point_ptr);//SCI_FREE
    }
}

/*****************************************************************************/
//  Description :交互事件处理
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN RippleHandleEvent(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{  
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    switch(msg_id)
    {
        case MSG_TP_PRESS_UP:
            Ripple_Tp_Up(param);
            break;

        default:
            return FALSE;
    }
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    
    return TRUE;
}


/*****************************************************************************/
//  Description : 互动判断
//  Global resource dependence : 
//  Author:Gaily.Wang
//  Note:
/*****************************************************************************/
LOCAL void Ripple_Tp_Up(DPARAM param)
{

    IMGREF_POINT_T     point       = {0};

    point.x = MMK_GET_TP_X(param);
    point.y = MMK_GET_TP_Y(param);
    
    if((point.x>0)&&(point.x<MMI_MAINSCREEN_WIDTH)&&(point.y>0)&&(point.y<MMI_MAINSCREEN_HEIGHT))
    {
        S2D_RippleSet(point);
    }
}
LOCAL void Ripple_WaveUpdate(COLOR16 *target_buf_ptr, uint16 scr_width, uint16 scr_height,uint16*source_buf_ptr)
{
    if(PNULL == target_buf_ptr || PNULL == source_buf_ptr)
    {
        return;
    }
    
   S2D_RippleUpdate(target_buf_ptr,source_buf_ptr,scr_width, scr_height);  
}

LOCAL BOOLEAN Ripple_Leaf_Z_Sort(int32 *temp_array,int32 iLen)
{
	uint16 i = 0;
	uint16 j = 0;
	BOOLEAN isChanged = SCI_TRUE;
	uint16 Geomrtry_Temp = 0;

        if(PNULL == temp_array )
        {
            return SCI_FALSE;
        }

	for( i = 0 ; i < iLen-1 ; i ++)
	{
		if(isChanged)
		{
			isChanged = SCI_FALSE;
			for( j = 1 ; j < iLen - i ; j ++)
			{
				if(s_leaf_param[temp_array[j-1]].z<s_leaf_param[temp_array[j]].z)
				{
				}
				else
				{
					isChanged = SCI_TRUE;
					Geomrtry_Temp = temp_array[j-1];
					temp_array[j-1] = temp_array[j];
					temp_array[j] = Geomrtry_Temp;
				}
			}
		}
	}
	return SCI_TRUE;
}

/*****************************************************************************/
//  Description : LEAF 刷新
//  Global resource dependence : 
//  Author:Gaily.Wang
//  Note:
/*****************************************************************************/
LOCAL void Ripple_LeafUpdate(COLOR16 *target_buf_ptr, uint16 scr_width, uint16 scr_height)
{
    int leaf_index = 0;
    IMGREF_SIZE_T	leaf_bg_window = {0,0};
    IMGREF_SIZE_T	leaf_up_window = {0,0};
    IMGREF_POINT_T window_mid = {0,0};
    IMGREF_POINT_T img_mid = {0,0}; 
    int temp_z = 0;
    int32 temp_leaf_array[LEAF_NUM];
    int i=0;

    if(PNULL == target_buf_ptr )
    {
        return;
    }
        
    leaf_bg_window.w = scr_width;
    leaf_bg_window.h = scr_height;

    for(i=0;i<LEAF_NUM;i++)
    {
        temp_leaf_array[i] = i;
    }

    Ripple_Leaf_Z_Sort(temp_leaf_array,LEAF_NUM);
    
    for(leaf_index = 0; leaf_index < LEAF_NUM; leaf_index++)
    {
        //如果越界屏幕外，则重新初始化
        if(scr_height <= s_leaf_param[temp_leaf_array[leaf_index]].y)
        {
            s_leaf_param[temp_leaf_array[leaf_index]].x = rand()%MMI_MAINSCREEN_WIDTH;
            s_leaf_param[temp_leaf_array[leaf_index]].y = (rand()%(MMI_MAINSCREEN_HEIGHT-RIPPLE_BG_DIFF_SIZE * 2))+RIPPLE_BG_DIFF_SIZE;
            s_leaf_param[temp_leaf_array[leaf_index]].angle = rand()%ANGLE_OF_LEAF;
            s_leaf_param[temp_leaf_array[leaf_index]].type = rand()%LEAF_TYPE_NUM;
            s_leaf_param[temp_leaf_array[leaf_index]].z =(Z_MAX*4);//((Z_MAX*4)*(leaf_index+1)/LEAF_NUM)+(Z_MAX*2)/LEAF_NUM - (rand()%((Z_MAX*4)/LEAF_NUM));
        }
        //是否已经落入水面
        if(s_leaf_param[temp_leaf_array[leaf_index]].z > Z_MAX*3)
        {
            window_mid.x = s_leaf_param[temp_leaf_array[leaf_index]].x;
            window_mid.y = s_leaf_param[temp_leaf_array[leaf_index]].y;
            
            if((s_leaf_param[temp_leaf_array[leaf_index]].z-Z_DOWN) <= Z_MAX*3)
            {
                //是震动点
                if((window_mid.x>0)&&(window_mid.x<MMI_MAINSCREEN_WIDTH)&&(window_mid.y>0)&&(window_mid.y<MMI_MAINSCREEN_HEIGHT))
                {
                    S2D_RippleSet(window_mid);
                }
                temp_z = Z_MAX/2;
            }
            else
            {
                temp_z = s_leaf_param[temp_leaf_array[leaf_index]].z/2-Z_MAX;
            }
        }
        else
        {
            s_leaf_param[temp_leaf_array[leaf_index]].y += Y_ADD;
            window_mid.x = s_leaf_param[temp_leaf_array[leaf_index]].x;
            window_mid.y = s_leaf_param[temp_leaf_array[leaf_index]].y;
            temp_z = Z_MAX/2;
        }
        
        s_leaf_param[temp_leaf_array[leaf_index]].angle -=LEAF_ANGLE_CHANGE;//转动的度数 
        s_leaf_param[temp_leaf_array[leaf_index]].z -= Z_DOWN;
        
        if(s_leaf_param[temp_leaf_array[leaf_index]].angle<=0)
        {
            s_leaf_param[temp_leaf_array[leaf_index]].angle += ANGLE_OF_LEAF;
        }
        
        leaf_up_window.w = s_leaf_layer_info_ptr[s_leaf_param[temp_leaf_array[leaf_index]].type].layer_width;
        leaf_up_window.h = s_leaf_layer_info_ptr[s_leaf_param[temp_leaf_array[leaf_index]].type].layer_height;

        img_mid.x = (leaf_up_window.w-1)/2;
        img_mid.y = (leaf_up_window.h-1)/2;
        
            
        S2d_rotation_slim(leaf_bg_window,
        				leaf_up_window,
        				window_mid,
        				img_mid,
        				s_leaf_param[temp_leaf_array[leaf_index]].angle,
        				(COLOR32 *)s_leaf_layer_info_ptr[s_leaf_param[temp_leaf_array[leaf_index]].type].layer_buf_ptr,
        				target_buf_ptr,
        				temp_z,
        				s_point_ptr
        				);

   }
}
/*****************************************************************************/
//  Description : 获得动态壁纸类型
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL uint32 RippleGetType(void)
{
    return MMI_LIVE_WALLPAPER_TYPE_RIPPLE;
}
/*****************************************************************************/
//  Description : 获得动态壁纸ThumbNail信息
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL void RippleGetThumbNailInfo(LW_THUMBNAIL_INFO *thumbnail_info_ptr)
{
    if(thumbnail_info_ptr)
    {
        thumbnail_info_ptr->is_image_file = FALSE;
        thumbnail_info_ptr->image_id = IMAGE_WALLPAPER_RIPPLE_THUMBNAIL;

        thumbnail_info_ptr->is_not_string = FALSE;
        thumbnail_info_ptr->text_id = TXT_SET_WALLPAPER_RIPPLE;
        thumbnail_info_ptr->instance_mem_pool_size = BLOCK_MEM_POOL_SIZE_RIPPLE;
    }
}
/*****************************************************************************/
//  Description : 获得动态壁纸源数据宽高
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL void RippleGetWidthAndHeight(uint16 *width_ptr,uint16 *height_ptr)
{
    if(PNULL != width_ptr )
    {
        *width_ptr = MMI_MAINSCREEN_WIDTH;
    }

    if(PNULL != width_ptr )
    {
        *height_ptr = MMI_MAINSCREEN_HEIGHT+RIPPLE_BG_DIFF_SIZE;
    }
}
#endif //LIVE_WALLPAPER_SUPPORT_RIPPLE
#endif //_MMI_LIVEWALLPAPER_RIPPLE_C_


/*Edit by script, ignore 1 case. Thu Apr 26 19:00:58 2012*/ //IGNORE9527
