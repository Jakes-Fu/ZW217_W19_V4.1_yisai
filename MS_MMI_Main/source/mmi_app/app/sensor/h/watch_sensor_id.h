/*****************************************************************************
** File Name:      watch_sensor_id.h                                         *
** Author:         longwei.qiao                                              *
** Date:           2021/04/15                                                *
** Copyright:      2011 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe sensor win and control id   *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2021/04/15    longwei.qiao        Create                                  *
******************************************************************************/

#ifndef WATCH_SENSOR_ID_H
#define WATCH_SENSOR_ID_H

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "os_api.h"
#include "mmk_type.h"
#include "mmi_module.h"

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


/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/

#define WIN_ID_DEF(win_id)          win_id
// window ID
typedef enum
{
    WATCH_SENSOR_WIN_ID_START = (WATCH_MODULE_SENSOR << 16),

#include "watch_sensor_id.def"

    WATCH_SENSOR_MAX_WIN_ID
}WATCH_SENSOR_WINDOW_ID_E;

#undef WIN_ID_DEF

// control ID
typedef enum
{
    WATCH_SENSOR_CONTROL_ID_START = WATCH_SENSOR_MAX_WIN_ID,
    WATCH_SENSOR_MAX_CONTROL_ID
}WATCH_SENSOR_CONTROL_ID_E;


/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : register sensor win id
//  Global resource dependence : none
//  Author: longwei.qiao
//  Note:
/*****************************************************************************/
PUBLIC void WATCH_Sensor_RegWinIdNameArr(void);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif 

