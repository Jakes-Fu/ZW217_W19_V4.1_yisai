/*************************************************************************
 ** File Name:      mmiwlan_nv.h                                         *
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

#ifndef MMIWLAN_NV_H_
#define MMIWLAN_NV_H_

#include "sci_types.h"
#include "mmi_module.h"
#include "mmiwlan_internal.h"

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/
typedef enum
{
    MMIWIFI_SETTING_INFO = MMI_MODULE_WIFI << 16,
    MMIWIFI_DEF_AP_SERVICE_ON,  
    MMIWIFI_DEF_AP_SERVICE_SET, 
    MMIWIFI_DEF_AP_ACCOUNT_INFO,
    MMIWIFI_DEF_AP_AUTO_CONNECT,
    MMIWIFI_WLAN_STATUS,
    MMIWIFI_WLAN_AUTO_CONNECT,
    MMIWIFI_WLAN_AUTO_SCAN_STATUS,
    MMIWIFI_IP_TYPE,
    MMIWIFI_IP_DATA,
    MMIWIFI_GATEWAY,
    MMIWIFI_SUBMASK,
    MMIWIFI_DNS1,
    MMIWIFI_DNS2,
    MMIWIFI_PROXY_QUERY,
    MMIWIFI_PROXY_IP,
    MMIWIFI_PROXY_PROT,
    MMIWIFI_PROXY_NAME,
    MMIWIFI_PROXY_PASSWORD,
    MMIWIFI_PROXY_REJECTLIST,
    MMIWIFI_PORTAL_CONFIG,
    MMIWIFI_WLAN_RESELECT,
    MMIWIFI_WLAN_AP_RESELECT,
    MMIWIFI_WLAN_NOWLAN_PROMPT,
    MMIWIFI_PORTAL_VERSION,
    MMIWIFI_PORTAL_SERVICE_PROVIDER,
    MMIWIFI_PORTAL_COMMAND_ACTIVE,
    MMIWIFI_PORTAL_COMMAND_RESET_PASSWORD,
    MMIWIFI_PORTAL_COMMAND_MODIFY_PASSWORD,
    MMIWIFI_PORTAL_MY_FORM,
    MMIWIFI_PORTAL_LOGIN_FORM,
    MMIWIFI_MONITOR_WAKEUP_PACKET,
    MMIWIFI_WAKEUP_PACKET_THRESHOLD,

    MMIWIFI_MAX_NV_ITEM_NUM
}MMIWIFI_NV_ITEM_E;


/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
// 	Description : register wifi module nv len and max item
//	Global resource dependence : none
//  Author: li.li
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIWIFI_RegWifiNv(void);

/*****************************************************************************/
//  Description : get is active service
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIWIFI_GetIsServiceOn(void);

/*****************************************************************************/
//  Description : Set is active service
//  Global resource dependence : 
//  Author: jiaoyou.wu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWIFI_SetIsServiceOn(BOOLEAN is_active);

/*****************************************************************************/
//  Description : get is active service
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIWIFI_GetIsAutoConnectingOn(void);

/*****************************************************************************/
//  Description : Set is active service
//  Global resource dependence : 
//  Author: jiaoyou.wu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWIFI_SetIsAutoConnectingOn(BOOLEAN is_auto_connecting);

/*****************************************************************************/
//  Description : get account info
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWIFI_GetAccountInfo(MMIWIFI_ACCOUNT_INFO_T *account_info);

/*****************************************************************************/
//  Description : Set account info
//  Global resource dependence : 
//  Author: jiaoyou.wu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWIFI_SetAccountInfo(MMIWIFI_ACCOUNT_INFO_T *account_info);

/*****************************************************************************/
//  Description : MMIWIFI_GetIsWlanOn
//  Global resource dependence : 
//  Author:
//  Note: 0为关闭，1为打开
/*****************************************************************************/
PUBLIC BOOLEAN MMIWIFI_GetIsWlanOn(void);

/*****************************************************************************/
//  Description : MMIWIFI_SetIsWlanOn
//  Global resource dependence : 
//  Author: 
//  Note: 0为关闭，1为打开
/*****************************************************************************/
PUBLIC void MMIWIFI_SetIsWlanOn(BOOLEAN is_in);

/*****************************************************************************/
//  Description : MMIWIFI_GetIsAutoConnect
//  Global resource dependence : 
//  Author:
//  Note: 0为关闭，1为打开
/*****************************************************************************/
PUBLIC BOOLEAN MMIWIFI_GetIsAutoConnect(void);

/*****************************************************************************/
//  Description : MMIWIFI_SetIsAutoConnect
//  Global resource dependence : 
//  Author: 
//  Note: 0为关闭，1为打开
/*****************************************************************************/
PUBLIC void MMIWIFI_SetIsAutoConnect(BOOLEAN is_on);

/*****************************************************************************/
//  Description : MMIWIFI_GetWlanSearchPeriod
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC uint16 MMIWIFI_GetWlanAutoScanStatus(void);
/*****************************************************************************/
//  Description : MMIWIFI_SetWlanSearchPeriod
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWIFI_SetWlanAutoScanStatus(uint16 index);
/*****************************************************************************/
//  Description : MMIWIFI_GetWlanIPType
//  Global resource dependence : 
//  Author:
//  Note: 0为动态IP，1为静态IP
/*****************************************************************************/
PUBLIC BOOLEAN MMIWIFI_GetIPType(void);
/*****************************************************************************/
//  Description : MMIWIFI_SetWlanIPType
//  Global resource dependence : 
//  Author: 
//  Note: 0为动态IP，1为静态IP
/*****************************************************************************/
PUBLIC void MMIWIFI_SetIPType(BOOLEAN index);

/*****************************************************************************/
//  Description : MMIWIFI_GetWlanIPType
//  Global resource dependence : 
//  Author:
//  Note: 0为不使用代理，1为使用代理
/*****************************************************************************/
PUBLIC BOOLEAN MMIWIFI_GetProxyQuery(void);
/*****************************************************************************/
//  Description : MMIWIFI_SetWlanIPType
//  Global resource dependence : 
//  Author: 
//  Note: 0为不使用代理，1为使用代理
/*****************************************************************************/
PUBLIC void MMIWIFI_SetProxyQuery(BOOLEAN index);

/*****************************************************************************/
//  Description : MMIWIFI_GetIPData
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMIWIFI_GetIPData(void);
/*****************************************************************************/
//  Description : MMIWIFI_SetIPData
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWIFI_SetIPData(uint32 index);

/*****************************************************************************/
//  Description : MMIWIFI_GetGateWay
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMIWIFI_GetGateWay(void);
/*****************************************************************************/
//  Description : MMIWIFI_SetGateWay
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWIFI_SetGateWay(uint32 index);

/*****************************************************************************/
//  Description : MMIWIFI_GetSubMask
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMIWIFI_GetSubMask(void);
/*****************************************************************************/
//  Description : MMIWIFI_SetSubMask
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWIFI_SetSubMask(uint32 index);

/*****************************************************************************/
//  Description : MMIWIFI_GetDNS1
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMIWIFI_GetDNS1(void);
/*****************************************************************************/
//  Description : MMIWIFI_SetDNS1
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWIFI_SetDNS1(uint32 index);

/*****************************************************************************/
//  Description : MMIWIFI_GetDNS2
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMIWIFI_GetDNS2(void);
/*****************************************************************************/
//  Description : MMIWIFI_SetDNS2
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWIFI_SetDNS2(uint32 index);

/*****************************************************************************/
//  Description : MMIWIFI_GetProxyURL
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWIFI_GetProxyURL(char *out_url, int16 max_len);

/*****************************************************************************/
//  Description : MMIWIFI_SetProxyURL
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWIFI_SetProxyURL(char *in_url);

/*****************************************************************************/
//  Description : MMIWIFI_GetProxyPort
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWIFI_GetProxyPort(uint16 * index);
/*****************************************************************************/
//  Description : MMIWIFI_SetProxyPort
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWIFI_SetProxyPort(uint16 * index);

/*****************************************************************************/
//  Description : MMIWIFI_GetProxyName
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWIFI_GetProxyName(MMIWIFI_PROXY_NAME_E * index);
/*****************************************************************************/
//  Description : MMIWIFI_SetProxyName
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWIFI_SetProxyName(MMIWIFI_PROXY_NAME_E * index);

/*****************************************************************************/
//  Description : MMIWIFI_GetProxyPassword
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWIFI_GetProxyPassword(MMIWIFI_PROXY_PASSWORD_E * index);
/*****************************************************************************/
//  Description : MMIWIFI_SetProxyPassword
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWIFI_SetProxyPassword(MMIWIFI_PROXY_PASSWORD_E * index);

/*****************************************************************************/
//  Description : MMIWIFI_SetProxyRejectList
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWIFI_SetProxyRejectList(MMIWIFI_PROXY_REJECTLIST_E * index);

/*****************************************************************************/
//  Description : MMIWIFI_GetProxyRejectList
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWIFI_GetProxyRejectList(MMIWIFI_PROXY_REJECTLIST_E * index);

/*****************************************************************************/
//  Description : MMIWIFI_SetGPRSToWlanResetct
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWIFI_SetGPRSToWlanReselect(MMI_GPRSTOWLAN_RESELECT * index);

/*****************************************************************************/
//  Description : MMIWIFI_GetGPRSToWlanResetct
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC MMI_GPRSTOWLAN_RESELECT MMIWIFI_GetGPRSToWlanReselect(void);


/*****************************************************************************/
//  Description : MMIWIFI_SetApReselect
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWIFI_SetApReselect( MMI_AP_RESELECT  *index);

/*****************************************************************************/
//  Description : MMIWIFI_GetApReselect
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC MMI_AP_RESELECT MMIWIFI_GetApReselect(void);

/*****************************************************************************/
//  Description : MMIWIFI_SetApReselect
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWIFI_SetIsShowNoWlanPrompt(BOOLEAN flag);

/*****************************************************************************/
//  Description : MMIWIFI_GetNoWlanPrompt
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN  MMIWIFI_GetIsShowNoWlanPrompt(void);

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWIFI_SetServiceProvider(char *name);

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWIFI_GetServiceProvider(char *name, int32 max_len);

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWIFI_SetActiveCMD(char *cmd);

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWIFI_GetActiveCMD(char *cmd, int32 max_len);

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWIFI_SetResetPasswordCMD(char *cmd);

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWIFI_GetResetPasswordCMD(char *cmd, int32 max_len);

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWIFI_SetModifyPasswordCMD(char *cmd);

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWIFI_GetModifyPasswordCMD(char *cmd, int32 max_len);

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWIFI_SetMyFormTag(char *tag);

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWIFI_GetMyFormTag(char *tag, int32 max_len);

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWIFI_SetLoginFormTag(char *tag);

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWIFI_GetLoginFormTag(char *tag, int32 max_len);

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWIFI_SetPortalVersion(char *ver);

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWIFI_GetPortalVersion(char *ver, int32 max_len);

/*****************************************************************************/
//  Description : MMIAPIWIFI_GetIsMonitorWakeupPacket
//  Global resource dependence :
//  Author: bin.wang1
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIWIFI_GetIsMonitorWakeupPacket( void );

/*****************************************************************************/
//  Description : MMIAPIWIFI_SetIsMonitorWakeupPacket
//  Global resource dependence :
//  Author: bin.wang1
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIWIFI_SetIsMonitorWakeupPacket( BOOLEAN is_monitor );

/*****************************************************************************/
//  Description : MMIAPIWIFI_GetWakeupPacketThreshold
//  Global resource dependence :
//  Author: bin.wang1
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIAPIWIFI_GetWakeupPacketThreshold( void );

/*****************************************************************************/
//  Description : MMIAPIWIFI_SetWakeupPacketThreshold
//  Global resource dependence :
//  Author: bin.wang1
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIWIFI_SetWakeupPacketThreshold( uint32 threshold );

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif	//#ifdef MMIWIFI_NV_H_

