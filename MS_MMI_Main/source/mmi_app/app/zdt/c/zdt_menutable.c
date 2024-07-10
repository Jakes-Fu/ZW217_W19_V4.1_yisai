/*****************************************************************************
** File Name:      mmicdma_menutable.c                                         *
** Author:                                                                   *
** Date:           2006/09/25                                                *
** Copyright:      2006 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to handle cc menutable                  *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2006/09/25     bruce.chi         Create                                   *
******************************************************************************/

#define _MMIZDT_MENUTABLE_C_
/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "std_header.h"
#include "window_parse.h"
#include "sci_types.h"
#include "mmi_module.h"
#include "guimenu.h"

#include "mmi_menutable.h"

#include "mmi_text.h"
#include "zdt_text.h"
#include "zdt_id.h"
#include "mmi_common.h"
#include "zdt_menutable.h"

#if defined(__POPO_MAINMENU_A__)&& defined (__PV_VIDEO__)
#define __POPO_PLAT_PARTY__
#include "..\\\\..\\\\..\\\\popo_pv\\\\popo_res\\\\res_inc\\\\popo_ui_id.h"
#endif //#if defined(__POPO_MAINMENU_A__)&& defined (__PV_VIDEO__)
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/
#define TIP_NULL 0

const GUIMENU_ITEM_T	menu_zdt_test[]  =
{
    {ID_ZDT_MENU_TEST1, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_ZDT_TEST, 0, 0, 0, MENU_NULL},
    {ID_ZDT_MENU_TEST2, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_ZDT_TEST, 0, 0, 0, MENU_NULL},
};
#ifdef ZDT_PLAT_YX_SUPPORT

const GUIMENU_ITEM_T	menu_zdt_yx_net[]  =
{
    {ID_ZDT_MENU_BH_ONOFF, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_BH_NET_ONOFF, 0, 0, 0, MENU_NULL},
#ifdef ZDT_PLAT_YX_SUPPORT_VOICE
    {ID_ZDT_MENU_BH_CHAT, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_BH_NET_CHAT, 0, 0, 0, MENU_NULL},
#endif
    {ID_ZDT_MENU_BH_2VM, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_BH_NET_2VM, 0, 0, 0, MENU_NULL},
    {ID_ZDT_MENU_BH_HELP, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_BH_NET_HELP, 0, 0, 0, MENU_NULL},
};
#endif

#ifdef ZDT_PLAT_SHB_SUPPORT
const GUIMENU_ITEM_T menu_zdt_shb[] =
{
	{ID_ZDT_MENU_SHB_SERVER, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_ZDT_SHB_SERVER, 0, 0, 0, MENU_NULL},
	{ID_ZDT_MENU_SHB_SPEEDDIAL, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_ZDT_SHB_SPEEDDIAL, 0, 0, 0, MENU_NULL},
#ifdef SOS_KEYLONGWEB
	{ID_ZDT_MENU_SHB_SOS, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_ZDT_SHB_SOS, 0, 0, 0, MENU_NULL},
 #endif
	{ID_ZDT_MENU_SHB_HELP, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_ZDT_SHB_HELP, 0, 0, 0, MENU_NULL},

	
};


const GUIMENU_ITEM_T menu_zdt_about_shb[] =
{
	{ID_ZDT_MENU_SHB_INTRODUCTION, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_ZDT_SHB_INTRODUCTION, 0, 0, 0, MENU_NULL},
	//{ID_ZDT_MENU_SHB_USERGUIDE, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_ZDT_SHB_USERGUIDE, 0, 0, 0, MENU_NULL},
	{ID_ZDT_MENU_SHB_CLIENTDOWNLOAD, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_ZDT_SHB_CLIENTDOWNLOAD, 0, 0, 0, MENU_NULL},
	{ID_ZDT_MENU_SHB_HOTLINE, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_ZDT_SHB_HOTLINE, 0, 0, 0, MENU_NULL},
	{ID_ZDT_MENU_SHB_VERSION, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_ZDT_SHB_VERSION, 0, 0, 0, MENU_NULL},
	//{ID_ZDT_MENU_SHB_ABOUTHELP, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_ZDT_SHB_ABOUTHELP, 0, 0, 0, MENU_NULL},

	
};




#endif


#define MENU_DEF(_TABLEID, _ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, \
	_ITEMCOUNT) \
     _ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, _ITEMCOUNT, 

const GUIMENU_GROUP_T mmizdt_menu_table[] = 
{
    {NULL},
    #include "zdt_menutable.def"
};

#undef MENU_DEF

/*****************************************************************************/
// 	Description : Register cc menu group
//	Global resource dependence : none
//  Author: bruce.chi
//	Note:
/*****************************************************************************/
PUBLIC void MMIZDT_RegMenuGroup(void)
{
    MMI_RegMenuGroup(MMI_MODULE_ZDT, mmizdt_menu_table);
}

