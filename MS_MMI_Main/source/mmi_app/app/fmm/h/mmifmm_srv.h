/*****************************************************************************
** File Name:      mmifmm_srv.h                                              *
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

#ifndef _MMIFMM_SRV_H_
#define _MMIFMM_SRV_H_


/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "mmifilearray_export.h"
#include "mmisrvfmm_export.h"
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/

#define FMM_FILTER_STRING_MAXLEN            120//240

//文件搜索方式
typedef enum
{
    SearchFileInPath,
    SearchFileInPathEx,
    SearchFileInPathByString,
    SearchFileInPathNoSort,
    SearchFileInPathBySort,
    SearchFileInPathAndSort,
} MMIFMM_SEARCHFILE_TYPE_E;

//typedef enum
//{
//    FUNC_FIND_NULL,
//
//    FUNC_FIND_FILE,                 //搜索文件
//    FUNC_FIND_FOLDER,               //搜索文件夹
//    FUNC_FIND_FILE_AND_FOLDER,      //搜索文件或文件夹
//    FUNC_FIND_FILE_AND_ALLFOLER,    //搜索文件和所有文件夹
//    FUNC_FIND_FOLDERS_WITH_SUBFILES  //搜索文件和包含文件的文件夹，并且文件夹要和它的文件相关联
//} MMIFMM_FUNC_TYPE_E;
// 
typedef void*   MMIFMM_SRV_HANDLE;

typedef void (*SearchFileCallBack) (MMIFMM_SRV_HANDLE search_handle, FILEARRAY   folder_array);


//typedef struct
//{
//    //用于筛选的关键字符串, 如"*.mp3"; 并且可以有多个关键字符串，当中用空格隔开, 如"*.mp3 *.mid *.wav"
//    //"*" 表示列出所有文件或文件夹
//    uint8   filter_str[FMM_FILTER_STRING_MAXLEN+2];
// } MMIFMM_FILTER_T;

/**--------------------------------------------------------------------------*
 **                         TYPE DEFINITION                                  *
 **--------------------------------------------------------------------------*/
typedef struct
{
    wchar                       *path_ptr;      //<IN>:搜索目录名，如果是NULL，则搜索所有存储器
    uint16                      path_len;       //<IN>:搜索目录路径
    wchar                       *default_path;  //<IN>:预设路径
    MMIFMM_FILTER_T             *filter_ptr;    //<IN>:筛选参数
    wchar                       *search_str;    //[IN]:查找字符串
    FILEARRAY                   file_array;     //<IN>:传入的应用filearray
    MMIFMM_FUNC_TYPE_E          func_type;      //<IN>:类型
    FILEARRAY_SORT_E            sort_type;      //<IN>:排序方式
    MMIFMM_SEARCHFILE_TYPE_E    search_type;    //<IN>:搜索类型
    BOOLEAN                     is_recurse;     //<IN>:是否需要递归搜索
    MMI_WIN_ID_T                win_id;         //[IN]:可选如果search_cbk为PNULL，则发消息处理，
    MMI_MESSAGE_ID_E            msg_id;         //[IN]:如果win_id和msg_id都为0，则采用同步方式搜索文件
    SearchFileCallBack          search_cbk;     //[IN]:search file callback,Maybe PNULL.
    MMIVIRTUALARRAY_COMPARE     compare_func;   //[IN]:user common compare func,Maybe PNULL.
#if defined MMIFMM_XFIND_SUPPORT
    SFS_XFIND_GETKEY            xfind_getKey;	    //[IN]: xfind get key callback,Maybe PNULL.
    SFS_XFIND_COMPARE           xfind_compareFunc; //[IN]: xfind compare func,Maybe PNULL.
#endif
} MMIFMM_SEARCH_FILE_INFO_T;

/**-------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : create search service
//  Global resource dependence : none
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC MMIFMM_SRV_HANDLE MMIFMM_CreateSrv (
                                            const char   *file,
                                            uint32       line
                                            );

/*****************************************************************************/
//  Description : destroy search service
//  Global resource dependence : none
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC void MMIFMM_DestroySrv (
                               MMIFMM_SRV_HANDLE handle,
                               const char   *file,
                               uint32       line
                               );

/*****************************************************************************/
//  Description : search file service
//  Global resource dependence : none
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIFMM_SearchFileSrv (
    MMIFMM_SRV_HANDLE handle,
    MMIFMM_SEARCH_FILE_INFO_T *search_file_info
);

/*****************************************************************************/
//  Description : 和MMIFMM_SearchFileInPath配合使用
//  Return:
//  Global resource dependence :
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIFMM_SearchFileInTask (
    MMIFMM_SRV_HANDLE handle,
    BOOLEAN    *is_stop_ptr
);

/*****************************************************************************/
//  Description : 和MMIFMM_SearchFileInPath配合使用
//  Return:
//  Global resource dependence :
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
PUBLIC void MMIFMM_SearchFileCallBack (MMIFMM_SRV_HANDLE handle);

/*****************************************************************************/
//  Description : 和MMIFMM_SearchFileInPath配合使用
//  Return:
//  Global resource dependence :
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
PUBLIC void MMIFMM_SearchFileStop (MMIFMM_SRV_HANDLE handle);

/*****************************************************************************/
//  Description : check search file stopped or not
//  Global resource dependence :
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIFMM_IsSearchingStopped (void);
#endif