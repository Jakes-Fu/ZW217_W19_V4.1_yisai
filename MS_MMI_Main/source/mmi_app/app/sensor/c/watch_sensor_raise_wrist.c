/*****************************************************************************
** File Name:      watch_sensor_raise2wake.c                                      *
** Author:         longwei.qiao                                              *
** Date:           04/15/2020                                                *
** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe notify function             *
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
#ifdef UAL_SENSORHUB_SUPPORT
#include "ual_sensorhub.h"
#endif
#ifdef WATCH_LEFT_RIGHT_WEAR_SUPPORT
#include "dal_display.h"
#endif

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                  *
**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         TYPE AND CONSTANT                                 *
**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                          LOCAL VARIABLES                                  *
**--------------------------------------------------------------------------*/
LOCAL uint32 s_raise_wrist_handle = 0;
/*---------------------------------------------------------------------------*
**                          LOCAL FUNCTION                                   *
**--------------------------------------------------------------------------*/
#ifdef UAL_SENSORHUB_SUPPORT
LOCAL BOOLEAN RaiseWristCallBack(ual_cms_msg_t param)
{
     ual_sensor_raise_wrist_t *raise_wrist_ptr = PNULL; 
     raise_wrist_ptr = (ual_sensor_raise_wrist_t *)param.p_body;
     TRACE_APP_SENSOR("enter");
     if(PNULL == raise_wrist_ptr)
     {
         return FALSE;
     }
     MMIDEFAULT_TurnOnBackLight();
     return TRUE;
}
#endif
/*****************************************************************************/
//  Description: regester sensor
//  Parameter: [In] None
//             [Out] None
//             [Return] TRUE/FALSE
//  Author: longwei.qiao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN AdultWatch_RegisterRaiseWristSensor(void)
{
#ifndef WIN32
#ifdef UAL_SENSORHUB_SUPPORT
         ual_sensor_type_e sensor_type = UAL_SENSOR_TYPE_MAX;
#ifdef WATCH_LEFT_RIGHT_WEAR_SUPPORT
         if (DISPLAY_ANGLE_0 == LCD_GetWearType())
         {
             sensor_type = UAL_SENSOR_TYPE_RAISE_WRIST;
         }
         else
         {
             sensor_type = UAL_SENSOR_TYPE_FLIP_WRIST;
         }
#else
         sensor_type = UAL_SENSOR_TYPE_RAISE_WRIST;
#endif
    s_raise_wrist_handle = ual_sensorhub_register_sensor (sensor_type, RaiseWristCallBack);
#endif
#endif
     TRACE_APP_SENSOR("s_heartrate_handle = %d ", s_raise_wrist_handle);
     if(s_raise_wrist_handle == 0)
     {
         return FALSE;
     }
     return TRUE;
}
/*****************************************************************************/
//  Description: regester sensor
//  Parameter: [In] None
//             [Out] None
//             [Return] TRUE/FALSE
//  Author: longwei.qiao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN AdultWatch_UnregesterRaiseWristSensor(void)
{
#ifndef WIN32
#ifdef UAL_SENSORHUB_SUPPORT
     ual_sensorhub_unregister_sensor (s_raise_wrist_handle);
#endif
#endif
     s_raise_wrist_handle = 0;

     return TRUE;
}
/*****************************************************************************/
//  Description: active sensor
//  Parameter: [In] None
//             [Out] None
//             [Return] ual_sensorhub_res_e
//  Author: longwei.qiao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN AdultWatch_ActiveRaiseWristSensor(void)
{
     BOOLEAN result = FALSE;
#ifndef WIN32
#ifdef UAL_SENSORHUB_SUPPORT
     result = ual_sensorhub_active_sensor (s_raise_wrist_handle, UAL_SENSOR_DELAY_NORMAL/*ms*/, UAL_SENSOR_DELAY_NORMAL*10/*ms*/, UAL_SENHUB_WAKEUP);
     if(UAL_SENSORHUB_SUCCESS == result)
     {
         result = TRUE;
     }
     else
     {
         result = FALSE;
     }

#endif
#endif
     TRACE_APP_SENSOR("result = %d ", result);
     return result;
}
/*****************************************************************************/
//  Description: deactive sensor
//  Parameter: [In] None
//             [Out] None
//             [Return] ual_sensorhub_res_e
//  Author: longwei.qiao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN AdultWatch_DeactiveRaiseWristSensor(void)
{
     BOOLEAN result = FALSE;
#ifndef WIN32
#ifdef UAL_SENSORHUB_SUPPORT
     result = ual_sensorhub_deactive_sensor (s_raise_wrist_handle);
     if(UAL_SENSORHUB_SUCCESS == result)
     {
         result = TRUE;
     }
#endif
#endif
     TRACE_APP_SENSOR("result = %d ", result);
     return result;
}
/*****************************************************************************/
//  Description: get handle
//  Parameter: [In] None
//             [Out] None
//             [Return] s_raise_wrist_handle
//  Author: longwei.qiao
//  Note:
/*****************************************************************************/
PUBLIC uint32 AdultWatch_GetRaiseWristHandle(void)
{
     TRACE_APP_SENSOR("s_raise_wrist_handle = %d ", s_raise_wrist_handle);
     return s_raise_wrist_handle;
}


