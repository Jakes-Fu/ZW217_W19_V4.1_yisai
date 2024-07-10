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
** File Name:       ual_sensorhub.h                                          *
** Author:          tingting.zheng                                           *
** Date:            01/30/2021                                               *
** Description:    This file is used to define ual sensorhub export          *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE                 NAME                  DESCRIPTION                    *
** 02/24/2021       tingting.zheng            Create                         *
******************************************************************************/
#ifndef _UAL_SENSORHUB_H_
#define _UAL_SENSORHUB_H_

#include "ual_cms_export.h"

#define UAL_SENSOR_DELAY_FASTEST    (10)    //FASTEST
#define UAL_SENSOR_DELAY_GAME       (20)    //ms,50HZ
#define UAL_SENSOR_DELAY_UI         (33)    //ms,30HZ
#define UAL_SENSOR_DELAY_NORMAL     (200)   //5HZ

typedef uint32 ual_sensor_handle_t;

typedef enum
{
    UAL_SENSOR_TYPE_ACCELEROMETER = 1,
    UAL_SENSOR_TYPE_GYROSCOPE,
    UAL_SENSOR_TYPE_AIR_PRESSURE,
    UAL_SENSOR_TYPE_LIGHT,
    UAL_SENSOR_TYPE_BLOOD_OXYGEN,
    UAL_SENSOR_TYPE_BODY_TEMPERATURE,
    UAL_SENSOR_TYPE_HEART_RATE,
    UAL_SENSOR_TYPE_CALORY,
    UAL_SENSOR_TYPE_FALL_DOWN,
    UAL_SENSOR_TYPE_FLIP_WRIST,
    UAL_SENSOR_TYPE_MOTION_STATUS,
    UAL_SENSOR_TYPE_RAISE_WRIST,
    UAL_SENSOR_TYPE_STEP_COUNTER,
    UAL_SENSOR_TYPE_SHAKE,
    UAL_SENSOR_TYPE_SEDENTARY,
    UAL_SENSOR_TYPE_HEART_RATE_PPG,
    UAL_SENSOR_TYPE_BLOOD_OXYGEN_PPG,
    UAL_SENSOR_TYPE_RESERVED_1,//给客户预留的3个sensor type，客户需按照协议规定发送数据
    UAL_SENSOR_TYPE_RESERVED_2,
    UAL_SENSOR_TYPE_RESERVED_3,
    UAL_SENSOR_TYPE_MAX
}ual_sensor_type_e;

typedef struct
{
    int16   x;
    int16   y;
    int16   z;
    uint64  timestamp;
}ual_sensor_acc_t,ual_sensor_gyr_t;

typedef struct
{
    uint32  air_pressure;
    uint64  timestamp;
}ual_sensor_air_pressure_t;

typedef struct
{
    uint32  light;
    uint64  timestamp;
}ual_sensor_light_t;

typedef struct
{
    uint32  blood_oxygen;
    uint64  timestamp;
}ual_sensor_blood_oxygen_t;

typedef struct
{
    float   body_temperature;
    uint64  timestamp;
}ual_sensor_body_temperature_t;

typedef struct
{
    uint32  heart_rate;
    uint64  timestamp;
}ual_sensor_heart_rate_t;

typedef struct
{
    uint32  calory;
    uint64  timestamp;
}ual_sensor_calory_t;

typedef struct
{
    uint32  fall_down;
    uint64  timestamp;
}ual_sensor_fall_down_t;

typedef struct
{
    uint32  flip_wrist;
    uint64  timestamp;
}ual_sensor_flip_wrist_t;

typedef struct
{
    uint32  motion_status;
    uint64  timestamp;
}ual_sensor_motion_status_t;

typedef struct
{
    uint32  raise_wrist;
    uint64  timestamp;
}ual_sensor_raise_wrist_t;

typedef struct
{
    uint32  step_counter;
    uint64  timestamp;
}ual_sensor_step_counter_t;

typedef struct
{
    uint32  shake;
    uint64  timestamp;
}ual_sensor_shake_t;

typedef struct
{
    uint32  sedentary;
    uint64  timestamp;
}ual_sensor_sedentary_t;

typedef struct
{
    int32   raw_green;
    int32   raw_infrared;
    uint64  timestamp;
}ual_sensor_heart_rate_ppg_t;

typedef struct
{
    int32   raw_red;
    int32   raw_infrared;
    uint64  timestamp;
}ual_sensor_blood_oxygen_ppg_t;

typedef enum
{
   UAL_SENHUB_NONWAKEUP,
   UAL_SENHUB_WAKEUP
}ual_sensorhub_wakeupmode_e;

typedef enum
{
   UAL_SENSORHUB_SUCCESS,
   UAL_SENSORHUB_ERROR,
   UAL_SENSORHUB_RES_MAX
}ual_sensorhub_res_e;

typedef enum
{
   UAL_SENSORHUB_CALIBRATE_SUCCESS,
   UAL_SENSORHUB_CALIBRATE_FAIL,
}ual_sensorhub_calibrate_res_e;

typedef struct
{
    int16   x;
    int16   y;
    int16   z;
    uint64  timestamp;
    ual_sensorhub_calibrate_res_e  cali_result;
}ual_sensor_acc_cali_t,ual_sensor_gyr_cali_t;

/*****************************************************************************/
//  Description : register sensor event for APPs run in APP_Task
//  Parameter: [In] sensor_type: the enum of sensor
//             [In] _ual_cms_client_register_callback: callback function
//             [In] None
//             [Out] None
//             [Return] ual_sensor_handle_t:the handle of sensor
//  Author: tingting.zheng
//  Note:
/*****************************************************************************/
ual_sensor_handle_t ual_sensorhub_register_sensor (ual_sensor_type_e sensor_type, _ual_cms_client_register_callback sensor_callback);

/*****************************************************************************/
//  Description : unregister sensor event according with sensor_handle
//  Parameter: [In] ual_sensor_handle_t: the handle of sensor
//             [Out] None
//             [Return] ual_sensorhub_res_e:the result of API
//  Author: tingting.zheng
//  Note:
/*****************************************************************************/
ual_sensorhub_res_e ual_sensorhub_unregister_sensor (ual_sensor_handle_t sensor_handle);

/*****************************************************************************/
//  Description : active sensor according with sensor_handle
//  Parameter: [In] ual_sensor_handle_t: the handle of sensor
//             [In] sample_rate: the value of sample rate
//             [In] batching_time: the value of batch time
//             [In] ual_sensorhub_wakeupmode_e: the value of wakeup mode
//             [Out] None
//             [Return] ual_sensorhub_res_e:the result of API
//  Author: tingting.zheng
//  Note:
/*****************************************************************************/
ual_sensorhub_res_e ual_sensorhub_active_sensor (ual_sensor_handle_t sensor_handle, uint32 sample_rate, uint32 batching_time, ual_sensorhub_wakeupmode_e wakeup_mode);

/*****************************************************************************/
//  Description : deactive sensor according with sensor_handle
//  Parameter: [In] ual_sensor_handle_t: the handle of sensor
//             [Out] None
//             [Return] ual_sensorhub_res_e:the result of API
//  Author: tingting.zheng
//  Note:
/*****************************************************************************/
ual_sensorhub_res_e ual_sensorhub_deactive_sensor (ual_sensor_handle_t sensor_handle);

/*****************************************************************************/
//  Description : deactive all sensor according with sensot_type
//  Parameter: [In] sensor_type: the enum of sensor
//             [Out] None
//             [Return] ual_sensorhub_res_e:the result of API
//  Author: tingting.zheng
//  Note:
/*****************************************************************************/
ual_sensorhub_res_e ual_sensorhub_deactive_sensor_by_type (ual_sensor_type_e sensor_type);

/*****************************************************************************/
//  Description : set sensor sample rate according with sensor_handle
//  Parameter: [In] ual_sensor_handle_t: the handle of sensor
//             [In] sample_rate: the value of sample rate
//             [Out] None
//             [Return] ual_sensorhub_res_e:the result of API
//  Author: tingting.zheng
//  Note:
/*****************************************************************************/
ual_sensorhub_res_e ual_sensorhub_set_sensor_sample_rate (ual_sensor_handle_t sensor_handle, uint32 sample_rate);

/*****************************************************************************/
//  Description : set sensor batch time according with sensor_handle
//  Parameter: [In] ual_sensor_handle_t: the handle of sensor
//             [In] batching_time: the value of batch time
//             [Out] None
//             [Return] ual_sensorhub_res_e:the result of API
//  Author: tingting.zheng
//  Note:
/*****************************************************************************/
ual_sensorhub_res_e ual_sensorhub_set_sensor_batching_time (ual_sensor_handle_t sensor_handle, uint32 batching_time);

/*****************************************************************************/
//  Description : set sensor wakeup mode according with sensor_handle
//  Parameter: [In] ual_sensor_handle_t: the handle of sensor
//             [In] ual_sensorhub_wakeupmode_e: the value of wakeup mode
//             [Out] None
//             [Return] ual_sensorhub_res_e:the result of API
//  Author: tingting.zheng
//  Note:
/*****************************************************************************/
ual_sensorhub_res_e ual_sensorhub_set_sensor_wakeup_mode (ual_sensor_handle_t sensor_handle, ual_sensorhub_wakeupmode_e wakeup_mode);

/*****************************************************************************/
//  Description : calibrate sensor according with sensot_type
//  Parameter: [In] sensor_type: the enum of sensor
//             [In] _ual_msg_callback: calibrate callback function
//             [Out] None
//             [Return] ual_sensorhub_res_e:the result of API
//  Author: tingting.zheng
//  Note:
/*****************************************************************************/
ual_sensorhub_res_e ual_sensorhub_calibrate_sensor (ual_sensor_type_e sensor_type, _ual_cms_client_register_callback sensor_callback);

#endif
