/*****************************************************************************
** File Name:      mmisfr_main.c
** Author:
** Copyright (C) 2019 Unisoc Communications Inc.
** Description:    Entry of DM APP Module
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE				NAME				DESCRIPTION
** 2020/07/17		mark.zhang1			Create
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
#include "mmisfr_lwm2m.h"
#include "mmk_timer.h"
#include "mmiwifi_export.h"
#include "mmisfr_main.h"
#include "sci_types.h"
#include "version.h"
#include "in_message.h"
#include "mmiphone_export.h"
#include "socket.h"
#include "mmiconnection_export.h"
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#define REG_RETRY_INTERVAL (60*60*1000) //milliseconds - 1 hour
#define REG_RETRY_INTERVAL_FOR_UPDATE (60*1000) //milliseconds - 1 MIN

#define FIRST_UPDATE_INTERVAL (86400*1000) //milliseconds - 2mins; release version: 86400s
#define GENERAL_UPDATE_INTERVAL (86400*1000) //milliseconds - 5 mins; release version: 86400s

/**--------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/
MMI_APPLICATION_T g_sfr_app = {0};

// allow to use pdp or not
BOOLEAN s_open_door = TRUE;

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description: main function
//  Param:
//  Author:
/*****************************************************************************/
LOCAL MMI_RESULT_E MMISFR_HandleModuleMsg(PWND app_ptr, uint16 msg_id,
        DPARAM param);

/*****************************************************************************/
//  Description:
//  Param:
//  Author:
/*****************************************************************************/
LOCAL BOOLEAN MMISFR_PdpByPsInterface(BOOLEAN s_is_gprs);

/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/
LOCAL void MMISFR_HandleWifiMsg(MMIWIFI_NOTIFY_EVENT_E event_id,
                                MMIWIFI_NOTIFY_PARAM_T *param_ptr)
{
    BOOLEAN is_busy = FALSE;
    uint32  trans_state = 0;
    SCI_TRACE_LOW("MMISFR_HandleWifiMsg, event_id=%d", event_id);

    switch (event_id)
    {
        case MMIWIFI_NOTIFY_EVENT_ON:
        case MMIWIFI_NOTIFY_EVENT_OFF:
            break;

        case MMIWIFI_NOTIFY_EVENT_CONNECT:
            MMISFR_PdpByPsInterface(FALSE);
            break;

        case MMIWIFI_NOTIFY_EVENT_DISCONNECT:
        case MMIWIFI_NOTIFY_EVENT_FIND_AP:
            break;

        default:
            break;
    }
}

LOCAL void MMISFR_RegWifiNotify(void)
{
    MMIWIFI_NOTIFY_INFO_T  notify_info = {0};

    SCI_TRACE_LOW("MMISFR_RegWifiNotify ...");
    notify_info.module_id = MMI_MODULE_SFR;
    notify_info.notify_func = MMISFR_HandleWifiMsg;
    MMIAPIWIFI_RegNotifyInfo(&notify_info);
}

LOCAL void  MMISFR_HandlePdpMsg(MMIPDP_CNF_INFO_T *msg_ptr)
{
    if (PNULL == msg_ptr)
    {
        return;
    }
    SCI_TRACE_LOW("MMISFR_HandlePdpMsg, msg_id/result=%d, %d", msg_ptr->msg_id,
                  msg_ptr->result);
    switch (msg_ptr->msg_id)
    {
        case MMIPDP_ACTIVE_CNF:
            if (MMIPDP_RESULT_SUCC == msg_ptr->result)
            {
#ifdef LWM2M_SUPPORT
                socket_SetNetId(msg_ptr->nsapi);
                MMISFR_LwM2MEnter();
#endif
                MMIAPIWIFI_DeRegNotifyInfo(MMI_MODULE_SFR);
            }
            else
            {
                MMIAPIPDP_Deactive(MMI_MODULE_SFR);
                s_open_door = TRUE;
            }
            break;

        case MMIPDP_DEACTIVE_CNF:
            break;

        case MMIPDP_DEACTIVE_IND:
#ifdef LWM2M_SUPPORT
            LWM2M_SDK_FINI();
#endif
            MMIAPIPDP_Deactive(MMI_MODULE_SFR);
#ifdef LWM2M_SUPPORT
            MMISFR_ReceivePdpDeactiveInd();
#endif
            //s_open_door = TRUE;
            break;

        default:
            break;
    }

}

LOCAL BOOLEAN MMISFR_PdpByPsInterface(BOOLEAN s_is_gprs)
{
    BOOLEAN return_val = FALSE;
    MMIPDP_ACTIVE_INFO_T    active_info     = {0};

    SCI_TRACE_LOW("MMISFR_PdpByPsInterface s_open_door=%d", s_open_door);
    if (!s_open_door)
    {
        return return_val;
    }

    s_open_door = FALSE;

    SCI_TRACE_LOW("MMISFR_PdpByPsInterface s_is_gprs=%d", s_is_gprs);
    if (s_is_gprs)
    {
        MN_DUAL_SYS_E sim_sys = 0;
        MMICONNECTION_LINKSETTING_DETAIL_T *linksetting = PNULL;
        char   *apn_str = PNULL;
        uint8 linkNum = MMIAPICONNECTION_GetLinkSettingNum(sim_sys);
        int i = 0;
        for (i = 0; i < linkNum; i++)
        {
            linksetting = PNULL;
            linksetting = MMIAPICONNECTION_GetLinkSettingItemByIndex(sim_sys, i);
            if (PNULL != linksetting && 0 != linksetting->apn_len)
            {
                // set apn, username and psw
                active_info.apn_ptr       = linksetting->apn;
                active_info.user_name_ptr = linksetting->username;
                active_info.psw_ptr       = linksetting->password;
                break;
            }
        }
        active_info.dual_sys            = sim_sys;
        active_info.ps_interface        = MMIPDP_INTERFACE_GPRS;
        active_info.ps_service_type = BROWSER_E;
        active_info.storage = MN_GPRS_STORAGE_ALL;
#ifdef IPVERSION_SUPPORT_V4_V6
        active_info.ip_type = MMICONNECTION_IP_V4;
#endif
    }
    else
    {
        active_info.ps_interface = MMIPDP_INTERFACE_WIFI;
    }
    active_info.app_handler = MMI_MODULE_SFR;
    active_info.priority            = 3;
    active_info.ps_service_rat      = MN_UNSPECIFIED;
    active_info.handle_msg_callback = MMISFR_HandlePdpMsg;

    if (MMIAPIPDP_Active(&active_info))
    {
        return_val = TRUE;
    }

    return return_val;
}

extern void MMISFR_RunGPRSModule(void)
{
    MN_DUAL_SYS_E dual_sys   = MN_DUAL_SYS_1;

    SCI_TRACE_LOW("MMISFR_RunGPRSModule, s_open_door=%d", s_open_door);
    if (s_open_door && MMIAPIPHONE_GetSimExistedStatus(dual_sys)
            && MMIPHONE_IsSimOk(dual_sys))
    {
        MMISFR_PdpByPsInterface(TRUE);
    }
}

PUBLIC void MMISFR_InitModule(void)
{
    SCI_TRACE_LOW("MMISFR_InitModule ...");
    MMISFR_RegWifiNotify();
    g_sfr_app.ProcessMsg = MMISFR_HandleModuleMsg;
}

LOCAL MMI_RESULT_E MMISFR_HandleModuleMsg(PWND app_ptr, uint16 msg_id,
        DPARAM param)
{
    SCI_TRACE_LOW("MMISFR_InitModule ...msg_id=%d", msg_id);
    switch (msg_id)
    {
        default:
            break;
    }
    return MMI_RESULT_FALSE;
}

LOCAL void MMISFR_HandleRegOrUpdateTimer(uint8  timer_id, uint32 param)
{
    SCI_TRACE_LOW("MMISFR_HandleRegOrUpdateTimer ...MMIAPIWIFI_GetStatus() = %d",
                  MMIAPIWIFI_GetStatus());
    MMK_StopTimer(timer_id);
    s_open_door = TRUE;
    if (MMIWIFI_STATUS_CONNECTED == MMIAPIWIFI_GetStatus())
    {
        MMISFR_PdpByPsInterface(FALSE);
    }
    else
    {
        MMISFR_RegWifiNotify();
    }
    MMISFR_RunGPRSModule();
}

PUBLIC void MMISFR_RetryRegister(BOOLEAN is_in_register)
{
    SCI_TRACE_LOW("MMISFR_RetryRegister is_in_register=%d", is_in_register);
    MMIAPIPDP_Deactive(MMI_MODULE_SFR);
    if (is_in_register)
    {
        MMK_CreateTimerCallback(REG_RETRY_INTERVAL,
                                MMISFR_HandleRegOrUpdateTimer, NULL,
                                FALSE);
    }
    else
    {
        MMK_CreateTimerCallback(REG_RETRY_INTERVAL_FOR_UPDATE,
                                MMISFR_HandleRegOrUpdateTimer, NULL,
                                FALSE);
    }
}

PUBLIC void MMISFR_RetryUpdate(BOOLEAN first_update)
{
    SCI_TRACE_LOW("MMISFR_RetryUpdate first_update=%d", first_update);
    MMIAPIPDP_Deactive(MMI_MODULE_SFR);
    if (first_update)
    {
        MMK_CreateTimerCallback(FIRST_UPDATE_INTERVAL, MMISFR_HandleRegOrUpdateTimer,
                                NULL, FALSE);
    }
    else
    {
        MMK_CreateTimerCallback(GENERAL_UPDATE_INTERVAL, MMISFR_HandleRegOrUpdateTimer,
                                NULL, FALSE);
    }
}

