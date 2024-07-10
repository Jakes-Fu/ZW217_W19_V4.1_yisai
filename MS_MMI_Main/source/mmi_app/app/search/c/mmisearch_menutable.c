/*****************************************************************************
** File Name:      mmisearch_menutable.c                                     *
** Author:                                                                   *
** Date:           03/2010                                                   *
** Copyright:      2010 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 11/2008      Jassmine              Creat
******************************************************************************/

#define _MMIPICVIEW_MENUTABLE_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#ifdef SEARCH_SUPPORT
#ifdef WIN32
#include "std_header.h"
#endif
#include "guimenu.h"
#include "mmisearch_menutable.h"
#include "mmi_text.h"
#include "mmisearch_text.h"
#include "mmi_modu_main.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define TIP_NULL    0
#ifndef MENU_NULL
#define MENU_NULL   0
#endif

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
//list popmenu
#ifdef MMI_PDA_SUPPORT
const GUIMENU_ITEM_T menu_search_file_menu_opt[] =
{
    {ID_SEARCH_MARKED,   TIP_NULL, {STXT_OK,TXT_NULL,STXT_RETURN},STXT_MARK,   IMAGE_COMMON_OPTMENU_MARK, 0, 0, MENU_SEARCH_MARK},
    {ID_SEARCH_SORT,     TIP_NULL, {STXT_OK,TXT_NULL,STXT_RETURN},TXT_COMMON_SORT,   IMAGE_COMMON_OPTMENU_SORT, 0, 0, MENU_SEARCH_SORT_OPT}
};

const GUIMENU_ITEM_T menu_search_file_list_opt[] =
{
    {ID_SEARCH_OPEN,     TIP_NULL, {STXT_OK,TXT_NULL,STXT_RETURN},TXT_VIEW,    0, 0, 0, MENU_NULL},
    {ID_SEARCH_DELETE,   TIP_NULL, {STXT_OK,TXT_NULL,STXT_RETURN},TXT_DELETE, 0, 0, 0, MENU_NULL},
    {ID_SEARCH_SEND,     TIP_NULL, {STXT_OK,TXT_NULL,STXT_RETURN},TXT_SEND,    0, 0, 0, MENU_SEARCH_SEND},
    {ID_SEARCH_DETAIL,   TIP_NULL, {STXT_OK,TXT_NULL,STXT_RETURN},STXT_DETAIL, 0, 0, 0, MENU_NULL},
    {TXT_COMMON_RENAME,   TIP_NULL, {TXT_COMMON_OK,TXT_NULL,STXT_RETURN},TXT_COMMON_RENAME, 0, 0, 0, MENU_NULL},
};
#else
const GUIMENU_ITEM_T menu_search_file_list_opt[] =
{
    {ID_SEARCH_OPEN,     TIP_NULL, {STXT_OK,TXT_NULL,STXT_RETURN},TXT_VIEW,    0, 0, 0, MENU_NULL},
    {ID_SEARCH_DELETE,   TIP_NULL, {STXT_OK,TXT_NULL,STXT_RETURN},TXT_DELETE, 0, 0, 0, MENU_NULL},
    {ID_SEARCH_SEND,     TIP_NULL, {STXT_OK,TXT_NULL,STXT_RETURN},TXT_SEND,    0, 0, 0, MENU_SEARCH_SEND},
    {ID_SEARCH_DETAIL,   TIP_NULL, {STXT_OK,TXT_NULL,STXT_RETURN},STXT_DETAIL, 0, 0, 0, MENU_NULL},
    {ID_SEARCH_RENAME,   TIP_NULL, {TXT_COMMON_OK,TXT_NULL,STXT_RETURN},TXT_COMMON_RENAME, 0, 0, 0, MENU_NULL},
    {ID_SEARCH_MARKED,   TIP_NULL, {STXT_OK,TXT_NULL,STXT_RETURN},STXT_MARK,   0, 0, 0, MENU_SEARCH_MARK},
    {ID_SEARCH_SORT,     TIP_NULL, {STXT_OK,TXT_NULL,STXT_RETURN},TXT_COMMON_SORT,   0, 0, 0, MENU_SEARCH_SORT_OPT}
};
#endif

//send
const GUIMENU_ITEM_T menu_search_send[] =
{
    {ID_SEARCH_SEND_MMS,    TIP_NULL, {STXT_OK,TXT_NULL,STXT_RETURN},TXT_MMS,       0, 0, 0, MENU_NULL},
#ifdef BLUETOOTH_SUPPORT
    {ID_SEARCH_SEND_BT,     TIP_NULL, {STXT_OK,TXT_NULL,STXT_RETURN},TXT_BLUETOOTH,        0, 0, 0, MENU_NULL}
#endif
};

//mark
const GUIMENU_ITEM_T menu_search_mark[] =
{
    {ID_SEARCH_MARKED,         TIP_NULL, {STXT_OK,TXT_NULL,STXT_RETURN},STXT_MARK,              0, 0, 0, MENU_NULL},
    {ID_SEARCH_UNMARKED,       TIP_NULL, {STXT_OK,TXT_NULL,STXT_RETURN},TXT_CANCEL_MARK,       0, 0, 0, MENU_NULL},
    {ID_SEARCH_MARKED_ALL,     TIP_NULL, {STXT_OK,TXT_NULL,STXT_RETURN},TXT_MARK_ALL,          0, 0, 0, MENU_NULL},
    {ID_SEARCH_UNMARKED_ALL,   TIP_NULL, {STXT_OK,TXT_NULL,STXT_RETURN},TXT_CANCEL_MARK_ALL,   0, 0, 0, MENU_NULL}
};

//sort option 
const GUIMENU_ITEM_T menu_search_sort_opt[] =
{
    {ID_SEARCH_NAME,   TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_COMMON_NAME,0, 0, 0, NULL},
    {ID_SEARCH_DATE,   TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_DATE,0, 0, 0, NULL},
    {ID_SEARCH_SIZE,   TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_COMMON_SIZE,0, 0, 0, NULL},
    {ID_SEARCH_TYPE,   TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_COMMON_TYPE,0, 0, 0, NULL}
};

#if 1 //!defined TOUCH_PANEL_SUPPORT // hermann liu
const GUIMENU_ITEM_T menu_search_type_opt[] =
{
    {ID_SEARCH_TYPE_ALL,   TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN},TXT_COMMON_ALL,0, 0, 0, NULL},
    {ID_SEARCH_TYPE_APP,   TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_APPLICATION,0, 0, 0, NULL},
    {ID_SEARCH_TYPE_FILE,   TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_FILE,0, 0, 0, NULL},
    {ID_SEARCH_TYPE_PB,   TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},STXT_MAIN_PB,0, 0, 0, NULL},
    {ID_SEARCH_TYPE_MSG,   TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},STXT_MAIN_MESSAGE,0, 0, 0, NULL},
    {ID_SEARCH_TYPE_CL,   TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},STXT_MAIN_CL,0, 0, 0, NULL}
};

#endif
/*---------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                 */
/*---------------------------------------------------------------------------*/
#define MENU_DEF(_TABLEID, _ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, \
    _ITEMCOUNT) \
    _ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, _ITEMCOUNT, 

const GUIMENU_GROUP_T search_menu_table[] = 
{
    {NULL},
    #include "mmisearch_menutable.def"
};

#undef MENU_DEF
/**--------------------------------------------------------------------------*
 **                         EXTERNAL DEFINITION                              *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : register picture viewer menu group
//	Global resource dependence : none
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC void MMISEARCH_RegMenuGroup(void)
{
    MMI_RegMenuGroup(MMI_MODULE_SEARCH, search_menu_table);
}
#endif
