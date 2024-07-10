
/*****************************************************************************
** File Name:      mmidcd_menutable.c    	                             *
** Author:                                                                   *
** Date:           2007/9/24												 *
** Copyright:      2007 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    						             *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2007/9/24      violent.wei      Create                                    *
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

#include "mmi_modu_main.h"
#include "mmi_menutable.h"
#include "mmi_text.h"
#include "mmi_common.h"
#include "mmi_nv.h"

#ifdef MMI_ENABLE_DCD	//!宏定义开头
#include "mmidcd_id.h"
#include "mmidcd_menutable.h"
#include "mmidcd_main.h"
#include "mmidcd_text.h"
#include "Mmiset_text.h"
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/
//#define TIP_NULL 0

const GUIMENU_ITEM_T	menu_dcd_item_setting_opt[] =
{
    {ID_DCD_ITEM_SETTING_DCD_OPEN_SWITCH, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_DCD_SERVICE_SWITCH, 0, 0, 0, MENU_NULL},
    {ID_DCD_ITEM_SETTING_CONNECT_MODE, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_DCD_CONNECT_MODE, 0, 0, 0, MENU_NULL},
    {ID_DCD_ITEM_SETTING_SCROLL_TIME, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_DCD_SCROLL_TIME, 0, 0, 0, MENU_NULL},
    {ID_DCD_ITEM_SETTING_SHOW_READED, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_DCD_SHOW_READED, 0, 0, 0, MENU_NULL},
#if !defined(MMI_MULTI_SIM_SYS_SINGLE)
    {ID_DCD_ITEM_SETTING_SIM_INTERET, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_DUAL_SIM_SWITCH, 0, 0, 0, MENU_NULL},
#endif    
    {ID_DCD_ITEM_SETTING_RECORY_FACRORTY, TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN}, TXT_COMMON_RESTORE_FACTORY_SETTINGS, 0, 0, 0, MENU_NULL},
};

#define MENU_DEF(_TABLEID, _ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, \
	_ITEMCOUNT) \
     _ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, _ITEMCOUNT, 

const GUIMENU_GROUP_T dcd_menu_table[] = 
{
    {NULL},
    #include "mmidcd_menutable.def"
};

#undef MENU_DEF

/*****************************************************************************/
//      Description : Register dcd menu group
//      Global resource dependence : none
//      Author: ryan.xu
//      Note:
/*****************************************************************************/
PUBLIC void MMIDCD_RegMenuGroup(void)
{
    MMI_RegMenuGroup(MMI_MODULE_DCD, dcd_menu_table);
}
#endif /*#ifdef MMI_ENABLE_DCD*/



