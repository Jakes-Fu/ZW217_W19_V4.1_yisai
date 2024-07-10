/*****************************************************************************
** File Name:      appstore_control.c                                        *
** Author:         xiuyun.wang                                               *
** Date:           08/12/2021                                                *
** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to define app  store control function   *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 8/2021       xiuyun.wang          Create                                  *
******************************************************************************/

/**--------------------------------------------------------------------------*
**                         Include Files                                     *
**---------------------------------------------------------------------------*/
#include "appstore_control.h"
#include "json_parser_config.h"
#include "sfs.h"
#include "mmi_filetask.h"
#include "appstore_operate_fail_note_win_view.h"
#include "appstore_home_win_view.h"
#include "mmi_filemgr.h"
#ifdef JAVASCRIPT_ECO_ENVIRONMENT_SUPPORT
#include "packagemanager.h"
#endif
#ifdef THEME_UNZIP_SUPPORT
#include "mmk_ext_app.h"
#include "theme_gzip_export.h"
#endif
#include "mmi_appmsg.h"
#include "appstore_operating_note_win_view.h"
#include "appstore_insufficient_space_note_win_view.h"
#include "appstore_bg_operation.h"

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                  *
**---------------------------------------------------------------------------*/
#define APP_STORE_JSON_FILE_ONE_OBJ_MAX_LEN (600)
#define INSTALLED_APP_CONFIG_FILE_PATH  L"D:\\installed\\apps\\APPsList.json"
#define SERVER_APP_CONFIG_FILE_PATH  L"D:\\server\\apps\\APPsList.json"
#define PRE_INSTALLED_APP_CONFIG_FILE_PATH  L"D:\\ur_root\\apps\\APPsList.json"
#define APP_FILES_FOLDER_FULLPATH_LEN   MMIFMM_FILE_FULL_PATH_MAX_LEN
#define INSTALL_APP_FILES_FOLDER        "D:\\installed\\apps"
#define INSTALL_PATH                    "D:\\installed"
#define APPS_FOLDER                     "apps"
#define INSTALL_JS_APP_FILES_FOLDER     INSTALL_APP_FILES_FOLDER //"D:\\jee\\data\\app"    //"C:\\app"
#define SERVER_APP_FILES_FOLDER         "D:\\server\\apps"
#define FILE_FOLDER_BACKSLASH           "\\"
#define APPSTORE_CLIENT_MAX_NUM  (5)
#define _URAPK ".urapk"
#define ZIP_FOLDER_SUFFIX  L"zip"
#define APP_JSON_NAME "app.json"
#define DEFAULT_APP_SIZE 102400
#define RESERVED_SPACE_100KB (102400)//安装/更新APP时，由于文件系统判断，判断剩余空间充足仍会因空间不足而操作失败，故预留100kb空间
/**--------------------------------------------------------------------------*
**                         TYPE AND CONSTANT                                 *
**---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                          LOCAL VARIABLES                                  *
**---------------------------------------------------------------------------*/
LOCAL BOOLEAN s_is_processing = 0;// 当前是否存在正在进行的操作
LOCAL APPSTORE_APP_NODE_T* s_p_cur_node_info = PNULL;// 当前正在操作的node 索引
LOCAL APPSTORE_APP_LIST_T* s_p_full_app_list = PNULL;// 当前存在的所有app信息

LOCAL APP_COPY_HANDLE_T*    s_p_app_copy_handle   = PNULL;// 记录执行复制动作的handle信息
LOCAL APP_DELETE_HANDLE_T*  s_p_app_delete_handle = PNULL;// 记录执行删除动作的handle信息

LOCAL const char s_json_file_header_info[] = "{\"app_info\"\:[]}";
LOCAL uint32 s_appstore_client[APPSTORE_CLIENT_MAX_NUM] = {0};
#ifdef THEME_UNZIP_SUPPORT
MMI_APPLICATION_T            g_appstore_unzip = {0};
#endif

MMI_APPLICATION_T            g_appstore_debug_mode = {0};
//Flag for debug mode operation
LOCAL BOOLEAN s_is_debug_mode_operation = FALSE;
LOCAL BOOLEAN s_is_debug_mode_operation_fail = FALSE;
// only in debug mode delete operation
LOCAL wchar s_unzip_folder_name[APP_FILES_FOLDER_FULLPATH_LEN + 1] = {0};
LOCAL APPSTORE_APP_NODE_T *s_p_debug_app_node = PNULL;
typedef enum
{
    APPSTORE_DEL_STATE_DEL_ZIP_FOLDER,
    APPSTORE_DEL_STATE_DEL_OLD_FOLDER_FOR_INSTALL,
    APPSTORE_DEL_STATE_FACTORY_RESET_JSON_FILE,
#if defined(JAVASCRIPT_ECO_ENVIRONMENT_SUPPORT) && defined(JS_APP_SUPPORT)
    APPSTORE_DEL_STATE_FACTORY_RESET_JS_APP,
#endif
    APPSTORE_DEL_STATE_UNINSTALL_APP,
    APPSTORE_DEL_STATE_IDLE = 0xFF
}APPSTORE_DEL_STATE_E;
LOCAL APPSTORE_DEL_STATE_E s_appstore_del_state = APPSTORE_DEL_STATE_IDLE;

/*---------------------------------------------------------------------------*
**                          LOCAL FUNCTION                                   *
**---------------------------------------------------------------------------*/
LOCAL APPSTORE_RES_E DeleteZipFileAndUnzipFolder(uint8 *p_folder_name);
#ifndef WIN32
#ifdef OSA_SUPPORT
extern int ur_con_printf(const char *format, ...);
#endif
#endif
/*****************************************************************************/
//  Description : APPStore do factory reset
//  Parameter: [In]  None
//             [Out] None
//             [Return] None
//  Author: xiuyun.wang
//  Note:
/*****************************************************************************/
PUBLIC void APPSTORE_FactoryReset(void)
{
    wchar install_apps_path[APP_FILES_FOLDER_FULLPATH_LEN + 1] = {0};
    APPSTORE_RES_E result = APPSTORE_RES_MAX;

    TRACE_APPSTORE("[APPSTORE_CONTROL]: enter");
    if(APPSTORE_DEL_STATE_IDLE != s_appstore_del_state)
    {
        TRACE_APPSTORE("[APPSTORE_CONTROL]:can not delete now, others deleting ");
        APPSTORE_Operating_Note_Win_Enter();
        return ;
    }

    //拼此文件的完整路径
    //sprintf(install_apps_path, "%s", INSTALL_APP_FILES_FOLDER,);
    //TRACE_APPSTORE("[APPSTORE_CONTROL]: folder full name:%s",install_apps_path);
    // 将char类型的字符串数组转化成wchar类型的字符串
    MMI_STRNTOWSTR(install_apps_path,APP_FILES_FOLDER_FULLPATH_LEN, INSTALL_APP_FILES_FOLDER,SCI_STRLEN(INSTALL_APP_FILES_FOLDER),SCI_STRLEN(INSTALL_APP_FILES_FOLDER));
    TRACE_APPSTORE("[APPSTORE_CONTROL]: do c:/installed/apps/folder factory reset");

    //check 文件是否存在
    if(!MMIAPIFMM_IsFolderExist(install_apps_path, MMIAPICOM_Wstrlen(install_apps_path)))
    {
        TRACE_APPSTORE("[APPSTORE_CONTROL]:the specfy folder is not exist");
    }
    else
    {
        MMIFILE_ERROR_E delete_res = 0;
        APP_DELETE_HANDLE_T app_del_handle ={0};
        TRACE_APPSTORE("[APPSTORE_CONTROL]:begin delete specfy folder");
        // 删除"D:\\installed\\apps"路径下的所有文件和文件夹
        {
            wchar install_path[APP_FILES_FOLDER_FULLPATH_LEN + 1] = {0};
            wchar apps_folder[APP_FILES_FOLDER_FULLPATH_LEN + 1] = {0};
            MMI_STRNTOWSTR(install_path,APP_FILES_FOLDER_FULLPATH_LEN, INSTALL_PATH,SCI_STRLEN(INSTALL_PATH),SCI_STRLEN(INSTALL_PATH));
            MMI_STRNTOWSTR(apps_folder,APP_FILES_FOLDER_FULLPATH_LEN, APPS_FOLDER,SCI_STRLEN(APPS_FOLDER),SCI_STRLEN(APPS_FOLDER));
            delete_res = MMIAPIFMM_DeleteAllTreeEx(install_path, apps_folder, FALSE);   //bug2137644
            TRACE_APPSTORE("[APPSTORE_CONTROL]delete_res:%d",delete_res);
            if (0 == delete_res)
            {
                APPSTORE_UpdateFullAppListForData();
            }
            else
            {
                APPSTORE_Operate_Fail_Note_Win_Enter();
            }
        }
#if 0
        // 删除操作
        s_p_app_delete_handle = MMIFILE_DeleteOpen(install_apps_path, MMIAPICOM_Wstrlen(install_apps_path));
        // 判断返回值
        TRACE_APPSTORE("[APPSTORE_CONTROL]:s_p_app_delete_handle = 0x%08x", s_p_app_delete_handle);

        /*删除 JS APP installed folder*/
#if defined(JAVASCRIPT_ECO_ENVIRONMENT_SUPPORT) && defined(JS_APP_SUPPORT)
        //删除 app
        s_appstore_del_state = APPSTORE_DEL_STATE_FACTORY_RESET_JS_APP;
        SCI_MEMCPY(app_del_handle.delete_full_path,install_apps_path,APP_FILES_FOLDER_FULLPATH_LEN);
        app_del_handle.delete_path_len = APP_FILES_FOLDER_FULLPATH_LEN;
        delete_res = MMIFILE_DeleteDirTree(&app_del_handle);
        // 判断返回值
        TRACE_APPSTORE("[APPSTORE_CONTROL]:delete_res = 0x%08x", delete_res);

        //拼此文件的完整路径
        // 将char类型的字符串数组转化成wchar类型的字符串
        SCI_MEMSET(install_apps_path, 0x00, APP_FILES_FOLDER_FULLPATH_LEN*sizeof(wchar));
        MMI_STRNTOWSTR(install_apps_path,APP_FILES_FOLDER_FULLPATH_LEN, INSTALL_JS_APP_FILES_FOLDER,SCI_STRLEN(INSTALL_JS_APP_FILES_FOLDER),SCI_STRLEN(INSTALL_JS_APP_FILES_FOLDER));
        TRACE_APPSTORE("[APPSTORE_CONTROL]: installed JS APP factory reset");

        //check 文件是否存在
        if(!MMIAPIFMM_IsFolderExist(install_apps_path, MMIAPICOM_Wstrlen(install_apps_path)))
        {
            TRACE_APPSTORE("[APPSTORE_CONTROL]:the specfy folder is not exist");
        }
        else
        {
            TRACE_APPSTORE("[APPSTORE_CONTROL]:begin delete specfy folder");
            // 删除操作
            s_appstore_del_state = APPSTORE_DEL_STATE_FACTORY_RESET_JS_APP;
            s_p_app_delete_handle = MMIFILE_DeleteOpen(install_apps_path, MMIAPICOM_Wstrlen(install_apps_path));

            // 判断返回值
            TRACE_APPSTORE("[APPSTORE_CONTROL]:s_p_app_delete_handle = 0x%08x", s_p_app_delete_handle);
        }

#endif
#endif
    }
}

#ifdef JAVASCRIPT_ECO_ENVIRONMENT_SUPPORT
/*****************************************************************************/
//  Description : Install JS APP
//  Parameter: [In]  p_app_id
//             [Out] None
//             [Return] APPSTORE_RES_E
//  Author: mingwei.jia
//  Note:
/*****************************************************************************/
LOCAL APPSTORE_RES_E APPSTORE_Install_JS_APP(uint8* p_app_id)
{
    APPSTORE_RES_E result = APPSTORE_RES_MAX;
    int result_js = -1;
    char app_package_path [MAX_PATH_LENTH*2] = {0};
    wchar w_app_package_path [MAX_PATH_LENTH] = {0};

    sprintf(app_package_path,"%s%s%s%s%s%s",SERVER_APP_FILES_FOLDER,FILE_FOLDER_BACKSLASH,p_app_id,FILE_FOLDER_BACKSLASH,p_app_id,_URAPK);
    TRACE_APPSTORE("[APPSTORE_CONTROL]:app_package_path :%s", app_package_path);

    MMIAPICOM_StrToWstr(app_package_path,w_app_package_path);
    result_js = js_app_install((wchar *)w_app_package_path, MMIAPICOM_Wstrlen((wchar *)w_app_package_path));

    TRACE_APPSTORE("[APPSTORE_CONTROL]:APPSTORE_InstallAPP ret=%d", result_js);
    if(0 == result_js)
    {
        result = APPSTORE_RES_SUCC;
    }
    else
    {
        result = APPSTORE_RES_FAIL;
    }
    return result;
}

/*****************************************************************************/
//  Description : Install JS APP
//  Parameter: [In]  p_app_node
//             [In]  app_set_type:待设置的类型
//             [Return] APPSTORE_RES_E
//  Author: mingwei.jia
//  Note:
/*****************************************************************************/
LOCAL APPSTORE_RES_E APPSTORE_Update_App_List(APPSTORE_APP_NODE_T *p_app_node,APPSTORE_INSTALLED_APP_SET_TYPE_E app_set_type)
{

    APPSTORE_RES_E result = APPSTORE_RES_MAX;
    TRACE_APPSTORE("[APPSTORE_CONTROL]:enter func, app_set_type:%d ", app_set_type);
    result = APPSTORE_SetInstalledAppList(*p_app_node,app_set_type);
    TRACE_APPSTORE("[APPSTORE_CONTROL]:Set Installed App List result = %d", result);

    // 进行更新列表等动作
    if(APPSTORE_RES_SUCC == result)
    {
        result = APPSTORE_UpdateFullAppListForData();
        TRACE_APPSTORE("[APPSTORE_CONTROL]:Update Full App List For Data result = %d", result);
    }

    return result;

}
#endif

/*****************************************************************************/
//  Description : check app node is valid
//  Parameter: [In]  app node
//             [Out] None
//             [Return] check result
//  Author:xiuyun.wang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CheckAppNodeIsValid(APPSTORE_APP_NODE_T app_node)
{
    TRACE_APPSTORE("[APPSTORE_CONTROL]:the app node param:p_app_id:%s, p_app_name:%s",app_node.p_app_id, app_node.p_app_name);

    if( PNULL == app_node.p_app_id || PNULL == app_node.p_app_name)
       // TODO: //|| PNULL == app_node.p_app_version_name || 0 == app_node.app_version_code
    {
        TRACE_APPSTORE("[APPSTORE_CONTROL]:the app node is invalid");
        return FALSE;
    }

    TRACE_APPSTORE("[APPSTORE_CONTROL]:the app node is valid");
    return TRUE;
}


/*---------------------------------------------------------------------------*
**                          GLOBAL FUNCTION                                  *
**---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : Create app Node space
//  Parameter: [In]  None
//             [Out] p_app_node: the app node
//             [Return] create space result
//  Author: xiuyun.wang
//  Note:
/*****************************************************************************/
PUBLIC APPSTORE_RES_E APPSTORE_CreateAppNode(APPSTORE_APP_NODE_LEN_T app_node_len, APPSTORE_APP_NODE_T** p_app_node)
{
    APPSTORE_APP_NODE_T* p_temp_node = PNULL;

    if(PNULL == p_app_node)
    {
        TRACE_APPSTORE("[APPSTORE_CONTROL]:the param is error");
        return APPSTORE_RES_PARAM_ERR;
    }
    if(PNULL != *p_app_node)
    {
        TRACE_APPSTORE("[APPSTORE_CONTROL]:the param is error");
        return APPSTORE_RES_PARAM_ERR;
    }

    //malloc app node
    p_temp_node = (APPSTORE_APP_NODE_T *)SCI_ALLOC_APPZ(sizeof(APPSTORE_APP_NODE_T) );
    if(PNULL == p_temp_node)
    {
        TRACE_APPSTORE("[APPSTORE_CONTROL]:malloc space fail");
        return APPSTORE_RES_FAIL;
    }
    SCI_MEMSET(p_temp_node, 0x00, sizeof(APPSTORE_APP_NODE_T) );

    /*malloc app node content*/
    //malloc app id space
    p_temp_node->p_app_id = (uint8 *)SCI_ALLOC_APPZ(app_node_len.app_id_len +1 );
    if(PNULL == p_temp_node->p_app_id)
    {
        TRACE_APPSTORE("[APPSTORE_CONTROL]:malloc space fail");
        APPSTORE_FreeAppNode(p_temp_node);
        p_temp_node = PNULL;
        return APPSTORE_RES_FAIL;
    }
    SCI_MEMSET(p_temp_node->p_app_id, 0x00, app_node_len.app_id_len +1 );

    //malloc app name space
    p_temp_node->p_app_name = (uint8 *)SCI_ALLOC_APPZ(app_node_len.app_name_len +1 );
    if(PNULL == p_temp_node->p_app_name)
    {
        TRACE_APPSTORE("[APPSTORE_CONTROL]:malloc space fail");
        APPSTORE_FreeAppNode(p_temp_node);
        p_temp_node = PNULL;
        return APPSTORE_RES_FAIL;
    }
    SCI_MEMSET(p_temp_node->p_app_name, 0x00, app_node_len.app_name_len +1 );

    //malloc app version name space
    p_temp_node->p_app_version_name = (uint8 *)SCI_ALLOC_APPZ(app_node_len.app_version_name_len +1 );
    if(PNULL == p_temp_node->p_app_version_name)
    {
        TRACE_APPSTORE("[APPSTORE_CONTROL]:malloc space fail");
        APPSTORE_FreeAppNode(p_temp_node);
        p_temp_node = PNULL;
        return APPSTORE_RES_FAIL;
    }
    SCI_MEMSET(p_temp_node->p_app_version_name, 0x00, app_node_len.app_version_name_len +1 );

    //malloc app type space
    p_temp_node->p_app_type = (uint8 *)SCI_ALLOC_APPZ(app_node_len.app_type_len +1 );
    if(PNULL == p_temp_node->p_app_type)
    {
        TRACE_APPSTORE("[APPSTORE_CONTROL]:malloc space fail");
        APPSTORE_FreeAppNode(p_temp_node);
        p_temp_node = PNULL;
        return APPSTORE_RES_FAIL;
    }
    SCI_MEMSET(p_temp_node->p_app_type, 0x00, app_node_len.app_type_len +1 );

    //malloc app introduction space
    p_temp_node->p_app_introduction = (uint8 *)SCI_ALLOC_APPZ(app_node_len.app_introduction_len +1 );
    if(PNULL == p_temp_node->p_app_introduction)
    {
        TRACE_APPSTORE("[APPSTORE_CONTROL]:malloc space fail");
        APPSTORE_FreeAppNode(p_temp_node);
        p_temp_node = PNULL;
        return APPSTORE_RES_FAIL;
    }
    SCI_MEMSET(p_temp_node->p_app_introduction, 0x00, app_node_len.app_introduction_len +1 );

    //malloc app icon full name space
    p_temp_node->p_app_icon_full_name = (uint8 *)SCI_ALLOC_APPZ(app_node_len.app_icon_full_name_len +1 );
    if(PNULL == p_temp_node->p_app_icon_full_name)
    {
        TRACE_APPSTORE("[APPSTORE_CONTROL]:malloc space fail");
        APPSTORE_FreeAppNode(p_temp_node);
        p_temp_node = PNULL;
        return APPSTORE_RES_FAIL;
    }
    SCI_MEMSET(p_temp_node->p_app_icon_full_name, 0x00, app_node_len.app_icon_full_name_len +1 );

    *p_app_node = p_temp_node;

    TRACE_APPSTORE("[APPSTORE_CONTROL]:malloc space successful");
    return APPSTORE_RES_SUCC;
}


/*****************************************************************************/
//  Description : Free app Node space
//  Parameter: [In]  p_app_node: the free app node
//             [Out] None
//             [Return] free space result
//  Author:xiuyun.wang
//  Note:
/*****************************************************************************/
PUBLIC APPSTORE_RES_E APPSTORE_FreeAppNode(APPSTORE_APP_NODE_T* p_app_node)
{
    if(PNULL == p_app_node)
    {
        TRACE_APPSTORE("[APPSTORE_CONTROL]:the param is error");
        return APPSTORE_RES_SUCC;
    }
    TRACE_APPSTORE("[APPSTORE_CONTROL]:enter");
    //free app id space
    if( PNULL != p_app_node->p_app_id)
    {
        TRACE_APPSTORE("[APPSTORE_CONTROL]:app node, app id:%s", p_app_node->p_app_id);
       // TRACE_APPSTORE("[APPSTORE_CONTROL]:free space p_app_id");
        SCI_FREE(p_app_node->p_app_id);
        p_app_node->p_app_id = PNULL;
    }

    //free app name space
    if(PNULL != p_app_node->p_app_name)
    {
        //TRACE_APPSTORE("[APPSTORE_CONTROL]:free space p_app_name");
        SCI_FREE(p_app_node->p_app_name);
        p_app_node->p_app_name = PNULL;
    }

    //free app version name space
    if(PNULL != p_app_node->p_app_version_name)
    {
        //TRACE_APPSTORE("[APPSTORE_CONTROL]:free space p_app_version_name");
        SCI_FREE(p_app_node->p_app_version_name);
        p_app_node->p_app_version_name = PNULL;
    }

    //free app type space
    if(PNULL != p_app_node->p_app_type)
    {
        //TRACE_APPSTORE("[APPSTORE_CONTROL]:free space p_app_type");
        SCI_FREE(p_app_node->p_app_type);
        p_app_node->p_app_type = PNULL;
    }

    //free app introduction space
    if(PNULL != p_app_node->p_app_introduction)
    {
        //TRACE_APPSTORE("[APPSTORE_CONTROL]:free space p_app_introduction");
        SCI_FREE(p_app_node->p_app_introduction);
        p_app_node->p_app_introduction = PNULL;
    }

    //free app icon full name space
    if(PNULL != p_app_node->p_app_icon_full_name)
    {
        //TRACE_APPSTORE("[APPSTORE_CONTROL]:free space p_app_icon_full_name");
        SCI_FREE(p_app_node->p_app_icon_full_name);
        p_app_node->p_app_icon_full_name = PNULL;
    }

    //free app node space
    TRACE_APPSTORE("[APPSTORE_CONTROL]:free space app node");
    SCI_FREE(p_app_node);
    p_app_node = PNULL;

    TRACE_APPSTORE("[APPSTORE_CONTROL]:free space successful");
    return APPSTORE_RES_SUCC;
}
/*****************************************************************************/
//  Description : Create app List space
//  Parameter: [In]  None
//             [Out] p_app_list: the app list
//             [Return] create space result
//  Author:xiuyun.wang
//  Note:
/*****************************************************************************/
LOCAL APPSTORE_RES_E CreateAppList(APPSTORE_APP_LIST_T** p_app_list)
{
    APPSTORE_APP_LIST_T *p_temp_list = PNULL;

    if(PNULL == p_app_list)
    {
        TRACE_APPSTORE("[APPSTORE_CONTROL]:the param is error");
        return APPSTORE_RES_PARAM_ERR;
    }
    if(PNULL != *p_app_list)
    {
        TRACE_APPSTORE("[APPSTORE_CONTROL]:the param is error");
        return APPSTORE_RES_PARAM_ERR;
    }

    //malloc app list
    p_temp_list = (APPSTORE_APP_LIST_T *)SCI_ALLOC_APPZ(sizeof(APPSTORE_APP_LIST_T));
    if(PNULL == p_temp_list)
    {
        TRACE_APPSTORE("[APPSTORE_CONTROL]:malloc space fail");
        return APPSTORE_RES_FAIL;
    }
    SCI_MEMSET(p_temp_list, 0x00, sizeof(APPSTORE_APP_LIST_T));

    *p_app_list = p_temp_list;
    TRACE_APPSTORE("[APPSTORE_CONTROL]:create app list succ, app_list:%x", *p_app_list);
    return APPSTORE_RES_SUCC;

}
/*****************************************************************************/
//  Description : add app node to app list
//  Parameter: [In]  app_node: the app node
//             [In/Out]  p_app_list: the app list
//             [Out] None
//             [Return] add result
//  Author:xiuyun.wang
//  Note:
/*****************************************************************************/
LOCAL APPSTORE_RES_E AddAppNodeToAppList(APPSTORE_APP_NODE_T app_node, APPSTORE_APP_LIST_T** p_app_list)
{
    APPSTORE_APP_NODE_T*    p_temp_node = PNULL;
    APPSTORE_APP_NODE_T*    p_pre_temp_node = PNULL;
    APPSTORE_APP_NODE_LEN_T new_node_len = {0};
    APPSTORE_RES_E          res = APPSTORE_RES_MAX;
    APPSTORE_APP_NODE_T*    p_new_node= PNULL;
    APPSTORE_APP_LIST_T*    p_temp_list = PNULL;

    if(PNULL == p_app_list)
    {
        TRACE_APPSTORE("[APPSTORE_CONTROL]:the param is error");
        return APPSTORE_RES_PARAM_ERR;
    }
    if(PNULL == *p_app_list)
    {
        TRACE_APPSTORE("[APPSTORE_CONTROL]:param is error");
        return APPSTORE_RES_PARAM_ERR;
    }
    //get app list
    p_temp_list = *p_app_list;

    /*calculate the length for app node content*/
    //app_id_len
    new_node_len.app_id_len = SCI_STRLEN(app_node.p_app_id);
    TRACE_APPSTORE("[APPSTORE_CONTROL]:app_id_len :%x",new_node_len.app_id_len);

    //app_name_len
    new_node_len.app_name_len = SCI_STRLEN(app_node.p_app_name);
    TRACE_APPSTORE("[APPSTORE_CONTROL]:app_name_len :%x",new_node_len.app_name_len);

    //app_version_name_len
    new_node_len.app_version_name_len = SCI_STRLEN(app_node.p_app_version_name);
    TRACE_APPSTORE("[APPSTORE_CONTROL]:app_version_name_len :%x",new_node_len.app_version_name_len);

    //app_type_len
    new_node_len.app_type_len = SCI_STRLEN(app_node.p_app_type);
    TRACE_APPSTORE("[APPSTORE_CONTROL]:app_type_len :%x",new_node_len.app_type_len);

    //app_introduction_len
    new_node_len.app_introduction_len = SCI_STRLEN(app_node.p_app_introduction);
    TRACE_APPSTORE("[APPSTORE_CONTROL]:app_introduction_len :%x",new_node_len.app_introduction_len);

    //app_icon_full_name_len
    new_node_len.app_icon_full_name_len = SCI_STRLEN(app_node.p_app_icon_full_name);
    TRACE_APPSTORE("[APPSTORE_CONTROL]:app_icon_full_name_len :%x",new_node_len.app_icon_full_name_len);

    //create app node
    res = APPSTORE_CreateAppNode(new_node_len,&p_new_node);
    if(APPSTORE_RES_SUCC != res || PNULL == p_new_node)
    {
        TRACE_APPSTORE("[APPSTORE_CONTROL]:create app node fail");
        return APPSTORE_RES_FAIL;
    }

    /*copy app node*/
    //copy app id
    SCI_MEMCPY(p_new_node->p_app_id, app_node.p_app_id, new_node_len.app_id_len);

    //copy app name
    SCI_MEMCPY(p_new_node->p_app_name, app_node.p_app_name, new_node_len.app_name_len);

    //copy app version code
    p_new_node->app_version_code = app_node.app_version_code;

    //copy app version name
    SCI_MEMCPY(p_new_node->p_app_version_name, app_node.p_app_version_name, new_node_len.app_version_name_len);

    //copy app type
    SCI_MEMCPY(p_new_node->p_app_type, app_node.p_app_type, new_node_len.app_type_len);

    //copy app introduction
    SCI_MEMCPY(p_new_node->p_app_introduction, app_node.p_app_introduction, new_node_len.app_introduction_len);

    //copy app icon full name
    SCI_MEMCPY(p_new_node->p_app_icon_full_name, app_node.p_app_icon_full_name, new_node_len.app_icon_full_name_len);

    //copy app size
    p_new_node->app_size = app_node.app_size;

    /*add to app list*/
    p_temp_node = p_temp_list->p_app_node_header;
    if(PNULL == p_temp_node)
    {
        p_temp_list->p_app_node_header = p_new_node;
        p_temp_list->p_app_node_header->p_app_node_next = PNULL;
    }
    else
    {
        while(PNULL != p_temp_node)
        {
            p_pre_temp_node = p_temp_node;
            p_temp_node = p_temp_node->p_app_node_next;
        };
        p_pre_temp_node->p_app_node_next = p_new_node;
    }
    //add app numner for app list
    p_temp_list->app_num ++;
    TRACE_APPSTORE("[APPSTORE_CONTROL]:app number:%d in app list", p_temp_list->app_num);
    return APPSTORE_RES_SUCC;
}

/*****************************************************************************/
//  Description : copy file
//  Parameter: [In] app_node
//             [Out] None
//             [Return] APPSTORE_RES_E
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
LOCAL APPSTORE_RES_E CopyAppFile(APPSTORE_APP_NODE_T app_node)
{
    BOOLEAN is_valid = FALSE;
    uint16 dest_full_path_len = 0;
    char src_app_folder_full_path[APP_FILES_FOLDER_FULLPATH_LEN + 1] = {0};
    char dest_app_folder_full_path[APP_FILES_FOLDER_FULLPATH_LEN + 1] = {0};
    wchar src_app_path_name[APP_FILES_FOLDER_FULLPATH_LEN + 1] = {0};
    wchar dest_app_path_name[APP_FILES_FOLDER_FULLPATH_LEN + 1] = {0};
    wchar dest_apps_folder_path_name[APP_FILES_FOLDER_FULLPATH_LEN + 1] = {0};
    APPSTORE_RES_E result = APPSTORE_RES_MAX;

    TRACE_APPSTORE("[APPSTORE_CONTROL]: enter");
    // 拼接COPY的app所在的文件夹
    sprintf(src_app_folder_full_path, "%s%s%s", SERVER_APP_FILES_FOLDER,FILE_FOLDER_BACKSLASH,app_node.p_app_id);
    // 将char类型的字符串数组转化成wchar类型的字符串
    MMI_STRNTOWSTR(src_app_path_name,APP_FILES_FOLDER_FULLPATH_LEN, (char *)src_app_folder_full_path,SCI_STRLEN(src_app_folder_full_path),SCI_STRLEN(src_app_folder_full_path));

    // 拼接COPY的app将要放置的目标文件夹
    dest_full_path_len = SCI_STRLEN(INSTALL_APP_FILES_FOLDER);
    MMI_STRNTOWSTR(dest_apps_folder_path_name,APP_FILES_FOLDER_FULLPATH_LEN, (char *)INSTALL_APP_FILES_FOLDER,dest_full_path_len,dest_full_path_len);

    // 拼接放置APP info 的目标文件夹
    sprintf(dest_app_folder_full_path, "%s%s%s", INSTALL_APP_FILES_FOLDER,FILE_FOLDER_BACKSLASH,app_node.p_app_id);
    // 将char类型的字符串数组转化成wchar类型的字符串
    MMI_STRNTOWSTR(dest_app_path_name,APP_FILES_FOLDER_FULLPATH_LEN, (char *)dest_app_folder_full_path,SCI_STRLEN(dest_app_folder_full_path),SCI_STRLEN(dest_app_folder_full_path));

    // 判断文件是否存在
    // TODO: 后期实现appstore与文件管理器剥离, FileManager_Task 重构
    if(!MMIAPIFMM_IsFolderExist (src_app_path_name, MMIAPICOM_Wstrlen(src_app_path_name)))
    {
        result = APPSTORE_RES_FAIL;
        TRACE_APPSTORE("[APPSTORE_CONTROL]:src app folder is not exist");
        return result;
    }
    if(!MMIAPIFMM_IsFolderExist (dest_apps_folder_path_name, MMIAPICOM_Wstrlen(dest_apps_folder_path_name)))
    {
        result = APPSTORE_RES_FAIL;
        TRACE_APPSTORE("[APPSTORE_CONTROL]:dest app folder is not exist");
        return result;
    }

    if(MMIAPIFMM_IsFolderExist (dest_app_path_name, MMIAPICOM_Wstrlen(dest_app_path_name)))
    {
        TRACE_APPSTORE("[APPSTORE_CONTROL]: excute delete old folder operation");
        if(APPSTORE_DEL_STATE_IDLE != s_appstore_del_state)
        {
            TRACE_APPSTORE("[APPSTORE_CONTROL]:can not delete now, others deleting ");
            APPSTORE_Operating_Note_Win_Enter();
            return APPSTORE_RES_FAIL;
        }
        s_appstore_del_state = APPSTORE_DEL_STATE_DEL_OLD_FOLDER_FOR_INSTALL;
        // 先删除已有文件夹，再copy目标文件夹
        // TODO: 后期实现appstore与文件管理器剥离, FileManager_Task 重构
        s_p_app_delete_handle = MMIFILE_DeleteOpen(dest_app_path_name, MMIAPICOM_Wstrlen(dest_app_path_name));

        // 判断返回值
        TRACE_APPSTORE("[APPSTORE_CONTROL]:s_p_app_delete_handle = 0x%08x", s_p_app_delete_handle);
        if(PNULL == s_p_app_delete_handle)
        {
            result = APPSTORE_RES_FAIL;
        }
        else
        {
            result = APPSTORE_RES_SUCC;
        }
        TRACE_APPSTORE("[APPSTORE_CONTROL]: exit, result = %d",result);
        return result;
    }

    // 继续COPY操作
    // TODO: 后期实现appstore与文件管理器剥离, FileManager_Task 重构
    s_p_app_copy_handle = MMIFILE_CopyOpen(src_app_path_name, MMIAPICOM_Wstrlen(src_app_path_name),dest_apps_folder_path_name,MMIAPICOM_Wstrlen(dest_apps_folder_path_name));

    // 判断返回值
    TRACE_APPSTORE("[APPSTORE_CONTROL]:s_p_app_copy_handle = 0x%08x", s_p_app_copy_handle);
    if(PNULL == s_p_app_copy_handle)
    {
        result = APPSTORE_RES_FAIL;
    }
    else
    {
        result = APPSTORE_RES_SUCC;
    }

    TRACE_APPSTORE("[APPSTORE_CONTROL]: exit, result = %d",result);
    return result;
}
/*****************************************************************************/
//  Description : 对所有注册appstore的client进行callback回调
//  Parameter: [In] None
//             [Out] None
//             [Return] APPSTORE_RES_E
//  Author: xiuyun.wang
//  Note:
/*****************************************************************************/
LOCAL APPSTORE_RES_E NotifyClientAppState(void)
{
    uint8 client_num = 0;
    uint8 i = 0;

    TRACE_APPSTORE("[APPSTORE_CONTROL]:enter");

    client_num = SCI_STRLEN(s_appstore_client)/sizeof(uint32);
    TRACE_APPSTORE("[APPSTORE_CONTROL]: client_num:%d", client_num);

    for(i = 0; i < client_num; i ++)
    {
        TRACE_APPSTORE("[APPSTORE_CONTROL]: s_appstore_client[i]:%p", s_appstore_client[i]);
        (( _appstore_client_callback )s_appstore_client[i])(MSG_APPSTORE_UPDATE_APP);
    }

    return APPSTORE_RES_SUCC;
}

/*****************************************************************************/
//  Description : copy file callback
//  Parameter: [In] value
//             [Out] None
//             [Return] APPSTORE_RES_E
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
PUBLIC APPSTORE_RES_E APPSTORE_CopyAPPCallback( uint16 value )
{
    APPSTORE_RES_E result = APPSTORE_RES_MAX;
    APPSTORE_APP_NODE_T* p_app_node = PNULL;
    APPSTORE_APP_LIST_T* p_full_app_list = PNULL;
    uint8 unzip_folder_name_str[APP_FILES_FOLDER_FULLPATH_LEN + 1] = {0};

    // 判断s_p_app_copy_handle是否为空，不为空则进行文件关闭操作
    TRACE_APPSTORE("[APPSTORE_CONTROL]:enter, s_p_app_copy_handle = 0x%08x", s_p_app_copy_handle);
    if (PNULL == s_p_app_copy_handle)
    {
        result = APPSTORE_RES_PARAM_ERR;
        TRACE_APPSTORE("[APPSTORE_CONTROL]:the s_p_app_copy_handle is PNULL");
        return result;
    }
    APPFILE_CopyClose (s_p_app_copy_handle);
    s_p_app_copy_handle = PNULL;

    // 查询需要进行操作的app node信息
    p_app_node = APPSTORE_GetCurNodeInfo();
    if(PNULL == p_app_node)
    {
        TRACE_APPSTORE("[APPSTORE_CONTROL]:the p_app_node is PNULL");
        // 设置flag
        APPSTORE_SetIsProcessing(FALSE);
        APPSTORE_SetCurNodeInfo(PNULL);
        APPSTORE_Operate_Fail_Note_Win_Enter();
        return APPSTORE_RES_PARAM_ERR;
    }

    TRACE_APPSTORE("[APPSTORE_CONTROL]:p_app_node->app_node_state = %d", p_app_node->app_node_state);
    // 设置flag
    APPSTORE_SetIsProcessing(FALSE);

    if( SFS_NO_ERROR != value )
    {
        TRACE_APPSTORE("[APPSTORE_CONTROL]:operation failed! value = %d",value);

        APPSTORE_SetCurNodeInfo(PNULL);

        if(APPSTORE_APP_STATE_UPDATING == p_app_node->app_node_state)
        {
            p_app_node->app_node_state = APPSTORE_APP_STATE_NEED_UPDATE;
            TRACE_APPSTORE("[APPSTORE_CONTROL]:p_app_node->app_node_state = %d", p_app_node->app_node_state);
        }
        else if(APPSTORE_APP_STATE_INSTALLING == p_app_node->app_node_state)
        {
            p_app_node->app_node_state = APPSTORE_APP_STATE_UNINSTALL;
            TRACE_APPSTORE("[APPSTORE_CONTROL]:p_app_node->app_node_state = %d", p_app_node->app_node_state);
        }

        NotifyClientAppState();

        APPSTORE_Operate_Fail_Note_Win_Enter();
        return APPSTORE_RES_PARAM_ERR;
    }

    // 判断node状态，进行对应处理
    // APPSTORE_APP_STATE_UPDATING : 表示当前进行的是更新操作且COPY文件夹的操作结束，需要进行更新JSON文件等动作
    if(APPSTORE_APP_STATE_UPDATING == p_app_node->app_node_state)
    {
        result = APPSTORE_SetInstalledAppList(*p_app_node,APPSTORE_INSTALLED_APP_SET_TYPE_REWRITE);
    }

    // APPSTORE_APP_STATE_INSTALLING : 表示当前进行的是安装操作且操作结束，需要进行更新JSON文件等动作
    else if(APPSTORE_APP_STATE_INSTALLING == p_app_node->app_node_state)
    {
        result = APPSTORE_SetInstalledAppList(*p_app_node,APPSTORE_INSTALLED_APP_SET_TYPE_ADD);
    }

    // 进行更新列表等动作
    if(APPSTORE_RES_SUCC == result)
    {
        result = APPSTORE_UpdateFullAppListForData();
    }

    if(s_is_debug_mode_operation == TRUE)
    {
        TRACE_APPSTORE("[APPSTORE_CONTROL]:is debug mode operation, so need delete zip foler");
        MMIAPICOM_WstrToStr(s_unzip_folder_name,unzip_folder_name_str);
        //delete zip folder and unzip folder in D://server/apps
        DeleteZipFileAndUnzipFolder(unzip_folder_name_str);
    }
    // 将全局值重置
    APPSTORE_SetCurNodeInfo(PNULL);
    if(APPSTORE_RES_SUCC != result)
    {
        // display pubwin
        APPSTORE_Operate_Fail_Note_Win_Enter();
    }
    
    TRACE_APPSTORE("[APPSTORE_CONTROL]:exit, result = %d", result);
    return result;
}

/*****************************************************************************/
//  Description : delete file
//  Parameter: [In] app_node
//             [Out] None
//             [Return] APPSTORE_RES_E
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
LOCAL APPSTORE_RES_E DeleteAppFile(APPSTORE_APP_NODE_T app_node)
{
    BOOLEAN is_valid = FALSE;
    char app_name_full_path[APP_FILES_FOLDER_FULLPATH_LEN + 1] = {0};
    wchar src_app_folder_path_name[APP_FILES_FOLDER_FULLPATH_LEN + 1] = {0};
    APPSTORE_RES_E result = APPSTORE_RES_MAX;

    TRACE_APPSTORE("[APPSTORE_CONTROL]: enter");
    // 拼接删除的app所在的文件夹
    sprintf(app_name_full_path, "%s%s%s", INSTALL_APP_FILES_FOLDER,FILE_FOLDER_BACKSLASH,app_node.p_app_id);
    TRACE_APPSTORE("[APPSTORE_CONTROL]:app_name_full_path:%s", app_name_full_path);

    // 将char类型的字符串数组转化成wchar类型的字符串
    MMI_STRNTOWSTR(src_app_folder_path_name,APP_FILES_FOLDER_FULLPATH_LEN, (char *) app_name_full_path,SCI_STRLEN(app_name_full_path),SCI_STRLEN(app_name_full_path));

    // 判断文件是否存在
    // TODO: 后期实现appstore与文件管理器剥离, FileManager_Task 重构
    if(!MMIAPIFMM_IsFolderExist (src_app_folder_path_name, MMIAPICOM_Wstrlen(src_app_folder_path_name)))
    {
        result = APPSTORE_SetInstalledAppList(app_node,APPSTORE_INSTALLED_APP_SET_TYPE_DELETE);
        if(APPSTORE_RES_SUCC == result)
        {
            result = APPSTORE_UpdateFullAppListForData();
        }

        APPSTORE_SetIsProcessing(FALSE);
        APPSTORE_SetCurNodeInfo(PNULL);
        //result = APPSTORE_RES_FAIL;

        TRACE_APPSTORE("[APPSTORE_CONTROL]:file is not exist, so delete finish, result = %d", result);
        return result;
    }

    // 继续删除操作
    // TODO: 后期实现appstore与文件管理器剥离, FileManager_Task 重构
    if(APPSTORE_DEL_STATE_IDLE != s_appstore_del_state)
    {
        TRACE_APPSTORE("[APPSTORE_CONTROL]:can not delete now, others deleting ");
        APPSTORE_Operating_Note_Win_Enter();
        return APPSTORE_RES_FAIL;
    }
    s_appstore_del_state = APPSTORE_DEL_STATE_UNINSTALL_APP;
    s_p_app_delete_handle = MMIFILE_DeleteOpen(src_app_folder_path_name, MMIAPICOM_Wstrlen(src_app_folder_path_name));

    // 判断返回值
    TRACE_APPSTORE("[APPSTORE_CONTROL]:s_p_app_delete_handle = 0x%08x", s_p_app_delete_handle);
    if(PNULL == s_p_app_delete_handle)
    {
        result = APPSTORE_RES_FAIL;
    }
    else
    {
        result = APPSTORE_RES_SUCC;
    }

    TRACE_APPSTORE("[APPSTORE_CONTROL]:exit, result = %d", result);
    return result;

}

/*****************************************************************************/
//  Description : delete file callback
//  Parameter: [In] value
//             [Out] None
//             [Return] APPSTORE_RES_E
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
PUBLIC APPSTORE_RES_E APPSTORE_DeleteAPPCallback( uint16 value )
{
    APPSTORE_RES_E result = APPSTORE_RES_SUCC;
    APPSTORE_APP_NODE_T* p_app_node = PNULL;

    // 判断s_p_app_delete_handle是否为空，不为空则进行文件关闭操作
    TRACE_APPSTORE("[APPSTORE_CONTROL]:s_p_app_delete_handle = 0x%08x", s_p_app_delete_handle);
    if (PNULL == s_p_app_delete_handle)
    {
        result = APPSTORE_RES_PARAM_ERR;
        TRACE_APPSTORE("[APPSTORE_CONTROL]:the s_p_app_delete_handle is PNULL");
        return result;
    }
    MMIFILE_DeleteClose (s_p_app_delete_handle);
    s_p_app_delete_handle = PNULL;

    if( SFS_NO_ERROR != value )
    {
        TRACE_APPSTORE("[APPSTORE_CONTROL]:operation failed! value = %d",value);
        APPSTORE_Operate_Fail_Note_Win_Enter();
        s_appstore_del_state = APPSTORE_DEL_STATE_IDLE;
        result = APPSTORE_RES_PARAM_ERR;
        return result;
    }
    TRACE_APPSTORE("[APPSTORE_CONTROL]:s_appstore_del_state:%d", s_appstore_del_state);
    switch(s_appstore_del_state)
    {
        case APPSTORE_DEL_STATE_DEL_OLD_FOLDER_FOR_INSTALL:
        {
            s_appstore_del_state = APPSTORE_DEL_STATE_IDLE;
            // 查询需要进行操作的app node信息
            p_app_node = APPSTORE_GetCurNodeInfo();
            if(PNULL == p_app_node)
            {
                TRACE_APPSTORE("[APPSTORE_CONTROL]:the p_app_node is PNULL");
                // 设置flag
                APPSTORE_SetIsProcessing(FALSE);
                APPSTORE_SetCurNodeInfo(PNULL);
                APPSTORE_Operate_Fail_Note_Win_Enter();
                result = APPSTORE_RES_PARAM_ERR;
                break;
            }

            // 判断node状态，进行对应处理
            // APPSTORE_APP_STATE_INSTALLED : 表示当前进行的是卸载操作且操作结束，需要进行更新JSON文件和列表等动作
            TRACE_APPSTORE("[APPSTORE_CONTROL]:p_app_node->app_node_state = %d", p_app_node->app_node_state);
            if(
                (APPSTORE_APP_STATE_INSTALLED == p_app_node->app_node_state)
                ||(APPSTORE_APP_STATE_NEED_UPDATE == p_app_node->app_node_state)
                ||(APPSTORE_APP_STATE_OFFLINE == p_app_node->app_node_state)
              )
            {
                result = APPSTORE_SetInstalledAppList(*p_app_node,APPSTORE_INSTALLED_APP_SET_TYPE_DELETE);
                if(APPSTORE_RES_SUCC == result)
                {
                    result = APPSTORE_UpdateFullAppListForData();
                }

                // 将全局值重置
                APPSTORE_SetIsProcessing(FALSE);
                APPSTORE_SetCurNodeInfo(PNULL);
                if(APPSTORE_RES_SUCC != result)
                {
                    // display pubwin
                    APPSTORE_Operate_Fail_Note_Win_Enter();
                }
            }
            // APPSTORE_APP_STATE_UPDATING : 表示当前进行的是更新操作且删除文件夹的操作结束，需要进行文件夹COPY工作
            else if(
                      (APPSTORE_APP_STATE_UPDATING == p_app_node->app_node_state)
                      ||(APPSTORE_APP_STATE_INSTALLING == p_app_node->app_node_state)
                   )
            {
                result = CopyAppFile(*p_app_node);
                if(APPSTORE_RES_SUCC != result)
                {
                    // 设置flag
                    APPSTORE_SetIsProcessing(FALSE);
                    APPSTORE_SetCurNodeInfo(PNULL);
                }
            }
            break;
        }

        case APPSTORE_DEL_STATE_DEL_ZIP_FOLDER:
        {
            s_appstore_del_state = APPSTORE_DEL_STATE_IDLE;
            if(TRUE == s_is_debug_mode_operation)
            {
                s_is_debug_mode_operation = FALSE;
                TRACE_APPSTORE("[APPSTORE_CONTROL]:debug mode delete folder suc!");
                if(TRUE == s_is_debug_mode_operation_fail)
                {
                    s_is_debug_mode_operation_fail = FALSE;
#ifndef WIN32
#ifdef OSA_SUPPORT
                    ur_con_printf("Debug APP operation fail!");
#endif
#endif
                }
                else
                {
#ifndef WIN32
#ifdef OSA_SUPPORT
                    ur_con_printf("Debug APP operation success!");
#endif
#endif
                }
            }
            else
            {
                TRACE_APPSTORE("[APPSTORE_CONTROL]:APPSTORE_DEL_STATE_DEL_ZIP_FOLDER but not debug mode");
            }
            break;
        }
        case APPSTORE_DEL_STATE_FACTORY_RESET_JSON_FILE:
        {
            TRACE_APPSTORE("[APPSTORE_CONTROL]: installed JS APP factory reset json sucess");
            s_appstore_del_state = APPSTORE_DEL_STATE_IDLE;
            result = APPSTORE_UpdateFullAppListForData();

            TRACE_APPSTORE("[APPSTORE_CONTROL]: do install app factory reset success");
            break;
        }
#if defined(JAVASCRIPT_ECO_ENVIRONMENT_SUPPORT) && defined(JS_APP_SUPPORT)
        case APPSTORE_DEL_STATE_FACTORY_RESET_JS_APP:
        {
            TRACE_APPSTORE("[APPSTORE_CONTROL]: do install app factory reset success");
            s_appstore_del_state = APPSTORE_DEL_STATE_IDLE;
            break;
        }
#endif
        case APPSTORE_DEL_STATE_UNINSTALL_APP:
        {
            TRACE_APPSTORE("[APPSTORE_CONTROL]: uinstall APP success");
            s_appstore_del_state = APPSTORE_DEL_STATE_IDLE;
            break;
        }
        default:
            break;
    }

    TRACE_APPSTORE("[APPSTORE_CONTROL]:exit, result = %d", result);
    return result;
}

/*****************************************************************************/
//  Description : get app node from app list,需要使用完成后进行释放此APP Node
//  Parameter: [In]  p_app_node: 想要获取的app node在此app list中对应的index
//             [In]  p_app_list: the app list
//             [Out] p_app_node:获取的app node
//             [Return] add result
//  Author:xiuyun.wang
//  Note:
/*****************************************************************************/
PUBLIC APPSTORE_RES_E APPSTORE_GetAppNodeFromAppList(uint16 app_node_index, APPSTORE_APP_LIST_T app_list, APPSTORE_APP_NODE_T** p_app_node)
{
    APPSTORE_APP_NODE_T*  p_temp_app_node = PNULL;
    uint16 cur_app_index = 0;

    if(PNULL == p_app_node)
    {
        TRACE_APPSTORE("[APPSTORE_CONTROL]:the param is error");
        return APPSTORE_RES_PARAM_ERR;
    }
    if(PNULL != *p_app_node)
    {
        TRACE_APPSTORE("[APPSTORE_CONTROL]:param is error");
        return APPSTORE_RES_PARAM_ERR;
    }

    if(app_node_index >= app_list.app_num)
    {
        TRACE_APPSTORE("[APPSTORE_CONTROL]:app node index is above app list number");
        return APPSTORE_RES_PARAM_ERR;
    }
    TRACE_APPSTORE("[APPSTORE_CONTROL]:app num:%d,app_node_index:%d",app_list.app_num,app_node_index);
    //get app node from app list
    p_temp_app_node = app_list.p_app_node_header;
    while(PNULL != p_temp_app_node)
    {
        if(cur_app_index == app_node_index)
        {
            TRACE_APPSTORE("[APPSTORE_CONTROL]:destination app node :%x, app id:%s", p_temp_app_node,p_temp_app_node->p_app_id);
            *p_app_node = p_temp_app_node;
            break;
        }
        p_temp_app_node = p_temp_app_node->p_app_node_next;
        cur_app_index++;
    }

    if(PNULL == p_temp_app_node)
    {
        TRACE_APPSTORE("[APPSTORE_CONTROL]:get app mode is PNULL");
        return APPSTORE_RES_FAIL;
    }

    return APPSTORE_RES_SUCC;

}

/*****************************************************************************/
//  Description : Free app list space
//  Parameter: [In]  p_app_list: the free app list
//             [Out] None
//             [Return] free space result
//  Author:xiuyun.wang
//  Note:
/*****************************************************************************/
PUBLIC APPSTORE_RES_E APPSTORE_FreeAppList(APPSTORE_APP_LIST_T* p_app_list)
{
    APPSTORE_APP_NODE_T* p_temp_app_node = PNULL;

    if(PNULL == p_app_list)
    {
        TRACE_APPSTORE("[APPSTORE_CONTROL]:param is error");
        return APPSTORE_RES_SUCC;
    }

    //free ervery app node in this app list first
    while(PNULL != p_app_list->p_app_node_header)
    {
        p_temp_app_node = p_app_list->p_app_node_header;
        p_app_list->p_app_node_header = p_temp_app_node->p_app_node_next;
        APPSTORE_FreeAppNode(p_temp_app_node);
        p_temp_app_node = PNULL;
    }

    //free app list
    TRACE_APPSTORE("[APPSTORE_CONTROL]:free app list:%x",p_app_list);
    SCI_FREE(p_app_list);
    p_app_list = PNULL;
    return APPSTORE_RES_SUCC;
}

/*****************************************************************************/
//  Description : 安装指定APP
//  Parameter: [In] 待安装的APP 索引信息
//             [Out] None
//             [Return] APPSTORE_RES_E
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
PUBLIC APPSTORE_RES_E APPSTORE_InstallAPP( APPSTORE_APP_NODE_T* p_app_node )
{
    APPSTORE_RES_E result = APPSTORE_RES_SUCC;
    if(FALSE == APPSTORE_GetIsProcessing())// 当前不存在正在进行的操作
    {
        BOOLEAN is_enough = FALSE;

        // 设置flag
        APPSTORE_SetIsProcessing(TRUE);

        // 查询需要进行操作的app node信息
        if(PNULL == p_app_node)
        {
            TRACE_APPSTORE("[APPSTORE_CONTROL]:the p_app_node is PNULL");
            // 设置flag
            APPSTORE_SetIsProcessing(FALSE);
            APPSTORE_SetCurNodeInfo(PNULL);
            return APPSTORE_RES_PARAM_ERR;
        }
        TRACE_APPSTORE("[APPSTORE_CONTROL]: enter, p_app_id = %s", p_app_node->p_app_id);

        APPSTORE_SetCurNodeInfo(p_app_node);

        TRACE_APPSTORE("[APPSTORE_CONTROL]: app_node_state = %d", p_app_node->app_node_state);

        if(APPSTORE_APP_STATE_UNINSTALL != p_app_node->app_node_state)
        {
            TRACE_APPSTORE("[APPSTORE_CONTROL]:the p_app_node is alreday installed");
            // 设置flag
            APPSTORE_SetIsProcessing(FALSE);
            APPSTORE_SetCurNodeInfo(PNULL);

            NotifyClientAppState();
            return APPSTORE_RES_SUCC;
        }

        //检查空间是否充足
        APPSTORE_CheckSpace(p_app_node,&is_enough);
        if(is_enough)
        {
            /*update app node state and notify client*/
            p_app_node->app_node_state = APPSTORE_APP_STATE_INSTALLING;
            NotifyClientAppState();

#ifdef DYNAMIC_C_APP_SUPPORT
            result = CopyAppFile(*p_app_node);
#endif
#if defined(JAVASCRIPT_ECO_ENVIRONMENT_SUPPORT) && defined(JS_APP_SUPPORT)
            result = APPSTORE_Install_JS_APP(p_app_node->p_app_id);
#endif

            if(APPSTORE_RES_SUCC != result)
            {
                TRACE_APPSTORE("[APPSTORE_CONTROL]:copy file fail, result is %d",result);
                // 设置flag
                APPSTORE_SetIsProcessing(FALSE);
                APPSTORE_SetCurNodeInfo(PNULL);
                return result;
            }
#if defined(JAVASCRIPT_ECO_ENVIRONMENT_SUPPORT) && defined(JS_APP_SUPPORT)
            result = APPSTORE_Update_App_List(p_app_node,APPSTORE_INSTALLED_APP_SET_TYPE_ADD);

            // 设置flag
            APPSTORE_SetIsProcessing(FALSE);
            APPSTORE_SetCurNodeInfo(PNULL);
#endif
            TRACE_APPSTORE("[APPSTORE_CONTROL]:exit, result = %d", result);
            return result;
        }
        else
        {
            // 设置flag
            APPSTORE_SetIsProcessing(FALSE);
            APPSTORE_SetCurNodeInfo(PNULL);

            TRACE_APPSTORE("[APPSTORE_FLOW]: open Insufficient Space note win");
            //打开空间不足弹窗
            APPSTORE_Insufficient_Space_Note_Win_Enter();

            result = APPSTORE_RES_NO_ENOUGH_SPACE;
            TRACE_APPSTORE("[APPSTORE_CONTROL]:exit, result = %d", result);
            return result;
        }
    }
    else // 当前存在正在进行的操作,弹出提示框" 当前正在进行app操作"
    {
        TRACE_APPSTORE("[APPSTORE_CONTROL]: open operating note win");
        APPSTORE_Operating_Note_Win_Enter();
        result = APPSTORE_RES_PARAM_ERR;
    }
    return result;
}
/*****************************************************************************/
//  Description : send message to app task for APPStore async function
//  Parameter: [In] message id
//             [In] message content
//             [Out] None
//             [Return] APPSTORE_RES_E
//  Author: xiuyun.wang
//  Note:
/*****************************************************************************/
LOCAL APPSTORE_RES_E SendMsgToAPPTask(uint32 msg_id, uint8 *p_name)
{
    APPSTORE_DEBUG_MODE_SIG_T *p_signal = PNULL;
    uint32 signal_id = 0;
    uint8  len = 0;
    if(PNULL == p_name )
    {
        TRACE_APPSTORE("[APPSTORE_CONTROL]: folder name is null");
        return APPSTORE_RES_PARAM_ERR;
    }
    if((0 == SCI_STRLEN(p_name))||(MAX_NAME_LEN*2 < SCI_STRLEN(p_name)))
    {
        TRACE_APPSTORE("[APPSTORE_CONTROL]: folder name len is 0 or too long");
        return APPSTORE_RES_PARAM_ERR;
    }

    signal_id = msg_id;
    MmiCreateSignal(signal_id, sizeof(APPSTORE_DEBUG_MODE_SIG_T), (MmiSignalS**)&p_signal);
    if(PNULL == p_signal)
    {
        TRACE_APPSTORE("[APPSTORE_CONTROL]:create signal fail");
        return APPSTORE_RES_FAIL;
    }
    SCI_MEMSET(p_signal->app_name, 0x00, MAX_NAME_LEN*2+1);
    len = SCI_STRLEN(p_name);
    SCI_MEMCPY(p_signal->app_name,p_name,len);
    p_signal->SignalSize = sizeof(APPSTORE_DEBUG_MODE_SIG_T);
    TRACE_APPSTORE("[APPSTORE_CONTROL]:send appstore msg by debug mode to app task");
    //Send the signal to the app task
    MmiSendSignal(P_APP, (MmiSignalS*)p_signal);
    return APPSTORE_RES_SUCC;
}
/*****************************************************************************/
//  Description :  async function Debug mode安装指定APP
//  Parameter: [In] 待安装的APP压缩包名称，包含扩展名
//             [Out] None
//             [Return] APPSTORE_RES_E
//  Author: xiuyun.wang
//  Note:
/*****************************************************************************/
PUBLIC APPSTORE_RES_E APPSTORE_InstallAPPByDebugMode(uint8 *p_zip_file_name)
{
    if(PNULL == p_zip_file_name )
    {
        TRACE_APPSTORE("[APPSTORE_CONTROL]: zip file name is null");
#ifndef WIN32
#ifdef OSA_SUPPORT
        ur_con_printf("APP install fail!,zip file name is null");
#endif
#endif
        return APPSTORE_RES_PARAM_ERR;
    }
    if((0 == SCI_STRLEN(p_zip_file_name))||(MAX_NAME_LEN*2 < SCI_STRLEN(p_zip_file_name)))
    {
        TRACE_APPSTORE("[APPSTORE_CONTROL]: zip file name len is 0 or too long");
#ifndef WIN32
#ifdef OSA_SUPPORT
        ur_con_printf("APP install fail!, zip file name len is 0 or too long, the max valid value is 50 Bytes!");
#endif
#endif
        return APPSTORE_RES_PARAM_ERR;
    }
    TRACE_APPSTORE("[APPSTORE_CONTROL]: zip folder name :%s",p_zip_file_name);

    return SendMsgToAPPTask(MSG_APPSTORE_INSTALL_BY_DEBUG_MODE,p_zip_file_name);
}
/*****************************************************************************/
//  Description : async function debug mode 卸载指定APP
//  Parameter: [In] 待卸载的APP压缩包名称，包含扩展名
//             [Out] None
//             [Return] APPSTORE_RES_E
//  Author: xiuyun.wang
//  Note:
/*****************************************************************************/
PUBLIC APPSTORE_RES_E APPSTORE_UninstallAPPByDebugMode(uint8 *p_app_id)
{
    if(PNULL == p_app_id )
    {
        TRACE_APPSTORE("[APPSTORE_CONTROL]: app id is null");
#ifndef WIN32
#ifdef OSA_SUPPORT
        ur_con_printf("APP uninstall fail!, app id is null");
#endif
#endif
        return APPSTORE_RES_PARAM_ERR;
    }
    if((0 == SCI_STRLEN(p_app_id))||(MAX_NAME_LEN < SCI_STRLEN(p_app_id)))
    {
        TRACE_APPSTORE("[APPSTORE_CONTROL]: app id len is 0 or too long");
#ifndef WIN32
#ifdef OSA_SUPPORT
        ur_con_printf("APP uninstall fail!, app id len is 0 or too long, the max valid value is 50 Bytes!");
#endif
#endif
        return APPSTORE_RES_PARAM_ERR;
    }
    TRACE_APPSTORE("[APPSTORE_CONTROL]: app id :%s",p_app_id);

    return SendMsgToAPPTask(MSG_APPSTORE_UNINSTALL_BY_DEBUG_MODE,p_app_id);
}

/*****************************************************************************/
//  Description : Debug mode 安装指定APP
//  Parameter: [In] 待安装的APP压缩包名称，包含扩展名
//             [Out] None
//             [Return] APPSTORE_RES_E
//  Author: xiuyun.wang
//  Note:
/*****************************************************************************/
LOCAL APPSTORE_RES_E InstallAPPForDebugMode(uint8 *p_zip_file_name)
{
    char src_app_folder_full_path[APP_FILES_FOLDER_FULLPATH_LEN + 1] = {0};
    wchar folder_name_wstr[APP_FILES_FOLDER_FULLPATH_LEN + 1] = {0};
    wchar src_app_folder_path_wstr[APP_FILES_FOLDER_FULLPATH_LEN + 1] = {0};
    APPSTORE_RES_E result = APPSTORE_RES_MAX;
    uint16 full_patch_len = 0;
    uint16 folder_name_len = 0;
    wchar suffix_name[APP_FILES_FOLDER_FULLPATH_LEN + 1] = {0};
    uint16 suffix_len = APP_FILES_FOLDER_FULLPATH_LEN;

    if(PNULL == p_zip_file_name )
    {
        TRACE_APPSTORE("[APPSTORE_CONTROL]: folder name is null");
        return APPSTORE_RES_PARAM_ERR;
    }
    if(0 == SCI_STRLEN(p_zip_file_name))
    {
        TRACE_APPSTORE("[APPSTORE_CONTROL]: folder name len is 0");
        return APPSTORE_RES_PARAM_ERR;
    }
    TRACE_APPSTORE("[APPSTORE_CONTROL]: enter, p_folder_full_name:%s, name_len:%d", p_zip_file_name, SCI_STRLEN(p_zip_file_name));

    full_patch_len = SCI_STRLEN(SERVER_APP_FILES_FOLDER)+ SCI_STRLEN(FILE_FOLDER_BACKSLASH) + SCI_STRLEN(p_zip_file_name);
    if(full_patch_len > APP_FILES_FOLDER_FULLPATH_LEN)
    {
        TRACE_APPSTORE("[APPSTORE_CONTROL]:full zip folder path name is too long, the len is:%d", full_patch_len);
        return APPSTORE_RES_PARAM_ERR;
    }

    //拼此文件的完整路径
    sprintf(src_app_folder_full_path, "%s%s%s", SERVER_APP_FILES_FOLDER,FILE_FOLDER_BACKSLASH,p_zip_file_name);
    TRACE_APPSTORE("[APPSTORE_CONTROL]: folder full name:%s",src_app_folder_full_path);

    // 将char类型的字符串数组转化成wchar类型的字符串
    MMI_STRNTOWSTR(src_app_folder_path_wstr,APP_FILES_FOLDER_FULLPATH_LEN, (char *)src_app_folder_full_path,SCI_STRLEN(src_app_folder_full_path),SCI_STRLEN(src_app_folder_full_path));

    SCI_MEMSET(s_unzip_folder_name,0x00, (APP_FILES_FOLDER_FULLPATH_LEN + 1)*sizeof(wchar));
    /*check suffix is zip*/
    //获取文件夹名称
    MMI_STRNTOWSTR(folder_name_wstr,APP_FILES_FOLDER_FULLPATH_LEN, (char *)p_zip_file_name,SCI_STRLEN(p_zip_file_name),SCI_STRLEN(p_zip_file_name));
    MMIAPIFMM_SplitFileName(folder_name_wstr,MMIAPICOM_Wstrlen(folder_name_wstr), s_unzip_folder_name, &folder_name_len, suffix_name, &suffix_len);
    if(folder_name_len > MAX_NAME_LEN)
    {
        TRACE_APPSTORE("[APPSTORE_CONTROL]: app name len is over size");
        return APPSTORE_RES_PARAM_ERR;
    }
    if(0 != MMIAPICOM_Wstrcmp(suffix_name,ZIP_FOLDER_SUFFIX))
    {
        TRACE_APPSTORE("[APPSTORE_CONTROL]: folder suffix name is not zip");
        // 删除压缩包操作
         MMIAPIFMM_DeleteFile(src_app_folder_path_wstr, PNULL);
        return APPSTORE_RES_PARAM_ERR;
    }
    TRACE_APPSTORE("[APPSTORE_CONTROL]:begin check zip folder is exist?");

    //check 文件是否存在
    if(!MMIAPIFMM_IsFileExist(src_app_folder_path_wstr, MMIAPICOM_Wstrlen(src_app_folder_path_wstr)))
    {
        result = APPSTORE_RES_FAIL;
        TRACE_APPSTORE("[APPSTORE_CONTROL]:src app folder is not exist");
        return result;
    }

    TRACE_APPSTORE("[APPSTORE_CONTROL]:begin  unzip folder ");
    //解压缩此压缩包
    SCI_MEMSET(folder_name_wstr, 0x00, APP_FILES_FOLDER_FULLPATH_LEN*sizeof(wchar));
    MMI_STRNTOWSTR(folder_name_wstr,APP_FILES_FOLDER_FULLPATH_LEN, SERVER_APP_FILES_FOLDER,SCI_STRLEN(SERVER_APP_FILES_FOLDER),SCI_STRLEN(SERVER_APP_FILES_FOLDER));
#ifdef THEME_UNZIP_SUPPORT
    theme_unzip(src_app_folder_path_wstr,folder_name_wstr,s_unzip_folder_name);
#endif

    s_is_debug_mode_operation = TRUE;
    return APPSTORE_RES_SUCC;
}
/*****************************************************************************/
//  Description : 解析app.json文件中app node信息
//  Parameter: [In] file full name of app json
//             [Out] APP Node信息
//             [Return] APPSTORE_RES_E
//  Author: xiuyun.wang
//  Note:
/*****************************************************************************/
LOCAL APPSTORE_RES_E ConstructAppNodeInfoFromAppJson(uint8* p_file_name, APPSTORE_APP_NODE_T** p_app_node)
{
    uint8* p_json_buffer = PNULL;
    SFS_HANDLE json_file_handle = NULL;
    uint32 json_file_size = 0;
    uint32 read_file_size = 0;
    COMMON_CONFIG_OBJECT_T json_object_info = {0};
    APPSTORE_APP_NODE_LEN_T app_node_len = {0};
    APPSTORE_APP_NODE_T* p_temp_node = PNULL;
    APPSTORE_RES_E res = APPSTORE_RES_MAX;
    char src_app_folder_full_path[APP_FILES_FOLDER_FULLPATH_LEN + 1] = {0};
    wchar src_path_wstr[APP_FILES_FOLDER_FULLPATH_LEN + 1] = {0};
    uint16 json_full_path_len = 0;

    if(PNULL == p_file_name)
    {
        TRACE_APPSTORE("[APPSTORE_CONTROL]:the param is error");
        return APPSTORE_RES_PARAM_ERR;
    }

    if(PNULL == p_app_node)
    {
        TRACE_APPSTORE("[APPSTORE_CONTROL]:the param is error");
        return APPSTORE_RES_PARAM_ERR;
    }

    if(PNULL != *p_app_node)
    {
        TRACE_APPSTORE("[APPSTORE_CONTROL]:the app node is not NULL, so need free");
        APPSTORE_FreeAppNode(* p_app_node);
        *p_app_node = PNULL;
    }
    json_full_path_len = SCI_STRLEN(SERVER_APP_FILES_FOLDER)+SCI_STRLEN(FILE_FOLDER_BACKSLASH)*2+SCI_STRLEN(APP_JSON_NAME)+SCI_STRLEN(p_file_name);
    if(json_full_path_len > APP_FILES_FOLDER_FULLPATH_LEN)
    {
        TRACE_APPSTORE("[APPSTORE_CONTROL]: app.json full path name is too long");
        return APPSTORE_RES_PARAM_ERR;
    }
    //拼此文件的完整路径
    sprintf(src_app_folder_full_path, "%s%s%s%s%s", SERVER_APP_FILES_FOLDER,FILE_FOLDER_BACKSLASH,p_file_name,FILE_FOLDER_BACKSLASH,APP_JSON_NAME);
    TRACE_APPSTORE("[APPSTORE_CONTROL]: folder full name:%s",src_app_folder_full_path);
    MMIAPICOM_StrToWstr(src_app_folder_full_path,src_path_wstr);

    /*read installed JSON file*/
    //read json file
    json_file_handle = SFS_CreateFile(src_path_wstr, (SFS_MODE_OPEN_EXISTING | SFS_MODE_SHARE_READ), 0, 0);
    if(json_file_handle == NULL)
    {
        TRACE_APPSTORE("[APPSTORE_CONTROL]:open debug mode json file fail");
        return APPSTORE_RES_FILE_MISSED;
    }

    //get installed json file size
    SFS_GetFileSize(json_file_handle, &json_file_size);

    //malloc json buffer
    p_json_buffer = (uint8*)SCI_ALLOC_APPZ(json_file_size);
    if(PNULL == p_json_buffer)
    {
        TRACE_APPSTORE("[APPSTORE_CONTROL]:malloc s_p_json_buffer fail");
        SFS_CloseFile(json_file_handle);
        return APPSTORE_RES_FAIL;
    }

    if(SFS_ERROR_NONE != SFS_ReadFile(json_file_handle, p_json_buffer, json_file_size, &read_file_size, NULL))
    {
        TRACE_APPSTORE("[APPSTORE_CONTROL]:read json file fail");
        SFS_CloseFile(json_file_handle);

        //free buffer
        SCI_FREE(p_json_buffer);
        p_json_buffer = PNULL;
        return APPSTORE_RES_FAIL;
    }

    SFS_CloseFile(json_file_handle);
    if(json_file_size != read_file_size)
    {
        TRACE_APPSTORE("[APPSTORE_CONTROL]:read json file is invalid, should read %d but read %d actruely",
                            json_file_size, read_file_size);
        //free buffer
        SCI_FREE(p_json_buffer);
        p_json_buffer = PNULL;
        return APPSTORE_RES_FAIL;
    }

    //get one object info
    SCI_MEMSET(&json_object_info, 0x00, sizeof(COMMON_CONFIG_OBJECT_T));
    JSONPARSER_GetAppJsonInfo(p_json_buffer, &json_object_info);
    TRACE_APPSTORE("[APPSTORE_CONTROL]: app id:%s, app name:%s",json_object_info.id, json_object_info.app_name);

    //get app node len
    SCI_MEMSET(&app_node_len, 0x00, sizeof(APPSTORE_APP_NODE_LEN_T));
    app_node_len.app_id_len = SCI_STRLEN(json_object_info.id);
    app_node_len.app_name_len = SCI_STRLEN(json_object_info.app_name);
    app_node_len.app_version_name_len = SCI_STRLEN(json_object_info.version_name);
    app_node_len.app_type_len = SCI_STRLEN(json_object_info.type);
    app_node_len.app_introduction_len = SCI_STRLEN(json_object_info.introduction);
    app_node_len.app_icon_full_name_len = SCI_STRLEN(json_object_info.icon) + SCI_STRLEN(SERVER_APP_FILES_FOLDER)+ SCI_STRLEN(FILE_FOLDER_BACKSLASH);

    //create app node and copy node
    APPSTORE_CreateAppNode(app_node_len,&p_temp_node);
    SCI_MEMCPY(p_temp_node->p_app_id, json_object_info.id, app_node_len.app_id_len);
    SCI_MEMCPY(p_temp_node->p_app_name, json_object_info.app_name, app_node_len.app_name_len);
    p_temp_node->app_version_code = json_object_info.version_code;
    SCI_MEMCPY(p_temp_node->p_app_version_name, json_object_info.version_name, app_node_len.app_version_name_len);
    SCI_MEMCPY(p_temp_node->p_app_type, json_object_info.type, app_node_len.app_type_len);
    SCI_MEMCPY(p_temp_node->p_app_introduction, json_object_info.introduction, app_node_len.app_introduction_len);
    p_temp_node->app_size = DEFAULT_APP_SIZE;
    sprintf(p_temp_node->p_app_icon_full_name, "%s%s%s",  SERVER_APP_FILES_FOLDER,FILE_FOLDER_BACKSLASH,json_object_info.icon);
    TRACE_APPSTORE("[APPSTORE_CONTROL]:app node icon full name:%s",p_temp_node->p_app_icon_full_name);
    p_temp_node->app_node_state = APPSTORE_APP_STATE_UNINSTALL;

    *p_app_node = p_temp_node;

    //free JSON buffer
    SCI_FREE(p_json_buffer);
    p_json_buffer = PNULL;
    return APPSTORE_RES_SUCC;
}
LOCAL APPSTORE_RES_E DeleteZipFileAndUnzipFolder(uint8 *p_folder_name)
{
    char src_app_folder_full_path[APP_FILES_FOLDER_FULLPATH_LEN + 1] = {0};
    wchar src_app_folder_path_name[APP_FILES_FOLDER_FULLPATH_LEN + 1] = {0};
    char zip_folder_full_path[APP_FILES_FOLDER_FULLPATH_LEN + 1] = {0};
    wchar zip_folder_path_wstr[APP_FILES_FOLDER_FULLPATH_LEN + 1] = {0};
    wchar app_[APP_FILES_FOLDER_FULLPATH_LEN + 1] = {0};
    APPSTORE_RES_E result = APPSTORE_RES_SUCC;

    TRACE_APPSTORE("[APPSTORE_CONTROL]: enter");
    if(PNULL == p_folder_name)
    {
        TRACE_APPSTORE("[APPSTORE_CONTROL]: p_folder_name is NULL");
        return APPSTORE_RES_PARAM_ERR;
    }

    /*delete folder first*/
    //拼此文件的完整路径
    sprintf(src_app_folder_full_path, "%s%s%s", SERVER_APP_FILES_FOLDER,FILE_FOLDER_BACKSLASH,p_folder_name);
    TRACE_APPSTORE("[APPSTORE_CONTROL]: folder full name:%s",src_app_folder_full_path);
    // 将char类型的字符串数组转化成wchar类型的字符串
    MMI_STRNTOWSTR(src_app_folder_path_name,APP_FILES_FOLDER_FULLPATH_LEN, (char *)src_app_folder_full_path,SCI_STRLEN(src_app_folder_full_path),SCI_STRLEN(src_app_folder_full_path));

    //拼压缩包的完整路径
    sprintf(zip_folder_full_path, "%s%s",src_app_folder_full_path,".zip");
    TRACE_APPSTORE("[APPSTORE_CONTROL]: zip folder full name:%s",zip_folder_full_path);
    // 将char类型的字符串数组转化成wchar类型的字符串
    MMI_STRNTOWSTR(zip_folder_path_wstr,APP_FILES_FOLDER_FULLPATH_LEN, (char *)zip_folder_full_path,SCI_STRLEN(zip_folder_full_path),SCI_STRLEN(zip_folder_full_path));

    //check 压缩包是否存在
    if(!MMIAPIFMM_IsFileExist (zip_folder_path_wstr, MMIAPICOM_Wstrlen(zip_folder_path_wstr)))
    {
        TRACE_APPSTORE("[APPSTORE_CONTROL]:the specfy zip folder is not exist");
    }
    else
    {
        TRACE_APPSTORE("[APPSTORE_CONTROL]:delete specfy zip folder ");
        // 删除压缩包操作
         MMIAPIFMM_DeleteFile(zip_folder_path_wstr, PNULL);
    }
    if(APPSTORE_DEL_STATE_IDLE != s_appstore_del_state)
    {
        TRACE_APPSTORE("[APPSTORE_CONTROL]:can not delete now, others deleting ");
        APPSTORE_Operating_Note_Win_Enter();
        return APPSTORE_RES_FAIL;
    }

    //check 文件是否存在
    if(!MMIAPIFMM_IsFolderExist(src_app_folder_path_name, MMIAPICOM_Wstrlen(src_app_folder_path_name)))
    {
        TRACE_APPSTORE("[APPSTORE_CONTROL]:the specfy folder is not exist");
    }
    else
    {
        TRACE_APPSTORE("[APPSTORE_CONTROL]:begin delete specfy folder");
        // 删除操作
        // TODO: 后期实现appstore与文件管理器剥离, FileManager_Task 重构
        s_appstore_del_state = APPSTORE_DEL_STATE_DEL_ZIP_FOLDER;
        s_p_app_delete_handle = MMIFILE_DeleteOpen(src_app_folder_path_name, MMIAPICOM_Wstrlen(src_app_folder_path_name));

        // 判断返回值
        TRACE_APPSTORE("[APPSTORE_CONTROL]:s_p_app_delete_handle = 0x%08x", s_p_app_delete_handle);
        if(PNULL == s_p_app_delete_handle)
        {
            result = APPSTORE_RES_FAIL;
        }
        else
        {
            result = APPSTORE_RES_SUCC;
        }
    }

    //free debug app node
    if(PNULL != s_p_debug_app_node)
    {
        TRACE_APPSTORE("[APPSTORE_CONTROL]:free debug app node");
        APPSTORE_FreeAppNode(s_p_debug_app_node);
        s_p_debug_app_node = PNULL;
    }
    return result;
}

#ifdef THEME_UNZIP_SUPPORT
#define ZIP_TOTAL_RATE 100
LOCAL MMI_RESULT_E HandleAppStoreZipMsg(PWND app_ptr, uint16 msg_id, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    THEME_UNZIP_SIG_T unzip_content = {0};
    APPSTORE_RES_E appstore_res = APPSTORE_RES_MAX;

    if(PNULL == param)
    {
        TRACE_APPSTORE("[APPSTORE_CONTROL]:param is PNULL");
        return MMI_RESULT_FALSE;
    }

    unzip_content = *((THEME_UNZIP_SIG_T*)param);

    TRACE_APPSTORE("[APPSTORE_CONTROL]:msg_id = 0x%x",msg_id);
    if(0 != MMIAPICOM_Wstrcmp(unzip_content.sig_content.p_param->p_dir,s_unzip_folder_name))
    {
        TRACE_APPSTORE("[APPSTORE_CONTROL]: the unzip handle is not appstore control");
        return MMI_RESULT_FALSE;
    }

    switch (msg_id)
    {
        case THEME_UNZIP_SCODE_PROCESS:
        {
            if (ZIP_TOTAL_RATE == unzip_content.sig_content.process)
            {
                char debug_app_folder_name[APP_FILES_FOLDER_FULLPATH_LEN + 1] = {0};

                MMIAPICOM_WstrToStr(unzip_content.sig_content.p_param->p_dir,debug_app_folder_name);
                TRACE_APPSTORE("[APPSTORE_CONTROL]:unzip app folder name is %s",debug_app_folder_name);

                //construct app node through app.json file info
                ConstructAppNodeInfoFromAppJson(debug_app_folder_name,&s_p_debug_app_node);
                theme_clean(&unzip_content.sig_content);

                //install app
                appstore_res = APPSTORE_InstallAPP(s_p_debug_app_node);
                if(APPSTORE_RES_SUCC != appstore_res)
                {
                    TRACE_APPSTORE("[APPSTORE_CONTROL]:install APP fail");
                    s_is_debug_mode_operation_fail = TRUE;
                }
#if defined(JAVASCRIPT_ECO_ENVIRONMENT_SUPPORT) && defined(JS_APP_SUPPORT)
                //delete zip folder and unzip folder in D://server/apps
                DeleteZipFileAndUnzipFolder(debug_app_folder_name);
#endif
            }
            break;
        }
        case THEME_UNZIP_SCODE_FAIL:
        {
            TRACE_APPSTORE("[APPSTORE_CONTROL]:unzip fail");
            theme_clean(&unzip_content.sig_content);
            break;
        }
        default:
        {
            result = MMI_RESULT_FALSE;
            break;
        }
    }

    return result;
}
#endif

/*****************************************************************************/
//  Description : Debug mode卸载指定APP
//  Parameter: [In] 待卸载的APP ID
//             [Out] None
//             [Return] APPSTORE_RES_E
//  Author: xiuyun.wang
//  Note:
/*****************************************************************************/
PUBLIC APPSTORE_RES_E UninstallAPPForDebugMode(uint8 *p_app_id)
{
    APPSTORE_APP_NODE_T* p_app_node = PNULL;
    APPSTORE_APP_LIST_T* p_app_list = PNULL;

    if(PNULL == p_app_id)
    {
        TRACE_APPSTORE("[APPSTORE_CONTROL]:UBD uninstall, app id is null");
        return APPSTORE_RES_PARAM_ERR;
    }
    TRACE_APPSTORE("[APPSTORE_CONTROL]:UBD uninstall, app id:%s",p_app_id);

    //get app node from install app list
    p_app_list = APPSTORE_GetFullAppList();
    APPSTORE_GetAppNodeByAppId(p_app_id, *p_app_list, &p_app_node);
    if((PNULL == p_app_node)||(PNULL == p_app_node->p_app_id))
    {
        TRACE_APPSTORE("[APPSTORE_CONTROL]:get app node is null");
        return APPSTORE_RES_PARAM_ERR;
    }

    TRACE_APPSTORE("[APPSTORE_CONTROL]:UBD uninstall app");
    s_is_debug_mode_operation = TRUE;
    APPSTORE_UninstallAPP(p_app_node);
    return APPSTORE_RES_SUCC;
}
LOCAL MMI_RESULT_E HandleAppStoreDebugModeMsg(PWND app_ptr, uint16 msg_id, DPARAM param)
{
    APPSTORE_DEBUG_MODE_SIG_T msg_body = {0};
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    APPSTORE_RES_E appstore_res = APPSTORE_RES_MAX;

    if(PNULL == param)
    {
        TRACE_APPSTORE("[APPSTORE_CONTROL]:param is PNULL");
        return MMI_RESULT_FALSE;
    }
    msg_body = *((APPSTORE_DEBUG_MODE_SIG_T*)param);
    TRACE_APPSTORE("[APPSTORE_CONTROL]:msg_id = 0x%x, msg_body:%s",msg_id,msg_body.app_name);
    switch (msg_id)
    {
        case MSG_APPSTORE_INSTALL_BY_DEBUG_MODE:
        {
            appstore_res = InstallAPPForDebugMode(msg_body.app_name);
            if(APPSTORE_RES_SUCC != appstore_res)
            {
                TRACE_APPSTORE("[APPSTORE_CONTROL]:appstore_res install error");
#ifndef WIN32
#ifdef OSA_SUPPORT
                ur_con_printf("APP install fail!");
#endif
#endif
            }
            break;
        }
        case MSG_APPSTORE_UNINSTALL_BY_DEBUG_MODE:
        {
            appstore_res = UninstallAPPForDebugMode(msg_body.app_name);
            if(APPSTORE_RES_SUCC != appstore_res)
            {
                TRACE_APPSTORE("[APPSTORE_CONTROL]:appstore_res error");
#ifndef WIN32
#ifdef OSA_SUPPORT
                ur_con_printf("APP uninstall fail!");
#endif
#endif
            }
            break;
        }
        default:
        {
            TRACE_APPSTORE("[APPSTORE_CONTROL]:message error");
            return MMI_RESULT_FALSE;
        }
    }
    return result;
}

/*****************************************************************************/
//  Description : 卸载指定APP
//  Parameter: [In] 待卸载的APP 索引信息
//             [Out] None
//             [Return] APPSTORE_RES_E
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
PUBLIC APPSTORE_RES_E APPSTORE_UninstallAPP( APPSTORE_APP_NODE_T* p_app_node )
{
    int result_js = -1;
    APPSTORE_RES_E result = APPSTORE_RES_SUCC;
    if(FALSE == APPSTORE_GetIsProcessing())// 当前不存在正在进行的操作
    {
        // 设置flag
        APPSTORE_SetIsProcessing(TRUE);

        // 查询需要进行操作的app node信息
        if(PNULL == p_app_node)
        {
            TRACE_APPSTORE("[APPSTORE_CONTROL]:the p_app_node is PNULL");
            // 设置flag
            APPSTORE_SetIsProcessing(FALSE);
            APPSTORE_SetCurNodeInfo(PNULL);
            return APPSTORE_RES_PARAM_ERR;
        }
        TRACE_APPSTORE("[APPSTORE_CONTROL]: enter, p_app_id = %s", p_app_node->p_app_id);

        APPSTORE_SetCurNodeInfo(p_app_node);

        TRACE_APPSTORE("[APPSTORE_CONTROL]: app_node_state = %d", p_app_node->app_node_state);

        if(APPSTORE_APP_STATE_UNINSTALL == p_app_node->app_node_state)
        {
            TRACE_APPSTORE("[APPSTORE_CONTROL]:the p_app_node is alreday del");
            // 设置flag
            APPSTORE_SetIsProcessing(FALSE);
            APPSTORE_SetCurNodeInfo(PNULL);

            NotifyClientAppState();
            return APPSTORE_RES_SUCC;
        }

#ifdef DYNAMIC_C_APP_SUPPORT
        result = DeleteAppFile(*p_app_node);
#endif
#if defined(JAVASCRIPT_ECO_ENVIRONMENT_SUPPORT) && defined(JS_APP_SUPPORT)
        result_js = js_app_uninstall(p_app_node->p_app_id);
        if(0 == result_js)
        {
            result=APPSTORE_RES_SUCC;
        }
        else
        {
            result=APPSTORE_RES_FAIL;
        }
#endif
        if(APPSTORE_RES_SUCC != result)
        {
            TRACE_APPSTORE("[APPSTORE_CONTROL]:result is %d",result);
            // 设置flag
            APPSTORE_SetIsProcessing(FALSE);
            APPSTORE_SetCurNodeInfo(PNULL);
            return result;
        }
#if defined(JAVASCRIPT_ECO_ENVIRONMENT_SUPPORT) && defined(JS_APP_SUPPORT)
        result = APPSTORE_Update_App_List(p_app_node,APPSTORE_INSTALLED_APP_SET_TYPE_DELETE);

        // 设置flag
        APPSTORE_SetIsProcessing(FALSE);
        APPSTORE_SetCurNodeInfo(PNULL);
        if(TRUE == s_is_debug_mode_operation)
        {
            s_is_debug_mode_operation = FALSE;
            TRACE_APPSTORE("[APPSTORE_CONTROL]:debug mode uninstall suc!");
#ifndef WIN32
#ifdef OSA_SUPPORT
            ur_con_printf("Debug APP operation success!");
#endif
#endif
        }
#endif
        TRACE_APPSTORE("[APPSTORE_CONTROL]:exit, result = %d", result);
        return result;
    }
    else // 当前存在正在进行的操作,弹出提示框" 当前正在进行app操作"
    {
        TRACE_APPSTORE("[APPSTORE_CONTROL]: open operating note win");
        APPSTORE_Operating_Note_Win_Enter();
        result = APPSTORE_RES_PARAM_ERR;
    }
    return result;
}


/*****************************************************************************/
//  Description : 更新指定APP
//  Parameter: [In] 待更新的APP 索引信息
//             [Out] None
//             [Return] APPSTORE_RES_E
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
PUBLIC APPSTORE_RES_E APPSTORE_UpdateAPP( APPSTORE_APP_NODE_T* p_app_node )
{
    int result_js = -1;
    APPSTORE_RES_E result = APPSTORE_RES_SUCC;
    if(FALSE == APPSTORE_GetIsProcessing())// 当前不存在正在进行的操作
    {
        BOOLEAN is_enough = FALSE;
        // 设置flag
        APPSTORE_SetIsProcessing(TRUE);

        // 查询需要进行操作的app node信息
        if(PNULL == p_app_node)
        {
            TRACE_APPSTORE("[APPSTORE_CONTROL]:the p_app_node is PNULL");
            // 设置flag
            APPSTORE_SetIsProcessing(FALSE);
            APPSTORE_SetCurNodeInfo(PNULL);
            return APPSTORE_RES_PARAM_ERR;
        }
        TRACE_APPSTORE("[APPSTORE_CONTROL]: enter, p_app_id = %s", p_app_node->p_app_id);  //coverity bug1941690

        APPSTORE_SetCurNodeInfo(p_app_node);

        TRACE_APPSTORE("[APPSTORE_CONTROL]: app_node_state = %d", p_app_node->app_node_state);

        if(APPSTORE_APP_STATE_NEED_UPDATE != p_app_node->app_node_state)
        {
            TRACE_APPSTORE("[APPSTORE_CONTROL]:the p_app_node is alreday update");
            // 设置flag
            APPSTORE_SetIsProcessing(FALSE);
            APPSTORE_SetCurNodeInfo(PNULL);

            NotifyClientAppState();
            return APPSTORE_RES_SUCC;
        }

        //检查空间是否充足
        APPSTORE_CheckSpace(p_app_node,&is_enough);

        if(is_enough)
        {
            /*update app node state and notify client*/
            p_app_node->app_node_state = APPSTORE_APP_STATE_UPDATING;
            NotifyClientAppState();

#ifdef DYNAMIC_C_APP_SUPPORT
            result = DeleteAppFile(*p_app_node);
#endif

#if defined(JAVASCRIPT_ECO_ENVIRONMENT_SUPPORT) && defined(JS_APP_SUPPORT)
            result_js = js_app_uninstall(p_app_node->p_app_id);
            if(0 == result_js)
            {
                result=APPSTORE_RES_SUCC;
            }
            else
            {
                result=APPSTORE_RES_FAIL;
            }
#endif

            if(APPSTORE_RES_SUCC != result)
            {
                TRACE_APPSTORE("[APPSTORE_CONTROL]:result is %d",result);
                // 设置flag
                APPSTORE_SetIsProcessing(FALSE);
                APPSTORE_SetCurNodeInfo(PNULL);
                return result;
            }
#if defined(JAVASCRIPT_ECO_ENVIRONMENT_SUPPORT) && defined(JS_APP_SUPPORT)
            result = APPSTORE_Install_JS_APP(p_app_node->p_app_id);

            if(APPSTORE_RES_SUCC != result)
            {
                TRACE_APPSTORE("[APPSTORE_CONTROL]: fail, result is %d",result);
                // 设置flag
                APPSTORE_SetIsProcessing(FALSE);
                APPSTORE_SetCurNodeInfo(PNULL);
                return result;
            }
            result = APPSTORE_Update_App_List(p_app_node,APPSTORE_INSTALLED_APP_SET_TYPE_REWRITE);

            TRACE_APPSTORE("[APPSTORE_CONTROL]:result is %d",result);
            // 设置flag
            APPSTORE_SetIsProcessing(FALSE);
            APPSTORE_SetCurNodeInfo(PNULL);
#endif
            TRACE_APPSTORE("[APPSTORE_CONTROL]:exit, result = %d", result);
            return result;
        }
        else
        {
            // 设置flag
            APPSTORE_SetIsProcessing(FALSE);
            APPSTORE_SetCurNodeInfo(PNULL);

            TRACE_APPSTORE("[APPSTORE_FLOW]: open Insufficient Space note win");
            //打开空间不足弹窗
            APPSTORE_Insufficient_Space_Note_Win_Enter();

            result = APPSTORE_RES_NO_ENOUGH_SPACE;
            TRACE_APPSTORE("[APPSTORE_CONTROL]:exit, result = %d", result);
            return result;
        }
    }
    else // 当前存在正在进行的操作,弹出提示框" 当前正在进行app操作"
    {
        TRACE_APPSTORE("[APPSTORE_CONTROL]: open operating note win");
        APPSTORE_Operating_Note_Win_Enter();
        result = APPSTORE_RES_PARAM_ERR;
    }
    return result;
}
/*****************************************************************************/
//  Description : 获取已安装APP List
//  Parameter: [In] None
//             [Out] 已安装APP List
//             [Return] APPSTORE_RES_E
//  Author: xiuyun.wang
//  Note:
/*****************************************************************************/
LOCAL APPSTORE_RES_E ConvertJsonObectToAppList(uint16* p_file_name, APPSTORE_APP_LIST_T** p_app_list)
{
    uint8* p_json_buffer = PNULL;
    APPSTORE_APP_LIST_T* p_temp_app_list = PNULL;
    SFS_HANDLE json_file_handle = NULL;
    uint32 installed_json_file_size = 0;
    uint32 read_file_size = 0;
    uint16 app_num = 0;
    uint32 i = 0;
    COMMON_CONFIG_OBJECT_T json_object_info = {0};
    APPSTORE_APP_NODE_LEN_T app_node_len = {0};
    APPSTORE_APP_NODE_T* p_app_node = PNULL;
    APPSTORE_RES_E res = APPSTORE_RES_MAX;

    if(PNULL == p_file_name)
    {
        TRACE_APPSTORE("[APPSTORE_CONTROL]:the param is error");
        return APPSTORE_RES_PARAM_ERR;
    }

    if(PNULL == p_app_list)
    {
        TRACE_APPSTORE("[APPSTORE_CONTROL]:the param is error");
        return APPSTORE_RES_PARAM_ERR;
    }

    if(PNULL != *p_app_list)
    {
        TRACE_APPSTORE("[APPSTORE_CONTROL]:the app list is not NULL, so need free");
        APPSTORE_FreeAppList(* p_app_list);
        *p_app_list = PNULL;
    }

    /*create installed app list first*/
    res = CreateAppList(&p_temp_app_list);
    if(APPSTORE_RES_SUCC != res)
    {
        TRACE_APPSTORE("[APPSTORE_CONTROL]:create app list fail");
        return APPSTORE_RES_FAIL;
    }

    /*read installed JSON file*/
    //read json file
    json_file_handle = SFS_CreateFile(p_file_name, (SFS_MODE_OPEN_EXISTING | SFS_MODE_SHARE_READ), 0, 0);
    if(json_file_handle == NULL)
    {
        TRACE_APPSTORE("[APPSTORE_CONTROL]:open installed json file fail");
        *p_app_list = p_temp_app_list;
        return APPSTORE_RES_FILE_MISSED;
    }

    //get installed json file size
    SFS_GetFileSize(json_file_handle, &installed_json_file_size);

    //malloc json buffer
    p_json_buffer = (uint8*)SCI_ALLOC_APPZ(installed_json_file_size + APP_STORE_JSON_FILE_ONE_OBJ_MAX_LEN);
    if(PNULL == p_json_buffer)
    {
        TRACE_APPSTORE("[APPSTORE_CONTROL]:malloc s_p_json_buffer fail");
        APPSTORE_FreeAppList(p_temp_app_list);
        p_temp_app_list = PNULL;
        return APPSTORE_RES_FAIL;
    }

    if(SFS_ERROR_NONE != SFS_ReadFile(json_file_handle, p_json_buffer, installed_json_file_size, &read_file_size, NULL))
    {
        TRACE_APPSTORE("[APPSTORE_CONTROL]:read json file fail");
        SFS_CloseFile(json_file_handle);

        //free buffer
        SCI_FREE(p_json_buffer);
        p_json_buffer = PNULL;
        //free app list
        APPSTORE_FreeAppList(p_temp_app_list);
        p_temp_app_list = PNULL;
        return APPSTORE_RES_FAIL;
    }

    SFS_CloseFile(json_file_handle);
    if(installed_json_file_size != read_file_size)
    {
        TRACE_APPSTORE("[APPSTORE_CONTROL]:read json file is invalid, should read %d but read %d actruely",
                            installed_json_file_size, read_file_size);
        //free buffer
        SCI_FREE(p_json_buffer);
        p_json_buffer = PNULL;
        //free app list
        APPSTORE_FreeAppList(p_temp_app_list);
        p_temp_app_list = PNULL;
        return APPSTORE_RES_FAIL;
    }

    //get the number of object
    JSONPARSER_GetConfigArrayCount(p_json_buffer,&app_num);

    //get object from JSON buffer
    for(i = 0; i< app_num; i++)
    {
        //get one object info
        SCI_MEMSET(&json_object_info, 0x00, sizeof(COMMON_CONFIG_OBJECT_T));
        JSONPARSER_GetConfigObjectInfo(p_json_buffer, i, &json_object_info);
        TRACE_APPSTORE("[APPSTORE_CONTROL]:object:%d, app id:%s, app name:%s, version code:%d, version name:%s",\
                        i, json_object_info.id, json_object_info.app_name, json_object_info.version_code, json_object_info.version_name);

        //get app node len
        SCI_MEMSET(&app_node_len, 0x00, sizeof(APPSTORE_APP_NODE_LEN_T));
        app_node_len.app_id_len = SCI_STRLEN(json_object_info.id);
        app_node_len.app_name_len = SCI_STRLEN(json_object_info.app_name);
        app_node_len.app_version_name_len = SCI_STRLEN(json_object_info.version_name);
        app_node_len.app_type_len = SCI_STRLEN(json_object_info.type);
        app_node_len.app_introduction_len = SCI_STRLEN(json_object_info.introduction);
        app_node_len.app_icon_full_name_len = SCI_STRLEN(json_object_info.icon) + SCI_STRLEN(json_object_info.url);

        //create app node and copy node
        p_app_node = PNULL;
        APPSTORE_CreateAppNode(app_node_len,&p_app_node);
        SCI_MEMCPY(p_app_node->p_app_id, json_object_info.id, app_node_len.app_id_len);
        SCI_MEMCPY(p_app_node->p_app_name, json_object_info.app_name, app_node_len.app_name_len);
        p_app_node->app_version_code = json_object_info.version_code;
        SCI_MEMCPY(p_app_node->p_app_version_name, json_object_info.version_name, app_node_len.app_version_name_len);
        SCI_MEMCPY(p_app_node->p_app_type, json_object_info.type, app_node_len.app_type_len);
        SCI_MEMCPY(p_app_node->p_app_introduction, json_object_info.introduction, app_node_len.app_introduction_len);
        p_app_node->app_size = json_object_info.size;
        sprintf(p_app_node->p_app_icon_full_name, "%s%s",  json_object_info.url, json_object_info.icon);
        TRACE_APPSTORE("[APPSTORE_CONTROL]:app node icon full name:%s",p_app_node->p_app_icon_full_name);

        //add app node to app list
        AddAppNodeToAppList(*p_app_node, &p_temp_app_list);

        APPSTORE_FreeAppNode(p_app_node);
        p_app_node = PNULL;
    }
    TRACE_APPSTORE("[APPSTORE_CONTROL]:app list number:%d",p_temp_app_list->app_num);

    *p_app_list = p_temp_app_list;

    //free JSON buffer
    SCI_FREE(p_json_buffer);
    p_json_buffer = PNULL;
    return APPSTORE_RES_SUCC;
}
/*****************************************************************************/
//  Description : 获取已安装APP List
//  Parameter: [In] None
//             [Out] 已安装APP List
//             [Return] APPSTORE_RES_E
//  Author: xiuyun.wang
//  Note:
/*****************************************************************************/
PUBLIC APPSTORE_RES_E APPSTORE_GetInstalledAppList(APPSTORE_APP_LIST_T** p_installed_app_list)
{
    APPSTORE_RES_E res = APPSTORE_RES_MAX;
    wchar apps_folder_path_name[APP_FILES_FOLDER_FULLPATH_LEN + 1] = {0};
    TRACE_APPSTORE("[APPSTORE_CONTROL]:enter");

    if(PNULL == p_installed_app_list)
    {
        TRACE_APPSTORE("[APPSTORE_CONTROL]:the param is error");
        return APPSTORE_RES_PARAM_ERR;
    }

    if(PNULL != *p_installed_app_list)
    {
        TRACE_APPSTORE("[APPSTORE_CONTROL]:the app list is not NULL, so need free");
        APPSTORE_FreeAppList(* p_installed_app_list);
        *p_installed_app_list = PNULL;
    }

    // 获取apps目标文件夹
    MMI_STRNTOWSTR(apps_folder_path_name,INSTALL_APP_FILES_FOLDER, (char *)INSTALL_APP_FILES_FOLDER,SCI_STRLEN(INSTALL_APP_FILES_FOLDER),SCI_STRLEN(INSTALL_APP_FILES_FOLDER));
    if(!MMIAPIFMM_IsFolderExist (apps_folder_path_name, MMIAPICOM_Wstrlen(apps_folder_path_name)))
    {
        TRACE_APPSTORE("[APPSTORE_CONTROL]:creat apps folder");
        if(SFS_ERROR_NONE != SFS_CreateDirectory((const uint16 *)apps_folder_path_name))
        {
            TRACE_APPSTORE("[APPSTORE_CONTROL]:creat apps folder failed");
            return APPSTORE_RES_FAIL;
        }
    }

    //convert json object to app list
    res = ConvertJsonObectToAppList((const uint16*)INSTALLED_APP_CONFIG_FILE_PATH, p_installed_app_list);
    if(APPSTORE_RES_SUCC != res)
    {
        if(APPSTORE_RES_FILE_MISSED == res)
        {
            SFS_HANDLE json_file_handle = NULL;
            TRACE_APPSTORE("[APPSTORE_CONTROL]:create installed json file");
            json_file_handle = SFS_CreateFile (INSTALLED_APP_CONFIG_FILE_PATH, SFS_MODE_WRITE | SFS_MODE_CREATE_ALWAYS, NULL, NULL);
            if(NULL == json_file_handle)
            {
                TRACE_APPSTORE("[APPSTORE_CONTROL]:creat JSON file failed");
                return APPSTORE_RES_FAIL;
            }
            else
            {
                uint32 write_size = 0;
                uint32 to_write_size = 0;

                TRACE_APPSTORE("[APPSTORE_CONTROL]:creat JSON file succeed");
                to_write_size = SCI_STRLEN(s_json_file_header_info);
                if (SFS_INVALID_HANDLE != json_file_handle)
                {
                    if (SFS_ERROR_NONE != SFS_WriteFile (json_file_handle, (const void *)s_json_file_header_info, to_write_size, &write_size, NULL))
                    {
                        TRACE_APPSTORE("[APPSTORE_CONTROL]:write JSON file failed");
                        SFS_CloseFile(json_file_handle);
                        return APPSTORE_RES_FAIL;
                    }
                }

                SFS_CloseFile(json_file_handle);
            }
        }
        else
        {
            TRACE_APPSTORE("[APPSTORE_CONTROL]:conver json object to app list fail");
            return APPSTORE_RES_FAIL;
        }
    }
    TRACE_APPSTORE("[APPSTORE_CONTROL]:installed app list number:%d",(*p_installed_app_list)->app_num);

    return APPSTORE_RES_SUCC;
}
/*****************************************************************************/
//  Description : 写入已安装JSON File
//  Parameter: [In]app_set_type: set type for JSON File
//             [In]app_node: app node for write JSON File
//             [In]app_node_index: app node index
//             [Out] NONE
//             [Return] APPSTORE_RES_E
//  Author: xiuyun.wang
//  Note:
/*****************************************************************************/
LOCAL APPSTORE_RES_E WriteInstalledJsonFile(APPSTORE_INSTALLED_APP_SET_TYPE_E app_set_type, APPSTORE_APP_NODE_T app_node, uint16 app_node_index)
{
    uint8* p_json_buffer = PNULL;
    SFS_HANDLE json_file_handle = NULL;
    uint32 installed_json_file_size = 0;
    uint32 read_file_size = 0;
    uint32 write_size = 0;
    uint32 to_write_size = 0;
    COMMON_CONFIG_OBJECT_T json_object_info = {0};
    JSONPARSER_RESULT_E json_res = JSON_PARSER_MAX;
    APPSTORE_RES_E res = APPSTORE_RES_MAX;
    BOOLEAN is_app_node_valid = CheckAppNodeIsValid(app_node);
    uint8* p_replace = PNULL;
    uint8* p_des_icon_full_name = PNULL;

    if( FALSE == is_app_node_valid)
    {
        TRACE_APPSTORE("[APPSTORE_CONTROL]:the param is error");
        return APPSTORE_RES_PARAM_ERR;
    }

    /*read installed JSON file*/
    //read json file
    json_file_handle = SFS_CreateFile((const uint16*)INSTALLED_APP_CONFIG_FILE_PATH, (SFS_MODE_OPEN_EXISTING | SFS_MODE_SHARE_READ), 0, 0);
    if(json_file_handle == NULL)
    {
        TRACE_APPSTORE("[APPSTORE_CONTROL]:open installed json file fail");
        return APPSTORE_RES_FAIL;
    }

    //get installed json file size
    SFS_GetFileSize(json_file_handle, &installed_json_file_size);

    //malloc json buffer
    p_json_buffer = (uint8*)SCI_ALLOC_APPZ(installed_json_file_size + APP_STORE_JSON_FILE_ONE_OBJ_MAX_LEN);
    if(PNULL == p_json_buffer)
    {
        TRACE_APPSTORE("[APPSTORE_CONTROL]:malloc s_p_json_buffer fail");
        return APPSTORE_RES_FAIL;
    }

    if(SFS_ERROR_NONE != SFS_ReadFile(json_file_handle, p_json_buffer, installed_json_file_size, &read_file_size, NULL))
    {
        TRACE_APPSTORE("[APPSTORE_CONTROL]:read json file fail");
        SFS_CloseFile(json_file_handle);

        //free buffer
        SCI_FREE(p_json_buffer);
        p_json_buffer = PNULL;
        return APPSTORE_RES_FAIL;
    }
    SFS_CloseFile(json_file_handle);
    if(installed_json_file_size != read_file_size)
    {
        TRACE_APPSTORE("[APPSTORE_CONTROL]:read json file is invalid, should read %d but read %d actruely",
                            installed_json_file_size, read_file_size);
        //free buffer
        SCI_FREE(p_json_buffer);
        p_json_buffer = PNULL;
        return APPSTORE_RES_FAIL;
    }

    /*convert to common json object*/
    //app id
    SCI_MEMSET(json_object_info.id, 0x00, sizeof(json_object_info.id));
    SCI_MEMCPY(json_object_info.id, app_node.p_app_id,SCI_STRLEN(app_node.p_app_id));

    //app name
    SCI_MEMSET(json_object_info.app_name, 0x00, sizeof(json_object_info.app_name));
    SCI_MEMCPY(json_object_info.app_name, app_node.p_app_name,SCI_STRLEN(app_node.p_app_name));

    //app version code
    json_object_info.version_code = app_node.app_version_code;

    //app version name
    SCI_MEMSET(json_object_info.version_name, 0x00, sizeof(json_object_info.version_name));
    if(PNULL != app_node.p_app_version_name)
    {
        SCI_MEMCPY(json_object_info.version_name, app_node.p_app_version_name, SCI_STRLEN(app_node.p_app_version_name));
    }
    //app type
    SCI_MEMSET(json_object_info.type, 0x00, sizeof(json_object_info.type));
    if(PNULL != app_node.p_app_type)
    {
        SCI_MEMCPY(json_object_info.type, app_node.p_app_type, SCI_STRLEN(app_node.p_app_type));
    }
    //app introduction
    SCI_MEMSET(json_object_info.introduction, 0x00, sizeof(json_object_info.introduction));
    if(PNULL != app_node.p_app_introduction)
    {
        SCI_MEMCPY(json_object_info.introduction, app_node.p_app_introduction, SCI_STRLEN(app_node.p_app_introduction));
    }
    //app icon full name
    SCI_MEMSET(json_object_info.icon, 0x00, sizeof(json_object_info.icon));
    TRACE_APPSTORE("[APPSTORE_CONTROL]:app_node.p_app_icon_full_name:%s",app_node.p_app_icon_full_name);
    p_replace = app_node.p_app_icon_full_name + SCI_STRLEN(SERVER_APP_FILES_FOLDER) + SCI_STRLEN(FILE_FOLDER_BACKSLASH);
    TRACE_APPSTORE("[APPSTORE_CONTROL]:p_replace:%s",p_replace);
    SCI_MEMCPY(json_object_info.icon, p_replace, SCI_STRLEN(p_replace));

    //app url
    SCI_MEMSET(json_object_info.url, 0x00, sizeof(json_object_info.url));
//#ifdef DYNAMIC_C_APP_SUPPORT
    sprintf(json_object_info.url, "%s%s", INSTALL_APP_FILES_FOLDER,FILE_FOLDER_BACKSLASH);
//#endif

//#if defined(JAVASCRIPT_ECO_ENVIRONMENT_SUPPORT) && defined(JS_APP_SUPPORT)
//   sprintf(json_object_info.url, "%s%s", INSTALL_JS_APP_FILES_FOLDER,FILE_FOLDER_BACKSLASH);
//#endif

    //app size
    json_object_info.size= app_node.app_size;

    /*handle json file with set type*/
    TRACE_APPSTORE("[APPSTORE_CONTROL]:set json file type:%d",app_set_type);
    switch(app_set_type)
    {
        case APPSTORE_INSTALLED_APP_SET_TYPE_ADD:
        {
            json_res =JSONPARSER_AddConfigObject(p_json_buffer, &json_object_info);
            break;
        }
        case APPSTORE_INSTALLED_APP_SET_TYPE_DELETE:
        {
            json_res = JSONPARSER_DeleteConfigObject(p_json_buffer, app_node_index);
            break;
        }
        case APPSTORE_INSTALLED_APP_SET_TYPE_REWRITE:
        {
            json_res = JSONPARSER_UpdateConfigObject(p_json_buffer, app_node_index, json_object_info);
            break;
        }
        default:
        {
            break;
        }
    }
    TRACE_APPSTORE("[APPSTORE_CONTROL]:CONVERT json success");

    /*write JSON file*/
    to_write_size = SCI_STRLEN(p_json_buffer);
    json_file_handle = SFS_CreateFile ((const uint16*)INSTALLED_APP_CONFIG_FILE_PATH, SFS_MODE_WRITE | SFS_MODE_CREATE_ALWAYS, NULL, NULL);/*lint !e655*/

    if (SFS_INVALID_HANDLE != json_file_handle)
    {
        if (SFS_ERROR_NONE == SFS_WriteFile (json_file_handle, (const void *) p_json_buffer, to_write_size, &write_size, NULL))
        {
            res = APPSTORE_RES_SUCC;
        }
    }
    SFS_CloseFile (json_file_handle);

    TRACE_APPSTORE("[APPSTORE_CONTROL]:set json file res:%d",json_res);
    if(JSON_PARSER_SUCCESS != json_res)
    {
        res = APPSTORE_RES_FAIL;
    }
    else
    {
        res = APPSTORE_RES_SUCC;
    }

    //free buffer
    SCI_FREE(p_json_buffer);
    p_json_buffer = PNULL;
    return res;

}
/*****************************************************************************/
//  Description :设置指定APP Node到已安装APP List
//  Parameter: [In]app_node_info: 待设置app node info
//             [In]app_set_type:待设置的类型
//             [Out] None
//             [Return] APPSTORE_RES_E
//  Author: xiuyun.wang
//  Note:
/*****************************************************************************/
PUBLIC APPSTORE_RES_E APPSTORE_SetInstalledAppList( APPSTORE_APP_NODE_T app_node_info, APPSTORE_INSTALLED_APP_SET_TYPE_E app_set_type)
{
    APPSTORE_RES_E res = APPSTORE_RES_MAX;
    APPSTORE_APP_LIST_T* p_temp_server_list = PNULL;
    APPSTORE_APP_LIST_T* p_temp_full_list = PNULL;
    APPSTORE_APP_NODE_T* p_temp_app_node = PNULL;
    APPSTORE_APP_NODE_T* p_need_set_app_node = {0};
    uint32 i = 0;
    int32 cmp_ret = 0;
    BOOLEAN is_app_node_valid = CheckAppNodeIsValid(app_node_info);
    uint16 app_index = 0;

    if((FALSE == is_app_node_valid) || (app_set_type >= APPSTORE_INSTALLED_APP_SET_TYPE_MAX))
    {
        TRACE_APPSTORE("[APPSTORE_CONTROL]:the param is error");
        return APPSTORE_RES_PARAM_ERR;
    }
    TRACE_APPSTORE("[APPSTORE_CONTROL]:installing app node id:%s", app_node_info.p_app_id);
    TRACE_APPSTORE("[APPSTORE_CONTROL]:installing app node icon:%s", app_node_info.p_app_icon_full_name);
    p_need_set_app_node = &app_node_info;
    if(TRUE == s_is_debug_mode_operation)
    {
        if(APPSTORE_INSTALLED_APP_SET_TYPE_DELETE == app_set_type)
        {
            //rewrite JSON file
            p_temp_full_list = APPSTORE_GetFullAppList();

            APPSTORE_GetAppIndexByAppId(p_need_set_app_node->p_app_id,*p_temp_full_list,&app_index);
            TRACE_APPSTORE("[APPSTORE_CONTROL]:debug mode get index from full list:%d", app_index);
        }
    }
    else
    {
        switch(app_set_type)
        {
            case APPSTORE_INSTALLED_APP_SET_TYPE_ADD:
            case APPSTORE_INSTALLED_APP_SET_TYPE_REWRITE:
            {
                TRACE_APPSTORE("[APPSTORE_CONTROL]:need update app node info with server app list");
                res = APPSTORE_GetServerAppList(&p_temp_server_list);
                if(APPSTORE_RES_SUCC != res || PNULL == p_temp_server_list)
                {
                    TRACE_APPSTORE("[APPSTORE_CONTROL]:get server app list fail, so can not update app node info");
                    return APPSTORE_RES_FAIL;
                }

                //查找对应的app node from server app list
                for(i = 0; i < p_temp_server_list->app_num; i++)
                {
                    p_temp_app_node = PNULL;
                    APPSTORE_GetAppNodeFromAppList(i,*p_temp_server_list,&p_temp_app_node);
                    if(PNULL == p_temp_app_node)
                    {
                        TRACE_APPSTORE("[APPSTORE_CONTROL]:get server app node is null, so can not update app node info");
                        APPSTORE_FreeAppList(p_temp_server_list);
                        p_temp_server_list = PNULL;
                        return APPSTORE_RES_FAIL;
                    }
                    else
                    {
                        cmp_ret = MMIAPICOM_StrCmp(p_temp_app_node->p_app_id,SCI_STRLEN(p_temp_app_node->p_app_id),\
                                            app_node_info.p_app_id,SCI_STRLEN(app_node_info.p_app_id));
                        if(0 == cmp_ret)//找到app node
                        {
                            TRACE_APPSTORE("[APPSTORE_CONTROL]:find out the app node from server app list, replace this app node");
                            p_need_set_app_node = p_temp_app_node;
                            TRACE_APPSTORE("[APPSTORE_CONTROL]:p_need_set_app_node,appid:%s",p_need_set_app_node->p_app_id);
                            break;
                        }
                    }
                }
                break;
            }
            default:
            {
                break;
            }
        }
        //rewrite JSON file
        p_temp_full_list = APPSTORE_GetFullAppList();

        APPSTORE_GetAppIndexByAppId(p_need_set_app_node->p_app_id,*p_temp_full_list,&app_index);
        TRACE_APPSTORE("[APPSTORE_CONTROL]:get index from full list:%d", app_index);
    }
    res = WriteInstalledJsonFile(app_set_type, *p_need_set_app_node, app_index);
    TRACE_APPSTORE("[APPSTORE_CONTROL]:write installed APP JSON File res:%d", res);
    if(PNULL != p_temp_server_list)
    {
        APPSTORE_FreeAppList(p_temp_server_list);
        p_temp_server_list = PNULL;
    }
    return res;
}
/*****************************************************************************/
//  Description : 获取模拟server端APP List
//  Parameter: [In] None
//             [Out] server端APP List
//             [Return] APPSTORE_RES_E
//  Author: xiuyun.wang
//  Note:
/*****************************************************************************/
PUBLIC APPSTORE_RES_E APPSTORE_GetServerAppList(APPSTORE_APP_LIST_T** p_server_app_list)
{
    APPSTORE_RES_E res = APPSTORE_RES_MAX;
    TRACE_APPSTORE("[APPSTORE_CONTROL]:enter");

    if(PNULL == p_server_app_list)
    {
        TRACE_APPSTORE("[APPSTORE_CONTROL]:the param is error");
        return APPSTORE_RES_PARAM_ERR;
    }
    if(PNULL != *p_server_app_list)
    {
        TRACE_APPSTORE("[APPSTORE_CONTROL]:the app list is not NULL, so need free");
        APPSTORE_FreeAppList(*p_server_app_list);
        *p_server_app_list = PNULL;
    }

    //convert json object to app list
    res = ConvertJsonObectToAppList((const uint16*)SERVER_APP_CONFIG_FILE_PATH, p_server_app_list);
    if(APPSTORE_RES_SUCC != res)
    {
        TRACE_APPSTORE("[APPSTORE_CONTROL]:conver json object to app list fail");
        APPSTORE_FreeAppList(*p_server_app_list);
        *p_server_app_list = PNULL;
    }
    else
    {
        TRACE_APPSTORE("[APPSTORE_CONTROL]:server app list number:%d",(*p_server_app_list)->app_num);
    }
    return res;
}

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
                                                                 APPSTORE_APP_LIST_T** p_full_app_list)
{
    uint32 app_total_num = 0;               //记录生成链表的node个数
    APPSTORE_RES_E result = APPSTORE_RES_MAX;
    APPSTORE_APP_LIST_T* p_full_list = PNULL;
    APPSTORE_APP_NODE_T* p_install_app_node = PNULL;// 记录将被对比的当前node节点
    APPSTORE_APP_NODE_T* p_new_node = PNULL;// 记录最终生成链表的最后一个node节点

    TRACE_APPSTORE("[APPSTORE_CONTROL]:enter");
    // 判断入参指针是否为空
    if((PNULL == p_server_app_list)||(PNULL == p_installed_app_list))
    {
        TRACE_APPSTORE("[APPSTORE_CONTROL]:p_server_app_list or p_installed_app_list is PNULL");
        return APPSTORE_RES_PARAM_ERR;
    }

    if(PNULL == p_full_app_list)
    {
        TRACE_APPSTORE("[APPSTORE_CONTROL]:p_full_app_list is PNULL");
        return APPSTORE_RES_PARAM_ERR;
    }

    if(PNULL != *p_full_app_list)
    {
        TRACE_APPSTORE("[APPSTORE_CONTROL]:the app list is not NULL, so need free");
        APPSTORE_FreeAppList(*p_full_app_list);
    }

    // 为 p_full_list 分配空间
    result = CreateAppList(&p_full_list);
    if(APPSTORE_RES_SUCC != result)
    {
        TRACE_APPSTORE("CreateAppList error! result = %d",result);
        return result;
    }
    *p_full_app_list = p_full_list;


    //确定 p_install_app_node 初始位置
    p_install_app_node = p_installed_app_list->p_app_node_header;

    while(PNULL != p_install_app_node)
    {
        BOOLEAN is_find = FALSE;
        APPSTORE_APP_NODE_T* p_server_node = PNULL;     // 记录对比的当前node节点
        APPSTORE_APP_NODE_T* p_pre_server_node = PNULL; // 记录对比的前一个node节点

        //确定 p_temp_node 初始位置
        p_server_node = p_server_app_list->p_app_node_header;
        while(PNULL != p_server_node)
        {
            //判断 APP ID 是否相同
            //TRUE: 根据条件修改节点信息并释放p_server_app_list中与p_installed_app_list相同APP的节点信息
            //FALSE: p_server_node指向下一节点,继续查询,直至查询到或指向链表尾部
            uint16 install_app_id_len = (uint16)SCI_STRLEN(p_install_app_node->p_app_id);
            uint16 server_app_id_len = (uint16)SCI_STRLEN(p_server_node->p_app_id);
            uint32 cmp_ret =MMIAPICOM_StrCmp(p_install_app_node->p_app_id ,
                                             install_app_id_len ,
                                             p_server_node->p_app_id ,
                                             server_app_id_len );
            if(0 == cmp_ret)//str1 == str2, cmp_ret == 0;
            {
                TRACE_APPSTORE("[APPSTORE_CONTROL]:find the same app node");
                is_find = TRUE;

                //判断 APP 版本号 是否相同
                if(p_install_app_node->app_version_code >= p_server_node->app_version_code)
                {
                    p_install_app_node->app_node_state = APPSTORE_APP_STATE_INSTALLED;
                }
                else
                {
                    p_install_app_node->app_node_state = APPSTORE_APP_STATE_NEED_UPDATE;
                }

                TRACE_APPSTORE("[APPSTORE_CONTROL]:p_app_id = %s",p_install_app_node->p_app_id);
                TRACE_APPSTORE("[APPSTORE_CONTROL]:app_node_state = %d",p_install_app_node->app_node_state);
                //判断当前节点是否为header节点
                if(p_server_node == p_server_app_list->p_app_node_header)
                {
                    TRACE_APPSTORE("[APPSTORE_CONTROL]:current find node is server node header");
                    p_server_app_list->p_app_node_header = p_server_node->p_app_node_next;
                }
                else
                {
                    p_pre_server_node->p_app_node_next = p_server_node->p_app_node_next;
                }

                //释放p_server_app_list中与p_installed_app_list相同APP的节点信息
                APPSTORE_FreeAppNode(p_server_node);
                p_server_node = PNULL;
                break;
            }

            TRACE_APPSTORE("[APPSTORE_CONTROL]:to the next app node");
            p_pre_server_node = p_server_node;
            p_server_node = p_server_node->p_app_node_next;
        }

        //链表查询结束,判断是否查到匹配信息
        TRACE_APPSTORE("[APPSTORE_CONTROL]:is_find = %d",is_find);
        if(!is_find)
        {
            p_install_app_node->app_node_state = APPSTORE_APP_STATE_OFFLINE;
            TRACE_APPSTORE("[APPSTORE_CONTROL]:p_app_id = %s",p_install_app_node->p_app_id);
            TRACE_APPSTORE("[APPSTORE_CONTROL]:app_node_state = %d",p_install_app_node->app_node_state);
        }

        //判断生成链表是否尚未添加node
        //TRUE:将p_full_list->p_app_node_header指向查询到的节点信息,并通过p_new_node记录最新节点信息
        //FALSE:将p_new_node的下一指针指向查询到的节点信息,并通过p_new_node记录最新节点信息
        if(PNULL == p_full_list->p_app_node_header)
        {
            p_full_list->p_app_node_header = p_install_app_node;
            p_new_node = p_full_list->p_app_node_header;
        }
        else
        {
            if(PNULL != p_new_node)  //coverity bug1941690
            {
                p_new_node->p_app_node_next = p_install_app_node;
                p_new_node = p_new_node->p_app_node_next;
            }
        }

        //p_install_app_node指向下一节点
        p_install_app_node = p_install_app_node->p_app_node_next;
        //生成链表的总数 + 1
        app_total_num++;

    }

    //判断p_server_app_list的header是否为空
    if(PNULL != p_server_app_list->p_app_node_header)
    {
        //判断当前是否存在已安装的app
        if(0 == p_installed_app_list->app_num)
        {
            p_full_list->p_app_node_header = p_server_app_list->p_app_node_header;
            p_new_node = p_full_list->p_app_node_header;
        }
        else
        {
            if(PNULL != p_new_node)  //coverity bug1962664
            {
                p_new_node->p_app_node_next = p_server_app_list->p_app_node_header;
                p_new_node = p_new_node->p_app_node_next;
            }
        }
        while(PNULL != p_new_node)
        {
            TRACE_APPSTORE("[APPSTORE_CONTROL]:p_app_id = %s",p_new_node->p_app_id);
            TRACE_APPSTORE("[APPSTORE_CONTROL]:app_node_state = %d",p_new_node->app_node_state);
            p_new_node->app_node_state = APPSTORE_APP_STATE_UNINSTALL;
            p_new_node = p_new_node->p_app_node_next;
            app_total_num++;
        }
    }

    //给p_full_list->app_num赋值
    TRACE_APPSTORE("[APPSTORE_CONTROL]:app_total_num = %d",app_total_num);
    p_full_list->app_num = app_total_num;

    //将p_installed_app_list和p_server_app_list的header地址指向置为空并释放链表
    p_installed_app_list->p_app_node_header = PNULL;
    p_server_app_list->p_app_node_header = PNULL;

    TRACE_APPSTORE("[APPSTORE_CONTROL]:exit, result = %d", result);
    return result;
}

/*****************************************************************************/
//  Description :获取is_processing
//  Parameter: [In] is_processing
//             [Out] None
//             [Return] None
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
PUBLIC void APPSTORE_SetIsProcessing( BOOLEAN is_processing )
{
    TRACE_APPSTORE("[APPSTORE_CONTROL]is_processing = %d",is_processing);
    s_is_processing = is_processing;
    return;
}

/*****************************************************************************/
//  Description :获取is_processing
//  Parameter: [In] None
//             [Out] None
//             [Return] is_processing
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN APPSTORE_GetIsProcessing( void )
{
    TRACE_APPSTORE("[APPSTORE_CONTROL]s_is_processing = %d",s_is_processing);
    return s_is_processing;
}

/*****************************************************************************/
//  Description :设置current node info
//  Parameter: [In] s_p_cur_node_info
//             [Out] None
//             [Return] None
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
PUBLIC void APPSTORE_SetCurNodeInfo( APPSTORE_APP_NODE_T* p_app_node )
{
    TRACE_APPSTORE("[APPSTORE_CONTROL]p_app_node = %x",p_app_node);
    s_p_cur_node_info = p_app_node;
    return;
}

/*****************************************************************************/
//  Description :获取current node info
//  Parameter: [In] None
//             [Out] None
//             [Return] s_p_cur_node_info
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
PUBLIC APPSTORE_APP_NODE_T* APPSTORE_GetCurNodeInfo( void )
{
    if(PNULL != s_p_cur_node_info)
    {
        TRACE_APPSTORE("[APPSTORE_CONTROL]s_cur_node app id = %s",s_p_cur_node_info->p_app_id);
    }
    return s_p_cur_node_info;
}

/*****************************************************************************/
//  Description :获取s_p_full_app_list
//  Parameter: [In] None
//             [Out] None
//             [Return] s_p_full_app_list
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
PUBLIC APPSTORE_APP_LIST_T* APPSTORE_GetFullAppList( void )
{
    TRACE_APPSTORE("[APPSTORE_CONTROL]: enter");
    if(PNULL ==s_p_full_app_list)
    {
        TRACE_APPSTORE("[APPSTORE_CONTROL]:s_p_full_app_list is PNULL");
        return PNULL;
    }
    return (const APPSTORE_APP_LIST_T*)s_p_full_app_list;
}

/*****************************************************************************/
//  Description : get full app list and set data list.
//  Parameter: [In] None
//             [Out] None
//             [Return] APPSTORE_RES_E
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
PUBLIC APPSTORE_RES_E APPSTORE_UpdateFullAppListForData( void )
{
    APPSTORE_RES_E result = APPSTORE_RES_MAX;
    APPSTORE_APP_LIST_T* p_server_app_list      = PNULL;
    APPSTORE_APP_LIST_T* p_installed_app_list   = PNULL;

    TRACE_APPSTORE("[APPSTORE_CONTROL]: enter");
    // get full app list and set data list.
    result = APPSTORE_GetServerAppList(&p_server_app_list);
    if((APPSTORE_RES_SUCC != result)&&(APPSTORE_RES_FILE_MISSED != result))
    {
        TRACE_APPSTORE("[APPSTORE_CONTROL]APPSTORE_GetServerAppList error! result = %d",result);
        return result;
    }
    if(PNULL != p_server_app_list)
    {
        TRACE_APPSTORE("[APPSTORE_CONTROL]: p_server_app_list->app_num = %d",p_server_app_list->app_num);
    }

    result = APPSTORE_GetInstalledAppList(&p_installed_app_list);
    if(APPSTORE_RES_SUCC != result)
    {
        APPSTORE_FreeAppList(p_server_app_list);
        TRACE_APPSTORE("[APPSTORE_CONTROL]APPSTORE_GetInstalledAppList error! result = %d",result);
        return result;
    }
    if(PNULL != p_installed_app_list)
    {
        TRACE_APPSTORE("[APPSTORE_CONTROL]: p_installed_app_list->app_num = %d",p_installed_app_list->app_num);
    }

    result = APPSTORE_GenerateFullAppList(p_server_app_list,p_installed_app_list,&s_p_full_app_list);
    if(APPSTORE_RES_SUCC == result)
    {
        TRACE_APPSTORE("[APPSTORE_CONTROL]: s_p_full_app_list->app_num = %d",s_p_full_app_list->app_num);
        NotifyClientAppState();
    }
    // free mem
    APPSTORE_FreeAppList(p_server_app_list);
    APPSTORE_FreeAppList(p_installed_app_list);

    TRACE_APPSTORE("[APPSTORE_CONTROL]: exit");
    return result;
}

/*****************************************************************************/
//  Description : APP Store初始化
//  Parameter: [In] None
//             [Out] None
//             [Return] APPSTORE_RES_E
//  Author: xiuyun.wang
//  Note:
/*****************************************************************************/
PUBLIC APPSTORE_RES_E APPSTORE_Init(void)
{
    uint8 i = 0;
    APPSTORE_RES_E  res = APPSTORE_RES_SUCC;
#ifdef THEME_UNZIP_SUPPORT
    //register unzip callback
    g_appstore_unzip.ProcessMsg = HandleAppStoreZipMsg;
#endif
    g_appstore_debug_mode.ProcessMsg = HandleAppStoreDebugModeMsg;
    //s_appstore_client init
    for(i = 0; i < APPSTORE_CLIENT_MAX_NUM; i++)
    {
        s_appstore_client[i] = 0;
    }

    //get full app list firt
    res = APPSTORE_UpdateFullAppListForData();
    TRACE_APPSTORE("[APPSTORE_CONTROL]: get app full list result:%d", res);

    //init app store bg module
    APPSTORE_BG_RegisterBLE();
    
    return res;
}

/*****************************************************************************/
//  Description : 注册client函数
//  Parameter: [In] appstore control的client的callback函数
//             [Out] None
//             [Return] APPSTORE_RES_E
//  Author: xiuyun.wang
//  Note:
/*****************************************************************************/
PUBLIC uint32 APPSTORE_AddClient(_appstore_client_callback client_callback)
{
    uint8 i = 0;
    TRACE_APPSTORE("[APPSTORE_CONTROL]: enter");

    if(PNULL == client_callback)
    {
        TRACE_APPSTORE("[APPSTORE_CONTROL]: client_callback is null");
        return PNULL;
    }
    TRACE_APPSTORE("[APPSTORE_CONTROL]: client_callback:%x", client_callback);

    for(i = 0; i < APPSTORE_CLIENT_MAX_NUM; i++)
    {
        if(PNULL == s_appstore_client[i])
        {
            s_appstore_client[i] = client_callback;
            return s_appstore_client[i];
        }
    }

    return PNULL;
}

/*****************************************************************************/
//  Description : 去注册client函数
//  Parameter: [In] appstore control的client的callback函数
//             [Out] None
//             [Return] APPSTORE_RES_E
//  Author: xiuyun.wang
//  Note:
/*****************************************************************************/
PUBLIC void APPSTORE_DeleteClient(uint32 appstore_handle)
{
    uint32 i  = 0;
    TRACE_APPSTORE("[APPSTORE_CONTROL]: appstore_handle:%x", appstore_handle);

    //防止全局数组写穿
    for(i = 0; i < APPSTORE_CLIENT_MAX_NUM; i++)
    {
        if(appstore_handle ==  s_appstore_client[i])
        {
            s_appstore_client[i] = PNULL;
            return;
        }
    }
    TRACE_APPSTORE("[APPSTORE_CONTROL]: invalid handle");
}


/*****************************************************************************/
//  Description : use app id get app node from app list,需要使用完成后进行释放此APP Node
//  Parameter: [In]  p_app_id: 想要获取的app node对应的id
//             [In]  app_list: the app list
//             [Out] p_app_node:获取的app node
//             [Return] add result
//  Author:donglin.su
//  Note:
/*****************************************************************************/
PUBLIC APPSTORE_RES_E APPSTORE_GetAppNodeByAppId(uint8* p_app_id, APPSTORE_APP_LIST_T app_list,
                                                                   APPSTORE_APP_NODE_T** p_app_node)
{
    APPSTORE_APP_NODE_T*  p_temp_app_node = PNULL;

    if(PNULL == p_app_node)
    {
        TRACE_APPSTORE("[APPSTORE_CONTROL]:the param is error");
        return APPSTORE_RES_PARAM_ERR;
    }
    if(PNULL != *p_app_node)
    {
        TRACE_APPSTORE("[APPSTORE_CONTROL]:param is error");
        return APPSTORE_RES_PARAM_ERR;
    }
    if((PNULL == p_app_id)||(0 == SCI_STRLEN(p_app_id)))
    {
        TRACE_APPSTORE("[APPSTORE_CONTROL]:p_app_id is error");
        return APPSTORE_RES_PARAM_ERR;
    }

    //get app node from app list
    p_temp_app_node = app_list.p_app_node_header;
    if((PNULL == p_temp_app_node)||(PNULL == p_temp_app_node->p_app_id))
    {
        TRACE_APPSTORE("[APPSTORE_CONTROL]:param is error.");
        return APPSTORE_RES_PARAM_ERR;
    }
    while(PNULL != p_temp_app_node)
    {
        if(0 == SCI_MEMCMP(p_app_id, p_temp_app_node->p_app_id,SCI_STRLEN(p_app_id)))
        {
            TRACE_APPSTORE("[APPSTORE_CONTROL]:destination app node :%x, app id:%s", p_temp_app_node,p_temp_app_node->p_app_id);
            *p_app_node = p_temp_app_node;
            break;
        }
        p_temp_app_node = p_temp_app_node->p_app_node_next;
    }

    if(PNULL == p_temp_app_node)
    {
        TRACE_APPSTORE("[APPSTORE_CONTROL]:get app mode is PNULL");
        return APPSTORE_RES_FAIL;
    }

    return APPSTORE_RES_SUCC;
}

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
                                                                            uint16* p_app_index)
{
    APPSTORE_APP_NODE_T*  p_temp_app_node = PNULL;
    uint32 cmp_ret = 0;

    if(PNULL == p_app_id)
    {
        TRACE_APPSTORE("[APPSTORE_CONTROL]:the param is error");
        return APPSTORE_RES_PARAM_ERR;
    }

    if(PNULL == p_app_index)
    {
        TRACE_APPSTORE("[APPSTORE_CONTROL]:the param is error");
        return APPSTORE_RES_PARAM_ERR;
    }

    //get app node from app list
    p_temp_app_node = app_list.p_app_node_header;
    if((PNULL == p_temp_app_node)||(PNULL == p_temp_app_node->p_app_id))
    {
        TRACE_APPSTORE("[APPSTORE_CONTROL]:param is error");
        return APPSTORE_RES_PARAM_ERR;
    }
    while(PNULL != p_temp_app_node)
    {
        uint32 cmp_ret = MMIAPICOM_StrCmp(p_app_id,
                                         SCI_STRLEN(p_app_id),
                                         p_temp_app_node->p_app_id,
                                         SCI_STRLEN(p_temp_app_node->p_app_id));
        if(0 == cmp_ret)
        {
            TRACE_APPSTORE("[APPSTORE_CONTROL]:app index:%d",*p_app_index);
            break;
        }
        p_temp_app_node = p_temp_app_node->p_app_node_next;
        (*p_app_index)++;
    }

    if(PNULL == p_temp_app_node)
    {
        TRACE_APPSTORE("[APPSTORE_CONTROL]:get app index fail");
        return APPSTORE_RES_FAIL;
    }

    return APPSTORE_RES_SUCC;
}

/*****************************************************************************/
//  Description : Check the Space of disk C
//  Parameter: [In] p_app_node
//             [Out] is_enough
//             [Return] APPSTORE_RES_E
//  Author: mingwei.jia
//  Note:
/*****************************************************************************/
PUBLIC APPSTORE_RES_E APPSTORE_CheckSpace(APPSTORE_APP_NODE_T* p_app_node,
                                                     BOOLEAN* p_is_enough)
{
    APPSTORE_RES_E result = APPSTORE_RES_MAX;

    //D盘
    MMIFILE_DEVICE_E dev = MMI_DEVICE_UDISK;

    //判断APP当前所处的状态
    switch(p_app_node->app_node_state)
    {
        //若需要更新
        case APPSTORE_APP_STATE_NEED_UPDATE:
        {
            APPSTORE_APP_LIST_T* p_server_app_list = PNULL;
            APPSTORE_APP_NODE_T* p_server_app_node = PNULL;

            //获取Server端的App List
            result = APPSTORE_GetServerAppList(&p_server_app_list);

            if(APPSTORE_RES_SUCC != result)
            {
                TRACE_APPSTORE("[APPSTORE_CONTROL]APPSTORE_GetServerAppList error! result = %d",result);
                return result;
            }

            TRACE_APPSTORE("[APPSTORE_CONTROL]: p_server_app_list->app_num = %d",p_server_app_list->app_num);

            //通过APP的ID获取该APP的信息
            APPSTORE_GetAppNodeByAppId(p_app_node->p_app_id,
                                       *p_server_app_list,
                                       &p_server_app_node);

            if(PNULL == p_server_app_node)
            {
                TRACE_APPSTORE("[APPSTORE_CONTROL]:get server app node is null");
                APPSTORE_FreeAppList(p_server_app_list);
                p_server_app_list = PNULL;
                return APPSTORE_RES_FAIL;
            }

            //判断C盘剩余空间是否可以完成下载
            if(p_server_app_node->app_size > p_app_node->app_size)
            {

                //记录更细前后大小的差值
                uint32 deviation = 0;

                deviation = p_server_app_node->app_size - p_app_node->app_size;

                *p_is_enough = MMIAPIFMM_IsDevEnoughSpace (deviation + RESERVED_SPACE_100KB,dev);
            }
            else
            {
                *p_is_enough = TRUE;
            }
            TRACE_APPSTORE("[APPSTORE_FLOW]:is_enough = %d",*p_is_enough);

            //释放链表空间
            APPSTORE_FreeAppList(p_server_app_list);
            p_server_app_list = PNULL;

            break;
        }

        //若需要下载
        case APPSTORE_APP_STATE_UNINSTALL:
        {
            *p_is_enough = MMIAPIFMM_IsDevEnoughSpace (p_app_node->app_size + RESERVED_SPACE_100KB,dev);
            TRACE_APPSTORE("[APPSTORE_FLOW]:is_enough = %d",*p_is_enough);
            break;
        }

        default:
        {
            result = APPSTORE_RES_PARAM_ERR;
            break;
        }

    }

    result = APPSTORE_RES_SUCC;
    return result;
}

/*****************************************************************************/
//  Description : 获取预置APP List
//  Parameter: [In] None
//             [Out] 预置APP List
//             [Return] APPSTORE_RES_E
//  Author: mingwei.jia
//  Note:
/*****************************************************************************/
PUBLIC APPSTORE_RES_E APPSTORE_GetPreInstalledAppList(APPSTORE_APP_LIST_T** p_pre_installed_app_list)
{
    APPSTORE_RES_E res = APPSTORE_RES_MAX;
    TRACE_APPSTORE("[APPSTORE_CONTROL]:enter");

    if(PNULL == p_pre_installed_app_list)
    {
        TRACE_APPSTORE("[APPSTORE_CONTROL]:the param is error");
        return APPSTORE_RES_PARAM_ERR;
    }
    if(PNULL != *p_pre_installed_app_list)
    {
        TRACE_APPSTORE("[APPSTORE_CONTROL]:the app list is not NULL, so need free");
        APPSTORE_FreeAppList(*p_pre_installed_app_list);
        *p_pre_installed_app_list = PNULL;
    }

    //convert json object to app list
    res = ConvertJsonObectToAppList((const uint16*)PRE_INSTALLED_APP_CONFIG_FILE_PATH, p_pre_installed_app_list);
    if(APPSTORE_RES_SUCC != res)
    {
        TRACE_APPSTORE("[APPSTORE_CONTROL]:conver json object to app list fail");
        APPSTORE_FreeAppList(*p_pre_installed_app_list);
        *p_pre_installed_app_list = PNULL;
    }
    else
    {
        TRACE_APPSTORE("[APPSTORE_CONTROL]:Pre Installed app list number:%d",(*p_pre_installed_app_list)->app_num);
    }
    return res;
}

/*****************************************************************************/
//  Description : Generate pre installed and installed App List
//  Parameter: [In] p_pre_installed_app_list: List of pre installed App
//             [In] p_installed_app_list: List of installed app
//             [Out] p_full_app_list: full App List
//             [Return] APPSTORE_RES_E
//  Author: mingwei.jia
//  Note: 将预置的和已安装APP列表整合为一个列表
/*****************************************************************************/
PUBLIC APPSTORE_RES_E APPSTORE_FullInstalledAppList( APPSTORE_APP_LIST_T** p_full_app_list )
{
    APPSTORE_RES_E result = APPSTORE_RES_MAX;
    APPSTORE_APP_LIST_T* p_full_list = PNULL;
    APPSTORE_APP_NODE_T* p_temp_app_node = PNULL;         // 记录当前node节点
    APPSTORE_APP_LIST_T* p_installed_app_list = PNULL;//已安装app list
    APPSTORE_APP_LIST_T* p_pre_installed_app_list = PNULL;//preinstalled app list

    if(PNULL == p_full_app_list)
    {
        TRACE_APPSTORE("[APPSTORE_CONTROL]:p_full_app_list is PNULL");
        return APPSTORE_RES_PARAM_ERR;
    }

    if(PNULL != *p_full_app_list)
    {
        TRACE_APPSTORE("[APPSTORE_CONTROL]:the app list is not NULL, so need free");
        APPSTORE_FreeAppList(*p_full_app_list);
        *p_full_app_list = PNULL;
    }

    // 为 p_full_list 分配空间
    result = CreateAppList(&p_full_list);
    if(APPSTORE_RES_SUCC != result)
    {
        TRACE_APPSTORE("[APPSTORE_CONTROL]:CreateAppList error! result = %d",result);
        return result;
    }
    *p_full_app_list = p_full_list;

    //preinstalled app list
    APPSTORE_GetPreInstalledAppList(&p_pre_installed_app_list);
    if(PNULL == p_pre_installed_app_list)
    {
        SCI_TRACE_LOW("[APPSTORE_CONTROL]:Get pre Installed AppList error");
    }
    else if(0 != p_pre_installed_app_list->app_num)
    {
        TRACE_APPSTORE("[APPSTORE_CONTROL]:p_pre_installed_app_list->app_num = %d",p_pre_installed_app_list->app_num);
        p_full_list->p_app_node_header = p_pre_installed_app_list->p_app_node_header;
        p_full_list->app_num = p_pre_installed_app_list->app_num;
        p_pre_installed_app_list->p_app_node_header = PNULL;
    }

    //installed app list
    APPSTORE_GetInstalledAppList(&p_installed_app_list);
    if(PNULL == p_installed_app_list)
    {
        TRACE_APPSTORE("[APPSTORE_CONTROL]:Get Installed AppList error");
    }
    else if(0 != p_installed_app_list->app_num)
    {
        TRACE_APPSTORE("[APPSTORE_CONTROL]:p_installed_app_list->app_num = %d",p_installed_app_list->app_num);
        if(0 == p_full_list->app_num)
        {
            p_full_list->p_app_node_header = p_installed_app_list->p_app_node_header;
        }
        else
        {
            //找到链表的尾部
            p_temp_app_node = p_full_list->p_app_node_header;
            while(PNULL != p_temp_app_node->p_app_node_next)
            {
                p_temp_app_node = p_temp_app_node->p_app_node_next;
            }
            p_temp_app_node->p_app_node_next = p_installed_app_list->p_app_node_header;
        }
        //计算APP列表总数
        p_full_list->app_num += p_installed_app_list->app_num;
        TRACE_APPSTORE("[APPSTORE_CONTROL]:p_full_list->app_num = %d",p_full_list->app_num);
        p_installed_app_list->p_app_node_header = PNULL;
    }

    APPSTORE_FreeAppList(p_installed_app_list);
    APPSTORE_FreeAppList(p_pre_installed_app_list);
    TRACE_APPSTORE("[APPSTORE_CONTROL]:exit, result = %d", result); 
    return result;
}
