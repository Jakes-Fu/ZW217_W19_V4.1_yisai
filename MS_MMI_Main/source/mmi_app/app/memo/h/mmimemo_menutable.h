/*****************************************************************************
** File Name:      mmimemo_menutable.h                                    *
** Author:          fen.xie                                                    *
** Date:            05/08/2011                                                *
** Copyright:      2007 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe the browser menutable       *
*****************************************************************************/
#ifndef MMIMEMO_MENUTAB_H_
#define MMIMEMO_MENUTAB_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "mmi_module.h"

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/
typedef enum
{
    ID_MEMO_MENU_START = (MMI_MODULE_MEMO<< 16),
    ID_POPUP_MENU_NEW_MEMO,
    ID_POPUP_MENU_EDIT_MEMO,
    ID_POPUP_MENU_DEL_MEMO,
    ID_POPUP_MENU_MARK_MEMO,
    ID_POPUP_MENU_MEMO_MARKED,
    ID_POPUP_MENU_MEMO_UNMARKED,
    ID_POPUP_MENU_MEMO_MARKED_ALL,
    ID_POPUP_MENU_MEMO_UNMARKED_ALL,
    ID_MEMO_MENU_TOTAL
}ID_BRW_MENU_E;
#define MENU_DEF(_TABLEID, _ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, \
	_ITEMCOUNT) \
     _TABLEID,

typedef enum
{
	MMI_MEMO_BEGIN_MENU_START = (MMI_MODULE_MEMO<< 16),
	#include "mmimemo_menutable.def"
	MENUTABLE_MAX
} MMI_MEMO_MENUTAB_E;
#undef MENU_DEF

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : reg menu
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIMEMO_RegMenuGroup(void);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
