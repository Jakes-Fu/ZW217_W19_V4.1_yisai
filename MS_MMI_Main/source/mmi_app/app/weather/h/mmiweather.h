/******************************************************************************
** File Name:      mmiweather.h                                               *
** Author:                                                                    *
** Date:           04/2011                                                    *
** Copyright:      2005 Spreadtrum, Incorporated. All Rights Reserved.        *
** Description:    This file is used to describe weather                      *
*******************************************************************************/

#ifndef _MMIWEATHER_H_
#define _MMIWEATHER_H_ 

/*---------------------------------------------------------------------------*/
/*                         Include Files                                     */
/*---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "mmk_type.h"
#include "mmiweather_nv.h"
#include "mmiweather_id.h"  
#include "mmiwidget.h"
#ifdef MMI_WEATHER_JSON_PARSE
#include "JSON_parser.h"
#endif
/*---------------------------------------------------------------------------*/
/*                         Compiler Flag                                     */
/*---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C"
{
#endif

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

#ifndef wchar
typedef uint16 wchar;
#endif
//华氏温度转换为摄氏温度。(扩大十倍保证精度)注意，该宏不能直接使用，需要配合MMIWEATHER_GetTemperature一起使用
#define TEMP_F2C(F) ((F - 32) * 5 * 10/ 9)  
#define TEMP_C2F(C) ((C * 9 / 5 + 32))  //摄氏温度转换为华氏温度。

#define MMIWEATHER_TEMP_UNKNOW  (-273)  //温度未知
#define MMIWEATHER_WIND_UNKNOW  (-1)     //风力风向未知

#if defined MAINLCD_SIZE_240X320
#define MMIWEATHER_MAX_CITY_NUM         3       //Max city number supported.
#elif defined MAINLCD_SIZE_240X400
#define MMIWEATHER_MAX_CITY_NUM         4       //Max city number supported.
#elif defined MAINLCD_SIZE_320X480
#define MMIWEATHER_MAX_CITY_NUM         5       //Max city number supported.
#elif defined MAINLCD_SIZE_176X220
#define MMIWEATHER_MAX_CITY_NUM         2       //Max city number supported.
#elif defined MAINLCD_SIZE_128X160
#define MMIWEATHER_MAX_CITY_NUM         2       //Max city number supported.
#endif

#define MMIWEATHER_FORECAST_DAY_NUM     4       //提供几天的天气预报信息。
#define MMIWEATHER_FORWARD_DAY_NUM      2       //转发几天的天气预报信息。该值必须小于 MMIWEATHER_FORECAST_DAY_NUM。

#define MMIWEATHER_MAX_CITY_NAME_LEN    64      //Max city name str length.
#define MMIWEATHER_MAX_CITY_CODE_LEN    MMIWEATHER_MAX_CITY_NAME_LEN       //Max city code str length.
#define MMIWEATHER_MAX_COUNTRY_CODE_LEN 3        //Max iso country code length.

#define MMIWEATHER_DEFAULT_UPDATE_FREQ  24        //默认的定时更新频率


#define MMIWEATHER_MAX_FILE_NAME_LEN    128     //Max city number supported.

#define MMIWEATHER_PUB_TIME_STR_LEN     8       //Max city number supported.
#define MMIWEATHER_PUB_DATE_STR_LEN     12      //Max city number supported.
#define MMIWEATHER_TEMP_RANGE_STR_LEN   10      //Max city number supported.

#define MMIWEATHER_UPDATE_TIME_BEGIN_NOLIMIT    4     // 4点被认为是自动更新时间没有限制，以便5点排在dropdownlist第一位
#define MMIWEATHER_UPDATE_TIME_END_NOLIMIT      18    // 18点被认为是自动更新时间没有限制，以便19点排在dropdownlist第一位

#define MAX_HUMIDITY_STR_LEN            5       //Max humidity string length.
#define MAX_DATE_STR_LEN                5       //Max humidity string length.
#define WEATHER_ERROR_WARNING_MAX_LEN  128
#define MMIWEATHER_MAX_COUNTRY_NAME_LEN   32 //MMIWEATHER_MAX_CITY_NAME_LEN/2
#define MMIWEATHER_MAX_COUNTRY_NUM     50
#if defined (WEATHER_SUPPORT_GOOGLE)
//Google Weather API Address Base
#define GOOGLE_API_REQ_WEATHER_DATA                 "http://www.google.com/ig/api?weather=" 
#define GOOGLE_API_REQ_SUPPORT_COUNTRIES            "http://www.google.com/ig/countries?output=xml&hl=zh-cn"
#define GOOGLE_API_REQ_SUPPORT_CITIES               "http://www.google.com/ig/cities?country="
#elif defined (WEATHER_SUPPORT_CHINA)
//China weather.com API Address Base
#define URL_BASE_REQ_WEATHER_DATA           "http://m.weather.com.cn/data/%s.html" 
#define URL_BASE_REQ_WEATHER_DATA           "http://m.weather.com.cn/data/" 
#endif //defined (WEATHER_SUPPORT_CHINA)


#define HTTP_HEADER_FILE_NAME  L"D:\\weather\\http_head.txt"
#define HTTP_CONTENT_FILE_NAME L"D:\\weather\\weather.txt"


//Language code
#define MMIWEATHER_LANGUAGE_CODE_ZH_CN          "zh-cn"
#define MMIWEATHER_LANGUAGE_CODE_EN             "en"

//Country code
#define MMIWEATHER_COUNTRY_CODE_DEFAULT  MMIWEATHER_COUNTRY_CODE_CHINA

#define MMIWEATHER_COUNTRY_CODE_CHINA           "cn"

#ifdef MMIWIDGET_WEATHER_ANIM
#define MMIWEATHER_MAX_ANIM_PIC_NUM   8     
#define MMIWEATHER_MAX_SUNNY_ARRAY_SIZE   4  
#define MMIWEATHER_MAX_ANIM_LAYER_NUM   3
#endif
/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/

typedef enum
{
    WEATHER_NETWORK_AUTO = 0,
    WEATHER_NETWORK_SIM1,
    WEATHER_NETWORK_SIM2,
#if defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)       
    WEATHER_NETWORK_SIM3,
#endif
#if defined(MMI_MULTI_SIM_SYS_QUAD)
    WEATHER_NETWORK_SIM4,
#endif
#ifdef MMI_WIFI_SUPPORT
    WEATHER_NETWORK_WIFI,
#endif
    WEATHER_NETWORK_MAX
} WEATHER_NETWORK_E;

typedef enum
{
    WEATHER_ICON_SMALL = 0,
    WEATHER_ICON_MIDDLE,
    WEATHER_ICON_BIG,
    WEATHER_ICON_WIDGET
} WEATHER_ICON_E;

//温度单位
typedef enum
{
    WEATHER_TEMP_UNIT_C = 0,    //摄氏温度
    WEATHER_TEMP_UNIT_F,        //华氏温度
    WEATHER_TEMP_UNIT_MAX
} WEATHER_TEMP_UNIT_E;


//Weather conditions
typedef enum 
{
    WEATHER_CONDITION_UNKNOW = 0,
    WEATHER_CONDITION_CLEAR,             //晴
    WEATHER_CONDITION_FOG,               //雾
    WEATHER_CONDITION_HAZE,              //霾
    WEATHER_CONDITION_CLOUDY,            //云
    WEATHER_CONDITION_PARTLY_CLOUDY,     //少云
    WEATHER_CONDITION_MOSTLY_CLOUDY,     //多云
    WEATHER_CONDITION_OVERCAST,          //阴
    WEATHER_CONDITION_RAIN,              //中雨	
    WEATHER_CONDITION_LIGHT_RAIN,        //小雨	
    WEATHER_CONDITION_RAIN_SHOWERS,      //阵雨
    WEATHER_CONDITION_SHOWERS,           //阵雨	       
    WEATHER_CONDITION_SCATTERED_SHOWERS, //零星阵雨	
    WEATHER_CONDITION_THUNDERSTORM,      //雷阵雨	
    WEATHER_CONDITION_CHANCE_OF_SHOWERS, //时有阵雨	
    WEATHER_CONDITION_CHANCE_OF_SNOW,    //时有雪	
    WEATHER_CONDITION_CHANCE_OF_STORM,   //时有暴雨	
    WEATHER_CONDITION_SUNNY,             //晴
    WEATHER_CONDITION_MOSTLY_SUNNY,      //晴间多云	
    WEATHER_CONDITION_PARTLY_SUNNY,      //多云间晴
    WEATHER_CONDITION_MIST,              //薄雾
    WEATHER_CONDITION_SMOKE,             //烟雾
    WEATHER_CONDITION_STORM,             //大雨
    WEATHER_CONDITION_FLURRIES,          //小雪
    WEATHER_CONDITION_CHANCE_OF_RAIN,    //时有雨	
    WEATHER_CONDITION_CHANCE_OF_TSTORM,  //时有雷暴	
    WEATHER_CONDITION_SLEET,             //雨夹雪
    WEATHER_CONDITION_SNOW,              //雪
    WEATHER_CONDITION_ICY,               //冰雹	
    WEATHER_CONDITION_DUST,              //沙尘	
    WEATHER_CONDITION_SCATTERED_THUNDERSTORMS,//零星雷阵雨	
    WEATHER_CONDITION_CHANCE_OF_FLURRIES,//Chance of Flurries
    WEATHER_CONDITION_CHANCE_OF_SLEET,//Chance of Sleet
    WEATHER_CONDITION_MAX
}MMIWEATHER_CONDITION_E;

//Forecast day
typedef enum
{
    FC_DAY_1 = 0,
    FC_DAY_2,
    FC_DAY_3,
    FC_DAY_4,
    FC_DAY_MAX
}MMIWEATHER_FC_DAY_E;

//Week day.
typedef enum
{
    WEEK_UNKNOW = 0,
    WEEK_MON,
    WEEK_TUE,
    WEEK_WED,
    WEEK_THU,
    WEEK_FRI,
    WEEK_SAT,
    WEEK_SUN,
    WEEK_MAX
}MMIWEATHER_WEEK_E;

//wind direction.
typedef enum
{
    WIND_DIR_UNKNOW = 0,
    WIND_DIR_N,         //北	0°
    WIND_DIR_NNE,       //东北偏北	22.5°
    WIND_DIR_NE,        //东北	45°
    WIND_DIR_ENE,       //东北偏东	67.5°
    WIND_DIR_E,         //东	90°
    WIND_DIR_ESE,       //东南偏东	112.5°
    WIND_DIR_SE,        //东南	135°
    WIND_DIR_SSE,       //东南偏南	157.5°
    WIND_DIR_S,         //南	180°
    WIND_DIR_SSW,       //西南偏南	202.5°
    WIND_DIR_SW,        //西南	225°
    WIND_DIR_WSW,       //西南偏西	247.5°
    WIND_DIR_W,         //西	270°
    WIND_DIR_WNW,       //西北偏西	292.5°
    WIND_DIR_NW,        //西北	315°
    WIND_DIR_NNW,       //西北偏北	337.5°
    WIND_DIR_MAX
}MMIWEATHER_WIND_DIR_E;

//wind force.
typedef enum
{
    WIND_FORCE_UNKNOW = 0,
    WIND_FORCE_0, 
    WIND_FORCE_1_2,
    WIND_FORCE_2_3,
    WIND_FORCE_3_4,
    WIND_FORCE_4_5,
    WIND_FORCE_5_6,
    WIND_FORCE_6_7,
    WIND_FORCE_7_8,
    WIND_FORCE_8_9,
    WIND_FORCE_9_10,
    WIND_FORCE_10_11,
    WIND_FORCE_11_12,
    WIND_FORCE_12_ABOVE,
    WIND_FORCE_MAX
}MMIWEATHER_WIND_FORCE_E;



//Update frequency.
typedef enum
{
    //UPDATE_FREQ_1H = 0,
    //UPDATE_FREQ_2H,
    UPDATE_FREQ_4H = 0,
    UPDATE_FREQ_6H,
    UPDATE_FREQ_8H,
    UPDATE_FREQ_12H,
    //UPDATE_FREQ_24H,
    UPDATE_FREQ_MAX
}MMIWEATHER_UPDATE_FREQ_E;

//File type
typedef enum 
{
    MMIWEATHER_UPDATE_FROM_UNKNOW = 0,
    MMIWEATHER_UPDATE_FROM_MAIN_WIN,    //天气预报主窗口更新
    MMIWEATHER_UPDATE_FROM_WIDGET,      //天气预报widget更新
    MMIWEATHER_UPDATE_FROM_AUTO,        //定时自动更新
    MMIWEATHER_UPDATE_FROM_ADD_CITY,    //添加城市
    MMIWEATHER_UPDATE_FROM_MAX
}MMIWEATHER_UPDATE_FROM_E;

//添加城市状态
typedef enum 
{
    MMIWEATHER_ADD_CITY_STATE_OK = 0,         //添加成功
    MMIWEATHER_ADD_CITY_STATE_ADDING,     //正在添加
    MMIWEATHER_ADD_CITY_STATE_CONN_FAILED,    //连接失败
    MMIWEATHER_ADD_CITY_STATE_UNSUPPORT,      //不支持
    MMIWEATHER_ADD_CITY_STATE_EXIST,          //已存在
    MMIWEATHER_ADD_CITY_STATE_FULL,           //已达最大数
    MMIWEATHER_ADD_CITY_STATE_MAX
}MMIWEATHER_ADD_CITY_STATE_E;

#ifdef MMIWIDGET_WEATHER_ANIM

//Weather anim layer
typedef enum
{
    ANIM_MOVE_NONE,
    ANIM_MOVE_R_TO_L,
    ANIM_MOVE_L_TO_R,
    ANIM_MOVE_MAX
}MMIWEATHER_ANIM_MOVE_DIRECTION_T;

//Weather anim layer
typedef struct _MMIWEATHER_WEATHER_ANIM_LAYER
{
    MMIWEATHER_ANIM_MOVE_DIRECTION_T move_direction;            //是否需要移动
    uint16 pic_cnt;                                             //图片数量
    uint16 cur_pic_idx;                                         //当前图片序号
    GUI_POINT_T cur_layer_point;                                //图层位置
    GUI_LCD_DEV_INFO dev_info;                                  //动态特效图层
    MMI_IMAGE_ID_T pic_info[MMIWEATHER_MAX_ANIM_PIC_NUM];//图片信息
}MMIWEATHE_ANIM_LAYER_T;

//Weather anim information
typedef struct _MMIWEATHER_WEATHER_ANIM
{
    BOOLEAN is_forbid;          //是否屏蔽动态特效
    BOOLEAN is_need_anim;       //是否显示动态特效
    uint8 anim_timer_id;        //动态特效timer_id
    uint16 layer_cnt;           //当前图层数量
    MMIWEATHE_ANIM_LAYER_T layer_info[MMIWEATHER_MAX_ANIM_LAYER_NUM];
}MMIWEATHER_ANIM_T;
#endif

//System info
typedef struct _DATE_TIME
{
    int  nYear;
    int  nMonth;
    int  nDay;
    int  nHour;
    int  nMinut;
    int  nSecond;
}DATE_TIME_T;

//System info
typedef struct _MMIWEATHER_SYS_INFO
{
    DATE_TIME_T         date_time;      //Current date time, local data.
    MMIWEATHER_WEEK_E   week;      
} MMIWEATHER_SYS_INFO_T;

//User Settings info
typedef struct _MMIWEATHER_SETTING
{
    uint8                       city_num;           //Added cities number.
    uint8                       cur_city;           //Current city index.
    uint8                       update_range_start; //Update range start.
    uint8                       update_range_end;   //Update range end. 
    BOOLEAN                     is_allow_auto_update;
    MMIWEATHER_UPDATE_FREQ_E    update_frequency;   //Update frequency.
    WEATHER_TEMP_UNIT_E         temp_unit;
    WEATHER_NETWORK_E           network;
    uint8                       account_index[MN_DUAL_SYS_MAX];    
} MMIWEATHER_SETTING_T;

//Current weather condition information.
typedef struct _MMIWEATHER_CUR_CONDITIONS
{
    MMIWEATHER_CONDITION_E  condition;                        //4//当前天气状况
    int32                   temp_c;                           //4//当前温度(℃)
    int32                   temp_f;                           //4//当前温度(℉)
    wchar                   humidity[MAX_HUMIDITY_STR_LEN + 1];   //4//当前湿度(%)
    MMIWEATHER_WIND_DIR_E   wind_dir;                         //4//风向
    int32                   wind_force;                       //4//风力(MPH)
    
} MMIWEATHER_CUR_CONDITIONS_T;

//Forecast Condition information.
typedef struct _MMIWEATHER_FC_CONDITIONS
{
    MMIWEATHER_WEEK_E           week1;         //星期
    wchar                       date1[MAX_DATE_STR_LEN];          //日期
    MMIWEATHER_CONDITION_E      weather1;      //天气状况
    int32                       low1;          //最低温度(摄氏度)
    int32                       high1;         //最高温度(摄氏度)
    MMIWEATHER_WEEK_E           week2;         //星期
    wchar                       date2[MAX_DATE_STR_LEN];          //日期
    MMIWEATHER_CONDITION_E      weather2;      //天气状况
    int32                       low2;          //最低温度(摄氏度)
    int32                       high2;         //最高温度(摄氏度)
    MMIWEATHER_WEEK_E           week3;         //星期
    wchar                       date3[MAX_DATE_STR_LEN];          //日期
    MMIWEATHER_CONDITION_E      weather3;      //天气状况
    int32                       low3;          //最低温度(摄氏度)
    int32                       high3;         //最高温度(摄氏度)
    MMIWEATHER_WEEK_E           week4;         //星期
    wchar                       date4[MAX_DATE_STR_LEN];          //日期
    MMIWEATHER_CONDITION_E      weather4;      //天气状况
    int32                       low4;          //最低温度(摄氏度)
    int32                       high4;         //最高温度(摄氏度)
} MMIWEATHER_FC_CONDITIONS_T;
    
typedef struct _MMIWEATHER_ERROR_INFO
{
    wchar                           error_description[WEATHER_ERROR_WARNING_MAX_LEN];
    uint16                          error_des_len;
}MMIWEATHER_ERROR_INFO_T;

typedef struct _MMIWEATHER_COUNTRY_INFO
{
    wchar                       *name[MMIWEATHER_MAX_COUNTRY_NUM];
    uint16                      name_len[MMIWEATHER_MAX_COUNTRY_NUM];
    uint8                       country_num;
    BOOLEAN                     is_need_country_list;
    BOOLEAN                     is_out_of_bounds;
}MMIWEATHER_COUNTRY_INFO_T;
//Forecast information
typedef struct _MMIWEATHER_WEATHER
{
    wchar							city_name[MMIWEATHER_MAX_CITY_NAME_LEN + 1];          //City name, used to display.
    wchar							city_code[MMIWEATHER_MAX_CITY_CODE_LEN + 1];          //City code, used by weather api.
    DATE_TIME_T						pub_time;           //Published date time
    wchar                           pub_date_wstr[MMIWEATHER_PUB_DATE_STR_LEN + 1];
    wchar                           pub_time_wstr[MMIWEATHER_PUB_TIME_STR_LEN + 1];
    wchar                           temp_range_wstr[MMIWEATHER_TEMP_RANGE_STR_LEN + 1];
    MMIWEATHER_CUR_CONDITIONS_T		current;        //Current weather conditions
    MMIWEATHER_FC_CONDITIONS_T		forecast;       //Forecast weather conditions
}MMIWEATHER_WEATHER_T;

//weather app data
typedef struct _MMIWEATHER_DATA
{
    MMIWEATHER_SYS_INFO_T       system_info;
    MMIWEATHER_SETTING_T        setting_info;
    MMIWEATHER_WEATHER_T        weather_info[MMIWEATHER_MAX_CITY_NUM]; 
#ifdef MMIWIDGET_WEATHER_ANIM
    MMIWEATHER_ANIM_T           anim_info;
#endif
#ifdef MMI_WEATHER_JSON_PARSE
    MMIWEATHER_ERROR_INFO_T     error_descript;
    MMIWEATHER_COUNTRY_INFO_T   country_info;
#endif

} MMIWEATHER_DATA_T;
        
typedef struct _MMIWEATHER_CITY_TABLE_
{
    const wchar*   city_name;  //用户输入的,界面显示的城市名
    const char*    city_code;   //向服务器发送请求时使用的城市名。
}MMIWEATHER_CITY_TAB_T;

/**--------------------------------------------------------------------------*
			                EXTERNAL DECLARE
 **--------------------------------------------------------------------------*/

extern MMIWEATHER_DATA_T g_weather_data;

#if defined MMI_WIDGET_WEATHER1
extern MMIWIDGET_ITEM_INFO_T g_weather_widget; 
#endif
#if defined MMI_WIDGET_WEATHER2
extern const MMIWIDGET_ITEM_INFO_T g_weather_widget2;
#endif
/*---------------------------------------------------------------------------*/
/*                         FUNCTION DEFINITION                               */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : Init weather app
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWEATHER_InitApp(void);


/*****************************************************************************/
//  Description : Reset weather app
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWEATHER_ResetApp(void);


/*****************************************************************************/
//  Description : Write settings info to NV 
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWEATHER_WriteNV(MMIWEATHER_NV_ITEM_E nv_item);


/*****************************************************************************/
//  Description : Read settings info from NV 
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWEATHER_ReadNV(MMIWEATHER_NV_ITEM_E nv_item);

/*****************************************************************************/
//  Description : Pdp Active, 
//  Global resource dependence : none
//  Author: Gaily.Wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIWEATHER_PdpActive(void);

/*****************************************************************************/
//  Description : Deactive Pdp
//  Global resource dependence : none
//  Author: Gaily.Wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIWEATHER_PdpDeactive(void);

/*****************************************************************************/
//  Description : Get weather info by city index.
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note: 
/*****************************************************************************/
PUBLIC MMIWEATHER_WEATHER_T* MMIWEATHER_GetCityWeatherInfo(uint8 index);


/*****************************************************************************/
//  Description : Get current weather info.
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note: 
/*****************************************************************************/
PUBLIC MMIWEATHER_WEATHER_T* MMIWEATHER_GetCurCityWeatherInfo(void);

/*****************************************************************************/
//  Description : Get Setting info
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note: 
/*****************************************************************************/
PUBLIC MMIWEATHER_SETTING_T* MMIWEATHER_GetSettingInfo(void);

#ifdef MMIWIDGET_WEATHER_ANIM
/*****************************************************************************/
//  Description : Get anim info
//  Global resource dependence : 
//  Author: Jin.Wang
//  Note: 
/*****************************************************************************/
PUBLIC MMIWEATHER_ANIM_T* MMIWEATHER_GetAnimInfo(void);
#endif

/*****************************************************************************/
//  Description : Check city whether updated yet by index.
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIWEATHER_IsCityWeatherInfoLegal(uint8 index);

/*****************************************************************************/
//  Description : Check current city whether updated yet.
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIWEATHER_IsCurCityWeatherInfoLegal(void);


/*****************************************************************************/   
//  Description : Get city code str prt by city name.
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note: 
/*****************************************************************************/
PUBLIC char* MMIWEATHER_GetCityCodeByName(wchar *city_name);


/*****************************************************************************/
//  Description : MMIWEATHER_IsTimeToUpdate
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIWEATHER_IsTimeToUpdate(void);

/*****************************************************************************/
//  Description : MMIWEATHER_SetUpdateTimeTable
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIWEATHER_SetUpdateTimeTable(BOOLEAN auto_update);


/*****************************************************************************/
//  Description : Send Msg To Weather App
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWEATHER_SendMsgToApp(uint32 sig_id);

/*****************************************************************************/
//  Description : Add city
//  Global resource dependence : 
//  Author: violent.wei
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIWEATHER_AddCity(wchar* city_name_wstr);

/*****************************************************************************/
//  Description : Del city
//  Global resource dependence : 
//  Author: violent.wei
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIWEATHER_DelCity(uint8 index);
/*****************************************************************************/
//  Description : Get current temp ramge of current city.
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note: 
/*****************************************************************************/
PUBLIC wchar* MMIWEATHER_GetCurTempRangeWstr(uint8 cur_index);

/*****************************************************************************/
//  Description : MMIWEATHER_InitNetworkAccount
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWEATHER_InitNetworkAccount(void);

/*****************************************************************************/
//  Description : Get the index of current city.
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note: 
/*****************************************************************************/
PUBLIC uint8 MMIWEATHER_GetCurCity(void);

/*****************************************************************************/
//  Description : Set the index of current city.
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWEATHER_SetCurCity(uint8 index);
/*****************************************************************************/
//  Description : 判断是否曾经添加过该城市
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIWEATHER_IsAddedCity(MMI_STRING_T* city_name_ptr);

/*****************************************************************************/
//  Description : MMIWEATHER_SetSupprotCity
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWEATHER_SetCitySupprot(BOOLEAN is_supprot);

/*****************************************************************************/
//  Description : MMIWEATHER_IsSupprotCity
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIWEATHER_IsSupprotCity(void);
/*****************************************************************************/
// 	Description : 获得天气信息
//	Global resource dependence : 
//  Author: Glen Li
//	Note:
/*****************************************************************************/
PUBLIC char* MMIWEATHER_GetWeatherInformation(void);
/*****************************************************************************/
// 	Description : 天气信息BUFFER 分配
//	Global resource dependence : 
//  Author: Glen Li
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIWEATHER_WeatherInfoAlloc(void);
/*****************************************************************************/
// 	Description : 天气信息BUFFER 释放
//	Global resource dependence : 
//  Author: Glen Li
//	Note:
/*****************************************************************************/
PUBLIC void MMIWEATHER_WeatherInfoRelease(void);

/*****************************************************************************/
// 	Description : MMIWEATHER_GetTemperature
//	Global resource dependence : 
//  Author: juan.wu
//	Note:对天气温度四舍五入
/*****************************************************************************/
PUBLIC int32 MMIWEATHER_GetTemperature(int32 temp);

/*****************************************************************************/
//  Description : 如果为表中内置城市名，返回转换后的城市名，否则返回空。
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note:
/*****************************************************************************/
PUBLIC const char* MMIWEATHER_GetSpecialCityCode(wchar* city_name);

PUBLIC MMIWEATHER_ERROR_INFO_T * MMIWEATHER_GetErrorInfo(void);

PUBLIC MMIWEATHER_COUNTRY_INFO_T * MMIWEATHER_GetCountryInfo(void);

PUBLIC BOOLEAN MMIWEATHER_GetSelCountryFlag(void);

PUBLIC void MMIWEATHER_SetSelCountryFlag(BOOLEAN is_sel);

PUBLIC MMIWEATHER_CITY_TAB_T * MMIWEATHER_GetSpecialCityTable(void);

PUBLIC uint32 MMIWEATHER_GetSpecialCityNum(void);

PUBLIC BOOLEAN MMIWEATHER_IsErrorMsgShowed(void);

PUBLIC void MMIAPIWEATHER_SetTryEnNameFlag(BOOLEAN is_try);
PUBLIC BOOLEAN MMIAPIWEATHER_GetHasTryEnName(void);
PUBLIC BOOLEAN MMIWEATHER_IsCityCanBeAdd(MMI_STRING_T * city_name_ptr);

/*---------------------------------------------------------------------------*/
/*                         Compiler Flag                                     */
/*---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
    
#endif

