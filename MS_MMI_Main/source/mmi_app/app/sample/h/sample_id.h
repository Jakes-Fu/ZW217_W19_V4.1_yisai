/*****************************************************************************
** File Name:      mmiacc_id.h                                               *
** Author:         xiaoqing.lu                                               *
** Date:           2009/04/03                                                *
** Copyright:      2006 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe ACC win and control id      *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2009/04/03     xiaoqing.lu       Create                                   *
******************************************************************************/

#ifndef _MMIHELLO_ID_H_
#define _MMIHELLO_ID_H_ 

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
    MMIHELLO_WIN_ID_START = (MMI_MODULE_SAMPLE << 16),
        
    MMI_HELLO_SPRD_WIN_ID,
    MMI_TEST_MENU_WIN_ID,
    MMI_TEST_LIST_WIN_ID,
    MMI_TEST_TEXTBOX_WIN_ID,
    MMI_TEST_BUTTON_WIN_ID,

    MMI_TEST_TAB_WIN_ID,
    MMI_TEST_TAB1_CHILD_WIN_ID,
    MMI_TEST_TAB2_CHILD_WIN_ID,

    MMI_TEST_EDITBOX_MAIN_WIN_ID,
    MMI_TEST_TEXT_EDITBOX_WIN_ID,
    MMI_TEST_PHONENUM_EDITBOX_WIN_ID,
    MMI_TEST_NUMLIST_EDITBOX_WIN_ID,
    MMI_TEST_DIGITAL_EDITBOX_WIN_ID,
    MMI_TEST_PASSWORD_EDITBOX_WIN_ID,
    MMI_TEST_DATE_EDITBOX_WIN_ID,
    MMI_TEST_TIME_EDITBOX_WIN_ID,
    MMI_TEST_IPADDRESS_EDITBOX_WIN_ID,

    MMI_TEST_DROPDOWNLIST_WIN_ID,

    MMI_TEST_RICHTEXT_WIN_ID,

    MMI_TEST_ANIM_WIN_ID,
    MMI_TEST_ICONLIST_WIN_ID,

    MMI_TEST_PUPMENU_WIN_ID,
    MMI_TEST_FORM_WIN_ID,
    MMI_TEST_SETLIST_WIN_ID,
    MMI_TEST_CHILDLIST_WIN_ID,

    MMIHELLO_MAX_WIN_ID
}MMIHELLO_WINDOW_ID_E;

#undef WIN_ID_DEF

// control ID
typedef enum
{
    MMIHELLO_CTRL_ID_ID_START = MMIHELLO_MAX_WIN_ID,
    
    MMI_TEST_CTRL_1_ID, // 可用于任何一个控件测试
    MMI_TEST_CTRL_2_ID, // 可用于任何一个控件测试
    MMI_TEST_CTRL_3_ID, // 可用于任何一个控件测试
    MMI_TEST_CTRL_4_ID, // 可用于任何一个控件测试
    
    MMI_TEST_MENU_CTRL_ID,
    MMI_TEST_LIST_CTRL_ID,
    MMI_TEST_TEXTBOX_CTRL_ID,
    MMI_TEST_BUTTON1_CTRL_ID,
    MMI_TEST_BUTTON2_CTRL_ID,
    MMI_TEST_BUTTON_LABEL1_CTRL_ID,
    MMI_TEST_BUTTON_LABEL2_CTRL_ID,
    // tab
    MMI_TEST_TAB_CTRL_ID,


    MMI_TEST_EDITBOX_LIST_CTRL_ID,
    MMI_TEST_TEXT_EDITBOX_CTRL_ID,
    MMI_TEST_PHONENUM_EDITBOX_CTRL_ID,
    MMI_TEST_NUMLIST_EDITBOX_CTRL_ID,
    MMI_TEST_DIGITAL_EDITBOX_CTRL_ID,
    MMI_TEST_PASSWORD_EDITBOX_CTRL_ID,
    MMI_TEST_DATE_EDITBOX_CTRL_ID,
    MMI_TEST_TIME_EDITBOX_CTRL_ID,
    MMI_TEST_IPADDRESS_EDITBOX_CTRL_ID,

    MMI_TEST_DROPDOWNLIST_CTRL_ID,

    MMI_TEST_RICHTEXT_CTRL_ID,

    MMI_TEST_ANIM_CTRL_ID,
    MMI_TEST_ICONLIST_CTRL_ID,

    MMI_TEST_PUPMENU_CTRL_ID,
    
    MMI_TEST_FORM_CTRL_ID,
    MMI_TEST_FORM_CTRL1_ID,
    MMI_TEST_FORM_CTRL2_ID,
    MMI_TEST_FORM_CTRL3_ID,
    MMI_TEST_FORM_ANIM_CTRL_ID,
    MMI_TEST_FORM_EDIT_TEXT_CTRL_ID,
    MMI_TEST_FORM_EDIT_TEXT_CTRL1_ID,
    MMI_TEST_FORM_EDIT_TEXT_CTRL2_ID,
    MMI_TEST_FORM_ANIM1_CTRL_ID,
    MMI_TEST_FORM_ANIM2_CTRL_ID,
    MMI_TEST_FORM_ANIM3_CTRL_ID,
    MMI_TEST_FORM_TEXT1_CTRL_ID,
    MMI_TEST_FORM_TEXT2_CTRL_ID,
    MMI_TEST_FORM_TEXT3_CTRL_ID,
    MMI_TEST_FORM_LABEL1_CTRL_ID,
    MMI_TEST_FORM_LABEL2_CTRL_ID,
    MMI_TEST_FORM_LABEL3_CTRL_ID,
    MMI_TEST_FORM_LABEL4_CTRL_ID,

    MMI_TEST_FORM_LABEL5_CTRL_ID,
    MMI_TEST_FORM_LABEL6_CTRL_ID,
    MMI_TEST_FORM_LABEL7_CTRL_ID,
    MMI_TEST_FORM_LABEL8_CTRL_ID,
    MMI_TEST_FORM_LABEL9_CTRL_ID,
    MMI_TEST_FORM_LABEL10_CTRL_ID,

    MMI_TEST_FORM_BUTTON1_CTRL_ID,
    MMI_TEST_FORM_BUTTON2_CTRL_ID,
    MMI_TEST_SETLIST_CTRL_ID,

    MMIHELLO_MAX_CTRL_ID
}MMIHELLO_CONTROL_ID_E;

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif //_MMIHELLO_ID_H_
