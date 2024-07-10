#include "std_header.h"
#include "os_api.h"
#include "sci_types.h"
#include "mmk_app.h"
#include "http_api.h"
#include "version.h"
#include "mmipdp_export.h"
#include "in_message.h"
#include "mmipdp_internal.h"
#include "mmi_module.h"
#include "mmiphone_export.h"

#include "sfr_carrier_manager.h"
#include "sfr_header.h"
#include "sfr_nv.h"

MMI_APPLICATION_T g_need_sfr_app = {0};
SFR_LINKER g_sfr_linker = {0};

#if defined(MMI_GPRS_SUPPORT)

LOCAL void HandleSlfRegPdpMsg(MMIPDP_CNF_INFO_T *msg_ptr)
{
    if(PNULL == msg_ptr || msg_ptr->app_handler != MMI_MODULE_SFR)
    {
        return;
    }
    SCI_TraceLow("[Self Reg] HandleSlfRegPdpMsg -- msg = %d", msg_ptr->msg_id);

    switch(msg_ptr->msg_id)
    {
        case MMIPDP_ACTIVE_CNF:     //PDP激活成功
            SCI_TraceLow("[Self Reg] MMIPDP_ACTIVE_CNF--- result = %d", msg_ptr->result);
            if(MMIPDP_RESULT_SUCC == msg_ptr->result)
            {
                SCI_TraceLow("[Self Reg] MMIPDP_ACTIVE_CNF--- net id  = %d", msg_ptr->nsapi);

                g_sfr_linker.net_id = msg_ptr->nsapi;
                SCI_MEMSET(&g_sfr_linker.http_ctx, 0, sizeof(SFR_HTTP_CONTEXT));
                SlfReg_StartHttpRequest(g_sfr_linker.net_id);
            }
            else
            {
                SCI_TraceLow("[Self Reg] MMIPDP_ACTIVE_CNF--- net id = %d", g_sfr_linker.http_ctx.context_id);//1456903
                g_sfr_linker.http_ctx.context_id = 0;
                MMISFR_ReTry(10);
            }
            break;

        case MMIPDP_DEACTIVE_CNF:   //PDP去激活成功
            SCI_TraceLow("[Self Reg] MMIPDP_DEACTIVE_CNF");
            g_sfr_linker.net_id = 0;
            break;

        case MMIPDP_DEACTIVE_IND:   //PDP被网络端去激活
            SCI_TraceLow("[Self Reg] MMIPDP_DEACTIVE_IND");
            g_sfr_linker.net_id = 0;
            MMIAPIPDP_Deactive(MMI_MODULE_SFR);
            break;

        default:
            break;
    }
}

#endif

BOOLEAN MMISFR_ConnectPDP(void)
{
#if defined(MMI_GPRS_SUPPORT)
    MMIPDP_ACTIVE_INFO_T    active_info     = {0};
    MMICONNECTION_APNTYPEINDEX_T *apn_idx_arr;
    MMICONNECTION_LINKSETTING_DETAIL_T* linksetting = PNULL;

    SCI_TraceLow("[Self Reg] MMISFR_ConnectPDP");

    if (FALSE == MMIAPIPHONE_GetDataServiceSIM(&g_sfr_linker.sim_sys))
    {
        SCI_TraceLow("[Self Reg] MMISFR_ConnectPDP [SIM ERROR]");
        return FALSE;
    }

    active_info.app_handler         = MMI_MODULE_SFR;
    active_info.dual_sys            = g_sfr_linker.sim_sys;
    active_info.apn_ptr             = NULL;
    active_info.user_name_ptr       = NULL;
    active_info.psw_ptr             = NULL;
    active_info.priority            = 3;
    active_info.ps_service_rat      = MN_TD_PREFER;
    active_info.ps_interface        = MMIPDP_INTERFACE_GPRS;
    active_info.handle_msg_callback = HandleSlfRegPdpMsg;
    active_info.ps_service_type     = BROWSER_E;
    active_info.storage             = MN_GPRS_STORAGE_ALL;

    apn_idx_arr = MMIAPICONNECTION_GetApnTypeIndex(g_sfr_linker.sim_sys);
    if (apn_idx_arr == NULL)
    {
        SCI_TraceLow("[Self Reg] MMISFR_ConnectPDP [APN IDX ERROR]");
        return FALSE;
    }
    SCI_TraceLow("[Self Reg] MMISFR_ConnectPDP [sim = %d, Net Setting = %d]", g_sfr_linker.sim_sys, apn_idx_arr->internet_index[g_sfr_linker.sim_sys].index);
    linksetting = MMIAPICONNECTION_GetLinkSettingItemByIndex(g_sfr_linker.sim_sys, apn_idx_arr->internet_index[g_sfr_linker.sim_sys].index);

    if(PNULL != linksetting && 0 != linksetting->apn_len)
    {
        active_info.apn_ptr = (char*)linksetting->apn;
        active_info.user_name_ptr = (char*)linksetting->username;
        active_info.psw_ptr = (char*)linksetting->password;
#ifdef IPVERSION_SUPPORT_V4_V6
        active_info.ip_type   = linksetting->ip_type;
#endif
        if(MMIAPIPDP_Active(&active_info))
        {
            return TRUE;
        }
    }
    SCI_TraceLow("[Self Reg] MMISFR_ConnectPDP [Net Setting ERROR]");
#endif
    return FALSE;
}

LOCAL MMI_RESULT_E SlfReg_HandleModuleMsg(PWND app_ptr, uint16 msg_id, DPARAM param)
{
    SCI_TraceLow("[Self Reg] SlfReg_HandleModuleMsg msg id = %d", msg_id);

    if (HTTP_SIGNAL_BEG <= msg_id && msg_id <= HTTP_SIGNAL_END)
    {
        SCI_TraceLow("[Self Reg] SlfReg_HandleModuleMsg msg_id = %d [HTTP_SIGNAL_BEG = %d]", msg_id, HTTP_SIGNAL_BEG);
        return SlfReg_HandleHttpMsg(app_ptr, msg_id, param, &g_sfr_linker.http_ctx);
    }
    return MMI_RESULT_FALSE;
}

LOCAL void SlfReg_HandlePdpEvent(MN_DUAL_SYS_E dual_sys, MMIPDP_NOTIFY_EVENT_E notify_event)
{
    if(g_sfr_linker.flags == FALSE)
    {
        if(MMIPDP_NOTIFY_EVENT_ACTIVED == notify_event)
        {
            SCI_TraceLow("[Self Reg] MMIPDP_NOTIFY_EVENT_ACTIVED");

            g_sfr_linker.flags = TRUE;
            g_sfr_linker.sim_sys = dual_sys;
        }
    }
}

LOCAL void HandleSFRNotifyEventCallback(MN_DUAL_SYS_E dual_sys, MMIPHONE_NOTIFY_EVENT_E notify_event, uint32 param)
{
    SCI_TraceLow("[Self Reg]: HandleSFRNotifyEventCallback dual_sys=%d, notify_event=%d, param=%d", dual_sys, notify_event, param);

    //无网恢复有网时
    if(MMIPHONE_NOTIFY_EVENT_CS_AVAIL == notify_event)
    {
        // 启动一次自注册
        // 如果之前没有注册成功，会重新开始注册
        // 如果已经注册过，不会重新注册
        if((g_sfr_linker.carrier = SFRM_GetFirstRegCarrier()) != NULL)
        {
            SCI_TraceLow("[Self Reg]: HandleSFRNotifyEventCallback : CS Avail -- start SFR.");
            MMISFR_Http_Run();
        }
    }
}

PUBLIC void SFR_InitModule(void)
{
    MMIPHONE_NOTIFY_INFO_T notify_info = {0};
    MMIPDP_NOTIFY_EVENT_E pdp_event[MMIPDP_NOTIFY_EVENT_MAX] = {
        MMIPDP_NOTIFY_EVENT_ACTIVED,
        MMIPDP_NOTIFY_EVENT_DEACTIVED,
        MMIPDP_NOTIFY_EVENT_ATTACH,
        MMIPDP_NOTIFY_EVENT_DETTACH
    };

    SCI_TraceLow("[Self Reg] SFR_InitModule");

    MMIAPIPDP_SubscribeEvent(MMI_MODULE_SFR,
            SlfReg_HandlePdpEvent,
            MMIPDP_NOTIFY_EVENT_MAX,
            (MMIPDP_NOTIFY_EVENT_E *)pdp_event);

    g_need_sfr_app.ProcessMsg = SlfReg_HandleModuleMsg;

    MMISFR_RegNv();

    notify_info.module_id = MMI_MODULE_SFR;
    notify_info.notify_func = HandleSFRNotifyEventCallback;

    MMIAPIPHONE_RegisterNotifyInfo(&notify_info);
}

