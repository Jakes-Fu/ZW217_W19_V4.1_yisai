/*****************************************************************************
** File Name:      mmisms_id.h                                               *
** Author:                                                                   *
** Date:           2006/10/20                                                *
** Copyright:      2006 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe SMS win and control id      *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2006/10/20     Tracy Zhang      Create                                    *
******************************************************************************/

#ifndef _MMISMS_ID_H_
#define _MMISMS_ID_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "os_api.h"
#include "mmk_type.h"
#include "mmi_module.h"
#ifdef MMI_SMS_CHAT_SUPPORT
#include "mmi_custom_define.h"
#include "mmi_signal_ext.h"
#include "mmisms_app.h"
#endif

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
    MMISMS_START_WIN_ID = (MMI_MODULE_SMS << 16),

#include "mmisms_id.def"

    MMISMS_END_WIN_ID
}MMISMS_WINDOW_ID_E;

#undef WIN_ID_DEF

// control ID
typedef enum
{
    MMISMS_CTRL_ID_START = MMISMS_END_WIN_ID, //(MMI_MODULE_SMS << 16),

    //watch
    MMISMS_MAINWIN_LIST_CTRL_ID,                 //WATCH_main
    MMISMS_PRESETWIN_LIST_CTRL_ID,           //WATCH_preset
    MMISMS_CONTACTWIN_LIST_CTRL_ID,        //WATCH contact  not use
    MMISMS_MESSAGEWIN_LIST_CTRL_ID,        //WATCH message  not use
    MMISMS_SHOW_CHAT_SMS_CHAT_FORM_CTRL_ID,              //WATCH_chat_form
    MMISMS_SHOW_CHAT_SMS_CHAT_BUTTON_CTRL_ID,          //WATCH_chat_button
    MMISMS_SHOW_CHAT_SMS_CHAT_BUTTONBG_CTRL_ID,     //WATCH_chat_button_bg
    MMISMS_SHOW_CHAT_SMS_EMOJI_BUTTON_CTRL_ID,          //WATCH_chat_button

    MMISMS_MENU_CTRL_ID,                    // the menu for SMS
    MMISMS_MAINMENU_OPT_MENU_CTRL_ID,       // the opt menu for sms main menu
#ifdef MMI_SMS_FONT_SETTING_SUPPORT
    MMISMS_MMS_SETTING_FONT_SIZE_CTRL_ID,   // the font size setting for sms_mms_in_1 
#endif
    MMISMS_SETTING_MENU_CTRL_ID,            // the menu for SMS
    MMISMS_SMS_SETTING_MENU_CTRL_ID,        // the menu for SMS
#ifdef MMI_SMS_VOICE_MAIL_SUPPORT    
    MMISMS_VOICEMAIL_SETTING_MENU_CTRL_ID,    // the menu for SMS
#endif    
    MMISMS_EDITWIN_EDITCTRL_ID,             // the editbox for edit-window
    MMISMS_NEW_MSG_MENU_CTRL_ID,            // the menu for new msg
    MMISMS_MSG_SPACE_MENU_CTRL_ID,

    MMISMS_MTBOX_OPT_MENU_CTRL_ID,          // the mt box list option menu
    MMISMS_SENDSUCCBOX_OPT_MENU_CTRL_ID,    // the sendsucc box list option menu
    MMISMS_SENDFAILBOX_OPT_MENU_CTRL_ID,    // the sendfail box list option menu
    MMISMS_DRAFTBOX_OPT_MENU_CTRL_ID,        // the draftbox list option menu
    MMISMS_SECURITYBOX_OPT_MENU_CTRL_ID,  
#if defined(BT_DIALER_SUPPORT) && defined(BT_MAP_SUPPORT)
    MMISMS_BT_MTBOX_OPT_MENU_CTRL_ID,          // the mt box list option menu
    MMISMS_BT_SENDSUCCBOX_OPT_MENU_CTRL_ID,    // the sendsucc box list option menu
    MMISMS_BT_SENDFAILBOX_OPT_MENU_CTRL_ID,    // the sendfail box list option menu
    MMISMS_BT_DRAFTBOX_OPT_MENU_CTRL_ID,        // the draftbox list option menu
    MMISMS_BT_SECURITYBOX_OPT_MENU_CTRL_ID, 
#endif
#ifdef MMI_PDA_SUPPORT
    MMISMS_MTBOX_MENUKEY_OPT_MENU_CTRL_ID,  
    MMISMS_CHAT_MENUKEY_OPT_MENU_CTRL_ID,  
    MMISMS_SECURITYLIST_MENUKEY_OPT_MENU_CTRL_ID,  
    MMISMS_MARK_MENU_OPT_MENU_CTRL_ID,  
    MMISMS_MARK_MENU_OPT_UNLOCK_MENU_CTRL_ID,  
    MMISMS_CHATMARK_MENU_OPT_MENU_CTRL_ID,
#endif
#ifdef MMI_SMS_CHAT_SUPPORT    
    MMISMS_CHATBOX_OPT_MENU_CTRL_ID,
#endif    
    MMISMS_MSGBOX_TAB_CTRL_ID,
    MMISMS_MSGDEL_ALL_CTRL_ID,
    MMISMS_BOXDEL_ALL_CHILD_CTRL_ID,
    MMISMS_MSGTYPEDEL_ALL_CHILD_CTRL_ID,

//    MMISMS_SMSBOX_LISTBOX_CTRL_ID,          // the listbox for SMS box
    MMISMS_WRITEOPTION_MENU_CTRL_ID,
    MMISMS_INPUTNUM_EDITBOX_CTRL_ID,
    MMISMS_SHOWMSG_TEXTBOX_CTRL_ID,            // the textbox ctrl in the showing msg window
    //MMISMS_MTBOXOPT_MENU_CTRL_ID,            // the menu ctrl in the mtbox option window
#ifdef DPHONE_SUPPORT 
    MMISMS_USERNUMWIN_LISTBOX_CTRL_ID,        // the listbox in the user number window
    MMISMS_EXTRACT_MENU_CTRL_ID,
#endif
    MMISMS_INPUTSCWIN_PHONENUM_CTRL_ID,        // the phonenum box in the input sc num win
#ifdef SMS_AUTO_SIGNATURE_SUPPORT
    MMISMS_INPUT_SIGNATURE_WIN_TEXT_CTRL_ID,        // the text box in the input auto signature win
#endif
    
    MMISMS_SELECTVPWIN_LISTBOX_CTRL_ID,        // the listbox ctrl in the selecting VP window
    MMISMS_SELECTSRWIN_LISTBOX_CTRL_ID,        //the listbox ctrl in the selecting SR window
    MMISMS_SELECTSAVEPOSWIN_LISTBOX_CTRL_ID,// the listbox ctrl in the selecting save postion win
    MMISMS_CONCATENATED_SMS_LISTBOX_CTRL_ID,// the listbox ctrl in the selecting concatenated sms win
    MMISMS_ALPHABET_MANAGEMENT_LISTBOX_CTRL_ID,// the listbox ctrl in the selecting alphabet management sms win
    MMISMS_PERFERRED_CONNECTION_LISTBOX_CTRL_ID,// the listbox ctrl in the selecting preferred connection win
    MMISMS_NUMOPTWIN_MENU_CTRL_ID,
    MMISMS_SEND_PROMPT_OPT_CTRL_ID,
    MMISMS_SENDSUCCOPT_MENU_CTRL_ID,
    MMISMS_MOBOXOPT_MENU_CTRL_ID,            // the menu for mobox window
#ifdef MMI_SMS_VOICE_MAIL_SUPPORT    
    MMISMS_INPUTVMWIN_PHONENUM_CTRL_ID,        // the phonenumber ctrl id for inputting voice mail window
    MMISMS_VM_EDIT_CTRL_ID,
    MMISMS_VM_FORM_CTRL_ID,
    MMISMS_VM_EDIT_WIN_CTRL_ID,
    MMISMS_VM_SHOW_EDIT_WIN_CTRL_ID,
#endif    
    MMISMS_WORDWIN_LISTBOX_CTRL_ID,
    MMISMS_WORDWIN_LISTBOX_FOR_CC_CTRL_ID,
    MMISMS_PHIZ_LISTBOX_CTRL_ID,
    MMISMS_WORDOPT_MENU_CTRL_ID,
    MMISMS_INPUTWORDWIN_TEXT_CTRL_ID,
    MMISMS_SELECTWORD_LIST_CTRL_ID,
    MMISMS_SHOWCLASS0_TEXTBOX_CTRL_ID,
    
    MMISMS_NOSENDBOX_LISTBOX_CTRL_ID,
    MMISMS_MTBOX_LISTBOX_CTRL_ID,
    MMISMS_SENDSUCCBOX_LISTBOX_CTRL_ID,
    MMISMS_SENDFAILBOX_LISTBOX_CTRL_ID,

#if defined(BT_DIALER_SUPPORT) && defined(BT_MAP_SUPPORT)
    MMISMS_BT_NOSENDBOX_LISTBOX_CTRL_ID,
    MMISMS_BT_MTBOX_LISTBOX_CTRL_ID,
    MMISMS_BT_SENDSUCCBOX_LISTBOX_CTRL_ID,
    MMISMS_BT_SENDFAILBOX_LISTBOX_CTRL_ID,
    MMISMS_BT_SELECT_SEND_FUNC_LIST_CTRL_ID,
#if defined (MMI_SMS_SECURITYBOX_SUPPORT)
    MMISMS_BT_SECURITYBOX_LISTBOX_CTRL_ID,
#endif    
#endif

    MMISMS_MTSMS_DETAIL_LABEL1_CTRL_ID,
    MMISMS_MTSMS_DETAIL_LABEL2_CTRL_ID,
    MMISMS_MTSMS_DETAIL_LABEL3_CTRL_ID,
    MMISMS_MTSMS_DETAIL_LABEL4_CTRL_ID,

    MMISMS_MESSAGE_CONTENT_TEXT_CTRL_ID,
    MMISMS_MTSMS_DETAIL_TEXT_CTRL_ID,
    
    //======================================================================================
    //小区广播使用控件   
    //======================================================================================
    MMISMS_CB_MAIN_MENU_CTRL_ID,
    MMISMS_CB_STARTUP_LISTBOX_CTRL_ID,                 //小区广播启动单选列表控件
    MMISMS_CB_SHOW_MSG_TEXTBOX_CTRL_ID,                //小区广播显示消息内容控件
    MMISMS_CB_CLASS0_SHOW_MSG_TEXTBOX_CTRL_ID,         //小区广播显示CLASS0级消息内容控件
    MMISMS_CB_LANGUAGE_LISTBOX_CTRL_ID,                //小区广播语言多选列表控件
    MMISMS_CB_CHANNEL_LISTBOX_CTRL_ID,                 //小区广播频道多选列表控件
    MMISMS_CB_CHANNEL_INDEX_LABEL_CTRL_ID,             //小区广播频道索引标签控件
    MMISMS_CB_CHANNEL_NAME_LABEL_CTRL_ID,              //小区广播频道名称标签控件
    MMISMS_CB_CHANNEL_INDEX_EDIT_CTRL_ID,              //小区广播频道索引编辑框控件
    MMISMS_CB_CHANNEL_NAME_EDIT_CTRL_ID,               //小区广播频道名称编辑框控件
    MMISMS_CB_CHANNEL_LIST_MENU_CTRL_ID,               //小区广播频道操作列表控件
    MMISMS_CB_CHANNEL_ADD_SOFTKEY_CTRL_ID,
    MMISMS_CB_CHANNEL_SHOW_INDEX_LABEL_CTRL_ID,
    MMISMS_CB_CHANNEL_SHOW_NAME_LABEL_CTRL_ID,
    MMISMS_CB_MSG_LISTBOX_CTRL_ID,                     //小区广播消息列表控件
    MMISMS_CB_ADD_EDIT_CHANNEL_CTRL_ID,
    MMISMS_CB_CHANNEL_INDEX_FORM_CTRL_ID,
    MMISMS_CB_CHANNEL_NAME_FORM_CTRL_ID,
    MMISMS_CB_CHANNEL_CHECK_CTRL_ID,
    MMISMS_CB_SETTING_CTRL_ID,
#ifdef MMI_ETWS_SUPPORT
    MMISMS_CB_ETWS_IDLE_RICHTEXT_CTRL_ID,
#endif
#ifdef MMI_SMS_VOICE_MAIL_SUPPORT    
    MMISMS_VMN_LABEL_CTRL_ID,
#endif    
#ifndef MMI_MULTI_SIM_SYS_SINGLE
    MMISMS_COM_SELECT_SIM_CTRL_ID,                  //for dual sys
#endif
#ifdef TTS_SUPPORT
    MMISMS_SET_TTS_CTRL_ID,
#endif
    MMISMS_EXTRACT_URLORST_CTRL_ID,
    MMISMS_CONNECT_URLORST_CTRL_ID,
    MMISMS_SHOW_MSG_RICHTEXT_CTRL_ID,
    MMISMS_URL_OPT_CTRL_ID,
    MMISMS_BROWSER_SETTING_CTRL_ID,
    MMISMS_SERVICE_LOADING_OPERATION_SETTING_CTRL_ID,
    MMISMS_SMS_PUSH_MESSAGES_CTRL_ID,
    MMISMS_COMMON_SETTING_MENU_CTRL_ID,
    MMISMS_EXTRACT_NUMBER_POP_CTRL_ID,
    MMISMS_EXTRACT_URL_POP_CTRL_ID,
    MMISMS_EXTRACT_EMAIL_POP_CTRL_ID,
    MMISMS_CLASS0_OPTION_POP_CTRL_ID,
    MMISMS_STATUSREPORT_CTRL_ID,
    MMISMS_SECURITYBOX_LISTBOX_CTRL_ID,
    MMISMS_SET_SECURITY_PWD_CTRL_ID,
    MMISMS_SET_SECURITY_NEW_PWD_CTRL_ID,
    MMISMS_SET_SECURITY_NEW_PWD_CNF_CTRL_ID,    
    MMISMS_SEARCH_LISTBOX_CTRL_ID,
    MMISMS_SEARCH_LIST_OPT_MENU_CTRL_ID,
    MMISMS_SETTING_EXPORT_CTRL_ID,
    MMISMS_SETTING_EXPORT_FORM_CTRL_ID,
    MMISMS_SELECTEXPORTWIN_CTRL_ID,
    MMISMS_SETTING_EXPORT_DROPDOWNLIST_CTRL_ID,
    MMISMS_SETTING_EXPORT_LOC_CTRL_ID,
    MMISMS_SETTING_EXPORT_LOC_FORM1_CTRL_ID,
    MMISMS_SETTING_EXPORT_LOC_FORM2_CTRL_ID,
    MMISMS_SETTING_EXPORT_SELLOC_CTRL_ID,
    MMISMS_SETTING_EXPORT_SAVEPOS_CTRL_ID,
    MMISMS_SETTING_EXPORT_LOC_DROPDOWNLIST_CTRL_ID,

    MMISMS_EDIT_SMS_NUM_CTRL_ID,
    MMISMS_EDIT_SMS_SENDTO_CTRL_ID,
    MMISMS_EDIT_SMS_PHONENUM_CTRL_ID,
    MMISMS_EDIT_SMS_TEXT_CTRL_ID,
    MMISMS_CB_SETTING_TAB_CTRL_ID,
    MMISMS_CB_CHILD_SET_CARD1_CTRL_ID,
    MMISMS_CB_CHILD_SET_CARD2_CTRL_ID,
    MMISMS_CB_CHILD_SET_CARD3_CTRL_ID,
    MMISMS_CB_CHILD_SET_CARD4_CTRL_ID,

#ifdef MMI_ETWS_SUPPORT
    MMISMS_CB_EXTRACT_NUMBER_POP_CTRL_ID,
#endif

#ifdef TOUCH_PANEL_SUPPORT
    MMISMS_TOOLBAR_MTBOX_CTRL_ID,
    MMISMS_TOOLBAR_SMSEDIT_CTRL_ID,
#endif

#ifdef SMS_AUTO_SIGNATURE_SUPPORT
    MMISMS_SMS_SINATURE_CTRL_ID,
    MMISMS_SMS_DISPLAY_SINATURE_CTRL_ID,
#endif

#ifdef MMI_SMS_CHAT_SUPPORT
    MMISMS_CHATBOX_LISTBOX_CTRL_ID,
#if defined(BT_DIALER_SUPPORT) && defined(BT_MAP_SUPPORT)
    MMISMS_BT_CHATBOX_LISTBOX_CTRL_ID,
#endif
    MMISMS_SHOW_CHAT_SMS_FORM_CTRL_ID,   
    MMISMS_SHOW_CHAT_SMS_EDITCTRL_ID,
    MMISMS_SHOW_CHAT_SMS_EDITCTRL_FORM_ID,
    MMISMS_SHOW_CHAT_SMS_PARENT_ITEM_FORM_CTRL_ID,
    MMISMS_MAX_SHOW_CHAT_SMS_ITEM_FATHER_FORM_CTRL_ID = MMISMS_SHOW_CHAT_SMS_PARENT_ITEM_FORM_CTRL_ID + MMISMS_CHAT_MAX_ITEM,    
    MMISMS_SHOW_CHAT_SMS_SUBPARENT_ITEM_FORM_CTRL_ID,
    MMISMS_MAX_SHOW_CHAT_SMS_ITEM_SUBFATHER_FORM_CTRL_ID = MMISMS_SHOW_CHAT_SMS_SUBPARENT_ITEM_FORM_CTRL_ID + MMISMS_CHAT_MAX_ITEM,    
    MMISMS_SHOW_CHAT_SMS_LABEL_ITEM_FORM_CTRL_ID,
    MMISMS_MAX_SHOW_CHAT_SMS_ITEM_LABEL_FORM_CTRL_ID = MMISMS_SHOW_CHAT_SMS_LABEL_ITEM_FORM_CTRL_ID + MMISMS_CHAT_MAX_ITEM,    
    MMISMS_SHOW_CHAT_SMS_SUBANIM_ITEM_FORM_CTRL_ID,
    MMISMS_MAX_SHOW_CHAT_SMS_ITEM_SUBANIM_FORM_CTRL_ID = MMISMS_SHOW_CHAT_SMS_SUBANIM_ITEM_FORM_CTRL_ID + MMISMS_CHAT_MAX_ITEM,    
    MMISMS_SHOW_CHAT_SMS_ITEM_FORM_CTRL_ID,
    MMISMS_MAX_SHOW_CHAT_SMS_ITEM_FORM_CTRL_ID = MMISMS_SHOW_CHAT_SMS_ITEM_FORM_CTRL_ID + MMISMS_CHAT_MAX_ITEM,    

    MMISMS_SHOW_CHAT_SMS_DATE_ITEM_FORM_CTRL_ID,
    MMISMS_MAX_SHOW_CHAT_SMS_DATE_ITEM_FORM_CTRL_ID = MMISMS_SHOW_CHAT_SMS_DATE_ITEM_FORM_CTRL_ID + MMISMS_CHAT_MAX_ITEM,    

    MMISMS_SHOW_CHAT_SMS_SIM_ITEM_FORM_CTRL_ID,
    MMISMS_MAX_SHOW_CHAT_SIM_ITEM_FORM_CTRL_ID = MMISMS_SHOW_CHAT_SMS_SIM_ITEM_FORM_CTRL_ID + MMISMS_CHAT_MAX_ITEM,    
    
    MMISMS_SHOW_CHAT_SMS_DATE_AND_SIM_ITEM_FORM_CTRL_ID,
    MMISMS_MAX_SHOW_CHAT_SMS_DATE_AND_SIM_ITEM_FORM_CTRL_ID = MMISMS_SHOW_CHAT_SMS_DATE_AND_SIM_ITEM_FORM_CTRL_ID + MMISMS_CHAT_MAX_ITEM,    
    
    MMISMS_SHOW_CHAT_SMS_DATE_AND_TEXT_FORM_CTRL_ID,
    MMISMS_MAX_SHOW_CHAT_SMS_DATE_AND_TEXT_ITEM_FORM_CTRL_ID = MMISMS_SHOW_CHAT_SMS_DATE_AND_TEXT_FORM_CTRL_ID + MMISMS_CHAT_MAX_ITEM,    

    MMISMS_SHOW_CHAT_SMS_OPT_CTRL_ID,
    #ifdef MMI_PDA_SUPPORT
    MMISMS_SHOW_CHAT_ITEM_OPT_CTRL_ID,
    #endif
    #ifdef MMIMMS_SMS_IN_1_SUPPORT
    MMISMS_CHAT_INSERT_OPT_CTRL_ID,
    #endif
#endif    
#ifdef MMI_TIMERMSG_SUPPORT
    MMISMS_TIMER_SMS_CTRL_ID,
    MMISMS_EDIT_TIMER_CTRL_ID,
    MMISMS_EDIT_TIMER_FREQDATE_CTRL_ID,
    MMISMS_TIMERMSG_EDIT_WIN_FORM_CTRL_ID,
    MMISMS_TIMERMSG_DATE_FORM_CTRL_ID,
    MMISMS_TIMERMSG_DATE_TITLE_LABEL_CTRL_ID,
    MMISMS_TIMERMSG_DATE_EDIT_CTRL_ID,
    MMISMS_TIMERMSG_TIME_FORM_CTRL_ID,
    MMISMS_TIMERMSG_TIME_TITLE_LABEL_CTRL_ID,
    MMISMS_TIMERMSG_TIME_EDIT_CTRL_ID,
    MMISMS_TIMERMSG_FREQ_FORM_CTRL_ID,
    MMISMS_TIMERMSG_FREQ_TITLE_LABEL_CTRL_ID,
    MMISMS_TIMERMSG_FREQ_SETLIST_CTRL_ID,
    MMISMS_TIMERMSG_EDITWIN_CHILD_FORM_CTRL_ID,
    MMISMS_TIMERMSG_EDITWIN_BUTTON0_CTRL_ID,
    MMISMS_TIMERMSG_EDITWIN_BUTTON1_CTRL_ID,
    MMISMS_TIMERMSG_EDITWIN_BUTTON2_CTRL_ID,
    MMISMS_TIMERMSG_EDITWIN_BUTTON3_CTRL_ID,
    MMISMS_TIMERMSG_EDITWIN_BUTTON4_CTRL_ID,
    MMISMS_TIMERMSG_EDITWIN_BUTTON5_CTRL_ID,
    MMISMS_TIMERMSG_EDITWIN_BUTTON6_CTRL_ID,
    MMISMS_TIMERMSG_DEADLINE_FORM_CTRL_ID,
    MMISMS_TIMERMSG_DEADLINE_TITLE_LABEL_CTRL_ID,
    MMISMS_TIMERMSG_DEADLINE_EDIT_CTRL_ID,
    #ifdef MMI_PDA_SUPPORT
    MMISMS_TIMERMSG_DEADLINE_EDIT_FORM_CTRL_ID,
    MMISMS_TIMERMSG_DEADLINE_DEADLINEEDITBOX_CTRL_ID,    
    MMISMS_TIMERMSG_DATE_EDIT_FORM_CTRL_ID,
    MMISMS_TIMERMSG_DATE_DATEEDITBOX_CTRL_ID,
    MMISMS_TIMERMSG_TIME_EDIT_FORM_CTRL_ID,
    MMISMS_TIMERMSG_TIME_TIMEEDITBOX_CTRL_ID,
    #endif
#endif	

#ifdef MMI_PDA_SUPPORT
    MMISMS_MTBOX_BUTTON_FORM_CTRL_ID,
    MMISMS_MTBOX_BUTTON_FORM_READ_CTRL_ID,
    MMISMS_MTBOX_BUTTON_FORM_SMS_REPLY_CTRL_ID,
    MMISMS_MTBOX_BUTTON_FORM_FORWARD_CTRL_ID,
    MMISMS_MTBOX_BUTTON_FORM_DELETE_CTRL_ID,

    MMISMS_CLASS0_BUTTON_FORM_CTRL_ID,
    MMISMS_CLASS0_BUTTON_FORM_CALL_CTRL_ID,
    MMISMS_CLASS0_BUTTON_FORM_SAVE_CTRL_ID,

    MMISMS_MTBOX_CHAT_FORM_CTRL_ID,
    MMISMS_MTBOX_EDIT_SEND_FORM_CTRL_ID,
    MMISMS_MTBOX_EDIT_CTRL_ID,
    MMISMS_MTBOX_SEND_CTRL_ID,   

    MMISMS_SMS_SETTING_LIST_CTRL_ID,   
    MMISMS_SMS_SETTING_EXPORT_SETLIST_CTRL_ID,   

    MMISMS_READ_FORM_CTRL_ID,   
    MMISMS_READ_FORWARD_CTRL_ID,
    MMISMS_READ_DELETE_CTRL_ID,   

    MMISMS_TEMPLATE_INPUT_FORM_CTRL_ID,
    MMISMS_MARK_LISTBOX_CTRL_ID,
    
    MMISMS_MTBOX_FORM_CTRL_ID,
    MMISMS_SENDFAILBOX_FORM_CTRL_ID,
    MMISMS_SENDSUCCBOX_FORM_CTRL_ID,
    MMISMS_DRAFTBOX_FORM_CTRL_ID,
    MMISMS_SECURITYBOX_FORM_CTRL_ID,
#if defined(BT_DIALER_SUPPORT) && defined(BT_MAP_SUPPORT)
    MMISMS_BT_MTBOX_FORM_CTRL_ID,
    MMISMS_BT_SENDFAILBOX_FORM_CTRL_ID,
    MMISMS_BT_SENDSUCCBOX_FORM_CTRL_ID,
    MMISMS_BT_DRAFTBOX_FORM_CTRL_ID,
    MMISMS_BT_SECURITYBOX_FORM_CTRL_ID,
#endif

#endif

#ifdef TTS_SUPPORT
    MMISMS_SMS_SETTING_TTSLIST_CTRL_ID,   
#endif    
    MMISMS_SMS_SETTING_EXPORTLIST_CTRL_ID,   
    MMISMS_SETTINGS_TAB_CTRL_ID,
    MMISMS_CB_LISTOPT_CTRL_ID,
    MMISMS_CB_OPT_CTRL_ID,
    MMISMS_CHATBOX_FORM_CTRL_ID,

#if defined(BT_DIALER_SUPPORT) && defined(BT_MAP_SUPPORT)
    MMISMS_BT_CHATBOX_FORM_CTRL_ID,
#endif

#ifdef MMI_SMS_REPLY_PATH_SUPPORT
    MMISMS_SETTING_STATUS_CTRL_ID,
#endif

#ifdef MMI_SMS_MULTI_PARAM_SUPPORT
    MMISMS_SELECTSIMMODE_LISTBOX_CTRL_ID,
    MMISMS_SELECTSIMMODE_LISTBOX_LONGOK_CTRL_ID,
    MMISMS_SETTING_SIMMODE_MENU_CTRL_ID,
    MMISMS_SETTING_SIMMODE_NAME_CTRL_ID,
    MMISMS_SETTING_SIMMODE_SENDMODE_CTRL_ID,
#endif

#ifdef MMISMS_CHATMODE_ONLY_SUPPORT
    MMISMS_CHATBOX_WRITEMSG_BUTTON_CTRL_ID,
    MMISMS_PUSH_MESSAGE_FORM_CTRL_ID,
    MMISMS_PUSHMSG_LISTBOX_CTRL_ID,
    MMISMS_PUSHMSG_MENUKEY_OPT_MENU_CTRL_ID,
    MMISMS_PUSHMSG_ITEMS_OPT_CTRL_ID,
#endif
    MMISMS_SMS_SETTING_FIXSIMLIST_CTRL_ID,
    MMISMS_SIMBOX_LISTBOX_CTRL_ID,
    MMISMS_MAX_CTRL_ID
}MMISMS_CONTROL_ID_E;

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//     Description : Register acc menu group
//    Global resource dependence : none
//  Author: haiyang.hu
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_RegWinIdNameArr(void);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif //_MMISMS_ID_H_
