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
#ifndef _MMIAP_LIST_CONTROL_H_
#define _MMIAP_LIST_CONTROL_H_

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "mmk_type.h"
#include "mmi_filemgr.h"
#include "mmiap_list.h"
#include "mmiap_list_manager.h"
#include "mmiap_common.h"
#include "mmiap_setting.h"
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
//delete list data
typedef struct
{
    MMI_WIN_ID_T                list_win_id;
    BOOLEAN                     is_music_list;
    uint16                      list_selection;
    uint16                      cur_file_selection;
    uint32                      select_num;
    uint16                      *select_index_ptr;
    uint32                      del_num;
    BOOLEAN                     is_del_file;
} MMIMP3_LIST_DEL_INFO_T;

typedef struct
{
    BOOLEAN is_music_list;
    uint16 cur_list_index;
} MMIMP3_PLAY_LIST_INFO_T;

/*---------------------------------------------------------------------------*/
/*                          FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : MMIMP3_LoadCurPlayInfo
//  Global resource dependence :
//  Author:xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMP3_LoadCurPlayInfo(void);


/*****************************************************************************/
//  Description : open list for play
//  Global resource dependence :
//  Author:xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC APLAYER_LIST_ERROR_E MMIMP3_SetCurPlayInfoByName(wchar * list_path_ptr,
        uint16 list_path_len,
        wchar * file_path_ptr,
        uint16 file_path_len,
        MMIMP3_PLAY_MODE_E play_mode);

/*****************************************************************************/
//  Description : get list full path
//  Global resource dependence :
//  Author:xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMP3_GetListFileNameInfo(BOOLEAN is_music_list,
        uint16 list_index,
        MMIFILE_DEVICE_E  * dev_type_ptr,
        wchar            *file_name_ptr,
        uint16           *file_name_len_ptr  );

/*****************************************************************************/
//  Description : MMIMP3_GetFilePathInCurPlayList
//  Global resource dependence :
//  Author:xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMP3_GetFilePathInCurPlayList(uint16 file_index,
        wchar *full_path_name_ptr,
        uint16 *full_path_len_ptr);

/*****************************************************************************/
//  Description : get list total num
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIMP3_GetListTotalNum(BOOLEAN is_music_list,
                                     uint16 list_index);

/*****************************************************************************/
//  Description : get cur play list index
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC MMIMP3_PLAY_LIST_INFO_T MMIMP3_GetCurPlayListInfo(void);
/*****************************************************************************/
//  Description : MMIMP3_UpdatePlayListInfo
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIMP3_UpdatePlayListInfo(void);

/*****************************************************************************/
//  Description : get cur play file index
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMIMP3_GetCurPlayFileIndex(void);

/*****************************************************************************/
//  Description : get cur play file total num
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIMP3_GetCurPlayListTotalNum(void);

/*****************************************************************************/
//  Description : get cur file full path
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMP3_GetCurPlayFileFullPath(wchar * full_path_ptr,
        uint16 *full_path_len_ptr);

/*****************************************************************************/
//  Description : get cur file full path
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMP3_GetCurPlayListFullPath(wchar * full_path_ptr,
        uint16 *full_path_len_ptr);

/*****************************************************************************/
//  Description : get total list num
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIMP3_GetTotalListNum(void);

/*****************************************************************************/
//  Description : MMIMP3_RenameListFile
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC APLAYER_LIST_ERROR_E MMIMP3_RenameListFile(BOOLEAN is_music_list,
        uint16 list_index,
        wchar * file_name_ptr,
        uint16 file_name_len);


/*****************************************************************************/
//  Description : MMIMP3_CopyList
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC APLAYER_LIST_ERROR_E MMIMP3_CopyList(BOOLEAN is_music_list,
        uint16 *list_index_ptr,
        MMIFILE_DEVICE_E dev);

/*****************************************************************************/
//  Description : MMIMP3_DeleteList
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMP3_DeleteList(BOOLEAN is_music_list,
                                 uint16 *list_index_ptr);

/*****************************************************************************/
//  Description : MMIMP3_ReleaseAllLists
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIMP3_ReleaseAllLists(void);

/*****************************************************************************/
//  Description : MMIMP3_OpenList
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIMP3_OpenList(BOOLEAN is_music_list,
                            uint16 list_index);

/*****************************************************************************/
//  Description : release cur play file info
//  Global resource dependence :
//  Author:xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIMP3_ClearCurPlayInfo(void);

/*****************************************************************************/
//  Description : init cur play file info
//  Global resource dependence :
//  Author:xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIMP3_InitCurPlayInfo(void);

/*****************************************************************************/
//  Description : release cur play file info
//  Global resource dependence :
//  Author:xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIMP3_ReleaseCurPlayFileInfo(BOOLEAN is_save_list);

/*****************************************************************************/
//  Description : MMIMP3_CloseList
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIMP3_CloseList(BOOLEAN is_music_list,
                             uint16 list_index,
                             BOOLEAN is_save_list_file);

/*****************************************************************************/
//  Description : MMIMP3_AddFileFromListToList
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC APLAYER_LIST_ERROR_E MMIMP3_AddFileFromListToList(BOOLEAN dst_is_music_list,
        uint16 dst_list_index,
        BOOLEAN src_is_music_list,
        uint16 src_list_index,
        uint16 *file_index_ptr,
        uint32  add_total_num,
        uint32  *added_num_ptr);



/*****************************************************************************/
//  Description : MMIMP3_CreateNewList
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC MMIFILE_DEVICE_E MMIMP3_GetListFileDev(BOOLEAN is_music_list,
        uint16 list_index);

/*****************************************************************************/
//  Description : MMIMP3_CreateListFile
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC APLAYER_LIST_ERROR_E MMIMP3_CreateList(uint16* list_index_ptr,
        wchar * file_name_ptr,
        uint16 file_name_len);


/*****************************************************************************/
//  Description : Get New List Name With Number
//  Global resource dependence :
//  Author:xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMP3_GetNewListFileName(MMI_STRING_T default_name,
        wchar * list_file_name_ptr,
        uint16 *list_file_name_len_ptr);

/*****************************************************************************/
//  Description : get audio file full path
//  Global resource dependence :
//  Author:xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMP3_GetAudioFileFullPath(BOOLEAN is_music_list,
        uint16 list_index,
        uint16 file_index,
        wchar   * full_path_ptr,
        uint16 *full_path_len_ptr);
/*****************************************************************************/
//  Description : sort list
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMP3_SortList(BOOLEAN is_music_list,
                               uint16 list_index,
                               APLAYER_LIST_SORT_TYPE_E sort_type);

/*****************************************************************************/
//  Description : MMIMP3_GetListSortType
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC APLAYER_LIST_SORT_TYPE_E MMIMP3_GetListSortType(BOOLEAN is_music_list,
        uint16 list_index);

/*****************************************************************************/
//  Description : start mp3 del task
//  Global resource dependence :
//  Author: paul.huang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMP3_StartDelFile(MMIMP3_LIST_DEL_INFO_T* opr_info_ptr);

/*****************************************************************************/
//  Description : stop mp3 del task
//  Global resource dependence :
//  Author: paul.huang
//  Note:
/*****************************************************************************/
PUBLIC void MMIMP3_StopDelFile(void);


/*****************************************************************************/
//  Description : Init current play list with file.
//  Global resource dependence :
//  Author:aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMP3_LoadAudioFile(wchar * file_full_path_ptr,
                                    uint16  file_full_path_len,
                                    BOOLEAN is_bg_play,
                                    MMIMP3_PLAY_MODE_E play_mode
                                   );

/*****************************************************************************/
//  Description : Init current play list with FMM file list.
//  Global resource dependence :
//  Author:xingdong.li
//  Note:只要不是当前播放的文件加载失败，则继续加载
/*****************************************************************************/
PUBLIC BOOLEAN MMIMP3_LoadFmmPlayList(
    MMIFMM_DATA_INFO_T  *file_info_ptr,
    uint16              file_num,
    uint16              file_index,
    uint16              *dir_path_ptr,
    uint16              dir_path_len,
    MMIMP3_PLAY_MODE_E play_mode);

/*****************************************************************************/
//  Description : resize all buffer size
//  Global resource dependence :
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIMP3_ResizeAllListBufferSize(BOOLEAN is_decrease);

/*****************************************************************************/
//  Description : Get the file info of the current play list.
//  Global resource dependence :
//  Author: xin.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMP3_GetCurPlayListFileInfo(uint16 file_index,
        wchar *file_full_path_ptr,
        uint16 *file_full_path_len_ptr);

/*****************************************************************************/
//  Description : MMIMP3_LoadLists
//  Global resource dependence :
//  Author:xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIMP3_LoadLists(void);

/*****************************************************************************/
//  Description : MMIMP3_StartLoadMusicFiles
//  Global resource dependence :
//  Author:xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIMP3_SearchAudioFilesInDefaultFolder(MMI_WIN_ID_T      win_id,
        MMI_MESSAGE_ID_E  end_msg_id,
        MMI_MESSAGE_ID_E  next_msg_id,
        MMIFILE_DEVICE_E tem_dev);

/*****************************************************************************/
//  Description : MMIMP3_LoadFilesToMyMusicList
//  Global resource dependence :
//  Author:xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIMP3_SaveFilesToMyMusicList(void);

/*****************************************************************************/
//  Description : MMIMP3_StopInitPlayer
//  Global resource dependence :
//  Author:xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIMP3_StopLoadAudioFiles(void);

/*****************************************************************************/
//  Description : LoadFilesToList
//  Global resource dependence :
//  Author:xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMP3_AppendFilesToList(BOOLEAN is_music_list,
                                        uint16 list_index,
                                        FILEARRAY file_array,
                                        uint32 *add_num_ptr);


/*****************************************************************************/
//  Description : MMIMP3_SetCurPlayInfo
//  Global resource dependence :
//  Author:robert.wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMP3_SetCurPlayInfo(BOOLEAN is_music_list,
                                     uint16 list_index,
                                     uint16 file_index,
                                     MMIMP3_PLAY_MODE_E play_mode);

/*****************************************************************************/
//  Description : MMIMP3_SetCurPlayFileIndex
//  Global resource dependence :
//  Author:robert.wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMP3_SetCurPlayFileIndex(BOOLEAN is_prev);

/*****************************************************************************/
//  Description : MMIMP3_ResetCurPlayFileIndex
//  Author:robert.wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMP3_ResetCurPlayFileIndex(BOOLEAN is_prev);

/*****************************************************************************/
//  Description : MMIMP3_GetPreOrNextPlayFileIndex
//  Global resource dependence :
//  Author:robert.wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMP3_GetPreOrNextPlayFileIndex(BOOLEAN is_prev,                                                                            
                                              uint16 *next_file_index_ptr //out
                                                );

/*****************************************************************************/
//  Description : MMIMP3_GetFileIndexByOffset
//  Global resource dependence :
//  Author:robert.wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMP3_GetFileIndexByOffset(int32 offset,
                                           uint16 *next_file_index_ptr);

/*****************************************************************************/
//  Description : MMIMP3_IsCurPlayList
//  Global resource dependence :
//  Author:robert.wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMP3_IsCurPlayList(BOOLEAN is_music_list,
                                    uint16 list_index);


/*****************************************************************************/
//  Description : MMIMP3_IsCurPlayList
//  Global resource dependence :
//  Author:robert.wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMP3_IsCurPlayFile(BOOLEAN is_music_list,
                                    uint16 cur_list_selection,
                                    uint16 item_index);

/*****************************************************************************/
//  Description : MMIMP3_SetCurPlayMode
//  Global resource dependence :
//  Author:robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIMP3_SetCurPlayMode(MMIMP3_PLAY_MODE_E play_mode);

/*****************************************************************************/
//  Description : MMIMP3_SearchAllAudioFiles
//  Global resource dependence :
//  Author:robert.wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMP3_SearchAllAudioFiles(BOOLEAN is_music_list,
        uint16 list_index,
        MMI_WIN_ID_T      win_id,
        MMI_MESSAGE_ID_E  msg_id);

/*****************************************************************************/
//  Description : MMIMP3_AddAllAudioFilesToList
//  Global resource dependence :
//  Author:robert.wang
//  Note:
/*****************************************************************************/
PUBLIC APLAYER_LIST_ERROR_E MMIMP3_SaveAllAudioFilesToList(BOOLEAN is_music_list,
        uint16 list_index);
        
/*****************************************************************************/
//  Description : get list type
//  Global resource dependence :
//  Author:
//  Note: BOOLEAN is_music_list :是否是音乐列表，音乐列表从my music开始
/*****************************************************************************/
PUBLIC APLAYER_LIST_TYPE_E MMIMP3_GetListType(
    BOOLEAN is_music_list,
    uint16 list_index
);

/*****************************************************************************/
//  Description : delete list file
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
/*****************************************************************************/
//  Description : delete list file
//  Global resource dependence :
//  Author:
//  Note:BOOLEAN is_music_list :是否是音乐列表，音乐列表从my music开始
/*****************************************************************************/
PUBLIC void MMIMP3_DeleteListFile(
    BOOLEAN is_music_list,
    uint16 list_index
);

/**--------------------------------------------------------------------------*
**                         Compiler Flag                                    *
**--------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif
#endif



