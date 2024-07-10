

/*****************************************************************************
** File Name:      mmi_livewallpaper_linear.c                                    *
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
#ifndef _MMI_LIVEWALLPAPER_POP_C_
#define _MMI_LIVEWALLPAPER_POP_C_

#include "mmi_app_livewallpaper_trc.h"
#ifdef LIVE_WALLPAPER_SUPPORT_POP

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
#include "graphics_3d_rotate.h"
#include "graphics_draw.h"


/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

#define POP_NUM	(28)

#define POP_WINDOW_W MMI_MAINSCREEN_WIDTH
#define POP_WINDOW_H MMI_MAINSCREEN_HEIGHT

//x_edge = size.w/2*(10*size.w)/(0-eye_z);
#define POP_X_EDGE_MIN (-5*POP_WINDOW_W*POP_WINDOW_W/(2*POP_WINDOW_H))
//y_edge = size.h/2*(10*size.w)/(0-eye_z);//(10*size.w) is max z
#define POP_Y_EDGE_MIN (-5*POP_WINDOW_W/2)

#define POP_RANDOM_Z_MAX (10*POP_WINDOW_W)
#define POP_RANDOM_Z_MIN (2*POP_WINDOW_W)

#define POP_MOVE_Z_PARAM (40)//手势转化参数

//#define POW2_TABLE_MAXANGLE (900)
#define BLOCK_MEM_POOL_SIZE_POP (10 * 1024)   // Set a  MEM POOL for different livewallpaper respectively;

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         LOCAL DEFINITION                                 *
 **--------------------------------------------------------------------------*/
//LOCAL LIVEWALLPAPER_UILAYER_INFO_T s_pop_bg_layer_info_ptr[POP_NUM] ;
//LOCAL GUI_LCD_DEV_INFO s_pop_bg_layer_handle[POP_NUM]; //图片层
LOCAL int s_moved_z_axis = 0;
LOCAL MAGICPOP_INFO_T *s_MagicPop = PNULL;
LOCAL IMGREF_POINT_3D_T TPMove_oft = {0};//+ right, - left

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/
extern   int s2d_sin(int angle);
extern  int s2d_cos(int angle);

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                                                         */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : POP墙纸初始化
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN PopInit(void);
/*****************************************************************************/
//  Description : 泡泡初始化
//  Global resource dependence : 
//  Author:arvin.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN PopConstruct(MMI_WIN_ID_T win_id);

/*****************************************************************************/
//  Description : 泡泡准备背景数据
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN PopDrawBg(MMI_WIN_ID_T win_id,const GUI_LCD_DEV_INFO *dev_info_ptr);

/*****************************************************************************/
//  Description : 泡泡刷新
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL void PopCalculate(LW_CAL_PARAM *lw_cal_param);
/*****************************************************************************/
//  Description : 泡泡层释放
//  Global resource dependence : 
//  Author:销毁临时申请的变量
//  Note:
/*****************************************************************************/
PUBLIC void PopDestruct(void);
/*****************************************************************************/
//  Description : 获得动态壁纸类型
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL uint32 PopGetType(void);
/*****************************************************************************/
//  Description : 获得动态壁纸ThumbNail信息
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL void PopGetThumbNailInfo(LW_THUMBNAIL_INFO *thumbnail_info_ptr);
/*****************************************************************************/
//  Description : 泡泡交互事件处理
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN PopHandleEvent(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
/*****************************************************************************/
//  Description : 获得动态壁纸源数据宽高
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL void POPGetWidthAndHeight(uint16 *width_ptr,uint16 *height_ptr);

/*****************************************************************************/
//  Description : 获得动态壁纸选项列表
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL OPTIONLIST *POPGetOptionList(void);
/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/
PUBLIC const MMI_LIVE_WALLPAPER_ITEM_INFO_T g_pop_Instance =
{
    PopInit,
    PopConstruct,
    PopDrawBg,
    PopCalculate,
    PopDestruct,
    PopHandleEvent,
    PNULL,
    PopGetType,
    PopGetThumbNailInfo,
    POPGetWidthAndHeight,
    PNULL,
    PNULL,
    POPGetOptionList
};
/*---------------------------------------------------------------------------*/
/*                          FUNCTION DEFINITION                                                              */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : POP
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void Init_Pops(MAGICPOP_INFO_T* pop, IMGREF_SIZE_T size,IMGREF_SIZE_T bg_size)
{
	int32 eye_z  = 0;
	int32 pop_index  = 0,i = 0;
	int32 z_max = 0;
	int32 pop_y_edge_max = (5*POP_WINDOW_W/2+bg_size.h);
	
	eye_z = size.h*(-2);
	z_max = size.h+1;

pop_index = 0;

    pop[pop_index].radius = 165;// 0~45

    pop[pop_index].rot_point.z = POP_RANDOM_Z_MAX-800;
    pop[pop_index].rot_point.x = POP_X_EDGE_MIN+300;//(rand()%(pop_x_edge_max-POP_X_EDGE_MIN))+POP_X_EDGE_MIN;
    pop[pop_index].rot_point.y = POP_Y_EDGE_MIN + 450;
    	        
    pop[pop_index].rot_x = 200;
    pop[pop_index].rot_y = 120;

    pop[pop_index].top_radius = 70;
     pop[pop_index].st_radius = 62;

pop_index++;

    pop[pop_index].radius = 80;// 0~45
    pop[pop_index].rot_point.z = POP_RANDOM_Z_MAX-1200;
    pop[pop_index].rot_point.x = POP_X_EDGE_MIN;
	pop[pop_index].rot_point.x += 360;
    pop[pop_index].rot_point.y = POP_Y_EDGE_MIN + 500;
    	        
    pop[pop_index].rot_x = 210;
    pop[pop_index].rot_y = 130;

     pop[pop_index].top_radius = 50;
     pop[pop_index].st_radius = 30;

pop_index++;
    pop[pop_index].radius = 120;// 0~45
    pop[pop_index].rot_point.z = POP_RANDOM_Z_MAX-1000;
    pop[pop_index].rot_point.x = POP_X_EDGE_MIN+300;//(rand()%(pop_x_edge_max-POP_X_EDGE_MIN))+POP_X_EDGE_MIN;
    pop[pop_index].rot_point.y = POP_Y_EDGE_MIN +400;
    	        
    pop[pop_index].rot_x = 140;
    pop[pop_index].rot_y = 280;

     pop[pop_index].top_radius = 58;
     pop[pop_index].st_radius = 50;

pop_index++;
    pop[pop_index].radius = 85;// 0~45
    pop[pop_index].rot_point.z = POP_RANDOM_Z_MAX-1600;
    pop[pop_index].rot_point.x = POP_X_EDGE_MIN+450;//(rand()%(pop_x_edge_max-POP_X_EDGE_MIN))+POP_X_EDGE_MIN;
    pop[pop_index].rot_point.y = pop_y_edge_max - 600;
    	        
    pop[pop_index].rot_x = 100;
    pop[pop_index].rot_y = 90;

     pop[pop_index].top_radius = 75;
     pop[pop_index].st_radius = 70;

pop_index++;
    pop[pop_index].radius = 90;// 0~45
    pop[pop_index].rot_point.z = POP_RANDOM_Z_MAX-1000;
    pop[pop_index].rot_point.x = POP_X_EDGE_MIN+550;//(rand()%(pop_x_edge_max-POP_X_EDGE_MIN))+POP_X_EDGE_MIN;
    pop[pop_index].rot_point.y = pop_y_edge_max - 600;
    	        
    pop[pop_index].rot_x = 160;
    pop[pop_index].rot_y = 160;

     pop[pop_index].top_radius = 82;
     pop[pop_index].st_radius = 72;

pop_index++;
    pop[pop_index].radius = 140;// 0~45
    pop[pop_index].rot_point.z = POP_RANDOM_Z_MAX-600;
    pop[pop_index].rot_point.x = POP_X_EDGE_MIN+380;//(rand()%(pop_x_edge_max-POP_X_EDGE_MIN))+POP_X_EDGE_MIN;
    pop[pop_index].rot_point.y = pop_y_edge_max - 800;
    	        
    pop[pop_index].rot_x = 240;
    pop[pop_index].rot_y = 180;

     pop[pop_index].top_radius = 60;
     pop[pop_index].st_radius = 52;

pop_index++;
    pop[pop_index].radius = 170;// 0~45
    pop[pop_index].rot_point.z = POP_RANDOM_Z_MAX-500;
    pop[pop_index].rot_point.x = POP_X_EDGE_MIN+380;//(rand()%(pop_x_edge_max-POP_X_EDGE_MIN))+POP_X_EDGE_MIN;
    pop[pop_index].rot_point.y = pop_y_edge_max - 800;
    	        
    pop[pop_index].rot_x = 200;
    pop[pop_index].rot_y = 160;

     pop[pop_index].top_radius = 68;
     pop[pop_index].st_radius = 60;

pop_index++;

    pop[pop_index].radius = 120;// 0~45
    pop[pop_index].rot_point.z = POP_RANDOM_Z_MAX-600;
    pop[pop_index].rot_point.x = POP_X_EDGE_MIN+900;//(rand()%(pop_x_edge_max-POP_X_EDGE_MIN))+POP_X_EDGE_MIN;
    pop[pop_index].rot_point.y = pop_y_edge_max - 720;
    	        
    pop[pop_index].rot_x = 220;
    pop[pop_index].rot_y = 200;

     pop[pop_index].top_radius = 56;
     pop[pop_index].st_radius = 50;
	 
pop_index++;

    pop[pop_index].radius = 100;// 0~45
    pop[pop_index].rot_point.z = POP_RANDOM_Z_MAX-400;
    pop[pop_index].rot_point.x = POP_X_EDGE_MIN+760;//(rand()%(pop_x_edge_max-POP_X_EDGE_MIN))+POP_X_EDGE_MIN;
    pop[pop_index].rot_point.y = pop_y_edge_max - 620;
    	        
    pop[pop_index].rot_x = 200;
    pop[pop_index].rot_y = 320;

     pop[pop_index].top_radius = 55;
     pop[pop_index].st_radius = 48;

pop_index++;

    pop[pop_index].radius = 70;// 0~45
    pop[pop_index].rot_point.z = POP_RANDOM_Z_MAX-1800;
    pop[pop_index].rot_point.x = POP_X_EDGE_MIN+750;//(rand()%(pop_x_edge_max-POP_X_EDGE_MIN))+POP_X_EDGE_MIN;
    pop[pop_index].rot_point.y = pop_y_edge_max - 600;
    	        
    pop[pop_index].rot_x = 100;
    pop[pop_index].rot_y = 100;

     pop[pop_index].top_radius = 56;
     pop[pop_index].st_radius = 46;

//白色泡泡
pop_index++;
    pop[pop_index].radius = 100;// 0~45
    pop[pop_index].rot_point.z = POP_RANDOM_Z_MAX-1000;
    pop[pop_index].rot_point.x = POP_X_EDGE_MIN+450;//(rand()%(pop_x_edge_max-POP_X_EDGE_MIN))+POP_X_EDGE_MIN;
    pop[pop_index].rot_point.y = pop_y_edge_max - 700;
    	        
    pop[pop_index].rot_x = 160;
    pop[pop_index].rot_y = 160;

     pop[pop_index].top_radius = 60;
     pop[pop_index].st_radius = 52;

pop_index++;
    pop[pop_index].radius = 150;// 0~45
    pop[pop_index].rot_point.z = POP_RANDOM_Z_MAX-1300;
    pop[pop_index].rot_point.x = POP_X_EDGE_MIN+650;//(rand()%(pop_x_edge_max-POP_X_EDGE_MIN))+POP_X_EDGE_MIN;
    pop[pop_index].rot_point.y = pop_y_edge_max - 900;
    	        
    pop[pop_index].rot_x = 200;
    pop[pop_index].rot_y = 160;

     pop[pop_index].top_radius = 60;
     pop[pop_index].st_radius = 56;

pop_index++;
    pop[pop_index].radius = 120;// 0~45
    pop[pop_index].rot_point.z = POP_RANDOM_Z_MAX-600;
    pop[pop_index].rot_point.x = POP_X_EDGE_MIN+550;//(rand()%(pop_x_edge_max-POP_X_EDGE_MIN))+POP_X_EDGE_MIN;
    pop[pop_index].rot_point.y = pop_y_edge_max - 800;
    	        
    pop[pop_index].rot_x = 130;
    pop[pop_index].rot_y = 140;

     pop[pop_index].top_radius = 70;
     pop[pop_index].st_radius = 65;
    
//绿色
pop_index++;
 
    pop[pop_index].radius = 100;// 0~45
    pop[pop_index].rot_point.z = POP_RANDOM_Z_MAX-1000;
    pop[pop_index].rot_point.x = POP_X_EDGE_MIN+880;//(rand()%(pop_x_edge_max-POP_X_EDGE_MIN))+POP_X_EDGE_MIN;
    pop[pop_index].rot_point.y = POP_Y_EDGE_MIN + 350;
    	        
    pop[pop_index].rot_x = 120;
    pop[pop_index].rot_y = 100;

     pop[pop_index].top_radius = 60;
     pop[pop_index].st_radius = 56;

pop_index++;

    pop[pop_index].radius = 120;// 0~45
    pop[pop_index].rot_point.z = POP_RANDOM_Z_MAX-1200;
    pop[pop_index].rot_point.x = POP_X_EDGE_MIN+1080;//(rand()%(pop_x_edge_max-POP_X_EDGE_MIN))+POP_X_EDGE_MIN;
    pop[pop_index].rot_point.y = POP_Y_EDGE_MIN + 450;
    	        
    pop[pop_index].rot_x = 140;
    pop[pop_index].rot_y = 120;

     pop[pop_index].top_radius = 65;
     pop[pop_index].st_radius = 60;

pop_index++;
    pop[pop_index].radius = 120;// 0~45
    pop[pop_index].rot_point.z = POP_RANDOM_Z_MAX-600;
    pop[pop_index].rot_point.x = POP_X_EDGE_MIN+1070;//(rand()%(pop_x_edge_max-POP_X_EDGE_MIN))+POP_X_EDGE_MIN;
    pop[pop_index].rot_point.y = POP_Y_EDGE_MIN + 400;
    	        
    pop[pop_index].rot_x = 100;
    pop[pop_index].rot_y = 110;

     pop[pop_index].top_radius = 72;
     pop[pop_index].st_radius = 60;

//黄色
pop_index++;
    pop[pop_index].radius = 160;// 0~45
    pop[pop_index].rot_point.z = POP_RANDOM_Z_MAX-900;
    pop[pop_index].rot_point.x = POP_X_EDGE_MIN+1800;//(rand()%(pop_x_edge_max-POP_X_EDGE_MIN))+POP_X_EDGE_MIN;
    pop[pop_index].rot_point.y = POP_Y_EDGE_MIN + 400;
    	        
    pop[pop_index].rot_x = 220;
    pop[pop_index].rot_y = 180;

     pop[pop_index].top_radius = 75;
     pop[pop_index].st_radius = 69;

pop_index++;
    pop[pop_index].radius = 90;// 0~45
    pop[pop_index].rot_point.z = POP_RANDOM_Z_MAX-1300;
    pop[pop_index].rot_point.x = POP_X_EDGE_MIN+1600;//(rand()%(pop_x_edge_max-POP_X_EDGE_MIN))+POP_X_EDGE_MIN;
    pop[pop_index].rot_point.y = POP_Y_EDGE_MIN + 700;
    	        
    pop[pop_index].rot_x = 150;
    pop[pop_index].rot_y = 160;

     pop[pop_index].top_radius = 68;
     pop[pop_index].st_radius = 60;

pop_index++;
    pop[pop_index].radius = 160;// 0~45
    pop[pop_index].rot_point.z = POP_RANDOM_Z_MAX-400;
    pop[pop_index].rot_point.x = POP_X_EDGE_MIN+1350;//(rand()%(pop_x_edge_max-POP_X_EDGE_MIN))+POP_X_EDGE_MIN;
    pop[pop_index].rot_point.y = POP_Y_EDGE_MIN +500;
    	        
    pop[pop_index].rot_x = 140;
    pop[pop_index].rot_y = 150;

     pop[pop_index].top_radius = 70;
     pop[pop_index].st_radius = 68;

//白色

pop_index++;
    pop[pop_index].radius = 132;// 0~45
    pop[pop_index].rot_point.z = POP_RANDOM_Z_MAX-1200;
    pop[pop_index].rot_point.x = POP_X_EDGE_MIN+1100;//(rand()%(pop_x_edge_max-POP_X_EDGE_MIN))+POP_X_EDGE_MIN;
    pop[pop_index].rot_point.y = POP_Y_EDGE_MIN +750;
    	        
    pop[pop_index].rot_x = 90;
    pop[pop_index].rot_y = 120;

     pop[pop_index].top_radius = 85;
     pop[pop_index].st_radius = 80;
    
//红色
pop_index++;
    pop[pop_index].radius = 70;// 0~45
    pop[pop_index].rot_point.z = POP_RANDOM_Z_MAX-400;
    pop[pop_index].rot_point.x = POP_X_EDGE_MIN+1200;//(rand()%(pop_x_edge_max-POP_X_EDGE_MIN))+POP_X_EDGE_MIN;
    pop[pop_index].rot_point.y = POP_Y_EDGE_MIN +750;
    	        
    pop[pop_index].rot_x = 90;
    pop[pop_index].rot_y = 100;

     pop[pop_index].top_radius = 70;
     pop[pop_index].st_radius = 62;

pop_index++;
    pop[pop_index].radius = 60;// 0~45
    pop[pop_index].rot_point.z = POP_RANDOM_Z_MAX-200;
    pop[pop_index].rot_point.x = POP_X_EDGE_MIN+1250;//(rand()%(pop_x_edge_max-POP_X_EDGE_MIN))+POP_X_EDGE_MIN;
    pop[pop_index].rot_point.y = POP_Y_EDGE_MIN +700;
    	        
    pop[pop_index].rot_x = 95;
    pop[pop_index].rot_y = 110;

     pop[pop_index].top_radius = 70;
     pop[pop_index].st_radius = 66;

pop_index++;
    pop[pop_index].radius = 90;// 0~45
    pop[pop_index].rot_point.z = POP_RANDOM_Z_MAX-750;
    pop[pop_index].rot_point.x = POP_X_EDGE_MIN+1350;//(rand()%(pop_x_edge_max-POP_X_EDGE_MIN))+POP_X_EDGE_MIN;
    pop[pop_index].rot_point.y = POP_Y_EDGE_MIN +800;
    	        
    pop[pop_index].rot_x = 155;
    pop[pop_index].rot_y = 140;

     pop[pop_index].top_radius = 72;
     pop[pop_index].st_radius = 68;

pop_index++;
    pop[pop_index].radius = 100;// 0~45
    pop[pop_index].rot_point.z = POP_RANDOM_Z_MAX-1318;
    pop[pop_index].rot_point.x = POP_X_EDGE_MIN+1300;//(rand()%(pop_x_edge_max-POP_X_EDGE_MIN))+POP_X_EDGE_MIN;
    pop[pop_index].rot_point.y = POP_Y_EDGE_MIN +1000;
    	        
    pop[pop_index].rot_x = 220;
    pop[pop_index].rot_y = 80;

     pop[pop_index].top_radius = 70;
     pop[pop_index].st_radius = 65;
	 
pop_index++;
    pop[pop_index].radius = 100;// 0~45
    pop[pop_index].rot_point.z = POP_RANDOM_Z_MAX-1500;
    pop[pop_index].rot_point.x = POP_X_EDGE_MIN+1500;//(rand()%(pop_x_edge_max-POP_X_EDGE_MIN))+POP_X_EDGE_MIN;
    pop[pop_index].rot_point.y = POP_Y_EDGE_MIN +950;
    	        
    pop[pop_index].rot_x = 200;
    pop[pop_index].rot_y = 120;

     pop[pop_index].top_radius = 70;
     pop[pop_index].st_radius = 67;

pop_index++;

    pop[pop_index].radius = 100;// 0~45
    pop[pop_index].rot_point.z = POP_RANDOM_Z_MAX-900;
    pop[pop_index].rot_point.x = POP_X_EDGE_MIN+1600;//(rand()%(pop_x_edge_max-POP_X_EDGE_MIN))+POP_X_EDGE_MIN;
    pop[pop_index].rot_point.y = POP_Y_EDGE_MIN +900;
    	        
    pop[pop_index].rot_x = 170;
    pop[pop_index].rot_y = 150;

     pop[pop_index].top_radius = 75;
     pop[pop_index].st_radius = 66;

pop_index++;
    pop[pop_index].radius = 90;// 0~45
    pop[pop_index].rot_point.z = POP_RANDOM_Z_MAX-900;
    pop[pop_index].rot_point.x = POP_X_EDGE_MIN+1800;//(rand()%(pop_x_edge_max-POP_X_EDGE_MIN))+POP_X_EDGE_MIN;
    pop[pop_index].rot_point.y = POP_Y_EDGE_MIN +1000;
    	        
    pop[pop_index].rot_x = 190;
    pop[pop_index].rot_y = 200;

     pop[pop_index].top_radius = 60;
     pop[pop_index].st_radius = 48;

pop_index++;
    pop[pop_index].radius = 80;// 0~45
    pop[pop_index].rot_point.z = POP_RANDOM_Z_MAX-600;
    pop[pop_index].rot_point.x = POP_X_EDGE_MIN+1850;//(rand()%(pop_x_edge_max-POP_X_EDGE_MIN))+POP_X_EDGE_MIN;
    pop[pop_index].rot_point.y = POP_Y_EDGE_MIN +1050;
    	        
    pop[pop_index].rot_x = 170;
    pop[pop_index].rot_y = 170;

     pop[pop_index].top_radius = 60;
     pop[pop_index].st_radius = 52;


    for(i = 0; i < POP_NUM; ++i)
    {
		pop[i].angle = rand()%3600;
		pop[i].direction = (rand()%2)*2-1;// 1 or -1
		pop[i].vel_angle = 8+rand()%6;
		pop[i].radius = pop[i].radius*10/8;
		pop[i].color = 0x48ffffff;
     //pop[i].top_radius = pop[i].top_radius*256/256;
     //pop[i].st_radius = pop[i].st_radius*256/256; 
    }
    
}
/*****************************************************************************/
//  Description : POP墙纸初始化
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN PopInit(void)
{
    BOOLEAN b_result = TRUE;

    return b_result;
}
/*****************************************************************************/
//  Description : 泡泡初始化
//  Global resource dependence : 
//  Author:arvin.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN PopConstruct(MMI_WIN_ID_T win_id)
{
    BOOLEAN b_result = TRUE;
    IMGREF_SIZE_T window = {0};
    IMGREF_SIZE_T bg_size = {0};
    
    if(PNULL != s_MagicPop)
    {
        SCI_TRACE_LOW("PopConstruct, have a exception before Construct: PNULL != s_MagicPop ");
        MMILIVEWALLPAPER_FREE(s_MagicPop);
        s_MagicPop = PNULL;
    }
    s_MagicPop = MMILIVEWALLPAPER_ALLOC((sizeof(MAGICPOP_INFO_T))*POP_NUM);
    if(PNULL == s_MagicPop)
    {
        return FALSE;
    }
        
    window.h = MMI_MAINSCREEN_HEIGHT;
    window.w = MMI_MAINSCREEN_WIDTH;

    POPGetWidthAndHeight(&bg_size.w,&bg_size.h);

    Init_Pops(s_MagicPop,window,bg_size);

    return b_result;
}
/*****************************************************************************/
//  Description : 泡泡准备背景数据
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN PopDrawBg(MMI_WIN_ID_T win_id,const GUI_LCD_DEV_INFO *dev_info_ptr)
{
    BOOLEAN b_result = FALSE;
    GUI_POINT_T display_point = {0,0};

        if (PNULL != dev_info_ptr)
        {
            b_result = GUIRES_DisplayImg(&display_point, 
        PNULL, 
        PNULL, 
        win_id, 
        IMAGE_WALLPAPER_POP_BG, 
        dev_info_ptr);
        }
        return b_result;
}
/*****************************************************************************/
//  Description : POP
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void UpdateLiveWallpaperPop(MAGICPOP_INFO_T* Pop, COLOR16* pInOutColor,IMGREF_SIZE_T size)
{
	IMGREF_3D_POINT_T sEye = {0};
	//IMGREF_3D_POINT_T in_Point = {0};
	IMGREF_POINT_3D_T pos_3d = {0};
	FIX16_POINT_T pos_inPic = {0};
	int32 radius_inPic = 0;
	int32 Cos_st_ang = 0;

	FIX16_3D_POINT_T in_Point1 = {0};
	FIX16_3D_POINT_T sEye1 = {0};
	FIX16_3D_POINT_T pos_inPic1 = {0};

    int32 top_radius = 0, st_radius = 0;
	


	sEye.x = size.w/2;
	sEye.y = size.h/2;
	sEye.z = (-2)*size.h;
	
//TP left means pos right, right means left. so minus TPMove_offset
	pos_3d.x = Pop->rot_point.x - TPMove_oft.x;//pStar->now_point.x;//
	pos_3d.y = Pop->rot_point.y - TPMove_oft.y;//pStar->now_point.y; //size.h/2;
	pos_3d.z = Pop->rot_point.z + TPMove_oft.z;//100 + TPMove_offset.z;

	if(pos_3d.z <= sEye.z)//物体在眼睛后面(平行)是看不到的
		return;
	radius_inPic = (((0-sEye.z)*Pop->radius)<<4)/(pos_3d.z - sEye.z);			
		
	Pop->angle = Pop->angle%3600;

	in_Point1.x = (pos_3d.x<<16) + (Pop->rot_x)*s2d_sin(Pop->angle);
	in_Point1.y = (pos_3d.y<<16) + (Pop->rot_y)*s2d_cos(Pop->angle);
	in_Point1.z = (pos_3d.z<<16);

	sEye1.x = ((size.w/2)<<16);
	sEye1.y = ((size.h/2)<<16);
	sEye1.z = (((-2)*size.h)<<16);

	s3d_Free_Point_change_2(in_Point1,sEye1,&pos_inPic1);

	pos_inPic.x = (pos_inPic1.x>>16);
	pos_inPic.x_dec = (pos_inPic1.x&0xffff);
	pos_inPic.y = (pos_inPic1.y>>16);
	pos_inPic.y_dec = (pos_inPic1.y&0xffff);

	//MagicPop(size,&pos_inPic,radius_inPic,radius_inPic*14/20,radius_inPic*26/40,Pop->color,pInOutColor);

	//这是cos开始起作用的角度，这里的比例和下面的两个比例参数是对应的，900是角度90度
	//需要增加这个参数的原因是精度问题，
	//radius_inPic*8/10和radius_inPic*15/20同样可以算出这个角度，可在函数里面计算会出现精度引起的振荡，
	//画面的体现就是泡泡闪

    top_radius = Pop->top_radius;//70;
    st_radius = Pop->st_radius;//62;
	Cos_st_ang = ((st_radius*st_radius-top_radius*top_radius)*900)/(100*100-top_radius*top_radius);
	
	MagicPop(size,&pos_inPic,radius_inPic,radius_inPic*top_radius/100,radius_inPic*st_radius/100,Cos_st_ang,Pop->color,pInOutColor);

	Pop->angle += Pop->direction*Pop->vel_angle;
}
//#define RGBA888_TO_RGB565(_ucolor32)  ((((_ucolor32>>24)*((_ucolor32>>16)&0xff)) & 0xf800)| ((((_ucolor32>>24)*(_ucolor32&0xff00))>>13) & 0x07e0) | ((((_ucolor32>>24)*((_ucolor32)&0xff))>>11) & 0x1f))

/*****************************************************************************/
//  Description : 泡泡刷新
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL void PopCalculate(LW_CAL_PARAM *lw_cal_param)
{
	IMGREF_SIZE_T size = {0,0};
	//IMGREF_SIZE_T pop_up_window = {0,0};
	//FIX16_POINT_T offset = {0};

	uint32 pop_index = 0;
	int32 i = 0, j = 0; //无意义 sort_i = 0;

	int32 z_sort[POP_NUM] = {0};
	int32 z_sort_index[POP_NUM] = {0};
	//int32 min_z = 0x7fffffff;
	BOOLEAN isChanged = TRUE;//TRUE,FALSE
	int32 iLen = POP_NUM;
	int32 swap_temp = 0;

	//static int32 s_sum = 0;
	//
	
	IMGREF_SIZE_T Part_img = {0};

	//COLOR32 *target_buf_cpy = PNULL;
	
	IMGREF_POINT_T carve_img = {0};
	int32 z_max = 0, eye_z = 0;
	//int32 pageNum = 0;

	uint16 *target_buf_ptr = PNULL;
	uint16 scr_width = 0; 
	uint16 scr_height = 0;
	uint16*source_buf_ptr = PNULL;
	int32 page_total_move = 0;
	int32 bg_total_move = 0;

	FIX16_POINT_T ST_offS = {0};// FOR PIC SCALE

	SCALE_IMAGE_IN_T scale_in = {0};
    SCALE_IMAGE_OUT_T scale_out = {0};

    if(PNULL == lw_cal_param->dest_buf_info.buf_ptr || PNULL == lw_cal_param->src_buf_info.buf_ptr)
    {
        return;
    }
	if(0==lw_cal_param->dest_buf_info.w||0==lw_cal_param->dest_buf_info.h)
	{
		return;
	}
	target_buf_ptr = lw_cal_param->dest_buf_info.buf_ptr;
	scr_width = lw_cal_param->src_buf_info.w; 
	scr_height = lw_cal_param->src_buf_info.h;
	source_buf_ptr = lw_cal_param->src_buf_info.buf_ptr;
	size.w = lw_cal_param->dest_buf_info.w;
	size.h = lw_cal_param->dest_buf_info.h;
	//SCI_TRACE_LOW:"[POP_POS]---------x = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_LIVEWALLPAPER_POP_862_112_2_18_2_32_16_27,(uint8*)"d",lw_cal_param->fg_pos_info.x);
	ST_offS.x = PIC_OFFS_WIDTH;
	ST_offS.y = (scr_height-size.h)/2;
	
    page_total_move = (lw_cal_param->fg_pos_info.rc.right - lw_cal_param->fg_pos_info.rc.left + 1-MMI_MAINSCREEN_WIDTH);
	bg_total_move = scr_width - MMI_MAINSCREEN_WIDTH-2*ST_offS.x;

	if(page_total_move<0)
	{
		return;
	}
	else if(0==page_total_move)
	{
		TPMove_oft.x = 0;
	}
	else
	{
		TPMove_oft.x = lw_cal_param->fg_pos_info.x*bg_total_move/page_total_move;
	}
	//SCI_TRACE_LOW:"[LIVE_WALLPAPER]---------fg_pos_info.x = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_LIVEWALLPAPER_POP_881_112_2_18_2_32_16_28,(uint8*)"d",lw_cal_param->fg_pos_info.x);
		//SCI_TRACE_LOW:"[LIVE_WALLPAPER]---------TPMove_oft.x = %d"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_LIVEWALLPAPER_POP_882_112_2_18_2_32_16_29,(uint8*)"d",TPMove_oft.x);
	if(TPMove_oft.x<0)
		TPMove_oft.x = 0;

	TPMove_oft.y = lw_cal_param->fg_pos_info.y;
	TPMove_oft.w = 0;
	TPMove_oft.z = s_moved_z_axis;
	
	eye_z = (-2)*size.h;	

    z_max = LIVEWALLPAPER_Z_MAX;

	//memset(target_buf_ptr,0x0,sizeof(COLOR16)*scr_width*scr_height);
    // SCI_MEMCPY(target_buf_ptr,source_buf_ptr,sizeof(uint16)*scr_width*scr_height);




Part_img.w = size.w*(z_max+TPMove_oft.z-eye_z)/(z_max-eye_z);
Part_img.h = size.h*(z_max+TPMove_oft.z-eye_z)/(z_max-eye_z);
//始终扣除中心位置的部分用于显示
/*carve_img.x = MIN( size.w/2-Part_img.w/2 - TPMove_oft.x,scr_width - Part_img.w);
carve_img.x = MAX(carve_img.x,0);
carve_img.y = MIN(size.h/2-Part_img.h/2 - TPMove_oft.y,scr_height-Part_img.h);
carve_img.y = MAX(carve_img.y,0);
*/

	//carve_img.x = size.w/2-Part_img.w/2- TPMove_offset.x*(bg_w-size.w)/((pageNum-1)*size.w);
	carve_img.x = MIN( size.w/2-Part_img.w/2 + TPMove_oft.x+ST_offS.x,scr_width - Part_img.w);
	carve_img.x = MAX(carve_img.x,0);


//carve_img.x = MIN( size.w/2-Part_img.w/2 - TPMove_offset.x,bg_w - Part_img.w);
//carve_img.x = MAX(carve_img.x,0);
carve_img.y = MIN(size.h/2-Part_img.h/2 + TPMove_oft.y+ST_offS.y,scr_height-Part_img.h);
carve_img.y = MAX(carve_img.y,0);



//target_buf_cpy = (COLOR16 *)SCI_ALLOCA(Part_img.w*Part_img.h*sizeof(COLOR32));

    scale_in.src_size.w = scr_width;
	scale_in.src_size.h = scr_height;
	scale_in.src_trim_rect.x = carve_img.x & 0xfffc;
	scale_in.src_trim_rect.y = carve_img.y & 0xfffc;
	scale_in.src_trim_rect.w = Part_img.w  & 0xfffc;
	scale_in.src_trim_rect.h = Part_img.h  & 0xfffc;

	scale_in.src_format = IMGREF_FORMAT_RGB565;
	scale_in.src_chn.chn0.ptr = source_buf_ptr;
	scale_in.src_chn.chn0.size = scale_in.src_size.w*scale_in.src_size.h*(sizeof(COLOR16));

	scale_in.target_size.w = size.w;
	scale_in.target_size.h = size.h;
	scale_in.target_format = IMGREF_FORMAT_RGB565;
	scale_in.target_buf.ptr = target_buf_ptr;
	scale_in.target_buf.size = scale_in.target_size.w*scale_in.target_size.h*(sizeof(COLOR16));

	scale_in.scale_mode = SCALE_MODE_DISTORT;
/*	scale_out.output_format = IMGREF_FORMAT_RGB565;
	scale_out.output_size.h = size.h;
	scale_out.output_size.w = size.w;
	scale_out.output_chn.chn0.ptr = target_buf_ptr;
	scale_out.output_chn.chn0.size = scale_out.output_size.h*scale_out.output_size.w*(sizeof(COLOR16));
*/
	if(SCI_SUCCESS != GRAPH_ScaleImage(&scale_in,&scale_out))
	    return;

/*
scale_in.src_size.w = scr_width;
scale_in.src_size.h = scr_height;
scale_in.src_trim_rect.x = carve_img.x;
scale_in.src_trim_rect.y = carve_img.y;
scale_in.src_trim_rect.w = Part_img.w;
scale_in.src_trim_rect.h = Part_img.h;
scale_in.src_format = IMGREF_FORMAT_ARGB888;
scale_in.src_chn.chn0.ptr = ((uint32*)s_pop_bg_layer_info_ptr[0].buf_addr);
scale_in.src_chn.chn0.size = scale_in.src_size.w*scale_in.src_size.h*(sizeof(COLOR32));
scale_in.target_size.w = size.w;
scale_in.target_size.h = size.h;
scale_in.target_format = IMGREF_FORMAT_RGB565;
scale_in.target_buf.ptr = target_buf_ptr;
scale_in.target_buf.size = scale_in.target_size.w*scale_in.target_size.h*(sizeof(COLOR16));

scale_out.output_format = IMGREF_FORMAT_RGB565;
scale_out.output_size.h = size.h;
scale_out.output_size.w = size.w;
scale_out.output_chn.chn0.ptr = target_buf_ptr;
scale_out.output_chn.chn0.size = scale_out.output_size.h*scale_out.output_size.w*(sizeof(COLOR16));

if(!GRAPH_ScaleImage(&scale_in,&scale_out))
    return;*/

/*for(j = 0; j <size.h; ++j)
{
	for(i = 0; i <size.w; ++i)
	{
		target_buf_ptr[j*size.w+i] = RGBA888_TO_RGB565(((uint32*)s_pop_bg_layer_info_ptr[0].buf_addr)[(j)*scr_width+(i)]);
	}
}
*/

	 ///************update*************///
	 
	//z轴排序
    //sort_i = 0;
    if(PNULL == s_MagicPop)
    {
        return;
    }
      for( i = 0; i < POP_NUM ; i++)
	{
		//update_randStars(&g_aStar[i], target_buf_ptr, size);
		z_sort[i] = s_MagicPop[i].rot_point.z;
	}

	for( i = 0 ; i < POP_NUM ; ++i)
	{
		z_sort_index[i] = i;
	}

	isChanged = TRUE;
	for( i = 0 ; i < iLen-1 ; i ++)
	{
		if(isChanged)
		{
			isChanged = FALSE;
			for( j = 1 ; j < iLen - i ; j ++)
			{
				if(z_sort[j-1]>=z_sort[j])
				{
					
				}
				else
				{
					isChanged = TRUE;
					
					swap_temp = z_sort[j-1];
					z_sort[j-1] = z_sort[j];
					z_sort[j] = swap_temp;

					swap_temp = z_sort_index[j-1];
					z_sort_index[j-1] = z_sort_index[j];
					z_sort_index[j] = swap_temp;
				}
			}
		}
	}
//UpdateLiveWallpaperPop(&s_MagicPop[z_sort_index[3]],target_buf_ptr,size);
	for( pop_index = 0 ; pop_index < POP_NUM ; pop_index += 3)
	{
		UpdateLiveWallpaperPop(&s_MagicPop[z_sort_index[pop_index]],target_buf_ptr,size);
	}
//S2D_Blending(size,target_buf_ptr,Part_img,offset,target_buf_cpy,target_buf_ptr);
	
    //SCI_Free(target_buf_cpy);
}

/*****************************************************************************/
//  Description : 泡泡层释放
//  Global resource dependence : 
//  Author:销毁临时申请的变量
//  Note:
/*****************************************************************************/
PUBLIC void PopDestruct(void)
{
    if(PNULL != s_MagicPop)
    {
        MMILIVEWALLPAPER_FREE(s_MagicPop);
            s_MagicPop = PNULL;
    }
}

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
/*****************************************************************************/
//  Description : 泡泡手势消息处理
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void Pop_HandleTpGesture(DPARAM param)
{	
	if (MMI_GESTURE_ZOOM_IN == MMK_GET_TP_GESTURE(param))//收缩
	{
		s_moved_z_axis += POP_MOVE_Z_PARAM;
	}
	else if(MMI_GESTURE_ZOOM_OUT == MMK_GET_TP_GESTURE(param))//伸展
	{
		s_moved_z_axis -= POP_MOVE_Z_PARAM;
	}
	else
	{

	}
	
	s_moved_z_axis = MIN(s_moved_z_axis,Z_DELTA_MAX);

	s_moved_z_axis = MAX(s_moved_z_axis,Z_ZOOMIN_MAX);
}
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

/*****************************************************************************/
//  Description : 泡泡交互事件处理
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN PopHandleEvent(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{ 
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527

    switch(msg_id)
    {
		case MSG_TP_GESTURE://手势处理 拉近  放缩
			Pop_HandleTpGesture(param);
			break;

	    default:
	        return FALSE;
    }
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    return TRUE;
}
//*****************************************************************************/
//  Description : 获得动态壁纸类型
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL uint32 PopGetType(void)
{
    return MMI_LIVE_WALLPAPER_TYPE_BUBLE;
}
/*****************************************************************************/
//  Description : 获得动态壁纸ThumbNail信息
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL void PopGetThumbNailInfo(LW_THUMBNAIL_INFO *thumbnail_info_ptr)
{
    if(thumbnail_info_ptr)
    {
        thumbnail_info_ptr->is_image_file = FALSE;
        thumbnail_info_ptr->image_id = IMAGE_WALLPAPER_POP_THUMBNAIL;

        thumbnail_info_ptr->is_not_string = FALSE;
        thumbnail_info_ptr->text_id = TXT_SET_WALLPAPER_POP;
        thumbnail_info_ptr->instance_mem_pool_size = BLOCK_MEM_POOL_SIZE_POP;
    }
}
/*****************************************************************************/
//  Description : 获得动态壁纸源数据宽高
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL void POPGetWidthAndHeight(uint16 *width_ptr,uint16 *height_ptr)
{
    if(PNULL == width_ptr ||PNULL == height_ptr )
    {
        return;
    }
	
    GUIRES_GetImgWidthHeight((uint16*)width_ptr, (uint16*)height_ptr,IMAGE_WALLPAPER_POP_BG,MMK_GetFocusWinId());
}
/*****************************************************************************/
//  Description : 获得动态壁纸选项列表
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL OPTIONLIST *POPGetOptionList(void)
{
    return (OPTIONLIST*)PNULL;
} 
#endif// LIVE_WALLPAPER_SUPPORT_POP


#endif //_MMI_LIVEWALLPAPER_POP_C_


/*Edit by script, ignore 1 case. Thu Apr 26 19:00:58 2012*/ //IGNORE9527
