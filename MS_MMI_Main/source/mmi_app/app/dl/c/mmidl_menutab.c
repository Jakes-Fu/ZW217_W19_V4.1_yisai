/****************************************************************************
** File Name:      mmidl_menutab.c                                            *
** Author:                                                                    *
** Date:           07/28/2009                                                *
** Copyright:      2004 Spreadtrum, Incorporated. All Rights Reserved.        *
** Description:    This file is used to describe the window for download            *
*****************************************************************************
**                         Important Edit History                            *
** -------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                                *
** 07/2009        wancan.you       Create
** 
****************************************************************************/
#define _MMICC_MENUTABLE_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#ifdef WIN32
#include "std_header.h"
#endif
#include "guimenu.h"
#include "mmi_menutable.h"
#include "mmidl_text.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmidl_menutab.h"
#include "mmi_modu_main.h"
#ifdef DL_SUPPORT
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
//set www page option menu 
const GUIMENU_ITEM_T menu_dl_manager_option[] =
{
#ifdef MMIDL_RESUME_BROKEN_TRANSFER_SUPPORT
    {ID_DL_MANAGER_OPT_PAUSE,       TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_DL_PAUSE_DOWNLOAD,    0, 0, 0, MENU_NULL},
    {ID_DL_MANAGER_OPT_RESUME,      TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_DL_RESUME_DOWNLOAD,      0, 0, 0, MENU_NULL},
    {ID_DL_MANAGER_OPT_REDOWNLOAD,       TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_DL_RE_DOWNLOAD, 0, 0, 0, MENU_NULL},
#endif
    {ID_DL_MANAGER_OPT_DEL,     TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_DL_DEL_TASK,          0, 0, 0, MENU_NULL},
    {ID_DL_MANAGER_OPT_DEL_ALL,       TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN},TXT_DELALL,       0, 0, 0, MENU_NULL},
    {ID_DL_MANAGER_OPT_DETAIL,       TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},STXT_DETAIL,   0, 0, 0, MENU_NULL},
    {ID_DL_MANAGER_OPT_FILE_EXPLORER,       TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_DL_FILE_EXPLORER,       0, 0, 0, MENU_NULL},
    {ID_DL_MANAGER_OPT_SETTINGS, TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN},STXT_MAIN_SETTING,       0, 0, 0, MENU_NULL}
};

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/
#define MENU_DEF(_TABLEID, _ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, \
     _ITEMCOUNT) \
     _ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, _ITEMCOUNT, 

const GUIMENU_GROUP_T mmidl_menu_table[] = 
{
    {NULL},
    #include "mmidl_menutab.def"
};

#undef MENU_DEF

/*****************************************************************************/
// 	Description : Register download menu group
//	Global resource dependence : none
//  Author: bruce.chi
//	Note:
/*****************************************************************************/
PUBLIC void MMIDL_RegMenuGroup(void)
{
    MMI_RegMenuGroup(MMI_MODULE_DL, mmidl_menu_table);
}
#endif //#ifdef DL_SUPPORT
