/*****************************************************************************
** Copyright 2021-2022 Unisoc(Shanghai) Technologies Co.,Ltd                 *
** Licensed under the Unisoc General Software License,                       *
** version 1.0 (the License);                                                *
** you may not use this file except in compliance with the License.          *
** You may obtain a copy of the License at                                   *
** https://www.unisoc.com/en_us/license/UNISOC_GENERAL_LICENSE_V1.0-EN_US    *
** Software distributed under the License is distributed on an "AS IS" BASIS,*
** WITHOUT WARRANTIES OF ANY KIND, either express or implied.                *
** See the Unisoc General Software License, version 1.0 for more details.    *
******************************************************************************/

/*****************************************************************************
** File Name:       ual_gnss.h                                               *
** Author:           zirui.li                                                *
** Date:            09/07/2022                                               *
** Description:    This file is used to define ual gnss export               *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE                 NAME                  DESCRIPTION                    *
** 09/07/2022         zirui.li                  Create                       *
******************************************************************************/
#ifndef _UAL_GNSS_H
#define _UAL_GNSS_H
/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "ual_type.h"
#include "os_api.h"
#include "ual_cms.h"
/*---------------------------------------------------------------------------*/
/*                          MACRO DEFINITION                                 */
/*---------------------------------------------------------------------------*/
/*nmea数据包的最大长度*/
#define UAL_GNSS_NMEA_DATA_MAX_SIZE  3072
/*一个星座下卫星的最大数量*/
#define UAL_GNSS_SATELLITE_MAX_NUM  32
/*---------------------------------------------------------------------------*/
/*                          ENUM AND STRUCT                                  */
/*---------------------------------------------------------------------------*/

/*定位模式*/
typedef enum
{
    UAL_GNSS_MODE_NONE,
    UAL_GNSS_MODE_GPS,                     //全球定位系统
    UAL_GNSS_MODE_BDS,                     //北斗卫星导航系统
    UAL_GNSS_MODE_GPS_BDS,                //全球定位系统和北斗卫星导航系统
    UAL_GNSS_MODE_GLONASS,                //格洛纳斯卫星导航系统
    UAL_GNSS_MODE_GPS_GLONASS,            //全球定位和格洛纳斯卫星
    UAL_GNSS_MODE_GPS_BDS_GALILEO = 19,   //全球定位、北斗卫星、伽利略卫星
    UAL_GNSS_MODE_GPS_B1C_GLONASS_GALILEO = 29,  //四模:全球定位、北斗卫星、伽利略卫星、格洛纳斯卫星
    UAL_GNSS_MODE_MAX
}ual_gnss_mode_e;

/*启动模式*/
typedef enum
{
    UAL_GNSS_START_MODE_HOT,               //热启动
    UAL_GNSS_START_MODE_COLD,              //冷启动
    UAL_GNSS_START_MODE_WARM,              //温启动
    UAL_GNSS_START_MODE_FACTORY,           //工厂模式
    UAL_GNSS_START_MODE_MAX
}ual_gnss_start_mode_e;

/*NMEA格式类型*/
typedef enum
{
    UAL_GNSS_NMEA_NONE_TYPE = 0,         //disable output
    UAL_GNSS_NMEA_GGA_TYPE = 0x01,       //GGS
    UAL_GNSS_NMEA_GLL_TYPE = 0x02,       //GLL
    UAL_GNSS_NMEA_GSA_TYPE = 0x04,       //GSA
    UAL_GNSS_NMEA_GSV_TYPE = 0x08,       //GSV
    UAL_GNSS_NMEA_RMC_TYPE = 0x10,       //RMC
    UAL_GNSS_NMEA_VTG_TYPE = 0x20,       //VTG
    UAL_GNSS_NMEA_ALL_TYPE = 0x3F        //以上所有类型
}ual_gnss_nmea_type_e;

/*ual gnss状态机的状态*/
typedef enum
{
    UAL_GNSS_STATE_IDLE,    //IDLE状态
    UAL_GNSS_STATE_READY,   //READY状态，初始化完成
    UAL_GNSS_STATE_WORK,    //work状态，正在定位
    UAL_GNSS_STATE_MAX
}ual_gnss_state_e;

/*ual gnss向上层上报的错误码*/
typedef enum
{
    UAL_GNSS_RES_SUCCESS,             //成功
    UAL_GNSS_RES_PARAM_ERR = UAL_MODULE_GNSS << 16,    //参数错误
    UAL_GNSS_RES_REGISTER_CMS_FAIL,    //注册cms失败
    UAL_GNSS_RES_UNREGISTER_CMS_FAIL,  //去注册cms失败
    UAL_GNSS_RES_CREATE_FSM_FAIL,     //创建状态机失败
    UAL_GNSS_RES_DESTROY_FSM_FAIL,     //销毁状态机失败
    UAL_GNSS_RES_STATE_ERR,           //状态错误(携带当前状态)
    UAL_GNSS_RES_HAVE_DONE,           //相应的请求已经做过且完成了
    UAL_GNSS_RES_OTHER_ERR,           //其他错误
    UAL_GNSS_RES_MAX
}ual_gnss_result_e;

/*外部模块注入的函数定义*/
typedef boolean (*ual_gnss_injection_func) (uint32 event, void* p_param, uint32 param_size);

/*ual gnss向上层上报的消息*/
typedef enum
{
    /*发给注入函数*/
    MSG_UAL_GNSS_START_REQ,              //启动定位请求
    MSG_UAL_GNSS_STOP_REQ,               //停止定位请求
    MSG_UAL_GNSS_POWEROFF_REQ,           //关闭定位请求

    /*反馈消息*/
    MSG_UAL_GNSS_START_CNF = UAL_MODULE_GNSS<<16,    //启动定位反馈(携带成功或失败的参数)，开始搜星
    MSG_UAL_GNSS_STOP_CNF,             //停止定位反馈(携带成功或失败的参数)
    MSG_UAL_GNSS_POWEROFF_CNF,         //进入低功耗反馈(携带成功或失败的参数)
    MSG_UAL_GNSS_SET_AGNSS_DNS_CNF,    //设置AGNSS域名反馈
    MSG_UAL_GNSS_SET_GNSS_MODE_CNF,    //设置定位模式反馈(携带成功或失败的参数)
    MSG_UAL_GNSS_READ_INFO_CNF,        //读取位置信息反馈(携带定位数据)
    MSG_UAL_GNSS_OUTPUT_NMEA_CNF,  //设置输出NMEA数据反馈(携带成功或失败的参数)
    MSG_UAL_GNSS_DEL_AID_DATA_CNF,     //清空定位数据反馈(携带成功或失败的参数)

    /*主动上报的消息*/
    MSG_UAL_GNSS_SET_STARTMODE_IND,         //目前条件支持的启动模式(携带当前真正的启动模式)
    MSG_UAL_GNSS_FIX_IND,                  //定位成功
    MSG_UAL_GNSS_OUTPUT_NMEA_IND,         //输出NMEA位置信息(携带定位数据)
    MSG_UAL_GNSS_MAX
}ual_gnss_msg_e;

/*开始定位需要的配置参数*/
typedef struct
{
    //ual_gnss_mode_e gnss_mode;     //定位模式(在初始化时默认定位模式为GNSS_GLONASS,所以这里先不用此参数。
                                  //如果要设置定位模式，请调用ual_gnss_set_gnss_mode)
    ual_gnss_start_mode_e     start_mode;         //启动模式
}ual_gnss_start_param_t;

/*设置NMEA数据需要的配置参数*/
typedef struct
{
    ual_gnss_nmea_type_e nmea_type;  //NMEA格式
    uint32 time_interval;           //上报NMEA数据的时间间隔(单位:秒)
}ual_gnss_nmea_output_param_t;

/*时间*/
typedef struct
{
    uint16 year;
    uint16 month;
    uint16 day;
    uint16 hour;
    uint16 minute;
    uint16 second;
}ual_gnss_time_stamp_t;

typedef struct
{
   uint32 milliseconds;
   uint32 second;
}ual_gnss_system_time_t;

/*定位信息*/
typedef struct
{
    float longitude;     // 经度
    float latitude;      // 纬度
    float altitude;      // 高度
    ual_gnss_time_stamp_t utc_time;  // 世界标准时间
    uint32 ttff_time;    // 首次定位时间
    uint8 satellite_num; // 卫星数量
    float speed;         // 速度
    float course;        // 航向
    uint16 clockdrift;   // 时钟漂移
    ual_gnss_system_time_t SystemTimeStamp; // RTC or any other system time
    uint32 m_nUTCTime;                  // second
    uint8 uncertaintySemiMajor;
    uint8 uncertaintySemiMinor;
    float Bearing;            // increments of 1 measured clockwise from the North.
    uint16 Direction;         // UP=0, Down=1
    float HorizontalVelocity; // m/s
    float VerticalVelocity;   // m/s
    uint32 fix_flag;
    float HorizontalAccuracy;  //水平精度
    float PDOP; // position dilution of precision,位置精度(纬度、经度和高程等误差平方和的开根号值)
    float HDOP; // horizontal dilution of precision,水平精度
    float VDOP; // vertical dilution of precision,垂直精度
}ual_gnss_gps_info_t;

/*卫星信息*/
typedef struct
{
    uint16 satellite_identifier;  //卫星标识符
    uint16 cn0;                  //信噪比
    uint16 elevation;            //海拔
    uint16 azimuth;             //方位角
    uint8 is_used;              //是否用到此卫星
}ual_gnss_satellite_info_t;

/*卫星信息列表*/
typedef struct
{
    uint8  satellite_num;                             //卫星数量
    ual_gnss_satellite_info_t  satellite_info[UAL_GNSS_SATELLITE_MAX_NUM];   //卫星信息
}ual_gnss_satellite_info_list_t;

/*MSG_UAL_GNSS_READ_INFO_CNF携带的参数*/
typedef struct
{
    ual_gnss_gps_info_t  gps_info;                          //定位信息
    ual_gnss_satellite_info_list_t  satellite_list_info;   //卫星信息列表
}ual_gnss_read_info_cnf_t;

/*MSG_UAL_GNSS_OUTPUT_NMEA_IND携带的参数*/
typedef struct
{
    ual_gnss_nmea_type_e type;                       //nmea格式
    uint8 nmea_data[UAL_GNSS_NMEA_DATA_MAX_SIZE];   //nmea数据
    uint16 length;                                 //nmea数据长度
}ual_gnss_output_nmea_ind_t;

/*MSG_UAL_GNSS_FIX_IND携带的参数*/
typedef struct
{
    float longitude;                  //经度
    float latitude;                   //纬度
    ual_gnss_time_stamp_t utc_time;  //世界统一时间
    uint32 ttff_time;               //首次定位时间
    uint16 cn0;                     //信噪比
}ual_gnss_fix_ind_t;

typedef struct
{
    boolean is_ok;   //相应的请求是否成功
}ual_gnss_download_cnf_t,
ual_gnss_start_cnf_t,
ual_gnss_stop_cnf_t,
ual_gnss_poweroff_cnf_t,
ual_gnss_output_nmea_cnf_t,
ual_gnss_set_gnss_mode_cnf_t,
ual_gnss_set_agnss_dns_cnf_t,
ual_gnss_del_aid_data_cnf_t;

/**--------------------------------------------------------------------------*
**                         FUNCTION DECLARATION                              *
**---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description: 注入callback，用于监听ual gnss上报的消息
//  Parameter: [In] p_callback   //app向ual gnss注册的callback，在此callback中可以接收ual gnss上报的消息
//             [Out] p_handle    //注册得到的cms handle，去注册的时候还需要传入此handle
//             [Return] 错误码
//  Author: zirui.li
//  Note:
/*****************************************************************************/
ual_gnss_result_e ual_gnss_register(_ual_cms_client_register_callback p_callback, uint32* p_handle);

/*****************************************************************************/
//  Description: callback去注册，去注册后不再监听ual gnss上报的消息
//  Parameter: [In] handle   //ual_gnss_register时，输出的handle
//             [Return] 错误码
//  Author: zirui.li
//  Note:
/*****************************************************************************/
ual_gnss_result_e ual_gnss_unregister(uint32 handle);

/*****************************************************************************/
//  Description: 注入函数
//  Parameter: [In] p_func   //外部模块向ual gnss注入的函数
//             [Out] none
//             [Return] 错误码
//  Author: zirui.li
//  Note:外部模块可调用此接口注入函数，p_func在ual_gnss_start接口中会被调用
/*****************************************************************************/
ual_gnss_result_e ual_gnss_inject_function(ual_gnss_injection_func p_func);

/*****************************************************************************/
//  Description:取消注入函数
//  Parameter: [In] none
//             [Out] none
//             [Return] 错误码
//  Author: zirui.li
//  Note:外部模块可调用此接口取消注入函数，调用完此接口后，ual_gnss_start中将不会再调用注入函数
/*****************************************************************************/
ual_gnss_result_e ual_gnss_uninject_function(void);

/*****************************************************************************/
//  Description: ual gnss初始化
//  Parameter: [In] none
//             [Out] none
//             [Return] 错误码
//  Author: zirui.li
//  Note:启机的时候调用
/*****************************************************************************/
ual_gnss_result_e ual_gnss_init(void);

/*****************************************************************************/
//  Description: 启动定位
//  Parameter: [In] p_start_param     //开始定位需要传入的参数
//             [Out] none
//             [Return] 错误码
//  Author: zirui.li
//  Note:定位成功后，会每隔1s上报MSG_UAL_GNSS_OUTPUT_INFO_IND，携带所有type的nmea数据
//       如果想要修改nmea type和上报时间间隔，可调用ual_gnss_set_nmea_output接口
/*****************************************************************************/
ual_gnss_result_e ual_gnss_start(ual_gnss_start_param_t* p_start_param);

/*****************************************************************************/
//  Description: 停止定位
//  Parameter: [In] none
//             [Out] none
//             [Return] 错误码
//  Author: zirui.li
//  Note:
/*****************************************************************************/
ual_gnss_result_e ual_gnss_stop(void);

/*****************************************************************************/
//  Description: 关闭定位
//  Parameter: [In] none
//             [Out] none
//             [Return] 错误码
//  Author: zirui.li
//  Note:
/*****************************************************************************/
ual_gnss_result_e ual_gnss_poweroff(void);

/*****************************************************************************/
//  Description: 设置AGNSS域名
//  Parameter: [In] p_url      //url
//             [Out] none
//             [Return] 错误码
//  Author: zirui.li
//  Note:
/*****************************************************************************/
ual_gnss_result_e ual_gnss_set_agnss_dns(char* p_url);

/*****************************************************************************/
//  Description: 设置参与定位的卫星模式
//  Parameter: [In] gnss_mode   //定位模式
//             [Out] none
//             [Return] 错误码
//  Author: zirui.li
//  Note:
/*****************************************************************************/
ual_gnss_result_e ual_gnss_set_gnss_mode(ual_gnss_mode_e gnss_mode);

/*****************************************************************************/
//  Description: 读取定位信息
//  Parameter: [In] none
//             [Out] none
//             [Return] 错误码
//  Author: zirui.li
//  Note:
/*****************************************************************************/
ual_gnss_result_e ual_gnss_read_info(void);

/*****************************************************************************/
//  Description: 设置输出NMEA数据
//  Parameter: [In] p_param    //配置的参数
//             [Out] none
//             [Return] 错误码
//  Author: zirui.li
//  Note:
/*****************************************************************************/
ual_gnss_result_e ual_gnss_output_nmea(ual_gnss_nmea_output_param_t* p_param);

/*****************************************************************************/
//  Description: 清空星历和历书
//  Parameter: [In] none
//             [Out] none
//             [Return] 错误码
//  Author: zirui.li
//  Note:
/*****************************************************************************/
ual_gnss_result_e ual_gnss_del_aid_data(void);

/*****************************************************************************/
//  Description: 接收libgps发来的signal
//  Parameter: [In] p_sig    //signal
//             [Out] none
//             [Return] 错误码
//  Author: zirui.li
//  Note:
/*****************************************************************************/
ual_gnss_result_e ual_gnss_process_libgps_msg(void* p_sig);

/*****************************************************************************/
//  Description: 获得ual gnss状态机的当前状态
//  Parameter: [In] none
//             [Out] none
//             [Return] 当前状态
//  Author: zirui.li
//  Note:
/*****************************************************************************/
ual_gnss_state_e ual_gnss_get_cur_state(void);

/*****************************************************************************/
//  Description: 获得cp mode
//  Parameter: [In] none
//             [Out] none
//             [Return] cp mode
//  Author: zirui.li
//  Note:
/*****************************************************************************/
int ual_gnss_get_cp_mode(void);
#endif //_UAL_GNSS_H
