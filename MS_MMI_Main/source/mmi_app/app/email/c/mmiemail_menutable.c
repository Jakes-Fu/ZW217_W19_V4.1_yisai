/***************************************************************************************
** File Name:      mmiemail_menutable.c                                                *
** Author:                                                                             *
** Date:           05/13/2011                                                          *
** Copyright:      2011 Spreadtrum, Incorporated. All Rights Reserved.                 *
** Description:    This file is used to handle SMS menutable                           *
****************************************************************************************
**                         Important Edit History                                      *
** ------------------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                                         *
** 05/2011                         create mmiemail_menutable.c                         *
**                                                                                     *
***************************************************************************************/
#define _MMIEMAIL_MENUTABLE_C_

/**------------------------------------------------------------------------------------*
**                         Include Files                                               *
**------------------------------------------------------------------------------------*/
#include "guimenu.h"
#include "mmi_menutable.h"
#include "mmiemail_menutable.h"
#include "email_text.h"
#include "email_image.h"

/**------------------------------------------------------------------------------------*
**                         TYPE AND CONSTANT                                           *
**------------------------------------------------------------------------------------*/
/**************************************************************************************/
// Description : set inbox view option.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
const GUIMENU_ITEM_T menu_email_view_inbox_option[] =
#if defined(MMI_PDA_SUPPORT)//@tao.xue
{   
    {EMAIL_VIEW_INBOX_OPT_DETAIL,  TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    STXT_DETAIL,  IMAGE_EMAIL_OPTMENU_DETAIL, 0, 0, MENU_NULL},
    {EMAIL_VIEW_INBOX_OPT_RECVFULLMESSAGE,  TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_EMAIL_RECEIVE_FULL_MESSAGE,  IMAGE_EMAIL_OPTMENU_RECEIVE_FULL_MAIL, 0, 0, MENU_NULL},

    {EMAIL_VIEW_INBOX_OPT_FORWARD_ATTACH,  TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_COMMON_FORWARD,  IMAGE_EMAIL_OPTMENU_FORWORD_ATTACH, 0, 0, MENU_EMAIL_VIEW_FORWARD_OPT},
};
#else
{   
    {EMAIL_VIEW_INBOX_OPT_DETAIL,  TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    STXT_DETAIL,  0, 0, 0, MENU_NULL},
    {EMAIL_VIEW_INBOX_OPT_RECVFULLMESSAGE,  TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_EMAIL_RECEIVE_FULL_MESSAGE,  0, 0, 0, MENU_NULL},
    {EMAIL_VIEW_INBOX_OPT_REPLY,  TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_REPLY,  0, 0, 0, MENU_NULL},
    {EMAIL_VIEW_INBOX_OPT_REPLYTOALL,  TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_EMAIL_REPLYTOALL,  0, 0, 0, MENU_NULL},
    {EMAIL_VIEW_INBOX_OPT_FORWARD,  TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_COMMON_FORWARD,  0, 0, 0, MENU_NULL},
    {EMAIL_VIEW_INBOX_OPT_FORWARD_ATTACH,  TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_COMMON_FORWARD,  0, 0, 0, MENU_EMAIL_VIEW_FORWARD_OPT},
    {EMAIL_VIEW_INBOX_OPT_DELETE,  TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_DELETE,  0, 0, 0, MENU_NULL},
    {EMAIL_VIEW_INBOX_OPT_SAVETO,  TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_COMM_SAVE_TO,  0, 0, 0, MENU_EMAIL_VIEW_ADDTOPB_OPT}, 
    {EMAIL_VIEW_INBOX_OPT_CONTENTEXTRACT,  TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_COMMON_CONTENT_EXTRACT,  0, 0, 0, MENU_EMAIL_VIEW_EMAIL_CONTENTEXTRACT_OPT},
};
#endif

/**************************************************************************************/
// Description : set outbox view option.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
const GUIMENU_ITEM_T menu_email_view_outbox_option[] =
#if defined(MMI_PDA_SUPPORT)//@tao.xue
{
    {EMAIL_VIEW_OUTBOX_OPT_DETAIL,  TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    STXT_DETAIL,  IMAGE_EMAIL_OPTMENU_DETAIL, 0, 0, MENU_NULL},

    {EMAIL_VIEW_OUTBOX_OPT_EDIT,  TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_EDIT,  IMAGE_EMAIL_OPTMENU_EDIT, 0, 0, MENU_NULL},
};
#else
{
    {EMAIL_VIEW_OUTBOX_OPT_DETAIL,  TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    STXT_DETAIL,  0, 0, 0, MENU_NULL},
    {EMAIL_VIEW_OUTBOX_OPT_SEND,  TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_SEND,  0, 0, 0, MENU_NULL},
    {EMAIL_VIEW_OUTBOX_OPT_DELETE,  TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_DELETE,  0, 0, 0, MENU_NULL},
    {EMAIL_VIEW_OUTBOX_OPT_EDIT,  TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_EDIT,  0, 0, 0, MENU_NULL},
    {EMAIL_VIEW_OUTBOX_OPT_CONTENTEXTRACT,  TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_COMMON_CONTENT_EXTRACT,  0, 0, 0, MENU_EMAIL_VIEW_EMAIL_CONTENTEXTRACT_OPT}, 
};
#endif
/**************************************************************************************/
// Description : set sentbox view option.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
const GUIMENU_ITEM_T menu_email_view_sentbox_option[] =
#if defined(MMI_PDA_SUPPORT)//@tao.xue
{
    {EMAIL_VIEW_SENTBOX_OPT_DETAIL,  TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    STXT_DETAIL,  IMAGE_EMAIL_OPTMENU_DETAIL, 0, 0, MENU_NULL},

    {EMAIL_VIEW_SENTBOX_OPT_FORWARD_ATTACH,  TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_COMMON_FORWARD, IMAGE_EMAIL_OPTMENU_FORWORD_ATTACH, 0, 0, MENU_EMAIL_VIEW_FORWARD_OPT}, 
};
#else
{
    {EMAIL_VIEW_SENTBOX_OPT_DETAIL,  TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    STXT_DETAIL,  0, 0, 0, MENU_NULL},
    {EMAIL_VIEW_SENTBOX_OPT_FORWARD,  TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_COMMON_FORWARD, 0, 0, 0, MENU_NULL},
    {EMAIL_VIEW_SENTBOX_OPT_FORWARD_ATTACH,  TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_COMMON_FORWARD, 0, 0, 0, MENU_EMAIL_VIEW_FORWARD_OPT}, 
    {EMAIL_VIEW_SENTBOX_OPT_RESEND,  TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_RESEND,  0, 0, 0, MENU_NULL},
    {EMAIL_VIEW_SENTBOX_OPT_DELETE,  TIP_NULL, {TXT_NULL, TXT_NULL,TXT_NULL},
    TXT_DELETE,  0, 0, 0, MENU_NULL},
    {EMAIL_VIEW_SENTBOX_OPT_CONTENTEXTRACT,  TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_COMMON_CONTENT_EXTRACT,  0, 0, 0, MENU_EMAIL_VIEW_EMAIL_CONTENTEXTRACT_OPT},
};
#endif

/**************************************************************************************/
// Description : set email view option locate number opt.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
const GUIMENU_ITEM_T menu_email_view_locate_number_option[] =
{
#ifdef MMI_IP_CALL_SUPPORT
    {EMAIL_VIEW_LOCATE_NUMBER_OPT_CALL,  TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_CALL,  0, 0, 0, MENU_EMAIL_VIEW_CALL_OPT},
#else
    {EMAIL_VIEW_CALL_OPT_VOICE,  TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_CALL,  0, 0, 0, MENU_NULL},
#endif
    {EMAIL_VIEW_LOCATE_NUMBER_OPT_WRITEMESSAGE,  TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_COMMON_WRITE_MESSAGE,  0, 0, 0, MENU_EMAIL_VIEW_NUM_WRITEMESSAGE_OPT},
    {EMAIL_VIEW_LOCATE_NUMBER_OPT_ADDTOPB,  TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_ADDCONTACT,  0, 0, 0, MENU_EMAIL_VIEW_ADDTOPB_OPT},
};

/**************************************************************************************/
// Description : set email view to locate opt.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
const GUIMENU_ITEM_T menu_email_view_locate_email_option[] =
{
    {EMAIL_VIEW_LOCATE_EMAIL_OPT_WRITEMESSAGE,  TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_COMMON_WRITE_MESSAGE,  0, 0, 0, MENU_EMAIL_VIEW_EMAIL_WRITEMESSAGE_OPT},
    {EMAIL_VIEW_LOCATE_EMAIL_OPT_ADDTOPB,  TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_ADDCONTACT,  0, 0, 0, MENU_EMAIL_VIEW_ADDTOPB_OPT},
};

/**************************************************************************************/
// Description : set email view to locate url opt.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
const GUIMENU_ITEM_T menu_email_view_locate_url_option[] =
{
    {EMAIL_VIEW_LOCATE_URL_OPT_STARTBROWSER,  TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_EMAIL_VIEW,  0, 0, 0, MENU_NULL},
    {EMAIL_VIEW_LOCATE_URL_OPT_ADDTOBOOKMARK,  TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_ADDTO_BOOKMARK,  0, 0, 0, MENU_NULL},
#ifndef PB_SUPPORT_LOW_MEMORY
    {EMAIL_VIEW_LOCATE_URL_OPT_ADDTOPB,  TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_ADDCONTACT,  0, 0, 0, MENU_EMAIL_VIEW_ADDTOPB_OPT},
#endif
};
#ifdef MMI_PDA_SUPPORT
const GUIMENU_ITEM_T menu_email_view_attach_option[] =
{
    {EMAIL_VIEW_ATTACH_OPT_OPEN,  TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_COMM_OPEN,  0, 0, 0, MENU_NULL},
    {EMAIL_VIEW_ATTACH_OPT_SAVE,  TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    STXT_SAVE,  0, 0, 0, MENU_NULL},
};
const GUIMENU_ITEM_T menu_email_edit_attach_option[] =
{
    {EMAIL_EDIT_ATTACH_OPT_OPEN,  TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_COMM_OPEN,  0, 0, 0, MENU_NULL},
    {EMAIL_EDIT_ATTACH_OPT_DELETE,  TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_DELETE,  0, 0, 0, MENU_NULL},
};
const GUIMENU_ITEM_T menu_email_save_attachment_option[] =
{
    {EMAIL_VIEW_SAVE_ATTACHMENT_OPT,  TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    STXT_SAVE,  IMAGE_EMAIL_OPTMENU_SAVE_TO, 0, 0, MENU_NULL},
};
#endif
#ifdef MMI_IP_CALL_SUPPORT
/**************************************************************************************/
// Description : set email view to call opt.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
const GUIMENU_ITEM_T menu_email_view_call_option[] =
{
    {EMAIL_VIEW_CALL_OPT_VOICE,  TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_CALL_AUDIO,  0, 0, 0, MENU_NULL},
    {EMAIL_VIEW_CALL_OPT_IP,  TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_CALL_IPCALL,  0, 0, 0, MENU_NULL},
};
#endif
/**************************************************************************************/
// Description : set email view to add to address opt.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
const GUIMENU_ITEM_T menu_email_view_addtopb_option[] =
{
    {EMAIL_VIEW_ADDTOPB_OPT_NEW,  TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_ADDTOPB_NEW,  0, 0, 0, MENU_NULL},
#ifdef MMIPB_ADD_EXIST_CONTACT_SUPPORT
    {EMAIL_VIEW_ADDTOPB_OPT_EXIST,  TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_ADDTOPB_EXIST,  0, 0, 0, MENU_NULL},
#endif
};

/**************************************************************************************/
// Description : set email view option to write message/mms by tel number.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
const GUIMENU_ITEM_T menu_email_view_num_writemessage_option[] =
{
    {EMAIL_VIEW_NUMBER_WRITEMESSAGE_OPT_SMS,  TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_SMS,  0, 0, 0, MENU_NULL},
    {EMAIL_VIEW_NUMBER_WRITEMESSAGE_OPT_MMS,  TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_MMS,  0, 0, 0, MENU_NULL},
};

/**************************************************************************************/
// Description : set email view option to write mms/email by tel number.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
const GUIMENU_ITEM_T menu_email_view_email_writemessage_option[] =
{
    {EMAIL_VIEW_EMAIL_WRITEMESSAGE_OPT_MMS,  TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_MMS,  0, 0, 0, MENU_NULL},
    {EMAIL_VIEW_EMAIL_WRITEMESSAGE_OPT_EMAIL,  TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_EMAIL_TITLE,  0, 0, 0, MENU_NULL},
};

/**************************************************************************************/
// Description : set email view option, to set conten textract off/on.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
const GUIMENU_ITEM_T menu_email_view_email_contentextract_option[] =
{
    {EMAIL_VIEW_EMAIL_CONTENTEXTRACT_OPT_ON,  TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_COMMON_ON,  0, 0, 0, MENU_NULL},
    {EMAIL_VIEW_EMAIL_CONTENTEXTRACT_OPT_OFF,  TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_COMMON_OFF,  0, 0, 0, MENU_NULL},
};

/**************************************************************************************/
// Description : set email view forward option.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
const GUIMENU_ITEM_T menu_email_view_forward_option[] =
{
    {EMAIL_VIEW_FORWARD_OPT_WITH_ATTACHMENT,  TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_EMAIL_FORWARD_WITH_ATTACH, 0, 0, 0, MENU_NULL},
    {EMAIL_VIEW_FORWARD_OPT_WITHOUT_ATTACHMENT,  TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_EMAIL_FORWARD_WITHOUT_ATTACH, 0, 0, 0, MENU_NULL},
};

/**************************************************************************************/
// Description : set inbox option.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
const GUIMENU_ITEM_T menu_email_inbox_option[] =
#if defined(MMI_PDA_SUPPORT)//@tao.xue
{ 
    {EMAIL_INBOX_OPT_CREATE_AN_EMAIL, TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_EMAIL_CREATE_AN_EMAIL, IMAGE_EMAIL_OPTMENU_NEW_MAIL, 0, 0, MENU_NULL},
    {EMAIL_INBOX_OPT_RECVEMAIL, TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_COMMON_RECV_EMAIL, IMAGE_EMAIL_OPTMENU_RECEIVE_FULL_MAIL, 0, 0, MENU_NULL},
    {EMAIL_INBOX_OPT_SORT, TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL}, 
    TXT_COMMON_SORT, IMAGE_COMMON_OPTMENU_SORT, 0, 0, MENU_EMAIL_INBOX_SORT_OPT},
    {EMAIL_BOX_OPT_MARK, TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    STXT_MARK, IMAGE_COMMON_OPTMENU_MARK, 0, 0, MENU_NULL},
    {EMAIL_INBOX_OPT_DELETEALL, TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_DELALL, IMAGE_COMMON_OPTMENU_REMOVE_ALL, 0, 0, MENU_NULL},
};
#else
{
    {EMAIL_INBOX_OPT_REPLY, TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_REPLY, 0, 0, 0, MENU_NULL},
    {EMAIL_INBOX_OPT_REPLYTOALL, TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_EMAIL_REPLYTOALL, 0, 0, 0, MENU_NULL},
    {EMAIL_INBOX_OPT_FORWORD, TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_COMMON_FORWARD, 0, 0, 0, MENU_NULL},
    {EMAIL_INBOX_OPT_FORWORD_ATTACH,  TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_COMMON_FORWARD,  0, 0, 0, MENU_EMAIL_VIEW_FORWARD_OPT},
    {EMAIL_INBOX_OPT_DELETE, TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_DELETE, 0, 0, 0, MENU_NULL},
    {EMAIL_INBOX_OPT_MARK, TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    STXT_MARK, 0, 0, 0, MENU_EMAIL_BOX_MARK_OPT},
    {EMAIL_INBOX_OPT_SET_MAIL_STATUS, TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_EMAIL_SET_MAIL_STATUS, 0, 0, 0, MENU_EMAIL_SET_STATUS_OPT},
    {EMAIL_INBOX_OPT_CREATE_AN_EMAIL, TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_EMAIL_CREATE_AN_EMAIL, 0, 0, 0, MENU_NULL},
    {EMAIL_INBOX_OPT_RECVEMAIL, TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_COMMON_RECV_EMAIL, 0, 0, 0, MENU_NULL},
    {EMAIL_INBOX_OPT_SORT, TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_COMMON_SORT, 0, 0, 0, MENU_EMAIL_INBOX_SORT_OPT},
    {EMAIL_INBOX_OPT_DELETEALL, TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_DELALL, 0, 0, 0, MENU_NULL},
};
#endif

/**************************************************************************************/
// Description : set MENU_EMAIL_SET_STATUS_OPT option.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
const GUIMENU_ITEM_T menu_email_set_status_option[] =
{
    {EMAIL_INBOX_OPT_MARKASREAD, TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_EMAIL_MARK_AS_READ, 0, 0, 0, MENU_NULL},
    {EMAIL_INBOX_OPT_MARKASUNREAD, TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_EMAIL_MARK_AS_UNREAD, 0, 0, 0, MENU_NULL},
};

/**************************************************************************************/
// Description : set inbox option ex.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
// const GUIMENU_ITEM_T menu_email_inbox_option_ex[] =
// {
//     {EMAIL_INBOX_OPT_FORWORD, TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
//     TXT_COMMON_FORWARD, 0, 0, 0, MENU_NULL},
//     {EMAIL_INBOX_OPT_DELETE, TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
//     TXT_DELETE, 0, 0, 0, MENU_NULL},
//     {EMAIL_INBOX_OPT_DELETEALL, TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
//     TXT_DELALL, 0, 0, 0, MENU_NULL},
//     {EMAIL_INBOX_OPT_MARK, TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
//     STXT_MARK, 0, 0, 0, MENU_EMAIL_BOX_MARK_OPT},
//     {EMAIL_INBOX_OPT_MARKASREAD, TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
//     TXT_EMAIL_MARK_AS_READ, 0, 0, 0, MENU_NULL},
//     {EMAIL_INBOX_OPT_MARKASUNREAD, TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
//     TXT_EMAIL_MARK_AS_UNREAD, 0, 0, 0, MENU_NULL},
//     {EMAIL_INBOX_OPT_SORT, TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
//     TXT_COMMON_SORT, 0, 0, 0, MENU_EMAIL_INBOX_SORT_OPT},
// };

/**************************************************************************************/
// Description : set inbox option then no email.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
const GUIMENU_ITEM_T menu_email_inbox_none_option[] =
#if defined(MMI_PDA_SUPPORT)//@tao.xue
{
    {EMAIL_INBOX_OPT_CREATE_AN_EMAIL, TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_EMAIL_CREATE_AN_EMAIL, IMAGE_EMAIL_OPTMENU_NEW_MAIL, 0, 0, MENU_NULL},
    {EMAIL_INBOX_OPT_RECVEMAIL, TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_COMMON_RECV_EMAIL, IMAGE_EMAIL_OPTMENU_RECEIVE_FULL_MAIL, 0, 0, MENU_NULL},
//     {EMAIL_INBOX_OPT_SEARCHMAIL, TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
//     TXT_EMAIL_SEARCH_MAIL, 0, 0, 0, MENU_NULL},
};
#else
{
    {EMAIL_INBOX_OPT_CREATE_AN_EMAIL, TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_EMAIL_CREATE_AN_EMAIL, 0, 0, 0, MENU_NULL},
    {EMAIL_INBOX_OPT_RECVEMAIL, TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_COMMON_RECV_EMAIL, 0, 0, 0, MENU_NULL},
//     {EMAIL_INBOX_OPT_SEARCHMAIL, TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
//     TXT_EMAIL_SEARCH_MAIL, 0, 0, 0, MENU_NULL},
};
#endif

/**************************************************************************************/
// Description : set inbox option then no email, ex.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
// const GUIMENU_ITEM_T menu_email_inbox_none_option_ex[] =
// {
//     {EMAIL_INBOX_OPT_RECVEMAIL, TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
//     TXT_COMMON_RECV_EMAIL, 0, 0, 0, MENU_EMAIL_RECV_TYPE_OPT},
// };

/**************************************************************************************/
// Description : set outbox option.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
const GUIMENU_ITEM_T menu_email_outbox_option[] =
#if defined(MMI_PDA_SUPPORT)//@tao.xue
{
    {EMAIL_OUTBOX_OPT_SORT, TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_COMMON_SORT, 0, 0, 0, MENU_EMAIL_OUTBOX_SORT_OPT},
    {EMAIL_BOX_OPT_MARK, TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    STXT_MARK, IMAGE_COMMON_OPTMENU_MARK, 0, 0, MENU_NULL},
    {EMAIL_OUTBOX_OPT_DELETEALL,  TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_DELALL, IMAGE_COMMON_OPTMENU_REMOVE_ALL, 0, 0, MENU_NULL},
};
#else
{
    {EMAIL_OUTBOX_OPT_RESEND, TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_RESEND, 0, 0, 0, MENU_NULL},
    {EMAIL_OUTBOX_OPT_DELETE, TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_DELETE, 0, 0, 0, MENU_NULL},
    {EMAIL_OUTBOX_OPT_DELETEALL,  TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_DELALL, 0, 0, 0, MENU_NULL},
    {EMAIL_OUTBOX_OPT_SORT, TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_COMMON_SORT, 0, 0, 0, MENU_EMAIL_OUTBOX_SORT_OPT},
    {EMAIL_OUTBOX_OPT_MARK, TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    STXT_MARK, 0, 0, 0, MENU_EMAIL_BOX_MARK_OPT},
    {EMAIL_OUTBOX_OPT_MOVETODRAFT, TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_EMAIL_MOVE_TO_DRAFT, 0, 0, 0, MENU_NULL},
};
#endif

/**************************************************************************************/
// Description : set sentbox option.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
const GUIMENU_ITEM_T menu_email_sentbox_option[] =
#if defined(MMI_PDA_SUPPORT)//@tao.xue
{
    {EMAIL_SENTBOX_OPT_SORT, TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_COMMON_SORT, IMAGE_COMMON_OPTMENU_SORT, 0, 0, MENU_EMAIL_SENTBOX_SORT_OPT},
    {EMAIL_BOX_OPT_MARK, TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    STXT_MARK, IMAGE_COMMON_OPTMENU_MARK, 0, 0, MENU_NULL},
    {EMAIL_SENTBOX_OPT_DELETEALL, TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_DELALL, IMAGE_COMMON_OPTMENU_REMOVE_ALL, 0, 0, MENU_NULL},
};
#else
{
    {EMAIL_SENTBOX_OPT_FORWORD, TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_COMMON_FORWARD, 0, 0, 0, MENU_NULL},
    {EMAIL_SENTBOX_OPT_RESEND, TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_RESEND, 0, 0, 0, MENU_NULL},
    {EMAIL_SENTBOX_OPT_DELETE, TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_DELETE, 0, 0, 0, MENU_NULL},
    {EMAIL_SENTBOX_OPT_DELETEALL, TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_DELALL, 0, 0, 0, MENU_NULL},
    {EMAIL_SENTBOX_OPT_SORT, TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_COMMON_SORT, 0, 0, 0, MENU_EMAIL_SENTBOX_SORT_OPT},
    {EMAIL_SENTBOX_OPT_MARK, TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    STXT_MARK, 0, 0, 0, MENU_EMAIL_BOX_MARK_OPT},
};
#endif

/**************************************************************************************/
// Description : set draftbox option.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
const GUIMENU_ITEM_T menu_email_draftbox_option[] =
#if defined(MMI_PDA_SUPPORT)//@tao.xue
{
    {EMAIL_DRAFTBOX_OPT_SORT, TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_COMMON_SORT, IMAGE_COMMON_OPTMENU_SORT, 0, 0, MENU_EMAIL_DRAFTBOX_SORT_OPT},
    {EMAIL_BOX_OPT_MARK, TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    STXT_MARK, IMAGE_COMMON_OPTMENU_MARK, 0, 0, MENU_NULL},
    {EMAIL_DRAFTBOX_OPT_DELETEALL, TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_DELALL, IMAGE_COMMON_OPTMENU_REMOVE_ALL, 0, 0, MENU_NULL},
};
#else
{
    {EMAIL_DRAFTBOX_OPT_SEND, TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_SEND, 0, 0, 0, MENU_NULL},
    {EMAIL_DRAFTBOX_OPT_DELETE, TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_DELETE, 0, 0, 0, MENU_NULL},
    {EMAIL_DRAFTBOX_OPT_SORT, TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_COMMON_SORT, 0, 0, 0, MENU_EMAIL_DRAFTBOX_SORT_OPT},
    {EMAIL_DRAFTBOX_OPT_MARK, TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    STXT_MARK, 0, 0, 0, MENU_EMAIL_BOX_MARK_OPT},
    {EMAIL_DRAFTBOX_OPT_DELETEALL, TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_DELALL, 0, 0, 0, MENU_NULL},
};
#endif

/**************************************************************************************/
// Description : set inbox sort option.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
const GUIMENU_ITEM_T menu_email_inbox_sort_option[] =
{
    {EMAIL_INBOX_SORT_OPT_REC_NEW, TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_EMAIL_SORT_AS_REC_NEW, 0, 0, 0, MENU_NULL},
    {EMAIL_INBOX_SORT_OPT_REC_OLD, TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_EMAIL_SORT_AS_REC_OLD, 0, 0, 0, MENU_NULL},
    {EMAIL_INBOX_SORT_OPT_SENDER, TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_COMMON_MAIL_FROM, 0, 0, 0, MENU_NULL},
    {EMAIL_INBOX_SORT_OPT_SUBJECT, TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_COMMON_SUBJECT, 0, 0, 0, MENU_NULL},
    {EMAIL_INBOX_SORT_OPT_SIZE_LARGE, TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_EMAIL_SORT_AS_SIZE_LARGE, 0, 0, 0, MENU_NULL},
    {EMAIL_INBOX_SORT_OPT_SIZE_SMALL, TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_EMAIL_SORT_AS_SIZE_SMALL, 0, 0, 0, MENU_NULL},
    {EMAIL_INBOX_SORT_OPT_PRO_HIGH, TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_EMAIL_SORT_AS_PRO_HIGH, 0, 0, 0, MENU_NULL},
    {EMAIL_INBOX_SORT_OPT_PRO_LOW, TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_EMAIL_SORT_AS_PRO_LOW, 0, 0, 0, MENU_NULL},
};

/**************************************************************************************/
// Description : set outbox sort option.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
const GUIMENU_ITEM_T menu_email_outbox_sort_option[] =
{
   {EMAIL_OUTBOX_SORT_OPT_RECV, TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
   TXT_COMMON_RECIPIENT, 0, 0, 0, MENU_NULL},
   {EMAIL_OUTBOX_SORT_OPT_DATE, TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
   TXT_EMAIL_DATATIME, 0, 0, 0, MENU_NULL},
};

/**************************************************************************************/
// Description : set sentbox sort option.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
const GUIMENU_ITEM_T menu_email_sentbox_sort_option[] =
{
   {EMAIL_SENTBOX_SORT_OPT_DATE, TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
   TXT_EMAIL_DATATIME, 0, 0, 0, MENU_NULL},
   {EMAIL_SENTBOX_SORT_OPT_RECV, TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
   TXT_COMMON_RECIPIENT, 0, 0, 0, MENU_NULL},
};

/**************************************************************************************/
// Description : set draftbox sort option.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
const GUIMENU_ITEM_T menu_email_draftbox_sort_option[] =
{
    {EMAIL_DRAFTBOX_SORT_OPT_DATE, TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_EMAIL_DATATIME, 0, 0, 0, MENU_NULL},
    {EMAIL_DRAFTBOX_SORT_OPT_RECV,  TIP_NULL, {TXT_NULL, TXT_NULL,TXT_NULL},
    TXT_COMMON_RECIPIENT, 0, 0, 0, MENU_NULL},
};

/**************************************************************************************/
// Description : set box mark option.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
const GUIMENU_ITEM_T menu_email_box_mark_option[] =
#if defined(MMI_PDA_SUPPORT)//@tao.xue
{
    {EMAIL_BOX_MARK_OPT_MARKALL, TIP_NULL, {TXT_NULL, TXT_NULL,TXT_NULL},
    TXT_MARK_ALL, IMAGE_COMMON_OPTMENU_MARK_ALL, 0, 0, MENU_NULL},
    {EMAIL_BOX_MARK_OPT_CANCEL_MARKALL, TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_CANCEL_MARK_ALL, IMAGE_COMMON_OPTMENU_CANCEL_MARK_ALL, 0, 0, MENU_NULL},
    {EMAIL_BOX_MARK_OPT_MARK_BY_TIME, TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_EMAIL_MARK_BY_TIME, IMAGE_COMMON_OPTMENU_MARK, 0, 0, MENU_NULL},

    {EMAIL_INBOX_OPT_MARKASREAD, TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_EMAIL_MARK_AS_READ, IMAGE_EMAIL_OPTMENU_MARKAS_READ, 0, 0, MENU_NULL},
    {EMAIL_INBOX_OPT_MARKASUNREAD, TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_EMAIL_MARK_AS_UNREAD, IMAGE_EMAIL_OPTMENU_MARKAS_UNREAD, 0, 0, MENU_NULL},

};
#else
{
    {EMAIL_BOX_MARK_OPT_MARK, TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    STXT_MARK, 0, 0, 0, MENU_NULL},
    {EMAIL_BOX_MARK_OPT_CANCEL_MARK, TIP_NULL, {TXT_NULL, TXT_NULL,TXT_NULL},
    TXT_CANCEL_MARK, 0, 0, 0, MENU_NULL},
    {EMAIL_BOX_MARK_OPT_MARKALL, TIP_NULL, {TXT_NULL, TXT_NULL,TXT_NULL},
    TXT_MARK_ALL, 0, 0, 0, MENU_NULL},
    {EMAIL_BOX_MARK_OPT_CANCEL_MARKALL, TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_CANCEL_MARK_ALL, 0, 0, 0, MENU_NULL},
    {EMAIL_BOX_MARK_OPT_MARK_BY_TIME, TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_EMAIL_MARK_BY_TIME, 0, 0, 0, MENU_NULL},
};
#endif

/**************************************************************************************/
// Description : set inbox forward option.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
const GUIMENU_ITEM_T menu_email_inbox_forward_option[] =
{
    {EMAIL_INBOX_FORWARD_OPT_WITH_ATTACHMENT,  TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_EMAIL_FORWARD_WITH_ATTACH, 0, 0, 0, MENU_NULL},
    {EMAIL_INBOX_FORWARD_OPT_WITHOUT_ATTACHMENT,  TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_EMAIL_FORWARD_WITHOUT_ATTACH, 0, 0, 0, MENU_NULL},
};

/**************************************************************************************/
// Description : set receive email's option.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
const GUIMENU_ITEM_T menu_email_recv_type_option[] =
{
    {EMAIL_RECV_TYPE_ONLYHEADER, TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_EMAIL_ONLYHEADER, 0, 0, 0, MENU_NULL},
    {EMAIL_RECV_TYPE_HEADER_BODY, TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_EMAIL_HEADERANDBODY, 0, 0, 0, MENU_NULL},
};

/**************************************************************************************/
// Description : set main menu option.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
const GUIMENU_ITEM_T menu_email_main_menu_opt[] =
#if defined(MMI_PDA_SUPPORT)//@tao.xue
{
    {EMAIL_MAINMENU_OPT_RECEIVE_SEND_ITEM_ID, TIP_NULL, {STXT_SELECT, TXT_NULL, STXT_RETURN},
    TXT_COMMON_RECV_EMAIL,  IMAGE_EMAIL_OPTMENU_RECEIVE_SEND_MAIL, 0, 0, MENU_NULL},
    //   {EMAIL_MAINMENU_OPT_SEARCH_MAIL_ITEM_ID,  TIP_NULL, {STXT_SELECT, TXT_NULL, STXT_RETURN},
    //   TXT_EMAIL_SEARCH_MAIL,  0, 0, 0, PNULL},
    //   {EMAIL_MAINMENU_OPT_NEWFOLER_ITEM_ID, TIP_NULL, {STXT_SELECT, TXT_NULL, STXT_RETURN},
    //   TXT_EMAIL_NEW_FOLDER,  0, 0, 0, MENU_NULL},
    //   {EMAIL_MAINMENU_OPT_MAIL_SYNC_ITEM_ID,  TIP_NULL, {STXT_SELECT, TXT_NULL, STXT_RETURN},
    //   TXT_EMAIL_SYNC_MAIL,  0, 0, 0, PNULL},
    {EMAIL_MAINMENU_OPT_ACCOUNT_SETTING_ITEM_ID, TIP_NULL, {STXT_SELECT, TXT_NULL, STXT_RETURN},
    TXT_ACCOUNT_SETTING,  IMAGE_EMAIL_OPTMENU_ACC_SETTING, 0, 0, MENU_NULL},
    {EMAIL_MAINMENU_OPT_SYS_SETTING_ITEM_ID,  TIP_NULL, {STXT_SELECT, TXT_NULL, STXT_RETURN},
    TXT_SYSTEM_SETTING,  IMAGE_COMMON_OPTMENU_SET, 0, 0, PNULL},
    //   {EMAIL_MAINMENU_OPT_HELP_ITEM_ID, TIP_NULL, {STXT_SELECT, TXT_NULL, STXT_RETURN},
    //   TXT_HELP,  0, 0, 0, MENU_NULL},
};
#else
{
    {EMAIL_MAINMENU_OPT_RECEIVE_SEND_ITEM_ID, TIP_NULL, {STXT_SELECT, TXT_NULL, STXT_RETURN},
    TXT_COMMON_RECV_EMAIL,  0, 0, 0, MENU_NULL},
    //   {EMAIL_MAINMENU_OPT_SEARCH_MAIL_ITEM_ID,  TIP_NULL, {STXT_SELECT, TXT_NULL, STXT_RETURN},
    //   TXT_EMAIL_SEARCH_MAIL,  0, 0, 0, PNULL},
    //   {EMAIL_MAINMENU_OPT_NEWFOLER_ITEM_ID, TIP_NULL, {STXT_SELECT, TXT_NULL, STXT_RETURN},
    //   TXT_EMAIL_NEW_FOLDER,  0, 0, 0, MENU_NULL},
    //   {EMAIL_MAINMENU_OPT_MAIL_SYNC_ITEM_ID,  TIP_NULL, {STXT_SELECT, TXT_NULL, STXT_RETURN},
    //   TXT_EMAIL_SYNC_MAIL,  0, 0, 0, PNULL},
    {EMAIL_MAINMENU_OPT_ACCOUNT_SETTING_ITEM_ID, TIP_NULL, {STXT_SELECT, TXT_NULL, STXT_RETURN},
    TXT_ACCOUNT_SETTING,  0, 0, 0, MENU_NULL},
    {EMAIL_MAINMENU_OPT_SYS_SETTING_ITEM_ID,  TIP_NULL, {STXT_SELECT, TXT_NULL, STXT_RETURN},
    TXT_SYSTEM_SETTING,  0, 0, 0, PNULL},
    //   {EMAIL_MAINMENU_OPT_HELP_ITEM_ID, TIP_NULL, {STXT_SELECT, TXT_NULL, STXT_RETURN},
    //   TXTL_HELP,  0, 0, 0, MENU_NULL},
};
#endif

#if defined(MMI_PDA_SUPPORT) 
/**************************************************************************************/
// Description : set inbox option.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
const GUIMENU_ITEM_T menu_email_inbox_pop_option[] =
{
    {EMAIL_BOX_OPT_OPEN, TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_COMM_OPEN, 0, 0, 0, MENU_NULL},
    {EMAIL_INBOX_OPT_REPLY, TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_REPLY, 0, 0, 0, MENU_NULL},
    {EMAIL_INBOX_OPT_REPLYTOALL, TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_EMAIL_REPLYTOALL, 0, 0, 0, MENU_NULL},
    {EMAIL_INBOX_OPT_FORWORD, TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_COMMON_FORWARD, 0, 0, 0, MENU_NULL},
    {EMAIL_INBOX_OPT_FORWORD_ATTACH,  TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_COMMON_FORWARD,  0, 0, 0, MENU_EMAIL_VIEW_FORWARD_OPT},
    {EMAIL_INBOX_OPT_DELETE, TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_DELETE, 0, 0, 0, MENU_NULL},
    {EMAIL_INBOX_OPT_SET_MAIL_STATUS, TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_EMAIL_SET_MAIL_STATUS, 0, 0, 0, MENU_EMAIL_SET_STATUS_OPT},
};

/**************************************************************************************/
// Description : set outbox option.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
const GUIMENU_ITEM_T menu_email_outbox_pop_option[] =
{
    {EMAIL_BOX_OPT_OPEN, TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_COMM_OPEN, 0, 0, 0, MENU_NULL},
    {EMAIL_OUTBOX_OPT_RESEND, TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_RESEND, 0, 0, 0, MENU_NULL},
    {EMAIL_OUTBOX_OPT_DELETE, TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_DELETE, 0, 0, 0, MENU_NULL},
    {EMAIL_OUTBOX_OPT_MOVETODRAFT, TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_EMAIL_MOVE_TO_DRAFT, 0, 0, 0, MENU_NULL},
};

/**************************************************************************************/
// Description : set sentbox option.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
const GUIMENU_ITEM_T menu_email_sentbox_pop_option[] =
{
    {EMAIL_BOX_OPT_OPEN, TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_COMM_OPEN, 0, 0, 0, MENU_NULL},
    {EMAIL_SENTBOX_OPT_FORWORD, TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_COMMON_FORWARD, 0, 0, 0, MENU_NULL},
    {EMAIL_SENTBOX_OPT_RESEND, TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_RESEND, 0, 0, 0, MENU_NULL},
    {EMAIL_SENTBOX_OPT_DELETE, TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_DELETE, 0, 0, 0, MENU_NULL},
};

/**************************************************************************************/
// Description : set draftbox option.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
const GUIMENU_ITEM_T menu_email_draftbox_pop_option[] =
{
    {EMAIL_BOX_OPT_OPEN, TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_COMM_OPEN, 0, 0, 0, MENU_NULL},
    {EMAIL_DRAFTBOX_OPT_SEND, TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_SEND, 0, 0, 0, MENU_NULL},
    {EMAIL_DRAFTBOX_OPT_DELETE, TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_DELETE, 0, 0, 0, MENU_NULL},
};
#endif

/**************************************************************************************/
// Description : set setting option.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
const GUIMENU_ITEM_T menu_email_setting_opt[] =
#if defined(MMI_PDA_SUPPORT)//@tao.xue
{
    //     {EMAIL_SETTING_OPT_SEND_RECEIVE_MAIL_ID,    TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN},
    //         TXT_COMMON_RECV_EMAIL,        0, 0, 0, MENU_NULL},
    {EMAIL_SETTING_OPT_CREATE_ACC_ID, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},
    TXT_EMAIL_CREATE, IMAGE_EMAIL_OPTMENU_NEW_ACC, 0, 0, PNULL},
    {EMAIL_SETTING_OPT_CREATE_ACC_ID_EX, TIP_NULL, {TXT_NULL, TXT_NULL, STXT_RETURN},
    TXT_EMAIL_CREATE, 0, 0, 0, PNULL},
};
#else
{
    //     {EMAIL_SETTING_OPT_SEND_RECEIVE_MAIL_ID,    TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN},
    //         TXT_COMMON_RECV_EMAIL,        0, 0, 0, MENU_NULL},
    {EMAIL_SETTING_OPT_CREATE_ACC_ID, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},
    TXT_EMAIL_CREATE, 0, 0, 0, PNULL},
    {EMAIL_SETTING_OPT_CREATE_ACC_ID_EX, TIP_NULL, {TXT_NULL, TXT_NULL, STXT_RETURN},
    TXT_EMAIL_CREATE, 0, 0, 0, PNULL},
};
#endif

#define MENU_DEF(_TABLEID, _ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, \
    _ITEMCOUNT) \
_ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, _ITEMCOUNT, 

const GUIMENU_GROUP_T mmiemail_menu_table[] = 
{
    {NULL},
#include "mmiemail_menutable.def"
};

#undef MENU_DEF

/**************************************************************************************/
// Description : Register email menu group.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC void MMIEMAIL_RegMenuGroup(void)
{
    MMI_RegMenuGroup(MMI_MODULE_EMAIL, mmiemail_menu_table);
}


