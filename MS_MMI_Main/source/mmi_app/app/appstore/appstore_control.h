/*****************************************************************************
** File Name:        appstore_control.h                                      *
** Author:           xiuyun.wang                                             *
** Date:             08/12/2021                                              *
** Copyright:                                                                *
** Description:    This file is used to define app store control api         *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE                 NAME                  DESCRIPTION                    *
** 08/12/2021           xiuyun.wang           Create                         *
******************************************************************************/
#ifndef _APPSTORE_CONTROL_H_
#define _APPSTORE_CONTROL_H_

/**--------------------------------------------------------------------------*
**                         Include Files                                     *
**---------------------------------------------------------------------------*/
#include "appstore_common.h"
#include "json_parser_config.h"
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                  *
**---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/

/*设置APP 的enum*/
typedef enum
{
    APPSTORE_INSTALLED_APP_SET_TYPE_REWRITE,
    APPSTORE_INSTALLED_APP_SET_TYPE_ADD,
    APPSTORE_INSTALLED_APP_SET_TYPE_DELETE,
    APPSTORE_INSTALLED_APP_SET_TYPE_MAX
} APPSTORE_INSTALLED_APP_SET_TYPE_E;

/*appp store control回调 callback传送的消息*/
typedef enum
{
    MSG_APPSTORE_UPDATE_APP,
    MSG_APPSTORE_MAX
} MSG_APPSTORE_E;

typedef BOOLEAN(* _appstore_client_callback)(MSG_APPSTORE_E msg_id);

/* APPStore message body for UDB*/
typedef struct{
    SIGNAL_VARS
    uint8       app_name[MAX_NAME_LEN*2+1];
}APPSTORE_DEBUG_MODE_SIG_T;


/**--------------------------------------------------------------------------*
**                         FUNCTION DECLARATION                              *
**---------------------------------------------------------------------------*/
#ifdef APP_STORE_SUPPORT
/*****************************************************************************/
//  Description : APPStore do factory reset
//  Parameter: [In]  None
//             [Out] None
//             [Return] None
//  Author: xiuyun.wang
//  Note:
/*****************************************************************************/
PUBLIC void APPSTORE_FactoryReset(void);
/*****************************************************************************/
//  Description : copy file callback
//  Parameter: [In] value
//             [Out] None
//             [Return] APPSTORE_RES_E
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
PUBLIC APPSTORE_RES_E APPSTORE_CopyAPPCallback( uint16 value );

/*****************************************************************************/
//  Description : delete file callback
//  Parameter: [In] value
//             [Out] None
//             [Return] APPSTORE_RES_E
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
PUBLIC APPSTORE_RES_E APPSTORE_DeleteAPPCallback( uint16 value );
/*****************************************************************************/
//  Description : Create app Node space
//  Parameter: [In]  app_node_len:每个app node 成员的大小
//             [Out] p_app_node: the app node
//             [Return] create space result
//  Author:xiuyun.wang
//  Note:
/*****************************************************************************/
PUBLIC APPSTORE_RES_E APPSTORE_CreateAppNode(APPSTORE_APP_NODE_LEN_T app_node_len, APPSTORE_APP_NODE_T** p_app_node);

/*****************************************************************************/
//  Description : Free app Node space
//  Parameter: [In]  p_app_node: the free app node
//             [Out] None
//             [Return] free space result
//  Author:xiuyun.wang
//  Note:
/*****************************************************************************/
PUBLIC APPSTORE_RES_E APPSTORE_FreeAppNode(APPSTORE_APP_NODE_T* p_app_node);

/*****************************************************************************/
//  Description : get app node from app list
//  Parameter: [In]  app_node_index: 想要获取的app node在此app list中对应的index
//             [In]  p_app_list: the app list
//             [Out] p_app_node:获取的app node
//             [Return] add result
//  Author:xiuyun.wang
//  Note:
/*****************************************************************************/
PUBLIC APPSTORE_RES_E APPSTORE_GetAppNodeFromAppList(uint16 app_node_index, APPSTORE_APP_LIST_T p_app_list, APPSTORE_APP_NODE_T** p_app_node);

/*****************************************************************************/
//  Description : Free app list space
//  Parameter: [In]  p_app_list: the free app list
//             [Out] None
//             [Return] free space result
//  Author:xiuyun.wang
//  Note:
/*****************************************************************************/
PUBLIC APPSTORE_RES_E APPSTORE_FreeAppList(APPSTORE_APP_LIST_T* p_app_list);

/*****************************************************************************/
//  Description : 安装指定APP
//  Parameter: [In] 待安装的APP 索引信息
//             [Out] None
//             [Return] APPSTORE_RES_E
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
PUBLIC APPSTORE_RES_E APPSTORE_InstallAPP( APPSTORE_APP_NODE_T* p_app_node );

/*****************************************************************************/
//  Description : 卸载指定APP
//  Parameter: [In] 待卸载的APP 索引信息
//             [Out] None
//             [Return] APPSTORE_RES_E
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
PUBLIC APPSTORE_RES_E APPSTORE_UninstallAPP( APPSTORE_APP_NODE_T* p_app_node );

/*****************************************************************************/
//  Description : 更新指定APP
//  Parameter: [In] 待更新的APP 索引信息
//             [Out] None
//             [Return] APPSTORE_RES_E
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
PUBLIC APPSTORE_RES_E APPSTORE_UpdateAPP( APPSTORE_APP_NODE_T* p_app_node );
/*****************************************************************************/
//  Description : Free app list space
//  Parameter: [In]  p_app_list: the free app list
//             [Out] None
//             [Return] free space result
//  Author:xiuyun.wang
//  Note:
/*****************************************************************************/
PUBLIC APPSTORE_RES_E APPSTORE_FreeAppList(APPSTORE_APP_LIST_T* p_app_list);
/*****************************************************************************/
//  Description : 获取已安装APP List
//  Parameter: [In] None
//             [Out] 已安装APP List
//             [Return] APPSTORE_RES_E
//  Author: xiuyun.wang
//  Note:
/*****************************************************************************/
PUBLIC APPSTORE_RES_E APPSTORE_GetInstalledAppList(APPSTORE_APP_LIST_T** p_installed_app_list);
/*****************************************************************************/
//  Description :设置指定APP Node到已安装APP List
//  Parameter: [In]app_node_info: 待设置app node info
//             [In]app_set_type:待设置的类型
//             [Out] None
//             [Return] APPSTORE_RES_E
//  Author: xiuyun.wang
//  Note:
/*****************************************************************************/
PUBLIC APPSTORE_RES_E APPSTORE_SetInstalledAppList( APPSTORE_APP_NODE_T app_node_info, APPSTORE_INSTALLED_APP_SET_TYPE_E app_set_type);

/*****************************************************************************/
//  Description : 获取模拟server端APP List
//  Parameter: [In] None
//             [Out] server端APP List
//             [Return] APPSTORE_RES_E
//  Author: xiuyun.wang
//  Note:
/*****************************************************************************/
PUBLIC APPSTORE_RES_E APPSTORE_GetServerAppList(APPSTORE_APP_LIST_T** p_server_app_list);

/*****************************************************************************/
//  Description : Generate Full App List
//  Parameter: [In] p_server_app_list: List of Installed App
//             [In] p_installed_app_list: List of App on the Server
//             [Out] p_full_app_list: Full App List
//             [Return] APPSTORE_RES_E
//  Author: mingwei.jia, fangfang.yao
//  Note: 将已安装和server端的APP列表整合为一个列表
/*****************************************************************************/
PUBLIC APPSTORE_RES_E APPSTORE_GenerateFullAppList(APPSTORE_APP_LIST_T* p_server_app_list,
                                                                 APPSTORE_APP_LIST_T* p_installed_app_list,
                                                                 APPSTORE_APP_LIST_T** p_full_app_list);

/*****************************************************************************/
//  Description :获取is_processing
//  Parameter: [In] is_processing
//             [Out] None
//             [Return] None
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
PUBLIC void APPSTORE_SetIsProcessing( BOOLEAN is_processing );

/*****************************************************************************/
//  Description :获取is_processing
//  Parameter: [In] None
//             [Out] None
//             [Return] is_processing
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN APPSTORE_GetIsProcessing( void );

/*****************************************************************************/
//  Description :设置current node info
//  Parameter: [In] s_cur_node_info
//             [Out] None
//             [Return] None
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
PUBLIC void APPSTORE_SetCurNodeInfo( APPSTORE_APP_NODE_T* p_app_node );

/*****************************************************************************/
//  Description :获取current node info
//  Parameter: [In] None
//             [Out] None
//             [Return] s_cur_node_info
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
PUBLIC APPSTORE_APP_NODE_T* APPSTORE_GetCurNodeInfo( void );

/*****************************************************************************/
//  Description :获取s_p_full_app_list
//  Parameter: [In] None
//             [Out] None
//             [Return] s_p_full_app_list
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
PUBLIC APPSTORE_APP_LIST_T* APPSTORE_GetFullAppList( void );

/*****************************************************************************/
//  Description : get full app list and set data list.
//  Parameter: [In] None
//             [Out] None
//             [Return] APPSTORE_RES_E
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
PUBLIC APPSTORE_RES_E APPSTORE_UpdateFullAppListForData( void );

/*****************************************************************************/
//  Description : APP Store初始化
//  Parameter: [In] None
//             [Out] None
//             [Return] APPSTORE_RES_E
//  Author: xiuyun.wang
//  Note:
/*****************************************************************************/
PUBLIC APPSTORE_RES_E APPSTORE_Init(void);

/*****************************************************************************/
//  Description : 注册client函数
//  Parameter: [In] appstore control的client的callback函数
//             [Out] None
//             [Return] APPSTORE_RES_E
//  Author: xiuyun.wang
//  Note:
/*****************************************************************************/
PUBLIC uint32 APPSTORE_AddClient(_appstore_client_callback client_callback);
/*****************************************************************************/
//  Description : 去注册client函数
//  Parameter: [In] appstore control的client的callback函数
//             [Out] None
//             [Return] APPSTORE_RES_E
//  Author: xiuyun.wang
//  Note:
/*****************************************************************************/
PUBLIC void APPSTORE_DeleteClient(uint32 appstore_handle);

/*****************************************************************************/
//  Description : use app id get app node from app list,需要使用完成后进行释放此APP Node
//  Parameter: [In]  p_app_id: 想要获取的app node对应的id
//             [In]  p_app_list: the app list
//             [Out] p_app_node:获取的app node
//             [Return] add result
//  Author:donglin.su
//  Note:
/*****************************************************************************/
PUBLIC APPSTORE_RES_E APPSTORE_GetAppNodeByAppId(uint8* p_app_id, APPSTORE_APP_LIST_T app_list,
                                                                   APPSTORE_APP_NODE_T** p_app_node);

/*****************************************************************************/
//  Description : Get App Index By App Id
//  Parameter: [In] p_app_id:应用ID信息
//             [In] app_list：应用全列表
//             [Out] p_app_index：应用的index
//             [Return] APPSTORE_RES_E
//  Author: mingwei.jia
//  Note:
/*****************************************************************************/
PUBLIC APPSTORE_RES_E APPSTORE_GetAppIndexByAppId(uint8* p_app_id,
                                                                             APPSTORE_APP_LIST_T app_list,
                                                                             uint16* p_app_index);

/*****************************************************************************/
//  Description : Check the Space of disk C
//  Parameter: [In] p_app_node
//             [Out] is_enough
//             [Return] APPSTORE_RES_E
//  Author: mingwei.jia
//  Note:
/*****************************************************************************/
PUBLIC APPSTORE_RES_E APPSTORE_CheckSpace(APPSTORE_APP_NODE_T* p_app_node,
                                                      BOOLEAN* is_enough );

/*****************************************************************************/
//  Description : 获取预置APP List
//  Parameter: [In] None
//             [Out] 预置APP List
//             [Return] APPSTORE_RES_E
//  Author: mingwei.jia
//  Note:
/*****************************************************************************/
PUBLIC APPSTORE_RES_E APPSTORE_GetPreInstalledAppList(APPSTORE_APP_LIST_T** p_pre_installed_app_list);

/*****************************************************************************/
//  Description : Generate preset and installed App List
//  Parameter: [In] p_preset_app_list: List of preset App
//             [In] p_installed_app_list: List of installed app
//             [Out] p_full_app_list: full App List
//             [Return] APPSTORE_RES_E
//  Author: mingwei.jia
//  Note: 将预置的和已安装APP列表整合为一个列表
/*****************************************************************************/
PUBLIC APPSTORE_RES_E APPSTORE_FullInstalledAppList(APPSTORE_APP_LIST_T** p_full_app_list);

/*****************************************************************************/
//  Description : async function for Debug mode安装指定APP
//  Parameter: [In] 待安装的APP压缩包名称，包含扩展名
//             [Out] None
//             [Return] APPSTORE_RES_E
//  Author: xiuyun.wang
//  Note:
/*****************************************************************************/
PUBLIC APPSTORE_RES_E APPSTORE_InstallAPPByDebugMode(uint8 *p_folder_full_name);

/*****************************************************************************/
//  Description : async function for Debug mode卸载指定APP
//  Parameter: [In] 待卸载的APP压缩包名称，不包含扩展名
//             [Out] None
//             [Return] APPSTORE_RES_E
//  Author: xiuyun.wang
//  Note:
/*****************************************************************************/
PUBLIC APPSTORE_RES_E APPSTORE_UninstallAPPByDebugMode(uint8 *p_app_id);

#endif

#endif
