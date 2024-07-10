/*****************************************************************************
** File Name:      mmidm_id.h                                                *
** Author:         jian.ma                                                   *
** Date:           2009/07/02                                                *
** Copyright:      2009 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe fm win and control id       *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2009/07/02     jian.ma          Create                                    *
******************************************************************************/

#ifndef MMIDM_ID_H_
#define MMIDM_ID_H_ 

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
    DM_WIN_ID_START = (MMI_MODULE_DM << 16),

#include "mmidm_id.def"

    DM_MAX_WIN_ID
}DM_WINDOW_ID_E;

#undef WIN_ID_DEF

// control ID
typedef enum
{
    DM_CTRL_ID_ID_START = DM_MAX_WIN_ID,
    MMIDM_MAIN_MENU_MENU_CTRL_ID,
	MMIDM_SHOW_VERSION_CTRL_ID,  
	MMIDM_PRG_CTRL_ID,  
    MMIDM_PRG_TEXT_CTRL_ID, 
    MMIDM_DIAL_BUTTON_CTRL_ID,
    MMIDM_DEBUG_SETTING_MENU_CTRL_ID,
	MMIDM_DEBUG_EDIT_CTRL_ID,
    MMIDM_DEBUG_LISTBOX_CTRL_ID,
    MMIDM_DELAY_LISTBOX_CTRL_ID,
    MMIDM_DISPLAY_CTRL_ID,
    MMIDM_ALERT_CONFIRM_CTRL_ID,
    MMIDM_CHOICELIST_EDITBOX_CTRL_ID,
    MMIDM_SELECT_SIM_CTRL_ID,
    DM_MAX_CTRL_ID
}DM_CONTROL_ID_E;

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
// 	Description : Register dm menu group
//	Global resource dependence : none
//  Author: jian.ma
//	Note:
/*****************************************************************************/
PUBLIC void MMIDM_RegWinIdNameArr(void);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
