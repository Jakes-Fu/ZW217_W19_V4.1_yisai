/*************************************************************************
 ** File Name:      mmiweather_parse.c                            *
 ** Author:         Gaily.Wang                                           *
 ** Date:           2011/04/18                                           *
 ** Copyright:      2011 Spreadtrum, Incorporated. All Rights Reserved.  *
 ** Description:    This file define the functions about data connection *
 *************************************************************************/

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_app_weather_trc.h"
#include "mn_type.h"
#include "xml_sap.h"
#include "mmi_appmsg.h"
#include "mmiweather.h"
#include "mmiweather_text.h"
#include "mmiweather_image.h"
#include "mmiweather_funcs.h"
#include "mmi_text.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmipub.h "

/**-------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/

#define PARSER_SEP_CHAR                "$"     //tag name separator
#define XML_TAG_MAX_LEN                128
#define WEATHER_STRING_MAX_LEN         128
#define WEATHER_CONDITION_STR_MAX_LEN  30
#define WEATHER_WIND_DIR_STR_MAX_LEN   8
#define WEATHER_WEEK_STR_MAX_LEN       4
#define WEATHER_NO_INFO_QUERY_STR     L"No cities match your search query"

#define MMI_CHINESE_FIRST (0x4e00)
#define MMI_CHINESE_LAST (0x9fa5)

#if defined(WEATHER_SUPPORT_GOOGLE) || defined(MMIWEATHER_SUPPORT_SPRD_SRV)
#define WEATHER_PARSE_BUF_SIZE         (1024 * 2)
#endif
#ifdef MMI_WEATHER_SUPPORT_WUNDERGROUND
#define WEATHER_PARSE_BUF_SIZE         (1024 * 20)
#endif

// error no
#define WEATHER_PARSE_SUCCESS                0
#define ERR_PARSE_FAILED 		            -1
#define ERR_OUT_OF_MEMORY			        -2
#define ERR_READ_FILE_FAILED		        -3
#define ERR_INVALID_PARA    				-4	

#define NEXT_WEEK_DAY(WEEK) (WEEK_SUN == WEEK ? WEEK_MON : (WEEK + 1))

/**-------------------------------------------------------------------------*
**                         ENUM DEFINITION                                  *
**--------------------------------------------------------------------------*/
#ifdef MMI_WEATHER_JSON_PARSE
typedef enum 
{
    JSON_TAG_NONE = 0,
    JSON_TAG_ERROR_DESCRIPTION,
    JSON_TAG_COUNTRY_NUM_RESULTS,
    JSON_TAG_COUNTRY_NAME,
    JSON_TAG_CURRENT_WEATHER,
    JSON_TAG_WEATHER_ESTIMATE,//estimated
    JSON_TAG_CURRENT_CITY_DISPLAY,
    JSON_TAG_CURRENT_CITY,
    JSON_TAG_CURRENT_TEMP_F,
    JSON_TAG_CURRENT_TEMP_C,
    JSON_TAG_CURRENT_HUMIDITY,//relative_humidity
    JSON_TAG_CURRENT_WIND_STRING,
    JSON_TAG_FORECAST_WEEK_DAY,
    JSON_TAG_FORECAST_TEMP_HIGH,
    JSON_TAG_FORECAST_TEMP_LOW,
    JSON_TAG_FORECAST_CONDITION, 
    JSON_TAG_FORECAST_TEMP_F,
    JSON_TAG_FORECAST_DATE_TIME,
    JSON_TAG_LOCAL_DATE_TIME,
#ifdef MMIWEATHER_SUPPORT_SPRD_SRV
    JSON_TAG_AUTH_ERROR,
    JSON_TAG_AUTH_ERROR_TYPE,
#endif
    JSON_TAG_MAX
}WEATHER_JSON_TAG_E;
#endif

#ifdef WEATHER_SUPPORT_GOOGLE
typedef enum 
{
    //Weather
    XML_TAG_NONE = 0,				//"xml_tag_none",
    XML_TAG_CONDITION,			    // 1"condition",
    XML_TAG_FORECAST_CONDITIONS,    // 2"forecast_conditions",
    XML_TAG_LOW,                    // 3"low",
    XML_TAG_HIGH,                   // 4"high"
    XML_TAG_DAY_OF_WEEK,            // 5"day_of_week",
    XML_TAG_CURRENT_CONDITIONS,     // 6"current_conditions",
    XML_TAG_FORECAST_INFORMATION,   // 7"forecast_information",
    XML_TAG_CURRENT_DATE_TIME,      // 8"current_date_time",
    XML_TAG_TEMP_F,                 // 9"temp_f",
    XML_TAG_TEMP_C,                 // 10"temp_c",
    XML_TAG_HUMIDITY,               // 11"humidity",
    XML_TAG_WIND_CONDITION,         // 12"wind_condition",
    XML_TAG_FORECAST_DATE,          // 13"forecast_date",
    XML_TAG_WEATHER,                // 14"weather",
    
    //City	
    XML_TAG_CITIES,                // 15"cities",
    XML_TAG_CITY,                  // 16"city",

	//Country
    XML_TAG_COUNTRIES,              // 17"countries",
    XML_TAG_COUNTRY,                // 18"country",
    XML_TAG_NAME,                   // 19"name",
    XML_TAG_ISO_CODE,               // 20"iso_code",
    
    //Others
    XML_TAG_PROBLEM_CAUSE,          // 21"problem_cause"    
    XML_TAG_XML_API_REPLY,          // 22"xml_api_reply"
    XML_TAG_MAX
}WEATHER_XML_TAG_E;
#endif
typedef enum 
{
    //Weather
    WEATHER_CONDITION_TYPE_UNKNOW = 0,
    WEATHER_CONDITION_TYPE_CURRENT,       //"Current condition",
    WEATHER_CONDITION_TYPE_FORECAST,       //"Forecast condition",
    WEATHER_CONDITION_TYPE_MAX
}WEATHER_CONDITION_TYPE_E;

#ifdef MMIWEATHER_SUPPORT_SPRD_SRV
typedef enum
{
    WEATHER_AUTH_ERROR_NONE = 0,
    WEATHER_AUTH_ERROR_KEY_NOT_EXSIT,
    WEATHER_AUTH_ERROR_SIGN_ERROR,
    WEATHER_AUTH_ERROR_AUTH_LEVEL_LESS_THAN,
    WEATHER_AUTH_TIME_PARAM_ERROR,
    WEATHER_AUTH_ERROR_TYPE_MAX
}WEATHER_AUTH_ERROR_TYPE_E;
#endif
/**-------------------------------------------------------------------------*
**                         TYPE DEFINITION                                  *
**--------------------------------------------------------------------------*/

typedef struct _WEATHER_TEMP_INFO
{
    //City
    wchar    *city;              //城市名
    wchar    *forecast_date;     //发布日期
    wchar    *cur_date_time;     //日期时间
    //Current
    wchar    *cur_condition;         //当前天气状况
#ifdef WEATHER_SUPPORT_GOOGLE
    wchar    *cur_temp_c;            //当前温度(℃)
    wchar    *cur_temp_f;            //当前温度(℉)
#endif
#ifdef MMI_WEATHER_JSON_PARSE
    int32    cur_temp_c;            //当前温度(℃)
    int32    cur_temp_f;            //当前温度(℉)
#endif
    wchar    *cur_humidity;          //当前湿度(%)
    wchar    *cur_wind_condition;    //风向()
    //Forecast
    wchar    *fc_week[MMIWEATHER_FORECAST_DAY_NUM];           //星期
    wchar    *fc_condition[MMIWEATHER_FORECAST_DAY_NUM];      //天气状况
    wchar    *fc_low_temp_f[MMIWEATHER_FORECAST_DAY_NUM];     //最低温度(摄氏度)
    wchar    *fc_high_temp_f[MMIWEATHER_FORECAST_DAY_NUM];    //最高温度(摄氏度)
    //Other
    wchar    *problem_cause;   //出问题的原因  
}WEATHER_TEMP_INFO_T;

#ifdef MMI_WEATHER_JSON_PARSE
typedef struct _MMIWEATHER_PARSE
{
    WEATHER_JSON_TAG_E           cur_tag;            // 记录正在解析的 xml tag
    WEATHER_TEMP_INFO_T         temp_info;          // 保存解析后得到的未转义的数据。
    WEATHER_CONDITION_TYPE_E    condition_type;     // 记录正在解析的天气状态的类型(当前的/未来的) 
    MMIWEATHER_COUNTRY_INFO_T   country_info;
#ifdef MMIWEATHER_SUPPORT_SPRD_SRV
    WEATHER_AUTH_ERROR_TYPE_E   error_type;
#endif
    uint8                       fc_index; 
    BOOLEAN                     is_temp_high;
    BOOLEAN                     is_estimated_info;
    BOOLEAN                     is_bad_data;  
    BOOLEAN                     is_support_city;    // 是否支持该城市。
    BOOLEAN                     is_need_add_string;
    BOOLEAN                     is_retry_en_name;
}MMIWEATHER_PARSE_TEMP_T;
#endif

#ifdef WEATHER_SUPPORT_GOOGLE
typedef struct _MMIWEATHER_PARSE
{
    WEATHER_XML_TAG_E           cur_tag;            // 记录正在解析的 xml tag
    WEATHER_TEMP_INFO_T         temp_info;          // 保存解析后得到的未转义的数据。
    WEATHER_CONDITION_TYPE_E    condition_type;     // 记录正在解析的天气状态的类型(当前的/未来的) 
    BOOLEAN                     is_bad_data;        // 非正常数据。
    BOOLEAN                     is_support_city;    // 是否支持该城市。
}MMIWEATHER_PARSE_TEMP_T;
#endif

typedef struct _MMIWEATHER_CONDITIONS_MAP
{
    MMIWEATHER_CONDITION_E      condition_id; 
    wchar                       condition_str[WEATHER_CONDITION_STR_MAX_LEN + 1];     
    MMI_TEXT_ID_T               text_id;
    MMI_IMAGE_ID_T              image_big_id;
    MMI_IMAGE_ID_T              image_middle_id;
    MMI_IMAGE_ID_T              image_small_id;
    MMI_IMAGE_ID_T              image_widget_id;
}MMIWEATHER_CONDITIONS_T;


typedef struct _MMIWEATHER_WIND_DIR_MAP
{
    MMIWEATHER_WIND_DIR_E   wind_dir_id; 
    wchar                   wind_dir_str[WEATHER_WIND_DIR_STR_MAX_LEN + 1];     
    MMI_TEXT_ID_T           text_id;
}MMIWEATHER_WIND_DIR_MAP_T;

typedef struct _MMIWEATHER_WEEK_MAP
{
    MMIWEATHER_WEEK_E   week_id; 
    wchar               week_str[WEATHER_WEEK_STR_MAX_LEN + 1];     
    MMI_TEXT_ID_T       text_id;
}MMIWEATHER_WEEK_MAP_T;

#ifdef MMI_WEATHER_JSON_PARSE
typedef struct _MMIWEATHER_MONTH_TABLE
{
    wchar    *  short_month_name;
    uint8         month_index;
}MMIWEATHER_MONTH_TABLE;

#define  MMI_MONTH_MAX     12
#define  MMI_MONTH_SHORT_LEN 3
#endif
/**-------------------------------------------------------------------------*
**                         GLOBAL VARIABLE DEFINITION                       *
**--------------------------------------------------------------------------*/



/**-------------------------------------------------------------------------*
**                         STATIC CLASS DEFINITION                          *
**--------------------------------------------------------------------------*/

/**-------------------------------------------------------------------------*
**                         LOCAL VARIABLE DEFINITION                        *
**--------------------------------------------------------------------------*/
//xml tags in weather data , must one-to-one with WEATHER_XML_TAG_E
#ifdef MMI_WEATHER_JSON_PARSE
LOCAL const char* const s_json_tag_arr[] = 
{
    "",
    "description",
    "results",
    "country_name",
    "weather",
    "estimated",
    "display_location",
    "city",
    "temp_f",
    "temp_c",
    "relative_humidity",
    "wind_string",
    "weekday_short",
    "high",
    "low",
    "conditions",
    "fahrenheit",
    "observation_time_rfc822",
    "local_time_rfc822",
#ifdef MMIWEATHER_SUPPORT_SPRD_SRV
    "auth_error",
    "error_message",
#endif
};

#endif

#ifdef WEATHER_SUPPORT_GOOGLE
LOCAL const char* const s_xml_tag_arr[] = 
{
	"",		       
    "condition",
    "forecast_conditions",
    "low",
    "high",    
    "day_of_week",
    "current_conditions",
    "forecast_information",
    "current_date_time",
    "temp_f",
    "temp_c",
    "humidity",
    "wind_condition",
    "forecast_date",
    "weather",
    "cities",
    "city",
    "countries",
    "country",
    "name",
    "iso_code",
    "problem_cause",     
    "xml_api_reply"
};
#endif

#ifdef MMIWEATHER_SUPPORT_SPRD_SRV
LOCAL BOOLEAN s_is_showed_err_msg = FALSE;
#endif

LOCAL const MMIWEATHER_CONDITIONS_T s_conditions_map[] = 
{
#ifndef PDA_WEATHER_SUPPORT_LOW_MEMORY
    {
    WEATHER_CONDITION_UNKNOW,
    L"", 
#ifdef MMI_PDA_SUPPORT
    TXT_UNKNOWN,
#else
    TXT_UNKNOWN,
#endif
    IMAGE_WEATHER_CONDITION_BIG_UNKNOWN,
    IMAGE_WEATHER_CONDITION_MIDDLE_UNKNOWN,
    IMAGE_WEATHER_CONDITION_SMALL_UNKNOWN,
    IMAGE_WEATHER_CONDITION_WIDGET_UNKNOWN,  
    },

    {
    WEATHER_CONDITION_CLEAR,
    L"Clear",
    TXT_WEATHER_CONDITION_CLEAR,
    IMAGE_WEATHER_CONDITION_BIG_CLEAR,
    IMAGE_WEATHER_CONDITION_MIDDLE_CLEAR,
    IMAGE_WEATHER_CONDITION_SMALL_CLEAR,
    IMAGE_WEATHER_CONDITION_WIDGET_CLEAR,
    },
    {
    WEATHER_CONDITION_FOG,
    L"Fog",
    TXT_WEATHER_CONDITION_FOG,
    IMAGE_WEATHER_CONDITION_BIG_FOG,
    IMAGE_WEATHER_CONDITION_MIDDLE_FOG,
    IMAGE_WEATHER_CONDITION_SMALL_FOG,
    IMAGE_WEATHER_CONDITION_WIDGET_FOG,
    },

    {
    WEATHER_CONDITION_HAZE,
    L"Haze",
    TXT_WEATHER_CONDITION_HAZE,
    IMAGE_WEATHER_CONDITION_BIG_HAZE,
    IMAGE_WEATHER_CONDITION_MIDDLE_HAZE,
    IMAGE_WEATHER_CONDITION_SMALL_HAZE,
    IMAGE_WEATHER_CONDITION_WIDGET_HAZE,
    },

    {
    WEATHER_CONDITION_CLOUDY,
    L"Cloudy",
    TXT_WEATHER_CONDITION_CLOUDY,
    IMAGE_WEATHER_CONDITION_BIG_CLOUDY,
    IMAGE_WEATHER_CONDITION_MIDDLE_CLOUDY,
    IMAGE_WEATHER_CONDITION_SMALL_CLOUDY,
    IMAGE_WEATHER_CONDITION_WIDGET_CLOUDY,
    },

    {
    WEATHER_CONDITION_PARTLY_CLOUDY,
    L"Partly Cloudy",
    TXT_WEATHER_CONDITION_PARTLY_CLOUDY,
    IMAGE_WEATHER_CONDITION_BIG_PARTLY_CLOUDY,
    IMAGE_WEATHER_CONDITION_MIDDLE_PARTLY_CLOUDY,
    IMAGE_WEATHER_CONDITION_SMALL_PARTLY_CLOUDY,
    IMAGE_WEATHER_CONDITION_WIDGET_PARTLY_CLOUDY,
    },

    {
    WEATHER_CONDITION_MOSTLY_CLOUDY,
    L"Mostly Cloudy",
    TXT_WEATHER_CONDITION_MOSTLY_CLOUDY,
    IMAGE_WEATHER_CONDITION_BIG_MOSTLY_CLOUDY,
    IMAGE_WEATHER_CONDITION_MIDDLE_MOSTLY_CLOUDY,
    IMAGE_WEATHER_CONDITION_SMALL_MOSTLY_CLOUDY,
    IMAGE_WEATHER_CONDITION_WIDGET_MOSTLY_CLOUDY,
    },

    {
    WEATHER_CONDITION_OVERCAST,
    L"Overcast",
    TXT_WEATHER_CONDITION_OVERCAST,
    IMAGE_WEATHER_CONDITION_BIG_OVERCAST,
    IMAGE_WEATHER_CONDITION_MIDDLE_OVERCAST,
    IMAGE_WEATHER_CONDITION_SMALL_OVERCAST,
    IMAGE_WEATHER_CONDITION_WIDGET_OVERCAST,
    },

    {
    WEATHER_CONDITION_RAIN,                
    L"Rain",                
    TXT_WEATHER_CONDITION_RAIN,                         
    IMAGE_WEATHER_CONDITION_BIG_RAIN,               
    IMAGE_WEATHER_CONDITION_MIDDLE_RAIN,                    
    IMAGE_WEATHER_CONDITION_SMALL_RAIN,
    IMAGE_WEATHER_CONDITION_WIDGET_RAIN,
    },

    {

    WEATHER_CONDITION_LIGHT_RAIN,          
    L"Light rain",          
    TXT_WEATHER_CONDITION_LIGHT_RAIN,                   
    IMAGE_WEATHER_CONDITION_BIG_LIGHT_RAIN,         
    IMAGE_WEATHER_CONDITION_MIDDLE_LIGHT_RAIN,              
    IMAGE_WEATHER_CONDITION_SMALL_LIGHT_RAIN,
    IMAGE_WEATHER_CONDITION_WIDGET_LIGHT_RAIN,
    },

    {
    WEATHER_CONDITION_RAIN_SHOWERS,        
    L"Rain Showers",        
    TXT_WEATHER_CONDITION_RAIN_SHOWERS,                 
    IMAGE_WEATHER_CONDITION_BIG_RAIN_SHOWERS,       
    IMAGE_WEATHER_CONDITION_MIDDLE_RAIN_SHOWERS,            
    IMAGE_WEATHER_CONDITION_SMALL_RAIN_SHOWERS,
    IMAGE_WEATHER_CONDITION_WIDGET_RAIN_SHOWERS,
    },

    {
    WEATHER_CONDITION_SCATTERED_SHOWERS,   
    L"Rain Showers",        
    TXT_WEATHER_CONDITION_SCATTERED_SHOWERS,            
    IMAGE_WEATHER_CONDITION_BIG_SCATTERED_SHOWERS,  
    IMAGE_WEATHER_CONDITION_MIDDLE_SCATTERED_SHOWERS,       
    IMAGE_WEATHER_CONDITION_SMALL_SCATTERED_SHOWERS,
    IMAGE_WEATHER_CONDITION_WIDGET_SCATTERED_SHOWERS,
    },

    {
    WEATHER_CONDITION_SHOWERS,             
    L"Showers",             
    TXT_WEATHER_CONDITION_SHOWERS,                      
    IMAGE_WEATHER_CONDITION_BIG_SHOWERS,            
    IMAGE_WEATHER_CONDITION_MIDDLE_SHOWERS,                 
    IMAGE_WEATHER_CONDITION_SMALL_SHOWERS,
    IMAGE_WEATHER_CONDITION_WIDGET_SHOWERS,
    },

    {
    WEATHER_CONDITION_THUNDERSTORM,       
    L"Thunderstorm",        
    TXT_WEATHER_CONDITION_THUNDERSTORM,                 
    IMAGE_WEATHER_CONDITION_BIG_THUNDERSTORM,       
    IMAGE_WEATHER_CONDITION_MIDDLE_THUNDERSTORM,            
    IMAGE_WEATHER_CONDITION_SMALL_THUNDERSTORM,
    IMAGE_WEATHER_CONDITION_WIDGET_THUNDERSTORM,
    },

    {
    WEATHER_CONDITION_CHANCE_OF_SHOWERS,   
    L"Chance of Showers",   
    TXT_WEATHER_CONDITION_CHANCE_OF_SHOWERS,            
    IMAGE_WEATHER_CONDITION_BIG_CHANCE_OF_SHOWERS,  
    IMAGE_WEATHER_CONDITION_MIDDLE_CHANCE_OF_SHOWERS,       
    IMAGE_WEATHER_CONDITION_SMALL_CHANCE_OF_SHOWERS,
    IMAGE_WEATHER_CONDITION_WIDGET_CHANCE_OF_SHOWERS,
    },

    {
    WEATHER_CONDITION_CHANCE_OF_SNOW,      
    L"Chance of Snow",      
    TXT_WEATHER_CONDITION_CHANCE_OF_SNOW,               
    IMAGE_WEATHER_CONDITION_BIG_CHANCE_OF_SNOW,     
    IMAGE_WEATHER_CONDITION_MIDDLE_CHANCE_OF_SNOW,          
    IMAGE_WEATHER_CONDITION_SMALL_CHANCE_OF_SNOW,
    IMAGE_WEATHER_CONDITION_WIDGET_CHANCE_OF_SNOW,
    },

    {
    WEATHER_CONDITION_CHANCE_OF_STORM,     
    L"Chance of Storm",     
    TXT_WEATHER_CONDITION_CHANCE_OF_STORM,              
    IMAGE_WEATHER_CONDITION_BIG_CHANCE_OF_STORM,    
    IMAGE_WEATHER_CONDITION_MIDDLE_CHANCE_OF_STORM,         
    IMAGE_WEATHER_CONDITION_SMALL_CHANCE_OF_STORM,
    IMAGE_WEATHER_CONDITION_WIDGET_CHANCE_OF_STORM,
    },

    {
    WEATHER_CONDITION_SUNNY,               
    L"Sunny",               
    TXT_WEATHER_CONDITION_SUNNY,                        
    IMAGE_WEATHER_CONDITION_BIG_SUNNY,              
    IMAGE_WEATHER_CONDITION_MIDDLE_SUNNY,                   
    IMAGE_WEATHER_CONDITION_SMALL_SUNNY,
    IMAGE_WEATHER_CONDITION_WIDGET_SUNNY,
    },

    {
    WEATHER_CONDITION_MOSTLY_SUNNY,        
    L"Mostly Sunny",        
    TXT_WEATHER_CONDITION_MOSTLY_SUNNY,                 
    IMAGE_WEATHER_CONDITION_BIG_MOSTLY_SUNNY,       
    IMAGE_WEATHER_CONDITION_MIDDLE_MOSTLY_SUNNY,            
    IMAGE_WEATHER_CONDITION_SMALL_MOSTLY_SUNNY,
    IMAGE_WEATHER_CONDITION_WIDGET_MOSTLY_SUNNY,
    },

    {
    WEATHER_CONDITION_PARTLY_SUNNY,        
    L"Partly Sunny",        
    TXT_WEATHER_CONDITION_PARTLY_SUNNY,                 
    IMAGE_WEATHER_CONDITION_BIG_PARTLY_SUNNY,       
    IMAGE_WEATHER_CONDITION_MIDDLE_PARTLY_SUNNY,            
    IMAGE_WEATHER_CONDITION_SMALL_PARTLY_SUNNY,
    IMAGE_WEATHER_CONDITION_WIDGET_PARTLY_SUNNY,
    },

    {
    WEATHER_CONDITION_MIST,                
    L"Mist",                
    TXT_WEATHER_CONDITION_MIST,                         
    IMAGE_WEATHER_CONDITION_BIG_MIST,               
    IMAGE_WEATHER_CONDITION_MIDDLE_MIST,                    
    IMAGE_WEATHER_CONDITION_SMALL_MIST,
    IMAGE_WEATHER_CONDITION_WIDGET_MIST,
    },

    {
    WEATHER_CONDITION_SMOKE,               
    L"Smoke",               
    TXT_WEATHER_CONDITION_SMOKE,                        
    IMAGE_WEATHER_CONDITION_BIG_SMOKE,              
    IMAGE_WEATHER_CONDITION_MIDDLE_SMOKE,                   
    IMAGE_WEATHER_CONDITION_SMALL_SMOKE,
    IMAGE_WEATHER_CONDITION_WIDGET_SMOKE,
    },

    {
    WEATHER_CONDITION_STORM,               
    L"Storm",               
    TXT_WEATHER_CONDITION_STORM,                        
    IMAGE_WEATHER_CONDITION_BIG_STORM,              
    IMAGE_WEATHER_CONDITION_MIDDLE_STORM,                   
    IMAGE_WEATHER_CONDITION_SMALL_STORM,
    IMAGE_WEATHER_CONDITION_WIDGET_STORM,
    },

    {
    WEATHER_CONDITION_FLURRIES,            
    L"Flurries",            
    TXT_WEATHER_CONDITION_FLURRIES,                     
    IMAGE_WEATHER_CONDITION_BIG_FLURRIES,           
    IMAGE_WEATHER_CONDITION_MIDDLE_FLURRIES,                
    IMAGE_WEATHER_CONDITION_SMALL_FLURRIES,
    IMAGE_WEATHER_CONDITION_WIDGET_FLURRIES,
    },

    {
    WEATHER_CONDITION_CHANCE_OF_RAIN,      
    L"Chance of Rain",      
    TXT_WEATHER_CONDITION_CHANCE_OF_RAIN,               
    IMAGE_WEATHER_CONDITION_BIG_CHANCE_OF_RAIN,     
    IMAGE_WEATHER_CONDITION_MIDDLE_CHANCE_OF_RAIN,          
    IMAGE_WEATHER_CONDITION_SMALL_CHANCE_OF_RAIN,
    IMAGE_WEATHER_CONDITION_WIDGET_CHANCE_OF_RAIN,
    },

    {
    WEATHER_CONDITION_CHANCE_OF_TSTORM,    
    L"Chance of TStorm",    
    TXT_WEATHER_CONDITION_CHANCE_OF_TSTORM,             
    IMAGE_WEATHER_CONDITION_BIG_CHANCE_OF_TSTORM,   
    IMAGE_WEATHER_CONDITION_MIDDLE_CHANCE_OF_TSTORM,        
    IMAGE_WEATHER_CONDITION_SMALL_CHANCE_OF_TSTORM,
    IMAGE_WEATHER_CONDITION_WIDGET_CHANCE_OF_TSTORM,
    },

    {
    WEATHER_CONDITION_SLEET,               
    L"Sleet",               
    TXT_WEATHER_CONDITION_SLEET,                        
    IMAGE_WEATHER_CONDITION_BIG_SLEET,              
    IMAGE_WEATHER_CONDITION_MIDDLE_SLEET,                   
    IMAGE_WEATHER_CONDITION_SMALL_SLEET,
    IMAGE_WEATHER_CONDITION_WIDGET_SLEET,
    },
    {
    WEATHER_CONDITION_SNOW,                
    L"Snow",                
    TXT_WEATHER_CONDITION_SNOW,                         
    IMAGE_WEATHER_CONDITION_BIG_SNOW,               
    IMAGE_WEATHER_CONDITION_MIDDLE_SNOW,                    
    IMAGE_WEATHER_CONDITION_SMALL_SNOW,
    IMAGE_WEATHER_CONDITION_WIDGET_SNOW,
    },

    {
    WEATHER_CONDITION_ICY,                 
    L"Icy",                 
    TXT_WEATHER_CONDITION_ICY,                          
    IMAGE_WEATHER_CONDITION_BIG_ICY,                
    IMAGE_WEATHER_CONDITION_MIDDLE_ICY,                     
    IMAGE_WEATHER_CONDITION_SMALL_ICY,
    IMAGE_WEATHER_CONDITION_WIDGET_ICY,
    },
    {
    WEATHER_CONDITION_DUST,                
    L"Dust",                
    TXT_WEATHER_CONDITION_DUST,                         
    IMAGE_WEATHER_CONDITION_BIG_DUST,               
    IMAGE_WEATHER_CONDITION_MIDDLE_DUST,                    
    IMAGE_WEATHER_CONDITION_SMALL_DUST,
    IMAGE_WEATHER_CONDITION_WIDGET_DUST,
    },

    {
    WEATHER_CONDITION_SCATTERED_THUNDERSTORMS,  
    L"Scattered Thunderstorms", 
    TXT_WEATHER_CONDITION_SCATTERED_THUNDERSTORMS,
    IMAGE_WEATHER_CONDITION_BIG_SCATTERED_THUNDERSTORMS, 
    IMAGE_WEATHER_CONDITION_MIDDLE_SCATTERED_THUNDERSTORMS, 
    IMAGE_WEATHER_CONDITION_SMALL_SCATTERED_THUNDERSTORMS,
    IMAGE_WEATHER_CONDITION_WIDGET_SCATTERED_THUNDERSTORMS,
    },
    {
    //可能有小雪
    WEATHER_CONDITION_CHANCE_OF_FLURRIES,  
    L"Chance of Flurries", 
    TXT_WEATHER_CONDITION_CHANCE_OF_FLURRIES,
    IMAGE_WEATHER_CONDITION_BIG_FLURRIES,           
    IMAGE_WEATHER_CONDITION_MIDDLE_FLURRIES,                
    IMAGE_WEATHER_CONDITION_SMALL_FLURRIES,
    IMAGE_WEATHER_CONDITION_WIDGET_FLURRIES,
    },
    {
    WEATHER_CONDITION_CHANCE_OF_SLEET,
    L"Chance of Freezing Rain", 
    TXT_WEATHER_CONDITION_CHANCE_OF_SLEET,
    IMAGE_WEATHER_CONDITION_BIG_SLEET,              
    IMAGE_WEATHER_CONDITION_MIDDLE_SLEET,                   
    IMAGE_WEATHER_CONDITION_SMALL_SLEET,
    IMAGE_WEATHER_CONDITION_WIDGET_SLEET,
    },
    {
    WEATHER_CONDITION_CHANCE_OF_SLEET,
    L"Chance of Sleet", 
    TXT_WEATHER_CONDITION_CHANCE_OF_SLEET,
    IMAGE_WEATHER_CONDITION_BIG_SLEET,              
    IMAGE_WEATHER_CONDITION_MIDDLE_SLEET,                   
    IMAGE_WEATHER_CONDITION_SMALL_SLEET,
    IMAGE_WEATHER_CONDITION_WIDGET_SLEET,
    },    
    {
    WEATHER_CONDITION_CHANCE_OF_TSTORM,
    L"Chance of Thunderstorms",
    TXT_WEATHER_CONDITION_CHANCE_OF_TSTORM,             
    IMAGE_WEATHER_CONDITION_BIG_CHANCE_OF_TSTORM,   
    IMAGE_WEATHER_CONDITION_MIDDLE_CHANCE_OF_TSTORM,        
    IMAGE_WEATHER_CONDITION_SMALL_CHANCE_OF_TSTORM,
    IMAGE_WEATHER_CONDITION_WIDGET_CHANCE_OF_TSTORM,
    },
    {
    WEATHER_CONDITION_CHANCE_OF_TSTORM,
    L"Chance of a Thunderstorm",
    TXT_WEATHER_CONDITION_CHANCE_OF_TSTORM,             
    IMAGE_WEATHER_CONDITION_BIG_CHANCE_OF_TSTORM,   
    IMAGE_WEATHER_CONDITION_MIDDLE_CHANCE_OF_TSTORM,        
    IMAGE_WEATHER_CONDITION_SMALL_CHANCE_OF_TSTORM,
    IMAGE_WEATHER_CONDITION_WIDGET_CHANCE_OF_TSTORM,
    },
    {
    WEATHER_CONDITION_SLEET,
    L"Freezing Rain",
    TXT_WEATHER_CONDITION_SLEET,                        
    IMAGE_WEATHER_CONDITION_BIG_SLEET,              
    IMAGE_WEATHER_CONDITION_MIDDLE_SLEET,                   
    IMAGE_WEATHER_CONDITION_SMALL_SLEET,
    IMAGE_WEATHER_CONDITION_WIDGET_SLEET,
    },
    {
    WEATHER_CONDITION_THUNDERSTORM,
    L"Thunderstorms",
    TXT_WEATHER_CONDITION_THUNDERSTORM,                 
    IMAGE_WEATHER_CONDITION_BIG_THUNDERSTORM,       
    IMAGE_WEATHER_CONDITION_MIDDLE_THUNDERSTORM,            
    IMAGE_WEATHER_CONDITION_SMALL_THUNDERSTORM,
    IMAGE_WEATHER_CONDITION_WIDGET_THUNDERSTORM,
    },
    {
    WEATHER_CONDITION_PARTLY_CLOUDY,
    L"Scattered Clouds",
    TXT_WEATHER_CONDITION_PARTLY_CLOUDY,
    IMAGE_WEATHER_CONDITION_BIG_PARTLY_CLOUDY,
    IMAGE_WEATHER_CONDITION_MIDDLE_PARTLY_CLOUDY,
    IMAGE_WEATHER_CONDITION_SMALL_PARTLY_CLOUDY,
    IMAGE_WEATHER_CONDITION_WIDGET_PARTLY_CLOUDY,
    },
    {
    WEATHER_CONDITION_UNKNOW,
    L"Unknown", 
    TXT_UNKNOWN,
    IMAGE_WEATHER_CONDITION_BIG_UNKNOWN,
    IMAGE_WEATHER_CONDITION_MIDDLE_UNKNOWN,
    IMAGE_WEATHER_CONDITION_SMALL_UNKNOWN,
    IMAGE_WEATHER_CONDITION_WIDGET_UNKNOWN,  
    },
    
#else
    {
    WEATHER_CONDITION_UNKNOW,
    L"", 
#if defined BAR_PHONE_WIDGET || defined MMI_WIDGET_WEATHER2 
    TXT_UNKNOWN_WEATHINFO,
#else
#ifdef MMI_PDA_SUPPORT
    TXT_UNKNOWN,
#else
    TXT_UNKNOWN,
#endif
#endif
    IMAGE_WEATHER_CONDITION_MIDDLE_UNKNOWN,
    IMAGE_WEATHER_CONDITION_SMALL_UNKNOWN,
    IMAGE_WEATHER_CONDITION_SMALL_UNKNOWN,
    IMAGE_WEATHER_CONDITION_MIDDLE_UNKNOWN,
    },

    {
    WEATHER_CONDITION_CLEAR,
    L"Clear",
    TXT_WEATHER_CONDITION_CLEAR,
    IMAGE_WEATHER_CONDITION_MIDDLE_SUNNY,              
    IMAGE_WEATHER_CONDITION_SMALL_SUNNY,                   
    IMAGE_WEATHER_CONDITION_SMALL_SUNNY,
    IMAGE_WEATHER_CONDITION_MIDDLE_SUNNY,
    },
    {
    WEATHER_CONDITION_FOG,
    L"Fog",
    TXT_WEATHER_CONDITION_FOG,
    IMAGE_WEATHER_CONDITION_MIDDLE_CLOUDY,
    IMAGE_WEATHER_CONDITION_SMALL_CLOUDY,
    IMAGE_WEATHER_CONDITION_SMALL_CLOUDY,
    IMAGE_WEATHER_CONDITION_MIDDLE_CLOUDY,
    },

    {
    WEATHER_CONDITION_HAZE,
    L"Haze",
    TXT_WEATHER_CONDITION_HAZE,
    IMAGE_WEATHER_CONDITION_MIDDLE_CLOUDY,
    IMAGE_WEATHER_CONDITION_SMALL_CLOUDY,
    IMAGE_WEATHER_CONDITION_SMALL_CLOUDY,
    IMAGE_WEATHER_CONDITION_MIDDLE_CLOUDY,
    },

    {
    WEATHER_CONDITION_CLOUDY,
    L"Cloudy",
    TXT_WEATHER_CONDITION_CLOUDY,
    IMAGE_WEATHER_CONDITION_MIDDLE_CLOUDY,
    IMAGE_WEATHER_CONDITION_SMALL_CLOUDY,
    IMAGE_WEATHER_CONDITION_SMALL_CLOUDY,
    IMAGE_WEATHER_CONDITION_MIDDLE_CLOUDY,
    },

    {
    WEATHER_CONDITION_PARTLY_CLOUDY,
    L"Partly Cloudy",
    TXT_WEATHER_CONDITION_PARTLY_CLOUDY,
    IMAGE_WEATHER_CONDITION_MIDDLE_CLOUDY,
    IMAGE_WEATHER_CONDITION_SMALL_CLOUDY,
    IMAGE_WEATHER_CONDITION_SMALL_CLOUDY,
    IMAGE_WEATHER_CONDITION_MIDDLE_CLOUDY,
    },

    {
    WEATHER_CONDITION_MOSTLY_CLOUDY,
    L"Mostly Cloudy",
    TXT_WEATHER_CONDITION_MOSTLY_CLOUDY,
    IMAGE_WEATHER_CONDITION_MIDDLE_CLOUDY,
    IMAGE_WEATHER_CONDITION_SMALL_CLOUDY,
    IMAGE_WEATHER_CONDITION_SMALL_CLOUDY,
    IMAGE_WEATHER_CONDITION_MIDDLE_CLOUDY,
    },

    {
    WEATHER_CONDITION_OVERCAST,
    L"Overcast",
    TXT_WEATHER_CONDITION_OVERCAST,
    IMAGE_WEATHER_CONDITION_MIDDLE_CLOUDY,
    IMAGE_WEATHER_CONDITION_SMALL_CLOUDY,
    IMAGE_WEATHER_CONDITION_SMALL_CLOUDY,
    IMAGE_WEATHER_CONDITION_MIDDLE_CLOUDY,
    },

    {
    WEATHER_CONDITION_RAIN,                
    L"Rain",                
    TXT_WEATHER_CONDITION_RAIN,                         
    IMAGE_WEATHER_CONDITION_MIDDLE_RAIN,               
    IMAGE_WEATHER_CONDITION_SMALL_RAIN,                    
    IMAGE_WEATHER_CONDITION_SMALL_RAIN,
    IMAGE_WEATHER_CONDITION_MIDDLE_RAIN,
    },

    {

    WEATHER_CONDITION_LIGHT_RAIN,          
    L"Light rain",          
    TXT_WEATHER_CONDITION_LIGHT_RAIN,                   
    IMAGE_WEATHER_CONDITION_MIDDLE_LIGHT_RAIN,         
    IMAGE_WEATHER_CONDITION_SMALL_LIGHT_RAIN,              
    IMAGE_WEATHER_CONDITION_SMALL_LIGHT_RAIN,
    IMAGE_WEATHER_CONDITION_MIDDLE_LIGHT_RAIN,
    },

    {
    WEATHER_CONDITION_RAIN_SHOWERS,        
    L"Rain Showers",        
    TXT_WEATHER_CONDITION_RAIN_SHOWERS,                 
    IMAGE_WEATHER_CONDITION_MIDDLE_RAIN,               
    IMAGE_WEATHER_CONDITION_SMALL_RAIN,                    
    IMAGE_WEATHER_CONDITION_SMALL_RAIN,
    IMAGE_WEATHER_CONDITION_MIDDLE_RAIN,
    },

    {
    WEATHER_CONDITION_SCATTERED_SHOWERS,   
    L"Rain Showers",        
    TXT_WEATHER_CONDITION_SCATTERED_SHOWERS,            
    IMAGE_WEATHER_CONDITION_MIDDLE_LIGHT_RAIN,         
    IMAGE_WEATHER_CONDITION_SMALL_LIGHT_RAIN,              
    IMAGE_WEATHER_CONDITION_SMALL_LIGHT_RAIN,
    IMAGE_WEATHER_CONDITION_MIDDLE_LIGHT_RAIN,
    },

    {
    WEATHER_CONDITION_SHOWERS,             
    L"Showers",             
    TXT_WEATHER_CONDITION_SHOWERS,                      
    IMAGE_WEATHER_CONDITION_MIDDLE_RAIN,               
    IMAGE_WEATHER_CONDITION_SMALL_RAIN,                    
    IMAGE_WEATHER_CONDITION_SMALL_RAIN,
    IMAGE_WEATHER_CONDITION_MIDDLE_RAIN,
    },

    {
    WEATHER_CONDITION_THUNDERSTORM,       
    L"Thunderstorm",        
    TXT_WEATHER_CONDITION_THUNDERSTORM,                 
    IMAGE_WEATHER_CONDITION_MIDDLE_RAIN,               
    IMAGE_WEATHER_CONDITION_SMALL_RAIN,                    
    IMAGE_WEATHER_CONDITION_SMALL_RAIN,
    IMAGE_WEATHER_CONDITION_MIDDLE_RAIN,
    },

    {
    WEATHER_CONDITION_CHANCE_OF_SHOWERS,   
    L"Chance of Showers",   
    TXT_WEATHER_CONDITION_CHANCE_OF_SHOWERS,            
    IMAGE_WEATHER_CONDITION_MIDDLE_LIGHT_RAIN,         
    IMAGE_WEATHER_CONDITION_SMALL_LIGHT_RAIN,              
    IMAGE_WEATHER_CONDITION_SMALL_LIGHT_RAIN,
    IMAGE_WEATHER_CONDITION_MIDDLE_LIGHT_RAIN,
    },

    {
    WEATHER_CONDITION_CHANCE_OF_SNOW,      
    L"Chance of Snow",      
    TXT_WEATHER_CONDITION_CHANCE_OF_SNOW,               
    IMAGE_WEATHER_CONDITION_MIDDLE_SNOW,               
    IMAGE_WEATHER_CONDITION_SMALL_SNOW,                    
    IMAGE_WEATHER_CONDITION_SMALL_SNOW,
    IMAGE_WEATHER_CONDITION_MIDDLE_SNOW,
    },

    {
    WEATHER_CONDITION_CHANCE_OF_STORM,     
    L"Chance of Storm",     
    TXT_WEATHER_CONDITION_CHANCE_OF_STORM,              
    IMAGE_WEATHER_CONDITION_MIDDLE_LIGHT_RAIN,         
    IMAGE_WEATHER_CONDITION_SMALL_LIGHT_RAIN,              
    IMAGE_WEATHER_CONDITION_SMALL_LIGHT_RAIN,
    IMAGE_WEATHER_CONDITION_MIDDLE_LIGHT_RAIN,
    },

    {
    WEATHER_CONDITION_SUNNY,               
    L"Sunny",               
    TXT_WEATHER_CONDITION_SUNNY,                        
    IMAGE_WEATHER_CONDITION_MIDDLE_SUNNY,              
    IMAGE_WEATHER_CONDITION_SMALL_SUNNY,                   
    IMAGE_WEATHER_CONDITION_SMALL_SUNNY,
    IMAGE_WEATHER_CONDITION_MIDDLE_SUNNY,
    },

    {
    WEATHER_CONDITION_MOSTLY_SUNNY,        
    L"Mostly Sunny",        
    TXT_WEATHER_CONDITION_MOSTLY_SUNNY,                 
    IMAGE_WEATHER_CONDITION_MIDDLE_SUNNY,              
    IMAGE_WEATHER_CONDITION_SMALL_SUNNY,                   
    IMAGE_WEATHER_CONDITION_SMALL_SUNNY,
    IMAGE_WEATHER_CONDITION_MIDDLE_SUNNY,
    },

    {
    WEATHER_CONDITION_PARTLY_SUNNY,        
    L"Partly Sunny",        
    TXT_WEATHER_CONDITION_PARTLY_SUNNY,                 
    IMAGE_WEATHER_CONDITION_MIDDLE_SUNNY,              
    IMAGE_WEATHER_CONDITION_SMALL_SUNNY,                   
    IMAGE_WEATHER_CONDITION_SMALL_SUNNY,
    IMAGE_WEATHER_CONDITION_MIDDLE_SUNNY,
    },

    {
    WEATHER_CONDITION_MIST,                
    L"Mist",                
    TXT_WEATHER_CONDITION_MIST,                         
    IMAGE_WEATHER_CONDITION_MIDDLE_CLOUDY,
    IMAGE_WEATHER_CONDITION_SMALL_CLOUDY,
    IMAGE_WEATHER_CONDITION_SMALL_CLOUDY,
    IMAGE_WEATHER_CONDITION_MIDDLE_CLOUDY,
    },

    {
    WEATHER_CONDITION_SMOKE,               
    L"Smoke",               
    TXT_WEATHER_CONDITION_SMOKE,                        
    IMAGE_WEATHER_CONDITION_MIDDLE_CLOUDY,
    IMAGE_WEATHER_CONDITION_SMALL_CLOUDY,
    IMAGE_WEATHER_CONDITION_SMALL_CLOUDY,
    IMAGE_WEATHER_CONDITION_MIDDLE_CLOUDY,
    },

    {
    WEATHER_CONDITION_STORM,               
    L"Storm",               
    TXT_WEATHER_CONDITION_STORM,                        
    IMAGE_WEATHER_CONDITION_MIDDLE_RAIN,               
    IMAGE_WEATHER_CONDITION_SMALL_RAIN,                    
    IMAGE_WEATHER_CONDITION_SMALL_RAIN,
    IMAGE_WEATHER_CONDITION_MIDDLE_RAIN,
    },

    {
    WEATHER_CONDITION_FLURRIES,            
    L"Flurries",            
    TXT_WEATHER_CONDITION_FLURRIES,                     
    IMAGE_WEATHER_CONDITION_MIDDLE_LIGHT_RAIN,         
    IMAGE_WEATHER_CONDITION_SMALL_LIGHT_RAIN,              
    IMAGE_WEATHER_CONDITION_SMALL_LIGHT_RAIN,
    IMAGE_WEATHER_CONDITION_MIDDLE_LIGHT_RAIN,
    },

    {
    WEATHER_CONDITION_CHANCE_OF_RAIN,      
    L"Chance of Rain",      
    TXT_WEATHER_CONDITION_CHANCE_OF_RAIN,               
    IMAGE_WEATHER_CONDITION_MIDDLE_LIGHT_RAIN,         
    IMAGE_WEATHER_CONDITION_SMALL_LIGHT_RAIN,              
    IMAGE_WEATHER_CONDITION_SMALL_LIGHT_RAIN,
    IMAGE_WEATHER_CONDITION_MIDDLE_LIGHT_RAIN,
    },

    {
    WEATHER_CONDITION_CHANCE_OF_TSTORM,    
    L"Chance of TStorm",    
    TXT_WEATHER_CONDITION_CHANCE_OF_TSTORM,             
    IMAGE_WEATHER_CONDITION_MIDDLE_LIGHT_RAIN,         
    IMAGE_WEATHER_CONDITION_SMALL_LIGHT_RAIN,              
    IMAGE_WEATHER_CONDITION_SMALL_LIGHT_RAIN,
    IMAGE_WEATHER_CONDITION_MIDDLE_LIGHT_RAIN,
    },

    {
    WEATHER_CONDITION_SLEET,               
    L"Sleet",               
    TXT_WEATHER_CONDITION_SLEET,                        
    IMAGE_WEATHER_CONDITION_MIDDLE_SLEET,              
    IMAGE_WEATHER_CONDITION_SMALL_SLEET,                   
    IMAGE_WEATHER_CONDITION_SMALL_SLEET,
    IMAGE_WEATHER_CONDITION_MIDDLE_SLEET,
    },
    {
    WEATHER_CONDITION_SNOW,                
    L"Snow",                
    TXT_WEATHER_CONDITION_SNOW,                         
    IMAGE_WEATHER_CONDITION_MIDDLE_SNOW,               
    IMAGE_WEATHER_CONDITION_SMALL_SNOW,                    
    IMAGE_WEATHER_CONDITION_SMALL_SNOW,
    IMAGE_WEATHER_CONDITION_MIDDLE_SNOW,
    },

    {
    WEATHER_CONDITION_ICY,                 
    L"Icy",                 
    TXT_WEATHER_CONDITION_ICY,                          
    IMAGE_WEATHER_CONDITION_MIDDLE_SLEET,              
    IMAGE_WEATHER_CONDITION_SMALL_SLEET,                   
    IMAGE_WEATHER_CONDITION_SMALL_SLEET,
    IMAGE_WEATHER_CONDITION_MIDDLE_SLEET,
    },
    {
    WEATHER_CONDITION_DUST,                
    L"Dust",                
    TXT_WEATHER_CONDITION_DUST,                         
    IMAGE_WEATHER_CONDITION_MIDDLE_CLOUDY,
    IMAGE_WEATHER_CONDITION_SMALL_CLOUDY,
    IMAGE_WEATHER_CONDITION_SMALL_CLOUDY,
    IMAGE_WEATHER_CONDITION_MIDDLE_CLOUDY,
    },

    {
    WEATHER_CONDITION_SCATTERED_THUNDERSTORMS,  
    L"Scattered Thunderstorms", 
    TXT_WEATHER_CONDITION_SCATTERED_THUNDERSTORMS,
    IMAGE_WEATHER_CONDITION_MIDDLE_LIGHT_RAIN,         
    IMAGE_WEATHER_CONDITION_SMALL_LIGHT_RAIN,              
    IMAGE_WEATHER_CONDITION_SMALL_LIGHT_RAIN,
    IMAGE_WEATHER_CONDITION_MIDDLE_LIGHT_RAIN,
    },
    {                                                          
    //可能有小雪                                            
	WEATHER_CONDITION_CHANCE_OF_FLURRIES,                    
	L"Chance of Flurries",                                 
	TXT_WEATHER_CONDITION_CHANCE_OF_FLURRIES,              
	IMAGE_WEATHER_CONDITION_MIDDLE_FLURRIES,               
	IMAGE_WEATHER_CONDITION_SMALL_FLURRIES,                
	IMAGE_WEATHER_CONDITION_SMALL_FLURRIES,                
	IMAGE_WEATHER_CONDITION_MIDDLE_FLURRIES,       
	},                                                 
	{                                                         
	WEATHER_CONDITION_CHANCE_OF_SLEET,                     
	L"Chance of Freezing Rain",                          
	TXT_WEATHER_CONDITION_CHANCE_OF_SLEET,               
	IMAGE_WEATHER_CONDITION_MIDDLE_SLEET,                
	IMAGE_WEATHER_CONDITION_SMALL_SLEET,                 
	IMAGE_WEATHER_CONDITION_SMALL_SLEET,                 
	IMAGE_WEATHER_CONDITION_MIDDLE_SLEET,                
	},                                                       
	{                                                        
	WEATHER_CONDITION_CHANCE_OF_SLEET,                   
	L"Chance of Sleet",                                
	TXT_WEATHER_CONDITION_CHANCE_OF_SLEET,             
	IMAGE_WEATHER_CONDITION_MIDDLE_SLEET,              
	IMAGE_WEATHER_CONDITION_SMALL_SLEET,               
	IMAGE_WEATHER_CONDITION_SMALL_SLEET,               
	IMAGE_WEATHER_CONDITION_MIDDLE_SLEET,              
	},                                                     
	{                                                      
	WEATHER_CONDITION_CHANCE_OF_TSTORM,                
	L"Chance of Thunderstorms",                      
	TXT_WEATHER_CONDITION_CHANCE_OF_TSTORM,          
	IMAGE_WEATHER_CONDITION_MIDDLE_CHANCE_OF_TSTORM, 
	IMAGE_WEATHER_CONDITION_SMALL_CHANCE_OF_TSTORM,  
	IMAGE_WEATHER_CONDITION_SMALL_CHANCE_OF_TSTORM,  
	IMAGE_WEATHER_CONDITION_MIDDLE_CHANCE_OF_TSTORM, 
	},                                                   
	{                                                    
	WEATHER_CONDITION_CHANCE_OF_TSTORM,              
	L"Chance of a Thunderstorm",                   
	TXT_WEATHER_CONDITION_CHANCE_OF_TSTORM,        
	IMAGE_WEATHER_CONDITION_MIDDLE_CHANCE_OF_TSTORM,
	IMAGE_WEATHER_CONDITION_SMALL_CHANCE_OF_TSTORM,
	IMAGE_WEATHER_CONDITION_SMALL_CHANCE_OF_TSTORM,
	IMAGE_WEATHER_CONDITION_MIDDLE_CHANCE_OF_TSTORM
	},                                                 
	{                                                  
	WEATHER_CONDITION_SLEET,                       
	L"Freezing Rain",                            
	TXT_WEATHER_CONDITION_SLEET,                 
	IMAGE_WEATHER_CONDITION_MIDDLE_SLEET,        
	IMAGE_WEATHER_CONDITION_SMALL_SLEET,         
	IMAGE_WEATHER_CONDITION_SMALL_SLEET,         
	IMAGE_WEATHER_CONDITION_MIDDLE_SLEET,        
	},                                               
	{                                                
	WEATHER_CONDITION_THUNDERSTORM,              
	L"Thunderstorms",                          
	TXT_WEATHER_CONDITION_THUNDERSTORM,        
	IMAGE_WEATHER_CONDITION_MIDDLE_THUNDERSTORM,       
	IMAGE_WEATHER_CONDITION_SMALL_THUNDERSTORM,        
	IMAGE_WEATHER_CONDITION_SMALL_THUNDERSTORM,        
	IMAGE_WEATHER_CONDITION_MIDDLE_THUNDERSTORM,       
	},                                                     
	{                                                      
	WEATHER_CONDITION_PARTLY_CLOUDY,                   
	L"Scattered Clouds",                             
	TXT_WEATHER_CONDITION_PARTLY_CLOUDY,             
	IMAGE_WEATHER_CONDITION_MIDDLE_PARTLY_CLOUDY,    
	IMAGE_WEATHER_CONDITION_SMALL_PARTLY_CLOUDY,     
	IMAGE_WEATHER_CONDITION_SMALL_PARTLY_CLOUDY,     
	IMAGE_WEATHER_CONDITION_MIDDLE_PARTLY_CLOUDY,    
	},                                                   
	{                                                    
	WEATHER_CONDITION_UNKNOW,                        
	L"Unknown",                                    
	TXT_UNKNOWN,                                   
	IMAGE_WEATHER_CONDITION_MIDDLE_UNKNOWN,        
	IMAGE_WEATHER_CONDITION_SMALL_UNKNOWN,         
	IMAGE_WEATHER_CONDITION_SMALL_UNKNOWN,         
	IMAGE_WEATHER_CONDITION_MIDDLE_UNKNOWN,        
    },
#endif
};

LOCAL const MMIWEATHER_WIND_DIR_MAP_T s_wind_dir_map[] = 
{
#ifdef MMI_PDA_SUPPORT
    {WIND_DIR_UNKNOW,   L"",                TXT_UNKNOWN     },
#else
    {WIND_DIR_UNKNOW,   L"",                TXT_UNKNOWN     },
#endif
    {WIND_DIR_NNE,      L" NNE ",           TXT_WEATHER_WIND_NNE    },
    {WIND_DIR_NE,       L" NE ",            TXT_WEATHER_WIND_NE     },
    {WIND_DIR_ENE,      L" ENE ",           TXT_WEATHER_WIND_ENE    },
    {WIND_DIR_ESE,      L" ESE ",           TXT_WEATHER_WIND_ESE    },
    {WIND_DIR_SE,       L" SE ",            TXT_WEATHER_WIND_SE     },
    {WIND_DIR_SSE,      L" SSE ",           TXT_WEATHER_WIND_SSE    },
    {WIND_DIR_SSW,      L" SSW ",           TXT_WEATHER_WIND_SSW    },
    {WIND_DIR_SW,       L" SW ",            TXT_WEATHER_WIND_SW     },
    {WIND_DIR_WSW,      L" WSW ",           TXT_WEATHER_WIND_WSW    },
    {WIND_DIR_WNW,      L" WNW ",           TXT_WEATHER_WIND_WNW    },
    {WIND_DIR_NW,       L" NW ",            TXT_WEATHER_WIND_NW     },
    {WIND_DIR_NNW,      L" NNW ",           TXT_WEATHER_WIND_NNW    },
#if defined(MMI_WEATHER_JSON_PARSE)
    {WIND_DIR_N,        L" North ",             TXT_WEATHER_WIND_N      },
    {WIND_DIR_E,        L" East ",             TXT_WEATHER_WIND_E      },
    {WIND_DIR_S,        L" South ",             TXT_WEATHER_WIND_S      },
    {WIND_DIR_W,        L" West ",             TXT_WEATHER_WIND_W      },
#else 
    {WIND_DIR_N,        L" N ",             TXT_WEATHER_WIND_N      },
    {WIND_DIR_E,        L" E ",             TXT_WEATHER_WIND_E      },
    {WIND_DIR_S,        L" S ",             TXT_WEATHER_WIND_S      },
    {WIND_DIR_W,        L" W ",             TXT_WEATHER_WIND_W      },
#endif
    
};

LOCAL const MMIWEATHER_WEEK_MAP_T s_week_map[] = 
{
#ifdef MMI_PDA_SUPPORT
    {WEEK_UNKNOW,   L"",                TXT_UNKNOWN     },
#else
    {WEEK_UNKNOW,   L"",                TXT_UNKNOWN     },
#endif
    {WEEK_MON,        L"Mon",   TXT_WEATHER_WEEK_MON    },
    {WEEK_TUE,        L"Tue",   TXT_WEATHER_WEEK_TUE    },
    {WEEK_WED,        L"Wed",   TXT_WEATHER_WEEK_WED    },
    {WEEK_THU,        L"Thu",   TXT_WEATHER_WEEK_THU    },
    {WEEK_FRI,        L"Fri",   TXT_WEATHER_WEEK_FRI    },
    {WEEK_SAT,        L"Sat",   TXT_WEATHER_WEEK_SAT    }, 
    {WEEK_SUN,        L"Sun",   TXT_WEATHER_WEEK_SUN    }
};


//解析时保存中间数据，用于转换为MMI显示的数据。


/**-------------------------------------------------------------------------*
**                         LOCAL FUNCTION DECLARE                           *
**--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : atoi function for wstring.
//  Global resource dependence : none
//  Author: Gaily.Wang
//  Note:
/*****************************************************************************/
LOCAL int32 wstr_atoi(wchar* wstr_ptr);

#ifdef WEATHER_SUPPORT_GOOGLE
/*****************************************************************************/
//  Description : Do parse weather data
//  Global resource dependence : none
//  Author: Gaily.Wang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN Xml_ParseWeatherData(
                            uint8* buf_ptr,                         //[in] Data buffer to paser
                            int32 buf_len,                         //[in] Data length to paser
                            MMIWEATHER_PARSE_TEMP_T *parse_data_ptr //[out] Data get
                            );

/*****************************************************************************
// 	Description : Set element's attributes
//	Global resource dependence : 
//  Author:Gaily.Wang
//	Note: 
*****************************************************************************/
LOCAL void Xml_GetElementAttributes(
                                MMIWEATHER_PARSE_TEMP_T *parse_data_ptr,
                                WEATHER_XML_TAG_E       tag, 
                                const uint16            **attributes_pptr
                                );
#endif
/*****************************************************************************/
//  Description : Handle parsed data
//  Global resource dependence : none
//  Author: Gaily.Wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIWEATHER_HandleParsedData(MMIWEATHER_PARSE_TEMP_T *temp_data);

#ifdef WEATHER_SUPPORT_GOOGLE
/*****************************************************************************/
//  Description : Start parse
//  Global resource dependence : none
//  Author: Gaily.Wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIWEATHER_ParseStart(void)
{
	BOOLEAN result      = FALSE;
    char* buf_ptr = PNULL;
        
	//SCI_TRACE_LOW:"[weather]:MMIWEATHER_StartParse---"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWEATHER_PARSE_913_112_2_18_3_6_41_65,(uint8*)"");
    
    //buf_ptr = SCI_ALLOC_APPZ(WEATHER_PARSE_BUF_SIZE);  //commented;
    buf_ptr = MMIWEATHER_GetWeatherInformation();  //compatible modification to store weather information with buffer and file mode;

    if(PNULL != buf_ptr)
    {
#ifdef WEATHER_INFORMATION_BUFFER_STORE_SUPPORT        
#else
        if(MMIWEATHER_ReadFile(HTTP_CONTENT_FILE_NAME, buf_ptr, WEATHER_PARSE_BUF_SIZE))
#endif
        {
            MMIWEATHER_PARSE_TEMP_T temp_data = {0};
            SCI_MEMSET(&temp_data, 0, sizeof(MMIWEATHER_PARSE_TEMP_T));
            //is_bad_data默认为TRUE,在解析过程中，
            //只要收到XML_TAG_XML_API_REPLY，则会置为FALSE，正常情况都为FALSE
            temp_data.is_bad_data = TRUE;
        	if(Xml_ParseWeatherData((uint8*)buf_ptr, strlen(buf_ptr), &temp_data))
        	{
            	//SCI_TRACE_LOW:"[weather]:MMIWEATHER_StartParse---Parse success!"
            	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWEATHER_PARSE_929_112_2_18_3_6_41_66,(uint8*)"");
            	//SCI_TRACE_LOW:"[weather]:MMIWEATHER_StartParse is_support = %d"
            	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWEATHER_PARSE_930_112_2_18_3_6_41_67,(uint8*)"d",temp_data.is_support_city);
                result = MMIWEATHER_HandleParsedData(&temp_data);
            }
            SCI_TRACE_LOW("MMIWEATHER_ParseStart is_bad_data = %d",temp_data.is_bad_data);
        }
    }

    //SCI_FREE(buf_ptr);  //commented;

    if(result)
    {
        MMIWEATHER_SendMsgToApp(MSG_WEATHER_PARSE_SUCCESS_CNF);
    }
    else
    {
        MMIWEATHER_SendMsgToApp(MSG_WEATHER_PARSE_FAILED_CNF);
    }
    
    return result;
}
#endif
/*****************************************************************************/
//  Description : Get Parsed Data
//  Global resource dependence : none
//  Author: Gaily.Wang
//  Note:
/*****************************************************************************/
LOCAL void GetNextDay(DATE_TIME_T *date_ptr)
{
    date_ptr->nDay++;
    switch(date_ptr->nMonth)
    {
        case 1:
        {
            if(date_ptr->nDay > 31)
            {
                date_ptr->nDay   = 1;
                date_ptr->nMonth = 2;
            }
        }
        break;

    case 2:
        {
            uint8 day_num = 28;
            if(MMIAPICOM_IsLeapYear(date_ptr->nYear))
            {
                day_num = 29;    
            }
            
            if(date_ptr->nDay > day_num)
            {
                date_ptr->nDay   = 1;
                date_ptr->nMonth = 3;
            }
        }
        break;

    case 3:
        {
            if(date_ptr->nDay > 31)
            {
                date_ptr->nDay   = 1;
                date_ptr->nMonth = 4;
            }
        }
        break;
    case 4:
        {
            if(date_ptr->nDay > 30)
            {
                date_ptr->nDay   = 1;
                date_ptr->nMonth = 5;
            }
        }
        break;
    case 5:
        {
            if(date_ptr->nDay > 31)
            {
                date_ptr->nDay   = 1;
                date_ptr->nMonth = 6;
            }
        }
        break;
    case 6:
        {
            if(date_ptr->nDay > 30)
            {
                date_ptr->nDay   = 1;
                date_ptr->nMonth = 7;
            }
        }
        break;
    case 7:
        {
            if(date_ptr->nDay > 31)
            {
                date_ptr->nDay   = 1;
                date_ptr->nMonth = 8;
            }
        }
        break;
    case 8:
        {
            if(date_ptr->nDay > 31)
            {
                date_ptr->nDay   = 1;
                date_ptr->nMonth = 9;
            }
        }
        break;
        
    case 9:
        {
            if(date_ptr->nDay > 30)
            {
                date_ptr->nDay   = 1;
                date_ptr->nMonth = 10;
            }
        }
        break;
        
    case 10:
        {
            if(date_ptr->nDay > 31)
            {
                date_ptr->nDay   = 1;
                date_ptr->nMonth = 11;
            }
        }
        break;
        
    case 11:
        {
            if(date_ptr->nDay > 30)
            {
                date_ptr->nDay   = 1;
                date_ptr->nMonth = 12;
            }
        }
        break;
        
    case 12:
        {
            if(date_ptr->nDay > 31)
            {
                date_ptr->nDay   = 1;
                date_ptr->nMonth = 1;
                date_ptr->nYear++;
            }
        }
        break;
        
    default:
        {
            
        }
        break;
    }
}
#ifdef MMI_WEATHER_JSON_PARSE
/*****************************************************************************/
//  Description : GetMonth by month short name
//  Global resource dependence : none
//  Author: juan.wu
//  Note:
/*****************************************************************************/
LOCAL int GetMonth(wchar * str_ptr,uint16 str_len)
{
    int month = 0;
    uint8 i = 0;
    uint16 cmp_len = MIN(str_len,MMI_MONTH_SHORT_LEN);
    MMIWEATHER_MONTH_TABLE month_table[MMI_MONTH_MAX] = {
                                            {L"Jan",1},
                                            {L"Feb",2},
                                            {L"Mar",3},
                                            {L"Apr",4},
                                            {L"May",5},
                                            {L"Jun",6},
                                            {L"Jul",7},
                                            {L"Aug",8},
                                            {L"Sep",9},
                                            {L"Oct",10},
                                            {L"Nov",11},
                                            {L"Dec",12}
                                          };
    
    for(i = 0; i < MMI_MONTH_MAX; i++)
    {
        if( 0 == MMIAPICOM_Wstrncmp(month_table[i].short_month_name, str_ptr, cmp_len))
        {
            month = month_table[i].month_index;
            break;
        }
    }
    return month;
}

/*****************************************************************************/
//  Description : GetForecastData
//  Global resource dependence : none
//  Author: juan.wu
//  Note:
/*****************************************************************************/
LOCAL void GetForecastDate(wchar * temp_ptr,
                            wchar * end_ptr,
                            MMIWEATHER_WEATHER_T * weather_info_ptr)
{
    wchar    space = L' ';
    MMI_STRING_T   temp_str[5] ={0};
    uint8 j = 0;
    
    while(temp_ptr < end_ptr)
    {
        if(space == *temp_ptr++)
        {
            temp_str[j].wstr_ptr = temp_ptr;
            j++;
        }
    }
    for(j = 0; j < 4; j++)
    {
        temp_str[j].wstr_len = temp_str[j+1].wstr_ptr - temp_str[j].wstr_ptr - 1;
    }
    weather_info_ptr->pub_time.nYear = wstr_atoi(temp_str[2].wstr_ptr);
    weather_info_ptr->pub_time.nMonth= GetMonth(temp_str[1].wstr_ptr,temp_str[1].wstr_len);
    weather_info_ptr->pub_time.nDay  = wstr_atoi(temp_str[0].wstr_ptr);

}
#endif
#ifdef MMIWEATHER_SUPPORT_SPRD_SRV
/*****************************************************************************/
//  Description : ShowErrorAuthTypeMsg
//  Global resource dependence : none
//  Author: juan.wu
//  Note:show auth error msg
/*****************************************************************************/
LOCAL void ShowErrorAuthTypeMsg(WEATHER_AUTH_ERROR_TYPE_E   error_type)
{
    SCI_TRACE_LOW("ShowErrorAuthTypeMsg error_type = %d",error_type);
    switch(error_type)
    {
    case WEATHER_AUTH_ERROR_KEY_NOT_EXSIT:
    case WEATHER_AUTH_ERROR_SIGN_ERROR:
    case WEATHER_AUTH_ERROR_AUTH_LEVEL_LESS_THAN:
        //Do nothing
        break;

    case WEATHER_AUTH_TIME_PARAM_ERROR:
        s_is_showed_err_msg = TRUE;
        MMIPUB_OpenAlertFailWin(TXT_WEATHER_PLS_CHECK_TIME_AND_DATE);
        break;

    default:
        break;
    }
}
#endif
/*****************************************************************************/
//  Description : Handle parsed data
//  Global resource dependence : none
//  Author: Gaily.Wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIWEATHER_HandleParsedData(MMIWEATHER_PARSE_TEMP_T *temp_data)
{
    BOOLEAN result    = TRUE;
    wchar   *temp_ptr = PNULL;
    wchar   *end_ptr  = PNULL;
    uint16  wstr_len  = 0;
    uint32  index     = 0;
    MMIWEATHER_WEATHER_T*    weather_info_ptr = PNULL;  
    
    if( PNULL == temp_data )
    {
        result = FALSE;
    }
    else
    {
        if(temp_data->is_bad_data)
        {
            //Send wrong city code will cause this problem.
            if(PNULL != temp_data->temp_info.problem_cause)
            {
                SCI_FREE(temp_data->temp_info.problem_cause);
            }
            result = FALSE;
        }
        else
        {
#ifdef MMIWEATHER_SUPPORT_SPRD_SRV
            if( temp_data->error_type != WEATHER_AUTH_ERROR_NONE)
            {
                ShowErrorAuthTypeMsg(temp_data->error_type);
                return FALSE;
            }
#endif
            //is support?
            if(temp_data->is_support_city)
            {
                MMIWEATHER_SetCitySupprot(TRUE);
                if(MMIWEATHER_UPDATE_FROM_ADD_CITY == MMIWEATHER_GetUpdateType())
                {
                	weather_info_ptr = MMIWEATHER_GetCityWeatherInfo(MMIWEATHER_GetCityNum());
                }
                else
                {
                	weather_info_ptr = MMIWEATHER_GetCurCityWeatherInfo();
                }
            }
            else
            {
                MMIWEATHER_SetCitySupprot(FALSE);
                return FALSE;
            }
            if(PNULL == weather_info_ptr)
            {
                //SCI_TRACE_LOW:"[WEATHER]MMIWEATHER_HandleParsedData PNULL == weather_info_ptr !"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWEATHER_PARSE_1158_112_2_18_3_6_41_68,(uint8*)"");
                return FALSE;
            }
            SCI_MEMSET(&weather_info_ptr->current, 0, sizeof(weather_info_ptr->current));
            SCI_MEMSET(&weather_info_ptr->forecast, 0, sizeof(weather_info_ptr->forecast));

            //City
            if(PNULL != temp_data->temp_info.city)
            {
                SCI_FREE(temp_data->temp_info.city);
            }
        
            //forecast_date
            temp_ptr = temp_data->temp_info.forecast_date;
            if(PNULL != temp_ptr)
            {
                end_ptr  = temp_ptr + MMIAPICOM_Wstrlen(temp_ptr);

#ifdef MMI_WEATHER_JSON_PARSE
                GetForecastDate(temp_ptr,end_ptr,weather_info_ptr);
#else
                //Year
                weather_info_ptr->pub_time.nYear = wstr_atoi(temp_ptr);
                
                //Month
                while(temp_ptr < end_ptr)
                {
                    if(L'-' == *temp_ptr)
                    {
                        temp_ptr++;
                        break;
                    }
                    
                    temp_ptr++;
                }
                
                if(temp_ptr < end_ptr)
                {
                    weather_info_ptr->pub_time.nMonth = wstr_atoi(temp_ptr);
                }
                
                //Day
                while(temp_ptr < end_ptr)
                {
                    if(L'-' == *temp_ptr)
                    {
                        temp_ptr++;
                        break;
                    }
                    
                    temp_ptr++;
                }
                
                if(temp_ptr < end_ptr)
                {
                    weather_info_ptr->pub_time.nDay = wstr_atoi(temp_ptr);
                }
#endif
                //fc_date
                if(weather_info_ptr->pub_time.nMonth && weather_info_ptr->pub_time.nDay)
                {
                    char   date_str[12] = {0};
                    
                    DATE_TIME_T date = weather_info_ptr->pub_time;
                    
                    //Day_1
                    sprintf(date_str, "%d/%d", date.nMonth, date.nDay);
                    MMIAPICOM_StrToWstr((uint8*)date_str, weather_info_ptr->forecast.date1);
                    
                    //Day_2
                    GetNextDay(&date);
                    sprintf(date_str, "%d/%d", date.nMonth, date.nDay);
                    MMIAPICOM_StrToWstr((uint8*)date_str, weather_info_ptr->forecast.date2);
                    
                    //Day_3
                    GetNextDay(&date);
                    sprintf(date_str, "%d/%d", date.nMonth, date.nDay);
                    MMIAPICOM_StrToWstr((uint8*)date_str, weather_info_ptr->forecast.date3);
                    
                    //Day_4
                    GetNextDay(&date);
                    sprintf(date_str, "%d/%d", date.nMonth, date.nDay);
                    MMIAPICOM_StrToWstr((uint8*)date_str, weather_info_ptr->forecast.date4);


                    //pub_date_wstr
                    sprintf(date_str, "%d/%d/%d", weather_info_ptr->pub_time.nYear, weather_info_ptr->pub_time.nMonth, weather_info_ptr->pub_time.nDay);                    
                    MMIAPICOM_StrToWstr((uint8*)date_str, weather_info_ptr->pub_date_wstr);
                    
                }
                
#ifndef MMI_WEATHER_JSON_PARSE
                SCI_FREE(temp_data->temp_info.forecast_date);
#endif
            }

#ifdef MMI_WEATHER_JSON_PARSE
            temp_ptr = temp_data->temp_info.forecast_date;
#else
            temp_ptr = temp_data->temp_info.cur_date_time;
#endif
            if(PNULL != temp_ptr)            //cur_date_time
            {
                char        time_str[6]  = {0};
                end_ptr  = temp_ptr + MMIAPICOM_Wstrlen(temp_ptr);
                while(temp_ptr && (temp_ptr + 3) < end_ptr)
                {
                    if(L':' == *(temp_ptr + 2))
                    {
                        //Hour
#ifdef MMI_WEATHER_JSON_PARSE
                        weather_info_ptr->pub_time.nHour  = wstr_atoi(temp_ptr);
#else
                        weather_info_ptr->pub_time.nHour  = (wstr_atoi(temp_ptr) + 18) % 24;
#endif
                        //Minut
                        weather_info_ptr->pub_time.nMinut = wstr_atoi(temp_ptr + 3);

                        #ifdef WIN32
                        {
                            SCI_TIME_T systime = {0};
                            TM_GetSysTime(&systime);
                            weather_info_ptr->pub_time.nMinut = systime.sec;
                        }
                        #endif

                        if(weather_info_ptr->pub_time.nHour < 10 && weather_info_ptr->pub_time.nMinut < 10) // 01:01
                        {
                            sprintf(time_str, "0%d:0%d", weather_info_ptr->pub_time.nHour, weather_info_ptr->pub_time.nMinut);
                        }
                        else if(weather_info_ptr->pub_time.nHour < 10)    // 01:12
                        {
                            sprintf(time_str, "0%d:%d", weather_info_ptr->pub_time.nHour, weather_info_ptr->pub_time.nMinut);
                        }
                        else if(weather_info_ptr->pub_time.nMinut < 10)   //12:01
                        {
                            sprintf(time_str, "%d:0%d", weather_info_ptr->pub_time.nHour, weather_info_ptr->pub_time.nMinut);
                        }
                        else                        //12:30
                        {
                            sprintf(time_str, "%d:%d", weather_info_ptr->pub_time.nHour, weather_info_ptr->pub_time.nMinut);
                        }
                        
                        //pub_time_wstr
						SCI_MEMSET(weather_info_ptr->pub_time_wstr, 0, (MMIWEATHER_PUB_TIME_STR_LEN + 1) * sizeof(wchar));
                        MMIAPICOM_StrToWstr((uint8*)time_str, weather_info_ptr->pub_time_wstr);
                        break;
                    }
                    
                    temp_ptr++;
                }
#ifdef MMI_WEATHER_JSON_PARSE
				SCI_FREE(temp_data->temp_info.forecast_date);
#endif
                SCI_FREE(temp_data->temp_info.cur_date_time);

            }
            
            //cur_condition
            if(PNULL != temp_data->temp_info.cur_condition)
            {
                index = ARR_SIZE(s_conditions_map);
                while(index--)
                {
                    if( 0 == MMIAPICOM_Wstrncmp(temp_data->temp_info.cur_condition, 
                                            s_conditions_map[index].condition_str, 
                                            MMIAPICOM_Wstrlen(s_conditions_map[index].condition_str)))
                    {
                       weather_info_ptr->current.condition = s_conditions_map[index].condition_id;
					   break;
                    }
                }
                if(WEATHER_CONDITION_UNKNOW == index)
                {
                    uint8 condition_str[30] = {0};
                    MMIAPICOM_WstrToStr(temp_data->temp_info.cur_condition, condition_str);
                	//SCI_TRACE_LOW:"[weather]:MMIWEATHER_HandleParsedData---%s---"
                	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWEATHER_PARSE_1320_112_2_18_3_6_42_69,(uint8*)"s", condition_str);
                }

                SCI_FREE(temp_data->temp_info.cur_condition);
            }

#ifdef MMI_WEATHER_JSON_PARSE
            weather_info_ptr->current.temp_c = temp_data->temp_info.cur_temp_c;
            weather_info_ptr->current.temp_f = temp_data->temp_info.cur_temp_f;
            SCI_TRACE_LOW("weather parse temp_f = %d,temp_c = %d",\
                weather_info_ptr->current.temp_f,weather_info_ptr->current.temp_c);
#else
            //cur_temp_c 
            if(PNULL != temp_data->temp_info.cur_temp_c)
            {
                if(MMIAPICOM_Wstrlen(temp_data->temp_info.cur_temp_c) > 0)
                {
                    weather_info_ptr->current.temp_c = wstr_atoi(temp_data->temp_info.cur_temp_c);
                }
                else
                {
                    weather_info_ptr->current.temp_c = MMIWEATHER_TEMP_UNKNOW;
                }
                SCI_FREE(temp_data->temp_info.cur_temp_c);
                
            }
            
            //cur_temp_f 
            if(PNULL != temp_data->temp_info.cur_temp_f)
            {
                if(MMIAPICOM_Wstrlen(temp_data->temp_info.cur_temp_f) > 0)
                {
                    weather_info_ptr->current.temp_f = wstr_atoi(temp_data->temp_info.cur_temp_f);
                }
                else
                {
                    weather_info_ptr->current.temp_f = MMIWEATHER_TEMP_UNKNOW;
                }
                SCI_FREE(temp_data->temp_info.cur_temp_f);
            }
#endif
            //cur_humidity 
            temp_ptr = temp_data->temp_info.cur_humidity;
            if(PNULL != temp_ptr)
            {
                end_ptr   = temp_ptr + MMIAPICOM_Wstrlen(temp_ptr);
#ifndef MMI_WEATHER_JSON_PARSE
                //找到湿度值的起始位置。
                while(temp_ptr < end_ptr)
                {
                    if(L' ' == *temp_ptr)
                    {
                        temp_ptr++;
                        break;
                    }
                    
                    temp_ptr++;
                }
#endif
				if (temp_ptr < end_ptr)
				{
				    uint8 humidity = wstr_atoi(temp_ptr);
                    if(0 != humidity)
					{
						MMIAPICOM_Wstrncpy(weather_info_ptr->current.humidity, temp_ptr, MMIAPICOM_Wstrlen(temp_ptr));
					}
				}
                SCI_FREE(temp_data->temp_info.cur_humidity);
            }
            
            //cur_wind_condition
            temp_ptr  = temp_data->temp_info.cur_wind_condition;
            if(PNULL != temp_ptr)
            {
#ifdef MMI_WEATHER_JSON_PARSE
            temp_ptr += 8;//add a string offset "from the"
#endif
				wstr_len  = MMIAPICOM_Wstrlen(temp_ptr);
				end_ptr   = temp_ptr + wstr_len;
				//获取风向
				index = WIND_DIR_MAX;
				while (index--)
				{
                    uint16 map_len = 3;
                    if(MMIAPICOM_Wstrnstr(temp_ptr,
										   wstr_len,
										   s_wind_dir_map[index].wind_dir_str,
										   map_len))
					{
						weather_info_ptr->current.wind_dir = s_wind_dir_map[index].wind_dir_id;
						break;
					}
				}

				//获取风力
                while( temp_ptr < end_ptr)
                {
                    if(L'0' <= *temp_ptr && *temp_ptr <= L'9')
                    {
                        break;
                    }
                    temp_ptr++;
                }
                
				if (temp_ptr < end_ptr)
				{
	                weather_info_ptr->current.wind_force = wstr_atoi(temp_ptr);
				}
                
                SCI_FREE(temp_data->temp_info.cur_wind_condition);

            }

            //fc_week
            if(PNULL != temp_data->temp_info.fc_week[FC_DAY_1])
            {
				index = WEEK_MAX;
				while (index--)
				{
                    if( 0 == MMIAPICOM_Wstrncmp(temp_data->temp_info.fc_week[FC_DAY_1], 
                                            s_week_map[index].week_str, 
                                            MMIAPICOM_Wstrlen(s_week_map[index].week_str)))
                    {
                       weather_info_ptr->forecast.week1 = s_week_map[index].week_id;
                       weather_info_ptr->forecast.week2 = NEXT_WEEK_DAY(weather_info_ptr->forecast.week1);
                       weather_info_ptr->forecast.week3 = NEXT_WEEK_DAY(weather_info_ptr->forecast.week2);
                       weather_info_ptr->forecast.week4 = NEXT_WEEK_DAY(weather_info_ptr->forecast.week3);
					   break;
                    }
				}
                SCI_FREE(temp_data->temp_info.fc_week[FC_DAY_1]);
                SCI_FREE(temp_data->temp_info.fc_week[FC_DAY_2]);
                SCI_FREE(temp_data->temp_info.fc_week[FC_DAY_3]);
                SCI_FREE(temp_data->temp_info.fc_week[FC_DAY_4]);
            }

            //fc_condition 1
            if(PNULL != temp_data->temp_info.fc_condition[FC_DAY_1])
            {
                index = ARR_SIZE(s_conditions_map);
                wstr_len = MMIAPICOM_Wstrlen(temp_data->temp_info.fc_condition[FC_DAY_1]);
                while(index--)
                {
                    if(wstr_len != MMIAPICOM_Wstrlen(s_conditions_map[index].condition_str))
                    {
                        continue;
                    }
                    if( 0 == MMIAPICOM_Wstrncmp(temp_data->temp_info.fc_condition[FC_DAY_1], 
                                            s_conditions_map[index].condition_str, 
                                            wstr_len))
                    {
                       weather_info_ptr->forecast.weather1 = s_conditions_map[index].condition_id;
                       if(WEATHER_CONDITION_UNKNOW == weather_info_ptr->current.condition)
                       {
                            //如果当前天气状况未知，则使用预报的当天的天气状况。
                            weather_info_ptr->current.condition = weather_info_ptr->forecast.weather1;
                       }
					   break;
                    }
                }
                SCI_FREE(temp_data->temp_info.fc_condition[FC_DAY_1]);

            }
            
            // 2
            if(PNULL != temp_data->temp_info.fc_condition[FC_DAY_2])
            {
                index = ARR_SIZE(s_conditions_map);
                wstr_len = MMIAPICOM_Wstrlen(temp_data->temp_info.fc_condition[FC_DAY_2]);
                while(index--)
                {
                    if(wstr_len != MMIAPICOM_Wstrlen(s_conditions_map[index].condition_str))
                    {
                        continue;
                    }

                    if( 0 == MMIAPICOM_Wstrncmp(temp_data->temp_info.fc_condition[FC_DAY_2], 
                                            s_conditions_map[index].condition_str, 
                                            wstr_len))
                    {
                       weather_info_ptr->forecast.weather2 = s_conditions_map[index].condition_id;
					   break;
                    }
                }
                SCI_FREE(temp_data->temp_info.fc_condition[FC_DAY_2]);
            }
            
            // 3
            if(PNULL != temp_data->temp_info.fc_condition[FC_DAY_3])
            {
                index = ARR_SIZE(s_conditions_map);
                wstr_len = MMIAPICOM_Wstrlen(temp_data->temp_info.fc_condition[FC_DAY_3]);
                while(index--)
                {
                    if(wstr_len != MMIAPICOM_Wstrlen(s_conditions_map[index].condition_str))
                    {
                        continue;
                    }
                    if( 0 == MMIAPICOM_Wstrncmp(temp_data->temp_info.fc_condition[FC_DAY_3], 
                                            s_conditions_map[index].condition_str, 
                                            wstr_len))
                    {
                       weather_info_ptr->forecast.weather3 = s_conditions_map[index].condition_id;
					   break;
                    }
                }
                SCI_FREE(temp_data->temp_info.fc_condition[FC_DAY_3]);
            }
            
            // 4
            if(PNULL != temp_data->temp_info.fc_condition[FC_DAY_4])
            {
                index = ARR_SIZE(s_conditions_map);
                wstr_len = MMIAPICOM_Wstrlen(temp_data->temp_info.fc_condition[FC_DAY_4]);
                while(index--)
                {
                    if(wstr_len != MMIAPICOM_Wstrlen(s_conditions_map[index].condition_str))
                    {
                        continue;
                    }
                    if( 0 == MMIAPICOM_Wstrncmp(temp_data->temp_info.fc_condition[FC_DAY_4], 
                                            s_conditions_map[index].condition_str, 
                                            wstr_len))
                    {
                       weather_info_ptr->forecast.weather4 = s_conditions_map[index].condition_id;
					   break;
                    }
                }
                SCI_FREE(temp_data->temp_info.fc_condition[FC_DAY_4]);
            }

            //fc_low_temp_f 1 
            if(PNULL != temp_data->temp_info.fc_low_temp_f[FC_DAY_1])
            {
                if(MMIAPICOM_Wstrlen(temp_data->temp_info.fc_low_temp_f[FC_DAY_1]) > 0)
                {
                    weather_info_ptr->forecast.low1 = wstr_atoi(temp_data->temp_info.fc_low_temp_f[FC_DAY_1]);
                }
                SCI_FREE(temp_data->temp_info.fc_low_temp_f[FC_DAY_1]);
            }

            // 2 
            if(PNULL != temp_data->temp_info.fc_low_temp_f[FC_DAY_2] )
            {
                if(MMIAPICOM_Wstrlen(temp_data->temp_info.fc_low_temp_f[FC_DAY_2]) > 0)
                {
                    weather_info_ptr->forecast.low2 = wstr_atoi(temp_data->temp_info.fc_low_temp_f[FC_DAY_2]);
                }
                SCI_FREE(temp_data->temp_info.fc_low_temp_f[FC_DAY_2]);
            }

            // 3 
            if(PNULL != temp_data->temp_info.fc_low_temp_f[FC_DAY_3])
            {
                if(MMIAPICOM_Wstrlen(temp_data->temp_info.fc_low_temp_f[FC_DAY_3]) > 0)
                {
                    weather_info_ptr->forecast.low3 = wstr_atoi(temp_data->temp_info.fc_low_temp_f[FC_DAY_3]);
                }
                SCI_FREE(temp_data->temp_info.fc_low_temp_f[FC_DAY_3]);
            }
            
            // 4 
            if(PNULL != temp_data->temp_info.fc_low_temp_f[FC_DAY_4])
            {
                if(MMIAPICOM_Wstrlen(temp_data->temp_info.fc_low_temp_f[FC_DAY_4]) > 0)
                {
                    weather_info_ptr->forecast.low4 = wstr_atoi(temp_data->temp_info.fc_low_temp_f[FC_DAY_4]);
                }
                SCI_FREE(temp_data->temp_info.fc_low_temp_f[FC_DAY_4]);
            }

            //fc_high_temp_f 1 
            if(PNULL != temp_data->temp_info.fc_high_temp_f[FC_DAY_1])
            {
                if(MMIAPICOM_Wstrlen(temp_data->temp_info.fc_high_temp_f[FC_DAY_1]) > 0)
                {
                    weather_info_ptr->forecast.high1 = wstr_atoi(temp_data->temp_info.fc_high_temp_f[FC_DAY_1]);
                }
                SCI_FREE(temp_data->temp_info.fc_high_temp_f[FC_DAY_1]);
            }

            // 2 
            if(PNULL != temp_data->temp_info.fc_high_temp_f[FC_DAY_2])
            {
                if(MMIAPICOM_Wstrlen(temp_data->temp_info.fc_high_temp_f[FC_DAY_2]) > 0)
                {
                    weather_info_ptr->forecast.high2 = wstr_atoi(temp_data->temp_info.fc_high_temp_f[FC_DAY_2]);
                }
                SCI_FREE(temp_data->temp_info.fc_high_temp_f[FC_DAY_2]);
            }

            // 3 
            if(PNULL != temp_data->temp_info.fc_high_temp_f[FC_DAY_3])
            {
                if(MMIAPICOM_Wstrlen(temp_data->temp_info.fc_high_temp_f[FC_DAY_3]) > 0)
                {
                    weather_info_ptr->forecast.high3 = wstr_atoi(temp_data->temp_info.fc_high_temp_f[FC_DAY_3]);
                }
                SCI_FREE(temp_data->temp_info.fc_high_temp_f[FC_DAY_3]);
            }
            
            // 4 
            if(PNULL != temp_data->temp_info.fc_high_temp_f[FC_DAY_4])
            {
                if(MMIAPICOM_Wstrlen(temp_data->temp_info.fc_high_temp_f[FC_DAY_4]) > 0)
                {
                    weather_info_ptr->forecast.high4 = wstr_atoi(temp_data->temp_info.fc_high_temp_f[FC_DAY_4]);
                }
                SCI_FREE(temp_data->temp_info.fc_high_temp_f[FC_DAY_4]);
            }

        }
        
    }
    
	//SCI_TRACE_LOW:"[weather]:MMIWEATHER_HandleParsedData---result = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWEATHER_PARSE_1617_112_2_18_3_6_42_70,(uint8*)"d", result);
    
    return result;
}

#ifdef WEATHER_SUPPORT_GOOGLE
/*****************************************************************************/
//  Description : start element handler
//  Global resource dependence : none
//  Author: Gaily.Wang
//  Note:
/*****************************************************************************/
void Xml_StartElementHandler(
                        void         *user_data_ptr,
                        const uint16 *localname_ptr,
                        const uint16 **attributes_pptr
                        )
{
    
    int32 tag_id = XML_TAG_MAX;
    MMIWEATHER_PARSE_TEMP_T *parse_data_ptr = (MMIWEATHER_PARSE_TEMP_T *)user_data_ptr;
	uint16   len = 0;
	uint16   localname_len = 0;
    
    MMIAPICOM_WstrTraceOut(localname_ptr, MMIAPICOM_Wstrlen(localname_ptr));

    // 设置解析的属性
    while( --tag_id )
	{
		len = strlen(s_xml_tag_arr[tag_id]);
        localname_len = MMIAPICOM_Wstrlen(localname_ptr);
		
		len = MAX(len,localname_len);
		if( 0 == MMIAPICOM_WstrStrncmp((const uint16*)localname_ptr, (const uint8*)s_xml_tag_arr[tag_id], len))
		{
            break;
		}
	}
    
	parse_data_ptr->cur_tag = tag_id;
    
    Xml_GetElementAttributes( parse_data_ptr, parse_data_ptr->cur_tag, attributes_pptr);
   
}

/*****************************************************************************/
//  Description : end element handler
//  Global resource dependence : none
//  Author: Gaily.Wang
//  Note:
/*****************************************************************************/
void Xml_EndElementHandler(
                        void         *user_data_ptr,
                        const uint16 *localname_ptr
                        )
{		
    CFLWCHAR_StrDump( localname_ptr );	         	    
}

/*****************************************************************************/
//  Description : character date handler
//  Global resource dependence : none
//  Author: Gaily.Wang
//  Note:
/*****************************************************************************/
void Xml_CharacterDataHandler(
                                 void         *user_data_ptr,
                                 const uint16 *ch_ptr 
                                 )
{
    CFLWCHAR_StrDump( ch_ptr );	     
}

/*****************************************************************************/
//  Description : Do parse weather data
//  Global resource dependence : none
//  Author: Gaily.Wang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN Xml_ParseWeatherData(
                            uint8* buf_ptr,                         //[in] Data buffer to paser
                            int32 buf_len,                         //[in] Data length to paser
                            MMIWEATHER_PARSE_TEMP_T *parse_data_ptr //[out] Data get
                            )
{
    BOOLEAN result = FALSE;
    CFLXML_PARSER_T* parser_ptr = PNULL;
	//SCI_TRACE_LOW:"[weather]:Xml_ParseWeatherData---"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWEATHER_PARSE_1691_112_2_18_3_6_42_71,(uint8*)"");
    parser_ptr = CFLXML_CreateParser( Xml_StartElementHandler, 
                                      Xml_EndElementHandler, 
                                      Xml_CharacterDataHandler, 
                                      PARSER_SEP_CHAR, 
                                      (void*)parse_data_ptr);
    
    if(PNULL != parser_ptr )
    {                
        //开始解析       
        if(CFLXML_ERR_OK == CFLXML_Parse( parser_ptr, buf_ptr, buf_len, TRUE))
        {
            //数据解析成功
        	//SCI_TRACE_LOW:"[weather]:Xml_ParseWeatherData---Success---"
        	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWEATHER_PARSE_1704_112_2_18_3_6_43_72,(uint8*)"");
            result = TRUE;
        }

        //结束解析       
        CFLXML_FreeParser( parser_ptr );
    }  

    return result;
}


/*****************************************************************************
// 	Description : Set element's attributes
//	Global resource dependence : 
//  Author:Gaily.Wang
//	Note: 
*****************************************************************************/
LOCAL void Xml_GetElementAttributes(
                                MMIWEATHER_PARSE_TEMP_T *parse_data_ptr,
                                WEATHER_XML_TAG_E       tag, 
                                const uint16            **attributes_pptr
                                )
{
    static uint8 fc_day_count = 0;
    wchar** element_ptr = PNULL;
    uint16 str_len = 0;
	//SCI_TRACE_LOW:"[weather]:Xml_GetElementAttributes---tag_id = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWEATHER_PARSE_1730_112_2_18_3_6_43_73,(uint8*)"d", tag);

    switch(tag)
    {
    case XML_TAG_XML_API_REPLY:
        {
            parse_data_ptr->is_bad_data = FALSE;
            parse_data_ptr->is_support_city = TRUE;
        }
        break;
    case XML_TAG_WEATHER:
        {
            parse_data_ptr->condition_type = WEATHER_CONDITION_TYPE_UNKNOW;
            fc_day_count = 0;
        }
        break;
    case XML_TAG_FORECAST_DATE:
        {
            element_ptr = &parse_data_ptr->temp_info.forecast_date;
        }
        break;
    case XML_TAG_CURRENT_DATE_TIME:
        {
            element_ptr = &parse_data_ptr->temp_info.cur_date_time;
        }
        break;
    case XML_TAG_CURRENT_CONDITIONS:
        {
            parse_data_ptr->condition_type = WEATHER_CONDITION_TYPE_CURRENT;
        }
    	break;
    case XML_TAG_CONDITION:
        {
            if(WEATHER_CONDITION_TYPE_CURRENT == parse_data_ptr->condition_type)
            {
                element_ptr = &parse_data_ptr->temp_info.cur_condition;
            }
            else if(WEATHER_CONDITION_TYPE_FORECAST == parse_data_ptr->condition_type)
            {
                element_ptr = &parse_data_ptr->temp_info.fc_condition[fc_day_count - 1];
            }
        }
    	break;
    case XML_TAG_TEMP_F:
        {
            element_ptr = &parse_data_ptr->temp_info.cur_temp_f;
        }
    	break;
    case XML_TAG_TEMP_C:
        {
            element_ptr = &parse_data_ptr->temp_info.cur_temp_c;
        }
    	break;
    case XML_TAG_HUMIDITY:
        {
            element_ptr = &parse_data_ptr->temp_info.cur_humidity;
        }
    	break;
    case XML_TAG_WIND_CONDITION:
        {
            element_ptr = &parse_data_ptr->temp_info.cur_wind_condition;
        }
    	break;
    case XML_TAG_FORECAST_CONDITIONS:
        {
            parse_data_ptr->condition_type = WEATHER_CONDITION_TYPE_FORECAST;
            
            if(fc_day_count < MMIWEATHER_FORECAST_DAY_NUM)
            {
                fc_day_count++;
            }
        }
    	break;
    case XML_TAG_DAY_OF_WEEK:
        {
            element_ptr = &parse_data_ptr->temp_info.fc_week[fc_day_count - 1];
        }
    	break;
    case XML_TAG_LOW:
        {
            element_ptr = &parse_data_ptr->temp_info.fc_low_temp_f[fc_day_count - 1];
        }
    	break;
    case XML_TAG_HIGH:
        {
            element_ptr = &parse_data_ptr->temp_info.fc_high_temp_f[fc_day_count - 1];
        }
    	break;
            
    case XML_TAG_CITY:
        {
            //Note: 需要根据req类型做不同处理。
            element_ptr = &parse_data_ptr->temp_info.city;
        }
    	break;
    case XML_TAG_PROBLEM_CAUSE:
        {
            element_ptr = &parse_data_ptr->temp_info.problem_cause;
            //parse_data_ptr->is_bad_data = TRUE;
            parse_data_ptr->is_support_city = FALSE;
        }
        break;
    default:
        {
            //unknow or unconcerned tag.
        }
        break;
    }

    if(element_ptr != PNULL)
    {
        if (attributes_pptr != PNULL && *attributes_pptr != PNULL)
        {
            str_len = MMIAPICOM_Wstrlen(attributes_pptr[1]);
            str_len = MIN(str_len, XML_TAG_MAX_LEN);
            *element_ptr = SCI_ALLOC_APPZ((str_len + 1) * sizeof(wchar));
            MMIAPICOM_Wstrncpy(*element_ptr, attributes_pptr[1], str_len);
        }
    }
    
}
#endif
/*****************************************************************************/
//  Description : wstr_atoi
//  Global resource dependence : none
//  Author: Gaily.Wang
//  Note:
/*****************************************************************************/
LOCAL int32 wstr_atoi(wchar* wstr_ptr)
{
    char* str_ptr  = PNULL;
    int32 wstr_len = 0;
    int32 result   = 0;

	int32 test = 0;
	char* test_ptr = " 82 mph";
    
    //wstr to str
    
    if(PNULL != wstr_ptr)
    {
        wstr_len = MMIAPICOM_Wstrlen(wstr_ptr);

        if(wstr_len > 0)
        {
            str_ptr = SCI_ALLOC_APPZ(wstr_len + 1);
            if(PNULL != str_ptr)
            {
                MMIAPICOM_WstrToStr(wstr_ptr, (uint8*)str_ptr);
                result = atoi(str_ptr);
                test = atoi(test_ptr);
                SCI_FREE(str_ptr);
            }

        }
    }

    return result;
}

MMI_IMAGE_ID_T MMIWEATHER_GetConditionImgId(MMIWEATHER_CONDITION_E condition_id, WEATHER_ICON_E type)
{
    MMI_IMAGE_ID_T image_id = IMAGE_WEATHER_CONDITION_SMALL_UNKNOWN;
    int32 i = 0, n = sizeof (s_conditions_map) / sizeof (s_conditions_map[0]);

    for (i = 0; i < n; i ++)
    {
        if (s_conditions_map[i].condition_id == condition_id)
        {
            switch(type)
            {
            case WEATHER_ICON_SMALL:
                image_id = s_conditions_map[i].image_small_id;
                break;

            case WEATHER_ICON_MIDDLE:
                image_id = s_conditions_map[i].image_middle_id;
                break;

            case WEATHER_ICON_BIG:
                image_id = s_conditions_map[i].image_big_id;
                break;
            case WEATHER_ICON_WIDGET:
                image_id = s_conditions_map[i].image_widget_id;
                break;
            default:
                image_id = s_conditions_map[i].image_middle_id;
                break;
            }
            break;
        }
    }
    
    return image_id;
}

MMI_TEXT_ID_T MMIWEATHER_GetConditionTxtId(MMIWEATHER_CONDITION_E condition_id)
{
#ifdef MMI_PDA_SUPPORT
    MMI_TEXT_ID_T text_id = TXT_UNKNOWN;
#else
    MMI_TEXT_ID_T text_id = TXT_UNKNOWN;
#endif

    int32 i = 0, n = sizeof (s_conditions_map) / sizeof (s_conditions_map[0]);

    for (i = 0; i < n; i ++)
    {
        if (s_conditions_map[i].condition_id == condition_id)
        {
            text_id = s_conditions_map[i].text_id;
            break;
        }
    }
    
    return text_id;
}

MMI_TEXT_ID_T MMIWEATHER_GetWeekTxtId(MMIWEATHER_WEEK_E week_id)
{
#ifdef MMI_PDA_SUPPORT
    MMI_TEXT_ID_T text_id = TXT_UNKNOWN;
#else
    MMI_TEXT_ID_T text_id = TXT_UNKNOWN;
#endif
    int32 i = 0, n = sizeof (s_week_map) / sizeof (s_week_map[0]);

    for (i = 0; i < n; i ++)
    {
        if (s_week_map[i].week_id == week_id)
        {
            text_id = s_week_map[i].text_id;
            break;
        }
    }
    
    return text_id;
}

MMI_TEXT_ID_T MMIWEATHER_GetWindDirTxtId(MMIWEATHER_WIND_DIR_E wind_dir_id)
{
#ifdef MMI_PDA_SUPPORT
    MMI_TEXT_ID_T text_id = TXT_UNKNOWN;
#else
    MMI_TEXT_ID_T text_id = TXT_UNKNOWN;
#endif
    int32 i = 0, n = sizeof (s_wind_dir_map) / sizeof (s_wind_dir_map[0]);

    for (i = 0; i < n; i ++)
    {
        if (s_wind_dir_map[i].wind_dir_id == wind_dir_id)
        {
            text_id = s_wind_dir_map[i].text_id;
            break;
        }
    }
    
    return text_id;
}

#ifdef MMI_WEATHER_JSON_PARSE
/*****************************************************************************/
//  Description : init json config
//  Global resource dependence : none
//  Author: juan.wu
//  Note:
/*****************************************************************************/
LOCAL void WEATHER_InitjsonConfig(JSON_config* config_ptr,
                                    JSON_parser_callback callback,
                                    MMIWEATHER_PARSE_TEMP_T *weather_data_ptr)
{
    if(PNULL == config_ptr)
    {
        SCI_TRACE_LOW("WEATHER_InitjsonConfig PNULL == config_ptr");
        return;
    }
    init_JSON_config(config_ptr);

    config_ptr->callback = callback;
    config_ptr->allow_comments = TRUE;
    config_ptr->handle_floats_manually = TRUE;
    config_ptr->callback_ctx = weather_data_ptr;
}
/*****************************************************************************/
//  Description : caculate the total json char num
//  Global resource dependence : none
//  Author: juan.wu
//  Note:
/*****************************************************************************/
LOCAL uint32 CalTotalChar(char* buf_ptr )
{
    uint32 i = 0;

    for(i = 0; ;i++)
    {
        if((*(buf_ptr+i))== '\0')
        {
            break;
        }
    }
    return i;
}
/*****************************************************************************/
//  Description : parse json char one by one
//  Global resource dependence : none
//  Author: juan.wu
//  Note:
/*****************************************************************************/
LOCAL int WEATHER_ParseJsonChar(JSON_parser jp_ptr)
{
    char* buf_ptr = PNULL;
    uint32 offset = 0;
    int  c = -1;
    uint32 total_cnum = 0;    
    int parse_res = 0;

    SCI_TRACE_LOW("WEATHER_ParseJsonChar ENTER");
    buf_ptr = MMIWEATHER_GetWeatherInformation();
    total_cnum = CalTotalChar(buf_ptr);
    
    for(offset = 0; offset < total_cnum; offset++)
    {
        c = (*(buf_ptr+offset))& 0x000000ff;
        parse_res = JSON_parser_char(jp_ptr, c);
        if(0 == parse_res)
        {
            return parse_res;
        }
    }
    return parse_res;
}

/*****************************************************************************/
//  Description : HandleJsonParseResult
//  Global resource dependence : none
//  Author: juan.wu
//  Note:
/*****************************************************************************/
LOCAL void HandleJsonParseResult(MMIWEATHER_PARSE_TEMP_T weather_data)
{
    if(!weather_data.is_support_city)
    {
        if(weather_data.is_retry_en_name)
        {
            MMIWEATHER_SendMsgToApp(MSG_WEATHER_RETRY_EN_NAME_CNF);
        }
        else
        {
            MMIWEATHER_SendMsgToApp(MSG_WEATHER_ERROR_DESCRIPTION_CNF);
        }
    }
    else if(weather_data.country_info.is_need_country_list)
    {
        SCI_TRACE_LOW("HandleJsonParseResult need_country_list");
        if(weather_data.country_info.is_out_of_bounds)
        {
            MMIWEATHER_SendMsgToApp(MSG_WEATHER_TOO_MANY_COUNTRY);
        }
        else
        {
            MMIWEATHER_SendMsgToApp(MSG_WEATHER_NEED_APPEND_COUNTRY_LIST_CNF);
        }
    }
    else if(MMIWEATHER_GetSelCountryFlag())
    {
        SCI_TRACE_LOW("HandleJsonParseResult no weather info");
        MMIWEATHER_SendMsgToApp(MSG_WEATHER_ONLY_COUNTRY_INFO);
    }
    else 
    {
        BOOLEAN result = FALSE;
        
        result = MMIWEATHER_HandleParsedData(&weather_data);

        if(result)
        {
            MMIWEATHER_SendMsgToApp(MSG_WEATHER_PARSE_SUCCESS_CNF);
        }
        else
        {
            MMIWEATHER_SendMsgToApp(MSG_WEATHER_PARSE_FAILED_CNF);
        }
    }
}
/*****************************************************************************/
//  Description : MMIWEATHER_JsonParseStart
//  Global resource dependence : none
//  Author: juan.wu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIWEATHER_JsonParseStart(JSON_parser_callback callback)
{
	JSON_config jc = {0};
	JSON_parser jp_ptr = PNULL;
    MMIWEATHER_PARSE_TEMP_T weather_data = {0};

    SCI_TRACE_LOW("MMIWEATHER_JsonParseStart ENTER");
    weather_data.is_support_city = TRUE; 
#ifdef MMIWEATHER_SUPPORT_SPRD_SRV
    s_is_showed_err_msg = FALSE;
#endif
    WEATHER_InitjsonConfig(&jc,callback,&weather_data);

    jp_ptr = new_JSON_parser(&jc); 
    if(PNULL == jp_ptr)
    {
        SCI_TRACE_LOW("MMIWEATHER_JsonParseStart PNULL == jp_ptr");
        return FALSE;
    }
    if(0 == WEATHER_ParseJsonChar(jp_ptr))
    {
        weather_data.is_bad_data = TRUE;
        SCI_TRACE_LOW("parse error!");
    }
    delete_JSON_parser(jp_ptr);  

    HandleJsonParseResult(weather_data);
    return TRUE;
    
}
/*****************************************************************************/
//  Description : set weather info flag by json_tag
//  Global resource dependence : none
//  Author: juan.wu
//  Note:
/*****************************************************************************/
LOCAL void SetWeatherFlag(const JSON_value* j_value,
                            WEATHER_JSON_TAG_E j_tag,
                            MMIWEATHER_PARSE_TEMP_T *weather_data_ptr)
{
    if(PNULL == weather_data_ptr)
    {
        SCI_TRACE_LOW("SetWeatherFlag PNULL == weather_data_ptr");
        return;
    }
    switch(j_tag)    
    {
    case JSON_TAG_COUNTRY_NAME:
        SCI_TRACE_LOW("JSON_TAG_COUNTRY_NAME is_out_of_bounds = %d,is_sel = %d",\
                            MMIWEATHER_GetSelCountryFlag(),\
                            weather_data_ptr->country_info.is_out_of_bounds);
        if(!MMIWEATHER_GetSelCountryFlag()
            && !weather_data_ptr->country_info.is_out_of_bounds)
        {
            weather_data_ptr->is_need_add_string = TRUE;
            weather_data_ptr->country_info.is_need_country_list = TRUE;

            weather_data_ptr->country_info.country_num++;            
        }
        break;
        
    case JSON_TAG_ERROR_DESCRIPTION:
    case JSON_TAG_CURRENT_TEMP_F:
    case JSON_TAG_CURRENT_TEMP_C:
    case JSON_TAG_CURRENT_HUMIDITY:
    case JSON_TAG_CURRENT_CITY_DISPLAY:
    case JSON_TAG_FORECAST_TEMP_F:
	case JSON_TAG_FORECAST_CONDITION:
    case JSON_TAG_FORECAST_DATE_TIME:
    case JSON_TAG_CURRENT_WIND_STRING:
    case JSON_TAG_LOCAL_DATE_TIME:
        weather_data_ptr->is_need_add_string = TRUE;
        break;

    case JSON_TAG_CURRENT_WEATHER:
        SCI_TRACE_LOW("wujuan3 s_has_sel = %d",MMIWEATHER_GetSelCountryFlag());
        if(MMIWEATHER_GetSelCountryFlag())
        {
            MMIWEATHER_SetSelCountryFlag(FALSE);
        }
        weather_data_ptr->is_need_add_string = TRUE;
        MMIAPIWEATHER_SetTryEnNameFlag(FALSE);
        break;
            

    case JSON_TAG_COUNTRY_NUM_RESULTS:
        SCI_TRACE_LOW("JSON_TAG_COUNTRY_NUM_RESULTS s_has_sel = %d",MMIWEATHER_GetSelCountryFlag());
        if(!MMIWEATHER_GetSelCountryFlag())
        {
            weather_data_ptr->is_need_add_string = TRUE;
        }
        MMIAPIWEATHER_SetTryEnNameFlag(FALSE);
        break;

    case JSON_TAG_FORECAST_WEEK_DAY:
        weather_data_ptr->is_need_add_string = TRUE;
        if(MMIWEATHER_FORECAST_DAY_NUM <= weather_data_ptr->fc_index)
        {
            weather_data_ptr->fc_index = 0;
        }
        weather_data_ptr->fc_index++;
        break;

    case JSON_TAG_FORECAST_TEMP_HIGH:
        weather_data_ptr->is_temp_high = TRUE;
        weather_data_ptr->is_need_add_string = TRUE;
        break;

    case JSON_TAG_FORECAST_TEMP_LOW:
        weather_data_ptr->is_temp_high = FALSE;
        weather_data_ptr->is_need_add_string = TRUE;
        break;

#ifdef MMIWEATHER_SUPPORT_SPRD_SRV
    case JSON_TAG_AUTH_ERROR:
        if(MMIWEATHER_GetSelCountryFlag())
        {
            MMIWEATHER_SetSelCountryFlag(FALSE);
        }
        break;
#endif

    default:
        break;
    }
}
/*****************************************************************************/
//  Description : get json tag 
//  Global resource dependence : none
//  Author: juan.wu
//  Note:
/*****************************************************************************/
PUBLIC void MMIWEATHER_GetJsonTAG(void * user_data,const JSON_value* j_value)
{
    WEATHER_JSON_TAG_E j_tag = JSON_TAG_MAX;
    uint16 len = 0;
    wchar *wchar_temp = PNULL;
    MMIWEATHER_PARSE_TEMP_T * weather_data_ptr = (MMIWEATHER_PARSE_TEMP_T *)user_data;

    if(PNULL == j_value)
    {
        SCI_TRACE_LOW("MMIWEATHER_GetJsonTAG PNULL == j_value");
        return;
    }

    while(--j_tag)
    {
		len = strlen(s_json_tag_arr[j_tag]);
		len = MAX(len,((((*j_value).vu).str).length+1));
        if(PNULL != wchar_temp)
        {
            SCI_FREE(wchar_temp);
            wchar_temp = PNULL;
        }
        wchar_temp = SCI_ALLOC_APPZ(len * sizeof(wchar));
        if(PNULL == wchar_temp)
        {
            return;
        }
        MMIAPICOM_StrToWstr((((*j_value).vu).str).value, wchar_temp); /*lint !e64*/   
		if( 0 == MMIAPICOM_WstrStrncmp((const uint16*)wchar_temp, 
										(const uint8*)s_json_tag_arr[j_tag], 
										len))
    
		{
            weather_data_ptr->cur_tag = j_tag;
			break;
		}
	}
	SCI_FREE(wchar_temp);
	wchar_temp = PNULL;
    SetWeatherFlag(j_value,j_tag,weather_data_ptr);
}


#ifdef MMIWEATHER_SUPPORT_SPRD_SRV
/*****************************************************************************/
//  Description : GetWeatherErrorType
//  Global resource dependence : none
//  Author: juan.wu
//  Note: get weather auth error type
/*****************************************************************************/
LOCAL void GetWeatherErrorType(
                                MMIWEATHER_PARSE_TEMP_T  * weather_data_ptr ,
                                uint32 error_code
                                )
{
    switch(error_code)
    {
        case 40201:
            weather_data_ptr->error_type = WEATHER_AUTH_ERROR_KEY_NOT_EXSIT;
            break;

        case 40202:
            weather_data_ptr->error_type = WEATHER_AUTH_ERROR_SIGN_ERROR;
            break;

        case 40203:
            weather_data_ptr->error_type = WEATHER_AUTH_ERROR_AUTH_LEVEL_LESS_THAN;
            break;

        case 40204:
            weather_data_ptr->error_type = WEATHER_AUTH_TIME_PARAM_ERROR;
            break;

        default:
            SCI_TRACE_LOW("GetWeatherErrorType error_code = %d",error_code);
            weather_data_ptr->error_type = WEATHER_AUTH_ERROR_TYPE_MAX;
            break;
    }
    SCI_TRACE_LOW("GetWeatherErrorType error_type = %d",weather_data_ptr->error_type);
}

PUBLIC BOOLEAN MMIWEATHER_IsErrorMsgShowed(void)
{
    return s_is_showed_err_msg;
}
#endif
/*****************************************************************************/
//  Description : get json int data 
//  Global resource dependence : none
//  Author: juan.wu
//  Note:
/*****************************************************************************/
PUBLIC void MMIWEATHER_GetJsonNumber(void * user_data,const JSON_value* j_value)
{
    MMIWEATHER_PARSE_TEMP_T * weather_data_ptr = (MMIWEATHER_PARSE_TEMP_T *)user_data;

    if(PNULL == j_value)
    {
        SCI_TRACE_LOW("MMIWEATHER_GetJsonNumber PNULL == j_value");
        return;
    }
    
    switch(weather_data_ptr->cur_tag)
    {
        case JSON_TAG_CURRENT_TEMP_F:
            weather_data_ptr->temp_info.cur_temp_f = j_value->vu.integer_value;/*lint !e712*/
            SCI_TRACE_LOW("MMIWEATHER_GetJsonNumber cur_temp_f = %d",weather_data_ptr->temp_info.cur_temp_f);
            break;

        case JSON_TAG_CURRENT_TEMP_C:
            weather_data_ptr->temp_info.cur_temp_c = j_value->vu.integer_value;/*lint !e712*/
            SCI_TRACE_LOW("MMIWEATHER_GetJsonNumber cur_temp_c = %d",weather_data_ptr->temp_info.cur_temp_c);
            break;

        case JSON_TAG_WEATHER_ESTIMATE:
            weather_data_ptr->is_estimated_info = j_value->vu.integer_value;
            SCI_TRACE_LOW("MMIWEATHER_GetJsonNumber is_estimated_info = %d",weather_data_ptr->is_estimated_info);
            break;

#ifdef MMIWEATHER_SUPPORT_SPRD_SRV
        case JSON_TAG_AUTH_ERROR_TYPE:
            GetWeatherErrorType(weather_data_ptr,(uint32)j_value->vu.integer_value);
            break;
#endif
        default:
            break;

    }
}
/*****************************************************************************/
//  Description : fill in forecast data 
//  Global resource dependence : none
//  Author: juan.wu
//  Note:
/*****************************************************************************/
LOCAL wchar** FillInFCData(MMIWEATHER_PARSE_TEMP_T * weather_data_ptr,uint8 index)
{
    wchar** element_pptr = PNULL;
    
    switch(weather_data_ptr->cur_tag)
    {
    case JSON_TAG_FORECAST_CONDITION:
        element_pptr = &weather_data_ptr->temp_info.fc_condition[index];
        break;
        
    case JSON_TAG_FORECAST_TEMP_F:
        if(weather_data_ptr->is_temp_high)
        {
            element_pptr = &weather_data_ptr->temp_info.fc_high_temp_f[index];
        }
        else
        {
            element_pptr = &weather_data_ptr->temp_info.fc_low_temp_f[index];
        }
        break;
        
    case JSON_TAG_FORECAST_WEEK_DAY:
        element_pptr = &weather_data_ptr->temp_info.fc_week[index];
        break;

    default:

        break;
        
    }
    return element_pptr;
}
/*****************************************************************************/
//  Description : change float string to int str
//  Global resource dependence : none
//  Author: juan.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN  ChangeFloatStringToIntStr(wchar *str_ptr)
{
    uint16 str_len = 0;
    uint8  i = 0;
    uint8  j = 0;
    wchar  temp_str[10] = {0};
    BOOLEAN result = FALSE;

    if(PNULL == str_ptr)
    {
        SCI_TRACE_LOW("ChangeFloatStringToIntStr PNULL == str_ptr");
        return FALSE;
    }
    str_len = MMIAPICOM_Wstrlen(str_ptr);
    for(i = 0; i < str_len; i++)
    {
        if(L'.' == str_ptr[i])
        {
            result = TRUE;
            continue;
        }
        temp_str[j] = str_ptr[i];
        j++;
    }
    SCI_MEMSET(str_ptr,0,str_len);
    MMIAPICOM_Wstrcpy(str_ptr, temp_str);
    return result;
}
/*****************************************************************************/
//  Description : set temp info
//  Global resource dependence : none
//  Author: juan.wu
//  Note:
/*****************************************************************************/
LOCAL void SetTempInfo(int32 tmp,MMIWEATHER_PARSE_TEMP_T * weather_data_ptr)
{
    int *p = PNULL;

    if(PNULL == weather_data_ptr)
    {
        SCI_TRACE_LOW("SetTempInfo param error!");
        return;
    }
    
    switch(weather_data_ptr->cur_tag)
    {
        case JSON_TAG_CURRENT_TEMP_F:
            p = &weather_data_ptr->temp_info.cur_temp_f;/*lint !e64*/
            break;

        case JSON_TAG_CURRENT_TEMP_C:
            p = &weather_data_ptr->temp_info.cur_temp_c;/*lint !e64*/
            break;

        default:
            break;
    }

    if(PNULL != p)
    {
        if(tmp%10 >= 5)
        {
            *p = tmp/10 +1;
        }
        else
        {
            *p = tmp/10;
        }    
    }
}
/*****************************************************************************/
//  Description : check if have same country name
//  Global resource dependence : none
//  Author: juan.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CheckIsHaveSameCountryName(MMIWEATHER_COUNTRY_INFO_T *country_info_ptr,
                                            wchar * src_str_ptr,
                                            uint16 index)
{
    uint8 i = 0;
    BOOLEAN check_res = FALSE;

    if(PNULL == country_info_ptr)
    {
        SCI_TRACE_LOW("CheckIsHaveSameCountryName PNULL == country_info_ptr");
        return FALSE;
    }
    for(i = 0; i < index; i++)
    {
        if(0 == MMIAPICOM_Wstrncmp(country_info_ptr->name[i], 
                                    src_str_ptr, 
                                    country_info_ptr->name_len[i]))
        {
            check_res = TRUE;
            break;
        }
    }
    SCI_TRACE_LOW("CheckIsHaveSameCountryName check_res = %d",check_res);
    return check_res;
}
/*****************************************************************************/
//  Description : fill in country info
//  Global resource dependence : none
//  Author: juan.wu
//  Note:
/*****************************************************************************/
LOCAL void FillInCountryInfo(MMIWEATHER_COUNTRY_INFO_T *country_info_ptr,
                                uint16 name_len,
                                wchar * name,
                                uint16 index)
{
    if(PNULL == country_info_ptr)
    {
        SCI_TRACE_LOW("FillInCountryInfo PNULL == country_info_ptr");
        return;
    }

    country_info_ptr->name_len[index] = name_len;
    if(PNULL != country_info_ptr->name[index])
    {
        SCI_FREE(country_info_ptr->name[index]);
        country_info_ptr->name[index] = PNULL;
    }
    country_info_ptr->name[index] =  SCI_ALLOC_APPZ((name_len+1)*sizeof(wchar));
    MMIAPICOM_Wstrncpy(country_info_ptr->name[index], 
                    name, 
                    name_len);
        
}
/*****************************************************************************/
//  Description : Deal With Description String
//  Global resource dependence : none
//  Author: juan.wu
//  Note:
/*****************************************************************************/
LOCAL void DealWithDescriptionString(MMIWEATHER_PARSE_TEMP_T * weather_data_ptr,
                                    wchar * temp_str_ptr,
                                    uint16  str_len)
{
    uint8   city_num = 0;
    MMIWEATHER_ERROR_INFO_T  *weather_error_ptr = MMIWEATHER_GetErrorInfo();
    MMIWEATHER_WEATHER_T* weather_info_ptr = PNULL;
    
    city_num = MMIWEATHER_GetCityNum(); 
    
    weather_info_ptr = MMIWEATHER_GetCityWeatherInfo(city_num);
    if(!weather_data_ptr->is_estimated_info)
    {
        weather_data_ptr->is_support_city = FALSE;
    }
    SCI_MEMSET(weather_error_ptr, 0, sizeof(MMIWEATHER_ERROR_INFO_T)); 
    str_len = MIN(str_len,WEATHER_ERROR_WARNING_MAX_LEN);
    weather_error_ptr->error_des_len = str_len;
    MMIAPICOM_Wstrncpy(weather_error_ptr->error_description, 
                        temp_str_ptr, 
                        str_len);
    //only chinese name need to retry alphabetic name,use the first str to judge
    if(MMI_CHINESE_FIRST <= weather_info_ptr->city_name[0] 
        && weather_info_ptr->city_name[0] <= MMI_CHINESE_LAST)
    {
        if(FALSE == MMIAPIWEATHER_GetHasTryEnName())
        {
            if(0 == MMIAPICOM_Wstrncmp(weather_error_ptr->error_description, 
                                        WEATHER_NO_INFO_QUERY_STR, 
                                        MMIAPICOM_Wstrlen(WEATHER_NO_INFO_QUERY_STR)))
            {
                SCI_MEMSET(weather_error_ptr,0,sizeof(MMIWEATHER_ERROR_INFO_T));
                weather_data_ptr->is_retry_en_name = TRUE;
            }
        }
        else
        {
            MMIAPIWEATHER_SetTryEnNameFlag(FALSE);
        }
    }
}
/*****************************************************************************/
//  Description : get json string 
//  Global resource dependence : none
//  Author: juan.wu
//  Note:
/*****************************************************************************/
PUBLIC void MMIWEATHER_GetJsonString(void * user_data,const JSON_value* j_value)
{
    MMIWEATHER_PARSE_TEMP_T * weather_data_ptr = (MMIWEATHER_PARSE_TEMP_T *)user_data;
    uint16                   str_len = 0;
    wchar *                  temp_str = PNULL;
    wchar** element_pptr = PNULL;

    if(PNULL == j_value || PNULL == weather_data_ptr)
    {
        SCI_TRACE_LOW("MMIWEATHER_GetJsonString param error!");
        return;
    }
    SCI_TRACE_LOW("---weather: MMIWEATHER_GetJsonString = %s---",(((*j_value).vu).str).value);

    if(weather_data_ptr->is_need_add_string)
    {
        SCI_TRACE_LOW("---weather: need add  = %s---",(((*j_value).vu).str).value);
        str_len = MIN(WEATHER_STRING_MAX_LEN,(((*j_value).vu).str).length);
        temp_str = SCI_ALLOC_APPZ(sizeof(wchar)*((*j_value).vu.str.length + 1));
        
        MMIAPICOM_StrToWstr((((*j_value).vu).str).value, temp_str);/*lint !e64*/
        switch(weather_data_ptr->cur_tag)
        {
        case JSON_TAG_ERROR_DESCRIPTION:
            {
                DealWithDescriptionString(weather_data_ptr,temp_str,str_len);
                weather_data_ptr->is_need_add_string = FALSE;
            }
            break;

        case JSON_TAG_COUNTRY_NAME:
            {
                uint16 list_index = 0;
                BOOLEAN check_res = FALSE;
                MMIWEATHER_COUNTRY_INFO_T * country_info_ptr = PNULL;
                
                country_info_ptr = MMIWEATHER_GetCountryInfo();
                country_info_ptr->country_num = weather_data_ptr->country_info.country_num;
                str_len = MIN(MMIWEATHER_MAX_COUNTRY_NAME_LEN,str_len);
                SCI_TRACE_LOW("weather: country_num = %d,is_out_of_bounds = %d",country_info_ptr->country_num,weather_data_ptr->country_info.is_out_of_bounds);
                if(country_info_ptr->country_num >= 1
                    && !weather_data_ptr->country_info.is_out_of_bounds)
                {
                    list_index = country_info_ptr->country_num-1;
                    SCI_TRACE_LOW("weather:country name = %s---",(((*j_value).vu).str).value);
                    if(list_index < MMIWEATHER_MAX_COUNTRY_NUM)
                    {
                        check_res = CheckIsHaveSameCountryName(country_info_ptr,temp_str,list_index);
                        if(!check_res)
                        {
                            FillInCountryInfo(country_info_ptr,
                                                str_len,
                                                temp_str,
                                                list_index);
                        }
                        else
                        {
                            weather_data_ptr->country_info.country_num--;
                            country_info_ptr->country_num--;
                        }
                    }
                    else
                    {
                        weather_data_ptr->country_info.is_out_of_bounds = TRUE;
                        SCI_TRACE_LOW("MMIWEATHER_GetJsonString list_index = %d",list_index);
                    }
                    weather_data_ptr->is_need_add_string = FALSE;                    
                }
                else
                {
                    SCI_TRACE_LOW("weather:country num out of bounds");
                }
             }
            break;

         case JSON_TAG_CURRENT_WEATHER:
            element_pptr = &weather_data_ptr->temp_info.cur_condition;
            weather_data_ptr->is_need_add_string = FALSE;
            break;

        case JSON_TAG_CURRENT_CITY:
            element_pptr = &weather_data_ptr->temp_info.city;
            weather_data_ptr->is_need_add_string = FALSE;
            break;
        
        case JSON_TAG_CURRENT_HUMIDITY:
            element_pptr = &weather_data_ptr->temp_info.cur_humidity;
            weather_data_ptr->is_need_add_string = FALSE;
            break;


        case JSON_TAG_FORECAST_DATE_TIME:
            if(!weather_data_ptr->is_estimated_info)
            {
                element_pptr = &weather_data_ptr->temp_info.forecast_date;
                weather_data_ptr->is_need_add_string = FALSE;
            }
            break;

        case JSON_TAG_LOCAL_DATE_TIME:
            if(weather_data_ptr->is_estimated_info)
            {
                element_pptr = &weather_data_ptr->temp_info.forecast_date;
                weather_data_ptr->is_need_add_string = FALSE;
            }
            break;

        case JSON_TAG_FORECAST_CONDITION:
        case JSON_TAG_FORECAST_TEMP_F:
        case JSON_TAG_FORECAST_WEEK_DAY:
            if(weather_data_ptr->fc_index >= 1 
                && weather_data_ptr->fc_index <= MMIWEATHER_FORECAST_DAY_NUM)
            {
                element_pptr = FillInFCData(weather_data_ptr,weather_data_ptr->fc_index - 1);                
            }
            weather_data_ptr->is_need_add_string = FALSE;
            break;

        case JSON_TAG_CURRENT_WIND_STRING:
            element_pptr = &weather_data_ptr->temp_info.cur_wind_condition;
            weather_data_ptr->is_need_add_string = FALSE;
            break;

        case JSON_TAG_CURRENT_TEMP_F:
        case JSON_TAG_CURRENT_TEMP_C:
            {
                int tmp = 0;
                
                ChangeFloatStringToIntStr(temp_str);

                tmp = wstr_atoi(temp_str);

                SetTempInfo(tmp,weather_data_ptr);
            }
            break;
                
        default:
            break;
        }

    }
    if(element_pptr != PNULL)
    {
        *element_pptr = SCI_ALLOC_APPZ((str_len + 1) * sizeof(wchar));
        MMIAPICOM_Wstrncpy(*element_pptr, temp_str, str_len);
    }
    if(PNULL != temp_str)
    {
        SCI_FREE(temp_str);
        temp_str = PNULL;
    }
}
#endif
