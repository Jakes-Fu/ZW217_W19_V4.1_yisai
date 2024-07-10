/****************************************************************************
** File Name:      mmipb_interface.c                                          *
** Author:                                                                 *
** Date:           11/09/2011                                              *
** Copyright:      2011 Spreadtrum, Incorporated. All Rights Reserved.      *
** Description:    This file  define synchronous and asynchronous access interface,
**                 management of this interface and handles, and data tables.
**                 这个文件主要是定义了主要同步和异步访问的接口，这个访问接口的管理，
**                 handle的管理.
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 11/2011        baokun yin         Create
** 
****************************************************************************/
#define _MMIPB_INTERFACE_C_  

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "mmi_app_pb_trc.h"
#include "mmipb_interface.h"
#include "mmipb_datalist.h"
#include "mmi_queue.h"
#include "mmi_event_api.h"
#include "mmivcard_export.h"
#include "mmipb_task.h"
#ifdef SNS_SUPPORT
#include "mmisns_sqlite.h"
#include "mmisns_srv.h"
#endif
#include "mmipb_text.h"
#include "mmipb_search.h"
#include "mmifmm_export.h"
#include "mmipb_adapt.h"

#include "mmi_modu_main.h"
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#define    MMIPB_HANDLE_TYPE_VALUE   0x10000
#define    MMIPB_HANDLE_ID_MASK      0xFFFF
 /**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/
//读数据函数
typedef  MMIPB_ERROR_E (*MMIPB_ACCESS_READ_FUNC) (MMIPB_ACCESS_POINT_T access_point, uint32 data_len, void *read_data_ptr);
//写数据函数
typedef  MMIPB_ERROR_E (*MMIPB_ACCESS_WRITE_FUNC) (MMIPB_ACCESS_POINT_T access_point, uint32 data_len, void *write_data_ptr);
//删数据函数
typedef  MMIPB_ERROR_E (*MMIPB_ACCESS_DELETE_FUNC) (MMIPB_ACCESS_POINT_T access_point);
//访问函数表的类型
typedef struct _MMIPB_ACCESS_FUNC_TABLE
{
    MMIPB_ACCESS_READ_FUNC   read;        
    MMIPB_ACCESS_WRITE_FUNC  write;   
    MMIPB_ACCESS_DELETE_FUNC delete; 
} MMIPB_ACCESS_FUNC_TABLE;
/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : 读取联系人信息
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
LOCAL MMIPB_ERROR_E ReadContact (
                           MMIPB_ACCESS_POINT_T access_point,//
                           uint32 data_len,//
                           void *contact_ptr//[OUT]
                           );

/*****************************************************************************/
//  Description : 保存联系人信息
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
LOCAL MMIPB_ERROR_E WriteContact (
                            MMIPB_ACCESS_POINT_T access_point,//
                            uint32 contact_size,
                            void *contact_ptr//[IN]
                            );

/*****************************************************************************/
//  Description : 删除联系人信息
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
LOCAL MMIPB_ERROR_E DeleteContact(
                            MMIPB_ACCESS_POINT_T access_point
                            );


/*****************************************************************************/
//  Description : 是否是访问sim记录
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsAccessSIMContact(
                MMIPB_ACCESS_E access_type,//[IN]信息类型
                MMIPB_ACCESS_POINT_T access_point//[IN]信息位置信息
                );

#ifdef CMCC_VCARD_SUPPORT
/*****************************************************************************/
//  Description : 读取USIM联系人信息
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
LOCAL MMIPB_ERROR_E ReadUSIMContact (
                           MMIPB_ACCESS_POINT_T access_point,//
                           uint32 data_len,//
                           void *contact_ptr//[OUT]
                           );

/*****************************************************************************/
//  Description : 写USIM联系人信息
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
LOCAL MMIPB_ERROR_E WriteUSIMContact(
                           uint16 contact_id,//
                           uint16 storage_id,//
                           MMIPB_CONTACT_T *contact_ptr//[IN]
                           );

/*****************************************************************************/
//  Description : 删除USIM联系人信息
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
LOCAL MMIPB_ERROR_E DeleteUSIMContact(
                            uint16 contact_id,
                            uint16 storage_id
                            );

/*****************************************************************************/
//  Description : 读取USIM 扩展号码的类型字符串
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
LOCAL MMIPB_ERROR_E ReadUSIMAASString (
                           MMIPB_ACCESS_POINT_T access_point,//
                           uint32 data_len,//
                           void *contact_ptr//[OUT]
                           );

/*****************************************************************************/
//  Description : 写USIM 扩展号码的类型字符串
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
LOCAL MMIPB_ERROR_E WriteUSIMAASString(
                           MMIPB_ACCESS_POINT_T access_point,//
                           uint32 data_len,//
                           void *contact_ptr//[IN]
                           );


#endif
#ifdef MMI_VCARD_SUPPORT
/*****************************************************************************/
//  Description : 读vcard联系人信息
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
LOCAL MMIPB_ERROR_E ReadVcardContact(
                            MMIPB_ACCESS_POINT_T access_point,
                            uint32 data_len,//
                            void *contact_ptr//[OUT]
                            );

/*****************************************************************************/
//  Description : 保存联系人到vcard
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
LOCAL MMIPB_ERROR_E WriteVcardContact (
                            MMIPB_ACCESS_POINT_T access_point,//
                            uint32 contact_size,
                            void *contact_ptr//[IN]
                            );
#endif
/*****************************************************************************/
//  Description : 读分组信息
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
LOCAL MMIPB_ERROR_E ReadGroup(
                            MMIPB_ACCESS_POINT_T access_point,
                            uint32 data_len,//
                            void *contact_ptr//[OUT]
                            );


#ifdef MMIPB_GROUP_MANAGE_SUPPORT
/*****************************************************************************/
//  Description : 保存分组信息
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
LOCAL MMIPB_ERROR_E WriteGroup (
                            MMIPB_ACCESS_POINT_T access_point,//
                            uint32 contact_size,
                            void *contact_ptr//[IN]
                            );

/*****************************************************************************/
//  Description : 删除分组信息
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
LOCAL MMIPB_ERROR_E DeleteGroup(
                            MMIPB_ACCESS_POINT_T access_point
                            );
#endif
/*****************************************************************************/
//  Description : 执行等待队列中的下个请求
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
LOCAL void DoNextWaitingReuest(void);

/*****************************************************************************/
//  Description : 获取pb是否忙状态
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetTaskStatus(void);

#ifdef SNS_SUPPORT 
/*****************************************************************************/
//  Description : 读sns信息
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
LOCAL MMIPB_ERROR_E ReadSnsContact(
                            MMIPB_ACCESS_POINT_T access_point,
                            uint32 data_len,//
                            void *search_handle//[OUT]
                            );
#endif
/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/ 
//当前PB Task是否在操作
LOCAL BOOLEAN s_pb_is_busy = FALSE;  //TRUE：PB Task忙；FALSE：PB Task空闲
//同步写返回值
LOCAL MMIPB_ERROR_E s_pb_write_error = MMIPB_ERROR_SUCCESS;
//同步删除返回值
LOCAL MMIPB_ERROR_E s_pb_delete_error = MMIPB_ERROR_SUCCESS;
//update event
LOCAL SCI_EVENT_GROUP_PTR s_pb_write_event = PNULL;
//delete event
LOCAL SCI_EVENT_GROUP_PTR s_pb_delete_event = PNULL;
//异步操作的互斥量
LOCAL SCI_MUTEX_PTR       s_pb_asyn_mutex_p = PNULL;
//handle读取创建的互斥量
LOCAL SCI_MUTEX_PTR       s_pb_handle_mutex_p = PNULL;
//handle数组
LOCAL MMIPB_HANDLE_ARRAY_T s_mmipb_handle_table = {0};
//当前handle
LOCAL MMIPB_HANDLE_T  s_mmipb_current_handle;
//等待消息队列队列
LOCAL MMI_QUEUE_NODE_T *s_mmipb_wait_queue = PNULL; 
//访问函数表
LOCAL const MMIPB_ACCESS_FUNC_TABLE g_mmipb_access_table[MMIPB_ACCESS_NUM] = 
{
    //联系人记录访问函数
    {ReadContact,     WriteContact,       DeleteContact},
#ifdef CMCC_VCARD_SUPPORT
    //usim aas 访问函数
    {ReadUSIMAASString, WriteUSIMAASString, PNULL},
#endif
#ifdef MMI_VCARD_SUPPORT
    //vcard访问函数
    {ReadVcardContact, WriteVcardContact,  PNULL},
#endif
    //分组访问函数

#ifdef MMIPB_GROUP_MANAGE_SUPPORT
    {ReadGroup, WriteGroup, DeleteGroup},
#else
    {ReadGroup, PNULL, PNULL},
#endif
#ifdef SNS_SUPPORT        
    {ReadSnsContact ,PNULL, PNULL},
#endif        
};


/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : 设置pb是否忙状态
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC void MMIPB_SetTaskStatus(BOOLEAN is_busy)
{
    SCI_DisableIRQ();
    s_pb_is_busy = is_busy;
    SCI_RestoreIRQ();
}
/*****************************************************************************/
//  Description : 获取pb是否忙状态
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetTaskStatus(void)
{
    BOOLEAN ret = FALSE;
    //SCI_DisableIRQ();
    ret = s_pb_is_busy;
    //SCI_RestoreIRQ();
    return ret;
}
/*****************************************************************************/
//  Description : 创建同步的event
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC void MMIPB_CreateEvent(void)
{
    //write event
    if(PNULL != s_pb_write_event)
    {
        SCI_DeleteEvent(s_pb_write_event);
        s_pb_write_event = PNULL;
    }
    s_pb_write_event = SCI_CreateEvent("s_pb_write_event");
    //delete event
    if(PNULL != s_pb_delete_event)
    {
        SCI_DeleteEvent(s_pb_delete_event);
        s_pb_delete_event = PNULL;
    }
    s_pb_delete_event = SCI_CreateEvent("s_pb_delete_event");    
    if(PNULL != s_pb_asyn_mutex_p)
    {
        SCI_DeleteMutex(s_pb_asyn_mutex_p);
        s_pb_asyn_mutex_p = PNULL;
    }
    s_pb_asyn_mutex_p = SCI_CreateMutex("s_pb_asyn_mutex_p", SCI_INHERIT);
    if(PNULL != s_pb_handle_mutex_p)
    {
        SCI_DeleteMutex(s_pb_handle_mutex_p);
        s_pb_handle_mutex_p = PNULL;
    }
    s_pb_handle_mutex_p = SCI_CreateMutex("s_pb_handle_mutex_p", SCI_INHERIT);

    MMIPB_CreateDataListSemphore();
#ifdef TCARD_PB_SUPPORT
    MMIPB_CreateTcardHandleMutex();
#endif
}

/*****************************************************************************/
//  Description : 同步写的confirm
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC void MMIPB_WriteTrigger(
                           uint16 contact_id,
                           uint16 storage_id,
                           MMIPB_ERROR_E error
                           )
{
    //trigger write event
    //SCI_TRACE_LOW:"[MMIPB] MMIPB_WriteTrigger"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_INTERFACE_335_112_2_18_2_43_28_190,(uint8*)"");
    MMIPB_SetWriteError(error);
    if(PNULL != s_pb_write_event)
    {
        SCI_SetEvent(s_pb_write_event, contact_id + (storage_id << 16), SCI_OR);
    }
}

/*****************************************************************************/
//  Description : 同步删除的confirm
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC void MMIPB_DeleteTrigger(
                           uint16 contact_id,
                           uint16 storage_id,
                           MMIPB_ERROR_E error
                           )
{
    //trigger delete event
    //SCI_TRACE_LOW:"[MMIPB] MMIPB_DeleteTrigger"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_INTERFACE_354_112_2_18_2_43_28_191,(uint8*)"");
    MMIPB_SetDeleteError(error);
    if(PNULL != s_pb_delete_event)
    {
        SCI_SetEvent(s_pb_delete_event, contact_id + (storage_id << 16), SCI_OR);
    }
}

/*****************************************************************************/
//  Description : 信息读取
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_Read (
                          MMIPB_ACCESS_E access_type,//[IN]信息类型
                          MMIPB_ACCESS_POINT_T access_point,//[IN]信息位置信息
                          uint32 data_len,//[IN]buf最大长度
                          void *out_data_ptr//[OUT]读取的数据
                          )
{
    MMIPB_ERROR_E result = MMIPB_ERROR_ERROR;

    //SCI_TRACE_LOW("[MMIPB] MMIPB_Read access_type %d", access_type);
    if(out_data_ptr == PNULL || access_type >= MMIPB_ACCESS_NUM)
    {
        return MMIPB_ERROR_INVALID_PARAM;
    }
    if(g_mmipb_access_table[access_type].read != PNULL)
    {
        result = g_mmipb_access_table[access_type].read(access_point, data_len, out_data_ptr);
    }
    return result;
}

/*****************************************************************************/
//  Description : 是否是访问sim记录
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsAccessSIMContact(
                MMIPB_ACCESS_E access_type,//[IN]信息类型
                MMIPB_ACCESS_POINT_T access_point//[IN]信息位置信息
                )
{
    if(
        (MMIPB_ACCESS_CONTACT == access_type)
       && MMIPB_GET_STORAGE(access_point.contact.storage_id) >= MMIPB_STORAGE_SIM1)
    {
       return TRUE;
    }
    return FALSE;
}

/*****************************************************************************/
//  Description : 写信息
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_Write (
                           MMIPB_ACCESS_E access_type,//[IN]信息类型
                           MMIPB_ACCESS_POINT_T access_point,//[IN]信息位置信息
                           uint32 data_len,//[IN]写入的数据长度
                           void *write_data_ptr//[IN]写入的数据
                           )
{
    uint32   event_flag = 0;
    MMIPB_ERROR_E result = MMIPB_ERROR_ERROR;

    if(write_data_ptr == PNULL || access_type >= MMIPB_ACCESS_NUM)
    {
        //SCI_TRACE_LOW:"[baokun] MMIPB_Write invalid param !"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_INTERFACE_421_112_2_18_2_43_28_192,(uint8*)"");
        return MMIPB_ERROR_INVALID_PARAM;
    }

    SCI_TRACE_LOW("[MMIPB] MMIPB_Write access_type %d", access_type);
    if(g_mmipb_access_table[access_type].write != PNULL)
    {
        result = g_mmipb_access_table[access_type].write(access_point, data_len, write_data_ptr);
    }

    if(IsAccessSIMContact(access_type, access_point))
    {
        //会在cnf中根据数据表
        if(MMIPB_ERROR_PENDING ==result)
        {
            if(s_pb_write_event != PNULL)
            {
                //如果是app task中，不能进行同步操作
                if(P_APP != SCI_IdentifyThread())
                {
                    SCI_GetEvent(s_pb_write_event, 
                        access_point.contact.contact_id + (access_point.contact.storage_id << 16),
                        SCI_OR_CLEAR,
                        &event_flag, 
                        SCI_WAIT_FOREVER
                        );
                    result = MMIPB_GetWriteError();
                }
            }
        }
    }
    else if(MMIPB_ERROR_SUCCESS == result && MMIPB_ACCESS_CONTACT == access_type)
    {
        MMIPB_RemoveContactFromList(access_point.contact.contact_id, access_point.contact.storage_id);
        ((MMIPB_CONTACT_T *)write_data_ptr)->contact_id = access_point.contact.contact_id;
        MMIPB_InsertContactToList((MMIPB_CONTACT_T *)write_data_ptr);        
    }
    SCI_TRACE_LOW("[MMIPB] MMIPB_Write exit result %d", result);
    return result;
}


/*****************************************************************************/
//  Description : 删除信息
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_Delete(
                            MMIPB_ACCESS_E access_type,
                            MMIPB_ACCESS_POINT_T access_point
                            )
{
    uint32   event_flag = 0;
    MMIPB_ERROR_E result = MMIPB_ERROR_ERROR;

    if(access_type >= MMIPB_ACCESS_NUM)
    {
        //SCI_TRACE_LOW:"[baokun] MMIPB_Delete invalid param !"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_INTERFACE_477_112_2_18_2_43_28_193,(uint8*)"");
        return MMIPB_ERROR_INVALID_PARAM;
    }

    //SCI_TRACE_LOW("[MMIPB] MMIPB_Delete access_type %d", access_type);
    if(g_mmipb_access_table[access_type].delete != PNULL)
    {
        result = g_mmipb_access_table[access_type].delete(access_point);
    }

    if(IsAccessSIMContact(access_type, access_point))
    {
        if(MMIPB_ERROR_PENDING ==result)
        {
            if(s_pb_delete_event != PNULL)
            {
                if(P_APP != SCI_IdentifyThread())
                {
                    SCI_GetEvent(s_pb_delete_event, 
                        access_point.contact.contact_id + (access_point.contact.storage_id << 16),
                        SCI_OR_CLEAR,
                        &event_flag, 
                        SCI_WAIT_FOREVER
                        );
                    result = MMIPB_GetDeleteError();
                }
            }
        }
    }
    if(MMIPB_ERROR_SUCCESS == result && MMIPB_ACCESS_CONTACT == access_type)
    {
        MMIPB_RemoveContactFromList(access_point.contact.contact_id, access_point.contact.storage_id);
    }
    //SCI_TRACE_LOW:"[MMIPB] MMIPB_Delete exit result %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_INTERFACE_510_112_2_18_2_43_28_194,(uint8*)"d", result);
    return result;
}

/*****************************************************************************/
//  Description : 设置写错误类型
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC void MMIPB_SetWriteError(MMIPB_ERROR_E error)
{
    //SCI_TRACE_LOW:"[MMIPB] MMIPB_SetWriteError error %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_INTERFACE_520_112_2_18_2_43_28_195,(uint8*)"d", error);
    SCI_DisableIRQ();
    s_pb_write_error = error;
    SCI_RestoreIRQ();
}

/*****************************************************************************/
//  Description : 读写错误类型
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_GetWriteError(void)
{
    MMIPB_ERROR_E  error = MMIPB_ERROR_SUCCESS;
    //SCI_DisableIRQ();
    error = s_pb_write_error;
    //SCI_RestoreIRQ();
    //SCI_TRACE_LOW:"[MMIPB] MMIPB_GetWriteError error %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_INTERFACE_536_112_2_18_2_43_28_196,(uint8*)"d", error);
    return error;
}

/*****************************************************************************/
//  Description : 设置删除错误类型
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC void MMIPB_SetDeleteError(MMIPB_ERROR_E error)
{
    //SCI_TRACE_LOW:"[MMIPB] MMIPB_SetDeleteError error %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_INTERFACE_546_112_2_18_2_43_28_197,(uint8*)"d", error);
    SCI_DisableIRQ();
    s_pb_delete_error = error;
    SCI_RestoreIRQ();
}

/*****************************************************************************/
//  Description : 读删除错误类型
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_GetDeleteError(void)
{
    MMIPB_ERROR_E  error = MMIPB_ERROR_SUCCESS;

    //SCI_DisableIRQ();
    error = s_pb_delete_error;
    //SCI_RestoreIRQ();
    //SCI_TRACE_LOW:"[MMIPB] MMIPB_GetDeleteError error %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_INTERFACE_563_112_2_18_2_43_28_198,(uint8*)"d", error);
    return error;
}

#ifdef CMCC_VCARD_SUPPORT
/*****************************************************************************/
//  Description : 异步读取USIM 扩展信息
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_ReadUSIMAddtionalAsyn(
                                           MN_DUAL_SYS_E dual_sys,
                                           uint16        contact_id
                                           )
{
    MMIPB_ERROR_E result = MMIPB_ERROR_ERROR;

    //SCI_TRACE_LOW:"[MMIPB] MMIPB_ReadUSIMAddtionalAsyn dual_sys %d contact_id %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_INTERFACE_579_112_2_18_2_43_28_199,(uint8*)"dd", dual_sys, contact_id);
    if(ERR_MNDATAMAG_NO_ERR == PHONEBOOK_ReadUsimAdditionalEntryEx(dual_sys, contact_id))
    {
        result = MMIPB_ERROR_PENDING;
    }

    return result;
}

/*****************************************************************************/
//  Description : 读取USIM联系人信息
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
LOCAL MMIPB_ERROR_E ReadUSIMContact (
                           MMIPB_ACCESS_POINT_T access_point,//
                           uint32 data_len,//
                           void *contact_ptr//[OUT]
                           )
{
    MMIPB_ERROR_E result = MMIPB_ERROR_INVALID_PARAM;
    MN_DUAL_SYS_E dual_sys = MMIPB_GetDualSysByStorageId(access_point.contact.storage_id);
    PHONEBOOK_ENTRY_T *simcontact_ptr = PNULL;

    if(dual_sys >= MMI_DUAL_SYS_MAX || PNULL == contact_ptr)
    {
        //SCI_TRACE_LOW:"[MMIPB]ReadUSimContact invalid param !"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_INTERFACE_604_112_2_18_2_43_28_200,(uint8*)"");
        return result;
    }
    //first read sim contact
    simcontact_ptr = (PHONEBOOK_ENTRY_T *)SCI_ALLOCA(sizeof(PHONEBOOK_ENTRY_T));
    if(simcontact_ptr != PNULL)
    {
        SCI_MEMSET(simcontact_ptr, 0x00, sizeof(PHONEBOOK_ENTRY_T));
        /*for bug 145104 start*/
        SCI_MEMSET(contact_ptr, 0x00, data_len);
        /*for bug 145104 end*/
        result = MMIPB_ReadSIMContact(access_point.contact.contact_id,
            dual_sys,
            MMIPB_GET_CONTACT_TYPE(access_point.contact.storage_id),
            simcontact_ptr);
        if(MMIPB_ERROR_SUCCESS == result)
        {
            MMIPB_SIMContactToContact(simcontact_ptr, 
                                     MMIPB_GetDualSysByStorageId(access_point.contact.storage_id),
                                     MMIPB_GET_CONTACT_TYPE(access_point.contact.storage_id),
                                     contact_ptr);
        }
        SCI_FREE(simcontact_ptr);
    }
    if(MMIPB_ERROR_SUCCESS == result)
    {
        //read addational contact
        MMIPB_ReadUSIMAddtionalFromContactList(access_point.contact.contact_id,
                                               access_point.contact.storage_id,
                                               contact_ptr);
    }

    return result;
}

/*****************************************************************************/
//  Description : 设置USIM需要更新的标志位
//  Global resource dependence : 
//  Author:baokun.yin
//  Note: old_contact_ptr == PNULL: add new; new_contact_ptr == PNULL: delete old
/*****************************************************************************/
LOCAL void SetUSIMUpdateFlag(
                           MMIPB_CONTACT_T *old_contact_ptr,//[IN] if PNULL, add new
                           MMIPB_CONTACT_T  *new_contact_ptr,//[IN] if PNULL delete old
                           BOOLEAN  *adn_flag_ptr,//bug355329
                           BOOLEAN  *anr_flag_ptr,
                           BOOLEAN  *email_flag_ptr,
                           BOOLEAN  *sne_flag_ptr
                           )
{
    uint32 i  = 0;
    BOOLEAN is_add_new = FALSE;
    if(old_contact_ptr == PNULL && (new_contact_ptr == PNULL))
    {
        //SCI_TRACE_LOW:"[MMIPB] SetUSIMUpdateFlag invalid param !"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_INTERFACE_652_112_2_18_2_43_28_201,(uint8*)"");
        return;
    }
	SCI_TRACE_LOW("[MMIPB] SetUSIMUpdateFlag old_contact_ptr=%x,new_contact_ptr=%x",old_contact_ptr,new_contact_ptr);
    if(new_contact_ptr == PNULL)
    {
        //delete old, set delete flag
        //set adn flag//bug355329
        if(adn_flag_ptr != PNULL)
        {
            if(
                old_contact_ptr != PNULL
                &&(old_contact_ptr->name.wstr_len > 0||MMIPB_IsValidPhoneNumber(old_contact_ptr->number[0].npi_ton, old_contact_ptr->number[0].number_len)))
            {
                *adn_flag_ptr = TRUE;
            }
            else
            {
                *adn_flag_ptr = FALSE;
            }
			SCI_TRACE_LOW("[MMIPB] ADN SetUSIMUpdateFlag old_contact_ptr->number[0].npi_ton=%d,old_contact_ptr->number[0].number_len=%d,old_contact_ptr->name.wstr_len=%d",
				old_contact_ptr->number[0].npi_ton,old_contact_ptr->number[0].number_len,old_contact_ptr->name.wstr_len);
			SCI_TRACE_LOW("[MMIPB] new_contact_ptr==PNULL SetUSIMUpdateFlag adn_flag_ptr=%d",adn_flag_ptr);
        }
        //set anr flag
        if(anr_flag_ptr != PNULL)
        {
            for(i = 0; i < MIN(MN_MAX_USIM_ANR_NUM, (MMIPB_MAX_NV_PHONE_NUM -1)); i++)/*lint !e506*/
            {
                if( old_contact_ptr != PNULL
                    &&MMIPB_IsValidPhoneNumber(old_contact_ptr->number[i+1].npi_ton, old_contact_ptr->number[i+1].number_len))
                {
                    anr_flag_ptr[i] = TRUE;
                }
                else
                {
                    anr_flag_ptr[i] = FALSE;
                }
				SCI_TRACE_LOW("[MMIPB] ANR SetUSIMUpdateFlag old_contact_ptr->number[%d+1].npi_ton=%d,old_contact_ptr->number[%d+1].number_len=%d",
					i,old_contact_ptr->number[i+1].npi_ton,i,old_contact_ptr->number[i+1].number_len);
				SCI_TRACE_LOW("[MMIPB] new_contact_ptr==PNULL SetUSIMUpdateFlag anr_flag_ptr[%d]=%d",i,anr_flag_ptr[i]);
            }
        }
#ifdef MMIPB_MAIL_SUPPORT
        //set email flag
        if(email_flag_ptr != PNULL)
        {
            if(old_contact_ptr != PNULL
               &&old_contact_ptr->mail.wstr_len > 0)
            {
                *email_flag_ptr = TRUE;
            }
            else
            {
                *email_flag_ptr = FALSE;
            }
        }
#endif
#ifdef CMCC_VCARD_SUPPORT
        //set sne flag
        if(sne_flag_ptr != PNULL)
        {
            if(
                old_contact_ptr != PNULL
                &&old_contact_ptr->alias.wstr_len > 0)
            {
                *sne_flag_ptr = TRUE;
            }
            else
            {
                *sne_flag_ptr = FALSE;
            }
        }
#endif
    }
    else 
    {
        //add or edit a contact
        if(old_contact_ptr == PNULL)
        {
            is_add_new = TRUE;
        }
        //set adn flag//bug355329
        if(adn_flag_ptr != PNULL)
        {
        	//不管是新增还是编辑，只要输入的名字不为0或者号码合法时，就将ADN的flag置为TRUE
            if((new_contact_ptr->name.wstr_len > 0||MMIPB_IsValidPhoneNumber(new_contact_ptr->number[0].npi_ton, new_contact_ptr->number[0].number_len)))//新加记录 
        	{
                *adn_flag_ptr = TRUE;
				SCI_TRACE_LOW("[MMIPB] ADN SetUSIMUpdateFlag new_contact_ptr->number[0].npi_ton=%d,new_contact_ptr->number[0].number_len=%d,new_contact_ptr->name.wstr_len=%d",
					new_contact_ptr->number[0].npi_ton,new_contact_ptr->number[0].number_len,new_contact_ptr->name.wstr_len);
        	}
            else
            {
                *adn_flag_ptr = FALSE;
            }
			SCI_TRACE_LOW("[MMIPB] new_contact_ptr==PNULL SetUSIMUpdateFlag adn_flag_ptr=%d",adn_flag_ptr);
        }
		//set anr flag
        if(anr_flag_ptr != PNULL)
        {
            for(i = 0; i < MIN(MN_MAX_USIM_ANR_NUM, (MMIPB_MAX_NV_PHONE_NUM -1)); i++)/*lint !e809 !e506*/
            {
                //新加一个有效号码，或者编辑一个号码
                anr_flag_ptr[i] = FALSE;
                if(is_add_new)//新加记录 
                {
                    //新加一个有效号码 
                    if(MMIPB_IsValidPhoneNumber(new_contact_ptr->number[i+1].npi_ton, new_contact_ptr->number[i+1].number_len))
                    {
                        anr_flag_ptr[i] = TRUE;
                    }
				SCI_TRACE_LOW("[MMIPB] ANR SetUSIMUpdateFlag new_contact_ptr->number[%d+1].npi_ton=%d,new_contact_ptr->number[%d+1].number_len=%d",
					i,new_contact_ptr->number[i+1].npi_ton,i,new_contact_ptr->number[i+1].number_len);
                }
                //编译 记录
                else if(
                    old_contact_ptr != PNULL
                    &&memcmp(&old_contact_ptr->number[i+1], 
                    &new_contact_ptr->number[i+1], sizeof(MMIPB_BCD_NUMBER_T)))
                {
                    //新旧号码不同，编辑号码
                    anr_flag_ptr[i] = TRUE;
                }
				SCI_TRACE_LOW("[MMIPB] new_contact_ptr!=PNULL SetUSIMUpdateFlag anr_flag_ptr[%d]=%d",i,anr_flag_ptr[i]);
            }
        }
#ifdef MMIPB_MAIL_SUPPORT
        //set email flag
        if(email_flag_ptr != PNULL)
        {
            if(is_add_new && new_contact_ptr->mail.wstr_len > 0)
            {
                //新增一个email
                *email_flag_ptr = TRUE;
            }
            else if(!is_add_new 
              &&old_contact_ptr != PNULL    
              && memcmp(&new_contact_ptr->mail, &old_contact_ptr->mail, sizeof(MMIPB_MAIL_T )))
            {
                //编辑email
                *email_flag_ptr = TRUE;
            }
            else
            {
                *email_flag_ptr = FALSE;
            }
        }
#endif
#ifdef CMCC_VCARD_SUPPORT
        //set sne falg
        if(sne_flag_ptr != PNULL)
        {
            if(is_add_new && new_contact_ptr->alias.wstr_len > 0)
            {
                //新增一个alias
                *sne_flag_ptr = TRUE;
            }
            else if(!is_add_new
                &&old_contact_ptr != PNULL
                && memcmp(&new_contact_ptr->alias, &old_contact_ptr->alias, sizeof(MMIPB_NAME_T )))
            {
                //编辑alias
                *sne_flag_ptr = TRUE;
            }
            else
            {
                *sne_flag_ptr = FALSE;
            }
        }
#endif
    }

}

/*****************************************************************************/
//  Description : 写USIM联系人信息
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
LOCAL MMIPB_ERROR_E WriteUSIMContact(
                           uint16 contact_id,//
                           uint16 storage_id,//
                           MMIPB_CONTACT_T *contact_ptr//[IN]
                           )
{
    MMIPB_ERROR_E result = MMIPB_ERROR_INVALID_PARAM;
    MN_DUAL_SYS_E dual_sys = MMIPB_GetDualSysByStorageId(storage_id);
    PHONEBOOK_ENTRY_T  *simcontact_ptr = PNULL;
    PHONEBOOK_ADDITIONAL_ENTRY_T    *additional_entry_ptr = PNULL;
    MMIPB_CONTACT_T      *old_contact = PNULL;
    MMIPB_ACCESS_POINT_T access_point = {0};
    BOOLEAN                         adn_flag = FALSE;//bug355329
    BOOLEAN                         anr_flag[MN_MAX_USIM_ANR_NUM] = {0};
    BOOLEAN                         email_flag = FALSE;
    BOOLEAN                         sne_flag = FALSE;
 
    if(dual_sys >= MMI_DUAL_SYS_MAX || PNULL == contact_ptr)
    {
        //SCI_TRACE_LOW:"[MMIPB]WriteUSimContact invalid param !"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_INTERFACE_790_112_2_18_2_43_29_202,(uint8*)"");
        return result;
    }

    old_contact = (MMIPB_CONTACT_T *)SCI_ALLOCA(sizeof(MMIPB_CONTACT_T));
    SCI_MEMSET(old_contact, 0x00, sizeof(MMIPB_CONTACT_T));
    
    contact_ptr->contact_id = contact_id;
    contact_ptr->storage_id = storage_id;
    //read old contact
    access_point.contact.contact_id = contact_id;
    access_point.contact.storage_id = storage_id;
    old_contact->contact_id =contact_id;
    old_contact->storage_id = storage_id;
    if(MMIPB_ERROR_SUCCESS == MMIPB_Read(MMIPB_ACCESS_CONTACT, access_point, sizeof(MMIPB_CONTACT_T), old_contact))
    {
        //old contact exit, edit this contact
        SetUSIMUpdateFlag(old_contact, contact_ptr, &adn_flag, anr_flag, &email_flag, &sne_flag);//bug355329
    }
    else
    {
        //add new contact
        SetUSIMUpdateFlag(PNULL, contact_ptr, &adn_flag, anr_flag, &email_flag, &sne_flag);//bug355329
    }
    //convert to sim contact   
    simcontact_ptr = (PHONEBOOK_ENTRY_T *)SCI_ALLOCA(sizeof(PHONEBOOK_ENTRY_T));
    if(simcontact_ptr != PNULL)
    {
        SCI_MEMSET(simcontact_ptr, 0x00, sizeof(PHONEBOOK_ENTRY_T));
        //convert to sim contact   
        if(MMIPB_ERROR_SUCCESS == MMIPB_ContactToSIMContact(contact_ptr, 
            MMIPB_NUMBER_MAX_STRING_LEN,
            simcontact_ptr))
        {
            //convert to addtional contact
            additional_entry_ptr = (PHONEBOOK_ADDITIONAL_ENTRY_T *)SCI_ALLOCA(sizeof(PHONEBOOK_ADDITIONAL_ENTRY_T));
            if(additional_entry_ptr != PNULL)
            {
                SCI_MEMSET(additional_entry_ptr, 0x00, sizeof(PHONEBOOK_ADDITIONAL_ENTRY_T));
                if(MMIPB_ERROR_SUCCESS == MMIPB_ContactToUSIMAddtionalContact(contact_ptr, additional_entry_ptr))
                {
                    //write usim
                    result = MMIPB_WriteUSIMContact(access_point.contact.contact_id,
                        dual_sys,
                        MMIPB_GET_CONTACT_TYPE(access_point.contact.storage_id),
                        anr_flag,
                        email_flag,
                        sne_flag,
                        adn_flag,//TRUE,//bug355329
                        simcontact_ptr,
                        additional_entry_ptr
                        );
                }
                SCI_FREE(additional_entry_ptr);
            }
        }
        SCI_FREE(simcontact_ptr);
    }
    //SCI_TRACE_LOW:"[MMIPB] WriteUSIMContact contact_id %d storage_id %d result %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_INTERFACE_846_112_2_18_2_43_29_203,(uint8*)"ddd", contact_id, storage_id, result);

    SCI_FREE(old_contact);
    return result;
}

/*****************************************************************************/
//  Description : 删除USIM联系人信息
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
LOCAL MMIPB_ERROR_E DeleteUSIMContact(
                            uint16 contact_id,
                            uint16 storage_id
                            )
{
    MMIPB_ERROR_E result = MMIPB_ERROR_INVALID_PARAM;
    MN_DUAL_SYS_E dual_sys = MMIPB_GetDualSysByStorageId(storage_id);
    MMIPB_CONTACT_T *old_contact = PNULL;  
    MMIPB_ACCESS_POINT_T access_point = {0};
    BOOLEAN                         adn_flag = FALSE;//bug355329
    BOOLEAN                         anr_flag[MN_MAX_USIM_ANR_NUM] = {0};
    BOOLEAN                         email_flag = FALSE;
    BOOLEAN                         sne_flag = FALSE;

    old_contact = (MMIPB_CONTACT_T *)SCI_ALLOCA(sizeof(MMIPB_CONTACT_T));
    //read old contact
    access_point.contact.contact_id = contact_id;
    access_point.contact.storage_id = storage_id;
    old_contact->contact_id =contact_id;
    old_contact->storage_id = storage_id;
    if(MMIPB_ERROR_SUCCESS == MMIPB_Read(MMIPB_ACCESS_CONTACT, access_point, sizeof(MMIPB_CONTACT_T), old_contact))
    {
        //old contact exit, edit this contact
        SetUSIMUpdateFlag(old_contact, PNULL, &adn_flag, anr_flag, &email_flag, &sne_flag);//bug355329
        result = MMIPB_DeleteUSIMContact( access_point.contact.contact_id,
            dual_sys,
            MMIPB_GET_CONTACT_TYPE(access_point.contact.storage_id),
            anr_flag,
            email_flag,
            sne_flag
            );
    }
    else
    {
        //contact no exist
        result = MMIPB_ERROR_NO_ENTRY;
    }

    SCI_FREE(old_contact);
    //SCI_TRACE_LOW:"[MMIPB] DeleteUSIMContact contact_id %d storage_id %d result %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_INTERFACE_894_112_2_18_2_43_29_204,(uint8*)"ddd", contact_id, storage_id, result);
    return result;
}

/*****************************************************************************/
//  Description : 读取USIM 扩展号码的类型字符串
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
LOCAL MMIPB_ERROR_E ReadUSIMAASString (
                           MMIPB_ACCESS_POINT_T access_point,//
                           uint32 data_len,//
                           void *contact_ptr//[OUT]
                           )
{
    return MMIPB_ReadUSIMAASString(access_point.aas_index_t.dual_sys,
                                  access_point.aas_index_t.aas_index,
                                  (MMIPB_NAME_T *)contact_ptr
                                  );
}

/*****************************************************************************/
//  Description : 写USIM 扩展号码的类型字符串
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
LOCAL MMIPB_ERROR_E WriteUSIMAASString(
                           MMIPB_ACCESS_POINT_T access_point,//
                           uint32 data_len,//
                           void *contact_ptr//[IN]
                           )
{
    return MMIPB_WriteUSIMAASString(access_point.aas_index_t.dual_sys,
                                  access_point.aas_index_t.aas_index,
                                  (MMIPB_NAME_T *)contact_ptr
                                  );
}


/*****************************************************************************/
//  Description : 读取usim 支持的扩展号码的类型个数
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMIPB_ReadUSIMAASCount(
                           MN_DUAL_SYS_E dual_sys
                           )
{
    return MMIPB_GetUSIMAASCount(dual_sys);
}
/*****************************************************************************/
//  Description : 读取usim 支持的扩展号码的可用个数
//  Global resource dependence : 
//  Author:yanyan.an
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMIPB_ReadUSIMFreeANRCount(
                           MN_DUAL_SYS_E dual_sys
                           )
{
    return MMIPB_GetUSIMFreeANRCount(dual_sys);
}
/*****************************************************************************/
//  Description : USIM 扩展信息保存到索引表中和号码查找表中
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_SaveUSIMAddtionContact(                                       
                                            uint16       contact_id,
                                            uint16       storage_id,
                                            MMIPB_CONTACT_T *contact_ptr
                                           )
{
   return MMIPB_SaveAddtionToList(contact_id, storage_id, contact_ptr);
}

#endif

/*****************************************************************************/
//  Description : 读取联系人信息
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
LOCAL MMIPB_ERROR_E ReadContact (
                           MMIPB_ACCESS_POINT_T access_point,//
                           uint32 data_len,//
                           void *contact_ptr//[OUT]
                           )
{
    MMIPB_ERROR_E result = MMIPB_ERROR_INVALID_PARAM;
    MN_DUAL_SYS_E dual_sys = MMIPB_GetDualSysByStorageId(access_point.contact.storage_id);
    MMIPB_CONTACT_PHONE_T *phonecontact_ptr = PNULL;
#ifdef MMIPB_MOST_USED_SUPPORT  
    MMIPB_CONTACT_MOSTUSED_T *mostusedcontact_ptr = PNULL;
#endif
    PHONEBOOK_ENTRY_T *simcontact_ptr = PNULL;
    
    if(MMIPB_GET_STORAGE(access_point.contact.storage_id) == MMIPB_STORAGE_PHONE)
    {
#ifdef MMIPB_MOST_USED_SUPPORT          
        if(MMIPB_GET_CONTACT_TYPE(access_point.contact.storage_id) == MMIPB_MOSTUSED_PHONE_CONTACT)
        {
            //most used contact
            mostusedcontact_ptr = (MMIPB_CONTACT_MOSTUSED_T *)SCI_ALLOCA(sizeof(MMIPB_CONTACT_MOSTUSED_T));
            if(mostusedcontact_ptr != PNULL)
            {
                SCI_MEMSET(mostusedcontact_ptr, 0x00, sizeof(MMIPB_CONTACT_MOSTUSED_T));
                result = MMIPB_ReadMostUsedContact(access_point.contact.contact_id, 
                                                   access_point.contact.storage_id,
                                                   mostusedcontact_ptr);
                if(result == MMIPB_ERROR_SUCCESS)
                {
                    result = MMIPB_MostUsedContactToContact(mostusedcontact_ptr, contact_ptr);
                }
                SCI_FREE(mostusedcontact_ptr)
            }
        }
        else
#endif            
        {
            //Phone contact
            phonecontact_ptr = (MMIPB_CONTACT_PHONE_T *)SCI_ALLOCA(sizeof(MMIPB_CONTACT_PHONE_T));
            if(phonecontact_ptr != PNULL)
            {
                SCI_MEMSET(phonecontact_ptr, 0x00, sizeof(MMIPB_CONTACT_PHONE_T));
                {
                
                    result = MMIPB_ReadPhoneContact(access_point.contact.contact_id, 
                                                    access_point.contact.storage_id,
                                                    phonecontact_ptr);
                }
                if(result == MMIPB_ERROR_SUCCESS)
                {
                    result = MMIPB_PhoneContactToContact(phonecontact_ptr, contact_ptr);
                }
                SCI_FREE(phonecontact_ptr);
            }
        }

    }
    else if(dual_sys < MMI_DUAL_SYS_MAX)
    {
        //sim contact
#ifdef CMCC_VCARD_SUPPORT
        if(MMIPB_IsUsim(access_point.contact.storage_id))
        {
            result = ReadUSIMContact(access_point, data_len, contact_ptr);
        }
        else
#endif            
        {
            simcontact_ptr = (PHONEBOOK_ENTRY_T *)SCI_ALLOCA(sizeof(PHONEBOOK_ENTRY_T));
            if(simcontact_ptr != PNULL)
            {
                SCI_MEMSET(simcontact_ptr, 0x00, sizeof(PHONEBOOK_ENTRY_T));
                result = MMIPB_ReadSIMContact(access_point.contact.contact_id,
                    dual_sys,
                    MMIPB_GET_CONTACT_TYPE(access_point.contact.storage_id),
                    simcontact_ptr);
                if(MMIPB_ERROR_SUCCESS == result)
                {
                    MMIPB_SIMContactToContact(simcontact_ptr, 
                                             MMIPB_GetDualSysByStorageId(access_point.contact.storage_id),
                                             MMIPB_GET_CONTACT_TYPE(access_point.contact.storage_id),
                                             contact_ptr);
                }
                SCI_FREE(simcontact_ptr);
            }
        }
    }
    //SCI_TRACE_LOW("[MMIPB] ReadContact contact_id %d storage_id %d result %d", access_point.contact.contact_id, access_point.contact.storage_id, result);

    return result;
}

/*****************************************************************************/
//  Description : 保存联系人信息
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
LOCAL MMIPB_ERROR_E WriteContact (
                            MMIPB_ACCESS_POINT_T access_point,//
                            uint32 contact_size,
                            void *contact_ptr//[IN]
                            )
{
    MMIPB_ERROR_E result = MMIPB_ERROR_INVALID_PARAM;
    MN_DUAL_SYS_E dual_sys = MMIPB_GetDualSysByStorageId(access_point.contact.storage_id);
    MMIPB_CONTACT_PHONE_T *phonecontact_ptr = PNULL;
#ifdef MMIPB_MOST_USED_SUPPORT 
    MMIPB_CONTACT_MOSTUSED_T *mostusedcontact_ptr = PNULL;
#endif
    PHONEBOOK_ENTRY_T *simcontact_ptr = PNULL;
#ifdef MMIPB_UID_SUPPORT
    uint32 uid = MMIPB_GetMaxUid();
#endif    
    if(MMIPB_GET_STORAGE(access_point.contact.storage_id) == MMIPB_STORAGE_PHONE)
    {

        if(MMIPB_GET_CONTACT_TYPE(access_point.contact.storage_id) == MMIPB_NORMAL_PHONE_CONTACT)
        {
            //write Phone contact
            phonecontact_ptr = (MMIPB_CONTACT_PHONE_T *)SCI_ALLOCA(sizeof(MMIPB_CONTACT_PHONE_T));
            if(phonecontact_ptr != PNULL)
            {
                SCI_MEMSET(phonecontact_ptr, 0x00, sizeof(MMIPB_CONTACT_PHONE_T));
#ifdef MMIPB_UID_SUPPORT
                    if(((MMIPB_CONTACT_T*)contact_ptr)->uid == 0)
                    {
                        if(uid== 0XFFFFFFFF)
                        {
                            SCI_TRACE_LOW("MMIPB WriteContact UID EXCCED MAXID");
                            uid = 0;
                        }
                        uid++;
                        ((MMIPB_CONTACT_T*)contact_ptr)->uid = uid;
                    }  
#endif
                //convert to phone contact
                if(MMIPB_ERROR_SUCCESS == MMIPB_ContactToPhoneContact(contact_ptr, phonecontact_ptr))
                {

                    {
                    
                        result = MMIPB_WritePhoneContact(access_point.contact.contact_id,
                                                         access_point.contact.storage_id,
                                                         phonecontact_ptr);
                    }
                    if(result == MMIPB_ERROR_SUCCESS)
                    {
#ifdef MMIPB_UID_SUPPORT
        MMIPB_SetMaxUid(uid);
#endif 
                    }
                }
                SCI_FREE(phonecontact_ptr);
            }
        }
#ifdef MMIPB_MOST_USED_SUPPORT          
        else
        {
            //write most used contact
            mostusedcontact_ptr = (MMIPB_CONTACT_MOSTUSED_T *)SCI_ALLOCA(sizeof(MMIPB_CONTACT_MOSTUSED_T));

            if(mostusedcontact_ptr != PNULL)
            {
                SCI_MEMSET(mostusedcontact_ptr, 0x00, sizeof(MMIPB_CONTACT_MOSTUSED_T));
                //convert to mostused contact
                if(MMIPB_ERROR_SUCCESS == MMIPB_ContactToMostUsedContact(contact_ptr, mostusedcontact_ptr))
                {
                      result = MMIPB_WriteMostUsedContact(access_point.contact.contact_id,
                                                 access_point.contact.storage_id,
                                                 mostusedcontact_ptr
                                                );
                       
                }
                SCI_FREE(mostusedcontact_ptr);
            }

        }
#endif        
    }
    else if(dual_sys < MMI_DUAL_SYS_MAX)
    {
        //write sim contact
#ifdef CMCC_VCARD_SUPPORT
        if(MMIPB_IsUsim(access_point.contact.storage_id))
        {
            
              result = WriteUSIMContact(access_point.contact.contact_id,
                                       access_point.contact.storage_id,
                                       (MMIPB_CONTACT_T *)contact_ptr);
        }
        else
#endif
        {
            //write sim
            simcontact_ptr = (PHONEBOOK_ENTRY_T *)SCI_ALLOCA(sizeof(PHONEBOOK_ENTRY_T));
            if(simcontact_ptr != PNULL)
            {               
                    SCI_MEMSET(simcontact_ptr, 0x00, sizeof(PHONEBOOK_ENTRY_T));
                    //convert to sim contact
                    if(MMIPB_ERROR_SUCCESS == MMIPB_ContactToSIMContact(contact_ptr, 
                        MMIPB_NUMBER_MAX_STRING_LEN,
                        simcontact_ptr))
                    {
                        result = MMIPB_WriteSIMContact(
                            access_point.contact.contact_id,
                            dual_sys,
                            MMIPB_GET_CONTACT_TYPE(access_point.contact.storage_id),
                            simcontact_ptr                                              
                            );
                    }

                SCI_FREE(simcontact_ptr);
            }
        }
    }
    //SCI_TRACE_LOW:"[MMIPB] WriteContact contact_id %d storage_id %d result %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_INTERFACE_1137_112_2_18_2_43_29_205,(uint8*)"ddd", access_point.contact.contact_id, access_point.contact.storage_id, result);

    return result;
}

/*****************************************************************************/
//  Description : 删除联系人信息
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
LOCAL MMIPB_ERROR_E DeleteContact(
                            MMIPB_ACCESS_POINT_T access_point
                            )
{
    MMIPB_ERROR_E result = MMIPB_ERROR_INVALID_PARAM;
    MN_DUAL_SYS_E dual_sys = MMIPB_GetDualSysByStorageId(access_point.contact.storage_id);

    if(MMIPB_GET_STORAGE(access_point.contact.storage_id) == MMIPB_STORAGE_PHONE)
    {
#ifdef MMIPB_MOST_USED_SUPPORT          
        if(MMIPB_GET_CONTACT_TYPE(access_point.contact.storage_id) == MMIPB_NORMAL_PHONE_CONTACT)
        {
            //delete Phone contact
           result= MMIPB_DeletePhoneContact(access_point.contact.contact_id, 
                                     access_point.contact.storage_id);
        }        
        else
        {
            //delete mostused contact
            result= MMIPB_DeleteMostContact(access_point.contact.contact_id, 
                                     access_point.contact.storage_id);
        }
#else
           result= MMIPB_DeletePhoneContact(access_point.contact.contact_id, 
                                     access_point.contact.storage_id);
#endif
    }
    else if(dual_sys < MMI_DUAL_SYS_MAX)
    {
        //delete sim contact
#ifdef CMCC_VCARD_SUPPORT
        if(MMIPB_IsUsim(access_point.contact.storage_id))
        {
            //usim
            result= DeleteUSIMContact(access_point.contact.contact_id, access_point.contact.storage_id);
            
        }
        else
#endif
        {
            //sim
            result= MMIPB_DeleteSIMContact(access_point.contact.contact_id,
                dual_sys,
                MMIPB_GET_CONTACT_TYPE(access_point.contact.storage_id)
                );
        }
    }
    //SCI_TRACE_LOW:"[MMIPB] DeleteContact contact_id %d storage_id %d result %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_INTERFACE_1188_112_2_18_2_43_30_206,(uint8*)"ddd", access_point.contact.contact_id, access_point.contact.storage_id, result);

    return result;
            
}
#ifdef MMI_VCARD_SUPPORT
/*****************************************************************************/
//  Description : 读vcard联系人信息
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
LOCAL MMIPB_ERROR_E ReadVcardContact(
                            MMIPB_ACCESS_POINT_T access_point,
                            uint32 data_len,//
                            void *contact_ptr//[OUT]
                            )
{
    MMIPB_ERROR_E result = MMIPB_ERROR_INVALID_PARAM;

    VCARD_ERROR_E  vcard_ret = 0;
    if(contact_ptr == PNULL)
    {
        //SCI_TRACE_LOW:"[MMIPB]ReadVcardContact invalid param !"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_INTERFACE_1210_112_2_18_2_43_30_207,(uint8*)"");
        return result;
    }
    //read vcard

    vcard_ret = MMIAPIVCARD_ReadContact(access_point.file_handle, contact_ptr, PNULL);

    if(VCARD_NO_ERROR == vcard_ret)
    {
        result = MMIPB_ERROR_SUCCESS;
    }
    else if(VCARD_FILE_TAIL == vcard_ret)
    {
       result = MMIPB_ERROR_NO_ENTRY;
    }
    else if(VCARD_SPACE_FULL == vcard_ret)
    {
        result = MMIPB_ERROR_NO_SPACE;
    }
    else if(VCARD_PARAM_ERROR == vcard_ret)
    {
        result = MMIPB_ERROR_INVALID_FORMAT;
    }
    else
    {
        result = MMIPB_ERROR_SFS_ERROR;
    }

    //SCI_TRACE_LOW:"[MMIPB] ReadVcardContact  result %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_INTERFACE_1230_112_2_18_2_43_30_208,(uint8*)"d", result);
    return result;
}


/*****************************************************************************/
//  Description : 保存联系人到vcard
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
LOCAL MMIPB_ERROR_E WriteVcardContact (
                            MMIPB_ACCESS_POINT_T access_point,//
                            uint32 contact_size,
                            void *contact_ptr//[IN]
                            )
{
    MMIPB_ERROR_E result = MMIPB_ERROR_INVALID_PARAM;
    uint8         *data_ptr = PNULL;
    uint32        out_data_len = VCARD_MAX_FILE_LEN;

    if(contact_ptr == PNULL)
    {
        //SCI_TRACE_LOW:"[MMIPB] WriteVcardContact invalid param !"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_INTERFACE_1252_112_2_18_2_43_30_209,(uint8*)"");
        return result;
    }

    data_ptr = (uint8 *)SCI_ALLOCA(VCARD_MAX_FILE_LEN);
    if(data_ptr != PNULL)
    {
        SCI_MEMSET(data_ptr, 0x00, VCARD_MAX_FILE_LEN); 
        //convert contact to vcard
        if(MMIPB_ERROR_SUCCESS == MMIPB_ContactToVcard(contact_ptr,
                                VCARD_MAX_FILE_LEN,
                                data_ptr,
                                &out_data_len
                                ))
        {
            //save to vcard file
            result = MMIPB_WriteVcardContact(access_point.file_handle, out_data_len, data_ptr);
        }
        SCI_FREE(data_ptr);
    }
    //SCI_TRACE_LOW:"[MMIPB]WriteVcardContact result %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_INTERFACE_1272_112_2_18_2_43_30_210,(uint8*)"d", result);
    return result;
}
#endif

/*****************************************************************************/
//  Description : 读分组信息
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
LOCAL MMIPB_ERROR_E ReadGroup(
                            MMIPB_ACCESS_POINT_T access_point,
                            uint32 data_len,//
                            void *contact_ptr//[OUT]
                            )
{
    //读分组信息
    return MMIPB_ReadGroup(access_point.group_id , contact_ptr);
}
#ifdef SNS_SUPPORT
/*****************************************************************************/
//  Description : 读sns信息
//  Global resource dependence : 
//  Author:MARYXIAO
//  Note:
/*****************************************************************************/
LOCAL MMIPB_ERROR_E ReadSnsContact(
                            MMIPB_ACCESS_POINT_T access_point,
                            uint32 data_len,//
                            void *contact_ptr//[OUT]
                            )

{
    MMIPB_ERROR_E ret = MMIPB_ERROR_ERROR;
    MMIPB_SNS_CONTACT_T   *sns_contact_info = PNULL;

    sns_contact_info = SCI_ALLOC_APPZ(sizeof(MMIPB_SNS_CONTACT_T));

    ret =  MMIPB_ReadSnsContact(access_point.sns_contact.sns_type, access_point.sns_contact.index,sns_contact_info);
    //SCI_TRACE_LOW:"[MMIPB][SNS] ReadSnsContact index %d, ret %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_INTERFACE_1326_112_2_18_2_43_30_212,(uint8*)"dd", ret,access_point.sns_contact.index);
    if(ret == MMIPB_ERROR_SUCCESS)
    {
        MMIPB_SnsToContact(sns_contact_info, contact_ptr);
    }
    
    SCI_Free(sns_contact_info);
    return ret;
}
#endif

#ifdef MMIPB_GROUP_MANAGE_SUPPORT
/*****************************************************************************/
//  Description : 保存分组信息
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
LOCAL MMIPB_ERROR_E WriteGroup (
                            MMIPB_ACCESS_POINT_T access_point,//
                            uint32 contact_size,
                            void *contact_ptr//[IN]
                            )
{
    return MMIPB_WriteGroup(access_point.group_id , contact_ptr);
}





/*****************************************************************************/
//  Description : 删除分组信息
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
LOCAL MMIPB_ERROR_E DeleteGroup(
                            MMIPB_ACCESS_POINT_T access_point
                            )
{
    return MMIPB_DeleteGroup(access_point.group_id);
}

#endif
/*****************************************************************************/
//  Description : 执行等待队列中的下个请求
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
LOCAL void DoNextWaitingReuest(void)
{
    MMIPB_MSG_QUEUE_NODE_T *wait_msg = PNULL;
    MMIPB_HANDLE_NODE_T handle_node = {0};
    MMIPB_ERROR_E       ret = MMIPB_ERROR_ERROR;

    //执行等待队列中消息
    while(!GetTaskStatus() && !MMIQUEUE_QueueIsEmpty(s_mmipb_wait_queue))
    {
        wait_msg = (MMIPB_MSG_QUEUE_NODE_T *)MMIQUEUE_PeekQueue(s_mmipb_wait_queue, 0);
        MMIQUEUE_OutQueue(s_mmipb_wait_queue);           
        if(wait_msg != PNULL)
        {
            if(MMIPB_GetHandleContent(wait_msg->cmd_handle, &handle_node))
            {
                if(MMIPB_READ_MODE == wait_msg->pb_mode)
                {
                    ret = MMIPB_ReadAsyn(wait_msg->cmd_handle,
                        handle_node.access_type,
                        handle_node.access_point,
                        handle_node.data_ptr,
                        handle_node.data_len,
                        handle_node.callback);
                    //sig_id = MMIPB_SIG_READ;
                }
                else if(MMIPB_WRITE_MODE == wait_msg->pb_mode)
                {
                    ret = MMIPB_WriteAsyn(wait_msg->cmd_handle,
                        handle_node.access_type,
                        handle_node.access_point,
                        handle_node.data_ptr,
                        handle_node.data_len,
                        handle_node.callback);
                }
                else if(MMIPB_DELETE_MODE == wait_msg->pb_mode)
                {
                    ret = MMIPB_DeleteAsyn(wait_msg->cmd_handle,
                        handle_node.access_type,
                        handle_node.access_point,
                        handle_node.callback);
                }
                if(MMIPB_ERROR_PENDING == ret)
                {
                    break;
                }
                else if(handle_node.callback != PNULL)
                {
                    handle_node.callback(wait_msg->cmd_handle, ret);                        
                }
            }
        } 
    }
}
#ifdef SNS_SUPPORT
/*****************************************************************************/
//  Description : 打开sns数据库
//  
//  NOTE: 
/*****************************************************************************/
PUBLIC void MMIPB_OpenSNSDB(uint16 sns_type)
{
    MMIPB_CreateSNSDBHandle(sns_type);
}

/*****************************************************************************/
//  Description : close sns database
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIPB_CloseSNSDB(void)
{
    MMIPB_CloseSNSDBHandle();
}
#endif

/*****************************************************************************/
//  Description : 联系人信息保存到索引表中和号码查找表中
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_InsertContactToList( 
                                               MMIPB_CONTACT_T *contact_ptr
                                               )
{
    MMIPB_ERROR_E result = MMIPB_ERROR_ERROR;

    if(contact_ptr != PNULL)
    {
#ifdef MMIPB_MOST_USED_SUPPORT         
        //insert to contact list
        if(MMIPB_IsMostUsedContact(contact_ptr->storage_id))
        {
            result = MMIPB_InsertMostUsedContactListNode((MMIPB_CONTACT_T *)contact_ptr);
        }
        else
#endif            
        {
            result = MMIPB_InsertContactListNode((MMIPB_CONTACT_T *)contact_ptr);
        }
        //insert to number list
        if(result == MMIPB_ERROR_SUCCESS
#ifdef SNS_SUPPORT            
            && (!MMIPB_IsSNSContact(contact_ptr->storage_id))
#endif            
            )
        {
            //sns联系人不需要号码匹配
            result = MMIPB_InsertNumberToList(contact_ptr);
        }
    }

    return result;
}

/*****************************************************************************/
//  Description : 从索引表中和号码查找表中删除一个节点
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_RemoveContactFromList(
                            uint16 contact_id,
                            uint16 storage_id
                            )
{
    MMIPB_ERROR_E result = MMIPB_ERROR_ERROR;
#ifdef MMIPB_MOST_USED_SUPPORT     
    if(MMIPB_IsMostUsedContact(storage_id))
    {
        //remove from contact list
        result = MMIPB_DeleteMostUsedContactListNode(contact_id, storage_id);        
    }
    else
#endif        
    {
        //remove from contact list
        result = MMIPB_DeleteContactListNode(contact_id, storage_id);        
    }
    //remove from number list
    if(result == MMIPB_ERROR_SUCCESS)
    {
        if(!MMIPB_DelNumberFromList(contact_id, storage_id, PNULL))
        {
            result = MMIPB_ERROR_ERROR;
        }
    }
    return result;
}

/*****************************************************************************/
//  Description : 创建PB的Handle
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_HANDLE_T MMIPB_CreateHandle (
                           MMIPB_OPC_E cmd_type,
                           MMI_HANDLE_T applet_handle
                           )
{
    uint32 i = 0;
    uint32 handle_id = 0;
    MMIPB_HANDLE_NODE_T *node_ptr = PNULL;
    SCI_GetMutex(s_pb_handle_mutex_p, SCI_WAIT_FOREVER);  
    if(s_mmipb_handle_table.handle_count < MMIPB_MAX_HANDLE_NUM)
    {
        node_ptr = (MMIPB_HANDLE_NODE_T *)SCI_ALLOCA(sizeof(MMIPB_HANDLE_NODE_T));
        if(node_ptr != PNULL)
        {
            SCI_MEMSET(node_ptr, 0x00, sizeof(MMIPB_HANDLE_NODE_T));
            node_ptr->cmd_type = cmd_type;
            node_ptr->applet_handle = applet_handle;
            for(i = s_mmipb_handle_table.cur_index; i < MMIPB_MAX_HANDLE_NUM; i++)
            {
                //向后查找
                if(s_mmipb_handle_table.handle_table[i] == PNULL)
                {
                    break;
                }
            }
            if(i >= MMIPB_MAX_HANDLE_NUM)
            {
                //向后没有查到，从最小开始查找
                for(i = 0; i < MMIPB_MAX_HANDLE_NUM; i++)
                {
                    if(s_mmipb_handle_table.handle_table[i] == PNULL)
                    {
                        break;
                    }
                }
            }
            if(i < MMIPB_MAX_HANDLE_NUM)
            {
                s_mmipb_handle_table.cur_index = i +1;
                handle_id = i + MMIPB_HANDLE_TYPE_VALUE;
                s_mmipb_handle_table.handle_table[i] = node_ptr;
                s_mmipb_handle_table.handle_count++;
            }
        }
        else
        {
            SCI_TRACE_LOW("[MMIPB] MMIPB_CreateHandle node_ptr == PNULL !!");          
        }
    }
    else
    {
        SCI_TRACE_LOW("[MMIPB] MMIPB_CreateHandle handle_count %d !!", s_mmipb_handle_table.handle_count);          
    }
    SCI_PutMutex(s_pb_handle_mutex_p);  
    //SCI_TRACE_LOW:"[MMIPB]MMIPB_CreateHandle applet_handle %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_INTERFACE_1552_112_2_18_2_43_30_213,(uint8*)"d", applet_handle);
    return (MMIPB_HANDLE_T)handle_id;
}

/*****************************************************************************/
//  Description : 销毁PB的Handle
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC void MMIPB_CloseHandle (
                               MMIPB_HANDLE_T handle
                               )
{
    uint32 handle_id = handle & MMIPB_HANDLE_ID_MASK;
    SCI_GetMutex(s_pb_handle_mutex_p, SCI_WAIT_FOREVER);  
    if(handle > 0)
    {
        if(handle_id < MMIPB_MAX_HANDLE_NUM)
        {
            if(s_mmipb_handle_table.handle_table[handle_id] != PNULL)
            {
                SCI_FREE(s_mmipb_handle_table.handle_table[handle_id]);
                //SCI_MEMSET(s_mmipb_handle_table.handle_table[handle_id], 0x00, sizeof(MMIPB_HANDLE_NODE_T));
                s_mmipb_handle_table.handle_table[handle_id] = PNULL;
            }
            if(s_mmipb_handle_table.handle_count > 0)
            {
                s_mmipb_handle_table.handle_count--;
            }
        }
    }
    SCI_PutMutex(s_pb_handle_mutex_p);  
    //SCI_TRACE_LOW:"[MMIPB]MMIPB_CloseHandle handle %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_INTERFACE_1581_112_2_18_2_43_30_214,(uint8*)"d", handle);
}

/*****************************************************************************/
//  Description : 获取handle所对应的结构体指针
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_GetHandleContent(                                       
                                       MMIPB_HANDLE_T handle,//[IN]
                                       MMIPB_HANDLE_NODE_T *node_ptr//[OUT]
                                       )
{
    uint32 handle_id = handle & MMIPB_HANDLE_ID_MASK;
    BOOLEAN result = FALSE;

    SCI_GetMutex(s_pb_handle_mutex_p, SCI_WAIT_FOREVER);  
    if(handle > 0)
    {
        if(handle_id < MMIPB_MAX_HANDLE_NUM && (node_ptr != PNULL) && (s_mmipb_handle_table.handle_table[handle_id] != PNULL))
        {            
            *node_ptr = *s_mmipb_handle_table.handle_table[handle_id];
            result = TRUE;
        }
    }
    SCI_PutMutex(s_pb_handle_mutex_p);  
    return result;
}

/*****************************************************************************/
//  Description : 设置handle所对应的结构体值
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_SetHandleContent(MMIPB_HANDLE_T handle, MMIPB_HANDLE_NODE_T *handle_node_ptr)
{
    BOOLEAN result = FALSE;
    uint32 handle_id = handle & MMIPB_HANDLE_ID_MASK;

    SCI_GetMutex(s_pb_handle_mutex_p, SCI_WAIT_FOREVER);
    if(handle > 0 && handle_node_ptr != PNULL)
    {
        if(handle_id < MMIPB_MAX_HANDLE_NUM && s_mmipb_handle_table.handle_table[handle_id] != PNULL)
        {
            SCI_MEMCPY(s_mmipb_handle_table.handle_table[handle_id], handle_node_ptr, sizeof(MMIPB_HANDLE_NODE_T));
            result = TRUE;
        }
    }
    SCI_PutMutex(s_pb_handle_mutex_p);
    return result;
}

/*****************************************************************************/
//  Description :等待消息队列的初始化
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC void MMIPB_InitWaitMsgQueue(void)
{
    //异步消息队列的初始化
    s_mmipb_wait_queue = MMIQUEUE_InitQueue(MMIPB_MAX_WAIT_CMD_NUM, sizeof(MMIPB_MSG_QUEUE_NODE_T));
}
/*****************************************************************************/
//  Description : 接口层初始化
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC void MMIPB_InitInterface(void)
{
    MMIPB_STORAGE_INFO_T storage_info_t = {0};
    uint16               sim_number_max_count = MMIPB_SIM_MAX_RECORD_NUM;
    //初始化数据表
#ifdef TCARD_PB_SUPPORT
    MMIPB_InitTcardFileHandle();
#endif
    MMIPB_InitDataList();
    storage_info_t = MMIPB_GetContactStorage(MMI_DUAL_SYS_MAX, PHONEBOOK_NV_STORAGE);
#ifdef CMCC_VCARD_SUPPORT
    sim_number_max_count += MMI_AAS_PHONE_NUM*MMIPB_SIM_MAX_RECORD_NUM;
#endif

    MMIPB_InitNumberList((sim_number_max_count*MMI_DUAL_SYS_MAX + (storage_info_t.max_record_num* MMIPB_MAX_NV_PHONE_NUM)));     
}

/*****************************************************************************/
//  Description : 异步信息读取
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:目前仅用于记录和vcard的读取
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_ReadAsyn (
                              MMIPB_HANDLE_T handle,//[IN]handle id
                              MMIPB_ACCESS_E access_type,//[IN]访问类型
                              MMIPB_ACCESS_POINT_T access_point,//[IN]信息位置信息
                              void *data_ptr,//[OUT]数据指针
                              uint32 data_len,//[IN]数据的最大长度
                              MMIPB_ACCESS_CALLBACK_FUNC callback//[IN]回调函数
                              )
{
   MMIPB_ERROR_E result = MMIPB_ERROR_ERROR;
   MMIPB_HANDLE_NODE_T  handle_node = {0};
   BOOLEAN is_valid = FALSE;
   MMIPB_MSG_QUEUE_NODE_T msg_queue_node = {0};

   is_valid = MMIPB_GetHandleContent(handle, &handle_node);
   SCI_GetMutex(s_pb_asyn_mutex_p, SCI_WAIT_FOREVER);   
   if(is_valid)
   {
       //handle有效
       handle_node.callback = callback;
       handle_node.data_len = data_len;
       handle_node.data_ptr = data_ptr;
       handle_node.access_type = access_type;
       handle_node.access_point = access_point;
       if(!MMIPB_SetHandleContent(handle, &handle_node))
       {
            if(callback != PNULL)
            {
                callback(handle, result);
            }
            return result;
       }
       if(!GetTaskStatus())
       {
           //当前消息队列为空,发送执行请求
           s_mmipb_current_handle = handle;
           //向pb task发送读请求
           MMIPB_SendSignalToPBTask(MMIPB_SIG_READ, handle);
           MMIPB_SetTaskStatus(TRUE);
           result = MMIPB_ERROR_PENDING;
       }
       else
       {
           //当前消息队列不为空
           if(MMIQUEUE_QueueIsFull(s_mmipb_wait_queue))
           {
               //消息队列满，返回MMIPB_ERROR_BUSY
               result = MMIPB_ERROR_BUSY;
               //SCI_TRACE_LOW:"[MMIPB]MMIPB_ReadAsyn queue full !"
               SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_INTERFACE_1665_112_2_18_2_43_31_215,(uint8*)"");
           }
           else
           {
               //消息队列未满，插入等待消息队列
               msg_queue_node.pb_mode = MMIPB_READ_MODE;
               msg_queue_node.cmd_handle = handle;
               if(MMIQUEUE_InQueue(s_mmipb_wait_queue, &msg_queue_node))
               {
                   result = MMIPB_ERROR_PENDING;
               }
           }
       }
      
   }
   SCI_PutMutex(s_pb_asyn_mutex_p);
   if(MMIPB_ERROR_PENDING != result)
   {
       DoNextWaitingReuest();
   }
   //SCI_TRACE_LOW("[MMIPB]MMIPB_ReadAsyn result %d", result);
   return result;
}

/*****************************************************************************/
//  Description : 异步写信息
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:目前仅用于记录和vcard的保存
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_WriteAsyn (
                               MMIPB_HANDLE_T handle,
                               MMIPB_ACCESS_E access_type,
                               MMIPB_ACCESS_POINT_T access_point,
                               void *data_ptr,
                               uint32 data_len,
                               MMIPB_ACCESS_CALLBACK_FUNC callback
                               )
{
   MMIPB_ERROR_E result = MMIPB_ERROR_ERROR;
   BOOLEAN is_valid = FALSE;
   MMIPB_HANDLE_NODE_T  handle_node = {0};
   MMIPB_MSG_QUEUE_NODE_T msg_queue_node = {0};

   is_valid = MMIPB_GetHandleContent(handle, &handle_node);
   SCI_GetMutex(s_pb_asyn_mutex_p, SCI_WAIT_FOREVER); 
   if(is_valid)
   {
       //handle有效
       handle_node.callback = callback;
       handle_node.data_len = data_len;
       handle_node.data_ptr = data_ptr;
       handle_node.access_type = access_type;
       handle_node.access_point = access_point;
       if(!MMIPB_SetHandleContent(handle, &handle_node))
       {
            if(callback != PNULL)
            {
                callback(handle, result);
            }
            return result;
       }
       if(!GetTaskStatus())
       {
           //当前消息队列为空,发送执行请求
           s_mmipb_current_handle = handle;
           //是否sim卡异步写
           if( IsAccessSIMContact(access_type, access_point))
           {
               //发送SIM　task 写请求
               if(g_mmipb_access_table[access_type].read != PNULL)
               {
                   MMIPB_SetTaskStatus(TRUE);
                   result = g_mmipb_access_table[access_type].write(access_point, data_len, data_ptr);
                   if(result != MMIPB_ERROR_PENDING)
                   {
                       MMIPB_SetTaskStatus(FALSE);
                   }
               }
           }
           else
           {
               //向pb task发送写请求
               MMIPB_SetTaskStatus(TRUE);
               MMIPB_SendSignalToPBTask(MMIPB_SIG_WRITE, handle);
               result = MMIPB_ERROR_PENDING;
               
           }
       }
       else
       {
           //当前消息队列不为空
           if(MMIQUEUE_QueueIsFull(s_mmipb_wait_queue))
           {
               //消息队列满，返回MMIPB_ERROR_BUSY
               result = MMIPB_ERROR_BUSY;
               //SCI_TRACE_LOW:"[MMIPB]MMIPB_WriteAsyn queue full !"
               SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_INTERFACE_1749_112_2_18_2_43_31_216,(uint8*)"");
           }
           else
           {
               //消息队列未满，插入等待消息队列
               msg_queue_node.pb_mode = MMIPB_WRITE_MODE;
               msg_queue_node.cmd_handle = handle;
               if(MMIQUEUE_InQueue(s_mmipb_wait_queue, &msg_queue_node))
               {
                   result = MMIPB_ERROR_PENDING;
               }
           }
       }
      
   }
   SCI_PutMutex(s_pb_asyn_mutex_p);
   if(MMIPB_ERROR_PENDING != result)
   {
       DoNextWaitingReuest();
   }
   //SCI_TRACE_LOW:"[MMIPB]MMIPB_WriteAsyn result %d"
   SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_INTERFACE_1769_112_2_18_2_43_31_217,(uint8*)"d", result);
   return result;
}

/*****************************************************************************/
//  Description : 异步信息删除
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:目前仅用于记录的异步删除
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_DeleteAsyn (
                                MMIPB_HANDLE_T handle,
                                MMIPB_ACCESS_E access_type,
                                MMIPB_ACCESS_POINT_T access_point,
                                MMIPB_ACCESS_CALLBACK_FUNC callback
                                )
{
   MMIPB_ERROR_E result = MMIPB_ERROR_ERROR;
   BOOLEAN is_valid = FALSE;
   MMIPB_HANDLE_NODE_T  handle_node = {0};
   MMIPB_MSG_QUEUE_NODE_T msg_queue_node = {0};

   is_valid = MMIPB_GetHandleContent(handle, &handle_node);
   SCI_GetMutex(s_pb_asyn_mutex_p, SCI_WAIT_FOREVER); 
   if(is_valid)
   {
       //handle有效
       handle_node.callback = callback;
       handle_node.access_type = access_type;
       handle_node.access_point = access_point;
       if(!MMIPB_SetHandleContent(handle, &handle_node))
       {
            if(callback != PNULL)
            {
                callback(handle, result);
            }
            return result;
       }
       if(!GetTaskStatus())
       {
           //当前消息队列为空,发送执行请求
           s_mmipb_current_handle = handle;
           //是否sim卡异步操作
           if( IsAccessSIMContact(access_type, access_point))
           {
               //发送SIM　task 删除请求
               if(g_mmipb_access_table[access_type].read != PNULL)
               {
                   MMIPB_SetTaskStatus(TRUE);
                   result = g_mmipb_access_table[access_type].delete(access_point);
                   if(result != MMIPB_ERROR_PENDING)
                   {
                       MMIPB_SetTaskStatus(FALSE);
                   }
               }
           }
           else
           {
               //向pb task发送删除请求
               MMIPB_SetTaskStatus(TRUE);
               MMIPB_SendSignalToPBTask(MMIPB_SIG_DELETE, s_mmipb_current_handle);
               result = MMIPB_ERROR_PENDING;               
           }
       }
       else
       {
           //当前消息队列不为空
           if(MMIQUEUE_QueueIsFull(s_mmipb_wait_queue))
           {
               //消息队列满，返回MMIPB_ERROR_BUSY
               result = MMIPB_ERROR_BUSY;
               //SCI_TRACE_LOW:"[MMIPB]MMIPB_DeleteAsyn queue full !"
               SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_INTERFACE_1824_112_2_18_2_43_31_218,(uint8*)"");
           }
           else
           {
               //消息队列未满，插入等待消息队列
               msg_queue_node.pb_mode = MMIPB_DELETE_MODE;
               msg_queue_node.cmd_handle = handle;
               if(MMIQUEUE_InQueue(s_mmipb_wait_queue, &msg_queue_node))
               {
                   result = MMIPB_ERROR_PENDING;
               }
           }
       }
      
   }
   SCI_PutMutex(s_pb_asyn_mutex_p);
   if(MMIPB_ERROR_PENDING != result)
   {
       DoNextWaitingReuest();
   }
   //SCI_TRACE_LOW:"[MMIPB]MMIPB_DeleteAsyn result %d"
   SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_INTERFACE_1844_112_2_18_2_43_31_219,(uint8*)"d", result);
   return result;
}

/*****************************************************************************/
//  Description : 操作是否是同一应用发起
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_IsCurrentOperation(
                              uint16 contact_id,
                              uint16 storage_id
                              )
{
    MMIPB_HANDLE_NODE_T handle_node = {0};
    BOOLEAN result = FALSE;
    if(MMIPB_GetHandleContent(s_mmipb_current_handle, &handle_node))
    {
        if(handle_node.access_type == MMIPB_ACCESS_CONTACT)
        {
            if(handle_node.access_point.contact.contact_id == contact_id
              &&handle_node.access_point.contact.storage_id == storage_id
             )
            {
                result = TRUE;
            }
        }
    }

    //SCI_TRACE_LOW:"[MMIPB]MMIPB_IsCurrentOperation contact_id %d storage_id %d result %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_INTERFACE_1872_112_2_18_2_43_31_220,(uint8*)"ddd", contact_id, storage_id, result);
    return result;
}


/*****************************************************************************/
//  Description : 异步操作后的处理
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_CmdConfirm(
                              MMIPB_HANDLE_NODE_T *handle_node_ptr,//IN
                              MMIPB_HANDLE_T handle,//[IN]handle id                              
                              MMIPB_ERROR_E  result
                              )
{
    MMIPB_ERROR_E       ret = MMIPB_ERROR_ERROR;
    //uint16 sig_id = 0;
    
    MMIPB_SetTaskStatus(FALSE);
    //执行该handle id对应的callback
    if(handle_node_ptr != PNULL && handle_node_ptr->callback != PNULL)
    {
        handle_node_ptr->callback(handle, result);
        ret = MMIPB_ERROR_SUCCESS;            
    }
    DoNextWaitingReuest();    
    //SCI_TRACE_LOW:"[MMIPB]MMIPB_CmdConfirm handle 0x%x  result %d ret %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_INTERFACE_1900_112_2_18_2_43_31_221,(uint8*)"ddd", handle, result, ret);
    return ret;
}

/*****************************************************************************/
//  Description : 所有普通联系人个数
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC uint16 MMIPB_GetAllNormalContactCount(void)
{
    MMIPB_CONTACT_LIST_INFO_T list_info = {0};
    uint16 count = 0;

    if(MMIPB_ERROR_SUCCESS == MMIPB_ReadContactList(&list_info))
    {
        count = list_info.count;
    }
    //SCI_TRACE_LOW:"[MMIPB]MMIPB_GetAllNormalContactCount count %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_INTERFACE_1917_112_2_18_2_43_31_222,(uint8*)"d", count);
    return count;
}
#ifdef MMIPB_MOST_USED_SUPPORT 
/*****************************************************************************/
//  Description : 所有常用联系人个数
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC uint16 MMIPB_GetAllMostUsedContactCount(void)
{
    uint16 count = 0;
    MMIPB_ReadMostUsedContactCount(&count);
    //SCI_TRACE_LOW:"[MMIPB]MMIPB_GetAllNormalContactCount count %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_INTERFACE_1929_112_2_18_2_43_31_223,(uint8*)"d", count);
    return count;
}
#endif
/*****************************************************************************/
//  Description : 所有普通电话总数
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC uint16 MMIPB_GetAllNormalContactPhoneCount(void)
{
    MMIPB_CONTACT_LIST_INFO_T list_info = {0};
    uint16 count = 0;

    if(MMIPB_ERROR_SUCCESS == MMIPB_ReadContactList(&list_info))
    {
        count = list_info.number_count;
    }

    //SCI_TRACE_LOW:"[MMIPB]MMIPB_GetAllNormalContactPhoneCount count %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_INTERFACE_1947_112_2_18_2_43_31_224,(uint8*)"d", count);
    return count;
}

/*****************************************************************************/
//  Description : 所有普通mail总数
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC uint16 MMIPB_GetAllNormalContactMailCount(void)
{
    MMIPB_CONTACT_LIST_INFO_T list_info = {0};
    uint16 count = 0;

    if(MMIPB_ERROR_SUCCESS == MMIPB_ReadContactList(&list_info))
    {
        count = list_info.mail_count;
    }
    
    //SCI_TRACE_LOW:"[MMIPB]MMIPB_GetAllNormalContactMailCount count %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_INTERFACE_1965_112_2_18_2_43_31_225,(uint8*)"d", count);
    return count;
}


/*****************************************************************************/
//  Description : 某个存储设备中联系人个数
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC uint16 MMIPB_GetContactCount 
(
    uint16 storage_id
)
{
    MMIPB_CONTACT_LIST_INFO_T list_info = {0};
    uint16 count = 0;

#ifdef MMIPB_MOST_USED_SUPPORT
    if(MMIPB_IsMostUsedContact(storage_id))
    {
        return MMIPB_GetAllMostUsedContactCount();
    }
#endif
#ifdef FDN_SUPPORT
    if(MMIPB_IsFdnContact(storage_id))
    {
        //fdn
        return MMIPB_GetFdnContactCount(storage_id);
    }
#endif
   if(MMIPB_IsSimRecord(storage_id) && !MMIPB_IsSIMContact(storage_id))
    {
        //msisdn.sdn, lnd,bdn...
        count = MMIPB_GetSIMDNContactCount(storage_id);
        
    }
    else
    {
        if(MMIPB_ERROR_SUCCESS == MMIPB_ReadContactList(&list_info))
        {
            if(MMIPB_GET_STORAGE(storage_id) < MMIPB_STORAGE_MAX)
            {
                count = list_info.storage_count[MMIPB_GET_STORAGE(storage_id)];
            }
        }
    }

    //SCI_TRACE_LOW:"[MMIPB]MMIPB_GetContactCount storage_id 0x%x count %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_INTERFACE_2007_112_2_18_2_43_31_226,(uint8*)"dd", storage_id, count);
    return count;
}
#ifdef MMIPB_MOST_USED_SUPPORT 
/*****************************************************************************/
//  Description : 删除全部常用联系人
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_CleanAllMostUsedContact(void)
{
    MMIPB_ERROR_E result = MMIPB_ERROR_ERROR;
    uint16 cur_index = 0;
    MMIPB_MOSTUSED_CONTACT_NODE_T find_data = {0};
    MMIPB_HANDLE_T handle = 0;
    MMIPB_ACCESS_POINT_T access_point = {0};
    
    //delete all most used from nv
    result = MMIPB_FindFirstMostUsedContact(&cur_index, &find_data, &handle);
    
    while(result == MMIPB_ERROR_SUCCESS)
    {
        access_point.contact.contact_id = find_data.entry_id;
        access_point.contact.storage_id = find_data.storage_id;
        result= MMIPB_DeleteMostContact(access_point.contact.contact_id, 
                                        access_point.contact.storage_id);
        if(MMIPB_ERROR_SUCCESS == result)
        {
            result = MMIPB_FindNextMostContact(handle, &cur_index, &find_data);
        }
        
    }
    if(handle)
    {
        MMIPB_FindClose(handle);
    }    
   //delete most used from cache
   MMIPB_DeleteAllMostUsedContactListNode();
   //delete all most used number list
   MMIPB_CleanAllMostusedNumber();

   return MMIPB_ERROR_SUCCESS;
   
}
#endif
/*****************************************************************************/
//  Description : 某个分组中联系人个数
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC uint16 MMIPB_GetGroupContactCount
(
    uint16 group_id
)
{
    MMIPB_CONTACT_LIST_INFO_T list_info = {0};
    uint16 count = 0;
#ifdef MMIPB_MOST_USED_SUPPORT 
    if(PB_GROUP_MOSTUSED == group_id)
    {
        count = MMIPB_GetAllMostUsedContactCount();
    }
    else
#endif
    {
        if(MMIPB_ERROR_SUCCESS == MMIPB_ReadContactList(&list_info))
        {
            if(group_id < MMIPB_MAX_GROUP_NUM)
            {
                count = list_info.group_count[group_id];
            }
        }
    }
    //SCI_TRACE_LOW:"[MMIPB]MMIPB_GetGroupContactCount group_id 0x%x count %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_INTERFACE_2075_112_2_18_2_43_32_227,(uint8*)"dd", group_id, count);
    return count;
}

/*****************************************************************************/
//  Description : 某个分组中电话个数
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC uint16 MMIPB_GetGroupContactPhoneCount
(
    uint16 group_id
)
{
    MMIPB_CONTACT_LIST_INFO_T list_info = {0};
    uint16 count = 0;
    if(MMIPB_ERROR_SUCCESS == MMIPB_ReadContactList(&list_info))
    {
        if(group_id < MMIPB_MAX_GROUP_NUM)
        {
            count = list_info.group_number_count[group_id];
        }
    }
    return count;
}


/*****************************************************************************/
//  Description : 某个分组中mail个数
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC uint16 MMIPB_GetGroupContactMailCount
(
    uint16 group_id
)
{
    MMIPB_CONTACT_LIST_INFO_T list_info = {0};
    uint16 count = 0;
    if(MMIPB_ERROR_SUCCESS == MMIPB_ReadContactList(&list_info))
    {
        if(group_id < MMIPB_MAX_GROUP_NUM)
        {
            count = list_info.group_mail_count[group_id];
        }
    }
    return count;
}

/*****************************************************************************/
//  Description : Delete group all contacts from list
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_RemoveOneGroupContactsFromList(uint32 group)
{
    return MMIPB_DeleteOneGroupContactList(group);
}

/*****************************************************************************/
//  Description : 查找第一个可见的联系人
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_FindFirstContact(
                                            uint32  group_mask,//[IN]
                                            uint16  storage_id, //要查找的联系人类型
                                            uint16  *cur_index_ptr,//[OUT]
                                            MMIPB_CONTACT_VALID_NUM_MAIL_INDEX_T *valid_index_array,//[OUT]
                                            MMIPB_CONTACT_BASE_INFO_T *find_data_ptr,//[OUT] 
                                            MMIPB_HANDLE_T *handle_ptr//[OUT]
                                            )
{
    MMIPB_HANDLE_NODE_T handle_node = {0};
    uint16               index = 0;
    MMIPB_ERROR_E        result = MMIPB_ERROR_ERROR;
    
    if(handle_ptr != PNULL && find_data_ptr != PNULL)
    {
        //create handle
        *handle_ptr = MMIPB_CreateHandle(MMIPB_OPC_FIND, 0);
        if(*handle_ptr != 0)
        {
            //valid handle
            if(MMIPB_GetHandleContent(*handle_ptr, &handle_node))
            {
                handle_node.group_mask = group_mask;
                //find from first one
                handle_node.access_point.list_index = 0;
                result = MMIPB_FindContact(handle_node.access_point.list_index, 
                                          group_mask,
                                          storage_id,
                                          &index,
                                          valid_index_array,
                                          find_data_ptr
                                          );
                if(result == MMIPB_ERROR_SUCCESS)
                {
                    if(cur_index_ptr != PNULL)
                    {
                        *cur_index_ptr = index;
                    }
                    //设置为下一个记录位置
                    handle_node.access_point.list_index = index+1;
                }
                MMIPB_SetHandleContent(*handle_ptr, &handle_node);
            }
        }
        if(result != MMIPB_ERROR_SUCCESS)
        {
            MMIPB_CloseHandle(*handle_ptr);
            *handle_ptr = 0;
        }
    }

    //SCI_TRACE_LOW:"[MMIPB]MMIPB_FindFirstContact group 0x%x handle_ptr 0x%x result %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_INTERFACE_2181_112_2_18_2_43_32_228,(uint8*)"ddd", group_mask, handle_ptr, result);

    return result;
}
#ifdef MMIPB_MOST_USED_SUPPORT 
/*****************************************************************************/
//  Description : 查找第一个可见的常用联系人
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_FindFirstMostUsedContact(                             
                                            uint16  *cur_index_ptr,//[OUT]
                                            MMIPB_MOSTUSED_CONTACT_NODE_T *find_data_ptr,//[OUT] 
                                            MMIPB_HANDLE_T *handle_ptr//[OUT]
                                            )
{
    MMIPB_HANDLE_NODE_T handle_node = {0};
    uint16               index = 0;
    MMIPB_ERROR_E        result = MMIPB_ERROR_ERROR;

    if(handle_ptr != PNULL && find_data_ptr != PNULL)
    {
        //create handle
        *handle_ptr = MMIPB_CreateHandle(MMIPB_OPC_FIND, 0);
        if(*handle_ptr != 0)
        {
            //valid handle
            if(MMIPB_GetHandleContent(*handle_ptr, &handle_node))
            {
                //find from first one
                handle_node.access_point.list_index = 0;
                result = MMIPB_FindMostUsedContact(
                    handle_node.access_point.list_index,
                    &index,//[OUT]                            
                    find_data_ptr//[OUT]
                    );
                if(result == MMIPB_ERROR_SUCCESS)
                {
                    if(cur_index_ptr != PNULL)
                    {
                        *cur_index_ptr = index;
                    }
                    //设置为下一个记录位置
                    handle_node.access_point.list_index = index+1;
                }
                MMIPB_SetHandleContent(*handle_ptr, &handle_node);
            }
        }
        if(result != MMIPB_ERROR_SUCCESS)
        {
            MMIPB_CloseHandle(*handle_ptr);
            *handle_ptr = 0;
        }
    }

    //SCI_TRACE_LOW:"[MMIPB]MMIPB_FindFirstMostUsedContact  handle_ptr 0x%x result %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_INTERFACE_2229_112_2_18_2_43_32_229,(uint8*)"dd", handle_ptr, result);
    return result;
}
#endif
/*****************************************************************************/
//  Description : 关闭一个查找
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_FindClose(
                                     MMIPB_HANDLE_T handle//[IN]
                                     )
{
    MMIPB_CloseHandle(handle);
    return MMIPB_ERROR_SUCCESS;
}

/*****************************************************************************/
//  Description : 查找下一个可见记录
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_FindNextContact( MMIPB_HANDLE_T handle, 
                                            uint32  group_mask,//[IN]
                                            uint16  storage_id, //要查找的联系人类型
                                            uint16  *cur_index_ptr,//[OUT]
                                            MMIPB_CONTACT_VALID_NUM_MAIL_INDEX_T *valid_index_array,//[OUT]
                                            MMIPB_CONTACT_BASE_INFO_T *find_data_ptr//[OUT] 
                                           )
{
    MMIPB_HANDLE_NODE_T  handle_node = {0};
    uint16               index = 0;
    MMIPB_ERROR_E        result = MMIPB_ERROR_ERROR;

    if(MMIPB_GetHandleContent(handle, &handle_node))
    {
        result = MMIPB_FindContact(
            handle_node.access_point.list_index,   
            group_mask, 
            storage_id,
            &index,//[OUT]
            valid_index_array,//[OUT]
            find_data_ptr//[OUT]
            );
        if(MMIPB_ERROR_SUCCESS == result)
        {
            if(cur_index_ptr != PNULL)
            {
                *cur_index_ptr = index;
            }
            handle_node.access_point.list_index = index+1;
            MMIPB_SetHandleContent(handle, &handle_node);
        }
    }

    return result;
}
#ifdef MMIPB_MOST_USED_SUPPORT 
/*****************************************************************************/
//  Description : 查找下一个可见常用联系人记录
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_FindNextMostContact(MMIPB_HANDLE_T handle, 
                                              uint16  *cur_index_ptr,//[OUT]
                                              MMIPB_MOSTUSED_CONTACT_NODE_T *find_data_ptr//[OUT] 
                                              )
{
    MMIPB_HANDLE_NODE_T handle_node = {0};
    uint16               index = 0;
    MMIPB_ERROR_E        result = MMIPB_ERROR_ERROR;

    if(MMIPB_GetHandleContent(handle, &handle_node))
    {
        result = MMIPB_FindMostUsedContact(
            handle_node.access_point.list_index,
            &index,//[OUT]                            
            find_data_ptr//[OUT]
            );
        if(result == MMIPB_ERROR_SUCCESS)
        {
            if(cur_index_ptr != PNULL)
            {
                *cur_index_ptr = index;
            }
            //设置为下一个记录位置
            handle_node.access_point.list_index = index+1;
            MMIPB_SetHandleContent(handle, &handle_node);
        }
    }    
    //SCI_TRACE_LOW:"[MMIPB]MMIPB_FindNextMostContact  handle 0x%x index %d result %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_INTERFACE_2314_112_2_18_2_43_32_230,(uint8*)"ddd", handle, index, result);
    
    return result;
}
#endif
/*****************************************************************************/
//  Description : 普通联系人的基本信息
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC MMIPB_CONTACT_BASE_INFO_T MMIPB_GetContactBaseInfo(uint16 index)
{
    MMIPB_CONTACT_BASE_INFO_T node = {0};

    MMIPB_ReadContactSimpleInfo(index, &node);

    return node;
}

/*****************************************************************************/
//  Description : 手机联系人的基本信息
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC MMIPB_CONTACT_BASE_INFO_T MMIPB_GetPhoneContactBaseInfo(uint16 index)
{
    MMIPB_CONTACT_BASE_INFO_T node = {0};

    MMIPB_ReadPhoneContactBaseInfo(index, &node);

    return node;
}
#ifdef MMIPB_MOST_USED_SUPPORT 
/*****************************************************************************/
//  Description : 常用联系人的基本信息
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC MMIPB_MOSTUSED_CONTACT_NODE_T MMIPB_GetMostUsedContactBaseInfo(uint16 index)
{
    MMIPB_MOSTUSED_CONTACT_NODE_T node = {0};
    
    MMIPB_ReadMostUsedContactListNode(index, &node);

    return node;
}
#endif
/*****************************************************************************/
//  Description : 获取当前的handle id
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_HANDLE_T MMIPB_GetCurrentHandle(void)
{
    return s_mmipb_current_handle;
}
#ifdef MMIPB_IM_MESSAGE_SUPPORT
/*****************************************************************************/
//  Description : 获得instacemsg_type对应的类型字符串
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPB_GetImTypeString(
    uint8  im_type,  //IN:
    MMI_STRING_T *type_str//OUT
)
{
    MMI_TEXT_ID_T default_text_id = TXT_COMM_QQ;
    
    switch(im_type)
    {
        case MMIPB_IM_QQ:
            default_text_id = TXT_COMM_QQ;
            break;
        case MMIPB_IM_MSN:
            default_text_id = TXT_PB_MSN;
            break;
        case MMIPB_IM_GOOGLE_TALK:
            default_text_id = TXT_PB_GOOGLE_TALK;
            break;
        case MMIPB_IM_YAHOO:
            default_text_id = TXT_PB_YAHOO;
            break;
        case MMIPB_IM_SKYPE:
            default_text_id = TXT_PB_SKYPE;
            break;
        case MMIPB_IM_AIM:
            default_text_id = TXT_PB_AIM;
            break;
        default:
            break;
    }
    MMI_GetLabelTextByLang(default_text_id, type_str);
}
/*****************************************************************************/
//  Description : 获得instacemsg_type对应的类型字符串
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC MMI_TEXT_ID_T MMIPB_GetImTypeTextId(uint8  im_type )
{
    MMI_TEXT_ID_T default_text_id = TXT_COMM_QQ;
    
    switch(im_type)
    {
        case MMIPB_IM_QQ:
            default_text_id = TXT_COMM_QQ;
            break;
        case MMIPB_IM_MSN:
            default_text_id = TXT_PB_MSN;
            break;
        case MMIPB_IM_GOOGLE_TALK:
            default_text_id = TXT_PB_GOOGLE_TALK;
            break;
        case MMIPB_IM_YAHOO:
            default_text_id = TXT_PB_YAHOO;
            break;
        case MMIPB_IM_SKYPE:
            default_text_id = TXT_PB_SKYPE;
            break;
        case MMIPB_IM_AIM:
            default_text_id = TXT_PB_AIM;
            break;
        default:
            break;
    }
    return default_text_id;
}
#endif
/*****************************************************************************/
//  Description : 获得nv num_type对应的类型字符串
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPB_GetNumberTypeString(uint16 storage_id,
    uint8  num_type,  //IN:
    MMI_STRING_T *type_str//OUT
)
{
	// MMI_STRING_T text_str = {0};
	MMI_TEXT_ID_T           default_text_id = 0;
#ifdef CMCC_VCARD_SUPPORT
	BOOLEAN                 is_usim_label = FALSE;
	MN_DUAL_SYS_E           dual_sys = MMIPB_GetDualSysByStorageId(storage_id);
	MMI_AAS_TYPE_E          aas_type = MMI_AAS_OFFICE_PHONE;
    static MMIPB_NAME_T     ass_name = {0};
    MMIPB_ACCESS_POINT_T    access_point = {0};
    MMIPB_ERROR_E           ret = MMIPB_ERROR_ERROR;
#endif
	if(type_str != PNULL)
	{
		type_str->wstr_len = 0;
        
#ifdef CMCC_VCARD_SUPPORT
        if(MMIPB_IsUsim(storage_id))
        {
            is_usim_label = TRUE;
			SCI_MEMSET(&ass_name, 0x00, sizeof(MMIPB_NAME_T));
        }
        else
#endif
		if(MMIPB_IsSimRecord(storage_id))
		{
			MMI_GetLabelTextByLang(TXT_PB_PHONE,type_str);
			return;
		}
        SCI_TRACE_LOW("yanyan.an GetString num_type:%d", num_type);
		switch(num_type)
		{
		case MMIPB_ADN_NUMBER:
			default_text_id = TXT_PB_MOBLIE;			
            break;
            
		case MMIPB_HOME_NUMBER:	
#if defined(CMCC_VCARD_SUPPORT)&& !defined(PB_SUPPORT_LOW_MEMORY)            
            access_point.aas_index_t.dual_sys = dual_sys;
            access_point.aas_index_t.aas_index = MMI_AAS_HOME_PHONE;
            ret = MMIPB_Read(MMIPB_ACCESS_USIM_AAS, access_point, sizeof(MMIPB_NAME_T), &ass_name);
			if((MMIPB_ERROR_SUCCESS == ret) && (MMI_DUAL_SYS_MAX > dual_sys) && (0 < ass_name.wstr_len) && (is_usim_label))
			{
				type_str->wstr_len = ass_name.wstr_len;
                type_str->wstr_ptr = ass_name.wstr;
                return;
			}
			else

#endif
			{
				default_text_id = TXT_PB_HOME;
			}
			break;
            
		case MMIPB_OFFICE_NUMBER:
#ifdef CMCC_VCARD_SUPPORT
            access_point.aas_index_t.dual_sys = dual_sys;
            access_point.aas_index_t.aas_index = MMI_AAS_OFFICE_PHONE;
            ret = MMIPB_Read(MMIPB_ACCESS_USIM_AAS, access_point, sizeof(MMIPB_NAME_T), &ass_name);
			if((MMIPB_ERROR_SUCCESS == ret) && (MMI_DUAL_SYS_MAX > dual_sys) && (0 < ass_name.wstr_len) && (is_usim_label))
			{
				type_str->wstr_len = ass_name.wstr_len;
                type_str->wstr_ptr = ass_name.wstr;
                return;
			}
			else
#endif
			{
				default_text_id = TXT_PB_WORK;
			}

			break;
         
        case MMIPB_FIXED_NUMBER:
            {
                default_text_id = TXT_PB_PHONE_NUMBER;
            }
    		break;
            
		case MMIPB_FAX_NUMBER:
#if defined(CMCC_VCARD_SUPPORT)&& !defined(PB_SUPPORT_LOW_MEMORY)
            access_point.aas_index_t.dual_sys = dual_sys;
            access_point.aas_index_t.aas_index = MMI_AAS_FAX_PHONE;
            ret = MMIPB_Read(MMIPB_ACCESS_USIM_AAS, access_point, sizeof(MMIPB_NAME_T), &ass_name);
			if((MMIPB_ERROR_SUCCESS == ret) && (MMI_DUAL_SYS_MAX > dual_sys) && (0 < ass_name.wstr_len) && (is_usim_label))
			{
				type_str->wstr_len = ass_name.wstr_len;
                type_str->wstr_ptr = ass_name.wstr;
                return;
			}
			else

#endif
			{
				default_text_id = TXT_PB_FAX_NUMBER;
			}
			break;
            
#ifdef MMIPB_MAIL_SUPPORT           
        case MMIPB_FEILD_FLAG_MASK_MAIL:
			default_text_id = TXT_PB_ITEM_DETAIL_EMAIL;
			break;
#endif
		default:
			default_text_id = TXT_UNKNOWN;
			break;
		}
		
        MMI_GetLabelTextByLang(default_text_id, type_str);

	}
}

/*****************************************************************************/
//  Description : 获得某一类型号码的最大长度
//  Global resource dependence :
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC uint8 MMIPB_GetNumberMaxLen(uint16 storage_id,                                  
                                   uint8 num_type,
                                   uint8 orignal_len//该号码原来的长度，是新增的号码填0
                                  )
{
    PHONEBOOK_STORAGE_E storage = PHONEBOOK_NV_STORAGE;
    uint8  max_num_len = 0;

    if(MMIPB_STORAGE_PHONE !=  MMIPB_GET_STORAGE(storage_id))
    {
        storage = MMIPB_GET_CONTACT_TYPE(storage_id);
    }

    max_num_len = MMIPB_GetContactMaxAlphaLen(storage, MMIPB_GetDualSysByStorageId(storage_id), MMIPB_CONTACT_NUMBER);
    
    if(MMIPB_IsSimRecord(storage_id) )
    {
        if(num_type != MMIPB_ADN_NUMBER)
        {
            //sim卡的非adn号码，都不支持扩展号码
            max_num_len = (MMIPB_BCD_NUMBER_NORMAL_LEN << 1);            
        }
        else if(max_num_len < orignal_len)
        {
            //adn号码，原来就占用一个扩展号码空间
            max_num_len = (MMIPB_BCD_NUMBER_MAX_LEN << 1);
        }
    }
    return max_num_len;
}

/*****************************************************************************/
//  Description : 获得联系人对应字段的最大alpha长度
//  Global resource dependence :
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC uint8 MMIPB_GetMaxAlphaLen(uint16 storage_id,                                  
                                  MMIPB_CONTACT_DOMAIN_E feild//联系人域
                                  )
{
    PHONEBOOK_STORAGE_E storage = PHONEBOOK_NV_STORAGE;

    if(MMIPB_STORAGE_PHONE !=  MMIPB_GET_STORAGE(storage_id))
    {
        storage = MMIPB_GET_CONTACT_TYPE(storage_id);
    }

    return MMIPB_GetContactMaxAlphaLen(storage, MMIPB_GetDualSysByStorageId(storage_id), feild);
}

/*****************************************************************************/
//  Description : 获得联系人对应字段的最大wstr长度
//  Global resource dependence :
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC uint8 MMIPB_GetMaxWstrLen(uint16 storage_id,//                                   
                                  MMIPB_CONTACT_DOMAIN_E feild//联系人域
                                 )
{
    PHONEBOOK_STORAGE_E storage = PHONEBOOK_NV_STORAGE;

    if(MMIPB_STORAGE_PHONE !=  MMIPB_GET_STORAGE(storage_id))
    {
        storage = MMIPB_GET_CONTACT_TYPE(storage_id);
    }
    return MMIPB_GetContactMaxWstrLen(storage, MMIPB_GetDualSysByStorageId(storage_id), feild);
}

/*****************************************************************************/
//  Description : 获得联系人对应字段的空间使用情况
//  Global resource dependence :
//  Author: 
//  Note: 每个字段相应如果有空间，设置为TRUE，否则为FALSE
//        通过该函数可以获取各个号码字段，mail字段，别名字段的使用情况
//  Return:如果contact_id为0，表示新增，返回一个新的id
/*****************************************************************************/
PUBLIC uint16 MMIPB_GetFieldUageInfo(
                                 uint16 storage_id,//                                   
                                 uint16 contact_id,//
                                 MMIPB_FIELD_USAGE_INOF_T *free_ptr
                                 )
{
    uint16 new_id = 0;
#ifdef MMIPB_MOST_USED_SUPPORT
    if(MMIPB_IsMostUsedContact(storage_id))
    {
        //常用联系人
        MMIPB_GetMostUsedContactFieldUageInfo( free_ptr);
        if(contact_id == 0)
        {
            new_id = MMIPB_GetFreeContactId(storage_id);
        }
    }
    else 
#endif
     if(MMIPB_STORAGE_PHONE == MMIPB_GET_STORAGE(storage_id))
    {
        //手机联系人
        MMIPB_GetPhoneContactFieldUageInfo(free_ptr);
        if(contact_id == 0)
        {
            new_id = MMIPB_GetFreeContactId(storage_id);
        }
    }
#ifdef SNS_SUPPORT     
    else if(MMIPB_IsSNSContact(storage_id))
    {
        //sns联系人
        MMIPB_GetSNSContactFieldUageInfo(free_ptr);
        new_id = contact_id;
    }
#endif    
    else
    {
        //sim卡联系人
        MMIPB_GetSIMContactFieldUageInfo(MMIPB_GetDualSysByStorageId(storage_id), 
                                        MMIPB_GET_CONTACT_TYPE(storage_id),
                                        free_ptr);
//         if(MMIPB_IsSIMContact(storage_id) && contact_id == 0)
//         {
//             new_id = PHONEBOOK_GetFirstFreeContactId(MMIPB_GetDualSysByStorageId(storage_id), PHONEBOOK_SIM_STORAGE);
//         }
    }

    return new_id;
}
/*****************************************************************************/
//  Description : get max number num of contact bcdnumber
//  Global resource dependence : none
//  Author: xiaohua.liu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPB_GetMaxNumbernum(uint16 storage_id,uint8 *number_num)
{
    MMIPB_FIELD_USAGE_INOF_T usage_flag = {0};
    uint32 i = 0;

    if(number_num != PNULL)
    {
        *number_num = 0;
        MMIPB_GetFieldUageInfo(storage_id, 0, &usage_flag);
        
        for(i = 0; i < MMIPB_MAX_NV_PHONE_NUM; i++)
        {
            if(usage_flag.number_free_flag[i])
            {
                *number_num += 1;
            }        
        }
    }

    return;
}

/*****************************************************************************/
//  Description : get all phone's group mask
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMIPB_GetAllPhoneGroup(void)
{
    uint32 group = 0;
    uint32 i = 0;

    for(i = 0; i < MMIPB_MAX_GROUP_NUM; i++)
    {
        if(MMIPB_IsInNvGroup(i))
        {
            group |= (1<< i);
        }
    }
    return group;
}


/*****************************************************************************/
//  Description : get all sim's group mask
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMIPB_GetAllSIMGroup(void)
{
    uint32 group = 0;
    uint32 i = 0;

    for(i = 0; i < MMIPB_MAX_GROUP_NUM; i++)
    {
        if(MMIPB_IsInSIMGroup(i))
        {
            group |= (1<< i);
        }
    }
    return group;
}

#ifdef SNS_SUPPORT
/*****************************************************************************/
//  Description : get all sns's group mask
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMIPB_GetAllSNSGroup(void)
{
    uint32 group = 0;
    uint32 i = 0;

    for(i = MMIPB_GROUP_SNS_SINA; i < MMIPB_GROUP_SNS_MAX; i++)
    {
        group |= (1<< i);
    }
    return group;
}
#endif

#ifdef BT_PBAP_SUPPORT 
/*****************************************************************************/
//  Description : 所有蓝牙联系人个数
//  Global resource dependence : 
//  Author:Yanyan.an 
//  Note: 
/*****************************************************************************/
PUBLIC uint16 MMIPB_GetAllBtContactCount(void)
{
    uint16 total_count = 0;
    uint16 bt_count = 0;
    uint16 i = 0;
    MMIPB_CONTACT_BASE_INFO_T contact_node_info = {0};
    
	//全部联系人总数total_count
    total_count = MMIPB_GetAllNormalContactCount();
	
	for (i = 0; i < total_count; i++)
	{
		//从索引表中读取每条记录对应的storage_id, contact_id
		contact_node_info = MMIPB_GetContactBaseInfo(i);
		
		//判断是否BT Contact
		if(MMIPB_IsBTContact(contact_node_info.contact_id, contact_node_info.storage_id))
		{
			bt_count++;
		}
	}
	
    SCI_TRACE_LOW("[MMIPB]MMIPB_GetAllBtContactCount bt_count = %d", bt_count);
	
    return bt_count;
}
/*****************************************************************************/
//  Description : 查找第一个可见的蓝牙联系人
//  Global resource dependence : 
//  Author:Yanyan.an 
//  Note: 
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_FindFirstBtContact(
											  uint16  *cur_index_ptr,//[OUT]
											  MMIPB_CONTACT_BASE_INFO_T *find_data_ptr,//[OUT] 
											  MMIPB_HANDLE_T *handle_ptr//[OUT]
											  )
{
    MMIPB_HANDLE_NODE_T handle_node = {0};
    uint16               index = 0;
    MMIPB_ERROR_E        result = MMIPB_ERROR_ERROR;
    
    if(handle_ptr != PNULL && find_data_ptr != PNULL)
    {
        //create handle
        *handle_ptr = MMIPB_CreateHandle(MMIPB_OPC_FIND, 0);
        if(*handle_ptr != 0)
        {
            //valid handle
            if(MMIPB_GetHandleContent(*handle_ptr, &handle_node))
            {
                //find from first one
                handle_node.access_point.list_index = 0;
                result = MMIPB_FindBtContact(
					handle_node.access_point.list_index, 
					&index, //[OUT]
					find_data_ptr//[OUT]
					);
                
                if(result == MMIPB_ERROR_SUCCESS)
                {
                    if(cur_index_ptr != PNULL)
                    {
                        *cur_index_ptr = index;
                    }
                    //设置为下一个记录位置
                    handle_node.access_point.list_index = index+1;
                }
                
                MMIPB_SetHandleContent(*handle_ptr, &handle_node);
                
            }
            
        }
        
        if(result != MMIPB_ERROR_SUCCESS)
        {
            MMIPB_CloseHandle(*handle_ptr);
            *handle_ptr = 0;
        }
    }
	
    // SCI_TRACE_LOW("MMIPB_FindFirstBtContact *cur_index_ptr %d, handle_ptr 0x%x, result %d", *cur_index_ptr, handle_ptr, result);
	
    return result;
}
/*****************************************************************************/
//  Description : 查找下一个可见的蓝牙联系人
//  Global resource dependence : 
//  Author:Yanyan.an 
//  Note: 
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_FindNextBtContact(MMIPB_HANDLE_T handle, 
											 uint16  *cur_index_ptr,//[OUT]
											 MMIPB_CONTACT_BASE_INFO_T *find_data_ptr//[OUT] 
											 )
{
    MMIPB_HANDLE_NODE_T  handle_node = {0};
    uint16               index = 0;
    MMIPB_ERROR_E        result = MMIPB_ERROR_ERROR;
	
    if(MMIPB_GetHandleContent(handle, &handle_node))
    {
        result = MMIPB_FindBtContact(
            handle_node.access_point.list_index, 
            &index,//[OUT] 
            find_data_ptr//[OUT]
            );
		
        if(MMIPB_ERROR_SUCCESS == result)
        {
            if(cur_index_ptr != PNULL)
            {
                *cur_index_ptr = index;
            }
            handle_node.access_point.list_index = index+1;
        }
		MMIPB_SetHandleContent(handle, &handle_node);
    }
	
    // SCI_TRACE_LOW("MMIPB_FindNextBtContact *cur_index_ptr %d, result %d", *cur_index_ptr, result);
	
    return result;
}
#endif