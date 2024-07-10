/*****************************************************************************
** File Name:      watchface_config.c                                        *
** Author:         yifan.yang                                                *
** Date:           08/17/2021                                                *
** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to define app  store flow function      *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 8/2021       yifan.yang          Create                                   *
******************************************************************************/

/**--------------------------------------------------------------------------*
**                         Include Files                                     *
**---------------------------------------------------------------------------*/
#include "mmk_type.h"
#include "sfs.h"
#include "json_parser_config.h"
#include "watchface_config.h"
#include "mmicom_trace.h"
#include "os_api.h"
#include "mmi_string.h"
#ifdef UAL_BT_BLE_DEV_SUPPORT
#include "ual_bt_ble_dev.h"
#endif
#ifdef UAL_BT_BLE_PRIVATE_SUPPORT
#include "ual_bt_ble_private.h"
#endif
#include "mmi_filetask.h"
#include "watchface_http.h"
#ifdef THEME_UNZIP_SUPPORT
#include "theme_gzip_export.h"
#endif
#include "mmi_appmsg.h"
#include "json_parser_watchface.h"
#include "watchface_flow.h"
#include "watchface_factory_reset_waiting_win_view.h"
#include "mmiset_id.h"

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                  *
**---------------------------------------------------------------------------*/
#ifdef PLATFORM_ANTISW3
#define INSTALLED_WATCHFACE_CONFIG_FILE_PATH    L"C:\\ur_root\\watchface\\watchfacelist.json"
#define INSTALLED_WATCHFACE_FILES_FOLDER        "C:\\ur_root\\watchface"
#elif defined PLATFORM_UWS6121E
//217不支持C盘，故将表盘文件放到D盘
#define INSTALLED_WATCHFACE_CONFIG_FILE_PATH    L"D:\\ur_root\\watchface\\watchfacelist.json"
#define INSTALLED_WATCHFACE_FILES_FOLDER        "D:\\ur_root\\watchface"
#endif
#define UNINSTALLED_WATCHFACE_CONFIG_FILE_PATH  L"D:\\wf_temp\\uninstallwatchfacelist.json"
#define UNINSTALLED_WATCHFACE_FILES_FOLDER      "D:\\wf_temp"
#define DESCRIPTION_JSON_NAME                   "description.json"
#define JSON_NAME                               "watchface_layout.json"
#define ADD_BUFFER                              500
#define SET_BUFFER                              20
#define ZIP_SUFFIX                              L"zip"
#define ZIP_TOTAL_RATE                          100
/**--------------------------------------------------------------------------*
**                         TYPE AND CONSTANT                                 *
**---------------------------------------------------------------------------*/
/* Watchface message body for Debug Mode*/
typedef struct
{
    SIGNAL_VARS
    uint8       wf_name[WATCHFACE_WATCHFACE_ID_LEN_MAX + 1];
}WATCHFACE_DEBUG_SIG_T;

/**--------------------------------------------------------------------------*
**                          LOCAL VARIABLES                                  *
**---------------------------------------------------------------------------*/
LOCAL uint32   s_wf_handle = 0;//接收ble private 模块的消息handle
LOCAL APP_DELETE_HANDLE_T*  s_p_watchface_delete_handle = PNULL;// 删除文件的handle信息
LOCAL uint16                s_del_index                 = WATCHFACE_MAX_NUM;//删除表盘的index
LOCAL APP_COPY_HANDLE_T*    s_p_watchface_copy_handle   = PNULL;// 复制文件的handle信息
LOCAL uint16                s_copy_index                = WATCHFACE_MAX_NUM;//拷贝表盘的index
LOCAL WATCHFACE_CONFIG_TYPE_E s_watchface_type          = WATCHFACE_CONFIG_TYPE_MAX;//表盘的类型
LOCAL WATCHFACE_STATUS_RES_E  s_watchface_status        = WATCHFACE_STATUS_RES_MAX;//表盘的状态
LOCAL WATCHFACE_STATUS_RES_E  s_watchface_folder_status = WATCHFACE_STATUS_RES_MAX;//表盘文件的状态
LOCAL const char        s_watchdace_json_header_info[]  = "{\"app_info\"\:[]}";
LOCAL char  s_unzip_filename[WATCHFACE_WATCHFACE_ID_LEN_MAX + 1] = {0};
#ifdef THEME_UNZIP_SUPPORT
MMI_APPLICATION_T            g_watchgzip_app = {0};
#endif
MMI_APPLICATION_T            g_watchdebug_app = {0};

LOCAL WATCHFACE_INSTALL_STATUS_E   g_watchface_install_status = WATCHFACE_INSTALL_STATUS_IDLE;

/*---------------------------------------------------------------------------*
**                          LOCAL FUNCTION                                   *
**---------------------------------------------------------------------------*/
#ifndef WIN32
#ifdef OSA_SUPPORT
extern int ur_con_printf(const char *format, ...);
#endif
#endif

/*****************************************************************************/
//  Description : Add_WatchfaceCfg_Info
//  Parameter: [In] type,Json类型
//                  p_wf_cfg_info，表盘config信息
//             [Out]NONE
//             [Return]WATCHFACE_STATUS_RES_E
//  Author:yifan.yang
// Note:Add watchface config info
/*****************************************************************************/
LOCAL WATCHFACE_STATUS_RES_E Add_WatchfaceCfg_Info(WATCHFACE_CONFIG_TYPE_E type, COMMON_CONFIG_OBJECT_T* p_wf_cfg_info);

/*****************************************************************************/
//  Description : Del_WatchfaceCfg_Info
//  Parameter: [In] type,Json类型
//                  idx:表盘idx
//             [Out]None
//             [Return]WATCHFACE_STATUS_RES_E
//  Author:yifan.yang
// Note:Delete watchface config info
/*****************************************************************************/
LOCAL WATCHFACE_STATUS_RES_E Del_WatchfaceCfg_ByIdx(WATCHFACE_CONFIG_TYPE_E type, uint16 idx);
/*****************************************************************************/
//  Description : Get_Idx_ByWatchfaceId
//  Parameter: [In]watchface ID
//             [In]folder type
//             [Out]watchface index
//             [Return] WATCHFACE_STATUS_RES_E
//  Author:donglin.su
// Note:get watchface index by ID
/*****************************************************************************/
LOCAL WATCHFACE_STATUS_RES_E Get_Idx_ByWatchfaceId(WATCHFACE_CONFIG_TYPE_E type ,char* p_wf_id, uint16* p_index);

/*****************************************************************************/
//  Description : Start_Download
//  Parameter: [In]p_wf_cfg_info 表盘的id和url
//             [Out]NONE
//             [Return] WATCHFACE_STATUS_RES_E
//  Author:donglin.su
// Note:control watchface URL download
/*****************************************************************************/
LOCAL WATCHFACE_STATUS_RES_E Start_Download(COMMON_CONFIG_OBJECT_T* p_wf_cfg_info);
/*****************************************************************************/
//  Description : HandleWatchfaceGzipMsg
//  Parameter: [Out]NONE
//             [Return] MMI_RESULT_E
//  Author:qi.zhang
// Note:处理解压缩消息
/*****************************************************************************/
#ifdef THEME_UNZIP_SUPPORT
LOCAL MMI_RESULT_E HandleWatchfaceGzipMsg(PWND app_ptr, uint16 msg_id, DPARAM param);
#endif
/*****************************************************************************/
//  Description : HandleWatchfaceDebugMsg
//  Parameter: [Out]NONE
//             [Return] MMI_RESULT_E
//  Author:qi.zhang
// Note:处理安装卸载表盘的消息
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleWatchfaceDebugMsg(PWND app_ptr, uint16 msg_id, DPARAM param);

/*****************************************************************************/
//  Description : IsExsitSameWatchfaceID
//  Parameter: [In]watchface ID
//             [Out]none
//             [Return] BOOLEAN:TRUE,exist same watchface id
//  Author:chunjuan.liang
// Note:IsExsitSameWatchfaceID
/*****************************************************************************/
LOCAL BOOLEAN IsExsitSameWatchfaceID(char *p_wf_id);

/*---------------------------------------------------------------------------*
**                          GLOBAL FUNCTION                                  *
**---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : WATCHFACE_Get_ConfigFilePath
//  Parameter: [In] NONE
//             [Out] NONE
//             [Return]Config File Path
//  Author:yifan.yang
// Note:Get watchface config file path
/*****************************************************************************/
PUBLIC wchar* WATCHFACE_Get_ConfigFilePath(void)
{
    return INSTALLED_WATCHFACE_CONFIG_FILE_PATH;
}

/*****************************************************************************/
//  Description : WATCHFACE_Get_UninstalledConfigFilePath
//  Parameter: [In] NONE
//             [Out] NONE
//             [Return]Uninstalled Config File Path
//  Author:yifan.yang
// Note:Get watchface uninstalled config file path
/*****************************************************************************/
PUBLIC wchar* WATCHFACE_Get_UninstalledConfigFilePath(void)
{
    return UNINSTALLED_WATCHFACE_CONFIG_FILE_PATH;
}

/*****************************************************************************/
//  Description : WATCHFACE_Get_InstalledFileFolder
//  Parameter: [In] NONE
//             [Out] NONE
//             [Return]Installed File Folder Path
//  Author:yifan.yang
// Note:Get watchface installed file folder
/*****************************************************************************/
PUBLIC char* WATCHFACE_Get_InstalledFileFolder(void)
{
    return INSTALLED_WATCHFACE_FILES_FOLDER;
}

/*****************************************************************************/
//  Description : WATCHFACE_Get_UninstalledFileFolder
//  Parameter: [In] NONE
//             [Out] NONE
//             [Return]Uninstalled File Folder Path
//  Author:yifan.yang
// Note:Get watchface uninstalled file folder
/*****************************************************************************/
PUBLIC char* WATCHFACE_Get_UnintalledFileFolder(void)
{
    return UNINSTALLED_WATCHFACE_FILES_FOLDER;
}

/*****************************************************************************/
//  Description : WATHCFACE_Get_LayoutFileName
//  Parameter: [In] NONE
//             [Out] NONE
//             [Return]Layout File Name
//  Author:yifan.yang
// Note:Get watchface layout file name
/*****************************************************************************/
PUBLIC char* WATHCFACE_Get_LayoutFileName(void)
{
    return JSON_NAME;
}

/****************************************************************************
//  Description : WATCHFACE_ResFilesExist
//  Parameter:  [In]:
//              [Return]:  result    //函数返回值
//  Author: yifan.yang
//  Note:
*****************************************************************************/
LOCAL BOOLEAN WATCHFACE_ResFilesExist(void)
{
    uint16 i         = 0;
    uint16 count     = 0;
    uint16 cur_index = 0;
    COMMON_CONFIG_OBJECT_T*    p_info = PNULL;
    WATCHFACE_STATUS_RES_E get_result = WATCHFACE_STATUS_RES_MAX;
    WATCHFACE_STATUS_RES_E del_result = WATCHFACE_STATUS_RES_MAX;
    wchar watchface_folder_path_name[WATCHFACE_FILES_FULLPATH_LEN + 1] = {0};

    TRACE_WATCHFACE("[config]enter");

    count = WATCHFACE_Get_Watchface_Num();
    if (0 == count)
    {
        TRACE_WATCHFACE("[config]watchface num is 0");
        return FALSE;
    }

    p_info = (COMMON_CONFIG_OBJECT_T*)SCI_ALLOC_APPZ(sizeof(COMMON_CONFIG_OBJECT_T));
    if (PNULL == p_info)
    {
        TRACE_WATCHFACE("[config]p_info is null");
        return FALSE;
    }

    WATCHFACE_Get_CurWatchface_Index(&cur_index);
    TRACE_WATCHFACE("[config]current index is %d", cur_index);

    for (i = count; i > 0; i--)
    {
        SCI_MEMSET(p_info, 0x00, sizeof(COMMON_CONFIG_OBJECT_T));
        get_result = WATCHFACE_Get_WatchfaceCfg_Info(WATCHFACE_CONFIG_TYPE_INSTALLED, i - 1, p_info);
        if (WATCHFACE_STATUS_RES_SUCCESS != get_result)
        {
            TRACE_WATCHFACE("[config]get watchface config info failed by index %d", (i - 1));
            continue;
        }
        SCI_MEMSET(watchface_folder_path_name, 0x00, sizeof(wchar) * MMIAPICOM_Wstrlen(watchface_folder_path_name));
        MMIAPICOM_StrToWstr(p_info->url, watchface_folder_path_name);
        if(!MMIAPIFMM_IsFolderExist (watchface_folder_path_name, MMIAPICOM_Wstrlen(watchface_folder_path_name)))
        {
            del_result = Del_WatchfaceCfg_ByIdx(WATCHFACE_CONFIG_TYPE_INSTALLED, i - 1);
            if (WATCHFACE_STATUS_RES_SUCCESS == del_result)
            {
                TRACE_WATCHFACE("delete config info by index %d", (i - 1));
                if (cur_index > (i - 1))
                {
                    cur_index = cur_index - 1;
                }
                else if (cur_index == (i - 1))
                {
                    cur_index = 0;
                }
            }
            else
            {
                TRACE_WATCHFACE("delete config info failed by index %d", (i - 1));
            }
        }
    }
    if (PNULL != p_info)
    {
        SCI_FREE(p_info);
        p_info = PNULL;
    }

    WATCHFACE_Set_CurWatchface_Index(cur_index);

    TRACE_WATCHFACE("[config]exist");
    return TRUE;
}

/*****************************************************************************/
//  Description : WATCHFACE_Get_Watchface_Num
//  Parameter: [In] NONE
//             [Out] NONE
//             [Return]watchface number
//  Author:yifan.yang
// Note:Get watchface number
/*****************************************************************************/
PUBLIC uint16 WATCHFACE_Get_Watchface_Num(void)
{
    uint16              wf_num = 0;
    SFS_HANDLE src_file_handle = 0;
    uint32           file_size = 0;
    uint32        bytes_readed = 0;
    wchar    src_file_name[WATCHFACE_FILES_FULLPATH_LEN + 1] = {0};
    char*                p_buf = PNULL;
    uint16            path_len = 0;
    JSONPARSER_RESULT_E  json_result = JSON_PARSER_MAX;

    path_len = MMIAPICOM_Wstrlen(WATCHFACE_Get_ConfigFilePath());
    SCI_MEMCPY(src_file_name, WATCHFACE_Get_ConfigFilePath(), sizeof(wchar) * path_len);
    src_file_handle = SFS_CreateFile(src_file_name, (uint32)(SFS_MODE_READ|SFS_MODE_OPEN_EXISTING), NULL, NULL);//for read

    TRACE_WATCHFACE("[config]handle = %d",src_file_handle);
    if (0 == src_file_handle)
    {
        TRACE_WATCHFACE("[config]handle = 0");
        return 0;
    }
    SFS_GetFileSize(src_file_handle, &file_size);

    TRACE_WATCHFACE("[config]file_size = %d",file_size);

    p_buf = (char*)SCI_ALLOC_APPZ(file_size);

    if(PNULL == p_buf)
    {
        TRACE_WATCHFACE("[config]buf is NULL");
        return 0;
    }

    SFS_ReadFile(src_file_handle, p_buf, file_size,&bytes_readed, PNULL);
    SFS_CloseFile(src_file_handle);
    src_file_handle = 0;

    //read json
    json_result = JSONPARSER_GetConfigArrayCount(p_buf,&wf_num);

    if(PNULL != p_buf)
    {
        SCI_FREE(p_buf);
        p_buf = PNULL;
    }
    if (JSON_PARSER_SUCCESS != json_result)
    {
        TRACE_WATCHFACE("[config]read json fail");
        return 0;
    }

    TRACE_WATCHFACE("[config]watchface number is %d", wf_num);
    return wf_num;
}

/*****************************************************************************/
//  Description : WATCHFACE_Get_WatchfaceCfg_Info
//  Parameter: [In] type,Json类型
//                   idx,表盘index
//             [Out] p_wf_cfg_info，表盘config信息
//             [Return]WATCHFACE_STATUS_RES_E
//  Author:yifan.yang
// Note:Get watchface config info
/*****************************************************************************/
PUBLIC WATCHFACE_STATUS_RES_E WATCHFACE_Get_WatchfaceCfg_Info(WATCHFACE_CONFIG_TYPE_E type, uint16 idx, COMMON_CONFIG_OBJECT_T* p_wf_cfg_info)
{
    SFS_HANDLE src_file_handle = 0;
    uint32           file_size = 0;
    uint32        bytes_readed = 0;
    wchar    src_file_name[WATCHFACE_FILES_FULLPATH_LEN + 1] = {0};
    char*                p_buf = PNULL;
    uint16            totalnum = 0;
    uint16            path_len = 0;
    JSONPARSER_RESULT_E  json_result = JSON_PARSER_MAX;

    TRACE_WATCHFACE("[config]WATCHFACE_CONFIG_TYPE is %d, index is %d", type, idx);

    if (WATCHFACE_CONFIG_TYPE_INSTALLED == type)
    {
        path_len = MMIAPICOM_Wstrlen(WATCHFACE_Get_ConfigFilePath());
        SCI_MEMCPY(src_file_name, WATCHFACE_Get_ConfigFilePath(), sizeof(wchar) *path_len);
        totalnum = WATCHFACE_Get_Watchface_Num();
        TRACE_WATCHFACE("[config]totalnum is %d", totalnum);
    }
    else if (WATCHFACE_CONFIG_TYPE_UNINSTALLED == type)
    {
        path_len = MMIAPICOM_Wstrlen(WATCHFACE_Get_UninstalledConfigFilePath());
        SCI_MEMCPY(src_file_name, WATCHFACE_Get_UninstalledConfigFilePath(), sizeof(wchar)*path_len);
        totalnum = WATCHFACE_Get_UnWatchface_Num();
        TRACE_WATCHFACE("[config]totalnum is %d", totalnum);
    }
    else
    {
        TRACE_WATCHFACE("[config]WATCHFACE_STATUS_RES_PARAM_ERR");
        return WATCHFACE_STATUS_RES_PARAM_ERR;
    }

    if (0 == totalnum)
    {
        TRACE_WATCHFACE("[config]error,totalnum is 0.");
        return WATCHFACE_STATUS_RES_PARAM_ERR;
    }
    if ((totalnum - 1) < idx)
    {
        TRACE_WATCHFACE("[config]index is error");
        return WATCHFACE_STATUS_RES_PARAM_ERR;
    }

    if(PNULL == p_wf_cfg_info)
    {
        TRACE_WATCHFACE("[config]p_wf_cfg_info is NULL");
        return WATCHFACE_STATUS_RES_PARAM_ERR;
    }

    src_file_handle = SFS_CreateFile(src_file_name, (uint32)(SFS_MODE_READ|SFS_MODE_OPEN_EXISTING), NULL, NULL);//for read

    TRACE_WATCHFACE("[config]handle = %d",src_file_handle);
    if (0 == src_file_handle)
    {
        TRACE_WATCHFACE("[config]handle = 0");
        return WATCHFACE_STATUS_RES_FAIL;
    }
    SFS_GetFileSize(src_file_handle, &file_size);

    p_buf = (char*)SCI_ALLOC_APPZ(file_size);

    if(PNULL == p_buf)
    {
        TRACE_WATCHFACE("[config]buf is NULL");
        return WATCHFACE_STATUS_RES_PARAM_ERR;
    }

    SFS_ReadFile(src_file_handle, p_buf, file_size,&bytes_readed, PNULL);
    SFS_CloseFile(src_file_handle);
    src_file_handle = 0;

    TRACE_WATCHFACE("[config]index is %d", idx);
    //read json
    json_result = JSONPARSER_GetConfigObjectInfo(p_buf,idx,p_wf_cfg_info);

    if(PNULL != p_buf)
    {
        SCI_FREE(p_buf);
        p_buf = PNULL;
    }
    
    if (JSON_PARSER_SUCCESS != json_result)
    {
        TRACE_WATCHFACE("[config]read json fail");
        return WATCHFACE_STATUS_RES_FAIL;
    }

    return WATCHFACE_STATUS_RES_SUCCESS;
}

/*****************************************************************************/
//  Description : Add_WatchfaceCfg_Info
//  Parameter: [In] type,Json类型
//                  p_wf_cfg_info，表盘config信息
//             [Out]NONE
//             [Return]WATCHFACE_STATUS_RES_E
//  Author:yifan.yang
// Note:Add watchface config info
/*****************************************************************************/
LOCAL WATCHFACE_STATUS_RES_E Add_WatchfaceCfg_Info(WATCHFACE_CONFIG_TYPE_E type, COMMON_CONFIG_OBJECT_T* p_wf_cfg_info)
{
    SFS_HANDLE src_file_handle = 0;
    SFS_HANDLE src_file_handle_write = 0;
    uint32           file_size = 0;
    uint32        bytes_readed = 0;
    uint32        bytes_writen = 0;
    wchar    src_file_name[WATCHFACE_FILES_FULLPATH_LEN + 1] = {0};
    char*                p_buf = PNULL;
    uint16            path_len = 0;
    JSONPARSER_RESULT_E  json_result = JSON_PARSER_MAX;

    TRACE_WATCHFACE("[config]WATCHFACE_CONFIG_TYPE is %d", type);

    if (WATCHFACE_CONFIG_TYPE_INSTALLED == type)
    {
        path_len = MMIAPICOM_Wstrlen(WATCHFACE_Get_ConfigFilePath());
        SCI_MEMCPY(src_file_name, WATCHFACE_Get_ConfigFilePath(), sizeof(wchar) * path_len);
    }
    else if (WATCHFACE_CONFIG_TYPE_UNINSTALLED == type)
    {
        path_len = MMIAPICOM_Wstrlen(WATCHFACE_Get_UninstalledConfigFilePath());
        SCI_MEMCPY(src_file_name, WATCHFACE_Get_UninstalledConfigFilePath(), sizeof(wchar) * path_len);
    }
    else
    {
        TRACE_WATCHFACE("[config]WATCHFACE_STATUS_RES_PARAM_ERR");
        return WATCHFACE_STATUS_RES_PARAM_ERR;
    }

    if(PNULL == p_wf_cfg_info)
    {
        TRACE_WATCHFACE("[config]p_wf_cfg_info is NULL");
        return WATCHFACE_STATUS_RES_PARAM_ERR;
    }

    src_file_handle = SFS_CreateFile(src_file_name, (uint32)(SFS_MODE_READ|SFS_MODE_OPEN_EXISTING), NULL, NULL);//for read   

    TRACE_WATCHFACE("[config]handle = %d",src_file_handle);
    if (0 == src_file_handle)
    {
        TRACE_WATCHFACE("[config]handle = 0");
        return WATCHFACE_STATUS_RES_FILE_NOT_EXISTED;
    }
    SFS_GetFileSize(src_file_handle, &file_size);

    TRACE_WATCHFACE("[config]file_size = %d",file_size);

    p_buf = (char*)SCI_ALLOC_APPZ(file_size + ADD_BUFFER);

    if(PNULL == p_buf)
    {
        TRACE_WATCHFACE("[config]buf is NULL");
        return WATCHFACE_STATUS_RES_PARAM_ERR;
    }

    SFS_ReadFile(src_file_handle, p_buf, file_size,&bytes_readed, PNULL);
    SFS_CloseFile(src_file_handle);
    src_file_handle = 0;

    //read json
    json_result = JSONPARSER_AddConfigObject(p_buf,p_wf_cfg_info);
    if (JSON_PARSER_SUCCESS != json_result)
    {
        TRACE_WATCHFACE("[config]read json fail");
        if(PNULL != p_buf)
        {
            SCI_FREE(p_buf);
            p_buf = PNULL;
        }
        return WATCHFACE_STATUS_RES_FAIL;
    }

    SFS_DeleteFile(src_file_name,PNULL);

    src_file_handle_write = SFS_CreateFile (src_file_name, SFS_MODE_WRITE | SFS_MODE_CREATE_ALWAYS, NULL, NULL);
    TRACE_WATCHFACE("[config]handle = %d",src_file_handle_write);
    if (0 == src_file_handle_write)
    {
        TRACE_WATCHFACE("[config]handle = 0");
        if(PNULL != p_buf)
        {
            SCI_FREE(p_buf);
            p_buf = PNULL;
        }
        return WATCHFACE_STATUS_RES_FAIL;
    }

    SFS_WriteFile(src_file_handle_write,p_buf,strlen(p_buf),&bytes_writen,PNULL);
    SFS_CloseFile(src_file_handle_write);
    src_file_handle_write = 0;

    if(PNULL != p_buf)
    {
        SCI_FREE(p_buf);
        p_buf = PNULL;
    }
    return WATCHFACE_STATUS_RES_SUCCESS;
}

/*****************************************************************************/
//  Description : Del_WatchfaceCfg_Info
//  Parameter: [In] type,Json类型
//                  idx:表盘idx
//             [Out]None
//             [Return]WATCHFACE_STATUS_RES_E
//  Author:yifan.yang
// Note:Delete watchface config info
/*****************************************************************************/
LOCAL WATCHFACE_STATUS_RES_E Del_WatchfaceCfg_ByIdx(WATCHFACE_CONFIG_TYPE_E type, uint16 idx)
{
    SFS_HANDLE src_file_handle = 0;
    SFS_HANDLE src_file_handle_write = 0;
    uint32           file_size = 0;
    uint32        bytes_readed = 0;
    uint32        bytes_writen = 0;
    wchar    src_file_name[WATCHFACE_FILES_FULLPATH_LEN + 1] = {0};
    char*                p_buf = PNULL;
    uint16            path_len = 0;
    uint16            totalnum = 0;
    JSONPARSER_RESULT_E  json_result = JSON_PARSER_MAX;

    TRACE_WATCHFACE("[config]WATCHFACE_CONFIG_TYPE is %d, index is %d", type, idx);

    if (WATCHFACE_CONFIG_TYPE_INSTALLED == type)
    {
        path_len = MMIAPICOM_Wstrlen(WATCHFACE_Get_ConfigFilePath());
        SCI_MEMCPY(src_file_name, WATCHFACE_Get_ConfigFilePath(), sizeof(wchar) * path_len);
    }
    else if (WATCHFACE_CONFIG_TYPE_UNINSTALLED == type)
    {
        path_len = MMIAPICOM_Wstrlen(WATCHFACE_Get_UninstalledConfigFilePath());
        SCI_MEMCPY(src_file_name, WATCHFACE_Get_UninstalledConfigFilePath(), sizeof(wchar) * path_len);
    }
    else
    {
        TRACE_WATCHFACE("[config]WATCHFACE_STATUS_RES_PARAM_ERR");
        return WATCHFACE_STATUS_RES_PARAM_ERR;
    }

    totalnum = WATCHFACE_Get_Watchface_Num();
    if (0 == totalnum)
    {
        TRACE_WATCHFACE("[config]error,totalnum is 0.");
        return WATCHFACE_STATUS_RES_PARAM_ERR;
    }
    if((totalnum - 1) < idx)
    {
        TRACE_WATCHFACE("[config]index is error");
        return WATCHFACE_STATUS_RES_PARAM_ERR;
    }

    src_file_handle = SFS_CreateFile(src_file_name, (uint32)(SFS_MODE_READ|SFS_MODE_OPEN_EXISTING), NULL, NULL);//for read
    TRACE_WATCHFACE("[config]handle = %d",src_file_handle);
    if (0 == src_file_handle)
    {
        TRACE_WATCHFACE("[config]handle = 0");
        return WATCHFACE_STATUS_RES_FAIL;
    }
    SFS_GetFileSize(src_file_handle, &file_size);

    TRACE_WATCHFACE("[config]file_size = %d",file_size);

    p_buf = (char*)SCI_ALLOC_APPZ(file_size + SET_BUFFER);

    if(PNULL == p_buf)
    {
        TRACE_WATCHFACE("[config]buf is NULL");
        return WATCHFACE_STATUS_RES_PARAM_ERR;
    }

    SFS_ReadFile(src_file_handle, p_buf, file_size,&bytes_readed, PNULL);
    SFS_CloseFile(src_file_handle);
    src_file_handle = 0;

    TRACE_WATCHFACE("[config]index is %d", idx);
    //read json
    json_result = JSONPARSER_DeleteConfigObject(p_buf,idx);
    if (JSON_PARSER_SUCCESS != json_result)
    {
        TRACE_WATCHFACE("[config]read json fail");
        if(PNULL != p_buf)
        {
            SCI_FREE(p_buf);
            p_buf = PNULL;
        }
        return WATCHFACE_STATUS_RES_FAIL;
    }

    SFS_DeleteFile(src_file_name,PNULL);

    src_file_handle_write = SFS_CreateFile (src_file_name, SFS_MODE_WRITE | SFS_MODE_CREATE_ALWAYS, NULL, NULL);
    TRACE_WATCHFACE("[config]handle = %d",src_file_handle_write);
    if (0 == src_file_handle_write)
    {
        TRACE_WATCHFACE("[config]handle = 0");
        if(PNULL != p_buf)
        {
            SCI_FREE(p_buf);
            p_buf = PNULL;
        }
        return WATCHFACE_STATUS_RES_FAIL;
    }

    SFS_WriteFile(src_file_handle_write,p_buf,strlen(p_buf),&bytes_writen,PNULL);
    SFS_CloseFile(src_file_handle_write);
    src_file_handle_write = 0;

    if(PNULL != p_buf)
    {
        SCI_FREE(p_buf);
        p_buf = PNULL;
    }
    return WATCHFACE_STATUS_RES_SUCCESS;
}

/*****************************************************************************/
//  Description : WATCHFACE_Get_CurWatchface_Index
//  Parameter: [In] None
//             [Out]index
//             [Return]WATCHFACE_STATUS_RES_E
//  Author:yifan.yang
// Note:Get current watchface index
/*****************************************************************************/
PUBLIC WATCHFACE_STATUS_RES_E WATCHFACE_Get_CurWatchface_Index(uint16* index)
{
    uint16                 idx = 0;
    SFS_HANDLE src_file_handle = 0;
    uint32           file_size = 0;
    uint32        bytes_readed = 0;
    wchar    src_file_name[WATCHFACE_FILES_FULLPATH_LEN + 1] = {0};
    char*                p_buf = PNULL;
    uint16            path_len = 0;
    JSONPARSER_RESULT_E  json_result = JSON_PARSER_MAX;

    path_len = MMIAPICOM_Wstrlen(WATCHFACE_Get_ConfigFilePath());
    SCI_MEMCPY(src_file_name, WATCHFACE_Get_ConfigFilePath(), sizeof(wchar) * path_len);

    if(PNULL == index)
    {
        TRACE_WATCHFACE("[config]index is null");
        return WATCHFACE_STATUS_RES_PARAM_ERR;
    }

    src_file_handle = SFS_CreateFile(src_file_name, (uint32)(SFS_MODE_READ|SFS_MODE_OPEN_EXISTING), NULL, NULL);//for read

    TRACE_WATCHFACE("[config]handle = %d",src_file_handle);
    if (0 == src_file_handle)
    {
        TRACE_WATCHFACE("[config]handle = 0");
        return WATCHFACE_STATUS_RES_PARAM_ERR;
    }
    SFS_GetFileSize(src_file_handle, &file_size);

    TRACE_WATCHFACE("[config]file_size = %d",file_size);

    p_buf = (char*)SCI_ALLOC_APPZ(file_size);

    if(PNULL == p_buf)
    {
        TRACE_WATCHFACE("[config]buf is NULL");
        return WATCHFACE_STATUS_RES_PARAM_ERR;
    }

    SFS_ReadFile(src_file_handle, p_buf, file_size,&bytes_readed, PNULL);
    SFS_CloseFile(src_file_handle);
    src_file_handle = 0;

    //read json
    json_result = JSONPARSER_GetConfigCurrentWatchface(p_buf,&idx);

    if(PNULL != p_buf)
    {
        SCI_FREE(p_buf);
        p_buf = PNULL;
    }

    if (JSON_PARSER_SUCCESS != json_result)
    {
        TRACE_WATCHFACE("[config]read json fail");
        return WATCHFACE_STATUS_RES_FAIL;
    }
    
    TRACE_WATCHFACE("[config]current index is %d", idx);
    *index = idx;

    return WATCHFACE_STATUS_RES_SUCCESS;
}

/*****************************************************************************/
//  Description : WATCHFACE_Set_CurWatchface_Index
//  Parameter: [In] idx:表盘idx
//             [Out]None
//             [Return]WATCHFACE_STATUS_RES_E
//  Author:yifan.yang
// Note:Set current watchface index
/*****************************************************************************/
PUBLIC WATCHFACE_STATUS_RES_E WATCHFACE_Set_CurWatchface_Index(uint16 idx)
{
    SFS_HANDLE src_file_handle = 0;
    SFS_HANDLE src_file_handle_write = 0;
    uint32           file_size = 0;
    uint32        bytes_readed = 0;
    uint32        bytes_writen = 0;
    wchar    src_file_name[WATCHFACE_FILES_FULLPATH_LEN + 1] = {0};
    char*                p_buf = PNULL;
    uint16            path_len = 0;
    uint16            totalnum = 0;
    JSONPARSER_RESULT_E  json_result = JSON_PARSER_MAX;

    totalnum = WATCHFACE_Get_Watchface_Num();

    TRACE_WATCHFACE("[config]index is %d", idx);
    if (0 == totalnum)
    {
        TRACE_WATCHFACE("[config]error,totalnum is 0.");
        return WATCHFACE_STATUS_RES_PARAM_ERR;
    }
    if((totalnum - 1) < idx)
    {
        TRACE_WATCHFACE("[config]index is error");
        return WATCHFACE_STATUS_RES_PARAM_ERR;
    }

    path_len = MMIAPICOM_Wstrlen(WATCHFACE_Get_ConfigFilePath());
    SCI_MEMCPY(src_file_name, WATCHFACE_Get_ConfigFilePath(), sizeof(wchar) * path_len);
    src_file_handle = SFS_CreateFile(src_file_name, (uint32)(SFS_MODE_READ|SFS_MODE_OPEN_EXISTING), NULL, NULL);//for read

    TRACE_WATCHFACE("[config]handle = %d",src_file_handle);
    if (0 == src_file_handle)
    {
        TRACE_WATCHFACE("[config]handle = 0");
        return WATCHFACE_STATUS_RES_FAIL;
    }
    SFS_GetFileSize(src_file_handle, &file_size);

    TRACE_WATCHFACE("[config]file_size = %d",file_size);

    p_buf = (char*)SCI_ALLOC_APPZ(file_size + SET_BUFFER);

    if(PNULL == p_buf)
    {
        TRACE_WATCHFACE("[config]buf is NULL");
        if(0 != src_file_handle )
        {
            SFS_CloseFile(src_file_handle);
            src_file_handle = 0;
        }
        return WATCHFACE_STATUS_RES_PARAM_ERR;
    }

    SFS_ReadFile(src_file_handle, p_buf, file_size,&bytes_readed, PNULL);
    SFS_CloseFile(src_file_handle);
    src_file_handle = 0;

    TRACE_WATCHFACE("[config]index is %d", idx);
    //read json
    json_result = JSONPARSER_SetConfigCurrentWatchface(p_buf,idx);
    if (JSON_PARSER_SUCCESS != json_result)
    {
        TRACE_WATCHFACE("[config]read json fail");
        if(PNULL != p_buf)
        {
            SCI_FREE(p_buf);
            p_buf = PNULL;
        }
        return WATCHFACE_STATUS_RES_FAIL;
    }

    SFS_DeleteFile(src_file_name,PNULL);

    src_file_handle_write = SFS_CreateFile (src_file_name, SFS_MODE_WRITE | SFS_MODE_CREATE_ALWAYS, NULL, NULL);
    TRACE_WATCHFACE("[config]handle = %d",src_file_handle_write);
    if (0 == src_file_handle_write)
    {
        TRACE_WATCHFACE("[config]handle = 0");
        if(PNULL != p_buf)
        {
            SCI_FREE(p_buf);
            p_buf = PNULL;
        }
        return WATCHFACE_STATUS_RES_FAIL;
    }
    
    SFS_WriteFile(src_file_handle_write,p_buf,strlen(p_buf),&bytes_writen,PNULL);
    SFS_CloseFile(src_file_handle_write);
    src_file_handle_write = 0;
    //set s_p_display_info
    WATCHFACE_Construct_Layout_Data();
    if(PNULL != p_buf)
    {
        SCI_FREE(p_buf);
        p_buf = PNULL;
    }
    return WATCHFACE_STATUS_RES_SUCCESS;
}

/*****************************************************************************/
//  Description : WATCHFACE_Get_UnWatchface_Num
//  Parameter: [In]None
//             [Out]None
//             [Return] uninstalled watchface number
//  Author:yifan.yang
// Note:Get uninstall watchface number
/*****************************************************************************/
PUBLIC uint16 WATCHFACE_Get_UnWatchface_Num(void)
{
    uint16             uwf_num = 0;
    SFS_HANDLE src_file_handle = 0;
    uint32           file_size = 0;
    uint32        bytes_readed = 0;
    wchar    src_file_name[WATCHFACE_FILES_FULLPATH_LEN + 1] = {0};
    char*                p_buf = PNULL;
    uint16            path_len = 0;
    JSONPARSER_RESULT_E  json_result = JSON_PARSER_MAX;

    path_len = MMIAPICOM_Wstrlen(WATCHFACE_Get_UninstalledConfigFilePath());
    SCI_MEMCPY(src_file_name, WATCHFACE_Get_UninstalledConfigFilePath(), sizeof(wchar) * path_len);
    src_file_handle = SFS_CreateFile(src_file_name, (uint32)(SFS_MODE_READ|SFS_MODE_OPEN_EXISTING), NULL, NULL);//for read

    TRACE_WATCHFACE("[config]handle = %d",src_file_handle);
    if (0 == src_file_handle)
    {
        TRACE_WATCHFACE("[config]handle = 0");
        return 0;
    }
    SFS_GetFileSize(src_file_handle, &file_size);

    TRACE_WATCHFACE("[config]file_size = %d",file_size);

    p_buf = (char*)SCI_ALLOC_APPZ(file_size);

    if(PNULL == p_buf)
    {
        TRACE_WATCHFACE("[config]buf is NULL");
        return 0;
    }

    SFS_ReadFile(src_file_handle, p_buf, file_size,&bytes_readed, PNULL);
    SFS_CloseFile(src_file_handle);
    src_file_handle = 0;

    //read json
    json_result = JSONPARSER_GetConfigArrayCount(p_buf,&uwf_num);

    if(PNULL != p_buf)
    {
        SCI_FREE(p_buf);
        p_buf = PNULL;
    }

    if (JSON_PARSER_SUCCESS != json_result)
    {
        TRACE_WATCHFACE("[config]read json fail");
        return 0;
    }

    TRACE_WATCHFACE("[config]uninstalled watchface number is %d", uwf_num);
    return uwf_num;
}

/*****************************************************************************/
//  Description : WATCHFACE_Get_WatchfaceFile_Path
//  Parameter: [In] idx,表盘idx,
//             [Out]p_install_wf_path,安装表盘的文件路径
//             [Return] WATCHFACE_STATUS_RES_E
//  Author:yifan.yang
// Note:Get watchface path
/*****************************************************************************/
PUBLIC WATCHFACE_STATUS_RES_E WATCHFACE_Get_WatchfaceFile_Path(uint16 idx,char* p_install_wf_path)
{
    uint16            totalnum = 0;
    char*         p_filefolder = PNULL;
    char*         p_layoutfile = PNULL;
    COMMON_CONFIG_OBJECT_T* p_wf_obj_info = PNULL;
    WATCHFACE_STATUS_RES_E  result = WATCHFACE_STATUS_RES_MAX;

    totalnum = WATCHFACE_Get_Watchface_Num();

    TRACE_WATCHFACE("[config]index is %d", idx);

    if (0 == totalnum)
    {
        TRACE_WATCHFACE("[config]error,totalnum is 0.");
        return WATCHFACE_STATUS_RES_PARAM_ERR;
    }
    if((totalnum - 1) < idx)
    {
        TRACE_WATCHFACE("[config]index is error");
        return WATCHFACE_STATUS_RES_PARAM_ERR;
    }

    if (PNULL == p_install_wf_path)
    {
        TRACE_WATCHFACE("[config]p_install_wf_path is null");
        return WATCHFACE_STATUS_RES_PARAM_ERR;
    }

    p_wf_obj_info = (COMMON_CONFIG_OBJECT_T*)SCI_ALLOC_APPZ(sizeof(COMMON_CONFIG_OBJECT_T));
    if (PNULL == p_wf_obj_info)
    {
        TRACE_WATCHFACE("[config]buf is NULL");
        return WATCHFACE_STATUS_RES_PARAM_ERR;
    }
    result = WATCHFACE_Get_WatchfaceCfg_Info(WATCHFACE_CONFIG_TYPE_INSTALLED, idx, p_wf_obj_info);
    if (WATCHFACE_STATUS_RES_SUCCESS != result)
    {
        TRACE_WATCHFACE("[config]get watchface config info failed");
        return WATCHFACE_STATUS_RES_FAIL;
    }

    p_filefolder = WATCHFACE_Get_InstalledFileFolder();
    p_layoutfile = WATHCFACE_Get_LayoutFileName();
    sprintf(p_install_wf_path, "%s\\%s\\%s", p_filefolder, p_wf_obj_info->id, p_layoutfile);

    if (PNULL != p_wf_obj_info)
    {
        SCI_FREE(p_wf_obj_info);
        p_wf_obj_info = PNULL;
    }
    TRACE_WATCHFACE("[config]watchface full path is %s", p_install_wf_path);

    return WATCHFACE_STATUS_RES_SUCCESS;
}

/*****************************************************************************/
//  Description : WATCHFACE_Get_CurrentWatchfaceFile_Path
//  Parameter: [In] NONE
//             [Out]p_cur_wf_path,当前表盘的文件路径
//             [Return] WATCHFACE_STATUS_RES_E
//  Author:yifan.yang
// Note:Get current watchface path
/*****************************************************************************/
PUBLIC WATCHFACE_STATUS_RES_E WATCHFACE_Get_CurrentWatchfaceFile_Path(char* p_cur_wf_path)
{
    uint16 idx = 0;
    WATCHFACE_STATUS_RES_E wf_config_result = WATCHFACE_STATUS_RES_MAX;

    if (PNULL == p_cur_wf_path)
    {
        TRACE_WATCHFACE("[config]the param is error");
        return WATCHFACE_STATUS_RES_PARAM_ERR;
    }

    wf_config_result = WATCHFACE_Get_CurWatchface_Index(&idx);
    if (WATCHFACE_STATUS_RES_SUCCESS != wf_config_result)
    {
        TRACE_WATCHFACE("[config]watchface config is error");
        return WATCHFACE_STATUS_RES_FAIL;
    }

    wf_config_result = WATCHFACE_Get_WatchfaceFile_Path(idx, p_cur_wf_path);
    if (WATCHFACE_STATUS_RES_SUCCESS != wf_config_result)
    {
        TRACE_WATCHFACE("[config]watchface config is error");
        return WATCHFACE_STATUS_RES_FAIL;
    }
    return WATCHFACE_STATUS_RES_SUCCESS;
}
/*****************************************************************************/
//  Description : IsExsitSameWatchfaceID
//  Parameter: [In]watchface ID
//             [Out]none
//             [Return] BOOLEAN:TRUE,exist same watchface id
//  Author:chunjuan.liang
// Note:IsExsitSameWatchfaceID
/*****************************************************************************/
LOCAL BOOLEAN IsExsitSameWatchfaceID(char *p_wf_id)
{
    BOOLEAN ret = FALSE;
    uint8 wf_num = 0;
    uint8 i = 0;
    char temp_wf_id[MAX_NAME_LEN] = {0};

    if(PNULL == p_wf_id)
    {
        TRACE_WATCHFACE("[config]p_wf_id is null");
        return ret;
    }

    wf_num = WATCHFACE_Get_UnWatchface_Num();
    TRACE_WATCHFACE("[config]p_wf_id is = %s,wf_num =%d",p_wf_id,wf_num);

    for(i= 0;i < wf_num;i++)
    {
        SCI_MEMSET(temp_wf_id,0,MAX_NAME_LEN);
        Get_Idx_ByWatchfaceId(WATCHFACE_CONFIG_TYPE_UNINSTALLED,temp_wf_id,i);
        if(0 == strcmp(temp_wf_id, p_wf_id))
        {
            ret = TRUE;
            break;
        }
    }
     TRACE_WATCHFACE("[config] ret = %d",ret);
    return ret;
}

/*****************************************************************************/
//  Description : Get_Idx_ByWatchfaceId
//  Parameter: [In]watchface ID
//             [In]folder type
//             [Out]watchface index
//             [Return] WATCHFACE_STATUS_RES_E
//  Author:donglin.su
// Note:get watchface index by watchface ID
/*****************************************************************************/
LOCAL WATCHFACE_STATUS_RES_E Get_Idx_ByWatchfaceId(WATCHFACE_CONFIG_TYPE_E type ,char* p_wf_id, uint16* p_index)
{
    WATCHFACE_STATUS_RES_E result = WATCHFACE_STATUS_RES_SUCCESS;
    uint16 wf_idx = 0;
    uint16 wf_num = 0;
    COMMON_CONFIG_OBJECT_T* p_wf_info = PNULL;
    uint16 wf_len = 0;

    if(PNULL == p_wf_id)
    {
        TRACE_WATCHFACE("[WATCHFACE_UNINSTALLWF]in p_wf_id is pnull");
        result = WATCHFACE_STATUS_RES_PARAM_ERR;
        return result;
    }
    if(PNULL == p_index)
    {
        TRACE_WATCHFACE("[WATCHFACE_UNINSTALLWF]the index is pnull");
        result = WATCHFACE_STATUS_RES_PARAM_ERR;
        return result;
    }

    if(WATCHFACE_CONFIG_TYPE_INSTALLED == type)
    {
        wf_num = WATCHFACE_Get_Watchface_Num();
    }
    else if(WATCHFACE_CONFIG_TYPE_UNINSTALLED == type)
    {
        wf_num = WATCHFACE_Get_UnWatchface_Num();
    }
    else
    {
        result = WATCHFACE_STATUS_RES_PARAM_ERR;
        return result;
    }
    p_wf_info = (COMMON_CONFIG_OBJECT_T*)SCI_ALLOC_APPZ(sizeof(COMMON_CONFIG_OBJECT_T));
    if(PNULL == p_wf_info)
    {
        TRACE_WATCHFACE("[WATCHFACE_UNINSTALLWF]the p_wf_info is pnull");
        result = WATCHFACE_STATUS_RES_PARAM_ERR;
        return result;
    }
    for(wf_idx = 0;wf_idx < wf_num;wf_idx++)
    {
        WATCHFACE_Get_WatchfaceCfg_Info(type,wf_idx,p_wf_info);
        TRACE_WATCHFACE("[WATCHFACE_UNINSTALLWF]the p_wf_id = %s p_wf_info->id = %s",p_wf_id,p_wf_info->id);
        wf_len = MAX(SCI_STRLEN(p_wf_info->id),SCI_STRLEN(p_wf_id));
        if(0 == SCI_MEMCMP( p_wf_id, p_wf_info->id,wf_len))
        {
            TRACE_WATCHFACE("[WATCHFACE_UNINSTALLWF]the p_wf_cfg_info->id = p_wf_info->id = %s",p_wf_id);
            break;
        }
    }
    if(PNULL != p_wf_info)
    {
        SCI_FREE(p_wf_info);
        p_wf_info = PNULL;
    }
    if(wf_idx == wf_num)
    {
        result = WATCHFACE_STATUS_RES_FAIL;
        TRACE_WATCHFACE("[WATCHFACE_UNINSTALLWF]the watchfaceid is not find");
        return result;
    }
    *p_index = wf_idx;
    TRACE_WATCHFACE("[WATCHFACE_UNINSTALLWF]the wf_idx is %d", wf_idx);
    return result;
}

/*****************************************************************************/
//  Description : WATCHFACE_DeleteWatchface
//  Parameter: [In] watchface index
//             [In] folder type
//             [Out]NONE
//             [Return] WATCHFACE_STATUS_RES_E
//  Author:yifan.yang donglin.su
// Note:delete watchface forlder
/*****************************************************************************/
PUBLIC WATCHFACE_STATUS_RES_E WATCHFACE_DeleteWatchface(WATCHFACE_CONFIG_TYPE_E type ,uint16 idx)
{
    WATCHFACE_STATUS_RES_E wf_config_result = WATCHFACE_STATUS_RES_MAX;
    COMMON_CONFIG_OBJECT_T* p_wf_cfg_info = PNULL;
    char watchface_name_full_path[WATCHFACE_FILES_FULLPATH_LEN + 1] = {0};
    wchar watchface_folder_path_name[WATCHFACE_FILES_FULLPATH_LEN + 1] = {0};
    WATCHFACE_STATUS_RES_E result = WATCHFACE_STATUS_RES_MAX;
    char* p_filefolder = PNULL;
    uint16 cur_idx = 0;

    if(WATCHFACE_CONFIG_TYPE_INSTALLED == type && WATCHFACE_STATUS_RES_FACTORY_RESET != s_watchface_status)
    {
        WATCHFACE_Get_CurWatchface_Index(&cur_idx);
        if(cur_idx == idx)
        {
            TRACE_WATCHFACE("[config]: cur_idx = %d",cur_idx);
            return WATCHFACE_STATUS_RES_PARAM_ERR;
        }
        if(idx < DEFAULT_WATCHFACE_NUM)
        {
            TRACE_WATCHFACE("[config]:default watchface can't delete, idx = %d",idx);
            return WATCHFACE_STATUS_RES_PARAM_ERR;
        }
    }

    TRACE_WATCHFACE("[WATCHFACE_UNINSTALLWF]: enter");
    p_wf_cfg_info = (COMMON_CONFIG_OBJECT_T*)SCI_ALLOC_APPZ(sizeof(COMMON_CONFIG_OBJECT_T));
    if(PNULL == p_wf_cfg_info)
    {
        TRACE_WATCHFACE("[WATCHFACE_UNINSTALLWF]the p_wf_cfg_info is pnull");
        return WATCHFACE_STATUS_RES_PARAM_ERR;
    }
    wf_config_result = WATCHFACE_Get_WatchfaceCfg_Info(type, idx, p_wf_cfg_info);
    TRACE_WATCHFACE("[WATCHFACE_UNINSTALLWF]: p_wf_cfg_info->id = %d",p_wf_cfg_info->id);
    s_del_index = idx;
    s_watchface_type = type;
    TRACE_WATCHFACE("[WATCHFACE_UNINSTALLWF]: idx = %d", idx);

    // 拼接删除所在的文件夹
    if(WATCHFACE_CONFIG_TYPE_INSTALLED == type)
    {
        p_filefolder = WATCHFACE_Get_InstalledFileFolder();
    }
    else if(WATCHFACE_CONFIG_TYPE_UNINSTALLED == type)
    {
        p_filefolder = WATCHFACE_Get_UnintalledFileFolder();
    }
    else
    {
        result = WATCHFACE_STATUS_RES_PARAM_ERR;
        return result;
    }
    sprintf(watchface_name_full_path, "%s\\%s", p_filefolder,p_wf_cfg_info->id);
    if(PNULL != p_wf_cfg_info)
    {
        SCI_FREE(p_wf_cfg_info);
        p_wf_cfg_info = PNULL;
    }
    TRACE_WATCHFACE("[WATCHFACE_UNINSTALLWF]: watchface_name_full_path = %s", watchface_name_full_path);

    // 将char类型的字符串数组转化成wchar类型的字符串
    MMI_STRNTOWSTR(watchface_folder_path_name,WATCHFACE_FILES_FULLPATH_LEN+1,
        (char *) watchface_name_full_path,SCI_STRLEN(watchface_name_full_path),SCI_STRLEN(watchface_name_full_path));

    // 判断文件是否存在
    if(!MMIAPIFMM_IsFolderExist (watchface_folder_path_name, MMIAPICOM_Wstrlen(watchface_folder_path_name)))
    {
        result = WATCHFACE_STATUS_RES_FAIL;
        TRACE_WATCHFACE("[WATCHFACE_UNINSTALLWF]:result = %d", result);
        return result;
    }

    // 继续删除操作
    s_p_watchface_delete_handle = MMIFILE_DeleteOpen(watchface_folder_path_name, MMIAPICOM_Wstrlen(watchface_folder_path_name));

    // 判断返回值
    TRACE_WATCHFACE("[WATCHFACE_UNINSTALLWF]:s_p_watchface_delete_handle = 0x%08x", (uint32)s_p_watchface_delete_handle);//bug id 1941703 CID 424348
    if(PNULL == s_p_watchface_delete_handle)
    {
        result = WATCHFACE_STATUS_RES_FAIL;
    }
    else
    {
        result = WATCHFACE_STATUS_RES_SUCCESS;
    }
    return result;
}

/*****************************************************************************/
//  Description : WATCHFACE_DeleteWatchface Callback
//  Parameter: [In]value
//             [Out]NONE
//             [Return] WATCHFACE_STATUS_RES_E
//  Author:yifan.yang donglin.su
// Note:delete watchface forlder callback
/*****************************************************************************/
PUBLIC WATCHFACE_STATUS_RES_E WATCHFACE_DeleteCallback(uint16 value)
{
    WATCHFACE_STATUS_RES_E result = WATCHFACE_STATUS_RES_MAX;
    uint16 idx = 0;
    uint16 totalnum = 0;
    uint16 cur_idx = 0;
    WATCHFACE_CONFIG_TYPE_E type = WATCHFACE_CONFIG_TYPE_MAX;
    COMMON_CONFIG_OBJECT_T * p_wf_cfg_info = PNULL;
    WATCHFACE_STATUS_RES_E watchface_status = WATCHFACE_STATUS_RES_MAX;
    WATCHFACE_STATUS_RES_E watchface_state = WATCHFACE_STATUS_RES_MAX;

    TRACE_WATCHFACE("[WATCHFACE_UNINSTALLWF]:s_p_watchface_delete_handle = 0x%08x", (uint32)s_p_watchface_delete_handle);//bug id 1941703 CID 425649

    if (PNULL == s_p_watchface_delete_handle)
    {
        TRACE_WATCHFACE("[WATCHFACE_UNINSTALLWF]the s_p_watchface_delete_handle is pnull");
        return WATCHFACE_STATUS_RES_PARAM_ERR;
    }
    MMIFILE_DeleteClose (s_p_watchface_delete_handle);
    s_p_watchface_delete_handle = PNULL;
    if( SFS_NO_ERROR != value )
    {
        TRACE_WATCHFACE("[WATCHFACE_UNINSTALLWF]:operation failed! value = %d",value);
        return WATCHFACE_STATUS_RES_PARAM_ERR;
    }

    watchface_status = s_watchface_status;

    p_wf_cfg_info = (COMMON_CONFIG_OBJECT_T*)SCI_ALLOC_APPZ(sizeof(COMMON_CONFIG_OBJECT_T));
    if(PNULL == p_wf_cfg_info)
    {
        TRACE_WATCHFACE("[WATCHFACE_UNINSTALLWF]the p_wf_cfg_info is pnull");
        return WATCHFACE_STATUS_RES_PARAM_ERR;
    }

    idx    = s_del_index;
    if(WATCHFACE_STATUS_RES_UPDATE == watchface_status)
    {
        WATCHFACE_Get_WatchfaceCfg_Info(WATCHFACE_CONFIG_TYPE_INSTALLED, idx, p_wf_cfg_info);
        WATCHFACE_CopyWatchface(p_wf_cfg_info->id);
        s_watchface_status = WATCHFACE_STATUS_RES_MAX;
        watchface_status = WATCHFACE_STATUS_RES_MAX;
    }

    type   = s_watchface_type;
    totalnum = WATCHFACE_Get_Watchface_Num();
    if(WATCHFACE_CONFIG_TYPE_INSTALLED == type)
    {
        if ((totalnum - 1) < idx)
        {
#ifdef WATCHFACE_MANAGE_SUPPORT
            result = WATCHFACE_Set_Flow_DeleteResulte( FALSE );
#endif
            if(PNULL != p_wf_cfg_info)
            {
                SCI_FREE(p_wf_cfg_info);
                p_wf_cfg_info = PNULL;
            }
            TRACE_WATCHFACE("[config]:error.index is %d", idx);
            return WATCHFACE_STATUS_RES_PARAM_ERR;
        }
        result = Del_WatchfaceCfg_ByIdx(type, idx);
        if(WATCHFACE_STATUS_RES_FACTORY_RESET == watchface_status)
        {
            totalnum = WATCHFACE_Get_Watchface_Num();
            s_del_index        = WATCHFACE_MAX_NUM;
            s_watchface_type   = WATCHFACE_CONFIG_TYPE_MAX;
            if(totalnum == DEFAULT_WATCHFACE_NUM)
            {
                WATCHFACE_Set_CurWatchface_Index(0);
                TRACE_WATCHFACE("[config]: factory reset success.");
                s_watchface_status = WATCHFACE_STATUS_RES_MAX;
                if(PNULL != p_wf_cfg_info)
                {
                    SCI_FREE(p_wf_cfg_info);
                    p_wf_cfg_info = PNULL;
                }
                MMK_SendMsg(MMISET_FACTORY_RESET_WIN_ID,MSG_WATCHFACE_FACTORY_RESET,PNULL);
                return WATCHFACE_STATUS_RES_SUCCESS;
            }
            else
            {
                WATCHFACE_DeleteWatchface(WATCHFACE_CONFIG_TYPE_INSTALLED, DEFAULT_WATCHFACE_NUM);
                TRACE_WATCHFACE("[config]: factory reset delete continue.");
                if(PNULL != p_wf_cfg_info)
                {
                    SCI_FREE(p_wf_cfg_info);
                    p_wf_cfg_info = PNULL;
                }
                return WATCHFACE_STATUS_RES_SUCCESS;
            }
        }
        else
        {
            WATCHFACE_Get_CurWatchface_Index(&cur_idx);
            if(idx < cur_idx)
            {
                cur_idx--;
                WATCHFACE_Set_CurWatchface_Index(cur_idx);
            }
        }
    }
    else if(WATCHFACE_CONFIG_TYPE_UNINSTALLED == type)
    {
        totalnum = WATCHFACE_Get_UnWatchface_Num();
        if ((totalnum - 1) < idx)
        {
#ifdef WATCHFACE_MANAGE_SUPPORT
            result = WATCHFACE_Set_Flow_DeleteResulte( FALSE );
#endif
            if(PNULL != p_wf_cfg_info)
            {
                SCI_FREE(p_wf_cfg_info);
                p_wf_cfg_info = PNULL;
            }
            TRACE_WATCHFACE("[WATCHFACE_UNINSTALLWF]:error.index is %d", idx);
            return WATCHFACE_STATUS_RES_PARAM_ERR;
        }
        result = Del_WatchfaceCfg_ByIdx(type , idx);
    }
    else
    {
#ifdef WATCHFACE_MANAGE_SUPPORT
        result = WATCHFACE_Set_Flow_DeleteResulte( FALSE );
#endif
        if(PNULL != p_wf_cfg_info)
        {
            SCI_FREE(p_wf_cfg_info);
            p_wf_cfg_info = PNULL;
        }
        result = WATCHFACE_STATUS_RES_PARAM_ERR;
        return result;
    }
    TRACE_WATCHFACE("[WATCHFACE_UNINSTALLWF]:index is %d", idx);
    TRACE_WATCHFACE("[WATCHFACE_UNINSTALLWF]:totalnum is %d", totalnum);

    watchface_state = s_watchface_folder_status;
    if((WATCHFACE_STATUS_RES_DOWNLOAD == watchface_state)&&(WATCHFACE_STATUS_RES_UPDATE != watchface_status))
    {
        watchface_state = WATCHFACE_Get_WatchfaceCfg_Info(WATCHFACE_CONFIG_TYPE_UNINSTALLED, 0, p_wf_cfg_info);
        TRACE_WATCHFACE("[WATCHFACE_UNINSTALLWF]:watchface_state = %d",watchface_state);
        if((0 == SCI_STRLEN(p_wf_cfg_info->url)) || (WATCHFACE_STATUS_RES_SUCCESS != watchface_state))
        {
            if(PNULL != p_wf_cfg_info)
            {
                SCI_FREE(p_wf_cfg_info);
                p_wf_cfg_info = PNULL;
            }
            s_watchface_folder_status = WATCHFACE_STATUS_RES_MAX;
            return WATCHFACE_STATUS_RES_SUCCESS;
        }
        else
        {
            TRACE_WATCHFACE("[WATCHFACE_UNINSTALLWF]:Start_Download");
            Start_Download(p_wf_cfg_info);
        }
    }
    if(PNULL != p_wf_cfg_info)
    {
        SCI_FREE(p_wf_cfg_info);
        p_wf_cfg_info = PNULL;
    }

    if (WATCHFACE_STATUS_RES_SUCCESS != result)
    {
#ifdef WATCHFACE_MANAGE_SUPPORT
        result = WATCHFACE_Set_Flow_DeleteResulte( FALSE );
#endif

        TRACE_WATCHFACE("[WATCHFACE_UNINSTALLWF]:result = %d", result);
        return WATCHFACE_STATUS_RES_FAIL;
    }

    s_del_index        = WATCHFACE_MAX_NUM;
    s_watchface_type   = WATCHFACE_CONFIG_TYPE_MAX;
    TRACE_WATCHFACE("[WATCHFACE_UNINSTALLWF]:result = %d", result);
#ifdef WATCHFACE_MANAGE_SUPPORT
    // update UI
    result = WATCHFACE_Set_Flow_DeleteResulte( TRUE );
#endif
    // update edit win
    WATCHFACE_Update_PreviewImg();
    if(WATCHFACE_STATUS_RES_SUCCESS == result)
    {
#ifndef WIN32
#ifdef OSA_SUPPORT
        ur_con_printf("watchface operation success!");
#endif
#endif

    }
    return result;
}

/*****************************************************************************/
//  Description : WATCHFACE_CopyWatchface
//  Parameter: [In] watchface config id
//             [Out]NONE
//             [Return] WATCHFACE_STATUS_RES_E
//  Author:donglin.su
// Note:copy watchface forlder
/*****************************************************************************/
PUBLIC WATCHFACE_STATUS_RES_E WATCHFACE_CopyWatchface(char* p_wf_id)
{
    WATCHFACE_STATUS_RES_E result = WATCHFACE_STATUS_RES_MAX;
    char  watchface_unstall_full_path[WATCHFACE_FILES_FULLPATH_LEN + 1] = {0};
    wchar watchface_unstall_path_name[WATCHFACE_FILES_FULLPATH_LEN + 1] = {0};
    char  watchface_install_full_path[WATCHFACE_FILES_FULLPATH_LEN + 1] = {0};
    wchar watchface_install_path_name[WATCHFACE_FILES_FULLPATH_LEN + 1] = {0};
    char  watchface_full_path[WATCHFACE_FILES_FULLPATH_LEN + 1] = {0};
    wchar watchface_path_name[WATCHFACE_FILES_FULLPATH_LEN + 1] = {0};
    WATCHFACE_STATUS_RES_E wf_config_result = WATCHFACE_STATUS_RES_MAX;
    uint16 wf_index = WATCHFACE_MAX_NUM;

    if(PNULL == p_wf_id)
    {
        TRACE_WATCHFACE("[WATCHFACE_UNINSTALLWF]the p_wf_id is pnull");
        result = WATCHFACE_STATUS_RES_PARAM_ERR;
        return result;
    }
    TRACE_WATCHFACE("[WATCHFACE_INSTALLWF]: enter");
    //unstall
    sprintf(watchface_unstall_full_path,"%s\\%s",UNINSTALLED_WATCHFACE_FILES_FOLDER,p_wf_id);
    MMI_STRNTOWSTR(watchface_unstall_path_name,WATCHFACE_FILES_FULLPATH_LEN,(char *)watchface_unstall_full_path,
        SCI_STRLEN(watchface_unstall_full_path),SCI_STRLEN(watchface_unstall_full_path));
    //install
    MMI_STRNTOWSTR(watchface_install_path_name,WATCHFACE_FILES_FULLPATH_LEN,(char *)INSTALLED_WATCHFACE_FILES_FOLDER,
        SCI_STRLEN(INSTALLED_WATCHFACE_FILES_FOLDER),SCI_STRLEN(INSTALLED_WATCHFACE_FILES_FOLDER));

    TRACE_WATCHFACE("[WATCHFACE_INSTALLWF] watchface_unstall_path_name = %ls",watchface_unstall_path_name);
    TRACE_WATCHFACE("[WATCHFACE_INSTALLWF] watchface_unstall_path_name LEN = %d",MMIAPICOM_Wstrlen(watchface_unstall_path_name));
    TRACE_WATCHFACE("[WATCHFACE_INSTALLWF] watchface_install_path_name = %ls !",watchface_install_path_name);
    TRACE_WATCHFACE("[WATCHFACE_INSTALLWF] watchface_install_path_name LEN = %d",MMIAPICOM_Wstrlen(watchface_install_path_name));

    if(!MMIAPIFMM_IsFolderExist (watchface_unstall_path_name, MMIAPICOM_Wstrlen(watchface_unstall_path_name)))
    {
        result = WATCHFACE_STATUS_RES_FAIL;
        TRACE_WATCHFACE("[WATCHFACE_INSTALLWF]:unstall folder is not exist");
        return result;
    }
    if(!MMIAPIFMM_IsFolderExist (watchface_install_path_name, MMIAPICOM_Wstrlen(watchface_install_path_name)))
    {
        result = WATCHFACE_STATUS_RES_FAIL;
        TRACE_WATCHFACE("[WATCHFACE_INSTALLWF]:install folder is not exist");
        return result;
    }

    wf_config_result = Get_Idx_ByWatchfaceId(WATCHFACE_CONFIG_TYPE_UNINSTALLED,p_wf_id,&wf_index);
    TRACE_WATCHFACE("[WATCHFACE_INSTALLWF]:wf_index = %d", wf_index);
    if (WATCHFACE_STATUS_RES_SUCCESS != wf_config_result)
    {
        TRACE_WATCHFACE("[config]watchface config is error");
        return WATCHFACE_STATUS_RES_FAIL;
    }
    s_copy_index = wf_index;

    //被拷贝的表盘文件存在，删除表盘文件
    sprintf(watchface_full_path,"%s\\%s",INSTALLED_WATCHFACE_FILES_FOLDER,p_wf_id);
    MMI_STRNTOWSTR(watchface_path_name,WATCHFACE_FILES_FULLPATH_LEN,(char *)watchface_full_path,
        SCI_STRLEN(watchface_full_path),SCI_STRLEN(watchface_full_path));
    if(MMIAPIFMM_IsFolderExist (watchface_path_name, MMIAPICOM_Wstrlen(watchface_path_name)))
    {
        Get_Idx_ByWatchfaceId(WATCHFACE_CONFIG_TYPE_INSTALLED, p_wf_id, &wf_index);
        TRACE_WATCHFACE("[WATCHFACE_INSTALLWF]:wf_index = %d", wf_index);
        result = WATCHFACE_DeleteWatchface(WATCHFACE_CONFIG_TYPE_INSTALLED, wf_index);
        s_watchface_status = WATCHFACE_STATUS_RES_UPDATE;
        return result;
    }

    // 继续COPY操作
    s_p_watchface_copy_handle = MMIFILE_CopyOpen(watchface_unstall_path_name, MMIAPICOM_Wstrlen(watchface_unstall_path_name),
        watchface_install_path_name,MMIAPICOM_Wstrlen(watchface_install_path_name));

    // 判断返回值
    TRACE_WATCHFACE("[WATCHFACE_INSTALLWF]:s_p_app_copy_handle = 0x%08x", (uint32)s_p_watchface_copy_handle);//bug id 1941703 CID 428232
    if(PNULL == s_p_watchface_copy_handle)
    {
        result = WATCHFACE_STATUS_RES_FAIL;
    }
    else
    {
        result = WATCHFACE_STATUS_RES_SUCCESS;
    }

    TRACE_WATCHFACE("[WATCHFACE_INSTALLWF]: exit, result = %d",result);
    return result;
}

/*****************************************************************************/
//  Description : WATCHFACE_CopyWatchface callback
//  Parameter: [In]value
//             [Out]NONE
//             [Return] WATCHFACE_STATUS_RES_E
//  Author:donglin.su
// Note:copy watchface forlder callback
/*****************************************************************************/
PUBLIC WATCHFACE_STATUS_RES_E WATCHFACE_CopyCallback(uint16 value)
{
    COMMON_CONFIG_OBJECT_T* p_wf_info = PNULL;
    uint16 index = 0;
    uint8 wf_url[WATCHFACE_URL_LEN_MAX + 1] = {0};

    TRACE_WATCHFACE("[WATCHFACE_INSTALLWF]:enter, s_p_app_copy_handle = 0x%08x", (uint32)s_p_watchface_copy_handle);//bug id 1941703  CID 421217

    WATCHFACE_Set_InstallStatus(WATCHFACE_INSTALL_STATUS_IDLE);

    if (PNULL == s_p_watchface_copy_handle)
    {
        TRACE_WATCHFACE("[WATCHFACE_INSTALLWF]the s_p_watchface_copy_handle is pnull");
        return WATCHFACE_STATUS_RES_PARAM_ERR;
    }
    APPFILE_CopyClose (s_p_watchface_copy_handle);
    TRACE_WATCHFACE("[WATCHFACE_INSTALLWF]:enter, s_p_app_copy_handle = 0x%08x", (uint32)s_p_watchface_copy_handle);//buh id 1941703 CID 421217
    s_p_watchface_copy_handle = PNULL;

    if( SFS_NO_ERROR != value )
    {
        TRACE_WATCHFACE("[WATCHFACE_INSTALLWF]:operation failed! value = %d",value);
        return WATCHFACE_STATUS_RES_PARAM_ERR;
    }

    p_wf_info = (COMMON_CONFIG_OBJECT_T*)SCI_ALLOC_APPZ(sizeof(COMMON_CONFIG_OBJECT_T));
    if(PNULL == p_wf_info)
    {
        TRACE_WATCHFACE("[WATCHFACE_INSTALLWF]the p_wf_info is pnull");
        return WATCHFACE_STATUS_RES_PARAM_ERR;
    }
    index = s_copy_index;
    WATCHFACE_Get_WatchfaceCfg_Info(WATCHFACE_CONFIG_TYPE_UNINSTALLED,index,p_wf_info);
    TRACE_WATCHFACE("[WATCHFACE_INSTALLWF]the index = %d p_wf_info->id %s ", index,p_wf_info->id);
    WATCHFACE_DeleteWatchface(WATCHFACE_CONFIG_TYPE_UNINSTALLED, index);
    sprintf(wf_url,"%s\\%s",INSTALLED_WATCHFACE_FILES_FOLDER,p_wf_info->id);
    SCI_MEMCPY(p_wf_info->url,wf_url,WATCHFACE_URL_LEN_MAX);
    Add_WatchfaceCfg_Info(WATCHFACE_CONFIG_TYPE_INSTALLED,p_wf_info);
    if(PNULL != p_wf_info)
    {
        SCI_FREE(p_wf_info);
        p_wf_info = PNULL;
    }
    s_copy_index = WATCHFACE_MAX_NUM;

    // update edit win
    WATCHFACE_Update_PreviewImg();

    return WATCHFACE_STATUS_RES_SUCCESS;
}

/*****************************************************************************/
//  Description : Start_Download
//  Parameter: [In]p_wf_cfg_info 表盘的id和url
//             [Out]NONE
//             [Return] WATCHFACE_STATUS_RES_E
//  Author:donglin.su
// Note:control watchface URL download
/*****************************************************************************/
LOCAL WATCHFACE_STATUS_RES_E Start_Download(COMMON_CONFIG_OBJECT_T* p_wf_cfg_info)
{
    char* p_filefolder = PNULL;
    char* p_jsonfolder = PNULL;
    char watchface_folder_full_path[WATCHFACE_FILES_FULLPATH_LEN + 1] = {0};
    wchar watchface_folder_path_name[WATCHFACE_FILES_FULLPATH_LEN + 1] = {0};
    WATCHFACE_STATUS_RES_E result = WATCHFACE_STATUS_RES_MAX;

    if(PNULL == p_wf_cfg_info)
    {
        TRACE_WATCHFACE("[WATCHFACE_UNINSTALLWF]the p_wf_cfg_info is pnull");
        return WATCHFACE_STATUS_RES_PARAM_ERR;
    }
    if((0 == SCI_STRLEN(p_wf_cfg_info->url)) || (0 == SCI_STRLEN(p_wf_cfg_info->id))
        || (WATCHFACE_URL_LEN_MAX <= SCI_STRLEN(p_wf_cfg_info->url))
        || (WATCHFACE_WATCHFACE_ID_LEN_MAX <= SCI_STRLEN(p_wf_cfg_info->id)))
    {
        TRACE_WATCHFACE("watchface data is wrong");
        return WATCHFACE_STATUS_RES_PARAM_ERR;
    }

    //get unintalled folder load
    p_filefolder = WATCHFACE_Get_UnintalledFileFolder();
    SCI_MEMCPY(watchface_folder_full_path, p_filefolder, SCI_STRLEN(p_filefolder));
    MMI_STRNTOWSTR(watchface_folder_path_name,WATCHFACE_FILES_FULLPATH_LEN+1,
        (char *) watchface_folder_full_path,SCI_STRLEN(watchface_folder_full_path),SCI_STRLEN(watchface_folder_full_path));

    TRACE_WATCHFACE("[WATCHFACE_INSTALLWF] watchface_folder_path_name = %ls",watchface_folder_path_name);
    TRACE_WATCHFACE("[WATCHFACE_INSTALLWF] watchface_folder_path_name LEN = %d",MMIAPICOM_Wstrlen(watchface_folder_path_name));

    //判断文件存在，不存在就创建
    if(FALSE == MMIAPIFMM_IsFolderExist (watchface_folder_path_name, MMIAPICOM_Wstrlen(watchface_folder_path_name)))
    {
        if(SFS_ERROR_NONE != SFS_CreateDirectory((const uint16 *)watchface_folder_path_name))
        {
            TRACE_WATCHFACE("[WATCHFACE_INSTALLWF]:creat apps folder failed");
            return WATCHFACE_STATUS_RES_FAIL;
        }
        TRACE_WATCHFACE("[WATCHFACE_INSTALLWF]:create installed json file");
    }
    //URL是路径进行拷贝
    if(0 == SCI_MEMCMP(p_filefolder, p_wf_cfg_info->url, SCI_STRLEN(p_filefolder)))
    {
        WATCHFACE_CopyWatchface(p_wf_cfg_info->id);
        return WATCHFACE_STATUS_RES_SUCCESS;
    }
    else //URL不是路径进行下载
    {
#ifdef DOWNLOAD_WATCHFACE_SUPPORT  //for 217 ram cutdown: unsupport ble,so move download watchface from mobile phone
        if(TRUE == WATCHFACE_HttpStart(p_wf_cfg_info->url,p_wf_cfg_info->id))
        {
            WATCHFACE_Set_InstallStatus(WATCHFACE_INSTALL_STATUS_DOWNLOADING);
        }
        else
        {
            TRACE_WATCHFACE("[WATCHFACE_INSTALLWF]:http start fail");
            return WATCHFACE_STATUS_RES_FAIL;
        }
#endif
    }
    return WATCHFACE_STATUS_RES_SUCCESS;
}

/*****************************************************************************/
//  Description : WATCHFACE_SelfCheckURL
//  Parameter: [In]  none
//             [Out] none
//             [Return] Success:TRUE;Fail:FALSE
//  Author:donglin.su
//  Note:在开机和网络重连的情况下检查未下载的URL，并进行下载安装
/*****************************************************************************/
PUBLIC BOOLEAN WATCHFACE_SelfCheckURL(void)
{
    char* p_filefolder = PNULL;
    COMMON_CONFIG_OBJECT_T * p_wf_cfg_info = PNULL;
    wchar watchface_folder_path_name[WATCHFACE_FILES_FULLPATH_LEN + 1] = {0};

    //get uninstall watchface file folder
    p_filefolder = WATCHFACE_Get_UninstalledConfigFilePath();
    MMI_STRNTOWSTR(watchface_folder_path_name,WATCHFACE_FILES_FULLPATH_LEN+1,
        (char *) p_filefolder,SCI_STRLEN(p_filefolder),SCI_STRLEN(p_filefolder));
    if(FALSE == MMIAPIFMM_IsFolderExist (watchface_folder_path_name, MMIAPICOM_Wstrlen(watchface_folder_path_name)))
    {
        TRACE_WATCHFACE("[WATCHFACE_INSTALLWF]the folder is not exist");
        return TRUE;
    }
    else
    {
        if(0 == WATCHFACE_Get_UnWatchface_Num())
        {
            TRACE_WATCHFACE("[WATCHFACE_INSTALLWF]the NUM of URL is 0");
            return TRUE;
        }
        p_wf_cfg_info = (COMMON_CONFIG_OBJECT_T*)SCI_ALLOC_APPZ(sizeof(COMMON_CONFIG_OBJECT_T));
        if(PNULL == p_wf_cfg_info)
        {
            TRACE_WATCHFACE("[WATCHFACE_INSTALLWF]the p_wf_cfg_info is pnull");
            return FALSE;
        }
        WATCHFACE_Get_WatchfaceCfg_Info(WATCHFACE_CONFIG_TYPE_UNINSTALLED, 0, p_wf_cfg_info);
        TRACE_WATCHFACE("[WATCHFACE_INSTALLWF]the WATCHFACE_Get_WatchfaceCfg_Info is ok");
        if(0 == SCI_STRLEN(p_wf_cfg_info->url))
        {
            if(PNULL != p_wf_cfg_info)
            {
                SCI_FREE(p_wf_cfg_info);
                p_wf_cfg_info = PNULL;
            }
            return FALSE;
        }
        else
        {
            Start_Download(p_wf_cfg_info);
            s_watchface_folder_status = WATCHFACE_STATUS_RES_DOWNLOAD;
        }
    }
    if(PNULL != p_wf_cfg_info)
    {
        SCI_FREE(p_wf_cfg_info);
        p_wf_cfg_info = PNULL;
    }
    return TRUE;
}
/*****************************************************************************/
//  Description : WATCHFACE_Init
//  Parameter: [In]  none
//             [Out] none
//             [Return] Success:TRUE;Fail:FALSE
//  Author:donglin.su
//  Note:init watchface handle ble private message module
/*****************************************************************************/
PUBLIC BOOLEAN WATCHFACE_Init(void)
{
    BOOLEAN ret = TRUE;
#ifdef UAL_BT_BLE_PRIVATE_SUPPORT
    ual_bt_ble_result_e wf_private_ret = UAL_BT_BLE_RESULT_START;
    //register ble private msg

    wf_private_ret = ual_bt_ble_private_register(WATCHFACE_HandleBlePrivateMsg,&s_wf_handle);
    if(UAL_BT_BLE_PRIVATE_RESULT_SUCESS == wf_private_ret)
    {
        TRACE_WATCHFACE("watchface register ble private sucess!\n");
    }
    else
    {
        TRACE_WATCHFACE("watchface register ble private fail!\n");
    }
#endif
    WATCHFACE_ResFilesExist();
#ifdef DOWNLOAD_WATCHFACE_SUPPORT  //for 217 ram cutdown: unsupport ble,so move download watchface from mobile phone
    WATCHFACE_HTTP_InitModule();
#endif
    #ifdef THEME_UNZIP_SUPPORT
    g_watchgzip_app.ProcessMsg = HandleWatchfaceGzipMsg;
    #endif
    g_watchdebug_app.ProcessMsg = HandleWatchfaceDebugMsg;
    return ret;
}

#ifdef UAL_BT_BLE_PRIVATE_SUPPORT
/*****************************************************************************/
//  Description : WATCHFACE_HandleBlePrivateMsg
//  Parameter: [In]  ual_cms_msg_t:msg id and msg body
//             [Out] none
//             [Return] Success:TRUE;Fail:FALSE
//  Author:donglin.su
//  Note:handle ble private message
/*****************************************************************************/
PUBLIC  BOOLEAN WATCHFACE_HandleBlePrivateMsg(ual_cms_msg_t param)
{
    BOOLEAN ret = TRUE;
    char*  p_command[5] = {"WFPURL",//下载表盘
                           "WFDDAT",//删除表盘
                           "WFGCUR",//获得当前表盘信息
                           "WFGONE",//获得所有表盘信息
                           "WFGIME"};//获得手表IMEI号
    uint8  received_command[WATCHFACE_CMD_LEN_MAX+1] = {0};
    uint8  received_wf_id[WATCHFACE_WATCHFACE_ID_LEN_MAX+1]={0};
    char*  p_command_response[2] = {"00","01"};//失败和成功
    uint16 all_wf_num = 0;
    uint16 cur_wf_index = 0;
    WATCHFACE_STATUS_RES_E result = WATCHFACE_STATUS_RES_MAX;
    COMMON_CONFIG_OBJECT_T* p_wf_info = PNULL;
    LOCAL  uint8 s_wf_index = 0;
    LOCAL  uint8 received_msg_id[WATCHFACE_MSG_ID_LEN_MAX+1] = {0};
    LOCAL  ual_bt_ble_private_message_data_t* p_private_message = PNULL;
    p_wf_info = (COMMON_CONFIG_OBJECT_T*)SCI_ALLOC_APPZ(sizeof(COMMON_CONFIG_OBJECT_T));

    if(PNULL == p_wf_info)
    {
        TRACE_WATCHFACE("watchface p_wf_info is null");
        return FALSE;
    }
    TRACE_WATCHFACE("watchface msg id= %d ",param.msg_id);
    switch(param.msg_id)
    {
        case UAL_BT_BLE_PRIVATE_MSG_RECEIVE_DATA_CNF:
        {
            p_private_message = (ual_bt_ble_private_message_data_t*)param.p_body;
            if(PNULL == p_private_message)
            {
                TRACE_WATCHFACE("watchface private_message is null");
                ret = FALSE;
                break;//bug id 1941703 CID 423430
            }

            if( UAL_BT_BLE_PRIVATE_WATCHFACE == p_private_message->msg_type)
            {
                TRACE_WATCHFACE("watchface data is msg len = %d,\n",p_private_message->msg_len);
                TRACE_WATCHFACE("watchface data is %s,\n",p_private_message->msg_data);
                if(0 >= p_private_message->msg_len)
                {
                    TRACE_WATCHFACE("watchface data is false");
                    ret = FALSE;
                }
                else
                {
                    SCI_MEMCPY(received_msg_id,&(p_private_message->msg_data[0]),WATCHFACE_MSG_ID_LEN_MAX);
                    SCI_MEMCPY(received_command,&(p_private_message->msg_data[WATCHFACE_MSG_ID_LEN_MAX]),WATCHFACE_CMD_LEN_MAX);
                    TRACE_WATCHFACE("watchface msgid_get is %s",received_msg_id);
                    TRACE_WATCHFACE("watchface command_get is %s",received_command);
                    if( 0 == SCI_MEMCMP(p_command[0],received_command,WATCHFACE_CMD_LEN_MAX))
                    {
                        TRACE_WATCHFACE("watchface command is WFPURL");
                        SCI_MEMSET(p_wf_info->url,0,MAX_NAME_LEN+1);
                        SCI_MEMSET(p_wf_info->id,0,MAX_NAME_LEN+1);
                        SCI_MEMCPY(p_wf_info->url,
                            &(p_private_message->msg_data[WATCHFACE_MSG_ID_LEN_MAX + WATCHFACE_CMD_LEN_MAX]),
                            WATCHFACE_URL_LEN_MAX);
                        TRACE_WATCHFACE("watchface url is %s",p_wf_info->url);
                        SCI_MEMCPY(p_wf_info->id,
                            &(p_private_message->msg_data[WATCHFACE_MSG_ID_LEN_MAX+WATCHFACE_CMD_LEN_MAX + WATCHFACE_URL_LEN_MAX]),
                            WATCHFACE_WATCHFACE_ID_LEN_MAX);
                        TRACE_WATCHFACE("watchface id is %s",p_wf_info->id);
                        if((0 == SCI_STRLEN(p_wf_info->url)) || (0 == SCI_STRLEN(p_wf_info->id))
                            || (WATCHFACE_URL_LEN_MAX <= SCI_STRLEN(p_wf_info->url))
                            || (WATCHFACE_WATCHFACE_ID_LEN_MAX <= SCI_STRLEN(p_wf_info->id)))
                        {
                            SCI_MEMSET(p_private_message->msg_data,0x00,UAL_MAX_PRIVATE_DATA_LEN);
                            sprintf(p_private_message->msg_data,"%s%s",received_msg_id,p_command_response[0]);
                            TRACE_WATCHFACE("watchface data is %s,\n",p_private_message->msg_data);
                            ual_bt_ble_private_send_data(p_private_message,p_private_message->msg_len + WATCHFACE_SEND_MSG_HEAD_LEN);
                            break;
                        }
                        SCI_MEMSET(p_private_message->msg_data,0x00,UAL_MAX_PRIVATE_DATA_LEN);
                        sprintf(p_private_message->msg_data,"%s%s",received_msg_id,p_command_response[1]);
                        TRACE_WATCHFACE("watchface data is %s,\n",p_private_message->msg_data);
                        p_private_message->msg_len = SCI_STRLEN(p_private_message->msg_data);
                        ual_bt_ble_private_send_data(p_private_message,p_private_message->msg_len + WATCHFACE_SEND_MSG_HEAD_LEN);
                        if(WATCHFACE_INSTALL_STATUS_IDLE ==  WATCHFACE_Get_InstallStatus())
                        {
                            if( WATCHFACE_STATUS_RES_SUCCESS == WATCHFACE_Add_Unistallwflist_Info(p_wf_info))
                            {
                                result =  Start_Download(p_wf_info);
                                TRACE_WATCHFACE("download result = %d",result);
                            }
                            else
                            {
                                TRACE_WATCHFACE("add install information fail");
                            }
                        }else
                        {
                            //pop up installing
                            WATCHFACE_Install_Fail_Note_Win_Enter();
                        }
                    }
                    else if( 0 == SCI_MEMCMP(p_command[1],received_command,WATCHFACE_CMD_LEN_MAX))
                    {
                        uint16 wf_temp_idx = 0;
                        TRACE_WATCHFACE("watchface command is WFDDAT");
                        SCI_MEMCPY(p_wf_info->id,
                            &(p_private_message->msg_data[WATCHFACE_MSG_ID_LEN_MAX+WATCHFACE_CMD_LEN_MAX]),
                            WATCHFACE_WATCHFACE_ID_LEN_MAX);
                        TRACE_WATCHFACE("watchface id is %s",p_wf_info->id);
                        if(0 == SCI_STRLEN(p_wf_info->id) || WATCHFACE_WATCHFACE_ID_LEN_MAX <= SCI_STRLEN(p_wf_info->id))
                        {
                            SCI_MEMSET(p_private_message->msg_data,0x00,UAL_MAX_PRIVATE_DATA_LEN);
                            sprintf(p_private_message->msg_data,"%s%s",received_msg_id,p_command_response[0]);
                            TRACE_WATCHFACE("watchface data is %s,\n",p_private_message->msg_data);
                            ual_bt_ble_private_send_data(p_private_message,p_private_message->msg_len + WATCHFACE_SEND_MSG_HEAD_LEN);
                            break;
                        }
                        SCI_MEMSET(p_private_message->msg_data,0x00,UAL_MAX_PRIVATE_DATA_LEN);
                        sprintf(p_private_message->msg_data,"%s%s",received_msg_id,p_command_response[1]);
                        TRACE_WATCHFACE("watchface data is %s,\n",p_private_message->msg_data);
                        p_private_message->msg_len = SCI_STRLEN(p_private_message->msg_data);
                        ual_bt_ble_private_send_data(p_private_message,p_private_message->msg_len + WATCHFACE_SEND_MSG_HEAD_LEN);
                        result = Get_Idx_ByWatchfaceId(WATCHFACE_CONFIG_TYPE_INSTALLED,p_wf_info->id,&wf_temp_idx);
                        if(WATCHFACE_STATUS_RES_SUCCESS != result)
                        {
                            SCI_MEMSET(p_private_message->msg_data,0x00,UAL_MAX_PRIVATE_DATA_LEN);
                            sprintf(p_private_message->msg_data,"%s%s",received_msg_id,p_command_response[0]);
                            TRACE_WATCHFACE("watchface data is %s,\n",p_private_message->msg_data);
                            ual_bt_ble_private_send_data(p_private_message,p_private_message->msg_len + WATCHFACE_SEND_MSG_HEAD_LEN);
                            break;
                        }
                        TRACE_WATCHFACE("wf_temp_idx = %d",wf_temp_idx);
                        WATCHFACE_DeleteWatchface(WATCHFACE_CONFIG_TYPE_INSTALLED,wf_temp_idx);
                    }
                    else if( 0 == SCI_MEMCMP(p_command[2],received_command,WATCHFACE_CMD_LEN_MAX))
                    {
                        TRACE_WATCHFACE("watchface command is WFGCUR");
                        WATCHFACE_Get_CurWatchface_Index(&cur_wf_index);
                        SCI_MEMSET(p_wf_info,0,sizeof(COMMON_CONFIG_OBJECT_T));
                        WATCHFACE_Get_WatchfaceCfg_Info(WATCHFACE_CONFIG_TYPE_INSTALLED,cur_wf_index,p_wf_info);
                        SCI_MEMSET(p_private_message->msg_data,0x00,sizeof(p_private_message->msg_data));
                        sprintf(p_private_message->msg_data,"%s%s%s",received_msg_id,p_command_response[1],p_wf_info->id);
                        TRACE_WATCHFACE("watchface data is %s,\n",p_private_message->msg_data);
                        p_private_message->msg_len = SCI_STRLEN(p_private_message->msg_data);
                        TRACE_WATCHFACE("watchface private_message->msg_len=%d",p_private_message->msg_len);
                        ual_bt_ble_private_send_data(p_private_message,WATCHFACE_SEND_CUR_WATCHFACE_MSG_LEN + WATCHFACE_SEND_MSG_HEAD_LEN);
                        TRACE_WATCHFACE("watchface data is %s,\n",p_private_message->msg_data);
                    }
                    else if( 0 == SCI_MEMCMP(p_command[3],received_command,WATCHFACE_CMD_LEN_MAX))
                    {
                        TRACE_WATCHFACE("watchface command is WFGONE");
                        all_wf_num = WATCHFACE_Get_Watchface_Num();
                        TRACE_WATCHFACE("watchface all_wf_num is %d",all_wf_num);
                        s_wf_index = 0;
                        SCI_MEMSET(p_wf_info,0,sizeof(COMMON_CONFIG_OBJECT_T));
                        WATCHFACE_Get_WatchfaceCfg_Info(WATCHFACE_CONFIG_TYPE_INSTALLED,s_wf_index,p_wf_info);
                        TRACE_WATCHFACE("watchface s_wf_index = %d,watchface id is %s",s_wf_index,p_wf_info->id);
                        SCI_MEMSET(p_private_message->msg_data,0x00,UAL_MAX_PRIVATE_DATA_LEN);
                        sprintf(p_private_message->msg_data,"%s%s%2d%2d%s",received_msg_id,p_command_response[1],
                            s_wf_index,all_wf_num,p_wf_info->id);
                        TRACE_WATCHFACE("watchface data is %s,\n",p_private_message->msg_data);
                        p_private_message->msg_len = SCI_STRLEN(p_private_message->msg_data);
                        ual_bt_ble_private_send_data(p_private_message,WATCHFACE_SEND_WATCHFACE_ID_LEN + WATCHFACE_SEND_MSG_HEAD_LEN);
                        s_wf_index = 1;
                        TRACE_WATCHFACE("watchface data is %s,\n",p_private_message->msg_data);
                    }
                    else if( 0 == SCI_MEMCMP(p_command[4],received_command,WATCHFACE_CMD_LEN_MAX))
                    {
                        MN_IMEISVN_T imeisvn = {0};
                        uint8 imei_str[(MN_MAX_IMEI_LENGTH<<1) + 1] = {0};
                        TRACE_WATCHFACE("watchface command is WFGIME");
                        ret = MNNV_GetIMEISVNEx(MN_DUAL_SYS_1, &imeisvn);
                        if(FALSE == ret)
                        {
                            SCI_MEMSET(p_private_message->msg_data,0x00,UAL_MAX_PRIVATE_DATA_LEN);
                            sprintf(p_private_message->msg_data,"%s%s",received_msg_id,p_command_response[0]);
                            TRACE_WATCHFACE("watchface data is %s,\n",p_private_message->msg_data);
                            ual_bt_ble_private_send_data(p_private_message,p_private_message->msg_len + WATCHFACE_SEND_MSG_HEAD_LEN);
                            break;
                        }
                        SCI_MEMSET(p_private_message->msg_data,0x00,sizeof(p_private_message->msg_data));
                        MMIAPICOM_BcdToStr(PACKED_LSB_FIRST, (uint8*)imeisvn.imei, MN_MAX_IMEI_LENGTH<<1, (char*)imei_str);
                        TRACE_WATCHFACE("watchface imei_str is %s,\n", &imei_str[1]);
                        sprintf(p_private_message->msg_data, "%s%s%s", received_msg_id,p_command_response[1], &imei_str[1]);
                        TRACE_WATCHFACE("watchface data is %s,\n",p_private_message->msg_data);
                        p_private_message->msg_len = SCI_STRLEN(p_private_message->msg_data);
                        TRACE_WATCHFACE("watchface private_message->msg_len=%d",p_private_message->msg_len);
                        ual_bt_ble_private_send_data(p_private_message,WATCHFACE_SEND_CUR_WATCHFACE_MSG_LEN + WATCHFACE_SEND_MSG_HEAD_LEN);
                        TRACE_WATCHFACE("watchface data is %s,\n",p_private_message->msg_data);
                    }
                    else
                    {
                        TRACE_WATCHFACE("watchface command is ERROR");
                        SCI_MEMSET(p_private_message->msg_data,0x00,sizeof(p_private_message->msg_data));
                        sprintf(p_private_message->msg_data,"%s%s",received_msg_id,p_command_response[0]);
                        TRACE_WATCHFACE("watchface data is %s,\n",p_private_message->msg_data);
                        ual_bt_ble_private_send_data(p_private_message,p_private_message->msg_len + WATCHFACE_SEND_MSG_HEAD_LEN);
                    }
                }
            }
            else
            {
                TRACE_WATCHFACE("WATCHFACE should not process current private data!\n");
            }
            break;
        }

        case UAL_BT_BLE_PRIVATE_MSG_SEND_DATA_CNF:
        {
            TRACE_WATCHFACE("WATCHFACE UAL_BT_BLE_PRIVATE_MSG_SEND_DATA_CNF\n");
            if( s_wf_index > 0 )
            {
                all_wf_num = WATCHFACE_Get_Watchface_Num();
                if(PNULL == p_private_message)
                {
                    TRACE_WATCHFACE("watchface private_message is null");
                    ret = FALSE;
                    break;
                }
                TRACE_WATCHFACE("watchface command is WFGONE123");
                if( s_wf_index < all_wf_num )
                {
                    TRACE_WATCHFACE("watchface command is WFGONE123");
                    SCI_MEMSET(p_private_message->msg_data,0x00,UAL_MAX_PRIVATE_DATA_LEN);
                    SCI_MEMSET(p_wf_info,0,sizeof(COMMON_CONFIG_OBJECT_T));
                    WATCHFACE_Get_WatchfaceCfg_Info(WATCHFACE_CONFIG_TYPE_INSTALLED,s_wf_index,p_wf_info);
                    TRACE_WATCHFACE("watchface s_wf_index=%d,p_wf_info->id=%s",s_wf_index,p_wf_info->id);
                    sprintf(p_private_message->msg_data,"%s%s%2d%2d%s",received_msg_id,p_command_response[1],
                        s_wf_index,all_wf_num,p_wf_info->id);
                    TRACE_WATCHFACE("watchface data is %s,\n",p_private_message->msg_data);
                    p_private_message->msg_type = UAL_BT_BLE_PRIVATE_WATCHFACE;
                    p_private_message->msg_len = SCI_STRLEN(p_private_message->msg_data);
                    ual_bt_ble_private_send_data(p_private_message,WATCHFACE_SEND_WATCHFACE_ID_LEN + WATCHFACE_SEND_MSG_HEAD_LEN);
                    s_wf_index++;
                    TRACE_WATCHFACE("watchface data is %s,s_wf_index=%d\n",p_private_message->msg_data,s_wf_index);
                }
                else
                {
                    s_wf_index = 0;
                }
            }
            else
            {
                TRACE_WATCHFACE("watchface s_wfidx is %d", s_wf_index );
            }
            break;
        }

        default:
            TRACE_WATCHFACE("msg_id:%d should not process\n",param.msg_id);
            break;

    }
    if(PNULL != p_wf_info)
    {
        SCI_FREE(p_wf_info);
        p_wf_info = PNULL;
    }
    return ret;
}
#endif

/****************************************************************************
//  Description : get full path by index
//  Parameter:  [In]: idx            //索引值
//                      max_len        //获取的路径最大长度
//              [Out]: p_full_path   //全路径的指针
//                       full_path_len //全路径的长度
//              [Return]:  result    //函数返回值
//  Author: yifan.yang
//  Note:Get pre img full path by index
*****************************************************************************/
PUBLIC BOOLEAN WATCHFACE_Get_PreImgPath_ByIdx(uint16 idx, uint16 max_len, uint16* p_full_path, uint16* full_path_len)
{
    char    path_info[WATCHFACE_FILES_FULLPATH_LEN + 1] = {0};
    COMMON_CONFIG_OBJECT_T*    p_wf_info = PNULL;
    BOOLEAN                       result = FALSE;
    WATCHFACE_STATUS_RES_E config_result = WATCHFACE_STATUS_RES_MAX;
    uint16                      path_len = 0;

    TRACE_WATCHFACE("[EDIT]enter GetImgFullPathByIdx");

    if (PNULL == p_full_path)
    {
        TRACE_WATCHFACE("[EDIT]malloc for p_full_path fail!!");
        return result;
    }

    p_wf_info = (COMMON_CONFIG_OBJECT_T*)SCI_ALLOC_APPZ(sizeof(COMMON_CONFIG_OBJECT_T));
    if (PNULL == p_wf_info)
    {
        TRACE_WATCHFACE("[EDIT]malloc for p_wf_info fail!!");
        return result;
    }
    config_result = WATCHFACE_Get_WatchfaceCfg_Info(WATCHFACE_CONFIG_TYPE_INSTALLED, idx, p_wf_info);
    if (WATCHFACE_STATUS_RES_SUCCESS != config_result)
    {
        TRACE_WATCHFACE("[EDIT]get watchface config info failed.");
        if (PNULL != p_wf_info)
        {
            SCI_FREE(p_wf_info);
            p_wf_info = PNULL;
        }
        return result;
    }

    sprintf(path_info, "%s\\%s", p_wf_info->url, p_wf_info->icon);
    if (PNULL != p_wf_info)
    {
        SCI_FREE(p_wf_info);
        p_wf_info = PNULL;
    }

    path_len = strlen(path_info);
    if (max_len < path_len)
    {
        TRACE_WATCHFACE("[config]path_len > max_len");
        return result;
    }

    MMIAPICOM_StrToWstr(path_info, p_full_path);

    *full_path_len = MMIAPICOM_Wstrlen(p_full_path);
    TRACE_WATCHFACE("[config]Pre Img path_len is %d", *full_path_len);

    result = TRUE;

    return result;
}

/****************************************************************************
//  Description : get editable by index
//  Parameter:  [in]: idx            //索引值
//              [out]]: TRUE:editable;FALSE:diseditble
//              [Return]:
//  Author: chunjuan.liang
//  Note:whether editable
*****************************************************************************/
PUBLIC BOOLEAN WATCHFACE_GetEditable_ByIdx(uint16 idx, BOOLEAN *p_editable)
{
    COMMON_CONFIG_OBJECT_T*    p_wf_info = PNULL;
    BOOLEAN                       result = FALSE;
    WATCHFACE_STATUS_RES_E config_result = WATCHFACE_STATUS_RES_MAX;

    TRACE_WATCHFACE("[EDIT]enter");

    if (PNULL == p_editable)
    {
        TRACE_WATCHFACE("[EDIT]p_editable param error!!");
        return result;
    }

    p_wf_info = (COMMON_CONFIG_OBJECT_T*)SCI_ALLOC_APPZ(sizeof(COMMON_CONFIG_OBJECT_T));
    if (PNULL == p_wf_info)
    {
        TRACE_WATCHFACE("[EDIT]malloc for p_wf_info fail!!");
        return result;
    }
    config_result = WATCHFACE_Get_WatchfaceCfg_Info(WATCHFACE_CONFIG_TYPE_INSTALLED, idx, p_wf_info);
    if (WATCHFACE_STATUS_RES_SUCCESS != config_result)
    {
        TRACE_WATCHFACE("[EDIT]get watchface config info failed.");
        if (PNULL != p_wf_info)
        {
            SCI_FREE(p_wf_info);
            p_wf_info = PNULL;
        }
        return result;
    }

    *p_editable = p_wf_info->is_edit;

    TRACE_WATCHFACE("[EDIT]*p_editable = %d.",*p_editable);

    if (PNULL != p_wf_info)
    {
        SCI_FREE(p_wf_info);
        p_wf_info = PNULL;
    }

    result = TRUE;
    return result;
}

/*****************************************************************************/
//  Description : Update_WatchfaceDescription_Info
//  Parameter: [In] index
//             [In] p_wf_desc_info，表盘Description信息
//             [Out]NONE
//             [Return]WATCHFACE_STATUS_RES_E
//  Author:qi.zhang
/*****************************************************************************/
LOCAL WATCHFACE_STATUS_RES_E Update_WatchfaceDescription_Info(uint16 idx, COMMON_CONFIG_OBJECT_T* p_wf_desc_info)
{
    SFS_HANDLE src_file_handle = 0;
    SFS_HANDLE src_file_handle_write = 0;
    uint32     file_size = 0;
    uint32     bytes_readed = 0;
    uint32     bytes_writen = 0;
    wchar      src_file_name[WATCHFACE_FILES_FULLPATH_LEN + 1] = {0};
    char*      p_buf = PNULL;
    uint16     path_len = 0;
    JSONPARSER_RESULT_E  json_result = JSON_PARSER_MAX;

    if(PNULL == p_wf_desc_info)
    {
        TRACE_WATCHFACE("[config]p_wf_cfg_info is NULL");
        return WATCHFACE_STATUS_RES_PARAM_ERR;
    }

    path_len = MMIAPICOM_Wstrlen(WATCHFACE_Get_UninstalledConfigFilePath());
    SCI_MEMCPY(src_file_name, WATCHFACE_Get_UninstalledConfigFilePath(), sizeof(wchar) * path_len);
    src_file_handle = SFS_CreateFile(src_file_name, (uint32)(SFS_MODE_READ|SFS_MODE_OPEN_EXISTING), NULL, NULL);//for read

    TRACE_WATCHFACE("[config]handle = %d",src_file_handle);
    if (0 == src_file_handle)
    {
        TRACE_WATCHFACE("[config]handle = 0");
        return WATCHFACE_STATUS_RES_FILE_NOT_EXISTED;
    }
    SFS_GetFileSize(src_file_handle, &file_size);

    TRACE_WATCHFACE("[config]file_size = %d",file_size);

    p_buf = (char*)SCI_ALLOC_APPZ(file_size + ADD_BUFFER);

    if(PNULL == p_buf)
    {
        SFS_CloseFile(src_file_handle);
        TRACE_WATCHFACE("[config]buf is NULL");
        return WATCHFACE_STATUS_RES_PARAM_ERR;
    }

    SFS_ReadFile(src_file_handle, p_buf, file_size, &bytes_readed, PNULL);
    SFS_CloseFile(src_file_handle);
    src_file_handle = 0;

    //update json
    json_result = JSONPARSER_UpdateConfigObject(p_buf, idx, *p_wf_desc_info);
    if (JSON_PARSER_SUCCESS != json_result)
    {
        TRACE_WATCHFACE("[config]read json fail");
        if(PNULL != p_buf)
        {
            SCI_FREE(p_buf);
            p_buf = PNULL;
        }
        return WATCHFACE_STATUS_RES_FAIL;
    }

    SFS_DeleteFile(src_file_name,PNULL);

    src_file_handle_write = SFS_CreateFile (src_file_name, SFS_MODE_WRITE | SFS_MODE_CREATE_ALWAYS, NULL, NULL);
    TRACE_WATCHFACE("[config]handle = %d",src_file_handle_write);
    if (0 == src_file_handle_write)
    {
        TRACE_WATCHFACE("[config]handle = 0");
        if(PNULL != p_buf)
        {
            SCI_FREE(p_buf);
            p_buf = PNULL;
        }
        return WATCHFACE_STATUS_RES_FAIL;
    }

    SFS_WriteFile(src_file_handle_write,p_buf,strlen(p_buf),&bytes_writen,PNULL);
    SFS_CloseFile(src_file_handle_write);
    src_file_handle_write = 0;

    if(PNULL != p_buf)
    {
        SCI_FREE(p_buf);
        p_buf = PNULL;
    }
    return WATCHFACE_STATUS_RES_SUCCESS;
}
#ifdef THEME_UNZIP_SUPPORT
LOCAL MMI_RESULT_E HandleWatchfaceGzipMsg(PWND app_ptr, uint16 msg_id, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    THEME_UNZIP_SIG_T unzip_content = {0};
    char watchface_id[WATCHFACE_WATCHFACE_ID_LEN_MAX + 1]  = {0};
    char   file_name [WATCHFACE_WATCHFACE_ID_LEN_MAX + 1] = {0};

    if(PNULL == param)
    {
        TRACE_WATCHFACE("[HTTP]:param is PNULL");
        return MMI_RESULT_FALSE;
    }
    unzip_content = *((THEME_UNZIP_SIG_T*)param);
    MMIAPICOM_WstrToStr(unzip_content.sig_content.p_param->p_dir,watchface_id);
    WATCHFACE_GetUnzipFileName(file_name);
    TRACE_WATCHFACE("[HandleWatchfaceGzipMsg]:watchface_id is %s,unzip file name is %s",watchface_id, file_name);
    if(0 != SCI_MEMCMP(file_name, watchface_id, WATCHFACE_WATCHFACE_ID_LEN_MAX))
    {
        TRACE_WATCHFACE("[HandleWatchfaceGzipMsg]:not watchface unzip.");
        return MMI_RESULT_FALSE;
    }
    TRACE_WATCHFACE("[HandleWatchfaceGzipMsg]:msg_id = 0x%x",msg_id);
    switch (msg_id)
    {
        case THEME_UNZIP_SCODE_PROCESS:
        {
            WATCHFACE_STATUS_RES_E copy_res = WATCHFACE_STATUS_RES_MAX;
            if (ZIP_TOTAL_RATE == unzip_content.sig_content.process)
            {
                WATCHFACE_DeleteZip(watchface_id);
                WATCHFACE_Add_Description_Info(watchface_id);
                JSONPARSER_Adaptation(watchface_id);
                copy_res = WATCHFACE_CopyWatchface(watchface_id);
                theme_clean(&unzip_content.sig_content);
                if(WATCHFACE_STATUS_RES_SUCCESS == copy_res)
                {
                    WATCHFACE_Set_InstallStatus(WATCHFACE_INSTALL_STATUS_COPYING);
                }
                else
                {
                    WATCHFACE_Set_InstallStatus(WATCHFACE_INSTALL_STATUS_IDLE);
                }
            }
        }
        break;
        case THEME_UNZIP_SCODE_FAIL:
        {
            TRACE_WATCHFACE("[HTTP]:unzip fail");
            theme_clean(&unzip_content.sig_content);
            //set install status
            WATCHFACE_Set_InstallStatus(WATCHFACE_INSTALL_STATUS_IDLE);
        }
        break;
        default:
        {
            result = MMI_RESULT_FALSE;
        }
        break;
    }

    return result;
}
#endif

/*****************************************************************************/
//  Description : WATCHFACE_AddDescription_Info
//  Parameter: [In] p_wf_id:表盘ID
//             [Out]NONE
//             [Return]WATCHFACE_STATUS_RES_E
//  Author:qi.zhang
/*****************************************************************************/
PUBLIC WATCHFACE_STATUS_RES_E WATCHFACE_Add_Description_Info(char* p_wf_id)
{
    SFS_HANDLE src_file_handle = 0;
    uint32     file_size = 0;
    uint32     bytes_readed = 0;
    uint16     path_len = 0;
    char*      p_buf = PNULL;
    uint16     wf_index = WATCHFACE_MAX_NUM;
    char       description_json_path[WATCHFACE_FILES_FULLPATH_LEN + 1] = {0};
    wchar      src_file_name[WATCHFACE_FILES_FULLPATH_LEN + 1] = {0};
    COMMON_CONFIG_OBJECT_T* p_wf_desc_info = PNULL;
    JSONPARSER_RESULT_E     json_result = JSON_PARSER_MAX;
    WATCHFACE_STATUS_RES_E  wf_add_result = WATCHFACE_STATUS_RES_MAX;

    if(PNULL == p_wf_id)
    {
        TRACE_WATCHFACE("[WATCHFACE_AddDescription_Info]the p_wf_id is pnull");
        return WATCHFACE_STATUS_RES_PARAM_ERR;
    }
    TRACE_WATCHFACE("WATCHFACE_AddDescription_Info enter");
    sprintf(description_json_path,"%s\\%s\\%s",UNINSTALLED_WATCHFACE_FILES_FOLDER,p_wf_id,DESCRIPTION_JSON_NAME);
    MMI_STRNTOWSTR(src_file_name, WATCHFACE_FILES_FULLPATH_LEN,(char *)description_json_path,
                   SCI_STRLEN(description_json_path),SCI_STRLEN(description_json_path));
    path_len = MMIAPICOM_Wstrlen(src_file_name);
    TRACE_WATCHFACE("[WATCHFACE_AddDescription_Info] src_file_name = %s",description_json_path);
    TRACE_WATCHFACE("[WATCHFACE_AddDescription_Info] path_len LEN = %d",SCI_STRLEN(description_json_path));
    src_file_handle = SFS_CreateFile(src_file_name, (uint32)(SFS_MODE_READ|SFS_MODE_OPEN_EXISTING), NULL, NULL);//for read

    TRACE_WATCHFACE("[WATCHFACE_AddDescription_Info]handle = %d",src_file_handle);
    if (0 == src_file_handle)
    {
        TRACE_WATCHFACE("[WATCHFACE_AddDescription_Info]handle = 0");
        return WATCHFACE_STATUS_RES_FAIL;
    }
    SFS_GetFileSize(src_file_handle, &file_size);

    p_buf = (char*)SCI_ALLOC_APPZ(file_size);

    if(PNULL == p_buf)
    {
        SFS_CloseFile(src_file_handle);
        TRACE_WATCHFACE("[WATCHFACE_AddDescription_Info]buf is NULL");
        return WATCHFACE_STATUS_RES_PARAM_ERR;
    }

    SFS_ReadFile(src_file_handle, p_buf, file_size,&bytes_readed, PNULL);
    SFS_CloseFile(src_file_handle);
    src_file_handle = 0;
    //read json
    p_wf_desc_info = (COMMON_CONFIG_OBJECT_T*)SCI_ALLOC_APPZ(sizeof(COMMON_CONFIG_OBJECT_T));
    if(PNULL == p_wf_desc_info)
    {
        SCI_FREE(p_buf);
        TRACE_WATCHFACE("[WATCHFACE_AddDescription_Info]the p_wf_desc_info is pnull");
        return WATCHFACE_STATUS_RES_PARAM_ERR;
    }
    json_result = JSONPARSER_GetDescriptionInfo(p_buf,p_wf_desc_info);
    if(PNULL != p_buf)
    {
        SCI_FREE(p_buf);
        p_buf = PNULL;
    }

    if (JSON_PARSER_SUCCESS != json_result)
    {
        SCI_FREE(p_wf_desc_info);
        TRACE_WATCHFACE("[WATCHFACE_AddDescription_Info]read json fail");
        return WATCHFACE_STATUS_RES_FAIL;
    }

    wf_add_result = Get_Idx_ByWatchfaceId(WATCHFACE_CONFIG_TYPE_UNINSTALLED,p_wf_id,&wf_index);
    TRACE_WATCHFACE("[WATCHFACE_AddDescription_Info]:wf_index = %d", wf_index);
    if (WATCHFACE_STATUS_RES_SUCCESS != wf_add_result)
    {
        if (PNULL != p_wf_desc_info)
        {
            SCI_FREE(p_wf_desc_info);
            p_wf_desc_info = PNULL;
        }
        TRACE_WATCHFACE("[WATCHFACE_AddDescription_Info]watchface config is error");
        return WATCHFACE_STATUS_RES_FAIL;
    }

    Update_WatchfaceDescription_Info(wf_index,p_wf_desc_info);
    if(PNULL != p_wf_desc_info)
    {
        SCI_FREE(p_wf_desc_info);
        p_wf_desc_info = PNULL;
    }

    return WATCHFACE_STATUS_RES_SUCCESS;
}
/*****************************************************************************/
//  Description : Debug Mode安装指定表盘
//  Parameter: [In] p_name：待安装的表盘压缩包名称，包含扩展名
//             [Out] None
//             [Return] WATCHFACE_STATUS_RES_E
//  Author: qi.zhang
//  Note:
/*****************************************************************************/
LOCAL WATCHFACE_STATUS_RES_E SendMsgToAPPTask(uint32 msg_id, uint8 *p_name)
{
    WATCHFACE_DEBUG_SIG_T *p_signal = PNULL;
    uint32 signal_id = 0;

    if(PNULL == p_name )
    {
        TRACE_WATCHFACE("[watchface_config]: folder name is null");
        return WATCHFACE_STATUS_RES_PARAM_ERR;
    }
    if(0 == SCI_STRLEN(p_name) || SCI_STRLEN(p_name) > WATCHFACE_WATCHFACE_ID_LEN_MAX)
    {
        TRACE_WATCHFACE("[watchface_config]: folder name len is error,len = %d",SCI_STRLEN(p_name));
        return WATCHFACE_STATUS_RES_PARAM_ERR;
    }

    signal_id = msg_id;
    MmiCreateSignal(signal_id, sizeof(WATCHFACE_DEBUG_SIG_T), (MmiSignalS**)&p_signal);
    if(PNULL == p_signal)
    {
        TRACE_WATCHFACE("[watchface_config]: p_signal is null");
        return WATCHFACE_STATUS_RES_PARAM_ERR;
    }
    SCI_MEMSET(p_signal->wf_name, 0x00, WATCHFACE_WATCHFACE_ID_LEN_MAX+1);
    SCI_MEMCPY(p_signal->wf_name, p_name,SCI_STRLEN(p_name));
    p_signal->SignalSize = sizeof(WATCHFACE_DEBUG_SIG_T);
    TRACE_WATCHFACE("[watchface_config]:send watchface msg by Debug Mode to app task");
    //Send the signal to the app task
    MmiSendSignal(P_APP, (MmiSignalS*)p_signal);
    return WATCHFACE_STATUS_RES_SUCCESS;
}

/*****************************************************************************/
//  Description : WATCHFACE_InstallByDebugMode
//  Parameter: [In] p_full_name:zip full name(压缩文件全名称包括拓展名)
//             [Out]NONE
//             [Return] WATCHFACE_STATUS_RES_E
//  Author:qi.zhang
// Note:Install Watchface
/*****************************************************************************/
PUBLIC WATCHFACE_STATUS_RES_E WATCHFACE_InstallByDebugMode(char* p_full_name)
{
    if(PNULL == p_full_name)
    {
        TRACE_WATCHFACE("[WATCHFACE_InstallByDebugMode]:watchface p_full_name is error.");
#ifndef WIN32
#ifdef OSA_SUPPORT
        ur_con_printf("watchface install fail! watchface id is null");
#endif
#endif
        return WATCHFACE_STATUS_RES_PARAM_ERR;
    }
    if(0 == SCI_STRLEN(p_full_name) || SCI_STRLEN(p_full_name) > WATCHFACE_WATCHFACE_ID_LEN_MAX)
    {
        TRACE_WATCHFACE("[WATCHFACE_InstallByDebugMode]:zip full name length is error, len = %d.",SCI_STRLEN(p_full_name));
#ifndef WIN32
#ifdef OSA_SUPPORT
        ur_con_printf("watchface install fail! watchface id is error.");
#endif
#endif
        return WATCHFACE_STATUS_RES_PARAM_ERR;
    }
    TRACE_WATCHFACE("[WATCHFACE_InstallByDebugMode]:p_full_name is %s", p_full_name);
    return SendMsgToAPPTask(MSG_WATCHFACE_INSTALL_BY_DEBUG,p_full_name);
}
/*****************************************************************************/
//  Description : WATCHFACE_UninstallByDebugMode
//  Parameter: [In] p_wf_id:wf_id(表盘id对应的文件夹的名字)
//             [Out]NONE
//             [Return] NONE
//  Author:qi.zhang
// Note:Unistall watchface
/*****************************************************************************/
PUBLIC WATCHFACE_STATUS_RES_E WATCHFACE_UninstallByDebugMode(char* p_wf_id)
{
    uint16                 wf_len = 0;

    if(PNULL == p_wf_id)
    {
        TRACE_WATCHFACE("[WATCHFACE_UninstallByDebugMode]:watchface p_full_name is error.");
#ifndef WIN32
#ifdef OSA_SUPPORT
        ur_con_printf("watchface uninstall fail! watchface id is null");
#endif
#endif
        return WATCHFACE_STATUS_RES_PARAM_ERR;
    }
    wf_len = SCI_STRLEN(p_wf_id);
    if(0 == wf_len || wf_len > WATCHFACE_WATCHFACE_ID_LEN_MAX)
    {
        TRACE_WATCHFACE("[WATCHFACE_UninstallByDebugMode]:watchface id length error (length = %d)",wf_len);
#ifndef WIN32
#ifdef OSA_SUPPORT
        ur_con_printf("watchface uninstall fail! watchface id is error.");
#endif
#endif
        return WATCHFACE_STATUS_RES_PARAM_ERR;
    }
    TRACE_WATCHFACE("[WATCHFACE_UninstallByDebugMode]:wf_id is %s", p_wf_id);
    return SendMsgToAPPTask(MSG_WATCHFACE_UNINSTALL_BY_DEBUG,p_wf_id);
}


/*****************************************************************************/
//  Description : InstallWFByDebug
//  Parameter: [In] p_full_name:zip full name(压缩文件全名称包括拓展名)
//             [Out]NONE
//             [Return] WATCHFACE_STATUS_RES_E
//  Author:qi.zhang
// Note:Install Watchface
/*****************************************************************************/
LOCAL WATCHFACE_STATUS_RES_E InstallWFByDebug(char* p_full_name)
{
    wchar     file_full_name[WATCHFACE_FILES_FULLPATH_LEN + 1] = {0};
    wchar     suffix_name[WATCHFACE_SUFFIX_MAX_LEN + 1] = {0};
    uint16    file_len = 0;
    uint16    suffix_len = WATCHFACE_SUFFIX_MAX_LEN;
    wchar     file_name[WATCHFACE_FILES_FULLPATH_LEN + 1] = {0};
    char      watchface_zip_full_path[WATCHFACE_FILES_FULLPATH_LEN + 1] = {0};
    wchar     watchface_zip_path_name[WATCHFACE_FILES_FULLPATH_LEN + 1] = {0};
    char      wf_id[WATCHFACE_WATCHFACE_ID_LEN_MAX + 1] = {0};
    char*     p_file_dir = PNULL;
    wchar     base_dir[WATCHFACE_FILES_FULLPATH_LEN + 1] = {0};
    WATCHFACE_STATUS_RES_E  result = WATCHFACE_STATUS_RES_MAX;
    COMMON_CONFIG_OBJECT_T* p_wf_info = PNULL;

    if(PNULL == p_full_name)
    {
        TRACE_WATCHFACE("[InstallWFByDebug]:watchface p_full_name is error.");
        return WATCHFACE_STATUS_RES_PARAM_ERR;
    }
    if(0 == SCI_STRLEN(p_full_name))
    {
        TRACE_WATCHFACE("[InstallWFByDebug]:zip full name length is 0.");
        return WATCHFACE_STATUS_RES_PARAM_ERR;
    }
    TRACE_WATCHFACE("[InstallWFByDebug]:p_full_name is %s", p_full_name);
    //分解文件名
    MMI_STRNTOWSTR(file_full_name,WATCHFACE_FILES_FULLPATH_LEN, p_full_name, SCI_STRLEN(p_full_name), SCI_STRLEN(p_full_name));
    MMIAPIFMM_SplitFileName(file_full_name, (uint16)MMIAPICOM_Wstrlen(file_full_name),file_name, &file_len,suffix_name,&suffix_len);
    //判断表盘id长度
    if(file_len > WATCHFACE_WATCHFACE_ID_LEN_MAX)
    {
        TRACE_WATCHFACE("[InstallWFByDebug]:watchface id length > 50.");
        return WATCHFACE_STATUS_RES_PARAM_ERR;
    }
    //判断是否为zip压缩包
    if(0 != MMIAPICOM_Wstrcmp(suffix_name, ZIP_SUFFIX))
    {
        TRACE_WATCHFACE("[InstallWFByDebug]:suffix name is not .zip.");
        return WATCHFACE_STATUS_RES_PARAM_ERR;
    }
    //create uninstall list
    p_wf_info = (COMMON_CONFIG_OBJECT_T*)SCI_ALLOC_APPZ(sizeof(COMMON_CONFIG_OBJECT_T));

    if(PNULL == p_wf_info)
    {
        TRACE_WATCHFACE("watchface p_wf_info is null");
        return WATCHFACE_STATUS_RES_PARAM_ERR;
    }
    MMIAPICOM_WstrToStr(file_name, wf_id);
    SCI_MEMCPY(p_wf_info->id, wf_id,WATCHFACE_WATCHFACE_ID_LEN_MAX);
    result = WATCHFACE_Add_Unistallwflist_Info(p_wf_info);
    if(WATCHFACE_STATUS_RES_SUCCESS != result)
    {
        if(PNULL != p_wf_info)
        {
            SCI_FREE(p_wf_info);
            p_wf_info = PNULL;
        }
        TRACE_WATCHFACE("Add unistallwatchfacelist.json failed");
        return WATCHFACE_STATUS_RES_FAIL;
    }
    if(PNULL != p_wf_info)
    {
        SCI_FREE(p_wf_info);
        p_wf_info = PNULL;
    }
    //解压
    p_file_dir = WATCHFACE_Get_UnintalledFileFolder();
    MMIAPICOM_StrToWstr(p_file_dir, base_dir);
    sprintf(watchface_zip_full_path,"%s\\%s",p_file_dir,p_full_name);
    MMI_STRNTOWSTR(watchface_zip_path_name, WATCHFACE_FILES_FULLPATH_LEN,(char *)watchface_zip_full_path,
                   SCI_STRLEN(watchface_zip_full_path),SCI_STRLEN(watchface_zip_full_path));
    TRACE_WATCHFACE("[InstallWFByDebug]:inname is %s,base dir is %s",watchface_zip_full_path,p_file_dir);
    if(!MMIAPIFMM_IsFileExist(watchface_zip_path_name, MMIAPICOM_Wstrlen(watchface_zip_path_name)))
    {
        TRACE_WATCHFACE("[InstallWFByDebug]:zip is not exist");
        return WATCHFACE_STATUS_RES_PARAM_ERR;
    }
    WATCHFACE_SetUnzipFileName(wf_id);
#ifdef THEME_UNZIP_SUPPORT
    theme_unzip(watchface_zip_path_name,base_dir,file_name);
#endif
     return WATCHFACE_STATUS_RES_SUCCESS;
}
/*****************************************************************************/
//  Description : UninstallWFByDebug
//  Parameter: [In] p_wf_id:wf_id(表盘id对应的文件夹的名字)
//             [Out]NONE
//             [Return] NONE
//  Author:qi.zhang
// Note:Unistall watchface by
/*****************************************************************************/
LOCAL WATCHFACE_STATUS_RES_E UninstallWFByDebug(char* p_wf_id)
{
    WATCHFACE_STATUS_RES_E result = WATCHFACE_STATUS_RES_MAX;
    uint16                 wf_index = WATCHFACE_MAX_NUM;
    uint16                 wf_len = 0;

    if(PNULL == p_wf_id)
    {
        TRACE_WATCHFACE("[UninstallWFByDebug]:watchface p_full_name is error.");
        return WATCHFACE_STATUS_RES_PARAM_ERR;
    }
    wf_len = SCI_STRLEN(p_wf_id);
    if(0 == wf_len || wf_len > WATCHFACE_WATCHFACE_ID_LEN_MAX)
    {
        TRACE_WATCHFACE("[UninstallWFByDebug]:watchface id length error (length = %d)",wf_len);
        return WATCHFACE_STATUS_RES_PARAM_ERR;
    }
    TRACE_WATCHFACE("[UninstallWFByDebug]:wf_id is %s", p_wf_id);
    Get_Idx_ByWatchfaceId(WATCHFACE_CONFIG_TYPE_INSTALLED,p_wf_id,&wf_index);
    result = WATCHFACE_DeleteWatchface(WATCHFACE_CONFIG_TYPE_INSTALLED, wf_index);
    if(result != WATCHFACE_STATUS_RES_SUCCESS)
    {
        TRACE_WATCHFACE("[UninstallWFByDebug]:Uninstal failed");
    }
    return WATCHFACE_STATUS_RES_SUCCESS;
}
/*****************************************************************************/
//  Description : HandleWatchfaceDebugMsg
//  Parameter: [Out]NONE
//             [Return] MMI_RESULT_E
//  Author:qi.zhang
// Note:处理Debug Mode安装卸载表盘的消息
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleWatchfaceDebugMsg(PWND app_ptr, uint16 msg_id, DPARAM param)
{
    WATCHFACE_DEBUG_SIG_T msg_body = {0};
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    WATCHFACE_STATUS_RES_E watchface_res = WATCHFACE_STATUS_RES_MAX;

    if(PNULL == param)
    {
        TRACE_WATCHFACE("[watchface_config]:param is PNULL");
        return MMI_RESULT_FALSE;
    }
    msg_body = *((WATCHFACE_DEBUG_SIG_T*)param);
    TRACE_WATCHFACE("[watchface_config]:msg_id = 0x%x, msg_body:%s",msg_id,msg_body.wf_name);
    switch (msg_id)
    {
        case MSG_WATCHFACE_INSTALL_BY_DEBUG:
        {
            watchface_res = InstallWFByDebug(msg_body.wf_name);
            if(WATCHFACE_STATUS_RES_SUCCESS != watchface_res)
            {
                TRACE_WATCHFACE("[watchface_config]:watchface_res install error.");
#ifndef WIN32
#ifdef OSA_SUPPORT
                ur_con_printf("watchface install fail!");
#endif
#endif
            }
            break;
        }
        case MSG_WATCHFACE_UNINSTALL_BY_DEBUG:
        {
            watchface_res = UninstallWFByDebug(msg_body.wf_name);
            if(WATCHFACE_STATUS_RES_SUCCESS != watchface_res)
            {
                TRACE_WATCHFACE("[watchface_config]:watchface_res uninstall error.");
#ifndef WIN32
#ifdef OSA_SUPPORT
                ur_con_printf("watchface uninstall fail!");
#endif
#endif
            }

            break;
        }
        default:
        {
            TRACE_WATCHFACE("[config]:message error");
            return MMI_RESULT_FALSE;
        }
    }
    return result;
}

/*****************************************************************************/
//  Description : WATCHFACE_Deletezip
//  Parameter: [In] p_wf_id: wf_id
//             [Out]NONE
//             [Return] BOOLEAN
//  Author:qi.zhang
// Note:delete zip
/*****************************************************************************/
PUBLIC BOOLEAN WATCHFACE_DeleteZip(char* p_wf_id)
{
    char  watchface_zip_full_path[WATCHFACE_FILES_FULLPATH_LEN + 1] = {0};
    wchar watchface_zip_path_name[WATCHFACE_FILES_FULLPATH_LEN + 1] = {0};

    if(PNULL == p_wf_id)
    {
        TRACE_WATCHFACE("[WATCHFACE_Deletezip]:p_wf_id is pnull.");
        return FALSE;
    }
    sprintf(watchface_zip_full_path,"%s\\%s.zip",UNINSTALLED_WATCHFACE_FILES_FOLDER,p_wf_id);
    MMI_STRNTOWSTR(watchface_zip_path_name, WATCHFACE_FILES_FULLPATH_LEN,(char *)watchface_zip_full_path,
                   SCI_STRLEN(watchface_zip_full_path),SCI_STRLEN(watchface_zip_full_path));
    TRACE_WATCHFACE("[WATCHFACE_Deletezip]:zip full path %s,",watchface_zip_full_path);
    if(MMIAPIFMM_IsFileExist(watchface_zip_path_name, MMIAPICOM_Wstrlen(watchface_zip_path_name)))
    {
        MMIAPIFMM_DeleteFile(watchface_zip_path_name,PNULL);
        return TRUE;
    }
    else
    {
        TRACE_WATCHFACE("[WATCHFACE_Deletezip]:zip is not exist");
        return FALSE;
    }
}
/*****************************************************************************/
//  Description : WATCHFACE_GetUnzipFileName
//  Parameter: [Out] p_file_name: 解压缩的文件名字
//             [Return] NONE
//  Author:qi.zhang
// Note:WATCHFACE_GetUnzipFileName
/*****************************************************************************/
PUBLIC void WATCHFACE_GetUnzipFileName(char* p_file_name)
{
    if(PNULL == p_file_name)
    {
        TRACE_WATCHFACE("[WATCHFACE_GetUnzipFileName]:p_wf_id is pnull.");
        return;
    }
    SCI_MEMSET(p_file_name, 0, SCI_STRLEN(p_file_name));
    SCI_MEMCPY(p_file_name, s_unzip_filename, WATCHFACE_WATCHFACE_ID_LEN_MAX);
    TRACE_WATCHFACE("[WATCHFACE_GetUnzipFileName]:p_wf_id is %s.",p_file_name);
}
/*****************************************************************************/
//  Description : WATCHFACE_SetUnzipFileName
//  Parameter: [In] p_file_name: 解压缩的文件名字
//             [Return] NONE
//  Author:qi.zhang
// Note:WATCHFACE_SetUnzipFileName
/*****************************************************************************/
PUBLIC void WATCHFACE_SetUnzipFileName(char* p_file_name)
{
    uint16  wf_len = 0;

    if(PNULL == p_file_name)
    {
        TRACE_WATCHFACE("[WATCHFACE_SetUnzipFileName]:p_file_name is pnull.");
        return;
    }
    wf_len = SCI_STRLEN(p_file_name);
    if(0 == wf_len || wf_len > WATCHFACE_WATCHFACE_ID_LEN_MAX)
    {
        TRACE_WATCHFACE("[WATCHFACE_SetUnzipFileName]:watchface id length error (length = %d)",wf_len);
        return;
    }
    SCI_MEMSET(s_unzip_filename, 0, (WATCHFACE_WATCHFACE_ID_LEN_MAX + 1));
    SCI_MEMCPY(s_unzip_filename, p_file_name, wf_len);
    TRACE_WATCHFACE("[WATCHFACE_SetUnzipFileName]:s_unzip_filename is %s.",s_unzip_filename);
}

/*****************************************************************************/
//  Description : WATCHFACE_StartUnzip
//  Parameter: [In] p_wf_id: 表盘id
//             [Return] NONE
//  Author:qi.zhang
// Note:WATCHFACE_StartUnzip
/*****************************************************************************/
PUBLIC BOOLEAN WATCHFACE_StartUnzip(char *p_wf_id)
{
    char *p_file_dir = PNULL;
    char str_full_path[MMIFILE_FULL_PATH_MAX_LEN+1] = {0};
    wchar base_dir[MMIFILE_FULL_PATH_MAX_LEN+1] = {0};
    wchar file_name[MMIFILE_FULL_PATH_MAX_LEN+1] = {0};
    wchar wstr_full_path[MMIFILE_FULL_PATH_MAX_LEN+1] = {0};

    if(PNULL == p_wf_id)
    {
        TRACE_WATCHFACE("pid is null");
        return FALSE;
    }
    p_file_dir = WATCHFACE_Get_UnintalledFileFolder();
    MMIAPICOM_StrToWstr(p_file_dir, base_dir);
    MMIAPICOM_StrToWstr(p_wf_id, file_name);
    WATCHFACE_SetUnzipFileName(p_wf_id);
    sprintf(str_full_path,"%s\\%s.zip",p_file_dir,p_wf_id);
    MMIAPICOM_StrToWstr(str_full_path, wstr_full_path);
#ifdef THEME_UNZIP_SUPPORT
    theme_unzip(wstr_full_path,base_dir,file_name);
#endif
    WATCHFACE_Set_InstallStatus(WATCHFACE_INSTALL_STATUS_UNZIPING);
    return TRUE;
}

/*****************************************************************************/
//  Description : WATCHFACE_AddDescription_Info
//  Parameter: [In] p_wf_id:表盘ID
//             [Out]NONE
//             [Return]WATCHFACE_STATUS_RES_E
//  Author:qi.zhang
/*****************************************************************************/
PUBLIC WATCHFACE_STATUS_RES_E WATCHFACE_Add_Unistallwflist_Info(COMMON_CONFIG_OBJECT_T* p_wf_info)
{
    uint32                  write_size = 0;
    uint32                  watch_write_size = 0;
    SFS_HANDLE              json_file_handle = NULL;
    WATCHFACE_STATUS_RES_E  result = WATCHFACE_STATUS_RES_MAX;

    if(TRUE == IsExsitSameWatchfaceID(p_wf_info->id))
    {
        TRACE_WATCHFACE("[WATCHFACE_Add_Unistallhas exsit");
        result = WATCHFACE_STATUS_RES_FAIL;
        return result;
    }

    result = Add_WatchfaceCfg_Info(WATCHFACE_CONFIG_TYPE_UNINSTALLED, p_wf_info);
    if(WATCHFACE_STATUS_RES_FILE_NOT_EXISTED == result) //json文件不存在就新建
    {
        json_file_handle = SFS_CreateFile (UNINSTALLED_WATCHFACE_CONFIG_FILE_PATH, SFS_MODE_WRITE | SFS_MODE_CREATE_ALWAYS, NULL, NULL);
        if(NULL == json_file_handle)
        {
            TRACE_WATCHFACE("[WATCHFACE_Add_Unistallwflist_Info]:creat JSON file failed");
            return WATCHFACE_STATUS_RES_FAIL;
        }
        else
        {
            TRACE_WATCHFACE("[WATCHFACE_Add_Unistallwflist_Info]:creat JSON file succeed");
            watch_write_size = SCI_STRLEN(s_watchdace_json_header_info);
            if (SFS_INVALID_HANDLE != json_file_handle)
            {
                if (SFS_ERROR_NONE != SFS_WriteFile (json_file_handle, (const void *)s_watchdace_json_header_info,
                    watch_write_size, &write_size, NULL))
                {
                    TRACE_WATCHFACE("[WATCHFACE_Add_Unistallwflist_Info]:write JSON file failed");
                    SFS_CloseFile(json_file_handle);
                    return WATCHFACE_STATUS_RES_FAIL;
                }
            }
            SFS_CloseFile(json_file_handle);
        }
        result = Add_WatchfaceCfg_Info(WATCHFACE_CONFIG_TYPE_UNINSTALLED, p_wf_info);
    }
    TRACE_WATCHFACE("[WATCHFACE_Add_Unistallwflist_Info]:Add result is %d",result);
    return result;
}
/*****************************************************************************/
//  Description : 表盘恢复出厂设置接口
//  Parameter: [In] NONE
//             [Out]NONE
//             [Return]NONE
//  Author:qi.zhang
/*****************************************************************************/
PUBLIC void WATCHFACE_FactoryReset(void)
{
    uint16                  totalnum = 0;
    uint32                  num = 0;
    WATCHFACE_STATUS_RES_E  res = 0;

    totalnum = WATCHFACE_Get_Watchface_Num();
    TRACE_WATCHFACE("[config]WATCHFACE_FactoryReset enter.");
    if (0 == totalnum || DEFAULT_WATCHFACE_NUM > totalnum)
    {
        TRACE_WATCHFACE("[config]totalnum is error.");
        MMK_SendMsg(MMISET_FACTORY_RESET_WIN_ID,MSG_WATCHFACE_FACTORY_RESET,PNULL);
        return ;
    }
    else if(DEFAULT_WATCHFACE_NUM == totalnum)
    {
        TRACE_WATCHFACE("[config]no new watchface expect default.");
        res = WATCHFACE_Set_CurWatchface_Index(0);
        if(res != WATCHFACE_STATUS_RES_SUCCESS)
        {
            TRACE_WATCHFACE("[config]set current indx=0 error");
        }
        MMK_SendMsg(MMISET_FACTORY_RESET_WIN_ID,MSG_WATCHFACE_FACTORY_RESET,PNULL);
        return;
    }
    else
    {
        TRACE_WATCHFACE("enter");
        //num = totalnum - DEFAULT_WATCHFACE_NUM;
        s_watchface_status = WATCHFACE_STATUS_RES_FACTORY_RESET;
        res = WATCHFACE_DeleteWatchface(WATCHFACE_CONFIG_TYPE_INSTALLED, DEFAULT_WATCHFACE_NUM);
        //进入等待界面
        WATCHFACE_WaitingFactoryResetWinEnter();
        if(res != WATCHFACE_STATUS_RES_SUCCESS)
        {
            TRACE_WATCHFACE("[config]delete watchface error");
        }
    }
}

/*****************************************************************************/
//  Description : WATCHFACE_Get_InstallStatus
//  Parameter: [In] NONE
//             [Out]NONE
//             [Return]WATCHFACE_INSTALL_STATUS_E
//  Author:chunjuan.liang
/*****************************************************************************/
PUBLIC WATCHFACE_INSTALL_STATUS_E WATCHFACE_Get_InstallStatus(void)
{
    TRACE_WATCHFACE("s_watchface_install_status %d",g_watchface_install_status);

    return g_watchface_install_status;
}

/*****************************************************************************/
//  Description : WATCHFACE_Set_InstallStatus
//  Parameter: [In] NONE
//             [Out]NONE
//             [Return]WATCHFACE_INSTALL_STATUS_E
//  Author:chunjuan.liang
/*****************************************************************************/

PUBLIC void WATCHFACE_Set_InstallStatus(WATCHFACE_INSTALL_STATUS_E status)
{
    TRACE_WATCHFACE("status %d",status);

    g_watchface_install_status = status;
}



