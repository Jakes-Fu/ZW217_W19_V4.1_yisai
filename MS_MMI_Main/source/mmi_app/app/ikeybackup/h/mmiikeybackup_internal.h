/****************************************************************************
** File Name:      mmiikeybackup_internal.h                                 *
** Author:         haiwu.chen                                               *
** Date:           12/25/2011                                               *
** Copyright:      2007 Spreadtrum, Incorporated. All Rights Reserved.      *
** Description:                                                             *
*****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 12/25/2011     haiwu.chen        Create                                 *
****************************************************************************/

#ifdef IKEYBACKUP_SUPPORT

#ifndef _MMIIKEYBACKUP_INTERNAL_H_
#define _MMIIKEYBACKUP_INTERNAL_H_


/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmiikeybackup_api.h"


/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/

//package content information, read bytes from in top of "pb.ib"
typedef struct
{
    wchar content_name[MMIIKB_PACK_CONTENT_NAME_LEN];  
                                            /* package content name         */
    MMIIKB_TYPE_E type;                     /* one type                     */
    MMIIKB_OPERATE_TYPE_E op_type;          /* backup operate-type          */      
    uint32 size;                            /* buf total sizes.             */
    uint32 number;                          /* option, used in app which 
                                               can be counted in buf op-type*/
    uint32 trans_cnt;                       /* buf transfer unit count.     */
    BOOLEAN is_enc;                         /* this item is encrypt or not. */
    
    BOOLEAN is_zip;                         /* option, dir must be true     */
    wchar content_path[MMIIKB_CON_PATH_LEN];/* option, used in dir or file type*/
    uint32 file_size;                       /* option, used in file op-type.*/    
    
    uint32 reserved1;                       /* Reserved1.                   */
    uint32 reserved2;                       /* Reserved2.                   */
    uint32 reserved3;                       /* Reserved3.                   */
    
}MMIIKB_PACK_CONT_HEAD_T;

//package content unit info, read bytes after content head of "pb.ib"
typedef struct
{
    uint32 len;                             /* content uint len.            */
    uint32 number;                          /* option, content number.      */
    uint32 reserved1;                       /* Reserved1.                   */
    uint32 reserved2;                       /* Reserved2.                   */
    void *buf_ptr;                          /* point to buffer from app.    */
    uint32 end_sign1;                       /* end sign1.                   */
    uint32 end_sign2;                       /* end sign2.                   */
    
}MMIIKB_PACK_CONT_UNIT_T;

//checking data before backup
typedef struct
{
    MMIIKB_TYPE_T types;                    /* all types of this pack backup*/
    BOOLEAN is_need_safe_inbox;
    BOOLEAN is_safe_inbox_legal;
    BOOLEAN is_need_private;
    BOOLEAN is_private_legal;    
    MMIIKB_PASSWORD_INFO_T psw_info;        /* password info.               */
    
}MMIIKB_PRE_BACKUP_DATA_T;

//task state
typedef enum
{
    MMIIKB_TASK_STATE_NONE,
    MMIIKB_TASK_STATE_START,                    // one item begin operating
    MMIIKB_TASK_STATE_PROCESSING,               // one item operating
    MMIIKB_TASK_STATE_END,                      // one item end operating
    MMIIKB_TASK_STATE_CACEL,                    // one item cancel operating
    MMIIKB_TASK_STATE_FAIL,                     // one item end operating
    
    MMIIKB_TASK_STATE_MAX,
}MMIIKB_TASK_STATE_E;

typedef struct
{
    MMIIKB_TYPE_E type;
    MMIIKB_TASK_TYPE_E task_type;
    MMIIKB_TASK_STATE_E task_state;
    uint32 trans_number;
    uint32 total_number;
    uint32 trans_size;
    uint32 total_size;
    uint32 trans_cnt;
    uint32 total_cnt;
    uint32 crc;
}MMIIKB_TASK_SEQ_TABLE_T;


typedef struct
{
    _SIGNAL_VARS

    MMIIKB_BACKUP_REQ_T backup_req;
}MMIIKB_SIG_BACKUP_REQ_T;

typedef struct
{
    _SIGNAL_VARS

    MMIIKB_RESTORE_REQ_T restore_req;
}MMIIKB_SIG_RESTORE_REQ_T;

typedef struct
{
    _SIGNAL_VARS

    MMIIKB_BACKUP_ITEM_DATA_RES_T backup_item_data_res;
}MMIIKB_SIG_BACKUP_ITEM_DATA_RES_T;

typedef struct
{
    _SIGNAL_VARS

    MMIIKB_RESTORE_ITEM_DATA_RES_T restore_item_data_res;
}MMIIKB_SIG_RESTORE_ITEM_DATA_RES_T;

typedef struct
{
    _SIGNAL_VARS

    MMIIKB_TASK_SEQ_TABLE_T seq_task_item;  /* 1-key to mmi just show prog. */
}MMIIKB_SIG_PROGRESS_RES_T;

typedef struct
{
    _SIGNAL_VARS

    MMIIKB_TASK_TYPE_E task_type;
}MMIIKB_SIG_CANCEL_REQ_T;


/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/



/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
 
/*****************************************************************************/
//  Description : 1-key backup init config nv value
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC void MMIIKB_InitConfigInfo(void);
 
/*****************************************************************************/
//  Description : 1-key backup get config nv value
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:获取程序的配置NV值，如果取不到，则设置一个默认值，并帮忙写入NV
/*****************************************************************************/
PUBLIC void MMIIKB_GetConfigNVInfo(MMIIKB_CONFIG_T *cfg_ptr);

/*****************************************************************************/
//  Description : 1-key backup set config nv value
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC void MMIIKB_SetConfigNVInfo(MMIIKB_CONFIG_T *cfg_ptr);

/*****************************************************************************/
//  Description : 1-key backup recover config nv value
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIIKB_RecoverConfigInfo(MMIIKB_CONFIG_T *cfg_ptr);

/*****************************************************************************/
//  Description : 1-key backup get current backup dir, e:\ibphone
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: path_ptr: out
/*****************************************************************************/
PUBLIC BOOLEAN MMIIKB_GetCurBaseDir(uint16 *path_ptr, uint16 *path_len_ptr);

/*****************************************************************************/
//  Description : MMIIKB_GetTypeProcessParamPtr
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC const MMIIKB_TYPE_PROCESS_PARAM_T* MMIIKB_GetTypeProcessParamPtr(
    MMIIKB_TYPE_E type
);

/*****************************************************************************/
//  Description : MMIIKB_GetTaskItemByType
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC const MMIIKB_TASK_SEQ_TABLE_T * MMIIKB_GetTaskItemByType(
    MMIIKB_TYPE_E type
);


//load all package
/*****************************************************************************/
//  Description : MMIIKB_CreateAllPackArray
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC void MMIIKB_CreateAllPackArray(void);

/*****************************************************************************/
//  Description : MMIIKB_DestroyAllPackArray
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC void MMIIKB_DestroyAllPackArray(void);

/*****************************************************************************/
//  Description : MMIIKB_GetAllPackArray
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC FILEARRAY MMIIKB_GetAllPackArray(void);

/*****************************************************************************/
//  Description : MMIIKB_GetAllPackNum
//  Global resource dependence :
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMIIKB_GetAllPackNum(void);

/*****************************************************************************/
//  Description : MMIIKB_GetPackFileInfo
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIIKB_GetPackFileInfo(
    uint32 index,
    FILEARRAY_DATA_T *file_info_ptr  //in/out
);

/*****************************************************************************/
//  Description : MMIIKB_ReadPackFileInfo
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIIKB_ReadPackFileInfo(
    uint16 *pack_path_ptr,
    uint16 pack_path_len,
    MMIIKB_PACK_INFO_T *pack_info_ptr  //out
);

/*****************************************************************************/
//  Description : MMIIKB_GetPackContentNumber
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIIKB_GetPackContentNumber(
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

#endif //IKEYBACKUP_SUPPORT

#endif //_MMIIKEYBACKUP_INTERNAL_H_


