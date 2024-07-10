/*****************************************************************************
** File Name:      mmifmm_srv.c                                              *
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

#define _MMIFMM_DETAIL_C_


/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmifmm_srv.h"
#include "mmi_app_fmm_trc.h"
#include "mmi_filetask.h"
#include "mmifmm_comfunc.h"
#include "mmifmm_internal.h"
#ifdef DRM_SUPPORT
#include "mmidrm_export.h"
#endif
#include "mmi_appmsg.h"
#include "mmifmm_app.h"
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/

#if defined MMIFMM_XFIND_SUPPORT
#define MMIFMM_XFIND_CTLSTR_MAX_LEN 40
#endif
/**--------------------------------------------------------------------------*
 **                         TYPE DEFINITION                                  *
 **--------------------------------------------------------------------------*/
//�����ļ�ʱ�Ļص���������
typedef void (*FindAllFiles_CallBack) (
    MMIFMM_SRV_HANDLE handle,
    uint16      *find_path,
    uint16      find_path_len,
    FILEARRAY   folder_array,
    SFS_FIND_DATA_T *find_data
);

typedef struct MMIFMM_XFIND_TAG
{
    wchar               find_path[FMM_SEARCH_FILENAME_MAXLEN];   //xfind path
    wchar               *filter;      //xfind filter eg *.mp3
    FILEARRAY_SORT_E    sort_type;    //xfind sort type
    MMIFMM_FUNC_TYPE_E  func_type;    //xfind type
    BOOLEAN             is_recurse;   //is recurrence
    SFS_HANDLE          find_handle;  //xfind handle
    SFS_XFIND_GETKEY    getKey_cbk;	  //xfind get key
    SFS_XFIND_COMPARE   compareFunc_cbk;//xfind compare func
} MMIFMM_XFIND_T;

typedef struct MMIFMM_XFIND_INFO_TAG
{
    wchar               find_path[FMM_SEARCH_FILENAME_MAXLEN];    //xfind path
    uint16              find_path_len;  //xfind path len
    uint32              file_num;       //xfind find file num
    uint32              folder_num;     //xfind find folder num
    FILEARRAY           folder_array ;  //xfind file array
    FILEARRAY           file_array ;    //xfind file array
    SFS_HANDLE          find_handle;    //xfind find handle
    uint32              find_state;     //xfind state
    MMI_WIN_ID_T        cbk_win_id;     //xfind cbk win id
    MMIFMM_FUNC_TYPE_E  func_type;      //xfind type
    FindAllFiles_CallBack callback;     //xfind callback
} MMIFMM_XFIND_INFO_T;

typedef struct
{
    MMIFMM_FILTER_T     s_filter;         //�����ؼ���
    wchar               s_default_path[FMM_SEARCH_FILENAME_MAXLEN+1];
    wchar               s_filter_str[FMM_FILTER_STRING_MAXLEN];
    wchar               s_search_str[FMM_FILTER_STRING_MAXLEN + 1];
    wchar               s_cur_path[FMM_SEARCH_FILENAME_MAXLEN+1]; //����Ŀ¼
    uint16              s_cur_path_len;
    BOOLEAN             s_is_recurse;           //�Ƿ�ݹ�����
    BOOLEAN             s_is_sent_msg;
    MMIFMM_FUNC_TYPE_E  s_func_type;            //����Ԫ������
    MMI_WIN_ID_T        s_cbk_win_id;
    MMI_MESSAGE_ID_E    s_cbk_msg_id;
    FILEARRAY           s_file_array_search;    //�����������
    FILEARRAY           s_farray_cbk;
    FILEARRAY           s_foldre_farray; //�����Ҫ������·��
    FILEARRAY_SORT_E    s_sort_type;            //��������
    SearchFileCallBack  search_cbk;  //search file callback,Maybe PNULL.
#if defined MMIFMM_XFIND_SUPPORT
    SFS_XFIND_GETKEY    s_xfind_getKey_cbk;
    SFS_XFIND_COMPARE   s_xfind_compareFunc_cbk;
    SFS_HANDLE          xfind_handle;  //xfind handle
    uint32              file_num;       //xfind find file num
    uint32              folder_num;     //xfind find folder num
#endif
#ifdef DRM_SUPPORT
    uint32              s_filter_type;
#endif
} MMIFMM_SEARCH_SRV_HANDLE_T;
/**-------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/
//�����ļ�task����ֹͣ�����ı���
LOCAL BOOLEAN               *s_is_stopfindfiles_ptr = NULL; //�Ƿ�ֹͣ�����ļ�
//LOCAL BOOLEAN                s_is_sent_msg = FALSE;
extern BOOLEAN unistrcmp_ext (const uint16* searchName, const uint16* fileName, BOOLEAN ifCCare);

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
#if defined MMIFMM_XFIND_SUPPORT
/*****************************************************************************/
// Description : get xfind ctlstr
// Global resource dependence :
// Author: jian.ma
// Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIFMM_GetXFindCtlStr (
    MMIFMM_XFIND_T *xfind_param,
    wchar   *ctlstr,
    uint16  ctlstr_len
);

/*****************************************************************************/
// Description : handle xfind routine callback
// Global resource dependence :
// Author: jian.ma
// Note:
/*****************************************************************************/
LOCAL int XFindRoutineCallback (
    uint32 inParam,
    uint32 status,
    uint32 outParam1,
    uint32 outParam2
);

/*****************************************************************************/
// Description : handle xfind pre search
// Global resource dependence :
// Author: jian.ma
// Note:
/*****************************************************************************/
LOCAL BOOLEAN XFindQuickSearch (
    MMIFMM_SRV_HANDLE handle,
    MMIFMM_XFIND_T *xfind_param
);

/*****************************************************************************/
//  Description : handle find callback
//  Global resource dependence :
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL void XFindHandleFindCallback (
    MMIFMM_SRV_HANDLE   handle,
    MMIFMM_XFIND_INFO_T *xfind_info
);

/*****************************************************************************/
// Description : search xfile get function
// Global resource dependence :
// Author: jian.ma
// Note:
/*****************************************************************************/
LOCAL SFS_ERROR_E MMIFMM_XFindGet (
    SFS_HANDLE sfs_handle,
    uint32 index,
    SFS_FIND_DATA_T *fd
);

/*****************************************************************************/
// Description : search xfile close
// Global resource dependence :
// Author: jian.ma
// Note:
/*****************************************************************************/
LOCAL SFS_ERROR_E MMIFMM_XFindClose (SFS_HANDLE sfs_handle);

/*****************************************************************************/
// Description : search xfile open
// Global resource dependence :
// Author: jian.ma
// Note:
/*****************************************************************************/
LOCAL SFS_ERROR_E MMIFMM_XFindOpen (
    uint16* pName,
    uint16* tName,
    uint16* ctlStr,
    SFS_XFIND_GETKEY getKey,
    SFS_XFIND_COMPARE compareFunc,
    SFS_XFUNC_ROUTINE routing,
    uint32	routingParam,
    SFS_HANDLE* handle,
    SFS_OVERLAPPED_T *overlapped_ptr
);


#endif

/*****************************************************************************/
// Description : xfind all files
// Global resource dependence :
// Author: jian.ma
// Note:
/*****************************************************************************/
LOCAL void XFindAllFiles (
    MMIFMM_SRV_HANDLE handle,
    wchar *find_path,
    uint16 find_path_len,
    FILEARRAY folder_array,
    FindAllFiles_CallBack callback
);
/*****************************************************************************/
//  Description : set param before search
//  Global resource dependence :
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
LOCAL void InitSearchParam (
    MMIFMM_SRV_HANDLE               handle,
    const wchar                     *path_ptr,          //in, ����Ŀ¼���������NULL�����������д洢��
    uint16                          path_len,
    MMIFMM_FILTER_T                 *filter_ptr    //ɸѡ����
);
/*****************************************************************************/
//  Description : ��ָ��Ŀ¼�������ļ�
//  Return:
//  Global resource dependence :
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
LOCAL void   SearchFile (MMIFMM_SRV_HANDLE handle);

/*****************************************************************************/
// Description : ��ָ��Ŀ¼�����������ļ�
// Global resource dependence :
// Author: liqing.peng
// Note:
/*****************************************************************************/
LOCAL void FindAllFiles (
    MMIFMM_SRV_HANDLE handle,
    wchar       *find_path,
    uint16      find_path_len,
    FILEARRAY     file_array,
    FindAllFiles_CallBack   callback
);


/*****************************************************************************/
// Description : SearchFileAndSaveFolder���õ��Ļص�����
// Global resource dependence :
// Author: liqing.peng
// Note:
/*****************************************************************************/
LOCAL void SearchFileAndSaveFolder_CallBack (
    MMIFMM_SRV_HANDLE handle,
    const wchar     *find_path,
    uint16      find_path_len,
    FILEARRAY   folder_array,
    SFS_FIND_DATA_T *find_data
);

/*****************************************************************************/
// Description : ��ָ��Ŀ¼�°�ɸѡ�������ļ����ļ���, ���ѽ����¼��file_array��
// Global resource dependence :
// Author: liqing.peng
// Note:
/*****************************************************************************/
LOCAL void SearchFileAndSaveFolder (
    MMIFMM_SRV_HANDLE handle,
    wchar     *find_path,
    uint16        find_path_len,
    FILEARRAY     file_array,
    FILEARRAY     folder_array,
    FILEARRAY     recurse_folder_array
);

/*****************************************************************************/
// Description : ��ָ��Ŀ¼�°�ɸѡ�������ļ��������ļ���, ���ѽ����¼��file_array��
// Global resource dependence :
// Author: liqing.peng
// Note:
/*****************************************************************************/
LOCAL void SearchFileAndFolder (
    MMIFMM_SRV_HANDLE handle,
    wchar         *find_path,
    uint16        find_path_len,
    FILEARRAY     folder_array,
    FILEARRAY     file_array
);

/*****************************************************************************/
// Description : SearchFileAndFolder���õ��Ļص�����
// Global resource dependence :
// Author: liqing.peng
// Note:
/*****************************************************************************/
LOCAL void SearchFileAndFolder_CallBack (
    MMIFMM_SRV_HANDLE handle,
    const wchar     *folder_ptr,
    uint16      folder_namelen,
    FILEARRAY     folder_array,
    SFS_FIND_DATA_T *find_data
);
/*****************************************************************************/
// Description : �����һ��Ŀ¼�²����ļ���������Ŀ¼��������Ŀ¼������
// Global resource dependence :
// Author: liqing.peng
// Note:
/*****************************************************************************/
LOCAL BOOLEAN RunOnceInSearchFolders (
    MMIFMM_SRV_HANDLE handle,
    FILEARRAY     recurse_folder_array,
    FILEARRAY     folder_array,
    FILEARRAY     file_array
);

/*****************************************************************************/
// Description : �ڵ�ǰĿ¼�����������ļ��кͷ���ɸѡ���������ļ�
// Global resource dependence :
// Author: liqing.peng
// Note:
/*****************************************************************************/
LOCAL void DoSearchInFolder (
    MMIFMM_SRV_HANDLE handle,
    wchar     *folder_ptr,
    uint16        folder_namelen,
    FILEARRAY     folder_array,
    FILEARRAY     file_array
);

/*****************************************************************************/
// Description : �ڵ�ǰĿ¼�����������ļ��кͷ���ɸѡ���������ļ�
// Global resource dependence :
// Author: liqing.peng
// Note:
/*****************************************************************************/
LOCAL void DoSearchInFolderRecurse (
    MMIFMM_SRV_HANDLE handle,
    const wchar       *folder_ptr,
    uint16        folder_namelen,
    FILEARRAY     folder_array,
    FILEARRAY     file_array
);


/*****************************************************************************/
// Description : ��Դ�ַ����н��������ؼ��ַ���
// Global resource dependence :
// Author: liqing.peng
// Note:
/*****************************************************************************/
LOCAL BOOLEAN ParseFilterStr (
    MMIFMM_FILTER_T                  *filter_ptr,
    MMIFMM_SRV_HANDLE                handle
);

/**--------------------------------------------------------------------------*
 **                         Function Implement                               *
 **--------------------------------------------------------------------------*/
#if defined MMIFMM_XFIND_SUPPORT
/*****************************************************************************/
// Description : search xfile get function
// Global resource dependence :
// Author: jian.ma
// Note:
/*****************************************************************************/
LOCAL SFS_ERROR_E MMIFMM_XFindGet (
    SFS_HANDLE sfs_handle,
    uint32 index,
    SFS_FIND_DATA_T *fd
)
{
    SFS_ERROR_E result = SFS_ERROR_NONE;

    if (SFS_INVALID_HANDLE == sfs_handle || PNULL == fd)
    {
        result =  SFS_ERROR_INVALID_PARAM;
    }
    else
    {
        result = SFS_XFindGet (sfs_handle, index, fd);
    }

    return result;
}

/*****************************************************************************/
// Description : search xfile close
// Global resource dependence :
// Author: jian.ma
// Note:
/*****************************************************************************/
LOCAL SFS_ERROR_E MMIFMM_XFindClose (SFS_HANDLE sfs_handle)
{
    SFS_ERROR_E result = SFS_ERROR_NONE;

    if (SFS_INVALID_HANDLE == sfs_handle)
    {
        result =  SFS_ERROR_INVALID_PARAM;
    }
    else
    {
        result = SFS_XFindClose (sfs_handle);
    }

    return result;
}

/*****************************************************************************/
// Description : search xfile open
// Global resource dependence :
// Author: jian.ma
// Note:
/*****************************************************************************/
LOCAL SFS_ERROR_E MMIFMM_XFindOpen (
    uint16* pName,
    uint16* tName,
    uint16* ctlStr,
    SFS_XFIND_GETKEY getKey,
    SFS_XFIND_COMPARE compareFunc,
    SFS_XFUNC_ROUTINE routing,
    uint32	routingParam,
    SFS_HANDLE* handle,
    SFS_OVERLAPPED_T *overlapped_ptr
)
{
    SFS_ERROR_E result = SFS_ERROR_NONE;

    if (PNULL == pName || PNULL == tName || PNULL == handle)
    {
        result =  SFS_ERROR_INVALID_PARAM;
    }
    else
    {
        result = SFS_XFindOpen (pName,
                                tName,
                                ctlStr,
                                getKey,
                                compareFunc,
                                routing,
                                routingParam,
                                handle,
                                overlapped_ptr
                               );
    }

    return result;
}

/*****************************************************************************/
// Description : get xfind ctlstr
// Global resource dependence :
// Author: jian.ma
// Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIFMM_GetXFindCtlStr (
    MMIFMM_XFIND_T *xfind_param,
    wchar   *ctlstr,
    uint16  ctlstr_len
)
{
    wchar       sort_n_a []  = L"/SB:+N";//���ִ�С����,����
    wchar       sort_n_d []  = L"/SB:-N";//���ִӴ�С,����
    wchar       sort_p_a []  = L"/SB:+P";//���ʹ�С��������
    wchar       sort_p_d []  = L"/SB:-P";//���ʹӴ�С������
    wchar       sort_s_a []  = L"/SB:+S";//��С��С��������
    wchar       sort_s_d []  = L"/SB:-S";//��С�Ӵ�С������
    wchar       sort_a_a []  = L"/SB:+A";//���Դ�С��������
    wchar       sort_a_d []  = L"/SB:-A";//���ԴӴ�С������
    wchar       sort_tc_a [] = L"/SB:+TC";//����ʱ���С��������
    wchar       sort_tc_d [] = L"/SB:-TC";//����ʱ��Ӵ�С������
    wchar       sort_tm_a [] = L"/SB:+TM";//�޸�ʱ���С��������
    wchar       sort_tm_d [] = L"/SB:-TM";//�޸�ʱ��Ӵ�С������
    wchar       sort_ta_a [] = L"/SB:+TA";//����ʱ���С��������
    wchar       sort_ta_d [] = L"/SB:-TA";//����ʱ��Ӵ�С������
    wchar       sort_cus []  = L"/SB:CUS";
    wchar       ctlStr_a[]   = L"/A:-H-S";
    wchar       ctlStr_d[]   = L"/A:-H-S/D:-H-S";
    wchar       *sort_ptr = PNULL;

    if (
        PNULL == xfind_param
        || PNULL == ctlstr
        || 0 == ctlstr_len
        || ctlstr_len < (MMIAPICOM_Wstrlen (sort_ptr) + MMIAPICOM_Wstrlen (ctlStr_d))
    )
    {
        //SCI_TRACE_LOW:"MMIFMM_GetXFindCtlStr: xfind_param = %d,ctlstr=%d,ctlstr_len=%d"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_INTERFACE_730_112_2_18_2_20_20_269, (uint8*) "ddd", xfind_param, ctlstr, ctlstr_len);
        return FALSE;
    }

    switch (xfind_param->sort_type)
    {
    case FILEARRAY_SORT_NAME_ASCEND:
        sort_ptr = sort_n_a;
        break;

    case FILEARRAY_SORT_NAME_DESCEND:
        sort_ptr = sort_n_d;
        break;

    case FILEARRAY_SORT_SIZE_ASCEND:
        sort_ptr = sort_s_a;
        break;

    case FILEARRAY_SORT_SIZE_DESCEND:
        sort_ptr = sort_s_d;
        break;

    case FILEARRAY_SORT_TYPE_ASCEND:
        sort_ptr = sort_p_a;
        break;

    case FILEARRAY_SORT_TYPE_DESCEND:
        sort_ptr = sort_p_d;
        break;

    case FILEARRAY_SORT_ATTR_ASCEND://���� ��
        sort_ptr = sort_a_a;
        break;

    case FILEARRAY_SORT_ATTR_DESCEND:
        sort_ptr = sort_a_d;
        break;

    case FILEARRAY_SORT_TIME_CREATE_ASCEND://����ʱ����
        sort_ptr = sort_tc_a;
        break;

    case FILEARRAY_SORT_TIME_CREATE_DESCEND:
        sort_ptr = sort_tc_d;
        break;

    case FILEARRAY_SORT_TIME_ASCEND:
    case FILEARRAY_SORT_TIME_MODIFY_ASCEND://�޸�ʱ����
        sort_ptr = sort_tm_a;
        break;

    case FILEARRAY_SORT_TIME_DESCEND:
    case FILEARRAY_SORT_TIME_MODIFY_DESCEND:
        sort_ptr = sort_tm_d;
        break;

    case FILEARRAY_SORT_TIME_ASK_ASCEND://����ʱ����
        sort_ptr = sort_ta_a;
        break;

    case FILEARRAY_SORT_TIME_ASK_DESCEND:
        sort_ptr = sort_ta_d;
        break;

    case FILEARRAY_SORT_TYPE_CUST://������û��Զ��������ʽ������Ҫ��д��getkey��comp����
        sort_ptr = sort_cus;
        break;

    default:
        sort_ptr = sort_n_a;
        break;
    }

    if (FUNC_FIND_FILE != xfind_param->func_type || xfind_param->is_recurse)
    {
        MMIAPICOM_Wstrcat (ctlstr, ctlStr_d);
    }
    else
    {
        MMIAPICOM_Wstrcat (ctlstr, ctlStr_a);
    }

    MMIAPICOM_Wstrcat (ctlstr, sort_ptr);

    return TRUE;
}


/*****************************************************************************/
// Description : handle xfind routine callback
// Global resource dependence :
// Author: jian.ma
// Note:
/*****************************************************************************/
LOCAL int XFindRoutineCallback (
    uint32 inParam,
    uint32 status,
    uint32 outParam1,
    uint32 outParam2
)
{
    //SCI_TRACE_LOW:"MMIXFIND XFindRoutineCallback: inParam = %d,status = %d,outParam1 = %d,outParam2 = %d,"
    SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_INTERFACE_828_112_2_18_2_20_20_270, (uint8*) "dddd", inParam, status, outParam1, outParam2);

    //search finish
    if (SFS_XFIND_SORTING_END == status)
    {
        MMIFMM_SEARCH_SRV_HANDLE_T *search_handle = (MMIFMM_SEARCH_SRV_HANDLE_T *)inParam;
        if (PNULL !=search_handle)
        {
            search_handle->folder_num = outParam1;
            search_handle->file_num = outParam2;
        }
    }

    //user cancel search
    if (MMIFMM_IsSearchingStopped())
    {
        //SCI_TRACE_LOW:"MMIXFIND XFindRoutineCallback: SFS_X_STOP"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_INTERFACE_839_112_2_18_2_20_20_271, (uint8*) "");
        return SFS_X_STOP;
    }

    //search continue
    return SFS_X_CONTINUE;

}

/*****************************************************************************/
//  Description : handle find callback
//  Global resource dependence :
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL void XFindHandleFindCallback (
    MMIFMM_SRV_HANDLE   handle,
    MMIFMM_XFIND_INFO_T *xfind_info
)
{
    uint16  max_num = 0;
    uint32  i = 0;
    SFS_FIND_DATA_T *find_data_ptr = PNULL;
    MMIFMM_SEARCH_SRV_HANDLE_T *search_handle = (MMIFMM_SEARCH_SRV_HANDLE_T *) handle;

    //SCI_TRACE_LOW:"MMIXFIND HandleFindCallback"
    SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_INTERFACE_857_112_2_18_2_20_20_272, (uint8*) "");

    if (PNULL == xfind_info || PNULL == search_handle)
    {
        //SCI_TRACE_LOW:"MMIXFIND HandleFindCallback PNULL == xfind_info!"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_INTERFACE_860_112_2_18_2_20_20_273, (uint8*) "");
        return;
    }
    find_data_ptr = (SFS_FIND_DATA_T *)SCI_ALLOC_APPZ(sizeof(SFS_FIND_DATA_T));

    max_num = search_handle->folder_num + search_handle->file_num;

    for (i = 0 ; i < max_num; i++)
    {
        //get find info folder + file
        if (SFS_ERROR_NONE == MMIFMM_XFindGet (search_handle->xfind_handle, i, find_data_ptr))
        {
            //call callback and update file array
            xfind_info->callback (
                search_handle,
                xfind_info->find_path,
                xfind_info->find_path_len,
                xfind_info->folder_array,
                find_data_ptr
            );
        }
        if (MMIFMM_IsSearchingStopped())
        {
            //SCI_TRACE_LOW:"MMIXFIND HandleFindCallback break"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_INTERFACE_878_112_2_18_2_20_20_274, (uint8*) "");
            break;
        }
    }

    SCI_FREE(find_data_ptr);
    //close xfind handle
    MMIFMM_XFindClose (search_handle->xfind_handle);
    search_handle->xfind_handle = SFS_INVALID_HANDLE;

    if (0 != max_num
            && search_handle->s_func_type == FUNC_FIND_FOLDERS_WITH_SUBFILES
            && 0 != search_handle->s_cbk_win_id) //search on folder
    {
        MMK_PostMsg (search_handle->s_cbk_win_id, MSG_FMM_SEARCH_ONE_FOLDER_CNF, PNULL, 0);
    }
}


/*****************************************************************************/
// Description : handle xfind pre search
// Global resource dependence :
// Author: jian.ma
// Note:
/*****************************************************************************/
LOCAL BOOLEAN XFindQuickSearch (
    MMIFMM_SRV_HANDLE handle,
    MMIFMM_XFIND_T *xfind_param
)
{
    SFS_ERROR_E         find_result = 0;
    SFS_HANDLE			find_handle = 0;
    BOOLEAN             ret = FALSE;
    wchar               ctlstr[MMIFMM_XFIND_CTLSTR_MAX_LEN] = {0};

    MMIFMM_SEARCH_SRV_HANDLE_T *search_handle = (MMIFMM_SEARCH_SRV_HANDLE_T *) handle;

    if (PNULL == xfind_param)
    {
        return FALSE;
    }

    if (!MMIFMM_GetXFindCtlStr (xfind_param, ctlstr, MMIFMM_XFIND_CTLSTR_MAX_LEN))
    {
        return FALSE;
    }

    find_result = MMIFMM_XFindOpen (
                      xfind_param->find_path,
                      xfind_param->filter,
                      ctlstr,
                      xfind_param->getKey_cbk,
                      xfind_param->compareFunc_cbk,
                      &XFindRoutineCallback,/*lint !e546*/
                      handle,/*lint !e64*/
                      &find_handle,
                      PNULL
                  );

    search_handle->xfind_handle = find_handle;

    //SCI_TRACE_LOW:"MMIFMM_XFindQuickSearch: result = %d,  find_handle= 0x%x"
    SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_INTERFACE_923_112_2_18_2_20_21_275, (uint8*) "dd", find_result, find_handle);

    if (SFS_ERROR_NONE != find_result) //folder file number > max xfind can sort number
    {
        ret = FALSE;
        //close xfind handle
        MMIFMM_XFindClose (search_handle->xfind_handle);
        search_handle->xfind_handle = SFS_INVALID_HANDLE;
    }
    else
    {
        ret = TRUE;
    }

    return ret;
}

#endif

/*****************************************************************************/
//  Description : ��ָ��Ŀ¼�������ļ�
//  Return:
//  Global resource dependence :
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
LOCAL void  SearchFile (MMIFMM_SRV_HANDLE handle)
{
    FILEARRAY  folder_array = {0};
    MMIFMM_SEARCH_SRV_HANDLE_T *search_handle = (MMIFMM_SEARCH_SRV_HANDLE_T *) handle;

    //ͬ������
    if (PNULL != search_handle)
    {
        if (0 == search_handle->s_cbk_win_id
                && 0 == search_handle->s_cbk_msg_id)
        {
            if (PNULL != search_handle->s_file_array_search)
            {
                if (search_handle->s_func_type == FUNC_FIND_FOLDERS_WITH_SUBFILES)
                {
                    //��Ҫ���ļ��е����г�
                    folder_array = MMIAPIFILEARRAY_Create();
                    MMIAPIFILEARRAY_AddFolderArray (search_handle->s_file_array_search, folder_array);
                }

                if (search_handle->s_is_recurse)
                {
                    DoSearchInFolderRecurse (search_handle, search_handle->s_cur_path, search_handle->s_cur_path_len, folder_array, search_handle->s_file_array_search);
                }
                else
                {
                    DoSearchInFolder (search_handle, search_handle->s_cur_path, search_handle->s_cur_path_len,  folder_array, search_handle->s_file_array_search);
                }

                if (search_handle->s_func_type == FUNC_FIND_FOLDERS_WITH_SUBFILES)
                {
                    MMIAPIFILEARRAY_SetFolderStartFileAndEndFile (search_handle->s_file_array_search);
                }
            }

            if (PNULL != search_handle->search_cbk)
            {
                search_handle->search_cbk (search_handle, search_handle->s_file_array_search);
            }
        }
        else
        {
            //�����첽��ʽ����һ���½���task�������ļ�
            APPFILE_StopFindFile();

            MMIFILE_ImpoveFileTaskPriority();

            //�ȴ�find file�Զ�����
            while (APPFILE_IsFindingFile())
            {
                SCI_Sleep (100);
            }

            search_handle->s_is_sent_msg = FALSE;
            MMIFILE_RestoreFileTaskPriority();
            APPFILE_SetStartFindFileFlag();
            APPFILE_CreateTask();
            MMI_SendSignal_2APPCOPY ( (uint16) APP_FIND_FILES, search_handle);/*lint !e64*/
        }
    }
}


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
)
{
    FILEARRAY  folder_array = {0};
    MMIFMM_SEARCH_SRV_HANDLE_T* search_handle = (MMIFMM_SEARCH_SRV_HANDLE_T*) handle;

    SCI_TRACE_LOW("MMIFMM_SearchFileInTask, enter search_handle = 0x%x",search_handle);
   
    //SCI_ASSERT(NULL != is_stop_ptr);
    if (NULL == is_stop_ptr)
    {
        //SCI_TRACE_LOW:"MMIFMM_SearchFileInTask param error:NULL == is_stop_ptr"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_INTERFACE_1333_112_2_18_2_20_22_289, (uint8*) "");
        return FALSE;
    }
    s_is_stopfindfiles_ptr = is_stop_ptr;

    if (MMIFMM_IsSearchingStopped())
    {
        //SCI_TRACE_LOW:"MMIFMM_SearchFileInTask, stoped by user !!"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_INTERFACE_1340_112_2_18_2_20_22_290, (uint8*) "");
        return FALSE;
    }

    if (PNULL != search_handle && PNULL !=search_handle->s_file_array_search)
    {
        if (search_handle->s_func_type == FUNC_FIND_FOLDERS_WITH_SUBFILES)
        {
            //��Ҫ���ļ��е����г�
            folder_array = MMIAPIFILEARRAY_Create();
            MMK_CheckAllocatedMemInfo( _D_FILE_NAME, _D_FILE_LINE );
            MMIAPIFILEARRAY_AddFolderArray (search_handle->s_file_array_search, folder_array);
            MMK_CheckAllocatedMemInfo( _D_FILE_NAME, _D_FILE_LINE );
        }

        if (search_handle->s_is_recurse)
        {
            MMK_CheckAllocatedMemInfo( _D_FILE_NAME, _D_FILE_LINE );
            DoSearchInFolderRecurse (search_handle, search_handle->s_cur_path, search_handle->s_cur_path_len, folder_array, search_handle->s_file_array_search);
            MMK_CheckAllocatedMemInfo( _D_FILE_NAME, _D_FILE_LINE );
        }
        else
        {
            MMK_CheckAllocatedMemInfo( _D_FILE_NAME, _D_FILE_LINE );
            DoSearchInFolder (search_handle, search_handle->s_cur_path, search_handle->s_cur_path_len, folder_array, search_handle->s_file_array_search);
            MMK_CheckAllocatedMemInfo( _D_FILE_NAME, _D_FILE_LINE );
        }

        if (search_handle->s_func_type == FUNC_FIND_FOLDERS_WITH_SUBFILES)
        {
            MMK_CheckAllocatedMemInfo( _D_FILE_NAME, _D_FILE_LINE );
            MMIAPIFILEARRAY_SetFolderStartFileAndEndFile (search_handle->s_file_array_search);
            MMK_CheckAllocatedMemInfo( _D_FILE_NAME, _D_FILE_LINE );
        }
        return TRUE;
    }

    return FALSE;
}


/*****************************************************************************/
//  Description : ��MMIFMM_SearchFileInPath���ʹ��
//  Return:
//  Global resource dependence :
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
PUBLIC void MMIFMM_SearchFileCallBack (MMIFMM_SRV_HANDLE handle)
{
    MMIFMM_SEARCH_SRV_HANDLE_T* search_handle = (MMIFMM_SEARCH_SRV_HANDLE_T*) handle;
    
    SCI_TRACE_LOW("MMIFMM_SearchFileCallBack, enter search_handle=0x%x",search_handle);

    //send msg back to window
    if (APPFILE_IsFindingFile())
    {
        //�µ������Ѿ���ʼ��һ��callback����Ҫ����
        //SCI_TRACE_LOW:"MMIAPIFMM_SearchFileCallBack, ignored !!"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_INTERFACE_1389_112_2_18_2_20_22_292, (uint8*) "");
        if (PNULL != search_handle)
        {
            if (PNULL != search_handle->search_cbk)
            {
                search_handle->search_cbk (search_handle, search_handle->s_file_array_search);
            }
        }
        return;
    }
    s_is_stopfindfiles_ptr = NULL;

    if (PNULL != search_handle && !search_handle->s_is_sent_msg)
    {
        search_handle->s_is_sent_msg = TRUE;

        if ( 0 != search_handle->s_cbk_win_id && 0 != search_handle->s_cbk_msg_id)
        {
            MMK_PostMsg(search_handle->s_cbk_win_id, search_handle->s_cbk_msg_id, NULL,0);
        }
        if(PNULL != search_handle->search_cbk)
        {
            search_handle->search_cbk (search_handle, search_handle->s_file_array_search);
        }
    }
}



/*****************************************************************************/
//  Description : ��MMIFMM_SearchFileInPath���ʹ��
//  Return:
//  Global resource dependence :
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
PUBLIC void MMIFMM_SearchFileStop (MMIFMM_SRV_HANDLE handle)
{
    BOOLEAN  result = 0;
    MMIFMM_SEARCH_SRV_HANDLE_T* search_handle = (MMIFMM_SEARCH_SRV_HANDLE_T*) handle;
    
    SCI_TRACE_LOW("MMIFMM_SearchFileStop, enter search_handle=0x%x",search_handle);

    APPFILE_StopFindFile();

    if (APPFILE_IsFindingFile()) //cr240799
    {
        //���file task���ȼ�
        MMIFILE_ImpoveFileTaskPriority();
        result = FALSE;
    }
    else
    {
        result = TRUE;
    }

    //�ȴ�find file�Զ�����
    while (APPFILE_IsFindingFile())
    {
        SCI_Sleep (100);
    }

    //ǿ�ƹر�task
    //APPFILE_ExitTask();
    if (!result)
    {
        //�ָ�file task���ȼ�
        MMIFILE_RestoreFileTaskPriority();
    }
    s_is_stopfindfiles_ptr = NULL;
    
    if (PNULL != search_handle && !search_handle->s_is_sent_msg)
    {
        search_handle->s_is_sent_msg = TRUE;
        //send msg back to window
        if (0 != search_handle->s_cbk_win_id && 0 != search_handle->s_cbk_msg_id)
        {
            /*cr111931*/
            MMK_SendMsg(search_handle->s_cbk_win_id, search_handle->s_cbk_msg_id, (DPARAM) &result);
            //MMK_PostMsg(search_handle->s_cbk_win_id, search_handle->s_cbk_msg_id, (DPARAM) &result,sizeof(BOOLEAN));
        }
        if (PNULL != search_handle->search_cbk)
        {
            search_handle->search_cbk (search_handle, search_handle->s_file_array_search);
        }
    }
}


/*****************************************************************************/
// Description : �ڵ�ǰĿ¼�����������ļ��кͷ���ɸѡ���������ļ�
// Global resource dependence :
// Author: liqing.peng
// Note:
/*****************************************************************************/
LOCAL void DoSearchInFolderRecurse (
    MMIFMM_SRV_HANDLE handle,
    const wchar       *folder_ptr,
    uint16        folder_namelen,
    FILEARRAY     folder_array,
    FILEARRAY     file_array
)
{
    //FMM_SEARCH_FOLDER_T *search_folder_ptr = NULL;
    FILEARRAY_DATA_T file_data = {0};
    FILEARRAY     Recurse_folder_array = {0};
    MMIFILE_DEVICE_E device = MMI_DEVICE_UDISK;
    uint32           i = 0;
    uint16           default_path_len = 0;
    MMIFMM_SEARCH_SRV_HANDLE_T *search_handle = (MMIFMM_SEARCH_SRV_HANDLE_T *) handle;

    if (NULL == file_array)
    {
        //SCI_TRACE_LOW:"DoSearchInFolderRecurse NULL == file_array !!"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_INTERFACE_2358_112_2_18_2_20_24_303, (uint8*) "");
        return;
    }

    default_path_len = (uint16) MMIAPICOM_Wstrlen (search_handle->s_default_path);
    Recurse_folder_array = MMIAPIFILEARRAY_Create();

    if (PNULL == Recurse_folder_array)
    {
        //SCI_TRACE_LOW:"DoSearchInFolderRecurse create s_foldre_farray fail !"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_INTERFACE_2366_112_2_18_2_20_24_304, (uint8*) "");
        return;
    }

    if (default_path_len > 0)
    {
        //add default folder
        SCI_MEMSET (&file_data, 0x00, sizeof (file_data));
        file_data.type = FILEARRAY_TYPE_FOLDER;

        MMI_WSTRNCPY (file_data.filename, FMM_SEARCH_FILENAME_MAXLEN,
                      search_handle->s_default_path, default_path_len, default_path_len);/*lint !e670 */
        file_data.name_len = default_path_len;
        MMIAPIFILEARRAY_Add (Recurse_folder_array, &file_data);
    }

    //file_data.type = FILEARRAY_TYPE_FOLDER;
    if (NULL != folder_ptr && 0 < folder_namelen)
    {
        //����ָ��Ŀ¼
        SCI_MEMSET (&file_data, 0x00, sizeof (file_data));

        if ( (MMIAPICOM_Wstrcmp (folder_ptr, search_handle->s_default_path))) //�û�Ԥ��·����������·������ͬ
        {
            file_data.type = FILEARRAY_TYPE_FOLDER;
            MMI_WSTRNCPY (file_data.filename, FMM_SEARCH_FILENAME_MAXLEN,
                          folder_ptr, folder_namelen, folder_namelen);/*lint !e670 */

            file_data.name_len = folder_namelen;
            MMIAPIFILEARRAY_Add (Recurse_folder_array, &file_data);
        }


    }
    else //�������д洢��
    {
        for (device = MMI_DEVICE_UDISK; device < MMI_DEVICE_NUM; device++)
        {
            if (MMIAPIFMM_GetDeviceStatus ( (uint16 *) MMIAPIFMM_GetDevicePath (device), MMIAPIFMM_GetDevicePathLen (device)))
            {
                file_data.filename[0] = *MMIAPIFMM_GetDevicePath (device);
                file_data.filename[1] = MMIFILE_COLON;
                file_data.name_len = 2;

                if ( (MMIAPICOM_Wstrcmp (file_data.filename, search_handle->s_default_path))) //�û�Ԥ��·����������·������ͬ
                {
                    file_data.type = FILEARRAY_TYPE_FOLDER;
                    MMIAPIFILEARRAY_Add (Recurse_folder_array, &file_data);
                }

            }
        }
    }

    if (search_handle->s_func_type == FUNC_FIND_FOLDERS_WITH_SUBFILES)
    {
        for (i = 0; i < MMIAPIFILEARRAY_GetArraySize (Recurse_folder_array); i++)
        {
            SCI_MEMSET (&file_data, 0x00, sizeof (file_data));

            if (MMIAPIFILEARRAY_Read (Recurse_folder_array, i, &file_data))
            {
                if (i == 0 && default_path_len > 0)
                {
                    //����Ĭ��·����Ĭ�ϴ���
                    MMIAPIFILEARRAY_Add (folder_array, &file_data);
                }
                else if (MMIFMM_IsFilesInFolder (file_data.filename, file_data.name_len, search_handle->s_filter_str))
                {
                    if (!MMIAPICOM_Wstrcmp (search_handle->s_default_path, file_data.filename))
                    {
                        file_data.type = FILEARRAY_TYPE_DEFAULT_FOLDER;
                    }

                    MMIAPIFILEARRAY_Add (folder_array, &file_data);
                }
            }
        }
    }

    //��ʼ����
    while (0 < MMIAPIFILEARRAY_GetArraySize (Recurse_folder_array))
    {
        if (!RunOnceInSearchFolders (search_handle, Recurse_folder_array, folder_array, file_array))
        {
            //SCI_TRACE_LOW:"[MMIFMM] DoSearchInFolderRecurse read error !"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_INTERFACE_2445_112_2_18_2_20_24_305, (uint8*) "");
            break;
        }

        if (MMIFMM_IsSearchingStopped())
        {
            //�û�Ҫ��ֹͣ����
            break;
        }
    }

    MMIAPIFILEARRAY_Destroy (&Recurse_folder_array);
}

/*****************************************************************************/
// Description : �ڵ�ǰĿ¼�����������ļ��кͷ���ɸѡ���������ļ�
// Global resource dependence :
// Author: liqing.peng
// Note:
/*****************************************************************************/
LOCAL void DoSearchInFolder (
    MMIFMM_SRV_HANDLE handle,
    wchar         *folder_ptr,
    uint16        folder_namelen,
    FILEARRAY     folder_array,
    FILEARRAY     file_array
)
{
    FILEARRAY_DATA_T    f_array_data = {0};
    MMIFILE_DEVICE_E    device = MMI_DEVICE_UDISK;
    MMIFMM_SEARCH_SRV_HANDLE_T *search_handle = (MMIFMM_SEARCH_SRV_HANDLE_T *) handle;

    if (NULL == file_array)
    {
        //SCI_TRACE_LOW:"DoSearchInFolder NULL == file_array !!"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_INTERFACE_2474_112_2_18_2_20_24_306, (uint8*) "");
        return;
    }

    if (NULL == folder_ptr || 0 == folder_namelen)
    {

        //��ʾ��Ŀ¼
        for (device = MMI_DEVICE_UDISK; device < MMI_DEVICE_NUM; device++)
        {
            if (MMIAPIFMM_GetDeviceStatus ( (uint16 *) MMIAPIFMM_GetDevicePath (device), MMIAPIFMM_GetDevicePathLen (device)))
            {
                f_array_data.filename[0] = *MMIAPIFMM_GetDevicePath (device);
                f_array_data.filename[1] = MMIFILE_COLON;
                f_array_data.name_len = 2;
                f_array_data.type = FILEARRAY_TYPE_FOLDER;
                MMIAPIFILEARRAY_Add (file_array, &f_array_data);
            }
        }
    }
    else
    {
        //�����ļ��������ļ���
        SearchFileAndFolder (search_handle, folder_ptr, folder_namelen, folder_array, file_array);
    }
}

/*****************************************************************************/
// Description : ������Ŀ¼������, ��һ��������
//               �������������һ��Ŀ¼����ɸѡ�ֲ����ļ���������Ŀ¼����
//               ����Ŀ¼������
// Global resource dependence :
// Author: liqing.peng
// Note:
/*****************************************************************************/
LOCAL BOOLEAN RunOnceInSearchFolders (
    MMIFMM_SRV_HANDLE handle,
    FILEARRAY     Recurse_folder_array,
    FILEARRAY     folder_array,
    FILEARRAY     file_array
)
{
    FILEARRAY_DATA_T file_data = {0};
    uint16          folder_num = MMIAPIFILEARRAY_GetArraySize (Recurse_folder_array);
    BOOLEAN         ret  = FALSE;
    MMIFMM_SEARCH_SRV_HANDLE_T *search_handle = (MMIFMM_SEARCH_SRV_HANDLE_T *) handle;

    if (0 == folder_num || NULL == file_array)
    {
        return FALSE;
    }

    //��ô˴�Ҫ������Ŀ¼, �����е����һ��Ŀ¼
    if (MMIAPIFILEARRAY_Read (Recurse_folder_array, 0, &file_data))
    {
        //�����һ��Ŀ¼�°�ɸѡ�ֲ���
        MMIAPIFILEARRAY_Delete (Recurse_folder_array, 0);

        if (FILEARRAY_TYPE_DEFAULT_FOLDER != file_data.type)
        {
            SearchFileAndSaveFolder (search_handle, file_data.filename, file_data.name_len,
                                     file_array, folder_array, Recurse_folder_array);
        }

        ret = TRUE;
    }

    return ret;
}

/*****************************************************************************/
// Description : SearchFileAndSaveFolder���õ��Ļص�����
// Global resource dependence :
// Author: liqing.peng
// Note:
/*****************************************************************************/
LOCAL void SearchFileAndSaveFolder_CallBack (
    MMIFMM_SRV_HANDLE handle,
    const wchar         *find_path,
    uint16      find_path_len,
    FILEARRAY   folder_array,
    SFS_FIND_DATA_T *find_data
)
{
    uint16          file_name_len = 0;
    FILEARRAY_DATA_T  *file_data_ptr = PNULL;
    MMIFMM_SEARCH_SRV_HANDLE_T *search_handle = (MMIFMM_SEARCH_SRV_HANDLE_T *) handle;

    if ( (PNULL == find_path) || (PNULL == find_data))
    {
        //SCI_TRACE_LOW:"SearchFileAndSaveFolder_CallBack param error!"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_INTERFACE_2554_112_2_18_2_20_24_307, (uint8*) "");
        return;
    }

    file_name_len = (uint16) MMIAPICOM_Wstrlen (find_data->name);

    if (FILEARRAY_FILENAME_MAX_LEN >= file_name_len
            && FMM_SEARCH_FILENAME_MAXLEN >= find_path_len + 1 + file_name_len)
    {
        //����Ŀ¼��������Ŀ¼������
        if (find_data->attr&SFS_ATTR_DIR)
        {
            //ȷ��������Ŀ¼�����������Ŀ¼��λ��, ���һ��λ��
            file_data_ptr = (FILEARRAY_DATA_T *) SCI_ALLOC_APP (sizeof (FILEARRAY_DATA_T));

            if (PNULL == file_data_ptr)
            {
                return;
            }

            SCI_MEMSET (file_data_ptr, 0x00, sizeof (FILEARRAY_DATA_T));
            //MMIAPIFILEARRAY_Read(s_foldre_farray, folder_num - 1, file_data_ptr);
            //folder_ptr = file_data_ptr->filename;

            //������������Ŀ¼������·����¼����
            MMI_WSTRNCPY (file_data_ptr->filename, FMM_SEARCH_FILENAME_MAXLEN,
                          find_path, find_path_len, find_path_len);
            file_data_ptr->filename[find_path_len] = MMIFILE_SLASH;
            MMI_WSTRNCPY (& (file_data_ptr->filename[find_path_len+1]), FMM_SEARCH_FILENAME_MAXLEN,
                          find_data->name, file_name_len, file_name_len);
            file_data_ptr->name_len = (uint16) (find_path_len + file_name_len + 1);

            if ( (!MMIAPICOM_Wstrcmp (file_data_ptr->filename, search_handle->s_default_path)))
            {
                //�û�Ԥ��·��
                file_data_ptr->type = FILEARRAY_TYPE_DEFAULT_FOLDER;
            }
            else
            {
                file_data_ptr->type = FILEARRAY_TYPE_FOLDER;
            }

            MMIAPIFILEARRAY_Add (search_handle->s_foldre_farray, file_data_ptr);
            SCI_FREE (file_data_ptr);
            file_data_ptr = PNULL;
            //search_folder_ptr->folder_num++;
        }

        SearchFileAndFolder_CallBack (search_handle, find_path, find_path_len, folder_array, find_data);

    }
}

/*****************************************************************************/
// Description : ��ָ��Ŀ¼�°�ɸѡ�������ļ����ļ���, ���ѽ����¼��file_array��
// Global resource dependence :
// Author: liqing.peng
// Note:
/*****************************************************************************/
LOCAL void SearchFileAndSaveFolder (
    MMIFMM_SRV_HANDLE handle,
    wchar         *find_path,
    uint16        find_path_len,
    FILEARRAY     file_array,
    FILEARRAY     folder_array,
    FILEARRAY     recurse_folder_array
)
{
    MMIFMM_SEARCH_SRV_HANDLE_T *search_handle = (MMIFMM_SEARCH_SRV_HANDLE_T *) handle;

    if ( (PNULL == find_path) || (PNULL == file_array))
    {
        //SCI_TRACE_LOW:"SearchFileAndSaveFolder param error!"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_INTERFACE_2615_112_2_18_2_20_24_308, (uint8*) "");
        return;
    }

    search_handle->s_farray_cbk = file_array;
    search_handle->s_foldre_farray = recurse_folder_array;

    XFindAllFiles (search_handle, find_path, find_path_len, folder_array, SearchFileAndSaveFolder_CallBack);
    search_handle->s_farray_cbk = NULL;
    search_handle->s_foldre_farray = NULL;
}

/*****************************************************************************/
// Description : SearchFileAndFolder���õ��Ļص�����
// Global resource dependence :
// Author: liqing.peng
// Note:
/*****************************************************************************/
LOCAL void SearchFileAndFolder_CallBack (
    MMIFMM_SRV_HANDLE handle,
    const wchar     *find_path,
    uint16          find_path_len,
    FILEARRAY       folder_array,
    SFS_FIND_DATA_T *find_data
)
{
    uint16          file_name_len = 0;
    BOOLEAN          need_add = FALSE;
    FILEARRAY_DATA_T    *f_array_data = NULL;//��Ϊmalloc����ռ䣬�����ջ���������

    MMIFMM_SEARCH_SRV_HANDLE_T *search_handle = (MMIFMM_SEARCH_SRV_HANDLE_T *) handle;

    if ( (PNULL == find_path) || (PNULL == find_data) || (PNULL == search_handle->s_farray_cbk))
    {
        //SCI_TRACE_LOW:"SearchFileAndFolder_CallBack param error!"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_INTERFACE_2643_112_2_18_2_20_24_309, (uint8*) "");
        return;
    }

    //alloc memory
    f_array_data = (FILEARRAY_DATA_T *) SCI_ALLOC_APP (sizeof (FILEARRAY_DATA_T));

    if (PNULL == f_array_data)
    {
        //SCI_TRACE_LOW:"MMIFMM:SearchFileAndFolder_CallBack error! PNULL == f_array_data"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_INTERFACE_2651_112_2_18_2_20_24_310, (uint8*) "");
        return;
    }

    SCI_MEMSET (f_array_data, 0, sizeof (FILEARRAY_DATA_T));

    //�������ļ����ļ���
    file_name_len = (uint16) MMIAPICOM_Wstrlen (find_data->name);

    if (FILEARRAY_FILENAME_MAX_LEN >= file_name_len
            && FMM_SEARCH_FILENAME_MAXLEN >= find_path_len + 1 + file_name_len)
    {
        if (folder_array != PNULL && (find_data->attr & SFS_ATTR_DIR) && (FUNC_FIND_FOLDERS_WITH_SUBFILES == search_handle->s_func_type))
        {
            MMI_WSTRNCPY (f_array_data->filename, FILEARRAY_FILENAME_MAX_LEN,
                          find_path, find_path_len, find_path_len);
            f_array_data->filename[find_path_len] = MMIFILE_SLASH;
            MMI_WSTRNCPY ( (f_array_data->filename + find_path_len + 1), FILEARRAY_FILENAME_MAX_LEN,
                           find_data->name, file_name_len, file_name_len);
            f_array_data->name_len = (uint16) (find_path_len + 1 + file_name_len);
            //��Ҫ�����ļ����ļ��з�������array

            if (MMIFMM_IsFilesInFolder (f_array_data->filename, MMIAPICOM_Wstrlen (f_array_data->filename), search_handle->s_filter_str)) //�ж��Ƿ��з����������ļ�
            {
                if ( (!MMIAPICOM_Wstrcmp (f_array_data->filename, search_handle->s_default_path)))
                {
                    //�û�Ԥ��·��
                    f_array_data->type = FILEARRAY_TYPE_DEFAULT_FOLDER;
                }
                else
                {
                    f_array_data->type = FILEARRAY_TYPE_FOLDER;
                }

                f_array_data->size = find_data->length;
                f_array_data->modify_date.year = find_data->modify_Date.year;
                f_array_data->modify_date.mon  = find_data->modify_Date.mon;
                f_array_data->modify_date.mday = find_data->modify_Date.mday;

                f_array_data->modify_time.hour = find_data->modify_time.hour;
                f_array_data->modify_time.min  = find_data->modify_time.min;
                f_array_data->modify_time.sec  = find_data->modify_time.sec;

                f_array_data->create_date.year = find_data->create_Date.year;
                f_array_data->create_date.mon  = find_data->create_Date.mon;
                f_array_data->create_date.mday = find_data->create_Date.mday;

                f_array_data->create_time.hour = find_data->create_time.hour;
                f_array_data->create_time.min  = find_data->create_time.min;  
                f_array_data->create_time.sec  = find_data->create_time.sec;
                MMIAPIFILEARRAY_Add (folder_array, f_array_data);
            }

        }
        else if (//IsMatchCurFilterStr(find_data->name, file_name_len)&&
            ( (FUNC_FIND_FILE == search_handle->s_func_type) && ! (find_data->attr & SFS_ATTR_DIR)
              || (FUNC_FIND_FOLDER == search_handle->s_func_type) && (find_data->attr & SFS_ATTR_DIR)
              || (FUNC_FIND_FILE_AND_FOLDER == search_handle->s_func_type)
              || (FUNC_FIND_FOLDERS_WITH_SUBFILES == search_handle->s_func_type) && ! (find_data->attr & SFS_ATTR_DIR)
              || (FUNC_FIND_FILE_AND_ALLFOLER == search_handle->s_func_type) && ! (find_data->attr & SFS_ATTR_DIR))
            || (FUNC_FIND_FILE_AND_ALLFOLER == search_handle->s_func_type) && (find_data->attr & SFS_ATTR_DIR))
        {
            //������д�뵽file_array��
            MMI_WSTRNCPY (f_array_data->filename, FILEARRAY_FILENAME_MAX_LEN,
                          find_path, find_path_len, find_path_len);
            f_array_data->filename[find_path_len] = MMIFILE_SLASH;
            MMI_WSTRNCPY ( (f_array_data->filename + find_path_len + 1), FILEARRAY_FILENAME_MAX_LEN,
                           find_data->name, file_name_len, file_name_len);
            f_array_data->name_len = (uint16) (find_path_len + 1 + file_name_len);

#ifdef DRM_SUPPORT

            if (MMIAPIFMM_IsDrmFileBySuffix (find_data->name, MMIAPICOM_Wstrlen (find_data->name)))
            {
                //�������ѷ��ļ���
                if (MMIAPICOM_Wstrlen (search_handle->s_search_str) == 0)	//���ַ������������������
                {
                    if (! (find_data->attr & SFS_ATTR_DIR))
                    {
                        MMIFILE_HANDLE drm_handle = 0;
                        DRM_RIGHTS_T rights_info = {0};
                        uint32 suffix_type = search_handle->s_filter_type;
                        uint32 drm_suffix_type = 0;

                        drm_handle = SFS_CreateFile (f_array_data->filename, SFS_MODE_READ | SFS_MODE_OPEN_EXISTING, 0, 0);

                        if (PNULL == drm_handle)
                        {
                            //��ʧ�ܣ������ԭ�д���
                            MMIAPIFMM_CloseFile (drm_handle);
                            SCI_Free (f_array_data);
                            return;
                        }

                        //��drm�ļ�
                        if (MMIAPIDRM_IsDRMFile (drm_handle, PNULL))
                        {
                            do
                            {
                                /*��DRM�ļ�����������ļ���mine type*/
                                SCI_MEMSET (&rights_info, 0, sizeof (DRM_RIGHTS_T));
                                MMIAPIDRM_GetRightsinfo (drm_handle, PNULL, &rights_info);
                                drm_suffix_type = MMIFMM_ConvertMimeTypeToSuffix (rights_info.mime_type);
                                suffix_type &= ~MMIFMM_DM_FILE;
                                suffix_type &= ~MMIFMM_DCF_FILE;

                                //������������������ֻ��Ҫ������λ���ɣ�
                                //����������չ�󣬺���λ����Ψһ���𣬹�ǰ���8λ�����ټ����
                                if (MMIFMM_FILE_ALL != search_handle->s_filter_type
                                        && ( (MMIFMM_MEDIA_TYPE (suffix_type) & MMIFMM_MEDIA_TYPE (drm_suffix_type)) == 0//С��λ����ͬ
                                             || (MMIFMM_FILE_TYPE (suffix_type) != MMIFMM_FILE_TYPE (drm_suffix_type)) //���಻���
                                           ))
                                {
                                    //drm ��֧���û����õ�����
                                    MMIAPIFMM_CloseFile (drm_handle);
                                    SCI_Free (f_array_data);
                                    return;
                                }
                            }
                            while (0);
                        }

                        MMIAPIFMM_CloseFile (drm_handle);
                    }
                }
            }

#endif
            f_array_data->size = find_data->length;
            f_array_data->modify_date.year = find_data->modify_Date.year;
            f_array_data->modify_date.mon  = find_data->modify_Date.mon;
            f_array_data->modify_date.mday = find_data->modify_Date.mday;

            f_array_data->modify_time.hour = find_data->modify_time.hour;
            f_array_data->modify_time.min  = find_data->modify_time.min;
            f_array_data->modify_time.sec  = find_data->modify_time.sec;

            f_array_data->create_date.year = find_data->create_Date.year;
            f_array_data->create_date.mon  = find_data->create_Date.mon;
            f_array_data->create_date.mday = find_data->create_Date.mday;

            f_array_data->create_time.hour = find_data->create_time.hour;
            f_array_data->create_time.min  = find_data->create_time.min;
            f_array_data->create_time.sec  = find_data->create_time.sec;

            if (find_data->attr & SFS_ATTR_DIR)
            {
                f_array_data->type = FILEARRAY_TYPE_FOLDER;

                if (MMIAPICOM_Wstrlen (search_handle->s_search_str) > 0)
                {
                    //MMIAPICOM_WstrnstrCaseIntensive(find_data->name, file_name_len, s_search_str, MMIAPICOM_Wstrlen(s_search_str)))
                    if (unistrcmp_ext (search_handle->s_filter_str , find_data->name, FALSE)) /*lint !e746 !e718*/
                    {
                        need_add = TRUE;
                    }
                }
                else
                {
                    need_add = TRUE;
                }
            }
            else
            {
                f_array_data->type = FILEARRAY_TYPE_FILE;
                need_add = TRUE;
            }

            if (need_add)
            {
                MMIAPIFILEARRAY_Add (search_handle->s_farray_cbk, f_array_data);
            }
        }
    }

    //free memory
    SCI_FREE (f_array_data);
}

/*****************************************************************************/
// Description : ��ָ��Ŀ¼�°�ɸѡ�������ļ��������ļ���, ���ѽ����¼��file_array��
// Global resource dependence :
// Author: liqing.peng
// Note:
/*****************************************************************************/
LOCAL void SearchFileAndFolder (
    MMIFMM_SRV_HANDLE handle,
    wchar         *find_path,
    uint16        find_path_len,
    FILEARRAY     folder_array,
    FILEARRAY     file_array
)
{
    MMIFMM_SEARCH_SRV_HANDLE_T *search_handle = (MMIFMM_SEARCH_SRV_HANDLE_T *) handle;

    if ( (PNULL == find_path) || (PNULL == file_array))
    {
        //SCI_TRACE_LOW:"MMIFMM:SearchFileAndFolder param error!"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_INTERFACE_2815_112_2_18_2_20_25_311, (uint8*) "");
        return;
    }

    search_handle->s_farray_cbk = file_array;

    XFindAllFiles (search_handle, find_path, find_path_len, folder_array, SearchFileAndFolder_CallBack);

    search_handle->s_farray_cbk = NULL;
}

/*****************************************************************************/
//  Description : set param before search
//  Global resource dependence :
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
LOCAL void InitSearchParam (
    MMIFMM_SRV_HANDLE               handle,
    const wchar                     *path_ptr,          //in, ����Ŀ¼���������NULL�����������д洢��
    uint16                          path_len,
    MMIFMM_FILTER_T                 *filter_ptr    //ɸѡ����

)
{
    MMIFMM_SEARCH_SRV_HANDLE_T *search_handle = (MMIFMM_SEARCH_SRV_HANDLE_T *) handle;

    //SCI_ASSERT(NULL != filter_ptr);
    if (PNULL == filter_ptr)
    {
        //SCI_TRACE_LOW:"InitSearchParam param error:PNULL == filter_ptr"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_INTERFACE_3634_112_2_18_2_20_26_321, (uint8*) "");
        return;
    }

    //������������
    if (filter_ptr != &search_handle->s_filter)
    {
        SCI_MEMSET (&search_handle->s_filter, 0, sizeof (search_handle->s_filter));
        MMI_MEMCPY (search_handle->s_filter.filter_str, FMM_FILTER_STRING_MAXLEN,
                    filter_ptr->filter_str, strlen ( (char*) filter_ptr->filter_str), strlen ( (char*) filter_ptr->filter_str));/*lint !e666*/
    }

    if (!ParseFilterStr (filter_ptr, search_handle))
    {
        return;
    }

    //��¼������Ŀ¼
    if (NULL != path_ptr)
    {
        if (path_ptr != search_handle->s_cur_path)
        {
            SCI_MEMSET (search_handle->s_cur_path, 0, sizeof (search_handle->s_cur_path));/*lint !e666*/
            MMI_WSTRNCPY (search_handle->s_cur_path, FMM_SEARCH_FILENAME_MAXLEN,
                          path_ptr, path_len, path_len);
            search_handle->s_cur_path_len = path_len;
        }
    }
    else
    {
        SCI_MEMSET (search_handle->s_cur_path, 0, sizeof (search_handle->s_cur_path));
        search_handle->s_cur_path_len = 0;
    }
}


/*****************************************************************************/
// Description : xfindall files
// Global resource dependence :
// Author: jian.ma
// Note:
/*****************************************************************************/
LOCAL void XFindAllFiles (
    MMIFMM_SRV_HANDLE handle,
    wchar *find_path,
    uint16 find_path_len,
    FILEARRAY folder_array,
    FindAllFiles_CallBack callback
)
{
    MMIFMM_SEARCH_SRV_HANDLE_T *search_handle = (MMIFMM_SEARCH_SRV_HANDLE_T *) handle;

#if defined MMIFMM_XFIND_SUPPORT
    MMIFMM_XFIND_T *xfind_param = PNULL;
    
    xfind_param = (MMIFMM_XFIND_T *)SCI_ALLOC_APPZ(sizeof(MMIFMM_XFIND_T));
    xfind_param->filter = search_handle->s_filter_str;
    MMI_WSTRNCPY (xfind_param->find_path, FMM_SEARCH_FILENAME_MAXLEN,find_path, find_path_len,MIN(FMM_SEARCH_FILENAME_MAXLEN,find_path_len));
    xfind_param->sort_type = search_handle->s_sort_type;
    xfind_param->func_type = search_handle->s_func_type;
    xfind_param->is_recurse = search_handle->s_is_recurse;
    xfind_param->compareFunc_cbk = search_handle->s_xfind_compareFunc_cbk;
    xfind_param->getKey_cbk = search_handle->s_xfind_getKey_cbk;

    //if current no xfind search and xfind open success
    if ( (FILEARRAY_SORT_NULL != xfind_param->func_type)
            && XFindQuickSearch (search_handle, xfind_param))
    {
        MMIFMM_XFIND_INFO_T *xfind_info_ptr = PNULL;
        xfind_info_ptr = (MMIFMM_XFIND_INFO_T *)SCI_ALLOC_APPZ(sizeof(MMIFMM_XFIND_INFO_T));
        xfind_info_ptr->callback = callback;
        xfind_info_ptr->folder_array = folder_array;
        xfind_info_ptr->find_path_len = find_path_len;
        SCI_MEMSET (xfind_info_ptr->find_path, 0x00, (sizeof (wchar) *FMM_SEARCH_FILENAME_MAXLEN));
        MMIAPICOM_Wstrncpy (xfind_info_ptr->find_path, find_path, find_path_len);

        XFindHandleFindCallback (search_handle, xfind_info_ptr);
        SCI_FREE(xfind_info_ptr);
    }
    else
#endif
    {
        //search all file and no sort
        FindAllFiles (search_handle, find_path, find_path_len, folder_array, callback);
    }
#if defined MMIFMM_XFIND_SUPPORT
    SCI_FREE(xfind_param);
#endif
    
}



/*****************************************************************************/
// Description : ��ָ��Ŀ¼�����������ļ�
// Global resource dependence :
// Author: liqing.peng
// Note:
/*****************************************************************************/
LOCAL void FindAllFiles (
    MMIFMM_SRV_HANDLE handle,
    wchar       *find_path,
    uint16      find_path_len,
    FILEARRAY   folder_array,
    FindAllFiles_CallBack   callback
)
{
    SFS_HANDLE      file_handle = NULL;
    wchar           *find = NULL;//��Ϊmalloc����ռ䣬�����ջ���������
    SFS_FIND_DATA_T *find_data = NULL;//��Ϊmalloc����ռ䣬�����ջ���������
    uint16          ctlStr_d[] = L"/A:-H-S/D:-H-S";
    uint16          ctlStr_a[] = L"/A:-H-S";
    SFS_ERROR_E     result = SFS_NO_ERROR;
    uint16          path_depth = 0;

    MMIFMM_SEARCH_SRV_HANDLE_T *search_handle = (MMIFMM_SEARCH_SRV_HANDLE_T *) handle;

    if ( (PNULL == find_path) || (PNULL == callback))
    {
        //SCI_TRACE_LOW:"FindAllFiles param error!"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_INTERFACE_3471_112_2_18_2_20_26_317, (uint8*) "");
        return;
    }

    //alloc memory
    find_data = (SFS_FIND_DATA_T *) SCI_ALLOC_APP (sizeof (SFS_FIND_DATA_T));

    //SCI_ASSERT(NULL != find_data);
    if (PNULL == find_data)
    {
        //SCI_TRACE_LOW:"MMIFMM:FindAllFiles error:PNULL == find_data"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_INTERFACE_3480_112_2_18_2_20_26_318, (uint8*) "");
        return;
    }

    SCI_MEMSET (find_data, 0, sizeof (SFS_FIND_DATA_T));

    find = (wchar *) SCI_ALLOC_APP ( (FMM_SEARCH_FILENAME_MAXLEN + 3) * sizeof (wchar));

    //SCI_ASSERT(NULL != find);
    if (PNULL == find)
    {
        //SCI_TRACE_LOW:"MMIFMM:FindAllFiles error:PNULL == find"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_INTERFACE_3489_112_2_18_2_20_26_319, (uint8*) "");
        return;
    }

    SCI_MEMSET (find, 0, ( (FMM_SEARCH_FILENAME_MAXLEN + 3) * sizeof (wchar)));

    //������������
    MMI_WSTRNCPY (find, FMM_SEARCH_FILENAME_MAXLEN,
                  find_path, find_path_len, find_path_len);

    //�����ļ�����
    if (FUNC_FIND_FILE != search_handle->s_func_type || search_handle->s_is_recurse) //�����������ֻ�����ļ�or����������ļ���������Ҫ������Ŀ¼�ļ�
    {
        result = SFS_FindFirstFileCtrl (find, search_handle->s_filter_str, ctlStr_d, &file_handle, find_data, PNULL);
    }
    else
    {
        result = SFS_FindFirstFileCtrl (find, search_handle->s_filter_str, ctlStr_a, &file_handle, find_data, PNULL);
    }

    path_depth = (uint16) MMIFMM_GetPathDepth (find, (uint16) MMIAPICOM_Wstrlen (find));

    if (NULL != file_handle && SFS_NO_ERROR == result)
    {
#ifdef  SQLITE_FILEARRAY_SUPPORT
        MMIAPIFILEARRAY_Transaction (search_handle->s_farray_cbk, TRUE);
        MMIAPIFILEARRAY_Transaction (search_handle->s_foldre_farray, TRUE);
#endif

        do
        {
            if ( (find_data->attr & SFS_ATTR_DIR) > 0)
            {
                // Ŀ¼����
                if (MMIFMM_PATH_DEPTH <= path_depth)
                {
                    //�Ѿ��ﵽ���Ŀ¼���
                    continue;
                }
            }

            callback (search_handle, find_path, find_path_len, folder_array, find_data);

            if (MMIFMM_IsSearchingStopped())
            {
                //�û�Ҫ��ֹͣ����
                break;
            }
        } //������һ������

        while (SFS_NO_ERROR == SFS_FindNextFile (file_handle, find_data));

#ifdef  SQLITE_FILEARRAY_SUPPORT
        MMIAPIFILEARRAY_Transaction (search_handle->s_foldre_farray, FALSE);
        MMIAPIFILEARRAY_Transaction (search_handle->s_farray_cbk, FALSE);
#endif
    }

    if (NULL != file_handle)
    {
        SFS_FindClose (file_handle);
    }

    SCI_FREE (find_data);
    SCI_FREE (find);

    if (0 != search_handle->s_cbk_win_id && search_handle->s_func_type == FUNC_FIND_FOLDERS_WITH_SUBFILES) //search on folderr
    {
        MMK_PostMsg (search_handle->s_cbk_win_id, MSG_FMM_SEARCH_ONE_FOLDER_CNF, PNULL, 0);
    }
}


/*****************************************************************************/
// Description : ��Դ�ַ����н��������ؼ��ַ���
// Global resource dependence :
// Author: liqing.peng
// Note:
/*****************************************************************************/
LOCAL BOOLEAN ParseFilterStr (
    MMIFMM_FILTER_T    *filter_ptr,
    MMIFMM_SRV_HANDLE   handle
)
{

    uint32 i = 0;
    MMIFMM_SEARCH_SRV_HANDLE_T *search_handle = (MMIFMM_SEARCH_SRV_HANDLE_T *) handle;

    if (PNULL == search_handle)
    {
        return FALSE;
    }

    SCI_MEMSET (search_handle->s_filter_str, 0x00, sizeof (search_handle->s_filter_str));
    MMIAPICOM_StrToWstr (filter_ptr->filter_str, search_handle->s_filter_str);

    for (i = 0; i < sizeof (search_handle->s_filter_str) / sizeof (wchar); i++)
    {
        if (' ' == search_handle->s_filter_str[i])
        {
            search_handle->s_filter_str[i] = '|';
        }
    }

#ifdef DRM_SUPPORT
    //ͨ����׺�ַ�����ú�׺������ֵ�����ڲ��ҽ���Ĺ���
    MMIFMM_GetSuffixTypeInfo ( (char*) filter_ptr->filter_str, &search_handle->s_filter_type);
#endif

    return TRUE;
}

/*****************************************************************************/
//  Description : check search file stopped or not
//  Global resource dependence :
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIFMM_IsSearchingStopped (void)

{
    BOOLEAN result = FALSE;

    if (NULL != s_is_stopfindfiles_ptr && (*s_is_stopfindfiles_ptr))
    {
        result = TRUE;
    }

    return result;
}


/*****************************************************************************/
//  Description : search file service
//  Global resource dependence : none
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIFMM_SearchFileSrv (
    MMIFMM_SRV_HANDLE handle,
    MMIFMM_SEARCH_FILE_INFO_T *search_file_info
)

{
    MMIFMM_SEARCH_SRV_HANDLE_T *search_handle = (MMIFMM_SEARCH_SRV_HANDLE_T *) handle;

    SCI_TRACE_LOW("MMIFMMSRV: MMIFMM_SearchFileSrv search_handle 0x%x",search_handle);

    if (PNULL == search_file_info)
    {
        SCI_TRACE_LOW("MMIFMMSRV:MMIFMM_SearchFileSrv PNULL == search_file_info!");
        return FALSE;
    }

    if (PNULL == handle)
    {
        SCI_TRACE_LOW("MMIFMMSRV:MMIFMM_SearchFileSrv PNULL == handle!");
        return FALSE;
    }
    if (SearchFileInPathByString == search_file_info->search_type)
    {
        uint16 str_len = 0;

        if (NULL != search_file_info->path_ptr)
        {
            if (search_file_info->path_ptr != search_handle->s_cur_path)
            {
                SCI_MEMSET (search_handle->s_cur_path, 0, sizeof (search_handle->s_cur_path));
                MMI_WSTRNCPY (search_handle->s_cur_path, FMM_SEARCH_FILENAME_MAXLEN,
                              search_file_info->path_ptr, search_file_info->path_len, search_file_info->path_len);
                search_handle->s_cur_path_len = search_file_info->path_len;
            }
        }
        else
        {
            SCI_MEMSET (search_handle->s_cur_path, 0, sizeof (search_handle->s_cur_path));
            search_handle->s_cur_path_len = 0;
        }

        SCI_MEMSET (search_handle->s_search_str, 0x00, sizeof (search_handle->s_search_str));
        //��������filter
        SCI_MEMSET (search_handle->s_filter_str, 0x00, sizeof (search_handle->s_filter_str));

        if (PNULL != search_file_info->search_str && MMIAPICOM_Wstrlen (search_file_info->search_str) > 0)
        {
            //search string  -->"*str*"
            if (MMIAPICOM_Wstrlen (search_file_info->search_str) < FMM_FILTER_STRING_MAXLEN - 2)
            {
                search_handle->s_filter_str[0] = '*';
                str_len = 1;
            }

            MMIAPICOM_Wstrncpy (&search_handle->s_filter_str[str_len], search_file_info->search_str, MIN (MMIAPICOM_Wstrlen (search_file_info->search_str), FMM_FILTER_STRING_MAXLEN - (uint32) str_len));/*lint !e666*/
            str_len = (uint16) MMIAPICOM_Wstrlen (search_handle->s_filter_str);

            if (str_len < FMM_FILTER_STRING_MAXLEN - 1)
            {
                search_handle->s_filter_str[str_len] = '*';
                str_len++;
            }

            //save search string
            MMIAPICOM_Wstrncpy (search_handle->s_search_str, search_file_info->search_str, MIN (MMIAPICOM_Wstrlen (search_file_info->search_str), FMM_FILTER_STRING_MAXLEN));/*lint !e666*/
        }
    }
    else
    {
        InitSearchParam (search_handle,
                         search_file_info->path_ptr,
                         search_file_info->path_len,
                         search_file_info->filter_ptr
                        );

        if (PNULL != search_file_info->default_path)
        {
            MMIAPICOM_Wstrncpy (search_handle->s_default_path, search_file_info->default_path,MIN(MMIAPICOM_Wstrlen (search_file_info->default_path),FMM_SEARCH_FILENAME_MAXLEN));/*lint !e666*/
        }
    }

    //������������
    search_handle->s_is_recurse = search_file_info->is_recurse;
    search_handle->s_func_type = search_file_info->func_type;
    search_handle->s_file_array_search = search_file_info->file_array;
    search_handle->s_cbk_win_id = search_file_info->win_id;
    search_handle->s_cbk_msg_id = search_file_info->msg_id;
    search_handle->s_sort_type = search_file_info->sort_type;
    search_handle->search_cbk = search_file_info->search_cbk;
#ifdef MMIFMM_XFIND_SUPPORT
    search_handle->s_xfind_getKey_cbk = search_file_info->xfind_getKey;
    search_handle->s_xfind_compareFunc_cbk = search_file_info->xfind_compareFunc;
#endif
    MMK_CheckAllocatedMemInfo( _D_FILE_NAME, _D_FILE_LINE );
    SearchFile ( (MMIFMM_SRV_HANDLE) search_handle);
    MMK_CheckAllocatedMemInfo( _D_FILE_NAME, _D_FILE_LINE );

    return TRUE;
}
/*****************************************************************************/
//  Description : create search service
//  Global resource dependence : none
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC MMIFMM_SRV_HANDLE MMIFMM_CreateSrv (
                                            const char   *file,
                                            uint32       line
                                            )
{
    MMIFMM_SEARCH_SRV_HANDLE_T *search_handle = PNULL;

    //search_handle = SCI_ALLOC_APPZ (sizeof (MMIFMM_SEARCH_SRV_HANDLE_T));
    search_handle = SCI_MallocApp(sizeof(MMIFMM_SEARCH_SRV_HANDLE_T),file,line);
    if (PNULL != search_handle)
    {
        SCI_MEMSET(search_handle,0x00,sizeof(MMIFMM_SEARCH_SRV_HANDLE_T));
    }
    SCI_TRACE_LOW("MMIFMMSRV: MMIFMM_CreateSrv search_handle 0x%x",search_handle);
    return search_handle;
}

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
                               )
{
    MMIFMM_SEARCH_SRV_HANDLE_T* search_handle = (MMIFMM_SEARCH_SRV_HANDLE_T*) handle;
    SCI_TRACE_LOW("MMIFMMSRV: MMIFMM_DestroySrv search_handle 0x%x",search_handle);
    SCI_FREE (search_handle);
}
