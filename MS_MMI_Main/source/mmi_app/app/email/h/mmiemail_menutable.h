/***************************************************************************************
** File Name:      mmiemail_menutable.h                                                 *
** Author:                                                                             *
** Date:           05/13/2011                                                          *
** Copyright:      2011 Spreadtrum, Incorporated. All Rights Reserved.                 *
** Description:    This file is used to describe the data struct of                    *
**                 system, application, window and control                             *
****************************************************************************************
**                         Important Edit History                                      *
** ------------------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                                         *
** 05/2011                         create                                              *
***************************************************************************************/
#ifndef _MMIEMAIL_MENUTABLE_H_
#define _MMIEMAIL_MENUTABLE_H_

/**------------------------------------------------------------------------------------*
**                         Include Files                                               *
**------------------------------------------------------------------------------------*/
#include "mmi_module.h"

/**------------------------------------------------------------------------------------*
**                         Compiler Flag                                               *
**------------------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern  "C"
    {
#endif

/**------------------------------------------------------------------------------------*
**                         Macro Declaration                                           *
**------------------------------------------------------------------------------------*/

/**------------------------------------------------------------------------------------*
**                         TYPE AND CONSTANT                                           *
**------------------------------------------------------------------------------------*/
//Menu ID
typedef enum
{
    EMAIL_MENU_MENU_START = (MMI_MODULE_EMAIL << 16),
    
    //view
    //inbox option
    EMAIL_VIEW_INBOX_OPT_DETAIL,
    EMAIL_VIEW_INBOX_OPT_RECVFULLMESSAGE,
    EMAIL_VIEW_INBOX_OPT_REPLY,
    EMAIL_VIEW_INBOX_OPT_REPLYTOALL,
    EMAIL_VIEW_INBOX_OPT_FORWARD,
    EMAIL_VIEW_INBOX_OPT_FORWARD_ATTACH,
    EMAIL_VIEW_INBOX_OPT_DELETE,
    EMAIL_VIEW_INBOX_OPT_SAVETO, 
    EMAIL_VIEW_INBOX_OPT_CONTENTEXTRACT,
    
    //outbox option
    EMAIL_VIEW_OUTBOX_OPT_DETAIL,
    EMAIL_VIEW_OUTBOX_OPT_SEND,
    EMAIL_VIEW_OUTBOX_OPT_DELETE,
    EMAIL_VIEW_OUTBOX_OPT_EDIT,
    EMAIL_VIEW_OUTBOX_OPT_CONTENTEXTRACT,
    
    //sentbox option
    EMAIL_VIEW_SENTBOX_OPT_DETAIL,
    EMAIL_VIEW_SENTBOX_OPT_FORWARD,
    EMAIL_VIEW_SENTBOX_OPT_FORWARD_ATTACH,
    EMAIL_VIEW_SENTBOX_OPT_RESEND,
    EMAIL_VIEW_SENTBOX_OPT_DELETE,
    EMAIL_VIEW_SENTBOX_OPT_CONTENTEXTRACT,
        
    //for number
    EMAIL_VIEW_LOCATE_NUMBER_OPT_CALL,
    EMAIL_VIEW_LOCATE_NUMBER_OPT_WRITEMESSAGE,
    EMAIL_VIEW_LOCATE_NUMBER_OPT_ADDTOPB,
#ifdef MMI_PDA_SUPPORT
    EMAIL_VIEW_SAVE_ATTACHMENT_OPT,
#endif    
    //for email
    EMAIL_VIEW_LOCATE_EMAIL_OPT_WRITEMESSAGE,
    EMAIL_VIEW_LOCATE_EMAIL_OPT_ADDTOPB,
    
    //for url
    EMAIL_VIEW_LOCATE_URL_OPT_STARTBROWSER,
    EMAIL_VIEW_LOCATE_URL_OPT_ADDTOBOOKMARK,
    EMAIL_VIEW_LOCATE_URL_OPT_ADDTOPB,
    
    //pb
    EMAIL_VIEW_CALL_OPT_VOICE,
    EMAIL_VIEW_CALL_OPT_IP,
    
    //pb
    EMAIL_VIEW_ADDTOPB_OPT_NEW,
    EMAIL_VIEW_ADDTOPB_OPT_EXIST,
    
    //number write
    EMAIL_VIEW_NUMBER_WRITEMESSAGE_OPT_SMS,
    EMAIL_VIEW_NUMBER_WRITEMESSAGE_OPT_MMS,
    
    //email write
    EMAIL_VIEW_EMAIL_WRITEMESSAGE_OPT_MMS,
    EMAIL_VIEW_EMAIL_WRITEMESSAGE_OPT_EMAIL,
    
    //content extract
    EMAIL_VIEW_EMAIL_CONTENTEXTRACT_OPT_ON,
    EMAIL_VIEW_EMAIL_CONTENTEXTRACT_OPT_OFF,
    
    //forwardoption
    EMAIL_VIEW_FORWARD_OPT_WITH_ATTACHMENT,
    EMAIL_VIEW_FORWARD_OPT_WITHOUT_ATTACHMENT,

    //box
    //inbox
    EMAIL_INBOX_OPT_RECVEMAIL,
    EMAIL_INBOX_OPT_SEARCHMAIL,

    EMAIL_INBOX_OPT_REPLY,
    EMAIL_INBOX_OPT_REPLYTOALL,
    EMAIL_INBOX_OPT_FORWORD,
    EMAIL_INBOX_OPT_FORWORD_ATTACH,
    EMAIL_INBOX_OPT_DELETE,
    EMAIL_INBOX_OPT_DELETEALL,
    EMAIL_INBOX_OPT_MARK,
    EMAIL_INBOX_OPT_MARKASREAD,
    EMAIL_INBOX_OPT_MARKASUNREAD,
    EMAIL_INBOX_OPT_SET_MAIL_STATUS,
    EMAIL_INBOX_OPT_CREATE_AN_EMAIL,
    EMAIL_INBOX_OPT_SORT,
    
    //inbox sort
    EMAIL_INBOX_SORT_OPT_REC_NEW,
    EMAIL_INBOX_SORT_OPT_REC_OLD,
    EMAIL_INBOX_SORT_OPT_SENDER,
    EMAIL_INBOX_SORT_OPT_SUBJECT,
    EMAIL_INBOX_SORT_OPT_SIZE_LARGE,
    EMAIL_INBOX_SORT_OPT_SIZE_SMALL,
    EMAIL_INBOX_SORT_OPT_PRO_HIGH,
    EMAIL_INBOX_SORT_OPT_PRO_LOW,
    
    //inbox forward option
    EMAIL_INBOX_FORWARD_OPT_WITH_ATTACHMENT,
    EMAIL_INBOX_FORWARD_OPT_WITHOUT_ATTACHMENT,
    
    //outbox
    EMAIL_OUTBOX_OPT_RESEND,
    EMAIL_OUTBOX_OPT_DELETE,
    EMAIL_OUTBOX_OPT_DELETEALL,
    EMAIL_OUTBOX_OPT_MARK,
    EMAIL_OUTBOX_OPT_SORT,
    EMAIL_OUTBOX_OPT_MOVETODRAFT,

    //outbox sort
    EMAIL_OUTBOX_SORT_OPT_DATE,
    EMAIL_OUTBOX_SORT_OPT_RECV,
    
    //sentsort
    EMAIL_SENTBOX_OPT_FORWORD,
    EMAIL_SENTBOX_OPT_RESEND,
    EMAIL_SENTBOX_OPT_DELETE,
    EMAIL_SENTBOX_OPT_DELETEALL,
    EMAIL_SENTBOX_OPT_MARK,
    EMAIL_SENTBOX_OPT_SORT,
    
    //sent box sort
    EMAIL_SENTBOX_SORT_OPT_DATE,
    EMAIL_SENTBOX_SORT_OPT_RECV,
    
    //draft box sort
    EMAIL_DRAFTBOX_OPT_SEND,
    EMAIL_DRAFTBOX_OPT_DELETE,
    EMAIL_DRAFTBOX_OPT_SORT,
    EMAIL_DRAFTBOX_OPT_MARK,
    EMAIL_DRAFTBOX_OPT_DELETEALL,
    
    //mark opt
    EMAIL_BOX_MARK_OPT_MARK,
    EMAIL_BOX_MARK_OPT_CANCEL_MARK,
    EMAIL_BOX_MARK_OPT_MARKALL,
    EMAIL_BOX_MARK_OPT_CANCEL_MARKALL,
    EMAIL_BOX_MARK_OPT_MARK_BY_TIME,
    
    //draft box sort
    EMAIL_DRAFTBOX_SORT_OPT_DATE,
    EMAIL_DRAFTBOX_SORT_OPT_RECV,
    EMAIL_DRAFTBOX_SORT_OPT_SUBJECT,
    
    //recv opt
    EMAIL_RECV_TYPE_ONLYHEADER,
    EMAIL_RECV_TYPE_HEADER_BODY,
    
    //email edit menu
    EMAIL_MAINMENU_OPT_RECEIVE_SEND_ITEM_ID,
    EMAIL_MAINMENU_OPT_SEARCH_MAIL_ITEM_ID,
    EMAIL_MAINMENU_OPT_NEWFOLER_ITEM_ID,
    EMAIL_MAINMENU_OPT_MAIL_SYNC_ITEM_ID,
    EMAIL_MAINMENU_OPT_ACCOUNT_SETTING_ITEM_ID,
    EMAIL_MAINMENU_OPT_SYS_SETTING_ITEM_ID,
    EMAIL_MAINMENU_OPT_HELP_ITEM_ID,
    
    EMAIL_SETTING_OPT_SEND_RECEIVE_MAIL_ID,
    EMAIL_SETTING_OPT_CREATE_ACC_ID,
    EMAIL_SETTING_OPT_CREATE_ACC_ID_EX,
    //     EMAIL_SETTING_OPT_RENAME_ITEM_ID,
    //     EMAIL_SETTING_OPT_DELETE_ITEM_ID,
    
    EMAIL_ACCOUNT_MAIN_SETTING_USER1,
    EMAIL_ACCOUNT_MAIN_SETTING_USER2,
    EMAIL_ACCOUNT_MAIN_USERINFO_ITEM_ID,
    EMAIL_ACCOUNT_MAIN_CONNECT_ITEM_ID,
    EMAIL_ACCOUNT_MAIN_PREFER_ITEM_ID,
    EMAIL_ACCOUNT_MAIN_SIGN_ITEM_ID,
    
    EMAIL_ACCOUNT_CONNECT_SEND_ID,
    EMAIL_CONNECT_SEND_SSL_YES_ID,
    EMAIL_CONNECT_SEND_SSL_NO_ID,
    EMAIL_CONNECT_SEND_AUTHENTICATION_YES_ID,
    EMAIL_CONNECT_SEND_AUTHENTICATION_NO_ID,
    EMAIL_ACCOUNT_CONNECT_RECV_ID,
    EMAIL_CONNECT_RECV_SERVTYPE_POP_ID,
    EMAIL_CONNECT_RECV_SERVTYPE_IMAP_ID,
    EMAIL_CONNECT_RECV_SSL_YES_ID,
    EMAIL_CONNECT_RECV_SSL_NO_ID,
    
    EMAIL_PREFER_DOWNLOAD_USERCONFIRM_ID,
    EMAIL_PREFER_DOWNLOAD_BODY_ID,
    EMAIL_PREFER_DOWNLOAD_HEAD_ID,
    
    EMAIL_PREFER_MAXMSGSIZE_1000_ID,
    EMAIL_PREFER_MAXMSGSIZE_2000_ID,
    EMAIL_PREFER_MAXMSGSIZE_3000_ID,
    
    EMAIL_PREFER_BACKUP_YES_ID,
    EMAIL_PREFER_BACKUP_NO_ID,
    
    EMAIL_PREFER_RECV_UNREAD_YES_ID,
    EMAIL_PREFER_RECV_UNREAD_NO_ID,
    
    EMAIL_PREFER_ATTACH_YES_ID,
    EMAIL_PREFER_ATTACH_NO_ID,
    
    EMAIL_PREFER_SAVE_SENT_YES_ID,
    EMAIL_PREFER_SAVE_SENT_NO_ID,
    
    EMAIL_PREFER_CONTAIN_ORGI_YES_ID,
    EMAIL_PREFER_CONTAIN_ORGI_NO_ID,
    
    EMAIL_PREFER_READCONFIRM_YES_ID,
    EMAIL_PREFER_READCONFIRM_NO_ID,
    
    EMAIL_PREFER_ADDSIGN_ALL_ID,
    EMAIL_PREFER_ADDSIGN_ONLY_ID,
    EMAIL_PREFER_ADDSIGN_NO_ID,
    
#if defined(MMI_PDA_SUPPORT)
    EMAIL_BOX_OPT_OPEN,
    EMAIL_BOX_OPT_MARK,
    EMAIL_VIEW_ATTACH_OPT_OPEN,
    EMAIL_VIEW_ATTACH_OPT_SAVE,
    EMAIL_EDIT_ATTACH_OPT_OPEN,
    EMAIL_EDIT_ATTACH_OPT_DELETE,
#endif

    EMAIL_MENU_MAX_ITEM_ID
}EMAIL_ID_MENU_E;

#define MENU_DEF(_TABLEID, _ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, \
    _ITEMCOUNT) \
     _TABLEID,

typedef enum
{
    EMAIL_MENU_LABEL_START = (MMI_MODULE_EMAIL << 16),
    #include "mmiemail_menutable.def"
    EMAIL_MENUTABLE_MAX
} EMAIL_MENU_LABEL_E;

#undef MENU_DEF

/**------------------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                                         *
**------------------------------------------------------------------------------------*/
/**************************************************************************************/
// Description : Register email menu group.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC void MMIEMAIL_RegMenuGroup(void);

/**------------------------------------------------------------------------------------*
**                         Compiler Flag                                               *
**------------------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif /*_MMIEMAIL_MENUTABLE_H_*/
