/*************************************************************************
** File Name:      mmiwifi_nv.c                                         *
** Author:         li.li                                                *
** Date:           2009/12/03                                           *
** Copyright:      2006 Spreadtrum, Incorporated. All Rights Reserved.  *
** Description:     This file defines the function about nv             *
*************************************************************************
*************************************************************************
**                        Edit History                                  *
** ---------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                          *
** 2009/12/03     li.li            Create.                              *
*************************************************************************/

#include "mmi_app_wlan_trc.h"
#include "sci_types.h"
#include "mmi_modu_main.h"
#include "mmiwlan_nv.h"
#include "mmiidle_export.h"
#include "mmiwlan_internal.h"
#include "mmiwifi_export.h"
#include "mmiwlan_portal.h"
#include "mmi_nv.h"
#include "mmicom_trace.h"

/*the length of wifi nv*/
const uint16 mmiwifi_nv_len[] =
{      
    sizeof(BOOLEAN),                    //is stopped by user
    sizeof(BOOLEAN),                    //is active Service
    sizeof(uint16),                     //which set package
    sizeof(MMIWIFI_ACCOUNT_INFO_T),     //Default AP account
    sizeof(BOOLEAN),                    //Default AP auto connect
    sizeof(BOOLEAN),                     //MMIWIFI_WLAN_STATUS,
    sizeof(BOOLEAN),                     //MMIWIFI_WLAN_AUTO_CONNECT,
    sizeof(uint16),                    //MMIWIFI_WLAN_SEARCH_PERIOD
    sizeof(BOOLEAN),                     //MMIWIFI_IP_TYPE,0为动态，1为静态
    sizeof(uint32),                     //IP地址类型TCPIP_IPADDR_T即为uint32
    sizeof(uint32),                     //GATEWAY地址
    sizeof(uint32),                     //SUBMASK地址
    sizeof(uint32),                     //DNS1地址
    sizeof(uint32),                     //DNS2地址
    sizeof(BOOLEAN),                     //MMIWIFI_PROXY_QUERY,0为否，1为是
    MMIWIFI_URL_MAX_LEN + 1,                     //代理IP地址
    sizeof(uint16),       //代理端口
    sizeof(MMIWIFI_PROXY_NAME_E),       //代理用户名
    sizeof(MMIWIFI_PROXY_PASSWORD_E),    //代理用户密码   
    sizeof(MMIWIFI_PROXY_REJECTLIST_E),  //代理设置排除列表
    sizeof(MMIWIFI_PORTAL_CONFIG_T),
    sizeof(MMI_GPRSTOWLAN_RESELECT),
    sizeof(MMI_AP_RESELECT),
    sizeof(BOOLEAN),
    sizeof(char) * MMIWIFI_PORTAL_VERSION_LEN_MAX,
    sizeof(char) * MMIWIFI_PORTAL_SERVICE_PROVIDER_NUM_MAX,
    sizeof(char) * MMIWIFI_PORTAL_COMMAND_LEN_MAX,
    sizeof(char) * MMIWIFI_PORTAL_COMMAND_LEN_MAX,
    sizeof(char) * MMIWIFI_PORTAL_COMMAND_LEN_MAX,
    sizeof(char) * MMIWIFI_PORTAL_LOGIN_FORM_LEN_MAX,
    sizeof(char) * MMIWIFI_PORTAL_LOGIN_FORM_LEN_MAX,
    sizeof(BOOLEAN),                    //MMIWIFI_MONITOR_WAKEUP_PACKET
    sizeof(uint32),                      //MMIWIFI_WAKEUP_PACKET_THRESHOLD
};

/*****************************************************************************/
//     Description : register wifi module nv len and max item
//    Global resource dependence : none
//  Author: li.li
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIWIFI_RegWifiNv(void)
{
    MMI_RegModuleNv(MMI_MODULE_WIFI, mmiwifi_nv_len, sizeof(mmiwifi_nv_len) / sizeof(uint16));
    return TRUE;
}

/*****************************************************************************/
//  Description : get is active service
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIWIFI_GetIsServiceOn(void)
{
    BOOLEAN is_active = FALSE;
    MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;
    
    MMINV_READ(MMIWIFI_DEF_AP_SERVICE_ON, &is_active, return_value);
    
    if (MN_RETURN_SUCCESS != return_value) {
        is_active = FALSE;
        MMIWIFI_SetIsServiceOn(FALSE);
    }
    
    return is_active;
}

/*****************************************************************************/
//  Description : Set is active service
//  Global resource dependence : 
//  Author: jiaoyou.wu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWIFI_SetIsServiceOn(BOOLEAN is_active)
{
    MMINV_WRITE(MMIWIFI_DEF_AP_SERVICE_ON, &is_active);
}

/*****************************************************************************/
//  Description : get is active service
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIWIFI_GetIsAutoConnectingOn(void)
{
    BOOLEAN is_auto_connecting = FALSE;
    MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;
    
    MMINV_READ(MMIWIFI_DEF_AP_AUTO_CONNECT, &is_auto_connecting, return_value);
    
    if (MN_RETURN_SUCCESS != return_value) {
        is_auto_connecting = TRUE;
        MMIWIFI_SetIsAutoConnectingOn(TRUE);
    }

    return is_auto_connecting;
}

/*****************************************************************************/
//  Description : Set is active service
//  Global resource dependence : 
//  Author: jiaoyou.wu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWIFI_SetIsAutoConnectingOn(BOOLEAN is_auto_connecting)
{
    MMINV_WRITE(MMIWIFI_DEF_AP_AUTO_CONNECT, &is_auto_connecting);
}

/*****************************************************************************/
//  Description : get account info
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWIFI_GetAccountInfo(MMIWIFI_ACCOUNT_INFO_T *account_info)
{
    MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;
    
    if (PNULL == account_info) {
        return;
    }
    
    MMINV_READ(MMIWIFI_DEF_AP_ACCOUNT_INFO, account_info, return_value);
    
    if (MN_RETURN_SUCCESS != return_value) {
        MMIWIFI_ACCOUNT_INFO_T init_info = {0};
        
        SCI_MEMCPY(account_info, &init_info, sizeof(MMIWIFI_ACCOUNT_INFO_T));
        MMIWIFI_SetAccountInfo(account_info);
    }
}

/*****************************************************************************/
//  Description : Set account info
//  Global resource dependence : 
//  Author: jiaoyou.wu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWIFI_SetAccountInfo(MMIWIFI_ACCOUNT_INFO_T *account_info)
{
    MMINV_WRITE(MMIWIFI_DEF_AP_ACCOUNT_INFO, account_info);
}

/*****************************************************************************/
//  Description : MMIWIFI_GetIsWlanOn
//  Global resource dependence : 
//  Author: 
//  Note: 0为关闭，1为打开
/*****************************************************************************/
PUBLIC BOOLEAN MMIWIFI_GetIsWlanOn(void)
{
    BOOLEAN is_on = FALSE;
    MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;
    
    MMINV_READ(MMIWIFI_WLAN_STATUS,&is_on,return_value);
    
    if (MN_RETURN_SUCCESS != return_value)
    {
        is_on = FALSE;
        MMIWIFI_SetIsWlanOn(is_on);
    }

    //SCI_TRACE_LOW:"MMIWIFI_GetIsWlanOn is_on = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_NV_171_112_2_18_3_9_35_91,(uint8*)"d", is_on);

    return is_on;
}

/*****************************************************************************/
//  Description : MMIWIFI_SetIsWlanOn
//  Global resource dependence : 
//  Author: 
//  Note: 0为关闭，1为打开
/*****************************************************************************/
PUBLIC void MMIWIFI_SetIsWlanOn(BOOLEAN is_on)
{
    MMINV_WRITE(MMIWIFI_WLAN_STATUS, &is_on);
}

/*****************************************************************************/
//  Description : MMIWIFI_GetIsAutoConnect
//  Global resource dependence : 
//  Author:
//  Note: 0为关闭，1为打开
/*****************************************************************************/
PUBLIC BOOLEAN MMIWIFI_GetIsAutoConnect(void)
{
    BOOLEAN is_on = FALSE;
    MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;
    
    MMINV_READ(MMIWIFI_WLAN_AUTO_CONNECT,&is_on,return_value);
    
    if (MN_RETURN_SUCCESS != return_value)
    {
        is_on = TRUE;
        MMIWIFI_SetIsAutoConnect(is_on);
    }
    return is_on;
}

/*****************************************************************************/
//  Description : MMIWIFI_SetIsAutoConnect
//  Global resource dependence : 
//  Author: 
//  Note: 0为关闭，1为打开
/*****************************************************************************/
PUBLIC void MMIWIFI_SetIsAutoConnect(BOOLEAN is_on)
{
    MMINV_WRITE(MMIWIFI_WLAN_AUTO_CONNECT, &is_on);
}

/*****************************************************************************/
//  Description : MMIWIFI_GetWlanIPType
//  Global resource dependence : 
//  Author:
//  Note: 0为动态IP，1为静态IP
/*****************************************************************************/
PUBLIC BOOLEAN MMIWIFI_GetIPType(void)
{
    BOOLEAN index= 0;
    MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;
    
    MMINV_READ(MMIWIFI_IP_TYPE,&index,return_value);
    
    if (MN_RETURN_SUCCESS != return_value)
    {
        index = 0;
        MMIWIFI_SetIPType(index);
    }
    return index;
}

/*****************************************************************************/
//  Description : MMIWIFI_SetWlanIPType
//  Global resource dependence : 
//  Author: 
//  Note: 0为动态IP，1为静态IP
/*****************************************************************************/
PUBLIC void MMIWIFI_SetIPType(BOOLEAN index)
{
    MMINV_WRITE(MMIWIFI_IP_TYPE,&index);
}

/*****************************************************************************/
//  Description : MMIWIFI_GetWlanIPType
//  Global resource dependence : 
//  Author:
//  Note: 0为不使用代理，1为使用代理
/*****************************************************************************/
PUBLIC BOOLEAN MMIWIFI_GetProxyQuery(void)
{
    BOOLEAN index= 0;
    MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;
    
    MMINV_READ(MMIWIFI_PROXY_QUERY,&index,return_value);
    
    if (MN_RETURN_SUCCESS != return_value)
    {
        index = 0;
        MMIWIFI_SetProxyQuery(index);
    }
    return index;
}

/*****************************************************************************/
//  Description : MMIWIFI_SetWlanIPType
//  Global resource dependence : 
//  Author: 
//  Note: 0为不使用代理，1为使用代理
/*****************************************************************************/
PUBLIC void MMIWIFI_SetProxyQuery(BOOLEAN index)
{
    MMINV_WRITE(MMIWIFI_PROXY_QUERY,&index);
}

/*****************************************************************************/
//  Description : MMIWIFI_GetIPData
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMIWIFI_GetIPData(void)
{
    uint32 index= 0;
    MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;
    
    MMINV_READ(MMIWIFI_IP_DATA,&index,return_value);
    
    if (MN_RETURN_SUCCESS != return_value)
    {
        index = 0;
        MMIWIFI_SetIPData(index);
    }
    return index;
}

/*****************************************************************************/
//  Description : MMIWIFI_SetIPData
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWIFI_SetIPData(uint32 index)
{
    MMINV_WRITE(MMIWIFI_IP_DATA,&index);
}

/*****************************************************************************/
//  Description : MMIWIFI_GetGateWay
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMIWIFI_GetGateWay(void)
{
    uint32 index= 0;
    MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;
    
    MMINV_READ(MMIWIFI_GATEWAY,&index,return_value);
    
    if (MN_RETURN_SUCCESS != return_value)
    {
        index = 0;
        MMIWIFI_SetGateWay(index);
    }
    return index;
}

/*****************************************************************************/
//  Description : MMIWIFI_SetGateWay
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWIFI_SetGateWay(uint32 index)
{
    MMINV_WRITE(MMIWIFI_GATEWAY,&index);
}

/*****************************************************************************/
//  Description : MMIWIFI_GetSubMask
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMIWIFI_GetSubMask(void)
{
    uint32 index= 0;
    MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;
    
    MMINV_READ(MMIWIFI_SUBMASK,&index,return_value);
    
    if (MN_RETURN_SUCCESS != return_value)
    {
        index = 0;
        MMIWIFI_SetSubMask(index);
    }
    return index;
}

/*****************************************************************************/
//  Description : MMIWIFI_SetSubMask
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWIFI_SetSubMask(uint32 index)
{
    MMINV_WRITE(MMIWIFI_SUBMASK,&index);
}

/*****************************************************************************/
//  Description : MMIWIFI_GetDNS1
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMIWIFI_GetDNS1(void)
{
    uint32 index= 0;
    MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;
    
    MMINV_READ(MMIWIFI_DNS1,&index,return_value);
    
    if (MN_RETURN_SUCCESS != return_value)
    {
        index = 0;
        MMIWIFI_SetDNS1(index);
    }
    return index;
}

/*****************************************************************************/
//  Description : MMIWIFI_SetDNS1
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWIFI_SetDNS1(uint32 index)
{
    MMINV_WRITE(MMIWIFI_DNS1,&index);
}

/*****************************************************************************/
//  Description : MMIWIFI_GetDNS2
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMIWIFI_GetDNS2(void)
{
    uint32 index= 0;
    MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;
    
    MMINV_READ(MMIWIFI_DNS2,&index,return_value);
    
    if (MN_RETURN_SUCCESS != return_value)
    {
        index = 0;
        MMIWIFI_SetDNS2(index);
    }
    return index;
}

/*****************************************************************************/
//  Description : MMIWIFI_SetDNS2
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWIFI_SetDNS2(uint32 index)
{
    MMINV_WRITE(MMIWIFI_DNS2,&index);
}

/*****************************************************************************/
//  Description : MMIWIFI_GetProxyURL
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWIFI_GetProxyURL(char *out_url, int16 max_len)
{
    char url[MMIWIFI_URL_MAX_LEN + 1] = {0};
    MN_RETURN_RESULT_E  return_value = MN_RETURN_FAILURE;

    if (PNULL == out_url)
    {
        return;
    }

    max_len = MIN(max_len, MMIWIFI_URL_MAX_LEN);

    MMINV_READ(MMIWIFI_PROXY_IP, url, return_value);

    if (MN_RETURN_SUCCESS != return_value)
    {
        MMIWIFI_SetProxyURL("");
    }

    SCI_MEMCPY(out_url, url, max_len);
}

/*****************************************************************************/
//  Description : MMIWIFI_SetProxyURL
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWIFI_SetProxyURL(char *in_url)
{
    int16       url_len = 0;
    char        url[MMIWIFI_URL_MAX_LEN + 1] = {0};

    if (PNULL == in_url)
    {
        return;
    }

    url_len = SCI_STRLEN(in_url);
    url_len = MIN(url_len, MMIWIFI_URL_MAX_LEN);

    SCI_MEMCPY(url, in_url, url_len);

    MMINV_WRITE(MMIWIFI_PROXY_IP,url);
}

/*****************************************************************************/
//  Description : MMIWIFI_GetProxyPort
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWIFI_GetProxyPort(uint16 * index)
{
    MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;
    
    MMINV_READ(MMIWIFI_PROXY_PROT,index,return_value);
    
    if (MN_RETURN_SUCCESS != return_value)
    {
        SCI_MEMSET(index,0,sizeof(uint16));
        MMIWIFI_SetProxyPort(index);
    }
}

/*****************************************************************************/
//  Description : MMIWIFI_SetProxyPort
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWIFI_SetProxyPort(uint16 * index)
{
    MMINV_WRITE(MMIWIFI_PROXY_PROT,index);
}

/*****************************************************************************/
//  Description : MMIWIFI_GetProxyPort
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWIFI_GetProxyName(MMIWIFI_PROXY_NAME_E * index)
{
    MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;
    
    MMINV_READ(MMIWIFI_PROXY_NAME,index,return_value);
    
    if (MN_RETURN_SUCCESS != return_value)
    {
        SCI_MEMSET(index,0,sizeof(MMIWIFI_PROXY_NAME_E));
        MMIWIFI_SetProxyName(index);
    }
}

/*****************************************************************************/
//  Description : MMIWIFI_SetProxyPort
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWIFI_SetProxyName(MMIWIFI_PROXY_NAME_E * index)
{
    MMINV_WRITE(MMIWIFI_PROXY_NAME,index);
}

/*****************************************************************************/
//  Description : MMIWIFI_GetProxyPassword
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWIFI_GetProxyPassword(MMIWIFI_PROXY_PASSWORD_E * index)
{
    MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;
    
    MMINV_READ(MMIWIFI_PROXY_PASSWORD,index,return_value);
    
    if (MN_RETURN_SUCCESS != return_value)
    {
        SCI_MEMSET(index,0,sizeof(MMIWIFI_PROXY_PASSWORD_E));
        MMIWIFI_SetProxyPassword(index);
    }
}

/*****************************************************************************/
//  Description : MMIWIFI_SetProxyPassword
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWIFI_SetProxyPassword(MMIWIFI_PROXY_PASSWORD_E * index)
{
    MMINV_WRITE(MMIWIFI_PROXY_PASSWORD,index);
}

/*****************************************************************************/
//  Description : MMIWIFI_GetProxyRejectList
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWIFI_GetProxyRejectList(MMIWIFI_PROXY_REJECTLIST_E * index)
{
    MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;
    
    MMINV_READ(MMIWIFI_PROXY_REJECTLIST,index,return_value);
    
    if (MN_RETURN_SUCCESS != return_value)
    {
        SCI_MEMSET(index,0,sizeof(MMIWIFI_PROXY_REJECTLIST_E));
        MMIWIFI_SetProxyRejectList(index);
    }
}

/*****************************************************************************/
//  Description : MMIWIFI_SetProxyRejectList
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWIFI_SetProxyRejectList(MMIWIFI_PROXY_REJECTLIST_E * index)
{
    MMINV_WRITE(MMIWIFI_PROXY_REJECTLIST,index);
}

/*****************************************************************************/
//  Description : MMIWIFI_GetWlanSearchPeriod
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC uint16 MMIWIFI_GetWlanAutoScanStatus(void)
{
    uint16 index = 0;
    MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;
    
    MMINV_READ(MMIWIFI_WLAN_AUTO_SCAN_STATUS,&index,return_value);
    
    if (MN_RETURN_SUCCESS != return_value)
    {
#ifdef MMI_WLAN_TAB_STYLE
        index = 0;
#else
        index = 1;
#endif /* MMI_WLAN_TAB_STYLE */
        MMIWIFI_SetWlanAutoScanStatus(index);
    }
    return index;
}
/*****************************************************************************/
//  Description : MMIWIFI_SetWlanSearchPeriod
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWIFI_SetWlanAutoScanStatus(uint16 index)
{
    MMINV_WRITE(MMIWIFI_WLAN_AUTO_SCAN_STATUS,&index);
}

/****************************************************************************/
//  Description : MMIAPIWIFI_GetProxySettingDetail
//  Global resource dependence : 
//  Author:
//  Note:该接口用于浏览器调用
/****************************************************************************/
PUBLIC void MMIAPIWIFI_GetProxySettingDetail(MMIWLAN_PROXY_SETTING_DETAIL_T *detail_struct)
{
    uint16 port = 0;
    MMIWIFI_PROXY_NAME_E proxyname = {0};
    MMIWIFI_PROXY_PASSWORD_E proxypassword = {0};

    detail_struct->is_use_proxy = MMIWIFI_GetProxyQuery();//是否使用代理
    
    MMIWIFI_GetProxyURL((char *)detail_struct->gateway, PROXY_PORT_MAX_LEN);//代理IP,代理网关
    detail_struct->gateway_len = SCI_STRLEN((char *)detail_struct->gateway);

    MMIWIFI_GetProxyPort(&port);//代理端口
    detail_struct->port = port;
    
    MMIWIFI_GetProxyName(&proxyname);//代理用户名
    MMIAPICOM_WstrToStr(proxyname.proxy_name, detail_struct->username);
    detail_struct->username_len = strlen((char *)detail_struct->username);
    
    MMIWIFI_GetProxyPassword(&proxypassword);//代理密码
    MMIAPICOM_WstrToStr(proxypassword.proxy_password, detail_struct->password);
    detail_struct->password_len= strlen((char *)detail_struct->password);

    //排除列表
    MMIWIFI_GetProxyRejectList(&(detail_struct->rejectlist));

}

/*****************************************************************************/
//  Description : Get Portal Config
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWIFI_GetPortalConfig(MMIWIFI_PORTAL_CONFIG_T *portal_cfg_ptr)
{
    MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;

    if (PNULL == portal_cfg_ptr)
    {
        return;
    }
    
    MMINV_READ(MMIWIFI_PORTAL_CONFIG, portal_cfg_ptr, return_value);
    
    if (MN_RETURN_SUCCESS != return_value)
    {
        SCI_MEMSET(portal_cfg_ptr, 0x00, sizeof(MMIWIFI_PORTAL_CONFIG_T));

#ifndef MMI_GUI_STYLE_TYPICAL
                portal_cfg_ptr->is_remember_password = TRUE;
                portal_cfg_ptr->is_notify = TRUE;
#endif /* MMI_GUI_STYLE_TYPICAL */

        MMINV_WRITE(MMIWIFI_PORTAL_CONFIG, portal_cfg_ptr);
    }
}

/*****************************************************************************/
//  Description : Set Portal Config
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWIFI_SetPortalConfig(MMIWIFI_PORTAL_CONFIG_T *portal_cfg_ptr)
{
    if (PNULL == portal_cfg_ptr)
    {
        return;
    }
    
    MMINV_WRITE(MMIWIFI_PORTAL_CONFIG, portal_cfg_ptr);
}

/*****************************************************************************/
//  Description : MMIWIFI_SetGPRSToWlanResetct
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWIFI_SetGPRSToWlanReselect(MMI_GPRSTOWLAN_RESELECT * index)
{
    if (PNULL == index)
    {
        return;
    }
    
    MMINV_WRITE(MMIWIFI_WLAN_RESELECT, index);
}

/*****************************************************************************/
//  Description : MMIWIFI_GetGPRSToWlanResetct
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC MMI_GPRSTOWLAN_RESELECT MMIWIFI_GetGPRSToWlanReselect(void)
{
    MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;
    MMI_GPRSTOWLAN_RESELECT  index = MMI_GPRSTOWLAN_ALWAYS_INQUIRY;
    MMINV_READ(MMIWIFI_WLAN_RESELECT, &index, return_value);
    
    if (MN_RETURN_SUCCESS != return_value)
    {
        MMINV_WRITE(MMIWIFI_WLAN_RESELECT, &index);
    }
    return index;
}

/*****************************************************************************/
//  Description : MMIWIFI_SetApReselect
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWIFI_SetApReselect( MMI_AP_RESELECT  *index)
{
    if (PNULL == index)
    {
        return;
    }
    
    MMINV_WRITE(MMIWIFI_WLAN_AP_RESELECT, index);
}

/*****************************************************************************/
//  Description : MMIWIFI_GetApReselect
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC MMI_AP_RESELECT MMIWIFI_GetApReselect(void)
{
    MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;
    MMI_AP_RESELECT index = MMI_AP_AUTO_RESELCET;
    MMINV_READ(MMIWIFI_WLAN_AP_RESELECT, &index, return_value);
    
    if (MN_RETURN_SUCCESS != return_value)
    {
        MMINV_WRITE(MMIWIFI_WLAN_AP_RESELECT, &index);
    }
    return index;
}

/*****************************************************************************/
//  Description : MMIWIFI_SetIsShowNoWlanPrompt
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWIFI_SetIsShowNoWlanPrompt(BOOLEAN flag)
{
    BOOLEAN is_prompt = TRUE;

    is_prompt = flag;

    MMINV_WRITE(MMIWIFI_WLAN_NOWLAN_PROMPT, &is_prompt);     
}

/*****************************************************************************/
//  Description : MMIWIFI_GetIsShowNoWlanPrompt
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIWIFI_GetIsShowNoWlanPrompt(void)
{
    MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;
    BOOLEAN is_prompt = TRUE;
    
    MMINV_READ(MMIWIFI_WLAN_NOWLAN_PROMPT, &is_prompt, return_value);
    
    if (MN_RETURN_SUCCESS != return_value)
    {
        MMINV_WRITE(MMIWIFI_WLAN_NOWLAN_PROMPT, &is_prompt);
    }

    return is_prompt;
}

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWIFI_SetServiceProvider(char *name)
{
    char *temp_name = PNULL;
    uint16 len = 0;

    temp_name = SCI_ALLOCA(MMIWIFI_PORTAL_SERVICE_PROVIDER_NUM_MAX + 1);

    if (PNULL == temp_name)
    {
        //SCI_TRACE_LOW:"MMIWIFI_SetServiceProvider No mem"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_NV_772_112_2_18_3_9_36_92,(uint8*)"");
        return;
    }

    SCI_MEMSET(temp_name, 0, MMIWIFI_PORTAL_SERVICE_PROVIDER_NUM_MAX + 1);

    if (PNULL != name)
    {
        len = strlen(name);
        len =  MIN(len, MMIWIFI_PORTAL_SERVICE_PROVIDER_NUM_MAX);
        SCI_MEMCPY(temp_name, name, len);
    }

    MMINV_WRITE(MMIWIFI_PORTAL_SERVICE_PROVIDER, temp_name);
    SCI_FREE(temp_name);
}

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWIFI_GetServiceProvider(char *name, int32 max_len)
{
    MN_RETURN_RESULT_E  return_value = MN_RETURN_FAILURE;
    uint16 len = 0;

    if (PNULL != name)
    {
        MMINV_READ(MMIWIFI_PORTAL_SERVICE_PROVIDER, name, return_value);
        if (MN_RETURN_SUCCESS != return_value)
        {
            len = strlen(MMIWIFI_PORTAL_SERVICE_PROVIDER_CHINA_MOBILE);
            len =  MIN(len, max_len);
            SCI_MEMCPY(name, MMIWIFI_PORTAL_SERVICE_PROVIDER_CHINA_MOBILE, len);
            MMIWIFI_SetServiceProvider(MMIWIFI_PORTAL_SERVICE_PROVIDER_CHINA_MOBILE);
        }
    }   
}

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWIFI_SetActiveCMD(char *cmd)
{
    char *temp_cmd = PNULL;
    uint16 len = 0;

    temp_cmd = SCI_ALLOCA(MMIWIFI_PORTAL_COMMAND_LEN_MAX + 1);

    if (PNULL == temp_cmd)
    {
        //SCI_TRACE_LOW:"MMIWIFI_SetServiceProvider No mem"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_NV_824_112_2_18_3_9_37_93,(uint8*)"");
        return;
    }

    SCI_MEMSET(temp_cmd, 0, MMIWIFI_PORTAL_COMMAND_LEN_MAX + 1);

    if (PNULL != cmd)
    {
        len = strlen(cmd);
        len =  MIN(len, MMIWIFI_PORTAL_COMMAND_LEN_MAX);
        SCI_MEMCPY(temp_cmd, cmd, len);
    }

    MMINV_WRITE(MMIWIFI_PORTAL_COMMAND_ACTIVE, temp_cmd);
    SCI_FREE(temp_cmd);
}

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWIFI_GetActiveCMD(char *cmd, int32 max_len)
{
    MN_RETURN_RESULT_E  return_value = MN_RETURN_FAILURE;
    uint16 len = 0;

    if (PNULL != cmd)
    {
        MMINV_READ(MMIWIFI_PORTAL_COMMAND_ACTIVE, cmd, return_value);
        if (MN_RETURN_SUCCESS != return_value)
        {
            len = strlen(MMIWIFI_PORTAL_COMMAND_ACTIVE_STRING);
            len =  MIN(len, max_len);
            SCI_MEMCPY(cmd, MMIWIFI_PORTAL_COMMAND_ACTIVE_STRING, len);
            MMIWIFI_SetActiveCMD(MMIWIFI_PORTAL_COMMAND_ACTIVE_STRING);
        }
    }   
}

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWIFI_SetResetPasswordCMD(char *cmd)
{
    char *temp_cmd = PNULL;
    uint16 len = 0;

    temp_cmd = SCI_ALLOCA(MMIWIFI_PORTAL_COMMAND_LEN_MAX + 1);

    if (PNULL == temp_cmd)
    {
        //SCI_TRACE_LOW:"MMIWIFI_SetServiceProvider No mem"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_NV_876_112_2_18_3_9_37_94,(uint8*)"");
        return;
    }

    SCI_MEMSET(temp_cmd, 0, MMIWIFI_PORTAL_COMMAND_LEN_MAX + 1);

    if (PNULL != cmd)
    {
        len = strlen(cmd);
        len =  MIN(len, MMIWIFI_PORTAL_COMMAND_LEN_MAX);
        SCI_MEMCPY(temp_cmd, cmd, len);
    }

    MMINV_WRITE(MMIWIFI_PORTAL_COMMAND_RESET_PASSWORD, temp_cmd);
    SCI_FREE(temp_cmd);
}

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWIFI_GetResetPasswordCMD(char *cmd, int32 max_len)
{
    MN_RETURN_RESULT_E  return_value = MN_RETURN_FAILURE;
    uint16 len = 0;

    if (PNULL != cmd)
    {
        MMINV_READ(MMIWIFI_PORTAL_COMMAND_RESET_PASSWORD, cmd, return_value);
        if (MN_RETURN_SUCCESS != return_value)
        {
            len = strlen(MMIWIFI_PORTAL_COMMAND_RESET_PASSWORD_STRING);
            len =  MIN(len, max_len);
            SCI_MEMCPY(cmd, MMIWIFI_PORTAL_COMMAND_RESET_PASSWORD_STRING, len);
            MMIWIFI_SetResetPasswordCMD(MMIWIFI_PORTAL_COMMAND_RESET_PASSWORD_STRING);
        }
    }   
}

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWIFI_SetModifyPasswordCMD(char *cmd)
{
    char *temp_cmd = PNULL;
    uint16 len = 0;

    temp_cmd = SCI_ALLOCA(MMIWIFI_PORTAL_COMMAND_LEN_MAX + 1);

    if (PNULL == temp_cmd)
    {
        //SCI_TRACE_LOW:"MMIWIFI_SetServiceProvider No mem"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_NV_928_112_2_18_3_9_37_95,(uint8*)"");
        return;
    }

    SCI_MEMSET(temp_cmd, 0, MMIWIFI_PORTAL_COMMAND_LEN_MAX + 1);

    if (PNULL != cmd)
    {
        len = strlen(cmd);
        len =  MIN(len, MMIWIFI_PORTAL_COMMAND_LEN_MAX);
        SCI_MEMCPY(temp_cmd, cmd, len);
    }

    MMINV_WRITE(MMIWIFI_PORTAL_COMMAND_MODIFY_PASSWORD, temp_cmd);
    SCI_FREE(temp_cmd);
}

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWIFI_GetModifyPasswordCMD(char *cmd, int32 max_len)
{
    MN_RETURN_RESULT_E  return_value = MN_RETURN_FAILURE;
    uint16 len = 0;

    if (PNULL != cmd)
    {
        MMINV_READ(MMIWIFI_PORTAL_COMMAND_MODIFY_PASSWORD, cmd, return_value);
        if (MN_RETURN_SUCCESS != return_value)
        {
            len = strlen(MMIWIFI_PORTAL_COMMAND_MODIFY_PASSWORD_STRING);
            len =  MIN(len, max_len);
            SCI_MEMCPY(cmd, MMIWIFI_PORTAL_COMMAND_MODIFY_PASSWORD_STRING, len);
            MMIWIFI_SetModifyPasswordCMD(MMIWIFI_PORTAL_COMMAND_MODIFY_PASSWORD_STRING);
        }
    }   
}

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWIFI_SetMyFormTag(char *tag)
{
    char *temp_tag = PNULL;
    uint16 len = 0;

    temp_tag = SCI_ALLOCA(MMIWIFI_PORTAL_LOGIN_FORM_LEN_MAX + 1);

    if (PNULL == temp_tag)
    {
        //SCI_TRACE_LOW:"MMIWIFI_SetServiceProvider No mem"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_NV_980_112_2_18_3_9_37_96,(uint8*)"");
        return;
    }

    SCI_MEMSET(temp_tag, 0, MMIWIFI_PORTAL_LOGIN_FORM_LEN_MAX + 1);

    if (PNULL != tag)
    {
        len = strlen(tag);
        len =  MIN(len, MMIWIFI_PORTAL_LOGIN_FORM_LEN_MAX);
        SCI_MEMCPY(temp_tag, tag, len);
    }

    MMINV_WRITE(MMIWIFI_PORTAL_MY_FORM, temp_tag);
    SCI_FREE(temp_tag);
}

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWIFI_GetMyFormTag(char *tag, int32 max_len)
{
    MN_RETURN_RESULT_E  return_value = MN_RETURN_FAILURE;
    uint16 len = 0;

    if (PNULL != tag)
    {
        MMINV_READ(MMIWIFI_PORTAL_MY_FORM, tag, return_value);
        if (MN_RETURN_SUCCESS != return_value)
        {
            len = strlen(MMIWIFI_PORTAL_MYFORM);
            len =  MIN(len, max_len);
            SCI_MEMCPY(tag, MMIWIFI_PORTAL_MYFORM, len);
            MMIWIFI_SetMyFormTag(MMIWIFI_PORTAL_MYFORM);
        }
    }   
}

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWIFI_SetLoginFormTag(char *tag)
{
    char *temp_tag = PNULL;
    uint16 len = 0;

    temp_tag = SCI_ALLOCA(MMIWIFI_PORTAL_LOGIN_FORM_LEN_MAX + 1);

    if (PNULL == temp_tag)
    {
        //SCI_TRACE_LOW:"MMIWIFI_SetServiceProvider No mem"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_NV_1032_112_2_18_3_9_37_97,(uint8*)"");
        return;
    }

    SCI_MEMSET(temp_tag, 0, MMIWIFI_PORTAL_LOGIN_FORM_LEN_MAX + 1);

    if (PNULL != tag)
    {
        len = strlen(tag);
        len =  MIN(len, MMIWIFI_PORTAL_LOGIN_FORM_LEN_MAX);
        SCI_MEMCPY(temp_tag, tag, len);
    }

    MMINV_WRITE(MMIWIFI_PORTAL_LOGIN_FORM, temp_tag);
    SCI_FREE(temp_tag);
}

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWIFI_GetLoginFormTag(char *tag, int32 max_len)
{
    MN_RETURN_RESULT_E  return_value = MN_RETURN_FAILURE;
    uint16 len = 0;

    if (PNULL != tag)
    {
        MMINV_READ(MMIWIFI_PORTAL_LOGIN_FORM, tag, return_value);
        if (MN_RETURN_SUCCESS != return_value)
        {
            len = strlen(MMIWIFI_PORTAL_LOGINFORM);
            len =  MIN(len, max_len);
            SCI_MEMCPY(tag, MMIWIFI_PORTAL_LOGINFORM, len);
            MMIWIFI_SetLoginFormTag(MMIWIFI_PORTAL_LOGINFORM);
        }
    }   
}

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWIFI_SetPortalVersion(char *ver)
{
    char *temp_ver = PNULL;
    uint16 len = 0;

    temp_ver = SCI_ALLOCA(MMIWIFI_PORTAL_VERSION_LEN_MAX + 1);

    if (PNULL == temp_ver)
    {
        //SCI_TRACE_LOW:"MMIWIFI_SetServiceProvider No mem"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWLAN_NV_1084_112_2_18_3_9_37_98,(uint8*)"");
        return;
    }

    SCI_MEMSET(temp_ver, 0, MMIWIFI_PORTAL_VERSION_LEN_MAX + 1);

    if (PNULL != ver)
    {
        len = strlen(ver);
        len =  MIN(len, MMIWIFI_PORTAL_VERSION_LEN_MAX);
        SCI_MEMCPY(temp_ver, ver, len);
    }

    MMINV_WRITE(MMIWIFI_PORTAL_VERSION, temp_ver);
    SCI_FREE(temp_ver);
}

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWIFI_GetPortalVersion(char *ver, int32 max_len)
{
    MN_RETURN_RESULT_E  return_value = MN_RETURN_FAILURE;
    uint16 len = 0;

    if (PNULL != ver)
    {
        MMINV_READ(MMIWIFI_PORTAL_VERSION, ver, return_value);
        if (MN_RETURN_SUCCESS != return_value)
        {
            len = strlen(MMIWIFI_PORTAL_VERSION_INFO);
            len =  MIN(len, max_len);
            SCI_MEMCPY(ver, MMIWIFI_PORTAL_VERSION_INFO, len);
            MMIWIFI_SetPortalVersion(MMIWIFI_PORTAL_VERSION_INFO);
        }
    }   
}

/*****************************************************************************/
//  Description : MMIAPIWIFI_GetIsMonitorWakeupPacket
//  Global resource dependence :
//  Author: bin.wang1
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIWIFI_GetIsMonitorWakeupPacket( void )
{
    BOOLEAN             is_monitor = TRUE;
    MN_RETURN_RESULT_E  return_value = MN_RETURN_FAILURE;

    MMINV_READ( MMIWIFI_MONITOR_WAKEUP_PACKET, &is_monitor, return_value );

    if( MN_RETURN_SUCCESS != return_value )
    {
        is_monitor = TRUE;
        MMIAPIWIFI_SetIsMonitorWakeupPacket( is_monitor );
    }

    TRACE_APP_WLAN("is_monitor %d", is_monitor);

    return is_monitor;
}

/*****************************************************************************/
//  Description : MMIAPIWIFI_SetIsMonitorWakeupPacket
//  Global resource dependence :
//  Author: bin.wang1
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIWIFI_SetIsMonitorWakeupPacket( BOOLEAN is_monitor )
{
    TRACE_APP_WLAN("is_monitor %d", is_monitor);

    MMINV_WRITE( MMIWIFI_MONITOR_WAKEUP_PACKET, &is_monitor );
}

/*****************************************************************************/
//  Description : MMIAPIWIFI_GetWakeupPacketThreshold
//  Global resource dependence :
//  Author: bin.wang1
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIAPIWIFI_GetWakeupPacketThreshold( void )
{
    uint32              threshold = MMIWIFI_CACL_PACKET_MAX_NUM;
    MN_RETURN_RESULT_E  return_value = MN_RETURN_FAILURE;

    MMINV_READ( MMIWIFI_WAKEUP_PACKET_THRESHOLD, &threshold, return_value );

    if( MN_RETURN_SUCCESS != return_value )
    {
        threshold = MMIWIFI_CACL_PACKET_MAX_NUM;
        MMIAPIWIFI_SetWakeupPacketThreshold( threshold );
    }

    TRACE_APP_WLAN("threshold %d", threshold);

    return threshold;
}

/*****************************************************************************/
//  Description : MMIAPIWIFI_SetWakeupPacketThreshold
//  Global resource dependence :
//  Author: bin.wang1
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIWIFI_SetWakeupPacketThreshold( uint32 threshold )
{
    TRACE_APP_WLAN("threshold %d", threshold);

    MMINV_WRITE( MMIWIFI_WAKEUP_PACKET_THRESHOLD, &threshold );
}
