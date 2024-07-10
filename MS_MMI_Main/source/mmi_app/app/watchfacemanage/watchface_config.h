/*****************************************************************************
** File Name:        watchface_config.h                                      *
** Author:           chunjuan.liang                                          *
** Date:             08/17/2021                                              *
** Copyright:                                                                *
** Description:    This file is used to define app store control api         *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE                 NAME                  DESCRIPTION                    *
** 08/17/2021         chunjuan.liang           Create                        *
******************************************************************************/
#ifndef _WATCHFACE_CONFIG_H_
#define _WATCHFACE_CONFIG_H_

/**--------------------------------------------------------------------------*
**                         Include Files                                     *
**---------------------------------------------------------------------------*/
#include "json_parser_config.h"

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                  *
**---------------------------------------------------------------------------*/
#define WATCHFACE_URL_LEN_MAX            (128)
#define WATCHFACE_CMD_LEN_MAX            (6)
#define WATCHFACE_MSG_ID_LEN_MAX         (8)
#define WATCHFACE_WATCHFACE_ID_LEN_MAX   (50)
#define WATCHFACE_SEND_MSG_HEAD_LEN      (6)
#define WATCHFACE_MAX_NUM                (20)
#define WATCHFACE_FILES_FULLPATH_LEN     (255)
#define WATCHFACE_SEND_CUR_WATCHFACE_MSG_LEN           (60)
#define WATCHFACE_SEND_WATCHFACE_ID_LEN  (64)
#define WATCHFACE_SUFFIX_MAX_LEN         (12)//文件后缀名最大长度
#ifdef PLATFORM_ANTISW3
#define DEFAULT_WATCHFACE_NUM            (4)
#elif defined PLATFORM_UWS6121E
#define DEFAULT_WATCHFACE_NUM            (2)
#endif


/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/

/* 函数运行结果*/
typedef enum
{
    WATCHFACE_FUN_RESULT_SUCCESS = 0,
    WATCHFACE_FUN_RESULT_PARAM_ERR,
    WATCHFACE_FUN_RESULT_FAIL,
    WATCHFACE_FUN_RESULT_MAX
}WATCHFACE_FUN_RESULT_E;

/*watchface中 api 运行结果*/
typedef enum
{
    WATCHFACE_STATUS_RES_SUCCESS = 0,
    WATCHFACE_STATUS_RES_PARAM_ERR,
    WATCHFACE_STATUS_RES_FAIL,
    WATCHFACE_STATUS_RES_FILE_NOT_EXISTED,
    WATCHFACE_STATUS_RES_UPDATE,
    WATCHFACE_STATUS_RES_DOWNLOAD,
    WATCHFACE_STATUS_RES_FACTORY_RESET,
    WATCHFACE_STATUS_RES_MAX
}WATCHFACE_STATUS_RES_E;

typedef enum
{
    WATCHFACE_CONFIG_TYPE_INSTALLED,
    WATCHFACE_CONFIG_TYPE_UNINSTALLED,
    WATCHFACE_CONFIG_TYPE_MAX
}WATCHFACE_CONFIG_TYPE_E;

typedef enum
{
    WATCHFACE_INSTALL_STATUS_IDLE = 0,
    WATCHFACE_INSTALL_STATUS_DOWNLOADING,
    WATCHFACE_INSTALL_STATUS_UNZIPING,
    WATCHFACE_INSTALL_STATUS_COPYING,
    WATCHFACE_INSTALL_STATUS_MAX
}WATCHFACE_INSTALL_STATUS_E;

/**--------------------------------------------------------------------------*
**                         FUNCTION DECLARATION                              *
**---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : WATCHFACE_Get_ConfigFilePath
//  Parameter: [In] NONE
//             [Out] NONE
//             [Return]Config File Path
//  Author:yifan.yang
// Note:Get watchface config file path
/*****************************************************************************/
PUBLIC wchar* WATCHFACE_Get_ConfigFilePath(void);

/*****************************************************************************/
//  Description : WATCHFACE_Get_UninstalledConfigFilePath
//  Parameter: [In] NONE
//             [Out] NONE
//             [Return]Uninstalled Config File Path
//  Author:yifan.yang
// Note:Get watchface uninstalled config file path
/*****************************************************************************/
PUBLIC wchar* WATCHFACE_Get_UninstalledConfigFilePath(void);

/*****************************************************************************/
//  Description : WATCHFACE_Get_InstalledFileFolder
//  Parameter: [In] NONE
//             [Out] NONE
//             [Return]Installed File Folder Path
//  Author:yifan.yang
// Note:Get watchface installed file folder
/*****************************************************************************/
PUBLIC char* WATCHFACE_Get_InstalledFileFolder(void);

/*****************************************************************************/
//  Description : WATCHFACE_Get_UninstalledFileFolder
//  Parameter: [In] NONE
//             [Out] NONE
//             [Return]Uninstalled File Folder Path
//  Author:yifan.yang
// Note:Get watchface uninstalled file folder
/*****************************************************************************/
PUBLIC char* WATCHFACE_Get_UnintalledFileFolder(void);

/*****************************************************************************/
//  Description : WATHCFACE_Get_LayoutFileName
//  Parameter: [In] NONE
//             [Out] NONE
//             [Return]Layout File Name
//  Author:yifan.yang
// Note:Get watchface layout file name
/*****************************************************************************/
PUBLIC char* WATHCFACE_Get_LayoutFileName(void);

/*****************************************************************************/
//  Description : WATCHFACE_Get_Watchface_Num
//  Parameter: [In] NONE
//             [Out] NONE
//             [Return]watchface number
//  Author:yifan.yang
// Note:Get watchface number
/*****************************************************************************/
PUBLIC uint16 WATCHFACE_Get_Watchface_Num(void);

/*****************************************************************************/
//  Description : WATCHFACE_Get_WatchfaceCfg_Info
//  Parameter: [In] type,Json类型
//                   idx,表盘index
//             [Out] p_wf_cfg_info，表盘config信息
//             [Return]WATCHFACE_STATUS_RES_E
//  Author:yifan.yang
// Note:Get watchface config info
/*****************************************************************************/
PUBLIC WATCHFACE_STATUS_RES_E WATCHFACE_Get_WatchfaceCfg_Info(WATCHFACE_CONFIG_TYPE_E type, uint16 idx,COMMON_CONFIG_OBJECT_T* p_wf_cfg_info);

/*****************************************************************************/
//  Description : WATCHFACE_Get_CurWatchface_Index
//  Parameter: [In] None
//             [Out]index
//             [Return]WATCHFACE_STATUS_RES_E
//  Author:yifan.yang
// Note:Get current watchface index
/*****************************************************************************/
PUBLIC WATCHFACE_STATUS_RES_E WATCHFACE_Get_CurWatchface_Index(uint16* index);

/*****************************************************************************/
//  Description : WATCHFACE_Set_CurWatchface_Index
//  Parameter: [In] idx:表盘idx
//             [Out]None
//             [Return]WATCHFACE_STATUS_RES_E
//  Author:yifan.yang
// Note:Set current watchface index
/*****************************************************************************/
PUBLIC WATCHFACE_STATUS_RES_E WATCHFACE_Set_CurWatchface_Index(uint16 idx);

/*****************************************************************************/
//  Description : WATCHFACE_Get_UnWatchface_Num
//  Parameter: [In]None
//             [Out]None
//             [Return] uninstalled watchface number
//  Author:yifan.yang
// Note:Get uninstall watchface number
/*****************************************************************************/
PUBLIC uint16 WATCHFACE_Get_UnWatchface_Num(void);

/*****************************************************************************/
//  Description : WATCHFACE_Get_WatchfaceFile_Path
//  Parameter: [In] idx,表盘idx,
//             [Out]p_install_wf_path,安装表盘的文件路径
//             [Return] WATCHFACE_STATUS_RES_E
//  Author:yifan.yang
// Note:Get watchface path
/*****************************************************************************/
PUBLIC WATCHFACE_STATUS_RES_E WATCHFACE_Get_WatchfaceFile_Path(uint16 idx,char* p_install_wf_path);

/*****************************************************************************/
//  Description : WATCHFACE_Get_CurrentWatchfaceFile_Path
//  Parameter: [In] NONE
//             [Out]p_cur_wf_path,当前表盘的文件路径
//             [Return] WATCHFACE_STATUS_RES_E
//  Author:yifan.yang
// Note:Get current watchface path
/*****************************************************************************/
PUBLIC WATCHFACE_STATUS_RES_E WATCHFACE_Get_CurrentWatchfaceFile_Path(char* p_cur_wf_path);

/*****************************************************************************/
//  Description : WATCHFACE_DeleteWatchface
//  Parameter: [In] watchface index
//             [In] folder type
//             [Out]NONE
//             [Return] WATCHFACE_STATUS_RES_E
//  Author:yifan.yang donglin.su
// Note:delete watchface forlder
/*****************************************************************************/
PUBLIC WATCHFACE_STATUS_RES_E WATCHFACE_DeleteWatchface(WATCHFACE_CONFIG_TYPE_E type, uint16 idx);

/*****************************************************************************/
//  Description : WATCHFACE_DeleteWatchface Callback
//  Parameter: [In]value
//             [Out]NONE
//             [Return] WATCHFACE_STATUS_RES_E
//  Author:yifan.yang donglin.su
// Note:delete watchface forlder callback
/*****************************************************************************/
PUBLIC WATCHFACE_STATUS_RES_E WATCHFACE_DeleteCallback(uint16 value);

/*****************************************************************************/
//  Description : WATCHFACE_CopyWatchface
//  Parameter: [In] watchface config id
//             [Out]NONE
//             [Return] WATCHFACE_STATUS_RES_E
//  Author:donglin.su
// Note:copy watchface forlder
/*****************************************************************************/
PUBLIC WATCHFACE_STATUS_RES_E WATCHFACE_CopyWatchface(char* p_wf_id);

/*****************************************************************************/
//  Description : WATCHFACE_CopyWatchface callback
//  Parameter: [In]value
//             [Out]NONE
//             [Return] WATCHFACE_STATUS_RES_E
//  Author:donglin.su
// Note:copy watchface forlder callback
/*****************************************************************************/
PUBLIC WATCHFACE_STATUS_RES_E WATCHFACE_CopyCallback(uint16 value);

/*****************************************************************************/
//  Description : WATCHFACE_SelfCheckURL
//  Parameter: [In]  none
//             [Out] none
//             [Return] Success:TRUE;Fail:FALSE
//  Author:donglin.su
//  Note:begin watchface http download when should to start
/*****************************************************************************/
PUBLIC BOOLEAN WATCHFACE_SelfCheckURL(void);

/*****************************************************************************/
//  Description : Watchface_Init
//  Parameter: [In]  none
//             [Out] none
//             [Return] Success:TRUE;Fail:FALSE
//  Author:donglin.su
//  Note:init watchface handle ble private message module
/*****************************************************************************/
PUBLIC BOOLEAN WATCHFACE_Init(void);

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
PUBLIC BOOLEAN WATCHFACE_Get_PreImgPath_ByIdx(uint16 idx, uint16 max_len, uint16* p_full_path, uint16* full_path_len);

/****************************************************************************
//  Description : get editable by index
//  Parameter:  [in]: idx            //索引值
//              [out]]: TRUE:editable;FALSE:diseditble
//              [Return]:
//  Author: chunjuan.liang
//  Note:whether editable
*****************************************************************************/
PUBLIC BOOLEAN WATCHFACE_GetEditable_ByIdx(uint16 idx, BOOLEAN *p_editable);
/*****************************************************************************/
//  Description : WATCHFACE_AddDescription_Info
//  Parameter: [In] p_wf_id:表盘ID
//             [Out]NONE
//             [Return]WATCHFACE_STATUS_RES_E
//  Author: qi.zhang
/*****************************************************************************/
PUBLIC WATCHFACE_STATUS_RES_E WATCHFACE_Add_Description_Info(char* p_wf_id);
/*****************************************************************************/
//  Description : WATCHFACE_InstallByDebugMode
//  Parameter: [In] p_full_name:zip full name(压缩文件全名称包括拓展名)
//             [Out]NONE
//             [Return] WATCHFACE_STATUS_RES_E
//  Author:qi.zhang
// Note:Install Watchface
/*****************************************************************************/
PUBLIC WATCHFACE_STATUS_RES_E WATCHFACE_InstallByDebugMode(char* p_full_name);

/*****************************************************************************/
//  Description : WATCHFACE_UninstallByDebugMode
//  Parameter: [In] p_wf_id:wf_id(表盘id对应的文件夹的名字)
//             [Out]NONE
//             [Return] NONE
//  Author:qi.zhang
// Note:Unistall watchface
/*****************************************************************************/
PUBLIC WATCHFACE_STATUS_RES_E WATCHFACE_UninstallByDebugMode(char* p_wf_id);
/*****************************************************************************/
//  Description : WATCHFACE_Deletezip
//  Parameter: [In] p_wf_id: wf_id
//             [Out]NONE
//             [Return] BOOLEAN
//  Author:qi.zhang
// Note:delete zip
/*****************************************************************************/
PUBLIC BOOLEAN WATCHFACE_DeleteZip(char* p_wf_id);
/*****************************************************************************/
//  Description : WATCHFACE_GetUnzipFileName
//  Parameter: [Out] p_file_name: 解压缩的文件名字(不带托拓展名)
//             [Return] NONE
//  Author:qi.zhang
// Note:WATCHFACE_GetUnzipFileName
/*****************************************************************************/
PUBLIC void WATCHFACE_GetUnzipFileName(char* p_file_name);
/*****************************************************************************/
//  Description : WATCHFACE_SetUnzipFileName
//  Parameter: [In] p_file_name: 解压缩的文件名字(不带托拓展名)
//             [Return] NONE
//  Author:qi.zhang
// Note:WATCHFACE_SetUnzipFileName
/*****************************************************************************/
PUBLIC void WATCHFACE_SetUnzipFileName(char* p_file_name);
/*****************************************************************************/
//  Description : WATCHFACE_AddDescription_Info
//  Parameter: [In] p_wf_id:表盘ID
//             [Out]NONE
//             [Return]WATCHFACE_STATUS_RES_E
//  Author:qi.zhang
/*****************************************************************************/
PUBLIC WATCHFACE_STATUS_RES_E WATCHFACE_Add_Unistallwflist_Info(COMMON_CONFIG_OBJECT_T* p_wf_info);
/*****************************************************************************/
//  Description : 表盘恢复出厂设置接口
//  Parameter: [In] NONE
//             [Out]NONE
//             [Return]NONE
//  Author:qi.zhang
/*****************************************************************************/
PUBLIC void WATCHFACE_FactoryReset(void);

/*****************************************************************************/
//  Description : WATCHFACE_Get_InstallStatus
//  Parameter: [In] NONE
//             [Out]NONE
//             [Return]WATCHFACE_INSTALL_STATUS_E
//  Author:chunjuan.liang
/*****************************************************************************/
PUBLIC WATCHFACE_INSTALL_STATUS_E WATCHFACE_Get_InstallStatus(void);

/*****************************************************************************/
//  Description : WATCHFACE_Set_InstallStatus
//  Parameter: [In] WATCHFACE_INSTALL_STATUS_E
//             [Out]NONE
//             [Return]NONE
//  Author:chunjuan.liang
/*****************************************************************************/

PUBLIC void WATCHFACE_Set_InstallStatus(WATCHFACE_INSTALL_STATUS_E status);

#endif
