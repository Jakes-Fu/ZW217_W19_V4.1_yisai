/*****************************************************************************
** File Name:      mmi_livewallpaper_spring.c                                    *
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
#ifndef _MMI_LIVEWALLPAPER_SPRING_C_
#define _MMI_LIVEWALLPAPER_SPRING_C_

#ifdef LIVE_WALLPAPER_SUPPORT_SPRING

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
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define BLOCK_MEM_POOL_SIZE_SPRING (450 * 1024)   // Set a  MEM POOL for different livewallpaper respectively;

#define BUTTERFLY1_RES_OBJ_NUM (8)
#define BUTTERFLY2_RES_OBJ_NUM (5)
#define PETAL1_RES_OBJ_NUM (5)
//#define PETAL2_RES_OBJ_NUM (6)
#define BUTTER_FLYING_NUM (2)
#define BUTTER_SLEEPING_NUM (1)
#define BUTTER_ACTION_PREC 8

#define SPRING_PETAL_OBJ_NUM (20)

#if defined(MAINLCD_SIZE_320X480) 

#define SPRING_BG_BOTTOM_YMIN (197)
#define SPRING_BG_BOTTOM_Y_DELTA (60)

#define GRASS_BG_TOP_YMIN (191)
#define GRASS_BG_TOP_Y_DELTA (30)

#define BRANCH_YMIN (38)
#define BRANCH_Y_DELTA (80)

#define SPRING_FG1_YMIN (132)
#define SPRING_FG1_Y_DELTA (135)
#define SPRING_FG1_XMAX (-10)
#define SPRING_FG1_X_AMP 4
#define SPRING_FG1_Y_AMP 6

#define SPRING_FG2_YMIN (303)
#define SPRING_FG2_Y_DELTA (240)
#define SPRING_FG2_XMAX (190)
#define SPRING_FG2_X_AMP 8
#define SPRING_FG2_Y_AMP 16

#define SPRING_SUNSHINE1_YMIN (-60)
#define SPRING_SUNSHINE1_Y_DELTA (35)
#define SPRING_SUNSHINE1_X (-20)
#define SPRING_SUNSHINE1_X_AMP (10)
#define SPRING_SUNSHINE1_Y_AMP (15)

#define SPRING_SUNSHINE2_YMIN (-50)
#define SPRING_SUNSHINE2_X (50)
#define SPRING_SUNSHINE2_X_AMP (-12)
#define SPRING_SUNSHINE2_Y_AMP (-18)

#define ButterFly_START_X (320+30)
#define ButterFly_START_Y_MIN (150)
#define ButterFly_START_Y_DELTA (100)
#define ButterFly_END_X (-100)
#define ButterFly_END_Y_DELTA (400)

#define SLEEPING_BUTTER_X (58)
#define SLEEPING_BUTTER_Y_MIN (266)

#define PETALFLY_START_Y (-50)
#define PETALFLY_START_X (-50)
#define PETALFLY_START_X_DETLA (320+100)
#define PETALFLY_END_Y (480+50)
#define PETALFLY_END_X (-100)
#define PETALFLY_END_X_DETLA (320+200)

#elif defined(MAINLCD_SIZE_240X400) 

#define SPRING_BG_BOTTOM_YMIN (197*5/6)
#define SPRING_BG_BOTTOM_Y_DELTA (60*5/6)

#define GRASS_BG_TOP_YMIN (191*5/6)
#define GRASS_BG_TOP_Y_DELTA (30*5/6)

#define BRANCH_YMIN (38*5/6)
#define BRANCH_Y_DELTA (80*5/6)

#define SPRING_FG1_YMIN (132*5/6)
#define SPRING_FG1_Y_DELTA (135*5/6)
#define SPRING_FG1_XMAX (-10*3/4)
#define SPRING_FG1_X_AMP (4*3/4)
#define SPRING_FG1_Y_AMP (6*5/6)

#define SPRING_FG2_YMIN (303*5/6)
#define SPRING_FG2_Y_DELTA (240*5/6)
#define SPRING_FG2_XMAX (190*3/4)
#define SPRING_FG2_X_AMP (8*3/4)
#define SPRING_FG2_Y_AMP (16*5/6)

#define ButterFly_START_X (240+30)
#define ButterFly_START_Y_MIN (150*5/6)
#define ButterFly_START_Y_DELTA (100*5/6)
#define ButterFly_END_X (-100*3/4)
#define ButterFly_END_Y_DELTA (400*5/6)

#define SLEEPING_BUTTER_X (58*3/4)
#define SLEEPING_BUTTER_Y_MIN (266*5/6)

#define PETALFLY_START_Y (-50)
#define PETALFLY_START_X (-50)
#define PETALFLY_START_X_DETLA (240+100)
#define PETALFLY_END_Y (400+50)
#define PETALFLY_END_X (-100)
#define PETALFLY_END_X_DETLA (240+200)

#elif defined(MAINLCD_SIZE_240X320) 

#define SPRING_BG_BOTTOM_YMIN (197*2/3)
#define SPRING_BG_BOTTOM_Y_DELTA (60*2/3)

#define GRASS_BG_TOP_YMIN (191*2/3)
#define GRASS_BG_TOP_Y_DELTA (30*2/3)

#define BRANCH_YMIN (38*2/3)
#define BRANCH_Y_DELTA (80*2/3)

#define SPRING_FG1_YMIN (132*2/3)
#define SPRING_FG1_Y_DELTA (135*2/3)
#define SPRING_FG1_XMAX (-10*3/4)
#define SPRING_FG1_X_AMP (4*3/4)
#define SPRING_FG1_Y_AMP (6*2/3)

#define SPRING_FG2_YMIN (303*2/3)
#define SPRING_FG2_Y_DELTA (240*2/3)
#define SPRING_FG2_XMAX (190*3/4)
#define SPRING_FG2_X_AMP (8*3/4)
#define SPRING_FG2_Y_AMP (16*2/3)

#define ButterFly_START_X (240+30)
#define ButterFly_START_Y_MIN (150*2/3)
#define ButterFly_START_Y_DELTA (100*2/3)
#define ButterFly_END_X (-100*3/4)
#define ButterFly_END_Y_DELTA (400*2/3)

#define SLEEPING_BUTTER_X (58*3/4)
#define SLEEPING_BUTTER_Y_MIN (266*2/3)

#define PETALFLY_START_Y (-50)
#define PETALFLY_START_X (-50)
#define PETALFLY_START_X_DETLA (240+100)
#define PETALFLY_END_Y (320+50)
#define PETALFLY_END_X (-100)
#define PETALFLY_END_X_DETLA (240+200)

#else

#define SPRING_BG_BOTTOM_YMIN (197)
#define SPRING_BG_BOTTOM_Y_DELTA (60)

#define GRASS_BG_TOP_YMIN (191)
#define GRASS_BG_TOP_Y_DELTA (30)

#define BENCH_TOP_YMIN (230)
#define BENCH_TOP_X (78)

#define BRANCH_YMIN (38)
#define BRANCH_Y_DELTA (80)

#define SPRING_FG1_YMIN (132)
#define SPRING_FG1_Y_DELTA (135)
#define SPRING_FG1_XMAX (-10)
#define SPRING_FG1_X_AMP 4
#define SPRING_FG1_Y_AMP 6

#define SPRING_FG2_YMIN (303)
#define SPRING_FG2_Y_DELTA (240)
#define SPRING_FG2_XMAX (190)
#define SPRING_FG2_X_AMP 8
#define SPRING_FG2_Y_AMP 16

#define ButterFly_START_X (320+30)
#define ButterFly_START_Y_MIN (150)
#define ButterFly_START_Y_DELTA (100)
#define ButterFly_END_X (-100)
#define ButterFly_END_Y_DELTA (400)

#define SLEEPING_BUTTER_X (58)
#define SLEEPING_BUTTER_Y_MIN (266)

#define PETALFLY_START_Y (-50)
#define PETALFLY_START_X (-50)
#define PETALFLY_START_X_DETLA (320+100)
#define PETALFLY_END_Y (480+50)
#define PETALFLY_END_X (-100)
#define PETALFLY_END_X_DETLA (320+200)

#endif


typedef enum
{
    SPRING_BRANCH = 0,
    BG_CROSSGRASS,
    FG1,
    FG2,
    PETALS1_0,
    PETALS1_1,
    PETALS1_2,
    PETALS1_3,
    PETALS1_4,
    BUTTERFLY1_0,
    BUTTERFLY1_1,
    BUTTERFLY1_2,
    BUTTERFLY1_3,
    BUTTERFLY1_4,
    BUTTERFLY1_5,
    BUTTERFLY1_6,
    BUTTERFLY1_7,
    SPRING_RES_NUM
}SPRING_RES_E;
typedef struct
{
	OBJECT_T* pButterFly_Res;
	int32 Action_total_frame;
	int32 cur_frame;
	int32 action_step;// bits 8 prec;
	BOOLEAN IsInAction;
	//FIX16_POINT_T offset;
	FIX16_POINT_T Sta_point;
	FIX16_POINT_T End_point;
	int32 vel_x;
	int32 dist_x;
	int32 wait_num;
}BUTTERFLY_T;

typedef struct
{
	OBJECT_T* pPetal_Res;
	int32 Action_total_frame;
	int32 cur_frame;
	int32 action_step;// bits 8 prec;
	BOOLEAN IsInAction;
	//FIX16_POINT_T offset;
	FIX16_POINT_T Sta_point;
	FIX16_POINT_T End_point;
	int32 vel_y;//8 bits prec
	int32 dist_y;//8 bits prec
	int32 wait_num;
}SPRING_PETAL_T;

OBJECT_T spring_Bg_Obj = {0};
OBJECT_T Grass_Bg_Obj = {0};
OBJECT_T Cross_Grass_Bg_Obj = {0};
COLOR16* pspring_Bg = NULL;
COLOR16* pGrass_Bg = NULL;
//OBJECT_T Garden_Bench_Obj = {0};
OBJECT_T Branch_Obj = {0};
OBJECT_T spring_fg1_Obj = {0};
OBJECT_T spring_fg2_Obj = {0};

OBJECT_T ButterFly1[BUTTERFLY1_RES_OBJ_NUM] = {0};
//OBJECT_T ButterFly2[BUTTERFLY2_RES_OBJ_NUM] = {0};

OBJECT_T Petal1[PETAL1_RES_OBJ_NUM] = {0};
//OBJECT_T Petal2[PETAL2_RES_OBJ_NUM] = {0};

BUTTERFLY_T FlyingButter[BUTTER_FLYING_NUM] = {0};
BUTTERFLY_T SleepingButter[BUTTER_SLEEPING_NUM] = {0};

LOCAL SPRING_PETAL_T *s_spring_Petal = PNULL;


GUI_LCD_DEV_INFO springRes_handle[SPRING_RES_NUM];
typedef struct
{
    GUI_LCD_DEV_INFO layer_handle;
    MMI_IMAGE_ID_T  img_id;
    IMGREF_SIZE_T size;
    uint8*    buf_ptr;
    BOOLEAN is_bg;
}RESOURCE_INFO_T;

    

LOCAL RESOURCE_INFO_T s_spring_resource_info[SPRING_RES_NUM]=
{
    {{0,UILAYER_NULL_HANDLE},IMAGE_WALLPAPER_SPRING_BRANCH,{0,0},PNULL,FALSE},
    {{0,UILAYER_NULL_HANDLE},IMAGE_WALLPAPER_SPRING_BG_CROSSGRASS,{0,0},PNULL,FALSE},
    {{0,UILAYER_NULL_HANDLE},IMAGE_WALLPAPER_SPRING_FG1,{0,0},PNULL,FALSE},
    {{0,UILAYER_NULL_HANDLE},IMAGE_WALLPAPER_SPRING_FG2,{0,0},PNULL,FALSE},
    {{0,UILAYER_NULL_HANDLE},IMAGE_WALLPAPER_SPRING_PETALS1_0,{0,0},PNULL,FALSE},
    {{0,UILAYER_NULL_HANDLE},IMAGE_WALLPAPER_SPRING_PETALS1_1,{0,0},PNULL,FALSE},
    {{0,UILAYER_NULL_HANDLE},IMAGE_WALLPAPER_SPRING_PETALS1_2,{0,0},PNULL,FALSE},
    {{0,UILAYER_NULL_HANDLE},IMAGE_WALLPAPER_SPRING_PETALS1_3,{0,0},PNULL,FALSE},
    {{0,UILAYER_NULL_HANDLE},IMAGE_WALLPAPER_SPRING_PETALS1_4,{0,0},PNULL,FALSE},
    {{0,UILAYER_NULL_HANDLE},IMAGE_WALLPAPER_SPRING_BUTTERFLY1_0,{0,0},PNULL,FALSE},
    {{0,UILAYER_NULL_HANDLE},IMAGE_WALLPAPER_SPRING_BUTTERFLY1_1,{0,0},PNULL,FALSE},
    {{0,UILAYER_NULL_HANDLE},IMAGE_WALLPAPER_SPRING_BUTTERFLY1_2,{0,0},PNULL,FALSE},
    {{0,UILAYER_NULL_HANDLE},IMAGE_WALLPAPER_SPRING_BUTTERFLY1_3,{0,0},PNULL,FALSE},
    {{0,UILAYER_NULL_HANDLE},IMAGE_WALLPAPER_SPRING_BUTTERFLY1_4,{0,0},PNULL,FALSE},
    {{0,UILAYER_NULL_HANDLE},IMAGE_WALLPAPER_SPRING_BUTTERFLY1_5,{0,0},PNULL,FALSE},
    {{0,UILAYER_NULL_HANDLE},IMAGE_WALLPAPER_SPRING_BUTTERFLY1_6,{0,0},PNULL,FALSE},
    {{0,UILAYER_NULL_HANDLE},IMAGE_WALLPAPER_SPRING_BUTTERFLY1_7,{0,0},PNULL,FALSE}  
};

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/
/**--------------------------------------------------------------------------*
 **                         LOCAL DEFINITION                                 *
 **--------------------------------------------------------------------------*/

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
LOCAL BOOLEAN SPRING_Init(void);
/*****************************************************************************/
//  Description : 初始化
//  Global resource dependence : 
//  Author:arvin.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SPRING_Construct(MMI_WIN_ID_T win_id);

/*****************************************************************************/
//  Description : 准备背景数据
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SPRING_DrawBg(MMI_WIN_ID_T win_id,const GUI_LCD_DEV_INFO *dev_info_ptr);

/*****************************************************************************/
//  Description : 算法
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL void SPRING_Calculate(LW_CAL_PARAM *lw_cal_param);
/*****************************************************************************/
//  Description : 释放
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:销毁临时申请的变量
/*****************************************************************************/
LOCAL void SPRING_Destruct(void);
/*****************************************************************************/
//  Description : 交互事件处理
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SPRING_HandleEvent(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
/*****************************************************************************/
//  Description : 获得动态壁纸类型
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL uint32 SPRING_GetType(void);
/*****************************************************************************/
//  Description : 获得动态壁纸ThumbNail信息
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL void SPRING_GetThumbNailInfo(LW_THUMBNAIL_INFO *thumbnail_info_ptr);
/*****************************************************************************/
//  Description : 获得动态壁纸源数据宽高
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL void SPRING_GetWidthAndHeight(uint16 *width_ptr,uint16 *height_ptr);
/*****************************************************************************/
//  Description : 获得动态壁纸选项列表
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL OPTIONLIST *SPRING_GetOptionList(void);
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void spring55Calc(int32 frame, int32 frame_total, COLOR16* pBG, IMGREF_SIZE_T bg_window);
/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/
PUBLIC const MMI_LIVE_WALLPAPER_ITEM_INFO_T g_spring_Instance =
{
    SPRING_Init,
    SPRING_Construct,
    SPRING_DrawBg,
    SPRING_Calculate,
    SPRING_Destruct,
    SPRING_HandleEvent,
    PNULL,
    SPRING_GetType,
    SPRING_GetThumbNailInfo,
    SPRING_GetWidthAndHeight,
    PNULL,
    PNULL,
    SPRING_GetOptionList
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
LOCAL BOOLEAN SPRING_Init(void)
{
    return TRUE;
}
/*****************************************************************************/
//  Description : 创建layer
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SPRING_CreateLayer(MMI_WIN_ID_T win_id,RESOURCE_INFO_T *img_info)
{
    UILAYER_CREATE_T create_info = {0}; 
    GUI_POINT_T display_point = {0,0};
    UILAYER_INFO_T layer_info = {0};
    BOOLEAN result = TRUE;

    UILAYER_RELEASELAYER(&img_info->layer_handle);
    
    /*先创建一个层*/
    img_info->layer_handle.lcd_id = 0;
    img_info->layer_handle.block_id = UILAYER_NULL_HANDLE;

    create_info.lcd_id = GUI_MAIN_LCD_ID;
    create_info.owner_handle = (MMI_HANDLE_T)win_id;
    create_info.offset_x = 0;
    create_info.offset_y = 0;
    GUIRES_GetImgWidthHeight((uint16*)&create_info.width, (uint16*)&create_info.height,img_info->img_id, win_id);
    create_info.is_bg_layer = img_info->is_bg;
    create_info.is_static_layer = FALSE; 
    create_info.func_ptr = MMIAPILIVEWALLPAPER_UilayerMemPoolFunc(); 
    
    if(UILAYER_RESULT_SUCCESS != UILAYER_CreateLayer(
    					&create_info,
    					&img_info->layer_handle
    					))
    {
        return FALSE;
    }
    
    // 设置color key
    UILAYER_SetLayerColorKey(&img_info->layer_handle, TRUE, UILAYER_TRANSPARENT_COLOR);

    // 使用color清除层
    UILAYER_Clear(&img_info->layer_handle);

    //把图片画到层上
    GUIRES_DisplayImg(&display_point,
    				PNULL,
    				PNULL,
    				win_id,
    				img_info->img_id,
    				(const GUI_LCD_DEV_INFO*)&img_info->layer_handle);
    
    //获取图片的信息
    UILAYER_GetLayerInfo(&img_info->layer_handle, &layer_info);
    
    img_info->size.w = layer_info.layer_width;
    img_info->size.h = layer_info.layer_height;
    
    //获取图片buf
    img_info->buf_ptr = UILAYER_GetLayerBufferPtr(&img_info->layer_handle);    
    
    return result;
}
/*****************************************************************************/
//  Description : 初始化
//  Global resource dependence : 
//  Author:arvin.wu
//  Note:
/*****************************************************************************/
LOCAL void SPRING_Resetbufptr(void)
{
    int32 i = 0;
    
    Cross_Grass_Bg_Obj.srcARGB_Map = PNULL;
    Branch_Obj.srcARGB_Map = PNULL;
    spring_fg1_Obj.srcARGB_Map = PNULL;
    spring_fg2_Obj.srcARGB_Map = PNULL;
    for(i= 0; i < BUTTERFLY1_RES_OBJ_NUM; i++)
    {
        ButterFly1[i].srcARGB_Map = PNULL;
    }


    for(i= 0; i < PETAL1_RES_OBJ_NUM; i++)
    {
        Petal1[i].srcARGB_Map = PNULL;
    }
    pGrass_Bg = PNULL;
    pspring_Bg = PNULL;
    
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:arvin.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SPRING_IsBufPtrNull(void)
{
    int32 i = 0;
    
    if(Cross_Grass_Bg_Obj.srcARGB_Map == PNULL
    ||Branch_Obj.srcARGB_Map == PNULL
    ||spring_fg1_Obj.srcARGB_Map == PNULL
    ||spring_fg2_Obj.srcARGB_Map == PNULL
    ||pGrass_Bg == PNULL
    ||pspring_Bg == PNULL
    )
    {
        return TRUE;
    }

    for(i= 0; i < BUTTERFLY1_RES_OBJ_NUM; i++)
    {
        if(ButterFly1[i].srcARGB_Map == PNULL)
        {
            return TRUE;
        }
    }


    for(i= 0; i < PETAL1_RES_OBJ_NUM; i++)
    {
        if(Petal1[i].srcARGB_Map == PNULL)
        {
            return TRUE;
        }
    }

    return FALSE;
}
/*****************************************************************************/
//  Description : 初始化
//  Global resource dependence : 
//  Author:arvin.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SPRING_Construct(MMI_WIN_ID_T win_id)
{
    BOOLEAN b_result = TRUE;
    int32 i = 0;

    do
    {
        //initial spring_Petal;
        if(PNULL != s_spring_Petal)
        {
            SCI_TRACE_LOW("SPRING_Construct, have a exception before Construct: PNULL != s_spring_Petal ");
            MMILIVEWALLPAPER_FREE(s_spring_Petal);
            s_spring_Petal = PNULL;
        }
        s_spring_Petal = MMILIVEWALLPAPER_ALLOC((sizeof(SPRING_PETAL_T))*SPRING_PETAL_OBJ_NUM);
        if(PNULL == s_spring_Petal)
        {
	    	b_result = FALSE;
            break;
        }
        
        for(i = 0; i< SPRING_RES_NUM; ++i)
        {
           b_result = SPRING_CreateLayer(win_id,&s_spring_resource_info[i]);

           if(!b_result)
            {
				b_result = FALSE;
                break;
            }
        }
    }while(0);
        
    if(!b_result)
    {
        //in the case of failure to allocate memory,we need to release all the mems allocated. 
        SCI_TRACE_LOW("SPRING_Construct, have a exception at allocating process in Construct() ");
        SPRING_Destruct();
    }
    
    Cross_Grass_Bg_Obj.srcARGB_Map = (uint32*)s_spring_resource_info[BG_CROSSGRASS].buf_ptr;
    Cross_Grass_Bg_Obj.up_window.w = s_spring_resource_info[BG_CROSSGRASS].size.w;
    Cross_Grass_Bg_Obj.up_window.h = s_spring_resource_info[BG_CROSSGRASS].size.h;

    //Garden_Bench_Obj.srcARGB_Map = s_spring_resource_info[SPRING_BENCH].buf_ptr;
    //Garden_Bench_Obj.up_window.w = s_spring_resource_info[SPRING_BENCH].size.w;
   // Garden_Bench_Obj.up_window.h = s_spring_resource_info[SPRING_BENCH].size.h;
    Branch_Obj.srcARGB_Map = (uint32*)s_spring_resource_info[SPRING_BRANCH].buf_ptr;
    Branch_Obj.up_window.w = s_spring_resource_info[SPRING_BRANCH].size.w;
    Branch_Obj.up_window.h = s_spring_resource_info[SPRING_BRANCH].size.h;

     spring_fg1_Obj.srcARGB_Map = (uint32*)s_spring_resource_info[FG1].buf_ptr;
    spring_fg1_Obj.up_window.w = s_spring_resource_info[FG1].size.w;
    spring_fg1_Obj.up_window.h = s_spring_resource_info[FG1].size.h;

    spring_fg2_Obj.srcARGB_Map = (uint32*)s_spring_resource_info[FG2].buf_ptr;
    spring_fg2_Obj.up_window.w = s_spring_resource_info[FG2].size.w;
    spring_fg2_Obj.up_window.h = s_spring_resource_info[FG2].size.h;

	
    for(i= 0; i < BUTTERFLY1_RES_OBJ_NUM; ++i)
    {
        ButterFly1[i].srcARGB_Map = (uint32*)s_spring_resource_info[BUTTERFLY1_0+i].buf_ptr;
        ButterFly1[i].up_window.w = s_spring_resource_info[BUTTERFLY1_0+i].size.w;
        ButterFly1[i].up_window.h = s_spring_resource_info[BUTTERFLY1_0+i].size.h;
    }

    /*for(i= 0; i < BUTTERFLY2_RES_OBJ_NUM; ++i)
    {
        ButterFly2[i].srcARGB_Map = s_spring_resource_info[BUTTERFLY2_0+i].buf_ptr;
        ButterFly2[i].up_window.w = s_spring_resource_info[BUTTERFLY2_0+i].size.w;
        ButterFly2[i].up_window.h = s_spring_resource_info[BUTTERFLY2_0+i].size.h;
    }*/

    for(i= 0; i < PETAL1_RES_OBJ_NUM; ++i)
    {
        Petal1[i].srcARGB_Map = (uint32*)s_spring_resource_info[PETALS1_0+i].buf_ptr;
        Petal1[i].up_window.w = s_spring_resource_info[PETALS1_0+i].size.w;
        Petal1[i].up_window.h = s_spring_resource_info[PETALS1_0+i].size.h;
    }

    for(i= 0; i < BUTTER_FLYING_NUM; ++i)
    {
        FlyingButter[i].IsInAction = FALSE;
        FlyingButter[i].wait_num = rand()%200;
    }

    for(i= 0; i < BUTTER_SLEEPING_NUM; ++i)
    {
        SleepingButter[i].IsInAction = FALSE;
    }

    if(PNULL == s_spring_Petal)
    {
        return FALSE;
    }
    for(i= 0; i < SPRING_PETAL_OBJ_NUM; ++i)
    {            
        s_spring_Petal[i].pPetal_Res = Petal1;
        s_spring_Petal[i].Action_total_frame = PETAL1_RES_OBJ_NUM;
        s_spring_Petal[i].cur_frame = (rand()%s_spring_Petal[i].Action_total_frame)<<BUTTER_ACTION_PREC;
        s_spring_Petal[i].action_step = (1<<(BUTTER_ACTION_PREC-2))+(1<<(BUTTER_ACTION_PREC-3))*(rand()%100)/100;
        s_spring_Petal[i].IsInAction = TRUE;

        s_spring_Petal[i].Sta_point.x = PETALFLY_START_X + rand()%PETALFLY_START_X_DETLA;
        s_spring_Petal[i].Sta_point.y = PETALFLY_START_Y;
        s_spring_Petal[i].End_point.x = PETALFLY_END_X + rand()%PETALFLY_END_X_DETLA;
        s_spring_Petal[i].End_point.y = PETALFLY_END_Y;

        s_spring_Petal[i].vel_y = 5<<8;
        s_spring_Petal[i].dist_y = (rand()%(s_spring_Petal[i].End_point.y-s_spring_Petal[i].Sta_point.y))<<BUTTER_ACTION_PREC;
        s_spring_Petal[i].wait_num = 0;//rand()%10;
    }
    
    return b_result;
}

/*****************************************************************************/
//  Description : 准备背景数据
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SPRING_DrawBg(MMI_WIN_ID_T win_id,const GUI_LCD_DEV_INFO *dev_info_ptr)
{
    BOOLEAN b_result1 = FALSE;
    BOOLEAN b_result2 = FALSE;
    uint16 width_grass = 0; //两幅图的宽度是一样的
    uint16 width_spring = 0; //两幅图的宽度是一样的
    uint16 height_grass = 0;
    uint16 height_spring = 0;
    GUI_POINT_T display_point = {0,0};
    
    GUIRES_GetImgWidthHeight((uint16*)&width_grass, (uint16*)&height_grass,IMAGE_WALLPAPER_SPRING_BG_GRASS, win_id);
    GUIRES_GetImgWidthHeight((uint16*)&width_spring, (uint16*)&height_spring,IMAGE_WALLPAPER_SPRING_BG_SPRING, win_id);

    spring_Bg_Obj.up_window.w = width_spring;
    spring_Bg_Obj.up_window.h = height_spring;
    Grass_Bg_Obj.up_window.w = width_grass;
    Grass_Bg_Obj.up_window.h = height_grass;

    if(PNULL == dev_info_ptr)
    {
        return FALSE;
    }
    else
    {
        b_result1 = GUIRES_DisplayImg(&display_point, 
                                            PNULL, 
                                            PNULL, 
                                            win_id, 
                                            IMAGE_WALLPAPER_SPRING_BG_SPRING, 
                                            dev_info_ptr);
        display_point.y += height_spring;
        
        b_result2 = GUIRES_DisplayImg(&display_point, 
                                            PNULL, 
                                            PNULL, 
                                            win_id, 
                                            IMAGE_WALLPAPER_SPRING_BG_GRASS, 
                                            dev_info_ptr);                                   
    }
    
    pspring_Bg = (uint16*)UILAYER_GetLayerBufferPtr(dev_info_ptr);
    pGrass_Bg = pspring_Bg + width_grass*height_spring;
    
    return (b_result1||b_result2); 
}

/*****************************************************************************/
//  Description : 算法
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL void SPRING_Calculate(LW_CAL_PARAM *lw_cal_param)
{
    int32 frame_spring = 0,frame_spring_total=3600;
    int32 page_total_move = 0;
    COLOR16 *target_buf_ptr = PNULL;
    IMGREF_SIZE_T size = {0}; 
    
    if(PNULL == lw_cal_param || PNULL == lw_cal_param->dest_buf_info.buf_ptr || PNULL == lw_cal_param->src_buf_info.buf_ptr)
    {
        return;
    }
    
    if(SPRING_IsBufPtrNull())
    {
        SCI_MEMCPY(lw_cal_param->dest_buf_info.buf_ptr,lw_cal_param->src_buf_info.buf_ptr,MMI_MAINSCREEN_WIDTH*MMI_MAINSCREEN_HEIGHT*sizeof(uint16));
        return;
    }
    
    target_buf_ptr = lw_cal_param->dest_buf_info.buf_ptr;

    size.w = lw_cal_param->dest_buf_info.w;
    size.h = lw_cal_param->dest_buf_info.h;
    
    page_total_move = (lw_cal_param->fg_pos_info.rc.right - lw_cal_param->fg_pos_info.rc.left + 1-MMI_MAINSCREEN_WIDTH);

    if(page_total_move<0)
    {
        return;
    }
    else if(page_total_move == 0)
    {
        frame_spring = frame_spring_total/2;
        spring55Calc(frame_spring, frame_spring_total,target_buf_ptr,size);
    }
    else
    {
       // if(lw_cal_param->fg_pos_info.x >50)
       // {
           // lw_cal_param->fg_pos_info.x = lw_cal_param->fg_pos_info.x;
       // }
        frame_spring = lw_cal_param->fg_pos_info.x*frame_spring_total/page_total_move;
        spring55Calc(frame_spring, frame_spring_total,target_buf_ptr,size);
    }


}
/*****************************************************************************/
//  Description : 释放
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL void SPRING_Destruct(void)
{
    int32 i = 0;
    
    for(i = 0; i< SPRING_RES_NUM; ++i)
    {
       UILAYER_RELEASELAYER(&s_spring_resource_info[i].layer_handle);
    }
    
    if(PNULL != s_spring_Petal)
    {
        MMILIVEWALLPAPER_FREE(s_spring_Petal);
        s_spring_Petal = PNULL;
    }
    
    SPRING_Resetbufptr();
}

/*****************************************************************************/
//  Description : 交互事件处理
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SPRING_HandleEvent(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    return FALSE;
}
/*****************************************************************************/
//  Description : 获得动态壁纸类型
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL uint32 SPRING_GetType(void)
{
    return MMI_LIVE_WALLPAPER_TYPE_SPRING;
}
/*****************************************************************************/
//  Description : 获得动态壁纸ThumbNail信息
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL void SPRING_GetThumbNailInfo(LW_THUMBNAIL_INFO *thumbnail_info_ptr)
{
    if(thumbnail_info_ptr)
    {
        thumbnail_info_ptr->is_image_file = FALSE;
        thumbnail_info_ptr->image_id = IMAGE_WALLPAPER_SPRING_THUMBNAIL;

        thumbnail_info_ptr->is_not_string = FALSE;
        thumbnail_info_ptr->text_id = TXT_SET_WALLPAPER_SPRING;
        thumbnail_info_ptr->instance_mem_pool_size = BLOCK_MEM_POOL_SIZE_SPRING;
    }
}
/*****************************************************************************/
//  Description : 获得动态壁纸源数据宽高
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL void SPRING_GetWidthAndHeight(uint16 *width_ptr,uint16 *height_ptr)
{
    uint16 width_grass = 0; //两幅图的宽度是一样的
    uint16 width_spring = 0; //两幅图的宽度是一样的
    uint16 height_grass = 0;
    uint16 height_spring = 0;
    
    GUIRES_GetImgWidthHeight((uint16*)&width_grass, (uint16*)&height_grass,IMAGE_WALLPAPER_SPRING_BG_GRASS, MMK_GetFocusWinId());
    GUIRES_GetImgWidthHeight((uint16*)&width_spring, (uint16*)&height_spring,IMAGE_WALLPAPER_SPRING_BG_SPRING, MMK_GetFocusWinId());

    SCI_ASSERT(width_grass == width_spring);   /*assert verified*/
    
    if(PNULL != width_ptr )
    {
        *width_ptr = width_grass;
    }

    if(PNULL != height_ptr )
    {
        *height_ptr = height_grass+height_spring;
    }

}
/*****************************************************************************/
//  Description : 获得动态壁纸选项列表
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL OPTIONLIST *SPRING_GetOptionList(void)
{
    return (OPTIONLIST*)PNULL;
}



LOCAL void springBgCalc(int32 frame, int32 frame_total, COLOR16* pBG, IMGREF_SIZE_T bg_window)
{
	int32 cross_y = 0,y_max = 0, y_min = 0;
    uint32 Bg_copy_min = 0,Bg_copy_max = 0,Copy_len = 0,Pic_min = 0;
	FIX16_POINT_T cross_offset = {0};

	if(spring_Bg_Obj.up_window.w != bg_window.w
		||Grass_Bg_Obj.up_window.w != bg_window.w
		||Cross_Grass_Bg_Obj.up_window.w != bg_window.w)
	{
		return;
	}

//grass
	cross_y = frame*GRASS_BG_TOP_Y_DELTA/frame_total+GRASS_BG_TOP_YMIN;
	//oss_y = ValFromDlg_1;
	//背景坐标系，图片最大最小y
	y_min = cross_y;
	y_max = cross_y+Grass_Bg_Obj.up_window.h;
	Bg_copy_min = MAX(y_min,0);
	Bg_copy_max = MIN(y_max,bg_window.h);
	Copy_len = MAX(Bg_copy_max-Bg_copy_min,0);
	//pic在全局坐标里的左上点为零点的坐标系，背景最大最小y
	y_min = 0-y_min;
	y_max = bg_window.h+y_min;
	Pic_min = MAX(y_min,0);
	
	memcpy( pBG+(Bg_copy_min*bg_window.w), pGrass_Bg+(Pic_min*bg_window.w), bg_window.w*Copy_len*sizeof(COLOR16));

	
//spring
	cross_y = frame*SPRING_BG_BOTTOM_Y_DELTA/frame_total+SPRING_BG_BOTTOM_YMIN;	
	//cross_y = ValFromDlg_1;
	//背景坐标系，图片最大最小y
	y_min = cross_y-spring_Bg_Obj.up_window.h;
	y_max = cross_y;
	Bg_copy_min = MAX(y_min,0);
	Bg_copy_max = MIN(y_max,bg_window.h);
	Copy_len = MAX(Bg_copy_max-Bg_copy_min,0);
	//pic在全局坐标里的左上点为零点的坐标系，背景最大最小y
	y_min = 0-y_min;
	y_max = bg_window.h+y_min;
	Pic_min = MAX(y_min,0);
	memcpy( pBG+(Bg_copy_min*bg_window.w), pspring_Bg+(Pic_min*bg_window.w), bg_window.w*Copy_len*sizeof(COLOR16));
//cross grass
	//cross_y = frame*(CROSS_GRASS_BG_TOP_YMAX-CROSS_GRASS_BG_TOP_YMIN)/frame_total+CROSS_GRASS_BG_TOP_YMIN;
	//cross_y = ValFromDlg_1;
	cross_offset.x = 0;
	cross_offset.y = cross_y;

	S2D_Blending_Dec(bg_window, pBG, Cross_Grass_Bg_Obj.up_window,cross_offset,Cross_Grass_Bg_Obj.srcARGB_Map,pBG);

//chair
	//cross_offset.x = BENCH_TOP_X;
	//cross_offset.y = frame*GRASS_BG_TOP_Y_DELTA/frame_total+BENCH_TOP_YMIN;//ValFromDlg_1;
	//S2D_Blending(bg_window, pBG, Garden_Bench_Obj.up_window,cross_offset,Garden_Bench_Obj.srcARGB_Map,pBG);


}

LOCAL void springButterFlyCalc(int32 frame, int32 frame_total, COLOR16* pBG, IMGREF_SIZE_T bg_window)
{
	int32 i = 0;
	int32 res_frame = 0;
	FIX16_POINT_T offset = {0};
	int32 calc_temp = 0;

	for(i=0;i<BUTTER_FLYING_NUM;i++)
	{
		if(!FlyingButter[i].IsInAction)
		{
			if(FlyingButter[i].wait_num>0)
			{
				FlyingButter[i].wait_num--;
				continue;
			}
			else
			{
				FlyingButter[i].pButterFly_Res = ButterFly1;
				FlyingButter[i].Action_total_frame = BUTTERFLY1_RES_OBJ_NUM;
				FlyingButter[i].cur_frame = (rand()%FlyingButter[i].Action_total_frame)<<BUTTER_ACTION_PREC;
				FlyingButter[i].action_step = (1<<(BUTTER_ACTION_PREC-2));
				FlyingButter[i].IsInAction = TRUE;
				FlyingButter[i].Sta_point.x = ButterFly_START_X;
				FlyingButter[i].Sta_point.y = ButterFly_START_Y_MIN+frame*GRASS_BG_TOP_Y_DELTA/frame_total+rand()%ButterFly_START_Y_DELTA;
				FlyingButter[i].End_point.x = ButterFly_END_X;
				FlyingButter[i].End_point.y = ButterFly_START_Y_MIN+frame*GRASS_BG_TOP_Y_DELTA/frame_total - rand()%ButterFly_END_Y_DELTA;
				FlyingButter[i].vel_x = 1<<8;
				FlyingButter[i].dist_x = 0;
				FlyingButter[i].wait_num = rand()%200;
			}
		}
		else
		{
			if(FlyingButter[i].Sta_point.x == FlyingButter[i].End_point.x)
			{
				FlyingButter[i].IsInAction = FALSE;
				continue;
			}
			
			FlyingButter[i].cur_frame = FlyingButter[i].cur_frame+FlyingButter[i].action_step*3;
			if(FlyingButter[i].cur_frame >= (FlyingButter[i].Action_total_frame<<BUTTER_ACTION_PREC))
			{
				FlyingButter[i].cur_frame = 0;
			}
			res_frame = FlyingButter[i].cur_frame>>BUTTER_ACTION_PREC;
			
			calc_temp = (FlyingButter[i].Sta_point.x<<16)-(FlyingButter[i].dist_x<<(16-BUTTER_ACTION_PREC));
			offset.x_dec = calc_temp&0xffff;
			offset.x = (calc_temp>>16);
			
			calc_temp = (FlyingButter[i].Sta_point.y<<16)-((FlyingButter[i].dist_x*(FlyingButter[i].Sta_point.y-FlyingButter[i].End_point.y)/(FlyingButter[i].Sta_point.x-FlyingButter[i].End_point.x))<<(16-BUTTER_ACTION_PREC));
			offset.y_dec = calc_temp&0xffff;
			offset.y = (calc_temp>>16)+frame*GRASS_BG_TOP_Y_DELTA/frame_total;

			if(offset.x>FlyingButter[i].Sta_point.x||offset.x<FlyingButter[i].End_point.x)
			{
				FlyingButter[i].IsInAction = FALSE;
				continue;
			}
			else
			{
				S2D_Blending_Dec(bg_window, pBG, FlyingButter[i].pButterFly_Res[res_frame].up_window,offset,FlyingButter[i].pButterFly_Res[res_frame].srcARGB_Map,pBG);
				FlyingButter[i].dist_x += FlyingButter[i].vel_x*3;
			}
		}
	}
//sleeping butterfly
	for(i=0;i<BUTTER_SLEEPING_NUM;i++)
	{
		if(!SleepingButter[i].IsInAction)
		{
			SleepingButter[i].pButterFly_Res = ButterFly1;
			SleepingButter[i].Action_total_frame = BUTTERFLY1_RES_OBJ_NUM;
			SleepingButter[i].cur_frame = (rand()%SleepingButter[i].Action_total_frame)<<BUTTER_ACTION_PREC;
			SleepingButter[i].action_step = (1<<(BUTTER_ACTION_PREC-3))-(1<<(BUTTER_ACTION_PREC-5));
			SleepingButter[i].IsInAction = TRUE;
		}
		else
		{
			SleepingButter[i].cur_frame = SleepingButter[i].cur_frame+SleepingButter[i].action_step*3;
			if(SleepingButter[i].cur_frame >= (SleepingButter[i].Action_total_frame<<BUTTER_ACTION_PREC))
			{
				SleepingButter[i].cur_frame = 0;
			}
			res_frame = SleepingButter[i].cur_frame>>BUTTER_ACTION_PREC;
			offset.x = SLEEPING_BUTTER_X;
			offset.x_dec = 0;
			offset.y = SLEEPING_BUTTER_Y_MIN+ frame*GRASS_BG_TOP_Y_DELTA/frame_total;
			offset.y_dec = 0;
			S2D_Blending_Dec(bg_window, pBG, SleepingButter[i].pButterFly_Res[res_frame].up_window,offset,SleepingButter[i].pButterFly_Res[res_frame].srcARGB_Map,pBG);
		}
	}
	


}

LOCAL void springUpWindowCalc(int32 frame, int32 frame_total, COLOR16* pBG, IMGREF_SIZE_T bg_window)
{
	
	FIX16_POINT_T offset = {0};
	offset.x = 0;
	offset.y = frame*BRANCH_Y_DELTA/frame_total+BRANCH_YMIN;//ValFromDlg_1-500;
	S2D_Blending_Dec(bg_window, pBG, Branch_Obj.up_window,offset,Branch_Obj.srcARGB_Map,pBG);
	
}

LOCAL void springUpUpWindowCalc(int32 frame, int32 frame_total, COLOR16* pBG, IMGREF_SIZE_T bg_window)
{
	
	FIX16_POINT_T offset = {0};
	static int32 fg1_ctrl = 0,fg2_ctrl = 0;
	int32 fg1_max = 200, fg1_off = 0, fg2_max = 160;

	fg1_ctrl++;
	if(fg1_ctrl>=fg1_max)
	{
		fg1_ctrl = 0;
	}

        fg2_ctrl++;
	if(fg2_ctrl>=fg2_max)
	{
		fg2_ctrl = 0;
	}

        fg1_off = (SPRING_FG1_XMAX<<16)+(SPRING_FG1_X_AMP*s2d_sin_new(fg1_ctrl*3600/fg1_max))-(SPRING_FG1_X_AMP<<16);
        offset.x_dec = fg1_off&0xffff;
        offset.x = fg1_off>>16;
        fg1_off = ((frame*SPRING_FG1_Y_DELTA/frame_total+SPRING_FG1_YMIN)<<16)+(SPRING_FG1_Y_AMP*s2d_sin_new(fg1_ctrl*3600/fg1_max))-(SPRING_FG1_Y_AMP<<16);
        offset.y_dec = fg1_off&0xffff;
        offset.y = fg1_off>>16;//ValFromDlg_1-500;
	S2D_Blending_Dec(bg_window, pBG, spring_fg1_Obj.up_window,offset,spring_fg1_Obj.srcARGB_Map,pBG);

        fg1_off = (SPRING_FG2_XMAX<<16)-(SPRING_FG2_X_AMP*s2d_sin_new(fg2_ctrl*3600/fg2_max))+(SPRING_FG2_X_AMP<<16);
        offset.x_dec = fg1_off&0xffff;
        offset.x = fg1_off>>16;
        fg1_off = ((frame*SPRING_FG2_Y_DELTA/frame_total+SPRING_FG2_YMIN)<<16)+(SPRING_FG2_Y_AMP*s2d_sin_new(fg2_ctrl*3600/fg2_max))-(SPRING_FG2_Y_AMP<<16);
        offset.y_dec = fg1_off&0xffff;
        offset.y =  fg1_off>>16;//ValFromDlg_1-500;////
	S2D_Blending_Dec(bg_window, pBG, spring_fg2_Obj.up_window,offset,spring_fg2_Obj.srcARGB_Map,pBG);
	
}


LOCAL void springPetalFlyCalc(int32 frame, int32 frame_total, COLOR16* pBG, IMGREF_SIZE_T bg_window)
{
	int32 i = 0, res_frame = 0, calc_temp = 0;
	FIX16_POINT_T offset = {0};

	//static int32 ctrl = 0;

    if(PNULL == s_spring_Petal)
    {
        return;
    }
          
	for(i=0;i<SPRING_PETAL_OBJ_NUM;i++)
	{
		if(!s_spring_Petal[i].IsInAction)
		{
		    	if(s_spring_Petal[i].wait_num>0)
			{
				s_spring_Petal[i].wait_num--;
				continue;
			}
			else
			{
				s_spring_Petal[i].pPetal_Res = Petal1;
				s_spring_Petal[i].Action_total_frame = PETAL1_RES_OBJ_NUM;
				s_spring_Petal[i].cur_frame = (rand()%s_spring_Petal[i].Action_total_frame)<<BUTTER_ACTION_PREC;
				s_spring_Petal[i].action_step = (1<<(BUTTER_ACTION_PREC-2))+(1<<(BUTTER_ACTION_PREC-3))*(rand()%100)/100;
				s_spring_Petal[i].IsInAction = TRUE;
				
				s_spring_Petal[i].Sta_point.x = PETALFLY_START_X + rand()%PETALFLY_START_X_DETLA;
				s_spring_Petal[i].Sta_point.y = PETALFLY_START_Y;
				s_spring_Petal[i].End_point.x = PETALFLY_END_X + rand()%PETALFLY_END_X_DETLA;
				s_spring_Petal[i].End_point.y = PETALFLY_END_Y;
				
				s_spring_Petal[i].vel_y = 5<<8;
				s_spring_Petal[i].dist_y = 0;
				s_spring_Petal[i].wait_num = rand()%10;
			}
		}
		else
		{
			if(s_spring_Petal[i].Sta_point.y >= s_spring_Petal[i].End_point.y)
			{
				s_spring_Petal[i].IsInAction = FALSE;
				continue;
			}
			
			s_spring_Petal[i].cur_frame = s_spring_Petal[i].cur_frame+s_spring_Petal[i].action_step*2;
			if(s_spring_Petal[i].cur_frame >= (s_spring_Petal[i].Action_total_frame<<BUTTER_ACTION_PREC))
			{
				s_spring_Petal[i].cur_frame = 0;
			}
			res_frame = s_spring_Petal[i].cur_frame>>BUTTER_ACTION_PREC;

			calc_temp = (s_spring_Petal[i].Sta_point.x<<16)+((s_spring_Petal[i].dist_y*(s_spring_Petal[i].Sta_point.x-s_spring_Petal[i].End_point.x)/(s_spring_Petal[i].Sta_point.y-s_spring_Petal[i].End_point.y))<<(16-BUTTER_ACTION_PREC));
			offset.x_dec = calc_temp&0xffff;
			offset.x = (calc_temp>>16);
			
			calc_temp = (s_spring_Petal[i].Sta_point.y<<16)+(s_spring_Petal[i].dist_y<<(16-BUTTER_ACTION_PREC));
			offset.y_dec = calc_temp&0xffff;
			offset.y = (calc_temp>>16)+frame*GRASS_BG_TOP_Y_DELTA/frame_total;

			if(offset.y<s_spring_Petal[i].Sta_point.y||offset.y>s_spring_Petal[i].End_point.y)
			{
				s_spring_Petal[i].IsInAction = FALSE;
				continue;
			}
			else
			{
				S2D_Blending_Dec(bg_window, pBG, s_spring_Petal[i].pPetal_Res[res_frame].up_window,offset,s_spring_Petal[i].pPetal_Res[res_frame].srcARGB_Map,pBG);
				s_spring_Petal[i].dist_y += s_spring_Petal[i].vel_y*2;
			}
		}
	}
}
LOCAL void spring55Calc(int32 frame, int32 frame_total, COLOR16* pBG, IMGREF_SIZE_T bg_window)
{
static uint32 cstTime1 = 0;
uint32 cstTime = 0;

SCI_TRACE_LOW("spring55Calc frame : %d ms", SCI_GetTickCount()-cstTime1);
cstTime1 = SCI_GetTickCount();

cstTime = SCI_GetTickCount();
	springBgCalc(frame,frame_total,pBG,bg_window);
SCI_TRACE_LOW("spring55Calc-springBgCalc: calc time : %d ms", SCI_GetTickCount()-cstTime);

cstTime = SCI_GetTickCount();
	springButterFlyCalc(frame,frame_total,pBG,bg_window);
SCI_TRACE_LOW("spring55Calc-springButterFlyCalc: calc time : %d ms", SCI_GetTickCount()-cstTime);

cstTime = SCI_GetTickCount();
	springUpWindowCalc(frame,frame_total,pBG,bg_window);
SCI_TRACE_LOW("spring55Calc-springUpWindowCalc: calc time : %d ms", SCI_GetTickCount()-cstTime);

	//springSunShineCalc(frame,frame_total,pBG, bg_window);
cstTime = SCI_GetTickCount();
	springPetalFlyCalc(frame,frame_total,pBG, bg_window);
SCI_TRACE_LOW("spring55Calc-springPetalFlyCalc: calc time : %d ms", SCI_GetTickCount()-cstTime);

cstTime = SCI_GetTickCount();
	springUpUpWindowCalc(frame,frame_total,pBG,bg_window);
SCI_TRACE_LOW("spring55Calc-springUpWindowCalc: calc time : %d ms", SCI_GetTickCount()-cstTime);

SCI_TRACE_LOW("spring55Calc calc all cost: %d ms", SCI_GetTickCount()-cstTime1);
	
}

#endif// LIVE_WALLPAPER_SUPPORT_SPRING


#endif //_MMI_LIVEWALLPAPER_SPRING_C_

