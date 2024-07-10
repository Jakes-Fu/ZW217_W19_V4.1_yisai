/*****************************************************************************
** File Name:               mmiap_list_manager.c                             *
** Author:                                                                   *
** Date:           2012/7/19                                                 *
** Copyright:      2012 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe ap control                  *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME              DESCRIPTION                              *
** 2012/7/19      aoke.hu           Create                                   *
******************************************************************************/

#define _MMIAP_LIST_MANAGER_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#ifdef WIN32
#include "std_header.h"
#endif
#include "sci_types.h"
#include "mmk_app.h"
#include "mmi_filemgr.h"
#include "mmiap_list.h"
#include "mmiap_list_manager.h"
#include "mmi_arraylist.h"
#include "mmisrvfmm_export.h"
#include "mmiap_musicarray.h"
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define  APLAYER_ALL_LIST_INFO_FILE_NAME  L"listinfo.data"
#define  APLAYER_ALL_LIST_INFO_FILE_NAME_LEN 13
#define  APLAYER_ALL_LIST_FILE_NUM_MAX 600


/**--------------------------------------------------------------------------*
 **                         LOCAL DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/
typedef struct
{
    APLAYER_LIST_TYPE_E   list_type;
    APLAYER_LIST_HANDLE list_handle;
    wchar               list_file_name[MMIFILE_FULL_PATH_MAX_LEN+1];
    uint16              list_file_name_len;
    uint32              open_lock;
    APLAYER_LIST_SORT_TYPE_E sort_type;
} APLAYER_LIST_INFO_T;


/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/
LOCAL void*  s_all_array_list_handle = PNULL;
/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : set list file name
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SetListFilePathName(uint16 list_index,
                                  wchar * full_path_ptr,
                                  uint16 full_path_len);

/*****************************************************************************/
//  Description : get list full path name
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetListFilePathName(uint16 list_index,
                                  wchar * full_path_ptr,
                                  uint16 * full_path_len_ptr);

/*****************************************************************************/
//  Description : set list handle
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SetListHandle(uint16 list_index,
                            APLAYER_LIST_HANDLE list_handle);

/*****************************************************************************/
//  Description : get list handle
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL APLAYER_LIST_HANDLE GetListHandle(uint16 list_index);

/*****************************************************************************/
//  Description : ungister list
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void UnregisterList(uint16 list_index);


/*****************************************************************************/
//  Description : getfile full path
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetFileFullPathForRead(wchar *file_name_ptr,
                                     uint16 file_name_len,
                                     wchar *full_path_ptr,
                                     uint16 *full_path_len_ptr);

/*****************************************************************************/
//  Description : get list file full path
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetFileFullPathForWrite(wchar *file_name_ptr,
                                      uint16 file_name_len,
                                      uint32  min_space,
                                      wchar *full_path_ptr,
                                      uint16 *full_path_len_ptr);


/*****************************************************************************/
//  Description : save all list info to file
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SaveAllListInfoFile(void);

/*****************************************************************************/
//  Description : save  list info to file
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SaveListFile(uint16 list_index,
                           APLAYER_LIST_HANDLE list_handle);

/*****************************************************************************/
//  Description : load all list info from file
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN LoadAllListInfoFile(void);

/*****************************************************************************/
//  Description : get list type
//  Global resource dependence :
//  Author:
//  Note: 注册
/*****************************************************************************/
LOCAL APLAYER_LIST_TYPE_E GetListType(uint16 list_index);


/*****************************************************************************/
//  Description : MMIAPlayer_GetListSortType
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL APLAYER_LIST_SORT_TYPE_E GetListSortType(uint16 list_index);

/*****************************************************************************/
//  Description : register list
//  Global resource dependence :
//  Author:
//  Note: 注册
/*****************************************************************************/
LOCAL BOOLEAN RegistNewList(wchar *full_path_ptr,
                            uint16 full_path_len,
                            APLAYER_LIST_TYPE_E list_type,
                            uint16 * list_index_ptr);

/*****************************************************************************/
//  Description : CloseAndFreeList
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CloseAndFreeList(uint16 list_index,
                               BOOLEAN is_save_list_file,
                               BOOLEAN is_free_list_always);

/*****************************************************************************/
//  Description : create list
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL APLAYER_LIST_ERROR_E CreateNewListByFullPath(wchar *full_path_ptr,
        uint16 full_path_len,
        uint16 * list_index_ptr);

/*****************************************************************************/
//  Description : DeleteListOpenLock
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL uint32 DeleteListOpenLock(uint16 list_index);

/*****************************************************************************/
//  Description : AddListOpenLock
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void AddListOpenLock(uint16 list_index);

/*****************************************************************************/
//  Description : SetListSortType
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SetListSortType(uint16 list_index,
                              APLAYER_LIST_SORT_TYPE_E sort_type);

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : load all list info from file
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN LoadAllListInfoFile(void)
{
    wchar full_path[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
    uint16 full_path_len = MMIFILE_FULL_PATH_MAX_LEN;
    BOOLEAN result = FALSE;
    uint32 i = 0;
    uint32  total_num = 0;

    result = GetFileFullPathForRead(APLAYER_ALL_LIST_INFO_FILE_NAME,APLAYER_ALL_LIST_INFO_FILE_NAME_LEN,full_path,&full_path_len);

    if (result)
    {
        result = MMIAPIFMM_IsFileExist(full_path,full_path_len);
        if (result)
        {
            result = MMIAPLAYERARRAY_LoadFromFile(s_all_array_list_handle,full_path,full_path_len);
            if (result)
            {
                total_num = MMIAPLAYERARRAY_GetTotalNum(s_all_array_list_handle);
                if(total_num > AP_MUSIC_LIST_NUM_MAX)
				{
					total_num = AP_MUSIC_LIST_NUM_MAX;
				}
                for (i = 0; i < total_num; i++)
                {
                    SetListHandle(i,PNULL);
                }
            }
        }
    }

    SCI_TRACE_LOW("[APlayer] LoadAllListInfoFile result = %d", result);
    return result;
}

/*****************************************************************************/
//  Description : save  list info to file
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPLAYER_SaveListFile(uint16 list_index)
{
    APLAYER_LIST_HANDLE list_handle = PNULL;

    list_handle = GetListHandle(list_index);

    return SaveListFile(list_index,list_handle);
}

/*****************************************************************************/
//  Description : save  list info to file
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SaveListFile(uint16 list_index,
                           APLAYER_LIST_HANDLE list_handle)
{
    wchar    full_path[MMIFILE_FULL_PATH_MAX_LEN+1] = {0};
    uint16   full_path_len = MMIFILE_FULL_PATH_MAX_LEN;
    BOOLEAN  result = FALSE;

    if(GetListFilePathName(list_index, full_path, &full_path_len))
    {
        result = MMIAPlayer_LIST_SaveFile(list_handle,full_path,full_path_len);
    }
    return result;

}

/*****************************************************************************/
//  Description : save all list info to file
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SaveAllListInfoFile(void)
{
    BOOLEAN result = FALSE;
    wchar full_path[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
    uint16 full_path_len = MMIFILE_FULL_PATH_MAX_LEN;
    MMIFILE_HANDLE  sfs_handle = PNULL;


    if (PNULL != s_all_array_list_handle
            && GetFileFullPathForRead(APLAYER_ALL_LIST_INFO_FILE_NAME,
                                      APLAYER_ALL_LIST_INFO_FILE_NAME_LEN,
                                      full_path,&full_path_len))
    {
        if (!MMIAPIFMM_IsFileExist(full_path,full_path_len))
        {
            sfs_handle = MMIAPIFMM_CreateFile(full_path,SFS_MODE_WRITE|SFS_MODE_CREATE_ALWAYS,PNULL,PNULL);
            if (PNULL != sfs_handle)
            {
                MMIAPIFMM_CloseFile(sfs_handle);
            }
        }
        if(SFS_NO_ERROR == MMIAPLAYERARRAY_SaveArrayToFile(s_all_array_list_handle,sizeof(APLAYER_LIST_INFO_T),full_path,full_path_len))
        {
            result = TRUE;
        }
    }

    SCI_TRACE_LOW("[APlayer] SaveAllListInfoFile result = %d", result);
    return result;
}

/*****************************************************************************/
//  Description : init all list info
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPlayer_InitAllList(void)
{
    BOOLEAN result = FALSE;
    BOOLEAN is_list_exsit = TRUE;
    wchar full_path[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
    uint16 full_path_len = MMIFILE_FULL_PATH_MAX_LEN;

    if (PNULL != s_all_array_list_handle)
    {
        return;
    }

    s_all_array_list_handle = MMIAPLAYERARRAY_CreateArray(sizeof(APLAYER_LIST_INFO_T),_D_FILE_NAME,_D_FILE_LINE);

    if (PNULL != s_all_array_list_handle)
    {
        //创建默认列表，否则加载文件
        result = LoadAllListInfoFile();
        SCI_TRACE_LOW("[APlayer] MMIAPlayer_InitAllList result = %d", result);
//        if (!result)
        {
            if(!GetListFilePathName(0, full_path,&full_path_len))
            {
            is_list_exsit = FALSE;
            GetFileFullPathForRead(APlayer_FMM_TEMP_LIST_NAME,APlayer_FMM_TEMP_LIST_NAME_LEN,full_path,&full_path_len);
            RegistNewList(full_path,full_path_len,APLAYER_LIST_TYPE_FMM,PNULL);
            }
            //加载my music
            if(!GetListFilePathName(1, full_path,&full_path_len))
            {
            is_list_exsit = FALSE;
            GetFileFullPathForRead(APLAYER_MY_MUSIC_NAME,APLAYER_MY_MUSIC_NAME_LEN,full_path,&full_path_len);
            RegistNewList(full_path,full_path_len,APLAYER_LIST_TYPE_MYMUSIC,PNULL);
            }
            if(!GetListFilePathName(2, full_path,&full_path_len))
            {
            is_list_exsit = FALSE;
            GetFileFullPathForRead(APLAYER_RES_LIST_1_NAME,APLAYER_RES_LIST_1_NAME_LEN,full_path,&full_path_len);
            RegistNewList(full_path,full_path_len,APLAYER_LIST_TYPE_DEFAULT,PNULL);
            }
            if(!GetListFilePathName(3, full_path,&full_path_len))
            {
            is_list_exsit = FALSE;
            GetFileFullPathForRead(APLAYER_RES_LIST_2_NAME,APLAYER_RES_LIST_2_NAME_LEN,full_path,&full_path_len);
            RegistNewList(full_path,full_path_len,APLAYER_LIST_TYPE_DEFAULT,PNULL);
            }
            if(!GetListFilePathName(4, full_path,&full_path_len))
            {            
            is_list_exsit = FALSE;
            GetFileFullPathForRead(APLAYER_RES_LIST_3_NAME,APLAYER_RES_LIST_3_NAME_LEN,full_path,&full_path_len);
            RegistNewList(full_path,full_path_len,APLAYER_LIST_TYPE_DEFAULT,PNULL);
            }
            if(!is_list_exsit)
            {
            SaveAllListInfoFile();
            }
        }
    }
}

/*****************************************************************************/
//  Description : delete list file
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPlayer_DeleteListFile(uint16 list_index)
{
    wchar    full_path[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
    uint16   full_path_len = MMIFILE_FULL_PATH_MAX_LEN;


    MMIAPlayer_CloseList(list_index, FALSE);
    if (MMIAPlayer_GetListFileFullPath(list_index,full_path,&full_path_len))
    {
        MMIAPIFMM_DeleteFile(full_path,PNULL);
    }
}

/*****************************************************************************/
//  Description : destory all list info
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPlayer_ReleaseAllList(void)
{
    uint32 i = 0;
    uint32 total_num = 0;

    SCI_TRACE_LOW("[APlayer] MMIAPlayer_ReleaseAllList");
    if (PNULL != s_all_array_list_handle)
    {
        //关闭所有列表
        /*
        * 临时列表文件不保存
        */
        MMIAPlayer_DeleteListFile(APLAYER_LIST_TYPE_FMM);

        
        total_num = MMIAPlayer_GetTotalListNum();
        for (i = 1; i < total_num; i++)
        {
            CloseAndFreeList((uint16)i,TRUE,TRUE);
        }
        //保存文件
        SaveAllListInfoFile();
        MMIAPLAYERARRAY_DestroyArray(&s_all_array_list_handle);
        s_all_array_list_handle = PNULL;
    }
}

/*****************************************************************************/
//  Description : getfile full path
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetFileFullPathForRead(wchar *file_name_ptr,
                                     uint16 file_name_len,
                                     wchar *full_path_ptr,
                                     uint16 *full_path_len_ptr)
{


    if(PNULL != file_name_ptr
            && 0 != file_name_len
            && PNULL != full_path_ptr
            && PNULL != full_path_len_ptr)
    {

        SCI_MEMSET(full_path_ptr,0,(sizeof(wchar)*(*full_path_len_ptr)));
        if(MMIAPIFMM_GetSysFileFullPathForReadEx(
                    FALSE,
                    FALSE,
                    APLAYER_RES_FOLDER_NAME,
                    APLAYER_RES_FOLDER_NAME_LEN,
                    file_name_ptr,
                    file_name_len,
                    full_path_ptr,
                    full_path_len_ptr
                ))
        {
            return TRUE;
        }
    }
    return FALSE;
}

/*****************************************************************************/
//  Description : getfile full path
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetFileFullPathForWrite(wchar *file_name_ptr,
                                      uint16 file_name_len,
                                      uint32  min_space,
                                      wchar *full_path_ptr,
                                      uint16 *full_path_len_ptr)
{


    if(PNULL != file_name_ptr
            && 0 != file_name_len
            && PNULL != full_path_ptr
            && PNULL != full_path_len_ptr)
    {

        SCI_MEMSET(full_path_ptr,0,(sizeof(wchar)*(*full_path_len_ptr)));
        if(MMIAPIFMM_GetSysFileFullPathForWriteEx(
                    FALSE,
                    FALSE,
                    APLAYER_RES_FOLDER_NAME,
                    APLAYER_RES_FOLDER_NAME_LEN,
                    file_name_ptr,
                    file_name_len,
                    min_space,
                    full_path_ptr,
                    full_path_len_ptr
                ))
        {
            return TRUE;
        }
    }
    return FALSE;
}


/*****************************************************************************/
//  Description : ungister list
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void UnregisterList(uint16 list_index)
{
    APLAYER_LIST_INFO_T   list_info = {0};

    if (PNULL != s_all_array_list_handle)
    {
        if (MMIAPLAYERARRAY_Read(s_all_array_list_handle,list_index,&list_info))
        {
            SCI_TRACE_LOW("[APlayer] UnregisterList list_index = %d, list_info_ptr->list_type = %d", list_index, list_info.list_type);
            if (APLAYER_LIST_TYPE_USER_LIST == list_info.list_type)
            {

                if(MMIAPIFMM_IsFileExist(list_info.list_file_name,list_info.list_file_name_len))
                {
                    MMIAPIFMM_DeleteFile(list_info.list_file_name,PNULL);

                }
            }
        }
        MMIAPLAYERARRAY_Delete(s_all_array_list_handle,list_index);
    }
}

/*****************************************************************************/
//  Description : get list handle
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL APLAYER_LIST_HANDLE GetListHandle(uint16 list_index)
{
    APLAYER_LIST_HANDLE list_handle = PNULL;
    APLAYER_LIST_INFO_T   list_info = {0};

    if (PNULL != s_all_array_list_handle)
    {
        if (MMIAPLAYERARRAY_Read(s_all_array_list_handle,list_index,&list_info))
        {
            list_handle = list_info.list_handle;
        }
    }

    SCI_TRACE_LOW("[APlayer] GetListHandle list_index = %d, list_handle = %d", list_index, list_handle);

    return list_handle;
}

/*****************************************************************************/
//  Description : set list handle
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SetListHandle(uint16 list_index,
                            APLAYER_LIST_HANDLE list_handle)
{
    BOOLEAN result = FALSE;
    APLAYER_LIST_INFO_T  list_info = {0};

    if (PNULL != s_all_array_list_handle)
    {

        if (MMIAPLAYERARRAY_Read(s_all_array_list_handle,list_index,&list_info))
        {
            list_info.list_handle = list_handle;
            MMIAPLAYERARRAY_Modify(s_all_array_list_handle,list_index,&list_info);
            result = TRUE;
        }
    }
    SCI_TRACE_LOW("[APlayer] SetListHandle list_index = %d, list_handle = %d, result = %d", list_index, list_handle, result);
    return result;
}

/*****************************************************************************/
//  Description : get list full path name
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetListFilePathName(uint16 list_index,
                                  wchar * full_path_ptr,
                                  uint16 * full_path_len_ptr)
{
    BOOLEAN result = FALSE;
    APLAYER_LIST_INFO_T   list_info = {0};

    if (PNULL != s_all_array_list_handle
            && PNULL != full_path_len_ptr
            && PNULL != full_path_ptr)
    {

        if (MMIAPLAYERARRAY_Read(s_all_array_list_handle,list_index, &list_info))
        {
            SCI_MEMSET(full_path_ptr,0,(sizeof(wchar) *(*full_path_len_ptr)));
            *full_path_len_ptr = MIN(*full_path_len_ptr,list_info.list_file_name_len);
            MMIAPICOM_Wstrncpy(full_path_ptr,list_info.list_file_name,*full_path_len_ptr);
            result = TRUE;
        }
    }
    SCI_TRACE_LOW("[APlayer] GetListFilePathName list_index = %d,  result = %d", list_index, result);
    return result;

}

/*****************************************************************************/
//  Description : set list file name
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SetListFilePathName(uint16 list_index,
                                  wchar * full_path_ptr,
                                  uint16 full_path_len)
{
    BOOLEAN result = FALSE;
    APLAYER_LIST_INFO_T   list_info = {0};

    if (PNULL != s_all_array_list_handle
            && 0 != full_path_len
            && PNULL != full_path_ptr)
    {

        if (MMIAPLAYERARRAY_Read(s_all_array_list_handle,list_index, &list_info))
        {
            SCI_MEMSET(list_info.list_file_name,0,(sizeof(wchar)*(MMIFILE_FULL_PATH_MAX_LEN+1)));
            list_info.list_file_name_len = MIN(MMIFILE_FULL_PATH_MAX_LEN, full_path_len);
            MMIAPICOM_Wstrncpy(list_info.list_file_name,full_path_ptr,list_info.list_file_name_len);
            MMIAPLAYERARRAY_Modify(s_all_array_list_handle,list_index,&list_info);
            SaveAllListInfoFile();
            result = TRUE;
        }
    }
    SCI_TRACE_LOW("[APLAYER] SetListFilePathName list_index = %d, result = %d", list_index, result);
    return result;

}

/*****************************************************************************/
//  Description : get list type
//  Global resource dependence :
//  Author:
//  Note: 注册
/*****************************************************************************/
LOCAL APLAYER_LIST_TYPE_E GetListType(uint16 list_index)
{
    APLAYER_LIST_TYPE_E  list_type = APLAYER_LIST_TYPE_USER_LIST;
    APLAYER_LIST_INFO_T  list_info = {0};

    if (PNULL != s_all_array_list_handle)
    {
        if (MMIAPLAYERARRAY_Read(s_all_array_list_handle,list_index,&list_info))
        {
            list_type = list_info.list_type;
        }
    }

    SCI_TRACE_LOW("[APlayer] GetListFilePathName list_index = %d,  list_type = %d", list_index, list_type);
    return list_type;

}

/*****************************************************************************/
//  Description : get list type
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC APLAYER_LIST_TYPE_E MMIAPlayer_GetListType(uint16 list_index)
{
    return GetListType(list_index);
}

/*****************************************************************************/
//  Description : SetListSortType
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SetListSortType(uint16 list_index,
                              APLAYER_LIST_SORT_TYPE_E sort_type)
{
    APLAYER_LIST_INFO_T  list_info = {0};
    BOOLEAN     result = FALSE;

    if (PNULL != s_all_array_list_handle)
    {
        if (MMIAPLAYERARRAY_Read(s_all_array_list_handle,list_index, &list_info))
        {
            list_info.sort_type = sort_type;
            result = MMIAPLAYERARRAY_Modify(s_all_array_list_handle,list_index,&list_info);
            if (result)
            {
                SaveAllListInfoFile();
            }
        }
    }

    SCI_TRACE_LOW("[APlayer] GetListSortType list_index = %d,  sort_type = %d, result = %d", list_index, sort_type, result);
    return result;

}

/*****************************************************************************/
//  Description : MMIAPlayer_GetListSortType
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL APLAYER_LIST_SORT_TYPE_E GetListSortType(uint16 list_index)
{

    APLAYER_LIST_SORT_TYPE_E  sort_type = APLAYER_LSIT_SORT_NULL;
    APLAYER_LIST_INFO_T  list_info = {0};

    if (PNULL != s_all_array_list_handle)
    {
        if (MMIAPLAYERARRAY_Read(s_all_array_list_handle,list_index, &list_info))
        {
            sort_type = list_info.sort_type;
        }
    }

    SCI_TRACE_LOW("[APlayer] GetListSortType list_index = %d,  sort_type = %d", list_index, sort_type);
    return sort_type;

}

/*****************************************************************************/
//  Description : AddListOpenLock
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void AddListOpenLock(uint16 list_index)
{
    APLAYER_LIST_INFO_T  list_info = {0};

    if (PNULL != s_all_array_list_handle)
    {
        if (MMIAPLAYERARRAY_Read(s_all_array_list_handle,list_index,&list_info))
        {
            list_info.open_lock++;
            MMIAPLAYERARRAY_Modify(s_all_array_list_handle,list_index,&list_info);
        }
    }

}

/*****************************************************************************/
//  Description : DeleteListOpenLock
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL uint32 DeleteListOpenLock(uint16 list_index)
{
    APLAYER_LIST_INFO_T  list_info = {0};

    if (PNULL != s_all_array_list_handle)
    {
        if (MMIAPLAYERARRAY_Read(s_all_array_list_handle,list_index,&list_info))
        {
            if (0 < list_info.open_lock)
            {
                list_info.open_lock--;
            }
            MMIAPLAYERARRAY_Modify(s_all_array_list_handle,list_index,&list_info);
            return list_info.open_lock;
        }
    }
    return 0;

}

/*****************************************************************************/
//  Description : get all list num
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIAPlayer_GetTotalListNum(void)
{
    uint32 total_num = 0;

    if (PNULL != s_all_array_list_handle)
    {
        total_num = MMIAPLAYERARRAY_GetTotalNum(s_all_array_list_handle);
    }

    SCI_TRACE_LOW("[APlayer] GetListSortType total_num = %d", total_num);
    return total_num;
}

/*****************************************************************************/
//  Description : MIAPlayer_GetListFilePath
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPlayer_GetListFilePath(wchar *list_file_name_ptr,
        uint16 list_file_name_len,
        wchar *list_full_path_ptr,
        uint16 *list_full_path_len_ptr)
{
    BOOLEAN       result = FALSE;

    if (PNULL != list_file_name_ptr
            && 0 != list_file_name_len
            && PNULL != list_full_path_ptr
            && PNULL != list_full_path_len_ptr
            && 0 != *list_full_path_len_ptr)
    {
        result = GetFileFullPathForRead(list_file_name_ptr,list_file_name_len,list_full_path_ptr,list_full_path_len_ptr);
    }
    return result;
}

/*****************************************************************************/
//  Description : create list
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC APLAYER_LIST_ERROR_E MMIAPlayer_CreateNewListByFileName(wchar *list_file_name_ptr,
        uint16 list_file_name_len,
        uint16 * list_index_ptr)
{
    APLAYER_LIST_ERROR_E   result = APLAYER_LIST_ERROR;
    wchar         list_full_path[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
    uint16        list_full_path_len = MMIFILE_FULL_PATH_MAX_LEN;

    if (PNULL != list_file_name_ptr
            && 0 != list_file_name_len)
    {
        if(GetFileFullPathForWrite(list_file_name_ptr,list_file_name_len,APLAYER_LIST_MIN_MEM,list_full_path,&list_full_path_len))
        {
            result = CreateNewListByFullPath(list_full_path,list_full_path_len,list_index_ptr);
        }
        else
        {
            result = APLAYER_LIST_NO_SAPCE;
        }
    }

    SCI_TRACE_LOW("[APlayer] MMIAPlayer_CreateNewListByFileName result = %d", result);
    return result;
}

/*****************************************************************************/
//  Description : create list
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL APLAYER_LIST_ERROR_E CreateNewListByFullPath(wchar *full_path_ptr,
        uint16 full_path_len,
        uint16 * list_index_ptr)
{
    APLAYER_LIST_ERROR_E   result = APLAYER_LIST_ERROR;

    if (PNULL != full_path_ptr
            && 0 != full_path_len)
    {

        if (MMIAPIFMM_IsFileExist(full_path_ptr,full_path_len))
        {
            result = APLAYER_LIST_HAS_EXIST;
        }
        else
        {
            if(RegistNewList(full_path_ptr,full_path_len,APLAYER_LIST_TYPE_USER_LIST,list_index_ptr))
            {
                SaveAllListInfoFile();
                result = APLAYER_LIST_SUCCESS;
            }
        }

    }
    SCI_TRACE_LOW("[APlayer] MMIAPlayer_CreateNewListByFullPath result = %d", result);
    return result;
}

/*****************************************************************************/
//  Description : register list
//  Global resource dependence :
//  Author:
//  Note: 注册
/*****************************************************************************/
LOCAL BOOLEAN RegistNewList(wchar *full_path_ptr,
                            uint16 full_path_len,
                            APLAYER_LIST_TYPE_E list_type,
                            uint16 * list_index_ptr)
{
    BOOLEAN  result = FALSE;
    APLAYER_LIST_INFO_T   *list_info_ptr = PNULL;
    MMIFILE_HANDLE file_handle = 0;

    if (PNULL != full_path_ptr
            && 0 != full_path_len
            && PNULL != s_all_array_list_handle)
    {
        list_info_ptr = SCI_ALLOCA(sizeof(APLAYER_LIST_INFO_T));
        if (PNULL != list_info_ptr)
        {
            SCI_MEMSET(list_info_ptr,0,sizeof(APLAYER_LIST_INFO_T));

            list_info_ptr->list_handle = PNULL;
            list_info_ptr->list_type = list_type;
            list_info_ptr->list_file_name_len = MIN(full_path_len, MMIFILE_FULL_PATH_MAX_LEN);
            SCI_MEMSET(list_info_ptr->list_file_name,0,(sizeof(wchar)*(MMIFILE_FULL_PATH_MAX_LEN+1)));
            MMIAPICOM_Wstrncpy(list_info_ptr->list_file_name,full_path_ptr,list_info_ptr->list_file_name_len);
            if (!MMIAPIFMM_IsFileExist(full_path_ptr,full_path_len))
            {
                file_handle = MMIAPIFMM_CreateFile(
                                  full_path_ptr,
                                  SFS_MODE_WRITE|SFS_MODE_CREATE_ALWAYS, /*lint !e655*/
                                  NULL,
                                  NULL);
                if (0 != file_handle)
                {
                    MMIAPIFMM_CloseFile(file_handle);
                }
            }

            result = MMIAPLAYERARRAY_Add(s_all_array_list_handle,list_info_ptr);

            if (PNULL != list_index_ptr)
            {
                *list_index_ptr = MMIAPlayer_GetTotalListNum()-1;
            }
            SCI_FREE(list_info_ptr);
        }
    }

    SCI_TRACE_LOW("[APLAYER] RegistNewList result = %d, list_type = %d",result, list_type);
    return result;
}

/*****************************************************************************/
//  Description : destroy list
//  Global resource dependence :
//  Author:
//  Note: 卸载
/*****************************************************************************/
PUBLIC void MMIAPlayer_DestroyList(uint16 list_index)
{
    if(0 != GetListHandle(list_index))
    {
        MMIAPlayer_CloseList(list_index,TRUE);
    }
    UnregisterList(list_index);
    SaveAllListInfoFile();
}

/*****************************************************************************/
//  Description : delete audio file from list
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC APLAYER_LIST_ERROR_E MMIAPlayer_DeleteFileFromList(uint16 list_index,
        uint16 file_index,
        BOOLEAN is_delete_file)
{
    APLAYER_LIST_ERROR_E  result = APLAYER_LIST_INVALID_PARAM;
    APLAYER_LIST_HANDLE   list_handle = PNULL;

    list_handle = GetListHandle(list_index);

    result = MMIAPlayer_LIST_DeleteItem(list_handle,file_index,is_delete_file);

    SCI_TRACE_LOW("[APLAYER] MMIAPlayer_DeleteFileFromList list_index = %d, file_index = %d, is_delete_file = %d, list_handle = %x, result = %d",list_index,file_index,is_delete_file,list_handle,result);
    return result;
}


/*****************************************************************************/
//  Description : open list
//  Global resource dependence :
//  Author:
//  Note: 任何list操作之前都需要open list,操作结束后需要close handle
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPlayer_OpenList(uint16 list_index)
{
    APLAYER_LIST_HANDLE   list_handle = PNULL;
    wchar    full_path[MMIFILE_FULL_PATH_MAX_LEN+1] = {0};
    uint16   full_path_len = MMIFILE_FULL_PATH_MAX_LEN;
    BOOLEAN  result = FALSE;
    MMIFILE_HANDLE file_handle = 0;

    AddListOpenLock(list_index);
    //判断list 是否已经打开
    list_handle = GetListHandle(list_index);
    if (PNULL == list_handle)
    {
        //获取list file name
        result = GetListFilePathName(list_index, full_path, &full_path_len);

        if (result)
        {
            //申请list handle
            list_handle = MMIAPlayer_LIST_Create();
            if (PNULL != list_handle)
            {
                //列表文件不存在，则创建列表文件
                if (!MMIAPIFMM_IsFileExist(full_path,full_path_len))
                {
                    file_handle = MMIAPIFMM_CreateFile(
                                      full_path,
                                      SFS_MODE_WRITE|SFS_MODE_CREATE_ALWAYS, /*lint !e655*/
                                      NULL,
                                      NULL);
                    if (0 != file_handle)
                    {
                        MMIAPIFMM_CloseFile(file_handle);
                    }
                    //设置list handle
                    result = SetListHandle(list_index,list_handle);
                }
                else
                {
                    //加载list file
                    MMIAPlayer_LIST_LoadFromFile(list_handle,full_path,full_path_len);
                    //设置list handle
                    result = SetListHandle(list_index,list_handle);
                }

                if (!result)
                {
                    MMIAPlayer_LIST_Destroy(list_handle);
                }

            }

        }
    }

    SCI_TRACE_LOW("[APLAYER] MMIAPlayer_OpenList list_index = %d, result = %d", list_index, result);
    return result;
}

/*****************************************************************************/
//  Description : CloseAndFreeList
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CloseAndFreeList(uint16 list_index,
                               BOOLEAN is_save_list_file,
                               BOOLEAN is_free_list_always)
{
    APLAYER_LIST_HANDLE   list_handle = PNULL;
    BOOLEAN  result = FALSE;


    //获取list handle
    list_handle = GetListHandle(list_index);
    if (PNULL != list_handle)
    {
        if (is_save_list_file)
        {
            //保存文件
            result = SaveListFile(list_index,list_handle);
        }


        if (APLAYER_LIST_TYPE_MYMUSIC == list_index) //只有在文件保存了或者强烈要求释放的时候才释放
        {
            if (result
                    || is_free_list_always)
            {
                //释放list handle
                MMIAPlayer_LIST_Destroy(list_handle);
                //设置list handle
                SetListHandle(list_index,PNULL);

                result = TRUE;
            }
        }
        else
        {
            //释放list handle
            MMIAPlayer_LIST_Destroy(list_handle);
            //设置list handle
            SetListHandle(list_index,PNULL);
            result = TRUE;
        }


    }

    SCI_TRACE_LOW("[APLAYER] CloseAndFreeList list_index = %d, result = %d, is_free_list_always = %d", list_index, result,is_free_list_always);
    return result;
}
        
/*****************************************************************************/
//  Description : close list
//  Global resource dependence :
//  Author:
//  Note: 任何list操作之前都需要open list,操作结束后需要close handle
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPlayer_CloseList(uint16 list_index,
                                    BOOLEAN is_save_list_file)
{

    BOOLEAN result = FALSE;
    uint32  open_lock = DeleteListOpenLock(list_index);

    if (0 == open_lock)
    {
        result = CloseAndFreeList(list_index,is_save_list_file,FALSE);
    }
    else
    {
        result = TRUE;
    }

    SCI_TRACE_LOW("[APLAYER] MMIAPlayer_CloseList list_index = %d, result = %d", list_index, result);
    return result;
}

/*****************************************************************************/
//  Description : get list name
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPlayer_GetListFileFullPath(uint16 list_index,
        wchar * list_full_path_ptr,
        uint16 * list_full_path_len_ptr)
{

    //获取全路径
    return GetListFilePathName(list_index,list_full_path_ptr,list_full_path_len_ptr);

}

/*****************************************************************************/
//  Description : MMIAPlayer_GetListSortType
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC APLAYER_LIST_SORT_TYPE_E MMIAPlayer_GetListSortType(uint16 list_index)
{
    return GetListSortType(list_index);

}

/*****************************************************************************/
//  Description : get audio file full path
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPlayer_GetAudioFileFullPath(uint16 list_index,
        uint16 file_index,
        wchar * full_path_ptr,
        uint16 * full_path_len_ptr)
{

    BOOLEAN result = FALSE;
    APLAYER_LIST_HANDLE list_handle = PNULL;
    APLAYER_LIST_ITEM_DATA_T item_data = {0};

    if (PNULL != full_path_len_ptr
            && PNULL != full_path_ptr)
    {
        //获取list file的hanlde
        list_handle = GetListHandle(list_index);
        if (PNULL != list_handle)
        {
            //获取文件名称
            result = MMIAPlayer_LIST_GetItemData(list_handle,file_index, &item_data);
            if (result)
            {
                SCI_MEMSET(full_path_ptr,0,(sizeof(wchar)*(*full_path_len_ptr)));
                *full_path_len_ptr = MIN(*full_path_len_ptr, item_data.name_len);
                MMIAPICOM_Wstrncpy(full_path_ptr,item_data.filename,*full_path_len_ptr);

            }
        }
    }

    SCI_TRACE_LOW("[APLAYER] MMIAPlayer_GetAudioFileFullPath list_index = %d, file_index = %d, result = %d", list_index, file_index, result);
    return result;
}

/*****************************************************************************/
//  Description : get list size
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIAPlayer_GetListSize(uint16 list_index)
{
    APLAYER_LIST_HANDLE list_handle = PNULL;
    uint32 total_size = 0;

    //获取list file的hanlde
    list_handle = GetListHandle(list_index);
    if (PNULL != list_handle)
    {
        //获取list total num
        total_size = MMIAPlayer_LIST_GetTotalNum(list_handle);
		if(total_size > APLAYER_ALL_LIST_FILE_NUM_MAX)
		{
			total_size = APLAYER_ALL_LIST_FILE_NUM_MAX;
		}

    }
    SCI_TRACE_LOW("[APLAYER] MMIAPlayer_GetListSize total_size = %d, list_index = %d", total_size, list_index);

    return total_size;
}

/*****************************************************************************/
//  Description : get audio file to list
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPlayer_AddListFileToList(uint16 src_list_index,
        uint16 file_index,
        uint16 dst_list_index)
{
    BOOLEAN result = FALSE;
    APLAYER_LIST_HANDLE list_handle = PNULL;
	APLAYER_LIST_HANDLE dst_list_handle = PNULL;

    APLAYER_LIST_ITEM_DATA_T   item_data = {0};
	uint32 total_size = 0;


    list_handle = GetListHandle(src_list_index);
	dst_list_handle = GetListHandle(dst_list_index);

	//获取list total num
    total_size = MMIAPlayer_LIST_GetTotalNum(dst_list_handle);   //bug 194120 自定义列表中音频文件个数没有上限，导致定屏,暂定600
	if (total_size >= APLAYER_ALL_LIST_FILE_NUM_MAX)
	{
		SCI_TRACE_LOW("[APLAYER] MMIAPlayer_GetItemDataFromList list_index = %d, file_index = %d, result = %d",src_list_index, file_index,result);
		return result;
	}

    if (PNULL != list_handle)
    {
        if(MMIAPlayer_LIST_GetItemData(list_handle,file_index,&item_data))
        {
            result = MMIAPlayer_AddFileToList(dst_list_index,item_data);
        }
    }

    SCI_TRACE_LOW("[APLAYER] MMIAPlayer_GetItemDataFromList list_index = %d, file_index = %d, result = %d",src_list_index, file_index,result);
    return result;
}

/*****************************************************************************/
//  Description : add audio file to list
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPlayer_AddFileToList(uint16 list_index,
                                        APLAYER_LIST_ITEM_DATA_T item_data)
{
    APLAYER_LIST_HANDLE list_handle = PNULL;
    BOOLEAN      result = FALSE;


    //获取list file的handle
    list_handle = GetListHandle(list_index);
    if (PNULL != list_handle)
    {
        result = MMIAPlayer_LIST_AddItem(list_handle,item_data);
        //增加item到list中
    }

    SCI_TRACE_LOW("[APLAYER] MMIAPlayer_AddFileToList list_index = %d, result = %d", list_index,result);
    return result;
}

/*****************************************************************************/
//  Description : sort list
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPlayer_SortList(uint16 list_index,
                                   APLAYER_LIST_SORT_TYPE_E sort_type)
{
    APLAYER_LIST_HANDLE list_handle = PNULL;
    BOOLEAN      result = FALSE;


    //获取list file的handle
    list_handle = GetListHandle(list_index);
    if (PNULL != list_handle)
    {
        //排序
        result = MMIAPlayer_LIST_Sort(list_handle,sort_type);
        if (result)
        {
            SetListSortType(list_index,sort_type);
            SaveListFile(list_index,list_handle);
        }


    }

    SCI_TRACE_LOW("[APLAYER] MMIAPlayer_SortList list_index = %d, result = %d", list_index, result);
    return result;
}

/*****************************************************************************/
//  Description : set list buffer size
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPlayer_SetAllListBufferSize(uint32 buffer_size)
{
    uint32 i = 0;
    uint32 total_num = 0;
    APLAYER_LIST_HANDLE list_handle = PNULL;

    total_num = MMIAPlayer_GetTotalListNum();
    for (i = 0; i < total_num; i++)
    {
        list_handle = GetListHandle(i);
        if (PNULL != list_handle)
        {
            MMIAPlayer_LIST_SetBufferSize(list_handle, buffer_size);
        }
    }


    if (PNULL != s_all_array_list_handle)
    {
        MMIAPlayer_LIST_SetBufferSize(s_all_array_list_handle, buffer_size);
    }
}

/*****************************************************************************/
//  Description : get list index by list name
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPlayer_GetListIndex(wchar * list_path_ptr,
                                       uint16 list_path_len,
                                       uint16  *list_index_ptr)
{
    BOOLEAN result = FALSE;
    uint32  total_list_num = 0;
    uint32  i = 0;
    wchar   full_path[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
    uint16  full_path_len = MMIFILE_FULL_PATH_MAX_LEN;

    if (PNULL != list_path_ptr
            && PNULL != list_index_ptr)
    {
        total_list_num = MMIAPlayer_GetTotalListNum();

        for (i = 0; i < total_list_num; i++)
        {
            full_path_len = MMIFILE_FULL_PATH_MAX_LEN;
            if (GetListFilePathName(i,full_path,&full_path_len)
                    && 0 == MMIAPICOM_Wstrcmp(list_path_ptr,full_path))
            {
                result = TRUE;
                break;
            }
        }

        if ( i < total_list_num)
        {
            *list_index_ptr = i;
        }

    }
    SCI_TRACE_LOW("[APLAYER] MMIAPlayer_GetListIndex i = %d, result = %d", i, result);
    return result;
}

/*****************************************************************************/
//  Description : get file index in list
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC APLAYER_LIST_ERROR_E MMIAPlayer_GetAudioFileIndexInList(uint16 list_index,
        wchar * file_path_ptr,
        uint16 file_path_len,
        uint16 *file_index_ptr)
{
    APLAYER_LIST_ERROR_E  result = APLAYER_LIST_INVALID_PARAM;
    APLAYER_LIST_HANDLE  list_handle = PNULL;
    APLAYER_LIST_ITEM_DATA_T item_data = {0};


    if (PNULL != file_path_ptr
            && PNULL != file_index_ptr)
    {
        list_handle = GetListHandle(list_index);
        if (PNULL != list_handle)
        {
            SCI_MEMSET(item_data.filename,0,(sizeof(wchar)*(MMIFILE_FULL_PATH_MAX_LEN + 1)));
            item_data.name_len = MIN(MMIFILE_FULL_PATH_MAX_LEN, file_path_len);
            MMIAPICOM_Wstrncpy(item_data.filename,file_path_ptr,item_data.name_len);
            result = MMIAPlayer_LIST_GetItemIndex(list_handle,item_data,file_index_ptr,TRUE);
        }
        else
        {
            result = APLAYER_LSIT_NOT_OPEN;
        }
    }
    SCI_TRACE_LOW("[APLAYER] MMIAPlayer_GetAudioFileIndexInList list_index = %d, result = %d",list_index, result);
    return result;
}

/*****************************************************************************/
//  Description : rename list
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC APLAYER_LIST_ERROR_E MMIAPLayer_RenameList(uint16 list_index,
        wchar * new_list_name_ptr,
        uint16 new_list_name_len)
{
    APLAYER_LIST_ERROR_E  result = APLAYER_LIST_ERROR;
    wchar                 full_path[MMIFILE_FULL_PATH_MAX_LEN+1] = {0};
    uint16                full_path_len = MMIFILE_FULL_PATH_MAX_LEN;
    wchar                 new_full_path[MMIFILE_FULL_PATH_MAX_LEN+1] = {0};
    uint16                new_full_path_len = MMIFILE_FULL_PATH_MAX_LEN;
    wchar                 device_wstr[MMIFILE_FULL_PATH_MAX_LEN+1] = {0};
    uint16                device_wstr_len = MMIFILE_FULL_PATH_MAX_LEN;
    wchar                 dir_wstr[MMIFILE_FULL_PATH_MAX_LEN+1] = {0};
    uint16                dir_wstr_len = MMIFILE_FULL_PATH_MAX_LEN;

    if (PNULL != new_list_name_ptr
            && 0 != new_list_name_len)
    {
        if(GetListFilePathName(list_index,full_path,&full_path_len))
        {
            if (MMIAPIFMM_SplitFullPath(full_path, full_path_len,
                                        device_wstr, &device_wstr_len,
                                        dir_wstr, &dir_wstr_len,
                                        NULL, NULL))
            {
                if (MMIAPIFMM_CombineFullPath (device_wstr, device_wstr_len,
                                               dir_wstr, dir_wstr_len,
                                               new_list_name_ptr, new_list_name_len,
                                               new_full_path, &new_full_path_len))
                {
                    //名称不变的情况
                    if (0 == MMIAPICOM_CompareTwoWstrExt(
                            full_path, full_path_len,
                            new_full_path, new_full_path_len,
                            FALSE
                            )
                       )
                    {
                        result = APLAYER_LIST_SUCCESS;
                    }
                    else if (MMIAPIFMM_IsFileExist(new_full_path,new_full_path_len))
                    {
                        result = APLAYER_LIST_HAS_EXIST;
                    }
                    else if (MMIAPIFMM_IsFileExist(full_path,full_path_len))
                    {
                        if(MMIAPIFMM_RenameFile(full_path,full_path_len,new_list_name_ptr,new_list_name_len))
                        {
                            SetListFilePathName(list_index,new_full_path,new_full_path_len);
                            result = APLAYER_LIST_SUCCESS;
                        }
                    }
                    else
                    {
                        SetListFilePathName(list_index,new_full_path,new_full_path_len);
                        result = APLAYER_LIST_SUCCESS;
                    }

                }
            }
        }

    }

    SCI_TRACE_LOW("[APLAYER] MMIAPLayer_RenameList list_index = %d, result = %d",list_index, result);
    return result;
}

/*****************************************************************************/
//  Description : MMIAPlayer_IsListExist
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPlayer_IsListExist(wchar *full_path_ptr,
                                      uint16 full_path_len)
{
    BOOLEAN result = FALSE;
    uint16 list_index = 0;

    if (PNULL != full_path_ptr)
    {
        result = MMIAPlayer_GetListIndex(full_path_ptr,full_path_len,&list_index);
    }

    return result;

}

/*****************************************************************************/
//  Description : MMIMP3_CopyList
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC APLAYER_LIST_ERROR_E MMIAPlayer_CopyList(uint16 *list_index_ptr,
        MMIFILE_DEVICE_E dev)
{
    APLAYER_LIST_ERROR_E     result  = APLAYER_LIST_ERROR;
    wchar       list_full_path[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
    uint16      list_full_path_len = MMIFILE_FULL_PATH_MAX_LEN;
    wchar       new_list_full_path[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
    uint16      new_list_full_path_len = MMIFILE_FULL_PATH_MAX_LEN;
    uint32      file_size = 0;


    //参数检查
    if (PNULL == list_index_ptr
            || MMI_DEVICE_NUM <= dev)
    {
        result = APLAYER_LIST_INVALID_PARAM;
        return result;
    }

    //目标盘符是否存在
    if(!MMIAPIFMM_CheckMemoryCard(dev))
    {
        result = APLAYER_LIST_NO_SAPCE;
        return result;
    }

    //获取原文件信息
    if(MMIAPlayer_GetListFileFullPath(*list_index_ptr,list_full_path,&list_full_path_len))
    {

        //组成新路径
        SCI_MEMSET(new_list_full_path,0,(sizeof(new_list_full_path)));
        new_list_full_path_len = list_full_path_len;
        MMIAPICOM_Wstrncpy(new_list_full_path,list_full_path,new_list_full_path_len);
        new_list_full_path[0] = *MMIAPIFMM_GetDevicePath(dev);

        //目标文件是否存在
        if (MMIAPIFMM_IsFileExist(new_list_full_path,new_list_full_path_len))
        {
            result = APLAYER_LIST_HAS_EXIST;
            return result;
        }

        //获取所需要的空间
        if (!MMIAPIFMM_IsFileExist(list_full_path,list_full_path_len))
        {
            result = APLAYER_LIST_FILE_NO_EXIST;
            return result;
        }

        MMIAPIFMM_GetFileInfo(list_full_path,list_full_path_len,&file_size,PNULL,PNULL);

        //检查空间是否足够，拷贝文件
        if(!MMIAPIFMM_IsEnoughSpace(MMIFMM_STORE_TYPE_FIXED,dev,file_size,PNULL))
        {
            result = APLAYER_LIST_NO_SAPCE;
            return result;
        }

        if(SFS_NO_ERROR != MMIAPIFMM_CopyFile(list_full_path,new_list_full_path))
        {
            result = APLAYER_LIST_ERROR;
            return result;
        }

        if(RegistNewList(new_list_full_path,new_list_full_path_len,APLAYER_LIST_TYPE_USER_LIST,list_index_ptr))
        {
            SaveAllListInfoFile();
            result = APLAYER_LIST_SUCCESS;
        }
        else
        {
            MMIAPIFMM_DeleteFile(new_list_full_path,PNULL);
        }

    }
    SCI_TRACE_LOW("[APLAYER] MMIAPlayer_CopyList  dev = %d, result = %d",dev,result);
    return result;
}

