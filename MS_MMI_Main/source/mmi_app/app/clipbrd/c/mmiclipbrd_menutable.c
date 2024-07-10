/*****************************************************************************
** File Name:      mmiclipbrd_menutable.c                                    *
** Author:         hua.fang                                                  *
** Date:           06/05/2010                                                *
** Copyright:      2010 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:                                                              *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 05/2010        hua.fang          Create                                   *
******************************************************************************/

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "guimenu.h"
#include "mmiclipbrd_menutable.h"
#include "mmiclipbrd_text.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmi_image.h"
#include "mmi_menutable.h"

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/
const GUIMENU_ITEM_T menu_clipboard[] =
{
    {CLIPBRD_MENU_COPY_ITEM_ID,     TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL}, TXT_COMMON_COPY,     0, 0, 0, MENU_NULL},
    {CLIPBRD_MENU_CUT_ITEM_ID,      TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL}, TXT_COMMON_CUT,      0, 0, 0, MENU_NULL},
    {CLIPBRD_MENU_PASTE_ITEM_ID,    TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL}, TXT_CLIPBRDMENU_PASTE,    0, 0, 0, MENU_NULL},
    {CLIPBRD_MENU_SELECTALL_ITEM_ID,TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL}, TXT_IKB_SELECT_ALL,0, 0, 0, MENU_NULL},
};

#define MENU_DEF(_TABLEID, _ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, \
    _ITEMCOUNT) \
     _ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, _ITEMCOUNT, 

const GUIMENU_GROUP_T mmiclipbrd_menu_table[] = 
{
    {NULL},
#include "mmiclipbrd_menutable.def"
};

#undef MENU_DEF

/**--------------------------------------------------------------------------*/
/**                         EXTERNAL DECLARE                                 */
/**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*/
/**                         STATIC DEFINITION                                */
/**--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*/
/**                         GLOBAL DEFINITION                                */
/**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : register clipboard menu group
//  Global resource dependence : 
//  Author: hua.fang
//  Note: 
/*****************************************************************************/
PUBLIC void MMICLIPBRD_RegMenuGroup(void)
{
    MMI_RegMenuGroup(MMI_MODULE_CLIPBRD, mmiclipbrd_menu_table);
}

