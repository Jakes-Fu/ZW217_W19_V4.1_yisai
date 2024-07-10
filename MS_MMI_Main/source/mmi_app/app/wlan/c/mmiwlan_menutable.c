/*****************************************************************************
** File Name:      mmiwifi_menutable.c                                       *
** Author:         li.li                                                     *
** Date:           2009/11/18                                                 *
** Copyright:      2007 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to handle wifi menutable                *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2009/11/18     li.li             Create                                    *
******************************************************************************/

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#ifdef WIN32
#include "std_header.h"
#endif
#include "window_parse.h"
#include "mmi_module.h"
#include "guimenu.h"

#include "mmi_menutable.h"
#include "mmi_text.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmiwlan_text.h"
#include "mmiwlan_id.h"
#include "mmiwlan_menutable.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/
const GUIMENU_ITEM_T    mmiwifi_list_menu[] =
{
    {MMIWIFI_POPMENU_ID_CONNECT,  TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_COMMON_CONNECT_COMN,      0, 0,0,MENU_NULL},
    {MMIWIFI_POPMENU_ID_TRUSTED_LIST,  TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_WIFI_TRUSTED_LIST,      0, 0,0,MENU_NULL},
    {MMIWIFI_POPMENU_ID_REFRESH,  TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_REFRESH,      0, 0,0,MENU_NULL},
    {MMIWIFI_POPMENU_ID_SPECTIFY_SEARCH_SSID,  TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_WIFI_SPECTIFY_SEARCH_SSID,      0, 0,0,MENU_NULL},
    {MMIWIFI_POPMENU_ID_DETAIL,  TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, STXT_DETAIL,      0, 0,0,MENU_NULL},
};

const GUIMENU_ITEM_T    mmiwifi_trusted_list_menu[] =
{
    {MMIWIFI_POPMENU_ID_EDIT,  TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN}, TXT_EDIT,      0, 0,0,MENU_NULL},
    {MMIWIFI_POPMENU_ID_ADD_APN,  TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN}, TXT_WIFI_ADD_APN,      0, 0,0,MENU_NULL},
    {MMIWIFI_POPMENU_ID_DELETE,  TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN}, TXT_DELETE,      0, 0,0,MENU_NULL},
    {MMIWIFI_POPMENU_ID_CHANGE_PRIORITY_HIGH,  TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN}, TXT_WIFI_CHANGE_PRIORITY_HIGH,      0, 0,0,MENU_NULL},
    {MMIWIFI_POPMENU_ID_CHANGE_PRIORITY_LOW,  TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN}, TXT_WIFI_CHANGE_PRIORITY_LOW,      0, 0,0,MENU_NULL},
};

const GUIMENU_ITEM_T    mmiwifi_menu_opt2[] =
{
    {MMIWIFI_MENU_ID_DISCONNECT,   TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN}, TXT_COMM_DISCONNECT,              0,  0,0,MENU_NULL},
    {MMIWIFI_MENU_ID_EDIT, TIP_NULL, {TXT_NULL,TXT_NULL, TXT_NULL},    TXT_EDIT,    0,  0,0,MENU_NULL},        
    {MMIWIFI_MENU_ID_SCAN,  TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, STXT_SEARCH,      0, 0,0,MENU_NULL},
    {MMIWIFI_MENU_ID_NEW,  TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN}, TXT_NEW,      0, 0,0,MENU_NULL},
    {MMIWIFI_MENU_ID_DELETE,  TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN}, TXT_DELETE,      0, 0,0,MENU_NULL},
    {MMIWIFI_MENU_ID_CLEAR,  TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN}, TXT_COMMON_CLEAR,      0, 0,0,MENU_NULL},
    {MMIWIFI_MENU_ID_HELP,  TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN}, TXT_HELP,      0, 0,0,MENU_NULL}
};

const GUIMENU_ITEM_T    mmiwifi_edit_menu[] =
{
    {MMIWIFI_SSID_CTRL_ID,   TIP_NULL, {STXT_SAVE, TXT_EDIT, STXT_RETURN}, TXT_WIFI_SSID,              0,  0,0,MENU_NULL},
    {MMIWIFI_NETWORK_STATE_CTRL_ID, TIP_NULL, {STXT_SAVE, TXT_EDIT, STXT_RETURN},    TXT_WIFI_NETWORK_STATE,    0,  0,0,MENU_NULL},        
    {MMIWIFI_NETWORK_TYPE_CTRL_ID,  TIP_NULL, {STXT_SAVE, TXT_EDIT, STXT_RETURN}, TXT_COMM_NETWORK_TYPE,      0, 0,0,MENU_NULL},
    {MMIWIFI_AUTH_TYPE_CTRL_ID,  TIP_NULL, {STXT_SAVE, TXT_EDIT, STXT_RETURN}, TXT_COMM_SECURITY_TYPE,      0, 0,0,MENU_NULL},
    {MMIWIFI_AUTH_SETTING_CTRL_ID,  TIP_NULL, {STXT_SAVE, TXT_EDIT, STXT_RETURN}, TXT_COMM_AUTH_SETTING,      0, 0,0,MENU_NULL},
    {MMIWIFI_IP_SETTING_CTRL_ID,  TIP_NULL, {STXT_SAVE, TXT_EDIT, STXT_RETURN}, TXT_WIFI_IP_SETTING,      0, 0,0,MENU_NULL},
//    {MMIWIFI_SAVE_SETTING_CTRL_ID,  TIP_NULL, {STXT_SAVE, TXT_EDIT, STXT_RETURN}, TXT_WIFI_SAVE_SETTING,      0, 0,0,MENU_NULL}
};

const GUIMENU_ITEM_T    mmiwifi_new_menu[] =
{
    {MMIWIFI_SSID_CTRL_ID,   TIP_NULL, {STXT_SAVE, TXT_EDIT, STXT_RETURN}, TXT_WIFI_SSID,              0,  0,0,MENU_NULL},
    {MMIWIFI_NETWORK_STATE_CTRL_ID, TIP_NULL, {STXT_SAVE, TXT_EDIT, STXT_RETURN},    TXT_WIFI_NETWORK_STATE,    0,  0,0,MENU_NULL},        
    {MMIWIFI_NETWORK_TYPE_CTRL_ID,  TIP_NULL, {STXT_SAVE, TXT_EDIT, STXT_RETURN}, TXT_COMM_NETWORK_TYPE,      0, 0,0,MENU_NULL},
    {MMIWIFI_AUTH_TYPE_CTRL_ID,  TIP_NULL, {STXT_SAVE, TXT_EDIT, STXT_RETURN}, TXT_COMM_SECURITY_TYPE,      0, 0,0,MENU_NULL},
    {MMIWIFI_AUTH_SETTING_CTRL_ID,  TIP_NULL, {STXT_SAVE, TXT_EDIT, STXT_RETURN}, TXT_COMM_AUTH_SETTING,      0, 0,0,MENU_NULL},
    {MMIWIFI_IP_SETTING_CTRL_ID,  TIP_NULL, {STXT_SAVE, TXT_EDIT, STXT_RETURN}, TXT_WIFI_IP_SETTING,      0, 0,0,MENU_NULL},
//    {MMIWIFI_SAVE_SETTING_CTRL_ID,  TIP_NULL, {STXT_SAVE, TXT_EDIT, STXT_RETURN}, TXT_WIFI_SAVE_SETTING,      0, 0,0,MENU_NULL}
};
#define MENU_DEF(_TABLEID, _ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, \
    _ITEMCOUNT) \
_ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, _ITEMCOUNT, 

const GUIMENU_GROUP_T mmiwifi_menu_table[] = 
{
    {NULL},
    #include "mmiwlan_menutable.def"
};

#undef MENU_DEF

/*****************************************************************************/
//     Description : Register wifi menu group
//    Global resource dependence : none
//  Author: ryan.xu
//    Note:
/*****************************************************************************/
PUBLIC void MMIWIFI_RegMenuGroup(void)
{
    MMI_RegMenuGroup(MMI_MODULE_WIFI, mmiwifi_menu_table);
}

