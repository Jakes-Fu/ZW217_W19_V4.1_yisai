/*****************************************************************************
** File Name:      mmiwidget_weather_se.c                                        *
** Author:                                                                   *
** Date:           06/26/2012                                                 *
** Copyright:      2004 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe widget weather special effect interface                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE                  NAME             DESCRIPTION                        *
** 06/26/2012        ying.xu              Creat
******************************************************************************/
#include "mmiwidget_special_effect.h"
//图片结构体
typedef enum
{
    WEATHER_STATE_NONE = 0,
    WEATHER_STATE_SUNNY,  
    WEATHER_STATE_CLOUDY,  
    WEATHER_STATE_RAINNY,  
    WEATHER_STATE_NA,  
    WEATHER_STATE_SUNCHANGE,
    WEATHER_STATE_FOGGY,
    WEATHER_STATE_LIGHTING,
    WEATHER_STATE_SNOWY,
    WEATHER_STATE_SNOWYANDRAINY,
	WEATHER_STATE_MAX
}WEATHER_STATUS_E;

//天气类型枚举
typedef struct Weather_Resoursce_Struct_t{
BMP_OBJ   sun;
BMP_OBJ   sun_bg;
BMP_OBJ   big_temperature;
BMP_OBJ   s_up_temperature;
BMP_OBJ   s_down_temperature;
BMP_OBJ   glow;
BMP_OBJ   city_change;
BMP_OBJ   weather_change;
BMP_OBJ   first_day;
BMP_OBJ   second_day;
BMP_OBJ   third_day;
BMP_OBJ   bg;
BMP_OBJ   divide_bar;
BMP_OBJ   cloud_one;
BMP_OBJ   cloud_two;
BMP_OBJ   cloud_left;
BMP_OBJ   cloud_right;
BMP_OBJ   na;
BMP_OBJ   rain;
BMP_OBJ   fog;
BMP_OBJ   light_one;
BMP_OBJ   light_two;
BMP_OBJ   snow_big;
BMP_OBJ   snow_small;
}WEATHER_RESOURCE_STRUCT_T;

#define WEATHER_FRAME_ANAMI_IN  (65*2)
#define WEATHER_FRAME_ANAMI_TIMES  (WEATHER_FRAME_ANAMI_IN/(65))

//资源对象
typedef struct Weather_Fouction_Param {
	IMGREF_SIZE_T out_size;           //输出buffer宽高
	void * out_buffer;								//输出buffer
	int32 cur_frame;									//当前帧
	int32 total_frame;								//总帧数
	WEATHER_STATUS_E  cur_weather;		//天气状态
	FIX16_POINT_T * Point_Add_ptr;		//算法要用到的一块内存 大小为 2048×sizeof(FIX16_POINT_T)	
    WEATHER_RESOURCE_STRUCT_T  *Weather_R_Instance;
}WEATHER_F_PARAM;


void WeatherForcast_Schedule(WEATHER_F_PARAM * in_param_data_t);
PUBLIC void SetImgColor(uint32 * InColor,IMGREF_SIZE_T InSize);

PUBLIC int32 MMIWIDGET_WEATHER_GetSEFrame(WEATHER_STATUS_E weather_state);

PUBLIC void WeatherForcast_Disappear(WEATHER_F_PARAM * in_param_data_t);


