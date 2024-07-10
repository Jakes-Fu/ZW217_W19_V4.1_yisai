/*****************************************************************************
** File Name:      mmisfr_ctcc_pdp.c
** Author:
** Copyright (C) 2000-2100, UNISOC Technoloies Co.,Ltd.
** Description:    Active PDP
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE                       NAME                          DESCRIPTION
** 2020/11/16           mark.zhang1             Create
******************************************************************************/

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "os_api.h"
#include "mmi_module.h"
#include "mmipdp_export.h"
#include "in_message.h"
#include "mmk_timer.h"
#ifdef MMI_WIFI_SUPPORT
#include "mmiwifi_export.h"
#endif
#include "sig_code.h"
#include "mmisfr_ctcc_pdp.h"
#include "mmisfr_ctcc_http.h"
#include "mmiconnection_export.h"

/**-------------------------------------------------------------------------*
**                         STATIC DEFINITION                       *
**--------------------------------------------------------------------------*/
// allow to use pdp or not
LOCAL BOOLEAN s_allow_pdp = TRUE;

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description: active pdp
//  Param:
//  Author:
/*****************************************************************************/
LOCAL BOOLEAN sfr_active_pdp(BOOLEAN is_gprs);

/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/
#ifdef MMI_WIFI_SUPPORT
LOCAL void handle_wifi_msg(MMIWIFI_NOTIFY_EVENT_E event_id,
                           MMIWIFI_NOTIFY_PARAM_T *param_ptr)
{
    SCI_TRACE_LOW("MMISFR_CTCC_HandleWifiMsg, event_id=%d", event_id);
    switch (event_id)
    {
        case MMIWIFI_NOTIFY_EVENT_CONNECT:
            sfr_active_pdp(FALSE);
            break;

        default:
            break;
    }
}

LOCAL void reg_wifi_event(void)
{
    MMIWIFI_NOTIFY_INFO_T  notify_info = {0};

    notify_info.module_id = MMI_MODULE_SFR_CTCC;
    notify_info.notify_func = handle_wifi_msg;
    MMIAPIWIFI_RegNotifyInfo(&notify_info);//make sure- reg once
}
#endif

LOCAL void handle_fail_ind()
{
    sfr_set_allow_pdp_flag(TRUE);//reset flag
#ifdef MMI_WIFI_SUPPORT
    reg_wifi_event();
#endif
}

LOCAL void  handle_pdp_msg(MMIPDP_CNF_INFO_T *msg_ptr)
{
    if (PNULL == msg_ptr)
    {
        return;
    }

    SCI_TRACE_LOW("[DMCTCC] handle_pdp_msg, msg_id/result=%d, %d",
                  msg_ptr->msg_id,
                  msg_ptr->result);
    switch (msg_ptr->msg_id)
    {
        case MMIPDP_ACTIVE_CNF:
            if (MMIPDP_RESULT_SUCC == msg_ptr->result)
            {
#ifdef MMI_WIFI_SUPPORT
                MMIAPIWIFI_DeRegNotifyInfo(MMI_MODULE_SFR_CTCC);
#endif
#ifdef HTTP_SUPPORT
                sfr_reg_by_http(msg_ptr->nsapi);
#endif
            }
            else
            {
                MMIAPIPDP_Deactive(MMI_MODULE_SFR_CTCC);
                handle_fail_ind();
                //MMISFR_CTCC_RunGPRSModule();
            }
            break;

        case MMIPDP_DEACTIVE_CNF: //deactive pdp by app
            break;

        case MMIPDP_DEACTIVE_IND: //network disconnected
            MMIAPIPDP_Deactive(MMI_MODULE_SFR_CTCC);
#ifdef HTTP_SUPPORT
            sfr_notify_deactive_ind();
#endif
            break;

        default:
            break;
    }
}

LOCAL BOOLEAN sfr_active_pdp(BOOLEAN is_gprs)
{
    BOOLEAN result = FALSE;
	MMIPDP_ACTIVE_INFO_T    active_info     = {0};

    SCI_TRACE_LOW("[DMCTCC] sfr_active_pdp, s_is_gprs=%d, s_allow_pdp=%d", is_gprs,
                  s_allow_pdp);
    if (!s_allow_pdp)
    {
        return result;
    }
    sfr_set_allow_pdp_flag(FALSE);

    
    if (is_gprs)
    {
        MN_DUAL_SYS_E sim_sys = MN_DUAL_SYS_1;
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
    active_info.app_handler = MMI_MODULE_SFR_CTCC;
    active_info.priority            = 3;
    active_info.ps_service_rat      = MN_UNSPECIFIED;
    active_info.handle_msg_callback = handle_pdp_msg;

    if (MMIAPIPDP_Active(&active_info))
    {
        SCI_TRACE_LOW("[DMCTCC] sfr_active_pdp, MMIAPIPDP_Active TRUE");
        return TRUE;
    }
    handle_fail_ind();

    return result;
}

PUBLIC void sfr_deactive_app(void)
{
    MMIAPIPDP_Deactive(MMI_MODULE_SFR_CTCC);
}

PUBLIC void sfr_set_allow_pdp_flag(BOOLEAN status)
{
    s_allow_pdp = status;
}

PUBLIC void sfr_trigger_pdp(void)
{
    SCI_TRACE_LOW("[DMCTCC] sfr_trigger_pdp, s_allow_pdp=%d", s_allow_pdp);
    if (!s_allow_pdp)
    {
        return;
    }
#ifdef MMI_WIFI_SUPPORT
    if (MMIWIFI_STATUS_CONNECTED == MMIAPIWIFI_GetStatus())   //choose wifi first
    {
        sfr_active_pdp(FALSE);
    }
    else if (!MMIAPICONNECTION_isGPRSSwitchedOff())     // gprs switch status
    {
        sfr_active_pdp(TRUE);
    }
    else
    {
        reg_wifi_event();
    }
#else
    if (!MMIAPICONNECTION_isGPRSSwitchedOff())     // gprs switch status
    {
        sfr_active_pdp(TRUE);
    }
#endif
}

