/*****************************************************************************
** File Name:      mmimail_id.h                                              *
** Author:         liyuanyuan                                                *
** Date:           2010/05/07                                                *
** Copyright:      2010 Hisense, Incorporated. All Rights Reserved.          *
** Description:    This file is used to describe mail win and control id     *
******************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 2010/05/07     yuanl            Create                                  *
**                                                                         *
****************************************************************************/

#ifndef _MMIMAIL_ID_H_
#define _MMIMAIL_ID_H_ 

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
    MMIMAIL_START_WIN_ID = (MMI_MODULE_PUSHMAIL << 16),

#include "mmimail_id.def"

    MMIMAIL_END_WIN_ID
}MMIMAIL_WINDOW_ID_E;

#undef WIN_ID_DEF

// control ID
typedef enum
{
    MMIMAIL_CTRL_ID_START = MMIMAIL_END_WIN_ID, //(MMI_MODULE_MAIL << 16),

    MMIMAIL_WELCOME_TEXT_CTRL_ID,               // 欢迎界面textbox控件
    MMIMAIL_MAINMENU_CTRL_ID,                   // 邮箱列表主菜单
    MMIMAIL_MAINMENU_OPT_MENU_CTRL_ID,          // 邮箱列表选项菜单
    MMIMAIL_SYS_SETTING_MENU_CTRL_ID,           // 系统设置菜单
    MMIMAIL_HELP_TEXT_CTRL_ID,                  // 帮助界面text控件
    
    MMIMAIL_MAILBOX_TAB_CTRL_ID,                // 邮箱界面TAB控件
    MMIMAIL_INBOX_LISTBOX_CTRL_ID,              // 收件箱列表控件
    MMIMAIL_OUTBOX_LISTBOX_CTRL_ID,             // 发件箱列表控件
    MMIMAIL_SENTBOX_LISTBOX_CTRL_ID,            // 已发件箱列表控件
    MMIMAIL_DRAFTBOX_LISTBOX_CTRL_ID,           // 草稿箱列表控件

    MMIMAIL_INBOX_OPT_MENU_CTRL_ID,              // 收件箱选项菜单
    MMIMAIL_OUTBOX_OPT_MENU_CTRL_ID,             // 发件箱选项菜单
    MMIMAIL_OUTBOX_OPT_EXT_MENU_CTRL_ID,         // 发件箱选项菜单/取消发送
    MMIMAIL_SENTBOX_OPT_MENU_CTRL_ID,            // 已发件箱选项菜单
    MMIMAIL_DRAFTBOX_OPT_MENU_CTRL_ID,           // 草稿箱选项菜单
    
    MMIMAIL_READ_TEXT_CTRL_ID,                   // 邮件阅读界面richtext控件
    MMIMAIL_READ_OPT_MENU_CTRL_ID,               // 邮件阅读界面选项菜单控件
    MMIMAIL_READ_ACCESSORY_LISTBOX_CTRL_ID,      // 阅读邮件附件列表控件
    MMIMAIL_READ_ACC_OPT_MENU_CTRL_ID,           // 附件列表选项菜单
    MMIMAIL_READ_ALL_CONTENT_TEXT_CTRL_ID,       // 显示全部正文text控件
    MMIMAIL_READ_ALL_RECVER_LISTBOX_CTRL_ID,     // 显示全部收件人列表控件
    MMIMAIL_READ_RECV_LIST_OPT_MENU_CTRL_ID,     // 收件人列表界面选项菜单
    MMIMAIL_READ_SEL_GROUP_MENU_CTRL_ID,         // 选择分组菜单控件
    MMIMAIL_PICTURE_PREVIEW_CTRL_ID,             // 附件为图片的预览窗口

    MMIMAIL_EDIT_RICHTEXT_CTRL_ID,               // 邮件编辑界面richtext控件
    MMIMAIL_EDIT_OPT_MENU_CTRL_ID,               // 编辑界面选项菜单
    MMIMAIL_EDIT_RECVER_LIST_CTRL_ID,            // 收件人列表编辑界面edit list控件
    MMIMAIL_EDIT_TEXT_CTRL_ID,                   // 编辑文本界面
    MMIMAIL_EDIT_ACC_LISTBOX_CTRL_ID,            // 编辑附件列表控件
    MMIMAIL_EDIT_ACC_OPT_MENU_CTRL_ID,           // 编辑附件选项菜单

    MMIMAIL_SET_ACCOUNT_LISTBOX_CTRL_ID,        // 邮箱帐号设置列表控件
    MMIMAIL_SET_ACCOUNT_OPT_MENU_CTRL_ID,       // 邮箱帐号设置选项菜单
    MMIMAIL_EDIT_NAME_LABEL_CTRL_ID,            // 编辑帐号名称label
    MMIMAIL_EDIT_NAME_EDIT_CTRL_ID,             // 编辑帐号名称edit
    MMIMAIL_EDIT_ADDR_LABEL_CTRL_ID,            // 编辑帐号地址label
    MMIMAIL_EDIT_ADDR_EDIT_CTRL_ID,             // 编辑帐号地址edit
    MMIMAIL_EDIT_USERNAME_LABEL_CTRL_ID,        // 编辑用户名称label
    MMIMAIL_EDIT_USERNAME_EDIT_CTRL_ID,         // 编辑用户名称edit
    MMIMAIL_EDIT_PASSWORD_LABEL_CTRL_ID,        // 编辑密码label
    MMIMAIL_EDIT_PASSWORD_EDIT_CTRL_ID,         // 编辑密码edit
    
    MMIMAIL_SET_MENU_CTRL_ID,                   // 设置界面菜单控件
    MMIMAIL_SET_LISTBOX_CTRL_ID,                // 设置界面列表控件
    MMIMAIL_SET_RICHTEXT_CTRL_ID,               // 设置界面richtext控件
    MMIMAIL_SET_TIMEEDIT_CTRL_ID,               // 设置静音时段编辑控件
    
    MMIMAIL_MAX_CTRL_ID
}MMIMAIL_CONTROL_ID_E;

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : Register pushmail menu group
//  Global resource dependence : none
//  Author: yuanl
//  Note:
/*****************************************************************************/
PUBLIC void MMIMAIL_RegWinIdNameArr(void);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif //_MMIMAIL_ID_H_
