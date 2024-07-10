/*****************************************************************************
** File Name:      mmiwclk_export.h                                                
** Author:           taul.zhu   
** Date:            2004/0824
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe gui common data struct      *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2004/08/24       taul zhu          Create                                   *
******************************************************************************/

#ifndef _MMIWCLK_EXPORT_H_
#define _MMIWCLK_EXPORT_H_


/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "std_header.h"
#include "sci_types.h"
//#include "mmiset.h"
#include "mmiset_export.h"
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#define WT_TIME_ZONE_NUM_TOTAL		 35  // 时区总数
//#define WT_ZONE_CITY_MAX_NUM         15  // 同一个时区内最大城市数量

#define     WT_DEFAULT_CITY_NUM  10
//#define     WT_TOTAL_CITY_NUM    81
#define     ONE_FROM_TOTAL_CHILD (8)


/**--------------------------------------------------------------------------*
**                         TYPE AND CONSTANT                                *
**--------------------------------------------------------------------------*/

//all city enum
typedef enum
{
    MMI_ACC_4_P_ABUDHABI,
    MMI_ACC_1_P_AMSTERDAM,
    MMI_ACC_9_N_ANCHORAGE,
    MMI_ACC_12_P_AUCKLAND,
    MMI_ACC_11_N_APIA,
    MMI_ACC_1_P_PARIS,
    MMI_ACC_3_N_BRASILIA,
    MMI_ACC_5P5_P_BANGALORE,
    MMI_ACC_8_P_BEIJING,
    MMI_ACC_1_P_BERLIN,
    MMI_ACC_9P5_N_POLYNESIA,
    MMI_ACC_1_P_BUDAPEST,
    MMI_ACC_2_P_BUCHAREST,
    MMI_ACC_1_P_PRAGUE,
    MMI_ACC_10P5_P_BRISBANE,
    MMI_ACC_1_P_BRUSSELS,
    MMI_ACC_3_N_BUENOS_AIRES,
    MMI_ACC_6_P_DHAKA,
    MMI_ACC_2_N_CENTRAL_ATLANTIC,
    MMI_ACC_3P5_P_TEHRAN,
    MMI_ACC_9_P_TOKYO,
    MMI_ACC_7_N_PHOENIX,
    MMI_ACC_8P5_N_EASTER_ISLAND,
    MMI_ACC_1_P_COPENHAGEN,
    MMI_ACC_10_P_GUAM,
    MMI_ACC_10_P_VLADIVOSTOK,
    MMI_ACC_7_P_HANOI,
    MMI_ACC_1_P_WARSAW,
    MMI_ACC_5P5_P_KOLKATA,
    MMI_ACC_5P75_P_KATHMANDU,
    MMI_ACC_2_P_KIEV,
    MMI_ACC_8_P_KUALA_LUMPUR,
    MMI_ACC_8_N_SAN_FRANCISCO,
    MMI_ACC_4P5_P_KABUL,
    MMI_ACC_2_P_CAIRO,
    MMI_ACC_2_P_CAPE_TOWN,
    MMI_ACC_10_P_CANBERRA,
    MMI_ACC_0_CASABLANCA,
    MMI_ACC_12_N_KWAJALEIN,
    MMI_ACC_3P5_N_LAPAZ,
    MMI_ACC_0_RABAT,
    MMI_ACC_0_LISBON,
    MMI_ACC_0_LONDON,
    MMI_ACC_1_P_ROME,
    MMI_ACC_8_N_LOSANGELES,
    MMI_ACC_1_P_LUXEMBOURG,
    MMI_ACC_1_P_MADRID,
    MMI_ACC_7_P_BANGKOK,
    MMI_ACC_8_P_MANILA,
    MMI_ACC_5P5_P_MUMBAI,
    MMI_ACC_5_N_MONTREAL,
    MMI_ACC_9P5_P_MELBOURNE,
    MMI_ACC_3_P_MOSCOW,
    MMI_ACC_6_N_MEXICO,
    MMI_ACC_5_N_NEWYORK,
    MMI_ACC_11P5_P_NORFOLD_ISLAND,
    MMI_ACC_1_N_PRAIA,
    MMI_ACC_1_P_GENEVA,
    MMI_ACC_4_N_SANTIAGO,
    MMI_ACC_5_N_BOGOTA,
    MMI_ACC_1_P_STOCKHOLM,
    MMI_ACC_1_P_ZURICH,
    MMI_ACC_2_P_SOFIA,
    MMI_ACC_11_P_SOLOMON,
    MMI_ACC_8_P_TAIPEI,
    MMI_ACC_12P75_P_TONGA,
    MMI_ACC_10_N_HONOLULU,
    MMI_ACC_12_P_WELLINGTON,
    MMI_ACC_1_P_VIENNA,
    MMI_ACC_8_N_VANCOUVER,
    MMI_ACC_8_P_HONGKONG,
    MMI_ACC_5P5_P_NEWDELHI,
    MMI_ACC_10_P_SYDNEY,
    MMI_ACC_8_P_SINGAPORE,
    MMI_ACC_2_P_ATHENS,
    MMI_ACC_7_P_JAKARTA,
    MMI_ACC_6P5_P_YANGON,
    MMI_ACC_5_P_ISLAMABAD,
    MMI_ACC_2_P_ISTANBUL,
    MMI_ACC_2_P_JOHANNESBURG,
    MMI_ACC_6_N_CHICAGO,
    
    MMI_ACC_CITY_MAX,
}WT_CITY_INDEX_E;

// 定义城市信息数据结构
typedef struct _MCC_WT_CITY_INFO_T_
{
	MMIACC_RTC_DATE_T date;         // 日期
	MMIACC_RTC_DATE_T time;         // 时间
}MCC_WT_CITY_INFO_T;

typedef struct _WT_TIME_CITY_ZONE
{
	MMI_TEXT_ID_T	city_index;//city
	MMI_TEXT_ID_T	zone_index;//zone
	uint16	         mcc;    //mobile country code
    WT_CITY_INDEX_E  city_order; //the order and index of city
}WT_TIME_CITY_ZONE;


typedef struct _WT_CITY_RECORD_INFO
{
	MMI_TEXT_ID_T	city_text_id;//city text id in str table
	BOOLEAN         summer_flag; //if summer time on/off
}WT_CITY_RECORD_INFO;

/*****************************************************************************/
//   FUNCTION:      MMIAPIACC_GetLocalTimeZone
//	Global resource dependence : 
//   Author:        taul.zhu
//	Note:   由于新的需求增加半时区，所以此接口返回的值将会是float
/*****************************************************************************/
extern PUBLIC  float MMIAPIACC_GetLocalTimeZone(void);
/*****************************************************************************/
//   FUNCTION:      MMIAPIACC_GetLocalTimeZoneEx
//	Global resource dependence : 
//   Author:        taul.zhu
//	Note:   由于新的需求增加半时区，所以此接口返回的值将会是float
/*****************************************************************************/
extern PUBLIC void  MMIAPIACC_GetLocalTimeZoneEx(float* fOut);

//typedef uint8 MMIACC_RTC_DATE_T[RTC_DATE_STRING_LENGTH + 1];
//typedef uint8 MMIACC_RTC_TIME_T[RTC_TIME_STRING_LENGTH + 1];

/*****************************************************************************/
//   FUNCTION:      MMIAPIWCLK_Init
// 	        Init NV, ...
//	Global resource dependence : 
//   Author:        taul.zhu
//	Note:   
/*****************************************************************************/
extern PUBLIC BOOLEAN MMIAPIWCLK_Init(void);

/***********************************************************************************
Name:			MMIWCLK_ResetEnvSetSetting
Description:	reset to default value of worldtime GMT
Author:    Murphy.xie
DATE:  2010/11/26
Return:		 True if  date is ok, otherwise false.
***********************************************************************************/
PUBLIC BOOLEAN MMIAPIWCLK_ResetFactorySetting(void);


/*****************************************************************************/
// 	Description : MMIAPIWCLK_OpenMainWin
//         
//	Global resource dependence : 
//   Author:        zhaohui
//	Note:   
/*****************************************************************************/
extern PUBLIC void MMIAPIWCLK_OpenMainWin(void);


/*****************************************************************************/
// 	Description : MMIAPIACC_GetLocalTimeZoneInfo
//	Global resource dependence : 
//  Author:       murphy.xie
//	Note:   
/*****************************************************************************/
PUBLIC MCC_WT_CITY_INFO_T* MMIAPIACC_GetLocalTimeZoneInfo(void);

/*****************************************************************************/
// 	Description : MMIAPIACC_GetOtherTimeZoneInfo
//	Global resource dependence : 
//  Author:   murphy.xie
//	Note:   
/*****************************************************************************/
PUBLIC MCC_WT_CITY_INFO_T* MMIAPIACC_GetOtherTimeZoneInfo(void);

/*****************************************************************************/
// 	Description : MMIAPIACC_GetLocalTimeZoneStringID
//	Global resource dependence : 
//  Author:       murphy.xie
//	Note:   
/*****************************************************************************/
PUBLIC MMI_TEXT_ID_T MMIAPIACC_GetLocalCityStringID(void);

/*****************************************************************************/
// 	Description : MMIAPIACC_GetLocalTimeZoneStringID
//	Global resource dependence : 
//  Author:       murphy.xie
//	Note:   
/*****************************************************************************/
PUBLIC MMI_TEXT_ID_T MMIAPIACC_GetOtherCityStringID(void);

/*****************************************************************************/
//   FUNCTION:      MMIAPIACC_IsLocalTimeZoneInSummer
//	Global resource dependence : 
//   Author:        dave.ruan
//	Note:   判断当前时区是否处于夏令时
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIACC_IsLocalTimeZoneInSummer(void);

/*****************************************************************************/
//   FUNCTION:      Get Local Time Zone Info
//	Global resource dependence : 
//   Author:        dave.ruan
//	Note:   get local city,timezone,and mcc info
/*****************************************************************************/
PUBLIC WT_TIME_CITY_ZONE MMIAPIACC_GetLocalTimeZoneCityInfo(void);

#if defined(MMI_UPDATE_TIME_SUPPORT)
/*****************************************************************************/
// Description : 设置本地时区和夏令时
//              (由网络时间更新系统时间的同时,时区和夏令时也需同步 更新)
// Global resource dependence :
// Author:dave.ruan
// Note: mcc  从plmn可以获取，是当前运营商的plmn，而非SIM卡的
//      time zone is int8,表示以15分钟为单位的时区，是整数时区的4倍,
//      dst_value is daylight saving time的值，可以是0,1,2
//      current_year    当前年份
/*****************************************************************************/
PUBLIC void MMIAPIWCLK_SetTimeZone(
                                   uint16  mcc,     //MN_PLMN_T.mcc
                                   const int8  time_zone,   //(-12*4~12.75*4)
                                   uint16 current_year,  //year index from NITZ
                                   uint8 dst_value      //is 0,1,2
                                   );

#endif

/**---------------------------------------------------------------------------*
 **                    Old Interface  Macro Declaration    				   *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  如下宏定义用以实现对外接口更名前后的兼容
/*****************************************************************************/
#ifdef MMI_FUNC_COMPATIBLE_SUPPORT

#define MMIWCLK_Init				                    MMIAPIWCLK_Init
#define MMIWCLK_ResetFactorySetting				    MMIAPIWCLK_ResetFactorySetting

#endif //MMI_FUNC_COMPATIBLE_SUPPORT

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif  


