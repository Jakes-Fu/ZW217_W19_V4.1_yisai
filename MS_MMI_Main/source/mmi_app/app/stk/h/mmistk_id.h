/*****************************************************************************
** File Name:      mmistk_id.h                                               *
** Author:         songbin.zeng                                                *
** Date:           2006/10/08                                                *
** Copyright:      2006 Spreadtrum, Incorporated. All Rights Reserved.        *
** Description:    This file is used to describe stk win and control id      *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2006/10/08 songbin.zeng   Create                                   *
******************************************************************************/

#ifndef _MMISTK_ID_H_
#define _MMISTK_ID_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "os_api.h"
#include "mmk_type.h"
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

#define WIN_ID_DEF(win_id)          win_id
// window ID
typedef enum
{
    MMISTK_WIN_ID_START = (MMI_MODULE_STK << 16),

#include "mmistk_id.def"

    MMISTK_MAX_WIN_ID
}MMISTK_WINDOW_ID_E;

#undef WIN_ID_DEF

// control ID
typedef enum
{
    MMISTK_CTRL_ID_ID_START = MMISTK_MAX_WIN_ID, //(MMI_MODULE_STK << 16),
    MMISTK_MAINMENU_CTRL_ID,
    MMISTK_MAINMENU_ACTION_CTRL_ID,
    MMISTK_MENU_CTRL_ID,
    MMISTK_MENU_ACTION_CTRL_ID,
    MMISTK_EDITBOX_CTRL_ID,
    MMISTK_TEXBOX_CTRL_ID,
    MMISTK_MSGBOX_CTRL_ID,
    MMISTK_SELECT_ITEM_OPTION_MENU_CTRL_ID,
    MMISTK_TOP_MENU_OPTION_MENU_CTRL_ID,
    MMISTK_CALL_SIM__CTRL_ID,
#ifndef MMI_MULTI_SIM_SYS_SINGLE
    MMISTK_COM_SELECT_SIM_CTRL_ID,
#endif

    MMISTK_SELECT_ITEM_FORM_CTRL_ID,
    MMISTK_DISPLAY_TEXT_FORM_CTRL_ID,
    MMISTK_GET_INKEY_FORM_CTRL_ID,
    MMISTK_GET_INPUT_FORM_CTRL_ID,
    MMISTK_SEND_SMS_FORM_CTRL_ID,
    MMISTK_SETUP_CALL_FORM_CTRL_ID,
    MMISTK_PLAY_TONE_FORM_CTRL_ID,
    MMISTK_SEND_SS_FORM_CTRL_ID,
    MMISTK_SETUP_MENU_FORM_CTRL_ID,

    MMISTK_GET_INPUT_ANIM_CTRL_ID,
    MMISTK_GET_INKEY_ANIM_CTRL_ID,
    MMISTK_TOP_MENU_ANIM_CTRL_ID,
    MMISTK_SELECT_ITEM_ANIM_CTRL_ID,
    MMISTK_SETUP_CALL_ANIM_CTRL_ID,

    MMISTK_MAX_CTRL_ID
}MMISTK_CONTROL_ID_E;

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
// 	Description : Register acc menu group
//	Global resource dependence : none
//  Author: haiyang.hu
//	Note:
/*****************************************************************************/
PUBLIC void MMISTK_RegWinIdNameArr(void);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif //_MMISTK_ID_H_
