/*****************************************************************************
** File Name:      mmi_livewallpaper_sakura.c                                            *
** Author:                                                                   *
** Date:                                                          *
** Copyright:      2004 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is for live wallpaper                  *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 08/2011      Listen.Li       Create                                  *
** 08/2011       Arvin.wu          Modify 
******************************************************************************/
#define _MMI_LIVEWALLPAPER_SAKURA_C_

#include "mmi_app_livewallpaper_trc.h"
#ifdef LIVE_WALLPAPER_SUPPORT_SAKURA
/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "graphics_draw.h"
#include "mmi_image.h"
#include "mmi_text.h"
#include "mmiset_text.h"
#ifdef MMI_GRID_IDLE_SUPPORT
#include "mmiwidget_nv.h"
#endif
#include "mmilivewallpaper_export.h"
#include "mmi_livewallpaper_image.h"
#include "mmi_livewallpaper_text.h"
#include "mmi_livewallpaper_sakura.h"
#include "mmi_livewallpaper_option.h"
#include "mmisrvaud_api.h"
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#ifdef MAINLCD_SIZE_320X480
#define X_AXIS_SPEED_DISTANCE    20    /*X轴方向每个速度代表的长度*/
#define Y_AXIS_SPEED_DISTANCE    20
#define INDUCTION_MAX_DISTANCE    120    /*在X和Y方向最大的感应距离*/
#define A_SPEED    0.05    // 加速度
#else
#define X_AXIS_SPEED_DISTANCE    20    /*X轴方向每个速度代表的长度*/
#define Y_AXIS_SPEED_DISTANCE    20
#define INDUCTION_MAX_DISTANCE    80    /*在X和Y方向最大的感应距离*/
#define A_SPEED    0.01    // 加速度
#endif

#define SAKURA_TOTAL    5
#define PETAL_TOTAL    12
#define POLLEN_TOTAL    30
#define PETAL_FRAME_MAX    60
#define PETAL_LINE_FRAME_NUM    10    /*Petal每行的图片帧数*/
#define BLOCK_MEM_POOL_SIZE_SAKURA (600 * 1024)   // Set a  MEM POOL for different livewallpaper respectively;

typedef struct 
{
    const void				*data_ptr;      
    uint32					data_size;
    IMGREF_DATA_FORMAT_E    data_type;      
    uint16					width;             
    uint16					height;
    int16					center_x;       //horizontal coordinate of rotation center 
    int16					center_y;       //vertical coordinate of rotation center 
}SAKURA_ROTATE_IMAGE_T;


/*---------------------------------------------------------------------------*/
typedef struct
{
    MMI_IMAGE_ID_T fg_id;    // 前景ID
    MMI_IMAGE_ID_T bg_id;    // 背景ID
    MMI_IMAGE_ID_T sakura_id;    // 樱花ID
    MMI_IMAGE_ID_T petal1_id;    // 花瓣ID 1
    MMI_IMAGE_ID_T petal2_id;    // 花瓣ID 2
    MMI_IMAGE_ID_T moon_id;
    MMI_IMAGE_ID_T pollen_id;

    uint16    petal_color;    // sakura和petal的颜色
    uint16    petal_type;    // 当前飘动物体的类型0:sakura    1: petal    2:随机,两者都包含    

    BOOLEAN need_pollen;
    BOOLEAN need_moon;
    BOOLEAN need_vibrate;
}GLOBAL_SAKURA_WALLPAPER_INFO_T;

/*
    Vt = V0 + at
    h = V0*t + a*t*t/2
*/
typedef struct
{
    MMI_IMAGE_ID_T img_id;    // 前景ID

    uint16 index;
    int16 left;
    int16 top;
    uint16 width;
    uint16 height;   
    uint16 rotate_width;
    uint16 rotate_height;
    double x_speed;    // 自身的速度，X轴方向
    double y_speed;    // 
    double a_speed;    // 加速度
    int16 rot_angle;    // 当前旋转的角度
    int16 rot_piece_angle;    // 每次旋转增加的角度
    
    BOOLEAN is_moving;    // 当前是否在屏幕上游动
}SAKURA_INFO_T;

typedef struct
{
    MMI_IMAGE_ID_T img_id;    // 前景ID
    int16 cur_frame;    // 当前帧数

    uint16 index;
    int16 left;
    int16 top;
    uint16 width;
    uint16 height;   
    double x_speed;    // 自身的速度，X轴方向
    double y_speed;    // 
    double a_speed;    // 加速度
    
    BOOLEAN is_moving;    // 当前是否在屏幕上游动
}PETAL_INFO_T;

typedef struct
{
    MMI_IMAGE_ID_T img_id;    // 前景ID

    uint16 index;
    int16 left;
    int16 top;
    uint16 width;
    uint16 height;   
    double x_speed;    // 自身的速度，X轴方向
    double y_speed;    // 
    double a_speed;    // 加速度
    
    BOOLEAN is_moving;    // 当前是否在屏幕上游动
}POLLEN_INFO_T;

typedef struct
{
    uint8 *data_ptr;
    uint32 data_size;
    uint16 width;
    uint16 height;
}ALLOC_BUFFER_INFO_T;


/**--------------------------------------------------------------------------*
 **                         LOCAL DEFINITION                                 *
 **--------------------------------------------------------------------------*/
LOCAL GUI_POINT_T s_touch_point = {0};
LOCAL GLOBAL_SAKURA_WALLPAPER_INFO_T s_global_wp_info = {0};
LOCAL SAKURA_INFO_T *s_sakura_array = PNULL;
LOCAL PETAL_INFO_T *s_petal_array = PNULL;
LOCAL POLLEN_INFO_T *s_pollen_array = PNULL;

LOCAL ALLOC_BUFFER_INFO_T s_petal1_buffer = {0};    // petrl1全图buffer
LOCAL ALLOC_BUFFER_INFO_T s_petal2_buffer = {0};    // petrl2全图buffer
LOCAL ALLOC_BUFFER_INFO_T s_petal_frame_buffer = {0};    // 用于存放当前帧的buffer
LOCAL ALLOC_BUFFER_INFO_T s_sakura_buffer = {0};    // sakura buffer
LOCAL ALLOC_BUFFER_INFO_T s_sakura_rotate_buffer = {0};    // sakura rotate buffer
LOCAL ALLOC_BUFFER_INFO_T s_moon_buffer = {0};
LOCAL ALLOC_BUFFER_INFO_T s_pollen_buffer = {0};

// NV
LOCAL WALLPAPER_SAKURA_NV_T s_sakura_nv= {0};

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
LOCAL void Single_Petal_Init(PETAL_INFO_T *petal, uint16 index);
LOCAL void Single_Sakura_Init(SAKURA_INFO_T *sakura, uint16 index);
LOCAL void Single_Pollen_Init(POLLEN_INFO_T *pollen, uint16 index);
LOCAL uint16 Sakura_Option_Get_BgID(void);
LOCAL void Sakura_Option_Set_BgID(uint16 value_id);
LOCAL uint16 Sakura_Option_Get_FgID(void);
LOCAL void Sakura_Option_Set_FgID(uint16 value_id);
LOCAL uint16 Sakura_Option_Get_Color(void);
LOCAL void Sakura_Option_Set_Color(uint16 value_id);
LOCAL uint16 Sakura_Option_Get_Need_Pollen(void);
LOCAL void Sakura_Option_Set_Need_Pollen(uint16 value_id);
LOCAL uint16 Sakura_Option_Get_Need_Moon(void);
LOCAL void Sakura_Option_Set_Need_Moon(uint16 value_id);

/*****************************************************************************/
//  Description : 初始化
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SakuraInit(void);
/*****************************************************************************/
//  Description : 构造函数
//  Global resource dependence : 
//  Author:arvin.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SakuraConstruct(MMI_WIN_ID_T win_id);

/*****************************************************************************/
//  Description : 准备背景数据
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SakuraDrawBg(MMI_WIN_ID_T win_id,const GUI_LCD_DEV_INFO *dev_info_ptr);

/*****************************************************************************/
//  Description : 算法
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL void SakuraCalculate(LW_CAL_PARAM *lw_cal_param);
/*****************************************************************************/
//  Description : 释放
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:销毁临时申请的变量
/*****************************************************************************/
LOCAL void SakuraDestruct(void);
/*****************************************************************************/
//  Description : 交互事件处理
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SakuraHandleEvent(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/*****************************************************************************/
//  Description : 获得动态壁纸类型
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL uint32 SakuraGetType(void);
/*****************************************************************************/
//  Description : 获得动态壁纸ThumbNail信息
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL void SakuraGetThumbNailInfo(LW_THUMBNAIL_INFO *thumbnail_info_ptr);
/*****************************************************************************/
//  Description : 获得动态壁纸选项列表
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL OPTIONLIST *SakuraGetOptionList(void);

LOCAL MMI_TEXT_ID_T s_sakura_option_bg_text[10]=
{
    TXT_SET_WALLPAPER_SAKURA_OPTION_COLOR1,
    TXT_SET_WALLPAPER_SAKURA_OPTION_COLOR2,
    TXT_SET_WALLPAPER_SAKURA_OPTION_COLOR3,
    TXT_SET_WALLPAPER_SAKURA_OPTION_COLOR4,
    TXT_SET_WALLPAPER_SAKURA_OPTION_COLOR5,
    TXT_SET_WALLPAPER_SAKURA_OPTION_COLOR6,
    TXT_SET_WALLPAPER_SAKURA_OPTION_COLOR7,
    TXT_SET_WALLPAPER_SAKURA_OPTION_COLOR8,
    TXT_SET_WALLPAPER_SAKURA_OPTION_COLOR9,
    TXT_SET_WALLPAPER_SAKURA_OPTION_COLOR10
};
LOCAL MMI_TEXT_ID_T s_sakura_option_fg_text[10]=
{
    TXT_SET_WALLPAPER_SAKURA_OPTION_COLOR1,
    TXT_SET_WALLPAPER_SAKURA_OPTION_COLOR2,
    TXT_SET_WALLPAPER_SAKURA_OPTION_COLOR3,
    TXT_SET_WALLPAPER_SAKURA_OPTION_COLOR4,
    TXT_SET_WALLPAPER_SAKURA_OPTION_COLOR5,
    TXT_SET_WALLPAPER_SAKURA_OPTION_COLOR6,
    TXT_SET_WALLPAPER_SAKURA_OPTION_COLOR7,
    TXT_SET_WALLPAPER_SAKURA_OPTION_COLOR8,
    TXT_SET_WALLPAPER_SAKURA_OPTION_COLOR9,
    TXT_SET_WALLPAPER_SAKURA_OPTION_COLOR10
};
LOCAL MMI_TEXT_ID_T s_sakura_option_color_text[10]=
{
    TXT_SET_WALLPAPER_SAKURA_OPTION_COLOR1,
    TXT_SET_WALLPAPER_SAKURA_OPTION_COLOR2,
    TXT_SET_WALLPAPER_SAKURA_OPTION_COLOR3,
    TXT_SET_WALLPAPER_SAKURA_OPTION_COLOR4,
    TXT_SET_WALLPAPER_SAKURA_OPTION_COLOR5,
    TXT_SET_WALLPAPER_SAKURA_OPTION_COLOR6,
    TXT_SET_WALLPAPER_SAKURA_OPTION_COLOR7,
    TXT_SET_WALLPAPER_SAKURA_OPTION_COLOR8,
    TXT_SET_WALLPAPER_SAKURA_OPTION_COLOR9,
    TXT_SET_WALLPAPER_SAKURA_OPTION_COLOR10
};
LOCAL MMI_TEXT_ID_T s_sakura_option_need_pollen_text[2]=
{
    TXT_CLOSE,
    TXT_OPEN
};
LOCAL MMI_TEXT_ID_T s_sakura_option_need_moon_text[2]=
{
    TXT_CLOSE,
    TXT_OPEN
};

LOCAL SelectList s_sakura_option_bg_sl={10, s_sakura_option_bg_text, Sakura_Option_Get_BgID,Sakura_Option_Set_BgID};
LOCAL SelectList s_sakura_option_fg_sl={10, s_sakura_option_fg_text, Sakura_Option_Get_FgID,Sakura_Option_Set_FgID};
LOCAL SelectList s_sakura_option_color_sl={10, s_sakura_option_color_text, Sakura_Option_Get_Color,Sakura_Option_Set_Color};
LOCAL SelectList s_sakura_option_need_pollen_sl={2, s_sakura_option_need_pollen_text, Sakura_Option_Get_Need_Pollen,Sakura_Option_Set_Need_Pollen};
LOCAL SelectList s_sakura_option_need_moon_sl={2, s_sakura_option_need_moon_text, Sakura_Option_Get_Need_Moon,Sakura_Option_Set_Need_Moon};

LOCAL ListItem s_sakura_option_items[6]=
{
	{TXT_COMM_WALLPAPER_BACKGROUND,TYPE_SELECT_LIST,&s_sakura_option_bg_sl},
	{TXT_SET_WALLPAPER_SAKURA_OPTION_FG_TITLE,TYPE_SELECT_LIST,&s_sakura_option_fg_sl},
	{TXT_SET_WALLPAPER_SAKURA_OPTION_PETAL_TITLE,TYPE_SELECT_LIST,&s_sakura_option_color_sl},
	{TXT_SET_WALLPAPER_SAKURA_OPTION_NEED_POLLEN_TITLE,TYPE_SELECT_LIST,&s_sakura_option_need_pollen_sl},
	{TXT_SET_WALLPAPER_SAKURA_OPTION_NEED_MOON_TITLE,TYPE_SELECT_LIST,&s_sakura_option_need_moon_sl},
      {0}
};

LOCAL OPTIONLIST s_sakura_option_list={TXT_SET_WALLPAPER_SAKURA, 5, s_sakura_option_items};

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


/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/
PUBLIC const MMI_LIVE_WALLPAPER_ITEM_INFO_T g_sakura_Instance =
{
    SakuraInit,
    SakuraConstruct,
    SakuraDrawBg,
    SakuraCalculate,
    SakuraDestruct,
    SakuraHandleEvent,
    PNULL,
    SakuraGetType,
    SakuraGetThumbNailInfo,
    PNULL,
    PNULL,
    PNULL,
    SakuraGetOptionList
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
LOCAL BOOLEAN MMI_DecompressARGB(
                         const SAKURA_ROTATE_IMAGE_T *src_img_ptr, 
                         SAKURA_ROTATE_IMAGE_T *dst_img_ptr, 
                         uint8 *dst_buf_ptr, 
                         uint32 dst_buf_size)
{
#ifndef WIN32_TEST
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
#endif

	return TRUE;
}

LOCAL BOOLEAN MMI_RotateArbitrary(
            SAKURA_ROTATE_IMAGE_T *handle_img_ptr, 
            int32 angle, 
            uint8 *rotated_buf_ptr, 
            uint32 rotated_buf_size,
            uint16 *out_w,
            uint16 *out_h
            )
{
	SAKURA_ROTATE_IMAGE_T handle_img = {0};
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
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_LIVEWALLPAPER_SAKURA_510_112_2_18_2_32_21_32,(uint8*)"ddddddddd", angle, rot_in.src_size.w, rot_in.src_size.h, rot_in.src_center.x, rot_in.src_center.y,rot_in.target_size.w, rot_in.target_size.h, rot_in.target_center.x, rot_in.target_center.y);

	
	if(SCI_SUCCESS != GRAPH_RotateArbitrary(&rot_in, &rot_out))
	{
		return FALSE;
	}

	*out_w = rot_out.output_size.w;
	*out_h = rot_out.output_size.h;

	//SCI_TRACE_LOW:"--Rotate Image after-- angle:%d  "
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_LIVEWALLPAPER_SAKURA_523_112_2_18_2_32_21_33,(uint8*)"ddddd",angle, rot_out.output_size.w, rot_out.output_size.h, rot_out.output_offset.x, rot_out.output_offset.y);

	return TRUE;
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
    SAKURA_ROTATE_IMAGE_T rot_img = {0};
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
         MMILIVEWALLPAPER_FREE(decomp_buf_ptr);
         decomp_buf_ptr = PNULL;
    }
    
    *decode_buf = PNULL;
    *decode_size = 0;
    *width = *height = 0;
    
    return FALSE;
}

LOCAL void MMI_Rotate_Sakura_Buffer(
                ALLOC_BUFFER_INFO_T *src_buffer,
                ALLOC_BUFFER_INFO_T *rot_buffer,
                SAKURA_INFO_T *sakura)
{
    SAKURA_ROTATE_IMAGE_T rot_img = {0};

    rot_img.data_ptr = src_buffer->data_ptr;
    rot_img.data_size= src_buffer->data_size;
    rot_img.width = src_buffer->width;
    rot_img.height = src_buffer->height;
    rot_img.center_x = src_buffer->width/2;
    rot_img.center_y = src_buffer->height/2;
    rot_img.data_type = IMGREF_FORMAT_ARGB888;
    
    if(!MMI_RotateArbitrary(
    		&rot_img, 
    		sakura->rot_angle, 
    		rot_buffer->data_ptr, 
    		rot_buffer->data_size,
    		&sakura->rotate_width,
    		&sakura->rotate_height
    		))
    {
        //SCI_TRACE_LOW:"[MMI_Rotate_Sakura_Buffer Fail] - angle: %d\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_LIVEWALLPAPER_SAKURA_617_112_2_18_2_32_21_34,(uint8*)"d", sakura->rot_angle);
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


#if 1

LOCAL uint16 Get_Sakura_Random_XPos(uint16 index)
{
    uint16 lcd_width = MMITHEME_GetMainScreenRightPixel() + 1;
    uint16 random = 0;
    uint32 time = SCI_GetTickCount() + index;
    srand(time);
    random = rand() % lcd_width;

    return random;
}

LOCAL int16 Get_Sakura_Random_X_Speed(uint16 index)
{
    int16 random = 0;
    uint32 time = SCI_GetTickCount() + index;
    srand(time);

#ifdef  MAINLCD_SIZE_320X480   
    random = rand() % 10;
    random -= 5;
#else
    random = rand() % 4;
    random -= 2;
#endif

    if (random >= 0)
    {
        random += 2;
    }
    else
    {
        random -= 1;
    }

    return random;
}

LOCAL uint16 Get_Sakura_Random_Y_Speed(uint16 index)
{
    uint16 random = 0;
    uint32 time = SCI_GetTickCount() + index;
    srand(time);

#ifdef  MAINLCD_SIZE_320X480
    random = rand() % 3 + 2;
#elif defined(MAINLCD_SIZE_240X400)
    random = rand() % 2 + 2;
#else
    random = rand() % 2 + 1;
#endif

    return random;
}

LOCAL uint16 Get_Sakura_Random_Piece_Angle(uint16 index)
{
    uint16 random = 0;
    uint32 time = SCI_GetTickCount() + index;
    srand(time);
    random = rand() % 5 + 5;

    return random;
}


LOCAL uint16 (*Get_Petal_Random_XPos)(uint16) = Get_Sakura_Random_XPos;
LOCAL int16 (*Get_Petal_Random_X_Speed)(uint16) = Get_Sakura_Random_X_Speed;
LOCAL uint16 (*Get_Petal_Random_Y_Speed)(uint16) = Get_Sakura_Random_Y_Speed;

LOCAL uint16 (*Get_Pollen_Random_XPos)(uint16) = Get_Sakura_Random_XPos;
LOCAL int16 Get_Pollen_Random_X_Speed(uint16 index)
{
    int16 random = 0;
    uint32 time = SCI_GetTickCount() + index;
    srand(time);
    random = rand() % 4;

    random -= 2;

    if (random >= 0)
    {
        random += 1;
    }
    else
    {
        
    }

    return random;
}

LOCAL uint16 Get_Pollen_Random_Y_Speed(uint16 index)
{
    uint16 random = 0;
    uint32 time = SCI_GetTickCount() + index;
    srand(time);
    random = rand() % 2 + 1;

    return random;
}

LOCAL void Set_Sakura_Speed_After_Touch(SAKURA_INFO_T  *sakura)
{
    int16 center_x = 0;
    int16 center_y = 0;
    int16 x_distance = 0;
    int16 y_distance = 0;
    int16 x_speed = 0;
    int16 y_speed = 0;

    if (PNULL == sakura)
    {
        return;
    }
     
    if (!sakura->is_moving)
    {
        return;
    }

    center_x = sakura->left + (sakura->rotate_width-1) / 2;
    center_y = sakura->top + (sakura->rotate_height-1) / 2;
    x_distance = abs(s_touch_point.x - center_x);
    y_distance = abs(s_touch_point.y - center_y);

    if (x_distance > INDUCTION_MAX_DISTANCE || y_distance > INDUCTION_MAX_DISTANCE)
    {
        return;
    }
    
    x_speed = (INDUCTION_MAX_DISTANCE - x_distance) / X_AXIS_SPEED_DISTANCE;
    
    if (s_touch_point.x >= center_x)
    {
        sakura->x_speed -= x_speed;
    }
    else
    {
        sakura->x_speed = x_speed;
    }

    y_speed = (INDUCTION_MAX_DISTANCE - y_distance) / Y_AXIS_SPEED_DISTANCE;
    if (s_touch_point.y >= center_y)
    {
        sakura->y_speed -= y_speed;
    }
    else
    {
        sakura->y_speed += y_speed;
    }
}

LOCAL void Set_Petal_Speed_After_Touch(PETAL_INFO_T  *petal)
{
    int16 center_x = 0;
    int16 center_y = 0;
    int16 x_distance = 0;
    int16 y_distance = 0;
    int16 x_speed = 0;
    int16 y_speed = 0;
 
    if (PNULL == petal)
    {
        return;
    }
     
    if (!petal->is_moving)
    {
        return;
    }

    center_x = petal->left + (petal->width-1) / 2;
    center_y = petal->top + (petal->height-1) / 2;
    x_distance = abs(s_touch_point.x - center_x);
    y_distance = abs(s_touch_point.y - center_y);

    if (x_distance > INDUCTION_MAX_DISTANCE || y_distance > INDUCTION_MAX_DISTANCE)
    {
        return;
    }
    
    x_speed = (INDUCTION_MAX_DISTANCE - x_distance) / X_AXIS_SPEED_DISTANCE;
    if (s_touch_point.x >= center_x)
    {
        petal->x_speed -= x_speed;
    }
    else
    {
        petal->x_speed = x_speed;
    }

    y_speed = (INDUCTION_MAX_DISTANCE - y_distance) / Y_AXIS_SPEED_DISTANCE;
    if (s_touch_point.y >= center_y)
    {
        petal->y_speed -= y_speed;
    }
    else
    {
        petal->y_speed += y_speed;
    }
}

LOCAL void Set_Pollen_Speed_After_Touch(POLLEN_INFO_T  *pollen)
{
    int16 center_x = 0;
    int16 center_y = 0;
    int16 x_distance = 0;
    int16 y_distance = 0;
    int16 x_speed = 0;
    int16 y_speed = 0;
  
    if (PNULL == pollen)
    {
        return;
    }
     
    if (!pollen->is_moving)
    {
        return;
    }

    center_x = pollen->left + (pollen->width-1) / 2;
    center_y = pollen->top + (pollen->height-1) / 2;
    x_distance = abs(s_touch_point.x - center_x);
    y_distance = abs(s_touch_point.y - center_y);

    if (x_distance > INDUCTION_MAX_DISTANCE || y_distance > INDUCTION_MAX_DISTANCE)
    {
        return;
    }
    
    x_speed = (INDUCTION_MAX_DISTANCE - x_distance) / X_AXIS_SPEED_DISTANCE;
    if (s_touch_point.x >= center_x)
    {
        pollen->x_speed -= x_speed;
    }
    else
    {
        pollen->x_speed = x_speed;
    }

    y_speed = (INDUCTION_MAX_DISTANCE - y_distance) / Y_AXIS_SPEED_DISTANCE;
    if (s_touch_point.y >= center_y)
    {
        pollen->y_speed -= y_speed;
    }
    else
    {
        pollen->y_speed += y_speed;
    }
}

LOCAL void Check_Sakura_Moving_Status(SAKURA_INFO_T  *sakura)
{    
    if (PNULL == sakura)
    {
        return;
    }
     
    if (sakura->left + sakura->rotate_width < 0 
        || sakura->left > MMITHEME_GetMainScreenRightPixel()
        || sakura->top + sakura->rotate_height < 0
        || sakura->top > MMITHEME_GetMainScreenBottomPixel())
    {
        sakura->is_moving = FALSE;
    } 

    if (!sakura->is_moving)
    {
        Single_Sakura_Init(sakura, sakura->index);
        sakura->is_moving = FALSE;
    }
}

LOCAL void Check_Petal_Moving_Status(PETAL_INFO_T  *petal)
{        
    if (PNULL == petal)
    {
        return;
    }
     
    if (petal->left + petal->width < 0 
        || petal->left > MMITHEME_GetMainScreenRightPixel()
        || petal->top + petal->height < 0
        || petal->top > MMITHEME_GetMainScreenBottomPixel())
    {
        petal->is_moving = FALSE;
    } 

    if (!petal->is_moving)
    {
        Single_Petal_Init(petal, petal->index);
        petal->is_moving = FALSE;
    }
}

LOCAL void Check_Pollen_Moving_Status(POLLEN_INFO_T  *pollen)
{    
    if (PNULL == pollen)
    {
        return;
    }
     
    if (pollen->left + pollen->width < 0 
        || pollen->left > MMITHEME_GetMainScreenRightPixel()
        || pollen->top + pollen->height < 0
        || pollen->top > MMITHEME_GetMainScreenBottomPixel())
    {
        pollen->is_moving = FALSE;
    } 

    if (!pollen->is_moving)
    {
        Single_Pollen_Init(pollen, pollen->index);
        pollen->is_moving = FALSE;
    }
}

LOCAL void Single_Sakura_Init(SAKURA_INFO_T *sakura, uint16 index)
{
    uint16 width=0, height=0;
    MMI_HANDLE_T win_id = MAIN_IDLE_WIN_ID;

    sakura->img_id = s_global_wp_info.sakura_id;
    sakura->index = index;
    sakura->left = Get_Sakura_Random_XPos(index);
    sakura->top = -10;
    GUIRES_GetImgWidthHeight(&width, &height, s_global_wp_info.sakura_id, win_id);
    sakura->width = width;
    sakura->height = height;
    sakura->rotate_width = 0;
    sakura->rotate_height = 0;
    sakura->x_speed = Get_Sakura_Random_X_Speed(index);
    sakura->y_speed = Get_Sakura_Random_Y_Speed(index);
    sakura->a_speed = A_SPEED;
    sakura->rot_angle = 0;
    sakura->rot_piece_angle = Get_Sakura_Random_Piece_Angle(index);
    sakura->is_moving = (index%2) ? TRUE : FALSE;
}

LOCAL void Single_Sakura_Move(SAKURA_INFO_T *sakura)
{
    sakura->y_speed += sakura->a_speed;
    sakura->left += (int16)sakura->x_speed;
    sakura->top += (int16)sakura->y_speed;
    sakura->rot_angle += sakura->rot_piece_angle;
    if (sakura->rot_angle >= 360)
    {
        sakura->rot_angle -= 360;
    }
}

LOCAL void Single_Petal_Init(PETAL_INFO_T *petal, uint16 index)
{
    uint16 width=0, height=0;
    MMI_HANDLE_T win_id = MAIN_IDLE_WIN_ID;

    petal->img_id = (SCI_GetTickCount()%2) ? s_global_wp_info.petal1_id : s_global_wp_info.petal2_id;
    petal->cur_frame = -1;
    petal->index = index;
    petal->left = Get_Petal_Random_XPos(index);
    petal->top = -10;
    GUIRES_GetImgWidthHeight(&width, &height, s_global_wp_info.petal1_id, win_id);
    petal->height = petal->width = width/10;
    petal->x_speed = Get_Petal_Random_X_Speed(index);
    petal->y_speed = Get_Petal_Random_Y_Speed(index);
    petal->a_speed = A_SPEED;
    petal->is_moving = (index%2) ? TRUE : FALSE;
}

LOCAL void Single_Petal_Move(PETAL_INFO_T *petal)
{
    petal->cur_frame++;
    if (petal->cur_frame >= PETAL_FRAME_MAX)
    {
        petal->cur_frame = 0;
    }
    petal->y_speed += petal->a_speed;
    petal->left += (int16)petal->x_speed;
    petal->top += (int16)petal->y_speed;
}

LOCAL void Single_Pollen_Init(POLLEN_INFO_T *pollen, uint16 index)
{
    uint16 width=0, height=0;
    MMI_HANDLE_T win_id = MAIN_IDLE_WIN_ID;

    pollen->img_id = s_global_wp_info.pollen_id;
    pollen->index = index;
    pollen->left = Get_Pollen_Random_XPos(index);
    pollen->top = 0;
    GUIRES_GetImgWidthHeight(&width, &height, s_global_wp_info.pollen_id, win_id);
    pollen->width = width;
    pollen->height = height;
    pollen->x_speed = Get_Pollen_Random_X_Speed(index);
    pollen->y_speed = Get_Pollen_Random_Y_Speed(index);
    pollen->a_speed = A_SPEED;
    pollen->is_moving = (index%2) ? TRUE : FALSE;
}

LOCAL void Single_Pollen_Move(POLLEN_INFO_T *pollen)
{
    pollen->y_speed += pollen->a_speed;
    pollen->left += (int16)pollen->x_speed;
    pollen->top += (int16)pollen->y_speed;
}

LOCAL void Add_Moving_Sakura(void)
{
    uint16 i = 0, j = 0;
    uint8 moving_sakura_num = 0;
    uint8 non_moving_sakura_num = 0;
    uint8 non_moving_sakura_array[SAKURA_TOTAL] = {0};

    if(PNULL == s_sakura_array)
    {
        return;
    }
      
    for (i = 0; i < SAKURA_TOTAL; ++i)
    {
        if (s_sakura_array[i].is_moving)
        {
            ++moving_sakura_num;
        }
        else
        {
            non_moving_sakura_array[j++] = i;
        }
    }

    non_moving_sakura_num = SAKURA_TOTAL - moving_sakura_num;
    
    if (moving_sakura_num < SAKURA_TOTAL)
    {
        uint16 random = 0;
        uint32 time = SCI_GetTickCount();
        srand(time);
        random = rand() % (non_moving_sakura_num + 1);

        for (i = 0; i < random; ++i)
        {
            s_sakura_array[non_moving_sakura_array[i]].is_moving = TRUE;
        }
    }
}

LOCAL void Add_Moving_Petal(void)
{   
    uint16 i = 0, j = 0;
    uint8 moving_petal_num = 0;
    uint8 non_moving_petal_num = 0;
    uint8 non_moving_petal_array[PETAL_TOTAL] = {0};

    if(PNULL == s_petal_array)
    {
        return;
    }
      
    for (i = 0; i < PETAL_TOTAL; ++i)
    {
        if (s_petal_array[i].is_moving)
        {
            ++moving_petal_num;
        }
        else
        {
            non_moving_petal_array[j++] = i;
        }
    }

    non_moving_petal_num = PETAL_TOTAL - moving_petal_num;
    
    if (moving_petal_num < PETAL_TOTAL)
    {
        uint16 random = 0;
        uint32 time = SCI_GetTickCount();
        srand(time);
        random = rand() % (non_moving_petal_num + 1);

        for (i = 0; i < random; ++i)
        {
            s_petal_array[non_moving_petal_array[i]].is_moving = TRUE;
        }
    }
}

LOCAL void Add_Moving_Pollen(void)
{
    uint16 i = 0, j = 0;
    uint8 moving_pollen_num = 0;
    uint8 non_moving_pollen_num = 0;
    uint8 non_moving_pollen_array[POLLEN_TOTAL] = {0};

    if(PNULL == s_pollen_array)
    {
        return;
    }
      
    for (i = 0; i < POLLEN_TOTAL; ++i)
    {
        if (s_pollen_array[i].is_moving)
        {
            ++moving_pollen_num;
        }
        else
        {
            non_moving_pollen_array[j++] = i;
        }
    }

    non_moving_pollen_num = POLLEN_TOTAL - moving_pollen_num;
    
    if (moving_pollen_num < POLLEN_TOTAL)
    {
        uint16 random = 0;
        uint32 time = SCI_GetTickCount();
        srand(time);
        random = rand() % (non_moving_pollen_num + 1);

        for (i = 0; i < random; ++i)
        {
            s_pollen_array[non_moving_pollen_array[i]].is_moving = TRUE;
        }
    }
}

LOCAL BOOLEAN Copy_Petal_Frame_Buffer(
                ALLOC_BUFFER_INFO_T* dst_buf, 
                ALLOC_BUFFER_INFO_T* src_buf, 
                uint16 frame,
                uint16 width, 
                uint16 height)
{
    uint16 i = 0, j = 0;
    uint32 x_off = 0;
    uint32 *dst_ptr = PNULL;
    uint32 *src_ptr = PNULL;
    uint16 row_num = 0, col_num = 0;

    if(PNULL == dst_buf || PNULL == src_buf)
    {
        return FALSE;
    }
    dst_ptr = (uint32 *)dst_buf->data_ptr;
    src_ptr = (uint32 *)src_buf->data_ptr;
    
    if(dst_ptr != PNULL && src_ptr != PNULL)
    {
        if (dst_buf->width > s_petal_frame_buffer.width || dst_buf->height > s_petal_frame_buffer.height)
        {
            return FALSE;
        }

        row_num = frame / PETAL_LINE_FRAME_NUM;    // 当前处于第几行
        col_num = frame % PETAL_LINE_FRAME_NUM;    // 当前处于第几列 
        x_off = row_num * (src_buf->width * height);    // 计算之前行的buffer size
        
        x_off += col_num * width;
        src_ptr += x_off;
        for (j = 0; j < height; ++j)
        {
            for (i = 0; i < width; ++i)
            {
                *(dst_ptr + i) = *(src_ptr + i);
            }
            dst_ptr += width;
            src_ptr += src_buf->width;
        }

        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
#endif

LOCAL void Init_Global_Sakura_WP_Info(void)
{
    /*temp setting*/
    s_global_wp_info.bg_id = IMAGE_WALLPAPER_SAKURA_BG8;
    s_global_wp_info.fg_id= IMAGE_WALLPAPER_SAKURA_FG7;
    s_global_wp_info.sakura_id= IMAGE_WALLPAPER_IMG_SAKURA7;
    s_global_wp_info.petal1_id= IMAGE_WALLPAPER_IMG_PETAL7_1;
    s_global_wp_info.petal2_id= IMAGE_WALLPAPER_IMG_PETAL7_2;
    s_global_wp_info.moon_id = IMAGE_WALLPAPER_SAKURA_MOON;
    s_global_wp_info.pollen_id = IMAGE_WALLPAPER_SAKURA_POLLEN;

    s_global_wp_info.petal_color = 6;
    s_global_wp_info.petal_type = 2;

    s_global_wp_info.need_pollen = TRUE;
    s_global_wp_info.need_moon = TRUE;
    s_global_wp_info.need_vibrate = FALSE;
}

LOCAL void Init_All_Sakura(void)
{
    uint16 i = 0;
    
    for (i = 0; i < SAKURA_TOTAL; ++i)
    {
        Single_Sakura_Init(&s_sakura_array[i], i);
    }
}

LOCAL void Init_All_Petal(void)
{
    uint16 i = 0;

    for (i = 0; i < PETAL_TOTAL; ++i)
    {
        Single_Petal_Init(&s_petal_array[i], i);
    }
}

LOCAL void Init_All_Pollen(void)
{
    uint16 i = 0;

    for (i = 0; i < POLLEN_TOTAL; ++i)
    {
        Single_Pollen_Init(&s_pollen_array[i], i);
    }
}

LOCAL void Sakura_Read_NV(void)
{
    MN_RETURN_RESULT_E return_value= MN_RETURN_FAILURE;
    
    return_value = MMI_ReadNVItem(MMINV_LIVE_WALLPAPER_SAKURA, &s_sakura_nv);
    if (MN_RETURN_SUCCESS != return_value)
    {
        s_sakura_nv.bg_id = s_global_wp_info.bg_id;
        s_sakura_nv.fg_id = s_global_wp_info.fg_id;
        s_sakura_nv.petal_color = s_global_wp_info.petal_color;
        s_sakura_nv.petal_type = s_global_wp_info.petal_type;
        s_sakura_nv.need_pollen = s_global_wp_info.need_pollen;
        s_sakura_nv.need_moon = s_global_wp_info.need_moon;
        MMI_WriteNVItem(MMINV_LIVE_WALLPAPER_SAKURA,&s_sakura_nv);
    }
    else
    {
        s_global_wp_info.bg_id = s_sakura_nv.bg_id;
        s_global_wp_info.fg_id = s_sakura_nv.fg_id;
        s_global_wp_info.petal_color = s_sakura_nv.petal_color;
        s_global_wp_info.petal_type = s_sakura_nv.petal_type;
        s_global_wp_info.need_pollen = s_sakura_nv.need_pollen;
        s_global_wp_info.need_moon = s_sakura_nv.need_moon;

        s_global_wp_info.sakura_id = IMAGE_WALLPAPER_IMG_SAKURA1 + s_global_wp_info.petal_color;
        s_global_wp_info.petal1_id = IMAGE_WALLPAPER_IMG_PETAL1_1 + s_global_wp_info.petal_color * 2;
        s_global_wp_info.petal2_id = s_global_wp_info.petal1_id + 1;
    }
}

LOCAL uint16 Sakura_Option_Get_BgID(void)
{
    uint16 result = 0;
    
    Sakura_Read_NV();

    result = s_sakura_nv.bg_id - IMAGE_WALLPAPER_SAKURA_BG1;

    return result;
}

LOCAL void Sakura_Option_Set_BgID(uint16 value_id)
{
    s_sakura_nv.bg_id = IMAGE_WALLPAPER_SAKURA_BG1 + value_id;
    
    MMI_WriteNVItem(MMINV_LIVE_WALLPAPER_SAKURA,&s_sakura_nv);

    s_global_wp_info.bg_id =  s_sakura_nv.bg_id;
}

LOCAL uint16 Sakura_Option_Get_FgID(void)
{
    uint16 result = 0;
    
    Sakura_Read_NV();

    result = s_sakura_nv.fg_id - IMAGE_WALLPAPER_SAKURA_FG1;

    return result;
}

LOCAL void Sakura_Option_Set_FgID(uint16 value_id)
{
    s_sakura_nv.fg_id = IMAGE_WALLPAPER_SAKURA_FG1 + value_id;
    
    MMI_WriteNVItem(MMINV_LIVE_WALLPAPER_SAKURA,&s_sakura_nv);

    s_global_wp_info.fg_id =  s_sakura_nv.fg_id;
}

LOCAL uint16 Sakura_Option_Get_Color(void)
{
    uint16 result = 0;
    
    Sakura_Read_NV();

    result = s_sakura_nv.petal_color;

    return result;
}

LOCAL void Sakura_Option_Set_Color(uint16 value_id)
{
    s_sakura_nv.petal_color = value_id;
    
    MMI_WriteNVItem(MMINV_LIVE_WALLPAPER_SAKURA,&s_sakura_nv);

    s_global_wp_info.petal_color =  s_sakura_nv.petal_color;
    s_global_wp_info.sakura_id = IMAGE_WALLPAPER_IMG_SAKURA1 + value_id;
    s_global_wp_info.petal1_id = IMAGE_WALLPAPER_IMG_PETAL1_1 + value_id*2;
    s_global_wp_info.petal2_id = s_global_wp_info.petal1_id + 1;
}

LOCAL uint16 Sakura_Option_Get_Need_Pollen(void)
{
    uint16 result = 0;
    
    Sakura_Read_NV();
    if (s_sakura_nv.need_pollen)
    {
        result = 1;
    }
    else
    {
        result = 0;
    }
    
    return result;
}

LOCAL void Sakura_Option_Set_Need_Pollen(uint16 value_id)
{
    if (value_id == 0)
    {
        s_sakura_nv.need_pollen = FALSE;
    }
    else if (value_id == 1)
    {
        s_sakura_nv.need_pollen = TRUE;
    }

    MMI_WriteNVItem(MMINV_LIVE_WALLPAPER_SAKURA,&s_sakura_nv);

    //s_global_wp_info.need_pollen = s_global_wp_info.need_pollen;    
}

LOCAL uint16 Sakura_Option_Get_Need_Moon(void)
{
    uint16 result = 0;
    
    Sakura_Read_NV();
    if (s_sakura_nv.need_moon)
    {
        result = 1;
    }
    else
    {
        result = 0;
    }
    
    return result;
}

LOCAL void Sakura_Option_Set_Need_Moon(uint16 value_id)
{
    if (value_id == 0)
    {
        s_sakura_nv.need_moon = FALSE;
    }
    else if (value_id == 1)
    {
        s_sakura_nv.need_moon = TRUE;
    }

    MMI_WriteNVItem(MMINV_LIVE_WALLPAPER_SAKURA,&s_sakura_nv);

  //  s_global_wp_info.need_moon = s_global_wp_info.need_moon;    
}
/*****************************************************************************/
//  Description : 初始化
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SakuraInit(void)
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
LOCAL BOOLEAN SakuraConstruct(MMI_WIN_ID_T win_id)
{
    BOOLEAN b_result = FALSE;

    do
    {
        //initial s_pollen_array;
        if(PNULL != s_pollen_array)
        {
            SCI_TRACE_LOW("SakuraConstruct, have a exception before Construct: PNULL != s_pollen_array ");
            MMILIVEWALLPAPER_FREE(s_pollen_array);
            s_pollen_array = PNULL;
        }
        s_pollen_array = MMILIVEWALLPAPER_ALLOC((sizeof(POLLEN_INFO_T))*POLLEN_TOTAL);
        if(PNULL == s_pollen_array)
        {
            break;
        }

        //initial s_petal_array;
        if(PNULL != s_petal_array)
        {
            SCI_TRACE_LOW("SakuraConstruct, have a exception before Construct: PNULL != s_petal_array ");
            MMILIVEWALLPAPER_FREE(s_petal_array);
            s_petal_array = PNULL;
        }
        s_petal_array = MMILIVEWALLPAPER_ALLOC((sizeof(PETAL_INFO_T))*PETAL_TOTAL);
        if(PNULL == s_petal_array)
        {
            break;
        }

        //initial s_sakura_array;
        if(PNULL != s_sakura_array)
        {
            SCI_TRACE_LOW("SakuraConstruct, have a exception before Construct: PNULL != s_sakura_array ");
            MMILIVEWALLPAPER_FREE(s_sakura_array);
            s_sakura_array = PNULL;
        }
        s_sakura_array = MMILIVEWALLPAPER_ALLOC((sizeof(SAKURA_INFO_T))*SAKURA_TOTAL);
        if(PNULL == s_sakura_array)
        {
            break;
        }

        //intial prepare;
        Init_Global_Sakura_WP_Info();
        Sakura_Read_NV();
        Init_All_Petal();
        Init_All_Sakura();
        if (s_global_wp_info.need_pollen)
        {
            Init_All_Pollen();
        }
    
        /* Init petal1 buffer */
        if (!MMI_Alloc_PNG_Decode_buffer(
                        s_global_wp_info.petal1_id,
                        win_id,
                        &s_petal1_buffer.data_ptr,
                        &s_petal1_buffer.data_size,
                        &s_petal1_buffer.width,
                        &s_petal1_buffer.height)
            )
         {
             break;
         }

        /* Init petal2 buffer */
        if (!MMI_Alloc_PNG_Decode_buffer(
                        s_global_wp_info.petal2_id,
                        win_id,
                        &s_petal2_buffer.data_ptr,
                        &s_petal2_buffer.data_size,
                        &s_petal2_buffer.width,
                        &s_petal2_buffer.height)
            )
         {
             break;
         }

        /* Init petal frame buffer */
        s_petal_frame_buffer.width = s_petal_array[0].width;
        s_petal_frame_buffer.height = s_petal_array[0].height;
        s_petal_frame_buffer.data_size = s_petal_array[0].width * s_petal_array[0].height *4;
        s_petal_frame_buffer.data_ptr = (uint8 *)MMILIVEWALLPAPER_ALLOC(s_petal_frame_buffer.data_size);
        if(PNULL == s_petal_frame_buffer.data_ptr)
         {
             break;
         }
        SCI_MEMSET(s_petal_frame_buffer.data_ptr, 0, s_petal_frame_buffer.data_size);

        /* Init sakura buffer */
        if (!MMI_Alloc_PNG_Decode_buffer(
                        s_global_wp_info.sakura_id,
                        win_id,
                        &s_sakura_buffer.data_ptr,
                        &s_sakura_buffer.data_size,
                        &s_sakura_buffer.width,
                        &s_sakura_buffer.height)
            )
         {
             break;
         }

         /* Init sakura rotate buffer */
        s_sakura_rotate_buffer.width = s_sakura_array[0].width + s_sakura_array[0].height;
        s_sakura_rotate_buffer.height = s_sakura_rotate_buffer.width;
        s_sakura_rotate_buffer.data_size = s_sakura_rotate_buffer.width *s_sakura_rotate_buffer.height *4;
        s_sakura_rotate_buffer.data_ptr = (uint8 *)MMILIVEWALLPAPER_ALLOC(s_sakura_rotate_buffer.data_size);
        if(PNULL == s_sakura_rotate_buffer.data_ptr)
         {
             break;
         }
        SCI_MEMSET(s_sakura_rotate_buffer.data_ptr, 0, s_sakura_rotate_buffer.data_size);

          /* Init moon buffer */
         if (s_global_wp_info.need_moon)
         {
           
            if (!MMI_Alloc_PNG_Decode_buffer(
                            s_global_wp_info.moon_id,
                            win_id,
                            &s_moon_buffer.data_ptr,
                            &s_moon_buffer.data_size,
                            &s_moon_buffer.width,
                            &s_moon_buffer.height)
                )
             {
                 break;
             }
         }

        /* Init pollen buffer */
        if (s_global_wp_info.need_pollen)
        {
            if (!MMI_Alloc_PNG_Decode_buffer(
                            s_global_wp_info.pollen_id,
                            win_id,
                            &s_pollen_buffer.data_ptr,
                            &s_pollen_buffer.data_size,
                            &s_pollen_buffer.width,
                            &s_pollen_buffer.height)
                )
             {
                 break;
             }
         }

        //no exception, have initiated successfully;
        b_result = TRUE;
    }while(0);

    if(!b_result)
    {
        //in the case of failure to allocate memory,we need to release all the mems allocated. 
        SCI_TRACE_LOW("SakuraConstruct, have a exception at allocating process in Construct() ");
        SakuraDestruct();
    }

    return b_result;
}


/*****************************************************************************/
//  Description : 准备背景数据
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SakuraDrawBg(MMI_WIN_ID_T win_id,const GUI_LCD_DEV_INFO *dev_info_ptr)
{
    BOOLEAN b_result = FALSE;
    GUI_POINT_T display_point = {0,0};
    uint8 *fg_ptr = PNULL;
    uint32 fg_size = 0;
    uint16 width = 0, height = 0;
    uint8 * wallpaper_layer_buf_ptr = PNULL;

    GUI_POINT_T point = {0};
    IMGREF_SIZE_T	pop_bg_window = {0,0};
    IMGREF_SIZE_T	pop_up_window = {0,0};
    FIX16_POINT_T	offset = {0};

    UILAYER_INFO_T layer_info_ptr;

    UILAYER_GetLayerInfo(dev_info_ptr, &layer_info_ptr);
                                 
    pop_bg_window.w = layer_info_ptr.layer_width;
    pop_bg_window.h = layer_info_ptr.layer_height;
    
    /* 绘制背景*/
    if (PNULL != dev_info_ptr)
    {
        b_result = GUIRES_DisplayImg(&display_point, 
		PNULL, 
		PNULL, 
		win_id, 
		s_global_wp_info.bg_id, 
		dev_info_ptr);
    }
    

    /* 绘制前景*/
    
    if (b_result)
    {
        b_result = MMI_Alloc_PNG_Decode_buffer(
                    s_global_wp_info.fg_id,
                    win_id,
                    &fg_ptr,
                    &fg_size,
                    &width,
                    &height);
        if (!b_result)
        {
            return b_result;
        }
     }

    wallpaper_layer_buf_ptr = UILAYER_GetLayerBufferPtr(dev_info_ptr);

    if(PNULL != wallpaper_layer_buf_ptr)
    {
        pop_up_window.w = width;
        pop_up_window.h = height;
        point.x = 0;
        point.y = 0;
        offset.x=(int16)point.x;
        offset.y=(int16)point.y;
        offset.x_dec=0;//(uint16)((point.x<<16)&0xffff);
        offset.y_dec=0;//(uint16)((point.y<<16)&0xffff);
                
        if (PNULL == fg_ptr)
        {
            return FALSE;
        }  
        S2D_Blending(
                pop_bg_window , 
                (uint16*)wallpaper_layer_buf_ptr, 
                pop_up_window , 
                offset, 
                (uint32*)fg_ptr, 
                (uint16*)wallpaper_layer_buf_ptr);        
    }

    if (fg_ptr != PNULL)
    {
        MMILIVEWALLPAPER_FREE(fg_ptr);//SCI_FREE
        fg_ptr = PNULL;
    }

    /*绘制月亮*/
    if (b_result && s_global_wp_info.need_moon)
    {
        b_result = MMI_Alloc_PNG_Decode_buffer(
                        s_global_wp_info.moon_id,
                        win_id,
                        &fg_ptr,
                        &fg_size,
                        &width,
                        &height);
        if (!b_result)
        {
            return b_result;
        }

        wallpaper_layer_buf_ptr = UILAYER_GetLayerBufferPtr(dev_info_ptr);

        if(PNULL != wallpaper_layer_buf_ptr)
        {
            pop_up_window.w = width;
            pop_up_window.h = height;
            point.x = 40;
            point.y = 20;
            offset.x=(int16)point.x;
            offset.y=(int16)point.y;
            offset.x_dec=0;//(uint16)((point.x<<16)&0xffff);
            offset.y_dec=0;//(uint16)((point.y<<16)&0xffff);

            if (PNULL == fg_ptr)
            {
                return FALSE;
            }  
            S2D_Blending(
                    pop_bg_window , 
                    (uint16*)wallpaper_layer_buf_ptr, 
                    pop_up_window , 
                    offset, 
                    (uint32*)fg_ptr, 
                    (uint16*)wallpaper_layer_buf_ptr);        
        }
        
        if (fg_ptr != PNULL)
        {
            MMILIVEWALLPAPER_FREE(fg_ptr);
            fg_ptr = PNULL;
        }
    }

    return b_result;
}
/*****************************************************************************/
//  Description : 算法
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL void SakuraCalculate(LW_CAL_PARAM *lw_cal_param)
{   
    uint16 i = 0;
    GUI_RECT_T rect = {0};
    IMGREF_SIZE_T	pop_bg_window = {0,0};
    IMGREF_SIZE_T	pop_up_window = {0,0};
    FIX16_POINT_T	offset = {0};
   
    if (PNULL == lw_cal_param || PNULL == lw_cal_param->dest_buf_info.buf_ptr)
    {
        return;
    }
    pop_bg_window.w = lw_cal_param->src_buf_info.w;
    pop_bg_window.h = lw_cal_param->src_buf_info.h;

//    if(PNULL != target_buf_ptr && PNULL != source_buf_ptr)
    if(PNULL != lw_cal_param->src_buf_info.buf_ptr) 
    {
        SCI_MEMCPY((GUI_COLOR_T*)lw_cal_param->dest_buf_info.buf_ptr, lw_cal_param->src_buf_info.buf_ptr,MMI_MAINSCREEN_WIDTH*MMI_MAINSCREEN_HEIGHT*sizeof(uint16));
    }
    /*  绘制pollen*/   
    if (s_global_wp_info.need_pollen)
    {
            if(PNULL == s_pollen_array)
            {
                return;
            }
             
        for (i = 0; i < POLLEN_TOTAL; ++i)
        {
            if (!s_pollen_array[i].is_moving)
            {
                continue;
            }
            Single_Pollen_Move(&s_pollen_array[i]);

            pop_up_window.w = s_pollen_array[i].width;
            pop_up_window.h = s_pollen_array[i].height;
            rect.left = s_pollen_array[i].left;
            rect.top = s_pollen_array[i].top;
            offset.x=(int16)rect.left;
            offset.y=(int16)rect.top;
            offset.x_dec=0;//(uint16)((rect.left<<16)&0xffff);
            offset.y_dec=0;//(uint16)((rect.top<<16)&0xffff);

            if (PNULL == s_pollen_buffer.data_ptr)
            {
                return;
            }          
            S2D_Blending(
                    pop_bg_window , 
                    lw_cal_param->dest_buf_info.buf_ptr, 
                    pop_up_window , 
                    offset, 
                    (uint32*)s_pollen_buffer.data_ptr, 
                    lw_cal_param->dest_buf_info.buf_ptr);
            
            
            Check_Pollen_Moving_Status(&s_pollen_array[i]);
        }

        Add_Moving_Pollen();
    }


    /*  绘制sakura*/   
        if(PNULL == s_sakura_array)
        {
            return;
        }
            
    for (i = 0; i < SAKURA_TOTAL; ++i)
    {           
        if (!s_sakura_array[i].is_moving)
        {
            continue;
        }
        Single_Sakura_Move(&s_sakura_array[i]);

        
        SCI_MEMSET(s_sakura_rotate_buffer.data_ptr, 0, s_sakura_rotate_buffer.data_size);
        MMI_Rotate_Sakura_Buffer(&s_sakura_buffer, &s_sakura_rotate_buffer, &s_sakura_array[i]);
        
        pop_up_window.w = s_sakura_array[i].rotate_width;
        pop_up_window.h = s_sakura_array[i].rotate_height;
        rect.left = s_sakura_array[i].left - (s_sakura_array[i].rotate_width - s_sakura_array[i].width + 1)/2;
        rect.top = s_sakura_array[i].top -(s_sakura_array[i].rotate_height - s_sakura_array[i].height + 1)/2;
        offset.x=(int16)rect.left;
        offset.y=(int16)rect.top;
        offset.x_dec=0;//(uint16)((rect.left<<16)&0xffff);
        offset.y_dec=0;//(uint16)((rect.top<<16)&0xffff);
        
        if (PNULL == s_sakura_rotate_buffer.data_ptr)
        {
            return;
        }    
        S2D_Blending(
                pop_bg_window , 
                lw_cal_param->dest_buf_info.buf_ptr, 
                pop_up_window , 
                offset, 
                (uint32*)s_sakura_rotate_buffer.data_ptr, 
                lw_cal_param->dest_buf_info.buf_ptr);
        
        
        Check_Sakura_Moving_Status(&s_sakura_array[i]);
    }

    Add_Moving_Sakura();


    /* 绘制petal */
    if(PNULL == s_petal_array)
    {
        return;
    }
         
    for (i = 0; i < PETAL_TOTAL; ++i)
    { 
        if (!s_petal_array[i].is_moving)
        {
            continue;
        }
        Single_Petal_Move(&s_petal_array[i]);

        SCI_MEMSET(s_petal_frame_buffer.data_ptr, 0, s_petal_frame_buffer.data_size);
        if (s_petal_array[i].img_id == s_global_wp_info.petal1_id)
        {
            Copy_Petal_Frame_Buffer(
                    &s_petal_frame_buffer, 
                    &s_petal1_buffer,
                    s_petal_array[i].cur_frame,
                    s_petal_array[i].width,
                    s_petal_array[i].height);
        }
        else
        {
            Copy_Petal_Frame_Buffer(
                    &s_petal_frame_buffer, 
                    &s_petal2_buffer,
                    s_petal_array[i].cur_frame,
                    s_petal_array[i].width,
                    s_petal_array[i].height);
        }
        
        pop_up_window.w = s_petal_array[i].width;
        pop_up_window.h = s_petal_array[i].height;
        rect.left = s_petal_array[i].left;
        rect.top = s_petal_array[i].top;
        offset.x=(int16)rect.left;
        offset.y=(int16)rect.top;
        offset.x_dec=0;//(uint16)((rect.left<<16)&0xffff);
        offset.y_dec=0;//(uint16)((rect.top<<16)&0xffff);
    
        if (PNULL == s_petal_frame_buffer.data_ptr)
        {
            return;
        }
        S2D_Blending(
                pop_bg_window , 
                lw_cal_param->dest_buf_info.buf_ptr, 
                pop_up_window , 
                offset, 
                (uint32*)s_petal_frame_buffer.data_ptr, 
                lw_cal_param->dest_buf_info.buf_ptr);
        
        
        Check_Petal_Moving_Status(&s_petal_array[i]);
    }

    Add_Moving_Petal();

}

/*****************************************************************************/
//  Description : 释放
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:销毁临时申请的变量
/*****************************************************************************/
LOCAL void SakuraDestruct(void)
{
    if (s_petal1_buffer.data_ptr != PNULL)
    {
        MMILIVEWALLPAPER_FREE(s_petal1_buffer.data_ptr);//SCI_FREE
        s_petal1_buffer.data_ptr = PNULL;
    }

    if (s_petal2_buffer.data_ptr != PNULL)
    {
        MMILIVEWALLPAPER_FREE(s_petal2_buffer.data_ptr);
        s_petal2_buffer.data_ptr = PNULL;
    }

    if (s_petal_frame_buffer.data_ptr != PNULL)
    {
        MMILIVEWALLPAPER_FREE(s_petal_frame_buffer.data_ptr);
        s_petal_frame_buffer.data_ptr = PNULL;
    }

    if (s_sakura_buffer.data_ptr != PNULL)
    {
        MMILIVEWALLPAPER_FREE(s_sakura_buffer.data_ptr);
        s_sakura_buffer.data_ptr = PNULL;
    }

    if (s_sakura_rotate_buffer.data_ptr != PNULL)
    {
        MMILIVEWALLPAPER_FREE(s_sakura_rotate_buffer.data_ptr);
        s_sakura_rotate_buffer.data_ptr = PNULL;
    }

    if (s_moon_buffer.data_ptr != PNULL)
    {
        MMILIVEWALLPAPER_FREE(s_moon_buffer.data_ptr);
        s_moon_buffer.data_ptr = PNULL;
    }

    if (s_pollen_buffer.data_ptr != PNULL)
    {
        MMILIVEWALLPAPER_FREE(s_pollen_buffer.data_ptr);
        s_pollen_buffer.data_ptr = PNULL;
    }

    if(PNULL != s_pollen_array)
    {
        MMILIVEWALLPAPER_FREE(s_pollen_array);
        s_pollen_array = PNULL;
    }

    if(PNULL != s_petal_array)
    {
        MMILIVEWALLPAPER_FREE(s_petal_array);
        s_petal_array = PNULL;
    }

    if(PNULL != s_sakura_array)
    {
        MMILIVEWALLPAPER_FREE(s_sakura_array);
        s_sakura_array = PNULL;
    }
}

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
LOCAL void Sakura_PointProc(uint16 x, uint16 y)
{
    uint16 i;

    s_touch_point.x = x;
    s_touch_point.y = y;

    if(PNULL == s_sakura_array)
    {
        return;
    }            
    for (i = 0; i < SAKURA_TOTAL; ++i)
    {
        Set_Sakura_Speed_After_Touch(&s_sakura_array[i]);
    }
    
    if(PNULL == s_petal_array)
    {
        return;
    }
    for (i = 0; i < PETAL_TOTAL; ++i)
    {
        Set_Petal_Speed_After_Touch(&s_petal_array[i]);
    }

    if (s_global_wp_info.need_pollen)
    {
        if(PNULL == s_pollen_array)
        {
            return;
        }
        for (i = 0; i < POLLEN_TOTAL; ++i)
        {
            Set_Pollen_Speed_After_Touch(&s_pollen_array[i]);
        }
    }


    if (s_global_wp_info.need_vibrate)
    {   
        MMISRVAUD_StartVibrate(MMISRVAUD_PRI_NORMAL, 50, 0, 1, PNULL);
    }
}
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

/*****************************************************************************/
//  Description : 交互事件处理
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SakuraHandleEvent(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    GUI_POINT_T   point       = {0};
    
    
    switch(msg_id)
    {
    case MSG_TP_PRESS_DOWN:
        point.x = MMK_GET_TP_X(param);
        point.y = MMK_GET_TP_Y(param);
        Sakura_PointProc(point.x,point.y);
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
LOCAL uint32 SakuraGetType(void)
{
    return MMI_LIVE_WALLPAPER_TYPE_SAKURA;
}
/*****************************************************************************/
//  Description : 获得动态壁纸ThumbNail信息
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL void SakuraGetThumbNailInfo(LW_THUMBNAIL_INFO *thumbnail_info_ptr)
{
    if(thumbnail_info_ptr)
    {
        thumbnail_info_ptr->is_image_file = FALSE;
        thumbnail_info_ptr->image_id = IMAGE_WALLPAPER_SAKURA_THUMBNAIL;

        thumbnail_info_ptr->is_not_string = FALSE;
        thumbnail_info_ptr->text_id = TXT_SET_WALLPAPER_SAKURA;
        thumbnail_info_ptr->instance_mem_pool_size = BLOCK_MEM_POOL_SIZE_SAKURA;
    }
}
/*****************************************************************************/
//  Description : 获得动态壁纸选项列表
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL OPTIONLIST *SakuraGetOptionList(void)
{
    return (OPTIONLIST*)&s_sakura_option_list;
}  
#endif //_MMI_LIVEWALLPAPER_SAKURA_C_



/*Edit by script, ignore 1 case. Thu Apr 26 19:00:58 2012*/ //IGNORE9527
