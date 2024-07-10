/*****************************************************************************
** File Name:      mmibt_menutable.c                                            *
** Author:                                                                   *
** Date:           2006/08/17                                                *
** Copyright:      2006 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to handle fm menutable                  *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2006/08/17     kelly.li      Creat                                     *
******************************************************************************/

//#ifndef _MMIBT_MENUTABLE_C_
#define _MMIBT_MENUTABLE_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#ifdef WIN32
#include "std_header.h"
#endif
#include "window_parse.h"
#include "guimenu.h"
#include "mmi_text.h"
#include "mmi_image.h"
#include "mmieng_menutable.h"
#include "mmi_menutable.h"
#include "mmi_modu_main.h"

#include "mmibt_menutable.h"
#include "mmibt_id.h"
#include "mmibt_text.h"

#include "mmi_module.h"
#include "mmi_common.h"
#include "mmi_nv.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/

const GUIMENU_ITEM_T menu_bluetooth[] =
{
    { ID_BT_HANDFREE, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_COMMON_HAND_FREE, 0, 0, 0, MENU_NULL },
    { ID_BT_DEVICE_LIST, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_BT_DEVICE_LIST, 0, 0, 0, MENU_NULL },  
    { ID_MY_BT_SETTING, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, STXT_MAIN_SETTING, 0, 0, 0, MENU_NULL },
};

const GUIMENU_ITEM_T menu_my_bt_setting[] =
{
    { ID_MY_BT_STATE, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_MY_BT_STATE, 0, 0, 0, MENU_NULL },
    { ID_MY_BT_VISIBILITY, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_MY_BT_VISIBILITY, 0, 0, 0, MENU_NULL },
    { ID_MY_BT_DEVICE_NAME, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_MY_BT_DEVICE_NAME, 0, 0, 0, MENU_NULL },
#ifdef NANDBOOT_SUPPORT    
    { ID_BT_SAVE_FILE_LOCATION, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_BT_SAVE_FILE_LOCATION, 0, 0, 0, MENU_NULL },	
#endif
};

#ifdef MMI_PDA_SUPPORT
const GUIMENU_ITEM_T menu_bt_pda_paired_details[] =
{
 { ID_BT_DEVICE_DELETE_ALL, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_DELALL, IMAGE_COMMON_OPTMENU_REMOVE_ALL, IMAGE_COMMON_OPTMENU_REMOVE_ALL, IMAGE_COMMON_OPTMENU_REMOVE_ALL, MENU_NULL },
};
#endif
//device option menu of headset disconnect 
const GUIMENU_ITEM_T menu_bt_device_details[] =
{
 { ID_BT_DEVICE_CONNECT, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_COMMON_CONNECT_COMN, 0, 0, 0, MENU_NULL },
 { ID_BT_DEVICE_RENAME, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_BT_DEVICE_RENAME, 0, 0, 0, MENU_NULL },
 { ID_BT_DEVICE_DELETE, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_DELETE, 0, 0, 0, MENU_NULL },
#ifndef MMI_PDA_SUPPORT
 { ID_BT_DEVICE_DELETE_ALL, TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN}, TXT_DELALL, 0, 0, 0, MENU_NULL },
#endif
 { ID_BT_DEVICE_DETAIL, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_BT_DEVICE_DETAILS, 0, 0, 0, MENU_NULL },
#ifdef MMIBT_HEADSET_AUTHORIZE 
 { ID_BT_ACCREDIT_SETTINGS, TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN}, TXT_BT_ACCREDIT_SETTINGS, 0, 0, 0, MENU_BT_ACCREDIT_SETTINGS_OPT},   
#endif
};
//device option menu of headset connect 
const GUIMENU_ITEM_T menu_bt_deactive_device_details[] =
{
 { ID_BT_DEVICE_DISCONNECT, TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN}, TXT_COMM_DISCONNECT, 0, 0, 0, MENU_NULL },
 { ID_BT_DEVICE_RENAME, TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN}, TXT_BT_DEVICE_RENAME, 0, 0, 0, MENU_NULL },
 { ID_BT_DEVICE_DELETE, TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN}, TXT_DELETE, 0, 0, 0, MENU_NULL },
#ifndef MMI_PDA_SUPPORT
 { ID_BT_DEVICE_DELETE_ALL, TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN}, TXT_DELALL, 0, 0, 0, MENU_NULL },
#endif
 { ID_BT_DEVICE_DETAIL, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_BT_DEVICE_DETAILS, 0, 0, 0, MENU_NULL },
#ifdef MMIBT_HEADSET_AUTHORIZE 
 { ID_BT_ACCREDIT_SETTINGS, TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN}, TXT_BT_ACCREDIT_SETTINGS, 0, 0, 0, MENU_BT_ACCREDIT_SETTINGS_OPT},
#endif
};

//device option menu of which is not headset 
const GUIMENU_ITEM_T menu_other_bt_device_details[] =
{
 { ID_BT_DEVICE_CONNECT, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_COMMON_CONNECT_COMN, 0, 0, 0, MENU_NULL },
 { ID_BT_DEVICE_DISCONNECT, TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN}, TXT_COMM_DISCONNECT, 0, 0, 0, MENU_NULL },
 { ID_BT_ACCREDIT_SETTINGS, TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN}, TXT_BT_ACCREDIT_SETTINGS, 0, 0, 0, MENU_BT_ACCREDIT_SETTINGS_OPT},
 { ID_BT_DEVICE_RENAME, TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN}, TXT_BT_DEVICE_RENAME, 0, 0, 0, MENU_NULL },
 { ID_BT_DEVICE_DELETE, TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN}, TXT_DELETE, 0, 0, 0, MENU_NULL },
#ifndef MMI_PDA_SUPPORT
 { ID_BT_DEVICE_DELETE_ALL, TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN}, TXT_DELALL, 0, 0, 0, MENU_NULL },
#endif
 { ID_BT_DEVICE_DETAIL, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_BT_DEVICE_DETAILS, 0, 0, 0, MENU_NULL },
};

//settings option 
const GUIMENU_ITEM_T menu_bt_accredit_settings_opt[] =
{
    {ID_BT_ASK_EVERYTIME,  TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_COMMON_ASK_EVERYTIME,0, 0, 0, NULL},
    {ID_BT_AUTO_CONNECT,   TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_BT_AUTO_CONNECT,0, 0, 0, NULL},
};    
#define MENU_DEF(_TABLEID, _ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, \
	_ITEMCOUNT) \
     _ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, _ITEMCOUNT, 

const GUIMENU_GROUP_T bt_menu_table[] = 
{
    {NULL},
    #include "mmibt_menutable.def"
};



/*****************************************************************************/
// 	Description : Register bt menu group
//	Global resource dependence : none
//  Author: kelly.li
//	Note:
/*****************************************************************************/
PUBLIC void MMIBT_RegMenuGroup(void)
{
    MMI_RegMenuGroup(MMI_MODULE_BT, bt_menu_table);
}



//#endif

