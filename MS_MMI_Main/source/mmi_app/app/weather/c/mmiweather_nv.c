/*************************************************************************
 ** File Name:      mmiweather_nv.c                                      *
 ** Author:         Gaily.Wang                                           *
 ** Date:           2011/04/18                                           *
 ** Copyright:      2011 Spreadtrum, Incorporated. All Rights Reserved.  *
 ** Description:    This file defines the function about nv              *
 *************************************************************************/
#include "mn_type.h"
#include "mmi_modu_main.h"
#include "mmiweather.h"

const uint16 mmiweather_nv_len[] =
{      
    sizeof(MMIWEATHER_SETTING_T),
    sizeof(MMIWEATHER_WEATHER_T) * MMIWEATHER_MAX_CITY_NUM, 
#if defined MMIWEATHER_SUPPORT_SPRD_SRV
    sizeof(uint8),
#endif
};

/*****************************************************************************/
// 	Description : register weather module nv len and max item
//	Global resource dependence : none
//  Author: Gaily.Wang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIWEATHER_RegWeatherNv(void)
{
    MMI_RegModuleNv(MMI_MODULE_WEATHER, mmiweather_nv_len, sizeof(mmiweather_nv_len) / sizeof(uint16));
    return TRUE;
}

