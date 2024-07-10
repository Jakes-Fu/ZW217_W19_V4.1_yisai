/*****************************************************************************
** File Name:      mmiconnection_menutable.c                                 *
** Author:                                                                   *
** Date:           2007/1/31												 *
** Copyright:      2007 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to handle connection menutable          *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2007/1/31      ryan.xu	       Create                                    *
******************************************************************************/

#define _MMICONNECTION_MENUTABLE_C_
/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#ifdef WIN32
#include "std_header.h"
#endif
#include "window_parse.h"
//#include "sci_types.h"
#include "mmi_module.h"
#include "guimenu.h"

#include "mmi_menutable.h"
#include "mmi_text.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmiconnection_text.h"
#include "mmiconnection_id.h"
#include "mmiconnection_menutable.h"
#include "mmiconnection_export.h"

#include "mmi_image.h"

#ifdef BLUETOOTH_SUPPORT
#include "mmibt_text.h"
#endif
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/
#ifdef MMI_GPRS_SUPPORT
#ifdef MMI_PDA_SUPPORT
const GUIMENU_ITEM_T menu_connection_linklist_longok[] =
{
    {ID_CONNECTION_SETTING_DELETE,TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN},TXT_DELETE, 0, 0, 0, MENU_NULL},
    {ID_CONNECTION_SETTING_EDIT,TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN},TXT_EDIT, 0, 0, 0, MENU_NULL},
};
#endif
#endif

#ifdef MMI_PDA_SUPPORT
const GUIMENU_ITEM_T menu_connection_longok_linklist[] =
{
    {ID_CONNECTION_SETTING_DELETE,TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN},TXT_DELETE, 0, 0, 0, MENU_NULL},   
};

const GUIMENU_ITEM_T menu_connection_setting_linklist[] =
{
    {ID_CONNECTION_SETTING_SETGBUILD,TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_CONNECTION_SETGBUILD, IMAGE_COMMON_OPTMENU_CREATE_NEW, 0, 0, MENU_NULL},
#if 0  //bug185707
    {ID_CONNECTION_SETTING_ADDLINKSETTING,TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_CONNECTION_ADDSETTINGLINK, IMAGE_COMMON_OPTMENU_ADD, 0, 0, MENU_NULL},
#endif
    {ID_CONNECTION_SETTING_REASERCH, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},STXT_SEARCH, IMAGE_COMMON_OPTMENU_REFRESH, 0, 0, MENU_NULL},
   //{ID_CONNECTION_SETTING_DELETE,TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN},TXT_DELETE, 0, 0, 0, MENU_NULL},
   //{ID_CONNECTION_SETTING_EDIT,TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN},TXT_EDIT, 0, 0, 0, MENU_NULL},
#if defined(DATA_ACCOUNT_IN_TCARD_SUPPORT) && defined(BROWSER_SUPPORT)
    {ID_CONNECTION_SETTING_DOWNLOAD_RES, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_CONNECTION_DOWNLOAD_RES, IMAGE_COMMON_WEB_ICON, 0, 0, MENU_NULL},
#endif
};
#else
const GUIMENU_ITEM_T menu_connection_setting_linklist[] =
{
    {ID_CONNECTION_SETTING_SETGBUILD,TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN},TXT_CONNECTION_SETGBUILD, 0, 0, 0, MENU_NULL},
#if 0  //bug185707
    {ID_CONNECTION_SETTING_ADDLINKSETTING,TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN},TXT_CONNECTION_ADDSETTINGLINK, 0, 0, 0, MENU_NULL},
#endif
    {ID_CONNECTION_SETTING_REASERCH, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},STXT_SEARCH, 0, 0, 0, MENU_NULL},
    {ID_CONNECTION_SETTING_DELETE,TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN},TXT_DELETE, 0, 0, 0, MENU_NULL},
    {ID_CONNECTION_SETTING_EDIT,TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN},TXT_EDIT, 0, 0, 0, MENU_NULL},
#if defined(DATA_ACCOUNT_IN_TCARD_SUPPORT) && defined(BROWSER_SUPPORT)
    {ID_CONNECTION_SETTING_DOWNLOAD_RES, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_CONNECTION_DOWNLOAD_RES, 0, 0, 0, MENU_NULL},
#endif
};
#endif

//set connection setting link menu 
const GUIMENU_ITEM_T menu_connection_setting_link[] =
{
#ifndef MMI_DUALMODE_ENABLE
    {ID_CONNECTION_SETTING_SECRET,      TIP_NULL, {STXT_SAVE, STXT_MAIN_SETTING, STXT_RETURN},TXT_CONNECTION_AUTH_TYPE, 0, 0, 0, MENU_NULL},
#endif
    {ID_CONNECTION_SETTING_USERNAME,    TIP_NULL, {STXT_SAVE, TXT_EDIT, STXT_RETURN},TXT_COMMON_USERNAME, 0, 0, 0, MENU_NULL},
    {ID_CONNECTION_SETTING_PASSWORD,    TIP_NULL, {STXT_SAVE, TXT_EDIT, STXT_RETURN},TXT_COMMON_PASSWORD, 0, 0, 0, MENU_NULL},
    {ID_CONNECTION_SETTING_APN,         TIP_NULL, {STXT_SAVE, TXT_EDIT, STXT_RETURN},TXT_CONNECTION_APN, 0, 0, 0, MENU_NULL},
    {ID_CONNECTION_SETTING_GATEWAY,     TIP_NULL, {STXT_SAVE, TXT_EDIT, STXT_RETURN},TXT_COMMON_GATEWAY, 0, 0, 0, MENU_NULL},
    {ID_CONNECTION_SETTING_PORT,        TIP_NULL, {STXT_SAVE, TXT_EDIT, STXT_RETURN},TXT_CONNECTION_PORT, 0, 0, 0, MENU_NULL},
    {ID_CONNECTION_SETTING_CHANGENAME,  TIP_NULL, {STXT_SAVE, TXT_EDIT, STXT_RETURN},TXT_CONNECTION_CHANGENAME, 0, 0, 0, MENU_NULL},
    {ID_CONNECTION_SETTING_SETHOMEPAGE,   TIP_NULL, {STXT_SAVE, TXT_EDIT, STXT_RETURN},TXT_CONNECTION_SETHOMEPAGE, 0, 0, 0, MENU_NULL},
    {ID_CONNECTION_SETTING_ADVANCE,     TIP_NULL, {STXT_SAVE, STXT_MAIN_SETTING,  STXT_RETURN},TXT_CONNECTION_ADVANCE, 0, 0, 0, MENU_NULL},
#ifdef IPVERSION_SUPPORT_V4_V6
    {ID_CONNECTION_SETTING_IPTYPE,      TIP_NULL, {STXT_SAVE, STXT_MAIN_SETTING, STXT_RETURN},TXT_CONNECTION_IP_TYPE, 0, 0, 0, MENU_NULL},
#endif
};

const GUIMENU_ITEM_T menu_connection_setting_link_advance[] =
{
    {ID_CONNECTION_SETTING_DNS,      TIP_NULL, {STXT_SAVE, TXT_EDIT, STXT_RETURN},TXT_CONNECTION_DNS, 0, 0, 0, MENU_NULL},
    {ID_CONNECTION_SETTING_NETTYPE,    TIP_NULL, {STXT_SAVE, STXT_MAIN_SETTING, STXT_RETURN},TXT_NETWORK_TYPE, 0, 0, 0, MENU_NULL},
};

#define MENU_DEF(_TABLEID, _ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, \
	_ITEMCOUNT) \
     _ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, _ITEMCOUNT, 

const GUIMENU_GROUP_T mmiconnection_menu_table[] = 
{
    {NULL},
    #include "mmiconnection_menutable.def"
};

#undef MENU_DEF

/*****************************************************************************/
// 	Description : Register connection menu group
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPICONNECTION_RegMenuGroup(void)
{
    MMI_RegMenuGroup(MMI_MODULE_CONNECTION, mmiconnection_menu_table);
}

