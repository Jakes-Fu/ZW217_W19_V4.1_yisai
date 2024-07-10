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
** File Name:       mmisfr_cmcc_main.h                                         *
** Author:          miao.liu2                                                  *
** Date:            03/04/2022                                                 *
** Description:    This file is used to define cmcc main export api            *
********************************************************************************
**                         Important Edit History                              *
** ----------------------------------------------------------------------------*
** DATE                 NAME                  DESCRIPTION                      *
** 03/04/2022          miao.liu2               Create                          *
********************************************************************************/

#ifndef _MMISFR_CMCC_MAIN_H_
#define _MMISFR_CMCC_MAIN_H_

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : 收到network status消息时进行移动自注册入口
//  Param :None
//  Author: miao.liu2
//  Note:
/*****************************************************************************/
PUBLIC void MMISFR_CMCC_HandleNetworkStatus(void);

/*****************************************************************************/
//  Description : 模块初始化函数
//  Param :None
//  Author: miao.liu2
//  Note:
/*****************************************************************************/
PUBLIC void MMISFR_CMCC_InitModule(void);

/*****************************************************************************/
//  Description :Set Allow Active Pdp
//  Param :is_allow_use_pdp
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMISFR_CMCC_SetAllowActivePdp(BOOLEAN is_allow_use_pdp);

#endif

