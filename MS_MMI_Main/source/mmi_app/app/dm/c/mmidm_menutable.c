/*****************************************************************************
** File Name:      mmidm_menutable.c                                         *
** Author:         jian.ma                                                   *
** Date:           2009/07/02                                                *
** Copyright:      2009 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to handle dm menutable                  *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2009/07/02     jian.ma           Create                                   *
******************************************************************************/


#define _MMIDM_MENUTABLE_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
//#include "std_header.h"
#if (defined(DM_SUPPORT) || defined(DM_ONLY_SELFREG_SUPPORT))
#include "window_parse.h"
#include "guimenu.h"
#include "mmidm_menutable.h"
#include "mmieng_menutable.h"
#include "mmi_menutable.h"
#include "mmi_modu_main.h"
#include "mmidm_text.h"
#include "mmi_module.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmieng_text.h"
#include "mmidm_app.h"
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/

#ifndef DM_ONLY_SELFREG_SUPPORT
const GUIMENU_ITEM_T menu_dm_main_menu[] =
{
    { ID_DM_SOFTWARE_UPDATE_SERVICE, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_DM_SOFTWARE_UPDATE_SERVICE, 0, 0, 0, MENU_DM_UPDATE_MENU},
};


const GUIMENU_ITEM_T menu_dm_update_menu[] =
{
    { ID_DM_SOFTWARE_VERSION, TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN}, TXT_DM_SOFTWARE_VERSION, 0, 0, 0, MENU_NULL },
#if defined (DM_SUPPORT) && defined (DM_FOTA_SUPPORT)
    { ID_DM_SOFTWARE_UPDATE, TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN}, TXT_DM_SOFTWARE_UPDATE, 0, 0, 0, MENU_NULL },
    { ID_DM_UPDATE_PACKAGE, TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN}, TXT_DM_INSTALL_UPDATE_PAG, 0, 0, 0, MENU_NULL },
#endif
#if !defined(MMI_MULTI_SIM_SYS_SINGLE)     
    { ID_DM_SELECT_SIM, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_DM_SEL_SIM, 0, 0, 0, MENU_NULL },//mary for add mutisim
#endif
};
#endif

const GUIMENU_ITEM_T menu_dm_param_set[] = 
{
	{ID_DM_TEST_SERVER, TIP_NULL,{TXT_COMMON_OK, TXT_NULL, STXT_RETURN},
	 TXT_DM_CHANGE_SERVER, 0, 0, 0, MENU_NULL},
	{ID_DM_IMEI_SET, TIP_NULL,{TXT_COMMON_OK, TXT_NULL, STXT_RETURN},
	 TXT_PHONE_IMEI, 0, 0, 0, MENU_NULL},
	{ID_DM_SMS_REG, TIP_NULL,{TXT_COMMON_OK, TXT_NULL, STXT_RETURN},
	 TXT_DM_SMS_REG, 0, 0, 0, MENU_NULL},
	{ID_DM_CNNECT_SERVER, TIP_NULL,{TXT_COMMON_OK, TXT_NULL, STXT_RETURN},
	 TXT_DM_CNNECT_SERVER, 0, 0, 0, MENU_NULL},
	{ID_DM_DEBUG_MODE, TIP_NULL,{TXT_COMMON_OK, TXT_NULL, STXT_RETURN},
	 TXT_COMM_DEBUG_MODE, 0, 0, 0, MENU_NULL},
	 #ifndef DM_ONLY_SELFREG_SUPPORT
	{ID_DM_LOG, TIP_NULL,{TXT_COMMON_OK, TXT_NULL, STXT_RETURN},
	 TXT_DM_LOG_SWITCH, 0, 0, 0, MENU_NULL},
// 	{ID_DM_LOCK, TIP_NULL,{STXT_OK, TXT_NULL, STXT_RETURN},
// 	 TXT_DM_HANDSET_LOCK, 0, 0, 0, MENU_NULL},
	{ID_DM_FOTA, TIP_NULL,{TXT_COMMON_OK, TXT_NULL, STXT_RETURN},
	 TXT_DM_FOTA_SWITCH, 0, 0, 0, MENU_NULL},
// 	{ID_DM_TREE, TIP_NULL,{STXT_OK, TXT_NULL, STXT_RETURN},
// 	 TXT_DM_TREE_SWITCH, 0, 0, 0, MENU_NULL},
    {ID_DM_APN_SET, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},
	 TXT_DM_APN_SET, 0, 0, 0, MENU_NULL},
	 #endif
	{ID_DM_MON_SET, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},
	 TXT_DM_MON_SET, 0, 0, 0, MENU_NULL},
	{ID_DM_DEV_VERSION_SET, TIP_NULL,{TXT_COMMON_OK, TXT_NULL, STXT_RETURN},
	 TXT_DM_DEV_VERSION_SET, 0, 0, 0, MENU_NULL},
	{ID_DM_MAN_SET, TIP_NULL,{TXT_COMMON_OK, TXT_NULL, STXT_RETURN},
	 TXT_DM_MAN_SET, 0, 0, 0, MENU_NULL},
// 	 #ifndef DM_ONLY_SELFREG_SUPPORT
// 	{ID_DM_NULL_SESSION, TIP_NULL,{STXT_OK, TXT_NULL, STXT_RETURN},
// 	 TXT_DM, 0, 0, 0, MENU_NULL},
// 	 #endif
	{ID_DM_SWITCH, TIP_NULL,{TXT_COMMON_OK, TXT_NULL, STXT_RETURN},
	 TXT_DM_SWITCH, 0, 0, 0, MENU_NULL},
	#ifndef DM_ONLY_SELFREG_SUPPORT
	{ID_DM_RESET, TIP_NULL,{TXT_COMMON_OK, TXT_NULL, STXT_RETURN},
	 TXT_DM_RESET, 0, 0, 0, MENU_NULL},
	 #endif
	{ID_DM_SRV_ADDR_SET, TIP_NULL,{TXT_COMMON_OK, TXT_NULL, STXT_RETURN},
	 TXT_DM_SRV_ADDR, 0, 0, 0, MENU_NULL},
	{ID_DM_SELF_REG_NUMBER, TIP_NULL,{TXT_COMMON_OK, TXT_NULL, STXT_RETURN},
	 TXT_DM_SELF_REG_NUMBER, 0, 0, 0, MENU_NULL},
	{ID_DM_SELF_REGPORT_NUMBER, TIP_NULL,{TXT_COMMON_OK, TXT_NULL, STXT_RETURN},
	 TXT_DM_SELF_REGPORT_NUMBER, 0, 0, 0, MENU_NULL},
    {ID_DM_SCOMO_INSTALL, TIP_NULL,{STXT_OK, TXT_NULL, STXT_RETURN}, TXT_DM_SCOMO_INSTALL, 0, 0, 0, MENU_NULL},
#ifndef  MMI_MULTI_SIM_SYS_SINGLE
    {ID_DM_MAIN_SIM, TIP_NULL,{STXT_OK, TXT_NULL, STXT_RETURN}, TXT_DM_MAIN_SIM, 0, 0, 0, MENU_NULL},
#endif
};

#define MENU_DEF(_TABLEID, _ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, \
	_ITEMCOUNT) \
     _ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, _ITEMCOUNT, 

const GUIMENU_GROUP_T dm_menu_table[] = 
{
    {NULL},
    #include "mmidm_menutable.def"
};



/*****************************************************************************/
// 	Description : Register dm menu group
//	Global resource dependence : none
//  Author: jian.ma
//	Note:
/*****************************************************************************/
PUBLIC void MMIDM_RegMenuGroup(void)
{
    MMI_RegMenuGroup(MMI_MODULE_DM, dm_menu_table);
}
#endif

