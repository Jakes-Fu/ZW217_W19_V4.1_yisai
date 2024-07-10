/*****************************************************************************
** File Name:      mmiweather_funcs.h                                        *
** Author:         Gaily.Wang                                                *
** Date:           2011/04/18                                                *
** Copyright:      2011 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file define the internal APIs for weather app        *
*****************************************************************************/
#ifndef MMIWEATHER_FUNCS_H_
#define MMIWEATHER_FUNCS_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmiweather.h"
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/


/*****************************************************************************/
//  Description : Get the index of current city.
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note: 
/*****************************************************************************/
PUBLIC uint8 MMIWEATHER_GetCurCityIndex(void);


/*****************************************************************************/
//  Description : Set the index of current city.
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWEATHER_SetCurCityIndex(uint8 index);

/*****************************************************************************/
//  Description : Get the name of current city.
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note: 
/*****************************************************************************/
PUBLIC wchar* MMIWEATHER_GetCurCityName(void);

/*****************************************************************************/
//  Description : Get city code by name, used by google weather api, must be utf-8 string.
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWEATHER_GetCityCode(char* utf8_str_ptr);

/*****************************************************************************/
//  Description : Get the name of current city.
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note: 
/*****************************************************************************/
//PUBLIC char* MMIWEATHER_GetCurCityCode(void);


/*****************************************************************************/
//  Description : Get added cities number
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note: 
/*****************************************************************************/
PUBLIC uint8 MMIWEATHER_GetCityNum(void);


/*****************************************************************************/
//  Description : Send request in order to update weather data.
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIWEATHER_SendReq_UpdateWeather(char* req_url_str);
   
/*****************************************************************************/
//  Description : Get added cities number
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note: 
/*****************************************************************************/
PUBLIC uint8 MMIWEATHER_GetUpdateFrequency(void);


/*****************************************************************************/
//  Description : Get added cities number
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWEATHER_SetUpdateFrequency(MMIWEATHER_UPDATE_FREQ_E frequency);


/*****************************************************************************/
//  Description : Get upadte range start time
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note: 
/*****************************************************************************/
PUBLIC uint8 MMIWEATHER_GetUpdateRangeStart(void);


/*****************************************************************************/
//  Description : Set upadte range start time
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWEATHER_SetUpdateRangeStart(uint8 start_time);


/*****************************************************************************/
//  Description : Get upadte range end time
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note: 
/*****************************************************************************/
PUBLIC uint8 MMIWEATHER_GetUpdateRangeEnd(void);


/*****************************************************************************/
//  Description : Set upadte range end time
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWEATHER_SetUpdateRangeEnd(uint8 end_time);

/*****************************************************************************/
// 	Description : Read file
//	Global resource dependence : 
//  Author: Gaily.Wang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIWEATHER_ReadFile(wchar* file_name, char *buf_ptr, uint32 bytes_to_read);

/*****************************************************************************/
//  Description : is supporting city
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note: 
/*****************************************************************************/
//PUBLIC BOOLEAN MMIWEATHER_IsSupportCity(wchar *city_name);


/*****************************************************************************/
//  Description : Get current weather Icon of current city.
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIWEATHER_GetCurWeatherIcon(void);

/*****************************************************************************/
//  Description : Get current weather Icon of current city.
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIWEATHER_GetCurWeatherStr(void);


/*****************************************************************************/
//  Description : Get current wind force of current city.
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note: 
/*****************************************************************************/
PUBLIC MMIWEATHER_WIND_FORCE_E MMIWEATHER_GetCurWindForce(void);


/*****************************************************************************/
//  Description : Get current wind force of current city.
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note: 
/*****************************************************************************/
PUBLIC MMIWEATHER_WIND_FORCE_E MMIWEATHER_GetWindForceStr(
                            int32  wind_force,  //[in] 风力值(单位海里/小时)
                            wchar* wstr_ptr     //[out]描述风力的字符串
                            );

/*****************************************************************************/
//  Description : Get current temp uint type.
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note: 
/*****************************************************************************/
PUBLIC WEATHER_TEMP_UNIT_E MMIWEATHER_GetTempUnitType(void);



/*****************************************************************************/
//  Description : Get Lowest Temp By Day
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note: 
/*****************************************************************************/
PUBLIC int32 MMIWEATHER_GetLowTempByDayAndIndex(MMIWEATHER_FC_DAY_E day,uint8 city_index);


/*****************************************************************************/
//  Description : Get Highest Temp By Day
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note: 
/*****************************************************************************/
PUBLIC int32 MMIWEATHER_GetHighTempByDayAndIndex(MMIWEATHER_FC_DAY_E day , uint8 city_index);

/*****************************************************************************/
//  Description : set update state: TRUE updating 
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWEATHER_SetUpdateState(BOOLEAN is_updating);


/*****************************************************************************/
//  Description : is update on progress or not.
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIWEATHER_GetUpdatingState(void);

/*****************************************************************************/
//  Description : Get selected Sim
//  Global resource dependence : none
//  Author: Gaily.Wang
//  Note:
/*****************************************************************************/
PUBLIC MN_DUAL_SYS_E MMIWEATHER_GetSelectSim(void);

/*****************************************************************************/
//  Description : Get update type.
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note: 
/*****************************************************************************/
PUBLIC MMIWEATHER_UPDATE_FROM_E MMIWEATHER_GetUpdateType(void);

/*****************************************************************************/
//  Description : Update weather information for current city.
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note: return FALSE if is updating, else return TRUE.
/*****************************************************************************/
PUBLIC BOOLEAN MMIWEATHER_UpdateWeatherInfo(MMIWEATHER_UPDATE_FROM_E update_from);

/*****************************************************************************/
//  Description : MMIWEATHER_StopUpdate
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIWEATHER_StopUpdate(void);


MMI_IMAGE_ID_T MMIWEATHER_GetConditionImgId(MMIWEATHER_CONDITION_E condition_id, WEATHER_ICON_E type);
MMI_TEXT_ID_T MMIWEATHER_GetConditionTxtId(MMIWEATHER_CONDITION_E condition_id);
MMI_TEXT_ID_T MMIWEATHER_GetWeekTxtId(MMIWEATHER_WEEK_E week_id);
MMI_TEXT_ID_T MMIWEATHER_GetWindDirTxtId(MMIWEATHER_WIND_DIR_E wind_dir_id);

PUBLIC MMIWEATHER_CONDITION_E MMIWEATHER_GetCurCondition(void);


#if  (defined MMIWIDGET_SUPPORT) || (defined MMI_GRID_IDLE_SUPPORT)
/*****************************************************************************/
//    Description : 打开快捷方式设置窗口
//    Global resource dependence : 
//    Author:   xiaoming.ren
//    Note:
/*****************************************************************************/
PUBLIC void MMIWIDGET_Weather_OpenShortcutSettingWin(void);
/*****************************************************************************/
//    Description : 打开字体颜色设置窗口
//    Global resource dependence : 
//    Author:   xiaoming.ren
//    Note:
/*****************************************************************************/
PUBLIC void MMIWIDGET_Weather_OpenBGTextColorSettingWin(void);
#endif

#endif

