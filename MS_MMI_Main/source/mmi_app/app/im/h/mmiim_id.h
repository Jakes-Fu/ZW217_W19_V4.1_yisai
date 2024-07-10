/*****************************************************************************
** File Name:      mmiim_id.h                                                *
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

#ifndef _MMIPICVIEW_ID_H_
#define _MMIPICVIEW_ID_H_ 

#include "sci_types.h"
#include "os_api.h"
#include "mmk_type.h"
#include "mmi_module.h"


/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/

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
#define WIN_ID_DEF(win_id)          win_id
//window id
typedef enum
{
    MMIIM_WIN_ID_START = (MMI_MODULE_IM << 16),

#include "mmiim_id.def"

    MMIIM_MAX_WIN_ID
} MMIIM_WINDOW_ID_E;
#undef WIN_ID_DEF

//control id
typedef enum
{
    MMIIM_SET_MENU_CTRL_ID = MMIIM_MAX_WIN_ID,
	MMIIM_SET_PY_FUZZY_LIST_CTRL_ID,
	MMIIM_SET_HW_SPEED_LIST_CTRL_ID,
	MMIIM_SET_HW_THICKNESS_LIST_CTRL_ID,
	MMIIM_SET_HW_COLOR_PREVIEW_LABEL_CTRL_ID,
	MMIIM_SET_HW_COLOR_TEXT_LABEL_CTRL_ID,
	MMIIM_SET_HW_COLOR_BUTTON1_CTRL_ID,
	MMIIM_SET_HW_COLOR_BUTTON2_CTRL_ID,
	MMIIM_SET_HW_COLOR_BUTTON3_CTRL_ID,
	MMIIM_SET_HW_COLOR_BUTTON4_CTRL_ID,
	MMIIM_SET_HW_COLOR_BUTTON5_CTRL_ID,
	MMIIM_SET_HW_COLOR_BUTTON6_CTRL_ID,
	MMIIM_SET_HW_COLOR_BUTTON7_CTRL_ID,
	MMIIM_SET_HW_COLOR_BUTTON8_CTRL_ID,

	//如果改完popmenu则请删除下面两行
	MMIIM_SET_INPUTMETHOD_CTRL_ID,
	MMIIM_SET_INPUT_LANGUAGE_CTRL_ID,

	//采用popmenu的切换方式
	MMIIM_SET_INPUTMETHOD_MENU_CTRL_ID,
	MMIIM_SET_INPUT_LANGUAGE_MENU_CTRL_ID,

    MMIIM_SET_FORM_CTRL_ID,
    MMIIM_SET_PY_FUZZY_FORM_CTRL_ID,
    MMIIM_SET_HANDWRITE_SPEED_FORM_CTRL_ID,
    MMIIM_SET_HANDWRITE_THICK_FORM_CTRL_ID,
    MMIIM_SET_HANDWRITE_COLOR_FORM_CTRL_ID,
    MMIIM_SET_KEYBOARD_SIZE_FORM_CTRL_ID,
#if defined IM_ENGINE_SOGOU    
    MMIIM_SET_SOGOU_SETTING_FORM_CTRL_ID,
#endif    
#if defined MMI_IM_PDA_SUPPORT
    MMIIM_SET_INPUT_LANG_FORM_CTRL_ID,
#endif
    MMIIM_SET_PY_FUZZY_LABEL_CTRL_ID,
    MMIIM_SET_HANDWRITE_SPEED_LABEL_CTRL_ID,
    MMIIM_SET_HANDWRITE_SPEED_SETLIST_CTRL_ID,
    MMIIM_SET_HANDWRITE_THICK_LABEL_CTRL_ID,
    MMIIM_SET_KEYBOARD_SIZE_LABEL_CTRL_ID,
    MMIIM_SET_HANDWRITE_THICK_SETLIST_CTRL_ID,
    MMIIM_SET_HANDWRITE_COLOR_LABEL_CTRL_ID,
    MMIIM_SET_HANDWRITE_COLOR_SETTING_LABEL_CTRL_ID,
    MMIIM_SET_KEYBOARD_SIZE_SETLIST_CTRL_ID,
#if defined IM_ENGINE_SOGOU      
    MMIIM_SET_SOGOU_SETTING_LABEL_CTRL_ID,
#endif
#if defined MMI_IM_PDA_SUPPORT
    MMIIM_SET_INPUT_LANG_LABEL_CTRL_ID,
    MMIIM_SET_INPUT_LANG_CONTENT_LABEL_CTRL_ID,
#endif
    //touch
    MMIIM_TOUCH_SETTING_MENU_ID,
    MMIIM_PINYIN_FUZZY_WIN_FORM_ID,
    MMIIM_SOFTKEY_CTRL_ID, 
    MMIIM_ADD_WORD_EDITBOX_CTRL_ID,


    MMIIM_MAX_CTRL_ID
} MMIIM_CONTROL_ID_E;

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : Register im menu group
//  Global resource dependence : none
//  Author: jassmine.meng
//  Note:
/*****************************************************************************/
PUBLIC void MMIIM_RegWinIdNameArr(void);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
