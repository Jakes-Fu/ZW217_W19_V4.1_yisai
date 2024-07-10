/*****************************************************************************
** File Name:      watch_sensor.h                                            *
** Author:         longwei.qiao                                              *
** Date:           15/04/2020                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe sensor export interface     *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 4/2021       longwei.qiao         Create                                  *
******************************************************************************/
#ifndef _WATCH_SENSOR_H_
#define _WATCH_SENSOR_H_
/**--------------------------------------------------------------------------*
**                          PUBLIC FUNCTION                                  *
**---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : init sensor module
//  Global resource dependence : none
//  Author:longwei.qiao
//  Note:
/*****************************************************************************/
PUBLIC void WATCH_Sensor_InitModule(void);
/*****************************************************************************/
//  Description : retry open sensor
//  Global resource dependence : none
//  Author:longwei.qiao
//  Note:
/*****************************************************************************/
PUBLIC void WATCH_Sensor_Retry(uint32 param);
/*****************************************************************************/
//  Description: regester sensor
//  Parameter: [In] None
//             [Out] None
//             [Return] TRUE/FALSE
//  Author: longwei.qiao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN AdultWatch_RegisterRaiseWristSensor(void);
/*****************************************************************************/
//  Description: regester sensor
//  Parameter: [In] None
//             [Out] None
//             [Return] TRUE/FALSE
//  Author: longwei.qiao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN AdultWatch_UnregesterRaiseWristSensor(void);
/*****************************************************************************/
//  Description: active sensor
//  Parameter: [In] None
//             [Out] None
//             [Return] ual_sensorhub_res_e
//  Author: longwei.qiao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN AdultWatch_ActiveRaiseWristSensor(void);
/*****************************************************************************/
//  Description: deactive sensor
//  Parameter: [In] None
//             [Out] None
//             [Return] ual_sensorhub_res_e
//  Author: longwei.qiao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN AdultWatch_DeactiveRaiseWristSensor(void);


#endif
