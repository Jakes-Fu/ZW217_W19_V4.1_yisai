/****************************************************************************
** File Name:      mmimms_id.h                                             *
** Author:         aoke.hu                                                 *
** Date:           04/13/2009                                              *
** Copyright:      2009 Spreadtrum, Incorporated. All Rights Reserved.     *
** Description:                                                            *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 04/2009        aoke.hu          create
** 
****************************************************************************/

#ifndef _MMIMMS_ID_H
#define _MMIMMS_ID_H

/**---------------------------------------------------------------------------*
 **                         Include Files                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "mmk_type.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern  "C"
    {
#endif

/**---------------------------------------------------------------------------*
 **                         Macro Declaration    							  *
 **---------------------------------------------------------------------------*/

        
/**---------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                 *
 **---------------------------------------------------------------------------*/
#define WIN_ID_DEF(win_id)          win_id
// window ID
// 命名规则: 模块名 + 窗口名 + WIN_ID  ,  eg: MMICC_DIALING_WIN_ID (CC部分呼叫窗口ID)
typedef enum
{
    MMIMMS_WIN_ID_START = (MMI_MODULE_MMS << 16),
    #include "mmimms_id.def"
    MMIMMS_MAX_WIN_ID
}MMIMMS_WINDOW_ID_E;

#undef WIN_ID_DEF

// control ID
typedef enum
{
    MMIMMS_CTRL_ID_START = MMIMMS_MAX_WIN_ID,

    MMIMMS_EDIT_MENU_CTRL_ID,                        //mms richedit edit menu ctl
    
    //FOR SETTING
    MMIMMS_SETTING_MENU_CTRL_ID,                     //mms setting window menu ctl
    MMIMMS_LINKLIST_CTRL_ID,                         //link setting list
    MMIMMS_SETTING_SOUND_LIST_CTRL_ID,               //setting sound choose
#ifdef DATA_ROAMING_SUPPORT
    MMS_SETTING_RETRIEVEMODE_LOCAL_TITLE_LABEL_CTRL_ID,
    MMS_SETTING_RETRIEVEMODE_ROAM_TITLE_LABEL_CTRL_ID,
    MMS_SETTING_RETRIEVEMODE_LOCAL_DROPDOWNLIST_CTRL_ID,
    MMS_SETTING_RETRIEVEMODE_ROAM_DROPDOWNLIST_CTRL_ID,
#else
    MMS_SETTING_AOTODOWN_TITLE_LABEL_CTRL_ID,        //recieve optiong aotodown label ctl
    MMS_SETTING_AOTODOWN_DROPDOWNLIST_CTRL_ID,       //recieve optiong aotodown dropdownlist ctl
#endif
    MMS_SETTING_ADVERTISE_TITLE_LABEL_CTRL_ID,       //recieve optiong advertise label ctl
    MMS_SETTING_ADVERTISE_DROPDOWNLIST_CTRL_ID,      //recieve optiong advertise dropdownlist ctl
    MMS_SETTING_PERMIT_RPT_TITLE_LABEL_CTRL_ID,
    MMS_SETTING_PERMIT_RPT_DROPDOWNLIST_CTRL_ID,
    MMS_SETTING_PERIOD_TITLE_LABEL_CTRL_ID,          //send optiong period label ctl
    MMS_SETTING_PERIOD_DROPDOWNLIST_CTRL_ID,         //send optiong period dropdownlist ctl
    MMS_SETTING_PRIORITY_TITLE_LABEL_CTRL_ID,        //send optiong priority label ctl
    MMS_SETTING_PRIORITY_DROPDOWNLIST_CTRL_ID,       //send optiong priority dropdownlist ctl
    MMS_SETTING_SENDRPT_TITLE_LABEL_CTRL_ID,         //send optiong period label ctl
    MMS_SETTING_SENDRPT_DROPDOWNLIST_CTRL_ID,        //send optiong period dropdownlist ctl
    MMS_SETTING_READRPT_TITLE_LABEL_CTRL_ID,         //send optiong period label ctl
    MMS_SETTING_READRPT_DROPDOWNLIST_CTRL_ID,        //send optiong period dropdownlist ctl
    MMS_SETTING_ANONYMITY_TITLE_LABEL_CTRL_ID,       //send optiong period label ctl
    MMS_SETTING_ANONYMITY_DROPDOWNLIST_CTRL_ID,      //send optiong period dropdownlist ctl 
    MMIMMS_RECEVINGSETTING_CTRL_ID,
#ifdef DATA_ROAMING_SUPPORT
    MMS_SETTING_RETRIEVEMODE_LOCAL_FORM_CTRL_ID,
    MMS_SETTING_RETRIEVEMODE_ROAM_FORM_CTRL_ID,
#else
    MMS_SETTING_AOTODOWN_FORM_CTRL_ID,
#endif
    MMS_SETTING_ADVERTISE_FORM_CTRL_ID,
    MMS_SETTING_PERMIT_RPT_FORM_CTRL_ID,
    MMIMMS_SENDINGSETTING_CTRL_ID,
    MMIMMS_SAVE_FILE_POS_ID,
#ifndef MMS_SMS_IN_1_SUPPORT
#ifdef MMI_SMS_FONT_SETTING_SUPPORT
    MMIMMS_SETTING_FONT_SIZE_CTRL_ID,
#endif
#endif
    MMIMMS_SETTING_SAVE_FILE_POS_CTRL_ID,
    MMIMMS_SETTING_SAVE_FILE_TITLE_CTRL_ID,
    MMS_SETTING_PERIOD_FORM_CTRL_ID,
    MMS_SETTING_PRIORITY_FORM_CTRL_ID,
    MMS_SETTING_SENDRPT_FORM_CTRL_ID,
    MMS_SETTING_READRPT_FORM_CTRL_ID,
    MMS_SETTING_ANONYMITY_FORM_CTRL_ID,

    //FOR MMS EDIT
    MMIMMS_EDIT_SUBJECT_CTRL_ID,                     //subject edit box ctl id
    MMIMMS_SET_SLIDE_TIME_CTRL_ID,                   //set slide time menu ctl
    MMIMMS_EDIT_TO_LIST_CTRL_ID,                     //to list edit box ctl
    MMIMMS_EDIT_TEXT_CTRL_ID,                        //text edit box ctl
    MMIMMS_EDIT_MAINMENU_OPT_CTRL_ID,                //main menu option
    MMIMMS_EDIT_ADDMENU_CTRL_ID,                     //add file menu
    MMIMMS_SELECT_SIM_SEND_CTRL_ID,                  //select sim card
    MMIMMS_EDIT_BUTTON_LF_CTRL_ID,
    MMIMMS_EDIT_BUTTON_RH_CTRL_ID,
    MMIMMS_EDIT_MULTI_POP_CTRL_ID,
    MMIMMS_TEXT_POSITION_CTRL_ID,
    MMIMMS_EDIT_ATTACHMENT_CTRL_ID,

    //FOR MMS PREVIEW
    MMIMMS_PREVIEW_CTRL_ID,                          //preview main menu
    MMIMMS_PREVIEW_NUMBER_POP_CTRL_ID,
    MMIMMS_PREVIEW_URL_POP_CTRL_ID,
    MMIMMS_PREVIEW_EMAIL_POP_CTRL_ID,
    MMIMMS_BUTTON_LF_CTRL_ID,                        //preview left button 
    MMIMMS_BUTTON_RH_CTRL_ID,                        //preview right button
    MMIMMS_INBOX_MMSOPT_MENU_CTRL_ID,                //inbox play mms opt menu ctl
    MMIMMS_SENTBOX_OPT_MENU_CTRL_ID,                 //sent box mms play opt
    MMIMMS_OUTBOX_OPT_MENU_CTRL_ID,                  //out box mms play opt
    MMIMMS_NOTIDETAIL_TEXTBOX_CTRL_ID,               //mms read detail menu ctl 
    MMIMMS_NOTIDETAIL_OPTMENU_CTRL_ID,
    MMIMMS_SAVE_MULTIFILE_LIST_CTRL_ID,              //multifile save
    MMIMMS_MULTIFILE_VIEW_CTRL_ID,
    
#ifdef MMI_PDA_SUPPORT
    MMIMMS_EDIT_BUTTON_FORM_CTRL_ID,
    MMIMMS_EDIT_BUTTON_FORM_PREVIEW_CTRL_ID,
    MMIMMS_EDIT_BUTTON_FORM_INSERT_ID,
    MMIMMS_EDIT_BUTTON_FORM_SEND_ID,
    MMIMMS_INBOX_NOTIDETAIL_BUTTON_FORM_CTRL_ID,
    
    MMIMMS_INBOX_NOTIDETAIL_BUTTON_DOWNLOAD_CTRL_ID,
    MMIMMS_INBOX_NOTIDETAIL_BUTTON_SMS_REPLY_CTRL_ID,
    MMIMMS_INBOX_NOTIDETAIL_BUTTON_DELETE_CTRL_ID,            

    MMIMMS_PREVIEW_BUTTON_FORM_CTRL_ID,
    
    MMIMMS_PREVIEW_BUTTON_PLAY_CTRL_ID,
    MMIMMS_PREVIEW_BUTTON_SMS_REPLEY_CTRL_ID,
    MMIMMS_PREVIEW_BUTTON_DELETE_CTRL_ID,            
    MMIMMS_PREVIEW_BUTTON_FOWARD_CTRL_ID,            
    MMIMMS_PREVIEW_BUTTON_SEND_CTRL_ID,   

    MMIMMS_EDIT_TO_LIST_FORM_ID,
    MMIMMS_EDIT_SUBJECT_FORM_ID,
    MMIMMS_EDIT_TEXT_FORM_ID,
    MMIMMS_EDIT_SET_SLIDE_FORM_ID,
#endif

#ifndef MMI_GUI_STYLE_TYPICAL
    MMIMMS_EDIT_INSERT_OPT_POP_CTRL_ID,
    MMIMMS_SAVE_FILE_MENU_CTRL_ID,
    MMIMMS_SEND_SETTING_CTRL_ID,
#endif

    MMIMMS_SMS_EDIT_TO_LIST_ID,

#ifdef MMIMMS_SMS_IN_1_SUPPORT
    MMIMMS_SMS_EDIT_FORM_ID,    

    MMIMMS_SMS_SUBFORM_CONTENT_AREA_FORM_ID,
    MMIMMS_SMS_SUBFORM_BUTTON_AREA_FORM_ID,
    
#ifdef SMS_AUTO_SIGNATURE_SUPPORT
    MMIMMS_SMS_DISPLAY_SINATURE_CTRL_ID,
#endif
#ifdef MMI_TIMERMSG_SUPPORT
    MMIMMS_SMS_TIMER_SMS_FORM_ID,
    MMIMMS_SMS_EDIT_TIMER_CTRL_ID,
    MMIMMS_SMS_EDIT_TIMER_FREQDATE_CTRL_ID,
#endif

    MMIMMS_SMS_EDIT_CONTACT_FORM_ID,  

    MMIMMS_SMS_EDIT_TITLE_FORM_ID,    
    MMIMMS_SMS_EDIT_BACK_BUTTON_ID,
    //MMIMMS_SMS_EDIT_LEFT_SLIDE_BUTTON_ID,
    MMIMMS_SMS_EDIT_MMS_INFO_TEXT_ID,
    //MMIMMS_SMS_EDIT_RIGHT_SLIDE_BUTTON_ID,
    
    MMIMMS_SMS_EDIT_TO_LIST_FORM_ID,    

#ifdef MMI_MESSAGE_CONTACT_NUM_MATCH_SUPPORT
    MMIMMS_SMS_EDIT_TO_DROPDOWN_LIST_ID,
#endif	
    MMIMMS_SMS_EDIT_TO_BUTTON_ID,
#ifdef MMIMMS_SMS_CC_SUPPORT
    MMIMMS_SMS_EDIT_CC_TO_LIST_FORM_ID,    
    MMIMMS_SMS_EDIT_CC_TO_LIST_ID,
    MMIMMS_SMS_EDIT_CC_TO_BUTTON_ID,

    MMIMMS_SMS_EDIT_BCC_TO_LIST_FORM_ID,    
    MMIMMS_SMS_EDIT_BCC_TO_LIST_ID,
    MMIMMS_SMS_EDIT_BCC_TO_BUTTON_ID,
#endif
    MMIMMS_SMS_EDIT_SUBJECT_LABEL_ID,    
    MMIMMS_SMS_EDIT_SUBJECT_EDIT_ID,    
    
    //MMIMMS_SMS_EDIT_TEXT_ID,  //contact input
    MMIMMS_SMS_EDIT_TEXT_CTRL_ID, // content input
    
    MMIMMS_SMS_EDIT_ELEMENT_AREA_FORM_ID,    
    MMIMMS_SMS_EDIT_ANIM_CTRL_ID, // anim input
    MMIMMS_SMS_EDIT_MUSIC_FORM_ID,
    MMIMMS_SMS_EDIT_MUSIC_ANIM_ID,
    MMIMMS_SMS_EDIT_MUSIC_TEXT_ID,

    MMIMMS_SMS_EDIT_ATTACHMENT_FORM_ID,
    MMIMMS_SMS_EDIT_ATTACHMENT_ANIM_ID,
    MMIMMS_SMS_EDIT_ATTACHMENT_TEXT_ID,

    MMIMMS_ATTACH_LISTBOX_CTRL_ID,

    MMIMMS_SMS_LABEL_COUNT_CTRL_ID,
    MMIMMS_SMS_BUTTON_INSERT_CTRL_ID,    
    MMIMMS_SMS_BUTTON_SEND_CTRL_ID,    
    MMIMMS_SMS_BUTTON_BACK_CTRL_ID,
    MMIMMS_SMS_EDIT_TO_LABEL_ID,
    MMIMMS_SMS_LABEL_AND_BUTTON_FORM_ID,
#endif
    MMIMMS_PHIZ_LISTBOX_CTRL_ID,
#ifdef DATA_ROAMING_SUPPORT    
    MMIMMS_SETTING_RETRIEVE_MODE_CTRL_ID, //added by feng.xiao
#endif    

    MMIMMS_MAX_CTRL_ID
}MMIMMS_CONTROL_ID_E;

/**---------------------------------------------------------------------------*
 **                         Function Declare                                  *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
// Description : Register mms menu group
// Global resource dependence : none
// Author: aoke.hu
// Note:
/*****************************************************************************/
PUBLIC void MMIMMS_RegWinIdNameArr(void);

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif

#endif  /* _MMIMMS_ID_H*/
