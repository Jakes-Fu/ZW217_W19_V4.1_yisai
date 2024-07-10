/*****************************************************************************
** File Name:      mmi_solarlunar.h                                          *
** Author:                                                                   *
** Date:           07/14/2006                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe solar lunar calendar func   *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 07/14/2006     liqing.peng      Create
** 08/09/2006     liqing.peng      Modify -- code review
******************************************************************************/


#ifndef _MMI_SOLARLUNAR_H 
#define _MMI_SOLARLUNAR_H


/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "mmicom_time.h"


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
#define QUERY_YEAR_BOTTOM           MMICOM_QUERY_MIN_YEAR    //可查询的最早年份
#define QUERY_YEAR_TOP              MMICOM_QUERY_MAX_YEAR    //可查询的最晚年份

#define WEEKDAY_NUM                 7       //一周的天数
#define MONTHNUM                    MMICOM_DATE_MAX_MONTH      //一年的月数
#define BIG_MONTH_DAY              MMICOM_DATE_BIG_MONTH_DAY
#define SMALL_MONTH_DAY            MMICOM_DATE_SMALL_MONTH_DAY
#define LEAP_FEB_MONTH_DAY         MMICOM_DATE_LEAP_MONTH_DAY
#define NOT_LEAP_FEB_MONTH_DAY     MMICOM_DATE_NOT_LEAP_MONTH_DAY
#define DAYS_OF_NORMAL_YEAR         365
#define DAYS_OF_LEAP_YEAR           366

#ifdef CALENDAR_SUPPORT
typedef struct
{
    uint8  month;
    uint8  date;
    uint8  index;
}MMI_LUNAR_FESTIVAL_T;

#if defined (IM_SIMP_CHINESE_SUPPORT) || defined (IM_TRAD_CHINESE_SUPPORT)
//农历节日的枚举值，因为只有中国有农历，所以用的是拼音
typedef enum
{
    CALENDAR_CHUNJIE,
    CALENDAR_YUANXIAOJIE,
    CALENDAR_DUANWUJIE,

    CALENDAR_QIXI,
    CALENDAR_ZHONGYUANJIE,
    CALENDAR_ZHONGQIUJIE,
    CALENDAR_CHONGYANGJIE,
    CALENDAR_JIZUJIE,
    CALENDAR_LABAJIE,
    CALENDAR_XIAONIAN,
    CALENDAR_CHUXI,
    CALENDAR_FESTIVAL_MAX
}CALENDAR_FESTIVAL_TYPE_E;
#endif
/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/



/*---------------------------------------------------------------------------*/
/*                          EXTERNAL FUNCTION DECLARE                        */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
//    Description : calculate how many days in a month
//    Global resource dependence : 
//    Author: liqing.peng
//    Note:
/*****************************************************************************/
uint8 MMICALENDAR_GetMonthday(
                            uint16      year,
                            uint8       month
                            );

/*****************************************************************************/
//     Description : 计算某年某月第一天为星期几
//    Global resource dependence : 
//  Author: Jibin
//    Note:
/*****************************************************************************/
uint8 MMICALENDAR_GetFirstday(
                              uint16    solar_year,
                              uint8     solar_month
                              );

/*****************************************************************************/
//    Description : calculate weekday by year, month and day, 
//                  return 0,1,2,3,4,5,6
//    Global resource dependence : 
//    Author: liqing.peng
//    Note:
/*****************************************************************************/
uint8 MMICALENDAR_GetWeekday(
                              uint16        year,
                              uint8         month,
                              uint8         day
                              );


/*****************************************************************************/
//    Description : 计算处于一年中的第几个星期
//    Global resource dependence : 
//    Author: liqing.peng
//    Note:
/*****************************************************************************/
uint8 MMICALENDAR_GetWeekInYear(
                              uint16        year,
                              uint8         month,
                              uint8         day
                              );


/*****************************************************************************/
//    Description : check date is valid or not
//    Global resource dependence : 
//    Author: liqing.peng
//    Note:
/*****************************************************************************/
BOOLEAN MMICALENDAR_IsDateValid(
                              uint16 year,
                              uint8  month,
                              uint8  day
                              );

/*****************************************************************************/
//    Description : check query date is valid or not
//    Global resource dependence : 
//    Author: yongwei.he
//    Note:
/*****************************************************************************/
BOOLEAN MMICALENDAR_IsQueryDateValid(
                              uint16 year,
                              uint8  month,
                              uint8  day
                              );

/*****************************************************************************/
//    Description : calculate date to cur date + days
//    Global resource dependence : 
//    Author: liqing.peng
//    Note:
/*****************************************************************************/
BOOLEAN MMICALENDAR_MoveDaysForward(
                                    uint16      cur_year,
                                    uint8       cur_month,
                                    uint8       cur_day,
                                    uint16      mov_days,
                                    uint16      *to_year_ptr,
                                    uint8       *to_month_ptr,
                                    uint8       *to_day_ptr
                                    );

#if defined (IM_SIMP_CHINESE_SUPPORT) || defined (IM_TRAD_CHINESE_SUPPORT)
/*****************************************************************************/
//    Description : get lunar date from solar date
//    Global resource dependence : 
//    Author: liqing.peng
//    Note:
/*****************************************************************************/
void MMICALENDAR_SolarToLunar( 
                             uint16     solar_year,
                             uint8      solar_month,
                             uint8      solar_day,
                             uint16     *lunar_year_ptr,
                             uint8      *lunar_month_ptr,
                             uint8      *lunar_day_ptr
                             );


/*****************************************************************************/
//    Description:   计算某天是否是阴历节气
//    Global resource dependence : 
//    Author: liqing.peng
//    Note:
/*****************************************************************************/
BOOLEAN MMICALENDAR_IsLunarFeast( 
                                 uint16     solar_year,
                                 uint8      solar_month,
                                 uint8      solar_day,
                                 uint8      *feast_index_ptr
                                 );

/*****************************************************************************/
//    Description:   获得阴历节气字串
//    Global resource dependence : 
//    Author: liqing.peng
//    Note:
/*****************************************************************************/
uint16* MMICALENDAR_GetLunarFeastString( uint8 feast_index);
#endif
#if defined (IM_SIMP_CHINESE_SUPPORT) || defined (IM_TRAD_CHINESE_SUPPORT)
/*****************************************************************************/
//    Description:   获得生肖字串
//    Global resource dependence : 
//    Author: liqing.peng
//    Note:
/*****************************************************************************/
uint16* MMICALENDAR_GetLunarShengxiaoString( uint16 year);
#endif
/*****************************************************************************/
//    Description:   是否闰月
//    Global resource dependence : 
//    Author: liqing.peng
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN  MMICALENDAR_IsLunarLeapMonth( void);
#if defined (IM_SIMP_CHINESE_SUPPORT) || defined (IM_TRAD_CHINESE_SUPPORT)
/*****************************************************************************/
//    Description:   计算某天是否是阴历节日
//    Global resource dependence : 
//    Author: juan.wu
//    Note:
/*****************************************************************************/
BOOLEAN MMICANLENDAR_IsLunarFestival(
                                    uint16     lular_year,
                                    uint8      lular_month,
                                    uint8      lular_date,
                                    CALENDAR_FESTIVAL_TYPE_E    *index_ptr
                                    );

/*****************************************************************************/
//    Description:   获得阴历节日字串
//    Global resource dependence : 
//    Author: juan.wu
//    Note:
/*****************************************************************************/
uint16* MMICALENDAR_GetLunarFestivalString(CALENDAR_FESTIVAL_TYPE_E index);
#endif
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif //_MMI_SOLARLUNAR_H
#endif