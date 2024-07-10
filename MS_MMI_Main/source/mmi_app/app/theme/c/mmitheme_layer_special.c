/*****************************************************************************
** File Name:      mmitheme_layer_special.c                                  *
** Author:         hua.fang                                                  *
** Date:           07/07/2011                                                *
** Copyright:      2011 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:                                                              *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 07/2011        hua.fang          Create                                   *
******************************************************************************/
#include "mmi_app_theme_trc.h"
#ifdef EFFECT_ZOOM_SUPPORT
/**--------------------------------------------------------------------------*/
/**                         Include Files                                    */
/**--------------------------------------------------------------------------*/
#include "std_header.h"
#include "mmidisplay_data.h"
#include "ui_special_effect.h"
#include "mmi_default.h"
#include "guilcd.h"
#include "mmiset_display.h"
#include "dal_lcd.h"
#include "lcm_drv.h"
#include "block_mem.h"
#include "mmi_theme.h"
#include "mmi3d_export.h"
#include "graphics_draw.h"
#include "special_effect.h" 
#include "mmk_tp.h"
#include "mmitheme_layer_special.h"
#include "graphics_3d_rotate.h"
#include "mmi_utility.h"
#if 0//使用新的缩放框架

/**--------------------------------------------------------------------------*/
/**                         MACRO DEFINITION                                 */
/**--------------------------------------------------------------------------*/
#define MMITHEME_ZOOM_FRAME_NUM         6           // 缩放的帧数

/*----------------------------------------------------------------------------*/
/*                          TYPE AND STRUCT                                   */
/*----------------------------------------------------------------------------*/
typedef struct
{
    uint32      zoom_a;         // 内容层缩放比例
    uint32      zoom_b;
    uint32      alpha_a;        // 蒙板层透明度比例
    uint32      alpha_b;
    uint8       layer_alpha;    // 内容层透明度
} MMITHEME_ZOOM_PARAM_T;

/*---------------------------------------------------------------------------*/
/*                          CONSTANT ARRAY                                   */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*/
/**                         EXTERNAL DECLARE                                 */
/**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*/
/**                         STATIC DEFINITION                                */
/**--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : is layer valid
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsLayerValid(
                           GUI_LCD_DEV_INFO   *lcd_dev_ptr
                           );

/*****************************************************************************/
//  Description : create layer
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CreateSpecialLayer(
                                 int16                  width,              // in
                                 int16                  height,             // in
                                 UILAYER_MEM_FORMAT_T   mem_format,         // in
                                 GUI_LCD_DEV_INFO       *lcd_dev_info_ptr   // out
                                 );

/*****************************************************************************/
//  Description : release layer
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void ReleaseSpecialLayer(
                               GUI_LCD_DEV_INFO    *lcd_dev_info_ptr
                               );

/*****************************************************************************/
//  Description : append layer
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void AppendSpecialLayer(
                              GUI_LCD_DEV_INFO    *lcd_dev_info_ptr
                              );

/**--------------------------------------------------------------------------*/
/**                         GLOBAL DEFINITION                                */
/**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
 /*****************************************************************************/
//  Description : play zoom anim
//  Global resource dependence :
//  Author: hua.fang
//  Note:
// 传入蒙板层和内容层，原则假设这两层没有添加到层系统里，纯粹作为数据层来处理，所
// 以，我将它们添加或移除层系统，对用户都不会有影响，而最终的结果，也是将这两层移
// 除，如果用户要显示层的话，必须调完此接口后，主动将层添加进层系统中。
// 内容层的数据不会被破坏，但蒙层的数据，由于要做渐变效果，有可能会受到破坏，如果
// 用户需要原始数据不被破坏，需做下备份。
// 注意点:
//  1. 源BUFFER数据需要居中显示，否则动画有问题
/*****************************************************************************/
PUBLIC void MMITHEME_PlayZoomAnim(
                                  MMITHEME_ZOOM_DATA_T      *zoom_data_ptr,     // 动画数据
                                  BOOLEAN                   is_zoom_out         // 是否缩小
                                  )
{
#ifdef EFFECT_ZOOM_SUPPORT
    BOOLEAN             result = FALSE;
    uint16              lcd_width = 0;
    uint16              lcd_height = 0;
    uint32              i = 0;
    uint32              frame = 0;                              // 当前帧
    int32               index = 0;                              // 在zoom_param的偏移
    GUI_RECT_T          rect = {0};                             // 层的区域
    GUI_LCD_DEV_INFO    zoom_lcd_dev = {0};                     // 缩放之后的层
    UILAYER_INFO_T      zoom_layer_info = {0};                  // 缩放层的信息
    UILAYER_INFO_T      content_layer_info = {0};               // menu层的信息
    MMITHEME_ZOOM_PARAM_T zoom_param[MMITHEME_ZOOM_FRAME_NUM] =  // zoom动画参数
        {
            {9, 10, 1, 10, 120},
            {95, 100, 3, 10, 200},
            {1, 1, 5, 10, 255},
            {1, 1, 6, 10, 255},
            {1, 1, 8, 10, 255},
            {1, 1, 10, 10, 255},
        };

    if (PNULL == zoom_data_ptr || !IsLayerValid(&zoom_data_ptr->content_lcd_dev))
    {
        return;
    }

    // 拿内容层的信息
    UILAYER_GetLayerInfo(&zoom_data_ptr->content_lcd_dev, &content_layer_info);

    //get lcd width and height
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &lcd_width, &lcd_height);

    // 所给的buffer宽高大于屏幕宽高，则不能进行缩放动画
    if (content_layer_info.layer_width > lcd_width
        || content_layer_info.layer_height > lcd_height)
    {
        return;
    }

    // 创建一个缩放层，用于合成
    result = CreateSpecialLayer(
            lcd_width, lcd_height,
            UILAYER_MEM_DOUBLE_COPY,
            &zoom_lcd_dev);
    if (!result)
    {
        return;
    }

    // 先摘掉所有层
    //UILAYER_RemoveBltLayer(&zoom_data_ptr->mask_lcd_dev);
    UILAYER_RemoveBltLayer(&zoom_data_ptr->content_lcd_dev);

    // 添加所需的层，底下蒙层，上面缩放层
    AppendSpecialLayer(&zoom_data_ptr->mask_lcd_dev);
    AppendSpecialLayer(&zoom_lcd_dev);

    // 拿缩放层的信息
    UILAYER_GetLayerInfo(&zoom_lcd_dev, &zoom_layer_info);
    rect.right = zoom_layer_info.layer_width - 1;
    rect.bottom = zoom_layer_info.layer_height - 1;

#ifndef WIN32
    CHNG_FREQ_SetArmClk(MMI_GetFreqHandler(), FREQ_INDEX_MP3);
#endif

    //anim
    if (is_zoom_out)
    {
        // ZOOM IN，从第4帧开始
        frame = 3;
    }
    else
    {
        frame = 0;
    }

    for (; frame < MMITHEME_ZOOM_FRAME_NUM; frame++)
    {
        SCALE_IMAGE_IN_T    scale_in  = {0};
        SCALE_IMAGE_OUT_T   scale_out = {0};
        SCALE_IMAGE_CAP_T   scale_cap = {0};

        if (is_zoom_out)
        {
            index = MMITHEME_ZOOM_FRAME_NUM - frame - 1;
        }
        else
        {
            index = frame;
        }

        // 进行缩放
        if (zoom_param[index].zoom_a != zoom_param[index].zoom_b)
        {
            GRAPH_GetScaleCapability(&scale_cap);        
            if (!scale_cap.is_idle)
            {
                continue;
            }

            scale_in.src_format           = IMGREF_FORMAT_ARGB888;
            scale_in.src_chn.chn0.ptr     = UILAYER_GetLayerBufferPtr(&zoom_data_ptr->content_lcd_dev);            
//            scale_in.src_chn.chn0.ptr     = content_layer_info.layer_buf_ptr;
            scale_in.src_chn.chn0.size    = content_layer_info.mem_width * content_layer_info.layer_height * 4;
            scale_in.src_size.w           = content_layer_info.mem_width;
            scale_in.src_size.h           = content_layer_info.layer_height;
            scale_in.src_trim_rect.x      = 0;
            scale_in.src_trim_rect.y      = 0;
            scale_in.src_trim_rect.w      = content_layer_info.mem_width;
            scale_in.src_trim_rect.h      = content_layer_info.layer_height;

            scale_in.target_format        = IMGREF_FORMAT_ARGB888;
            scale_in.target_size.w        = scale_in.src_trim_rect.w * zoom_param[index].zoom_a/zoom_param[index].zoom_b;
            scale_in.target_size.h        = scale_in.src_trim_rect.h * zoom_param[index].zoom_a/zoom_param[index].zoom_b;
            scale_in.target_buf.size      = scale_in.target_size.w * scale_in.target_size.h * 4 + 255 + 31;
            scale_in.target_buf.ptr       = SCI_ALLOCA(scale_in.target_buf.size);

            if (PNULL == scale_in.target_buf.ptr)
            {
                break;
            }

            //// 不清0，这里也花时间，缩放会放数据的
            //SCI_MEMSET(scale_in.target_buf.ptr, 0, scale_in.target_buf.size);

            if (SCI_SUCCESS == GRAPH_ScaleImage(&scale_in,&scale_out))
            {
                UILAYER_COPYBUF_PARAM_T     copy_param = {0};
                UILAYER_COPYBUF_T           copy_buf = {0};

                SCI_ASSERT(zoom_layer_info.layer_width >= scale_out.output_size.w);     /*assert verified*/
                SCI_ASSERT(zoom_layer_info.layer_height >= scale_out.output_size.h);    /*assert verified*/

                copy_param.rect.left = (zoom_layer_info.layer_width - scale_out.output_size.w)/2;
                copy_param.rect.top = (zoom_layer_info.layer_height - scale_out.output_size.h)/2;

                copy_param.rect.right = copy_param.rect.left + scale_out.output_size.w - 1;
                copy_param.rect.bottom = copy_param.rect.top + scale_out.output_size.h - 1;

                copy_buf.buf_ptr = (uint8*)scale_out.output_chn.chn0.ptr;
                copy_buf.data_type = DATA_TYPE_ARGB888;
                copy_buf.width = scale_out.output_size.w;
                copy_buf.height = scale_out.output_size.h;

                UILAYER_CopyBufferToLayer(&zoom_lcd_dev, &copy_param, &copy_buf);

                // 让层变透明
                if (255 != zoom_param[index].layer_alpha)
                {
                    UILAYER_WeakLayerAlpha(&zoom_lcd_dev, zoom_param[index].layer_alpha);
                }
            }
            else
            {
                UILAYER_BltLayerToLayer(&zoom_lcd_dev, &zoom_data_ptr->content_lcd_dev, &rect, FALSE);
            }
        }
        else
        {
            // BUG: 如果源内容数据不居中显示，则这里拷贝时会有问题，因为缩放是按
            // 居中来做的，这里不居中，会有跳动感
            // 所以，给数据时，内容需居中
            UILAYER_BltLayerToLayer(&zoom_lcd_dev, &zoom_data_ptr->content_lcd_dev, &rect, FALSE);
        }

        // 画蒙板
        if (0 != zoom_param[index].alpha_a)
        {
            LCD_FillArgbRect(
                (const GUI_LCD_DEV_INFO*)&zoom_data_ptr->mask_lcd_dev,
                rect,
                zoom_data_ptr->mask_color,
                zoom_data_ptr->mask_alpha * zoom_param[index].alpha_a/zoom_param[index].alpha_b);
        }

        GUILCD_Invalidate( GUI_MAIN_LCD_ID, 0 );

        if (PNULL != scale_in.target_buf.ptr)
        {
            SCI_FREE(scale_in.target_buf.ptr);
        }
    }

#ifndef WIN32
    CHNG_FREQ_RestoreARMClk(MMI_GetFreqHandler());
#endif

    // 释放缩放层
    ReleaseSpecialLayer(&zoom_lcd_dev);

    // 将MASK层remove掉
    UILAYER_RemoveBltLayer(&zoom_data_ptr->mask_lcd_dev);
#endif
}

/*****************************************************************************/
//  Description : is layer valid
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsLayerValid(
                           GUI_LCD_DEV_INFO   *lcd_dev_ptr
                           )
{
    if (PNULL != lcd_dev_ptr
        && 0 != lcd_dev_ptr->block_id
        && UILAYER_NULL_HANDLE != lcd_dev_ptr->block_id)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*****************************************************************************/
//  Description : create layer
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CreateSpecialLayer(
                                 int16                  width,              // in
                                 int16                  height,             // in
                                 UILAYER_MEM_FORMAT_T   mem_format,         // in
                                 GUI_LCD_DEV_INFO       *lcd_dev_info_ptr   // out
                                 )
{
    BOOLEAN             result = FALSE;
    UILAYER_RESULT_E    layer_result = UILAYER_RESULT_SUCCESS;
    UILAYER_CREATE_T    create_info = {0};

    if (PNULL != lcd_dev_info_ptr
        && (0 == lcd_dev_info_ptr->block_id || UILAYER_NULL_HANDLE == lcd_dev_info_ptr->block_id)
        && UILAYER_IsMultiLayerEnable())
    {
        create_info.lcd_id = GUI_MAIN_LCD_ID;
        create_info.owner_handle = 0;
        create_info.offset_x = 0;
        create_info.offset_y = 0;
        create_info.width = width;;
        create_info.height = height;
        create_info.is_bg_layer = FALSE;
        create_info.is_static_layer = TRUE;
        create_info.format = mem_format;

        layer_result = UILAYER_CreateLayer(&create_info, lcd_dev_info_ptr);

        if (UILAYER_RESULT_SUCCESS == layer_result)
        {
            UILAYER_SetLayerColorKey(lcd_dev_info_ptr, TRUE, UILAYER_TRANSPARENT_COLOR);
            UILAYER_Clear(lcd_dev_info_ptr);

            result = TRUE;
        }
    }

    return result;
}

/*****************************************************************************/
//  Description : release layer
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void ReleaseSpecialLayer(
                               GUI_LCD_DEV_INFO    *lcd_dev_info_ptr
                               )
{
    if (PNULL != lcd_dev_info_ptr
        && 0 != lcd_dev_info_ptr->block_id
        && UILAYER_NULL_HANDLE != lcd_dev_info_ptr->block_id)
    {
        UILAYER_RELEASELAYER(lcd_dev_info_ptr);   /*lint !e506 !e774*/
//        UILAYER_ReleaseLayer(lcd_dev_info_ptr);
//
//        lcd_dev_info_ptr->lcd_id = GUI_MAIN_LCD_ID;
//        lcd_dev_info_ptr->block_id = UILAYER_NULL_HANDLE;
    }
}

/*****************************************************************************/
//  Description : append layer
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void AppendSpecialLayer(
                              GUI_LCD_DEV_INFO    *lcd_dev_info_ptr
                              )
{
    UILAYER_APPEND_BLT_T append_layer = {0};

    if (PNULL != lcd_dev_info_ptr
        && 0 != lcd_dev_info_ptr->block_id
        && UILAYER_NULL_HANDLE != lcd_dev_info_ptr->block_id)
    {
        append_layer.lcd_dev_info = *lcd_dev_info_ptr;
        append_layer.layer_level = UILAYER_LEVEL_NORMAL;

        UILAYER_AppendBltLayer(&append_layer);
    }
}

#else

//#define CALTIMEZOOM //打时间的宏

#define MMITHEME_ZOOM_DEFAULT_DST_SIZE 100

//算法实现的入参
typedef struct
{
    GUI_RECT_T rect;
    uint8*     src_ptr;
    uint32     src_width;
    uint32     src_height;
    uint8*     dst_ptr;
    uint32     dst_width;
    uint32     dst_height;
    uint8*     ext_ptr;
    uint32     ext_size;
    uint32     cur_frame;
    uint32     total_frame;
    uint32     color;
    BOOLEAN    is_zoom_out;
    BOOLEAN    is_clip_rect;
} ZOOMSE_IN_PARAM_T;

//算法实现的出
typedef struct
{
    uint32     color;
} ZOOMSE_OUT_PARAM_T;

typedef struct  
{
    uint32 ext_size;       //算法需要的格外数据
    uint32 zoom_in_frame;  //放大的总帧数
    uint32 zoom_out_frame; //缩小的总帧数
	uint32 sleep_ticks;    //等待ticks
    uint32 dst_size;       //需要dst buffer的百分比，100即为原始buffer
    void (*ZoomseFunc)( ZOOMSE_IN_PARAM_T* in_ptr, ZOOMSE_OUT_PARAM_T* out_ptr );//算法的回调
}ZOOMSE_IMPL_T;

LOCAL void ZoomseImpl1( ZOOMSE_IN_PARAM_T* in_ptr, ZOOMSE_OUT_PARAM_T* out_ptr );

//算法实现1
LOCAL const ZOOMSE_IMPL_T s_zoomse_impl1 = 
{
    2048*sizeof(FIX16_POINT_T), //算法需要的格外数据
    8,                          //放大的总帧数
    8,                          //缩小的总帧数
	0,                          //等待ticks,
    MMITHEME_ZOOM_DEFAULT_DST_SIZE,
    ZoomseImpl1    
};

LOCAL void ZoomseImpl2( ZOOMSE_IN_PARAM_T* in_ptr, ZOOMSE_OUT_PARAM_T* out_ptr );

//算法实现1
LOCAL const ZOOMSE_IMPL_T s_zoomse_impl2 = 
{
    2048*sizeof(FIX16_POINT_T), //算法需要的格外数据
    15,                         //放大的总帧数
    8,                          //缩小的总帧数
	0,                          //等待ticks
    MMITHEME_ZOOM_DEFAULT_DST_SIZE+20,
    ZoomseImpl2    
};

LOCAL const ZOOMSE_IMPL_T* const s_zoomse_impl_array[] = 
{
    &s_zoomse_impl1,
    &s_zoomse_impl2,
};

/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author: James.Zhang
//  Note:算法实现
/*****************************************************************************/
LOCAL void ZoomseImpl1(
                       ZOOMSE_IN_PARAM_T    *zoom_in_data_ptr,     // 动画数据
                       ZOOMSE_OUT_PARAM_T   *zoom_out_data_ptr//
                       )
{
	uint32 pre_z_para=0,pre_alpha=0,step_frame=0,SinData=0;
	uint32 min_z_para=1500,bg_min_alpha=0,min_alpha=35;
	uint32 cur_frame=0,total_frame=0;
	int16 zero_size=0;
	IMGREF_POINT_T window_mid={0},img_mid={0};
	IMGREF_SIZE_T window_size={0};
	IMGREF_SIZE_T image_size={0};
	FIX16_POINT_T * Point_Add_ptr = PNULL;
	uint32 * src_buff = PNULL;
#ifdef CALTIMEZOOM
	static uint32 cal_time=0,whole_time1=0,whole_time2=0;
	whole_time1=SCI_GetTickCount();
	SCI_TRACE_LOW("ZOOMONE_total, cost_time ONE frame = %d ms", whole_time1-whole_time2);
	whole_time2=SCI_GetTickCount();
	cal_time=SCI_GetTickCount();
#endif	    
    if ( PNULL == zoom_in_data_ptr
        || PNULL == zoom_out_data_ptr
        || PNULL == zoom_in_data_ptr->ext_ptr )
    {
        return;
    }
	bg_min_alpha=255-((zoom_in_data_ptr->color)>>24);
	cur_frame=zoom_in_data_ptr->cur_frame;
	total_frame=zoom_in_data_ptr->total_frame;
	window_size.w = zoom_in_data_ptr->dst_width;
	window_size.h = zoom_in_data_ptr->dst_height;
    if(!zoom_in_data_ptr->is_clip_rect)
    {
        image_size.w = zoom_in_data_ptr->src_width;
        image_size.h = zoom_in_data_ptr->src_height-2*MMITHEME_ZOOM_MEM_ALIGN;
        src_buff=((uint32 *)(zoom_in_data_ptr->src_ptr))+MMITHEME_ZOOM_MEM_ALIGN*zoom_in_data_ptr->src_width;
        window_mid.y=window_size.h/2;
    }
    else
    {	
#if 0
        if(zoom_in_data_ptr->rect.left <=zoom_in_data_ptr->rect.top)
        {
            zero_size=zoom_in_data_ptr->rect.left;
            src_buff=((uint32 *)(zoom_in_data_ptr->src_ptr));
            src_buff=src_buff+(zoom_in_data_ptr->rect.top-zero_size)*zoom_in_data_ptr->src_width;/*lint !e737*/
            image_size.w = zoom_in_data_ptr->src_width;
            image_size.h = (zoom_in_data_ptr->rect.bottom-zoom_in_data_ptr->rect.top)+2*zero_size;
            window_mid.y=zoom_in_data_ptr->rect.top+(zoom_in_data_ptr->rect.bottom-zoom_in_data_ptr->rect.top)/2;
        }
        else
        {
            return;
        }
#endif       
        zero_size=zoom_in_data_ptr->rect.left;
        src_buff=((uint32 *)(zoom_in_data_ptr->src_ptr));
        src_buff=src_buff+(zoom_in_data_ptr->rect.top-zero_size)*zoom_in_data_ptr->src_width;/*lint !e737*/
        image_size.w = zoom_in_data_ptr->src_width;
        image_size.h = (zoom_in_data_ptr->rect.bottom-zoom_in_data_ptr->rect.top)+2*zero_size;
        window_mid.y=zoom_in_data_ptr->rect.top+(zoom_in_data_ptr->rect.bottom-zoom_in_data_ptr->rect.top)/2;
    }
	Point_Add_ptr = (FIX16_POINT_T*)zoom_in_data_ptr->ext_ptr;

	zoom_out_data_ptr->color=255;

	window_mid.x=window_size.w/2+1;
	
	img_mid.x=image_size.w/2;
	img_mid.y=image_size.h/2;

    /*
    window_mid.x=zoom_in_data_ptr->rect.left;
    window_mid.y=zoom_in_data_ptr->rect.top;
	*/
    
    step_frame=total_frame/2;
        
    
    if(0==total_frame)
    {
        return;
    }
    if(0==step_frame)
    {
        return;
    }
    
    if(!zoom_in_data_ptr->is_zoom_out)
    {
        SinData=(int)s2d_sin_new(-900+(int)(1800*(cur_frame)/total_frame))+65536;
        zoom_out_data_ptr->color=(((255-bg_min_alpha)*SinData/(65536*2))<<24);
        
        SinData=(int)s2d_sin_new(-900+(int)(1800*cur_frame/total_frame))+65536;
        pre_alpha=min_alpha+(255-min_alpha)*(SinData)/(65536*2);
        pre_z_para=min_z_para+(4096-min_z_para)*SinData/(65536*2);
		/*
		for(i=0;i<window_size.h;i++)
			{
				for(j=0;j<window_size.w;j++)
					{
						((uint32 *)zoom_in_data_ptr->dst_ptr)[i*window_size.w+j]=((uint32 *)zoom_in_data_ptr->src_ptr)[i*window_size.w+j];
					}
			}
			*/
        S2d_4_in_one_fast_Out32_Zero_size(window_size, image_size,  window_mid,  img_mid, 0, src_buff,(uint32 *)zoom_in_data_ptr->dst_ptr,zero_size,pre_z_para,pre_alpha,Point_Add_ptr);
	}
    else
    {	
        if(cur_frame<=step_frame)
        {
            zoom_out_data_ptr->color=((255-bg_min_alpha-(255-bg_min_alpha)*cur_frame/step_frame)<<24);
        }
        
        SinData=s2d_sin_new(-900+(int)(1800*(total_frame-cur_frame)/total_frame))+65536;
        pre_alpha=255*(SinData)/(65536*2);
        pre_z_para=min_z_para+(4096-min_z_para)*SinData/(65536*2);
       S2d_4_in_one_fast_Out32_Zero_size(window_size, image_size,  window_mid,  img_mid, 0, src_buff,(uint32 *)zoom_in_data_ptr->dst_ptr,zero_size,pre_z_para,pre_alpha,Point_Add_ptr);
    }

#ifdef CALTIMEZOOM
    cal_time=SCI_GetTickCount()-cal_time;
    SCI_TRACE_LOW("ZOOMONE_FRAME, cost_time ONE frame = %d ms",cal_time);
#endif	
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author: James.Zhang
//  Note:算法实现
/*****************************************************************************/
#define MMITHEME_PI 3.141592653

LOCAL void ZoomseImpl2(
                       ZOOMSE_IN_PARAM_T    *zoom_in_data_ptr,     // 动画数据
                       ZOOMSE_OUT_PARAM_T   *zoom_out_data_ptr
                       )
{
	uint32 pre_z_para=0,pre_alpha=0,step_frame=0,SinData=0;
	uint32 min_z_para=3500,bg_min_alpha=0;
	uint32 cur_frame=0;
	uint32 total_frame=0;
	int16 zero_size=0;
	int amplitude=200,/*btm_zpara=1700,*/up_distance=4096,/*top_z_para=700,*/CosData=0,cur_angle=0,max_angle=4500;
	IMGREF_POINT_T window_mid={0},img_mid={0};
	IMGREF_SIZE_T window_size={0};
	IMGREF_SIZE_T image_size={0};
	FIX16_POINT_T * Point_Add_ptr = PNULL;
	uint32 * src_buff = PNULL;
#ifdef CALTIMEZOOM
    static uint32 cal_time=0,whole_time1=0,whole_time2=0;
    whole_time1=SCI_GetTickCount();
    SCI_TRACE_LOW("ZOOMTWO_TOTAL, cost_time ONE frame = %d ms", whole_time1-whole_time2);
    whole_time2=SCI_GetTickCount();
    cal_time=SCI_GetTickCount();
#endif	
    if ( PNULL == zoom_in_data_ptr
        || PNULL == zoom_out_data_ptr
        || PNULL == zoom_in_data_ptr->ext_ptr )
    {
        return;
    }
	bg_min_alpha=255-((zoom_in_data_ptr->color)>>24);
	cur_frame=zoom_in_data_ptr->cur_frame;
	total_frame=zoom_in_data_ptr->total_frame;
	window_size.w = zoom_in_data_ptr->dst_width;
	window_size.h = zoom_in_data_ptr->dst_height;
	Point_Add_ptr=(FIX16_POINT_T*)zoom_in_data_ptr->ext_ptr;
	zoom_out_data_ptr->color=255;
    if(!zoom_in_data_ptr->is_clip_rect)
    {
        image_size.w = zoom_in_data_ptr->src_width;
        image_size.h = zoom_in_data_ptr->src_height-2*MMITHEME_ZOOM_MEM_ALIGN;
        src_buff=((uint32 *)(zoom_in_data_ptr->src_ptr))+MMITHEME_ZOOM_MEM_ALIGN*zoom_in_data_ptr->src_width;
        window_mid.y=window_size.h/2;
    }
    else
    {	
        if(zoom_in_data_ptr->rect.left <=zoom_in_data_ptr->rect.top)
        {
            zero_size=zoom_in_data_ptr->rect.left;
            src_buff=((uint32 *)(zoom_in_data_ptr->src_ptr));
            src_buff=src_buff+(zoom_in_data_ptr->rect.top-zero_size)*zoom_in_data_ptr->src_width;/*lint !e737*/
            image_size.w = zoom_in_data_ptr->src_width;
            image_size.h = (zoom_in_data_ptr->rect.bottom-zoom_in_data_ptr->rect.top)+2*zero_size;
            window_mid.y=zoom_in_data_ptr->rect.top+(zoom_in_data_ptr->rect.bottom-zoom_in_data_ptr->rect.top)/2;
        }
        else
        {
            return;
        }
        
    }
	window_mid.x=window_size.w/2+1;
	
	img_mid.x=image_size.w/2;
	img_mid.y=image_size.h/2;
	step_frame=total_frame/2;
    
    
    if(0==total_frame)
    {
        return;
    }
    if(0==step_frame)
    {
        return;
    }
    
    if(!zoom_in_data_ptr->is_zoom_out)
    {
        SinData=(int)s2d_sin_new(-900+(int)(1800*(cur_frame)/total_frame))+65536;
        zoom_out_data_ptr->color=(((255-bg_min_alpha)*SinData/(65536*2))<<24);
        
        SinData=s2d_sin_new(-900+(int)(1800*cur_frame/total_frame))+65536;
        pre_alpha=255*(SinData)/(65536*2);
        cur_angle=max_angle*cur_frame/total_frame;/*lint !e737*/
        CosData=s2d_cos_new(cur_angle);
        //pre_z_para=-((((-btm_zpara+top_z_para)*cur_angle/1800+btm_zpara)*CosData)>>16)+up_distance;
        pre_z_para=-amplitude*(max_angle-cur_angle)*MMITHEME_PI*s2d_sin_new(max_angle-cur_angle)/(65536*1800)+up_distance;/*lint !e790 !e524*/
        S2d_4_in_one_fast_Out32_Zero_size(window_size, image_size,  window_mid,  img_mid, 0, src_buff,(uint32 *)zoom_in_data_ptr->dst_ptr,0,pre_z_para,pre_alpha,Point_Add_ptr);
    }
    else
    {	
        if(cur_frame<=step_frame)
        {
            zoom_out_data_ptr->color=((255-bg_min_alpha-(255-bg_min_alpha)*cur_frame/step_frame)<<24);
        }
        
        SinData=s2d_sin_new(-900+(int)(1800*(total_frame-cur_frame)/total_frame))+65536;
        pre_alpha=255*(SinData)/(65536*2);
        pre_z_para=min_z_para+(4096-min_z_para)*SinData/(65536*2);
        S2d_4_in_one_fast_Out32_Zero_size(window_size, image_size,  window_mid,  img_mid, 0,  src_buff,(uint32 *)zoom_in_data_ptr->dst_ptr,0,pre_z_para,pre_alpha,Point_Add_ptr);
    }
#ifdef CALTIMEZOOM
    cal_time=SCI_GetTickCount()-cal_time;
    SCI_TRACE_LOW("ZOOMTWO_FRAME, cost_time ONE frame = %d ms",cal_time);
#endif
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author: James.Zhang
//  Note:获得当前特效
/*****************************************************************************/
LOCAL const ZOOMSE_IMPL_T* GetZoomseImpl( uint32 type )
{
    uint32 max = ARR_SIZE(s_zoomse_impl_array);

    if ( type >= max )
    {
        type = 0;
    }

    return s_zoomse_impl_array[type];
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author: James.Zhang
//  Note:blend的回调，用DMA方式，提高性能
/*****************************************************************************/
// LOCAL void ZoomseBlendCallBack(
//                                uint8* buffer, 
//                                UILAYER_INFO_T *layer_info_ptr,
//                                uint32 param
//                                )
// {
//     if ( PNULL != buffer
//         && PNULL != layer_info_ptr )
//     {
// #ifdef WIN32
//         MMI_MEM32SET( (uint32*)buffer, 0, layer_info_ptr->layer_height * layer_info_ptr->layer_width * sizeof(uint32) );
// #else
//         //use dma here
//         MMI_MEM32SET( (uint32*)buffer, 0, layer_info_ptr->layer_height * layer_info_ptr->layer_width * sizeof(uint32) );
// #endif
//     }
// }

/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author: James.Zhang
//  Note:blend的回调，用DMA方式，提高性能
/*****************************************************************************/
// LOCAL void MaskBlendCallBack(
//                              uint8* buffer, 
//                              UILAYER_INFO_T *layer_info_ptr,
//                              uint32 param
//                              )
// {
//     if ( PNULL != buffer
//         && PNULL != layer_info_ptr )
//     {
// #ifdef WIN32
//         MMI_MEM32SET( (uint32*)buffer, param, layer_info_ptr->layer_height * layer_info_ptr->layer_width * sizeof(uint32) );
// #else
//         //use dma here
//         MMI_MEM32SET( (uint32*)buffer, param, layer_info_ptr->layer_height * layer_info_ptr->layer_width * sizeof(uint32) );
// #endif
//     }
// }

//#define DST_LAYER_HEIGHT( height ) ( height *120 /100 )

/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author: James.Zhang
//  Note:播放缩放特效
/*****************************************************************************/
PUBLIC void MMITHEME_PlayZoomAnim(
                                  MMITHEME_ZOOM_DATA_T      *zoom_data_ptr,     // 动画数据
                                  BOOLEAN                   is_zoom_out         // 是否缩小
                                  )
{
    const ZOOMSE_IMPL_T* se_ptr     = PNULL;
    ZOOMSE_IN_PARAM_T    in_param   = {0};
    ZOOMSE_OUT_PARAM_T   out_param  = {0};
    UILAYER_INFO_T       content_layer_info = {0};
    GUI_LCD_DEV_INFO     zoomse_lcd_dev = {0};
    GUI_LCD_DEV_INFO     content_lcd_dev = {0};
    UILAYER_CREATE_T     create_info = {0};
    uint16               lcd_width   = 0;
    uint16               lcd_height  = 0;
    GUI_LCD_DEV_INFO     mask_lcd_dev = {GUI_MAIN_LCD_ID, UILAYER_NULL_HANDLE};

    if ( PNULL == zoom_data_ptr )
    {
        return;
    }

    se_ptr = GetZoomseImpl( MMITHEME_IsIstyle() ? 1 : 0 );

    if ( PNULL == se_ptr
        || PNULL == se_ptr->ZoomseFunc )
    {
        return;
    }

    //先申请图层，避免大内存分配后分不到
    if (UILAYER_HANDLE_MULTI == UILAYER_GetHandleType(&zoom_data_ptr->mask_lcd_dev))
    {
        mask_lcd_dev = zoom_data_ptr->mask_lcd_dev;
    }
    
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &lcd_width, &lcd_height);
    
    content_lcd_dev = zoom_data_ptr->content_lcd_dev;
    
    // 拿内容层的信息
    UILAYER_GetLayerInfo(&zoom_data_ptr->content_lcd_dev, &content_layer_info);
    in_param.src_width  = content_layer_info.layer_width;
    in_param.src_height = content_layer_info.layer_height;
	in_param.rect = UILAYER_GetLayerRect(&content_lcd_dev);
    
    if ( PNULL != zoom_data_ptr->rect_ptr )
    {
        if ( GUI_IsInvalidRectEx( *zoom_data_ptr->rect_ptr ) )
        {
            return;
        }
    }

	//算法需要内存对齐
// 	in_param.rect.top -= MMITHEME_ZOOM_MEM_ALIGN;
// 	in_param.rect.bottom += MMITHEME_ZOOM_MEM_ALIGN;
//     in_param.src_width  = in_param.rect.right - in_param.rect.left + 1;
//     in_param.src_height = in_param.rect.bottom - in_param.rect.top + 1;
    
    // 创建临时层
//     create_info.lcd_id = GUI_MAIN_LCD_ID;
//     create_info.width = in_param.src_width;
//     create_info.height = in_param.src_height;
//     create_info.is_static_layer = TRUE;
//     create_info.offset_x = in_param.rect.left;
//     create_info.offset_y = in_param.rect.top;
//     UILAYER_CreateLayer( &create_info, &content_lcd_dev );
//     UILAYER_BltLayerToLayer(&content_lcd_dev, &zoom_data_ptr->content_lcd_dev, zoom_data_ptr->rect_ptr, FALSE );


    // 创建缩放层
    create_info.lcd_id = GUI_MAIN_LCD_ID;
    create_info.width = in_param.src_width;
	//目标buffer需要适当大一点，否则超出区域就会被截断
    create_info.height = in_param.src_height * se_ptr->dst_size/MMITHEME_ZOOM_DEFAULT_DST_SIZE;
    if ( create_info.height > lcd_height )
    {
        create_info.height = lcd_height;
    }
    create_info.format = UILAYER_MEM_FPS_ACCELERATE;
    create_info.is_static_layer = TRUE;
    create_info.offset_x = in_param.rect.left;

	if ( create_info.height - (uint16)in_param.src_height > 0 )
	{
		create_info.offset_y = in_param.rect.top - (create_info.height - (uint16)in_param.src_height) / 2;
	}
	else
	{
		create_info.offset_y = in_param.rect.top;
	}
    
    UILAYER_CreateLayer( &create_info, &zoomse_lcd_dev );
    UILAYER_Clear(&zoomse_lcd_dev);

    in_param.dst_width  = create_info.width;
    in_param.dst_height = create_info.height;

    in_param.src_ptr = UILAYER_GetLayerBufferPtr( &content_lcd_dev );
    in_param.color = ConvertRgb565ToArgb888( zoom_data_ptr->mask_color, zoom_data_ptr->mask_alpha );
    in_param.total_frame = ( is_zoom_out ) ? se_ptr->zoom_out_frame : se_ptr->zoom_in_frame;
    in_param.is_zoom_out = is_zoom_out;
    in_param.ext_size = se_ptr->ext_size;

	if ( PNULL != zoom_data_ptr->rect_ptr )
    {
        in_param.rect = *zoom_data_ptr->rect_ptr;
        in_param.is_clip_rect = TRUE;
    }

    if ( in_param.ext_size > 0 )
    {
        in_param.ext_ptr = SCI_ALLOCA( in_param.ext_size );
    }

    if ( 0 == in_param.ext_size
        || PNULL != in_param.ext_ptr )
    {
        uint32 i = 0;
        UILAYER_APPEND_BLT_T old_array[UILAYER_TOTAL_BLT_COUNT] = {0};
        UILAYER_APPEND_BLT_T cur_array[3] = {0};
        uint32 old_count = UILAYER_GetBltLayer(old_array,UILAYER_TOTAL_BLT_COUNT);
        uint32 old_format = UILAYER_GetLayerMemFormat( &mask_lcd_dev );
#ifdef CALTIMEZOOM  
        uint32 tick_total=0,tick3=0,ticks=0;
#endif
        cur_array[0].lcd_dev_info = *MMITHEME_GetDefaultLcdDev();
        cur_array[1].lcd_dev_info = mask_lcd_dev;
        cur_array[2].lcd_dev_info = zoomse_lcd_dev;

        UILAYER_SetBltLayer( cur_array, 3 );

        UILAYER_SetLayerMemFormat( &mask_lcd_dev, old_format | UILAYER_MEM_FORMAT_IMMUTABLE );

        if ( !is_zoom_out )
        {
            UILAYER_Clear( &mask_lcd_dev );
        }
#ifdef CALTIMEZOOM            
		tick_total = SCI_GetTickCount();
#endif	        
        for (; i <= in_param.total_frame; i++)
        {
            in_param.cur_frame = i;
	           
            UILAYER_SwitchDoubleMem( &zoomse_lcd_dev, TRUE );
            in_param.dst_ptr = UILAYER_GetLayerBufferPtr( &zoomse_lcd_dev );
 
            if ( i < in_param.total_frame  )
            {
                UILAYER_SetDirectDraw(TRUE);
                UILAYER_SetMainLayerImmutable( TRUE );
            }

            if ( PNULL != in_param.src_ptr
                && PNULL != in_param.dst_ptr )
            {
#ifdef CALTIMEZOOM            
                ticks = SCI_GetTickCount();
#endif	
                UILAYER_Clear( &zoomse_lcd_dev );
                
                se_ptr->ZoomseFunc( &in_param, &out_param );
                
                UILAYER_FillColor( &mask_lcd_dev, out_param.color );
                
#ifdef CALTIMEZOOM				
                SCI_TRACE_LOW( "ZoomseFunc_step2, ticks = %d", SCI_GetTickCount() - ticks );
#endif
                //设置callback
                //UILAYER_SetBlendCallback( &zoomse_lcd_dev, ZoomseBlendCallBack, 0 );
                //UILAYER_SetBlendCallback( &mask_lcd_dev, MaskBlendCallBack, out_param.color );
#ifdef CALTIMEZOOM            
                tick3 = SCI_GetTickCount();
#endif
                GUILCD_Invalidate( GUI_MAIN_LCD_ID, 0 );
#ifdef CALTIMEZOOM				
                SCI_TRACE_LOW( "ZoomseFunc_step3, ticks = %d", SCI_GetTickCount() - tick3 );
#endif           
                
#ifdef WIN32
                SCI_Sleep(30);
#else
                SCI_Sleep( se_ptr->sleep_ticks );
#endif
            }
        }
#ifdef CALTIMEZOOM				
		SCI_TRACE_LOW( "ZoomseFunc_total, ticks = %d", SCI_GetTickCount() - tick_total );
#endif         
        UILAYER_SetBltLayer( old_array, old_count );

        UILAYER_SetLayerMemFormat( &mask_lcd_dev, old_format );
    }

    UILAYER_ReleaseLayer(&zoomse_lcd_dev);

    //UILAYER_ReleaseLayer(&content_lcd_dev);

    if ( PNULL != in_param.ext_ptr )
    {
        SCI_FREE( in_param.ext_ptr );
    }
}

#endif
#endif


