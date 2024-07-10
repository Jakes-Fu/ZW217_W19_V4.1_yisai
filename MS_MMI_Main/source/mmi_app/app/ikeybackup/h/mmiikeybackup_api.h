/*****************************************************************************
** File Name:      mmiikeybackup_api.h                                       *
** Author:                                                                   *
** Date:           12/12/2011                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:                                                              *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 12/12/2011     haiwu.chen       Create                                    *
******************************************************************************/

#if defined(IKEYBACKUP_SUPPORT)

#ifndef _MMIIKEYBACKUP_API_H_
#define _MMIIKEYBACKUP_API_H_


/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "std_header.h"
#include "sci_types.h"
#include "mmifilearray_export.h"
#include "mmi_appmsg.h"
#include "mmifmm_export.h"



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
#define MMIIKB_MODU_NAME            "mmikeyback"

#define MMIIKB_PACK_NAME_LEN            (24)
#define MMIIKB_PACK_DIR_LEN             (40)
#define MMIIKB_TYPE_CONTENT_NAME_LEN    (16)
#define MMIIKB_VERSION_LEN              (50)
#define MMIIKB_MODULE_LEN              (10)
#define MMIIKB_PASSWORD_LEN             (10)
#define MMIIKB_PACK_CONTENT_NAME_LEN    (16)
#define MMIIKB_CON_PATH_LEN             (255)
#define MMIIKB_TYPE_MAX_CNT             (16)
#define MMIIKB_SPACE_LIMIT_VALUE        (10*1024)

#define IKEYBACKUP_SEND_SIGNAL(_SIG_PTR, _RECEIVER) \
        SCI_SendSignal((xSignalHeader)(_SIG_PTR), _RECEIVER);


/**--------------------------------------------------------------------------*
 **                         Type Definition                                  *
 **--------------------------------------------------------------------------*/
 
//storage position
typedef enum
{
    MMIIKB_STORE_POS_NONE,
    MMIIKB_STORE_POS_SD_1,              // storage in sd card 1
    MMIIKB_STORE_POS_SD_2,              // storage in sd card 2
    MMIIKB_STORE_POS_SERVER,            // storage in server
    
    MMIIKB_STORE_POS_MAX,
}MMIIKB_STORE_POS_E;

//the types of backup contents
typedef enum
{
    MMIIKB_TYPE_CALLLOG,
    MMIIKB_TYPE_PHONEBOOK,
    MMIIKB_TYPE_SMS,
    MMIIKB_TYPE_SCHEDULE,
    MMIIKB_TYPE_BRO_LABEL,

    // should less than MMIIKB_TYPE_MAX_CNT
    MMIIKB_TYPE_MAX,                    
}MMIIKB_TYPE_E;

//the content type which get from application
typedef enum
{
    MMIIKB_OPERATE_TYPE_NONE,
    MMIIKB_OPERATE_TYPE_BUF,                    // buffer type
    MMIIKB_OPERATE_TYPE_FILE,                   // file type
    MMIIKB_OPERATE_TYPE_DIR,                    // directory type,inc sub dir
    
    MMIIKB_OPERATE_TYPE_MAX,
}MMIIKB_OPERATE_TYPE_E;

//task type
typedef enum
{
    MMIIKB_TASK_TYPE_NONE,
    MMIIKB_TASK_TYPE_BACKUP,                    // task backup
    MMIIKB_TASK_TYPE_RESTORE,                   // task restore
    
    MMIIKB_TASK_TYPE_MAX,
}MMIIKB_TASK_TYPE_E;

//各个应用告知，总数的单位是字节，条数，或者其它后补
typedef enum
{
    MMIIKB_TOTAL_TYPE_NONE,
    MMIIKB_TOTAL_TYPE_BYTE,                     // 字节
    MMIIKB_TOTAL_TYPE_NUMBER,                   // 条数
    
    MMIIKB_TOTAL_TYPE_MAX,
}MMIIKB_TOTAL_TYPE_E;

//message, define in mmi_appmsg.h
typedef uint16 MSG_IKB_SIG_E;

//backup and restore status code
typedef enum
{
    MMIIKB_STATUS_NONE = 0x00,      /* 00 : No errors encountered.      */
    MMIIKB_STATUS_SUCCESS = 0x00,   /* 00 : No errors encountered.      */
    MMIIKB_STATUS_FAILED,           /* 01 : General error status.       */
    MMIIKB_STATUS_SYSTEM_ERROR,     /* 02 : System error status.        */
    MMIIKB_STATUS_FILE_ERROR,       /* 03 : FILE error status.          */
    MMIIKB_STATUS_NO_MEMORY,        /* 04 : no dynamic memory.          */
    MMIIKB_STATUS_INSUFFI_MEMORY,   /* 05 : insufficient memoryerror.   */
    MMIIKB_STATUS_INVALID_FORMAT,   /* 06 : invalid format.             */
    MMIIKB_STATUS_UNKNOWN_ERROR,    /* 07 : unknown error.              */
    MMIIKB_STATUS_OS_ERROR,         /* 08 : system error.               */
    MMIIKB_STATUS_INVALID_PARAM,    /* 09 : invalid parameter.          */
    MMIIKB_STATUS_DISK_NO_SPACE,    /* 10 : not enough space.           */
    MMIIKB_STATUS_FILE_EOF,         /* 10 : reach the end of file.      */
    MMIIKB_STATUS_CREATE_FILE_FAILED,/* 11 : create file error.         */
    MMIIKB_STATUS_BASE64_FILE_ERROR,/* 12 : encripy buf error.          */
    MMIIKB_STATUS_RESERVED,         /* 13 : Reserved.                   */
    MMIIKB_STATUS_14,               /* 14 : Reserved.                   */
    MMIIKB_STATUS_15,               /* 15 : Reserved.                   */
    MMIIKB_STATUS_IKB_IS_BUSY,      /* 16 : task is busy now, can't 
                                            execute this request now.   */
    MMIIKB_STATUS_NO_INIT_TASK,     /* 17 : Reserved.                   */
    MMIIKB_STATUS_WRONG_TASK,       /* 18 : Reserved.                   */
    MMIIKB_STATUS_WRONG_OP_TYPE,    /* 19 : Reserved.                   */
    MMIIKB_STATUS_EMPTY_APP_BUF,    /* 20 : Reserved.                   */
    MMIIKB_STATUS_CRC_ERROR,        /* 21 : Crc error.                  */
    MMIIKB_STATUS_SD_NOT_READY,     /* 22 : sd card is not ready.       */
    MMIIKB_STATUS_READ_PACK_ERROR,  /* 23 : Read pack info error.       */
    MMIIKB_STATUS_CANCEL_ACTION,    /* 24 : user cancel action.         */
    MMIIKB_STATUS_BACKUP_TIME_OUT,  /* 25 : backup time out.            */
    MMIIKB_STATUS_RESTORE_TIME_OUT, /* 26 : restore time out.           */
    MMIIKB_STATUS_SD_NOT_EXIST,     /* 27 : sd card is not exist.       */
    MMIIKB_STATUS_APP_REG_ERROR,    /* 28 : app register error.         */
    MMIIKB_STATUS_SD_OPERATE_FAIL,  /* 29 : sd operate fail.            */
    MMIIKB_STATUS_PB_FULL,          /* 30 : PB is full.                 */
    MMIIKB_STATUS_ILLEGAL_VERSION,  /* 31 : version illegal.            */
    MMIIKB_STATUS_UDISK_USING,      /* 32 : udisk in using.             */
    MMIIKB_STATUS_CL_WRONG_FORMAT,  /* 33 : wrong format of cl          */

    MMIIKB_STATUS_MAX
}MMIIKB_ERROR_E;



typedef uint16 MMIIKB_TYPE_T;

//global config define
typedef struct
{
    MMIIKB_STORE_POS_E store_pos;
    MMIIKB_TYPE_T types;
    BOOLEAN is_crc_check;
}MMIIKB_CONFIG_T;


//app register call back
typedef struct
{
    MMIIKB_TYPE_E type;
    MMIIKB_OPERATE_TYPE_E op_type;
    uint32 begin;
    uint32 len;
    uint32 number;                          /* have got all numbers.        */
    uint32 trans_cnt;                       /* have transfered unit count.  */
        
}MMIIKB_BACKUP_ITEM_DATA_REQ_T;

typedef struct
{    
    MMIIKB_TYPE_E type;
    MMIIKB_OPERATE_TYPE_E op_type;
    void *buf_ptr;
    uint32 begin;
    uint32 len;
    uint32 number;
    uint16 *path_ptr;
    uint16 path_len;
    BOOLEAN is_need_free;
    BOOLEAN is_finished;
    
    MMIIKB_ERROR_E error_code;              /* app request backup items err.*/
}MMIIKB_BACKUP_ITEM_DATA_RES_T;

typedef struct
{
    MMIIKB_TYPE_E type;
    MMIIKB_OPERATE_TYPE_E op_type;
    void *buf_ptr;                          /* point to buffer from app.    */
    uint32 begin;
    uint32 len;
    uint32 number;
    uint32 trans_idx;                       /* buf transfer unit index.   */
    uint16 *path_ptr;
    uint16 path_len;
}MMIIKB_RESTORE_ITEM_DATA_REQ_T;

typedef struct
{
    MMIIKB_TYPE_E type;
    MMIIKB_OPERATE_TYPE_E op_type;
    
    uint32 suc_cnt;
    uint32 err_cnt;
    
    MMIIKB_ERROR_E error_code;              /* app request backup items err.*/
}MMIIKB_RESTORE_ITEM_DATA_RES_T;

typedef struct
{
    uint32 total;                           /*可以是总条数，也可以是总大小，
                                              由应用自行决定                */
    MMIIKB_TOTAL_TYPE_E total_type;
    
    MMIIKB_ERROR_E error_code;              /* app request backup items err.*/
}MMIIKB_GET_TOTAL_OUT_T;


typedef MMIIKB_BACKUP_ITEM_DATA_REQ_T MMIIKB_BACKUP_CALLBACK_PARAM_IN_T;
typedef MMIIKB_BACKUP_ITEM_DATA_RES_T MMIIKB_BACKUP_CALLBACK_PARAM_OUT_T;
typedef MMIIKB_RESTORE_ITEM_DATA_REQ_T MMIIKB_RESTORE_CALLBACK_PARAM_IN_T;
typedef MMIIKB_RESTORE_ITEM_DATA_RES_T MMIIKB_RESTORE_CALLBACK_PARAM_OUT_T;

typedef void (*MMIIKB_TYPE_BACKUP_CALLBACK)(
    MMIIKB_BACKUP_CALLBACK_PARAM_IN_T *param_in_ptr,
    MMIIKB_BACKUP_CALLBACK_PARAM_OUT_T *param_out_ptr);

typedef void (*MMIIKB_TYPE_RESTORE_CALLBACK)(
    MMIIKB_RESTORE_CALLBACK_PARAM_IN_T *param_in_ptr,
    MMIIKB_RESTORE_CALLBACK_PARAM_OUT_T *param_out_ptr);

typedef void (*MMIIKB_TYPE_GET_TOTAL_CALLBACK)(
    MMIIKB_GET_TOTAL_OUT_T *param_out_ptr);

typedef struct
{
    MMIIKB_TYPE_E type;
    MMIIKB_OPERATE_TYPE_E op_type;
    MMIIKB_TYPE_BACKUP_CALLBACK backup_call_back;
    BOOLEAN is_backup_asyn;
    MMIIKB_TYPE_RESTORE_CALLBACK restore_call_back;
    BOOLEAN is_restore_asyn;
    MMIIKB_TYPE_GET_TOTAL_CALLBACK get_total_call_back;
}MMIIKB_TYPE_PROCESS_PARAM_T;


typedef struct
{
    SCI_DATE_T min_date;
    SCI_TIME_T min_time;
}MMIIKB_TIME_T;

typedef struct
{
    BOOLEAN is_password;                    /* is need password.            */
    wchar password[MMIIKB_PASSWORD_LEN];    /* password info.               */
    uint16 password_len;                    /* password len.                */
}MMIIKB_PASSWORD_INFO_T;

//package head file information, read from "Ibphone_head.in"
typedef struct _mmiikb_pack_info_t
{
    uint32 id;                              /* Reserved                     */
    wchar pack_name[MMIIKB_PACK_NAME_LEN];  /* package directory name       */
    wchar pack_dir[MMIIKB_PACK_DIR_LEN];    /* Reserved package absolute dir*/
    char version[MMIIKB_VERSION_LEN];       /* storage first 50 chars       */
    char mod_name[MMIIKB_MODULE_LEN+1];     /*to check is the file we restore*/
    MMIIKB_TIME_T begin_time;               /* backup beginning times       */
    MMIIKB_TIME_T eng_time;                 /* backup ending times          */
    MMIIKB_TYPE_T types;                    /* all types of this pack backup*/
    uint32 state;                           /* Reserved. package state info */
    uint32 size;                            /* options. all package size    */
    MMIIKB_PASSWORD_INFO_T psw_info;        /* password info.               */
    uint32 crcs[MMIIKB_TYPE_MAX_CNT];       /* all content file crc.        */
    
    uint32 reserved1;                       /* Reserved1.                   */
    uint32 reserved2;                       /* Reserved2.                   */
    uint32 reserved3;                       /* Reserved3.                   */
    
}MMIIKB_PACK_INFO_T;


//消息对外公开的结构体数据，请求消息方，需要填写下面这些结构体内容
typedef struct
{
    wchar pack_dir[MMIIKB_PACK_DIR_LEN];    /* APP have created pack dir.   */
    MMIIKB_TYPE_T types;                    /* all types of this pack backup.
                                            such as ((0x01<<pb)|(0x01<<cc)) */
    MMIIKB_PASSWORD_INFO_T psw_info;        /* password info.               */
    MMIIKB_CONFIG_T *cfg_ptr;
}MMIIKB_BACKUP_REQ_T;


typedef struct
{
    MMIIKB_PACK_INFO_T *pack_info_ptr;      /* all types of this pack backup*/
    
    MMIIKB_TYPE_T types;                    /* all selected types in pack.  */
    
    MMIIKB_CONFIG_T *cfg_ptr;
}MMIIKB_RESTORE_REQ_T;

typedef struct
{
    FILEARRAY file_array;
    
}MMIIKB_LOAD_PACK_INFO_T;


//下面这段中的消息回执需要公开在api.h中
typedef struct
{
    _SIGNAL_VARS
        
    wchar pack_dir[MMIIKB_PACK_DIR_LEN];    /* APP have created pack dir.   */        
    MMIIKB_TYPE_T types;                    /* reserved, backup suc items.  */
    uint32 size;                            /* options. all package size    */
    MMIIKB_PASSWORD_INFO_T psw_info;        /* password info.               */
    uint32 crcs[MMIIKB_TYPE_MAX_CNT];       /* all content file crc.        */
    
    MMIIKB_ERROR_E error_code;              /* app request backup items err.*/
}MMIIKB_SIG_BACKUP_RES_T;

typedef struct
{
    _SIGNAL_VARS
        
    MMIIKB_BACKUP_ITEM_DATA_REQ_T backup_item_data_req;
}MMIIKB_SIG_BACKUP_ITEM_DATA_REQ_T;

typedef struct
{
    _SIGNAL_VARS
        
    MMIIKB_TYPE_T suc_types;                /* reserved, backup suc items.  */
    MMIIKB_TYPE_T err_types;                /* reserved, backup suc items.  */    
    
    MMIIKB_ERROR_E error_code;              /* app request backup items err.*/
}MMIIKB_SIG_RESTORE_RES_T;

typedef struct
{
    _SIGNAL_VARS
        
    MMIIKB_RESTORE_ITEM_DATA_REQ_T restore_item_data_req;
}MMIIKB_SIG_RESTORE_ITEM_DATA_REQ_T;

typedef struct
{
    _SIGNAL_VARS

    FILEARRAY file_array;
    
}MMIIKB_SIG_LOAD_PACK_INFO_REQ_T;

typedef struct
{
    _SIGNAL_VARS
    
    MMIIKB_ERROR_E error_code;              /* app request backup items err.*/
}MMIIKB_SIG_LOAD_PACK_INFO_RES_T;

typedef struct
{
    _SIGNAL_VARS

    MMIIKB_TASK_TYPE_E task_type;
    wchar pack_dir[MMIIKB_PACK_DIR_LEN];    /* APP have created pack dir.   */
    
    MMIIKB_ERROR_E error_code;
}MMIIKB_SIG_CANCEL_RES_T;



/**--------------------------------------------------------------------------*
 **                         EXPORT FUNCTION DEFINITION                       *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : 1-key backup init module
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIIKB_InitModule(void);

/*****************************************************************************/
//  Description : 1-key backup init
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIIKB_Init(void);

/*****************************************************************************/
//  Description : 1-key backup register by app
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIIKB_RegTypeFunc(MMIIKB_TYPE_PROCESS_PARAM_T *param_ptr);

/*****************************************************************************/
//  Description : 1-key backup application start
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIIKB_ApplicationStart(void);

/*****************************************************************************/
//  Description : 1-key backup application terminate
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIIKB_ApplicationTerminate(void);
 
/*****************************************************************************/
//  Description : 1-key backup get global config value
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:获取程序的全局配置值，启动的时候就这么一个值
/*****************************************************************************/
PUBLIC MMIIKB_CONFIG_T * MMIAPIIKB_GetConfigInfo(void);

/*****************************************************************************/
//  Description : MMIAPIIKB_SuspendTask
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIAPIIKB_SuspendTask(void);

/*****************************************************************************/
//  Description : MMIAPIIKB_ResumeTask
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIAPIIKB_ResumeTask(void);
 
/*****************************************************************************/
//  Description : 1-key backup is do backup thing now
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:备份或者还原任务正在忙碌中
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIIKB_IsBusy(void);
 
/*****************************************************************************/
//  Description : 1-key backup is do backup thing now
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:是否正在备份中
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIIKB_IsBackupBusy(void);
 
/*****************************************************************************/
//  Description : 1-key backup is do backup thing now
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:是否正在还原中
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIIKB_IsRestoreBusy(void);

/*****************************************************************************/
//  Description : 创建主窗口
//  Global resource dependence :
//  Author: haiwu.chen
//  Note: 本函数算是一键备份还原的UI层实例创建
/*****************************************************************************/
PUBLIC MMI_HANDLE_T MMIAPIIKB_OpenMainWin(void);

/*****************************************************************************/
//  Description : MMIAPIIKB_HandleAppSigRes
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 处理1-key backup反馈消息
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIIKB_HandleAppSigRes(MMI_MESSAGE_ID_E msg_id, void *param);

/*****************************************************************************/
//  Description : 1-key backup all items request by app
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: app send MSG_IKB_BACKUP_DATA_REQ request to 1-key
/*****************************************************************************/
PUBLIC MMIIKB_ERROR_E MMIAPIIKB_BackupAllItemReq(
    MMIIKB_BACKUP_REQ_T *backup_req_ptr
);

/*****************************************************************************/
//  Description : app send item data to 1-key task
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: app send MSG_IKB_BACKUP_ITEM_DATA_RES data to 1-key
/*****************************************************************************/
PUBLIC MMIIKB_ERROR_E MMIAPIIKB_SendBackupItemDataRes(
    MMIIKB_BACKUP_ITEM_DATA_RES_T * backup_item_data_res
);

/*****************************************************************************/
//  Description : 1-key restore all items request by app
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: app send MSG_IKB_RESTORE_DATA_REQ request to 1-key
/*****************************************************************************/
PUBLIC MMIIKB_ERROR_E MMIAPIIKB_RestoreAllItemReq(
    MMIIKB_RESTORE_REQ_T *restore_req_ptr
);

/*****************************************************************************/
//  Description : app send restore item data respone message to 1-key task
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: app send MSG_IKB_RESTORE_ITEM_DATA_REQ response to 1-key
/*****************************************************************************/
PUBLIC MMIIKB_ERROR_E MMIAPIIKB_SendRestoreItemDataRes(
    MMIIKB_RESTORE_ITEM_DATA_RES_T * restore_item_data_res
);

/*****************************************************************************/
//  Description : 1-key send cancel action (backup or restore) req by app
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: app send MSG_IKB_CANCEL_ACTION_REQ request to 1-key
/*****************************************************************************/
PUBLIC MMIIKB_ERROR_E MMIAPIIKB_CancelActionReq(
    MMIIKB_TASK_TYPE_E task_type
);

/*****************************************************************************/
//  Description : MMIAPIIKB_PrintRegTable
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIIKB_PrintRegTable(void);






/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/

#ifdef   __cplusplus
    }
#endif

#endif  //_MMIIKEYBACKUP_API_H_

#endif  //#if defined(IKEYBACKUP_SUPPORT)
