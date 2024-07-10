#ifdef WRE_SUPPORT

#include "mmk_app.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmiphone_export.h"
#include "mmipdp_export.h"
#ifdef MMI_WIFI_SUPPORT
#include "mmiwifi_export.h"
#endif
#include "watchdog.h"
#include "../../../../../../wre/wrekernel/include/sdk/connect_type.h"
#include "mmiwre_dial.h"
#include "mmiwre_other.h"
#include "mmiconnection_export.h"
//#include "mmiset.h"
#include "mmiset_export.h"
#include "../../../../../../wre/wrekernel/include/sdk/config.h"

LOCAL WREPDP_MSG_CALLBACK adp_pdp_proc = NULL;
PUBLIC int WRE_GetDefaultConnectLinkIndex(DIAL_TYPE NetType);

/*****************************************************************************/
//  Note: no use
/*****************************************************************************/

WRE_WIFI_STATUS_E WREWIFI_GetStatus(void)
{
    return WRE_WIFI_STATUS_OFF;
}

/*****************************************************************************/
//  Note: no use, only keep symbol
/*****************************************************************************/
uint32 WREWIFI_GetNetId(void)
{
    //return (DWORD)MMIAPIWIFI_GetNetId();
    return 0;
}

/*****************************************************************************/
//  Description : WREWIFI_Deactive
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/

BOOLEAN WREWIFI_Deactive(uint32 hdial)
{
    return MMIAPIPDP_Deactive(hdial);
}


/*****************************************************************************/
//  Description : HandlePsMsg
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/

LOCAL void HandlePsMsg(MMIPDP_CNF_INFO_T *msg_ptr)
{
    WREPDP_CNF_INFO_T   wrepdp_cnf_info = {0};

    wrepdp_cnf_info.app_handler = msg_ptr->app_handler;
    wrepdp_cnf_info.nsapi       = msg_ptr->nsapi;
    wrepdp_cnf_info.result      = msg_ptr->result;

    switch(msg_ptr->result)
    {
        case    MMIPDP_RESULT_SUCC:
            wrepdp_cnf_info.result = WREPDP_RESULT_SUCC;
            break;
        default:
            wrepdp_cnf_info.result = WREPDP_RESULT_FAIL;
            break;
    }
    switch(msg_ptr->msg_id)
    {
        case    MMIPDP_ACTIVE_CNF:
            wrepdp_cnf_info.msg_id = WREPDP_ACTIVE_CNF;
            break;
        case    MMIPDP_DEACTIVE_CNF:
            wrepdp_cnf_info.msg_id = WREPDP_DEACTIVE_CNF;
            break;
        case    MMIPDP_DEACTIVE_IND:
            wrepdp_cnf_info.msg_id = WREPDP_DEACTIVE_IND;
            break;
        case    MMIPDP_ACTIVE_TIMOUT_IND:
            wrepdp_cnf_info.msg_id = WREPDP_ACTIVE_TIMOUT_IND;
            break;
        default:
            wrepdp_cnf_info.msg_id = WREPDP_APP_MSG_INVALID;
            break;
    }

	if(WBOOT_IsWRERunning() && adp_pdp_proc != NULL)
		adp_pdp_proc(&wrepdp_cnf_info);
}

PUBLIC void WREPDP_ResetPdpProc(void)
{
    adp_pdp_proc = NULL;
}

/*****************************************************************************/
//  Description : WREPDP_Active
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/
BOOLEAN WREPDP_Active(WREPDP_ACTIVE_INFO_T *app_info_ptr)
{
	BOOLEAN     ret;
	uint32      wifiState = WIFI_NOT_SUPPORT;
    MMIPDP_ACTIVE_INFO_T pdp_app_info = {0};
    MMICONNECTION_LINKSETTING_DETAIL_T *mmiLinkDetail = NULL;
	
    if (app_info_ptr == NULL)
    {
         return FALSE;
    }
    mmiLinkDetail = MMIAPICONNECTION_GetLinkSettingItemByIndex((MN_DUAL_SYS_E)app_info_ptr->dual_sys, app_info_ptr->iLinkIndex);

    pdp_app_info.app_handler = app_info_ptr->app_handler;
    pdp_app_info.apn_ptr = (char *)mmiLinkDetail->apn;
    pdp_app_info.user_name_ptr = (char *)mmiLinkDetail->username;
    pdp_app_info.psw_ptr = (char *)mmiLinkDetail->password;
    pdp_app_info.ps_interface = (MMIPDP_INTERFACE_E)app_info_ptr->ps_interface;
    pdp_app_info.dual_sys = (MN_DUAL_SYS_E)app_info_ptr->dual_sys;
/*
    switch(app_info_ptr->dual_sys)
    {
        case WRE_DUAL_SYS_1:
            pdp_app_info.dual_sys = MN_DUAL_SYS_1;
            break;
        case WRE_DUAL_SYS_2:
            pdp_app_info.dual_sys = MN_DUAL_SYS_2;
            break;
	case WRE_DUAL_SYS_3:
	     pdp_app_info.dual_sys = MN_DUAL_SYS_3; 
	     break;		
        default:
            pdp_app_info.dual_sys = MN_DUAL_SYS_1;
            break;
    }
*/    
   //if (MMIWIFI_STATUS_CONNECTED != MMIAPIWIFI_GetStatus())
	ret = WRE_GetPhoneInfo((char*)&wifiState,sizeof(uint32),WIFI_INFO_TYPE);
	if(ret == FALSE || wifiState != WIFI_CONNECTED)
   {
        pdp_app_info.ps_interface = MMIPDP_INTERFACE_GPRS;
        app_info_ptr->ps_interface = WRE_MMIPDP_INTERFACE_GPRS;
   }

    pdp_app_info.priority = app_info_ptr->priority;
    pdp_app_info.handle_msg_callback = HandlePsMsg;
    adp_pdp_proc  = app_info_ptr->handle_msg_callback;

    return MMIAPIPDP_Active(&pdp_app_info);
}


/*****************************************************************************/
//  Description : WREPDP_GetDualSimStatus
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/

BOOLEAN WREPDP_GetDualSimStatus(BOOLEAN *is_sim1_ok_ptr, BOOLEAN *is_sim2_ok_ptr)
{
    //return MMIAPIPHONE_GetDualSimStatus(is_sim1_ok_ptr, is_sim2_ok_ptr);
    return FALSE;
}


/*****************************************************************************/
//  Description : WRE_WifiConnect
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/

int WRE_WifiConnect(void *ptr)
{
   return 0;
}

  
/*****************************************************************************/
//  Description : WREPDP_GetConnectionLinkSettingNum
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/

uint8 WREPDP_GetConnectionLinkSettingNum(WRE_DUAL_SYS_E dual_sys)//add by qingjun.yu 20110.06.02
{
    WRE_DUAL_SYS_E dualSys = dual_sys;
    if (/*dualSys < MN_DUAL_SYS_1 || */dualSys >= MN_DUAL_SYS_MAX)
    {
      	  dualSys = (WRE_DUAL_SYS_E)MMIAPISET_GetActiveSim();
    }
	return MMIAPICONNECTION_GetLinkSettingNum((MN_DUAL_SYS_E)dualSys);
}


/*****************************************************************************/
//  Description : WREPDP_GetConnectionLinkSettingDetailByIndex
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/

BOOL WREPDP_GetConnectionLinkSettingDetailByIndex(CONNECT_LINKSETTING_DETAIL *linkDetail, uint8 iLinkIndex, WRE_DUAL_SYS_E dual_sys)
{
    uint8 nLinkNum = 0;
    MMICONNECTION_LINKSETTING_DETAIL_T *mmiLinkDetail = NULL;
    int nLen = 0;

    WRE_DUAL_SYS_E dualSys = dual_sys;
    if (/*dualSys < MN_DUAL_SYS_1 || */dualSys >= MN_DUAL_SYS_MAX)
    {
      	 dualSys = (WRE_DUAL_SYS_E)MMIAPISET_GetActiveSim();
    }

    nLinkNum = MMIAPICONNECTION_GetLinkSettingNum((MN_DUAL_SYS_E)dualSys);

    if (iLinkIndex >= nLinkNum)
   	{
      	 return FALSE;
    }

    memset(linkDetail, 0, sizeof(CONNECT_LINKSETTING_DETAIL));
    mmiLinkDetail = MMIAPICONNECTION_GetLinkSettingItemByIndex((MN_DUAL_SYS_E)dualSys, iLinkIndex);

	   
	nLen = MMIAPICOM_Wstrlen(mmiLinkDetail->name);
	nLen = nLen>WRE_CONNECTION_MAX_LINKMODENAME_LEN?WRE_CONNECTION_MAX_LINKMODENAME_LEN:nLen;
	MMIAPICOM_Wstrncpy(linkDetail->name, mmiLinkDetail->name, nLen);
	linkDetail->name_len = nLen;

	nLen = strlen((char *)mmiLinkDetail->apn);
	nLen = nLen>WRE_CONNECTION_MAX_APN_LEN?WRE_CONNECTION_MAX_APN_LEN:nLen;
	strncpy((char *)linkDetail->apn,  (char *)mmiLinkDetail->apn, nLen);
	linkDetail->apn_len = nLen;

	 nLen = strlen((char *)mmiLinkDetail->gateway);
	 nLen = nLen>WRE_CONNECTION_MAX_GATEWAY_LEN?WRE_CONNECTION_MAX_GATEWAY_LEN:nLen;
	strncpy((char *)linkDetail->gateway, (char *)mmiLinkDetail->gateway, nLen); 	
	linkDetail->gateway_len = nLen;

	linkDetail->port = mmiLinkDetail->port;

	return TRUE;
}


/*****************************************************************************/
//  Description : WREPDP_GetDefaultConnectLinkIndex
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/

int WREPDP_GetDefaultConnectLinkIndex(DIAL_NET_TYPE NetType)
{
	return WRE_GetDefaultConnectLinkIndex((DIAL_TYPE)NetType);
}

#endif
