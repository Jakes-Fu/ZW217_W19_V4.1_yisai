/***************************************************************************
** File Name:      mmimail_menutable.h                                     *
** Author:         liyuanyuan                                              *
** Date:           2010/05/07                                              *
** Copyright:      2010 Hisense, Incorporated. All Rights Reserved.        *
** Description:    This file is used to define menutable relative content  *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 2010/05/07     yuanl            Create                                  *
**                                                                         *
****************************************************************************/
#ifndef _MMIMAIL_MENUTABLE_H_
#define _MMIMAIL_MENUTABLE_H_

#include "mmi_module.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/*-------------------------------------------------------------------------*/
/*                         TYPES/CONSTANTS                                 */
/*-------------------------------------------------------------------------*/


//Menu ID
typedef enum
{
    MMIMAIL_MENU_MENU_START = (MMI_MODULE_PUSHMAIL << 16),
    
    //main menu id
    MMIMAIL_MENU_SYS_SETTING_ITEM_ID,
    MMIMAIL_MENU_HELP_ITEM_ID,
//sunguochen add for test 
    MMIMAIL_MENU_SEND_ITEM_ID,
    MMIMAIL_MENU_RECV_ITEM_ID,    
//end
    //system setting menu id
    MMIMAIL_MENU_ACCOUNT_SETTING_ITEM_ID,
    MMIMAIL_MENU_EDIT_OPTION_ITEM_ID,
    MMIMAIL_MENU_SILENT_TIME_ITEM_ID,
    MMIMAIL_MENU_ATTACH_DL_OPTION_ITEM_ID,
    MMIMAIL_MENU_DEL_ALL_MAILS_ITEM_ID,
    MMIMAIL_MENU_RESET_FACTORY_SETTING_ITEM_ID,
    MMIMAIL_MENU_LOGOUT_ITEM_ID,

    //account setting option menu id
    MMIMAIL_MENU_SET_DEF_ACCOUNT_ITEM_ID,
    MMIMAIL_MENU_SYNC_ACC_SET_ITEM_ID,

    //edit option setting menu id
    MMIMAIL_MENU_EDIT_OPT_INC_ORG_CONT_ITEM_ID,
    MMIMAIL_MENU_EDIT_OPT_SIGN_ITEM_ID,

    //acc dl option setting menu id
    MMIMAIL_MENU_ACC_AUTO_DL_ITEM_ID,
    MMIMAIL_MENU_ACC_AUTO_CHANGE_ITEM_ID,
    MMIMAIL_MENU_ACC_SIZE_LIMIT_ITEM_ID,
    
    //inbox option menu id
    MMIMAIL_MENU_REPLY_ITEM_ID,
    MMIMAIL_MENU_REPLY_ALL_ITEM_ID,
    MMIMAIL_MENU_FORWARD_CONTENT_ITEM_ID,
    MMIMAIL_MENU_SERVER_FORWARD_ACC_ITEM_ID,
    MMIMAIL_MENU_SERVER_FORWARD_ALL_ITEM_ID,
    MMIMAIL_MENU_NEW_MAIL_ITEM_ID,
    MMIMAIL_MENU_DEL_ITEM_ID,
    MMIMAIL_MENU_DEL_ALL_ITEM_ID,
    MMIMAIL_MENU_MARK_ITEM_ID,
    MMIMAIL_MENU_RECEIVE_ITEM_ID,

    //outbox option menu id
    MMIMAIL_MENU_OPT_CANCEL_SEND_ITEM_ID,
    MMIMAIL_MENU_OPT_SEND_ITEM_ID,
    MMIMAIL_MENU_OPT_EDIT_ITEM_ID,

    //sentbox option menu id
    MMIMAIL_MENU_OPT_FORWARD_ITEM_ID,
    MMIMAIL_MENU_OPT_RESEND_ITEM_ID,
    
    //mark option menu id
    MMIMAIL_MENU_MARK_ONE_ITEM_ID,
    MMIMAIL_MENU_CANCEL_MARK_ONE_ITEM_ID,
    MMIMAIL_MENU_MARK_ALL_ITEM_ID,
    MMIMAIL_MENU_CANCEL_MARK_ALL_ITEM_ID,

    //edit win option menu id
    MMIMAIL_MENU_EDIT_SEND_ITEM_ID,
    MMIMAIL_MENU_EDIT_SAVE_ITEM_ID,
    MMIMAIL_MENU_EDIT_CANCEL_ITEM_ID,

    //recver list win option menu id
    MMIMAIL_MENU_SAVETO_NEW_ITEM_ID,
    MMIMAIL_MENU_SAVETO_EXIST_ITEM_ID,

    //select group menu id
    MMIMAIL_MENU_PHONE_ITEM_ID,
    MMIMAIL_MENU_SIM_ITEM_ID,
    
    MMIMAIL_MENU_MENU_MAX_ITEM_ID
}MMISMS_ID_MENU_E;


#define MENU_DEF(_TABLEID, _ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, \
    _ITEMCOUNT) \
     _TABLEID,

typedef enum
{
    MMIMAIL_MENU_LABEL_START = (MMI_MODULE_PUSHMAIL << 16),

    #include "mmimail_menutable.def"

    MMIMAIL_MENUTABLE_MAX
} MMIMAIL_MENU_LABEL_E;

#undef MENU_DEF


/*****************************************************************************/
// 	Description : Register mail menu group
//	Global resource dependence : none
//  Author:yuanl
//	Note:
/*****************************************************************************/
PUBLIC void MMIMAIL_RegMenuGroup(void);

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
/*-------------------------------------------------------------------------*/

#endif //_MMIMAIL_MENUTABLE_H_


