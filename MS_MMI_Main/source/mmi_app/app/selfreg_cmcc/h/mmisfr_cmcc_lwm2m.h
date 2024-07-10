/*****************************************************************************
** Copyright 2023 Unisoc(Shanghai) Technologies Co.,Ltd                      *
** Licensed under the Unisoc General Software License,                       *
** version 1.0 (the License);                                                *
** you may not use this file except in compliance with the License.          *
** You may obtain a copy of the License at                                   *
** https://www.unisoc.com/en_us/license/UNISOC_GENERAL_LICENSE_V1.0-EN_US    *
** Software distributed under the License is distributed on an "AS IS" BASIS,*
** WITHOUT WARRANTIES OF ANY KIND, either express or implied.                *
** See the Unisoc General Software License, version 1.0 for more details.    *
******************************************************************************/

/*******************************************************************************
** File Name:       mmisfr_cmcc_lwm2m.h                                        *
** Author:          miao.liu2                                                  *
** Date:            03/04/2022                                                 *
** Description:    This file is used to define cmcc lwm2m export api           *
********************************************************************************
**                         Important Edit History                              *
** ----------------------------------------------------------------------------*
** DATE                 NAME                  DESCRIPTION                      *
** 03/04/2022          miao.liu2               Create                          *
********************************************************************************/

#ifndef _MMISFR_CMCC_LWM2M_H_
#define _MMISFR_CMCC_LWM2M_H_

/*---------------------------------------------------------------------------*
**                            MACRO DEFINITION                               *
**---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         TYPE DEFINITION                                   *
**---------------------------------------------------------------------------*/
typedef void (*cmcc_self_register_callback)(BOOLEAN);

/**--------------------------------------------------------------------------*
**                         FUNCTION DECLARE                                 *
**--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description :真正进行自注册流程的入口
//  Param :p_callback
//  Author:miao.liu2
//  Note:
/*****************************************************************************/
PUBLIC void MMISFR_CMCC_OpenLwm2m(cmcc_self_register_callback p_callback);

/*****************************************************************************/
//  Description :clear lwm2m相关数据
//  Param :ref:实例ID
//  Author:miao.liu2
//  Note:
/*****************************************************************************/
PUBLIC void MMISFR_CMCC_ClearLwM2M(void);

#endif
