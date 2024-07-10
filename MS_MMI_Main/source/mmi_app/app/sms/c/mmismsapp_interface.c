/****************************************************************************
** File Name:      mmismsapp_interface.c                                       *
** Author:                                                                 *
** Date:           04/11/2012                                              *
** Copyright:      2012 Spreadtrum, Incorporated. All Rights Reserved.     *
** Description:    This file is used to describe the SMS                   *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 12/2012        rong.gu        Create
****************************************************************************/
/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "mmi_app_sms_trc.h"
#include "mmisrv_handlelist.h"
#include "mmismsapp_interface.h"
#include "mmisms_id.h"
#include "mmisms_order.h"
#include "mmisms_appcontrol.h"
#include "mmisms_save.h"
#include "mmisms_send.h"
#ifdef MMI_TIMERMSG_SUPPORT
#include "mmisms_timermsg.h"
#endif
#define SMS_DATASRV_MAX_HANDLE_SUPPORT   10

#define SMS_DATASRV_AUTO_NEXT

//typedef for srv_main
typedef enum
{
    SRVSTATUS_NULL,
    SRVSTATUS_INITIALIZING,
    SRVSTATUS_READY,
    SRVSTATUS_OPERATING,
    SRVSTATUS_MAX
}DATASRV_STATUS_E;



typedef struct
{
    DATASRV_STATUS_E    srv_status;
    MMI_SRVHANDLE_T     srvhandle_lst;
    SCI_MUTEX_PTR       srv_mutex;
    SMS_HANDLE_T        cur_handle;
} DATASRV_SMS_INSTANCE_T;

//typedef for srv_main

MMI_APPLICATION_T           g_smsdatasrv_app;

LOCAL DATASRV_SMS_INSTANCE_T   s_smssrv_instance = {0};
extern MMISMS_GLOBAL_T g_mmisms_global;            // SMS的全局变量

extern BOOLEAN g_mmisms_is_need_mem_full;

/******************************************************************************
//  Description : sms memory is full
//  Global resource dependence :
//  Author:  bingjie.chen
//  Note:
 ******************************************************************************/
LOCAL BOOLEAN MMISMS_IsMemoryFull(MN_DUAL_SYS_E dual_sys)
{
    uint16 sim_max_num;
    uint16 sim_used_num;
    uint16 nv_max_num;
    uint16 nv_used_num;

    MMISMS_GetSMSMemory(dual_sys, FALSE, &sim_max_num, &sim_used_num);
    MMISMS_GetSMSMemory(dual_sys, TRUE, &nv_max_num, &nv_used_num); 

    if(sim_max_num == sim_used_num && nv_max_num == nv_used_num)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/******************************************************************************
//  Description : create mutex
//  Global resource dependence :
//  Author:  sam.hua
//  Note:
 ******************************************************************************/
LOCAL void CreateDataSrvMutex(SCI_MUTEX_PTR *srv_mutex)
{
    if (srv_mutex == PNULL )
    {
        return;
    }
    if (*srv_mutex == 0)
    {
        *srv_mutex = SCI_CreateMutex("SMS_SRV_HANDLELIST_MUTEX", SCI_INHERIT);
    }
}

/******************************************************************************
//  Description : destroy mutex
//  Global resource dependence :
//  Author:  sam.hua
//  Note:
 ******************************************************************************/
LOCAL void DestroyDataSrvMutex(SCI_MUTEX_PTR *srv_mutex)
{
    if (srv_mutex == PNULL)
    {
        return;
    }

    if (*srv_mutex != 0)
    {
        SCI_DeleteMutex(*srv_mutex);
        *srv_mutex = PNULL;
    }
    return;
}

/******************************************************************************
//  Description : GetMutex
//  Global resource dependence :
//  Author:  sam.hua
//  Note:
 ******************************************************************************/
LOCAL void GetDataSrvMutex(SCI_MUTEX_PTR *srv_mutex)
{
    if (srv_mutex == PNULL)
    {
        return;
    }
    SCI_GetMutex(*srv_mutex, SCI_WAIT_FOREVER);
}

/******************************************************************************
//  Description : Put Mutex
//  Global resource dependence :
//  Author:  sam.hua
//  Note:
 ******************************************************************************/
LOCAL void PutDataSrvMutex(SCI_MUTEX_PTR *srv_mutex)
{
    if (srv_mutex == PNULL)
    {
        return;
    }
    SCI_PutMutex(*srv_mutex);
}


//Srv main functions
//LOCAL MMI_RESULT_E SmsDataSrv_HandlePsMsg( PWND app_ptr, uint16 msg_id, DPARAM param);
/******************************************************************************
//  Description : sms handle mgr init
//  Global resource dependence :
//  Author:  sam.hua
//  Note:
 ******************************************************************************/
PUBLIC void SmsDataSrv_init(void)
{
//    g_smsdatasrv_app.ProcessMsg = SmsDataSrv_HandlePsMsg;

    SCI_MEMSET(&s_smssrv_instance, 0, sizeof(DATASRV_SMS_INSTANCE_T));

    //Create datasrv handle instance
    s_smssrv_instance.srvhandle_lst = DataSrvHandle_Create(sizeof(SMSDATA_SRV_HANDLE_T),
                                                           PNULL,
                                                           PNULL);

    CreateDataSrvMutex(&s_smssrv_instance.srv_mutex);
}

/******************************************************************************
//  Description : sms handle mgr destroy
//  Global resource dependence :
//  Author:  sam.hua
//  Note:
 ******************************************************************************/
LOCAL void SmsDataSrv_Destroy(void)
{
    DataSrvHandle_Destroy(&s_smssrv_instance.srvhandle_lst);

    SCI_MEMSET(&s_smssrv_instance, 0, sizeof(DATASRV_SMS_INSTANCE_T));

}

PUBLIC DATASRV_SMS_INSTANCE_T SmsDataSrv_GetInstance(void)
{
    return s_smssrv_instance;
}


PUBLIC BOOLEAN SmsDataSrvAPI_IsReady(void)
{
    if (s_smssrv_instance.srv_status == SRVSTATUS_READY)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }

}


PUBLIC BOOLEAN SmsDataSrvAPI_IsOperating(void)
{
    if (s_smssrv_instance.srv_status == SRVSTATUS_OPERATING)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

LOCAL void SmsDataSrv_EnterReady()
{
    s_smssrv_instance.srv_status = SRVSTATUS_READY;
}

LOCAL BOOLEAN SmsDataSrv_EnterOperating(void)
{
    if (SmsDataSrvAPI_IsReady())
    {
        s_smssrv_instance.srv_status = SRVSTATUS_OPERATING;
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}



LOCAL BOOLEAN SmsDataSrv_ExitOperating(void)
{
    //Free cur handle;
    DataSrvHandle_FreeHandle(s_smssrv_instance.srvhandle_lst, s_smssrv_instance.cur_handle);

    if (SmsDataSrvAPI_IsOperating())
    {
        PutDataSrvMutex(&s_smssrv_instance.srv_mutex);
        s_smssrv_instance.srv_status = SRVSTATUS_READY;
        return TRUE;
    }
    else
    {
        return FALSE;
    }
#ifdef SMS_DATASRV_AUTO_NEXT
    //Set next valid Handle as current
    //SmsHandle_GetNextHandle()
    //Do opreation

    //return;
#endif

}

typedef struct
{
    MN_SMS_RECORD_ID_T record_id;
    MN_SMS_STORAGE_E   storage;
    SRV_OPERATION_ENUM_E oper_type;
}CMP_DATA_TAG;


/******************************************************************************
//  Description : sms handle cmp function
//  Global resource dependence :
//  Author:  sam.hua
//  Note:
 ******************************************************************************/
LOCAL int CmpHandle(void* srv_handle,void* cmp_data)
{
    
    SMS_HANDLE_T cmp_src = (SMS_HANDLE_T)srv_handle;
    CMP_DATA_TAG* cmp_dst= (CMP_DATA_TAG*)cmp_data;
    if (cmp_src== PNULL || cmp_dst == PNULL)
    {
        return -1;      //Error
    }
	SCI_TRACE_LOW("src record id =%d,storage = %x,oper_type=%x",cmp_src->record_id,cmp_src->storage,cmp_src->oper_type);
    
    if (cmp_src->record_id == cmp_dst->record_id &&
        cmp_src->storage == cmp_dst->storage &&
        cmp_src->oper_type == cmp_dst->oper_type)
    {
        return 0;       //Equal
    }

    return 1;           //Different
}

/******************************************************************************
//  Description : sms handle sysn callback
//  Global resource dependence :
//  Author:  sam.hua
//  Note:
 ******************************************************************************/
PUBLIC BOOLEAN SMSSRV_Sync_Callback(
    SRV_OPERATION_ENUM_E operation,
    BOOLEAN result,
    uint32 cause,
    void *opreation_cb,
    void *user_data,
    SmsSrvCallbackFunc callback_func)
{

    SMSSRV_CALLBACK_STRUCT_T oper_data = {0};

    if (callback_func == PNULL)
    {
        SCI_TRACE_LOW("mmisms callback func pnull");
        return TRUE;    //Close it.
    }
    SCI_TRACE_LOW("mmisms callback func callback_func = %x",callback_func);//打印回调ptr
    oper_data.operation = operation;
    oper_data.opreation_cb = opreation_cb;
    oper_data.result = result;
    oper_data.user_data = user_data;
    oper_data.cause = cause;

    {//temp debug	
    SCI_TRACE_MID("[MMI] MMI_CheckAllocatedMemInfo file:%s, line:%d", _D_FILE_NAME, _D_FILE_LINE);
    MMI_CheckAllocatedMemInfo();
    }
    
    return callback_func(&oper_data);

}

#if 0
LOCAL MMI_RESULT_E SmsDataSrv_HandlePsMsg( PWND app_ptr, uint16 msg_id, DPARAM param)
{
    //Get cnf data

    //Get cur handle

    //build result data

    //Callback


    SmsDataSrv_ExitOperating();
}
#endif
//PUBLIC BOOLEAN MMISMS_HandleIncorrectMessage(uint16 msg_id)
//{

//}


/******************************************************************************
//  Description : sms api: Delete
//  Global resource dependence :
//  Author:  sam.hua
//  Note:
 ******************************************************************************/
PUBLIC SMS_HANDLE_T MMISMSAPP_DeleteSms(
    MMISMS_ORDER_ID_T order_id,
    SmsSrvCallbackFunc delte_callback,
    void *action_data,
    void *user_data)
{
    SMS_HANDLE_T  new_handle = PNULL;

    SMSSRV_CALLBACK_STRUCT_T *oper_data = PNULL;
    MN_SMS_STORAGE_E storage = MN_SMS_NO_STORED;
    MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_MAX;
    ERR_MNSMS_CODE_E result = ERR_MNSMS_NONE;
    MN_SMS_RECORD_ID_T record_id = 0;
    if (s_smssrv_instance.srvhandle_lst == PNULL)
    {
        SmsDataSrv_init();
    }
    SCI_TRACE_LOW("SCI_TRACE_LOW MMISMSAPP_DeleteSms");
    GetDataSrvMutex(&s_smssrv_instance.srv_mutex);

    //Get new sms handle
    new_handle = (SMS_HANDLE_T)DataSrvHandle_NewHandle(s_smssrv_instance.srvhandle_lst);

    if (new_handle == PNULL)
    {
        SCI_TRACE_LOW("mmisms new_handle ptr PNULL");
        return PNULL;
    }
    SCI_TRACE_LOW("mmisms new srv handle delete sms");
    s_smssrv_instance.cur_handle = new_handle;

    //construct operator data and save it
    oper_data = &new_handle->oper_data.oper_cb_data;
    oper_data->user_data = user_data;
    oper_data->opreation_cb = action_data;
    new_handle->oper_data.callback_func = delte_callback;
    //send to MN
     
    new_handle->record_id = order_id->record_id;
    new_handle->storage = (MN_SMS_STORAGE_E)order_id->flag.storage;
    new_handle->oper_type = SRV_OPERATION_DELETE;
    if ( SmsDataSrvAPI_IsOperating())
    {
        return PNULL;
    }
    
    storage = (MN_SMS_STORAGE_E)order_id->flag.storage;
    dual_sys = (MN_DUAL_SYS_E)order_id->flag.dual_sys;
    record_id = (MN_SMS_RECORD_ID_T)order_id->record_id;
    // to delete the SMS
    SCI_TRACE_LOW("MMISMS: DeleteReqToMN record_id = %d, storage = %d",record_id,storage);
    PutDataSrvMutex(&s_smssrv_instance.srv_mutex);
    MMISMS_SetGlobalOperationOrderId(order_id);
    MMISMS_SetOperStatus(MMISMS_DELETE_MSG);
    if(storage == MN_SMS_NO_STORED)
    {
       MMK_CloseWin(MMISMS_DELETE_ANIM_WIN_ID);
       return PNULL;
    }
    result = MNSMS_UpdateSmsStatusEx(dual_sys, storage, record_id, MN_SMS_FREE_SPACE);
    
    if(result == ERR_MNSMS_NONE)
    {
        return new_handle;
    }
    return PNULL;
}

/******************************************************************************
//  Description : sms api: Read Chat sms
//  Global resource dependence :
//  Author:  sam.hua
//  Note:
 ******************************************************************************/
PUBLIC SMS_HANDLE_T MMISMSAPP_ReadChatSms(MMISMS_ORDER_ID_T oper_order_id,
    SmsSrvCallbackFunc read_callback,
    void *action_data,//must not be free in window
    void *user_data)
{
    SMS_READ_OPERATE_DATA_T   op_data = {0};


    SCI_TRACE_LOW("SCI_TRACE_LOW MMISMSAPP_ReadChatSms");
    MMISMS_SetGlobalOperationOrderId(oper_order_id);
#ifdef MMI_SMS_CHAT_SUPPORT  
    MMISMS_SetOperStatus(MMISMS_READ_CHAT_NUM_SMS);
#endif
    MMISMS_ReadyReadSms();
    //开始读取
    op_data.dual_sys = oper_order_id->flag.dual_sys;
    op_data.record_id =oper_order_id->record_id;
    op_data.oper_order_id = oper_order_id;
    op_data.storage = oper_order_id->flag.storage;
    return MMISMSAPP_ReadSms(op_data,
            read_callback,
            action_data,
            user_data
        );
}

#ifdef MMI_SMS_EXPORT_SUPPORT
/******************************************************************************
//  Description : sms api: Export SMS
//  Global resource dependence :
//  Author:  sam.hua
//  Note:
 ******************************************************************************/
PUBLIC SMS_HANDLE_T MMISMSAPP_ReadExportSms(MMISMS_ORDER_ID_T oper_order_id,
    SmsSrvCallbackFunc read_callback,
    void *action_data,
    void *user_data)
{
    SMS_READ_OPERATE_DATA_T   op_data = {0};

    SCI_TRACE_LOW("SCI_TRACE_LOW MMISMSAPP_ReadExportSms");

    MMISMS_SetGlobalOperationOrderId(oper_order_id);
    MMISMS_SetOperStatus(MMISMS_EXPORT_SMS);
    MMISMS_ReadyReadSms();
    //开始读取
    op_data.dual_sys = oper_order_id->flag.dual_sys;
    op_data.record_id =oper_order_id->record_id;
    op_data.oper_order_id = oper_order_id;
    op_data.storage = oper_order_id->flag.storage;
    return MMISMSAPP_ReadSms(op_data,
            read_callback,
            action_data,
            user_data
        );
}
#endif


#ifdef MMI_SMS_REPLY_PATH_SUPPORT
/******************************************************************************
//  Description : sms api: Read sms to get sms center
//  Global resource dependence :
//  Author:  sam.hua
//  Note:
 ******************************************************************************/
PUBLIC SMS_HANDLE_T MMISMSAPP_ReadSmsCenter(MMISMS_ORDER_ID_T oper_order_id,
    SmsSrvCallbackFunc read_callback,
    void *action_data,
    void *user_data)
{
    SMS_READ_OPERATE_DATA_T   op_data = {0};

    SCI_TRACE_LOW("SCI_TRACE_LOW MMISMSAPP_ReadSmsCenter storage = %d,record_id =%d ",
            (uint8)oper_order_id->flag.storage,(uint8)oper_order_id->record_id);

    MMISMS_SetGlobalOperationOrderId(oper_order_id);
    MMISMS_SetOperStatus(MMISMS_READ_SMS_NOT_SHOW);
    MMISMS_ReadyReadSms();
    //开始读取
    op_data.dual_sys = oper_order_id->flag.dual_sys;
    op_data.record_id =oper_order_id->record_id;
    op_data.oper_order_id = oper_order_id;
    op_data.storage = oper_order_id->flag.storage;
    return MMISMSAPP_ReadSms(op_data,
            read_callback,
            action_data,
            user_data
        );
}
#endif
#ifdef MMI_TIMERMSG_SUPPORT
/******************************************************************************
//  Description : sms api: Read timer sms
//  Global resource dependence :
//  Author:  sam.hua
//  Note:
 ******************************************************************************/
PUBLIC SMS_HANDLE_T MMISMSAPP_ReadTimerSms(MMISMS_ORDER_ID_T oper_order_id,
    SmsSrvCallbackFunc read_callback,
    void *action_data,
    void *user_data)
{

    SMS_READ_OPERATE_DATA_T   op_data = {0};

    SCI_TRACE_LOW("SCI_TRACE_LOW MMISMSAPP_ReadTimerSms storage = %d,record_id =%d ",
            (uint8)oper_order_id->flag.storage,(uint8)oper_order_id->record_id);

    MMISMS_SetIsTimerMsg(TRUE);

    MMISMS_SetOperStatus(MMISMS_READ_FOR_TIMERMSG);
    MMISMS_SetGlobalOperationOrderId(oper_order_id);
    MMISMS_ReadyReadSms();
    //开始读取
    op_data.dual_sys = oper_order_id->flag.dual_sys;
    op_data.record_id =oper_order_id->record_id;
    op_data.oper_order_id = oper_order_id;
    op_data.storage = oper_order_id->flag.storage;
    return MMISMSAPP_ReadSms(op_data,
            read_callback,
            action_data,
            user_data
        );

}
#endif
/******************************************************************************
//  Description : sms api: Read one sms by id
//  Global resource dependence :
//  Author:  sam.hua
//  Note:
 ******************************************************************************/
PUBLIC SMS_HANDLE_T MMISMSAPP_ReadOneSms(MMISMS_ORDER_ID_T oper_order_id,
    SmsSrvCallbackFunc read_callback,
    void *action_data,   //must not be free in window
    void *user_data)
{
    SMS_READ_OPERATE_DATA_T   op_data = {0};

    SCI_TRACE_LOW("SCI_TRACE_LOW MMISMSAPP_ReadOneSms storage = %d,record_id =%d ",
            (uint8)oper_order_id->flag.storage,(uint8)oper_order_id->record_id);

    if (MMISMS_IsMemoryFull(oper_order_id->flag.dual_sys) && (MN_SMS_STORAGE_SIM == oper_order_id->flag.storage) && (MMISMS_MT_TO_BE_READ == oper_order_id->flag.mo_mt_type))
    {
        g_mmisms_is_need_mem_full = TRUE;
    }
    
    
    MMISMS_SetGlobalOperationOrderId(oper_order_id);
#ifdef MMI_SMS_CHAT_SUPPORT
    MMISMS_SetOperStatus(MMISMS_READ_CHAT_NUM_SMS);
#else
    MMISMS_SetOperStatus(MMISMS_READ_SMS);
#endif
    MMISMS_ReadyReadSms();
    //开始读取
    op_data.dual_sys = oper_order_id->flag.dual_sys;
    op_data.record_id =oper_order_id->record_id;
    op_data.oper_order_id = oper_order_id;
    op_data.storage = oper_order_id->flag.storage;

    return MMISMSAPP_ReadSms(op_data,
            read_callback,
            action_data,
            user_data
        );
}

/******************************************************************************
//  Description : sms api: Read sms common api
//  Global resource dependence :
//  Author:  sam.hua
//  Note:
 ******************************************************************************/
PUBLIC SMS_HANDLE_T MMISMSAPP_ReadSms(
    SMS_READ_OPERATE_DATA_T op_data,
    SmsSrvCallbackFunc read_callback,
    void *action_data,//must not be free in window
    void *user_data)
{
    SMS_HANDLE_T  new_handle = PNULL;
    ERR_MNSMS_CODE_E    err_code=ERR_MNSMS_NONE;
    SMSSRV_CALLBACK_STRUCT_T *oper_data = PNULL;

    if (s_smssrv_instance.srvhandle_lst == PNULL)
    {
        SmsDataSrv_init();
    }

    GetDataSrvMutex(&s_smssrv_instance.srv_mutex);

    //Get new sms handle
    new_handle = (SMS_HANDLE_T)DataSrvHandle_NewHandle(s_smssrv_instance.srvhandle_lst);

    if (new_handle == PNULL)
    {
        return PNULL;
    }
    SCI_TRACE_LOW("mmisms new srv handle read sms");
    s_smssrv_instance.cur_handle = new_handle;

    //construct operator data and save it
    oper_data = &new_handle->oper_data.oper_cb_data;
    oper_data->user_data = user_data;
    oper_data->opreation_cb = action_data;
//    oper_data->oper_order_id = op_data.oper_order_id;
    new_handle->oper_data.callback_func = read_callback;
    //send to MN
    new_handle->record_id = op_data.record_id;
    new_handle->storage = (MN_SMS_STORAGE_E)op_data.storage;
    new_handle->oper_type = SRV_OPERATION_READ;

    PutDataSrvMutex(&s_smssrv_instance.srv_mutex);
    SCI_TRACE_LOW("mmisms MMISMSAPP_ReadSms dual_sys = %d,storage = %d,record_id = %d",op_data.dual_sys,op_data.storage,op_data.record_id);
    if (op_data.storage == MN_SMS_NO_STORED)
    {
        if (MMK_IsOpenWin(MMISMS_READSMS_WAIT_WIN_ID))
        {
            MMK_CloseWin(MMISMS_READSMS_WAIT_WIN_ID);
        }
        
        return PNULL;
    }
    err_code = MNSMS_ReadSmsEx(
        (MN_DUAL_SYS_E)op_data.dual_sys,
        (MN_SMS_STORAGE_E)op_data.storage,
        op_data.record_id
        );

    if (err_code>ERR_MNSMS_NONE)
    {
        //Error handler
        SCI_TRACE_LOW("mmisms cmd_handle pnull err_code = %d", err_code);
        MMISMSAPP_HandleReadCnf(op_data.record_id,op_data.storage,FALSE);
    }

    return new_handle;
}

/******************************************************************************
//  Description : sms api: move sms
//  Global resource dependence :
//  Author:  sam.hua
//  Note:
 ******************************************************************************/
PUBLIC SMS_HANDLE_T MMISMSAPP_MoveSms(
    SMS_MOVE_OPERATE_DATA_T op_data,
    SmsSrvCallbackFunc move_callback,
    void *action_data,
    void *user_data)
{
    SMS_HANDLE_T  new_handle = PNULL;
    MMISMS_ORDER_ID_T   move_order_id = PNULL;
    SMSSRV_CALLBACK_STRUCT_T *oper_data = PNULL;
    MMISMS_OPER_STATUS_E oper_status = MMISMS_NO_OPER;
    MMISMS_ORDER_ID_T cur_order_id = PNULL;
    uint16 i = 0;
    ERR_MNSMS_CODE_E    err_code = ERR_MNSMS_NONE;
    
    move_order_id = op_data.move_order_id;
    oper_status = op_data.oper_status;

    SCI_TRACE_LOW("SCI_TRACE_LOW MMISMSAPP_MoveSms");
    
    if (s_smssrv_instance.srvhandle_lst == PNULL)
    {
        SmsDataSrv_init();
    }

    GetDataSrvMutex(&s_smssrv_instance.srv_mutex);

    //Get new sms handle
    new_handle = (SMS_HANDLE_T)DataSrvHandle_NewHandle(s_smssrv_instance.srvhandle_lst);

    if (new_handle == PNULL)
    {
        return PNULL;
    }
    SCI_TRACE_LOW("mmisms new srv handle move sms");
    s_smssrv_instance.cur_handle = new_handle;

    //construct operator data and save it
    oper_data = &new_handle->oper_data.oper_cb_data;
    oper_data->user_data = user_data;
    oper_data->opreation_cb = action_data;
    new_handle->oper_data.callback_func = move_callback;
    //send to MN
    new_handle->record_id = 0;
    new_handle->storage = (MN_SMS_STORAGE_E)MN_SMS_NO_STORED;
    new_handle->oper_type = SRV_OPERATION_MOVE;

    MMISMS_InitCurOperationOrderId();
    move_order_id->flag.is_marked = FALSE;// 找到后将该标记清除，避免反复找到同一条信息

    if (move_order_id->flag.is_search)
    {
        move_order_id->flag.is_search = FALSE;
    }
 
    if (move_order_id->flag.is_concatenate_sms) //is long sms
    {
        cur_order_id = move_order_id;
        while (cur_order_id != PNULL)
        {
            MMISMS_SetCurOrderId(i,cur_order_id); //设置当前order_id到全局变量数组
            cur_order_id  = cur_order_id->next_long_sms_ptr;
            i++;
        }
    }
    else //is normal sms
    {
        MMISMS_SetCurOrderId(0,move_order_id); //设置当前order_id到全局变量数组 
    }

    MMISMS_SetOperStatus(oper_status);
    if((MN_SMS_STORAGE_SIM == move_order_id->flag.storage) ||
       (MMISMS_MOVE_SMS_TO_ME == oper_status) ||
       (MMISMS_MOVE_SMS_TO_SIM == oper_status))
    {
        MMISMS_ReadyReadSms();
        move_order_id->flag.is_need_update = FALSE;
        err_code = MNSMS_ReadSmsEx(
            (MN_DUAL_SYS_E)move_order_id->flag.dual_sys, 
            (MN_SMS_STORAGE_E)move_order_id->flag.storage, 
            move_order_id->record_id 
            );
        if (err_code>ERR_MNSMS_NONE)
        {
            //Error handler
            MMISMSAPP_HandleMoveCnf(MMISMS_SMS_READ_FAIL);
        }
    }
    else
    {

        MMISMS_GetOrderIdByStorage(
            &cur_order_id,
            MMISMS_TYPE_SMS,
            (MN_DUAL_SYS_E)move_order_id->flag.dual_sys,
            (MN_SMS_STORAGE_E)move_order_id->flag.storage,
            move_order_id->record_id
            );

        MMISMS_SetCurMsgFolderType(cur_order_id,oper_status);

#ifdef MMI_TIMERMSG_SUPPORT
        MMISMS_TIMERMSG_DeleteTimerMsg(cur_order_id);
#endif
        oper_data->cause = MMISMS_NO_ERR;
        if(move_callback(oper_data))
        {
            DataSrvHandle_FreeHandle(s_smssrv_instance.srvhandle_lst,new_handle);
            SCI_TRACE_LOW("mmisms free srv handle");
        }
    }

    PutDataSrvMutex(&s_smssrv_instance.srv_mutex);

    return new_handle;
}

/******************************************************************************
//  Description : sms api: copy sms
//  Global resource dependence :
//  Author:  sam.hua
//  Note:
 ******************************************************************************/
PUBLIC SMS_HANDLE_T MMISMSAPP_CopySms(
    SMS_COPY_OPERATE_DATA_T op_data,
    SmsSrvCallbackFunc move_callback,
    void *action_data,
    void *user_data)
{
    SMS_HANDLE_T  new_handle = PNULL;
    MMISMS_ORDER_ID_T   copy_order_id = PNULL;
    SMSSRV_CALLBACK_STRUCT_T *oper_data = PNULL;
//    MMISMS_OPER_STATUS_E oper_status = MMISMS_NO_OPER;
    MMISMS_ORDER_ID_T cur_order_id = PNULL;
    ERR_MNSMS_CODE_E    err_code = ERR_MNSMS_NONE;
    uint16 i = 0;
    copy_order_id = op_data.copy_order_id;

    SCI_TRACE_LOW("SCI_TRACE_LOW MMISMSAPP_CopySms");
    
    if (s_smssrv_instance.srvhandle_lst == PNULL)
    {
        SmsDataSrv_init();
    }

    GetDataSrvMutex(&s_smssrv_instance.srv_mutex);

    //Get new sms handle
    new_handle = (SMS_HANDLE_T)DataSrvHandle_NewHandle(s_smssrv_instance.srvhandle_lst);

    if (new_handle == PNULL)
    {
        return PNULL;
    }
    SCI_TRACE_LOW("mmisms new srv handle read sms");
    s_smssrv_instance.cur_handle = new_handle;

    //construct operator data and save it
    oper_data = &new_handle->oper_data.oper_cb_data;
    oper_data->user_data = user_data;
    oper_data->opreation_cb = action_data;
    new_handle->oper_data.callback_func = move_callback;
    //send to MN
    new_handle->record_id = 0;
    new_handle->storage = (MN_SMS_STORAGE_E)MN_SMS_NO_STORED;
    new_handle->oper_type = SRV_OPERATION_COPY;

    copy_order_id->flag.is_marked = FALSE;// 找到后将该标记清除，避免反复找到同一条信息

    if (copy_order_id->flag.is_search)
    {
        copy_order_id->flag.is_search = FALSE;
    }

    MMISMS_InitCurOperationOrderId();
    
    if (copy_order_id->flag.is_concatenate_sms) //is long sms
    {
        cur_order_id = copy_order_id;
        while (cur_order_id != PNULL)
        {
            MMISMS_SetCurOrderId(i,cur_order_id); //设置当前order_id到全局变量数组
            cur_order_id  = cur_order_id->next_long_sms_ptr;
            i++;
        }
    }
    else //is normal sms
    {
        MMISMS_SetCurOrderId(0,copy_order_id); //设置当前order_id到全局变量数组 
    }     
    
    MMISMS_ReadyReadSms();
    err_code = MNSMS_ReadSmsEx(
        (MN_DUAL_SYS_E)copy_order_id->flag.dual_sys, 
        (MN_SMS_STORAGE_E)copy_order_id->flag.storage, 
        copy_order_id->record_id 
        );
    
    if (err_code>ERR_MNSMS_NONE)
    {
        //Error handler
        MMISMSAPP_HandleCopyCnf(MMISMS_SMS_READ_FAIL);
    }            
    
    PutDataSrvMutex(&s_smssrv_instance.srv_mutex);

    return new_handle;
}

//This function is used for APP Control now
/******************************************************************************
//  Description : sms cnf handler: Read cnf
//  Global resource dependence :
//  Author:  sam.hua
//  Note:
 ******************************************************************************/
PUBLIC BOOLEAN MMISMSAPP_HandleReadCnf(MN_SMS_RECORD_ID_T record_id,MN_SMS_STORAGE_E   storage,BOOLEAN is_success)
{
    SMS_HANDLE_T    cmd_handle = PNULL;
    CMP_DATA_TAG    cmp_data = {0};

    SCI_TRACE_LOW("SCI_TRACE_LOW MMISMSAPP_HandleReadCnf result = %d,storage = %d,record_id = %d ",
           (uint8)is_success,(uint8)storage,(uint8)record_id);

    cmp_data.record_id = record_id;
    cmp_data.storage = storage;
    cmp_data.oper_type = SRV_OPERATION_READ;
    cmd_handle = DataSrvHandle_FindHandle(s_smssrv_instance.srvhandle_lst,
                &cmp_data,
                CmpHandle
                );
    //cmd_handle = s_smssrv_instance.cur_handle;
    
    if (cmd_handle == PNULL)
    {
        MMK_CloseWin(MMISMS_READSMS_WAIT_WIN_ID);

        SCI_TRACE_LOW("mmisms cmd_handle pnull");
        return FALSE;
    }

    cmd_handle->oper_data.oper_cb_data.result = is_success;

   //窗口已关闭的情况，cmd_handle->oper_data.oper_cb_data.opreation_cb已被释放了,不能使用与窗口绑定的数据
    if (is_success)
    {
        //Get result of read 
        if (cmd_handle->oper_data.oper_cb_data.opreation_cb!=PNULL)
        {
            MMISMS_READ_MSG_T read_result=MMISMS_GetReadResult();

            SCI_MEMCPY(cmd_handle->oper_data.oper_cb_data.opreation_cb,
                &read_result,
                sizeof(MMISMS_READ_MSG_T));
        }
    }

    {//temp debug	
    SCI_TRACE_MID("[MMI] MMI_CheckAllocatedMemInfo file:%s, line:%d", _D_FILE_NAME, _D_FILE_LINE);
    MMI_CheckAllocatedMemInfo();
    }
    
    //called sysn callback
    if(SMSSRV_Sync_Callback(
        cmd_handle->oper_data.oper_cb_data.operation,
        cmd_handle->oper_data.oper_cb_data.result,
        0,
        cmd_handle->oper_data.oper_cb_data.opreation_cb,
        cmd_handle->oper_data.oper_cb_data.user_data,
        cmd_handle->oper_data.callback_func))
    {
        {//temp debug	
        SCI_TRACE_MID("[MMI] MMI_CheckAllocatedMemInfo file:%s, line:%d", _D_FILE_NAME, _D_FILE_LINE);
        MMI_CheckAllocatedMemInfo();
        }
    
        DataSrvHandle_FreeHandle(s_smssrv_instance.srvhandle_lst,cmd_handle);
        SCI_TRACE_LOW("mmisms free read srv handle");
    }

    {//temp debug	
    SCI_TRACE_MID("[MMI] MMI_CheckAllocatedMemInfo file:%s, line:%d", _D_FILE_NAME, _D_FILE_LINE);
    MMI_CheckAllocatedMemInfo();
    }
        
   // MMISMS_ClearOperInfo();

   //Common actions

   //Close Waiting win and if need to update status

    //MMK_CloseWin(MMISMS_READSMS_WAIT_WIN_ID);

    return TRUE;
}

//This function is used for APP Control now
//All common issues should be placed here
/*****************************************************************************/
//     Description : delete cnf call back
//                  deleting the related MO SMS box
//    Global resource dependence : 
//  Author: rong.gu
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISMSAPP_HandleDeleteCnf(MN_SMS_RECORD_ID_T record_id,MN_SMS_STORAGE_E   storage)
{
    SMS_HANDLE_T    cmd_handle = PNULL;
    CMP_DATA_TAG    cmp_data = {0};

    SCI_TRACE_LOW("mmisms MMISMSAPP_HandleDeleteCnf  storage = %d,record_id =%d ",
           (uint8)storage,(uint8)record_id);
    
    cmp_data.record_id = record_id;
    cmp_data.storage = storage;
    cmp_data.oper_type = SRV_OPERATION_DELETE;
    
    cmd_handle = DataSrvHandle_FindHandle(s_smssrv_instance.srvhandle_lst,
                &cmp_data,
                CmpHandle
                );
    //cmd_handle = s_smssrv_instance.cur_handle;
    
    if (cmd_handle == PNULL)
    {
        SCI_TRACE_LOW("mmisms cmd_handle pnull");
        return FALSE;
    }
    //Get info from cnf
    SCI_TRACE_LOW("mmisms cmd_handle->oper_type %d",cmd_handle->oper_type);    
    if (cmd_handle->oper_type != SRV_OPERATION_DELETE)
    {
        return FALSE;
    }
   
    if(SMSSRV_Sync_Callback(
        cmd_handle->oper_data.oper_cb_data.operation,
        cmd_handle->oper_data.oper_cb_data.result,
        0,
        cmd_handle->oper_data.oper_cb_data.opreation_cb,
        cmd_handle->oper_data.oper_cb_data.user_data,
        cmd_handle->oper_data.callback_func))
    {
        DataSrvHandle_FreeHandle(s_smssrv_instance.srvhandle_lst,cmd_handle);
        SCI_TRACE_LOW("mmisms free delete srv handle");
    }
    return TRUE;
}

/*****************************************************************************/
//     Description : delete cnf call back
//                  deleting the related MO SMS box
//    Global resource dependence : 
//  Author: rong.gu
//    Note:
/*****************************************************************************/
PUBLIC   BOOLEAN MMISMSAPP_HandleMoveCnf(MMISMS_OPER_ERR_E oper_err)
{
//    SMSSRV_CALLBACK_STRUCT_T *oper_data = PNULL;
    SMS_HANDLE_T    cmd_handle = PNULL;
    CMP_DATA_TAG    cmp_data = {0};

    SCI_TRACE_LOW("MMISMSAPP_HandleMoveCnf  oper_err = %d",(uint8)oper_err);

    cmp_data.record_id = 0;
    cmp_data.storage = 0;
    cmp_data.oper_type = SRV_OPERATION_MOVE;
    cmd_handle = DataSrvHandle_FindHandle(s_smssrv_instance.srvhandle_lst,
                &cmp_data,
                CmpHandle
                );
    //cmd_handle = s_smssrv_instance.cur_handle;
    
    if (cmd_handle == PNULL)
    {
        SCI_TRACE_LOW("mmisms cmd_handle pnull");
        return FALSE;
    }
    //Get info from cnf
    SCI_TRACE_LOW("mmisms cmd_handle->oper_type %d",cmd_handle->oper_type);    
    if (cmd_handle->oper_type != SRV_OPERATION_MOVE)
    {
        return FALSE;
    }


    if(SMSSRV_Sync_Callback(
        cmd_handle->oper_data.oper_cb_data.operation,
        cmd_handle->oper_data.oper_cb_data.result,
        oper_err,
        cmd_handle->oper_data.oper_cb_data.opreation_cb,
        cmd_handle->oper_data.oper_cb_data.user_data,
        cmd_handle->oper_data.callback_func))
    {
        DataSrvHandle_FreeHandle(s_smssrv_instance.srvhandle_lst,cmd_handle);
        SCI_TRACE_LOW("mmisms free move srv handle");
    }
    return TRUE;
}


/*****************************************************************************/
//     Description : delete cnf call back
//                  deleting the related MO SMS box
//    Global resource dependence : 
//  Author: rong.gu
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISMSAPP_HandleCopyCnf(MMISMS_OPER_ERR_E oper_err)
{
//    SMSSRV_CALLBACK_STRUCT_T *oper_data = PNULL;
    SMS_HANDLE_T    cmd_handle = PNULL;
    CMP_DATA_TAG    cmp_data = {0};

    SCI_TRACE_LOW("mmisms MMISMSAPP_HandleCopyCnf  oper_err = %d",(uint8)oper_err);

    cmp_data.record_id = 0;
    cmp_data.storage = 0;
    cmp_data.oper_type = SRV_OPERATION_COPY;
    cmd_handle = DataSrvHandle_FindHandle(s_smssrv_instance.srvhandle_lst,
                &cmp_data,
                CmpHandle
                );
    //cmd_handle = s_smssrv_instance.cur_handle;
    
    if (cmd_handle == PNULL)
    {
        SCI_TRACE_LOW("mmisms cmd_handle pnull");
        return FALSE;
    }
    //Get info from cnf
    SCI_TRACE_LOW("mmisms cmd_handle->oper_type %d",cmd_handle->oper_type);    
    if (cmd_handle->oper_type != SRV_OPERATION_COPY)
    {
        return FALSE;
    }

    //cmd_handle->oper_data.oper_err = oper_err; 
        
    if(SMSSRV_Sync_Callback(
        cmd_handle->oper_data.oper_cb_data.operation,
        cmd_handle->oper_data.oper_cb_data.result,
        oper_err,
        cmd_handle->oper_data.oper_cb_data.opreation_cb,
        cmd_handle->oper_data.oper_cb_data.user_data,
        cmd_handle->oper_data.callback_func))
    {
        DataSrvHandle_FreeHandle(s_smssrv_instance.srvhandle_lst,cmd_handle);
        SCI_TRACE_LOW("mmisms free copy srv handle");
    }
	MMISMS_SetMoveSingleFlag(FALSE);
    return TRUE;
}


/******************************************************************************
//  Description : sms api: save sms
//  Global resource dependence :
//  Author:  sam.hua
//  Note:
 ******************************************************************************/
PUBLIC SMS_HANDLE_T MMISMSAPP_SaveSms(
    SMS_SAVE_OPERATE_DATA_T op_data,
    SmsSrvCallbackFunc save_callback,
    void *action_data,
    void *user_data)
{
    SMS_HANDLE_T  new_handle = PNULL;
    SMSSRV_CALLBACK_STRUCT_T *oper_data = PNULL;
    MMISMS_OPER_ERR_E err_code = MMISMS_NO_ERR;
    uint8        org_num = 0;

    SCI_TRACE_LOW("mmisms new srv handle save sms");
    if (s_smssrv_instance.srvhandle_lst == PNULL)
    {
        SmsDataSrv_init();
    }

    GetDataSrvMutex(&s_smssrv_instance.srv_mutex);

    //Get new sms handle
    new_handle = (SMS_HANDLE_T)DataSrvHandle_NewHandle(s_smssrv_instance.srvhandle_lst);

    if (new_handle == PNULL)
    {
        return PNULL;
    }

    s_smssrv_instance.cur_handle = new_handle;

    new_handle->oper_type  = SRV_OPERATION_SAVE;

    //construct operator data and save it to handle
    oper_data = &new_handle->oper_data.oper_cb_data;
    oper_data->user_data = user_data;
    oper_data->opreation_cb = action_data;
    
    new_handle->oper_data.callback_func = save_callback;

    
    PutDataSrvMutex(&s_smssrv_instance.srv_mutex);

    MMISMS_SetCurSaveDualSys(op_data.dual_sys);   

    //Convert save info
    if (!op_data.is_use_global_data)
    {
        MMISMS_SetMessageContent(op_data.save_strlen,(wchar*)op_data.save_string_ptr, TRUE, &g_mmisms_global.edit_content);
        SCI_MEMCPY(&g_mmisms_global.edit_content.dest_info,
                   op_data.sms_addr,sizeof(MMISMS_DEST_ADDR_INFO_T));
        MMISMS_SetIsSaveDraft(FALSE);
    }    

#ifdef MMI_TIMERMSG_SUPPORT
    MMISMS_SetIsTimerMode(op_data.is_timer_msg);
#endif
    
    //Judge conditions
    
    if (!MMISMS_HaveEnoughSpace(op_data.dual_sys, FALSE, org_num))
    {
        MMIPUB_OpenAlertWarningWin(TXT_NO_SPACE);
        //Free Handle 
        DataSrvHandle_FreeHandle(s_smssrv_instance.srvhandle_lst,new_handle);
        return PNULL;
    }

    //Call Save action
    err_code = MMISMS_AppSaveSms(op_data.dual_sys,FALSE);

    //Check API result
    if( err_code>MMISMS_NO_ERR)
    {
        if (err_code == MMISMS_NO_SPACE)
        {
            MMIPUB_OpenAlertWarningWin(TXT_NO_SPACE);
        }
        DataSrvHandle_FreeHandle(s_smssrv_instance.srvhandle_lst,new_handle);
        return PNULL;
    }
    else
    {
    #ifdef MMI_TIMERMSG_SUPPORT
        if (op_data.is_timer_msg)
        {
            MMISMS_DisplayPrompByErr(err_code, TXT_COMM_SAVE_SUCCESS);		
        }
        else
#endif        
        {
            MMISMS_DisplayPrompByErr(err_code, TXT_COMM_SAVE_AS_DRAFT);	
            if(MMK_IsOpenWin(MMISMS_SAVEMSG_ANIM_WIN_ID))
            {
                MMK_CloseWin(MMISMS_SAVEMSG_ANIM_WIN_ID);
            }
        }
        return new_handle;
        
   }     
}


/******************************************************************************
//  Description : sms cnf handler: save cnf
//  Global resource dependence :
//  Author:  sam.hua
//  Note:
 ******************************************************************************/
PUBLIC BOOLEAN MMISMSAPP_HandleSaveCnf(BOOLEAN is_succss)
{

//    SMSSRV_CALLBACK_STRUCT_T *oper_data = PNULL;
    SMS_HANDLE_T    cmd_handle = PNULL;
    CMP_DATA_TAG    cmp_data = {0};

    SCI_TRACE_LOW("mmisms SCI_TRACE_LOW SMSHandleSaveCnf result = %d",(uint8)is_succss);

    cmp_data.record_id = 0;
    cmp_data.storage = 0;
    cmp_data.oper_type = SRV_OPERATION_SAVE;
    cmd_handle = DataSrvHandle_FindHandle(s_smssrv_instance.srvhandle_lst,
                &cmp_data,
                CmpHandle
                );
    //cmd_handle = s_smssrv_instance.cur_handle;
    
    if (cmd_handle == PNULL)
    {
        return FALSE;
    }

    cmd_handle->oper_data.oper_cb_data.result  = is_succss;
    
    //called sysn callback
    if(SMSSRV_Sync_Callback(
        cmd_handle->oper_data.oper_cb_data.operation,
        cmd_handle->oper_data.oper_cb_data.result,
        0,
        cmd_handle->oper_data.oper_cb_data.opreation_cb,
        cmd_handle->oper_data.oper_cb_data.user_data,
        cmd_handle->oper_data.callback_func))
    {
        DataSrvHandle_FreeHandle(s_smssrv_instance.srvhandle_lst,cmd_handle);
        SCI_TRACE_LOW("mmisms free srv handle");
    }

#ifdef MMI_TIMERMSG_SUPPORT
    if (MMISMS_GetIsTimerMode())
    {
        MMISMS_SetIsTimerMode(FALSE);
    }
#endif
   
    MMISMS_ClearSaveInfo();
    if (!is_succss)
    {
        MMISMS_OpenMsgBox(
                TXT_ERROR, 
                IMAGE_PUBWIN_WARNING  
                );
    }
    MMIPUB_CloseWaitWin(MMISMS_SAVEMSG_ANIM_WIN_ID);
#ifdef MMI_TIMERMSG_SUPPORT
    if (MMISMS_GetIsTimerMsgReplace()) //timermsg oper MMISMS_DELETE_MSG
    {    
        MMISMS_SetIsTimerMsgReplace(FALSE);
    }
    else
#endif                
    {
        MMISMS_ClearOperInfo();
    }
    return TRUE;
}

/******************************************************************************
//  Description : sms api: Replace sms
//  Global resource dependence :
//  Author:  sam.hua
//  Note:
 ******************************************************************************/
PUBLIC SMS_HANDLE_T MMISMSAPP_ReplaceSms(
    SMS_SAVE_OPERATE_DATA_T op_data,
    SmsSrvCallbackFunc save_callback,
    void *action_data,
    void *user_data)
{
    SMS_HANDLE_T  new_handle = PNULL;

    SMSSRV_CALLBACK_STRUCT_T *oper_data = PNULL;
    MMISMS_OPER_ERR_E err_code = MMISMS_NO_ERR;
    uint8        org_num = 0;
    MN_CALLED_NUMBER_T dest_num = {0};

    SCI_TRACE_LOW("SCI_TRACE_LOW MMISMSAPP_ReplaceSms");

    
    if (s_smssrv_instance.srvhandle_lst == PNULL)
    {
        SmsDataSrv_init();
    }

    GetDataSrvMutex(&s_smssrv_instance.srv_mutex);

    //Get new sms handle
    new_handle = (SMS_HANDLE_T)DataSrvHandle_NewHandle(s_smssrv_instance.srvhandle_lst);

    if (new_handle == PNULL)
    {
        return PNULL;
    }

    s_smssrv_instance.cur_handle = new_handle;

    //construct operator data and save it to handle
    oper_data = &new_handle->oper_data.oper_cb_data;
    oper_data->user_data = user_data;
    oper_data->opreation_cb = action_data;
    
    new_handle->oper_type  = SRV_OPERATION_REPLACE;
    new_handle->oper_data.callback_func = save_callback;

    
    PutDataSrvMutex(&s_smssrv_instance.srv_mutex);


    //Convert save info
    if (!op_data.is_use_global_data)
    {    
        MMISMS_SetCurSaveDualSys(op_data.dual_sys);   
        MMISMS_SetMessageContent(op_data.save_strlen,(wchar*)op_data.save_string_ptr, TRUE, &g_mmisms_global.edit_content);
        SCI_MEMCPY(&g_mmisms_global.edit_content.dest_info,
                   op_data.sms_addr,sizeof(MMISMS_DEST_ADDR_INFO_T));
    }

    MMISMS_SetIsSaveDraft(FALSE);
    
#ifdef MMI_TIMERMSG_SUPPORT
    MMISMS_SetIsTimerMode(op_data.is_timer_msg);
#endif
    //Judge conditions
    org_num = MMISMS_GetReadSMSNum();
    
    if (!MMISMS_HaveEnoughSpace(op_data.dual_sys, FALSE, org_num))
    {
        MMIPUB_OpenAlertWarningWin(TXT_NO_SPACE);
        //Free Handle 
        DataSrvHandle_FreeHandle(s_smssrv_instance.srvhandle_lst,new_handle);
        return PNULL;
    }

    MMISMS_SaveUpdateAddr(TRUE);
    if (MMISMS_GetDestNumber(&dest_num, &g_mmisms_global.edit_content.dest_info))
    {
        MMISMS_SetSMSSaveInfo(op_data.dual_sys, &dest_num);
    }
    else
    {
        MMISMS_ClearSaveInfo();
    }
    MMISMS_SetOperStatus(MMISMS_UPDATE_NOSEND_SMS);
    // the draft box is opened, indicate update current message
    err_code = MMISMS_AppUpdateMoSms(op_data.dual_sys, MMISMS_UPDATE_NOSEND_SMS, MN_SMS_MO_TO_BE_SEND, PNULL);

    //Check API result
    if( err_code>MMISMS_NO_ERR)
    {
        if (err_code == MMISMS_NO_SPACE)
        {
            MMIPUB_OpenAlertWarningWin(TXT_NO_SPACE);
        }
        DataSrvHandle_FreeHandle(s_smssrv_instance.srvhandle_lst,new_handle);
        return PNULL;
    }
    else
    {
#ifdef MMI_TIMERMSG_SUPPORT
        if (op_data.is_timer_msg)
        {
                MMISMS_DisplayPrompByErr(err_code, TXT_COMM_SAVE_SUCCESS);		
        }
        else
#endif        
        {
            MMISMS_DisplayPrompByErr(err_code, TXT_COMM_SAVE_AS_DRAFT);	
        }
        
        return new_handle;
   }     
}

/******************************************************************************
//  Description : sms cnf handler: Replace cnf
//  Global resource dependence :
//  Author:  sam.hua
//  Note:
 ******************************************************************************/
PUBLIC BOOLEAN MMISMSAPP_HandleReplaceCnf(MN_SMS_RECORD_ID_T record_id,MN_SMS_STORAGE_E   storage,BOOLEAN is_success)
{

    SMS_HANDLE_T    cmd_handle = PNULL;
    CMP_DATA_TAG    cmp_data = {0};

    SCI_TRACE_LOW("mmisms MMISMSAPP_HandleReplaceCnf result= %d,record_id =%d,storage = %d",
           (uint8)is_success, (uint8)record_id,(uint8)storage);

    cmp_data.record_id = 0;
    cmp_data.storage = 0;
    cmp_data.oper_type = SRV_OPERATION_REPLACE;
    cmd_handle = DataSrvHandle_FindHandle(s_smssrv_instance.srvhandle_lst,
                &cmp_data,
                CmpHandle
                );
    //cmd_handle = s_smssrv_instance.cur_handle;
    
    if (cmd_handle == PNULL)
    {
        return FALSE;
    }

    cmd_handle->oper_data.oper_cb_data.result  = is_success;
    
    //called sysn callback
    if(SMSSRV_Sync_Callback(
        cmd_handle->oper_data.oper_cb_data.operation,
        cmd_handle->oper_data.oper_cb_data.result,
        0,
        cmd_handle->oper_data.oper_cb_data.opreation_cb,
        cmd_handle->oper_data.oper_cb_data.user_data,
        cmd_handle->oper_data.callback_func))
    {
        DataSrvHandle_FreeHandle(s_smssrv_instance.srvhandle_lst,cmd_handle);
    }
#ifdef MMI_TIMERMSG_SUPPORT
    if (MMISMS_GetIsTimerMode())
    {
        MMISMS_SetIsTimerMode(FALSE);
    }
#endif

    MMISMS_ClearSaveInfo();
    if (!is_success)
    {
        MMISMS_OpenMsgBox(
                TXT_ERROR, 
                IMAGE_PUBWIN_WARNING  
                );
    }
    MMIPUB_CloseWaitWin(MMISMS_SAVEMSG_ANIM_WIN_ID);
    MMISMS_ClearOperInfo();

    return TRUE;
}


/******************************************************************************
//  Description : sms internal api: send sms
//  Global resource dependence :
//  Author:  sam.hua
//  Note:
 ******************************************************************************/
LOCAL SMS_HANDLE_T MMISMSAPP_SendSms(
    SMS_SEND_OPERATE_DATA_T op_data,
    SmsSrvCallbackFunc read_callback,
    void *action_data,
    void *user_data);

/******************************************************************************
//  Description : sms api: send sms by its send data
//  Global resource dependence :
//  Author:  sam.hua
//  Note:
 ******************************************************************************/
PUBLIC SMS_HANDLE_T MMISMSAPP_SendSmsWithData(
                                MN_DUAL_SYS_E    dual_sys,
                                BOOLEAN        is_re_send ,
                                MMISMS_DEST_ADDR_INFO_T*  sms_addr,
                                wchar         *save_string_ptr,    
                                uint32        save_strlen,    
                                SmsSrvCallbackFunc read_callback
                               )
{
    SMS_SEND_OPERATE_DATA_T op_data={0};

    SCI_TRACE_LOW("SCI_TRACE_LOW MMISMSAPP_SendSmsWithData");

    op_data.dual_sys = dual_sys;
    op_data.is_resend = is_re_send;
    op_data.save_strlen = save_strlen;
    op_data.save_string_ptr = save_string_ptr;
    op_data.sms_addr = sms_addr;

    MMISMS_SetCurSendDualSys(op_data.dual_sys);   
    MMISMS_SetMessageContent(op_data.save_strlen,(wchar*)op_data.save_string_ptr, TRUE, &g_mmisms_global.edit_content);
    SCI_MEMCPY(&g_mmisms_global.edit_content.dest_info,
               op_data.sms_addr,sizeof(MMISMS_DEST_ADDR_INFO_T));

    
    return MMISMSAPP_SendSms(op_data,read_callback,PNULL,PNULL);
}    

/******************************************************************************
//  Description : sms api: send sms by sending content
//  Global resource dependence :
//  Author:  sam.hua
//  Note:
 ******************************************************************************/
PUBLIC SMS_HANDLE_T MMISMSAPP_SendSmsWithContent(
                                MN_DUAL_SYS_E    dual_sys,
                                BOOLEAN        is_re_send ,
                                MMISMS_SMSEDIT_CONTENT_T*  edit_content,
                                SmsSrvCallbackFunc read_callback
                               )
{
    SMS_SEND_OPERATE_DATA_T op_data={0};

    SCI_TRACE_LOW("SCI_TRACE_LOW MMISMSAPP_SendSmsWithContent");

    op_data.dual_sys = dual_sys;
    op_data.is_resend = is_re_send;

    MMISMS_SetEditContentInfo(edit_content);
    MMISMS_SetCurSendDualSys(op_data.dual_sys);   
    SCI_MEMCPY(&g_mmisms_global.edit_content.dest_info,
               op_data.sms_addr,sizeof(MMISMS_DEST_ADDR_INFO_T));
    
    return MMISMSAPP_SendSms(op_data,read_callback,PNULL,PNULL);
}

/******************************************************************************
//  Description : sms api: send sms by global content
//  Global resource dependence :
//  Author:  sam.hua
//  Note:
 ******************************************************************************/
PUBLIC SMS_HANDLE_T MMISMSAPP_SendSmsWithGlobalContent(
                                MN_DUAL_SYS_E    dual_sys,
                                BOOLEAN          is_re_send
                               )
{
    SMS_SEND_OPERATE_DATA_T op_data={0};

    SCI_TRACE_LOW("SCI_TRACE_LOW MMISMSAPP_SendSmsWithGlobalContent");

    op_data.dual_sys = dual_sys;
    op_data.is_resend = is_re_send;
    
    return MMISMSAPP_SendSms(op_data,PNULL,PNULL,PNULL);
}    

/******************************************************************************
//  Description : sms api: sms send api
//  Global resource dependence :
//  Author:  sam.hua
//  Note:
 ******************************************************************************/
LOCAL SMS_HANDLE_T MMISMSAPP_SendSms(
    SMS_SEND_OPERATE_DATA_T op_data,
    SmsSrvCallbackFunc read_callback,
    void *action_data,
    void *user_data)
{
    SMS_HANDLE_T  new_handle = PNULL;
    MMISMS_OPER_ERR_E    err_code= MMISMS_NO_ERR;
    SMSSRV_CALLBACK_STRUCT_T *oper_data = PNULL;

    SCI_TRACE_LOW("SCI_TRACE_LOW MMISMSAPP_SendSms");
    
    if (s_smssrv_instance.srvhandle_lst == PNULL)
    {
        SmsDataSrv_init();
    }

    GetDataSrvMutex(&s_smssrv_instance.srv_mutex);

    //Get new sms handle
    new_handle = (SMS_HANDLE_T)DataSrvHandle_NewHandle(s_smssrv_instance.srvhandle_lst);

    if (new_handle == PNULL)
    {
        return PNULL;
    }
    SCI_TRACE_LOW("mmisms new srv handle send sms");
    s_smssrv_instance.cur_handle = new_handle;

    //construct operator data and save it
    oper_data = &new_handle->oper_data.oper_cb_data;
    oper_data->user_data = user_data;
    oper_data->opreation_cb = action_data;
//    oper_data->oper_order_id = op_data.oper_order_id;
    new_handle->oper_data.callback_func = read_callback;
    //send to MN
    new_handle->record_id = 0;
    new_handle->storage = 0;
    new_handle->oper_type = SRV_OPERATION_SEND;

    PutDataSrvMutex(&s_smssrv_instance.srv_mutex);

    MMISMS_ClearDestId();
    MMISMS_SetDeliverResendFlag(FALSE);
    MMISMS_SetCurSendDualSys(op_data.dual_sys);
    err_code = MMISMS_HandleSendOperation(op_data.dual_sys, PNULL,op_data.is_resend);

    if(err_code>MMISMS_NO_ERR)
    {
        DataSrvHandle_FreeHandle(s_smssrv_instance.srvhandle_lst,new_handle);
        return PNULL;
    }
    //Now we need handle send cnf 
    if (read_callback == PNULL)
    {
        DataSrvHandle_FreeHandle(s_smssrv_instance.srvhandle_lst,new_handle);
    }
    return new_handle;
}

PUBLIC BOOLEAN MMISMSAPP_HandleSendCnf(BOOLEAN is_succss)
{
    SMS_HANDLE_T    cmd_handle = PNULL;
    CMP_DATA_TAG    cmp_data = {0};

    SCI_TRACE_LOW("mmisms SCI_TRACE_LOW SMSHandleSendCnf result = %d",(uint8)is_succss);

    cmp_data.record_id = 0;
    cmp_data.storage = 0;
    cmp_data.oper_type = SRV_OPERATION_SEND;
    cmd_handle = DataSrvHandle_FindHandle(s_smssrv_instance.srvhandle_lst,
                &cmp_data,
                CmpHandle
                );

    if (cmd_handle == PNULL)
    {
        return FALSE;
    }

    cmd_handle->oper_data.oper_cb_data.result  = is_succss;

    //called sysn callback
    SMSSRV_Sync_Callback(
        cmd_handle->oper_data.oper_cb_data.operation,
        cmd_handle->oper_data.oper_cb_data.result,
        0,
        cmd_handle->oper_data.oper_cb_data.opreation_cb,
        cmd_handle->oper_data.oper_cb_data.user_data,
        cmd_handle->oper_data.callback_func);

    DataSrvHandle_FreeHandle(s_smssrv_instance.srvhandle_lst,cmd_handle);
    SCI_TRACE_LOW("mmisms free srv handle");

    return TRUE;
}

wchar g_mmifile_sms_dir_wstr[]      = { 'S', 'M' ,'S', 0 };
#define MMISMS_INFO_FILE_NAME_LEN   20
#define MMISMS_INFO_FILE_SUFFIX  "SMS"
#define MMISMS_INFO_FILE_TXT  ".txt"

#define MMIFILE_DIR_SMS                  (g_mmifile_sms_dir_wstr)
#define MMIFILE_DIR_SMS_LEN                3
/*****************************************************************************/
//  Description : 根据event_id生成sms文件
//  Global resource
//  Author: qi.liu1
//  Note: GetRingInfoFileName
/*****************************************************************************/
LOCAL BOOLEAN GetSmsInfoFileName(
                                     uint16 event_id,
                                     wchar *u16filename_ptr,//OUT
                                     uint16 u16filename_max_len
                                     )
{
    BOOLEAN         ret = FALSE;
    uint8           u8filename[MMISMS_INFO_FILE_NAME_LEN +1] = {0};
    wchar           wstr_filename[MMISMS_INFO_FILE_NAME_LEN +1] = {0};
    uint32          u32index = event_id;//(group_id << 16) + entry_id;
//  FILE_DEV_E_T    file_dev = FS_UDISK;
    uint16          full_path_len = u16filename_max_len;
    uint16          wstr_len  = 0;
    wchar           *device_name_ptr = PNULL;
    uint16           device_name_len = 0;
    MMIFILE_DEVICE_E dev = MMI_DEVICE_UDISK;
    uint8           name_temp[MMISMS_INFO_FILE_NAME_LEN +1] = {0};

    if(PNULL != u16filename_ptr)
    {
        sprintf( (char*)name_temp,"%s%ld",MMISMS_INFO_FILE_SUFFIX,u32index);
        sprintf( (char*)u8filename,"%s%s",(char*)name_temp,MMISMS_INFO_FILE_TXT);

        ////dev = MMIAPIFMM_GetFirstValidDevice();
        device_name_ptr = MMIAPIFMM_GetDevicePath(dev);
        device_name_len = MMIAPIFMM_GetDevicePathLen(dev);

        if((PNULL == device_name_ptr) || (0 == device_name_len))
        {
            return FALSE;
        }

        wstr_len = MIN(strlen( (char*)u8filename), MMISMS_INFO_FILE_NAME_LEN); /*lint !e666*/
        MMI_STRNTOWSTR(wstr_filename, MMISMS_INFO_FILE_NAME_LEN, (char*)u8filename, MMISMS_INFO_FILE_NAME_LEN, wstr_len);
        if (!MMIAPIFMM_CombineFullPath(
                                device_name_ptr,
                                device_name_len,
                                MMIFILE_DIR_SMS,
                                MMIFILE_DIR_SMS_LEN,
                                wstr_filename,
                                wstr_len,
                                (wchar *)u16filename_ptr,
                                &full_path_len
                              )
        )
        {
            SCI_TRACE_LOW("lqlq: GetSmsInfoFileName CombineFullPath fail!");
            return ret;
        }

        if(full_path_len <= MMIFILE_FILE_NAME_MAX_LEN)
        {
            u16filename_ptr[full_path_len] = 0;
        }

        ret = TRUE;
    }

    SCI_TRACE_LOW("lqlq:GetSmsInfoFileName event_id %d, ret %d",event_id,ret);
    return ret;
}


/*****************************************************************************/
// Description : 创建SMS目录
// Global resource dependence :
// Author:qi.liu1
// Note: InitAlarmFolder
/*****************************************************************************/
LOCAL BOOLEAN InitSMSFolder(void)
{
    wchar            full_path_name[MMIFILE_FULL_PATH_MAX_LEN+2] = {0};
    uint16           full_path_len = MMIFILE_FULL_PATH_MAX_LEN+2;
    wchar            *file_dev_ptr = PNULL;
    uint16           file_dev_len = 0;
    BOOLEAN          result = FALSE;
    MMIFILE_DEVICE_E dev = MMI_DEVICE_UDISK;

    //dev = MMIAPIFMM_GetFirstValidDevice();
    file_dev_ptr = MMIAPIFMM_GetDevicePath(dev);
    file_dev_len = MMIAPIFMM_GetDevicePathLen(dev);
    if((PNULL == file_dev_ptr) || (0 == file_dev_len))
    {
        SCI_TRACE_LOW("InitSMSFolder ptr=PNULL!");
        return FALSE;
    }

    if(MMIAPIFMM_CombineFullPath(file_dev_ptr, file_dev_len,
        MMIFILE_DIR_SMS, MMIFILE_DIR_SMS_LEN,
        NULL, 0,
        full_path_name, &full_path_len))
    {
        if(!MMIAPIFMM_IsFolderExist(full_path_name, full_path_len))
        {
            result = MMIAPIFMM_CreateDir(full_path_name, full_path_len);
        }
        else
        {
            result = TRUE;
        }
    }

	SCI_TRACE_LOW("InitSMSFolder result %d",result);
    return result;
}


/*****************************************************************************/
//  Description : 把sms_ptr保存到根据event_id映射成的SMS id信息文件中
//  Global resource
//  Author: qi.liu1
//  Note:
//  Return: SaveRingInfoToFile
/*****************************************************************************/
LOCAL BOOLEAN SaveSmsInfoToFile(
                                           uint16 event_id,
                                           const MN_SMS_MESSAGE_ITEM_T *sms_ptr//IN
                                           )
{
    BOOLEAN ret = FALSE;
    uint16 u16filename[MMIFILE_FILE_NAME_MAX_LEN +1] = {0};
    uint32 write_size = 0;

    MMIFILE_HANDLE file_handle = 0;
    MMIFILE_ERROR_E file_ret = SFS_NO_ERROR;

    if(PNULL != sms_ptr)
    {
        if(GetSmsInfoFileName(event_id, u16filename,(MMIFILE_FILE_NAME_MAX_LEN + 1)))
        {
            if(InitSMSFolder())
            {
                //打开文件
                file_handle = MMIAPIFMM_CreateFile(u16filename, SFS_MODE_WRITE|SFS_MODE_OPEN_ALWAYS, NULL, NULL);
                SCI_TRACE_LOW("SaveSmsInfoToFile id=%d,status=%d,dual_sys=%d,tp_mr=%d",event_id,sms_ptr->status,sms_ptr->dual_sys,sms_ptr->tpdu);

                if(SFS_INVALID_HANDLE != file_handle)
                {
                    file_ret = MMIAPIFMM_WriteFile(
                        file_handle, 
                        sms_ptr,
                        sizeof(MN_SMS_MESSAGE_ITEM_T),
                        &write_size, 
                        NULL
                        );

                    SCI_TRACE_LOW("SaveSmsInfoToFile file_ret %d",file_ret);
                    if(SFS_ERROR_NONE == file_ret)
                    {
                        ret = TRUE;
                    }
                    MMIAPIFMM_CloseFile(file_handle);
                }

            }
            else
            {
                SCI_TRACE_LOW("lqlq SaveSmsInfoToFile create SMS file Fail!");
            }
        }
    }
    return ret;
}

/*****************************************************************************/
//  Description : OTA升级前，将SMS从NV保存到文件
//  Global resource
//  Author: qi.liu1
//  Note:
//  Return:
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_OTASaveSMSToFile(void)
{
    uint32 i = 0;
    BOOLEAN nv_ret = FALSE;
    MN_SMS_MESSAGE_ITEM_T    *sms_ptr = PNULL;

    sms_ptr = (MN_SMS_MESSAGE_ITEM_T *)SCI_ALLOC_APPZ(sizeof(MN_SMS_MESSAGE_ITEM_T));

    if(PNULL != sms_ptr)
    {
        for(i=0; i<MMINV_MAX_SMS_RECORDS; i++)
        {
            SCI_MEMSET(sms_ptr,0,sizeof(MN_SMS_MESSAGE_ITEM_T));
            nv_ret = MMISMS_NV_ReadSmsItem(i,sms_ptr);

            SCI_TRACE_LOW("MMISMS_OTASaveSMSToFile for nv_ret=%d i=%d",nv_ret,i);

            if(FALSE != nv_ret)
            {
                nv_ret = SaveSmsInfoToFile(i,sms_ptr);
    
                if(FALSE == nv_ret)
                {
                    SCI_TRACE_LOW("MMISMS_OTASaveSMSToFile i=%d save file Fail!",i);
                }
            }
            else
            {
                SCI_TRACE_LOW("MMISMS_OTASaveSMSToFile i=%d read nv Fail!",i);
            }
        }

    }
    else
    {
        SCI_TRACE_LOW("MMISMS_OTASaveSMSToFile ptr=PNULL!");
        return FALSE;
    }

    SCI_FREE(sms_ptr);

    SCI_TRACE_LOW("MMISMS_OTASaveSMSToFile nv_ret=%d",nv_ret);
    return nv_ret;
}

#if 0
PUBLIC BOOLEAN MMISMS_DeleteAllNvSMS(void)
{
    uint32 i = 0;
	BOOLEAN del_ret = FALSE;

	for(i=0; i<MMINV_MAX_SMS_RECORDS; i++)
    {
		del_ret = MMISMS_NV_DelAllSmsItem(i);
	}
    return del_ret;
}
#endif

/*****************************************************************************/
//  Description : 把id，将文件中的短信数据取出来
//  Global resource
//  Author: qi.liu1
//  Note:
//  Return:
/*****************************************************************************/
LOCAL BOOLEAN ReadSMSFileInfo(
                                           uint16 event_id,
                                           MN_SMS_MESSAGE_ITEM_T *sms_ptr//IN
                                           )
{
    BOOLEAN ret = FALSE;
    uint16 u16filename[MMIFILE_FILE_NAME_MAX_LEN +1] = {0};
    uint32 write_size = 0;

    MMIFILE_HANDLE file_handle = 0;
    MMIFILE_ERROR_E file_ret = SFS_NO_ERROR;

    if(PNULL != sms_ptr)
    {
        if(GetSmsInfoFileName(event_id, u16filename,(MMIFILE_FILE_NAME_MAX_LEN + 1)))
        {
            {
                //打开文件
                file_handle = MMIAPIFMM_CreateFile(u16filename, SFS_MODE_READ | SFS_MODE_OPEN_EXISTING, NULL, NULL);/*lint !e655*/
                if(SFS_INVALID_HANDLE != file_handle)
                {
                    file_ret = MMIAPIFMM_ReadFile(
                        file_handle,
                        sms_ptr,//sms_ptr
                        sizeof(MN_SMS_MESSAGE_ITEM_T),
                        &write_size,
                        NULL
                        );
                    SCI_TRACE_LOW("ReadSMSFileInfo id=%d,status=%d,dual_sys=%d,tp_mr=%d",event_id,sms_ptr->status,sms_ptr->dual_sys,sms_ptr->tpdu);
                    SCI_TRACE_LOW("ReadSMSFileInfo file_ret %d",file_ret);
                    if(SFS_ERROR_NONE == file_ret)
                    {
                        ret = TRUE;
                    }
                    MMIAPIFMM_CloseFile(file_handle);
                }
                else
                {
                    SCI_TRACE_LOW("ReadSMSFileInfo FAIL!");
                }
            }
        }
    }

    return ret;
}

#if 1
/*****************************************************************************/
//  Description : OTA升级后，将SMS文件保存到NV
//  Global resource
//  Author: qi.liu1
//  Note:
//  Return:
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_OTASaveFileToNv(void)
{
    uint32 i = 0;
    BOOLEAN nv_ret = FALSE;
    MN_SMS_MESSAGE_ITEM_T  *sms_ptr = PNULL;

    SCI_TRACE_LOW("MMISMS_OTASaveFileToNv enter");

    sms_ptr = (MN_SMS_MESSAGE_ITEM_T *)SCI_ALLOC_APPZ(sizeof(MN_SMS_MESSAGE_ITEM_T));

    if(PNULL != sms_ptr)
    {
        for(i=0; i<MMINV_MAX_SMS_RECORDS; i++)
        {
            SCI_MEMSET(sms_ptr,0,sizeof(MN_SMS_MESSAGE_ITEM_T));
            nv_ret = ReadSMSFileInfo(i,sms_ptr);
        
            if(FALSE != nv_ret)
            {
                nv_ret = MMIAPISMS_NV_WriteSmsItem(FALSE,i,sms_ptr);
                if(FALSE == nv_ret)
                {
                    SCI_TRACE_LOW("MMISMS_OTASaveFileToNv i=%d save nv Fail!",i);
                }
            }
            else
            {
                SCI_TRACE_LOW("MMISMS_OTASaveFileToNv i=%d read Fail!",i);
            }
        }
    }
    else
    {
        SCI_TRACE_LOW("MMISMS_OTASaveFileToNv ptr=PNULL!");
        return FALSE;
    }

    if(PNULL != sms_ptr)
    {
        SCI_FREE(sms_ptr);
    }

    return nv_ret;
}
#endif

#if 0
/*****************************************************************************/
//  Description : 根据id删除短信文件
//  Global resource
//  Author: qi.liu1
//  Note:
//  Return:
/*****************************************************************************/
LOCAL BOOLEAN DeleteSMSFile(uint16 event_id)
{
    BOOLEAN ret = TRUE;
    uint16 u16filename[MMIFILE_FILE_NAME_MAX_LEN +1] = {0};
    uint32 write_size = 0;

    MMIFILE_HANDLE file_handle = 0;
    MMIFILE_ERROR_E file_ret = SFS_NO_ERROR;

    {
        if(GetSmsInfoFileName(event_id, u16filename,(MMIFILE_FILE_NAME_MAX_LEN + 1)))
        {
            {
                file_ret = MMIFILE_DeleteFile(u16filename,PNULL);
                if(SFS_NO_ERROR != file_ret)
                {
                    SCI_TRACE_LOW("DeleteSMSFile FAIL!");
                }
                SCI_TRACE_LOW("DeleteSMSFile file_ret=%d!",file_ret);
            }
        }
    }
    return ret;

}
#endif

/*****************************************************************************/
//  Description : OTA升级后，删除所有的短信文件
//  Global resource
//  Author: qi.liu1
//  Note:
//  Return:
/*****************************************************************************/
PUBLIC void MMISMS_OTADelSMSFile(void)
{
    uint16 i=0;
    uint16 u16filename[MMIFILE_FILE_NAME_MAX_LEN +1] = {0};
    uint32 write_size = 0;
    MMIFILE_ERROR_E file_ret = SFS_NO_ERROR;

    SCI_TRACE_LOW("lqlq:MMISMS_OTADeleteAllSMSFile enter!");
    for(i=0; i<MMINV_MAX_SMS_RECORDS; i++)
    {
        //获取全路径名称
        if(GetSmsInfoFileName(i, u16filename,(MMIFILE_FILE_NAME_MAX_LEN + 1)))
        {
            //删除文件
            file_ret = MMIFILE_DeleteFile(u16filename,PNULL);
            if(SFS_NO_ERROR != file_ret)
            {
                SCI_TRACE_LOW("DeleteSMSFile FAIL!");
            }
            SCI_TRACE_LOW("DeleteSMSFile file_ret=%d!",file_ret);
        }
    }
}

