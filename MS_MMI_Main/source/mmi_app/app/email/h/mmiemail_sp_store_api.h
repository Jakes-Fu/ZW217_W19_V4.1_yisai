/***************************************************************************************
** File Name:      mmiemail_sp_store_api.h                                             *
** Author:                                                                             *
** Date:           05/13/2011                                                          *
** Copyright:      2011 Spreadtrum, Incorporated. All Rights Reserved.                 *
** Description:    Head file of E-mail store. This File will gathers functions that    *
**       special handle email store. It will include save a email, update a email, get *
**       specify box information and so on. These functions don't be changed by project*
**       changed.                                                                      *
****************************************************************************************
**                         Important Edit History                                      *
** ------------------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                                         *
** 05/2011                         create                                              *
**                                                                                     *
***************************************************************************************/
#ifndef _MMIEMAIL_SP_STORE_API_H_
#define _MMIEMAIL_SP_STORE_API_H_

/**------------------------------------------------------------------------------------*
**                         Include Files                                               *
**------------------------------------------------------------------------------------*/
//#include "AEEVectorModel.h"
#include "mmiemail_vector.h"
#include "sci_types.h"

#include "mmiemail_def.h"
#include "EMS_Store_API.h"

/**------------------------------------------------------------------------------------*
**                         Macro Declaration                                           *
**------------------------------------------------------------------------------------*/
#define EMASP_ACCOUNT_MAX_ID                      0xffff
#define EMASP_BOX_MAX_ID                          0xffff
#define EMASP_ACTINFO_MAX_NUMBER                  0xffff
#define EMASP_MSGBASEINFO_MAX_NUMBER              0xffff

#define QEE_INVALID_MSG_ID                        0

/*.act file*/
//#define EMASP_ACCOUNT_NAME_MAX_LENGTH             64  
#define EMASP_ACCOUNT_CFGINFO_MAX                 0xff

/* Define ENUM Type Start */
/* Field mask */
#define  EMAILA_MSGMASK_FULL                      0x00000000
#define  EMAILA_MSGMASK_SIZE                      0x00000001
#define  EMAILA_MSGMASK_TOADDR                    0x00000002
#define  EMAILA_MSGMASK_CCADDR                    0x00000004
#define  EMAILA_MSGMASK_BCCADDR                   0x00000008
#define  EMAILA_MSGMASK_SUBJ                      0x00000010
#define  EMAILA_MSGMASK_ATTACHMENT                0x00000020
#define  EMAILA_MSGMASK_STATE                     0x00000040
#define  EMAILA_MSGMASK_FOLDER                    0x00000080
#define  EMAILA_MSGMASK_UNREAD                    0x00000100
#define  EMAILA_MSGMASK_FROMADDR                  0x00000200
typedef uint32 EMAILA_MSGMAK;

typedef uint32 EMASP_ACCOUNT_ID;

#define EMASP_INBOX_INDEX                         0x01
#define EMASP_OUTBOX_INDEX                        0x02
#define EMASP_SENTBOX_INDEX                       0x03
#define EMASP_DRAFT_INDEX                         0x04
typedef uint32 EMASP_BOX_INDEX;

/*userinfo*/
#define EMASP_PRFITEM_ACCOUNT_NAME                0x01   /*AECHAR*/
#define EMASP_PRFITEM_ACCOUNT_EMAIL               0x02   /*AECHAR*/
#define EMASP_PRFITEM_REPLY_TO_EMAIL              0x03   /*AECHAR*/
#define EMASP_PRFITEM_EMAIL_USER_ID               0x04   /*AECHAR*/
#define EMASP_PRFITEM_EMAIL_PASSWORD              0x05   /*AECHAR*/
#define EMASP_PRFITEM_SMTP_USER_ID                0x06   /*AECHAR*/	
#define EMASP_PRFITEM_SMTP_PASSWORD               0x07   /*AECHAR*/

/*connect info*/
#define EMASP_PRFITEM_DATA_SERVICE_PROVIDER       0x08 /*uint32*/
#define EMASP_PRFITEM_RECEIVE_SERVER_TYPE         0x09 /*uint32*/
#define EMASP_PRFITEM_POP3_SERVER                 0x0A /*AECHAR*/
#define EMASP_PRFITEM_POP3_PORT                   0x0B /*AECHAR*/
#define EMASP_PRFITEM_POP3_SSL_OR_TLS             0x0C /*boolean*/
#define EMASP_PRFITEM_IMAP4_SERVER                0x0D /*AECHAR*/
#define EMASP_PRFITEM_IMAP4_SERVER_PORT           0x0E /*AECHAR*/
#define EMASP_PRFITEM_IMAP4_SSL_OR_TLS            0x0F /*boolean*/
#define EMASP_PRFITEM_SMTP_SERVER                 0x10 /*AECHAR*/
#define EMASP_PRFITEM_SMTP_SERVER_PORT            0x11 /*AECHAR*/
#define EMASP_PRFITEM_SMTP_SSL_OR_TLS             0x12 /*boolean*/
#define EMASP_PRFITEM_SMTP_AUTHENTICATE           0x13 /*boolean*/

/*message preferences*/
#define EMASP_PRFITEM_EMAIL_DOWNLOAD_MODE         0x14 /*uint32*/
#define EMASP_PRFITEM_NEW_MSG_NOTIFICATION        0x15 /*boolean*/
#define EMASP_PRFITEM_MAX_MSG_SIZE                0x16 /*AECHAR*/
#define EMASP_PRFITEM_RECEIVER_LEAVE_COPY         0x17 /*boolean*/
#define EMASP_PRFITEM_DELETE_LEAVE_COPY           0x18 /*boolean*/
#define EMASP_PRFITEM_SAVE_SENT                   0x19 /*boolean*/
#define EMASP_PRFITEM_INCLUDE_ORIGINAL            0x1A /*boolean*/
#define EMASP_PRFITEM_NUMBER_LIMIT                0x1B /*uint32*/
#define EMASP_PRFITEM_READ_RECEIPT                0x1C /*boolean*/
#define EMASP_PRFITEM_EMAIL_ADD_SIGNATURE_MODE    0x1D /*uint32*/
#define EMASP_PRFITEM_EMAIL_CREATE_SIGNATURE      0x1E /*AECHAR*/
#define EMASP_PRFITEM_EMAIL_DEFAULTINBOXID        0x1F /*uint32*/
#define EMASP_PRFILEM_EMAIL_UIDLFILEDIR           0x20  /*char*/
#define EMASP_PRFILEM_EMAIL_UIDFILEDIR            0x21  /*char*/
#define EMASP_PRFITEM_RECEIVE_UNSEEN              0x22 /*boolean*/
#define EMASP_PRFITEM_DOWNLOAD_ATTACH             0x23 /*boolean*/
#define EMASP_PRFILEM_USER_NAME                   0x24  /*AECHAR*/

#define EMASP_PRFITEM_USER1_PROFILE               0x25
#define EMASP_PRFITEM_USER2_PROFILE               0x26
#define EMASP_PRFITEM_USER3_PROFILE               0x27

//added by eva
#define EMASP_PRFITEM_NET_PATH                    0x28 /*boolean*/
#define EMASP_PRFITEM_POP3_PORT_DEFAULT           0x29 /*boolean*/
#define EMASP_PRFITEM_IMAP4_PORT_DEFAULT          0x2A /*boolean*/
#define EMASP_PRFITEM_SMTP_PORT_DEFAULT           0x2B /*boolean*/


#define EMASP_PRFITEM_SEND_MAIL                   0x2C /*boolean*/
#define EMASP_PRFITEM_SENDER_NAME                 0x2D /*wchar*/
#define EMASP_PRFITEM_RECEIVE_FREQUENCY           0x2E /*boolean*/
#define EMASP_PRFITEM_RECEIVE_ONLY_NEW_MAIL       0x2F /*boolean*/
#define EMASP_PRFITEM_MAIL_AUTO_DELETE            0x31 /*boolean*/
#define EMASP_PRFITEM_MAIL_DELETE                 0x32 /*boolean*/
#define EMASP_PRFITEM_NEW_MAIL_TIP                0x33 /*boolean*/

#define EMASP_PRFITEM_STARTUP_SCREEN               0x34 /*boolean*/
#define EMASP_PRFITEM_MAIL_STORAGE                 0x35 /*boolean*/
#define EMASP_PRFITEM_TONE                         0x36 /*boolean*/
#define EMASP_PRFITEM_TONE_VOLUME                  0x37 /*boolean*/
#define EMASP_PRFITEM_SLIENT_TIME                  0x38 /*boolean*/
#define EMASP_PRFITEM_ACCOUNT_PASSWORD_PROTECTION  0x39 /*boolean*/
#define EMASP_PRFITEM_SET_PASSWORD_PROTECTION      0x3A /*boolean*/
#define EMASP_PRFITEM_STORAGE_STATISTICS           0x3B /*boolean*/
#define EMASP_PRFITEM_FLOW_STATISTICS              0x3C /*boolean*/
typedef uint32 EMASP_PROFILE_ITEM;

//PersonalizedSetting
//download num
#define EMASP_DOWN_NUM_25                          0x00
#define EMASP_DOWN_NUM_50                          0x01
#define EMASP_DOWN_NUM_100                         0x02
#define EMASP_DOWN_NUM_TOTAL                       0x03

//receive limit
//define in emaila_sp_config_api.h

//DataConnection
//net_path
#if defined(MMI_MULTI_SIM_SYS_SINGLE)
#define EMASP_NET_PATH_SIM1                       0x00
#define EMASP_NET_PATH_WIFI                       0x01
#elif defined(MMI_MULTI_SIM_SYS_DUAL)
#define EMASP_NET_PATH_SIM1                       0x00
#define EMASP_NET_PATH_SIM2                       0x01
#define EMASP_NET_PATH_WIFI                       0x02
#elif defined(MMI_MULTI_SIM_SYS_TRI) 
#define EMASP_NET_PATH_SIM1                       0x00
#define EMASP_NET_PATH_SIM2                       0x01
#define EMASP_NET_PATH_SIM3                       0x02
#define EMASP_NET_PATH_WIFI                       0x03       
#elif defined(MMI_MULTI_SIM_SYS_QUAD) 
#define EMASP_NET_PATH_SIM1                       0x00
#define EMASP_NET_PATH_SIM2                       0x01
#define EMASP_NET_PATH_SIM3                       0x02
#define EMASP_NET_PATH_SIM4                       0x03
#define EMASP_NET_PATH_WIFI                       0x04
#endif


//ServerSetting
//server type
#define EMASP_POP3                                0x01
#define EMASP_IMAP4                               0x02

//port
#define EMASP_PORT_DEFAULT                        0x00
#define EMASP_PORT_CUSTOMIZE                      0x01

//security
#define EMASP_SECURITY_SSLTLS                     0x00
#define EMASP_SECURITY_STARTTLS                   0x01
#define EMASP_SECURITY_OFF                        0x02

//Authentication
#define EMASP_AUTHENTICATE_NOTHING                0x00
#define EMASP_AUTHENTICATE_USERNAME_PWD           0x01 
#define EMASP_AUTHENTICATE_SSL                    0x02 
#define EMASP_AUTHENTICATE_USERNAME_PWD_SSL       0x03 


#define EMASP_BOX_STATE_LOCAL                     0x01
#define EMASP_BOX_STATE_ON_SERVER                 0x02
#define EMASP_BOX_STATE_SUB_ON_SERVER             0x04
typedef unsigned char EMASP_BOX_STATE;

/**------------------------------------------------------------------------------------*
**                         TYPE AND CONSTANT                                           *
**------------------------------------------------------------------------------------*/
typedef EMS_PRFITEM_VAL EMASP_PRFITEM_VAL;
typedef EMSMsgOpt       EMASP_MSG_OPT;

#define EMASPMSGOPT_END   EMSMSGOPT_END

typedef struct emasp_actinfo_tag
{
    uint32 act_id;
    uint32 email_num;
    wchar  act_name[EMS_ACCOUNT_NAME_MAX_LEN + 1];
}EMASP_ACTINFO_T;

typedef void* HStoreMgr;

/**------------------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                                         *
**------------------------------------------------------------------------------------*/
/**************************************************************************************/
// Description : email sp store, to init handle.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC int32 MMIEMAIL_SP_Store_InitHandle(
                                          HStoreMgr        *sp_handle_pptr,
                                          void             *parent_handle_ptr
                                          );
/**************************************************************************************/
// Description : email sp store, to delete handle.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC void MMIEMAIL_SP_Store_DeleteHandle(HStoreMgr *sp_handle_pptr);
/**************************************************************************************/
// Description : email sp store, to delete handle.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC int32 MMIEMAIL_SP_Store_CreateAccount(
                                             HStoreMgr      store_handle_ptr,
                                             wchar        *act_name_ptr,
                                             uint32        *act_id_ptr
                                             );
/**************************************************************************************/
// Description : email sp store, to delete account.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC int32 MMIEMAIL_SP_Store_DeleteAccount(
                                             HStoreMgr      store_handle_ptr,
                                             uint32         act_id,
                                             BOOLEAN        is_del_all
                                             );
/**************************************************************************************/
// Description : email sp store, get account info.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC int32 MMIEMAIL_SP_Store_GetActInfo(
                                          HStoreMgr      store_handle_ptr,
                                          uint32         start_pos,
                                          uint32         num,
                                          EMAIL_VECTOR_T *vector_model_ptr
                                          );
/**************************************************************************************/
// Description : email sp store, set account config item date.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC int32 MMIEMAIL_SP_Store_SetActCfgItem(
                                             HStoreMgr      store_handle_ptr,
                                             EMASP_ACCOUNT_ID act_id,
                                             EMASP_PROFILE_ITEM item,
                                             EMASP_PRFITEM_VAL *item_val_ptr
                                             );

/**************************************************************************************/
// Description : email sp store, set account config item date.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC int32 MMIEMAIL_SP_Store_SetActCfgItemEx(
                                               HStoreMgr      store_handle_ptr,
                                               EMASP_ACCOUNT_ID act_id,
                                               EMASP_MSG_OPT   *setting_opt_ptr
                                             );
/**************************************************************************************/
// Description : email sp store, get account config item date.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC int32 MMIEMAIL_SP_Store_GetActCfgItem(
                                             HStoreMgr      store_handle_ptr,
                                             EMASP_ACCOUNT_ID act_id, 
                                             EMASP_PROFILE_ITEM item,
                                             EMASP_PRFITEM_VAL *item_val_ptr
                                             );
/**************************************************************************************/
// Description : email sp store, get account config item date.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC int32 MMIEMAIL_SP_Store_UpdateEmail(
                                         HStoreMgr        store_handle_ptr,
                                         EMAILA_MSGMAK    mask, 
                                         EMAIL_MSGDATACACHE_T  *data_cache_ptr
                                         );
/**************************************************************************************/
// Description : email sp store, save email datecache.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC int32 MMIEMAIL_SP_Store_SaveEmail(
                                       HStoreMgr  store_handle_ptr,
                                       EMAIL_MSGDATACACHE_T *data_cache_ptr
                                       );
/**************************************************************************************/
// Description : email sp store, save email date by msgid.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC int32 MMIEMAIL_SP_Store_GetEmailData(
                                            HStoreMgr        store_handle_ptr,
                                            uint32           email_id, 
                                            uint32           act_id,
                                            EMAIL_MSGDATACACHE_T *data_cache_ptr
                                            );
/**************************************************************************************/
// Description : email sp store, delete email date by msgid.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC int32 MMIEMAIL_SP_Store_DeleteEmail(
                                         HStoreMgr store_handle_ptr,
                                         uint32   act_id,
                                         uint32   email_id
                                         );

/**************************************************************************************/
// Description : email sp store, cancel delete all email
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC int32 MMIEMAIL_SP_Store_CancelDeleteAllEmail(HStoreMgr store_handle_ptr);

/**************************************************************************************/
// Description : email sp store, save email date.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC int32 MMIEMAIL_SP_Store_DeleteAllEmail(
                                              HStoreMgr store_handle_ptr,
                                              uint32 *email_list_ptr,
                                              uint32 email_num
                                              );
/**************************************************************************************/
// Description : email sp store, move email to folder by msgid.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC int32 MMIEMAIL_SP_Store_MoveEmailtoFolder(
                                               HStoreMgr store_handle_ptr,
                                               uint32 src_act_id,
                                               EMASP_BOX_INDEX src_box,
                                               uint32 des_act_id,
                                               EMASP_BOX_INDEX des_box,
                                               uint32 email_id
                                               );
/**************************************************************************************/
// Description : email sp store, get part number from email.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC int32 MMIEMAIL_SP_Store_GetPartCount(
                                            HStoreMgr  store_handle_ptr,
                                            uint32     act_id,
                                            uint32     email_id,
                                            uint32     *part_num_ptr
                                            );
/**************************************************************************************/
// Description : email sp store, get box info.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC int32 MMIEMAIL_SP_Store_GetBoxInfo(
                                          HStoreMgr store_handle_ptr,
                                          uint32 act_id, 
                                          EMASP_BOX_INDEX box_id,
                                          EMAIL_VECTOR_T **pvector_model_pptr
                                          );
/**************************************************************************************/
// Description : email sp store, get number of email from box.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC int32 MMIEMAIL_SP_Store_GetBoxEmailNum(
                                              HStoreMgr store_handle_ptr,
                                              uint32 act_id,
                                              uint32 box_index,
                                              uint32* num_ptr
                                              );
/**************************************************************************************/
// Description : email sp store, create a email box.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC int32 MMIEMAIL_SP_Store_CreateBox(
                                         HStoreMgr      store_handle_ptr,
                                         wchar         *box_name_ptr,
                                         uint32         *box_id_ptr,
                                         uint32         act_id,
                                         uint32         box_fa_id,
                                         uint8          state
                                         );
/**************************************************************************************/
// Description : email sp store, delete a box's email.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC int MMIEMAIL_SP_Store_DeleteBox(
                                       HStoreMgr     store_handle_ptr,
                                       uint32        box_id,
                                       uint32        act_id,
                                       uint32        box_fa_id
                                       );
/**************************************************************************************/
// Description : email sp store, get space info of box.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC int32 MMIEMAIL_SP_Store_GetSpecBoxInfo(
                                              HStoreMgr store_handle_ptr,
                                              uint32 act_id,
                                              uint8 state,
                                              EMAIL_VECTOR_T *vector_model_ptr
                                              ); 
/**************************************************************************************/
// Description : email sp store, set box's status.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC int MMIEMAIL_SP_Store_SetBoxStatus(
                                          HStoreMgr  store_handle_ptr,
                                          uint32     act_id,
                                          wchar*    box_name_ptr,
                                          uint8      box_status
                                          );
/**************************************************************************************/
// Description : email sp store, get box's status.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC int MMIEMAIL_SP_Store_GetBoxStatus(
                                          HStoreMgr  store_handle_ptr,
                                          uint32     act_id,
                                          wchar     *box_name_ptr,
                                          uint8      *box_status_ptr
                                          );   //value should be free in char type
/**************************************************************************************/
// Description : email sp store, get box's status.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC int MMIEMAIL_SP_Store_IsBoxExit(
                                       HStoreMgr  store_handle_ptr,
                                       uint32     act_id,
                                       wchar     *box_name_ptr,
                                       BOOLEAN    *is_ret_ptr
                                       );
/**************************************************************************************/
// Description : email sp store, email be down.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC int MMIEMAIL_SP_Store_Bedown(
                                    HStoreMgr  store_handle_ptr,
                                    uint32     act_id,
                                    uint32     box_id,
                                    uint32     email_id,
                                    BOOLEAN    *is_down_ptr    // true:there is somthing to be down
                                    );                 // false:there is nothing to be down
/**************************************************************************************/
// Description : email sp store, get account id by email.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC int MMIEMAIL_SP_Store_GetActIdByEmail(
                                             HStoreMgr     store_handle_ptr,
                                             const wchar  *email_addr_ptr,
                                             uint32        *act_id_ptr
                                             );
/**************************************************************************************/
// Description : email sp store, remove email part by Dir.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC int MMIEMAIL_SP_Store_RemovePartByDir(
                                             HStoreMgr  store_handle_ptr,
                                             uint32     act_id,
                                             uint32     email_id,
                                             BOOLEAN    is_attach_or_text,     //true is attach, false is text
                                             wchar     *part_dir_ptr     // only for attach
                                             );
/**************************************************************************************/
// Description : email sp store, remove all email part.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC int MMIEMAIL_SP_Store_RemoveParts(
                                         HStoreMgr     store_handle_ptr,
                                         uint32        email_id,
                                         uint32        act_id,
                                         BOOLEAN       is_only_attach
                                         );
/**************************************************************************************/
// Description : email sp store, copy email date to folder.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC int32 MMIEMAIL_SP_Store_CopyEmailtoFolder(
                                                 HStoreMgr store_handle_ptr,
                                                 EMASP_BOX_INDEX des_box_id,
                                                 uint32 email_id,
                                                 uint32 act_id
                                                 );
/**************************************************************************************/
// Description : email sp store, get email view type.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC int32 MMIEMAIL_SP_Store_GetViewEmlType(
                                              HStoreMgr   store_handle_ptr,
                                              uint32      email_id,
                                              uint32      act_id,
                                              uint32      *view_type_ptr,
                                              char        **source_buff_pptr
                                              );
/**************************************************************************************/
// Description : email sp store, get box id by msgid.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC int32 MMIEMAIL_SP_Store_GetBoxIdByEmailId(
                                                 HStoreMgr        store_handle_ptr,
                                                 uint32           email_id, 
                                                 uint32           act_id,
                                                 uint32*          box_id_ptr
                                                 );
/**************************************************************************************/
// Description : email sp store, get default account.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC int32 MMIEMAIL_SP_Store_GetDefaultAccount(
                                                 HStoreMgr      store_handle_ptr,
                                                 uint32         *num_ptr,
                                                 EMS_ServerConf **serv_con_info_pptr
                                                 );
/**************************************************************************************/
// Description : email sp store, factory reset.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC void MMIEMAIL_SP_Store_FactoryReset(
                                           HStoreMgr      store_handle_ptr
                                           );

/**************************************************************************************/
// Description : email sp store, to delete multi account.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC int32 MMIEMAIL_SP_Store_DeleteMultiAccount(
                                                  HStoreMgr      store_handle_ptr,
                                                  uint32 *act_id_ptr,
                                                  uint32 num
                                                  );

/**************************************************************************************/
// Description : email sp store, update email's header by msg id.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC int32 MMIEMAIL_SP_Store_UpdateEmailHeader(
                                                 HStoreMgr store_handle_ptr,
                                                 uint32    email_id
                                                 );

/**************************************************************************************/
// Description : email sp store, update email's state.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC int32 MMIEMAIL_SP_Store_UpdateEmailState(
                                              HStoreMgr store_handle_ptr,
                                              EMAIL_UPDATE_STATE_T *email_state_ptr,
                                              unsigned int email_num
                                              );

/**************************************************************************************/
// Description : email sp store, convent to EME status.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC uint32 MMIEMAIL_SP_Store_ConventToEmeStatus(uint32 eme_status);

/**************************************************************************************/
// Description : email sp store, update email's msg index.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC void MMIEMAIL_SP_Store_SyncMsgIndexInfo(HStoreMgr store_handle_ptr);

#endif /* _MMIEMAIL_SP_STORE_API_H_ */
