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

#define _MMIWIDGET_WEATHER_C_

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "mmi_app_widget_trc.h"
#ifdef MMI_WIDGET_WEATHER1
#include "window_parse.h"
#include "guilabel.h"
#include "guiform.h"
#include "mmipub.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmidisplay_data.h"
#include "mmiwidget_image.h"
#include "guibutton.h"
#include "mmi_utility.h"
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
#include "mmipicview_text.h"
#include "mmimp3_text.h"
#include "mmidc_export.h"
#include "mmipicview_export.h"
#include "mmialarm_export.h"
//#include "mmiebook.h"
#include "mmiebook_export.h"
#include "mmicalendar_export.h"
#include "mmicountedtime_export.h"
#include "mmicalc_export.h"
#include "mmiwclk_export.h"
#include "mmimp3_export.h"
#include "mmiweather_funcs.h"
#include "mmiweather_win.h"
#include "mmiweather.h"
#include "mmiweather_text.h"
#include "mmiweather_export.h"
#include "mmiebook_text.h"
#include "mmiwidget_export.h"
#include "mmiwidget_internal.h"
#include "mmi_appmsg.h"
#ifdef BROWSER_SUPPORT
#include "mmibrowser_export.h"
#endif
#ifdef QQ_SUPPORT
#include "mmiqq_export.h"
#endif
#ifdef PDA_WEATHER_TIME_SE
#include "graphics_3d_rotate.h"
#endif

#ifdef QBTHEME_SUPPORT
#include "mmiqbtheme_idlewin.h"
#endif

#include "mmiwidget_special_effect.h"
#include "mmiwidget_weather_se.h"


#if defined MMIWIDGET_WEATHER_SE 
typedef struct
{
    WEATHER_RESOURCE_STRUCT_T resource_array;
#ifdef MMI_WEATHER_TEST//this macro define to test all weather anim
    uint32 s_msg_id;
    uint32 s_pre_msg_id;
#endif
    int8   cur_city_idx;
    BOOLEAN is_need_disappear_anim;
}MMI_WEATHER_SE_T;

LOCAL MMI_WEATHER_SE_T s_weather_t = {0};
LOCAL MMIWIDGET_SPECIAL_EFFECT_T s_weather_se = {0};

LOCAL int32 BarPhone_CreateImageBuffer(MMI_WIN_ID_T    win_id, MMIWIDGET_SPECIAL_EFFECT_T *p_widget_se);
LOCAL int32 BarPhone_DestroyImageBuffer(MMI_WIN_ID_T    win_id, MMIWIDGET_SPECIAL_EFFECT_T *p_widget_se);
LOCAL int32 BarPhone_Display(MMI_WIN_ID_T  win_id, MMIWIDGET_SPECIAL_EFFECT_T *p_widget_se);

#define MMI_WEEK_TOTAL_DAY     7
#endif

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**-------------------------------------------------------------------------*/
#ifdef PDA_WEATHER_TIME_SE
#define TIME_UPDATE_SE_FRAME_NUM        (16)
#define HOUR_PAGE_OFFSET_FRAME_NUM      (5)     
#define TIME_UPDATE_SE_TIMER       (1000/TIME_UPDATE_SE_FRAME_NUM)
#endif //PDA_UI_ANDROID_WEATHER_TIME_SE
#if defined MAINLCD_SIZE_320X480
#define   MMIWIDGET_WEATHER_TEXT_FONT                   SONG_FONT_16            //天气信息字体
#else
#define   MMIWIDGET_WEATHER_TEXT_FONT                   SONG_FONT_14            //天气信息字体
#endif
#define   MMIWIDGET_WEATHER_CITY_TEXT_FONT              SONG_FONT_20            //城市名称字体

#define   MMIWIDGET_WEATHER_TEXT_COLOR                  MMI_WHITE_COLOR         //天气信息字体颜色

#define   MMIWIDGET_WEATHER_ITEM_HEIGHT                 20                      //快捷方式设置上边距
#define   MMIWIDGET_WEATHER_ITEM_LEFT                   10                      //快捷方式设置左边距
#define   MMIWIDGET_WEATHER_ITEM_EX                     20                      //快捷方式设置间距

#define   MMIWIDGET_WEATHER_DEFAULT_X                   50                      //widget初始化x坐标
#define   MMIWIDGET_WEATHER_DEFAULT_Y                   50                      //widget初始化y坐标

#if defined MAINLCD_SIZE_320X480
#ifdef BAR_PHONE_WIDGET

#define   MMIWIDGET_BARWEATHER_TIME_FONT            SONG_FONT_64
#define   MMIWIDGET_BARWEATHER_TIME_WIDTH			47
#define   MMIWIDGET_BARWEATHER_TIME_HEIGHT			64
#define   MMIWIDGET_BARWEATHER_TIME_SPACE			35
#define   MMIWIDGET_BARWEATHER_TEXT_START_X		32
#define   MMIWIDGET_BARWEATHER_TEXT_START_Y		120
#define   MMIWIDGET_BARWEATHER_TEXT_SPACE			20
#define   MMIWIDGET_BARWEATHER_TEXT_HEIGHT			21
#define   MMIWIDGET_BARWEATHER_TEXT_TEMP_WIDTH       129
#define   MMIWIDGET_BARWEATHER_TEXT_DATE_WIDTH	129
#define   MMIWIDGET_BARWEATHER_TEXT_CONDITION_WIDTH  129
#define   MMIWIDGET_BARWEATHER_TEXT_CITY_WIDTH        129
#define   MMIWIDGET_BARWEATHER_TIME_IMG_START_Y            24                      //时间图片起始Y坐标
#define   MMIWIDGET_BARWEATHER_TIME_IMG_HOUR_START_X       44                       //小时图片起始X坐标
#endif

#define   MMIWIDGET_WEATHER_TIME_COLON_IMG_WIDTH        30                      //冒号图片宽度
#define   MMIWIDGET_WEATHER_TIME_COLON_IMG_HEIGHT       73                      //冒号图片高度
#define   MMIWIDGET_WEATHER_TIME_IMG_WIDTH              50                      //时间图片宽度
#define   MMIWIDGET_WEATHER_TIME_IMG_HEIGHT             90                      //时间图片宽度
#define   MMIWIDGET_WEATHER_TIME_IMG_START_Y            25                      //时间图片起始Y坐标
#define   MMIWIDGET_WEATHER_TIME_IMG_HOUR_START_X       37                      //小时图片起始X坐标
#define   MMIWIDGET_WEATHER_TIME_IMG_MINUTE_START_X     175                     //分钟图片起始X坐标
#define   MMIWIDGET_WEATHER_TIME_IMG_SPACE              5                       //时间图片间距

#define   MMIWIDGET_WEATHER_TEXT_WIDTH                  90                       //文本宽度
#define   MMIWIDGET_WEATHER_TEXT_SPACE                  5                       //文本间距
#define   MMIWIDGET_WEATHER_TEXT_START_Y                152                     //文本起始Y坐标
#define   MMIWIDGET_WEATHER_LEFT_TEXT_START_X           25                      //左边文本起始X坐标
#define   MMIWIDGET_WEATHER_RIGHT_TEXT_START_X          225                     //右边文本起始X坐标

#define   MMIWIDGET_WEATHER_IMG_START_X                 120                     //天气图片起始X坐标
#define   MMIWIDGET_WEATHER_IMG_START_Y                 131                     //天气图片起始Y坐标

#define   MMIWIDGET_WEATHER_HIT_TOP_LEFT_IMG_START_X    35                      //上半部分高亮条起始X坐标
#define   MMIWIDGET_WEATHER_HIT_TOP_RIGHT_IMG_START_X   175                      //上半部分高亮条起始X坐标
#define   MMIWIDGET_WEATHER_HIT_TOP_IMG_START_Y         20                      //上半部分高亮条起始Y坐标
#define   MMIWIDGET_WEATHER_HIT_BOTTOM_IMG_START_Y      130                     //下半部分高亮条起始Y坐标
#define   MMIWIDGET_WEATHER_HIT_TOP_IMG_WIDTH           115                     //上半部分高亮条宽度
#define   MMIWIDGET_WEATHER_HIT_TOP_IMG_HEIGHT          125                     //上半部分高亮条高度
#define   MMIWIDGET_WEATHER_HIT_BOTTOM_IMG_WIDTH        90                      //下半部分高亮条宽度
#define   MMIWIDGET_WEATHER_HIT_BOTTOM_IMG_HEIGHT       75      
//下半部分高亮条高度
#ifdef PDA_WEATHER_TIME_SE
#define TIME_BOX_WIDTH       108      //时间显示框的宽度。
#define TIME_BOX_HEIGHT      108      //时间显示框的高度。
#define HOUR_PAGE_OFFSET_X    MMIWIDGET_WEATHER_TIME_IMG_HOUR_START_X      //小时显示框到天气预报widget边框的水平距离
#define HOUR_PAGE_OFFSET_Y    16 //MMIWIDGET_WEATHER_TIME_IMG_START_Y           //小时显示框到天气预报widget边框的垂直距离
#define MINUTE_PAGE_OFFSET_X  MMIWIDGET_WEATHER_TIME_IMG_MINUTE_START_X    //分钟显示框到天气预报widget边框的水平距离
#define MINUTE_PAGE_OFFSET_Y  16 //MMIWIDGET_WEATHER_TIME_IMG_START_Y           //分钟显示框到天气预报widget边框的垂直距离
#endif

#else
#ifdef BAR_PHONE_WIDGET

#define   MMIWIDGET_BARWEATHER_TIME_FONT            SONG_FONT_60
#define   MMIWIDGET_BARWEATHER_TIME_WIDTH			35
#define   MMIWIDGET_BARWEATHER_TIME_HEIGHT			48
#define   MMIWIDGET_BARWEATHER_TIME_SPACE			33
#define   MMIWIDGET_BARWEATHER_TEXT_START_X		22
#define   MMIWIDGET_BARWEATHER_TEXT_START_Y		97
#define   MMIWIDGET_BARWEATHER_TEXT_SPACE			19
#define   MMIWIDGET_BARWEATHER_TEXT_HEIGHT			16
#define   MMIWIDGET_BARWEATHER_TEXT_TEMP_WIDTH       98
#define   MMIWIDGET_BARWEATHER_TEXT_DATE_WIDTH	     98
#define   MMIWIDGET_BARWEATHER_TEXT_CONDITION_WIDTH  98
#define   MMIWIDGET_BARWEATHER_TEXT_CITY_WIDTH        98 
#define   MMIWIDGET_BARWEATHER_TIME_IMG_START_Y            18                      //时间图片起始Y坐标
#define   MMIWIDGET_BARWEATHER_TIME_IMG_HOUR_START_X       33                       //小时图片起始X坐标
#endif
#define   MMIWIDGET_WEATHER_TIME_COLON_IMG_WIDTH        20                      //冒号图片宽度
#define   MMIWIDGET_WEATHER_TIME_COLON_IMG_HEIGHT       50                      //冒号图片高度
#define   MMIWIDGET_WEATHER_TIME_IMG_WIDTH              38                      //时间图片宽度
#define   MMIWIDGET_WEATHER_TIME_IMG_HEIGHT             68                      //时间图片宽度
#define   MMIWIDGET_WEATHER_TIME_IMG_START_Y            20                      //时间图片起始Y坐标
#define   MMIWIDGET_WEATHER_TIME_IMG_HOUR_START_X       27                      //小时图片起始X坐标
#define   MMIWIDGET_WEATHER_TIME_IMG_MINUTE_START_X     134                     //分钟图片起始X坐标
#define   MMIWIDGET_WEATHER_TIME_IMG_SPACE              5                       //时间图片间距

#if defined(MMI_PDA_SUPPORT)
#define   MMIWIDGET_WEATHER_TEXT_WIDTH                  80                       //文本宽度
#define   MMIWIDGET_WEATHER_LEFT_TEXT_START_X           8                      //左边文本起始X坐标
#define   MMIWIDGET_WEATHER_HIT_BOTTOM_IMG_START_Y      110                     //下半部分高亮条起始Y坐标
#else
#define   MMIWIDGET_WEATHER_TEXT_WIDTH                  66                       //文本宽度
#define   MMIWIDGET_WEATHER_LEFT_TEXT_START_X           18                      //左边文本起始X坐标
#define   MMIWIDGET_WEATHER_HIT_BOTTOM_IMG_START_Y      105                     //下半部分高亮条起始Y坐标
#endif
#define   MMIWIDGET_WEATHER_TEXT_SPACE                  5                       //文本间距
#define   MMIWIDGET_WEATHER_TEXT_START_Y                111                     //文本起始Y坐标
#define   MMIWIDGET_WEATHER_RIGHT_TEXT_START_X          154                     //右边文本起始X坐标

#define   MMIWIDGET_WEATHER_IMG_START_X                 90                     //天气图片起始X坐标
#define   MMIWIDGET_WEATHER_IMG_START_Y                 102                     //天气图片起始Y坐标

#define   MMIWIDGET_WEATHER_HIT_TOP_LEFT_IMG_START_X    21                      //上半部分高亮条起始X坐标
#define   MMIWIDGET_WEATHER_HIT_TOP_RIGHT_IMG_START_X   128                     //上半部分高亮条起始X坐标
#define   MMIWIDGET_WEATHER_HIT_TOP_IMG_START_Y         15                      //上半部分高亮条起始Y坐标
#define   MMIWIDGET_WEATHER_HIT_TOP_IMG_WIDTH           90                     //上半部分高亮条宽度
#define   MMIWIDGET_WEATHER_HIT_TOP_IMG_HEIGHT          100                     //上半部分高亮条高度
#define   MMIWIDGET_WEATHER_HIT_BOTTOM_IMG_WIDTH        66                      //下半部分高亮条宽度
#define   MMIWIDGET_WEATHER_HIT_BOTTOM_IMG_HEIGHT       56                      //下半部分高亮条高度

#ifdef PDA_WEATHER_TIME_SE
#define TIME_BOX_WIDTH       84      //时间显示框的宽度。
#define TIME_BOX_HEIGHT      84      //时间显示框的高度。
#define HOUR_PAGE_OFFSET_X    (MMIWIDGET_WEATHER_TIME_IMG_HOUR_START_X - 5)      //小时显示框到天气预报widget边框的水平距离
#define HOUR_PAGE_OFFSET_Y    10 //MMIWIDGET_WEATHER_TIME_IMG_START_Y           //小时显示框到天气预报widget边框的垂直距离
#define MINUTE_PAGE_OFFSET_X  (MMIWIDGET_WEATHER_TIME_IMG_MINUTE_START_X - 4)    //分钟显示框到天气预报widget边框的水平距离
#define MINUTE_PAGE_OFFSET_Y  HOUR_PAGE_OFFSET_Y //MMIWIDGET_WEATHER_TIME_IMG_START_Y           //分钟显示框到天气预报widget边框的垂直距离
#endif

#endif

#ifdef MMIWIDGET_WEATHER_ANIM
#define MMIWEATHER_ANIM_MOVE_LENGTH   8//动态天体特效每次移动像素数
#define MMIWEATHER_MAX_ANIM_MOVE_CNT  70//移动次数
#define MMIWEATHER_ANIM_MOVE_START_POS  220//移动图片起始位置
#endif

#ifdef MMIWIDGET_WEATHER_SE
#define WEATHER_MAX_STRING_LEN          64 // 字符串连接时，buffer的最大长度
#endif
/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/

#ifdef PDA_WEATHER_TIME_SE
typedef struct
{
    GUI_LCD_DEV_INFO    widget_layer_ptr;
    GUI_RECT_T          win_rect;
    SCI_TIME_T          pre_sys_time;
    BOOLEAN             is_hour_changed;
    BOOLEAN             is_minute_changed;
    uint8               se_frame_id;
} MMIWIDGET_TIME_SE_T;

typedef enum
{
    TIME_UPDATE_NONE = 0,
    TIME_UPDATE_MINUTE_ONLY,
    TIME_UPDATE_MINUTE_HOUR
} TIME_UPDATE_E;
#endif //PDA_WEATHER_TIME_SE


//提示信息类型
typedef enum
{
    MMIWIDGET_WEATHER_DIS_PROMPT_STR_TYPE_UPDATEING,
    MMIWIDGET_WEATHER_DIS_PROMPT_STR_TYPE_NET_ERROR,
    MMIWIDGET_WEATHER_DIS_PROMPT_STR_TYPE_SWITCH_CITY,
    MMIWIDGET_WEATHER_DIS_PROMPT_STR_TYPE_INVALID
}MMIWIDGET_WEATHER_DIS_PROMPT_STR_TYPE_E;

//widget窗口字符显示类型
typedef enum
{
    MMIWIDGET_WEATHER_DISPALY_TEXT_TYPE_CITYNAME,
    MMIWIDGET_WEATHER_DISPALY_TEXT_TYPE_DATE,
    MMIWIDGET_WEATHER_DISPALY_TEXT_TYPE_CONDITION,
    MMIWIDGET_WEATHER_DISPALY_TEXT_TYPE_TEMPRANGE,
    MMIWIDGET_WEATHER_DISPALY_TEXT_TYPE_PROMPT,
    MMIWIDGET_WEATHER_DISPALY_TEXT_TYPE_INVALID
}MMIWIDGET_WEATHER_DISPALY_TEXT_TYPE_E;

//widget设置类型
typedef enum
{
    MMIWIDGET_WEATHER_SET_WIN_TYPE_HIT_FUNC,
    MMIWIDGET_WEATHER_SET_WIN_TYPE_BG_TEXTCOLOUR,
    MMIWIDGET_WEATHER_SET_WIN_TYPE_INVALID
}MMIWIDGET_WEATHER_SET_WIN_TYPE_E;

//快捷方式NV项内容
typedef enum
{
    MMIWIDGET_WEATHER_HIT_SET_NV_ITEM_LEFT_TOP = 1,
    MMIWIDGET_WEATHER_HIT_SET_NV_ITEM_RIGHT_TOP,
    MMIWIDGET_WEATHER_HIT_SET_NV_ITEM_LEFT_BOTTOM,
    MMIWIDGET_WEATHER_HIT_SET_NV_ITEM_RIGHT_BOTTOM,
    MMIWIDGET_WEATHER_HIT_SET_NV_ITEM_INVALID
}MMIWIDGET_WEATHER_HIT_SET_NV_ITEM_E;

//背景&时间字体颜色NV项内容
typedef enum
{
    MMIWIDGET_WEATHER_BG_TEXTCOLOUR_SET_NV_ITEM_BG = 1,
    MMIWIDGET_WEATHER_BG_TEXTCOLOUR_SET_NV_ITEM_TEXTCOLOUR,
    MMIWIDGET_WEATHER_BG_TEXTCOLOUR_SET_NV_ITEM_INVALID
}MMIWIDGET_WEATHER_BG_TEXTCOLOUR_SET_NV_ITEM_E;

//时间字体颜色类型
typedef enum
{
    MMIWIDGET_WEATHER_TEXT_COLOUR_WHITE,
    MMIWIDGET_WEATHER_TEXT_COLOUR_BLACK,
    MMIWIDGET_WEATHER_TEXT_COLOUR_INVALID
}MMIWIDGET_WEATHER_TEXT_COLOUR_E;

//widget点击区域
typedef enum
{
    MMIWIDGET_WEATHER_CLICK_RECT_LEFT_TOP,
    MMIWIDGET_WEATHER_CLICK_RECT_RIGHT_TOP,
    MMIWIDGET_WEATHER_CLICK_RECT_LEFT_BOTTOM,
    MMIWIDGET_WEATHER_CLICK_RECT_MID_BOTTOM,
    MMIWIDGET_WEATHER_CLICK_RECT_RIGHT_BOTTOM,
    MMIWIDGET_WEATHER_CLICK_RECT_INVALID
}MMIWIDGET_WEATHER_CLICK_RECT_E;

//快捷方式设置控件组
typedef enum
{
    MMIWIDGET_WEATHER_CONTROL_TYPE_FORM,
    MMIWIDGET_WEATHER_CONTROL_TYPE_LABLE,
    MMIWIDGET_WEATHER_CONTROL_TYPE_DROPDOWN,
    MMIWIDGET_WEATHER_CONTROL_TYPE_INVALID
}MMIWIDGET_WEATHER_CONTROL_TYPE_E;

//控件定义
typedef enum
{
    MMIWIDGET_WEATHER_CONTROL_ID_START = MMIWIDGET_WIDGET_MAX,

    MMIWIDGET_WEATHER_SET_HIT_FUNC_FORM_CTRL_ID,

    MMIWIDGET_WEATHER_SET_LEFTTOP_FORM_CTRL_ID,
    MMIWIDGET_WEATHER_SET_LEFTTOP_LABEL_CTRL_ID,
    MMIWIDGET_WEATHER_SET_LEFTTOP_DROPDOWN_CTRL_ID,

    MMIWIDGET_WEATHER_SET_RIGHTTOP_FORM_CTRL_ID,
    MMIWIDGET_WEATHER_SET_RIGHTTOP_LABEL_CTRL_ID,
    MMIWIDGET_WEATHER_SET_RIGHTTOP_DROPDOWN_CTRL_ID,

    MMIWIDGET_WEATHER_SET_LEFTBOTTOM_FORM_CTRL_ID,
    MMIWIDGET_WEATHER_SET_LEFTBOTTOM_LABEL_CTRL_ID,
    MMIWIDGET_WEATHER_SET_LEFTBOTTOM_DROPDOWN_CTRL_ID,

    MMIWIDGET_WEATHER_SET_RIGHTBOTTOM_FORM_CTRL_ID,
    MMIWIDGET_WEATHER_SET_RIGHTBOTTOM_LABEL_CTRL_ID,
    MMIWIDGET_WEATHER_SET_RIGHTBOTTOM_DROPDOWN_CTRL_ID,

    MMIWIDGET_WEATHER_SET_BG_TEXTCOLOUR_FORM_CTRL_ID,

    MMIWIDGET_WEATHER_SET_BG_TYPE_FORM_CTRL_ID,
    MMIWIDGET_WEATHER_SET_BG_TYPE_LABEL_CTRL_ID,
    MMIWIDGET_WEATHER_SET_BG_TYPE_SETLIST_CTRL_ID,

    MMIWIDGET_WEATHER_SET_TEXT_COLOUR_FORM_CTRL_ID,
    MMIWIDGET_WEATHER_SET_TEXT_COLOUR_LABEL_CTRL_ID,
    MMIWIDGET_WEATHER_SET_TEXT_COLOUR_SETLIST_CTRL_ID,

    MMIWIDGET_WEATHER_CONTROL_ID_MAX
}MMIWIDGET_WEATHER_CONTROL_ID_E;

//快捷方式组
typedef enum
{
    MMIWIDGET_WEATHER_HANDLE_CHANGE_CITY,
    MMIWIDGET_WEATHER_HANDLE_UPDATE_WEATHER,    
    MMIWIDGET_WEATHER_HANDLE_SET_TIME,
    MMIWIDGET_WEATHER_HANDLE_SET_DATE,
    MMIWIDGET_WEATHER_HANDLE_ALARM,
    MMIWIDGET_WEATHER_HANDLE_CALENDAR,
    MMIWIDGET_WEATHER_HANDLE_COUNTEDTIME,
    MMIWIDGET_WEATHER_HANDLE_WCLK,
    MMIWIDGET_WEATHER_HANDLE_CALC,
    MMIWIDGET_WEATHER_HANDLE_BROWSER,
    MMIWIDGET_WEATHER_HANDLE_CAMERA,
    MMIWIDGET_WEATHER_HANDLE_EBOOK,
    MMIWIDGET_WEATHER_HANDLE_MP3,
    MMIWIDGET_WEATHER_HANDLE_QQ,
    MMIWIDGET_WEATHER_HANDLE_PIC_VIEWER,
    MMIWIDGET_WEATHER_HANDLE_MAX
}MMIWIDGET_WEATHER_HANDLE_E;

#ifdef MMIWIDGET_WEATHER_SE
typedef enum
{
    MMIWIDGET_WEATHER_SE_SUN,
    MMIWIDGET_WEATHER_SE_SUN_BG,
    MMIWIDGET_WEATHER_SE_BIG_TEMP,
    MMIWIDGET_WEATHER_SE_UP_TEMP,
    MMIWIDGET_WEATHER_SE_DOWN_TEMP,
    MMIWIDGET_WEATHER_SE_GLOW,
    MMIWIDGET_WEATHER_SE_CITY_CHANGE,
    MMIWIDGET_WEATHER_SE_WEATHER_CHANGE,
    MMIWIDGET_WEATHER_SE_FIRST_DAY,
    MMIWIDGET_WEATHER_SE_SECOND_DAY,
    MMIWIDGET_WEATHER_SE_THIRD_DAY,
    MMIWIDGET_WEATHER_SE_BG,
    MMIWIDGET_WEATHER_SE_DIVIDE_BAR,
    MMIWIDGET_WEATHER_SE_CLOUD1,
    MMIWIDGET_WEATHER_SE_CLOUD2,
    MMIWIDGET_WEATHER_SE_CLOUD_LEFT,
    MMIWIDGET_WEATHER_SE_CLOUD_RIGHT,    
    MMIWIDGET_WEATHER_SE_NONE,
    MMIWIDGET_WEATHER_SE_RAIN,
    MMIWIDGET_WEATHER_SE_FOG, 
    MMIWIDGET_WEATHER_SE_LIGHT1,   
    MMIWIDGET_WEATHER_SE_LIGHT2,   
    MMIWIDGET_WEATHER_SE_SNOW_BIG,   
    MMIWIDGET_WEATHER_SE_SNOW_SMALL,              
    MMIWIDGET_WEATHER_SE_TEMP_MAX
}MMIWIDGET_WEATHER_SE_E;//与WEATHER_RESOURCE_STRUCT_T结构对应
#endif

typedef void (*WEATHERCLICKFUNCTION)(void);

//widget点击项
typedef struct
{
    MMIWIDGET_WEATHER_HANDLE_E       handle;
    WEATHERCLICKFUNCTION             pfun;
    MMI_TEXT_ID_T                    text_id;
} MMIWIDGET_WEATHER_HOT_RECT_ITEM_T;

//widget图片组
LOCAL MMI_IMAGE_ID_T s_Number_Image_index[] [MMIWIDGET_WEATHER_TEXT_COLOUR_INVALID]= 
{
    {IMAGE_WIDGET_WEATHER_0, IMAGE_WIDGET_WEATHER_B0},
    {IMAGE_WIDGET_WEATHER_1, IMAGE_WIDGET_WEATHER_B1},
    {IMAGE_WIDGET_WEATHER_2, IMAGE_WIDGET_WEATHER_B2},
    {IMAGE_WIDGET_WEATHER_3, IMAGE_WIDGET_WEATHER_B3},
    {IMAGE_WIDGET_WEATHER_4, IMAGE_WIDGET_WEATHER_B4},
    {IMAGE_WIDGET_WEATHER_5, IMAGE_WIDGET_WEATHER_B5},
    {IMAGE_WIDGET_WEATHER_6, IMAGE_WIDGET_WEATHER_B6},
    {IMAGE_WIDGET_WEATHER_7, IMAGE_WIDGET_WEATHER_B7},
    {IMAGE_WIDGET_WEATHER_8, IMAGE_WIDGET_WEATHER_B8},
    {IMAGE_WIDGET_WEATHER_9, IMAGE_WIDGET_WEATHER_B9},
    {IMAGE_WIDGET_WEATHER_COLON_W, IMAGE_WIDGET_WEATHER_COLON_B},
};


//widget点击区域
LOCAL GUI_RECT_T s_click_rect[MMIWIDGET_WEATHER_CLICK_RECT_INVALID] = {0};

//快捷方式设置控件组
LOCAL MMI_CTRL_ID_T s_sel_hit_ctrl_id_table[] [MMIWIDGET_WEATHER_CONTROL_TYPE_INVALID]= {
    {MMIWIDGET_WEATHER_SET_LEFTTOP_FORM_CTRL_ID,     MMIWIDGET_WEATHER_SET_LEFTTOP_LABEL_CTRL_ID,     MMIWIDGET_WEATHER_SET_LEFTTOP_DROPDOWN_CTRL_ID}, 
    {MMIWIDGET_WEATHER_SET_RIGHTTOP_FORM_CTRL_ID,    MMIWIDGET_WEATHER_SET_RIGHTTOP_LABEL_CTRL_ID,    MMIWIDGET_WEATHER_SET_RIGHTTOP_DROPDOWN_CTRL_ID}, 
    {MMIWIDGET_WEATHER_SET_LEFTBOTTOM_FORM_CTRL_ID,  MMIWIDGET_WEATHER_SET_LEFTBOTTOM_LABEL_CTRL_ID,  MMIWIDGET_WEATHER_SET_LEFTBOTTOM_DROPDOWN_CTRL_ID}, 
    {MMIWIDGET_WEATHER_SET_RIGHTBOTTOM_FORM_CTRL_ID, MMIWIDGET_WEATHER_SET_RIGHTBOTTOM_LABEL_CTRL_ID, MMIWIDGET_WEATHER_SET_RIGHTBOTTOM_DROPDOWN_CTRL_ID}};

//背景&时间字体颜色控件组
LOCAL MMI_CTRL_ID_T s_sel_bg_textcolour_ctrl_id_table[] [MMIWIDGET_WEATHER_CONTROL_TYPE_INVALID]= {
    {MMIWIDGET_WEATHER_SET_BG_TYPE_FORM_CTRL_ID,     MMIWIDGET_WEATHER_SET_BG_TYPE_LABEL_CTRL_ID,     MMIWIDGET_WEATHER_SET_BG_TYPE_SETLIST_CTRL_ID}, 
    {MMIWIDGET_WEATHER_SET_TEXT_COLOUR_FORM_CTRL_ID, MMIWIDGET_WEATHER_SET_TEXT_COLOUR_LABEL_CTRL_ID, MMIWIDGET_WEATHER_SET_TEXT_COLOUR_SETLIST_CTRL_ID}};

LOCAL  uint8  s_cur_focus_index = 0xff;
/*****************************************************************************/
//    Description : 打开浏览器
//    Global resource dependence : 
//    Author:   xiaoming.ren
//    Note:
/*****************************************************************************/
LOCAL void OpenBrowserMainWin(void);

/*****************************************************************************/
//    Description : 切换城市
//    Global resource dependence : 
//    Author:   xiaoming.ren
//    Note:
/*****************************************************************************/
LOCAL void ChangeCurCity(void);

/*****************************************************************************/
//    Description : 更新天气
//    Global resource dependence : 
//    Author:   xiaoming.ren
//    Note:
/*****************************************************************************/
LOCAL void UpdateWeather(void);

#ifdef MMIWIDGET_WEATHER_ANIM

/*****************************************************************************/
//    Description : 是否屏蔽天气特效
//    Global resource dependence : 
//    Author:   jin.wang
//    Note:非屏蔽状态下设置需要才能播放特效
/*****************************************************************************/
LOCAL void SetIsForbidAnim(BOOLEAN is_forbid);

/*****************************************************************************/
//    Description : 是否需要动态天气特效
//    Global resource dependence : 
//    Author:   jin.wang
//    Note:
/*****************************************************************************/
LOCAL void SetWeatherAnimState(BOOLEAN is_need);

/*****************************************************************************/
//    Description : widget界面天气动态特效初始化
//    Global resource dependence : 
//    Author:   jin.wang
//    Note:
/*****************************************************************************/
LOCAL void InitWeatherAnim(MMI_WIN_ID_T win_id);

/*****************************************************************************/
//    Description : 设置图层透明度
//    Global resource dependence : 
//    Author:   jin.wang
//    Note:
/*****************************************************************************/
LOCAL void SetWeatherAnimLayerAlpha(
    MMIWEATHER_CONDITION_E  condition,
    MMIWEATHER_ANIM_T* anim_info_ptr,
    uint16 layer_idx
);

/*****************************************************************************/
//    Description : 创建显示动态特效的图层
//    Global resource dependence : 
//    Author:   jin.wang
//    Note:
/*****************************************************************************/
LOCAL UILAYER_RESULT_E CreateAnimLayer(
    UILAYER_CREATE_T* create_info_ptr,
    MMI_WIN_ID_T win_id,
    MMIWEATHER_ANIM_T* anim_info_ptr
);

/*****************************************************************************/
//    Description : 创建显示动态特效的timer
//    Global resource dependence : 
//    Author:   jin.wang
//    Note:
/*****************************************************************************/
LOCAL void CreateAnimTimer(
    MMI_WIN_ID_T win_id,
    MMIWEATHER_ANIM_T* anim_info_ptr,
    MMIWEATHER_CONDITION_E  condition
);

/*****************************************************************************/
//    Description : 销毁显示动态特效的图层
//    Global resource dependence : 
//    Author:   jin.wang
//    Note:
/*****************************************************************************/
LOCAL void ReleaseAnimLayer(MMIWEATHER_ANIM_T* anim_info_ptr);

/*****************************************************************************/
//    Description : 处理天气特效timer的callback
//    Global resource dependence : 
//    Author:   jin.wang
//    Note:
/*****************************************************************************/
LOCAL void DisplayAnimCallback(uint8 timer_id, DPARAM param);

/*****************************************************************************/
//    Description : 获取天气特效显示位置
//    Global resource dependence : 
//    Author:   jin.wang
//    Note:
/*****************************************************************************/
LOCAL void DrawWeatherAnim(
    MMI_WIN_ID_T win_id,
    MMIWEATHE_ANIM_LAYER_T* layer_info_ptr
);

/*****************************************************************************/
//    Description : 关闭动态特效timer
//    Global resource dependence : 
//    Author:   jin.wang
//    Note:
/*****************************************************************************/
LOCAL void StopDyWeatherTimer(MMIWEATHER_ANIM_T* anim_info_ptr);

/*****************************************************************************/
//    Description : 销毁天气动画
//    Global resource dependence : 
//    Author:   jin.wang
//    Note:
/*****************************************************************************/
LOCAL void DestoryWeatherAnim(void);

/*****************************************************************************/
//  Description : 根据天气状况获取动画资源
//  Global resource dependence : none
//  Author: jin.wang
//  Note:
/*****************************************************************************/
LOCAL void GetAnimInfoByCondition
(
    MMIWEATHER_CONDITION_E condition_id,
    MMIWEATHER_ANIM_T* anim_info_ptr
);

/*****************************************************************************/
//  Description : 根据天气状况获取铃声
//  Global resource dependence : none
//  Author: jin.wang
//  Note:
/*****************************************************************************/
MMI_RING_ID_T GetRingIdByCondition(MMIWEATHER_CONDITION_E condition_id);
#endif
/*****************************************************************************/
//    Description : 创建高亮Layer
//    Global resource dependence : 
//    Author:   xiaoming.ren
//    Note:
/*****************************************************************************/
LOCAL BOOLEAN CreatHighlightLayer(MMI_WIN_ID_T     win_id);

/*****************************************************************************/
//    Description : 删除高亮Layer
//    Global resource dependence : 
//    Author:   xiaoming.ren
//    Note:
/*****************************************************************************/
LOCAL BOOLEAN DeleteHighlightLayer(void);


#ifdef PDA_WEATHER_TIME_SE

/*****************************************************************************/
//    Description : 创建天气预报widget时间翻页特效Layer
//    Global resource dependence : 
//    Author:  Gaily.Wang
//    Note:
/*****************************************************************************/
LOCAL BOOLEAN CreateTimeBoxLayer(
    GUI_LCD_DEV_INFO *layer_ptr, 
    int16 offset_x, 
    int16 offset_y
);

/*****************************************************************************/
//    Description :画特效的背景
//    Global resource dependence : 
//    Author:  Arvin.wu
//    Note:
/*****************************************************************************/
LOCAL void DrawTimeBg(
    GUI_LCD_DEV_INFO *layer_handle, 
    int16 offset_x, 
    int16 offset_y
);

/*****************************************************************************/
//  Discription: 释放时间特效图层
//  Global resource dependence: None
//  Author: Jin.Wang
//  Note : 
/*****************************************************************************/
LOCAL void RealeaseTimeSeLayer(void);

/*****************************************************************************/
//  Description : handle time update se timer
//  Global resource dependence : none
//  Author: Gaily.Wang
//  Note:
/*****************************************************************************/
LOCAL void CalcuteOneFrame(
    uint8  frame_id,                          //[in]第几帧      
    uint16 width,           
    uint16 height,
    GUI_LCD_DEV_INFO*  old_src_layer_ptr,     //[in]更新前的时间
    GUI_LCD_DEV_INFO*  new_src_layer_ptr,     //[in]更新后的时间
    GUI_LCD_DEV_INFO*  dst_layer_ptr          //[out]输出计算结果的Layer
);

/*****************************************************************************/
//  Description : Handle weather time update timer
//  Global resource dependence : none
//  Author: Gaily.Wang
//  Note:
/*****************************************************************************/
LOCAL void HandleTimeUpdateSeTimer(
    uint8  timer_id,
    uint32 param
);

/*****************************************************************************/
//  Discription: Start time update se timer
//  Global resource dependence: None
//  Author: Gaily.Wang
//  Note : 
/*****************************************************************************/
LOCAL void StartTimeUpdateSeTimer(void);

/*****************************************************************************/
//    Description :播放时间更新特效
//    Global resource dependence : 
//    Author:  Gaily.Wang
//    Note:
/*****************************************************************************/
LOCAL BOOLEAN PlayTimeUpdateSe(SCI_TIME_T pre_sys_time);

/*****************************************************************************/
//    Description :在天气预报widget上显示当前时间。
//    Global resource dependence : 
//    Author:  Gaily.Wang
//    Note:
/*****************************************************************************/
LOCAL void DisplayCurrentTime(void);

/*****************************************************************************/
//    Description :播放时间更新特效
//    Global resource dependence : 
//    Author:  Gaily.Wang
//    Note:
/*****************************************************************************/
LOCAL void StopTimeUpdateSe(void);

/*****************************************************************************/
//  Discription: Start time update se timer
//  Global resource dependence: None
//  Author: Gaily.Wang
//  Note : 
/*****************************************************************************/
LOCAL void StopTimeUpdateSeTimer(void);

/*****************************************************************************/
//    Description :判断某widget是否可见
//    Global resource dependence : 
//    Author:  Gaily.Wang
//    Note:
/*****************************************************************************/
LOCAL BOOLEAN IsWidgetWinVisible(MMI_WIN_ID_T win_id);

#endif //PDA_WEATHER_TIME_SE

//快捷方式组
LOCAL MMIWIDGET_WEATHER_HOT_RECT_ITEM_T s_func_list_table[] = {
    {MMIWIDGET_WEATHER_HANDLE_CHANGE_CITY, ChangeCurCity, TXT_WIDGET_WEATHER_CHANGE_CITY},
    {MMIWIDGET_WEATHER_HANDLE_UPDATE_WEATHER, UpdateWeather, TXT_WIDGET_WEATHER_UPDATE_WEATHER},
    {MMIWIDGET_WEATHER_HANDLE_SET_TIME, MMIAPISET_SetTime, TXT_SET_TIME},
    {MMIWIDGET_WEATHER_HANDLE_SET_DATE, MMIAPISET_SetDate, TXT_SET_DATE},
    {MMIWIDGET_WEATHER_HANDLE_ALARM, MMIAPIALM_OpenMainWin, TXT_ALARM_CLOCK},
#ifdef CALENDAR_SUPPORT
    {MMIWIDGET_WEATHER_HANDLE_CALENDAR, MMIAPICALEND_OpenMainWin, TXT_TOOLS_CALENDAR},
#endif
    {MMIWIDGET_WEATHER_HANDLE_COUNTEDTIME, MMIAPICT_OpenCoutimeWin, TXT_TOOLS_COUNTEDTIME},
    {MMIWIDGET_WEATHER_HANDLE_WCLK, MMIAPIWCLK_OpenMainWin, TXT_TOOLS_WORLDTIME},
    {MMIWIDGET_WEATHER_HANDLE_CALC, MMIAPICALC_OpenMainWin, TXT_TOOLS_CALCULATOR},
    {MMIWIDGET_WEATHER_HANDLE_BROWSER, OpenBrowserMainWin, TXT_COMMUNICATION_BROWSER},
#ifdef QQ_SUPPORT
    {MMIWIDGET_WEATHER_HANDLE_QQ, MMIAPIQQ_OpenMainWin, TXT_SPREADTRUM_QQ_SUPPORT},    /*lint !e64*/
#endif
    {MMIWIDGET_WEATHER_HANDLE_CAMERA, MMIAPIDC_OpenPhotoWin, TXT_ENTERTAIMENT_CAMERA}, /*lint !e64*/
#ifdef EBOOK_SUPPORT
    {MMIWIDGET_WEATHER_HANDLE_EBOOK, MMIAPIEBOOK_ManagerWinMsg, TXT_COMM_EBOOK},/*lint !e64*/
#endif
    {MMIWIDGET_WEATHER_HANDLE_PIC_VIEWER, MMIAPIPICVIEW_OpenPicViewer, TXT_COMMON_PIC},
#ifdef MMI_AUDIO_PLAYER_SUPPORT
    {MMIWIDGET_WEATHER_HANDLE_MP3, MMIAPIMP3_OpenMp3Player, TXT_AUDIO}
#endif
};

//快捷方式组长度
#define FUNC_LIST_TABLE_LEN  (sizeof(s_func_list_table)/sizeof(MMIWIDGET_WEATHER_HOT_RECT_ITEM_T))

//点击区域组长度
#define HIT_CTRL_ID_TABLE_LEN  (sizeof(s_sel_hit_ctrl_id_table)/(sizeof(MMI_CTRL_ID_T) * MMIWIDGET_WEATHER_CONTROL_TYPE_INVALID))

//背景&时间字体颜色组长度
#define BG_TEXTCOLOUR_CTRL_ID_TABLE_LEN  (sizeof(s_sel_bg_textcolour_ctrl_id_table)/(sizeof(MMI_CTRL_ID_T) * MMIWIDGET_WEATHER_CONTROL_TYPE_INVALID))

//点击控件区域
LOCAL GUI_RECT_T     s_sel_hit_rect[HIT_CTRL_ID_TABLE_LEN] [MMIWIDGET_WEATHER_CONTROL_TYPE_DROPDOWN]= {0};

//背景&时间信息控件区域
LOCAL GUI_RECT_T     s_sel_bg_textcolour_rect[BG_TEXTCOLOUR_CTRL_ID_TABLE_LEN] [MMIWIDGET_WEATHER_CONTROL_TYPE_DROPDOWN]= {0};

//当前快捷方式设置值
LOCAL uint32  s_cur_hit_set_info = 0;

//是否需要重新读取快捷方式设置值
LOCAL BOOLEAN s_is_need_read_hit_set_info = TRUE;

//默认快捷方式设置值
#define DEFAULT_HIT_SET_INFO     0x02040001 //(2,4,0,1)依次8位存一个数据位，共四位，位置分别对应MMIWIDGET_WEATHER_BG_TEXTCOLOUR_SET_NV_ITEM_E中的值

//当前背景&时间字体颜色设置值
LOCAL uint32  s_cur_bg_textcolour_set_info = 0;

//是否需要重新读取背景&时间字体颜色设置值
LOCAL BOOLEAN s_is_need_read_bg_textcolour_set_info = TRUE;

//默认背景&时间字体颜色设置值
#define DEFAULT_BG_TEXTCOLOUR_SET_INFO     0x01010000 //(0,0)依次8位存一个数据位，共四位，位置分别对应MMIWIDGET_WEATHER_BG_TEXTCOLOUR_SET_NV_ITEM_E中的值,后16位暂时未空

//当前点击区域类型
LOCAL MMIWIDGET_WEATHER_CLICK_RECT_E s_tp_hit_type = MMIWIDGET_WEATHER_CLICK_RECT_INVALID;

//当前提示字符串类型
LOCAL MMIWIDGET_WEATHER_DIS_PROMPT_STR_TYPE_E s_Need_dis_prompt_str_type = MMIWIDGET_WEATHER_DIS_PROMPT_STR_TYPE_INVALID;

//需要显示的字符串最大长度
#define DIS_TEXT_MAX_LEN   12

LOCAL wchar          s_cur_temprange_u16[DIS_TEXT_MAX_LEN] = {0};
LOCAL wchar          s_cur_city_u16[DIS_TEXT_MAX_LEN]      = {0};
LOCAL wchar          s_cur_condition_u16[DIS_TEXT_MAX_LEN] = {0};
LOCAL wchar          s_cur_time_info_u16[DIS_TEXT_MAX_LEN] = {0};
LOCAL MMI_STRING_T   s_cur_temprange_str                   = {0};
LOCAL MMI_STRING_T   s_cur_city_str                        = {0};
LOCAL MMI_STRING_T   s_cur_condition_str                   = {0};
#ifdef BAR_PHONE_WIDGET
typedef enum 
{
    NEED_ADD_CITY,
    NEED_UPDATE_WEATHER,
    ENTER_WEATHER_WIN,
    WEAHTER_STATUS_MAX
}CUR_WEAHTER_STATUS;

LOCAL CUR_WEAHTER_STATUS cur_webkey_condition = WEAHTER_STATUS_MAX;
LOCAL CUR_WEAHTER_STATUS  pre_webkey_condition = WEAHTER_STATUS_MAX;
#endif
//高亮Layer
LOCAL GUI_LCD_DEV_INFO  s_highlight_layer_info = {0,UILAYER_NULL_HANDLE};


#ifdef PDA_WEATHER_TIME_SE
uint8       s_se_timer_id = 0;
uint8       s_se_frame_id = 0;
BOOLEAN     s_is_hour_changed = FALSE;
//显示时间特效所需层数
#define WEATHER_TIME_LAYER_MAX   6
LOCAL SCI_TIME_T  s_sys_time = {0}; //记录最后一次更新的时间
LOCAL GUI_LCD_DEV_INFO  s_min_old_layer    = {0,UILAYER_NULL_HANDLE};
LOCAL GUI_LCD_DEV_INFO  s_min_new_layer    = {0,UILAYER_NULL_HANDLE};
LOCAL GUI_LCD_DEV_INFO  s_min_calc_layer   = {0,UILAYER_NULL_HANDLE};
LOCAL GUI_LCD_DEV_INFO  s_hour_old_layer    = {0,UILAYER_NULL_HANDLE};
LOCAL GUI_LCD_DEV_INFO  s_hour_new_layer    = {0,UILAYER_NULL_HANDLE};
LOCAL GUI_LCD_DEV_INFO  s_hour_calc_layer   = {0,UILAYER_NULL_HANDLE};
const int16 s_time_se_angle_able[]={200,240,278,322,374,433,501,579,668,769,882,1008,1147,1297,1458,1625};
#endif  //PDA_WEATHER_TIME_SE


/*---------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                 */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         EXTERNAL DEFINITION                              *
**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
//    Description : widget界面事件处理
//    Global resource dependence : 
//    Author:   xiaoming.ren
//    Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleWidgetWeatherWinMsg(
    MMI_WIN_ID_T        win_id,        //IN:
    MMI_MESSAGE_ID_E    msg_id,        //IN:
    DPARAM            param        //IN:
);

/*****************************************************************************/
//    Description : widget界面显示
//    Global resource dependence : 
//    Author:   xiaoming.ren
//    Note:
/*****************************************************************************/
LOCAL void MMIWIDGET_Weather_Redraw(MMI_WIN_ID_T     win_id);

/*****************************************************************************/
//    Description : 显示时间和天气信息
//    Global resource dependence : 
//    Author:   xiaoming.ren
//    Note:
/*****************************************************************************/
LOCAL void MMIWIDGET_Weather_Display_Time_WeatherInfo(MMI_WIN_ID_T    win_id, GUI_POINT_T  bg_point);
#if defined TOUCH_PANEL_SUPPORT
/*****************************************************************************/
//    Description : MSG_WIDGET_TP_PRESS_UP消息处理
//    Global resource dependence : 
//    Author:   xiaoming.ren
//    Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  MMIWIDGET_Weather_HandleTpPressUp(MMI_WIN_ID_T  win_id,   DPARAM  param );

/*****************************************************************************/
//    Description : MSG_WIDGET_TP_PRESS_DOWN消息处理
//    Global resource dependence : 
//    Author:   xiaoming.ren
//    Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  MMIWIDGET_Weather_HandleTpPressDown(MMI_WIN_ID_T  win_id,   DPARAM  param );
#endif
/*****************************************************************************/
//    Description :  显示高亮图片
//    Global resource dependence : 
//    Author:   xiaoming.ren
//    Note:
/*****************************************************************************/
LOCAL void DisplayHighlightImg(MMI_WIN_ID_T  win_id, MMI_IMAGE_ID_T img_id, GUI_RECT_T  *border_rect_ptr);

/*****************************************************************************/
//    Description : 获取显示类型对应的字符串
//    Global resource dependence : 
//    Author:   xiaoming.ren
//    Note:
/*****************************************************************************/
LOCAL void GetDisplayText(MMI_STRING_T* Text_Ptr, MMIWIDGET_WEATHER_DISPALY_TEXT_TYPE_E dis_type);

/*****************************************************************************/
//    Description : 提取位于index位的NV数据到set_info_ptr中
//    Global resource dependence : 
//    Author:   xiaoming.ren
//    Note:
/*****************************************************************************/
LOCAL uint16 ParserNvValue(uint8 index, uint32 *set_info_ptr);

/*****************************************************************************/
//    Description : 将value设置到set_info_ptr的index位
//    Global resource dependence : 
//    Author:   xiaoming.ren
//    Note:
/*****************************************************************************/
LOCAL void MakeNvValue(uint8 index, uint16 value, uint32 *set_info_ptr);

/*****************************************************************************/
//    Description : 读取快捷方式设置信息
//    Global resource dependence : 
//    Author:   xiaoming.ren
//    Note:
/*****************************************************************************/
LOCAL void ReadHitSetInfo(uint32* set_info);

/*****************************************************************************/
//    Description : 保存快捷方式设置信息
//    Global resource dependence : 
//    Author:   xiaoming.ren
//    Note:
/*****************************************************************************/
LOCAL void WriteHitSetInfo(void);

/*****************************************************************************/
//    Description : 读取背景&时间字体颜色设置信息
//    Global resource dependence : 
//    Author:   xiaoming.ren
//    Note:
/*****************************************************************************/
LOCAL void ReadBgAndTextColorSetInfo(uint32* set_info);

/*****************************************************************************/
//    Description : 保存背景&时间字体颜色设置信息
//    Global resource dependence : 
//    Author:   xiaoming.ren
//    Note:
/*****************************************************************************/
LOCAL void WriteBgAndTextColorSetInfo(void);

/*****************************************************************************/
//    Description : 快捷方式设置界面事件处理
//    Global resource dependence : 
//    Author:   xiaoming.ren
//    Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleWeatherWidgetHitFuncSetWinMsg(MMI_WIN_ID_T win_id,MMI_MESSAGE_ID_E msg_id,DPARAM   param);

/*****************************************************************************/
//    Description : 背景&时间字体颜色设置界面事件处理
//    Global resource dependence : 
//    Author:   xiaoming.ren
//    Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleWeatherWidgetBGTextColourSetWinMsg(MMI_WIN_ID_T win_id,MMI_MESSAGE_ID_E msg_id,DPARAM   param);


/*****************************************************************************/
//    Description : 初始化设置界面控件区域
//    Global resource dependence : 
//    Author:   xiaoming.ren
//    Note:
/*****************************************************************************/
LOCAL void WeatherSetWin_InitCtrlRect(MMIWIDGET_WEATHER_SET_WIN_TYPE_E win_type);

/*****************************************************************************/
//    Description : 初始化设置界面控件参数
//    Global resource dependence : 
//    Author:   xiaoming.ren
//    Note:
/*****************************************************************************/
LOCAL void WeatherSetWin_SetCtrlParam(MMIWIDGET_WEATHER_SET_WIN_TYPE_E win_type);

/*****************************************************************************/
//    Description : 初始化Widget设置信息
//    Global resource dependence : 
//    Author:   xiaoming.ren
//    Note:
/*****************************************************************************/
LOCAL void InitWidgetWeatherSetInfo(void);

/*****************************************************************************/
//    Description : 获取当前天气信息
//    Global resource dependence : 
//    Author:   xiaoming.ren
//    Note:
/*****************************************************************************/
LOCAL void GetCurWeatherInfo(void);

/*****************************************************************************/
//    Description : 获取点击区域
//    Global resource dependence : 
//    Author:   xiaoming.ren
//    Note:
/*****************************************************************************/
LOCAL void CalcHitRect(MMI_WIN_ID_T        win_id);

/*****************************************************************************/
//    Description : 数据复位
//    Global resource dependence : 
//    Author:   xiaoming.ren
//    Note:
/*****************************************************************************/
LOCAL void ResetWidgetWeather(void);
/*****************************************************************************/
//    Description :设为focus
//    Global resource dependence : 
//    Author:  juan.wu
//    Note:
/*****************************************************************************/
LOCAL BOOLEAN SetShortCutFocus(MMI_WIN_ID_T win_id,uint8 focus_index);

/*****************************************************************************/
//    Description :响应web key
//    Global resource dependence : 
//    Author:  juan.wu
//    Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E MMIWIDGET_Weather_HandleWebKey(uint32 index);

/*****************************************************************************/
//    Description : DrawAmPmString
//    Global resource dependence : 
//    Author:  juan.wu
//    Note:
/*****************************************************************************/
LOCAL void DrawAmPmString(
                        BOOLEAN is_am,
                        GUI_POINT_T start_point,
                        GUI_LCD_DEV_INFO  *lcd_info_ptr
                        );
                        
/*****************************************************************************/
//    Description : Get12HoursTypeTime
//    Global resource dependence : 
//    Author:  juan.wu
//    Note:
/*****************************************************************************/
LOCAL BOOLEAN Get12HoursTypeTime(SCI_TIME_T * s_time_ptr);

#ifdef BAR_PHONE_WIDGET
LOCAL void BarPhoneWeather_SetWebKeyCondition(void);
LOCAL MMI_RESULT_E BarPhoneWeather_HandleWebKey(void);

#if !defined MMIWIDGET_WEATHER_SE
LOCAL int16 BarPhoneWeather_DisplayTimeOneNum(GUI_LCD_DEV_INFO    * lcd_dev_info,
									GUI_POINT_T      start_point,
									uint8 time_num);
LOCAL void BarPhoneWeather_Redraw(MMI_WIN_ID_T     win_id);
LOCAL void BarPhoneWeather_Display_Time_WeatherInfo(MMI_WIN_ID_T        win_id, GUI_POINT_T  bg_point);
#endif
LOCAL void BarPhoneWeather_DisplaySoftkey(MMI_WIN_ID_T    win_id, BOOLEAN is_update);
#endif
/**--------------------------------------------------------------------------*/

//天气预报widget
PUBLIC MMIWIDGET_ITEM_INFO_T g_weather_widget =
{
        MMIWIDGET_WEATHER_ID, 
		HandleWidgetWeatherWinMsg, 	
		PNULL,
		ResetWidgetWeather,
		PNULL,		
		WIDGET_WEATHER_WIN_ID,
		IMAGE_WIDGET_WEATHER_SHORTCUT,
		IMAGE_WIDGET_WEATHER_BG,
		TXT_WIDGET_WEATHER,
		MMIWIDGET_WEATHER_DEFAULT_X,
		MMIWIDGET_WEATHER_DEFAULT_Y,
		FALSE,
};

//快捷方式设置窗口
WINDOW_TABLE(WIDGET_WEATHER_SET_HIT_FUNC_WIN_TAB) =
{
    WIN_FUNC((uint32)HandleWeatherWidgetHitFuncSetWinMsg), 
        WIN_ID(WIDGET_WEATHER_SET_HIT_FUNC_WIN_ID),
        
#ifdef MMI_PDA_SUPPORT
        WIN_TITLE(TXT_SET_IDLE_SHORTCUT),
#else
        WIN_TITLE(TXT_WIDGET_WEATHER_SHORTCUT_SETTING),
#endif
        WIN_BACKGROUND_ID(IMAGE_COMMON_BG),
        
        CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER, MMIWIDGET_WEATHER_SET_HIT_FUNC_FORM_CTRL_ID),

        //LEFTTOP
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIWIDGET_WEATHER_SET_LEFTTOP_FORM_CTRL_ID,MMIWIDGET_WEATHER_SET_HIT_FUNC_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMIWIDGET_WEATHER_SET_LEFTTOP_LABEL_CTRL_ID,MMIWIDGET_WEATHER_SET_LEFTTOP_FORM_CTRL_ID),
        CHILD_DROPDOWNLIST_CTRL(TRUE, MMIWIDGET_WEATHER_SET_LEFTTOP_DROPDOWN_CTRL_ID, MMIWIDGET_WEATHER_SET_LEFTTOP_FORM_CTRL_ID),
        
        //RIGHTTOP
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIWIDGET_WEATHER_SET_RIGHTTOP_FORM_CTRL_ID,MMIWIDGET_WEATHER_SET_HIT_FUNC_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMIWIDGET_WEATHER_SET_RIGHTTOP_LABEL_CTRL_ID,MMIWIDGET_WEATHER_SET_RIGHTTOP_FORM_CTRL_ID),
        CHILD_DROPDOWNLIST_CTRL(TRUE, MMIWIDGET_WEATHER_SET_RIGHTTOP_DROPDOWN_CTRL_ID, MMIWIDGET_WEATHER_SET_RIGHTTOP_FORM_CTRL_ID),
        
        //LEFTBOTTOM
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIWIDGET_WEATHER_SET_LEFTBOTTOM_FORM_CTRL_ID,MMIWIDGET_WEATHER_SET_HIT_FUNC_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMIWIDGET_WEATHER_SET_LEFTBOTTOM_LABEL_CTRL_ID,MMIWIDGET_WEATHER_SET_LEFTBOTTOM_FORM_CTRL_ID),
        CHILD_DROPDOWNLIST_CTRL(TRUE, MMIWIDGET_WEATHER_SET_LEFTBOTTOM_DROPDOWN_CTRL_ID, MMIWIDGET_WEATHER_SET_LEFTBOTTOM_FORM_CTRL_ID),

        //RIGHTBOTTOM
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIWIDGET_WEATHER_SET_RIGHTBOTTOM_FORM_CTRL_ID,MMIWIDGET_WEATHER_SET_HIT_FUNC_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMIWIDGET_WEATHER_SET_RIGHTBOTTOM_LABEL_CTRL_ID,MMIWIDGET_WEATHER_SET_RIGHTBOTTOM_FORM_CTRL_ID),
        CHILD_DROPDOWNLIST_CTRL(TRUE, MMIWIDGET_WEATHER_SET_RIGHTBOTTOM_DROPDOWN_CTRL_ID, MMIWIDGET_WEATHER_SET_RIGHTBOTTOM_FORM_CTRL_ID),

        WIN_SOFTKEY(STXT_SAVE,TXT_NULL,STXT_RETURN),
        END_WIN
};

//背景&时间字体颜色设置窗口
WINDOW_TABLE(WIDGET_WEATHER_SET_BG_TEXTCOLOUR_WIN_TAB) =
{
    WIN_FUNC((uint32)HandleWeatherWidgetBGTextColourSetWinMsg), 
        WIN_ID(WIDGET_WEATHER_SET_BG_TEXTCOLOUR_WIN_ID),
        WIN_TITLE(TXT_WIDGET_WEATHER_BG_TEXTCOLOUR_SETTING),
        WIN_BACKGROUND_ID(IMAGE_COMMON_BG),
        
        CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER, MMIWIDGET_WEATHER_SET_BG_TEXTCOLOUR_FORM_CTRL_ID),

        CHILD_FORM_CTRL(TRUE, GUIFORM_LAYOUT_ORDER, MMIWIDGET_WEATHER_SET_BG_TYPE_FORM_CTRL_ID, MMIWIDGET_WEATHER_SET_BG_TEXTCOLOUR_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIWIDGET_WEATHER_SET_BG_TYPE_LABEL_CTRL_ID, MMIWIDGET_WEATHER_SET_BG_TYPE_FORM_CTRL_ID),
        CHILD_SETLIST_CTRL(TRUE, MMIWIDGET_WEATHER_SET_BG_TYPE_SETLIST_CTRL_ID, MMIWIDGET_WEATHER_SET_BG_TYPE_FORM_CTRL_ID),
        
        CHILD_FORM_CTRL(TRUE, GUIFORM_LAYOUT_ORDER, MMIWIDGET_WEATHER_SET_TEXT_COLOUR_FORM_CTRL_ID, MMIWIDGET_WEATHER_SET_BG_TEXTCOLOUR_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIWIDGET_WEATHER_SET_TEXT_COLOUR_LABEL_CTRL_ID, MMIWIDGET_WEATHER_SET_TEXT_COLOUR_FORM_CTRL_ID),
        CHILD_SETLIST_CTRL(TRUE, MMIWIDGET_WEATHER_SET_TEXT_COLOUR_SETLIST_CTRL_ID, MMIWIDGET_WEATHER_SET_TEXT_COLOUR_FORM_CTRL_ID),
        
        WIN_SOFTKEY(STXT_SAVE,TXT_NULL,STXT_RETURN),
        END_WIN
};

/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/

/*****************************************************************************/
//    Description : widget界面事件处理
//    Global resource dependence : 
//    Author:   xiaoming.ren
//    Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleWidgetWeatherWinMsg(
    MMI_WIN_ID_T        win_id,        //IN:
    MMI_MESSAGE_ID_E    msg_id,        //IN:
    DPARAM            param        //IN:
)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    static BOOLEAN is_update = FALSE;
#if defined  MMIWIDGET_WEATHER_SE
    MMIWEATHER_SETTING_T * setting_info_ptr = MMIWEATHER_GetSettingInfo();
#ifdef MMI_WEATHER_TEST
    static uint8    timer_id = 0;
#endif
#endif

    //SCI_TRACE_LOW:"HandleWidgetWeatherWinMsg msg_id = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_WEATHER_999_112_2_18_3_8_46_246,(uint8*)"d", msg_id);
	
    switch (msg_id)
    {
	case MSG_OPEN_WINDOW:
	    is_update = FALSE;
#ifdef PDA_WEATHER_TIME_SE  
        TM_GetSysTime(&s_sys_time);
#endif //PDA_WEATHER_TIME_SE
        
        InitWidgetWeatherSetInfo();
        GetCurWeatherInfo();
        MMIWEATHER_InitNetworkAccount();
#ifdef PDA_WEATHER_TIME_SE
        CreateTimeBoxLayer(&s_min_old_layer,  MINUTE_PAGE_OFFSET_X, MINUTE_PAGE_OFFSET_Y);
        // -2- ======= Create New Time Layer ========
        CreateTimeBoxLayer(&s_min_new_layer,  MINUTE_PAGE_OFFSET_X, MINUTE_PAGE_OFFSET_Y);
        // -3- ======= Create temp layer used to calcute frame data========
        CreateTimeBoxLayer(&s_min_calc_layer,  MINUTE_PAGE_OFFSET_X, MINUTE_PAGE_OFFSET_Y);
         // -4- ======= hour Old ========
        CreateTimeBoxLayer(&s_hour_old_layer,  HOUR_PAGE_OFFSET_X, HOUR_PAGE_OFFSET_Y);
        // -5- ======= hour New ========
        CreateTimeBoxLayer(&s_hour_new_layer,  HOUR_PAGE_OFFSET_X, HOUR_PAGE_OFFSET_Y);
        // -6- ======= hour calcute ========
        CreateTimeBoxLayer(&s_hour_calc_layer,  HOUR_PAGE_OFFSET_X, HOUR_PAGE_OFFSET_Y);
#endif
#ifndef PDA_WEATHER_SUPPORT_LOW_MEMORY
        CreatHighlightLayer(win_id);
#endif

#if defined  MMIWIDGET_WEATHER_SE
        s_weather_se.CreateResourceBuffer = BarPhone_CreateImageBuffer;  /*lint !e64 */
        s_weather_se.DestroyResourceBuffer = BarPhone_DestroyImageBuffer;    /*lint !e64 */     
        s_weather_se.Display = BarPhone_Display;         /*lint !e64 */
        if(PNULL != setting_info_ptr)
        {
            s_weather_t.cur_city_idx= setting_info_ptr->cur_city;
        }
        else
        {
            SCI_TRACE_LOW("HandleWidgetWeatherWinMsg PNULL == setting_info_ptr");
        }
        s_weather_t.is_need_disappear_anim = TRUE;
#ifdef MMI_WEATHER_TEST
        timer_id = MMK_CreateWinTimer(win_id, 15000, FALSE);
        s_weather_t.s_msg_id= 1;
        s_weather_t.s_pre_msg_id= s_weather_t.s_msg_id;
#endif
#endif        
		break;
    case MSG_WIDGET_MODIFY_RECT:
        CalcHitRect(win_id);
        break;
    case MSG_GRID_ADD_WIDGET:
#ifdef PDA_WEATHER_TIME_SE
        TM_GetSysTime(&s_sys_time);
#endif
        break;
#if defined TOUCH_PANEL_SUPPORT
	case MSG_WIDGET_TP_PRESS_UP:
        recode= MMIWIDGET_Weather_HandleTpPressUp(win_id,param);
		break;
        
    case MSG_WIDGET_TP_PRESS_DOWN:
        recode= MMIWIDGET_Weather_HandleTpPressDown(win_id,param);
        break;
        
    case MSG_WIDGET_MOVE_START:
    case MSG_WIDGET_PAGE_SLIDE_START:
        if (MMIWIDGET_WEATHER_CLICK_RECT_INVALID != s_tp_hit_type)
        {
            ResetWidgetWeather();
        }
#ifdef PDA_WEATHER_TIME_SE
        if(0 != s_se_timer_id)
        {
            StopTimeUpdateSe();
        }
#endif //PDA_WEATHER_TIME_SE

#ifdef MMIWIDGET_WEATHER_ANIM
        DestoryWeatherAnim();
#endif
        s_cur_focus_index = 0xff;
        break;
#endif 

    case MSG_TIMER:
#ifdef MMI_WEATHER_TEST 
        if(*(uint8*)param == timer_id)
        {
            MMK_StopTimer(timer_id);
            timer_id = MMK_CreateWinTimer(win_id, 15000, FALSE);
            s_weather_t.s_msg_id++; 
            s_weather_se.current_frame = -1;
        }
#endif
        break;



	case MSG_FULL_PAINT:
        if(is_update)
        {
            GetCurWeatherInfo();
            InitWidgetWeatherSetInfo();
#ifdef PDA_WEATHER_TIME_SE
            MMIAPIWEATHER_HandleTimeUpdate();
#endif //PDA_UI_ANDROID_WEATHER_TIME_SE
            is_update = FALSE;
        }
#ifdef   BAR_PHONE_WIDGET   
        GetCurWeatherInfo();
        BarPhoneWeather_SetWebKeyCondition();
#if !defined MMIWIDGET_WEATHER_SE
        BarPhoneWeather_Redraw(win_id);
#endif
#else       
        MMIWIDGET_Weather_Redraw(win_id);//NEWMS00124462
#endif        
        SetShortCutFocus(win_id,s_cur_focus_index);
#ifdef PDA_WEATHER_TIME_SE
            if (!MMIAPIWEATHER_HandleTimeUpdate()
            && 0 == s_se_timer_id)
            {
#ifdef MMIWIDGET_WEATHER_ANIM
                MMK_SendMsg(win_id, MSG_WEATHER_START_ANIM, PNULL);
#endif   
            }
#endif
        break;

#ifdef   BAR_PHONE_WIDGET   
    case MSG_WIDGET_UPDATE_SOFTKEY:
        BarPhoneWeather_DisplaySoftkey(win_id, FALSE);    
        break;
#endif

    case MSG_GET_FOCUS:
        {
            is_update = TRUE;
        }
        break;

    case MSG_LOSE_FOCUS:
        if (MMIWIDGET_WEATHER_CLICK_RECT_INVALID != s_tp_hit_type)
        {
            //ResetWidgetWeather();
            s_tp_hit_type = MMIWIDGET_WEATHER_CLICK_RECT_INVALID;
        }
#ifdef PDA_WEATHER_TIME_SE
        if(0 != s_se_timer_id)
        {
            StopTimeUpdateSe();
        }
#endif //PDA_WEATHER_TIME_SE

#ifdef MMIWIDGET_WEATHER_ANIM
        DestoryWeatherAnim();
#endif
        s_cur_focus_index = 0xff;
#ifdef MMIWIDGET_WEATHER_SE
        s_weather_t.is_need_disappear_anim = TRUE;
#endif
        break;

#ifdef BAR_PHONE_WIDGET
#ifdef MMIWIDGET_WEATHER_SE
    case MSG_APP_UP:
        s_weather_t.is_need_disappear_anim = TRUE;
        if(PNULL != setting_info_ptr)
        {
            s_weather_t.cur_city_idx = setting_info_ptr->cur_city - 1;
            s_weather_se.current_frame = -1;
        }
        break;

    case MSG_APP_DOWN:
        s_weather_t.is_need_disappear_anim = TRUE;
        if(PNULL != setting_info_ptr)
        {
            s_weather_t.cur_city_idx = setting_info_ptr->cur_city + 1;
            s_weather_se.current_frame = -1;
        }
        break;
#endif
#else
    case MSG_KEYREPEAT_DOWN:
    case MSG_APP_DOWN:
        if(MMIWIDGET_WEATHER_CLICK_RECT_RIGHT_BOTTOM < s_cur_focus_index)
        {
            s_cur_focus_index = 0;
        }
        else
        {
            s_cur_focus_index++;
        }
        if(!SetShortCutFocus(win_id,s_cur_focus_index))
        {
            recode = MMI_RESULT_FALSE;
        }
        break;

    case MSG_KEYREPEAT_UP:
    case MSG_APP_UP:
        if(MMIWIDGET_WEATHER_CLICK_RECT_RIGHT_BOTTOM < s_cur_focus_index)
        {
            s_cur_focus_index = MMIWIDGET_WEATHER_CLICK_RECT_RIGHT_BOTTOM;
        }
        else
        {
            s_cur_focus_index--;
        }
        if(!SetShortCutFocus(win_id,s_cur_focus_index))
        {
            recode = MMI_RESULT_FALSE;
        }
        break;
#endif

    case MSG_WIDGET_DEACTIVE:

        break;

    case MSG_WIDGET_ACTIVE:
        if(0xff == s_cur_focus_index)
        {
            s_cur_focus_index = 0;
        }
        else if(MMIWIDGET_WEATHER_CLICK_RECT_INVALID == s_cur_focus_index)
        {
            s_cur_focus_index = MMIWIDGET_WEATHER_CLICK_RECT_RIGHT_BOTTOM;
        }
        if(!SetShortCutFocus(win_id,s_cur_focus_index))
        {
            recode = MMI_RESULT_FALSE;
        }
        break;      

    case MSG_APP_WEB:
#ifdef BAR_PHONE_WIDGET  
        BarPhoneWeather_HandleWebKey();
#else
        MMIWIDGET_Weather_HandleWebKey(s_cur_focus_index);
#endif
        break;
        
    case MSG_CLOSE_WINDOW:
        s_cur_focus_index = 0xff;
        ResetWidgetWeather();
#ifdef PDA_WEATHER_TIME_SE
        RealeaseTimeSeLayer();
#endif
        break;
        
    case MSG_WEATHER_WGT_UPDATE_CNF:
    case MSG_WEATHER_WGT_SWITCH_CITY_CNF:
        MMIPUB_ClearTips();
        if(MMK_IsFocusWin(win_id))
        {
            GetCurWeatherInfo();
            MMK_UpdateScreen();
        }
        break;

#ifdef MMIWIDGET_WEATHER_ANIM
    case MSG_WEATHER_NEED_ANIM:
        SetWeatherAnimState(TRUE);
        break;

    case MSG_WEATHER_START_ANIM:     
        InitWeatherAnim(win_id);
        break;

    case MSG_WEATHER_FORBID_ANIM:
        SetIsForbidAnim(TRUE);
        break;
#endif    

    default:
		recode = MMI_RESULT_FALSE;
		break;
    }
    
#if defined  MMIWIDGET_WEATHER_SE
    MMIWIDGET_HandleWidgetSpecialEffectWinMsg(&s_weather_se, win_id, msg_id, param);
#endif

    return recode;        
}

#ifdef MMIWIDGET_WEATHER_ANIM
/*****************************************************************************/
//    Description : widget界面天气动态特效初始化
//    Global resource dependence : 
//    Author:   jin.wang
//    Note:
/*****************************************************************************/
LOCAL void InitWeatherAnim(MMI_WIN_ID_T win_id)
{
    MMIWEATHER_WEATHER_T* weather_info_ptr = MMIWEATHER_GetCurCityWeatherInfo();
    MMIWEATHER_ANIM_T *anim_info_ptr = MMIWEATHER_GetAnimInfo();
    UILAYER_CREATE_T create_info = {0};
    UILAYER_RESULT_E create_result = UILAYER_RESULT_ERROR;
    MMIWEATHER_CONDITION_E condition = 0;
    BOOLEAN is_all_ready = FALSE;
    
    if (PNULL == weather_info_ptr
        || PNULL == anim_info_ptr || (!anim_info_ptr->is_need_anim)
        || anim_info_ptr->is_forbid)
    {
        return;
    }

    condition = MMIWEATHER_GetCurCondition();

    //目前只完成5种天气动态特效
    //晴天、雨天、阴天、多云、雪
    if (condition != WEATHER_CONDITION_SUNNY
        && condition != WEATHER_CONDITION_RAIN
        && condition != WEATHER_CONDITION_CLOUDY
        && condition != WEATHER_CONDITION_MOSTLY_CLOUDY
        && condition != WEATHER_CONDITION_SNOW)
    {
        return;
    }

    anim_info_ptr->is_need_anim = FALSE;
    anim_info_ptr->is_forbid = FALSE;

    //1 动画 
    //2 图层
    //3 声音
    do
    {
        //-1- 动画
        GetAnimInfoByCondition(MMIWEATHER_GetCurCondition(), anim_info_ptr);

        if (0 == anim_info_ptr->layer_cnt) break;
        
        //-2- 图层
        create_result = CreateAnimLayer(&create_info, win_id, anim_info_ptr);

        if (UILAYER_RESULT_SUCCESS != create_result) break;

        //-3- 声音 暂时没有
        //ring_id = GetRingIdByCondition(weather_info_ptr->current.condition);

        //if (0 == ring_id) break;

        is_all_ready = TRUE;
    }
    while(0);

    if (is_all_ready)
    {
        //MMIAPISET_PlayRingById(TRUE, ring_id, 1, MMISET_RING_TYPE_ALARM, PNULL);
        CreateAnimTimer(win_id, anim_info_ptr, MMIWEATHER_GetCurCondition());
    }
    else
    {
        ReleaseAnimLayer(anim_info_ptr);
    }

    return;
}

/*****************************************************************************/
//    Description : 是否屏蔽天气特效
//    Global resource dependence : 
//    Author:   jin.wang
//    Note:非屏蔽状态下设置需要才能播放特效
/*****************************************************************************/
LOCAL void SetIsForbidAnim(BOOLEAN is_forbid)
{
    MMIWEATHER_ANIM_T *anim_info_ptr = MMIWEATHER_GetAnimInfo();

    if (PNULL == anim_info_ptr)
    {
        return;
    }

    anim_info_ptr->is_forbid = is_forbid;

    return;
}

/*****************************************************************************/
//    Description : 是否需要动态天气特效
//    Global resource dependence : 
//    Author:   jin.wang
//    Note:
/*****************************************************************************/
LOCAL void SetWeatherAnimState(BOOLEAN is_need)
{
    MMIWEATHER_ANIM_T *anim_info_ptr = MMIWEATHER_GetAnimInfo();

    if (PNULL == anim_info_ptr)
    {
        return;
    }

    anim_info_ptr->is_need_anim = is_need;

    return;
}

/*****************************************************************************/
//    Description : 创建显示动态特效的图层
//    Global resource dependence : 
//    Author:   jin.wang
//    Note:
/*****************************************************************************/
LOCAL UILAYER_RESULT_E CreateAnimLayer(
    UILAYER_CREATE_T* create_info_ptr,
    MMI_WIN_ID_T win_id,
    MMIWEATHER_ANIM_T* anim_info_ptr
)
{
    
    UILAYER_RESULT_E result = UILAYER_RESULT_ERROR;
    UILAYER_APPEND_BLT_T append_layer = {0};
    uint16 layer_idx = 0;
    
    if (PNULL == create_info_ptr || PNULL == anim_info_ptr)
    {
        return UILAYER_RESULT_ERROR;
    }

    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &create_info_ptr->width, &create_info_ptr->height);/*lint !e64*/

    //creat layer
    create_info_ptr->lcd_id = GUI_MAIN_LCD_ID;
    create_info_ptr->owner_handle = win_id;
    create_info_ptr->offset_x = 0;
    create_info_ptr->offset_y = 0;
    create_info_ptr->is_bg_layer = FALSE;
    create_info_ptr->is_static_layer = FALSE; 

    for (layer_idx = 0; layer_idx < anim_info_ptr->layer_cnt; layer_idx++)
    {
        result = UILAYER_CreateLayer(create_info_ptr, &anim_info_ptr->layer_info[layer_idx].dev_info);    

        if (UILAYER_RESULT_SUCCESS != result)
        {
            break;
        }

        UILAYER_SetLayerColorKey(&anim_info_ptr->layer_info[layer_idx].dev_info, TRUE, UILAYER_TRANSPARENT_COLOR);
        
        //add into layer queue
        append_layer.lcd_dev_info = anim_info_ptr->layer_info[layer_idx].dev_info;
        append_layer.layer_level = UILAYER_LEVEL_NORMAL;
        
        UILAYER_AppendBltLayer(&append_layer);
    }

    if (UILAYER_RESULT_SUCCESS != result)
    {
        for (layer_idx = 0; layer_idx < anim_info_ptr->layer_cnt; layer_idx++)
        {
            UILAYER_RELEASELAYER(&anim_info_ptr->layer_info[layer_idx].dev_info);   /*lint !e506 !e774*/
//            UILAYER_ReleaseLayer(&anim_info_ptr->layer_info[layer_idx].dev_info);
        }
    }

    return result;
}

/*****************************************************************************/
//    Description : 销毁显示动态特效的图层
//    Global resource dependence : 
//    Author:   jin.wang
//    Note:
/*****************************************************************************/
LOCAL void ReleaseAnimLayer(MMIWEATHER_ANIM_T* anim_info_ptr)
{
    uint16 layer_idx = 0;
    
    if (PNULL == anim_info_ptr)
    {
        return;
    }

    if (UILAYER_IsMultiLayerEnable())
    {
        for (layer_idx = 0; layer_idx < anim_info_ptr->layer_cnt;layer_idx++)
        {
            if (0 != anim_info_ptr->layer_info[layer_idx].dev_info.block_id
                && UILAYER_NULL_HANDLE != anim_info_ptr->layer_info[layer_idx].dev_info.block_id)
            {
                UILAYER_RELEASELAYER(&anim_info_ptr->layer_info[layer_idx].dev_info);   /*lint !e506 !e774*/
//                UILAYER_ReleaseLayer(&anim_info_ptr->layer_info[layer_idx].dev_info);
//                anim_info_ptr->layer_info[layer_idx].dev_info.lcd_id = GUI_MAIN_LCD_ID;
//                anim_info_ptr->layer_info[layer_idx].dev_info.block_id = UILAYER_NULL_HANDLE;
            }
        }
    }


    return;
}

/*****************************************************************************/
//    Description : 处理天气特效timer
//    Global resource dependence : 
//    Author:   jin.wang
//    Note:
/*****************************************************************************/
LOCAL void DisplayAnimCallback(uint8 timer_id, DPARAM param)
{
    MMI_WIN_ID_T win_id = (MMI_WIN_ID_T)param;
    MMIWEATHER_ANIM_T* anim_info_ptr = MMIWEATHER_GetAnimInfo();
    MMIWEATHER_WEATHER_T* weather_info_ptr = MMIWEATHER_GetCurCityWeatherInfo();
    uint16 layer_idx = 0;
    BOOLEAN is_all_disp_end = TRUE;
    
    if (PNULL == anim_info_ptr || PNULL == weather_info_ptr) 
    {
        return;
    }
    
    if(timer_id != anim_info_ptr->anim_timer_id)
    {
        return;
    }
    
    for (layer_idx = 0; layer_idx < anim_info_ptr->layer_cnt; layer_idx++)
    {
        MMIWEATHE_ANIM_LAYER_T layer_info = anim_info_ptr->layer_info[layer_idx];

        if (WEATHER_CONDITION_RAIN == MMIWEATHER_GetCurCondition())
        {
            //重复的图片不刷新
            if (ANIM_MOVE_NONE == layer_info.move_direction)
            {
                if (layer_info.cur_pic_idx > 0
                    && layer_info.cur_pic_idx < layer_info.pic_cnt
                    && layer_info.pic_info[layer_info.cur_pic_idx] == layer_info.pic_info[layer_info.cur_pic_idx - 1])
                {
                    is_all_disp_end = FALSE;
                    anim_info_ptr->layer_info[layer_idx].cur_pic_idx++;
                    continue;
                }
            }
            //雨天效果先刷雨点然后再刷新雨刷
            if (0 != layer_idx
                && anim_info_ptr->layer_info[0].cur_pic_idx < anim_info_ptr->layer_info[0].pic_cnt)
            {
                is_all_disp_end = FALSE;
                continue;
            }

            SetWeatherAnimLayerAlpha(MMIWEATHER_GetCurCondition(), anim_info_ptr, layer_idx);
        }
        
        if (layer_info.cur_pic_idx < layer_info.pic_cnt)
        {
            DrawWeatherAnim(win_id, &layer_info);
            anim_info_ptr->layer_info[layer_idx] = layer_info;
            is_all_disp_end = FALSE;
        }
    }

    //全部播放完毕，销毁动画
    if (is_all_disp_end)
    {
        DestoryWeatherAnim();
    }
    
    return;
}

/*****************************************************************************/
//    Description : 设置图层透明度
//    Global resource dependence : 
//    Author:   jin.wang
//    Note:
/*****************************************************************************/
LOCAL void SetWeatherAnimLayerAlpha(
    MMIWEATHER_CONDITION_E condition,
    MMIWEATHER_ANIM_T* anim_info_ptr,
    uint16 layer_idx
)
{
    uint8 alpha[MMIWEATHER_MAX_ANIM_PIC_NUM] = {110, 105, 100, 95, 85, 60, 40, 30};
    MMIWEATHE_ANIM_LAYER_T cur_layer_info = {0};
    BOOLEAN is_need_set_alpha = FALSE;
    uint8 idx = 0;
    
    if (PNULL == anim_info_ptr || WEATHER_CONDITION_RAIN != condition)
    {
        return;
    }

    cur_layer_info = anim_info_ptr->layer_info[layer_idx];

    //雨天效果出现蒙版和雨刷后设置图层透明度，达到刷雨滴的效果
    if ((0 == layer_idx && cur_layer_info.cur_pic_idx == cur_layer_info.pic_cnt)
        || (1 == layer_idx && cur_layer_info.cur_pic_idx <= cur_layer_info.pic_cnt
        && cur_layer_info.cur_pic_idx > 0))
    {
        is_need_set_alpha = TRUE;

        if (0 == layer_idx)
        {
            idx = 0;
        }
        else
        {
            idx = cur_layer_info.cur_pic_idx;
        }
    }  

    if (is_need_set_alpha)
    {
        UILAYER_WeakLayerAlpha(&anim_info_ptr->layer_info[layer_idx].dev_info, alpha[idx]);
    }

    return;
}

/*****************************************************************************/
//    Description : 获取天气特效显示位置
//    Global resource dependence : 
//    Author:   jin.wang
//    Note:
/*****************************************************************************/
LOCAL void DrawWeatherAnim(
    MMI_WIN_ID_T win_id,
    MMIWEATHE_ANIM_LAYER_T* layer_info_ptr
)
{
    MMIWEATHER_WEATHER_T* weather_info_ptr = MMIWEATHER_GetCurCityWeatherInfo();
    GUI_POINT_T disp_point = {0};
    uint16 width = 0;
    uint16 height = 0;
    uint16 pic_idx = 0;
    
    if (PNULL == weather_info_ptr || PNULL == layer_info_ptr) 
    {
        return;
    }

    pic_idx = layer_info_ptr->cur_pic_idx;
    
    if (0 == layer_info_ptr->pic_info[pic_idx]
        || pic_idx >= MMIWEATHER_MAX_ANIM_PIC_NUM)
    {
        pic_idx = 0;
    }

    if (layer_info_ptr->move_direction == ANIM_MOVE_R_TO_L
        || layer_info_ptr->move_direction == ANIM_MOVE_L_TO_R)
    {
        
        //第一帧的位置初始化时已经赋值
        if (layer_info_ptr->cur_pic_idx > 0)
        {
            if (ANIM_MOVE_R_TO_L == layer_info_ptr->move_direction)
            {
                layer_info_ptr->cur_layer_point.x += MMIWEATHER_ANIM_MOVE_LENGTH;
            }
            else
            {
                layer_info_ptr->cur_layer_point.x -= MMIWEATHER_ANIM_MOVE_LENGTH;
            }
        }
        else
        {            
            GUI_RECT_T img_rect = {0};

            GUIRES_GetImgWidthHeight(&width, &height, 
                layer_info_ptr->pic_info[0],
                win_id);
            
            img_rect.right = width;
            img_rect.bottom = height;        

            //第一帧
            GUIRES_DisplayImg(&disp_point, PNULL, &img_rect, win_id, 
                layer_info_ptr->pic_info[0],
                &layer_info_ptr->dev_info);
        }

        UILAYER_SetLayerPosition(&layer_info_ptr->dev_info, 
            layer_info_ptr->cur_layer_point.x,
            layer_info_ptr->cur_layer_point.y);

    }
    else if (layer_info_ptr->move_direction == ANIM_MOVE_NONE)
    {
        UILAYER_Clear(&layer_info_ptr->dev_info);  
        GUIRES_DisplayImg(&disp_point, PNULL, PNULL, win_id, 
            layer_info_ptr->pic_info[pic_idx],
            &layer_info_ptr->dev_info);
    }

    layer_info_ptr->cur_pic_idx++;

    return;
}

/*****************************************************************************/
//    Description : 创建显示动态特效的timer
//    Global resource dependence : 
//    Author:   jin.wang
//    Note:
/*****************************************************************************/
LOCAL void CreateAnimTimer(
    MMI_WIN_ID_T win_id,
    MMIWEATHER_ANIM_T* anim_info_ptr,
    MMIWEATHER_CONDITION_E  condition
)
{
    uint32 time_out = 0;
    
    if (WEATHER_CONDITION_CLOUDY == condition
        || WEATHER_CONDITION_MOSTLY_CLOUDY == condition)
    {
        time_out = 20;
    }
    else if (WEATHER_CONDITION_RAIN == condition)
    {
        time_out = 150;
    }
    else if (WEATHER_CONDITION_SNOW == condition)
    {
        time_out = 300;
    }
    else
    {
        time_out = 300;
    }
    
    anim_info_ptr->anim_timer_id = MMK_CreateTimerCallback(
                                       time_out,
                                       DisplayAnimCallback,/*lint !e64*/
                                       win_id,
                                       TRUE);
}

/*****************************************************************************/
//    Description : 关闭动态特效timer
//    Global resource dependence : 
//    Author:   jin.wang
//    Note:
/*****************************************************************************/
LOCAL void StopDyWeatherTimer(MMIWEATHER_ANIM_T* anim_info_ptr)
{
    if (PNULL == anim_info_ptr)
    {
        return;
    }

    if (0 != anim_info_ptr->anim_timer_id)
    {
        MMK_StopTimer(anim_info_ptr->anim_timer_id);
        anim_info_ptr->anim_timer_id = 0;
    }

    return;
}

/*****************************************************************************/
//    Description : 销毁天气动画
//    Global resource dependence : 
//    Author:   jin.wang
//    Note:
/*****************************************************************************/
LOCAL void DestoryWeatherAnim(void)
{
    MMIWEATHER_ANIM_T* anim_info_ptr = MMIWEATHER_GetAnimInfo();

    if (PNULL == anim_info_ptr)
    {
        return;
    }

    StopDyWeatherTimer(anim_info_ptr);
    ReleaseAnimLayer(anim_info_ptr);
    anim_info_ptr->is_forbid = FALSE;
    anim_info_ptr->is_need_anim = FALSE;
}

/*****************************************************************************/
//  Description : 根据天气状况获取动画资源
//  Global resource dependence : none
//  Author: jin.wang
//  Note:
/*****************************************************************************/
LOCAL void GetAnimInfoByCondition
(
    MMIWEATHER_CONDITION_E condition_id,
    MMIWEATHER_ANIM_T* anim_info_ptr
)
{
    //widget天气动态效果图片组 
    MMI_IMAGE_ID_T anim_image_array[] [MMIWEATHER_MAX_ANIM_PIC_NUM]= 
    {
        {0},
        {0},
        {0},
        {0},
        {IMAGE_WIDGET_WEATHER_ANIM_CLOUDY1, IMAGE_WIDGET_WEATHER_ANIM_CLOUDY2},/*lint !e785*/
        {0},
        {IMAGE_WIDGET_WEATHER_ANIM_MOSTLY_CLOUDY1, IMAGE_WIDGET_WEATHER_ANIM_CLOUDY1,
         IMAGE_WIDGET_WEATHER_ANIM_CLOUDY2},/*lint !e785*/
        {0},
        {IMAGE_WIDGET_WEATHER_ANIM_RAIN1, IMAGE_WIDGET_WEATHER_ANIM_RAIN2,
         IMAGE_WIDGET_WEATHER_ANIM_RAIN3, IMAGE_WIDGET_WEATHER_ANIM_RAIN4,
         IMAGE_WIDGET_WEATHER_ANIM_RAIN5, IMAGE_WIDGET_WEATHER_ANIM_RAIN6},/*lint !e785*/
        {0},
        {0},
        {0},
        {0},
        {0},
        {0},
        {0},
        {0},
        {IMAGE_WIDGET_WEATHER_ANIM_SUNNY1, IMAGE_WIDGET_WEATHER_ANIM_SUNNY2,
         IMAGE_WIDGET_WEATHER_ANIM_SUNNY3, IMAGE_WIDGET_WEATHER_ANIM_SUNNY4},/*lint !e785*/
        {0},
        {0},
        {0},
        {0},
        {0},
        {0},
        {0},
        {0},
        {0},
        {IMAGE_WIDGET_WEATHER_ANIM_SNOW1, IMAGE_WIDGET_WEATHER_ANIM_SNOW2,
         IMAGE_WIDGET_WEATHER_ANIM_SNOW3},/*lint !e785*/
        {0},
        {0},
        {0},
    };
    uint16 width = 0;
    uint16 height = 0;
    
    if (PNULL == anim_info_ptr)
    {
        return;
    }

    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &width, &height);
    
    SCI_MEMSET(anim_info_ptr->layer_info, 
        0, 
        MMIWEATHER_MAX_ANIM_LAYER_NUM * sizeof(MMIWEATHE_ANIM_LAYER_T));
    
    switch(condition_id)
    {
        case WEATHER_CONDITION_SUNNY:
            anim_info_ptr->layer_cnt = 1;
            anim_info_ptr->layer_info[0].pic_cnt = 7;
            anim_info_ptr->layer_info[0].pic_info[0] = anim_image_array[condition_id][0];
            anim_info_ptr->layer_info[0].pic_info[1] = anim_image_array[condition_id][1];
            anim_info_ptr->layer_info[0].pic_info[2] = anim_image_array[condition_id][2];
            anim_info_ptr->layer_info[0].pic_info[3] = anim_image_array[condition_id][3];
            anim_info_ptr->layer_info[0].pic_info[4] = anim_image_array[condition_id][2];
            anim_info_ptr->layer_info[0].pic_info[5] = anim_image_array[condition_id][1];
            anim_info_ptr->layer_info[0].pic_info[6] = anim_image_array[condition_id][0];
            break;
            
        case WEATHER_CONDITION_RAIN:
            anim_info_ptr->layer_cnt = 3;
            
            anim_info_ptr->layer_info[0].pic_cnt = 4;
            anim_info_ptr->layer_info[0].pic_info[0] = anim_image_array[condition_id][0];
            anim_info_ptr->layer_info[0].pic_info[1] = anim_image_array[condition_id][0];
            anim_info_ptr->layer_info[0].pic_info[2] = anim_image_array[condition_id][1];
            anim_info_ptr->layer_info[0].pic_info[3] = anim_image_array[condition_id][1];

            anim_info_ptr->layer_info[1].pic_cnt = 3;
            anim_info_ptr->layer_info[1].pic_info[0] = anim_image_array[condition_id][2];
            anim_info_ptr->layer_info[1].pic_info[1] = anim_image_array[condition_id][2];
            anim_info_ptr->layer_info[1].pic_info[2] = anim_image_array[condition_id][2];

            anim_info_ptr->layer_info[2].pic_cnt = 7;
            anim_info_ptr->layer_info[2].pic_info[0] = anim_image_array[condition_id][3];
            anim_info_ptr->layer_info[2].pic_info[1] = anim_image_array[condition_id][4];
            anim_info_ptr->layer_info[2].pic_info[2] = anim_image_array[condition_id][4];
            anim_info_ptr->layer_info[2].pic_info[3] = anim_image_array[condition_id][5];
            anim_info_ptr->layer_info[2].pic_info[4] = anim_image_array[condition_id][4];
            anim_info_ptr->layer_info[2].pic_info[5] = anim_image_array[condition_id][4];
            anim_info_ptr->layer_info[2].pic_info[6] = anim_image_array[condition_id][3];
            break;
            
        case WEATHER_CONDITION_CLOUDY:
            anim_info_ptr->layer_cnt = 2;
            
            anim_info_ptr->layer_info[0].cur_layer_point.x = (-MMIWEATHER_ANIM_MOVE_START_POS);
            anim_info_ptr->layer_info[0].cur_layer_point.y = height / 4;
            anim_info_ptr->layer_info[0].move_direction = ANIM_MOVE_R_TO_L;
            anim_info_ptr->layer_info[0].pic_cnt = MMIWEATHER_MAX_ANIM_MOVE_CNT;
            anim_info_ptr->layer_info[0].pic_info[0] = anim_image_array[condition_id][0];

            anim_info_ptr->layer_info[1].cur_layer_point.x = width - MMIWEATHER_ANIM_MOVE_LENGTH;
            anim_info_ptr->layer_info[1].cur_layer_point.y = height / 4;
            anim_info_ptr->layer_info[1].move_direction = ANIM_MOVE_L_TO_R;
            anim_info_ptr->layer_info[1].pic_cnt = MMIWEATHER_MAX_ANIM_MOVE_CNT;
            anim_info_ptr->layer_info[1].pic_info[0] = anim_image_array[condition_id][1];
            break;
            
        case WEATHER_CONDITION_SNOW:
            anim_info_ptr->layer_cnt = 1;

            anim_info_ptr->layer_info[0].pic_cnt = 5;
            anim_info_ptr->layer_info[0].pic_info[0] = anim_image_array[condition_id][0];
            anim_info_ptr->layer_info[0].pic_info[1] = anim_image_array[condition_id][1];
            anim_info_ptr->layer_info[0].pic_info[2] = anim_image_array[condition_id][2];
            anim_info_ptr->layer_info[0].pic_info[3] = anim_image_array[condition_id][1];
            anim_info_ptr->layer_info[0].pic_info[4] = anim_image_array[condition_id][0];
            break;
            
        case WEATHER_CONDITION_MOSTLY_CLOUDY:
            anim_info_ptr->layer_cnt = 3;
            
            anim_info_ptr->layer_info[0].pic_cnt = 1;
            anim_info_ptr->layer_info[0].pic_info[0] = anim_image_array[condition_id][0];

            anim_info_ptr->layer_info[1].cur_layer_point.x = (-MMIWEATHER_ANIM_MOVE_START_POS);
            anim_info_ptr->layer_info[1].cur_layer_point.y = height / 4;
            anim_info_ptr->layer_info[1].move_direction = ANIM_MOVE_R_TO_L;
            anim_info_ptr->layer_info[1].pic_cnt = MMIWEATHER_MAX_ANIM_MOVE_CNT;
            anim_info_ptr->layer_info[1].pic_info[0] = anim_image_array[condition_id][1];

            anim_info_ptr->layer_info[2].cur_layer_point.x = width - MMIWEATHER_ANIM_MOVE_LENGTH;
            anim_info_ptr->layer_info[2].cur_layer_point.y = height / 4;
            anim_info_ptr->layer_info[2].move_direction = ANIM_MOVE_L_TO_R;
            anim_info_ptr->layer_info[2].pic_cnt = MMIWEATHER_MAX_ANIM_MOVE_CNT;
            anim_info_ptr->layer_info[2].pic_info[0] = anim_image_array[condition_id][2];

            break;
            
        default :

            break;
    }

    return;
}

/*****************************************************************************/
//  Description : 根据天气状况获取铃声
//  Global resource dependence : none
//  Author: jin.wang
//  Note:
/*****************************************************************************/
MMI_RING_ID_T GetRingIdByCondition(MMIWEATHER_CONDITION_E condition_id)
{
    MMI_RING_ID_T ring_id = 0;
    
    switch(condition_id)
    {
        case WEATHER_CONDITION_SUNNY:
            ring_id = 1;
            break;
            
        case WEATHER_CONDITION_RAIN:
            ring_id = 1;
            break;
            
        case WEATHER_CONDITION_CLOUDY:
            ring_id = 1;
            break;
            
        case WEATHER_CONDITION_SNOW:
            ring_id = 1;
            break;
            
        case WEATHER_CONDITION_MOSTLY_CLOUDY:
            ring_id = 1;
            break;
            
        default :
            break;
    }
    
    return ring_id;
}
#endif
/*****************************************************************************/
//    Description : widget界面显示
//    Global resource dependence : 
//    Author:   xiaoming.ren
//    Note:
/*****************************************************************************/
LOCAL void MMIWIDGET_Weather_Redraw(MMI_WIN_ID_T     win_id)
{
    GUI_RECT_T win_rect = {0};
    GUI_POINT_T      point={0};
    
    MMK_GetWinRect(win_id, &win_rect);

    point.x = win_rect.left;
    point.y = win_rect.top;
    
    MMIWIDGET_Weather_Display_Time_WeatherInfo(win_id, point); 
}

/*****************************************************************************/
//    Description : 显示时间和天气信息
//    Global resource dependence : 
//    Author:   xiaoming.ren
//    Note:
/*****************************************************************************/
LOCAL void MMIWIDGET_Weather_Display_Time_WeatherInfo(MMI_WIN_ID_T        win_id, GUI_POINT_T  bg_point)
{
    GUI_LCD_DEV_INFO     lcd_dev_info    = {0};
    SCI_TIME_T           sys_time        = {0};
    GUI_RECT_T           bg_rect         = {0};
    uint16               bg_height       = 0;
    uint16               bg_width        = 0;
#ifndef PDA_WEATHER_TIME_SE
    uint16               icon_height     = 0;
    uint16               icon_width      = 0;
#endif
    MMI_IMAGE_ID_T       icon_id         = 0;
    
    //get system time
    TM_GetSysTime(&sys_time);
    
    MMK_GetWinLcdDevInfo(win_id,&lcd_dev_info);
    
    if (!UILAYER_IsLayerActive(&lcd_dev_info))
    {
        return;
    }
        if (GUIRES_GetImgWidthHeight(&bg_width, &bg_height, g_weather_widget.large_bg_img, win_id))
        {
            GUI_POINT_T      start_point  = {0};
            uint16           need_dis_bg  = 0;
            bg_rect.left   = bg_point.x;
            bg_rect.top    = bg_point.y;
            bg_rect.right  = bg_rect.left + bg_width;
            bg_rect.bottom = bg_rect.top + bg_height;
            
            need_dis_bg    = ParserNvValue(MMIWIDGET_WEATHER_BG_TEXTCOLOUR_SET_NV_ITEM_BG, &s_cur_bg_textcolour_set_info);
            
            if (1 == need_dis_bg)
            {
                //显示背景图
                GUIRES_DisplayImg(&bg_point, PNULL, PNULL, win_id, g_weather_widget.large_bg_img, &lcd_dev_info);
            }

//打开时间翻页特效时，这里不执行显示时间的动作。
#ifndef PDA_WEATHER_TIME_SE
            if (GUIRES_GetImgWidthHeight(&icon_width, &icon_height, s_Number_Image_index[0][0], win_id))
            {
                if (bg_width > icon_width * 4 && bg_height > icon_height)
                {
                    
                    MMI_IMAGE_ID_T   cur_time_img[5] = {0};
                    uint16           cur_text_colour = 0;
                    cur_text_colour = ParserNvValue(MMIWIDGET_WEATHER_BG_TEXTCOLOUR_SET_NV_ITEM_TEXTCOLOUR, &s_cur_bg_textcolour_set_info);
                    
                    cur_time_img[0] = s_Number_Image_index[sys_time.hour / 10][cur_text_colour];
                    cur_time_img[1] = s_Number_Image_index[sys_time.hour % 10][cur_text_colour];
                    cur_time_img[2] = s_Number_Image_index[sys_time.min / 10][cur_text_colour];
                    cur_time_img[3] = s_Number_Image_index[sys_time.min % 10][cur_text_colour];
                    cur_time_img[4] = s_Number_Image_index[10][cur_text_colour];
                    
                    start_point.y = bg_point.y + MMIWIDGET_WEATHER_TIME_IMG_START_Y;
                    
                    start_point.x = bg_point.x + MMIWIDGET_WEATHER_TIME_IMG_HOUR_START_X;
                    GUIRES_DisplayImg(&start_point, PNULL, PNULL, win_id, cur_time_img[0], &lcd_dev_info);
                    
                    start_point.x += (MMIWIDGET_WEATHER_TIME_IMG_WIDTH + MMIWIDGET_WEATHER_TIME_IMG_SPACE);
                    GUIRES_DisplayImg(&start_point, PNULL, PNULL, win_id, cur_time_img[1], &lcd_dev_info);
                    
                    if (0 == need_dis_bg)
                    {
                        GUI_POINT_T colon_point    = {0};
                        GUI_RECT_T  colon_dis_rect = {0};
                        
                        colon_dis_rect.left   = start_point.x + MMIWIDGET_WEATHER_TIME_IMG_WIDTH + MMIWIDGET_WEATHER_TIME_IMG_SPACE;
                        colon_dis_rect.top    = start_point.y;
                        colon_dis_rect.right  = bg_point.x + MMIWIDGET_WEATHER_TIME_IMG_MINUTE_START_X;
                        colon_dis_rect.bottom = colon_dis_rect.top + MMIWIDGET_WEATHER_TIME_IMG_HEIGHT;
                        colon_dis_rect = GUI_GetCenterRectExt(colon_dis_rect, MMIWIDGET_WEATHER_TIME_COLON_IMG_WIDTH, MMIWIDGET_WEATHER_TIME_COLON_IMG_HEIGHT);
                        colon_point.x = colon_dis_rect.left;
                        colon_point.y = colon_dis_rect.top;
                        GUIRES_DisplayImg(&colon_point, PNULL, PNULL, win_id, cur_time_img[4], &lcd_dev_info);
                    }
                    
                    start_point.x = bg_point.x + MMIWIDGET_WEATHER_TIME_IMG_MINUTE_START_X;
                    GUIRES_DisplayImg(&start_point, PNULL, PNULL, win_id, cur_time_img[2], &lcd_dev_info);
                    
                    start_point.x += (MMIWIDGET_WEATHER_TIME_IMG_WIDTH + MMIWIDGET_WEATHER_TIME_IMG_SPACE);
                    GUIRES_DisplayImg(&start_point, PNULL, PNULL, win_id, cur_time_img[3], &lcd_dev_info);
                    
                }
                else
                {
                    //SCI_TRACE_LOW:"Please check time number img size"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_WEATHER_2024_112_2_18_3_8_48_247,(uint8*)"");
                }
            }
#else
            DisplayCurrentTime();
#endif //PDA_WEATHER_TIME_SE

            {
                //MMIWEATHER_WEATHER_T*   weather_info = MMIWEATHER_GetCurCityWeatherInfo();
                icon_id = MMIWEATHER_GetConditionImgId(MMIWEATHER_GetCurCondition(), WEATHER_ICON_WIDGET);
            }
            
            start_point.x = bg_point.x + MMIWIDGET_WEATHER_IMG_START_X;
            start_point.y = bg_point.y + MMIWIDGET_WEATHER_IMG_START_Y;
            
            GUIRES_DisplayImg(&start_point, PNULL, PNULL, win_id, icon_id, &lcd_dev_info);
            
            {
                GUISTR_STYLE_T text_style = {0};
                MMI_STRING_T   tempStr = {0};
                GUISTR_STATE_T text_state = GUISTR_STATE_ALIGN|GUISTR_STATE_EFFECT|GUISTR_STATE_EFFECT|GUISTR_STATE_ELLIPSIS;
                GUI_RECT_T     text_rect  = {0};
                uint16         text_height = 0;
                uint16         text_width  = 0;
                
                text_style.effect = FONT_EFFECT_REVERSE_EDGE;
                text_style.angle = ANGLE_0;
                text_style.align = ALIGN_LVMIDDLE;
                text_style.font = MMIWIDGET_WEATHER_CITY_TEXT_FONT;
                text_style.font_color = MMIWIDGET_WEATHER_TEXT_COLOR;
                
                text_height = GUIFONT_GetHeight(text_style.font);
                text_width  = MMIWIDGET_WEATHER_TEXT_WIDTH;
                text_rect.left   = bg_point.x + MMIWIDGET_WEATHER_LEFT_TEXT_START_X;
                text_rect.top    = bg_point.y + MMIWIDGET_WEATHER_TEXT_START_Y;
                text_rect.right  = text_rect.left + text_width;
                text_rect.bottom = text_rect.top + text_height;
                GUISTR_DrawTextToLCDInRect(&lcd_dev_info, &text_rect, &text_rect, &s_cur_city_str,	&text_style, text_state, GUISTR_TEXT_DIR_AUTO);
                
                text_style.font = MMIWIDGET_WEATHER_TEXT_FONT;
                
                text_rect.top    = text_rect.bottom + MMIWIDGET_WEATHER_TEXT_SPACE;
                text_rect.bottom = text_rect.top + text_height;
                
                SCI_MEMSET(&tempStr, 0x00, sizeof(MMI_STRING_T));
                GetDisplayText(&tempStr, MMIWIDGET_WEATHER_DISPALY_TEXT_TYPE_DATE);
                GUISTR_DrawTextToLCDInRect(&lcd_dev_info, &text_rect, &text_rect, &tempStr,	&text_style, text_state, GUISTR_TEXT_DIR_AUTO);
                
                text_style.align = ALIGN_RVMIDDLE;
                
                text_rect.right = bg_rect.right - MMIWIDGET_WEATHER_LEFT_TEXT_START_X;
                text_rect.left  = text_rect.right - text_width;
                GUISTR_DrawTextToLCDInRect(&lcd_dev_info, &text_rect, &text_rect, &s_cur_temprange_str,	&text_style, text_state, GUISTR_TEXT_DIR_AUTO);
                
                text_rect.top    = bg_point.y + MMIWIDGET_WEATHER_TEXT_START_Y;
                text_rect.bottom = text_rect.top + text_height;
                GUISTR_DrawTextToLCDInRect(&lcd_dev_info, &text_rect, &text_rect, &s_cur_condition_str,	&text_style, text_state, GUISTR_TEXT_DIR_AUTO);
            }
        }    
}
#if defined TOUCH_PANEL_SUPPORT
/*****************************************************************************/
//    Description : MSG_WIDGET_TP_PRESS_UP消息处理
//    Global resource dependence : 
//    Author:   xiaoming.ren
//    Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  MMIWIDGET_Weather_HandleTpPressUp(MMI_WIN_ID_T  win_id,   DPARAM  param )
{
    MMI_RESULT_E recode   = MMI_RESULT_TRUE;
    GUI_POINT_T point     = {0};    
    uint16      cur_index = 0;
    
    point.x = MMK_GET_TP_X(param);
    point.y = MMK_GET_TP_Y(param);

    if (GUI_PointIsInRect(point, s_click_rect[MMIWIDGET_WEATHER_CLICK_RECT_LEFT_TOP]))
    {
        if (MMIWIDGET_WEATHER_CLICK_RECT_LEFT_TOP == s_tp_hit_type)
        {
            cur_index = ParserNvValue(MMIWIDGET_WEATHER_HIT_SET_NV_ITEM_LEFT_TOP, &s_cur_hit_set_info);
            s_func_list_table[cur_index].pfun();
        }
    }
    else if (GUI_PointIsInRect(point, s_click_rect[MMIWIDGET_WEATHER_CLICK_RECT_RIGHT_TOP]))
    {
        if (MMIWIDGET_WEATHER_CLICK_RECT_RIGHT_TOP == s_tp_hit_type)
        {
            cur_index = ParserNvValue(MMIWIDGET_WEATHER_HIT_SET_NV_ITEM_RIGHT_TOP, &s_cur_hit_set_info);
            s_func_list_table[cur_index].pfun();
        }
    }
    else if (GUI_PointIsInRect(point, s_click_rect[MMIWIDGET_WEATHER_CLICK_RECT_LEFT_BOTTOM]))
    {
        if (MMIWIDGET_WEATHER_CLICK_RECT_LEFT_BOTTOM == s_tp_hit_type)
        {
#ifdef WIN32
#ifdef PDA_WEATHER_TIME_SE
            SCI_TIME_T sys_time = {0};
            TM_GetSysTime(&sys_time);
            sys_time.min += 1;
            sys_time.hour += 1;
            PlayTimeUpdateSe(sys_time);
#endif
#endif
            cur_index = ParserNvValue(MMIWIDGET_WEATHER_HIT_SET_NV_ITEM_LEFT_BOTTOM, &s_cur_hit_set_info);
            s_func_list_table[cur_index].pfun();
                   
        }
    }
    else if (GUI_PointIsInRect(point, s_click_rect[MMIWIDGET_WEATHER_CLICK_RECT_MID_BOTTOM]))
    {
        if (MMIWIDGET_WEATHER_CLICK_RECT_MID_BOTTOM == s_tp_hit_type)
        {
            MMIWEATHER_OpenMainWin();
        }
    }
    else if (GUI_PointIsInRect(point, s_click_rect[MMIWIDGET_WEATHER_CLICK_RECT_RIGHT_BOTTOM]))
    {
        if (MMIWIDGET_WEATHER_CLICK_RECT_RIGHT_BOTTOM == s_tp_hit_type)
        {
#ifdef WIN32
#ifdef PDA_WEATHER_TIME_SE
            SCI_TIME_T sys_time = {0};
            TM_GetSysTime(&sys_time);
            sys_time.min += 1;
            PlayTimeUpdateSe(sys_time);
#endif
#endif
            cur_index = ParserNvValue(MMIWIDGET_WEATHER_HIT_SET_NV_ITEM_RIGHT_BOTTOM, &s_cur_hit_set_info);
            s_func_list_table[cur_index].pfun();
        }
    }
	else
	{
		recode   = MMI_RESULT_FALSE;
	}

    ResetWidgetWeather();
    
    return recode;
}

/*****************************************************************************/
//    Description : MSG_WIDGET_TP_PRESS_DOWN消息处理
//    Global resource dependence : 
//    Author:   xiaoming.ren
//    Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  MMIWIDGET_Weather_HandleTpPressDown(MMI_WIN_ID_T  win_id,   DPARAM  param )
{
    MMI_RESULT_E recode   = MMI_RESULT_TRUE;
    GUI_POINT_T point     = {0};
    MMI_IMAGE_ID_T img_id = 0;

    point.x = MMK_GET_TP_X(param);
    point.y = MMK_GET_TP_Y(param);   
    
    if (GUI_PointIsInRect(point, s_click_rect[MMIWIDGET_WEATHER_CLICK_RECT_LEFT_TOP]))
    {
        s_tp_hit_type             = MMIWIDGET_WEATHER_CLICK_RECT_LEFT_TOP;        
    }
    else if (GUI_PointIsInRect(point, s_click_rect[MMIWIDGET_WEATHER_CLICK_RECT_RIGHT_TOP]))
    {
        s_tp_hit_type             = MMIWIDGET_WEATHER_CLICK_RECT_RIGHT_TOP;
    }
    else if (GUI_PointIsInRect(point, s_click_rect[MMIWIDGET_WEATHER_CLICK_RECT_LEFT_BOTTOM]))
    {
        s_tp_hit_type             = MMIWIDGET_WEATHER_CLICK_RECT_LEFT_BOTTOM;
    }
    else if (GUI_PointIsInRect(point, s_click_rect[MMIWIDGET_WEATHER_CLICK_RECT_MID_BOTTOM]))
    {
        s_tp_hit_type             = MMIWIDGET_WEATHER_CLICK_RECT_MID_BOTTOM;
    }
    else if (GUI_PointIsInRect(point, s_click_rect[MMIWIDGET_WEATHER_CLICK_RECT_RIGHT_BOTTOM]))
    {
        s_tp_hit_type             = MMIWIDGET_WEATHER_CLICK_RECT_RIGHT_BOTTOM;
    }
	else
	{
		ResetWidgetWeather();
		recode = MMI_RESULT_FALSE;
	}

    if (MMIWIDGET_WEATHER_CLICK_RECT_INVALID != s_tp_hit_type)
    {
        img_id = (s_tp_hit_type < MMIWIDGET_WEATHER_CLICK_RECT_LEFT_BOTTOM) ? IMAGE_WIDGET_WEATHER_HIGHLIGHT_L : IMAGE_WIDGET_WEATHER_HIGHLIGHT_S;
        DisplayHighlightImg(win_id, img_id, &s_click_rect[s_tp_hit_type]);
    }

    if (MMI_RESULT_TRUE == recode)
    {
        //这里强制让widget响应tp down事件，使得可以执行long move事件
        recode = MMI_RESULT_FALSE;
    }
    
    return recode;
}
#endif
/*****************************************************************************/
//    Description : 显示高亮图片
//    Global resource dependence : 
//    Author:   xiaoming.ren
//    Note:
/*****************************************************************************/
LOCAL void DisplayHighlightImg(MMI_WIN_ID_T  win_id, MMI_IMAGE_ID_T img_id, GUI_RECT_T  *border_rect_ptr)
{
    uint16      icon_width  = 0;
    uint16      icon_height = 0;
    uint16      rect_width  = 0;
    uint16      rect_height = 0;
    GUI_POINT_T start_point = {0};
    GUI_RECT_T  cur_rect    = {0};
    GUI_LCD_DEV_INFO  lcd_dev_info    = {0};
    
    if(PNULL == border_rect_ptr)
    {
        return;
    }
    
    MMK_GetWinLcdDevInfo(win_id,&lcd_dev_info);
    (void)GUIRES_GetImgWidthHeight(&icon_width, &icon_height, img_id, win_id);

    cur_rect = *border_rect_ptr;

    rect_width  = border_rect_ptr->right - border_rect_ptr->left;
    rect_height = border_rect_ptr->bottom - border_rect_ptr->top;
    if (icon_width >= rect_width && icon_height >= rect_height)
    {
#ifndef PDA_WEATHER_SUPPORT_LOW_MEMORY
        CreatHighlightLayer(win_id);
#endif

        start_point.x = border_rect_ptr->left - (icon_width - rect_width) / 2;
        if ((s_tp_hit_type < MMIWIDGET_WEATHER_CLICK_RECT_LEFT_BOTTOM)
            || (s_cur_focus_index < MMIWIDGET_WEATHER_CLICK_RECT_LEFT_BOTTOM))
        {
            start_point.y = border_rect_ptr->top - (rect_height - rect_height) / 2 - 10;
        }
        else
        {
            start_point.y = border_rect_ptr->top - (rect_height - rect_height) / 2;
        }

        if (GUIRES_DisplayImg(&start_point, PNULL, PNULL, win_id, img_id, &lcd_dev_info))
        {
#ifndef PDA_WEATHER_SUPPORT_LOW_MEMORY
            UILAYER_APPEND_BLT_T append_layer = {0};
            //add into layer queue
            append_layer.lcd_dev_info = s_highlight_layer_info;
            append_layer.layer_level = UILAYER_LEVEL_NORMAL;
            
            UILAYER_AppendBltLayer(&append_layer);
#endif
        }
        else
        {
            ResetWidgetWeather();
        }
    }
}

/*****************************************************************************/
//    Description : 获取显示类型对应的字符串
//    Global resource dependence : 
//    Author:   xiaoming.ren
//    Note:
/*****************************************************************************/
LOCAL void GetDisplayText(MMI_STRING_T* Text_Ptr, MMIWIDGET_WEATHER_DISPALY_TEXT_TYPE_E dis_type)
{
    MMIWEATHER_SETTING_T* setting_ptr = MMIWEATHER_GetSettingInfo();

    if(PNULL == Text_Ptr)
    {
        return;
    }

    switch(dis_type)
    {
    case MMIWIDGET_WEATHER_DISPALY_TEXT_TYPE_CITYNAME:
        {
#ifdef BAR_PHONE_WIDGET
            if(cur_webkey_condition == NEED_ADD_CITY)
            {
                MMI_GetLabelTextByLang(TXT_WEATHER_NOCITY, &s_cur_city_str);
            }  
            else 
            {
#endif   
            if(setting_ptr->city_num > 0)
            {
                Text_Ptr->wstr_ptr = MMIWEATHER_GetCurCityName();
                Text_Ptr->wstr_len = MMIAPICOM_Wstrlen(Text_Ptr->wstr_ptr);
                SCI_MEMSET(s_cur_city_u16, 0x00, sizeof(s_cur_city_u16));
                MMIAPICOM_Wstrncpy(s_cur_city_u16, Text_Ptr->wstr_ptr, Text_Ptr->wstr_len);
                s_cur_city_str.wstr_ptr = s_cur_city_u16;
                s_cur_city_str.wstr_len = Text_Ptr->wstr_len;
            }
#ifdef BAR_PHONE_WIDGET
            }
#endif
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
#ifdef BAR_PHONE_WIDGET
            if(cur_webkey_condition == NEED_ADD_CITY)
            {
                break;
            }    
#endif      
            text_id = MMIWEATHER_GetConditionTxtId(MMIWEATHER_GetCurCondition());
            MMI_GetLabelTextByLang(text_id, Text_Ptr);
            SCI_MEMSET(s_cur_condition_u16, 0x00, sizeof(s_cur_condition_u16));
            MMIAPICOM_Wstrncpy(s_cur_condition_u16, Text_Ptr->wstr_ptr, Text_Ptr->wstr_len);
            s_cur_condition_str.wstr_ptr = s_cur_condition_u16;
            s_cur_condition_str.wstr_len = Text_Ptr->wstr_len;
        }
        break;
    case MMIWIDGET_WEATHER_DISPALY_TEXT_TYPE_TEMPRANGE:
        {
            Text_Ptr->wstr_ptr = MMIWEATHER_GetCurTempRangeWstr(MMIWEATHER_GetCurCityIndex());
            Text_Ptr->wstr_len = MMIAPICOM_Wstrlen(Text_Ptr->wstr_ptr);
            SCI_MEMSET(s_cur_temprange_u16, 0x00, sizeof(s_cur_temprange_u16));
            MMIAPICOM_Wstrncpy(s_cur_temprange_u16, Text_Ptr->wstr_ptr, Text_Ptr->wstr_len);
            s_cur_temprange_str.wstr_ptr = s_cur_temprange_u16;
            s_cur_temprange_str.wstr_len = Text_Ptr->wstr_len;
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
    default:
        break;
    }
}

/*****************************************************************************/
//    Description : 打开浏览器
//    Global resource dependence : 
//    Author:   xiaoming.ren
//    Note:
/*****************************************************************************/
LOCAL void OpenBrowserMainWin(void)
{
    MMIBROWSER_ENTRY_PARAM entry_param = {0};
    
    entry_param.type = MMIBROWSER_ACCESS_MAINMENU;
    entry_param.dual_sys = MN_DUAL_SYS_MAX;
    entry_param.url_ptr = PNULL;
    entry_param.user_agent_ptr = PNULL;
    
    MMIAPIBROWSER_Entry(&entry_param);
}

/*****************************************************************************/
//    Description : 切换城市
//    Global resource dependence : 
//    Author:   xiaoming.ren
//    Note:
/*****************************************************************************/
LOCAL void ChangeCurCity(void)
{
    BOOLEAN is_need_respond = TRUE;
    uint8   cur_city_num    = 0;
    uint32  dis_time        = MMI_1SECONDS;

    cur_city_num = MMIWEATHER_GetCityNum();
    is_need_respond = cur_city_num > 1 ? TRUE : FALSE;
    if (is_need_respond)
    {
        MMI_STRING_T dis_text = {0};
        s_Need_dis_prompt_str_type   = MMIWIDGET_WEATHER_DIS_PROMPT_STR_TYPE_SWITCH_CITY;
        GetDisplayText(&dis_text, MMIWIDGET_WEATHER_DISPALY_TEXT_TYPE_PROMPT);
        MMIPUB_DisplayTipsPtrEx(&g_weather_widget.win_id, &dis_text, MMIPUB_TIPS_DIS_BOTTOM, &dis_time);
        MMIWEATHER_SendMsgToApp(MSG_WEATHER_WGT_SWITCH_CITY_IND);
        MMIPUB_ClearTips();
    }
    else
    {
        if (0 == cur_city_num)
        {
            MMIPUB_DisplayTipsIDEx(&g_weather_widget.win_id, TXT_WIDGET_WEATHER_PLEASE_ADD_CITY, MMIPUB_TIPS_DIS_BOTTOM, &dis_time);
        }
        else
        {
            MMIPUB_DisplayTipsIDEx(&g_weather_widget.win_id, TXT_WIDGET_WEATHER_ONE_CITY_NO_SWITCH, MMIPUB_TIPS_DIS_BOTTOM, &dis_time);
        }
    }
}

/*****************************************************************************/
//    Description : 更新天气
//    Global resource dependence : 
//    Author:   xiaoming.ren
//    Note:
/*****************************************************************************/
LOCAL void UpdateWeather(void)
{
    BOOLEAN is_need_respond = TRUE;
    is_need_respond = MMIWEATHER_GetCityNum() > 0 ? TRUE : FALSE;

    if (is_need_respond)
    {
        MMI_STRING_T dis_text = {0};
        s_Need_dis_prompt_str_type   = MMIWIDGET_WEATHER_DIS_PROMPT_STR_TYPE_UPDATEING;
        GetDisplayText(&dis_text, MMIWIDGET_WEATHER_DISPALY_TEXT_TYPE_PROMPT);
         
        MMIPUB_DisplayTipsPtrEx(&g_weather_widget.win_id, &dis_text, MMIPUB_TIPS_DIS_BOTTOM, PNULL);
        MMIWEATHER_SendMsgToApp(MSG_WEATHER_WGT_UPDATE_IND);
    }
    else
    {
        uint32 dis_time = MMI_1SECONDS;
         
        MMIPUB_DisplayTipsIDEx(&g_weather_widget.win_id, TXT_WIDGET_WEATHER_PLEASE_ADD_CITY, MMIPUB_TIPS_DIS_BOTTOM, &dis_time);
    }
}

/*****************************************************************************/
//    Description : 创建高亮Layer
//    Global resource dependence : 
//    Author:   xiaoming.ren
//    Note:
/*****************************************************************************/
LOCAL BOOLEAN CreatHighlightLayer(MMI_WIN_ID_T     win_id)
{
    UILAYER_CREATE_T     create_info = {0};
    UILAYER_RESULT_E     result      = UILAYER_RESULT_SUCCESS;
    GUI_RECT_T           win_rect    = {0};
#ifdef FPS_ACCELERATE
    if (UILAYER_HANDLE_MULTI != UILAYER_GetHandleType((GUI_LCD_DEV_INFO *)&s_highlight_layer_info))
#else
    if (UILAYER_IsEmptyLayerHandle(&s_highlight_layer_info))
#endif
    {
        MMK_GetWinRect(win_id, &win_rect);
        
        //creat text layer
        create_info.lcd_id          = GUI_MAIN_LCD_ID;
        create_info.owner_handle    = win_id;
        create_info.offset_x        = win_rect.left;
        create_info.offset_y        = win_rect.top;
        create_info.width           = win_rect.right - win_rect.left + 1;
        create_info.height          = win_rect.bottom - win_rect.top + 1;
        create_info.is_bg_layer     = FALSE;
        create_info.is_static_layer = FALSE;
        
        result = UILAYER_CreateLayer(&create_info, &s_highlight_layer_info);
        
        if (UILAYER_RESULT_ERROR == result)
        {
            return FALSE;
        }
    }
    else
    {
        UILAYER_Clear(&s_highlight_layer_info);
    }
    return TRUE;

}

/*****************************************************************************/
//    Description : 删除高亮Layer
//    Global resource dependence : 
//    Author:   xiaoming.ren
//    Note:
/*****************************************************************************/
LOCAL BOOLEAN DeleteHighlightLayer(void)
{
#ifdef FPS_ACCELERATE
    if (UILAYER_HANDLE_MULTI == UILAYER_GetHandleType((GUI_LCD_DEV_INFO *)&s_highlight_layer_info))
#else
    if (!UILAYER_IsEmptyLayerHandle(&s_highlight_layer_info))
#endif
    {    
        UILAYER_RELEASELAYER(&s_highlight_layer_info);   /*lint !e506 !e774*/
//        UILAYER_ReleaseLayer(&s_highlight_layer_info);
//        s_highlight_layer_info.block_id = UILAYER_NULL_HANDLE;
    }

    return TRUE;
}

/*****************************************************************************/
//    Description : 快捷方式设置界面事件处理
//    Global resource dependence : 
//    Author:   xiaoming.ren
//    Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleWeatherWidgetHitFuncSetWinMsg(MMI_WIN_ID_T win_id,MMI_MESSAGE_ID_E msg_id,DPARAM   param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;

        switch(msg_id)
        {
        case MSG_OPEN_WINDOW:
            {
                if (s_is_need_read_hit_set_info)
                {
                    ReadHitSetInfo(&s_cur_hit_set_info);
                }
                WeatherSetWin_InitCtrlRect(MMIWIDGET_WEATHER_SET_WIN_TYPE_HIT_FUNC);
                WeatherSetWin_SetCtrlParam(MMIWIDGET_WEATHER_SET_WIN_TYPE_HIT_FUNC);
                MMK_SetAtvCtrl(win_id, s_sel_hit_ctrl_id_table[0][MMIWIDGET_WEATHER_CONTROL_TYPE_DROPDOWN]);
            }
            break;

        case MSG_LCD_SWITCH:
            {
                WeatherSetWin_InitCtrlRect(MMIWIDGET_WEATHER_SET_WIN_TYPE_HIT_FUNC);
            }
            break;
            
        case MSG_FULL_PAINT:
            
            break;
            
#ifdef MMI_PDA_SUPPORT
        case MSG_APP_CANCEL:
        case MSG_CTL_CANCEL:
#endif
        case MSG_APP_OK:
        case MSG_CTL_OK:
            {
                uint32 cur_ctrl_sel_info = 0;
                uint32 i                 = 0;

                for (i = 0; i < HIT_CTRL_ID_TABLE_LEN; i++ )
                {
                    MakeNvValue(i + 1, GUIDROPDOWNLIST_GetCurItemIndex(s_sel_hit_ctrl_id_table[i][MMIWIDGET_WEATHER_CONTROL_TYPE_DROPDOWN]), &cur_ctrl_sel_info);
                }
                
                if (s_cur_hit_set_info != cur_ctrl_sel_info)
                {
                    s_cur_hit_set_info = cur_ctrl_sel_info;
                    WriteHitSetInfo();
                    MMIPUB_DisplayTipsIDEx(&win_id, TXT_SET_SUCCESS, MMIPUB_TIPS_DIS_BOTTOM, PNULL);
                    break;
                }
                MMK_CloseWin(win_id);
            }
            break;
        case MSG_TIPSWIN_CLOSED:
            MMK_CloseWin(win_id);
            break;
#ifndef MMI_PDA_SUPPORT
        case MSG_APP_CANCEL:
        case MSG_CTL_CANCEL:
            MMK_CloseWin(win_id);
            break;
#endif
            
        default:
            result = MMI_RESULT_FALSE;
            break;
        }
        
        return (result);
}

/*****************************************************************************/
//    Description : 背景&时间字体颜色设置界面事件处理
//    Global resource dependence : 
//    Author:   xiaoming.ren
//    Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleWeatherWidgetBGTextColourSetWinMsg(MMI_WIN_ID_T win_id,MMI_MESSAGE_ID_E msg_id,DPARAM   param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;

        switch(msg_id)
        {
        case MSG_OPEN_WINDOW:
            {
                if (s_is_need_read_bg_textcolour_set_info)
                {
                    ReadBgAndTextColorSetInfo(&s_cur_bg_textcolour_set_info);
                }
                WeatherSetWin_InitCtrlRect(MMIWIDGET_WEATHER_SET_WIN_TYPE_BG_TEXTCOLOUR);
                WeatherSetWin_SetCtrlParam(MMIWIDGET_WEATHER_SET_WIN_TYPE_BG_TEXTCOLOUR);
                MMK_SetAtvCtrl(win_id, s_sel_bg_textcolour_ctrl_id_table[0][MMIWIDGET_WEATHER_CONTROL_TYPE_DROPDOWN]);
            }
            break;

        case MSG_LCD_SWITCH:
            {
                WeatherSetWin_InitCtrlRect(MMIWIDGET_WEATHER_SET_WIN_TYPE_BG_TEXTCOLOUR);
            }
            break;
            
        case MSG_FULL_PAINT:
            
            break;
            
#ifdef MMI_PDA_SUPPORT
        case MSG_APP_CANCEL:
        case MSG_CTL_CANCEL:
#endif
        case MSG_APP_OK:
        case MSG_CTL_OK:
            {
                uint32 cur_ctrl_sel_info = 0;
                uint32 i                 = 0;

                for (i = 0; i < BG_TEXTCOLOUR_CTRL_ID_TABLE_LEN; i++ )
                {
                    MakeNvValue(i + 1, GUISETLIST_GetCurIndex(s_sel_bg_textcolour_ctrl_id_table[i][MMIWIDGET_WEATHER_CONTROL_TYPE_DROPDOWN]), &cur_ctrl_sel_info);
                }
                
                if (s_cur_bg_textcolour_set_info != cur_ctrl_sel_info)
                {
                    s_cur_bg_textcolour_set_info = cur_ctrl_sel_info;
                    WriteBgAndTextColorSetInfo();
                    MMIPUB_DisplayTipsIDEx(&win_id, TXT_SET_SUCCESS, MMIPUB_TIPS_DIS_BOTTOM, PNULL);
                    break;
                }
                MMK_CloseWin(win_id);
            }
            break;
        case MSG_TIPSWIN_CLOSED:
            MMK_CloseWin(win_id);
            break;
            
#ifndef MMI_PDA_SUPPORT
        case MSG_APP_CANCEL:
        case MSG_CTL_CANCEL:
            MMK_CloseWin(win_id);
            break;
#endif
            
        default:
            result = MMI_RESULT_FALSE;
            break;
        }
        
        return (result);
}

/*****************************************************************************/
//    Description : 初始化设置界面控件区域
//    Global resource dependence : 
//    Author:   xiaoming.ren
//    Note:
/*****************************************************************************/
LOCAL void WeatherSetWin_InitCtrlRect(MMIWIDGET_WEATHER_SET_WIN_TYPE_E win_type)
{
    uint32 i = 0;
    uint32 j = 0;
    uint16 lcd_width  = 0;
    uint16 lcd_height = 0;
    uint32 title_height = 0;
    
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);
    title_height = MMITHEME_GetWinTitleHeight();

    switch(win_type)
    {
    case MMIWIDGET_WEATHER_SET_WIN_TYPE_HIT_FUNC:
        {
            for (i = 0; i < HIT_CTRL_ID_TABLE_LEN; i++)
            {
                for (j = 0; j < MMIWIDGET_WEATHER_CONTROL_TYPE_DROPDOWN; j++)
                {
                    s_sel_hit_rect[i][j].left = ((j == 0) ? 5 : MMIWIDGET_WEATHER_ITEM_LEFT);
                    s_sel_hit_rect[i][j].right = lcd_width - MMIWIDGET_WEATHER_ITEM_LEFT;
                    if (j > 0)
                    {
                        s_sel_hit_rect[i][j].top = s_sel_hit_rect[i][j - 1].bottom;
                    }
                    else
                    {
                        s_sel_hit_rect[i][j].top = title_height + MMIWIDGET_WEATHER_ITEM_EX + i * (MMIWIDGET_WEATHER_ITEM_HEIGHT * 2 + MMIWIDGET_WEATHER_ITEM_EX);
                    }
                    s_sel_hit_rect[i][j].bottom = s_sel_hit_rect[i][j].top + MMIWIDGET_WEATHER_ITEM_HEIGHT;
                }
            }
            
            for (i = 0; i < HIT_CTRL_ID_TABLE_LEN; i++)
            {
                GUILABEL_SetRect(s_sel_hit_ctrl_id_table[i][MMIWIDGET_WEATHER_CONTROL_TYPE_LABLE], &s_sel_hit_rect[i][0], FALSE);
                GUIDROPDOWNLIST_SetRect(s_sel_hit_ctrl_id_table[i][MMIWIDGET_WEATHER_CONTROL_TYPE_DROPDOWN], &s_sel_hit_rect[i][1]);
            }
        }
    	break;
    case MMIWIDGET_WEATHER_SET_WIN_TYPE_BG_TEXTCOLOUR:
        {
            for (i = 0; i < BG_TEXTCOLOUR_CTRL_ID_TABLE_LEN; i++)
            {
                for (j = 0; j < MMIWIDGET_WEATHER_CONTROL_TYPE_DROPDOWN; j++)
                {
                    s_sel_bg_textcolour_rect[i][j].left = ((j == 0) ? 5 : MMIWIDGET_WEATHER_ITEM_LEFT);
                    s_sel_bg_textcolour_rect[i][j].right = lcd_width - MMIWIDGET_WEATHER_ITEM_LEFT;
                    if (j > 0)
                    {
                        s_sel_bg_textcolour_rect[i][j].top = s_sel_bg_textcolour_rect[i][j - 1].bottom;
                    }
                    else
                    {
                        s_sel_bg_textcolour_rect[i][j].top = title_height + MMIWIDGET_WEATHER_ITEM_EX + i * (MMIWIDGET_WEATHER_ITEM_HEIGHT * 2 + MMIWIDGET_WEATHER_ITEM_EX);
                    }
                    s_sel_bg_textcolour_rect[i][j].bottom = s_sel_bg_textcolour_rect[i][j].top + MMIWIDGET_WEATHER_ITEM_HEIGHT;
                }
            }
            
            for (i = 0; i < BG_TEXTCOLOUR_CTRL_ID_TABLE_LEN; i++)
            {
                GUILABEL_SetRect(s_sel_bg_textcolour_ctrl_id_table[i][MMIWIDGET_WEATHER_CONTROL_TYPE_LABLE], &s_sel_bg_textcolour_rect[i][0], FALSE);
            }
        }
        break;
    default:
        break;
    }
}

/*****************************************************************************/
//    Description : 初始化设置界面控件参数
//    Global resource dependence : 
//    Author:   xiaoming.ren
//    Note:
/*****************************************************************************/
LOCAL void WeatherSetWin_SetCtrlParam(MMIWIDGET_WEATHER_SET_WIN_TYPE_E win_type)
{
    uint32                  i = 0;
    switch(win_type)
    {
    case MMIWIDGET_WEATHER_SET_WIN_TYPE_HIT_FUNC:
        {
            uint32                  len     = 0;
            MMI_STRING_T            itemStr = {0};
            MMI_STRING_T            ShortcutStr = {0};


            MMI_STRING_T            ShortcutStr_Temp = {0};
            MMI_STRING_T            dropdownlist[FUNC_LIST_TABLE_LEN] = {0};
			char                    shortcut_str_u8[5] = {0};
            wchar                   shortcut_str_u16[10] = {0};
			wchar                   dropdownlist_item_u16[FUNC_LIST_TABLE_LEN][DROPDOWNLIST_STRING_MAX_NUM] = {0};
            
            for(i=0; i < FUNC_LIST_TABLE_LEN; i ++)
            {
                MMI_GetLabelTextByLang(s_func_list_table[i].text_id, &itemStr);
				dropdownlist[i].wstr_ptr = dropdownlist_item_u16[i];
                MMI_WSTRNCPY(dropdownlist[i].wstr_ptr, DROPDOWNLIST_STRING_MAX_NUM, itemStr.wstr_ptr, itemStr.wstr_len, itemStr.wstr_len);
                dropdownlist[i].wstr_len = itemStr.wstr_len;
            }
            
            
            for (i = 0; i < HIT_CTRL_ID_TABLE_LEN; i++)
            {
                MMI_GetLabelTextByLang(TXT_WIDGET_ADD_SHORT, &ShortcutStr);
                
                SCI_MEMSET(shortcut_str_u8,0, sizeof(shortcut_str_u8));
                sprintf(shortcut_str_u8, " %d:", i + 1);
                
                SCI_MEMSET(shortcut_str_u16,0, sizeof(shortcut_str_u16));
                (void)MMI_STRNTOWSTR(shortcut_str_u16, (size_t)sizeof(shortcut_str_u16), (uint8*)shortcut_str_u8, (size_t)SCI_STRLEN(shortcut_str_u8), (size_t)SCI_STRLEN(shortcut_str_u8));
                
                len = (MMIAPICOM_Wstrlen(shortcut_str_u16) + ShortcutStr.wstr_len + 2) * sizeof(wchar);
                ShortcutStr_Temp.wstr_ptr = (wchar*)SCI_ALLOC_APP(len);
                if (PNULL != ShortcutStr_Temp.wstr_ptr)
                {
                    SCI_MEMSET(ShortcutStr_Temp.wstr_ptr,0,len);
                    (void)MMIAPICOM_Wstrncat(ShortcutStr_Temp.wstr_ptr, ShortcutStr.wstr_ptr, ShortcutStr.wstr_len);
                }
                
                (void)MMIAPICOM_Wstrncat(ShortcutStr_Temp.wstr_ptr, shortcut_str_u16, MMIAPICOM_Wstrlen(shortcut_str_u16));
                
                ShortcutStr_Temp.wstr_len = MMIAPICOM_Wstrlen(ShortcutStr_Temp.wstr_ptr);
                
                GUILABEL_SetText(s_sel_hit_ctrl_id_table[i][MMIWIDGET_WEATHER_CONTROL_TYPE_LABLE], &ShortcutStr_Temp, FALSE);
                GUIDROPDOWNLIST_AppendItemArray(s_sel_hit_ctrl_id_table[i][MMIWIDGET_WEATHER_CONTROL_TYPE_DROPDOWN], dropdownlist, ARR_SIZE(dropdownlist));
                
                GUIDROPDOWNLIST_SetCurItemIndex(s_sel_hit_ctrl_id_table[i][MMIWIDGET_WEATHER_CONTROL_TYPE_DROPDOWN], ParserNvValue((i + 1), &s_cur_hit_set_info));
                GUIFORM_SetStyle(s_sel_hit_ctrl_id_table[i][MMIWIDGET_WEATHER_CONTROL_TYPE_FORM], GUIFORM_STYLE_UNIT);
                
                SCI_FREE(ShortcutStr_Temp.wstr_ptr);
            }
        }
        break;

    case MMIWIDGET_WEATHER_SET_WIN_TYPE_BG_TEXTCOLOUR:
        {
            MMI_TEXT_ID_T          bg_type_text_id[] = {TXT_CLOSE, TXT_OPEN};
            MMI_TEXT_ID_T          text_colour_text_id[] = {TXT_WIDGET_WEATHER_TEXT_COLOUR_WHITE, TXT_WIDGET_WEATHER_TEXT_COLOUR_BLACK};
#ifdef MMI_PDA_SUPPORT
            MMI_TEXT_ID_T          set_type_text_id[] = {TXT_WIDGET_WEATHER_BG_IMG_TYPE, TXT_COMMON_FONT_COLOR};
#else
            MMI_TEXT_ID_T          set_type_text_id[] = {TXT_WIDGET_WEATHER_BG_IMG_TYPE, TXT_COMMON_FONT_COLOR};
#endif
            MMI_STRING_T           itemStr = {0};
            GUISETLIST_ITEM_INFO_T item_info  ={0};

            for(i =0; i < ARR_SIZE(bg_type_text_id); i++)
            {
                MMI_GetLabelTextByLang(bg_type_text_id[i], &itemStr);

                item_info.item_state = GUISETLIST_ITEM_STATE_TEXT;
                item_info.str_info.wstr_len = itemStr.wstr_len;
                item_info.str_info.wstr_ptr = itemStr.wstr_ptr;
                GUISETLIST_AddItem(s_sel_bg_textcolour_ctrl_id_table[0][MMIWIDGET_WEATHER_CONTROL_TYPE_DROPDOWN], &item_info);
            }

            for(i =0; i < ARR_SIZE(text_colour_text_id); i++)
            {
                MMI_GetLabelTextByLang(text_colour_text_id[i], &itemStr);

                item_info.item_state = GUISETLIST_ITEM_STATE_TEXT;
                item_info.str_info.wstr_len = itemStr.wstr_len;
                item_info.str_info.wstr_ptr = itemStr.wstr_ptr;
                GUISETLIST_AddItem(s_sel_bg_textcolour_ctrl_id_table[1][MMIWIDGET_WEATHER_CONTROL_TYPE_DROPDOWN], &item_info);
            }

            for (i = 0; i < BG_TEXTCOLOUR_CTRL_ID_TABLE_LEN; i++)
            {
                MMI_GetLabelTextByLang(set_type_text_id[i], &itemStr);
                GUILABEL_SetText(s_sel_bg_textcolour_ctrl_id_table[i][MMIWIDGET_WEATHER_CONTROL_TYPE_LABLE], &itemStr, FALSE);

                GUISETLIST_SetCurIndex(s_sel_bg_textcolour_ctrl_id_table[i][MMIWIDGET_WEATHER_CONTROL_TYPE_DROPDOWN], ParserNvValue(i + 1, &s_cur_bg_textcolour_set_info));
                GUISETLIST_SetCtrlState(s_sel_bg_textcolour_ctrl_id_table[i][MMIWIDGET_WEATHER_CONTROL_TYPE_DROPDOWN], GUISETLIST_STATE_OPEN_POPWIN, TRUE);
                GUISETLIST_SetTitleTextId(s_sel_bg_textcolour_ctrl_id_table[i][MMIWIDGET_WEATHER_CONTROL_TYPE_DROPDOWN],set_type_text_id[i]);
                GUIFORM_SetStyle(s_sel_bg_textcolour_ctrl_id_table[i][MMIWIDGET_WEATHER_CONTROL_TYPE_FORM], GUIFORM_STYLE_UNIT);
            }
        }
        break;
    default:
        break;
    }
    
}

/*****************************************************************************/
//    Description : 初始化Widget设置信息
//    Global resource dependence : 
//    Author:   xiaoming.ren
//    Note:
/*****************************************************************************/
LOCAL void InitWidgetWeatherSetInfo(void)
{
    if (s_is_need_read_hit_set_info)
    {
        ReadHitSetInfo(&s_cur_hit_set_info);
    }
    
    if (s_is_need_read_bg_textcolour_set_info)
    {
        ReadBgAndTextColorSetInfo(&s_cur_bg_textcolour_set_info);
    }
    
}

/*****************************************************************************/
//    Description : 获取当前天气信息
//    Global resource dependence : 
//    Author:   xiaoming.ren
//    Note:
/*****************************************************************************/
LOCAL void GetCurWeatherInfo(void)
{
    MMI_STRING_T weather_str = {0};
#ifdef BAR_PHONE_WIDGET
    BarPhoneWeather_SetWebKeyCondition();
#endif

    GetDisplayText(&weather_str, MMIWIDGET_WEATHER_DISPALY_TEXT_TYPE_CITYNAME);
    GetDisplayText(&weather_str, MMIWIDGET_WEATHER_DISPALY_TEXT_TYPE_CONDITION);
    GetDisplayText(&weather_str, MMIWIDGET_WEATHER_DISPALY_TEXT_TYPE_TEMPRANGE);
}

/*****************************************************************************/
//    Description : 获取点击区域
//    Global resource dependence : 
//    Author:   xiaoming.ren
//    Note:
/*****************************************************************************/
LOCAL void CalcHitRect(MMI_WIN_ID_T        win_id)
{
    GUI_RECT_T bg_rect = {0};
    
    MMK_GetWinRect(win_id, &bg_rect);

    SCI_MEMSET(&s_click_rect[0], 0x00, sizeof(s_click_rect));
    s_click_rect[MMIWIDGET_WEATHER_CLICK_RECT_LEFT_TOP].left    = bg_rect.left + MMIWIDGET_WEATHER_HIT_TOP_LEFT_IMG_START_X;
    s_click_rect[MMIWIDGET_WEATHER_CLICK_RECT_LEFT_TOP].top     = bg_rect.top + MMIWIDGET_WEATHER_HIT_TOP_IMG_START_Y;
    s_click_rect[MMIWIDGET_WEATHER_CLICK_RECT_LEFT_TOP].right   = s_click_rect[MMIWIDGET_WEATHER_CLICK_RECT_LEFT_TOP].left + MMIWIDGET_WEATHER_HIT_TOP_IMG_WIDTH;
    s_click_rect[MMIWIDGET_WEATHER_CLICK_RECT_LEFT_TOP].bottom  = s_click_rect[MMIWIDGET_WEATHER_CLICK_RECT_LEFT_TOP].top + MMIWIDGET_WEATHER_HIT_TOP_IMG_HEIGHT;
    
    s_click_rect[MMIWIDGET_WEATHER_CLICK_RECT_RIGHT_TOP].left   = bg_rect.left + MMIWIDGET_WEATHER_HIT_TOP_RIGHT_IMG_START_X;
    s_click_rect[MMIWIDGET_WEATHER_CLICK_RECT_RIGHT_TOP].top    = s_click_rect[MMIWIDGET_WEATHER_CLICK_RECT_LEFT_TOP].top;
    s_click_rect[MMIWIDGET_WEATHER_CLICK_RECT_RIGHT_TOP].right  = s_click_rect[MMIWIDGET_WEATHER_CLICK_RECT_RIGHT_TOP].left + MMIWIDGET_WEATHER_HIT_TOP_IMG_WIDTH;
    s_click_rect[MMIWIDGET_WEATHER_CLICK_RECT_RIGHT_TOP].bottom = s_click_rect[MMIWIDGET_WEATHER_CLICK_RECT_RIGHT_TOP].top + MMIWIDGET_WEATHER_HIT_TOP_IMG_HEIGHT;

#if defined(MMI_PDA_SUPPORT)&&!defined(MAINLCD_SIZE_320X480)
    s_click_rect[MMIWIDGET_WEATHER_CLICK_RECT_LEFT_BOTTOM].left   = bg_rect.left + MMIWIDGET_WEATHER_LEFT_TEXT_START_X+11;
#else
    s_click_rect[MMIWIDGET_WEATHER_CLICK_RECT_LEFT_BOTTOM].left   = bg_rect.left + MMIWIDGET_WEATHER_LEFT_TEXT_START_X;
#endif
    s_click_rect[MMIWIDGET_WEATHER_CLICK_RECT_LEFT_BOTTOM].top    = bg_rect.top + MMIWIDGET_WEATHER_HIT_BOTTOM_IMG_START_Y;
    s_click_rect[MMIWIDGET_WEATHER_CLICK_RECT_LEFT_BOTTOM].right  = s_click_rect[MMIWIDGET_WEATHER_CLICK_RECT_LEFT_BOTTOM].left + MMIWIDGET_WEATHER_HIT_BOTTOM_IMG_WIDTH;
    s_click_rect[MMIWIDGET_WEATHER_CLICK_RECT_LEFT_BOTTOM].bottom = s_click_rect[MMIWIDGET_WEATHER_CLICK_RECT_LEFT_BOTTOM].top + MMIWIDGET_WEATHER_HIT_BOTTOM_IMG_HEIGHT;
    
    s_click_rect[MMIWIDGET_WEATHER_CLICK_RECT_MID_BOTTOM].left    = s_click_rect[MMIWIDGET_WEATHER_CLICK_RECT_LEFT_BOTTOM].right;
    s_click_rect[MMIWIDGET_WEATHER_CLICK_RECT_MID_BOTTOM].top     = s_click_rect[MMIWIDGET_WEATHER_CLICK_RECT_LEFT_BOTTOM].top;
    s_click_rect[MMIWIDGET_WEATHER_CLICK_RECT_MID_BOTTOM].right   = s_click_rect[MMIWIDGET_WEATHER_CLICK_RECT_MID_BOTTOM].left + MMIWIDGET_WEATHER_HIT_BOTTOM_IMG_WIDTH;
    s_click_rect[MMIWIDGET_WEATHER_CLICK_RECT_MID_BOTTOM].bottom  = s_click_rect[MMIWIDGET_WEATHER_CLICK_RECT_LEFT_BOTTOM].bottom;

    
    s_click_rect[MMIWIDGET_WEATHER_CLICK_RECT_RIGHT_BOTTOM].top     = s_click_rect[MMIWIDGET_WEATHER_CLICK_RECT_LEFT_BOTTOM].top;
    s_click_rect[MMIWIDGET_WEATHER_CLICK_RECT_RIGHT_BOTTOM].right   = bg_rect.right - MMIWIDGET_WEATHER_LEFT_TEXT_START_X;
    s_click_rect[MMIWIDGET_WEATHER_CLICK_RECT_RIGHT_BOTTOM].left    = s_click_rect[MMIWIDGET_WEATHER_CLICK_RECT_RIGHT_BOTTOM].right - MMIWIDGET_WEATHER_HIT_BOTTOM_IMG_WIDTH;
    s_click_rect[MMIWIDGET_WEATHER_CLICK_RECT_RIGHT_BOTTOM].bottom  = s_click_rect[MMIWIDGET_WEATHER_CLICK_RECT_LEFT_BOTTOM].bottom;

#ifndef PDA_WEATHER_SUPPORT_LOW_MEMORY
    UILAYER_SetLayerPosition(&s_highlight_layer_info, bg_rect.left, bg_rect.top);
#endif
}

/*****************************************************************************/
//    Description : 数据复位
//    Global resource dependence : 
//    Author:   xiaoming.ren
//    Note:
/*****************************************************************************/
LOCAL void ResetWidgetWeather(void)
{
	s_tp_hit_type = MMIWIDGET_WEATHER_CLICK_RECT_INVALID;
    DeleteHighlightLayer();
}

/*****************************************************************************/
//    Description : 提取位于index位的NV数据到set_info_ptr中
//    Global resource dependence : 
//    Author:   xiaoming.ren
//    Note:
/*****************************************************************************/
LOCAL uint16 ParserNvValue(uint8 index, uint32 *set_info_ptr)
{
    uint16 value = 0;

    uint32 set_info = 0;

    if (PNULL != set_info_ptr)
    {
        set_info = *set_info_ptr;
        
        value = (set_info & (0xff000000 >>((index -1) * 4 * 2))) >> ((8 - (index * 2))*4);
    }

    return value;
}

/*****************************************************************************/
//    Description : 将value设置到set_info_ptr的index位
//    Global resource dependence : 
//    Author:   xiaoming.ren
//    Note:
/*****************************************************************************/
LOCAL void MakeNvValue(uint8 index, uint16 value, uint32 *set_info_ptr)
{
    if (index <= 4 && PNULL != set_info_ptr)
    {
        *set_info_ptr = (*set_info_ptr & (~(uint32)(0x000000ff << ((8-index * 2) * 4)))) | ((0x000000ff & value) << ((8-index * 2) * 4));
    }
}

/*****************************************************************************/
//    Description : 读取快捷方式设置信息
//    Global resource dependence : 
//    Author:   xiaoming.ren
//    Note:
/*****************************************************************************/
LOCAL void ReadHitSetInfo(uint32* set_info)
{
    MN_RETURN_RESULT_E	return_value = MN_RETURN_FAILURE;

    MMINV_READ(MMIWIDGET_WEATHER_HIT_SET_INFO, set_info, return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        s_cur_hit_set_info = DEFAULT_HIT_SET_INFO;

        WriteHitSetInfo();
    }
    s_is_need_read_hit_set_info = FALSE;
}

/*****************************************************************************/
//    Description : 保存快捷方式设置信息
//    Global resource dependence : 
//    Author:   xiaoming.ren
//    Note:
/*****************************************************************************/
LOCAL void WriteHitSetInfo(void)
{
    MMINV_WRITE(MMIWIDGET_WEATHER_HIT_SET_INFO, &s_cur_hit_set_info);
    s_is_need_read_hit_set_info = TRUE;
}

/*****************************************************************************/
//    Description : 读取背景&时间字体颜色设置信息
//    Global resource dependence : 
//    Author:   xiaoming.ren
//    Note:
/*****************************************************************************/
LOCAL void ReadBgAndTextColorSetInfo(uint32* set_info)
{
    MN_RETURN_RESULT_E	return_value = MN_RETURN_FAILURE;
    
    MMINV_READ(MMIWIDGET_WEATHER_BG_TEXTCOLOUR_SET_INFO, set_info, return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        s_cur_bg_textcolour_set_info = DEFAULT_BG_TEXTCOLOUR_SET_INFO;
        
        WriteBgAndTextColorSetInfo();
    }
    s_is_need_read_bg_textcolour_set_info = FALSE;
}

/*****************************************************************************/
//    Description : 保存背景&时间字体颜色设置信息
//    Global resource dependence : 
//    Author:   xiaoming.ren
//    Note:
/*****************************************************************************/
LOCAL void WriteBgAndTextColorSetInfo(void)
{
    MMINV_WRITE(MMIWIDGET_WEATHER_BG_TEXTCOLOUR_SET_INFO, &s_cur_bg_textcolour_set_info);
    s_is_need_read_bg_textcolour_set_info = TRUE;
}

/*****************************************************************************/
//    Description : 打开快捷方式设置窗口
//    Global resource dependence : 
//    Author:   xiaoming.ren
//    Note:
/*****************************************************************************/
PUBLIC void MMIWIDGET_Weather_OpenShortcutSettingWin(void)
{
    MMK_CreateWin((uint32*)WIDGET_WEATHER_SET_HIT_FUNC_WIN_TAB,PNULL);
}

/*****************************************************************************/
//    Description : 打开字体颜色设置窗口
//    Global resource dependence : 
//    Author:   xiaoming.ren
//    Note:
/*****************************************************************************/
PUBLIC void MMIWIDGET_Weather_OpenBGTextColorSettingWin(void)
{
    MMK_CreateWin((uint32*)WIDGET_WEATHER_SET_BG_TEXTCOLOUR_WIN_TAB,PNULL);
}

/*****************************************************************************/
//    Description : 恢复默认设置
//    Global resource dependence : 
//    Author:   xiaoming.ren
//    Note:
/*****************************************************************************/
PUBLIC void MMIAPIWEATHER_ResetDefaultSetting(void)
{
    s_cur_hit_set_info = DEFAULT_HIT_SET_INFO;
    s_cur_bg_textcolour_set_info = DEFAULT_BG_TEXTCOLOUR_SET_INFO;
    
    WriteHitSetInfo();
    WriteBgAndTextColorSetInfo();
}


//===================================================================================================//
#ifdef PDA_WEATHER_TIME_SE

/*****************************************************************************/
//    Description : 创建天气预报widget时间翻页特效Layer
//    Global resource dependence : 
//    Author:  Gaily.Wang
//    Note:
/*****************************************************************************/
LOCAL BOOLEAN CreateTimeBoxLayer(GUI_LCD_DEV_INFO *layer_ptr, int16 offset_x, int16 offset_y)
{
    BOOLEAN             result          = FALSE;
    UILAYER_CREATE_T    create_info     = {0};
    GUI_LCD_DEV_INFO    layer_handle    = {0};
    GUI_RECT_T          win_rect        = {0};
    uint16              is_bg_enable    =FALSE;

    if (PNULL != layer_ptr)
    {
        MMK_GetWinRect(g_weather_widget.win_id, &win_rect);
        
        create_info.lcd_id          = GUI_MAIN_LCD_ID;
        create_info.owner_handle    = MAIN_IDLE_WIN_ID;
        create_info.offset_x        = win_rect.left + offset_x; 
        create_info.offset_y        = win_rect.top + offset_y;
        create_info.width           = TIME_BOX_WIDTH;
        create_info.height          = TIME_BOX_HEIGHT;
        create_info.is_bg_layer     = FALSE;
        create_info.is_static_layer = FALSE;
        
        if (UILAYER_RESULT_SUCCESS == UILAYER_CreateLayer(&create_info, &layer_handle))
        {
            is_bg_enable 
                = ParserNvValue(MMIWIDGET_WEATHER_BG_TEXTCOLOUR_SET_NV_ITEM_BG, &s_cur_bg_textcolour_set_info);

            //时间框的背景
            if(is_bg_enable)
            {   
                GUI_POINT_T     display_point   = {0};
                MMI_IMAGE_ID_T  img_id          = IMAGE_WIDGET_WEATHER_TIME_BG;

                display_point.x = win_rect.left + offset_x;
                display_point.y = win_rect.top  + offset_y;
                GUIRES_DisplayImg(&display_point, PNULL, PNULL, g_weather_widget.win_id, img_id, &layer_handle);
            }

            *layer_ptr = layer_handle;
            
            result = TRUE;
        }
    }

    return result;
}

/*****************************************************************************/
//  Discription: Start time update se timer
//  Global resource dependence: None
//  Author: Gaily.Wang
//  Note : 
/*****************************************************************************/
LOCAL void StopTimeUpdateSeTimer(void)
{
    //SCI_TRACE_LOW:"[weather]:Weather_StopSockTimer"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_WEATHER_3151_112_2_18_3_8_50_248,(uint8*)"");

    if(0 != s_se_timer_id)
    {
        MMK_StopTimer(s_se_timer_id);
        s_se_timer_id = 0;
    }
}
/*****************************************************************************/
//  Discription: 释放时间特效图层
//  Global resource dependence: None
//  Author: Jin.Wang
//  Note : 
/*****************************************************************************/
LOCAL void RealeaseTimeSeLayer(void)
{
    GUI_LCD_DEV_INFO *layer_array[] = {&s_min_new_layer,
                                      &s_min_old_layer,
                                      &s_min_calc_layer,
                                      &s_hour_new_layer,
                                      &s_hour_old_layer,
                                      &s_hour_calc_layer};
    uint16 i = 0;
    
    for (i = 0; i < WEATHER_TIME_LAYER_MAX; i++)
    {
        if (0 != layer_array[i]->block_id 
            && UILAYER_NULL_HANDLE != layer_array[i]->block_id)
        {
            UILAYER_RELEASELAYER(layer_array[i]);   /*lint !e506 !e774*/
//            UILAYER_ReleaseLayer(layer_array[i]);
//            layer_array[i]->block_id = UILAYER_NULL_HANDLE;
        }
    }

    return;
}

/*****************************************************************************/
//  Description : handle time update se timer
//  Global resource dependence : none
//  Author: Gaily.Wang
//  Note:
/*****************************************************************************/
LOCAL void CalcuteOneFrame(
    uint8  frame_id,                          //[in]第几帧      
    uint16 width,           
    uint16 height,
    GUI_LCD_DEV_INFO*  old_src_layer_ptr,     //[in]更新前的时间
    GUI_LCD_DEV_INFO*  new_src_layer_ptr,     //[in]更新后的时间
    GUI_LCD_DEV_INFO*  dst_layer_ptr          //[out]输出计算结果的Layer
)
{
    S3D_ROTATION_GEOMETRY_T       s3d_geometry = {0};
    S3D_ROTATION_IMG_T            s3d_src_img = {0};
    S3D_ROTATION_INOUT_IMG_INFO_T s3d_img = {0};
    UILAYER_INFO_T layer_info = {0};

    s3d_geometry.angle = 1800-s_time_se_angle_able[frame_id];

    s3d_geometry.img_size.w = width;//NUM_WIDTH;
    s3d_geometry.img_size.h = height;//NUM_HEIGHT;
    s3d_geometry.img_size.x = 0;
    s3d_geometry.img_size.y = height/2;

    s3d_geometry.eye.x = width/2;
    s3d_geometry.eye.y = height;
    s3d_geometry.eye.z = -MMI_MAINSCREEN_WIDTH*50;

    s3d_geometry.axis.x = width/2;
    s3d_geometry.axis.y = height;
    s3d_geometry.axis.z = MMI_MAINSCREEN_HEIGHT*0;
    s3d_geometry.axis.axis = 0;

    s3d_geometry.is_axis_use_default = SCI_FALSE;
    s3d_geometry.is_eye_use_default = SCI_FALSE;

    s3d_src_img.color       = 0;
    s3d_src_img.src_type    = S3D_DATA_ARGB888;
    
    if(s3d_geometry.angle > 900)
    {
        s3d_src_img.src_ptr = UILAYER_GetLayerBufferPtr(old_src_layer_ptr) ;
        if (PNULL == s3d_src_img.src_ptr)
        {
            return;
        }
    }
    else
    {
		s3d_geometry.img_size.y = -height/2;
		s3d_img.color_ptr = UILAYER_GetLayerBufferPtr(dst_layer_ptr);
        UILAYER_GetLayerInfo(dst_layer_ptr, &layer_info);
        if ((PNULL == s3d_img.color_ptr) || (width > layer_info.layer_width) || (height > layer_info.layer_height))
        {
            return;
        }
        s3d_img.color_ptr = (uint8*)s3d_img.color_ptr + (width * (height) * sizeof(uint32));
		s3d_geometry.angle = 1800 - s3d_geometry.angle;
        s3d_src_img.src_ptr = UILAYER_GetLayerBufferPtr(new_src_layer_ptr);
        UILAYER_GetLayerInfo(new_src_layer_ptr, &layer_info);
        if ((PNULL == s3d_src_img.src_ptr) || (width > layer_info.layer_width) || (height > layer_info.layer_height))
        {
            return;
        }        
        s3d_src_img.src_ptr = (uint8*)s3d_src_img.src_ptr + (width * (height) * sizeof(uint32));
    }

    s3d_img.size.w = width;
    s3d_img.size.h = height*2;
    s3d_img.color_ptr = UILAYER_GetLayerBufferPtr(dst_layer_ptr);
    if (PNULL == s3d_img.color_ptr)
    {
        return;
    }
    UILAYER_Clear(dst_layer_ptr);
    S3D_ImageMidAxisRotation(&s3d_geometry, &s3d_src_img, &s3d_img);

	s3d_geometry.img_size.y = height/2;

	s3d_src_img.src_ptr = UILAYER_GetLayerBufferPtr(new_src_layer_ptr);
    if (PNULL == s3d_src_img.src_ptr)
    {
        return;
    }
	s3d_geometry.angle = 1800;
	S3D_ImageMidAxisRotation(&s3d_geometry,
							&s3d_src_img,
							&s3d_img);

    s3d_img.color_ptr = UILAYER_GetLayerBufferPtr(dst_layer_ptr);
    UILAYER_GetLayerInfo(dst_layer_ptr, &layer_info);
    if ((PNULL == s3d_src_img.src_ptr) || (width > layer_info.layer_width) || (height > layer_info.layer_height))
    {
        return;
    }
    s3d_img.color_ptr = (uint8*)s3d_img.color_ptr + (width * (height) * sizeof(uint32));
    
	s3d_src_img.src_ptr = UILAYER_GetLayerBufferPtr(old_src_layer_ptr);
    UILAYER_GetLayerInfo(old_src_layer_ptr, &layer_info);
    if ((PNULL == s3d_src_img.src_ptr) || (width > layer_info.layer_width) || (height > layer_info.layer_height))
    {
        return;
    }
    s3d_src_img.src_ptr = (uint8*)s3d_src_img.src_ptr + (width * (height) * sizeof(uint32));
	s3d_geometry.angle = 1800;
	S3D_ImageMidAxisRotation(&s3d_geometry,
							&s3d_src_img,
							&s3d_img);
}

/*****************************************************************************/
//  Description : Handle weather time update timer
//  Global resource dependence : none
//  Author: Gaily.Wang
//  Note:
/*****************************************************************************/
LOCAL void HandleTimeUpdateSeTimer(
    uint8  timer_id,
    uint32 param
)
{
    GUI_RECT_T      win_rect    = {0};
    GUI_RECT_T      minute_rect = {0};
    GUI_RECT_T      hour_rect   = {0};
    GUI_RECT_T      rect        = {0};
    GUI_RECT_T      scr_rect    = MMITHEME_GetFullScreenRect();
    
    //SCI_TRACE_LOW:"[weather]:HandleTimeUpdateTimer---%d/16---"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_WEATHER_3313_112_2_18_3_8_50_249,(uint8*)"d" ,s_se_frame_id );
//#if defined MMI_GRID_IDLE_SUPPORT
//    if (MMIGRID_IsForbidPaint(MMIWIDGET_WEATHER_ID))
//    {
//        StopTimeUpdateSe();
//        return;
//    }
//#endif
    if(!MMK_IsFocusWin(MAIN_IDLE_WIN_ID)) 
    {
        return ;
    }

#if defined MMI_GRID_IDLE_SUPPORT
    if(MMISET_IDLE_STYLE_GRID == MMIAPISET_GetIdleStyle())
    {
        if(MMIGRID_IsForbidPaint(MMIWIDGET_WEATHER_ID))
        {
            return ;
        }
    }
#endif
        
#if defined QBTHEME_SUPPORT
    if(MMISET_IDLE_QBTHEME == MMIAPISET_GetIdleStyle())
    {
        if(MMIQBIDLE_IsForbidPaint(MMIWIDGET_WEATHER_ID))
        {
            return ;
        }
    }
#endif
    
      
    MMK_GetWinRect(WIDGET_WEATHER_WIN_ID, &win_rect);
    
    if(!GUI_IntersectRect(&rect, win_rect, scr_rect))
    {
        return ;
    }
    
    if(s_se_frame_id <= TIME_UPDATE_SE_FRAME_NUM + HOUR_PAGE_OFFSET_FRAME_NUM)
    {
        GUI_LCD_DEV_INFO    weather_widget_layer = {0};
        MMK_GetWinLcdDevInfo(g_weather_widget.win_id, &weather_widget_layer);
        minute_rect.left   = win_rect.left + MINUTE_PAGE_OFFSET_X;
        minute_rect.top    = win_rect.top  + MINUTE_PAGE_OFFSET_Y;
        minute_rect.right  = minute_rect.left + TIME_BOX_WIDTH;
        minute_rect.bottom = minute_rect.top  + TIME_BOX_HEIGHT;
        hour_rect.left   = win_rect.left + HOUR_PAGE_OFFSET_X;
        hour_rect.top    = win_rect.top  + HOUR_PAGE_OFFSET_Y;
        hour_rect.right  = hour_rect.left + TIME_BOX_WIDTH;
        hour_rect.bottom = hour_rect.top  + TIME_BOX_HEIGHT;
        if(s_is_hour_changed)
        {
            if(s_se_frame_id < HOUR_PAGE_OFFSET_FRAME_NUM)
            {
                //Minute
                CalcuteOneFrame(s_se_frame_id, 
                                TIME_BOX_WIDTH, 
                                TIME_BOX_HEIGHT/2,
                                &s_min_old_layer, 
                                &s_min_new_layer, 
                                &s_min_calc_layer);
                UILAYER_BltLayerToLayer(&weather_widget_layer, &s_min_calc_layer, &minute_rect, FALSE);
                MMITHEME_StoreUpdateRect(&weather_widget_layer, minute_rect);
                
                //Hour
                UILAYER_BltLayerToLayer(&weather_widget_layer, &s_hour_old_layer, &hour_rect, FALSE);
                MMITHEME_StoreUpdateRect(&weather_widget_layer, hour_rect);
                
            }
            else if(s_se_frame_id < TIME_UPDATE_SE_FRAME_NUM)
            {
                //Minute
                CalcuteOneFrame(s_se_frame_id, 
                                TIME_BOX_WIDTH, 
                                TIME_BOX_HEIGHT/2,
                                &s_min_old_layer, 
                                &s_min_new_layer, 
                                &s_min_calc_layer);
                UILAYER_BltLayerToLayer(&weather_widget_layer, &s_min_calc_layer, &minute_rect, FALSE);
                MMITHEME_StoreUpdateRect(&weather_widget_layer, minute_rect);
                
                //Hour
                CalcuteOneFrame(s_se_frame_id - HOUR_PAGE_OFFSET_FRAME_NUM, 
                                TIME_BOX_WIDTH, 
                                TIME_BOX_HEIGHT/2,
                                &s_hour_old_layer, 
                                &s_hour_new_layer, 
                                &s_hour_calc_layer);
                
                UILAYER_BltLayerToLayer(&weather_widget_layer, &s_hour_calc_layer, &hour_rect, FALSE);
                MMITHEME_StoreUpdateRect(&weather_widget_layer, hour_rect);
            }
            else if(s_se_frame_id < TIME_UPDATE_SE_FRAME_NUM + HOUR_PAGE_OFFSET_FRAME_NUM)
            {
                //Minute
                UILAYER_BltLayerToLayer(&weather_widget_layer, &s_min_new_layer, &minute_rect, FALSE);
                MMITHEME_StoreUpdateRect(&weather_widget_layer, minute_rect);
            
                //Hour
                CalcuteOneFrame(s_se_frame_id - HOUR_PAGE_OFFSET_FRAME_NUM, 
                                TIME_BOX_WIDTH, 
                                TIME_BOX_HEIGHT/2,
                                &s_hour_old_layer, 
                                &s_hour_new_layer, 
                                &s_hour_calc_layer);
                UILAYER_BltLayerToLayer(&weather_widget_layer, &s_hour_calc_layer, &hour_rect, FALSE);
                MMITHEME_StoreUpdateRect(&weather_widget_layer, hour_rect);
            }
            else
            {
                UILAYER_BltLayerToLayer(&weather_widget_layer, &s_hour_new_layer, &hour_rect, FALSE);
                MMITHEME_StoreUpdateRect(&weather_widget_layer, hour_rect);
            }

        }
        else //小时没变，只更新分钟
        {
			if(s_se_frame_id < TIME_UPDATE_SE_FRAME_NUM)
			{
				//Hour
				UILAYER_BltLayerToLayer(&weather_widget_layer, &s_hour_old_layer, &hour_rect, FALSE);
				MMITHEME_StoreUpdateRect(&weather_widget_layer, hour_rect);
                
				//Minute
				CalcuteOneFrame(s_se_frame_id, 
								TIME_BOX_WIDTH, 
								TIME_BOX_HEIGHT/2,
								&s_min_old_layer, 
								&s_min_new_layer, 
								&s_min_calc_layer);
				UILAYER_BltLayerToLayer(&weather_widget_layer, &s_min_calc_layer, &minute_rect, FALSE);
				MMITHEME_StoreUpdateRect(&weather_widget_layer, minute_rect);
			}
            else
            {
				UILAYER_BltLayerToLayer(&weather_widget_layer, &s_min_new_layer, &minute_rect, FALSE);
				MMITHEME_StoreUpdateRect(&weather_widget_layer, minute_rect);
            }

        }

#ifdef MMIWIDGET_SUPPORT
        if (MMISET_IDLE_STYLE_WIDGET == MMIAPISET_GetIdleStyle()) 
        {        
            MMIWIDGET_UpdateWidgetByWidgetId(MMIWIDGET_WEATHER_ID);
        }
#endif
        MMITHEME_UpdateRect();
        
        s_se_frame_id++;
    }
    else
    {
        StopTimeUpdateSe();
#ifdef MMIWIDGET_WEATHER_ANIM
        MMK_SendMsg(g_weather_widget.win_id, MSG_WEATHER_START_ANIM, PNULL);
#endif
    }
}

/*****************************************************************************/
//  Discription: Start time update se timer
//  Global resource dependence: None
//  Author: Gaily.Wang
//  Note : 
/*****************************************************************************/
LOCAL void StartTimeUpdateSeTimer(void)
{
    if(0 != s_se_timer_id)
    {
        MMK_StopTimer(s_se_timer_id);
        s_se_timer_id = 0;
    }

    s_se_frame_id = 0;  //从第0帧开始显示翻页动画
    
    s_se_timer_id = MMK_CreateTimerCallback(TIME_UPDATE_SE_TIMER, 
                                            HandleTimeUpdateSeTimer, 
                                            0, 
                                            TRUE);

    //SCI_TRACE_LOW:"[weather]:Weather_StartSockTimer"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_WEATHER_3495_112_2_18_3_8_51_250,(uint8*)"");
}

/*****************************************************************************/
//    Description :画特效的背景
//    Global resource dependence : 
//    Author:  Arvin.wu
//    Note:
/*****************************************************************************/
LOCAL void DrawTimeBg(
    GUI_LCD_DEV_INFO *layer_handle, 
    int16 offset_x, 
    int16 offset_y
)
{
    BOOLEAN is_bg_enable = FALSE;
    GUI_RECT_T  win_rect = {0};
    
    is_bg_enable = ParserNvValue(MMIWIDGET_WEATHER_BG_TEXTCOLOUR_SET_NV_ITEM_BG, &s_cur_bg_textcolour_set_info);

    UILAYER_Clear(layer_handle); 
    //时间框的背景
    if(is_bg_enable)
    {   
        GUI_POINT_T     display_point   = {0};
        MMI_IMAGE_ID_T  img_id          = 0;
        
        MMK_GetWinRect(g_weather_widget.win_id, &win_rect);
        img_id = IMAGE_WIDGET_WEATHER_TIME_BG;
        display_point.x = win_rect.left + offset_x;
        display_point.y = win_rect.top  + offset_y;
        
        UILAYER_SetLayerPosition(layer_handle,display_point.x,display_point.y);
        GUIRES_DisplayImg(&display_point, PNULL, PNULL, g_weather_widget.win_id, img_id, layer_handle);
    }
}

/*****************************************************************************/
//    Description :播放时间更新特效
//    Global resource dependence : 
//    Author:  Gaily.Wang
//    Note:
/*****************************************************************************/
LOCAL BOOLEAN PlayTimeUpdateSe(SCI_TIME_T pre_sys_time)
{
    GUI_RECT_T          win_rect        = {0};
    SCI_TIME_T          cur_sys_time    = {0};
    uint16              text_colour     = 0;
    GUI_POINT_T         display_point   = {0};
    MMI_IMAGE_ID_T      img_id          = 0;
    //MMIWIDGET_TIME_SE_T time_update_se  = {0};
    
    MMK_GetWinRect(g_weather_widget.win_id, &win_rect);
    TM_GetSysTime(&cur_sys_time);
    
    if(0 == s_se_timer_id && (pre_sys_time.hour != cur_sys_time.hour || pre_sys_time.min != cur_sys_time.min))
    {       
        if(pre_sys_time.hour == cur_sys_time.hour)
        {
            s_is_hour_changed = FALSE;
        }
        else
        {
            s_is_hour_changed = TRUE;
        }

        text_colour = ParserNvValue(MMIWIDGET_WEATHER_BG_TEXTCOLOUR_SET_NV_ITEM_TEXTCOLOUR, &s_cur_bg_textcolour_set_info);;
        
        // -1- ======= Create Old Time Layer ========
        DrawTimeBg(&s_min_old_layer,  MINUTE_PAGE_OFFSET_X, MINUTE_PAGE_OFFSET_Y);
        //分钟十位数
        img_id = s_Number_Image_index[pre_sys_time.min / 10][text_colour];
        display_point.x = win_rect.left + MMIWIDGET_WEATHER_TIME_IMG_MINUTE_START_X;
        display_point.y = win_rect.top  + MMIWIDGET_WEATHER_TIME_IMG_START_Y;
        GUIRES_DisplayImg(&display_point, PNULL, PNULL, g_weather_widget.win_id, img_id, &s_min_old_layer);
        //分钟个位数
        img_id = s_Number_Image_index[pre_sys_time.min % 10][text_colour];
        display_point.x = win_rect.left + MMIWIDGET_WEATHER_TIME_IMG_MINUTE_START_X + MMIWIDGET_WEATHER_TIME_IMG_WIDTH + 3;
        display_point.y = win_rect.top  + MMIWIDGET_WEATHER_TIME_IMG_START_Y;
        GUIRES_DisplayImg(&display_point, PNULL, PNULL, g_weather_widget.win_id, img_id, &s_min_old_layer);

        // -2- ======= Create New Time Layer ========
        DrawTimeBg(&s_min_new_layer,  MINUTE_PAGE_OFFSET_X, MINUTE_PAGE_OFFSET_Y);
        //分钟十位数
        img_id = s_Number_Image_index[cur_sys_time.min / 10][text_colour];
        display_point.x = win_rect.left + MMIWIDGET_WEATHER_TIME_IMG_MINUTE_START_X;
        display_point.y = win_rect.top  + MMIWIDGET_WEATHER_TIME_IMG_START_Y;
        GUIRES_DisplayImg(&display_point, PNULL, PNULL, g_weather_widget.win_id, img_id, &s_min_new_layer);
        //分钟个位数
        img_id = s_Number_Image_index[cur_sys_time.min % 10][text_colour];
        display_point.x = win_rect.left + MMIWIDGET_WEATHER_TIME_IMG_MINUTE_START_X + MMIWIDGET_WEATHER_TIME_IMG_WIDTH + 3;
        display_point.y = win_rect.top  + MMIWIDGET_WEATHER_TIME_IMG_START_Y;
        GUIRES_DisplayImg(&display_point, PNULL, PNULL, g_weather_widget.win_id, img_id, &s_min_new_layer);

        // -3- ======= Create temp layer used to calcute frame data========
        DrawTimeBg(&s_min_calc_layer,  MINUTE_PAGE_OFFSET_X, MINUTE_PAGE_OFFSET_Y);
        
         // -4- ======= hour Old ========
        DrawTimeBg(&s_hour_old_layer,  HOUR_PAGE_OFFSET_X, HOUR_PAGE_OFFSET_Y);
         //小时十位数
        img_id = s_Number_Image_index[pre_sys_time.hour / 10][text_colour];
        display_point.x = win_rect.left + MMIWIDGET_WEATHER_TIME_IMG_HOUR_START_X;
        display_point.y = win_rect.top  + MMIWIDGET_WEATHER_TIME_IMG_START_Y;
        GUIRES_DisplayImg(&display_point, PNULL, PNULL, g_weather_widget.win_id, img_id, &s_hour_old_layer);
        //小时个位数
        img_id = s_Number_Image_index[pre_sys_time.hour % 10][text_colour];
        display_point.x = win_rect.left + MMIWIDGET_WEATHER_TIME_IMG_HOUR_START_X + MMIWIDGET_WEATHER_TIME_IMG_WIDTH + 3;
        display_point.y = win_rect.top  + MMIWIDGET_WEATHER_TIME_IMG_START_Y;
        GUIRES_DisplayImg(&display_point, PNULL, PNULL, g_weather_widget.win_id, img_id, &s_hour_old_layer);

        // -5- ======= hour New ========
        DrawTimeBg(&s_hour_new_layer,  HOUR_PAGE_OFFSET_X, HOUR_PAGE_OFFSET_Y);
        //小时十位数
        img_id = s_Number_Image_index[cur_sys_time.hour / 10][text_colour];
        display_point.x = win_rect.left + MMIWIDGET_WEATHER_TIME_IMG_HOUR_START_X;
        display_point.y = win_rect.top  + MMIWIDGET_WEATHER_TIME_IMG_START_Y;
        GUIRES_DisplayImg(&display_point, PNULL, PNULL, g_weather_widget.win_id, img_id, &s_hour_new_layer);
        //小时个位数
        img_id = s_Number_Image_index[cur_sys_time.hour % 10][text_colour];
        display_point.x = win_rect.left + MMIWIDGET_WEATHER_TIME_IMG_HOUR_START_X + MMIWIDGET_WEATHER_TIME_IMG_WIDTH + 3;
        display_point.y = win_rect.top  + MMIWIDGET_WEATHER_TIME_IMG_START_Y;
        GUIRES_DisplayImg(&display_point, PNULL, PNULL, g_weather_widget.win_id, img_id, &s_hour_new_layer);

        // -6- ======= hour calcute ========
        DrawTimeBg(&s_hour_calc_layer,  HOUR_PAGE_OFFSET_X, HOUR_PAGE_OFFSET_Y);
        StartTimeUpdateSeTimer();

        return TRUE;    
    }

    return FALSE;
}

/*****************************************************************************/
//    Description :播放时间更新特效
//    Global resource dependence : 
//    Author:  Gaily.Wang
//    Note:
/*****************************************************************************/
LOCAL void StopTimeUpdateSe(void)
{
    TM_GetSysTime(&s_sys_time);
    StopTimeUpdateSeTimer();
    DisplayCurrentTime();

    return;
}

/*****************************************************************************/
//    Description :在天气预报widget上显示当前时间。
//    Global resource dependence : 
//    Author:  Gaily.Wang
//    Note:
/*****************************************************************************/
LOCAL void DisplayCurrentTime(void)
{
    GUI_LCD_DEV_INFO    weather_widget_layer = {0};
    GUI_RECT_T          win_rect        = {0};
    GUI_POINT_T         display_point   = {0};
    MMI_IMAGE_ID_T      img_id          = 0;
    uint16              text_colour     = 0;
    uint16              is_bg_enable    = 0;
    SCI_TIME_T          sys_time        = s_sys_time;

    //SCI_TRACE_LOW:"[weather]:DisplayCurrentTime---%02d : %02d ---"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_WEATHER_3651_112_2_18_3_8_51_251,(uint8*)"dd" ,sys_time.hour, sys_time.min);

    is_bg_enable = ParserNvValue(MMIWIDGET_WEATHER_BG_TEXTCOLOUR_SET_NV_ITEM_BG, &s_cur_bg_textcolour_set_info);
    text_colour  = ParserNvValue(MMIWIDGET_WEATHER_BG_TEXTCOLOUR_SET_NV_ITEM_TEXTCOLOUR, &s_cur_bg_textcolour_set_info);;
    
    MMK_GetWinRect(g_weather_widget.win_id, &win_rect);
    MMK_GetWinLcdDevInfo(g_weather_widget.win_id, &weather_widget_layer);

    if (!is_bg_enable)
    {
        GUI_POINT_T colon_point    = {0};
        GUI_RECT_T  colon_dis_rect = {0};
        
        colon_dis_rect.left   = win_rect.left + MMIWIDGET_WEATHER_TIME_IMG_HOUR_START_X + 2 * (MMIWIDGET_WEATHER_TIME_IMG_WIDTH + MMIWIDGET_WEATHER_TIME_IMG_SPACE);
        colon_dis_rect.top    = win_rect.top + MMIWIDGET_WEATHER_TIME_IMG_START_Y;
        colon_dis_rect.right  = win_rect.left + MMIWIDGET_WEATHER_TIME_IMG_MINUTE_START_X;
        colon_dis_rect.bottom = colon_dis_rect.top + MMIWIDGET_WEATHER_TIME_IMG_HEIGHT;
        colon_dis_rect = GUI_GetCenterRectExt(colon_dis_rect, MMIWIDGET_WEATHER_TIME_COLON_IMG_WIDTH, MMIWIDGET_WEATHER_TIME_COLON_IMG_HEIGHT);
        colon_point.x = colon_dis_rect.left;
        colon_point.y = colon_dis_rect.top;
        GUIRES_DisplayImg(&colon_point, PNULL, PNULL, g_weather_widget.win_id, s_Number_Image_index[10][text_colour], &weather_widget_layer);
    }

    //小时的背景
    if(is_bg_enable)
    {
        img_id = IMAGE_WIDGET_WEATHER_TIME_BG;
        display_point.x = win_rect.left + HOUR_PAGE_OFFSET_X;
        display_point.y = win_rect.top  + HOUR_PAGE_OFFSET_Y;
        GUIRES_DisplayImg(&display_point, PNULL, PNULL, g_weather_widget.win_id, img_id, &weather_widget_layer);
    }
    else
    {
        GUI_RECT_T rect = {0};
        rect.left   = win_rect.left + HOUR_PAGE_OFFSET_X;
        rect.top    = win_rect.top  + HOUR_PAGE_OFFSET_Y;
        rect.right  = rect.left + TIME_BOX_WIDTH;
        rect.bottom = rect.top + TIME_BOX_HEIGHT;
        UILAYER_ClearRect(&weather_widget_layer, rect);
    }
    
    //小时十位数
    img_id = s_Number_Image_index[sys_time.hour/ 10][text_colour];
    display_point.x = win_rect.left + MMIWIDGET_WEATHER_TIME_IMG_HOUR_START_X;
    display_point.y = win_rect.top  + MMIWIDGET_WEATHER_TIME_IMG_START_Y;
    GUIRES_DisplayImg(&display_point, PNULL, PNULL, g_weather_widget.win_id, img_id, &weather_widget_layer);
    
    //小时个位数
    img_id = s_Number_Image_index[sys_time.hour % 10][text_colour];
    display_point.x = win_rect.left + MMIWIDGET_WEATHER_TIME_IMG_HOUR_START_X + MMIWIDGET_WEATHER_TIME_IMG_WIDTH + 3;
    display_point.y = win_rect.top  + MMIWIDGET_WEATHER_TIME_IMG_START_Y;
    GUIRES_DisplayImg(&display_point, PNULL, PNULL, g_weather_widget.win_id, img_id, &weather_widget_layer);

    //分钟的背景
    if(is_bg_enable)
    {
        img_id = IMAGE_WIDGET_WEATHER_TIME_BG;
        display_point.x = win_rect.left + MINUTE_PAGE_OFFSET_X;
        display_point.y = win_rect.top  + MINUTE_PAGE_OFFSET_Y;
        GUIRES_DisplayImg(&display_point, PNULL, PNULL, g_weather_widget.win_id, img_id, &weather_widget_layer);
    }
    else
    {
        GUI_RECT_T rect = {0};
        rect.left   = win_rect.left + MINUTE_PAGE_OFFSET_X;
        rect.top    = win_rect.top  + MINUTE_PAGE_OFFSET_Y;
        rect.right  = rect.left + TIME_BOX_WIDTH;
        rect.bottom = rect.top + TIME_BOX_HEIGHT;
        UILAYER_ClearRect(&weather_widget_layer, rect);
    }
    
    //分钟十位数
    img_id = s_Number_Image_index[sys_time.min / 10][text_colour];
    display_point.x = win_rect.left + MMIWIDGET_WEATHER_TIME_IMG_MINUTE_START_X;
    display_point.y = win_rect.top  + MMIWIDGET_WEATHER_TIME_IMG_START_Y;
    GUIRES_DisplayImg(&display_point, PNULL, PNULL, g_weather_widget.win_id, img_id, &weather_widget_layer);
    
    //分钟个位数
    img_id = s_Number_Image_index[sys_time.min % 10][text_colour];
    display_point.x = win_rect.left + MMIWIDGET_WEATHER_TIME_IMG_MINUTE_START_X + MMIWIDGET_WEATHER_TIME_IMG_WIDTH + 3;
    display_point.y = win_rect.top  + MMIWIDGET_WEATHER_TIME_IMG_START_Y;
    GUIRES_DisplayImg(&display_point, PNULL, PNULL, g_weather_widget.win_id, img_id, &weather_widget_layer);

    MMITHEME_StoreUpdateRect(&weather_widget_layer, win_rect);
}

/*****************************************************************************/
//    Description :判断某widget是否可见
//    Global resource dependence : 
//    Author:  Gaily.Wang
//    Note:
/*****************************************************************************/
LOCAL BOOLEAN IsWidgetWinVisible(MMI_WIN_ID_T win_id)
{
    BOOLEAN     visible     = FALSE;
    GUI_RECT_T  win_rect    = {0};
    GUI_RECT_T  scr_rect    = {0, 0, MMI_MAINSCREEN_WIDTH, MMI_MAINSCREEN_HEIGHT};
    GUI_RECT_T  rect    = {0};

    if(MMK_IsFocusWin(MAIN_IDLE_WIN_ID))
    {
        MMK_GetWinRect(win_id, &win_rect);
        
        if(GUI_IntersectRect(&rect, win_rect, scr_rect))
        {
            visible = TRUE;
        }
    }
    
    return visible;
}

/*****************************************************************************/
//    Description :处理时间更新消息
//    Global resource dependence : 
//    Author:  Gaily.Wang
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIWEATHER_HandleTimeUpdate(void)
{
    GUI_RECT_T  rect        = {0};
    GUI_RECT_T  win_rect    = {0};
    GUI_RECT_T  scr_rect    = MMITHEME_GetFullScreenRect();


    if(!MMK_IsFocusWin(MAIN_IDLE_WIN_ID)) 
    {
        return FALSE;
    }

#if defined MMI_GRID_IDLE_SUPPORT
    if(MMISET_IDLE_STYLE_GRID == MMIAPISET_GetIdleStyle())
    {
        if(MMIGRID_IsForbidPaint(MMIWIDGET_WEATHER_ID))
        {
            return FALSE;
        }
    }
#endif
        
#if defined QBTHEME_SUPPORT
    if(MMISET_IDLE_QBTHEME == MMIAPISET_GetIdleStyle())
    {
        if(MMIQBIDLE_IsForbidPaint(MMIWIDGET_WEATHER_ID))
        {
            return FALSE;
        }
    }
#endif

  
    MMK_GetWinRect(WIDGET_WEATHER_WIN_ID, &win_rect);
    
    if(GUI_IntersectRect(&rect, win_rect, scr_rect))
    {
        if(PlayTimeUpdateSe(s_sys_time))
        {
            return TRUE;
        }
    }
    else
    {
        SCI_TIME_T sys_time = {0};
        TM_GetSysTime(&sys_time);
        s_sys_time = sys_time;
        DisplayCurrentTime();
    }
  

    return FALSE;
}
#endif //PDA_WEATHER_TIME_SE
/*****************************************************************************/
//    Description :设为focus
//    Global resource dependence : 
//    Author:  juan.wu
//    Note:
/*****************************************************************************/
LOCAL BOOLEAN  SetShortCutFocus(MMI_WIN_ID_T win_id,uint8 focus_index)
{
    MMI_IMAGE_ID_T img_id = 0;

    
    if(focus_index < MMIWIDGET_WEATHER_CLICK_RECT_INVALID)//focus_index:0~4
    {
        img_id = (focus_index < MMIWIDGET_WEATHER_CLICK_RECT_LEFT_BOTTOM) ? IMAGE_WIDGET_WEATHER_HIGHLIGHT_L : IMAGE_WIDGET_WEATHER_HIGHLIGHT_S;

        DisplayHighlightImg(win_id, img_id, &s_click_rect[focus_index]);
        s_cur_focus_index = focus_index;
        return TRUE;
    }
    else
    {   
        return FALSE;
    }
    
}
/*****************************************************************************/
//    Description :响应web key
//    Global resource dependence : 
//    Author:  juan.wu
//    Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E MMIWIDGET_Weather_HandleWebKey(uint32 index)
{
    MMI_RESULT_E recode   = MMI_RESULT_TRUE;
    //GUI_POINT_T point     = {0};    
    uint16      cur_index = 0;
    uint8       max_num =  ARR_SIZE(s_func_list_table);

    switch(index)
    {
    case MMIWIDGET_WEATHER_CLICK_RECT_LEFT_TOP:
        cur_index = ParserNvValue(MMIWIDGET_WEATHER_HIT_SET_NV_ITEM_LEFT_TOP, &s_cur_hit_set_info);
        if(cur_index >= max_num)
        {
            //SCI_TRACE_LOW:"MMIWIDGET_Weather_HandleWebKey cur_index = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_WEATHER_3859_112_2_18_3_8_52_252,(uint8*)"d",cur_index);
            recode   = MMI_RESULT_FALSE;
            break;
        }
        s_func_list_table[cur_index].pfun();
        break;

    case MMIWIDGET_WEATHER_CLICK_RECT_RIGHT_TOP:
        cur_index = ParserNvValue(MMIWIDGET_WEATHER_HIT_SET_NV_ITEM_RIGHT_TOP, &s_cur_hit_set_info);
        if(cur_index >= max_num)
        {
            //SCI_TRACE_LOW:"MMIWIDGET_Weather_HandleWebKey cur_index = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_WEATHER_3870_112_2_18_3_8_52_253,(uint8*)"d",cur_index);
            recode   = MMI_RESULT_FALSE;
            break;
        }
        s_func_list_table[cur_index].pfun();
        break;
        
    case MMIWIDGET_WEATHER_CLICK_RECT_LEFT_BOTTOM:
#if defined(WIN32) && defined(PDA_WEATHER_TIME_SE)
    {
        SCI_TIME_T sys_time = {0};
        TM_GetSysTime(&sys_time);
        sys_time.min += 1;
        sys_time.hour += 1;
        PlayTimeUpdateSe(sys_time);
    }
#endif
        cur_index = ParserNvValue(MMIWIDGET_WEATHER_HIT_SET_NV_ITEM_LEFT_BOTTOM, &s_cur_hit_set_info);
        if(cur_index >= max_num)
        {
            //SCI_TRACE_LOW:"MMIWIDGET_Weather_HandleWebKey cur_index = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_WEATHER_3890_112_2_18_3_8_52_254,(uint8*)"d",cur_index);
            recode   = MMI_RESULT_FALSE;
            break;
        }
        s_func_list_table[cur_index].pfun();
        break;

    case MMIWIDGET_WEATHER_CLICK_RECT_MID_BOTTOM:
        MMIWEATHER_OpenMainWin();
        break;

    case MMIWIDGET_WEATHER_CLICK_RECT_RIGHT_BOTTOM:
#if defined(WIN32) && defined(PDA_WEATHER_TIME_SE)
    {
        SCI_TIME_T sys_time = {0};
        TM_GetSysTime(&sys_time);
        sys_time.min += 1;
        PlayTimeUpdateSe(sys_time);
    }
#endif
        cur_index = ParserNvValue(MMIWIDGET_WEATHER_HIT_SET_NV_ITEM_RIGHT_BOTTOM, &s_cur_hit_set_info);
        if(cur_index >= max_num)
        {
            //SCI_TRACE_LOW:"MMIWIDGET_Weather_HandleWebKey cur_index = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_WEATHER_3913_112_2_18_3_8_52_255,(uint8*)"d",cur_index);
            recode   = MMI_RESULT_FALSE;
            break;
        }
        s_func_list_table[cur_index].pfun();
        break;

    default:
        recode   = MMI_RESULT_FALSE;
        break;
    }    
    return recode;
}
#ifdef BAR_PHONE_WIDGET
#if !defined MMIWIDGET_WEATHER_SE
LOCAL int16 BarPhoneWeather_DisplayTimeOneNum(
                                    GUI_LCD_DEV_INFO    * lcd_dev_info,
									GUI_POINT_T      start_point,
									uint8 time_num
									)
{
    uint8               *num[] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"};
    wchar               wchar_num[2] = {0};
    MMI_STRING_T        dis_time = {0};
    GUI_RECT_T          string_rect = {0};
    GUISTR_STYLE_T      text_style      = {0}; 
    GUISTR_STATE_T      state = GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE|GUISTR_STATE_NOT_HALF_CHAR; 
    uint16              cur_text_colour = 0;

    cur_text_colour = ParserNvValue(MMIWIDGET_WEATHER_BG_TEXTCOLOUR_SET_NV_ITEM_TEXTCOLOUR, &s_cur_bg_textcolour_set_info);
    //hour1
    string_rect.left = start_point.x;
    string_rect.top = start_point.y;
    string_rect.right = start_point.x + MMIWIDGET_BARWEATHER_TIME_WIDTH;
    string_rect.bottom =  start_point.y + MMIWIDGET_BARWEATHER_TIME_HEIGHT;

    text_style.font = MMIWIDGET_BARWEATHER_TIME_FONT;
    text_style.align = ALIGN_HVMIDDLE;
    text_style.font_color = cur_text_colour;

    MMIAPICOM_StrToWstr(num[time_num], wchar_num);
    dis_time.wstr_ptr = wchar_num;
    dis_time.wstr_len = 1;
    GUISTR_DrawTextToLCDInRect(lcd_dev_info, &string_rect, &string_rect, &dis_time, &text_style, state, GUISTR_TEXT_DIR_AUTO);	
    return string_rect.right;
}
LOCAL void BarPhoneWeather_Display_Time_WeatherInfo(MMI_WIN_ID_T        win_id, GUI_POINT_T  bg_point)
{
    GUI_LCD_DEV_INFO     lcd_dev_info    = {0};
    SCI_TIME_T           sys_time        = {0};
    GUI_RECT_T           bg_rect         = {0};
    uint16               bg_height       = 0;
    uint16               bg_width        = 0;
    //MMI_IMAGE_ID_T       icon_id         = 0;
    uint8 time_num = 0;
    MMISET_TIME_DISPLAY_TYPE_E  time_display_type = MMIAPISET_GetTimeDisplayType();
    BOOLEAN              b_am            = TRUE;
    
    //get system time
    TM_GetSysTime(&sys_time);
    
        //12小时制
    if(time_display_type == MMISET_TIME_12HOURS)
    {
        b_am = Get12HoursTypeTime(&sys_time);
    }

    MMK_GetWinLcdDevInfo(win_id,&lcd_dev_info);
    
    if (UILAYER_IsLayerActive(&lcd_dev_info))
    {
        UILAYER_Clear(&lcd_dev_info);//NEWMS00197486
    }
    else
    {
        return;
    }

    if (GUIRES_GetImgWidthHeight(&bg_width, &bg_height, g_weather_widget.large_bg_img, win_id))
    {
        GUI_POINT_T      start_point  = {0};
        uint16           need_dis_bg  = 0;
        bg_rect.left   = bg_point.x;
        bg_rect.top    = bg_point.y;
        bg_rect.right  = bg_rect.left + bg_width;
        bg_rect.bottom = bg_rect.top + bg_height;

        need_dis_bg    = ParserNvValue(MMIWIDGET_WEATHER_BG_TEXTCOLOUR_SET_NV_ITEM_BG, &s_cur_bg_textcolour_set_info);
        
        if (1 == need_dis_bg)
        {
            //显示背景图
            GUIRES_DisplayImg(&bg_point, PNULL, PNULL, win_id, g_weather_widget.large_bg_img, &lcd_dev_info);
        }

        //打开时间翻页特效时，这里不执行显示时间的动作。
        //字符的x坐标画一个字符变更一次
		//hour1
        start_point.y = bg_point.y + MMIWIDGET_BARWEATHER_TIME_IMG_START_Y;                  
        start_point.x = bg_point.x + MMIWIDGET_BARWEATHER_TIME_IMG_HOUR_START_X;
		time_num = 	sys_time.hour / 10;	
		start_point.x = BarPhoneWeather_DisplayTimeOneNum(&lcd_dev_info, start_point, time_num);
		  
		//hour2
		start_point.x += 1;
		time_num = 	sys_time.hour % 10;
		start_point.x = BarPhoneWeather_DisplayTimeOneNum(&lcd_dev_info, start_point, time_num); 
		//min1
		start_point.x += MMIWIDGET_BARWEATHER_TIME_SPACE;
		time_num = sys_time.min / 10;
		start_point.x = BarPhoneWeather_DisplayTimeOneNum(&lcd_dev_info, start_point, time_num); 
		//min2
		start_point.x += 1;
		time_num = sys_time.min % 10;
		BarPhoneWeather_DisplayTimeOneNum(&lcd_dev_info, start_point, time_num);                      

        //AM/PM
        if(time_display_type == MMISET_TIME_12HOURS)
        {
            DrawAmPmString(b_am,start_point,&lcd_dev_info);
        }

        {
            GUISTR_STYLE_T text_style = {0};
            MMI_STRING_T   tempStr = {0};
            GUISTR_STATE_T text_state = GUISTR_STATE_ALIGN|GUISTR_STATE_EFFECT|GUISTR_STATE_EFFECT|GUISTR_STATE_ELLIPSIS;
            GUI_RECT_T     text_rect  = {0};

          //temp text
            start_point.x = bg_point.x + MMIWIDGET_BARWEATHER_TEXT_START_X;
            start_point.y = bg_point.y + MMIWIDGET_BARWEATHER_TEXT_START_Y;

            text_style.effect = FONT_EFFECT_REVERSE_EDGE;
            text_style.angle = ANGLE_0;
            text_style.align = ALIGN_LVMIDDLE;
            text_style.font = MMIWIDGET_WEATHER_TEXT_FONT;
            text_style.font_color = MMIWIDGET_WEATHER_TEXT_COLOR;
            
            text_rect.left   = start_point.x;
            text_rect.top    = start_point.y;
            text_rect.right  = text_rect.left + MMIWIDGET_BARWEATHER_TEXT_TEMP_WIDTH;
            text_rect.bottom = text_rect.top + MMIWIDGET_BARWEATHER_TEXT_HEIGHT;
            GUISTR_DrawTextToLCDInRect(&lcd_dev_info, &text_rect, &text_rect, &s_cur_temprange_str,	&text_style, text_state, GUISTR_TEXT_DIR_AUTO);
            //date text		
            text_style.align = ALIGN_RVMIDDLE;
            start_point.x = text_rect.right;
			 
            text_rect.left  = start_point.x;
            text_rect.right = text_rect.left + MMIWIDGET_BARWEATHER_TEXT_DATE_WIDTH;
            SCI_MEMSET(&tempStr, 0x00, sizeof(MMI_STRING_T));
            GetDisplayText(&tempStr, MMIWIDGET_WEATHER_DISPALY_TEXT_TYPE_DATE);
            GUISTR_DrawTextToLCDInRect(&lcd_dev_info, &text_rect, &text_rect, &tempStr,	&text_style, text_state, GUISTR_TEXT_DIR_AUTO);    

            //condition text		
            text_style.align = ALIGN_LVMIDDLE;
            start_point.x = bg_point.x + MMIWIDGET_BARWEATHER_TEXT_START_X;
            start_point.y = text_rect.bottom + MMIWIDGET_BARWEATHER_TEXT_SPACE;
	 
            text_rect.top    = start_point.y;
            text_rect.bottom = text_rect.top + MMIWIDGET_BARWEATHER_TEXT_HEIGHT;
            text_rect.left = start_point.x;
            text_rect.right = text_rect.left + MMIWIDGET_BARWEATHER_TEXT_CONDITION_WIDTH;
            GUISTR_DrawTextToLCDInRect(&lcd_dev_info, &text_rect, &text_rect, &s_cur_condition_str,	&text_style, text_state, GUISTR_TEXT_DIR_AUTO);
            //city text
            text_style.align = ALIGN_RVMIDDLE;
//             text_style.font = MMIWIDGET_WEATHER_CITY_TEXT_FONT;

            start_point.x = text_rect.right;	

            text_rect.left  = start_point.x;
            text_rect.right = text_rect.left + MMIWIDGET_BARWEATHER_TEXT_CITY_WIDTH;
            GUISTR_DrawTextToLCDInRect(&lcd_dev_info, &text_rect, &text_rect, &s_cur_city_str,	&text_style, text_state, GUISTR_TEXT_DIR_AUTO);                
        }
    }    
}
LOCAL void BarPhoneWeather_Redraw(MMI_WIN_ID_T     win_id)
{
    GUI_RECT_T win_rect = {0};
    GUI_POINT_T      point={0};
    
    MMK_GetWinRect(win_id, &win_rect);

    point.x = win_rect.left;
    point.y = win_rect.top;
    
    BarPhoneWeather_Display_Time_WeatherInfo(win_id, point); 
}
#endif

LOCAL MMI_RESULT_E BarPhoneWeather_HandleWebKey(void)
{
    MMI_RESULT_E recode   = MMI_RESULT_TRUE;
    switch(cur_webkey_condition)
    {
    case NEED_ADD_CITY:
        MMIWEATHER_OpenMainWin();
        break;

    case NEED_UPDATE_WEATHER:
        UpdateWeather();
        break;

    case ENTER_WEATHER_WIN:
        MMIWEATHER_OpenMainWin();
        break;

    default:
        recode   = MMI_RESULT_FALSE;
        break;
    }    
    return recode;
}
LOCAL void BarPhoneWeather_SetWebKeyCondition(void)
{
    MMIWEATHER_SETTING_T* setting_ptr = MMIWEATHER_GetSettingInfo();
    MMIWEATHER_WEATHER_T*   weather_info = MMIWEATHER_GetCurCityWeatherInfo();
       
    if(setting_ptr->city_num == 0)
        cur_webkey_condition = NEED_ADD_CITY;
    else if (weather_info->current.condition == WEATHER_CONDITION_UNKNOW)
        cur_webkey_condition = NEED_UPDATE_WEATHER;
    else
        cur_webkey_condition = ENTER_WEATHER_WIN;
}
LOCAL void BarPhoneWeather_DisplaySoftkey(MMI_WIN_ID_T    win_id, BOOLEAN is_update)
{
    switch(cur_webkey_condition)
    {
        case NEED_ADD_CITY:
            MMIWIDGET_SoftkeySetTextId(win_id, STXT_ADD, is_update);
            break;
        case NEED_UPDATE_WEATHER:
            MMIWIDGET_SoftkeySetTextId(win_id, TXT_COMMON_UPDATE, is_update);
            break;
        case ENTER_WEATHER_WIN:
            MMIWIDGET_SoftkeySetTextId(win_id, TXT_VIEW, is_update);
            break;
        default:
            break;

    }
} 


#if defined MMIWIDGET_WEATHER_SE 
/*****************************************************************************/
//  Discription: Darw Week string And Temp string
//  Global resource dependence: None
//  Author: juan.wu
//  Note : 
/*****************************************************************************/
LOCAL void DrawWeekAndTemp(MMIWIDGET_WEATHER_SE_E se_index,
                            GUI_RECT_T bg_rect,
                            GUI_LCD_DEV_INFO  *dev_info_ptr)
{
    GUI_RECT_T     week_rect                  = {0};
    GUI_RECT_T     icon_rect                  = {0};
    MMI_STRING_T   week_str                   = {0};
    GUISTR_STYLE_T font_style                 = {0};
    GUISTR_STATE_T font_state                 = GUISTR_STATE_ELLIPSIS|GUISTR_STATE_SINGLE_LINE;
    uint32         week_id                    = 0;
    MMIWEATHER_WEATHER_T    *weather_info_ptr = PNULL;
    MMIWEATHER_SETTING_T*    setting_info_ptr = PNULL;
    GUISTR_INFO_T  str_info                   = {0};
    GUI_RECT_T     temp_rect                  = {0};
    MMI_STRING_T   temp_str                   = {0};
    MMIWEATHER_CONDITION_E  condition         = {0};
    int32          low_temp                   = 0;
    int32          high_temp                  = 0;
    wchar*         date_ptr                   = PNULL;
    wchar          wstr[WEATHER_MAX_STRING_LEN + 1]    = {0};
    MMI_IMAGE_ID_T weather_icon               = 0;
    uint16         icon_w                     = 0;
    uint16         icon_h                     = 0;
    uint8           day                       = 0;
    MMIWEATHER_CONDITION_E condition_id = WEATHER_CONDITION_UNKNOW;
    MMI_TEXT_ID_T  week_text_id[]             = {TXT_NULL,                                                
                                                TXT_WEATHER_WEEK_MON,
                                                TXT_WEATHER_WEEK_TUE,
                                                TXT_WEATHER_WEEK_WED,
                                                TXT_WEATHER_WEEK_THU,
                                                TXT_WEATHER_WEEK_FRI,
                                                TXT_WEATHER_WEEK_SAT,
                                                TXT_WEATHER_WEEK_SUN,
                                                TXT_NULL};


    weather_info_ptr = MMIWEATHER_GetCurCityWeatherInfo();
    setting_info_ptr = MMIWEATHER_GetSettingInfo();
    if(PNULL == weather_info_ptr || PNULL == setting_info_ptr)
    {
        SCI_TRACE_LOW("DarwWeekAndTemp weather_info_ptr = %x,setting_info_ptr = %x",
                        weather_info_ptr,setting_info_ptr);
        return;
    }
    if(setting_info_ptr->city_num != 0
        &&setting_info_ptr->cur_city >= setting_info_ptr->city_num)
    {
        SCI_TRACE_LOW("DarwWeekAndTemp cur_city = %d",setting_info_ptr->cur_city);
        return;
    }
    font_style.align      = ALIGN_HVMIDDLE;
    font_style.font       = MMI_DEFAULT_SMALL_FONT;
    font_style.font_color = MMI_WHITE_COLOR;

    day = (se_index - MMIWIDGET_WEATHER_SE_FIRST_DAY);/*lint !e656 */
    MMIWEATHER_GetFCId(&weather_info_ptr->forecast, 
                        day, 
                        &week_id, 
                        &condition, 
                        &low_temp, 
                        &high_temp, 
                        &date_ptr);
    //display bg
    GUIRES_DisplayImg(PNULL, 
                        &bg_rect, 
                        PNULL, 
                        g_weather_widget.win_id, 
                        IMAGE_WIDGET_WEATHER_FORCAST_BG, 
                        dev_info_ptr);
    if(0 == setting_info_ptr->city_num)
    {
        SCI_DATE_T      date             = {0};
        MMI_TEXT_ID_T  na_week_text_id[] = {
                                        TXT_WEATHER_WEEK_SUN,
                                        TXT_WEATHER_WEEK_MON,
                                        TXT_WEATHER_WEEK_TUE,
                                        TXT_WEATHER_WEEK_WED,
                                        TXT_WEATHER_WEEK_THU,
                                        TXT_WEATHER_WEEK_FRI,
                                        TXT_WEATHER_WEEK_SAT
                                        };

        TM_GetSysDate(&date);
        date.wday += day;
        if(date.wday >= MMI_WEEK_TOTAL_DAY)
        {
            date.wday -= MMI_WEEK_TOTAL_DAY;
        }
        if(date.wday < MMI_WEEK_TOTAL_DAY)
        {
            MMI_GetLabelTextByLang(na_week_text_id[date.wday],&week_str); 
        }
        else
        {
            SCI_TRACE_LOW("DrawWeekAndTemp date.wday = %d",date.wday);
            return;
        }
    }
    else
    {
        MMI_GetLabelTextByLang(week_text_id[week_id], &week_str);
    }

    GUISTR_GetStringInfo(&font_style, &week_str, font_state, &str_info);
    //display week string
    week_rect = bg_rect;
    week_rect.bottom = week_rect.top + str_info.height;
    
    GUISTR_DrawTextToLCDInRect(dev_info_ptr, 
                                &week_rect, 
                                &week_rect, 
                                &week_str, 
                                &font_style, 
                                font_state, 
                                GUISTR_TEXT_DIR_AUTO); 
    //display weather icon
    icon_rect = bg_rect;
    icon_rect.top = week_rect.bottom + 1;
    switch(se_index)
    {
    case MMIWIDGET_WEATHER_SE_FIRST_DAY:
        condition_id = weather_info_ptr->forecast.weather1;
        break;

    case MMIWIDGET_WEATHER_SE_SECOND_DAY:
        condition_id = weather_info_ptr->forecast.weather2;
        break;

    case MMIWIDGET_WEATHER_SE_THIRD_DAY:
        condition_id = weather_info_ptr->forecast.weather3;
        break;

    default:
        break;
    }
    weather_icon = MMIWEATHER_GetConditionImgId(condition_id,WEATHER_ICON_SMALL);
    GUIRES_GetImgWidthHeight(&icon_w, &icon_h, weather_icon, g_weather_widget.win_id);
    icon_rect.left = (icon_rect.right + icon_rect.left)/2 - icon_w/2;

    GUIRES_DisplayImg(PNULL, 
                        &icon_rect, 
                        PNULL, 
                        g_weather_widget.win_id, 
                        weather_icon, 
                        dev_info_ptr);
    //display temp str
    temp_str.wstr_ptr = wstr;
#ifdef MMI_WEATHER_TEST
    if(0 == setting_info_ptr->city_num)
    {
        low_temp = 59;
        high_temp = 77;
    }
    MMIAPIWEATHER_GetTemperatureRangeString(low_temp, high_temp, &temp_str,FALSE);
#else
    if(0 == setting_info_ptr->city_num)
    {
        wstr[0] = 0x003f;
        temp_str.wstr_len = 1;
    }
    else
    {
        MMIAPIWEATHER_GetTemperatureRangeString(low_temp, high_temp, &temp_str,FALSE);
    }
#endif
    GUISTR_GetStringInfo(&font_style, &temp_str, font_state, &str_info);
    temp_rect = bg_rect;
    temp_rect.top = bg_rect.bottom - str_info.height;

    font_style.font = SONG_FONT_12;
    GUISTR_DrawTextToLCDInRect(dev_info_ptr, 
                                &temp_rect, 
                                &temp_rect, 
                                &temp_str, 
                                &font_style, 
                                font_state, 
                                GUISTR_TEXT_DIR_AUTO); 
}

/*****************************************************************************/
//  Discription: DarwCurCityWeatherInfo
//  Global resource dependence: None
//  Author: juan.wu
//  Note : 
/*****************************************************************************/
LOCAL void DrawCurCityWeatherInfo(GUI_RECT_T bg_rect,GUI_LCD_DEV_INFO  *dev_info_ptr)
{
    MMIWEATHER_WEATHER_T*    weather_info_ptr = PNULL;
    MMIWEATHER_SETTING_T*    setting_info_ptr = PNULL;
    MMI_STRING_T            city_name         = {0};
    MMI_STRING_T            city_weather      = {0};     
    GUI_RECT_T              text_rect         = {0};
    GUISTR_STYLE_T          font_style        = {0};
    GUISTR_STATE_T          font_state        = GUISTR_STATE_ELLIPSIS|GUISTR_STATE_SINGLE_LINE; 
    GUISTR_INFO_T           str_info          = {0};
    
    weather_info_ptr = MMIWEATHER_GetCurCityWeatherInfo();
    setting_info_ptr = MMIWEATHER_GetSettingInfo();
    if(PNULL == weather_info_ptr || PNULL == setting_info_ptr)
    {
        SCI_TRACE_LOW("DarwCurCityWeatherInfo weather_info_ptr = %x,setting_info_ptr = %x",
                        weather_info_ptr,setting_info_ptr);
        return;
    }
    if(setting_info_ptr->city_num != 0
        &&setting_info_ptr->cur_city >= setting_info_ptr->city_num)
    {
        SCI_TRACE_LOW("DrawCurCityWeatherInfo cur_city = %d",setting_info_ptr->cur_city);
        return;
    }

    font_style.align      = ALIGN_HVMIDDLE;
    font_style.font       = SONG_FONT_12;
    font_style.font_color = MMI_WHITE_COLOR;

    //display bg
    GUIRES_DisplayImg(PNULL, 
                        &bg_rect, 
                        &bg_rect, 
                        g_weather_widget.win_id, 
                        IMAGE_WIDGET_WEATHER_FORCAST_BG, 
                        dev_info_ptr);
    //display city name
    if(0 == setting_info_ptr->city_num)
    {
        city_name.wstr_ptr = L"Unkonw city";
        city_name.wstr_len = MMIAPICOM_Wstrlen(city_name.wstr_ptr);
    }
    else
    {
        city_name.wstr_ptr = weather_info_ptr->city_name;
        city_name.wstr_len = MMIAPICOM_Wstrlen(city_name.wstr_ptr);
    }
    text_rect = bg_rect;
    GUISTR_GetStringInfo(&font_style, &city_name, font_state, &str_info);
    text_rect.bottom = bg_rect.top + str_info.height;
    GUISTR_DrawTextToLCDInRect(dev_info_ptr, 
                                &text_rect, 
                                &text_rect, 
                                &city_name, 
                                &font_style, 
                                font_state, 
                                GUISTR_TEXT_DIR_AUTO); 
    //display city weather info
    if(0 == setting_info_ptr->city_num)
    {
        city_weather.wstr_ptr = L"Unkonw weather";
        city_weather.wstr_len = MMIAPICOM_Wstrlen(city_weather.wstr_ptr);
    }
    else
    {
        MMI_GetLabelTextByLang(MMIWEATHER_GetConditionTxtId(weather_info_ptr->current.condition),
                                &city_weather);
    }
    text_rect.top = text_rect.bottom + 1;
	text_rect.bottom = bg_rect.bottom;
    GUISTR_DrawTextToLCDInRect(dev_info_ptr,  
                                &text_rect, 
                                &text_rect, 
                                &city_weather, 
                                &font_style, 
                                font_state, 
                                GUISTR_TEXT_DIR_AUTO); 
}
/*****************************************************************************/
//  Discription: DarwCityChangeInfo
//  Global resource dependence: None
//  Author: juan.wu
//  Note : display as cur_city_index/total_city_num
/*****************************************************************************/
LOCAL void DrawCityChangeInfo(GUI_RECT_T bg_rect,GUI_LCD_DEV_INFO  *dev_info_ptr)
{
    MMIWEATHER_SETTING_T*  setting_info_ptr = PNULL;
    MMI_STRING_T           city_change_info = {0};
    wchar                  str[3+1] = {0};
    char                   num[4]   = {0};
    GUISTR_STYLE_T         font_style        = {0};
    GUISTR_STATE_T         font_state        = GUISTR_STATE_ELLIPSIS|GUISTR_STATE_SINGLE_LINE;
    GUISTR_INFO_T          str_info          = {0};

    setting_info_ptr = MMIWEATHER_GetSettingInfo();
    if(PNULL == setting_info_ptr)
    {
        SCI_TRACE_LOW("DarwCityChangeInfo PNULL == setting_info_ptr");
        return;
    }
    if(setting_info_ptr->city_num != 0
        &&setting_info_ptr->cur_city >= setting_info_ptr->city_num)
    {
        SCI_TRACE_LOW("DrawCityChangeInfo cur_city = %d",setting_info_ptr->cur_city);
        return;
    }
    //display bg
    GUIRES_DisplayImg(PNULL, 
                        &bg_rect, 
                        &bg_rect, 
                        g_weather_widget.win_id, 
                        IMAGE_WIDGET_WEATHER_CITY_CHANGE_BG, 
                        dev_info_ptr);
    //display str
    if(0 == setting_info_ptr->city_num)
    {
        sprintf(num, "%d/%d", setting_info_ptr->cur_city , setting_info_ptr->city_num);;
    }
    else
    {
        sprintf(num, "%d/%d", setting_info_ptr->cur_city + 1, setting_info_ptr->city_num);
    }
    MMIAPICOM_StrToWstr((uint8 *)num, str);
    city_change_info.wstr_ptr = str;
    city_change_info.wstr_len = MMIAPICOM_Wstrlen(city_change_info.wstr_ptr);

    font_style.align      = ALIGN_HVMIDDLE;
    font_style.font       = SONG_FONT_12;
    font_style.font_color = MMI_WHITE_COLOR;

    GUISTR_GetStringInfo(&font_style, &city_change_info, font_state, &str_info);

    GUISTR_DrawTextToLCDInRect(dev_info_ptr, 
                                &bg_rect, 
                                &bg_rect, 
                                &city_change_info, 
                                &font_style, 
                                font_state, 
                                GUISTR_TEXT_DIR_AUTO); 
}

/*****************************************************************************/
//  Discription: BarPhone_GetWeatherState
//  Global resource dependence: None
//  Author: juan.wu
//  Note : change weather condition to sepecial effect state
/*****************************************************************************/
LOCAL WEATHER_STATUS_E BarPhone_GetWeatherState(void)
{
    MMIWEATHER_CONDITION_E  cur_condition = WEATHER_CONDITION_UNKNOW;
    WEATHER_STATUS_E        weather_state = WEATHER_STATE_NONE;
    MMIWEATHER_WEATHER_T    *weather_info_ptr = PNULL;
    MMIWEATHER_SETTING_T*  setting_info_ptr = PNULL;
    weather_info_ptr = MMIWEATHER_GetCityWeatherInfo(MMIWEATHER_GetCurCity());
    setting_info_ptr = MMIWEATHER_GetSettingInfo();

    if(PNULL == setting_info_ptr|| PNULL == weather_info_ptr)
    {
        SCI_TRACE_LOW("BarPhone_GetWeatherState setting_info_ptr =%x, weather_info_ptr =%x",
                        setting_info_ptr,weather_info_ptr);
        return WEATHER_STATE_NA;
    }
    if(0 == setting_info_ptr->city_num)
    {
        SCI_TRACE_LOW("BarPhone_GetWeatherState 0 == setting_info_ptr->city_num");
        return WEATHER_STATE_NA;
    }
    if(setting_info_ptr->city_num != 0
        &&setting_info_ptr->cur_city >= setting_info_ptr->city_num)
    {
        SCI_TRACE_LOW("BarPhone_GetWeatherState cur_city = %d",setting_info_ptr->cur_city);
        return WEATHER_STATE_NA;
    }
    cur_condition  = weather_info_ptr->current.condition;

    switch(cur_condition)
    {
    case WEATHER_CONDITION_SUNNY:
    case WEATHER_CONDITION_CLEAR:
        weather_state = WEATHER_STATE_SUNNY;
        break;
        
    case WEATHER_CONDITION_CLOUDY:
    case WEATHER_CONDITION_PARTLY_CLOUDY:
    case WEATHER_CONDITION_MOSTLY_CLOUDY:
    case WEATHER_CONDITION_OVERCAST:
        weather_state = WEATHER_STATE_CLOUDY;
        break;

    case WEATHER_CONDITION_RAIN:
    case WEATHER_CONDITION_LIGHT_RAIN:
    case WEATHER_CONDITION_RAIN_SHOWERS:
    case WEATHER_CONDITION_SHOWERS:
    case WEATHER_CONDITION_SCATTERED_SHOWERS:
    case WEATHER_CONDITION_STORM:
    case WEATHER_CONDITION_CHANCE_OF_RAIN:
    case WEATHER_CONDITION_CHANCE_OF_SHOWERS:
    case WEATHER_CONDITION_CHANCE_OF_STORM:
        weather_state = WEATHER_STATE_RAINNY;
        break;

    case WEATHER_CONDITION_SLEET:
    case WEATHER_CONDITION_ICY:
        weather_state = WEATHER_STATE_SNOWYANDRAINY;
        break;

    case WEATHER_CONDITION_FLURRIES:
    case WEATHER_CONDITION_SNOW:
    case WEATHER_CONDITION_CHANCE_OF_SNOW:
        weather_state = WEATHER_STATE_SNOWY;
        break;

    case WEATHER_CONDITION_MOSTLY_SUNNY:
    case WEATHER_CONDITION_PARTLY_SUNNY:
        weather_state = WEATHER_STATE_SUNCHANGE;
        break;

    case WEATHER_CONDITION_THUNDERSTORM:
    case WEATHER_CONDITION_CHANCE_OF_TSTORM:
    case WEATHER_CONDITION_SCATTERED_THUNDERSTORMS:
        weather_state = WEATHER_STATE_LIGHTING;
        break;

    case WEATHER_CONDITION_FOG:
    case WEATHER_CONDITION_HAZE:
    case WEATHER_CONDITION_MIST:
    case WEATHER_CONDITION_SMOKE:
    case WEATHER_CONDITION_DUST:
        weather_state = WEATHER_STATE_FOGGY;
        break;
    
    default:
        weather_state = WEATHER_STATE_NA;
        break;
    }
    return weather_state;
}
/*****************************************************************************/
//  Discription: DrawCurTempreture
//  Global resource dependence: None
//  Author: juan.wu
//  Note : draw current day curren temp(big temp)
/*****************************************************************************/
LOCAL void DrawCurTempreture(                            
                                GUI_RECT_T        bg_rect,
                                GUI_LCD_DEV_INFO  *dev_info_ptr
                                )
{
    MMIWEATHER_WEATHER_T*    weather_info_ptr = PNULL;
    MMIWEATHER_SETTING_T*    setting_info_ptr = PNULL;
    MMI_STRING_T             temp_string      = {0};
    GUISTR_STYLE_T          font_style        = {0};
    GUISTR_STATE_T          font_state        = GUISTR_STATE_EFFECT|GUISTR_STATE_ELLIPSIS|GUISTR_STATE_SINGLE_LINE; 
    wchar wstr[WEATHER_MAX_STRING_LEN + 1]    = {0};
    char temp_str[WEATHER_MAX_STRING_LEN + 1] = {""};
    int32                   temp              = 0;
    MMI_STRING_T            unit_str          = {0};
    GUI_RECT_T              string_rect       = {0};
    GUISTR_INFO_T           str_info          = {0};
    
    weather_info_ptr = MMIWEATHER_GetCurCityWeatherInfo();
    setting_info_ptr = MMIWEATHER_GetSettingInfo();
    if(PNULL == weather_info_ptr || PNULL == setting_info_ptr)
    {
        SCI_TRACE_LOW("DrawCurTempreture weather_info_ptr = %x,setting_info_ptr = %x",
                        weather_info_ptr,setting_info_ptr);
        return;
    }
    if(setting_info_ptr->city_num != 0
        &&setting_info_ptr->cur_city >= setting_info_ptr->city_num)
    {
        SCI_TRACE_LOW("DrawCurTempreture cur_city = %d",setting_info_ptr->cur_city);
        return;
    }
    //display bg
    GUIRES_DisplayImg(PNULL, 
                        &bg_rect, 
                        &bg_rect, 
                        g_weather_widget.win_id, 
                        IMAGE_WIDGET_WEATHER_FORCAST_BG, 
                        dev_info_ptr);
    //display str
    temp_string.wstr_ptr = wstr;
    if(0 == setting_info_ptr->city_num)
    {
#ifdef MMI_WEATHER_TEST
        temp = 77;
#endif
    }
    else
    {
        temp = weather_info_ptr->current.temp_f;
    }
    // 转换成摄氏温度
    if (WEATHER_TEMP_UNIT_C == setting_info_ptr->temp_unit)
    {
        temp = MMIWEATHER_GetTemperature(TEMP_F2C(temp));
    }
    
    // 获得数字的字符串
    sprintf((char*)temp_str, "%d", temp);
    
    temp_string.wstr_len = strlen((char *)temp_str);
    // 转换成宽字符
    MMI_STRNTOWSTR(temp_string.wstr_ptr,
        WEATHER_MAX_STRING_LEN,
        (uint8*)temp_str,
        WEATHER_MAX_STRING_LEN,
        temp_string.wstr_len);
    //set font style
    font_style.font_color = MMI_WHITE_COLOR;
    font_style.font       = SONG_FONT_38;
    font_style.align      = ALIGN_HVMIDDLE;
    font_style.effect     = FONT_EFFECT_CUSTOM;
    font_style.edge_color = 0x0841*20;

    GUISTR_GetStringInfo(&font_style, &temp_string, font_state, &str_info);
    
    string_rect = bg_rect;
    GUISTR_DrawTextToLCDInRect(dev_info_ptr, 
                            &string_rect, 
                            &string_rect, 
                            &temp_string, 
                            &font_style, 
                            font_state, 
                            GUISTR_TEXT_DIR_AUTO); 
    //draw unit char
    MMI_GetLabelTextByLang((WEATHER_TEMP_UNIT_C == setting_info_ptr->temp_unit) ? 
                            TXT_WEATHER_CEL_SIGNAL : TXT_WEATHER_FAH_SIGNAL, &unit_str);

    string_rect.left = (bg_rect.left + bg_rect.right + str_info.width)/2;
    string_rect.top  = (bg_rect.top  + bg_rect.bottom - str_info.height)/2;
    string_rect.right = bg_rect.right;
    string_rect.bottom = string_rect.top + str_info.height;
    font_style.font       = SONG_FONT_20;
    font_style.align      = ALIGN_TOP;

    GUISTR_DrawTextToLCDInRect(dev_info_ptr, 
                            &string_rect, 
                            &string_rect, 
                            &unit_str, 
                            &font_style, 
                            font_state, 
                            GUISTR_TEXT_DIR_AUTO); 
}
/*****************************************************************************/
//  Discription: DrawCurDayLowHighTemp
//  Global resource dependence: None
//  Author: juan.wu
//  Note : draw current day low and high temp
/*****************************************************************************/
LOCAL void DrawCurDayLowHighTemp(MMIWIDGET_WEATHER_SE_E se_index,
                            GUI_RECT_T        bg_rect,
                            GUI_LCD_DEV_INFO  *dev_info_ptr)
{
    MMIWEATHER_WEATHER_T*    weather_info_ptr = PNULL;
    MMIWEATHER_SETTING_T*    setting_info_ptr = PNULL;
    MMI_STRING_T             temp_string      = {0};
    GUISTR_STYLE_T          font_style        = {0};
    GUISTR_STATE_T          font_state        = GUISTR_STATE_ELLIPSIS|GUISTR_STATE_SINGLE_LINE; 
    wchar wstr[WEATHER_MAX_STRING_LEN + 1] = {0};
    int32                   temp = 0;

    weather_info_ptr = MMIWEATHER_GetCurCityWeatherInfo();
    setting_info_ptr = MMIWEATHER_GetSettingInfo();
    if(PNULL == weather_info_ptr || PNULL == setting_info_ptr)
    {
        SCI_TRACE_LOW("DrawCurDayLowHighTemp weather_info_ptr = %x,setting_info_ptr = %x",
                        weather_info_ptr,setting_info_ptr);
        return;
    }
    if(setting_info_ptr->city_num != 0
        &&setting_info_ptr->cur_city >= setting_info_ptr->city_num)
    {
        SCI_TRACE_LOW("DrawCurDayLowHighTemp cur_city = %d",setting_info_ptr->cur_city);
        return;
    }
    //display bg
    GUIRES_DisplayImg(PNULL, 
                        &bg_rect, 
                        &bg_rect, 
                        g_weather_widget.win_id, 
                        IMAGE_WIDGET_WEATHER_FORCAST_BG, 
                        dev_info_ptr);
    
    //display str
    font_style.font_color = MMI_WHITE_COLOR;
    temp_string.wstr_ptr = wstr;
    switch(se_index)
    {
    case MMIWIDGET_WEATHER_SE_UP_TEMP:
        temp = weather_info_ptr->forecast.low1;//up
        font_style.font       = SONG_FONT_12;
        font_style.align      = ALIGN_RVMIDDLE;
        break;

    case MMIWIDGET_WEATHER_SE_DOWN_TEMP:
        temp = weather_info_ptr->forecast.high1;//down
        font_style.font       = SONG_FONT_12;
        font_style.align      = ALIGN_RVMIDDLE;
        break;

    default:
        break;
    }
#ifdef MMI_WEATHER_TEST
    if(0 == setting_info_ptr->city_num)
    {
        temp = 77;
    }
#endif
    MMIAPIWEATHER_GetTemperatureString(temp,&temp_string);
    SCI_TRACE_LOW("DrawCurDayLowHighTemp city_num = %d",setting_info_ptr->city_num);
    GUISTR_DrawTextToLCDInRect(dev_info_ptr, 
                            &bg_rect, 
                            &bg_rect, 
                            &temp_string, 
                            &font_style, 
                            font_state, 
                            GUISTR_TEXT_DIR_AUTO); 
  

}
/*****************************************************************************/
//  Discription: DisplayImgFilter
//  Global resource dependence: None
//  Author: juan.wu
//  Note : if the case has been processed,return FALSE;
//         else it is need diaplay img,return TRUE
/*****************************************************************************/
LOCAL BOOLEAN DisplayImgFilter(
                                MMIWIDGET_WEATHER_SE_E se_index,
                                GUI_RECT_T             bg_rect,
                                GUI_LCD_DEV_INFO  *dev_info_ptr
                                )
{
    BOOLEAN result = FALSE;
    
    
    switch(se_index)
    {
    case MMIWIDGET_WEATHER_SE_BIG_TEMP:
        DrawCurTempreture(bg_rect, dev_info_ptr);
        break;
        
    case MMIWIDGET_WEATHER_SE_UP_TEMP:
    case MMIWIDGET_WEATHER_SE_DOWN_TEMP:
        DrawCurDayLowHighTemp(se_index,bg_rect,dev_info_ptr);
        break;

    case MMIWIDGET_WEATHER_SE_CITY_CHANGE:
        DrawCityChangeInfo(bg_rect,dev_info_ptr);
        break;

    case MMIWIDGET_WEATHER_SE_WEATHER_CHANGE:
        DrawCurCityWeatherInfo(bg_rect,dev_info_ptr);
        break;

    case MMIWIDGET_WEATHER_SE_FIRST_DAY:
    case MMIWIDGET_WEATHER_SE_SECOND_DAY:
    case MMIWIDGET_WEATHER_SE_THIRD_DAY:
        DrawWeekAndTemp(se_index,bg_rect,dev_info_ptr);
        break;

    default:
        result = TRUE;
        break;      
    } 
    return result;
}
/*****************************************************************************/
//  Discription: BarPhone_CreateImageBuffer
//  Global resource dependence: None
//  Author: ying.xu
//  Note : 
/*****************************************************************************/
LOCAL int32 BarPhone_CreateImageBuffer(MMI_WIN_ID_T    win_id, MMIWIDGET_SPECIAL_EFFECT_T *p_widget_se)
{    
    MMIWIDGET_WEATHER_SE_E i = 0;
    int32 resource_num = sizeof(s_weather_t.resource_array) / sizeof(s_weather_t.resource_array.sun);
    uint16 image_width = 0;
    uint16 image_height = 0;
    UILAYER_CREATE_T create_info = {0};
    GUI_LCD_DEV_INFO layer_handle = {0};
    uint8* p_buffer = 0;
    BMP_OBJ *p_bmp_obj = 0;    
    uint8*      p_layer_buffer = 0;
    uint32      buffer_size = 0;
    MMI_WIN_ID_T parent_id = MMK_GetParentWinHandle(win_id);
    GUI_RECT_T  rect = {0};
	//这个数组的顺序和大小必须和WEATHER_RESOURCE_STRUCT_T保持一致
    MMI_IMAGE_ID_T image_array[] ={
                                    IMAGE_WIDGET_WEATHER_SE_SUN             ,  
                                    IMAGE_WIDGET_WEATHER_SE_SUN_BG          ,  
                                    IMAGE_WIDGET_WEATHER_SE_BIG_TEMPERATURE ,
                                    IMAGE_WIDGET_WEATHER_SE_UP_TEMPERATURE ,        
                                    IMAGE_WIDGET_WEATHER_SE_DOWN_TEMPERATURE ,
                                    IMAGE_WIDGET_WEATHER_SE_GLOW            ,    
                                    IMAGE_WIDGET_WEATHER_CITY_CHANGE_BG     ,
                                    IMAGE_WIDGET_WEATHER_SE_WEATHER_CHANGE  ,
                                    IMAGE_WIDGET_WEATHER_SE_FIRST_DAY       ,    
                                    IMAGE_WIDGET_WEATHER_SE_FIRST_DAY      ,  
                                    IMAGE_WIDGET_WEATHER_SE_FIRST_DAY       ,   
                                    IMAGE_WIDGET_WEATHER_SE_BG              ,  
                                    IMAGE_WIDGET_WEATHER_SE_DIVIDE_BAR      ,                                
                                    IMAGE_WIDGET_WEATHER_SE_CLOUD1          ,      
                                    IMAGE_WIDGET_WEATHER_SE_CLOUD2           ,   
                                    IMAGE_WIDGET_WEATHER_SE_CLOUD_LEFT       ,   
                                    IMAGE_WIDGET_WEATHER_SE_CLOUD_RIGHT      ,   
                                    IMAGE_WIDGET_WEATHER_SE_NONE             , 
                                    IMAGE_WIDGET_WEATHER_SE_RAIN             ,   
                                    IMAGE_WIDGET_WEATHER_SE_FOG              ,                                   
                                    IMAGE_WIDGET_WEATHER_SE_LIGHT1           ,   
                                    IMAGE_WIDGET_WEATHER_SE_LIGHT2           ,   
                                    IMAGE_WIDGET_WEATHER_SE_SNOW_BIG         ,   
                                    IMAGE_WIDGET_WEATHER_SE_SNOW_SMALL               
                                    };

    //SCI_ASSERT(resource_num == sizeof(image_array) / sizeof(image_array[0]));  /*lint !e737 */
    if(resource_num != ARR_SIZE(image_array))
    {
        return;
    }
    for(i = 0; i < resource_num; i++)
    {
        GUIRES_GetImgWidthHeight(&image_width, &image_height, image_array[i],win_id);    
    
        //create layer    
        create_info.lcd_id          = GUI_MAIN_LCD_ID;
        create_info.owner_handle    = parent_id;
        create_info.offset_x        = 0; 
        create_info.offset_y        = 0;
        create_info.width           = image_width;
        create_info.height          = image_height;
        create_info.is_bg_layer     = FALSE;
        create_info.is_static_layer = FALSE;
        
        UILAYER_CreateLayer(&create_info, &layer_handle);
        p_layer_buffer = UILAYER_GetLayerBufferPtr(&layer_handle);
        if(0 == p_layer_buffer)
        {        
            UILAYER_RELEASELAYER(&layer_handle);
            return -2;
        }
        //draw image
        UILAYER_Clear(&layer_handle);
        rect = GUI_CreateRect(0,0, image_width - 1, image_height - 1);

        if(DisplayImgFilter(i,rect,&layer_handle))
        {
            GUIRES_DisplayImg(0, &rect,  0,  win_id,  image_array[i], &layer_handle);
        }
        
        //tranform to PMA888
        buffer_size = image_width*image_height*4 ;
        MMI_ARGB888TOPMARGB888( (uint32*)p_layer_buffer, buffer_size);
        
        //copy layer to new buffer
        p_buffer = SCI_ALLOCA(buffer_size);
        if(0 == p_buffer)
        {
            //release layer        
            UILAYER_RELEASELAYER(&layer_handle);
            return -3;
        }
        SCI_MEMCPY(p_buffer, p_layer_buffer, buffer_size);

        UILAYER_RELEASELAYER(&layer_handle);
        //SET BMP OBJ
        p_bmp_obj = (BMP_OBJ*)&s_weather_t.resource_array + i;
        p_bmp_obj->image_buffer = (uint32*)p_buffer;
        p_bmp_obj->image_size.w = image_width;
        p_bmp_obj->image_size.h = image_height;
        
    }
    return 0;
}
/*****************************************************************************/
//  Discription: BarPhone_DestroyImageBuffer
//  Global resource dependence: None
//  Author: ying.xu
//  Note : 
/*****************************************************************************/
LOCAL int32 BarPhone_DestroyImageBuffer(MMI_WIN_ID_T    win_id, MMIWIDGET_SPECIAL_EFFECT_T *p_widget_se)
{    
    int32 i = 0;
    int32 resource_num = sizeof(s_weather_t.resource_array) / sizeof(s_weather_t.resource_array.sun);
    BMP_OBJ *p_buffer = 0;
    GUI_LCD_DEV_INFO lcd_dev_info = {0};
    
    for(i = 0; i < resource_num; i++)
    {
        p_buffer = (BMP_OBJ*)&s_weather_t.resource_array + i;
        SCI_FREE(p_buffer->image_buffer);
    }
    
    MMK_GetWinLcdDevInfo(win_id, &lcd_dev_info);
    UILAYER_SetLayerMemFormat(&lcd_dev_info, UILAYER_MEM_IMMUTABLE);
    return 0;
}
#ifdef MMI_WEATHER_TEST
LOCAL WEATHER_STATUS_E BarPhone_GetTestWeatherState(void)
{
    WEATHER_STATUS_E status = WEATHER_STATE_NONE;
    if(s_weather_t.s_msg_id >= WEATHER_STATE_MAX)
    {
        s_weather_t.s_msg_id = 1;
    }
    status = s_weather_t.s_msg_id;
    return status;
}
#endif
/*****************************************************************************/
//  Discription: BarPhone_Display
//  Global resource dependence: None
//  Author: ying.xu
//  Note : 
/*****************************************************************************/
LOCAL int32 BarPhone_Display(MMI_WIN_ID_T  win_id, MMIWIDGET_SPECIAL_EFFECT_T *p_widget_se)
{
    GUI_LCD_DEV_INFO     lcd_dev_info    = {0};
    GUI_RECT_T      win_rect = {0};
    WEATHER_F_PARAM weather_fuction_param={0}; 
    MMIWEATHER_SETTING_T * setting_info_ptr = MMIWEATHER_GetSettingInfo();
  
    MMK_GetWinRect(win_id, &win_rect);
    
    MMK_GetWinLcdDevInfo(win_id,&lcd_dev_info);   

    UILAYER_SetPreMultiplyAlpha(&lcd_dev_info,  TRUE,  FALSE);
    weather_fuction_param.out_size.w  = win_rect.right - win_rect.left + 1;                    
    weather_fuction_param.out_size.h  = win_rect.bottom - win_rect.top + 1;    
    
    weather_fuction_param.Weather_R_Instance = &s_weather_t.resource_array;       
    weather_fuction_param.Point_Add_ptr = SCI_ALLOC_APP(2048*2);      

#ifdef MMI_WEATHER_TEST
    weather_fuction_param.cur_weather = BarPhone_GetTestWeatherState();
#else
    weather_fuction_param.cur_weather = BarPhone_GetWeatherState();
#endif

#ifdef MMI_WEATHER_TEST
    if(s_weather_t.s_pre_msg_id != s_weather_t.s_msg_id)
#else
    if(s_weather_t.is_need_disappear_anim)//(s_weather_t.cur_city_idx != setting_info_ptr->cur_city)
#endif
    {
        weather_fuction_param.total_frame = 20;
#ifdef MMI_WEATHER_TEST
        weather_fuction_param.cur_weather = s_weather_t.s_pre_msg_id;
#endif
    }
    else
    {
        weather_fuction_param.total_frame = MMIWIDGET_WEATHER_GetSEFrame(weather_fuction_param.cur_weather);
    }
    
    UILAYER_SetLayerMemFormat(&lcd_dev_info, UILAYER_MEM_DOUBLE_COPY);
    UILAYER_SwitchDoubleMem( &lcd_dev_info, TRUE );
    UILAYER_SetDirectDraw(TRUE);   
    
    if(-1 == p_widget_se->current_frame)
    {  
        weather_fuction_param.cur_frame = 0;//weather_fuction_param.total_frame;      
        p_widget_se->current_frame = 0;
    }
    else if(p_widget_se->current_frame < weather_fuction_param.total_frame)
    {
        weather_fuction_param.cur_frame = p_widget_se->current_frame; 
        p_widget_se->current_frame ++;
    }
    else
    {
        weather_fuction_param.cur_frame = p_widget_se->current_frame; 
    }
    UILAYER_Clear(&lcd_dev_info);
    weather_fuction_param.out_buffer = UILAYER_GetLayerBufferPtr(&lcd_dev_info);             
    if(s_weather_t.is_need_disappear_anim)
    {
        WeatherForcast_Disappear(&weather_fuction_param);
        if(weather_fuction_param.total_frame == p_widget_se->current_frame)
        {
#ifdef MMI_WEATHER_TEST
            if(s_weather_t.s_pre_msg_id != s_weather_t.s_msg_id)
#else
            if(s_weather_t.cur_city_idx != setting_info_ptr->cur_city)
#endif
            {
#ifdef MMI_WEATHER_TEST
                s_weather_t.s_pre_msg_id = s_weather_t.s_msg_id;
#endif
                if(setting_info_ptr->city_num != 0)
                {
    				if(s_weather_t.cur_city_idx < 0)
    				{
                        s_weather_t.cur_city_idx = setting_info_ptr->city_num - 1;
    				}
                    else if(s_weather_t.cur_city_idx >= setting_info_ptr->city_num)
                    {
                        s_weather_t.cur_city_idx = 0;
                    }
                }
                else
                {
                    s_weather_t.cur_city_idx = 0;
                }
                setting_info_ptr->cur_city = s_weather_t.cur_city_idx;
                MMIWEATHER_SetCurCity(s_weather_t.cur_city_idx);
    			p_widget_se->current_frame = 0;
                BarPhone_DestroyImageBuffer(win_id,&s_weather_se);
                if(0 != BarPhone_CreateImageBuffer(win_id,&s_weather_se))
                {
                    BarPhone_DestroyImageBuffer(win_id,&s_weather_se);
                }
            }
            s_weather_t.is_need_disappear_anim = FALSE; 
            WeatherForcast_Schedule(&weather_fuction_param);
        }
    }
    else
    {
        WeatherForcast_Schedule(&weather_fuction_param);
    }
    SCI_FREE(weather_fuction_param.Point_Add_ptr);    
    return 0;

}   
/*****************************************************************************/
//  Discription: MMIWIDGET_SetCityIndexFlag
//  Global resource dependence: None
//  Author: ying.xu
//  Note : set s_weather_t.cur_city_idx  
/*****************************************************************************/
PUBLIC void MMIWIDGET_SetCityIndexFlag(uint8 cur_city)
{
    s_weather_t.cur_city_idx = cur_city;
}
#endif
#endif
/*****************************************************************************/
//    Description : Get12HoursTypeTime
//    Global resource dependence : 
//    Author:  juan.wu
//    Note:
/*****************************************************************************/
LOCAL BOOLEAN Get12HoursTypeTime(SCI_TIME_T * s_time_ptr)
{
    BOOLEAN                     result = TRUE;

    if (12 > s_time_ptr->hour)//AM
    {
        if (1 > s_time_ptr->hour)//00:00-1:00
        {
            s_time_ptr->hour = s_time_ptr->hour + 12;
        }
    }
    else//PM
    {
        if ((12 <= s_time_ptr->hour) && (13 > s_time_ptr->hour))//12:00-13:00
        {
        }
        else
        {
            s_time_ptr->hour = s_time_ptr->hour-12;
        }
        result= FALSE;
    }   
    return result;
}
/*****************************************************************************/
//    Description : DrawAmPmString
//    Global resource dependence : 
//    Author:  juan.wu
//    Note:
/*****************************************************************************/
LOCAL void DrawAmPmString(
                        BOOLEAN is_am,
                        GUI_POINT_T start_point,
                        GUI_LCD_DEV_INFO  *lcd_info_ptr
                        )
{
    MMI_STRING_T        time_pa_string  = {0};
    GUISTR_STYLE_T      str_style       = {0};
    GUISTR_INFO_T       str_info        = {0};
    GUISTR_STATE_T      pa_text_state   = GUISTR_STATE_ALIGN|GUISTR_STATE_EFFECT|GUISTR_STATE_ELLIPSIS|GUISTR_STATE_SINGLE_LINE;
    GUI_RECT_T          win_rect        = {0};
    GUI_RECT_T          pa_rect         = {0};

    
    MMK_GetWinRect(g_weather_widget.win_id, &win_rect);
    if(is_am)
    {
        time_pa_string.wstr_ptr = L"AM";
    }
    else
    {
        time_pa_string.wstr_ptr = L"PM";
    }
    time_pa_string.wstr_len = MMIAPICOM_Wstrlen(time_pa_string.wstr_ptr);
    str_style.font = MMI_DEFAULT_SMALL_FONT;
    str_style.font_color = ParserNvValue(MMIWIDGET_WEATHER_BG_TEXTCOLOUR_SET_NV_ITEM_TEXTCOLOUR, &s_cur_bg_textcolour_set_info);
    GUISTR_GetStringInfo(&str_style, &time_pa_string, pa_text_state, &str_info);

    pa_rect = win_rect;

    pa_rect.top  = start_point.y + MMIWIDGET_BARWEATHER_TIME_HEIGHT + 2;
    pa_rect.bottom = pa_rect.top + str_info.height;
    pa_rect.right = start_point.x + MMIWIDGET_BARWEATHER_TIME_WIDTH;
    pa_rect.left = pa_rect.right - str_info.width;
    
    GUISTR_DrawTextToLCDInRect(lcd_info_ptr, 
                                &pa_rect, 
                                &pa_rect, 
                                &time_pa_string, 
                                &str_style, 
                                pa_text_state, 
                                GUISTR_TEXT_DIR_AUTO
                                );

}
#endif
