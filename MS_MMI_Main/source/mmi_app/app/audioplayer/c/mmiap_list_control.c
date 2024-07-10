/*****************************************************************************
** File Name:               mmiap_list_control.c                             *
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

#define _MMIAP_LIST_CONTROL_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#ifdef WIN32
#include "std_header.h"
#endif
#include "mmi_appmsg.h"
#include "sci_types.h"
#include "mmk_app.h"
#include "mmk_type.h"
#include "mmi_filemgr.h"
#include "mmiap_list.h"
#include "mmiap_list_manager.h"
#include "mmi_arraylist.h"
#include "mmiap_list_control.h"
#include "mmi_string.h"
#include "mmisrvfmm_export.h"
#include "mmimp3_id.h"
#include "mmiap_common.h"
#include "priority_app.h"
#include "mmiap_play.h"

#ifdef MMIAP_MUSICMARK_SUPPORT
#include "mmiap_musicmark.h"
#endif

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define INVALID_INDEX  0xFFFF

#define   P_APP_MP3_DEL_STACK_SIZE              6144    //6K
#define   P_APP_MP3_DEL_QUEUE_NUM               8
#define   P_APP_MP3_DEL_TASK_PRIORITY         PRI_APP_FILE_TASK //  (30)
/**--------------------------------------------------------------------------*
 **                         LOCAL DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/
typedef struct
{
    BOOLEAN is_music_list;
    uint16 cur_list_index;
    uint16 cur_file_index;  //在音乐列表中的索引
    wchar  file_full_path[MMIFILE_FULL_PATH_MAX_LEN+1];
    uint16 file_full_path_len;
    uint16  active_index;  //在播放列表中的索引
    MMI_ALISTHANDLE_T play_list_ptr;
    MMIMP3_PLAY_MODE_E play_mode;
} APLAYER_CUR_PLAY_INFO;


/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/
LOCAL APLAYER_CUR_PLAY_INFO s_cur_audioplayer_list_info = {0};

static FILEARRAY s_file_array = PNULL;

LOCAL BLOCK_ID g_app_mp3_del_task_id = SCI_INVALID_BLOCK_ID;
/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : update cur play file info
//  Global resource dependence :
//  Author:xingdong.li
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN UpdateCurPlayFileInfo(BOOLEAN is_music_list,
                                    uint16 list_selection,
                                    uint16 file_index,
                                    MMIMP3_PLAY_MODE_E play_mode);

/*****************************************************************************/
//  Description : mp3 del task
//  Global resource dependence :
//  Author: paul.huang
//  Note:
/*****************************************************************************/
LOCAL void MP3Del_Task(
    uint32 argc,
    void * argv
);

/*****************************************************************************/
//  Description : Delete music files from list file or list array.
//  Global resource dependence :
//  Author:aoke.hu
//  Note:
/*****************************************************************************/
LOCAL void DelFileInfo(MMIMP3_LIST_DEL_INFO_T *opr_info_ptr);

/*****************************************************************************/
//  Description : DestroyRandomList
//  Global resource dependence :
//  Author:robert.wang
//  Note:
/*****************************************************************************/
LOCAL void DestroyRandomList(MMI_ALISTHANDLE_T play_list_ptr);

/*****************************************************************************/
//  Description : 获取指定范围内的不重复的随机整数.
//  Global resource dependence :
//  Author:apple.zhang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetDifferentRandom(MMI_ALISTHANDLE_T play_list_ptr,
                                 uint16 start_index,
                                 uint32 total_num,
                                 uint16 cur_file_index
                                );

/*****************************************************************************/
//  Description : CreateRandomList
//  Global resource dependence :
//  Author:robert.wang
//  Note:
/*****************************************************************************/
LOCAL MMI_ALISTHANDLE_T CreateRandomList(uint16 start_index,
        uint32 total_num,
        uint16 *active_file_index_ptr,
        uint16 cur_index,
        MMIMP3_PLAY_MODE_E play_mode);



/*****************************************************************************/
//  Description : GetPreOrNextIndexByList
//  Global resource dependence :
//  Author:apple.zhang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetPlayFileIndex(int32 offset,
                               uint16 *next_file_index_ptr //out
                              );


/*****************************************************************************/
//  Description : GetPlayFileIndexByMode
//  Global resource dependence :
//  Author:robert.wang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetPlayFileIndexByMode(int32 offset,
                                     uint16 *next_file_index_ptr //out
                                    );


/*****************************************************************************/
//  Description : GetIndex
//  Global resource dependence :
//  Author:robert.wang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetIndex(int32 offset,
                       uint16 *cur_index_ptr,
                       uint32 total_num);

/*****************************************************************************/
//  Description : LoadFilesToList
//  Global resource dependence :
//  Author:xingdong.li
//  Note:从头开始添加
/*****************************************************************************/
LOCAL BOOLEAN AddFilesToList(BOOLEAN is_music_list,
                             uint16 list_index,
                             FILEARRAY file_array,
                             uint32 *add_num_ptr);

/*****************************************************************************/
//  Description : AddFileToList
//  Global resource dependence :
//  Author:xingdong.li
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN AddFileToList(FILEARRAY_DATA_T  file_array_data,
                            BOOLEAN is_music_list,
                            uint16 dst_list_index);

/*****************************************************************************/
//  Description : ClearList
//  Global resource dependence :
//  Author:xingdong.li
//  Note:
/*****************************************************************************/
LOCAL void ClearList(BOOLEAN is_music_list,
                     uint16 list_index);

/*****************************************************************************/
//  Description : AddFileFromListToList
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN  AddFileFromListToList(BOOLEAN src_is_music_list,
                                     uint16  src_list_index,
                                     uint16  file_index,
                                     BOOLEAN dst_is_music_list,
                                     uint16  dst_list_index);

/*****************************************************************************/
//  Description : UpdateCurPlayFileInfoByList
//  Global resource dependence :
//  Author:robert.wang
//  Note:
/*****************************************************************************/
LOCAL void UpdateCurPlayFileInfoByList(void);

/*****************************************************************************/
//  Description : UpdateCurPlayFileInfoByFileIndex
//  Global resource dependence :
//  Author:robert.wang
//  Note:
/*****************************************************************************/
LOCAL void UpdateCurPlayFileInfoByFileIndex(uint16 file_index,
        uint16 active_index);

/*****************************************************************************/
//  Description : UpdateCurPlayFileInfoByName
//  Global resource dependence :
//  Author:xingdong.li
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN UpdateCurPlayFileInfoByName(void);

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : MMIMP3_LoadCurPlayInfo
//  Global resource dependence :
//  Author:xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMP3_LoadCurPlayInfo(void)
{
    BOOLEAN result = FALSE;

    //加载文件

    //否则
    MMIMP3_SetCurPlayInfo(TRUE,0,0,MMIMP3_GetMp3PlayMode());

    return result;
}


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
        MMIMP3_PLAY_MODE_E play_mode)

{
    APLAYER_LIST_ERROR_E   result = APLAYER_LIST_ERROR;
    uint16    list_index = 0;
    uint16    list_selection = 0;
    uint16    file_index = 0;
    BOOLEAN   is_music_list = PNULL;

    if (PNULL != list_path_ptr
            && PNULL != file_path_ptr)
    {

        if (MMIAPlayer_GetListIndex(list_path_ptr,list_path_len,&list_index))
        {
            if (list_index >= APLAYER_LIST_TYPE_MYMUSIC)
            {
                is_music_list = TRUE;
                list_selection = list_index - APLAYER_LIST_TYPE_MYMUSIC;
            }

            if (!MMIMP3_IsCurPlayList(is_music_list,list_selection))
            {
                MMIMP3_OpenList(is_music_list,list_selection);
            }


            result = MMIAPlayer_GetAudioFileIndexInList(list_index,file_path_ptr,file_path_len,&file_index);
            if (APLAYER_LIST_SUCCESS == result)
            {
                if(UpdateCurPlayFileInfo(is_music_list,list_selection,file_index,play_mode))
                {
                    result = APLAYER_LIST_SUCCESS;
                }
                else
                {
                    if (!MMIMP3_IsCurPlayList(is_music_list,list_selection))
                    {
                        MMIMP3_CloseList(is_music_list,list_selection,FALSE);
                    }
                    result = APLAYER_LIST_ERROR;
                }
            }
            else if (!MMIMP3_IsCurPlayList(is_music_list,list_selection))
            {
                MMIMP3_CloseList(is_music_list,list_selection,FALSE);
            }

        }
        else
        {
            result = APLAYER_LIST_NO_EXIST;
        }

    }
    else
    {
        result = APLAYER_LIST_NO_EXIST;
    }

    SCI_TRACE_LOW("[APLIST] MMIMP3_SetCurPlayInfoByName is_music_list = %d, list_index = %d, file_index = %d, play_mode = %d, result = %d",is_music_list,list_index,file_index, play_mode, result);
    return result;
}

/*****************************************************************************/
//  Description : MMIMP3_OpenList
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIMP3_OpenList(BOOLEAN is_music_list,
                            uint16 list_index)
{
    if (is_music_list)
    {
        list_index += APLAYER_LIST_TYPE_MYMUSIC;
    }
    MMIAPlayer_OpenList(list_index);
    SCI_TRACE_LOW("[APLIST] MMIMP3_OpenListForOPRl list_index = %d, is_music_list = %d", list_index,is_music_list);
}

/*****************************************************************************/
//  Description : release cur play file info
//  Global resource dependence :
//  Author:xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIMP3_ClearCurPlayInfo(void)
{
    uint16 list_index = s_cur_audioplayer_list_info.cur_list_index;
    BOOLEAN is_save_list = TRUE;

    if (s_cur_audioplayer_list_info.is_music_list)
    {
        list_index++;
        is_save_list = TRUE;
    }

    MMIMP3_CloseList(s_cur_audioplayer_list_info.is_music_list,s_cur_audioplayer_list_info.cur_list_index,is_save_list);

    if (PNULL != s_cur_audioplayer_list_info.play_list_ptr)
    {
        DestroyRandomList(s_cur_audioplayer_list_info.play_list_ptr);
        s_cur_audioplayer_list_info.play_list_ptr = PNULL;
    }

    SCI_MEMSET(&s_cur_audioplayer_list_info,0,sizeof(s_cur_audioplayer_list_info));
    s_cur_audioplayer_list_info.cur_list_index = INVALID_INDEX;

    SCI_TRACE_LOW("[APLIST] MMIMP3_CloseCurPlayList ");

}

/*****************************************************************************/
//  Description : init cur play file info
//  Global resource dependence :
//  Author:xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIMP3_InitCurPlayInfo(void)
{
	s_cur_audioplayer_list_info.is_music_list = TRUE;
	s_cur_audioplayer_list_info.cur_list_index = 0;

    SCI_TRACE_LOW("[APLIST] MMIMP3_InitCurPlayInfo ");
}

/*****************************************************************************/
//  Description : UpdateCurPlayFileInfoByName
//  Global resource dependence :
//  Author:xingdong.li
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN UpdateCurPlayFileInfoByName(void)
{
    BOOLEAN result = FALSE;
    uint16  file_index = 0;
    uint16  list_index = s_cur_audioplayer_list_info.cur_list_index;

    if (s_cur_audioplayer_list_info.is_music_list)
    {
        list_index++;
    }

    if(APLAYER_LIST_SUCCESS == MMIAPlayer_GetAudioFileIndexInList(list_index,
            s_cur_audioplayer_list_info.file_full_path,
            s_cur_audioplayer_list_info.file_full_path_len,
            &file_index))
    {
        s_cur_audioplayer_list_info.cur_file_index = file_index;
        UpdateCurPlayFileInfoByList();
        result = TRUE;
    }
    return result;
}

/*****************************************************************************/
//  Description : MMIMP3_UpdatePlayListInfo
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIMP3_UpdatePlayListInfo(void)
{
    uint16  file_index = 0;
    uint16  list_index = s_cur_audioplayer_list_info.cur_list_index;
	
    if (s_cur_audioplayer_list_info.is_music_list)
    {
        list_index++;
    }
	
    if(APLAYER_LIST_SUCCESS == MMIAPlayer_GetAudioFileIndexInList(list_index,
		s_cur_audioplayer_list_info.file_full_path,
		s_cur_audioplayer_list_info.file_full_path_len,
		&file_index))
    {
        s_cur_audioplayer_list_info.cur_file_index = file_index;
	}

}


/*****************************************************************************/
//  Description : update cur play file info
//  Global resource dependence :
//  Author:xingdong.li
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN UpdateCurPlayFileInfo(BOOLEAN is_music_list,
                                    uint16 list_selection,
                                    uint16 file_index,
                                    MMIMP3_PLAY_MODE_E play_mode)
{
    uint32    total_file_num = 0;
    BOOLEAN   result = FALSE;
    uint16    list_index = list_selection;
    wchar     full_path[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
    uint16    full_path_len = MMIFILE_FULL_PATH_MAX_LEN;
    MMI_ALISTHANDLE_T play_list_ptr = PNULL;
    uint16   active_index = 0;


    SCI_TRACE_LOW("[APLIST] UpdateCurPlayFileInfo is_music_list = %d, list_index = %d, file_index = %d, play_mode=%d", is_music_list,list_index,file_index, play_mode);

    if (is_music_list)
    {
        list_index += APLAYER_LIST_TYPE_MYMUSIC;
    }

    result = MMIAPlayer_GetAudioFileFullPath(list_index,file_index,full_path,&full_path_len);

    if (result)
    {
        total_file_num = MMIAPlayer_GetListSize(list_index);
        play_list_ptr = CreateRandomList(0,total_file_num,&active_index,file_index,play_mode);
        if (PNULL != play_list_ptr)
        {
            if (list_selection != s_cur_audioplayer_list_info.cur_list_index
                    || is_music_list != s_cur_audioplayer_list_info.is_music_list)
            {
                MMIMP3_ClearCurPlayInfo();
            }

            DestroyRandomList(s_cur_audioplayer_list_info.play_list_ptr);

            s_cur_audioplayer_list_info.play_list_ptr = play_list_ptr;
            s_cur_audioplayer_list_info.is_music_list = is_music_list;
            s_cur_audioplayer_list_info.cur_file_index = file_index;
            s_cur_audioplayer_list_info.cur_list_index = list_selection;
            s_cur_audioplayer_list_info.file_full_path_len = full_path_len;
            s_cur_audioplayer_list_info.active_index = active_index;
            s_cur_audioplayer_list_info.play_mode = play_mode;

            MMIAPICOM_Wstrncpy(s_cur_audioplayer_list_info.file_full_path,full_path,full_path_len);
            result = TRUE;
        }

    }

    return result;

}

/*****************************************************************************/
//  Description : get cur play list index
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC MMIMP3_PLAY_LIST_INFO_T MMIMP3_GetCurPlayListInfo(void)
{
    MMIMP3_PLAY_LIST_INFO_T play_list_info = {0};
    play_list_info.cur_list_index = s_cur_audioplayer_list_info.cur_list_index;
    play_list_info.is_music_list = s_cur_audioplayer_list_info.is_music_list;

    return play_list_info;
}

/*****************************************************************************/
//  Description : get cur play file index
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMIMP3_GetCurPlayFileIndex(void)
{
    return s_cur_audioplayer_list_info.cur_file_index;
}

/*****************************************************************************/
//  Description : get cur play file total num
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIMP3_GetCurPlayListTotalNum(void)
{
    uint16 list_index = s_cur_audioplayer_list_info.cur_list_index;

    if (s_cur_audioplayer_list_info.is_music_list)
    {
        list_index += APLAYER_LIST_TYPE_MYMUSIC;
    }
    return MMIAPlayer_GetListSize(list_index);
}

/*****************************************************************************/
//  Description : get cur file full path
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMP3_GetCurPlayFileFullPath(wchar * full_path_ptr,
        uint16 *full_path_len_ptr)
{
    BOOLEAN result = FALSE;

    if (PNULL != full_path_ptr
            && PNULL != full_path_len_ptr
            && 0 != *full_path_len_ptr)
    {
        SCI_MEMSET(full_path_ptr,0,(sizeof(wchar)*(*full_path_len_ptr)));
        *full_path_len_ptr = MIN(*full_path_len_ptr,s_cur_audioplayer_list_info.file_full_path_len);
        MMIAPICOM_Wstrncpy(full_path_ptr,s_cur_audioplayer_list_info.file_full_path,*full_path_len_ptr);
        result = TRUE;
    }
    return result;
}


/*****************************************************************************/
//  Description : get cur file full path
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMP3_GetCurPlayListFullPath(wchar * full_path_ptr,
        uint16 *full_path_len_ptr)
{
    BOOLEAN result = FALSE;
    uint16  list_index = s_cur_audioplayer_list_info.cur_list_index;

    if (PNULL != full_path_ptr
            && PNULL != full_path_len_ptr
            && 0 != *full_path_len_ptr)
    {
        if (s_cur_audioplayer_list_info.is_music_list)
        {
            list_index += APLAYER_LIST_TYPE_MYMUSIC;
        }
        result = MMIAPlayer_GetListFileFullPath(list_index,full_path_ptr,full_path_len_ptr);

    }
    return result;
}

/*****************************************************************************/
//  Description : UpdateCurPlayFileInfoByList
//  Global resource dependence :
//  Author:robert.wang
//  Note:
/*****************************************************************************/
LOCAL void UpdateCurPlayFileInfoByList(void)
{
    uint32 total_num  = MMIMP3_GetCurPlayListTotalNum();
    MMI_ALISTHANDLE_T play_list_ptr = PNULL;

    if (0 < total_num)
    {
        play_list_ptr = CreateRandomList(0,total_num,
                                         &(s_cur_audioplayer_list_info.active_index),
                                         s_cur_audioplayer_list_info.cur_file_index,
                                         s_cur_audioplayer_list_info.play_mode);

        if (PNULL != play_list_ptr)
        {
            if (PNULL != s_cur_audioplayer_list_info.play_list_ptr)
            {
                DestroyRandomList(s_cur_audioplayer_list_info.play_list_ptr);
            }
            s_cur_audioplayer_list_info.play_list_ptr = play_list_ptr;
        }


    }
}

/*****************************************************************************/
//  Description : UpdateCurPlayFileInfoByFileIndex
//  Global resource dependence :
//  Author:robert.wang
//  Note:
/*****************************************************************************/
LOCAL void UpdateCurPlayFileInfoByFileIndex(uint16 file_index,
        uint16 active_index)
{
    wchar full_path[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
    uint16 full_path_len = MMIFILE_FULL_PATH_MAX_LEN;

    if(MMIMP3_GetAudioFileFullPath(s_cur_audioplayer_list_info.is_music_list,
                                   s_cur_audioplayer_list_info.cur_list_index,
                                   file_index,
                                   full_path,
                                   &full_path_len))
    {
        s_cur_audioplayer_list_info.cur_file_index = file_index;
        s_cur_audioplayer_list_info.file_full_path_len = full_path_len;
        s_cur_audioplayer_list_info.active_index = active_index;
        SCI_MEMSET(s_cur_audioplayer_list_info.file_full_path, 0, sizeof(s_cur_audioplayer_list_info.file_full_path));
        MMIAPICOM_Wstrncpy(s_cur_audioplayer_list_info.file_full_path,full_path,full_path_len);
    }

}
/*****************************************************************************/
//  Description : MMIMP3_UpdateCurPlayIndexList
//  Global resource dependence :
//  Author:robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIMP3_SetCurPlayMode(MMIMP3_PLAY_MODE_E play_mode)
{
    s_cur_audioplayer_list_info.play_mode = play_mode;
    UpdateCurPlayFileInfoByList();
    SCI_TRACE_LOW("[APLIST] MMIMP3_SetCurPlayMode play_mode = %d",play_mode);
}


/*****************************************************************************/
//  Description : MMIMP3_IsCurPlayList
//  Global resource dependence :
//  Author:robert.wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMP3_IsCurPlayList(BOOLEAN is_music_list,
                                    uint16 list_index)
{
    if (is_music_list == s_cur_audioplayer_list_info.is_music_list
            && list_index == s_cur_audioplayer_list_info.cur_list_index)
    {
        return TRUE;
    }

    return FALSE;
}

/*****************************************************************************/
//  Description : MMIMP3_IsCurPlayList
//  Global resource dependence :
//  Author:robert.wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMP3_IsCurPlayFile(BOOLEAN is_music_list,
                                    uint16 cur_list_selection,
                                    uint16 item_index)
{
    if (is_music_list == s_cur_audioplayer_list_info.is_music_list
            && cur_list_selection == s_cur_audioplayer_list_info.cur_list_index
            && item_index == s_cur_audioplayer_list_info.cur_file_index)
    {
        return TRUE;
    }

    return FALSE;
}

/*****************************************************************************/
//  Description : MMIMP3_GetFilePathInCurPlayList
//  Global resource dependence :
//  Author:xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMP3_GetFilePathInCurPlayList(uint16 file_index,
        wchar *full_path_name_ptr,
        uint16 *full_path_len_ptr)
{
    BOOLEAN result = FALSE;
    uint16  list_index = s_cur_audioplayer_list_info.cur_list_index;

    if (s_cur_audioplayer_list_info.is_music_list)
    {
        list_index += APLAYER_LIST_TYPE_MYMUSIC;
    }
    result = MMIAPlayer_GetAudioFileFullPath(list_index,file_index,full_path_name_ptr,full_path_len_ptr);

    SCI_TRACE_LOW("[APLIST] MMIMP3_GetFilePathInCurPlayList file_index = %d, list_index = %d, result = %d",file_index,list_index,result);
    return result;

}

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
        uint16           *file_name_len_ptr  )
{
    BOOLEAN result = FALSE;
    wchar   full_path[MMIFILE_FULL_PATH_MAX_LEN +1] = {0};
    uint16  full_path_len = MMIFILE_FULL_PATH_MAX_LEN;
    wchar   device_ptr[MMIFILE_FULL_PATH_MAX_LEN +1] = {0};
    uint16  device_len = MMIFILE_FULL_PATH_MAX_LEN;

    if (is_music_list)
    {
        list_index += APLAYER_LIST_TYPE_MYMUSIC;
    }

    result =  MMIAPlayer_GetListFileFullPath(list_index,full_path,&full_path_len);
    SCI_TRACE_LOW("[APLIST] MMIMP3_GetListFileNameInfo is_music_list = %d, list_index = %d, result = %d",is_music_list,list_index,result);
    if (result)
    {

        if (PNULL != dev_type_ptr
                || PNULL != file_name_ptr)
        {
            result = MMIAPIFMM_SplitFullPath(full_path,
                                             full_path_len,
                                             device_ptr,
                                             &device_len,
                                             PNULL,
                                             PNULL,
                                             file_name_ptr,
                                             file_name_len_ptr);

            if (result
                    && PNULL != dev_type_ptr)
            {
                *dev_type_ptr = MMIAPIFMM_GetDeviceTypeByPath(device_ptr, device_len);
            }
        }
    }
    return result;

}

/*****************************************************************************/
//  Description : get list total num
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIMP3_GetListTotalNum(BOOLEAN is_music_list,
                                     uint16 list_index)
{
    if (is_music_list)
    {
        list_index += APLAYER_LIST_TYPE_MYMUSIC;
    }
    return MMIAPlayer_GetListSize(list_index);
}


/*****************************************************************************/
//  Description : get total list num
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIMP3_GetTotalListNum(void)
{
    return MMIAPlayer_GetTotalListNum() - 1;
}


/*****************************************************************************/
//  Description : MMIMP3_RenameListFile
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC APLAYER_LIST_ERROR_E MMIMP3_RenameListFile(BOOLEAN is_music_list,
        uint16 list_index,
        wchar * file_name_ptr,
        uint16 file_name_len)
{
    APLAYER_LIST_ERROR_E   result = APLAYER_LIST_ERROR;

    if(PNULL != file_name_ptr)
    {
        if (is_music_list)
        {
            list_index += APLAYER_LIST_TYPE_MYMUSIC;
        }

        result = MMIAPLayer_RenameList(list_index, file_name_ptr,file_name_len);
    }

    SCI_TRACE_LOW("[APLIST] MMIMP3_RenameListFile is_music_list = %d, list_index = %d, result = %d",is_music_list,list_index,result);
    return result;
}

/*****************************************************************************/
//  Description : MMIMP3_CopyList
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC APLAYER_LIST_ERROR_E MMIMP3_CopyList(BOOLEAN is_music_list,
        uint16 *list_index_ptr,
        MMIFILE_DEVICE_E dev)
{
    APLAYER_LIST_ERROR_E     result  = APLAYER_LIST_ERROR;
    uint16 list_index = 0;

    if (PNULL != list_index_ptr)
    {
        list_index = *list_index_ptr;
        if (is_music_list)
        {
            list_index += APLAYER_LIST_TYPE_MYMUSIC;
        }
        result = MMIAPlayer_CopyList(&list_index,dev);
        if (APLAYER_LIST_SUCCESS == result)
        {
            if (is_music_list
                    && list_index >= APLAYER_LIST_TYPE_MYMUSIC)
            {
                list_index -= APLAYER_LIST_TYPE_MYMUSIC;
            }
            *list_index_ptr = list_index;
        }
    }
    SCI_TRACE_LOW("[APLAYER] MMIMP3_CopyList  dev = %d, result = %d, list_index = %d",dev,result,list_index);
    return result;
}

/*****************************************************************************/
//  Description : MMIMP3_DeleteList
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMP3_DeleteList(BOOLEAN is_music_list,
                                 uint16 *list_index_ptr)
{

    BOOLEAN result = FALSE;
    uint32  total_list_num = 0;
    uint32  cur_list_index = 0;

    if (PNULL != list_index_ptr)
    {
        cur_list_index = *list_index_ptr;
        //清除当前播放信息
        if (MMIMP3_IsCurPlayList(is_music_list,cur_list_index))
        {
            MMIMP3_ClearCurPlayInfo();
        }

        //删除列表
        if (is_music_list)
        {
            cur_list_index += APLAYER_LIST_TYPE_MYMUSIC;
        }

        MMIAPlayer_DestroyList(cur_list_index);

        //获取当前列表
        total_list_num = MMIMP3_GetTotalListNum();

        if (*list_index_ptr >= total_list_num
                && 0 != total_list_num)
        {
            *list_index_ptr = total_list_num - 1;
        }
        result = TRUE;


        SCI_TRACE_LOW("[APLIST] MMIMP3_DeleteList is_music_list = %d, list_index = %d, result = %d",is_music_list,*list_index_ptr,result);
    }
    return result;
}

/*****************************************************************************/
//  Description : MMIMP3_ReleaseAllLists
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIMP3_ReleaseAllLists(void)
{
    MMIAPlayer_ReleaseAllList();
    MMIMP3_ClearCurPlayInfo();
}

/*****************************************************************************/
//  Description : MMIMP3_CloseList
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIMP3_CloseList(BOOLEAN is_music_list,
                             uint16 list_index,
                             BOOLEAN is_save_list_file)
{
    if (is_music_list)
    {
        list_index += APLAYER_LIST_TYPE_MYMUSIC;
    }
    MMIAPlayer_CloseList(list_index,is_save_list_file);
}


/*****************************************************************************/
//  Description : AddFileFromListToList
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN  AddFileFromListToList(BOOLEAN src_is_music_list,
                                     uint16  src_list_index,
                                     uint16  file_index,
                                     BOOLEAN dst_is_music_list,
                                     uint16  dst_list_index)
{
    if (src_is_music_list)
    {
        src_list_index += APLAYER_LIST_TYPE_MYMUSIC;
    }

    if (dst_is_music_list)
    {
        dst_list_index += APLAYER_LIST_TYPE_MYMUSIC;
    }

    return MMIAPlayer_AddListFileToList(src_list_index, file_index,dst_list_index);

}

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
        uint32  *added_num_ptr)
{
    APLAYER_LIST_ERROR_E result = APLAYER_LIST_ERROR;
    uint32   i = 0;



    if (PNULL != file_index_ptr
            && PNULL != added_num_ptr
            && 0 != add_total_num)
    {
        MMIMP3_OpenList(dst_is_music_list,dst_list_index);

        for (i = 0; i < add_total_num; i ++)
        {
            if(!AddFileFromListToList(src_is_music_list,src_list_index, file_index_ptr[i],dst_is_music_list,dst_list_index))
            {
                break;
            }

        }
        if (i > 0)
        {
            *added_num_ptr = i;
            result = APLAYER_LIST_SUCCESS;
        }

        if (MMIMP3_IsCurPlayList(dst_is_music_list,dst_list_index))
        {
            UpdateCurPlayFileInfoByList();
        }

        MMIMP3_CloseList(dst_is_music_list,dst_list_index,TRUE);

    }

    SCI_TRACE_LOW("[APLIST] MMIMP3_AddFileFromListToList i = %d, add_total_num = %d, result = %d",i,add_total_num,result);
    return result;
}


/*****************************************************************************/
//  Description : MMIMP3_CreateNewList
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC MMIFILE_DEVICE_E MMIMP3_GetListFileDev(BOOLEAN is_music_list,
        uint16 list_index)
{
    wchar list_full_path[MMIFILE_FULL_PATH_MAX_LEN +1] = {0};
    uint16 list_full_path_len = MMIFILE_FULL_PATH_MAX_LEN;

    if (is_music_list)
    {
        list_index += APLAYER_LIST_TYPE_MYMUSIC;
    }
    if(MMIAPlayer_GetListFileFullPath(list_index, list_full_path, &list_full_path_len))
    {
        return MMIAPIFMM_GetDeviceTypeByPath(list_full_path, MMIFILE_DEVICE_LEN);
    }

    return MMI_DEVICE_NUM;
}

/*****************************************************************************/
//  Description : MMIMP3_CreateListFile
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC APLAYER_LIST_ERROR_E MMIMP3_CreateList(uint16* list_index_ptr,
        wchar * file_name_ptr,
        uint16 file_name_len)
{
    APLAYER_LIST_ERROR_E   result = APLAYER_LIST_ERROR;

    if(PNULL != file_name_ptr
            && 0 != file_name_len)
    {

        result = MMIAPlayer_CreateNewListByFileName(file_name_ptr,file_name_len,list_index_ptr);
        if (PNULL != list_index_ptr
                && *list_index_ptr >= APLAYER_LIST_TYPE_MYMUSIC)
        {
            *list_index_ptr -= APLAYER_LIST_TYPE_MYMUSIC;
        }

    }
    return result;
}

/*****************************************************************************/
//  Description : Get New List Name With Number
//  Global resource dependence :
//  Author:xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMP3_GetNewListFileName(MMI_STRING_T default_name,
        wchar * list_file_name_ptr,
        uint16 *list_file_name_len_ptr)
{
    uint32 i = 0;
//    uint32 total_num = 0xFFFFFFFF;
    uint32 total_num = 0x0A;  //Bug 151063      
    uint8  init_num[16] = {0};
    uint16 init_len = 0;
    BOOLEAN result = FALSE;
    wchar   full_path[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
    uint16  full_path_len = MMIFILE_FULL_PATH_MAX_LEN;
    wchar  new_list_name[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
    uint16  new_list_name_len = MMIFILE_FULL_PATH_MAX_LEN;
    uint16  new_default_len = 0;


    if (PNULL != list_file_name_ptr
            && PNULL != list_file_name_len_ptr
            && 0 != *list_file_name_len_ptr
            && PNULL != default_name.wstr_ptr)
    {
        for(i = 1; i < total_num; i++)
        {
            sprintf((char*)init_num, "%d", i);
            init_len = (uint16)strlen((char*)init_num);

            SCI_MEMSET(new_list_name, 0x00, sizeof(wchar) * (MMIFILE_FULL_PATH_MAX_LEN + 1));
            new_list_name_len = MMIFILE_FULL_PATH_MAX_LEN;
            SCI_MEMSET(full_path, 0x00, sizeof(wchar) * (MMIFILE_FULL_PATH_MAX_LEN + 1));
            full_path_len = MMIFILE_FULL_PATH_MAX_LEN;

            MMI_WSTRNCPY(
                new_list_name, MMIFILE_FULL_PATH_MAX_LEN,
                default_name.wstr_ptr ,
                default_name.wstr_len,
                default_name.wstr_len
            );
            new_list_name_len = default_name.wstr_len;

            if (MMIFILE_FULL_PATH_MAX_LEN - new_list_name_len > init_len + MMIMP3_PLAY_LIST_SUFFIX_UCS_LEN)
            {
                MMI_STRNTOWSTR(
                    &new_list_name[new_list_name_len], MMIFILE_FULL_PATH_MAX_LEN - new_list_name_len,
                    init_num, init_len, init_len
                );

                new_list_name_len += init_len;
                new_default_len = new_list_name_len;

                MMI_WSTRNCPY(
                    &new_list_name[new_list_name_len], MMIFILE_FULL_PATH_MAX_LEN - new_list_name_len,
                    MMIMP3_PLAY_LIST_SUFFIX_UCS, MMIMP3_PLAY_LIST_SUFFIX_UCS_LEN, MMIMP3_PLAY_LIST_SUFFIX_UCS_LEN
                );

                new_list_name_len += MMIMP3_PLAY_LIST_SUFFIX_UCS_LEN;
                if(MMIAPlayer_GetListFilePath(new_list_name,new_list_name_len,full_path,&full_path_len))
                {
                    if(!MMIAPlayer_IsListExist(full_path, full_path_len))
                    {
                        SCI_MEMSET(list_file_name_ptr,0,(sizeof(wchar)*(*list_file_name_len_ptr)));
                        *list_file_name_len_ptr = MIN(*list_file_name_len_ptr,new_default_len);
                        MMIAPICOM_Wstrncpy(list_file_name_ptr,new_list_name,*list_file_name_len_ptr);
                        result = TRUE;
                        break;

                    }

                }

            }
        }

    }

    return result;
}


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
        uint16 *full_path_len_ptr)
{
    BOOLEAN result = FALSE;

    if (PNULL != full_path_len_ptr
            && PNULL != full_path_ptr)
    {
        if (is_music_list)
        {
            list_index += APLAYER_LIST_TYPE_MYMUSIC;
        }
        result = MMIAPlayer_GetAudioFileFullPath(list_index, file_index, full_path_ptr, full_path_len_ptr);
    }
    return result;
}

/*****************************************************************************/
//  Description : sort list
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMP3_SortList(BOOLEAN is_music_list,
                               uint16 list_index,
                               APLAYER_LIST_SORT_TYPE_E sort_type)
{
    BOOLEAN  result = FALSE;

    if (is_music_list)
    {
        list_index += APLAYER_LIST_TYPE_MYMUSIC;
    }
    result = MMIAPlayer_SortList(list_index,sort_type);

    if (result)
    {
        if (is_music_list)
        {
            list_index -= APLAYER_LIST_TYPE_MYMUSIC;
        }
        if (MMIMP3_IsCurPlayList(is_music_list,list_index))
        {
            UpdateCurPlayFileInfoByName();
        }
    }
    SCI_TRACE_LOW("[APLIST] MMIMP3_SortList is_music_list = %d, sort_type = %d, result = %d",is_music_list,sort_type,result);
    return result;
}

/*****************************************************************************/
//  Description : MMIMP3_GetListSortType
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC APLAYER_LIST_SORT_TYPE_E MMIMP3_GetListSortType(BOOLEAN is_music_list,
        uint16 list_index)
{

    if (is_music_list)
    {
        list_index += APLAYER_LIST_TYPE_MYMUSIC;
    }
    return MMIAPlayer_GetListSortType(list_index);
}

/*****************************************************************************/
//  Description : start mp3 del task
//  Global resource dependence :
//  Author: paul.huang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMP3_StartDelFile(MMIMP3_LIST_DEL_INFO_T* opr_info_ptr)
{
    MP3_DEL_SIG_T *sig_ptr = PNULL;
    BOOLEAN result = FALSE;
    if(SCI_INVALID_BLOCK_ID == g_app_mp3_del_task_id)
    {
        g_app_mp3_del_task_id = SCI_CreateAppThread(
                                    "T_P_APP_MP3_DEL_TASK",
                                    "Q_P_APP_MP3_DEL_TASK",
                                    MP3Del_Task,
                                    0,
                                    0,
                                    P_APP_MP3_DEL_STACK_SIZE,
                                    P_APP_MP3_DEL_QUEUE_NUM,
                                    P_APP_MP3_DEL_TASK_PRIORITY,
                                    SCI_PREEMPT,
                                    SCI_AUTO_START);
    }

    if(SCI_INVALID_BLOCK_ID != g_app_mp3_del_task_id)
    {
        MMI_CREATE_SIGNAL(sig_ptr,
                          MP3_START_DEL_SIG,
                          sizeof(MP3_DEL_SIG_T),
                          P_APP
                         );

        sig_ptr->data_ptr = opr_info_ptr;
        MMI_SEND_SIGNAL(sig_ptr, g_app_mp3_del_task_id);

        result = TRUE;
    }

    return result;
}

/*****************************************************************************/
//  Description : mp3 del task
//  Global resource dependence :
//  Author: paul.huang
//  Note:
/*****************************************************************************/
LOCAL void MP3Del_Task(
    uint32 argc,
    void * argv
)
{
    xSignalHeaderRec    *psig = NULL;
    MMIMP3_LIST_DEL_INFO_T* opr_info_ptr = PNULL;
    MP3_DEL_SIG_T *sig_ptr = PNULL;

    while(1) /*lint !e716*/
    {
        MMI_RECEIVE_SIGNAL(psig, SCI_IdentifyThread());/*lint !e63*/
        //psig = s_cur_sig;
        SCI_TRACE_LOW("[MMIMP3]MP3Del_Task psig->SignalCode = 0x%x", psig->SignalCode);/*lint !e613*/

        switch(psig->SignalCode) /*lint !e613*/
        {
        case MP3_START_DEL_SIG:
            sig_ptr = (MP3_DEL_SIG_T *)psig;
            opr_info_ptr = (MMIMP3_LIST_DEL_INFO_T*)sig_ptr->data_ptr;/*lint !e613*/
            DelFileInfo(opr_info_ptr);
            MMIAPICOM_OtherTaskToMMI(MMIMP3_DEL_FILE_WAIT_WIN_ID, MSG_MP3_DEL_CNF, &opr_info_ptr, sizeof(opr_info_ptr));
            break;

        case MP3_STOP_DEL_SIG:
            MMIAPICOM_OtherTaskToMMI(MMIMP3_DEL_FILE_WAIT_WIN_ID, MSG_MP3_DEL_DONE, (DPARAM)PNULL, 0);
            break;

        default:
            break;
        }

        MMI_FREE_SIGNAL(psig);
    }
}

/*****************************************************************************/
//  Description : Delete music files from list file or list array.
//  Global resource dependence :
//  Author:aoke.hu
//  Note:
/*****************************************************************************/
LOCAL void DelFileInfo(MMIMP3_LIST_DEL_INFO_T* opr_info_ptr)
{

    uint32 i = 0;
    APLAYER_LIST_ERROR_E  del_result  = APLAYER_LIST_INVALID_PARAM;
    uint16 list_index = 0;
    uint32 total_num = 0;
    BOOLEAN is_cur_file = FALSE;


    SCI_TRACE_LOW("[APLIST] DelFileInfo ");

    if (PNULL != opr_info_ptr
            && PNULL != opr_info_ptr->select_index_ptr)
    {
    
	    list_index = opr_info_ptr->list_selection;
	 
        if (opr_info_ptr->is_music_list)
        {
            list_index+= APLAYER_LIST_TYPE_MYMUSIC;
        }

        for (i = opr_info_ptr->select_num -1; i >= 0 && i < opr_info_ptr->select_num; i--) /*lint !e685 !e568*/
        {

            //删除文件信息
            del_result = MMIAPlayer_DeleteFileFromList(list_index,
                         opr_info_ptr->select_index_ptr[i],
                         opr_info_ptr->is_del_file);

            if (APLAYER_LIST_SUCCESS == del_result
                    || APLAYER_LIST_DELETE_FILE_ERROR == del_result)
            {

                //更新当前列表
                if (MMIMP3_IsCurPlayFile(opr_info_ptr->is_music_list,opr_info_ptr->list_selection,opr_info_ptr->select_index_ptr[i]))
                {
                    //MMIMP3_ClearCurPlayInfo();
                    s_cur_audioplayer_list_info.cur_file_index = 0;
                    is_cur_file = TRUE;
                }

                //更新当前xuanxiang
                total_num = MMIMP3_GetListTotalNum(opr_info_ptr->is_music_list,opr_info_ptr->list_selection);

                if (total_num <= opr_info_ptr->cur_file_selection)
                {

                    opr_info_ptr->cur_file_selection = 0;
                }
                if(total_num <= MMIMP3_GetCurPlayFileIndex())
                {
                    s_cur_audioplayer_list_info.cur_file_index = 0;
                }
                opr_info_ptr->del_num ++;
            }
        }
        MMIAPLAYER_SaveListFile(list_index);

        //更新播放列表
        if (MMIMP3_IsCurPlayList(opr_info_ptr->is_music_list,opr_info_ptr->list_selection))
        {
            UpdateCurPlayFileInfoByList();
            if(is_cur_file)
            {
                wchar     full_path[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
                uint16    full_path_len = MMIFILE_FULL_PATH_MAX_LEN;


                
                list_index = s_cur_audioplayer_list_info.cur_list_index;

                MMIMP3_GetAudioFileFullPath(s_cur_audioplayer_list_info.is_music_list, list_index, 
                    s_cur_audioplayer_list_info.cur_file_index, full_path, &full_path_len);
                
                MMIAPICOM_Wstrncpy(s_cur_audioplayer_list_info.file_full_path,full_path,full_path_len);
                s_cur_audioplayer_list_info.file_full_path_len = full_path_len;
            }
        }


    }


}

/*****************************************************************************/
//  Description : stop mp3 del task
//  Global resource dependence :
//  Author: paul.huang
//  Note:
/*****************************************************************************/
PUBLIC void MMIMP3_StopDelFile(void)
{
    MP3_DEL_SIG_T *sig_ptr = PNULL;

    SCI_TRACE_LOW("[APLIST] MMIMP3_StopDelFile ");
    if(SCI_INVALID_BLOCK_ID != g_app_mp3_del_task_id)
    {
        MMI_CREATE_SIGNAL(sig_ptr,
                          MP3_STOP_DEL_SIG,
                          sizeof(MP3_DEL_SIG_T),
                          P_APP
                         );

        MMI_SEND_SIGNAL(sig_ptr, g_app_mp3_del_task_id);
    }
}



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
                                   )
{
    BOOLEAN result = FALSE;
    APLAYER_LIST_ITEM_DATA_T item_data= {0};

    if (PNULL == file_full_path_ptr)
    {
        return result;
    }

    MMIMP3_OpenList(FALSE,0);

    SCI_MEMSET(&item_data, 0, sizeof(APLAYER_LIST_ITEM_DATA_T));
    SCI_MEMSET(item_data.filename, 0, sizeof(item_data.filename));
    MMI_WSTRNCPY(item_data.filename,
                 MMIFILE_FULL_PATH_MAX_LEN,
                 file_full_path_ptr,
                 file_full_path_len,
                 file_full_path_len);

    item_data.name_len = file_full_path_len;

    result = MMIAPlayer_AddFileToList(0,item_data);
    if (!result)
    {
        MMIMP3_CloseList(FALSE,0,FALSE);
    }
    else
    {
        UpdateCurPlayFileInfo(FALSE,0,0, play_mode);
    }


    SCI_TRACE_LOW("[APLIST] MMIMP3_LoadAudioFile result = %d",result);
    return result;
}

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
    MMIMP3_PLAY_MODE_E play_mode)
{
    BOOLEAN result = FALSE;
    uint16 music_num = 0;
    uint16 i = 0;
    MMI_TM_T tm_time = {0};
    APLAYER_LIST_ITEM_DATA_T item_data= {0};


    MMIMP3_OpenList(FALSE,0);

    music_num = 0;
    for(i = 0; i < file_num; i++)
    {
        if(MMIFMM_FILE_TYPE_MUSIC == file_info_ptr->data[i]->type
#ifdef DRM_SUPPORT
                || MMIFMM_FILE_TYPE_DRM == file_info_ptr->data[i]->type
#endif
          )
        {
            SCI_MEMSET(&item_data, 0, sizeof(APLAYER_LIST_ITEM_DATA_T));
            SCI_MEMSET(item_data.filename, 0, sizeof(item_data.filename));
            if(file_info_ptr->path_is_valid)
            {
                MMI_WSTRNCPY(item_data.filename,
                             MMIFILE_FULL_PATH_MAX_LEN,
                             file_info_ptr->data[i]->filename_ptr,
                             file_info_ptr->data[i]->filename_len,
                             file_info_ptr->data[i]->filename_len);
                item_data.name_len = file_info_ptr->data[i]->filename_len;
            }
            else
            {
                MMI_WSTRNCPY(item_data.filename,
                             MMIFILE_FULL_PATH_MAX_LEN,
                             dir_path_ptr,
                             dir_path_len,
                             dir_path_len);
                item_data.filename[dir_path_len] = MMIFILE_SLASH;
                MMI_WSTRNCPY(&item_data.filename[dir_path_len + 1],
                             MMIFILE_FULL_PATH_MAX_LEN - (dir_path_len + 1),
                             file_info_ptr->data[i]->filename_ptr,
                             file_info_ptr->data[i]->filename_len, file_info_ptr->data[i]->filename_len);
                item_data.name_len = (dir_path_len + 1) + file_info_ptr->data[i]->filename_len;
            }
#ifdef DRM_SUPPORT
            if(!MMIAPIMP3_IsMp3PlayerLoadType(item_data.filename, item_data.name_len))
            {
                if (i == file_index)
                {
                    break;
                }
                else
                {
                    continue;
                }
            }
#endif
            tm_time = MMIAPICOM_Second2Tm(file_info_ptr->data[i]->time);
            item_data.modify_date.mday = tm_time.tm_mday;
            item_data.modify_date.mon = tm_time.tm_mon;
            item_data.modify_date.wday = tm_time.tm_wday;
            item_data.modify_date.year = tm_time.tm_year;
            item_data.modify_time.hour = tm_time.tm_hour;
            item_data.modify_time.min = tm_time.tm_min;
            item_data.modify_time.sec = tm_time.tm_sec;
            item_data.size = file_info_ptr->data[i]->file_size;

            if(!MMIAPlayer_AddFileToList(0,item_data))
            {
                if (i == file_index)
                {
                    break;
                }
                else
                {
                    continue;
                }
            }
            else if(i == file_index)
            {
                file_index = music_num;
            }
            music_num++;
        }
    }
    if (i == file_num)
    {
        UpdateCurPlayFileInfo(FALSE,0,file_index,play_mode);
        result = TRUE;
    }
    else
    {
        MMIMP3_CloseList(FALSE,0,FALSE);
    }


    SCI_TRACE_LOW("[APLIST] MMIMP3_LoadFmmPlayList result = %d",result);
    return result;
}

/*****************************************************************************/
//  Description : resize all buffer size
//  Global resource dependence :
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIMP3_ResizeAllListBufferSize(BOOLEAN is_decrease)
{
    uint32 size = 0;


    if(is_decrease)
    {
        size = 2;
    }
    else
    {
        size = 50;
    }

    MMIAPlayer_SetAllListBufferSize(size);

#ifdef MMIAP_MUSICMARK_SUPPORT
    MMIAP_ResizeMusicMarkBufferSize(size);
#endif
    
    SCI_TRACE_LOW("[MMIMP3] MMIMP3_ResizeAllListBufferSize is_decrease=%d,size=%d", is_decrease, size);
}




/*****************************************************************************/
//  Description : MMIMP3_LoadLists
//  Global resource dependence :
//  Author:xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIMP3_LoadLists(void)
{
    SCI_TRACE_LOW("[APLIST] MMIMP3_LoadLists");
    MMIAPlayer_InitAllList();
    //MMIMP3_ClearCurPlayInfo();   //bug2135595
    MMIMP3_InitCurPlayInfo();
}

/*****************************************************************************/
//  Description : MMIMP3_SearchAudioFilesInDefaultFolder
//  Global resource dependence :
//  Author:xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIMP3_SearchAudioFilesInDefaultFolder(MMI_WIN_ID_T      win_id,
        MMI_MESSAGE_ID_E  end_msg_id,
        MMI_MESSAGE_ID_E  next_msg_id,
        MMIFILE_DEVICE_E tem_dev)
{
    wchar full_dir_path[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
    uint16 full_dir_path_len = MMIFILE_FULL_PATH_MAX_LEN;
    MMIFMM_FILTER_T filter = {0};


    SCI_TRACE_LOW("[APLIST] MMIMP3_SearchAudioFilesInDefaultFolder tem_dev = %d", tem_dev);

    if (tem_dev >= MMI_DEVICE_NUM)
    {
        MMK_PostMsg(win_id,end_msg_id,PNULL,PNULL);
        return;
    }

    if(!MMIAPIFMM_GetDeviceTypeStatus(tem_dev))
    {
        MMK_PostMsg(win_id,next_msg_id,PNULL,PNULL);
    }

    if(s_file_array == PNULL)
    {
        s_file_array = MMIAPIFILEARRAY_Create();
        if (PNULL == s_file_array)
        {
            MMK_PostMsg(win_id,end_msg_id,PNULL,PNULL);
            return;
        }
    }
    SCI_MEMSET(full_dir_path, 0x00, (MMIFILE_FULL_PATH_MAX_LEN + 1) * sizeof(wchar));
    MMIMP3_GetMusicFilter(&filter);

    MMIAPIFMM_CombineFullPath(MMIAPIFMM_GetDevicePath(tem_dev),
                              MMIAPIFMM_GetDevicePathLen(tem_dev),
                              MMIMULTIM_DIR_MUSIC,
                              MMIAPICOM_Wstrlen(MMIMULTIM_DIR_MUSIC),
                              PNULL,
                              NULL,
                              full_dir_path,
                              &full_dir_path_len);

    MMIFMM_SearchFileInPathNoSort(full_dir_path,
                                  full_dir_path_len,
                                  &filter,
                                  TRUE,
                                  FUNC_FIND_FILE,
                                  s_file_array/*[index]*/,
                                  win_id,
                                  next_msg_id);
}

/*****************************************************************************/
//  Description : LoadFilesToList
//  Global resource dependence :
//  Author:xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMP3_AppendFilesToList(BOOLEAN is_music_list,
                                        uint16 list_index,
                                        FILEARRAY file_array,
                                        uint32 *add_num_ptr)
{
    uint32   i = 0;
    FILEARRAY_DATA_T          file_array_data = {0};
    uint32 total_num = 0;
    BOOLEAN   result = FALSE;
    uint32 add_max_num = 0;

    if (PNULL != file_array
            && PNULL != add_num_ptr)
    {
        add_max_num = *add_num_ptr;

        total_num = MMIAPIFILEARRAY_GetArraySize(file_array);
        if (total_num>add_max_num)
        {
			total_num = add_max_num;
        }

        *add_num_ptr = 0;
        for (i = 0; i < total_num; i++)
        {
            if(MMIAPIFILEARRAY_Read(file_array,i,&file_array_data)
                    && AddFileToList(file_array_data,is_music_list,list_index))
            {
                (*add_num_ptr)++;
                result = TRUE;
            }
        }
    }

    if (is_music_list)
    {
        list_index+=APLAYER_LIST_TYPE_MYMUSIC;
    }
    MMIAPLAYER_SaveListFile(list_index);
    if (result)
    {
        UpdateCurPlayFileInfoByList();
    }
    SCI_TRACE_LOW("[APLIST] MMIMP3_AppendFilesToList result = %d", result);
    return result;
}


/*****************************************************************************/
//  Description : ClearList
//  Global resource dependence :
//  Author:xingdong.li
//  Note:
/*****************************************************************************/
LOCAL void ClearList(BOOLEAN is_music_list,
                     uint16 list_index)
{
    uint32   i = 0;
    uint16   total_num = 0;

    if (is_music_list)
    {
        list_index+= APLAYER_LIST_TYPE_MYMUSIC;
    }

    total_num = MMIAPlayer_GetListSize(list_index);
    for (i = 0; i < total_num; i++)
    {
        MMIAPlayer_DeleteFileFromList(list_index,0,FALSE);
    }
    MMIAPLAYER_SaveListFile(list_index);

}

/*****************************************************************************/
//  Description : AddFileToList
//  Global resource dependence :
//  Author:xingdong.li
//  Note:添加一个文件到列表中
/*****************************************************************************/
LOCAL BOOLEAN AddFileToList(FILEARRAY_DATA_T  file_array_data,
                            BOOLEAN is_music_list,
                            uint16 dst_list_index)
{
    APLAYER_LIST_ITEM_DATA_T   item_data = {0};

    item_data.modify_date.mday = file_array_data.modify_date.mday;
    item_data.modify_date.mon = file_array_data.modify_date.mon;
    item_data.modify_date.wday = file_array_data.modify_date.wday;
    item_data.modify_date.year = file_array_data.modify_date.year;
    item_data.modify_time.hour = file_array_data.modify_time.hour;
    item_data.modify_time.min = file_array_data.modify_time.min;
    item_data.modify_time.sec = file_array_data.modify_time.sec;
    item_data.size = file_array_data.size;
    SCI_MEMSET(item_data.filename,0,(sizeof(wchar)*(MMIFILE_FULL_PATH_MAX_LEN+1)));
    item_data.name_len = MIN(file_array_data.name_len, MMIFILE_FULL_PATH_MAX_LEN);
    MMIAPICOM_Wstrncpy(item_data.filename,file_array_data.filename,item_data.name_len);

    if (is_music_list)
    {
        dst_list_index++;
    }

    return MMIAPlayer_AddFileToList(dst_list_index,item_data);
}

/*****************************************************************************/
//  Description : LoadFilesToList
//  Global resource dependence :
//  Author:xingdong.li
//  Note:从头开始添加多个文件到列表中
/*****************************************************************************/
LOCAL BOOLEAN AddFilesToList(BOOLEAN is_music_list,
                             uint16 list_index,
                             FILEARRAY file_array,
                             uint32 *add_num_ptr)
{
    uint32   i = 0;
    FILEARRAY_DATA_T          file_array_data = {0};
    uint32 total_num = 0;
    BOOLEAN   result = FALSE;


    if (PNULL != file_array
            && PNULL != add_num_ptr)
    {
        total_num = MMIAPIFILEARRAY_GetArraySize(file_array);
//        if (0 < total_num)
		if (total_num >= 0)   //Bug 132825
        {
            
			if (total_num > MMIAP_PLAY_LIST_MAX_NUM)
			{
				total_num = MMIAP_PLAY_LIST_MAX_NUM;
			}			
			
			*add_num_ptr = 0;
            //清除列表
            ClearList(is_music_list,list_index);

            //添加文件到列表中
            for (i = 0; i < total_num; i++)
            {
                if(MMIAPIFILEARRAY_Read(file_array,i,&file_array_data)
                        && AddFileToList(file_array_data, is_music_list,list_index))
                {
                    (*add_num_ptr)++;
                    result = TRUE;
                }
            }

            //排序
            MMIMP3_SortList(is_music_list,list_index,MMIMP3_GetListSortType(is_music_list,list_index));

            //更新当前播放信息
            if (MMIMP3_IsCurPlayList(is_music_list,list_index))
            {
                UpdateCurPlayFileInfoByName();
            }

        }
    }


    SCI_TRACE_LOW("[APLIST] AddFilesToList result = %d", result);
    return result;
}


/*****************************************************************************/
//  Description : MMIMP3_StopLoadAudioFiles
//  Global resource dependence :
//  Author:xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIMP3_StopLoadAudioFiles(void)
{

    SCI_TRACE_LOW("[APLIST] MMIMP3_StopLoadAudioFiles ");
    if (PNULL != s_file_array)
    {
        MMIAPIFMM_SearchFileStop();
    }
}



/*****************************************************************************/
//  Description : MMIMP3_SetCurPlayInfo
//  Global resource dependence :
//  Author:robert.wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMP3_SetCurPlayInfo(BOOLEAN is_music_list,
                                     uint16 list_index,
                                     uint16 file_index,
                                     MMIMP3_PLAY_MODE_E play_mode)
{
    BOOLEAN  result = FALSE;

    if (!MMIMP3_IsCurPlayList(is_music_list,list_index))
    {
        MMIMP3_OpenList(is_music_list,list_index);
    }
    result = UpdateCurPlayFileInfo(is_music_list,list_index,file_index,play_mode);
    if (!result
            && !MMIMP3_IsCurPlayList(is_music_list,list_index))
    {
        MMIMP3_CloseList(is_music_list,list_index,FALSE);
    }
    return result;
}

/*****************************************************************************/
//  Description : MMIMP3_GetPreOrNextPlayFileIndex
//  Global resource dependence :
//  Author:robert.wang
//  Note:user op
/*****************************************************************************/
PUBLIC BOOLEAN MMIMP3_GetPreOrNextPlayFileIndex(BOOLEAN is_prev,
        uint16 *next_file_index_ptr //out
                                               )
{
    int32 offset = 0;

    if (is_prev)
    {
        offset = -1;
    }
    else
    {
        offset = 1;
    }

    return MMIMP3_GetFileIndexByOffset(offset, next_file_index_ptr);
}

/*****************************************************************************/
//  Description : MMIMP3_GetFileIndexByOffset
//  Global resource dependence :
//  Author:robert.wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMP3_GetFileIndexByOffset(int32 offset,
        uint16 *next_file_index_ptr)
{
    if (MMIMP3_PLAY_ONE_REPEAT == s_cur_audioplayer_list_info.play_mode)
    {
        return GetPlayFileIndex(offset,next_file_index_ptr);
    }
    else
    {
        return GetPlayFileIndexByMode(offset,next_file_index_ptr);
    }
}

/*****************************************************************************/
//  Description : MMIMP3_ResetCurPlayFileIndex
//  Author:robert.wang
//  Note:此函数用于非播放状态的歌曲切换，
/*****************************************************************************/
PUBLIC BOOLEAN MMIMP3_ResetCurPlayFileIndex(BOOLEAN is_prev)
{
    BOOLEAN result = FALSE;
    uint16  active_index = s_cur_audioplayer_list_info.active_index;
    uint16  file_index = s_cur_audioplayer_list_info.cur_file_index;
    uint32  total_num = 0;
    uint16   *file_index_ptr = PNULL;
    //uint16   *cur_file_index_ptr = PNULL;
    int32 offset = 0;

    if (is_prev)
    {
        offset = -1;
    }
    else
    {
        offset = 1;
    }


    SCI_TRACE_LOW("[APLIST] MMIMP3_SetCurPlayFileIndex ");

    if (MMIMP3_PLAY_ONE_REPEAT == s_cur_audioplayer_list_info.play_mode)
    {
        result = GetPlayFileIndex(offset,&file_index);
        if (result)
        {
            result = UpdateCurPlayFileInfo(s_cur_audioplayer_list_info.is_music_list,s_cur_audioplayer_list_info.cur_list_index,file_index,s_cur_audioplayer_list_info.play_mode);
        }
    }
    else
    {
        /*
            total_num = MMI_ALIST_GetSize(s_cur_audioplayer_list_info.play_list_ptr);
            result = GetIndex(offset,&active_index,total_num);
            if (result)
            {
                file_index_ptr = MMI_ALIST_Remove(s_cur_audioplayer_list_info.play_list_ptr,active_index,PNULL);
                cur_file_index_ptr = MMI_ALIST_Get(s_cur_audioplayer_list_info.play_list_ptr,s_cur_audioplayer_list_info.active_index);
                if (PNULL != file_index_ptr
                        && PNULL != cur_file_index_ptr)
                {
                    s_cur_audioplayer_list_info.cur_file_index = *file_index_ptr;
                    s_cur_audioplayer_list_info.file_full_path_len = MMIFILE_FULL_PATH_MAX_LEN;
                    MMIMP3_GetAudioFileFullPath(s_cur_audioplayer_list_info.is_music_list,
                                                s_cur_audioplayer_list_info.cur_list_index,
                                                s_cur_audioplayer_list_info.cur_file_index,
                                                s_cur_audioplayer_list_info.file_full_path,
                                                &(s_cur_audioplayer_list_info.file_full_path_len));

                    MMI_ALIST_Set(s_cur_audioplayer_list_info.play_list_ptr,s_cur_audioplayer_list_info.active_index,file_index_ptr);
                    if (is_prev)
                    {
                        MMI_ALIST_Insert(s_cur_audioplayer_list_info.play_list_ptr,s_cur_audioplayer_list_info.active_index+1,cur_file_index_ptr);
                    }
                    else
                    {
                        MMI_ALIST_Append(s_cur_audioplayer_list_info.play_list_ptr,cur_file_index_ptr);
                    }

                    result = TRUE;
                }
            }
        */
        if (PNULL != s_cur_audioplayer_list_info.play_list_ptr)
        {
            total_num = MMI_ALIST_GetSize(s_cur_audioplayer_list_info.play_list_ptr);
            result = GetIndex(offset,&active_index,total_num);
            if (result)
            {
                file_index_ptr = MMI_ALIST_Get(s_cur_audioplayer_list_info.play_list_ptr,active_index);
                if (PNULL != file_index_ptr)
                {
                    UpdateCurPlayFileInfoByFileIndex(*file_index_ptr, active_index);
                    result = TRUE;
                }


                /* 循环随机播放，循环结束后，再随机
                */
                if ((MMIMP3_PLAY_RANDOM_REPEAT == s_cur_audioplayer_list_info.play_mode)
                        && (0 == active_index)
                   )
                {
                    UpdateCurPlayFileInfoByList();
                }
            }
        }
    }

    return result;

}

/*****************************************************************************/
//  Description : MMIMP3_SetCurPlayFileIndex
//  Global resource dependence :
//  Author:robert.wang
//  Note:此函数用于自动播放状态的歌曲切换
/*****************************************************************************/
PUBLIC BOOLEAN MMIMP3_SetCurPlayFileIndex(BOOLEAN is_prev)
{
    BOOLEAN result = FALSE;
    uint16  active_index = s_cur_audioplayer_list_info.active_index;
    uint32  total_num = 0;
    uint16   *file_index_ptr = PNULL;
    int32 offset = 0;


    SCI_TRACE_LOW("[APLIST] MMIMP3_SetCurPlayFileIndex ");

    if (PNULL != s_cur_audioplayer_list_info.play_list_ptr)
    {
        total_num = MMI_ALIST_GetSize(s_cur_audioplayer_list_info.play_list_ptr);
        if (is_prev)
        {
            offset = -1;
        }
        else
        {
            offset = 1;
        }
        result = GetIndex(offset,&active_index,total_num);
        if (result)
        {
            /*
            * 若是MMIMP3_PLAY_LIST_ONCE 或MMIMP3_PLAY_RANDOM_ONCE
            * 播放到最后一首时，停止播放。
            */
            if (MMIMP3_PLAY_LIST_ONCE == s_cur_audioplayer_list_info.play_mode
                    || MMIMP3_PLAY_RANDOM_ONCE == s_cur_audioplayer_list_info.play_mode)
            {
                if (0 == active_index)
                {
                    result = FALSE;
                    return result;
                }
            }

            file_index_ptr = MMI_ALIST_Get(s_cur_audioplayer_list_info.play_list_ptr,active_index);
            if (PNULL != file_index_ptr)
            {
                UpdateCurPlayFileInfoByFileIndex(*file_index_ptr, active_index);
                result = TRUE;
            }


            /* 循环随机播放，循环结束后，再随机
            */
            if ((MMIMP3_PLAY_RANDOM_REPEAT == s_cur_audioplayer_list_info.play_mode)
                    && (0 == active_index)
               )
            {
                UpdateCurPlayFileInfoByList();
            }
        }
    }


    SCI_TRACE_LOW("[APLIST] MMIMP3_SetCurPlayFileIndex result=%d",
                 result);

    return result;

}

/*****************************************************************************/
//  Description : GetPreOrNextIndexByList
//  Global resource dependence :
//  Author:robert.wang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetPlayFileIndex(int32 offset,
                               uint16 *next_file_index_ptr //out
                              )
{
    BOOLEAN result = FALSE;
    uint32  total_num = 0;
    uint16  list_index = s_cur_audioplayer_list_info.cur_list_index;


    if (PNULL != next_file_index_ptr)
    {
        *next_file_index_ptr = s_cur_audioplayer_list_info.cur_file_index;
        if (s_cur_audioplayer_list_info.is_music_list)
        {
            list_index++;
        }
        total_num = MMIAPlayer_GetListSize(list_index);
        result = GetIndex(offset, next_file_index_ptr,total_num);

    }

    SCI_TRACE_LOW("[APLIST] GetPreOrNextIndexByList result = %d, offset = %d, total_num=%d",result,offset,total_num);
    return result;
}

/*****************************************************************************/
//  Description : GetIndex
//  Global resource dependence :
//  Author:robert.wang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetIndex(int32 offset,
                       uint16 *cur_index_ptr,
                       uint32 total_num)
{
    BOOLEAN result = FALSE;

    if (PNULL != cur_index_ptr
            && total_num > 0)
    {

        if (0 > offset)
        {
            *cur_index_ptr = abs(*cur_index_ptr+(total_num + offset)) % total_num; /*lint !e737 !e573*/

        }
        else if(0 < offset)
        {
            *cur_index_ptr = (*cur_index_ptr + offset)%total_num; /*lint !e737 !e573*/
        }

        result = TRUE;

    }

    return result;

}


/*****************************************************************************/
//  Description : GetPreOrNextIndexByList
//  Global resource dependence :
//  Author:robert.wang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetPlayFileIndexByMode(int32 offset,
                                     uint16 *next_file_index_ptr //out
                                    )
{
    BOOLEAN result = FALSE;
    uint32  total_num = 0;
    uint16  active_index = 0;
    uint16   *file_index_ptr = PNULL;

    if (PNULL != next_file_index_ptr
            && PNULL != s_cur_audioplayer_list_info.play_list_ptr)
    {
        total_num = MMI_ALIST_GetSize(s_cur_audioplayer_list_info.play_list_ptr);
        active_index = s_cur_audioplayer_list_info.active_index;
        result = GetIndex(offset,&active_index,total_num);

        if (result)
        {
            file_index_ptr = MMI_ALIST_Get(s_cur_audioplayer_list_info.play_list_ptr,active_index);
            if (PNULL != file_index_ptr)
            {
                *next_file_index_ptr = *file_index_ptr;
                result = TRUE;
            }
        }
    }

    SCI_TRACE_LOW("[APLIST] GetPreOrNextIndexByList result = %d, offset = %d, total_num=%d, active_index = %d ",result,offset,total_num,active_index);
    return result;
}



/*****************************************************************************/
//  Description : MMIMP3_SearchAllAudioFiles
//  Global resource dependence :
//  Author:robert.wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMP3_SearchAllAudioFiles(
    BOOLEAN is_music_list,
    uint16 list_index,
    MMI_WIN_ID_T      win_id,
    MMI_MESSAGE_ID_E  msg_id)
{
    MMIFMM_FILTER_T filter = {0};
    BOOLEAN    result = FALSE;

    SCI_TRACE_LOW("[APLIST] MMIMP3_SearchAllAudioFiles ");

    if (PNULL != s_file_array)
    {
        MMIAPIFILEARRAY_Destroy(&s_file_array);
        s_file_array = PNULL;
    }

    if(s_file_array == PNULL)
    {
        s_file_array = MMIAPIFILEARRAY_Create();
        if (PNULL == s_file_array)
        {
            return result;
        }
    }

    MMIMP3_GetMusicFilter(&filter);

    MMIFMM_SearchFileInPathNoSort(PNULL,
                                  0,
                                  &filter,
                                  TRUE,
                                  FUNC_FIND_FILE,
                                  s_file_array/*[index]*/,
                                  win_id,
                                  msg_id);
    result = TRUE;
    return result;

}

/*****************************************************************************/
//  Description : MMIMP3_AddAllAudioFilesToList
//  Global resource dependence :
//  Author:robert.wang
//  Note:
/*****************************************************************************/
PUBLIC APLAYER_LIST_ERROR_E MMIMP3_SaveAllAudioFilesToList(BOOLEAN is_music_list,
        uint16 list_index)
{
    APLAYER_LIST_ERROR_E    result = APLAYER_LIST_ERROR;
    uint32     added_num = 0;
    uint32     total_num = 0;

    if (PNULL != s_file_array)
    {
        total_num = MMIAPIFILEARRAY_GetArraySize(s_file_array);
        if (0 == total_num)
        {
            result = APLAYER_LIST_SUCCESS;
        }
        else if(AddFilesToList(is_music_list,list_index,s_file_array,&added_num))
        {
            if (total_num == added_num)
            {
                result = APLAYER_LIST_SUCCESS;
            }
            else
            {
                result = APLAYER_LIST_NO_SAPCE;
            }

        }

        MMIAPIFILEARRAY_Destroy(&s_file_array);
        s_file_array = PNULL;
    }
    SCI_TRACE_LOW("[APLIST] MMIMP3_SaveAllAudioFilesToList result = %d, list_index = %d, total_num=%d, added_num = %d ",result,list_index,total_num,added_num);

    return result;

}

/*****************************************************************************/
//  Description : MMIMP3_SaveFilesToMyMusicList
//  Global resource dependence :
//  Author:xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIMP3_SaveFilesToMyMusicList(void)
{
    uint32 total_num = 0;
    uint32 added_num = 0;

    if (PNULL != s_file_array)
    {
        total_num = MMIAPIFILEARRAY_GetArraySize(s_file_array);

        SCI_TRACE_LOW("[APLIST] MMIMP3_SaveFilesToMyMusicList total_num = %d", total_num);

//        if (0 < total_num)
		if (total_num >= 0) //Bug 132825
        {
            MMIMP3_OpenList(TRUE,0);
            AddFilesToList(TRUE,0,s_file_array,&added_num);
            MMIMP3_CloseList(TRUE,0,TRUE);
        }
        MMIAPIFILEARRAY_Destroy(&s_file_array);
        s_file_array = PNULL;
    }
}
/*****************************************************************************/
//  Description : DestroyRandomList
//  Global resource dependence :
//  Author:robert.wang
//  Note:
/*****************************************************************************/
LOCAL void DestroyRandomList(MMI_ALISTHANDLE_T play_list_ptr)
{
    uint32 total_num = 0;
    uint32 i = 0;
    uint16 *index_ptr = PNULL;

    if (PNULL != play_list_ptr)
    {
        total_num = MMI_ALIST_GetSize(play_list_ptr);
        for (i = 0; i < total_num; i++)
        {
            index_ptr = MMI_ALIST_RemoveItem(play_list_ptr,0,PNULL);
            if (PNULL != index_ptr)
            {
                SCI_FREE(index_ptr);
            }
        }
        MMI_ALIST_Destroy(&play_list_ptr, PNULL);
        
        SCI_TRACE_LOW("[APLIST] DestroyRandomList play_list_ptr != PNULL");
    }
}

/*****************************************************************************/
//  Description : CreateRandomList
//  Global resource dependence :
//  Author:robert.wang
//  Note:
/*****************************************************************************/
LOCAL MMI_ALISTHANDLE_T CreateRandomList(uint16 start_index,
        uint32 total_num,
        uint16 *active_file_index_ptr,
        uint16 cur_index,
        MMIMP3_PLAY_MODE_E play_mode)
{
    MMI_ALISTHANDLE_T play_list_ptr = PNULL;
    uint32 i = 0;
    uint16 *cur_index_ptr = PNULL;

    SCI_TRACE_LOW("[APLIST] CreateRandomList play_mode = %d ",play_mode);
    if (PNULL != active_file_index_ptr)
    {
        play_list_ptr = MMI_ALIST_Create();

        if (PNULL != play_list_ptr)
        {
			SCI_TRACE_LOW("[APLIST] CreateRandomList play_list_ptr != PNULL");
            switch(play_mode)
            {
            case MMIMP3_PLAY_ONE_REPEAT: //单首重复播放
                cur_index_ptr = SCI_ALLOC_APP(sizeof(uint16));
                if (PNULL != cur_index_ptr)
                {
                    SCI_MEMSET(cur_index_ptr,0,sizeof(uint16));
                    *cur_index_ptr = cur_index;
                    if(0 != MMI_ALIST_Append(play_list_ptr,cur_index_ptr))
                    {
                        SCI_FREE(cur_index_ptr);
                        break;
                    }
                }
                *active_file_index_ptr = 0;
                break;

            case MMIMP3_PLAY_LIST_ONCE://列表循环一次
                for (i = start_index; i < total_num; i++)
                {
                    cur_index_ptr = SCI_ALLOC_APP(sizeof(uint16));
                    if (PNULL != cur_index_ptr)
                    {
                        SCI_MEMSET(cur_index_ptr,0,sizeof(uint16));
                        *cur_index_ptr = (uint16)i;
                        if(0 != MMI_ALIST_Append(play_list_ptr,cur_index_ptr))
                        {
                            SCI_FREE(cur_index_ptr);
                            *active_file_index_ptr = 0;
                            return play_list_ptr;
                        }
                    }
                }
                *active_file_index_ptr = cur_index;
                break;
            case MMIMP3_PLAY_LIST_REPEAT:   //列表重复循环
                for (i = cur_index; i < total_num; i++)
                {
                    cur_index_ptr = SCI_ALLOC_APP(sizeof(uint16));
                    if (PNULL != cur_index_ptr)
                    {
                        SCI_MEMSET(cur_index_ptr,0,sizeof(uint16));
                        *cur_index_ptr = (uint16)i;
                        if(0 != MMI_ALIST_Append(play_list_ptr,cur_index_ptr))
                        {
                            SCI_FREE(cur_index_ptr);
                            break;
                        }
                    }
                }
                for (i = start_index; i < cur_index; i++)
                {
                    cur_index_ptr = SCI_ALLOC_APP(sizeof(uint16));
                    if (PNULL != cur_index_ptr)
                    {
                        SCI_MEMSET(cur_index_ptr,0,sizeof(uint16));
                        *cur_index_ptr = (uint16)i;
                        if(0 != MMI_ALIST_Append(play_list_ptr,cur_index_ptr))
                        {
                            SCI_FREE(cur_index_ptr);
                            break;
                        }
                    }
                }
                *active_file_index_ptr = 0;
                break;

            case MMIMP3_PLAY_RANDOM_REPEAT: //列表重复随机
            case MMIMP3_PLAY_RANDOM_ONCE:
                if(!GetDifferentRandom(play_list_ptr,start_index,total_num,cur_index))
                {
                    MMI_ALIST_Destroy(&play_list_ptr, PNULL);
                    play_list_ptr = PNULL;
                }
                *active_file_index_ptr = 0;
                break;

            default:
                MMI_ALIST_Destroy(&play_list_ptr, PNULL);
                play_list_ptr = PNULL;
                SCI_TRACE_LOW("[APLIST] CreateRandomList play_mode = default");
                break;
            }
        }
    }
    return play_list_ptr;
}


/*****************************************************************************/
//  Description : 获取指定范围内的不重复的随机整数.
//  Global resource dependence :
//  Author:apple.zhang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetDifferentRandom(MMI_ALISTHANDLE_T play_list_ptr,
                                 uint16 start_index,
                                 uint32 total_num,
                                 uint16 cur_file_index
                                )
{
    BOOLEAN *random_arr_ptr = PNULL;
    uint32      i , j   ,count     = 0;
    uint16      random          = 0;
    BOOLEAN    result = FALSE;
    uint16     *cur_index_ptr = PNULL;
    uint16      cur_index = MIN(cur_file_index, (total_num -1));


    SCI_TRACE_LOW("[APLIST] GetDifferentRandom start_index = %d, total_num = %d, cur_index = %d, ",start_index,total_num,total_num,cur_index);

    if (PNULL != play_list_ptr
            && cur_index >= start_index
            && 0 < total_num)
    {
        cur_index_ptr = SCI_ALLOCA(sizeof(uint16));
        if (PNULL != cur_index_ptr)
        {
            SCI_MEMSET(cur_index_ptr,0,sizeof(uint16));
            *cur_index_ptr = cur_index;
            if(0 != MMI_ALIST_Append(play_list_ptr,cur_index_ptr))
            {
                SCI_FREE(cur_index_ptr);
               return result;
            }
        }
        else
        {
            return result;
        }

        //把最终需要的结果（不重复的数）预先放在一个数组中random_arr_ptr，因为rand函数产生的随机数会重复，所以不直接使用，而是对应数据的下标
        random_arr_ptr = (BOOLEAN*)SCI_ALLOC_APP((sizeof(BOOLEAN)*total_num));
    if(PNULL != random_arr_ptr)
    {
        SCI_MEMSET(random_arr_ptr,0,(sizeof(BOOLEAN)*total_num));
        for (i = 0; i < total_num; i++)
        {
            random_arr_ptr[i] = FALSE;
        }
        random_arr_ptr[cur_index - start_index] = TRUE;

        srand(SCI_GetTickCount());
        for (i=0; i<total_num; i++)
        {
            count =0;
            random = (uint16)((uint16)(rand())%(total_num-i));//生成0到total-1-i之间随机数

            //找到boolean数组中没有没选中的第random+1个数的下标
            for(j = 0; j<total_num; j++)
            {
                if(FALSE == random_arr_ptr[j])
                {
                    count++;
                }
                if(count == random+1)
                    break;
            }
            if (j < total_num)
            {
                //设置标志数组下标j已经被占用
                random_arr_ptr[j] = TRUE;

                j = j + start_index;

                cur_index_ptr = SCI_ALLOCA(sizeof(uint16));
                if (PNULL != cur_index_ptr)
                {
                    SCI_MEMSET(cur_index_ptr,0,sizeof(uint16));
                    *cur_index_ptr = (uint16)j;
                    //将该数的数组下标作为第i个随机数
                    if(0 != MMI_ALIST_Append(play_list_ptr,cur_index_ptr))
                    {
                        SCI_FREE(cur_index_ptr);
                        break;
                    }
                }


            }

        }

        //free memory
        SCI_FREE(random_arr_ptr);
        random_arr_ptr = PNULL;
    }
        result = TRUE;
    }
    return result;
}

/*****************************************************************************/
//  Description : current play music file info is init or not
//  Global resource dependence :
//  Author:aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMP3_IsLoadCurPlayInfo(void)
{
    if(0 < s_cur_audioplayer_list_info.file_full_path_len
            && s_cur_audioplayer_list_info.cur_list_index != INVALID_INDEX)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*****************************************************************************/
//  Description : list file is exist
//  Global resource dependence :
//  Author:aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMP3_IsListFileExist(BOOLEAN is_music_list,
                                      uint16 list_index)
{
    wchar full_path[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
    uint16 full_path_len = MMIFILE_FULL_PATH_MAX_LEN;

    if (is_music_list)
    {
        list_index++;
    }
    if(MMIAPlayer_GetListFileFullPath(list_index,full_path,&full_path_len)
            && MMIAPIFMM_IsFileExist(full_path,full_path_len))
    {
        return TRUE;
    }

    return FALSE;

}

/*****************************************************************************/
//  Description : get list type
//  Global resource dependence :
//  Author:
//  Note:BOOLEAN is_music_list :是否是音乐列表，音乐列表从my music开始
/*****************************************************************************/
PUBLIC APLAYER_LIST_TYPE_E MMIMP3_GetListType(
    BOOLEAN is_music_list,
    uint16 list_index
)
{
    if (is_music_list)
    {
        list_index += APLAYER_LIST_TYPE_MYMUSIC;
    }

    return MMIAPlayer_GetListType(list_index);
}

/*****************************************************************************/
//  Description : delete list file
//  Global resource dependence :
//  Author:
//  Note:BOOLEAN is_music_list :是否是音乐列表，音乐列表从my music开始
/*****************************************************************************/
PUBLIC void MMIMP3_DeleteListFile(
    BOOLEAN is_music_list,
    uint16 list_index
)
{
    if (is_music_list)
    {
        list_index += APLAYER_LIST_TYPE_MYMUSIC;
    }
    MMIAPlayer_DeleteListFile(list_index);
}

