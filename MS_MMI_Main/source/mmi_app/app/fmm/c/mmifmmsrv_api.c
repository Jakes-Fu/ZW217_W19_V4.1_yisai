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
//  Description : ��ָ��Ŀ¼�������ļ�
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
//  Description : ��MMIFMM_SearchFileInPath���ʹ��
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
//  Description : ��MMIFMM_SearchFileInPath���ʹ��
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
//  Description : ��MMIFMM_SearchFileInPath���ʹ��
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
            //�ȴ�filetask ִ�к�õ���ǰִ�е�handle
            handle = APPFILE_GetCurrentFindHandle();

        } while(PNULL == handle);
    }
    if (PNULL != handle)
    {
        MMIFMM_SearchFileStop(handle);
    }
}
/*****************************************************************************/
//  Description : ��ָ��Ŀ¼�������ļ�
//  Return: 
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
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
//  Description : ��ָ��Ŀ¼�������ļ�ͬʱ����
//  Return: 
//  Global resource dependence : 
//  Author: yaye.jiang
//  Note:
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
//  Description :��ָ��Ŀ¼�������ļ�ͬʱָ����������
//  Return: 
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
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
//  Description : ��ָ��Ŀ¼�������ļ�
//  Return: 
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
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
//  Description : ��ָ��Ŀ¼�������ļ�������search_ptr���ļ�
//  Return: 
//  Global resource dependence : 
//  Author:
//  Note:
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
//  Description : ��ָ��Ŀ¼�������ļ�
//  Return: 
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:��������������
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