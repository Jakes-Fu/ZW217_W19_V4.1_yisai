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

    MMIMAIL_WELCOME_TEXT_CTRL_ID,               // ��ӭ����textbox�ؼ�
    MMIMAIL_MAINMENU_CTRL_ID,                   // �����б����˵�
    MMIMAIL_MAINMENU_OPT_MENU_CTRL_ID,          // �����б�ѡ��˵�
    MMIMAIL_SYS_SETTING_MENU_CTRL_ID,           // ϵͳ���ò˵�
    MMIMAIL_HELP_TEXT_CTRL_ID,                  // ��������text�ؼ�
    
    MMIMAIL_MAILBOX_TAB_CTRL_ID,                // �������TAB�ؼ�
    MMIMAIL_INBOX_LISTBOX_CTRL_ID,              // �ռ����б�ؼ�
    MMIMAIL_OUTBOX_LISTBOX_CTRL_ID,             // �������б�ؼ�
    MMIMAIL_SENTBOX_LISTBOX_CTRL_ID,            // �ѷ������б�ؼ�
    MMIMAIL_DRAFTBOX_LISTBOX_CTRL_ID,           // �ݸ����б�ؼ�

    MMIMAIL_INBOX_OPT_MENU_CTRL_ID,              // �ռ���ѡ��˵�
    MMIMAIL_OUTBOX_OPT_MENU_CTRL_ID,             // ������ѡ��˵�
    MMIMAIL_OUTBOX_OPT_EXT_MENU_CTRL_ID,         // ������ѡ��˵�/ȡ������
    MMIMAIL_SENTBOX_OPT_MENU_CTRL_ID,            // �ѷ�����ѡ��˵�
    MMIMAIL_DRAFTBOX_OPT_MENU_CTRL_ID,           // �ݸ���ѡ��˵�
    
    MMIMAIL_READ_TEXT_CTRL_ID,                   // �ʼ��Ķ�����richtext�ؼ�
    MMIMAIL_READ_OPT_MENU_CTRL_ID,               // �ʼ��Ķ�����ѡ��˵��ؼ�
    MMIMAIL_READ_ACCESSORY_LISTBOX_CTRL_ID,      // �Ķ��ʼ������б�ؼ�
    MMIMAIL_READ_ACC_OPT_MENU_CTRL_ID,           // �����б�ѡ��˵�
    MMIMAIL_READ_ALL_CONTENT_TEXT_CTRL_ID,       // ��ʾȫ������text�ؼ�
    MMIMAIL_READ_ALL_RECVER_LISTBOX_CTRL_ID,     // ��ʾȫ���ռ����б�ؼ�
    MMIMAIL_READ_RECV_LIST_OPT_MENU_CTRL_ID,     // �ռ����б����ѡ��˵�
    MMIMAIL_READ_SEL_GROUP_MENU_CTRL_ID,         // ѡ�����˵��ؼ�
    MMIMAIL_PICTURE_PREVIEW_CTRL_ID,             // ����ΪͼƬ��Ԥ������

    MMIMAIL_EDIT_RICHTEXT_CTRL_ID,               // �ʼ��༭����richtext�ؼ�
    MMIMAIL_EDIT_OPT_MENU_CTRL_ID,               // �༭����ѡ��˵�
    MMIMAIL_EDIT_RECVER_LIST_CTRL_ID,            // �ռ����б�༭����edit list�ؼ�
    MMIMAIL_EDIT_TEXT_CTRL_ID,                   // �༭�ı�����
    MMIMAIL_EDIT_ACC_LISTBOX_CTRL_ID,            // �༭�����б�ؼ�
    MMIMAIL_EDIT_ACC_OPT_MENU_CTRL_ID,           // �༭����ѡ��˵�

    MMIMAIL_SET_ACCOUNT_LISTBOX_CTRL_ID,        // �����ʺ������б�ؼ�
    MMIMAIL_SET_ACCOUNT_OPT_MENU_CTRL_ID,       // �����ʺ�����ѡ��˵�
    MMIMAIL_EDIT_NAME_LABEL_CTRL_ID,            // �༭�ʺ�����label
    MMIMAIL_EDIT_NAME_EDIT_CTRL_ID,             // �༭�ʺ�����edit
    MMIMAIL_EDIT_ADDR_LABEL_CTRL_ID,            // �༭�ʺŵ�ַlabel
    MMIMAIL_EDIT_ADDR_EDIT_CTRL_ID,             // �༭�ʺŵ�ַedit
    MMIMAIL_EDIT_USERNAME_LABEL_CTRL_ID,        // �༭�û�����label
    MMIMAIL_EDIT_USERNAME_EDIT_CTRL_ID,         // �༭�û�����edit
    MMIMAIL_EDIT_PASSWORD_LABEL_CTRL_ID,        // �༭����label
    MMIMAIL_EDIT_PASSWORD_EDIT_CTRL_ID,         // �༭����edit
    
    MMIMAIL_SET_MENU_CTRL_ID,                   // ���ý���˵��ؼ�
    MMIMAIL_SET_LISTBOX_CTRL_ID,                // ���ý����б�ؼ�
    MMIMAIL_SET_RICHTEXT_CTRL_ID,               // ���ý���richtext�ؼ�
    MMIMAIL_SET_TIMEEDIT_CTRL_ID,               // ���þ���ʱ�α༭�ؼ�
    
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
