/***************************************************************************************
** File Name:      mmiemail_sp_store.c                                                 *
** Author:                                                                             *
** Date:           05/13/2011                                                          *
** Copyright:      2011 Spreadtrum, Incorporated. All Rights Reserved.                 *
** Description:    Head file of E-mail store.This File will gather functions that      *
**    special handle email store. It will include save a email, update a email, get    *
**    specify box information and so on. These functions don't be changed by project   *
**    changed.                                                                         *
****************************************************************************************
**                         Important Edit History                                      *
** ------------------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                                         *
** 05/2011                         create                                              *
**                                                                                     *
***************************************************************************************/
/**------------------------------------------------------------------------------------*
**                         Include Files                                               *
**------------------------------------------------------------------------------------*/
/*#include "mmifmm_export.h"*/
#include "os_api.h"
#include "sci_types.h"
#include "mmk_type.h"
#include "mmi_modu_main.h"
#include "email_text.h"
#include "mmiemail_sp_api.h"
#include "mmiemail_sp_store_api.h"
#include "mmiemail_utils.h"
#include "mmifmm_export.h"
#include "EMS_Store_API.h"

//#include "string.brh"
#include "we_def.h"

/**------------------------------------------------------------------------------------*
**                         Macro Declaration                                           *
**------------------------------------------------------------------------------------*/


/**------------------------------------------------------------------------------------*
**                             TYPE AND CONSTANT                                       *
**------------------------------------------------------------------------------------*/
typedef struct email_storemgr_tag
{
   HEMSStore  qee_store_ptr; 
   HEmlSPMgr   parent_handle_ptr;
   EMAIL_MEM_HANDLE_T *mem_mgr_ptr;
}EMAIL_STOREMGR_T;

typedef struct email_decodeadr_tag
{
   wchar* str_to_pos_ptr; /* Record the last address of string which will will be decoded*/   
}EMAIL_DECODEADR_T;

extern BOOLEAN  g_email_is_engine_task_busy;
/*-------------------------------------------------------------------------------------*
**                             LOCAL FUNCTION DECLARE                                  *
**------------------------------------------------------------------------------------*/
/**************************************************************************************/
// Description : email sp store, free buffer.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL void SP_Store_FreeBuf(void* data_ptr);
/**************************************************************************************/
// Description : email sp store, clear cache.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL int32 SP_Store_ClearCache(
                                HStoreMgr store_handle_ptr,
                                EMAIL_MSGDATACACHE_T  *data_cache_ptr
                                );
/**************************************************************************************/
// Description : email sp store, get to string.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL BOOLEAN SP_Store_GetToStr(
                                EMAIL_DECODEADR_T  *cx_ptr, 
                                wchar     *ret_str_ptr, 
                                wchar     *list_str_ptr
                                );
/**************************************************************************************/
// Description : email sp store, save or update email.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL int32 SP_Store_SaveOrUpdateEmail(
                                       HStoreMgr store_handle_ptr,
                                       EMAIL_MSGDATACACHE_T *data_cache_ptr,
                                       BOOLEAN is_save
                                       );
/**************************************************************************************/
// Description : email sp store, update to.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL int32 SP_Store_UpdateTo(
                              HStoreMgr        store_handle_ptr,
                              HEMSMsg          qee_msg_ptr,
                              HEMSStore        qee_store_ptr,
                              EMAIL_MSGDATACACHE_T  *data_cache_ptr
                              );
/**************************************************************************************/
// Description : email sp store, update CC.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL int32 SP_Store_UpdateCc(
                              HStoreMgr         store_handle_ptr,
                              HEMSMsg           qee_msg_ptr,
                              HEMSStore         qee_store_ptr,
                              EMAIL_MSGDATACACHE_T   *data_cache_ptr
                              );
/**************************************************************************************/
// Description : email sp store, update BCC.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL int32 SP_Store_UpdateBcc(
                               HStoreMgr        store_handle_ptr,
                               HEMSMsg          qee_msg_ptr,
                               HEMSStore        qee_store_ptr,
                               EMAIL_MSGDATACACHE_T  *data_cache_ptr
                               );
/**************************************************************************************/
// Description : email sp store, update subject.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL int32 SP_Store_UpdateSubject(
                                   HEMSMsg           qee_msg_ptr,
                                   HEMSStore         qee_store_ptr,
                                   EMAIL_MSGDATACACHE_T   *data_cache_ptr
                                   );
/**************************************************************************************/
// Description : email sp store, update attachment.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL int32 SP_Store_UpdateAttachment(
                                      HEMSMsg           qee_msg_ptr,
                                      HEMSStore         qee_store_ptr,
                                      EMAIL_MSGDATACACHE_T   *data_cache_ptr
                                      );
/**************************************************************************************/
// Description : email sp store, clear EMN cache.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL int32 SP_Store_ClearEMNCache(
                                   HStoreMgr store_handle_ptr,
                                   EMAIL_EMNCACHE_T *emn_cache_ptr
                                   );
/**************************************************************************************/
// Description : email sp store, convent to EMA status.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL uint32 SP_Store_ConventToEmaStatus(uint32 eml_status);
/**************************************************************************************/
// Description : email sp store, update From addr.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL int32 SP_Store_UpdateFrom(
                                HStoreMgr         store_handle_ptr,
                                HEMSMsg           qee_msg_ptr,
                                HEMSStore         qee_store_ptr,
                                EMAIL_MSGDATACACHE_T   *data_cache_ptr
                                );
/**************************************************************************************/
// Description : email sp store, convent to EME status.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
//PUBLIC uint32 SP_Store_ConventToEmeStatus(uint32 eme_status);
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
                                          void             *parent_handle_ptr /*,
                                          St_EMA_MemHandle *mem_mgr_ptr*/
                                          )
{
    int   ret = EMAIL_SUCCESS;
    EMAIL_STOREMGR_T  *store_mgr_ptr = PNULL;
    
    if (PNULL == sp_handle_pptr || PNULL == parent_handle_ptr)
    {
        return EMAIL_ERROR;
    }
    do 
    {
        store_mgr_ptr = (EMAIL_STOREMGR_T*)EMA_UTILS_MALLOC(mem_mgr_ptr,sizeof(EMAIL_STOREMGR_T));
        EMA_UTILS_BREAKNULL(store_mgr_ptr,ret,"~~~~~~~~MMIEMAIL_SP_Store_InitHandle: Malloc store_mgr_ptr failed!~~~~~~~~~~~");
        
        store_mgr_ptr->parent_handle_ptr = (HEmlSPMgr)parent_handle_ptr;
        
        store_mgr_ptr->qee_store_ptr = HEMSSTORE_New(PNULL, PNULL);
        EMA_UTILS_BREAKNULL(store_mgr_ptr->qee_store_ptr, ret, "create store handle failed");
        
        *sp_handle_pptr = (HStoreMgr)store_mgr_ptr;
    } while(0);
    
    if (EMAIL_SUCCESS != ret)
    {
        MMIEMAIL_SP_Store_DeleteHandle((HStoreMgr)&store_mgr_ptr);
    }
    return ret;  
}

/**************************************************************************************/
// Description : email sp store, to delete handle.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC void MMIEMAIL_SP_Store_DeleteHandle(HStoreMgr *sp_handle_pptr)
{
    EMAIL_STOREMGR_T *store_mgr_ptr = (EMAIL_STOREMGR_T*)(*sp_handle_pptr);
    
    if (PNULL == store_mgr_ptr)
    {
        return ;
    }
    
    HEMSSTORE_Delete(store_mgr_ptr->qee_store_ptr);
    EMA_UTILS_FREE(store_mgr_ptr->mem_mgr_ptr, store_mgr_ptr);
    
    *sp_handle_pptr = PNULL;
}

/**************************************************************************************/
// Description : email sp store, EMAOpt Convent EMEOp.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL uint32 SP_Store_EMAOpt_Convent_EMEOpt(uint32 eml_opt)
{
    switch(eml_opt)
    {
    case EMASP_PRFITEM_ACCOUNT_NAME:
        return EMS_PRFITEM_ACCOUNT_NAME;
        
    case EMASP_PRFITEM_ACCOUNT_EMAIL:
        return EMS_PRFITEM_ACCOUNT_EMAIL;
        
    case EMASP_PRFITEM_REPLY_TO_EMAIL:
        return EMS_PRFITEM_REPLY_TO_EMAIL;
        
    case EMASP_PRFITEM_EMAIL_USER_ID:
        return EMS_PRFITEM_EMAIL_USER_ID;
        
    case EMASP_PRFITEM_EMAIL_PASSWORD:
        return EMS_PRFITEM_EMAIL_PASSWORD;
        
    case EMASP_PRFITEM_SMTP_USER_ID:
        return EMS_PRFITEM_SMTP_USER_ID;
        
    case EMASP_PRFITEM_SMTP_PASSWORD:
        return EMS_PRFITEM_SMTP_PASSWORD;
        
    case EMASP_PRFITEM_DATA_SERVICE_PROVIDER:
        return EMS_PRFITEM_DATA_SERVICE_PROVIDER;
        
    case EMASP_PRFITEM_RECEIVE_SERVER_TYPE:
        return EMS_PRFITEM_RECEIVE_SERVER_TYPE;
        
    case EMASP_PRFITEM_POP3_SERVER:
        return EMS_PRFITEM_POP3_SERVER;
        
    case EMASP_PRFITEM_POP3_PORT:
        return EMS_PRFITEM_POP3_PORT;
        
    case EMASP_PRFITEM_POP3_SSL_OR_TLS:
        return EMS_PRFITEM_POP3_SSL_OR_TLS;
        
    case EMASP_PRFITEM_IMAP4_SERVER:
        return EMS_PRFITEM_IMAP4_SERVER;
        
    case EMASP_PRFITEM_IMAP4_SERVER_PORT:
        return EMS_PRFITEM_IMAP4_SERVER_PORT;
        
    case EMASP_PRFITEM_IMAP4_SSL_OR_TLS:
        return EMS_PRFITEM_IMAP4_SSL_OR_TLS;
        
    case EMASP_PRFITEM_SMTP_SERVER:
        return EMS_PRFITEM_SMTP_SERVER;
        
    case EMASP_PRFITEM_SMTP_SERVER_PORT:
        return EMS_PRFITEM_SMTP_SERVER_PORT;
        
    case EMASP_PRFITEM_SMTP_SSL_OR_TLS:
        return EMS_PRFITEM_SMTP_SSL_OR_TLS;
        
    case EMASP_PRFITEM_SMTP_AUTHENTICATE:
        return EMS_PRFITEM_SMTP_AUTHENTICATE;
        
    case EMASP_PRFITEM_EMAIL_DOWNLOAD_MODE:
        return EMS_PRFITEM_EMAIL_DOWNLOAD_MODE;
        
    case EMASP_PRFITEM_NEW_MSG_NOTIFICATION:
        return EMS_PRFITEM_NEW_MSG_NOTIFICATION;
        
    case EMASP_PRFITEM_MAX_MSG_SIZE:
        return EMS_PRFITEM_MAX_MSG_SIZE;
        
    case EMASP_PRFITEM_RECEIVER_LEAVE_COPY:
        return EMS_PRFITEM_RECEIVER_LEAVE_COPY;
        
    case EMASP_PRFITEM_DELETE_LEAVE_COPY:
        return EMS_PRFITEM_DELETE_LEAVE_COPY;
        
    case EMASP_PRFITEM_SAVE_SENT:
        return EMS_PRFITEM_SAVE_SENT;
        
    case EMASP_PRFITEM_INCLUDE_ORIGINAL:
        return EMS_PRFITEM_INCLUDE_ORIGINAL;
        
    case EMASP_PRFITEM_NUMBER_LIMIT:
        return EMS_PRFITEM_NUMBER_LIMIT;
        
    case EMASP_PRFITEM_READ_RECEIPT:
        return EMS_PRFITEM_READ_RECEIPT;
        
    case EMASP_PRFITEM_EMAIL_ADD_SIGNATURE_MODE:
        return EMS_PRFITEM_EMAIL_ADD_SIGNATURE_MODE;
        
    case EMASP_PRFITEM_EMAIL_CREATE_SIGNATURE:
        return EMS_PRFITEM_EMAIL_CREATE_SIGNATURE;
        
    case EMASP_PRFITEM_EMAIL_DEFAULTINBOXID:
        return EMS_PRFITEM_EMAIL_DEFAULTINBOXID;
        
    case EMASP_PRFILEM_EMAIL_UIDLFILEDIR:
        return EMS_PRFITEM_EMAIL_UIDLFILEDIR;
        
    case EMASP_PRFILEM_EMAIL_UIDFILEDIR:
        return EMS_PRFITEM_EMAIL_UIDFILEDIR;
        
    case EMASP_PRFITEM_RECEIVE_UNSEEN:
        return EMS_PRFITEM_RECEIVE_UNSEEN;
        
    case EMASP_PRFITEM_DOWNLOAD_ATTACH:
        return EMS_PRFITEM_DOWNLOAD_ATTACH;
        
    case EMASP_PRFILEM_USER_NAME:
        return EMS_PRFITEM_USER_NAME;
        
    case EMASP_PRFITEM_USER1_PROFILE:
        return EMS_PRFITEM_USER1_PROFILE;
        
    case EMASP_PRFITEM_USER2_PROFILE:
        return EMS_PRFITEM_USER2_PROFILE;
        
    case EMASP_PRFITEM_USER3_PROFILE:
        return EMS_PRFITEM_USER3_PROFILE;
        
    case EMASP_PRFITEM_NET_PATH:
        return EMS_PRFITEM_NET_PATH;
        
    case EMASP_PRFITEM_POP3_PORT_DEFAULT:
        return EMS_PRFITEM_POP3_PORT_DEFAULT;
        
    case EMASP_PRFITEM_IMAP4_PORT_DEFAULT:
        return EMS_PRFITEM_IMAP4_PORT_DEFAULT;
        
    case EMASP_PRFITEM_SMTP_PORT_DEFAULT:
        return EMS_PRFITEM_SMTP_PORT_DEFAULT;
        
    case EMASP_PRFITEM_SEND_MAIL:
        return EMS_PRFITEM_SEND_MAIL;
        
    case EMASP_PRFITEM_SENDER_NAME:
        return EMS_PRFITEM_SENDER_NAME;
        
    case EMASP_PRFITEM_RECEIVE_FREQUENCY:
        return EMS_PRFITEM_RECEIVE_FREQUENCY;
        
    case EMASP_PRFITEM_RECEIVE_ONLY_NEW_MAIL:
        return EMS_PRFITEM_RECEIVE_ONLY_NEW_MAIL;
        
    case EMASP_PRFITEM_MAIL_AUTO_DELETE:
        return EMS_PRFITEM_MAIL_AUTO_DELETE;
        
    case EMASP_PRFITEM_MAIL_DELETE:
        return EMS_PRFITEM_MAIL_DELETE;
        
    case EMASP_PRFITEM_NEW_MAIL_TIP:
        return EMS_PRFITEM_NEW_MAIL_TIP;
        
    case EMASP_PRFITEM_STARTUP_SCREEN:
        return EMS_PRFITEM_STARTUP_SCREEN;
        
    case EMASP_PRFITEM_MAIL_STORAGE:
        return EMS_PRFITEM_SAVE_PLACE;
        
    case EMASP_PRFITEM_TONE:
        return EMS_PRFITEM_TONE;
        
    case EMASP_PRFITEM_TONE_VOLUME:
        return EMS_PRFITEM_TONE_VOLUME;
        
    case EMASP_PRFITEM_SLIENT_TIME:
        return EMS_PRFITEM_SLIENT_TIME;
        
    case EMASP_PRFITEM_ACCOUNT_PASSWORD_PROTECTION:
        return EMS_PRFITEM_ACCOUNT_PASSWORD_PROTECTION;
        
    case EMASP_PRFITEM_SET_PASSWORD_PROTECTION:
        return EMS_PRFITEM_SET_PASSWORD_PROTECTION;
        
    case EMASP_PRFITEM_STORAGE_STATISTICS:
        return EMS_PRFITEM_STORAGE_STATISTICS;
        
    case EMASP_PRFITEM_FLOW_STATISTICS:
        return EMS_PRFITEM_FLOW_STATISTICS;
        
    default:
        return 0;
   }
}

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
                                             )
{
    EMAIL_STOREMGR_T *store_mgr_ptr = (EMAIL_STOREMGR_T*)store_handle_ptr;
    int32     ret = EMAIL_ERROR;
    uint32    act_id = 0;
    uint32    box_id = 0;
    
    if (PNULL == store_mgr_ptr || PNULL == act_name_ptr || PNULL == act_id_ptr || PNULL == store_mgr_ptr->qee_store_ptr)
    {
        EMA_UTILS_LOG_ERROR(("~~~~~~~~~MMIEMAIL_SP_Store_CreateAccount: Bad paramter!~~~~~~~~"));
        return EMAIL_BADPARAM;
    }
    
    do 
    {
        ret = HEMSSTORE_CreateAccount(store_mgr_ptr->qee_store_ptr, act_name_ptr, (unsigned int*)&act_id);
        EMA_UTILS_BREAKIF(ret, "MMIEMAIL_SP_Store_CreateAccount: IQEESTORE_CreateAccount failed!");
        *act_id_ptr = act_id;
        
        EMA_UTILS_LOG_INFO(("MMIEMAIL_SP_Store_CreateAccount act_id = 0x%x", act_id));
        
        ret = HEMSSTORE_CreateBox(store_mgr_ptr->qee_store_ptr, (unsigned short *)L"Inbox", (unsigned int*)&box_id, act_id, EMASP_BOX_STATE_LOCAL);
        EMA_UTILS_BREAKIF(ret, "create box failed");
        
        ret = HEMSSTORE_CreateBox(store_mgr_ptr->qee_store_ptr, (unsigned short *)L"Outbox", (unsigned int*)&box_id, act_id, EMASP_BOX_STATE_LOCAL);
        EMA_UTILS_BREAKIF(ret, "create box failed");
        
        ret = HEMSSTORE_CreateBox(store_mgr_ptr->qee_store_ptr, (unsigned short *)L"Sentbox", (unsigned int*)&box_id, act_id, EMASP_BOX_STATE_LOCAL);
        EMA_UTILS_BREAKIF(ret, "create box failed");
        
        ret = HEMSSTORE_CreateBox(store_mgr_ptr->qee_store_ptr, (unsigned short *)L"Draft", (unsigned int*)&box_id, act_id, EMASP_BOX_STATE_LOCAL);
        EMA_UTILS_BREAKIF(ret, "create box failed");
        
    } while(0);
    
    if(EMAIL_SUCCESS != ret)
    {
        HEMSSTORE_DeleteAccount(store_mgr_ptr->qee_store_ptr, act_id);
    }
    
    return ret;
}

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
                                             )
{
    EMAIL_STOREMGR_T *store_mgr_ptr = (EMAIL_STOREMGR_T*)store_handle_ptr;
    
    if (PNULL == store_mgr_ptr || PNULL == store_mgr_ptr->qee_store_ptr)
    {
        EMA_UTILS_LOG_ERROR(("~~~~~~~~~MMIEMAIL_SP_Store_DeleteAccount: Bad paramter!~~~~~~~~~~~"));
        return EMAIL_BADPARAM;
    }
    
    return HEMSSTORE_DeleteAccount(store_mgr_ptr->qee_store_ptr, act_id);
}


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
                                                  )
{
    EMAIL_STOREMGR_T *store_mgr_ptr = (EMAIL_STOREMGR_T*)store_handle_ptr;
    uint32  ret = EMAIL_SUCCESS;

    if (PNULL == store_mgr_ptr || PNULL == store_mgr_ptr->qee_store_ptr)
    {
        EMA_UTILS_LOG_ERROR(("~~~~~~~~~MMIEMAIL_SP_Store_DeleteAccount: Bad paramter!~~~~~~~~~~~"));
        return EMAIL_BADPARAM;
    }
    
    
    ret = HEMSSTORE_DeleteMultiAccount(store_mgr_ptr->qee_store_ptr, (EMS_ACTID*)act_id_ptr, num);
    if (EMAIL_SUCCESS == ret)
    {
        g_email_is_engine_task_busy = TRUE;
        SCI_TRACE_LOW("mmiemail_sp_stroe.c set g_email_is_engine_task_busy TRUE, MMIEMAIL_SP_Store_DeleteMultiAccount");
    }
    return ret;
}

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
                                              )
{
    EMAIL_STOREMGR_T  *store_mgr_ptr = (EMAIL_STOREMGR_T*)store_handle_ptr;
    
    
    if (PNULL == store_mgr_ptr || PNULL == store_mgr_ptr->qee_store_ptr ||0 == act_id || PNULL == num_ptr)
    {
        EMA_UTILS_LOG_ERROR(("~~~~~~~~~MMIEMAIL_SP_Store_GetBoxEmailNum: Bad paramter!~~~~~~~~~~~~~"));
        return EMAIL_BADPARAM;
    }
    
    return HEMSSTORE_GetMsgInfo(store_mgr_ptr->qee_store_ptr, act_id, box_index, 0, (unsigned int*)num_ptr, PNULL);
}

/**************************************************************************************/
// Description : email sp store, get number of unread email.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL int32 SP_Store_GetActUnreadNum(
                                     HStoreMgr store_handle_ptr,
                                     uint32 act_id,
                                     uint32* num_ptr
                                     )
{
    int ret = EMAIL_SUCCESS;
    EMAIL_STOREMGR_T  *store_mgr_ptr = (EMAIL_STOREMGR_T*)store_handle_ptr;
    EMS_MsgInfo  *email_info_ptr = PNULL;
    uint32       num = 0;
    uint32       i = 0;
    
    if (PNULL == store_mgr_ptr || PNULL == store_mgr_ptr->qee_store_ptr ||0 == act_id || PNULL == num_ptr)
    {
        EMA_UTILS_LOG_ERROR(("~~~~~~~~~SP_Store_GetActUnreadNum: Bad paramter!~~~~~~~~~~~~~"));
        return EMAIL_BADPARAM;
    }
    
    do
    {
        *num_ptr = 0;
        
        ret = HEMSSTORE_GetMsgInfo(store_mgr_ptr->qee_store_ptr, act_id, E_INBOX_INDEX, 0, (unsigned int*)&num, PNULL);
        EMA_UTILS_BREAKIF(ret, "get msg info failed");
        
        if(0 == num)
        {
            break;
        }
        
        email_info_ptr = EMA_UTILS_MALLOC(PNULL, num * sizeof(EMS_MsgInfo));
        EMA_UTILS_BREAKNULL(email_info_ptr, ret, "no memory");
        
        EMA_UTILS_MEMSET(email_info_ptr, 0x00, num * sizeof(EMS_MsgInfo));
        
        ret = HEMSSTORE_GetMsgInfo(store_mgr_ptr->qee_store_ptr, act_id, E_INBOX_INDEX, 0, (unsigned int*)&num, email_info_ptr);
        EMA_UTILS_BREAKIF(ret, "get msg info failed");
        
        *num_ptr = num;
        
        for(i = 0; i < num; i++)
        {
            if(email_info_ptr[i].dwEmlStatus & EMSMSG_STATUS_READ)
            {
                *num_ptr = *num_ptr - 1;
            }
        }
        
    } while(0);
    
    EMA_UTILS_FREEIF(PNULL, email_info_ptr);
    
    return ret;
}

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
                                          )
{
    EMAIL_STOREMGR_T  *store_mgr_ptr = (EMAIL_STOREMGR_T*)store_handle_ptr;
    EMASP_ACTINFO_T   *act_info_ptr = PNULL;
    EMS_ActInfo       *ems_act_info_ptr = PNULL;
    uint32            i = 0;
    uint32            act_num = 0;
    int32             ret = EMAIL_ERROR;
    
    if(PNULL == store_mgr_ptr || PNULL == store_mgr_ptr->qee_store_ptr || PNULL == vector_model_ptr)
    {
        EMA_UTILS_LOG_ERROR(("~~~~~~~~~MMIEMAIL_SP_Store_GetActInfo: Bad paramter!~~~~~~~~~~~~~~`"));
        return EMAIL_BADPARAM;
    }
    
    do 
    {
        MMIEMAIL_VectorSetPfnFree(vector_model_ptr, SP_Store_FreeBuf);
        
        ret = HEMSSTORE_GetActInfo(store_mgr_ptr->qee_store_ptr, 0, (unsigned int*)&act_num, PNULL);
        EMA_UTILS_BREAKIF(ret, "get act info failed");
        
        if(0 == act_num)
        {
            EMA_UTILS_LOG_ERROR(("MMIEMAIL_SP_Store_GetActInfo act_num = 0"));
            break;
        }
        
        ems_act_info_ptr = EMA_UTILS_MALLOC(PNULL, act_num * sizeof(EMS_ActInfo));
        EMA_UTILS_BREAKNULL(ems_act_info_ptr, ret, "no memory");
        
        EMA_UTILS_MEMSET(ems_act_info_ptr, 0x00, act_num * sizeof(EMS_ActInfo));
        
        ret = HEMSSTORE_GetActInfo(store_mgr_ptr->qee_store_ptr, 0, (unsigned int*)&act_num, ems_act_info_ptr);
        EMA_UTILS_BREAKIF(ret, "get act info failed");
        
        for(i = 0; i < act_num; i++)
        {
            act_info_ptr = EMA_UTILS_MALLOC(store_mgr_ptr->mem_mgr_ptr,sizeof(EMASP_ACTINFO_T));
            EMA_UTILS_BREAKNULL(act_info_ptr,ret,"MMIEMAIL_SP_Store_GetActInfo,malloc pstemaactinfo failed");
            
            EMA_UTILS_MEMSET(act_info_ptr, 0, sizeof(EMASP_ACTINFO_T));
            
            act_info_ptr->act_id = ems_act_info_ptr[i].uiActId;
            EMA_UTILS_LOG_INFO(("MMIEMAIL_SP_Store_GetActInfo uiActId = %d", ems_act_info_ptr[i].uiActId));
            ret = SP_Store_GetActUnreadNum(store_handle_ptr,ems_act_info_ptr[i].uiActId,&act_info_ptr->email_num);
            EMA_UTILS_BREAKIF(ret, "MMIEMAIL_SP_Store_GetActInfo:get act unread act_num failed");
            
            EMA_UTILS_MEMCPY(act_info_ptr->act_name,ems_act_info_ptr[i].wazActName,EMS_ACCOUNT_NAME_MAX_LEN * 2);
            
            ret = MMIEMAIL_VectorInsert(vector_model_ptr,act_info_ptr);
            EMA_UTILS_BREAKIF(ret, "MMIEMAIL_SP_Store_GetActInfo:add emaactinfo failed");
        }

    } while(0);
    
    EMA_UTILS_FREEIF(PNULL, ems_act_info_ptr);
    
    return ret;
}

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
                                             )
{
    EMAIL_STOREMGR_T       *store_mgr_ptr = (EMAIL_STOREMGR_T*)store_handle_ptr;
    HConfigMgr     cfg_mgr_ptr = PNULL;
    int32          ret = EMAIL_SUCCESS;
    uint32         opt_id = SP_Store_EMAOpt_Convent_EMEOpt(item);
    
    if (PNULL == store_mgr_ptr || PNULL == store_mgr_ptr->parent_handle_ptr || PNULL == store_mgr_ptr->qee_store_ptr || PNULL == item_val_ptr || 0  == opt_id)
    {
        EMA_UTILS_LOG_ERROR(("~~~~~~~~~MMIEMAIL_SP_Store_SetActCfgItem: Bad paramter!~~~~~~~~~~~~~"));
        return EMAIL_BADPARAM;
    }
    
    if (EMA_SP_ACTIVE_ACCOUNT == act_id)
    {
        cfg_mgr_ptr = MMIEMAIL_SP_GetCfgHandle(store_mgr_ptr->parent_handle_ptr);
        if (PNULL == cfg_mgr_ptr)
        {
            EMA_UTILS_LOG_ERROR(("~~~~~~~~~MMIEMAIL_SP_Store_SetActCfgItem: get cfgmgr failed!~~~~~~~~~~~~~"));
            return EMAIL_ERROR;
        }
        
        ret = MMIEMAIL_SP_CFG_GetActiveAct(cfg_mgr_ptr,&act_id);
        if (EMAIL_SUCCESS != ret)
        {
            EMA_UTILS_LOG_ERROR(("MMIEMAIL_SP_Store_SetActCfgItem: MMIEMAIL_SP_CFG_GetActiveAct failed!~"));
            return EMAIL_ERROR;
        }
    }
    
    return HEMSSTORE_SetActCfgItem(store_mgr_ptr->qee_store_ptr, act_id, (EMS_PRFITEM_ID)opt_id, (EMS_PRFITEM_VAL*)item_val_ptr);
}


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
                                             )
{
    EMAIL_STOREMGR_T       *store_mgr_ptr = (EMAIL_STOREMGR_T*)store_handle_ptr;
    HConfigMgr     cfg_mgr_ptr = PNULL;
    int32          ret = EMAIL_SUCCESS;
    EMASP_MSG_OPT  *temp_ptr = PNULL;
    
    if (PNULL == store_mgr_ptr || PNULL == store_mgr_ptr->parent_handle_ptr || PNULL == store_mgr_ptr->qee_store_ptr || PNULL == setting_opt_ptr)
    {
        EMA_UTILS_LOG_ERROR(("~~~~~~~~~MMIEMAIL_SP_Store_SetActCfgItem: Bad paramter!~~~~~~~~~~~~~"));
        return EMAIL_BADPARAM;
    }
    
    if (EMA_SP_ACTIVE_ACCOUNT == act_id)
    {
        cfg_mgr_ptr = MMIEMAIL_SP_GetCfgHandle(store_mgr_ptr->parent_handle_ptr);
        if (PNULL == cfg_mgr_ptr)
        {
            EMA_UTILS_LOG_ERROR(("~~~~~~~~~MMIEMAIL_SP_Store_SetActCfgItem: get cfgmgr failed!~~~~~~~~~~~~~"));
            return EMAIL_ERROR;
        }
        
        ret = MMIEMAIL_SP_CFG_GetActiveAct(cfg_mgr_ptr,&act_id);
        if (EMAIL_SUCCESS != ret)
        {
            EMA_UTILS_LOG_ERROR(("MMIEMAIL_SP_Store_SetActCfgItem: MMIEMAIL_SP_CFG_GetActiveAct failed!~"));
            return EMAIL_ERROR;
        }
    }
    

    temp_ptr = setting_opt_ptr;
    while(EMASPMSGOPT_END != temp_ptr->nId)
    {
        temp_ptr->nId = SP_Store_EMAOpt_Convent_EMEOpt(temp_ptr->nId);
        temp_ptr++;
    }
    
    return HEMSSTORE_SetActCfgItemEx(store_mgr_ptr->qee_store_ptr, act_id, (EMSMsgOpt*)setting_opt_ptr);
}

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
                                             )
{
    EMAIL_STOREMGR_T    *store_mgr_ptr = (EMAIL_STOREMGR_T*)store_handle_ptr;
    HConfigMgr  cfg_mgr_ptr = PNULL;
    int32       ret = EMAIL_SUCCESS;
    uint32      opt_id = SP_Store_EMAOpt_Convent_EMEOpt(item);
    
    if (PNULL == store_mgr_ptr || PNULL == store_mgr_ptr->parent_handle_ptr || PNULL == store_mgr_ptr->qee_store_ptr || PNULL == item_val_ptr)
    {
        EMA_UTILS_LOG_ERROR(("~~~~~~~~~MMIEMAIL_SP_Store_GetActCfgItem: Bad paramter!~~~~~~~~~~~~~"));
        return EMAIL_BADPARAM;
    }
    
    if (EMA_SP_ACTIVE_ACCOUNT == act_id)
    {
        cfg_mgr_ptr = MMIEMAIL_SP_GetCfgHandle(store_mgr_ptr->parent_handle_ptr);
        if (PNULL == cfg_mgr_ptr)
        {
            EMA_UTILS_LOG_ERROR(("~~~~~~~~~MMIEMAIL_SP_Store_GetActCfgItem: get cfgmgr failed!~~~~~~~~~~~~~"));
            return EMAIL_ERROR;
        }
        
        ret = MMIEMAIL_SP_CFG_GetActiveAct(cfg_mgr_ptr,&act_id);
        if (EMAIL_SUCCESS != ret)
        {
            EMA_UTILS_LOG_ERROR(("MMIEMAIL_SP_Store_GetActCfgItem: MMIEMAIL_SP_CFG_GetActiveAct failed!~"));
            return EMAIL_ERROR;
        }
    }
    
    return HEMSSTORE_GetActCfgItem(store_mgr_ptr->qee_store_ptr, act_id, (EMS_PRFITEM_ID)opt_id, (EMS_PRFITEM_VAL*)item_val_ptr);
}

/**************************************************************************************/
// Description : email sp store, save email datecache.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC int32 MMIEMAIL_SP_Store_SaveEmail(
                                         HStoreMgr  store_handle_ptr,
                                         EMAIL_MSGDATACACHE_T *data_cache_ptr
                                         )
{
    int         ret = EMAIL_ERROR;
    EMAIL_STOREMGR_T    *store_mgr_ptr = (EMAIL_STOREMGR_T*)store_handle_ptr;
    
    if (PNULL == store_mgr_ptr || PNULL == store_mgr_ptr->qee_store_ptr || PNULL == data_cache_ptr)
    {
        EMA_UTILS_LOG_ERROR(("~~~~~~~~~MMIEMAIL_SP_Store_SaveEmail: Bad paramter!~~~~~~~~~~~~~~~~"));
        return EMAIL_BADPARAM;
    }
    
    do 
    {
        data_cache_ptr->folder_id = E_DRAFT_INDEX;
        
        if (data_cache_ptr->attach_num > 0)
        {
            data_cache_ptr->mail_status = EMA_BASIC_DRAFT_WITH_ATTACH;
        }
        else
        {
            data_cache_ptr->mail_status = EMA_BASIC_DRAFT;
        }
        
        ret = SP_Store_SaveOrUpdateEmail(store_handle_ptr, data_cache_ptr, TRUE);
        
    } while(0);
    
    return ret;
}

/**************************************************************************************/
// Description : email sp store, save or update email.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL int32 SP_Store_SaveOrUpdateEmail(
                                       HStoreMgr store_handle_ptr,
                                       EMAIL_MSGDATACACHE_T *data_cache_ptr,
                                       BOOLEAN is_save
                                       )
{
    HEMSMsg           *qee_msg_ptr = PNULL;
    HEMSMsgPart       *qee_msg_part_ptr = PNULL;
    HEMSStore         *qee_store_ptr = PNULL;
    EMSMsgOpt         qee_mo_opt[2] = {0};
    EMSMsgOpt         qee_mpo_opt[2] = {0};
    wchar             *opt_val_ptr = PNULL;
    wchar             *item_ptr = PNULL;
    uint32            len = 0;
    uint32            item_len = 0;
    uint32            i = 0;
    uint32            num = 0;
    int32             ret = EMAIL_SUCCESS;
    EMAIL_STOREMGR_T  *store_mgr_ptr = (EMAIL_STOREMGR_T*)store_handle_ptr;
    HConfigMgr        cfg_mgr_ptr = PNULL;
    uint32            act_id = 0;
    uint16            spit = (uint16)(',');
    
    if (PNULL == store_mgr_ptr || PNULL == store_mgr_ptr->parent_handle_ptr || PNULL == store_mgr_ptr->qee_store_ptr/* || PNULL == store_mgr_ptr->piShell*/ || PNULL == data_cache_ptr)
    {
        EMA_UTILS_LOG_ERROR(("~~~~~~~~~SP_Store_SaveMsg: Bad paramter!~~~~~~~~~~~~~~~~~~"));
        return EMAIL_BADPARAM;
    }
    
    do 
    {
        if (EMA_SP_ACTIVE_ACCOUNT == data_cache_ptr->act_id)
        {
            cfg_mgr_ptr = MMIEMAIL_SP_GetCfgHandle(store_mgr_ptr->parent_handle_ptr);
            if (PNULL == cfg_mgr_ptr)
            {
                EMA_UTILS_LOG_ERROR(("~~~~~~~~~SP_Store_SetActCfgItem: get cfgmgr failed!~~~~~~~~~~~~~"));
                return EMAIL_ERROR;
            }
            ret = MMIEMAIL_SP_CFG_GetActiveAct(cfg_mgr_ptr,&act_id);
            if (EMAIL_SUCCESS != ret)
            {
                EMA_UTILS_LOG_ERROR(("SP_Store_SetActCfgItem: MMIEMAIL_SP_CFG_GetActiveAct failed!~"));
                return EMAIL_ERROR;
            }
        }
        else
        {
            act_id = data_cache_ptr->act_id;
        }
        
        qee_store_ptr = store_mgr_ptr->qee_store_ptr;
        
        qee_msg_ptr = HEMSMSG_New(PNULL);
        EMA_UTILS_BREAKNULL(qee_msg_ptr, ret, "create msg failed");
        
        /*----------------ADD HEAD OPT-----------------------------------*/
        /* Add recipient */
        {
            char *str_temp_ptr = PNULL;
            
            num = data_cache_ptr->to_addrs_num;
            if (num > 0)
            {
                len = num * (EMA_MAX_ADDR_LEN + 10) * sizeof(wchar);
                opt_val_ptr = (wchar *)EMA_UTILS_MALLOC(store_mgr_ptr->mem_mgr_ptr,len);
                EMA_UTILS_BREAKNULL(opt_val_ptr, ret, "Malloc opt_val_ptr failed!");
                EMA_UTILS_MEMSET(opt_val_ptr, 0, len);
                
                qee_mo_opt[0].nId = EMS_MSGOPT_TO;
                for (i = 0; i < num; i++)
                {
                    uint32 uiWstrLen = 0;
                    
                    item_len = (EMA_MAX_ADDR_LEN + 1) * sizeof(wchar);
                    item_ptr = (wchar *)EMA_UTILS_MALLOC(store_mgr_ptr->mem_mgr_ptr,item_len);
                    EMA_UTILS_BREAKNULL(item_ptr, ret, "Malloc item_ptr failed!");
                    EMA_UTILS_MEMSET(item_ptr, 0, item_len);
                    
                    if (1 == num || (i == (num - 1)))
                    {
                        //EMA_UTILS_WSPRINTF(item_ptr, item_len, L"%s", data_cache_ptr->to_addrs[i]);
                        uiWstrLen = EMA_UTILS_WSTRLEN(data_cache_ptr->to_addrs[i]) * sizeof(wchar);
                        EMA_UTILS_MEMCPY(item_ptr, data_cache_ptr->to_addrs[i], uiWstrLen);
                        MMIAPICOM_Wstrncat(opt_val_ptr, item_ptr, len);
                    }
                    else
                    {
                        //EMA_UTILS_WSPRINTF(item_ptr, item_len, L"%s,", data_cache_ptr->to_addrs[i]);
                        uiWstrLen = EMA_UTILS_WSTRLEN(data_cache_ptr->to_addrs[i]) * sizeof(wchar);
                        EMA_UTILS_MEMCPY(item_ptr, data_cache_ptr->to_addrs[i], uiWstrLen);
                        EMA_UTILS_MEMCPY(item_ptr + EMA_UTILS_WSTRLEN(data_cache_ptr->to_addrs[i]), &spit, sizeof(uint16));
                        MMIAPICOM_Wstrncat(opt_val_ptr, item_ptr, len);
                    }
                    
                    EMA_UTILS_FREE(store_mgr_ptr->mem_mgr_ptr,item_ptr);
                }
                
                if (EMAIL_SUCCESS != ret)
                {
                    break;
                }
                
                str_temp_ptr = EMA_UTILS_WSTRTOUTF8(store_mgr_ptr->mem_mgr_ptr, opt_val_ptr);
                EMA_UTILS_FREE(store_mgr_ptr->mem_mgr_ptr, opt_val_ptr);
                
                qee_mo_opt[0].pVal = (void*)str_temp_ptr;
                ret = HEMSMSG_AddOpt(qee_msg_ptr, &qee_mo_opt[0]);
                EMA_UTILS_BREAKIF(ret, "Add qee_mo_opt[0] failed!");
                EMA_UTILS_FREEIF(store_mgr_ptr->mem_mgr_ptr,str_temp_ptr);
            }
        }
        
        /* Add Cc */
        {
            char *str_temp_ptr = PNULL;
            
            num = data_cache_ptr->cc_addrs_num;
            if (num > 0)
            {
                len = num * (EMA_MAX_ADDR_LEN + 10) * sizeof(wchar);
                opt_val_ptr = (wchar*)EMA_UTILS_MALLOC(store_mgr_ptr->mem_mgr_ptr,len);
                EMA_UTILS_BREAKNULL(opt_val_ptr, ret, "Malloc opt_val_ptr failed!");
                EMA_UTILS_MEMSET(opt_val_ptr, 0, len);
                
                qee_mo_opt[0].nId = EMS_MSGOPT_CC;
                for (i = 0; i < num; i++)
                {
                    uint32 uiWstrLen = 0;
                    item_len = (EMA_MAX_ADDR_LEN + 1) * sizeof(wchar);
                    item_ptr = (wchar *)EMA_UTILS_MALLOC(store_mgr_ptr->mem_mgr_ptr,item_len);
                    EMA_UTILS_BREAKNULL(item_ptr, ret, "Malloc item_ptr failed!");
                    EMA_UTILS_MEMSET(item_ptr, 0, item_len);
                    
                    if (1 == num || (i == (num - 1)))
                    {
                        //EMA_UTILS_WSPRINTF(item_ptr, item_len, L"%s", data_cache_ptr->cc_addrs[i]);
                        uiWstrLen = EMA_UTILS_WSTRLEN(data_cache_ptr->cc_addrs[i]) * sizeof(wchar);
                        EMA_UTILS_MEMCPY(item_ptr, data_cache_ptr->cc_addrs[i], uiWstrLen);
                        MMIAPICOM_Wstrncat(opt_val_ptr, item_ptr, len);
                    }
                    else
                    {
                        //EMA_UTILS_WSPRINTF(item_ptr, item_len, L"%s,", data_cache_ptr->cc_addrs[i]);
                        uiWstrLen = EMA_UTILS_WSTRLEN(data_cache_ptr->cc_addrs[i]) * sizeof(wchar);
                        EMA_UTILS_MEMCPY(item_ptr, data_cache_ptr->cc_addrs[i], uiWstrLen);
                        EMA_UTILS_MEMCPY(item_ptr + EMA_UTILS_WSTRLEN(data_cache_ptr->cc_addrs[i]), &spit, sizeof(uint16));
                        MMIAPICOM_Wstrncat(opt_val_ptr, item_ptr, len);
                    }  
                    EMA_UTILS_FREE(store_mgr_ptr->mem_mgr_ptr,item_ptr);
                }
                
                if (EMAIL_SUCCESS != ret)
                {
                    break;
                }
                
                str_temp_ptr = EMA_UTILS_WSTRTOUTF8(store_mgr_ptr->mem_mgr_ptr, opt_val_ptr);
                EMA_UTILS_FREE(store_mgr_ptr->mem_mgr_ptr, opt_val_ptr);
                
                qee_mo_opt[0].pVal = (void*)str_temp_ptr;
                ret = HEMSMSG_AddOpt(qee_msg_ptr, &qee_mo_opt[0]);
                EMA_UTILS_BREAKIF(ret, "Add qee_mo_opt[0] failed!");
                EMA_UTILS_FREEIF(store_mgr_ptr->mem_mgr_ptr,str_temp_ptr);
            }
        }
        
        /* Add Bcc */
        {
            char *str_temp_ptr = PNULL;
            
            num = data_cache_ptr->bcc_addrs_num;
            if (num > 0)
            {
                len = num * (EMA_MAX_ADDR_LEN + 10) * sizeof(wchar);
                opt_val_ptr = (wchar*)EMA_UTILS_MALLOC(store_mgr_ptr->mem_mgr_ptr,len);
                EMA_UTILS_BREAKNULL(opt_val_ptr, ret, "Malloc opt_val_ptr failed!");
                EMA_UTILS_MEMSET(opt_val_ptr, 0, len);
                
                qee_mo_opt[0].nId = EMS_MSGOPT_BCC;
                for (i = 0; i < num; i++)
                {
                    uint32 uiWstrLen = 0;
                    item_len = (EMA_MAX_ADDR_LEN + 1) * sizeof(wchar);
                    item_ptr = (wchar *)EMA_UTILS_MALLOC(store_mgr_ptr->mem_mgr_ptr,item_len);
                    EMA_UTILS_BREAKNULL(item_ptr, ret, "Malloc item_ptr failed!");
                    EMA_UTILS_MEMSET(item_ptr, 0, item_len);
                    
                    if (1 == num || (i == (num - 1)))
                    {
                        //EMA_UTILS_WSPRINTF(item_ptr, item_len, L"%s", data_cache_ptr->bcc_addrs[i]);
                        uiWstrLen = EMA_UTILS_WSTRLEN(data_cache_ptr->bcc_addrs[i]) * sizeof(wchar);
                        EMA_UTILS_MEMCPY(item_ptr, data_cache_ptr->bcc_addrs[i], uiWstrLen);
                        MMIAPICOM_Wstrncat(opt_val_ptr, item_ptr, len);
                    }
                    else
                    {
                        //EMA_UTILS_WSPRINTF(item_ptr, item_len, L"%s,", data_cache_ptr->bcc_addrs[i]);
                        uiWstrLen = EMA_UTILS_WSTRLEN(data_cache_ptr->bcc_addrs[i]) * sizeof(wchar);
                        EMA_UTILS_MEMCPY(item_ptr, data_cache_ptr->bcc_addrs[i], uiWstrLen);
                        EMA_UTILS_MEMCPY(item_ptr + EMA_UTILS_WSTRLEN(data_cache_ptr->bcc_addrs[i]), &spit, sizeof(uint16));
                        MMIAPICOM_Wstrncat(opt_val_ptr, item_ptr, len);
                    }  
                    EMA_UTILS_FREE(store_mgr_ptr->mem_mgr_ptr,item_ptr);
                }
                if (EMAIL_SUCCESS != ret)
                {
                    break;
                }
                
                str_temp_ptr = EMA_UTILS_WSTRTOUTF8(store_mgr_ptr->mem_mgr_ptr, opt_val_ptr);
                EMA_UTILS_FREE(store_mgr_ptr->mem_mgr_ptr, opt_val_ptr);
                
                qee_mo_opt[0].pVal = (void*)str_temp_ptr;
                ret = HEMSMSG_AddOpt(qee_msg_ptr, &qee_mo_opt[0]);
                EMA_UTILS_BREAKIF(ret, "Add qee_mo_opt[0] failed!");
                EMA_UTILS_FREEIF(store_mgr_ptr->mem_mgr_ptr,str_temp_ptr);
            }
        }
        
        /* Add Date */
        {
            uint32 time = 0;
            time = EMA_UTILS_GETSECONDS;
            qee_mo_opt[0].pVal = (void*)time;
            qee_mo_opt[0].nId = EMS_MSGOPT_DATE;
            ret = HEMSMSG_AddOpt(qee_msg_ptr, &qee_mo_opt[0]);
            EMA_UTILS_BREAKIF(ret, "Add qee_mo_opt[0] failed!");
        }
        
        /* Add time-zone */
        {
            int32 time_zone = 0;
#if 0
            float time_zone_temp = 0;
            //#ifdef WORLD_CLOCK_SUPPORT
            time_zone_temp = MMIAPIACC_GetLocalTimeZone();
            if (time_zone_temp > 12.75 || time_zone_temp < -12.0)
            {
                time_zone_temp = 8.0;
            }
            time_zone = (int32)(time_zone_temp * 3600);
            //#endif
#endif
            qee_mo_opt[0].pVal = (void*)time_zone;
            qee_mo_opt[0].nId = EMS_MSGOPT_TIME_ZONE;
            ret = HEMSMSG_AddOpt(qee_msg_ptr, &qee_mo_opt[0]);
            EMA_UTILS_BREAKIF(ret, "Add qee_mo_opt[0] failed!");
        }
        
        /* Add account id */
        qee_mo_opt[0].nId = EMS_MSGOPT_ACCOUNTID;
        qee_mo_opt[0].pVal = (void*)act_id; 
        ret = HEMSMSG_AddOpt(qee_msg_ptr, &qee_mo_opt[0]);
        EMA_UTILS_BREAKIF(ret, "Add qee_mo_opt[0] failed!");
        /* add box id*/
        qee_mo_opt[0].nId = EMS_MSGOPT_BOXID;
        qee_mo_opt[0].pVal = (void*)data_cache_ptr->folder_id;
        ret = HEMSMSG_AddOpt(qee_msg_ptr, &qee_mo_opt[0]);
        EMA_UTILS_BREAKIF(ret, "Add qee_mo_opt[0] failed!");
        
        /* Add subject */
        qee_mo_opt[0].nId = EMS_MSGOPT_SUBJECT;
        if((PNULL == data_cache_ptr->subject_ptr)||(0 == EMA_UTILS_WSTRLEN(data_cache_ptr->subject_ptr)))
        {
            qee_mo_opt[0].pVal = PNULL; 
        }
        else
        {
            char* str_temp_ptr = PNULL;
            
            str_temp_ptr = EMA_UTILS_WSTRTOUTF8(store_mgr_ptr->mem_mgr_ptr, data_cache_ptr->subject_ptr);
            qee_mo_opt[0].pVal = (void*)str_temp_ptr; 
            ret = HEMSMSG_AddOpt(qee_msg_ptr, &qee_mo_opt[0]);
            EMA_UTILS_BREAKIF(ret, "Add qee_mo_opt[0] failed!");
            EMA_UTILS_FREEIF(store_mgr_ptr->mem_mgr_ptr, str_temp_ptr);
        }
        
        /* Add status */
        qee_mo_opt[0].nId = EMS_MSGOPT_STATUS;
        qee_mo_opt[0].pVal = (void*)MMIEMAIL_SP_Store_ConventToEmeStatus(data_cache_ptr->mail_status); 
        ret = HEMSMSG_AddOpt(qee_msg_ptr, &qee_mo_opt[0]);
        EMA_UTILS_BREAKIF(ret, "add status failed");
        
        /* Add priority */
        qee_mo_opt[0].nId = EMS_MSGOPT_PRIO;
        if (EMA_SETTING_FOR_ONE_MSG == data_cache_ptr->uipriority)
        {
            qee_mo_opt[0].pVal = (void*)EMA_PRIORITY_NORMAL; 
        }
        else
        {
            qee_mo_opt[0].pVal = (void*)data_cache_ptr->uipriority; 
        }
        ret = HEMSMSG_AddOpt(qee_msg_ptr, &qee_mo_opt[0]);
        EMA_UTILS_BREAKIF(ret, "add status failed");
        
        /* Add bedown */
        qee_mo_opt[0].nId = EMS_MSGOPT_BEDOWN;
        qee_mo_opt[0].pVal = (void*)data_cache_ptr->be_down; 
        ret = HEMSMSG_AddOpt(qee_msg_ptr, &qee_mo_opt[0]);
        EMA_UTILS_BREAKIF(ret, "add status failed");
        
        /* add msgtype*/
        qee_mo_opt[0].nId = EMS_MSGOPT_MSG_TYPE;
        qee_mo_opt[0].pVal = (void*)EMSMSG_TYPE_EMAIL;
        ret = HEMSMSG_AddOpt(qee_msg_ptr, &qee_mo_opt[0]);
        EMA_UTILS_BREAKIF(ret, "Add qee_mo_opt[0] failed!");
        
        //       if (PNULL != data_cache_ptr->awDisposNotifTo)
        //       {
        //          qee_mo_opt[0].nId = QEEMSG_DISPOS_NOTIF_TO;
        //          qee_mo_opt[0].pVal = (void*)data_cache_ptr->awDisposNotifTo; 
        //          ret = HEMSMSG_AddOpt(qee_msg_ptr, &qee_mo_opt[0]);
        //          EMA_UTILS_BREAKIF(ret, "add status failed");
        //       }
        
        /* Add from and Add DisposNotifTo*/
        {
            EMASP_PRFITEM_VAL val = {0};
            WE_BOOL          is_read_reply = 0;
            
            if (EMA_SETTING_FOR_ONE_MSG == data_cache_ptr->read_confirm)
            {
                ret = HEMSSTORE_GetActCfgItem(qee_store_ptr, act_id, EMS_PRFITEM_READ_RECEIPT, (EMS_PRFITEM_VAL*)&val);
                EMA_UTILS_BREAKIF(ret, "SP_Store_SaveOrUpdateEmail,HEMSSTORE_GetActCfgItem failed!");
                is_read_reply = (WE_BOOL)(uint32)val.pData;
            }
            else
            {
                is_read_reply = (BOOLEAN)data_cache_ptr->read_confirm;
            }
            
            ret = HEMSSTORE_GetActCfgItem(qee_store_ptr, act_id, EMS_PRFITEM_ACCOUNT_EMAIL, (EMS_PRFITEM_VAL*)&val);
            EMA_UTILS_BREAKIF(ret, "SP_Store_SaveOrUpdateEmail,HEMSSTORE_GetActCfgItem failed!");
            
            {
                char *str_temp_ptr = PNULL;
                
                str_temp_ptr = EMA_UTILS_WSTRTOUTF8(store_mgr_ptr->mem_mgr_ptr, val.pData);
                
                qee_mo_opt[0].nId = EMS_MSGOPT_FROM;
                qee_mo_opt[0].pVal = str_temp_ptr;
                ret = HEMSMSG_AddOpt(qee_msg_ptr, &qee_mo_opt[0]);
                EMA_UTILS_BREAKIF(ret, "Add qee_mo_opt[0] failed!");
                EMA_UTILS_FREEIF(store_mgr_ptr->mem_mgr_ptr, str_temp_ptr);
            }
            
            if (is_read_reply)
            {
                char *str_temp_ptr = PNULL;
                
                str_temp_ptr = EMA_UTILS_WSTRTOUTF8(store_mgr_ptr->mem_mgr_ptr, val.pData);
                
                qee_mo_opt[0].nId = EMS_MSGOPT_DISP_NOTIFY_TO;
                qee_mo_opt[0].pVal = str_temp_ptr;
                ret = HEMSMSG_AddOpt(qee_msg_ptr, &qee_mo_opt[0]);
                EMA_UTILS_BREAKIF(ret, "Add qee_mo_opt[0] failed!");
                EMA_UTILS_FREEIF(store_mgr_ptr->mem_mgr_ptr, str_temp_ptr);
            }
            // EMA_UTILS_FREEIF(store_mgr_ptr->mem_mgr_ptr,val.pData);
        }
        
        /*add reply to*/
        {
            EMASP_PRFITEM_VAL val = {0};
            char *str_temp_ptr = PNULL;
            
            ret = HEMSSTORE_GetActCfgItem(qee_store_ptr,act_id, EMS_PRFITEM_REPLY_TO_EMAIL,(EMS_PRFITEM_VAL*)&val);
            EMA_UTILS_BREAKIF(ret, "SP_Store_SaveOrUpdateEmail,HEMSSTORE_GetActCfgItem failed!");
            
            str_temp_ptr = EMA_UTILS_WSTRTOUTF8(store_mgr_ptr->mem_mgr_ptr, val.pData);
            
            qee_mo_opt[0].nId = EMS_MSGOPT_REPLY_TO;
            qee_mo_opt[0].pVal = str_temp_ptr;
            ret = HEMSMSG_AddOpt(qee_msg_ptr, &qee_mo_opt[0]);
            EMA_UTILS_BREAKIF(ret, "Add qee_mo_opt[0] failed!");
            EMA_UTILS_FREEIF(store_mgr_ptr->mem_mgr_ptr, str_temp_ptr);
            
            //EMA_UTILS_FREEIF(store_mgr_ptr->mem_mgr_ptr,val.pData);
        }
        
        
        /*----------------------ADD BODY OPT-----------------------------------*/
        if(PNULL != data_cache_ptr->data_buff_ptr)
        {
            char *str_temp_ptr = PNULL;
            
            str_temp_ptr = EMA_UTILS_WSTRTOUTF8(store_mgr_ptr->mem_mgr_ptr, data_cache_ptr->data_buff_ptr);
            
            qee_msg_part_ptr = HEMSMSGPART_New(PNULL);
            EMA_UTILS_BREAKNULL(qee_msg_part_ptr, ret, "create msgpart failed");
            
            /* Add data buf */
            qee_mpo_opt[0].nId = EMS_PARTOPT_DATABUF;
            qee_mpo_opt[0].pVal = (void*)str_temp_ptr;
            ret = HEMSMSGPART_AddOpt(qee_msg_part_ptr, &qee_mpo_opt[0]);
            EMA_UTILS_BREAKIF(ret, "Add qee_mo_opt[0] failed!");
            EMA_UTILS_FREEIF(store_mgr_ptr->mem_mgr_ptr, str_temp_ptr);
            
            /* Add content type */
            qee_mpo_opt[0].nId = EMS_PARTOPT_PARTTYPE;
            qee_mpo_opt[0].pVal = (void*)EMSPART_TYPE_TEXT_PLAIN;
            ret = HEMSMSGPART_AddOpt(qee_msg_part_ptr, &qee_mpo_opt[0]);
            EMA_UTILS_BREAKIF(ret, "Add qee_mo_opt[0] failed!");
            
            /* Add source type */
            qee_mpo_opt[0].nId = EMS_PARTOPT_SRCTYPE;
            qee_mpo_opt[0].pVal = (void*)EMSPART_SRCTYPE_BUFFER;
            ret = HEMSMSGPART_AddOpt(qee_msg_part_ptr, &qee_mpo_opt[0]);
            EMA_UTILS_BREAKIF(ret, "Add qee_mo_opt[0] failed!");
            
            ret = HEMSMSG_AddMessagePart(qee_msg_ptr, qee_msg_part_ptr, 1);
            EMA_UTILS_BREAKIF(ret, "SP_Store_SaveOrUpdateEmail,IQEEMULTIPARTMSG_AddMessagePart failed!");
            
            HEMSMSGPART_Delete(qee_msg_part_ptr);
        }
        
        /*---------------------Add Attachment---------------------------------- */
        {
            num = data_cache_ptr->attach_num;
            if (num > 0)
            {
                char *str_temp_ptr = PNULL;
                
                /* Add attachment URI */
                for (i = 0; i < num; i++)
                {
                    qee_msg_part_ptr = HEMSMSGPART_New(PNULL);
                    EMA_UTILS_BREAKNULL(qee_msg_part_ptr, ret, "create msgpart failed");
                    
                    /* Add attachment name */
                    str_temp_ptr = EMA_UTILS_WSTRTOUTF8(store_mgr_ptr->mem_mgr_ptr, data_cache_ptr->attach_name[i]);
                    qee_mpo_opt[0].nId = EMS_PARTOPT_ATTACHNAME;
                    qee_mpo_opt[0].pVal = (void*)str_temp_ptr;
                    ret = HEMSMSGPART_AddOpt(qee_msg_part_ptr, &qee_mpo_opt[0]);
                    EMA_UTILS_BREAKIF(ret, "Add qee_mo_opt[0] failed!");
                    EMA_UTILS_FREEIF(store_mgr_ptr->mem_mgr_ptr, str_temp_ptr);
                    
                    /* Add attachment content */
                    str_temp_ptr = EMA_UTILS_WSTRTOUTF8(store_mgr_ptr->mem_mgr_ptr, data_cache_ptr->attach_uri[i]);
                    qee_mpo_opt[0].nId = EMS_PARTOPT_DATABUF;
                    qee_mpo_opt[0].pVal = (void*)str_temp_ptr;
                    ret = HEMSMSGPART_AddOpt(qee_msg_part_ptr, &qee_mpo_opt[0]);
                    EMA_UTILS_BREAKIF(ret, "Add qee_mo_opt[0] failed!");
                    EMA_UTILS_FREEIF(store_mgr_ptr->mem_mgr_ptr, str_temp_ptr);
                    
                    /* Add content type */
                    qee_mpo_opt[0].nId = EMS_PARTOPT_PARTTYPE;
                    qee_mpo_opt[0].pVal = (void*)EMSPART_TYPE_ATTACHMENT;
                    ret = HEMSMSGPART_AddOpt(qee_msg_part_ptr, &qee_mpo_opt[0]);
                    EMA_UTILS_BREAKIF(ret, "Add qee_mo_opt[0] failed!");
                    
                    /* Add source type */
                    qee_mpo_opt[0].nId = EMS_PARTOPT_SRCTYPE;
                    qee_mpo_opt[0].pVal = (void*)EMSPART_SRCTYPE_FILE;
                    ret = HEMSMSGPART_AddOpt(qee_msg_part_ptr, &qee_mpo_opt[0]);
                    EMA_UTILS_BREAKIF(ret, "Add qee_mo_opt[0] failed!");
                    
                    ret = HEMSMSG_AddMessagePart(qee_msg_ptr, qee_msg_part_ptr, 1);
                    EMA_UTILS_BREAKIF(ret, "SP_Store_SaveOrUpdateEmail,IQEEMULTIPARTMSG_AddMessagePart failed!");
                    
                    HEMSMSGPART_Delete(qee_msg_part_ptr);
                }
            }
        }
        
        if (is_save)/* Save message*/
        {
            ret = HEMSSTORE_StoreMsg(qee_store_ptr, qee_msg_ptr);
            EMA_UTILS_BREAKIF(ret, "SP_Store_SaveOrUpdateEmail:IQEESTORE_StoreMsg failed!");
            
            /* Get Msg id and save it in data_cache_ptr */
            ret = HEMSMSG_GetOpt(qee_msg_ptr, EMS_MSGOPT_MSGID, 0, &qee_mo_opt[0]);
            EMA_UTILS_BREAKIF(ret, "get QEEMSG_OPT_MSGID failed!");
            data_cache_ptr->email_id = *((uint32*)(&qee_mo_opt[0].pVal));
        }
        else/* Update message*/
        {
            //         /* Add MsgId*/
            qee_mpo_opt[0].nId = EMS_MSGOPT_MSGID;
            qee_mpo_opt[0].pVal = (void*)data_cache_ptr->email_id;
            ret = HEMSMSG_AddOpt(qee_msg_ptr, &qee_mpo_opt[0]);
            EMA_UTILS_BREAKIF(ret, "add QEEMSG_OPT_MSGID failed!");
            //
            //         /* Add ActId*/
            //         qee_mpo_opt[0].nId = QEEMSG_OPT_ACCOUNTID;
            //         qee_mpo_opt[0].pVal = (void*)act_id;
            //         ret = HEMSMSG_AddOpt(qee_msg_ptr, &qee_mpo_opt[0]);
            //         EMA_UTILS_BREAKIF(ret, "add QEEMSG_OPT_ACCOUNTID failed!");
            //
            /* Add BoxId*/
            //         qee_mpo_opt[0].nId = QEEMSG_OPT_BOXID;
            //         qee_mpo_opt[0].pVal = (void*)E_DRAFT_INDEX;
            //         ret = HEMSMSG_AddOpt(qee_msg_ptr, &qee_mpo_opt[0]);
            //         EMA_UTILS_BREAKIF(ret, "add QEEMSG_OPT_BOXID failed!");
            
            ret = HEMSSTORE_UpdateMsg(qee_store_ptr, qee_msg_ptr);
            EMA_UTILS_BREAKIF(ret, "SP_Store_SaveOrUpdateEmail,HEMSSTORE_UpdateMsg failed!");
        }
   } while(0);
   
   if(PNULL != qee_msg_ptr)
   {
       HEMSMSG_Delete(qee_msg_ptr);
   }
   EMA_UTILS_FREEIF(store_mgr_ptr->mem_mgr_ptr,opt_val_ptr);
   EMA_UTILS_FREEIF(store_mgr_ptr->mem_mgr_ptr,item_ptr);
   return ret;
}

/**************************************************************************************/
// Description : email sp store, submit update to file
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL int32 SP_Store_UpdateSubmit(
                                 HEMSMsg           qee_msg_ptr,
                                 HEMSStore         qee_store_ptr,
                                 BOOLEAN          bHeadOnly
                                 )
{
    int32     ret = EMAIL_ERROR;
    
    if (PNULL == qee_msg_ptr || PNULL == qee_store_ptr)
    {
        EMA_UTILS_LOG_ERROR(("~~~~~~~~~SP_Store_UpdateSubmite: Bad paramter!~~~~~~~~~~~~~~~~~~"));
        return EMAIL_BADPARAM;
    }
    
    do 
    {   
        if(bHeadOnly)
        {
            ret = HEMSSTORE_UpdateMsgHeader(qee_store_ptr, qee_msg_ptr);
        }
        else
        {
            ret = HEMSSTORE_UpdateMsg(qee_store_ptr, qee_msg_ptr);
        }
        
        EMA_UTILS_BREAKIF(ret, "SP_Store_UpdateSubmite: SP_Store_UpdateSubmite failed!");
    } while(0);
    
    return  ret;
}

/**************************************************************************************/
// Description : email sp store, update email size.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL int32 SP_Store_UpdateSize(
                                HEMSMsg           qee_msg_ptr,
                                HEMSStore         qee_store_ptr,
                                EMAIL_MSGDATACACHE_T   *data_cache_ptr
                                )
{
    EMSMsgOpt      qee_mpo_opt[2] = {0};
    HEMSMsgPart    qee_msg_part_ptr = PNULL;
    int32          ret = EMAIL_ERROR;
    uint32         content_type = 0;
    char           *str_temp_ptr = PNULL;
    
    if (PNULL == qee_msg_ptr || PNULL == qee_store_ptr || PNULL == data_cache_ptr)
    {
        EMA_UTILS_LOG_ERROR(("~~~~~~~~~SP_Store_UpdateSize: Bad paramter!~~~~~~~~~~~~~~~~~~"));
        return EMAIL_BADPARAM;
    }
    
    do 
    {
        content_type = EMSPART_TYPE_TEXT_PLAIN;
        ret = HEMSMSG_GetMessagePart(qee_msg_ptr, EMS_MSGPART_ID_PART_TYPE, &content_type, &qee_msg_part_ptr);
        EMA_UTILS_BREAKIF(ret, "SP_Store_UpdateSize,get msg part failed!");
        
        /* Add data buf */
        str_temp_ptr = EMA_UTILS_WSTRTOUTF8(PNULL, data_cache_ptr->data_buff_ptr);
        qee_mpo_opt[0].nId = EMS_PARTOPT_DATABUF;
        qee_mpo_opt[0].pVal = (void*)str_temp_ptr;
        ret = HEMSMSGPART_UpdateOpt(qee_msg_part_ptr, 0,&qee_mpo_opt[0]);
        EMA_UTILS_BREAKIF(ret, "SP_Store_UpdateSize,HEMSMSGPART_UpdateOpt failed!");
        EMA_UTILS_FREEIF(PNULL, str_temp_ptr);
        
        /* Add source type */
        qee_mpo_opt[0].nId = EMS_PARTOPT_SRCTYPE;
        qee_mpo_opt[0].pVal = (void*)EMSPART_SRCTYPE_BUFFER;
        ret = HEMSMSGPART_UpdateOpt(qee_msg_part_ptr, 0,&qee_mpo_opt[0]);
        EMA_UTILS_BREAKIF(ret, "SP_Store_UpdateSize,HEMSMSGPART_UpdateOpt failed!");
        
    } while(0);
    
    HEMSMSGPART_Delete(qee_msg_part_ptr);
    
    return ret;
}

/**************************************************************************************/
// Description : email sp store, update to.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL int32 SP_Store_UpdateTo(
                              HStoreMgr        store_handle_ptr,
                              HEMSMsg          qee_msg_ptr,
                              HEMSStore        qee_store_ptr,
                              EMAIL_MSGDATACACHE_T  *data_cache_ptr
                              )
{
    uint32  num = 0;
    uint32  len = 0;
    uint32  i = 0;
    wchar *opt_val_ptr = PNULL;
    wchar *item_ptr = PNULL;
    char	 *item_temp_ptr = PNULL;
    char  *addr_temp_ptr = PNULL;
    uint32  item_len = 0;
    int32   ret = EMAIL_SUCCESS;
    EMSMsgOpt qee_mo_opt[2] = {0};
    EMAIL_STOREMGR_T* store_mgr_ptr = (EMAIL_STOREMGR_T*)store_handle_ptr;
    
    if (PNULL == store_mgr_ptr || PNULL == qee_msg_ptr || PNULL == qee_store_ptr || PNULL == data_cache_ptr)
    {
        EMA_UTILS_LOG_ERROR(("~~~~~~~~~SP_Store_UpdateTo: Bad paramter!~~~~~~~~~~~~~~~~~~"));
        return EMAIL_ERROR;
    }
    do 
    {
        char *str_temp_ptr = PNULL;
        
        num = data_cache_ptr->to_addrs_num;
        if (num > 0)
        {
            len = num * (EMA_MAX_ADDR_LEN + EMA_MAX_RECPT_NUM) * sizeof(wchar);
            opt_val_ptr = (wchar *)EMA_UTILS_MALLOC(store_mgr_ptr->mem_mgr_ptr,len);
            EMA_UTILS_BREAKNULL(opt_val_ptr, ret, "Malloc opt_val_ptr failed!");
            EMA_UTILS_MEMSET(opt_val_ptr, 0, len);
            
            qee_mo_opt[0].nId = EMS_MSGOPT_TO;
            for (i = 0; i < num; i++)
            {
                item_len = (EMA_MAX_ADDR_LEN + EMA_MAX_RECPT_NUM + EMA_MAX_RECPT_NUM);
                item_temp_ptr = (char*)EMA_UTILS_MALLOC(store_mgr_ptr->mem_mgr_ptr,item_len);
                EMA_UTILS_BREAKNULL(item_temp_ptr, ret, "SP_Store_UpdateEmail:Malloc item_ptr failed!");
                EMA_UTILS_MEMSET(item_temp_ptr, 0, item_len);
                addr_temp_ptr = EMA_UTILS_WSTRTOSTR(PNULL, data_cache_ptr->to_addrs[i]);
                EMA_UTILS_BREAKNULL(addr_temp_ptr, ret, "SP_Store_UpdateEmail:Malloc item_ptr failed!");
                
                if (1 == num || (i == (num - 1)))
                {
                    EMA_UTILS_SPRINTF(item_temp_ptr, "\" \"<%s>", addr_temp_ptr);
                }
                else
                {
                    EMA_UTILS_SPRINTF(item_temp_ptr, "\" \"<%s>,", addr_temp_ptr);
                }
                
                item_ptr = EMA_UTILS_STRTOWSTR(PNULL, item_temp_ptr);
                EMA_UTILS_BREAKNULL(item_ptr, ret, "SP_Store_UpdateEmail:Malloc item_ptr failed!");
                MMIAPICOM_Wstrncat(opt_val_ptr, item_ptr, len);
                EMA_UTILS_FREE(store_mgr_ptr->mem_mgr_ptr,item_ptr);
                EMA_UTILS_FREE(store_mgr_ptr->mem_mgr_ptr,item_temp_ptr);
                EMA_UTILS_FREE(store_mgr_ptr->mem_mgr_ptr,addr_temp_ptr);
            }
            
            if (EMAIL_SUCCESS != ret)
            {
                break;
            }
            
            str_temp_ptr = EMA_UTILS_WSTRTOUTF8(PNULL, opt_val_ptr);
            
            qee_mo_opt[0].pVal = (void*)str_temp_ptr;
            
            ret = HEMSMSG_UpdateOpt(qee_msg_ptr, 0, &qee_mo_opt[0]);
            EMA_UTILS_BREAKIF(ret, "SP_Store_UpdateTo:HEMSMSG_UpdateOpt failed!");
            
            EMA_UTILS_FREEIF(PNULL, str_temp_ptr);
                        
        } 
    } while(0);
    
    EMA_UTILS_FREEIF(store_mgr_ptr->mem_mgr_ptr,item_ptr);
    EMA_UTILS_FREEIF(store_mgr_ptr->mem_mgr_ptr,opt_val_ptr);
    
    return ret;
}

/**************************************************************************************/
// Description : email sp store, update CC.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL int32 SP_Store_UpdateCc(
                              HStoreMgr        store_handle_ptr,
                              HEMSMsg          qee_msg_ptr,
                              HEMSStore        qee_store_ptr,
                              EMAIL_MSGDATACACHE_T  *data_cache_ptr
                              )
{
    uint32  num = 0;
    uint32  len = 0;
    uint32  i = 0;
    wchar *opt_val_ptr = PNULL;
    wchar *item_ptr = PNULL;
    char	 *item_temp_ptr = PNULL;
    char  *addr_temp_ptr = PNULL;
    uint32  item_len = 0;
    int32   ret = EMAIL_ERROR;
    EMSMsgOpt qee_mo_opt[2] = {0};
    EMAIL_STOREMGR_T* store_mgr_ptr = (EMAIL_STOREMGR_T*)store_handle_ptr;
    
    if (PNULL == store_mgr_ptr ||PNULL == qee_msg_ptr || PNULL == qee_store_ptr || PNULL == data_cache_ptr)
    {
        EMA_UTILS_LOG_ERROR(("~~~~~~~~~SP_Store_UpdateCc: Bad paramter!~~~~~~~~~~~~~~~~~~"));
        return EMAIL_BADPARAM;
    }
    do 
    {
        char *str_temp_ptr = PNULL;
        
        num = data_cache_ptr->cc_addrs_num;
        if (num > 0)
        {
            len = num * (EMA_MAX_ADDR_LEN + EMA_MAX_RECPT_NUM) * sizeof(wchar);
            opt_val_ptr = (wchar *)EMA_UTILS_MALLOC(store_mgr_ptr->mem_mgr_ptr,len);
            EMA_UTILS_BREAKNULL(opt_val_ptr, ret, "SP_Store_UpdateCc:Malloc opt_val_ptr failed!");
            EMA_UTILS_MEMSET(opt_val_ptr, 0, len);
            
            qee_mo_opt[0].nId = EMS_MSGOPT_CC;
            for (i = 0; i < num; i++)
            {
                item_len = (EMA_MAX_ADDR_LEN + EMA_MAX_RECPT_NUM + EMA_MAX_RECPT_NUM);
                item_temp_ptr = (char*)EMA_UTILS_MALLOC(store_mgr_ptr->mem_mgr_ptr,item_len);
                EMA_UTILS_BREAKNULL(item_temp_ptr, ret, "SP_Store_UpdateEmail:Malloc item_ptr failed!");
                EMA_UTILS_MEMSET(item_temp_ptr, 0, item_len);
                addr_temp_ptr = EMA_UTILS_WSTRTOSTR(PNULL, data_cache_ptr->cc_addrs[i]);
                EMA_UTILS_BREAKNULL(addr_temp_ptr, ret, "SP_Store_UpdateEmail:Malloc item_ptr failed!");
                
                if (1 == num || (i == (num - 1)))
                {
                    EMA_UTILS_SPRINTF(item_temp_ptr, "\" \"<%s>", addr_temp_ptr);
                }
                else
                {
                    EMA_UTILS_SPRINTF(item_temp_ptr, "\" \"<%s>,", addr_temp_ptr);
                }
                
                item_ptr = EMA_UTILS_STRTOWSTR(PNULL, item_temp_ptr);
                EMA_UTILS_BREAKNULL(item_ptr, ret, "SP_Store_UpdateEmail:Malloc item_ptr failed!");
                MMIAPICOM_Wstrncat(opt_val_ptr, item_ptr, len);
                EMA_UTILS_FREE(store_mgr_ptr->mem_mgr_ptr,item_ptr);
                EMA_UTILS_FREE(store_mgr_ptr->mem_mgr_ptr,item_temp_ptr);
                EMA_UTILS_FREE(store_mgr_ptr->mem_mgr_ptr,addr_temp_ptr);
            }
            if (EMAIL_SUCCESS != ret)
            {
                break;
            }
            
            str_temp_ptr = EMA_UTILS_WSTRTOUTF8(PNULL, opt_val_ptr);
            
            qee_mo_opt[0].pVal = (void*)str_temp_ptr;
            
            ret = HEMSMSG_UpdateOpt(qee_msg_ptr, 0, &qee_mo_opt[0]);
            EMA_UTILS_BREAKIF(ret, "SP_Store_UpdateCc:HEMSMSG_UpdateOpt failed!");
            
            EMA_UTILS_FREEIF(PNULL, str_temp_ptr);
            
        } 
    } while(0);
    
    EMA_UTILS_FREEIF(store_mgr_ptr->mem_mgr_ptr,item_ptr);
    EMA_UTILS_FREEIF(store_mgr_ptr->mem_mgr_ptr,opt_val_ptr);
    return ret;
}

/**************************************************************************************/
// Description : email sp store, update BCC.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL int32 SP_Store_UpdateBcc(
                               HStoreMgr        store_handle_ptr,
                               HEMSMsg          qee_msg_ptr,
                               HEMSStore        qee_store_ptr,
                               EMAIL_MSGDATACACHE_T  *data_cache_ptr
                               )
{
    uint32  num = 0;
    uint32  len = 0;
    uint32  i = 0;
    wchar *opt_val_ptr = PNULL;
    wchar *item_ptr = PNULL;
    char	 *item_temp_ptr = PNULL;
    char  *addr_temp_ptr = PNULL;
    uint32  item_len = 0;
    int32   ret = EMAIL_ERROR;
    EMSMsgOpt qee_mo_opt[2] = {0};
    EMAIL_STOREMGR_T* store_mgr_ptr = (EMAIL_STOREMGR_T*)store_handle_ptr;
    
    if (PNULL == store_mgr_ptr ||PNULL == qee_msg_ptr || PNULL == qee_store_ptr || PNULL == data_cache_ptr)
    {
        EMA_UTILS_LOG_ERROR(("~~~~~~~~~SP_Store_UpdateCc: Bad paramter!~~~~~~~~~~~~~~~~~~"));
        return EMAIL_BADPARAM;
    }
    
    do 
    {
        char *str_temp_ptr = PNULL;
        
        num = data_cache_ptr->bcc_addrs_num;
        if (num > 0)
        {
            len = num * (EMA_MAX_ADDR_LEN + EMA_MAX_RECPT_NUM) * sizeof(wchar);
            opt_val_ptr = (wchar *)EMA_UTILS_MALLOC(store_mgr_ptr->mem_mgr_ptr,len);
            EMA_UTILS_BREAKNULL(opt_val_ptr, ret, "SP_Store_UpdateBcc:Malloc opt_val_ptr failed!");
            EMA_UTILS_MEMSET(opt_val_ptr, 0, len);
            
            qee_mo_opt[0].nId = EMS_MSGOPT_BCC;
            for (i = 0; i < num; i++)
            {
                item_len = (EMA_MAX_ADDR_LEN + EMA_MAX_RECPT_NUM + EMA_MAX_RECPT_NUM);
                item_temp_ptr = (char*)EMA_UTILS_MALLOC(store_mgr_ptr->mem_mgr_ptr,item_len);
                EMA_UTILS_BREAKNULL(item_temp_ptr, ret, "SP_Store_UpdateEmail:Malloc item_ptr failed!");
                EMA_UTILS_MEMSET(item_temp_ptr, 0, item_len);
                addr_temp_ptr = EMA_UTILS_WSTRTOSTR(PNULL, data_cache_ptr->bcc_addrs[i]);
                EMA_UTILS_BREAKNULL(addr_temp_ptr, ret, "SP_Store_UpdateEmail:Malloc item_ptr failed!");
                
                if (1 == num || (i == (num - 1)))
                {
                    EMA_UTILS_SPRINTF(item_temp_ptr, "\" \"<%s>", addr_temp_ptr);
                }
                else
                {
                    EMA_UTILS_SPRINTF(item_temp_ptr, "\" \"<%s>,", addr_temp_ptr);
                }
                
                item_ptr = EMA_UTILS_STRTOWSTR(PNULL, item_temp_ptr);
                EMA_UTILS_BREAKNULL(item_ptr, ret, "SP_Store_UpdateEmail:Malloc item_ptr failed!");
                MMIAPICOM_Wstrncat(opt_val_ptr, item_ptr, len);
                EMA_UTILS_FREE(store_mgr_ptr->mem_mgr_ptr,item_ptr);
                EMA_UTILS_FREE(store_mgr_ptr->mem_mgr_ptr,item_temp_ptr);
                EMA_UTILS_FREE(store_mgr_ptr->mem_mgr_ptr,addr_temp_ptr);
            }
            if (EMAIL_SUCCESS != ret)
            {
                break;
            }
            
            str_temp_ptr = EMA_UTILS_WSTRTOUTF8(PNULL, opt_val_ptr);
            
            qee_mo_opt[0].pVal = (void*)str_temp_ptr;
            ret = HEMSMSG_UpdateOpt(qee_msg_ptr, 0, &qee_mo_opt[0]);
            EMA_UTILS_BREAKIF(ret, "SP_Store_UpdateBcc:HEMSMSG_UpdateOpt failed!");
            
            EMA_UTILS_FREEIF(PNULL, str_temp_ptr);
            
        } 
    } while(0);
    
    EMA_UTILS_FREEIF(store_mgr_ptr->mem_mgr_ptr,item_ptr);
    EMA_UTILS_FREEIF(store_mgr_ptr->mem_mgr_ptr,opt_val_ptr);
    
    return ret;
}

/**************************************************************************************/
// Description : email sp store, update subject.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL int32 SP_Store_UpdateSubject(
                                   HEMSMsg           qee_msg_ptr,
                                   HEMSStore         qee_store_ptr,
                                   EMAIL_MSGDATACACHE_T   *data_cache_ptr
                                   )
{
    EMSMsgOpt qee_mo_opt[2] = {0};
    int32     ret = EMAIL_ERROR;
    
    if (PNULL == qee_msg_ptr || PNULL == qee_store_ptr || PNULL == data_cache_ptr)
    {
        EMA_UTILS_LOG_ERROR(("~~~~~~~~~SP_Store_UpdateCc: Bad paramter!~~~~~~~~~~~~~~~~~~"));
        return EMAIL_BADPARAM;
    }
    
    do 
    {
        char *str_temp_ptr = PNULL;
        
        str_temp_ptr = EMA_UTILS_WSTRTOUTF8(PNULL, data_cache_ptr->subject_ptr);
        
        qee_mo_opt[0].nId = EMS_MSGOPT_SUBJECT;
        qee_mo_opt[0].pVal = (void*)str_temp_ptr; 
        
        ret = HEMSMSG_UpdateOpt(qee_msg_ptr, 0, &qee_mo_opt[0]);
        EMA_UTILS_BREAKIF(ret, "SP_Store_UpdateSubject: HEMSMSG_UpdateOpt failed!");
        
        EMA_UTILS_FREEIF(PNULL, str_temp_ptr);
                
    } while(0);
    
    return  ret;
}

/**************************************************************************************/
// Description : email sp store, update email state.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL int32 SP_Store_UpdateState(
                                 HEMSMsg           qee_msg_ptr,
                                 HEMSStore         qee_store_ptr,
                                 EMAIL_MSGDATACACHE_T   *data_cache_ptr
                                 )
{
    EMSMsgOpt qee_mo_opt[2] = {0};
    int32     ret = EMAIL_ERROR;
    
    if (PNULL == qee_msg_ptr || PNULL == qee_store_ptr || PNULL == data_cache_ptr)
    {
        EMA_UTILS_LOG_ERROR(("~~~~~~~~~SP_Store_UpdateCc: Bad paramter!~~~~~~~~~~~~~~~~~~"));
        return EMAIL_BADPARAM;
    }
    
    do 
    {
        qee_mo_opt[0].nId = EMS_MSGOPT_STATUS;
        qee_mo_opt[0].pVal = (void*)MMIEMAIL_SP_Store_ConventToEmeStatus(data_cache_ptr->mail_status); 
        
        ret = HEMSMSG_UpdateOpt(qee_msg_ptr, 0, &qee_mo_opt[0]);
        EMA_UTILS_BREAKIF(ret, "SP_Store_UpdateState: HEMSMSG_UpdateOpt failed!");
        
    } while(0);
    return  ret;
}

/**************************************************************************************/
// Description : email sp store, update attachment.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL int32 SP_Store_UpdateAttachment(
                                      HEMSMsg           qee_msg_ptr,
                                      HEMSStore         qee_store_ptr,
                                      EMAIL_MSGDATACACHE_T   *data_cache_ptr
                                      )
{
    int32          ret = EMAIL_SUCCESS;
    uint32         num = 0;
    uint32         i = 0;
    EMSMsgOpt      qee_mpo_opt[2] = {0};
    HEMSMsgPart    *qee_msg_part_ptr = PNULL;
    uint32         content_type = 0;
    
    if (PNULL == qee_msg_ptr|| PNULL == qee_store_ptr || PNULL == data_cache_ptr)
    {
        EMA_UTILS_LOG_ERROR(("~~~~~~~~~SP_Store_UpdateCc: Bad paramter!~~~~~~~~~~~~~~~~~~"));
        return EMAIL_BADPARAM;
    }
    
    do 
    {
        /*remove all attachment part*/
        content_type = EMSPART_TYPE_ATTACHMENT;
        while (EMAIL_SUCCESS == ret)
        {
            ret = HEMSMSG_RemoveMessagePart(qee_msg_ptr, EMS_MSGPART_ID_PART_TYPE, &content_type);
        }
        
        num = data_cache_ptr->attach_num;
        
        if (num > 0)
        {            
            /* Add attachment URI */
            for (i = 0; i < num; i++)
            {
                char *str_temp_ptr = PNULL;
                
                qee_msg_part_ptr = HEMSMSGPART_New(PNULL);
                EMA_UTILS_BREAKNULL(qee_msg_part_ptr, ret, "create msg part failed");
                
                /* Add attachment name */
                str_temp_ptr = EMA_UTILS_WSTRTOUTF8(PNULL, data_cache_ptr->attach_name[i]);
                qee_mpo_opt[0].nId = EMS_PARTOPT_ATTACHNAME;
                qee_mpo_opt[0].pVal = (void*)str_temp_ptr;
                ret = HEMSMSGPART_AddOpt(qee_msg_part_ptr, &qee_mpo_opt[0]);
                EMA_UTILS_BREAKIF(ret, "SP_Store_UpdateAttachment failed!");
                EMA_UTILS_FREEIF(PNULL, str_temp_ptr);
                
                /* Add attachment content */
                str_temp_ptr = EMA_UTILS_WSTRTOUTF8(PNULL, data_cache_ptr->attach_uri[i]);
                qee_mpo_opt[0].nId = EMS_PARTOPT_DATABUF;
                qee_mpo_opt[0].pVal = (void*)str_temp_ptr;
                ret = HEMSMSGPART_AddOpt(qee_msg_part_ptr, &qee_mpo_opt[0]);  
                EMA_UTILS_BREAKIF(ret, "SP_Store_UpdateAttachment failed!");
                EMA_UTILS_FREEIF(PNULL, str_temp_ptr);
                
                /* Add content type */
                qee_mpo_opt[0].nId = EMS_PARTOPT_PARTTYPE;
                qee_mpo_opt[0].pVal = (void*)EMSPART_TYPE_ATTACHMENT;
                ret = HEMSMSGPART_AddOpt(qee_msg_part_ptr, &qee_mpo_opt[0]);
                EMA_UTILS_BREAKIF(ret, "SP_Store_UpdateAttachment failed!");
                
                /* Add source type */
                qee_mpo_opt[0].nId = EMS_PARTOPT_SRCTYPE;
                qee_mpo_opt[0].pVal = (void*)EMSPART_SRCTYPE_FILE;
                ret = HEMSMSGPART_AddOpt(qee_msg_part_ptr, &qee_mpo_opt[0]);  
                EMA_UTILS_BREAKIF(ret, "SP_Store_UpdateAttachment failed!");
                
                ret = HEMSMSG_AddMessagePart(qee_msg_ptr, qee_msg_part_ptr,1);
                EMA_UTILS_BREAKIF(ret, "SP_Store_UpdateAttachment,add message part failed!");
                
                HEMSMSGPART_Delete(qee_msg_part_ptr);
                qee_msg_part_ptr = PNULL;
            }
        }
                
    } while(0);
    
    if(PNULL != qee_msg_part_ptr)
    {
        HEMSMSGPART_Delete(qee_msg_part_ptr);
    }
    
    return ret;
}

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
                                           )
{
    EMAIL_STOREMGR_T  *store_mgr_ptr = (EMAIL_STOREMGR_T*)store_handle_ptr;
    int32             ret = EMAIL_ERROR;
    HEMSStore         qee_store_ptr = PNULL;
    HEMSMsg           qee_msg_ptr = PNULL;
    BOOLEAN           bHeadOnly = TRUE;
    
    if (PNULL == store_mgr_ptr || PNULL == data_cache_ptr || PNULL == store_mgr_ptr->qee_store_ptr)
    {
        EMA_UTILS_LOG_ERROR(("~~~~~~~~~MMIEMAIL_SP_Store_UpdateMsg: Bad paramter!~~~~~~~~~~~~~~~"));
        return EMAIL_BADPARAM;
    }
    do 
    {
        qee_store_ptr = store_mgr_ptr->qee_store_ptr;
        
        if (EMAILA_MSGMASK_FULL == mask)/* Update a whole message*/
        { 
            ret = SP_Store_SaveOrUpdateEmail(store_mgr_ptr, data_cache_ptr, FALSE);
            EMA_UTILS_BREAKIF(ret, "MMIEMAIL_SP_Store_UpdateEmail: SP_Store_SaveOrUpdateEmail failed!");
        }
        else
        {
            ret = HEMSSTORE_GetMsg(qee_store_ptr, data_cache_ptr->email_id, &qee_msg_ptr);
            EMA_UTILS_BREAKIF(ret, "get msg failed");
            
            if (EMAILA_MSGMASK_SIZE == (mask & EMAILA_MSGMASK_SIZE))/* Update email content*/
            {
                ret = SP_Store_UpdateSize(qee_msg_ptr, qee_store_ptr, data_cache_ptr);
                EMA_UTILS_BREAKIF(ret, "MMIEMAIL_SP_Store_UpdateEmail SP_Store_UpdateSize failed!");
                bHeadOnly = FALSE;
            }
            if (EMAILA_MSGMASK_TOADDR == (mask & EMAILA_MSGMASK_TOADDR))/* Update 'To' field*/
            {
                ret = SP_Store_UpdateTo(store_handle_ptr,qee_msg_ptr, qee_store_ptr, data_cache_ptr);
                EMA_UTILS_BREAKIF(ret, "MMIEMAIL_SP_Store_UpdateEmail SP_Store_UpdateTo failed!");
            }
            if (EMAILA_MSGMASK_CCADDR == (mask & EMAILA_MSGMASK_CCADDR))/* Update 'CC' field*/
            {
                ret = SP_Store_UpdateCc(store_handle_ptr,qee_msg_ptr, qee_store_ptr, data_cache_ptr);
                EMA_UTILS_BREAKIF(ret, "MMIEMAIL_SP_Store_UpdateEmail SP_Store_UpdateCc failed!");
            }
            if (EMAILA_MSGMASK_BCCADDR == (mask & EMAILA_MSGMASK_BCCADDR))/* Update 'BCc' field*/
            {
               ret = SP_Store_UpdateBcc(store_handle_ptr,qee_msg_ptr, qee_store_ptr, data_cache_ptr);
               EMA_UTILS_BREAKIF(ret, "MMIEMAIL_SP_Store_UpdateEmail SP_Store_UpdateBcc failed!");
            }
            if (EMAILA_MSGMASK_SUBJ == (mask & EMAILA_MSGMASK_SUBJ))/* Update 'Subject' field*/
            {
               ret = SP_Store_UpdateSubject(qee_msg_ptr, qee_store_ptr, data_cache_ptr);
               EMA_UTILS_BREAKIF(ret, "MMIEMAIL_SP_Store_UpdateEmail SP_Store_UpdateSubject failed!");
            }
            if (EMAILA_MSGMASK_ATTACHMENT == (mask & EMAILA_MSGMASK_ATTACHMENT))/* Update 'attachment' filed*/
            {
               ret = SP_Store_UpdateAttachment(qee_msg_ptr, qee_store_ptr, data_cache_ptr);
               EMA_UTILS_BREAKIF(ret, "MMIEMAIL_SP_Store_UpdateEmail SP_Store_UpdateAttachment failed!");
               bHeadOnly = FALSE;
            }
            if (EMAILA_MSGMASK_STATE == (mask & EMAILA_MSGMASK_STATE))/* TBD */
            {
               ret = SP_Store_UpdateState(qee_msg_ptr, qee_store_ptr, data_cache_ptr);
               EMA_UTILS_BREAKIF(ret, "MMIEMAIL_SP_Store_UpdateEmail SP_Store_UpdateAttachment failed!");
            }
            if (EMAILA_MSGMASK_FROMADDR == (mask & EMAILA_MSGMASK_FROMADDR)) /*Update 'From' field*/
            {
               ret = SP_Store_UpdateFrom(store_handle_ptr,qee_msg_ptr, qee_store_ptr, data_cache_ptr);
               EMA_UTILS_BREAKIF(ret, "MMIEMAIL_SP_Store_UpdateEmail SP_Store_UpdateFrom failed!");
            }
            if (EMAILA_MSGMASK_FOLDER == (mask & EMAILA_MSGMASK_FOLDER))/* TBD */
            {
               
            }

            ret = SP_Store_UpdateSubmit(qee_msg_ptr, qee_store_ptr, bHeadOnly);
            EMA_UTILS_BREAKIF(ret, "SP_Store_UpdateSubmite submit failed!!!");
            
        }
    } while(0);
    
    if(PNULL != qee_msg_ptr)
    {
        HEMSMSG_Delete(qee_msg_ptr);
        qee_msg_ptr = PNULL;
    }
    
    return ret;
}
 
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
                                            )
{
    EMAIL_STOREMGR_T         *store_mgr_ptr = (EMAIL_STOREMGR_T*)store_handle_ptr;
    int32            ret = EMAIL_ERROR;
    HEMSMsg          qee_msg_ptr = PNULL;
    HConfigMgr       cfg_mgr_ptr = PNULL;
    
    // check input param
    if (PNULL == store_mgr_ptr || PNULL == part_num_ptr || PNULL == store_mgr_ptr->parent_handle_ptr || PNULL == store_mgr_ptr->qee_store_ptr)
    {
        EMA_UTILS_LOG_ERROR(("~~~~~~~~~MMIEMAIL_SP_Store_GetPartCount: Bad paramter!~~~~~~~~~~~~~~~"));
        return EMAIL_BADPARAM;
    }
    
    do 
    {    
        if (EMA_SP_ACTIVE_ACCOUNT == act_id)
        {
            cfg_mgr_ptr = MMIEMAIL_SP_GetCfgHandle(store_mgr_ptr->parent_handle_ptr);
            if (PNULL == cfg_mgr_ptr)
            {
                EMA_UTILS_LOG_ERROR(("~~~~~~~~~MMIEMAIL_SP_Store_GetPartCount: get cfgmgr failed!~~~~~~~~~~~~~"));
                return EMAIL_ERROR;
            }
            ret = MMIEMAIL_SP_CFG_GetActiveAct(cfg_mgr_ptr,&act_id);
            if (EMAIL_SUCCESS != ret)
            {
                EMA_UTILS_LOG_ERROR(("MMIEMAIL_SP_Store_GetPartCount: MMIEMAIL_SP_CFG_GetActiveAct failed!~"));
                return EMAIL_ERROR;
            }
        }
        
        ret = HEMSSTORE_GetMsg(store_mgr_ptr->qee_store_ptr, email_id, &qee_msg_ptr);
        EMA_UTILS_BREAKIF(ret, "get msg failed");
        
        *part_num_ptr = HEMSMSG_GetPartCount(qee_msg_ptr);
        
    } while(0);
    
    if(PNULL != qee_msg_ptr)
    {
        HEMSMSG_Delete(qee_msg_ptr);
    }
    
    return ret;
}
 
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
                                            )
{
    EMAIL_STOREMGR_T *store_mgr_ptr   = (EMAIL_STOREMGR_T*)store_handle_ptr;
    HEMSStore        qee_store_ptr    = PNULL;
    HEMSMsg          qee_msg_ptr      = PNULL;
    HEMSMsgPart      qee_msg_part_ptr = PNULL;
    int32            ret              = EMAIL_ERROR;
    EMSMsgOpt        qee_mo_opt       = {0};
    EMSMsgOpt        qee_mpo_opt      = {0};
    EMSMsgOpt        qee_mppo_opt     = {0};
    uint32           len              = 0;
    uint32           part_num         = 0;
    uint32           iIndex           = 0;
    uint32           j                = 0;
    HConfigMgr       cfg_mgr_ptr      = PNULL;
    
    // check input param
    if (PNULL == store_mgr_ptr || PNULL == data_cache_ptr || PNULL == store_mgr_ptr->parent_handle_ptr || PNULL == store_mgr_ptr->qee_store_ptr)
    {
        EMA_UTILS_LOG_ERROR(("~~~~~~~~~MMIEMAIL_SP_Store_GetEmailData: Bad paramter!~~~~~~~~~~~~~~~"));
        return EMAIL_BADPARAM;
    }
    
    EMA_UTILS_LOG_INFO(("MMIEMAIL_SP_Store_GetEmailData, Begin"));
    do 
    {
        if (EMA_SP_ACTIVE_ACCOUNT == act_id)
        {
            cfg_mgr_ptr = MMIEMAIL_SP_GetCfgHandle(store_mgr_ptr->parent_handle_ptr);
            if (PNULL == cfg_mgr_ptr)
            {
                EMA_UTILS_LOG_ERROR(("~~~~~~~~~MMIEMAIL_SP_Store_GetEmailData: get cfgmgr failed!~~~~~~~~~~~~~"));
                return EMAIL_ERROR;
            }
            ret = MMIEMAIL_SP_CFG_GetActiveAct(cfg_mgr_ptr, &act_id);
            if (EMAIL_SUCCESS != ret)
            {
                EMA_UTILS_LOG_ERROR(("MMIEMAIL_SP_Store_GetEmailData: MMIEMAIL_SP_CFG_GetActiveAct failed!~"));
                return EMAIL_ERROR;
            }
        }
        
        qee_store_ptr = store_mgr_ptr->qee_store_ptr;
        
        ret = HEMSSTORE_GetMsg(qee_store_ptr, email_id, &qee_msg_ptr);
        EMA_UTILS_BREAKIF(ret, "MMIEMAIL_SP_Store_GetEmailData: IQEESTORE_GetMsgs failed!");
        
        /*--------------Set pDatacache---------------------------------*/
        /* Clear DataCache */
        ret = SP_Store_ClearCache(store_handle_ptr, data_cache_ptr);
        EMA_UTILS_BREAKIF(ret, "MMIEMAIL_SP_Store_GetEmailData, SP_Store_ClearCache failed!");
        EMA_UTILS_TRACE("MMIEMAIL_SP_Store_GetEmailData, Clear DataCache");
        
        /*0412 set Priority*/
        if (EMAIL_SUCCESS == HEMSMSG_GetOpt(qee_msg_ptr, EMS_MSGOPT_PRIO, 0, &qee_mo_opt))
        {
            data_cache_ptr->uipriority = (uint32)qee_mo_opt.pVal;
        }
        
        /*0412 set Priority*/
        ret = HEMSMSG_GetOpt(qee_msg_ptr, EMS_MSGOPT_DISP_NOTIFY_TO, 0, &qee_mo_opt);
        if (EMAIL_SUCCESS == ret && PNULL != qee_mo_opt.pVal)
        {  
            data_cache_ptr->read_report_ptr = EMA_UTILS_UTF8TOWSTR(store_mgr_ptr->mem_mgr_ptr, qee_mo_opt.pVal);
        }
        else
        {
            ret = EMAIL_SUCCESS;
        }
        
        /*0405 set state*/
        ret = HEMSMSG_GetOpt(qee_msg_ptr, EMS_MSGOPT_STATUS, 0, &qee_mo_opt);
        //      EMA_UTILS_BREAKIF(ret, "SP_Store_GetEmailData,HEMSMSG_GetOpt failed!");
        data_cache_ptr->mail_status = SP_Store_ConventToEmaStatus((uint32)qee_mo_opt.pVal);
        
        EMA_UTILS_MEMSET(&qee_mo_opt, 0, sizeof(EMSMsgOpt));
        ret = HEMSMSG_GetOpt(qee_msg_ptr, EMS_MSGOPT_BEDOWN, 0, &qee_mo_opt);
        data_cache_ptr->be_down = (uint32)qee_mo_opt.pVal;
        
        EMA_UTILS_MEMSET(&qee_mo_opt, 0, sizeof(EMSMsgOpt));
        ret = HEMSMSG_GetOpt(qee_msg_ptr, EMS_MSGOPT_MSGID, 0, &qee_mo_opt);
        EMA_UTILS_BREAKIF(ret, "MMIEMAIL_SP_Store_GetEmailData,HEMSMSG_GetOpt failed!");
        data_cache_ptr->email_id = (uint32)qee_mo_opt.pVal;
        
        EMA_UTILS_MEMSET(&qee_mo_opt, 0, sizeof(EMSMsgOpt));
        ret = HEMSMSG_GetOpt(qee_msg_ptr, EMS_MSGOPT_ACCOUNTID, 0, &qee_mo_opt);
        EMA_UTILS_BREAKIF(ret, "MMIEMAIL_SP_Store_GetEmailData,HEMSMSG_GetOpt failed!");
        data_cache_ptr->act_id = (uint32)qee_mo_opt.pVal;
        
        EMA_UTILS_MEMSET(&qee_mo_opt, 0, sizeof(EMSMsgOpt));
        ret = HEMSMSG_GetOpt(qee_msg_ptr, EMS_MSGOPT_BOXID, 0, &qee_mo_opt);
        EMA_UTILS_BREAKIF(ret, "MMIEMAIL_SP_Store_GetEmailData,HEMSMSG_GetOpt failed!");
        data_cache_ptr->folder_id = (uint32)qee_mo_opt.pVal;
        
        EMA_UTILS_TRACE("MMIEMAIL_SP_Store_GetEmailData,Set From");
        /* Set From */
         EMA_UTILS_MEMSET(&qee_mo_opt, 0, sizeof(EMSMsgOpt));
         ret = HEMSMSG_GetOpt(qee_msg_ptr, EMS_MSGOPT_FROM, 0, &qee_mo_opt);
         if (EMAIL_SUCCESS == ret)
         {
            //wchar *str_ptr = PNULL;
            wchar *str_temp_ptr = PNULL;
            
            str_temp_ptr = EMA_UTILS_UTF8TOWSTR(PNULL, qee_mo_opt.pVal);
            
            /*from address length limited.*/
            len = EMA_UTILS_STRLEN((char*)qee_mo_opt.pVal);
            len = (len + 1) < (EMA_MAX_ADDR_LEN + 1) ? (len + 1) : (EMA_MAX_ADDR_LEN + 1);
            data_cache_ptr->email_size += len;
            EMA_UTILS_WSTRLCPY(data_cache_ptr->from_addrs, str_temp_ptr, len);
            EMA_UTILS_FREEIF(PNULL, str_temp_ptr);
            
            //EMA_GetNameFromPhoneBookEX(/*piShell, */data_cache_ptr->awFromAddrURL, &str_ptr);
            //if (PNULL != str_ptr)
            //{
            //     len = EMA_UTILS_WSTRLEN(str_ptr);
            //     len = (len + 1) < (EMA_MAX_ADDR_LEN + 1) ? (len + 1) : (EMA_MAX_ADDR_LEN + 1);
            //    EMA_UTILS_WSTRLCPY(data_cache_ptr->from_addr_name, str_ptr, len );
            //    EMA_UTILS_FREE(PNULL,str_ptr);
            //}
         }
         else
         {
            ret = EMAIL_SUCCESS;
         }

        /*set Date*/
        EMA_UTILS_MEMSET(&qee_mo_opt,0,sizeof(EMSMsgOpt));
        ret = HEMSMSG_GetOpt(qee_msg_ptr, EMS_MSGOPT_DATE, 0, &qee_mo_opt);
        if (EMAIL_SUCCESS == ret)
        {
            data_cache_ptr->uidate = (uint32)qee_mo_opt.pVal;
        }
        else
        {
            ret = EMAIL_SUCCESS;
        }
        
        EMA_UTILS_TRACE("MMIEMAIL_SP_Store_GetEmailData,Set TO");
        /* Set To */
        EMA_UTILS_MEMSET(&qee_mo_opt,0,sizeof(EMSMsgOpt));
        ret = HEMSMSG_GetOpt(qee_msg_ptr, EMS_MSGOPT_TO, 0, &qee_mo_opt);
        if(EMAIL_SUCCESS == ret)
        {
            int i = 0;
            wchar  *addr_list_ptr = PNULL;
            wchar  *temp_ptr  = PNULL;
			wchar  *temp_start = PNULL;
			BOOLEAN skip_spil = FALSE;
            
            addr_list_ptr = EMA_UTILS_UTF8TOWSTR(store_mgr_ptr->mem_mgr_ptr, qee_mo_opt.pVal);
            temp_ptr = addr_list_ptr ;
			temp_start = temp_ptr;

			/*recipient number limited.*/
			while(PNULL != temp_ptr && i < EMA_MAX_RECPT_NUM && *temp_ptr != L'\0')
			{
				if(*temp_ptr == L'\"')
				{
					skip_spil = !skip_spil;
				}
				
				if((*temp_ptr == L',' || *temp_ptr == L';' || *(temp_ptr+1) == L'\0') && !skip_spil)
				{
					if(*temp_ptr == L',' || *temp_ptr == L';')
					{
						*temp_ptr = L'\0';
					}

					len = EMA_UTILS_WSTRLEN(temp_start) * 2 + 2;
					len = len < (EMA_MAX_ADDR_LEN + 1) * 2 ? len : (EMA_MAX_ADDR_LEN + 1) * 2;
                    EMA_UTILS_MEMCPY(data_cache_ptr->to_addrs[i],temp_start, len - 2);
                    *(data_cache_ptr->to_addrs[i++] + len / 2 - 1) = L'\0';
					
					temp_ptr++;
					temp_start = temp_ptr;
					continue;
				}

				temp_ptr++;
			}

            data_cache_ptr->to_addrs_num = i ;
            EMA_UTILS_FREEIF(store_mgr_ptr->mem_mgr_ptr,addr_list_ptr);
            
        }
        
        /* Set CC */
        EMA_UTILS_MEMSET(&qee_mo_opt,0,sizeof(EMSMsgOpt));
        ret = HEMSMSG_GetOpt(qee_msg_ptr, EMS_MSGOPT_CC, 0, &qee_mo_opt);
        if(EMAIL_SUCCESS == ret)
        {
            int i = 0;
            wchar  *addr_list_ptr = PNULL;
            wchar  *temp_ptr  = PNULL;
			wchar  *temp_start = PNULL;
			BOOLEAN skip_spil = FALSE;
            
            addr_list_ptr = EMA_UTILS_UTF8TOWSTR(store_mgr_ptr->mem_mgr_ptr,(char*)qee_mo_opt.pVal);
            temp_ptr = addr_list_ptr ;
			temp_start = temp_ptr;
			
            /*recipient number limited.*/
			while(PNULL != temp_ptr && i < EMA_MAX_RECPT_NUM && *temp_ptr != L'\0')
			{
				if(*temp_ptr == L'\"')
				{
					skip_spil = !skip_spil;
				}
				
				if((*temp_ptr == L',' || *temp_ptr == L';' || *(temp_ptr+1) == L'\0') && !skip_spil)
				{
					if(*temp_ptr == L',' || *temp_ptr == L';')
					{
						*temp_ptr = L'\0';
					}

					len = EMA_UTILS_WSTRLEN(temp_start) * 2 + 2;
					len = len < (EMA_MAX_ADDR_LEN + 1) * 2 ? len : (EMA_MAX_ADDR_LEN + 1) * 2;
                    EMA_UTILS_MEMCPY(data_cache_ptr->cc_addrs[i],temp_start, len - 2);
                    *(data_cache_ptr->cc_addrs[i++] + len / 2 - 1) = L'\0';
					
					temp_ptr++;
					temp_start = temp_ptr;
					continue;
				}

				temp_ptr++;
			}
			
            data_cache_ptr->cc_addrs_num = i ;
            EMA_UTILS_FREEIF(store_mgr_ptr->mem_mgr_ptr,addr_list_ptr);
        }
        
        /* Set Bcc */
        EMA_UTILS_MEMSET(&qee_mo_opt,0,sizeof(EMSMsgOpt));
        ret = HEMSMSG_GetOpt(qee_msg_ptr, EMS_MSGOPT_BCC, 0, &qee_mo_opt);
        if(EMAIL_SUCCESS == ret)
        {
            int i = 0;
            wchar  *addr_list_ptr = PNULL;
            wchar  *temp_ptr  = PNULL;
            wchar  *temp_start = PNULL;
			BOOLEAN skip_spil = FALSE;

            addr_list_ptr = EMA_UTILS_UTF8TOWSTR(store_mgr_ptr->mem_mgr_ptr, qee_mo_opt.pVal);
            temp_ptr = addr_list_ptr ;
            temp_start = temp_ptr;

            /*recipient number limited.*/
			while(PNULL != temp_ptr && i < EMA_MAX_RECPT_NUM && *temp_ptr != L'\0')
			{
				if(*temp_ptr == L'\"')
				{
					skip_spil = !skip_spil;
				}
				
				if((*temp_ptr == L',' || *temp_ptr == L';' || *(temp_ptr+1) == L'\0') && !skip_spil)
				{
					if(*temp_ptr == L',' || *temp_ptr == L';')
					{
						*temp_ptr = L'\0';
					}
					
					len = EMA_UTILS_WSTRLEN(temp_start) * 2 + 2;
					len = len < (EMA_MAX_ADDR_LEN + 1) * 2 ? len : (EMA_MAX_ADDR_LEN + 1) * 2;
                    EMA_UTILS_MEMCPY(data_cache_ptr->bcc_addrs[i],temp_start, len - 2);
                    *(data_cache_ptr->bcc_addrs[i++] + len / 2 - 1) = L'\0';
					
					temp_ptr++;
					temp_start = temp_ptr;
					continue;
				}
				
				temp_ptr++;
			}

            data_cache_ptr->bcc_addrs_num = i;
            EMA_UTILS_FREEIF(store_mgr_ptr->mem_mgr_ptr,addr_list_ptr);
        }
        EMA_UTILS_TRACE("MMIEMAIL_SP_Store_GetEmailData,Set Subject");
        
        /* Set Subject*/
        EMA_UTILS_MEMSET(&qee_mo_opt,0,sizeof(EMSMsgOpt));
        ret = HEMSMSG_GetOpt(qee_msg_ptr, EMS_MSGOPT_SUBJECT, 0, &qee_mo_opt);
        if (EMAIL_SUCCESS == ret)
        {
            wchar *str_temp_ptr = EMA_UTILS_UTF8TOWSTR(PNULL, qee_mo_opt.pVal);
            
            /*subject length limited.*/
            len = (EMA_UTILS_WSTRLEN(str_temp_ptr) + 1) * sizeof(wchar);
            // len = (len + 1) < (EMA_MAX_SUBJ_LEN + 1) ? (len + 1) : (EMA_MAX_SUBJ_LEN + 1);
            data_cache_ptr->email_size += len;
            
            data_cache_ptr->subject_ptr = EMA_UTILS_MALLOC(PNULL, len);
            EMA_UTILS_MEMSET(data_cache_ptr->subject_ptr,0x00,len);
            
            len = len < (EMA_MAX_SUBJ_LEN + 1) * 2 ? len: (EMA_MAX_SUBJ_LEN + 1) * 2; 
            EMA_UTILS_MEMCPY(data_cache_ptr->subject_ptr, (wchar *)str_temp_ptr, len - 2);
            
            EMA_UTILS_FREEIF(PNULL, str_temp_ptr);
        }
        else
        {
            ret = EMAIL_SUCCESS;
        }
        EMA_UTILS_TRACE("MMIEMAIL_SP_Store_GetEmailData,Set Boby");
        
        part_num = HEMSMSG_GetPartCount(qee_msg_ptr);
        
        for (iIndex = 0; iIndex < part_num; iIndex++)
        {
            ret = HEMSMSG_GetMessagePart(qee_msg_ptr, EMS_MSGPART_ID_POSITION, (void*)iIndex, &qee_msg_part_ptr);
            EMA_UTILS_BREAKIF(ret, "MMIEMAIL_SP_Store_GetEmailData,IQEEMULTIPARTMSG_GetMessagePart failed!");
            
            ret = HEMSMSGPART_GetOpt(qee_msg_part_ptr,EMS_PARTOPT_PARTTYPE, 0, &qee_mpo_opt);
            EMA_UTILS_BREAKIF(ret, "MMIEMAIL_SP_Store_GetEmailData,HEMSMSG_GetOpt failed!");
            
            ret = HEMSMSGPART_GetOpt(qee_msg_part_ptr,EMS_PARTOPT_SRCTYPE, 0, &qee_mppo_opt);
            EMA_UTILS_BREAKIF(ret, "MMIEMAIL_SP_Store_GetEmailData,HEMSMSG_GetOpt failed!");
            
            /* Set attachment */
            if ((EMSPART_SRCTYPE_FILE == (uint32)qee_mppo_opt.pVal) 
                &&(EMSPART_TYPE_ATTACHMENT == (uint32)qee_mpo_opt.pVal)
                && data_cache_ptr->attach_num < EMA_MAX_ATTACH_NUM)
            {
                SFS_HANDLE file = PNULL;
                uint32     size = 0;
                char	   *szie_temp_ptr = PNULL;
                wchar      *str_temp_ptr = PNULL;
                uint32     file_size;
                
                ret = HEMSMSGPART_GetOpt(qee_msg_part_ptr,EMS_PARTOPT_ATTACHNAME, 0, &qee_mpo_opt);
                EMA_UTILS_BREAKIF(ret, "MMIEMAIL_SP_Store_GetEmailData,HEMSMSG_GetOpt failed!");
                
                str_temp_ptr = EMA_UTILS_UTF8TOWSTR(PNULL, qee_mpo_opt.pVal);
                
                len = EMA_UTILS_WSTRLEN(str_temp_ptr) * 2 + 2;
                
                if (len > EMA_MAX_ATTACHNAME_LEN)
                {
                    EMA_UTILS_MEMCPY(data_cache_ptr->attach_name[j], str_temp_ptr, EMA_MAX_ATTACHNAME_LEN);
                }
                else
                {
                    EMA_UTILS_MEMCPY(data_cache_ptr->attach_name[j], str_temp_ptr,len - 2);
                }
                EMA_UTILS_FREEIF(PNULL, str_temp_ptr);
                
                ret = HEMSMSGPART_GetOpt(qee_msg_part_ptr,EMS_PARTOPT_DATABUF, 0, &qee_mpo_opt);
                EMA_UTILS_BREAKIF(ret, "MMIEMAIL_SP_Store_GetEmailData,HEMSMSG_GetOpt failed!");
                
                str_temp_ptr = EMA_UTILS_UTF8TOWSTR(PNULL, qee_mpo_opt.pVal);
                
                len = EMA_UTILS_WSTRLEN(str_temp_ptr) * 2 + 2;
                EMA_UTILS_MEMCPY(data_cache_ptr->attach_uri[j], str_temp_ptr,len - 2);
                //EMA_UTILS_FREEIF(PNULL, str_temp_ptr);
                
                /*Attachment size*/
                file = SFS_CreateFile(str_temp_ptr, SFS_MODE_OPEN_EXISTING | SFS_MODE_READ, PNULL, PNULL);
                EMA_UTILS_BREAKNULL(file,ret,"MMIEMAIL_SP_Store_GetEmailData,open text part file failed");
                EMA_UTILS_FREEIF(PNULL, str_temp_ptr);
                
                //ret = IFILE_GetInfo(file,&FInfo);
                ret = SFS_GetFileSize(file, &file_size);
                EMA_UTILS_BREAKIF(ret,"MMIEMAIL_SP_Store_GetEmailData,get fileinfo failed");
                
                size = file_size;
                szie_temp_ptr = EMA_UTILS_MALLOC(store_mgr_ptr->mem_mgr_ptr, 80);
                EMA_UTILS_BREAKNULL(szie_temp_ptr, ret, "MMIEMAIL_SP_Store_GetEmailData, malloc pwszSize failed!");
                EMA_UTILS_MEMSET(szie_temp_ptr, 0, 80);
                EMA_UTILS_SPRINTF(szie_temp_ptr, "%dB", size);
                
                data_cache_ptr->email_size +=size;
                // 				uiLen = WSTRLEN(pwszSize);
                // 				MEMSET(pwszSize, 0, 80);
                // 				switch(uiLen)
                // 				{
                // 				case 1:
                // 					EMA_UTILS_WSPRINTF(pwszSize, 80, L"0.00%d", size);
                // 					break;
                // 				case 2:
                // 					EMA_UTILS_WSPRINTF(pwszSize, 80, L"0.0%d", size);
                // 					break;
                // 				case 3:
                // 					EMA_UTILS_WSPRINTF(pwszSize, 80, L"0.%d", size);
                // 				    break;
                // 				default:
                // 				    break;
                // 				}
                // 
                // 				if (uiLen > 3)
                // 				{
                // 					EMA_UTILS_WSPRINTF(pwszSize, 80, L"%d.%d", size / 1024, size % 1024);
                // 				}
                
                data_cache_ptr->attach_size_ptr[j] = EMA_UTILS_STRTOWSTR(store_mgr_ptr->mem_mgr_ptr, szie_temp_ptr);
                
                EMA_UTILS_FREE(store_mgr_ptr->mem_mgr_ptr, szie_temp_ptr);
                SFS_CloseFile(file);
                
                //             ret = HEMSMSGPART_GetOpt(qee_msg_part_ptr,
                //                                         QEE_MSGPART_BUFFSZ,
                //                                         &qee_mpo_opt);
                //             data_cache_ptr->attach_size_ptr[j] = EMA_UTILS_MALLOC(store_mgr_ptr->mem_mgr_ptr, 80);
                //             EMA_UTILS_BREAKNULL(data_cache_ptr->attach_size_ptr[j], ret, "SP_Store_GetEmailData, malloc data_cache_ptr->pwszAttachSize[j] failed!");
                //             EMA_UTILS_WSPRINTF(data_cache_ptr->attach_size_ptr[j], 80, L"%d", (uint32)qee_mpo_opt.pVal);
                
                data_cache_ptr->attach_num = j + 1;
                j++;
                
            }
            else if (EMSPART_TYPE_TEXT_PLAIN == (uint32)qee_mpo_opt.pVal)
            {
                wchar     *str_temp_ptr = PNULL;
                
                ret = HEMSMSGPART_GetOpt(qee_msg_part_ptr,EMS_PARTOPT_DATABUF, 0, &qee_mpo_opt);
                if(EMAIL_SUCCESS == ret)
                {
                    str_temp_ptr = EMA_UTILS_UTF8TOWSTR(PNULL, qee_mpo_opt.pVal);
                    
                    if (EMSPART_SRCTYPE_FILE == (uint32)qee_mppo_opt.pVal)
                    {
                        SFS_HANDLE file = PNULL; 
                        char* pcTmp = PNULL;
                        uint32 size = 0;
                        uint32 file_size = 0;
                        /* Set content */
                        file = SFS_CreateFile(str_temp_ptr, SFS_MODE_OPEN_EXISTING | SFS_MODE_READ, PNULL, PNULL);
                        EMA_UTILS_BREAKNULL(file,ret,"MMIEMAIL_SP_Store_GetEmailData,open text part file failed");
                        
                        SFS_GetFileSize(file, &file_size);
                        //EMA_UTILS_BREAKIF(ret,"MMIEMAIL_SP_Store_GetEmailData,get fileinfo failed");
                        
                        pcTmp = (char*)EMA_UTILS_MALLOC(store_mgr_ptr->mem_mgr_ptr,file_size +1);
                        EMA_UTILS_MEMSET(pcTmp,0,file_size + 1);
                        
                        SFS_ReadFile(file, pcTmp, file_size, &size, 0);
                        
                        if (size != file_size)
                        {
                            ret = EMAIL_ERROR;
                        }
                        EMA_UTILS_BREAKIF(ret,"MMIEMAIL_SP_Store_GetEmailData,read file failed");
                        
                        data_cache_ptr->email_size +=  size;
                        EMA_UTILS_FREEIF(PNULL, data_cache_ptr->data_buff_ptr);
                        data_cache_ptr->data_buff_ptr = EMA_UTILS_UTF8TOWSTR(store_mgr_ptr->mem_mgr_ptr,pcTmp);
                        if(PNULL == data_cache_ptr->data_buff_ptr)
                        {
                            EMA_UTILS_LOG_ERROR(("MMIEMAIL_SP_Store_GetEmailData: Malloc data_cache_ptr->data_buff_ptr failed!"));
                            ret = EMAIL_ERROR;
                        }
                        SFS_CloseFile(file);
                        EMA_UTILS_FREEIF(store_mgr_ptr->mem_mgr_ptr,pcTmp);
                        EMA_UTILS_FREEIF(PNULL, str_temp_ptr);
                    }
                    else
                    {
                        len = EMA_UTILS_WSTRLEN(str_temp_ptr) * 2 + 2;
                        EMA_UTILS_FREEIF(PNULL, data_cache_ptr->data_buff_ptr);
                        
                        data_cache_ptr->data_buff_ptr = (wchar*)EMA_UTILS_MALLOC(store_mgr_ptr->mem_mgr_ptr,len);
                        EMA_UTILS_BREAKNULL(data_cache_ptr->data_buff_ptr, ret,"MMIEMAIL_SP_Store_GetEmailData: Malloc data_cache_ptr->data_buff_ptr failed!");
                        EMA_UTILS_MEMSET(data_cache_ptr->data_buff_ptr, 0, len);
                        EMA_UTILS_MEMCPY(data_cache_ptr->data_buff_ptr, str_temp_ptr, len - 2);
                        EMA_UTILS_FREEIF(PNULL, str_temp_ptr);
                        EMA_UTILS_TRACE("data_buff_ptr len is  %d", len);
                        EMA_UTILS_TRACE("data_buff_ptr is  %d", *data_cache_ptr->data_buff_ptr);
                        EMA_UTILS_TRACE("data_buff_ptr is  %d", *(data_cache_ptr->data_buff_ptr+1));
                        EMA_UTILS_TRACE("data_buff_ptr is  %d", *(data_cache_ptr->data_buff_ptr+2));
     
                        data_cache_ptr->email_size +=  len;
                    }
                }
                else
                {
                    ret = EMAIL_SUCCESS;
                    EMA_UTILS_LOG_INFO(("get part data failed and skip this part!!!"));
                }
            }
            
            HEMSMSGPART_Delete(qee_msg_part_ptr);
            qee_msg_part_ptr = PNULL;
       }
       
       
       EMA_UTILS_MEMSET(&qee_mo_opt,0,sizeof(EMSMsgOpt));
       HEMSMSG_GetOpt(qee_msg_ptr, EMS_MSGOPT_STORESIZE, 0, &qee_mo_opt);
       data_cache_ptr->email_size = (uint32)qee_mo_opt.pVal;
       
       /* Set the Date*/
       EMA_UTILS_MEMSET(&qee_mo_opt,0,sizeof(EMSMsgOpt));
       HEMSMSG_GetOpt(qee_msg_ptr, EMS_MSGOPT_DATE, 0, &qee_mo_opt);
       if (PNULL != qee_mo_opt.pVal)
       {
           data_cache_ptr->uidate = (uint32)qee_mo_opt.pVal;
       }
       else
       {
           data_cache_ptr->uidate = MMIAPICOM_GetCurTime();
       }
       
   }while (0);
   
   if(PNULL != qee_msg_part_ptr)
   {
       HEMSMSGPART_Delete(qee_msg_part_ptr);
   }
   
   if(PNULL != qee_msg_ptr)
   {
       HEMSMSG_Delete(qee_msg_ptr);
   }
   
   return ret;
}

/**************************************************************************************/
// Description : email sp store, get to string.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL BOOLEAN SP_Store_GetToStr(
                                EMAIL_DECODEADR_T  *cx_ptr, 
                                wchar     *ret_str_ptr, 
                                wchar     *list_str_ptr
                                )
{
    wchar   *left_bracket  = PNULL;
    wchar   *right_bracket = PNULL;
   // wchar   *now_pos = PNULL;
    
    // check input param
    if (PNULL == cx_ptr || PNULL == ret_str_ptr || PNULL == list_str_ptr)
    {
        EMA_UTILS_LOG_ERROR(("~~~~~~~~~SP_Store_GetToStr: Bad paramter!~~~~~~~~~~~~~~~"));
        return EMAIL_BADPARAM;
    }
    
    if (PNULL == cx_ptr->str_to_pos_ptr)
    {
        cx_ptr->str_to_pos_ptr = list_str_ptr;
    }
    
    if (PNULL != EMA_UTILS_WSTRCHR(cx_ptr->str_to_pos_ptr, L','))/* addr in list */
    {
        left_bracket  = EMA_UTILS_WSTRCHR(cx_ptr->str_to_pos_ptr, L'<');
        right_bracket = EMA_UTILS_WSTRCHR(cx_ptr->str_to_pos_ptr, L'>');
        
        if (PNULL != left_bracket && PNULL != right_bracket)
        {
            cx_ptr->str_to_pos_ptr = ++left_bracket;
        }
        
        while (cx_ptr->str_to_pos_ptr != right_bracket)
        {
            *(ret_str_ptr++) = *(cx_ptr->str_to_pos_ptr++);
        }
        *(ret_str_ptr) = L'\0';
        ++cx_ptr->str_to_pos_ptr; /* go to next To str. */
        return TRUE;
    }
    else if (L'\0' != *(cx_ptr->str_to_pos_ptr))/* has last addr in list */
    {
        left_bracket  = EMA_UTILS_WSTRCHR(cx_ptr->str_to_pos_ptr, L'<');
        right_bracket = EMA_UTILS_WSTRCHR(cx_ptr->str_to_pos_ptr, L'>');
        
        if (PNULL != left_bracket && PNULL != right_bracket)
        {
            cx_ptr->str_to_pos_ptr = ++left_bracket;
            while (cx_ptr->str_to_pos_ptr != right_bracket)
            {
                *(ret_str_ptr++) = *(cx_ptr->str_to_pos_ptr++);
            }
            *(ret_str_ptr) = L'\0';
        }
        /*by echo  more consider*/
        else if(PNULL == left_bracket && PNULL == right_bracket)
        {
            uint32 uiWstrLen = EMA_UTILS_WSTRLEN(cx_ptr->str_to_pos_ptr);
            EMA_UTILS_MEMCPY(ret_str_ptr,cx_ptr->str_to_pos_ptr, uiWstrLen);
        }
        cx_ptr->str_to_pos_ptr = list_str_ptr + EMA_UTILS_WSTRLEN(list_str_ptr); /* find end go to '\0' */
        return TRUE;
    }
    return FALSE;
}

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
                                           )
{
    EMAIL_STOREMGR_T    *store_mgr_ptr = (EMAIL_STOREMGR_T*)store_handle_ptr;
    int32       ret = EMAIL_ERROR;
    HEMSStore   qee_store_ptr = PNULL;
    HConfigMgr  cfg_mgr_ptr = PNULL;
    
    if (PNULL == store_mgr_ptr || PNULL == store_mgr_ptr->parent_handle_ptr || PNULL == store_mgr_ptr->qee_store_ptr)
    {
        EMA_UTILS_LOG_ERROR(("~~~~~~~~~MMIEMAIL_SP_Store_DeleteMsg: Bad paramter!~~~~~~~~~~~~~~~~~"));
        return EMAIL_BADPARAM;
    }
    
    qee_store_ptr = store_mgr_ptr->qee_store_ptr;
    
    if (EMA_SP_ACTIVE_ACCOUNT == act_id)
    {
        cfg_mgr_ptr = MMIEMAIL_SP_GetCfgHandle(store_mgr_ptr->parent_handle_ptr);
        if (PNULL == cfg_mgr_ptr)
        {
            EMA_UTILS_LOG_ERROR(("~~~~~~~~~MMIEMAIL_SP_Store_DeleteMsg: get cfgmgr failed!~~~~~~~~~~~~~"));
            return EMAIL_ERROR;
        }
        ret = MMIEMAIL_SP_CFG_GetActiveAct(cfg_mgr_ptr,&act_id);
        if (EMAIL_SUCCESS != ret)
        {
            EMA_UTILS_LOG_ERROR(("MMIEMAIL_SP_Store_DeleteEmail: MMIEMAIL_SP_CFG_GetActiveAct failed!~"));
            return EMAIL_ERROR;
        }
    }
    
    return HEMSSTORE_RemoveMsg(qee_store_ptr,email_id);
}

/**************************************************************************************/
// Description : email sp store, cancel delete all email
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC int32 MMIEMAIL_SP_Store_CancelDeleteAllEmail(HStoreMgr store_handle_ptr)
{
    EMAIL_STOREMGR_T    *store_mgr_ptr = (EMAIL_STOREMGR_T*)store_handle_ptr;
    HEMSStore   qee_store_ptr = PNULL;
    uint32      ret = EMAIL_SUCCESS;
    
    if (PNULL == store_mgr_ptr || PNULL == store_mgr_ptr->parent_handle_ptr || PNULL == store_mgr_ptr->qee_store_ptr)
    {
        EMA_UTILS_LOG_ERROR(("~~~~~~~~~MMIEMAIL_SP_Store_DeleteAllMsg: Bad paramter!~~~~~~~~~~~~~~~~~"));
        return EMAIL_BADPARAM;
    }
    
    qee_store_ptr = store_mgr_ptr->qee_store_ptr;
    
#ifndef MMI_DUALMODE_ENABLE
    HEMSSTORE_CancelRemoveMultiMsg(qee_store_ptr);
#endif
    
    return ret;
}

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
                                              )
{
    EMAIL_STOREMGR_T    *store_mgr_ptr = (EMAIL_STOREMGR_T*)store_handle_ptr;
    HEMSStore   qee_store_ptr = PNULL;
    uint32      ret = EMAIL_SUCCESS;
    
    if (PNULL == store_mgr_ptr || PNULL == store_mgr_ptr->parent_handle_ptr || PNULL == store_mgr_ptr->qee_store_ptr)
    {
        EMA_UTILS_LOG_ERROR(("~~~~~~~~~MMIEMAIL_SP_Store_DeleteAllMsg: Bad paramter!~~~~~~~~~~~~~~~~~"));
        return EMAIL_BADPARAM;
    }
    
    qee_store_ptr = store_mgr_ptr->qee_store_ptr;
    
    ret = HEMSSTORE_RemoveMultiMsg(qee_store_ptr, (unsigned int*)email_list_ptr, email_num);
    if (EMAIL_SUCCESS == ret)
    {
        g_email_is_engine_task_busy = TRUE;
        SCI_TRACE_LOW("mmiemail_sp_stroe.c set g_email_is_engine_task_busy TRUE, MMIEMAIL_SP_Store_DeleteAllEmail");
    }
    return ret;
}
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
                                                 )
{
    EMAIL_STOREMGR_T    *store_mgr_ptr = (EMAIL_STOREMGR_T*)store_handle_ptr;
    HEMSStore   qee_store_ptr = PNULL;
    HConfigMgr  cfg_mgr_ptr = PNULL;
    uint32      act_id = 0;
    uint32      ret = EMAIL_SUCCESS;
    
    if (PNULL == store_mgr_ptr || PNULL == store_mgr_ptr->parent_handle_ptr || PNULL == store_mgr_ptr->qee_store_ptr)
    {
        EMA_UTILS_LOG_ERROR(("~~~~~~~~~MMIEMAIL_SP_Store_MoveEmailtoFolder: Bad paramter!~~~~~~~~~~~~~"));
        return EMAIL_BADPARAM;
    }
    
    qee_store_ptr = store_mgr_ptr->qee_store_ptr;
    
    if (EMA_SP_ACTIVE_ACCOUNT == src_act_id || EMA_SP_ACTIVE_ACCOUNT == des_act_id)
    {
        cfg_mgr_ptr = MMIEMAIL_SP_GetCfgHandle(store_mgr_ptr->parent_handle_ptr);
        if (PNULL == cfg_mgr_ptr)
        {
            EMA_UTILS_LOG_ERROR(("~~~~~~~~~MMIEMAIL_SP_Store_MoveEmailtoFolder: get cfgmgr failed!~~~~~~~~~~~~~"));
            return EMAIL_ERROR;
        }
        
        ret = MMIEMAIL_SP_CFG_GetActiveAct(cfg_mgr_ptr,&act_id);
        if (EMAIL_SUCCESS != ret)
        {
            EMA_UTILS_LOG_ERROR(("MMIEMAIL_SP_Store_MoveEmailtoFolder: MMIEMAIL_SP_CFG_GetActiveAct failed!~"));
            return EMAIL_ERROR;
        }
        
        if (EMA_SP_ACTIVE_ACCOUNT == src_act_id)
        {
            src_act_id = act_id;
        }
        
        if (EMA_SP_ACTIVE_ACCOUNT == des_act_id)
        {
            des_act_id = act_id;
        }
    }
    
    return HEMSSTORE_MoveMsg(qee_store_ptr, src_act_id, src_box, des_box, email_id);
}

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
                                                 )
{
    EMAIL_STOREMGR_T    *store_mgr_ptr= (EMAIL_STOREMGR_T*)store_handle_ptr;
    HEMSMsg     qee_msg_ptr = PNULL;
    HConfigMgr  cfg_mgr_ptr = PNULL;
    uint32      ret = EMAIL_SUCCESS;
    EMSMsgOpt   stQEEMsgOpt = {0};
    
    if (PNULL == store_mgr_ptr || PNULL == store_mgr_ptr->parent_handle_ptr || PNULL == store_mgr_ptr->qee_store_ptr || 0 == act_id)
    {
        EMA_UTILS_LOG_ERROR(("~~~~~~~~~MMIEMAIL_SP_Store_CopyEmailtoFolder: Bad paramter!~~~~~~~~~~~~~"));
        return EMAIL_BADPARAM;
    }
    
    if (EMA_SP_ACTIVE_ACCOUNT == act_id)
    {
        cfg_mgr_ptr = MMIEMAIL_SP_GetCfgHandle(store_mgr_ptr->parent_handle_ptr);
        if (PNULL == cfg_mgr_ptr)
        {
            EMA_UTILS_LOG_ERROR(("~~~~~~~~~MMIEMAIL_SP_Store_CopyEmailtoFolder: get cfgmgr failed!~~~~~~~~~~~~~"));
            return EMAIL_ERROR;
        }
        ret = MMIEMAIL_SP_CFG_GetActiveAct(cfg_mgr_ptr,&act_id);
        if (EMAIL_SUCCESS != ret)
        {
            EMA_UTILS_LOG_ERROR(("MMIEMAIL_SP_Store_CopyEmailtoFolder: MMIEMAIL_SP_CFG_GetActiveAct failed!~"));
            return EMAIL_ERROR;
        }
    }
    
    do
    {
        ret = HEMSSTORE_GetMsg(store_mgr_ptr->qee_store_ptr, email_id, &qee_msg_ptr);
        EMA_UTILS_BREAKIF(ret, "MMIEMAIL_SP_Store_CopyEmailtoFolder: IQEESTORE_GetMsgs failed!");
        
        stQEEMsgOpt.nId = EMS_MSGOPT_BOXID;
        stQEEMsgOpt.pVal = (void*)des_box_id;
        ret = HEMSMSG_UpdateOpt(qee_msg_ptr,0,&stQEEMsgOpt);
        EMA_UTILS_BREAKIF(ret, "MMIEMAIL_SP_Store_CopyEmailtoFolder: IVECTORMODEL_GetAt failed!");
        
        ret = HEMSSTORE_StoreMsg(store_mgr_ptr->qee_store_ptr,qee_msg_ptr);
        EMA_UTILS_BREAKIF(ret, "MMIEMAIL_SP_Store_CopyEmailtoFolder: IVECTORMODEL_GetAt failed!");
        
    } while(0);
    
    if(PNULL != qee_msg_ptr)
    {
        HEMSMSG_Delete(qee_msg_ptr);
    }
    
    return ret;
}

/**************************************************************************************/
// Description : email sp store, clear cache.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL int32 SP_Store_ClearCache(
                                HStoreMgr store_handle_ptr,
                                EMAIL_MSGDATACACHE_T *data_cache_ptr
                                )
{
    uint32 i = 0;
    uint32 len = 0;
    EMAIL_STOREMGR_T  *store_mgr_ptr = (EMAIL_STOREMGR_T*)store_handle_ptr;
    
    //check input param
    if (PNULL == store_mgr_ptr || PNULL == data_cache_ptr)
    {
        EMA_UTILS_LOG_ERROR(("~~~~~~~~~SP_Store_ClearCache: Bad paramter!~~~~~~~~~~~~~"));
        return EMAIL_BADPARAM;
    }
    
    EMA_UTILS_FREEIF(store_mgr_ptr->mem_mgr_ptr,data_cache_ptr->data_buff_ptr);
    EMA_UTILS_FREEIF(store_mgr_ptr->mem_mgr_ptr,data_cache_ptr->read_report_ptr);
    EMA_UTILS_FREEIF(store_mgr_ptr->mem_mgr_ptr,data_cache_ptr->subject_ptr);
    EMA_UTILS_FREEIF(store_mgr_ptr->mem_mgr_ptr,data_cache_ptr->source_buff_ptr);
    
    len = data_cache_ptr->attach_num;
    for (i = 0; i < len; i++)
    { 
        EMA_UTILS_FREEIF(store_mgr_ptr->mem_mgr_ptr,data_cache_ptr->attach_size_ptr[i]);
    }
    EMA_UTILS_MEMSET(data_cache_ptr,0,sizeof(EMAIL_MSGDATACACHE_T));
    return EMAIL_SUCCESS;
}

/**************************************************************************************/
// Description : email sp store, clear EMN cache.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL int32 SP_Store_ClearEMNCache(
                                   HStoreMgr store_handle_ptr,
                                   EMAIL_EMNCACHE_T *emn_cache_ptr
                                   )
{
    EMAIL_STOREMGR_T  *store_mgr_ptr = (EMAIL_STOREMGR_T*)store_handle_ptr;
    
    //check input param
    if (PNULL == store_mgr_ptr || PNULL == emn_cache_ptr)
    {
        EMA_UTILS_LOG_ERROR(("~~~~~~~~~SP_Store_ClearEMNCache: Bad paramter!~~~~~~~~~~~~~"));
        return EMAIL_BADPARAM;
    }
    
    EMA_UTILS_FREEIF(store_mgr_ptr->mem_mgr_ptr,emn_cache_ptr->host_ptr);
    EMA_UTILS_FREEIF(store_mgr_ptr->mem_mgr_ptr,emn_cache_ptr->mail_box_name_ptr);
    EMA_UTILS_FREEIF(store_mgr_ptr->mem_mgr_ptr,emn_cache_ptr->user_name_ptr);
    return EMAIL_SUCCESS; 
}

/**************************************************************************************/
// Description : email sp store, release EMA msg node.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL void SP_Store_ReleaseEMAMsgNode(void* email_node_ptr)
{
    EMAIL_MSGHEAD_T *email_header_ptr = (EMAIL_MSGHEAD_T*)email_node_ptr;
    
    if (PNULL == email_header_ptr)
    {
        return;
    }
    
    // EMA_UTILS_FREEIF(EMA_UTILS_MEMMGR_FOR_FREE,msg_header_ptr->pwszDate);
    EMA_UTILS_FREEIF(EMA_UTILS_MEMMGR_FOR_FREE,email_header_ptr->wsz_from_ptr);
    EMA_UTILS_FREEIF(EMA_UTILS_MEMMGR_FOR_FREE,email_header_ptr->wsz_subject_ptr);
    EMA_UTILS_FREEIF(EMA_UTILS_MEMMGR_FOR_FREE,email_header_ptr->wsz_to_ptr);
    EMA_UTILS_FREE(EMA_UTILS_MEMMGR_FOR_FREE,email_header_ptr);
}

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
                                          EMAIL_VECTOR_T **vector_model_pptr
                                          )
{
    EMAIL_STOREMGR_T  *store_mgr_ptr = (EMAIL_STOREMGR_T*)store_handle_ptr;
    int32             ret = EMAIL_ERROR;
    EMAIL_VECTOR_T    *vetor_temp_ptr = PNULL;
    int32             i = 0;
    uint32            len = 0;
    EMS_PRFITEM_VAL   value = {0};
    EMAIL_MSGHEAD_T   *email_node_ptr = PNULL;
    HConfigMgr        cfg_mgr_ptr = PNULL;
    EMS_MsgInfo       *msg_info_ptr = PNULL;
    uint32            count = 0;
    
    if (PNULL == store_mgr_ptr|| PNULL == store_mgr_ptr->parent_handle_ptr || PNULL == vector_model_pptr)
    {
        EMA_UTILS_LOG_ERROR(("~~~~~~~~~MMIEMAIL_SP_Store_GetBoxInfo: Bad paramter!~~~~~~~~~~~~~"));
        return EMAIL_BADPARAM;
    }   
    
    do 
    {
        if (EMA_SP_ACTIVE_ACCOUNT == act_id )
        {
            cfg_mgr_ptr = MMIEMAIL_SP_GetCfgHandle(store_mgr_ptr->parent_handle_ptr);
            if ( PNULL == cfg_mgr_ptr)
            {
                EMA_UTILS_LOG_ERROR(("~~~~~~~~~MMIEMAIL_SP_Store_GetBoxInfo: get cfgmgr failed!~~~~~~~~~~~~~"));
                return EMAIL_ERROR;
            }
            
            ret = MMIEMAIL_SP_CFG_GetActiveAct(cfg_mgr_ptr, &act_id);
            if (EMAIL_SUCCESS != ret)
            {
                EMA_UTILS_LOG_ERROR(("MMIEMAIL_SP_Store_GetBoxInfo: MMIEMAIL_SP_CFG_GetActiveAct failed!~"));
                return EMAIL_ERROR;
            }
        }
        
        ret = HEMSSTORE_GetMsgInfo(store_mgr_ptr->qee_store_ptr, act_id, box_id, 0, (unsigned int*)&count, PNULL);
        EMA_UTILS_BREAKIF(ret, "get msg info failed");
        if(0 == count)
        {
            break;
        }
        
        msg_info_ptr = EMA_UTILS_MALLOC(PNULL, count * sizeof(EMS_MsgInfo));
        EMA_UTILS_BREAKNULL(msg_info_ptr, ret, "no memory for msg info");
        EMA_UTILS_MEMSET(msg_info_ptr, 0x00, sizeof(EMS_MsgInfo) * count);
        
        ret = HEMSSTORE_GetMsgInfo(store_mgr_ptr->qee_store_ptr, act_id, box_id, 0, (unsigned int*)&count, msg_info_ptr);
        EMA_UTILS_BREAKIF(ret, "get msg info failed");
        
        MMIEMAIL_VectorNew(&vetor_temp_ptr);
        MMIEMAIL_VectorSetPfnFree(vetor_temp_ptr, SP_Store_ReleaseEMAMsgNode);
        
        ret = HEMSSTORE_GetActCfgItem(store_mgr_ptr->qee_store_ptr, act_id, EMS_PRFITEM_RECEIVE_SERVER_TYPE, &value);
        EMA_UTILS_BREAKIF(ret, "MMIEMAIL_SP_Store_GetBoxInfo:HEMSSTORE_GetActCfgItem failed!");
        
        //       if (EMS_RECEIVE_SERVER_TYPE_IMAP4 == (uint32)value.pData)
        //       {
        //       }
        /*else if (EMS_RECEIVE_SERVER_TYPE_POP3 == (uint32)value.pData)*/
        {
            //         switch(emabtBoxType)
            //         {
            //         case EMA_INBOX:
            //            stBoxIndex = E_INBOX_INDEX;
            //         	break;
            //         case EMA_SENTBOX:
            //            stBoxIndex = E_SENTBOX_INDEX;
            //         	break;
            //         case EMA_DRAFTBOX:
            //            stBoxIndex = E_DRAFT_INDEX;
            //             break;
            //         case EMA_OUTBOX:
            //             stBoxIndex = E_OUTBOX_INDEX;
            //             break;
            //         default:
            //             break;
            //         }
            /* IQEEBASEMSG_GetBaseMsg */
            
            for (i = 0; i < (int32)count; i++)
            {
                /*Message id*/
                email_node_ptr = EMA_UTILS_MALLOC(store_mgr_ptr->mem_mgr_ptr, sizeof(EMAIL_MSGHEAD_T));
                EMA_UTILS_BREAKNULL(email_node_ptr, ret, "MMIEMAIL_SP_Store_GetBoxInfo:Malloc email_node_ptr failed!");
                EMA_UTILS_MEMSET(email_node_ptr, 0x00, sizeof(EMAIL_MSGHEAD_T));
                
                email_node_ptr->email_id        = msg_info_ptr[i].dwMsgId;
                email_node_ptr->act_id          = msg_info_ptr[i].dwActId;
                email_node_ptr->mail_status     = SP_Store_ConventToEmaStatus(msg_info_ptr[i].dwEmlStatus);
                email_node_ptr->email_type      = msg_info_ptr[i].dwEmlType;
                email_node_ptr->dwdate          = msg_info_ptr[i].dwDate;
                email_node_ptr->email_size      = msg_info_ptr[i].dwMsgSize;
                email_node_ptr->email_prio      = msg_info_ptr[i].dwMsgPrio;
                email_node_ptr->wsz_subject_ptr = EMA_UTILS_WSTRDUP(store_mgr_ptr->mem_mgr_ptr, msg_info_ptr[i].wszSubject);
                email_node_ptr->wsz_from_ptr    = EMA_UTILS_WSTRDUP(store_mgr_ptr->mem_mgr_ptr, msg_info_ptr[i].wszFrom);
                
                /*To*/
                if (0 < EMA_UTILS_WSTRLEN(msg_info_ptr[i].wszTo))
                {
                    /*********************Modify in 8.6.2008***************************************************/
                    char   * str_ptr  = PNULL;
                    char   * temp1_ptr = PNULL;
                    char   * temp2_ptr = PNULL;
                    wchar  * temp3_ptr = PNULL;
                    MMI_STRING_T size_string = {0};
                    uint32   size = 0;
                    
                    str_ptr = EMA_UTILS_WSTRTOSTR(store_mgr_ptr->mem_mgr_ptr, msg_info_ptr[i].wszTo);
                    if (PNULL != str_ptr)
                    {
                        temp1_ptr = EMA_UTILS_STRSTR(str_ptr,"recipients");
                    }
                    else
                    {
                        EMA_UTILS_LOG_ERROR(("MMIEMAIL_SP_Store_GetBoxInfo: Failed to get temp while arabic"));
                    }
                    
                    if (PNULL != temp1_ptr && PNULL != str_ptr)
                    {
                        uint32 uiWstrLen = 0;
                        
                        size = temp1_ptr-str_ptr;
                        temp2_ptr = EMA_UTILS_MALLOC(store_mgr_ptr->mem_mgr_ptr, size+1);
                        EMA_UTILS_BREAKNULL(temp2_ptr, ret, "no memory!!");
                        EMA_UTILS_MEMSET(temp2_ptr,0,size);
                        //                         if (stDeviceInfo.dwLang == LNG_CHINESE||stDeviceInfo.dwLang == LNG_SCHINESE||stDeviceInfo.dwLang == LNG_TCHINESE)
                        //                         {
                        //                             EMA_UTILS_MEMCPY(temp2_ptr,str_ptr,size-1);
                        //                         }
                        //                         else
                        //                         {
                        //                             EMA_UTILS_MEMCPY(temp2_ptr,str_ptr,size);
                        //                         }
                        
                        EMA_UTILS_MEMCPY(temp2_ptr, str_ptr, size-1);
                        
                        temp3_ptr = EMA_UTILS_STRTOWSTR(store_mgr_ptr->mem_mgr_ptr, temp2_ptr);
                        EMA_UTILS_BREAKNULL(temp3_ptr, ret, "no memory!!");
                        
                        MMI_GetLabelTextByLang(TXT_EMAIL_RECIPIENTS, &size_string);
                        
                        //len = EMA_UTILS_WSTRLEN(temp4_ptr) * 2 + size*2+2;
                        len = size_string.wstr_len * 2 + size*2 + 2;
                        email_node_ptr->wsz_to_ptr = EMA_UTILS_MALLOC(store_mgr_ptr->mem_mgr_ptr, len);
                        EMA_UTILS_BREAKNULL(email_node_ptr->wsz_to_ptr, ret,"MMIEMAIL_SP_Store_GetBoxInfo:Malloc email_node_ptr[i].wsz_to_ptr failed!");
                        EMA_UTILS_MEMSET(email_node_ptr->wsz_to_ptr, 0, len);
                        //(void)EMA_UTILS_WSPRINTF(msg_node_ptr->wsz_to_ptr,len,L"%s%s", temp3_ptr,temp4_ptr);
                        //(void)EMA_UTILS_WSPRINTF(msg_node_ptr->wsz_to_ptr, len, L"%s%s", temp3_ptr, size_string.wstr_ptr);
                        
                        uiWstrLen = EMA_UTILS_WSTRLEN(temp3_ptr) * sizeof(wchar);
                        EMA_UTILS_MEMCPY(email_node_ptr->wsz_to_ptr, temp3_ptr, uiWstrLen);
                        EMA_UTILS_MEMCPY(email_node_ptr->wsz_to_ptr + EMA_UTILS_WSTRLEN(temp3_ptr), size_string.wstr_ptr, size_string.wstr_len * 2);
                        
                        EMA_UTILS_FREE(store_mgr_ptr->mem_mgr_ptr, temp2_ptr);
                        EMA_UTILS_FREE(store_mgr_ptr->mem_mgr_ptr, temp3_ptr);
                        //EMA_UTILS_FREEIF(store_mgr_ptr->mem_mgr_ptr, temp4_ptr);
                    }
                    else
                    {
                        len = EMA_UTILS_WSTRLEN(msg_info_ptr[i].wszTo) * 2 + 2;
                        email_node_ptr->wsz_to_ptr = EMA_UTILS_MALLOC(store_mgr_ptr->mem_mgr_ptr,len);
                        EMA_UTILS_BREAKNULL(email_node_ptr->wsz_to_ptr, ret, "MMIEMAIL_SP_Store_GetBoxInfo:Malloc email_node_ptr[i].wsz_to_ptr failed!");
                        
                        EMA_UTILS_MEMSET(email_node_ptr->wsz_to_ptr, 0, len);
                        EMA_UTILS_MEMCPY(email_node_ptr->wsz_to_ptr, msg_info_ptr[i].wszTo, len);
                    }
                    
                    EMA_UTILS_FREEIF(store_mgr_ptr->mem_mgr_ptr, str_ptr);
                }
                
                MMIEMAIL_VectorInsert(vetor_temp_ptr, email_node_ptr);
                EMA_UTILS_BREAKIF(ret, "MMIEMAIL_SP_Store_GetBoxInfo:IQEEBASEMSG_GetBaseMsg failed!");
            }
      }
      
      *vector_model_pptr = vetor_temp_ptr;
      
   } while(0); 
   
   EMA_UTILS_FREEIF(PNULL, msg_info_ptr);
   
   if (EMAIL_SUCCESS != ret || 0 == count)
   {
       MMIEMAIL_VectorDelete(vetor_temp_ptr);
       *vector_model_pptr = PNULL;
   }
   
   return ret;
}

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
                                         )
{
    EMAIL_STOREMGR_T    *store_mgr_ptr = (EMAIL_STOREMGR_T*)store_handle_ptr;
    HConfigMgr  cfg_mgr_ptr = PNULL;
    int         ret = EMAIL_SUCCESS;
    
    if (PNULL == store_mgr_ptr|| PNULL == store_mgr_ptr->qee_store_ptr || PNULL == box_name_ptr || PNULL == box_id_ptr || 0 == act_id || 0 == box_fa_id)
    {
        EMA_UTILS_LOG_ERROR(("~~~~~~~~~MMIEMAIL_SP_Store_CreateBox: Bad paramter!~~~~~~~~~~~~~"));
        return EMAIL_BADPARAM;
    }   
    
    if (EMA_SP_ACTIVE_ACCOUNT == act_id )
    {
        cfg_mgr_ptr = MMIEMAIL_SP_GetCfgHandle(store_mgr_ptr->parent_handle_ptr);
        if (PNULL == cfg_mgr_ptr)
        {
            EMA_UTILS_LOG_ERROR(("~~~~~~~~~MMIEMAIL_SP_Store_CreateBox: get cfgmgr failed!~~~~~~~~~~~~~"));
            return EMAIL_ERROR;
        }
        
        ret = MMIEMAIL_SP_CFG_GetActiveAct(cfg_mgr_ptr,&act_id);
        if (EMAIL_SUCCESS != ret)
        {
            EMA_UTILS_LOG_ERROR(("MMIEMAIL_SP_Store_CreateBox: MMIEMAIL_SP_CFG_GetActiveAct failed!~"));
            return EMAIL_ERROR;
        }
    }
    return HEMSSTORE_CreateBox(store_mgr_ptr->qee_store_ptr,box_name_ptr,(unsigned int*)box_id_ptr,act_id,state);
}

/**************************************************************************************/
// Description : email sp store, create a email box.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC int MMIEMAIL_SP_Store_DeleteBox(
                                       HStoreMgr     store_handle_ptr,
                                       uint32        box_id,
                                       uint32        act_id,
                                       uint32        box_fa_id
                                       )
{
    EMAIL_STOREMGR_T       *store_mgr_ptr = (EMAIL_STOREMGR_T*)store_handle_ptr;
    
    if (PNULL == store_mgr_ptr|| PNULL == store_mgr_ptr->qee_store_ptr || 0 == box_id || 0 == act_id || 0 == box_fa_id)
    {
        EMA_UTILS_LOG_ERROR(("~~~~~~~~~MMIEMAIL_SP_Store_DeleteBox: Bad paramter!~~~~~~~~~~~~~"));
        return EMAIL_BADPARAM;
    }   
    
    return HEMSSTORE_DeleteBox(store_mgr_ptr->qee_store_ptr,box_id,act_id);
}

/**************************************************************************************/
// Description : email sp store, free box spec info.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL void SP_Store_GetSpecBoxInfo_Free(void *data_ptr)
{
   EMA_UTILS_FREEIF(PNULL, data_ptr);
}

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
                                              )
{
    int               ret = EMAIL_SUCCESS;
    EMAIL_STOREMGR_T  *store_mgr_ptr = (EMAIL_STOREMGR_T*)store_handle_ptr;
    int               i = 0;
    EMS_BoxInfo       *ems_box_info_ptr = PNULL;
    EMAIL_SUBBOX_INFO_T  *box_info_ptr = PNULL;
    HConfigMgr  cfg_mgr_ptr = PNULL;
    uint32      cnt = 0;
    
    if (PNULL == store_mgr_ptr || PNULL == store_mgr_ptr->qee_store_ptr ||0 == act_id || PNULL == vector_model_ptr)
    {
        EMA_UTILS_LOG_ERROR(("~~~~~~~~~MMIEMAIL_SP_Store_GetSpecBoxInfo: Bad paramter!~~~~~~~~~~~~~"));
        return EMAIL_BADPARAM;
    }
    
    do
    {
        MMIEMAIL_VectorSetPfnFree(vector_model_ptr, SP_Store_GetSpecBoxInfo_Free);
        
        if (EMA_SP_ACTIVE_ACCOUNT == act_id )
        {
            cfg_mgr_ptr = MMIEMAIL_SP_GetCfgHandle(store_mgr_ptr->parent_handle_ptr);
            if (PNULL == cfg_mgr_ptr)
            {
                EMA_UTILS_LOG_ERROR(("~~~~~~~~~MMIEMAIL_SP_Store_GetSpecBoxInfo: get cfgmgr failed!~~~~~~~~~~~~~"));
                return EMAIL_ERROR;
            }
            ret = MMIEMAIL_SP_CFG_GetActiveAct(cfg_mgr_ptr,&act_id);
            if (EMAIL_SUCCESS != ret)
            {
                EMA_UTILS_LOG_ERROR(("MMIEMAIL_SP_Store_GetSpecBoxInfo: MMIEMAIL_SP_CFG_GetActiveAct failed!~"));
                return EMAIL_ERROR;
            }
        }
        
        ret = HEMSSTORE_GetBoxInfo(store_mgr_ptr->qee_store_ptr, act_id, 0, (unsigned int*)&cnt, PNULL);
        EMA_UTILS_BREAKIF(ret, "get box info failed");
        
        ems_box_info_ptr = EMA_UTILS_MALLOC(PNULL, cnt * sizeof(EMS_BoxInfo));
        EMA_UTILS_BREAKNULL(ems_box_info_ptr, ret, "no memory");
        EMA_UTILS_MEMSET(ems_box_info_ptr, 0x00, sizeof(EMS_BoxInfo) * cnt);
        
        ret = HEMSSTORE_GetBoxInfo(store_mgr_ptr->qee_store_ptr, act_id, 0, (unsigned int*)&cnt, ems_box_info_ptr);
        EMA_UTILS_BREAKIF(ret, "get box info failed");
        
        for(i = 0; i < (int32)cnt; i++)
        {
            if(ems_box_info_ptr[i].ucState & state)
            {
                uint32 uiWstrLen = 0;
                box_info_ptr = EMA_UTILS_MALLOC(PNULL, sizeof(EMAIL_SUBBOX_INFO_T));
                EMA_UTILS_BREAKNULL(box_info_ptr, ret, "no memory");
                EMA_UTILS_MEMSET(box_info_ptr, 0x00, sizeof(EMAIL_SUBBOX_INFO_T));
                
                box_info_ptr->box_id = ems_box_info_ptr[i].uiBoxId;
                box_info_ptr->num    = ems_box_info_ptr[i].uiUnRead;
                box_info_ptr->is_ser = FALSE;
                
                uiWstrLen = EMA_UTILS_WSTRLEN(ems_box_info_ptr[i].wszFolder) * sizeof(wchar);
                EMA_UTILS_MEMCPY(box_info_ptr->wsz_folder, ems_box_info_ptr[i].wszFolder, uiWstrLen);
                
                ret = MMIEMAIL_VectorInsert(vector_model_ptr, box_info_ptr);
            }
        }
    } while(0);
    
    EMA_UTILS_FREEIF(PNULL, ems_box_info_ptr);
    
    return ret;
}

/**************************************************************************************/
// Description : email sp store, set box's status.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC int MMIEMAIL_SP_Store_SetBoxStatus(
                                          HStoreMgr  store_handle_ptr,
                                          uint32     act_id,
                                          wchar      *box_name_ptr,
                                          uint8      box_status
                                          )
{
    int         ret = EMAIL_SUCCESS;
    EMAIL_STOREMGR_T    *store_mgr_ptr = (EMAIL_STOREMGR_T*)store_handle_ptr;
    HConfigMgr  cfg_mgr_ptr = PNULL;
    
    if (PNULL == store_mgr_ptr || PNULL == store_mgr_ptr->qee_store_ptr||0 == act_id)
    {
        EMA_UTILS_LOG_ERROR(("~~~~~~~~~MMIEMAIL_SP_Store_SetBoxStatus: Bad paramter!~~~~~~~~~~~~~"));
        return EMAIL_BADPARAM;
    } 
    
    if (EMA_SP_ACTIVE_ACCOUNT == act_id )
    {
        cfg_mgr_ptr = MMIEMAIL_SP_GetCfgHandle(store_mgr_ptr->parent_handle_ptr);
        if (PNULL == cfg_mgr_ptr)
        {
            EMA_UTILS_LOG_ERROR(("~~~~~~~~~MMIEMAIL_SP_Store_SetBoxStatus: get cfgmgr failed!~~~~~~~~~~~~~"));
            return EMAIL_ERROR;
        }
        
        ret = MMIEMAIL_SP_CFG_GetActiveAct(cfg_mgr_ptr,&act_id);
        if (EMAIL_SUCCESS != ret)
        {
            EMA_UTILS_LOG_ERROR(("MMIEMAIL_SP_Store_SetBoxStatus: MMIEMAIL_SP_CFG_GetActiveAct failed!~"));
            return EMAIL_ERROR;
        }
    }
    
    return EMAIL_ERROR;
}

/**************************************************************************************/
// Description : email sp store, get box info.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL int SP_Store_GetBoxU(
                           HStoreMgr  store_handle_ptr,
                           uint32     act_id,
                           wchar      *box_name_ptr,
                           uint8      *box_status_ptr,
                           BOOLEAN    *is_ret_ptr 
                           )
{
    int         ret = EMAIL_SUCCESS;
    EMAIL_STOREMGR_T    *store_mgr_ptr = (EMAIL_STOREMGR_T*)store_handle_ptr;
    HConfigMgr  cfg_mgr_ptr = PNULL;
    EMS_BoxInfo *ems_box_info_ptr = PNULL;
    uint32      cnt = 0;
    uint32      i = 0;
    
    if (PNULL == store_mgr_ptr || PNULL == store_mgr_ptr->qee_store_ptr|| 0 == act_id || PNULL == box_name_ptr || PNULL == box_status_ptr || PNULL ==is_ret_ptr)
    {
        EMA_UTILS_LOG_ERROR(("~~~~~~~~~SP_Store_GetBoxU: Bad paramter!~~~~~~~~~~~~~"));
        return EMAIL_BADPARAM;
    }
    
    do 
    {
        *is_ret_ptr = FALSE;
        
        if (EMA_SP_ACTIVE_ACCOUNT == act_id )
        {
            cfg_mgr_ptr = MMIEMAIL_SP_GetCfgHandle(store_mgr_ptr->parent_handle_ptr);
            if (PNULL == cfg_mgr_ptr)
            {
                EMA_UTILS_LOG_ERROR(("~~~~~~~~~SP_Store_GetBoxU: get cfgmgr failed!~~~~~~~~~~~~~"));
                return EMAIL_ERROR;
            }
            ret = MMIEMAIL_SP_CFG_GetActiveAct(cfg_mgr_ptr,&act_id);
            if (EMAIL_SUCCESS != ret)
            {
                EMA_UTILS_LOG_ERROR(("SP_Store_GetBoxU: MMIEMAIL_SP_CFG_GetActiveAct failed!~"));
                return EMAIL_ERROR;
            }
        }
        
        ret = HEMSSTORE_GetBoxInfo(store_mgr_ptr->qee_store_ptr, act_id, 0, (unsigned int*)&cnt, PNULL);
        EMA_UTILS_BREAKIF(ret, "get box info failed");
        
        ems_box_info_ptr = EMA_UTILS_MALLOC(PNULL, sizeof(EMS_BoxInfo) * cnt);
        EMA_UTILS_BREAKNULL(ems_box_info_ptr, ret, "no memory");
        EMA_UTILS_MEMSET(ems_box_info_ptr, 0x00, sizeof(EMS_BoxInfo) * cnt);
        
        ret = HEMSSTORE_GetBoxInfo(store_mgr_ptr->qee_store_ptr, act_id, 0, (unsigned int*)&cnt, ems_box_info_ptr);
        EMA_UTILS_BREAKIF(ret, "get box info failed");
        
        for(i = 0; i < cnt; i++)
        {
            if(0 == EMA_UTILS_WSTRCMP(ems_box_info_ptr[i].wszFolder, box_name_ptr))
            {
                *box_status_ptr = ems_box_info_ptr[i].ucState;
                *is_ret_ptr = TRUE;
            }
        }
        
    } while(0);
    
    EMA_UTILS_FREEIF(PNULL, ems_box_info_ptr);
    
    return ret;
}

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
                                          )
{
    BOOLEAN  bRet = FALSE; 
    if (PNULL == store_handle_ptr ||0 == act_id || PNULL== box_name_ptr || PNULL ==box_status_ptr)
    {
        EMA_UTILS_LOG_ERROR(("~~~~~~~~~MMIEMAIL_SP_Store_GetBoxStatus: Bad paramter!~~~~~~~~~~~~~"));
        return EMAIL_BADPARAM;
    }  
    return SP_Store_GetBoxU(store_handle_ptr,act_id,box_name_ptr,box_status_ptr,&bRet);
}

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
                                       )
{
    uint8  box_status = 0; 
    if (PNULL == store_handle_ptr||0 == act_id  || PNULL== box_name_ptr || PNULL ==is_ret_ptr)
    {
        EMA_UTILS_LOG_ERROR(("~~~~~~~~~MMIEMAIL_SP_Store_IsBoxExit: Bad paramter!~~~~~~~~~~~~~"));
        return EMAIL_BADPARAM;
    }
    return SP_Store_GetBoxU(store_handle_ptr,act_id,box_name_ptr,&box_status,is_ret_ptr);
}

/**************************************************************************************/
// Description : email sp store, free buffer.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL void SP_Store_FreeBuf(void* data_ptr)
{
    EMA_UTILS_FREEIF(EMA_UTILS_MEMMGR_FOR_FREE, data_ptr);
}

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
                                    BOOLEAN    *is_down_ptr
                                    )
{
    EMAIL_STOREMGR_T       *store_mgr_ptr = (EMAIL_STOREMGR_T*)store_handle_ptr;
    HEMSMsg        qee_msg_ptr = PNULL;
    EMSMsgOpt      stQEEMsgOpt = {0};
    int            ret = EMAIL_SUCCESS;
    HConfigMgr     cfg_mgr_ptr = PNULL;
    
    
    if (PNULL == store_mgr_ptr || PNULL == store_mgr_ptr->qee_store_ptr || 0 == act_id  || 0== box_id || 0 ==email_id|| PNULL == is_down_ptr)
    {
        EMA_UTILS_LOG_ERROR(("~~~~~~~~~MMIEMAIL_SP_Store_Bedown: Bad paramter!~~~~~~~~~~~~~"));
        return EMAIL_BADPARAM;
    }
    
    do 
    {
        if (EMA_SP_ACTIVE_ACCOUNT == act_id )
        {
            cfg_mgr_ptr = MMIEMAIL_SP_GetCfgHandle(store_mgr_ptr->parent_handle_ptr);
            if (PNULL == cfg_mgr_ptr)
            {
                EMA_UTILS_LOG_ERROR(("~~~~~~~~~MMIEMAIL_SP_Store_Bedown: get cfgmgr failed!~~~~~~~~~~~~~"));
                return EMAIL_ERROR;
            }
            
            ret = MMIEMAIL_SP_CFG_GetActiveAct(cfg_mgr_ptr,&act_id);
            if (EMAIL_SUCCESS != ret)
            {
                EMA_UTILS_LOG_ERROR(("MMIEMAIL_SP_Store_Bedown: MMIEMAIL_SP_CFG_GetActiveAct failed!~"));
                return EMAIL_ERROR;
            }
        } 
        
        ret = HEMSSTORE_GetMsg(store_mgr_ptr->qee_store_ptr, email_id, &qee_msg_ptr);
        EMA_UTILS_BREAKIF(ret, "MMIEMAIL_SP_Store_Bedown:create vec failed");
        
        ret = HEMSMSG_GetOpt(qee_msg_ptr,EMS_MSGOPT_BEDOWN, 0, &stQEEMsgOpt);
        EMA_UTILS_BREAKIF(ret, "MMIEMAIL_SP_Store_Bedown:get opt bedown failed");
        
        *is_down_ptr = (BOOLEAN)(uint32)stQEEMsgOpt.pVal;
    } while(0);
    
    if(PNULL != qee_msg_ptr)
    {
        HEMSMSG_Delete(qee_msg_ptr);
    }
    
    return ret;
}

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
                                             )
{
    int            ret = EMAIL_SUCCESS;
    EMAIL_STOREMGR_T *store_mgr_ptr = (EMAIL_STOREMGR_T*)store_handle_ptr;
    int            i = 0;
    EMS_ActInfo    *ems_act_info_ptr = PNULL;
    EMS_PRFITEM_VAL value = {0} ;
    uint32          cnt = 0;
    
    if (PNULL == store_mgr_ptr || PNULL == store_mgr_ptr->qee_store_ptr || PNULL ==email_addr_ptr || PNULL ==act_id_ptr)
    {
        EMA_UTILS_LOG_ERROR(("~~~~~~~~~MMIEMAIL_SP_Store_GetActIdByEmail: Bad paramter!~~~~~~~~~~~~~"));
        return EMAIL_BADPARAM;
    }
    
    do
    {
        *act_id_ptr = 0 ;
        
        ret = HEMSSTORE_GetActInfo(store_mgr_ptr->qee_store_ptr, 0, (unsigned int*)&cnt, PNULL);
        EMA_UTILS_BREAKIF(ret, "get act info failed");
        
        ems_act_info_ptr = EMA_UTILS_MALLOC(PNULL, sizeof(EMS_ActInfo) * cnt);
        EMA_UTILS_BREAKNULL(ems_act_info_ptr, ret, "no memory");
        EMA_UTILS_MEMSET(ems_act_info_ptr, 0x00, sizeof(EMS_ActInfo) * cnt);
        
        ret = HEMSSTORE_GetActInfo(store_mgr_ptr->qee_store_ptr, 0, (unsigned int*)&cnt, ems_act_info_ptr);
        EMA_UTILS_BREAKIF(ret, "get act info failed");
        
        for(i = 0; i < (int)cnt; i++)
        {
            ret = HEMSSTORE_GetActCfgItem(store_mgr_ptr->qee_store_ptr, ems_act_info_ptr[i].uiActId, EMS_PRFITEM_ACCOUNT_EMAIL, &value); 
            EMA_UTILS_BREAKIF(ret, "MMIEMAIL_SP_Store_GetActIdByEmail,get account email in cfg file failed!");
            
            if (!EMA_UTILS_WSTRCMP(email_addr_ptr,(wchar*)value.pData))
            {
                *act_id_ptr = ems_act_info_ptr->uiActId;
                break;
            }
            EMA_UTILS_FREEIF(store_mgr_ptr->mem_mgr_ptr,value.pData);
            
        }
        
    } while(0);
    
    EMA_UTILS_FREEIF(store_mgr_ptr->mem_mgr_ptr,value.pData);
    
    EMA_UTILS_FREEIF(PNULL, ems_act_info_ptr);
    
    return ret;
}

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
                                             )
{
    int            ret = EMAIL_SUCCESS;
    EMAIL_STOREMGR_T *store_mgr_ptr = (EMAIL_STOREMGR_T*)store_handle_ptr;
    int            count = 0;
    int            num = 0;
    HEMSMsg        qee_msg_ptr = PNULL;
    HEMSMsgPart    qee_msg_part_ptr = PNULL;
    EMSMsgOpt      part_opt = {0};
    BOOLEAN        is_del = FALSE;
    HConfigMgr     cfg_mgr_ptr = PNULL;
    
    if (PNULL == store_mgr_ptr || PNULL == store_mgr_ptr->qee_store_ptr || 0 ==act_id ||((is_attach_or_text==TRUE) && (part_dir_ptr == PNULL)))
    {
        EMA_UTILS_LOG_ERROR(("~~~~~~~~~MMIEMAIL_SP_Store_RemovePartByDir: Bad paramter!~~~~~~~~~~~~~"));
        return EMAIL_BADPARAM;
    }
    
    do
    {
        if (EMA_SP_ACTIVE_ACCOUNT == act_id )
        {
            cfg_mgr_ptr = MMIEMAIL_SP_GetCfgHandle(store_mgr_ptr->parent_handle_ptr);
            if (PNULL == cfg_mgr_ptr)
            {
                EMA_UTILS_LOG_ERROR(("~~~~~~~~~MMIEMAIL_SP_Store_RemovePartByDir: get cfgmgr failed!~~~~~~~~~~~~~"));
                return EMAIL_ERROR;
            }
            ret = MMIEMAIL_SP_CFG_GetActiveAct(cfg_mgr_ptr,&act_id);
            if (EMAIL_SUCCESS != ret)
            {
                EMA_UTILS_LOG_ERROR(("MMIEMAIL_SP_Store_RemovePartByDir: MMIEMAIL_SP_CFG_GetActiveAct failed!~"));
                return EMAIL_ERROR;
            }
        } 
        
        ret = HEMSSTORE_GetMsg(store_mgr_ptr->qee_store_ptr, email_id, &qee_msg_ptr);
        EMA_UTILS_BREAKIF(ret, "MMIEMAIL_SP_Store_RemovePartByDir:IVECTORMODEL_GetAt!");
        
        if (PNULL != qee_msg_ptr)
        {
            wchar *str_temp_ptr = PNULL;
            
            count = HEMSMSG_GetPartCount(qee_msg_ptr);
            for (;num < count; num++)
            {
                ret = HEMSMSG_GetMessagePart(qee_msg_ptr, EMS_MSGPART_ID_POSITION, (void*)num, &qee_msg_part_ptr);
                EMA_UTILS_BREAKIF(ret, "MMIEMAIL_SP_Store_RemovePartByDir,IQEEMULTIPARTMSG_GetMessagePart failed!");
                
                EMA_UTILS_MEMSET(&part_opt,0,sizeof(EMSMsgOpt));
                if (is_attach_or_text == TRUE)
                {
                    ret = HEMSMSGPART_GetOpt(qee_msg_part_ptr, EMS_PARTOPT_DATABUF, 0, &part_opt);
                    EMA_UTILS_BREAKIF(ret, "MMIEMAIL_SP_Store_RemovePartByDir,HEMSMSG_GetOpt failed!");
                    
                    str_temp_ptr = EMA_UTILS_UTF8TOWSTR(PNULL, part_opt.pVal);
                    
                    if (!EMA_UTILS_WSTRCMP(str_temp_ptr,part_dir_ptr))
                    {
                        is_del = TRUE;
                    }
                    
                    EMA_UTILS_FREEIF(PNULL, str_temp_ptr);
                }
                else
                {
                    ret = HEMSMSGPART_GetOpt(qee_msg_part_ptr,EMS_PARTOPT_PARTTYPE, 0, &part_opt);
                    EMA_UTILS_BREAKIF(ret, "MMIEMAIL_SP_Store_RemovePartByDir,HEMSMSG_GetOpt failed!");
                    
                    if (EMSPART_TYPE_TEXT_PLAIN == (uint32)part_opt.pVal)
                    {
                        is_del = TRUE;
                    }
                }
                
                if (TRUE == is_del)
                {
                    ret = HEMSMSG_RemoveMessagePart(qee_msg_ptr,EMS_MSGPART_ID_POSITION,(void*)&num);
                    EMA_UTILS_BREAKIF(ret, "MMIEMAIL_SP_Store_RemovePartByDir,IQEEMULTIPARTMSG_GetMessagePart failed!");
                    
                    ret = HEMSSTORE_UpdateMsg(store_mgr_ptr->qee_store_ptr,qee_msg_ptr);
                    EMA_UTILS_BREAKIF(ret, "MMIEMAIL_SP_Store_RemovePartByDir,HEMSSTORE_UpdateMsg failed!");
                    break;
                }
                
                HEMSMSGPART_Delete(qee_msg_part_ptr);
                qee_msg_part_ptr = PNULL;
            }
            
            if (num >= count)
            {
                ret = EMAIL_ERROR;
            }
        }
        
        EMA_UTILS_BREAKIF(ret, "MMIEMAIL_SP_Store_RemovePartByDir: failed!");
        
    }while(0);
    
    if(PNULL != qee_msg_part_ptr)
    {
        HEMSMSGPART_Delete(qee_msg_part_ptr);
    }
    
    if(PNULL != qee_msg_ptr)
    {
        HEMSMSG_Delete(qee_msg_ptr);
    }
    
    return ret;
}

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
                                              )
{
    EMAIL_STOREMGR_T       *store_mgr_ptr = (EMAIL_STOREMGR_T*)store_handle_ptr;
    int32          result = EMAIL_SUCCESS;
    HConfigMgr     cfg_mgr_ptr = PNULL;
    HEMSMsg        qee_msg_ptr = PNULL;
    HEMSMsgPart    qee_msg_part_ptr = PNULL;
    EMSMsgOpt      part_opt = {0};
    int            num = 0;
    int            count = 0;
    uint32         email_part_type = 0;
//    char           *file_name_ptr = PNULL;
    char           *buffer_ptr = PNULL;
    char           *source_buffer_ptr = PNULL;
    uint32         attach_buff_len = 0;
    char           *attach_uri_ptr[EMA_MAX_ATTACH_NUM] = {0};
    char           *attach_conten_info_ptr[EMA_MAX_ATTACH_NUM] = {0};
    uint16         attach_uri[EMA_MAX_ATTACH_NUM] = {0};
    int            attach_num = 0;
    SFS_HANDLE     file = PNULL;
    
    if (PNULL == store_mgr_ptr || PNULL == view_type_ptr )
    {
        EMA_UTILS_LOG_ERROR(("MMIEMAIL_SP_Store_GetViewEmlType, bad param."));
        return EMAIL_BADPARAM;
    }
    
    do 
    {
        if (EMA_SP_ACTIVE_ACCOUNT == act_id )
        {
            cfg_mgr_ptr = MMIEMAIL_SP_GetCfgHandle(store_mgr_ptr->parent_handle_ptr);
            EMA_UTILS_BREAKNULL(cfg_mgr_ptr, result, "MMIEMAIL_SP_Store_GetViewEmlType: get cfgmgr failed!");
            
            result = MMIEMAIL_SP_CFG_GetActiveAct(cfg_mgr_ptr,&act_id);
            EMA_UTILS_BREAKIF(result, "MMIEMAIL_SP_Store_GetViewEmlType: MMIEMAIL_SP_CFG_GetActiveAct failed!");
        }
        
        result = HEMSSTORE_GetMsg(store_mgr_ptr->qee_store_ptr, email_id, &qee_msg_ptr);
        EMA_UTILS_BREAKIF(result, "MMIEMAIL_SP_Store_GetViewEmlType:IQEESTORE_GetMsgs failed!");
        
        count = HEMSMSG_GetPartCount(qee_msg_ptr);
        for (num = 0; num < count; num++)
        {
            result = HEMSMSG_GetMessagePart(qee_msg_ptr, EMS_MSGPART_ID_POSITION, (void*)num, &qee_msg_part_ptr);
            EMA_UTILS_BREAKIF(result, "MMIEMAIL_SP_Store_GetViewEmlType,IQEEMULTIPARTMSG_GetMessagePart failed!");
            EMA_UTILS_MEMSET(&part_opt,0,sizeof(EMSMsgOpt));
            
            result = HEMSMSGPART_GetOpt(qee_msg_part_ptr, EMS_PARTOPT_SRCTYPE, 0, &part_opt);
            EMA_UTILS_BREAKIF(result, "MMIEMAIL_SP_Store_GetViewEmlType,HEMSMSG_GetOpt failed!");
            
            if (EMSPART_TYPE_TEXT_HTML == (uint32)part_opt.pVal)
            {
                *view_type_ptr = EMA_MSGPART_CONTENT_TYPE_TEXT_HTML;
                
                email_part_type = EMSPART_TYPE_TEXT_HTML;
                
                EMA_UTILS_MEMSET(&part_opt,0,sizeof(EMSMsgOpt));
                
                result = HEMSMSGPART_GetOpt(qee_msg_part_ptr, EMS_PARTOPT_DATABUF, 0, &part_opt);
                EMA_UTILS_BREAKIF(result, "MMIEMAIL_SP_Store_GetViewEmlType,HEMSMSG_GetOpt failed!");
                
//                file_name_ptr = EMA_UTILS_STRDUP(store_mgr_ptr->mem_mgr_ptr, part_opt.pVal);
            }
            else if (EMSPART_TYPE_ATTACHMENT == (uint32)part_opt.pVal && attach_num < EMA_MAX_ATTACH_NUM)
            {
                wchar *temp_ptr = PNULL;
                uint32 temp_len = 0;
                
                EMA_UTILS_MEMSET(&part_opt,0,sizeof(EMSMsgOpt));
                
                result = HEMSMSGPART_GetOpt(qee_msg_part_ptr,EMS_PARTOPT_CONTENTID, 0, &part_opt);
                if (EMAIL_BADPARAM != result)
                {
                    wchar *str_temp_ptr = EMA_UTILS_UTF8TOWSTR(PNULL, part_opt.pVal);
                    
                    EMA_UTILS_BREAKIF(result, "MMIEMAIL_SP_Store_GetViewEmlType,HEMSMSG_GetOpt failed!");
                    
                    temp_len = (EMA_UTILS_WSTRLEN(str_temp_ptr) + EMA_UTILS_WSTRLEN((wchar *)L"cid:")) * 2 + 2;
                    
                    temp_ptr = EMA_UTILS_MALLOC(store_mgr_ptr->mem_mgr_ptr, temp_len);
                    EMA_UTILS_BREAKNULL(temp_ptr, result, "MMIEMAIL_SP_Store_GetViewEmlType, temp_ptr is PNULL.");
                    EMA_UTILS_MEMSET(temp_ptr, 0, temp_len);
                    
                    EMA_UTILS_WSTRCAT(temp_ptr, (wchar *)L"cid:");
                    EMA_UTILS_WSTRCAT(temp_ptr, str_temp_ptr);
                    EMA_UTILS_FREEIF(PNULL, str_temp_ptr);
                    
                    attach_conten_info_ptr[attach_num] = EMA_UTILS_WSTRTOUTF8(store_mgr_ptr->mem_mgr_ptr, temp_ptr);
                    EMA_UTILS_FREE(store_mgr_ptr->mem_mgr_ptr, temp_ptr);
                    
                    EMA_UTILS_MEMSET(&part_opt,0,sizeof(EMSMsgOpt));
                    
                    result = HEMSMSGPART_GetOpt(qee_msg_part_ptr,EMS_PARTOPT_DATABUF, 0, &part_opt);
                    EMA_UTILS_BREAKIF(result, "MMIEMAIL_SP_Store_GetViewEmlType,HEMSMSG_GetOpt failed!");
                    
                    attach_buff_len += (uint32)EMA_UTILS_STRLEN((char*)part_opt.pVal + 1);
                    attach_uri[attach_num] = EMA_UTILS_STRLEN((char*)part_opt.pVal + 1);
                    attach_uri_ptr[attach_num++] = EMA_UTILS_STRDUP(store_mgr_ptr->mem_mgr_ptr, (char*)part_opt.pVal + 1);
                }
                else
                {
                    result = EMAIL_SUCCESS;
                }
            }
            
            HEMSMSGPART_Delete(qee_msg_part_ptr);
            qee_msg_part_ptr = PNULL;
        }
        
#if 0
        if (EMSPART_TYPE_TEXT_HTML == email_part_type )
        {  
            file_full_path_len = GUI_GBToWstr(file_full_path, (uint8 *)file_name_ptr, strlen(file_name_ptr));
            file = SFS_CreateFile(file_full_path, SFS_MODE_OPEN_EXISTING | SFS_MODE_READ, PNULL, PNULL);
            EMA_UTILS_BREAKNULL(file, result, "file is PNULL.");
            
            SFS_GetFileSize(file, &file_size);
            
            if (PNULL != file_name_ptr)
            {
                EMA_UTILS_MEMCPY(new_name, file_name_ptr, EMA_UTILS_STRLEN(file_name_ptr) - 5);
                EMA_UTILS_STRCAT(new_name, "_view.html");
            }
            
            buffer_ptr = EMA_UTILS_MALLOC(store_mgr_ptr->mem_mgr_ptr, file_size * 2);
            EMA_UTILS_BREAKNULL(buffer_ptr, result, "MMIEMAIL_SP_Store_GetViewEmlType, buffer_ptr is PNULL.");
            EMA_UTILS_MEMSET(buffer_ptr, 0, file_size*2);
            
            SFS_ReadFile(file, buffer_ptr, file_size, &read_len, 0);
            SFS_CloseFile(file);
            source_buffer_ptr = EMA_UTILS_MALLOC(store_mgr_ptr->mem_mgr_ptr, file_size * 2);
            EMA_UTILS_BREAKNULL(source_buffer_ptr, result, "MMIEMAIL_SP_Store_GetViewEmlType, source_buffer_ptr is PNULL.");
            EMA_UTILS_MEMSET(source_buffer_ptr, 0, file_size * 2);
            
            for (num = 0; num < attach_num; num++)
            {
                int i = 0;
                char *temp_buffer_ptr = PNULL;
                
                temp_buffer_ptr = buffer_ptr;
                EMA_UTILS_MEMSET(source_buffer_ptr, 0, file_size*2);
                if (PNULL != attach_conten_info_ptr[num])
                {
                    while('\0' != *temp_buffer_ptr)
                    {
                        EMA_UTILS_MEMCMP(temp_buffer_ptr, attach_conten_info_ptr[num], EMA_UTILS_STRLEN(attach_conten_info_ptr[num]));
                        
                        source_buffer_ptr[i++] = *temp_buffer_ptr;
                        temp_buffer_ptr++;
                    }
                }
                EMA_UTILS_MEMSET(buffer_ptr, 0, file_size*2);
                EMA_UTILS_MEMCPY(buffer_ptr, source_buffer_ptr, file_size*2);
            }
            
            if(0 == attach_num&& PNULL != buffer_ptr)
            {
                EMA_UTILS_MEMCPY(source_buffer_ptr, buffer_ptr, file_size*2);
            }
            
            if (PNULL != source_buffer_ptr)
            {
                *source_buff_pptr = source_buffer_ptr;
                source_buffer_ptr = PNULL;
            }
        }
#endif  
   } while(0);
   
   for (num = 0; num < attach_num; num++)
   {
       EMA_UTILS_FREEIF(store_mgr_ptr->mem_mgr_ptr, attach_conten_info_ptr[num]);
       EMA_UTILS_FREEIF(store_mgr_ptr->mem_mgr_ptr, attach_uri_ptr[num]);
   }
   
//   EMA_UTILS_FREEIF(store_mgr_ptr->mem_mgr_ptr, file_name_ptr);
   EMA_UTILS_FREE(store_mgr_ptr->mem_mgr_ptr, buffer_ptr);
   EMA_UTILS_FREE(store_mgr_ptr->mem_mgr_ptr, source_buffer_ptr);
   
   SFS_CloseFile(file);
   
   if(PNULL != qee_msg_part_ptr)
   {
       HEMSMSGPART_Delete(qee_msg_part_ptr);
   }
   
   if(PNULL != qee_msg_ptr)
   {
       HEMSMSG_Delete(qee_msg_ptr);
   }
   return result;
}

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
                                                 )
{
    EMAIL_STOREMGR_T         *store_mgr_ptr = (EMAIL_STOREMGR_T*)store_handle_ptr;
    HEMSStore        qee_store_ptr = PNULL;
    HEMSMsg          qee_msg_ptr = PNULL;
    int32            ret = EMAIL_ERROR;
    EMSMsgOpt        qee_mo_opt = {0};
    HConfigMgr       cfg_mgr_ptr = PNULL;
    
    // check input param
    if (PNULL == store_mgr_ptr || PNULL == store_mgr_ptr->parent_handle_ptr || PNULL == store_mgr_ptr->qee_store_ptr/* || PNULL == store_mgr_ptr->piShell*/)
    {
        EMA_UTILS_LOG_ERROR(("~~~~~~~~~MMIEMAIL_SP_Store_GetBoxIdByMsgId: Bad paramter!~~~~~~~~~~~~~~~"));
        return EMAIL_BADPARAM;
    }
    
    do 
    {
        if (EMA_SP_ACTIVE_ACCOUNT == act_id )
        {
            cfg_mgr_ptr = MMIEMAIL_SP_GetCfgHandle(store_mgr_ptr->parent_handle_ptr);
            if (PNULL == cfg_mgr_ptr)
            {
                EMA_UTILS_LOG_ERROR(("~~~~~~~~~MMIEMAIL_SP_Store_GetBoxIdByMsgId: get cfgmgr failed!~~~~~~~~~~~~~"));
                return EMAIL_ERROR;
            }
            ret = MMIEMAIL_SP_CFG_GetActiveAct(cfg_mgr_ptr,&act_id);
            if (EMAIL_SUCCESS != ret)
            {
                EMA_UTILS_LOG_ERROR(("MMIEMAIL_SP_Store_GetBoxIdByEmailId: MMIEMAIL_SP_CFG_GetActiveAct failed!~"));
                return EMAIL_ERROR;
            }
        }
        
        qee_store_ptr = store_mgr_ptr->qee_store_ptr;
        
        ret = HEMSSTORE_GetMsg(store_mgr_ptr->qee_store_ptr, email_id, &qee_msg_ptr);
        EMA_UTILS_BREAKIF(ret, "MMIEMAIL_SP_Store_GetBoxIdByEmailId: IVECTORMODEL_GetAt failed!");
        
        ret = HEMSMSG_GetOpt(qee_msg_ptr, EMS_MSGOPT_BOXID, 0, &qee_mo_opt);
        EMA_UTILS_BREAKIF(ret, "MMIEMAIL_SP_Store_GetBoxIdByEmailId,HEMSMSG_GetOpt failed!");
        *box_id_ptr = (uint32)qee_mo_opt.pVal;
        
    }while (0);
    
    if(PNULL != qee_msg_ptr)
    {
        HEMSMSG_Delete(qee_msg_ptr);
    }
    
    return ret;
}

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
                                                 )
{
    EMAIL_STOREMGR_T *store_mgr_ptr = (EMAIL_STOREMGR_T*)store_handle_ptr;
    int32     ret = EMAIL_ERROR;
    unsigned int    num = 0;
    EMS_ServerConf *temp_ptr= PNULL;
    
    if (PNULL == store_mgr_ptr)
    {
        EMA_UTILS_LOG_ERROR(("~~~~~~~~~MMIEMAIL_SP_Store_GetDefaultAccount: Bad paramter!~~~~~~~~"));
        return EMAIL_BADPARAM;
    }
    
    do 
    {
        ret = HEMSSTORE_GetServerConf(store_mgr_ptr->qee_store_ptr, &num, PNULL);
        EMA_UTILS_BREAKIF(ret, "MMIEMAIL_SP_Store_GetDefaultAccount: get server conf failed");
        
        if (num > 0)
        {
            temp_ptr = EMA_UTILS_MALLOC(PNULL, sizeof(EMS_ServerConf) * num);
            EMA_UTILS_BREAKNULL(temp_ptr, ret, "SP_Store_CreateAccount: get server conf failed");
            EMA_UTILS_MEMSET(temp_ptr, 0x00, sizeof(EMS_ServerConf) * num);
            
            ret = HEMSSTORE_GetServerConf(store_mgr_ptr->qee_store_ptr, &num, temp_ptr);
            EMA_UTILS_BREAKIF(ret, "MMIEMAIL_SP_Store_GetDefaultAccount: get server conf failed");
        }
        
    } while(0);
    
    *num_ptr = num;
    *serv_con_info_pptr = temp_ptr;
    return ret;
}

/**************************************************************************************/
// Description : email sp store, factory reset.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC void MMIEMAIL_SP_Store_FactoryReset(
                                           HStoreMgr      store_handle_ptr
                                           )
{
    EMAIL_STOREMGR_T *store_mgr_ptr = (EMAIL_STOREMGR_T*)store_handle_ptr;
    
    if (PNULL == store_mgr_ptr)
    {
        EMA_UTILS_LOG_ERROR(("~~~~~~~~~MMIEMAIL_SP_Store_FactoryReset: Bad paramter!~~~~~~~~"));
        return;
    }
    
    HEMSSTORE_FactoryReset(store_mgr_ptr->qee_store_ptr);
    g_email_is_engine_task_busy = TRUE;
    SCI_TRACE_LOW("mmiemail_sp_stroe.c set g_email_is_engine_task_busy TRUE, MMIEMAIL_SP_Store_FactoryReset");
}

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
                                         )
{
    EMAIL_STOREMGR_T  *store_mgr_ptr = (EMAIL_STOREMGR_T*)store_handle_ptr;
    HConfigMgr cfg_mgr_ptr = PNULL;
    int32      result = EMAIL_ERROR;
    
    HEMSMsg    piQMEMsg = PNULL;
    uint32     cnt = 0;
    uint32     i = 0;
    int        ret = EMAIL_SUCCESS;
    
    if (PNULL == store_mgr_ptr || PNULL == store_mgr_ptr->qee_store_ptr)
    {
        EMA_UTILS_LOG_ERROR(("~~~~~~~~~MMIEMAIL_SP_Store_RemoveParts: Bad paramter!~~~~~~~~~~~~~~~"));
        return EMAIL_BADPARAM;
    }
    
    if (EMA_SP_ACTIVE_ACCOUNT == act_id )
    {
        cfg_mgr_ptr = MMIEMAIL_SP_GetCfgHandle(store_mgr_ptr->parent_handle_ptr);
        if (PNULL == cfg_mgr_ptr)
        {
            EMA_UTILS_LOG_ERROR(("~~~~~~~~~MMIEMAIL_SP_Store_RemoveParts: get cfgmgr failed!~~~~~~~~~~~~~"));
            return EMAIL_ERROR;
        }
        result = MMIEMAIL_SP_CFG_GetActiveAct(cfg_mgr_ptr,&act_id);
        if (result != EMAIL_SUCCESS)
        {
            EMA_UTILS_LOG_ERROR(("MMIEMAIL_SP_Store_RemoveParts: MMIEMAIL_SP_CFG_GetActiveAct failed!~"));
            return EMAIL_ERROR;
        }
    }
    
    do 
    {
        ret = HEMSSTORE_GetMsg(store_mgr_ptr->qee_store_ptr, email_id, &piQMEMsg);
        EMA_UTILS_BREAKIF(ret, "get msg failed");
        
        cnt = HEMSMSG_GetPartCount(piQMEMsg);
        
        for(i = 0; i < cnt; i++)
        {
            HEMSMSG_RemoveMessagePart(piQMEMsg, EMS_MSGPART_ID_POSITION, (void*)i);
        }
        
        ret = HEMSSTORE_UpdateMsg(store_mgr_ptr->qee_store_ptr, piQMEMsg);
        
    } while (0);
    
    if(PNULL != piQMEMsg)
    {
        HEMSMSG_Delete(piQMEMsg);
    }
    
    return ret;
}

/**************************************************************************************/
// Description : email sp store, convent to EME status.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC uint32 MMIEMAIL_SP_Store_ConventToEmeStatus(uint32 eml_status)
{
    uint32 uiStatus = 0;
    
    switch(eml_status)
    {
    case EMA_BASIC_UNREAD:
        uiStatus = EMSMSG_STATUS_WITH_BODY;
        break;
    case EMA_BASIC_UNREAD_HEAD_ONLY:
        uiStatus = 0;
        break;
    case EMA_UNREAD_WITH_ATTACH:
        uiStatus = (EMSMSG_STATUS_WITH_ATTACH | EMSMSG_STATUS_WITH_BODY);
        break;
    case EMA_UNREAD_AND_FORWARDED:
        uiStatus = (EMSMSG_STATUS_WITH_BODY | EMSMSG_STATUS_FORWARDED);
        break;
    case EMA_UNREAD_WITH_ATTACH_AND_FORWARDED:
        uiStatus = (EMSMSG_STATUS_WITH_BODY | EMSMSG_STATUS_WITH_ATTACH | EMSMSG_STATUS_FORWARDED);
        break;
    case EMA_UNREAD_AND_REPLIED:
        uiStatus = (EMSMSG_STATUS_WITH_BODY | EMSMSG_STATUS_REPLIED);
        break;
    case EMA_UNREAD_WITH_ATTACH_AND_REPLIED:
        uiStatus = (EMSMSG_STATUS_WITH_BODY | EMSMSG_STATUS_WITH_ATTACH | EMSMSG_STATUS_REPLIED);
        break;
    case EMA_BASIC_READ:
        uiStatus = (EMSMSG_STATUS_READ | EMSMSG_STATUS_WITH_BODY);
        break;
    case EMA_BASIC_READ_HEAD_ONLY:
        uiStatus = EMSMSG_STATUS_READ;
        break;
    case EMA_READ_WITH_ATTACH:
        uiStatus = (EMSMSG_STATUS_READ | EMSMSG_STATUS_WITH_ATTACH | EMSMSG_STATUS_WITH_BODY);
        break;
    case EMA_READ_AND_FORWARDED:
        uiStatus = (EMSMSG_STATUS_READ | EMSMSG_STATUS_FORWARDED | EMSMSG_STATUS_WITH_BODY);
        break;
    case EMA_READ_WITH_ATTACH_AND_FORWARDED:
        uiStatus = (EMSMSG_STATUS_READ | EMSMSG_STATUS_FORWARDED | EMSMSG_STATUS_WITH_ATTACH | EMSMSG_STATUS_WITH_BODY);
        break;
    case EMA_READ_AND_REPLIED:
        uiStatus = (EMSMSG_STATUS_READ | EMSMSG_STATUS_REPLIED | EMSMSG_STATUS_WITH_BODY);
        break;
    case EMA_READ_WITH_ATTACH_AND_REPLIED:
        uiStatus = (EMSMSG_STATUS_READ | EMSMSG_STATUS_WITH_ATTACH | EMSMSG_STATUS_REPLIED | EMSMSG_STATUS_WITH_BODY);
        break;
    case EMA_SEND_FAILED:
        uiStatus = (EMSMSG_STATUS_SEND_FAILED | EMSMSG_STATUS_WITH_BODY);
        break;
    case EMA_SEND_FAILED_WITH_ATTACH:
        uiStatus = (EMSMSG_STATUS_SEND_FAILED | EMSMSG_STATUS_WITH_ATTACH | EMSMSG_STATUS_WITH_BODY);
        break;
    case EMA_SEND_FAILED_READ:
        uiStatus = (EMSMSG_STATUS_READ | EMSMSG_STATUS_SEND_FAILED | EMSMSG_STATUS_WITH_BODY);
        break;
    case EMA_SEND_FAILED_WITH_ATTACH_READ:
        uiStatus = (EMSMSG_STATUS_READ | EMSMSG_STATUS_SEND_FAILED | EMSMSG_STATUS_WITH_ATTACH | EMSMSG_STATUS_WITH_BODY);
        break;
    case EMA_SENT:
        uiStatus = (EMSMSG_STATUS_SENT | EMSMSG_STATUS_WITH_BODY);
        break;
    case EMA_SENT_WITH_ATTACH:
        uiStatus = (EMSMSG_STATUS_SENT | EMSMSG_STATUS_WITH_ATTACH | EMSMSG_STATUS_WITH_BODY);
        break;
    case EMA_SENT_READ:
        uiStatus = (EMSMSG_STATUS_READ | EMSMSG_STATUS_SENT | EMSMSG_STATUS_WITH_BODY);
        break;
    case EMA_SENT_WITH_ATTACH_READ:
        uiStatus = (EMSMSG_STATUS_READ | EMSMSG_STATUS_SENT | EMSMSG_STATUS_WITH_ATTACH | EMSMSG_STATUS_WITH_BODY);
        break;
    case EMA_BASIC_DRAFT:
        uiStatus = (EMSMSG_STATUS_DRAFT | EMSMSG_STATUS_WITH_BODY);
        break;
    case EMA_BASIC_DRAFT_WITH_ATTACH:
        uiStatus = (EMSMSG_STATUS_DRAFT | EMSMSG_STATUS_WITH_ATTACH | EMSMSG_STATUS_WITH_BODY);
        break;
    case EMA_BASIC_DRAFT_READ:
        uiStatus = (EMSMSG_STATUS_READ | EMSMSG_STATUS_DRAFT | EMSMSG_STATUS_WITH_BODY);
        break;
    case EMA_BASIC_DRAFT_WITH_ATTACH_READ:
        uiStatus = (EMSMSG_STATUS_READ | EMSMSG_STATUS_DRAFT | EMSMSG_STATUS_WITH_ATTACH | EMSMSG_STATUS_WITH_BODY);
        break;
    default:
        uiStatus = 0;
        break;
    }
    
    return uiStatus;
}

/**************************************************************************************/
// Description : email sp store, convent to EMA status.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL uint32 SP_Store_ConventToEmaStatus(uint32 eme_status)
{
    uint32 uiStatus = 0;
    
    if(eme_status & EMSMSG_STATUS_WITH_BODY)
    {
        //read
        if(eme_status & EMSMSG_STATUS_READ)
        {
            if((eme_status & EMSMSG_STATUS_SEND_FAILED) && (eme_status & EMSMSG_STATUS_WITH_ATTACH))
            {
                uiStatus = EMA_SEND_FAILED_WITH_ATTACH_READ;
            }
            else if(eme_status & EMSMSG_STATUS_SEND_FAILED)
            {
                uiStatus = EMA_SEND_FAILED_READ;
            }
            else if((eme_status & EMSMSG_STATUS_SENT) && (eme_status & EMSMSG_STATUS_WITH_ATTACH))
            {
                uiStatus = EMA_SENT_WITH_ATTACH_READ;
            }
            else if(eme_status & EMSMSG_STATUS_SENT)
            {
                uiStatus = EMA_SENT_READ;
            }
            else if((eme_status & EMSMSG_STATUS_DRAFT) && (eme_status & EMSMSG_STATUS_WITH_ATTACH))
            {
                uiStatus = EMA_BASIC_DRAFT_WITH_ATTACH_READ;
            }
            else if(eme_status & EMSMSG_STATUS_DRAFT)
            {
                uiStatus = EMA_BASIC_DRAFT_READ;
            }
            else if((eme_status & EMSMSG_STATUS_WITH_ATTACH) && (eme_status & EMSMSG_STATUS_FORWARDED))
            {
                uiStatus = EMA_READ_WITH_ATTACH_AND_FORWARDED;
            }
            else if((eme_status & EMSMSG_STATUS_WITH_ATTACH) && (eme_status & EMSMSG_STATUS_REPLIED))
            {
                uiStatus = EMA_READ_WITH_ATTACH_AND_REPLIED;
            }
            else if(eme_status & EMSMSG_STATUS_WITH_ATTACH)
            {
                uiStatus = EMA_READ_WITH_ATTACH;
            }
            else if(eme_status & EMSMSG_STATUS_FORWARDED)
            {
                uiStatus = EMA_READ_AND_FORWARDED;
            }
            else if(eme_status & EMSMSG_STATUS_REPLIED)
            {
                uiStatus = EMA_READ_AND_REPLIED;
            }
            else
            {
                uiStatus = EMA_BASIC_READ;
            }
        }
        else
        {
            /*unread*/
            if((eme_status & EMSMSG_STATUS_SEND_FAILED) && (eme_status & EMSMSG_STATUS_WITH_ATTACH))
            {
                uiStatus = EMA_SEND_FAILED_WITH_ATTACH;
            }
            else if(eme_status & EMSMSG_STATUS_SEND_FAILED)
            {
                uiStatus = EMA_SEND_FAILED;
            }
            else if((eme_status & EMSMSG_STATUS_SENT) && (eme_status & EMSMSG_STATUS_WITH_ATTACH))
            {
                uiStatus = EMA_SENT_WITH_ATTACH;
            }
            else if(eme_status & EMSMSG_STATUS_SENT)
            {
                uiStatus = EMA_SENT;
            }
            else if((eme_status & EMSMSG_STATUS_DRAFT) && (eme_status & EMSMSG_STATUS_WITH_ATTACH))
            {
                uiStatus = EMA_BASIC_DRAFT_WITH_ATTACH;
            }
            else if(eme_status & EMSMSG_STATUS_DRAFT)
            {
                uiStatus = EMA_BASIC_DRAFT;
            }
            else if ((eme_status & EMSMSG_STATUS_FORWARDED) && (eme_status & EMSMSG_STATUS_WITH_ATTACH))
            {
                uiStatus = EMA_UNREAD_WITH_ATTACH_AND_FORWARDED;
            }
            else if (eme_status & EMSMSG_STATUS_FORWARDED)
            {
                uiStatus = EMA_UNREAD_AND_FORWARDED;
            }
            else if ((eme_status & EMSMSG_STATUS_REPLIED) && (eme_status & EMSMSG_STATUS_WITH_ATTACH))
            {
                uiStatus = EMA_UNREAD_WITH_ATTACH_AND_REPLIED;
            }
            else if (eme_status & EMSMSG_STATUS_REPLIED)
            {
                uiStatus = EMA_UNREAD_AND_REPLIED;
            }
            else if(eme_status & EMSMSG_STATUS_WITH_ATTACH)
            {
                uiStatus = EMA_UNREAD_WITH_ATTACH;
            }
            else
            {
                uiStatus = EMA_BASIC_UNREAD;
            }
        }
    }
    else
    {
        /*unread*/
        if(eme_status & EMSMSG_STATUS_READ)
        {
            uiStatus = EMA_BASIC_READ_HEAD_ONLY;
        }
        else
        {
            uiStatus = EMA_BASIC_UNREAD_HEAD_ONLY;
        }
    }
    
    return uiStatus;
}

/**************************************************************************************/
// Description : email sp store, update email's header.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC int32 MMIEMAIL_SP_Store_UpdateEmailHeader(
                                                 HStoreMgr store_handle_ptr,
                                                 uint32    email_id
                                                 )
{
    EMAIL_STOREMGR_T    *store_mgr_ptr = (EMAIL_STOREMGR_T*)store_handle_ptr;
    HEMSStore   qee_store_ptr = PNULL;
    HEMSMsg     qee_msg_ptr = PNULL;
    uint32      ret = EMAIL_SUCCESS;
    
    if (PNULL == store_mgr_ptr 
        || PNULL == store_mgr_ptr->parent_handle_ptr 
        || PNULL == store_mgr_ptr->qee_store_ptr)
    {
        EMA_UTILS_LOG_ERROR(("~~~~~~~~~MMIEMAIL_SP_Store_UpdateEmailHeader: Bad paramter!~~~~~~~~~~~~~~~~~"));
        return EMAIL_BADPARAM;
    }
    
    ret = HEMSSTORE_GetMsg(store_mgr_ptr->qee_store_ptr, email_id, &qee_msg_ptr);
    if (EMAIL_SUCCESS != ret || PNULL == qee_msg_ptr)
    {
        return EMAIL_BADPARAM;
    }
    
    qee_store_ptr = store_mgr_ptr->qee_store_ptr;
    
    return HEMSSTORE_UpdateMsgHeader(qee_store_ptr, qee_msg_ptr);
}

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
                                                )
{
    EMAIL_STOREMGR_T    *store_mgr_ptr = (EMAIL_STOREMGR_T*)store_handle_ptr;
    HEMSStore   qee_store_ptr = PNULL;
    uint32      ret = EMAIL_SUCCESS;
    
    if (PNULL == store_mgr_ptr 
        || PNULL == store_mgr_ptr->parent_handle_ptr 
        || PNULL == store_mgr_ptr->qee_store_ptr)
    {
        EMA_UTILS_LOG_ERROR(("~~~~~~~~~MMIEMAIL_SP_Store_UpdateEmailHeader: Bad paramter!~~~~~~~~~~~~~~~~~"));
        return EMAIL_BADPARAM;
    }
    
    qee_store_ptr = store_mgr_ptr->qee_store_ptr;
    
    ret = HEMSSTORE_UpdateMsgState(qee_store_ptr, (StEMSUpdateState*)email_state_ptr, email_num);
    if (EMAIL_SUCCESS == ret)
    {
        g_email_is_engine_task_busy = TRUE;
        SCI_TRACE_LOW("mmiemail_sp_stroe.c set g_email_is_engine_task_busy TRUE, MMIEMAIL_SP_Store_UpdateEmailState");
    }
    return ret;
}


/**************************************************************************************/
// Description : email sp store, update email's msg index.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC void MMIEMAIL_SP_Store_SyncMsgIndexInfo(HStoreMgr store_handle_ptr)
{
    HEMSSTORE_SyncMsgIndexInfo();
    return;
}

/**************************************************************************************/
// Description : email sp store, update email's from addr.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL int32 SP_Store_UpdateFrom(
                                HStoreMgr         store_handle_ptr,
                                HEMSMsg           qee_msg_ptr,
                                HEMSStore         qee_store_ptr,
                                EMAIL_MSGDATACACHE_T   *data_cache_ptr
                                )
{
   EMAIL_STOREMGR_T *store_mgr_ptr   = (EMAIL_STOREMGR_T*)store_handle_ptr;
   HConfigMgr       cfg_mgr_ptr      = PNULL;
   int32            ret              = EMAIL_ERROR;
   uint32           len              = 0;
   EMSMsgOpt        qee_mo_opt[2]    = {0};
   EMASP_PRFITEM_VAL val             = {0};
   uint32           act_id           = EMA_SP_ACTIVE_ACCOUNT;
   if(PNULL !=store_mgr_ptr)
   {
	  qee_store_ptr = store_mgr_ptr->qee_store_ptr;
   }
   
   if (PNULL == store_mgr_ptr  || PNULL == qee_store_ptr || PNULL == data_cache_ptr)
   {
      EMA_UTILS_LOG_ERROR(("~~~~~~~~~SP_Store_UpdateFrom: Bad paramter!~~~~~~~~~~~~~~~~~~"));
      return EMAIL_BADPARAM;
   }

   do 
   {
      if (EMA_SP_ACTIVE_ACCOUNT == act_id)
      {
         cfg_mgr_ptr = MMIEMAIL_SP_GetCfgHandle(store_mgr_ptr->parent_handle_ptr);
         if (PNULL == cfg_mgr_ptr)
         {
            EMA_UTILS_LOG_ERROR(("~~~~~~~~~SP_Store_UpdateFrom: get cfgmgr failed!~~~~~~~~~~~~~"));
            return EMAIL_ERROR;
         }
         ret = MMIEMAIL_SP_CFG_GetActiveAct(cfg_mgr_ptr, &act_id);
         if (EMAIL_SUCCESS != ret)
         {
            EMA_UTILS_LOG_ERROR(("SP_Store_UpdateFrom: MMIEMAIL_SP_CFG_GetActiveAct failed!~"));
            return EMAIL_ERROR;
         }
      }
      
      ret = HEMSSTORE_GetActCfgItem(qee_store_ptr, act_id, EMS_PRFITEM_ACCOUNT_EMAIL, (EMS_PRFITEM_VAL*)&val);
      if (EMAIL_SUCCESS == ret)
      {
         char *str_temp_ptr = PNULL;

         str_temp_ptr = EMA_UTILS_WSTRTOUTF8(PNULL, val.pData);
         /*from address length limited.*/
         len = EMA_UTILS_STRLEN((char*)str_temp_ptr);
         len = (len + 1) < (EMA_MAX_ADDR_LEN + 1) ? (len + 1) : (EMA_MAX_ADDR_LEN + 1);
         data_cache_ptr->email_size += len;
         EMA_UTILS_WSTRLCPY(data_cache_ptr->from_addrs, val.pData, len);
         qee_mo_opt[0].nId = EMS_MSGOPT_FROM;
         qee_mo_opt[0].pVal = str_temp_ptr;
         ret = HEMSMSG_UpdateOpt(qee_msg_ptr, 0, &qee_mo_opt[0]);
         EMA_UTILS_BREAKIF(ret, "Add qee_mo_opt[0] failed!");
         EMA_UTILS_FREEIF(PNULL, str_temp_ptr);
      } 
      else
      {
         ret = EMAIL_SUCCESS;
      }
   } while(0);
   
   return ret;
}
