/******************************************************************************
 ** File Name:      mmi_app_weather_trc.h                                         *
 ** DATE:           2012.03.18                                                *
 ** Copyright:      Copyright 2012 by Spreadtrum Communications,Inc.          *
 **                 All Rights Reserved.                                      *
 **                 This software is supplied under the terms of a license    *
 **                 agreement or non-disclosure agreement with Spreadtrum.    *
 **                 Passing on and copying of this document,and communication *
 **                 of its contents is not permitted without prior written    *
 **                 authorization.                                            *
 ** Description:                                                              *
 **                                                                           *
 **                                                                           *
 ******************************************************************************
 ******************************************************************************
 **                               Edit History                                *
 ** ------------------------------------------------------------------------- *
 **     DATE          NAME             DESCRIPTION                            *
 **---------------------------------------------------------------------------*
 **  2012.03.18                         Create.                               *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/
//trace_id:238
#ifndef _MMI_APP_WEATHER_TRC_H_
#define _MMI_APP_WEATHER_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define MMIWEATHER_624_112_2_18_3_6_33_0 "[weather]:MMIWEATHER_InitApp---"
#define MMIWEATHER_639_112_2_18_3_6_33_1 "[weather]:MMIWEATHER_ResetApp---"
#define MMIWEATHER_657_112_2_18_3_6_33_2 "[weather]:MMIWEATHER_WriteNV---"
#define MMIWEATHER_685_112_2_18_3_6_33_3 "[weather]:MMIWEATHER_ReadNV---"
#define MMIWEATHER_807_112_2_18_3_6_33_4 "[weather]:MMIWEATHER_AddCity---"
#define MMIWEATHER_833_112_2_18_3_6_33_5 "[weather]:MMIWEATHER_AddCity---"
#define MMIWEATHER_871_112_2_18_3_6_33_6 "[weather]:MMIWEATHER_DelCity---"
#define MMIWEATHER_976_112_2_18_3_6_34_7 "[weather]:HandlePdpActiveTimer---Time out error!---"
#define MMIWEATHER_998_112_2_18_3_6_34_8 "[weather]:Weather_StartSockTimer"
#define MMIWEATHER_1007_112_2_18_3_6_34_9 "[weather]:Weather_StopSockTimer"
#define MMIWEATHER_1024_112_2_18_3_6_34_10 "[weather]:MMIWEATHER_HandlePdpMsg--- msg_id=%d"
#define MMIWEATHER_1025_112_2_18_3_6_34_11 "[weather]:MMIWEATHER_HandlePdpMsg--- result=%d"
#define MMIWEATHER_1035_112_2_18_3_6_34_12 "[weather]:MMIWEATHER_HandlePdpMsg--- MMIPDP_ACTIVE_CNF"
#define MMIWEATHER_1050_112_2_18_3_6_34_13 "[weather]:MMIWEATHER_HandlePdpMsg--- MMIPDP_DEACTIVE_CNF"
#define MMIWEATHER_1055_112_2_18_3_6_34_14 "[weather]:MMIWEATHER_HandlePdpMsg--- MMIPDP_DEACTIVE_IND"
#define MMIWEATHER_1078_112_2_18_3_6_34_15 "[weather]:MMIWEATHER_PdpActive"
#define MMIWEATHER_1195_112_2_18_3_6_34_16 "[weather]:MMIWEATHER_PdpDeactive---"
#define MMIWEATHER_1225_112_2_18_3_6_34_17 "[weather]:MMIWEATHER_IsSupportCity---TRUE---"
#define MMIWEATHER_1242_112_2_18_3_6_34_18 "[weather]:MMIWEATHER_IsSupportCity---res_val = %d "
#define MMIWEATHER_1256_112_2_18_3_6_34_19 "[weather]:MMIWEATHER_GetCityCodeByName---"
#define MMIWEATHER_1276_112_2_18_3_6_34_20 "[weather]:MMIWEATHER_GetCityCodeByName---city_code = %s "
#define MMIWEATHER_1412_112_2_18_3_6_34_21 "[weather]:HandleHttpInitCnf---Not weather task---"
#define MMIWEATHER_1421_112_2_18_3_6_34_22 "[weather]:HandleHttpInitCnf--- no memory---"
#define MMIWEATHER_1454_112_2_18_3_6_35_23 "[weather]: uri.uri_ptr = %s ---"
#define MMIWEATHER_1503_112_2_18_3_6_35_24 "[weather]:HandleHttpWeatherDataInd---receive weather information as buffer mode---"
#define MMIWEATHER_1620_112_2_18_3_6_35_25 "[weather]:HandleHttpErrorInd error = %d"
#define MMIWEATHER_1783_112_2_18_3_6_35_26 "[weather]---HandleWeatherMsg---:msg_id = 0x%04x"
#define MMIWEATHER_1788_112_2_18_3_6_35_27 "[weather]---MSG_WEATHER_PDP_ACTIVE_CNF---"
#define MMIWEATHER_1802_112_2_18_3_6_35_28 "[weather]---MSG_WEATHER_PDP_ACTIVE_FIILED_CNF---"
#define MMIWEATHER_1838_112_2_18_3_6_35_29 "MMIWEATHER_UPDATE_FROM_ADD_CITY failed city_num = %d"
#define MMIWEATHER_1851_112_2_18_3_6_35_30 "[weather]---MSG_WEATHER_PARSE_START_IND---"
#define MMIWEATHER_1857_112_2_18_3_6_35_31 "[weather]---MSG_WEATHER_PARSE_SUCCESS_CNF---"
#define MMIWEATHER_1921_112_2_18_3_6_35_32 "[weather]---MSG_WEATHER_TASK_DELETE---"
#define MMIWEATHER_1981_112_2_18_3_6_36_33 "[weather]---default---"
#define MMIWEATHER_2007_112_2_18_3_6_36_34 "[weather]:HandleHttpMsg receive HTTP_SIG_INIT_CNF"
#define MMIWEATHER_2013_112_2_18_3_6_36_35 "HandleHttpWeatherDataInd receive HTTP_SIG_DATA_IND"
#define MMIWEATHER_2018_112_2_18_3_6_36_36 "[weather]:HandleHttpMsg receive HTTP_SIG_NEED_AUTH_IND"
#define MMIWEATHER_2023_112_2_18_3_6_36_37 "[weather]:HandleHttpMsg receive HTTP_SIG_REQUEST_ID_IND"
#define MMIWEATHER_2028_112_2_18_3_6_36_38 "[weather]:HandleHttpMsg receive HTTP_SIG_GET_CNF"
#define MMIWEATHER_2033_112_2_18_3_6_36_39 "[weather]:HandleHttpMsg receive HTTP_SIG_HEADER_IND"
#define MMIWEATHER_2038_112_2_18_3_6_36_40 "[weather]:HandleHttpMsg receive HTTP_SIG_ERROR_IND"
#define MMIWEATHER_2043_112_2_18_3_6_36_41 "[weather]:HandleHttpMsg receive HTTP_SIG_CLOSE_CNF"
#define MMIWEATHER_2048_112_2_18_3_6_36_42 "[weather]:HandleHttpMsg receive HTTP_SIG_CANCEL_CNF"
#define MMIWEATHER_2053_112_2_18_3_6_36_43 "[weather]:HandleHttpMsg receive HTTP_SIG_NET_PROG_IND"
#define MMIWEATHER_2058_112_2_18_3_6_36_44 "[weather]:HandleHttpMsg receive HTTP_SIG_REDIRECT_IND"
#define MMIWEATHER_2063_112_2_18_3_6_36_45 "[weather]:HandleHttpMsg Not handle msg_id = %x"
#define MMIWEATHER_2126_112_2_18_3_6_36_46 "[weather]:MMIWEATHER_SetUpdateTimeTable---start=%d, end=%d, freq=%d"
#define MMIWEATHER_2142_112_2_18_3_6_36_47 "[weather]:MMIWEATHER_IsTimeToUpdate---%02d:%02d:%02d"
#define MMIWEATHER_2171_112_2_18_3_6_36_48 "[weather]:MMIWEATHER_GetCurTempRangeStr---"
#define MMIWEATHER_2174_112_2_18_3_6_36_49 "[weather]:MMIWEATHER_GetCurTempRangeStr--- error!"
#define MMIWEATHER_2179_112_2_18_3_6_36_50 "[WEATHER]MMIWEATHER_GetCurTempRangeWstr city_index = %d"
#define MMIWEATHER_2260_112_2_18_3_6_36_51 "[weather]:MMIWEATHER_InitNetworkAccount-----"
#define MMIWEATHER_2274_112_2_18_3_6_36_52 "[weather]:MMIWEATHER_InitNetworkAccount----sim%d apn = %s"
#define MMIWEATHER_2287_112_2_18_3_6_36_53 "[weather]:MMIWEATHER_InitNetworkAccount----sim%d  index = %d"
#define MMIWEATHER_2346_112_2_18_3_6_36_54 "MMIWEATHER_IsAddedCity is_existed = %d,city_num = %d"
#define MMIWEATHER_2389_112_2_18_3_6_36_55 "MMIWEATHER_WeatherInfoAlloc,because PNULL == s_http_content_weather_buffer_address, then alloc"
#define MMIWEATHER_2463_112_2_18_3_6_37_56 "[weather]: city_code = %s ---"
#define MMIWEATHER_FUNCS_128_112_2_18_3_6_37_57 "[weather]: city_code = %d ---"
#define MMIWEATHER_FUNCS_139_112_2_18_3_6_37_58 "[weather]: utf8_str_ptr = %s ---"
#define MMIWEATHER_FUNCS_203_112_2_18_3_6_37_59 "[weather]:MMIWEATHER_UpdateWeatherInfo---"
#define MMIWEATHER_FUNCS_214_112_2_18_3_6_37_60 "[weather]:MMIWEATHER_UpdateWeatherInfo---result = %d"
#define MMIWEATHER_FUNCS_295_112_2_18_3_6_37_61 "[WEATHER]:g_weather_data.setting_info.update_range_end = %d"
#define MMIWEATHER_FUNCS_340_112_2_18_3_6_38_62 "[weather]:MMIWEATHER_ReadFile---bytes_read = %d ---"
#define MMIWEATHER_FUNCS_356_112_2_18_3_6_38_63 "[weather]:MMIWEATHER_SendMsgToApp---sig_id = 0x%04x ---"
#define MMIWEATHER_FUNCS_565_112_2_18_3_6_38_64 "[WEATHER]MMIWEATHER_GetCurCondition city_index = %d"
#define MMIWEATHER_PARSE_913_112_2_18_3_6_41_65 "[weather]:MMIWEATHER_StartParse---"
#define MMIWEATHER_PARSE_929_112_2_18_3_6_41_66 "[weather]:MMIWEATHER_StartParse---Parse success!"
#define MMIWEATHER_PARSE_930_112_2_18_3_6_41_67 "[weather]:MMIWEATHER_StartParse is_support = %d"
#define MMIWEATHER_PARSE_1158_112_2_18_3_6_41_68 "[WEATHER]MMIWEATHER_HandleParsedData PNULL == weather_info_ptr !"
#define MMIWEATHER_PARSE_1320_112_2_18_3_6_42_69 "[weather]:MMIWEATHER_HandleParsedData---%s---"
#define MMIWEATHER_PARSE_1617_112_2_18_3_6_42_70 "[weather]:MMIWEATHER_HandleParsedData---result = %d"
#define MMIWEATHER_PARSE_1691_112_2_18_3_6_42_71 "[weather]:Xml_ParseWeatherData---"
#define MMIWEATHER_PARSE_1704_112_2_18_3_6_43_72 "[weather]:Xml_ParseWeatherData---Success---"
#define MMIWEATHER_PARSE_1730_112_2_18_3_6_43_73 "[weather]:Xml_GetElementAttributes---tag_id = %d"
#define MMIWEATHER_WIN_1105_112_2_18_3_6_46_74 "[weather]:MMIWEATHER_GetFCId---Bad para!"
#define MMIWEATHER_WIN_1677_112_2_18_3_6_47_75 "[WEATHER]SetTitleBar PNULL == weather_info"
#define MMIWEATHER_WIN_2382_112_2_18_3_6_48_76 "[WEATHER]DrawWeatherDetailsPage PNULL == weather_info"
#define MMIWEATHER_WIN_2389_112_2_18_3_6_48_77 "[weather]:weather_info->current.condition = %d"
#define MMIWEATHER_WIN_2531_112_2_18_3_6_48_78 "[WEATHER]:DrawWeatherTrendsPage PNULL == weather_info"
#define MMIWEATHER_WIN_2536_112_2_18_3_6_48_79 "[WEATHER]:DrawWeatherTrendsPage PNULL == cur_weather_info_ptr"
#define MMIWEATHER_WIN_3549_112_2_18_3_6_51_80 "[WEATHER]MMIWEATHER_SendMessage PNULL == weather_info"
#define MMIWEATHER_WIN_3605_112_2_18_3_6_51_81 "Weather SendMessage no mermory"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(MMI_APP_WEATHER_TRC)
TRACE_MSG(MMIWEATHER_624_112_2_18_3_6_33_0,"[weather]:MMIWEATHER_InitApp---")
TRACE_MSG(MMIWEATHER_639_112_2_18_3_6_33_1,"[weather]:MMIWEATHER_ResetApp---")
TRACE_MSG(MMIWEATHER_657_112_2_18_3_6_33_2,"[weather]:MMIWEATHER_WriteNV---")
TRACE_MSG(MMIWEATHER_685_112_2_18_3_6_33_3,"[weather]:MMIWEATHER_ReadNV---")
TRACE_MSG(MMIWEATHER_807_112_2_18_3_6_33_4,"[weather]:MMIWEATHER_AddCity---")
TRACE_MSG(MMIWEATHER_833_112_2_18_3_6_33_5,"[weather]:MMIWEATHER_AddCity---")
TRACE_MSG(MMIWEATHER_871_112_2_18_3_6_33_6,"[weather]:MMIWEATHER_DelCity---")
TRACE_MSG(MMIWEATHER_976_112_2_18_3_6_34_7,"[weather]:HandlePdpActiveTimer---Time out error!---")
TRACE_MSG(MMIWEATHER_998_112_2_18_3_6_34_8,"[weather]:Weather_StartSockTimer")
TRACE_MSG(MMIWEATHER_1007_112_2_18_3_6_34_9,"[weather]:Weather_StopSockTimer")
TRACE_MSG(MMIWEATHER_1024_112_2_18_3_6_34_10,"[weather]:MMIWEATHER_HandlePdpMsg--- msg_id=%d")
TRACE_MSG(MMIWEATHER_1025_112_2_18_3_6_34_11,"[weather]:MMIWEATHER_HandlePdpMsg--- result=%d")
TRACE_MSG(MMIWEATHER_1035_112_2_18_3_6_34_12,"[weather]:MMIWEATHER_HandlePdpMsg--- MMIPDP_ACTIVE_CNF")
TRACE_MSG(MMIWEATHER_1050_112_2_18_3_6_34_13,"[weather]:MMIWEATHER_HandlePdpMsg--- MMIPDP_DEACTIVE_CNF")
TRACE_MSG(MMIWEATHER_1055_112_2_18_3_6_34_14,"[weather]:MMIWEATHER_HandlePdpMsg--- MMIPDP_DEACTIVE_IND")
TRACE_MSG(MMIWEATHER_1078_112_2_18_3_6_34_15,"[weather]:MMIWEATHER_PdpActive")
TRACE_MSG(MMIWEATHER_1195_112_2_18_3_6_34_16,"[weather]:MMIWEATHER_PdpDeactive---")
TRACE_MSG(MMIWEATHER_1225_112_2_18_3_6_34_17,"[weather]:MMIWEATHER_IsSupportCity---TRUE---")
TRACE_MSG(MMIWEATHER_1242_112_2_18_3_6_34_18,"[weather]:MMIWEATHER_IsSupportCity---res_val = %d ")
TRACE_MSG(MMIWEATHER_1256_112_2_18_3_6_34_19,"[weather]:MMIWEATHER_GetCityCodeByName---")
TRACE_MSG(MMIWEATHER_1276_112_2_18_3_6_34_20,"[weather]:MMIWEATHER_GetCityCodeByName---city_code = %s ")
TRACE_MSG(MMIWEATHER_1412_112_2_18_3_6_34_21,"[weather]:HandleHttpInitCnf---Not weather task---")
TRACE_MSG(MMIWEATHER_1421_112_2_18_3_6_34_22,"[weather]:HandleHttpInitCnf--- no memory---")
TRACE_MSG(MMIWEATHER_1454_112_2_18_3_6_35_23,"[weather]: uri.uri_ptr = %s ---")
TRACE_MSG(MMIWEATHER_1503_112_2_18_3_6_35_24,"[weather]:HandleHttpWeatherDataInd---receive weather information as buffer mode---")
TRACE_MSG(MMIWEATHER_1620_112_2_18_3_6_35_25,"[weather]:HandleHttpErrorInd error = %d")
TRACE_MSG(MMIWEATHER_1783_112_2_18_3_6_35_26,"[weather]---HandleWeatherMsg---:msg_id = 0x%04x")
TRACE_MSG(MMIWEATHER_1788_112_2_18_3_6_35_27,"[weather]---MSG_WEATHER_PDP_ACTIVE_CNF---")
TRACE_MSG(MMIWEATHER_1802_112_2_18_3_6_35_28,"[weather]---MSG_WEATHER_PDP_ACTIVE_FIILED_CNF---")
TRACE_MSG(MMIWEATHER_1838_112_2_18_3_6_35_29,"MMIWEATHER_UPDATE_FROM_ADD_CITY failed city_num = %d")
TRACE_MSG(MMIWEATHER_1851_112_2_18_3_6_35_30,"[weather]---MSG_WEATHER_PARSE_START_IND---")
TRACE_MSG(MMIWEATHER_1857_112_2_18_3_6_35_31,"[weather]---MSG_WEATHER_PARSE_SUCCESS_CNF---")
TRACE_MSG(MMIWEATHER_1921_112_2_18_3_6_35_32,"[weather]---MSG_WEATHER_TASK_DELETE---")
TRACE_MSG(MMIWEATHER_1981_112_2_18_3_6_36_33,"[weather]---default---")
TRACE_MSG(MMIWEATHER_2007_112_2_18_3_6_36_34,"[weather]:HandleHttpMsg receive HTTP_SIG_INIT_CNF")
TRACE_MSG(MMIWEATHER_2013_112_2_18_3_6_36_35,"HandleHttpWeatherDataInd receive HTTP_SIG_DATA_IND")
TRACE_MSG(MMIWEATHER_2018_112_2_18_3_6_36_36,"[weather]:HandleHttpMsg receive HTTP_SIG_NEED_AUTH_IND")
TRACE_MSG(MMIWEATHER_2023_112_2_18_3_6_36_37,"[weather]:HandleHttpMsg receive HTTP_SIG_REQUEST_ID_IND")
TRACE_MSG(MMIWEATHER_2028_112_2_18_3_6_36_38,"[weather]:HandleHttpMsg receive HTTP_SIG_GET_CNF")
TRACE_MSG(MMIWEATHER_2033_112_2_18_3_6_36_39,"[weather]:HandleHttpMsg receive HTTP_SIG_HEADER_IND")
TRACE_MSG(MMIWEATHER_2038_112_2_18_3_6_36_40,"[weather]:HandleHttpMsg receive HTTP_SIG_ERROR_IND")
TRACE_MSG(MMIWEATHER_2043_112_2_18_3_6_36_41,"[weather]:HandleHttpMsg receive HTTP_SIG_CLOSE_CNF")
TRACE_MSG(MMIWEATHER_2048_112_2_18_3_6_36_42,"[weather]:HandleHttpMsg receive HTTP_SIG_CANCEL_CNF")
TRACE_MSG(MMIWEATHER_2053_112_2_18_3_6_36_43,"[weather]:HandleHttpMsg receive HTTP_SIG_NET_PROG_IND")
TRACE_MSG(MMIWEATHER_2058_112_2_18_3_6_36_44,"[weather]:HandleHttpMsg receive HTTP_SIG_REDIRECT_IND")
TRACE_MSG(MMIWEATHER_2063_112_2_18_3_6_36_45,"[weather]:HandleHttpMsg Not handle msg_id = %x")
TRACE_MSG(MMIWEATHER_2126_112_2_18_3_6_36_46,"[weather]:MMIWEATHER_SetUpdateTimeTable---start=%d, end=%d, freq=%d")
TRACE_MSG(MMIWEATHER_2142_112_2_18_3_6_36_47,"[weather]:MMIWEATHER_IsTimeToUpdate---%02d:%02d:%02d")
TRACE_MSG(MMIWEATHER_2171_112_2_18_3_6_36_48,"[weather]:MMIWEATHER_GetCurTempRangeStr---")
TRACE_MSG(MMIWEATHER_2174_112_2_18_3_6_36_49,"[weather]:MMIWEATHER_GetCurTempRangeStr--- error!")
TRACE_MSG(MMIWEATHER_2179_112_2_18_3_6_36_50,"[WEATHER]MMIWEATHER_GetCurTempRangeWstr city_index = %d")
TRACE_MSG(MMIWEATHER_2260_112_2_18_3_6_36_51,"[weather]:MMIWEATHER_InitNetworkAccount-----")
TRACE_MSG(MMIWEATHER_2274_112_2_18_3_6_36_52,"[weather]:MMIWEATHER_InitNetworkAccount----sim%d apn = %s")
TRACE_MSG(MMIWEATHER_2287_112_2_18_3_6_36_53,"[weather]:MMIWEATHER_InitNetworkAccount----sim%d  index = %d")
TRACE_MSG(MMIWEATHER_2346_112_2_18_3_6_36_54,"MMIWEATHER_IsAddedCity is_existed = %d,city_num = %d")
TRACE_MSG(MMIWEATHER_2389_112_2_18_3_6_36_55,"MMIWEATHER_WeatherInfoAlloc,because PNULL == s_http_content_weather_buffer_address, then alloc")
TRACE_MSG(MMIWEATHER_2463_112_2_18_3_6_37_56,"[weather]: city_code = %s ---")
TRACE_MSG(MMIWEATHER_FUNCS_128_112_2_18_3_6_37_57,"[weather]: city_code = %d ---")
TRACE_MSG(MMIWEATHER_FUNCS_139_112_2_18_3_6_37_58,"[weather]: utf8_str_ptr = %s ---")
TRACE_MSG(MMIWEATHER_FUNCS_203_112_2_18_3_6_37_59,"[weather]:MMIWEATHER_UpdateWeatherInfo---")
TRACE_MSG(MMIWEATHER_FUNCS_214_112_2_18_3_6_37_60,"[weather]:MMIWEATHER_UpdateWeatherInfo---result = %d")
TRACE_MSG(MMIWEATHER_FUNCS_295_112_2_18_3_6_37_61,"[WEATHER]:g_weather_data.setting_info.update_range_end = %d")
TRACE_MSG(MMIWEATHER_FUNCS_340_112_2_18_3_6_38_62,"[weather]:MMIWEATHER_ReadFile---bytes_read = %d ---")
TRACE_MSG(MMIWEATHER_FUNCS_356_112_2_18_3_6_38_63,"[weather]:MMIWEATHER_SendMsgToApp---sig_id = 0x%04x ---")
TRACE_MSG(MMIWEATHER_FUNCS_565_112_2_18_3_6_38_64,"[WEATHER]MMIWEATHER_GetCurCondition city_index = %d")
TRACE_MSG(MMIWEATHER_PARSE_913_112_2_18_3_6_41_65,"[weather]:MMIWEATHER_StartParse---")
TRACE_MSG(MMIWEATHER_PARSE_929_112_2_18_3_6_41_66,"[weather]:MMIWEATHER_StartParse---Parse success!")
TRACE_MSG(MMIWEATHER_PARSE_930_112_2_18_3_6_41_67,"[weather]:MMIWEATHER_StartParse is_support = %d")
TRACE_MSG(MMIWEATHER_PARSE_1158_112_2_18_3_6_41_68,"[WEATHER]MMIWEATHER_HandleParsedData PNULL == weather_info_ptr !")
TRACE_MSG(MMIWEATHER_PARSE_1320_112_2_18_3_6_42_69,"[weather]:MMIWEATHER_HandleParsedData---%s---")
TRACE_MSG(MMIWEATHER_PARSE_1617_112_2_18_3_6_42_70,"[weather]:MMIWEATHER_HandleParsedData---result = %d")
TRACE_MSG(MMIWEATHER_PARSE_1691_112_2_18_3_6_42_71,"[weather]:Xml_ParseWeatherData---")
TRACE_MSG(MMIWEATHER_PARSE_1704_112_2_18_3_6_43_72,"[weather]:Xml_ParseWeatherData---Success---")
TRACE_MSG(MMIWEATHER_PARSE_1730_112_2_18_3_6_43_73,"[weather]:Xml_GetElementAttributes---tag_id = %d")
TRACE_MSG(MMIWEATHER_WIN_1105_112_2_18_3_6_46_74,"[weather]:MMIWEATHER_GetFCId---Bad para!")
TRACE_MSG(MMIWEATHER_WIN_1677_112_2_18_3_6_47_75,"[WEATHER]SetTitleBar PNULL == weather_info")
TRACE_MSG(MMIWEATHER_WIN_2382_112_2_18_3_6_48_76,"[WEATHER]DrawWeatherDetailsPage PNULL == weather_info")
TRACE_MSG(MMIWEATHER_WIN_2389_112_2_18_3_6_48_77,"[weather]:weather_info->current.condition = %d")
TRACE_MSG(MMIWEATHER_WIN_2531_112_2_18_3_6_48_78,"[WEATHER]:DrawWeatherTrendsPage PNULL == weather_info")
TRACE_MSG(MMIWEATHER_WIN_2536_112_2_18_3_6_48_79,"[WEATHER]:DrawWeatherTrendsPage PNULL == cur_weather_info_ptr")
TRACE_MSG(MMIWEATHER_WIN_3549_112_2_18_3_6_51_80,"[WEATHER]MMIWEATHER_SendMessage PNULL == weather_info")
TRACE_MSG(MMIWEATHER_WIN_3605_112_2_18_3_6_51_81,"Weather SendMessage no mermory")
END_TRACE_MAP(MMI_APP_WEATHER_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _MMI_APP_WEATHER_TRC_H_

