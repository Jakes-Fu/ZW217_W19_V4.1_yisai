/*****************************************************************************
** File Name:      mmiim_menutable.c                                         *
** Author:                                                                   *
** Date:           04/20/2004                                                *
** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 11/2008      Jassmine              Creat
******************************************************************************/

#define _MMIIM_MENUTABLE_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#ifdef WIN32
#include "std_header.h"
#endif
#include "guimenu.h"
#include "mmiim_menutable.h"
#include "mmiim_text.h"
#include "mmi_text.h"
#include "mmi_image.h"
#include "mmi_modu_main.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define TIP_NULL    0
#define MENU_NULL   NULL    //lint !e767

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
//im setting
const GUIMENU_ITEM_T mmiim_set_menu[] =
{
    {ID_MMIIM_PY_FUZZY,				TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},IM_TXT_INPUT_PY_FUZZY,				0,0,0,MENU_NULL},
#ifdef TOUCH_PANEL_SUPPORT
    {ID_MMIIM_HANDWRITING_SPEED,	TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},IM_TXT_INPUT_HANDWRITING_SPEED,		0,0,0,MENU_NULL},
    {ID_MMIIM_HANDWRITING_THICKNESS,TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},IM_TXT_INPUT_HANDWRITING_THICKNESS,	0,0,0,MENU_NULL},
    {ID_MMIIM_HANDWRITING_COLOR,	TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},IM_TXT_INPUT_HANDWRITING_COLOR,		0,0,0,MENU_NULL},
#endif
};

/*---------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                 */
/*---------------------------------------------------------------------------*/
#define MENU_DEF(_TABLEID, _ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, \
    _ITEMCOUNT) \
    _ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, _ITEMCOUNT, 

const GUIMENU_GROUP_T mmiim_menu_table[] = 
{
    {NULL},
    #include "mmiim_menutable.def"
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
// 	Description : register im menu group
//	Global resource dependence : none
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC void MMIIM_RegMenuGroup(void)
{
    MMI_RegMenuGroup(MMI_MODULE_IM, mmiim_menu_table);
}


