/*****************************************************************************
** File Name:      mmi_solarlunar.c                                          *
** Author:                                                                   *
** Date:           07/24/2006                                                *
** Copyright:      2003 Spreadtrum, Incorporated. All Rights Reserved.         *
** Description:    This file is used to describe solar lunar calendar func   *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 07/24/2006     liqing.peng      Create
** 08/09/2006     liqing.peng      Modify -- code review
******************************************************************************/



/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#ifdef CALENDAR_SUPPORT
#include "mmi_app_accessory_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "os_api.h"
#include "guicommon.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmi_solarlunar.h"

#if defined (IM_SIMP_CHINESE_SUPPORT) || defined (IM_TRAD_CHINESE_SUPPORT)
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define LUNAR_START_YEAR            MMICOM_DATE_MIN_YEAR_CALENDAR//1900    //阴历起始年份
#define LUNAR_END_YEAR              MMICOM_DATE_MAX_YEAR_CALENDAR//2100    //阴历终止年份
#endif
/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/
//month
typedef enum
{
    MONTH_JAN = 1,   //January
    MONTH_FEB,       //February
    MONTH_MAR,       //March
    MONTH_APR,       //April
    MONTH_MAY,       //May
    MONTH_JUN,       //June
    MONTH_JUL,       //July
    MONTH_AUG,       //August
    MONTH_SEP,       //September
    MONTH_OCT,       //October
    MONTH_NOV,       //November
    MONTH_DEC        //December
}DATE_MONTH_E;


/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/
//非闰年，s_gui_calendar_month_day[i]对应每月的天数
//闰年，s_gui_calendar_month_day_leap[i]对应每月的天数
LOCAL const uint8 s_gui_calendar_month_day[MONTHNUM] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
LOCAL const uint8 s_gui_calendar_month_day_leap[MONTHNUM] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};


//非闰年，假设1月1日是周日，s_gui_calendar_first_day[i]对应每月1日的星期
//闰年，假设1月1日是周日，s_gui_calendar_first_day_leap[i]对应每月1日的星期
LOCAL const uint8 s_gui_calendar_first_day[MONTHNUM] = {0, 3, 3, 6, 1, 4, 6, 2, 5, 0, 3, 5};
LOCAL const uint8 s_gui_calendar_first_day_leap[MONTHNUM] = {0, 3, 4, 0, 2, 5, 0, 3, 6, 1, 4, 6};

#if defined (IM_SIMP_CHINESE_SUPPORT) || defined (IM_TRAD_CHINESE_SUPPORT)
//数组gLunarMonthDay存入阴历1901年到2100年每年中的月天数信息，
//阴历每月只能是29或30天，一年用12（或13）个二进制位表示，对应位为1表30天，否则为29天
LOCAL const uint16 gLunarMonthDay[]=
{
  0X4b68, //1900
  0X4ae0, 0Xa570, 0X5268, 0Xd260, 0Xd950, 0X6aa8, 0X56a0, 0X9ad0, 0X4ae8, 0X4ae0,   //1910
  0Xa4d8, 0Xa4d0, 0Xd250, 0Xd548, 0Xb550, 0X56a0, 0X96d0, 0X95b0, 0X49b8, 0X49b0,   //1920
  0Xa4b0, 0Xb258, 0X6a50, 0X6d40, 0Xada8, 0X2b60, 0X9570, 0X4978, 0X4970, 0X64b0,   //1930
  0Xd4a0, 0Xea50, 0X6d48, 0X5ad0, 0X2b60, 0X9370, 0X92e0, 0Xc968, 0Xc950, 0Xd4a0,   //1940
  0Xda50, 0Xb550, 0X56a0, 0Xaad8, 0X25d0, 0X92d0, 0Xc958, 0Xa950, 0Xb4a8, 0X6ca0,   //1950
  0Xb550, 0X55a8, 0X4da0, 0Xa5b0, 0X52b8, 0X52b0, 0Xa950, 0Xe950, 0X6aa0, 0Xad50,   //1960
  0Xab50, 0X4b60, 0Xa570, 0Xa570, 0X5260, 0Xe930, 0Xd950, 0X5aa8, 0X56a0, 0X96d0,   //1970
  0X4ae8, 0X4ad0, 0Xa4d0, 0Xd268, 0Xd250, 0Xd528, 0Xb540, 0Xb6a0, 0X96d0, 0X95b0,   //1980
  0X49b0, 0Xa4b8, 0Xa4b0, 0Xb258, 0X6a50, 0X6d40, 0Xada0, 0Xab60, 0X9570, 0X4978,   //1990
  0X4970, 0X64b0, 0X6a50, 0Xea50, 0X6b28, 0X5ac0, 0Xab60, 0X9368, 0X92e0, 0Xc960,   //2000
  0Xd4a8, 0Xd4a0, 0Xda50, 0X5aa8, 0X56a0, 0Xaad8, 0X25d0, 0X92d0, 0Xc958, 0Xa950,   //2010
  0Xb4a0, 0Xb550, 0Xad50, 0X55a8, 0X4ba0, 0Xa5b0, 0X52b8, 0X52b0, 0Xa930, 0X74a8,   //2020
  0X6aa0, 0Xad50, 0X4da8, 0X4b60, 0XA570, 0Xa4e0, 0Xd260, 0Xe930, 0Xd530, 0X5aa0,   //2030
  0X6b50, 0X96d0, 0X4ae8, 0X4ad0, 0Xa4d0, 0Xd258, 0Xd250, 0Xd520, 0Xdaa0, 0Xb5a0,   //2040
  0X56d0, 0X4ad8, 0X49b0, 0Xa4b8, 0Xa4b0, 0Xaa50, 0Xb528, 0X6d20, 0Xada0, 0X55b0,   //2050
  0X9370, 0X4978, 0X4970, 0X64b0, 0X6a50, 0Xea50, 0X6b20, 0Xab60, 0Xaae0, 0X92e0,   //2060
  0Xc970, 0Xc960, 0Xd4a8, 0Xd4a0, 0Xda50, 0X5aa8, 0X56a0, 0Xa6d0, 0X52e8, 0X52d0,   //2070 
  0Xa958, 0Xa950, 0Xb4a0, 0Xb550, 0Xad50, 0X55a0, 0Xa5d0, 0Xa5b0, 0X52b0, 0Xa938,   //2080
  0X6930, 0X7298, 0X6aa0, 0Xad50, 0X4da8, 0X4b60, 0Xa570, 0X5270, 0Xd260, 0Xe930,   //2090
  0Xd520, 0Xdaa0, 0X6b50, 0X56d0, 0X4ae0, 0Xa4e8, 0Xa4d0, 0Xd150, 0Xd928, 0Xd520,   //2100
};

//数组gLanarMonth存放阴历1901年到2050年闰月的月份，如没有则为0，每字节存两年
LOCAL const uint8  gLunarMonth[]=
{
  0X08, //1900
  0X00, 0X50, 0X04, 0X00, 0X20,   //1910
  0X60, 0X05, 0X00, 0X20, 0X70,   //1920
  0X05, 0X00, 0X40, 0X02, 0X06,   //1930
  0X00, 0X50, 0X03, 0X07, 0X00,   //1940
  0X60, 0X04, 0X00, 0X20, 0X70,   //1950
  0X05, 0X00, 0X30, 0X80, 0X06,   //1960
  0X00, 0X40, 0X03, 0X07, 0X00,   //1970
  0X50, 0X04, 0X08, 0X00, 0X60,   //1980
  0X04, 0X0a, 0X00, 0X60, 0X05,   //1990
  0X00, 0X30, 0X80, 0X05, 0X00,   //2000
  0X40, 0X02, 0X07, 0X00, 0X50,   //2010
  0X04, 0X09, 0X00, 0X60, 0X04,   //2020
  0X00, 0X20, 0X60, 0X05, 0X00,   //2030
  0X30, 0Xb0, 0X06, 0X00, 0X50,   //2040
  0X02, 0X07, 0X00, 0X50, 0X03,   //2050
  0X08, 0X00, 0X60, 0X04, 0X00,   //2060
  0X30, 0X70, 0X05, 0X00, 0X40,   //2070 
  0X80, 0X06, 0X00, 0X40, 0X03,   //2080 
  0X07, 0X00, 0X50, 0X04, 0X08,   //2090 
  0X00, 0X60, 0X04, 0X00, 0X20,   //2100 
};

//数组sLunarFeast存放阴历节气
LOCAL const uint8 sLunarFeast[]=
{
0x95, 0xB4, 0x96, 0xA5, 0x96, 0x97, 0x88, 0x78, 0x78, 0x69, 0x78, 0x87, ///1900
0x96, 0xB4, 0x96, 0xA6, 0x97, 0x97, 0x78, 0x79, 0x79, 0x69, 0x78, 0x77, ///1901
0x96, 0xA4, 0x96, 0x96, 0x97, 0x87, 0x79, 0x79, 0x79, 0x69, 0x78, 0x78, ///1902
0x96, 0xA5, 0x87, 0x96, 0x87, 0x87, 0x79, 0x69, 0x69, 0x69, 0x78, 0x78, ///1903
0x86, 0xA5, 0x96, 0xA5, 0x96, 0x97, 0x88, 0x78, 0x78, 0x79, 0x78, 0x87, ///1904
0x96, 0xB4, 0x96, 0xA6, 0x97, 0x97, 0x78, 0x79, 0x79, 0x69, 0x78, 0x77, ///1905
0x96, 0xA4, 0x96, 0x96, 0x97, 0x97, 0x79, 0x79, 0x79, 0x69, 0x78, 0x78, ///1906
0x96, 0xA5, 0x87, 0x96, 0x87, 0x87, 0x79, 0x69, 0x69, 0x69, 0x78, 0x78, ///1907
0x86, 0xA5, 0x96, 0xA5, 0x96, 0x97, 0x88, 0x78, 0x78, 0x69, 0x78, 0x87, ///1908
0x96, 0xB4, 0x96, 0xA6, 0x97, 0x97, 0x78, 0x79, 0x79, 0x69, 0x78, 0x77, ///1909
0x96, 0xA4, 0x96, 0x96, 0x97, 0x97, 0x79, 0x79, 0x79, 0x69, 0x78, 0x78, ///1910
0x96, 0xA5, 0x87, 0x96, 0x87, 0x87, 0x79, 0x69, 0x69, 0x69, 0x78, 0x78, ///1911
0x86, 0xA5, 0x96, 0xA5, 0x96, 0x97, 0x88, 0x78, 0x78, 0x69, 0x78, 0x87, ///1912
0x95, 0xB4, 0x96, 0xA6, 0x97, 0x97, 0x78, 0x79, 0x79, 0x69, 0x78, 0x77, ///1913
0x96, 0xB4, 0x96, 0xA6, 0x97, 0x97, 0x79, 0x79, 0x79, 0x69, 0x78, 0x78, ///1914
0x96, 0xA5, 0x97, 0x96, 0x97, 0x87, 0x79, 0x79, 0x69, 0x69, 0x78, 0x78, ///1915
0x96, 0xA5, 0x96, 0xA5, 0x96, 0x97, 0x88, 0x78, 0x78, 0x79, 0x77, 0x87, ///1916
0x95, 0xB4, 0x96, 0xA6, 0x96, 0x97, 0x78, 0x79, 0x78, 0x69, 0x78, 0x87, ///1917
0x96, 0xB4, 0x96, 0xA6, 0x97, 0x97, 0x79, 0x79, 0x79, 0x69, 0x78, 0x77, ///1918
0x96, 0xA5, 0x97, 0x96, 0x97, 0x87, 0x79, 0x79, 0x69, 0x69, 0x78, 0x78, ///1919
0x96, 0xA5, 0x96, 0xA5, 0x96, 0x97, 0x88, 0x78, 0x78, 0x79, 0x77, 0x87, ///1920
0x95, 0xB4, 0x96, 0xA5, 0x96, 0x97, 0x78, 0x79, 0x78, 0x69, 0x78, 0x87, ///1921
0x96, 0xB4, 0x96, 0xA6, 0x97, 0x97, 0x79, 0x79, 0x79, 0x69, 0x78, 0x77, ///1922
0x96, 0xA4, 0x96, 0x96, 0x97, 0x87, 0x79, 0x79, 0x69, 0x69, 0x78, 0x78, ///1923
0x96, 0xA5, 0x96, 0xA5, 0x96, 0x97, 0x88, 0x78, 0x78, 0x79, 0x77, 0x87, ///1924
0x95, 0xB4, 0x96, 0xA5, 0x96, 0x97, 0x78, 0x79, 0x78, 0x69, 0x78, 0x87, ///1925
0x96, 0xB4, 0x96, 0xA6, 0x97, 0x97, 0x78, 0x79, 0x79, 0x69, 0x78, 0x77, ///1926
0x96, 0xA4, 0x96, 0x96, 0x97, 0x87, 0x79, 0x79, 0x79, 0x69, 0x78, 0x78, ///1927
0x96, 0xA5, 0x96, 0xA5, 0x96, 0x96, 0x88, 0x78, 0x78, 0x78, 0x87, 0x87, ///1928
0x95, 0xB4, 0x96, 0xA5, 0x96, 0x97, 0x88, 0x78, 0x78, 0x79, 0x77, 0x87, ///1929
0x96, 0xB4, 0x96, 0xA6, 0x97, 0x97, 0x78, 0x79, 0x79, 0x69, 0x78, 0x77, ///1930
0x96, 0xA4, 0x96, 0x96, 0x97, 0x87, 0x79, 0x79, 0x79, 0x69, 0x78, 0x78, ///1931
0x96, 0xA5, 0x96, 0xA5, 0x96, 0x96, 0x88, 0x78, 0x78, 0x78, 0x87, 0x87, ///1932
0x95, 0xB4, 0x96, 0xA5, 0x96, 0x97, 0x88, 0x78, 0x78, 0x69, 0x78, 0x87, ///1933
0x96, 0xB4, 0x96, 0xA6, 0x97, 0x97, 0x78, 0x79, 0x79, 0x69, 0x78, 0x77, ///1934
0x96, 0xA4, 0x96, 0x96, 0x97, 0x97, 0x79, 0x79, 0x79, 0x69, 0x78, 0x78, ///1935
0x96, 0xA5, 0x96, 0xA5, 0x96, 0x96, 0x88, 0x78, 0x78, 0x78, 0x87, 0x87, ///1936
0x95, 0xB4, 0x96, 0xA5, 0x96, 0x97, 0x88, 0x78, 0x78, 0x69, 0x78, 0x87, ///1937
0x96, 0xB4, 0x96, 0xA6, 0x97, 0x97, 0x78, 0x79, 0x79, 0x69, 0x78, 0x77, ///1938
0x96, 0xA4, 0x96, 0x96, 0x97, 0x97, 0x79, 0x79, 0x79, 0x69, 0x78, 0x78, ///1939
0x96, 0xA5, 0x96, 0xA5, 0x96, 0x96, 0x88, 0x78, 0x78, 0x78, 0x87, 0x87, ///1940
0x95, 0xB4, 0x96, 0xA5, 0x96, 0x97, 0x88, 0x78, 0x78, 0x69, 0x78, 0x87, ///1941
0x96, 0xB4, 0x96, 0xA6, 0x97, 0x97, 0x78, 0x79, 0x79, 0x69, 0x78, 0x77, ///1942
0x96, 0xA4, 0x96, 0x96, 0x97, 0x97, 0x79, 0x79, 0x79, 0x69, 0x78, 0x78, ///1943
0x96, 0xA5, 0x96, 0xA5, 0xA6, 0x96, 0x88, 0x78, 0x78, 0x78, 0x87, 0x87, ///1944
0x95, 0xB4, 0x96, 0xA5, 0x96, 0x97, 0x88, 0x78, 0x78, 0x79, 0x77, 0x87, ///1945
0x95, 0xB4, 0x96, 0xA6, 0x97, 0x97, 0x78, 0x79, 0x78, 0x69, 0x78, 0x77, ///1946
0x96, 0xB4, 0x96, 0xA6, 0x97, 0x97, 0x79, 0x79, 0x79, 0x69, 0x78, 0x78, ///1947
0x96, 0xA5, 0xA6, 0xA5, 0xA6, 0x96, 0x88, 0x88, 0x78, 0x78, 0x87, 0x87, ///1948
0xA5, 0xB4, 0x96, 0xA5, 0x96, 0x97, 0x88, 0x79, 0x78, 0x79, 0x77, 0x87, ///1949
0x95, 0xB4, 0x96, 0xA5, 0x96, 0x97, 0x78, 0x79, 0x78, 0x69, 0x78, 0x77, ///1950
0x96, 0xB4, 0x96, 0xA6, 0x97, 0x97, 0x79, 0x79, 0x79, 0x69, 0x78, 0x78, ///1951
0x96, 0xA5, 0xA6, 0xA5, 0xA6, 0x96, 0x88, 0x88, 0x78, 0x78, 0x87, 0x87, ///1952
0xA5, 0xB4, 0x96, 0xA5, 0x96, 0x97, 0x88, 0x78, 0x78, 0x79, 0x77, 0x87, ///1953
0x95, 0xB4, 0x96, 0xA5, 0x96, 0x97, 0x78, 0x79, 0x78, 0x68, 0x78, 0x87, ///1954
0x96, 0xB4, 0x96, 0xA6, 0x97, 0x97, 0x78, 0x79, 0x79, 0x69, 0x78, 0x77, ///1955
0x96, 0xA5, 0xA5, 0xA5, 0xA6, 0x96, 0x88, 0x88, 0x78, 0x78, 0x87, 0x87, ///1956
0xA5, 0xB4, 0x96, 0xA5, 0x96, 0x97, 0x88, 0x78, 0x78, 0x79, 0x77, 0x87, ///1957
0x95, 0xB4, 0x96, 0xA5, 0x96, 0x97, 0x88, 0x78, 0x78, 0x69, 0x78, 0x87, ///1958
0x96, 0xB4, 0x96, 0xA6, 0x97, 0x97, 0x78, 0x79, 0x79, 0x69, 0x78, 0x77, ///1959
0x96, 0xA4, 0xA5, 0xA5, 0xA6, 0x96, 0x88, 0x88, 0x88, 0x78, 0x87, 0x87, ///1960
0xA5, 0xB4, 0x96, 0xA5, 0x96, 0x96, 0x88, 0x78, 0x78, 0x78, 0x87, 0x87, ///1961
0x96, 0xB4, 0x96, 0xA5, 0x96, 0x97, 0x88, 0x78, 0x78, 0x69, 0x78, 0x87, ///1962
0x96, 0xB4, 0x96, 0xA6, 0x97, 0x97, 0x78, 0x79, 0x79, 0x69, 0x78, 0x77, ///1963
0x96, 0xA4, 0xA5, 0xA5, 0xA6, 0x96, 0x88, 0x88, 0x88, 0x78, 0x87, 0x87, ///1964
0xA5, 0xB4, 0x96, 0xA5, 0x96, 0x96, 0x88, 0x78, 0x78, 0x78, 0x87, 0x87, ///1965
0x95, 0xB4, 0x96, 0xA5, 0x96, 0x97, 0x88, 0x78, 0x78, 0x69, 0x78, 0x87, ///1966
0x96, 0xB4, 0x96, 0xA6, 0x97, 0x97, 0x78, 0x79, 0x79, 0x69, 0x78, 0x77, ///1967
0x96, 0xA4, 0xA5, 0xA5, 0xA6, 0xA6, 0x88, 0x88, 0x88, 0x78, 0x87, 0x87, ///1968
0xA5, 0xB4, 0x96, 0xA5, 0x96, 0x96, 0x88, 0x78, 0x78, 0x78, 0x87, 0x87, ///1969
0x95, 0xB4, 0x96, 0xA5, 0x96, 0x97, 0x88, 0x78, 0x78, 0x69, 0x78, 0x87, ///1970
0x96, 0xB4, 0x96, 0xA6, 0x97, 0x97, 0x78, 0x79, 0x79, 0x69, 0x78, 0x77, ///1971
0x96, 0xA4, 0xA5, 0xA5, 0xA6, 0xA6, 0x88, 0x88, 0x88, 0x78, 0x87, 0x87, ///1972
0xA5, 0xB5, 0x96, 0xA5, 0xA6, 0x96, 0x88, 0x78, 0x78, 0x78, 0x87, 0x87, ///1973
0x95, 0xB4, 0x96, 0xA5, 0x96, 0x97, 0x88, 0x78, 0x78, 0x69, 0x78, 0x87, ///1974
0x96, 0xB4, 0x96, 0xA6, 0x97, 0x97, 0x78, 0x79, 0x78, 0x69, 0x78, 0x77, ///1975
0x96, 0xA4, 0xA5, 0xB5, 0xA6, 0xA6, 0x88, 0x89, 0x88, 0x78, 0x87, 0x87, ///1976
0xA5, 0xB4, 0x96, 0xA5, 0x96, 0x96, 0x88, 0x88, 0x78, 0x78, 0x87, 0x87, ///1977
0x95, 0xB4, 0x96, 0xA5, 0x96, 0x97, 0x88, 0x78, 0x78, 0x79, 0x78, 0x87, ///1978
0x96, 0xB4, 0x96, 0xA6, 0x96, 0x97, 0x78, 0x79, 0x78, 0x69, 0x78, 0x77, ///1979
0x96, 0xA4, 0xA5, 0xB5, 0xA6, 0xA6, 0x88, 0x88, 0x88, 0x78, 0x87, 0x87, ///1980
0xA5, 0xB4, 0x96, 0xA5, 0xA6, 0x96, 0x88, 0x88, 0x78, 0x78, 0x77, 0x87, ///1981
0x95, 0xB4, 0x96, 0xA5, 0x96, 0x97, 0x88, 0x78, 0x78, 0x79, 0x77, 0x87, ///1982
0x95, 0xB4, 0x96, 0xA5, 0x96, 0x97, 0x78, 0x79, 0x78, 0x69, 0x78, 0x77, ///1983
0x96, 0xB4, 0xA5, 0xB5, 0xA6, 0xA6, 0x87, 0x88, 0x88, 0x78, 0x87, 0x87, ///1984
0xA5, 0xB4, 0xA6, 0xA5, 0xA6, 0x96, 0x88, 0x88, 0x78, 0x78, 0x87, 0x87, ///1985
0xA5, 0xB4, 0x96, 0xA5, 0x96, 0x97, 0x88, 0x78, 0x78, 0x79, 0x77, 0x87, ///1986
0x95, 0xB4, 0x96, 0xA5, 0x96, 0x97, 0x88, 0x79, 0x78, 0x69, 0x78, 0x87, ///1987
0x96, 0xB4, 0xA5, 0xB5, 0xA6, 0xA6, 0x87, 0x88, 0x88, 0x78, 0x87, 0x86, ///1988
0xA5, 0xB4, 0xA5, 0xA5, 0xA6, 0x96, 0x88, 0x88, 0x88, 0x78, 0x87, 0x87, ///1989
0xA5, 0xB4, 0x96, 0xA5, 0x96, 0x96, 0x88, 0x78, 0x78, 0x79, 0x77, 0x87, ///1990
0x95, 0xB4, 0x96, 0xA5, 0x86, 0x97, 0x88, 0x78, 0x78, 0x69, 0x78, 0x87, ///1991
0x96, 0xB4, 0xA5, 0xB5, 0xA6, 0xA6, 0x87, 0x88, 0x88, 0x78, 0x87, 0x86, ///1992
0xA5, 0xB3, 0xA5, 0xA5, 0xA6, 0x96, 0x88, 0x88, 0x88, 0x78, 0x87, 0x87, ///1993
0xA5, 0xB4, 0x96, 0xA5, 0x96, 0x96, 0x88, 0x78, 0x78, 0x78, 0x87, 0x87, ///1994
0x95, 0xB4, 0x96, 0xA5, 0x96, 0x97, 0x88, 0x76, 0x78, 0x69, 0x78, 0x87, ///1995
0x96, 0xB4, 0xA5, 0xB5, 0xA6, 0xA6, 0x87, 0x88, 0x88, 0x78, 0x87, 0x86, ///1996
0xA5, 0xB3, 0xA5, 0xA5, 0xA6, 0xA6, 0x88, 0x88, 0x88, 0x78, 0x87, 0x87, ///1997
0xA5, 0xB4, 0x96, 0xA5, 0x96, 0x96, 0x88, 0x78, 0x78, 0x78, 0x87, 0x87, ///1998
0x95, 0xB4, 0x96, 0xA5, 0x96, 0x97, 0x88, 0x78, 0x78, 0x69, 0x78, 0x87, ///1999
0x96, 0xB4, 0xA5, 0xB5, 0xA6, 0xA6, 0x87, 0x88, 0x88, 0x78, 0x87, 0x86, ///2000
0xA5, 0xB3, 0xA5, 0xA5, 0xA6, 0xA6, 0x88, 0x88, 0x88, 0x78, 0x87, 0x87, ///2001
0xA5, 0xB4, 0x96, 0xA5, 0x96, 0x96, 0x88, 0x78, 0x78, 0x78, 0x87, 0x87, ///2002
0x95, 0xB4, 0x96, 0xA5, 0x96, 0x97, 0x88, 0x78, 0x78, 0x69, 0x78, 0x87, ///2003
0x96, 0xB4, 0xA5, 0xB5, 0xA6, 0xA6, 0x87, 0x88, 0x88, 0x78, 0x87, 0x86, ///2004
0xA5, 0xB3, 0xA5, 0xA5, 0xA6, 0xA6, 0x88, 0x88, 0x88, 0x78, 0x87, 0x87, ///2005
0xA5, 0xB4, 0x96, 0xA5, 0xA6, 0x96, 0x88, 0x88, 0x78, 0x78, 0x87, 0x87, ///2006
0x95, 0xB4, 0x96, 0xA5, 0x96, 0x97, 0x88, 0x78, 0x78, 0x69, 0x78, 0x87, ///2007
0x96, 0xB4, 0xA5, 0xB5, 0xA6, 0xA6, 0x87, 0x88, 0x87, 0x78, 0x87, 0x86, ///2008
0xA5, 0xB3, 0xA5, 0xB5, 0xA6, 0xA6, 0x88, 0x88, 0x88, 0x78, 0x87, 0x87, ///2009
0xA5, 0xB4, 0x96, 0xA5, 0xA6, 0x96, 0x88, 0x88, 0x78, 0x78, 0x87, 0x87, ///2010
0x95, 0xB4, 0x96, 0xA5, 0x96, 0x97, 0x88, 0x78, 0x78, 0x79, 0x78, 0x87, ///2011
0x96, 0xB4, 0xA5, 0xB5, 0xA5, 0xA6, 0x87, 0x88, 0x87, 0x78, 0x87, 0x86, ///2012
0xA5, 0xB3, 0xA5, 0xB5, 0xA6, 0xA6, 0x87, 0x88, 0x88, 0x78, 0x87, 0x87, ///2013
0xA5, 0xB4, 0x96, 0xA5, 0xA6, 0x96, 0x88, 0x88, 0x78, 0x78, 0x87, 0x87, ///2014
0x95, 0xB4, 0x96, 0xA5, 0x96, 0x97, 0x88, 0x78, 0x78, 0x79, 0x77, 0x87, ///2015
0x95, 0xB4, 0xA5, 0xB4, 0xA5, 0xA6, 0x87, 0x88, 0x87, 0x78, 0x87, 0x86, ///2016
0xA5, 0xC3, 0xA5, 0xB5, 0xA6, 0xA6, 0x87, 0x88, 0x88, 0x78, 0x87, 0x87, ///2017
0xA5, 0xB4, 0xA6, 0xA5, 0xA6, 0x96, 0x88, 0x88, 0x78, 0x78, 0x87, 0x87, ///2018
0xA5, 0xB4, 0x96, 0xA5, 0x96, 0x96, 0x88, 0x78, 0x78, 0x79, 0x77, 0x87, ///2019
0x95, 0xB4, 0xA5, 0xB4, 0xA5, 0xA6, 0x97, 0x87, 0x87, 0x78, 0x87, 0x86, ///2020
0xA5, 0xC3, 0xA5, 0xB5, 0xA6, 0xA6, 0x87, 0x88, 0x88, 0x78, 0x87, 0x86, ///2021
0xA5, 0xB4, 0xA5, 0xA5, 0xA6, 0x96, 0x88, 0x88, 0x88, 0x78, 0x87, 0x87, ///2022
0xA5, 0xB4, 0x96, 0xA5, 0x96, 0x96, 0x88, 0x78, 0x78, 0x79, 0x77, 0x87, ///2023
0x95, 0xB4, 0xA5, 0xB4, 0xA5, 0xA6, 0x97, 0x87, 0x87, 0x78, 0x87, 0x96, ///2024
0xA5, 0xC3, 0xA5, 0xB5, 0xA6, 0xA6, 0x87, 0x88, 0x88, 0x78, 0x87, 0x86, ///2025
0xA5, 0xB3, 0xA5, 0xA5, 0xA6, 0xA6, 0x88, 0x88, 0x88, 0x78, 0x87, 0x87, ///2026
0xA5, 0xB4, 0x96, 0xA5, 0x96, 0x96, 0x88, 0x78, 0x78, 0x78, 0x87, 0x87, ///2027
0x95, 0xB4, 0xA5, 0xB4, 0xA5, 0xA6, 0x97, 0x87, 0x87, 0x78, 0x87, 0x96, ///2028
0xA5, 0xC3, 0xA5, 0xB5, 0xA6, 0xA6, 0x87, 0x88, 0x88, 0x78, 0x87, 0x86, ///2029
0xA5, 0xB3, 0xA5, 0xA5, 0xA6, 0xA6, 0x88, 0x88, 0x88, 0x78, 0x87, 0x87, ///2030
0xA5, 0xB4, 0x96, 0xA5, 0x96, 0x96, 0x88, 0x78, 0x78, 0x78, 0x87, 0x87, ///2031
0x95, 0xB4, 0xA5, 0xB4, 0xA5, 0xA6, 0x97, 0x87, 0x87, 0x78, 0x87, 0x96, ///2032
0xA5, 0xC3, 0xA5, 0xB5, 0xA6, 0xA6, 0x88, 0x88, 0x88, 0x78, 0x87, 0x86, ///2033
0xA5, 0xB3, 0xA5, 0xA5, 0xA6, 0xA6, 0x88, 0x78, 0x88, 0x78, 0x87, 0x87, ///2034
0xA5, 0xB4, 0x96, 0xA5, 0xA6, 0x96, 0x88, 0x88, 0x78, 0x78, 0x87, 0x87, ///2035
0x95, 0xB4, 0xA5, 0xB4, 0xA5, 0xA6, 0x97, 0x87, 0x87, 0x78, 0x87, 0x96, ///2036
0xA5, 0xC3, 0xA5, 0xB5, 0xA6, 0xA6, 0x87, 0x88, 0x88, 0x78, 0x87, 0x86, ///2037
0xA5, 0xB3, 0xA5, 0xA5, 0xA6, 0xA6, 0x88, 0x88, 0x88, 0x78, 0x87, 0x87, ///2038
0xA5, 0xB4, 0x96, 0xA5, 0xA6, 0x96, 0x88, 0x88, 0x78, 0x78, 0x87, 0x87, ///2039
0x95, 0xB4, 0xA5, 0xB4, 0xA5, 0xA6, 0x97, 0x87, 0x87, 0x78, 0x87, 0x96, ///2040
0xA5, 0xC3, 0xA5, 0xB5, 0xA5, 0xA6, 0x87, 0x88, 0x87, 0x78, 0x87, 0x86, ///2041
0xA5, 0xB3, 0xA5, 0xB5, 0xA6, 0xA6, 0x88, 0x88, 0x88, 0x78, 0x87, 0x87, ///2042
0xA5, 0xB4, 0x96, 0xA5, 0xA6, 0x96, 0x88, 0x88, 0x78, 0x78, 0x87, 0x87, ///2043
0x95, 0xB4, 0xA5, 0xB4, 0xA5, 0xA6, 0x97, 0x87, 0x87, 0x88, 0x87, 0x96, ///2044
0xA5, 0xC3, 0xA5, 0xB4, 0xA5, 0xA6, 0x87, 0x88, 0x87, 0x78, 0x87, 0x86, ///2045
0xA5, 0xB3, 0xA5, 0xB5, 0xA6, 0xA6, 0x87, 0x88, 0x88, 0x78, 0x87, 0x87, ///2046
0xA5, 0xB4, 0x96, 0xA5, 0xA6, 0x96, 0x88, 0x88, 0x78, 0x78, 0x87, 0x87, ///2047
0x95, 0xB4, 0xA5, 0xB4, 0xA5, 0xA5, 0x97, 0x87, 0x87, 0x88, 0x86, 0x96, ///2048
0xA4, 0xC3, 0xA5, 0xA5, 0xA5, 0xA6, 0x97, 0x87, 0x87, 0x78, 0x87, 0x86, ///2049
0xA5, 0xC3, 0xA5, 0xB5, 0xA6, 0xA6, 0x88, 0x88, 0x88, 0x78, 0x87, 0x86, ///2050
0xA5, 0xB3, 0xA5, 0xA5, 0xA6, 0x96, 0x88, 0x88, 0x78, 0x78, 0x87, 0x87, ///2051
0xA5, 0xB4, 0xA5, 0xB4, 0xA5, 0xA6, 0x97, 0x87, 0x87, 0x88, 0x86, 0x96, ///2052
0xA4, 0xC3, 0xA5, 0xB4, 0xA5, 0xA6, 0x87, 0x88, 0x87, 0x78, 0x87, 0x96, ///2053
0xA5, 0xC3, 0xA5, 0xB5, 0xA6, 0xA6, 0x87, 0x88, 0x88, 0x78, 0x87, 0x86, ///2054
0xA5, 0xB3, 0xA5, 0xA5, 0xA6, 0x96, 0x88, 0x88, 0x78, 0x78, 0x87, 0x87, ///2055
0xA5, 0xB4, 0xA5, 0xB4, 0xA5, 0xA6, 0x97, 0x87, 0x87, 0x87, 0x96, 0x96, ///2056
0xA4, 0xC3, 0xA5, 0xB4, 0xA5, 0xA6, 0x97, 0x87, 0x87, 0x78, 0x87, 0x96, ///2057
0xA5, 0xB3, 0xA5, 0xB5, 0xA6, 0xA6, 0x87, 0x88, 0x88, 0x78, 0x87, 0x87, ///2058
0xA5, 0xB3, 0xA5, 0xA5, 0xA6, 0x96, 0x88, 0x88, 0x88, 0x78, 0x87, 0x87, ///2059
0xA5, 0xB4, 0xA5, 0xB4, 0xA5, 0xA5, 0x97, 0x87, 0x87, 0x87, 0x96, 0x96, ///2060
0xA4, 0xC3, 0xA5, 0xB4, 0xA5, 0xA6, 0x97, 0x87, 0x87, 0x78, 0x87, 0x96, ///2061
0xA5, 0xB3, 0xA5, 0xB5, 0xA6, 0xA6, 0x87, 0x88, 0x88, 0x78, 0x87, 0x86, ///2062
0xA5, 0xB3, 0xA5, 0xA5, 0xA6, 0x96, 0x88, 0x88, 0x88, 0x78, 0x87, 0x87, ///2063
0xA5, 0xB4, 0xA5, 0xB4, 0xA5, 0xA5, 0x97, 0x87, 0x87, 0x87, 0x96, 0x96, ///2064
0xA4, 0xB3, 0xA5, 0xB4, 0xA5, 0xA6, 0x97, 0x87, 0x87, 0x78, 0x87, 0x96, ///2065
0xA5, 0xB3, 0xA5, 0xB5, 0xA6, 0xA6, 0x87, 0x88, 0x88, 0x78, 0x87, 0x86, ///2066
0xA5, 0xB3, 0xA5, 0xA5, 0xA6, 0xA6, 0x88, 0x88, 0x88, 0x78, 0x87, 0x87, ///2067
0xA5, 0xB4, 0xA5, 0xB4, 0xA5, 0xA5, 0x97, 0x87, 0x87, 0x87, 0x96, 0x96, ///2068
0xA4, 0xB3, 0xA5, 0xB4, 0xA5, 0xA6, 0x97, 0x87, 0x87, 0x78, 0x87, 0x96, ///2069
0xA4, 0xB3, 0xA5, 0xB5, 0xA6, 0xA6, 0x87, 0x88, 0x88, 0x78, 0x87, 0x86, ///2070
0xA5, 0xC3, 0xA5, 0xB5, 0xA6, 0xA6, 0x88, 0x88, 0x88, 0x78, 0x87, 0x87, ///2071
0xA5, 0xB4, 0xA5, 0xB4, 0xB5, 0xA5, 0x97, 0x87, 0x87, 0x87, 0x96, 0x96, ///2072
0xB4, 0xC3, 0xA5, 0xB4, 0xA5, 0xA6, 0x97, 0x87, 0x87, 0x88, 0x86, 0x96, ///2073
0xA4, 0xC3, 0xA5, 0xB5, 0xA6, 0xA6, 0x87, 0x88, 0x87, 0x78, 0x87, 0x86, ///2074
0xA5, 0xC3, 0xA5, 0xB5, 0xA6, 0xA6, 0x88, 0x88, 0x88, 0x78, 0x87, 0x86, ///2075
0xA5, 0xB4, 0xB5, 0xB4, 0xB5, 0xA5, 0x97, 0x97, 0x87, 0x87, 0x96, 0x96, ///2076
0xB4, 0xC3, 0xA5, 0xB4, 0xA5, 0xA6, 0x97, 0x87, 0x87, 0x88, 0x86, 0x96, ///2077
0xA4, 0xC3, 0xA5, 0xB5, 0xA5, 0xA6, 0x87, 0x88, 0x87, 0x78, 0x87, 0x96, ///2078
0xA5, 0xC3, 0xA5, 0xB5, 0xA6, 0xA6, 0x88, 0x88, 0x88, 0x78, 0x87, 0x86, ///2079
0xA5, 0xB4, 0xB4, 0xB4, 0xB5, 0xA5, 0x97, 0x97, 0x87, 0x87, 0x96, 0x96, ///2080
0xB4, 0xC3, 0xA5, 0xB4, 0xA5, 0xA6, 0x97, 0x87, 0x87, 0x88, 0x86, 0x96, ///2081
0xA4, 0xC3, 0xA5, 0xB4, 0xA5, 0xA6, 0x87, 0x88, 0x87, 0x78, 0x87, 0x96, ///2082
0xA5, 0xC3, 0xA5, 0xB5, 0xA6, 0xA6, 0x88, 0x88, 0x88, 0x78, 0x87, 0x86, ///2083
0xA5, 0xB3, 0xB4, 0xB4, 0xB5, 0xA5, 0x97, 0x97, 0x87, 0x87, 0x96, 0x96, ///2084
0xB4, 0xC3, 0xA5, 0xB4, 0xA5, 0xA6, 0x97, 0x87, 0x87, 0x88, 0x86, 0x96, ///2085
0xA4, 0xC3, 0xA5, 0xB4, 0xA5, 0xA6, 0x87, 0x88, 0x87, 0x78, 0x87, 0x96, ///2086
0xA5, 0xC3, 0xA5, 0xB5, 0xA6, 0xA6, 0x87, 0x88, 0x88, 0x78, 0x87, 0x86, ///2087
0xA5, 0xB3, 0xB4, 0xB4, 0xB5, 0xA5, 0x97, 0x97, 0x87, 0x87, 0x96, 0x96, ///2088
0xB4, 0xC3, 0xA5, 0xB4, 0xA5, 0xA6, 0x97, 0x87, 0x87, 0x87, 0x96, 0x96, ///2089
0xA4, 0xC3, 0xA5, 0xB4, 0xA5, 0xA6, 0x97, 0x87, 0x87, 0x78, 0x87, 0x96, ///2090
0xA5, 0xC3, 0xA5, 0xB5, 0xA6, 0xA6, 0x87, 0x88, 0x88, 0x78, 0x87, 0x86, ///2091
0xA5, 0xB3, 0xB4, 0xB4, 0xB5, 0xA5, 0x97, 0x97, 0x97, 0x87, 0x96, 0x96, ///2092
0xB4, 0xC3, 0xA5, 0xB4, 0xA5, 0xA5, 0x97, 0x87, 0x87, 0x78, 0x87, 0x96, ///2093
0xA4, 0xC3, 0xA5, 0xB4, 0xA5, 0xA6, 0x97, 0x87, 0x87, 0x78, 0x87, 0x96, ///2094
0xA5, 0xC3, 0xA5, 0xB5, 0xA6, 0x96, 0x87, 0x88, 0x88, 0x78, 0x87, 0x86, ///2095
0xA5, 0xB3, 0xB4, 0xB4, 0xB5, 0xB5, 0x97, 0x97, 0x97, 0x87, 0x96, 0x96, ///2096
0xB4, 0xC3, 0xA5, 0xB4, 0xA5, 0xA5, 0x97, 0x87, 0x87, 0x87, 0x96, 0x96, ///2097
0xA4, 0xC3, 0xA5, 0xB4, 0xA5, 0xA6, 0x97, 0x87, 0x87, 0x78, 0x87, 0x96, ///2098
0xA6, 0xC3, 0xA5, 0xB5, 0xA6, 0xA6, 0x87, 0x88, 0x88, 0x78, 0x87, 0x86, ///2099
0xA5, 0xB3, 0xA5, 0xA5, 0xA6, 0xA6, 0x88, 0x88, 0x88, 0x78, 0x87, 0x87, ///2100
};

LOCAL const uint16 sLunarFeaststring[][2]=
{
    0x5C0F,0x5BD2, //小寒
    0x5927,0x5BD2, //大寒
    0x7ACB,0x6625, 
    0x96E8,0x6C34, 
    0x60CA,0x86F0, 
    0x6625,0x5206, 
    0x6E05,0x660E, 
    0x8C37,0x96E8, 
    0x7ACB,0x590F, 
    0x5C0F,0x6EE1, 
    0x8292,0x79CD, 
    0x590F,0x81F3, 
    0x5C0F,0x6691, 
    0x5927,0x6691, 
    0x7ACB,0x79CB, 
    0x5904,0x6691, 
    0x767D,0x9732, 
    0x79CB,0x5206, 
    0x5BD2,0x9732, 
    0x971C,0x964D, 
    0x7ACB,0x51AC, 
    0x5C0F,0x96EA, 
    0x5927,0x96EA, 
    0x51AC,0x81F3   //冬至
};
LOCAL const uint16 sLunarFestivalstring[][3]=
{
    0x6625,0x8282,0x0020,//春节
    0x5143,0x5BB5,0x8282,//元宵节 
    0x7AEF,0x5348,0x8282,//端午节
    0x4E03,0x5915,0x0020,//七夕
    0x4E2D,0x5143,0x8282,//中元节
    0x4E2D,0x79CB,0x8282,//中秋节
    0x91CD,0x9633,0x8282,//重阳节
    0x796D,0x7956,0x8282,//祭祖节
    0x814A,0x516B,0x8282,//腊八节
    0x5C0F,0x5E74,0x0020,//小年
    0x9664,0x5915,0x0020//除夕
};

LOCAL const MMI_LUNAR_FESTIVAL_T sLunarFestivallist[] =
{
    { 1,1,1  },
    { 1,15,2 },
    { 5,5,3  },
    { 7,7,4  },
    { 7,15,5 },
    { 8,15,6 },
    { 9,9,7  },
    { 10,1,8 },
    { 12,8,9 },
    {12,23,10}
};
//鼠牛虎兔龙蛇马羊猴鸡狗猪
LOCAL const uint16 sLunarShengxiaoString[]=
{
    0x9F20,0x725B,0x864E,0x5154,0x9F99,0x86C7,0x9A6C,0x7F8A,0x7334,0x9E21,0x72D7,0x732A
};

LOCAL BOOLEAN s_is_leap_month=FALSE;
#endif
/*---------------------------------------------------------------------------*/
/*                          FUNCTION DECLARE                                 */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//    Description : calculate how many days between two dates(year, month, day)
//    Global resource dependence : 
//    Author: liqing.peng
//    Note:
/*****************************************************************************/
LOCAL uint32 CalcDaysBetweenTwoDate(
                                  uint16    start_year,
                                  uint8     start_month,
                                  uint8     start_day,
                                  uint16    end_year,
                                  uint8     end_month,
                                  uint8     end_day
                                  );

#if defined (IM_SIMP_CHINESE_SUPPORT) || defined (IM_TRAD_CHINESE_SUPPORT)
/*****************************************************************************/
//    Description:   计算指定阴历年份的日数
//    Global resource dependence : 
//    Author: liqing.peng
//    Note:
/*****************************************************************************/
LOCAL uint16 LUNAR_GetYearDays( 
                               uint16 iLunarYear 
                               );


/*****************************************************************************/
//    Description:   计算指定阴历月份的日数
//    Global resource dependence : 
//    Author: liqing.peng
//    Note:
/*****************************************************************************/
LOCAL uint32 LUNAR_GetMonthDays( 
                                uint16  iLunarYear, 
                                uint16  iLunarMonth 
                                );


/*****************************************************************************/
//    Description:   计算指定阴历年份的闰月值
//    Global resource dependence : 
//    Author: liqing.peng
//    Note:
/*****************************************************************************/
LOCAL uint8 LUNAR_GetLeapMonth( 
                               uint16  iLunarYear 
                               );


#endif
/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//    Description : 使得输入的day,在1~BIG_MONTH_DAY之间 
//    Global resource dependence : 
//    Author: murphy.xie
//    Note:
/*****************************************************************************/
LOCAL uint8 MakeDayValid(
                          uint8         day_need_check
                          )
{
    uint8   ret_valid_day = day_need_check;
    
    if(BIG_MONTH_DAY < day_need_check)
    {
        ret_valid_day = BIG_MONTH_DAY;
    }

    else if(1 > day_need_check)
    {
        ret_valid_day= 1;
    }
    
    return ret_valid_day;
    
}

/*****************************************************************************/
//    Description : 使得输入的month,在1~MONTHNUM之间 
//    Global resource dependence : 
//    Author: murphy.xie
//    Note:
/*****************************************************************************/
LOCAL uint8 MakeMonthValid(
                          uint8         month_need_check
                          )
{
    uint8   ret_valid_month = month_need_check;

    if(MONTHNUM < month_need_check)
    {
        ret_valid_month = MONTHNUM;
    }
    else if(1 > month_need_check)
    {
        ret_valid_month = 1;
    }
    
    return ret_valid_month;
    
}

#if defined (IM_SIMP_CHINESE_SUPPORT) || defined (IM_TRAD_CHINESE_SUPPORT)
/*****************************************************************************/
//    Description : 使得输入的year,在LUNAR_START_YEAR~LUNAR_END_YEAR之间 
//    Global resource dependence : 
//    Author: murphy.xie
//    Note:
/*****************************************************************************/
LOCAL uint16 MakeYearValid(
                          uint16         year_need_check
                          )
{
    uint16   ret_valid_year = year_need_check;

    if (LUNAR_START_YEAR > year_need_check)
    {
        ret_valid_year = LUNAR_START_YEAR;
    }
    else if(LUNAR_END_YEAR < year_need_check)
    {
        ret_valid_year = LUNAR_END_YEAR;
    }
    
    return ret_valid_year;
    
}
#endif
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
                          )
{
    uint8   ret_val = 0;
    uint16  year_plus = 0;   
	
    //SCI_PASSERT(1 <= month && 12 >= month, ("MMICALENDAR_GetWeekday: month = %d",month));/*assert verified*/	
    //SCI_PASSERT(1 <= day && 31 >= day, ("MMICALENDAR_GetWeekday: day = %d",day));/*assert verified*/	

    month = MakeMonthValid(month);
    day= MakeDayValid(day);

    //等于公元元年到上年12月31日所经过的天数减去年数乘上364的值, 除以7的余数就是那天的星期
    year_plus = (uint16)((year-1) + (year-1)/4 - (year-1)/100 + (year-1)/400);

    if (MMIAPICOM_IsLeapYear(year))
    {
	    ret_val = (uint8)(( day + s_gui_calendar_first_day_leap[month - 1] + year_plus ) % WEEKDAY_NUM);
    }
    else
    {
        ret_val = (uint8)(( day + s_gui_calendar_first_day[month - 1] + year_plus ) % WEEKDAY_NUM);
    }

    return ret_val;
}


/*****************************************************************************/
//    Description : calculate how many days in a month
//    Global resource dependence : 
//    Author: liqing.peng
//    Note:
/*****************************************************************************/
uint8 MMICALENDAR_GetMonthday(
                            uint16      year,
                            uint8       month
                            )
{
	uint8 ret_val = 0;

    if (month == 0 || month > MONTHNUM)
    {
        //SCI_TRACE_LOW:"MMICALENDAR_GetMonthday error month=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_SOLARLUNAR_551_112_2_18_2_0_45_0,(uint8*)"d",month);
        return 0;
    }
    if (MMIAPICOM_IsLeapYear(year))
    {
        ret_val = s_gui_calendar_month_day_leap[month - 1];
    }
    else
    {
        ret_val  = s_gui_calendar_month_day[month - 1];
    }

    return ret_val;
}

/*****************************************************************************/
//     Description : 计算某年某月第一天为星期几
//    Global resource dependence : 
//  Author: Jibin
//    Note:
/*****************************************************************************/
uint8 MMICALENDAR_GetFirstday(
                              uint16    solar_year,
                              uint8     solar_month
                              )
{
    uint8   ret_val = 0;
    uint16  year = (uint16)(solar_year-1);
    uint16  year_plus = (uint16)(year + year/4 - year/100 + year/400);   //等于公元元年到上年12月31日所经过的天数减去年数乘上364的值, 除以7的余数就是那天的星期
    
    if ((0 < solar_month) &&
        (MONTHNUM >= solar_month))
    {
        if (GUI_IsLeapYear(solar_year))
        {
            ret_val = (uint8)(( 1 + s_gui_calendar_first_day_leap[solar_month - 1] + year_plus ) % WEEKDAY_NUM);
        }
        else
        {
            ret_val = (uint8)(( 1 + s_gui_calendar_first_day[solar_month - 1] + year_plus ) % WEEKDAY_NUM);
        }
    }
    else
    {
        //SCI_TRACE_LOW:"MMICALENDAR_GetFirstday:solar_month %d is error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_SOLARLUNAR_593_112_2_18_2_0_45_1,(uint8*)"d",solar_month);
    }

    return (ret_val);
}

/*****************************************************************************/
//    Description : calculate how many days between two dates(year, month, day)
//    Global resource dependence : 
//    Author: liqing.peng
//    Note:
/*****************************************************************************/
LOCAL uint32 CalcDaysBetweenTwoDate(
                                  uint16    start_year,
                                  uint8     start_month,
                                  uint8     start_day,
                                  uint16    end_year,
                                  uint8     end_month,
                                  uint8     end_day
                                  )
{
    uint16 monthday[]={0, 31, 59 ,90, 120, 151, 181, 212, 243, 273, 304, 334}; 
    uint32 iDiffDays = 0;

    //SCI_PASSERT(start_year <= end_year, ("CalcDaysBetweenTwoDate: start_year = %d,end_year = %d",start_year,end_year));	/*assert verified*/
    //SCI_PASSERT(12 >= start_month, ("CalcDaysBetweenTwoDate: start_month = %d",start_month));/*assert verified*/
    //SCI_PASSERT(12 >= end_month, ("CalcDaysBetweenTwoDate: end_month = %d",end_month));/*assert verified*/
    //SCI_PASSERT(31 >= start_day, ("CalcDaysBetweenTwoDate: start_day = %d",start_day));/*assert verified*/
    //SCI_PASSERT(31 >= end_day, ("CalcDaysBetweenTwoDate: end_day = %d",end_day));/*assert verified*/

    if(start_year > end_year)
    {
        //exchange 
        //SCI_TRACE_LOW:"CalcDaysBetweenTwoDate: start_year,end_year = %d.%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_SOLARLUNAR_624_112_2_18_2_0_45_2,(uint8*)"dd",start_year,end_year);
        start_year = start_year + end_year;
        end_year = start_year - end_year;
        start_year = start_year - end_year;
    }
    
    start_month = MakeMonthValid(start_month);
    end_month = MakeMonthValid(end_month);
    
    start_day = MakeDayValid(start_day);
    end_day = MakeDayValid(end_day);
    
    //计算与起始年份1月1日之间相差的天数
    if(start_year < end_year)
    {
        iDiffDays = ( end_year - start_year ) * 365;
        iDiffDays += ( end_year - 1 ) / 4 - ( start_year - 1 ) / 4;
        iDiffDays -= (( end_year - 1 ) / 100 - ( start_year - 1 ) / 100 );
        iDiffDays += ( end_year - 1 ) / 400 - ( start_year - 1 ) / 400;
    }

    //加上iEndYear年1月1日到iEndMonth月iEndDay日之间的天数
    if (end_month == 0 )
    {
        //SCI_TRACE_LOW:"CalcDaysBetweenTwoDate error end_month=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_SOLARLUNAR_648_112_2_18_2_0_45_3,(uint8*)"d",end_month);
        return 0;
    }
    iDiffDays += monthday[end_month-1] + ( MMIAPICOM_IsLeapYear( end_year ) && end_month > 2 ? 1 : 0 );
    iDiffDays += end_day;

    //减去iStartYear年1月1日到iStartMonth月iStartDay日之间的天数
    if (start_month  ==0 )
    {
        //SCI_TRACE_LOW:"CalcDaysBetweenTwoDate error start_month=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_SOLARLUNAR_657_112_2_18_2_0_45_4,(uint8*)"d",start_month);
        return 0;
    }
    iDiffDays -= monthday[start_month-1] + ( MMIAPICOM_IsLeapYear( start_year  )&& start_month > 2 ? 1 : 0 );
    iDiffDays -= start_day;

    return iDiffDays;
}


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
                              )
{
    uint16  days_betw = 0;
    uint8   week_day_begin = 0;
    uint8   week_day_now = 0;
    uint8   weeks = 0;

    days_betw = CalcDaysBetweenTwoDate(year, 1, 1,
                                     year, month, day);
    
    weeks= days_betw/WEEKDAY_NUM + 1;
    if(0 != days_betw%WEEKDAY_NUM)
    {
        week_day_begin = MMICALENDAR_GetWeekday(year, 1, 1);
        week_day_now = MMICALENDAR_GetWeekday(year, month, day);

        if(week_day_now < week_day_begin)
        {
            weeks++;
        }
    }

    return weeks;
}


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
                              )
{
    BOOLEAN result = FALSE;

    if( (QUERY_YEAR_BOTTOM <= year && QUERY_YEAR_TOP >= year) &&
        (0 < month && MONTHNUM >= month) &&
        (0 < day))
    {
        switch (month)
        {
        case MONTH_JAN:
        case MONTH_MAR:
        case MONTH_MAY:
        case MONTH_JUL:
        case MONTH_AUG:
        case MONTH_OCT:
        case MONTH_DEC:
            //check big month
            if (BIG_MONTH_DAY >= day)
            {
                result = TRUE;
            }
            break;

        case MONTH_APR:
        case MONTH_JUN:
        case MONTH_SEP:
        case MONTH_NOV:
            //check small month
            if (SMALL_MONTH_DAY >= day)
            {
                result = TRUE;
            }
            break;

        case MONTH_FEB:
            if (MMIAPICOM_IsLeapYear(year))
            {
                //check leap month
                if (LEAP_FEB_MONTH_DAY >= day)
                {
                    result = TRUE;
                }
            }
            else
            {
                //check not leap month
                if (NOT_LEAP_FEB_MONTH_DAY >= day)
                {
                    result = TRUE;
                }
            }
            break;

        default:
            //SCI_TRACE_LOW:"MMICALENDAR_IsDateValid: month is %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_SOLARLUNAR_764_112_2_18_2_0_46_5,(uint8*)"d",month);
            break;
        }
    }

    return result;
}
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
                              )
{
    BOOLEAN result = FALSE;

    if( (QUERY_YEAR_BOTTOM <= year && QUERY_YEAR_TOP >= year) &&
        (0 < month && MONTHNUM >= month) &&
        (0 < day))
    {
        switch (month)
        {
        case MONTH_JAN:
        case MONTH_MAR:
        case MONTH_MAY:
        case MONTH_JUL:
        case MONTH_AUG:
        case MONTH_OCT:
        case MONTH_DEC:
            //check big month
            if (BIG_MONTH_DAY >= day)
            {
                result = TRUE;
            }
            break;

        case MONTH_APR:
        case MONTH_JUN:
        case MONTH_SEP:
        case MONTH_NOV:
            //check small month
            if (SMALL_MONTH_DAY >= day)
            {
                result = TRUE;
            }
            break;

        case MONTH_FEB:
            if (MMIAPICOM_IsLeapYear(year))
            {
                //check leap month
                if (LEAP_FEB_MONTH_DAY >= day)
                {
                    result = TRUE;
                }
            }
            else
            {
                //check not leap month
                if (NOT_LEAP_FEB_MONTH_DAY >= day)
                {
                    result = TRUE;
                }
            }
            break;

        default:
            //SCI_TRACE_LOW:"MMICALENDAR_IsQueryDateValid: month is %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_SOLARLUNAR_834_112_2_18_2_0_46_6,(uint8*)"d",month);
            break;
        }
    }

    return result;
}

/*****************************************************************************/
//    Description : 计算给定日期处于一年中的第几天
//    Global resource dependence : 
//    Author: liqing.peng
//    Note:
/*****************************************************************************/
LOCAL uint16 GetDaysInYear(
                         uint16     year,
                         uint8      month,
                         uint8      day
                         )
{
    uint8       past_month = 0;
    uint16      days = 0;
    uint32      i = 0;

    //SCI_PASSERT(0 < month && MONTHNUM >= month, ("GetDaysInYear: month is %d",month));/*assert verified*/
    //SCI_PASSERT(0 < day && BIG_MONTH_DAY >= day, ("GetDaysInYear: day is %d",day));/*assert verified*/
    month = MakeMonthValid(month);
    day = MakeDayValid(day);
    
    past_month = month-1;
    
    for(i=0; i<past_month; i++)
    {
        days = days + s_gui_calendar_month_day[i];
    }

    if(MMIAPICOM_IsLeapYear(year) && 2 <= past_month)
    {
        days =days+1;
    }

    days = days + day;

    return days;
}


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
                                    )
{
    BOOLEAN     result = TRUE;
    uint16      days_to_mov = 0;
    uint16      a_year = 0;
    uint8       a_month = 0;
    uint16      days_of_year = 0;
    uint16      days_of_month = 0;

    //SCI_PASSERT(QUERY_YEAR_TOP >= cur_year, ("MMICALENDAR_MoveDaysForward: cur_year is %d",cur_year));/*assert verified*/
    //SCI_PASSERT(MONTHNUM >= cur_month, ("MMICALENDAR_MoveDaysForward: cur_month is %d",cur_month));/*assert verified*/
    //SCI_PASSERT(BIG_MONTH_DAY >= cur_day, ("MMICALENDAR_MoveDaysForward: cur_day is %d",cur_day));/*assert verified*/
    //SCI_ASSERT(NULL != to_year_ptr);/*assert verified*/
    //SCI_ASSERT(NULL != to_month_ptr);/*assert verified*/
    //SCI_ASSERT(NULL != to_day_ptr);/*assert verified*/
    if(PNULL == to_year_ptr || PNULL == to_month_ptr || PNULL == to_day_ptr)
    {
        //SCI_TRACE_LOW:"MMICALENDAR_MoveDaysForward: PNULL == to_year_ptr or to_month_ptr or to_day_ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_SOLARLUNAR_908_112_2_18_2_0_46_7,(uint8*)"");
        return FALSE;
    }

    if(QUERY_YEAR_TOP < cur_year)
    {
        //SCI_TRACE_LOW:"MMICALENDAR_MoveDaysForward: cur_year = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_SOLARLUNAR_914_112_2_18_2_0_46_8,(uint8*)"d",cur_year);
        cur_year = QUERY_YEAR_TOP;
    }
    
    cur_month = MakeMonthValid(cur_month);
    cur_day = MakeDayValid(cur_day);


    days_to_mov = mov_days;
    if(MMIAPICOM_IsLeapYear(cur_year))
    {
        days_of_year = DAYS_OF_LEAP_YEAR;
    }
    else
    {
        days_of_year = DAYS_OF_NORMAL_YEAR;
    }

    if(GetDaysInYear(cur_year, cur_month, cur_day)+days_to_mov <= days_of_year)
    {
        //to date is in current year
        *to_year_ptr = cur_year;

        days_of_month = MMICALENDAR_GetMonthday(cur_year, cur_month);
        if(cur_day+days_to_mov <= days_of_month)
        {
            //to date is in current month
            *to_month_ptr = cur_month;
            *to_day_ptr = cur_day+days_to_mov;
        }
        else
        {
            //to date is not in current month
            days_to_mov = days_to_mov - (days_of_month-cur_day);

            for(a_month = cur_month+1; a_month<=MONTHNUM; a_month++)
            {
                days_of_month = MMICALENDAR_GetMonthday(cur_year, a_month);
                if(days_to_mov > days_of_month)
                {
                    // days_to_mov = days_to_mov-days_of_month;
                }
                else
                {
                    break;
                }
            }

            //SCI_PASSERT(MONTHNUM >= a_month, ("MMICALENDAR_MoveDaysForward: a_month is %d",a_month));/*assert verified*/
            a_month = MakeMonthValid(a_month);
            *to_month_ptr = a_month;
            *to_day_ptr = days_to_mov;
        }
    }
    else
    {
        //to date is not in current year
        days_to_mov = days_to_mov - (days_of_year-GetDaysInYear(cur_year, cur_month, cur_day));

        for(a_year=cur_year+1; a_year<=QUERY_YEAR_TOP; a_year++)
        {
            if(MMIAPICOM_IsLeapYear(a_year))
            {
                days_of_year = DAYS_OF_LEAP_YEAR;
            }
            else
            {
                days_of_year = DAYS_OF_NORMAL_YEAR;
            }

            if(days_to_mov > days_of_year)
            {
                // days_to_mov = days_to_mov-days_of_year;
            }
            else
            {
                break;
            }
        }

        if(QUERY_YEAR_TOP < a_year)
        {
            result = FALSE;
        }
        else
        {
            *to_year_ptr = a_year;

            for(a_month=1; a_month<=MONTHNUM; a_month++)
            {
                days_of_month = MMICALENDAR_GetMonthday(a_year, a_month);
                if(days_to_mov > days_of_month)
                {
                    // days_to_mov = days_to_mov-days_of_month;
                }
                else
                {
                    break;
                }
            }

            //SCI_PASSERT(MONTHNUM >= a_month, ("MMICALENDAR_MoveDaysForward: a_month is %d",a_month));/*assert verified*/
            a_month = MakeMonthValid(a_month);
            *to_month_ptr = a_month;
            *to_day_ptr = days_to_mov;
        }
    }
    
    return result;
}

#if defined (IM_SIMP_CHINESE_SUPPORT) || defined (IM_TRAD_CHINESE_SUPPORT)
/*****************************************************************************/
//    Description:   计算指定阴历年份的闰月值
//    Global resource dependence : 
//    Author: liqing.peng
//    Note:
/*****************************************************************************/
LOCAL uint8 LUNAR_GetLeapMonth( 
                               uint16       iLunarYear 
                               )
{
    uint8    flag = 0;

    //SCI_PASSERT(LUNAR_START_YEAR <= iLunarYear && LUNAR_END_YEAR >= iLunarYear, ("LUNAR_GetLeapMonth: iLunarYear is %d",iLunarYear));	/*assert verified*/
    iLunarYear = MakeYearValid(iLunarYear);

    //@yongwei.he modify
    flag = gLunarMonth[(iLunarYear - LUNAR_START_YEAR + 1)/2];
    return (uint8)(( iLunarYear - LUNAR_START_YEAR ) % 2 ? flag >> 4 : flag&0x0f);
}


/*****************************************************************************/
//    Description:   计算指定阴历月份的日数
//    Global resource dependence : 
//    Author: liqing.peng
//    Note:
/*****************************************************************************/
LOCAL uint32 LUNAR_GetMonthDays( 
                                uint16          iLunarYear, 
                                uint16          iLunarMonth 
                                )
{
    uint16      high = 0;
    uint16      low = 0;
    int         iBit = 16 - iLunarMonth;
    uint8       leap_month = 0;

    //SCI_PASSERT(LUNAR_START_YEAR <= iLunarYear && LUNAR_END_YEAR >= iLunarYear, ("LUNAR_GetMonthDays: iLunarYear is %d",iLunarYear));	/*assert verified*/
    iLunarYear = MakeYearValid(iLunarYear);
    
    leap_month = LUNAR_GetLeapMonth( iLunarYear );
    if ( leap_month && iLunarMonth > leap_month ) 
    {
        iBit --;
    }

    if ( gLunarMonthDay[iLunarYear - LUNAR_START_YEAR] & ( 1 << iBit ))
    {
        low = 30;
    }
    else
    {
        low = 29;
    }

    if ( iLunarMonth == leap_month) 
    {
         if ( gLunarMonthDay[iLunarYear - LUNAR_START_YEAR] & ( 1 << ( iBit -1 )))
         {
            high =30;
         }
         else
         {
            high =29;
         }
    }

    return MMICOM_MAKELONG(low, high);
}


/*****************************************************************************/
//    Description:   计算指定阴历年份的日数
//    Global resource dependence : 
//    Author: liqing.peng
//    Note:
/*****************************************************************************/
LOCAL uint16 LUNAR_GetYearDays( 
                               uint16   iLunarYear 
                               )
{
    uint32    tmp = 0;
    uint16    i = 0;
    uint16    days = 0;

    //SCI_PASSERT(LUNAR_START_YEAR <= iLunarYear && LUNAR_END_YEAR >= iLunarYear, ("LUNAR_GetYearDays: iLunarYear is %d",iLunarYear));	/*assert verified*/
    iLunarYear = MakeYearValid(iLunarYear);

    for ( i = 1; i <= 12; i++ ) 
    { 
        tmp = LUNAR_GetMonthDays( iLunarYear, i ); 
        days += MMICOM_HIWORD(tmp);
        days += MMICOM_LOWORD(tmp);
    }

    return days;
}
#endif
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
                             )
{
    uint32   iSpanDays = 0;
    uint32   tmp = 0;
    uint16   iYear = 0;
    uint16   iMonth = 0;
    uint16   iDay = 0;

    //SCI_PASSERT(LUNAR_START_YEAR <= solar_year, ("MMICALENDAR_SolarToLunar: solar_year is %d",solar_year));	/*assert verified*/
    solar_year = MakeYearValid(solar_year);

    if(NULL == lunar_year_ptr ||  NULL == lunar_month_ptr || NULL == lunar_day_ptr)
    {
        //SCI_TRACE_LOW:"MMI_SOLARLUNAR.C MMICALENDAR_SolarToLunar *year=%x, *moth=%x, *day=%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_SOLARLUNAR_1144_112_2_18_2_0_46_9,(uint8*)"ddd",lunar_year_ptr, lunar_month_ptr, lunar_day_ptr);
        return;
    }  
    s_is_leap_month=FALSE;
    iSpanDays = CalcDaysBetweenTwoDate(LUNAR_START_YEAR, 1, 1,
                                     solar_year, solar_month, solar_day);
/*
    //阳历1901年2月19日为阴历1901年正月初一
    //阳历1901年1月1日到2月19日共有49天
    if ( iSpanDays < 49 ) 
    {
        *lunar_year_ptr = LUNAR_START_YEAR - 1;
        if ( iSpanDays < 19 ) 
        { 
            *lunar_month_ptr = 11;  
            *lunar_day_ptr   = 11 + iSpanDays;
        } 
        else 
        {
            *lunar_month_ptr = 12;
            *lunar_day_ptr   = iSpanDays -18;
        }
        return;
    }
  
    //下面从阴历1901年正月初一算起
    iSpanDays -= 49;
    iYear      = LUNAR_START_YEAR;
    iMonth     = 1;
    iDay       = 1;
*/
    //阳历1900年1月31日为阴历1900年正月初一
    //阳历1900年1月1日到1月31日共有30天
	if ( iSpanDays < 30 ) 
    {
         *lunar_year_ptr = LUNAR_START_YEAR - 1;
         *lunar_month_ptr = 12;  
         *lunar_day_ptr   = 1 + iSpanDays;
		return;
    }
  
    //下面从阴历1900年正月初一算起
    iSpanDays -= 30;
    iYear      = LUNAR_START_YEAR;
    iMonth     = 1;
    iDay       = 1;	
    //计算年
    tmp = LUNAR_GetYearDays( iYear ); 
    while( iSpanDays >= tmp )
    {
        iSpanDays -= tmp;
        tmp        = LUNAR_GetYearDays( ++iYear );
    }

    //计算月
    tmp = MMICOM_LOWORD(LUNAR_GetMonthDays( iYear, iMonth ));
    while( iSpanDays >= tmp )
    {
        iSpanDays -= tmp;
        if ( iMonth == LUNAR_GetLeapMonth( iYear )) 
        {
            tmp  = MMICOM_HIWORD(LUNAR_GetMonthDays( iYear, iMonth ));
            if ( iSpanDays < tmp )
            {
                s_is_leap_month=TRUE;
                break;
            }
            iSpanDays -= tmp;
        }
        tmp = MMICOM_LOWORD(LUNAR_GetMonthDays( iYear, ++iMonth ));
    }

    //计算日
    iDay += iSpanDays;

    //返回
    *lunar_year_ptr  = iYear;
    *lunar_month_ptr = iMonth;
    *lunar_day_ptr   = iDay;

    return;
}
#endif

#if defined (IM_SIMP_CHINESE_SUPPORT) || defined (IM_TRAD_CHINESE_SUPPORT)
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
                                 )
{
    BOOLEAN     is_feast = FALSE;
    uint8       feast_month = 0;
    uint8       feast_day = 0;
    int32       offset = 0;

    if(NULL == feast_index_ptr)
    {
        //SCI_TRACE_LOW:"MMI_SOLARLUNAR.C MMICALENDAR_IsLunarFeast *feast_index_ptr=%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_SOLARLUNAR_1245_112_2_18_2_0_47_10,(uint8*)"d",feast_index_ptr);
        return FALSE;
    }
    offset = (solar_year-LUNAR_START_YEAR)*12+solar_month-1;
    if (offset < 0)
    {
        //SCI_TRACE_LOW:"MMICALENDAR_IsLunarFeast error offset=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_SOLARLUNAR_1251_112_2_18_2_0_47_11,(uint8*)"d",offset);
        return is_feast;
    }
    feast_month = sLunarFeast[(solar_year-LUNAR_START_YEAR)*12+solar_month-1];

    if(15 > solar_day)
    {
        feast_day = (feast_month>>4)&0x0f;
        feast_day = 15 - feast_day;
        *feast_index_ptr = (solar_month-1)*2+1;
    }
    else
    {
        feast_day = feast_month&0x0f;
        feast_day = 15 + feast_day;
        *feast_index_ptr = (solar_month-1)*2+2;
    }

    if(solar_day == feast_day)
    {
        is_feast = TRUE;
    }

    return is_feast;
}
#endif
#if defined (IM_SIMP_CHINESE_SUPPORT) || defined (IM_TRAD_CHINESE_SUPPORT)
/*****************************************************************************/
//    Description:   获得阴历节气字串
//    Global resource dependence : 
//    Author: liqing.peng
//    Note:
/*****************************************************************************/
uint16* MMICALENDAR_GetLunarFeastString( uint8 feast_index)
{
    if(feast_index<1 || feast_index >24)
    {
        return PNULL;
    }
    else
    {
        return sLunarFeaststring[feast_index-1];/*lint !e605*/
    }
}
#endif
#if defined (IM_SIMP_CHINESE_SUPPORT) || defined (IM_TRAD_CHINESE_SUPPORT)
/*****************************************************************************/
//    Description:   获得生肖字串
//    Global resource dependence : 
//    Author: liqing.peng
//    Note:
/*****************************************************************************/
uint16* MMICALENDAR_GetLunarShengxiaoString( uint16 year)
{
    uint8 index=(year-4)%12;

    return &sLunarShengxiaoString[index];/*lint !e605*/
}

/*****************************************************************************/
//    Description:   是否闰月
//    Global resource dependence : 
//    Author: liqing.peng
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN  MMICALENDAR_IsLunarLeapMonth( void)
{
    return s_is_leap_month;
}
#endif
#if defined (IM_SIMP_CHINESE_SUPPORT) || defined (IM_TRAD_CHINESE_SUPPORT)
/*****************************************************************************/
//    Description:   计算某天是否是阴历节日
//    Global resource dependence : 
//    Author: juan.wu
//    Note:
/*****************************************************************************/
BOOLEAN MMICANLENDAR_IsLunarFestival(
                                    uint16     lunar_year,
                                    uint8      lunar_month,
                                    uint8      lunar_date,
                                    CALENDAR_FESTIVAL_TYPE_E      *index_ptr
                                    )
{
    uint32  month_days = 0;
    BOOLEAN result = FALSE;
    CALENDAR_FESTIVAL_TYPE_E i = 0;
    
    //除夕有点特殊，单独写
    if(12 == lunar_month)
    {
        month_days = LUNAR_GetMonthDays(lunar_year, lunar_month);
        if(lunar_date == month_days)
        {
            *index_ptr = CALENDAR_CHUXI;
            return TRUE;
        }

    }
    
    for(i=0;i<(CALENDAR_FESTIVAL_MAX -1);i++)
    {
        if((lunar_month == sLunarFestivallist[i].month) && (lunar_date == sLunarFestivallist[i].date))
        {
            *index_ptr = i;
            result = TRUE;
            break;
        }
    }
        
    return result;
}
#endif
#if defined (IM_SIMP_CHINESE_SUPPORT) || defined (IM_TRAD_CHINESE_SUPPORT)
/*****************************************************************************/
//    Description:   获得阴历节日字串
//    Global resource dependence : 
//    Author: juan.wu
//    Note:
/*****************************************************************************/
uint16* MMICALENDAR_GetLunarFestivalString(CALENDAR_FESTIVAL_TYPE_E index)
{
    if(index > CALENDAR_CHUXI)
    {
        return PNULL;
    }
    else
    {
        return sLunarFestivalstring[index];/*lint !e605*/
    }
}
#endif
#endif
