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

//�ļ�������ʽ
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
//    FUNC_FIND_FILE,                 //�����ļ�
//    FUNC_FIND_FOLDER,               //�����ļ���
//    FUNC_FIND_FILE_AND_FOLDER,      //�����ļ����ļ���
//    FUNC_FIND_FILE_AND_ALLFOLER,    //�����ļ��������ļ���
//    FUNC_FIND_FOLDERS_WITH_SUBFILES  //�����ļ��Ͱ����ļ����ļ��У������ļ���Ҫ�������ļ������
//} MMIFMM_FUNC_TYPE_E;
// 
typedef void*   MMIFMM_SRV_HANDLE;

typedef void (*SearchFileCallBack) (MMIFMM_SRV_HANDLE search_handle, FILEARRAY   folder_array);


//typedef struct
//{
//    //����ɸѡ�Ĺؼ��ַ���, ��"*.mp3"; ���ҿ����ж���ؼ��ַ����������ÿո����, ��"*.mp3 *.mid *.wav"
//    //"*" ��ʾ�г������ļ����ļ���
//    uint8   filter_str[FMM_FILTER_STRING_MAXLEN+2];
// } MMIFMM_FILTER_T;

/**--------------------------------------------------------------------------*
 **                         TYPE DEFINITION                                  *
 **--------------------------------------------------------------------------*/
typedef struct
{
    wchar                       *path_ptr;      //<IN>:����Ŀ¼���������NULL�����������д洢��
    uint16                      path_len;       //<IN>:����Ŀ¼·��
    wchar                       *default_path;  //<IN>:Ԥ��·��
    MMIFMM_FILTER_T             *filter_ptr;    //<IN>:ɸѡ����
    wchar                       *search_str;    //[IN]:�����ַ���
    FILEARRAY                   file_array;     //<IN>:�����Ӧ��filearray
    MMIFMM_FUNC_TYPE_E          func_type;      //<IN>:����
    FILEARRAY_SORT_E            sort_type;      //<IN>:����ʽ
    MMIFMM_SEARCHFILE_TYPE_E    search_type;    //<IN>:��������
    BOOLEAN                     is_recurse;     //<IN>:�Ƿ���Ҫ�ݹ�����
    MMI_WIN_ID_T                win_id;         //[IN]:��ѡ���search_cbkΪPNULL������Ϣ����
    MMI_MESSAGE_ID_E            msg_id;         //[IN]:���win_id��msg_id��Ϊ0�������ͬ����ʽ�����ļ�
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
//  Description : ��MMIFMM_SearchFileInPath���ʹ��
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
//  Description : ��MMIFMM_SearchFileInPath���ʹ��
//  Return:
//  Global resource dependence :
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
PUBLIC void MMIFMM_SearchFileCallBack (MMIFMM_SRV_HANDLE handle);

/*****************************************************************************/
//  Description : ��MMIFMM_SearchFileInPath���ʹ��
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