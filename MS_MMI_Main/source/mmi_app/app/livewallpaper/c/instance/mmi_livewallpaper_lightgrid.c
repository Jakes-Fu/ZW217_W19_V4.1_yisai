

/*****************************************************************************
** File Name:      mmi_livewallpaper_lightgrid.c                                    *
** Author:         Gaily.Wang                                                *
** Date:           18/06/2011                                                *
** Copyright:      2011 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe runninglight wallpaer       *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 06/2011       Gaily.Wang          Create                                  *
** 08/2011       Arvin.wu          Modify                                  *
******************************************************************************/
#ifndef _MMI_LIVEWALLPAPER_LIGHTGRID_C_
#define _MMI_LIVEWALLPAPER_LIGHTGRID_C_

#include "mmi_app_livewallpaper_trc.h"
#ifdef LIVE_WALLPAPER_SUPPORT_LIGHTGRID

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
#define LIGHT_GRID_GRID_NUM (10)
#define LIGHT_GRID_WALL_PAPER_TIMER_INTERVAL (100)  /* 霓虹格子效果墙纸帧定时器*/
#define LIGHT_GRID_UPDATE_BALL_TIMER_INTERVAL (1000)  /* 球个数 更新定时器*/
#define LIGHT_GRID_SCR_SIZE    ((MMI_MAINSCREEN_HEIGHT*MMI_MAINSCREEN_WIDTH))
#define LIGHT_GRID_PICTURE_FORMAT  (IMGREF_FORMAT_RGB565)
#define  LIGHT_GRID_UPDATE_NUM  (1)
#define BLOCK_MEM_POOL_SIZE_LIGHT_GRID  (10 * 1024)   // Set a  MEM POOL for different livewallpaper respectively;

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/

typedef struct _LIGHT_GRID_WALLPAPER_PARAM_T_
{
    SE_HANDLE se_handle;
    MMI_HANDLE_T win_handle;
    uint32 *target_bg_buf_ptr;
    uint32 *target_bg_out_buf_ptr;
    SE_GRID_POINT_PROP_T grid_img[LIGHT_GRID_GRID_NUM]; 
    uint16  total_touch_point_num;
    uint16  current_touch_point_num;
    uint16  update_touch_point_num;  
    uint16 width;
    uint16 height;
    IMGREF_DATA_FORMAT_E grid_picture_format;
    BOOLEAN is_update_grid;

    uint32 gravity_direction;
}LIGHT_GRID_WALLPAPER_PARAM_T;
/**--------------------------------------------------------------------------*
 **                         LOCAL DEFINITION                                 *
 **--------------------------------------------------------------------------*/
LIGHT_GRID_WALLPAPER_PARAM_T s_light_grid_wallpaper_param = {0};
/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                                                         */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : 初始化
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN LightgridInit(void);
/*****************************************************************************/
//  Description : 初始化
//  Global resource dependence : 
//  Author:arvin.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN LightgridConstruct(MMI_WIN_ID_T win_id);

/*****************************************************************************/
//  Description : 准备背景数据
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN LightgridDrawBg(MMI_WIN_ID_T win_id,const GUI_LCD_DEV_INFO *dev_info_ptr);

/*****************************************************************************/
//  Description : 算法
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL void LightgridCalculate(LW_CAL_PARAM *lw_cal_param);
/*****************************************************************************/
//  Description : 释放
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:销毁临时申请的变量
/*****************************************************************************/
LOCAL void LightgridDestruct(void);
/*****************************************************************************/
//  Description : 交互事件处理
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN LightgridHandleEvent(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
/*****************************************************************************/
//  Description : 静态数据初始化
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL void LightgridDeinit(void);
/*****************************************************************************/
//  Description : save light grid tp point each timer time.
//  Global resource dependence :
//  Author: hongbo.lan
//  Note:
/*****************************************************************************/
LOCAL void SaveLightGridTPPoint(GUI_POINT_T pt, MMI_MESSAGE_ID_E msg_id );

/*****************************************************************************/
//  Description : 获得动态壁纸类型
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL uint32 LightgridGetType(void);
/*****************************************************************************/
//  Description : 获得动态壁纸ThumbNail信息
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL void LightgridGetThumbNailInfo(LW_THUMBNAIL_INFO *thumbnail_info_ptr);
/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/
PUBLIC const MMI_LIVE_WALLPAPER_ITEM_INFO_T g_lightgrid_Instance =
{
    LightgridInit,
    LightgridConstruct,
    LightgridDrawBg,
    LightgridCalculate,
    LightgridDestruct,
    LightgridHandleEvent,
    LightgridDeinit,
    LightgridGetType,
    LightgridGetThumbNailInfo,
    PNULL,
    PNULL,
    PNULL,
    PNULL
};
/*---------------------------------------------------------------------------*/
/*                          FUNCTION DEFINITION                                                              */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : 初始化
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN LightgridInit(void)
{
    BOOLEAN b_result = TRUE;
    return b_result;
}

/*****************************************************************************/
//  Description :初始化
//  Global resource dependence : 
//  Author:arvin.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN LightgridConstruct(MMI_WIN_ID_T win_id)
{
    BOOLEAN b_result = TRUE;
    SE_GRID_INIT_PARAM_T init_param = {0};
    
    if(0 == s_light_grid_wallpaper_param.se_handle)
    {   
        s_light_grid_wallpaper_param.total_touch_point_num = LIGHT_GRID_GRID_NUM;
        s_light_grid_wallpaper_param.target_bg_buf_ptr = PNULL;
        s_light_grid_wallpaper_param.grid_picture_format = LIGHT_GRID_PICTURE_FORMAT;
        GUILCD_GetLogicWidthHeight(MAIN_LCD_ID,&s_light_grid_wallpaper_param.width,&s_light_grid_wallpaper_param.height);
        s_light_grid_wallpaper_param.current_touch_point_num = 0;
        
        init_param.width  =s_light_grid_wallpaper_param.width;
        init_param.height = s_light_grid_wallpaper_param.height;
        init_param.target_data_format = s_light_grid_wallpaper_param.grid_picture_format;
        init_param.target_pixels  = s_light_grid_wallpaper_param.target_bg_buf_ptr;
       
        s_light_grid_wallpaper_param.se_handle = SE_Create(SE_ID_GRID,&init_param);
        if(s_light_grid_wallpaper_param.se_handle > 0)
        {
            b_result = TRUE;
        }
        else
        {    
    	    b_result = FALSE;
        }

    }
    return b_result;
}
/*****************************************************************************/
//  Description :准备背景数据
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN LightgridDrawBg(MMI_WIN_ID_T win_id,const GUI_LCD_DEV_INFO *dev_info_ptr)
{
    BOOLEAN b_result = TRUE;
    uint8* buf_ptr = PNULL;
    SE_GRID_INIT_PARAM_T init_param = {0};
    BOOLEAN is_create_se_here = FALSE;

    /* 这里处理的比较特殊，因为DrawBg在Init里就画了，
        所以此时se_handle没创建，先创建一次，画一个
        背景，来后销毁。
    */
    if(0 == s_light_grid_wallpaper_param.se_handle)
    {   
        s_light_grid_wallpaper_param.total_touch_point_num = LIGHT_GRID_GRID_NUM;
        s_light_grid_wallpaper_param.target_bg_buf_ptr = PNULL;
        s_light_grid_wallpaper_param.grid_picture_format = LIGHT_GRID_PICTURE_FORMAT;
        GUILCD_GetLogicWidthHeight(MAIN_LCD_ID,&s_light_grid_wallpaper_param.width,&s_light_grid_wallpaper_param.height);
        s_light_grid_wallpaper_param.current_touch_point_num = 0;
        
        init_param.width  =s_light_grid_wallpaper_param.width;
        init_param.height = s_light_grid_wallpaper_param.height;
        init_param.target_data_format = s_light_grid_wallpaper_param.grid_picture_format;
        init_param.target_pixels  = s_light_grid_wallpaper_param.target_bg_buf_ptr;
       
        s_light_grid_wallpaper_param.se_handle = SE_Create(SE_ID_GRID,&init_param);
        if(s_light_grid_wallpaper_param.se_handle > 0)
        {
            is_create_se_here = TRUE;
        }
    }
    
    if(is_create_se_here)
    {
        SE_Destory(s_light_grid_wallpaper_param.se_handle);
        s_light_grid_wallpaper_param.se_handle = 0;
    }
    return b_result;
}

/*****************************************************************************/
//  Description : 算法
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL void LightgridCalculate(LW_CAL_PARAM *lw_cal_param)
{

    SE_GRID_RENDER_INPUT_PARAM_T se_grid_render_input_para = {0};
    SE_RESULT_E ret = SE_RESULT_SUCCESS;

    

    if(PNULL == lw_cal_param || PNULL == lw_cal_param->dest_buf_info.buf_ptr)
    {
        return;
    }
    
    se_grid_render_input_para.point_num    = s_light_grid_wallpaper_param.current_touch_point_num ;    
    se_grid_render_input_para.points = s_light_grid_wallpaper_param.grid_img;
    se_grid_render_input_para.target_pixels = lw_cal_param->dest_buf_info.buf_ptr;
    if(0 != s_light_grid_wallpaper_param.se_handle)
    {
        ret = SE_Render(s_light_grid_wallpaper_param.se_handle, &se_grid_render_input_para, NULL);
    }
        
    s_light_grid_wallpaper_param.current_touch_point_num = 0;  
}
/*****************************************************************************/
//  Description : 释放
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL void LightgridDestruct(void)
{
    if(s_light_grid_wallpaper_param.se_handle > 0)
    {
        SE_Destory(s_light_grid_wallpaper_param.se_handle);

        s_light_grid_wallpaper_param.se_handle = 0;
        s_light_grid_wallpaper_param.win_handle = 0;

        if(PNULL !=  s_light_grid_wallpaper_param.target_bg_buf_ptr)
        {
            s_light_grid_wallpaper_param.target_bg_buf_ptr = PNULL;
        }
    }
}
/*****************************************************************************/
//  Description : 交互事件处理
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN LightgridHandleEvent(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    GUI_POINT_T   point       = {0}; 
    
    switch(msg_id)
    {
    case MSG_TP_PRESS_UP:
    case MSG_TP_PRESS_MOVE:
    case MSG_TP_PRESS_DOWN:
        point.x = MMK_GET_TP_X(param);
        point.y = MMK_GET_TP_Y(param);
        SaveLightGridTPPoint(point,msg_id);
        break;

    default:
        return FALSE;
    }
    #endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    return TRUE;
}

/*****************************************************************************/
//  Description : 静态数据初始化
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL void LightgridDeinit(void)
{	
}

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
/*****************************************************************************/
//  Description : save light grid tp point each timer time.
//  Global resource dependence :
//  Author: hongbo.lan
//  Note:
/*****************************************************************************/
LOCAL void SaveLightGridTPPoint(GUI_POINT_T pt, MMI_MESSAGE_ID_E msg_id )
{
    //SCI_TRACE_LOW:"SaveLightGridTPPoint  entry"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_LIVEWALLPAPER_LIGHTGRID_299_112_2_18_2_32_6_26,(uint8*)"");

    if (s_light_grid_wallpaper_param.current_touch_point_num < LIGHT_GRID_GRID_NUM) 
    {
        switch(msg_id)
        {
            case MSG_TP_PRESS_DOWN:
                s_light_grid_wallpaper_param.grid_img[s_light_grid_wallpaper_param.current_touch_point_num].mode = SE_GRID_MODE_DOWN; 
                break;
            case MSG_TP_PRESS_UP:
                s_light_grid_wallpaper_param.grid_img[s_light_grid_wallpaper_param.current_touch_point_num].mode = SE_GRID_MODE_UP; 
                break;
            case MSG_TP_PRESS_MOVE:
                s_light_grid_wallpaper_param.grid_img[s_light_grid_wallpaper_param.current_touch_point_num].mode = SE_GRID_MODE_MOVE; 
                break;

            default:
                break;
        }

        s_light_grid_wallpaper_param.grid_img[s_light_grid_wallpaper_param.current_touch_point_num].pos.x = pt.x;
        s_light_grid_wallpaper_param.grid_img[s_light_grid_wallpaper_param.current_touch_point_num].pos.y = pt.y;
        s_light_grid_wallpaper_param.current_touch_point_num++;
    }
}
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
/*****************************************************************************/
//  Description : 获得动态壁纸类型
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL uint32 LightgridGetType(void)
{
    return MMI_LIVE_WALLPAPER_TYPE_LIGHTGRID;
}
/*****************************************************************************/
//  Description : 获得动态壁纸ThumbNail信息
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL void LightgridGetThumbNailInfo(LW_THUMBNAIL_INFO *thumbnail_info_ptr)
{      
    if(thumbnail_info_ptr)
    {
        thumbnail_info_ptr->is_image_file = FALSE;
        thumbnail_info_ptr->image_id = IMAGE_WALLPAPER_LIGHTGRID_THUMBNAIL;

        thumbnail_info_ptr->is_not_string = FALSE;
        thumbnail_info_ptr->text_id = TXT_SET_WALLPAPER_LIGHTGRID;
        thumbnail_info_ptr->instance_mem_pool_size = BLOCK_MEM_POOL_SIZE_LIGHT_GRID;
    }
}
#endif// LIVE_WALLPAPER_SUPPORT_LIGHTGRID


#endif //_MMI_LIVEWALLPAPER_LIGHTGRID_C_


/*Edit by script, ignore 6 case. Thu Apr 26 19:00:58 2012*/ //IGNORE9527
