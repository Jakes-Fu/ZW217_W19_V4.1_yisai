/*****************************************************************************
** File Name:      mmifmmsrv_api.c                                           *
** Author:                                                                   *
** Date:           07/2012                                                   *
** Copyright:      2012 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe file manager module         *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 07/2012        jian.ma          Create                                    *
******************************************************************************/

#define _MMIFMMSRV_API_C_


/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmifmmsrv_api.h"
#include "mmi_filetask.h"
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         TYPE DEFINITION                                  *
 **--------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/            

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : default search file call back
//  Return: 
//  Global resource dependence : 
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL void DefaultSearchFileCallBack (MMIFMM_SRV_HANDLE search_handle,FILEARRAY folder_array);

/*****************************************************************************/
//  Description : 在指定目录下搜索文件
//  Return: 
//  Global resource dependence : 
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFMM_SearchFile( 
                       MMIFMM_SRV_HANDLE handle, 
                       MMIFMM_SEARCH_FILE_INFO_T *search_file_info
                       )
{
    return MMIFMM_SearchFileSrv(handle,search_file_info);
}
/*****************************************************************************/
//  Description : 和MMIFMM_SearchFileInPath配合使用
//  Return: 
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIFMM_SearchFileCallBack(MMIFMM_SRV_HANDLE handle)
{
    MMIFMM_SearchFileCallBack(handle);
}

/*****************************************************************************/
//  Description : 和MMIFMM_SearchFileInPath配合使用
//  Return: 
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
//PUBLIC void MMIAPIFMM_SearchFileStopExt(MMIFMM_SRV_HANDLE handle)
//{
//    MMIFMM_SearchFileStop(handle);
// }

/*****************************************************************************/
//  Description : 和MMIFMM_SearchFileInPath配合使用
//  Return: 
//  Global resource dependence : 
//  Author: 
/*****************************************************************************/
PUBLIC void MMIAPIFMM_SearchFileStop(void)
{
    MMIFMM_SRV_HANDLE handle = APPFILE_GetCurrentFindHandle();
    if (PNULL == handle)
    {
        do {
        	if (!APPFILE_IsFindingFile())
            {
                break;
            }
            SCI_Sleep (10);
            //等待filetask 执行后得到当前执行的handle
            handle = APPFILE_GetCurrentFindHandle();

        } while(PNULL == handle);
    }
    if (PNULL != handle)
    {
        MMIFMM_SearchFileStop(handle);
    }
}
/*****************************************************************************/
//  Description : 在指定目录下搜索文件
//  Return: 
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
void MMIAPIFMM_SearchFileInPath(
                            const wchar         *path_ptr,      //in, 搜索目录名，如果是NULL，则搜索所有存储器
                            uint16              path_len,
                            MMIFMM_FILTER_T     *filter_ptr,    //筛选参数
                            BOOLEAN             is_recurse,     //搜索是否递归
                            MMIFMM_FUNC_TYPE_E  func_type,      //类型
                            FILEARRAY           file_array,     //out, 输出
                            MMI_WIN_ID_T        win_id,         //如果win_id和msg_id都为0，则采用同步方式搜索文件
                            MMI_MESSAGE_ID_E    msg_id
                            )
{
    MMIFMM_SEARCH_FILE_INFO_T search_file_info = {0};
    MMIFMM_SRV_HANDLE handle = MMIAPIFMM_CreateSrv();
    search_file_info.path_ptr = path_ptr;
    search_file_info.path_len = path_len;
    search_file_info.file_array = file_array;
    search_file_info.filter_ptr = filter_ptr;
    search_file_info.is_recurse = is_recurse;
    search_file_info.sort_type  = FILEARRAY_SORT_TYPE_ASCEND;
    search_file_info.win_id     = win_id;
    search_file_info.msg_id     = msg_id;
    search_file_info.func_type = func_type;
    search_file_info.search_type = SearchFileInPath;
    search_file_info.search_cbk = DefaultSearchFileCallBack;

    MMIFMM_SearchFileSrv(handle,&search_file_info);
}

/*****************************************************************************/
//  Description : 在指定目录下搜索文件同时排序
//  Return: 
//  Global resource dependence : 
//  Author: yaye.jiang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIFMM_SearchFileInPathAndSort(
                            const wchar             *path_ptr,      //in, 搜索目录名，如果是NULL，则搜索所有存储器
                            uint16              path_len,
                            MMIFMM_FILTER_T     *filter_ptr,    //筛选参数
                            BOOLEAN             is_recurse,     //搜索是否递归
                            MMIFMM_FUNC_TYPE_E  func_type,      //类型
                            FILEARRAY           file_array,     //out, 输出
                            MMI_WIN_ID_T        win_id,         //如果win_id和msg_id都为0，则采用同步方式搜索文件
                            MMI_MESSAGE_ID_E    msg_id,
                            FILEARRAY_SORT_E    sort_type       //排序类型
                            )
{
    MMIFMM_SEARCH_FILE_INFO_T search_file_info = {0};
    MMIFMM_SRV_HANDLE handle = MMIAPIFMM_CreateSrv();

    search_file_info.path_ptr = path_ptr;
    search_file_info.path_len = path_len;
    search_file_info.file_array = file_array;
    search_file_info.filter_ptr = filter_ptr;
    search_file_info.is_recurse = is_recurse;
    search_file_info.sort_type  = sort_type;
    search_file_info.win_id     = win_id;
    search_file_info.msg_id     = msg_id;
    search_file_info.func_type = func_type; 
    search_file_info.search_type = SearchFileInPathAndSort;
    search_file_info.search_cbk = DefaultSearchFileCallBack;
    
    MMIFMM_SearchFileSrv(handle,&search_file_info);
}


/*****************************************************************************/
//  Description :在指定目录下搜索文件同时指定排序类型
//  Return: 
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
void MMIAPIFMM_SearchFileInPathBySort(
                            const wchar         *path_ptr,      //in, 搜索目录名，如果是NULL，则搜索所有存储器
                            uint16              path_len,
                            MMIFMM_FILTER_T     *filter_ptr,    //筛选参数
                            BOOLEAN             is_recurse,     //搜索是否递归
                            MMIFMM_FUNC_TYPE_E  func_type,      //类型
                            FILEARRAY_SORT_E    sort_type,      //排序类型
                            FILEARRAY           file_array,     //out, 输出
                            MMI_WIN_ID_T        win_id,         //如果win_id和msg_id都为0，则采用同步方式搜索文件
                            MMI_MESSAGE_ID_E    msg_id
                            )
{
    MMIFMM_SEARCH_FILE_INFO_T search_file_info = {0};
    MMIFMM_SRV_HANDLE handle = MMIAPIFMM_CreateSrv();

    search_file_info.path_ptr   = path_ptr;
    search_file_info.path_len   = path_len;
    search_file_info.filter_ptr = filter_ptr;
    search_file_info.is_recurse = is_recurse;
    search_file_info.func_type  = func_type; 
    search_file_info.sort_type  = sort_type;
    search_file_info.file_array = file_array;
    search_file_info.win_id     = win_id;
    search_file_info.msg_id     = msg_id;
    search_file_info.search_type = SearchFileInPathBySort;
    search_file_info.search_cbk = DefaultSearchFileCallBack;
    
    MMIFMM_SearchFileSrv(handle,&search_file_info);
}

/*****************************************************************************/
//  Description : 在指定目录下搜索文件
//  Return: 
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
void MMIAPIFMM_SearchFileInPathEx(
                            const wchar        *path_ptr,      //in, 搜索目录名，如果是NULL，则搜索所有存储器
                            uint16             path_len,
							const wchar        *default_path_ptr,//预设路径
                            MMIFMM_FILTER_T     *filter_ptr,    //筛选参数
                            BOOLEAN             is_recurse,     //搜索是否递归
                            MMIFMM_FUNC_TYPE_E  func_type,      //类型
                            FILEARRAY_SORT_E    sort_type,      //排序类型
                            FILEARRAY           file_array,     //out, 输出
                            MMI_WIN_ID_T        win_id,         //如果win_id和msg_id都为0，则采用同步方式搜索文件
                            MMI_MESSAGE_ID_E    msg_id
                            )
{
    MMIFMM_SEARCH_FILE_INFO_T search_file_info = {0};
    MMIFMM_SRV_HANDLE handle = MMIAPIFMM_CreateSrv();
    search_file_info.path_ptr = path_ptr;
    search_file_info.path_len = path_len;
    search_file_info.default_path = default_path_ptr;
    search_file_info.file_array = file_array;
    search_file_info.filter_ptr = filter_ptr;
    search_file_info.is_recurse = is_recurse;
    search_file_info.sort_type  = sort_type;
    search_file_info.win_id     = win_id;
    search_file_info.msg_id     = msg_id;
    search_file_info.func_type = func_type; 
    search_file_info.search_type = SearchFileInPathEx;
    search_file_info.search_cbk = DefaultSearchFileCallBack;

    MMIFMM_SearchFileSrv(handle,&search_file_info);
}


/*****************************************************************************/
//  Description : 在指定目录下搜索文件名包含search_ptr的文件
//  Return: 
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
void MMIFMM_SearchFileInPathByString(
                            const wchar         *path_ptr,      //in, 搜索目录名，如果是NULL，则搜索所有存储器
                            uint16              path_len,       
                            const wchar         *search_str,    //查找字符串
                            BOOLEAN             is_recurse,     //搜索是否递归
							FILEARRAY_SORT_E    sort_type,      //排序类型
                            MMIFMM_FUNC_TYPE_E  func_type,      //类型
                            FILEARRAY           file_array,     //out, 输出
                            MMI_WIN_ID_T        win_id,         //如果win_id和msg_id都为0，则采用同步方式搜索文件
                            MMI_MESSAGE_ID_E    msg_id
                            )
{
    MMIFMM_SEARCH_FILE_INFO_T search_file_info = {0};
    MMIFMM_SRV_HANDLE handle = MMIAPIFMM_CreateSrv();

    search_file_info.path_ptr = path_ptr;
    search_file_info.path_len = path_len;
    search_file_info.file_array = file_array;
    search_file_info.is_recurse = is_recurse;
    search_file_info.sort_type  = sort_type;
    search_file_info.win_id     = win_id;
    search_file_info.msg_id     = msg_id;
    search_file_info.func_type = func_type; 
    search_file_info.search_str = search_str;
    search_file_info.search_type = SearchFileInPathByString;
    search_file_info.search_cbk = DefaultSearchFileCallBack;

    MMK_CheckAllocatedMemInfo( _D_FILE_NAME, _D_FILE_LINE );
    MMIFMM_SearchFileSrv(handle,&search_file_info);
    MMK_CheckAllocatedMemInfo( _D_FILE_NAME, _D_FILE_LINE );
    
    //SearchFile(&search_file_info);
}
/*****************************************************************************/
//  Description : 在指定目录下搜索文件
//  Return: 
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:搜索，但不排序
/*****************************************************************************/
void MMIFMM_SearchFileInPathNoSort(
                            const wchar             *path_ptr,      //in, 搜索目录名，如果是NULL，则搜索所有存储器
                            uint16              path_len,
                            MMIFMM_FILTER_T     *filter_ptr,    //筛选参数
                            BOOLEAN             is_recurse,     //搜索是否递归
                            MMIFMM_FUNC_TYPE_E  func_type,      //类型
                            FILEARRAY           file_array,     //out, 输出
                            MMI_WIN_ID_T        win_id,         //如果win_id和msg_id都为0，则采用同步方式搜索文件
                            MMI_MESSAGE_ID_E    msg_id
                            )
{
    MMIFMM_SEARCH_FILE_INFO_T search_file_info = {0};
    MMIFMM_SRV_HANDLE handle = MMIAPIFMM_CreateSrv();

    search_file_info.path_ptr = path_ptr;
    search_file_info.path_len = path_len;
    search_file_info.file_array = file_array;
    search_file_info.filter_ptr = filter_ptr;
    search_file_info.is_recurse = is_recurse;
    search_file_info.sort_type  = FILEARRAY_SORT_NULL;
    search_file_info.win_id     = win_id;
    search_file_info.msg_id     = msg_id;
    search_file_info.func_type = func_type; 
    search_file_info.search_type = SearchFileInPathNoSort;
    search_file_info.search_cbk = DefaultSearchFileCallBack;

    MMIFMM_SearchFileSrv(handle,&search_file_info);
}
/*****************************************************************************/
//  Description : default search file call back
//  Return: 
//  Global resource dependence : 
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL void DefaultSearchFileCallBack (MMIFMM_SRV_HANDLE search_handle,FILEARRAY folder_array)
{
    MMIAPIFMM_DestroySrv(search_handle);
}