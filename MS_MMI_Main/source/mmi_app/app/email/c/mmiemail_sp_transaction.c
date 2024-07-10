/***************************************************************************************
** File Name:      mmiemail_sp_transaction.c                                           *
** Author:                                                                             *
** Date:           05/13/2011                                                          *
** Copyright:      2011 Spreadtrum, Incorporated. All Rights Reserved.                 *
** Description:    Head file of E-mail transaction.This File will gather functions that*
**    special handle email transaction. It contains send a email, receive emails,      *
**    view a email and so on. These functions don't be changed by project changed.     *
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
#include "os_api.h"
#include "mmk_type.h"
#include "AEEEMETM.h"
#include "mmi_modu_main.h"
#include "email_text.h"
#include "mmiemail_sp_api.h"
#include "mmiemail_sp_store_api.h"
#include "mmiemail_sp_transaction_api.h"
#include "mmiemail_utils.h"
#include "tasks_id.h"

/**------------------------------------------------------------------------------------*
**                         Macro Declaration                                           *
**------------------------------------------------------------------------------------*/
#define EMA_MAX_INT_STR_LEN         11
#define EMA_ALL_SESSION_OPTS_NUM    27
#define EMA_ENGINE_REPLY_CLSID      P_APP
#define EMA_DOWNLOAD_LIMIT_NUM      25

/**------------------------------------------------------------------------------------*
**                         TYPE AND CONSTANT                                           *
**------------------------------------------------------------------------------------*/
typedef struct email_msgtm_tag
{
    uint32        trans_type;
    uint32        trans_id;
    uint32        email_id;
    struct email_msgtm_tag  *next_ptr;
}EMAIL_MSGTM_T;

typedef struct email_transmgr_handle_tag
{
    IEMETMHandle        *eme_tm_ptr;
    HEmlSPMgr           ema_sp_mgr;
    EMAIL_MSGTM_T       *ema_tm_list_ptr; /* Record transaction manager in the list */
    EMAIL_MEM_HANDLE_T  *mem_ptr;
}EMAIL_TRANSMGR_HANDLE_T;

//typedef struct email_decodeadr_tag
//{
//    wchar* str_to_pos_ptr; /* Record the last address of string which will be decoded*/
//}EMAIL_DECODEADR_T;
extern BOOLEAN  g_email_is_engine_task_busy;
/*-------------------------------------------------------------------------------------*
**                             LOCAL FUNCTION DECLARE                                  *
**------------------------------------------------------------------------------------*/
/**************************************************************************************/
// Description : email sp transaction, create session.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL int32 SP_TRS_CreateSession(
                                 HTransMgr trans_mgr,
                                 HStoreMgr store_mgr,
                                 uint32 act_id
                                 );
/**************************************************************************************/
// Description : email sp transaction, delete transaction list.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL int SP_TRS_DeleteTransList(
                                 HTransMgr trans_mgr, 
                                 EMAIL_MSGTM_T **ema_tm_list_pptr
                                 );
/**************************************************************************************/
// Description : email sp transaction, insert to transaction.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL int SP_TRS_InsertTrans(
                             HTransMgr trans_mgr,
                             EMAIL_MSGTM_T **ema_tm_list_pptr,
                             uint32 trans_type,
                             uint32 trans_id,
                             uint32 email_id
                             );
/**************************************************************************************/
// Description : email sp transaction, notify about sent email.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL int32 SP_TRS_NotifyEmailSent(
                                   HTransMgr trans_mgr,
                                   uint32 email_id,
                                   EMAIL_MSGTM_T **ema_tm_list_pptr
                                   );
/**************************************************************************************/
// Description : email sp transaction, get transaction id by msg id.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL int SP_TRS_GetTransidByEmailId(
                                     const EMAIL_MSGTM_T *ema_tm_list_ptr, 
                                     uint32 email_id,
                                     uint32 *trans_id_ptr
                                     );

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
                                        )
{
    int32           result = EMAIL_ERROR;
    EMAIL_TRANSMGR_HANDLE_T *transmgr_handle_ptr = PNULL;
    
    if (PNULL == trans_mgr_ptr)
    {
        return EMAIL_BADPARAM;
    }
    
    do 
    {
        transmgr_handle_ptr = (EMAIL_TRANSMGR_HANDLE_T*)EMA_UTILS_MALLOC(PNULL,sizeof(EMAIL_TRANSMGR_HANDLE_T));
        EMA_UTILS_BREAKNULL(transmgr_handle_ptr,result,"MMIEMAIL_SP_TRS_InitHandle: Malloc transmgr_handle_ptr failed!");
        EMA_UTILS_MEMSET(transmgr_handle_ptr, 0, sizeof(EMAIL_TRANSMGR_HANDLE_T));
        
        transmgr_handle_ptr->ema_sp_mgr = (HEmlSPMgr)parent_handle_ptr;
        
        EMA_UTILS_TRACE("EEEEEEEEEEEEEEEEEEEEEEE create tm start");
        
        result = AEEEMETM_Init(&transmgr_handle_ptr->eme_tm_ptr);
        EMA_UTILS_BREAKIF(result, "MMIEMAIL_SP_TRS_InitHandle: init eme_tm_ptr failed!");
        EMA_UTILS_TRACE("EEEEEEEEEEEEEEEEEEEEEEE create tm end: %x", transmgr_handle_ptr->eme_tm_ptr);
        
        *trans_mgr_ptr = (HTransMgr)transmgr_handle_ptr;
        
    } while(0);
    
    if (EMAIL_SUCCESS != result)
    {
        MMIEMAIL_SP_TRS_DeleteHandle((HTransMgr*)&transmgr_handle_ptr);
    }
    return result;
}

/**************************************************************************************/
// Description : email sp transaction, to delete handle.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC void MMIEMAIL_SP_TRS_DeleteHandle(HTransMgr *trans_mgr_ptr)
{
    EMAIL_TRANSMGR_HANDLE_T *transmgr_handle_ptr = PNULL;
    
    if (PNULL == trans_mgr_ptr || PNULL == *trans_mgr_ptr)
    {
        return;
    }
    transmgr_handle_ptr = (EMAIL_TRANSMGR_HANDLE_T*)(*trans_mgr_ptr);
    
    if (transmgr_handle_ptr->eme_tm_ptr)
    {
        AEEEMETM_Delete(&transmgr_handle_ptr->eme_tm_ptr);
    }
    
    SP_TRS_DeleteTransList(transmgr_handle_ptr, &transmgr_handle_ptr->ema_tm_list_ptr);
    EMA_UTILS_FREEIF(transmgr_handle_ptr->mem_ptr, transmgr_handle_ptr);
    *trans_mgr_ptr = PNULL;
}

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
                                     )
{
    EMAIL_TRANSMGR_HANDLE_T *transmgr_handle_ptr = (EMAIL_TRANSMGR_HANDLE_T*)trans_mgr; 
    AEEEMEOpt       trans_opt[5] = {0};
    int32           result = EMAIL_ERROR;
    TransID         trans_id = 0;
    HStoreMgr       store_mgr = PNULL;  
    char            ema_msg_id[EMA_MAX_INT_STR_LEN] = {0};
    uint32          index = 0;
    
    if (transmgr_handle_ptr == PNULL)
    {
        return EMAIL_BADPARAM;
    }   
    do 
    {
        store_mgr = MMIEMAIL_SP_GetStoreHandle(transmgr_handle_ptr->ema_sp_mgr);
        result = SP_TRS_CreateSession(trans_mgr, store_mgr, act_id);
        EMA_UTILS_BREAKIF(result, "");
        
        trans_opt[index].iId = EME_OPTID_TRANS_TYPE;
        trans_opt[index].pVal = (void*)EME_TRANS_SMTP_SEND_MAIL;
        index++;
        trans_opt[index].iId = EME_OPTID_MESSAGE_ID;
        EMA_UTILS_SPRINTF(ema_msg_id,"%x",email_id);
        trans_opt[index].pVal = (void*)ema_msg_id; 
        index++;
        trans_opt[index].iId = EME_OPTID_REPLY_CLSID;
        trans_opt[index].pVal = (void*)EMA_ENGINE_REPLY_CLSID;
        index++;
        trans_opt[index].iId = EME_OPTID_CONN_NETID;
        trans_opt[index].pVal = (void*)net_id;
        index++;
        trans_opt[index].iId = EME_OPTID_END;
        result = (int32)AEEEMETM_EnqueueTrans(transmgr_handle_ptr->eme_tm_ptr, trans_opt, &trans_id);
        EMA_UTILS_BREAKIF(result, "");
        
        result = SP_TRS_InsertTrans(trans_mgr,&transmgr_handle_ptr->ema_tm_list_ptr, EME_TRANS_SMTP_SEND_MAIL, trans_id, email_id);
        EMA_UTILS_BREAKIF(result, "");
    } while(0);
    return result;
}

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
                                    )
{
    EMAIL_TRANSMGR_HANDLE_T *transmgr_handle_ptr = (EMAIL_TRANSMGR_HANDLE_T*)trans_mgr; 
    AEEEMEOpt       trans_opt[6] = {0};
    int32           result = EMAIL_ERROR;
    TransID         trans_id = 0;
    HStoreMgr       store_mgr = PNULL;  
    char            *subject_ptr = PNULL;
    char            *rr_text_ptr = PNULL;
    char            ema_msg_id[EMA_MAX_INT_STR_LEN] = {0};
    MMI_STRING_T    size_string1 = {0};
    MMI_STRING_T    size_string2 = {0};
    uint32          index = 0;
    
    
    if (PNULL == transmgr_handle_ptr)
    {
        return EMAIL_BADPARAM;
    }
    do
    {
        store_mgr = MMIEMAIL_SP_GetStoreHandle(transmgr_handle_ptr->ema_sp_mgr);
        result = SP_TRS_CreateSession(trans_mgr,store_mgr,act_id);
        EMA_UTILS_BREAKIF(result, "");
        
        MMI_GetLabelTextByLang(TXT_EMAIL_RR_SUBJECT, &size_string1);
        EMA_UTILS_BREAKNULL(size_string1.wstr_ptr, result, "MMIEMAIL_SP_TRS_SendRR, load string failed");
        subject_ptr = EMA_UTILS_WSTRTOUTF8(transmgr_handle_ptr->mem_ptr, size_string1.wstr_ptr);
        EMA_UTILS_BREAKNULL(subject_ptr, result, "MMIEMAIL_SP_TRS_SendRR, load string failed");
        
        MMI_GetLabelTextByLang(TXT_EMAIL_RR_TEXT, &size_string2);
        EMA_UTILS_BREAKNULL(size_string2.wstr_ptr, result, "MMIEMAIL_SP_TRS_SendRR, load string failed");
        rr_text_ptr = EMA_UTILS_WSTRTOUTF8(transmgr_handle_ptr->mem_ptr, size_string2.wstr_ptr);
        EMA_UTILS_BREAKNULL(rr_text_ptr, result, "MMIEMAIL_SP_TRS_SendRR, load string failed");
        
        trans_opt[index].iId = EME_OPTID_TRANS_TYPE;
        trans_opt[index].pVal = (void*)EME_TRANS_SMTP_SEND_RECEIPT;
        index++;
        trans_opt[index].iId = EME_OPTID_MESSAGE_ID;
        EMA_UTILS_SPRINTF(ema_msg_id,"%x",email_id);
        trans_opt[index].pVal = (void*)ema_msg_id; 
        index++;
        trans_opt[index].iId = EME_OPTID_REPLY_CLSID;
        trans_opt[index].pVal = (void*)PNULL;
        index++;
        trans_opt[index].iId = EME_OPTID_CONN_NETID;
        trans_opt[index].pVal = (void*)net_id;      
        index++;
        trans_opt[index].iId = EME_OPTID_END;
        result = (int32)AEEEMETM_EnqueueTrans(transmgr_handle_ptr->eme_tm_ptr, trans_opt, &trans_id);
        EMA_UTILS_BREAKIF(result, "");
        
        result = SP_TRS_InsertTrans(trans_mgr,&transmgr_handle_ptr->ema_tm_list_ptr, EME_TRANS_SMTP_SEND_RECEIPT, trans_id, email_id);
        EMA_UTILS_BREAKIF(result, "");
        
    } while(0);
    
    EMA_UTILS_FREEIF(transmgr_handle_ptr->mem_ptr, subject_ptr);
    EMA_UTILS_FREEIF(transmgr_handle_ptr->mem_ptr, rr_text_ptr);
    
    return result;
}

/**************************************************************************************/
// Description : email sp transaction, cancel current transaction.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC int32 MMIEMAIL_SP_TRS_CancelCurrentTrans(
                                                HTransMgr trans_mgr
                                                )
{
    int32 result = EMAIL_ERROR;
    EMAIL_TRANSMGR_HANDLE_T *transmgr_handle_ptr = (EMAIL_TRANSMGR_HANDLE_T*)trans_mgr;
    
    if (PNULL == transmgr_handle_ptr || PNULL == transmgr_handle_ptr->eme_tm_ptr)
    {
        return EMAIL_BADPARAM;
    }
    do 
    {
        result = AEEEMETM_DequeueTrans(transmgr_handle_ptr->eme_tm_ptr, EME_PROGRESS_INDI_MASK);
        EMA_UTILS_BREAKIF(result,"MMIEMAIL_SP_TRS_CancelCurrentTrans:_DequeueTrans failed!");
    } while(0);
    
    return result;
}

/**************************************************************************************/
// Description : email sp transaction, get transaction id by msg id.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL int SP_TRS_GetTransidByEmailId(
                                     const EMAIL_MSGTM_T *ema_tm_list_ptr, 
                                     uint32 email_id,
                                     uint32 *trans_id_ptr
                                     )
{
    const EMAIL_MSGTM_T* tm_list_ptr = ema_tm_list_ptr;
    
    if (PNULL == tm_list_ptr || PNULL == trans_id_ptr)
    {
        EMA_UTILS_LOG_ERROR(("SP_TRS_GetTransidByEmailId, param error"));
        return EMAIL_BADPARAM;
    }   
    *trans_id_ptr = 0;
    while (PNULL != tm_list_ptr)
    {
        if (tm_list_ptr->email_id == email_id)
        {
            *trans_id_ptr = tm_list_ptr->trans_id;
            break;
        }
        tm_list_ptr = tm_list_ptr->next_ptr;
    }
    if (0 == *trans_id_ptr)
    {
        return EMAIL_ERROR;
    }
    else
    {
        return EMAIL_SUCCESS;
    }
}

/**************************************************************************************/
// Description : email sp transaction, insert to transaction.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL int SP_TRS_InsertTrans(
                             HTransMgr trans_mgr,
                             EMAIL_MSGTM_T **ema_tm_list_pptr, 
                             uint32 trans_type,
                             uint32 trans_id,
                             uint32 email_id
                             )
{
    int result = EMAIL_SUCCESS;
    EMAIL_MSGTM_T *tm_list_ptr = PNULL;
    EMAIL_TRANSMGR_HANDLE_T *transmgr_handle_ptr = (EMAIL_TRANSMGR_HANDLE_T*)trans_mgr;
    
    if (PNULL == transmgr_handle_ptr || PNULL == ema_tm_list_pptr)
    {
        EMA_UTILS_LOG_ERROR(("EmailATransMgr_InsertTMList, param error"));
        return EMAIL_BADPARAM;
    }
    
    do 
    {
        tm_list_ptr = EMA_UTILS_MALLOC(transmgr_handle_ptr->mem_ptr,sizeof(EMAIL_MSGTM_T));
        EMA_UTILS_BREAKNULL(tm_list_ptr,
            result,
            "SP_TRS_InsertTrans, MALLOC tm_list_ptr error!");
        EMA_UTILS_MEMSET(tm_list_ptr, 0, sizeof(EMAIL_MSGTM_T));
        tm_list_ptr->trans_type = trans_type;
        tm_list_ptr->trans_id = trans_id;
        tm_list_ptr->email_id = email_id;
        
        if (PNULL == *ema_tm_list_pptr)
        {
            *ema_tm_list_pptr = tm_list_ptr;  
        }
        else
        {
            tm_list_ptr->next_ptr = *ema_tm_list_pptr;
            *ema_tm_list_pptr = tm_list_ptr;
        }
        
        g_email_is_engine_task_busy = TRUE;
        SCI_TRACE_LOW("mmiemail_sp_transaction.c set g_email_is_engine_task_busy TRUE, SP_TRS_InsertTrans");
    } while(0);
    
    return result;
}

/**************************************************************************************/
// Description : email sp transaction, free transaction.
// Global resource dependence : 
// Author: 
// Note: Delete message id and transaction id to maintained transaction list.
/**************************************************************************************/
PUBLIC int MMIEMAIL_SP_TRS_DeleteTrans(
                                       HTransMgr trans_mgr,
                                       uint32 trans_id,
                                       uint32 email_id
                                       )
{
    int result = EMAIL_ERROR;
    EMAIL_MSGTM_T *tm_list_ptr = PNULL;
    EMAIL_MSGTM_T *tm_list_prev_ptr = PNULL;
    EMAIL_MSGTM_T *tm_list_free_ptr = PNULL;
    EMAIL_TRANSMGR_HANDLE_T *transmgr_handle_ptr = (EMAIL_TRANSMGR_HANDLE_T*)trans_mgr;
    
    if (PNULL == transmgr_handle_ptr)
    {
        return EMAIL_BADPARAM;
    }
    
    tm_list_ptr = transmgr_handle_ptr->ema_tm_list_ptr;
    
    while (PNULL != tm_list_ptr)
    {
        if (tm_list_ptr->trans_id == trans_id
            || (tm_list_ptr->email_id == email_id && 0 != email_id))
        {
            result = EMAIL_SUCCESS;
            tm_list_free_ptr = tm_list_ptr;
            if (PNULL == tm_list_prev_ptr)
            {
                tm_list_ptr = tm_list_ptr->next_ptr;
                EMA_UTILS_FREEIF(transmgr_handle_ptr->mem_ptr, tm_list_free_ptr);/*lint !e774*/
                transmgr_handle_ptr->ema_tm_list_ptr = tm_list_ptr;
                break;
            }
            else
            {
                tm_list_prev_ptr->next_ptr = tm_list_ptr->next_ptr;
                EMA_UTILS_FREEIF(transmgr_handle_ptr->mem_ptr, tm_list_free_ptr);/*lint !e774*/
                break;
            }
        }
        tm_list_prev_ptr = tm_list_ptr;
        tm_list_ptr = tm_list_ptr->next_ptr;
    }
    
    return result;
}

/**************************************************************************************/
// Description : email sp transaction, delete transaction list.
// Global resource dependence : 
// Author: 
// Note: Delete message id and transaction id to maintained transaction list.
/**************************************************************************************/
LOCAL int SP_TRS_DeleteTransList(
                                 HTransMgr trans_mgr,
                                 EMAIL_MSGTM_T **ema_tm_list_pptr
                                 )
{
    int result = EMAIL_SUCCESS;
    EMAIL_MSGTM_T *tm_list_ptr = PNULL;
    EMAIL_TRANSMGR_HANDLE_T *transmgr_handle_ptr = (EMAIL_TRANSMGR_HANDLE_T*)trans_mgr;
    EMAIL_MSGTM_T *tm_list_prior_ptr = PNULL;
    
    EMA_UTILS_LOG_INFO(("SP_TRS_DeleteTransList Begin"));
    
    if (PNULL == transmgr_handle_ptr || PNULL == ema_tm_list_pptr || PNULL == *ema_tm_list_pptr)
    {
        EMA_UTILS_LOG_ERROR(("SP_TRS_DeleteTransList, param error"));
        return EMAIL_BADPARAM;
    }
    do 
    {
        tm_list_prior_ptr = *ema_tm_list_pptr;
        tm_list_ptr = tm_list_prior_ptr;
        while (PNULL != tm_list_ptr)
        {
            tm_list_prior_ptr = tm_list_ptr->next_ptr;
            EMA_UTILS_FREEIF(transmgr_handle_ptr->mem_ptr,tm_list_ptr);/*lint !e774*/
            
            tm_list_ptr = tm_list_prior_ptr;
        }
    } while(0);
    
    return result;
}

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
                                               )
{
    EMAIL_TRANSMGR_HANDLE_T *transmgr_handle_ptr = (EMAIL_TRANSMGR_HANDLE_T*)trans_mgr;
    
    EMAIL_MSGTM_T* tm_list_ptr = PNULL;
    
    if (PNULL == transmgr_handle_ptr || PNULL == email_id_ptr)
    {
        EMA_UTILS_LOG_ERROR(("MMIEMAIL_SP_TRS_GetEmailidByTransId, param error"));
        return EMAIL_BADPARAM; 
    }   
    tm_list_ptr = transmgr_handle_ptr->ema_tm_list_ptr;
    *email_id_ptr = 0;
    while (PNULL != tm_list_ptr)
    {
        if (tm_list_ptr->trans_id == trans_id)
        {
            *email_id_ptr = tm_list_ptr->email_id;
            break;
        }
        tm_list_ptr = tm_list_ptr->next_ptr;
    }
    if (0 == *email_id_ptr)
    {
        return EMAIL_ERROR;
    }
    else
    {
        return EMAIL_SUCCESS;
    }
}

/**************************************************************************************/
// Description : email sp transaction, check is busy or not.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC int MMIEMAIL_SP_TRS_IsBusy(
                                  HTransMgr trans_mgr,
                                  BOOLEAN* is_busy_ptr,
                                  uint32 *trans_state_ptr
                                  )
{
    EMAIL_TRANSMGR_HANDLE_T *transmgr_handle_ptr = (EMAIL_TRANSMGR_HANDLE_T*)trans_mgr;
    
    if (PNULL == transmgr_handle_ptr || PNULL == is_busy_ptr)
    {
        EMA_UTILS_LOG_ERROR(("MMIEMAIL_SP_TRS_IsBusy, param error"));
        return EMAIL_BADPARAM; 
    }   
    if (PNULL != transmgr_handle_ptr->ema_tm_list_ptr)
    {
        *is_busy_ptr = TRUE;
        *trans_state_ptr = transmgr_handle_ptr->ema_tm_list_ptr->trans_type;
    }
    else
    {
        *is_busy_ptr = FALSE;
        *trans_state_ptr = 0;
    }
    return EMAIL_SUCCESS;
}

/**************************************************************************************/
// Description : email sp transaction, notify about sent email.
// Global resource dependence : 
// Author: 
// Note: Maintain transaction state after a message is sent.
/**************************************************************************************/
LOCAL int32 SP_TRS_NotifyEmailSent(
                                   HTransMgr trans_mgr,
                                   uint32 email_id,
                                   EMAIL_MSGTM_T **ema_tm_list_pptr
                                   )
{
    int            result = EMAIL_SUCCESS;
    EMAIL_TRANSMGR_HANDLE_T *transmgr_handle_ptr = (EMAIL_TRANSMGR_HANDLE_T*)trans_mgr;
    
    if (PNULL == transmgr_handle_ptr || PNULL == *ema_tm_list_pptr)
    {
        EMA_UTILS_LOG_ERROR(("SP_TRS_NotifyEmailSent, param error"));
        return EMAIL_BADPARAM;
    }
    do 
    {
        /*note ;*/
        result = MMIEMAIL_SP_TRS_DeleteTrans(trans_mgr, 0, email_id);
        EMA_UTILS_BREAKIF(result, "SP_TRS_NotifyEmailSent, Delete transaction list failed!");
        if (PNULL == (*ema_tm_list_pptr)->next_ptr)
        {
            EMA_UTILS_FREEIF(transmgr_handle_ptr->mem_ptr,*ema_tm_list_pptr);
        }
        
    } while(0);
    return result;
}

/**************************************************************************************/
// Description : email sp transaction, create session.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL int32 SP_TRS_CreateSession(
                                 HTransMgr trans_mgr,
                                 HStoreMgr store_mgr,
                                 uint32 act_id
                                 )
{
    AEEEMEOpt eme_opt[EMA_ALL_SESSION_OPTS_NUM] = {0};
    int32 result = EMAIL_ERROR;
    uint32 i = 0;
    EMASP_PRFITEM_VAL stval  = {0};
    char *size_limit_ptr     = PNULL;
    //char *num_limit_ptr      = PNULL;
    char *recserv_addr_ptr   = PNULL;
    char *recserv_port_ptr   = PNULL;
    char *recuser_name_ptr   = PNULL;
    char *rec_password_ptr   = PNULL;
    char *sendserv_addr_ptr  = PNULL;
    char *sendserv_port_ptr  = PNULL;
    char *senduser_name_ptr  = PNULL;
    char *send_password_ptr  = PNULL;
    char *send_emailaddr_ptr = PNULL;
    char *pcuidl_ptr = PNULL;
    char *pcuid_ptr = PNULL;
    uint32 dwtemp = 0;
    BOOLEAN is_pop3 = FALSE;
    uint32 wstr_len = 0;
    EMAIL_TRANSMGR_HANDLE_T *transmgr_handle_ptr = (EMAIL_TRANSMGR_HANDLE_T*)trans_mgr;
    HConfigMgr config_mgr = PNULL;
    
    /*0405*/
    uint32 down_server = 0;
    uint32 down_port = 0;
    
    if (PNULL == transmgr_handle_ptr || PNULL == transmgr_handle_ptr->ema_sp_mgr \
        || PNULL == transmgr_handle_ptr->eme_tm_ptr || PNULL == store_mgr)
    {
        return EMAIL_BADPARAM;
    }
    
    do 
    {
        /* account information */
        /*
        || EMLENG_OPTID_ACCOUNT_ID          //value type: uint32            
        || EMLENG_OPTID_DEFAULT_FOLDER_ID   //value type: uint32 
        || EMLENG_OPTID_PROTOCOL_TYPE       //value type: PROTOCOL TYPE  
        || EMLENG_OPTID_IS_KEEPMAIL         //value type: Boolean
        || EMLENG_OPTID_SIZE_LIMIT          //value type: uint32
        || EMLENG_OPTID_NUM_LIMIT           //value type: uint32 
        || EMLENG_OPTID_RECEIVE_SERVER_ADDR //value type: char* 
        || EMLENG_OPTID_RECEIVE_SERVER_PORT //value type: uint32 
        || EMLENG_OPTID_RECEIVE_USERNAME    //value type: char* 
        || EMLENG_OPTID_RECEIVE_PASSWORD    //value type: char* 
        || EMLENG_OPTID_SEND_SERVER_ADDR    //value type: char*             
        || EMLENG_OPTID_SEND_SERVER_PORT    //value type: uint32         
        || EMLENG_OPTID_SEND_USERNAME       //value type: char*            
        || EMLENG_OPTID_SEND_PASSWORD       //value type: char*            
        || EMLENG_OPTID_UIDL_FILE_POP       //value type: char*           
        || EMLENG_OPTID_UID_FILE_IMAP       //value type: char*
        || EMLENG_OPTID_END                 //option end mark 
        */
        
        if (EMA_SP_ACTIVE_ACCOUNT == act_id)
        {
            config_mgr = MMIEMAIL_SP_GetCfgHandle(transmgr_handle_ptr->ema_sp_mgr);
            if (PNULL == config_mgr)
            {
                EMA_UTILS_LOG_ERROR(("~~~~~~~~~SP_TRS_CreateSession: get cfgmgr failed!~~~~~~~~~~~~~"));
                return EMAIL_ERROR;
            }
            result = MMIEMAIL_SP_CFG_GetActiveAct(config_mgr, &act_id);
            if (EMAIL_SUCCESS != result)
            {
                EMA_UTILS_LOG_ERROR(("SP_TRS_CreateSession: MMIEMAIL_SP_CFG_GetActiveAct failed!~"));
                return EMAIL_ERROR;
            }
        }
        
        /* Set EMOpt array*/
        /*EMLENG_OPTID_ACCOUNT_ID : uint32*/
        eme_opt[i].iId = EME_OPTID_ACCOUNT_ID;
        eme_opt[i].pVal = (void*)act_id;
        i++;

        /* EMLENG_OPTID_DEFAULT_FOLDER_ID : uint32 */
        result = MMIEMAIL_SP_Store_GetActCfgItem(store_mgr,
            act_id,
            EMASP_PRFITEM_EMAIL_DEFAULTINBOXID,
            &stval);
        EMA_UTILS_BREAKIF(result, "");
        eme_opt[i].pVal = (void*)stval.pData;
        eme_opt[i].iId = EME_OPTID_DEFAULT_FOLDER_ID;
        i++;
        
        /* EMLENG_OPTID_PROTOCOL_TYPE : EMProtocolTye */
        result = MMIEMAIL_SP_Store_GetActCfgItem(store_mgr,
            act_id,
            EMASP_PRFITEM_RECEIVE_SERVER_TYPE,
            &stval);
        EMA_UTILS_BREAKIF(result, "");
        dwtemp = (uint32)stval.pData;
        if (EMASP_POP3 == dwtemp)
        {
            //          eme_opt[2].pVal = (void*)EME_PROTOCOL_TYPE_POP;
            is_pop3 = TRUE;
        }
        else if (EMASP_IMAP4 == dwtemp)
        {
            //          eme_opt[2].pVal = (void*)EME_PROTOCOL_TYPE_IMAP;
            is_pop3 = FALSE;
        }
        //     eme_opt[2].iId = EME_OPTID_PROTOCOL_TYPE;
        
        /* EME_OPTID_IS_KEEPMAIL : BOOLEAN*/ 
        result = MMIEMAIL_SP_Store_GetActCfgItem(store_mgr,
            act_id,
            EMASP_PRFITEM_RECEIVER_LEAVE_COPY,
            &stval);
        EMA_UTILS_BREAKIF(result, "");
        eme_opt[i].pVal = (void*)stval.pData;
        eme_opt[i].iId = EME_OPTID_POP_KEEP_MAIL;
        i++;
        
        /* EME_OPTID_SIZE_LIMIT : uint32 */ 
        result = MMIEMAIL_SP_Store_GetActCfgItem(store_mgr,
            act_id,
            EMASP_PRFITEM_MAX_MSG_SIZE,
            &stval); /* have question???*/
        EMA_UTILS_BREAKIF(result, "");
        
        size_limit_ptr = EMA_UTILS_WSTRTOSTR(PNULL, (wchar*)stval.pData);     
        //eme_opt[i].pVal = (void*)5000;//(void*)(EMA_UTILS_ATOI(size_limit_ptr) * 4 /3);  // the unit is K
        eme_opt[i].pVal = (void*)(EMA_UTILS_ATOI((unsigned char*)size_limit_ptr));
        eme_opt[i].iId = EME_OPTID_SIZE_LIMIT;
        i++;
        
        /* EME_OPTID_NUM_LIMIT : uint32 */
        result = MMIEMAIL_SP_Store_GetActCfgItem(store_mgr,
            act_id,
            EMASP_PRFITEM_NUMBER_LIMIT,
            &stval); /* have question???*/
        EMA_UTILS_BREAKIF(result, "");
        //num_limit_ptr = EMA_UTILS_WSTRTOSTR(PNULL, (wchar*)stval.pData);     
        if (0 == (uint32)stval.pData || 1 == (uint32)stval.pData)
        {
            eme_opt[i].pVal = (void*)(((uint32)stval.pData + 1) * EMA_DOWNLOAD_LIMIT_NUM);  
        }
        else if(2 == (uint32)stval.pData)
        {
            eme_opt[i].pVal = (void*)(((uint32)stval.pData + 2) * EMA_DOWNLOAD_LIMIT_NUM);  
        }
        else
        {  
            eme_opt[i].pVal = (void*)0x0FFFFFFF;  
        }
        eme_opt[i].iId = EME_OPTID_NUM_LIMIT;
        i++;
        
        //EMA_UTILS_FREEIF(transmgr_handle_ptr->mem_ptr,stval.pData);
        
        if (is_pop3)/* set property when POP3*/
        {
            down_server = EMASP_PRFITEM_POP3_SERVER;
            down_port = EMASP_PRFITEM_POP3_PORT;
        }
        else/* set property when IMAP4*/
        {
            down_server = EMASP_PRFITEM_IMAP4_SERVER;
            down_port = EMASP_PRFITEM_IMAP4_SERVER_PORT;
        }
        /* EME_OPTID_RECEIVE_SERVER_ADDR : char**/ 
        result = MMIEMAIL_SP_Store_GetActCfgItem(store_mgr,
            act_id,
            down_server,
            &stval);
        EMA_UTILS_BREAKIF(result, "");
        
        recserv_addr_ptr = EMA_UTILS_WSTRTOSTR(PNULL, (wchar*)stval.pData);
        eme_opt[i].pVal = (void*)recserv_addr_ptr;
        eme_opt[i].iId = EME_OPTID_RECEIVE_SERVER_ADDR;
        i++;
        //EMA_UTILS_FREEIF(transmgr_handle_ptr->mem_ptr,stval.pData);
        
        /* EME_OPTID_RECEIVE_SERVER_PORT : uint32 */ 
        result = MMIEMAIL_SP_Store_GetActCfgItem(store_mgr,
            act_id,
            down_port,
            &stval);
        EMA_UTILS_BREAKIF(result, "");
        
        recserv_port_ptr = EMA_UTILS_WSTRTOSTR(PNULL, (wchar*)stval.pData);
        eme_opt[i].pVal = (void*)EMA_UTILS_ATOI((unsigned char*)recserv_port_ptr);
        eme_opt[i].iId = EME_OPTID_RECEIVE_SERVER_PORT;
        i++;
        //EMA_UTILS_FREEIF(transmgr_handle_ptr->mem_ptr,stval.pData);
        
        
        /* EME_OPTID_RECEIVE_USERNAME : char* */ 
        result = MMIEMAIL_SP_Store_GetActCfgItem(store_mgr,
            act_id,
            //EMASP_PRFITEM_EMAIL_USER_ID,
            EMASP_PRFITEM_ACCOUNT_EMAIL, //for spredtrum
            &stval);
        EMA_UTILS_BREAKIF(result, "");
        
        recuser_name_ptr = EMA_UTILS_WSTRTOSTR(PNULL, (wchar*)stval.pData);
        eme_opt[i].pVal = (void*)recuser_name_ptr;
        eme_opt[i].iId = EME_OPTID_RECEIVE_USERNAME;
        i++;
        //EMA_UTILS_FREEIF(transmgr_handle_ptr->mem_ptr,stval.pData);
        
        /* EM_RECEIVE_PASSWORD : char* */ 
        result = MMIEMAIL_SP_Store_GetActCfgItem(store_mgr,
            act_id,
            EMASP_PRFITEM_EMAIL_PASSWORD,
            &stval);
        EMA_UTILS_BREAKIF(result, "");
        
        rec_password_ptr = EMA_UTILS_WSTRTOSTR(PNULL, (wchar*)stval.pData);
        eme_opt[i].pVal = (void*)rec_password_ptr;
        eme_opt[i].iId = EME_OPTID_RECEIVE_PASSWORD;
        i++;
        //EMA_UTILS_FREEIF(transmgr_handle_ptr->mem_ptr,stval.pData);
        
        /* EM_SEND_SERVER_ADDR : char* */ 
        result = MMIEMAIL_SP_Store_GetActCfgItem(store_mgr,
            act_id,
            EMASP_PRFITEM_SMTP_SERVER,
            &stval);
        EMA_UTILS_BREAKIF(result, "");
        
        sendserv_addr_ptr = EMA_UTILS_WSTRTOSTR(PNULL, (wchar*)stval.pData);
        eme_opt[i].pVal = (void*)sendserv_addr_ptr;
        eme_opt[i].iId = EME_OPTID_SEND_SERVER_ADDR;
        i++;
        //EMA_UTILS_FREEIF(transmgr_handle_ptr->mem_ptr,stval.pData);
        
        /* EME_OPTID_SEND_SERVER_PORT : uint32 */ 
        result = MMIEMAIL_SP_Store_GetActCfgItem(store_mgr,
            act_id,
            EMASP_PRFITEM_SMTP_SERVER_PORT,
            &stval);
        EMA_UTILS_BREAKIF(result, "");
        
        sendserv_port_ptr = EMA_UTILS_WSTRTOSTR(PNULL, (wchar*)stval.pData);
        eme_opt[i].pVal = (void*)EMA_UTILS_ATOI((unsigned char*)sendserv_port_ptr);
        eme_opt[i].iId = EME_OPTID_SEND_SERVER_PORT;
        i++;
        //EMA_UTILS_FREEIF(transmgr_handle_ptr->mem_ptr,stval.pData);
        
        /* EME_OPTID_SEND_USERNAME : char* */ 
        result = MMIEMAIL_SP_Store_GetActCfgItem(store_mgr,
            act_id,
            EMASP_PRFITEM_SMTP_USER_ID,
            &stval);
        EMA_UTILS_BREAKIF(result, "");
        
        if (PNULL == stval.pData || EMA_UTILS_WSTRLEN(stval.pData) < 1)
        {
            result = MMIEMAIL_SP_Store_GetActCfgItem(store_mgr,
                act_id,
                //EMASP_PRFITEM_EMAIL_USER_ID,
                EMASP_PRFITEM_ACCOUNT_EMAIL, //for spredtrum
                &stval);
            EMA_UTILS_BREAKIF(result, "");
        }
        
        senduser_name_ptr = EMA_UTILS_WSTRTOSTR(PNULL, (wchar*)stval.pData);
        eme_opt[i].pVal = (void*)senduser_name_ptr;
        eme_opt[i].iId = EME_OPTID_SEND_USERNAME;
        i++;
        //EMA_UTILS_FREEIF(transmgr_handle_ptr->mem_ptr,stval.pData);
        
        /* EME_OPTID_SEND_PASSWORD : char* */ 
        result = MMIEMAIL_SP_Store_GetActCfgItem(store_mgr,
            act_id,
            EMASP_PRFITEM_SMTP_PASSWORD,
            &stval);
        EMA_UTILS_BREAKIF(result, "");
        
        if (PNULL == stval.pData || EMA_UTILS_WSTRLEN(stval.pData) < 1)
        {
            result = MMIEMAIL_SP_Store_GetActCfgItem(store_mgr,
                act_id,
                EMASP_PRFITEM_EMAIL_PASSWORD,
                &stval);
            EMA_UTILS_BREAKIF(result, "");
        }
        
        send_password_ptr = EMA_UTILS_WSTRTOSTR(PNULL, (wchar*)stval.pData);
        eme_opt[i].pVal = (void*)send_password_ptr;
        eme_opt[i].iId = EME_OPTID_SEND_PASSWORD;
        i++;
        //EMA_UTILS_FREEIF(transmgr_handle_ptr->mem_ptr,stval.pData);
        
        /* EM_SEND_EMAIL_ADDR : char* */ 
        result = MMIEMAIL_SP_Store_GetActCfgItem(store_mgr,
            act_id,
            EMASP_PRFITEM_ACCOUNT_EMAIL,
            &stval);
        EMA_UTILS_BREAKIF(result, "");
        
        send_emailaddr_ptr = EMA_UTILS_WSTRTOSTR(PNULL, (wchar*)stval.pData);
        eme_opt[i].pVal = (void*)send_emailaddr_ptr;
        eme_opt[i].iId = EME_OPTID_SEND_EMAIL_ADDR;
        i++;
        //EMA_UTILS_FREEIF(transmgr_handle_ptr->mem_ptr,stval.pData);
        
        
        /* EME_OPTID_UIDL_FILE_POP : char* */ 
        result = MMIEMAIL_SP_Store_GetActCfgItem(store_mgr,
            act_id,
            EMASP_PRFILEM_EMAIL_UIDLFILEDIR,
            &stval);
        EMA_UTILS_BREAKIF(result, "");
        pcuidl_ptr = EMA_UTILS_STRDUP(transmgr_handle_ptr->mem_ptr,stval.pData);
        eme_opt[i].pVal = (void*)pcuidl_ptr;   /*must free*/
        eme_opt[i].iId = EME_OPTID_UIDL_FILE_POP;
        i++;
        //EMA_UTILS_FREEIF(transmgr_handle_ptr->mem_ptr,stval.pData);
        
        /* EME_OPTID_UID_FILE_IMAP : char* */
        result = MMIEMAIL_SP_Store_GetActCfgItem(store_mgr,
            act_id,
            EMASP_PRFILEM_EMAIL_UIDFILEDIR,
            &stval);
        EMA_UTILS_BREAKIF(result, "");
        pcuid_ptr = EMA_UTILS_STRDUP(transmgr_handle_ptr->mem_ptr,stval.pData);
        eme_opt[i].pVal = (void*)pcuid_ptr;   /*must free*/
        eme_opt[i].iId = EME_OPTID_UID_FILE_IMAP;
        i++;
        //EMA_UTILS_FREEIF(transmgr_handle_ptr->mem_ptr,stval.pData);
        
        /* EME_OPTID_SMTP_NEED_SSL : char* */
        result = MMIEMAIL_SP_Store_GetActCfgItem(store_mgr,
            act_id,
            EMASP_PRFITEM_SMTP_SSL_OR_TLS,
            &stval);
        EMA_UTILS_BREAKIF(result, "");
        eme_opt[i].pVal = (void*)stval.pData;
        eme_opt[i].iId = EME_OPTID_SMTP_NEED_SSL;
        i++;
        
        /* EME_OPTID_SMTP_NEED_SSL : char* */
        result = MMIEMAIL_SP_Store_GetActCfgItem(store_mgr,
            act_id,
            EMASP_PRFITEM_POP3_SSL_OR_TLS,
            &stval);
        EMA_UTILS_BREAKIF(result, "");
        eme_opt[i].pVal = (void*)stval.pData;
        eme_opt[i].iId = EME_OPTID_POP_NEED_SSL;
        i++;
        
        /* EME_OPTID_SMTP_NEED_SSL : int */
        result = MMIEMAIL_SP_Store_GetActCfgItem(store_mgr,
            act_id,
            EMASP_PRFITEM_IMAP4_SSL_OR_TLS,
            &stval);
        EMA_UTILS_BREAKIF(result, "");
        eme_opt[i].pVal = (void*)stval.pData;
        eme_opt[i].iId = EME_OPTID_IMAP_NEED_SSL;
        i++;
        
        /* EME_OPTID_NETWORK_UMTS_PDP : int */
        result = MMIEMAIL_SP_Store_GetActCfgItem(store_mgr,
            act_id,
            EMASP_PRFITEM_DATA_SERVICE_PROVIDER,
            &stval);
        EMA_UTILS_BREAKIF(result, "");
        eme_opt[i].pVal = (void*)stval.pData;
        eme_opt[i].iId = EME_OPTID_NETWORK_UMTS_PDP;
        i++;

        /* EME_OPTID_SMTP_AUTH */
        result = MMIEMAIL_SP_Store_GetActCfgItem(store_mgr,
            act_id,
            EMASP_PRFITEM_SMTP_AUTHENTICATE,
            &stval);
        EMA_UTILS_BREAKIF(result, "");
        eme_opt[i].pVal = (void*)stval.pData;
        eme_opt[i].iId = EME_OPTID_SMTP_AUTHENTICATE;
        i++;

        /* EME_OPTID_RECEIVE_UNSEEN */
        //       result = MMIEMAIL_SP_Store_GetActCfgItem(store_mgr,
        //          act_id,
        //          EMASP_PRFITEM_RECEIVE_UNSEEN,
        //          &stval);
        //       EMA_UTILS_BREAKIF(result, "");
        //       eme_opt[i].pVal = (void*)stval.pData;
        //       eme_opt[i].iId = EME_OPTID_RECEIVE_UNSEEN;
        //       i++;
        //       result = MMIEMAIL_SP_Store_GetActCfgItem(store_mgr,
        //          act_id,
        //          EMASP_PRFITEM_DOWNLOAD_ATTACH,
        //          &stval);
        //       EMA_UTILS_BREAKIF(result, "");
        //       eme_opt[i].pVal = (void*)stval.pData;
        //       eme_opt[i].iId = EME_OPTID_DOWNLOAD_ATTACH;
        //       i++;
        //       result = SP_CFG_GetActiveProfile(SP_GetCfgHandle(transmgr_handle_ptr->ema_sp_mgr), &profile_id);
        //       eme_opt[i].pVal = (void*)profile_id;
        //       eme_opt[i].iId = EME_OPTID_PROFILE_ID;
        //       i++;
        //       result = SP_CFG_GetCustom_ProfileOpt(SP_GetCfgHandle(transmgr_handle_ptr->ema_sp_mgr), profile_id,&EmailParms);
        //       eme_opt[i].pVal = (void*)EmailParms.strEmailPPPNAI;
        //       eme_opt[i].iId = EME_OPTID_PPP_NAME;
        //       i++;
        //       eme_opt[i].pVal = (void*)EmailParms.strEmailPPPPwd;
        //       eme_opt[i].iId = EME_OPTID_PPP_PDW;
        //       i++;
        
        /* END */
        result = MMIEMAIL_SP_Store_GetActCfgItem(store_mgr,
            act_id,
            EMASP_PRFITEM_RECEIVE_ONLY_NEW_MAIL,
            &stval); /* have question???*/
        EMA_UTILS_BREAKIF(result, "");
        
        eme_opt[i].pVal = (void*)stval.pData;  
        eme_opt[i].iId = EME_OPTID_RECEIVE_LATEST;
        i++;

        /*end*/
        eme_opt[i].iId = EME_OPTID_END;
        eme_opt[i].pVal = PNULL;
        result = AEEEMETM_CreateSession(transmgr_handle_ptr->eme_tm_ptr, eme_opt);
        EMA_UTILS_BREAKIF(result, "");
        
   } while(0);
   
   EMA_UTILS_FREEIF(transmgr_handle_ptr->mem_ptr, size_limit_ptr);
   //EMA_UTILS_FREEIF(transmgr_handle_ptr->mem_ptr, num_limit_ptr);
   EMA_UTILS_FREEIF(transmgr_handle_ptr->mem_ptr, recserv_addr_ptr);
   EMA_UTILS_FREEIF(transmgr_handle_ptr->mem_ptr, recserv_port_ptr);
   EMA_UTILS_FREEIF(transmgr_handle_ptr->mem_ptr, recuser_name_ptr);
   EMA_UTILS_FREEIF(transmgr_handle_ptr->mem_ptr, rec_password_ptr);
   EMA_UTILS_FREEIF(transmgr_handle_ptr->mem_ptr, sendserv_addr_ptr);
   EMA_UTILS_FREEIF(transmgr_handle_ptr->mem_ptr, sendserv_port_ptr);
   EMA_UTILS_FREEIF(transmgr_handle_ptr->mem_ptr, senduser_name_ptr);
   EMA_UTILS_FREEIF(transmgr_handle_ptr->mem_ptr, send_password_ptr);
   EMA_UTILS_FREEIF(transmgr_handle_ptr->mem_ptr, send_emailaddr_ptr);
   EMA_UTILS_FREEIF(transmgr_handle_ptr->mem_ptr, pcuid_ptr);
   EMA_UTILS_FREEIF(transmgr_handle_ptr->mem_ptr, pcuidl_ptr);
   return result;
}

/**************************************************************************************/
// Description : email sp transaction, retrieve account email head.
// Global resource dependence : 
// Author: 
// Note: Parameter is TBD. Need to differentiate POP3 and IMAP4?
/**************************************************************************************/
PUBLIC int32 MMIEMAIL_SP_TRS_RetrieveActEmailHead(
                                                  HTransMgr   trans_mgr, 
                                                  uint32      act_id,
                                                  char	    *box_name_ptr,
                                                  uint32      net_id
                                                  )
{
    EMAIL_TRANSMGR_HANDLE_T *transmgr_handle_ptr = (EMAIL_TRANSMGR_HANDLE_T*)trans_mgr;
    int32    result = EMAIL_ERROR;
    TransID  trans_id = 0;
    AEEEMEOpt trans_opt[5] = {0};
    EMASP_PRFITEM_VAL stval = {0};
    uint32 trans_type = 0;
    HStoreMgr   store_mgr = PNULL;
    int index = 0;   
    
    if (PNULL == transmgr_handle_ptr || PNULL == transmgr_handle_ptr->eme_tm_ptr)
    {
        return EMAIL_BADPARAM;
    }
    
    do 
    {
        store_mgr = MMIEMAIL_SP_GetStoreHandle(transmgr_handle_ptr->ema_sp_mgr);
        EMA_UTILS_BREAKNULL(store_mgr, result, "MMIEMAIL_SP_TRS_RetrieveActEmailHead: Get store_mgr failed!");
        
        result = SP_TRS_CreateSession(trans_mgr,store_mgr,act_id);
        EMA_UTILS_BREAKIF(result, "MMIEMAIL_SP_TRS_RetrieveActEmailHead:SP_TRS_CreateSession failed!");
        
        result = MMIEMAIL_SP_Store_GetActCfgItem(store_mgr,
            act_id,
            EMASP_PRFITEM_RECEIVE_SERVER_TYPE,
            &stval);
        EMA_UTILS_BREAKIF(result, "MMIEMAIL_SP_TRS_RetrieveActEmailHead: get receive server type failed!");
        if (EMASP_POP3 == (uint32)stval.pData)
        {
            trans_type = EME_TRANS_POP3_FETCH_NEW_MAILS_HEADER;
        }
        else if (EMASP_IMAP4 == (uint32)stval.pData)
        {
            trans_type = EME_TRANS_IMAP4_FETCH_NEW_MAILS_HEADER;
            trans_opt[index].iId = EME_OPTID_LOGIN_MAILBOXNAME;
            trans_opt[index].pVal = (void*)box_name_ptr;
            index++;
        }
        trans_opt[index].iId = EME_OPTID_TRANS_TYPE;
        trans_opt[index].pVal = (void*)trans_type;
        index++;
        trans_opt[index].iId = EME_OPTID_REPLY_CLSID;
        trans_opt[index].pVal = (void*)EMA_ENGINE_REPLY_CLSID;
        index++;
        trans_opt[index].iId = EME_OPTID_CONN_NETID;
        trans_opt[index].pVal = (void*)net_id;
        index++;
        trans_opt[index].iId = EME_OPTID_END;
        result = (int32)AEEEMETM_EnqueueTrans(transmgr_handle_ptr->eme_tm_ptr, trans_opt, &trans_id);
        EMA_UTILS_BREAKIF(result, "");
        
        /* Save transaction id and message id into list */
        result = SP_TRS_InsertTrans(trans_mgr,&transmgr_handle_ptr->ema_tm_list_ptr, trans_type, trans_id, 0);
        EMA_UTILS_BREAKIF(result, "");
    } while(0);
    
    return result;
}

/**************************************************************************************/
// Description : email sp transaction, retrieve account email.
// Global resource dependence : 
// Author: 
// Note: Parameter is TBD. Need to differentiate POP3 and IMAP4?
/**************************************************************************************/
PUBLIC int32 MMIEMAIL_SP_TRS_RetrieveActEmail(
                                              HTransMgr  trans_mgr, 
                                              uint32     act_id,
                                              char	      *box_name_ptr,
                                              uint32     net_id
                                              )
{
    EMAIL_TRANSMGR_HANDLE_T *transmgr_handle_ptr = (EMAIL_TRANSMGR_HANDLE_T*)trans_mgr;
    int32    result = EMAIL_ERROR;
    TransID  trans_id = 0;
    AEEEMEOpt trans_opt[5] = {0};
    EMASP_PRFITEM_VAL stval = {0};
    uint32 trans_type = 0;
    HStoreMgr   store_mgr = PNULL;
    int index = 0;
    
    if (PNULL == transmgr_handle_ptr || PNULL == transmgr_handle_ptr->eme_tm_ptr)
    {
        return EMAIL_BADPARAM;
    }
    
    do 
    {
        store_mgr = MMIEMAIL_SP_GetStoreHandle(transmgr_handle_ptr->ema_sp_mgr);
        EMA_UTILS_BREAKNULL(store_mgr, result, "MMIEMAIL_SP_TRS_RetrieveActEmail: Get store_mgr failed!");
        
        result = SP_TRS_CreateSession(trans_mgr,store_mgr,act_id);
        EMA_UTILS_BREAKIF(result, "MMIEMAIL_SP_TRS_RetrieveActEmail:SP_TRS_CreateSession failed!");
        
        
        result = MMIEMAIL_SP_Store_GetActCfgItem(store_mgr,
            act_id,
            EMASP_PRFITEM_RECEIVE_SERVER_TYPE,
            &stval);
        EMA_UTILS_BREAKIF(result, "MMIEMAIL_SP_TRS_RetrieveActEmail: get receive server type failed!");
        if (EMASP_POP3 == (uint32)stval.pData)
        {
            trans_type = EME_TRANS_POP3_FETCH_NEW_MAILS;
        }
        else if (EMASP_IMAP4 == (uint32)stval.pData)
        {
            trans_type = EME_TRANS_IMAP4_FETCH_NEW_MAILS;
            trans_opt[index].iId = EME_OPTID_LOGIN_MAILBOXNAME;
            trans_opt[index].pVal = (void*)box_name_ptr;
            index++;
        }
        trans_opt[index].iId = EME_OPTID_TRANS_TYPE;
        trans_opt[index].pVal = (void*)trans_type;
        index++;
        trans_opt[index].iId = EME_OPTID_REPLY_CLSID;
        trans_opt[index].pVal = (void*)EMA_ENGINE_REPLY_CLSID;
        index++;
        trans_opt[index].iId = EME_OPTID_CONN_NETID;
        trans_opt[index].pVal = (void*)net_id;
        index++;
        trans_opt[index].iId = EME_OPTID_END;
        result = (int32)AEEEMETM_EnqueueTrans(transmgr_handle_ptr->eme_tm_ptr, trans_opt, &trans_id);
        EMA_UTILS_BREAKIF(result, "");
        
        /* Save transaction id and message id into list */
        result = SP_TRS_InsertTrans(trans_mgr,&transmgr_handle_ptr->ema_tm_list_ptr, trans_type, trans_id, 0);
        EMA_UTILS_BREAKIF(result, "");
    } while(0);
    
    return result;
}

/**************************************************************************************/
// Description : email sp transaction, retrieve account email body.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC int32 MMIEMAIL_SP_TRS_RetrieveEmailBody(
                                             HTransMgr trans_mgr, 
                                             uint32    act_id,
                                             uint32    email_id,
                                             uint32    net_id
                                             )
{
    EMAIL_TRANSMGR_HANDLE_T *transmgr_handle_ptr = (EMAIL_TRANSMGR_HANDLE_T*)trans_mgr;
    AEEEMEOpt       trans_opt[5] = {0};
    int32           result = EMAIL_ERROR;
    TransID         trans_id = 0;
    EMASP_PRFITEM_VAL stval = {0};
    uint32 trans_type = 0;
    HStoreMgr   store_mgr = PNULL;
    char ema_msg_id[9] = {0};
    uint32 index = 0;
    
    if (PNULL == transmgr_handle_ptr || PNULL == transmgr_handle_ptr->eme_tm_ptr)
    {
        return EMAIL_BADPARAM;
    }
    
    do 
    {
        store_mgr = MMIEMAIL_SP_GetStoreHandle(transmgr_handle_ptr->ema_sp_mgr);
        EMA_UTILS_BREAKNULL(store_mgr, result, "MMIEMAIL_SP_TRS_RetrieveEmailBody: Get store_mgr failed!");
        
        /* Provide the necessary session to engine */
        result = SP_TRS_CreateSession(trans_mgr,store_mgr,act_id);
        EMA_UTILS_BREAKIF(result, "MMIEMAIL_SP_TRS_RetrieveEmailBody: SP_TRS_CreateSession failed!");
        
        /* Init Opt & fetch email body from service*/
        result = MMIEMAIL_SP_Store_GetActCfgItem(store_mgr,
            act_id,
            EMASP_PRFITEM_RECEIVE_SERVER_TYPE,
            &stval);
        EMA_UTILS_BREAKIF(result, "MMIEMAIL_SP_TRS_RetrieveEmailBody: get receive server type failed!");
        if (EMASP_POP3 == (uint32)stval.pData)
        {
            trans_type = EME_TRANS_POP3_FETCH_SPEC_MAIL;
        }
        else if (EMASP_IMAP4 == (uint32)stval.pData)
        {
            trans_type = EME_TRANS_IMAP4_FETCH_SPEC_MAIL;
        }
        trans_opt[index].iId = EME_OPTID_TRANS_TYPE;
        trans_opt[index].pVal = (void*)trans_type;
        index++;
        trans_opt[index].iId = EME_OPTID_MESSAGE_ID;
        EMA_UTILS_SPRINTF(ema_msg_id,"%x",email_id);
        trans_opt[index].pVal = (void*)ema_msg_id;
        index++;
        trans_opt[index].iId = EME_OPTID_REPLY_CLSID;
        trans_opt[index].pVal = (void*)EMA_ENGINE_REPLY_CLSID;
        index++;
        trans_opt[index].iId = EME_OPTID_CONN_NETID;
        trans_opt[index].pVal = (void*)net_id;
        index++;
        trans_opt[index].iId = EME_OPTID_END;
        result = (int32)AEEEMETM_EnqueueTrans(transmgr_handle_ptr->eme_tm_ptr, trans_opt, &trans_id);
        EMA_UTILS_BREAKIF(result, "MMIEMAIL_SP_TRS_RetrieveEmailBody: enqueue trans failed!");
        
        /* Save transaction id and message id into list */
        result = SP_TRS_InsertTrans(trans_mgr,&transmgr_handle_ptr->ema_tm_list_ptr, trans_type, trans_id, email_id);
        EMA_UTILS_BREAKIF(result, "");
    } while(0);
    return result;
}

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
                                                )
{
    return EMAIL_SUCCESS;
}

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
                                            )
{
    EMAIL_TRANSMGR_HANDLE_T *transmgr_handle_ptr = (EMAIL_TRANSMGR_HANDLE_T*)trans_mgr;
    AEEEMEOpt       trans_opt[3] = {0};
    int32           result = EMAIL_ERROR;
    TransID         trans_id = 0;
    HStoreMgr   store_mgr = PNULL;
    uint32    trans_type = EME_TRANS_IMAP4_LIST_ALL_MAILBOXES;
    
    if (PNULL == transmgr_handle_ptr || PNULL == transmgr_handle_ptr->eme_tm_ptr)
    {
        return EMAIL_BADPARAM;
    }
    
    do 
    {
        store_mgr = MMIEMAIL_SP_GetStoreHandle(transmgr_handle_ptr->ema_sp_mgr);
        EMA_UTILS_BREAKNULL(store_mgr, result, "MMIEMAIL_SP_TRS_RetrieveFolder: Get store_mgr failed!");
        
        /* Provide the necessary session to engine */
        result = SP_TRS_CreateSession(trans_mgr,store_mgr,act_id);
        EMA_UTILS_BREAKIF(result, "MMIEMAIL_SP_TRS_RetrieveFolder: SP_TRS_CreateSession failed!");
        
        trans_opt[0].iId = EME_OPTID_TRANS_TYPE;
        if (FALSE == is_all_or_sub)
        {
            trans_type = EME_TRANS_IMAP4_LIST_SUBSCRIBED_MAILBOXES;
        }
        trans_opt[0].pVal = (void*)trans_type;
        trans_opt[1].iId = EME_OPTID_REPLY_CLSID;
        trans_opt[1].pVal = (void*)EMA_ENGINE_REPLY_CLSID;
        trans_opt[2].iId = EME_OPTID_END;
        result = (int32)AEEEMETM_EnqueueTrans(transmgr_handle_ptr->eme_tm_ptr, trans_opt, &trans_id);
        EMA_UTILS_BREAKIF(result, "MMIEMAIL_SP_TRS_RetrieveFolder: enqueue trans failed!");
        
        /* Save transaction id and message id into list */
        result = SP_TRS_InsertTrans(trans_mgr,&transmgr_handle_ptr->ema_tm_list_ptr, trans_type, trans_id, 0);
        EMA_UTILS_BREAKIF(result, "MMIEMAIL_SP_TRS_RetrieveFolder,SP_TRS_InsertTrans failed");
    } while(0);
    
    return result;
}

/**************************************************************************************/
// Description : email sp transaction, sub or unsub folder.
// Global resource dependence : 
// Author: 
// Note:  True is Sub, Flase is Unsub
/**************************************************************************************/
PUBLIC int32 MMIEMAIL_SP_TRS_SubFolder(
                                       HTransMgr trans_mgr, 
                                       uint32    act_id,
                                       const char* box_name_ptr,
                                       BOOLEAN is_sub_folder    //True is Sub, Flase is Unsub
                                       )
{
    EMAIL_TRANSMGR_HANDLE_T *transmgr_handle_ptr = (EMAIL_TRANSMGR_HANDLE_T*)trans_mgr;
    AEEEMEOpt       trans_opt[4] = {0};
    int32           result = EMAIL_ERROR;
    TransID         trans_id = 0;
    HStoreMgr       store_mgr = PNULL;
    uint32          trans_type = EME_TRANS_IMAP4_SUBSCRIBE_MAILBOX;
    
    if (PNULL == transmgr_handle_ptr || PNULL == transmgr_handle_ptr->eme_tm_ptr)
    {
        return EMAIL_BADPARAM;
    }
    
    do 
    {
        store_mgr = MMIEMAIL_SP_GetStoreHandle(transmgr_handle_ptr->ema_sp_mgr);
        EMA_UTILS_BREAKNULL(store_mgr, result, "MMIEMAIL_SP_TRS_SubFolder: Get store_mgr failed!");
        
        /* Provide the necessary session to engine */
        result = SP_TRS_CreateSession(trans_mgr,store_mgr,act_id);
        EMA_UTILS_BREAKIF(result, "MMIEMAIL_SP_TRS_SubFolder: SP_TRS_CreateSession failed!");
        
        trans_opt[0].iId = EME_OPTID_TRANS_TYPE;
        if (FALSE == is_sub_folder)
        {
            trans_type = EME_TRANS_IMAP4_UNSUBSCRIBE_MAILBOX;
        }
        trans_opt[0].pVal = (void*)trans_type;
        trans_opt[1].iId = EME_OPTID_HANDLE_MAILBOXNAME;
        trans_opt[1].pVal = (void*)box_name_ptr;
        trans_opt[2].iId = EME_OPTID_REPLY_CLSID;
        trans_opt[2].pVal = (void*)EMA_ENGINE_REPLY_CLSID;
        trans_opt[3].iId = EME_OPTID_END;
        result = (int32)AEEEMETM_EnqueueTrans(transmgr_handle_ptr->eme_tm_ptr, trans_opt, &trans_id);
        EMA_UTILS_BREAKIF(result, "MMIEMAIL_SP_TRS_SubFolder: enqueue trans failed!");
        
        /* Save transaction id and message id into list */
        result = SP_TRS_InsertTrans(trans_mgr,&transmgr_handle_ptr->ema_tm_list_ptr, trans_type, trans_id, 0);
        EMA_UTILS_BREAKIF(result, "MMIEMAIL_SP_TRS_SubFolder,SP_TRS_InsertTrans failed");
    } while(0);
    
    return result;
}

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
                                       )
{
    EMAIL_TRANSMGR_HANDLE_T *transmgr_handle_ptr = (EMAIL_TRANSMGR_HANDLE_T*)trans_mgr;
    int32    result = EMAIL_ERROR;
    TransID  trans_id = 0;
    AEEEMEOpt trans_opt[4] = {0};
    uint32 trans_type = 0;
    HStoreMgr   store_mgr = PNULL;
    char ema_msg_id[9] = {0};
    int index = 0;

    if (PNULL == transmgr_handle_ptr || PNULL == transmgr_handle_ptr->eme_tm_ptr || 0 == email_id || 0 == act_id)
    {
        return EMAIL_BADPARAM;
    }
    
    do 
    {
        store_mgr = MMIEMAIL_SP_GetStoreHandle(transmgr_handle_ptr->ema_sp_mgr);
        EMA_UTILS_BREAKNULL(store_mgr, result, "MMIEMAIL_SP_TRS_Imap4Mark: Get store_mgr failed!");
        
        result = SP_TRS_CreateSession(trans_mgr,store_mgr,act_id);
        EMA_UTILS_BREAKIF(result, "MMIEMAIL_SP_TRS_Imap4Mark:SP_TRS_CreateSession failed!");
        
        switch(make_type)
        {
        case EMA_IMAP4_MARK_DEL_ON_SERVER:
            trans_type = EME_TRANS_IMAP4_MARK_DEL_ON_SERVER;
            break;
        case EMA_IMAP4_UNMARK_DEL_ON_SERVER:
            trans_type = EME_TRANS_IMAP4_UNMARK_DEL_ON_SERVER;
            break;
        case EMA_IMAP4_MARK_SEEN_ON_SERVER:
            trans_type = EME_TRANS_IMAP4_MARK_SEEN_ON_SERVER;
            break;
        case EMA_IMAP4_UNMARK_SEEN_ON_SERVER:
            trans_type = EME_TRANS_IMAP4_MARK_SEEN_ON_SERVER;
            break;
        default:
            break;
        }
        trans_opt[index].iId = EME_OPTID_TRANS_TYPE;
        trans_opt[index].pVal = (void*)trans_type;
        index++;
        trans_opt[index].iId = EME_OPTID_REPLY_CLSID;
        trans_opt[index].pVal = (void*)EMA_ENGINE_REPLY_CLSID;
        index++;
        trans_opt[index].iId = EME_OPTID_MESSAGE_ID;
        EMA_UTILS_SPRINTF(ema_msg_id,"%x",email_id);
        trans_opt[index].pVal = (void*)ema_msg_id;
        index++;
        trans_opt[index].iId = EME_OPTID_END;
        result = (int32)AEEEMETM_EnqueueTrans(transmgr_handle_ptr->eme_tm_ptr, trans_opt, &trans_id);
        EMA_UTILS_BREAKIF(result, "");
        
        /* Save transaction id and message id into list */
        result = SP_TRS_InsertTrans(trans_mgr,&transmgr_handle_ptr->ema_tm_list_ptr, trans_type, trans_id, email_id);
        EMA_UTILS_BREAKIF(result, "");
        
    } while(0);
    
    return result;
}

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
                                              const char* box_name_ptr,
                                              BOOLEAN is_upload   
                                              )
{
    EMAIL_TRANSMGR_HANDLE_T *transmgr_handle_ptr = (EMAIL_TRANSMGR_HANDLE_T*)trans_mgr;
    int32      result = EMAIL_ERROR;
    TransID    trans_id = 0;
    AEEEMEOpt  trans_opt[5] = {0};
    uint32     trans_type = 0;
    HStoreMgr  store_mgr = PNULL;
    char       ema_msg_id[9] = {0};
    int index = 0;
    if (PNULL == transmgr_handle_ptr || PNULL == transmgr_handle_ptr->eme_tm_ptr || 0 == email_id || 0 == act_id)
    {
        return EMAIL_BADPARAM;
    }
    
    do 
    {
        store_mgr = MMIEMAIL_SP_GetStoreHandle(transmgr_handle_ptr->ema_sp_mgr);
        EMA_UTILS_BREAKNULL(store_mgr, result, "MMIEMAIL_SP_TRS_UploadOrCopyMail: Get store_mgr failed!");
        
        result = SP_TRS_CreateSession(trans_mgr,store_mgr,act_id);
        EMA_UTILS_BREAKIF(result, "MMIEMAIL_SP_TRS_UploadOrCopyMail:SP_TRS_CreateSession failed!");
        if (TRUE == is_upload)
        {
            trans_type = EME_TRANS_IMAP4_UPLOAD_MAIL_TO_SERVER;
        } 
        else
        {
            trans_type = EME_TRANS_IMAP4_COPY_MAIL_TO_MAILBOX;
        }
        trans_opt[index].iId = EME_OPTID_TRANS_TYPE;
        trans_opt[index].pVal = (void*)trans_type;
        index++;
        trans_opt[index].iId = EME_OPTID_REPLY_CLSID;
        trans_opt[index].pVal = (void*)EMA_ENGINE_REPLY_CLSID;
        index++;
        trans_opt[index].iId = EME_OPTID_MESSAGE_ID;
        EMA_UTILS_SPRINTF(ema_msg_id,"%x",email_id);
        trans_opt[index].pVal = (void*)ema_msg_id;
        index++;
        trans_opt[index].iId = EME_OPTID_HANDLE_MAILBOXNAME;
        trans_opt[index].pVal = (void*)box_name_ptr;
        index++;
        trans_opt[index].iId = EME_OPTID_END;
        result = (int32)AEEEMETM_EnqueueTrans(transmgr_handle_ptr->eme_tm_ptr, trans_opt, &trans_id);
        EMA_UTILS_BREAKIF(result, "");
        
        /* Save transaction id and message id into list */
        result = SP_TRS_InsertTrans(trans_mgr,&transmgr_handle_ptr->ema_tm_list_ptr, trans_type, trans_id, email_id);
        EMA_UTILS_BREAKIF(result, "");
        
    } while(0);
    
    return result;
}

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
                                     BOOLEAN          *is_has_download_ptr
                                     )
{
    EMAIL_TRANSMGR_HANDLE_T *transmgr_handle_ptr = (EMAIL_TRANSMGR_HANDLE_T*)trans_mgr;
    HStoreMgr       store_mgr = PNULL;
    uint32          part_num = 0;
    int32           result = EMAIL_ERROR;
    
    if (PNULL == transmgr_handle_ptr || PNULL == email_datacache_ptr || PNULL == is_has_download_ptr)
    {
        return EMAIL_BADPARAM;
    }
    
    do 
    {
        store_mgr = MMIEMAIL_SP_GetStoreHandle(transmgr_handle_ptr->ema_sp_mgr);
        EMA_UTILS_BREAKNULL(store_mgr, result, "MMIEMAIL_SP_TRS_ViewEml: Get store_mgr failed!");
        
        // Get the number of email part.
        result = MMIEMAIL_SP_Store_GetPartCount(store_mgr, act_id, email_id, &part_num);
        EMA_UTILS_BREAKIF(result, "MMIEMAIL_SP_TRS_ViewEml:MMIEMAIL_SP_Store_GetPartCount failed!");
        
        if (0 == part_num)/* Need to download*/
        {
            result = MMIEMAIL_SP_TRS_RetrieveEmailBody(transmgr_handle_ptr, act_id, email_id, 0);
            EMA_UTILS_BREAKIF(result, "MMIEMAIL_SP_TRS_ViewEml: MMIEMAIL_SP_TRS_RetrieveEmailBody failed!");
            *is_has_download_ptr = FALSE;
        }
        else /* The email is exist, get it from store*/
        {
            result = MMIEMAIL_SP_Store_GetEmailData(store_mgr, 
                email_id, 
                act_id, 
                email_datacache_ptr);
            EMA_UTILS_BREAKIF(result, "MMIEMAIL_SP_TRS_ViewEml: MMIEMAIL_SP_Store_GetEmailData failed!");
            *is_has_download_ptr = TRUE;
        }
    } while(0); 
    return result;
}

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
                                           )
{
    AEEEMETransResult *trans_result_ptr = PNULL;   
    AEEEMESizeNotify  *eme_sizenotify_ptr = PNULL; 
    EMAIL_TRANSMGR_HANDLE_T* transmgr_handle_ptr = (EMAIL_TRANSMGR_HANDLE_T*)trans_mgr ;
    uint32 ema_event= 0;
    
    if (PNULL == transmgr_handle_ptr)
    {
        EMA_UTILS_LOG_ERROR(("MMIEMAIL_SP_TRS_HandleEvent,bad inparam")); 
        return FALSE;
    }

    switch(eCode)
    {
    case EVT_EME_TRANS_RESULT:
        trans_result_ptr = (AEEEMETransResult*)dwparam;
        MMIEMAIL_SP_TRS_GetEmailidByTransId(trans_mgr,trans_result_ptr->uiTransType,(uint32*)&wparam);
        ema_event = EVT_EMA_TRANS_RESULT;
        MMIEMAIL_SP_TRS_DeleteTrans(trans_mgr, trans_result_ptr->uiTransType, 0);
        break;
    case EVT_EME_NEW_MESSAGE:
        ema_event = EVT_EMA_NEW_MESSAGE;
        break;
    case EVT_EME_MAILBOX_GETED:
        ema_event = EVT_EMA_MAILBOX_GETED;
        break;
    case EVT_EME_MESSAGE_SIZE_NOTIFY:
        ema_event = EVT_EMA_MESSAGE_SIZE_NOTIFY;
        break;
    case EVT_EME_MESSAGE_NUMBER_NOTIFY:
        ema_event = EVT_EMA_MESSAGE_NUMBER_NOTIFY;
        break;
        //   case EVT_EME_MESSAGE_NUMBER_NOTIFY:
        //      break;
        //   case EVT_EME_CANCEL_SUCCESS:
        //      break;
        //   case EVT_EME_TRANSFERNOTIFY:
        //      break;
    default:
        return FALSE;
    }
    if (EVT_EMA_MESSAGE_SIZE_NOTIFY == ema_event)
    {
        eme_sizenotify_ptr = (AEEEMESizeNotify*)dwparam;
        if (PNULL == eme_sizenotify_ptr)
        {
            return FALSE;
        }
        
        if (EME_TRANS_IMAP4_FETCH_NEW_MAILS_HEADER == eme_sizenotify_ptr->uiTransType ||
            EME_TRANS_POP3_FETCH_NEW_MAILS_HEADER == eme_sizenotify_ptr->uiTransType)
        {
            return TRUE;
        }
    }
    
    return TRUE;
}

