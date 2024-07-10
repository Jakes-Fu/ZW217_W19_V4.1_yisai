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

/*����APP ��enum*/
typedef enum
{
    APPSTORE_INSTALLED_APP_SET_TYPE_REWRITE,
    APPSTORE_INSTALLED_APP_SET_TYPE_ADD,
    APPSTORE_INSTALLED_APP_SET_TYPE_DELETE,
    APPSTORE_INSTALLED_APP_SET_TYPE_MAX
} APPSTORE_INSTALLED_APP_SET_TYPE_E;

/*appp store control�ص� callback���͵���Ϣ*/
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
//  Parameter: [In]  app_node_len:ÿ��app node ��Ա�Ĵ�С
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
//  Parameter: [In]  app_node_index: ��Ҫ��ȡ��app node�ڴ�app list�ж�Ӧ��index
//             [In]  p_app_list: the app list
//             [Out] p_app_node:��ȡ��app node
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
//  Description : ��װָ��APP
//  Parameter: [In] ����װ��APP ������Ϣ
//             [Out] None
//             [Return] APPSTORE_RES_E
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
PUBLIC APPSTORE_RES_E APPSTORE_InstallAPP( APPSTORE_APP_NODE_T* p_app_node );

/*****************************************************************************/
//  Description : ж��ָ��APP
//  Parameter: [In] ��ж�ص�APP ������Ϣ
//             [Out] None
//             [Return] APPSTORE_RES_E
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
PUBLIC APPSTORE_RES_E APPSTORE_UninstallAPP( APPSTORE_APP_NODE_T* p_app_node );

/*****************************************************************************/
//  Description : ����ָ��APP
//  Parameter: [In] �����µ�APP ������Ϣ
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
//  Description : ��ȡ�Ѱ�װAPP List
//  Parameter: [In] None
//             [Out] �Ѱ�װAPP List
//             [Return] APPSTORE_RES_E
//  Author: xiuyun.wang
//  Note:
/*****************************************************************************/
PUBLIC APPSTORE_RES_E APPSTORE_GetInstalledAppList(APPSTORE_APP_LIST_T** p_installed_app_list);
/*****************************************************************************/
//  Description :����ָ��APP Node���Ѱ�װAPP List
//  Parameter: [In]app_node_info: ������app node info
//             [In]app_set_type:�����õ�����
//             [Out] None
//             [Return] APPSTORE_RES_E
//  Author: xiuyun.wang
//  Note:
/*****************************************************************************/
PUBLIC APPSTORE_RES_E APPSTORE_SetInstalledAppList( APPSTORE_APP_NODE_T app_node_info, APPSTORE_INSTALLED_APP_SET_TYPE_E app_set_type);

/*****************************************************************************/
//  Description : ��ȡģ��server��APP List
//  Parameter: [In] None
//             [Out] server��APP List
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
//  Note: ���Ѱ�װ��server�˵�APP�б�����Ϊһ���б�
/*****************************************************************************/
PUBLIC APPSTORE_RES_E APPSTORE_GenerateFullAppList(APPSTORE_APP_LIST_T* p_server_app_list,
                                                                 APPSTORE_APP_LIST_T* p_installed_app_list,
                                                                 APPSTORE_APP_LIST_T** p_full_app_list);

/*****************************************************************************/
//  Description :��ȡis_processing
//  Parameter: [In] is_processing
//             [Out] None
//             [Return] None
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
PUBLIC void APPSTORE_SetIsProcessing( BOOLEAN is_processing );

/*****************************************************************************/
//  Description :��ȡis_processing
//  Parameter: [In] None
//             [Out] None
//             [Return] is_processing
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN APPSTORE_GetIsProcessing( void );

/*****************************************************************************/
//  Description :����current node info
//  Parameter: [In] s_cur_node_info
//             [Out] None
//             [Return] None
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
PUBLIC void APPSTORE_SetCurNodeInfo( APPSTORE_APP_NODE_T* p_app_node );

/*****************************************************************************/
//  Description :��ȡcurrent node info
//  Parameter: [In] None
//             [Out] None
//             [Return] s_cur_node_info
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
PUBLIC APPSTORE_APP_NODE_T* APPSTORE_GetCurNodeInfo( void );

/*****************************************************************************/
//  Description :��ȡs_p_full_app_list
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
//  Description : APP Store��ʼ��
//  Parameter: [In] None
//             [Out] None
//             [Return] APPSTORE_RES_E
//  Author: xiuyun.wang
//  Note:
/*****************************************************************************/
PUBLIC APPSTORE_RES_E APPSTORE_Init(void);

/*****************************************************************************/
//  Description : ע��client����
//  Parameter: [In] appstore control��client��callback����
//             [Out] None
//             [Return] APPSTORE_RES_E
//  Author: xiuyun.wang
//  Note:
/*****************************************************************************/
PUBLIC uint32 APPSTORE_AddClient(_appstore_client_callback client_callback);
/*****************************************************************************/
//  Description : ȥע��client����
//  Parameter: [In] appstore control��client��callback����
//             [Out] None
//             [Return] APPSTORE_RES_E
//  Author: xiuyun.wang
//  Note:
/*****************************************************************************/
PUBLIC void APPSTORE_DeleteClient(uint32 appstore_handle);

/*****************************************************************************/
//  Description : use app id get app node from app list,��Ҫʹ����ɺ�����ͷŴ�APP Node
//  Parameter: [In]  p_app_id: ��Ҫ��ȡ��app node��Ӧ��id
//             [In]  p_app_list: the app list
//             [Out] p_app_node:��ȡ��app node
//             [Return] add result
//  Author:donglin.su
//  Note:
/*****************************************************************************/
PUBLIC APPSTORE_RES_E APPSTORE_GetAppNodeByAppId(uint8* p_app_id, APPSTORE_APP_LIST_T app_list,
                                                                   APPSTORE_APP_NODE_T** p_app_node);

/*****************************************************************************/
//  Description : Get App Index By App Id
//  Parameter: [In] p_app_id:Ӧ��ID��Ϣ
//             [In] app_list��Ӧ��ȫ�б�
//             [Out] p_app_index��Ӧ�õ�index
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
//  Description : ��ȡԤ��APP List
//  Parameter: [In] None
//             [Out] Ԥ��APP List
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
//  Note: ��Ԥ�õĺ��Ѱ�װAPP�б�����Ϊһ���б�
/*****************************************************************************/
PUBLIC APPSTORE_RES_E APPSTORE_FullInstalledAppList(APPSTORE_APP_LIST_T** p_full_app_list);

/*****************************************************************************/
//  Description : async function for Debug mode��װָ��APP
//  Parameter: [In] ����װ��APPѹ�������ƣ�������չ��
//             [Out] None
//             [Return] APPSTORE_RES_E
//  Author: xiuyun.wang
//  Note:
/*****************************************************************************/
PUBLIC APPSTORE_RES_E APPSTORE_InstallAPPByDebugMode(uint8 *p_folder_full_name);

/*****************************************************************************/
//  Description : async function for Debug modeж��ָ��APP
//  Parameter: [In] ��ж�ص�APPѹ�������ƣ���������չ��
//             [Out] None
//             [Return] APPSTORE_RES_E
//  Author: xiuyun.wang
//  Note:
/*****************************************************************************/
PUBLIC APPSTORE_RES_E APPSTORE_UninstallAPPByDebugMode(uint8 *p_app_id);

#endif

#endif
