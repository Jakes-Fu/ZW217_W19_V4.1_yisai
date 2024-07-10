/****************************************************************************
** File Name:      mmitheme_graph_data.c                                   *
** Author:         James.Zhang                                             *
** Date:           01/18/2012                                              *
** Copyright:      2012 Spreadtrum, Incoporated. All Rights Reserved.      *
** Description:    This file is used to describe the application modual.   *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 01/2012        James.Zhang       Create
** 
****************************************************************************/

#define _MMITHEME_GRAPH_DATA_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_utility.h"
#include "mmidisplay_data.h"
#include "mmi_color.h"
#include "mmi_position.h"
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
//******************* title graphy ****************************************//
/*title bg*/
#define MMI_TITLE_BG_COL_LEFT  0xFF6400
#define MMI_TITLE_BG_COL_RIGHT 0x00C8C8
#define MMI_TITLE_BG_COL_UP    0xFFFFFF
#define MMI_TITLE_BG_COL_SPACE 0
#define MMI_TITLE_BG_COL_GAP   2
#ifdef MAINLCD_SIZE_240X400
#define MMI_TITLE_BG_COL_HEIGHT   32
#elif defined MAINLCD_SIZE_240X320
#define MMI_TITLE_BG_COL_HEIGHT   32
#elif defined MAINLCD_SIZE_320X480
#define MMI_TITLE_BG_COL_HEIGHT   40
#else
#define MMI_TITLE_BG_COL_HEIGHT   20
#endif
//////////////////////////////////////
#ifdef MAINLCD_SIZE_240X400
#define MMI_SUB_TITLE_SCALE   4/5
#elif defined MAINLCD_SIZE_240X320
	//此分辨率下横屏title右侧区域字体显示不开,特单独调整bug89848
	#ifdef MAINLCD_DEV_SIZE_240X320
		#define MMI_SUB_TITLE_SCALE   3/4//lint !e773
	#else
		#define MMI_SUB_TITLE_SCALE   4/5//lint !e773
	#endif
#elif defined MAINLCD_SIZE_320X480
#define MMI_SUB_TITLE_SCALE   5/6
#else
#define MMI_SUB_TITLE_SCALE   4/5//lint !e773
#endif
/////////////////////////////////////
#define MMI_TITLE_BG_HOL_GAP    (MMI_MAINSCREEN_WIDTH * MMI_SUB_TITLE_SCALE-(MMI_TITLE_BG_COL_HEIGHT-MMI_TITLE_BG_COL_GAP))
#define MMI_TITLE_BG_COL0_GAP   (MMI_MAINSCREEN_HEIGHT * MMI_SUB_TITLE_SCALE-(MMI_TITLE_BG_COL_HEIGHT-MMI_TITLE_BG_COL_GAP))

/*竖屏坐标*/
LOCAL const GUI_POINT_T  s_title_point_array_up_v[] =  
{
    {0,  0},
	{MMI_MAINSCREEN_WIDTH,  0},
	{MMI_MAINSCREEN_WIDTH,  MMI_TITLE_BG_COL_GAP},
	{0,  MMI_TITLE_BG_COL_GAP },
};
LOCAL const MMI_GRAPH_POLYGON_T s_image_title_bg_point_up_data_v[] = 
{
    {ARR_SIZE(s_title_point_array_up_v), s_title_point_array_up_v},
};

LOCAL const GUI_POINT_T  s_title_point_array_left_v[] =  
{
    {0, MMI_TITLE_BG_COL_GAP},
	{MMI_MAINSCREEN_WIDTH * MMI_SUB_TITLE_SCALE , MMI_TITLE_BG_COL_GAP},
	{MMI_TITLE_BG_HOL_GAP, MMI_TITLE_BG_COL_HEIGHT},	
	{0, MMI_TITLE_BG_COL_HEIGHT},
};

LOCAL const MMI_GRAPH_POLYGON_T s_image_title_bg_point_left_data_v[] = 
{
    {ARR_SIZE(s_title_point_array_left_v), s_title_point_array_left_v},
};

LOCAL const GUI_POINT_T  s_title_point_array_mid_v[] =  
{
    {MMI_MAINSCREEN_WIDTH * MMI_SUB_TITLE_SCALE , MMI_TITLE_BG_COL_GAP},
	{MMI_MAINSCREEN_WIDTH * MMI_SUB_TITLE_SCALE + 3, MMI_TITLE_BG_COL_GAP},
	{MMI_TITLE_BG_HOL_GAP + 3, MMI_TITLE_BG_COL_HEIGHT},
	{MMI_TITLE_BG_HOL_GAP, MMI_TITLE_BG_COL_HEIGHT},	
};

LOCAL const MMI_GRAPH_POLYGON_T s_image_title_bg_point_mid_data_v[] = 
{
    {ARR_SIZE(s_title_point_array_mid_v), s_title_point_array_mid_v},
};

LOCAL const GUI_POINT_T  s_title_point_array_right_v[] =  
{
    {MMI_MAINSCREEN_WIDTH * MMI_SUB_TITLE_SCALE + 3, MMI_TITLE_BG_COL_GAP},
	{MMI_MAINSCREEN_WIDTH , MMI_TITLE_BG_COL_GAP},
	{MMI_MAINSCREEN_WIDTH , MMI_TITLE_BG_COL_HEIGHT},
	{MMI_TITLE_BG_HOL_GAP + 3, MMI_TITLE_BG_COL_HEIGHT},
};

LOCAL const MMI_GRAPH_POLYGON_T s_image_title_bg_point_right_data_v[] = 
{
    {ARR_SIZE(s_title_point_array_right_v), s_title_point_array_right_v},
};

LOCAL const MMI_GRAPH_ITEM_T s_image_title_bg_data_v[] =
{

    //up
	{
        MMI_GRAPH_POLYGON,                 //type
        0, //format
        MMI_TITLE_BG_COL_UP,                
        s_image_title_bg_point_up_data_v
	},

    //left
    {
        MMI_GRAPH_POLYGON,                 //type
        0, //format
        MMI_TITLE_BG_COL_LEFT,                
        s_image_title_bg_point_left_data_v           //fill display rect
    },

	//mid
    {
        MMI_GRAPH_POLYGON,                 //type
        0, //format
        MMI_TITLE_BG_COL_UP,                
        s_image_title_bg_point_mid_data_v           //fill display rect
    },
	
	//right
	{
	    MMI_GRAPH_POLYGON,                 //type
        0, //format
        MMI_TITLE_BG_COL_RIGHT,//0xCB2,                
        s_image_title_bg_point_right_data_v           //fill display rect
	}

};

/*横屏坐标*/
LOCAL const GUI_POINT_T  s_title_point_array_up_h[] =  
{
    {0,  0},
	{MMI_MAINSCREEN_HEIGHT,  0},
	{MMI_MAINSCREEN_HEIGHT,  MMI_TITLE_BG_COL_GAP},
	{0,  MMI_TITLE_BG_COL_GAP },
};
LOCAL const MMI_GRAPH_POLYGON_T s_image_title_bg_point_up_data_h[] = 
{
    {ARR_SIZE(s_title_point_array_up_h), s_title_point_array_up_h},
};

LOCAL const GUI_POINT_T  s_title_point_array_left_h[] =  
{
    {0, MMI_TITLE_BG_COL_GAP},
	{MMI_MAINSCREEN_HEIGHT * MMI_SUB_TITLE_SCALE , MMI_TITLE_BG_COL_GAP},
	{MMI_MAINSCREEN_HEIGHT, MMI_TITLE_BG_COL_HEIGHT},	
	{0, MMI_TITLE_BG_COL_HEIGHT},
};

LOCAL const MMI_GRAPH_POLYGON_T s_image_title_bg_point_left_data_h[] = 
{
    {ARR_SIZE(s_title_point_array_left_h), s_title_point_array_left_h},
};

LOCAL const GUI_POINT_T  s_title_point_array_mid_h[] =  
{
    {MMI_MAINSCREEN_HEIGHT * MMI_SUB_TITLE_SCALE , MMI_TITLE_BG_COL_GAP},
	{MMI_MAINSCREEN_HEIGHT * MMI_SUB_TITLE_SCALE + 3, MMI_TITLE_BG_COL_GAP},
	{MMI_TITLE_BG_COL0_GAP + 3, MMI_TITLE_BG_COL_HEIGHT},
	{MMI_TITLE_BG_COL0_GAP, MMI_TITLE_BG_COL_HEIGHT},	
};

LOCAL const MMI_GRAPH_POLYGON_T s_image_title_bg_point_mid_data_h[] = 
{
    {ARR_SIZE(s_title_point_array_mid_h), s_title_point_array_mid_h},
};

LOCAL const GUI_POINT_T  s_title_point_array_right_h[] =  
{
    {MMI_MAINSCREEN_HEIGHT * MMI_SUB_TITLE_SCALE + 3, MMI_TITLE_BG_COL_GAP},
	{MMI_MAINSCREEN_HEIGHT , MMI_TITLE_BG_COL_GAP},
	{MMI_MAINSCREEN_HEIGHT , MMI_TITLE_BG_COL_HEIGHT},
	{MMI_TITLE_BG_COL0_GAP + 3, MMI_TITLE_BG_COL_HEIGHT},
};

LOCAL const MMI_GRAPH_POLYGON_T s_image_title_bg_point_right_data_h[] = 
{
    {ARR_SIZE(s_title_point_array_right_h), s_title_point_array_right_h},
};

LOCAL const MMI_GRAPH_ITEM_T s_image_title_bg_data_h[] =
{

    //up
	{
        MMI_GRAPH_POLYGON,                 //type
        0, //format
        MMI_TITLE_BG_COL_UP,                
        s_image_title_bg_point_up_data_h
	},

    //left
    {
        MMI_GRAPH_POLYGON,                 //type
        0, //format
        MMI_TITLE_BG_COL_LEFT,                
        s_image_title_bg_point_left_data_h           //fill display rect
    },

	//mid
    {
        MMI_GRAPH_POLYGON,                 //type
        0, //format
        MMI_TITLE_BG_COL_UP,                
        s_image_title_bg_point_mid_data_h           //fill display rect
    },
	
	//right
	{
	    MMI_GRAPH_POLYGON,                 //type
        0, //format
        MMI_TITLE_BG_COL_RIGHT,//0xCB2,                
        s_image_title_bg_point_right_data_h           //fill display rect
	}

};
//******************* common bg graphy ****************************************//
#define MMI_COMMON_BG_COL    0xFFFFFF
/*竖屏坐标*/
LOCAL const MMI_GRAPH_ITEM_T s_image_common_bg_data_v[] =
{
    //bg graphy
	{
        MMI_GRAPH_RECT,                 //type
        0, //format
        MMI_COMMON_BG_COL,                
        PNULL
	},
};

/*横屏坐标*/
LOCAL const MMI_GRAPH_ITEM_T s_image_commom_bg_data_h[] =
{
    //bg graphy
	{
        MMI_GRAPH_RECT,                 //type
        0, //format
        MMI_COMMON_BG_COL,                
        PNULL
	},
};

/*---------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                 */
/*---------------------------------------------------------------------------*/
/*竖屏的显示数据*/
#if defined MMI_RES_ORIENT_PORTRAIT || defined MMI_RES_ORIENT_BOTH
const MMI_GRAPH_DATA_T g_graph_data_array_v[] =      /*lint -esym(552, g_graph_data_array_v)*/
{
    {
        "IMAGE_TITLE_BAR",                //用字符串表示id
         ARR_SIZE(s_image_title_bg_data_v), //图形个数
         s_image_title_bg_data_v,           //图形数据
         MMI_MAINSCREEN_WIDTH,            //图形的宽
         MMI_TITLE_BG_COL_HEIGHT       //图形的高
    },
    {
        "IMAGE_COMMON_BG",                //用字符串表示id
         ARR_SIZE(s_image_common_bg_data_v), //图形个数
         s_image_common_bg_data_v,           //图形数据
         MMI_MAINSCREEN_WIDTH,            //图形的宽
         MMI_MAINSCREEN_HEIGHT       //图形的高
    },
};

const uint32 g_graph_data_num_v = ARR_SIZE(g_graph_data_array_v);
#endif
/*横屏的显示数据*/
#if defined MMI_RES_ORIENT_LANDSCAPE || defined MMI_RES_ORIENT_BOTH
const MMI_GRAPH_DATA_T g_graph_data_array_h[] =    /*lint -esym(552, g_graph_data_array_h)*/
{
    {
        "IMAGE_TITLE_BAR",                //用字符串表示id
         ARR_SIZE(s_image_title_bg_data_h), //图形个数
         s_image_title_bg_data_h,           //图形数据
         MMI_MAINSCREEN_HEIGHT,            //图形的宽
         MMI_TITLE_BG_COL_HEIGHT       //图形的高
    },
    {
        "IMAGE_COMMON_BG",                //用字符串表示id
         ARR_SIZE(s_image_commom_bg_data_h), //图形个数
         s_image_commom_bg_data_h,           //图形数据
         MMI_MAINSCREEN_HEIGHT,            //图形的宽
         MMI_MAINSCREEN_WIDTH       //图形的高
    },
};

const uint32 g_graph_data_num_h = ARR_SIZE(g_graph_data_array_h);
#endif

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DEFINITION                              *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/


