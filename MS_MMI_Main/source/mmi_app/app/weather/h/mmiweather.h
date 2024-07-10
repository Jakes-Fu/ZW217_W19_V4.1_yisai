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
//�����¶�ת��Ϊ�����¶ȡ�(����ʮ����֤����)ע�⣬�ú겻��ֱ��ʹ�ã���Ҫ���MMIWEATHER_GetTemperatureһ��ʹ��
#define TEMP_F2C(F) ((F - 32) * 5 * 10/ 9)  
#define TEMP_C2F(C) ((C * 9 / 5 + 32))  //�����¶�ת��Ϊ�����¶ȡ�

#define MMIWEATHER_TEMP_UNKNOW  (-273)  //�¶�δ֪
#define MMIWEATHER_WIND_UNKNOW  (-1)     //��������δ֪

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

#define MMIWEATHER_FORECAST_DAY_NUM     4       //�ṩ���������Ԥ����Ϣ��
#define MMIWEATHER_FORWARD_DAY_NUM      2       //ת�����������Ԥ����Ϣ����ֵ����С�� MMIWEATHER_FORECAST_DAY_NUM��

#define MMIWEATHER_MAX_CITY_NAME_LEN    64      //Max city name str length.
#define MMIWEATHER_MAX_CITY_CODE_LEN    MMIWEATHER_MAX_CITY_NAME_LEN       //Max city code str length.
#define MMIWEATHER_MAX_COUNTRY_CODE_LEN 3        //Max iso country code length.

#define MMIWEATHER_DEFAULT_UPDATE_FREQ  24        //Ĭ�ϵĶ�ʱ����Ƶ��


#define MMIWEATHER_MAX_FILE_NAME_LEN    128     //Max city number supported.

#define MMIWEATHER_PUB_TIME_STR_LEN     8       //Max city number supported.
#define MMIWEATHER_PUB_DATE_STR_LEN     12      //Max city number supported.
#define MMIWEATHER_TEMP_RANGE_STR_LEN   10      //Max city number supported.

#define MMIWEATHER_UPDATE_TIME_BEGIN_NOLIMIT    4     // 4�㱻��Ϊ���Զ�����ʱ��û�����ƣ��Ա�5������dropdownlist��һλ
#define MMIWEATHER_UPDATE_TIME_END_NOLIMIT      18    // 18�㱻��Ϊ���Զ�����ʱ��û�����ƣ��Ա�19������dropdownlist��һλ

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

//�¶ȵ�λ
typedef enum
{
    WEATHER_TEMP_UNIT_C = 0,    //�����¶�
    WEATHER_TEMP_UNIT_F,        //�����¶�
    WEATHER_TEMP_UNIT_MAX
} WEATHER_TEMP_UNIT_E;


//Weather conditions
typedef enum 
{
    WEATHER_CONDITION_UNKNOW = 0,
    WEATHER_CONDITION_CLEAR,             //��
    WEATHER_CONDITION_FOG,               //��
    WEATHER_CONDITION_HAZE,              //��
    WEATHER_CONDITION_CLOUDY,            //��
    WEATHER_CONDITION_PARTLY_CLOUDY,     //����
    WEATHER_CONDITION_MOSTLY_CLOUDY,     //����
    WEATHER_CONDITION_OVERCAST,          //��
    WEATHER_CONDITION_RAIN,              //����	
    WEATHER_CONDITION_LIGHT_RAIN,        //С��	
    WEATHER_CONDITION_RAIN_SHOWERS,      //����
    WEATHER_CONDITION_SHOWERS,           //����	       
    WEATHER_CONDITION_SCATTERED_SHOWERS, //��������	
    WEATHER_CONDITION_THUNDERSTORM,      //������	
    WEATHER_CONDITION_CHANCE_OF_SHOWERS, //ʱ������	
    WEATHER_CONDITION_CHANCE_OF_SNOW,    //ʱ��ѩ	
    WEATHER_CONDITION_CHANCE_OF_STORM,   //ʱ�б���	
    WEATHER_CONDITION_SUNNY,             //��
    WEATHER_CONDITION_MOSTLY_SUNNY,      //������	
    WEATHER_CONDITION_PARTLY_SUNNY,      //���Ƽ���
    WEATHER_CONDITION_MIST,              //����
    WEATHER_CONDITION_SMOKE,             //����
    WEATHER_CONDITION_STORM,             //����
    WEATHER_CONDITION_FLURRIES,          //Сѩ
    WEATHER_CONDITION_CHANCE_OF_RAIN,    //ʱ����	
    WEATHER_CONDITION_CHANCE_OF_TSTORM,  //ʱ���ױ�	
    WEATHER_CONDITION_SLEET,             //���ѩ
    WEATHER_CONDITION_SNOW,              //ѩ
    WEATHER_CONDITION_ICY,               //����	
    WEATHER_CONDITION_DUST,              //ɳ��	
    WEATHER_CONDITION_SCATTERED_THUNDERSTORMS,//����������	
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
    WIND_DIR_N,         //��	0��
    WIND_DIR_NNE,       //����ƫ��	22.5��
    WIND_DIR_NE,        //����	45��
    WIND_DIR_ENE,       //����ƫ��	67.5��
    WIND_DIR_E,         //��	90��
    WIND_DIR_ESE,       //����ƫ��	112.5��
    WIND_DIR_SE,        //����	135��
    WIND_DIR_SSE,       //����ƫ��	157.5��
    WIND_DIR_S,         //��	180��
    WIND_DIR_SSW,       //����ƫ��	202.5��
    WIND_DIR_SW,        //����	225��
    WIND_DIR_WSW,       //����ƫ��	247.5��
    WIND_DIR_W,         //��	270��
    WIND_DIR_WNW,       //����ƫ��	292.5��
    WIND_DIR_NW,        //����	315��
    WIND_DIR_NNW,       //����ƫ��	337.5��
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
    MMIWEATHER_UPDATE_FROM_MAIN_WIN,    //����Ԥ�������ڸ���
    MMIWEATHER_UPDATE_FROM_WIDGET,      //����Ԥ��widget����
    MMIWEATHER_UPDATE_FROM_AUTO,        //��ʱ�Զ�����
    MMIWEATHER_UPDATE_FROM_ADD_CITY,    //��ӳ���
    MMIWEATHER_UPDATE_FROM_MAX
}MMIWEATHER_UPDATE_FROM_E;

//��ӳ���״̬
typedef enum 
{
    MMIWEATHER_ADD_CITY_STATE_OK = 0,         //��ӳɹ�
    MMIWEATHER_ADD_CITY_STATE_ADDING,     //�������
    MMIWEATHER_ADD_CITY_STATE_CONN_FAILED,    //����ʧ��
    MMIWEATHER_ADD_CITY_STATE_UNSUPPORT,      //��֧��
    MMIWEATHER_ADD_CITY_STATE_EXIST,          //�Ѵ���
    MMIWEATHER_ADD_CITY_STATE_FULL,           //�Ѵ������
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
    MMIWEATHER_ANIM_MOVE_DIRECTION_T move_direction;            //�Ƿ���Ҫ�ƶ�
    uint16 pic_cnt;                                             //ͼƬ����
    uint16 cur_pic_idx;                                         //��ǰͼƬ���
    GUI_POINT_T cur_layer_point;                                //ͼ��λ��
    GUI_LCD_DEV_INFO dev_info;                                  //��̬��Чͼ��
    MMI_IMAGE_ID_T pic_info[MMIWEATHER_MAX_ANIM_PIC_NUM];//ͼƬ��Ϣ
}MMIWEATHE_ANIM_LAYER_T;

//Weather anim information
typedef struct _MMIWEATHER_WEATHER_ANIM
{
    BOOLEAN is_forbid;          //�Ƿ����ζ�̬��Ч
    BOOLEAN is_need_anim;       //�Ƿ���ʾ��̬��Ч
    uint8 anim_timer_id;        //��̬��Чtimer_id
    uint16 layer_cnt;           //��ǰͼ������
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
    MMIWEATHER_CONDITION_E  condition;                        //4//��ǰ����״��
    int32                   temp_c;                           //4//��ǰ�¶�(��)
    int32                   temp_f;                           //4//��ǰ�¶�(�H)
    wchar                   humidity[MAX_HUMIDITY_STR_LEN + 1];   //4//��ǰʪ��(%)
    MMIWEATHER_WIND_DIR_E   wind_dir;                         //4//����
    int32                   wind_force;                       //4//����(MPH)
    
} MMIWEATHER_CUR_CONDITIONS_T;

//Forecast Condition information.
typedef struct _MMIWEATHER_FC_CONDITIONS
{
    MMIWEATHER_WEEK_E           week1;         //����
    wchar                       date1[MAX_DATE_STR_LEN];          //����
    MMIWEATHER_CONDITION_E      weather1;      //����״��
    int32                       low1;          //����¶�(���϶�)
    int32                       high1;         //����¶�(���϶�)
    MMIWEATHER_WEEK_E           week2;         //����
    wchar                       date2[MAX_DATE_STR_LEN];          //����
    MMIWEATHER_CONDITION_E      weather2;      //����״��
    int32                       low2;          //����¶�(���϶�)
    int32                       high2;         //����¶�(���϶�)
    MMIWEATHER_WEEK_E           week3;         //����
    wchar                       date3[MAX_DATE_STR_LEN];          //����
    MMIWEATHER_CONDITION_E      weather3;      //����״��
    int32                       low3;          //����¶�(���϶�)
    int32                       high3;         //����¶�(���϶�)
    MMIWEATHER_WEEK_E           week4;         //����
    wchar                       date4[MAX_DATE_STR_LEN];          //����
    MMIWEATHER_CONDITION_E      weather4;      //����״��
    int32                       low4;          //����¶�(���϶�)
    int32                       high4;         //����¶�(���϶�)
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
    const wchar*   city_name;  //�û������,������ʾ�ĳ�����
    const char*    city_code;   //���������������ʱʹ�õĳ�������
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
//  Description : �ж��Ƿ�������ӹ��ó���
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
// 	Description : ���������Ϣ
//	Global resource dependence : 
//  Author: Glen Li
//	Note:
/*****************************************************************************/
PUBLIC char* MMIWEATHER_GetWeatherInformation(void);
/*****************************************************************************/
// 	Description : ������ϢBUFFER ����
//	Global resource dependence : 
//  Author: Glen Li
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIWEATHER_WeatherInfoAlloc(void);
/*****************************************************************************/
// 	Description : ������ϢBUFFER �ͷ�
//	Global resource dependence : 
//  Author: Glen Li
//	Note:
/*****************************************************************************/
PUBLIC void MMIWEATHER_WeatherInfoRelease(void);

/*****************************************************************************/
// 	Description : MMIWEATHER_GetTemperature
//	Global resource dependence : 
//  Author: juan.wu
//	Note:�������¶���������
/*****************************************************************************/
PUBLIC int32 MMIWEATHER_GetTemperature(int32 temp);

/*****************************************************************************/
//  Description : ���Ϊ�������ó�����������ת����ĳ����������򷵻ؿա�
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

