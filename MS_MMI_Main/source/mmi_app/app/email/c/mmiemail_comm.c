/***************************************************************************************
** File Name:      mmiemail_comm.c                                                     *
** Author:                                                                             *
** Date:           05/13/2011                                                          *
** Copyright:      2011 Spreadtrum, Incorporated. All Rights Reserved.                 *
** Description:    This File will gather functions that all events to this app are     *
**                 handled. These functions don't be changed by project changed.       *
****************************************************************************************
**                         Important Edit History                                      *
** ------------------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                                         *
** 05/2011                         create mmiemail_comm.c                              *
**                                                                                     *
***************************************************************************************/

/**------------------------------------------------------------------------------------*
**                         Include Files                                               *
**------------------------------------------------------------------------------------*/
#include "mmiemail_def.h"
#include "mmiemail_utils.h"
#include "priority_app.h"
#include "os_api.h"
/**------------------------------------------------------------------------------------*
**                         Macro Declaration    						               *
**------------------------------------------------------------------------------------*/

/**------------------------------------------------------------------------------------*
**                         TYPE AND CONSTANT    				 	                   *
**------------------------------------------------------------------------------------*/
extern wchar   g_email_send_str[EMA_MAX_ADDR_LEN];

/**------------------------------------------------------------------------------------*
**                         Constant Variables                                          *
**------------------------------------------------------------------------------------*/

/**------------------------------------------------------------------------------------*
**                         STATIC DEFINITION                                           *
**------------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------------*
**                          LOCAL FUNCTION DECLARE                                     *
**------------------------------------------------------------------------------------*/

/**------------------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                                         *
**------------------------------------------------------------------------------------*/

/**************************************************************************************/
// Description : Release EMAIL_MSGDATACACHE_T struct.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC void MMIEMAIL_ReleaseEmailDataCache(void *mem_mgr_ptr, EMAIL_MSGDATACACHE_T *data_cache_ptr)
{
    int index = 0;
    if (PNULL == data_cache_ptr)
    {
        EMA_UTILS_TRACE("MMIEMAIL_ReleaseEmailDataCache, bad param");
        return;
    }
    for (index = 0; index < data_cache_ptr->attach_num; index++)
    {
        EMA_UTILS_FREEIF(mem_mgr_ptr, data_cache_ptr->attach_size_ptr[index]);
    }
    EMA_UTILS_FREEIF(PNULL, data_cache_ptr->data_buff_ptr);
    EMA_UTILS_FREEIF(PNULL, data_cache_ptr->read_report_ptr);
    EMA_UTILS_FREEIF(PNULL, data_cache_ptr->source_buff_ptr);
    EMA_UTILS_FREEIF(PNULL, data_cache_ptr->subject_ptr);
    EMA_UTILS_FREE(PNULL, data_cache_ptr);
}

/**************************************************************************************/
// Description : Copy EMAIL_MSGDATACACHE_T function.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC int32 MMIEMAIL_CopyEmailDataCache(
                                       EMAIL_MSGDATACACHE_T *dest_data_cache_ptr, 
                                       EMAIL_MSGDATACACHE_T *src_data_cache_ptr, 
                                       void *mem_mgr_ptr
                                       )
{
    int index = 0;
    uint len = 0;

    if (PNULL == src_data_cache_ptr || PNULL == dest_data_cache_ptr)
    {
        EMA_UTILS_TRACE(("MMIEMAIL_CopyEmailDataCache, bad param."));
        return EMAIL_ERROR;
    }
    
    EMA_UTILS_MEMCPY(dest_data_cache_ptr, src_data_cache_ptr, sizeof(EMAIL_MSGDATACACHE_T));
    
    if (PNULL != src_data_cache_ptr->read_report_ptr)
    {
        dest_data_cache_ptr->read_report_ptr = EMA_UTILS_WSTRDUP(mem_mgr_ptr, src_data_cache_ptr->read_report_ptr);
    }
    
    for (index = 0; index < src_data_cache_ptr->attach_num; index++)
    {
        if (PNULL != src_data_cache_ptr->attach_size_ptr[index])
        {
            len = (EMA_UTILS_WSTRLEN(src_data_cache_ptr->attach_size_ptr[index]) + 1) * sizeof(wchar);
            dest_data_cache_ptr->attach_size_ptr[index] = (wchar *)EMA_UTILS_MALLOC(mem_mgr_ptr, len);
            if (PNULL == dest_data_cache_ptr->attach_size_ptr[index])
            {
                EMA_UTILS_TRACE(("MMIEMAIL_CopyEmailDataCache, pDest->attach_size_ptr[i] is PNULL."));
                return EMAIL_ERROR;
            }
            EMA_UTILS_MEMSET(dest_data_cache_ptr->attach_size_ptr[index], 0, len);
            EMA_UTILS_MEMCPY(dest_data_cache_ptr->attach_size_ptr[index], src_data_cache_ptr->attach_size_ptr[index], len - 2);
        }
    }
    
    if (PNULL != src_data_cache_ptr->data_buff_ptr)
    {
        EMA_UTILS_TRACE("data_buff_ptr len");
        len = (EMA_UTILS_WSTRLEN(src_data_cache_ptr->data_buff_ptr) + 1) * sizeof(wchar);
        dest_data_cache_ptr->data_buff_ptr = (wchar *)EMA_UTILS_MALLOC(mem_mgr_ptr, len);
        if (PNULL == dest_data_cache_ptr->data_buff_ptr)
        {
            EMA_UTILS_TRACE(("MMIEMAIL_CopyEmailDataCache, pDest->data_buff_ptr is PNULL."));
            return EMAIL_ERROR;
        }
        EMA_UTILS_MEMSET(dest_data_cache_ptr->data_buff_ptr, 0, len);
        EMA_UTILS_MEMCPY(dest_data_cache_ptr->data_buff_ptr, src_data_cache_ptr->data_buff_ptr, len - 2);
    }
    
    if (PNULL != src_data_cache_ptr->source_buff_ptr)
    {
        dest_data_cache_ptr->source_buff_ptr =PNULL;
        dest_data_cache_ptr->source_buff_ptr = EMA_UTILS_STRDUP(mem_mgr_ptr, src_data_cache_ptr->source_buff_ptr);
    }
    
    if (PNULL != src_data_cache_ptr->subject_ptr)
    {
        dest_data_cache_ptr->subject_ptr = PNULL;
        dest_data_cache_ptr->subject_ptr = EMA_UTILS_WSTRDUP(mem_mgr_ptr, src_data_cache_ptr->subject_ptr);
    }
    return EMAIL_SUCCESS;
}

/**************************************************************************************/
// Description : Copy St_EMAALLFolderName function.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC int32 MMIEMAIL_CopyAllFolderName(
                                        EMAIL_FOLDER_NAME_T *dest_data_cache_ptr, 
                                        EMAIL_FOLDER_NAME_T *src_data_cache_ptr, 
                                        void *mem_mgr_ptr
                                        )
{
    uint32 index = 0;
    
    if (PNULL == src_data_cache_ptr || PNULL == dest_data_cache_ptr)
    {
        EMA_UTILS_TRACE(("MMIEMAIL_CopyALLFolderName, bad param."));
        return EMAIL_ERROR;
    }
    EMA_UTILS_MEMCPY(dest_data_cache_ptr, src_data_cache_ptr, sizeof(EMAIL_FOLDER_NAME_T));
    
    for (index = 0; index < src_data_cache_ptr->folder_num; index++)
    {
        if (PNULL != src_data_cache_ptr->all_folder_name_ptr[index])
        {
            dest_data_cache_ptr->all_folder_name_ptr[index] = EMA_UTILS_WSTRDUP(mem_mgr_ptr, src_data_cache_ptr->all_folder_name_ptr[index]);
        }
    }
    return EMAIL_SUCCESS;
}

/**************************************************************************************/
// Description : Copy  EMAIL_MSGHEAD_T function.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC int32 MMIEMAIL_CopyEmailHead(
                                     EMAIL_MSGHEAD_T *dest_data_cache_ptr, 
                                     EMAIL_MSGHEAD_T *src_data_cache_ptr, 
                                     void *mem_mgr_ptr
                                     )
{
    if (PNULL == src_data_cache_ptr || PNULL == dest_data_cache_ptr)
    {
        EMA_UTILS_TRACE(("MMIEMAIL_CopyEmailHead, bad param."));
        return EMAIL_ERROR;
    }
    
    EMA_UTILS_MEMCPY(dest_data_cache_ptr, src_data_cache_ptr, sizeof(EMAIL_MSGHEAD_T));
    
    dest_data_cache_ptr->dwdate = src_data_cache_ptr->dwdate;
    
    if (PNULL != src_data_cache_ptr->wsz_from_ptr)
    {
        dest_data_cache_ptr->wsz_from_ptr = EMA_UTILS_WSTRDUP(mem_mgr_ptr, src_data_cache_ptr->wsz_from_ptr);
    }
    if (PNULL != src_data_cache_ptr->wsz_to_ptr)
    {
        dest_data_cache_ptr->wsz_to_ptr = EMA_UTILS_WSTRDUP(mem_mgr_ptr, src_data_cache_ptr->wsz_to_ptr);
    }
    if (PNULL != src_data_cache_ptr->wsz_subject_ptr)
    {
        dest_data_cache_ptr->wsz_subject_ptr = EMA_UTILS_WSTRDUP(mem_mgr_ptr, src_data_cache_ptr->wsz_subject_ptr);
    }
    
    dest_data_cache_ptr->email_size = src_data_cache_ptr->email_size;
    dest_data_cache_ptr->email_prio = src_data_cache_ptr->email_prio;
    return EMAIL_SUCCESS;
}

/**************************************************************************************/
// Description : Get all recipients for EMAIL_MSGDATACACHE_T.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC int32 MMIEMAIL_GetAllRecipients(void *mem_mgr_ptr, EMAIL_MSGDATACACHE_T *data_cache_ptr)
{
    wchar   *recipients_ptr = PNULL;
    int32   result = EMAIL_ERROR;
    uint32   i = 0;
    uint32   j = 0;
    uint32  addr_len = 0;
    uint32  addr_num = 0;
    
    do 
    {
        addr_num = data_cache_ptr->to_addrs_num + data_cache_ptr->cc_addrs_num + data_cache_ptr->bcc_addrs_num;
        
        addr_len = (addr_num + 1) * (EMA_MAX_ADDR_LEN + 1) * sizeof(wchar);
        recipients_ptr = EMA_UTILS_MALLOC(mem_mgr_ptr, addr_len);
        EMA_UTILS_BREAKNULL(recipients_ptr, result, "MMIEmail_EditGetRecipient: Malloc recipients_ptr failed!");
        EMA_UTILS_MEMSET(recipients_ptr, 0 , addr_len);
        for (i = 0; i < (uint32)data_cache_ptr->to_addrs_num; i++,j++)
        {
            EMA_UTILS_WSTRCAT(recipients_ptr, data_cache_ptr->to_addrs[i]);
            if (j < (addr_num - 1))
            {
                EMA_UTILS_WSTRCAT(recipients_ptr, (wchar*)L";");
            }
        }
        
        for (i = 0; i < (uint32)data_cache_ptr->cc_addrs_num; i++,j++)
        {
            EMA_UTILS_WSTRCAT(recipients_ptr, data_cache_ptr->cc_addrs[i]);
            if (j < (addr_num - 1))
            {
                EMA_UTILS_WSTRCAT(recipients_ptr, (wchar*)L";");
            }
        }
        
        for (i = 0; i < (uint32)data_cache_ptr->bcc_addrs_num; i++,j++)
        {
            EMA_UTILS_WSTRCAT(recipients_ptr, data_cache_ptr->bcc_addrs[i]);
            if (j < (addr_num - 1))
            {
                EMA_UTILS_WSTRCAT(recipients_ptr, (wchar*)L";");
            }
        }
        
        EMA_UTILS_MEMSET(&g_email_send_str, 0x00, EMA_MAX_ADDR_LEN * sizeof(wchar));
        EMA_UTILS_MEMCPY(&g_email_send_str, recipients_ptr, EMA_MAX_ADDR_LEN * sizeof(wchar));
    } while(0);
    
    EMA_UTILS_FREEIF(PNULL, recipients_ptr);
    return EMAIL_SUCCESS;
}

PUBLIC uint32 MMIEMAIL_GetEmailTaskPriority(void)
{
   return PRI_EMAIL_APP_TASK;
}

PUBLIC void *email_alloc(uint32 size, const char *file, uint32 line) 
{ 
#ifdef SCI_MEMORY_DEBUG
        return SCI_MallocApp_Zero(size, file, line);
#else
        return SCI_MallocApp_Zero_NoDebug(size);
#endif
} 

PUBLIC void email_free(void *p)
{
   SCI_FREE(p);
}
