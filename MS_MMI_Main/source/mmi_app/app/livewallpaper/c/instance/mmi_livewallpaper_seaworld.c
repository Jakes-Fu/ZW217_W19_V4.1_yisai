/*****************************************************************************
** File Name:      .c                                            *
** Author:                                                                   *
** Date:                                                          *
** Copyright:      2004 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is for live wallpaper                  *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 08/2011      Listen.Li       Create                                  *
******************************************************************************/
#define _MMI_LIVEWALLPAPER_SEAWORLD_C_

#include "mmi_app_livewallpaper_trc.h"
#ifdef _MMI_LIVEWALLPAPER_SEAWORLD_C_
#ifdef LIVE_WALLPAPER_SUPPORT_SEAWORLD
/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "graphics_draw.h"
#include "mmi_image.h"
#include "mmiset_text.h"
#ifdef MMI_GRID_IDLE_SUPPORT
#include "mmiwidget_nv.h"
#endif
#include "mmi_livewallpaper_image.h"
#include "mmi_livewallpaper_text.h"
#include "mmi_livewallpaper_seaworld.h"
#include "mmilivewallpaper_export.h"
#include "mmi_livewallpaper_option.h"
#include "mmisrvaud_api.h"
#include "mmi_text.h"
#include "mmiset_export.h"
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define  BUBBLE_TOTAL    40
#define  BUBBLE_IMG_TOTAL    3
#define  SUNSHINE_TOTAL    12
#define  FISH_TOTAL    14
#define  FISH_MOVING_MAX     10     // 当前屏幕上鱼的最大数量
#define  FISH_ROTATE_RADIUS    132    // 旋转buffer
#define  FISH_DECODE_RADIUS    66    // 解码buffer
#ifdef MAINLCD_SIZE_320X480
#define  FISH_TOUCH_MAX_SPEED    40
#else
#define  FISH_TOUCH_MAX_SPEED    30
#endif
#define  FISH_ADD_MAX    3      /*每次最多添加的鱼的条数*/
#define BLOCK_MEM_POOL_SIZE_SEAWORLD (250 * 1024)   // Set a  MEM POOL for different livewallpaper respectively;
#define  FISH_VIBRATE_TIME       50

typedef struct 
{
    const void				*data_ptr;      
    uint32					data_size;
    IMGREF_DATA_FORMAT_E    data_type;      
    uint16					width;             
    uint16					height;
    int16					center_x;       //horizontal coordinate of rotation center 
    int16					center_y;       //vertical coordinate of rotation center 
}SEAWORLD_ROTATE_IMAGE_T;

typedef    SEAWORLD_ROTATE_IMAGE_T    MIRROR_IMAGE_T;

/*---------------------------------------------------------------------------*/

/*
    正弦曲线
    y = A * sin(wx + t) + k;
    A: 振幅
    w: 控制单位角度内周期(单位角度内震动的次数，
          在其他条件不变的条件下，w越大图像越尖，w越小图像越平) 
    t: 为相偏移（横轴左右）。
    k: 偏距，反应在坐标系上的图像为图像的整体上移或下移
    x: 为x轴弧度
    y: 为y轴值


注意:
    Sin bubble中，y轴为MainScreenRightPixel方向
    Sin bubble中可以将公式简化为y = A * sin(wx);
*/

typedef struct
{
    MMI_IMAGE_ID_T   img;
    MMI_IMAGE_ID_T   tpdown_img;
    int16    left;
    int16    top;
    int16    start_x;
    int16    start_y;
    uint16    index;    // 索引

    int16    A;    //振幅
    int16    w;    //控制单位角度内周期(单位角度内震动的次数，在其他条件不变的条件下，w越大图像越尖，w越小图像越平)    
    int16    t;    //为相偏移（横轴左右）。
    int16    k;    //偏距，反应在坐标系上的图像为图像的整体上移或下移
    int16    angle;    //角度，不是弧度
    int16    yValue;    //angle角度对应的当前值

    int16    angle_v;    // angle增加的速度
    BOOLEAN    direction;    // 气泡起始漂移的方向
    
    BOOLEAN    is_over;    //在某个s_bubble_startPos是否已经显示完整个正弦轨迹
}SIN_BUBBLE_INFO_T;

typedef struct
{
    uint16    index;    // 索引
    int16    left;    // 起始位置
    int16    top; 
    uint16    length;    // 长度
    int16    speed;    //移动速度
    int16    range_start;    // 可以移动的范围
    int16    range_end;
    BOOLEAN   direction;    // TRUE:向右移动;FALSE:向左移动
}SEAWORLD_SUNSHINE_T;

typedef enum
{
    GROUP_FISH_MOVE_NORMAL = 0,    // 很多不同种类的鱼同时游动。
    GROUP_FISH_MOVE_SAME,    // 同一种鱼按照相同角度游过
    GROUP_FISH_MOVE_CROSS,   //两组鱼平行相向穿过

    GROUP_FISH_MOVE_TYPE_MAX
}GROUP_FISH_MOVE_TYPE_E;

typedef enum
{
    SINGLE_FISH_MOVE_LINEAR,    // 线性
    SINGLE_FISH_MOVE_CIRCLE,    // 画一个圈，实际上市从A°> 360° 再至当前A°
    SINGLE_FISH_MOVE_HALF_CIRCLE,    //半圆
    SINGLE_FISH_MOVE_WAVE,    // 波浪形前进

    SINGLE_FISH_MOVE_TYPE_MAX
}SINGLE_FISH_MOVE_TYPE_E;

typedef enum
{
    WAVE_ANGLE_INCREASE,
    WAVE_ANGLE_DECREASE
}SINGLE_FISH_MOVE_WAVE_TYPE_E;

typedef struct
{
    void *data_buf;
    uint32 data_size;
    MMI_IMAGE_ID_T img_id;
    
    uint16 index;
    int16 left;
    int16 top;
    uint16 width;
    uint16 height;
    int16 angle;
    uint16 rotate_width;
    uint16 rotate_height;
    uint16 speed;    //移动速度

    BOOLEAN is_moving;    // 当前是否在屏幕上游动
    BOOLEAN direction;    // TRUE:向右移动;FALSE:向左移动
    BOOLEAN is_touch;    // 是否被点中

    SINGLE_FISH_MOVE_TYPE_E move_type;

    SINGLE_FISH_MOVE_WAVE_TYPE_E wave_type;
}SEAWORLD_FISH_T;


/**--------------------------------------------------------------------------*
 **                         LOCAL DEFINITION                                 *
 **--------------------------------------------------------------------------*/
/* bubble */
LOCAL GUI_POINT_T  s_bubble_startPos;
LOCAL SIN_BUBBLE_INFO_T *s_bubble_array = PNULL;
LOCAL GUI_LCD_DEV_INFO s_bubble_layer[BUBBLE_IMG_TOTAL] = {0};
/*该参数用来标记当前所有的气泡是否已经全部显示完毕。
如果全部显示，则重新设置s_bubble_startPos的值，否则不设置*/
LOCAL uint16 s_bubble_change_startPos_flag = 0;

/* sunshine */
LOCAL SEAWORLD_SUNSHINE_T *s_sunshine_array = PNULL;
LOCAL GUI_LCD_DEV_INFO s_sunshine_layer = {0};

/* fish */
LOCAL SEAWORLD_FISH_T *s_fish_array = PNULL;
LOCAL uint8* s_rotate_buf_ptr = PNULL;    // 旋转用到的buffer. 目前申请大小为128*128*4
LOCAL uint32 s_rotate_buf_size = 0;

// NV
LOCAL WALLPAPER_SEAWORLD_NV_T s_seaworld_nv = {0};

LOCAL uint8 s_vibrator_timer_id;

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/
extern IMG_RES_ERROR_E ALPHA_RES_Display_Img(IMG_RES_SRC_T  *img_src_ptr, IMG_RES_DST_T *img_dst_ptr);
extern IMG_RES_ERROR_E ALPHA_RES_GetImageInfo(IMG_RES_SRC_T  *img_src_ptr, IMG_RES_INFO_T *img_info_ptr);
/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
LOCAL void Init_All_Bubbles(void);
LOCAL void Handle_Fish_Mirro(SEAWORLD_FISH_T *fish);
LOCAL uint16 Seaworld_Option_GetBgID(void);
LOCAL void Seaworld_Option_SetBgID(uint16 value_id);
LOCAL uint16 Seaworld_Option_GetNeedVibrate(void);
LOCAL void Seaworld_Option_SetNeedVibrate(uint16 value_id);

/*****************************************************************************/
//  Description : 初始化
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SeaworldInit(void);
/*****************************************************************************/
//  Description : 构造函数
//  Global resource dependence : 
//  Author:arvin.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SeaworldConstruct(MMI_WIN_ID_T win_id);

/*****************************************************************************/
//  Description : 准备背景数据
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SeaworldDrawBg(MMI_WIN_ID_T win_id,const GUI_LCD_DEV_INFO *dev_info_ptr);

/*****************************************************************************/
//  Description : 算法
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL void SeaworldCalculate(LW_CAL_PARAM *lw_cal_param);
/*****************************************************************************/
//  Description : 释放
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:销毁临时申请的变量
/*****************************************************************************/
LOCAL void SeaworldDestruct(void);
/*****************************************************************************/
//  Description : 交互事件处理
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SeaworldHandleEvent(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/*****************************************************************************/
//  Description : 获得动态壁纸类型
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL uint32 SeaworldGetType(void);
/*****************************************************************************/
//  Description : 获得动态壁纸ThumbNail信息
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL void SeaworldGetThumbNailInfo(LW_THUMBNAIL_INFO *thumbnail_info_ptr);
/*****************************************************************************/
//  Description : 获得动态壁纸选项列表
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL OPTIONLIST *SeaworldGetOptionList(void);

LOCAL void StopFishVibratorTimer(void);
LOCAL void StartFishVibratorTimer(void);
LOCAL void StopFishTPVibrator(uint8 timer_id, uint32 param);
LOCAL MMI_TEXT_ID_T s_seaworld_option_bg_text[4]=
{
    TXT_SET_WALLPAPER_SEAWORLD_OPTION_BG1,
    TXT_SET_WALLPAPER_SEAWORLD_OPTION_BG2,
    TXT_SET_WALLPAPER_SEAWORLD_OPTION_BG3,
    TXT_SET_WALLPAPER_SEAWORLD_OPTION_BG4
};
LOCAL MMI_TEXT_ID_T s_seaworld_option_vibrate_text[2]=
{
    TXT_CLOSE,
    TXT_OPEN
};
LOCAL SelectList s_seaworld_option_bg_sl={4, s_seaworld_option_bg_text, Seaworld_Option_GetBgID,Seaworld_Option_SetBgID};
LOCAL SelectList s_seaworld_option_vibrate_sl={2, s_seaworld_option_vibrate_text, Seaworld_Option_GetNeedVibrate,Seaworld_Option_SetNeedVibrate};

LOCAL ListItem s_seaworld_option_items[2]=
{
	{TXT_COMM_WALLPAPER_BACKGROUND,TYPE_SELECT_LIST,&s_seaworld_option_bg_sl},
	{TXT_RING_VIBRA,TYPE_SELECT_LIST,&s_seaworld_option_vibrate_sl}
};


LOCAL OPTIONLIST s_seaworld_option_list={TXT_SET_WALLPAPER_SEAWORLD, 2, s_seaworld_option_items};



/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/
PUBLIC const MMI_LIVE_WALLPAPER_ITEM_INFO_T g_seaworld_Instance =
{
    SeaworldInit,
    SeaworldConstruct,
    SeaworldDrawBg,
    SeaworldCalculate,
    SeaworldDestruct,
    SeaworldHandleEvent,
    PNULL,
    SeaworldGetType,
    SeaworldGetThumbNailInfo,
    PNULL,
    PNULL,
    PNULL,
    SeaworldGetOptionList
};
/*---------------------------------------------------------------------------*/
LOCAL const float _sin_0_90_table[91] = 
{
    0,
    0.017, 0.034, 0.052, 0.069, 0.087, 0.104, 0.121, 0.139, 0.156, 0.173,
    0.190, 0.207, 0.224, 0.241, 0.258, 0.275, 0.292, 0.309, 0.325, 0.342,
    0.358, 0.374, 0.390, 0.406, 0.422, 0.438, 0.453, 0.469, 0.484, 0.499,
    0.515, 0.529, 0.544, 0.559, 0.573, 0.587, 0.601, 0.615, 0.629, 0.642,
    0.656, 0.669, 0.681, 0.694, 0.707, 0.719, 0.731, 0.743, 0.754, 0.766,
    0.777, 0.788, 0.798, 0.809, 0.819, 0.829, 0.838, 0.848, 0.857, 0.866,
    0.874, 0.882, 0.891, 0.898, 0.906, 0.913, 0.920, 0.927, 0.933, 0.939,
    0.945, 0.951, 0.956, 0.961, 0.965, 0.970, 0.974, 0.978, 0.981, 0.984,
    0.987, 0.990, 0.992, 0.994, 0.996, 0.997, 0.998, 0.999, 0.999, 1.000,
};
/******************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: 
//  Note:   input:  angle
//          output: sin(angle) * PRCISION
/*****************************************************************************/
LOCAL float SIN_S(int16 angle)
{
    angle %= 360;

    if(angle <= 90)
    {
        return _sin_0_90_table[angle];
    }
    else if (angle <= 180)
    {
        return _sin_0_90_table[180 - angle];
    }
    else if (angle <= 270)
    {
        return -_sin_0_90_table[angle - 180];
    }
    else if (angle <= 360)
    {
        return -_sin_0_90_table[360 - angle];
    }
    else 
    {
        return 0;
    }
}
/******************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: 
//  Note:   input:  angle
//          output: sin(angle) * PRCISION
/*****************************************************************************/
LOCAL float COS_S(int16 angle)
{
    angle %= 360;

    if (angle <= 90)
    {
        return _sin_0_90_table[90 - angle];
    }
    else if (angle <= 180)
    {
        return -_sin_0_90_table[angle - 90];
    }
    else if (angle <= 270)
    {
        return -_sin_0_90_table[270 - angle];
    }
    else if (angle <= 360)
    {
        return _sin_0_90_table[angle - 270];
    }
    else
    {
        return 0;
    }
    
}


/********************************************************************************************
*********************************************************************************************
*********************************************************************************************
*********************************************************************************************
*********************************************************************************************
*********************************************************************************************
********************************************************************************************/

#if 1

/******************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: 
//  Note:  
/*****************************************************************************/
LOCAL uint32 POW(uint32 b)
{
    return (b*b);
}

LOCAL BOOLEAN MMI_DecompressARGB(
                         const SEAWORLD_ROTATE_IMAGE_T *src_img_ptr, 
                         SEAWORLD_ROTATE_IMAGE_T *dst_img_ptr, 
                         uint8 *dst_buf_ptr, 
                         uint32 dst_buf_size)
{
	uint32 img_bytes = 0;
	IMG_RES_SRC_T img_src = {0};
	IMG_RES_DST_T img_dst = {0};
	IMG_RES_INFO_T img_info = {0};
	IMG_RES_ERROR_E error = 0;
	IMG_RES_DST_CTL_T dstctl ={0};

	img_src.imgstream_ptr = (uint8 *)src_img_ptr->data_ptr;
	img_src.imgstreamsize = src_img_ptr->data_size;

	error = ALPHA_RES_GetImageInfo(&img_src, &img_info);
	if(IMG_RES_SUCCESS != error)
	{
		return FALSE;
	}

	img_bytes = img_info.width * img_info.height * 4;

	if(PNULL == dst_buf_ptr || dst_buf_size < img_bytes)
	{
		return FALSE;
	}

	img_src.imgrect.left = 0;
	img_src.imgrect.top = 0;
	img_src.imgrect.right = img_info.width - 1;
	img_src.imgrect.bottom = img_info.height - 1;

	img_dst.dstctl = &dstctl;
	img_dst.dstctl->bg_type = IMG_BG_NORMAL;
	img_dst.dstctl->format = IMGREF_FORMAT_ARGB888;
	img_dst.dstctl->dstmem = dst_buf_ptr;
	img_dst.dstctl->width = img_info.width;
	img_dst.dstctl->height = img_info.height;
	img_dst.dstrect.top = 0;
	img_dst.dstrect.left = 0;
	img_dst.dstrect.right = img_info.width - 1;
	img_dst.dstrect.bottom = img_info.height - 1;

	SCI_MEMSET(dst_buf_ptr, 0, dst_buf_size);

	error = ALPHA_RES_Display_Img(&img_src, &img_dst);

	if(IMG_RES_SUCCESS != error)
	{
		return FALSE;
	}

	*dst_img_ptr = *src_img_ptr;

	dst_img_ptr->data_ptr = dst_buf_ptr;
	dst_img_ptr->data_size = img_bytes;
	dst_img_ptr->data_type = IMGREF_FORMAT_ARGB888;

	return TRUE;
}

LOCAL BOOLEAN MMI_RotateArbitrary(
            SEAWORLD_ROTATE_IMAGE_T *handle_img_ptr, 
            int32 angle, 
            uint8 *rotated_buf_ptr, 
            uint32 rotated_buf_size,
            uint16 *out_w,
            uint16 *out_h
            )
{
	SEAWORLD_ROTATE_IMAGE_T handle_img = {0};
	ROTATE_ARBITRARY_IN_T rot_in = {0};
	ROTATE_ARBITRARY_OUT_T rot_out = {0};
	
	handle_img = *handle_img_ptr;

       /* 设置参数*/
	rot_in.angle = angle;
	rot_in.src_format = IMGREF_FORMAT_ARGB888;
	rot_in.src_size.w = handle_img.width;
	rot_in.src_size.h = handle_img.height;
	rot_in.src_center.x = handle_img.center_x;
	rot_in.src_center.y = handle_img.center_y;
	rot_in.src_chn.chn0.ptr = (void *)handle_img.data_ptr;
	rot_in.src_chn.chn0.size = handle_img.width * handle_img.height * 4;
      rot_in.mode = ROTATE_ARBITRARY_NO_CROP;
      
	rot_in.target_format = IMGREF_FORMAT_ARGB888;
	rot_in.target_size.w = handle_img.width + handle_img.height;
	rot_in.target_size.h = handle_img.width + handle_img.height;
	rot_in.target_chn.chn0.ptr = (void *)rotated_buf_ptr;
	rot_in.target_chn.chn0.size = rotated_buf_size;
	rot_in.target_center.x = rot_in.target_size.w/2;
	rot_in.target_center.y = rot_in.target_size.h/2;

	//rot_in.mode = ROTATE_ARBITRARY_NO_CROP;// ROTATE_ARBITRARY_NO_CROP  , ROTATE_ARBITRARY_AUTO_CROP

	//SCI_TRACE_LOW:"--Rotate Image before-- angle:%d  "
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_LIVEWALLPAPER_SEAWORLD_483_112_2_18_2_32_25_35,(uint8*)"ddddddddd", angle, rot_in.src_size.w, rot_in.src_size.h, rot_in.src_center.x, rot_in.src_center.y,rot_in.target_size.w, rot_in.target_size.h, rot_in.target_center.x, rot_in.target_center.y);

	
	if(SCI_SUCCESS != GRAPH_RotateArbitrary(&rot_in, &rot_out))
	{
		return FALSE;
	}

	*out_w = rot_out.output_size.w;
	*out_h = rot_out.output_size.h;

	//SCI_TRACE_LOW:"--Rotate Image after-- angle:%d  "
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_LIVEWALLPAPER_SEAWORLD_496_112_2_18_2_32_25_36,(uint8*)"ddddd",angle, rot_out.output_size.w, rot_out.output_size.h, rot_out.output_offset.x, rot_out.output_offset.y);

	return TRUE;
}


/* 目前只支持ARGB888格式*/
LOCAL void MMI_Image_Mirror(MIRROR_IMAGE_T *mirror_ptr)
{
    MIRROR_IMAGE_T mirror = {0};
    uint16 i=0, j=0;
    uint16 half_w = 0;

    if(PNULL == mirror_ptr)
    {
        return;
    }
    mirror = *mirror_ptr;
    if(mirror.data_ptr != PNULL)
    {
        half_w = mirror.width / 2;
        
        if (mirror.data_type == IMGREF_FORMAT_ARGB888)
        {
            uint32 *buf_ptr = (uint32 *)mirror.data_ptr;
            uint32 temp_value = 0;
            uint32 *temp_ptr = PNULL;
            uint32 *front_ptr = PNULL;
            uint32 *end_ptr = PNULL;
            
            for (j = 0; j < mirror.height; ++j)
            {
                temp_ptr = buf_ptr + j*mirror.width;
                for (i = 0; i < half_w; ++i)
                {
                    front_ptr = temp_ptr + i;
                    end_ptr = temp_ptr + mirror.width - 1 - i;
                    
                    temp_value = *front_ptr;
                    *front_ptr = *end_ptr;
                    *end_ptr = temp_value;
                }
            }
        }
    }
}

/* 务必在不使用时释放*decode_buf */
LOCAL BOOLEAN MMI_Alloc_PNG_Decode_buffer(
                MMI_IMAGE_ID_T img_id, 
                MMI_WIN_ID_T win_id,
                uint8 **decode_buf,
                uint32 *decode_size,
                uint16 *width,
                uint16 *height)
{
    SEAWORLD_ROTATE_IMAGE_T rot_img = {0};
    uint8 *decomp_buf_ptr = PNULL;
    uint32 decomp_buf_size = 0;

    rot_img.data_type= IMGREF_FORMAT_COMPRESSED_ARGB;
    GUIRES_GetImgWidthHeight(
        &rot_img.width, 
        &rot_img.height, 
        img_id, 
        win_id);
    rot_img.data_ptr = 
        (uint8 *)MMI_GetLabelImage(
             img_id,
             win_id,
             &(rot_img.data_size));   
    
    do
    {
        decomp_buf_size = rot_img.width*rot_img.height*4;
        decomp_buf_ptr = (void*)MMILIVEWALLPAPER_ALLOC(decomp_buf_size);
        if (PNULL == decomp_buf_ptr)
        {
            break;
        }
        SCI_MEMSET(decomp_buf_ptr, 0, decomp_buf_size);

        if(!MMI_DecompressARGB(
                 &rot_img, 
                 &rot_img, 
                 decomp_buf_ptr, 
                 decomp_buf_size))
        {
            break;
        }

        *decode_buf = decomp_buf_ptr;
        *decode_size = decomp_buf_size;
        *width = rot_img.width;
        *height = rot_img.height;

        return TRUE;
    }while(0);

    /* Error to exit*/
    if (decomp_buf_ptr != PNULL)
    {
         MMILIVEWALLPAPER_FREE(decomp_buf_ptr);//SCI_FREE
         decomp_buf_ptr = PNULL;
    }
    
    *decode_buf = PNULL;
    *decode_size = 0;
    *width = *height = 0;
    
    return FALSE;
}

/* 分配内存并镜像*/
LOCAL MIRROR_IMAGE_T MMI_Mirror_PNG_buffer(
                MMI_IMAGE_ID_T img_id, 
                MMI_WIN_ID_T win_id)
{
    MIRROR_IMAGE_T mirror = {0};

    mirror.data_type= IMGREF_FORMAT_COMPRESSED_ARGB;

    if(!MMI_Alloc_PNG_Decode_buffer(
                img_id, 
                win_id, 
                (uint8**)&mirror.data_ptr, 
                (uint32*)&mirror.data_size, 
                (uint16*)&mirror.width,
                (uint16*)&mirror.height))
        {
            return mirror;
        }
                
    MMI_Image_Mirror(&mirror);
    
    return mirror;
}

/*
调用该接口获取图片旋转后的buffer。
请记住: 务必在不使用后释放该buffer。
*/
LOCAL void MMI_Rotate_Fish_Buffer(
                SEAWORLD_FISH_T *fish, 
                void *rotate_buf_ptr, 
                uint32 rotate_buf_size)
{
    SEAWORLD_ROTATE_IMAGE_T rot_img = {0};

    rot_img.data_ptr = fish->data_buf;
    rot_img.data_size= fish->data_size;
    rot_img.width = fish->width;
    rot_img.height = fish->height;
    rot_img.center_x = fish->width/2;
    rot_img.center_y = fish->height/2;
    rot_img.data_type = IMGREF_FORMAT_ARGB888;
    
    if(!MMI_RotateArbitrary(
    		&rot_img, 
    		fish->angle, 
    		rotate_buf_ptr, 
    		rotate_buf_size,
    		&fish->rotate_width,
    		&fish->rotate_height
    		))
    {
        //SCI_TRACE_LOW:"[MMI_Rotate_Fish_Buffer Fail] - angle: %d\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_LIVEWALLPAPER_SEAWORLD_655_112_2_18_2_32_25_37,(uint8*)"d", fish->angle);
    }

}
#endif
/********************************************************************************************
*********************************************************************************************
*********************************************************************************************
*********************************************************************************************
*********************************************************************************************
*********************************************************************************************
********************************************************************************************/





LOCAL void CreateStaticNewLayer(GUI_LCD_DEV_INFO *layer_dev_info_ptr,uint16 width,uint16 height)
{
    GUI_LCD_DEV_INFO widget_layer_handle = {0};
    UILAYER_CREATE_T create_info = {0};   

    if (PNULL == layer_dev_info_ptr)
    {
        return;
    }
    create_info.lcd_id          = LCD_ID_0;
    create_info.owner_handle    = MAIN_IDLE_WIN_ID;
    create_info.offset_x        = 0;
    create_info.offset_y        = 0;
    create_info.width           = width;
    create_info.height          = height;
    create_info.is_bg_layer     = FALSE;
    create_info.is_static_layer = TRUE;   
    create_info.func_ptr = MMIAPILIVEWALLPAPER_UilayerMemPoolFunc(); 
    
    UILAYER_CreateLayer(&create_info, &widget_layer_handle);
        
    UILAYER_SetLayerColorKey(&widget_layer_handle, TRUE, UILAYER_TRANSPARENT_COLOR);
    *layer_dev_info_ptr = widget_layer_handle;
    if (UILAYER_IsLayerActive(&widget_layer_handle))
    {
        UILAYER_Clear(&widget_layer_handle);
    }
}

LOCAL void DestroyNewLayer(GUI_LCD_DEV_INFO *layer_dev_info_ptr)
{
    UILAYER_RELEASELAYER(layer_dev_info_ptr);   /*lint !e506 !e774*/
//    UILAYER_ReleaseLayer(layer_dev_info_ptr);     
//    layer_dev_info_ptr->block_id = UILAYER_NULL_HANDLE;
}	

 /*获取正弦振幅。振幅A为0-45中间*/
LOCAL uint16 Get_Sin_Random_A(uint16 index)
{
    uint16 random = 0;
    uint32 time = SCI_GetTickCount() + index;
    srand(time);
    random = rand() % 20;

    return random;
}

/*获取正弦w值*/
LOCAL uint16 Get_Sin_Random_w(uint16 index)
{
    uint16 random = 0;
    uint32 time = SCI_GetTickCount() + index;
    srand(time);
    random = rand() % 2 + 2;

    return random;
}

/*获取正弦angle_v值*/
LOCAL uint16 Get_Sin_Random_angle_v(uint16 index)
{
    uint16 random = 0;
    uint32 time = SCI_GetTickCount() + index;
    srand(time);
#ifdef MAINLCD_SIZE_320X480
    random = rand() % 5 + 7;
#else
    random = rand() % 4 + 3;
#endif

    return random;
}

LOCAL GUI_POINT_T Get_Random_Bubble_startPos(void)
{
    GUI_POINT_T point = {0};
    uint16 lcd_width = MMITHEME_GetMainScreenRightPixel() + 1;
    uint16 lcd_height = MMITHEME_GetMainScreenBottomPixel() + 1;
    uint16 random = 0;
    uint32 time = SCI_GetTickCount();
    srand(time);
    random = rand() % lcd_width;
    point.x = random;
    time += random;
#ifdef MAINLCD_SIZE_320X480    
    random = rand() % 40 + 40;   /* 40 -79*/
#else
    random = rand() % 30 + 20;   /* 40 -79*/
#endif
    point.y = lcd_height - random;

    return point;
}

LOCAL uint16 Get_Random_Sunshine_Length(uint16 index)
{
    uint16 random = 0;
    uint16 lcd_height = MMITHEME_GetMainScreenBottomPixel() + 1;
    uint32 time = SCI_GetTickCount() + (index << 10) + index;
    srand(time);
    if (index < SUNSHINE_TOTAL/3*2)
    {
        random = rand() % (lcd_height/3) + lcd_height/4;
    }
    else
    {
        random = rand() % (lcd_height/4) + lcd_height/3*2;
    }
    

    return random;
}

LOCAL uint16 Get_Random_Sunshine_Speed(uint16 index)
{
    uint16 random = 0;
    uint32 time = SCI_GetTickCount() + index;
    srand(time);
#ifdef MAINLCD_SIZE_320X480    
    random = rand() % 3 + 7;
#else
    random = rand() % 3 + 2;
#endif

    return random;
}

LOCAL uint16 Get_Random_Fish_Speed(uint16 index)
{
    uint16 random = 0;
    uint32 time = SCI_GetTickCount() + index;
    srand(time);
#ifdef MAINLCD_SIZE_320X480        
    random = rand() % 5 + 7;
#else
    random = rand() % 4 + 4;
#endif

    return random;
}

/* 
     向左移动 [0 90) :左上  (270, 360):左下
     向右移动 [0 90) :右下  (270, 360):右上
*/
LOCAL uint16 Get_Random_Fish_Angle(uint16 index)
{
    int16 random = 0;
    uint32 time = SCI_GetTickCount() + index;
    srand(time);
    random = rand() % 90 - 45;

    if (random < 0)
    {
        random += 360;
    }
    return random;
}


LOCAL uint16 Get_Random_Fish_YPos(uint16 index)
{
    uint16 random = 0;
    uint16 lcd_height = MMITHEME_GetMainScreenBottomPixel() + 1;
    uint32 time = SCI_GetTickCount() + index;
    srand(time);
    random = rand() % (lcd_height *2/3) + (lcd_height/6);

    return random;
}

LOCAL uint16 Get_Random_Single_Fish_Move_Type(uint16 index)
{
    uint16 random = 0;
    uint32 time = SCI_GetTickCount() + index;
    srand(time);
    random = rand() % SINGLE_FISH_MOVE_TYPE_MAX;

    return random;
}

/* 检查是否需要该表气泡的起点位置*/
LOCAL BOOLEAN Check_Change_Bubble_StartPos(void)
{
    return (BUBBLE_TOTAL == s_bubble_change_startPos_flag) ? TRUE : FALSE ;
}

LOCAL void Single_Sina_Bubble_Init(SIN_BUBBLE_INFO_T *bubble, uint16 index)
{
    uint16 random_A = Get_Sin_Random_A(index);
    uint16 random_w = Get_Sin_Random_w(index);
    uint16 random_angle_v = Get_Sin_Random_angle_v(index);

    bubble->index = index;
    bubble->left = s_bubble_startPos.x;
    bubble->top = s_bubble_startPos.y + index;
    bubble->start_x = s_bubble_startPos.x;
    bubble->start_y = s_bubble_startPos.y;
    bubble->A = random_A;
    bubble->w = random_w;
    bubble->t = 0;    /*目前初始化为0,暂时未用到*/
    bubble->k = 0;    /*目前初始化为0,暂时未用到*/
    bubble->yValue = 0;
    bubble->angle = 0;
    bubble->angle_v = random_angle_v;
    bubble->direction = index % 2;

    bubble->img = IMAGE_WALLPAPER_SEAWORLD_BUBBLE1;

    bubble->is_over = FALSE;

    //Console_PrintfN("%d, %d, %d", random_A, random_w, random_angle_v);
}
			
LOCAL void Single_Sina_Bubble_Move(SIN_BUBBLE_INFO_T *bubble)
{
    uint16 lcd_height = MMITHEME_GetMainScreenBottomPixel() + 1;
    uint16 piece_h = lcd_height / BUBBLE_IMG_TOTAL;

    if (bubble->top > bubble->start_y)
    {
        bubble->top--;

        /*
        // 如果起始点改变了，没有显示的bubble重新按照新初始点初始化
        if (bubble->start_x != s_bubble_startPos.x && bubble->start_y != s_bubble_startPos.y)
        {
            Single_Sina_Bubble_Init(bubble, bubble->index);
        }
        */
        return;
    }

    if (bubble->top < 0)   /* 这里需要重新设置，最好是根据图片大小设置*/
    {
        bubble->angle = 0;
        //Single_Sina_Bubble_Init(bubble, bubble->index);
        
        if (!bubble->is_over)
        {
            bubble->is_over = TRUE;
            ++s_bubble_change_startPos_flag;
        }
       
        return;
    }

    // 主要算法
    bubble->yValue = (int16)(bubble->A * SIN_S(bubble->angle));
    bubble->angle += bubble->angle_v;
    if (bubble->direction)
    {
        bubble->left = bubble->start_x - bubble->yValue;
    }
    else
    {
        bubble->left = bubble->start_x + bubble->yValue;
    }
    bubble->top = bubble->start_y - (bubble->angle );
    

    // 指定图片
    if (bubble->top > bubble->start_y - piece_h)
    {
        bubble->img = IMAGE_WALLPAPER_SEAWORLD_BUBBLE1;
    }
    else if (bubble->top > bubble->start_y - piece_h * 2)
    {
        bubble->img = IMAGE_WALLPAPER_SEAWORLD_BUBBLE1 + 1;
    }
    else
    {
        bubble->img = IMAGE_WALLPAPER_SEAWORLD_BUBBLE1 + 2;
    }
}

LOCAL void Single_Sunshine_Init(
                SEAWORLD_SUNSHINE_T *sunshine, 
                uint16 index, 
                int16 start_x,    // 起始x坐标
                int16 range_start,
                int16 range_end,
                BOOLEAN direction
                )
{
    uint16 random_speed = Get_Random_Sunshine_Speed(index);
    uint16 random_length = Get_Random_Sunshine_Length(index);
    
    sunshine->index = index;
    sunshine->left = start_x;
    sunshine->top = 0;
    sunshine->length = random_length;
    sunshine->speed = random_speed;
    sunshine->range_start = range_start;
    sunshine->range_end = range_end;   
    sunshine->direction = direction;
}

LOCAL void Single_Sunshine_Move(SEAWORLD_SUNSHINE_T *sunshine, uint16 index)
{
    if (sunshine->direction)  /*向右*/
    {
        sunshine->left += sunshine->speed;
        if (sunshine->left  > sunshine->range_end)
        {
            sunshine->left = sunshine->range_end;
            sunshine->direction = !sunshine->direction;
            sunshine->speed = Get_Random_Sunshine_Speed(index);
        }
    }
    else
    {
        sunshine->left -= sunshine->speed;
        if (sunshine->left  < sunshine->range_start)
        {
            sunshine->left = sunshine->range_start;
            sunshine->direction = !sunshine->direction;
            sunshine->speed = Get_Random_Sunshine_Speed(index);
        }
    }
}

LOCAL void Single_Fish_Init(SEAWORLD_FISH_T *fish, uint16 index)
{
    uint16 width=0, height=0;
    MMI_HANDLE_T win_id = MAIN_IDLE_WIN_ID;

    fish->img_id = IMAGE_WALLPAPER_SEAWORLD_BIG_FISH1 + index;
    GUIRES_GetImgWidthHeight(&width, &height, fish->img_id, win_id);
    fish->index = index;
    fish->left = 0;
    fish->top = Get_Random_Fish_YPos(index);
    fish->width = width;
    fish->height = height;
    fish->speed = Get_Random_Fish_Speed(index);
    fish->direction =  (index%2) ? TRUE : FALSE;
    fish->is_touch = FALSE;
    fish->is_moving = FALSE;
    fish->angle = Get_Random_Fish_Angle(index);
    fish->move_type = SINGLE_FISH_MOVE_LINEAR;//Get_Random_Single_Fish_Move_Type(index);

    if (fish->direction)
    {
        fish->left = 0 - fish->width;
        fish->top = Get_Random_Fish_YPos(fish->index);
    }
    else
    {
        fish->left = MMITHEME_GetMainScreenRightPixel() + 1;
        fish->top = Get_Random_Fish_YPos(fish->index);
    }
}

LOCAL void Single_Fish_Move(SEAWORLD_FISH_T *fish)
{
    int16 x_off = 0, y_off = 0;
    uint16 angle = fish->angle;
     
    x_off = (int16)(fish->speed * COS_S(angle));
    y_off = (int16)(fish->speed * SIN_S(angle));

    if (fish->direction)
    {
        fish->left += x_off;
        fish->top += y_off;
    }
    else
    {
        fish->left -= x_off;
        fish->top -= y_off;
    }
}

LOCAL void Handle_Fish_Mirro(SEAWORLD_FISH_T *fish)
{
    MIRROR_IMAGE_T mirror = {0};

    mirror.data_type = IMGREF_FORMAT_ARGB888;
    mirror.data_ptr = fish->data_buf;
    mirror.data_size = fish->data_size;
    mirror.width = fish->width;
    mirror.height = fish->height;

    MMI_Image_Mirror(&mirror);
}


/* 检查鱼是否被点中*/
LOCAL BOOLEAN Check_Fish_Touch(SEAWORLD_FISH_T *fish, int16 x, int16 y)
{
    if(PNULL == fish)
    {
        return FALSE;
    }
    
    if (x >= fish->left && x < (fish->left+fish->rotate_width)
      && y >= fish->top && y < (fish->top + fish->rotate_height))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

LOCAL void Check_Fish_Direction_Change(SEAWORLD_FISH_T *fish)
{
    BOOLEAN direction_change = FALSE;
    int16 lcd_width = MMITHEME_GetMainScreenRightPixel() + 1;
    int16 lcd_height = MMITHEME_GetMainScreenBottomPixel() + 1;
    
    if (fish->direction)    // 向右移动
    {
        if (fish->left < 0 - fish->rotate_width
          || fish->left >= lcd_width
          || fish->top < 0 - fish->rotate_height
          || fish->top > lcd_height)
        {
            direction_change = TRUE;
            fish->left = MMITHEME_GetMainScreenRightPixel() + 1;
            fish->top = Get_Random_Fish_YPos(fish->index);
        }
    }
    else    // 向左移动
    {
        if (fish->left + fish->rotate_width < 0
          || fish->left >= lcd_width
          || fish->top < 0 - fish->rotate_height
          || fish->top > lcd_height)
        {
            direction_change = TRUE;
            fish->left = 0 - fish->width;
            fish->top = Get_Random_Fish_YPos(fish->index);
        }
    }

    if (direction_change)
    {
        fish->direction = !fish->direction;
        fish->speed = Get_Random_Fish_Speed(fish->index);
        fish->is_moving = FALSE;
        fish->is_touch = FALSE;
        fish->angle = Get_Random_Fish_Angle(fish->index);
        Handle_Fish_Mirro(fish);
    }
}


/* 当前指定，一次最多可以添加[ 3 ]条鱼*/
LOCAL void Add_Moving_Fish(void)
{
    
    uint16 i = 0, j = 0;
    uint8 moving_fish_num = 0;
    uint8 non_moving_fish_num = 0;
    uint8 non_moving_fish_array[FISH_TOTAL] = {0};

    for (i = 0; i < FISH_TOTAL; ++i)
    {
        if (s_fish_array[i].is_moving)
        {
            ++moving_fish_num;
        }
        else
        {
            non_moving_fish_array[j++] = i;
        }
    }

    non_moving_fish_num = FISH_TOTAL - moving_fish_num;
    
    if (moving_fish_num < FISH_MOVING_MAX)
    {
        uint16 random1, random2 = 0;
        uint32 time = SCI_GetTickCount();
        srand(time);
        random1 = rand() % FISH_ADD_MAX;

        // 如果超出最大限制
        if (moving_fish_num + random1 > FISH_MOVING_MAX)
        {
            random1 = FISH_MOVING_MAX - moving_fish_num;  // 还剩多少条鱼可以添加
        }

        for (i = 0; i < random1; ++i)
        {
            random2 = rand()%non_moving_fish_num;
            s_fish_array[non_moving_fish_array[random2]].is_moving = TRUE;
        }
    }
}

LOCAL void Init_All_Bubbles(void)
{
    uint16 i = 0;
    
    s_bubble_startPos = Get_Random_Bubble_startPos();
    s_bubble_change_startPos_flag = 0;

    for (i = 0; i < BUBBLE_TOTAL; ++i)
    {
        Single_Sina_Bubble_Init(&s_bubble_array[i], i);
    }
}

LOCAL void Init_All_Sunshine(void)
{
    uint16 i = 0;
    uint16 lcd_width = MMITHEME_GetMainScreenRightPixel() + 1;
    uint16 piece = (SUNSHINE_TOTAL + 2) / 3;
    int16 range1_start = -15, range1_end = lcd_width/2;
    int16 range2_start = lcd_width/2, range2_end = lcd_width -5;
    int16 range3_start = lcd_width/4, range3_end = lcd_width - range3_start;
    int16 start_x = 0;


    start_x = (range1_end - range1_start) / (piece + 1);
    for (i = 0; i < piece; ++i)
    {
        Single_Sunshine_Init(&s_sunshine_array[i], i, start_x*i, range1_start, range1_end, TRUE);
    }

    start_x = (range2_end - range2_start) / (piece + 1);
    for (; i < piece*2; ++i)
    {
        Single_Sunshine_Init(&s_sunshine_array[i], i, range2_start + start_x*(i%piece), range2_start, range2_end, FALSE);
    }

    start_x = (range3_end - range3_start) / (piece + 1);
    for (; i < SUNSHINE_TOTAL; ++i)
    {
        Single_Sunshine_Init(&s_sunshine_array[i], i, range3_start + start_x*(i%piece), range3_start, range3_end, TRUE);
    }
}

LOCAL void Init_All_Fish(void)
{
    uint16 i = 0;
    
    for (i = 0; i < FISH_TOTAL; ++i)
    {
        Single_Fish_Init(&s_fish_array[i], i);
    }
}

LOCAL void Seaworld_Read_NV(void)
{
    MN_RETURN_RESULT_E return_value= MN_RETURN_FAILURE;
    
    return_value = MMI_ReadNVItem(MMINV_LIVE_WALLPAPER_SEAWORLD, &s_seaworld_nv);
    if (MN_RETURN_SUCCESS != return_value)
    {
        s_seaworld_nv.bg_id = IMAGE_WALLPAPER_SEAWORLD_BG1;
        s_seaworld_nv.need_vibrate = FALSE;
        MMI_WriteNVItem(MMINV_LIVE_WALLPAPER_SEAWORLD,&s_seaworld_nv);
    }
}

LOCAL uint16 Seaworld_Option_GetBgID(void)
{
    uint16 result = 0;
    
    Seaworld_Read_NV();
    
    result = s_seaworld_nv.bg_id - IMAGE_WALLPAPER_SEAWORLD_BG1;
    
    return result;
}

LOCAL void Seaworld_Option_SetBgID(uint16 value_id)
{

    s_seaworld_nv.bg_id = IMAGE_WALLPAPER_SEAWORLD_BG1 + value_id;
    
    MMI_WriteNVItem(MMINV_LIVE_WALLPAPER_SEAWORLD,&s_seaworld_nv);
}

LOCAL uint16 Seaworld_Option_GetNeedVibrate(void)
{
    uint16 result = 0;
    
    Seaworld_Read_NV();
    if (s_seaworld_nv.need_vibrate)
    {
        result = 1;
    }
    else
    {
        result = 0;
    }
    
    return result;
}

LOCAL void Seaworld_Option_SetNeedVibrate(uint16 value_id)
{
    if (value_id == 0)
    {
        s_seaworld_nv.need_vibrate = FALSE;
    }
    else if (value_id == 1)
    {
        s_seaworld_nv.need_vibrate = TRUE;
    }

    MMI_WriteNVItem(MMINV_LIVE_WALLPAPER_SEAWORLD,&s_seaworld_nv);
}

/*****************************************************************************/
//  Description : 初始化
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SeaworldInit(void)
{
    BOOLEAN b_result = TRUE;

    return b_result;
}
/*****************************************************************************/
//  Description : 构造函数
//  Global resource dependence : 
//  Author:arvin.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SeaworldConstruct(MMI_WIN_ID_T win_id)
{
    BOOLEAN b_result = TRUE;
    uint16 i = 0;
    uint16 width, height;
    GUI_POINT_T start_point = {0};

    do
    {
        //initial s_bubble_array;
        if(PNULL != s_bubble_array)
        {
            SCI_TRACE_LOW("SeaworldConstruct, have a exception before Construct: PNULL != s_bubble_array ");
            MMILIVEWALLPAPER_FREE(s_bubble_array);
            s_bubble_array = PNULL;
        }
        s_bubble_array = MMILIVEWALLPAPER_ALLOC((sizeof(SIN_BUBBLE_INFO_T))*BUBBLE_TOTAL);
        if(PNULL == s_bubble_array)
        {
            b_result = FALSE;
            break;
        }

        //initial s_sunshine_array;
        if(PNULL != s_sunshine_array)
        {
            SCI_TRACE_LOW("SeaworldConstruct, have a exception before Construct: PNULL != s_sunshine_array ");
            MMILIVEWALLPAPER_FREE(s_sunshine_array);
            s_sunshine_array = PNULL;
        }
        s_sunshine_array = MMILIVEWALLPAPER_ALLOC((sizeof(SEAWORLD_SUNSHINE_T))*SUNSHINE_TOTAL);
        if(PNULL == s_sunshine_array)
        {
            b_result = FALSE;
            break;
        }

        //initial s_fish_array;
        if(PNULL != s_fish_array)
        {
            SCI_TRACE_LOW("SeaworldConstruct, have a exception before Construct: PNULL != s_fish_array ");
            MMILIVEWALLPAPER_FREE(s_fish_array);
            s_fish_array = PNULL;
        }
        s_fish_array = MMILIVEWALLPAPER_ALLOC((sizeof(SEAWORLD_FISH_T))*FISH_TOTAL);
        if(PNULL == s_fish_array)
        {
            b_result = FALSE;
            break;
        }

        //intial prepare;
        Seaworld_Read_NV();
        Init_All_Bubbles();
        Init_All_Sunshine();
        Init_All_Fish();
        
        /* init bubble layer */
        for (i = 0; i < BUBBLE_IMG_TOTAL; ++i)
        {
            GUIRES_GetImgWidthHeight(&width, &height, IMAGE_WALLPAPER_SEAWORLD_BUBBLE1+i, win_id);
            CreateStaticNewLayer(&s_bubble_layer[i], width, height);
            GUIRES_DisplayImg(&start_point,
                PNULL,
                PNULL,
                win_id,
                IMAGE_WALLPAPER_SEAWORLD_BUBBLE1+i, 
                &s_bubble_layer[i]);
        }

        /* init sunshine layer */
        GUIRES_GetImgWidthHeight(&width, &height, IMAGE_WALLPAPER_SEAWORLD_IMG_SUNSHINE, win_id);
        CreateStaticNewLayer(&s_sunshine_layer, width, height);
        GUIRES_DisplayImg(&start_point,
            PNULL,
            PNULL,
            win_id,
            IMAGE_WALLPAPER_SEAWORLD_IMG_SUNSHINE, 
            &s_sunshine_layer);

        /* init fish buffer */
        s_rotate_buf_size = POW(FISH_ROTATE_RADIUS)*4;
        s_rotate_buf_ptr = (uint8 *)MMILIVEWALLPAPER_ALLOC(s_rotate_buf_size);
        if(PNULL == s_rotate_buf_ptr)
        {
            b_result = FALSE;
            break;
        }
        SCI_MEMSET(s_rotate_buf_ptr, 0, s_rotate_buf_size);
        //
        if (PNULL == s_fish_array)
        {
            return FALSE;
        }
                    
        for (i = 0; i < FISH_TOTAL; ++i)
        {            
            if(!MMI_Alloc_PNG_Decode_buffer(
                            IMAGE_WALLPAPER_SEAWORLD_BIG_FISH1 + i,
                            win_id,
                            &s_fish_array[i].data_buf,
                            &s_fish_array[i].data_size,
                            &s_fish_array[i].width,
                            &s_fish_array[i].height
                            ))
            {
                b_result = FALSE;
                break;
            }
            if (s_fish_array[i].direction)
            {
                Handle_Fish_Mirro(&s_fish_array[i]);
            }
        }
    }while(0);
    
    if(!b_result)
    {
        //in the case of failure to allocate memory,we need to release all the mems allocated. 
        SCI_TRACE_LOW("SeaworldConstruct, have a exception at allocating process in Construct() ");
        SeaworldDestruct();
    }
    
    return b_result;
}

/*****************************************************************************/
//  Description : 准备背景数据
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SeaworldDrawBg(MMI_WIN_ID_T win_id,const GUI_LCD_DEV_INFO *dev_info_ptr)
{
    BOOLEAN b_result = FALSE;
    GUI_POINT_T display_point = {0,0};

    Seaworld_Read_NV();
    
    if (PNULL != dev_info_ptr)
    {
        b_result = GUIRES_DisplayImg(&display_point, 
		PNULL, 
		PNULL, 
		win_id, 
		s_seaworld_nv.bg_id, 
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
LOCAL void SeaworldCalculate(LW_CAL_PARAM *lw_cal_param)
{   
    uint16 i = 0;
    MMI_HANDLE_T win_id = MAIN_IDLE_WIN_ID;
    
    UILAYER_INFO_T layer_info = {0};
    GUI_LCD_DEV_INFO *cur_layer = NULL;
    GUI_RECT_T rect = {0};
    IMGREF_SIZE_T	pop_bg_window = {0,0};
    IMGREF_SIZE_T	pop_up_window = {0,0};
    FIX16_POINT_T	offset = {0};
    uint8* layer_buf_ptr;
    uint16 width = 0, height = 0;

    if ((PNULL == lw_cal_param) || (PNULL == lw_cal_param->dest_buf_info.buf_ptr) || (PNULL == lw_cal_param->src_buf_info.buf_ptr))
    {
        return;
    }
    if((PNULL != lw_cal_param->src_buf_info.buf_ptr) && (PNULL != lw_cal_param->dest_buf_info.buf_ptr))
    {
        SCI_MEMCPY((GUI_COLOR_T*)lw_cal_param->dest_buf_info.buf_ptr, lw_cal_param->src_buf_info.buf_ptr,MMI_MAINSCREEN_WIDTH*MMI_MAINSCREEN_HEIGHT*sizeof(uint16));
    }
    pop_bg_window.w = lw_cal_param->src_buf_info.w;
    pop_bg_window.h = lw_cal_param->src_buf_info.h;

    /* 1. 绘制鱼*/   
    if (PNULL == s_fish_array)
    {
        return;
    }
            
    for (i = 0; i < FISH_TOTAL; ++i)
    {        
        if (!s_fish_array[i].is_moving)
        {
            continue;
        }
        Single_Fish_Move(&s_fish_array[i]);
        
        SCI_MEMSET(s_rotate_buf_ptr, 0, s_rotate_buf_size);
        MMI_Rotate_Fish_Buffer(&s_fish_array[i], s_rotate_buf_ptr, s_rotate_buf_size);
        
        pop_up_window.w = s_fish_array[i].rotate_width;
        pop_up_window.h = s_fish_array[i].rotate_height;
        rect.left = s_fish_array[i].left;
        rect.top = s_fish_array[i].top;
        offset.x=(int16)rect.left;
        offset.y=(int16)rect.top;
        offset.x_dec=(uint16)((rect.left<<16)&0xffff);
        offset.y_dec=(uint16)((rect.top<<16)&0xffff);
        S2D_Blending(pop_bg_window , lw_cal_param->dest_buf_info.buf_ptr, pop_up_window , offset, (uint32*)s_rotate_buf_ptr, lw_cal_param->dest_buf_info.buf_ptr);        
        
        Check_Fish_Direction_Change(&s_fish_array[i]);
    }

    Add_Moving_Fish();


    /* 2. 绘制气泡*/
    if (PNULL == s_bubble_array)
    {
        return;
    }
          
    for (i = 0; i < BUBBLE_TOTAL; ++i)
    {
        Single_Sina_Bubble_Move(&s_bubble_array[i]);
        if (s_bubble_array[i].top >= s_bubble_array[i].start_y 
            || s_bubble_array[i].is_over )
        {
            continue;
        }

        cur_layer = &s_bubble_layer[s_bubble_array[i].img - IMAGE_WALLPAPER_SEAWORLD_BUBBLE1];
        UILAYER_GetLayerInfo(cur_layer, &layer_info);
        rect.left = s_bubble_array[i].left;
        rect.top = s_bubble_array[i].top;
        rect.right = rect.left + layer_info.layer_width -1;
        rect.bottom = rect.top + layer_info.layer_height -1;

        offset.x=(int16)rect.left;
        offset.y=(int16)rect.top;
        offset.x_dec=(uint16)((rect.left<<16)&0xffff);
        offset.y_dec=(uint16)((rect.top<<16)&0xffff);

        pop_up_window.w = layer_info.layer_width;
        pop_up_window.h = layer_info.layer_height;

        layer_buf_ptr=UILAYER_GetLayerBufferPtr(cur_layer);
        if(PNULL != layer_buf_ptr)
        {
            S2D_Blending(pop_bg_window ,lw_cal_param->dest_buf_info.buf_ptr,pop_up_window ,offset,(uint32*)layer_buf_ptr,lw_cal_param->dest_buf_info.buf_ptr);
        }
    }

    // 检查是否要重新初始化气泡
    if (Check_Change_Bubble_StartPos())
    {
        Init_All_Bubbles();
    }


    /*3.  绘制阳光*/
    if (PNULL == s_sunshine_array)
    {
        return;
    }
          
    GUIRES_GetImgWidthHeight(&width, &height, IMAGE_WALLPAPER_SEAWORLD_IMG_SUNSHINE, win_id);
    for (i = 0; i < SUNSHINE_TOTAL; ++i)
    { 
        Single_Sunshine_Move(&s_sunshine_array[i], s_sunshine_array[i].index);

        cur_layer = &s_sunshine_layer;
        UILAYER_GetLayerInfo(cur_layer, &layer_info);
        rect.left = s_sunshine_array[i].left;
        rect.top = 0 - (height - s_sunshine_array[i].length);
        rect.right = rect.left + width - 1;
        rect.bottom = s_sunshine_array[i].length -1;

        offset.x=(int16)rect.left;
        offset.y=(int16)rect.top;
        offset.x_dec=(uint16)((rect.left<<16)&0xffff);
        offset.y_dec=(uint16)((rect.top<<16)&0xffff);

        pop_up_window.w = layer_info.layer_width;
        pop_up_window.h = layer_info.layer_height;

        layer_buf_ptr=UILAYER_GetLayerBufferPtr(cur_layer);
        if(PNULL != layer_buf_ptr)
        {
            S2D_Blending(pop_bg_window ,lw_cal_param->dest_buf_info.buf_ptr,pop_up_window ,offset,(uint32*)layer_buf_ptr,lw_cal_param->dest_buf_info.buf_ptr);
        }
    }

}
/*****************************************************************************/
//  Description : 释放
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:销毁临时申请的变量
/*****************************************************************************/
LOCAL void SeaworldDestruct(void)
{
    uint16 i = 0;
    
    /* free bubble layer */
    for (i = 0; i < BUBBLE_IMG_TOTAL; ++i)
    {
        DestroyNewLayer(&s_bubble_layer[i]);     
    }

    /* free sunshine layer */
    DestroyNewLayer(&s_sunshine_layer);     

    /* free fish related */
    if(PNULL != s_rotate_buf_ptr)
    {
        MMILIVEWALLPAPER_FREE(s_rotate_buf_ptr);
        s_rotate_buf_ptr = PNULL;
    }

    if(PNULL != s_fish_array)
    {
        for (i = 0; i < FISH_TOTAL; ++i)
        {
            if (s_fish_array[i].data_buf != PNULL)
            {
                MMILIVEWALLPAPER_FREE(s_fish_array[i].data_buf);
                s_fish_array[i].data_buf = PNULL;
                s_fish_array[i].data_size = 0;
            }
        }
    }

    if(PNULL != s_bubble_array)
    {
        MMILIVEWALLPAPER_FREE(s_bubble_array);
        s_bubble_array = PNULL;
    }

    if(PNULL != s_sunshine_array)
    {
        MMILIVEWALLPAPER_FREE(s_sunshine_array);
        s_sunshine_array = PNULL;
    }

    if(PNULL != s_fish_array)
    {
        MMILIVEWALLPAPER_FREE(s_fish_array);
        s_fish_array = PNULL;
    }
}

LOCAL void Seaworld_PointProc(uint16 x, uint16 y, MMI_MESSAGE_ID_E msg_id)
{
    uint16 i;
    BOOLEAN is_touch = FALSE;
    
    if(PNULL == s_fish_array)
    {
        return;
    }
                
    for (i = 0; i < FISH_TOTAL; ++i)
    {
        if (Check_Fish_Touch(&s_fish_array[i], x, y))
        {
            s_fish_array[i].speed = FISH_TOUCH_MAX_SPEED;
            s_fish_array[i].is_touch = TRUE;
            s_fish_array[i].angle = Get_Random_Fish_Angle(s_fish_array[i].index);
            is_touch =  TRUE;
        }
    }

    if (is_touch)
    {   
        switch(msg_id)//NEWMS00165600
        {
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
            case MSG_TP_PRESS_DOWN:
        if (s_seaworld_nv.need_vibrate)
        {
                    StopFishVibratorTimer();
                    StartFishVibratorTimer();
                    MMIAPISET_SetAutoStopVibrator(TRUE, MMISET_RING_TYPE_OTHER, FISH_VIBRATE_TIME);
                    //MMISRVAUD_StartVibrate(MMISRVAUD_PRI_NORMAL, 50, 0, 1, PNULL);
                }
                break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
            case MSG_TP_PRESS_UP:
                if (s_seaworld_nv.need_vibrate && s_vibrator_timer_id > 0)
                {  
                    StopFishTPVibrator(s_vibrator_timer_id, 0);
                }
                break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
            default:
                break;
        }
    }
}
/*****************************************************************************/
//  Description : 交互事件处理
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SeaworldHandleEvent(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    GUI_POINT_T   point       = {0};
        
    switch(msg_id)
    {
    case MSG_TP_PRESS_UP:
    case MSG_TP_PRESS_DOWN:
        point.x = MMK_GET_TP_X(param);
        point.y = MMK_GET_TP_Y(param);
        Seaworld_PointProc(point.x,point.y, msg_id);//NEWMS00165600
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
LOCAL uint32 SeaworldGetType(void)
{
    return MMI_LIVE_WALLPAPER_TYPE_SEAWORLD;
}
/*****************************************************************************/
//  Description : 获得动态壁纸ThumbNail信息
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL void SeaworldGetThumbNailInfo(LW_THUMBNAIL_INFO *thumbnail_info_ptr)
{
    if(thumbnail_info_ptr)
    {
        thumbnail_info_ptr->is_image_file = FALSE;
        thumbnail_info_ptr->image_id = IMAGE_WALLPAPER_SEAWORLD_THUMBNAIL;

        thumbnail_info_ptr->is_not_string = FALSE;
        thumbnail_info_ptr->text_id = TXT_SET_WALLPAPER_SEAWORLD;
        thumbnail_info_ptr->instance_mem_pool_size = BLOCK_MEM_POOL_SIZE_SEAWORLD;
    }
}
/*****************************************************************************/
//  Description : 获得动态壁纸选项列表
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL OPTIONLIST *SeaworldGetOptionList(void)
{
    return (OPTIONLIST*)&s_seaworld_option_list;
}  
LOCAL void StopFishVibratorTimer(void)
{
#ifdef TOUCH_PANEL_SUPPORT /* lint_lai */
    if (0 < s_vibrator_timer_id)
    {
        MMK_StopTimer(s_vibrator_timer_id);
        s_vibrator_timer_id = 0;
    }
#endif
}
LOCAL void StartFishVibratorTimer(void)
{
#ifdef TOUCH_PANEL_SUPPORT /* lint_lai */
    if (0 == s_vibrator_timer_id)
    {
        s_vibrator_timer_id = MMK_CreateTimerCallback(FISH_VIBRATE_TIME,StopFishTPVibrator,NULL,FALSE);
    }
#endif
}
LOCAL void StopFishTPVibrator(uint8 timer_id, uint32 param)
{
#ifdef TOUCH_PANEL_SUPPORT /* lint_lai */
    StopFishVibratorTimer();
    MMIAPISET_SetAutoStopVibrator(FALSE, MMISET_RING_TYPE_OTHER, FISH_VIBRATE_TIME);
#endif
}
//NEWMS00165600 end
#endif //LIVE_WALLPAPER_SUPPORT_SEAWORLD
#endif //LIVE_WALLPAPER_SUPPORT_SEAWORLD


/*Edit by script, ignore 4 case. Thu Apr 26 19:00:58 2012*/ //IGNORE9527
