/*****************************************************************************
** File Name:      mmiwifi_export.h                                             *
** Author:         li.li                                                     *
** Date:           2009/11/18                                                *
** Copyright:      2006 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe wifi api                    *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2009/11/18     li.li          Create                                      *
******************************************************************************/

#ifndef _MMIWIFI_API_H_
#define _MMIWIFI_API_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "os_api.h"
#include "mmk_type.h"
#include "wifisupp_api.h"
#include "app_tcp_if.h"
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
#ifdef WIN32
#define __func__ __FILE__
#endif

/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/
typedef uint32 MMIWIFI_INDEX_T;

typedef enum MMIWIFI_STATUS
{
    MMIWIFI_STATUS_OFF,
    MMIWIFI_STATUS_ON,
    MMIWIFI_STATUS_CONNECTED,
    MMIWIFI_STATUS_MAX
} MMIWIFI_STATUS_E;

typedef enum MMIWIFI_AUTO_CONNECT_STATUS_E_
{
    MMIWIFI_AUTO_CONNECT_STATUS_NONE,
    MMIWIFI_AUTO_CONNECT_STATUS_CONNECTING,
    MMIWIFI_AUTO_CONNECT_STATUS_CONNECTED,

    MMIWIFI_AUTO_CONNECT_STATUS_MAX
} MMIWIFI_AUTO_CONNECT_STATUS_E;

typedef enum MMIWIFI_NOTIFY_EVENT
{
    MMIWIFI_NOTIFY_EVENT_ON,
    MMIWIFI_NOTIFY_EVENT_OFF,
    MMIWIFI_NOTIFY_EVENT_CONNECT,
    MMIWIFI_NOTIFY_EVENT_IPV6_CONNECT, //Bug 1334078
    MMIWIFI_NOTIFY_EVENT_DISCONNECT,
    MMIWIFI_NOTIFY_EVENT_FIND_AP,
    MMIWIFI_NOTIFY_EVENT_SCAN_CNF,
    MMIWIFI_NOTIFY_EVENT_MAX
}MMIWIFI_NOTIFY_EVENT_E;

typedef struct MMIWIFI_NOTIFY_PARAM
{
    WIFISUPP_RESULT_E    result;
} MMIWIFI_NOTIFY_PARAM_T;

typedef void (*MMIWIFI_NOTIFY_FUNC)(MMIWIFI_NOTIFY_EVENT_E event_id,MMIWIFI_NOTIFY_PARAM_T* param_ptr);

typedef struct MMIWIFI_NOTIFY_INFO
{
    uint32              module_id;
    MMIWIFI_NOTIFY_FUNC notify_func;
} MMIWIFI_NOTIFY_INFO_T;

typedef enum MMIWIFI_ENTRY_TYPE_E_
{
    MMIWIFI_ENTRY_TYPE_LIST,
    MMIWIFI_ENTRY_TYPE_TRUSTED_LIST,
    MMIWIFI_ENTRY_TYPE_SETTINGS,

    MMIWIFI_ENTRY_TYPE_MAX
} MMIWIFI_ENTRY_TYPE_E;

typedef struct
{
    BOOLEAN                         is_use_proxy;                                                       //WLAN是否使用代理
    uint8					gateway[PROXY_PORT_MAX_LEN + 1];		//代理IP地址
    uint8					gateway_len;								//IP地址
    uint16					port;										//端口号
    uint8					username[PROXY_NAME_LEN + 1];	//用户名
    uint8					username_len;								//用户名长度
    uint8					password[PROXY_PASSWORD_LEN + 1];	//密码
    uint8					password_len;								//密码长度
    MMIWIFI_PROXY_REJECTLIST_E					rejectlist;//排除列表
}MMIWLAN_PROXY_SETTING_DETAIL_T;//WALN 代理设置的结构

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/****************************************************************************/
//  Description : open wifi main window
//  Global resource dependence : 
//  Author:li.li 
//  Note: 
/****************************************************************************/
PUBLIC void MMIAPIWIFI_OpenMainWin(void);

/****************************************************************************/
//  Description : open wifi main window
//  Global resource dependence :
//  Author:bin.wang1
//  Note: 
/****************************************************************************/
PUBLIC void WatchWIFI_MainWin_Enter( void );

/****************************************************************************/
//  Description : open wifi list window
//  Global resource dependence : 
//  Author:li.li 
//  Note: 
/****************************************************************************/
PUBLIC void MMIAPIWIFI_OpenListWin(void);

/*****************************************************************************/
//  Description : init wifi module
//  Global resource dependence : none
//  Author: li.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIWIFI_InitModule(void);

/****************************************************************************/
//  Description : wifi get status function
//  Global resource dependence : 
//  Author:li.li 
//  Note: 
/****************************************************************************/
PUBLIC MMIWIFI_STATUS_E MMIAPIWIFI_GetStatus(void);

/****************************************************************************/
//  Description : wifi on function
//  Global resource dependence : 
//  Author:li.li 
//  Note: 
/****************************************************************************/
PUBLIC BOOLEAN MMIAPIWIFI_On(BOOLEAN is_need_wait_win);

/****************************************************************************/
//  Description : wifi on function
//  Global resource dependence : 
//  Author:li.li 
//  Note: 
/****************************************************************************/
PUBLIC BOOLEAN MMIAPIWIFI_GetIsWlanOn(void);

/****************************************************************************/
//  Description : delete the wifi profile by index
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
PUBLIC BOOLEAN MMIAPIWIFI_APIsAvailable(void);

/****************************************************************************/
//  Description : wifi off function
//  Global resource dependence : 
//  Author:li.li 
//  Note: 
/****************************************************************************/
PUBLIC BOOLEAN MMIAPIWIFI_Off(void);

/*****************************************************************************/
//  Description : init wifi 
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIWIFI_Init(void);

/****************************************************************************/
//  Description : APP reg notify function
//  Global resource dependence : 
//  Author:li.li 
//  Note: 
/****************************************************************************/
PUBLIC void MMIAPIWIFI_RegNotifyInfo(MMIWIFI_NOTIFY_INFO_T* notify_info_ptr);

/****************************************************************************/
//  Description : APP dereg notify function
//  Global resource dependence : 
//  Author:li.li 
//  Note: 
/****************************************************************************/
PUBLIC void MMIAPIWIFI_DeRegNotifyInfo(uint32 module_id);

/****************************************************************************/
//  Description : wifi get net id function
//  Global resource dependence : 
//  Author:li.li 
//  Note: 
/****************************************************************************/
PUBLIC TCPIP_NETID_T MMIAPIWIFI_GetNetId(void);

/*****************************************************************************/
//  Description : set if wifi is on when power off 
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIWIFI_SetInfo(BOOLEAN result);

/*****************************************************************************/
//  Description : 将Wi-Fi模块复位                                            
//  Global resource dependence :                                             
//  Author: George.Liu 01526                                                 
//  Note:                                                                    
/*****************************************************************************/
PUBLIC void MMIAPIWIFI_Reset(void);

/****************************************************************************/
//  Description : 添加在灭屏下使用WIFI的用户
//  Global resource dependence :
//  Author: bin.wang1
//  Note:
/****************************************************************************/
PUBLIC void MMIAPIWIFI_AddDeepSleepWifiUser( char* module_name_ptr );

/****************************************************************************/
//  Description : 移除在灭屏下使用WIFI的用户
//  Global resource dependence :
//  Author: bin.wang1
//  Note:
/****************************************************************************/
PUBLIC void MMIAPIWIFI_RemoveDeepSleepWifiUser( char* module_name_ptr );

/*****************************************************************************/
//  Description : set ui option max
//  Global resource dependence : 
//  Author: bin.ji
//  Note: fix MS00206348
/*****************************************************************************/
PUBLIC void MMIAPIWIFI_SetUiOptionMAX(void);

/****************************************************************************/
//  Description : MMIAPIWIFI_GetProxySettingDetail
//  Global resource dependence : 
//  Author:
//  Note:该接口用于浏览器调用
/****************************************************************************/
PUBLIC void MMIAPIWIFI_GetProxySettingDetail(MMIWLAN_PROXY_SETTING_DETAIL_T *detail_struct);

/*****************************************************************************/
//  Description : MMIAPIWIFI_IsExistInExcludingList
//  Global resource dependence : 
//  Author: bin.ji
//  Note: fix MS00206348
/*****************************************************************************/
PUBLIC BOOLEAN  MMIAPIWIFI_IsExistInExcludingList(char *str_ptr);

/*****************************************************************************/
//  Description : append item to listbox 
//  Global resource dependence : none
//  Author: fen.xie
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIWIFI_AppendSelectListItem(MMI_CTRL_ID_T ctrl_id);

/****************************************************************************/
//  Description : MMIAPIWIFI_HandleDeepSleep
//  Global resource dependence : 
//  Author:
//  Note:
/****************************************************************************/
PUBLIC void MMIAPIWIFI_HandleDeepSleep(BOOLEAN  is_deepsleep);


#ifdef WLAN_RESELECT_SUPPORT    
/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIWIFI_SetIsShowNoWlanPrompt(BOOLEAN flag);

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIWIFI_GetIsShowNoWlanPrompt(void);
#endif

/*****************************************************************************/
//  Description :Get Wlan Status icon
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC MMI_IMAGE_ID_T MMIAPIWIFI_GetWifiStatusIcon(void);

/****************************************************************************/
//  Description : get the wifi profile num
//  Global resource dependence :
//  Author:bin.wang1
//  Note:
/****************************************************************************/
PUBLIC uint32 MMIAPIWIFI_GetSearchedAPNum( void );

/****************************************************************************/
//  Description : get the profile ssid by index
//  Global resource dependence :
//  IN/OUT: ssid_t_ptr searched ssid information
//  Author:bin.wang1
//  Note: length is 0 mean hide ssid
/****************************************************************************/
PUBLIC BOOLEAN MMIAPIWIFI_GetSearchedSsidByIndex( MMIWIFI_INDEX_T index, WIFISUPP_SSID_T* ssid_t_ptr );

/**---------------------------------------------------------------------------*
 **                         Macro Declaration                                 *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  如下宏定义用以实现对外接口更名前后的兼容
/*****************************************************************************/
#ifdef MMI_FUNC_COMPATIBLE_SUPPORT

#define 	MMIWIFI_APIsAvailable	   		MMIAPIWIFI_APIsAvailable

#define 	MMIWIFI_GetProxySettingDetail   			MMIAPIWIFI_GetProxySettingDetail

#endif //MMI_FUNC_COMPATIBLE_SUPPORT

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif 


