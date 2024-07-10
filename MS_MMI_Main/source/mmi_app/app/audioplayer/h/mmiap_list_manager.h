/*****************************************************************************
** File Name:               mmiap_list_manager.h                             *
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
#ifndef _MMIAP_LIST_MANAGER_H_
#define _MMIAP_LIST_MANAGER_H_

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "mmi_filemgr.h"
#include "mmiap_list.h"
/**--------------------------------------------------------------------------*
**                         Compiler Flag                                    *
**--------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C"
{
#endif

/*---------------------------------------------------------------------------*/
/*                          MACRO DEFINITION                                 */
/*---------------------------------------------------------------------------*/
//列表文件所需最小空间
#define APLAYER_LIST_MIN_MEM  (1024<<5)  //32K
#define APlayer_FMM_TEMP_LIST_NAME         L"audiotemplist.temp"
#define APlayer_FMM_TEMP_LIST_NAME_LEN    18
#define APLAYER_MY_MUSIC_NAME              L"MyMusic.lst"
#define APLAYER_MY_MUSIC_NAME_LEN          11
#define APLAYER_RES_LIST_1_NAME            L"list1.lst"
#define APLAYER_RES_LIST_1_NAME_LEN        9
#define APLAYER_RES_LIST_2_NAME            L"list2.lst"
#define  APLAYER_RES_LIST_2_NAME_LEN       9
#define  APLAYER_RES_LIST_3_NAME           L"list3.list"
#define APLAYER_RES_LIST_3_NAME_LEN        9
#define  APLAYER_RES_FOLDER_NAME           L"Mp3_res"
#define APLAYER_RES_FOLDER_NAME_LEN        7
#define AP_MUSIC_LIST_NUM_MAX              100     //最多能创建100个音乐列表

/**--------------------------------------------------------------------------*
**                         TYPE AND CONSTANT                                *
**--------------------------------------------------------------------------*/
typedef enum
{
    APLAYER_LIST_TYPE_FMM,
    APLAYER_LIST_TYPE_MYMUSIC,
    APLAYER_LIST_TYPE_DEFAULT,
    APLAYER_LIST_TYPE_USER_LIST,
    APLAYER_LIST_TYPE_MAX
}
APLAYER_LIST_TYPE_E;

/*---------------------------------------------------------------------------*/
/*                          FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : save  list info to file
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPLAYER_SaveListFile(uint16 list_index);

/*****************************************************************************/
//  Description : init all list info
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPlayer_InitAllList(void);

/*****************************************************************************/
//  Description : destory all list info
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPlayer_ReleaseAllList(void);

/*****************************************************************************/
//  Description : MIAPlayer_GetListFilePath
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPlayer_GetListFilePath(wchar *list_file_name_ptr,
        uint16 list_file_name_len,
        wchar *list_full_path_ptr,
        uint16 *list_full_path_len_ptr);

/*****************************************************************************/
//  Description : create list
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC APLAYER_LIST_ERROR_E MMIAPlayer_CreateNewListByFileName(wchar *list_file_name_ptr,
        uint16 list_file_name_len,
        uint16 * list_index_ptr);

/*****************************************************************************/
//  Description : get all list num
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIAPlayer_GetTotalListNum(void);

/*****************************************************************************/
//  Description : destroy list
//  Global resource dependence :
//  Author:
//  Note: 卸载
/*****************************************************************************/
PUBLIC void MMIAPlayer_DestroyList(uint16 list_index);

/*****************************************************************************/
//  Description : delete audio file from list
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC APLAYER_LIST_ERROR_E MMIAPlayer_DeleteFileFromList(uint16 list_index,
        uint16 file_index,
        BOOLEAN is_delete_file) ;

/*****************************************************************************/
//  Description : open list
//  Global resource dependence :
//  Author:
//  Note: 任何list操作之前都需要open list,操作结束后需要close handle
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPlayer_OpenList(uint16 list_index);

/*****************************************************************************/
//  Description : close list
//  Global resource dependence :
//  Author:
//  Note: 任何list操作之前都需要open list,操作结束后需要close handle
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPlayer_CloseList(uint16 list_index,
                                    BOOLEAN is_save_list_file);

/*****************************************************************************/
//  Description : get list name
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPlayer_GetListFileFullPath(uint16 list_index,
        wchar * list_full_path_ptr,
        uint16 * list_full_path_len_ptr);

/*****************************************************************************/
//  Description : get audio file full path
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPlayer_GetAudioFileFullPath(uint16 list_index,
        uint16 file_index,
        wchar * full_path_ptr,
        uint16 * full_path_len_ptr);


/*****************************************************************************/
//  Description : MMIAPlayer_GetListSortType
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC APLAYER_LIST_SORT_TYPE_E MMIAPlayer_GetListSortType(uint16 list_index);

/*****************************************************************************/
//  Description : get list size
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIAPlayer_GetListSize(uint16 list_index);

/*****************************************************************************/
//  Description : get audio file to list
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPlayer_AddListFileToList(uint16 src_list_index,
        uint16 file_index,
        uint16 dst_list_index);

/*****************************************************************************/
//  Description : add audio file to list
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPlayer_AddFileToList(uint16 list_index,
                                        APLAYER_LIST_ITEM_DATA_T item_data);

/*****************************************************************************/
//  Description : sort list
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPlayer_SortList(uint16 list_index,
                                   APLAYER_LIST_SORT_TYPE_E sort_type);


/*****************************************************************************/
//  Description : set list buffer size
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPlayer_SetAllListBufferSize(uint32 buffer_size);

/*****************************************************************************/
//  Description : get list index by list name
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPlayer_GetListIndex(wchar * list_path_ptr,
                                       uint16 list_path_len,
                                       uint16  *list_index_ptr);

/*****************************************************************************/
//  Description : get file index in list
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC APLAYER_LIST_ERROR_E MMIAPlayer_GetAudioFileIndexInList(uint16 list_index,
        wchar * file_path_ptr,
        uint16 file_path_len,
        uint16 *file_index_ptr);

/*****************************************************************************/
//  Description : rename list
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC APLAYER_LIST_ERROR_E MMIAPLayer_RenameList(uint16 list_index,
        wchar * list_full_path_ptr,
        uint16 list_full_path_len);

/*****************************************************************************/
//  Description : MMIAPlayer_IsListExist
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPlayer_IsListExist(wchar *full_path_ptr,
                                      uint16 full_path_len);

/*****************************************************************************/
//  Description : MMIMP3_CopyList
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC APLAYER_LIST_ERROR_E MMIAPlayer_CopyList(uint16 *list_index_ptr,
        MMIFILE_DEVICE_E dev);

/*****************************************************************************/
//  Description : get list type
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC APLAYER_LIST_TYPE_E MMIAPlayer_GetListType(uint16 list_index);

/*****************************************************************************/
//  Description : delete list file
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPlayer_DeleteListFile(uint16 list_index);

/**--------------------------------------------------------------------------*
**                         Compiler Flag                                    *
**--------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif
#endif


