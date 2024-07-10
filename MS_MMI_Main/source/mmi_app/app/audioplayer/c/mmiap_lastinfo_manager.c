/*****************************************************************************
** File Name:      mmimp3_historylist.c                                      *
** Author:                                                                   *
** Date:           5/29/2012                                                 *
** Copyright:      2010 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to mp3 history list                     *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 5/2012         robert.wang        Create
******************************************************************************/


#ifdef MMIAP_F_HISTORY_PLAYLIST

#define MMIMP3_HISTORYLIST_C
/*---------------------------------------------------------------------------*/
/*                          Include Files                                    */
/*---------------------------------------------------------------------------*/
#include "mmimp3_nv.h"
#include "mmiap_lastinfo_manager.h"
#include "mmiap_list.h"

/*---------------------------------------------------------------------------*/
/*                         STATIC DEFINITION                                 */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

LOCAL MMIAP_HISTORY_NV_INFO_T s_mp3_history_nv_info = {0};


/*---------------------------------------------------------------------------*/
/*                          FUNCTION DEFINITION                              */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : get history default nv info
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL void GetDefaultHistoryNVInfo(
    MMIAP_HISTORY_NV_INFO_T *info_ptr //out
)
{
    if(PNULL != info_ptr)
    {
        info_ptr->list_dev = MMI_DEVICE_NUM;
    }
}

/*****************************************************************************/
//  Description : save history nv info
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL void SaveHistoryNVInfo(
    MMIAP_HISTORY_NV_INFO_T *info_ptr //in
)
{
    if(PNULL != info_ptr)
    {
        MMINV_WRITE(MMINV_AP_LAST_INFO, info_ptr);
    }
}

/*****************************************************************************/
//  Description : get history nv info
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL void GetHistoryNVInfo(
    MMIAP_HISTORY_NV_INFO_T *info_ptr //out
)
{
    uint32 nv_id = MMINV_AP_LAST_INFO;
    MN_RETURN_RESULT_E  return_value = MN_RETURN_FAILURE;

    if(PNULL == info_ptr)
    {
        return;
    }

    MMINV_READ(nv_id, info_ptr, return_value);

    if(MN_RETURN_SUCCESS != return_value)
    {
        GetDefaultHistoryNVInfo(info_ptr);

        SaveHistoryNVInfo(info_ptr);
    }
}

/*****************************************************************************/
//  Description : get history list saved device
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL MMIFILE_DEVICE_E GetHistoryListSavedDevice(void)
{
    MMIAP_HISTORY_NV_INFO_T nv_info = {0};
    MMIFILE_DEVICE_E dev_type = MMI_DEVICE_NUM;

    GetHistoryNVInfo(&nv_info);
    dev_type = nv_info.list_dev;

    SCI_TRACE_LOW("[MMIMP3] GetHistoryListSavedDevice dev_type=%d",
                  dev_type);

    return dev_type;
}

/*****************************************************************************/
//  Description : 获取存放历史记录列表文件的路径
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetAPHistoryFileNameForRead(
    wchar          *full_path_name_ptr,//out
    uint16          *full_path_len_ptr //out
)
{
    BOOLEAN         result      =   FALSE;

    if(PNULL != full_path_name_ptr && PNULL != full_path_len_ptr)
    {
        result = MMIAP_GetSysFileFullPathForRead(
                     GetHistoryListSavedDevice(),
                     MMIAP_DEFAULT_DIR,
                     MMIAP_DEFAULT_DIR_LEN,
                     MMIAP_HISTORY_FILENAME,
                     MMIAP_HISTORY_FILENAME_LEN,
                     full_path_name_ptr,
                     full_path_len_ptr
                 );
    }

    SCI_TRACE_LOW("[MMIMP3 GetAPHistoryFileNameForRead result=%d",
                  result);

    return result;
}

/*****************************************************************************/
//  Description : 获取存放历史记录列表文件的路径
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetAPHistoryFileNameForWrite(
    wchar          *full_path_name_ptr,//out
    uint16          *full_path_len_ptr //out
)
{
    BOOLEAN         result          = FALSE;
    uint32          min_free_size   = 0;

    if(PNULL != full_path_len_ptr && PNULL != full_path_name_ptr)
    {
        min_free_size = sizeof(MMIAP_HISTORY_INFO_T);
        SCI_TRACE_LOW("[MMIMP3] GetAPHistoryFileNameForWrite min_free_size = %d", \
                      min_free_size);

        result = MMIAPIFMM_GetSysFileFullPathForWrite(
                     TRUE,
                     MMIAP_DEFAULT_DIR,
                     MMIAP_DEFAULT_DIR_LEN,
                     MMIAP_HISTORY_FILENAME,
                     MMIAP_HISTORY_FILENAME_LEN,
                     min_free_size,
                     full_path_name_ptr,
                     full_path_len_ptr
                 );

        if(result)
        {
            s_mp3_history_nv_info.list_dev = MMIAPIFMM_GetDeviceTypeByPath(full_path_name_ptr, \
                                             MMIFILE_DEVICE_LEN
                                                                          );
            SCI_TRACE_LOW("[MMIMP3] GetAPHistoryFileNameForWrite list_dev = %d", \
                          s_mp3_history_nv_info.list_dev);
        }
    }

    SCI_TRACE_LOW("[MMIMP3] GetAPHistoryFileNameForWrite result = %d", \
                  result);

    return result;
}

/*****************************************************************************/
//  Description : 保存历史记录列表信息到默认文件中
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAP_SaveToHistoryFile(
    MMIAP_HISTORY_IN_PARAM_INFO_T *param_ptr //in
)
{
    MMIFILE_HANDLE              file_handle                 = SFS_INVALID_HANDLE;
    wchar                       abs_default_path[MMIFILE_FILE_NAME_MAX_LEN + 1] = {0};
    uint16                      abs_default_path_length     = MMIFILE_FILE_NAME_MAX_LEN;
    uint32                      write_size                  = 0;
    uint32                      writed_bytes                 = 0;
    MMIFILE_ERROR_E             file_write_result           = SFS_ERROR_NONE;
    BOOLEAN read_file_ret = FALSE;


    MMIAP_HISTORY_INFO_T his_info = {0};

    BOOLEAN is_ok = FALSE;



    if(PNULL == param_ptr)
    {
        return;
    }


    //获取历史记录列表文件的文件名信息

    do
    {
        read_file_ret = GetAPHistoryFileNameForWrite(abs_default_path, &abs_default_path_length);

        if(!read_file_ret)
        {
            break;
        }

        //删除默认文件
        if(MMIAPIFMM_IsFileExist(abs_default_path, abs_default_path_length))
        {
            MMIAPIFMM_DeleteFile(abs_default_path, PNULL);
        }


        //重新创建并打开默认文件
        file_handle = MMIAPIFMM_CreateFile(
                          abs_default_path,
                          SFS_MODE_WRITE | SFS_MODE_CREATE_ALWAYS, /*lint !e655*/
                          NULL,
                          NULL);

        if(SFS_INVALID_HANDLE == file_handle)
        {
            SCI_TRACE_LOW("[MMIMP3] MMIAP_SaveToHistoryFile file_handle fail");
            break;
        }

        //获取历史记录列表文件头信息;
        //将历史记录列表文件头信息写入文件
        SCI_MEMSET(&his_info, 0, sizeof(his_info));

        SCI_MEMCPY(his_info.header.version,
                   MMIAP_HISTORY_VER,
                   MMIAP_HISTORY_VER_LEN);

        //copy context info
        his_info.context = param_ptr->context;


        write_size = sizeof(his_info);
        file_write_result = MMIAPIFMM_WriteFile(file_handle,
                                                &his_info,
                                                write_size,
                                                &writed_bytes,
                                                PNULL);

        if((SFS_ERROR_NONE == file_write_result)
                && (write_size == writed_bytes))
        {
            is_ok = TRUE;

            SaveHistoryNVInfo(&s_mp3_history_nv_info);

            SCI_TRACE_LOW("[MMIMP3] MMIAP_SaveToHistoryFile SUCESS");
        }
    }
    while(0);


    if(file_handle)
    {
        MMIAPIFMM_CloseFile(file_handle);
    }

    if(!is_ok)
    {
        MMIAPIFMM_DeleteFile(abs_default_path, PNULL);
    }


    SCI_TRACE_LOW("[MMIMP3] MMIAP_SaveToHistoryFile read_file_ret = %d", \
                  read_file_ret);
    SCI_TRACE_LOW("[MMIMP3] MMIAP_SaveToHistoryFile file_write_result = %d", \
                  file_write_result);

    SCI_TRACE_LOW("[MMIMP3] MMIAP_SaveToHistoryFile is_ok = %d", \
                  is_ok);
}

/*****************************************************************************/
//  Description : 读取历史记录信息
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAP_ReadFromHistoryFile(
    MMIAP_HISTORY_OUT_PARAM_INFO_T *param_ptr //out
)
{
    BOOLEAN                     read_result             = FALSE;
    wchar                      full_path_name[MMIFILE_FILE_NAME_MAX_LEN + 1] = {0};
    uint16                      full_path_len               = MMIFILE_FILE_NAME_MAX_LEN;
    MMIFILE_HANDLE              file_handle                 = 0;
    uint32                      read_size                   = 0;
    uint32                      readed_bytes                = 0;
    MMIFILE_ERROR_E             file_read_result = 0;

    MMIAP_HISTORY_INFO_T his_info = {0};

    BOOLEAN is_ok = FALSE;



    if(PNULL == param_ptr
      )
    {
        return read_result;
    }


    //  1.查找判断默认文件是否被破坏
    //获取默认文件全路径
    do
    {
        read_result = GetAPHistoryFileNameForRead(full_path_name, &full_path_len);

        if(!read_result)
        {
            break;
        }

        //打开文件
        file_handle = MMIAPIFMM_CreateFile(full_path_name,
                                           SFS_MODE_READ | SFS_MODE_OPEN_EXISTING,
                                           NULL,
                                           NULL);   /*lint !e655*/

        if(SFS_INVALID_HANDLE == file_handle)
        {
            SCI_TRACE_LOW("[MMIMP3] MMIAP_ReadFromHistoryFile file_handle fail");
            break;
        }


        //读取info,
        read_size = sizeof(his_info);
        file_read_result = MMIAPIFMM_ReadFile(file_handle,
                                              &his_info,
                                              read_size,
                                              &readed_bytes,
                                              NULL
                                             );

        if((SFS_ERROR_NONE != file_read_result)
                || (read_size != readed_bytes)
          )
        {
            SCI_TRACE_LOW("[MMIMP3] MMIAP_ReadFromHistoryFile read file fail");
            break;
        }

        //检验版本是否正确，数据是否完整
        if(0 != MMIAPICOM_Stricmp((uint8 *)his_info.header.version,
                                  (uint8 *)MMIAP_HISTORY_VER)
          )
        {
            SCI_TRACE_LOW("[MMIMP3] MMIAP_ReadFromHistoryFile ver error");

            break;
        }

        param_ptr->context = his_info.context;

        is_ok = TRUE;
    }
    while(0);


    if(file_handle)
    {
        MMIAPIFMM_CloseFile(file_handle);
    }


    SCI_TRACE_LOW("[MMIMP3] MMIAP_ReadFromHistoryFile read_result=%d",
                  read_result);
    SCI_TRACE_LOW("[MMIMP3] MMIAP_ReadFromHistoryFile is_ok=%d",
                  is_ok);

    return is_ok;
}

/*****************************************************************************/
//  Description : reset nv
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAP_ResetLastInfoMgrNV(
    MMIAP_HISTORY_NV_INFO_T *info_ptr //out
)
{
        GetDefaultHistoryNVInfo(info_ptr);

        SaveHistoryNVInfo(info_ptr);
}        
#endif  //#ifdef MMIAP_F_HISTORY_PLAYLIST

