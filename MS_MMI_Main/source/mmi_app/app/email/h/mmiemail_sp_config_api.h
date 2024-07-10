/***************************************************************************************
** File Name:      mmiemail_sp_config_api.h                                            *
** Author:                                                                             *
** Date:           05/13/2011                                                          *
** Copyright:      2011 Spreadtrum, Incorporated. All Rights Reserved.                 *
** Description:    Head file of E-mail config. This File will gathers functions that   *
**         special handle email account config. It can create a new account, set the   *
**         account config, get the account config and so on. These functions don't     *
**         be changed by project changed.                                              *
****************************************************************************************
**                         Important Edit History                                      *
** ------------------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                                         *
** 05/2011                         create                                              *
**                                                                                     *
***************************************************************************************/
#ifndef _MMIEMAIL_SP_CONFIG_API_H_
#define _MMIEMAIL_SP_CONFIG_API_H_

/**------------------------------------------------------------------------------------*
**                         Include Files                                               *
**------------------------------------------------------------------------------------*/
#include "sci_types.h"
#include "ffs.h"
#include "mmiemail_utils.h"
#include "mmiemail_def.h"

/**------------------------------------------------------------------------------------*
**                         Macro Declaration                                           *
**------------------------------------------------------------------------------------*/
/*Account name length and number*/
#define ACCOUNT_NAME_MAX_NUM 5
/*user information.*/
#define SETTINGMASK_USERINFO_FULL                     0x00000000
#define SETTINGMASK_USERINFO_USERNME                  0x00000001
#define SETTINGMASK_USERINFO_EMLADDR                  0x00000002
#define SETTINGMASK_USERINFO_RPLYTOADDR               0x00000004
#define SETTINGMASK_USERINFO_USERID                   0x00000008
#define SETTINGMASK_USERINFO_USERPASSWORD             0x00000010
#define SETTINGMASK_USERINFO_SMTPUSERID               0x00000020
#define SETTINGMASK_USERINFO_SMTPUSERPASSWORD         0x00000040
typedef uint32 SETTINGMASK_USERINFO;
/*Incoming mail server*/
#define SETTINGMASK_INCOMINGMAILSERV_FULL             0x00000000
#define SETTINGMASK_INCOMINGMAILSERV_TYPE             0x00000001
#define SETTINGMASK_INCOMINGMAILSERV_NAME             0x00000002
#define SETTINGMASK_INCOMINGMAILSERV_PORT             0x00000004
#define SETTINGMASK_INCOMINGMAILSERV_USESSLORTLS      0x00000008
typedef uint32 SETTINGMASK_INCOMINGMAILSERV;
/*Outgoing mail server*/
#define SETTINGMASK_OUTGOINGMAILSERV_FULL             0x00000000
#define SETTINGMASK_OUTGOINGMAILSERV_NAME             0x00000001
#define SETTINGMASK_OUTGOINGMAILSERV_PORT             0x00000002
#define SETTINGMASK_OUTGOINGMAILSERV_USESSLORTLS      0x00000004
#define SETTINGMASK_OUTGOINGMAILSERV_AUTHENTICATION   0x00000008
typedef uint32 SETTINGMASK_OUTGOINGMAILSERV;
/*Message preference*/
#define SETTINGMASK_MSGPREFERENCE_FULL                0x00000000
#define SETTINGMASK_MSGPREFERENCE_DOWNLOAD            0x00000001
#define SETTINGMASK_MSGPREFERENCE_MSGSIZE             0x00000002
#define SETTINGMASK_MSGPREFERENCE_LEAVECOPYONSERV     0x00000004
#define SETTINGMASK_MSGPREFERENCE_DELETION            0x00000008
#define SETTINGMASK_MSGPREFERENCE_SAVESENTMSG         0x00000010
#define SETTINGMASK_MSGPREFERENCE_INCLUDEORIGINAL     0x00000020
#define SETTINGMASK_MSGPREFERENCE_NUMBERLIMIT         0x00000040
#define SETTINGMASK_MSGPREFERENCE_REQUSTREADRECPT     0x00000080
#define SETTINGMASK_MSGPREFERENCE_ADDSIGNMODE         0x00000100
#define SETTINGMASK_MSGPREFERENCE_RECEIVEUNSEEN       0x00000200
#define SETTINGMASK_MSGPREFERENCE_DOWNLOADATTACH      0x00000400
typedef uint32 SETTINGMASK_MSGPREFERENCE;
/*Signatures*/
#define EMA_DOWNLOAD_ONLY_HEADERS        0x01
#define EMA_DOWNLOAD_FULL_MESSAGE        0x02
#define EMA_DOWNLOAD_ASK_USER            0x03
typedef uint32 EMAIL_DOWNLOAD_MODE;

#define EMA_ADD_SIGNATURE_NO_EMAIL       0x00
#define EMA_ADD_SIGNATURE_ONLY_NEW_EMAIL 0x01
#define EMA_ADD_SIGNATURE_ALL_EMAIL      0x02
typedef uint32 EMA_ADD_SIGNATURE_MODE;

/**------------------------------------------------------------------------------------*
**                         TYPE AND CONSTANT                                           *
**------------------------------------------------------------------------------------*/
typedef enum
{
    E_EMAILA_NOT_ACCOUNT = 0x00000001,
    E_EMAILA_EXSIT_ACCOUNT = 0x000000002,
    E_EMAILA_MAX_ACCOUNT = 0x000000003
}EMAIL_ACCOUNT_STATE_E;

typedef struct email_act_userinfo_tag
{
    wchar *name_ptr;
    wchar *addr_ptr;
    wchar *reply_addr_ptr;
    wchar *id_ptr;
    wchar *password_ptr;
    wchar *smtp_id_ptr;
    wchar *smtp_password_ptr;
}EMAIL_ACT_USERINFO_T;

typedef struct email_act_pop3mailserv_tag
{
    uint32  serv_type;
    wchar   *serv_name_ptr;
    wchar   *port_ptr;
    BOOLEAN is_use_ssl;
    char    acres[3];
}EMAIL_ACT_POP3MAILSERV_T;

typedef struct email_act_imap4mailserv_tag
{
    uint32  serv_type;
    wchar   *serv_name_ptr;
    wchar   *port_ptr;
    BOOLEAN is_use_ssl;
    char    acres[3];
}EMAIL_ACT_IMAP4MAILSERV_T;

typedef struct email_act_out_mailserv_tag
{
    wchar   *serv_name_ptr;
    wchar   *port_ptr;
    BOOLEAN is_use_ssl;
    BOOLEAN is_need_authentication;
    char    acres[2];
}EMAIL_ACT_OUT_MAILSERV_T;

typedef struct email_act_msgpreference_tag
{
    EMAIL_DOWNLOAD_MODE  download_mode;
    wchar                *email_size_ptr;
    uint32               limit_num;
    EMA_ADD_SIGNATURE_MODE signature_mode;
    BOOLEAN is_copy_onserv;
    BOOLEAN is_del_onserv;
    BOOLEAN is_save_sentemail;
    BOOLEAN is_include_original;
    BOOLEAN is_receive_rr;
    BOOLEAN is_receive_uuseen;
    BOOLEAN is_download_attach;
    char    acres;
}EMAIL_ACT_MSGPREFERENCE_T;

typedef struct email_act_setting_tag 
{
    uint32                active_act_id;
    EMAIL_ACCOUNT_STATE_E act_state;
    EMAIL_MAILSERVTYPE_E  in_mailserv_type;
}EMAIL_ACT_SETTING_T;

typedef void* HConfigMgr;

/**------------------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                                         *
**------------------------------------------------------------------------------------*/
/**************************************************************************************/
// Description : email sp, to init config handle.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC int32 MMIEMAIL_SP_CFG_InitHandle(
                                        HConfigMgr *cfg_handle_pptr,
                                        void       *parent_handle_ptr
                                        );
/**************************************************************************************/
// Description : email sp, to delete config handle.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC int32 MMIEMAIL_SP_CFG_DeleteHandle(HConfigMgr *cfg_handle_pptr);
/**************************************************************************************/
// Description : email sp, create account.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC int32 MMIEMAIL_SP_CFG_CreateAccount(
                                           HConfigMgr cfg_handle_ptr, 
                                           wchar *act_name_ptr,
                                           uint32 *act_id_ptr,
                                           EMAIL_ACCOUNT_STATE_E act_state
                                           );
/**************************************************************************************/
// Description : email sp, set user info date.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC int32 MMIEMAIL_SP_CFG_SetUserInfoData(
                                             HConfigMgr cfg_handle_ptr,
                                             uint32 act_id,
                                             uint32 mask,
                                             EMAIL_ACT_USERINFO_T *user_info_ptr
                                             );
/**************************************************************************************/
// Description : email sp, set come in email servece date , pop3.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC int32 MMIEMAIL_SP_CFG_SetIncmPOP3MaiLServData(
                                                     HConfigMgr cfg_handle_ptr, 
                                                     uint32 act_id, 
                                                     uint32 mask, 
                                                     EMAIL_ACT_POP3MAILSERV_T *pop3_mail_serv_ptr
                                                     );
/**************************************************************************************/
// Description : email sp, set come in email servece date , imap4.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC int32 MMIEMAIL_SP_CFG_SetIncmIMAP4MaiLServData(
                                                      HConfigMgr cfg_handle_ptr, 
                                                      uint32 act_id,
                                                      uint32 mask, 
                                                      EMAIL_ACT_IMAP4MAILSERV_T *imap4_mail_serv_ptr
                                                      );
/**************************************************************************************/
// Description : email sp, set out email servece date , pop3.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC int32 MMIEMAIL_SP_CFG_SetOutgoMaiLServData(
                                                  HConfigMgr cfg_handle_ptr, 
                                                  uint32 act_id, 
                                                  uint32 mask, 
                                                  EMAIL_ACT_OUT_MAILSERV_T *send_serv_ptr
                                                  );
/**************************************************************************************/
// Description : email sp, set set email preference.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC int32 MMIEMAIL_SP_CFG_SetEmailPreference(
                                              HConfigMgr cfg_handle_ptr, 
                                              uint32 act_id, 
                                              uint32 mask, 
                                              EMAIL_ACT_MSGPREFERENCE_T *email_prf_ptr
                                              );
/**************************************************************************************/
// Description : email sp, get user info date.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC int32 MMIEMAIL_SP_CFG_GetUserInfoData(
                                             HConfigMgr cfg_handle_ptr,
                                             uint32 act_id,
                                             EMAIL_ACT_USERINFO_T *user_info_ptr
                                             );
/**************************************************************************************/
// Description : email sp, get pop3 servece date.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC int32 MMIEMAIL_SP_CFG_GetIncmPOP3MaiLServData(
                                                     HConfigMgr cfg_handle_ptr, 
                                                     uint32 act_id, 
                                                     EMAIL_ACT_POP3MAILSERV_T *pop3_mail_serv_ptr
                                                     );
/**************************************************************************************/
// Description : email sp, get imap4 servece date.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC int32 MMIEMAIL_SP_CFG_GetIncmIMAP4MaiLServData(
                                                      HConfigMgr cfg_handle_ptr, 
                                                      uint32 act_id, 
                                                      EMAIL_ACT_IMAP4MAILSERV_T *imap4_mail_serv_ptr
                                                      );
/**************************************************************************************/
// Description : email sp, get sign dir.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC int32 MMIEMAIL_SP_CFG_GetSignDir(
                                        HConfigMgr cfg_handle_ptr, 
                                        uint32 act_id, 
                                        char** sig_dir_pptr
                                        );
/**************************************************************************************/
// Description : email sp, get out mail servece date.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC int32 MMIEMAIL_SP_CFG_GetOutgoMaiLServData(
                                                  HConfigMgr cfg_handle_ptr, 
                                                  uint32 act_id,  
                                                  EMAIL_ACT_OUT_MAILSERV_T *send_serv_ptr
                                                  );
/**************************************************************************************/
// Description : email sp, get msg preference.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC int32 MMIEMAIL_SP_CFG_GetEmailPreference(
                                              HConfigMgr cfg_handle_ptr, 
                                              uint32 act_id, 
                                              EMAIL_ACT_MSGPREFERENCE_T *email_prf_ptr
                                              );
/**************************************************************************************/
// Description : email sp, delete account.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC int32 MMIEMAIL_SP_CFG_DeleteAccount(
                                           HConfigMgr cfg_handle_ptr,
                                           uint32 act_id,
                                           BOOLEAN is_del_all_act
                                           );
/**************************************************************************************/
// Description : email sp, get active account.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC int32 MMIEMAIL_SP_CFG_GetActiveAct(
                                          HConfigMgr cfg_handle_ptr,
                                          uint32 *act_id_ptr
                                          );
/**************************************************************************************/
// Description : email sp, Set active account in file.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC int32 MMIEMAIL_SP_CFG_SetActiveAct(
                                          HConfigMgr cfg_handle_ptr,
                                          uint32 act_id
                                          );
/**************************************************************************************/
// Description : email sp, Release struct EMAIL_ACT_USERINFO_T.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC int32 MMIEMAIL_SP_CFG_DEL_ActUserInfo(
                                             HConfigMgr cfg_handle_ptr,
                                             EMAIL_ACT_USERINFO_T *user_into_ptr
                                             );
/**************************************************************************************/
// Description : email sp, Release struct EMAIL_ACT_POP3MAILSERV_T.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC int32 MMIEMAIL_SP_CFG_DEL_IncmPOP3MailServ(
                                                  HConfigMgr cfg_handle_ptr,
                                                  EMAIL_ACT_POP3MAILSERV_T *pop3_mail_serv_ptr
                                                  );
/**************************************************************************************/
// Description : email sp, Release struct EMAIL_ACT_IMAP4MAILSERV_T.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC int32 MMIEMAIL_SP_CFG_DEL_IncmIMAP4MailServ(
                                                   HConfigMgr cfg_handle_ptr,
                                                   EMAIL_ACT_IMAP4MAILSERV_T *imap4_mail_serv_ptr
                                                   );
/**************************************************************************************/
// Description : email sp, Release struct EMAIL_ACT_OUT_MAILSERV_T.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC int32 MMIEMAIL_SP_CFG_DEL_OutgoMailServ(
                                               HConfigMgr cfg_handle_ptr,
                                               EMAIL_ACT_OUT_MAILSERV_T *mail_serv_ptr
                                               );
/**************************************************************************************/
// Description : email sp, Release struct EMAIL_ACT_MSGPREFERENCE_T.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC int32 MMIEMAIL_SP_CFG_DEL_EmailPreference(
                                               HConfigMgr cfg_handle_ptr,
                                               EMAIL_ACT_MSGPREFERENCE_T *email_prf_ptr
                                               );
/**************************************************************************************/
// Description : email sp, Exsit account.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC uint32 MMIEMAIL_SP_CFG_ExsitAct(HConfigMgr cfg_handle_ptr);
/**************************************************************************************/
// Description : email sp, account's receive type.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC uint32 MMIEMAIL_SP_CFG_ActRecType(HConfigMgr cfg_handle_ptr);
/**************************************************************************************/
// Description : email sp, Set default configure for account.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC int32 MMIEMAIL_SP_CFG_SetActDefaultCfg(
                                              HConfigMgr cfg_handle_ptr,
                                              uint32 act_id
                                              );
/**************************************************************************************/
// Description : email sp, get active profile.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC int32 MMIEMAIL_SP_CFG_GetActiveProfile(
                                              HConfigMgr cfg_handle_ptr,
                                              uint32* profile_id_ptr
                                              );
/**************************************************************************************/
// Description : email sp, set active profile.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC int32 MMIEMAIL_SP_CFG_SetActiveProfile(
                                              HConfigMgr cfg_handle_ptr,
                                              uint32 profile_id
                                              );
/**************************************************************************************/
// Description : email sp, check profile ext.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC int32 MMIEMAIL_SP_CFG_CheckProfileExt(
                                             HConfigMgr cfg_handle_ptr
                                             );
#endif /*_MMIEMAIL_SP_CONFIG_API_H_*/

