/*****************************************************************************
** File Name:      mmiikeybackup_task.h                                      *
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

#if defined(IKEYBACKUP_SUPPORT)

#ifndef _MMIIKEYBACKUP_TASK_H_
#define _MMIIKEYBACKUP_TASK_H_


/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmiikeybackup_api.h"
#include "mmiikeybackup_internal.h"
#include "priority_app.h"

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
#define P_IKB_STACK_SIZE                (1024*8)
#define P_IKB_QUEUE_NUM                 15
#define P_IKB_TASK_PRIORITY             PRI_APP_IKB_TASK

#define UNUSED_PARAM(param)             ((param) = (param))


/**--------------------------------------------------------------------------*
 **                         Type Definition                                  *
 **--------------------------------------------------------------------------*/

typedef struct
{
    MMIIKB_TYPE_E type;
    wchar type_file_name[MMIIKB_TYPE_CONTENT_NAME_LEN];
    
}MMIIKB_TYPE_SETTING_T;




/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : init ikb agent
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC void IKeyBackup_InitIKBAgent(void);

/*****************************************************************************/
//  Description : create 1-key backup task
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC void IKeyBackup_StartIKBHandlerTask(void);

/*****************************************************************************/
//  Description : IKeyBackup_GetIKBTaskId
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC BLOCK_ID IKeyBackup_GetIKBTaskId(void);

/*****************************************************************************/
//  Description : IKeyBackup_SuspendTask
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC uint32 IKeyBackup_SuspendTask(void);

/*****************************************************************************/
//  Description : IKeyBackup_ResumeTask
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC uint32 IKeyBackup_ResumeTask(void);
 
/*****************************************************************************/
//  Description : 1-key backup is do backup thing now
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:备份或者还原任务正在忙碌中
/*****************************************************************************/
PUBLIC BOOLEAN IKeyBackup_IsBusy(void);
 
/*****************************************************************************/
//  Description : 1-key backup is do backup thing now
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:是否正在备份中
/*****************************************************************************/
PUBLIC BOOLEAN IKeyBackup_IsBackupBusy(void);
 
/*****************************************************************************/
//  Description : 1-key backup is do backup thing now
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:是否正在还原中
/*****************************************************************************/
PUBLIC BOOLEAN IKeyBackup_IsRestoreBusy(void);

/*****************************************************************************/
//  Description : IKeyBackup_ActionSetCancel
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC void IKeyBackup_ActionSetCancel(void);

/*****************************************************************************/
//  Description : IKeyBackup_ActionSetOk
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC void IKeyBackup_ActionSetOk(void);

/*****************************************************************************/
//  Description : IKeyBackup_GetCancelState
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN IKeyBackup_GetCancelState(void);

/*****************************************************************************/
//  Description : IKeyBackup_GetContentName
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC const uint16* IKeyBackup_GetContentName(MMIIKB_TYPE_E type);

/*****************************************************************************/
//  Description : IKeyBackup_GetTotalSizeByType
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC MMIIKB_ERROR_E IKeyBackup_GetTotalSizeByType(
    MMIIKB_TYPE_E type,
    MMIIKB_GET_TOTAL_OUT_T *param_out_ptr
);

/*****************************************************************************/
//  Description : 1-key backup all items request by app
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: app send MSG_IKB_BACKUP_DATA_REQ request to 1-key
/*****************************************************************************/
PUBLIC MMIIKB_ERROR_E IKeyBackup_BackupAllItemReq(
    MMIIKB_BACKUP_REQ_T *backup_req_ptr
);

/*****************************************************************************/
//  Description : IKeyBackup_SendBackupItemDataRes
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
PUBLIC MMIIKB_ERROR_E IKeyBackup_SendBackupItemDataRes(
    MMIIKB_BACKUP_ITEM_DATA_RES_T * backup_item_data_res
);

/*****************************************************************************/
//  Description : 1-key restore all items request by app
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: app send MSG_IKB_RESTORE_DATA_REQ request to 1-key
/*****************************************************************************/
PUBLIC MMIIKB_ERROR_E IKeyBackup_RestoreAllItemReq(
    MMIIKB_RESTORE_REQ_T *restore_req_ptr
);

/*****************************************************************************/
//  Description : IKeyBackup_SendRestoreItemDataRes
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
PUBLIC MMIIKB_ERROR_E IKeyBackup_SendRestoreItemDataRes(
    MMIIKB_RESTORE_ITEM_DATA_RES_T * restore_item_data_res
);

/*****************************************************************************/
//  Description : IKeyBackup_SendProgressInfo
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
PUBLIC MMIIKB_ERROR_E IKeyBackup_SendProgressInfo(
    const MMIIKB_TASK_SEQ_TABLE_T *seq_task_ptr
);

/*****************************************************************************/
//  Description : 1-key send cancel action (backup or restore) req by app
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: app send MSG_IKB_CANCEL_ACTION_REQ request to 1-key
/*****************************************************************************/
PUBLIC MMIIKB_ERROR_E IKeyBackup_CancelActionReq(
    MMIIKB_TASK_TYPE_E task_type
);


//crc
/*****************************************************************************/
//  Description : IKeyBackup_IsCrc
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN IKeyBackup_IsCrc(void);

/*****************************************************************************/
//  Description : IKeyBackup_CalCrcItemData
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 文件内容分段算CRC
/*****************************************************************************/
PUBLIC MMIIKB_ERROR_E IKeyBackup_CalCrcItemData(
    uint16 *file_path_ptr,
    uint32 *crc_ptr
);

/*****************************************************************************/
//  Description : IKeyBackup_IsCrcRight
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
PUBLIC MMIIKB_ERROR_E IKeyBackup_IsCrcRight(
    uint16 *file_path,
    uint32 crc
);


//encript
/*****************************************************************************/
//  Description : IKeyBackup_IsEnc
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN IKeyBackup_IsEnc(MMIIKB_TYPE_E type);

/*****************************************************************************/
//  Description : IKeyBackup_EncBuf
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
PUBLIC MMIIKB_ERROR_E IKeyBackup_EncBuf(
    uint8* buf_ptr,
    uint32 buf_len
);

/*****************************************************************************/
//  Description : IKeyBackup_DecBuf
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
PUBLIC MMIIKB_ERROR_E IKeyBackup_DecBuf(
    uint8 * buf_ptr,
    uint16 buf_len
);

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
);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/

#ifdef   __cplusplus
    }
#endif

#endif  //_MMIIKEYBACKUP_TASK_H_

#endif  //#if defined(IKEYBACKUP_SUPPORT)

