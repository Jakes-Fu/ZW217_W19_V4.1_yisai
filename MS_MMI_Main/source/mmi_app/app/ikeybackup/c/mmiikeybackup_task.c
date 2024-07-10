/*****************************************************************************
** File Name:      mmiikeybackup_task.c                                      *
** Author:                                                                   *
** Date:           12/12/2011                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to handle 1-keybackup task message.     *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 12/12/2011     haiwu.chen       Create                                    *
******************************************************************************/

#include "mmi_app_ikeybackup_trc.h"
#if defined(IKEYBACKUP_SUPPORT)

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmiikeybackup_task.h"
#include "tasks_id.h"
#include "os_api.h"
#include "mmi_signal.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmk_app.h"
#include "mmk_timer.h"
#include "sci_api.h"


#ifdef WIN32
#pragma warning (disable:4244)
#pragma warning (disable:4127)
#pragma warning (disable:4761)
#endif

/*lint -e401*/
/*lint -save -e785 */

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif


/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define IKEYBACKUP_ENCRYPT_DATA         (37)
#define IKEYBACKUP_CRC_SEG_LEN          (10*1024)       //算CRC分段算
#define IKEYBACKUP_TIMER_OUT            (10*1000)       //一键备份过时保护时长
#define IKEYBACKUP_DATA_END_SIG         (0xFFFFFFFF)
#define IKEYBACKUP_TEST_CODE            (0)

/**--------------------------------------------------------------------------*
 **                         TYPE DEFINITION                                  *
 **--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         LOCAL VARIABLE DEFINITION                        *
 **--------------------------------------------------------------------------*/
//task id
LOCAL BLOCK_ID s_ikeybackup_task_id = SCI_INVALID_BLOCK_ID;

//current task sequeneces table
LOCAL MMIIKB_TASK_SEQ_TABLE_T s_ikb_task_seq_table[MMIIKB_TYPE_MAX] = {0};


LOCAL MMIIKB_TYPE_SETTING_T const s_type_setting[] =
{
    {MMIIKB_TYPE_CALLLOG,   {'c','a','l','l','o','g','.','i','b',0}},
    {MMIIKB_TYPE_PHONEBOOK, {'p','h','o','n','e','b','o','o','k','.','i','b',0}},
    {MMIIKB_TYPE_SMS,       {'s','m','s','.','i','b',0}},
    {MMIIKB_TYPE_SCHEDULE,  {'s','c','h','e','d','u','l','e','.','i','b',0}},
    {MMIIKB_TYPE_BRO_LABEL, {'b','r','o','_','l','a','b','l','e','.','i','b',0}}
};

//由于信号从APP传给IKEYBACKUP后，如果是异步的话，
//下面这些关键信息还没有做完，就已经释放了，因此需要在任务启动之前COPY一份
LOCAL MMIIKB_SIG_BACKUP_REQ_T * s_backup_req_ptr = PNULL;
LOCAL MMIIKB_SIG_RESTORE_REQ_T * s_restore_req_ptr = PNULL;

//cancel mutex
LOCAL SCI_MUTEX_PTR s_ikb_cancel_mutext = PNULL;
LOCAL BOOLEAN s_ikb_action_cancel = FALSE;
LOCAL wchar *s_cancel_pack_path_ptr = PNULL;

//timeout protection mechanism
LOCAL SCI_TIMER_PTR s_ikb_protect_timer = PNULL;
LOCAL BOOLEAN s_is_timeout = FALSE;

/**--------------------------------------------------------------------------*
 **                         LOCAL FUNCTION DECLARE                           *
 **--------------------------------------------------------------------------*/
 /*****************************************************************************/
//  Description : create 1-key backup task
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL void IKBHandlerMain(uint32 argc, void* argv);



//task sequences table
/*****************************************************************************/
//  Description : InitTaskSeqTable
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL MMIIKB_ERROR_E InitTaskSeqTable(
    MMIIKB_TYPE_T types,
    MMIIKB_TASK_TYPE_E task_type
);

/*****************************************************************************/
//  Description : GetCurTaskItem
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:取不是完成状态的第一个任务
/*****************************************************************************/
LOCAL const MMIIKB_TASK_SEQ_TABLE_T * GetCurTaskItem(void);

/*****************************************************************************/
//  Description : GetCurTaskItem
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:取不是完成状态的第一个任务
/*****************************************************************************/
LOCAL BOOLEAN ResetTaskSeqTable(void);

/*****************************************************************************/
//  Description : SetTaskTableMaxValue
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL MMIIKB_ERROR_E SetTaskTableMaxValue(
    const MMIIKB_TYPE_PROCESS_PARAM_T * process_param_ptr
);

/*****************************************************************************/
//  Description : UpdateTaskTable
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 更新任务情况表
/*****************************************************************************/
LOCAL MMIIKB_ERROR_E UpdateTaskTable(
    MMIIKB_TYPE_E type,
    MMIIKB_TASK_STATE_E task_state,    
    const uint32 *trans_number_ptr,   
    const uint32 *total_number_ptr,
    const uint32 *trans_size_ptr,
    const uint32 *total_size_ptr,
    const uint32 *trans_cnt_ptr,
    const uint32 *total_cnt_ptr,
    const uint32 *crc_ptr
);

/*****************************************************************************/
//  Description : DestoryTaskSeqTable
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL MMIIKB_ERROR_E DestoryTaskSeqTable(void);

/*****************************************************************************/
//  Description : IsTaskOver
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsTaskOver(void);

/*****************************************************************************/
//  Description : SetContentFileName
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
LOCAL MMIIKB_ERROR_E SetContentFileName(
    MMIIKB_TYPE_E type, 
    uint16 *dest_file_name,
    uint16 dest_file_len
);



//backup item
/*****************************************************************************/
//  Description : IKBBackupAllItems
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
LOCAL MMIIKB_ERROR_E IKBBackupAllItems(xSignalHeaderRec *sig_ptr);

/*****************************************************************************/
//  Description : IKBBackupItemUnitDataAsy
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
LOCAL MMIIKB_ERROR_E IKBBackupItemUnitDataAsy(xSignalHeaderRec *sig_ptr);

/*****************************************************************************/
//  Description : SetBackupResParmater
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
LOCAL MMIIKB_ERROR_E SetBackupResParmater(
    MMIIKB_SIG_BACKUP_RES_T *sig_backup_res_ptr,
    MMIIKB_SIG_BACKUP_REQ_T *sig_backup_req_ptr
);

/*****************************************************************************/
//  Description : BackupCircleTask
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 本函数一定要保证只有一个出口
/*****************************************************************************/
LOCAL MMIIKB_ERROR_E BackupCircleTask(
    MMIIKB_SIG_BACKUP_REQ_T * sig_backup_req_ptr
);

/*****************************************************************************/
//  Description : BackupResProcess
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 本函数一定要保证只有一个出口
/*****************************************************************************/
LOCAL MMIIKB_ERROR_E BackupResProcess(
    MMIIKB_ERROR_E param_error_code, 
    BOOLEAN is_free
);

/*****************************************************************************/
//  Description : BackupItemDataSyn
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
LOCAL MMIIKB_ERROR_E BackupItemDataSyn(
    const MMIIKB_TYPE_PROCESS_PARAM_T * process_param_ptr,
    MMIIKB_SIG_BACKUP_REQ_T * backup_req_ptr
);

/*****************************************************************************/
//  Description : BackupItemDataSynByBuf
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
LOCAL MMIIKB_ERROR_E BackupItemDataSynByBuf(
    const MMIIKB_TYPE_PROCESS_PARAM_T * process_param_ptr,
    MMIIKB_SIG_BACKUP_REQ_T * backup_req_ptr
);

/*****************************************************************************/
//  Description : BackupItemDataSynByFile
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
LOCAL MMIIKB_ERROR_E BackupItemDataSynByFile(
    const MMIIKB_TYPE_PROCESS_PARAM_T * process_param_ptr,
    MMIIKB_SIG_BACKUP_REQ_T * backup_req_ptr
);

/*****************************************************************************/
//  Description : BackupItemDataSynByDir
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
LOCAL MMIIKB_ERROR_E BackupItemDataSynByDir(
    const MMIIKB_TYPE_PROCESS_PARAM_T * process_param_ptr,
    MMIIKB_SIG_BACKUP_REQ_T * backup_req_ptr
);

/*****************************************************************************/
//  Description : BackupItemDataAsy
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
LOCAL MMIIKB_ERROR_E BackupItemDataAsy(
    const MMIIKB_TYPE_PROCESS_PARAM_T * process_param_ptr,
    MMIIKB_SIG_BACKUP_REQ_T * backup_req_ptr
);

/*****************************************************************************/
//  Description : BackupHandleItemUnitDataAsy
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
LOCAL MMIIKB_ERROR_E BackupHandleItemUnitDataAsy(
    MMIIKB_BACKUP_ITEM_DATA_RES_T * item_data_res_ptr,
    const MMIIKB_TASK_SEQ_TABLE_T * task_seq_ptr,
    const MMIIKB_TYPE_PROCESS_PARAM_T * process_param_ptr,
    MMIIKB_SIG_BACKUP_REQ_T * backup_req_ptr
);

/*****************************************************************************/
//  Description : BackupHandleUnitDataAsyByBuf
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
LOCAL MMIIKB_ERROR_E BackupHandleUnitDataAsyByBuf(
    MMIIKB_BACKUP_ITEM_DATA_RES_T * item_data_res_ptr,
    const MMIIKB_TASK_SEQ_TABLE_T * task_seq_ptr,
    const MMIIKB_TYPE_PROCESS_PARAM_T * process_param_ptr,
    MMIIKB_SIG_BACKUP_REQ_T * backup_req_ptr
);

/*****************************************************************************/
//  Description : BackupHandleUnitDataAsyByFile
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
LOCAL MMIIKB_ERROR_E BackupHandleUnitDataAsyByFile(
    MMIIKB_BACKUP_ITEM_DATA_RES_T * item_data_res_ptr,
    const MMIIKB_TASK_SEQ_TABLE_T * task_seq_ptr,
    const MMIIKB_TYPE_PROCESS_PARAM_T * process_param_ptr,
    MMIIKB_SIG_BACKUP_REQ_T * backup_req_ptr
);

/*****************************************************************************/
//  Description : BackupHandleUnitDataAsyByDir
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
LOCAL MMIIKB_ERROR_E BackupHandleUnitDataAsyByDir(
    MMIIKB_BACKUP_ITEM_DATA_RES_T * item_data_res_ptr,
    const MMIIKB_TASK_SEQ_TABLE_T * task_seq_ptr,
    const MMIIKB_TYPE_PROCESS_PARAM_T * process_param_ptr,
    MMIIKB_SIG_BACKUP_REQ_T * backup_req_ptr
);



//restore item
/*****************************************************************************/
//  Description : IKBRestoreAllItems
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
LOCAL MMIIKB_ERROR_E IKBRestoreAllItems(xSignalHeaderRec *sig_ptr);

/*****************************************************************************/
//  Description : IKBRestoreItemUnitDataAsy
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
LOCAL MMIIKB_ERROR_E IKBRestoreItemUnitDataAsy(xSignalHeaderRec *sig_ptr);

/*****************************************************************************/
//  Description : SetRestoreResParmater
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
LOCAL MMIIKB_ERROR_E SetRestoreResParmater(
    MMIIKB_SIG_RESTORE_RES_T *sig_restore_res_ptr
);

/*****************************************************************************/
//  Description : RestoreCircleTask
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 本函数一定要保证只有一个出口
/*****************************************************************************/
LOCAL MMIIKB_ERROR_E RestoreCircleTask(
    MMIIKB_SIG_RESTORE_REQ_T * sig_restore_req_ptr
);

/*****************************************************************************/
//  Description : RestoreResProcess
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
LOCAL MMIIKB_ERROR_E RestoreResProcess(
    MMIIKB_ERROR_E param_error_code, 
    BOOLEAN is_free
);

/*****************************************************************************/
//  Description : RestoreItemDataSyn
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
LOCAL MMIIKB_ERROR_E RestoreItemDataSyn(
    const MMIIKB_TYPE_PROCESS_PARAM_T * process_param_ptr,
    MMIIKB_SIG_RESTORE_REQ_T * restore_req_ptr
);

/*****************************************************************************/
//  Description : RestoreItemDataSynByBuf
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
LOCAL MMIIKB_ERROR_E RestoreItemDataSynByBuf(
    const MMIIKB_TYPE_PROCESS_PARAM_T * process_param_ptr,
    MMIIKB_SIG_RESTORE_REQ_T * restore_req_ptr
);

/*****************************************************************************/
//  Description : RestoreItemDataSynByFile
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
LOCAL MMIIKB_ERROR_E RestoreItemDataSynByFile(
    const MMIIKB_TYPE_PROCESS_PARAM_T * process_param_ptr,
    MMIIKB_SIG_RESTORE_REQ_T * restore_req_ptr
);

/*****************************************************************************/
//  Description : RestoreItemDataSynByDir
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
LOCAL MMIIKB_ERROR_E RestoreItemDataSynByDir(
    const MMIIKB_TYPE_PROCESS_PARAM_T * process_param_ptr,
    MMIIKB_SIG_RESTORE_REQ_T * restore_req_ptr
);

/*****************************************************************************/
//  Description : RestoreItemDataAsy
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
LOCAL MMIIKB_ERROR_E RestoreItemDataAsy(
    const MMIIKB_TYPE_PROCESS_PARAM_T * process_param_ptr,
    MMIIKB_SIG_RESTORE_REQ_T * restore_req_ptr
);

/*****************************************************************************/
//  Description : SendUnitDataCircle
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
LOCAL MMIIKB_ERROR_E SendUnitDataCircle(
    uint16 * file_path_ptr,
    const MMIIKB_TASK_SEQ_TABLE_T * task_seq_ptr,
    const MMIIKB_TYPE_PROCESS_PARAM_T * process_param_ptr,
    const MMIIKB_PACK_CONT_HEAD_T *cont_head_ptr
);

/*****************************************************************************/
//  Description : RestoreHandleItemUnitDataAsy
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
LOCAL MMIIKB_ERROR_E RestoreHandleItemUnitDataAsy(
    MMIIKB_RESTORE_ITEM_DATA_RES_T * item_data_res_ptr,
    const MMIIKB_TASK_SEQ_TABLE_T * task_seq_ptr,
    const MMIIKB_TYPE_PROCESS_PARAM_T * process_param_ptr,
    MMIIKB_SIG_RESTORE_REQ_T * restore_req_ptr
);

/*****************************************************************************/
//  Description : RestoreHandleUnitDataAsyByBuf
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
LOCAL MMIIKB_ERROR_E RestoreHandleUnitDataAsyByBuf(
    MMIIKB_RESTORE_ITEM_DATA_RES_T * item_data_res_ptr,
    const MMIIKB_TASK_SEQ_TABLE_T * task_seq_ptr,
    const MMIIKB_TYPE_PROCESS_PARAM_T * process_param_ptr,
    MMIIKB_SIG_RESTORE_REQ_T * restore_req_ptr
);

/*****************************************************************************/
//  Description : RestoreHandleUnitDataAsyByFile
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
LOCAL MMIIKB_ERROR_E RestoreHandleUnitDataAsyByFile(
    MMIIKB_RESTORE_ITEM_DATA_RES_T * item_data_res_ptr,
    const MMIIKB_TASK_SEQ_TABLE_T * task_seq_ptr,
    const MMIIKB_TYPE_PROCESS_PARAM_T * process_param_ptr,
    MMIIKB_SIG_RESTORE_REQ_T * restore_req_ptr
);

/*****************************************************************************/
//  Description : RestoreHandleUnitDataAsyByDir
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
LOCAL MMIIKB_ERROR_E RestoreHandleUnitDataAsyByDir(
    MMIIKB_RESTORE_ITEM_DATA_RES_T * item_data_res_ptr,
    const MMIIKB_TASK_SEQ_TABLE_T * task_seq_ptr,
    const MMIIKB_TYPE_PROCESS_PARAM_T * process_param_ptr,
    MMIIKB_SIG_RESTORE_REQ_T * restore_req_ptr
);



//cacel process
/*****************************************************************************/
//  Description : IKBCancelAction
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
LOCAL MMIIKB_ERROR_E IKBCancelAction(xSignalHeaderRec *sig_ptr);



//time-out protection mechanism
/*****************************************************************************/
//  Description : CreateProtectionTimer
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 创建超时保护定时器
/*****************************************************************************/
LOCAL void CreateProtectionTimer(void);

/******************************************************************************/
// Description: StopProtectionTimer
// Global resource dependence: NONE
// Author: haiwu.chen
// Note: 停止超时保护定时器
/******************************************************************************/
LOCAL void StopProtectionTimer(void);

/******************************************************************************/
// Description: ProcessProtectionTimerCB
// Global resource dependence: NONE
// Author: haiwu.chen
// Note: 超时保护机制
/******************************************************************************/
LOCAL void ProcessProtectionTimerCB(uint32 param);



//file process
/*****************************************************************************/
//  Description : GetContentFilePath
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
LOCAL MMIIKB_ERROR_E GetContentFilePath(
    MMIIKB_TYPE_E type, 
    uint16 *pack_dir_ptr,
    uint16 *file_path_ptr,              //out
    uint16 *file_path_len_ptr           //out
);

/*****************************************************************************/
//  Description : GetUnitData
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
LOCAL MMIIKB_ERROR_E ReceiveUnitData(
    const MMIIKB_TYPE_PROCESS_PARAM_T * process_param_ptr,
    MMIIKB_BACKUP_CALLBACK_PARAM_IN_T *param_in_ptr,
    MMIIKB_BACKUP_CALLBACK_PARAM_OUT_T *param_out_ptr
);

/*****************************************************************************/
//  Description : WriteUnitData
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
LOCAL MMIIKB_ERROR_E WriteUnitData(
    uint16 *file_path_ptr,
    uint16 file_path_len,    
    MMIIKB_PACK_CONT_UNIT_T *cont_unit_ptr
);

/*****************************************************************************/
//  Description : WriteContentHead
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
LOCAL MMIIKB_ERROR_E WriteContentHead(
    uint16 *file_path_ptr,
    uint16 file_path_len,
    MMIIKB_PACK_CONT_HEAD_T *cont_head_ptr
);

/*****************************************************************************/
//  Description : SendUnitData
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
LOCAL MMIIKB_ERROR_E SendUnitData(
    const MMIIKB_TYPE_PROCESS_PARAM_T * process_param_ptr,
    MMIIKB_RESTORE_CALLBACK_PARAM_IN_T *param_in_ptr,
    MMIIKB_RESTORE_CALLBACK_PARAM_OUT_T *param_out_ptr
);

/*****************************************************************************/
//  Description : ReadUnitData
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
LOCAL MMIIKB_ERROR_E ReadUnitData(
    MMIFILE_HANDLE file_handle,
    uint32 offset,
    MMIIKB_PACK_CONT_UNIT_T *cont_unit_ptr
);

/*****************************************************************************/
//  Description : ReadContentHead
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
LOCAL MMIIKB_ERROR_E ReadContentHead(
    uint16 *file_path_ptr,
    uint16 file_path_len,
    MMIIKB_PACK_CONT_HEAD_T *cont_head_ptr
);


/**--------------------------------------------------------------------------*
 **                         EXTERNAL FUNCTION DEFINITION                     *
 **--------------------------------------------------------------------------*/
 
/*****************************************************************************/
//  Description : init ikb agent
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC void IKeyBackup_InitIKBAgent(void)
{
    if (PNULL == s_ikb_cancel_mutext)
    {
        s_ikb_cancel_mutext = SCI_CreateMutex(
            "ikeybackup_mutex", SCI_NO_INHERIT);
    }
}

/*****************************************************************************/
//  Description : create 1-key backup task
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC void IKeyBackup_StartIKBHandlerTask(void)
{
    if(SCI_INVALID_BLOCK_ID != s_ikeybackup_task_id)
    {
        return;
    }
    
    s_ikeybackup_task_id = SCI_CreateThread(
        "T_P_IKEYBACKUP",
        "Q_P_IKEYBACKUP",
        IKBHandlerMain,
        0,
        PNULL,
        P_IKB_STACK_SIZE,
        P_IKB_QUEUE_NUM,
        P_IKB_TASK_PRIORITY,
        SCI_PREEMPT,
        SCI_AUTO_START);
    
    //SCI_TRACE_LOW:"[MMIIKB] IKeyBackup_StartIKBHandlerTask task_id=%04X."
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_TASK_590_112_2_18_2_24_25_18,(uint8*)"d", s_ikeybackup_task_id);
    
    return;
}

/*****************************************************************************/
//  Description : IKeyBackup_GetIKBTaskId
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC BLOCK_ID IKeyBackup_GetIKBTaskId(void)
{
    return s_ikeybackup_task_id;
}

/*****************************************************************************/
//  Description : IKeyBackup_SuspendTask
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC uint32 IKeyBackup_SuspendTask(void)
{
    //return SCI_SuspendThread(s_ikeybackup_task_id);
  
    //lock
    return SCI_GetMutex(s_ikb_cancel_mutext, SCI_WAIT_FOREVER);  
}

/*****************************************************************************/
//  Description : IKeyBackup_ResumeTask
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC uint32 IKeyBackup_ResumeTask(void)
{
    //return SCI_ResumeThread(s_ikeybackup_task_id);
    
    //unlock
    return SCI_PutMutex(s_ikb_cancel_mutext);
}
 
/*****************************************************************************/
//  Description : 1-key backup is do backup thing now
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:备份或者还原任务正在忙碌中
/*****************************************************************************/
PUBLIC BOOLEAN IKeyBackup_IsBusy()
{
    return (IKeyBackup_IsBackupBusy() || IKeyBackup_IsRestoreBusy());
}
 
/*****************************************************************************/
//  Description : 1-key backup is do backup thing now
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:是否正在备份中
/*****************************************************************************/
PUBLIC BOOLEAN IKeyBackup_IsBackupBusy()
{
    return (PNULL != s_backup_req_ptr);
}
 
/*****************************************************************************/
//  Description : 1-key backup is do backup thing now
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:是否正在还原中
/*****************************************************************************/
PUBLIC BOOLEAN IKeyBackup_IsRestoreBusy()
{
    return (PNULL != s_restore_req_ptr);
}

/*****************************************************************************/
//  Description : IKeyBackup_ActionSetCancel
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC void IKeyBackup_ActionSetCancel()
{
    //lock
    SCI_GetMutex(s_ikb_cancel_mutext, SCI_WAIT_FOREVER);
    s_ikb_action_cancel = TRUE;
    
    //unlock
    SCI_PutMutex(s_ikb_cancel_mutext);
}

/*****************************************************************************/
//  Description : IKeyBackup_ActionSetOk
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC void IKeyBackup_ActionSetOk()
{
    //lock
    SCI_GetMutex(s_ikb_cancel_mutext, SCI_WAIT_FOREVER);
    s_ikb_action_cancel = FALSE;
    
    //unlock
    SCI_PutMutex(s_ikb_cancel_mutext);
}

/*****************************************************************************/
//  Description : IKeyBackup_GetCancelState
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN IKeyBackup_GetCancelState()
{
    return s_ikb_action_cancel;
}


//operate package information
/*****************************************************************************/
//  Description : IKeyBackup_UpdataPackInfo
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
PUBLIC MMIIKB_ERROR_E IKeyBackup_UpdataPackInfo(
    MMIIKB_PACK_INFO_T *pack_info_ptr
)
{
    return MMIIKB_STATUS_INVALID_PARAM;
}

/*****************************************************************************/
//  Description : IKeyBackup_GetContentName
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC const uint16* IKeyBackup_GetContentName(MMIIKB_TYPE_E type)
{
    uint16 i = 0;
    const uint16 * cont_name_ptr = PNULL;

    for (i = 0; i < ARR_SIZE(s_type_setting); i++)
    {
        if (type == s_type_setting[i].type)
        {
            cont_name_ptr = s_type_setting[i].type_file_name;
            break;
        }
    }
    
    return cont_name_ptr;
}

/*****************************************************************************/
//  Description : IKeyBackup_GetTotalSizeByType
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC MMIIKB_ERROR_E IKeyBackup_GetTotalSizeByType(
    MMIIKB_TYPE_E type,
    MMIIKB_GET_TOTAL_OUT_T *param_out_ptr
)
{
    MMIIKB_ERROR_E error_code = MMIIKB_STATUS_INVALID_PARAM;
    const MMIIKB_TYPE_PROCESS_PARAM_T *process_param_ptr = PNULL;

    if (MMIIKB_TYPE_MAX <= type
        || PNULL == param_out_ptr)
    {
        //SCI_TRACE_LOW:"[MMIIKB] IKeyBackup_GetTotalSizeByType param_ptr is null, type=%d."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_TASK_718_112_2_18_2_24_26_19,(uint8*)"d", type);
            
        return error_code;
    }

    process_param_ptr = MMIIKB_GetTypeProcessParamPtr(type);

    if (PNULL == process_param_ptr)
    {
        //SCI_TRACE_LOW:"[MMIIKB] IKeyBackup_GetTotalSizeByType param_ptr is null, type=%d."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_TASK_728_112_2_18_2_24_26_20,(uint8*)"d", type);
        
        return error_code;
    }

    if (PNULL == process_param_ptr->get_total_call_back)
    {
        //SCI_TRACE_LOW:"[MMIIKB] IKeyBackup_GetTotalSizeByType call_back is null, type=%d."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_TASK_736_112_2_18_2_24_26_21,(uint8*)"d", type);
                    
        error_code = MMIIKB_STATUS_APP_REG_ERROR;
        return error_code;
    }

    process_param_ptr->get_total_call_back(param_out_ptr);

    error_code = param_out_ptr->error_code;
    
    //SCI_TRACE_LOW:"[MMIIKB] IKeyBackup_GetTotalSizeByType error_code is %d, type=%d, total_type=%d, total_size=%d."
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_TASK_748_112_2_18_2_24_26_22,(uint8*)"dddd", error_code, type, param_out_ptr->total_type, param_out_ptr->total);
    
    return error_code;
}

/*****************************************************************************/
//  Description : 1-key backup all items request by app
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: app send MSG_IKB_BACKUP_DATA_REQ request to 1-key
/*****************************************************************************/
PUBLIC MMIIKB_ERROR_E IKeyBackup_BackupAllItemReq(
    MMIIKB_BACKUP_REQ_T *backup_req_ptr
)
{
    MMIIKB_ERROR_E error_code = MMIIKB_STATUS_INVALID_PARAM;
    MMIIKB_SIG_BACKUP_REQ_T *sig_backup_req_ptr = PNULL;
    uint16 sig_len = 0;
    
    if(PNULL == backup_req_ptr)
    {
        //SCI_TRACE_LOW:"[MMIIKB] IKeyBackup_BackupAllItemReq param is null."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_TASK_767_112_2_18_2_24_26_23,(uint8*)"");
        
        return error_code;
    }

    sig_len = sizeof(MMIIKB_SIG_BACKUP_REQ_T);
    sig_backup_req_ptr = (MMIIKB_SIG_BACKUP_REQ_T*)SCI_ALLOCA(sig_len);
    if (PNULL == sig_backup_req_ptr)
    {
        //SCI_TRACE_LOW:"[MMIIKB] IKeyBackup_BackupAllItemReq memory is out."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_TASK_776_112_2_18_2_24_26_24,(uint8*)"");
        
        error_code =  MMIIKB_STATUS_NO_MEMORY;
        return error_code;
    }
    SCI_MEMSET(sig_backup_req_ptr, 0x00, sig_len);

    sig_backup_req_ptr->SignalCode = MSG_IKB_BACKUP_DATA_REQ;
    sig_backup_req_ptr->Sender = SCI_IdentifyThread();
    sig_backup_req_ptr->SignalSize = sig_len;

    SCI_MEMCPY(&sig_backup_req_ptr->backup_req, backup_req_ptr, 
        sizeof(MMIIKB_BACKUP_REQ_T));
    
    //SCI_TRACE_LOW:"[MMIIKB] IKeyBackup_BackupAllItemReq request success."
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_TASK_790_112_2_18_2_24_26_25,(uint8*)"");
    
    IKEYBACKUP_SEND_SIGNAL(sig_backup_req_ptr, IKeyBackup_GetIKBTaskId());

    error_code = MMIIKB_STATUS_SUCCESS;
    return error_code;
}

/*****************************************************************************/
//  Description : IKeyBackup_SendBackupItemDataRes
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
PUBLIC MMIIKB_ERROR_E IKeyBackup_SendBackupItemDataRes(
    MMIIKB_BACKUP_ITEM_DATA_RES_T * backup_item_data_res
)
{
    MMIIKB_ERROR_E error_code = MMIIKB_STATUS_INVALID_PARAM;
    MMIIKB_SIG_BACKUP_ITEM_DATA_RES_T *sig_item_data_ptr = PNULL;
    uint16 sig_len = 0;
    
    if(PNULL == backup_item_data_res)
    {
        //SCI_TRACE_LOW:"[MMIIKB] IKeyBackup_SendBackupItemDataRes param is null."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_TASK_812_112_2_18_2_24_26_26,(uint8*)"");
        
        return error_code;
    }

    sig_len = sizeof(MMIIKB_SIG_BACKUP_ITEM_DATA_RES_T);
    sig_item_data_ptr = (MMIIKB_SIG_BACKUP_ITEM_DATA_RES_T*)SCI_ALLOCA(
        sizeof(MMIIKB_SIG_BACKUP_ITEM_DATA_RES_T));
    if (PNULL == sig_item_data_ptr)
    {
        //SCI_TRACE_LOW:"[MMIIKB] IKeyBackup_SendBackupItemDataRes sig_item_data_ptr memory is out."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_TASK_823_112_2_18_2_24_26_27,(uint8*)"");
        
        error_code =  MMIIKB_STATUS_NO_MEMORY;
        return error_code;
    }
    SCI_MEMSET(sig_item_data_ptr, 0x00, sig_len);

    sig_item_data_ptr->SignalCode = MSG_IKB_BACKUP_ITEM_DATA_RES;
    sig_item_data_ptr->Sender = SCI_IdentifyThread();
    sig_item_data_ptr->SignalSize = sig_len;

    sig_item_data_ptr->backup_item_data_res.type = 
        backup_item_data_res->type;
    sig_item_data_ptr->backup_item_data_res.op_type = 
        backup_item_data_res->op_type;
    sig_item_data_ptr->backup_item_data_res.begin = 
        backup_item_data_res->begin;
    sig_item_data_ptr->backup_item_data_res.number =
        backup_item_data_res->number;    
    sig_item_data_ptr->backup_item_data_res.is_need_free = 
        backup_item_data_res->is_need_free;
    sig_item_data_ptr->backup_item_data_res.is_finished = 
        backup_item_data_res->is_finished;
    sig_item_data_ptr->backup_item_data_res.error_code = 
        backup_item_data_res->error_code;

    if (MMIIKB_OPERATE_TYPE_BUF == 
        sig_item_data_ptr->backup_item_data_res.op_type)
    {
        sig_item_data_ptr->backup_item_data_res.len = 
            backup_item_data_res->len;
        
        sig_item_data_ptr->backup_item_data_res.buf_ptr = SCI_ALLOCA(
            sig_item_data_ptr->backup_item_data_res.len);
        if (PNULL == sig_item_data_ptr->backup_item_data_res.buf_ptr)
        {
            //SCI_TRACE_LOW:"[MMIIKB] IKeyBackup_SendBackupItemDataRes sig_item_data_ptr->buf memory is out."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_TASK_860_112_2_18_2_24_26_28,(uint8*)"");
            
            SCI_FREE(sig_item_data_ptr);
            error_code =  MMIIKB_STATUS_NO_MEMORY;
            return error_code;
        }
        SCI_MEMSET(sig_item_data_ptr->backup_item_data_res.buf_ptr, 0x00,
            sig_item_data_ptr->backup_item_data_res.len);
        SCI_MEMCPY(sig_item_data_ptr->backup_item_data_res.buf_ptr, 
            backup_item_data_res->buf_ptr,
            sig_item_data_ptr->backup_item_data_res.len);
        if (backup_item_data_res->is_need_free
            && PNULL != backup_item_data_res->buf_ptr)
        {
            SCI_FREE(backup_item_data_res->buf_ptr);
        }
    }
    else if (MMIIKB_OPERATE_TYPE_FILE == 
        sig_item_data_ptr->backup_item_data_res.op_type
        || MMIIKB_OPERATE_TYPE_DIR == 
        sig_item_data_ptr->backup_item_data_res.op_type)
    {
        sig_item_data_ptr->backup_item_data_res.path_len = 
            backup_item_data_res->path_len;
        
        sig_item_data_ptr->backup_item_data_res.path_ptr = SCI_ALLOCA(
            sig_item_data_ptr->backup_item_data_res.path_len);
        if (PNULL == sig_item_data_ptr->backup_item_data_res.path_ptr)
        {
            //SCI_TRACE_LOW:"[MMIIKB] IKeyBackup_SendBackupItemDataRes sig_item_data_ptr->path_ptr memory is out."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_TASK_890_112_2_18_2_24_26_29,(uint8*)"");
                
            SCI_FREE(sig_item_data_ptr);
            error_code =  MMIIKB_STATUS_NO_MEMORY;
            return error_code;
        }
        SCI_MEMSET(sig_item_data_ptr->backup_item_data_res.path_ptr, 0x00,
            sig_item_data_ptr->backup_item_data_res.path_len);
        SCI_MEMCPY(sig_item_data_ptr->backup_item_data_res.path_ptr, 
            backup_item_data_res->path_ptr,
            sig_item_data_ptr->backup_item_data_res.path_len);
    }
    
    //SCI_TRACE_LOW:"[MMIIKB] IKeyBackup_SendBackupItemDataRes request success."
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_TASK_903_112_2_18_2_24_26_30,(uint8*)"");
    
    IKEYBACKUP_SEND_SIGNAL(sig_item_data_ptr, IKeyBackup_GetIKBTaskId());

    error_code = MMIIKB_STATUS_SUCCESS;
    return error_code;
}

/*****************************************************************************/
//  Description : 1-key restore all items request by app
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: app send MSG_IKB_RESTORE_DATA_REQ request to 1-key
/*****************************************************************************/
PUBLIC MMIIKB_ERROR_E IKeyBackup_RestoreAllItemReq(
    MMIIKB_RESTORE_REQ_T *restore_req_ptr
)
{
    MMIIKB_ERROR_E error_code = MMIIKB_STATUS_INVALID_PARAM;
    MMIIKB_SIG_RESTORE_REQ_T *sig_restore_req_ptr = PNULL;
    uint16 sig_len = 0;
    
    if(PNULL == restore_req_ptr)
    {
        //SCI_TRACE_LOW:"[MMIIKB] IKeyBackup_RestoreAllItemReq param is null."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_TASK_925_112_2_18_2_24_26_31,(uint8*)"");
        
        return error_code;
    }

    sig_len = sizeof(MMIIKB_SIG_RESTORE_REQ_T);
    sig_restore_req_ptr = (MMIIKB_SIG_RESTORE_REQ_T*)SCI_ALLOCA(sig_len);
    if (PNULL == sig_restore_req_ptr)
    {
        error_code =  MMIIKB_STATUS_NO_MEMORY;
        return error_code;
    }
    SCI_MEMSET(sig_restore_req_ptr, 0x00, sig_len);

    sig_restore_req_ptr->SignalCode = MSG_IKB_RESTORE_DATA_REQ;
    sig_restore_req_ptr->Sender = SCI_IdentifyThread();
    sig_restore_req_ptr->SignalSize = sig_len;

    sig_restore_req_ptr->restore_req.types = restore_req_ptr->types;
    sig_restore_req_ptr->restore_req.cfg_ptr = restore_req_ptr->cfg_ptr;
    
    sig_restore_req_ptr->restore_req.pack_info_ptr = 
        (MMIIKB_PACK_INFO_T*)SCI_ALLOCA(sizeof(MMIIKB_PACK_INFO_T));
    if (PNULL == sig_restore_req_ptr->restore_req.pack_info_ptr)
    {
        //SCI_TRACE_LOW:"[MMIIKB] IKeyBackup_RestoreAllItemReq memory is out."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_TASK_950_112_2_18_2_24_26_32,(uint8*)"");
        
        error_code =  MMIIKB_STATUS_NO_MEMORY;
        SCI_FREE(sig_restore_req_ptr);
        return error_code;
    }
    SCI_MEMSET(sig_restore_req_ptr->restore_req.pack_info_ptr, 0x00, 
        sizeof(MMIIKB_PACK_INFO_T));
    
    SCI_MEMCPY(sig_restore_req_ptr->restore_req.pack_info_ptr, 
        restore_req_ptr->pack_info_ptr, sizeof(MMIIKB_PACK_INFO_T));
    
    //SCI_TRACE_LOW:"[MMIIKB] IKeyBackup_RestoreAllItemReq request success."
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_TASK_962_112_2_18_2_24_26_33,(uint8*)"");
    
    IKEYBACKUP_SEND_SIGNAL(sig_restore_req_ptr, IKeyBackup_GetIKBTaskId());

    error_code = MMIIKB_STATUS_SUCCESS;
    return error_code;
}

/*****************************************************************************/
//  Description : IKeyBackup_SendRestoreItemDataRes
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
PUBLIC MMIIKB_ERROR_E IKeyBackup_SendRestoreItemDataRes(
    MMIIKB_RESTORE_ITEM_DATA_RES_T * restore_item_data_res
)
{
    MMIIKB_ERROR_E error_code = MMIIKB_STATUS_INVALID_PARAM;
    MMIIKB_SIG_RESTORE_ITEM_DATA_RES_T *sig_item_data_ptr = PNULL;
    uint16 sig_len = 0;
    
    if(PNULL == restore_item_data_res)
    {
        //SCI_TRACE_LOW:"[MMIIKB] IKeyBackup_SendRestoreItemDataRes param is null."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_TASK_984_112_2_18_2_24_26_34,(uint8*)"");
        
        return error_code;
    }

    sig_len = sizeof(MMIIKB_SIG_RESTORE_ITEM_DATA_RES_T);
    sig_item_data_ptr = (MMIIKB_SIG_RESTORE_ITEM_DATA_RES_T*)SCI_ALLOCA(
        sizeof(MMIIKB_SIG_RESTORE_ITEM_DATA_RES_T));
    if (PNULL == sig_item_data_ptr)
    {
        //SCI_TRACE_LOW:"[MMIIKB] IKeyBackup_SendRestoreItemDataRes sig_item_data_ptr memory is out."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_TASK_995_112_2_18_2_24_26_35,(uint8*)"");
        
        error_code =  MMIIKB_STATUS_NO_MEMORY;
        return error_code;
    }
    SCI_MEMSET(sig_item_data_ptr, 0x00, sig_len);

    sig_item_data_ptr->SignalCode = MSG_IKB_RESTORE_ITEM_DATA_RES;
    sig_item_data_ptr->Sender = SCI_IdentifyThread();
    sig_item_data_ptr->SignalSize = sig_len;

    sig_item_data_ptr->restore_item_data_res.type = 
        restore_item_data_res->type;
    sig_item_data_ptr->restore_item_data_res.op_type = 
        restore_item_data_res->op_type;
    
    sig_item_data_ptr->restore_item_data_res.suc_cnt = 
        restore_item_data_res->suc_cnt;
    sig_item_data_ptr->restore_item_data_res.err_cnt =
        restore_item_data_res->err_cnt;
    
    sig_item_data_ptr->restore_item_data_res.error_code = 
        restore_item_data_res->error_code;
    
    //SCI_TRACE_LOW:"[MMIIKB] IKeyBackup_SendRestoreItemDataRes request success."
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_TASK_1019_112_2_18_2_24_26_36,(uint8*)"");
    
    IKEYBACKUP_SEND_SIGNAL(sig_item_data_ptr, IKeyBackup_GetIKBTaskId());

    error_code = MMIIKB_STATUS_SUCCESS;
    return error_code;
}

/*****************************************************************************/
//  Description : IKeyBackup_SendProgressInfo
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
PUBLIC MMIIKB_ERROR_E IKeyBackup_SendProgressInfo(
    const MMIIKB_TASK_SEQ_TABLE_T *seq_task_ptr
)
{
    MMIIKB_ERROR_E error_code = MMIIKB_STATUS_INVALID_PARAM;
    MMIIKB_SIG_PROGRESS_RES_T *sig_progress_ptr = PNULL;
    uint16 sig_len = sizeof(MMIIKB_SIG_PROGRESS_RES_T);
    MMIIKB_TASK_SEQ_TABLE_T *print_seq_ptr = PNULL;

    if (PNULL == seq_task_ptr)
    {
        //SCI_TRACE_LOW:"[MMIIKB] IKeyBackup_SendProgressInfo param is null."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_TASK_1042_112_2_18_2_24_26_37,(uint8*)"");
        
        return error_code;
    }

    sig_progress_ptr = (MMIIKB_SIG_PROGRESS_RES_T*)SCI_ALLOCA(sig_len);
    if (PNULL == sig_progress_ptr)
    {
        //SCI_TRACE_LOW:"[MMIIKB] IKeyBackup_SendProgressInfo meory is out."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_TASK_1050_112_2_18_2_24_26_38,(uint8*)"");
        
        error_code =  MMIIKB_STATUS_NO_MEMORY;
        return error_code;
    }
    SCI_MEMSET(sig_progress_ptr, 0x00, sig_len);

    sig_progress_ptr->SignalCode = MSG_IKB_PROGRESS_VALUE_RES;
    sig_progress_ptr->Sender = SCI_IdentifyThread();
    sig_progress_ptr->SignalSize = sig_len;

    SCI_MEMCPY(&sig_progress_ptr->seq_task_item, seq_task_ptr, 
        sizeof(MMIIKB_TASK_SEQ_TABLE_T));

    print_seq_ptr = &sig_progress_ptr->seq_task_item;
    //SCI_TRACE_LOW:"[MMIIKB] IKeyBackup_SendProgressInfo type=%d, task_type=%d, state=%d, cur_num=%d, totol_num=%d, cur_size=%d, total_size=%d, cur_cnt=%d total_cnt=%d."
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_TASK_1072_112_2_18_2_24_26_39,(uint8*)"ddddddddd", print_seq_ptr->type, print_seq_ptr->task_type, print_seq_ptr->task_state,print_seq_ptr->trans_number, print_seq_ptr->total_number, print_seq_ptr->trans_size, print_seq_ptr->total_size, print_seq_ptr->trans_cnt, print_seq_ptr->total_cnt);
    
    IKEYBACKUP_SEND_SIGNAL(sig_progress_ptr, P_APP);

    //app flush screen, maybe need time, also scm lcd flush need prepare
    SCI_Sleep(3);
    
    error_code = MMIIKB_STATUS_SUCCESS;
    return error_code;
}

/*****************************************************************************/
//  Description : 1-key send cancel action (backup or restore) req by app
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: app send MSG_IKB_CANCEL_ACTION_REQ request to 1-key
/*****************************************************************************/
PUBLIC MMIIKB_ERROR_E IKeyBackup_CancelActionReq(
    MMIIKB_TASK_TYPE_E task_type
)
{
    MMIIKB_ERROR_E error_code = MMIIKB_STATUS_INVALID_PARAM;
    MMIIKB_SIG_CANCEL_REQ_T *sig_cancel_req_ptr = PNULL;
    uint16 sig_len = 0;
    
    if(MMIIKB_TASK_TYPE_NONE >= task_type
        || MMIIKB_TASK_TYPE_MAX <= task_type)
    {
        //SCI_TRACE_LOW:"[MMIIKB] IKeyBackup_CancelActionReq param is 0, and task_type=%d."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_TASK_1096_112_2_18_2_24_26_40,(uint8*)"d", task_type);
        return error_code;
    }

    IKeyBackup_ActionSetCancel();

    sig_len = sizeof(MMIIKB_SIG_CANCEL_REQ_T);
    sig_cancel_req_ptr = (MMIIKB_SIG_CANCEL_REQ_T*)SCI_ALLOCA(sig_len);
    if (PNULL == sig_cancel_req_ptr)
    {
        //SCI_TRACE_LOW:"[MMIIKB] IKeyBackup_CancelActionReq memory is out."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_TASK_1106_112_2_18_2_24_26_41,(uint8*)"");
        
        error_code =  MMIIKB_STATUS_NO_MEMORY;
        return error_code;
    }
    SCI_MEMSET(sig_cancel_req_ptr, 0x00, sig_len);

    sig_cancel_req_ptr->SignalCode = MSG_IKB_CANCEL_ACTION_REQ;
    sig_cancel_req_ptr->Sender = SCI_IdentifyThread();
    sig_cancel_req_ptr->SignalSize = sig_len;

    sig_cancel_req_ptr->task_type = task_type;
    
    //SCI_TRACE_LOW:"[MMIIKB] IKeyBackup_CancelActionReq request success."
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_TASK_1119_112_2_18_2_24_26_42,(uint8*)"");
    
    IKEYBACKUP_SEND_SIGNAL(sig_cancel_req_ptr, IKeyBackup_GetIKBTaskId());

    error_code = MMIIKB_STATUS_SUCCESS;
    return error_code;
}



//crc operate
//crc 16 "Crc16.c (ms_code\ms_ref\source\base\src)"
/*****************************************************************************/
//  Description : IKeyBackup_IsCrc
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN IKeyBackup_IsCrc(void)
{    
    return MMIAPIIKB_GetConfigInfo()->is_crc_check;
}

/*****************************************************************************/
//  Description : IKeyBackup_CalCrcItemData
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
PUBLIC MMIIKB_ERROR_E IKeyBackup_CalCrcItemData(
    uint16 *file_path_ptr,
    uint32 *crc_ptr
)
{
    MMIIKB_ERROR_E error_code = MMIIKB_STATUS_INVALID_PARAM;
    MMIFILE_HANDLE file_handle = SFS_INVALID_HANDLE;
    MMIFILE_ERROR_E file_error = SFS_ERROR_NONE;
    void *buf_ptr = PNULL;
    uint32 bytes_to_read = 0;
    uint32 read_len = 0;
    uint32 file_size = 0;
    uint32 offset = 0;
    uint16 src_crc = 0;    

    if (PNULL == file_path_ptr
        || PNULL == crc_ptr)
    {
        //SCI_TRACE_LOW:"[MMIIKB] IKeyBackup_CalCrcItemData param is null."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_TASK_1162_112_2_18_2_24_26_43,(uint8*)"");
        return error_code;
    }
    
    file_handle = MMIAPIFMM_CreateFile(file_path_ptr, 
        SFS_MODE_READ|SFS_MODE_OPEN_EXISTING, 0, 0);

    if (SFS_INVALID_HANDLE == file_handle)
    {
        error_code = MMIIKB_STATUS_CREATE_FILE_FAILED;
    }
    else
    {        
        //read content head info
        file_size = MMIAPIFMM_GetFileSize(file_handle);
        
        if (0 == file_size)
        {
            error_code =  MMIIKB_STATUS_FILE_EOF;
        }
        else
        {
            //第一段需要读取多大
            buf_ptr = SCI_ALLOCA(IKEYBACKUP_CRC_SEG_LEN);
            if (PNULL == buf_ptr)
            {
                //无内存了，跳出
                error_code =  MMIIKB_STATUS_NO_MEMORY;
                MMIAPIFMM_CloseFile(file_handle);
                return error_code;
            }
                
            offset = 0;
            if (offset + IKEYBACKUP_CRC_SEG_LEN >= file_size)
            {
                bytes_to_read = file_size - offset;
            }
            else
            {
                bytes_to_read = IKEYBACKUP_CRC_SEG_LEN;
            }
            
            while (1)/*lint !e716 */
            {
                SCI_MEMSET(buf_ptr, 0x00, IKEYBACKUP_CRC_SEG_LEN);
                file_error = MMIAPIFMM_ReadFile(file_handle, buf_ptr,
                    bytes_to_read, &read_len, PNULL);
                
                if (SFS_ERROR_NONE == file_error && bytes_to_read == read_len)
                {
                    src_crc = mmi_crc16(src_crc, buf_ptr, read_len);
                    error_code = MMIIKB_STATUS_SUCCESS;
                }
                else
                {
                    error_code = MMIIKB_STATUS_FILE_ERROR;
                }

                if (MMIIKB_STATUS_SUCCESS == error_code)
                {
                    if (offset + bytes_to_read >= file_size)
                    {
                        //读完了，跳出
                        *crc_ptr = src_crc;
                        break;
                    }
                    else
                    {
                        //没读完，算出下一段需要读出的段大小
                        offset += bytes_to_read;
                        if (offset + IKEYBACKUP_CRC_SEG_LEN >= file_size)
                        {
                            bytes_to_read = file_size - offset;
                        }
                        else
                        {
                            bytes_to_read = IKEYBACKUP_CRC_SEG_LEN;
                        }
                    }
                }
                else
                {
                    //出错了，跳出
                    break;
                }
            }
                    
            SCI_FREE(buf_ptr);
        }
    }

    MMIAPIFMM_CloseFile(file_handle);
    
    //SCI_TRACE_LOW:"[MMIIKB] ReadContentHead error_code=%d."
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_TASK_1255_112_2_18_2_24_27_44,(uint8*)"d", error_code);
    
    return error_code;
}

/*****************************************************************************/
//  Description : IKeyBackup_IsCrcRight
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
PUBLIC MMIIKB_ERROR_E IKeyBackup_IsCrcRight(
    uint16 *file_path,
    uint32 crc
)
{
    MMIIKB_ERROR_E error_code = MMIIKB_STATUS_INVALID_PARAM;
    uint32 cal_crc = 0;
    
    if (PNULL == file_path)
    {
        //SCI_TRACE_LOW:"[MMIIKB] IKeyBackup_IsCrcRight param is null."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_TASK_1274_112_2_18_2_24_27_45,(uint8*)"");
        return error_code;
    }
    
    if (IKeyBackup_IsCrc())
    {
        error_code = IKeyBackup_CalCrcItemData(file_path, &cal_crc);
        if (MMIIKB_STATUS_SUCCESS == error_code
            && crc == cal_crc)
        {
            error_code = MMIIKB_STATUS_SUCCESS;
        }
        else
        {
            error_code = MMIIKB_STATUS_CRC_ERROR;
        }
    }
    else
    {
        error_code = MMIIKB_STATUS_SUCCESS;
    }
    
    //SCI_TRACE_LOW:"[MMIIKB] IKeyBackup_IsCrcRight error_code=%d."
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_TASK_1296_112_2_18_2_24_27_46,(uint8*)"d", error_code);

    return error_code;
}



//enc and dec buf
/*****************************************************************************/
//  Description : IKeyBackup_IsEnc
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN IKeyBackup_IsEnc(MMIIKB_TYPE_E type)
{
    BOOLEAN is_ret = FALSE;

    if (0 == MMIAPIIKB_GetConfigInfo()->types)
    {
        is_ret = FALSE;
    }
    else
    {
        is_ret = (0x0 != (MMIAPIIKB_GetConfigInfo()->types & (0x01 << type)));
    }
    
    return is_ret;
}

/*****************************************************************************/
//  Description : IKeyBackup_EncBuf
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
PUBLIC MMIIKB_ERROR_E IKeyBackup_EncBuf(
    uint8 * buf_ptr,
    uint32 buf_len
)
{
    MMIIKB_ERROR_E error_code = MMIIKB_STATUS_SUCCESS;
    
#if 0
    uint8 *buf_src_ptr = buf_ptr;
    uint32 test_tt = 90000000;
    uint16 src_buf_len = buf_len;
#endif

    if (PNULL == buf_ptr
        || 0 == buf_len)
    {
        error_code = MMIIKB_STATUS_INVALID_PARAM;
        //SCI_TRACE_LOW:"[MMIIKB] IKeyBackup_EncBuf param is null."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_TASK_1345_112_2_18_2_24_27_47,(uint8*)"");
        return error_code;
    }
    
    while (buf_len--)
    {
        if (IKeyBackup_GetCancelState())
        {
            //SCI_TRACE_LOW:"[MMIIKB] IKeyBackup_EncBuf cancel by user."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_TASK_1353_112_2_18_2_24_27_48,(uint8*)"");
            error_code = MMIIKB_STATUS_CANCEL_ACTION;
            break;
        }
        
        *buf_ptr = ((*buf_ptr)^IKEYBACKUP_ENCRYPT_DATA);
        buf_ptr++;
        
#if 0

        SCI_SLEEP(1000);
        if (buf_len == 0)
        {
            if (test_tt-- > 0)
            {
                buf_len = src_buf_len;
                buf_ptr = buf_src_ptr;
            }
        }
#endif        
    }
    
    return error_code;
}

/*****************************************************************************/
//  Description : IKeyBackup_DecBuf
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
PUBLIC MMIIKB_ERROR_E IKeyBackup_DecBuf(
    uint8 * buf_ptr,
    uint16 buf_len
)
{
    MMIIKB_ERROR_E error_code = MMIIKB_STATUS_SUCCESS;

    if (PNULL == buf_ptr
        || 0 == buf_len)
    {
        error_code = MMIIKB_STATUS_INVALID_PARAM;
        //SCI_TRACE_LOW:"[MMIIKB] IKeyBackup_DecBuf param is null."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_TASK_1393_112_2_18_2_24_27_49,(uint8*)"");
        return error_code;
    }
    
    while (buf_len--)
    {
        if (IKeyBackup_GetCancelState())
        {
            //SCI_TRACE_LOW:"[MMIIKB] IKeyBackup_DecBuf cancel by user."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_TASK_1401_112_2_18_2_24_27_50,(uint8*)"");
            error_code = MMIIKB_STATUS_CANCEL_ACTION;
            break;
        }
        
        *buf_ptr = ((*buf_ptr)^IKEYBACKUP_ENCRYPT_DATA);
        buf_ptr++;
    }

    return error_code;
}

/*****************************************************************************/
//  Description : IKeyBackup_GetPackContentNumber
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC uint32 IKeyBackup_GetPackContentNumber(
    const uint16 *pack_path_ptr,
    uint16 pack_path_len,
    MMIIKB_TYPE_E type
)
{
    MMIIKB_ERROR_E error_code = MMIIKB_STATUS_SUCCESS;
    uint32 total = 0;    
    uint16 *file_path = PNULL;
    uint16 file_path_len = 0;
    MMIIKB_PACK_CONT_HEAD_T *cont_head_ptr = PNULL;

    if (PNULL == pack_path_ptr
        || 0 == pack_path_len
        || MMIIKB_TYPE_MAX <= type)
    {
        return total;
    }

    file_path = (wchar *)SCI_ALLOCA(sizeof(wchar)*(MMIIKB_CON_PATH_LEN));
    if (PNULL == file_path)
    {
        //SCI_TRACE_LOW:"[MMIIKB] IKeyBackup_GetPackContentNumber no memory."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_TASK_1439_112_2_18_2_24_27_51,(uint8*)"");
        return total;
    }
    SCI_MEMSET(file_path, 0x0, sizeof(wchar)*(MMIIKB_CON_PATH_LEN));

    //get content file path
    //SCI_MEMSET(file_path, 0x0, sizeof(file_path));
    error_code = 
        GetContentFilePath(type, pack_path_ptr, file_path, &file_path_len);/*lint !e605 */
    
    if (MMIIKB_STATUS_SUCCESS == error_code)
    {
        cont_head_ptr = (MMIIKB_PACK_CONT_HEAD_T *)SCI_ALLOCA(
            sizeof(MMIIKB_PACK_CONT_HEAD_T));
        if (PNULL == cont_head_ptr)
        {
            //SCI_TRACE_LOW:"[MMIIKB] IKeyBackup_GetPackContentNumber no memory."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_TASK_1455_112_2_18_2_24_27_52,(uint8*)"");
            SCI_FREE(file_path);
            return total;
        }
        SCI_MEMSET(cont_head_ptr, 0x0, sizeof(MMIIKB_PACK_CONT_HEAD_T));

        error_code = ReadContentHead(file_path, file_path_len, cont_head_ptr);
        if (MMIIKB_STATUS_SUCCESS == error_code)
        {
            total = cont_head_ptr->number;
        }

        SCI_FREE(cont_head_ptr);
    }

    SCI_FREE(file_path);

    return (total);    
}



/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DEFINITION                        */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : create 1-key backup task
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL void IKBHandlerMain(uint32 argc, void* argv)
{
    MMIIKB_ERROR_E error_code = MMIIKB_STATUS_INVALID_PARAM;
    xSignalHeaderRec *sig_ptr = PNULL;

    UNUSED_PARAM(argc);
    UNUSED_PARAM(argv);

    while(1) /*lint !e716 */
    {        
        //Get SIG From Queue
        sig_ptr = SCI_GetSignal(SCI_IdentifyThread());
       
        switch(sig_ptr->SignalCode)
        {
        case MSG_IKB_LOAD_ALL_NODE_REQ:
            //SCI_TRACE_LOW:"[MMIIKB] Command is: MSG_IKB_LOAD_ALL_NODE_REQ"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_TASK_1499_112_2_18_2_24_27_53,(uint8*)"");
            break;
        case MSG_IKB_BACKUP_DATA_REQ:
            //SCI_TRACE_LOW:"[MMIIKB] Command is: MSG_IKB_BACKUP_DATA_REQ"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_TASK_1502_112_2_18_2_24_27_54,(uint8*)"");
            error_code = IKBBackupAllItems(sig_ptr);
            break;
        case MSG_IKB_BACKUP_ITEM_DATA_RES:   
            //收到应用的回执signal，则停止定时器
            StopProtectionTimer();    
            //SCI_TRACE_LOW:"[MMIIKB] Command is: MSG_IKB_BACKUP_ITEM_DATA_RES"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_TASK_1508_112_2_18_2_24_27_55,(uint8*)"");
            error_code = IKBBackupItemUnitDataAsy(sig_ptr);

            //释放内部内存
            if (PNULL != sig_ptr)
            {
                MMIIKB_SIG_BACKUP_ITEM_DATA_RES_T * sig_item_data_ptr = PNULL;
                sig_item_data_ptr = (MMIIKB_SIG_BACKUP_ITEM_DATA_RES_T*)sig_ptr;
                if (PNULL != sig_item_data_ptr->backup_item_data_res.buf_ptr)
                {
                    SCI_FREE(sig_item_data_ptr->backup_item_data_res.buf_ptr);
                }
            }
            break;
        case MSG_IKB_RESTORE_DATA_REQ:
            //SCI_TRACE_LOW:"[MMIIKB] Command is: MSG_IKB_RESTORE_DATA_REQ"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_TASK_1512_112_2_18_2_24_27_56,(uint8*)"");
            error_code = IKBRestoreAllItems(sig_ptr);
            break;
        case MSG_IKB_RESTORE_ITEM_DATA_RES:
            //SCI_TRACE_LOW:"[MMIIKB] Command is: MSG_IKB_RESTORE_ITEM_DATA_RES"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_TASK_1516_112_2_18_2_24_27_57,(uint8*)"");
            StopProtectionTimer();
            error_code = IKBRestoreItemUnitDataAsy(sig_ptr);
            break;
        case MSG_IKB_CANCEL_ACTION_REQ:
            //SCI_TRACE_LOW:"[MMIIKB] Command is: MSG_IKB_CANCEL_ACTION_REQ"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_TASK_1521_112_2_18_2_24_27_58,(uint8*)"");
            error_code = IKBCancelAction(sig_ptr);
            break;
            
        default:
            //SCI_TRACE_LOW:"[MMIIKB] Command is: IKB_SIG_DEFAULT code=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_TASK_1527_112_2_18_2_24_27_59,(uint8*)"d",sig_ptr->SignalCode);
            break;
        }  
        
        //SCI_TRACE_LOW:"[MMIIKB] IKBHandlerMain ret_code = %d\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_TASK_1531_112_2_18_2_24_27_60,(uint8*)"d", error_code);
        
        MMI_FREE_SIGNAL(sig_ptr);
        sig_ptr = PNULL;
        
    }
}

/*****************************************************************************/
//  Description : IKBBackupAllItems
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
LOCAL MMIIKB_ERROR_E IKBBackupAllItems(xSignalHeaderRec *sig_ptr)
{
    MMIIKB_ERROR_E error_code = MMIIKB_STATUS_INVALID_PARAM;
    MMIIKB_SIG_BACKUP_REQ_T * backup_req_ptr = PNULL;

    if (PNULL == sig_ptr)
    {
        //SCI_TRACE_LOW:"[MMIIKB] IKBBackupAllItems parma is null."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_TASK_1550_112_2_18_2_24_27_61,(uint8*)"");
        
        error_code = MMIIKB_STATUS_INVALID_PARAM;
        return error_code;
    }

    //1: copy signal info to static var
    if (PNULL != s_backup_req_ptr)
    {
        //not allowed if task is busy in backup last time
        //SCI_TRACE_LOW:"[MMIIKB] IKBBackupAllItems s_backup_req_ptr is no-null."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_TASK_1560_112_2_18_2_24_27_62,(uint8*)"");
        
        error_code =  MMIIKB_STATUS_IKB_IS_BUSY;
        BackupResProcess(error_code, TRUE);
        return error_code;
    }

    backup_req_ptr = (MMIIKB_SIG_BACKUP_REQ_T*)sig_ptr;
    s_backup_req_ptr = (MMIIKB_SIG_BACKUP_REQ_T*)SCI_ALLOCA(
        sizeof(MMIIKB_SIG_BACKUP_REQ_T));
    if (PNULL == s_backup_req_ptr)
    {
        //SCI_TRACE_LOW:"[MMIIKB] IKBBackupAllItems parma memory is out."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_TASK_1572_112_2_18_2_24_27_63,(uint8*)"");
        
        error_code =  MMIIKB_STATUS_NO_MEMORY;
        return error_code;
    }
    SCI_MEMSET(s_backup_req_ptr, 0x00, sizeof(MMIIKB_SIG_BACKUP_REQ_T));
    SCI_MEMCPY(s_backup_req_ptr, backup_req_ptr, 
        sizeof(MMIIKB_SIG_BACKUP_REQ_T));

    //2: init task sequences table
    InitTaskSeqTable(backup_req_ptr->backup_req.types, MMIIKB_TASK_TYPE_BACKUP);

    //3: Backup circle tasks
    error_code = BackupCircleTask(s_backup_req_ptr);
        
    return error_code;
}

/*****************************************************************************/
//  Description : IKBBackupItemUnitDataAsy
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
LOCAL MMIIKB_ERROR_E IKBBackupItemUnitDataAsy(xSignalHeaderRec *sig_ptr)    
{
    MMIIKB_ERROR_E error_code = MMIIKB_STATUS_INVALID_PARAM;
    MMIIKB_SIG_BACKUP_ITEM_DATA_RES_T *sig_item_data_ptr = PNULL;
    const MMIIKB_TASK_SEQ_TABLE_T *task_seq_ptr = PNULL;
    const MMIIKB_TYPE_PROCESS_PARAM_T *process_param_ptr = PNULL;
    MMIIKB_TYPE_E type = MMIIKB_TYPE_MAX;

    if (PNULL == sig_ptr)
    {
        error_code = MMIIKB_STATUS_INVALID_PARAM;
        BackupResProcess(error_code, FALSE);
        if (PNULL != s_backup_req_ptr)
        {
            SCI_FREE(s_backup_req_ptr);
        }
        return error_code;
    }

    if (PNULL == s_backup_req_ptr && !s_is_timeout)
    {
        error_code = MMIIKB_STATUS_NO_INIT_TASK;
        BackupResProcess(error_code, FALSE);
        return error_code;
    }

    sig_item_data_ptr = (MMIIKB_SIG_BACKUP_ITEM_DATA_RES_T*)sig_ptr;
    
    error_code = sig_item_data_ptr->backup_item_data_res.error_code;
    if (MMIIKB_STATUS_SUCCESS == error_code)
    {
        type = sig_item_data_ptr->backup_item_data_res.type;
        task_seq_ptr = MMIIKB_GetTaskItemByType(type);

        if (PNULL != task_seq_ptr)
        {
            process_param_ptr = MMIIKB_GetTypeProcessParamPtr(type);

            if (PNULL != process_param_ptr)
            {
                if (MMIIKB_TASK_TYPE_BACKUP == task_seq_ptr->task_type)
                {
                    if (process_param_ptr->is_backup_asyn)
                    {
                        error_code = BackupHandleItemUnitDataAsy(
                            &(sig_item_data_ptr->backup_item_data_res),
                            task_seq_ptr, process_param_ptr, s_backup_req_ptr);
                    }
                    else
                    {
                        error_code = MMIIKB_STATUS_WRONG_TASK;
                    }
                }
                else
                {
                    error_code = MMIIKB_STATUS_WRONG_OP_TYPE;
                }
            }
            else
            {
                error_code = MMIIKB_STATUS_INVALID_PARAM;
            }
        }
        else
        {
            error_code = MMIIKB_STATUS_WRONG_TASK;
        }
    }

    //if not success, notify app
    if (MMIIKB_STATUS_SUCCESS != error_code)
    {
        BackupResProcess(error_code, TRUE);
    }
    
    return error_code;
}

/*****************************************************************************/
//  Description : SetContentFileName
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
LOCAL MMIIKB_ERROR_E SetContentFileName(
    MMIIKB_TYPE_E type, 
    uint16 *dest_file_name,
    uint16 dest_file_len
)
{
    MMIIKB_ERROR_E error_code = MMIIKB_STATUS_INVALID_PARAM;
    const uint16 * content_name_ptr = PNULL;
    uint16 content_name_len = 0;

    if (PNULL == dest_file_name
        || 0 == dest_file_len)
    {
        //SCI_TRACE_LOW:"[MMIIKB] SetContentFileName param is null."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_TASK_1689_112_2_18_2_24_28_64,(uint8*)"");
        return error_code;
    }

    content_name_ptr = IKeyBackup_GetContentName(type);
    if (PNULL == content_name_ptr)
    {
        //SCI_TRACE_LOW:"[MMIIKB] SetContentFileName can't get content name withtype=%d."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_TASK_1697_112_2_18_2_24_28_65,(uint8*)"d", type);
            
        error_code = MMIIKB_STATUS_FAILED;
        return error_code;
    }
    
    content_name_len = MMIAPICOM_Wstrlen(content_name_ptr);
    SCI_MEMCPY(dest_file_name, content_name_ptr, 
        sizeof(uint16)*(MIN(dest_file_len, content_name_len)));
    
    error_code = MMIIKB_STATUS_SUCCESS;    
    return error_code;
}

/*****************************************************************************/
//  Description : SetBackupResParmater
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
LOCAL MMIIKB_ERROR_E SetBackupResParmater(
    MMIIKB_SIG_BACKUP_RES_T *sig_backup_res_ptr,
    MMIIKB_SIG_BACKUP_REQ_T *sig_backup_req_ptr
)
{
    MMIIKB_ERROR_E error_code = MMIIKB_STATUS_INVALID_PARAM;
    uint16 i = 0;
    MMIIKB_TYPE_E type = 0;
    
    if (PNULL == sig_backup_res_ptr)
    {
        //SCI_TRACE_LOW:"[MMIIKB] SetBackupResParmater param is null."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_TASK_1726_112_2_18_2_24_28_66,(uint8*)"");
        return error_code;
    }

    for (i = 0; i < MMIIKB_TYPE_MAX; i++)
    {
        type = s_ikb_task_seq_table[i].type;
        if (MMIIKB_TYPE_MAX != type
            && MMIIKB_TASK_TYPE_BACKUP == s_ikb_task_seq_table[i].task_type)
        {
            sig_backup_res_ptr->size += s_ikb_task_seq_table[i].total_size;
            sig_backup_res_ptr->types |= (0x01 << s_ikb_task_seq_table[i].type);
            sig_backup_res_ptr->crcs[type] = s_ikb_task_seq_table[i].crc;
        }
    }

    if (PNULL != sig_backup_req_ptr)
    {
        SCI_MEMCPY(sig_backup_res_ptr->pack_dir, 
            sig_backup_req_ptr->backup_req.pack_dir, 
            sizeof(sig_backup_res_ptr->pack_dir));
        SCI_MEMCPY(&sig_backup_res_ptr->psw_info, 
            &sig_backup_req_ptr->backup_req.psw_info, 
            sizeof(MMIIKB_PASSWORD_INFO_T));
    }

    error_code = MMIIKB_STATUS_SUCCESS;
    return error_code;    
}

/*****************************************************************************/
//  Description : BackupCircleTask
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 本函数一定要保证只有一个出口
/*****************************************************************************/
LOCAL MMIIKB_ERROR_E BackupCircleTask(
    MMIIKB_SIG_BACKUP_REQ_T * sig_backup_req_ptr
)
{
    MMIIKB_ERROR_E error_code = MMIIKB_STATUS_INVALID_PARAM;
    const MMIIKB_TASK_SEQ_TABLE_T *task_seq_ptr = PNULL;
    const MMIIKB_TYPE_PROCESS_PARAM_T *process_param_ptr = PNULL;
    BOOLEAN has_backup_item = FALSE;
    BOOLEAN is_asy_ret = FALSE;

    //SCI_ASSERT(PNULL != sig_backup_req_ptr);
    if (PNULL == sig_backup_req_ptr)
    {
        //这里理论上应该是永远不该跑的
        SCI_TRACE_LOW("[MMIIKB] BackupCircleTask parma is null.");
        return error_code;
    }
    
    //SCI_TRACE_LOW:"[MMIIKB] BackupCircleTask begin."
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_TASK_1772_112_2_18_2_24_28_67,(uint8*)"");
        
    task_seq_ptr = GetCurTaskItem();

    while (PNULL != task_seq_ptr)
    {
        process_param_ptr = MMIIKB_GetTypeProcessParamPtr(task_seq_ptr->type);

        if (MMIIKB_TASK_TYPE_BACKUP == task_seq_ptr->task_type)
        {
            has_backup_item = TRUE;
        
            if (!process_param_ptr->is_backup_asyn)
            {
                error_code = BackupItemDataSyn(
                    process_param_ptr, sig_backup_req_ptr);

                if (MMIIKB_STATUS_SUCCESS != error_code)
                {
                    break;
                }
            }
            else
            {
                error_code = BackupItemDataAsy(
                    process_param_ptr, sig_backup_req_ptr);
                
                is_asy_ret = TRUE;
                break;
            }
        }
        
        task_seq_ptr = GetCurTaskItem();
    }
    
    //SCI_TRACE_LOW:"[MMIIKB] BackupCircleTask end with error_code=%d, is_asy_ret=%d, has_backup_item=%d."
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_TASK_1808_112_2_18_2_24_28_68,(uint8*)"ddd", error_code, is_asy_ret, has_backup_item);

    //check task over
    if (!has_backup_item)
    {
    
        error_code = MMIIKB_STATUS_SUCCESS;
        BackupResProcess(error_code, TRUE);
    }
    else
    {
        if (MMIIKB_STATUS_SUCCESS == error_code)
        {
            if (!is_asy_ret && IsTaskOver())
            {             
                BackupResProcess(error_code, TRUE);
            }
        }
        else
        {
            //failed whether syn or asyn
            BackupResProcess(error_code, TRUE);
        }
    }

    return error_code;
}

/*****************************************************************************/
//  Description : BackupResProcess
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 本函数一定要保证只有一个出口
/*****************************************************************************/
LOCAL MMIIKB_ERROR_E BackupResProcess(
    MMIIKB_ERROR_E param_error_code, 
    BOOLEAN is_free
)
{
    MMIIKB_ERROR_E error_code = MMIIKB_STATUS_INVALID_PARAM;
    MMIIKB_SIG_BACKUP_RES_T *sig_backup_res_ptr = PNULL;
    
    //1: response message to sending task
    sig_backup_res_ptr = SCI_ALLOCA(sizeof(MMIIKB_SIG_BACKUP_RES_T));
    if (PNULL == sig_backup_res_ptr)
    {
        SCI_TRACE_LOW("[MMIIKB] BackupResProcess memory is out.");
        return error_code;
    }
    SCI_MEMSET(sig_backup_res_ptr, 0x00, sizeof(MMIIKB_SIG_BACKUP_RES_T));
    
    sig_backup_res_ptr->SignalCode = MSG_IKB_BACKUP_DATA_RES;
    sig_backup_res_ptr->Sender = IKeyBackup_GetIKBTaskId();
    sig_backup_res_ptr->SignalSize = sizeof(MMIIKB_SIG_BACKUP_RES_T);

    sig_backup_res_ptr->error_code = param_error_code;
    if (PNULL != s_backup_req_ptr && is_free)
    {
        SetBackupResParmater(sig_backup_res_ptr, s_backup_req_ptr);
    }

    //user cancel
    if (MMIIKB_STATUS_CANCEL_ACTION == param_error_code)
    {
        if (PNULL != s_cancel_pack_path_ptr)
        {
            SCI_FREE(s_cancel_pack_path_ptr);
            s_cancel_pack_path_ptr = PNULL;
        }
            
        if (PNULL != s_backup_req_ptr)
        {
            s_cancel_pack_path_ptr = (wchar*)SCI_ALLOCA(
                MMIIKB_PACK_DIR_LEN*sizeof(wchar));
            
            SCI_MEMCPY(s_cancel_pack_path_ptr,
                s_backup_req_ptr->backup_req.pack_dir,
                MMIIKB_PACK_DIR_LEN*sizeof(wchar));
        }

        if (IKeyBackup_GetCancelState())
        {
            IKeyBackup_ActionSetOk();
        }
    }
    
    //SCI_TRACE_LOW:"[MMIIKB] BackupResProcess send response to app."
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_TASK_1888_112_2_18_2_24_28_69,(uint8*)"");
    
    IKEYBACKUP_SEND_SIGNAL(sig_backup_res_ptr, P_APP);

    if (is_free)
    {
        //2: free backup request parameter
        if (PNULL != s_backup_req_ptr)
        {
            SCI_FREE(s_backup_req_ptr);
        }

        //3: reset task sequences table
        ResetTaskSeqTable();
    }

    error_code = MMIIKB_STATUS_SUCCESS;
    return error_code;
}

/*****************************************************************************/
//  Description : InitTaskSeqTable
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL MMIIKB_ERROR_E InitTaskSeqTable(
    MMIIKB_TYPE_T types,
    MMIIKB_TASK_TYPE_E task_type
)
{
    MMIIKB_ERROR_E error_code = MMIIKB_STATUS_INVALID_PARAM;
    uint16 i = 0;
    
    if (0 == types || MMIIKB_TASK_TYPE_NONE == task_type)
    {
        error_code = MMIIKB_STATUS_INVALID_PARAM;
        return error_code;
    }

    //1: clear table values
    SCI_MEMSET(s_ikb_task_seq_table, 0x0, sizeof(s_ikb_task_seq_table));

    //2: init this task values
    for (i = 0; i < MMIIKB_TYPE_MAX; i++)
    {
        if (0x0 != (types & (0x1 << i)))
        {
            s_ikb_task_seq_table[i].type = (MMIIKB_TYPE_E)(i);
            s_ikb_task_seq_table[i].task_type = task_type;
            s_ikb_task_seq_table[i].task_state = MMIIKB_TASK_STATE_START;
        }
        else
        {
            s_ikb_task_seq_table[i].type = MMIIKB_TYPE_MAX;
            s_ikb_task_seq_table[i].task_type = MMIIKB_TASK_TYPE_MAX;
            s_ikb_task_seq_table[i].task_state = MMIIKB_TASK_STATE_MAX;
        }
    }
    
    //SCI_TRACE_LOW:"[MMIIKB] InitTaskSeqTable success."
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_TASK_1946_112_2_18_2_24_28_70,(uint8*)"");
    
    error_code = MMIIKB_STATUS_SUCCESS;
    return error_code;
}

/*****************************************************************************/
//  Description : GetCurTaskItem
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:取不是完成状态的第一个任务
/*****************************************************************************/
LOCAL const MMIIKB_TASK_SEQ_TABLE_T * GetCurTaskItem(void)
{
    uint16 i = 0;
    const MMIIKB_TASK_SEQ_TABLE_T *task_seq_ptr = PNULL;
    
    for (i = 0; i < MMIIKB_TYPE_MAX; i++)
    {
        if (MMIIKB_TYPE_MAX != s_ikb_task_seq_table[i].type
            && ((MMIIKB_TASK_STATE_START == s_ikb_task_seq_table[i].task_state)
            || (MMIIKB_TASK_STATE_PROCESSING == 
            s_ikb_task_seq_table[i].task_state)))
            
        {
            task_seq_ptr = &(s_ikb_task_seq_table[i]);
            break;
        }
    }
    
    return task_seq_ptr;
}

/*****************************************************************************/
//  Description : GetCurTaskItem
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:取不是完成状态的第一个任务
/*****************************************************************************/
LOCAL BOOLEAN ResetTaskSeqTable(void)
{
    BOOLEAN is_ret = FALSE;
    uint16 i = 0;
    
    //SCI_TRACE_LOW:"[MMIIKB] ResetTaskSeqTable."
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_TASK_1986_112_2_18_2_24_28_71,(uint8*)"");

    SCI_MEMSET(s_ikb_task_seq_table, 0x0, sizeof(s_ikb_task_seq_table));
    for (i = 0; i < MMIIKB_TYPE_MAX; i++)
    {
        s_ikb_task_seq_table[i].type = MMIIKB_TYPE_MAX;
    }
    
    is_ret = TRUE;
    return is_ret;
}

/*****************************************************************************/
//  Description : MMIIKB_GetTaskItemByType
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC const MMIIKB_TASK_SEQ_TABLE_T * MMIIKB_GetTaskItemByType(
    MMIIKB_TYPE_E type
)
{    
    uint16 i = 0;
    const MMIIKB_TASK_SEQ_TABLE_T *task_seq_ptr = PNULL;

    if (type >= MMIIKB_TYPE_MAX)
    {
        //SCI_TRACE_LOW:"[MMIIKB] MMIIKB_GetTaskItemByType param is type=%d."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_TASK_2011_112_2_18_2_24_28_72,(uint8*)"d", type);
        return PNULL;
    }
    
    for (i = 0; i < MMIIKB_TYPE_MAX; i++)
    {
        if (type == s_ikb_task_seq_table[i].type)
            
        {
            task_seq_ptr = &(s_ikb_task_seq_table[i]);
            break;
        }
    }
    
    return task_seq_ptr;
}

/*****************************************************************************/
//  Description : SetTaskTableMaxValue
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL MMIIKB_ERROR_E SetTaskTableMaxValue(
    const MMIIKB_TYPE_PROCESS_PARAM_T * process_param_ptr
)
{
    MMIIKB_ERROR_E error_code = MMIIKB_STATUS_INVALID_PARAM;
    MMIIKB_GET_TOTAL_OUT_T param_out = {0};
    MMIIKB_TOTAL_TYPE_E total_type = MMIIKB_TOTAL_TYPE_NONE;

    if (PNULL == process_param_ptr)
    {
        //SCI_TRACE_LOW:"[MMIIKB] SetTaskTableMaxValue param is null."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_TASK_2042_112_2_18_2_24_28_73,(uint8*)"");
        return error_code;
    }

    process_param_ptr->get_total_call_back(&param_out);
    
    error_code = param_out.error_code;

    if (MMIIKB_STATUS_SUCCESS == error_code)
    {
        total_type =  param_out.total_type;

        if (MMIIKB_TOTAL_TYPE_NUMBER == param_out.total_type)
        {
            error_code = UpdateTaskTable(
                process_param_ptr->type, MMIIKB_TASK_STATE_MAX, 
                PNULL, &param_out.total,
                PNULL, PNULL,
                PNULL, PNULL, PNULL);
        }
        else if (MMIIKB_TOTAL_TYPE_BYTE == param_out.total_type)
        {
            error_code = UpdateTaskTable(
                process_param_ptr->type, MMIIKB_TASK_STATE_MAX, 
                PNULL, PNULL,
                PNULL, &param_out.total,
                PNULL, PNULL, PNULL);
        }
        else
        {
            error_code = UpdateTaskTable(
                process_param_ptr->type, MMIIKB_TASK_STATE_MAX, 
                PNULL, &param_out.total,
                PNULL, &param_out.total,
                PNULL, &param_out.total, PNULL);
        }
    }
    
    //SCI_TRACE_LOW:"[MMIIKB] SetTaskTableMaxValue type=%d, error_code=%d, total_type=%d, total_size=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_TASK_2082_112_2_18_2_24_28_74,(uint8*)"dddd", process_param_ptr->type, error_code, param_out.total_type, param_out.total);
    
    return error_code;
}

/*****************************************************************************/
//  Description : UpdateTaskTable
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 更新任务情况表
/*****************************************************************************/
LOCAL MMIIKB_ERROR_E UpdateTaskTable(
    MMIIKB_TYPE_E type,
    MMIIKB_TASK_STATE_E task_state,    
    const uint32 *trans_number_ptr,   
    const uint32 *total_number_ptr,
    const uint32 *trans_size_ptr,
    const uint32 *total_size_ptr,
    const uint32 *trans_cnt_ptr,
    const uint32 *total_cnt_ptr,
    const uint32 *crc_ptr
)
{
    MMIIKB_ERROR_E error_code = MMIIKB_STATUS_INVALID_PARAM;
    uint16 i = 0;
    BOOLEAN is_find = FALSE;

    if (type >= MMIIKB_TYPE_MAX)
    {
        //SCI_TRACE_LOW:"[MMIIKB] UpdateTaskTable param is type=%d."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_TASK_2109_112_2_18_2_24_28_75,(uint8*)"d", type);
        return error_code;
    }

    for (i = 0; i < MMIIKB_TYPE_MAX; i++)
    {
        if (type == s_ikb_task_seq_table[i].type)
        {
            if (MMIIKB_TASK_STATE_MAX != task_state)
            {
                s_ikb_task_seq_table[i].task_state = task_state;
            }
            if (PNULL != trans_number_ptr)
            {
                s_ikb_task_seq_table[i].trans_number = *trans_number_ptr;
            }
            if (PNULL != total_number_ptr)
            {
                s_ikb_task_seq_table[i].total_number = *total_number_ptr;
            }
            if (PNULL != trans_size_ptr)
            {
                s_ikb_task_seq_table[i].trans_size = *trans_size_ptr;
            }
            if (PNULL != total_size_ptr)
            {
                s_ikb_task_seq_table[i].total_size = *total_size_ptr;
            }
            if (PNULL != trans_cnt_ptr)
            {
                s_ikb_task_seq_table[i].trans_cnt = *trans_cnt_ptr;
            }
            if (PNULL != total_cnt_ptr)
            {
                s_ikb_task_seq_table[i].total_cnt = *total_cnt_ptr;
            }
            if (PNULL != crc_ptr)
            {
                s_ikb_task_seq_table[i].crc = *crc_ptr;
            }
            
            is_find = TRUE;
            break;
        }
    }

    if (is_find)
    {
        error_code = MMIIKB_STATUS_SUCCESS;
    }
    else
    {
        error_code = MMIIKB_STATUS_NO_INIT_TASK;
    }

    IKeyBackup_SendProgressInfo(MMIIKB_GetTaskItemByType(type));
    
    return error_code;
}

/*****************************************************************************/
//  Description : DestoryTaskSeqTable
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL MMIIKB_ERROR_E DestoryTaskSeqTable(void)
{
    //free all memory.利用全局表指针
    return MMIIKB_STATUS_INVALID_PARAM;
}

/*****************************************************************************/
//  Description : IsTaskOver
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsTaskOver(void)
{
    uint16 i = 0;
    BOOLEAN is_over = TRUE;

    for (i = 0; i < MMIIKB_TYPE_MAX; i++)
    {
        if (MMIIKB_TYPE_MAX != s_ikb_task_seq_table[i].type
            && MMIIKB_TASK_STATE_END != s_ikb_task_seq_table[i].task_state
            && MMIIKB_TASK_STATE_FAIL != s_ikb_task_seq_table[i].task_state)
        {
            is_over = FALSE;
            break;
        }
    }
    
    return is_over;
}

/*****************************************************************************/
//  Description : BackupItemDataSyn
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
LOCAL MMIIKB_ERROR_E BackupItemDataSyn(
    const MMIIKB_TYPE_PROCESS_PARAM_T * process_param_ptr,
    MMIIKB_SIG_BACKUP_REQ_T * backup_req_ptr
)
{
    MMIIKB_ERROR_E error_code = MMIIKB_STATUS_INVALID_PARAM;

    if (PNULL == process_param_ptr
        || PNULL == backup_req_ptr)
    {
        //SCI_TRACE_LOW:"[MMIIKB] BackupItemDataSyn param is null."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_TASK_2216_112_2_18_2_24_29_76,(uint8*)"");
        
        return error_code;
    }

    if (MMIIKB_OPERATE_TYPE_BUF == process_param_ptr->op_type)
    {
        error_code = BackupItemDataSynByBuf(
            process_param_ptr, backup_req_ptr);
    }
    else if (MMIIKB_OPERATE_TYPE_FILE == process_param_ptr->op_type)
    {
        error_code = BackupItemDataSynByFile(
            process_param_ptr, backup_req_ptr);
    }
    else if (MMIIKB_OPERATE_TYPE_DIR == process_param_ptr->op_type)
    {
        error_code = BackupItemDataSynByDir(
            process_param_ptr, backup_req_ptr);
    }
    
    return error_code;
}

/*****************************************************************************/
//  Description : BackupItemDataSynByBuf
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
LOCAL MMIIKB_ERROR_E BackupItemDataSynByBuf(
    const MMIIKB_TYPE_PROCESS_PARAM_T * process_param_ptr,
    MMIIKB_SIG_BACKUP_REQ_T * backup_req_ptr
)
{
    MMIIKB_ERROR_E error_code = MMIIKB_STATUS_INVALID_PARAM;
    MMIIKB_BACKUP_CALLBACK_PARAM_IN_T param_in = {0};
    MMIIKB_BACKUP_CALLBACK_PARAM_OUT_T param_out = {0};
    MMIIKB_TYPE_E type = MMIIKB_TYPE_MAX;
    uint16 file_path[MMIIKB_CON_PATH_LEN] = {0};
    uint16 file_path_len = 0;
    void *buf_ptr = PNULL;
    MMIIKB_PACK_CONT_HEAD_T cont_head = {0};
    MMIIKB_PACK_CONT_UNIT_T cont_unit = {0};
    uint32 crc = 0;
    BOOLEAN is_enc = FALSE;

    if (PNULL == process_param_ptr
        || PNULL == backup_req_ptr)
    {
        //SCI_TRACE_LOW:"[MMIIKB] BackupItemDataSynByBuf param is null."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_TASK_2264_112_2_18_2_24_29_77,(uint8*)"");
        return error_code;
    }

    type = process_param_ptr->type;

    //get content file path
    SCI_MEMSET(file_path, 0x0, sizeof(file_path));
    error_code = GetContentFilePath(type, backup_req_ptr->backup_req.pack_dir, 
        file_path, &file_path_len);
    if (MMIIKB_STATUS_SUCCESS != error_code)
    {
        //SCI_TRACE_LOW:"[MMIIKB] BackupItemDataSynByBuf can't get file path."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_TASK_2276_112_2_18_2_24_29_78,(uint8*)"");
        return error_code;
    }
    
    SCI_MEMSET(cont_head.content_name, 0x0, sizeof(cont_head.content_name));
    SetContentFileName(type, cont_head.content_name, 
        MMIIKB_PACK_CONTENT_NAME_LEN);
    cont_head.type = type;
    cont_head.op_type = MMIIKB_OPERATE_TYPE_BUF;
    cont_head.size = 0;
    cont_head.number = 0;
    cont_head.trans_cnt = 0;
    cont_head.is_zip = FALSE;
    SCI_MEMSET(cont_head.content_path, 0x0, MMIIKB_CON_PATH_LEN*sizeof(wchar))
    cont_head.file_size = 0;
    cont_head.reserved1 = 0;
    cont_head.reserved2 = 0;
    cont_head.reserved3 = 0;

    //init parma_in info
    param_in.type = type;
    param_in.op_type = MMIIKB_OPERATE_TYPE_BUF;
    param_in.begin = 0;
    param_in.len = 0;
    param_in.number = 0;
    param_in.trans_cnt = 0;

    //set item total values
    SetTaskTableMaxValue(process_param_ptr);

    while (MMIIKB_STATUS_SUCCESS == 
        ReceiveUnitData(process_param_ptr, &param_in, &param_out))
    {
        if (MMIIKB_STATUS_SUCCESS != param_out.error_code)
        {   
            //type=%d, in.begin=%d, in.len=%d, in.num=%d, in.trans=%d, out.type=%d, out.begin=%d, out.len=%d, out.num=%d, out.fin=%d, error_code=%d"
            //SCI_TRACE_LOW:"[MMIIKB] BackupItemDataSynByBuf get data from app error. type=%d, in.begin=%d, in.len=%d, in.num=%d, in.trans=%dout.type=%d, out.begin=%d, out.len=%d, out.num=%d, out.fin=%derror_code=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_TASK_2317_112_2_18_2_24_29_79,(uint8*)"ddddddddddd",type, param_in.begin, param_in.len, param_in.number, param_in.trans_cnt, param_out.begin, param_out.len, param_out.number, param_out.is_finished, param_out.error_code);

            error_code = param_out.error_code;
            break;
        }
        
        if (PNULL == param_out.buf_ptr)
        {
            //SCI_TRACE_LOW:"[MMIIKB] BackupItemDataSynByBuf get buf is null,type=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_TASK_2325_112_2_18_2_24_29_80,(uint8*)"d", type);
            
            error_code = MMIIKB_STATUS_EMPTY_APP_BUF;
            break;
        }

        buf_ptr = SCI_ALLOCA(param_out.len);
        if (PNULL == buf_ptr)
        {
            //SCI_TRACE_LOW:"[MMIIKB] BackupItemDataSynByBuf memory is out,type=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_TASK_2335_112_2_18_2_24_29_81,(uint8*)"d", type);
                
            error_code =  MMIIKB_STATUS_NO_MEMORY;
            break;
        }
        SCI_MEMSET(buf_ptr, 0x00, param_out.len);
        SCI_MEMCPY(buf_ptr, param_out.buf_ptr, param_out.len);
        
        if (param_out.is_need_free && PNULL != param_out.buf_ptr)
        {
            SCI_FREE(param_out.buf_ptr);
        }

        //encript buf
        is_enc = IKeyBackup_IsEnc(type);
        if (is_enc)
        {
            error_code = IKeyBackup_EncBuf((uint8*)buf_ptr, param_out.len);
        }
        else
        {
            error_code = MMIIKB_STATUS_SUCCESS;
        }

        if (MMIIKB_STATUS_SUCCESS == error_code)
        {
            //update unit content info
            cont_unit.len = param_out.len;
            cont_unit.number = param_out.number;
            cont_unit.reserved1 = IKEYBACKUP_DATA_END_SIG;
            cont_unit.reserved2 = IKEYBACKUP_DATA_END_SIG;
            cont_unit.buf_ptr = buf_ptr;
            cont_unit.end_sign1 = IKEYBACKUP_DATA_END_SIG;
            cont_unit.end_sign2 = IKEYBACKUP_DATA_END_SIG;
            
            //write unit content
            error_code = WriteUnitData(file_path, file_path_len, &cont_unit);

            //free buf whether success or not
            SCI_FREE(buf_ptr);
            
            if (MMIIKB_STATUS_SUCCESS != error_code)
            {
                //SCI_TRACE_LOW:"[MMIIKB] BackupItemDataSynByBuf writedata error,type=%d, error_code=%d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_TASK_2379_112_2_18_2_24_29_82,(uint8*)"dd", type, error_code);
                
                break;
            }

            //update content head info
            cont_head.size += param_out.len;
            cont_head.number += param_out.number;
            cont_head.trans_cnt++;

            if (param_out.is_finished)
            {
                //write content head info
                cont_head.is_enc = is_enc;
                error_code = WriteContentHead(
                    file_path, file_path_len, &cont_head);

                if (MMIIKB_STATUS_SUCCESS == error_code)
                {
                    //cal crc
                    if (IKeyBackup_IsCrc())
                    {
                        error_code = IKeyBackup_CalCrcItemData(
                            file_path, &crc);
                    }
                    
                    if (MMIIKB_STATUS_SUCCESS == error_code)
                    {
                        error_code = UpdateTaskTable(
                            type, MMIIKB_TASK_STATE_END, 
                            &cont_head.number, &cont_head.number, 
                            &cont_head.size, &cont_head.size, 
                            &cont_head.trans_cnt, &cont_head.trans_cnt, &crc);
                    }
                }                
                break;
            }
            else
            {
                //not yet
                param_in.len = cont_head.size;
                param_in.number = cont_head.number;
                param_in.begin = cont_head.size;
                param_in.trans_cnt = cont_head.trans_cnt;

                SCI_MEMSET(&param_out, 0x0, 
                    sizeof(MMIIKB_BACKUP_CALLBACK_PARAM_OUT_T));

                //go to while circle and get buf form app.
                error_code = UpdateTaskTable(
                    type, MMIIKB_TASK_STATE_PROCESSING, 
                    &cont_head.number, PNULL, 
                    &cont_head.size, PNULL, 
                    &cont_head.trans_cnt, PNULL, PNULL);
            }
        }
        else
        {
            //SCI_TRACE_LOW:"[MMIIKB] BackupItemDataSynByBuf encryp data error,type=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_TASK_2438_112_2_18_2_24_29_83,(uint8*)"d", type);
                
            SCI_FREE(buf_ptr);
            break;
        }

        //如果动作被取消，则本单元完成后，做一次检查动作
        if (IKeyBackup_GetCancelState())
        {
            //SCI_TRACE_LOW:"[MMIIKB] BackupItemDataSynByBuf cancel by user."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_TASK_2447_112_2_18_2_24_29_84,(uint8*)"");
            error_code = MMIIKB_STATUS_CANCEL_ACTION;
            break;
        }
    }
    
    //SCI_TRACE_LOW:"[MMIIKB] BackupItemDataSynByBuf end with type=%d,error_code=%d."
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_TASK_2454_112_2_18_2_24_29_85,(uint8*)"dd", type, error_code);
    
    return error_code;
}

/*****************************************************************************/
//  Description : BackupItemDataSynByFile
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
LOCAL MMIIKB_ERROR_E BackupItemDataSynByFile(
    const MMIIKB_TYPE_PROCESS_PARAM_T * process_param_ptr,
    MMIIKB_SIG_BACKUP_REQ_T * backup_req_ptr
)
{
    return MMIIKB_STATUS_INVALID_PARAM;
}

/*****************************************************************************/
//  Description : BackupItemDataSynByDir
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
LOCAL MMIIKB_ERROR_E BackupItemDataSynByDir(
    const MMIIKB_TYPE_PROCESS_PARAM_T * process_param_ptr,
    MMIIKB_SIG_BACKUP_REQ_T * backup_req_ptr
)
{
    return MMIIKB_STATUS_INVALID_PARAM;
}

/*****************************************************************************/
//  Description : BackupItemDataAsy
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
LOCAL MMIIKB_ERROR_E BackupItemDataAsy(
    const MMIIKB_TYPE_PROCESS_PARAM_T * process_param_ptr,
    MMIIKB_SIG_BACKUP_REQ_T * backup_req_ptr
)
{
    MMIIKB_ERROR_E error_code = MMIIKB_STATUS_INVALID_PARAM;
    MMIIKB_BACKUP_CALLBACK_PARAM_IN_T param_in = {0};
    MMIIKB_BACKUP_CALLBACK_PARAM_OUT_T param_out = {0};
    MMIIKB_TYPE_E type = MMIIKB_TYPE_MAX;
    const MMIIKB_TASK_SEQ_TABLE_T * task_seq_ptr = PNULL;
        
    if (PNULL == process_param_ptr
        || PNULL == backup_req_ptr)
    {
        //SCI_TRACE_LOW:"[MMIIKB] BackupItemDataAsy param is null."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_TASK_2501_112_2_18_2_24_29_86,(uint8*)"");
        return error_code;
    }

    type = process_param_ptr->type;
    task_seq_ptr = MMIIKB_GetTaskItemByType(type);
    if (PNULL == task_seq_ptr)
    {
        //SCI_TRACE_LOW:"[MMIIKB] BackupItemDataAsy task is null type=%d."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_TASK_2509_112_2_18_2_24_29_87,(uint8*)"d", type);
        
        error_code = MMIIKB_STATUS_NO_INIT_TASK;
        return error_code;
    }

    //set item total values
    SetTaskTableMaxValue(process_param_ptr);

    //init parma_in info, if op is file dir, then all below valus is zero.
    param_in.type = type;
    param_in.op_type = process_param_ptr->op_type;
    param_in.begin = task_seq_ptr->trans_size;
    param_in.len = task_seq_ptr->trans_size;
    param_in.number = task_seq_ptr->trans_number;
    param_in.trans_cnt = task_seq_ptr->trans_cnt;

    error_code = ReceiveUnitData(process_param_ptr, 
        &param_in, &param_out);

    if (MMIIKB_STATUS_SUCCESS == error_code
        && MMIIKB_STATUS_SUCCESS != param_out.error_code)
    {
        error_code = param_out.error_code;
    }
    
    return error_code;
}

/*****************************************************************************/
//  Description : BackupHandleItemUnitDataAsy
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
LOCAL MMIIKB_ERROR_E BackupHandleItemUnitDataAsy(
    MMIIKB_BACKUP_ITEM_DATA_RES_T * item_data_res_ptr,
    const MMIIKB_TASK_SEQ_TABLE_T * task_seq_ptr,
    const MMIIKB_TYPE_PROCESS_PARAM_T * process_param_ptr,
    MMIIKB_SIG_BACKUP_REQ_T * backup_req_ptr
)
{
    MMIIKB_ERROR_E error_code = MMIIKB_STATUS_INVALID_PARAM;

    if (PNULL == item_data_res_ptr
        || PNULL == task_seq_ptr
        || PNULL == process_param_ptr
        || PNULL == backup_req_ptr)
    {
        //SCI_TRACE_LOW:"[MMIIKB] BackupHandleItemUnitDataAsy param is null."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_TASK_2556_112_2_18_2_24_29_88,(uint8*)"");
        return error_code;
    }

    if (MMIIKB_OPERATE_TYPE_BUF == process_param_ptr->op_type)
    {
        error_code = BackupHandleUnitDataAsyByBuf(
            item_data_res_ptr, task_seq_ptr,process_param_ptr, backup_req_ptr);
    }
    else if (MMIIKB_OPERATE_TYPE_FILE == process_param_ptr->op_type)
    {
        error_code = BackupHandleUnitDataAsyByFile(
            item_data_res_ptr, task_seq_ptr,process_param_ptr, backup_req_ptr);
    }
    else if (MMIIKB_OPERATE_TYPE_DIR == process_param_ptr->op_type)
    {
        error_code = BackupHandleUnitDataAsyByDir(
            item_data_res_ptr, task_seq_ptr,process_param_ptr, backup_req_ptr);
    }
    
    return error_code;
}

/*****************************************************************************/
//  Description : BackupHandleUnitDataAsyByBuf
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
LOCAL MMIIKB_ERROR_E BackupHandleUnitDataAsyByBuf(
    MMIIKB_BACKUP_ITEM_DATA_RES_T * item_data_res_ptr,
    const MMIIKB_TASK_SEQ_TABLE_T * task_seq_ptr,
    const MMIIKB_TYPE_PROCESS_PARAM_T * process_param_ptr,
    MMIIKB_SIG_BACKUP_REQ_T * backup_req_ptr
)
{
    MMIIKB_ERROR_E error_code = MMIIKB_STATUS_INVALID_PARAM;
    MMIIKB_TYPE_E type = MMIIKB_TYPE_MAX;
    uint16 file_path[MMIIKB_CON_PATH_LEN] = {0};
    uint16 file_path_len = 0;
    void *buf_ptr = PNULL;
    MMIIKB_PACK_CONT_HEAD_T cont_head = {0};
    MMIIKB_PACK_CONT_UNIT_T cont_unit = {0};
    uint32 crc = 0;
    uint32 len = 0;
    BOOLEAN is_enc = FALSE;
    BOOLEAN is_cur_task_over = FALSE;

    if (PNULL == item_data_res_ptr
        || PNULL == task_seq_ptr
        || PNULL == process_param_ptr
        || PNULL == backup_req_ptr)
    {
        //SCI_TRACE_LOW:"[MMIIKB] BackupHandleUnitDataAsyByBuf param is null."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_TASK_2607_112_2_18_2_24_29_89,(uint8*)"");
        return error_code;
    }

    type = process_param_ptr->type;

    //get content file path
    SCI_MEMSET(file_path, 0x0, sizeof(file_path));
    error_code = GetContentFilePath(type, backup_req_ptr->backup_req.pack_dir, 
        file_path, &file_path_len);
    if (MMIIKB_STATUS_SUCCESS != error_code)
    {
        //SCI_TRACE_LOW:"[MMIIKB] BackupHandleUnitDataAsyByBuf can't get file path."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_TASK_2619_112_2_18_2_24_29_90,(uint8*)"");
        return error_code;
    }
    
    //init content head info    
    SCI_MEMSET(cont_head.content_name, 0x0, sizeof(cont_head.content_name));
    SetContentFileName(type, cont_head.content_name, 
        MMIIKB_PACK_CONTENT_NAME_LEN);
    cont_head.type = type;
    cont_head.op_type = MMIIKB_OPERATE_TYPE_BUF;
    cont_head.size = task_seq_ptr->trans_size;
    cont_head.number = task_seq_ptr->trans_number;
    cont_head.trans_cnt = task_seq_ptr->trans_cnt;
    
    if (PNULL == item_data_res_ptr->buf_ptr)
    {
        //SCI_TRACE_LOW:"[MMIIKB] BackupHandleUnitDataAsyByBuf app buf is null, type=%d."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_TASK_2636_112_2_18_2_24_29_91,(uint8*)"d", type);
            
        error_code = MMIIKB_STATUS_EMPTY_APP_BUF;
        return error_code;
    }

    len = item_data_res_ptr->len;
    buf_ptr = SCI_ALLOCA(len);
    if (PNULL == buf_ptr)
    {
        //SCI_TRACE_LOW:"[MMIIKB] BackupHandleUnitDataAsyByBuf memory is out. type=%d."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_TASK_2647_112_2_18_2_24_29_92,(uint8*)"d", type);
            
        error_code =  MMIIKB_STATUS_NO_MEMORY;
        return error_code;
    }
    SCI_MEMSET(buf_ptr, 0x00, len);
    SCI_MEMCPY(buf_ptr, item_data_res_ptr->buf_ptr, len);

    //encript buf
    is_enc = IKeyBackup_IsEnc(type);
    if (is_enc)
    {
        error_code = IKeyBackup_EncBuf((uint8*)buf_ptr, len);
    }
    else
    {
        error_code = MMIIKB_STATUS_SUCCESS;
    }

    if (MMIIKB_STATUS_SUCCESS == error_code)
    {
        //update unit content info
        cont_unit.len = len;
        cont_unit.number = item_data_res_ptr->number;
        cont_unit.reserved1 = IKEYBACKUP_DATA_END_SIG;
        cont_unit.reserved2 = IKEYBACKUP_DATA_END_SIG;
        cont_unit.buf_ptr = buf_ptr;
        cont_unit.end_sign1 = IKEYBACKUP_DATA_END_SIG;
        cont_unit.end_sign2 = IKEYBACKUP_DATA_END_SIG;
        
        //write unit content
        error_code = WriteUnitData(file_path, file_path_len, &cont_unit);
        if (MMIIKB_STATUS_SUCCESS == error_code)
        {
            //update content head info
            cont_head.size += cont_unit.len;
            cont_head.number += cont_unit.number;
            cont_head.trans_cnt++;

            if (item_data_res_ptr->is_finished)
            {
                //write content head info
                cont_head.is_enc = is_enc;
                error_code = WriteContentHead(
                    file_path, file_path_len, &cont_head);

                if (MMIIKB_STATUS_SUCCESS == error_code)
                {
                    //cal crc
                    if (IKeyBackup_IsCrc())
                    {
                        error_code = IKeyBackup_CalCrcItemData(
                            file_path, &crc);
                    }
                    if (MMIIKB_STATUS_SUCCESS == error_code)
                    {
                        error_code = UpdateTaskTable(
                            type, MMIIKB_TASK_STATE_END, 
                            &cont_head.number, &cont_head.number, 
                            &cont_head.size, &cont_head.size, 
                            &cont_head.trans_cnt, &cont_head.trans_cnt, &crc);
                    }
                }

                //不管成功与否，本次任务就都是结束了
                is_cur_task_over = TRUE;
            }
            else
            {
                error_code = UpdateTaskTable(
                    type, MMIIKB_TASK_STATE_MAX, 
                    &cont_head.number, PNULL, 
                    &cont_head.size, PNULL, 
                    &cont_head.trans_cnt, PNULL, PNULL);

                if (MMIIKB_STATUS_SUCCESS == error_code)
                {
                    error_code = BackupItemDataAsy(
                        process_param_ptr, backup_req_ptr);
                }
            }
        }
    }
    
    SCI_FREE(buf_ptr);
    
    
    //如果动作被取消，则本单元完成后，做一次检查动作
    if (IKeyBackup_GetCancelState())
    {
        //SCI_TRACE_LOW:"[MMIIKB] BackupHandleUnitDataAsyByBuf cancel by user."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_TASK_2737_112_2_18_2_24_30_93,(uint8*)"");
        error_code = MMIIKB_STATUS_CANCEL_ACTION;
    }
    
    //本次任务如完成，则进入任务循环表
    if (MMIIKB_STATUS_SUCCESS != error_code)
    {
        //SCI_TRACE_LOW:"[MMIIKB] BackupHandleUnitDataAsyByBuf end failed,type=%d, error_code=%d."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_TASK_2745_112_2_18_2_24_30_94,(uint8*)"dd", type, error_code);
            
        BackupResProcess(error_code, TRUE);
    }
    else
    {
        if (is_cur_task_over)
        {
            BackupCircleTask(backup_req_ptr);
        }
    }
        
    return error_code;
}

/*****************************************************************************/
//  Description : BackupHandleUnitDataAsyByFile
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
LOCAL MMIIKB_ERROR_E BackupHandleUnitDataAsyByFile(
    MMIIKB_BACKUP_ITEM_DATA_RES_T * item_data_res_ptr,
    const MMIIKB_TASK_SEQ_TABLE_T * task_seq_ptr,
    const MMIIKB_TYPE_PROCESS_PARAM_T * process_param_ptr,
    MMIIKB_SIG_BACKUP_REQ_T * backup_req_ptr
)
{
    return MMIIKB_STATUS_INVALID_PARAM;
}

/*****************************************************************************/
//  Description : BackupHandleUnitDataAsyByDir
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
LOCAL MMIIKB_ERROR_E BackupHandleUnitDataAsyByDir(
    MMIIKB_BACKUP_ITEM_DATA_RES_T * item_data_res_ptr,
    const MMIIKB_TASK_SEQ_TABLE_T * task_seq_ptr,
    const MMIIKB_TYPE_PROCESS_PARAM_T * process_param_ptr,
    MMIIKB_SIG_BACKUP_REQ_T * backup_req_ptr
)
{
    return MMIIKB_STATUS_INVALID_PARAM;
}


//restore item
/*****************************************************************************/
//  Description : IKBRestoreAllItems
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
LOCAL MMIIKB_ERROR_E IKBRestoreAllItems(xSignalHeaderRec *sig_ptr)
{
    MMIIKB_ERROR_E error_code = MMIIKB_STATUS_INVALID_PARAM;
    MMIIKB_SIG_RESTORE_REQ_T * restore_req_ptr = PNULL;

    if (PNULL == sig_ptr)
    {
        //SCI_TRACE_LOW:"[MMIIKB] IKBRestoreAllItems parma is null."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_TASK_2801_112_2_18_2_24_30_95,(uint8*)"");
        
        error_code = MMIIKB_STATUS_INVALID_PARAM;
        return error_code;
    }

    //1: copy signal info to static var
    if (PNULL != s_restore_req_ptr)
    {
        //not allowed if task is busy in restore last time
        //SCI_TRACE_LOW:"[MMIIKB] IKBRestoreAllItems s_restore_req_ptr is no-null."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_TASK_2811_112_2_18_2_24_30_96,(uint8*)"");
            
        error_code =  MMIIKB_STATUS_IKB_IS_BUSY;
        RestoreResProcess(error_code, TRUE);
        return error_code;
    }

    restore_req_ptr = (MMIIKB_SIG_RESTORE_REQ_T*)sig_ptr;
    s_restore_req_ptr = (MMIIKB_SIG_RESTORE_REQ_T*)SCI_ALLOCA(
        sizeof(MMIIKB_SIG_RESTORE_REQ_T));
    if (PNULL == s_restore_req_ptr)
    {
        //SCI_TRACE_LOW:"[MMIIKB] IKBRestoreAllItems memory1 is out."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_TASK_2823_112_2_18_2_24_30_97,(uint8*)"");
        
        error_code =  MMIIKB_STATUS_NO_MEMORY;
        return error_code;
    }
    SCI_MEMSET(s_restore_req_ptr, 0x00, sizeof(MMIIKB_SIG_RESTORE_REQ_T));    
    SCI_MEMCPY(s_restore_req_ptr, restore_req_ptr, 
        sizeof(MMIIKB_SIG_RESTORE_REQ_T));

    s_restore_req_ptr->restore_req.pack_info_ptr = 
        (MMIIKB_PACK_INFO_T*)SCI_ALLOCA(sizeof(MMIIKB_PACK_INFO_T));
    if (PNULL == s_restore_req_ptr->restore_req.pack_info_ptr)
    {
        //SCI_TRACE_LOW:"[MMIIKB] IKBRestoreAllItems memory2 is out."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_TASK_2836_112_2_18_2_24_30_98,(uint8*)"");
        error_code =  MMIIKB_STATUS_NO_MEMORY;
        RestoreResProcess(error_code, FALSE);
        return error_code;
    }
    SCI_MEMSET(s_restore_req_ptr->restore_req.pack_info_ptr, 0x00, 
        sizeof(MMIIKB_PACK_INFO_T));
    
    SCI_MEMCPY(s_restore_req_ptr->restore_req.pack_info_ptr, 
        restore_req_ptr->restore_req.pack_info_ptr, sizeof(MMIIKB_PACK_INFO_T));

    MMIAPIIKB_PrintRegTable();

    //2: init task sequences table
    InitTaskSeqTable(restore_req_ptr->restore_req.types,
        MMIIKB_TASK_TYPE_RESTORE);

    //3: restore circle tasks
    error_code = RestoreCircleTask(s_restore_req_ptr);

    //4: free sig_ptr's second pointer memory.
    SCI_FREE(restore_req_ptr->restore_req.pack_info_ptr);
    
    return error_code;
}

/*****************************************************************************/
//  Description : IKBRestoreItemUnitDataAsy
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
LOCAL MMIIKB_ERROR_E IKBRestoreItemUnitDataAsy(xSignalHeaderRec *sig_ptr)
{
    MMIIKB_ERROR_E error_code = MMIIKB_STATUS_INVALID_PARAM;
    MMIIKB_SIG_RESTORE_ITEM_DATA_RES_T *sig_item_data_ptr = PNULL;
    const MMIIKB_TASK_SEQ_TABLE_T *task_seq_ptr = PNULL;
    const MMIIKB_TYPE_PROCESS_PARAM_T *process_param_ptr = PNULL;
    MMIIKB_TYPE_E type = MMIIKB_TYPE_MAX;

    if (PNULL == sig_ptr)
    {
        error_code = MMIIKB_STATUS_INVALID_PARAM;
        RestoreResProcess(error_code, FALSE);
        if (PNULL != s_restore_req_ptr)
        {
            SCI_FREE(s_restore_req_ptr);
        }
        return error_code;
    }

    if (PNULL == s_restore_req_ptr)
    {
        error_code = MMIIKB_STATUS_NO_INIT_TASK;
        RestoreResProcess(error_code, FALSE);
        return error_code;
    }

    sig_item_data_ptr = (MMIIKB_SIG_RESTORE_ITEM_DATA_RES_T*)sig_ptr;
    type = sig_item_data_ptr->restore_item_data_res.type;
    error_code = sig_item_data_ptr->restore_item_data_res.error_code;
    
    if (MMIIKB_STATUS_SUCCESS == error_code)
    {        
        task_seq_ptr = MMIIKB_GetTaskItemByType(type);

        if (PNULL != task_seq_ptr)
        {
            process_param_ptr = MMIIKB_GetTypeProcessParamPtr(type);

            if (PNULL != process_param_ptr)
            {
                if (MMIIKB_TASK_TYPE_RESTORE == task_seq_ptr->task_type)
                {
                    if (process_param_ptr->is_restore_asyn)
                    {
                        error_code = RestoreHandleItemUnitDataAsy(
                            &(sig_item_data_ptr->restore_item_data_res),
                            task_seq_ptr, process_param_ptr, s_restore_req_ptr);
                    }
                    else
                    {
                        error_code = MMIIKB_STATUS_WRONG_TASK;
                    }
                }
                else
                {
                    error_code = MMIIKB_STATUS_WRONG_OP_TYPE;
                }
            }
            else
            {
                error_code = MMIIKB_STATUS_INVALID_PARAM;
            }
        }
        else
        {
            error_code = MMIIKB_STATUS_WRONG_TASK;
        }
    }

    /*异步的情况下，成功的可能就只有两种，（都是正常流程，不必操心）
    一是ITEM结束，在程序里会自动进入循环序列;
    二是ITEM未结束，但是seg写成功了，则会向应用发下一段seg
    失败的情况下：之前的逻辑则是直接退出，但是需求要求考虑下一项
    先置本ITEM任务失败，然后进任务循环列表
    */
    if (MMIIKB_STATUS_SUCCESS != error_code)
    {
        //RestoreResProcess(error_code, TRUE);
        UpdateTaskTable(type, MMIIKB_TASK_STATE_FAIL,
            PNULL, PNULL, PNULL, PNULL, PNULL, PNULL, PNULL);
        
        RestoreCircleTask(s_restore_req_ptr);
    }
    
    return error_code;
}

/*****************************************************************************/
//  Description : SetRestoreResParmater
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
LOCAL MMIIKB_ERROR_E SetRestoreResParmater(
    MMIIKB_SIG_RESTORE_RES_T *sig_restore_res_ptr
)
{
    MMIIKB_ERROR_E error_code = MMIIKB_STATUS_INVALID_PARAM;
    uint16 i = 0;
    MMIIKB_TYPE_E type = 0;
    
    if (PNULL == sig_restore_res_ptr)
    {
        return error_code;
    }

    for (i = 0; i < MMIIKB_TYPE_MAX; i++)
    {
        type = s_ikb_task_seq_table[i].type;
        if (MMIIKB_TYPE_MAX != type
            && MMIIKB_TASK_TYPE_RESTORE == s_ikb_task_seq_table[i].task_type)
        {
            if (MMIIKB_TASK_STATE_END == s_ikb_task_seq_table[i].task_state)
            {
                sig_restore_res_ptr->suc_types |= 
                    (0x01 << s_ikb_task_seq_table[i].type);
            }
            else
            {
                sig_restore_res_ptr->err_types |= 
                    (0x01 << s_ikb_task_seq_table[i].type);
            }
        }
    }

    error_code = MMIIKB_STATUS_SUCCESS;
    return error_code;    
}

/*****************************************************************************/
//  Description : RestoreCircleTask
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 本函数一定要保证只有一个出口
/*****************************************************************************/
LOCAL MMIIKB_ERROR_E RestoreCircleTask(
    MMIIKB_SIG_RESTORE_REQ_T * sig_restore_req_ptr
)
{
    MMIIKB_ERROR_E error_code = MMIIKB_STATUS_INVALID_PARAM;
    const MMIIKB_TASK_SEQ_TABLE_T *task_seq_ptr = PNULL;
    const MMIIKB_TYPE_PROCESS_PARAM_T *process_param_ptr = PNULL;
    BOOLEAN has_restore_item = FALSE;
    BOOLEAN is_asy_ret = FALSE;

    //SCI_ASSERT(PNULL != sig_restore_req_ptr);
    if (PNULL == sig_restore_req_ptr)
    {
        //这里理论上应该是永远不该跑的
        SCI_TRACE_LOW("[MMIIKB] RestoreCircleTask parma is null.");
        return error_code;
    }
        
    task_seq_ptr = GetCurTaskItem();

    while (PNULL != task_seq_ptr)
    {
        process_param_ptr = MMIIKB_GetTypeProcessParamPtr(task_seq_ptr->type);

        //SCI_TRACE_LOW:"[MMIIKB] RestoreCircleTask cur_type=%d, task_type=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_TASK_3016_112_2_18_2_24_30_99,(uint8*)"dd", task_seq_ptr->type, task_seq_ptr->task_type);

        #if IKEYBACKUP_TEST_CODE
        MMIAPIIKB_PrintRegTable();
        #endif
        
        if (MMIIKB_TASK_TYPE_RESTORE == task_seq_ptr->task_type)
        {
            has_restore_item = TRUE;
        
            if (!process_param_ptr->is_restore_asyn)
            {
                error_code = RestoreItemDataSyn(
                    process_param_ptr, sig_restore_req_ptr);

                //同步的还原，不管成功还是失败，总要检查下一项
                if (MMIIKB_STATUS_SUCCESS != error_code)
                {
                    //SCI_TRACE_LOW:"[MMIIKB] RestoreCircleTask restore syn type=%d fail."
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_TASK_3035_112_2_18_2_24_30_100,(uint8*)"d", task_seq_ptr->type);
                    
                    //set task table fail state.
                    UpdateTaskTable(task_seq_ptr->type, MMIIKB_TASK_STATE_FAIL,
                        PNULL, PNULL, PNULL, PNULL, PNULL, PNULL, PNULL);
                }
                else
                {
                    //SCI_TRACE_LOW:"[MMIIKB] RestoreCircleTask restore syn type=%d suc."
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_TASK_3044_112_2_18_2_24_30_101,(uint8*)"d", task_seq_ptr->type);
                }
            }
            else
            {
                error_code = RestoreItemDataAsy(
                    process_param_ptr, sig_restore_req_ptr);
                
                //异步还原，成功发出signal，则break;若错误，则应该检查下一项
                if (MMIIKB_STATUS_SUCCESS != error_code)
                {
                    //SCI_TRACE_LOW:"[MMIIKB] RestoreCircleTask restore asy type=%d fail."
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_TASK_3056_112_2_18_2_24_30_102,(uint8*)"d", task_seq_ptr->type);
                    
                    //set task table fail state.
                    UpdateTaskTable(task_seq_ptr->type, MMIIKB_TASK_STATE_FAIL,
                        PNULL, PNULL, PNULL, PNULL, PNULL, PNULL, PNULL);
                }
                else
                {
                    is_asy_ret = TRUE;
                    //SCI_TRACE_LOW:"[MMIIKB] RestoreCircleTask restore asy type=%d suc."
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_TASK_3066_112_2_18_2_24_30_103,(uint8*)"d", task_seq_ptr->type);
                    break;
                }
            }
        }
        
        task_seq_ptr = GetCurTaskItem();
    }
    
    //SCI_TRACE_LOW:"[MMIIKB] RestoreCircleTask end with error_code=%d,is_asy_ret=%d, has_restore_item=%d."
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_TASK_3076_112_2_18_2_24_31_104,(uint8*)"ddd", error_code, is_asy_ret, has_restore_item);

    //check task over
    if (!has_restore_item)
    {        
        error_code = MMIIKB_STATUS_SUCCESS;
        RestoreResProcess(error_code, TRUE);
    }
    else
    {
        if (MMIIKB_STATUS_SUCCESS == error_code)
        {
            if (!is_asy_ret && IsTaskOver())
            {
                RestoreResProcess(error_code, TRUE);
            }
        }
        else
        {
            //failed whether syn or asyn
            RestoreResProcess(error_code, TRUE);
        }
    }

    return error_code;
}

/*****************************************************************************/
//  Description : RestoreResProcess
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
LOCAL MMIIKB_ERROR_E RestoreResProcess(
    MMIIKB_ERROR_E param_error_code, 
    BOOLEAN is_free
)
{
    MMIIKB_ERROR_E error_code = MMIIKB_STATUS_INVALID_PARAM;
    MMIIKB_SIG_RESTORE_RES_T *sig_restore_res_ptr = PNULL;
    
    //1: response message sending to task
    sig_restore_res_ptr = SCI_ALLOCA(sizeof(MMIIKB_SIG_RESTORE_RES_T));
    if (PNULL == sig_restore_res_ptr)
    {
        //SCI_TRACE_LOW:"[MMIIKB] RestoreResProcess param is null."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_TASK_3119_112_2_18_2_24_31_105,(uint8*)"");
        return error_code;
    }
    
    SCI_MEMSET(sig_restore_res_ptr, 0x00, sizeof(MMIIKB_SIG_RESTORE_RES_T));
    
    sig_restore_res_ptr->SignalCode = MSG_IKB_RESTORE_DATA_RES;
    sig_restore_res_ptr->Sender = IKeyBackup_GetIKBTaskId();
    sig_restore_res_ptr->SignalSize = sizeof(MMIIKB_SIG_RESTORE_RES_T);

    sig_restore_res_ptr->error_code = param_error_code;
    //还原不管失败或者成功都需要对所有types进行提示，因此这里需要把types参数带上
    if (is_free)
    {
        SetRestoreResParmater(sig_restore_res_ptr);
    }

    //user cancel
    if (MMIIKB_STATUS_CANCEL_ACTION == param_error_code)
    {
        if (IKeyBackup_GetCancelState())
        {
            IKeyBackup_ActionSetOk();
        }
    }
    
    //SCI_TRACE_LOW:"[MMIIKB] RestoreResProcess send response to app."
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_TASK_3144_112_2_18_2_24_31_106,(uint8*)"");
    
    IKEYBACKUP_SEND_SIGNAL(sig_restore_res_ptr, P_APP);

    if (is_free)
    {
        if (PNULL != s_restore_req_ptr
            && PNULL != s_restore_req_ptr->restore_req.pack_info_ptr)
        {
            SCI_FREE(s_restore_req_ptr->restore_req.pack_info_ptr);
        }
        
        if (PNULL != s_restore_req_ptr)
        {
            SCI_FREE(s_restore_req_ptr);
        }

        //ResetTaskSeqTable();
    }
    
    error_code = MMIIKB_STATUS_SUCCESS;
    return error_code;
}

/*****************************************************************************/
//  Description : RestoreItemDataSyn
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
LOCAL MMIIKB_ERROR_E RestoreItemDataSyn(
    const MMIIKB_TYPE_PROCESS_PARAM_T * process_param_ptr,
    MMIIKB_SIG_RESTORE_REQ_T * restore_req_ptr
)
{
    MMIIKB_ERROR_E error_code = MMIIKB_STATUS_INVALID_PARAM;

    if (PNULL == process_param_ptr
        || PNULL == restore_req_ptr)
    {
        //SCI_TRACE_LOW:"[MMIIKB] RestoreItemDataSyn param is null."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_TASK_3182_112_2_18_2_24_31_107,(uint8*)"");
        return error_code;
    }

    if (MMIIKB_OPERATE_TYPE_BUF == process_param_ptr->op_type)
    {
        error_code = RestoreItemDataSynByBuf(
            process_param_ptr, restore_req_ptr);
    }
    else if (MMIIKB_OPERATE_TYPE_FILE == process_param_ptr->op_type)
    {
        error_code = RestoreItemDataSynByFile(
            process_param_ptr, restore_req_ptr);
    }
    else if (MMIIKB_OPERATE_TYPE_DIR == process_param_ptr->op_type)
    {
        error_code = RestoreItemDataSynByDir(
            process_param_ptr, restore_req_ptr);
    }
    
    return error_code;
}

/*****************************************************************************/
//  Description : RestoreItemDataSynByBuf
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
LOCAL MMIIKB_ERROR_E RestoreItemDataSynByBuf(
    const MMIIKB_TYPE_PROCESS_PARAM_T * process_param_ptr,
    MMIIKB_SIG_RESTORE_REQ_T * restore_req_ptr
)
{
    MMIIKB_ERROR_E error_code = MMIIKB_STATUS_INVALID_PARAM;
    MMIIKB_RESTORE_CALLBACK_PARAM_IN_T param_in = {0};
    MMIIKB_RESTORE_CALLBACK_PARAM_OUT_T param_out = {0};
    const MMIIKB_TASK_SEQ_TABLE_T * task_seq_ptr = PNULL;
    MMIIKB_TYPE_E type = MMIIKB_TYPE_MAX;
    uint16 file_path[MMIIKB_CON_PATH_LEN] = {0};
    uint16 file_path_len = 0;
    MMIFILE_HANDLE file_handle = SFS_INVALID_HANDLE;
    uint32 trans_cnt = 0;
    uint32 read_begin = 0;
    MMIIKB_PACK_CONT_HEAD_T cont_head = {0};
    MMIIKB_PACK_CONT_UNIT_T cont_unit = {0};

    if (PNULL == process_param_ptr
        || PNULL == restore_req_ptr)
    {
        //SCI_TRACE_LOW:"[MMIIKB] RestoreItemDataSynByBuf param is null."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_TASK_3230_112_2_18_2_24_31_108,(uint8*)"");
        return error_code;
    }

    type = process_param_ptr->type;
    
    //get content file path
    SCI_MEMSET(file_path, 0x0, sizeof(file_path));
    error_code = GetContentFilePath(type, 
        restore_req_ptr->restore_req.pack_info_ptr->pack_dir, 
        file_path, &file_path_len);
    if (MMIIKB_STATUS_SUCCESS != error_code)
    {
        //SCI_TRACE_LOW:"[MMIIKB] RestoreItemDataSynByBuf can't get file path."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_TASK_3243_112_2_18_2_24_31_109,(uint8*)"");
        return error_code;
    }

    //check file exist
    if (!MMIAPIFMM_IsFileExist(file_path, file_path_len))
    {
        //SCI_TRACE_LOW:"[MMIIKB] RestoreItemDataSynByBuf file is not exist."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_TASK_3250_112_2_18_2_24_31_110,(uint8*)"");
        return MMIIKB_STATUS_CREATE_FILE_FAILED;
    }

    //check crc
    error_code = IKeyBackup_IsCrcRight(file_path, 
        restore_req_ptr->restore_req.pack_info_ptr->crcs[type]);
    if (MMIIKB_STATUS_SUCCESS != error_code)
    {
        return error_code;
    }
    
    //read content head info
    error_code = ReadContentHead(file_path, file_path_len, &cont_head);
    if (MMIIKB_STATUS_SUCCESS != error_code)
    {
        return error_code;
    }

    //update task table
    error_code = UpdateTaskTable(
        type, MMIIKB_TASK_STATE_PROCESSING, PNULL, &cont_head.number,
        PNULL, &cont_head.size, PNULL, &cont_head.trans_cnt, PNULL);
    if (MMIIKB_STATUS_SUCCESS != error_code)
    {
        return error_code;
    }
    
    //restore content
    file_handle = MMIAPIFMM_CreateFile(file_path, 
        SFS_MODE_READ|SFS_MODE_OPEN_EXISTING, 0, 0);
    if (SFS_INVALID_HANDLE == file_handle)
    {
        error_code = MMIIKB_STATUS_CREATE_FILE_FAILED;
    }
    else
    {
        task_seq_ptr = MMIIKB_GetTaskItemByType(type);
        if (PNULL == task_seq_ptr)
        {
            //SCI_TRACE_LOW:"[MMIIKB] RestoreItemDataSynByBuf task null, type=%d."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_TASK_3291_112_2_18_2_24_31_111,(uint8*)"d",type);
            
            MMIAPIFMM_CloseFile(file_handle);
            error_code = MMIIKB_STATUS_NO_INIT_TASK;
            return error_code;
        }
        
        trans_cnt = cont_head.trans_cnt;
        read_begin = sizeof(MMIIKB_PACK_CONT_HEAD_T);    
        
        while (trans_cnt > 0)
        {
            uint32 unit_begin = 0;
            uint32 trans_idx = cont_head.trans_cnt - trans_cnt;
            
            error_code = ReadUnitData(file_handle, read_begin, &cont_unit);

            if (MMIIKB_STATUS_SUCCESS == error_code)
            {
                //encript buf
                if (cont_head.is_enc)
                {
                    error_code = IKeyBackup_DecBuf(
                        (uint8*)(cont_unit.buf_ptr), cont_unit.len);
                }
                else
                {
                    error_code = MMIIKB_STATUS_SUCCESS;
                }

                if (MMIIKB_STATUS_SUCCESS == error_code)
                {
                    //init parma_in info
                    param_in.type = type;
                    param_in.op_type = MMIIKB_OPERATE_TYPE_BUF;
                    param_in.buf_ptr = cont_unit.buf_ptr;
                    param_in.begin = unit_begin;
                    param_in.len = cont_unit.len;
                    param_in.number = cont_unit.number;
                    param_in.trans_idx = trans_idx;
            
                    error_code = SendUnitData(process_param_ptr, 
                        &param_in, &param_out);

                    if (MMIIKB_STATUS_SUCCESS == param_out.error_code)
                    {
                        uint32 task_number = 
                            task_seq_ptr->trans_number + param_out.suc_cnt;
                        uint32 task_size = 
                            task_seq_ptr->trans_size + param_in.len;
                        uint32 task_cnt = task_seq_ptr->trans_cnt + 1;
                        
                        if (1 == trans_cnt)
                        {
                            //last seg
                            error_code = UpdateTaskTable(
                                type, MMIIKB_TASK_STATE_END,
                                &task_number, PNULL,
                                &task_size, PNULL,
                                &task_cnt, PNULL,
                                PNULL);
                        }
                        else
                        {
                            error_code = UpdateTaskTable(
                                type, MMIIKB_TASK_STATE_MAX,
                                &task_number, PNULL,
                                &task_size, PNULL,
                                &task_cnt, PNULL,
                                PNULL);
                        }
                    }
                    else
                    {
                        error_code = param_out.error_code;
                        //SCI_TRACE_LOW:"[MMIIKB] RestoreItemDataSynByBuf send unitdata error, restore error by type=%d, error=%dtrans_idx=%d, begin=%d, len=%d"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_TASK_3369_112_2_18_2_24_31_112,(uint8*)"ddddd", type, error_code,trans_idx, param_in.begin, param_in.len);
                    }
                }
            }
            
            if (PNULL != cont_unit.buf_ptr)
            {
                SCI_FREE(cont_unit.buf_ptr);
            }         

            if (MMIIKB_STATUS_SUCCESS == error_code)
            {
                unit_begin += cont_unit.len;
                read_begin += (sizeof(MMIIKB_PACK_CONT_UNIT_T)
                    - sizeof(cont_unit.buf_ptr) + (cont_unit.len));
                SCI_MEMSET(&cont_unit, 0x0, sizeof(MMIIKB_PACK_CONT_UNIT_T));
                trans_cnt--;
            }

            //如果动作被取消，则本单元完成后，做一次检查动作
            if (IKeyBackup_GetCancelState())
            {
                //SCI_TRACE_LOW:"[MMIIKB] BackupItemDataSynByBuf cancel by user."
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_TASK_3391_112_2_18_2_24_31_113,(uint8*)"");
                error_code = MMIIKB_STATUS_CANCEL_ACTION;
                break;
            }

            if (MMIIKB_STATUS_SUCCESS != error_code)
            {
                break;
            }
        }
    }
    
    MMIAPIFMM_CloseFile(file_handle);
    
    //SCI_TRACE_LOW:"[MMIIKB] RestoreItemDataSynByBuf end with type=%d,error_code=%d."
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_TASK_3406_112_2_18_2_24_31_114,(uint8*)"dd", type, error_code);
    
    return error_code;
}

/*****************************************************************************/
//  Description : RestoreItemDataSynByFile
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
LOCAL MMIIKB_ERROR_E RestoreItemDataSynByFile(
    const MMIIKB_TYPE_PROCESS_PARAM_T * process_param_ptr,
    MMIIKB_SIG_RESTORE_REQ_T * restore_req_ptr
)
{
    return MMIIKB_STATUS_INVALID_PARAM;
}

/*****************************************************************************/
//  Description : RestoreItemDataSynByDir
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
LOCAL MMIIKB_ERROR_E RestoreItemDataSynByDir(
    const MMIIKB_TYPE_PROCESS_PARAM_T * process_param_ptr,
    MMIIKB_SIG_RESTORE_REQ_T * restore_req_ptr
)
{
    return MMIIKB_STATUS_INVALID_PARAM;
}

/*****************************************************************************/
//  Description : RestoreItemDataAsy
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
LOCAL MMIIKB_ERROR_E RestoreItemDataAsy(
    const MMIIKB_TYPE_PROCESS_PARAM_T * process_param_ptr,
    MMIIKB_SIG_RESTORE_REQ_T * restore_req_ptr
)
{
    MMIIKB_ERROR_E error_code = MMIIKB_STATUS_INVALID_PARAM;
    MMIIKB_TYPE_E type = MMIIKB_TYPE_MAX;
    const MMIIKB_TASK_SEQ_TABLE_T * task_seq_ptr = PNULL;    
    uint16 file_path[MMIIKB_CON_PATH_LEN] = {0};
    uint16 file_path_len = 0;
    MMIIKB_PACK_CONT_HEAD_T cont_head = {0};
    
    if (PNULL == process_param_ptr
        || PNULL == restore_req_ptr)
    {
        //SCI_TRACE_LOW:"[MMIIKB] RestoreItemDataAsy param is null."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_TASK_3454_112_2_18_2_24_31_115,(uint8*)"");
        
        return error_code;
    }

    type = process_param_ptr->type;
    task_seq_ptr = MMIIKB_GetTaskItemByType(type);
    if (PNULL == task_seq_ptr)
    {
        //SCI_TRACE_LOW:"[MMIIKB] RestoreItemDataAsy task null, type=%d."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_TASK_3463_112_2_18_2_24_31_116,(uint8*)"d", type);
        
        error_code = MMIIKB_STATUS_NO_INIT_TASK;
        return error_code;
    }
    
    //get content file path
    SCI_MEMSET(file_path, 0x0, sizeof(file_path));
    error_code = GetContentFilePath(type, 
        restore_req_ptr->restore_req.pack_info_ptr->pack_dir, 
        file_path, &file_path_len);
    if (MMIIKB_STATUS_SUCCESS != error_code)
    {
        //SCI_TRACE_LOW:"[MMIIKB] RestoreItemDataAsy can't get file path."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_TASK_3476_112_2_18_2_24_31_117,(uint8*)"");
        return error_code;
    }

    //check file exist
    if (!MMIAPIFMM_IsFileExist(file_path, file_path_len))
    {
        //SCI_TRACE_LOW:"[MMIIKB] RestoreItemDataAsy file is not exist."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_TASK_3483_112_2_18_2_24_31_118,(uint8*)"");
        return MMIIKB_STATUS_CREATE_FILE_FAILED;
    }

    //check crc
    error_code = IKeyBackup_IsCrcRight(file_path, 
        restore_req_ptr->restore_req.pack_info_ptr->crcs[type]);
    if (MMIIKB_STATUS_SUCCESS != error_code)
    {
        return error_code;
    }
    
    //read content head info
    error_code = ReadContentHead(file_path, file_path_len, &cont_head);
    if (MMIIKB_STATUS_SUCCESS != error_code)
    {
        return error_code;
    }

    //update task table
    error_code = UpdateTaskTable(
        type, MMIIKB_TASK_STATE_PROCESSING, PNULL, &cont_head.number,
        PNULL, &cont_head.size, PNULL, &cont_head.trans_cnt, PNULL);
    if (MMIIKB_STATUS_SUCCESS != error_code)
    {
        return error_code;
    }

    //restore content
    error_code = SendUnitDataCircle(file_path, task_seq_ptr, 
        process_param_ptr, &cont_head);
    
    return error_code;
}

/*****************************************************************************/
//  Description : SendUnitDataCircle
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
LOCAL MMIIKB_ERROR_E SendUnitDataCircle(
    uint16 * file_path_ptr,
    const MMIIKB_TASK_SEQ_TABLE_T * task_seq_ptr,
    const MMIIKB_TYPE_PROCESS_PARAM_T * process_param_ptr,
    const MMIIKB_PACK_CONT_HEAD_T *cont_head_ptr
)
{
    MMIIKB_ERROR_E error_code = MMIIKB_STATUS_INVALID_PARAM;
    MMIIKB_RESTORE_CALLBACK_PARAM_IN_T param_in = {0};
    MMIIKB_RESTORE_CALLBACK_PARAM_OUT_T param_out = {0};
    MMIIKB_TYPE_E type = MMIIKB_TYPE_MAX;
    MMIFILE_HANDLE file_handle = SFS_INVALID_HANDLE;
    uint32 read_begin = 0;
    MMIIKB_PACK_CONT_UNIT_T cont_unit = {0};
    
    if (PNULL == file_path_ptr
        || PNULL == task_seq_ptr
        || PNULL == process_param_ptr
        || PNULL == cont_head_ptr)
    {
        //SCI_TRACE_LOW:"[MMIIKB] SendUnitDataCircle param is null."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_TASK_3542_112_2_18_2_24_31_119,(uint8*)"");
        
        return error_code;
    }

    type = task_seq_ptr->type;
    
    file_handle = MMIAPIFMM_CreateFile(file_path_ptr, 
        SFS_MODE_READ|SFS_MODE_OPEN_EXISTING, 0, 0);
    if (SFS_INVALID_HANDLE == file_handle)
    {
        error_code = MMIIKB_STATUS_CREATE_FILE_FAILED;
    }
    else
    {
        uint32 unit_begin = 0;
        uint32 trans_idx = task_seq_ptr->trans_cnt;        
        read_begin = sizeof(MMIIKB_PACK_CONT_HEAD_T) 
            + (uint32)(task_seq_ptr->trans_size) + 
            (uint32)((sizeof(MMIIKB_PACK_CONT_UNIT_T) 
            - sizeof(cont_unit.buf_ptr)) * trans_idx);
        
        error_code = ReadUnitData(file_handle, read_begin, &cont_unit);

        if (MMIIKB_STATUS_SUCCESS == error_code)
        {
            //encript buf
            if (cont_head_ptr->is_enc)
            {
                error_code = IKeyBackup_DecBuf(
                    (uint8*)(cont_unit.buf_ptr), cont_unit.len);
            }
            else
            {
                error_code = MMIIKB_STATUS_SUCCESS;
            }

            if (MMIIKB_STATUS_SUCCESS == error_code)
            {
                //init parma_in info
                param_in.type = type;
                param_in.op_type = MMIIKB_OPERATE_TYPE_BUF;
                param_in.buf_ptr = cont_unit.buf_ptr;
                param_in.begin = unit_begin;
                param_in.len = cont_unit.len;
                param_in.number = cont_unit.number;
                param_in.trans_idx = trans_idx;
        
                error_code = SendUnitData(process_param_ptr, 
                    &param_in, &param_out);
            }
        }
            
        if (PNULL != cont_unit.buf_ptr)
        {
            SCI_FREE(cont_unit.buf_ptr);
        }
    }
    
    MMIAPIFMM_CloseFile(file_handle);
    
    return error_code;
}

/*****************************************************************************/
//  Description : RestoreHandleItemUnitDataAsy
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
LOCAL MMIIKB_ERROR_E RestoreHandleItemUnitDataAsy(
    MMIIKB_RESTORE_ITEM_DATA_RES_T * item_data_res_ptr,
    const MMIIKB_TASK_SEQ_TABLE_T * task_seq_ptr,
    const MMIIKB_TYPE_PROCESS_PARAM_T * process_param_ptr,
    MMIIKB_SIG_RESTORE_REQ_T * restore_req_ptr
)
{
    MMIIKB_ERROR_E error_code = MMIIKB_STATUS_INVALID_PARAM;

    if (PNULL == item_data_res_ptr
        || PNULL == task_seq_ptr
        || PNULL == process_param_ptr
        || PNULL == restore_req_ptr)
    {
        //SCI_TRACE_LOW:"[MMIIKB] RestoreHandleItemUnitDataAsy param is null."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_TASK_3624_112_2_18_2_24_32_120,(uint8*)"");
        
        return error_code;
    }

    if (MMIIKB_OPERATE_TYPE_BUF == process_param_ptr->op_type)
    {
        error_code = RestoreHandleUnitDataAsyByBuf(
            item_data_res_ptr, task_seq_ptr,process_param_ptr, restore_req_ptr);
    }
    else if (MMIIKB_OPERATE_TYPE_FILE == process_param_ptr->op_type)
    {
        error_code = RestoreHandleUnitDataAsyByFile(
            item_data_res_ptr, task_seq_ptr,process_param_ptr, restore_req_ptr);
    }
    else if (MMIIKB_OPERATE_TYPE_DIR == process_param_ptr->op_type)
    {
        error_code = RestoreHandleUnitDataAsyByDir(
            item_data_res_ptr, task_seq_ptr,process_param_ptr, restore_req_ptr);
    }
    
    return error_code;
}

/*****************************************************************************/
//  Description : RestoreHandleUnitDataAsyByBuf
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
LOCAL MMIIKB_ERROR_E RestoreHandleUnitDataAsyByBuf(
    MMIIKB_RESTORE_ITEM_DATA_RES_T * item_data_res_ptr,
    const MMIIKB_TASK_SEQ_TABLE_T * task_seq_ptr,
    const MMIIKB_TYPE_PROCESS_PARAM_T * process_param_ptr,
    MMIIKB_SIG_RESTORE_REQ_T * restore_req_ptr
)
{
    MMIIKB_ERROR_E error_code = MMIIKB_STATUS_INVALID_PARAM;
    MMIIKB_TYPE_E type = MMIIKB_TYPE_MAX;
    uint16 file_path[MMIIKB_CON_PATH_LEN] = {0};
    uint16 file_path_len = 0;

    if (PNULL == item_data_res_ptr
        || PNULL == task_seq_ptr
        || PNULL == process_param_ptr
        || PNULL == restore_req_ptr)
    {
        //SCI_TRACE_LOW:"[MMIIKB] RestoreHandleUnitDataAsyByBuf param is null."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_TASK_3669_112_2_18_2_24_32_121,(uint8*)"");
        
        return error_code;
    }

    type = process_param_ptr->type;
    
    //get content file path
    SCI_MEMSET(file_path, 0x0, sizeof(file_path));
    error_code = GetContentFilePath(type, 
        restore_req_ptr->restore_req.pack_info_ptr->pack_dir, 
        file_path, &file_path_len);
    if (MMIIKB_STATUS_SUCCESS != error_code)
    {
        //SCI_TRACE_LOW:"[MMIIKB] RestoreHandleUnitDataAsyByBuf can't get file path."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_TASK_3683_112_2_18_2_24_32_122,(uint8*)"");
        return error_code;
    }

    //check file exist
    if (!MMIAPIFMM_IsFileExist(file_path, file_path_len))
    {
        //SCI_TRACE_LOW:"[MMIIKB] RestoreHandleUnitDataAsyByBuf file is not exist."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_TASK_3690_112_2_18_2_24_32_123,(uint8*)"");
        return MMIIKB_STATUS_CREATE_FILE_FAILED;
    }

    //update task table
    if (task_seq_ptr->trans_cnt + 1 == task_seq_ptr->total_cnt)
    {
        //finish
        error_code = UpdateTaskTable(
            type, MMIIKB_TASK_STATE_END,
            &(task_seq_ptr->total_number), PNULL,
            &(task_seq_ptr->total_size), PNULL,
            &(task_seq_ptr->total_cnt), PNULL, PNULL);

        //SCI_TRACE_LOW:"[MMIIKB] RestoreHandleUnitDataAsyByBuf this item is end,type=%d, error_code=%d."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_TASK_3705_112_2_18_2_24_32_124,(uint8*)"dd", type, error_code);
        
        if (MMIIKB_STATUS_SUCCESS == error_code)
        {
            RestoreCircleTask(restore_req_ptr);
        }
    }
    else
    {
        //not finish        
        MMIFILE_HANDLE file_handle = SFS_INVALID_HANDLE;
        MMIFILE_ERROR_E file_error = SFS_ERROR_NONE;
        MMIIKB_PACK_CONT_HEAD_T cont_head = {0};
        MMIIKB_PACK_CONT_UNIT_T cont_unit = {0};
        uint32 trans_idx = 0;
        uint32 read_begin = 0;
        uint32 read_len = 0;
    
        file_handle = MMIAPIFMM_CreateFile(file_path,
            SFS_MODE_READ|SFS_MODE_OPEN_EXISTING, 0, 0);
        if (SFS_INVALID_HANDLE != file_handle)
        {
            file_error = MMIAPIFMM_ReadFile(file_handle, &cont_head,
                sizeof(MMIIKB_PACK_CONT_HEAD_T), &read_len, PNULL);
            if (SFS_ERROR_NONE == file_error 
                    && read_len == sizeof(MMIIKB_PACK_CONT_HEAD_T))
            {
                error_code = MMIIKB_STATUS_SUCCESS;
            }
            else
            {
                error_code = MMIIKB_STATUS_FILE_ERROR;
            }
            
            if (MMIIKB_STATUS_SUCCESS == error_code)
            {
                trans_idx = task_seq_ptr->trans_cnt;
                read_begin = sizeof(MMIIKB_PACK_CONT_HEAD_T)
                    + (uint32)(task_seq_ptr->trans_size) +
                    (uint32)((sizeof(MMIIKB_PACK_CONT_UNIT_T)
                    - sizeof(cont_unit.buf_ptr)) * trans_idx);
                error_code = ReadUnitData(file_handle, read_begin, &cont_unit);
                if (PNULL != cont_unit.buf_ptr)
                {
                    SCI_FREE(cont_unit.buf_ptr);
                }
                
                if (MMIIKB_STATUS_SUCCESS == error_code)
                {
                    uint32 task_number = 
                        task_seq_ptr->trans_number + cont_unit.number;
                    uint32 task_size = 
                        task_seq_ptr->trans_size + cont_unit.len;
                    uint32 task_cnt = task_seq_ptr->trans_cnt + 1;
                    
                    error_code = UpdateTaskTable(
                        type, MMIIKB_TASK_STATE_PROCESSING,
                        &task_number, PNULL,
                        &task_size, PNULL,
                        &task_cnt, PNULL, PNULL);
                }
            }
        }
        else
        {
            error_code = MMIIKB_STATUS_CREATE_FILE_FAILED;
        }
        MMIAPIFMM_CloseFile(file_handle);
        
        //SCI_TRACE_LOW:"[MMIIKB] RestoreHandleUnitDataAsyByBuf this uint is end,type=%d, error_code=%d."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_TASK_3775_112_2_18_2_24_32_125,(uint8*)"dd", type, error_code);
    
    
        //如果动作被取消，则本单元完成后，做一次检查动作
        if (IKeyBackup_GetCancelState())
        {
            //SCI_TRACE_LOW:"[MMIIKB] RestoreHandleUnitDataAsyByBuf cancel by user."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_TASK_3781_112_2_18_2_24_32_126,(uint8*)"");
            error_code = MMIIKB_STATUS_CANCEL_ACTION;
        }

        if (MMIIKB_STATUS_SUCCESS == error_code)
        {
            error_code = SendUnitDataCircle(file_path,
                task_seq_ptr, process_param_ptr, &cont_head);
        }
    }

    return error_code;
}

/*****************************************************************************/
//  Description : RestoreHandleUnitDataAsyByFile
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
LOCAL MMIIKB_ERROR_E RestoreHandleUnitDataAsyByFile(
    MMIIKB_RESTORE_ITEM_DATA_RES_T * item_data_res_ptr,
    const MMIIKB_TASK_SEQ_TABLE_T * task_seq_ptr,
    const MMIIKB_TYPE_PROCESS_PARAM_T * process_param_ptr,
    MMIIKB_SIG_RESTORE_REQ_T * restore_req_ptr
)
{
    return MMIIKB_STATUS_INVALID_PARAM;
}

/*****************************************************************************/
//  Description : RestoreHandleUnitDataAsyByDir
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
LOCAL MMIIKB_ERROR_E RestoreHandleUnitDataAsyByDir(
    MMIIKB_RESTORE_ITEM_DATA_RES_T * item_data_res_ptr,
    const MMIIKB_TASK_SEQ_TABLE_T * task_seq_ptr,
    const MMIIKB_TYPE_PROCESS_PARAM_T * process_param_ptr,
    MMIIKB_SIG_RESTORE_REQ_T * restore_req_ptr
)
{
    return MMIIKB_STATUS_INVALID_PARAM;
}



//cacel process
/*****************************************************************************/
//  Description : IKBCancelAction
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
LOCAL MMIIKB_ERROR_E IKBCancelAction(xSignalHeaderRec *sig_ptr)
{
    MMIIKB_ERROR_E error_code = MMIIKB_STATUS_INVALID_PARAM;
    MMIIKB_SIG_CANCEL_REQ_T *sig_cancel_req_ptr = PNULL;
    MMIIKB_SIG_CANCEL_RES_T *sig_cancel_res_ptr = PNULL;

    if (PNULL == sig_ptr)
    {
        //SCI_TRACE_LOW:"[MMIIKB] IKBCancelAction param is null."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_TASK_3838_112_2_18_2_24_32_127,(uint8*)"");
            
        error_code = MMIIKB_STATUS_INVALID_PARAM;
        return error_code;
    }

    sig_cancel_req_ptr = (MMIIKB_SIG_CANCEL_REQ_T*)sig_ptr;
    
    //set install ok
    if (IKeyBackup_GetCancelState())
    {
        IKeyBackup_ActionSetOk();
    }
    
    sig_cancel_res_ptr = SCI_ALLOCA(sizeof(MMIIKB_SIG_CANCEL_RES_T));
    if (PNULL == sig_cancel_res_ptr)
    {
        //SCI_TRACE_LOW:"[MMIIKB] IKBCancelAction memory is out."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_TASK_3855_112_2_18_2_24_32_128,(uint8*)"");
        error_code = MMIIKB_STATUS_NO_MEMORY;
        return error_code;
    }
    SCI_MEMSET(sig_cancel_res_ptr, 0x00, sizeof(MMIIKB_SIG_CANCEL_RES_T));
    
    sig_cancel_res_ptr->task_type = sig_cancel_req_ptr->task_type;

    switch (sig_cancel_req_ptr->task_type)
    {
    case MMIIKB_TASK_TYPE_BACKUP:
        {
            if (PNULL != s_cancel_pack_path_ptr)
            {
                SCI_MEMCPY(sig_cancel_res_ptr->pack_dir,
                    s_cancel_pack_path_ptr, 
                    sizeof(sig_cancel_res_ptr->pack_dir));
                
                error_code = MMIIKB_STATUS_SUCCESS;
            }
        }
        break;
    
    case MMIIKB_TASK_TYPE_RESTORE:
        error_code = MMIIKB_STATUS_SUCCESS;
        break;
        
    default:
        break;
    }
    
    sig_cancel_res_ptr->SignalCode = MSG_IKB_CANCEL_ACTION_RES;
    sig_cancel_res_ptr->Sender = IKeyBackup_GetIKBTaskId();
    sig_cancel_res_ptr->SignalSize = sizeof(MMIIKB_SIG_CANCEL_RES_T);

    sig_cancel_res_ptr->error_code = error_code;
    
    //SCI_TRACE_LOW:"[MMIIKB] IKBCancelAction task_type=%d, error_code =%dn"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_TASK_3892_112_2_18_2_24_32_129,(uint8*)"dd",sig_cancel_req_ptr->task_type, error_code);
    
    IKEYBACKUP_SEND_SIGNAL(sig_cancel_res_ptr, P_APP);
    
    return error_code;
}

//time-out protection mechanism
/*****************************************************************************/
//  Description : CreateProtectionTimer
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 创建超时保护定时器
/*****************************************************************************/
LOCAL void CreateProtectionTimer()
{
    //SCI_TRACE_LOW:"[MMIIKB] CreateProtectionTimer create timer, begin on=%d."
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_TASK_3907_112_2_18_2_24_32_130,(uint8*)"d", SCI_GetTickCount());
        
    StopProtectionTimer();
    
    s_ikb_protect_timer = SCI_CreateTimer("IKeyBackup_Timer", 
        ProcessProtectionTimerCB, 0, IKEYBACKUP_TIMER_OUT, SCI_AUTO_ACTIVATE);      
    
    return;
}

/******************************************************************************/
// Description: StopProtectionTimer
// Global resource dependence: NONE
// Author: haiwu.chen
// Note: 停止超时保护定时器
/******************************************************************************/
LOCAL void StopProtectionTimer()
{
    //SCI_TRACE_LOW:"[MMIIKB] StopProtectionTimer stop time, end on=%d."
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_TASK_3924_112_2_18_2_24_32_131,(uint8*)"d", SCI_GetTickCount());
    
    s_is_timeout = FALSE;
    
    if (SCI_IsTimerActive(s_ikb_protect_timer))
    {
        SCI_DeactiveTimer(s_ikb_protect_timer);
        SCI_DeleteTimer(s_ikb_protect_timer);
    }
    s_ikb_protect_timer = PNULL;
    
    return;
}

/******************************************************************************/
// Description: ProcessProtectionTimerCB
// Global resource dependence: NONE
// Author: haiwu.chen
// Note: 超时保护机制
/******************************************************************************/
LOCAL void ProcessProtectionTimerCB(uint32 param)
{
    //SCI_TRACE_LOW:"[MMIIKB] ProcessProtectionTimerCB time out, end on=%d."
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_TASK_3945_112_2_18_2_24_32_132,(uint8*)"d", SCI_GetTickCount());
    
    s_is_timeout = TRUE;
    
    if (IKeyBackup_IsBackupBusy())
    {
        //SCI_TRACE_LOW:"[MMIIKB] ProcessProtectionTimerCB backup time out."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_TASK_3951_112_2_18_2_24_32_133,(uint8*)"");
        BackupResProcess(MMIIKB_STATUS_BACKUP_TIME_OUT, TRUE);
    }
    else if (IKeyBackup_IsRestoreBusy())
    {
        //SCI_TRACE_LOW:"[MMIIKB] ProcessProtectionTimerCB restore time out."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_TASK_3956_112_2_18_2_24_32_134,(uint8*)"");
        RestoreResProcess(MMIIKB_STATUS_RESTORE_TIME_OUT, TRUE);
    }
    else
    {
        //SCI_TRACE_LOW:"[MMIIKB] ProcessProtectionTimerCB all task is unbusy."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_TASK_3961_112_2_18_2_24_32_135,(uint8*)"");
    }

    return;
}


//file process
/*****************************************************************************/
//  Description : GetContentFilePath
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
LOCAL MMIIKB_ERROR_E GetContentFilePath(
    MMIIKB_TYPE_E type, 
    uint16 *pack_dir_ptr,
    uint16 *file_path_ptr,              //out
    uint16 *file_path_len_ptr           //out
)
{
    MMIIKB_ERROR_E error_code = MMIIKB_STATUS_INVALID_PARAM;
    uint16 content_name[MMIIKB_PACK_CONTENT_NAME_LEN] = {0};

    if (MMIIKB_TYPE_MAX <= type
        || PNULL == pack_dir_ptr
        || PNULL == file_path_ptr
        || PNULL == file_path_len_ptr)
    {
        //SCI_TRACE_LOW:"[MMIIKB] GetContentFilePath param is null."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_TASK_3988_112_2_18_2_24_32_136,(uint8*)"");
        return error_code;
    }
    
    //get content head name
    SCI_MEMSET(content_name, 0x0, sizeof(content_name));
    error_code = SetContentFileName(type, content_name, 
        MMIIKB_PACK_CONTENT_NAME_LEN);
    
    if (MMIIKB_STATUS_SUCCESS == error_code)
    {
        //get full path
        MMIAPICOM_Wstrcpy(file_path_ptr, pack_dir_ptr);
        MMIAPICOM_Wstrcat(file_path_ptr, L"\\");
        MMIAPICOM_Wstrcat(file_path_ptr, content_name);
        *file_path_len_ptr = MMIAPICOM_Wstrlen(file_path_ptr);
    }
    
    return error_code;
}

/*****************************************************************************/
//  Description : GetUnitData
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
LOCAL MMIIKB_ERROR_E ReceiveUnitData(
    const MMIIKB_TYPE_PROCESS_PARAM_T * process_param_ptr,
    MMIIKB_BACKUP_CALLBACK_PARAM_IN_T *param_in_ptr,
    MMIIKB_BACKUP_CALLBACK_PARAM_OUT_T *param_out_ptr
)
{
    MMIIKB_ERROR_E error_code = MMIIKB_STATUS_INVALID_PARAM;
    
    if (PNULL == process_param_ptr
        || PNULL == param_in_ptr
        || PNULL == param_out_ptr)
    {
        //SCI_TRACE_LOW:"[MMIIKB] ReceiveUnitData param is null."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_TASK_4025_112_2_18_2_24_32_137,(uint8*)"");
        return error_code;
    }

    if (PNULL == process_param_ptr->restore_call_back)
    {
        //SCI_TRACE_LOW:"[MMIIKB] ReceiveUnitData backup_call_back is null, type=%d."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_TASK_4032_112_2_18_2_24_32_138,(uint8*)"d", process_param_ptr->type);
            
        error_code = MMIIKB_STATUS_APP_REG_ERROR;
        return error_code;
    }

    //调用callback取数据之前，如果是发signal的形式，则先启用定时器计时
    if (process_param_ptr->is_backup_asyn)
    {
        CreateProtectionTimer();
    }
    
    //lock
    SCI_GetMutex(s_ikb_cancel_mutext, SCI_WAIT_FOREVER);  

    process_param_ptr->backup_call_back(param_in_ptr, param_out_ptr);

    //unlock
    SCI_PutMutex(s_ikb_cancel_mutext);
    
    error_code = MMIIKB_STATUS_SUCCESS;
    return error_code;
}

/*****************************************************************************/
//  Description : WriteUnitData
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
LOCAL MMIIKB_ERROR_E WriteUnitData(
    uint16 *file_path_ptr,
    uint16 file_path_len,    
    MMIIKB_PACK_CONT_UNIT_T *cont_unit_ptr
)
{
    MMIIKB_ERROR_E error_code = MMIIKB_STATUS_INVALID_PARAM;
    uint32 write_len = 0;
    MMIFILE_HANDLE file_handle = SFS_INVALID_HANDLE;
    MMIFILE_ERROR_E file_error = SFS_ERROR_NONE;
    BOOLEAN is_first = FALSE;
    MMIIKB_PACK_CONT_HEAD_T cont_head = {0};

    if (PNULL == file_path_ptr
        || PNULL == cont_unit_ptr
        || 0 == file_path_len)
    {
        //SCI_TRACE_LOW:"[MMIIKB] WriteUnitData param is null."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_TASK_4071_112_2_18_2_24_32_139,(uint8*)"");
        
        return error_code;
    }
    
    if (MMIAPIFMM_IsFileExist(file_path_ptr, file_path_len))
    {
        file_handle = MMIAPIFMM_CreateFile(file_path_ptr, 
            SFS_MODE_APPEND|SFS_MODE_WRITE, 0, 0);
    }
    else
    {
        file_handle = MMIAPIFMM_CreateFile(file_path_ptr, 
            SFS_MODE_WRITE|SFS_MODE_CREATE_NEW, 0, 0);
        is_first = TRUE;
    }

    if (SFS_INVALID_HANDLE != file_handle)
    {
        if (is_first)
        {
            //write content head to occupy position
            SCI_MEMSET(&cont_head, 0x0, sizeof(MMIIKB_PACK_CONT_HEAD_T));
            file_error = MMIAPIFMM_WriteFile(file_handle, &cont_head,
                sizeof(MMIIKB_PACK_CONT_HEAD_T), &write_len, PNULL);
        }

        if (SFS_ERROR_NONE == file_error)
        {
            file_error = MMIAPIFMM_WriteFile(file_handle, 
                (void*)(&(cont_unit_ptr->len)), sizeof(cont_unit_ptr->len), 
                &write_len, PNULL);
            if (SFS_ERROR_NONE == file_error)
            {
                MMIAPIFMM_WriteFile(file_handle, 
                    (void*)(&(cont_unit_ptr->number)), sizeof(cont_unit_ptr->number), 
                    &write_len, PNULL);
                MMIAPIFMM_WriteFile(file_handle, 
                    (void*)(&(cont_unit_ptr->reserved1)),
                    sizeof(uint32), &write_len, PNULL);
                MMIAPIFMM_WriteFile(file_handle, 
                    (void*)(&(cont_unit_ptr->reserved2)),
                    sizeof(uint32), &write_len, PNULL);
                MMIAPIFMM_WriteFile(file_handle, cont_unit_ptr->buf_ptr, 
                    cont_unit_ptr->len, &write_len, PNULL);
                MMIAPIFMM_WriteFile(file_handle, 
                    (void*)(&(cont_unit_ptr->end_sign1)),
                    sizeof(uint32), &write_len, PNULL);
                MMIAPIFMM_WriteFile(file_handle, 
                    (void*)(&(cont_unit_ptr->end_sign2)),
                    sizeof(uint32), &write_len, PNULL);
            
                error_code = MMIIKB_STATUS_SUCCESS;
            }
            else
            {
                error_code = MMIIKB_STATUS_FILE_ERROR;
            }
        }
        else
        {
            error_code = MMIIKB_STATUS_FILE_ERROR;
        }
        
    }
    else
    {
        error_code = MMIIKB_STATUS_FILE_ERROR;
    }
    
    MMIAPIFMM_CloseFile(file_handle);
    
    return error_code;
}

/*****************************************************************************/
//  Description : WriteContentHead
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
LOCAL MMIIKB_ERROR_E WriteContentHead(
    uint16 *file_path_ptr,
    uint16 file_path_len,
    MMIIKB_PACK_CONT_HEAD_T *cont_head_ptr
)
{
    MMIIKB_ERROR_E error_code = MMIIKB_STATUS_INVALID_PARAM;
    uint32 write_len = 0;
    MMIFILE_HANDLE file_handle = SFS_INVALID_HANDLE;
    MMIFILE_ERROR_E file_error = SFS_ERROR_NONE;
    

    if (PNULL == file_path_ptr
        || PNULL == cont_head_ptr
        || 0 == file_path_len)
    {
        //SCI_TRACE_LOW:"[MMIIKB] WriteContentHead param is null."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_TASK_4166_112_2_18_2_24_33_140,(uint8*)"");
        
        return error_code;
    }
    
    file_handle = MMIAPIFMM_CreateFile(file_path_ptr, 
        SFS_MODE_WRITE|SFS_MODE_OPEN_EXISTING, 0, 0);

    if (SFS_INVALID_HANDLE != file_handle)
    {
        if (SFS_ERROR_NONE == 
            MMIAPIFMM_SetFilePointer(file_handle, 0, SFS_SEEK_BEGIN))
        {
            file_error = MMIAPIFMM_WriteFile(file_handle, cont_head_ptr,
                sizeof(MMIIKB_PACK_CONT_HEAD_T), &write_len, PNULL);
            if (SFS_ERROR_NONE == file_error)
            {
                error_code = MMIIKB_STATUS_SUCCESS;
            }
            else
            {
                error_code = MMIIKB_STATUS_FILE_ERROR;
            }
        }
        else
        {
            error_code = MMIIKB_STATUS_FILE_ERROR;
        }
    }
    else
    {
        error_code = MMIIKB_STATUS_FILE_ERROR;
    }
    
    MMIAPIFMM_CloseFile(file_handle);
    
    return error_code;
}

/*****************************************************************************/
//  Description : SendUnitData
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
LOCAL MMIIKB_ERROR_E SendUnitData(
    const MMIIKB_TYPE_PROCESS_PARAM_T * process_param_ptr,
    MMIIKB_RESTORE_CALLBACK_PARAM_IN_T *param_in_ptr,
    MMIIKB_RESTORE_CALLBACK_PARAM_OUT_T *param_out_ptr
)
{
    MMIIKB_ERROR_E error_code = MMIIKB_STATUS_INVALID_PARAM;
    
    if (PNULL == process_param_ptr
        || PNULL == param_in_ptr
        || PNULL == param_out_ptr)
    {
        //SCI_TRACE_LOW:"[MMIIKB] SendUnitData param is null."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_TASK_4221_112_2_18_2_24_33_141,(uint8*)"");
        return error_code;
    }

    if (PNULL == process_param_ptr->restore_call_back)
    {
        //SCI_TRACE_LOW:"[MMIIKB] SendUnitData restore_call_back is null, type=%d."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_TASK_4228_112_2_18_2_24_33_142,(uint8*)"d", process_param_ptr->type);
            
        error_code = MMIIKB_STATUS_APP_REG_ERROR;
        return error_code;
    }

    //调用callback之前，如果是发signal的形式，则先启用定时器计时
    if (process_param_ptr->is_restore_asyn)
    {
        CreateProtectionTimer();
    }
    
    //lock
    SCI_GetMutex(s_ikb_cancel_mutext, SCI_WAIT_FOREVER);  

    process_param_ptr->restore_call_back(param_in_ptr, param_out_ptr);
    
    //unlock
    SCI_PutMutex(s_ikb_cancel_mutext);
    
    error_code = MMIIKB_STATUS_SUCCESS;
    return error_code;
}

/*****************************************************************************/
//  Description : ReadUnitData
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
LOCAL MMIIKB_ERROR_E ReadUnitData(
    MMIFILE_HANDLE file_handle,
    uint32 offset,
    MMIIKB_PACK_CONT_UNIT_T *cont_unit_ptr
)
{
    MMIIKB_ERROR_E error_code = MMIIKB_STATUS_INVALID_PARAM;
    uint32 read_len = 0;
    void *buf_ptr = PNULL;
    MMIFILE_ERROR_E file_error = SFS_ERROR_NONE;
    
    if (SFS_INVALID_HANDLE == file_handle
        || PNULL == cont_unit_ptr)
    {
        //SCI_TRACE_LOW:"[MMIIKB] ReadUnitData param is null."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_TASK_4264_112_2_18_2_24_33_143,(uint8*)"");
        return error_code;
    }
    
    if (SFS_ERROR_NONE == MMIAPIFMM_SetFilePointer(file_handle, 
        (int32)offset, SFS_SEEK_BEGIN))
    {
        file_error = MMIAPIFMM_ReadFile(file_handle, &(cont_unit_ptr->len),
            sizeof(cont_unit_ptr->len), &read_len, PNULL);
        file_error = MMIAPIFMM_ReadFile(file_handle, &(cont_unit_ptr->number),
            sizeof(cont_unit_ptr->number), &read_len, PNULL);
        file_error = MMIAPIFMM_ReadFile(file_handle, &(cont_unit_ptr->reserved1),
            sizeof(cont_unit_ptr->reserved1), &read_len, PNULL);
        file_error = MMIAPIFMM_ReadFile(file_handle, &(cont_unit_ptr->reserved2),
            sizeof(cont_unit_ptr->reserved2), &read_len, PNULL);

        buf_ptr = SCI_ALLOCA(cont_unit_ptr->len);
        if (PNULL != buf_ptr)
        {
            SCI_MEMSET(buf_ptr, 0x00, cont_unit_ptr->len);
            file_error = MMIAPIFMM_ReadFile(file_handle, buf_ptr,
                cont_unit_ptr->len, &read_len, PNULL);
            cont_unit_ptr->buf_ptr = buf_ptr;
        
            file_error = MMIAPIFMM_ReadFile(file_handle, 
                &(cont_unit_ptr->end_sign1), 
                sizeof(cont_unit_ptr->end_sign1), &read_len, PNULL);
            file_error = MMIAPIFMM_ReadFile(file_handle, 
                &(cont_unit_ptr->end_sign2),
                sizeof(cont_unit_ptr->end_sign2), &read_len, PNULL);
            
            if (SFS_ERROR_NONE == file_error)
            {
                error_code = MMIIKB_STATUS_SUCCESS;
            }
            else
            {
                error_code = MMIIKB_STATUS_FILE_ERROR;
            }
        }
        else
        {
            error_code =  MMIIKB_STATUS_NO_MEMORY;
        }
    }
    
    //SCI_TRACE_LOW:"[MMIIKB] ReadUnitData offset=%d, error_code=%d."
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_TASK_4311_112_2_18_2_24_33_144,(uint8*)"dd", offset, error_code);
    
    return error_code;
}

/*****************************************************************************/
//  Description : ReadContentHead
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
LOCAL MMIIKB_ERROR_E ReadContentHead(
    uint16 *file_path_ptr,
    uint16 file_path_len,
    MMIIKB_PACK_CONT_HEAD_T *cont_head_ptr
)
{
    MMIIKB_ERROR_E error_code = MMIIKB_STATUS_INVALID_PARAM;
    uint32 read_len = 0;
    MMIFILE_HANDLE file_handle = SFS_INVALID_HANDLE;
    MMIFILE_ERROR_E file_error = SFS_ERROR_NONE;
    

    if (PNULL == file_path_ptr
        || 0 == file_path_len
        || PNULL == cont_head_ptr)
    {
        //SCI_TRACE_LOW:"[MMIIKB] ReadContentHead param is null."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_TASK_4336_112_2_18_2_24_33_145,(uint8*)"");
        return error_code;
    }
    
    file_handle = MMIAPIFMM_CreateFile(file_path_ptr, 
        SFS_MODE_READ|SFS_MODE_OPEN_EXISTING, 0, 0);

    if (SFS_INVALID_HANDLE == file_handle)
    {
        error_code = MMIIKB_STATUS_CREATE_FILE_FAILED;
    }
    else
    {
        //read content head info
        if (SFS_ERROR_NONE == 
            MMIAPIFMM_SetFilePointer(file_handle, 0, SFS_SEEK_BEGIN))
        {
            file_error = MMIAPIFMM_ReadFile(file_handle, cont_head_ptr,
                sizeof(MMIIKB_PACK_CONT_HEAD_T), &read_len, PNULL);
            if (SFS_ERROR_NONE == file_error 
                    && read_len == sizeof(MMIIKB_PACK_CONT_HEAD_T))
            {
                error_code = MMIIKB_STATUS_SUCCESS;
            }
            else
            {
                error_code = MMIIKB_STATUS_FILE_ERROR;
            }
        }
        else
        {
            error_code = MMIIKB_STATUS_FILE_ERROR;
        }
    }

    MMIAPIFMM_CloseFile(file_handle);
    
    //SCI_TRACE_LOW:"[MMIIKB] ReadContentHead error_code=%d."
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_TASK_4373_112_2_18_2_24_33_146,(uint8*)"d", error_code);
    
    return error_code;
}


/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif  //#if defined(IKEYBACKUP_SUPPORT)















