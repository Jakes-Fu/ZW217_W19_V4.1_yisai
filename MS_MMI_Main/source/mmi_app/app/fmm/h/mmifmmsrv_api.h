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
//  Description : ��ָ��Ŀ¼�°���ָ�����������ļ�
//  Global resource dependence : 
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFMM_SearchFile( 
                       MMIFMM_SRV_HANDLE handle,   //����handle
                       MMIFMM_SEARCH_FILE_INFO_T *search_file_info
                       );

/*****************************************************************************/
//  Description : ��ָ��Ŀ¼�������ļ�
//  Return: 
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:�˽ӿڽ����ϣ���������MMIAPIFMM_SearchFile�滻
/*****************************************************************************/
void MMIAPIFMM_SearchFileInPath(
                            const wchar         *path_ptr,      //in, ����Ŀ¼���������NULL�����������д洢��
                            uint16              path_len,
                            MMIFMM_FILTER_T     *filter_ptr,    //ɸѡ����
                            BOOLEAN             is_recurse,     //�����Ƿ�ݹ�
                            MMIFMM_FUNC_TYPE_E  func_type,      //����
                            FILEARRAY           file_array,     //out, ���
                            MMI_WIN_ID_T        win_id,         //���win_id��msg_id��Ϊ0�������ͬ����ʽ�����ļ�
                            MMI_MESSAGE_ID_E    msg_id
                            );

/*****************************************************************************/
//  Description : ��ָ��Ŀ¼�������ļ�ͬʱ����
//  Return: 
//  Global resource dependence : 
//  Author: yaye.jiang
//  Note:�˽ӿڽ����ϣ���������MMIAPIFMM_SearchFile�滻
/*****************************************************************************/
PUBLIC void MMIAPIFMM_SearchFileInPathAndSort(
                            const wchar             *path_ptr,      //in, ����Ŀ¼���������NULL�����������д洢��
                            uint16              path_len,
                            MMIFMM_FILTER_T     *filter_ptr,    //ɸѡ����
                            BOOLEAN             is_recurse,     //�����Ƿ�ݹ�
                            MMIFMM_FUNC_TYPE_E  func_type,      //����
                            FILEARRAY           file_array,     //out, ���
                            MMI_WIN_ID_T        win_id,         //���win_id��msg_id��Ϊ0�������ͬ����ʽ�����ļ�
                            MMI_MESSAGE_ID_E    msg_id,
                            FILEARRAY_SORT_E    sort_type       //��������
                            );


/*****************************************************************************/
//  Description :��ָ��Ŀ¼�������ļ�ͬʱָ����������
//  Return: 
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:�˽ӿڽ����ϣ���������MMIAPIFMM_SearchFile�滻
/*****************************************************************************/
void MMIAPIFMM_SearchFileInPathBySort(
                            const wchar         *path_ptr,      //in, ����Ŀ¼���������NULL�����������д洢��
                            uint16              path_len,
                            MMIFMM_FILTER_T     *filter_ptr,    //ɸѡ����
                            BOOLEAN             is_recurse,     //�����Ƿ�ݹ�
                            MMIFMM_FUNC_TYPE_E  func_type,      //����
                            FILEARRAY_SORT_E    sort_type,      //��������
                            FILEARRAY           file_array,     //out, ���
                            MMI_WIN_ID_T        win_id,         //���win_id��msg_id��Ϊ0�������ͬ����ʽ�����ļ�
                            MMI_MESSAGE_ID_E    msg_id
                            );

/*****************************************************************************/
//  Description : ��ָ��Ŀ¼�������ļ�
//  Return: 
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:�˽ӿڽ����ϣ���������MMIAPIFMM_SearchFile�滻
/*****************************************************************************/
void MMIAPIFMM_SearchFileInPathEx(
                            const wchar        *path_ptr,      //in, ����Ŀ¼���������NULL�����������д洢��
                            uint16             path_len,
							const wchar        *default_path_ptr,//Ԥ��·��
                            MMIFMM_FILTER_T     *filter_ptr,    //ɸѡ����
                            BOOLEAN             is_recurse,     //�����Ƿ�ݹ�
                            MMIFMM_FUNC_TYPE_E  func_type,      //����
                            FILEARRAY_SORT_E    sort_type,      //��������
                            FILEARRAY           file_array,     //out, ���
                            MMI_WIN_ID_T        win_id,         //���win_id��msg_id��Ϊ0�������ͬ����ʽ�����ļ�
                            MMI_MESSAGE_ID_E    msg_id
                            );

/*****************************************************************************/
//  Description : ��ָ��Ŀ¼�������ļ�������search_ptr���ļ�
//  Return: 
//  Global resource dependence : 
//  Author:
//  Note:�˽ӿڽ����ϣ���������MMIAPIFMM_SearchFile�滻
/*****************************************************************************/
void MMIFMM_SearchFileInPathByString(
                            const wchar         *path_ptr,      //in, ����Ŀ¼���������NULL�����������д洢��
                            uint16              path_len,       
                            const wchar         *search_str,    //�����ַ���
                            BOOLEAN             is_recurse,     //�����Ƿ�ݹ�
							FILEARRAY_SORT_E    sort_type,      //��������
                            MMIFMM_FUNC_TYPE_E  func_type,      //����
                            FILEARRAY           file_array,     //out, ���
                            MMI_WIN_ID_T        win_id,         //���win_id��msg_id��Ϊ0�������ͬ����ʽ�����ļ�
                            MMI_MESSAGE_ID_E    msg_id
                            );
/*****************************************************************************/
//  Description : ��ָ��Ŀ¼�������ļ�
//  Return: 
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:�������������򣬴˽ӿڽ����ϣ���������MMIAPIFMM_SearchFile�滻
/*****************************************************************************/
void MMIFMM_SearchFileInPathNoSort(
                            const wchar             *path_ptr,      //in, ����Ŀ¼���������NULL�����������д洢��
                            uint16              path_len,
                            MMIFMM_FILTER_T     *filter_ptr,    //ɸѡ����
                            BOOLEAN             is_recurse,     //�����Ƿ�ݹ�
                            MMIFMM_FUNC_TYPE_E  func_type,      //����
                            FILEARRAY           file_array,     //out, ���
                            MMI_WIN_ID_T        win_id,         //���win_id��msg_id��Ϊ0�������ͬ����ʽ�����ļ�
                            MMI_MESSAGE_ID_E    msg_id
                            );
/*****************************************************************************/
//  Description : ��MMIFMM_SearchFileInPath���ʹ��
//  Return: 
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIFMM_SearchFileCallBack(MMIFMM_SRV_HANDLE handle);

/*****************************************************************************/
//  Description : ��MMIFMM_SearchFileInPath���ʹ��
//  Return: 
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
// PUBLIC void MMIAPIFMM_SearchFileStopExt(MMIFMM_SRV_HANDLE handle);

/*****************************************************************************/
//  Description : ��MMIFMM_SearchFileInPath���ʹ��
//  Return: 
//  Global resource dependence : 
//  Author: 
/*****************************************************************************/
PUBLIC void MMIAPIFMM_SearchFileStop(void);
#endif