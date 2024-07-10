/*****************************************************************************
** File Name:      mmimemo_menutable.c                                    *
** Author:          fen.xie                                                    *
** Date:            05/08/2011                                                *
** Copyright:      2007 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe the browser menu table      *
*****************************************************************************/
#ifdef MMI_MEMO_SUPPORT
/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/

#include "mmimemo_menutable.h"
#include "guimenu.h"
#include "mmimemo_id.h"
#include "mmimemo_text.h"
#include "mmi_text.h"
#include "mmi_modu_main.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmi_image.h"
#include "mmimemo_wintable.h"
//#include "mmi_appmsg.h"
//#include "mmi_mainmenu_export.h"

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**-------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/
const GUIMENU_ITEM_T menu_memo_option[] =
{
#ifdef MMI_PDA_SUPPORT
    {ID_POPUP_MENU_NEW_MEMO,   0, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_NEW, IMAGE_COMMON_OPTMENU_ADD, 0, 0, 0},
#else
    {ID_POPUP_MENU_NEW_MEMO,   0, {STXT_OK, TXT_NULL, STXT_RETURN},TXT_NEW, 0, 0, 0, 0},
    {ID_POPUP_MENU_EDIT_MEMO,   0, {STXT_OK, TXT_NULL, STXT_RETURN},TXT_EDIT, 0, 0, 0, 0},
    {ID_POPUP_MENU_DEL_MEMO,   0, {STXT_OK, TXT_NULL, STXT_RETURN},TXT_DELETE, 0, 0, 0, 0},
    {ID_POPUP_MENU_MARK_MEMO,   0, {STXT_OK, TXT_NULL, STXT_RETURN},STXT_MARK, 0, 0, 0, MENU_MEMO_MARK},
#endif
};

const GUIMENU_ITEM_T menu_memo_mark[] =
{
    {ID_POPUP_MENU_MEMO_MARKED,         0, {STXT_OK,TXT_NULL,STXT_RETURN},STXT_MARK, 0, 0, 0, 0},
    {ID_POPUP_MENU_MEMO_UNMARKED,       0, {STXT_OK,TXT_NULL,STXT_RETURN},TXT_CANCEL_MARK, 0, 0, 0, 0},
    {ID_POPUP_MENU_MEMO_MARKED_ALL,     0, {STXT_OK,TXT_NULL,STXT_RETURN},TXT_MARK_ALL, 0, 0, 0, 0},
    {ID_POPUP_MENU_MEMO_UNMARKED_ALL,   0, {STXT_OK,TXT_NULL,STXT_RETURN},TXT_CANCEL_MARK_ALL, 0, 0, 0, 0},
};

#define MENU_DEF(_TABLEID, _ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, \
	_ITEMCOUNT) \
     _ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, _ITEMCOUNT, 

const GUIMENU_GROUP_T memo_menu_table[] = 
{
    {0},   
    #include "mmimemo_menutable.def"
};
#undef MENU_DEF

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : browser reg menu group
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIMEMO_RegMenuGroup(void)
{
    MMI_RegMenuGroup(MMI_MODULE_MEMO, memo_menu_table);
}

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif


