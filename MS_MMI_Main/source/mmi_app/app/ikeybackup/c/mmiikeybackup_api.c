/*****************************************************************************
** File Name:      mmiikeybackup_api.c                                       *
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

#include "mmi_app_ikeybackup_trc.h"
#if defined(IKEYBACKUP_SUPPORT)

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmiikeybackup_api.h"
#include "mmiikeybackup_internal.h"
#include "mmiikeybackup_nv.h"
#include "mmiikeybackup_menutable.h"
#include "mmiikeybackup_id.h"
#include "mmiikeybackup_task.h"
#include "mmi_nv.h"


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


/**--------------------------------------------------------------------------*
 **                         TYPE DEFINITION                                  *
 **--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         LOCAL VARIABLE DEFINITION                        *
 **--------------------------------------------------------------------------*/
//read from nv
LOCAL MMIIKB_CONFIG_T s_ikb_config = {0};

//register from app
LOCAL MMIIKB_TYPE_PROCESS_PARAM_T 
    s_ikb_type_process_param[MMIIKB_TYPE_MAX] = {0};

LOCAL MMIIKB_PACK_INFO_T *s_pack_node_list_ptr = PNULL;

//load all package info
LOCAL FILEARRAY s_ikb_pack_array = PNULL;

/**--------------------------------------------------------------------------*
 **                         LOCAL FUNCTION DECLARE                           *
 **--------------------------------------------------------------------------*/





/**--------------------------------------------------------------------------*
 **                         EXTERNAL FUNCTION DEFINITION                     *
 **--------------------------------------------------------------------------*/
 
/*****************************************************************************/
//  Description : 1-key backup init module
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIIKB_InitModule(void)
{
    //SCI_TRACE_LOW:"[MMIIKB] MMIAPIIKB_InitModule begin."
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_API_71_112_2_18_2_24_21_0,(uint8*)"");
        
    //register menu
    MMIIKB_RegMenuGroup();

    //register nv
    MMIIKB_RegNv();
    
    //register win id name
    MMIIKB_RegWinIdNameArr();
}

/*****************************************************************************/
//  Description : 1-key backup init
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIIKB_Init(void)
{
    //SCI_TRACE_LOW:"[MMIIKB] MMIAPIIKB_Init begin."
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_API_89_112_2_18_2_24_21_1,(uint8*)"");
    
    MMIIKB_InitConfigInfo();

    MMIIKB_GetConfigNVInfo(&s_ikb_config);
    
    MMIIKB_RecoverConfigInfo(&s_ikb_config);

    MMIAPIIKB_ApplicationStart();
}

/*****************************************************************************/
//  Description : 1-key backup register by app
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIIKB_RegTypeFunc(MMIIKB_TYPE_PROCESS_PARAM_T *param_ptr)
{
    uint16  i = 0;
    BOOLEAN is_ret = FALSE;

    if (PNULL == param_ptr)
    {
        //SCI_TRACE_LOW:"[MMIIKB] MMIAPIIKB_RegTypeFunc param is null."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_API_111_112_2_18_2_24_22_2,(uint8*)"");
        return is_ret;
    }

    if (MMIIKB_TYPE_MAX <= param_ptr->type
        || PNULL == param_ptr->backup_call_back
        || PNULL == param_ptr->restore_call_back
        || PNULL == param_ptr->get_total_call_back)
    {
        //SCI_TRACE_LOW:"[MMIIKB] MMIAPIIKB_RegTypeFunc param is invalid."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_API_120_112_2_18_2_24_22_3,(uint8*)"");
        return is_ret;
    }

    i = (uint16)param_ptr->type;

    s_ikb_type_process_param[i].type = param_ptr->type;
    s_ikb_type_process_param[i].op_type = param_ptr->op_type;
    s_ikb_type_process_param[i].backup_call_back = param_ptr->backup_call_back;
    s_ikb_type_process_param[i].is_backup_asyn = param_ptr->is_backup_asyn;
    s_ikb_type_process_param[i].restore_call_back=param_ptr->restore_call_back;
    s_ikb_type_process_param[i].is_restore_asyn = param_ptr->is_restore_asyn;
    s_ikb_type_process_param[i].get_total_call_back
        = param_ptr->get_total_call_back;
    
    //SCI_TRACE_LOW:"[MMIIKB] MMIAPIIKB_RegTypeFunc type=%d, op=%d, backup=%d,is_ba_asy=%d, restore=%d, is_re_asy=%d, total_fun=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_API_139_112_2_18_2_24_22_4,(uint8*)"ddddddd", param_ptr->type, param_ptr->op_type, param_ptr->backup_call_back,param_ptr->is_backup_asyn, param_ptr->restore_call_back,param_ptr->is_restore_asyn, param_ptr->get_total_call_back);

    is_ret = TRUE;
    return is_ret;
}
 
/*****************************************************************************/
//  Description : 1-key backup get global config value
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:获取程序的全局配置值，启动的时候就这么一个值
/*****************************************************************************/
PUBLIC MMIIKB_CONFIG_T * MMIAPIIKB_GetConfigInfo(void)
{
    return &s_ikb_config;
}

/*****************************************************************************/
//  Description : MMIAPIIKB_SuspendTask
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIAPIIKB_SuspendTask(void)
{
    return IKeyBackup_SuspendTask();
}

/*****************************************************************************/
//  Description : MMIAPIIKB_ResumeTask
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIAPIIKB_ResumeTask(void)
{
    return IKeyBackup_ResumeTask();
}
 
/*****************************************************************************/
//  Description : 1-key backup is do backup thing now
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:备份或者还原任务正在忙碌中
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIIKB_IsBusy(void)
{
    return IKeyBackup_IsBusy();
}
 
/*****************************************************************************/
//  Description : 1-key backup is do backup thing now
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:是否正在备份中
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIIKB_IsBackupBusy(void)
{
    return IKeyBackup_IsBackupBusy();
}
 
/*****************************************************************************/
//  Description : 1-key backup is do backup thing now
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:是否正在还原中
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIIKB_IsRestoreBusy(void)
{
    return IKeyBackup_IsRestoreBusy();
}

/*****************************************************************************/
//  Description : 1-key backup application start
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIIKB_ApplicationStart(void)
{
    //init task global var
    IKeyBackup_InitIKBAgent();
    
    //start thread
    IKeyBackup_StartIKBHandlerTask();
}

/*****************************************************************************/
//  Description : 1-key backup application terminate
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIIKB_ApplicationTerminate(void)
{
    //SCI_TRACE_LOW:"[MMIIKB] MMIAPIIKB_ApplicationTerminate."
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_API_200_112_2_18_2_24_22_5,(uint8*)"");
    
    //terminate thread
    SCI_ThreadExit();
}

/*****************************************************************************/
//  Description : 1-key backup all items request by app
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: app send MSG_IKB_BACKUP_DATA_REQ request to 1-key
/*****************************************************************************/
PUBLIC MMIIKB_ERROR_E MMIAPIIKB_BackupAllItemReq(
    MMIIKB_BACKUP_REQ_T *backup_req_ptr
)
{
    MMIIKB_ERROR_E error_code = MMIIKB_STATUS_INVALID_PARAM;

    if(PNULL == backup_req_ptr)
    {
        //SCI_TRACE_LOW:"[MMIIKB] MMIAPIIKB_BackupAllItemReq param is null."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_API_218_112_2_18_2_24_22_6,(uint8*)"");
        return error_code;
    }
    
    error_code = IKeyBackup_BackupAllItemReq(backup_req_ptr);
    
    return error_code;
}

/*****************************************************************************/
//  Description : app send item data to 1-key task
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: app send MSG_IKB_BACKUP_ITEM_DATA_RES data to 1-key
/*****************************************************************************/
PUBLIC MMIIKB_ERROR_E MMIAPIIKB_SendBackupItemDataRes(
    MMIIKB_BACKUP_ITEM_DATA_RES_T * backup_item_data_res
)
{
    MMIIKB_ERROR_E error_code = MMIIKB_STATUS_INVALID_PARAM;
    
    if(PNULL == backup_item_data_res)
    {
        //SCI_TRACE_LOW:"[MMIIKB] MMIAPIIKB_SendBackupItemDataRes param is null."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_API_239_112_2_18_2_24_22_7,(uint8*)"");
        return error_code;
    }

    error_code = IKeyBackup_SendBackupItemDataRes(backup_item_data_res);
    
    return error_code;
}

/*****************************************************************************/
//  Description : 1-key restore all items request by app
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: app send MSG_IKB_RESTORE_DATA_REQ request to 1-key
/*****************************************************************************/
PUBLIC MMIIKB_ERROR_E MMIAPIIKB_RestoreAllItemReq(
    MMIIKB_RESTORE_REQ_T *restore_req_ptr
)
{
    MMIIKB_ERROR_E error_code = MMIIKB_STATUS_INVALID_PARAM;
    
    if(PNULL == restore_req_ptr)
    {
        //SCI_TRACE_LOW:"[MMIIKB] MMIAPIIKB_RestoreAllItemReq param is null."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_API_260_112_2_18_2_24_22_8,(uint8*)"");
        return error_code;
    }
    
    error_code = IKeyBackup_RestoreAllItemReq(restore_req_ptr);
    
    return error_code;
}

/*****************************************************************************/
//  Description : app send restore item data respone message to 1-key task
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: app send MSG_IKB_RESTORE_ITEM_DATA_REQ response to 1-key
/*****************************************************************************/
PUBLIC MMIIKB_ERROR_E MMIAPIIKB_SendRestoreItemDataRes(
    MMIIKB_RESTORE_ITEM_DATA_RES_T * restore_item_data_res
)
{
    MMIIKB_ERROR_E error_code = MMIIKB_STATUS_INVALID_PARAM;
    
    if(PNULL == restore_item_data_res)
    {
        //SCI_TRACE_LOW:"[MMIIKB] MMIAPIIKB_SendRestoreItemDataRes param is null."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_API_281_112_2_18_2_24_22_9,(uint8*)"");
        return error_code;
    }

    error_code = IKeyBackup_SendRestoreItemDataRes(restore_item_data_res);
    
    return error_code;
}

/*****************************************************************************/
//  Description : 1-key send cancel action (backup or restore) req by app
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: app send MSG_IKB_CANCEL_ACTION_REQ request to 1-key
/*****************************************************************************/
PUBLIC MMIIKB_ERROR_E MMIAPIIKB_CancelActionReq(
    MMIIKB_TASK_TYPE_E task_type
)
{
    MMIIKB_ERROR_E error_code = MMIIKB_STATUS_INVALID_PARAM;
    
    if(MMIIKB_TASK_TYPE_NONE >= task_type
        || MMIIKB_TASK_TYPE_MAX <= task_type)
    {
        //SCI_TRACE_LOW:"[MMIIKB] MMIAPIIKB_CancelActionReq param is 0, and task_type=%d."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_API_304_112_2_18_2_24_22_10,(uint8*)"d", task_type);
        return error_code;
    }
    
    error_code = IKeyBackup_CancelActionReq(task_type);
    
    return error_code;
}

/*****************************************************************************/
//  Description : MMIAPIIKB_PrintRegTable
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIIKB_PrintRegTable()
{
    uint16  i = 0;
    MMIIKB_TYPE_PROCESS_PARAM_T *param_ptr = PNULL;
    
    for (i = 0; i < MMIIKB_TYPE_MAX; i++)
    {
        param_ptr = &s_ikb_type_process_param[i];
        
        //SCI_TRACE_LOW:"[MMIIKB] MMIAPIIKB_RegTypeFunc type=%d, op=%d, backup=%d,is_ba_asy=%d, restore=%d, is_re_asy=%d, total_fun=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_API_330_112_2_18_2_24_22_11,(uint8*)"ddddddd", param_ptr->type, param_ptr->op_type, param_ptr->backup_call_back,param_ptr->is_backup_asyn, param_ptr->restore_call_back,param_ptr->is_restore_asyn, param_ptr->get_total_call_back);
    }

    return;
}



/**--------------------------------------------------------------------------*
 **                         INTERNAL FUNCTION DEFINITION                     *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : 1-key backup init config nv value
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC void MMIIKB_InitConfigInfo(void)
{
    MMIIKB_CONFIG_T ikb_config = {0};
    MN_RETURN_RESULT_E return_value = MN_RETURN_SUCCESS;

    //init config info
    MMINV_READ(MMINV_IKEYBACKUP_CONFIG, &ikb_config, return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        MMINV_WRITE(MMINV_IKEYBACKUP_CONFIG, &ikb_config);
    }

    return;
}
 
/*****************************************************************************/
//  Description : 1-key backup get config nv value
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:获取程序的配置NV值，如果取不到，则设置一个默认值，并帮忙写入NV
/*****************************************************************************/
PUBLIC void MMIIKB_GetConfigNVInfo(MMIIKB_CONFIG_T *cfg_ptr)
{
    MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;

    //get the num of local time minus network time
    MMINV_READ(MMINV_IKEYBACKUP_CONFIG, cfg_ptr, return_value);
    
    return;
}

/*****************************************************************************/
//  Description : 1-key backup set config nv value
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC void MMIIKB_SetConfigNVInfo(MMIIKB_CONFIG_T *cfg_ptr)
{
    //write the num of local time minus network time
    MMINV_WRITE(MMINV_IKEYBACKUP_CONFIG, cfg_ptr);

    return;
}

/*****************************************************************************/
//  Description : 1-key backup recover config nv value
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIIKB_RecoverConfigInfo(MMIIKB_CONFIG_T *cfg_ptr)
{
    BOOLEAN is_recover = FALSE;
    
    if (PNULL == cfg_ptr)
    {
        //SCI_TRACE_LOW:"[MMIIKB] MMIIKB_RecoverConfigInfo param is null."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_API_396_112_2_18_2_24_22_12,(uint8*)"");
        return is_recover;
    }

    if (MMIIKB_STORE_POS_NONE == cfg_ptr->store_pos)
    {
        cfg_ptr->store_pos = MMIIKB_STORE_POS_SD_1;
        is_recover = TRUE;
    }

    //目前所有的内容都需要CRC检验，故为恒真，不由UI控制
    cfg_ptr->is_crc_check = TRUE;

    if (is_recover)
    {
        //SCI_TRACE_LOW:"[MMIIKB] MMIIKB_RecoverConfigInfo is recover."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_API_411_112_2_18_2_24_22_13,(uint8*)"");
        MMIIKB_SetConfigNVInfo(cfg_ptr);
    }

    return is_recover;
}

/*****************************************************************************/
//  Description : 1-key backup get current backup dir, e:\ibphone
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: path_ptr: out
/*****************************************************************************/
PUBLIC BOOLEAN MMIIKB_GetCurBaseDir(uint16 *path_ptr, uint16 *path_len_ptr)
{
    MMIIKB_CONFIG_T *cfg_ptr = MMIAPIIKB_GetConfigInfo();
    BOOLEAN is_ret = FALSE;
    BOOLEAN is_find = FALSE;
    uint16 *device_ptr = PNULL;
    uint16 device_name_len = 0;
    MMIFILE_DEVICE_E device_id = 0;

    if (PNULL == path_ptr
        || PNULL == path_len_ptr)
    {
        //SCI_TRACE_LOW:"[MMIIKB] MMIIKB_GetCurBaseDir param is null."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_API_434_112_2_18_2_24_22_14,(uint8*)"");
        return is_ret;
    }

    if (MMIIKB_STORE_POS_NONE == cfg_ptr->store_pos)
    {
        //SCI_TRACE_LOW:"[MMIIKB] MMIIKB_GetCurBaseDir: dir value=%d!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_API_441_112_2_18_2_24_22_15,(uint8*)"d", cfg_ptr->store_pos);
        return is_ret;
    }

    is_ret = TRUE;
    switch (cfg_ptr->store_pos)
    {
    case MMIIKB_STORE_POS_SD_1:
        device_id = MMI_DEVICE_SDCARD;
        is_find = TRUE;
        break;
        
    case MMIIKB_STORE_POS_SD_2:
        device_id = MMI_DEVICE_SDCARD_1;
        is_find = TRUE;
        break;
        
    case MMIIKB_STORE_POS_SERVER:
        is_ret = TRUE;
        break;
        
    default:
        is_ret = FALSE;
        break;
    }
    
    //SCI_TRACE_LOW:"[MMIIKB] MMIIKB_GetCurBaseDir: dir value=%d!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_API_468_112_2_18_2_24_22_16,(uint8*)"d", cfg_ptr->store_pos);

    if (is_ret && is_find)
    {
        device_ptr = MMIAPIFMM_GetDevicePath(device_id);
        device_name_len = MMIAPIFMM_GetDevicePathLen(device_id);
        MMIAPICOM_Wstrcpy(path_ptr, device_ptr);
        MMIAPICOM_Wstrcat(path_ptr, L":\\ibphone");
        *path_len_ptr = MMIAPICOM_Wstrlen(path_ptr);
    }
    
    return is_ret;
}

/*****************************************************************************/
//  Description : MMIIKB_GetTypeProcessParamPtr
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC const MMIIKB_TYPE_PROCESS_PARAM_T* MMIIKB_GetTypeProcessParamPtr(
    MMIIKB_TYPE_E type
)
{
    const MMIIKB_TYPE_PROCESS_PARAM_T *process_param_ptr = PNULL;
    uint32 i = 0;

    if (MMIIKB_TYPE_MAX <= type)
    {
        //SCI_TRACE_LOW:"[MMIIKB] MMIIKB_GetTypeProcessParamPtr type=%d wrong."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_API_496_112_2_18_2_24_22_17,(uint8*)"d", type);
        return PNULL;
    }

    i = (uint32)type;
    process_param_ptr = &(s_ikb_type_process_param[i]);

    if (MMIIKB_OPERATE_TYPE_NONE == process_param_ptr->op_type)
    {
        process_param_ptr = PNULL;
    }

    return process_param_ptr;
}



//load package
/*****************************************************************************/
//  Description : MMIIKB_CreateAllPackArray
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC void MMIIKB_CreateAllPackArray(void)
{
    if (PNULL != s_ikb_pack_array)
    {
        MMIAPIFILEARRAY_Destroy(&s_ikb_pack_array);
    }
    
    s_ikb_pack_array = MMIAPIFILEARRAY_Create();
}

/*****************************************************************************/
//  Description : MMIIKB_DestroyAllPackArray
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC void MMIIKB_DestroyAllPackArray(void)
{
    if (PNULL != s_ikb_pack_array)
    {
        MMIAPIFILEARRAY_Destroy(&s_ikb_pack_array);
        s_ikb_pack_array = PNULL;
    }
}

/*****************************************************************************/
//  Description : MMIIKB_GetAllPackArray
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC FILEARRAY MMIIKB_GetAllPackArray(void)
{
    return s_ikb_pack_array;
}

/*****************************************************************************/
//  Description : MMIIKB_GetAllPackNum
//  Global resource dependence :
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMIIKB_GetAllPackNum(void)
{
    uint16 num = 0;
    
    num = MMIAPIFILEARRAY_GetArraySize(s_ikb_pack_array);
        
    return num;
}

/*****************************************************************************/
//  Description : MMIIKB_GetPackFileInfo
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIIKB_GetPackFileInfo(
    uint32 index,
    FILEARRAY_DATA_T *file_info_ptr  //in/out
)
{
    BOOLEAN result = FALSE;

    if (PNULL != file_info_ptr)
    {
        result = MMIAPIFILEARRAY_Read(s_ikb_pack_array, index, file_info_ptr);
        if (result && 0 == file_info_ptr->name_len)
        {
            result = FALSE;
        }
    }

    return (result);
}

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
)
{
    BOOLEAN result = FALSE;
    MMIFILE_HANDLE file_handle = SFS_INVALID_HANDLE;
    MMIFILE_ERROR_E file_error = SFS_ERROR_NONE;
    uint32 read_size = 0;

    if (PNULL != pack_path_ptr
        && 0 != pack_path_len
        && PNULL != pack_info_ptr)
    {
        //read file info        
        file_handle = MMIAPIFMM_CreateFile(pack_path_ptr, 
            SFS_MODE_READ|SFS_MODE_OPEN_EXISTING, 0, 0);
        
        if (SFS_INVALID_HANDLE != file_handle)
        {
            file_error = MMIAPIFMM_ReadFile(file_handle, (void*)pack_info_ptr, 
                sizeof(MMIIKB_PACK_INFO_T), &read_size, PNULL);

            if (SFS_ERROR_NONE != file_error
                || read_size != sizeof(MMIIKB_PACK_INFO_T))
            {
                result = FALSE;
            }
            else
            {
                result = TRUE;
            }
        }

        MMIAPIFMM_CloseFile(file_handle);
    }

    return (result);
}

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
)
{
    uint32 total = 0;

    if (PNULL == pack_path_ptr
        || 0 == pack_path_len
        || MMIIKB_TYPE_MAX <= type)
    {
        return total;
    }

    total = IKeyBackup_GetPackContentNumber(pack_path_ptr, pack_path_len, type);

    return (total);
}


/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DEFINITION                        */
/*---------------------------------------------------------------------------*/







/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif  //#if defined(IKEYBACKUP_SUPPORT)

