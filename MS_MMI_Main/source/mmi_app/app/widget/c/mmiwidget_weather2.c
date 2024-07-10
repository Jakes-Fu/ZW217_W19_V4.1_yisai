/*****************************************************************************
** File Name:      mmiwidget_weather.c                                        *
** Author:                                                                   *
** Date:           05/05/2011                                                *
** Copyright:      2004 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE                  NAME             DESCRIPTION                        *
** 05/05/2011        xiaoming.ren              Creat
******************************************************************************/

#define _MMIWIDGET_WEATHER_TWO_C_

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "mmi_app_widget_trc.h"
#ifdef WEATHER_SUPPORT
#ifdef MMI_WIDGET_WEATHER2
#include "window_parse.h"
#include "guilabel.h"
#include "guiform.h"
#include "mmipub.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmidisplay_data.h"
#include "mmiwidget_image.h"
#include "guibutton.h"
#include "mmiwidget.h"
#include "mmiwidget_nv.h"
#include "mmiwidget_id.h"
#include "guires.h"
#include "mmitheme_widget.h"
#include "mmi_appmsg.h"
#include "mmiwidget_text.h"
//#include "mmiset.h"
#include "mmiset_export.h"
#include "mmi_nv.h"
#include "mmiset_text.h"
#include "guidropdownlist.h"
#include "guisetlist.h"
#include "mmiweather_funcs.h"
#include "mmiweather_win.h"
#include "mmiweather.h"
#include "mmiweather_text.h"
#include "mmiweather_export.h"
#include "mmiebook_text.h"
#include "mmiwidget_export.h"
#include "mmi_appmsg.h"

#ifdef PDA_WEATHER_TIME_SE
#include "graphics_3d_rotate.h"
#endif

#ifdef QBTHEME_SUPPORT
#include "mmiqbtheme_idlewin.h"
#endif

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**-------------------------------------------------------------------------*/
#if defined (MAINLCD_SIZE_240X400) || defined (MAINLCD_SIZE_240X320)
#define   MMIWIDGET_WEATHER_TEXT_FONT                   SONG_FONT_14            //天气信息字体
#define   MMIWIDGET_WEATHER_TEXT_COLOR                  MMI_WHITE_COLOR         //天气信息字体颜色
#define   MMIWIDGET_WEATHER_DEFAULT_X                   50                      //widget初始化x坐标
#define   MMIWIDGET_WEATHER_DEFAULT_Y                   50                      //widget初始化y坐标

#define   MMIWIDGET_WEATHER_TIME_COLOR                 MMI_WHITE_COLOR                     //时间颜色
#define   MMIWIDGET_WEATHER_TIME_FONT                   SONG_FONT_26                                //时间字体

#define   MMIWIDGET_WEATHER_TIME_TEXT_MAX           10   

#define	MMIWIDGET_WEATHER_TIME_X			82         
#define	MMIWIDGET_WEATHER_TIME_Y			34        
#define	MMIWIDGET_WEATHER_TODAY_X			82          
#define	MMIWIDGET_WEATHER_TODAY_Y			12       
#define	MMIWIDGET_WEATHER_TOMORROW_X			54          
#define	MMIWIDGET_WEATHER_TOMORROW_Y			93          
#define	MMIWIDGET_WEATHER_AFTER_TOMORROW_X			54
#define	MMIWIDGET_WEATHER_AFTER_TOMORROW_Y			131
#define	MMIWIDGET_WEATHER_CITY_X			82
#define	MMIWIDGET_WEATHER_CITY_Y			61
#define	MMIWIDGET_WEATHER_TODAY_IMG_X			13          
#define	MMIWIDGET_WEATHER_TODAY_IMG_Y			12       
#define	MMIWIDGET_WEATHER_TOMORROW_IMG_X			16          
#define	MMIWIDGET_WEATHER_TOMORROW_IMG_Y			83          
#define	MMIWIDGET_WEATHER_AFTER_TOMORROW_IMG_X			16
#define	MMIWIDGET_WEATHER_AFTER_TOMORROW_IMG_OFFSET			6
#define	MMIWIDGET_WEATHER_AFTER_TOMORROW_IMG_Y			(MMIWIDGET_WEATHER_TOMORROW_IMG_Y + MMIWIDGET_WEATHER_IMG_SMALL_HEIGHT + MMIWIDGET_WEATHER_AFTER_TOMORROW_IMG_OFFSET)
#define	MMIWIDGET_WEATHER_IMG_BIG_WIDTH			64             
#define	MMIWIDGET_WEATHER_IMG_BIG_HEIGHT			64             
#define	MMIWIDGET_WEATHER_IMG_SMALL_WIDTH			32      
#define	MMIWIDGET_WEATHER_IMG_SMALL_HEIGHT			32   
#define	MMIWIDGET_WEATHER_TIME_TEXT_RECT_WIDTH			139
#define	MMIWIDGET_WEATHER_TIME_TEXT_RECT_HEIGHT			20
#define	MMIWIDGET_WEATHER_TEXT_ITEM1_WIDTH			139             //上面宽度
#define	MMIWIDGET_WEATHER_TEXT_ITEM2_WIDTH			168             //下面宽度
#define	MMIWIDGET_WEATHER_TEXT_ITEM_HEIGHT			14              //ITEM 高度
/*cfy*/
#define	MMIWIDGET_WEATHER__FOCUS_OFFSET_H			1
#define	MMIWIDGET_WEATHER__FOCUS_OFFSET_V			1  
#define	MMIWIDGET_WEATHER_TIME_FOCUS_X			(MMIWIDGET_WEATHER_TIME_X - MMIWIDGET_WEATHER__FOCUS_OFFSET_H)         
#define	MMIWIDGET_WEATHER_TIME_FOCUS_Y			(MMIWIDGET_WEATHER_TIME_Y - MMIWIDGET_WEATHER__FOCUS_OFFSET_V)        
#define	MMIWIDGET_WEATHER_TODAY_FOCUS_X			(MMIWIDGET_WEATHER_TODAY_X - MMIWIDGET_WEATHER__FOCUS_OFFSET_H)          
#define	MMIWIDGET_WEATHER_TODAY_FOCUS_Y			(MMIWIDGET_WEATHER_TODAY_Y - MMIWIDGET_WEATHER__FOCUS_OFFSET_V)       

#define	MMIWIDGET_WEATHER_CITY_FOCUS_X			(MMIWIDGET_WEATHER_CITY_X - MMIWIDGET_WEATHER__FOCUS_OFFSET_H)
#define	MMIWIDGET_WEATHER_CITY_FOCUS_Y			(MMIWIDGET_WEATHER_CITY_Y - MMIWIDGET_WEATHER__FOCUS_OFFSET_V)
#define	MMIWIDGET_WEATHER_TODAY_IMG_FOCUS_X			(MMIWIDGET_WEATHER_TODAY_IMG_X - MMIWIDGET_WEATHER__FOCUS_OFFSET_H)          
#define	MMIWIDGET_WEATHER_TODAY_IMG_FOCUS_Y			(MMIWIDGET_WEATHER_TODAY_IMG_Y - MMIWIDGET_WEATHER__FOCUS_OFFSET_V)       
#define	MMIWIDGET_WEATHER_TOMORROW_IMG_FOCUS_X			(MMIWIDGET_WEATHER_TOMORROW_IMG_X - MMIWIDGET_WEATHER__FOCUS_OFFSET_H)          
#define	MMIWIDGET_WEATHER_TOMORROW_IMG_FOCUS_Y			(MMIWIDGET_WEATHER_TOMORROW_IMG_Y - MMIWIDGET_WEATHER__FOCUS_OFFSET_V)          
#define	MMIWIDGET_WEATHER_AFTER_TOMORROW_IMG_FOCUS_X			(MMIWIDGET_WEATHER_AFTER_TOMORROW_IMG_X - MMIWIDGET_WEATHER__FOCUS_OFFSET_H)
#define	MMIWIDGET_WEATHER_AFTER_TOMORROW_IMG_FOCUS_OFFSET			(MMIWIDGET_WEATHER_AFTER_TOMORROW_IMG_OFFSET - MMIWIDGET_WEATHER__FOCUS_OFFSET_V*2)
#define	MMIWIDGET_WEATHER_AFTER_TOMORROW_IMG_FOCUS_Y			(MMIWIDGET_WEATHER_TOMORROW_IMG_FOCUS_Y + MMIWIDGET_WEATHER_IMG_SMALL_FOUCS_HEIGHT + MMIWIDGET_WEATHER_AFTER_TOMORROW_IMG_FOCUS_OFFSET)

#define	MMIWIDGET_WEATHER_CITY_FOUCS_WIDTH			(MMIWIDGET_WEATHER_TEXT_ITEM1_WIDTH + MMIWIDGET_WEATHER__FOCUS_OFFSET_H*2)             //上面宽度
#define	MMIWIDGET_WEATHER_CITY_FOUCS_HEIGHT			(MMIWIDGET_WEATHER_TEXT_ITEM_HEIGHT + MMIWIDGET_WEATHER__FOCUS_OFFSET_V*2)              //ITEM 高度
#define	MMIWIDGET_WEATHER_TIME_FOUCS_WIDTH			(MMIWIDGET_WEATHER_TIME_TEXT_RECT_WIDTH + MMIWIDGET_WEATHER__FOCUS_OFFSET_H*2)             //上面宽度
#define	MMIWIDGET_WEATHER_TIME_FOUCS_HEIGHT			(MMIWIDGET_WEATHER_TIME_TEXT_RECT_HEIGHT + MMIWIDGET_WEATHER__FOCUS_OFFSET_V*2)              //ITEM 高度

#define	MMIWIDGET_WEATHER_IMG_BIG_FOUCS_WIDTH			(MMIWIDGET_WEATHER_IMG_BIG_WIDTH + MMIWIDGET_WEATHER__FOCUS_OFFSET_H*2)             
#define	MMIWIDGET_WEATHER_IMG_BIG_FOUCS_HEIGHT			(MMIWIDGET_WEATHER_IMG_BIG_HEIGHT + MMIWIDGET_WEATHER__FOCUS_OFFSET_V*2)             
#define	MMIWIDGET_WEATHER_IMG_SMALL_FOUCS_WIDTH			(MMIWIDGET_WEATHER_IMG_SMALL_WIDTH + MMIWIDGET_WEATHER__FOCUS_OFFSET_H*2 + MMIWIDGET_WEATHER_TEXT_ITEM2_WIDTH + 8)      
#define	MMIWIDGET_WEATHER_IMG_SMALL_FOUCS_HEIGHT			(MMIWIDGET_WEATHER_IMG_SMALL_HEIGHT + MMIWIDGET_WEATHER__FOCUS_OFFSET_V*2)   
#elif defined MAINLCD_SIZE_320X480
#define   MMIWIDGET_WEATHER_TEXT_FONT                   SONG_FONT_14            //天气信息字体
#define   MMIWIDGET_WEATHER_TEXT_COLOR                  MMI_WHITE_COLOR         //天气信息字体颜色
#define   MMIWIDGET_WEATHER_DEFAULT_X                   50                      //widget初始化x坐标
#define   MMIWIDGET_WEATHER_DEFAULT_Y                   50                      //widget初始化y坐标

#define   MMIWIDGET_WEATHER_TIME_COLOR                 MMI_WHITE_COLOR                     //时间颜色
#define   MMIWIDGET_WEATHER_TIME_FONT                   SONG_FONT_26                                //时间字体

#define   MMIWIDGET_WEATHER_TIME_TEXT_MAX           10   

#define	MMIWIDGET_WEATHER_TIME_X			82         
#define	MMIWIDGET_WEATHER_TIME_Y			34        
#define	MMIWIDGET_WEATHER_TODAY_X			82         
#define	MMIWIDGET_WEATHER_TODAY_Y			12       
#define	MMIWIDGET_WEATHER_TOMORROW_X			54          
#define	MMIWIDGET_WEATHER_TOMORROW_Y			93          
#define	MMIWIDGET_WEATHER_AFTER_TOMORROW_X			54
#define	MMIWIDGET_WEATHER_AFTER_TOMORROW_Y			131
#define	MMIWIDGET_WEATHER_CITY_X			82
#define	MMIWIDGET_WEATHER_CITY_Y			61
#define	MMIWIDGET_WEATHER_TODAY_IMG_X			13          
#define	MMIWIDGET_WEATHER_TODAY_IMG_Y			12       
#define	MMIWIDGET_WEATHER_TOMORROW_IMG_X			16          
#define	MMIWIDGET_WEATHER_TOMORROW_IMG_Y			83          
#define	MMIWIDGET_WEATHER_AFTER_TOMORROW_IMG_X			16
#define	MMIWIDGET_WEATHER_AFTER_TOMORROW_IMG_OFFSET			6
#define	MMIWIDGET_WEATHER_AFTER_TOMORROW_IMG_Y			(MMIWIDGET_WEATHER_TOMORROW_IMG_Y + MMIWIDGET_WEATHER_IMG_SMALL_HEIGHT + MMIWIDGET_WEATHER_AFTER_TOMORROW_IMG_OFFSET)
#define	MMIWIDGET_WEATHER_IMG_BIG_WIDTH			64             
#define	MMIWIDGET_WEATHER_IMG_BIG_HEIGHT			64             
#define	MMIWIDGET_WEATHER_IMG_SMALL_WIDTH			32      
#define	MMIWIDGET_WEATHER_IMG_SMALL_HEIGHT			32   
#define	MMIWIDGET_WEATHER_TIME_TEXT_RECT_WIDTH			190
#define	MMIWIDGET_WEATHER_TIME_TEXT_RECT_HEIGHT			20
#define	MMIWIDGET_WEATHER_TEXT_ITEM1_WIDTH			190             //上面宽度
#define	MMIWIDGET_WEATHER_TEXT_ITEM2_WIDTH			215             //下面宽度
#define	MMIWIDGET_WEATHER_TEXT_ITEM_HEIGHT			14              //ITEM 高度
/*cfy*/
#define	MMIWIDGET_WEATHER__FOCUS_OFFSET_H			1
#define	MMIWIDGET_WEATHER__FOCUS_OFFSET_V			1  
#define	MMIWIDGET_WEATHER_TIME_FOCUS_X			(MMIWIDGET_WEATHER_TIME_X - MMIWIDGET_WEATHER__FOCUS_OFFSET_H)         
#define	MMIWIDGET_WEATHER_TIME_FOCUS_Y			(MMIWIDGET_WEATHER_TIME_Y - MMIWIDGET_WEATHER__FOCUS_OFFSET_V)        
#define	MMIWIDGET_WEATHER_TODAY_FOCUS_X			(MMIWIDGET_WEATHER_TODAY_X - MMIWIDGET_WEATHER__FOCUS_OFFSET_H)          
#define	MMIWIDGET_WEATHER_TODAY_FOCUS_Y			(MMIWIDGET_WEATHER_TODAY_Y - MMIWIDGET_WEATHER__FOCUS_OFFSET_V)       

#define	MMIWIDGET_WEATHER_CITY_FOCUS_X			(MMIWIDGET_WEATHER_CITY_X - MMIWIDGET_WEATHER__FOCUS_OFFSET_H)
#define	MMIWIDGET_WEATHER_CITY_FOCUS_Y			(MMIWIDGET_WEATHER_CITY_Y - MMIWIDGET_WEATHER__FOCUS_OFFSET_V)
#define	MMIWIDGET_WEATHER_TODAY_IMG_FOCUS_X			(MMIWIDGET_WEATHER_TODAY_IMG_X - MMIWIDGET_WEATHER__FOCUS_OFFSET_H)          
#define	MMIWIDGET_WEATHER_TODAY_IMG_FOCUS_Y			(MMIWIDGET_WEATHER_TODAY_IMG_Y - MMIWIDGET_WEATHER__FOCUS_OFFSET_V)       
#define	MMIWIDGET_WEATHER_TOMORROW_IMG_FOCUS_X			(MMIWIDGET_WEATHER_TOMORROW_IMG_X - MMIWIDGET_WEATHER__FOCUS_OFFSET_H)          
#define	MMIWIDGET_WEATHER_TOMORROW_IMG_FOCUS_Y			(MMIWIDGET_WEATHER_TOMORROW_IMG_Y - MMIWIDGET_WEATHER__FOCUS_OFFSET_V)          
#define	MMIWIDGET_WEATHER_AFTER_TOMORROW_IMG_FOCUS_X			(MMIWIDGET_WEATHER_AFTER_TOMORROW_IMG_X - MMIWIDGET_WEATHER__FOCUS_OFFSET_H)
#define	MMIWIDGET_WEATHER_AFTER_TOMORROW_IMG_FOCUS_OFFSET			(MMIWIDGET_WEATHER_AFTER_TOMORROW_IMG_OFFSET - MMIWIDGET_WEATHER__FOCUS_OFFSET_V*2)
#define	MMIWIDGET_WEATHER_AFTER_TOMORROW_IMG_FOCUS_Y			(MMIWIDGET_WEATHER_TOMORROW_IMG_FOCUS_Y + MMIWIDGET_WEATHER_IMG_SMALL_FOUCS_HEIGHT + MMIWIDGET_WEATHER_AFTER_TOMORROW_IMG_FOCUS_OFFSET)

#define	MMIWIDGET_WEATHER_CITY_FOUCS_WIDTH			(MMIWIDGET_WEATHER_TEXT_ITEM1_WIDTH + MMIWIDGET_WEATHER__FOCUS_OFFSET_H*2)             //上面宽度
#define	MMIWIDGET_WEATHER_CITY_FOUCS_HEIGHT			(MMIWIDGET_WEATHER_TEXT_ITEM_HEIGHT + MMIWIDGET_WEATHER__FOCUS_OFFSET_V*2)              //ITEM 高度
#define	MMIWIDGET_WEATHER_TIME_FOUCS_WIDTH			(MMIWIDGET_WEATHER_TIME_TEXT_RECT_WIDTH + MMIWIDGET_WEATHER__FOCUS_OFFSET_H*2)             //上面宽度
#define	MMIWIDGET_WEATHER_TIME_FOUCS_HEIGHT			(MMIWIDGET_WEATHER_TIME_TEXT_RECT_HEIGHT + MMIWIDGET_WEATHER__FOCUS_OFFSET_V*2)              //ITEM 高度

#define	MMIWIDGET_WEATHER_IMG_BIG_FOUCS_WIDTH			(MMIWIDGET_WEATHER_IMG_BIG_WIDTH + MMIWIDGET_WEATHER__FOCUS_OFFSET_H*2)             
#define	MMIWIDGET_WEATHER_IMG_BIG_FOUCS_HEIGHT			(MMIWIDGET_WEATHER_IMG_BIG_HEIGHT + MMIWIDGET_WEATHER__FOCUS_OFFSET_V*2)             
#define	MMIWIDGET_WEATHER_IMG_SMALL_FOUCS_WIDTH			(MMIWIDGET_WEATHER_IMG_SMALL_WIDTH + MMIWIDGET_WEATHER__FOCUS_OFFSET_H*2 + MMIWIDGET_WEATHER_TEXT_ITEM2_WIDTH + 8)      
#define	MMIWIDGET_WEATHER_IMG_SMALL_FOUCS_HEIGHT			(MMIWIDGET_WEATHER_IMG_SMALL_HEIGHT + MMIWIDGET_WEATHER__FOCUS_OFFSET_V*2)   
       
#endif
/*cfy end*/ 

/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/
//信息类型
typedef enum
{
    MMIWIDGET_WEATHER_TODAY = 0,                //今天温度
    MMIWIDGET_WEATHER_TOMORROW,         //明天天气
    MMIWIDGET_WEATHER_AFTER_TOMORROW,   //后天天气
    MMIWIDGET_WEATHER_CITY,                     //城市
    MMIWIDGET_WEATHER_TIME,                     //时间
    MMIWIDGET_WEATHER_TODAY_IMG,    //今天图片
    MMIWIDGET_WEATHER_TOMORROW_IMG, //明天图片
    MMIWIDGET_WEATHER_AFTER_TOMORROW_IMG,//后天图片
    MMIWIDGET_WEATHER_MAX
}MMIWIDGET_WEATHER_POSITION_TYPE_E;

//FOUCS 信息类型
typedef enum
{
    MMIWIDGET_WEATHER_FOCUS_TODAY = 0,                //今天温度
    MMIWIDGET_WEATHER_FOCUS_TIME,       //时间
    MMIWIDGET_WEATHER_FOCUS_CITY,                     //城市
    MMIWIDGET_WEATHER_FOCUS_TODAY_IMG,    //今天图片
    MMIWIDGET_WEATHER_FOCUS_TOMORROW_IMG, //明天图片
    MMIWIDGET_WEATHER_FOCUS_AFTER_TOMORROW_IMG,//后天图片
    MMIWIDGET_WEATHER_FOCUS_MAX
}MMIWIDGET_WEATHER_FOCUS_POSITION_TYPE_E;


//WIDGET区域
typedef enum _WIDGET_WEATHER_RECT_TYPE
{
    TIME_RECT,
    MAIN_WEATHER_RECT,
    FORCAST_WEATHER_RECT,
    WEATHER_RECT_TYPE_MAX
}WIDGET_WEATHER_RECT_TYPE;
//WEATHER WIDGET状态
typedef enum 
{
    NEED_ADD_CITY,
    NEED_UPDATE_WEATHER,
    ENTER_WEATHER_WIN,
    WEATHER_STATUS_MAX
}CUR_WEAHTER_STATUS;
//显示图片类新
typedef enum
{
    BIG_WIDGET_ICON,
    SMALL_WIDGET_ICON
}WIDGET_ICON_TYPE;

typedef struct
{
    MMIWEATHER_CONDITION_E      condition_id; 
    MMI_IMAGE_ID_T              image_widget_big_id;
    MMI_IMAGE_ID_T              image_widget_small_id;
}MMIWEATHER_TWO_WIDGET_T;

typedef enum
{
    TODAY,
    TOMORROW,
    DAY_AFTER_TOMORROW,
    WEEK_DAY_INVALID
}SELECT_WEEK_DAY;
//zhm end

//widget窗口字符显示类型
typedef enum
{
    MMIWIDGET_WEATHER_DISPALY_TEXT_TYPE_CITYNAME,
    MMIWIDGET_WEATHER_DISPALY_TEXT_TYPE_DATE,
    MMIWIDGET_WEATHER_DISPALY_TEXT_TYPE_CONDITION,
    MMIWIDGET_WEATHER_DISPALY_TEXT_TYPE_TEMPRANGE,
    MMIWIDGET_WEATHER_DISPALY_TEXT_TYPE_PROMPT,
    MMIWIDGET_WEATHER_DISPALY_TEXT_TYPE_WEEKDAY_STRING,
    MMIWIDGET_WEATHER_DISPALY_TEXT_TYPE_INVALID
}MMIWIDGET_WEATHER_DISPALY_TEXT_TYPE_E;

//提示信息类型
typedef enum
{
    MMIWIDGET_WEATHER_DIS_PROMPT_STR_TYPE_UPDATEING,
    MMIWIDGET_WEATHER_DIS_PROMPT_STR_TYPE_NET_ERROR,
    MMIWIDGET_WEATHER_DIS_PROMPT_STR_TYPE_SWITCH_CITY,
    MMIWIDGET_WEATHER_DIS_PROMPT_STR_TYPE_INVALID
}MMIWIDGET_WEATHER_DIS_PROMPT_STR_TYPE_E;

//widget点击区域
typedef enum
{
    MMIWIDGET_WEATHER_CLICK_RECT_TODAY_TEMP,
    MMIWIDGET_WEATHER_CLICK_RECT_TODAY_IMG,
    MMIWIDGET_WEATHER_CLICK_RECT_TIME,
    MMIWIDGET_WEATHER_CLICK_RECT_CITY,
    MMIWIDGET_WEATHER_CLICK_RECT_TOMORROW,
    MMIWIDGET_WEATHER_CLICK_RECT_AFTER_TOMORROW,
    MMIWIDGET_WEATHER_CLICK_RECT_INVALID
}MMIWIDGET_WEATHER_CLICK_RECT_E;

/*---------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                 */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         EXTERNAL DEFINITION                              *
**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/
LOCAL SCI_TIME_T  s_sys_time = {0}; //记录最后一次更新的时间

//信息类型区域
LOCAL GUI_RECT_T s_weather_position[MMIWIDGET_WEATHER_MAX] = {
    {MMIWIDGET_WEATHER_TODAY_X,MMIWIDGET_WEATHER_TODAY_Y,(MMIWIDGET_WEATHER_TODAY_X + MMIWIDGET_WEATHER_TEXT_ITEM1_WIDTH),(MMIWIDGET_WEATHER_TODAY_Y + MMIWIDGET_WEATHER_TEXT_ITEM_HEIGHT)},    //今天温度
    {MMIWIDGET_WEATHER_TOMORROW_X,MMIWIDGET_WEATHER_TOMORROW_Y,(MMIWIDGET_WEATHER_TOMORROW_X + MMIWIDGET_WEATHER_TEXT_ITEM2_WIDTH),(MMIWIDGET_WEATHER_TOMORROW_Y + MMIWIDGET_WEATHER_TEXT_ITEM_HEIGHT)},    //明天天气
    {MMIWIDGET_WEATHER_AFTER_TOMORROW_X,MMIWIDGET_WEATHER_AFTER_TOMORROW_Y,(MMIWIDGET_WEATHER_AFTER_TOMORROW_X + MMIWIDGET_WEATHER_TEXT_ITEM2_WIDTH),(MMIWIDGET_WEATHER_AFTER_TOMORROW_Y + MMIWIDGET_WEATHER_TEXT_ITEM_HEIGHT)},    //后天天气
    {MMIWIDGET_WEATHER_CITY_X,MMIWIDGET_WEATHER_CITY_Y,(MMIWIDGET_WEATHER_CITY_X + MMIWIDGET_WEATHER_TEXT_ITEM1_WIDTH),(MMIWIDGET_WEATHER_CITY_Y + MMIWIDGET_WEATHER_TEXT_ITEM_HEIGHT)},    //城市
    {MMIWIDGET_WEATHER_TIME_X,MMIWIDGET_WEATHER_TIME_Y,(MMIWIDGET_WEATHER_TIME_X + MMIWIDGET_WEATHER_TIME_TEXT_RECT_WIDTH),(MMIWIDGET_WEATHER_TIME_Y + MMIWIDGET_WEATHER_TIME_TEXT_RECT_HEIGHT)},   //时间
    {MMIWIDGET_WEATHER_TODAY_IMG_X,MMIWIDGET_WEATHER_TODAY_IMG_Y,(MMIWIDGET_WEATHER_TODAY_IMG_X + MMIWIDGET_WEATHER_IMG_BIG_WIDTH),(MMIWIDGET_WEATHER_TODAY_IMG_Y + MMIWIDGET_WEATHER_IMG_BIG_HEIGHT)}, //今天大图片
    {MMIWIDGET_WEATHER_TOMORROW_IMG_X,MMIWIDGET_WEATHER_TOMORROW_IMG_Y,(MMIWIDGET_WEATHER_TOMORROW_IMG_X + MMIWIDGET_WEATHER_IMG_SMALL_WIDTH),(MMIWIDGET_WEATHER_TOMORROW_IMG_Y + MMIWIDGET_WEATHER_IMG_SMALL_HEIGHT)},     //明天小图片
    {MMIWIDGET_WEATHER_AFTER_TOMORROW_IMG_X,MMIWIDGET_WEATHER_AFTER_TOMORROW_IMG_Y,(MMIWIDGET_WEATHER_AFTER_TOMORROW_IMG_X + MMIWIDGET_WEATHER_IMG_SMALL_WIDTH),(MMIWIDGET_WEATHER_AFTER_TOMORROW_IMG_Y + MMIWIDGET_WEATHER_IMG_SMALL_HEIGHT)}, //后天小图片
 }; 

 //FOUCS 信息类型区域
LOCAL GUI_RECT_T s_weather_focus_position[MMIWIDGET_WEATHER_FOCUS_MAX ] = {
    {MMIWIDGET_WEATHER_TODAY_FOCUS_X,MMIWIDGET_WEATHER_TODAY_FOCUS_Y,(MMIWIDGET_WEATHER_TODAY_FOCUS_X + MMIWIDGET_WEATHER_CITY_FOUCS_WIDTH),(MMIWIDGET_WEATHER_TODAY_FOCUS_Y + MMIWIDGET_WEATHER_CITY_FOUCS_HEIGHT)},//今天温度选中
    {MMIWIDGET_WEATHER_TIME_FOCUS_X,MMIWIDGET_WEATHER_TIME_FOCUS_Y,(MMIWIDGET_WEATHER_TIME_FOCUS_X + MMIWIDGET_WEATHER_TIME_FOUCS_WIDTH),(MMIWIDGET_WEATHER_TIME_FOCUS_Y + MMIWIDGET_WEATHER_TIME_FOUCS_HEIGHT)},//时间选中
    {MMIWIDGET_WEATHER_CITY_FOCUS_X,MMIWIDGET_WEATHER_CITY_FOCUS_Y,(MMIWIDGET_WEATHER_CITY_FOCUS_X + MMIWIDGET_WEATHER_CITY_FOUCS_WIDTH),(MMIWIDGET_WEATHER_CITY_FOCUS_Y + MMIWIDGET_WEATHER_CITY_FOUCS_HEIGHT)},//城市选中
    {MMIWIDGET_WEATHER_TODAY_IMG_FOCUS_X,MMIWIDGET_WEATHER_TODAY_IMG_FOCUS_Y,(MMIWIDGET_WEATHER_TODAY_IMG_FOCUS_X + MMIWIDGET_WEATHER_IMG_BIG_FOUCS_WIDTH),(MMIWIDGET_WEATHER_TODAY_IMG_FOCUS_Y + MMIWIDGET_WEATHER_IMG_BIG_FOUCS_HEIGHT)},//今天图片选中
    {MMIWIDGET_WEATHER_TOMORROW_IMG_FOCUS_X,MMIWIDGET_WEATHER_TOMORROW_IMG_FOCUS_Y,(MMIWIDGET_WEATHER_TOMORROW_IMG_X + MMIWIDGET_WEATHER_IMG_SMALL_FOUCS_WIDTH),(MMIWIDGET_WEATHER_TOMORROW_IMG_Y + MMIWIDGET_WEATHER_IMG_SMALL_FOUCS_HEIGHT)},     //明天小图片
    {MMIWIDGET_WEATHER_AFTER_TOMORROW_IMG_FOCUS_X,MMIWIDGET_WEATHER_AFTER_TOMORROW_IMG_FOCUS_Y,(MMIWIDGET_WEATHER_AFTER_TOMORROW_IMG_FOCUS_X + MMIWIDGET_WEATHER_IMG_SMALL_FOUCS_WIDTH),(MMIWIDGET_WEATHER_AFTER_TOMORROW_IMG_FOCUS_Y + MMIWIDGET_WEATHER_IMG_SMALL_FOUCS_HEIGHT)}, //后天小图片
}; 


LOCAL const MMIWEATHER_TWO_WIDGET_T s_weather_two_widget_map[] = 
{
    {
    WEATHER_CONDITION_UNKNOW,
    IMAGE_WIDGET_WEATHER_NO_B,
    IMAGE_WIDGET_WEATHER_NO_S,
    },

    {
    WEATHER_CONDITION_CLEAR,
    IMAGE_WIDGET_WEATHER_CLEAR_B,
    IMAGE_WIDGET_WEATHER_CLEAR_S,
    },
    {
    WEATHER_CONDITION_FOG,
    IMAGE_WIDGET_WEATHER_FOG_B,
    IMAGE_WIDGET_WEATHER_FOG_S,
    },

    {
    WEATHER_CONDITION_HAZE,
    IMAGE_WIDGET_WEATHER_FOG_B,
    IMAGE_WIDGET_WEATHER_FOG_S,
    },

    {
    WEATHER_CONDITION_CLOUDY,
    IMAGE_WIDGET_WEATHER_CLOUDY_B,
    IMAGE_WIDGET_WEATHER_CLOUDY_S,
    },

    {
    WEATHER_CONDITION_PARTLY_CLOUDY,
    IMAGE_WIDGET_WEATHER_CHANGE_B,
    IMAGE_WIDGET_WEATHER_CHANGE_S,
    },

    {
    WEATHER_CONDITION_MOSTLY_CLOUDY,
    IMAGE_WIDGET_WEATHER_CLOUDY_B,
    IMAGE_WIDGET_WEATHER_CLOUDY_S,
    },

    {
    WEATHER_CONDITION_OVERCAST,
    IMAGE_WIDGET_WEATHER_CLOUDY_B,
    IMAGE_WIDGET_WEATHER_CLOUDY_S,
    },

    {
    WEATHER_CONDITION_RAIN,                
    IMAGE_WIDGET_WEATHER_RAIN_B,
    IMAGE_WIDGET_WEATHER_RAIN_S,   
    },

    {

    WEATHER_CONDITION_LIGHT_RAIN,          
    IMAGE_WIDGET_WEATHER_RAIN_B,
    IMAGE_WIDGET_WEATHER_RAIN_S,   
    },

    {
    WEATHER_CONDITION_RAIN_SHOWERS,        
    IMAGE_WIDGET_WEATHER_RAIN_B,
    IMAGE_WIDGET_WEATHER_RAIN_S,   
    },

    {
    WEATHER_CONDITION_SCATTERED_SHOWERS,   
    IMAGE_WIDGET_WEATHER_RAIN_B,
    IMAGE_WIDGET_WEATHER_RAIN_S,   
    },

    {
    WEATHER_CONDITION_SHOWERS,             
    IMAGE_WIDGET_WEATHER_RAIN_B,
    IMAGE_WIDGET_WEATHER_RAIN_S,   
    },

    {
    WEATHER_CONDITION_THUNDERSTORM,       
    IMAGE_WIDGET_WEATHER_THUNDER_B,
    IMAGE_WIDGET_WEATHER_THUNDER_S,   
    },

    {
    WEATHER_CONDITION_CHANCE_OF_SHOWERS,   
    IMAGE_WIDGET_WEATHER_RAIN_B,
    IMAGE_WIDGET_WEATHER_RAIN_S,   
    },

    {
    WEATHER_CONDITION_CHANCE_OF_SNOW,      
    IMAGE_WIDGET_WEATHER_SNOW_B,
    IMAGE_WIDGET_WEATHER_SNOW_S,   
    },

    {
    WEATHER_CONDITION_CHANCE_OF_STORM,     
    IMAGE_WIDGET_WEATHER_RAIN_B,
    IMAGE_WIDGET_WEATHER_RAIN_S,   
    },

    {
    WEATHER_CONDITION_SUNNY,               
    IMAGE_WIDGET_WEATHER_CHANGE_B,
    IMAGE_WIDGET_WEATHER_CHANGE_S,
    },

    {
    WEATHER_CONDITION_MOSTLY_SUNNY,        
    IMAGE_WIDGET_WEATHER_CLEAR_B,
    IMAGE_WIDGET_WEATHER_CLEAR_S,
    },

    {
    WEATHER_CONDITION_PARTLY_SUNNY,        
    IMAGE_WIDGET_WEATHER_CHANGE_B,
    IMAGE_WIDGET_WEATHER_CHANGE_S,
    },

    {
    WEATHER_CONDITION_MIST,                
    IMAGE_WIDGET_WEATHER_FOG_B,
    IMAGE_WIDGET_WEATHER_FOG_S,
    },

    {
    WEATHER_CONDITION_SMOKE,               
    IMAGE_WIDGET_WEATHER_FOG_B,
    IMAGE_WIDGET_WEATHER_FOG_S,
    },

    {
    WEATHER_CONDITION_STORM,               
    IMAGE_WIDGET_WEATHER_RAIN_B,
    IMAGE_WIDGET_WEATHER_RAIN_S,
    },

    {
    WEATHER_CONDITION_FLURRIES,            
    IMAGE_WIDGET_WEATHER_RAIN_B,
    IMAGE_WIDGET_WEATHER_RAIN_S,
    },

    {
    WEATHER_CONDITION_CHANCE_OF_RAIN,      
    IMAGE_WIDGET_WEATHER_RAIN_B,
    IMAGE_WIDGET_WEATHER_RAIN_S,
    },

    {
    WEATHER_CONDITION_CHANCE_OF_TSTORM,    
    IMAGE_WIDGET_WEATHER_RAIN_B,
    IMAGE_WIDGET_WEATHER_RAIN_S,
    },

    {
    WEATHER_CONDITION_SLEET,               
    IMAGE_WIDGET_WEATHER_SNOW_B,
    IMAGE_WIDGET_WEATHER_SNOW_S,
    },
    {
    WEATHER_CONDITION_SNOW,                
    IMAGE_WIDGET_WEATHER_SNOW_B,
    IMAGE_WIDGET_WEATHER_SNOW_S,
    },

    {
    WEATHER_CONDITION_ICY,                 
    IMAGE_WIDGET_WEATHER_ICE_B,
    IMAGE_WIDGET_WEATHER_ICE_S,
    },
    {
    WEATHER_CONDITION_DUST,                
    IMAGE_WIDGET_WEATHER_CLOUDY_B,
    IMAGE_WIDGET_WEATHER_CLOUDY_S,
    },

    {
    WEATHER_CONDITION_SCATTERED_THUNDERSTORMS,  
    IMAGE_WIDGET_WEATHER_THUNDER_B,
    IMAGE_WIDGET_WEATHER_THUNDER_S,
    },
};
//LOCAL GUI_LCD_DEV_INFO  weathertwo_highlight_layer_info = {0,UILAYER_NULL_HANDLE};
LOCAL  MMIWIDGET_WEATHER_FOCUS_POSITION_TYPE_E  weathertwo_cur_focus_index = 0xff;

//需要显示的字符串最大长度
#define DIS_TEXT_MAX_LEN   12

//当前点击区域类型
LOCAL MMIWIDGET_WEATHER_CLICK_RECT_E s_tp_hit_type = MMIWIDGET_WEATHER_CLICK_RECT_INVALID;



LOCAL CUR_WEAHTER_STATUS cur_weather_status = WEATHER_STATUS_MAX;
/*zhm end*/

//当前提示字符串类型
LOCAL MMIWIDGET_WEATHER_DIS_PROMPT_STR_TYPE_E s_Need_dis_prompt_str_type = MMIWIDGET_WEATHER_DIS_PROMPT_STR_TYPE_INVALID;

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//    Description : widget界面事件处理
//    Global resource dependence : 
//    Author:   cfy
//    Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleWidgetWeatherTwoWinMsg(
    MMI_WIN_ID_T        win_id,        //IN:
    MMI_MESSAGE_ID_E    msg_id,        //IN:
    DPARAM            param        //IN:
);

/*****************************************************************************/
//  Description : display WeatherTwo Panel
//  Global resource dependence : 
//  Author: cfy
//  Note:
/*****************************************************************************/
LOCAL void WeatherTwo_DisplayWeatherBg(
                           MMI_WIN_ID_T     win_id
                           );

/*****************************************************************************/
//    Description :在天气预报widget上显示当前时间。
//    Global resource dependence : 
//    Author:  cfy
//    Note:
/*****************************************************************************/
LOCAL void WeatherTwo_DisplayCurrentTime(void);

/*****************************************************************************/
//    Description : 获取区域
//    Global resource dependence : 
//    Author:   cfy
//    Note:
/*****************************************************************************/

LOCAL GUI_RECT_T WeatherTwo_CalcRect(MMIWIDGET_WEATHER_POSITION_TYPE_E index);


/*****************************************************************************/
//    Description : 获取选中区域
//    Global resource dependence : 
//    Author:   cfy
//    Note:
/*****************************************************************************/

LOCAL GUI_RECT_T WeatherTwo_CalcFoucsRect(MMIWIDGET_WEATHER_FOCUS_POSITION_TYPE_E index);


/*****************************************************************************/
//    Description : 显示天气信息
//    Global resource dependence : 
//    Author:   cfy
//    Note:
/*****************************************************************************/
LOCAL void WeatherTwo_DisplayWeatherInfo(MMI_WIN_ID_T        win_id);



LOCAL MMIWEATHER_CONDITION_E WeatherTwo_GetConditionbyDay(SELECT_WEEK_DAY week_day);
LOCAL MMI_IMAGE_ID_T Weathertwo_GetWidgetConditionImgId(MMIWEATHER_CONDITION_E condition_id, WIDGET_ICON_TYPE  icon_type);
LOCAL MMI_IMAGE_ID_T WeatherTwo_GetWidgetConditionImgIdbyDay(SELECT_WEEK_DAY week_day);
LOCAL wchar* WeatherTwo_GetTempRangeWstrbyDay(SELECT_WEEK_DAY week_day);
LOCAL void WeatherTwo_GetDisplayText(MMI_STRING_T* Text_Ptr, //out 字串指针
                                                                  MMIWIDGET_WEATHER_DISPALY_TEXT_TYPE_E dis_type,//in 
                                                                            SELECT_WEEK_DAY WEEK_DAY);
LOCAL void WeatherTwo_SetCurWeatherStatus(void);
LOCAL WIDGET_WEATHER_RECT_TYPE WeatherTwo_GetRectTypebyIndex(MMIWIDGET_WEATHER_FOCUS_POSITION_TYPE_E rect_index);
LOCAL void WeatherTwo_UpdateWeather(void);
LOCAL MMI_RESULT_E WeatherTwo_HandleWidgetTpUpMSG(MMI_WIN_ID_T  win_id,   DPARAM  param );
LOCAL BOOLEAN WeatherTwo_GetFocusRectnIndex(GUI_POINT_T point, //in
                                                                                        GUI_RECT_T   *rect, //out
                                                                                        MMIWIDGET_WEATHER_FOCUS_POSITION_TYPE_E *rect_index //out
                                                                                        );
LOCAL MMI_RESULT_E WeatherTwo_HandleWidgetTpDownMSG(MMI_WIN_ID_T  win_id,   DPARAM  param );
//LOCAL BOOLEAN WeatherTwo_CreatHighlightLayer(MMI_WIN_ID_T     win_id);
//LOCAL BOOLEAN WeatherTwo_DeleteHighlightLayer(void);
//LOCAL void WeatherTwo_DisplayHighlightImg(MMI_WIN_ID_T  win_id,  GUI_RECT_T  *border_rect_ptr);
//LOCAL void WeatherTwo_ResetWidgetWeather(void);
LOCAL void WeatherTwo_DisplayWeatherWidget( MMI_WIN_ID_T        win_id);
LOCAL MMI_RESULT_E WeatherTwo_HandleWebKeyMSG(void);
//LOCAL void WeatherTwo_DisplayFocusRect(MMI_WIN_ID_T win_id);

//天气预报widget
PUBLIC const MMIWIDGET_ITEM_INFO_T g_weather_widget2 =
{
        MMIWIDGET_WEATHER2_ID, 
		HandleWidgetWeatherTwoWinMsg, 
		PNULL,
		PNULL,//WeatherTwo_ResetWidgetWeather,
		PNULL,		
		WIDGET_WEATHER_TWO_WIN_ID,
		IMAGE_WIDGET_WEATHER_TWO_SHORTCUT,
		IMAGE_WIDGET_WEATHER_TWO_BG,
		TXT_WIDGET_WEATHER_TWO,
		MMIWIDGET_WEATHER_DEFAULT_X,
		MMIWIDGET_WEATHER_DEFAULT_Y,
		FALSE,
};
//zhm end
/**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/

/*****************************************************************************/
//    Description : widget界面事件处理
//    Global resource dependence : 
//    Author:   cfy
//    Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleWidgetWeatherTwoWinMsg(
    MMI_WIN_ID_T        win_id,        //IN:
    MMI_MESSAGE_ID_E    msg_id,        //IN:
    DPARAM            param        //IN:
)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    //static BOOLEAN is_need_dis_highlight = TRUE;

    //SCI_TRACE_LOW:"HandleWidgetWeatherWinMsg msg_id = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_WEATHER2_630_112_2_18_3_8_53_256,(uint8*)"d", msg_id);
	
    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:
            TM_GetSysTime(&s_sys_time);
            //WeatherTwo_ResetWidgetWeather();
            break;

        case MSG_IDLE_UPDATE_DATETIME:
        case MSG_FULL_PAINT:
            WeatherTwo_SetCurWeatherStatus();
            WeatherTwo_DisplayWeatherWidget(win_id);
            //if(is_need_dis_highlight)
            //    WeatherTwo_DisplayFocusRect(win_id);
            break;
                
        case MSG_GET_FOCUS:
            break;            
        case MSG_LOSE_FOCUS:
            weathertwo_cur_focus_index = 0xff;        
            break;
        case MSG_APP_UP:
            if(MMIWIDGET_WEATHER_FOCUS_AFTER_TOMORROW_IMG < weathertwo_cur_focus_index
                    || MMIWIDGET_WEATHER_FOCUS_TODAY == weathertwo_cur_focus_index)
            {
                weathertwo_cur_focus_index = MMIWIDGET_WEATHER_FOCUS_AFTER_TOMORROW_IMG;
            }
            else
            {
                weathertwo_cur_focus_index--;
            }         
            recode = MMI_RESULT_FALSE;
            break;
        case MSG_APP_DOWN:
            if(MMIWIDGET_WEATHER_FOCUS_AFTER_TOMORROW_IMG <= weathertwo_cur_focus_index)
            {
                weathertwo_cur_focus_index = MMIWIDGET_WEATHER_FOCUS_TODAY;
            }
            else
            {
                weathertwo_cur_focus_index++;
            }        
            recode = MMI_RESULT_FALSE;
            break;
        case MSG_APP_WEB:
            recode = WeatherTwo_HandleWebKeyMSG();
            break;
        case MSG_WIDGET_TP_PRESS_UP:
            //is_need_dis_highlight = TRUE;
            recode= WeatherTwo_HandleWidgetTpUpMSG(win_id, param);
            break;
        case MSG_WIDGET_TP_PRESS_DOWN:
            recode = WeatherTwo_HandleWidgetTpDownMSG(win_id, param);
            break;
        //case MSG_WIDGET_TP_PRESS_LONG_DOWN:
            //is_need_dis_highlight = FALSE;
            //WeatherTwo_ResetWidgetWeather();
            //break;
        case MSG_CLOSE_WINDOW:
            weathertwo_cur_focus_index = 0xff;
            //WeatherTwo_ResetWidgetWeather();
            break;
        default:
            recode = MMI_RESULT_FALSE;
            break;
    }
    
    return recode;        
}


PUBLIC BOOLEAN GUIRES_DisplayFrameBySmallImg(
                                 GUI_RECT_T             *display_rect_ptr,  //in:lcd display rect,
                                 int16    frame_width,    //in:frame_width
                                 int16    frame_heigh,    //in:frame_heigh
                                 MMI_WIN_ID_T           win_id,             //in:win id
                                 MMI_IMAGE_ID_T         img_id,             //in:image id
                                 const GUI_LCD_DEV_INFO *dev_info_ptr       //in:
                                 )

{
    BOOLEAN result = TRUE;
    uint16          image_width= 0;
    uint16          image_height = 0;
    uint16          step_length = 0;

    GUI_RECT_T dest_trans_area = {0};
    GUI_RECT_T dst_area_temp = {0};
    GUI_RECT_T src_area_temp = {0};
    GUI_RECT_T image_area = {0};
    
    
    if (PNULL == dev_info_ptr
        || ((PNULL == display_rect_ptr)
        || (IMAGE_NULL == img_id)))
    {
        return FALSE;
    }


    GUIRES_GetImgWidthHeight(
            &image_width, &image_height,
            img_id,
            win_id);
            
    image_area.right = image_width -1;
    image_area.bottom = image_height -1;
    
    if(frame_width == 0 || frame_heigh ==0)
    {
        frame_width = image_width/4;
        frame_heigh = image_height/4;
    }
    
    dest_trans_area.left = display_rect_ptr->left + frame_width;
    dest_trans_area.top = display_rect_ptr->top + frame_heigh;
    dest_trans_area.right = display_rect_ptr->right - frame_width;
    dest_trans_area.bottom = display_rect_ptr->bottom - frame_heigh;

//    if(GUI_IsInvalidRect(dest_trans_area))
//    {
//        return result;
//    }

    //first corner
    dst_area_temp = *display_rect_ptr;
    dst_area_temp.right = dst_area_temp.left + frame_width -1;
    dst_area_temp.bottom = dst_area_temp.top + frame_heigh -1;

    src_area_temp = image_area;
    src_area_temp.right = src_area_temp.left + frame_width -1;
    src_area_temp.bottom = src_area_temp.top + frame_heigh -1;
    GUIRES_DisplayImg(PNULL,&dst_area_temp,&src_area_temp,win_id,img_id,dev_info_ptr);


    //first frame
    src_area_temp = image_area;
    src_area_temp.bottom = src_area_temp.top + frame_heigh -1;
    src_area_temp.left += frame_width;
    src_area_temp.right -= frame_width;
    step_length = src_area_temp.right - src_area_temp.left + 1;;
    
    do
    {
        if(((dst_area_temp.right + step_length)>=dest_trans_area.right))
        {
            uint16 step_length_temp = dest_trans_area.right-dst_area_temp.right;

            dst_area_temp.right += step_length_temp;
            dst_area_temp.left = dst_area_temp.right - step_length_temp + 1;
        }
        else
        {
            dst_area_temp.right += step_length;
            dst_area_temp.left = dst_area_temp.right - step_length + 1;
        }
        GUIRES_DisplayImg(PNULL,&dst_area_temp,&src_area_temp,win_id,img_id,dev_info_ptr);
    }
    while(dst_area_temp.right < dest_trans_area.right);


    
    //second corner
    dst_area_temp = *display_rect_ptr;
    dst_area_temp.left = dst_area_temp.right - frame_width +1;
    dst_area_temp.bottom = dst_area_temp.top + frame_heigh -1;

    src_area_temp = image_area;
    src_area_temp.left = src_area_temp.right - frame_width + 1;
    src_area_temp.bottom = src_area_temp.top + frame_heigh -1;
    GUIRES_DisplayImg(PNULL,&dst_area_temp,&src_area_temp,win_id,img_id,dev_info_ptr);


    //second frame
    src_area_temp = image_area;
    src_area_temp.left = src_area_temp.right - frame_heigh +1;
    src_area_temp.top += frame_heigh;
    src_area_temp.bottom -= frame_heigh;
    step_length = src_area_temp.bottom - src_area_temp.top + 1;;
    
    do
    {
        if(((dst_area_temp.bottom + step_length)>=dest_trans_area.bottom))
        {
            uint16 step_length_temp = dest_trans_area.bottom-dst_area_temp.bottom;

            dst_area_temp.bottom += step_length_temp;
            dst_area_temp.top = dst_area_temp.bottom - step_length_temp + 1;
        }
        else
        {
            dst_area_temp.bottom += step_length;
            dst_area_temp.top = dst_area_temp.bottom - step_length + 1;
        }
        GUIRES_DisplayImg(PNULL,&dst_area_temp,&src_area_temp,win_id,img_id,dev_info_ptr);
    }
    while(dst_area_temp.bottom < dest_trans_area.bottom);


    
    //trird corner
    dst_area_temp = *display_rect_ptr;
    dst_area_temp.right = dst_area_temp.left + frame_width -1;
    dst_area_temp.top = dst_area_temp.bottom - frame_heigh +1;

    src_area_temp = image_area;
    src_area_temp.right = src_area_temp.left + frame_width -1;
    src_area_temp.top = src_area_temp.bottom - frame_heigh +1;
    GUIRES_DisplayImg(PNULL,&dst_area_temp,&src_area_temp,win_id,img_id,dev_info_ptr);

    //third frame

    dst_area_temp = *display_rect_ptr;
    dst_area_temp.right = dst_area_temp.left + frame_width -1;
    dst_area_temp.top = dst_area_temp.bottom - frame_heigh + 1;

    src_area_temp = image_area;
    src_area_temp.top = src_area_temp.bottom - frame_heigh + 1;
    src_area_temp.left += frame_width;
    src_area_temp.right -= frame_width;
    step_length = src_area_temp.right - src_area_temp.left + 1;;
    
    do
    {
        if(((dst_area_temp.right + step_length)>=dest_trans_area.right))
        {
            uint16 step_length_temp = dest_trans_area.right-dst_area_temp.right;

            dst_area_temp.right += step_length_temp;
            dst_area_temp.left = dst_area_temp.right - step_length_temp + 1;
        }
        else
        {
            dst_area_temp.right += step_length;
            dst_area_temp.left = dst_area_temp.right - step_length + 1;
        }
        GUIRES_DisplayImg(PNULL,&dst_area_temp,&src_area_temp,win_id,img_id,dev_info_ptr);
    }
    while(dst_area_temp.right < dest_trans_area.right);


    //forth corner
    dst_area_temp = *display_rect_ptr;
    dst_area_temp.left = dst_area_temp.right - frame_width +1;
    dst_area_temp.top = dst_area_temp.bottom - frame_heigh +1;

    src_area_temp = image_area;
    src_area_temp.left  = src_area_temp.right - frame_width +1;
    src_area_temp.top = src_area_temp.bottom - frame_heigh +1;
    GUIRES_DisplayImg(PNULL,&dst_area_temp,&src_area_temp,win_id,img_id,dev_info_ptr);

    //forth frame

    dst_area_temp = *display_rect_ptr;
    dst_area_temp.right = dst_area_temp.left + frame_width -1;
    dst_area_temp.bottom = dst_area_temp.top + frame_heigh -1;

    src_area_temp = image_area;
    src_area_temp.right = src_area_temp.left + frame_heigh -1;
    src_area_temp.top += frame_heigh;
    src_area_temp.bottom -= frame_heigh;
    step_length = src_area_temp.bottom - src_area_temp.top + 1;;
    
    do
    {
        if(((dst_area_temp.bottom + step_length)>=dest_trans_area.bottom))
        {
            uint16 step_length_temp = dest_trans_area.bottom-dst_area_temp.bottom;

            dst_area_temp.bottom += step_length_temp;
            dst_area_temp.top = dst_area_temp.bottom - step_length_temp + 1;
        }
        else
        {
            dst_area_temp.bottom += step_length;
            dst_area_temp.top = dst_area_temp.bottom - step_length + 1;
        }
        GUIRES_DisplayImg(PNULL,&dst_area_temp,&src_area_temp,win_id,img_id,dev_info_ptr);
    }
    while(dst_area_temp.bottom < dest_trans_area.bottom);
    return result;
}


/*****************************************************************************/
//  Description : display SpeedDial Panel
//  Global resource dependence : 
//  Author: cfy
//  Note:
/*****************************************************************************/
LOCAL void WeatherTwo_DisplayWeatherBg(
                           MMI_WIN_ID_T     win_id
                           )
{
    GUI_RECT_T		win_rect = {0};
    GUI_LCD_DEV_INFO lcd_dev_info = {0};
    GUI_POINT_T		point = {0};

    MMK_GetWinRect(win_id, &win_rect);
    MMK_GetWinLcdDevInfo(win_id, &lcd_dev_info);
    //显示背景图
    point.x = win_rect.left;
    point.y = win_rect.top;
    
    if(UILAYER_HANDLE_MULTI == UILAYER_GetHandleType(&lcd_dev_info))
    {
        UILAYER_ClearRect(&lcd_dev_info,win_rect);
    }

    GUIRES_DisplayImg(&point, PNULL, PNULL, win_id,
        IMAGE_WIDGET_WEATHER_TWO_BG, &lcd_dev_info);
#if 0
{
    int8 i = MMIWIDGET_WEATHER_TODAY;
    GUI_RECT_T rect = {0};
        
    for(i = MMIWIDGET_WEATHER_FOCUS_TODAY; i < MMIWIDGET_WEATHER_FOCUS_MAX; i++)
        {
        rect = CalcFoucsRect(i);
        GUI_FillRect(&lcd_dev_info,rect, MMI_RED_COLOR);
        }
        

    for(i = MMIWIDGET_WEATHER_TODAY; i < MMIWIDGET_WEATHER_MAX; i++)
        {
        rect = CalcRect(i);
        GUI_FillRect(&lcd_dev_info,rect, MMI_GRAY_COLOR);
        }
}
#else
    //MMIWIDGET_Weather_Display_WeatherInfo(win_id);
#endif 
}
/*****************************************************************************/
//    Description :在天气预报widget上显示当前时间。
//    Global resource dependence : 
//    Author:  cfy
//    Note:
/*****************************************************************************/
LOCAL void WeatherTwo_DisplayCurrentTime(void)
{
    GUI_LCD_DEV_INFO    weather_widget_layer = {0};
    GUI_RECT_T          win_rect        = {0};
    GUI_POINT_T         display_point   = {0};
    SCI_TIME_T          sys_time        = s_sys_time;
    MMI_STRING_T name_str_temp={0};
    char           time_str[MMIWIDGET_WEATHER_TIME_TEXT_MAX] = {0};
    wchar           time_wstr[MMIWIDGET_WEATHER_TIME_TEXT_MAX] = {0};
    uint16 str_len = 0;

    GUI_RECT_T			     rect = {0};
    GUISTR_STYLE_T      text_style      = {0}; /*lint !e64*/
    GUISTR_STATE_T      state = GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE|GUISTR_STATE_NOT_HALF_CHAR;  

    text_style.align = ALIGN_LVMIDDLE;//左对齐
    text_style.font = MMIWIDGET_WEATHER_TIME_FONT;
    text_style.font_color = MMIWIDGET_WEATHER_TIME_COLOR;
    text_style.line_space = 4;

    //SCI_TRACE_LOW:"[mmiwidget_weather_two.c]:DisplayCurrentTime---%02d : %02d ---"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_WEATHER2_978_112_2_18_3_8_54_257,(uint8*)"dd" ,sys_time.hour, sys_time.min);
    
    MMK_GetWinRect(g_weather_widget2.win_id, &win_rect);
    MMK_GetWinLcdDevInfo(g_weather_widget2.win_id, &weather_widget_layer);  
    
    TM_GetSysTime(&s_sys_time);
    sys_time        = s_sys_time;

    if (MMISET_TIME_12HOURS == MMIAPISET_GetTimeDisplayType())
    {
        MMIAPISET_GetTimeStr12((uint8*)time_str);
    }
    else
    {
        MMIAPISET_GetTimeStr24((uint8*)time_str);
    }
    //sprintf(time_str, "%02d:%02d", sys_time.hour, sys_time.min);
     (void)MMI_STRNTOWSTR(time_wstr, MMIWIDGET_WEATHER_TIME_TEXT_MAX, (uint8*)time_str, MMIWIDGET_WEATHER_TIME_TEXT_MAX, (size_t)SCI_STRLEN(time_str));

    name_str_temp.wstr_ptr = time_wstr;
    name_str_temp.wstr_len=MMIAPICOM_Wstrlen(time_wstr);
    
    str_len = (name_str_temp.wstr_len>MMIWIDGET_WEATHER_TIME_TEXT_MAX)?MMIWIDGET_WEATHER_TIME_TEXT_MAX:name_str_temp.wstr_len;
    name_str_temp.wstr_len = str_len;
    
    rect = WeatherTwo_CalcRect(MMIWIDGET_WEATHER_TIME);
    
    GUISTR_DrawTextToLCDInRect(        (const GUI_LCD_DEV_INFO *)&weather_widget_layer,
        (const GUI_RECT_T      *)&rect,       //the fixed display area
        (const GUI_RECT_T      *)&rect,       //用户要剪切的实际区域
        (const MMI_STRING_T    *)&name_str_temp,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        );
}
/*****************************************************************************/
//    Description : 获取区域
//    Global resource dependence : 
//    Author:  cfy
//    Note:
/*****************************************************************************/

LOCAL GUI_RECT_T WeatherTwo_CalcRect(MMIWIDGET_WEATHER_POSITION_TYPE_E index)
{
    GUI_RECT_T          win_rect        = {0};
    GUI_RECT_T           rect ={0};
    MMK_GetWinRect(g_weather_widget2.win_id, &win_rect); 

    rect.left = win_rect.left + s_weather_position[index].left;
    rect.top = win_rect.top + s_weather_position[index].top;
    rect.right = win_rect.left + s_weather_position[index].right;
    rect.bottom = win_rect.top + s_weather_position[index].bottom;

    return rect;
}
/*****************************************************************************/
//    Description : 获取选中区域
//    Global resource dependence : 
//    Author:   cfy
//    Note:
/*****************************************************************************/

LOCAL GUI_RECT_T WeatherTwo_CalcFoucsRect(MMIWIDGET_WEATHER_FOCUS_POSITION_TYPE_E index)
{
    GUI_RECT_T          win_rect        = {0};
    GUI_RECT_T           rect ={0};
    MMK_GetWinRect(g_weather_widget2.win_id, &win_rect); 

    rect.left = win_rect.left + s_weather_focus_position[index].left;
    rect.top = win_rect.top + s_weather_focus_position[index].top;
    rect.right = win_rect.left + s_weather_focus_position[index].right;
    rect.bottom = win_rect.top + s_weather_focus_position[index].bottom;

    return rect;
}
/*****************************************************************************/
//    Description : 显示天气信息
//    Global resource dependence : 
//    Author:   cfy
//    Note:
/*****************************************************************************/
LOCAL void WeatherTwo_DisplayWeatherInfo(MMI_WIN_ID_T        win_id)
{
    GUI_LCD_DEV_INFO     lcd_dev_info    = {0};
    
    MMK_GetWinLcdDevInfo(win_id,&lcd_dev_info);
    
    if (UILAYER_IsLayerActive(&lcd_dev_info))
    {  
        MMI_IMAGE_ID_T       icon_id         = IMAGE_WIDGET_WEATHER_NO_B;   
        GUISTR_STYLE_T text_style = {0};
        MMI_STRING_T   tempStr = {0};
        char chr[DIS_TEXT_MAX_LEN] = {0};
        GUISTR_STATE_T text_state = GUISTR_STATE_ALIGN|GUISTR_STATE_EFFECT|GUISTR_STATE_EFFECT|GUISTR_STATE_ELLIPSIS;
        GUI_RECT_T     rect  = {0}; 
 
        MMIWEATHER_WEATHER_T*   weather_info = MMIWEATHER_GetCurCityWeatherInfo();
        
        icon_id = WeatherTwo_GetWidgetConditionImgIdbyDay(TODAY);
        rect = WeatherTwo_CalcRect(MMIWIDGET_WEATHER_TODAY_IMG);//今天图片
        GUIRES_DisplayImg(PNULL, &rect, PNULL, win_id, icon_id, &lcd_dev_info);
        
        text_style.effect = 0;
        text_style.angle = ANGLE_0;
        text_style.align = ALIGN_LVMIDDLE;
        text_style.font = MMIWIDGET_WEATHER_TEXT_FONT;
        text_style.font_color = MMIWIDGET_WEATHER_TEXT_COLOR;

        text_style.align = ALIGN_LVMIDDLE;
        rect = WeatherTwo_CalcRect(MMIWIDGET_WEATHER_CITY);//城市
        WeatherTwo_GetDisplayText(&tempStr, MMIWIDGET_WEATHER_DISPALY_TEXT_TYPE_CITYNAME, WEEK_DAY_INVALID);
        GUISTR_DrawTextToLCDInRect(&lcd_dev_info, &rect, &rect, &tempStr,	&text_style, text_state, GUISTR_TEXT_DIR_AUTO);
        
        text_style.align = ALIGN_RVMIDDLE;        
        rect = WeatherTwo_CalcRect(MMIWIDGET_WEATHER_TODAY);//今天温度
        WeatherTwo_GetDisplayText(&tempStr, MMIWIDGET_WEATHER_DISPALY_TEXT_TYPE_TEMPRANGE, TODAY);
        GUISTR_DrawTextToLCDInRect(&lcd_dev_info, &rect, &rect, &tempStr,	&text_style, text_state, GUISTR_TEXT_DIR_AUTO);       
        
        text_style.align = ALIGN_RVMIDDLE;
        rect = WeatherTwo_CalcRect(MMIWIDGET_WEATHER_CITY);//今天环境
        WeatherTwo_GetDisplayText(&tempStr, MMIWIDGET_WEATHER_DISPALY_TEXT_TYPE_CONDITION, TODAY);
        GUISTR_DrawTextToLCDInRect(&lcd_dev_info, &rect, &rect, &tempStr,	&text_style, text_state, GUISTR_TEXT_DIR_AUTO);         

        icon_id = WeatherTwo_GetWidgetConditionImgIdbyDay(TOMORROW);
        rect = WeatherTwo_CalcRect(MMIWIDGET_WEATHER_TOMORROW_IMG);//明天图片
        GUIRES_DisplayImg(PNULL, &rect, PNULL, win_id, icon_id, &lcd_dev_info);
        
        text_style.align = ALIGN_LVMIDDLE;        
        rect = WeatherTwo_CalcRect(MMIWIDGET_WEATHER_TOMORROW);//明天
        WeatherTwo_GetDisplayText(&tempStr, MMIWIDGET_WEATHER_DISPALY_TEXT_TYPE_WEEKDAY_STRING, TOMORROW);
        GUISTR_DrawTextToLCDInRect(&lcd_dev_info, &rect, &rect, &tempStr,	&text_style, text_state, GUISTR_TEXT_DIR_AUTO);
        
        text_style.align = ALIGN_RVMIDDLE;        
        rect = WeatherTwo_CalcRect(MMIWIDGET_WEATHER_TOMORROW);//明天温度
        WeatherTwo_GetDisplayText(&tempStr, MMIWIDGET_WEATHER_DISPALY_TEXT_TYPE_TEMPRANGE, TOMORROW);
        GUISTR_DrawTextToLCDInRect(&lcd_dev_info, &rect, &rect, &tempStr,	&text_style, text_state, GUISTR_TEXT_DIR_AUTO);

        icon_id = WeatherTwo_GetWidgetConditionImgIdbyDay(DAY_AFTER_TOMORROW);
        rect = WeatherTwo_CalcRect(MMIWIDGET_WEATHER_AFTER_TOMORROW_IMG);//后天图片
        GUIRES_DisplayImg(PNULL, &rect, PNULL, win_id, icon_id, &lcd_dev_info); 

        text_style.align = ALIGN_LVMIDDLE;        
        rect = WeatherTwo_CalcRect(MMIWIDGET_WEATHER_AFTER_TOMORROW);//后天
        WeatherTwo_GetDisplayText(&tempStr, MMIWIDGET_WEATHER_DISPALY_TEXT_TYPE_WEEKDAY_STRING, DAY_AFTER_TOMORROW);
        GUISTR_DrawTextToLCDInRect(&lcd_dev_info, &rect, &rect, &tempStr,	&text_style, text_state, GUISTR_TEXT_DIR_AUTO);
        
        text_style.align = ALIGN_RVMIDDLE;        
        rect = WeatherTwo_CalcRect(MMIWIDGET_WEATHER_AFTER_TOMORROW);//后天温度
        WeatherTwo_GetDisplayText(&tempStr, MMIWIDGET_WEATHER_DISPALY_TEXT_TYPE_TEMPRANGE, DAY_AFTER_TOMORROW);
        GUISTR_DrawTextToLCDInRect(&lcd_dev_info, &rect, &rect, &tempStr,	&text_style, text_state, GUISTR_TEXT_DIR_AUTO);      
    }
}

 
//得到WIDGET天气预报的图片
LOCAL MMI_IMAGE_ID_T Weathertwo_GetWidgetConditionImgId(MMIWEATHER_CONDITION_E condition_id, WIDGET_ICON_TYPE  icon_type)
{
    MMI_IMAGE_ID_T image_id = IMAGE_WIDGET_WEATHER_NO_B;
    int32 i = 0, n = sizeof (s_weather_two_widget_map) / sizeof (s_weather_two_widget_map[0]);

    for (i = 0; i < n; i ++)
    {
        if (s_weather_two_widget_map[i].condition_id == condition_id)
        {
            switch(icon_type)
            {
            case BIG_WIDGET_ICON:
                image_id = s_weather_two_widget_map[i].image_widget_big_id;
                break;

            case SMALL_WIDGET_ICON:
                image_id = s_weather_two_widget_map[i].image_widget_small_id;
                break;

            default:
                break;
            }
            break;
        }
    }
    
    return image_id;
}
//得到今天明天后天的天气情况的图标
LOCAL MMI_IMAGE_ID_T WeatherTwo_GetWidgetConditionImgIdbyDay(SELECT_WEEK_DAY week_day)
{
       MMIWEATHER_WEATHER_T*   weather_info = MMIWEATHER_GetCurCityWeatherInfo();
       MMI_IMAGE_ID_T image_id = IMAGE_WIDGET_WEATHER_NO_B;
       
       switch(week_day)
       {
            case TODAY:
                image_id = Weathertwo_GetWidgetConditionImgId(weather_info->current.condition, BIG_WIDGET_ICON);
                break;
            case TOMORROW:
                image_id = Weathertwo_GetWidgetConditionImgId(weather_info->forecast.weather2, SMALL_WIDGET_ICON);
                break;
            case DAY_AFTER_TOMORROW:
                image_id = Weathertwo_GetWidgetConditionImgId(weather_info->forecast.weather3, SMALL_WIDGET_ICON);
                break;
            default:
                break;
       }
       
       return image_id;
}

//得到气温范围
LOCAL wchar* WeatherTwo_GetTempRangeWstrbyDay(SELECT_WEEK_DAY week_day)
{
    MMI_STRING_T        unit_str  = {0};
    int32               temp_low  = 0;
    int32               temp_high = 0;
    WEATHER_TEMP_UNIT_E temp_unit = 0;
    uint8               temp_range_str[MMIWEATHER_TEMP_RANGE_STR_LEN + 1] = {0};
    MMIWEATHER_SETTING_T* setting_ptr  = MMIWEATHER_GetSettingInfo();
    MMIWEATHER_WEATHER_T* weather_info = MMIWEATHER_GetCurCityWeatherInfo();
    uint8               cur_city_index = MMIWEATHER_GetCurCityIndex();
    
    //SCI_TRACE_LOW:"[weather]:MMIWEATHER_GetCurTempRangeStr---"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_WEATHER2_1192_112_2_18_3_8_55_258,(uint8*)"");
    
    //clean old data
    SCI_MEMSET(weather_info->temp_range_wstr, 0, sizeof(wchar)*(MMIWEATHER_TEMP_RANGE_STR_LEN + 1));
    
    if( setting_ptr->city_num > 0 && MMIWEATHER_IsCurCityWeatherInfoLegal())
    {
        temp_low  = MMIWEATHER_GetLowTempByDayAndIndex((MMIWEATHER_FC_DAY_E)week_day,cur_city_index);
        temp_high = MMIWEATHER_GetHighTempByDayAndIndex((MMIWEATHER_FC_DAY_E)week_day,cur_city_index);
        temp_unit = MMIWEATHER_GetTempUnitType();

        if(temp_low <= temp_high)
        {
            if(WEATHER_TEMP_UNIT_C == temp_unit)
            {
                temp_low  = MMIWEATHER_GetTemperature(TEMP_F2C(temp_low));
                temp_high = MMIWEATHER_GetTemperature(TEMP_F2C(temp_high));
                sprintf((char*)temp_range_str, "%d/%d", temp_low, temp_high);
                MMIAPICOM_StrToWstr(temp_range_str, weather_info->temp_range_wstr);
                MMI_GetLabelTextByLang(TXT_WEATHER_CEL_SIGNAL, &unit_str);
                MMIAPICOM_Wstrncat(weather_info->temp_range_wstr, unit_str.wstr_ptr,unit_str.wstr_len);
            }
            else if(WEATHER_TEMP_UNIT_F == temp_unit)
            {
                sprintf((char*)temp_range_str, "%d/%d", temp_low, temp_high);
                MMIAPICOM_StrToWstr(temp_range_str, weather_info->temp_range_wstr);
                MMI_GetLabelTextByLang(TXT_WEATHER_FAH_SIGNAL, &unit_str);
                MMIAPICOM_Wstrncat(weather_info->temp_range_wstr, unit_str.wstr_ptr,unit_str.wstr_len);
            }
        }

    }

    return weather_info->temp_range_wstr;
    
}

//得到WIDGET显示天气预报字串
LOCAL void WeatherTwo_GetDisplayText(MMI_STRING_T* Text_Ptr, //out 字串指针
                                                        MMIWIDGET_WEATHER_DISPALY_TEXT_TYPE_E dis_type,//in 
                                                        SELECT_WEEK_DAY week_day
                                                        )
{
    if(PNULL == Text_Ptr)
    {
        return;
    }
    //先清空指针
    Text_Ptr->wstr_ptr = 0;
    Text_Ptr->wstr_len = 0;
    switch(dis_type)
    {
    case MMIWIDGET_WEATHER_DISPALY_TEXT_TYPE_CITYNAME:
        {
            if(cur_weather_status == NEED_ADD_CITY)
            {
                MMI_GetLabelTextByLang(TXT_WEATHER_NOCITY, Text_Ptr);
            }  
            else
            {
                Text_Ptr->wstr_ptr = MMIWEATHER_GetCurCityName();
                Text_Ptr->wstr_len = MMIAPICOM_Wstrlen(Text_Ptr->wstr_ptr);
            }
        }
        break;
    case MMIWIDGET_WEATHER_DISPALY_TEXT_TYPE_DATE:
        {
            MMI_TEXT_ID_T       week_text[] = {TXT_WEATHER_WEEK_SUN, TXT_WEATHER_WEEK_MON,
                TXT_WEATHER_WEEK_TUE,TXT_WEATHER_WEEK_WED,
                TXT_WEATHER_WEEK_THU,TXT_WEATHER_WEEK_FRI,
                TXT_WEATHER_WEEK_SAT};
            SCI_DATE_T     sys_date      = {0};
            MMI_STRING_T   week_string   = {0};
            char           date_str[DIS_TEXT_MAX_LEN] = {0};
            wchar          s_cur_time_info_u16[DIS_TEXT_MAX_LEN] = {0};

            //get system date
            TM_GetSysDate(&sys_date);
            
            MMI_GetLabelTextByLang(week_text[sys_date.wday], &week_string);
            
            SCI_MEMSET(date_str, 0, sizeof(date_str));
            sprintf(date_str, "%02d/%02d ", sys_date.mon, sys_date.mday);

            SCI_MEMSET(s_cur_time_info_u16, 0, sizeof(s_cur_time_info_u16));
            (void)MMI_STRNTOWSTR(s_cur_time_info_u16, DIS_TEXT_MAX_LEN, (uint8*)date_str, DIS_TEXT_MAX_LEN, (size_t)SCI_STRLEN(date_str));

            (void)MMIAPICOM_Wstrncat(s_cur_time_info_u16,(wchar*)week_string.wstr_ptr,week_string.wstr_len);

            Text_Ptr->wstr_ptr = s_cur_time_info_u16;
            Text_Ptr->wstr_len = MMIAPICOM_Wstrlen(s_cur_time_info_u16);
        }
    	break;
    case MMIWIDGET_WEATHER_DISPALY_TEXT_TYPE_CONDITION:
        {
            MMI_TEXT_ID_T text_id = TXT_NULL;
            if(cur_weather_status == NEED_ADD_CITY)
            {
                    break;
            }            
                
            switch(week_day)
            {
            case TODAY:
                
                //TODAY
                text_id = MMIWEATHER_GetConditionTxtId(WeatherTwo_GetConditionbyDay(TODAY));
                MMI_GetLabelTextByLang(text_id, Text_Ptr);
                break;
                
            case TOMORROW:      
                //tomorrow
                text_id = MMIWEATHER_GetConditionTxtId(WeatherTwo_GetConditionbyDay(TOMORROW));
                MMI_GetLabelTextByLang(text_id, Text_Ptr);
                break;

            case  DAY_AFTER_TOMORROW:
                //day after tomorrow
                text_id = MMIWEATHER_GetConditionTxtId(WeatherTwo_GetConditionbyDay(DAY_AFTER_TOMORROW));
                MMI_GetLabelTextByLang(text_id, Text_Ptr);   
                break;
            default:
                break;    
            }
            
        }
        break;
    case MMIWIDGET_WEATHER_DISPALY_TEXT_TYPE_TEMPRANGE:
        {
            switch(week_day)
            {
            case TODAY:
                //TODAY
                Text_Ptr->wstr_ptr = WeatherTwo_GetTempRangeWstrbyDay(TODAY);
                Text_Ptr->wstr_len = MMIAPICOM_Wstrlen(Text_Ptr->wstr_ptr);
                break;
            case TOMORROW:
                //TOMORROW
                Text_Ptr->wstr_ptr = WeatherTwo_GetTempRangeWstrbyDay(TOMORROW);
                Text_Ptr->wstr_len = MMIAPICOM_Wstrlen(Text_Ptr->wstr_ptr);
                break;
            case DAY_AFTER_TOMORROW:
              //DAY AFTER TOMORROW
                Text_Ptr->wstr_ptr = WeatherTwo_GetTempRangeWstrbyDay(DAY_AFTER_TOMORROW);
                Text_Ptr->wstr_len = MMIAPICOM_Wstrlen(Text_Ptr->wstr_ptr);
                break;
            default:
                break;
            }
        }
        break;
    case MMIWIDGET_WEATHER_DISPALY_TEXT_TYPE_PROMPT:
        {
            MMI_TEXT_ID_T text_id = TXT_NULL;

            switch(s_Need_dis_prompt_str_type)
            {
            case MMIWIDGET_WEATHER_DIS_PROMPT_STR_TYPE_UPDATEING:
                text_id = TXT_COMMON_UPDATING;
            	break;
            case MMIWIDGET_WEATHER_DIS_PROMPT_STR_TYPE_NET_ERROR:
                text_id = TXT_WIDGET_WEATHER_NET_ERROR;
            	break;
            case MMIWIDGET_WEATHER_DIS_PROMPT_STR_TYPE_SWITCH_CITY:
                text_id = TXT_WIDGET_WEATHER_SWITCH_CITY;
                break;
            default:
                break;
            }

            if (TXT_NULL != text_id)
            {
                MMI_GetLabelTextByLang(text_id, Text_Ptr);
            }
        }
        break;
    case MMIWIDGET_WEATHER_DISPALY_TEXT_TYPE_WEEKDAY_STRING:
        if(cur_weather_status == NEED_ADD_CITY || cur_weather_status == NEED_UPDATE_WEATHER)
        {
            break;
        }

        switch(week_day)
        {
            case TODAY:
                
                break;
            case TOMORROW:
                MMI_GetLabelTextByLang((MMI_TEXT_ID_T)TXT_WIDGET_WEATHER_TWO_TOMORROW, Text_Ptr);
                break;
            case DAY_AFTER_TOMORROW:
                MMI_GetLabelTextByLang((MMI_TEXT_ID_T)TXT_WIDGET_WEATHER_TWO_AFTER_TOMORROW, Text_Ptr);
                break;
            default:
                break;
       }
       break; 
     default:
       break;
    }
}
//得到指定天的天气预报
LOCAL MMIWEATHER_CONDITION_E WeatherTwo_GetConditionbyDay(SELECT_WEEK_DAY week_day)
{
	MMIWEATHER_WEATHER_T*   weather_info = MMIWEATHER_GetCurCityWeatherInfo();
	
	switch(week_day)
	{
         case TODAY:
             return weather_info->current.condition;
             //break;
         case TOMORROW:
             return weather_info->forecast.weather2;
             //break;
         case DAY_AFTER_TOMORROW:
             return weather_info->forecast.weather3;
             //break;
         default:
             return weather_info->current.condition;
        //     break;
       }
}

//设置当前天气预报状态
LOCAL void WeatherTwo_SetCurWeatherStatus(void)
{
    MMIWEATHER_SETTING_T* setting_ptr = MMIWEATHER_GetSettingInfo();
    MMIWEATHER_WEATHER_T*   weather_info = MMIWEATHER_GetCurCityWeatherInfo();

    if(setting_ptr->city_num == 0)
        cur_weather_status = NEED_ADD_CITY;
    else if (weather_info->current.condition == WEATHER_CONDITION_UNKNOW)
        cur_weather_status = NEED_UPDATE_WEATHER;
    else
        cur_weather_status = ENTER_WEATHER_WIN;
}

LOCAL MMI_RESULT_E WeatherTwo_HandleWidgetTpDownMSG(MMI_WIN_ID_T  win_id,   DPARAM  param )
{
    MMI_RESULT_E recode   = MMI_RESULT_TRUE;
    GUI_POINT_T point     = {0};    
    uint16      cur_index = 0;
    GUI_LCD_DEV_INFO lcd_dev_info = {0};
    GUI_RECT_T focus_rect = {0};
    MMIWIDGET_WEATHER_FOCUS_POSITION_TYPE_E rect_index = MMIWIDGET_WEATHER_FOCUS_TODAY;
    MMK_GetWinLcdDevInfo(win_id, &lcd_dev_info);

    point.x = MMK_GET_TP_X(param);
    point.y = MMK_GET_TP_Y(param);
    
    if(WeatherTwo_GetFocusRectnIndex(point, &focus_rect, &rect_index))
    {
        //WeatherTwo_DisplayHighlightImg(win_id, &focus_rect);
        weathertwo_cur_focus_index = rect_index;
        // GUI_FillRect(&lcd_dev_info, focus_rect, MMI_GRAY_COLOR);
    }
    else
        recode = MMI_RESULT_FALSE;
     
    if (MMI_RESULT_TRUE == recode)
    {
        //这里强制让widget响应tp down事件，使得可以执行long move事件
        recode = MMI_RESULT_FALSE;
    }
    //SCI_TRACE_LOW:"HandleWidgetWeatherTwoTpDownMSG x= %d y= %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_WEATHER2_1456_112_2_18_3_8_55_259,(uint8*)"dd",point.x,point.y);
    return recode;
}

LOCAL BOOLEAN WeatherTwo_GetFocusRectnIndex(GUI_POINT_T point, //in
                                                                                        GUI_RECT_T   *rect, //out
                                                                                        MMIWIDGET_WEATHER_FOCUS_POSITION_TYPE_E *rect_index //out
                                                                                        )
{
    *rect_index = MMIWIDGET_WEATHER_FOCUS_TODAY;
    for( ; *rect_index < MMIWIDGET_WEATHER_FOCUS_MAX; (*rect_index)++)
    {
        if(GUI_PointIsInRect(point, WeatherTwo_CalcFoucsRect(*rect_index)))
            break;
    }
    if(*rect_index == MMIWIDGET_WEATHER_FOCUS_MAX)
        return FALSE;
    
    *rect = WeatherTwo_CalcFoucsRect(*rect_index);
    return TRUE;
}

//TP UP消息处理
LOCAL MMI_RESULT_E WeatherTwo_HandleWidgetTpUpMSG(MMI_WIN_ID_T  win_id,   DPARAM  param )
{
    MMI_RESULT_E recode   = MMI_RESULT_TRUE;   
    uint16      cur_index = 0;
    
   switch(WeatherTwo_GetRectTypebyIndex(weathertwo_cur_focus_index))
   {
        case TIME_RECT:
            MMIAPISET_SetTime();
            break;
        case MAIN_WEATHER_RECT:
            switch(cur_weather_status)
            {
                case NEED_ADD_CITY:
                    MMIWEATHER_OpenMainWin(); 
                    break;
                case NEED_UPDATE_WEATHER:
                    WeatherTwo_UpdateWeather();  
                    break;
                case ENTER_WEATHER_WIN:
                    MMIWEATHER_OpenMainWin();
                    break;
                default:
                    recode = MMI_RESULT_FALSE;
                    break;
            }
            break;
        case FORCAST_WEATHER_RECT:
            if(cur_weather_status == ENTER_WEATHER_WIN)
            {
                MMIWEATHER_OpenMainWin();
            }        
            break;
        default:
            recode = MMI_RESULT_FALSE;
            break;
   }

    //WeatherTwo_ResetWidgetWeather();
    
    return recode;
}
LOCAL WIDGET_WEATHER_RECT_TYPE WeatherTwo_GetRectTypebyIndex(MMIWIDGET_WEATHER_FOCUS_POSITION_TYPE_E rect_index)
{
    WIDGET_WEATHER_RECT_TYPE rect_type = WEATHER_RECT_TYPE_MAX;
    switch(rect_index)
    {
        case MMIWIDGET_WEATHER_FOCUS_TIME:
            rect_type = TIME_RECT; 
            break;
        case MMIWIDGET_WEATHER_FOCUS_TODAY:
        case MMIWIDGET_WEATHER_FOCUS_CITY:
        case MMIWIDGET_WEATHER_FOCUS_TODAY_IMG:
            rect_type = MAIN_WEATHER_RECT;
            break;
        case MMIWIDGET_WEATHER_FOCUS_TOMORROW_IMG:
        case MMIWIDGET_WEATHER_FOCUS_AFTER_TOMORROW_IMG:
            rect_type = FORCAST_WEATHER_RECT;
            break;
        default:
            break;        
    }    
    return rect_type;
}

LOCAL void WeatherTwo_UpdateWeather(void)
{
    BOOLEAN is_need_respond = TRUE;
    is_need_respond = MMIWEATHER_GetCityNum() > 0 ? TRUE : FALSE;

    if (is_need_respond)
    {
        MMI_STRING_T dis_text = {0};
        wchar string[DIS_TEXT_MAX_LEN] = {0};
        dis_text.wstr_ptr = string;
        s_Need_dis_prompt_str_type   = MMIWIDGET_WEATHER_DIS_PROMPT_STR_TYPE_UPDATEING;
        WeatherTwo_GetDisplayText(&dis_text, MMIWIDGET_WEATHER_DISPALY_TEXT_TYPE_PROMPT, WEEK_DAY_INVALID);
        MMIPUB_DisplayTipsPtrEx(((MMI_WIN_ID_T *)&g_weather_widget2.win_id), &dis_text, MMIPUB_TIPS_DIS_BOTTOM, PNULL);
        MMIWEATHER_SendMsgToApp(MSG_WEATHER_WGT_UPDATE_IND);
    }
    else
    {
        uint32 dis_time = MMI_1SECONDS;
        MMIPUB_DisplayTipsIDEx(((MMI_WIN_ID_T*)&g_weather_widget2.win_id), TXT_WIDGET_WEATHER_PLEASE_ADD_CITY, MMIPUB_TIPS_DIS_BOTTOM, &dis_time);
    }
}
#if 0
LOCAL BOOLEAN WeatherTwo_CreatHighlightLayer(MMI_WIN_ID_T     win_id)
{
    UILAYER_CREATE_T     create_info = {0};
    UILAYER_RESULT_E     result      = UILAYER_RESULT_SUCCESS;
    GUI_RECT_T  rect = {0};
    
    MMK_GetWinRect(win_id, &rect);

    if (UILAYER_IsEmptyLayerHandle(&weathertwo_highlight_layer_info))
    {        
        //creat text layer
        create_info.lcd_id          = GUI_MAIN_LCD_ID;
        create_info.owner_handle    = win_id;
        create_info.offset_x        = rect.left;
        create_info.offset_y        = rect.top;
        create_info.width           = rect.right - rect.left + 1;
        create_info.height          = rect.bottom - rect.top + 1;
        create_info.is_bg_layer     = FALSE;
        create_info.is_static_layer = FALSE;
        
        result = UILAYER_CreateLayer(&create_info, &weathertwo_highlight_layer_info);
        
        if (UILAYER_RESULT_ERROR == result)
        {
            return FALSE;
        }
    }
    else
    {
        UILAYER_Clear(&weathertwo_highlight_layer_info);
    }
    return TRUE;

}
LOCAL BOOLEAN WeatherTwo_DeleteHighlightLayer(void)
{
    if (!UILAYER_IsEmptyLayerHandle(&weathertwo_highlight_layer_info))
    {    
        UILAYER_RELEASELAYER(&weathertwo_highlight_layer_info);   /*lint !e506 !e774*/
//        UILAYER_ReleaseLayer(&s_highlight_layer_info);
//        s_highlight_layer_info.block_id = UILAYER_NULL_HANDLE;
    }

    return TRUE;
}
LOCAL void WeatherTwo_DisplayHighlightImg(MMI_WIN_ID_T  win_id, GUI_RECT_T  *border_rect_ptr)
{
    GUI_RECT_T  cur_rect    = {0};
    GUI_LCD_DEV_INFO  lcd_dev_info    = {0};
    GUI_BORDER_T        border; 
    UILAYER_APPEND_BLT_T append_layer = {0};
    
    if(PNULL == border_rect_ptr)
    {
        return;
    }
    
    MMK_GetWinLcdDevInfo(win_id,&lcd_dev_info);
    WeatherTwo_CreatHighlightLayer(win_id);

//set border
   border.type = GUI_BORDER_SOLID;
   border.color = MMI_GREEN_COLOR;
   border.width = 2;
   GUI_DisplayBorder(*border_rect_ptr, *border_rect_ptr, &border, &weathertwo_highlight_layer_info);
 //   if(GUIRES_DisplayFrameBySmallImg(border_rect_ptr, 10, 10, win_id, IMAGE_WIDGET_WEATHER_TWO_HIGHLIGHT, &weathertwo_highlight_layer_info))
    {
//add into layer queue
    append_layer.lcd_dev_info = weathertwo_highlight_layer_info;
    append_layer.layer_level = UILAYER_LEVEL_NORMAL;
            
    UILAYER_AppendBltLayer(&append_layer);
    }
}

LOCAL void WeatherTwo_ResetWidgetWeather(void)
{
    WeatherTwo_DeleteHighlightLayer();
}
#endif

LOCAL void WeatherTwo_DisplayWeatherWidget( MMI_WIN_ID_T        win_id)
{
    WeatherTwo_DisplayWeatherBg(win_id);
    WeatherTwo_DisplayCurrentTime();
    WeatherTwo_DisplayWeatherInfo(win_id);
}
LOCAL MMI_RESULT_E WeatherTwo_HandleWebKeyMSG(void)
{
         MMI_RESULT_E recode = MMI_RESULT_TRUE;
         
         switch(WeatherTwo_GetRectTypebyIndex(weathertwo_cur_focus_index))
        {
            case TIME_RECT:
                MMIAPISET_SetTime();
                break;
            case MAIN_WEATHER_RECT:
                switch(cur_weather_status)
                {
                    case NEED_ADD_CITY:
                        MMIAPIWEATHER_OpenAddCityWin();
                        break;
                    case NEED_UPDATE_WEATHER:
                        WeatherTwo_UpdateWeather();  
                        break;
                    case ENTER_WEATHER_WIN:
                        MMIWEATHER_OpenMainWin();
                        break;
                    default:
                        recode = MMI_RESULT_FALSE;
                        break;
                }
                break;
            case FORCAST_WEATHER_RECT:
                if(cur_weather_status == ENTER_WEATHER_WIN)
                {
                    MMIWEATHER_OpenMainWin();
                }        
                break;
            default:
                recode = MMI_RESULT_FALSE;
                break;
        }

    return recode;
}
LOCAL MMI_RESULT_E WeatherTwo_HandleKeyUpMSG(MMI_WIN_ID_T        win_id)
{
    GUI_RECT_T dis_rect = {0};
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    
    if(MMIWIDGET_WEATHER_FOCUS_AFTER_TOMORROW_IMG < weathertwo_cur_focus_index
                    || MMIWIDGET_WEATHER_FOCUS_TODAY == weathertwo_cur_focus_index)
    {
        weathertwo_cur_focus_index = MMIWIDGET_WEATHER_FOCUS_AFTER_TOMORROW_IMG;
    }
    else
    {
        weathertwo_cur_focus_index--;
    } 
    dis_rect = WeatherTwo_CalcFoucsRect(weathertwo_cur_focus_index);
    //WeatherTwo_DisplayHighlightImg(win_id, &dis_rect);
    return recode;
}

#if 0
LOCAL void WeatherTwo_DisplayFocusRect(MMI_WIN_ID_T win_id)
{
    GUI_RECT_T dis_rect = {0};

    if(weathertwo_cur_focus_index <= MMIWIDGET_WEATHER_FOCUS_AFTER_TOMORROW_IMG
        && weathertwo_cur_focus_index >= MMIWIDGET_WEATHER_FOCUS_TODAY)
    {
        dis_rect = WeatherTwo_CalcFoucsRect(weathertwo_cur_focus_index);
        //WeatherTwo_DisplayHighlightImg(win_id, &dis_rect);
    }
}
#endif
//zhm end

#endif
#endif

