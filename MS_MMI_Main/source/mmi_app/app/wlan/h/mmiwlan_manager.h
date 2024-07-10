/*****************************************************************************
** File Name:      mmiwlan_manager.h                                         *
** Author:         li.li                                                     *
** Date:           2009.11.13                                                *
** Copyright:      2003 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:                                                              *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2009.11.13     li.li            Create                                    *
******************************************************************************/

#ifndef _MMIWLAN_MANAGER_H_
#define _MMIWLAN_MANAGER_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmiwlan_profile.h"
#include "app_tcp_if.h"
#include "mmiwifi_export.h"
#include "mmiwlan_internal.h"
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif


/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         typedef                                          *
 **--------------------------------------------------------------------------*/

typedef enum MMIWIFI_RESULT
{
    MMIWIFI_RESULT_SUCC,
    MMIWIFI_RESULT_FAIL,
    MMIWIFI_RESULT_NOT_FOUNT,
    MMIWIFI_RESULT_TIMEOUT,
    MMIWIFI_RESULT_RETRICTED,
    MMIWIFI_RESULT_PWD_ERROR,
    MMIWIFI_RESULT_MAX
}MMIWIFI_RESULT_E;

typedef enum MMIWIFI_MSG
{
    MMIWIFI_MSG_ON_REQ,
    MMIWIFI_MSG_ON_CNF,
    MMIWIFI_MSG_OFF_REQ,
    MMIWIFI_MSG_OFF_CNF,
    MMIWIFI_MSG_SCAN_REQ,
    MMIWIFI_MSG_SCAN_CNF,
    MMIWIFI_MSG_SCAN_IND,

    MMIWIFI_MSG_SCAN_BEGIN_IND,
    
    MMIWIFI_MSG_CONNECT_REQ,
    MMIWIFI_MSG_CONNECT_CNF,
    MMIWIFI_MSG_DISCONNECT_REQ,
    MMIWIFI_MSG_DISCONNECT_CNF,
    MMIWIFI_MSG_DISCONNECT_IND,
    MMIWIFI_MSG_DHCP_REQ,
    MMIWIFI_MSG_DHCP_CNF,
    MMIWIFI_MSG_BSSID_CHANGED_IND,
    MMIWIFI_MSG_RSSI_CHANGED_IND,
    MMIWIFI_MSG_MAX
}MMIWIFI_MSG_E;

typedef enum MMIWIFI_AUTOSCAN_STATUS
{
    MMIWIFI_ATUOSCAN_OFF = (0),

#ifdef MMI_WLAN_TAB_STYLE
    MMIWIIF_AUTOSCAN_PERIOD_TEN_SECONDS = (10),
#else
    MMIWIIF_AUTOSCAN_PERIOD_TEN_SECONDS = (30),
#endif /* MMI_WLAN_TAB_STYLE */
    MMIWIFI_AUTOSCAN_PERIOD_ONE_MINUTE = (60),
    MMIWIFI_AUTOSCAN_PERIOD_TWO_MINUTES = (2*60),
    MMIWIFI_AUTOSCAN_PERIOD_FIVE_MINUTES = (5*60),
    MMIWIFI_AUTOSCAN_PERIOD_TEN_MINUTES = (10*60)
}MMIWIFI_AUTOSCAN_STATUS_E;

typedef BOOLEAN (*MMIWIFI_PROC_FUNC)(MMIWIFI_MSG_E message_id,void* param,uint16 size_of_param);

typedef struct MMIWIFI_CONNECT_CNF
{
    MMIWIFI_INDEX_T     index;
    MMIWIFI_RESULT_E    result;
}MMIWIFI_CONNECT_CNF_T;

typedef struct MMIWIFI_ON_CNF
{
    MMIWIFI_RESULT_E    result;
}MMIWIFI_ON_CNF_T;

typedef struct MMIWIFI_OFF_CNF
{
    MMIWIFI_RESULT_E    result;
}MMIWIFI_OFF_CNF_T;

typedef struct MMIWIFI_SCAN_CNF
{
    MMIWIFI_RESULT_E    result;
}MMIWIFI_SCAN_CNF_T;

typedef struct MMIWIFI_RSSI_CHANGED_IND
{
    MMIWIFI_INDEX_T index;
    int32           new_rssi;
}MMIWIFI_RSSI_CHANGED_IND_T;

typedef struct MMIWIFI_DISCONNECT_CNF
{
    MMIWIFI_INDEX_T     index;
    MMIWIFI_RESULT_E    result;
}MMIWIFI_DISCONNECT_CNF_T;

typedef struct MMIWIFI_DHCP_CNF
{
    MMIWIFI_RESULT_E      result;
    MMIWIFI_INDEX_T         index;
    TCPIP_NETIF_IPADDR_T  netif;
} MMIWIFI_DHCP_CNF_T;

/*---------------------------------------------------------------------------*/
/*                          Enum                                             */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                          Struct                                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          Message                                          */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                          EXTERNAL FUNCTION DECLARE                        */
/*---------------------------------------------------------------------------*/
/****************************************************************************/
//  Description : wifi ui reg msg handle function
//  Global resource dependence : 
//  Author:li.li 
//  Note: 
/****************************************************************************/
PUBLIC BOOLEAN MMIWIFI_RegHandleMsg(MMIWIFI_PROC_FUNC HandleMsgFunc);

/****************************************************************************/
//  Description : MMIWIFI_DeepSleepCMD
//  Global resource dependence : 
//  Author:
//  Note: 输入is_on = FALSE，表示WAKEUP，is_on = TRUE，表示deepsleep
/****************************************************************************/
PUBLIC BOOLEAN MMIWIFI_DeepSleepCMD(BOOLEAN  is_enter);

/****************************************************************************/
//  Description : wifi scan function
//  Global resource dependence : 
//  Author:li.li 
//  Note: ssid类型是gb2312 string ，当为PNULL时，执行搜索功能，否则为指定接入点搜索
/****************************************************************************/
PUBLIC BOOLEAN MMIWIFI_Scan(uint8 *ssid);

/****************************************************************************/
//  Description : wifi scan function
//  Global resource dependence : 
//  Author:Jiaoyou
//  Note: is_prompt:is showing waiting win
/****************************************************************************/
PUBLIC BOOLEAN MMIWIFI_ScanEx(uint8 *ssid, BOOLEAN is_prompt);

/****************************************************************************/
//  Description : wifi MMIWIFI_AutoScanByTimer function
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/****************************************************************************/
PUBLIC  BOOLEAN MMIWIFI_AutoScanByTimer(MMIWIFI_AUTOSCAN_STATUS_E autoscan_status);

/****************************************************************************/
//  Description : wifi connect function
//  Global resource dependence : 
//  Author:li.li 
//  Note: 
/****************************************************************************/
PUBLIC BOOLEAN MMIWIFI_Connect(MMIWIFI_PROFILE_T *profile_ptr);

/****************************************************************************/
//  Description : wifi disconnect function
//  Global resource dependence : 
//  Author:li.li 
//  Note: 
/****************************************************************************/
PUBLIC BOOLEAN MMIWIFI_DisConnect(MMIWIFI_PROFILE_T *profile_ptr);

/****************************************************************************/
//  Description : wifi DHCP function
//  Global resource dependence : 
//  Author:li.li 
//  Note: 
/****************************************************************************/
PUBLIC BOOLEAN MMIWIFI_Dhcp(MMIWIFI_PROFILE_T* profile_ptr);

/****************************************************************************/
//  Description : wifi get status function
//  Global resource dependence : 
//  Author:li.li 
//  Note: 
/****************************************************************************/
PUBLIC BOOLEAN MMIWIFI_GetHasConnectedAp(void);

/****************************************************************************/
//  Description : wifi set current Profile ptr
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
PUBLIC void MMIWIFI_SetCurrProfile(MMIWIFI_PROFILE_T* profile_ptr);

/****************************************************************************/
//  Description : wifi get current Profile ptr
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
PUBLIC MMIWIFI_PROFILE_T*  MMIWIFI_GetCurrProfile(void);

/****************************************************************************/
//  Description : wifi handle supplicant msg function
//  Global resource dependence : 
//  Author:li.li 
//  Note: 
/****************************************************************************/
PUBLIC BOOLEAN MMIWIFI_HandleSuppMsg(uint32 msg_id, void* param_ptr);

/****************************************************************************/
//  Description : wifi notify event to app
//  Global resource dependence : 
//  Author:li.li 
//  Note: 
/****************************************************************************/
PUBLIC void MMIWIFI_NotifyApp(MMIWIFI_NOTIFY_EVENT_E event_id, MMIWIFI_NOTIFY_PARAM_T* param_ptr);


/****************************************************************************/
//  Description : set wifi is prompt
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/****************************************************************************/
PUBLIC void MMIWIFI_SetPromptResult(BOOLEAN is_prompt_ok);


/****************************************************************************/
//  Description : get wifi is prompt
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/****************************************************************************/
PUBLIC BOOLEAN MMIWIFI_GetPromptResult(void);

/****************************************************************************/
//  Description : set wifi is initialized
//  Global resource dependence : 
//  Author:li.li 
//  Note: 
/****************************************************************************/
PUBLIC void MMIWIFI_SetIsInitialized(BOOLEAN is_initialized);

/****************************************************************************/
//  Description : get wifi is initialized
//  Global resource dependence : 
//  Author:li.li 
//  Note: 
/****************************************************************************/
PUBLIC BOOLEAN MMIWIFI_GetIsInitialized(void);

/****************************************************************************/
//  Description : Set wifi  auto connect status
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
PUBLIC void MMIWIFI_SetAutoConnectStatus(MMIWIFI_AUTO_CONNECT_STATUS_E auto_connect_status);

/****************************************************************************/
//  Description : Get wifi auto connect status
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
PUBLIC MMIWIFI_AUTO_CONNECT_STATUS_E MMIWIFI_GetAutoConnectStatus(void);


/****************************************************************************/
//  Description : Set wifi scan status
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/****************************************************************************/
PUBLIC void MMIWIFI_SetScanStatus(BOOLEAN is_scanning);

/****************************************************************************/
//  Description : Get wifi scan status
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/****************************************************************************/
PUBLIC BOOLEAN MMIWIFI_GetScanStatus(void);


/****************************************************************************/
//  Description : Set wifi spicified scan status
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
PUBLIC void MMIWIFI_SetSpecifiedScanStatus(BOOLEAN is_specified);

/****************************************************************************/
//  Description : Set wifi spicified scan status
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
PUBLIC BOOLEAN MMIWIFI_GetSpecifiedScanStatus(void);

/****************************************************************************/
//  Description : Check if the connect is started by auto
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
PUBLIC  BOOLEAN MMIWIFI_IsConnectByAutoConnecting(void);

/* BEGIN: Added by George.Liu 01526, 2010/7/20   PN:C59783 MS00189474*/
/****************************************************************************/
//  Description : 不做预处理，直接使用config结构进行连接
//  Global resource dependence : 
//  Author:George.Liu
//  Note: 
/****************************************************************************/
PUBLIC BOOLEAN MMIWIFI_DirectConnect(WIFISUPP_SSID_CONFIG_T *pstConfig_info);
/* END:   Added by George.Liu 01526, 2010/7/20   PN:C59783 MS00189474*/

/****************************************************************************/
//  Description : 终止DHCP过程
//  Global resource dependence : 
//  Author:George.Liu
//  Note: 
/****************************************************************************/
PUBLIC BOOLEAN MMIWIFI_DisDhcp(MMIWIFI_PROFILE_T* profile_ptr);

/****************************************************************************/
//  Description : 开启wifi
//  Global resource dependence : 
//  Author:George.Liu
//  Note: 
/****************************************************************************/
PUBLIC BOOLEAN MMIWIFI_On(void);

/****************************************************************************/
//  Description : Auto connect function
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note:
/****************************************************************************/
PUBLIC BOOLEAN MMIWIFI_AutoConnect(void);

/****************************************************************************/
//  Description : Start AutoScan
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note:
/****************************************************************************/
PUBLIC BOOLEAN MMIWIFI_StartAutoScan(void);

/****************************************************************************/
//  Description : Stop AutoScan
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note:
/****************************************************************************/
PUBLIC BOOLEAN MMIWIFI_StopAutoScan(void);

/****************************************************************************/
//  Description : wifi is need redraw wifi icon
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 信号差异很小的话，可能处于一个等级，不需要重画
/****************************************************************************/
LOCAL BOOLEAN MMIWIFI_IsNeedRedrawWifiIcon(int32 new_rssi, int32 old_rssi);


/****************************************************************************/
//  Description : wifi connect function
//  Global resource dependence : 
//  Author:li.li 
//  Note:  Modified by George.Liu 01526, 2010/9/2
/****************************************************************************/
PUBLIC BOOLEAN MMIAPIWIFI_Connect(void *profile_ptr);
/****************************************************************************/
//  Description : MMIWIFI_SetIsWlanOffReqHandled
//  Global resource dependence : 
//  Author:rong.gu 
//  Note:  
/****************************************************************************/
PUBLIC void MMIWIFI_SetIsWlanOffReqHandled(BOOLEAN is_handled);
/****************************************************************************/
//  Description : MMIWIFI_SetIsWlanOffReqHandled
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/****************************************************************************/
PUBLIC BOOLEAN MMIWIFI_GetIsWlanOffReqHandled(void);

#ifdef   __cplusplus
    }
#endif


#endif 
