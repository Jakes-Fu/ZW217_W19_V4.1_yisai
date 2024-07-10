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

/*****************************************************************************
** File Name:      mmisfr_cmcc_main.c                                        *
** Author:         miao.liu2                                                 *
** Date:           3/4/2023                                                  *
** Description:    This file is used to define cmcc self register function   *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 04/2023      miao.liu2             Create                                 *
******************************************************************************/

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "std_header.h"
#include "os_api.h"
#include "mmipdp_export.h"
#include "mmipdp_internal.h"
#include "mmi_module.h"
#include "mmk_app.h"
#include "mmk_timer.h"
#include "sci_types.h"
#include "version.h"
#include "in_message.h"
#include "mmiphone_export.h"
#include "socket.h"
#include "mmiconnection_export.h"
#include "mmisfr_cmcc_lwm2m.h"
#include "mmicom_trace.h"
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#define CMCC_REG_RETRY_INTERVAL (60*60*1000) //milliseconds - 1 hour
#define CMCC_REG_RETRY_INTERVAL_FOR_UPDATE (60*1000) //milliseconds - 1 MIN
#define CMCC_REG_RETRY_ACTIVE_TIME (10*1000)
#define CMCC_REG_RETRY_TIME_INTERVAL (10*60*1000) //milliseconds-10mins
#define CMCC_REG_RETRY_MAX_TIMES (3)
#define MMI_MODULE_SFR (0x01 << 16)
/**--------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/
LOCAL MMI_APPLICATION_T s_sfr_app = {0};
// allow to use pdp or not
LOCAL BOOLEAN s_allow_use_pdp = TRUE;
LOCAL uint8 s_cmcc_register_times = 0;
LOCAL uint32 s_cmcc_retry_timer = 0;
LOCAL uint8 s_cmcc_retry_pdp_times = 0;
LOCAL uint32 s_cmcc_retry_pdp_timer = 0;
/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description :Active Pdp的函数
//  Param :none
//  Author:miao.liu2
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SFR_CMCC_ActivePdp(void);

/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : 收到network status消息时进行移动自注册入口
//  Param :None
//  Author: miao.liu2
//  Note:
/*****************************************************************************/
PUBLIC void MMISFR_CMCC_HandleNetworkStatus(void)
{
    MN_DUAL_SYS_E dual_sys   = MN_DUAL_SYS_1;
    TRACE_SFR_CMCC("s_allow_use_pdp=%d", s_allow_use_pdp);

    if (s_allow_use_pdp && MMIAPIPHONE_GetSimExistedStatus(dual_sys)
            && MMIPHONE_IsSimOk(dual_sys))
    {
        SFR_CMCC_ActivePdp();
    }
}

/*****************************************************************************/
//  Description :重试的callback函数
//  Param :None
//  Author: miao.liu2
//  Note:
/*****************************************************************************/
LOCAL void SFR_CMCC_HandleRegTimer(uint8 timer_id, uint32 param)
{
    TRACE_SFR_CMCC("enter");
    if(0 != timer_id)
    {
        MMK_StopTimer(timer_id);
    }
    MMISFR_CMCC_HandleNetworkStatus();
}

/*****************************************************************************/
//  Description :挂给lwm2m.c监听移动自注册是否成功的callback
//  Param :MMIPDP_CNF_INFO_T
//  Author:miao.liu2
//  Note:
/*****************************************************************************/
LOCAL void  SFR_CMCC_RegisterCb(BOOLEAN is_success)
{
    if(TRUE == is_success)
    {
        s_cmcc_register_times = 0;
        if(0 != s_cmcc_retry_timer)
        {
            MMK_StopTimer(s_cmcc_retry_timer);
        }
        TRACE_SFR_CMCC("cmcc register succ");
    }
    else
    {
        MMIAPIPDP_Deactive(MMI_MODULE_SFR);
        s_allow_use_pdp = TRUE;

        MMISFR_CMCC_ClearLwM2M();
        TRACE_SFR_CMCC("s_sfr_retry_times = %d", s_cmcc_register_times);
        if (s_cmcc_register_times++ < CMCC_REG_RETRY_MAX_TIMES)
        {
            s_cmcc_retry_timer = MMK_CreateTimerCallback(CMCC_REG_RETRY_TIME_INTERVAL, SFR_CMCC_HandleRegTimer, PNULL, FALSE);
        }
        else
        {
            TRACE_SFR_CMCC("cmcc register fail, s_cmcc_retry_timer = %d", s_cmcc_retry_timer);
            s_cmcc_register_times = 0;
            if(0 != s_cmcc_retry_timer)
            {
                MMK_StopTimer(s_cmcc_retry_timer);
            }
        }
    }
    return;
}

/*****************************************************************************/
//  Description :收到pdp deactive ind的消息
//  Param :none
//  Author:miao.liu2
//  Note:
/*****************************************************************************/
LOCAL void SFR_CMCC_ReceivePdpDeactiveInd(void)
{
    TRACE_SFR_CMCC("enter");
    MMISFR_CMCC_ClearLwM2M();

    //retry register
    MMK_CreateTimerCallback(CMCC_REG_RETRY_INTERVAL_FOR_UPDATE, SFR_CMCC_HandleRegTimer, PNULL, FALSE);
    return;
}

/*****************************************************************************/
//  Description :Active Pdp的回调函数
//  Param :MMIPDP_CNF_INFO_T
//  Author:miao.liu2
//  Note:
/*****************************************************************************/
LOCAL void  SFR_CMCC_HandlePdpMsg(MMIPDP_CNF_INFO_T *p_msg)
{
    MN_DUAL_SYS_E sim_id = 0;
    BOOLEAN result = FALSE;
    if (PNULL == p_msg)
    {
        TRACE_SFR_CMCC("p_msg is pnull");
        return;
    }
    TRACE_SFR_CMCC("msg_id/result=%d, %d", p_msg->msg_id, p_msg->result);
    switch (p_msg->msg_id)
    {
        case MMIPDP_ACTIVE_CNF:
        {
            if (MMIPDP_RESULT_SUCC == p_msg->result)
            {
                s_cmcc_retry_pdp_times = 0;
                if(0 != s_cmcc_retry_pdp_timer)
                {
                    MMK_StopTimer(s_cmcc_retry_pdp_timer);
                }
                socket_SetNetId(p_msg->nsapi);
                MMISFR_CMCC_OpenLwm2m(SFR_CMCC_RegisterCb);
            }
            else
            {
                MMIAPIPDP_Deactive(MMI_MODULE_SFR);
                s_allow_use_pdp = TRUE;
                if(0 != s_cmcc_retry_pdp_timer)
                {
                    MMK_StopTimer(s_cmcc_retry_pdp_timer);
                }
                TRACE_SFR_CMCC("s_cmcc_register_times=%d", s_cmcc_retry_pdp_times);
                if (s_cmcc_retry_pdp_times++ < CMCC_REG_RETRY_MAX_TIMES)
                {
                    s_cmcc_retry_pdp_timer = MMK_CreateTimerCallback(CMCC_REG_RETRY_ACTIVE_TIME, SFR_CMCC_HandleRegTimer, PNULL, FALSE);
                }
            }
            break;
        }

        case MMIPDP_DEACTIVE_CNF:
            break;

        case MMIPDP_DEACTIVE_IND:
        {
            MMIAPIPDP_Deactive(MMI_MODULE_SFR);
            s_allow_use_pdp = TRUE;
            SFR_CMCC_ReceivePdpDeactiveInd();
            break;
        }

        default:
            break;
    }

}

/*****************************************************************************/
//  Description :Active Pdp的函数
//  Param :none
//  Author:miao.liu2
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SFR_CMCC_ActivePdp(void)
{
    BOOLEAN return_val = FALSE;
    MMIPDP_ACTIVE_INFO_T active_info = {0};
    MN_DUAL_SYS_E sim_sys = 0;
    MMICONNECTION_LINKSETTING_DETAIL_T *p_linksetting = PNULL;
    uint8 linkNum = MMIAPICONNECTION_GetLinkSettingNum(sim_sys);
    int i = 0;

    TRACE_SFR_CMCC("s_allow_use_pdp=%d", s_allow_use_pdp);
    if (FALSE == s_allow_use_pdp)
    {
        return return_val;
    }
    s_allow_use_pdp = FALSE;

    for (i = 0; i < linkNum; i++)
    {
        p_linksetting = PNULL;
        p_linksetting = MMIAPICONNECTION_GetLinkSettingItemByIndex(sim_sys, i);
        if (PNULL != p_linksetting && 0 != p_linksetting->apn_len)
        {
            // set apn, username and psw
            active_info.apn_ptr       = p_linksetting->apn;
            active_info.user_name_ptr = p_linksetting->username;
            active_info.psw_ptr       = p_linksetting->password;
            break;
        }
    }
    active_info.dual_sys = sim_sys;
    active_info.ps_interface = MMIPDP_INTERFACE_GPRS;
    active_info.ps_service_type = BROWSER_E;
    active_info.storage = MN_GPRS_STORAGE_ALL;
#ifdef IPVERSION_SUPPORT_V4_V6
    active_info.ip_type = p_linksetting->ip_type;
#endif

    active_info.app_handler = MMI_MODULE_SFR;
    active_info.priority = 3;
    active_info.ps_service_rat = MN_UNSPECIFIED;
    active_info.handle_msg_callback = SFR_CMCC_HandlePdpMsg;

    if (TRUE == MMIAPIPDP_Active(&active_info))
    {
        return_val = TRUE;
    }
    TRACE_SFR_CMCC("return_val=%d", return_val);
    return return_val;
}

LOCAL MMI_RESULT_E SFR_CMCC_HandleModuleMsg(PWND app_ptr, uint16 msg_id, DPARAM param)
{
    TRACE_SFR_CMCC("msg_id=%d", msg_id);
    return MMI_RESULT_FALSE;
}

/*****************************************************************************/
//  Description : 模块初始化函数
//  Param :None
//  Author: miao.liu2
//  Note:
/*****************************************************************************/
PUBLIC void MMISFR_CMCC_InitModule(void)
{
    TRACE_SFR_CMCC("enter");
    s_sfr_app.ProcessMsg = SFR_CMCC_HandleModuleMsg;
}

/*****************************************************************************/
//  Description :Set Allow Active Pdp
//  Param :is_allow_use_pdp
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMISFR_CMCC_SetAllowActivePdp(BOOLEAN is_allow_use_pdp)
{
    s_allow_use_pdp = is_allow_use_pdp;
    TRACE_SFR_CMCC("is_allow_use_pdp=%d", is_allow_use_pdp);
}
