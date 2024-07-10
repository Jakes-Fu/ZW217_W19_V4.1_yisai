

/*****************************************************************************
** File Name:      mmi_livewallpaper_collision_menu.c                                    *
** Author:         Glen.Li                                                       *
** Date:           19/09/2011                                                *
** Copyright:      2011 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe menu collision wallpaer       *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 09/2011       Glen.Li          Create                                  *
******************************************************************************/
#ifndef _MMI_LIVEWALLPAPER_COLLISION_MENU_C_
#define _MMI_LIVEWALLPAPER_COLLISION_MENU_C_
#if defined(LIVE_WALLPAPER_SUPPORT_COLLISION_MENU)

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
#include "mmk_gsensor.h"
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define COLLISION_WALL_PAPER_TIMER_INTERVAL (30)  /* 碰撞效果墙纸帧定时器*/
#define COLLISION_UPDATE_BALL_TIMER_INTERVAL (1000)  /* 球个数 更新定时器*/
#define SCR_SIZE    ((MMI_MAINSCREEN_HEIGHT*MMI_MAINSCREEN_WIDTH)*sizeof(uint16))
#define BACKGROUND_PICTURE_FORMAT  (IMGREF_FORMAT_RGB565)
#define  BALL_UPDATE_NUM  (1)
#define  COLLISION_BALL_AUTOADD_MAX_NUM  (10) 
#define  INITIAL_BALL_NUM  (6)
#define COLL_ICON_NUM_TOTAL (25)
#define BLOCK_MEM_POOL_SIZE_COLLISION_MENU (800 * 1024)   // Set a  MEM POOL for different livewallpaper respectively;
/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/

typedef struct _COLLISION_MENU_WALLPAPER_PARAM_T_
{
    SE_HANDLE se_handle;
    MMI_HANDLE_T win_handle;
    SE_COLLISION_CLICK_IN_T tp_icon_point; 
    GUI_POINT_T  touch_centre_pos;
    uint16  total_ball_num;
    uint16  current_ball_num;
    uint16  update_ball_num;
    uint16  initial_ball_num;
    uint16 width;
    uint16 height;
    IMGREF_DATA_FORMAT_E bg_picture_format;

    SE_COLLISION_IOCTRL_E render_type;
    uint32 gravity_direction;
    SE_COLLISION_MODE_E collision_mode;
}COLLISION_MENU_WALLPAPER_PARAM_T;
/**--------------------------------------------------------------------------*
 **                         LOCAL DEFINITION                                 *
 **--------------------------------------------------------------------------*/

LOCAL COLLISION_MENU_WALLPAPER_PARAM_T s_Collision_menu_wallpaper_param = {0};
LOCAL SE_COLLISION_RENDER_IN_T s_param_input_menu = {0};
LOCAL SE_COLLISION_RENDER_OUT_T s_param_output_menu = {0};
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
LOCAL BOOLEAN CollisionInit(void);
/*****************************************************************************/
//  Description : 初始化
//  Global resource dependence : 
//  Author:arvin.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CollisionConstruct(MMI_WIN_ID_T win_id);

/*****************************************************************************/
//  Description : 准备背景数据
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CollisionDrawBg(MMI_WIN_ID_T win_id,const GUI_LCD_DEV_INFO *dev_info_ptr);

/*****************************************************************************/
//  Description : 算法
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL void CollisionCalculate(LW_CAL_PARAM *lw_cal_param);
/*****************************************************************************/
//  Description : 释放
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:销毁临时申请的变量
/*****************************************************************************/
LOCAL void CollisionDestruct(void);
/*****************************************************************************/
//  Description : 交互事件处理
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CollisionHandleEvent(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
/*****************************************************************************/
//  Description : 静态数据初始化
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL void CollisionDeinit(void);
/*****************************************************************************/
//  Description : save tp point each timer time.
//  Global resource dependence :
//  Author: hongbo.lan
//  Note:
/*****************************************************************************/
LOCAL void SaveCollisionTPPoint(GUI_POINT_T pt, MMI_MESSAGE_ID_E msg_id );
/*****************************************************************************/
//  Description : 获得动态壁纸类型
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL uint32 CollisionGetType(void);
/*****************************************************************************/
//  Description : 获得动态壁纸ThumbNail信息
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL void CollisionGetThumbNailInfo(LW_THUMBNAIL_INFO *thumbnail_info_ptr);
/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/
PUBLIC const MMI_LIVE_WALLPAPER_ITEM_INFO_T g_collision_menu_Instance =
{
    CollisionInit,
    CollisionConstruct,
    CollisionDrawBg,
    CollisionCalculate,
    CollisionDestruct,
    CollisionHandleEvent,
    CollisionDeinit,
    CollisionGetType,
    CollisionGetThumbNailInfo,
    PNULL,
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
LOCAL BOOLEAN CollisionInit(void)
{
    BOOLEAN b_result = TRUE;
    const uint8 *ball_ptr = PNULL;
    const uint8 *box_ptr = PNULL; 
    uint32 ball_size = 0;
    uint32 box_size = 0;
    GUI_POINT_T             ball_pt={0};
    GUI_POINT_T             box_pt={0};
    uint16 bg_width = {0};
    uint16 bg_height = {0};
    uint16 ball_width = {0};
    uint16 ball_height = {0};
    uint16 box_width = {0};
    uint16 box_height = {0};
    SE_COLLISION_INIT_T init_param = {0};
    uint32 *psz_ballbuf_ptr[INITIAL_BALL_NUM] = {0};
    GUI_COLOR_T *target_box_buf_ptr = PNULL;
    GUIRES_DISPLAY_T     display_info = {0};
    
    display_info.format = IMGREF_FORMAT_ARGB888; 
    
    
    if(0 == s_Collision_menu_wallpaper_param.se_handle)
    {   
        uint32 i = 0;
        
        GUILCD_GetLogicWidthHeight( GUI_MAIN_LCD_ID, &(s_Collision_menu_wallpaper_param.width), &(s_Collision_menu_wallpaper_param.height) );

        GUIRES_GetImgWidthHeight(&bg_width
                                                                , &bg_height
                                                                , IMAGE_SET_DYNAMIC_WALLPAPER_COLLISION_MENU_BACKGROUND_JPG_FORMAL_1
                                                                , VIRTUAL_WIN_ID);
        GUIRES_GetImgWidthHeight(&box_width
                                                                , &box_height
                                                                , IMAGE_SET_DYNAMIC_WALLPAPER_COLLISION_MENU_BOX_1
                                                                , VIRTUAL_WIN_ID);
        GUIRES_GetImgWidthHeight(&ball_width
                                                                , &ball_height
                                                                , IMAGE_SET_DYNAMIC_WALLPAPER_COLLISION_MENU_BALL_1
                                                                , VIRTUAL_WIN_ID);

                      
        
        target_box_buf_ptr = (GUI_COLOR_T*)MMILIVEWALLPAPER_ALLOC(box_width*box_height * 4);//MMILIVEWALLPAPER_ALLOC
        if(PNULL == target_box_buf_ptr)
        {
	    b_result = FALSE;
            return b_result;
        }
        //protection mechanism would be used if win_handle is zero;MMI_GetLabelImage()  needed;
      //  if(0 != VIRTUAL_WIN_ID )    
        {
            box_ptr = MMI_GetLabelImage(IMAGE_SET_DYNAMIC_WALLPAPER_COLLISION_MENU_BOX_1, 
                                            VIRTUAL_WIN_ID, 
                                            &box_size);

            GUIRES_CopyImgBufEx(box_ptr
                            ,box_width
                            ,box_height
                            ,box_width * box_height * 4
                            ,box_size
                            ,&box_pt
                            ,PNULL
                            ,(GUI_COLOR_T *) target_box_buf_ptr
                            ,&display_info);


        }
        
        s_Collision_menu_wallpaper_param.collision_mode = SE_COLLISION_MODE_MENU;
        s_Collision_menu_wallpaper_param.render_type = SE_COLLISION_IOCTRL_UPDATE;
        s_Collision_menu_wallpaper_param.bg_picture_format = BACKGROUND_PICTURE_FORMAT;
        s_Collision_menu_wallpaper_param.total_ball_num = COLL_ICON_NUM_TOTAL;
        s_Collision_menu_wallpaper_param.update_ball_num = BALL_UPDATE_NUM; 
        s_Collision_menu_wallpaper_param.initial_ball_num = INITIAL_BALL_NUM;

        init_param.collision_mode = s_Collision_menu_wallpaper_param.collision_mode;
        init_param.icon_max_num = s_Collision_menu_wallpaper_param.total_ball_num;
        init_param.bg_data_format = IMGREF_FORMAT_RGB565;
        init_param.bg_width =s_Collision_menu_wallpaper_param.width;
        init_param.bg_height = s_Collision_menu_wallpaper_param.height;

        init_param.bg_img_addr = PNULL;
        init_param.bg_img_size = init_param.bg_height * init_param.bg_width * 2;


        init_param.icon_img_num = INITIAL_BALL_NUM + 1;
        s_Collision_menu_wallpaper_param.current_ball_num = 1;

        init_param.icon_img[0].icon_num=2;
        init_param.icon_img[0].data_format = IMGREF_FORMAT_ARGB888;
        init_param.icon_img[0].icon_width = box_width/2;
        init_param.icon_img[0].icon_height = box_height;
        init_param.icon_img[0].icon_type=SE_ICON_TYPE_POLYON;
        init_param.icon_img[0].img.polycon.vertex_point_num= 4;
        init_param.centre_pos[0].x = 160;
        init_param.centre_pos[0].y = 240;
        init_param.icon_img[0].icon_img_addr = (uint32)(void*)target_box_buf_ptr;
        init_param.icon_img[0].icon_img_buf_size = box_width*box_height*4;

#if 0
        init_param.icon_img[0].img.polycon.vertex_point_ptr[0].x= -1;
        init_param.icon_img[0].img.polycon.vertex_point_ptr[0].y= -29;
        init_param.icon_img[0].img.polycon.vertex_point_ptr[1].x= -23;
        init_param.icon_img[0].img.polycon.vertex_point_ptr[1].y= -21;
        init_param.icon_img[0].img.polycon.vertex_point_ptr[2].x= -23;
        init_param.icon_img[0].img.polycon.vertex_point_ptr[2].y= 12;
        init_param.icon_img[0].img.polycon.vertex_point_ptr[3].x= -19;
        init_param.icon_img[0].img.polycon.vertex_point_ptr[3].y= 22;
        init_param.icon_img[0].img.polycon.vertex_point_ptr[4].x= -12;
        init_param.icon_img[0].img.polycon.vertex_point_ptr[4].y= 30;
        init_param.icon_img[0].img.polycon.vertex_point_ptr[5].x= -1;
        init_param.icon_img[0].img.polycon.vertex_point_ptr[5].y= 30;
        init_param.icon_img[0].img.polycon.vertex_point_ptr[6].x= 15;
        init_param.icon_img[0].img.polycon.vertex_point_ptr[6].y= 28;
        init_param.icon_img[0].img.polycon.vertex_point_ptr[7].x= 23;
        init_param.icon_img[0].img.polycon.vertex_point_ptr[7].y= 11;
        init_param.icon_img[0].img.polycon.vertex_point_ptr[8].x= 23;
        init_param.icon_img[0].img.polycon.vertex_point_ptr[8].y= -21;
#else
	init_param.icon_img[0].img.polycon.vertex_point_ptr[0].x= -25;
	init_param.icon_img[0].img.polycon.vertex_point_ptr[0].y= -25;
	init_param.icon_img[0].img.polycon.vertex_point_ptr[1].x= -25;
	init_param.icon_img[0].img.polycon.vertex_point_ptr[1].y= 25;
	init_param.icon_img[0].img.polycon.vertex_point_ptr[2].x= 25;
	init_param.icon_img[0].img.polycon.vertex_point_ptr[2].y= 25;
	init_param.icon_img[0].img.polycon.vertex_point_ptr[3].x= 25;
	init_param.icon_img[0].img.polycon.vertex_point_ptr[3].y= -25;
#endif

        for(i=1; i<(init_param.icon_img_num); i++)
        {
        	init_param.icon_img[i].data_format = IMGREF_FORMAT_ARGB888;
        	init_param.icon_img[i].icon_width = ball_width /2;
        	init_param.icon_img[i].icon_height = ball_height;
        	init_param.icon_img[i].icon_num=2;
        	init_param.centre_pos[i].x = 100;
        	init_param.centre_pos[i].y = 240;
        	init_param.icon_img[i].icon_type=SE_ICON_TYPE_POLYON;
            
            psz_ballbuf_ptr[i- 1] = (uint32*)SCI_ALLOCA(ball_width*ball_height * 4);//MMILIVEWALLPAPER_ALLOC
            if(PNULL == psz_ballbuf_ptr[i- 1])
            {
    	        b_result = FALSE;
                return b_result;
            }
            SCI_MEMSET(psz_ballbuf_ptr[i - 1], 0, ball_width*ball_height * 4 );
            //get picture information of the ball; start;
            ball_ptr = MMI_GetLabelImage(IMAGE_SET_DYNAMIC_WALLPAPER_COLLISION_MENU_BALL_1 + i - 1
                                            ,VIRTUAL_WIN_ID
                                            ,&ball_size);

            GUIRES_CopyImgBufEx(ball_ptr
                            ,ball_width
                            ,ball_height
                            ,ball_width * ball_height * 4
                            ,ball_size
                            ,&ball_pt
                            ,PNULL
                            ,(void *)psz_ballbuf_ptr[i - 1]
                            ,&display_info);
            
        	init_param.icon_img[i].icon_img_addr = (uint32)psz_ballbuf_ptr[i - 1];
        	init_param.icon_img[i].img.polycon.vertex_point_num = 8;

        	init_param.icon_img[i].icon_img_buf_size = ball_width*ball_height*4;
            
#if 0
        	init_param.icon_img[i].img.polycon.vertex_point_ptr[0].x= 1;
        	init_param.icon_img[i].img.polycon.vertex_point_ptr[0].y= -24;

        	init_param.icon_img[i].img.polycon.vertex_point_ptr[1].x= -15;
        	init_param.icon_img[i].img.polycon.vertex_point_ptr[1].y= -18;

        	init_param.icon_img[i].img.polycon.vertex_point_ptr[2].x= -18;
        	init_param.icon_img[i].img.polycon.vertex_point_ptr[2].y= 1;

        	init_param.icon_img[i].img.polycon.vertex_point_ptr[3].x= -14;
        	init_param.icon_img[i].img.polycon.vertex_point_ptr[3].y= 20;
        	
        	init_param.icon_img[i].img.polycon.vertex_point_ptr[4].x= 1;
        	init_param.icon_img[i].img.polycon.vertex_point_ptr[4].y= 25;

        	init_param.icon_img[i].img.polycon.vertex_point_ptr[5].x= 15;
        	init_param.icon_img[i].img.polycon.vertex_point_ptr[5].y= 19;

        	init_param.icon_img[i].img.polycon.vertex_point_ptr[6].x= 18;
        	init_param.icon_img[i].img.polycon.vertex_point_ptr[6].y= 1;

        	init_param.icon_img[i].img.polycon.vertex_point_ptr[7].x= 16;
        	init_param.icon_img[i].img.polycon.vertex_point_ptr[7].y= -18;
#else
        	init_param.icon_img[i].img.polycon.vertex_point_ptr[0].x= -9;
        	init_param.icon_img[i].img.polycon.vertex_point_ptr[0].y= -22;

        	init_param.icon_img[i].img.polycon.vertex_point_ptr[1].x= -17;
        	init_param.icon_img[i].img.polycon.vertex_point_ptr[1].y= -11;

        	init_param.icon_img[i].img.polycon.vertex_point_ptr[2].x= -17;
        	init_param.icon_img[i].img.polycon.vertex_point_ptr[2].y= 11;

        	init_param.icon_img[i].img.polycon.vertex_point_ptr[3].x= -9;
        	init_param.icon_img[i].img.polycon.vertex_point_ptr[3].y= 22;
        	
        	init_param.icon_img[i].img.polycon.vertex_point_ptr[4].x= 9;
        	init_param.icon_img[i].img.polycon.vertex_point_ptr[4].y= 22;

        	init_param.icon_img[i].img.polycon.vertex_point_ptr[5].x= 17;
        	init_param.icon_img[i].img.polycon.vertex_point_ptr[5].y= 11;

        	init_param.icon_img[i].img.polycon.vertex_point_ptr[6].x= 17;
        	init_param.icon_img[i].img.polycon.vertex_point_ptr[6].y= -11;

        	init_param.icon_img[i].img.polycon.vertex_point_ptr[7].x= 9;
        	init_param.icon_img[i].img.polycon.vertex_point_ptr[7].y= -22;
#endif


        }
        s_Collision_menu_wallpaper_param.se_handle = SE_Create(SE_ID_COLLISION,&init_param);
        if(s_Collision_menu_wallpaper_param.se_handle > 0)
        {
            b_result = TRUE;
        }
        else
        {    
    	    b_result = FALSE;
        }

        for(i = 0; i<(INITIAL_BALL_NUM); i++)
        {
            MMILIVEWALLPAPER_FREE(psz_ballbuf_ptr[i]);//SCI_FREE
        }
        MMILIVEWALLPAPER_FREE(target_box_buf_ptr);

    }
    return b_result;
}

/*****************************************************************************/
//  Description : 初始化
//  Global resource dependence : 
//  Author:arvin.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CollisionConstruct(MMI_WIN_ID_T win_id)
{
    BOOLEAN b_result = TRUE;

    return b_result;
}

/*****************************************************************************/
//  Description : 线性光幕准备背景数据
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CollisionDrawBg(MMI_WIN_ID_T win_id,const GUI_LCD_DEV_INFO *dev_info_ptr)
{
    BOOLEAN b_result = FALSE;
    GUI_POINT_T display_point = {0,0};
	
    if (PNULL != dev_info_ptr)
    {
        b_result = GUIRES_DisplayImg(&display_point, 
        PNULL, 
        PNULL, 
        win_id, 
        IMAGE_SET_DYNAMIC_WALLPAPER_COLLISION_MENU_BACKGROUND_JPG_FORMAL_1, 
        dev_info_ptr);
    }
    
    return b_result;
}

/*****************************************************************************/
//  Description : 线性光幕算法
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL void CollisionCalculate(LW_CAL_PARAM *lw_cal_param)
{
    SE_RESULT_E ret = SE_RESULT_SUCCESS;
    
    if(PNULL == lw_cal_param || PNULL == lw_cal_param->dest_buf_info.buf_ptr || PNULL == lw_cal_param->src_buf_info.buf_ptr )
    {
        return;
    }
    
    SCI_MEMSET(&s_param_input_menu,0,sizeof(s_param_input_menu));
    SCI_MEMSET(&s_param_output_menu,0,sizeof(s_param_output_menu));
    
    SCI_MEMCPY((void*)(lw_cal_param->dest_buf_info.buf_ptr ), (void*)(lw_cal_param->src_buf_info.buf_ptr), SCR_SIZE);

    if(SE_COLLISION_MODE_EGG == s_Collision_menu_wallpaper_param.collision_mode)
    {
        if(SE_COLLISION_IOCTRL_CHG_GRAVITY == s_Collision_menu_wallpaper_param.render_type)
        {
            s_param_input_menu.type = s_Collision_menu_wallpaper_param.render_type;
            s_param_input_menu.param.chg_gravity.direction = s_Collision_menu_wallpaper_param.gravity_direction;

            if(0 != s_Collision_menu_wallpaper_param.se_handle)
            {
                ret = SE_Render(s_Collision_menu_wallpaper_param.se_handle, &s_param_input_menu, &s_param_output_menu);
            }
            if(SE_COLLISION_IOCTRL_CHG_GRAVITY == s_Collision_menu_wallpaper_param.render_type)
            {
                s_Collision_menu_wallpaper_param.render_type = SE_COLLISION_IOCTRL_UPDATE;
            }
            s_param_input_menu.param.update_param.bg_img_addr = (uint32)lw_cal_param->dest_buf_info.buf_ptr; 
            s_param_input_menu.param.update_param.bg_img_size = s_Collision_menu_wallpaper_param.width * s_Collision_menu_wallpaper_param.height * 2;
            s_param_input_menu.param.update_param.bg_data_format = s_Collision_menu_wallpaper_param.bg_picture_format;
            s_param_input_menu.type = s_Collision_menu_wallpaper_param.render_type;
            s_param_input_menu.param.update_param.icon_img_num = 0;

            if(0 != s_Collision_menu_wallpaper_param.se_handle)
            {
                ret = SE_Render(s_Collision_menu_wallpaper_param.se_handle, &s_param_input_menu, &s_param_output_menu);
            }
        }
        else if(SE_COLLISION_IOCTRL_CLICK == s_Collision_menu_wallpaper_param.render_type)
        {
            s_param_input_menu.type = s_Collision_menu_wallpaper_param.render_type;   
            s_param_input_menu.param.click_param.click_action = s_Collision_menu_wallpaper_param.tp_icon_point.click_action;
            s_param_input_menu.param.click_param.click_state = s_Collision_menu_wallpaper_param.tp_icon_point.click_state;
            s_param_input_menu.param.click_param.centre_pos.x = s_Collision_menu_wallpaper_param.tp_icon_point.centre_pos.x;
            s_param_input_menu.param.click_param.centre_pos.y = s_Collision_menu_wallpaper_param.tp_icon_point.centre_pos.y;

            if(0 != s_Collision_menu_wallpaper_param.se_handle)
            {
                ret = SE_Render(s_Collision_menu_wallpaper_param.se_handle, &s_param_input_menu, &s_param_output_menu);
            }

            if(SE_COLLISION_IOCTRL_CLICK == s_Collision_menu_wallpaper_param.render_type)
            {
                s_Collision_menu_wallpaper_param.render_type = SE_COLLISION_IOCTRL_UPDATE;
            }
            s_param_input_menu.param.update_param.bg_img_addr = (uint32)lw_cal_param->dest_buf_info.buf_ptr; 
            s_param_input_menu.param.update_param.bg_img_size = s_Collision_menu_wallpaper_param.width * s_Collision_menu_wallpaper_param.height * 2;
            s_param_input_menu.param.update_param.bg_data_format = s_Collision_menu_wallpaper_param.bg_picture_format;
            s_param_input_menu.type = s_Collision_menu_wallpaper_param.render_type;
            s_param_input_menu.param.update_param.icon_img_num = 0;

            if(0 != s_Collision_menu_wallpaper_param.se_handle)
            {
                ret = SE_Render(s_Collision_menu_wallpaper_param.se_handle, &s_param_input_menu, &s_param_output_menu);
            }
            
            s_Collision_menu_wallpaper_param.current_ball_num++;
        }
        else
        {
            
            s_param_input_menu.param.update_param.bg_img_addr = (uint32)lw_cal_param->dest_buf_info.buf_ptr; 
            s_param_input_menu.param.update_param.bg_img_size = s_Collision_menu_wallpaper_param.width * s_Collision_menu_wallpaper_param.height * 2;
            s_param_input_menu.param.update_param.bg_data_format = s_Collision_menu_wallpaper_param.bg_picture_format;
            s_param_input_menu.type = s_Collision_menu_wallpaper_param.render_type;
            s_param_input_menu.param.update_param.icon_img_num = 0;

            if(0 != s_Collision_menu_wallpaper_param.se_handle)
            {
                ret = SE_Render(s_Collision_menu_wallpaper_param.se_handle, &s_param_input_menu, &s_param_output_menu);
            }
        }
    }
    else if(SE_COLLISION_MODE_BALLOON == s_Collision_menu_wallpaper_param.collision_mode)
    {
        s_param_input_menu.param.update_param.bg_img_addr = (uint32)lw_cal_param->dest_buf_info.buf_ptr; 
        s_param_input_menu.param.update_param.bg_img_size = s_Collision_menu_wallpaper_param.width * s_Collision_menu_wallpaper_param.height * 2;
        s_param_input_menu.param.update_param.bg_data_format = s_Collision_menu_wallpaper_param.bg_picture_format;
        s_param_input_menu.type = s_Collision_menu_wallpaper_param.render_type;
        s_param_input_menu.param.update_param.icon_img_num = 0;
        if(0 != s_Collision_menu_wallpaper_param.se_handle)
        {
            ret = SE_Render(s_Collision_menu_wallpaper_param.se_handle, &s_param_input_menu, &s_param_output_menu);
        }
    }
    else if(SE_COLLISION_MODE_MENU == s_Collision_menu_wallpaper_param.collision_mode)
    {
        s_param_input_menu.param.update_param.bg_img_addr = (uint32)lw_cal_param->dest_buf_info.buf_ptr; 
        s_param_input_menu.param.update_param.bg_img_size = s_Collision_menu_wallpaper_param.width * s_Collision_menu_wallpaper_param.height * 2;
        s_param_input_menu.param.update_param.bg_data_format = s_Collision_menu_wallpaper_param.bg_picture_format;
        s_param_input_menu.type = s_Collision_menu_wallpaper_param.render_type;
        s_param_input_menu.param.update_param.icon_img_num = 0;
        if(0 != s_Collision_menu_wallpaper_param.se_handle)
        {
            ret = SE_Render(s_Collision_menu_wallpaper_param.se_handle, &s_param_input_menu, &s_param_output_menu);
        }
    }
    else
    {
        s_param_input_menu.param.update_param.bg_img_addr = (uint32)lw_cal_param->dest_buf_info.buf_ptr; 
        s_param_input_menu.param.update_param.bg_img_size = s_Collision_menu_wallpaper_param.width * s_Collision_menu_wallpaper_param.height * 2;
        s_param_input_menu.param.update_param.bg_data_format = s_Collision_menu_wallpaper_param.bg_picture_format;
        s_param_input_menu.type = s_Collision_menu_wallpaper_param.render_type;
        s_param_input_menu.param.update_param.icon_img_num = 0;
        if(0 != s_Collision_menu_wallpaper_param.se_handle)
        {
            ret = SE_Render(s_Collision_menu_wallpaper_param.se_handle, &s_param_input_menu, &s_param_output_menu);
        }
    }
}
/*****************************************************************************/
//  Description : 线性光幕释放
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL void CollisionDestruct(void)
{
}

/*****************************************************************************/
//  Description : 线性光幕释交互事件处理
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CollisionHandleEvent(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    GUI_POINT_T   point       = {0};
    LOCAL BOOLEAN is_tp_down = FALSE;
    
    switch(msg_id)
    {
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_TP_PRESS_DOWN:
            is_tp_down = TRUE;
            point.x = MMK_GET_TP_X(param);
            point.y = MMK_GET_TP_Y(param);
            SaveCollisionTPPoint(point,msg_id);
            break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_TP_PRESS_UP:
            if(!is_tp_down)
            {
                break;
            }
            is_tp_down = FALSE;
            point.x = MMK_GET_TP_X(param);
            point.y = MMK_GET_TP_Y(param);
            SaveCollisionTPPoint(point,msg_id);
            break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_GSENSOR:
        {
            switch( (uint32 )param)
            {
                case MMK_GSENSOR_STATUS_LEFT_SIDE:
                    s_Collision_menu_wallpaper_param.gravity_direction = SE_GRAVITY_RIGHT;
                    s_Collision_menu_wallpaper_param.render_type = SE_COLLISION_IOCTRL_CHG_GRAVITY;
                    break;
                case MMK_GSENSOR_STATUS_RIGHT_SIDE:
                    s_Collision_menu_wallpaper_param.gravity_direction = SE_GRAVITY_LEFT;
                    s_Collision_menu_wallpaper_param.render_type = SE_COLLISION_IOCTRL_CHG_GRAVITY;
                    break;
                case MMK_GSENSOR_STATUS_UP_SIDE:
                    s_Collision_menu_wallpaper_param.gravity_direction = SE_GRAVITY_DOWN;
                    s_Collision_menu_wallpaper_param.render_type = SE_COLLISION_IOCTRL_CHG_GRAVITY;
                    break;
                case MMK_GSENSOR_STATUS_DOWN_SIDE:
                    s_Collision_menu_wallpaper_param.gravity_direction = SE_GRAVITY_UP;
                    s_Collision_menu_wallpaper_param.render_type = SE_COLLISION_IOCTRL_CHG_GRAVITY;
                    break;
                default:
                    break;
            }
        }
        break;
        default:
            return FALSE;
    }
    return TRUE;
}

/*****************************************************************************/
//  Description : 静态数据初始化
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL void CollisionDeinit(void)
{
    if(s_Collision_menu_wallpaper_param.se_handle > 0)
    {
        SE_Destory(s_Collision_menu_wallpaper_param.se_handle);

        s_Collision_menu_wallpaper_param.se_handle = 0;
        s_Collision_menu_wallpaper_param.win_handle = 0;
        
        s_Collision_menu_wallpaper_param.current_ball_num = 0;  
        s_Collision_menu_wallpaper_param.update_ball_num = 0;  
        s_Collision_menu_wallpaper_param.total_ball_num = 0;   

    }
}
/*****************************************************************************/
//  Description : save tp point each timer time.
//  Global resource dependence :
//  Author: hongbo.lan
//  Note:
/*****************************************************************************/
LOCAL void SaveCollisionTPPoint(GUI_POINT_T pt, MMI_MESSAGE_ID_E msg_id )
{
    if( (s_Collision_menu_wallpaper_param.current_ball_num < s_Collision_menu_wallpaper_param.total_ball_num))
    {
        switch(msg_id)
        {
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
            case MSG_TP_PRESS_DOWN:
                s_Collision_menu_wallpaper_param.tp_icon_point.click_state = SE_CLICK_DOWN; 
                break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
            case MSG_TP_PRESS_UP:
                s_Collision_menu_wallpaper_param.tp_icon_point.click_action = SE_ACTION_ADD;
                s_Collision_menu_wallpaper_param.render_type = SE_COLLISION_IOCTRL_CLICK;
                s_Collision_menu_wallpaper_param.tp_icon_point.click_state = SE_CLICK_UP; 
                break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
            default:
                break;
        }

        s_Collision_menu_wallpaper_param.tp_icon_point.centre_pos.x = pt.x;
        s_Collision_menu_wallpaper_param.tp_icon_point.centre_pos.y = pt.y;
    }
}
/*****************************************************************************/
//  Description : 获得动态壁纸类型
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL uint32 CollisionGetType(void)
{
    return MMI_LIVE_WALLPAPER_TYPE_COLLISION_MENU;
}
/*****************************************************************************/
//  Description : 获得动态壁纸ThumbNail信息
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL void CollisionGetThumbNailInfo(LW_THUMBNAIL_INFO *thumbnail_info_ptr)
{
    if(thumbnail_info_ptr)
    {
        thumbnail_info_ptr->is_image_file = FALSE;
        thumbnail_info_ptr->image_id = IMAGE_WALLPAPER_COLLISION_MENU_THUMBNAIL;

        thumbnail_info_ptr->is_not_string = FALSE;
        thumbnail_info_ptr->text_id = TXT_SET_WALLPAPER_COLLISION_MENU;
        thumbnail_info_ptr->instance_mem_pool_size = BLOCK_MEM_POOL_SIZE_COLLISION_MENU;
    }
}
#endif //LIVE_WALLPAPER_SUPPORT_COLLISION_MENU

#endif //_MMI_LIVEWALLPAPER_COLLISION_MENU_C_


/*Edit by script, ignore 4 case. Thu Apr 26 19:00:57 2012*/ //IGNORE9527
