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

#define _MMIZFB_MENUTABLE_C_
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
#include "zfb_text.h"
#include "zfb_id.h"
#include "mmi_common.h"
#include "zfb_menutable.h"

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

const GUIMENU_ITEM_T	menu_zfb_test[]  =
{
    {ID_ZFB_MENU_TEST, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_ZFB_TEST, 0, 0, 0, MENU_NULL},
};

#define MENU_DEF(_TABLEID, _ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, \
	_ITEMCOUNT) \
     _ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, _ITEMCOUNT, 

const GUIMENU_GROUP_T mmizfb_menu_table[] = 
{
    {NULL},
    #include "zfb_menutable.def"
};

#undef MENU_DEF

/*****************************************************************************/
// 	Description : Register cc menu group
//	Global resource dependence : none
//  Author: bruce.chi
//	Note:
/*****************************************************************************/
PUBLIC void MMIZFB_RegMenuGroup(void)
{
    MMI_RegMenuGroup(MMI_MODULE_ZFB, mmizfb_menu_table);
}

