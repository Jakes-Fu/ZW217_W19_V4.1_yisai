

/*****************************************************************************
** File Name:      mmi_livewallpaper_aquarium.c                                    *
** Author:         Ben.LIU                                                       *
** Date:           13/09/2011                                                *
** Copyright:      2011 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe runninglight wallpaer       *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 09/2011       Ben.LIU          Modify                                  *
******************************************************************************/
#ifndef _MMI_LIVEWALLPAPER_AQUARIUM_C_
#define _MMI_LIVEWALLPAPER_AQUARIUM_C_

#include "mmi_app_livewallpaper_trc.h"
#ifdef LIVE_WALLPAPER_SUPPORT_AQUARIUM

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
#define OCEAN_BG_WIDTH  MMI_MAINSCREEN_WIDTH//320
#define OCEAN_BG_HEIGHT MMI_MAINSCREEN_HEIGHT//480

#define BUBBLE_NUM 40
#define SUNSHINE_NUM 7
#define FISH_NUM 8
#define FOOD_NUM 8

#define BUBBLE_X_WINDOW 15
#define BUBBLE_Y_WINDOW 30

#define FOOD_X_WINDOW 50
#define FOOD_Y_WINDOW 30

#define FISH_SIZE_MAX 80
#ifdef LOW_MEMORY_SUPPORT
#define BLOCK_MEM_POOL_SIZE_AQUARIUM (500 * 1024)   // Set a  MEM POOL for different livewallpaper respectively;
#else
#define BLOCK_MEM_POOL_SIZE_AQUARIUM (1000 * 1024)   // Set a  MEM POOL for different livewallpaper respectively;
#endif

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/
typedef enum
{
	OTE_NULL=0,
	FISH_1,
	FISH_1_2,
	FISH_1_3,
	FISH_1_4,
	FISH_2,
	FISH_2_2,
	FISH_2_3,
	FISH_2_4,
	FISH_3,
	FISH_3_2,
	FISH_3_3,
	FISH_3_4,
	FISH_4,
	FISH_4_2,
	FISH_4_3,
	FISH_4_4,
	SUNSHINE_H240_T40,
	SUNSHINE_H240_T60,
	SUNSHINE_H240_T90,
#ifndef LOW_MEMORY_SUPPORT
	SUNSHINE_H290_T40,
	SUNSHINE_H290_T60,
	SUNSHINE_H290_T90,
	SUNSHINE_H314_T40,
	SUNSHINE_H314_T60,
	SUNSHINE_H314_T90,
	SUNSHINE_H350_T40,
	SUNSHINE_H350_T60,
	SUNSHINE_H350_T90,
#endif
	BUBBLE_1,
	BUBBLE_2,
	BUBBLE_3,
	BUBBLE_4,
	FOOD,
	OCEAN_TYPE_MAX
}OCEAN_TYPE_E;

typedef struct
{
	COLOR32* srcARGB_Map;
	IMGREF_SIZE_T up_window;
}OCEAN_OBJ_T;

typedef struct
{
	int pos_x;
	int pos_y;
	BOOLEAN IsVisible;
	OCEAN_TYPE_E	bubble_type;
}OCEAN_BUBBLE_T;

typedef struct
{
	int pos_x;
	int pos_y;
	int pos_y_end;
	BOOLEAN IsVisible;
	OCEAN_TYPE_E	food_type;
}OCEAN_FOOD_T;

typedef struct
{
	int posX;
	int posY;
	int Velocity;
	OCEAN_TYPE_E	sunshine_type;
}OCEAN_SUNSHINE_T;

typedef struct
{
	/*int posX;
	int posY;*/
	FIX16_POINT_T	offset;
	OCEAN_TYPE_E     State;
	int32 Velocity;
	int32 pre_angle;
	int32 angleStep;
	BOOLEAN doAngleCtrl;
	int32 EndPos_Y_min;
	int32 EndPos_Y_max;
	int32 EndPos_X_min;
	int32 EndPos_X_max;
	int32 HitFrame;
	int32 HitFrameMAX;
	int32 EscapeDist;
	FIX16_POINT_T	base_Damp_offset;
	IMGREF_POINT_T endPos;
	OCEAN_TYPE_E	fish_type;
}OCEAN_FISH_T;
/**--------------------------------------------------------------------------*
 **                         LOCAL DEFINITION                                 *
 **--------------------------------------------------------------------------*/
    LOCAL OCEAN_OBJ_T *s_ocean_obj = PNULL;
    LOCAL OCEAN_BUBBLE_T *s_ocean_bubble = PNULL;
    LOCAL OCEAN_SUNSHINE_T *s_ocean_sunshine = PNULL;
    LOCAL OCEAN_FISH_T *s_ocean_fish = PNULL;
    LOCAL OCEAN_FOOD_T *s_ocean_food = PNULL;

    static int32 bubble_y = 0;// it is necessary;
    static BOOLEAN BubblingState = SCI_FALSE;

    uint16 *bg_rgb565_buf_ptr = PNULL;
 //   uint16 *bg_merged_buf_ptr = PNULL;
    LOCAL FIX16_POINT_T *s_Point_Add_ptr= PNULL;
//    OCEAN_OBJ_T ocean_obj_buf;
    LOCAL COLOR32* Fish_srcARGB_temp = PNULL;
	
    BOOLEAN FoodState = SCI_TRUE;
    uint16 food_x = 0,food_y = 0;
	
    LOCAL const uint16 atan_table[]=
    {
	0,8,17,26,35,44,53,62,71,80,88,97,106,114,123,131,140,
		148,157,165,173,181,189,197,205,213,221,228,236,
		243,251,258,265,272,279,286,293,300,306,313,320,
		326,332,338,345,351,357,362,368,374,379,385,390,
		396,401,406,411,416,421,426,431,436,440,445,450,
		454,458,463,467,471,475,479,483,487,491,495,498,
		502,506,509,513,516,520,523,526,530,533,536,539,
		542,545,548,551,554,557,560,563,565,568,571,573,
		576,578,581,583,586,588,591,593,595,598,600,602,
		604,606,609,611,613,615,617,619,621,623,625,627,
		628,630,632,634,636,637,639,641,643,644,646,647,
		649,651,652,654,655,657,658,660,661,663,664,666,
		667,668,670,671,673,674,675,676,678,679,680,681,
		683,684,685,686,687,689,690,691,692,693,694,695,
		696,698,699,700,701,702,703,704,705,706,707,708,
		709,710,711,712,712,713,714,715,716,717,718,719,
		720,720,721,722,723,724,725,725,726,727,728,728,
		729,730,731,732,732,733,734,734,735,736,737,737,
		738,739,739,740,741,741,742,743,743,744,745,745,
		746,747,747,748,748,749,750,750,751,751,752,753,
		753,754,754,755,755,756,756,757,758,758,759
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
//  Author:Ben.LIU
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN AquariumInit(void);

/*****************************************************************************/
//  Description :AquariumPrepare
//  Global resource dependence : 
//  Author:Ben.LIU
//  Note:
/*****************************************************************************/
LOCAL void AquariumPrepare(void);

/*****************************************************************************/
//  Description : 初始化
//  Global resource dependence : 
//  Author:Ben.LIU
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN AquariumConstruct(MMI_WIN_ID_T win_id);

/*****************************************************************************/
//  Description : 准备背景数据
//  Global resource dependence : 
//  Author:Ben.LIU
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN AquariumDrawBg(MMI_WIN_ID_T win_id,const GUI_LCD_DEV_INFO *dev_info_ptr);

/*****************************************************************************/
//  Description : 算法
//  Global resource dependence : 
//  Author:Ben.LIU
//  Note:
/*****************************************************************************/
LOCAL void AquariumCalculate(LW_CAL_PARAM *lw_cal_param);
/*****************************************************************************/
//  Description : 释放
//  Global resource dependence : 
//  Author:Ben.LIU
//  Note:销毁临时申请的变量
/*****************************************************************************/
LOCAL void AquariumDestruct(void);
/*****************************************************************************/
//  Description : 交互事件处理
//  Global resource dependence : 
//  Author:Ben.LIU
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN AquariumHandleEvent(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
/*****************************************************************************/
//  Description : get pic buf
//  Global resource dependence : 
//  Author:Ben.LIU
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN submarineWorldObj(
	OCEAN_OBJ_T *ocean_obj, 
	OCEAN_TYPE_E type,
	MMI_IMAGE_ID_T image_id, 
	MMI_HANDLE_T win_id);

/*****************************************************************************/
//  Description : a standard way to get pic buf by layer
//  Global resource dependence : 
//  Author:Ben.LIU
//  Note:
/*****************************************************************************/
LOCAL uint32 MMIIDLE_Aquarium_CreateImageLayer(
	OCEAN_OBJ_T *g_ocean_obj,
	OCEAN_TYPE_E type,
	UILAYER_INFO_T *layer_info_ptr, 
	GUI_LCD_DEV_INFO *layer_handle_ptr, 
	MMI_IMAGE_ID_T image_id, 
	MMI_HANDLE_T win_id);
/*****************************************************************************/
//  Description : sun shine action
//  Global resource dependence : 
//  Author:Ben.LIU
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SunlightShining(
	const OCEAN_OBJ_T *ocean_obj,
	OCEAN_SUNSHINE_T *ocean_sunshine,
	uint16 bg_window_w, 
	uint16 bg_window_h,
	COLOR16 *bg_buf, 
	COLOR16	*out_buf);
/*****************************************************************************/
//  Description : Bubble action
//  Global resource dependence : 
//  Author:Ben.LIU
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN OceanBubbling(	
	const OCEAN_OBJ_T *ocean_obj,
	OCEAN_BUBBLE_T *ocean_bubble,
	uint16 bg_window_w, 
	uint16 bg_window_h,
	COLOR16 *bg_buf, 
	uint16 bubble_x,//where do the bubbling
	uint16 bubble_static_y,//where do the bubbling
	COLOR16	*out_buf,
	BOOLEAN *static_BubblingState);
/*****************************************************************************/
//  Description : Food action
//  Global resource dependence : 
//  Author:Ben.LIU
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN FoodFallingDown(	
	const OCEAN_OBJ_T *ocean_obj,
	OCEAN_FOOD_T *ocean_food,
	uint16 bg_window_w, 
	uint16 bg_window_h,
	COLOR16 *bg_buf, 
	uint16 Fallingfood_x,
	uint16 Fallingfood_y,
	COLOR16	*out_buf,BOOLEAN *Food_State);
/*****************************************************************************/
//  Description : Fish action
//  Global resource dependence : 
//  Author:Ben.LIU
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN FishSwimming(	
	const OCEAN_OBJ_T *ocean_obj,
	OCEAN_FISH_T *ocean_fish,
	OCEAN_FOOD_T *ocean_food,
	uint16 bg_window_w, 
	uint16 bg_window_h,
	COLOR16 *bg_buf, 
	COLOR16	*out_buf);
/*****************************************************************************/
//  Description : 获得动态壁纸类型
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL uint32 AquariumGetType(void);
/*****************************************************************************/
//  Description : 获得动态壁纸ThumbNail信息
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL void AquariumGetThumbNailInfo(LW_THUMBNAIL_INFO *thumbnail_info_ptr);

LOCAL void Hit_PointProc(const GUI_POINT_T *PointDown);
LOCAL int atan_degree10(int dx, int dy);

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/
PUBLIC const MMI_LIVE_WALLPAPER_ITEM_INFO_T g_aquarium_Instance =
{
    AquariumInit,
    AquariumConstruct,
    AquariumDrawBg,
    AquariumCalculate,
    AquariumDestruct,
    AquariumHandleEvent,
    PNULL,
    AquariumGetType,
    AquariumGetThumbNailInfo,
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
//  Author:Ben.LIU
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN AquariumInit(void)
{	
    BOOLEAN b_result = TRUE;

    return b_result;
}

/*****************************************************************************/
//  Description :AquariumPrepare
//  Global resource dependence : 
//  Author:Ben.LIU
//  Note:
/*****************************************************************************/
LOCAL void AquariumPrepare(void)
{
    int32 i = 0,sunshine_velocity = 1;
    
    SCI_TRACE_LOW("AquariumPrepare,before Construct. ");
        
    for(i = OTE_NULL;i<OCEAN_TYPE_MAX; ++i)
    {
    	s_ocean_obj[i].srcARGB_Map = PNULL;
    }
    for(i = 0; i < BUBBLE_NUM; ++i)
    {
    	s_ocean_bubble[i].pos_x = 0;
    	s_ocean_bubble[i].pos_y = 0;
    	s_ocean_bubble[i].IsVisible = SCI_FALSE;
    	s_ocean_bubble[i].bubble_type = BUBBLE_1;
    }

    i = 0;
    s_ocean_sunshine[i].sunshine_type = SUNSHINE_H240_T40;
    s_ocean_sunshine[i].posX = 0;
    s_ocean_sunshine[i].posY = 0;
    s_ocean_sunshine[i].Velocity = sunshine_velocity;

#ifndef LOW_MEMORY_SUPPORT
    ++i;
    s_ocean_sunshine[i].sunshine_type = SUNSHINE_H314_T60;
    s_ocean_sunshine[i].posX = ((OCEAN_BG_WIDTH-60)/2)-60;		
    s_ocean_sunshine[i].posY = 0;
    s_ocean_sunshine[i].Velocity = sunshine_velocity;

    ++i;
    s_ocean_sunshine[i].sunshine_type = SUNSHINE_H290_T90;
    s_ocean_sunshine[i].posX = (OCEAN_BG_WIDTH-60)/2;
    s_ocean_sunshine[i].posY = 0;
    s_ocean_sunshine[i].Velocity = sunshine_velocity;

    /*++i;
    s_ocean_sunshine[i].posX = ((OCEAN_BG_WIDTH-SUNSHINE_WIDTH)/2)+40;
    s_ocean_sunshine[i].posY = 0;
    s_ocean_sunshine[i].Velocity =sunshine_velocity;
    s_ocean_sunshine[i].sunshine_type = SUNSHINE_H240_T60;
    ++i;
    s_ocean_sunshine[i].posX = (OCEAN_BG_WIDTH-SUNSHINE_WIDTH)-40;//-25;
    s_ocean_sunshine[i].posY = 0;
    s_ocean_sunshine[i].Velocity = sunshine_velocity;
    s_ocean_sunshine[i].sunshine_type = SUNSHINE_H314_T90;
    ++i;
    s_ocean_sunshine[i].posX = (OCEAN_BG_WIDTH-SUNSHINE_WIDTH);
    s_ocean_sunshine[i].posY = 0;
    s_ocean_sunshine[i].Velocity = sunshine_velocity;
    s_ocean_sunshine[i].sunshine_type = SUNSHINE_H350_T40;*/

    ++i;
    s_ocean_sunshine[i].posX = 30;
    s_ocean_sunshine[i].posY = 0;
    s_ocean_sunshine[i].Velocity = (-1)*sunshine_velocity;
    s_ocean_sunshine[i].sunshine_type = SUNSHINE_H314_T40;
    ++i;
    s_ocean_sunshine[i].posX = ((OCEAN_BG_WIDTH-60)/2);//-s_ocean_obj[s_ocean_sunshine[i].sunshine_type].up_window.w;
    s_ocean_sunshine[i].posY = 0;
    s_ocean_sunshine[i].Velocity = (-1)*sunshine_velocity;
    s_ocean_sunshine[i].sunshine_type = SUNSHINE_H314_T60;
    ++i;
    s_ocean_sunshine[i].posX = (OCEAN_BG_WIDTH-60)-60;
    s_ocean_sunshine[i].posY = 0;
    s_ocean_sunshine[i].Velocity = (-1)*sunshine_velocity;
    s_ocean_sunshine[i].sunshine_type = SUNSHINE_H350_T90;
    /*++i;
    s_ocean_sunshine[i].posX = (OCEAN_BG_WIDTH-s_ocean_obj[s_ocean_sunshine[i].sunshine_type].up_window.w);
    s_ocean_sunshine[i].posY = 0;
    s_ocean_sunshine[i].Velocity = (-1)*sunshine_velocity;
    s_ocean_sunshine[i].sunshine_type = SUNSHINE_H240_T90;*/
    ++i;
    s_ocean_sunshine[i].posX = OCEAN_BG_WIDTH-60;
    s_ocean_sunshine[i].posY = 0;
    s_ocean_sunshine[i].Velocity = (-1)*sunshine_velocity;
    s_ocean_sunshine[i].sunshine_type = SUNSHINE_H290_T60;
#endif

    for(i = 0; i < FOOD_NUM; ++i)
    {
    	s_ocean_food[i].pos_x = 0;
    	s_ocean_food[i].pos_y = 0;
    	s_ocean_food[i].IsVisible = SCI_FALSE;
    	s_ocean_food[i].food_type = FOOD;
    }

    i = 0;
    s_ocean_fish[i].State = FISH_1;
    s_ocean_fish[i].offset.x = 20;//180;
    s_ocean_fish[i].offset.y = OCEAN_BG_HEIGHT-60/2;//250;
    s_ocean_fish[i].offset.x_dec = 0;
    s_ocean_fish[i].offset.y_dec = 0;		
    s_ocean_fish[i].Velocity = 3;
    s_ocean_fish[i].EndPos_Y_min = 5*OCEAN_BG_HEIGHT/12;
    s_ocean_fish[i].EndPos_Y_max = OCEAN_BG_HEIGHT-60/2-40;
    s_ocean_fish[i].EndPos_X_min = -180;
    s_ocean_fish[i].EndPos_X_max = OCEAN_BG_WIDTH+180;
    s_ocean_fish[i].endPos.x = s_ocean_fish[i].EndPos_X_max;
    s_ocean_fish[i].endPos.y = s_ocean_fish[i].EndPos_Y_min;
    s_ocean_fish[i].fish_type = FISH_1;
    s_ocean_fish[i].pre_angle = 0x7fffffff;
    s_ocean_fish[i].angleStep = 15;//5 degree
    s_ocean_fish[i].doAngleCtrl = SCI_TRUE;
    s_ocean_fish[i].HitFrame = 0;
    s_ocean_fish[i].HitFrameMAX = 14*OCEAN_BG_WIDTH/320;
    s_ocean_fish[i].EscapeDist = s_ocean_fish[i].Velocity*s_ocean_fish[i].HitFrameMAX*2;

    ++i;
    s_ocean_fish[i].State = FISH_2;
    s_ocean_fish[i].offset.x = 100;
    s_ocean_fish[i].offset.y = OCEAN_BG_HEIGHT/2;
    s_ocean_fish[i].offset.x_dec = 0;
    s_ocean_fish[i].offset.y_dec = 0;		
    s_ocean_fish[i].Velocity = 4;
    s_ocean_fish[i].EndPos_Y_min = 5*OCEAN_BG_HEIGHT/12;;
    s_ocean_fish[i].EndPos_Y_max = OCEAN_BG_HEIGHT-60/2-40;
    s_ocean_fish[i].EndPos_X_min = -180;
    s_ocean_fish[i].EndPos_X_max = OCEAN_BG_WIDTH+180;
    s_ocean_fish[i].endPos.x = s_ocean_fish[i].EndPos_X_min;
    s_ocean_fish[i].endPos.y = s_ocean_fish[i].EndPos_Y_min;
    s_ocean_fish[i].fish_type = FISH_2;
    s_ocean_fish[i].pre_angle = 0x7fffffff;
    s_ocean_fish[i].angleStep = 15;//5 degree
    s_ocean_fish[i].doAngleCtrl = SCI_TRUE;
    s_ocean_fish[i].HitFrame = 0;
    s_ocean_fish[i].HitFrameMAX = 15*OCEAN_BG_WIDTH/320;
    s_ocean_fish[i].EscapeDist = s_ocean_fish[i].Velocity*s_ocean_fish[i].HitFrameMAX*2;

    ++i;
    s_ocean_fish[i].State = FISH_3;
    s_ocean_fish[i].offset.x = OCEAN_BG_WIDTH - 60;
    s_ocean_fish[i].offset.y = OCEAN_BG_HEIGHT/2;
    s_ocean_fish[i].offset.x_dec = 0;
    s_ocean_fish[i].offset.y_dec = 0;		
    s_ocean_fish[i].Velocity = 2;
    s_ocean_fish[i].EndPos_Y_min = OCEAN_BG_HEIGHT/4;
    s_ocean_fish[i].EndPos_Y_max = OCEAN_BG_HEIGHT/2;
    s_ocean_fish[i].EndPos_X_min = -180;
    s_ocean_fish[i].EndPos_X_max = OCEAN_BG_WIDTH+180;
    s_ocean_fish[i].endPos.x = s_ocean_fish[i].EndPos_X_min;
    s_ocean_fish[i].endPos.y = s_ocean_fish[i].EndPos_Y_max;
    s_ocean_fish[i].fish_type = FISH_3;
    s_ocean_fish[i].pre_angle = 0x7fffffff;
    s_ocean_fish[i].angleStep = 20;//5 degree
    s_ocean_fish[i].doAngleCtrl = SCI_TRUE;
    s_ocean_fish[i].HitFrame = 0;
    s_ocean_fish[i].HitFrameMAX = 15*OCEAN_BG_WIDTH/320;
    s_ocean_fish[i].EscapeDist = s_ocean_fish[i].Velocity*s_ocean_fish[i].HitFrameMAX*2;

    ++i;
    s_ocean_fish[i].State = FISH_4;
    s_ocean_fish[i].offset.x = 40;
    s_ocean_fish[i].offset.y = OCEAN_BG_HEIGHT/2;
    s_ocean_fish[i].offset.x_dec = 0;
    s_ocean_fish[i].offset.y_dec = 0;		
    s_ocean_fish[i].Velocity = 3;
    s_ocean_fish[i].EndPos_Y_min = 32*OCEAN_BG_HEIGHT/48;
    s_ocean_fish[i].EndPos_Y_max = OCEAN_BG_HEIGHT-60/2-40;
    s_ocean_fish[i].EndPos_X_min = -180;
    s_ocean_fish[i].EndPos_X_max = OCEAN_BG_WIDTH+180;
    s_ocean_fish[i].endPos.x = s_ocean_fish[i].EndPos_X_max ;
    s_ocean_fish[i].endPos.y = s_ocean_fish[i].EndPos_Y_max;
    s_ocean_fish[i].fish_type = FISH_4;
    s_ocean_fish[i].pre_angle = 0x7fffffff;
    s_ocean_fish[i].angleStep = 15;//5 degree
    s_ocean_fish[i].doAngleCtrl = SCI_TRUE;
    s_ocean_fish[i].HitFrame = 0;
    s_ocean_fish[i].HitFrameMAX = 14*OCEAN_BG_WIDTH/320;
    s_ocean_fish[i].EscapeDist = s_ocean_fish[i].Velocity*s_ocean_fish[i].HitFrameMAX*2;

    ++i;
    s_ocean_fish[i].State = FISH_2;
    s_ocean_fish[i].offset.x = 150;
    s_ocean_fish[i].offset.y = OCEAN_BG_HEIGHT/2+50;
    s_ocean_fish[i].offset.x_dec = 0;
    s_ocean_fish[i].offset.y_dec = 0;		
    s_ocean_fish[i].Velocity = 3;
    s_ocean_fish[i].EndPos_Y_min = 5*OCEAN_BG_HEIGHT/12;;
    s_ocean_fish[i].EndPos_Y_max = OCEAN_BG_HEIGHT-60/2;
    s_ocean_fish[i].EndPos_X_min = -80;
    s_ocean_fish[i].EndPos_X_max = OCEAN_BG_WIDTH+80;
    s_ocean_fish[i].endPos.x = s_ocean_fish[i].EndPos_X_max;
    s_ocean_fish[i].endPos.y = s_ocean_fish[i].EndPos_Y_min;
    s_ocean_fish[i].fish_type = FISH_2;
    s_ocean_fish[i].pre_angle = 0x7fffffff;
    s_ocean_fish[i].angleStep = 15;//5 degree
    s_ocean_fish[i].doAngleCtrl = SCI_TRUE;
    s_ocean_fish[i].HitFrame = 0;
    s_ocean_fish[i].HitFrameMAX = 14*OCEAN_BG_WIDTH/320;
    s_ocean_fish[i].EscapeDist = s_ocean_fish[i].Velocity*s_ocean_fish[i].HitFrameMAX*2;
    ++i;
    s_ocean_fish[i].State = FISH_1;
    s_ocean_fish[i].offset.x = 90;//180;
    s_ocean_fish[i].offset.y = OCEAN_BG_HEIGHT-200/2;//250;
    s_ocean_fish[i].offset.x_dec = 0;
    s_ocean_fish[i].offset.y_dec = 0;		
    s_ocean_fish[i].Velocity = 4;
    s_ocean_fish[i].EndPos_Y_min = 5*OCEAN_BG_HEIGHT/12;
    s_ocean_fish[i].EndPos_Y_max = OCEAN_BG_HEIGHT-60/2-40;
    s_ocean_fish[i].EndPos_X_min = -180;
    s_ocean_fish[i].EndPos_X_max = OCEAN_BG_WIDTH+180;
    s_ocean_fish[i].endPos.x = s_ocean_fish[i].EndPos_X_min;
    s_ocean_fish[i].endPos.y = s_ocean_fish[i].EndPos_Y_min;
    s_ocean_fish[i].fish_type = FISH_1;
    s_ocean_fish[i].pre_angle = 0x7fffffff;
    s_ocean_fish[i].angleStep = 15;//5 degree
    s_ocean_fish[i].doAngleCtrl = SCI_TRUE;
    s_ocean_fish[i].HitFrame = 0;
    s_ocean_fish[i].HitFrameMAX = 15*OCEAN_BG_WIDTH/320;
    s_ocean_fish[i].EscapeDist = s_ocean_fish[i].Velocity*s_ocean_fish[i].HitFrameMAX*2;

    ++i;
    s_ocean_fish[i].State = FISH_1;
    s_ocean_fish[i].offset.x = 180;//180;
    s_ocean_fish[i].offset.y = OCEAN_BG_HEIGHT-120/2;//250;
    s_ocean_fish[i].offset.x_dec = 0;
    s_ocean_fish[i].offset.y_dec = 0;		
    s_ocean_fish[i].Velocity = 2;
    s_ocean_fish[i].EndPos_Y_min = 5*OCEAN_BG_HEIGHT/12;
    s_ocean_fish[i].EndPos_Y_max = OCEAN_BG_HEIGHT-60/2-40;
    s_ocean_fish[i].EndPos_X_min = -180;
    s_ocean_fish[i].EndPos_X_max = OCEAN_BG_WIDTH+180;
    s_ocean_fish[i].endPos.x = s_ocean_fish[i].EndPos_X_max;
    s_ocean_fish[i].endPos.y = s_ocean_fish[i].EndPos_Y_min;
    s_ocean_fish[i].fish_type = FISH_1;
    s_ocean_fish[i].pre_angle = 0x7fffffff;
    s_ocean_fish[i].angleStep = 15;//5 degree
    s_ocean_fish[i].doAngleCtrl = SCI_TRUE;
    s_ocean_fish[i].HitFrame = 0;
    s_ocean_fish[i].HitFrameMAX = 14*OCEAN_BG_WIDTH/320;
    s_ocean_fish[i].EscapeDist = s_ocean_fish[i].Velocity*s_ocean_fish[i].HitFrameMAX*2;

    ++i;
    s_ocean_fish[i].State = FISH_4;
    s_ocean_fish[i].offset.x = 140;
    s_ocean_fish[i].offset.y = OCEAN_BG_HEIGHT/2+80;
    s_ocean_fish[i].offset.x_dec = 0;
    s_ocean_fish[i].offset.y_dec = 0;		
    s_ocean_fish[i].Velocity = 2;
    s_ocean_fish[i].EndPos_Y_min = 32*OCEAN_BG_HEIGHT/48;
    s_ocean_fish[i].EndPos_Y_max = OCEAN_BG_HEIGHT-60/2-40;
    s_ocean_fish[i].EndPos_X_min = -180;
    s_ocean_fish[i].EndPos_X_max = OCEAN_BG_WIDTH+180;
    s_ocean_fish[i].endPos.x = s_ocean_fish[i].EndPos_X_max ;
    s_ocean_fish[i].endPos.y = s_ocean_fish[i].EndPos_Y_min;
    s_ocean_fish[i].fish_type = FISH_4;
    s_ocean_fish[i].pre_angle = 0x7fffffff;
    s_ocean_fish[i].angleStep = 15;//5 degree
    s_ocean_fish[i].doAngleCtrl = SCI_TRUE;
    s_ocean_fish[i].HitFrame = 0;
    s_ocean_fish[i].HitFrameMAX = 14*OCEAN_BG_WIDTH/320;
    s_ocean_fish[i].EscapeDist = s_ocean_fish[i].Velocity*s_ocean_fish[i].HitFrameMAX*2;

    bubble_y = 0;
    BubblingState = SCI_FALSE;
}

/*****************************************************************************/
//  Description : 线性光幕初始化
//  Global resource dependence : 
//  Author:Ben.LIU
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN AquariumConstruct(MMI_WIN_ID_T win_id)
{
    BOOLEAN b_result = FALSE;
	
    do
    {
        //initial s_ocean_obj;
        if(PNULL != s_ocean_obj)
        {
            SCI_TRACE_LOW("SeaworldConstruct, have a exception before Construct: PNULL != s_bubble_array ");
            MMILIVEWALLPAPER_FREE(s_ocean_obj);
            s_ocean_obj = PNULL;
        }
        s_ocean_obj = MMILIVEWALLPAPER_ALLOC((sizeof(OCEAN_OBJ_T))*OCEAN_TYPE_MAX);
        if(PNULL == s_ocean_obj)
        {
            break;
        }

        //initial s_ocean_bubble;
        if(PNULL != s_ocean_bubble)
        {
            SCI_TRACE_LOW("SeaworldConstruct, have a exception before Construct: PNULL != s_sunshine_array ");
            MMILIVEWALLPAPER_FREE(s_ocean_bubble);
            s_ocean_bubble = PNULL;
        }
        s_ocean_bubble = MMILIVEWALLPAPER_ALLOC((sizeof(OCEAN_BUBBLE_T))*BUBBLE_NUM);
        if(PNULL == s_ocean_bubble)
        {
            break;
        }

        //initial s_ocean_sunshine;
        if(PNULL != s_ocean_sunshine)
        {
            SCI_TRACE_LOW("SeaworldConstruct, have a exception before Construct: PNULL != s_fish_array ");
            MMILIVEWALLPAPER_FREE(s_ocean_sunshine);
            s_ocean_sunshine = PNULL;
        }
        s_ocean_sunshine = MMILIVEWALLPAPER_ALLOC((sizeof(OCEAN_SUNSHINE_T))*SUNSHINE_NUM);
        if(PNULL == s_ocean_sunshine)
        {
            break;
        }
        
        //initial s_ocean_fish;
        if(PNULL != s_ocean_fish)
        {
            SCI_TRACE_LOW("SeaworldConstruct, have a exception before Construct: PNULL != s_bubble_array ");
            MMILIVEWALLPAPER_FREE(s_ocean_fish);
            s_ocean_fish = PNULL;
        }
        s_ocean_fish = MMILIVEWALLPAPER_ALLOC((sizeof(OCEAN_FISH_T))*FISH_NUM);
        if(PNULL == s_ocean_fish)
        {
            break;
        }

        //initial s_ocean_food;
        if(PNULL != s_ocean_food)
        {
            SCI_TRACE_LOW("SeaworldConstruct, have a exception before Construct: PNULL != s_sunshine_array ");
            MMILIVEWALLPAPER_FREE(s_ocean_food);
            s_ocean_food = PNULL;
        }
        s_ocean_food = MMILIVEWALLPAPER_ALLOC((sizeof(OCEAN_FOOD_T))*FOOD_NUM);
        if(PNULL == s_ocean_food)
        {
            break;
        }

        //intial preparation;
        AquariumPrepare();

        if(PNULL != s_Point_Add_ptr)
        {
            SCI_TRACE_LOW("AquariumConstruct, have a exception before Construct: PNULL != s_Point_Add_ptr ");
            MMILIVEWALLPAPER_FREE(s_Point_Add_ptr);
            s_Point_Add_ptr = PNULL;
        }
        s_Point_Add_ptr=MMILIVEWALLPAPER_ALLOC((sizeof(FIX16_POINT_T))*1024);
        if(PNULL == s_Point_Add_ptr)
        {
            break;
        }

        if(PNULL != Fish_srcARGB_temp)
        {
            SCI_TRACE_LOW("AquariumConstruct, have a exception before Construct: PNULL != Fish_srcARGB_temp ");
            MMILIVEWALLPAPER_FREE(Fish_srcARGB_temp);
            Fish_srcARGB_temp = PNULL;
        }
        Fish_srcARGB_temp = MMILIVEWALLPAPER_ALLOC((sizeof(COLOR32))*FISH_SIZE_MAX*FISH_SIZE_MAX);
        if(PNULL == Fish_srcARGB_temp)
        {
            //SCI_TRACE_LOW:"Ben, AquariumConstruct, Fish_srcARGB_temp buffer failed"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_LIVEWALLPAPER_AQUARIUM_603_112_2_18_2_31_54_18,(uint8*)"");
            break;
        }

        //all the mems have been allocated successfully, set the flag as true.
        b_result = TRUE;
    }while(0);

    if(!b_result)
    {
        //in the case of failure to allocate memory,we need to release all the mems allocated. 
        SCI_TRACE_LOW("AquariumConstruct, have a exception at allocating process in Construct() ");
        AquariumDestruct();
    }

    //start to get different pics' buf;
    SCI_TRACE_LOW("AquariumConstruct, to get different pics' buf;start.");
    if(b_result)
    {
        b_result = submarineWorldObj(s_ocean_obj,FISH_1,IMAGE_WALLPAPER_AQUARIUM_FISH1_1, win_id);
    }

    if(b_result)
    {        
        b_result = submarineWorldObj(s_ocean_obj,FISH_1_2,IMAGE_WALLPAPER_AQUARIUM_FISH1_2, win_id);
    }
    
    if(b_result)
    {
        b_result = submarineWorldObj(s_ocean_obj,FISH_1_3,IMAGE_WALLPAPER_AQUARIUM_FISH1_3, win_id);
    }
    
    if(b_result)
    {
        b_result = submarineWorldObj(s_ocean_obj,FISH_1_4,IMAGE_WALLPAPER_AQUARIUM_FISH1_4, win_id);
    }
    
    if(b_result)
    {
        b_result = submarineWorldObj(s_ocean_obj,FISH_2,IMAGE_WALLPAPER_AQUARIUM_FISH2_1, win_id);
    }
    
    if(b_result)
    {
        b_result = submarineWorldObj(s_ocean_obj,FISH_2_2,IMAGE_WALLPAPER_AQUARIUM_FISH2_2, win_id);
    }
    if(b_result)
    {
        b_result = submarineWorldObj(s_ocean_obj,FISH_2_3,IMAGE_WALLPAPER_AQUARIUM_FISH2_3, win_id);
    }
    
    if(b_result)
    {
        b_result = submarineWorldObj(s_ocean_obj,FISH_2_4,IMAGE_WALLPAPER_AQUARIUM_FISH2_4, win_id);
    }
    
    if(b_result)
    {
        b_result = submarineWorldObj(s_ocean_obj,FISH_3,IMAGE_WALLPAPER_AQUARIUM_FISH3_1, win_id);
    }
    
    if(b_result)
    {
        b_result = submarineWorldObj(s_ocean_obj,FISH_3_2,IMAGE_WALLPAPER_AQUARIUM_FISH3_2, win_id);
    }
    
    if(b_result)
    {
        b_result = submarineWorldObj(s_ocean_obj,FISH_3_3,IMAGE_WALLPAPER_AQUARIUM_FISH3_3, win_id);
    }
    
    if(b_result)
    {
        b_result = submarineWorldObj(s_ocean_obj,FISH_3_4,IMAGE_WALLPAPER_AQUARIUM_FISH3_4, win_id);
    }
    
    if(b_result)
    {
        b_result = submarineWorldObj(s_ocean_obj,FISH_4, IMAGE_WALLPAPER_AQUARIUM_FISH4_1, win_id);
    }
    
    if(b_result)
    {
        b_result = submarineWorldObj(s_ocean_obj,FISH_4_2,IMAGE_WALLPAPER_AQUARIUM_FISH4_2, win_id);
    }
    
    if(b_result)
    {
        b_result = submarineWorldObj(s_ocean_obj,FISH_4_3,IMAGE_WALLPAPER_AQUARIUM_FISH4_3, win_id);
    }
    
    if(b_result)
    {
        b_result = submarineWorldObj(s_ocean_obj,FISH_4_4,IMAGE_WALLPAPER_AQUARIUM_FISH4_4, win_id);
    }
    
    if(b_result)
    {
        b_result = submarineWorldObj(s_ocean_obj,FOOD,IMAGE_WALLPAPER_AQUARIUM_FOOD, win_id);
    }
    
    if(b_result)
    {
        b_result = submarineWorldObj(s_ocean_obj,BUBBLE_1,IMAGE_WALLPAPER_AQUARIUM_BUBBLE_1, win_id);
    }
    
    if(b_result)
    {
        b_result = submarineWorldObj(s_ocean_obj,BUBBLE_2,IMAGE_WALLPAPER_AQUARIUM_BUBBLE_2, win_id);
    }
    
    if(b_result)
    {
        b_result = submarineWorldObj(s_ocean_obj,BUBBLE_3,IMAGE_WALLPAPER_AQUARIUM_BUBBLE_3, win_id);
    }
    
    if(b_result)
    {
        b_result = submarineWorldObj(s_ocean_obj,BUBBLE_4,IMAGE_WALLPAPER_AQUARIUM_BUBBLE_4, win_id);
    }
    
    if(b_result)
    {
        b_result = submarineWorldObj(s_ocean_obj,SUNSHINE_H240_T40,IMAGE_WALLPAPER_AQUARIUM_SUNLIGHT_240_40, win_id);
    }
    
    if(b_result)
    {
        b_result = submarineWorldObj(s_ocean_obj,SUNSHINE_H240_T60,IMAGE_WALLPAPER_AQUARIUM_SUNLIGHT_240_60, win_id);
    }
    
#if 0 //#ifndef LOW_MEMORY_SUPPORT    //aquarium needs 1.3M, all projects' RAM can't support these photoes below; whatever LOW_MEMORY_SUPPORT is;
    if(b_result)
    {
        b_result = submarineWorldObj(s_ocean_obj,SUNSHINE_H240_T90,IMAGE_WALLPAPER_AQUARIUM_SUNLIGHT_240_90, win_id);
    }
    
    if(b_result)
    {      
        b_result = submarineWorldObj(s_ocean_obj,SUNSHINE_H290_T40,IMAGE_WALLPAPER_AQUARIUM_SUNLIGHT_290_40, win_id);
    }
    
    if(b_result)
    {
        b_result = submarineWorldObj(s_ocean_obj,SUNSHINE_H290_T60,IMAGE_WALLPAPER_AQUARIUM_SUNLIGHT_290_60, win_id);
    }
    
    if(b_result)
    {
        b_result = submarineWorldObj(s_ocean_obj,SUNSHINE_H290_T90,IMAGE_WALLPAPER_AQUARIUM_SUNLIGHT_290_90, win_id);
    }
    
    if(b_result)
    {
        b_result = submarineWorldObj(s_ocean_obj,SUNSHINE_H314_T40,IMAGE_WALLPAPER_AQUARIUM_SUNLIGHT_314_40, win_id);
    }
    
    if(b_result)
    {
        b_result = submarineWorldObj(s_ocean_obj,SUNSHINE_H314_T60,IMAGE_WALLPAPER_AQUARIUM_SUNLIGHT_314_60, win_id);
    }
    
    if(b_result)
    {
        b_result = submarineWorldObj(s_ocean_obj,SUNSHINE_H314_T90,IMAGE_WALLPAPER_AQUARIUM_SUNLIGHT_314_90, win_id);
    }
    if(b_result)
    {
        b_result = submarineWorldObj(s_ocean_obj,SUNSHINE_H350_T40,IMAGE_WALLPAPER_AQUARIUM_SUNLIGHT_350_40, win_id);
    }
    
    if(b_result)
    {
        b_result = submarineWorldObj(s_ocean_obj,SUNSHINE_H350_T60,IMAGE_WALLPAPER_AQUARIUM_SUNLIGHT_350_60, win_id);
    }
    if(b_result)
    {      
        b_result = submarineWorldObj(s_ocean_obj,SUNSHINE_H350_T90,IMAGE_WALLPAPER_AQUARIUM_SUNLIGHT_350_90, win_id);
    }
#endif
    SCI_TRACE_LOW("AquariumConstruct, to get different pics' buf;end.");

    return b_result;
}

/*****************************************************************************/
//  Description : 线性光幕准备背景数据
//  Global resource dependence : 
//  Author:Ben.LIU
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN AquariumDrawBg(MMI_WIN_ID_T win_id,const GUI_LCD_DEV_INFO *dev_info_ptr)
{
    BOOLEAN b_result = FALSE;
	GUI_POINT_T display_point = {0,0};

    if (PNULL != dev_info_ptr)
    {
        b_result = GUIRES_DisplayImg(&display_point, 
                                            PNULL, 
                                            PNULL, 
                                            win_id, 
                                            IMAGE_WALLPAPER_AQUARIUM_BG, 
                                            dev_info_ptr);
    }
    return b_result;
}

/*****************************************************************************/
//  Description : 线性光幕算法
//  Global resource dependence : 
//  Author:Ben.LIU
//  Note:
/*****************************************************************************/
LOCAL void AquariumCalculate(LW_CAL_PARAM *lw_cal_param)
{
    int32 i = 0,bubble_x = 0;
    IMGREF_SIZE_T bg_window;	
    uint16 scr_width = 0;
    uint16 scr_height = 0;

    if(PNULL == lw_cal_param || PNULL == lw_cal_param->dest_buf_info.buf_ptr || PNULL == lw_cal_param->src_buf_info.buf_ptr)
    {
        return;
    }
    scr_width = lw_cal_param->src_buf_info.w;
    scr_height = lw_cal_param->src_buf_info.h;

    SCI_MEMCPY(lw_cal_param->dest_buf_info.buf_ptr, lw_cal_param->src_buf_info.buf_ptr, sizeof(uint16)*scr_width*scr_height);
    bg_window.h = scr_height;
    bg_window.w = scr_width;
    //*******************FISH**********************	
    if(PNULL == s_ocean_obj || PNULL == s_ocean_food || PNULL == s_ocean_fish|| PNULL == s_ocean_sunshine || PNULL == s_ocean_bubble)
    {
        return;
    }
        
    FoodFallingDown(s_ocean_obj, s_ocean_food,bg_window.w, bg_window.h, lw_cal_param->dest_buf_info.buf_ptr, food_x, food_y, lw_cal_param->dest_buf_info.buf_ptr, &FoodState);

    //*******************FISH**********************	

    FishSwimming(s_ocean_obj, s_ocean_fish, s_ocean_food,bg_window.w, bg_window.h, lw_cal_param->dest_buf_info.buf_ptr, lw_cal_param->dest_buf_info.buf_ptr);

    //*******************SUNSHINE**********************	

    SunlightShining(s_ocean_obj, s_ocean_sunshine,bg_window.w, bg_window.h, lw_cal_param->dest_buf_info.buf_ptr, lw_cal_param->dest_buf_info.buf_ptr);

    //*******************BUBBLE**********************	

    if(SCI_TRUE == BubblingState)
    {
        i=bubble_y;
        OceanBubbling(s_ocean_obj, s_ocean_bubble,bg_window.w, bg_window.h, lw_cal_param->dest_buf_info.buf_ptr, bubble_x, bubble_y, lw_cal_param->dest_buf_info.buf_ptr, &BubblingState);
    }
    else
    {
        if(3 == (rand()%5))
        {
            bubble_x = 30+rand()%(OCEAN_BG_WIDTH-30);
            bubble_y = 2*OCEAN_BG_HEIGHT/3+rand()%80;
            i=bubble_y;
            OceanBubbling(s_ocean_obj, s_ocean_bubble,bg_window.w, bg_window.h, lw_cal_param->dest_buf_info.buf_ptr, bubble_x, bubble_y, lw_cal_param->dest_buf_info.buf_ptr, &BubblingState);
        }
    }	
	
}
/*****************************************************************************/
//  Description : 线性光幕释放
//  Global resource dependence : 
//  Author:Ben.LIU
//  Note:
/*****************************************************************************/
LOCAL void AquariumDestruct(void)
{	
    int32 i = 0;
    //SCI_FREE(bg_merged_buf_ptr);

	if(PNULL != Fish_srcARGB_temp)
	{
    	MMILIVEWALLPAPER_FREE(Fish_srcARGB_temp);//SCI_FREE
	}

    if(PNULL != s_ocean_obj)
	{
		for(i = FISH_1; i < OCEAN_TYPE_MAX; ++i)
		{
			if(PNULL != s_ocean_obj[i].srcARGB_Map)
			{
				MMILIVEWALLPAPER_FREE(s_ocean_obj[i].srcARGB_Map);
			}
		}
	}

    if(PNULL != s_Point_Add_ptr)
    {
        MMILIVEWALLPAPER_FREE(s_Point_Add_ptr);
        s_Point_Add_ptr = PNULL;
    }
        
    if(PNULL != s_ocean_obj)
    {
        MMILIVEWALLPAPER_FREE(s_ocean_obj);
        s_ocean_obj = PNULL;
    }

    if(PNULL != s_ocean_bubble)
    {
        MMILIVEWALLPAPER_FREE(s_ocean_bubble);
        s_ocean_bubble = PNULL;
    }

    if(PNULL != s_ocean_sunshine)
    {
        MMILIVEWALLPAPER_FREE(s_ocean_sunshine);
        s_ocean_sunshine = PNULL;
    }
    
    if(PNULL != s_ocean_fish)
    {
        MMILIVEWALLPAPER_FREE(s_ocean_fish);
        s_ocean_fish = PNULL;
    }

    if(PNULL != s_ocean_food)
    {
        MMILIVEWALLPAPER_FREE(s_ocean_food);
        s_ocean_food = PNULL;
    }
}

/*****************************************************************************/
//  Description : 线性光幕释交互事件处理
//  Global resource dependence : 
//  Author:Ben.LIU
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN AquariumHandleEvent(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    GUI_POINT_T   point_down       = {0};

    switch(msg_id)
    {
    case MSG_TP_PRESS_DOWN:
        point_down.x = MMK_GET_TP_X(param);
        point_down.y = MMK_GET_TP_Y(param);
        Hit_PointProc(&point_down);
        break;
    default:
        return FALSE;
    }
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    return TRUE;
}

/*****************************************************************************/
//  Description : get pic buf
//  Global resource dependence : 
//  Author:Ben.LIU
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN submarineWorldObj(
	OCEAN_OBJ_T *ocean_obj, 
	OCEAN_TYPE_E type,
	MMI_IMAGE_ID_T image_id, 
	MMI_HANDLE_T win_id)
{
	int i,j;
	UILAYER_INFO_T s_windmill_wing_layer_info ;
	GUI_LCD_DEV_INFO s_windmill_wing_layer_handle ;	
        BOOLEAN b_result = TRUE;

        if(PNULL == ocean_obj || (type >= OCEAN_TYPE_MAX))
        {
            return FALSE;
        }
	
	b_result = MMIIDLE_Aquarium_CreateImageLayer(ocean_obj,type, &s_windmill_wing_layer_info, &s_windmill_wing_layer_handle, image_id, win_id);
        if(!b_result)
        {
            return FALSE;
        }
        
    if(PNULL == ocean_obj[type].srcARGB_Map)
	{
    	return FALSE;
    }
	for(i=0;i<ocean_obj[type].up_window.h;++i)
	{
		for(j=0;j<ocean_obj[type].up_window.w;++j)
		{
			if(ocean_obj[type].srcARGB_Map[i*ocean_obj[type].up_window.w+j] < 0x08000000)// approximate to 0
			{
				ocean_obj[type].srcARGB_Map[i*ocean_obj[type].up_window.w+j]= 0;
			}
		}
	}

    return b_result;
}
/*****************************************************************************/
//  Description : a standard way to get pic buf by layer
//  Global resource dependence : 
//  Author:Ben.LIU
//  Note:
/*****************************************************************************/
LOCAL uint32 MMIIDLE_Aquarium_CreateImageLayer(OCEAN_OBJ_T *ocean_obj,OCEAN_TYPE_E type,UILAYER_INFO_T *layer_info_ptr, GUI_LCD_DEV_INFO *layer_handle_ptr, MMI_IMAGE_ID_T image_id, MMI_HANDLE_T win_id)
{
    UILAYER_CREATE_T create_info = {0}; 
    GUI_POINT_T      display_point       = {0,0};
    uint32 *buf_ptr;

    uint8 * wallpaper_layer_buf_ptr = PNULL;
    if((PNULL == ocean_obj) || (PNULL == layer_info_ptr) || (PNULL == layer_handle_ptr))
    {
        return 0;
    }
    
	/*先创建一个层*/
	layer_handle_ptr->lcd_id = 0;
	layer_handle_ptr->block_id = UILAYER_NULL_HANDLE;

	create_info.lcd_id = GUI_MAIN_LCD_ID;
	create_info.owner_handle = (MMI_HANDLE_T)win_id;
	create_info.offset_x = 0;
	create_info.offset_y = 0;
	GUIRES_GetImgWidthHeight((uint16*)&create_info.width, (uint16*)&create_info.height, image_id, win_id);
	
	ocean_obj[type].up_window.h = create_info.height;
	ocean_obj[type].up_window.w = create_info.width;
	ocean_obj[type].srcARGB_Map = MMILIVEWALLPAPER_ALLOC((sizeof(COLOR32))*(ocean_obj[type].up_window.w)*(ocean_obj[type].up_window.h));
	if(PNULL == ocean_obj[type].srcARGB_Map)
	{
		//SCI_TRACE_LOW:"Ben, MMIIDLE_Aquarium_CreateImageLayer buf Alloca error, type = %d ms"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_LIVEWALLPAPER_AQUARIUM_812_112_2_18_2_31_54_19,(uint8*)"d", type);
		return 0;	
	}
	buf_ptr = ocean_obj[type].srcARGB_Map;
	
	create_info.is_bg_layer = FALSE;
	create_info.is_static_layer = TRUE; 
    	create_info.func_ptr = MMIAPILIVEWALLPAPER_UilayerMemPoolFunc(); 
	
	UILAYER_CreateLayer(
							&create_info,
							layer_handle_ptr
							);	
	// 设置color key
	UILAYER_SetLayerColorKey(layer_handle_ptr, TRUE, UILAYER_TRANSPARENT_COLOR);

	// 使用color清除层
	UILAYER_Clear(layer_handle_ptr);

	//把图片画到层上
	GUIRES_DisplayImg(&display_point,
						PNULL,
						PNULL,
						win_id,
						image_id,
						(const GUI_LCD_DEV_INFO*)layer_handle_ptr);
	//获取图片的信息
	UILAYER_GetLayerInfo(layer_handle_ptr, layer_info_ptr);

        wallpaper_layer_buf_ptr = UILAYER_GetLayerBufferPtr(layer_handle_ptr);  

	//
        if((PNULL == buf_ptr) || (PNULL == wallpaper_layer_buf_ptr))
        {
            return 0;
        }
	SCI_MEMCPY(buf_ptr, wallpaper_layer_buf_ptr, create_info.width*create_info.height*4);

	
	//注销层
	UILAYER_ReleaseLayer(layer_handle_ptr);

	return 1;

}

LOCAL BOOLEAN SunlightShining(
	const OCEAN_OBJ_T *ocean_obj,
	OCEAN_SUNSHINE_T *ocean_sunshine,
	uint16 bg_window_w, 
	uint16 bg_window_h,
	COLOR16 *bg_buf, 
	COLOR16	*out_buf)
{

	int edge_L =0;
	int edge_R =0;
	//int deta_posX = 0;

	FIX16_POINT_T	Sunlight_offset;
	COLOR32	*up_buf;
	OCEAN_TYPE_E ocean_type_tmp;
	IMGREF_SIZE_T bg_window;	
	int i = 0;
	static int32 slowdown = 0;;

	slowdown++;
	if(slowdown>0)
		slowdown = 0;
	bg_window.w = bg_window_w;
	bg_window.h = bg_window_h;
	
	edge_L = (ocean_obj[ocean_sunshine[0].sunshine_type].up_window.w)*(-1);
	edge_R = OCEAN_BG_WIDTH;
	
	for(i = 0; i < SUNSHINE_NUM; ++i)
	{
		if(!slowdown)
		{
			ocean_sunshine[i].posX = ocean_sunshine[i].posX + ocean_sunshine[i].Velocity;
			if(edge_L > ocean_sunshine[i].posX)
			{
				ocean_sunshine[i].posX = 2*edge_L -ocean_sunshine[i].posX;
				ocean_sunshine[i].Velocity = ocean_sunshine[i].Velocity * (-1);
			}
			else if(edge_R < ocean_sunshine[i].posX)
			{
				ocean_sunshine[i].posX = 2*edge_R -ocean_sunshine[i].posX;
				ocean_sunshine[i].Velocity = ocean_sunshine[i].Velocity * (-1);
			}
		}

		Sunlight_offset.x = ocean_sunshine[i].posX;
		Sunlight_offset.y = ocean_sunshine[i].posY;
		Sunlight_offset.x_dec = 0;
		Sunlight_offset.y_dec = 0;
		
		ocean_type_tmp =  ocean_sunshine[i].sunshine_type;

		up_buf = ocean_obj[ocean_type_tmp].srcARGB_Map;

		//merge
		if( !S2D_Blending(bg_window, bg_buf, ocean_obj[ocean_type_tmp].up_window,Sunlight_offset, up_buf, out_buf) )
		{
			//SCI_TRACE_LOW:"Ben, SUNSHINE buffer merge failed"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_LIVEWALLPAPER_AQUARIUM_911_112_2_18_2_31_54_20,(uint8*)"");
			return SCI_FALSE;
		}
	}

	return SCI_TRUE;
}
LOCAL BOOLEAN OceanBubbling(	
	const OCEAN_OBJ_T *ocean_obj,
	OCEAN_BUBBLE_T *ocean_bubble,
	uint16 bg_window_w, 
	uint16 bg_window_h,
	COLOR16 *bg_buf, 
	uint16 bubble_x,//where do the bubbling
	uint16 bubble_static_y,//where do the bubbling
	COLOR16	*out_buf,
	BOOLEAN *static_BubblingState)
{
	BOOLEAN IsBubbling = *static_BubblingState;
	IMGREF_SIZE_T bg_window;
	int i = 0, bubble_y_rand = 0;//, j = 0
	FIX16_POINT_T	offset;
	COLOR32	*up_buf;
	static int32 slowdown = 0;;

	slowdown++;
	if(slowdown>0)
		slowdown = 0;

	bg_window.w = bg_window_w;
	bg_window.h = bg_window_h;
	if(!IsBubbling)
	{
		IsBubbling = SCI_TRUE;
		for(i = 0; i < BUBBLE_NUM; ++i)
		{
			ocean_bubble[i].pos_x = bubble_x + (rand()%BUBBLE_X_WINDOW);
			bubble_y_rand = bubble_y_rand + (rand()%BUBBLE_Y_WINDOW);
			ocean_bubble[i].pos_y = bubble_static_y + bubble_y_rand;
			ocean_bubble[i].IsVisible = ((rand()%10)<3) ? SCI_FALSE: SCI_TRUE;
			ocean_bubble[i].bubble_type = (OCEAN_TYPE_E)((rand()%4)+BUBBLE_1) ;
		}
	}
	else if(ocean_bubble[BUBBLE_NUM-1].pos_y > 0)
	{		
		if(0 == slowdown)
		{
			for(i = 0; i < BUBBLE_NUM; ++i)
			{
				if( ocean_bubble[i].pos_y > 0)
				{
					--ocean_bubble[i].pos_y;
				}
			}
		}
	}
	else
	{
		IsBubbling = SCI_FALSE;
	}

	if(IsBubbling)
	{
		for(i = 0; i < BUBBLE_NUM; ++i)
		{
			if( SCI_TRUE == ocean_bubble[i].IsVisible )
			{				
				if((ocean_bubble[i].pos_y <= bubble_static_y) && (ocean_bubble[i].pos_y >= 0)&&(ocean_bubble[i].pos_x>=0)&&(ocean_bubble[i].pos_x<=OCEAN_BG_WIDTH))
				{
					offset.x = ocean_bubble[i].pos_x;
					offset.y = ocean_bubble[i].pos_y;
					offset.x_dec = 0;
					offset.y_dec = 0;
					up_buf = ocean_obj[ocean_bubble[i].bubble_type].srcARGB_Map;
					if( !S2D_Blending(bg_window, bg_buf, ocean_obj[ocean_bubble[i].bubble_type].up_window,offset, up_buf, out_buf) )
					{
						//SCI_TRACE_LOW:"Bubbling buffer failed"
						SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_LIVEWALLPAPER_AQUARIUM_987_112_2_18_2_31_54_21,(uint8*)"");
						//MessageBox(NULL, "Bubbling buffer failed", "Error", MB_OK);
						return SCI_FALSE;
					}
				}				
			}
		}
	}
	*static_BubblingState = IsBubbling;
	return SCI_TRUE;
}
LOCAL BOOLEAN FoodFallingDown(	
	const OCEAN_OBJ_T *ocean_obj,
	OCEAN_FOOD_T *ocean_food,
	uint16 bg_window_w, 
	uint16 bg_window_h,
	COLOR16 *bg_buf, 
	uint16 Fallingfood_x,
	uint16 Fallingfood_y,
	COLOR16	*out_buf,BOOLEAN *Food_State)
{
	BOOLEAN IsFalling = *Food_State;
	//static BOOLEAN defaultValSet = SCI_FALSE;
	IMGREF_SIZE_T bg_window;
	FIX16_POINT_T	offset;
	COLOR32	*up_buf;
	int velocity = 1;
	int Y_Stop_Begin = 35*OCEAN_BG_HEIGHT/48-15,Y_Stop_Deta= OCEAN_BG_HEIGHT/8;
	
	int i = 0;//, j = 0;
	static int32 slowdown = 1;

	slowdown++;
	if(slowdown>1)
		slowdown = 0;


	bg_window.w = bg_window_w;
	bg_window.h = bg_window_h;

	if(!IsFalling)
	{
		IsFalling = SCI_TRUE;
		for(i = 0; i < FOOD_NUM; ++i)
		{
			if(SCI_FALSE == ocean_food[i].IsVisible)
			{
				ocean_food[i].pos_x = Fallingfood_x + (rand()%FOOD_X_WINDOW)-(FOOD_X_WINDOW/2);
				ocean_food[i].pos_y = Fallingfood_y + (rand()%FOOD_Y_WINDOW);
				ocean_food[i].pos_y_end = Y_Stop_Begin + (rand()%Y_Stop_Deta);
				ocean_food[i].IsVisible = (0 == (rand()%3) ) ? SCI_TRUE: SCI_FALSE;
				if(ocean_food[i].pos_y > OCEAN_BG_HEIGHT && ocean_food[i].pos_y < 0&& ocean_food[i].pos_x<0&&ocean_food[i].pos_x>OCEAN_BG_WIDTH)
				{
					ocean_food[i].IsVisible = SCI_FALSE;
				}
				ocean_food[i].food_type = FOOD;
			}
		}
	}
	else
	{
		for(i = 0; i < FOOD_NUM; ++i)
		{
			if(SCI_TRUE == ocean_food[i].IsVisible)
			{
				if(ocean_food[i].pos_y <= OCEAN_BG_HEIGHT && ocean_food[i].pos_y >= 0&& ocean_food[i].pos_x>=0&&ocean_food[i].pos_x<=OCEAN_BG_WIDTH)
				{					
					if(ocean_food[i].pos_y_end > ocean_food[i].pos_y)//random stop
					{
						if(!slowdown)
						{
							ocean_food[i].pos_y += velocity;
						}
					}
				}
				else
				{
					ocean_food[i].IsVisible = SCI_FALSE;
				}
			}
			
		}
	}
	for(i = 0; i < FOOD_NUM; ++i)
	{
		if(SCI_TRUE == ocean_food[i].IsVisible)
		{
			if(ocean_food[i].pos_y <= OCEAN_BG_HEIGHT)
			{
				offset.x = ocean_food[i].pos_x;
				offset.y = ocean_food[i].pos_y;
				offset.x_dec = 0;
				offset.y_dec = 0;
				up_buf = ocean_obj[ocean_food[i].food_type].srcARGB_Map;
				if(!S2D_Blending(bg_window, bg_buf, ocean_obj[ocean_food[i].food_type].up_window,offset, up_buf, out_buf) )
				{
					//SCI_TRACE_LOW:"Food buffer failed"
					SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_LIVEWALLPAPER_AQUARIUM_1083_112_2_18_2_31_55_22,(uint8*)"");
					//MessageBox(NULL, "Food buffer failed", "Error", MB_OK);
					return SCI_FALSE;
				}
			}
		}
	}

	*Food_State = IsFalling;
	return SCI_TRUE;
}
LOCAL BOOLEAN FishSwimming(	
	const OCEAN_OBJ_T *ocean_obj,
	OCEAN_FISH_T *ocean_fish,
	OCEAN_FOOD_T *ocean_food,
	uint16 bg_window_w, 
	uint16 bg_window_h,
	COLOR16 *bg_buf, 
	COLOR16	*out_buf)
{
	COLOR32	*up_buf;
	//FIX16_POINT_T	Fish_offset;
	int i = 0,m = 0, n = 0;
	IMGREF_SIZE_T bg_window;
	
	int buf_change_deta = 8;
	int buf_change_begin = 0;
	int buf_change_end = 0;
	//OCEAN_TYPE_E fish_state_tmp;
	IMGREF_POINT_T window_mid;//rotation surround point
	IMGREF_POINT_T img_mid;//deta between rotation surround point and up window
	int32 angle;
	BOOLEAN DoAngleMod = SCI_TRUE;
	int32 tmp_angle;
	//int32 step_angle = 50;//5 degree
	int32 local_food_x,local_food_y;//GotFoodRadius = 8,zero_sz = 0;//food_x1,	
	//FIX16_POINT_T FishLip;
	FIX16_POINT_T	offset;
	FIX16_POINT_T	pre_offset;
	IMGREF_POINT_T pre_window_mid;
	int32 food_index = 0;
	int32 food_fish_distance;
	int32 food_fish_MIN_Limit = 6400;//80
	int32 food_fish_MIN = 0x7fffffff;
	

	//static BOOLEAN defaultValSet = SCI_FALSE;
	static int32 buf_change = 0;
	BOOLEAN ShouldCatchFood = SCI_TRUE;
	//static int32 pre_angle = 0x7fffffff;
	//static BOOLEAN doAngleCtrl = SCI_TRUE;
	BOOLEAN IsReversed = SCI_FALSE;
	
	uint16 tmp_width,tmp_height;
	int32 ori_coe;//orientation coefficient
	COLOR32	*dst_ptr= PNULL;
	COLOR32	*fill_ptr= PNULL;
	static int32 slowdown = 3;

	int32 DampDist=0;

	slowdown++;
	if(slowdown>0)
		slowdown = 0;
	
	
	bg_window.w = bg_window_w;
	bg_window.h = bg_window_h;	

	//change buf for dynamic
	buf_change = buf_change+1;

	if(buf_change>(buf_change_begin+buf_change_end+buf_change_deta*4-1))
	{
		buf_change = 0;
	}

	for(i = 0; i < FISH_NUM; ++i)
	{	
		//dynamic pic change
		if(buf_change<buf_change_begin+buf_change_deta)
		{
			ocean_fish[i].State = ocean_fish[i].fish_type;
		}
		else if(buf_change<buf_change_begin+buf_change_deta*2)
		{
			ocean_fish[i].State = (OCEAN_TYPE_E)(ocean_fish[i].fish_type+1);
		}
		else if(buf_change<buf_change_begin+buf_change_deta*3)
		{
			ocean_fish[i].State = (OCEAN_TYPE_E)(ocean_fish[i].fish_type+2);
		}
		else if(buf_change<buf_change_begin+buf_change_end+buf_change_deta*4)
		{
			ocean_fish[i].State = (OCEAN_TYPE_E)(ocean_fish[i].fish_type+3);
		}
		else
		{
			ocean_fish[i].State = ocean_fish[i].fish_type;
		}		
		up_buf = ocean_obj[ocean_fish[i].State].srcARGB_Map;
		if(PNULL == up_buf)
		{
			return SCI_FALSE;
		}

		//reverse pic if needed
		if((ocean_fish[i].offset.x+ocean_obj[ocean_fish[i].fish_type].up_window.w/2) > ocean_fish[i].endPos.x)
		{
			IsReversed = SCI_TRUE;
			tmp_width = ocean_obj[ocean_fish[i].fish_type].up_window.w;
			tmp_height = ocean_obj[ocean_fish[i].fish_type].up_window.h;
            
                    if(PNULL == Fish_srcARGB_temp)
                    {
                        return SCI_FALSE;
                    }
                        
			for(m = 0; m < tmp_height; ++m)
			{
				dst_ptr = &(Fish_srcARGB_temp[m*tmp_width]);
				fill_ptr =&(up_buf[m*tmp_width + tmp_width-1]);
				for(n = 0; n < tmp_width; ++n)
				{
					*dst_ptr = *fill_ptr;
					dst_ptr++ ;
					fill_ptr--;
				}
			}
		}
		else
		{
			IsReversed = SCI_FALSE;
			tmp_width = ocean_obj[ocean_fish[i].fish_type].up_window.w;
			tmp_height = ocean_obj[ocean_fish[i].fish_type].up_window.h;

                    if(PNULL == Fish_srcARGB_temp)
                    {
                        return SCI_FALSE;
                    }

			for(m = 0; m < tmp_height; ++m)
			{
				dst_ptr = &(Fish_srcARGB_temp[m*tmp_width]);
				fill_ptr =&(up_buf[m*tmp_width]);
				for(n = 0; n < tmp_width; ++n)
				{
					*dst_ptr = *fill_ptr;
					dst_ptr++ ;
					fill_ptr++;
				}
			}
		}
		up_buf = Fish_srcARGB_temp;
		ori_coe = IsReversed ? (-1): (1);

		img_mid.x = IsReversed ? 0: (ocean_obj[ocean_fish[i].fish_type].up_window.w);
		img_mid.y = ((ocean_obj[ocean_fish[i].fish_type].up_window.h)>>1);
		pre_window_mid.x = ocean_fish[i].offset.x+img_mid.x;
		pre_window_mid.y = ocean_fish[i].offset.y+img_mid.y;
		
		ShouldCatchFood = SCI_FALSE;
        
		if(ocean_fish[i].HitFrame>=ocean_fish[i].HitFrameMAX)
		{
			ocean_fish[i].base_Damp_offset.x = ocean_fish[i].offset.x;
			ocean_fish[i].base_Damp_offset.y = ocean_fish[i].offset.y;

			ocean_fish[i].HitFrame = ocean_fish[i].HitFrameMAX;
			
			DampDist = MMK_ClcltDampPlayOffset(DAMP_PLAY_TYPE_1,ocean_fish[i].EscapeDist,ocean_fish[i].HitFrameMAX+1,ocean_fish[i].HitFrameMAX-ocean_fish[i].HitFrame+1)+ocean_fish[i].Velocity*(ocean_fish[i].HitFrameMAX-ocean_fish[i].HitFrame+1);

			angle = ocean_fish[i].pre_angle;
			angle = IsReversed ? (angle+1800): (angle);			
			ocean_fish[i].offset.x = offset.x = ocean_fish[i].base_Damp_offset.x + (((s2d_cos_new(angle))*DampDist)/65536);
			ocean_fish[i].offset.y = offset.y = ocean_fish[i].base_Damp_offset.y + (((s2d_sin_new(angle))*DampDist)/65536);
			//if(1==i)
			//	SCI_TRACE_LOW("Ben---start-MMK_ClcltDampPlayOffset--ocean_fish[i].offset.x: %d, ocean_fish[i].HitFrame : %d, DampDist: %d",ocean_fish[i].offset.x,ocean_fish[i].HitFrame,DampDist);
			window_mid.x = offset.x+img_mid.x;
			window_mid.y = offset.y+img_mid.y;
			angle = IsReversed ? (angle-1800): (angle);
			/*while(1)
			 {
			 	if(angle>1800)
					angle -= 3600;
				else if(angle <-1800)
					angle += 3600;
				else
					break;
			 }*/
			 DoAngleMod = SCI_TRUE;
			 while(DoAngleMod)
			 {
			 	if(angle>1800)
			 	{
					angle -= 3600;
					DoAngleMod = SCI_TRUE;
			 	}
				else if(angle <-1800)
				{
					angle += 3600;
					DoAngleMod = SCI_TRUE;
			 	}
				else
				{
					DoAngleMod = SCI_FALSE;
				}
			 }
			 
			S2d_rotation_slim(bg_window,ocean_obj[ocean_fish[i].State].up_window,window_mid,img_mid,angle,up_buf,out_buf,4096,s_Point_Add_ptr);
			ocean_fish[i].HitFrame--;
			ocean_fish[i].doAngleCtrl = SCI_TRUE;
			
			/*angle = ocean_fish[i].pre_angle;
			angle = IsReversed ? (angle+1800): (angle);			
			ocean_fish[i].offset.x = offset.x = ocean_fish[i].offset.x + (((s2d_cos(angle))*(ocean_fish[i].Velocity+2))/65536);
			ocean_fish[i].offset.y = offset.y = ocean_fish[i].offset.y + (((s2d_sin(angle))*(ocean_fish[i].Velocity+2))/65536);
			window_mid.x = offset.x+img_mid.x;
			window_mid.y = offset.y+img_mid.y;
			angle = IsReversed ? (angle-1800): (angle);
			while(1)
			 {
			 	if(angle>1800)
					angle -= 3600;
				else if(angle <-1800)
					angle += 3600;
				else
					break;
			 }
			S2d_rotation_slim(bg_window,ocean_obj[ocean_fish[i].State].up_window,window_mid,img_mid,angle,up_buf,out_buf,4096,s_Point_Add_ptr);
			ocean_fish[i].HitFrame = ocean_fish[i].HitFrameMAX-1;
			ocean_fish[i].doAngleCtrl = SCI_TRUE;*/
			//SCI_FREE(ocean_obj_buf.srcARGB_Map);
			//return SCI_TRUE;
			continue;
		}
		else if(ocean_fish[i].HitFrame>0)
		{
			DampDist = MMK_ClcltDampPlayOffset(DAMP_PLAY_TYPE_1,ocean_fish[i].EscapeDist,ocean_fish[i].HitFrameMAX+1,ocean_fish[i].HitFrameMAX-ocean_fish[i].HitFrame+1)+ocean_fish[i].Velocity*(ocean_fish[i].HitFrameMAX-ocean_fish[i].HitFrame+1);

			angle = ocean_fish[i].pre_angle;
			angle = IsReversed ? (angle+1800): (angle);			
			ocean_fish[i].offset.x = offset.x = ocean_fish[i].base_Damp_offset.x + (((s2d_cos_new(angle))*DampDist)/65536);
			ocean_fish[i].offset.y = offset.y = ocean_fish[i].base_Damp_offset.y + (((s2d_sin_new(angle))*DampDist)/65536);
			//if(1==i)
				//SCI_TRACE_LOW("Ben---MMK_ClcltDampPlayOffset--ocean_fish[i].offset.x: %d, ocean_fish[i].HitFrame : %d, DampDist: %d",ocean_fish[i].offset.x,ocean_fish[i].HitFrame,DampDist);

			window_mid.x = offset.x+img_mid.x;
			window_mid.y = offset.y+img_mid.y;
			angle = IsReversed ? (angle-1800): (angle);

			DoAngleMod = SCI_TRUE;
			 while(DoAngleMod)
			 {
			 	if(angle>1800)
			 	{
					angle -= 3600;
					DoAngleMod = SCI_TRUE;
			 	}
				else if(angle <-1800)
				{
					angle += 3600;
					DoAngleMod = SCI_TRUE;
			 	}
				else
				{
					DoAngleMod = SCI_FALSE;
				}
			 }
			 
			S2d_rotation_slim(bg_window,ocean_obj[ocean_fish[i].State].up_window,window_mid,img_mid,angle,up_buf,out_buf,4096,s_Point_Add_ptr);
			ocean_fish[i].HitFrame--;
			ocean_fish[i].doAngleCtrl = SCI_TRUE;
			continue;
		}
		
		food_fish_MIN = 0x7fffffff;
		if(FISH_3 != ocean_fish[i].fish_type)
		{
			for(m = 0;m < FOOD_NUM; ++m)
			{
				if(ocean_food[m].IsVisible)
				{				
					local_food_x = ocean_food[m].pos_x+((ocean_obj[ocean_food[m].food_type].up_window.w)>>1);
					local_food_y = ocean_food[m].pos_y+((ocean_obj[ocean_food[m].food_type].up_window.h)>>1);
					if(((ori_coe*local_food_x)>=(ori_coe*pre_window_mid.x))//&&(abs(pre_window_mid.y - food_y)<(OCEAN_BG_HEIGHT/2))						
						&&((abs(pre_window_mid.y - local_food_y))<=(abs(pre_window_mid.x - local_food_x)*17/10)))
					{
						food_fish_distance = (local_food_x - pre_window_mid.x)*(local_food_x - pre_window_mid.x)+(local_food_y - pre_window_mid.y)*(local_food_y - pre_window_mid.y);
						if(food_fish_MIN_Limit>food_fish_distance
							&&food_fish_MIN>food_fish_distance)
						{
							food_index = m;
							ShouldCatchFood = SCI_TRUE;
							food_fish_MIN = food_fish_distance;
						}
					}
				}
			}
		}
		//ShouldCatchFood = SCI_FALSE;
		
		if(ShouldCatchFood)// do catch food
		{
			n = food_index;
			local_food_x = ocean_food[n].pos_x+ocean_obj[ocean_food[n].food_type].up_window.w/2;
			local_food_y = ocean_food[n].pos_y+ocean_obj[ocean_food[n].food_type].up_window.h/2;
			if((ocean_food[n].pos_y<ocean_food[n].pos_y_end)
				&&(ocean_fish[i].offset.y>ocean_food[n].pos_y))
			{
				local_food_y += 2;
			}
			angle = atan_degree10((local_food_x -pre_window_mid.x), (local_food_y -pre_window_mid.y));
			offset.x = ocean_fish[i].offset.x + (((s2d_cos_new(angle))*(ocean_fish[i].Velocity+2))/65536);
			offset.y = ocean_fish[i].offset.y + (((s2d_sin_new(angle))*(ocean_fish[i].Velocity+2))/65536);
		}
		else// do normal swimming
		{
			n = food_index;
			local_food_x = ocean_fish[i].endPos.x;
			local_food_y = ocean_fish[i].endPos.y;
			angle = atan_degree10((local_food_x -pre_window_mid.x), (local_food_y -pre_window_mid.y));
			offset.x = ocean_fish[i].offset.x + (((s2d_cos_new(angle))*ocean_fish[i].Velocity)/65536);
			offset.y = ocean_fish[i].offset.y + (((s2d_sin_new(angle))*ocean_fish[i].Velocity)/65536);
		}		
		
		
		 if(0 != slowdown)
		 {
		 	angle = ocean_fish[i].pre_angle;
			window_mid.x = ocean_fish[i].offset.x+img_mid.x;
			window_mid.y = ocean_fish[i].offset.y+img_mid.y;
			/*while(1)
			 {
			 	if(angle>1800)
					angle -= 3600;
				else if(angle <-1800)
					angle += 3600;
				else
					break;
			 }*/
			 DoAngleMod = SCI_TRUE;
			 while(DoAngleMod)
			 {
			 	if(angle>1800)
			 	{
					angle -= 3600;
					DoAngleMod = SCI_TRUE;
			 	}
				else if(angle <-1800)
				{
					angle += 3600;
					DoAngleMod = SCI_TRUE;
			 	}
				else
				{
					DoAngleMod = SCI_FALSE;
				}
			 }
		 }
		 else
		 {
			window_mid.x = offset.x+img_mid.x;
			window_mid.y = offset.y+img_mid.y;

			if(0 >= (local_food_x-window_mid.x)*(local_food_x-pre_window_mid.x))
			{
				window_mid.x = local_food_x;
			}			
				
			if(0 >= (local_food_y-window_mid.y)*(local_food_y-pre_window_mid.y))
			{
				window_mid.y = local_food_y;
			}
			
			pre_offset.x = ocean_fish[i].offset.x;
			pre_offset.y = ocean_fish[i].offset.y;
			ocean_fish[i].offset.x = window_mid.x - img_mid.x;
			ocean_fish[i].offset.y = window_mid.y - img_mid.y;

			angle = atan_degree10((ocean_fish[i].offset.x - pre_offset.x), (ocean_fish[i].offset.y - pre_offset.y));

			angle = IsReversed ? (angle-1800): (angle);
			
			if(0x7fffffff == ocean_fish[i].pre_angle)
			 {
			 	ocean_fish[i].pre_angle = angle;
			 }
			if(ocean_fish[i].doAngleCtrl)
			{
				tmp_angle = angle-ocean_fish[i].pre_angle;
				if(abs(tmp_angle)>1800)
				{
					
					if(tmp_angle > 0)
						tmp_angle -= 3600;
					else
						tmp_angle += 3600;
				}
				
				 if(abs(tmp_angle)>ocean_fish[i].angleStep)
				 {
				 	if(tmp_angle > 0)
						angle = ocean_fish[i].pre_angle+ocean_fish[i].angleStep;
					else
						angle = ocean_fish[i].pre_angle-ocean_fish[i].angleStep;
				 }
			}
			ocean_fish[i].doAngleCtrl = SCI_TRUE;
			 ocean_fish[i].pre_angle = angle;

			 DoAngleMod = SCI_TRUE;
			 while(DoAngleMod)
			 {
			 	if(angle>1800)
			 	{
					angle -= 3600;
					DoAngleMod = SCI_TRUE;
			 	}
				else if(angle <-1800)
				{
					angle += 3600;
					DoAngleMod = SCI_TRUE;
			 	}
				else
				{
					DoAngleMod = SCI_FALSE;
				}
			 }
		 }

		 //if(1==i)
		 	//SCI_TRACE_LOW("Ben-----ocean_fish[i].offset.x: %d",ocean_fish[i].offset.x);
		S2d_rotation_slim(bg_window,ocean_obj[ocean_fish[i].State].up_window,window_mid,img_mid,angle,up_buf,out_buf,4096,s_Point_Add_ptr);
		if( (window_mid.x == local_food_x) && (window_mid.y == local_food_y))
		{
			if(ShouldCatchFood)
			{
				ocean_food[n].IsVisible = FALSE;
			}
			else
			{//if the fish arrived at the end, then set the end point to another side.
				if(ocean_fish[i].EndPos_X_min== ocean_fish[i].endPos.x)
				{
					ocean_fish[i].endPos.x = ocean_fish[i].EndPos_X_max;					
				}
				else
				{
					ocean_fish[i].endPos.x = ocean_fish[i].EndPos_X_min;
				}
				ocean_fish[i].endPos.y = (rand()%(ocean_fish[i].EndPos_Y_max-ocean_fish[i].EndPos_Y_min))+ocean_fish[i].EndPos_Y_min;
				ocean_fish[i].doAngleCtrl = SCI_FALSE;
			}
			
		}

	}
	return SCI_TRUE;
}

LOCAL void Hit_PointProc(const GUI_POINT_T *PointDown)
{
	int32 i = 0;
	BOOLEAN IsHitFish = SCI_FALSE;

    if(PNULL == s_ocean_obj || PNULL == s_ocean_food || PNULL == s_ocean_fish|| PNULL == s_ocean_sunshine || PNULL == s_ocean_bubble)
    {
        return;
    }
      
	for(i = 0; i < FISH_NUM; ++i)
	{
		if(PointDown->x >= s_ocean_fish[i].offset.x
			&&(PointDown->x <= (s_ocean_fish[i].offset.x+s_ocean_obj[s_ocean_fish[i].fish_type].up_window.w))
			&&PointDown->y >= s_ocean_fish[i].offset.y
			&&(PointDown->y <= (s_ocean_fish[i].offset.y+s_ocean_obj[s_ocean_fish[i].fish_type].up_window.h)))
		{
			s_ocean_fish[i].HitFrame = s_ocean_fish[i].HitFrameMAX;
			IsHitFish = SCI_TRUE;
		}
	}
	
	if(!IsHitFish)
	{
		/*if(PointDown->x >= (OCEAN_BG_WIDTH/16)
			&&(PointDown->x <= (OCEAN_BG_WIDTH*15/16))
			&&PointDown->y >= (OCEAN_BG_HEIGHT/24)
			&&(PointDown->y <= (OCEAN_BG_HEIGHT/3)))
		{
			FoodState = SCI_FALSE; // if foodstate is false,FoodFallingDown() will set it fo true and set some food particles;
			food_x = PointDown->x;
			food_y = PointDown->y;
		}*/
		if(PointDown->x >= (0)
			&&(PointDown->x <= (OCEAN_BG_WIDTH))
			&&PointDown->y >= (0)
			&&(PointDown->y <= (OCEAN_BG_HEIGHT/2)))
		{
			FoodState = SCI_FALSE; // if foodstate is false,FoodFallingDown() will set it fo true and set some food particles;
			food_x = PointDown->x;
			food_y = PointDown->y;
		}
	}
}

LOCAL int atan_degree10(int dx, int dy)//from -90 to 90
{
	int tmp;
	uint32 tmp_angle;
	int x,y;
	x = dx;
	y = dy;
	
	if((0 == y) && (0 <= x))
	{
		return 0;
	}
	else if((0 < y) && (0 == x))
	{
		return 900;
	}
	else if((0 > y) && (0 == x))
	{
		return 2700;
	}
	else if((0 == y) && (0 > x))
	{
		return 1800;
	}
	x = abs(dx);
	y = abs(dy);
		
	tmp = (y<<6)/x;

	if(tmp >= 256)
		tmp = 255;

	tmp_angle = atan_table[tmp];

	if(0>dx&&0<dy)
	{
		tmp_angle = 1800 - tmp_angle;
	}
	else if(0>dx&&0>dy)
	{
		tmp_angle = 1800 + tmp_angle;
	}
	else if(0<dx&&0>dy)
	{
		tmp_angle = 3600 - tmp_angle;
	}

	return tmp_angle;
	
}
/*****************************************************************************/
//  Description : 获得动态壁纸类型
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL uint32 AquariumGetType(void)
{
    return MMI_LIVE_WALLPAPER_TYPE_AQUARIUM;
}
/*****************************************************************************/
//  Description : 获得动态壁纸ThumbNail信息
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL void AquariumGetThumbNailInfo(LW_THUMBNAIL_INFO *thumbnail_info_ptr)
{
    if(thumbnail_info_ptr)
    {
        thumbnail_info_ptr->is_image_file = FALSE;
        thumbnail_info_ptr->image_id = IMAGE_WALLPAPER_AQUARIUM_THUMBNAIL;

        thumbnail_info_ptr->is_not_string = FALSE;
        thumbnail_info_ptr->text_id = TXT_SET_WALLPAPER_AQUARIUM;
        thumbnail_info_ptr->instance_mem_pool_size = BLOCK_MEM_POOL_SIZE_AQUARIUM;
    }
}
#endif// LIVE_WALLPAPER_SUPPORT_AQUARIUM


#endif //_MMI_LIVEWALLPAPER_AQUARIUM_C_


/*Edit by script, ignore 1 case. Thu Apr 26 19:00:57 2012*/ //IGNORE9527
