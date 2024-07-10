/*****************************************************************************
** File Name:      mmifmmsrv_api.h                                           *
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

#ifndef _MMIFMMSRV_API_H_
#define _MMIFMMSRV_API_H_


/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmifmm_srv.h"
#include "mmk_app.h"

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         TYPE DEFINITION                                  *
 **--------------------------------------------------------------------------*/

/**-------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/
#define MMIAPIFMM_CreateSrv()            MMIFMM_CreateSrv(_D_FILE_NAME, _D_FILE_LINE)
#define MMIAPIFMM_DestroySrv(HANDLE)     MMIFMM_DestroySrv(HANDLE, _D_FILE_NAME, _D_FILE_LINE)

/*****************************************************************************/
//  Description : 在指定目录下按照指定规则搜索文件
//  Global resource dependence : 
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFMM_SearchFile( 
                       MMIFMM_SRV_HANDLE handle,   //搜索handle
                       MMIFMM_SEARCH_FILE_INFO_T *search_file_info
                       );

/*****************************************************************************/
//  Description : 在指定目录下搜索文件
//  Return: 
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:此接口将作废，后续将用MMIAPIFMM_SearchFile替换
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
                            );

/*****************************************************************************/
//  Description : 在指定目录下搜索文件同时排序
//  Return: 
//  Global resource dependence : 
//  Author: yaye.jiang
//  Note:此接口将作废，后续将用MMIAPIFMM_SearchFile替换
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
                            );


/*****************************************************************************/
//  Description :在指定目录下搜索文件同时指定排序类型
//  Return: 
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:此接口将作废，后续将用MMIAPIFMM_SearchFile替换
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
                            );

/*****************************************************************************/
//  Description : 在指定目录下搜索文件
//  Return: 
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:此接口将作废，后续将用MMIAPIFMM_SearchFile替换
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
                            );

/*****************************************************************************/
//  Description : 在指定目录下搜索文件名包含search_ptr的文件
//  Return: 
//  Global resource dependence : 
//  Author:
//  Note:此接口将作废，后续将用MMIAPIFMM_SearchFile替换
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
                            );
/*****************************************************************************/
//  Description : 在指定目录下搜索文件
//  Return: 
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:搜索，但不排序，此接口将作废，后续将用MMIAPIFMM_SearchFile替换
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
                            );
/*****************************************************************************/
//  Description : 和MMIFMM_SearchFileInPath配合使用
//  Return: 
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIFMM_SearchFileCallBack(MMIFMM_SRV_HANDLE handle);

/*****************************************************************************/
//  Description : 和MMIFMM_SearchFileInPath配合使用
//  Return: 
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
// PUBLIC void MMIAPIFMM_SearchFileStopExt(MMIFMM_SRV_HANDLE handle);

/*****************************************************************************/
//  Description : 和MMIFMM_SearchFileInPath配合使用
//  Return: 
//  Global resource dependence : 
//  Author: 
/*****************************************************************************/
PUBLIC void MMIAPIFMM_SearchFileStop(void);
#endif