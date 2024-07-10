/*****************************************************************************
** Copyright 2022 Unisoc(Shanghai) Technologies Co.,Ltd                      *
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
** File Name:      ual_timer.h
** Author: bin.xie
** Description:
*****************************************************************************/

#ifndef _UAL_TIMER_H_
#define _UAL_TIMER_H_
/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/

#include "ual_common.h"
#include "ual_type.h"

/*---------------------------------------------------------------------------*/
/*                          MACRO DEFINITION                                 */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                 */
/*---------------------------------------------------------------------------*/
// timer call back
typedef void (*UAL_TIMER_FUNC) (uint32 app_timer_id,uint32 param);

/**--------------------------------------------------------------------------*
 **                         FUNCTION DECLARE                                 *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : init the ual timer; register the global callback to cms
//  Parameter: [In] NONE
//             [In] NONE
//             [Out] NONE
//             [Return] boolean: TRUE: register success ;FALSE: register failure
//  Author: bin.xie
// Note:
/*****************************************************************************/
boolean ual_timer_init(void);

/*****************************************************************************/
//  Description :start a timer
//  Parameter: [In] timer_out : timer 时间
//             [In] fun :  timer 回调函数
//             [In] param :  app 的 param
//             [In] is_period : 是否是周期性的，目前为FALSE,暂不支持周期性的timer
//             [Out] 无
//             [Return] uint32: the timmer id
//  Author: bin.xie
//  Note:
/*****************************************************************************/
uint32 ual_timer_start(uint32 timer_out,
                       UAL_TIMER_FUNC func,
                       uint32 param,
                       boolean is_period
                       );

/*****************************************************************************/
//  Description :stop a timer
//  Parameter: [In] app_timer_id: app timer id
//             [Out] 无
//             [Return] boolean: TRUE: STOP success ;FALSE: STOP failure
//  Author: bin.xie
//  Note:
/*****************************************************************************/
boolean ual_timer_stop(uint32 app_timer_id);

#endif
