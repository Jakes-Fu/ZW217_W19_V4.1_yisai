/*************************************************************************
 ** File Name:      mmiweather_nv.h                                      *
 ** Author:         Gaily.Wang                                           *
 ** Date:           2011/04/18                                           *
 ** Copyright:      2011 Spreadtrum, Incorporated. All Rights Reserved.  *
 ** Description:    This file defines the function about nv              *
 *************************************************************************/

#ifndef MMIWEATHER_NV_H_
#define MMIWEATHER_NV_H_

#include "sci_types.h"
#include "mmi_module.h"
#include "mmi_modu_main.h"

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

#define MMINV_READ(_item_id,_addr,_ret_value)\
_ret_value = MMI_ReadNVItem(_item_id, _addr)

#define MMINV_WRITE(_item_id,_addr) \
			 MMI_WriteNVItem(_item_id, _addr)

/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/
typedef enum
{
    MMIWEATHER_NV_SETTING_INFO = MMI_MODULE_WEATHER << 16, 
    MMIWEATHER_NV_WEATHER_INFO,
    
#if defined MMIWEATHER_SUPPORT_SPRD_SRV
    MMIWEAHTER_URL_INDEX,
#endif
    MMIWEATHER_NV_MAX_ITEM_NUM
}MMIWEATHER_NV_ITEM_E;


/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
// 	Description : register weather module nv len and max item
//	Global resource dependence : none
//  Author: Gaily.Wang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIWEATHER_RegWeatherNv(void);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif	//MMIWEATHER_NV_H_
