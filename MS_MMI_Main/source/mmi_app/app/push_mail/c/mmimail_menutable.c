/****************************************************************************
** File Name:      mail_welcom.c                                            *
** Author:                                                                    *
** Date:           11/10/2010                                                *
** Copyright:      2004 Spreadtrum, Incorporated. All Rights Reserved.        *
** Description:                                                               *
*****************************************************************************
**                         Important Edit History                            *
** -------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                                *
** 11/2010      Jiaoyou.wu        Create
** 
****************************************************************************/
#define _MMIMAIL_MENUTABLE_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
 #ifdef WIN32
#include "std_header.h"
#endif
#include "window_parse.h"
#include "sci_types.h"
#include "mmi_module.h"
#include "guimenu.h"

#include "mmi_menutable.h"
#include "mmi_text.h"
#include "mmi_common.h"
#include "mmi_nv.h"

#include "mmimail_text.h"
#include "mmimail_menutable.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#ifdef TIP_NULL
#undef TIP_NULL
#endif
#define TIP_NULL 0

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/
//邮箱列表界面选项菜单
const GUIMENU_ITEM_T menu_mail_mainmenu_opt[] =
{
    {MMIMAIL_MENU_SYS_SETTING_ITEM_ID, TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
        TXT_SYSTEM_SETTING,    0, 0, 0, MENU_NULL},
    {MMIMAIL_MENU_HELP_ITEM_ID,        TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
        TXT_HELP,            0, 0, 0, MENU_NULL},
#if 0        
//sunguochen add for test 
    {MMIMAIL_MENU_RECV_ITEM_ID,        TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
        TXT_MAIL_RECEIVE,            0, 0, 0, MENU_NULL},        
//addend    
#endif
};

//系统设置菜单
const GUIMENU_ITEM_T menu_mail_sys_setting[] =
{
    {MMIMAIL_MENU_ACCOUNT_SETTING_ITEM_ID, TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
        TXT_MAIL_ACCOUNT_SETTING,    0, 0, 0, MENU_NULL},
    {MMIMAIL_MENU_EDIT_OPTION_ITEM_ID,     TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
        TXT_MAIL_EDIT_OPTION,        0, 0, 0, MENU_NULL},
    {MMIMAIL_MENU_SILENT_TIME_ITEM_ID,     TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
        TXT_COMMON_SLIENT_TIME,        0, 0, 0, MENU_NULL},
    {MMIMAIL_MENU_ATTACH_DL_OPTION_ITEM_ID,TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
        TXT_MAIL_ATTACH_DL_OPTION,   0, 0, 0, MENU_NULL},
    {MMIMAIL_MENU_DEL_ALL_MAILS_ITEM_ID,   TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
        TXT_MAIL_DEL_ALL,            0, 0, 0, MENU_NULL},
    {MMIMAIL_MENU_RESET_FACTORY_SETTING_ITEM_ID, TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
        TXT_COMMON_RESTORE_FACTORY_SETTINGS,              0, 0, 0, MENU_NULL},
    {MMIMAIL_MENU_LOGOUT_ITEM_ID, TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
        TXT_MAIL_LOGOUT,             0, 0, 0, MENU_NULL},
};

//编辑选项设置菜单
const GUIMENU_ITEM_T menu_mail_edit_opt_setting[] =
{
    {MMIMAIL_MENU_EDIT_OPT_INC_ORG_CONT_ITEM_ID, TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
        TXT_MAIL_INCLUDE_ORIGINAL,    0, 0, 0, MENU_NULL},
    {MMIMAIL_MENU_EDIT_OPT_SIGN_ITEM_ID,     TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
        TXT_COMMON_SIGNATURE,        0, 0, 0, MENU_NULL},
};

//附件下载选项设置菜单
const GUIMENU_ITEM_T menu_mail_acc_dl_setting[] =
{
#if 0//del by yuanl  2010.6.10
    {MMIMAIL_MENU_ACC_AUTO_DL_ITEM_ID, TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
        TXT_COMMON_AUTO_DOWNLOAD,    0, 0, 0, MENU_NULL},
#endif
    {MMIMAIL_MENU_ACC_AUTO_CHANGE_ITEM_ID, TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
        TXT_MAIL_AUTO_FORMAT_CHANGE,    0, 0, 0, MENU_NULL},
#if 0
    {MMIMAIL_MENU_ACC_SIZE_LIMIT_ITEM_ID,     TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
        TXT_MAIL_SIZE_LIMIT,        0, 0, 0, MENU_NULL},
#endif
};

//邮箱帐号设置菜单
const GUIMENU_ITEM_T menu_mail_set_account_opt[] =
{
    {MMIMAIL_MENU_OPT_EDIT_ITEM_ID,         TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
        TXT_EDIT,              0, 0, 0, MENU_NULL},
    {MMIMAIL_MENU_DEL_ITEM_ID,              TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
        TXT_DELETE,               0, 0, 0, MENU_NULL},
    {MMIMAIL_MENU_SET_DEF_ACCOUNT_ITEM_ID,  TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
        TXT_MAIL_SET_DEF_ACCOUNT,   0, 0, 0, MENU_NULL},
#if 0//del by yuanl  2010.6.10
    {MMIMAIL_MENU_SYNC_ACC_SET_ITEM_ID,     TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
        TXT_MAIL_SYNC_ACCOUNT_SETTING,  0, 0, 0, MENU_NULL},
#endif
};

//收件箱选项菜单
const GUIMENU_ITEM_T menu_mail_inbox_opt[] =
{
    {MMIMAIL_MENU_REPLY_ITEM_ID,            TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
        TXT_REPLY,             0, 0, 0, MENU_NULL},
    {MMIMAIL_MENU_REPLY_ALL_ITEM_ID,        TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
        TXT_MAIL_REPLY_ALL,         0, 0, 0, MENU_NULL},
    {MMIMAIL_MENU_FORWARD_CONTENT_ITEM_ID,          TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
        TXT_MAIL_FORWARD_CONTENT,   0, 0, 0, MENU_NULL},
    {MMIMAIL_MENU_SERVER_FORWARD_ACC_ITEM_ID,   TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
        TXT_MAIL_FORWARD_ACC,       0, 0, 0, MENU_NULL},
    {MMIMAIL_MENU_SERVER_FORWARD_ALL_ITEM_ID,   TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
        TXT_MAIL_FORWARD_ALL,       0, 0, 0, MENU_NULL},
    {MMIMAIL_MENU_NEW_MAIL_ITEM_ID,         TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
        TXT_MAIL_NEW,               0, 0, 0, MENU_NULL},
    {MMIMAIL_MENU_DEL_ITEM_ID,              TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
        TXT_DELETE,               0, 0, 0, MENU_NULL},
    {MMIMAIL_MENU_MARK_ITEM_ID,             TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
        STXT_MARK,              0, 0, 0, MENU_MAIL_MARK_OPT},
    {MMIMAIL_MENU_DEL_ALL_ITEM_ID,          TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
        TXT_DELALL,0, 0, 0, MENU_NULL},
    {MMIMAIL_MENU_RECEIVE_ITEM_ID,          TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
        TXT_MAIL_RECEIVE,           0, 0, 0, MENU_NULL},
};

//发件箱选项菜单
const GUIMENU_ITEM_T menu_mail_outbox_opt[] =
{
    {MMIMAIL_MENU_OPT_SEND_ITEM_ID,         TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
        TXT_SEND,              0, 0, 0, MENU_NULL},
    {MMIMAIL_MENU_OPT_EDIT_ITEM_ID,         TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
        TXT_EDIT,              0, 0, 0, MENU_NULL},
    {MMIMAIL_MENU_DEL_ITEM_ID,              TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
        TXT_DELETE,               0, 0, 0, MENU_NULL},
    {MMIMAIL_MENU_MARK_ITEM_ID,             TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
        STXT_MARK,              0, 0, 0, MENU_MAIL_MARK_OPT},
    {MMIMAIL_MENU_DEL_ALL_ITEM_ID,          TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
        TXT_DELALL,0, 0, 0, MENU_NULL},
};

const GUIMENU_ITEM_T menu_mail_outbox_opt_ext[] =
{
    {MMIMAIL_MENU_OPT_CANCEL_SEND_ITEM_ID,  TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
        TXT_MAIL_CANCEL_SEND,       0, 0, 0, MENU_NULL},
};

//已发件箱选项菜单
const GUIMENU_ITEM_T menu_mail_sentbox_opt[] =
{
    {MMIMAIL_MENU_OPT_FORWARD_ITEM_ID,   TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
        TXT_COMMON_FORWARD,           0, 0, 0, MENU_NULL},
    {MMIMAIL_MENU_OPT_RESEND_ITEM_ID,         TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
        TXT_RESEND,        0, 0, 0, MENU_NULL},
    {MMIMAIL_MENU_DEL_ITEM_ID,              TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
        TXT_DELETE,               0, 0, 0, MENU_NULL},
    {MMIMAIL_MENU_MARK_ITEM_ID,             TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
        STXT_MARK,              0, 0, 0, MENU_MAIL_MARK_OPT},
    {MMIMAIL_MENU_DEL_ALL_ITEM_ID,          TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
        TXT_DELALL,0, 0, 0, MENU_NULL},
};

//草稿箱选项菜单
const GUIMENU_ITEM_T menu_mail_draftbox_opt[] =
{
    {MMIMAIL_MENU_OPT_EDIT_ITEM_ID,        TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
        TXT_EDIT,              0, 0, 0, MENU_NULL},
    {MMIMAIL_MENU_OPT_SEND_ITEM_ID,            TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
        TXT_SEND,              0, 0, 0, MENU_NULL},
    {MMIMAIL_MENU_DEL_ITEM_ID,              TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
        TXT_DELETE,               0, 0, 0, MENU_NULL},
    {MMIMAIL_MENU_MARK_ITEM_ID,             TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
        STXT_MARK,              0, 0, 0, MENU_MAIL_MARK_OPT},
    {MMIMAIL_MENU_DEL_ALL_ITEM_ID,          TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
        TXT_DELALL,0, 0, 0, MENU_NULL},
};

//标记子菜单
const GUIMENU_ITEM_T menu_mail_mark_opt[] =
{
    {MMIMAIL_MENU_MARK_ONE_ITEM_ID,         TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},
        STXT_MARK,              0, 0, 0, MENU_NULL},
    {MMIMAIL_MENU_CANCEL_MARK_ONE_ITEM_ID,  TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},
        TXT_CANCEL_MARK,       0, 0, 0, MENU_NULL},
    {MMIMAIL_MENU_MARK_ALL_ITEM_ID,         TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},
        TXT_MARK_ALL,          0, 0, 0, MENU_NULL},
    {MMIMAIL_MENU_CANCEL_MARK_ALL_ITEM_ID,  TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},
        TXT_CANCEL_MARK_ALL,   0, 0, 0, MENU_NULL},
};

//编辑界面选项菜单
const GUIMENU_ITEM_T menu_mail_edit_opt[] =
{
    {MMIMAIL_MENU_EDIT_SEND_ITEM_ID,         TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},
        TXT_SEND,              0, 0, 0, MENU_NULL},
    {MMIMAIL_MENU_EDIT_SAVE_ITEM_ID,         TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},
        STXT_SAVE,              0, 0, 0, MENU_NULL},
    {MMIMAIL_MENU_EDIT_CANCEL_ITEM_ID,       TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},
        STXT_CANCEL,                0, 0, 0, MENU_NULL},
};

//收件人列表界面选项菜单
const GUIMENU_ITEM_T menu_mail_recv_list_opt[] =
{
    {MMIMAIL_MENU_SAVETO_NEW_ITEM_ID,         TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},
        TXT_SAVE_TO_NEWCONTACT,              0, 0, 0, MENU_NULL},
#ifdef MMIPB_ADD_EXIST_CONTACT_SUPPORT
    {MMIMAIL_MENU_SAVETO_EXIST_ITEM_ID,         TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},
        TXT_SAVE_TO_EXSISTEDCONTACT,              0, 0, 0, MENU_NULL},
#endif
};

//选择分组菜单
const GUIMENU_ITEM_T menu_mail_sel_group[] =
{
    {MMIMAIL_MENU_PHONE_ITEM_ID,         TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},
        TXT_PHONE,              0, 0, 0, MENU_NULL},
    {MMIMAIL_MENU_SIM_ITEM_ID,         TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},
        TXT_COMM_SIM,              0, 0, 0, MENU_NULL},
};
#define MENU_DEF(_TABLEID, _ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, \
    _ITEMCOUNT) \
_ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, _ITEMCOUNT, 

const GUIMENU_GROUP_T mmimail_menu_table[] = 
{
    {NULL},
#include "mmimail_menutable.def"
};

#undef MENU_DEF

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         LOCAL FUNCTION DECLARE                           *
 **--------------------------------------------------------------------------*/ 

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
// 	Description : Register mail menu group
//	Global resource dependence : none
//  Author:yuanl
//	Note:
/*****************************************************************************/
PUBLIC void MMIMAIL_RegMenuGroup(void)
{
    MMI_RegMenuGroup(MMI_MODULE_PUSHMAIL, mmimail_menu_table);
}
