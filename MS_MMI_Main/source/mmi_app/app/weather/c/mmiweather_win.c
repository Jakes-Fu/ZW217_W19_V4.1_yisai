/*************************************************************************
** File Name:      mmiweather_win.c                                      *
** Author:         Gaily.Wang                                            *
** Date:           2011/04/18                                            *
** Copyright:      2011 Spreadtrum, Incorporated. All Rights Reserved.   *
** Description:    This file is used to describe weather app             *
**************************************************************************/

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "mmi_app_weather_trc.h"
#include "mn_type.h"
#include "sci_types.h"
#include "mmk_type.h"
#include "mmipub.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "window_parse.h"
#include "mmiweather.h"
#include "mmiweather_funcs.h"
#include "mmiweather_text.h"
#include "mmiweather_image.h"
#include "mmiweather_menutable.h"
#include "mmiweather_win.h"
#include "guiform.h"
#include "guilabel.h"
#include "guisetlist.h"
#include "guidropdownlist.h"
#include "guianim.h"
#include "guires.h"
#include "mmidisplay_data.h"
#include "mmi_appmsg.h"
#include "mmisms_text.h"
#include "mmisms_export.h"
#include "guictrl_api.h"
#include "mmiconnection_export.h"
#include "mmiudisk_export.h"
#include "mmitheme_pos.h"
/**--------------------------------------------------------------------------*
**                         Compiler Flag                                    *
**--------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C"
{
#endif

/*--------------------------------------------------------------------------*/
/*                         MACRO DEFINITION                                 */
/*--------------------------------------------------------------------------*/
  
#if defined MMI_PDA_SUPPORT && defined TOUCH_PANEL_SUPPORT
#define WEATHER_NO_SOFTKEYBAR_STYLE
#endif

// 字符串连接时，buffer的最大长度
#define WEATHER_MAX_STRING_LEN                      64
#define MAX_WEATHER_MSG_LEN	                        200
#define UPDATE_ALL_PAGE_DELAY_TIME                  200
#define TP_MOVE_VALID_VALUE                         4

#define WEATHER_DETAIL_CUR_WIND_LEFT                200
#define WEATHER_DETAIL_CUR_WIND_RIGHT               300

#if defined MAINLCD_SIZE_240X320
#define WEATHER_SLIDE_STEP_LEN                      30
#define WEATHER_SLIDE_STEP_TIME                     10
// 城市列表中，每个城市条目的高度，间距，城市名宽度，天气图标宽度
#define WEATHER_CITY_LIST_TOP_MERGIN                0
#if defined MMI_RES_ORIENT_BOTH || defined MMI_RES_ORIENT_PORTRAIT
#define WEATHER_CITY_LIST_ITEM_HEIGHT               74
#define WEATHER_CITY_LIST_ITEM_SPACE_HEIGHT     	6
#define WEATHER_CITY_LIST_ITEM_CITYNAME_WIDTH       80
#define WEATHER_CITY_LIST_ITEM_WEAICON_WIDTH        52
#else
#define WEATHER_CITY_LIST_ITEM_HEIGHT               60
#define WEATHER_CITY_LIST_ITEM_SPACE_HEIGHT     	2
#define WEATHER_CITY_LIST_ITEM_CITYNAME_WIDTH       100//66
#define WEATHER_CITY_LIST_ITEM_WEAICON_WIDTH        52
#endif

// 添加城市时，城市名editbox的巨型范围
#define WEAHTER_CITYNAME_EDITBOX_LEFT               10
#define WEAHTER_CITYNAME_EDITBOX_TOP                54 
#define WEAHTER_CITYNAME_EDITBOX_RIGHT              230  
#define WEAHTER_CITYNAME_EDITBOX_RIGHT_H            310 
#define WEAHTER_CITYNAME_EDITBOX_BOTTOM             85 

// 天气详情界面
// 当前天气信息的左右坐标
#define WEATHER_DETAIL_CUR_DETAIL_LEFT              10
#define WEATHER_DETAIL_CUR_DETAIL_RIGHT             105
// 温度，天气，温度范围，风力，湿度的上下坐标
#if defined MMI_RES_ORIENT_BOTH || defined MMI_RES_ORIENT_PORTRAIT
#define WEATHER_DETAIL_CUR_TEMP_TOP                 50
#define WEATHER_DETAIL_CUR_WEA_TOP                  78
#define WEATHER_DETAIL_CUR_TEMP_RANGE_TOP           99
#define WEATHER_DETAIL_CUR_WIND_TOP                 120
#define WEATHER_DETAIL_CUR_HUMI_TOP                 141
#define WEATHER_DETAIL_CUR_HUMI_BOTTOM              162
// 未来天气第一条的上下坐标，第二条以后坐标由条目高度计算(同小图标大小)
#define WEATHER_DETAIL_FORE_TOP                     170
#define WEATHER_DETAIL_FORE_BOTTOM                  194
// 未来天气条目高度
#define WEATHER_DETAIL_FORE_ITEM_HEIGHT             24
#else
#define WEATHER_DETAIL_CUR_TEMP_TOP                 30
#define WEATHER_DETAIL_CUR_WEA_TOP                  58
#define WEATHER_DETAIL_CUR_TEMP_RANGE_TOP           79
#define WEATHER_DETAIL_CUR_WIND_TOP                 30
#define WEATHER_DETAIL_CUR_HUMI_TOP                 60
#define WEATHER_DETAIL_CUR_HUMI_BOTTOM              81
// 未来天气第一条的上下坐标，第二条以后坐标由条目高度计算(同小图标大小)
#define WEATHER_DETAIL_FORE_TOP                     116
#define WEATHER_DETAIL_FORE_BOTTOM                  136
// 未来天气条目高度
#define WEATHER_DETAIL_FORE_ITEM_HEIGHT             22
#endif
// 当前天气大图标的左右坐标
#define WEATHER_DETAIL_CUR_WEA_ICON_LEFT            110
#define WEATHER_DETAIL_CUR_WEA_ICON_RIGHT           230
#define WEATHER_DETAIL_CUR_WEA_ICON_TOP             45

// 未来天气日期左右坐标
#define WEATHER_DETAIL_FORE_DATE_LEFT               11
#define WEATHER_DETAIL_FORE_DATE_RIGHT              64
// 未来天气星期左右坐标
#define WEATHER_DETAIL_FORE_WEEK_LEFT               66
#define WEATHER_DETAIL_FORE_WEEK_RIGHT              119
// 未来天气天气图标左右坐标
#define WEATHER_DETAIL_FORE_ICON_LEFT               121
#define WEATHER_DETAIL_FORE_ICON_RIGHT              174
// 未来天气温度范围左右坐标
#define WEATHER_DETAIL_FORE_TEMP_RANGE_LEFT         176
#define WEATHER_DETAIL_FORE_TEMP_RANGE_RIGHT        229
// 天气更新时间显示区域
#define WEATHER_DETAIL_UPDATE_TIME_LEFT             5
#define WEATHER_DETAIL_UPDATE_TIME_TOP              265
#define WEATHER_DETAIL_UPDATE_TIME_RIGHT            235
#define WEATHER_DETAIL_UPDATE_TIME_BOTTOM           289

// 温度趋势界面
// 第一条左右坐标，第二条以后左右坐标由条目宽度计算(同中图标大小)
#define WEATHER_TRENDS_ITEM_LEFT                    10
#define WEATHER_TRENDS_ITEM_RIGHT                   61

#if defined MMI_RES_ORIENT_BOTH || defined MMI_RES_ORIENT_PORTRAIT
// 条目宽度
#define WEATHER_TRENDS_ITEM_WIDTH                   55
// 星期上下坐标
#define WEATHER_TRENDS_WEEK_TOP                     52
#define WEATHER_TRENDS_WEEK_BOTTOM                  68
// 图标天气上下坐标
#define WEATHER_TRENDS_WEA_ICON_TOP                 73
#define WEATHER_TRENDS_WEEK_ICON_BOTTOM             120
// 日期上下坐标
#define WEATHER_TRENDS_DATE_TOP                     253
#define WEATHER_TRENDS_DATE_BOTTOM                  272

// 温度曲线上下边界
#define WEATHER_TEMP_LINE_SPAN                      73
#define WEATHER_TEMP_LINE_BOTTOM                    227
#define WEATHER_TEMP_STRING_HEIGHT                  20
#else
// 条目宽度
#define WEATHER_TRENDS_ITEM_WIDTH                   78
// 星期上下坐标
#define WEATHER_TRENDS_WEEK_TOP                     30
#define WEATHER_TRENDS_WEEK_BOTTOM                  46
// 图标天气上下坐标
//#define WEATHER_TRENDS_WEA_ICON_TOP                 50
//#define WEATHER_TRENDS_WEEK_ICON_BOTTOM             100
// 日期上下坐标
#define WEATHER_TRENDS_DATE_TOP                     40
#define WEATHER_TRENDS_DATE_BOTTOM                  90

// 温度曲线上下边界
#define WEATHER_TEMP_LINE_SPAN                      90
#define WEATHER_TEMP_LINE_BOTTOM                    190

#define WEATHER_TEMP_STRING_HEIGHT                  16
#endif
#define WEATHER_TEMP_CIRCLE_SIZE                    3

#ifdef WEATHER_NO_SOFTKEYBAR_STYLE
#define PAGE_DOTS_BOX_WIDTH                         60
#define PAGE_DOTS_BOX_HEIGHT                        20
#endif

#elif defined MAINLCD_SIZE_240X400
#define WEATHER_SLIDE_STEP_LEN                      30
#define WEATHER_SLIDE_STEP_TIME                     8
// 城市列表中，每个城市条目的高度，间距，城市名宽度，天气图标宽度
#define WEATHER_CITY_LIST_TOP_MERGIN                10
#define WEATHER_CITY_LIST_ITEM_HEIGHT               74
#define WEATHER_CITY_LIST_ITEM_SPACE_HEIGHT     	6
#define WEATHER_CITY_LIST_ITEM_CITYNAME_WIDTH       80
#define WEATHER_CITY_LIST_ITEM_WEAICON_WIDTH        52

// 添加城市时，城市名editbox的巨型范围
#define WEAHTER_CITYNAME_EDITBOX_LEFT               10
#define WEAHTER_CITYNAME_EDITBOX_TOP                54 
#define WEAHTER_CITYNAME_EDITBOX_RIGHT              230  
#define WEAHTER_CITYNAME_EDITBOX_RIGHT_H            390 
#define WEAHTER_CITYNAME_EDITBOX_BOTTOM             85 

// 天气详情界面
// 当前天气信息的左右坐标
#define WEATHER_DETAIL_CUR_DETAIL_LEFT              10
#define WEATHER_DETAIL_CUR_DETAIL_RIGHT             105
// 温度，天气，温度范围，风力，湿度的上下坐标
#define WEATHER_DETAIL_CUR_TEMP_TOP                 65
#define WEATHER_DETAIL_CUR_WEA_TOP                  98
#define WEATHER_DETAIL_CUR_TEMP_RANGE_TOP           120
#define WEATHER_DETAIL_CUR_WIND_TOP                 142
#define WEATHER_DETAIL_CUR_HUMI_TOP                 174
#define WEATHER_DETAIL_CUR_HUMI_BOTTOM              186
// 当前天气大图标的左右坐标
#define WEATHER_DETAIL_CUR_WEA_ICON_LEFT            110
#define WEATHER_DETAIL_CUR_WEA_ICON_RIGHT           230
#define WEATHER_DETAIL_CUR_WEA_ICON_TOP             58
// 未来天气第一条的上下坐标，第二条以后坐标由条目高度计算(同小图标大小)
#define WEATHER_DETAIL_FORE_TOP                     205
#define WEATHER_DETAIL_FORE_BOTTOM                  237
// 未来天气条目高度
#define WEATHER_DETAIL_FORE_ITEM_HEIGHT             32
// 未来天气日期左右坐标
#define WEATHER_DETAIL_FORE_DATE_LEFT               11
#define WEATHER_DETAIL_FORE_DATE_RIGHT              64
// 未来天气星期左右坐标
#define WEATHER_DETAIL_FORE_WEEK_LEFT               66
#define WEATHER_DETAIL_FORE_WEEK_RIGHT              119
// 未来天气天气图标左右坐标
#define WEATHER_DETAIL_FORE_ICON_LEFT               121
#define WEATHER_DETAIL_FORE_ICON_RIGHT              174
// 未来天气温度范围左右坐标
#define WEATHER_DETAIL_FORE_TEMP_RANGE_LEFT         176
#define WEATHER_DETAIL_FORE_TEMP_RANGE_RIGHT        229
// 天气更新时间显示区域
#define WEATHER_DETAIL_UPDATE_TIME_LEFT             5
#define WEATHER_DETAIL_UPDATE_TIME_TOP              340
#define WEATHER_DETAIL_UPDATE_TIME_RIGHT            235
#define WEATHER_DETAIL_UPDATE_TIME_BOTTOM           364

// 温度趋势界面
// 第一条左右坐标，第二条以后左右坐标由条目宽度计算(同中图标大小)
#define WEATHER_TRENDS_ITEM_LEFT                    10
#define WEATHER_TRENDS_ITEM_RIGHT                   61
// 条目宽度
#define WEATHER_TRENDS_ITEM_WIDTH                   55
// 星期上下坐标
#define WEATHER_TRENDS_WEEK_TOP                     63//43
#define WEATHER_TRENDS_WEEK_BOTTOM                  83//68
// 图标天气上下坐标
#define WEATHER_TRENDS_WEA_ICON_TOP                 90
#define WEATHER_TRENDS_WEEK_ICON_BOTTOM             137
// 日期上下坐标
#define WEATHER_TRENDS_DATE_TOP                     323
#define WEATHER_TRENDS_DATE_BOTTOM                  343

// 温度曲线上下边界
#define WEATHER_TEMP_LINE_SPAN                      102
#define WEATHER_TEMP_LINE_BOTTOM                    300

#define WEATHER_TEMP_STRING_HEIGHT                  20
#define WEATHER_TEMP_CIRCLE_SIZE                    3

#ifdef WEATHER_NO_SOFTKEYBAR_STYLE
#define PAGE_DOTS_BOX_WIDTH                         60
#define PAGE_DOTS_BOX_HEIGHT                        20
#endif

#elif defined MAINLCD_SIZE_320X480
#define WEATHER_SLIDE_STEP_LEN                      80
#define WEATHER_SLIDE_STEP_TIME                     0
// 城市列表中，每个城市条目的高度，间距，城市名宽度，天气图标宽度
#define WEATHER_CITY_LIST_TOP_MERGIN                0
#define WEATHER_CITY_LIST_ITEM_HEIGHT               74
#define WEATHER_CITY_LIST_ITEM_SPACE_HEIGHT     	4
#define WEATHER_CITY_LIST_ITEM_CITYNAME_WIDTH       115
#define WEATHER_CITY_LIST_ITEM_WEAICON_WIDTH        70

// 添加城市时，城市名editbox的巨型范围
#define WEAHTER_CITYNAME_EDITBOX_LEFT               10
#define WEAHTER_CITYNAME_EDITBOX_TOP                54 
#define WEAHTER_CITYNAME_EDITBOX_RIGHT              310  
#define WEAHTER_CITYNAME_EDITBOX_RIGHT_H            470 
#define WEAHTER_CITYNAME_EDITBOX_BOTTOM             85 

// 天气详情界面
// 当前天气信息的左右坐标
#define WEATHER_DETAIL_CUR_DETAIL_LEFT              13
#define WEATHER_DETAIL_CUR_DETAIL_RIGHT             136
// 温度，天气，温度范围，风力，湿度的上下坐标
#define WEATHER_DETAIL_CUR_TEMP_TOP                 75
#define WEATHER_DETAIL_CUR_WEA_TOP                  117
#define WEATHER_DETAIL_CUR_TEMP_RANGE_TOP           148
#define WEATHER_DETAIL_CUR_WIND_TOP                 180
#define WEATHER_DETAIL_CUR_HUMI_TOP                 211
#define WEATHER_DETAIL_CUR_HUMI_BOTTOM              243
// 当前天气大图标的左右坐标
#define WEATHER_DETAIL_CUR_WEA_ICON_LEFT            173
#define WEATHER_DETAIL_CUR_WEA_ICON_RIGHT           310
#define WEATHER_DETAIL_CUR_WEA_ICON_TOP             87
// 未来天气第一条的上下坐标，第二条以后坐标由条目高度计算(同小图标大小)
#define WEATHER_DETAIL_FORE_TOP                     255
#define WEATHER_DETAIL_FORE_BOTTOM                  287
// 未来天气条目高度
#define WEATHER_DETAIL_FORE_ITEM_HEIGHT             36
// 未来天气日期左右坐标
#define WEATHER_DETAIL_FORE_DATE_LEFT               13
#define WEATHER_DETAIL_FORE_DATE_RIGHT              85
// 未来天气星期左右坐标
#define WEATHER_DETAIL_FORE_WEEK_LEFT               87
#define WEATHER_DETAIL_FORE_WEEK_RIGHT              159
// 未来天气天气图标左右坐标
#define WEATHER_DETAIL_FORE_ICON_LEFT               161
#define WEATHER_DETAIL_FORE_ICON_RIGHT              233
// 未来天气温度范围左右坐标
#define WEATHER_DETAIL_FORE_TEMP_RANGE_LEFT         235
#define WEATHER_DETAIL_FORE_TEMP_RANGE_RIGHT        307
// 天气更新时间显示区域
#define WEATHER_DETAIL_UPDATE_TIME_LEFT             5
#define WEATHER_DETAIL_UPDATE_TIME_TOP              402
#define WEATHER_DETAIL_UPDATE_TIME_RIGHT            315
#define WEATHER_DETAIL_UPDATE_TIME_BOTTOM           438

// 温度趋势界面
// 第一条左右坐标，第二条以后左右坐标由条目宽度计算(同中图标大小)
#define WEATHER_TRENDS_ITEM_LEFT                    16
#define WEATHER_TRENDS_ITEM_RIGHT                   67
// 条目宽度
#define WEATHER_TRENDS_ITEM_WIDTH                   75
// 星期上下坐标
#define WEATHER_TRENDS_WEEK_TOP                     64
#define WEATHER_TRENDS_WEEK_BOTTOM                  102
// 图标天气上下坐标
#define WEATHER_TRENDS_WEA_ICON_TOP                 119
#define WEATHER_TRENDS_WEEK_ICON_BOTTOM             180
// 日期上下坐标
#define WEATHER_TRENDS_DATE_TOP                     379
#define WEATHER_TRENDS_DATE_BOTTOM                  408

// 温度曲线上下边界
#define WEATHER_TEMP_LINE_SPAN                      123
#define WEATHER_TEMP_LINE_BOTTOM                    354

#define WEATHER_TEMP_STRING_HEIGHT                  20
#define WEATHER_TEMP_CIRCLE_SIZE                    3

#ifdef WEATHER_NO_SOFTKEYBAR_STYLE
#define PAGE_DOTS_BOX_WIDTH                         90
#define PAGE_DOTS_BOX_HEIGHT                        30
#endif

#elif defined MAINLCD_SIZE_176X220
#define WEATHER_SLIDE_STEP_LEN                      40
#define WEATHER_SLIDE_STEP_TIME                     0
// 城市列表中，每个城市条目的高度，间距，城市名宽度，天气图标宽度
#define WEATHER_CITY_LIST_TOP_MERGIN                0
#define WEATHER_CITY_LIST_ITEM_HEIGHT               37
#define WEATHER_CITY_LIST_ITEM_SPACE_HEIGHT     	2
#define WEATHER_CITY_LIST_ITEM_CITYNAME_WIDTH       58
#define WEATHER_CITY_LIST_ITEM_WEAICON_WIDTH        35

// 添加城市时，城市名editbox的巨型范围
#define WEAHTER_CITYNAME_EDITBOX_LEFT               5
#define WEAHTER_CITYNAME_EDITBOX_TOP                27 
#define WEAHTER_CITYNAME_EDITBOX_RIGHT              155  
#define WEAHTER_CITYNAME_EDITBOX_RIGHT_H            235 
#define WEAHTER_CITYNAME_EDITBOX_BOTTOM             43 

// 天气详情界面
// 当前天气信息的左右坐标
#define WEATHER_DETAIL_CUR_DETAIL_LEFT              7
#define WEATHER_DETAIL_CUR_DETAIL_RIGHT             68
// 温度，天气，温度范围，风力，湿度的上下坐标
#define WEATHER_DETAIL_CUR_TEMP_TOP                 38
#define WEATHER_DETAIL_CUR_WEA_TOP                  58
#define WEATHER_DETAIL_CUR_TEMP_RANGE_TOP           74
#define WEATHER_DETAIL_CUR_WIND_TOP                 90
#define WEATHER_DETAIL_CUR_HUMI_TOP                 105
#define WEATHER_DETAIL_CUR_HUMI_BOTTOM              122
// 当前天气大图标的左右坐标
#define WEATHER_DETAIL_CUR_WEA_ICON_LEFT            86
#define WEATHER_DETAIL_CUR_WEA_ICON_RIGHT           155
#define WEATHER_DETAIL_CUR_WEA_ICON_TOP             44
// 未来天气第一条的上下坐标，第二条以后坐标由条目高度计算(同小图标大小)
#define WEATHER_DETAIL_FORE_TOP                     128
#define WEATHER_DETAIL_FORE_BOTTOM                  144
// 未来天气条目高度
#define WEATHER_DETAIL_FORE_ITEM_HEIGHT             18
// 未来天气日期左右坐标
#define WEATHER_DETAIL_FORE_DATE_LEFT               7
#define WEATHER_DETAIL_FORE_DATE_RIGHT              43
// 未来天气星期左右坐标
#define WEATHER_DETAIL_FORE_WEEK_LEFT               44
#define WEATHER_DETAIL_FORE_WEEK_RIGHT              80
// 未来天气天气图标左右坐标
#define WEATHER_DETAIL_FORE_ICON_LEFT               81
#define WEATHER_DETAIL_FORE_ICON_RIGHT              117
// 未来天气温度范围左右坐标
#define WEATHER_DETAIL_FORE_TEMP_RANGE_LEFT         118
#define WEATHER_DETAIL_FORE_TEMP_RANGE_RIGHT        154
// 天气更新时间显示区域
#define WEATHER_DETAIL_UPDATE_TIME_LEFT             2
#define WEATHER_DETAIL_UPDATE_TIME_TOP              201
#define WEATHER_DETAIL_UPDATE_TIME_RIGHT            158
#define WEATHER_DETAIL_UPDATE_TIME_BOTTOM           219

// 温度趋势界面
// 第一条左右坐标，第二条以后左右坐标由条目宽度计算(同中图标大小)
#define WEATHER_TRENDS_ITEM_LEFT                    8
#define WEATHER_TRENDS_ITEM_RIGHT                   34
// 条目宽度
#define WEATHER_TRENDS_ITEM_WIDTH                   38
// 星期上下坐标
#define WEATHER_TRENDS_WEEK_TOP                     32
#define WEATHER_TRENDS_WEEK_BOTTOM                  51
// 图标天气上下坐标
#define WEATHER_TRENDS_WEA_ICON_TOP                 60
#define WEATHER_TRENDS_WEEK_ICON_BOTTOM             90
// 日期上下坐标
#define WEATHER_TRENDS_DATE_TOP                     190
#define WEATHER_TRENDS_DATE_BOTTOM                  255

// 温度曲线上下边界
#define WEATHER_TEMP_LINE_SPAN                      62
#define WEATHER_TEMP_LINE_BOTTOM                    177

#define WEATHER_TEMP_STRING_HEIGHT                  10
#define WEATHER_TEMP_CIRCLE_SIZE                    2

#ifdef WEATHER_NO_SOFTKEYBAR_STYLE
#define PAGE_DOTS_BOX_WIDTH                         45
#define PAGE_DOTS_BOX_HEIGHT                        15
#endif

#elif defined MAINLCD_SIZE_128X160
#define WEATHER_SLIDE_STEP_LEN                      40
#define WEATHER_SLIDE_STEP_TIME                     0
// 城市列表中，每个城市条目的高度，间距，城市名宽度，天气图标宽度
#define WEATHER_CITY_LIST_TOP_MERGIN                0
#define WEATHER_CITY_LIST_ITEM_HEIGHT               37
#define WEATHER_CITY_LIST_ITEM_SPACE_HEIGHT     	2
#define WEATHER_CITY_LIST_ITEM_CITYNAME_WIDTH       58
#define WEATHER_CITY_LIST_ITEM_WEAICON_WIDTH        35

// 添加城市时，城市名editbox的巨型范围
#define WEAHTER_CITYNAME_EDITBOX_LEFT               5
#define WEAHTER_CITYNAME_EDITBOX_TOP                27 
#define WEAHTER_CITYNAME_EDITBOX_RIGHT              155  
#define WEAHTER_CITYNAME_EDITBOX_RIGHT_H            235 
#define WEAHTER_CITYNAME_EDITBOX_BOTTOM             43 

// 天气详情界面
// 当前天气信息的左右坐标
#define WEATHER_DETAIL_CUR_DETAIL_LEFT              7
#define WEATHER_DETAIL_CUR_DETAIL_RIGHT             68
// 温度，天气，温度范围，风力，湿度的上下坐标
#define WEATHER_DETAIL_CUR_TEMP_TOP                 38
#define WEATHER_DETAIL_CUR_WEA_TOP                  58
#define WEATHER_DETAIL_CUR_TEMP_RANGE_TOP           74
#define WEATHER_DETAIL_CUR_WIND_TOP                 90
#define WEATHER_DETAIL_CUR_HUMI_TOP                 105
#define WEATHER_DETAIL_CUR_HUMI_BOTTOM              122
// 当前天气大图标的左右坐标
#define WEATHER_DETAIL_CUR_WEA_ICON_LEFT            86
#define WEATHER_DETAIL_CUR_WEA_ICON_RIGHT           155
#define WEATHER_DETAIL_CUR_WEA_ICON_TOP             44
// 未来天气第一条的上下坐标，第二条以后坐标由条目高度计算(同小图标大小)
#define WEATHER_DETAIL_FORE_TOP                     128
#define WEATHER_DETAIL_FORE_BOTTOM                  144
// 未来天气条目高度
#define WEATHER_DETAIL_FORE_ITEM_HEIGHT             18
// 未来天气日期左右坐标
#define WEATHER_DETAIL_FORE_DATE_LEFT               7
#define WEATHER_DETAIL_FORE_DATE_RIGHT              43
// 未来天气星期左右坐标
#define WEATHER_DETAIL_FORE_WEEK_LEFT               44
#define WEATHER_DETAIL_FORE_WEEK_RIGHT              80
// 未来天气天气图标左右坐标
#define WEATHER_DETAIL_FORE_ICON_LEFT               81
#define WEATHER_DETAIL_FORE_ICON_RIGHT              117
// 未来天气温度范围左右坐标
#define WEATHER_DETAIL_FORE_TEMP_RANGE_LEFT         118
#define WEATHER_DETAIL_FORE_TEMP_RANGE_RIGHT        154
// 天气更新时间显示区域
#define WEATHER_DETAIL_UPDATE_TIME_LEFT             2
#define WEATHER_DETAIL_UPDATE_TIME_TOP              201
#define WEATHER_DETAIL_UPDATE_TIME_RIGHT            158
#define WEATHER_DETAIL_UPDATE_TIME_BOTTOM           219

// 温度趋势界面
// 第一条左右坐标，第二条以后左右坐标由条目宽度计算(同中图标大小)
#define WEATHER_TRENDS_ITEM_LEFT                    8
#define WEATHER_TRENDS_ITEM_RIGHT                   34
// 条目宽度
#define WEATHER_TRENDS_ITEM_WIDTH                   38
// 星期上下坐标
#define WEATHER_TRENDS_WEEK_TOP                     32
#define WEATHER_TRENDS_WEEK_BOTTOM                  51
// 图标天气上下坐标
#define WEATHER_TRENDS_WEA_ICON_TOP                 60
#define WEATHER_TRENDS_WEEK_ICON_BOTTOM             90
// 日期上下坐标
#define WEATHER_TRENDS_DATE_TOP                     190
#define WEATHER_TRENDS_DATE_BOTTOM                  255

// 温度曲线上下边界
#define WEATHER_TEMP_LINE_SPAN                      62
#define WEATHER_TEMP_LINE_BOTTOM                    177

#define WEATHER_TEMP_STRING_HEIGHT                  10
#define WEATHER_TEMP_CIRCLE_SIZE                    2

#ifdef WEATHER_NO_SOFTKEYBAR_STYLE
#define PAGE_DOTS_BOX_WIDTH                         45
#define PAGE_DOTS_BOX_HEIGHT                        15
#endif

#endif

#ifdef MMI_WEATHER_JSON_PARSE
LOCAL BOOLEAN s_has_sel = FALSE;
#endif

/**--------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/

/**-------------------------------------------------------------------------*
**                         LOCAL FUNCTION DECLARE                           *
**--------------------------------------------------------------------------*/

/****************************************************************************/
//  Description : 得到温度值+单位符号字符串
//  Global resource dependence : 
//  Author:violent.wei
//  Note: 
/****************************************************************************/
LOCAL void GetTemperatureString(
                          int32 temp,
                          MMI_STRING_T* string_ptr
                          );

/****************************************************************************/
//  Description : 得到X-Y温度字符串
//  Global resource dependence : 
//  Author:violent.wei
//  Note: 
/****************************************************************************/
LOCAL void GetTemperatureRangeString(int32 low,
                               int32 high,
                               MMI_STRING_T* string_ptr,
                               BOOLEAN is_need_uint_str
                               );

/****************************************************************************/
//  Description : 得到风向+风力字符串
//  Global resource dependence : 
//  Author:violent.wei
//  Note: 
/****************************************************************************/
LOCAL void GetWindInfoString(
                       MMIWEATHER_WIND_DIR_E wind_dir,
                       int32 wind_force,
                       MMI_STRING_T* string_ptr
                       );

/****************************************************************************/
//  Description : 得到湿度:x%字符串
//  Global resource dependence : 
//  Author:violent.wei
//  Note: 
/****************************************************************************/
LOCAL void GetHumidityString(
                       wchar* humidity_data_str,
                       MMI_STRING_T* string_ptr
                       );

/****************************************************************************/
//  Description : 得到更新于:xxx-xxx xx:xx字符串
//  Global resource dependence : 
//  Author:violent.wei
//  Note: 
/****************************************************************************/
LOCAL void GetUpdateTimeString(
                         wchar* date_str,
                         wchar* time_str,
                         MMI_STRING_T* string
                         );

/****************************************************************************/
//  Description : 计算温度对应的坐标点，进而连线
//  Global resource dependence : 
//  Author:violent.wei
//  Note: 
/****************************************************************************/
LOCAL void CalculateTempPoint(
                         MMIWEATHER_FC_CONDITIONS_T* fc_conditions_ptr,
                         GUI_POINT_T* temp_high_point_ptr,
                         GUI_POINT_T* temp_low_point_ptr,
                         uint32 num
                         );

/****************************************************************************/
//  Description : Load City list form, set its parameter for displaying
//  Global resource dependence : 
//  Author:violent.wei
//  Note: 
/****************************************************************************/
LOCAL void LoadCityListControl(
                          MMI_WIN_ID_T        win_id
                          );

/****************************************************************************/
//  Description : Load City list data, insert into form
//  Global resource dependence : 
//  Author:violent.wei
//  Note: 
/****************************************************************************/
LOCAL void LoadCityListData(
                            MMI_WIN_ID_T        win_id
                            );

/****************************************************************************/
//  Description : 通过触摸点，设置FORM的Active Item
//  Global resource dependence : 
//  Author:violent.wei
//  Note: 
/****************************************************************************/
LOCAL int32 SetFormActiveItem(
                                int16 point_y
                                );

/****************************************************************************/
//  Description : 主界面UI Layer创建
//  Global resource dependence : 
//  Author:violent.wei
//  Note: 
/****************************************************************************/
LOCAL void UILayerCreate();

/****************************************************************************/
//  Description : 主界面UI Layer销毁
//  Global resource dependence : 
//  Author:violent.wei
//  Note: 
/****************************************************************************/
LOCAL void UILayerDestroy(void);

/****************************************************************************/
//  Description : 主界面UI Layer附着
//  Global resource dependence : 
//  Author:violent.wei
//  Note: 
/****************************************************************************/
LOCAL void UILayerAppend(
                         WEATHER_WIN_PAGE win_page
                         );

/****************************************************************************/
//  Description : 主界面UI Layer分离
//  Global resource dependence : 
//  Author:violent.wei
//  Note: 
/****************************************************************************/
LOCAL void UILayerRemoveOthers(
                               WEATHER_WIN_PAGE win_page
                               );

/****************************************************************************/
//  Description : 切换当前page
//  Global resource dependence : 
//  Author:violent.wei
//  Note: 
/****************************************************************************/
LOCAL void SwitchPage(
                      WEATHER_WIN_PAGE weather_win_page, 
                      BOOLEAN is_slide_effect
                      );
#if defined TOUCH_PANEL_SUPPORT
/****************************************************************************/
//  Description : 触摸滑动过程中对图层的处理
//  Global resource dependence : 
//  Author:violent.wei
//  Note: 
/****************************************************************************/
LOCAL void MovePage(
                    int16 offset
                    );
#endif
/****************************************************************************/
//  Description : 设置标题栏文字
//  Global resource dependence : 
//  Author:violent.wei
//  Note: 
/****************************************************************************/
LOCAL void SetTitleBar(
                       WEATHER_WIN_PAGE weather_win_page
                       );
/****************************************************************************/
//  Description : 设置子控件是否为隐藏
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 
/****************************************************************************/
LOCAL void SetFormHideOrNormal(BOOLEAN is_hide);

#ifdef WEATHER_NO_SOFTKEYBAR_STYLE
/****************************************************************************/
//  Description : 设置当前页点
//  Global resource dependence : 
//  Author:violent.wei
//  Note: 
/****************************************************************************/
LOCAL void SetPageDots(
                       WEATHER_WIN_PAGE weather_win_page
                       );
#endif

#if defined TOUCH_PANEL_SUPPORT
/****************************************************************************/
//  Description : 考虑到FORM相应TP，对滑动有影响，在滑动之前先做处理
//  Global resource dependence : 
//  Author:violent.wei
//  Note: 
/****************************************************************************/
LOCAL void  PrepareTPCondition(
                               BOOLEAN is_form_disable_tp
                               );
#endif
/****************************************************************************/
//  Description : 城市列表消息处理函数
//  Global resource dependence : 
//  Author:violent.wei
//  Note: 
/****************************************************************************/
LOCAL MMI_RESULT_E  HandleWeatherMainWinMsg(
                                         MMI_WIN_ID_T        win_id, 
                                         MMI_MESSAGE_ID_E    msg_id, 
                                         DPARAM              param
                                         );

/****************************************************************************/
//  Description : 添加城市消息处理函数
//  Global resource dependence : 
//  Author:violent.wei
//  Note: 
/****************************************************************************/
LOCAL MMI_RESULT_E  HandleAddCityWinMsg(
                                        MMI_WIN_ID_T        win_id, 
                                        MMI_MESSAGE_ID_E    msg_id, 
                                        DPARAM              param
                                        );

/****************************************************************************/
//  Description : 因为层的原因，城市列表form控件需要同步刷新
//  Global resource dependence : 
//  Author:violent.wei
//  Note: 
/****************************************************************************/
LOCAL void DrawWeatherCityList(void);

/****************************************************************************/
//  Description : 画详情界面
//  Global resource dependence : 
//  Author:violent.wei
//  Note: 
/****************************************************************************/
LOCAL void DrawWeatherDetailsPage(MMI_WIN_ID_T win_id);

/****************************************************************************/
//  Description : 画温度趋势
//  Global resource dependence : 
//  Author:violent.wei
//  Note: 
/****************************************************************************/
LOCAL void DrawWeatherTrendsPage(MMI_WIN_ID_T win_id);

/****************************************************************************/
//  Description : 设置消息处理函数
//  Global resource dependence : 
//  Author:violent.wei
//  Note: 
/****************************************************************************/
LOCAL MMI_RESULT_E  HandleWeatherSettingWinMsg(
                                        MMI_WIN_ID_T        win_id, 
                                        MMI_MESSAGE_ID_E    msg_id, 
                                        DPARAM              param
                                        );

/****************************************************************************/
//  Description : 菜单消息处理函数
//  Global resource dependence : 
//  Author:violent.wei
//  Note: 
/****************************************************************************/
LOCAL MMI_RESULT_E  HandleMainMenuWinMsg(
                                         MMI_WIN_ID_T        win_id, 
                                         MMI_MESSAGE_ID_E    msg_id, 
                                         DPARAM              param
                                         );

/*****************************************************************************/
//  Description : handle open wait window message
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleUpdateWaitingWinMsg(
                                       MMI_WIN_ID_T win_id, 
                                       MMI_MESSAGE_ID_E msg_id, 
                                       DPARAM param
                                       );

/****************************************************************************/
//  Description : 发送短消息
//  Global resource dependence : 
//  Author:xiaoming.ren
//  Note: 
/****************************************************************************/
LOCAL void MMIWEATHER_SendMessage(void);


/****************************************************************************/
//  Description : 网络选择消息处理
//  Global resource dependence : 
//  Author:Gaily.Wang
//  Note: 
/****************************************************************************/
LOCAL MMI_RESULT_E  HandleNetSelectWinMsg(
                                        MMI_WIN_ID_T        win_id, 
                                        MMI_MESSAGE_ID_E    msg_id, 
                                        DPARAM              param
                                        );


/*****************************************************************************/
//  Description : handle open wait window message
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleCancelWaitingWinMsg(
                                       MMI_WIN_ID_T win_id, 
                                       MMI_MESSAGE_ID_E msg_id, 
                                       DPARAM param
                                       );

#ifndef WEATHER_SUPPORT_GOOGLE
/*****************************************************************************/
//  Description : HandleCountyListWinMsg
//  Global resource dependence : 
//  Author: juan.wu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleCountyListWinMsg(
                                       MMI_WIN_ID_T win_id, 
                                       MMI_MESSAGE_ID_E msg_id, 
                                       DPARAM param
                                       );
/*****************************************************************************/
//  Description : HandleCountyListWinMsg
//  Global resource dependence : 
//  Author: juan.wu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleWeatherAboutWinMsg(
                                           MMI_WIN_ID_T win_id, 
                                           MMI_MESSAGE_ID_E msg_id, 
                                           DPARAM param
                                           );
#endif
/**-------------------------------------------------------------------------*
**                         STATIC VARIABLE DEFINITION                       *
**--------------------------------------------------------------------------*/

// 记录当前在哪个PAGE
LOCAL WEATHER_WIN_PAGE  s_weather_win_page = WEATHER_WIN_CITYLIST;
// 三个PAGE的图层
LOCAL GUI_LCD_DEV_INFO  s_citylist_lcd_dev_info = {0};
LOCAL GUI_LCD_DEV_INFO  s_details_lcd_dev_info = {0};
LOCAL GUI_LCD_DEV_INFO  s_trends_lcd_dev_info = {0};
// 做滑动前的偏移
LOCAL int16             s_source_ui_layer_offset = 0;
// 做滑动后的偏移
LOCAL int16             s_target_ui_layer_offset = 0;
// 改变焦点后，是否直接进入下一页，TP会，KEY不会
LOCAL BOOLEAN           s_is_switch_after_cur_city_changed = FALSE;
// 触摸过程中是否有tp move，如果有就会滑动，没有就改变form的焦点item
LOCAL BOOLEAN           s_is_touch_move_happen = FALSE;
// 如果在更新图层数据时，正在滑动过程中，那么延时更新数据，否则会滑屏
LOCAL uint8             s_update_all_page_delay_timer_id = 0;

//城市列表
WINDOW_TABLE(MMIWEATHER_MAIN_WIN_TAB) = 
{
    WIN_FUNC( (uint32)HandleWeatherMainWinMsg ),    
    WIN_ID( MMIWEATHER_MAIN_WIN_ID ),
    WIN_SUPPORT_ANGLE(WIN_SUPPORT_ANGLE_INIT),
#if defined MMI_RES_ORIENT_BOTH || defined MMI_RES_ORIENT_PORTRAIT
    WIN_TITLE( TXT_WEATHER_CITY_LIST ),
#else
//    WIN_HIDE_STATUS,
#endif
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER, MMIWEATHER_CITY_LIST_FORM_CTRL_ID),
    
    // city0
    CHILD_FORM_CTRL(TRUE, GUIFORM_LAYOUT_SBS, MMIWEATHER_CITY_LIST_CITY0_FORM_CTRL_ID, MMIWEATHER_CITY_LIST_FORM_CTRL_ID),
    CHILD_FORM_CTRL(FALSE, GUIFORM_LAYOUT_ORDER, MMIWEATHER_CITY_LIST_CITY0_CW_FORM_CTRL_ID, MMIWEATHER_CITY_LIST_CITY0_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIWEATHER_CITY_LIST_CITY0_CW_CITY_LABEL_CTRL_ID, MMIWEATHER_CITY_LIST_CITY0_CW_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIWEATHER_CITY_LIST_CITY0_CW_WEAH_LABEL_CTRL_ID, MMIWEATHER_CITY_LIST_CITY0_CW_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, FALSE, MMIWEATHER_CITY_LIST_CITY0_TEMP_LABEL_CTRL_ID, MMIWEATHER_CITY_LIST_CITY0_FORM_CTRL_ID),
    CHILD_ANIM_CTRL(FALSE, FALSE, MMIWEATHER_CITY_LIST_CITY0_WICON_ANIM_CTRL_ID, MMIWEATHER_CITY_LIST_CITY0_FORM_CTRL_ID),  
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, TRUE, MMIWEATHER_CITY_LIST_CITY0_HELP_LABEL_CTRL_ID, MMIWEATHER_CITY_LIST_CITY0_FORM_CTRL_ID),
    
    // city1
    CHILD_FORM_CTRL(TRUE, GUIFORM_LAYOUT_SBS, MMIWEATHER_CITY_LIST_CITY1_FORM_CTRL_ID, MMIWEATHER_CITY_LIST_FORM_CTRL_ID),
    CHILD_FORM_CTRL(FALSE, GUIFORM_LAYOUT_ORDER, MMIWEATHER_CITY_LIST_CITY1_CW_FORM_CTRL_ID, MMIWEATHER_CITY_LIST_CITY1_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIWEATHER_CITY_LIST_CITY1_CW_CITY_LABEL_CTRL_ID, MMIWEATHER_CITY_LIST_CITY1_CW_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIWEATHER_CITY_LIST_CITY1_CW_WEAH_LABEL_CTRL_ID, MMIWEATHER_CITY_LIST_CITY1_CW_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, FALSE, MMIWEATHER_CITY_LIST_CITY1_TEMP_LABEL_CTRL_ID, MMIWEATHER_CITY_LIST_CITY1_FORM_CTRL_ID),
    CHILD_ANIM_CTRL(FALSE, FALSE, MMIWEATHER_CITY_LIST_CITY1_WICON_ANIM_CTRL_ID, MMIWEATHER_CITY_LIST_CITY1_FORM_CTRL_ID),  
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, TRUE, MMIWEATHER_CITY_LIST_CITY1_HELP_LABEL_CTRL_ID, MMIWEATHER_CITY_LIST_CITY1_FORM_CTRL_ID),
    
    // city2
    CHILD_FORM_CTRL(TRUE, GUIFORM_LAYOUT_SBS, MMIWEATHER_CITY_LIST_CITY2_FORM_CTRL_ID, MMIWEATHER_CITY_LIST_FORM_CTRL_ID),
    CHILD_FORM_CTRL(FALSE, GUIFORM_LAYOUT_ORDER, MMIWEATHER_CITY_LIST_CITY2_CW_FORM_CTRL_ID, MMIWEATHER_CITY_LIST_CITY2_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIWEATHER_CITY_LIST_CITY2_CW_CITY_LABEL_CTRL_ID, MMIWEATHER_CITY_LIST_CITY2_CW_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIWEATHER_CITY_LIST_CITY2_CW_WEAH_LABEL_CTRL_ID, MMIWEATHER_CITY_LIST_CITY2_CW_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, FALSE, MMIWEATHER_CITY_LIST_CITY2_TEMP_LABEL_CTRL_ID, MMIWEATHER_CITY_LIST_CITY2_FORM_CTRL_ID),
    CHILD_ANIM_CTRL(FALSE, FALSE, MMIWEATHER_CITY_LIST_CITY2_WICON_ANIM_CTRL_ID, MMIWEATHER_CITY_LIST_CITY2_FORM_CTRL_ID),  
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, TRUE, MMIWEATHER_CITY_LIST_CITY2_HELP_LABEL_CTRL_ID, MMIWEATHER_CITY_LIST_CITY2_FORM_CTRL_ID),
    
#if (MMIWEATHER_MAX_CITY_NUM >= 4)
    // city3
    CHILD_FORM_CTRL(TRUE, GUIFORM_LAYOUT_SBS, MMIWEATHER_CITY_LIST_CITY3_FORM_CTRL_ID, MMIWEATHER_CITY_LIST_FORM_CTRL_ID),
    CHILD_FORM_CTRL(FALSE, GUIFORM_LAYOUT_ORDER, MMIWEATHER_CITY_LIST_CITY3_CW_FORM_CTRL_ID, MMIWEATHER_CITY_LIST_CITY3_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIWEATHER_CITY_LIST_CITY3_CW_CITY_LABEL_CTRL_ID, MMIWEATHER_CITY_LIST_CITY3_CW_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIWEATHER_CITY_LIST_CITY3_CW_WEAH_LABEL_CTRL_ID, MMIWEATHER_CITY_LIST_CITY3_CW_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, FALSE, MMIWEATHER_CITY_LIST_CITY3_TEMP_LABEL_CTRL_ID, MMIWEATHER_CITY_LIST_CITY3_FORM_CTRL_ID),
    CHILD_ANIM_CTRL(FALSE, FALSE, MMIWEATHER_CITY_LIST_CITY3_WICON_ANIM_CTRL_ID, MMIWEATHER_CITY_LIST_CITY3_FORM_CTRL_ID),  
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, TRUE, MMIWEATHER_CITY_LIST_CITY3_HELP_LABEL_CTRL_ID, MMIWEATHER_CITY_LIST_CITY3_FORM_CTRL_ID),
#endif
#if (MMIWEATHER_MAX_CITY_NUM >= 5)
    // city4
    CHILD_FORM_CTRL(TRUE, GUIFORM_LAYOUT_SBS, MMIWEATHER_CITY_LIST_CITY4_FORM_CTRL_ID, MMIWEATHER_CITY_LIST_FORM_CTRL_ID),
    CHILD_FORM_CTRL(FALSE, GUIFORM_LAYOUT_ORDER, MMIWEATHER_CITY_LIST_CITY4_CW_FORM_CTRL_ID, MMIWEATHER_CITY_LIST_CITY4_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIWEATHER_CITY_LIST_CITY4_CW_CITY_LABEL_CTRL_ID, MMIWEATHER_CITY_LIST_CITY4_CW_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIWEATHER_CITY_LIST_CITY4_CW_WEAH_LABEL_CTRL_ID, MMIWEATHER_CITY_LIST_CITY4_CW_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, FALSE, MMIWEATHER_CITY_LIST_CITY4_TEMP_LABEL_CTRL_ID, MMIWEATHER_CITY_LIST_CITY4_FORM_CTRL_ID),
    CHILD_ANIM_CTRL(FALSE, FALSE, MMIWEATHER_CITY_LIST_CITY4_WICON_ANIM_CTRL_ID, MMIWEATHER_CITY_LIST_CITY4_FORM_CTRL_ID),  
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, TRUE, MMIWEATHER_CITY_LIST_CITY4_HELP_LABEL_CTRL_ID, MMIWEATHER_CITY_LIST_CITY4_FORM_CTRL_ID),
#endif

    CHILD_FORM_CTRL(TRUE, GUIFORM_LAYOUT_SBS, MMIWEATHER_CITY_LIST_ADD_CITY_FORM_CTRL_ID, MMIWEATHER_CITY_LIST_FORM_CTRL_ID),
    CHILD_ANIM_CTRL(FALSE, TRUE, MMIWEATHER_CITY_LIST_ADD_CITY_ANIM_CTRL_ID, MMIWEATHER_CITY_LIST_ADD_CITY_FORM_CTRL_ID),
#if 0    
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, FALSE, MMIWEATHER_CITY_LIST_ADD_CITY_LABEL_CTRL_ID, MMIWEATHER_CITY_LIST_ADD_CITY_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, TRUE, MMIWEATHER_CITY_LIST_ADD_CITY_HELP_LABEL_CTRL_ID, MMIWEATHER_CITY_LIST_ADD_CITY_FORM_CTRL_ID),
#endif

#ifndef WEATHER_NO_SOFTKEYBAR_STYLE
    WIN_SOFTKEY(STXT_OPTION, TXT_NULL, STXT_RETURN),
#endif
    END_WIN
};

// 添加城市
WINDOW_TABLE(MMIWEATHER_ADD_CITY_WIN_TAB) = 
{
    WIN_FUNC( (uint32)HandleAddCityWinMsg ),    
    WIN_ID( MMIWEATHER_ADD_CITY_WIN_ID ),
    WIN_SUPPORT_ANGLE(WIN_SUPPORT_ANGLE_INIT),
    WIN_TITLE( TXT_WEATHER_ADD_CITY ),
    //WIN_BACKGROUND_ID(IMAGE_COMMON_BG),
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMIWEATHER_ADD_CITY_FORM_CTRL_ID),
    CHILD_EDIT_TEXT_CTRL(TRUE,MMIWEATHER_MAX_CITY_NAME_LEN/2,MMIWEATHER_ADD_CITY_EDIT_CTRL_ID,MMIWEATHER_ADD_CITY_FORM_CTRL_ID),
#ifdef MMI_PDA_SUPPORT
    // WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
    CHILD_SOFTKEY_CTRL(TXT_NULL, STXT_ADD, TXT_NULL,MMICOMMON_SOFTKEY_CTRL_ID,MMIWEATHER_ADD_CITY_FORM_CTRL_ID),
#else
    //CREATE_EDIT_TEXT_CTRL(MMIWEATHER_MAX_CITY_NAME_LEN, MMIWEATHER_ADD_CITY_EDIT_CTRL_ID),
    WIN_SOFTKEY(STXT_ADD, TXT_NULL, STXT_RETURN),
#endif
    END_WIN
};

//连接设置
WINDOW_TABLE( MMIWEATHER_NET_SELECT_WIN_TAB ) =
{
    WIN_FUNC((uint32)HandleNetSelectWinMsg),
    WIN_ID(MMIWEATHER_NET_SELECT_WIN_ID),
    WIN_SUPPORT_ANGLE(WIN_SUPPORT_ANGLE_INIT),
    WIN_TITLE(TXT_NETWORK_SELECT),
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER, MMIWEATHER_NET_SETTING_FORM_ID),
        //选择网络
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIWEATHER_NET_SELECT_FORM_ID,MMIWEATHER_NET_SETTING_FORM_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMIWEATHER_NET_SELECT_LABEL_ID,MMIWEATHER_NET_SELECT_FORM_ID),
        CHILD_DROPDOWNLIST_CTRL(TRUE,MMIWEATHER_NET_SELECT_DROPDOWNLIST_ID,MMIWEATHER_NET_SELECT_FORM_ID),
        //SIM1 帐户数据
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIWEATHER_NET_SELECT_FORM_SIM1_ID,MMIWEATHER_NET_SETTING_FORM_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMIWEATHER_NET_SELECT_LABEL_SIM1_ID,MMIWEATHER_NET_SELECT_FORM_SIM1_ID),
        CHILD_DROPDOWNLIST_CTRL(TRUE,MMIWEATHER_NET_SELECT_DROPDOWNLIST_SIM1_ID,MMIWEATHER_NET_SELECT_FORM_SIM1_ID),
        //SIM2 帐户数据
#if defined(MMI_MULTI_SIM_SYS_DUAL)||defined(MMI_MULTI_SIM_SYS_TRI)||defined(MMI_MULTI_SIM_SYS_QUAD)
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIWEATHER_NET_SELECT_FORM_SIM2_ID,MMIWEATHER_NET_SETTING_FORM_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMIWEATHER_NET_SELECT_LABEL_SIM2_ID,MMIWEATHER_NET_SELECT_FORM_SIM2_ID),
        CHILD_DROPDOWNLIST_CTRL(TRUE,MMIWEATHER_NET_SELECT_DROPDOWNLIST_SIM2_ID,MMIWEATHER_NET_SELECT_FORM_SIM2_ID),
#endif
        //SIM3 帐户数据
#if defined(MMI_MULTI_SIM_SYS_TRI)||defined(MMI_MULTI_SIM_SYS_QUAD)
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIWEATHER_NET_SELECT_FORM_SIM3_ID,MMIWEATHER_NET_SETTING_FORM_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMIWEATHER_NET_SELECT_LABEL_SIM3_ID,MMIWEATHER_NET_SELECT_FORM_SIM3_ID),
        CHILD_DROPDOWNLIST_CTRL(TRUE,MMIWEATHER_NET_SELECT_DROPDOWNLIST_SIM3_ID,MMIWEATHER_NET_SELECT_FORM_SIM3_ID),
#endif
        //SIM4 帐户数据
#if defined(MMI_MULTI_SIM_SYS_QUAD)
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIWEATHER_NET_SELECT_FORM_SIM4_ID,MMIWEATHER_NET_SETTING_FORM_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMIWEATHER_NET_SELECT_LABEL_SIM4_ID,MMIWEATHER_NET_SELECT_FORM_SIM4_ID),
        CHILD_DROPDOWNLIST_CTRL(TRUE,MMIWEATHER_NET_SELECT_DROPDOWNLIST_SIM4_ID,MMIWEATHER_NET_SELECT_FORM_SIM4_ID),
#endif

    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN    
};


// 设置
WINDOW_TABLE(MMIWEATHER_SETTING_WIN_TAB) = 
{
    WIN_FUNC( (uint32)HandleWeatherSettingWinMsg ),    
    WIN_ID( MMIWEATHER_SETTING_WIN_ID ),
    WIN_TITLE( STXT_MAIN_SETTING ),
    WIN_SUPPORT_ANGLE(WIN_SUPPORT_ANGLE_INIT),
#ifdef MMI_PDA_SUPPORT
    WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
#endif
    WIN_SOFTKEY(STXT_SAVE, TXT_NULL, STXT_RETURN),
    
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER, MMIWEATHER_SETTING_FORM_CTRL_ID),
    
    CHILD_FORM_CTRL(TRUE, GUIFORM_LAYOUT_ORDER, MMIWEATHER_SETTING_AUTO_UPDATE_FORM_CTRL_ID, MMIWEATHER_SETTING_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIWEATHER_SETTING_AUTO_UPDATE_LABEL_CTRL_ID, MMIWEATHER_SETTING_AUTO_UPDATE_FORM_CTRL_ID),
    CHILD_SETLIST_CTRL(TRUE, MMIWEATHER_SETTING_AUTO_UPDATE_SETLIST_CTRL_ID, MMIWEATHER_SETTING_AUTO_UPDATE_FORM_CTRL_ID),
    
    CHILD_FORM_CTRL(TRUE, GUIFORM_LAYOUT_ORDER, MMIWEATHER_SETTING_UPDATE_INTERVAL_FORM_CTRL_ID, MMIWEATHER_SETTING_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIWEATHER_SETTING_UPDATE_INTERVAL_LABEL_CTRL_ID, MMIWEATHER_SETTING_UPDATE_INTERVAL_FORM_CTRL_ID),
    CHILD_DROPDOWNLIST_CTRL(TRUE, MMIWEATHER_SETTING_UPDATE_INTERVAL_DROPDOWNLIST_CTRL_ID, MMIWEATHER_SETTING_UPDATE_INTERVAL_FORM_CTRL_ID),
    
    CHILD_FORM_CTRL(TRUE, GUIFORM_LAYOUT_ORDER, MMIWEATHER_SETTING_UPDATE_BEGIN_TIME_FORM_CTRL_ID, MMIWEATHER_SETTING_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIWEATHER_SETTING_UPDATE_BEGIN_TIME_LABEL_CTRL_ID, MMIWEATHER_SETTING_UPDATE_BEGIN_TIME_FORM_CTRL_ID),
    CHILD_DROPDOWNLIST_CTRL(TRUE, MMIWEATHER_SETTING_UPDATE_BEGIN_TIME_DROPDOWNLIST_CTRL_ID, MMIWEATHER_SETTING_UPDATE_BEGIN_TIME_FORM_CTRL_ID),
    
    CHILD_FORM_CTRL(TRUE, GUIFORM_LAYOUT_ORDER, MMIWEATHER_SETTING_UPDATE_END_TIME_FORM_CTRL_ID, MMIWEATHER_SETTING_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIWEATHER_SETTING_UPDATE_END_TIME_LABEL_CTRL_ID, MMIWEATHER_SETTING_UPDATE_END_TIME_FORM_CTRL_ID),
    CHILD_DROPDOWNLIST_CTRL(TRUE, MMIWEATHER_SETTING_UPDATE_END_TIME_DROPDOWNLIST_CTRL_ID, MMIWEATHER_SETTING_UPDATE_END_TIME_FORM_CTRL_ID),
    
    CHILD_FORM_CTRL(TRUE, GUIFORM_LAYOUT_ORDER, MMIWEATHER_SETTING_TEMPERATURE_UNIT_FORM_CTRL_ID, MMIWEATHER_SETTING_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIWEATHER_SETTING_TEMPERATURE_UNIT_LABEL_CTRL_ID, MMIWEATHER_SETTING_TEMPERATURE_UNIT_FORM_CTRL_ID),
    CHILD_DROPDOWNLIST_CTRL(TRUE, MMIWEATHER_SETTING_TEMPERATURE_UNIT_DROPDOWNLIST_CTRL_ID, MMIWEATHER_SETTING_TEMPERATURE_UNIT_FORM_CTRL_ID),

    //连接设置
    CHILD_FORM_CTRL(TRUE, GUIFORM_LAYOUT_ORDER, MMIWEATHER_SETTING_NET_SELECT_FORM_CTRL_ID, MMIWEATHER_SETTING_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIWEATHER_SETTING_NET_SELECT_LABEL_CTRL_ID, MMIWEATHER_SETTING_NET_SELECT_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, TRUE, MMIWEATHER_SETTING_NET_SELECT_LABEL1_CTRL_ID, MMIWEATHER_SETTING_NET_SELECT_FORM_CTRL_ID),
    END_WIN
};

// 设置菜单
WINDOW_TABLE(MMIWEATHER_MAINMENU_WIN_TAB) = 
{
    WIN_FUNC( (uint32)HandleMainMenuWinMsg ),    
    WIN_ID( MMIWEATHER_MENU_WIN_ID ),
#if defined MMI_RES_ORIENT_BOTH || defined MMI_RES_ORIENT_PORTRAIT
    WIN_TITLE(TXT_NULL),
#endif
    WIN_SUPPORT_ANGLE(WIN_SUPPORT_ANGLE_INIT),
    CREATE_POPMENU_CTRL(MMIWEATHER_MAINMENU_OPT, MMIWEATHER_MAINMENU_CTRL_ID),
    
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};

#ifdef MMI_WEATHER_JSON_PARSE
WINDOW_TABLE(MMIWEATHER_COUNTRY_LIST_WINTAB)=
{
    WIN_FUNC( (uint32)HandleCountyListWinMsg ),    
    WIN_ID( MMIWEATHER_COUNTRY_LIST_ID ),
    WIN_TITLE(TXT_PLS_SEL_COUNTRY),
    WIN_SUPPORT_ANGLE(WIN_SUPPORT_ANGLE_INIT),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIWEATHER_COUNTRY_LIST_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};

WINDOW_TABLE(MMIWEATHER_ABOUT_WINTAB)=
{
    WIN_FUNC( (uint32)HandleWeatherAboutWinMsg ),    
    WIN_ID( MMIWEATHER_ABOUT_WIN_ID ),
    WIN_TITLE(TXT_WEATHER_ABOUT),
    WIN_SUPPORT_ANGLE(WIN_SUPPORT_ANGLE_INIT),
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_RETURN),
    END_WIN
};
#endif

LOCAL uint32 s_city_form_ctrl_id_arr[] =
{
    MMIWEATHER_CITY_LIST_CITY0_FORM_CTRL_ID,
    MMIWEATHER_CITY_LIST_CITY1_FORM_CTRL_ID,
    MMIWEATHER_CITY_LIST_CITY2_FORM_CTRL_ID,
#if (MMIWEATHER_MAX_CITY_NUM >= 4)
    MMIWEATHER_CITY_LIST_CITY3_FORM_CTRL_ID,
#endif
#if (MMIWEATHER_MAX_CITY_NUM >= 5)
    MMIWEATHER_CITY_LIST_CITY4_FORM_CTRL_ID,
#endif
};

/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/

/****************************************************************************/
//  Description : Open Weather Main Window
//  Global resource dependence : 
//  Author:violent.wei
//  Note: 
/****************************************************************************/
PUBLIC void MMIWEATHER_OpenMainWin(void)
{
    MMK_CreateWin((uint32*)MMIWEATHER_MAIN_WIN_TAB, PNULL);
}

/****************************************************************************/
//  Description : 得到温度值+单位符号字符串
//  Global resource dependence : 
//  Author:violent.wei
//  Note: 
/****************************************************************************/
LOCAL void GetTemperatureString(
                          int32 temp,
                          MMI_STRING_T* string_ptr
                          )
{
    char temp_str[WEATHER_MAX_STRING_LEN + 1] = {""};
    MMI_STRING_T unit_str = {0};
    MMIWEATHER_SETTING_T* setting_ptr = MMIWEATHER_GetSettingInfo();
    
    if (MMIWEATHER_TEMP_UNKNOW == temp)
    {
        string_ptr->wstr_len = 0;
        return;
    }
    
    // 转换成摄氏温度
    if (WEATHER_TEMP_UNIT_C == setting_ptr->temp_unit)
    {
        temp = MMIWEATHER_GetTemperature(TEMP_F2C(temp));
    }
    
    // 获得数字的字符串
    sprintf((char*)temp_str, "%d", temp);
    string_ptr->wstr_len = strlen((char *)temp_str);
    // 转换成宽字符
    MMI_STRNTOWSTR(string_ptr->wstr_ptr,
        WEATHER_MAX_STRING_LEN,
        (uint8*)temp_str,
        WEATHER_MAX_STRING_LEN,
        string_ptr->wstr_len);

    // 加单位符号
    MMI_GetLabelTextByLang((WEATHER_TEMP_UNIT_C == setting_ptr->temp_unit) ? TXT_WEATHER_CEL_SIGNAL : TXT_WEATHER_FAH_SIGNAL, &unit_str);
    MMI_WSTRNCPY(string_ptr->wstr_ptr + string_ptr->wstr_len, unit_str.wstr_len, unit_str.wstr_ptr, unit_str.wstr_len, unit_str.wstr_len);
    string_ptr->wstr_len += unit_str.wstr_len;
    string_ptr->wstr_ptr[string_ptr->wstr_len] = 0;
}

/****************************************************************************/
//  Description : 得到X-Y温度字符串
//  Global resource dependence : 
//  Author:violent.wei
//  Note: 
/****************************************************************************/
LOCAL void GetTemperatureRangeString(int32 low,
                               int32 high,
                               MMI_STRING_T* string_ptr,
                               BOOLEAN is_need_uint_str
                               )
{
    uint32 len = 0;
    char temp_str[WEATHER_MAX_STRING_LEN + 1] = {""};
    MMI_STRING_T unit_str = {0};
    MMIWEATHER_SETTING_T* setting_ptr = MMIWEATHER_GetSettingInfo();
    
    if (MMIWEATHER_TEMP_UNKNOW == low || MMIWEATHER_TEMP_UNKNOW == high)
    {
        string_ptr->wstr_len = 0;
        return;
    }
    
    // 转换成摄氏度
    if (WEATHER_TEMP_UNIT_C == setting_ptr->temp_unit)
    {
        low  = MMIWEATHER_GetTemperature(TEMP_F2C(low));
        high = MMIWEATHER_GetTemperature(TEMP_F2C(high));
    }
    // 数字字符串
    sprintf((char*)temp_str, "%d/", low);
    string_ptr->wstr_len = strlen((char *)temp_str);
    // 宽字符
    MMI_STRNTOWSTR(string_ptr->wstr_ptr,
        WEATHER_MAX_STRING_LEN,
        (uint8*)temp_str,
        WEATHER_MAX_STRING_LEN,
        string_ptr->wstr_len);
    
    sprintf((char*)temp_str, "%d", high);
    len = strlen((char *)temp_str);
    // 温度串联起来
    MMI_STRNTOWSTR(string_ptr->wstr_ptr + string_ptr->wstr_len,
        WEATHER_MAX_STRING_LEN,
        (uint8*)temp_str,
        WEATHER_MAX_STRING_LEN,
        len);
    string_ptr->wstr_len += len;

    if(is_need_uint_str)
    {
        // 加符号
        MMI_GetLabelTextByLang((WEATHER_TEMP_UNIT_C == setting_ptr->temp_unit) ? TXT_WEATHER_CEL_SIGNAL : TXT_WEATHER_FAH_SIGNAL, &unit_str);
        MMI_WSTRNCPY(string_ptr->wstr_ptr + string_ptr->wstr_len, unit_str.wstr_len, unit_str.wstr_ptr, unit_str.wstr_len, unit_str.wstr_len);
        string_ptr->wstr_len += unit_str.wstr_len;
        string_ptr->wstr_ptr[string_ptr->wstr_len] = 0;
    }
}

/****************************************************************************/
//  Description : 得到风向+风力字符串
//  Global resource dependence : 
//  Author:violent.wei
//  Note: 
/****************************************************************************/
LOCAL void GetWindInfoString(
                       MMIWEATHER_WIND_DIR_E wind_dir,
                       int32 wind_force,
                       MMI_STRING_T* string_ptr
                       )
{
    MMI_STRING_T wind_dir_str = {0};
    MMI_STRING_T wind_level_str = {0};

    if(WIND_DIR_UNKNOW == wind_dir || MMIWEATHER_WIND_UNKNOW == wind_force)
    {
        string_ptr->wstr_len = 0;
        return;
    }
    
    // 风向字符串
    MMI_GetLabelTextByLang(MMIWEATHER_GetWindDirTxtId(wind_dir), &wind_dir_str);
    MMI_WSTRNCPY(string_ptr->wstr_ptr, wind_dir_str.wstr_len, wind_dir_str.wstr_ptr, wind_dir_str.wstr_len, wind_dir_str.wstr_len);
    // 加一个空格
    string_ptr->wstr_len = wind_dir_str.wstr_len;
    string_ptr->wstr_ptr[wind_dir_str.wstr_len] = 0x20;
    string_ptr->wstr_len ++;
    // 风力字符串
    MMIWEATHER_GetWindForceStr(wind_force, string_ptr->wstr_ptr + string_ptr->wstr_len);
    string_ptr->wstr_len = MMIAPICOM_Wstrlen(string_ptr->wstr_ptr);
    // 风力单位
    MMI_GetLabelTextByLang(TXT_WEATHER_WIND_LEVEL, &wind_level_str);
    MMI_WSTRNCPY(string_ptr->wstr_ptr + string_ptr->wstr_len, wind_level_str.wstr_len, wind_level_str.wstr_ptr, wind_level_str.wstr_len, wind_level_str.wstr_len);
    string_ptr->wstr_len += wind_level_str.wstr_len;
    string_ptr->wstr_ptr[string_ptr->wstr_len] = 0;
}

/****************************************************************************/
//  Description : 得到湿度:x%字符串
//  Global resource dependence : 
//  Author:violent.wei
//  Note: 
/****************************************************************************/
LOCAL void GetHumidityString(
                       wchar* humidity_data_str,
                       MMI_STRING_T* string_ptr
                       )
{
    MMI_STRING_T humidity_str = {0};
    
    if(0 == MMIAPICOM_Wstrlen(humidity_data_str))
    {
        string_ptr->wstr_len = 0;
        return;
    }

    
    // 湿度字符串
    MMI_GetLabelTextByLang(TXT_WEATHER_HUMIDITY, &humidity_str);
    MMI_WSTRNCPY(string_ptr->wstr_ptr, humidity_str.wstr_len, humidity_str.wstr_ptr, humidity_str.wstr_len, humidity_str.wstr_len);
    string_ptr->wstr_len = humidity_str.wstr_len;
    
    // 数字+%
    MMIAPICOM_Wstrcpy(string_ptr->wstr_ptr + string_ptr->wstr_len, humidity_data_str);
    string_ptr->wstr_len = MMIAPICOM_Wstrlen(string_ptr->wstr_ptr);
}

/****************************************************************************/
//  Description : 得到更新时间信息:xx/xx xx:xx字符串
//  Global resource dependence : 
//  Author:violent.wei
//  Note: 
/****************************************************************************/
LOCAL void GetUpdateTimeString(
                         wchar* date_str,
                         wchar* time_str,
                         MMI_STRING_T* string
                         )
{
    //MMI_STRING_T update_at_str = {0};
    MMI_STRING_T public_at_str = {0};

#if 0    
    // 更新于:
    MMI_GetLabelTextByLang(TXT_WEATHER_UPDATED_AT, &update_at_str);
    MMIAPICOM_Wstrncpy(string->wstr_ptr, update_at_str.wstr_ptr, update_at_str.wstr_len);
    string->wstr_len = update_at_str.wstr_len;
#else    
    // 发布于:
    MMI_GetLabelTextByLang(TXT_WEATHER_PUBLISHED_AT, &public_at_str);
    MMIAPICOM_Wstrncpy(string->wstr_ptr, public_at_str.wstr_ptr, public_at_str.wstr_len);
    string->wstr_len = public_at_str.wstr_len;
#endif    

    // 日期
    MMIAPICOM_Wstrcpy(string->wstr_ptr + string->wstr_len, date_str);
    string->wstr_len += MMIAPICOM_Wstrlen(date_str);
    
    MMIAPICOM_Wstrcat(string->wstr_ptr, (wchar*)L" ");
    string->wstr_len += 1;
    
    // 时间
    MMIAPICOM_Wstrcpy(string->wstr_ptr + string->wstr_len, time_str);
    string->wstr_len += MMIAPICOM_Wstrlen(time_str);
    
    string->wstr_ptr[string->wstr_len] = 0;
}

/****************************************************************************/
//  Description : 得到未来天气数据，因为未来天气不是数组，所以需要提取
//  Global resource dependence : 
//  Author:violent.wei
//  Note: 
/****************************************************************************/
PUBLIC void MMIWEATHER_GetFCId(
                        MMIWEATHER_FC_CONDITIONS_T* fc_conditions_ptr, 
                        uint8 index, 
                        MMIWEATHER_WEEK_E* week_id_ptr,
                        MMIWEATHER_CONDITION_E* condition_id_ptr,
                        int32* low_ptr,
                        int32* high_ptr,
                        wchar** date_wstr_ptr
                        )
{

    if((NULL == fc_conditions_ptr) 
            || (NULL == week_id_ptr) 
            || (NULL == low_ptr) 
            || (NULL == high_ptr) 
            || (NULL == date_wstr_ptr) 
            || (NULL == condition_id_ptr))
    {
    	//SCI_TRACE_LOW:"[weather]:MMIWEATHER_GetFCId---Bad para!"
    	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWEATHER_WIN_1105_112_2_18_3_6_46_74,(uint8*)"");
        return;
    }
    
    switch(index)
    {
    case 0:
        *week_id_ptr = fc_conditions_ptr->week1;
        *condition_id_ptr = fc_conditions_ptr->weather1;
        *low_ptr = fc_conditions_ptr->low1;
        *high_ptr = fc_conditions_ptr->high1;
        *date_wstr_ptr = fc_conditions_ptr->date1;
        break;

    case 1:
        *week_id_ptr = fc_conditions_ptr->week2;
        *condition_id_ptr = fc_conditions_ptr->weather2;
        *low_ptr = fc_conditions_ptr->low2;
        *high_ptr = fc_conditions_ptr->high2;
        *date_wstr_ptr = fc_conditions_ptr->date2;
        break;

    case 2:
        *week_id_ptr = fc_conditions_ptr->week3;
        *condition_id_ptr = fc_conditions_ptr->weather3;
        *low_ptr = fc_conditions_ptr->low3;
        *high_ptr = fc_conditions_ptr->high3;
        *date_wstr_ptr = fc_conditions_ptr->date3;
        break;

    case 3:
        *week_id_ptr = fc_conditions_ptr->week4;
        *condition_id_ptr = fc_conditions_ptr->weather4;
        *low_ptr = fc_conditions_ptr->low4;
        *high_ptr = fc_conditions_ptr->high4;
        *date_wstr_ptr = fc_conditions_ptr->date4;
        break;

    default:
        break;
    }
}

/****************************************************************************/
//  Description : 计算温度对应的坐标点，进而连线
//  Global resource dependence : 
//  Author:violent.wei
//  Note: 
/****************************************************************************/
LOCAL void CalculateTempPoint(
                         MMIWEATHER_FC_CONDITIONS_T* fc_conditions_ptr,
                         GUI_POINT_T* temp_high_point_ptr,
                         GUI_POINT_T* temp_low_point_ptr,
                         uint32 num
                         )
{
    uint32 span = 0;
    int16 temp_high_max = fc_conditions_ptr->high1;
    int16 temp_low_min = fc_conditions_ptr->low1;
    
    // 获取四天最高温度
    if (temp_high_max < fc_conditions_ptr->high2)
        temp_high_max = fc_conditions_ptr->high2;
    if (temp_high_max < fc_conditions_ptr->high3)
        temp_high_max = fc_conditions_ptr->high3;
    if (temp_high_max < fc_conditions_ptr->high4)
        temp_high_max = fc_conditions_ptr->high4;
    
    // 获取四天最低温度
    if (temp_low_min > fc_conditions_ptr->low2)
        temp_low_min = fc_conditions_ptr->low2;
    if (temp_low_min > fc_conditions_ptr->low3)
        temp_low_min = fc_conditions_ptr->low3;
    if (temp_low_min > fc_conditions_ptr->low4)
        temp_low_min = fc_conditions_ptr->low4;
    
    // 保证区间不为0
    if (temp_high_max <= temp_low_min) temp_high_max = temp_low_min + 1;
    span = temp_high_max - temp_low_min;
    
    // 根据最高温度在top,最低温度在bottom，折算坐标
    temp_high_point_ptr[0].y = WEATHER_TEMP_LINE_BOTTOM - (uint32)(fc_conditions_ptr->high1 - temp_low_min) * WEATHER_TEMP_LINE_SPAN / span;
    temp_high_point_ptr[1].y = WEATHER_TEMP_LINE_BOTTOM - (uint32)(fc_conditions_ptr->high2 - temp_low_min) * WEATHER_TEMP_LINE_SPAN / span;
    temp_high_point_ptr[2].y = WEATHER_TEMP_LINE_BOTTOM - (uint32)(fc_conditions_ptr->high3 - temp_low_min) * WEATHER_TEMP_LINE_SPAN / span;
    temp_high_point_ptr[3].y = WEATHER_TEMP_LINE_BOTTOM - (uint32)(fc_conditions_ptr->high4 - temp_low_min) * WEATHER_TEMP_LINE_SPAN / span;
    
    temp_low_point_ptr[0].y = WEATHER_TEMP_LINE_BOTTOM - (uint32)(fc_conditions_ptr->low1 - temp_low_min) * WEATHER_TEMP_LINE_SPAN / span;
    temp_low_point_ptr[1].y = WEATHER_TEMP_LINE_BOTTOM - (uint32)(fc_conditions_ptr->low2 - temp_low_min) * WEATHER_TEMP_LINE_SPAN / span;
    temp_low_point_ptr[2].y = WEATHER_TEMP_LINE_BOTTOM - (uint32)(fc_conditions_ptr->low3 - temp_low_min) * WEATHER_TEMP_LINE_SPAN / span;
    temp_low_point_ptr[3].y = WEATHER_TEMP_LINE_BOTTOM - (uint32)(fc_conditions_ptr->low4 - temp_low_min) * WEATHER_TEMP_LINE_SPAN / span;
}

/****************************************************************************/
//  Description : Load City list form, set its parameter for displaying
//  Global resource dependence : 
//  Author:violent.wei
//  Note: 
/****************************************************************************/
LOCAL void LoadCityListControl(
                          MMI_WIN_ID_T        win_id
                          )
{
    int32 i = 0;
    GUIFORM_CHILD_HEIGHT_T height = {WEATHER_CITY_LIST_ITEM_HEIGHT, GUIFORM_CHILD_HEIGHT_FIXED};
    GUIFORM_CHILD_WIDTH_T width = {0, GUIFORM_CHILD_WIDTH_FIXED};
    GUI_BG_T gui_bg = {0};
    GUI_RECT_T form_rect = {0};

    GUICTRL_GetRect(MMK_GetCtrlPtr(MMIWEATHER_CITY_LIST_FORM_CTRL_ID), &form_rect);
    form_rect.top += WEATHER_CITY_LIST_TOP_MERGIN;
    GUICTRL_SetRect(MMK_GetCtrlPtr(MMIWEATHER_CITY_LIST_FORM_CTRL_ID), &form_rect);
    
    // 背景图，蒙版
    gui_bg.bg_type = GUI_BG_IMG;
    gui_bg.img_id  = IMAGE_WEATHER_CITY_LIST_BG;
    GUIFORM_SetBg(MMIWEATHER_CITY_LIST_FORM_CTRL_ID, &gui_bg);
    
    for (i = 0; i < MMIWEATHER_MAX_CITY_NUM; i ++)
    {
        // 设置item高度，间距，字体
        uint16 v_space = WEATHER_CITY_LIST_ITEM_SPACE_HEIGHT;
        GUIFORM_SetChildHeight(MMIWEATHER_CITY_LIST_FORM_CTRL_ID, s_city_form_ctrl_id_arr[i], &height);
        GUIFORM_SetChildSpace(MMIWEATHER_CITY_LIST_FORM_CTRL_ID, s_city_form_ctrl_id_arr[i], NULL, &v_space);
        GUIFORM_SetStyle(s_city_form_ctrl_id_arr[i] + MMIWEATHER_CW_FORM_CTRL_ID_OFFSET, GUIFORM_STYLE_UNIT);
        GUIFORM_SetStyle(s_city_form_ctrl_id_arr[i] + MMIWEATHER_FORM_CTRL_ID_OFFSET, GUIFORM_STYLE_UNIT);
        GUIFORM_PermitChildFont(s_city_form_ctrl_id_arr[i] + MMIWEATHER_FORM_CTRL_ID_OFFSET, FALSE);
        GUILABEL_SetFont(s_city_form_ctrl_id_arr[i] + MMIWEATHER_CW_WEAH_LABEL_CTRL_ID_OFFSET, MMI_DEFAULT_SMALL_FONT, MMI_WHITE_COLOR);
#if defined MAINLCD_SIZE_240X320 || defined MAINLCD_SIZE_240X400
        GUILABEL_SetFont(s_city_form_ctrl_id_arr[i] + MMIWEATHER_CW_CITY_LABEL_CTRL_ID_OFFSET, MMI_DEFAULT_SMALL_FONT, MMI_WHITE_COLOR);
#else
        GUILABEL_SetFont(s_city_form_ctrl_id_arr[i] + MMIWEATHER_CW_CITY_LABEL_CTRL_ID_OFFSET, MMI_DEFAULT_NORMAL_FONT, MMI_WHITE_COLOR);
#endif
        GUILABEL_SetFont(s_city_form_ctrl_id_arr[i] + MMIWEATHER_TEMP_LABEL_CTRL_ID_OFFSET, MMI_DEFAULT_NORMAL_FONT, MMI_WHITE_COLOR);

        // 设置每个item中子item的宽度
        width.add_data = 1;
        GUIFORM_SetChildWidth(s_city_form_ctrl_id_arr[i], s_city_form_ctrl_id_arr[i] + MMIWEATHER_HELP_LABEL_CTRL_ID_OFFSET, &width);
        width.add_data = WEATHER_CITY_LIST_ITEM_CITYNAME_WIDTH;
        GUIFORM_SetChildWidth(s_city_form_ctrl_id_arr[i], s_city_form_ctrl_id_arr[i] + MMIWEATHER_CW_FORM_CTRL_ID_OFFSET, &width);
        width.add_data = WEATHER_CITY_LIST_ITEM_WEAICON_WIDTH;
        GUIFORM_SetChildWidth(s_city_form_ctrl_id_arr[i], s_city_form_ctrl_id_arr[i] + MMIWEATHER_WICON_ANIM_CTRL_ID_OFFSET, &width);
    }
    GUIFORM_SetStyle(MMIWEATHER_CITY_LIST_ADD_CITY_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
    GUIFORM_SetChildHeight(MMIWEATHER_CITY_LIST_FORM_CTRL_ID, MMIWEATHER_CITY_LIST_ADD_CITY_FORM_CTRL_ID, &height);
    GUIFORM_PermitChildBg(MMIWEATHER_CITY_LIST_ADD_CITY_FORM_CTRL_ID, FALSE);
    GUIFORM_PermitChildBorder(MMIWEATHER_CITY_LIST_ADD_CITY_FORM_CTRL_ID, FALSE);
}

/****************************************************************************/
//  Description : Load City list data, insert into form
//  Global resource dependence : 
//  Author:violent.wei
//  Note: 
/****************************************************************************/
LOCAL void LoadCityListData(
                            MMI_WIN_ID_T        win_id
                            )
{
    int32 i = 0;
    MMIWEATHER_SETTING_T* setting_ptr = MMIWEATHER_GetSettingInfo();
    GUIANIM_CTRL_INFO_T     control_info = {0};
    GUIANIM_DATA_INFO_T     data_info =    {0};
    GUIANIM_DISPLAY_INFO_T  display_info = {0};
    
    for (i = 0; i < MMIWEATHER_MAX_CITY_NUM; i ++)
    {
        if (i < setting_ptr->city_num)
        {
            MMIWEATHER_WEATHER_T*   weather_info = MMIWEATHER_GetCityWeatherInfo(i);
            MMI_TEXT_ID_T condition_text_id = MMIWEATHER_GetConditionTxtId(weather_info->current.condition);
            MMI_IMAGE_ID_T condition_img_id = MMIWEATHER_GetConditionImgId(weather_info->current.condition, WEATHER_ICON_MIDDLE);
            
            //wchar wstr[WEATHER_MAX_STRING_LEN + 1] = {0};
            MMI_STRING_T string = {0};
            
            // 城市名
            string.wstr_ptr = weather_info->city_name;
            string.wstr_len = MMIAPICOM_Wstrlen(weather_info->city_name);
            GUILABEL_SetText(s_city_form_ctrl_id_arr[i] + MMIWEATHER_CW_CITY_LABEL_CTRL_ID_OFFSET, &string, FALSE);
            // 天气字符串
            GUILABEL_SetTextById(s_city_form_ctrl_id_arr[i] + MMIWEATHER_CW_WEAH_LABEL_CTRL_ID_OFFSET, condition_text_id, FALSE);
            
            // 如果天气数据曾被更新过，就显示温度
            if (MMIWEATHER_IsCityWeatherInfoLegal(i))
            {
                string.wstr_ptr = MMIWEATHER_GetCurTempRangeWstr(i);
                string.wstr_len = MMIAPICOM_Wstrlen(string.wstr_ptr);
            }
            else
            {
                string.wstr_len = 0;
            }
            GUILABEL_SetText(s_city_form_ctrl_id_arr[i] + MMIWEATHER_TEMP_LABEL_CTRL_ID_OFFSET, &string, FALSE);
            
            // 显示天气图标
            control_info.is_ctrl_id = TRUE;
            control_info.ctrl_id = s_city_form_ctrl_id_arr[i] + MMIWEATHER_WICON_ANIM_CTRL_ID_OFFSET;
            display_info.is_syn_decode = TRUE;
            display_info.align_style = GUIANIM_ALIGN_HVMIDDLE;
            data_info.img_id = condition_img_id;
            GUIANIM_SetParam(&control_info, &data_info, PNULL, &display_info);
            
            // 显示此form
            GUIFORM_SetChildDisplay(MMIWEATHER_CITY_LIST_FORM_CTRL_ID, s_city_form_ctrl_id_arr[i], GUIFORM_CHILD_DISP_NORMAL);
            
        }
        else
        {
            // 隐藏此form
            GUIFORM_SetChildDisplay(MMIWEATHER_CITY_LIST_FORM_CTRL_ID, s_city_form_ctrl_id_arr[i], GUIFORM_CHILD_DISP_HIDE);
        }
    }
    
    {
        GUIFORM_CHILD_DISPLAY_E child_display = setting_ptr->city_num < MMIWEATHER_MAX_CITY_NUM ? GUIFORM_CHILD_DISP_NORMAL : GUIFORM_CHILD_DISP_HIDE;

        control_info.is_ctrl_id = TRUE;
        control_info.ctrl_id = MMIWEATHER_CITY_LIST_ADD_CITY_ANIM_CTRL_ID;
        display_info.is_syn_decode = TRUE;
        display_info.align_style = GUIANIM_ALIGN_LEFT_TOP;
        data_info.img_id = IMAGE_WEATHER_ADD_CITY;

        GUIANIM_SetParam(&control_info, &data_info, PNULL, &display_info);
        GUIFORM_SetChildDisplay(MMIWEATHER_CITY_LIST_FORM_CTRL_ID, MMIWEATHER_CITY_LIST_ADD_CITY_FORM_CTRL_ID, child_display);
    }
       
    DrawWeatherCityList();
    // 根据当前城市设置焦点
    if(setting_ptr->city_num > 0)
    {
        MMK_SetAtvCtrl(win_id, s_city_form_ctrl_id_arr[setting_ptr->cur_city] + MMIWEATHER_HELP_LABEL_CTRL_ID_OFFSET);
    }
}

/****************************************************************************/
//  Description : 通过触摸点，设置FORM的Active Item
//  Global resource dependence : 
//  Author:violent.wei
//  Note: 
/****************************************************************************/
LOCAL int32 SetFormActiveItem(
                                int16 point_y
                                )
{
    int32 i = 0;
    MMI_WIN_ID_T win_id = MMIWEATHER_MAIN_WIN_ID;
    MMIWEATHER_SETTING_T* setting_ptr = MMIWEATHER_GetSettingInfo();
    
    for (i = 0; i < setting_ptr->city_num; i ++)
    {
        GUI_RECT_T rect = {0};
        GUICTRL_GetRect(MMK_GetCtrlPtr(s_city_form_ctrl_id_arr[i]), &rect);
        if (rect.top <= point_y && rect.bottom >= point_y)
        {
            MMK_SetAtvCtrl(win_id, s_city_form_ctrl_id_arr[i]);
            break;
        }
    }

    return i;
}

/****************************************************************************/
//  Description : 主界面UI Layer创建
//  Global resource dependence : 
//  Author:violent.wei
//  Note: 
/****************************************************************************/
LOCAL void UILayerCreate()
{
    //int32               i = 0;
    UILAYER_CREATE_T    create_info = {0};
    uint16              logic_width = 0;
    uint16              logic_height = 0;

    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &logic_width, &logic_height);
    
    create_info.lcd_id = GUI_MAIN_LCD_ID;
    create_info.owner_handle = MMIWEATHER_MAIN_WIN_ID;
    create_info.offset_x = 0;
    create_info.offset_y = 0;
    create_info.width = logic_width;
    create_info.height = logic_height;
    create_info.is_bg_layer = FALSE;
    create_info.is_static_layer = FALSE;

    UILAYER_CreateLayer(&create_info, &s_citylist_lcd_dev_info);
    UILAYER_SetLayerColorKey(&s_citylist_lcd_dev_info, TRUE, UILAYER_TRANSPARENT_COLOR);

    UILAYER_CreateLayer(&create_info, &s_details_lcd_dev_info);
    UILAYER_SetLayerColorKey(&s_details_lcd_dev_info, TRUE, UILAYER_TRANSPARENT_COLOR);

    UILAYER_CreateLayer(&create_info, &s_trends_lcd_dev_info);
    UILAYER_SetLayerColorKey(&s_trends_lcd_dev_info, TRUE, UILAYER_TRANSPARENT_COLOR);
}

/****************************************************************************/
//  Description : 主界面UI Layer销毁
//  Global resource dependence : 
//  Author:violent.wei
//  Note: 
/****************************************************************************/
LOCAL void UILayerDestroy(void)
{
    UILAYER_RemoveBltLayer(&s_citylist_lcd_dev_info);
    UILAYER_RemoveBltLayer(&s_details_lcd_dev_info);
    UILAYER_RemoveBltLayer(&s_trends_lcd_dev_info);

    UILAYER_ReleaseAllLayerOfHandle(MMIWEATHER_MAIN_WIN_ID);
}

/****************************************************************************/
//  Description : 主界面UI Layer附着
//  Global resource dependence : 
//  Author:violent.wei
//  Note: 
/****************************************************************************/
LOCAL void UILayerAppend(
                         WEATHER_WIN_PAGE win_page
                         )
{
    UILAYER_APPEND_BLT_T append_layer = {0};

    append_layer.layer_level = UILAYER_LEVEL_NORMAL;

    switch(win_page)
    {
    case WEATHER_WIN_CITYLIST:
        append_layer.lcd_dev_info = s_citylist_lcd_dev_info;
        break;

    case WEATHER_WIN_DETAILS:
        append_layer.lcd_dev_info = s_details_lcd_dev_info;
        break;

    case WEATHER_WIN_TRENDS:
        append_layer.lcd_dev_info = s_trends_lcd_dev_info;
        break;
    }
    
    UILAYER_AppendBltLayer(&append_layer);
}

/****************************************************************************/
//  Description : 主界面UI Layer分离
//  Global resource dependence : 
//  Author:violent.wei
//  Note: 
/****************************************************************************/
LOCAL void UILayerRemoveOthers(
                               WEATHER_WIN_PAGE win_page
                               )
{
    if (WEATHER_WIN_CITYLIST != win_page)
    {
        UILAYER_RemoveBltLayer(&s_citylist_lcd_dev_info);
    }
    if (WEATHER_WIN_DETAILS != win_page)
    {
        UILAYER_RemoveBltLayer(&s_details_lcd_dev_info);
    }
    if (WEATHER_WIN_TRENDS != win_page)
    {
        UILAYER_RemoveBltLayer(&s_trends_lcd_dev_info);
    }
}

/****************************************************************************/
//  Description : 切换当前page
//  Global resource dependence : 
//  Author:violent.wei
//  Note: 
/****************************************************************************/
LOCAL void SwitchPage(
                      WEATHER_WIN_PAGE weather_win_page, 
                      BOOLEAN is_slide_effect
                      )
{
    int16               step = 0;
    uint16              logic_width = 0;
    uint16              logic_height = 0;
    
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &logic_width, &logic_height);

    switch(s_weather_win_page)
    {
    case WEATHER_WIN_CITYLIST:
        {
            if (WEATHER_WIN_DETAILS == weather_win_page)
            {
                s_target_ui_layer_offset = -logic_width;
                step = -WEATHER_SLIDE_STEP_LEN;
            }
            // 循环
            else if (WEATHER_WIN_TRENDS == weather_win_page)
            {
                s_target_ui_layer_offset = logic_width;
                step = WEATHER_SLIDE_STEP_LEN;
            }
        }
        break;

    case WEATHER_WIN_DETAILS:
        {
            if (WEATHER_WIN_CITYLIST == weather_win_page)
            {
                s_target_ui_layer_offset = 0;
                step = WEATHER_SLIDE_STEP_LEN;
            }
            else if (WEATHER_WIN_TRENDS == weather_win_page)
            {
                s_target_ui_layer_offset = -logic_width * 2;
                step = -WEATHER_SLIDE_STEP_LEN;
            }
        }
        break;

    case WEATHER_WIN_TRENDS:
        {
            if (WEATHER_WIN_DETAILS == weather_win_page)
            {
                s_target_ui_layer_offset = -logic_width;
                step = WEATHER_SLIDE_STEP_LEN;
            }
            // 循环
            else if (WEATHER_WIN_CITYLIST == weather_win_page)
            {
                s_target_ui_layer_offset = -logic_width * 3;
                step = -WEATHER_SLIDE_STEP_LEN;
            }
        }
        break;

    default:
        break;
    }

#ifndef WIN32
    CHNG_FREQ_SetArmClk(MMI_GetFreqHandler(), FREQ_INDEX_MP3);
#endif

    // 改变当前page
    if (step != 0)
    {
        UILayerAppend(weather_win_page);

        // 需要滑动
        if (is_slide_effect)
        {
            int16 offset_x = 0;
            for (offset_x = s_source_ui_layer_offset; ; offset_x += step)
            {
                int16 citylist_offset = offset_x;
                int16 details_offset = logic_width + offset_x;
                int16 trends_offset = logic_width * 2 + offset_x;

                if ((step < 0 && offset_x <= s_target_ui_layer_offset) ||
                    (step > 0 && offset_x >= s_target_ui_layer_offset))
                {
                    break;
                }
                
                // 循环
                if (offset_x < -logic_width * 2)
                {
                    citylist_offset = logic_width * 3 + offset_x;
                }
                else if (offset_x > 0)
                {
                    trends_offset = -logic_width + offset_x;
                }

                UILAYER_SetLayerPosition(&s_citylist_lcd_dev_info, citylist_offset, 0);
                UILAYER_SetLayerPosition(&s_details_lcd_dev_info, details_offset, 0);
                UILAYER_SetLayerPosition(&s_trends_lcd_dev_info, trends_offset, 0);

                MMITHEME_UpdateRect();
                SCI_SLEEP(WEATHER_SLIDE_STEP_TIME);
            }
        }

        UILayerRemoveOthers(weather_win_page);

        // 为了正常画图，还需要重置位置
        UILAYER_SetLayerPosition(&s_citylist_lcd_dev_info, 0, 0);
        UILAYER_SetLayerPosition(&s_details_lcd_dev_info, 0, 0);
        UILAYER_SetLayerPosition(&s_trends_lcd_dev_info, 0, 0);

        SetTitleBar(weather_win_page);

#ifdef WEATHER_NO_SOFTKEYBAR_STYLE
        SetPageDots(weather_win_page);
#endif

        MMITHEME_UpdateRect();
        SCI_SLEEP(WEATHER_SLIDE_STEP_TIME);

        s_weather_win_page = weather_win_page;
    }
    // 不改变PAGE，一般出现在TP滑动一半后又自然弹回情况下
    else if (weather_win_page == s_weather_win_page)
    {
        UILayerRemoveOthers(s_weather_win_page);

        UILAYER_SetLayerPosition(&s_citylist_lcd_dev_info, 0, 0);
        UILAYER_SetLayerPosition(&s_details_lcd_dev_info, 0, 0);
        UILAYER_SetLayerPosition(&s_trends_lcd_dev_info, 0, 0);

#ifdef WEATHER_NO_SOFTKEYBAR_STYLE
        SetPageDots(s_weather_win_page);
#endif

        MMITHEME_UpdateRect();
    }

    // 在循环滑动PAGE中，target offset暂时性超出范围，需要重置
    if (s_target_ui_layer_offset == logic_width)
    {
        s_target_ui_layer_offset = -2 * logic_width;
    }
    else if (s_target_ui_layer_offset == -3 * logic_width)
    {
        s_target_ui_layer_offset = 0;
    }
    s_source_ui_layer_offset = s_target_ui_layer_offset;
#if defined TOUCH_PANEL_SUPPORT
    // 不断调用它，否则form控件会接收TP的！！
    PrepareTPCondition(TRUE);
#endif
#ifndef WIN32
    CHNG_FREQ_RestoreARMClk(MMI_GetFreqHandler());
#endif
}
#if defined TOUCH_PANEL_SUPPORT
/****************************************************************************/
//  Description : 触摸滑动过程中对图层的处理
//  Global resource dependence : 
//  Author:violent.wei
//  Note: 
/****************************************************************************/
LOCAL void MovePage(
                    int16 offset
                    )
{
    int16 total_offset = s_source_ui_layer_offset + offset;

    int16 citylist_offset = total_offset;
    int16 details_offset = 0;
    int16 trends_offset = 0;
    uint16 client_w = MMITHEME_GetClientRectWidth();

    details_offset = client_w + total_offset;
    trends_offset = client_w * 2 + total_offset;
#ifndef WIN32
    CHNG_FREQ_SetArmClk(MMI_GetFreqHandler(), FREQ_INDEX_MP3);
#endif

    // 循环
    if (total_offset < -client_w * 2)
    {
        citylist_offset = client_w * 3 + total_offset;
    }
    else if (total_offset > 0)
    {
        trends_offset = -client_w + total_offset;
    }

    UILayerAppend(WEATHER_WIN_CITYLIST);
    UILayerAppend(WEATHER_WIN_DETAILS);
    UILayerAppend(WEATHER_WIN_TRENDS);

    UILAYER_SetLayerPosition(&s_citylist_lcd_dev_info, citylist_offset, 0);
    UILAYER_SetLayerPosition(&s_details_lcd_dev_info, details_offset, 0);
    UILAYER_SetLayerPosition(&s_trends_lcd_dev_info, trends_offset, 0);

    MMITHEME_UpdateRect();

#ifndef WIN32
    CHNG_FREQ_RestoreARMClk(MMI_GetFreqHandler());
#endif
}
#endif
/****************************************************************************/
//  Description : 设置标题栏文字
//  Global resource dependence : 
//  Author:violent.wei
//  Note: 
/****************************************************************************/
LOCAL void SetTitleBar(
                       WEATHER_WIN_PAGE weather_win_page
                       )
{
    MMIWEATHER_WEATHER_T*   weather_info = MMIWEATHER_GetCurCityWeatherInfo();
    MMI_WIN_ID_T win_id = MMIWEATHER_MAIN_WIN_ID;

    if(PNULL == weather_info)
    {
        //SCI_TRACE_LOW:"[WEATHER]SetTitleBar PNULL == weather_info"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWEATHER_WIN_1677_112_2_18_3_6_47_75,(uint8*)"");
        return;
    }

    switch(weather_win_page)
    {
    case WEATHER_WIN_CITYLIST:
        {
             GUIWIN_SetTitleTextId(win_id, TXT_WEATHER_CITY_LIST, TRUE);
        }
        break;

    case WEATHER_WIN_DETAILS:
        {
            GUIWIN_SetTitleText(win_id, weather_info->city_name, MMIAPICOM_Wstrlen(weather_info->city_name), TRUE);
        }
        break;

    case WEATHER_WIN_TRENDS:
        {
            MMI_STRING_T temp_trends_str = {0};
            wchar city_temp_trend_str[WEATHER_MAX_STRING_LEN + 1] = {0};
            
            // “XXX天气趋势”
            MMIAPICOM_Wstrcpy(city_temp_trend_str, weather_info->city_name);
            MMI_GetLabelTextByLang(TXT_WEATHER_TEMPERATURE_TREND, &temp_trends_str);
            MMIAPICOM_Wstrncpy(city_temp_trend_str + MMIAPICOM_Wstrlen(city_temp_trend_str), temp_trends_str.wstr_ptr, temp_trends_str.wstr_len);  
            GUIWIN_SetTitleText(win_id, city_temp_trend_str, MMIAPICOM_Wstrlen(city_temp_trend_str), TRUE);
        }
        break;

    default:
        break;
    }
}

#ifdef WEATHER_NO_SOFTKEYBAR_STYLE
/****************************************************************************/
//  Description : 设置当前页点
//  Global resource dependence : 
//  Author:violent.wei
//  Note: 
/****************************************************************************/
LOCAL void SetPageDots(
                       WEATHER_WIN_PAGE weather_win_page
                       )
{
    uint32 i = 0;
    MMI_WIN_ID_T win_id = MMIWEATHER_MAIN_WIN_ID;
    MMIWEATHER_SETTING_T* setting_ptr = MMIWEATHER_GetSettingInfo();
    const GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};

    GUI_POINT_T point = {(MMI_MAINSCREEN_WIDTH - PAGE_DOTS_BOX_WIDTH) / 2, MMI_MAINSCREEN_HEIGHT - PAGE_DOTS_BOX_HEIGHT};
    MMI_IMAGE_ID_T image_arr[] = {IMAGE_SLIDE_MAINMENU_TITLE_OFF, IMAGE_SLIDE_MAINMENU_TITLE_OFF, IMAGE_SLIDE_MAINMENU_TITLE_OFF};
    GUI_RECT_T rect = {0};

    switch(weather_win_page)
    {
    case WEATHER_WIN_CITYLIST:
        image_arr[0] = IMAGE_SLIDE_MAINMENU_TITLE_ON;
        break;

    case WEATHER_WIN_DETAILS:
        image_arr[1] = IMAGE_SLIDE_MAINMENU_TITLE_ON;
        break;

    case WEATHER_WIN_TRENDS:
        image_arr[2] = IMAGE_SLIDE_MAINMENU_TITLE_ON;
        break;

    default:
        break;
    }
    
    rect.left = point.x;
    rect.top = point.y;
    rect.right = point.x + PAGE_DOTS_BOX_WIDTH;
    rect.bottom = point.y + PAGE_DOTS_BOX_HEIGHT;

    // 刷新三个点的背景区域
    GUIRES_DisplayImg(NULL,
        &rect,
        &rect,
        win_id,
        IMAGE_WEATHER_MAIN_BG,
        &lcd_dev_info);
        
    if (setting_ptr->city_num > 0)
    {
        for (i = 0; i < ARR_SIZE(image_arr); i ++, point.x += PAGE_DOTS_BOX_WIDTH / 3)
        {
            GUIRES_DisplayImg(&point,
                PNULL,
                PNULL,
                win_id,
                image_arr[i],
                &lcd_dev_info);
        }
    }
}

#endif
#if defined TOUCH_PANEL_SUPPORT
/****************************************************************************/
//  Description : 考虑到FORM相应TP，对滑动有影响，在滑动之前先做处理
//  Global resource dependence : 
//  Author:violent.wei
//  Note: 
/****************************************************************************/
LOCAL void  PrepareTPCondition(
                               BOOLEAN is_form_disable_tp
                               )
{
    int32 i = 0;
    MMIWEATHER_SETTING_T* setting_ptr = MMIWEATHER_GetSettingInfo();

    GUIAPICTRL_SetState(MMIWEATHER_CITY_LIST_FORM_CTRL_ID, GUICTRL_STATE_DISABLE_TP, is_form_disable_tp);
    
    for (i = 0; i < setting_ptr->city_num; i ++)
    {
        GUIAPICTRL_SetState(s_city_form_ctrl_id_arr[i], GUICTRL_STATE_DISABLE_TP, is_form_disable_tp);
        GUIAPICTRL_SetState((s_city_form_ctrl_id_arr[i] + MMIWEATHER_CW_FORM_CTRL_ID_OFFSET), GUICTRL_STATE_DISABLE_TP, is_form_disable_tp);
        GUIAPICTRL_SetState((s_city_form_ctrl_id_arr[i] + MMIWEATHER_CW_CITY_LABEL_CTRL_ID_OFFSET), GUICTRL_STATE_DISABLE_TP, is_form_disable_tp);
        GUIAPICTRL_SetState((s_city_form_ctrl_id_arr[i] + MMIWEATHER_CW_WEAH_LABEL_CTRL_ID_OFFSET), GUICTRL_STATE_DISABLE_TP, is_form_disable_tp);
        GUIAPICTRL_SetState((s_city_form_ctrl_id_arr[i] + MMIWEATHER_TEMP_LABEL_CTRL_ID_OFFSET), GUICTRL_STATE_DISABLE_TP, is_form_disable_tp);
        GUIAPICTRL_SetState((s_city_form_ctrl_id_arr[i] + MMIWEATHER_WICON_ANIM_CTRL_ID_OFFSET), GUICTRL_STATE_DISABLE_TP, is_form_disable_tp);
        GUIAPICTRL_SetState((s_city_form_ctrl_id_arr[i] + MMIWEATHER_HELP_LABEL_CTRL_ID_OFFSET), GUICTRL_STATE_DISABLE_TP, is_form_disable_tp);
    }
    
    GUIAPICTRL_SetState(MMIWEATHER_CITY_LIST_ADD_CITY_FORM_CTRL_ID, GUICTRL_STATE_DISABLE_TP, is_form_disable_tp);
    GUIAPICTRL_SetState(MMIWEATHER_CITY_LIST_ADD_CITY_ANIM_CTRL_ID, GUICTRL_STATE_DISABLE_TP, is_form_disable_tp);
}
#endif
/****************************************************************************/
//  Description : 城市列表消息处理函数
//  Global resource dependence : 
//  Author:violent.wei
//  Note: 
/****************************************************************************/
LOCAL MMI_RESULT_E  HandleWeatherMainWinMsg(
                                         MMI_WIN_ID_T        win_id, 
                                         MMI_MESSAGE_ID_E    msg_id, 
                                         DPARAM              param
                                         )
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    LOCAL int16     s_tp_down_start_x = 0;
    LOCAL BOOLEAN   s_is_valid_tp_msg = FALSE;
#if defined TOUCH_PANEL_SUPPORT
    GUI_BOTH_RECT_T client_rect = {0};
#endif
    uint16              logic_width = 0;
    uint16              logic_height = 0;

    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &logic_width, &logic_height);
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            MMIWEATHER_SETTING_T* setting_ptr = MMIWEATHER_GetSettingInfo();

            UILayerCreate();

            // 默认添加城市列表PAGE
            s_weather_win_page = WEATHER_WIN_CITYLIST;
            UILayerAppend(s_weather_win_page);

            // 重置全局标志量
            s_tp_down_start_x = 0;
            s_is_valid_tp_msg = FALSE;
            s_is_touch_move_happen = FALSE;
            s_source_ui_layer_offset = 0;
            s_target_ui_layer_offset = 0;

            // 加载数据
            LoadCityListControl(win_id);
            SetFormHideOrNormal(TRUE);

            // 数据画到图层上
            MMIWEATHER_UpdateWinAllPage();

            MMIWEATHER_InitNetworkAccount();
#if defined TOUCH_PANEL_SUPPORT
            PrepareTPCondition(TRUE);
#endif
            if (0 == setting_ptr->city_num)
            {
                MMK_CreateWin((uint32*)MMIWEATHER_ADD_CITY_WIN_TAB, PNULL);
            }
            else
            {
                SwitchPage(WEATHER_WIN_DETAILS, FALSE);
            }

#ifdef MMI_PDA_SUPPORT
        GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
#endif
        }
        break;  
        
    case MSG_FULL_PAINT:
        {
#ifdef PDA_WEATHER_SUPPORT_LOW_MEMORY
	        GUI_RECT_T rect = {0};
#else
            GUI_POINT_T dis_point = {0, 0};
#endif
            const GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};

#ifdef PDA_WEATHER_SUPPORT_LOW_MEMORY
            rect.left = 0;
            rect.top = 0;
            rect.right = logic_width; 
            rect.bottom = logic_height; 
			
	   GUIRES_DisplayImg(PNULL,
                &rect,
                PNULL,
                win_id,
                IMAGE_WEATHER_MAIN_BG,
                &lcd_dev_info);
#else 
            GUIRES_DisplayImg(&dis_point,
                PNULL,
                PNULL,
                win_id,
                IMAGE_WEATHER_MAIN_BG,
                &lcd_dev_info);
#endif 
            if(WEATHER_WIN_CITYLIST == s_weather_win_page)
            {
                DrawWeatherCityList();
            }
            else if(WEATHER_WIN_DETAILS == s_weather_win_page)
            {
                DrawWeatherDetailsPage(win_id);
            }
            else if(WEATHER_WIN_TRENDS == s_weather_win_page)
            {
                DrawWeatherTrendsPage(win_id);
            }
#ifdef WEATHER_NO_SOFTKEYBAR_STYLE
            SetPageDots(s_weather_win_page);
#endif
        }
        break;
        
    case MSG_GET_FOCUS:
        {
            // 因为是动态图层，需要重画
            MMIWEATHER_UpdateWinAllPage();
            UILayerAppend(s_weather_win_page);
        }
        break;

    case MSG_NOTIFY_UPDATE:
        {
            GUI_RECT_T rect = *((GUI_RECT_T*)param);
            UILAYER_ClearRect(&s_citylist_lcd_dev_info, rect);
        }
        break;
        
    case MSG_LOSE_FOCUS:
        {
            // TP滑动一半时LOSE FOCUS，需要重置TP标志量
            s_tp_down_start_x = 0;
            s_is_touch_move_happen = FALSE;
        }
        break;

    case MSG_CLOSE_WINDOW:
        {
            UILayerDestroy();
        }
        break;
        
    case MSG_NOTIFY_FORM_SWITCH_ACTIVE:
        {
            int32 i = 0;
            MMI_CTRL_ID_T ctrl_id = MMK_GetActiveCtrlId(win_id);
            
            for (i = 0; i < MMIWEATHER_MAX_CITY_NUM; i ++)
            {
                if (ctrl_id == s_city_form_ctrl_id_arr[i] + MMIWEATHER_HELP_LABEL_CTRL_ID_OFFSET)
                {
                    // 一旦改变当前城市，就要重画所有PAGE
                    MMIWEATHER_SetCurCityIndex(i);
                    MMIWEATHER_UpdateWinAllPage();

                    // TP
                    if (s_is_switch_after_cur_city_changed)
                    {
                        SwitchPage(WEATHER_WIN_DETAILS, TRUE);
                        s_is_switch_after_cur_city_changed = FALSE;
                    }
                    break;
                }
            }
#if defined TOUCH_PANEL_SUPPORT
            // 不断调用它才行
            PrepareTPCondition(TRUE);
#endif
        }
        break;
        
    case MSG_CTL_OK:
    case MSG_APP_OK:
#ifdef MMI_PDA_SUPPORT
    case MSG_APP_MENU:
#endif
        MMK_CreateWin((uint32*)MMIWEATHER_MAINMENU_WIN_TAB, NULL);
        break;
        
    case MSG_APP_RIGHT:
#if defined TOUCH_PANEL_SUPPORT
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#endif
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        {
            MMIWEATHER_SETTING_T* setting_ptr = MMIWEATHER_GetSettingInfo();
            if(MSG_APP_WEB == msg_id || MSG_CTL_MIDSK == msg_id)
            {
                if(MMIWEATHER_CITY_LIST_ADD_CITY_ANIM_CTRL_ID == MMK_GetActiveCtrlId(win_id))
                {
                    MMK_CreateWin((uint32*)MMIWEATHER_ADD_CITY_WIN_TAB, PNULL);
                    break;
                }
            }
            if (setting_ptr->city_num > 0)
            {
                if (WEATHER_WIN_CITYLIST == s_weather_win_page)
                {
                     SwitchPage(WEATHER_WIN_DETAILS, TRUE);
                }
                else if (WEATHER_WIN_DETAILS == s_weather_win_page)
                {
                    SwitchPage(WEATHER_WIN_TRENDS, TRUE);
                }
                else if (WEATHER_WIN_TRENDS == s_weather_win_page)
                {
                    SwitchPage(WEATHER_WIN_CITYLIST, TRUE);
                }
            }
        }
        break;

    case MSG_APP_LEFT:
        {
            MMIWEATHER_SETTING_T* setting_ptr = MMIWEATHER_GetSettingInfo();
            if (setting_ptr->city_num > 0)
            {
                if (WEATHER_WIN_DETAILS == s_weather_win_page)
                {
                    SwitchPage(WEATHER_WIN_CITYLIST, TRUE);
                }
                else if (WEATHER_WIN_TRENDS == s_weather_win_page)
                {
                    SwitchPage(WEATHER_WIN_DETAILS, TRUE);
                }
                else if (WEATHER_WIN_CITYLIST == s_weather_win_page)
                {
                    SwitchPage(WEATHER_WIN_TRENDS, TRUE);
                }
            }
        }
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;
#if defined TOUCH_PANEL_SUPPORT
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_DOWN:
        client_rect = MMITHEME_GetWinClientBothRect(win_id);
        if (MMK_GET_TP_Y(param) > client_rect.v_rect.top &&
            MMK_GET_TP_Y(param) < client_rect.v_rect.bottom)
        {
            MMIWEATHER_SETTING_T* setting_ptr = MMIWEATHER_GetSettingInfo();
            if (setting_ptr->city_num > 0)
            {
                s_tp_down_start_x = MMK_GET_TP_X(param);
            }

            s_is_touch_move_happen = FALSE;
            s_is_valid_tp_msg = TRUE;
        }
        else
        {
            s_is_valid_tp_msg = FALSE;
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_MOVE:
        if (s_is_valid_tp_msg)
        {
            LOCAL uint32 s_weather_tp_move_num = 0;

            MMIWEATHER_SETTING_T* setting_ptr = MMIWEATHER_GetSettingInfo();
            if (setting_ptr->city_num > 0)
            {
                int16 offset = MMK_GET_TP_X(param) - s_tp_down_start_x;

                if (offset > 0 && offset > TP_MOVE_VALID_VALUE ||
                    offset < 0 && offset < -TP_MOVE_VALID_VALUE)
                {
                    s_is_touch_move_happen = TRUE;
                }

                if (s_is_touch_move_happen && 0 == s_weather_tp_move_num)
                {
                    // 保证offset是偶数
                    MovePage((offset / 2) * 2);
                }
            }

            s_weather_tp_move_num = (s_weather_tp_move_num + 1) % 2;
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_UP:
        if (s_is_valid_tp_msg)
        {
            MMIWEATHER_SETTING_T* setting_ptr = MMIWEATHER_GetSettingInfo();

            // 如果在添加城市一栏，及时城市个数为0，也要处理，所以单独处理
            if (WEATHER_WIN_CITYLIST == s_weather_win_page && !s_is_touch_move_happen)
            {
                GUI_RECT_T rect = {0};
                GUICTRL_GetRect(MMK_GetCtrlPtr(MMIWEATHER_CITY_LIST_ADD_CITY_FORM_CTRL_ID), &rect);

                if (MMK_GET_TP_Y(param) >= rect.top && MMK_GET_TP_Y(param) <= rect.bottom)
                {
                    MMK_CreateWin((uint32*)MMIWEATHER_ADD_CITY_WIN_TAB, NULL);
                    break;
                }
            }

            if (setting_ptr->city_num > 0)
            {
                // 没有滑动过，直接进入点击的城市
                if (WEATHER_WIN_CITYLIST == s_weather_win_page && !s_is_touch_move_happen)
                {
                    int32 i = SetFormActiveItem(MMK_GET_TP_Y(param));
                    if (i == setting_ptr->cur_city)
                    {
                        SwitchPage(WEATHER_WIN_DETAILS, TRUE);
                    }
                    else if (i < setting_ptr->city_num)
                    {
                        s_is_switch_after_cur_city_changed = TRUE;
                    }
                }
                else
                {
                    int16 offset = MMK_GET_TP_X(param) - s_tp_down_start_x;

                    s_source_ui_layer_offset += offset;

                    if (offset > WEATHER_SLIDE_STEP_LEN)
                    {
                        if (WEATHER_WIN_DETAILS == s_weather_win_page)
                        {
                            SwitchPage(WEATHER_WIN_CITYLIST, TRUE);
                        }
                        else if (WEATHER_WIN_TRENDS == s_weather_win_page)
                        {
                            SwitchPage(WEATHER_WIN_DETAILS, TRUE);
                        }
                        else if (WEATHER_WIN_CITYLIST == s_weather_win_page)
                        {
                            SwitchPage(WEATHER_WIN_TRENDS, TRUE);
                        }
                    }
                    else if (offset < -WEATHER_SLIDE_STEP_LEN)
                    {
                        if (WEATHER_WIN_CITYLIST == s_weather_win_page)
                        {
                            SwitchPage(WEATHER_WIN_DETAILS, TRUE);
                        }
                        else if (WEATHER_WIN_DETAILS == s_weather_win_page)
                        {
                            SwitchPage(WEATHER_WIN_TRENDS, TRUE);
                        }
                        else if (WEATHER_WIN_TRENDS == s_weather_win_page)
                        {
                            SwitchPage(WEATHER_WIN_CITYLIST, TRUE);
                        }
                    }
                    else
                    {
                        SwitchPage(s_weather_win_page, TRUE);
                    }
                }
            }

            s_is_touch_move_happen = FALSE;
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

#if 0
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_LONG:
        {
            if (WEATHER_WIN_CITYLIST == s_weather_win_page && !s_is_touch_move_happen)
            {
                SetFormActiveItem(MMK_GET_TP_Y(param));
                MMK_CreateWin((uint32*)MMIWEATHER_MAINMENU_WIN_TAB, NULL);
            }
        }
        break;
#endif
#endif
#endif 
    case MSG_TIMER:
        {
            if (s_update_all_page_delay_timer_id == *(uint8*)param)
            {
                MMIWEATHER_UpdateWinAllPage();
            }
        }
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}

/****************************************************************************/
//  Description : 添加城市消息处理函数
//  Global resource dependence : 
//  Author:violent.wei
//  Note: 
/****************************************************************************/
LOCAL MMI_RESULT_E  HandleAddCityWinMsg(
                                        MMI_WIN_ID_T        win_id, 
                                        MMI_MESSAGE_ID_E    msg_id, 
                                        DPARAM              param
                                        )
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    MMI_HANDLE_T    edit_ctrl_id = MMIWEATHER_ADD_CITY_EDIT_CTRL_ID;
    

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            GUI_BG_T        edit_bg = {0};
            GUI_BORDER_T    edit_border = {0};
            GUI_BOTH_RECT_T edit_both_rect = {
                WEAHTER_CITYNAME_EDITBOX_LEFT, WEAHTER_CITYNAME_EDITBOX_TOP, 
                WEAHTER_CITYNAME_EDITBOX_RIGHT, WEAHTER_CITYNAME_EDITBOX_BOTTOM, 
                WEAHTER_CITYNAME_EDITBOX_LEFT, WEAHTER_CITYNAME_EDITBOX_TOP, 
                WEAHTER_CITYNAME_EDITBOX_RIGHT_H, WEAHTER_CITYNAME_EDITBOX_BOTTOM, 
            };
            
            GUIAPICTRL_SetBothRect(edit_ctrl_id, &edit_both_rect);
            
            edit_bg.bg_type = GUI_BG_COLOR;
            edit_bg.color   = MMI_WHITE_COLOR;
            edit_bg.shape   = GUI_SHAPE_ROUNDED_RECT;
            GUIEDIT_SetBg(edit_ctrl_id, &edit_bg);
            
            edit_border.type  = GUI_BORDER_ROUNDED;
            edit_border.width = 2;
            edit_border.color = MMITHEME_GetCurThemeFontColor(MMI_THEME_SELECT);
            GUIEDIT_SetBorder(edit_ctrl_id, &edit_border);
            GUIEDIT_SetFontColor(edit_ctrl_id, MMI_BLACK_COLOR);
            
            GUIEDIT_SetStyle(edit_ctrl_id, GUIEDIT_STYLE_MULTI);//GUIEDIT_STYLE_SINGLE);NEWMS00122175
            //NEWMS00179882 [6530][64X64_240X320BAR_OP]天气预报中添加城市切换输入法后，每次进入会恢复为en输入法
            GUIEDIT_SetIm(edit_ctrl_id, GUIIM_TYPE_SMART|GUIIM_TYPE_ABC|GUIIM_TYPE_DIGITAL, GUIIM_TYPE_ABC);/*lint !e655*/     
            //set left softkey
            GUIEDIT_SetSoftkey(edit_ctrl_id, 0, 1, TXT_NULL, STXT_ADD, PNULL);
            //GUIEDIT_SetMaxLen(edit_ctrl_id, MMIWEATHER_MAX_CITY_NAME_LEN);
            MMK_SetAtvCtrl(win_id, edit_ctrl_id);
        }
        break;
        
#ifdef MMI_PDA_SUPPORT
#if defined TOUCH_PANEL_SUPPORT
    case MSG_CTL_PENOK:
    if (PNULL != param)
    {
        uint32 src_id = ((MMI_NOTIFY_T*)param)->src_id;
        
        switch (src_id)
        {    
        case MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID:
            MMK_SendMsg(win_id, MSG_CTL_OK, PNULL);
            break;

        default:
            break;
        }
    }
        break;      
#else  
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif//TOUCH_PANEL_SUPPORT //IGNORE9527
#endif
#endif 
    case MSG_CTL_OK:
    case MSG_APP_OK:    
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        {
            MMI_STRING_T city_name = {0};

            GUIEDIT_GetString(edit_ctrl_id, &city_name);
            
            if (city_name.wstr_len > 0)
            {
                if (MMIWEATHER_IsAddedCity(&city_name))
                {
                    MMIWEATHER_ProcAddCityStatus(MMIWEATHER_ADD_CITY_STATE_EXIST);
                }
                else if(!MMIWEATHER_IsCityCanBeAdd(&city_name))
                {
                    MMIWEATHER_ProcAddCityStatus(MMIWEATHER_ADD_CITY_STATE_UNSUPPORT);
                }
                else
                {
                    //下载该城市的天气数据:
                    //-1.连接成功，并且拿到天气数据，表示支持该城市，提示添加城市成功。
                    //-2.连接成功，但没拿到天气数据，表示不支持该城市，提示不支持该城市。
                    //-3.连接失败，提示连接失败。检查网络连接后再试。
                    if(MMIWEATHER_AddCity(city_name.wstr_ptr))
                    {
                        MMIWEATHER_SendMsgToApp(MSG_WEATHER_ADD_CITY_UPDATE_IND);
                        MMK_CloseWin(win_id);//关闭编辑窗口，返回上一级。
                    }
                    else
                    {
                        //提示: 输入正确的城市名.
                    }
                }
            }
            }
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin( win_id );
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}



/*****************************************************************************/
//  Description : handle open wait window message
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleUpdateWaitingWinMsg(
                                       MMI_WIN_ID_T win_id, 
                                       MMI_MESSAGE_ID_E msg_id, 
                                       DPARAM param
                                       )
{
    MMI_RESULT_E    result   = MMI_RESULT_TRUE;  
    
    switch(msg_id)
    {
        case MSG_APP_RED:
        case MSG_APP_CANCEL:
            /*用户手动取消后，相关变量reset，不弹出提示界面*/
            //if(MMIWEATHER_GetUpdatingState())
            {
                MMIWEATHER_SendMsgToApp(MSG_WEATHER_UPDATE_CANCEL_IND);
            }
            break; 

        default:
            result = MMIPUB_HandleWaitWinMsg(win_id,msg_id,param);
            break;
    }
    return result;
}

/****************************************************************************/
//  Description : 因为层的原因，城市列表form控件需要同步刷新
//  Global resource dependence : 
//  Author:violent.wei
//  Note: 
/****************************************************************************/
LOCAL void DrawWeatherCityList(void)
{
    int32 i = 0;
    GUI_LCD_DEV_INFO citylist_lcd_info = {0};
    
    if (UILAYER_IsLayerActive(&s_citylist_lcd_dev_info))
    {
        citylist_lcd_info = s_citylist_lcd_dev_info;
        UILAYER_Clear(&s_citylist_lcd_dev_info);
        UILAYER_SetLayerPosition(&s_citylist_lcd_dev_info, 0, 0);
    }
    else
    {
        citylist_lcd_info = *MMITHEME_GetDefaultLcdDev();
    }

    // 将所有FORM子控件的lcd info设置到这个图层上
    MMK_SetCtrlLcdDevInfo(MMIWEATHER_CITY_LIST_FORM_CTRL_ID, &citylist_lcd_info);
    for (i = 0; i < MMIWEATHER_MAX_CITY_NUM; i ++)
    {
        MMK_SetCtrlLcdDevInfo(s_city_form_ctrl_id_arr[i], &citylist_lcd_info);
        MMK_SetCtrlLcdDevInfo(s_city_form_ctrl_id_arr[i] + MMIWEATHER_CW_CITY_LABEL_CTRL_ID_OFFSET, &citylist_lcd_info);
        MMK_SetCtrlLcdDevInfo(s_city_form_ctrl_id_arr[i] + MMIWEATHER_CW_WEAH_LABEL_CTRL_ID_OFFSET, &citylist_lcd_info);
        MMK_SetCtrlLcdDevInfo(s_city_form_ctrl_id_arr[i] + MMIWEATHER_TEMP_LABEL_CTRL_ID_OFFSET, &citylist_lcd_info);
        MMK_SetCtrlLcdDevInfo(s_city_form_ctrl_id_arr[i] + MMIWEATHER_WICON_ANIM_CTRL_ID_OFFSET, &citylist_lcd_info);
    }
    MMK_SetCtrlLcdDevInfo(MMIWEATHER_CITY_LIST_ADD_CITY_ANIM_CTRL_ID, &citylist_lcd_info);

    MMK_SendMsg(MMIWEATHER_CITY_LIST_FORM_CTRL_ID, MSG_CTL_PAINT, 0);
    for (i = 0; i < MMIWEATHER_MAX_CITY_NUM; i ++)
    {
        MMK_SendMsg(s_city_form_ctrl_id_arr[i], MSG_CTL_PAINT, 0);
        MMK_SendMsg(s_city_form_ctrl_id_arr[i] + MMIWEATHER_CW_CITY_LABEL_CTRL_ID_OFFSET, MSG_CTL_PAINT, 0);
        MMK_SendMsg(s_city_form_ctrl_id_arr[i] + MMIWEATHER_CW_WEAH_LABEL_CTRL_ID_OFFSET, MSG_CTL_PAINT, 0);
        MMK_SendMsg(s_city_form_ctrl_id_arr[i] + MMIWEATHER_TEMP_LABEL_CTRL_ID_OFFSET, MSG_CTL_PAINT, 0);
        MMK_SendMsg(s_city_form_ctrl_id_arr[i] + MMIWEATHER_WICON_ANIM_CTRL_ID_OFFSET, MSG_CTL_PAINT, 0);
    }

    MMK_SendMsg(MMIWEATHER_CITY_LIST_ADD_CITY_ANIM_CTRL_ID, MSG_CTL_PAINT, 0);
}

/****************************************************************************/
//  Description : 天气详情消息处理函数
//  Global resource dependence : 
//  Author:violent.wei
//  Note: 
/****************************************************************************/
LOCAL void DrawWeatherDetailsPage(MMI_WIN_ID_T win_id)
{
    MMIWEATHER_WEATHER_T*   weather_info = MMIWEATHER_GetCurCityWeatherInfo();
    MMI_TEXT_ID_T condition_text_id = TXT_NULL;
    MMI_IMAGE_ID_T condition_img_id = IMAGE_NULL;
    GUI_LCD_DEV_INFO  lcd_info = {0};
    int32 i = 0;
    
    GUISTR_STYLE_T big_text_style = {MMI_DEFAULT_BIG_FONT, MMI_GRAY_WHITE_COLOR, ALIGN_LVMIDDLE, 0}; /*lint !e785*/
    GUISTR_STYLE_T small_text_left_style = {MMI_DEFAULT_SMALL_FONT, MMI_GRAY_WHITE_COLOR, ALIGN_LVMIDDLE, 0}; /*lint !e785*/
    GUISTR_STYLE_T small_text_middle_style = {MMI_DEFAULT_SMALL_FONT, MMI_GRAY_WHITE_COLOR, ALIGN_HVMIDDLE, 0}; /*lint !e785*/
    GUISTR_STYLE_T small_text_right_style = {MMI_DEFAULT_SMALL_FONT, MMI_GRAY_WHITE_COLOR, ALIGN_RVMIDDLE, 0}; /*lint !e785*/
    GUISTR_STATE_T state = GUISTR_STATE_ALIGN | GUISTR_STATE_WORDBREAK | GUISTR_STATE_SINGLE_LINE; 
    
    // 当前天气所有区域数组
    GUI_RECT_T cur_info_rect[] = {
        {WEATHER_DETAIL_CUR_DETAIL_LEFT, WEATHER_DETAIL_CUR_TEMP_TOP, WEATHER_DETAIL_CUR_DETAIL_RIGHT, WEATHER_DETAIL_CUR_WEA_TOP}, 
        {WEATHER_DETAIL_CUR_DETAIL_LEFT, WEATHER_DETAIL_CUR_WEA_TOP, WEATHER_DETAIL_CUR_DETAIL_RIGHT, WEATHER_DETAIL_CUR_TEMP_RANGE_TOP}, 
#if defined MMI_RES_ORIENT_BOTH || defined MMI_RES_ORIENT_PORTRAIT
        {WEATHER_DETAIL_CUR_DETAIL_LEFT, WEATHER_DETAIL_CUR_TEMP_RANGE_TOP, WEATHER_DETAIL_CUR_DETAIL_RIGHT, WEATHER_DETAIL_CUR_WIND_TOP}, 
        {WEATHER_DETAIL_CUR_DETAIL_LEFT, WEATHER_DETAIL_CUR_WIND_TOP, WEATHER_DETAIL_CUR_DETAIL_RIGHT, WEATHER_DETAIL_CUR_HUMI_TOP}, 
        {WEATHER_DETAIL_CUR_DETAIL_LEFT, WEATHER_DETAIL_CUR_HUMI_TOP, WEATHER_DETAIL_CUR_DETAIL_RIGHT, WEATHER_DETAIL_CUR_HUMI_BOTTOM},
#else
        {WEATHER_DETAIL_CUR_DETAIL_LEFT, WEATHER_DETAIL_CUR_TEMP_RANGE_TOP, WEATHER_DETAIL_CUR_DETAIL_RIGHT, WEATHER_DETAIL_FORE_TOP}, 
        {WEATHER_DETAIL_CUR_WIND_LEFT, WEATHER_DETAIL_CUR_WIND_TOP, WEATHER_DETAIL_CUR_WIND_RIGHT, WEATHER_DETAIL_CUR_HUMI_TOP}, 
        {WEATHER_DETAIL_CUR_WIND_LEFT, WEATHER_DETAIL_CUR_HUMI_TOP, WEATHER_DETAIL_CUR_WIND_RIGHT, WEATHER_DETAIL_CUR_HUMI_BOTTOM},
#endif
        {WEATHER_DETAIL_CUR_WEA_ICON_LEFT, WEATHER_DETAIL_CUR_WEA_ICON_TOP, WEATHER_DETAIL_CUR_WEA_ICON_RIGHT, WEATHER_DETAIL_CUR_WEA_ICON_TOP + (WEATHER_DETAIL_CUR_WEA_ICON_RIGHT - WEATHER_DETAIL_CUR_WEA_ICON_LEFT)}
    };
    
    // 未来天气所有区域数组
    GUI_RECT_T forecast_info_rect[] = {
        {WEATHER_DETAIL_FORE_DATE_LEFT, WEATHER_DETAIL_FORE_TOP, WEATHER_DETAIL_FORE_DATE_RIGHT, WEATHER_DETAIL_FORE_BOTTOM}, 
        {WEATHER_DETAIL_FORE_WEEK_LEFT, WEATHER_DETAIL_FORE_TOP, WEATHER_DETAIL_FORE_WEEK_RIGHT, WEATHER_DETAIL_FORE_BOTTOM}, 
        {WEATHER_DETAIL_FORE_ICON_LEFT, WEATHER_DETAIL_FORE_TOP, WEATHER_DETAIL_FORE_ICON_RIGHT, WEATHER_DETAIL_FORE_BOTTOM}, 
        {WEATHER_DETAIL_FORE_TEMP_RANGE_LEFT, WEATHER_DETAIL_FORE_TOP, WEATHER_DETAIL_FORE_TEMP_RANGE_RIGHT, WEATHER_DETAIL_FORE_BOTTOM}
    };
    
    // 更新于区域
    const GUI_RECT_T update_rect = {WEATHER_DETAIL_UPDATE_TIME_LEFT, WEATHER_DETAIL_UPDATE_TIME_TOP, WEATHER_DETAIL_UPDATE_TIME_RIGHT, WEATHER_DETAIL_UPDATE_TIME_BOTTOM};
    
    wchar wstr[WEATHER_MAX_STRING_LEN + 1] = {0};
    MMI_STRING_T string = {0};
    uint16              logic_width = 0;
    uint16              logic_height = 0;

    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &logic_width, &logic_height);
    if(PNULL == weather_info)
    {
        //SCI_TRACE_LOW:"[WEATHER]DrawWeatherDetailsPage PNULL == weather_info"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWEATHER_WIN_2382_112_2_18_3_6_48_76,(uint8*)"");
        return;
    }
    condition_text_id = MMIWEATHER_GetConditionTxtId(weather_info->current.condition);
    condition_img_id = MMIWEATHER_GetConditionImgId(weather_info->current.condition, WEATHER_ICON_BIG);	
    if (UILAYER_IsLayerActive(&s_details_lcd_dev_info))
    {
        lcd_info = s_details_lcd_dev_info;
        UILAYER_Clear(&s_details_lcd_dev_info);
        UILAYER_SetLayerPosition(&s_details_lcd_dev_info, 0, 0);
    }
    else
    {
        lcd_info = *MMITHEME_GetDefaultLcdDev();
    }

    //SCI_TRACE_LOW:"[weather]:weather_info->current.condition = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWEATHER_WIN_2389_112_2_18_3_6_48_77,(uint8*)"d",weather_info->current.condition);

    if (MMIWEATHER_IsCurCityWeatherInfoLegal())
    {
        // 蒙版
#ifdef PDA_WEATHER_SUPPORT_LOW_MEMORY
    	GUI_RECT_T rect = {0};
#else
        GUI_POINT_T dis_point = {0, 0};
#endif
	
#ifdef PDA_WEATHER_SUPPORT_LOW_MEMORY        
        rect.left = 0; //5;
        rect.top = 0; //55;
        rect.right = logic_width;  //235;
        rect.bottom = logic_height;  //275;

        GUIRES_DisplayImg(PNULL,
                &rect,
                PNULL,
                win_id,
                IMAGE_WEATHER_DETAILS_BG,
                &lcd_info);
#else
        GUIRES_DisplayImg(&dis_point,
            PNULL,
            PNULL,
            win_id,
            IMAGE_WEATHER_DETAILS_BG,
            &lcd_info);
#endif
    }
    
    // 显示天气大图标
    GUIRES_DisplayImg(NULL, &cur_info_rect[5], PNULL, win_id, condition_img_id, &lcd_info);
    
    // 如果更新过，显示温度
    if (MMIWEATHER_IsCurCityWeatherInfoLegal())
    {
        // 显示温度
        if(MMIWEATHER_TEMP_UNKNOW == weather_info->current.temp_f)
        {
            MMI_GetLabelTextByLang(TXT_UNKNOWN, &string);
            GUISTR_DrawTextToLCDInRect(&lcd_info, &cur_info_rect[0], &cur_info_rect[0],
                (const MMI_STRING_T*)&string, &big_text_style, state, GUISTR_TEXT_DIR_AUTO); 
        }
        else
        {
            string.wstr_ptr = wstr;
            GetTemperatureString(weather_info->current.temp_f, &string);
            GUISTR_DrawTextToLCDInRect(&lcd_info, &cur_info_rect[0], &cur_info_rect[0],
                (const MMI_STRING_T*)&string, &big_text_style, state, GUISTR_TEXT_DIR_AUTO); 
        }
        // 显示天气字符串
        MMI_GetLabelTextByLang(condition_text_id, &string);
        GUISTR_DrawTextToLCDInRect(&lcd_info, &cur_info_rect[1], &cur_info_rect[1],
            (const MMI_STRING_T*)&string, &small_text_left_style, state, GUISTR_TEXT_DIR_AUTO); 
        
        // 显示温度区间
        string.wstr_ptr = wstr;
        GetTemperatureRangeString(weather_info->forecast.low1, weather_info->forecast.high1, &string,TRUE);
        GUISTR_DrawTextToLCDInRect(&lcd_info, &cur_info_rect[2], &cur_info_rect[2],
            (const MMI_STRING_T*)&string, &small_text_left_style, state, GUISTR_TEXT_DIR_AUTO); 
        
        // 显示风向风力
        string.wstr_ptr = wstr;
        GetWindInfoString(weather_info->current.wind_dir, weather_info->current.wind_force, &string);
        GUISTR_DrawTextToLCDInRect(&lcd_info, &cur_info_rect[3], &cur_info_rect[3],
            (const MMI_STRING_T*)&string, &small_text_left_style, state, GUISTR_TEXT_DIR_AUTO); 
        
        // 显示湿度
        string.wstr_ptr = wstr;
        GetHumidityString(weather_info->current.humidity, &string);
        GUISTR_DrawTextToLCDInRect(&lcd_info, &cur_info_rect[4], &cur_info_rect[4],
            (const MMI_STRING_T*)&string, &small_text_left_style, state, GUISTR_TEXT_DIR_AUTO); 
        
        for (i = 0; i < MMIWEATHER_FORECAST_DAY_NUM; i ++)
        {
            MMIWEATHER_WEEK_E week_id = 0;
            MMIWEATHER_CONDITION_E condition_id = 0;
            MMI_TEXT_ID_T text_id = TXT_UNKNOWN;
            MMI_IMAGE_ID_T img_id = IMAGE_WEATHER_CONDITION_SMALL_UNKNOWN;
            int32 low = 0, high = 0;
            wchar* date = NULL;
            
            MMIWEATHER_GetFCId(&weather_info->forecast, i, &week_id, &condition_id, &low, &high, &date);
            
            // 日期
            string.wstr_ptr = date;
            string.wstr_len = MMIAPICOM_Wstrlen(date);
            GUISTR_DrawTextToLCDInRect(&lcd_info, &forecast_info_rect[0], &forecast_info_rect[0],
                (const MMI_STRING_T*)&string, &small_text_left_style, state, GUISTR_TEXT_DIR_AUTO);

            // 星期
            text_id = MMIWEATHER_GetWeekTxtId(week_id);
            MMI_GetLabelTextByLang(text_id, &string);
            GUISTR_DrawTextToLCDInRect(&lcd_info, &forecast_info_rect[1], &forecast_info_rect[1],
                (const MMI_STRING_T*)&string, &small_text_left_style, state, GUISTR_TEXT_DIR_AUTO); 
            
            // 天气图标
            img_id = MMIWEATHER_GetConditionImgId(condition_id, WEATHER_ICON_SMALL);
            GUIRES_DisplayImg(NULL, &forecast_info_rect[2], NULL, win_id, img_id, &lcd_info);
            
            // 温度范围
            string.wstr_ptr = wstr;
            GetTemperatureRangeString(low, high, &string,TRUE);
            GUISTR_DrawTextToLCDInRect(&lcd_info, &forecast_info_rect[3], &forecast_info_rect[3],
                (const MMI_STRING_T*)&string, &small_text_right_style, state, GUISTR_TEXT_DIR_AUTO); 
            
            // 调整到下一排
            forecast_info_rect[0].top += WEATHER_DETAIL_FORE_ITEM_HEIGHT; forecast_info_rect[0].bottom += WEATHER_DETAIL_FORE_ITEM_HEIGHT;
            forecast_info_rect[1].top += WEATHER_DETAIL_FORE_ITEM_HEIGHT; forecast_info_rect[1].bottom += WEATHER_DETAIL_FORE_ITEM_HEIGHT;
            forecast_info_rect[2].top += WEATHER_DETAIL_FORE_ITEM_HEIGHT; forecast_info_rect[2].bottom += WEATHER_DETAIL_FORE_ITEM_HEIGHT;
            forecast_info_rect[3].top += WEATHER_DETAIL_FORE_ITEM_HEIGHT; forecast_info_rect[3].bottom += WEATHER_DETAIL_FORE_ITEM_HEIGHT;
        }
        
        // 更新于字符串
        string.wstr_ptr = wstr;
        GetUpdateTimeString(weather_info->pub_date_wstr, weather_info->pub_time_wstr, &string);
        GUISTR_DrawTextToLCDInRect(&lcd_info, &update_rect, &update_rect,
            (const MMI_STRING_T*)&string, &small_text_middle_style, state, GUISTR_TEXT_DIR_AUTO); 
    }
}

/****************************************************************************/
//  Description : 温度趋势消息处理函数
//  Global resource dependence : 
//  Author:violent.wei
//  Note: 
/****************************************************************************/
LOCAL void DrawWeatherTrendsPage(MMI_WIN_ID_T win_id)
{
    MMIWEATHER_WEATHER_T*   weather_info = MMIWEATHER_GetCurCityWeatherInfo();
    wchar wstr[WEATHER_MAX_STRING_LEN + 1] ={0};
    MMI_STRING_T string = {0};    
    int32 i = 0;
    GUI_POINT_T temp_high_point[MMIWEATHER_FORECAST_DAY_NUM] = {0};
    GUI_POINT_T temp_low_point[MMIWEATHER_FORECAST_DAY_NUM] = {0};
    GUISTR_STYLE_T normal_text_middle_style = {MMI_DEFAULT_NORMAL_FONT, MMI_GRAY_WHITE_COLOR, ALIGN_HVMIDDLE, 0}; /*lint !e785*/
    GUISTR_STYLE_T small_text_middle_style = {MMI_DEFAULT_SMALL_FONT, MMI_GRAY_WHITE_COLOR, ALIGN_HVMIDDLE, 0}; /*lint !e785*/
    GUISTR_STATE_T state = GUISTR_STATE_ALIGN | GUISTR_STATE_WORDBREAK | GUISTR_STATE_SINGLE_LINE; 
    MMIWEATHER_WEATHER_T* cur_weather_info_ptr = MMIWEATHER_GetCurCityWeatherInfo();
    uint16               week_img_width = 0; 
    uint16               week_img_height = 0;
    GUI_POINT_T          week_img_point = {0};
    GUI_LCD_DEV_INFO     lcd_info = {0};
    
    // 未来天气所有区域
    GUI_RECT_T forecast_info_rect[] = {
        {WEATHER_TRENDS_ITEM_LEFT, WEATHER_TRENDS_WEEK_TOP, WEATHER_TRENDS_ITEM_RIGHT, WEATHER_TRENDS_WEEK_BOTTOM}, 
#if defined MMI_RES_ORIENT_BOTH || defined MMI_RES_ORIENT_PORTRAIT
        {WEATHER_TRENDS_ITEM_LEFT, WEATHER_TRENDS_WEA_ICON_TOP, WEATHER_TRENDS_ITEM_RIGHT, WEATHER_TRENDS_WEEK_ICON_BOTTOM}, 
#else
        {WEATHER_TRENDS_ITEM_LEFT+33, WEATHER_TRENDS_WEEK_TOP, WEATHER_TRENDS_ITEM_RIGHT+33, WEATHER_TRENDS_WEEK_BOTTOM}, 
#endif
        {WEATHER_TRENDS_ITEM_LEFT, WEATHER_TRENDS_DATE_TOP, WEATHER_TRENDS_ITEM_RIGHT, WEATHER_TRENDS_DATE_BOTTOM}
    };
    uint16              logic_width = 0;
    uint16              logic_height = 0;

    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &logic_width, &logic_height);

    if(PNULL == weather_info)
    {
        //SCI_TRACE_LOW:"[WEATHER]:DrawWeatherTrendsPage PNULL == weather_info"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWEATHER_WIN_2531_112_2_18_3_6_48_78,(uint8*)"");
        return;
    }
    if(PNULL == cur_weather_info_ptr)
    {
        //SCI_TRACE_LOW:"[WEATHER]:DrawWeatherTrendsPage PNULL == cur_weather_info_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWEATHER_WIN_2536_112_2_18_3_6_48_79,(uint8*)"");
        return;
    }
    
    UILAYER_Clear(&s_trends_lcd_dev_info);
    UILAYER_SetLayerPosition(&s_trends_lcd_dev_info, 0, 0);
    if (UILAYER_IsLayerActive(&s_trends_lcd_dev_info))
    {
        lcd_info = s_trends_lcd_dev_info;
        UILAYER_Clear(&s_trends_lcd_dev_info);
        UILAYER_SetLayerPosition(&s_trends_lcd_dev_info, 0, 0);
    }
    else
    {
        lcd_info = *MMITHEME_GetDefaultLcdDev();
    }

    
    if (MMIWEATHER_IsCurCityWeatherInfoLegal())
    {
            // 蒙版
#ifdef PDA_WEATHER_SUPPORT_LOW_MEMORY
    	GUI_RECT_T rect = {0};
#else
        GUI_POINT_T dis_point = {0, 0};
#endif
		
#ifdef PDA_WEATHER_SUPPORT_LOW_MEMORY
        rect.left = 0; //5;
        rect.top = 0; //63;
        rect.right = logic_width; //235;
        rect.bottom = logic_height; //270;

    	GUIRES_DisplayImg(PNULL,
                &rect,
                PNULL,
                win_id,
                IMAGE_WEATHER_TRENDS_BG,
                &lcd_info);
#else
        GUIRES_DisplayImg(&dis_point,
            PNULL,
            PNULL,
            win_id,
            IMAGE_WEATHER_TRENDS_BG,
            &lcd_info);
#endif
            
        // 计算所有温度的坐标
        CalculateTempPoint(&cur_weather_info_ptr->forecast, temp_high_point, temp_low_point, MMIWEATHER_FORECAST_DAY_NUM);
        
        for (i = 0; i < MMIWEATHER_FORECAST_DAY_NUM; i ++)
        {
            MMIWEATHER_WEEK_E week_id = 0;
            MMIWEATHER_CONDITION_E condition_id = 0;
            MMI_TEXT_ID_T text_id = TXT_UNKNOWN;
            MMI_IMAGE_ID_T img_id = IMAGE_WEATHER_CONDITION_MIDDLE_UNKNOWN;
            int32 low = 0, high = 0;
            wchar* date = NULL;
            
            MMIWEATHER_GetFCId(&weather_info->forecast, i, &week_id, &condition_id, &low, &high, &date);
            
            // 星期
            text_id = MMIWEATHER_GetWeekTxtId(week_id);
            MMI_GetLabelTextByLang(text_id, &string);
            GUISTR_DrawTextToLCDInRect(&lcd_info, &forecast_info_rect[0], &forecast_info_rect[0],
                (const MMI_STRING_T*)&string, &normal_text_middle_style, state, GUISTR_TEXT_DIR_AUTO); 
            
            // 天气图标
            img_id = MMIWEATHER_GetConditionImgId(condition_id, WEATHER_ICON_MIDDLE);
            GUIRES_GetImgWidthHeight(&week_img_width, &week_img_height, img_id, win_id);
            week_img_point.x = (forecast_info_rect[1].right + forecast_info_rect[1].left - week_img_width) /2;
            week_img_point.y = forecast_info_rect[1].top;            
            GUIRES_DisplayImg(&week_img_point, PNULL, PNULL, win_id, img_id, &lcd_info);
            
            // 日期
            string.wstr_ptr = date;
            string.wstr_len = MMIAPICOM_Wstrlen(date);
            GUISTR_DrawTextToLCDInRect(&lcd_info, &forecast_info_rect[2], &forecast_info_rect[2],
                (const MMI_STRING_T*)&string, &normal_text_middle_style, state, GUISTR_TEXT_DIR_AUTO); 
            
            // 画线
            temp_high_point[i].x = temp_low_point[i].x = (forecast_info_rect[0].right + forecast_info_rect[0].left) / 2;
            
            if (i > 0)
            {
                LCD_DrawThickLineEx(&lcd_info, temp_high_point[i - 1].x, temp_high_point[i - 1].y, temp_high_point[i].x, temp_high_point[i].y, MMI_RED_COLOR, 2, GUILCD_PEN_STYLE_DEFAULT);
                LCD_DrawThickLineEx(&lcd_info, temp_low_point[i - 1].x, temp_low_point[i - 1].y, temp_low_point[i].x, temp_low_point[i].y, MMI_BLUE_COLOR, 2, GUILCD_PEN_STYLE_DEFAULT);
            }
            
            // 移动到下一列
            forecast_info_rect[0].left += WEATHER_TRENDS_ITEM_WIDTH;  forecast_info_rect[0].right += WEATHER_TRENDS_ITEM_WIDTH;
            forecast_info_rect[1].left += WEATHER_TRENDS_ITEM_WIDTH;  forecast_info_rect[1].right += WEATHER_TRENDS_ITEM_WIDTH;
            forecast_info_rect[2].left += WEATHER_TRENDS_ITEM_WIDTH;  forecast_info_rect[2].right += WEATHER_TRENDS_ITEM_WIDTH;
        }  
        
        // 复位坐标，考虑到温度字符串不能被曲线所覆盖，所以再画一边温度字符串
        forecast_info_rect[0].left -= WEATHER_TRENDS_ITEM_WIDTH * MMIWEATHER_FORECAST_DAY_NUM;  forecast_info_rect[0].right -= WEATHER_TRENDS_ITEM_WIDTH * MMIWEATHER_FORECAST_DAY_NUM;
        for (i = 0; i < MMIWEATHER_FORECAST_DAY_NUM; i ++)
        {
            MMIWEATHER_WEEK_E week_id = 0;
            MMIWEATHER_CONDITION_E condition_id = 0;
            int32 low = 0, high = 0;
            wchar* date = NULL;
            
            GUI_RECT_T clip_rect = {0};
            GUI_RECT_T temp_high_rect = {0};
            GUI_RECT_T temp_low_rect = {0};
            
            temp_low_rect.left   = forecast_info_rect[0].left;
            temp_low_rect.top    = temp_low_point[i].y;
            temp_low_rect.right  = forecast_info_rect[0].right;
            temp_low_rect.bottom = temp_low_point[i].y + WEATHER_TEMP_STRING_HEIGHT;
            
            temp_high_rect.left = forecast_info_rect[0].left;
            temp_high_rect.top= temp_high_point[i].y - WEATHER_TEMP_STRING_HEIGHT;
            temp_high_rect.right= forecast_info_rect[0].right;
            temp_high_rect.bottom= temp_high_point[i].y;

            clip_rect.right = logic_width;
            clip_rect.bottom = logic_height;
            
            // 画小圈圈
            LCD_FillCircle(&lcd_info, &clip_rect, temp_high_point[i].x, temp_high_point[i].y, WEATHER_TEMP_CIRCLE_SIZE, MMI_GRAY_WHITE_COLOR);
            LCD_FillCircle(&lcd_info, &clip_rect, temp_low_point[i].x, temp_low_point[i].y, WEATHER_TEMP_CIRCLE_SIZE, MMI_GRAY_WHITE_COLOR);
            
            MMIWEATHER_GetFCId(&weather_info->forecast, i, &week_id, &condition_id, &low, &high, &date);
            
            // 显示高温
            string.wstr_ptr = wstr;
            GetTemperatureString(high, &string);
            GUISTR_DrawTextToLCDInRect(&lcd_info, &temp_high_rect, &temp_high_rect,
                (const MMI_STRING_T*)&string, &small_text_middle_style, state, GUISTR_TEXT_DIR_AUTO); 
            
            // 显示低温
            string.wstr_ptr = wstr;
            GetTemperatureString(low, &string);
            GUISTR_DrawTextToLCDInRect(&lcd_info, &temp_low_rect, &temp_low_rect,
                (const MMI_STRING_T*)&string, &small_text_middle_style, state, GUISTR_TEXT_DIR_AUTO); 
            
            // 移动到下一列
            forecast_info_rect[0].left += WEATHER_TRENDS_ITEM_WIDTH;  forecast_info_rect[0].right += WEATHER_TRENDS_ITEM_WIDTH;
        }
    }
}

/****************************************************************************/
//  Description : 自动更新关闭时，相应的选项应该灰化
//  Global resource dependence : 
//  Author:violent.wei
//  Note: 
/****************************************************************************/
LOCAL void SetAutoUpdateItemsState(BOOLEAN is_allow_auto_update)
{
    GUIFORM_CHILD_DISPLAY_E child_display = (is_allow_auto_update ? GUIFORM_CHILD_DISP_NORMAL : GUIFORM_CHILD_DISP_GRAY);

    GUIFORM_SetChildDisplay(MMIWEATHER_SETTING_FORM_CTRL_ID, MMIWEATHER_SETTING_UPDATE_INTERVAL_FORM_CTRL_ID, child_display);
    GUIFORM_SetChildDisplay(MMIWEATHER_SETTING_FORM_CTRL_ID, MMIWEATHER_SETTING_UPDATE_BEGIN_TIME_FORM_CTRL_ID, child_display);
    GUIFORM_SetChildDisplay(MMIWEATHER_SETTING_FORM_CTRL_ID, MMIWEATHER_SETTING_UPDATE_END_TIME_FORM_CTRL_ID, child_display);
    MMK_PostMsg(MMIWEATHER_SETTING_WIN_ID, MSG_FULL_PAINT, 0, 0);
}

/****************************************************************************/
//  Description : 设置界面是否允许自动更新
//  Global resource dependence : 
//  Author:violent.wei
//  Note: 
/****************************************************************************/
LOCAL void LoadAutoUpdateForm(void)
{
    MMIWEATHER_SETTING_T* setting_info = MMIWEATHER_GetSettingInfo();
    
    uint8                   i = 0;
    MMI_STRING_T            itemStr = {0};
    GUISETLIST_ITEM_INFO_T  item_info = {0};
    uint32                  text_id[] =
    {
        TXT_OPEN,
        TXT_CLOSE
    };
    
    GUIFORM_SetStyle(MMIWEATHER_SETTING_AUTO_UPDATE_FORM_CTRL_ID, GUIFORM_STYLE_UNIT);
    GUILABEL_SetTextById(MMIWEATHER_SETTING_AUTO_UPDATE_LABEL_CTRL_ID, TXT_COMMON_AUTO_UPDATE, FALSE);
    
    for(i=0; i < ARR_SIZE(text_id); i ++)
    {
        MMI_GetLabelTextByLang(text_id[i], &itemStr);
        item_info.item_state = GUISETLIST_ITEM_STATE_TEXT;
        item_info.str_info.wstr_len = itemStr.wstr_len;
        item_info.str_info.wstr_ptr = itemStr.wstr_ptr;
        GUISETLIST_AddItem(MMIWEATHER_SETTING_AUTO_UPDATE_SETLIST_CTRL_ID, &item_info);
    }
    
    GUISETLIST_SetCurIndex(MMIWEATHER_SETTING_AUTO_UPDATE_SETLIST_CTRL_ID, setting_info->is_allow_auto_update ? 0 : 1);
    GUISETLIST_SetTitleTextId(MMIWEATHER_SETTING_AUTO_UPDATE_SETLIST_CTRL_ID, TXT_COMMON_AUTO_UPDATE);
    SetAutoUpdateItemsState(setting_info->is_allow_auto_update);
}

/****************************************************************************/
//  Description : 设置界面更新时间间隔
//  Global resource dependence : 
//  Author:violent.wei
//  Note: 
/****************************************************************************/
LOCAL void LoadUpdateIntervalForm(void)
{
    MMIWEATHER_SETTING_T* setting_info = MMIWEATHER_GetSettingInfo();
    
    uint8                   i = 0;
    MMI_STRING_T            itemStr = {0};
    uint32                  text_id[] =
    {
        //TXT_COMM_1_HOUR,
        //TXT_WEATHER_2_HOURS,
        TXT_WEATHER_4_HOURS,
        TXT_WEATHER_6_HOURS,
        TXT_WEATHER_8_HOURS,
        TXT_COMM_12_HOURS,
        //TXT_COMM_24_HOURS,
    };
    MMI_STRING_T            dropdownlist[ARR_SIZE(text_id)] = {0};
	wchar                   dropdownlist_item_ptr[ARR_SIZE(text_id)][DROPDOWNLIST_STRING_MAX_NUM] = {0};
    
    GUIFORM_SetStyle(MMIWEATHER_SETTING_UPDATE_INTERVAL_FORM_CTRL_ID, GUIFORM_STYLE_UNIT);
    GUILABEL_SetTextById(MMIWEATHER_SETTING_UPDATE_INTERVAL_LABEL_CTRL_ID, TXT_WEATHER_UPDATE_INTERVAL, FALSE);
    
    for(i=0; i < ARR_SIZE(text_id); i ++)
    {
        MMI_GetLabelTextByLang(text_id[i], &itemStr);
		dropdownlist[i].wstr_ptr = dropdownlist_item_ptr[i];
		MMI_WSTRNCPY(dropdownlist[i].wstr_ptr, DROPDOWNLIST_STRING_MAX_NUM, itemStr.wstr_ptr, itemStr.wstr_len, itemStr.wstr_len);
		dropdownlist[i].wstr_len = itemStr.wstr_len;
    }
    
    GUIDROPDOWNLIST_AppendItemArray(MMIWEATHER_SETTING_UPDATE_INTERVAL_DROPDOWNLIST_CTRL_ID, dropdownlist, ARR_SIZE(dropdownlist));
    GUIDROPDOWNLIST_SetCurItemIndex(MMIWEATHER_SETTING_UPDATE_INTERVAL_DROPDOWNLIST_CTRL_ID, setting_info->update_frequency);
    
}

/****************************************************************************/
//  Description : 设置界面，自动更新开始时间
//  Global resource dependence : 
//  Author:violent.wei
//  Note: 
/****************************************************************************/
LOCAL void LoadUpdateBeginTimeForm(void)
{
    MMIWEATHER_SETTING_T* setting_info = MMIWEATHER_GetSettingInfo();
    
    uint8                   i = 0;
    MMI_STRING_T            itemStr = {0};
    uint32                  text_id[] =
    {
        TXT_WEATHER_04_CLOCK,
        TXT_WEATHER_05_CLOCK,
        TXT_WEATHER_06_CLOCK,
        TXT_WEATHER_07_CLOCK,
        TXT_WEATHER_08_CLOCK,
        TXT_WEATHER_09_CLOCK,
        TXT_WEATHER_10_CLOCK,
    };
    MMI_STRING_T            dropdownlist[ARR_SIZE(text_id)] = {0};
	wchar                   dropdownlist_item_ptr[ARR_SIZE(text_id)][DROPDOWNLIST_STRING_MAX_NUM] = {0};
    
    GUIFORM_SetStyle(MMIWEATHER_SETTING_UPDATE_BEGIN_TIME_FORM_CTRL_ID, GUIFORM_STYLE_UNIT);
    GUILABEL_SetTextById(MMIWEATHER_SETTING_UPDATE_BEGIN_TIME_LABEL_CTRL_ID, TXT_WEATHER_UPDATE_BEGIN_TIME, FALSE);
    
    for(i=0; i < ARR_SIZE(text_id); i ++)
    {
        MMI_GetLabelTextByLang(text_id[i], &itemStr);
		dropdownlist[i].wstr_ptr = dropdownlist_item_ptr[i];
        MMI_WSTRNCPY(dropdownlist[i].wstr_ptr, DROPDOWNLIST_STRING_MAX_NUM, itemStr.wstr_ptr, itemStr.wstr_len, itemStr.wstr_len);
		dropdownlist[i].wstr_len = itemStr.wstr_len;
    }
    
    GUIDROPDOWNLIST_AppendItemArray(MMIWEATHER_SETTING_UPDATE_BEGIN_TIME_DROPDOWNLIST_CTRL_ID, dropdownlist, ARR_SIZE(dropdownlist));
    GUIDROPDOWNLIST_SetCurItemIndex(MMIWEATHER_SETTING_UPDATE_BEGIN_TIME_DROPDOWNLIST_CTRL_ID, setting_info->update_range_start - MMIWEATHER_UPDATE_TIME_BEGIN_NOLIMIT);
}

/****************************************************************************/
//  Description : 设置界面，自动更新结束时间
//  Global resource dependence : 
//  Author:violent.wei
//  Note: 
/****************************************************************************/
LOCAL void LoadUpdateEndTimeForm(void)
{
    MMIWEATHER_SETTING_T* setting_info = MMIWEATHER_GetSettingInfo();
    
    uint8                   i = 0;
    MMI_STRING_T            itemStr = {0};
    uint32                  text_id[] =
    {
        TXT_WEATHER_18_CLOCK,
        TXT_WEATHER_19_CLOCK,
        TXT_WEATHER_20_CLOCK,
        TXT_WEATHER_21_CLOCK,
        TXT_WEATHER_22_CLOCK,
        TXT_WEATHER_23_CLOCK,
        TXT_WEATHER_24_CLOCK,
    };
    MMI_STRING_T            dropdownlist[ARR_SIZE(text_id)] = {0};
	wchar                   dropdownlist_item_ptr[ARR_SIZE(text_id)][DROPDOWNLIST_STRING_MAX_NUM] = {0};
    
    GUIFORM_SetStyle(MMIWEATHER_SETTING_UPDATE_END_TIME_FORM_CTRL_ID, GUIFORM_STYLE_UNIT);
    GUILABEL_SetTextById(MMIWEATHER_SETTING_UPDATE_END_TIME_LABEL_CTRL_ID, TXT_WEATHER_UPDATE_END_TIME, FALSE);
    
    for(i=0; i < ARR_SIZE(text_id); i ++)
    {
        MMI_GetLabelTextByLang(text_id[i], &itemStr);
		
        dropdownlist[i].wstr_ptr = dropdownlist_item_ptr[i];
        MMI_WSTRNCPY(dropdownlist[i].wstr_ptr, DROPDOWNLIST_STRING_MAX_NUM, itemStr.wstr_ptr, itemStr.wstr_len, itemStr.wstr_len);
		dropdownlist[i].wstr_len = itemStr.wstr_len;
    }
    
    GUIDROPDOWNLIST_AppendItemArray(MMIWEATHER_SETTING_UPDATE_END_TIME_DROPDOWNLIST_CTRL_ID, dropdownlist, ARR_SIZE(dropdownlist));
    GUIDROPDOWNLIST_SetCurItemIndex(MMIWEATHER_SETTING_UPDATE_END_TIME_DROPDOWNLIST_CTRL_ID, setting_info->update_range_end - MMIWEATHER_UPDATE_TIME_END_NOLIMIT);
}

/****************************************************************************/
//  Description : 设置温度单位
//  Global resource dependence : 
//  Author:violent.wei
//  Note: 
/****************************************************************************/
LOCAL void LoadTemperatureUnitForm(void)
{
    MMIWEATHER_SETTING_T* setting_info = MMIWEATHER_GetSettingInfo();
    
    uint8                   i = 0;
    MMI_STRING_T            itemStr = {0};
    uint32                  text_id[] =
    {
        TXT_WEATHER_CELSIUS,
        TXT_WEATHER_FAHRENHEIT,
    };
    MMI_STRING_T            dropdownlist[ARR_SIZE(text_id)] = {0};
	wchar                   dropdownlist_item_ptr[ARR_SIZE(text_id)][DROPDOWNLIST_STRING_MAX_NUM] = {0};
    
    GUIFORM_SetStyle(MMIWEATHER_SETTING_TEMPERATURE_UNIT_FORM_CTRL_ID, GUIFORM_STYLE_UNIT);
    GUILABEL_SetTextById(MMIWEATHER_SETTING_TEMPERATURE_UNIT_LABEL_CTRL_ID, TXT_WEATHER_TEMPERATURE_UINT, FALSE);
    
    for(i=0; i < ARR_SIZE(text_id); i ++)
    {
        MMI_GetLabelTextByLang(text_id[i], &itemStr);
        dropdownlist[i].wstr_ptr = dropdownlist_item_ptr[i];
        MMI_WSTRNCPY(dropdownlist[i].wstr_ptr, DROPDOWNLIST_STRING_MAX_NUM, itemStr.wstr_ptr, itemStr.wstr_len, itemStr.wstr_len);
		dropdownlist[i].wstr_len = itemStr.wstr_len;
    }
    
    GUIDROPDOWNLIST_AppendItemArray(MMIWEATHER_SETTING_TEMPERATURE_UNIT_DROPDOWNLIST_CTRL_ID, dropdownlist, ARR_SIZE(dropdownlist));
    GUIDROPDOWNLIST_SetCurItemIndex(MMIWEATHER_SETTING_TEMPERATURE_UNIT_DROPDOWNLIST_CTRL_ID, WEATHER_TEMP_UNIT_C == setting_info->temp_unit ? 0 : 1);
}

/****************************************************************************/
//  Description : 设置网络连接
//  Global resource dependence : 
//  Author:Gaily.Wang
//  Note: 
/****************************************************************************/
LOCAL void LoadNetSelectForm(void)
{
    MMIWEATHER_SETTING_T*   setting_info    = MMIWEATHER_GetSettingInfo();
    
    MMI_TEXT_ID_T label_str[] = 
    {
        TXT_AUTO,
        TXT_SIM_SEL_SIM1,
        TXT_SIM_SEL_SIM2,
#if defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)        
        TXT_SIM_SEL_SIM3,
#endif
#if defined(MMI_MULTI_SIM_SYS_QUAD)
        TXT_SIM_SEL_SIM4,
#endif
#ifdef MMI_WIFI_SUPPORT
        TXT_WIFI
#endif
    };
    
    GUIFORM_SetStyle(MMIWEATHER_SETTING_NET_SELECT_FORM_CTRL_ID, GUIFORM_STYLE_UNIT);
    GUILABEL_SetTextById(MMIWEATHER_SETTING_NET_SELECT_LABEL_CTRL_ID, TXT_NETWORK_SELECT, FALSE);
    GUILABEL_SetTextById(MMIWEATHER_SETTING_NET_SELECT_LABEL1_CTRL_ID, label_str[(int)(setting_info->network)], FALSE);
    
}

#if 0
/****************************************************************************/
//  Description : GetUpdateFreqFromIndex
//  Global resource dependence : 
//  Author:Gaily.Wang
//  Note: 
/****************************************************************************/
LOCAL uint8 GetUpdateFreqFromIndex(uint8 index)
{
    uint8 update_freq = 0;
    switch(index)		   	
    {
    case 0:
        update_freq = 1;
        break;			            
    case 1:
        update_freq = 2;
        break;			            
    case 2:
        update_freq = 4;
        break;			            
    case 3:
        update_freq = 6;
        break;			            
    case 4:
        update_freq = 8;
        break;			            
    case 5:
        update_freq = 12;
        break;			            
    case 6:
        update_freq = 24;
        break;			            
    default:
        update_freq = MMIWEATHER_DEFAULT_UPDATE_FREQ;
        break;			            
    }   

    return update_freq;
}
#endif
/*****************************************************************************/
//  Description :SetSelectNetFormParam
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void SetSelectNetFormParam(MMI_WIN_ID_T	win_id)
{
    MMIWEATHER_SETTING_T*   setting_info_ptr = MMIWEATHER_GetSettingInfo();
	MMI_STRING_T        account_list[MMICONNECTION_MAX_SETTING_NUM] = {0};
    uint32              index           = 0;
    uint32              dual_sys        = 0;
    uint16              form_space      = 0;
    uint8               account_num     = 0;

    MMI_TEXT_ID_T text_id_table[] =
    {
        TXT_NETWORK_SELECT,
        TXT_NETWORK_ACCOUNT_SIM1, 
        TXT_NETWORK_ACCOUNT_SIM2,
#if defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)        
        TXT_NETWORK_ACCOUNT_SIM3,
#endif
#if defined(MMI_MULTI_SIM_SYS_QUAD)
        TXT_NETWORK_ACCOUNT_SIM4
#endif
    };

    
    MMI_CTRL_ID_T   form_id_table[] =
    {
        MMIWEATHER_NET_SELECT_FORM_ID,
        MMIWEATHER_NET_SELECT_FORM_SIM1_ID,
        MMIWEATHER_NET_SELECT_FORM_SIM2_ID,
#if defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)        
        MMIWEATHER_NET_SELECT_FORM_SIM3_ID,
#endif
#if defined(MMI_MULTI_SIM_SYS_QUAD)
        MMIWEATHER_NET_SELECT_FORM_SIM4_ID
#endif

    };
    
    MMI_CTRL_ID_T   lable_id_table[] =
    {
        MMIWEATHER_NET_SELECT_LABEL_ID,
        MMIWEATHER_NET_SELECT_LABEL_SIM1_ID,
        MMIWEATHER_NET_SELECT_LABEL_SIM2_ID,
#if defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)        
        MMIWEATHER_NET_SELECT_LABEL_SIM3_ID,
#endif
#if defined(MMI_MULTI_SIM_SYS_QUAD)
        MMIWEATHER_NET_SELECT_LABEL_SIM4_ID
#endif
    };
    
    MMI_CTRL_ID_T   dropdownlist_id[] =
    {
        MMIWEATHER_NET_SELECT_DROPDOWNLIST_SIM1_ID,
        MMIWEATHER_NET_SELECT_DROPDOWNLIST_SIM2_ID,
#if defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)        
        MMIWEATHER_NET_SELECT_DROPDOWNLIST_SIM3_ID,
#endif
#if defined(MMI_MULTI_SIM_SYS_QUAD)
        MMIWEATHER_NET_SELECT_DROPDOWNLIST_SIM4_ID
#endif
    };   

    MMI_TEXT_ID_T label_str[] = 
    {
        TXT_AUTO,
        TXT_SIM_SEL_SIM1,
        TXT_SIM_SEL_SIM2,
#if defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)        
        TXT_SIM_SEL_SIM3,
#endif
        
#if defined(MMI_MULTI_SIM_SYS_QUAD)
        TXT_SIM_SEL_SIM4,
#endif

#ifdef MMI_WIFI_SUPPORT
        TXT_WIFI
#endif
    };


    if(PNULL == setting_info_ptr)
    {
        return;
    }

    //Set Form style and lable text
    for(index = 0; index < ARR_SIZE(form_id_table); index++)
    {
        GUIFORM_SetStyle(form_id_table[index], GUIFORM_STYLE_UNIT);
        GUILABEL_SetTextById(lable_id_table[index],text_id_table[index],FALSE);
    }
    
    //Append downlist item for 
    GUIDROPDOWNLIST_SetMaxItem(MMIWEATHER_NET_SELECT_DROPDOWNLIST_ID, WEATHER_NETWORK_MAX);
    for(index = WEATHER_NETWORK_AUTO; index < WEATHER_NETWORK_MAX; index++)
    {
        GUIDROPDOWNLIST_AppendItemById(MMIWEATHER_NET_SELECT_DROPDOWNLIST_ID, label_str[index]);
    }
    GUIDROPDOWNLIST_SetCurItemIndex(MMIWEATHER_NET_SELECT_DROPDOWNLIST_ID, setting_info_ptr->network);
    
    //Append downlist item for each sim card.
    for(dual_sys = MN_DUAL_SYS_1; dual_sys < MMI_DUAL_SYS_MAX; dual_sys++)
    {
        account_num = MMIAPICONNECTION_GetLinkSettingNum(dual_sys);

        GUIDROPDOWNLIST_SetMaxItem(dropdownlist_id[dual_sys], account_num);

        for(index = 0; index < account_num; index++) 
        {
            MMICONNECTION_LINKSETTING_DETAIL_T* link_setting_ptr = MMIAPICONNECTION_GetLinkSettingItemByIndex(dual_sys, index);
            if(PNULL != link_setting_ptr)
            {
        		account_list[index].wstr_len = MIN(DROPDOWNLIST_STRING_MAX_NUM,link_setting_ptr->name_len);
                account_list[index].wstr_ptr = link_setting_ptr->name;
                GUIDROPDOWNLIST_AppendItem(dropdownlist_id[dual_sys], &account_list[index]);
            }
        } 
        
        GUIDROPDOWNLIST_SetCurItemIndex(dropdownlist_id[dual_sys], setting_info_ptr->account_index[dual_sys]);
    }


    MMK_SetAtvCtrl(win_id, MMIWEATHER_NET_SELECT_DROPDOWNLIST_ID);

    GUIFORM_SetSpace(MMIWEATHER_NET_SETTING_FORM_ID,&form_space,&form_space);
    GUIFORM_SetMargin(MMIWEATHER_NET_SETTING_FORM_ID,0);
}


/****************************************************************************/
//  Description : 网络选择消息处理
//  Global resource dependence : 
//  Author:violent.wei
//  Note: 
/****************************************************************************/
LOCAL MMI_RESULT_E  HandleNetSelectWinMsg(
                                        MMI_WIN_ID_T        win_id, 
                                        MMI_MESSAGE_ID_E    msg_id, 
                                        DPARAM              param
                                        )
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            SetSelectNetFormParam(win_id);
        }
        break;  
        
#ifdef MMI_PDA_SUPPORT
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
#endif

    case MSG_CTL_OK:
    case MSG_APP_OK:
        {
            MMIWEATHER_SETTING_T* setting_info = MMIWEATHER_GetSettingInfo();
            uint32 index = 0;
            BOOLEAN is_changed = FALSE;

            index = GUIDROPDOWNLIST_GetCurItemIndex(MMIWEATHER_NET_SELECT_DROPDOWNLIST_ID);
            if (setting_info->network != index)
            {
                is_changed = TRUE;
                setting_info->network = index;
            }
            
            index = GUIDROPDOWNLIST_GetCurItemIndex(MMIWEATHER_NET_SELECT_DROPDOWNLIST_SIM1_ID);
            if (setting_info->account_index[MN_DUAL_SYS_1] != index)
            {
                is_changed = TRUE;
                setting_info->account_index[MN_DUAL_SYS_1] = index;
            }
#if defined MULTI_SIM_SYS_DUAL || defined MULTI_SIM_SYS_TRI || defined MULTI_SIM_SYS_QUAD
            index = GUIDROPDOWNLIST_GetCurItemIndex(MMIWEATHER_NET_SELECT_DROPDOWNLIST_SIM2_ID);
            if (setting_info->account_index[MN_DUAL_SYS_2] != index)
            {
                is_changed = TRUE;
                setting_info->account_index[MN_DUAL_SYS_2] = index;
            }
#endif
#if defined MULTI_SIM_SYS_TRI || defined MULTI_SIM_SYS_QUAD
            index = GUIDROPDOWNLIST_GetCurItemIndex(MMIWEATHER_NET_SELECT_DROPDOWNLIST_SIM3_ID);
            if (setting_info->account_index[MN_DUAL_SYS_3] != index)
            {
                is_changed = TRUE;
                setting_info->account_index[MN_DUAL_SYS_3] = index;
            }
#endif
#if defined MULTI_SIM_SYS_QUAD
            index = GUIDROPDOWNLIST_GetCurItemIndex(MMIWEATHER_NET_SELECT_DROPDOWNLIST_SIM4_ID);
            if (setting_info->account_index[MN_DUAL_SYS_4] != index)
            {
                is_changed = TRUE;
                setting_info->account_index[MN_DUAL_SYS_4] = index;
            }
#endif
            if (is_changed)
            {
                // 保存到NV
                MMIWEATHER_WriteNV(MMIWEATHER_NV_SETTING_INFO);
                MMIWEATHER_OpenPubWin(MMIWEATHER_PUBWIN_SUCCESS, TXT_SET_SUCCESS, NULL);
            }
            
            MMK_CloseWin(win_id);
            
        }
        break;

    case MSG_CTL_MIDSK:
        {
            MMI_CTRL_ID_T ctrl_id = MMK_GetActiveCtrlId(win_id);
            if(SPRD_GUI_DROPDOWNLIST_ID == IGUICTRL_GetCtrlGuid(MMK_GetCtrlPtr(ctrl_id)))
            {
                GUIDROPDOWNLIST_OpenList(MMIWEATHER_NET_SELECT_WIN_ID,ctrl_id);
            }    
        }
        break;
        
#ifndef MMI_PDA_SUPPORT
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin( win_id );
        break;
#endif
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}


/****************************************************************************/
//  Description : 设置消息处理函数
//  Global resource dependence : 
//  Author:violent.wei
//  Note: 
/****************************************************************************/
LOCAL MMI_RESULT_E  HandleWeatherSettingWinMsg(
                                        MMI_WIN_ID_T        win_id, 
                                        MMI_MESSAGE_ID_E    msg_id, 
                                        DPARAM              param
                                        )
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            // 根据设置，加载所有form的index到正确位置
            LoadAutoUpdateForm();
            LoadUpdateIntervalForm();
            LoadUpdateBeginTimeForm();
            LoadUpdateEndTimeForm();
            LoadTemperatureUnitForm();
            LoadNetSelectForm();
            MMK_SetAtvCtrl(win_id, MMIWEATHER_SETTING_FORM_CTRL_ID);
        }
        break;  
#if defined TOUCH_PANEL_SUPPORT
    case MSG_CTL_PENOK:
#endif
    case MSG_NOTIFY_MIDSK:
        {
        #ifdef MMI_PDA_SUPPORT
            uint32        src_id = ((MMI_NOTIFY_T*)param)->src_id;
            
            if (PNULL != param)
            {
                switch (src_id)
                {
                case MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID:
                    MMK_SendMsg(win_id, MSG_APP_OK, PNULL);
                    break;
                    
                case MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID:
                    MMK_SendMsg(win_id, MSG_APP_CANCEL, PNULL);
                    break;

                case MMIWEATHER_SETTING_NET_SELECT_LABEL1_CTRL_ID:
                    MMK_CreateWin((uint32*)MMIWEATHER_NET_SELECT_WIN_TAB,PNULL);
                    break;
                    
                default:
                    break;
                }

            }
        #else
            MMI_CTRL_ID_T ctrl_id = MMK_GetActiveCtrlId(win_id);
        
            if(MMIWEATHER_SETTING_NET_SELECT_LABEL1_CTRL_ID ==ctrl_id )
            {
                MMK_CreateWin((uint32*)MMIWEATHER_NET_SELECT_WIN_TAB,PNULL);
            }
            if(SPRD_GUI_DROPDOWNLIST_ID == IGUICTRL_GetCtrlGuid(MMK_GetCtrlPtr(ctrl_id)))
            {
                GUIDROPDOWNLIST_OpenList(MMIWEATHER_SETTING_WIN_ID,ctrl_id);
            }
        #endif
        }
        break;

    case MSG_GET_FOCUS:
        {
            // 根据设置，加载所有form的index到正确位置
//            LoadAutoUpdateForm();
//            LoadUpdateIntervalForm();
//            LoadUpdateBeginTimeForm();
//            LoadUpdateEndTimeForm();
//            LoadTemperatureUnitForm();
            LoadNetSelectForm();
        }
        break;  
        
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_MENU:
        {
            MMIWEATHER_SETTING_T* setting_info = MMIWEATHER_GetSettingInfo();
            uint32 index = 0;
            BOOLEAN is_changed = FALSE;
            
            // 根据form当前位置，保存设置
            index = GUISETLIST_GetCurIndex(MMIWEATHER_SETTING_AUTO_UPDATE_SETLIST_CTRL_ID);
            if (setting_info->is_allow_auto_update != ((0 == index) ? TRUE : FALSE))
            {
                is_changed = TRUE;
                setting_info->is_allow_auto_update = (0 == index) ? TRUE : FALSE;
            }
            
            index = GUIDROPDOWNLIST_GetCurItemIndex(MMIWEATHER_SETTING_UPDATE_INTERVAL_DROPDOWNLIST_CTRL_ID);
            if (setting_info->update_frequency != index)
            {
                is_changed = TRUE;
                setting_info->update_frequency = index;
            }
            
            index = GUIDROPDOWNLIST_GetCurItemIndex(MMIWEATHER_SETTING_UPDATE_BEGIN_TIME_DROPDOWNLIST_CTRL_ID);
            if (setting_info->update_range_start != MMIWEATHER_UPDATE_TIME_BEGIN_NOLIMIT + index)
            {
                is_changed = TRUE;
                setting_info->update_range_start = MMIWEATHER_UPDATE_TIME_BEGIN_NOLIMIT + index;
            }            
            
            index = GUIDROPDOWNLIST_GetCurItemIndex(MMIWEATHER_SETTING_UPDATE_END_TIME_DROPDOWNLIST_CTRL_ID);
            if (setting_info->update_range_end != MMIWEATHER_UPDATE_TIME_END_NOLIMIT + index)
            {
                is_changed = TRUE;
                setting_info->update_range_end = MMIWEATHER_UPDATE_TIME_END_NOLIMIT + index;
            }            
            
            index = GUIDROPDOWNLIST_GetCurItemIndex(MMIWEATHER_SETTING_TEMPERATURE_UNIT_DROPDOWNLIST_CTRL_ID);
            if (setting_info->temp_unit != ((0 == index) ? WEATHER_TEMP_UNIT_C : WEATHER_TEMP_UNIT_F))
            {
                is_changed = TRUE;
                setting_info->temp_unit = (0 == index) ? WEATHER_TEMP_UNIT_C : WEATHER_TEMP_UNIT_F;
            }
            
            if (is_changed)
            {
            // 保存到NV
            MMIWEATHER_WriteNV(MMIWEATHER_NV_SETTING_INFO);
            //更新定时更新时间表
            MMIWEATHER_SetUpdateTimeTable(setting_info->is_allow_auto_update);
            
            //MMIWEATHER_OpenPubWin(MMIWEATHER_PUBWIN_SUCCESS, TXT_SET_SUCCESS, NULL); NEWMS00182004
            }
            MMK_CloseWin(win_id);
        }
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin( win_id );
        break;

    case MSG_CTL_SETLIST_SWITCH:
        {
            if(MMIWEATHER_SETTING_AUTO_UPDATE_SETLIST_CTRL_ID == MMK_GetActiveCtrlId(win_id))
            {
                uint32 i = GUISETLIST_GetCurIndex(MMIWEATHER_SETTING_AUTO_UPDATE_SETLIST_CTRL_ID);
                SetAutoUpdateItemsState((0 == i ? TRUE : FALSE));
            }
        }
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}

/****************************************************************************/
//  Description : 确认是否删除城市
//  Global resource dependence : 
//  Author:violent.wei
//  Note: 
/****************************************************************************/
LOCAL MMI_RESULT_E  HandleQueryDeleteCityMsg(
                                             MMI_WIN_ID_T        win_id, 
                                             MMI_MESSAGE_ID_E    msg_id, 
                                             DPARAM              param
                                             )
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    
    switch (msg_id)
    {
    case MSG_APP_OK:
    case MSG_CTL_OK:
#if defined TOUCH_PANEL_SUPPORT
    case MSG_CTL_PENOK:
#endif
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        {
            // 选择“确定”，删除城市
            MMIWEATHER_SETTING_T* setting_info_ptr = MMIWEATHER_GetSettingInfo();
            MMIWEATHER_DelCity(setting_info_ptr->cur_city);
        }
        
        MMK_CloseWin(win_id);
        break;

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    default:
        recode = MMIPUB_HandleQueryWinMsg(win_id,msg_id,param);
        break;
    }
    
    return recode;
}

/****************************************************************************/
//  Description : 菜单消息处理函数
//  Global resource dependence : 
//  Author:violent.wei
//  Note: 
/****************************************************************************/
LOCAL MMI_RESULT_E  HandleMainMenuWinMsg(
                                         MMI_WIN_ID_T        win_id, 
                                         MMI_MESSAGE_ID_E    msg_id, 
                                         DPARAM              param
                                         )
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMIWEATHER_MAINMENU_CTRL_ID;
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            MMIWEATHER_SETTING_T* setting_info_ptr = MMIWEATHER_GetSettingInfo();
            
            MMI_MENU_ID_T       menu_id  = 0;
            MMI_MENU_GROUP_ID_T group_id = 0;
            MMI_CTRL_ID_T       active_ctrl_id  = MMK_GetActiveCtrlId(MMIWEATHER_MAIN_WIN_ID);
            
            GUIMENU_GetId(ctrl_id, &group_id, &menu_id);
            
            /*#if defined(MMIWIDGET_SUPPORT) && !defined(FLASH_SUPPORT)//cr251090
                if(MMISET_IDLE_STYLE_WIDGET == MMIAPISET_GetIdleStyle())
                {
                    GUIMENU_SetItemVisible(ctrl_id, group_id, MMIWEATHER_MENU_ID_PLUNGE_SETTING, TRUE);
                }
            #elif defined(MMI_PDA_SUPPORT)
                if(MMISET_IDLE_STYLE_GRID == MMIAPISET_GetIdleStyle())
                {
                    GUIMENU_SetItemVisible(ctrl_id, group_id, MMIWEATHER_MENU_ID_PLUNGE_SETTING, TRUE);
                }
            #else
                GUIMENU_SetItemVisible(ctrl_id, group_id, MMIWEATHER_MENU_ID_PLUNGE_SETTING, FALSE);
            #endif
            这一段改写成以下code，简洁，不使用PDA的宏，而直接使用MMI_WIDGET_WEATHER1控制:
            */

            #ifdef FLASH_SUPPORT
                GUIMENU_SetItemVisible(ctrl_id, group_id, MMIWEATHER_MENU_ID_PLUNGE_SETTING, FALSE);
            #else
            #ifdef MMI_WIDGET_WEATHER1
                if(MMISET_IDLE_STYLE_WIDGET == MMIAPISET_GetIdleStyle()
                    || MMISET_IDLE_STYLE_GRID == MMIAPISET_GetIdleStyle())
                {
                    GUIMENU_SetItemVisible(ctrl_id, group_id, MMIWEATHER_MENU_ID_PLUNGE_SETTING, TRUE);
                }
                else
                {
                    GUIMENU_SetItemVisible(ctrl_id, group_id, MMIWEATHER_MENU_ID_PLUNGE_SETTING, FALSE);
                }
            #endif
            #endif

            
            if (WEATHER_WIN_CITYLIST == s_weather_win_page)
            {
                // 城市列表，没有update和forword
                GUIMENU_SetItemVisible(ctrl_id, group_id, MMIWEATHER_MENU_ID_UPDATE_WEATHER, FALSE);
                GUIMENU_SetItemVisible(ctrl_id, group_id, MMIWEATHER_MENU_ID_FORWARD_WEATHER, FALSE);
                
                //if (0 == setting_info_ptr->city_num) BUG87608
                if(0 == setting_info_ptr->city_num 
                    || MMIWEATHER_CITY_LIST_ADD_CITY_ANIM_CTRL_ID == active_ctrl_id)
                {
                    GUIMENU_SetItemGrayed(ctrl_id, group_id, MMIWEATHER_MENU_ID_DEL_CITY, TRUE);
                }
                else
                {
                    GUIMENU_SetItemGrayed(ctrl_id, group_id, MMIWEATHER_MENU_ID_DEL_CITY, FALSE);
                }
                if (MMIWEATHER_MAX_CITY_NUM == setting_info_ptr->city_num)
                {
                    GUIMENU_SetItemGrayed(ctrl_id, group_id, MMIWEATHER_MENU_ID_ADD_CITY, TRUE);
                }
            }
            else
            {
                // 详情和趋势界面，没有添加和删除城市
                GUIMENU_SetItemVisible(ctrl_id, group_id, MMIWEATHER_MENU_ID_ADD_CITY, FALSE);
                GUIMENU_SetItemVisible(ctrl_id, group_id, MMIWEATHER_MENU_ID_DEL_CITY, FALSE);

                if (0 == setting_info_ptr->city_num)
                {
                    GUIMENU_SetItemVisible(ctrl_id, group_id, MMIWEATHER_MENU_ID_UPDATE_WEATHER, FALSE);
                    GUIMENU_SetItemVisible(ctrl_id, group_id, MMIWEATHER_MENU_ID_FORWARD_WEATHER, FALSE);
                }

                if (!MMIWEATHER_IsCurCityWeatherInfoLegal())
                {
                    GUIMENU_SetItemGrayed(ctrl_id, group_id, MMIWEATHER_MENU_ID_FORWARD_WEATHER, TRUE);
                }
            }
            
            MMK_SetAtvCtrl(win_id, ctrl_id);
        }
        break;  
        
    case MSG_CTL_OK:
    case MSG_APP_OK:
#if defined TOUCH_PANEL_SUPPORT
    case MSG_CTL_PENOK:
#endif
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        {
            MMI_MENU_ID_T       menu_id  = 0;
            MMI_MENU_GROUP_ID_T group_id = 0;
            
            GUIMENU_GetId(ctrl_id, &group_id, &menu_id);
            
            switch(menu_id)
            {
            case MMIWEATHER_MENU_ID_ADD_CITY:
                {
                    MMK_CreateWin((uint32*)MMIWEATHER_ADD_CITY_WIN_TAB, NULL);
                }
                break;
                
            case MMIWEATHER_MENU_ID_DEL_CITY:
                {
                    // 确实是否删除
                    MMIWEATHER_OpenPubWin(MMIWEATHER_PUBWIN_QUERY, TXT_WEATHER_QUERY_DELETE, HandleQueryDeleteCityMsg);
                }
                break;
                
            case MMIWEATHER_MENU_ID_UPDATE_WEATHER:
                {
                    if (MMIAPIUDISK_UdiskIsRun()) //U盘使用中
                    {
                        MMIPUB_OpenAlertWarningWin(TXT_COMMON_UDISK_USING);
                        break;
                    }
                    if(MMIWEATHER_UpdateWeatherInfo(MMIWEATHER_UPDATE_FROM_MAIN_WIN))
                    {
                        MMIWEATHER_OpenPubWin(MMIWEATHER_PUBWIN_WAITING, TXT_COMMON_UPDATING, HandleUpdateWaitingWinMsg);
                    }
                }
                break;
                
            case MMIWEATHER_MENU_ID_FORWARD_WEATHER:
                {
                    MMIWEATHER_SendMessage();
                }
                break;
                
            case MMIWEATHER_MENU_ID_SETTING:
                {
                    MMK_CreateWin((uint32*)MMIWEATHER_SETTING_WIN_TAB, NULL);
                }
                break;
                
            case MMIWEATHER_MENU_ID_SHORTCUT_SETTING:
                {
#if defined (MMI_WIDGET_WEATHER1)
                    MMIWIDGET_Weather_OpenShortcutSettingWin();
#endif                    
                }
                break;
                
            case MMIWEATHER_MENU_ID_BG_TEXTCOLOUR_SETTING:
                {
#if defined MMI_WIDGET_WEATHER1
                    MMIWIDGET_Weather_OpenBGTextColorSettingWin();
#endif                    
                }
                break;

#ifdef MMI_WEATHER_JSON_PARSE
            case MMIWEATHER_MENU_ID_ABOUT:
                {
                    MMK_CreateWin((uint32 *) MMIWEATHER_ABOUT_WINTAB, PNULL);
                }
                break;
#endif
            default:
                break;
            }
            
            MMK_CloseWin( win_id );
        }
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
    case MSG_LOSE_FOCUS:
        MMK_CloseWin( win_id );
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}

/****************************************************************************/
//  Description : 发送短消息
//  Global resource dependence : 
//  Author:xiaoming.ren
//  Note: 
/****************************************************************************/
LOCAL void MMIWEATHER_SendMessage(void)
{
    wchar                         wstr[WEATHER_MAX_STRING_LEN + 1]  = {0};
    uint16	                      msg_length                        = 0;
    uint32                        i                                 = 0;
    MMI_STRING_T                  string                            = {0};
    MMI_STRING_T                  weather_msg_string                = {0};
    MMIWEATHER_WEATHER_T         *weather_info                      = PNULL;
    MMI_TEXT_ID_T                 condition_text_id                 = TXT_NULL;
    uint16                        text_length                       = 0;

    msg_length = MAX_WEATHER_MSG_LEN * sizeof(wchar);

    weather_msg_string.wstr_ptr = SCI_ALLOCA(msg_length);

    if (PNULL != weather_msg_string.wstr_ptr)
    {
        SCI_MEMSET(weather_msg_string.wstr_ptr, 0x00, msg_length);
        
        weather_info = MMIWEATHER_GetCurCityWeatherInfo();

        if(PNULL == weather_info)
        {
            //SCI_TRACE_LOW:"[WEATHER]MMIWEATHER_SendMessage PNULL == weather_info"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWEATHER_WIN_3549_112_2_18_3_6_51_80,(uint8*)"");
            return;
        }
        MMIAPICOM_Wstrncat(weather_msg_string.wstr_ptr, weather_info->city_name, MMIAPICOM_Wstrlen(weather_info->city_name));
        MMIAPICOM_Wstrncat(weather_msg_string.wstr_ptr, (wchar*)L",", MMIAPICOM_Wstrlen((wchar*)L","));
        for (i = 0; i < MMIWEATHER_FORWARD_DAY_NUM; i++)
        {
            MMIWEATHER_WEEK_E      week_id       = WEEK_UNKNOW;
            MMIWEATHER_CONDITION_E condition_id  = WEATHER_CONDITION_UNKNOW;
            MMI_TEXT_ID_T          text_id       = NULL;
            int32                  low           = NULL;
            int32                  high          = NULL;
            wchar                 *date          = NULL;
            
            MMIWEATHER_GetFCId(&weather_info->forecast, i, &week_id, &condition_id, &low, &high, &date);

            if (WEATHER_CONDITION_UNKNOW != condition_id)
            {
                // 星期
                text_id = MMIWEATHER_GetWeekTxtId(week_id);
                MMI_GetLabelTextByLang(text_id, &string);
                //check the string length
                text_length = MMIAPICOM_Wstrlen(weather_msg_string.wstr_ptr) +
                                string.wstr_len + 1;
                if(text_length > MAX_WEATHER_MSG_LEN)
                {
                    SCI_FREE(weather_msg_string.wstr_ptr);
                    weather_msg_string.wstr_ptr = PNULL;
                    MMIPUB_OpenAlertWarningWin(TXT_WEATHER_SEND_BEYOND_LEN);
                    return;
                }

                MMIAPICOM_Wstrncat(weather_msg_string.wstr_ptr, string.wstr_ptr, string.wstr_len);
                MMIAPICOM_Wstrncat(weather_msg_string.wstr_ptr, (wchar*)L":", MMIAPICOM_Wstrlen((wchar*)L":"));
                // 获取天气字符串
                SCI_MEMSET(wstr, 0x00, sizeof(wstr));
                string.wstr_ptr = wstr;
                condition_text_id = MMIWEATHER_GetConditionTxtId(condition_id);
                MMI_GetLabelTextByLang(condition_text_id, &string);
                //check the string length
                text_length = MMIAPICOM_Wstrlen(weather_msg_string.wstr_ptr) +
                                string.wstr_len + 1;
                if(text_length > MAX_WEATHER_MSG_LEN)
                {
                    SCI_FREE(weather_msg_string.wstr_ptr);
                    weather_msg_string.wstr_ptr = PNULL;
                    MMIPUB_OpenAlertWarningWin(TXT_WEATHER_SEND_BEYOND_LEN);                    
                    return;
                }

                MMIAPICOM_Wstrncat(weather_msg_string.wstr_ptr, string.wstr_ptr, string.wstr_len);
                MMIAPICOM_Wstrncat(weather_msg_string.wstr_ptr, (wchar*)L",", MMIAPICOM_Wstrlen((wchar*)L","));
                // 获取温度区间
                SCI_MEMSET(wstr, 0x00, sizeof(wstr));
                string.wstr_ptr = wstr;
                GetTemperatureRangeString(low, high, &string,TRUE);
                //check the string length
                text_length = MMIAPICOM_Wstrlen(weather_msg_string.wstr_ptr) +
                                string.wstr_len + 1;
                if(text_length > MAX_WEATHER_MSG_LEN)
                {
                    SCI_FREE(weather_msg_string.wstr_ptr);
                    weather_msg_string.wstr_ptr = PNULL;
                    MMIPUB_OpenAlertWarningWin(TXT_WEATHER_SEND_BEYOND_LEN);                    
                    return;
                }

                MMIAPICOM_Wstrncat(weather_msg_string.wstr_ptr, string.wstr_ptr, string.wstr_len);
                MMIAPICOM_Wstrncat(weather_msg_string.wstr_ptr, (wchar*)L";", MMIAPICOM_Wstrlen((wchar*)L";"));

            }
        }

        // 获取更新时间字符串
        SCI_MEMSET(wstr, 0x00, sizeof(wstr));
        string.wstr_ptr = wstr;
        GetUpdateTimeString(weather_info->pub_date_wstr, weather_info->pub_time_wstr, &string);
        //check the string length
        text_length = MMIAPICOM_Wstrlen(weather_msg_string.wstr_ptr) + string.wstr_len;
        if(text_length > MAX_WEATHER_MSG_LEN)
        {
            SCI_FREE(weather_msg_string.wstr_ptr);
            weather_msg_string.wstr_ptr = PNULL;
            MMIPUB_OpenAlertWarningWin(TXT_WEATHER_SEND_BEYOND_LEN);            
            return;
        }
        
        MMIAPICOM_Wstrncat(weather_msg_string.wstr_ptr, string.wstr_ptr, string.wstr_len);

        weather_msg_string.wstr_len = MMIAPICOM_Wstrlen(weather_msg_string.wstr_ptr);

        MMIAPISMS_WriteNewMessage(MN_DUAL_SYS_MAX, &weather_msg_string, PNULL, 0);
        
        SCI_FREE(weather_msg_string.wstr_ptr);
        weather_msg_string.wstr_ptr = PNULL;
    }
    else
    {
        //SCI_TRACE_LOW:"Weather SendMessage no mermory"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWEATHER_WIN_3605_112_2_18_3_6_51_81,(uint8*)"");
    }
    
}

/*****************************************************************************/
// 	Description : Open Pub Win
//	Global resource dependence : 
//  Author: Gaily.Wang
//	Note:
/*****************************************************************************/
PUBLIC void MMIWEATHER_OpenPubWin(MMIWEATHER_PUBWIN_TYPE_E win_type, MMI_TEXT_ID_T text_id, PROCESSMSG_FUNC handle_func)
{
    MMI_WIN_ID_T    win_id = MMIWEATHER_PUB_WIN_ID;
    
    /*关闭等待窗口*/
    MMK_CloseWin(win_id);

    switch (win_type)
    {
    case MMIWEATHER_PUBWIN_SUCCESS:
        {
            text_id = (NULL == text_id) ? TXT_SUCCESS : text_id;
            MMIPUB_OpenAlertWinByTextId(PNULL,text_id,TXT_NULL,IMAGE_PUBWIN_SUCCESS,&win_id,PNULL,MMIPUB_SOFTKEY_ONE,handle_func);
        }
        break;
    case MMIWEATHER_PUBWIN_FAIL:
        {
            text_id = (NULL == text_id) ? TXT_ERROR : text_id;
            MMIPUB_OpenAlertWinByTextId(PNULL,text_id,TXT_NULL,IMAGE_PUBWIN_FAIL,&win_id,PNULL,MMIPUB_SOFTKEY_ONE,handle_func);
        }
        break;
    case MMIWEATHER_PUBWIN_WAITING:
        {
            MMI_STRING_T          str = {0};
            text_id = (NULL == text_id) ? TXT_COMMON_WAITING : text_id;
            MMI_GetLabelTextByLang(text_id, &str);
            if (TXT_WEATHER_CANCELING == text_id)
            {
                MMIPUB_OpenWaitWin(1, &str, PNULL, PNULL, win_id, IMAGE_NULL, ANIM_PUBWIN_WAIT, WIN_ONE_LEVEL, MMIPUB_SOFTKEY_NONE, handle_func);
            }
            else
            {
                MMIPUB_OpenWaitWin(1, &str, PNULL, PNULL, win_id, IMAGE_NULL, ANIM_PUBWIN_WAIT, WIN_ONE_LEVEL, MMIPUB_SOFTKEY_ONE, handle_func);
            }
        }
        break;
        
    case MMIWEATHER_PUBWIN_QUERY:
        {
            text_id = (NULL == text_id) ? TXT_HELP : text_id;
            MMIPUB_OpenQueryWinByTextId(text_id, IMAGE_PUBWIN_QUERY, &win_id, handle_func);
        }
        break;
    default:
        break;
    }
    
}

/*****************************************************************************/
// 	Description : 更新天气，或者删减城市，重画三个page
//	Global resource dependence : none
//  Author: Gaily.Wang
//	Note:
/*****************************************************************************/
PUBLIC void MMIWEATHER_UpdateWinAllPage()
{
    MMI_WIN_ID_T win_id = MMIWEATHER_MAIN_WIN_ID;

    if (win_id == MMK_GetFocusWinId())
    {
        if (!s_is_touch_move_happen)
        {
            MMIWEATHER_SETTING_T* setting_ptr = MMIWEATHER_GetSettingInfo();

            SetTitleBar(s_weather_win_page);

            LoadCityListData(win_id);

            if (setting_ptr->city_num > 0)
            {
                DrawWeatherDetailsPage(win_id);
                DrawWeatherTrendsPage(win_id);
            }
        }
        else
        {
            s_update_all_page_delay_timer_id = MMK_CreateWinTimer(win_id, UPDATE_ALL_PAGE_DELAY_TIME, FALSE);
        }
    }
}

/*****************************************************************************/
// 	Description : Open Pub Win
//	Global resource dependence : 
//  Author: Gaily.Wang
//	Note:
/*****************************************************************************/
PUBLIC void MMIWEATHER_ProcAddCityStatus(MMIWEATHER_ADD_CITY_STATE_E state)
{
    switch(state)
    {
        case MMIWEATHER_ADD_CITY_STATE_OK:
            {
                MMIWEATHER_SETTING_T* setting_info_ptr = MMIWEATHER_GetSettingInfo();
                MMIWEATHER_SetCurCity(setting_info_ptr->city_num);
#ifdef MMIWIDGET_WEATHER_SE
                MMIWIDGET_SetCityIndexFlag(setting_info_ptr->city_num);
#endif
                setting_info_ptr->city_num++;                
                MMIWEATHER_WriteNV(MMIWEATHER_NV_SETTING_INFO);
                MMIWEATHER_OpenPubWin(MMIWEATHER_PUBWIN_SUCCESS, TXT_WEATHER_ADDING_SUCC, NULL);
            }
            break;
        case MMIWEATHER_ADD_CITY_STATE_ADDING:
            {
                MMIWEATHER_OpenPubWin(MMIWEATHER_PUBWIN_WAITING, TXT_COMMON_UPDATING, HandleUpdateWaitingWinMsg);
            }
            break;
        case MMIWEATHER_ADD_CITY_STATE_EXIST:
            {
                //该城市已经添加过
                MMIWEATHER_OpenPubWin(MMIWEATHER_PUBWIN_FAIL, TXT_WEATHER_ADDING_FAILED_EXISTED, NULL);
            }
            break;
        case MMIWEATHER_ADD_CITY_STATE_CONN_FAILED:
            {
                //提示连接失败
                MMIWEATHER_OpenPubWin(MMIWEATHER_PUBWIN_FAIL, TXT_COMMON_LINK_FAILED, NULL);
            }
            break;
        case MMIWEATHER_ADD_CITY_STATE_UNSUPPORT:
            {
                //提示不支持该城市
                MMIWEATHER_OpenPubWin(MMIWEATHER_PUBWIN_FAIL, TXT_WEATHER_ADDING_FAILED_UNSUPPORT, NULL);
            }
            break;

        default:
            {
                
            }
            break;
    }
   
}
/*****************************************************************************/
//  Description : handle open wait window message
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleCancelWaitingWinMsg(
                                       MMI_WIN_ID_T win_id, 
                                       MMI_MESSAGE_ID_E msg_id, 
                                       DPARAM param
                                       )
{
    MMI_RESULT_E    result   = MMI_RESULT_TRUE;  
    
    switch(msg_id)
    {
        case MSG_APP_RED:
        case MSG_APP_CANCEL:

            break; 

        default:
            result = MMIPUB_HandleWaitWinMsg(win_id,msg_id,param);
            break;
    }
    return result;
}

#ifdef MMI_WEATHER_JSON_PARSE
/*****************************************************************************/
//  Description : MMIWEATHER_OpenCountryList
//  Global resource dependence : 
//  Author: juan.wu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWEATHER_OpenCountryList(void)
{
    MMK_CreatePubListWin((uint32 *)MMIWEATHER_COUNTRY_LIST_WINTAB,PNULL);
}
/*****************************************************************************/
//  Description : AppendCountryList
//  Global resource dependence : 
//  Author: juan.wu
//  Note: 
/*****************************************************************************/
LOCAL void AppendCountryList(MMI_CTRL_ID_T ctrl_id,MMIWEATHER_COUNTRY_INFO_T country_info)
{
    GUILIST_ITEM_T      item_t      =   {0};/*lint !e64*/
    GUILIST_ITEM_DATA_T item_data   =   {0};/*lint !e64*/
    uint16              i = 0;
    
    item_t.item_style    = GUIITEM_STYLE_ONE_LINE_RADIO;
    item_t.item_data_ptr = &item_data; 
    item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
    for(i = 0; i<country_info.country_num;i++)
    {
        item_data.item_content[0].item_data.text_buffer.wstr_ptr = country_info.name[i];
        item_data.item_content[0].item_data.text_buffer.wstr_len = country_info.name_len[i];
        GUILIST_AppendItem( ctrl_id, &item_t );
    }
}
/*****************************************************************************/
//  Description : FreeCountryInfoMem
//  Global resource dependence : 
//  Author: juan.wu
//  Note: 
/*****************************************************************************/
LOCAL void FreeCountryInfoMem(MMIWEATHER_COUNTRY_INFO_T *country_info_ptr)
{
    uint16 i = 0;

    SCI_TRACE_LOW("weather:FreeCountryInfoMem country_num = %d",country_info_ptr->country_num);
    for(i = 0; i < country_info_ptr->country_num; i++)
    {
        if(PNULL != country_info_ptr->name[i])
        {
            SCI_FREE(country_info_ptr->name[i]);
            country_info_ptr->name[i] = PNULL;
        }
        country_info_ptr->name_len[i] = 0;
    }
}
/*****************************************************************************/
//  Description : MMIWEATHER_FreeCountryInfoMem
//  Global resource dependence : 
//  Author: juan.wu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWEATHER_FreeCountryInfoMem(void)
{
    MMIWEATHER_COUNTRY_INFO_T *  country_info_ptr = MMIWEATHER_GetCountryInfo();

    FreeCountryInfoMem(country_info_ptr);
    
}
/*****************************************************************************/
//  Description : PinCountryAndCityName
//  Global resource dependence : 
//  Author: juan.wu
//  Note: 
/*****************************************************************************/
LOCAL void PinCountryAndCityName(wchar * country_name)
{
    wchar  city_temp[MMIWEATHER_MAX_CITY_NAME_LEN+1] = {0};
    uint16 temp_len = 0;
    uint16 total_len = 0;    
    uint8 city_num = MMIWEATHER_GetCityNum();
    
    MMIWEATHER_WEATHER_T* weather_info_ptr = MMIWEATHER_GetCityWeatherInfo(city_num);

	SCI_TRACE_LOW("PinCountryAndCityName Enter");
    total_len = MMIAPICOM_Wstrlen(weather_info_ptr->city_name) 
                    + MMIAPICOM_Wstrlen(country_name) +1;
    
    if(total_len <= MMIWEATHER_MAX_CITY_NAME_LEN)
    {
        MMIAPICOM_Wstrncpy(city_temp, 
                            weather_info_ptr->city_name, 
                            MMIAPICOM_Wstrlen(weather_info_ptr->city_name));
        SCI_MEMSET(weather_info_ptr->city_name, 0, sizeof(weather_info_ptr->city_name));
        SCI_MEMSET(weather_info_ptr->city_code, 0, sizeof(weather_info_ptr->city_code));
        temp_len = MMIAPICOM_Wstrlen(country_name);
        MMIAPICOM_Wstrncpy(weather_info_ptr->city_name,
                            country_name,
                            temp_len);

        MMIAPICOM_Wstrcat(weather_info_ptr->city_name, L"/");

        MMIAPICOM_Wstrncpy(weather_info_ptr->city_name+temp_len+1,
                            city_temp,
                            MMIAPICOM_Wstrlen(city_temp));
        MMIWEATHER_SendMsgToApp(MSG_WEATHER_ADD_CITY_UPDATE_IND);
    }
    else
    {
        MMIPUB_OpenAlertFailWin(TXT_WEATHER_CITY_NAME_LONG);
    }
}
/*****************************************************************************/
//  Description : HandleCountyListWinMsg
//  Global resource dependence : 
//  Author: juan.wu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleCountyListWinMsg(
                                       MMI_WIN_ID_T win_id, 
                                       MMI_MESSAGE_ID_E msg_id, 
                                       DPARAM param
                                       )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T  ctrl_id = MMIWEATHER_COUNTRY_LIST_CTRL_ID;
    MMIWEATHER_COUNTRY_INFO_T *  country_info_ptr = MMIWEATHER_GetCountryInfo();
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUILIST_SetMaxItem(ctrl_id, country_info_ptr->country_num, FALSE );
        AppendCountryList(ctrl_id,*country_info_ptr);
        MMK_SetAtvCtrl(win_id,ctrl_id);
        break;

    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_CTL_PENOK:
    case MSG_CTL_OK:
    case MSG_CTL_MIDSK:
        {
            uint16 index = GUILIST_GetCurItemIndex(ctrl_id);

            PinCountryAndCityName(country_info_ptr->name[index]);
            s_has_sel = TRUE;
#ifdef WIN32
            Simulator_StartUpdateTimer();
#endif
            MMK_CloseWin(win_id);
        }
        
        break;
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;

    case MSG_CLOSE_WINDOW:
        FreeCountryInfoMem(country_info_ptr);
        break;
        
    default:

        break;
    }
    return result;
}

/*****************************************************************************/
//  Description : DrawAboutWeatherInfo
//  Global resource dependence : 
//  Author: juan.wu
//  Note: 
/*****************************************************************************/
LOCAL void DrawAboutWeatherInfo(MMI_WIN_ID_T win_id)
{
    MMI_IMAGE_ID_T weather_logo = IMAGE_WEATHER_LOGO;
    GUI_LCD_DEV_INFO  dev_info  = {0,0};
    uint16         logo_width   = 0;
    uint16         logo_height  = 0;
    uint16         scr_width    = 0;
    uint16         scr_height   = 0;
    GUI_RECT_T    icon_rect     = {0};
    GUI_RECT_T     str_rect     = {0};
    GUISTR_STYLE_T  style       = {0};
    GUISTR_STATE_T  state       = 0;
    MMI_STRING_T    str         = {0};
    GUISTR_INFO_T  str_info     = {0};
    
    GUILCD_GetLogicWidthHeight(MAIN_LCD_ID, &scr_width, &scr_height);
    GUIRES_GetImgWidthHeight(&logo_width, 
                            &logo_height, 
                            weather_logo, 
                            win_id);
    icon_rect.bottom = scr_height - 1;
    icon_rect.right   = scr_width - 1;
    LCD_FillRect(&dev_info, icon_rect, MMI_WHITE_COLOR);
    
    icon_rect.top = (scr_height - logo_height)/2;
    icon_rect.left = (scr_width - logo_width)/2;
    icon_rect.bottom = icon_rect.top + logo_height;
    icon_rect.right = icon_rect.left + logo_width;

    GUIRES_DisplayImg(PNULL, 
                        &icon_rect, 
                        PNULL, 
                        win_id, 
                        weather_logo, 
                        &dev_info);

    MMI_GetLabelTextByLang(TXT_WEATHER_WEATHER_INFO_FROM, &str);
    style.font = MMIBROWSER_NORMAL_FONT;  
    style.align = ALIGN_HVMIDDLE;
    GUISTR_GetStringInfo(&style, &str, state, &str_info);

    str_rect.top = icon_rect.bottom  + 10;
    str_rect.right = scr_width - 1;
    str_rect.bottom = str_rect.top + str_info.height;    
    GUISTR_DrawTextToLCDInRect(&dev_info, 
                            &str_rect, 
                            &str_rect,
                            &str, 
                            &style,
                            state, 
                            GUISTR_TEXT_DIR_AUTO);
    
    MMI_GetLabelTextByLang(TXT_WEATHER_WEATHER_URL, &str);
    str_rect.top = str_rect.bottom + 2;
    str_rect.bottom = str_rect.top + str_info.height;
    GUISTR_DrawTextToLCDInRect(&dev_info, 
                        &str_rect, 
                        &str_rect,
                        &str, 
                        &style,
                        state, 
                        GUISTR_TEXT_DIR_AUTO);

    
}

/*****************************************************************************/
//  Description : HandleWeatherAboutWinMsg
//  Global resource dependence : 
//  Author: juan.wu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleWeatherAboutWinMsg(
                                           MMI_WIN_ID_T win_id, 
                                           MMI_MESSAGE_ID_E msg_id, 
                                           DPARAM param
                                           )
{
    MMI_RESULT_E res = MMI_RESULT_TRUE;

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        
        break;

    case MSG_FULL_PAINT:
        DrawAboutWeatherInfo(win_id);
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;

    default:
        res = MMI_RESULT_FALSE;
        break;
    }
    return res;
}

/*****************************************************************************/
//  Description : MMIWEATHER_GetSelCountryFlag
//  Global resource dependence : 
//  Author: juan.wu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIWEATHER_GetSelCountryFlag(void)
{
    return s_has_sel;
}

/*****************************************************************************/
//  Description : MMIWEATHER_SetSelCountryFlag
//  Global resource dependence : 
//  Author: juan.wu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWEATHER_SetSelCountryFlag(BOOLEAN is_sel)
{
    s_has_sel = is_sel;
}
#endif

/****************************************************************************/
//  Description : 设置子控件是否为隐藏
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 
/****************************************************************************/
LOCAL void SetFormHideOrNormal(BOOLEAN is_hide)
{
    uint8    i = 0;
    uint8    max_num = 0;
    GUIFORM_CHILD_DISPLAY_E child_display = GUIFORM_CHILD_DISP_NORMAL;
    MMIWEATHER_SETTING_T* setting_ptr = MMIWEATHER_GetSettingInfo();
    
    if(is_hide)//如果需要隐藏，则隐藏所有
    {
        child_display = GUIFORM_CHILD_DISP_HIDE;
        max_num   =   MMIWEATHER_MAX_CITY_NUM;
    }
    else
    {
        max_num = setting_ptr->city_num;
    }
    for(i = 0; i < max_num; i++)
    {
        GUIFORM_SetChildDisplay(MMIWEATHER_CITY_LIST_FORM_CTRL_ID, s_city_form_ctrl_id_arr[i], child_display);
    }
    
    //GUIFORM_SetChildDisplay(MMIWEATHER_CITY_LIST_FORM_CTRL_ID, s_city_form_ctrl_id_arr[i], child_display);
}

/****************************************************************************/
//  Description : 得到X-Y温度字符串
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 供外部使用
/****************************************************************************/
PUBLIC void MMIAPIWEATHER_GetTemperatureRangeString(int32 low, 
                                                    int32 high, 
                                                    MMI_STRING_T * string_ptr,
                                                    BOOLEAN is_need_uint_str)
{
    GetTemperatureRangeString(low, high, string_ptr,is_need_uint_str);
}
/****************************************************************************/
//  Description : 得到温度字串(eg.33oc)
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 供外部使用
/****************************************************************************/
PUBLIC void MMIAPIWEATHER_GetTemperatureString(int32 temp, 
                                                MMI_STRING_T * string_ptr)
{
    GetTemperatureString(temp, string_ptr);
}
#if defined BAR_PHONE_WIDGET || defined MMI_WIDGET_WEATHER2
PUBLIC void MMIAPIWEATHER_OpenAddCityWin(void)
{
    MMK_CreateWin((uint32*)MMIWEATHER_ADD_CITY_WIN_TAB, PNULL);
}
#endif
/**--------------------------------------------------------------------------*
**                         Compiler Flag                                    *
**--------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif


/*Edit by script, ignore 4 case. Thu Apr 26 19:01:10 2012*/ //IGNORE9527


/*Edit by script, ignore 6 case. Fri Apr 27 09:38:56 2012*/ //IGNORE9527
