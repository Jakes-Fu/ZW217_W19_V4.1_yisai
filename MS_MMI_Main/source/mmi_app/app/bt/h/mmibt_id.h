/*****************************************************************************
** File Name:      mmibt_id.h                                                   *
** Author:         kelly.li                                                         *
** Date:           2006/08/21                                                *
** Copyright:      2006 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe fm win and control id       *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2006/08/21     kelly.li      Create                                    *
******************************************************************************/

#ifndef MMIBT_ID_H_
#define MMIBT_ID_H_ 

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
    BT_WIN_ID_START = (MMI_MODULE_BT << 16),

#include "mmibt_id.def"

    BT_MAX_WIN_ID
}BT_WINDOW_ID_E;

#undef WIN_ID_DEF

// control ID
typedef enum
{
    BT_CTRL_ID_ID_START = BT_MAX_WIN_ID,//(MMI_MODULE_BT << 16),
	MMIBT_MAIN_MENU_CTRL_ID,                                                       
	MMIBT_MAIN_LIST_CTRL_ID,  
	MMIBT_SHARE_SETTING_LIST_CTRL_ID,                                                     
	MMIBT_SET_VISIBILITY_CTRL_ID,                                         
	MMIBT_DEVICELIST_LIST_CTRL_ID,                                                                                                                                                         
	MMIBT_PIN_EDITBOX_CTRL_ID,                                                                                                                          
    MMIBT_INQUIRED_DEV_LIST_CTRL_ID,                                                                                                                                                                                                                                                              
	MMIBT_BT_SEND_FILE_LAB1_CTRL_ID,
	MMIBT_BT_SEND_FILE_LAB2_CTRL_ID,
	MMIBT_BT_SEND_FILE_LAB3_CTRL_ID,
	MMIBT_BT_SEND_FILE_TEXT_CTRL_ID,                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             
	MMIBT_EDIT_DEVICE_NAME_EDITBOX_CTRL_ID,                                                                                                              
	MMIBT_SET_SAVE_LOCATION_CTRL_ID,
	MMIBT_UI_RESULT_LABEL_CTRL_ID,
	MMIBT_DUN_QUERY_LABEL_CTRL_ID,
	MMIBT_LIST_INQUIRY_MENU_CTRL_ID,
    MMIBT_LIST_MENU_SHARED_ID,
    MMIBT_LIST_BOX_SHARED_AUTHORITY_ID,
    MMIBT_LIST_BOX_SHARED_DIRECTORY_ID,
    MMIBT_TEXT_BOX_DEVICE_DETAILS_CTRL_ID,  
	MMIBT_TEXT_BOX_NEW_FILE_DETAIL_CTRL_ID,
	MMIBT_ANIMATE_SEARCHING_CTRL_ID,
    MMIBT_DEVICE_DETAILS_MENU_CTRL_ID,
    MMIBT_NUMERIC_PASSKEY_CTRL_ID,
#ifdef BT_DIALER_SUPPORT
    MMIBT_BTPHONE_MAIN_LIST_CTRL_ID,
	MMIBT_BTPHONE_LIST_CTRL_ID,
	MMIBT_SETTING_LIST_CTRL_ID,
	MMIBT_BT_CAMERA_PANEL_LIST_CTRL_ID,
	MMIBT_HID_CAM_MAIN_LIST_CTRL_ID,
#endif
#ifdef ADULT_WATCH_SUPPORT
    MMIBT_ADULTWACHT_MAIN_LIST_CTRL_ID,
    MMIBT_ADULTWACHT_PAIR_LIST_CTRL_ID,
    MMIBT_ADULTWACHT_PIN_EDIT_ID,
    MMIBT_ADULTWACHT_PIN_CNF_BTN_ID,
    MMIBT_ADULTWACHT_PIN_CANCEL_BTN_ID,
#endif
    BT_MAX_CTRL_ID
}BT_CONTROL_ID_E;

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
// 	Description : Register acc menu group
//	Global resource dependence : none
//  Author: haiyang.hu
//	Note:
/*****************************************************************************/
PUBLIC void MMIBT_RegWinIdNameArr(void);


/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
