/******************************************************************************
 ** File Name:      mn_gps_signal_def.h                                                 *
 ** Author:         CUIFANG DENG                                              *
 ** Date:           05/31/2017                                               *
 ** Copyright:      Copyright 2001-2017 by Spreadtrum Communications,Inc.     *
 **                 All Rights Reserved.                                      *
 **                 This software is supplied under the terms of a license    *
 **                 agreement or non-disclosure agreement with Spreadtrum.    *
 **                 Passing on and copying of this document,and communication *
 **                 of its contents is not permitted without prior written    *
 **                 authorization.                                            *
 ** Description:                                                              *                                                      
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** --------------------------------------------------------------------------*
 ** Date           Name             Description                               *
 **---------------------------------------------------------------------------*
 ** 05/31/2017     cuifang.deng      Create.                                   *
 ******************************************************************************/
//#ifdef _MNGPS_SUPPORT_
#if !defined (_MN_GPS_SIGNAL_DEF_H)
#define _MN_GPS_SIGNAL_DEF_H

#include "sci_types.h"
#include "os_api.h"
#include "mn_type.h"

typedef enum
{
    MN_GPS_NOT_RUN,   
    MN_GPS_NOT_FIX,      
    MN_GPS_2D_FIX,    
    MN_GPS_3D_FIX
} MN_GPS_STATUS_E;

typedef enum 
{
    MN_GPS_COLD_MODE,      //冷启动
    MN_GPS_HOT_MODE,       //热启动
    MN_GPS_WARM_MODE    //温启动
} MN_GPS_RESET_MODE_E;

// following signal only have signal header
typedef xSignalHeaderRec  SIGNALS_NO_PARA_T;

typedef struct 
{
    SIGNAL_VARS
    MN_GPS_RESET_MODE_E     reset_mode;
} MN_GPS_RESET_REQ_T;


typedef struct
{
    SIGNAL_VARS
    MN_GPS_STATUS_E     status;
} MN_GPS_STATUS_CNF_T;

typedef struct 
{
    SIGNAL_VARS
    uint16          type; // value 0- disable ouput;  BIT0=1--GGA,  BIT1=1--GLL,  BIT2=1--GSA,  BIT3=1--GSV,  BIT4=1--RMC, BIT5=1--VTG
    uint32          time_interval;
} MN_GPS_OUTPUT_REQ_T;

typedef struct 
{
    SIGNAL_VARS
    MN_DUAL_SYS_E dual_sys;
    uint8     netid; //value 5~11.   0:invalid
} MN_GPS_PDP_INFO_REQ_T;

typedef struct 
{
    SIGNAL_VARS
    MN_DUAL_SYS_E dual_sys;
    uint8  type;
} MN_GPS_ASSERT_REQ_T;

typedef struct
{
    SIGNAL_VARS
    BOOLEAN     is_ok;
}MN_GPS_DOWNLOAD_CNF_T,
MN_GPS_INIT_CNF_T,
MN_GPS_START_CNF_T, 
MN_GPS_STOP_CNF_T, 
MN_GPS_RESET_CNF_T,
MN_GPS_OUTPUT_CNF_T,
MN_GPS_POWEROFF_CNF_T;

typedef SIGNALS_NO_PARA_T  MN_GPS_ERR_IND_T;

typedef struct 
{
    SIGNAL_VARS
    MN_GPS_STATUS_E     fix_status;   //定位状态
} MN_GPS_READ_STATUS_CNF_T;

typedef struct {
    uint16      wYear;
    uint16      wMonth;
    uint16      wDay;
    uint16      wHour;
    uint16      wMinute;
    uint16      wSecond;
} TimeStamp;

typedef struct 
{
    float       longitude;  //经度
    float       latitude;   //纬度
    float       altitude;  //高度
    TimeStamp      utc_time;  // 世界时间
    uint32       ttff_time; //第一次定位所用时间，单位s
    uint8         satellite_num; //卫星数目
    float       speed;    //对地速度
    float       course;   //对地方向
} MN_GPS_INFO_T;


typedef struct
{
    SIGNAL_VARS
    MN_GPS_INFO_T       gps_info;
} MN_GPS_READ_INFO_CNF_T;


typedef struct 
{
    SIGNAL_VARS
    uint16     length;
    uint16     type; // BIT0=1--GGA,  BIT1=1--GLL,  BIT2=1--GSA,  BIT3=1--GSV,  BIT4=1--RMC, BIT5=1--VTG
    uint8       nmea_data[1024]; //NMEA格式数据
} MN_GPS_OUTPUT_INFO_IND_T;

typedef struct
{
    SIGNAL_VARS
    float       longitude;  //经度
    float       latitude;   //纬度
    uint32       ttff_time; //第一次定位所用时间，单位s
} MN_GPS_FIX_IND_T;

#endif /* end of _MN_GPS_SIGNAL_DEF_H */
//#endif
