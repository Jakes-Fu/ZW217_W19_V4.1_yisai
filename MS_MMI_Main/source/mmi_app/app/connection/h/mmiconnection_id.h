/*****************************************************************************
** File Name:      mmiconnection_id.h                                        *
** Author:         ryan.xu                                                   *
** Date:           2007/01/31                                                *
** Copyright:      2006 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe connection manager window   *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2007/01/31     ryan.xu          Create                                    *
******************************************************************************/

#ifndef _MMICONNECTION_ID_H_
#define _MMICONNECTION_ID_H_ 

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
    MMICONNECTION_WIN_ID_START = (MMI_MODULE_CONNECTION << 16),

#include "mmiconnection_id.def"

    MMICONNECTION_MAX_WIN_ID
}MMICONNECTION_WINDOW_ID_E;

#undef WIN_ID_DEF

// control ID
typedef enum
{
    MMICONNECTION_CTRL_ID_ID_START = MMICONNECTION_MAX_WIN_ID, //(MMI_MODULE_CONNECTION << 16),
    MMICONNECTION_MENU_CTRL_ID, 
    MMICONNECTION_SET_TV_OUT_CTRL_ID,
    MMICONNECTION_LIST_SETTING_CTRL_ID,
    MMICONNECTION_MENU_SETTINGLINKLIST_CTRL_ID,
    MMICONNECTION_LIST_SEARCHLINKLIST_CTRL_ID,
    MMICONNECTION_LIST_SETTINGADDLIST_CTRL_ID,
    MMICONNECTION_MENU_SETTINGLINK_CTRL_ID,
    MMICONNECTION_RICHTEXT_SETTINGLINKDETAILVIEW_CTRL_ID,

    MMICONNECTION_LIST_SETTINGSECURITYCONNECT_CTRL_ID,
#ifdef IPVERSION_SUPPORT_V4_V6
    MMICONNECTION_LIST_SETTINGIPTYPE_CTRL_ID,
#endif
    MMICONNECTION_EDIT_INPUTUSERNAME_CTRL_ID,
    MMICONNECTION_EDIT_INPUTPASSWORD_CTRL_ID,
    MMICONNECTION_EDIT_INPUTAPN_CTRL_ID,
    MMICONNECTION_EDIT_INPUTGATEWAY_CTRL_ID,
    MMICONNECTION_EDIT_INPUTPORT_CTRL_ID,
    MMICONNECTION_EDIT_INPUTAGENTNAME_CTRL_ID,
    MMICONNECTION_EDIT_INPUTHOMEPAGE_CTRL_ID,
    MMICONNECTION_MENU_SETTINGLINK_ADVANCE_CTRL_ID,
    MMICONNECTION_GPRS_SETTINGS_CTRL_ID,
    MMICONNECTION_EDIT_INPUT_DNS_CTRL_ID,
    MMICONNECTION_LIST_SELECT_NETTYPE_CTRL_ID,
    
    MMICONNECTION_MENU_GPRS_CTRL_ID,	
    MMICONNECTION_GPRSSWITCH_LIST_CTRL_ID,
    MMICONNECTION_COM_SELECT_SIM_CTRL_ID,
#ifndef MMI_GUI_STYLE_TYPICAL
    MMICONNECTION_MENU_SETTINGLINK_LONGOK_CTRL_ID,
#endif /* MMI_GUI_STYLE_TYPICAL  */

#ifdef MMI_GPRS_SUPPORT
#ifdef MMI_PDA_SUPPORT
    MMICONNECTION_LINKLIST_CTRL_ID,
#endif
#endif
#ifdef MMI_GPRS_DATA_SIM_SET
    MMICONNECTION_GPRS_MODE_CTRL_ID,
#endif

    MMICONNECTION_MAX_CTRL_ID
}MMICONNECTION_CONTROL_ID_E;

typedef enum
{
		MMICONNECTION_NV_ITEM_START = (MMI_MODULE_CONNECTION << 16),
        MMICONNECTION_SET_TV_OUT_MODE,

        MMICONNECTION_MAX_NV_ITEM,
}MMICONNECTION_NV_ITEM_E;

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : Register acc menu group
//	Global resource dependence : none
//  Author: haiyang.hu
//	Note:
/*****************************************************************************/
PUBLIC void MMICONNECTION_RegWinIdNameArr(void);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif //_MMICONNECTION_ID_H_
