/*****************************************************************************
** File Name:               mmiap_list.h                                     *
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
#ifndef _MMIAP_LIST_H_
#define _MMIAP_LIST_H_

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "sci_types.h"
#include "mmi_filemgr.h"
#include "dal_time.h"
#include "mmiap_musicarray.h"
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

/**--------------------------------------------------------------------------*
**                         TYPE AND CONSTANT                                *
**--------------------------------------------------------------------------*/
typedef void* APLAYER_LIST_HANDLE;

#define   MMIAP_PLAY_LIST_MAX_NUM              1000    //这个问题主要避免7701小块内存分配存在的问题

typedef struct
{
    wchar       filename[MMIFILE_FULL_PATH_MAX_LEN+1]; //文件名或文件夹名
    uint16      name_len;
    SCI_DATE_T  modify_date;    //修改日期
    SCI_TIME_T  modify_time;    //修改时间
    uint32      size;   //文件大小
} APLAYER_LIST_ITEM_DATA_T;

typedef enum
{
    APLAYER_LIST_ERROR,
    APLAYER_LIST_SUCCESS,
    APLAYER_LIST_INVALID_PARAM,
    APLAYER_LIST_DATA_ERROR,
    APLAYER_LIST_DELETE_FILE_ERROR,
    APLAYER_LIST_NO_SAPCE,
    APLAYER_LIST_HAS_EXIST,
    APLAYER_LIST_NO_EXIST,
    APLAYER_LIST_NO_MEMORY,
    APLAYER_LIST_FILE_NOT_IN_LIST,
    APLAYER_LIST_LIST_NO_EXIST,
    APLAYER_LIST_FILE_NO_EXIST,
    APLAYER_LSIT_NOT_OPEN,
    APLAYER_LIST_MAX
} APLAYER_LIST_ERROR_E;


/*---------------------------------------------------------------------------*/
/*                          FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : add an item to the list
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPlayer_LIST_AddItem(APLAYER_LIST_HANDLE list_handle,
                                       APLAYER_LIST_ITEM_DATA_T item_data);

/*****************************************************************************/
//  Description : delete an item to the list
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC APLAYER_LIST_ERROR_E MMIAPlayer_LIST_DeleteItem(APLAYER_LIST_HANDLE list_handle,
        uint16  index,
        BOOLEAN is_delete_file);

/*****************************************************************************/
//  Description : get list total item num
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIAPlayer_LIST_GetTotalNum(APLAYER_LIST_HANDLE list_handle);

/*****************************************************************************/
//  Description : load list from file
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPlayer_LIST_LoadFromFile(APLAYER_LIST_HANDLE list_handle,
        wchar   *full_path_ptr,
        uint16  full_path_len);

/*****************************************************************************/
//  Description : modify item data
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPlayer_LIST_ModifyItem(APLAYER_LIST_HANDLE list_handle,
        uint16 index,
        APLAYER_LIST_ITEM_DATA_T item_data);

/*****************************************************************************/
//  Description : get item data
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPlayer_LIST_GetItemData(APLAYER_LIST_HANDLE list_handle,
        uint16 index,
        APLAYER_LIST_ITEM_DATA_T *item_data_ptr);

/*****************************************************************************/
//  Description : save list to file
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPlayer_LIST_SaveFile(APLAYER_LIST_HANDLE list_handle,
                                        wchar  *full_path_ptr,
                                        uint16  full_path_len);

/*****************************************************************************/
//  Description : sort list
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPlayer_LIST_Sort(APLAYER_LIST_HANDLE list_handle,
                                    APLAYER_LIST_SORT_TYPE_E sort_type);

/*****************************************************************************/
//  Description : create list handle
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC APLAYER_LIST_HANDLE MMIAPlayer_LIST_Create(void);

/*****************************************************************************/
//  Description : destroy list handle
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPlayer_LIST_Destroy(APLAYER_LIST_HANDLE list_handle);

/*****************************************************************************/
//  Description : get item index
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC APLAYER_LIST_ERROR_E MMIAPlayer_LIST_GetItemIndex(APLAYER_LIST_HANDLE list_handle,
        APLAYER_LIST_ITEM_DATA_T item_data,
        uint16  * file_index_ptr,
        BOOLEAN is_only_for_name);

/*****************************************************************************/
//  Description : set buffer size
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPlayer_LIST_SetBufferSize(APLAYER_LIST_HANDLE list_handle,
        uint32 buffer_size);

/**--------------------------------------------------------------------------*
**                         Compiler Flag                                    *
**--------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif
#endif




