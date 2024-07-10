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
** File Name:   ual_at.h                                                     *
** Author:      longwei.qiao                                                 *
** Date:        2021/12/15                                                   *
** Description: This file is used to handle at export api                    *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
**   DATE                  NAME              DESCRIPTION                     *
** 12/15/2021             longwei.qiao         Create                        *
******************************************************************************/

#ifndef _UAL_AT_H_
#define _UAL_AT_H_
/*
example
#include "ual_at.h"

local void user_callback(char* buf, int len)
{
    sci_trace_low("buf is %s", buf):
}

ual_at_send_command("AT+COPS?",user_callback);// "+COPS:0,2"46001",6OK"

ual_at_send_command("ATA",user_callback);// "+CME ERROR:22"

ual_at_send_command("AT+CGACT=1,1",user_callback);// "~ORIG:1,2+CME ERROR:28"

*/

/*****************************************************************************/
//  Description : send at command
//  Parameter: [In] buf_ptr: AT command
//             [In] at_rsp_callback: user callback
//             [Return]NONE
//  Author:longwei.qiao
// Note:only support 0x68(command type)
/*****************************************************************************/
PUBLIC void ual_at_send_command(char* buf_ptr, void (*at_rsp_callback)( char* outBuf, int outBufLen));

#endif //_UAL_AT_COMMAND_H_

