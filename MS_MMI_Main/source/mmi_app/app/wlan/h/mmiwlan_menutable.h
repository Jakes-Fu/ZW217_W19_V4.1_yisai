/*****************************************************************************
** File Name:      mmiwlan_menutable.h                                       *
** Author:         li.li													 *
** Date:           2009/11/18                                                *
** Copyright:      2007 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe wifi menu id                *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2007/1/31      ryan.xu	       Create                                    *
******************************************************************************/

#ifndef MMIWLAN_MENUTABLE_H_
#define MMIWLAN_MENUTABLE_H_

#include "mmi_module.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/*-------------------------------------------------------------------------*/
/*                         TYPES/CONSTANTS                                 */
/*-------------------------------------------------------------------------*/
#define TIP_NULL 0
typedef enum
{
    MMIWIFI_MENU_ID_START = (MMI_MODULE_WIFI << 16),
    MMIWIFI_MENU_ID_DETAIL,
    MMIWIFI_MENU_ID_DISCONNECT,
    MMIWIFI_MENU_ID_EDIT,
    MMIWIFI_MENU_ID_SCAN,
    MMIWIFI_MENU_ID_NEW,
    MMIWIFI_MENU_ID_DELETE,
    MMIWIFI_MENU_ID_CLEAR,
    MMIWIFI_MENU_ID_HELP,
    MMIWIFI_MENU_ID_ON,
    MMIWIFI_MENU_ID_OFF,
    MMIWIFI_MENU_LIST_ON_ID,
    MMIWIFI_MENU_LIST_OFF_ID,
    MMIWIFI_MENU_LIST_AP_ID,
    MMIWIFI_MENU_LIST_SCAN_ID,
    MMIWIFI_SSID_CTRL_ID,
    MMIWIFI_NETWORK_STATE_CTRL_ID,
    MMIWIFI_NETWORK_TYPE_CTRL_ID,
    MMIWIFI_AUTH_TYPE_CTRL_ID,
    MMIWIFI_AUTH_SETTING_CTRL_ID,
    MMIWIFI_IP_SETTING_CTRL_ID,
    MMIWIFI_SAVE_SETTING_CTRL_ID,
    MMIWIFI_MENU_IP_ADDRESS_CTRL_ID,
    MMIWIFI_MENU_IP_GATEWAY_CTRL_ID,
    MMIWIFI_MENU_IP_SUBMASK_CTRL_ID,
    MMIWIFI_MENU_IP_DNS1_CTRL_ID,
    MMIWIFI_MENU_IP_DNS2_CTRL_ID,
    MMIWIFI_MENU_USERNAME_CTRL_ID,
    MMIWIFI_MENU_PASSWORD_CTRL_ID,
/* Do Not add between MMIWIFI_DEF_AP_SETTING_NODE_ACTIVE and MMIWIFI_DEF_AP_SETTING_NODE_AUTO_CONNECTING */
    MMIWIFI_DEF_AP_SETTING_NODE_ACTIVE,
    MMIWIFI_DEF_AP_SETTING_NODE_SUBSCRIBE,
    MMIWIFI_DEF_AP_SETTING_NODE_PERSONAL_INFO,
    MMIWIFI_DEF_AP_SETTING_NODE_AUTO_CONNECTING,

    MMIWIFI_POPMENU_ID_CONNECT,
    MMIWIFI_POPMENU_ID_DISCONNECT,
    MMIWIFI_POPMENU_ID_TRUSTED_LIST,
    MMIWIFI_POPMENU_ID_REFRESH,
    MMIWIFI_POPMENU_ID_SPECTIFY_SEARCH_SSID,
    MMIWIFI_POPMENU_ID_DETAIL,
    MMIWIFI_POPMENU_ID_DEFAULT_AP_SETTING,
    MMIWIFI_POPMENU_ID_EDIT,
    MMIWIFI_POPMENU_ID_ADD_APN,
    MMIWIFI_POPMENU_ID_DELETE,
    MMIWIFI_POPMENU_ID_CHANGE_PRIORITY_HIGH,
    MMIWIFI_POPMENU_ID_CHANGE_PRIORITY_LOW,

    MMIWIFI_POPMENU_ID_ADD,
    MMIWIFI_POPMENU_ID_CLEAR,
    MMIWIFI_POPMENU_ID_SETTING,
    MMIWIFI_POPMENU_ID_FORGET_AP,

    MMIWIFI_POPMENU_ID_ADD_AP,

    MMIWIFI_MENU_ID_MAX
}MMIWIFI_MENU_ID_E;
		
		
#define MENU_DEF(_TABLEID, _ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, \
	_ITEMCOUNT) \
     _TABLEID,

typedef enum
{
    MMIWIFI_MENU_LABEL_START = (MMI_MODULE_WIFI << 16),

    #include "mmiwlan_menutable.def"

    MMIWIFI_MENUTABLE_MAX
} MMIWIFI_MENU_LABEL_E;

#undef MENU_DEF

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
/*-------------------------------------------------------------------------*/

#endif //_MMIWLAN_MENUTABLE_H_

