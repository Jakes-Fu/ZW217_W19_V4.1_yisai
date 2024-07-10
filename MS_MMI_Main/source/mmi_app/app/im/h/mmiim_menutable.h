/*****************************************************************************
** File Name:      mmiim_menutable.h                                         *
** Author:                                                                   *
** Date:           24/03/2006                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to mmimp3_internal func                 *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 11/2008       Jassmine           Create
******************************************************************************/

#ifndef _MMIIM_MENUTABLE_H_
#define _MMIIM_MENUTABLE_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
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
        
/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/
//menu id
typedef enum
{
    ID_MMIIM_MENU_START = (MMI_MODULE_IM << 16),

    ID_MMIIM_PY_FUZZY,
#ifdef TOUCH_PANEL_SUPPORT
    ID_MMIIM_HANDWRITING_SPEED,
    ID_MMIIM_HANDWRITING_THICKNESS,
    ID_MMIIM_HANDWRITING_COLOR,
#endif

	ID_MMIIM_INPUTMETHOD_MENU_0,
	ID_MMIIM_INPUTMETHOD_MENU_1,
	ID_MMIIM_INPUTMETHOD_MENU_2,
	ID_MMIIM_INPUTMETHOD_MENU_3,
	ID_MMIIM_INPUTMETHOD_MENU_4,
	ID_MMIIM_INPUTMETHOD_MENU_5,
	ID_MMIIM_INPUTMETHOD_MENU_6,
	ID_MMIIM_INPUTMETHOD_MENU_7,
	ID_MMIIM_INPUTMETHOD_MENU_8,
	ID_MMIIM_INPUTMETHOD_MENU_9,
	ID_MMIIM_INPUTMETHOD_MENU_10,
	ID_MMIIM_INPUTMETHOD_MENU_11,
        
	ID_MMIIM_MENU_MAX
} MMIPICVIEW_MENU_ID_E;

#define MENU_DEF(_TABLEID, _ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, \
    _ITEMCOUNT) \
     _TABLEID,
typedef enum
{
    MMIIM_MENU_START = (MMI_MODULE_IM << 16),
    #include "mmiim_menutable.def"
    MMIIM_MENU_MAX
} MMIIM_MENU_LABEL_E;
#undef MENU_DEF

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : register im menu group
//	Global resource dependence : none
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC void MMIIM_RegMenuGroup(void);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
