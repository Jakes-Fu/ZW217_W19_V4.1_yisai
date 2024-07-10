/*****************************************************************************
** File Name:      watch_sensor.c                                            *
** Author:         longwei.qiao                                              *
** Date:           04/15/2020                                                *
** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe sensor function             *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 4/2021      longwei.qiao          Create                                  *
******************************************************************************/

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmk_type.h"
#include "mmi_common.h"
#include "mmicom_trace.h"
#include "watch_sensor.h"
#include "watch_steps.h"
#include "watch_heartrate.h"
#include "mmk_timer.h"
#include "mmiset_export.h"
#include "watch_sensor_id.h"
#ifdef UAL_SENSORHUB_SUPPORT
#include "ual_sensorhub.h"
#endif

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/
#define SENSOR_RETRY_TIME    (5000)//(mS)
/**--------------------------------------------------------------------------*
**                          LOCAL VARIABLES                                  *
**---------------------------------------------------------------------------*/
LOCAL uint8  s_raise_wrist_retry_timer_id = 0;
LOCAL uint8  s_steps_retry_timer_id = 0;
/*---------------------------------------------------------------------------*
**                          LOCAL FUNCTION                                   *
**---------------------------------------------------------------------------*/
LOCAL void HandleRetrySensorTimer(uint8  timer_id,uint32 param)
{
    BOOLEAN register_result = TRUE;
    BOOLEAN active_result = TRUE;
    if((timer_id != s_raise_wrist_retry_timer_id)&&(timer_id != s_steps_retry_timer_id))
    {
        TRACE_APP_SENSOR("s_raise_wrist_retry_timer_id = %d,s_steps_retry_timer_id = %d,  timer_id = %d", s_raise_wrist_retry_timer_id,s_steps_retry_timer_id,timer_id);
        return;
    }
    MMK_StopTimer(timer_id);
    TRACE_APP_SENSOR("sensor type = %d", param);
    switch(param)
    {
        case UAL_SENSOR_TYPE_ACCELEROMETER:
            break;
        case UAL_SENSOR_TYPE_GYROSCOPE:
            break;
        case UAL_SENSOR_TYPE_LIGHT:
            break;
        case UAL_SENSOR_TYPE_BLOOD_OXYGEN:
            break;
        case UAL_SENSOR_TYPE_BODY_TEMPERATURE:
            break;
        case UAL_SENSOR_TYPE_HEART_RATE:
            break;
        case UAL_SENSOR_TYPE_CALORY:
            break;
        case UAL_SENSOR_TYPE_FALL_DOWN:
            break;
        case UAL_SENSOR_TYPE_FLIP_WRIST:
            break;
        case UAL_SENSOR_TYPE_MOTION_STATUS:
            break;
        case UAL_SENSOR_TYPE_RAISE_WRIST:
            if(0 == AdultWatch_GetRaiseWristHandle())
            {
                register_result = AdultWatch_RegisterRaiseWristSensor();
            }
            active_result = AdultWatch_ActiveRaiseWristSensor();
            break;
        case UAL_SENSOR_TYPE_STEP_COUNTER:
            if(0 == AdultWatch_GetStepsHandle())
            {
                register_result = AdultWatchSteps_RegesterSensor();
            }
            active_result = AdultWatchSteps_ActiveSensor();
        break;
        case UAL_SENSOR_TYPE_SHAKE:
        break;
        default:
        break;
    }
    TRACE_APP_SENSOR("active_result = %d",active_result);
    if(!active_result)
    {
        WATCH_Sensor_Retry(param);
    }
}
/*****************************************************************************/
//  Description : retry open sensor
//  Global resource dependence : none
//  Author:longwei.qiao
//  Note:
/*****************************************************************************/
PUBLIC void WATCH_Sensor_Retry(uint32 param)
{
    switch(param)
    {
        case UAL_SENSOR_TYPE_ACCELEROMETER:
        break;
        case UAL_SENSOR_TYPE_GYROSCOPE:
        break;
        case UAL_SENSOR_TYPE_LIGHT:
        break;
        case UAL_SENSOR_TYPE_BLOOD_OXYGEN:
        break;
        case UAL_SENSOR_TYPE_BODY_TEMPERATURE:
        break;
        case UAL_SENSOR_TYPE_HEART_RATE:
        break;
        case UAL_SENSOR_TYPE_CALORY:
        break;
        case UAL_SENSOR_TYPE_FALL_DOWN:
        break;
        case UAL_SENSOR_TYPE_FLIP_WRIST:
        break;
        case UAL_SENSOR_TYPE_MOTION_STATUS:
        break;
        case UAL_SENSOR_TYPE_RAISE_WRIST:
        {
            if (NULL != s_raise_wrist_retry_timer_id)
            {
                MMK_StopTimer(s_raise_wrist_retry_timer_id);
                s_raise_wrist_retry_timer_id = 0;
            }
            s_raise_wrist_retry_timer_id = MMK_CreateTimerCallback(SENSOR_RETRY_TIME,HandleRetrySensorTimer,param,FALSE);
        }
        break;
        case UAL_SENSOR_TYPE_STEP_COUNTER:
        {
            if (NULL != s_steps_retry_timer_id)
            {
                MMK_StopTimer(s_steps_retry_timer_id);
                s_steps_retry_timer_id= 0;
            }
            s_steps_retry_timer_id = MMK_CreateTimerCallback(SENSOR_RETRY_TIME,HandleRetrySensorTimer,param,FALSE);
        }
        break;
        case UAL_SENSOR_TYPE_SHAKE:
        break;
        default:
        break;
    }
}
/*****************************************************************************/
//  Description : Init sensor module
//  Global resource dependence : none
//  Author:longwei.qiao
//  Note:
/*****************************************************************************/
PUBLIC void WATCH_Sensor_InitModule(void)
{
    BOOLEAN result = TRUE;
    WATCH_Sensor_RegWinIdNameArr();
    // raise wrist init
    if(WATCHAPISET_GetRaiseWrist())
    {
        result = AdultWatch_RegisterRaiseWristSensor();
        result &= AdultWatch_ActiveRaiseWristSensor();
        if(result == FALSE)
        {
            WATCH_Sensor_Retry(UAL_SENSOR_TYPE_RAISE_WRIST);
        }
    }
    //steps init
    AdultWatchSteps_ReadInfoWhenPowerOn();
    result = AdultWatchSteps_RegesterSensor();
    result &= AdultWatchSteps_ActiveSensor();
    if(result == FALSE)
    {
        WATCH_Sensor_Retry(UAL_SENSOR_TYPE_STEP_COUNTER);
    }
    //heartrate init
    result = AdultWatchHeartRate_RegesterSensor();
}
