/*****************************************************************************
** File Name:      mmibrowser_funcs.c                                        *
** Author:         Gaily.Wang                                                *
** Date:           18/04/2011                                                *
** Copyright:      2011 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file define the internal APIs for weather app        *
*****************************************************************************/


/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_app_weather_trc.h"
#include "tasks_id.h"
#include "mn_type.h"
#include "mmi_appmsg.h"
#include "mmi_osbridge.h"
#include "mmifmm_export.h"
#include "mmipub.h"
#include "mmiweather_funcs.h"
#include "mmiweather_text.h"
#include "mmi_signal_ext.h"
#include "mmi_appmsg.h"
#include "gui_ucs2b_converter.h"
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/



/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

MMIWEATHER_UPDATE_FROM_E s_update_from = MMIWEATHER_UPDATE_FROM_UNKNOW;

BOOLEAN                  s_is_updating        = FALSE;

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/


/*****************************************************************************/
//  Description : Get update type.
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note: 
/*****************************************************************************/
PUBLIC MMIWEATHER_UPDATE_FROM_E MMIWEATHER_GetUpdateType(void)
{
    return s_update_from;
}

/*****************************************************************************/
//  Description : Set update type.
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWEATHER_SetUpdateType(MMIWEATHER_UPDATE_FROM_E update_from)
{
    if(update_from < MMIWEATHER_UPDATE_FROM_MAX)
    {
    	s_update_from = update_from;
    }
}
 

/*****************************************************************************/
//  Description : Get the index of current city.
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note: 
/*****************************************************************************/
PUBLIC uint8 MMIWEATHER_GetCurCityIndex(void)
{
	return g_weather_data.setting_info.cur_city;
}

/*****************************************************************************/
//  Description : Set the index of current city.
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWEATHER_SetCurCityIndex(uint8 index)
{
#if defined MMI_WIDGET_WEATHER1 
    MMK_PostMsg(g_weather_widget.win_id, MSG_WEATHER_WGT_SWITCH_CITY_CNF, PNULL, 0); 
#endif
#if defined MMI_WIDGET_WEATHER2
    MMK_PostMsg(g_weather_widget2.win_id, MSG_WEATHER_WGT_SWITCH_CITY_CNF, PNULL, 0);
#endif
	g_weather_data.setting_info.cur_city = index;
    MMIWEATHER_WriteNV(MMIWEATHER_NV_SETTING_INFO);
}

/*****************************************************************************/
//  Description : Get the name of current city.
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note: 
/*****************************************************************************/
PUBLIC wchar* MMIWEATHER_GetCurCityName(void)
{
    wchar*   city_name   = PNULL;
    uint8   index       = MMIWEATHER_GetCurCityIndex();
    city_name = g_weather_data.weather_info[index].city_name;
	return city_name;
}

/*****************************************************************************/
//  Description : Get city code by name, used by google weather api, must be utf-8 string.
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWEATHER_GetCityCode(char* utf8_str_ptr)
{
    wchar* city_name = PNULL;
    const char*  city_code = PNULL;
    uint8  cur_city  = MMIWEATHER_GetCurCity();
    uint8  city_num  = MMIWEATHER_GetCityNum();
    MMIWEATHER_UPDATE_FROM_E update_from = MMIWEATHER_GetUpdateType();

    if(PNULL != utf8_str_ptr)
    {
        if(MMIWEATHER_UPDATE_FROM_ADD_CITY == update_from)
        {
            if(0 < MMIAPICOM_Wstrlen(g_weather_data.weather_info[city_num].city_code))
            {
                city_name = g_weather_data.weather_info[city_num].city_code;
            }
            else
            {
                city_name = g_weather_data.weather_info[city_num].city_name;
            }
        }
        else
        {
            if(0 < MMIAPICOM_Wstrlen(g_weather_data.weather_info[cur_city].city_code))
            {
                city_name = g_weather_data.weather_info[cur_city].city_code;
            }
            else
            {
                city_name = g_weather_data.weather_info[cur_city].city_name;
            }
        }

        city_code = MMIWEATHER_GetSpecialCityCode(city_name);

        //SCI_TRACE_LOW:"[weather]: city_code = %d ---"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWEATHER_FUNCS_128_112_2_18_3_6_37_57,(uint8*)"d",city_code);

        if(city_code != PNULL)
        {
            strncpy(utf8_str_ptr,city_code,MMIWEATHER_MAX_CITY_CODE_LEN);
        }
        else
        {
            GUI_WstrToUTF8((uint8*)utf8_str_ptr, MMIWEATHER_MAX_CITY_CODE_LEN, city_name, MMIAPICOM_Wstrlen(city_name));
        }

        //SCI_TRACE_LOW:"[weather]: utf8_str_ptr = %s ---"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWEATHER_FUNCS_139_112_2_18_3_6_37_58,(uint8*)"s",utf8_str_ptr);
    }
}

/*****************************************************************************/
//  Description : Get the name of current city.
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note: 
/*****************************************************************************/
/*PUBLIC char* MMIWEATHER_GetCurCityCode(void)
{
    char*   city_code   = PNULL;
    uint8   index       = MMIWEATHER_GetCurCityIndex();
    city_code = g_weather_data.weather_info[index].city_code;
	return city_code;
}*/

/*****************************************************************************/
//  Description : Get added cities number
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note: 
/*****************************************************************************/
PUBLIC uint8 MMIWEATHER_GetCityNum(void)
{
    return g_weather_data.setting_info.city_num;
}

#ifdef WIN32

LOCAL uint8 s_update_timer_id = 0;

LOCAL void Simulator_HandleUpdateTimer(
                       uint8  timer_id,
                       uint32 param
                       )
{
    if(timer_id == s_update_timer_id)
    {
        MMK_StopTimer(s_update_timer_id);
        s_update_timer_id = 0;
    }
    MMIWEATHER_SetUpdateState(FALSE);
    MMIWEATHER_SendMsgToApp(MSG_WEATHER_PARSE_START_IND);
    
}

PUBLIC BOOLEAN Simulator_StartUpdateTimer(void)
{
    MMIWEATHER_SetUpdateState(TRUE);
    s_update_timer_id = MMK_CreateTimerCallback(3, Simulator_HandleUpdateTimer, 0, FALSE);
}

PUBLIC BOOLEAN Simulator_StopUpdateTimer(void)
{
    MMK_StopTimer(s_update_timer_id);
    s_update_timer_id = 0;
}
#endif

/*****************************************************************************/
//  Description : Update weather information for current city.
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note: return FALSE if is updating, else return TRUE.
/*****************************************************************************/
PUBLIC BOOLEAN MMIWEATHER_UpdateWeatherInfo(MMIWEATHER_UPDATE_FROM_E update_from)
{
    BOOLEAN result = FALSE;
    //SCI_TRACE_LOW:"[weather]:MMIWEATHER_UpdateWeatherInfo---"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWEATHER_FUNCS_203_112_2_18_3_6_37_59,(uint8*)"");

    if(FALSE == MMIWEATHER_GetUpdatingState())
    {
        MMIWEATHER_SetUpdateType(update_from);
        #ifdef WIN32
        //result = MMIWEATHER_ParseStart();
        result = Simulator_StartUpdateTimer();
        #else
        result = MMIWEATHER_PdpActive();
        #endif
        //SCI_TRACE_LOW:"[weather]:MMIWEATHER_UpdateWeatherInfo---result = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWEATHER_FUNCS_214_112_2_18_3_6_37_60,(uint8*)"d",result);
        if(result)
        {
            MMIWEATHER_SetUpdateState(TRUE); 
        }
    }
    
    return result;
}   


/*****************************************************************************/
//  Description : MMIWEATHER_GetUpdateFrequency
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note: 
/*****************************************************************************/
PUBLIC uint8 MMIWEATHER_GetUpdateFrequency(void)
{
    uint8 update_freq = 0;
    switch(g_weather_data.setting_info.update_frequency)		   	
    {
    //case UPDATE_FREQ_1H:
    //    update_freq = 1;
    //    break;			            
    //case UPDATE_FREQ_2H:
    //    update_freq = 2;
    //    break;			            
    case UPDATE_FREQ_4H:
        update_freq = 4;
        break;			            
    case UPDATE_FREQ_6H:
        update_freq = 6;
        break;			            
    case UPDATE_FREQ_8H:
        update_freq = 8;
        break;			            
    case UPDATE_FREQ_12H:
        update_freq = 12;
        break;			            
    //case UPDATE_FREQ_24H:
    //    update_freq = 24;
    //    break;			            
    default:
        update_freq = MMIWEATHER_DEFAULT_UPDATE_FREQ;
        break;			            
    }       
    return update_freq;
}

/*****************************************************************************/
//  Description : MMIWEATHER_SetUpdateFrequency
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWEATHER_SetUpdateFrequency(MMIWEATHER_UPDATE_FREQ_E frequency)
{
    g_weather_data.setting_info.update_frequency = frequency;
}

/*****************************************************************************/
//  Description : Get upadte range start time
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note: 
/*****************************************************************************/
PUBLIC uint8 MMIWEATHER_GetUpdateRangeStart(void)
{
    return (MMIWEATHER_UPDATE_TIME_BEGIN_NOLIMIT == g_weather_data.setting_info.update_range_start ? 0 : g_weather_data.setting_info.update_range_start);
}

/*****************************************************************************/
//  Description : Set upadte range start time
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWEATHER_SetUpdateRangeStart(uint8 start_time)
{
    g_weather_data.setting_info.update_range_start = start_time;
}

/*****************************************************************************/
//  Description : Get upadte range end time
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note: 
/*****************************************************************************/
PUBLIC uint8 MMIWEATHER_GetUpdateRangeEnd(void)
{
    //SCI_TRACE_LOW:"[WEATHER]:g_weather_data.setting_info.update_range_end = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWEATHER_FUNCS_295_112_2_18_3_6_37_61,(uint8*)"d",g_weather_data.setting_info.update_range_end);

    if(g_weather_data.setting_info.update_range_end >= 24)//cr130950:加入保护
    {
        return 23;
    }
    else
    {
        return (MMIWEATHER_UPDATE_TIME_END_NOLIMIT == g_weather_data.setting_info.update_range_end ? 23 : g_weather_data.setting_info.update_range_end);
    }
}

/*****************************************************************************/
//  Description : Set upadte range end time
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWEATHER_SetUpdateRangeEnd(uint8 end_time)
{
    g_weather_data.setting_info.update_range_end = end_time;
}

#if 1

/*****************************************************************************/
// 	Description : Read file
//	Global resource dependence : 
//  Author: Gaily.Wang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIWEATHER_ReadFile(wchar* file_name, char *buf_ptr, uint32 bytes_to_read)
{
    MMIFILE_HANDLE file_handle = NULL;
    BOOLEAN result = FALSE;
    uint32 bytes_read = 0;

    file_handle = MMIAPIFMM_CreateFile(file_name, (uint32)SFS_MODE_READ|SFS_MODE_OPEN_EXISTING, NULL, NULL);	

    if(SFS_INVALID_HANDLE != file_handle)
    {
        if (SFS_ERROR_NONE == MMIAPIFMM_ReadFile(file_handle, buf_ptr, bytes_to_read, &bytes_read, NULL))
        {
            result = TRUE;
        }
        
        MMIAPIFMM_CloseFile(file_handle);
    }

    //SCI_TRACE_LOW:"[weather]:MMIWEATHER_ReadFile---bytes_read = %d ---"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWEATHER_FUNCS_340_112_2_18_3_6_38_62,(uint8*)"d",bytes_read);

    return result;
}

#endif


/*****************************************************************************/
//  Description : Open Popup win
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWEATHER_SendMsgToApp(uint32 sig_id)
{
    MmiSignalS *signal = PNULL;
    
    //SCI_TRACE_LOW:"[weather]:MMIWEATHER_SendMsgToApp---sig_id = 0x%04x ---"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWEATHER_FUNCS_356_112_2_18_3_6_38_63,(uint8*)"d", sig_id);

    //creat signal
    MMI_CREATE_SIGNAL(signal,(uint16)sig_id, sizeof(MmiSignalS), P_APP);

    //send signal
    MMI_SEND_SIGNAL(signal, P_APP);
}
/*****************************************************************************/
//  Description : Get current temp uint type.
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note: 
/*****************************************************************************/
PUBLIC WEATHER_TEMP_UNIT_E MMIWEATHER_GetTempUnitType(void)
{
    return g_weather_data.setting_info.temp_unit;
}


/*****************************************************************************/
//  Description : Get Lowest Temp By Day for current city
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note: 
/*****************************************************************************/
PUBLIC int32 MMIWEATHER_GetLowTempByDayAndIndex(MMIWEATHER_FC_DAY_E day, uint8 city_index)
{
    int32 low_temp = 0;
    //uint8 cur_city_id = 0;

    if(MMIWEATHER_GetCityNum() > 0)
    {
        //cur_city_id = MMIWEATHER_GetCurCityIndex();
        switch(day)		   	
        {
            case FC_DAY_1:
            {
                low_temp = g_weather_data.weather_info[city_index].forecast.low1;
                break;			            
            }
            case FC_DAY_2:
            {
                low_temp = g_weather_data.weather_info[city_index].forecast.low2;
                break;			            
            }
            case FC_DAY_3:
            {
                low_temp = g_weather_data.weather_info[city_index].forecast.low3;
                break;			            
            }
            case FC_DAY_4:
            {
                low_temp = g_weather_data.weather_info[city_index].forecast.low4;
                break;			            
            }
            default:
            {
                break;			            
            }
        }   
    }
        
    return low_temp;
}

/*****************************************************************************/
//  Description : Get Highest Temp By Day for current city
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note: 
/*****************************************************************************/
PUBLIC int32 MMIWEATHER_GetHighTempByDayAndIndex(MMIWEATHER_FC_DAY_E day,uint8 city_index)
{
    int32 high_temp = 0;

    //uint8 cur_city_id = 0;

    if(MMIWEATHER_GetCityNum() > 0)
    {
        //cur_city_id = MMIWEATHER_GetCurCityIndex();
        switch(day)		   	
        {
            case FC_DAY_1:
            {
                high_temp = g_weather_data.weather_info[city_index].forecast.high1;
                break;			            
            }
            case FC_DAY_2:
            {
                high_temp = g_weather_data.weather_info[city_index].forecast.high2;
                break;			            
            }
            case FC_DAY_3:
            {
                high_temp = g_weather_data.weather_info[city_index].forecast.high3;
                break;			            
            }
            case FC_DAY_4:
            {
                high_temp = g_weather_data.weather_info[city_index].forecast.high4;
                break;			            
            }
            default:
            {
                break;			            
            }
        }   
    }    
        
    return high_temp;
}


/*****************************************************************************/
//  Description : Get current wind force of current city.
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note: 
/*****************************************************************************/
PUBLIC MMIWEATHER_WIND_FORCE_E MMIWEATHER_GetWindForceStr(
                            int32  wind_force,  //[in] 风力值(单位海里/小时)
                            wchar* wstr_ptr     //[out]描述风力的字符串
                            )
{
    MMIWEATHER_WIND_FORCE_E wind_force_id = WIND_FORCE_UNKNOW;

    if(PNULL != wstr_ptr)

    {
        if(wind_force < 1) 
        {
            wind_force_id = WIND_FORCE_0;
            MMIAPICOM_Wstrcpy(wstr_ptr, L"0");
        }
        else if(wind_force < 4) 
        {
            wind_force_id = WIND_FORCE_1_2;
            MMIAPICOM_Wstrcpy(wstr_ptr, L"1-2");
        }
        else if(wind_force < 11) 
        {
            wind_force_id = WIND_FORCE_2_3;
            MMIAPICOM_Wstrcpy(wstr_ptr, L"2-3");
        }
        else if(wind_force < 16) 
        {
            wind_force_id = WIND_FORCE_3_4;
            MMIAPICOM_Wstrcpy(wstr_ptr, L"3-4");
        }
        else if(wind_force < 22) 
        {
            wind_force_id = WIND_FORCE_4_5;
            MMIAPICOM_Wstrcpy(wstr_ptr, L"4-5");
        }
        else if(wind_force < 28) 
        {
            wind_force_id = WIND_FORCE_5_6;
            MMIAPICOM_Wstrcpy(wstr_ptr, L"5-6");
        }
        else if(wind_force < 34) 
        {
            wind_force_id = WIND_FORCE_6_7;
            MMIAPICOM_Wstrcpy(wstr_ptr, L"6-7");
        }
        else if(wind_force < 41) 
        {
            wind_force_id = WIND_FORCE_7_8;
            MMIAPICOM_Wstrcpy(wstr_ptr, L"7-8");
        }
        else if(wind_force < 48) 
        {
            wind_force_id = WIND_FORCE_8_9;
            MMIAPICOM_Wstrcpy(wstr_ptr, L"8-9");
        }
        else if(wind_force < 56) 
        {
            wind_force_id = WIND_FORCE_9_10;
            MMIAPICOM_Wstrcpy(wstr_ptr, L"9-10");
        }
        else if(wind_force < 64) 
        {
            wind_force_id = WIND_FORCE_10_11;
            MMIAPICOM_Wstrcpy(wstr_ptr, L"10-11");
        }
        else if(wind_force < 72) 
        {
            wind_force_id = WIND_FORCE_11_12;
            MMIAPICOM_Wstrcpy(wstr_ptr, L"11-12");
        }
        else
        {
            wind_force_id = WIND_FORCE_12_ABOVE;
            MMIAPICOM_Wstrcpy(wstr_ptr, L"12");
        }
            
    }

    return wind_force_id;

}

/*****************************************************************************/
//  Description : Get current condition of current city.
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note: 
/*****************************************************************************/
PUBLIC MMIWEATHER_CONDITION_E MMIWEATHER_GetCurCondition(void)
{
	uint8 city_index = 0;
    uint16 wstr_len = 0;
    char   cur_date_str[12] = {0};
    wchar  cur_date_wstr[6] = {0};
    SCI_DATE_T cur_date = {0};
    
	city_index = MMIWEATHER_GetCurCityIndex();
    if(city_index  >= MMIWEATHER_MAX_CITY_NUM)
    {
        //SCI_TRACE_LOW:"[WEATHER]MMIWEATHER_GetCurCondition city_index = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWEATHER_FUNCS_565_112_2_18_3_6_38_64,(uint8*)"d",city_index);
        return WEATHER_CONDITION_UNKNOW;
    }
    TM_GetSysDate(&cur_date);
    sprintf(cur_date_str, "%d/%d", cur_date.mon, cur_date.mday);
    MMIAPICOM_StrToWstr((uint8*)cur_date_str, cur_date_wstr);
    wstr_len = MMIAPICOM_Wstrlen(cur_date_wstr);
    if(0 == MMIAPICOM_Wstrncmp(cur_date_wstr, 
                                g_weather_data.weather_info[city_index].forecast.date1, 
                                wstr_len))
    {
        return g_weather_data.weather_info[city_index].forecast.weather1;
    }
    else if(0 == MMIAPICOM_Wstrncmp(cur_date_wstr, 
                                g_weather_data.weather_info[city_index].forecast.date2, 
                                wstr_len))
    {
        return g_weather_data.weather_info[city_index].forecast.weather2;
    }
    else if(0 == MMIAPICOM_Wstrncmp(cur_date_wstr, 
                                g_weather_data.weather_info[city_index].forecast.date3, 
                                wstr_len))
    {
        return g_weather_data.weather_info[city_index].forecast.weather3;
    }
    else if(0 == MMIAPICOM_Wstrncmp(cur_date_wstr, 
                                g_weather_data.weather_info[city_index].forecast.date4, 
                                wstr_len))
    {
        return g_weather_data.weather_info[city_index].forecast.weather4;
    }
    else
    {
        return WEATHER_CONDITION_UNKNOW;
    }
}


/*****************************************************************************/
//  Description : set update state: TRUE updating 
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWEATHER_SetUpdateState(BOOLEAN is_updating)
{
    s_is_updating = is_updating;
}



/*****************************************************************************/
//  Description : is update on progress or not.
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIWEATHER_GetUpdatingState(void)
{
    return s_is_updating;
}

/*****************************************************************************/
//  Description : MMIWEATHER_StopUpdate
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note:
/*****************************************************************************/
/*PUBLIC BOOLEAN MMIWEATHER_StopUpdate(void)
{
#ifdef WIN32
    Simulator_StopUpdateTimer();
#else
    //-1-. stop pdp active timer.
    //StopPdpActiveTimer();
    //-2-. cancel req.
    //-3-. pdp deactive.
    if(s_is_http_init)
    {
        CancelHttpRequest();
    }
    else
    {
        MMIWEATHER_PdpDeactive();
    }
#endif
    return TRUE;
}*/

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif


