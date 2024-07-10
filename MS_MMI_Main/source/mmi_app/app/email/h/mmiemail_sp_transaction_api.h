/***************************************************************************************
** File Name:      mmiemail_sp_transaction_api.h                                       *
** Author:                                                                             *
** Date:           05/13/2011                                                          *
** Copyright:      2011 Spreadtrum, Incorporated. All Rights Reserved.                 *
** Description:    Head file of E-mail transaction.This File will gather functions that*
**         special handle email transaction. It contains send a email, receive emails, *
**         view a email and so on. These functions don't be changed by project changed.*
****************************************************************************************
**                         Important Edit History                                      *
** ------------------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                                         *
** 05/2011                         create                                              *
**                                                                                     *
***************************************************************************************/
#ifndef _MMIEMAIL_SP_TRANSACTION_API_H_
#define _MMIEMAIL_SP_TRANSACTION_API_H_

/**------------------------------------------------------------------------------------*
**                         Include Files                                               *
**------------------------------------------------------------------------------------*/
#include "mmiemail_def.h"

/**------------------------------------------------------------------------------------*
**                         Macro Declaration                                           *
**------------------------------------------------------------------------------------*/
#define EMA_IMAP4_MARK_DEL_ON_SERVER               0x01
#define EMA_IMAP4_UNMARK_DEL_ON_SERVER             0x02
#define EMA_IMAP4_MARK_SEEN_ON_SERVER              0x03
#define EMA_IMAP4_UNMARK_SEEN_ON_SERVER            0x04

//#define EME_TRANS_IMAP4_EXPUNGE_DEL_MAIL_IN_MAILBOX      0x2B
//#define EME_TRANS_IMAP4_CREAT_MAILBOX                    0x2C
//#define EME_TRANS_IMAP4_DELETE_MAILBOX                   0x2E
//#define EME_TRANS_IMAP4_SUBSCRIBE_MAILBOX                0x2F
//#define EME_TRANS_IMAP4_UNSUBSCRIBE_MAILBOX              0x30

/**------------------------------------------------------------------------------------*
**                         TYPE AND CONSTANT                                           *
**------------------------------------------------------------------------------------*/
typedef uint32 IMAP4_MARK_TYPE;
typedef void* HTransMgr;

/**------------------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                                         *
**------------------------------------------------------------------------------------*/
/**************************************************************************************/
// Description : email sp transaction, to init handle.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC int32 MMIEMAIL_SP_TRS_InitHandle(
                                        HTransMgr *trans_mgr_ptr,
                                        void  *parent_handle_ptr
                                        );
/**************************************************************************************/
// Description : email sp transaction, to delete handle.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC void MMIEMAIL_SP_TRS_DeleteHandle(HTransMgr *trans_mgr_ptr);
/**************************************************************************************/
// Description : email sp transaction, send email.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC int32 MMIEMAIL_SP_TRS_SendEmail(
                                     HTransMgr trans_mgr, 
                                     uint32 email_id,
                                     uint32 act_id,
                                     uint32 net_id
                                     );
/**************************************************************************************/
// Description : email sp transaction, send RR.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC int32 MMIEMAIL_SP_TRS_SendRR(
                                    HTransMgr trans_mgr, 
                                    uint32 email_id,
                                    uint32 act_id,
                                    uint32 net_id
                                    );
/**************************************************************************************/
// Description : email sp transaction, cancel current transaction.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC int32 MMIEMAIL_SP_TRS_CancelCurrentTrans(
                                                HTransMgr trans_mgr
                                                );
/**************************************************************************************/
// Description : email sp transaction, retrieve account email head.
// Global resource dependence : 
// Author: 
// Note: Parameter is TBD. Need to differentiate POP3 and IMAP4?
/**************************************************************************************/
PUBLIC int32 MMIEMAIL_SP_TRS_RetrieveActEmailHead(
                                                HTransMgr trans_mgr, 
                                                uint32  act_id,
                                                char    *box_name_ptr,
                                                uint32  net_id
                                                );
/**************************************************************************************/
// Description : email sp transaction, retrieve account email.
// Global resource dependence : 
// Author: 
// Note: Parameter is TBD. Need to differentiate POP3 and IMAP4?
/**************************************************************************************/
PUBLIC int32 MMIEMAIL_SP_TRS_RetrieveActEmail(
                                              HTransMgr trans_mgr, 
                                              uint32 act_id,
                                              char   *box_name_ptr,
                                              uint32 net_id
                                              );
/**************************************************************************************/
// Description : email sp transaction, retrieve attachment.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC int32 MMIEMAIL_SP_TRS_RetrieveAttachment(
                                                HTransMgr trans_mgr, 
                                                uint32    act_id,
                                                uint32    email_id,
                                                wchar     *section_id_ptr
                                                );
/**************************************************************************************/
// Description : email sp transaction, retrieve account email body.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC int32 MMIEMAIL_SP_TRS_RetrieveEmailBody(
                                             HTransMgr trans_mgr, 
                                             uint32  act_id,
                                             uint32 email_id,
                                             uint32 net_id
                                             );
/**************************************************************************************/
// Description : email sp transaction, cancel retrieve email.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC int32 MMIEMAIL_SP_TRS_CancelRetrieve(
                                            HTransMgr trans_mgr,
                                            uint32    trans_id
                                            );
/**************************************************************************************/
// Description : email sp transaction, view email.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC int32 MMIEMAIL_SP_TRS_ViewEml(
                                     HTransMgr        trans_mgr,
                                     uint32           act_id,
                                     uint32           email_id,
                                     EMAIL_MSGDATACACHE_T *email_datacache_ptr,
                                     BOOLEAN         *is_has_download_ptr
                                     );
/**************************************************************************************/
// Description : email sp transaction, to handle email transaction.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC BOOLEAN MMIEMAIL_SP_TRS_HandleEvent(
                                           HTransMgr trans_mgr,
                                           uint32 eCode,
                                           uint16 wparam,
                                           uint32 dwparam
                                           );
/**************************************************************************************/
// Description : email sp transaction, retrieve email folder.
// Global resource dependence : 
// Author: 
// Note:  True is All ,False is sub 
/**************************************************************************************/
PUBLIC int32 MMIEMAIL_SP_TRS_RetrieveFolder(
                                            HTransMgr trans_mgr, 
                                            uint32    act_id,
                                            BOOLEAN   is_all_or_sub
                                            );
/**************************************************************************************/
// Description : email sp transaction, sub or unsub folder.
// Global resource dependence : 
// Author: 
// Note:  True is Sub, Flase is Unsub
/**************************************************************************************/
PUBLIC int32 MMIEMAIL_SP_TRS_SubFolder(
                                       HTransMgr trans_mgr, 
                                       uint32    act_id,
                                       const char *boxname_type_ptr,
                                       BOOLEAN is_sub_folder
                                       );
/**************************************************************************************/
// Description : email sp transaction, Imap4 mark.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC int32 MMIEMAIL_SP_TRS_Imap4Mark(
                                       HTransMgr trans_mgr, 
                                       IMAP4_MARK_TYPE make_type,
                                       uint32  act_id,
                                       uint32  email_id
                                       );
/**************************************************************************************/
// Description : email sp transaction, uploda or copy mail.
// Global resource dependence : 
// Author: 
// Note: True is upload,false is copy
/**************************************************************************************/
PUBLIC int32 MMIEMAIL_SP_TRS_UploadOrCopyMail(
                                              HTransMgr trans_mgr, 
                                              uint32  act_id,
                                              uint32  email_id,
                                              const char *box_name_ptr,
                                              BOOLEAN is_upload
                                              );
/**************************************************************************************/
// Description : email sp transaction, check is busy or not.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC int MMIEMAIL_SP_TRS_IsBusy(HTransMgr trans_mgr,
                                  BOOLEAN   *is_busy_ptr, 
                                  uint32    *trans_state_ptr);
/**************************************************************************************/
// Description : email sp transaction, get email's msg id by transaction id.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC int MMIEMAIL_SP_TRS_GetEmailidByTransId(
                                               HTransMgr trans_mgr,
                                               uint32 trans_id,
                                               uint32 *email_id_ptr
                                               );
/**************************************************************************************/
// Description : email sp transaction, free transaction.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC int MMIEMAIL_SP_TRS_DeleteTrans(
                                       HTransMgr trans_mgr,
                                       uint32 trans_id,
                                       uint32 email_id
                                       );

#endif /* _MMIEMAIL_SP_TRANSACTION_API_H_*/


