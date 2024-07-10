/****************************************************************************
** File Name:      mmimms_edit.c                                           *
** Author:         aoke.hu                                                 *
** Date:           04/13/2009                                              *
** Copyright:      2009 Spreadtrum, Incorporated. All Rights Reserved.     *
** Description:                                                            *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 04/2009        aoke.hu          create
** 
****************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Include Files                                     *
 **---------------------------------------------------------------------------*/
#include "mmi_app_mms_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "mmimms_file.h"
#include "mmimms_edit.h"
#include "mmisms_export.h"
#include "mmisms_id.h"
#include "mmifmm_export.h"
#include "mmimms_setting.h"
#include "mmimms_control_main.h"
#include "mmimms_push.h"
#include "os_api.h"
#include "mmiwclk_export.h"
#include "sysMemFile.h"
#include "mmisd_export.h"
#include "mmimms_id.h"
#include "mmimms_internal.h"
#include "guifont.h"
#include "gui_ucs2b_converter.h "
//#include "mmiset.h"
#include "mmiset_export.h"
#ifdef DRM_SUPPORT
#include "mmidrm_export.h"
#endif
/**---------------------------------------------------------------------------*
 **                         Macro Declaration    							  *
 **---------------------------------------------------------------------------*/
#define INVALID_MEM_HANDLE  0
#define MMIMMS_LIST_NAME            (s_mmimms_list_name)
#define MMIMMS_LIST_NAME_LEN        11
#define MMS_POOL_NAME               "mms pool"
#define MEM_FROM_BLOCK              0xFFFFFFF
#define MEM_FROM_SYS_POOL           0x00
#define MMIMMS_TEXT_TYPE_UCS2       0xFEFF
#define MMIMMS_DECODEHEAD_BUFFER_LEN    512
#define MMIMMS_ENCODE_LEFT_SIZE     (2*1024)

//#ifdef WIN32 //type defined in mmiebook_file.c
//#define MMIMMS_TEXT_TYPE_UCS2  0xFEFF
//#else
//#define MMIMMS_TEXT_TYPE_UCS2  0xFFFE
//#endif
/**---------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT    							  *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/
MMIMMS_LISTINFO_T s_mms_list = {0};     //彩信链表信息，
MPOOL_HANDLE s_mms_mem_handle = INVALID_MEM_HANDLE;
const wchar s_mmimms_list_name[] = { 'm', 'm', 's', 'l', 'i', 's', 't', '.', 'i', 'n', 'i', 0 };

int8 *itoa( int32 v, int8* str, uint32 r);  /*lint -esym(765,itoa) */

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : get mms file full path
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetMMSListPath(
                             const wchar    *file_name_ptr,         //[IN]
                             uint16         file_name_len,          //[IN]
                             wchar          *full_path_name_ptr,    //[OUT]
                             uint16         *full_path_len_ptr      //[OUT]
                             );

/*****************************************************************************/
//  Description : get mms file full path for read
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetMMSFilePathForRead(
                                    const wchar    *file_name_ptr,         //[IN]
                                    uint16         file_name_len,          //[IN]
                                    wchar          *full_path_name_ptr,    //[OUT]
                                    uint16         *full_path_len_ptr      //[OUT]
                                    );

/*****************************************************************************/
//  Description : add item to mms list
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
LOCAL void AddListItem(
                       const MMISMS_ORDER_INFO_T *item_ptr, 
                       BOOLEAN is_update
                       );

/*****************************************************************************/
//  Description : delete list item
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
LOCAL MMISMS_MO_MT_TYPE_E DeleteListItem(int32 record_id);

/*****************************************************************************/
//  Description : check record id is unique or not
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CheckUniqueRecordId(int32 record_id);

/*****************************************************************************/
//  Description : 将wstr转化为mms库中的str格式（前两位存储长度，后面存储字符） 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
LOCAL void ConvertWstrToMMSStr(char *subject_string, 
                               uint16 size, 
                               const wchar *wstr_ptr, 
                               uint16 wstr_len
                               );

/*****************************************************************************/
//  Description : delete all mms file callback
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/ 
LOCAL void DeleteAllMMSCallback(
                                SFS_ERROR_E   error, 
                                uint32        irp_param, 
                                uint32        param1,
                                uint32        param2
                                );

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : get mms list full path
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetMMSListPath(
                             const wchar    *file_name_ptr,         //[IN]
                             uint16         file_name_len,          //[IN]
                             wchar          *full_path_name_ptr,    //[OUT]
                             uint16         *full_path_len_ptr      //[OUT]
                             )
{
    BOOLEAN     result_val = FALSE;

    result_val = MMIAPIFMM_GetSysFileFullPathForReadUdiskPrefer(
        TRUE,
        FALSE,
        MMIMMS_DEFAULT_DIR,
        MMIMMS_DEFAULT_DIR_LEN,
        file_name_ptr,
        file_name_len,
        full_path_name_ptr,
        full_path_len_ptr
        );

    return result_val;
}

/*****************************************************************************/
//  Description : get mms file full path for read
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetMMSFilePathForRead(
                                    const wchar    *file_name_ptr,         //[IN]
                                    uint16         file_name_len,          //[IN]
                                    wchar          *full_path_name_ptr,    //[OUT]
                                    uint16         *full_path_len_ptr      //[OUT]
                                    )
{
    BOOLEAN     result_val = FALSE;

    result_val = MMIAPIFMM_GetSysFileFullPathForReadUdiskPrefer(
        FALSE,
        FALSE,
        MMIMMS_DEFAULT_DIR,
        MMIMMS_DEFAULT_DIR_LEN,
        file_name_ptr,
        file_name_len,
        full_path_name_ptr,
        full_path_len_ptr
        );

    return result_val;  
}


/*****************************************************************************/
//  Description : get mms file full path for write
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMMS_GetMMSFilePathForWrite(
                                             const wchar    *file_name_ptr,         //[IN]
                                             uint16         file_name_len,          //[IN]
                                             wchar          *full_path_name_ptr,    //[OUT]
                                             uint16         *full_path_len_ptr,      //[OUT]
                                             uint32         size                    //[IN]
                                             )
{
    BOOLEAN return_val = FALSE;

    return_val = MMIAPIFMM_GetSysFileFullPathForWriteUdiskPrefer(
        FALSE,
        FALSE,
        MMIMMS_DEFAULT_DIR,
        MMIMMS_DEFAULT_DIR_LEN,
        file_name_ptr,
        file_name_len,
        size,
        full_path_name_ptr,
        full_path_len_ptr
        );

    return return_val;
}

/*****************************************************************************/
//  Description : add item to mms list
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
LOCAL void AddListItem(
                       const MMISMS_ORDER_INFO_T *item_ptr, 
                       BOOLEAN is_update
                       )
{
    MMISMS_ORDER_INFO_T* cursor_ptr = PNULL;
    MMISMS_ORDER_INFO_T* list_item_ptr = PNULL;
    BOOLEAN is_lock = FALSE;

    if(0 == item_ptr->record_id)
    {
        //SCI_TRACE_LOW:"[MMIMMS]:AddListItem error record_id==0"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_FILE_233_112_2_18_2_38_20_143,(uint8*)"");
        return;
    }
    cursor_ptr= s_mms_list.list_head_ptr;

    if(PNULL == cursor_ptr)
    {
        list_item_ptr = SCI_ALLOCAZ(sizeof(MMISMS_ORDER_INFO_T));
        if (PNULL == list_item_ptr)
        {
            //SCI_TRACE_LOW:"[MMIMMS]:AddListItem PNULL == list_item_ptr"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_FILE_243_112_2_18_2_38_20_144,(uint8*)"");
            return;
        }
        SCI_MEMCPY(list_item_ptr, item_ptr, sizeof(MMISMS_ORDER_INFO_T));
        list_item_ptr->next = PNULL;
        s_mms_list.list_head_ptr = list_item_ptr;
        s_mms_list.total_num = 1;
        if(    MMISMS_MT_NOT_DOWNLOAD == list_item_ptr->mo_mt_type
            || MMISMS_MT_NOT_NOTIFY == list_item_ptr->mo_mt_type
            || MMISMS_MT_NEED_NOT_DOWNLOAD == list_item_ptr->mo_mt_type)
        {
            s_mms_list.unsettled_num = 1;
        }
        else if(MMISMS_MT_TO_BE_READ == list_item_ptr->mo_mt_type)
        {
            s_mms_list.unread_num = 1;
        }
        MMIAPISMS_InsertOneMsgToOrder(list_item_ptr);
    }
    else if(is_update)
    {
        while(1)/*lint !e716*/
        {
            if(item_ptr->record_id == cursor_ptr->record_id)
            {
                if(    MMISMS_MT_NOT_DOWNLOAD == cursor_ptr->mo_mt_type
                    || MMISMS_MT_NOT_NOTIFY == cursor_ptr->mo_mt_type
                    || MMISMS_MT_NEED_NOT_DOWNLOAD == cursor_ptr->mo_mt_type)
                {
                    s_mms_list.unsettled_num--;
                }
                else if(MMISMS_MT_TO_BE_READ == cursor_ptr->mo_mt_type)
                {
                    s_mms_list.unread_num--;
                }
                if(    MMISMS_MT_NOT_DOWNLOAD == item_ptr->mo_mt_type
                    || MMISMS_MT_NOT_NOTIFY == item_ptr->mo_mt_type
                    || MMISMS_MT_NEED_NOT_DOWNLOAD == item_ptr->mo_mt_type)
                {
                    s_mms_list.unsettled_num++;
                }
                else if(MMISMS_MT_TO_BE_READ == item_ptr->mo_mt_type)
                {
                    s_mms_list.unread_num++;
                }
                is_lock = cursor_ptr->is_lock;    //保持原有的LOCK状态
                list_item_ptr = cursor_ptr->next;
                SCI_MEMCPY(cursor_ptr,item_ptr,sizeof(MMISMS_ORDER_INFO_T));
                cursor_ptr->is_lock = is_lock;
                cursor_ptr->next = list_item_ptr;
                MMIAPISMS_UpdateMsgStatusInOrder(cursor_ptr);
                break;
            }
            if(PNULL != cursor_ptr->next)
            {
                cursor_ptr = cursor_ptr->next;
            }
            else
            {
                list_item_ptr = SCI_ALLOCAZ(sizeof(MMISMS_ORDER_INFO_T));
                if (PNULL == list_item_ptr)
                {
                    //SCI_TRACE_LOW:"[MMIMMS]:AddListItem PNULL == list_item_ptr"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_FILE_305_112_2_18_2_38_20_145,(uint8*)"");
                    return;
                }
                SCI_MEMCPY(list_item_ptr, item_ptr, sizeof(MMISMS_ORDER_INFO_T));
                cursor_ptr->next = list_item_ptr;
                list_item_ptr->next = PNULL;
                s_mms_list.total_num++;
                if(  MMISMS_MT_NOT_DOWNLOAD == list_item_ptr->mo_mt_type
                  || MMISMS_MT_NOT_NOTIFY == list_item_ptr->mo_mt_type
                  || MMISMS_MT_NEED_NOT_DOWNLOAD == list_item_ptr->mo_mt_type)
                {
                    s_mms_list.unsettled_num++;
                }
                else if(MMISMS_MT_TO_BE_READ == list_item_ptr->mo_mt_type)
                {
                    s_mms_list.unread_num++;
                }
                MMIAPISMS_InsertOneMsgToOrder(list_item_ptr);
                break;
            }
        }
    }
    else
    {
        list_item_ptr = SCI_ALLOCAZ(sizeof(MMISMS_ORDER_INFO_T));
        if (PNULL == list_item_ptr)
        {
            //SCI_TRACE_LOW:"[MMIMMS]:AddListItem PNULL == list_item_ptr"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_FILE_332_112_2_18_2_38_20_146,(uint8*)"");
            return;
        }
        SCI_MEMCPY(list_item_ptr, item_ptr, sizeof(MMISMS_ORDER_INFO_T));
        while (PNULL != cursor_ptr->next)
        {
            cursor_ptr = cursor_ptr->next;
        }
        cursor_ptr->next = list_item_ptr;
        list_item_ptr->next = PNULL;
        s_mms_list.total_num++;
        if(   MMISMS_MT_NOT_DOWNLOAD == list_item_ptr->mo_mt_type
           || MMISMS_MT_NOT_NOTIFY == list_item_ptr->mo_mt_type
           || MMISMS_MT_NEED_NOT_DOWNLOAD == list_item_ptr->mo_mt_type)
        {
            s_mms_list.unsettled_num++;
        }
        else if(MMISMS_MT_TO_BE_READ == list_item_ptr->mo_mt_type)
        {
            s_mms_list.unread_num++;
        }
        MMIAPISMS_InsertOneMsgToOrder(list_item_ptr);
    }
}

/*****************************************************************************/
//  Description : delete list item
//  Global resource dependence : none
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
LOCAL MMISMS_MO_MT_TYPE_E DeleteListItem(int32 record_id)
{
    MMISMS_MO_MT_TYPE_E return_val = MMISMS_MT_SR_TO_BE_READ;
    MMISMS_ORDER_INFO_T* cursor_ptr = s_mms_list.list_head_ptr;
    MMISMS_ORDER_INFO_T* list_ptr = PNULL;

    MMIMMS_DeleteAutoListItem(record_id);
    MMIMMS_DeleteManualListItem(record_id);   //后台收发时需要调用这个函数
    if(record_id && cursor_ptr)
    {
        if(record_id == cursor_ptr->record_id)
        {
            list_ptr = cursor_ptr;
            s_mms_list.list_head_ptr = cursor_ptr->next;
            s_mms_list.total_num --;
            if(    MMISMS_MT_NOT_DOWNLOAD == list_ptr->mo_mt_type
                || MMISMS_MT_NOT_NOTIFY == list_ptr->mo_mt_type
                || MMISMS_MT_NEED_NOT_DOWNLOAD == list_ptr->mo_mt_type)
            {
                s_mms_list.unsettled_num--;
            }
            else if(MMISMS_MT_TO_BE_READ == list_ptr->mo_mt_type)
            {
                s_mms_list.unread_num--;
            }
            return_val = list_ptr->mo_mt_type;
            SCI_FREE(list_ptr);
            MMIAPISMS_DeleteMsgInorder(MMISMS_TYPE_MMS,record_id);
        }
        else
        {
            while(cursor_ptr->next)
            {
                if(record_id == cursor_ptr->next->record_id)
                {
                    list_ptr = cursor_ptr->next;
                    cursor_ptr->next = cursor_ptr->next->next;
                    s_mms_list.total_num --;
                    if(    MMISMS_MT_NOT_DOWNLOAD == list_ptr->mo_mt_type
                        || MMISMS_MT_NOT_NOTIFY == list_ptr->mo_mt_type
                        || MMISMS_MT_NEED_NOT_DOWNLOAD == list_ptr->mo_mt_type)
                    {
                        s_mms_list.unsettled_num--;
                    }
                    else if(MMISMS_MT_TO_BE_READ == list_ptr->mo_mt_type)
                    {
                        s_mms_list.unread_num--;
                    }
                    return_val = list_ptr->mo_mt_type;
                    SCI_FREE(list_ptr);
                    MMIAPISMS_DeleteMsgInorder(MMISMS_TYPE_MMS,record_id);
                    break;
                }
                cursor_ptr = cursor_ptr->next;
            }
        }
    }

    return return_val;
}


/*****************************************************************************/
//  Description : check record id is unique or not
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CheckUniqueRecordId(int32 record_id)
{
    BOOLEAN return_val = TRUE;
    MMISMS_ORDER_INFO_T* cursor_ptr = s_mms_list.list_head_ptr;

    if(0 != record_id)
    {
        while(PNULL != cursor_ptr)
        {
            if(record_id == cursor_ptr->record_id)
            {
                return_val = FALSE;
                break;
            }
            cursor_ptr = cursor_ptr->next;
        }
    }
    else
    {
        return_val = FALSE;
    }
    
    return return_val;
}

/*****************************************************************************/
// 	Description: 生成唯一文件名
//	Global resource dependence:
//  Author: aoke.hu
//	Note:
/*****************************************************************************/
PUBLIC uint16 MMIMMS_GenUniName(uint8 *fname,uint16 max_len)
{
    uint16      file_len = 0;
    int32       record_id  = 0;
    BOOLEAN     is_unique = FALSE;
    uint16      count = 1;

    while(1)/*lint !e716*/
    {
        file_len = MMIMMS_GenRandomName(fname,max_len);
        record_id = atoi((char *)fname);
        is_unique = CheckUniqueRecordId(record_id);
        if(is_unique)
        {
            return file_len;
        }
        else if(count++ >= MMIMMS_FILE_CREATE_COUNT)
        {
            break;
        }        
    }

    return 0;
}

/*****************************************************************************/
//  Description : 获取未下载彩信的数目
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIMMS_GetUnsettleNum(void)
{
    return s_mms_list.unsettled_num;
}

/*****************************************************************************/
//  Description : 获取未读彩信的数目
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIMMS_GetUnreadNum(void)
{
    return s_mms_list.unread_num;
}

/*****************************************************************************/
//  Description : 获取彩信数目
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIMMS_GetTotalNum(void)
{
    return s_mms_list.total_num;
}

/*****************************************************************************/
//  Description : 获取彩信已占用的总大小
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIMMS_GetTotalSize(void)
{
    uint32 total_size = 0;
    MMISMS_ORDER_INFO_T* cursor_ptr = s_mms_list.list_head_ptr;

    //累加每条彩信的大小
    while(PNULL != cursor_ptr)
    {
        total_size += cursor_ptr->file_size;
        cursor_ptr = cursor_ptr->next;
    }
    //加上链表的大小
    if(PNULL != s_mms_list.list_head_ptr)
    {
        total_size += sizeof(MMIMMS_LIST_FILE_HEAD_T)+s_mms_list.total_num*sizeof(MMISMS_ORDER_INFO_T);
    }

    return total_size;
}

/*****************************************************************************/
//  Description : 获取自动下载链表的首个条目
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC MMIMMS_AUTOLIST_T* MMIMMS_GetAutoListFirstItem(void)
{
    return s_mms_list.autolist_head_ptr;
}

/*****************************************************************************/
//  Description : 获取手动收发链表的首个条目
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC MMIMMS_AUTOLIST_T* MMIMMS_GetManualListFirstItem(void)
{
    return s_mms_list.manuallist_head_ptr;
}

/*****************************************************************************/
//  Description : 判断自动下载LIST是否有内容
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMMS_IsAutolistWithItem(void)
{
    if(s_mms_list.autolist_head_ptr)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*****************************************************************************/
//  Description : 开机初始化自动下载链表
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMIMMS_InitAutoList(void)
{
    MMISMS_ORDER_INFO_T* list_ptr = PNULL;
//  MMIMMS_AUTOLIST_T* cursor_ptr = PNULL;

    list_ptr = s_mms_list.list_head_ptr;
    while(PNULL != list_ptr)
    {
        if(MMISMS_MT_NOT_NOTIFY == list_ptr->mo_mt_type)
        {
            if (MMIMMS_GetIsStopDownloadForFlyMode())
            {
                MMIMMS_AddAutoList(list_ptr->dual_sys,list_ptr->record_id, MMIMMS_SENDRECV_RECVING);
            }
            else if (MMIMMS_IsSimCardOK(list_ptr->dual_sys))
            {
                MMIMMS_AddAutoList(list_ptr->dual_sys,list_ptr->record_id, MMIMMS_SENDRECV_RECVING);
            }
        }
        list_ptr = list_ptr->next;
    }
}

/*****************************************************************************/
//  Description : 新增条目到自动下载链表
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMIMMS_AddAutoList(
                               MN_DUAL_SYS_E dual_sys ,
                               int32 record_id,
                               MMIMMS_SENDRECV_TYPE_E  send_type
                               )
{
    MMIMMS_AUTOLIST_T   *cursor_ptr = PNULL;
    MMIMMS_AUTOLIST_T   *list_ptr = PNULL;

    if(record_id <= 0)
    {
        return ;
    }

    list_ptr = SCI_ALLOCAZ(sizeof(MMIMMS_AUTOLIST_T));
    if (PNULL == list_ptr)
    {
        //SCI_TRACE_LOW:"[MMIMMS]:MMIMMS_AddAutoList PNULL == list_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_FILE_614_112_2_18_2_38_20_147,(uint8*)"");
        return ;
    }
    //SCI_MEMSET(list_ptr,0,sizeof(MMIMMS_AUTOLIST_T));
    list_ptr->record_id = record_id;
    list_ptr->dual_sys = dual_sys;
    list_ptr->send_type = send_type;

    if(s_mms_list.autolist_head_ptr)
    {
        cursor_ptr = s_mms_list.autolist_head_ptr;
        while(PNULL != cursor_ptr->next)
        {
            cursor_ptr = cursor_ptr->next;
        }
        cursor_ptr->next = list_ptr;
    }
    else
    {
        s_mms_list.autolist_head_ptr = list_ptr;
    }
}

/*****************************************************************************/
//  Description : 新增条目到手动收发链表
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMMS_AddManualList(
                                    MN_DUAL_SYS_E dual_sys ,
                                    int32      record_id,
                                    MMIMMS_SENDRECV_TYPE_E  send_type
                                    )
{
    MMIMMS_AUTOLIST_T   *cursor_ptr = PNULL;
    MMIMMS_AUTOLIST_T   *list_ptr = PNULL;
    MMISMS_ORDER_INFO_T * list_head_ptr = PNULL;
    
    //SCI_TRACE_LOW:"[MMIMMS]:MMIMMS_AddManualList record_id=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_FILE_650_112_2_18_2_38_20_148,(uint8*)"d",record_id);
    if(record_id <= 0)
    {
        return FALSE;
    }

    list_ptr = SCI_ALLOCAZ(sizeof(MMIMMS_AUTOLIST_T));
    if (PNULL == list_ptr)
    {
        //SCI_TRACE_LOW:"[MMIMMS]:MMIMMS_AddManualList PNULL == list_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_FILE_659_112_2_18_2_38_20_149,(uint8*)"");
        return FALSE;
    }
    //SCI_MEMSET(list_ptr,0,sizeof(MMIMMS_AUTOLIST_T));
    list_ptr->record_id = record_id;
    list_ptr->dual_sys = dual_sys;
    list_ptr->send_type = send_type;
    
    list_head_ptr = MMIMMS_GetListItemByID(record_id);
    
    if(s_mms_list.manuallist_head_ptr)
    {
        cursor_ptr = s_mms_list.manuallist_head_ptr;
        if(cursor_ptr->record_id == record_id)  //队列中已存在则不加
        {
            if(list_head_ptr != PNULL)
            {
                list_head_ptr->dual_sys = dual_sys;
            }
            return TRUE;
        }
        while(PNULL != cursor_ptr->next)
        {
            cursor_ptr = cursor_ptr->next;
            if(cursor_ptr->record_id == record_id)  //队列中已存在则不加
            {
                if(list_head_ptr != PNULL)
                {
                    list_head_ptr->dual_sys = dual_sys;
                }
                return TRUE;
            }
        }
        cursor_ptr->next = list_ptr;
    }
    else
    {
        if(list_head_ptr != PNULL)
        {
            list_head_ptr->dual_sys = dual_sys;
        }
        s_mms_list.manuallist_head_ptr = list_ptr;
    }
    return TRUE;
}

/*****************************************************************************/
//  Description : 删除自动下载链表的条目
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMIMMS_DeleteAutoListItem(int32 record_id)
{
    MMIMMS_AUTOLIST_T   *cursor_ptr = s_mms_list.autolist_head_ptr;
    MMIMMS_AUTOLIST_T   *list_ptr = PNULL;

    if(record_id && cursor_ptr)
    {
        if(record_id == cursor_ptr->record_id)
        {
            list_ptr = cursor_ptr;
            s_mms_list.autolist_head_ptr = cursor_ptr->next;
            SCI_FREE(list_ptr);
        }
        else
        {
            while(cursor_ptr->next)
            {
                if(record_id == cursor_ptr->next->record_id)
                {
                    list_ptr = cursor_ptr->next;
                    cursor_ptr->next = cursor_ptr->next->next;
                    SCI_FREE(list_ptr);
                    break;
                }
                cursor_ptr = cursor_ptr->next;
            }
        }
    }
}

/*****************************************************************************/
//  Description : 删除手动收发链表的条目
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMIMMS_DeleteManualListItem(int32 record_id)
{
    MMIMMS_AUTOLIST_T   *cursor_ptr = s_mms_list.manuallist_head_ptr;
    MMIMMS_AUTOLIST_T   *list_ptr = PNULL;

    if(record_id && cursor_ptr)
    {
        if(record_id == cursor_ptr->record_id)
        {
            list_ptr = cursor_ptr;
            s_mms_list.manuallist_head_ptr = cursor_ptr->next;
            SCI_FREE(list_ptr);
        }
        else
        {
            while(cursor_ptr->next)
            {
                if(record_id == cursor_ptr->next->record_id)
                {
                    list_ptr = cursor_ptr->next;
                    cursor_ptr->next = cursor_ptr->next->next;
                    SCI_FREE(list_ptr);
                    break;
                }
                cursor_ptr = cursor_ptr->next;
            }
        }
    }
}

/*****************************************************************************/
//  Description : 释放整个自动下载链表与手动收发链表
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMIMMS_FreeSendRecvList(void)
{
    MMIMMS_AUTOLIST_T* cursor_ptr = PNULL;

    while(s_mms_list.manuallist_head_ptr)
    {
        cursor_ptr = s_mms_list.manuallist_head_ptr;
        s_mms_list.manuallist_head_ptr = s_mms_list.manuallist_head_ptr->next;
        if(MMIAPIMMS_IsProcessMMSById(cursor_ptr->record_id))
        {
            MMIMMS_CancelSendRecvById(cursor_ptr->record_id);
        }
        SCI_FREE(cursor_ptr);
    }

    while(s_mms_list.autolist_head_ptr)
    {
        cursor_ptr = s_mms_list.autolist_head_ptr;
        s_mms_list.autolist_head_ptr = s_mms_list.autolist_head_ptr->next;
        if(MMIAPIMMS_IsProcessMMSById(cursor_ptr->record_id))
        {
            MMIMMS_CancelSendRecvById(cursor_ptr->record_id);
        }
        SCI_FREE(cursor_ptr);
    }
}

/*****************************************************************************/
//  Description : 下载链表是否为空
//  Global resource dependence : none
//  Author: minghu.mao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMMS_IsSendRecvListEmpty(void)
{
    if (PNULL == s_mms_list.manuallist_head_ptr && PNULL == s_mms_list.autolist_head_ptr)
    {
        return TRUE;
    }
    return FALSE;
}

/*****************************************************************************/
//  Description : 是否到达最大数量
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMMS_IsMaxNum(void)
{
    if(s_mms_list.total_num < MMIMMS_MAX_MMS_NUM)
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

/*****************************************************************************/
//  Description : read mms list file to memory 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMIMMS_ReadMMSListFile(void)
{
    MMIFILE_HANDLE  file_handle = 0;
    BOOLEAN         readlist_result = FALSE;
    uint32          i = 0;
    wchar           list_path[MMIFILE_FULL_PATH_MAX_LEN+1] = {0};
    uint16          list_path_len = MMIFILE_FULL_PATH_MAX_LEN;
    MMIMMS_LIST_FILE_HEAD_T list_head = {0};
    MMISMS_ORDER_INFO_T     list_item = {0};
    uint32          list_len = 0;
    uint32          read_size = 0;
    uint8           *buffer_ptr = PNULL;
    uint32          load_num = 0;
    uint32          load_size = 0;
    uint32          rest_num = 0;
    uint32          file_size = 0;
    uint32          write_size = 0;
    MMIFILE_HANDLE  file_handle_wr = 0;

    if(GetMMSListPath(MMIMMS_LIST_NAME,MMIMMS_LIST_NAME_LEN,list_path, &list_path_len))
    {
        //打开文件
        file_handle = MMIAPIFMM_CreateFile(list_path, SFS_MODE_READ|SFS_MODE_OPEN_EXISTING, NULL, NULL);/*lint !e655*/
        
        //读取head info
        if(SFS_INVALID_HANDLE != file_handle)
        {
            if(SFS_ERROR_NONE == MMIAPIFMM_ReadFile(
                file_handle, 
                &list_head, 
                sizeof(MMIMMS_LIST_FILE_HEAD_T), 
                &read_size, 
                NULL
                ))
            {
                //SCI_TRACE_LOW:"[MMIMMS]:MMIMMS_ReadMMSListFile read_size=%d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_FILE_858_112_2_18_2_38_21_150,(uint8*)"d",read_size);
                file_size = MMIAPIFMM_GetFileSize(file_handle);
                //检验版本是否正确，数据是否完整
                if(0 == MMIAPICOM_Stricmp((uint8 *)list_head.version, (uint8 *)MMIMMS_LIST_VER)
                    && MMIMMS_MAX_MMS_NUM >= list_head.total_num
                    &&file_size ==(sizeof(MMIMMS_LIST_FILE_HEAD_T)+MMIMMS_MAX_MMS_NUM*sizeof(MMISMS_ORDER_INFO_T)))
                {
                    readlist_result = TRUE;
                }
                else
                {
                    //SCI_TRACE_LOW:"[MMIMMS]:MMIMMS_ReadMMSListFile mms version error. clean all..."
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_FILE_869_112_2_18_2_38_21_151,(uint8*)"");
                    //版本出错，清空文件夹
                    MMIAPIFMM_CloseFile(file_handle);
                    MMIAPIMMS_DelUserInfo();
                }
            }
        }
        else//两种情况，一种是系统无法分析handle，一种是不存在list的ini文件
        {
            file_handle_wr = MMIAPIFMM_CreateFile(list_path, SFS_MODE_WRITE|SFS_MODE_CREATE_ALWAYS, NULL, NULL);/*lint !e655*/
            if (PNULL != file_handle_wr)//能够创建handle，表明是list不存在，创建list成功了
            {                
                SCI_MEMCPY(list_head.version,MMIMMS_LIST_VER,MMIMMS_LIST_VER_LEN);
                list_head.total_num = 0;
                MMIAPIFMM_WriteFile(file_handle_wr, &list_head, sizeof(MMIMMS_LIST_FILE_HEAD_T), &write_size, PNULL);
                MMIAPIFMM_SetFileSize(file_handle_wr, (sizeof(MMIMMS_LIST_FILE_HEAD_T)+MMIMMS_MAX_MMS_NUM*sizeof(MMISMS_ORDER_INFO_T)));//设置大小
                MMIAPIFMM_CloseFile(file_handle_wr);
            }
        }
    }
    if(readlist_result && list_head.total_num > 0)
    {
        list_len = sizeof(MMISMS_ORDER_INFO_T);
        load_num = MIN(MMIMMS_BUFFER_LIST_NUM,list_head.total_num);
        load_size = load_num * list_len;
        buffer_ptr = SCI_ALLOCAZ(load_size);
        if (PNULL == buffer_ptr)
        {
            if(PNULL != file_handle)
            {
                MMIAPIFMM_CloseFile(file_handle);
            }
            //SCI_TRACE_LOW:"[MMIMMS]:MMIMMS_ReadMMSListFile PNULL == buffer_ptr"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_FILE_901_112_2_18_2_38_21_152,(uint8*)"");
            return;
        }
        rest_num = list_head.total_num;
        while(rest_num > 0)
        {
            load_num = MIN(MMIMMS_BUFFER_LIST_NUM,rest_num);
            rest_num -= load_num; 
            load_size = load_num * list_len;
            if(SFS_ERROR_NONE == MMIAPIFMM_ReadFile(file_handle, buffer_ptr, load_size, &read_size, NULL))
            {
                for(i=0; i<load_num; i++)
                {
                    SCI_MEMCPY(&list_item,(buffer_ptr+i*list_len),list_len);
                    AddListItem(&list_item,FALSE);
                } 
            }
            else
            {
                //链表出错，清空文件夹
                //SCI_TRACE_LOW:"[MMIMMS]:MMIMMS_ReadMMSListFile mms list error. clean all..."
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_FILE_921_112_2_18_2_38_21_153,(uint8*)"");
                if(PNULL != file_handle)
                {
                    MMIAPIFMM_CloseFile(file_handle);
                }
                MMIAPIMMS_DelUserInfo();
                break;
            }
        }
        SCI_FREE(buffer_ptr);
    }
    if(PNULL != file_handle)
    {
        MMIAPIFMM_CloseFile(file_handle);
    }
}

/*****************************************************************************/
//  Description : 清除短彩合一中的所有彩信条目,释放整个彩信链表,同时清除收发链表
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMIMMS_FreeMMSList(void)
{
    MMISMS_ORDER_INFO_T* cursor_ptr = PNULL;

    //清除收发链表
    MMIMMS_FreeSendRecvList();

    while(s_mms_list.list_head_ptr)
    {
        cursor_ptr = s_mms_list.list_head_ptr;
        s_mms_list.list_head_ptr = s_mms_list.list_head_ptr->next;
        MMIAPISMS_DeleteMsgInorder(MMISMS_TYPE_MMS,cursor_ptr->record_id);
        SCI_FREE(cursor_ptr);
    }
    SCI_MEMSET(&s_mms_list,0,sizeof(MMIMMS_LISTINFO_T));
}

/*****************************************************************************/
//  Description : insert all mms to sms order list 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMIMMS_InsertMMSListToOrderList(void)
{
    MMISMS_ORDER_INFO_T* cursor_ptr = s_mms_list.list_head_ptr;

    while(PNULL != cursor_ptr)
    {
        MMIAPISMS_InsertOneMsgToOrder(cursor_ptr);
        cursor_ptr = cursor_ptr->next;
    }
}

/*****************************************************************************/
//  Description : insert one mms to sms order list 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMMS_InsertMMSItemToOrderList(
                                            MMISMS_ORDER_INFO_T *order_ptr,
                                            int32				record_id
                                            )
{
    BOOLEAN result = FALSE;

    if(PNULL == order_ptr)
    {
        order_ptr = MMIMMS_GetListItemByID(record_id);
    }

    if(PNULL != order_ptr)
    {
        result = MMIAPISMS_InsertOneMsgToOrder(order_ptr);
    }
    return result;
}

/*****************************************************************************/
//  Description : read push file 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMMS_ReadPush(
                               MMIMMS_PUSH_FILE_T * push_file_ptr,
                               int32				record_id
                               )
{
    uint8 *fname = PNULL;
    wchar *w_fname = PNULL;
    uint16 file_len = 0;
    wchar *mms_file_path = PNULL;
    uint16 mms_file_path_len = MMIFILE_FULL_PATH_MAX_LEN;
    uint32 file_size = 0;

    if (PNULL == push_file_ptr)
    {
        return FALSE;
    }
    fname = (uint8 *)SCI_ALLOCAZ(MMIMMS_FILE_NAME_LEN + 1);
    w_fname = (wchar *)SCI_ALLOCAZ((MMIMMS_FILE_NAME_LEN + 1) * sizeof(wchar));
    mms_file_path = (wchar *)SCI_ALLOCAZ((MMIFILE_FULL_PATH_MAX_LEN + 1) * sizeof(wchar));
    if (PNULL == fname || PNULL == w_fname || PNULL == mms_file_path)
    {
        if (PNULL != fname)
        {
            SCI_FREE(fname);
            fname = PNULL;
        }
        if (PNULL != w_fname)
        {
            SCI_FREE(w_fname);
            w_fname = PNULL;
        }
        if (PNULL != mms_file_path)
        {
            SCI_FREE(mms_file_path);
            mms_file_path = PNULL;
        }
        //SCI_TRACE_LOW:"[MMIMMS]: MMIMMS_ReadPush malloc mem failed"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_FILE_1037_112_2_18_2_38_21_154,(uint8*)"");
        return FALSE;
    }
    //SCI_MEMSET(fname, 0, (MMIMMS_FILE_NAME_LEN + 1));
    //SCI_MEMSET(w_fname, 0, ((MMIMMS_FILE_NAME_LEN + 1) * sizeof(wchar)));
    //SCI_MEMSET(mms_file_path, 0, ((MMIFILE_FULL_PATH_MAX_LEN + 1) * sizeof(wchar)));
    
    if(0 != record_id)
    {
        _snprintf((char *)fname, (MMIMMS_FILE_NAME_LEN - 1), "%ld.push", record_id);
        file_len = SCI_STRLEN((char *)fname);
        MMI_STRNTOWSTR(w_fname,MMIMMS_FILE_NAME_LEN,(uint8 *)fname,MMIMMS_FILE_NAME_LEN,file_len);
        if(GetMMSFilePathForRead(w_fname,file_len,mms_file_path, &mms_file_path_len))
        {
            file_size = sizeof(MMIMMS_PUSH_FILE_T);
            MMIAPIFMM_ReadFilesDataSyn(mms_file_path,mms_file_path_len,(uint8*)push_file_ptr,file_size);
        } 
        else
        {
            //SCI_TRACE_LOW:"[MMIMMS]: MMIMMS_ReadPush file is not exist!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_FILE_1056_112_2_18_2_38_21_155,(uint8*)"");
            return FALSE;
        }
    }
    else
    {
        //SCI_TRACE_LOW:"[MMIMMS]: MMIMMS_ReadPush fetal error record_id==0!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_FILE_1062_112_2_18_2_38_21_156,(uint8*)"");
        return FALSE;
    }
    
    SCI_FREE(fname);
    fname = PNULL;

    SCI_FREE(w_fname);
    w_fname = PNULL;

    SCI_FREE(mms_file_path);
    mms_file_path = PNULL;
    
    return TRUE;
}

/*****************************************************************************/
//  Description : read mms file 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC MMIMMS_EDIT_DOCUMENT_T * MMIMMS_ReadMMS(
                                               int32				record_id
                                               )

{
    uint8                   fname [MMIMMS_FILE_NAME_LEN] = {0};
    wchar                   w_fname[MMIMMS_FILE_NAME_LEN] = {0};
    uint16                  file_len = 0;
    wchar                   mms_file_path [MMIFILE_FULL_PATH_MAX_LEN+1] = {0};
    uint16                  mms_file_path_len = MMIFILE_FULL_PATH_MAX_LEN;
    uint32                  file_size = 0;
    uint8                   *buffer_ptr = PNULL;
    FILE_HANDLE             vf_handle = 0;
//  MMIFILE_HANDLE          file_handle = NULL;
    MMIMMS_EDIT_DOCUMENT_T  *editdoc_ptr = PNULL;
//  uint16                  subject_len = 0;
    MMISMS_ORDER_INFO_T     *cursor_ptr = PNULL;
    P_MMS_SLIDE_T           slide = PNULL;

    if(0 != record_id)
    {
        _snprintf((char *)fname,(MMIMMS_FILE_NAME_LEN-1),"%ld.mms",record_id);
        file_len = SCI_STRLEN((char *)fname);
        MMI_STRNTOWSTR(w_fname,MMIMMS_FILE_NAME_LEN,(uint8 *)fname,MMIMMS_FILE_NAME_LEN,file_len);
        if(GetMMSFilePathForRead(w_fname,file_len,mms_file_path, &mms_file_path_len))
        {
            MMIAPIFMM_GetFileInfo(mms_file_path, mms_file_path_len, &file_size, NULL, NULL);
        }
        if(file_size > 0)// && file_size <= MMIMMS_MMS_LIMIT_SIZE + MMIMMS_ENCODE_LEFT_SIZE)
        {
            //SCI_TRACE_LOW:"[MMIMMS]:MMIMMS_ReadMMS record_id=%d,file_size=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_FILE_1112_112_2_18_2_38_21_157,(uint8*)"dd",record_id,file_size);
            buffer_ptr = (uint8*)MMIMMS_ALLOC(file_size);
            if(PNULL != buffer_ptr)
            {
                if(MMIAPIFMM_ReadFilesDataSyn(mms_file_path,mms_file_path_len,buffer_ptr,file_size))
                {
                    vf_handle = sysVFopen((int8*)fname, "wb");
                    if(vf_handle)
                    {
                        sysVFwrite((int8*)buffer_ptr,file_size, 1, vf_handle);
                        //Free tmp buffer
                        MMIMMS_FREE(buffer_ptr);
                        buffer_ptr = PNULL; 
                        sysVFclose(vf_handle);
                        editdoc_ptr = (MMIMMS_EDIT_DOCUMENT_T*)MMIMMS_ALLOC(sizeof(MMIMMS_EDIT_DOCUMENT_T));
                        if(PNULL != editdoc_ptr)
                        {
                            SCI_MEMSET(editdoc_ptr,0,sizeof(MMIMMS_EDIT_DOCUMENT_T));
                            MMIMMS_ParseMMS(fname,editdoc_ptr);
                            MMIMMS_SetMMSNameListByTo(editdoc_ptr);
                            editdoc_ptr->record_id = record_id;                           
                            
                            if (PNULL == editdoc_ptr->editbody_ptr->slide_cur)
                            {
                                MMS_CreateNewSlide(editdoc_ptr->editbody_ptr);
                            }
                            cursor_ptr = MMIMMS_GetListItemByID(record_id);
                            if(PNULL != cursor_ptr)
                            {
                                if (MMISMS_MT_TO_BE_READ == cursor_ptr->mo_mt_type ||
                                    MMISMS_MT_HAVE_READ == cursor_ptr->mo_mt_type ||
                                    MMISMS_MO_SEND_SUCC == cursor_ptr->mo_mt_type ||
                                    MMISMS_MO_SEND_FAIL == cursor_ptr->mo_mt_type)
                                {
                                    editdoc_ptr->editbody_ptr->totalsize = file_size;
                                }
                                editdoc_ptr->textimage_order = (MMS_TEXT_IMAGE_ORDER_E)cursor_ptr->textimage_order;
                                slide = editdoc_ptr->editbody_ptr->slide_root;
                                for(; PNULL != slide; slide = slide->next)
                                {
                                    if(MMS_ORDER_INVALID == slide->textimage_order)
                                    {
                                        slide->textimage_order = editdoc_ptr->textimage_order;
                                    } 
                                }
                            } 
                        }
                        sysVFremove((int8 *)fname);
                    }
                }
                if (PNULL != buffer_ptr)
                {
                    MMIMMS_FREE(buffer_ptr);
                }               
            }
            else
            {
                //SCI_TRACE_LOW:"[MMIMMS]:MMIMMS_ReadMMS buffer_ptr alloc fail!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_FILE_1169_112_2_18_2_38_21_158,(uint8*)"");
            }
        }
        else
        {
            //SCI_TRACE_LOW:"[MMIMMS]:MMIMMS_ReadMMS file is wrong or not exist!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_FILE_1174_112_2_18_2_38_21_159,(uint8*)"");
        }
    }
    return editdoc_ptr;
}

/*****************************************************************************/
//  Description : read mms file 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIMMS_LoadMMSToVfile(
                                     int32      record_id
                                     )
{
    uint8 fname [MMIMMS_FILE_NAME_LEN] = {0};
    wchar w_fname[MMIMMS_FILE_NAME_LEN] = {0};
    uint16 file_len = 0;
    wchar mms_file_path [MMIFILE_FULL_PATH_MAX_LEN+1] = {0};
    uint16 mms_file_path_len = MMIFILE_FULL_PATH_MAX_LEN;
//  MMIFILE_HANDLE file_handle = NULL;
    uint32 file_size = 0;
    uint8* buffer_ptr = PNULL;
    FILE_HANDLE vf_handle = 0;

    if(0 != record_id)
    {
        _snprintf((char *)fname,(MMIMMS_FILE_NAME_LEN-1),"%ld.mms",record_id);
        if(sysFileIsExist((char *)fname))
        {
            vf_handle = sysVFopen((int8*)fname, "rb");
            if(vf_handle)
            {
                file_size = sysVFfileLength(vf_handle);
                sysVFclose(vf_handle);
            }
        }
        else    //如果虚拟文件不存在，则LOAD
        {
            file_len = SCI_STRLEN((char *)fname);
            MMI_STRNTOWSTR(w_fname,MMIMMS_FILE_NAME_LEN,(uint8 *)fname,MMIMMS_FILE_NAME_LEN,file_len);
            if(GetMMSFilePathForRead(w_fname,file_len,mms_file_path, &mms_file_path_len))
            {
                MMIAPIFMM_GetFileInfo(mms_file_path, mms_file_path_len, &file_size, NULL, NULL);
            }
            if(file_size > 0)// && file_size <= MMIMMS_MMS_LIMIT_SIZE + MMIMMS_ENCODE_LEFT_SIZE)
            {
                buffer_ptr = (uint8*)MMIMMS_ALLOC(file_size);
                if(PNULL != buffer_ptr)
                {
                    if(MMIAPIFMM_ReadFilesDataSyn(mms_file_path,mms_file_path_len,buffer_ptr,file_size))
                    {
                        if (0 == sysVFCreateByData((int8*)fname,(int8*)buffer_ptr,(int32)file_size))
                        {
                            //create virtual file fail 
                            file_size = 0;
                            MMIMMS_FREE(buffer_ptr);
                            buffer_ptr = PNULL;
                        }
                    }
                    else
                    {
                        //read file fail,free buffer
                        MMIMMS_FREE(buffer_ptr);
                        buffer_ptr = PNULL;
                    }
                }
            }
        }
    }

    return file_size;
}

/*****************************************************************************/
//  Description : 将wstr转化为mms库中的str格式（前两位存储长度，后面存储字符） 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:   |--2byte(length)--|-----------content-------|
/*****************************************************************************/
LOCAL void ConvertWstrToMMSStr(char *subject_string, 
                               uint16 size, 
                               const wchar *wstr_ptr, 
                               uint16 wstr_len
                               )
{
    uint16 subject_len = wstr_len*sizeof(wchar);
    if(subject_len>size-3)  //其中3是为了确保2byte的长度与1byte的结尾符
    {
        subject_len=size-3;
    }
    SCI_MEMCPY(subject_string,&subject_len,sizeof(uint16));
    SCI_MEMCPY((int8 *)(subject_string+2),wstr_ptr,subject_len); 
}

/*****************************************************************************/
//  Description : 将mms库中的str格式（前两位存储长度，后面存储字符） 转化为wstr
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
LOCAL void ConvertMMSstrToWStr(wchar *wstr_ptr, uint16 *wstr_len, const char *subject_string)
{
 /*   
    uint16 subject_len = 0;
    MMIMMS_SubjectAlloc();
    subject_len =Cms_UTF16Strlen(subject_string);
    g_p_mms_subject.wstr_len = subject_len;    
    MMI_WSTRNCPY(g_p_mms_subject.wstr_ptr, MMIMMS_MAX_SUBJECT_LEN, 
        subject_string+2, subject_len, subject_len);
 */       
}

/*****************************************************************************/
//  Description : integrate mms head and body
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMMS_IntegrateMMS(
                                   MMIMMS_EDIT_DOCUMENT_T * editdoc_ptr
                                   )
{
    uint8 fname [MMIMMS_FILE_NAME_LEN] = {0};
//  wchar wfname[MMIMMS_FILE_NAME_LEN] = {0};
    MMS_DOCUMENT_HANDLE  pMms_Doc = NULL;
//  BOOLEAN     is_unique = FALSE;
    uint16      file_len = 0;
//  uint16      count = 1;
    uint8       *phone_list_ptr = PNULL;
    uint8       *subject_ptr = PNULL;
    uint16      subject_size = 0;
    int32       record_id = 0;

    if(PNULL == editdoc_ptr)
    {
        return FALSE;
    }

    if(0 == editdoc_ptr->record_id)
    {
        file_len = MMIMMS_GenUniName(fname,(MMIMMS_FILE_NAME_LEN-MMIMMS_SUFFIX_LEN));
        if(file_len > 0)
        {
            record_id = atoi((char *)fname);
        }
        else
        {
            return FALSE;
        }
        strncat((char *)fname,".mms",SCI_STRLEN(".mms"));
        file_len += SCI_STRLEN(".mms");
    }
    else
    { 
        record_id = editdoc_ptr->record_id;
        _snprintf((char *)fname,(MMIMMS_FILE_NAME_LEN-1),"%ld.mms",record_id);
        file_len = SCI_STRLEN((char *)fname);
    }
    //SCI_TRACE_LOW:"[MMIMMS]:IntegrateMMS record_id=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_FILE_1326_112_2_18_2_38_22_160,(uint8*)"d",record_id);
    pMms_Doc = MMS_IntegrateMmsBody(editdoc_ptr->editbody_ptr);
    if(PNULL == pMms_Doc)
    {
        return FALSE;
    }

    if(!MMS_SetMmsFile(pMms_Doc,(char *)fname))
	{
		//SCI_TRACE_LOW:"[MMIMMS]:IntegrateMMS MMS_SetMmsFile failed"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_FILE_1335_112_2_18_2_38_22_161,(uint8*)"");
	    MMS_DeleteMms(pMms_Doc);
        return FALSE;
    }

    if(editdoc_ptr->edit_to.wstr_ptr)
    {
        phone_list_ptr = MMIMMS_ALLOC(editdoc_ptr->edit_to.wstr_len+1);
        if(PNULL == phone_list_ptr)
        {
            MMS_DeleteMms(pMms_Doc);
            return FALSE;
        }
        SCI_MEMSET(phone_list_ptr,0,(editdoc_ptr->edit_to.wstr_len+1));
        MMI_WSTRNTOSTR((uint8 *)phone_list_ptr, editdoc_ptr->edit_to.wstr_len, editdoc_ptr->edit_to.wstr_ptr, editdoc_ptr->edit_to.wstr_len, editdoc_ptr->edit_to.wstr_len);
        MMS_SetHeadField(pMms_Doc, MMS_TO, (long)0, (char *)phone_list_ptr);
        MMIMMS_FREE(phone_list_ptr);
    }
    
#ifdef MMIMMS_SMS_IN_1_SUPPORT  //支持抄送，密送
#ifdef MMIMMS_SMS_CC_SUPPORT
    if(editdoc_ptr->edit_cc.wstr_ptr)
    {
        phone_list_ptr = MMIMMS_ALLOC(editdoc_ptr->edit_cc.wstr_len+1);
        if(PNULL == phone_list_ptr)
        {
            MMS_DeleteMms(pMms_Doc);
            return FALSE;
        }
        SCI_MEMSET(phone_list_ptr,0,(editdoc_ptr->edit_cc.wstr_len+1));
        MMI_WSTRNTOSTR((uint8 *)phone_list_ptr, editdoc_ptr->edit_cc.wstr_len, editdoc_ptr->edit_cc.wstr_ptr, editdoc_ptr->edit_cc.wstr_len, editdoc_ptr->edit_cc.wstr_len);
        MMS_SetHeadField(pMms_Doc, MMS_CC, (long)0, (char *)phone_list_ptr);
        MMIMMS_FREE(phone_list_ptr);
    }

    if(editdoc_ptr->edit_bcc.wstr_ptr)
    {
        phone_list_ptr = MMIMMS_ALLOC(editdoc_ptr->edit_bcc.wstr_len+1);
        if(PNULL == phone_list_ptr)
        {
            MMS_DeleteMms(pMms_Doc);
            return FALSE;
        }
        SCI_MEMSET(phone_list_ptr,0,(editdoc_ptr->edit_bcc.wstr_len+1));
        MMI_WSTRNTOSTR((uint8 *)phone_list_ptr, editdoc_ptr->edit_bcc.wstr_len, editdoc_ptr->edit_bcc.wstr_ptr, editdoc_ptr->edit_bcc.wstr_len, editdoc_ptr->edit_bcc.wstr_len);
        MMS_SetHeadField(pMms_Doc, MMS_BCC, (long)0, (char *)phone_list_ptr);
        MMIMMS_FREE(phone_list_ptr);
    }
#endif
#endif

    MMS_SetHeadField(pMms_Doc, MMS_MESSAGE_CLASS, (long)editdoc_ptr->message_class, (char *)0);
    
    subject_size = editdoc_ptr->edit_subject.wstr_len * sizeof(wchar) +sizeof(uint16) + 1;
    subject_ptr = MMIMMS_ALLOC(subject_size);
    if(PNULL != subject_ptr)
    {
        SCI_MEMSET(subject_ptr,0,subject_size);
        ConvertWstrToMMSStr((char *)subject_ptr,subject_size,editdoc_ptr->edit_subject.wstr_ptr,editdoc_ptr->edit_subject.wstr_len);
        MMS_SetHeadField(pMms_Doc, MMS_SUBJECT, (long)0, (char*)subject_ptr);//设置主题
        MMIMMS_FREE(subject_ptr);
    }
    //匿名发送
    MMS_SetHeadField(pMms_Doc, MMS_SENDER_VISIBILITY,(long)editdoc_ptr->sender_visibility, (char *)0);
    //优先级
    MMS_SetHeadField(pMms_Doc, MMS_PRIORITY, (long)editdoc_ptr->pritority, (char *)0);
    //送达报告
    MMS_SetHeadField(pMms_Doc, MMS_DELIVERY_REPORT, (long)editdoc_ptr->delivery_report, (char *)0);
    //阅读报告
    MMS_SetHeadField(pMms_Doc, MMS_READ_REPORT, (long)editdoc_ptr->read_report, (char *)0);
    //有效期
    if(editdoc_ptr->expiry > 0)
    {
        MMS_SetHeadField(pMms_Doc, MMS_EXPIRY, (long)editdoc_ptr->expiry, (char *)0);
    }

    if(!MMS_EncodeMms(pMms_Doc))
	{
		//SCI_TRACE_LOW:"[MMIMMS]:IntegrateMMS MMS_EncodeMms failed"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_FILE_1413_112_2_18_2_38_22_162,(uint8*)"");
        MMS_DeleteMms(pMms_Doc);
 		sysVFremove((char *)fname);
		return FALSE;
    }
    MMS_DeleteMms(pMms_Doc);
    editdoc_ptr->record_id = record_id;
    return TRUE;
}

/*****************************************************************************/
//  Description : Parse MMS, get mms body and head  
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMMS_ParseMMS(
                               uint8 *fname,
                               MMIMMS_EDIT_DOCUMENT_T * editdoc_ptr
                               )
{
    BOOLEAN         return_val = FALSE;
    uint16          str_len = 0;
    MMS_DOCUMENT_HANDLE mmsdoc =0;
    uint8           buffer[LOCAL_STR_HUNDRED_LEN] = {0};
    char            *receiver_str = NULL;
    #ifdef MMIMMS_SMS_IN_1_SUPPORT
	#ifdef MMIMMS_SMS_CC_SUPPORT
    char            *cc_str = NULL;
    char            *bcc_str = NULL;
	#endif
    #endif
    
    mmsdoc = MMS_CreateNewMms();
    if(mmsdoc)
    {
        MMS_SetMmsFile(mmsdoc, (char *)fname);
        MMS_DecodeMms(mmsdoc);
        
        editdoc_ptr->editbody_ptr = MMS_CreateNewEditDoc();
        MMS_GetEditBody(mmsdoc,editdoc_ptr->editbody_ptr);
        MMS_GetHeadField(mmsdoc, MMS_SUBJECT, 0, (char *)buffer);
        if(0 != buffer[0] || 0 != buffer[1])  /*lint !e415*/
        {
            str_len = *(uint16*)buffer;
            str_len = MIN(str_len/2, MMIMMS_MAX_SUBJECT_LEN);
            //                str_len = encodingUtf16toUtf8Len(((buffer+2));
            editdoc_ptr->edit_subject.wstr_ptr = MMIMMS_ALLOC(MMIMMS_MAX_SUBJECT_LEN*sizeof(wchar));
            if(editdoc_ptr->edit_subject.wstr_ptr)
            {
                SCI_MEMSET(editdoc_ptr->edit_subject.wstr_ptr,0,(MMIMMS_MAX_SUBJECT_LEN*sizeof(wchar)));
                SCI_MEMCPY(editdoc_ptr->edit_subject.wstr_ptr,(buffer+2),str_len*2);
                editdoc_ptr->edit_subject.wstr_len = str_len;
            }
        }
        
        MMS_GetHeadField(mmsdoc, MMS_TO, 0, (char *)&receiver_str);
        if(receiver_str)
        {
            str_len = SCI_STRLEN(receiver_str);
            editdoc_ptr->edit_to.wstr_ptr = MMIMMS_ALLOC((str_len + 1)*sizeof(wchar));
            if(editdoc_ptr->edit_to.wstr_ptr)
            {
                MMIAPICOM_StrToWstr((uint8 *)receiver_str,editdoc_ptr->edit_to.wstr_ptr);
                editdoc_ptr->edit_to.wstr_len = str_len;
            }
            MMIMMS_FREE(receiver_str);
            receiver_str = NULL;
        }
        
    #ifdef MMIMMS_SMS_IN_1_SUPPORT
	#ifdef MMIMMS_SMS_CC_SUPPORT
        MMS_GetHeadField(mmsdoc, MMS_CC, 0, (char *)&cc_str);
        if(cc_str)
        {
            str_len = SCI_STRLEN(cc_str);
            editdoc_ptr->edit_cc.wstr_ptr = MMIMMS_ALLOC((str_len + 1)*sizeof(wchar));
            if(editdoc_ptr->edit_cc.wstr_ptr)
            {
                MMIAPICOM_StrToWstr((uint8 *)cc_str,editdoc_ptr->edit_cc.wstr_ptr);
                editdoc_ptr->edit_cc.wstr_len = str_len;
            }
            MMIMMS_FREE(cc_str);
            cc_str = NULL;
        }

        MMS_GetHeadField(mmsdoc, MMS_BCC, 0, (char *)&bcc_str);
        if(bcc_str)
        {
            str_len = SCI_STRLEN(bcc_str);
            editdoc_ptr->edit_bcc.wstr_ptr = MMIMMS_ALLOC((str_len + 1)*sizeof(wchar));
            if(editdoc_ptr->edit_bcc.wstr_ptr)
            {
                MMIAPICOM_StrToWstr((uint8 *)bcc_str,editdoc_ptr->edit_bcc.wstr_ptr);
                editdoc_ptr->edit_bcc.wstr_len = str_len;
            }
            MMIMMS_FREE(bcc_str);
            bcc_str = NULL;
        }
    #endif		
    #endif
        
        MMS_GetHeadField(mmsdoc, MMS_TRANSACTION_ID, 0, (char *)buffer);
        if(buffer[0])
        {
            str_len = SCI_STRLEN((char *)buffer);
            editdoc_ptr->msg_id.wstr_ptr = MMIMMS_ALLOC((str_len + 1)*sizeof(wchar));
            if(editdoc_ptr->msg_id.wstr_ptr)
            {
                MMIAPICOM_StrToWstr((uint8 *)buffer,editdoc_ptr->msg_id.wstr_ptr);
                editdoc_ptr->msg_id.wstr_len = str_len;
            }
        }

        MMS_GetHeadField(mmsdoc, MMS_PRIORITY, (int32 *)&editdoc_ptr->pritority, (char *)0);
        MMS_GetHeadField(mmsdoc, MMS_READ_REPORT, (int32 *)&editdoc_ptr->read_report, (char *)0);

        MMS_DeleteMms(mmsdoc);
        return_val = TRUE;
    } 

    return return_val;
}

/*****************************************************************************/
//  Description : decode received mms 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note: 保存LIST ，保存MMS，删除PUSH
/*****************************************************************************/
PUBLIC BOOLEAN MMIMMS_UpdateRecvItem(int32 record_id)
{
    BOOLEAN             return_val = FALSE;
    MMISMS_ORDER_INFO_T *cursor_ptr = PNULL;
    MMS_DOCUMENT_HANDLE mmsdoc =0;
    uint8               fname[MMIMMS_FILE_NAME_LEN] = {0};
    uint16              file_len = 0;
    wchar               w_fname [MMIMMS_FILE_NAME_LEN] = {0};
    wchar               mms_file_path[MMIFILE_FULL_PATH_MAX_LEN+1] = {0};
    uint16              mms_file_path_len = MMIFILE_FULL_PATH_MAX_LEN;
//  uint8               *vfilebuf = NULL;
//  int32               vfilebuf_len = 0;
#ifdef WORLD_CLOCK_SUPPORT
    float               time_zone = 0;
#endif    
    int32               ldate = 0;
    char                buffer[MMIMMS_DECODEHEAD_BUFFER_LEN] = {0};
    uint32              file_size = 0;
	char	            phoneNumber[MMISMS_NUMBER_MAX_LEN+1]	= {0};    
    
    cursor_ptr = MMIMMS_GetListItemByID(record_id);   
    if(cursor_ptr)
    {    
        //        cursor_ptr->time = MMIAPICOM_GetCurTime();  //原时间为收到PUSH时间。
        //DECODE MMS
        _snprintf((char *)fname,(MMIMMS_FILE_NAME_LEN-1),"%ld.mms",record_id);
        file_len = SCI_STRLEN((char *)fname);
        MMI_STRNTOWSTR(w_fname,MMIMMS_FILE_NAME_LEN,(uint8 *)fname,MMIMMS_FILE_NAME_LEN,file_len);
        if(GetMMSFilePathForRead(w_fname,file_len,mms_file_path,&mms_file_path_len))
        {
            MMIAPIFMM_GetFileInfo(mms_file_path, mms_file_path_len, &file_size, NULL, NULL);
            cursor_ptr->mo_mt_type = MMISMS_MT_TO_BE_READ;
            cursor_ptr->file_size = file_size;
            s_mms_list.unsettled_num--;
            s_mms_list.unread_num++;
            mmsdoc = MMS_CreateNewMms();
            if(mmsdoc)
            {
                if(MMS_GetHeaderFromSysFile(mms_file_path,mms_file_path_len,(char *)fname))
                {                    
                    MMS_SetMmsFile(mmsdoc, (char *)fname);
                    MMS_DecodeMmsHead(mmsdoc);
                    MMS_GetHeadField(mmsdoc, MMS_DATE, &ldate, NULL);
#ifdef WORLD_CLOCK_SUPPORT
                    MMIAPIACC_GetLocalTimeZoneEx(&time_zone);
                    SCI_TRACE_LOW("[MMIMMS:]MMIMMS_UpdateRecvItem [CBK]: time_zone = %f", time_zone);
                    /* 判断时区的合法性，没有宏，用数字 */
                    if (time_zone > 12.75 || time_zone < -12.0)
                    {
                        time_zone = 8.0;
                    }
                    //这个时间是从1970年算起的，而系统是从1980年算的，需要减10年。
                    cursor_ptr->time = (uint32)ldate + (uint32)(time_zone * MMIMMS_1_HOUR_SECONDS) - MMIMMS_SECOND_1970_TO_1980;
#else
                    cursor_ptr->time = MMIAPICOM_GetCurTime();
#endif
                    SCI_TRACE_LOW("[MMIMMS:]MMIMMS_UpdateRecvItem [CBK]: ldate=%ld, cursor_ptr->time=%ld", ldate, cursor_ptr->time);

                   
                    MMS_GetHeadField(mmsdoc, MMS_SUBJECT, 0, buffer);
                    if(buffer[0] || buffer[1])  /*lint !e415*/
                    {
                        cursor_ptr->display_content.content_len = MIN(*(uint16*)buffer/2,MMIMESSAGE_DIAPLAY_LEN);
                        SCI_MEMSET(cursor_ptr->display_content.content,0,((MMIMESSAGE_DIAPLAY_LEN+1)*sizeof(wchar)));
                        SCI_MEMCPY(cursor_ptr->display_content.content,(buffer+2),cursor_ptr->display_content.content_len*2);
                    }
                    SCI_TRACE_LOW("[MMIMMS:]MMIMMS_UpdateRecvItem sender_beforeDL = s%", cursor_ptr->sender);
                	MMS_GetHeadField(mmsdoc, MMS_FROM, (int32 *)0, phoneNumber);
                    SCI_MEMSET(cursor_ptr->sender, 0, MMISMS_NUMBER_MAX_LEN);
                    SCI_MEMCPY(cursor_ptr->sender, phoneNumber, MMISMS_NUMBER_MAX_LEN);
                    SCI_TRACE_LOW("[MMIMMS:]MMIMMS_UpdateRecvItem sender_afterDL = s%", cursor_ptr->sender);
                }
                MMS_DeleteMms(mmsdoc);
            }
            //
            MMIMMS_SaveMMSListFile();
            MMIAPISMS_UpdateMsgStatusInOrder(cursor_ptr);
            
            SCI_MEMSET(fname,0,MMIMMS_FILE_NAME_LEN);
            SCI_MEMSET(w_fname,0,(MMIMMS_FILE_NAME_LEN*sizeof(wchar)));
            SCI_MEMSET(mms_file_path,0,((MMIFILE_FULL_PATH_MAX_LEN+1)*sizeof(wchar)));
            _snprintf((char *)fname,(MMIMMS_FILE_NAME_LEN-1),"%ld.push",record_id);
            file_len = SCI_STRLEN((char *)fname);
            MMI_STRNTOWSTR(w_fname,MMIMMS_FILE_NAME_LEN,(uint8 *)fname,MMIMMS_FILE_NAME_LEN,file_len);
            mms_file_path_len = MMIFILE_FULL_PATH_MAX_LEN;
            if(GetMMSFilePathForRead(w_fname,file_len,mms_file_path, &mms_file_path_len))
            {
                MMIAPIFMM_DeleteFileSyn(mms_file_path,mms_file_path_len);
            }
            return_val = TRUE;
        }
    } 
    
    return return_val;
}


/*****************************************************************************/
//  Description : change mms type
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMMS_ChangeRecordType(
                                      int32                 record_id,
                                      MMISMS_MO_MT_TYPE_E   mo_mt_type
                                      )
{
    BOOLEAN return_val = FALSE;
    MMISMS_ORDER_INFO_T* cursor_ptr = PNULL;
 // MMISMS_ORDER_INFO_T* list_item_ptr = PNULL;

    cursor_ptr= s_mms_list.list_head_ptr;
    if(0 != record_id)
    {
        while(PNULL != cursor_ptr)
        {
            if(record_id == cursor_ptr->record_id)
            {
                if(mo_mt_type != cursor_ptr->mo_mt_type)
                {
                    if(MMISMS_MT_TO_BE_READ == cursor_ptr->mo_mt_type)
                    {
                        s_mms_list.unread_num--;
                    }
                    cursor_ptr->mo_mt_type = mo_mt_type;
                    MMIMMS_SaveMMSListFile();
                    MMIAPISMS_UpdateMsgStatusInOrder(cursor_ptr);
                    
                }
                return_val = TRUE;
                break;
            }
            cursor_ptr = cursor_ptr->next;
        }
    }
    return return_val;
}


/*****************************************************************************/
//  Description :
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMMS_IsRecordIDExist(int32 record_id)
{
    BOOLEAN return_val = FALSE;
    MMISMS_ORDER_INFO_T* cursor_ptr = PNULL;

    cursor_ptr= s_mms_list.list_head_ptr;
    if(0 != record_id)
    {
        while(PNULL != cursor_ptr)
        {
            if(record_id == cursor_ptr->record_id)
            {
                return_val = TRUE;
                break;
            }
            cursor_ptr = cursor_ptr->next;
        }
    }
    return return_val;
}


/*****************************************************************************/
//  Description : save mms list file 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMMS_SaveMMSListFile(void)
{
    MMIFILE_HANDLE  file_handle = SFS_INVALID_HANDLE;
    BOOLEAN         return_val = FALSE;
    uint32          i = 0;
    wchar           list_path[MMIFILE_FULL_PATH_MAX_LEN+1] = {0};
    uint16          list_path_len = MMIFILE_FULL_PATH_MAX_LEN;
    MMIMMS_LIST_FILE_HEAD_T list_head = {0};
//  MMISMS_ORDER_INFO_T     list_item = {0};
    uint32          list_len = 0;
    uint32          write_size = 0;
    uint8           *buffer_ptr = PNULL;
    uint32          load_num = 0;
    uint32          load_size = 0;
    uint32          rest_num = 0;
    MMISMS_ORDER_INFO_T* cursor_ptr = PNULL;

    if(GetMMSListPath(MMIMMS_LIST_NAME,MMIMMS_LIST_NAME_LEN,list_path, &list_path_len))
    {
        file_handle = MMIAPIFMM_CreateFile(list_path, SFS_MODE_WRITE|SFS_MODE_CREATE_ALWAYS, NULL, NULL);/*lint !e655*/
        
        if(SFS_INVALID_HANDLE != file_handle)
        {
            SCI_MEMCPY(list_head.version,MMIMMS_LIST_VER,MMIMMS_LIST_VER_LEN);
            list_head.total_num = s_mms_list.total_num;
			//SCI_TRACE_LOW:"[MMIMMS]:%x %x %x %x %x %x %x %x"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_FILE_1726_112_2_18_2_38_23_163,(uint8*)"dddddddd",((uint8*)&list_head)[0],((uint8*)&list_head)[1],((uint8*)&list_head)[2],((uint8*)&list_head)[3],((uint8*)&list_head)[14],((uint8*)&list_head)[15],((uint8*)&list_head)[16],((uint8*)&list_head)[17]);
            MMIAPIFMM_WriteFile(file_handle, &list_head, sizeof(MMIMMS_LIST_FILE_HEAD_T), &write_size, PNULL);
            MMIAPIFMM_SetFileSize(file_handle, (sizeof(MMIMMS_LIST_FILE_HEAD_T)+MMIMMS_MAX_MMS_NUM*sizeof(MMISMS_ORDER_INFO_T)));//设置大小s
            return_val = TRUE;
            if(s_mms_list.total_num > 0)
            {
                list_len = sizeof(MMISMS_ORDER_INFO_T);
                load_num = MIN(MMIMMS_BUFFER_LIST_NUM,list_head.total_num);
                load_size = load_num * list_len;
                buffer_ptr = SCI_ALLOCAZ(load_size);
                if (PNULL == buffer_ptr)
                {
                    MMIAPIFMM_CloseFile(file_handle);
                    //SCI_TRACE_LOW:"[MMIMMS]:MMIMMS_SaveMMSListFile PNULL == buffer_ptr"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_FILE_1739_112_2_18_2_38_23_164,(uint8*)"");
                    return FALSE;
                }
                rest_num = list_head.total_num;
                cursor_ptr= s_mms_list.list_head_ptr;
                while(rest_num > 0)
                {
                    load_num = MIN(MMIMMS_BUFFER_LIST_NUM,rest_num);
                    rest_num -= load_num; 
                    load_size = load_num *sizeof(MMISMS_ORDER_INFO_T);
                    for(i=0; i<load_num; i++)
                    {
                        SCI_MEMCPY((buffer_ptr+i*list_len),cursor_ptr,list_len);
                        cursor_ptr = cursor_ptr->next;
                    }
					//SCI_TRACE_LOW:"[MMIMMS]:%x %x %x %x %x %x %x %x"
					SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_FILE_1757_112_2_18_2_38_23_165,(uint8*)"dddddddd",buffer_ptr[0],buffer_ptr[1],buffer_ptr[2],buffer_ptr[3],buffer_ptr[4],buffer_ptr[5],buffer_ptr[6],buffer_ptr[7]);
                    if(SFS_ERROR_NONE != MMIAPIFMM_WriteFile(file_handle, buffer_ptr, load_size, &write_size, PNULL))
                    {
                        //链表写出错
                        //SCI_TRACE_LOW:"[MMIMMS]:MMIMMS_SaveMMSListFile FAIL, fetal error!"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_FILE_1761_112_2_18_2_38_23_166,(uint8*)"");
                        return_val = FALSE;
                        break;
                    }  
                }
                SCI_FREE(buffer_ptr);
            }
        }
    }
    MMIAPIFMM_CloseFile(file_handle);

    return return_val;
}

/*****************************************************************************/
//  Description : save push file 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC int32 MMIMMS_SavePush(
                             MN_DUAL_SYS_E dual_sys ,
                             MMISMS_MO_MT_TYPE_E  mo_mt_type,
                             const MMS_NOTIFICATION_IND_MSG_T *msg_ptr,
                             const uint8 *sender_ptr
                             )
{
    uint8       fname[MMIMMS_FILE_NAME_LEN] = {0};
    uint16      file_len = 0;
    wchar       w_fname[MMIMMS_FILE_NAME_LEN] = {0};
    wchar       mms_file_path[MMIFILE_FULL_PATH_MAX_LEN+1] = {0};
    uint16      mms_file_path_len = MMIFILE_FULL_PATH_MAX_LEN;
    int32       record_id = 0;
    MMISMS_ORDER_INFO_T list_item = {0};
    MMIMMS_PUSH_FILE_T *push_info_ptr = PNULL;
    uint32              str_len  = 0;
    uint16 len = 0;
    uint32 i = 0;
    uint16 num_len = 0;
#if defined(MMI_BLACKLIST_SUPPORT)
    MMIPB_BCD_NUMBER_T black_bcd_num = {0};
    MMI_PARTY_NUMBER_T  party_num = {MN_NUM_TYPE_UNKNOW, 0, 0};
#endif

#if defined(MMI_BLACKLIST_SUPPORT)
    if (PNULL != msg_ptr->phone_number)
    {
        num_len = SCI_STRLEN(msg_ptr->phone_number);
        if (0 < num_len && MMIAPICOM_GenPartyNumber((uint8 *)msg_ptr->phone_number, num_len, &party_num))
        {
            black_bcd_num.npi_ton = MMIAPICOM_ChangeTypePlanToUint8(party_num.number_type, MN_NUM_PLAN_UNKNOW);
            black_bcd_num.number_len = (uint8)MIN(MMIPB_BCD_NUMBER_MAX_LEN, party_num.num_len);
            SCI_MEMCPY(black_bcd_num.number, party_num.bcd_num, black_bcd_num.number_len);
            if(MMIAPISET_IsSMSBlacklist(&black_bcd_num))
            {
                //SCI_TRACE_LOW:"[MMIMMS]:MMIMMS_SavePush mms sender is in black lists!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_FILE_1814_112_2_18_2_38_23_167,(uint8*)"");
                return -1;
            }
        }
        else
        {
            //SCI_TRACE_LOW:"[MMIMMS]:MMIMMS_SavePush num_len is d%!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_FILE_1820_112_2_18_2_38_23_168,(uint8*)"", num_len);
        }
    }
    else
    {
        //SCI_TRACE_LOW:"[MMIMMS]:MMIMMS_SavePush msg_ptr->phone_number is PNULL!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_FILE_1825_112_2_18_2_38_23_169,(uint8*)"");
    }
#endif

    file_len = MMIMMS_GenUniName(fname,(MMIMMS_FILE_NAME_LEN-MMIMMS_SUFFIX_LEN));
    if(file_len > 0)
    {
        record_id = atoi((char *)fname);
    }
    else
    {
        return 0;
    }
    _snprintf((char *)(fname+file_len),(MMIMMS_FILE_NAME_LEN-file_len-1),"%s",".push");
    file_len += SCI_STRLEN(".push");

    MMI_STRNTOWSTR(w_fname,MMIMMS_FILE_NAME_LEN,(uint8 *)fname,MMIMMS_FILE_NAME_LEN,file_len);
    if(!MMIMMS_GetMMSFilePathForWrite(w_fname,file_len,mms_file_path, &mms_file_path_len,MMIMMS_RESERVE_LIST_SIZE))
    {
        //SCI_TRACE_LOW:"[MMIMMS]:MMIMMS_SavePush not enough space for list!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_FILE_1844_112_2_18_2_38_23_170,(uint8*)"");
        return 0;
    }

    push_info_ptr = (MMIMMS_PUSH_FILE_T*)SCI_ALLOCAZ(sizeof(MMIMMS_PUSH_FILE_T));
    if (PNULL == push_info_ptr)
    {
        //SCI_TRACE_LOW:"[MMIMMS]:MMIMMS_SavePush read push error alloc failed!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_FILE_1851_112_2_18_2_38_23_171,(uint8*)"");
        return 0;
    }
    //SCI_MEMSET((uint8*)push_info_ptr, 0, sizeof(MMIMMS_PUSH_FILE_T));
    push_info_ptr->size = (uint32)msg_ptr->msgsize;
    push_info_ptr->expiry_time = (uint32)msg_ptr->expiry_time;
    len = *(uint16*)msg_ptr->subject;
    len = MIN(len/2,MMIMMS_MAX_SUBJECT_LEN);
    push_info_ptr->subject_len = len;
    SCI_MEMCPY(push_info_ptr->subject,msg_ptr->subject+2,len*2);
    len = SCI_STRLEN(msg_ptr->mms_url);
    len = MIN(len,MMS_MAX_URL_LENGTH);
    SCI_MEMCPY(push_info_ptr->url,msg_ptr->mms_url,len);

    if (sender_ptr != PNULL)
    {
        num_len = SCI_STRLEN(sender_ptr);
        num_len = MAX(num_len, MMIMMS_MAX_ADDRESS_LEN);
        SCI_MEMCPY((char *)push_info_ptr->sender, sender_ptr, num_len);
    }    

    //SCI_TRACE_LOW:"[MMIMMS]: MMIMMS_SavePush record_id=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_FILE_1872_112_2_18_2_38_23_172,(uint8*)"d",record_id);
	//SCI_TRACE_LOW:"[MMIMMS]: %x %x %x %x %x %x %x %x "
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_FILE_1876_112_2_18_2_38_23_173,(uint8*)"dddddddd",((char*)push_info_ptr)[0],((char*)push_info_ptr)[1],((char*)push_info_ptr)[2],((char*)push_info_ptr)[3],((char*)push_info_ptr)[5],((char*)push_info_ptr)[6],((char*)push_info_ptr)[7],((char*)push_info_ptr)[8]);
    if(MMIAPIFMM_WriteFilesDataSyn(mms_file_path,mms_file_path_len,(uint8 *)push_info_ptr,sizeof(MMIMMS_PUSH_FILE_T)))
    {
        list_item.dual_sys = dual_sys;
        list_item.mo_mt_type = mo_mt_type;
        list_item.msg_type = MMISMS_TYPE_MMS;
        list_item.record_id = record_id;
        if(msg_ptr->subject)
        {
            if(msg_ptr->subject[i]!=0 || msg_ptr->subject[i+1]!=0)
            {
//                outlen = encodingUtf16toUtf8Len((unsigned char*)msg_ptr->subject);
//                encodingUtf16toUtf8((unsigned char*)msg_ptr->subject, outlen, outlen+1, subject);
//                list_item.display_content.content_len = GUI_UTF8ToWstr(list_item.display_content.content,MMIMESSAGE_DIAPLAY_LEN,subject,outlen);
            
                str_len = *(uint16*)msg_ptr->subject;
            //                str_len = encodingUtf16toUtf8Len(((buffer+2));
                SCI_MEMSET(list_item.display_content.content,0,((MMIMESSAGE_DIAPLAY_LEN + 1)*sizeof(wchar)));
                list_item.display_content.content_len  = MIN(str_len/2, MMIMESSAGE_DIAPLAY_LEN);
                SCI_MEMCPY(list_item.display_content.content,(msg_ptr->subject+2),list_item.display_content.content_len*2);
            }
        }
        if (msg_ptr->phone_number && msg_ptr->phone_number[0])
        {
            num_len = SCI_STRLEN(msg_ptr->phone_number);
            MMI_MEMCPY(list_item.sender,MMISMS_NUMBER_MAX_LEN,msg_ptr->phone_number,num_len,num_len);
        }
        else
        {
            SCI_STRCPY((char *)list_item.sender,(char*)"MMS");
        }
        list_item.time = MMIAPICOM_GetCurTime();
        list_item.file_size = push_info_ptr->size;
        AddListItem(&list_item,FALSE);
        MMIMMS_SaveMMSListFile();
    }
    else
    {
        //SCI_TRACE_LOW:"[MMIMMS]: MMIMMS_SavePush error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_FILE_1914_112_2_18_2_38_23_174,(uint8*)"");
        record_id = 0;
    }

    SCI_FREE(push_info_ptr);
    push_info_ptr = PNULL;
    return record_id;
}


/*****************************************************************************/
//  Description : save mms (非后台收发：vfile fname需单独释放；后台收发：本函数释放)
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC MMIMMS_ERROR_E MMIMMS_SaveMMS(
                                     MN_DUAL_SYS_E              dual_sys,
                                     MMISMS_MO_MT_TYPE_E        mo_mt_type,
                                     MMIMMS_EDIT_DOCUMENT_T *   editdoc_ptr
                                     )
{
    MMIMMS_ERROR_E return_val = MMIMMS_NO_ERROR;
    BOOLEAN     is_maxnum = FALSE;
    BOOLEAN     is_enough_space = FALSE;
    BOOLEAN     is_record_exist = FALSE;
    uint8       fname[MMIMMS_FILE_NAME_LEN] = {0};
    uint16      file_len = 0;
    wchar       w_fname[MMIMMS_FILE_NAME_LEN] = {0};
    wchar       mms_file_path[MMIFILE_FULL_PATH_MAX_LEN+1] = {0};
    uint16      mms_file_path_len = MMIFILE_FULL_PATH_MAX_LEN;
    wchar       mms_file_path_re[MMIFILE_FULL_PATH_MAX_LEN+1] = {0};
    uint16      mms_file_path_len_re = MMIFILE_FULL_PATH_MAX_LEN;
    MMS_DOCUMENT_HANDLE  pMms_Doc = NULL;
    MMISMS_ORDER_INFO_T     list_item = {0};
    uint8                   *vfilebuf = NULL;
    int32                   vfilebuf_len = 0;
    uint32                  file_size = 0;
    char*                   receiver_str = NULL;

    if(PNULL == editdoc_ptr)
    {
        return MMIMMS_ERROR_FATAL;
    }
    if (editdoc_ptr->editbody_ptr->totalsize > (int32)MMIMMS_MMS_LIMIT_SIZE)
    {
        return MMIMMS_ERROR_INVALID_SIZE;
    }
    //SCI_TRACE_LOW:"[MMIMMS]:MMIMMS_SaveMMS save mms in"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_FILE_1960_112_2_18_2_38_23_175,(uint8*)"");
    MMIMMS_SetDefaultHeadValue(editdoc_ptr);
    if(MMIMMS_IntegrateMMS(editdoc_ptr))
        
    {
        is_maxnum = MMIMMS_IsMaxNum();
        is_enough_space = MMIMMS_IsEnoughSpace((editdoc_ptr->editbody_ptr->totalsize * 3)/2);
        is_record_exist = MMIMMS_IsRecordIDExist(editdoc_ptr->record_id);
        _snprintf((char *)fname,(MMIMMS_FILE_NAME_LEN-1),"%ld.mms",editdoc_ptr->record_id);
        //SCI_TRACE_LOW:"[MMIMMS]:MMIMMS_SaveMMS no space is_record_exist=%d,is_maxnum=%d,is_enough_space=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_FILE_1969_112_2_18_2_38_23_176,(uint8*)"ddd",is_record_exist,is_maxnum,is_enough_space);
        if((is_record_exist || !is_maxnum) && is_enough_space)
        {
            if(PNULL != editdoc_ptr->edit_subject.wstr_ptr)
            {
                list_item.display_content.content_len = MIN(MMIMESSAGE_DIAPLAY_LEN, editdoc_ptr->edit_subject.wstr_len);
                MMI_WSTRNCPY(list_item.display_content.content, MMIMESSAGE_DIAPLAY_LEN, editdoc_ptr->edit_subject.wstr_ptr, list_item.display_content.content_len, list_item.display_content.content_len);
            }
            list_item.dual_sys = dual_sys;
            list_item.mo_mt_type = mo_mt_type;
            list_item.msg_type = MMISMS_TYPE_MMS;
            list_item.record_id = editdoc_ptr->record_id;
            list_item.textimage_order = (int8)editdoc_ptr->textimage_order;
            if (MMK_IsOpenWin(MMISMS_SECURITYBOX_CHILD_WIN_ID))
            {
                list_item.folder_type = MMISMS_FOLDER_SECURITY;
            }
            
            //get list sender
            pMms_Doc = MMS_CreateNewMms();
            if (PNULL != pMms_Doc)
            {
                MMS_SetMmsFile(pMms_Doc,(char*)fname);
                MMS_DecodeMmsHead(pMms_Doc);

#if 0
                
                if (MMISMS_MO_DRAFT == mo_mt_type || MMISMS_MO_SEND_FAIL == mo_mt_type)
                {
                    wchar *to_wstr                        = PNULL;
                    uint16 to_len                         = 0;
                    wchar temp_wch[]                      = L" ";
                    uint16 to_alloc_len                   = 0;
                    MMIMMS_EDIT_NUMBER_LIST_T *numberlist_ptr = PNULL;
                    MMIMMS_EDIT_NUMBER_LIST_T *temp_numberlist_ptr = PNULL;
                    BOOLEAN is_alloc                      = FALSE;
                    wchar semicolon[]                     = {';', 0};
                    
                    numberlist_ptr = editdoc_ptr->numberlist_header_ptr;
                    temp_numberlist_ptr = editdoc_ptr->numberlist_header_ptr;
                    if (PNULL != numberlist_ptr)   
                    {
                        while(PNULL != temp_numberlist_ptr)
                        {
                            to_alloc_len = to_alloc_len + MAX(temp_numberlist_ptr->number_len, temp_numberlist_ptr->number_name_len);
                            temp_numberlist_ptr = temp_numberlist_ptr->next;
                        }
                        to_alloc_len = to_alloc_len + MMIMMS_MAX_RECEIVER_NUM + 1;//分号
                        if(0 < to_alloc_len)
                        {
                            to_wstr = (wchar *)MMIMMS_ALLOC(to_alloc_len * sizeof(wchar));
                            is_alloc = TRUE;
                            SCI_MEMSET(to_wstr, 0, to_alloc_len * sizeof(wchar));
                            //SCI_TRACE_LOW:"MMSEditAddToItem %d"
                            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_FILE_2022_112_2_18_2_38_23_177,(uint8*)"d", to_alloc_len);
                            while(PNULL != numberlist_ptr)
                            {//changed for 126203 by minghu
//                                 if(numberlist_ptr->is_object && PNULL != numberlist_ptr->number_name_ptr)
//                                 {
//                                     MMIAPICOM_Wstrncpy(to_wstr + to_len, numberlist_ptr->number_name_ptr, numberlist_ptr->number_name_len);
//                                     to_len = to_len + numberlist_ptr->number_name_len;
//                                 }
//                                 else
                                {
                                    MMIAPICOM_Wstrncpy(to_wstr + to_len, numberlist_ptr->number_ptr, numberlist_ptr->number_len);
                                    to_len = to_len + numberlist_ptr->number_len;
                                }
                                MMIAPICOM_Wstrncpy(to_wstr + to_len, semicolon, MMIAPICOM_Wstrlen(semicolon));
                                to_len = to_len + MMIAPICOM_Wstrlen(semicolon);
                                numberlist_ptr = numberlist_ptr->next;
                            }            
                        }
                    }
                    else
                    {
                        to_wstr = temp_wch;
                        to_len = MMIAPICOM_Wstrlen(to_wstr);
                    }
                    to_len = MIN(to_len, MMISMS_NUMBER_MAX_LEN);
                    MMI_WSTRNTOSTR(list_item.sender, MMISMS_NUMBER_MAX_LEN, to_wstr, to_len, to_len);
                    if (is_alloc)
                    {
                        MMIMMS_FREE(to_wstr);
                        to_wstr = NULL;
                    }
                }
                else
#endif
                {
                    MMS_GetHeadField(pMms_Doc, MMS_TO, 0, (char *)&receiver_str);
                    if(PNULL != receiver_str)
                    {
                        uint16 first_num_len = 0;

                        for (first_num_len = 0; first_num_len <= MMISMS_NUMBER_MAX_LEN; first_num_len++)
                        {
                            if (';' == receiver_str[first_num_len] || 0 == receiver_str[first_num_len])
                            {
                                break;
                            }
                        }
                        first_num_len = MIN(first_num_len, MMISMS_NUMBER_MAX_LEN);
                        strncpy((char *)list_item.sender,receiver_str,first_num_len);
                        MMIMMS_FREE(receiver_str);
                        receiver_str = NULL;
                    } 
                }
                                  
                MMS_DeleteMms(pMms_Doc);
                pMms_Doc = PNULL;
            }
            
            list_item.time = MMIAPICOM_GetCurTime();
            sysVFGetConstFileData((int8 *)fname,(int8 **)&vfilebuf,(int32 *)&vfilebuf_len);
            list_item.file_size = vfilebuf_len;
            AddListItem(&list_item,TRUE);
            if(MMIMMS_SaveMMSListFile())
            {
                file_len = strlen((char *)fname);
                MMI_STRNTOWSTR(w_fname,MMIMMS_FILE_NAME_LEN,(uint8 *)fname,MMIMMS_FILE_NAME_LEN,file_len);
                if(MMIMMS_GetMMSFilePathForWrite(w_fname,file_len,mms_file_path,&mms_file_path_len,(uint32)(editdoc_ptr->editbody_ptr->totalsize + MMIMMS_RESERVE_LIST_SIZE)))
                {
                    //SCI_TRACE_LOW:"[MMIMMS]:MMIMMS_SaveMMS record_id=%d"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_FILE_2090_112_2_18_2_38_23_178,(uint8*)"d",list_item.record_id);
                    MMIAPIFMM_WriteFilesDataSyn(mms_file_path,mms_file_path_len,vfilebuf,vfilebuf_len); 
                    if(GetMMSFilePathForRead(w_fname,file_len,mms_file_path_re, &mms_file_path_len_re))
                    {
                        MMIAPIFMM_GetFileInfo(mms_file_path_re, mms_file_path_len_re, &file_size, NULL, NULL);
                        if (file_size != (uint32)vfilebuf_len || 0 != MMIAPICOM_Wstrncmp(mms_file_path, mms_file_path_re, mms_file_path_len_re))
                        {
                            MMIAPIFMM_DeleteFileSyn(mms_file_path_re,mms_file_path_len_re);
                        }
                    }
                }
                else
                {
                    //SCI_TRACE_LOW:"[MMIMMS]:MMIMMS_SaveMMS write mms file error"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_FILE_2103_112_2_18_2_38_23_179,(uint8*)"");
                    DeleteListItem(list_item.record_id);
                    return_val = MMIMMS_ERROR_NO_SPACE;
                }
            }
            else
            {
                //SCI_TRACE_LOW:"[MMIMMS]:MMIMMS_SaveMMS save mms list error"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_FILE_2110_112_2_18_2_38_23_180,(uint8*)"");
                DeleteListItem(list_item.record_id);
                return_val = MMIMMS_ERROR_SFS_ERROR;
            }
            sysVFremove((int8 *)fname);
        }
        else
        {
            //SCI_TRACE_LOW:"[MMIMMS]:MMIMMS_SaveMMS no space"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_FILE_2118_112_2_18_2_38_23_181,(uint8*)"");
            if (MMISMS_MO_SEND_FAIL != mo_mt_type)
            {
                sysVFremove((int8 *)fname);
            }
            return_val = MMIMMS_ERROR_NO_SPACE;
        }
    }
    else
    {
        //SCI_TRACE_LOW:"[MMIMMS]:MMIMMS_SaveMMS encode error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_FILE_2128_112_2_18_2_38_23_182,(uint8*)"");
        return_val = MMIMMS_ERROR_ENCODE_ERROR;
    }    
   
    return return_val;   
}

#ifdef DRM_SUPPORT
#define DRM_SPLIT_SYMBOL ","
#define DRM_SPLIT_LINE_SYMBOL "\r"
#define DRM_SPLIT_SYMBOL_LEN 5 // 4 : 3 ',' and 1 '\r' len
#define DRM_SAVED_ORIFN_LEN  4
#define DRM_SAVED_RECPATH "System\\MMS" //MMIMULTIM_DIR_SYSTEM_CHAR
#define DRM_SAVED_RECPATH_LEN (SCI_STRLEN(DRM_SAVED_RECPATH))

typedef enum{
    MMIDRM_SAVED_NOTEXIST,
    MMIDRM_SAVED_YES,
    MMIDRM_SAVED_NO,
    MMIDRM_SAVED_OTHER
}MMIDRM_TYPE_SAVED_E;
typedef enum{
    MMIDRM_SAVEDFIND_1,
    MMIDRM_SAVEDFIND_2,
    MMIDRM_SAVEDFIND_3,
    MMIDRM_SAVEDFIND_MAX
}MMIDRM_TYPE_SAVEDFIND_E;

/*****************************************************************************/
//  Description : get record member string by type
//  Global resource dependence : none
//  Author: CBK
//  Note:record format : oriName,SaveNam,Flage,\r
/*****************************************************************************/
LOCAL uint16 MMIDRM_SavRec_GetStrByType(
                                        char* inStrStar, 
                                        char* inStrEnd,
                                        char** outStr, 
                                        MMIDRM_TYPE_SAVEDFIND_E findType
                                        )
{
    uint8* iCurPtr = inStrStar;
    uint8* iEndPtr = inStrStar;
    MMIDRM_TYPE_SAVEDFIND_E itype = MMIDRM_SAVEDFIND_1;
    char*  cTmpPtr = DRM_SPLIT_SYMBOL;

    if((PNULL == inStrStar) || (MMIDRM_SAVEDFIND_MAX <= findType))
    {
        return 0;
    }
    
    for(itype = 0; (itype<=findType)&&(iEndPtr); itype++)
    {
        if(itype)
        {
            iCurPtr = iEndPtr + 1;
        }
        iEndPtr = MMIAPICOM_SearchChar(iCurPtr, *cTmpPtr);
        if(iEndPtr > (uint8*)inStrEnd)
        {
            *outStr = PNULL;
            return 0;
        }
    }
    
    if(PNULL != iEndPtr)
    {        
        *outStr = iCurPtr;
        return (iEndPtr-iCurPtr);
    }

    return 0;
}

/*****************************************************************************/
//  Description : Get if saved
//  Global resource dependence : none
//  Author: CBK
//  Note:
/*****************************************************************************/
PUBLIC MMIDRM_TYPE_SAVED_E MMIAPIDRM_SavRec_GetIsSaved(
                                                       char* cBaseBufPtr, 
                                                       char* cOriFnamePtr
                                                       )
{
    uint8* iCurPtr = cBaseBufPtr;
    uint8* iEndPtr = cBaseBufPtr;
    uint8* outStrPtr = PNULL;
    uint16 outStrLen = 0;
    char* cTmpPtr = DRM_SPLIT_LINE_SYMBOL;
    int8 iFlage = 0;

    if((PNULL == iCurPtr) || (PNULL == cOriFnamePtr) )
    {
        return MMIDRM_SAVED_OTHER;
    }

    SCI_TRACE_BUF("MMIMMS DRM [CBK]: MMIAPIDRM_SavRec_GetIsSaved cOriFnamePtr=", cOriFnamePtr, SCI_STRLEN(cOriFnamePtr));

    while(iCurPtr)
    {
        //search every line
        iEndPtr = MMIAPICOM_SearchChar(iCurPtr, *cTmpPtr);
        //get out saved flage of real name in line
        outStrLen = MMIDRM_SavRec_GetStrByType(iCurPtr, iEndPtr, &outStrPtr, MMIDRM_SAVEDFIND_1);
        SCI_TRACE_LOW("MMIMMS DRM [CBK]: MMIAPIDRM_SavRec_GetIsSaved outStrLen=", outStrLen);
        SCI_TRACE_BUF("MMIMMS DRM [CBK]: MMIAPIDRM_SavRec_GetIsSaved outStrPtr=", outStrPtr, outStrLen);
        if(!MMIAPICOM_StrCmp(outStrPtr, outStrLen, cOriFnamePtr, SCI_STRLEN(cOriFnamePtr)))
        {
            outStrLen = MMIDRM_SavRec_GetStrByType(iCurPtr, iEndPtr, &outStrPtr, MMIDRM_SAVEDFIND_3);
            iFlage |= ((*outStrPtr!='0')?1:0);
        }
        if(!iEndPtr)
        {
            break;
        }
        iCurPtr = iEndPtr + 1;
    }

    if(iFlage)
    {
        return MMIDRM_SAVED_YES;
    }
    else
    {
        return MMIDRM_SAVED_NO;
    }
    
}

/*****************************************************************************/
//  Description : add one record with buffer
//  Global resource dependence : none
//  Author: CBK
//  Note:
/*****************************************************************************/
// better need check size before use this function
PUBLIC void MMIDRM_SavRec_AddRecord(char* cBufPosPtr, char* cOriNamePtr, char* cSavedFnamePtr, char* cIsSaved)
{
    uint32 iOutStrLen = 0;
    uint8* iCurPosPtr = cBufPosPtr;

    MMIAPICOM_CopyString(&iOutStrLen, iCurPosPtr, SCI_STRLEN(cOriNamePtr), SCI_STRLEN(cOriNamePtr), cOriNamePtr);
    iCurPosPtr += iOutStrLen;
    MMIAPICOM_CopyString(&iOutStrLen, iCurPosPtr, SCI_STRLEN(DRM_SPLIT_SYMBOL), SCI_STRLEN(DRM_SPLIT_SYMBOL), DRM_SPLIT_SYMBOL);
    iCurPosPtr += SCI_STRLEN(DRM_SPLIT_SYMBOL);

    MMIAPICOM_CopyString(&iOutStrLen, iCurPosPtr, SCI_STRLEN(cSavedFnamePtr), SCI_STRLEN(cSavedFnamePtr), cSavedFnamePtr);
    iCurPosPtr += iOutStrLen;
    MMIAPICOM_CopyString(&iOutStrLen, iCurPosPtr, SCI_STRLEN(DRM_SPLIT_SYMBOL), SCI_STRLEN(DRM_SPLIT_SYMBOL), DRM_SPLIT_SYMBOL);
    iCurPosPtr += SCI_STRLEN(DRM_SPLIT_SYMBOL);

    MMIAPICOM_CopyString(&iOutStrLen, iCurPosPtr, SCI_STRLEN(cIsSaved), SCI_STRLEN(cIsSaved), cIsSaved);
    iCurPosPtr += iOutStrLen;
    MMIAPICOM_CopyString(&iOutStrLen, iCurPosPtr, SCI_STRLEN(DRM_SPLIT_SYMBOL), SCI_STRLEN(DRM_SPLIT_SYMBOL), DRM_SPLIT_SYMBOL);
    iCurPosPtr += SCI_STRLEN(DRM_SPLIT_SYMBOL);

    MMIAPICOM_CopyString(&iOutStrLen, iCurPosPtr, SCI_STRLEN(DRM_SPLIT_LINE_SYMBOL), SCI_STRLEN(DRM_SPLIT_LINE_SYMBOL), DRM_SPLIT_LINE_SYMBOL);
}

/*****************************************************************************/
//  Description : set  saved flage following the savedName
//  Global resource dependence : none
//  Author: CBK
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIDRM_SetSaved(char* cBaseBufPtr, char* cSavedFname, BOOLEAN isSaved)
{
    uint8* iCurPtr = cBaseBufPtr;
    uint8* iEndPtr = cBaseBufPtr;
    uint8* outStrPtr = PNULL;
    uint16 outStrLen = 0;
    int8 iFlage = 0;
    char*  cSplitLinePtr = DRM_SPLIT_LINE_SYMBOL;

    if((PNULL == iCurPtr) || (PNULL == cSavedFname) )
    {
        return FALSE;
    }
    
    while(iCurPtr)
    {
        //search every line
        iEndPtr = MMIAPICOM_SearchChar(iCurPtr, *cSplitLinePtr);
        //get out saved flage of real name in line
        outStrLen = MMIDRM_SavRec_GetStrByType(iCurPtr, iEndPtr, &outStrPtr, MMIDRM_SAVEDFIND_2);
        if(!MMIAPICOM_StrCmp(outStrPtr, outStrLen, cSavedFname, SCI_STRLEN(cSavedFname)))
        {
            outStrLen = MMIDRM_SavRec_GetStrByType(iCurPtr, iEndPtr, &outStrPtr, MMIDRM_SAVEDFIND_3);
            *outStrPtr = (isSaved?'1':'0');
            return TRUE;
        }
        if(!iEndPtr)
        {
            break;
        }
        iCurPtr = iEndPtr + 1;
    }

    return FALSE;
}

/*****************************************************************************/
//  Description : Get saved file  record content
//  Global resource dependence : none
//  Author: CBK
//  Note:
/*****************************************************************************/
PUBLIC MMIFILE_HANDLE MMIDRM_GetSavedRecBuf(uint32 uRecId, uint8** outBufPtr, uint16 addEndLen)
{
    uint8                   fname [MMIMMS_FILE_NAME_LEN] = {0};
    wchar                   w_fname[MMIMMS_FILE_NAME_LEN] = {0};
    uint16                  file_len = 0;  
    uint32                  file_size = 0;
    
    MMIFILE_DEVICE_E cur_file_type= MMI_DEVICE_NUM;
    uint16 device_name_len = 0;
    wchar* device_name_ptr = PNULL;
    wchar  cmsmms_full_path[MMIFILE_FULL_PATH_MAX_LEN+1] = {0};
    uint16 cmsmms_full_path_len = MMIFILE_FULL_PATH_MAX_LEN;
    uint32 charsread = 0;
    SFS_HANDLE file_handle = PNULL;
        
    cur_file_type = MMIAPIFMM_GetFirstValidDevice();
    device_name_len = MMIAPIFMM_GetDevicePathLen((cur_file_type));
    device_name_ptr = MMIAPIFMM_GetDevicePath(cur_file_type);
    
    //file name
    _snprintf((char *)fname,(MMIMMS_FILE_NAME_LEN-1),"%ld.txt",uRecId);
    MMI_STRNTOWSTR(w_fname,MMIMMS_FILE_NAME_LEN,(uint8 *)fname,MMIMMS_FILE_NAME_LEN,SCI_STRLEN(fname));
    
    if(MMIAPIFMM_CombineFullPath(
        (wchar*)device_name_ptr, device_name_len,
        (uint16*)DRM_SAVED_RECPATH, DRM_SAVED_RECPATH_LEN,
        (wchar*)w_fname, 
        (uint16)(MMIAPICOM_Wstrlen( w_fname)), //in bytes
        cmsmms_full_path,          //[out]
        &cmsmms_full_path_len      //[out]
        ))
    {
        
        
            file_handle = MMIAPIFMM_CreateFile(
                (const wchar *)cmsmms_full_path, 
                SFS_MODE_READ|SFS_MODE_WRITE|SFS_MODE_OPEN_ALWAYS,
                NULL,    //Must be NULL,File System have not realize it 
                NULL    //Must be NULL,File System have not realize it 
                );
        
    }
    
    if (SFS_INVALID_HANDLE != file_handle)
    {
        file_size = MMIAPIFMM_GetFileSize(file_handle);
        //MMIAPIFMM_GetFileInfo(cmsmms_full_path, cmsmms_full_path_len, &file_size, PNULL, PNULL);
        //MMIAPIFMM_ReadFile(file_handle, &file_size, 4, &charsread,PNULL);         
        //file_offset=MMIAPIFMM_GetFilePointer(file_handle,MMIFILE_SEEK_BEGIN);
    }
    else
    {
        return PNULL;
    }
  
    *outBufPtr = (char*)SCI_ALLOCAZ(sizeof(char)*(file_size + addEndLen));
    if(PNULL == *outBufPtr)
    {
        return PNULL;
    }
    SCI_MEMSET(*outBufPtr, 0, file_size + addEndLen);
    MMIAPIFMM_ReadFile(file_handle,*outBufPtr,file_size,&charsread,PNULL);         
    
    return file_handle;
}

/*****************************************************************************/
//  Description : set if the content saved
//  Global resource dependence : none
//  Author: CBK
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIDRM_SetSaved(wchar* w_saved_fname, BOOLEAN isSaved)
{
    char* cTmpBuf = PNULL;
    MMIMMS_EDIT_DOCUMENT_T* editdoc_ptr = PNULL;
    MMIFILE_HANDLE file_handle = PNULL;
    uint16 uFullNameLen = MMIAPICOM_Wstrlen(w_saved_fname) + 1;
    char* cFullNamePtr = (char*)SCI_ALLOCAZ(sizeof(char)*uFullNameLen);

    uint32 iWrittenLen = 0;

    editdoc_ptr = GetMMSReadEditDoc();

    file_handle = MMIDRM_GetSavedRecBuf(editdoc_ptr->record_id, &cTmpBuf, 0);

    if(PNULL == file_handle)
    {
        return;
    }
    MMIAPICOM_WstrToStr(w_saved_fname, cFullNamePtr);

    SCI_TRACE_BUF("MMIMMS DRM [CBK]: MMIAPIDRM_SetSaved cFullNamePtr=", cFullNamePtr, uFullNameLen);
    if( MMIDRM_SetSaved(cTmpBuf, cFullNamePtr, isSaved))
    {
        MMIAPIFMM_SetFilePointer(file_handle, 0, MMIFILE_SEEK_BEGIN);
        MMIAPIFMM_WriteFile(file_handle, cTmpBuf, MMIAPIFMM_GetFileSize(file_handle), &iWrittenLen, PNULL);
    }
    if(cTmpBuf)
    {
        SCI_FREE(cTmpBuf);
    }
    if(cFullNamePtr)
    {
        SCI_FREE(cFullNamePtr);
    }
    MMIAPIFMM_CloseFile(file_handle);

}
#endif
/*****************************************************************************/
//  Description : save mms content
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC MMIMMS_ERROR_E MMIMMS_SaveMMSContent(
                                            uint8	*filename_ptr,
                                            BOOLEAN is_text,
                                            wchar * full_path_ptr
#ifdef DRM_SUPPORT
                                            ,uint16 selectIndex
#endif
                                            )
{
    MMIMMS_ERROR_E return_val = MMIMMS_NO_ERROR;
    uint8   *buffer_ptr = PNULL;
    uint8 *text_buffer_ptr = PNULL;
    uint32 text_len = 0;
    uint32   buffer_len = 0;
    int32  size = 0;
    //MMIFILE_DEVICE_E fs_dev = MMI_DEVICE_SDCARD;
    MMIFILE_DEVICE_E device_type = MMI_DEVICE_UDISK;
    uint32 write_size = 0;
    wchar full_path_name [MMIFILE_FULL_PATH_MAX_LEN+1] = {0};
    uint16 full_path_len = MMIFILE_FULL_PATH_MAX_LEN;
    wchar  w_fname[MMIMMS_FILE_NAME_LEN + 1] = {0};
    wchar  new_fname[MMIMMS_FILE_NAME_LEN + 1] = {0};
    uint16 new_fname_len = 0;
    uint16 file_len = 0;
    uint16 file_wlen = 0;
    MMIFILE_HANDLE file_handle = NULL;
    
    //SCI_TRACE_LOW:"[MMIMMS]:MMIMMS_SaveMMSContent is_text = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_FILE_2163_112_2_18_2_38_23_183,(uint8*)"d", is_text);
    if(PNULL == filename_ptr)
    {
        return MMIMMS_ERROR_NO_CONTENT;
    }
    sysVFGetConstFileData((int8 *)filename_ptr,(char **)&buffer_ptr,&size);
    if(buffer_ptr)
    {
#ifdef DRM_SUPPORT
        MMIDRM_TYPE_SAVED_E eIfSaved = MMIDRM_SAVED_NO;
#endif

        device_type = MMIMMS_GetDeviceForSave(size);
        if(MMI_DEVICE_NUM == device_type)
        {
            //SCI_TRACE_LOW:"[MMIMMS]:MMIMMS_SaveMMSContent no space"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_FILE_2174_112_2_18_2_38_23_184,(uint8*)"");
            return MMIMMS_ERROR_NO_SPACE;
        }
        
        file_len = SCI_STRLEN((char *)filename_ptr);
        file_len = MIN(file_len,MMIMMS_FILE_NAME_LEN);
        
        file_wlen = GUI_UTF8ToWstr(w_fname, MMIMMS_FILE_NAME_LEN, filename_ptr, file_len);

        if (MMIAPICOM_IsIncludeInvalidChar(w_fname, file_wlen))
        {//如果有非法的名称使用随机的名称
            uint8  new_radomname[MMIMMS_FILE_NAME_LEN + 1]   = {0};
            uint16 suffix_wstr[MMIMMS_SUFFIX_LEN + 1]        = {0};
            uint16 suffix_len                                = MMIMMS_SUFFIX_LEN;
            wchar  dot_text[]                                = {'.',0};

            MMIAPIFMM_SplitFileName(w_fname, file_len, PNULL, PNULL, suffix_wstr, &suffix_len);
            MMIMMS_GenRandomName(new_radomname, MMIMMS_FILE_NAME_LEN);
            new_fname_len = SCI_STRLEN((char *)new_radomname);
            MMI_STRNTOWSTR(new_fname, MMIMMS_FILE_NAME_LEN, new_radomname, MMIMMS_FILE_NAME_LEN, new_fname_len);
            MMI_WSTRNCPY(new_fname + new_fname_len, MMIMMS_FILE_NAME_LEN, dot_text, MMIAPICOM_Wstrlen(dot_text), MMIAPICOM_Wstrlen(dot_text));
            new_fname_len = new_fname_len + MMIAPICOM_Wstrlen(dot_text);
            MMI_WSTRNCPY(new_fname + new_fname_len, MMIMMS_FILE_NAME_LEN - new_fname_len, suffix_wstr, MMIMMS_SUFFIX_LEN, suffix_len);
            new_fname_len = MMIAPICOM_Wstrlen(new_fname);
            MMIAPICOM_CombineDefaultFullPathByFilename(device_type, new_fname, new_fname_len, 
            full_path_name, full_path_len);
            MMI_WSTRNCPY(full_path_ptr, MMIMMS_FILE_NAME_LEN, full_path_name, MMIMMS_FILE_NAME_LEN, full_path_len - new_fname_len);
        }
        else
        {
            MMIAPICOM_CombineDefaultFullPathByFilename(device_type, w_fname, file_wlen, 
            full_path_name, full_path_len);
            MMI_WSTRNCPY(full_path_ptr, MMIMMS_FILE_NAME_LEN, full_path_name, MMIMMS_FILE_NAME_LEN, full_path_len - file_wlen);
        }        
        
        full_path_len = MMIAPICOM_Wstrlen(full_path_name);
        MMIAPICOM_GetNoExistFileName(full_path_name,full_path_len);
#ifdef DRM_SUPPORT
        // find if the drm file had been saved
        if(MMIFMM_DCF_FILE == MMIMMS_GetDrmFileTypeFromName(filename_ptr) || MMIFMM_DM_FILE == MMIMMS_GetDrmFileTypeFromName(filename_ptr))
        {
            char* cRecBuf = PNULL;
            char cTmpOriFName[DRM_SAVED_ORIFN_LEN] = {0};//selectIndex
            uint16 uAddRecLen = 0;
            MMIMMS_EDIT_DOCUMENT_T* editdoc_ptr = GetMMSReadEditDoc();
            MMIFILE_HANDLE file_handle_rec = PNULL;

            itoa(selectIndex, cTmpOriFName, 10);
            uAddRecLen = MMIAPICOM_Wstrlen(full_path_name) + SCI_STRLEN(cTmpOriFName) + DRM_SPLIT_SYMBOL_LEN; // 5 : 1 flage, 3 ',' and 1 '\r' len
            file_handle_rec = MMIDRM_GetSavedRecBuf(editdoc_ptr->record_id, &cRecBuf, uAddRecLen); // get  the record content
            eIfSaved = MMIAPIDRM_SavRec_GetIsSaved(cRecBuf, cTmpOriFName);

            SCI_TRACE_LOW("MMIMMS DRM [CBK]: MMIMMS_SaveMMSContent eIfSaved=", eIfSaved);
            
            if(MMIDRM_SAVED_YES != eIfSaved)
            {
                uint32 uFileSize = 0;
                uint32 iWrittenLen = 0;
                uint16 uFullPathNameLen = MMIAPICOM_Wstrlen(full_path_name) + 1;
                char* cFullPathNamePtr = (char*)SCI_ALLOCAZ(sizeof(char)*uFullPathNameLen);
                
                if(PNULL != cFullPathNamePtr)
                {
                    SCI_MEMSET(cFullPathNamePtr, 0, uFullPathNameLen);
                    MMIAPICOM_WstrToStr(full_path_name, cFullPathNamePtr);
                    uFileSize = MMIAPIFMM_GetFileSize(file_handle_rec);
                    MMIDRM_SavRec_AddRecord((cRecBuf + uFileSize), cTmpOriFName, cFullPathNamePtr, "0");
                    MMIAPIFMM_SetFilePointer(file_handle_rec, 0, MMIFILE_SEEK_BEGIN);
                    MMIAPIFMM_WriteFile(file_handle_rec, cRecBuf, (uFileSize + uAddRecLen), &iWrittenLen, PNULL);            
                    SCI_FREE(cFullPathNamePtr);
                }
                
            }
            if(cRecBuf)
            {
                SCI_FREE(cRecBuf);
            }
            MMIAPIFMM_CloseFile(file_handle_rec);
        }
#endif
        file_handle = MMIAPIFMM_CreateFile(full_path_name,SFS_MODE_CREATE_ALWAYS|SFS_MODE_WRITE,PNULL,PNULL);

        if(SFS_INVALID_HANDLE != file_handle)
        {
            if (is_text)
            {
#if defined(GBK_SUPPORT) || defined(IM_SIMP_CHINESE_SUPPORT)
                buffer_len = *(uint32 *)buffer_ptr;
#else
                //增加两个字节存储Unicode文件头(0xFF,0xFE)
                buffer_len = *(uint32 *)buffer_ptr + 2;
#endif
                text_buffer_ptr = SCI_ALLOCAZ(buffer_len + 1);
                if (PNULL == text_buffer_ptr)
                {
                    MMIAPIFMM_CloseFile(file_handle);
                    return MMIMMS_ERROR_FAIL;
                }
                //SCI_MEMSET(text_buffer_ptr, 0 ,buffer_len + 1);
                text_len = MMIAPICOM_Wstrlen((wchar *)(buffer_ptr + 4));

#if defined(GBK_SUPPORT) || defined(IM_SIMP_CHINESE_SUPPORT)
                text_len = GUI_WstrToGB(text_buffer_ptr, (wchar *)(buffer_ptr + 4), text_len);
#else
                //需要写入的Unicode文件头
                text_buffer_ptr[0] = 0xFF;
                text_buffer_ptr[1] = 0xFE;
                //需要写入的Unicode文件内容
                MMI_WSTRNCPY((uint8 *)(text_buffer_ptr + 2), text_len, (wchar *)(buffer_ptr + 4), text_len, text_len);
                //需要写入文件的字节数
                text_len = 2*text_len + 2;
#endif
                //SCI_TRACE_LOW:"[MMIMMS]:MMIMMS_SaveMMSContent buffer_len = %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_FILE_2225_112_2_18_2_38_24_185,(uint8*)"d", buffer_len);
                if(MMIAPIFMM_WriteFile(file_handle, text_buffer_ptr, text_len, &write_size, PNULL))
                {
                    return_val = MMIMMS_ERROR_SFS_ERROR;
                }
                SCI_FREE(text_buffer_ptr);
                text_buffer_ptr = PNULL;
            }
            else
            {
                if(MMIAPIFMM_WriteFile(file_handle, buffer_ptr, size, &write_size, PNULL))
                {
                    return_val = MMIMMS_ERROR_SFS_ERROR;
                }
#ifdef DRM_SUPPORT
                if(MMIFMM_DCF_FILE == MMIMMS_GetDrmFileTypeFromName(filename_ptr) || MMIFMM_DM_FILE == MMIMMS_GetDrmFileTypeFromName(filename_ptr))
                {
                    MIME_TYPE_E                 mime_type = MIME_TYPE_APPLICATION_VND_OMA_DRM_CONTENT;
                    
                    if (MMIFMM_DM_FILE == MMIMMS_GetDrmFileTypeFromName(filename_ptr))
                    {
                        mime_type = MIME_TYPE_APPLICATION_VND_OMA_DRM_MESSAGE;
                    }
                    MMIAPIDRM_StartToInstallDrm(full_path_name, mime_type, PNULL, PNULL, (eIfSaved==MMIDRM_SAVED_YES)?TRUE:FALSE);
                }
#endif
            }            
                       
            MMIAPIFMM_CloseFile(file_handle);
        }
        else
        {
            return_val = MMIMMS_ERROR_SFS_ERROR;
        }
    }
    else
    {
        return_val = MMIMMS_ERROR_NO_CONTENT;
    }

    return return_val;
}

/*****************************************************************************/
//  Description : delete mms  and save list
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:  本函数删除彩信条目并保存LIST
/*****************************************************************************/
PUBLIC void MMIMMS_DeleteMMSAndSaveList(
                                        int32 record_id
                                        )
{
    MMIMMS_DeleteMMS(record_id);
    MMIMMS_SaveMMSListFile();
}

/*****************************************************************************/
//  Description : delete mms  考虑多选删除的效率问题，该函数只对内存中的LIST做修改，不做LIST的保存处理
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMIMMS_DeleteMMS(
                             int32				record_id
                             )
{
    uint8 fname [MMIMMS_FILE_NAME_LEN] = {0};
    wchar w_fname[MMIMMS_FILE_NAME_LEN] = {0};
    uint16 file_len = 0;
    wchar mms_file_path [MMIFILE_FULL_PATH_MAX_LEN+1] = {0};
    uint16 mms_file_path_len = MMIFILE_FULL_PATH_MAX_LEN;
    MMISMS_ORDER_INFO_T* cursor_ptr = s_mms_list.list_head_ptr;
    MMISMS_MO_MT_TYPE_E  mms_type = MMISMS_MT_NOT_DOWNLOAD;

    SCI_TRACE_LOW("MMIMMS_MMIMMS_DeleteMMS cursor_ptr:%d, record_id:%d", cursor_ptr, record_id);
    if(cursor_ptr && record_id)
    {
        mms_type = DeleteListItem(record_id);
        if(   MMISMS_MT_NOT_DOWNLOAD == mms_type
            ||MMISMS_MT_NOT_NOTIFY == mms_type
            ||MMISMS_MT_NEED_NOT_DOWNLOAD == mms_type)
        {
            _snprintf((char *)fname,(MMIMMS_FILE_NAME_LEN-1),"%ld.push",record_id);
        }
        else
        {
            _snprintf((char *)fname,(MMIMMS_FILE_NAME_LEN-1),"%ld.mms",record_id);
        }
        file_len = SCI_STRLEN((char *)fname);
        MMI_STRNTOWSTR(w_fname,MMIMMS_FILE_NAME_LEN,(uint8 *)fname,MMIMMS_FILE_NAME_LEN,file_len);
        if(GetMMSFilePathForRead(w_fname,file_len,mms_file_path, &mms_file_path_len))
        {
            MMIAPIFMM_DeleteFileSyn(mms_file_path,mms_file_path_len);     // 此处也可扩展为异步删除
        }
    }
    //    MMIMMS_ControlToUIMsg(MSG_MMS_DELETEMMS_END,&del_info);   //扩展为异步时使用此消息
}


/*****************************************************************************/
//     Description : delete all files in MMS directory
//    Global resource dependence : 
//  Author:
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMMS_DeleteAllMMSFiles(void)
{
    BOOLEAN             result = TRUE;
//  uint8               fname [MMIMMS_FILE_NAME_LEN] = {0};
//  wchar               w_fname[MMIMMS_FILE_NAME_LEN] = {0};
//  uint16              file_len = 0;
    wchar               mms_full_path [MMIFILE_FULL_PATH_MAX_LEN+1] = {0};
    uint16              mms_full_path_len = MMIFILE_FULL_PATH_MAX_LEN;
    SFS_OVERLAPPED_T    overLapped = {0};
    wchar               *device_ptr = PNULL;
    uint16              device_len = 0;

    device_ptr = MMIAPIFMM_GetDevicePath(MMIMMS_GetPreferDisk());
    device_len = MMIAPIFMM_GetDevicePathLen(MMIMMS_GetPreferDisk());
    if(PNULL == device_ptr || 0 == device_len)
    {
        return FALSE;
    }
    MMIAPIFMM_CombineFullPath(
            device_ptr, 
            device_len,
            MMIMULTIM_DIR_SYSTEM, 
            MMIMULTIM_DIR_SYSTEM_LEN,
            PNULL, 
            0,
            mms_full_path,
            &mms_full_path_len
            );
    overLapped.complete_route = DeleteAllMMSCallback;
//    SFS_DeleteTree(mms_full_path,MMIMMS_DEFAULT_DIR,&overLapped,PNULL);
    MMIAPIFMM_DeleteTree(mms_full_path,MMIMMS_DEFAULT_DIR);
    
    return result;
}

/*****************************************************************************/
//  Description : delete all mms file callback
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/ 
LOCAL void DeleteAllMMSCallback(
                                SFS_ERROR_E   error, 
                                uint32        irp_param, 
                                uint32        param1,
                                uint32        param2
                                )
{
    
}

/*****************************************************************************/
//  Description : whether mms push is in queue or not
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMMS_IsInQueue(int32 record_id)
{
    BOOLEAN return_val = FALSE;
    MMIMMS_AUTOLIST_T* cursor_ptr = PNULL;

    cursor_ptr= s_mms_list.manuallist_head_ptr;
    while(PNULL != cursor_ptr)
    {
        if(cursor_ptr->record_id == record_id)
        {
            return_val = TRUE;
            break;
        }
        cursor_ptr = cursor_ptr->next;
    }
    if(!return_val)
    {
        cursor_ptr= s_mms_list.autolist_head_ptr;
        while(PNULL != cursor_ptr)
        {
            if(cursor_ptr->record_id == record_id)
            {
                return_val = TRUE;
                break;
            }
            cursor_ptr = cursor_ptr->next;
        }
    }

    return return_val;
}


/*****************************************************************************/
//  Description : Get list item by ID 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC MMISMS_ORDER_INFO_T *MMIMMS_GetListItemByID(
                                                   int32				record_id
                                                   )
{
    MMISMS_ORDER_INFO_T* return_ptr = PNULL;
    MMISMS_ORDER_INFO_T* cursor_ptr = PNULL;

    cursor_ptr= s_mms_list.list_head_ptr;
    while(PNULL != cursor_ptr)
    {
        if(cursor_ptr->record_id == record_id)
        {
            return_ptr = cursor_ptr;
            break;
        }
        cursor_ptr = cursor_ptr->next;
    }

    return return_ptr;
}

/*****************************************************************************/
//  Description : init mms block memory 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMMS_InitBlock(void)
{
    BOOLEAN return_val = TRUE;
/*
    void* mmsblock_buffer_ptr = NULL;
    uint32 block_buff_size = 0;
    APP_MEM_CFG_T    mem_cfg = {0};
    
    //SCI_TRACE_LOW:"[MMIMMS]:MMIMMS_InitBlock"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_FILE_2446_112_2_18_2_38_24_186,(uint8*)"");
    if(INVALID_MEM_HANDLE == s_mms_mem_handle)
    {     
        mmsblock_buffer_ptr = MMI_BL_MALLOC((BLOCK_MEM_ID_E)BLOCK_MEM_POOL_MMS);
        block_buff_size = BL_GetSize((BLOCK_MEM_ID_E)BLOCK_MEM_POOL_MMS);  //return memory size.
        
        
        mem_cfg.min_pool_size = 4;
        mem_cfg.pool_space_size =  block_buff_size >> 2;
        mem_cfg.heap_mem_watermark = MMIMMS_BLOCK_WATERMARK;
        s_mms_mem_handle = SCI_CreateMemPool(MMS_POOL_NAME, mmsblock_buffer_ptr, block_buff_size, &mem_cfg);
        if(INVALID_MEM_HANDLE == s_mms_mem_handle)
        {
            //SCI_TRACE_LOW:"[MMIMMS]:MMIMMS_InitBlock call MMIAPIMEM_Config fail!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_FILE_2459_112_2_18_2_38_24_187,(uint8*)"");
            MMI_BL_FREE((BLOCK_MEM_ID_E)BLOCK_MEM_POOL_MMS);
            return_val = FALSE;
        }  
    }
    else
    {
        //SCI_TRACE_LOW:"[MMIMMS]:MMIMMS_InitBlock INVALID_MEM_HANDLE != s_mms_mem_handle"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_FILE_2466_112_2_18_2_38_24_188,(uint8*)"");
    }
*/
    return return_val;
}

/*****************************************************************************/
//  Description : mms alloc in block
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC void *MMIMMS_AllocInBlock(
                                 uint32         size,
                                 const char     *file_ptr, 
                                 uint32         line
                                 )
{
    return SCI_MallocApp (size,file_ptr,line);
    /*
    uint8 *new_ptr = PNULL; 
    uint32  real_size = size+sizeof(uint32);

    if(size == 0)
    {
        //SCI_TRACE_LOW:"[MMIMMS]:malloc error:size == 0"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_FILE_2489_112_2_18_2_38_24_189,(uint8*)"");
        return PNULL;
    }
    //SCI_TRACE_LOW:"[MMIMMS]: malloc size= %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_FILE_2492_112_2_18_2_38_24_190,(uint8*)"d", real_size);
    if(s_mms_mem_handle != INVALID_MEM_HANDLE)
    {
        new_ptr = SCI_MPALLOC(real_size, s_mms_mem_handle);
        if(PNULL != new_ptr)
        {
            *((uint32*)new_ptr) = MEM_FROM_BLOCK;
        }
    }
    
    if(PNULL == new_ptr)
    {
        new_ptr = SCI_ALLOCA(real_size);
        if(PNULL != new_ptr)
        {
            *((uint32*)new_ptr) = MEM_FROM_SYS_POOL;        
        }
    }   
     
    if(new_ptr != PNULL)
    {
        SCI_MEMSET((new_ptr+sizeof(uint32)),0,size);
        return (void*)(new_ptr+sizeof(uint32));
    }
    else
    {
        //SCI_TRACE_LOW:"[BRW]error:malloc failed,size = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_FILE_2518_112_2_18_2_38_24_191,(uint8*)"d",size);
        return PNULL;
    }
    */
}
/*****************************************************************************/
//  Description : destroy mms block memory 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMIMMS_DestroyBlock(void)
{
    /*
    if (PNULL != s_mms_mem_handle)
    {
        SCI_DeleteMemPool(s_mms_mem_handle,TRUE);
        MMI_BL_FREE((BLOCK_MEM_ID_E)BLOCK_MEM_POOL_MMS);
        s_mms_mem_handle = PNULL;
    }
    else
    {
        //SCI_TRACE_LOW:"MMIMMS_DestroyBlock, handle is null!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_FILE_2538_112_2_18_2_38_24_192,(uint8*)"");
    }
    */
}

/*****************************************************************************/
//  Description : mms free in block
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC void  MMIMMS_FreeInBlock(
                                void 		*ptr
                                )
{
    SCI_FREE(ptr);
    /*
    uint32 flag = 0;
    void* real_ptr=PNULL;
    
    real_ptr = (void*)((uint32)ptr-sizeof(uint32));
    flag=*((uint32*)real_ptr);
    if(flag == MEM_FROM_SYS_POOL)
    {
       SCI_FREE(real_ptr);
       ptr=PNULL;
    }
    else if(flag == MEM_FROM_BLOCK)
    {
        SCI_MPFREE(real_ptr);
    }
    else
    {
        //SCI_TRACE_LOW:"[MMIMMS]:MMIMMS_FREE ERR:memory leak!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_FILE_2569_112_2_18_2_38_24_193,(uint8*)"");
    } 
    */
}
/*****************************************************************************/
//  Description : get vf file length
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC uint32  MMIMMS_GetVfFileLength(uint8 *fname)
{
    uint32      total_size = 0;
    FILE_HANDLE vf_handle = 0;
    
    vf_handle = sysVFopen((int8*)fname, "rb");
    if(vf_handle)
    {
        total_size = (uint32)sysVFfileLength(vf_handle);
        sysVFclose(vf_handle);
    }

    return total_size;
}

/*****************************************************************************/
//  Description : create mms dir if not exist
//  Global resource dependence : none
//  Author: bin.ji
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMMS_CreateMMSDir(void)
{
    BOOLEAN return_val = FALSE;

    return_val = MMIAPIFMM_CreateSysFileDir(MMIMMS_GetPreferDisk(),MMIMMS_DEFAULT_DIR, MMIMMS_DEFAULT_DIR_LEN,PNULL);

    return return_val;
}

