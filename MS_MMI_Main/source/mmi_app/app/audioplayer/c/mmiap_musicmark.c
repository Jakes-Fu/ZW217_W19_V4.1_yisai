/*****************************************************************************
** File Name:               mmiap_musicmark.c                                *
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
#ifdef MMIAP_MUSICMARK_SUPPORT
#define _MMIAP_MUSICMARK_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#ifdef WIN32
#include "std_header.h"
#endif

#include "mmiap_play.h"
#include "mmimp3_export.h"
#include "mmiap_musicmark.h"
#include "mmiap_list_control.h"
#include "mmiap_musicarray.h"
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         LOCAL DEFINITION                                 *
 **--------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/
LOCAL void * s_music_mark_array = PNULL;

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : Generate a new mark info by the current playing file info.
//  Global resource dependence :
//  Author:xingdong.li
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GenNewMarkInfo(
    MMIMP3_MUSIC_MARK_DETAIL_INFO_T *mark_info_ptr
);

/*****************************************************************************/
//  Description : Combine the music mark info file name.
//  Global resource dependence :
//  Author:xingdong.li
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CombineMarkFileNameForRead(
    wchar           *full_path_name_ptr,    //[OUT]
    uint16          *full_path_len_ptr      //[OUT]
);


/*****************************************************************************/
//  Description : Combine the music mark info file name.
//  Global resource dependence :
//  Author:xingdong.li
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CombineMarkFileNameForWrite(
    wchar           *full_path_name_ptr,    //[OUT]
    uint16          *full_path_len_ptr      //[OUT]
);

/*****************************************************************************/
//  Description :save and free
//  Global resource dependence :
//  Author:xingdong.li
//  Note:
/*****************************************************************************/
LOCAL void SaveMusicMark(void);

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description :save and free
//  Global resource dependence :
//  Author:xingdong.li
//  Note:
/*****************************************************************************/
LOCAL void SaveMusicMark(void)
{
    wchar full_path[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
    uint16 full_path_len = MMIFILE_FULL_PATH_MAX_LEN;
    MMIFILE_HANDLE sfs_handle = PNULL;

    if(PNULL != s_music_mark_array
            && CombineMarkFileNameForWrite(full_path,&full_path_len))
    {
        if (!MMIAPIFMM_IsFileExist(full_path,full_path_len))
        {
            sfs_handle = MMIAPIFMM_CreateFile(full_path, SFS_MODE_WRITE|SFS_MODE_CREATE_ALWAYS, NULL, NULL);/*lint !e655*/
            if (PNULL != sfs_handle)
            {
                MMIAPIFMM_CloseFile(sfs_handle);
            }
        }
        MMIAPLAYERARRAY_SaveArrayToFile(s_music_mark_array,sizeof(MMIMP3_MUSIC_MARK_DETAIL_INFO_T),full_path,full_path_len);
    }
}

/*****************************************************************************/
//  Description :save and free
//  Global resource dependence :
//  Author:xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIMP3_SaveMusicMark(void)
{
    if (PNULL != s_music_mark_array)
    {
        SaveMusicMark();
        MMIAPLAYERARRAY_DestroyArray(&s_music_mark_array);
        s_music_mark_array = PNULL;
    }

}

/*****************************************************************************/
//  Description : Get the total number of music mark.
//  Global resource dependence :
//  Author:xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIMP3_GetMusicMarkNum(void)
{
    uint32 total_num = 0;

    if (PNULL != s_music_mark_array)
    {
        total_num = MMIAPLAYERARRAY_GetTotalNum(s_music_mark_array);
    }
    return total_num;
}

/*****************************************************************************/
//  Description : Get music mark detail info.
//  Global resource dependence :
//  Author:xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMP3_GetMusicMarkInfo(
    uint32 index,                                       //[IN]
    MMIMP3_MUSIC_MARK_DETAIL_INFO_T *mark_info_ptr      //[OUT]
)
{
    BOOLEAN result = FALSE;

    if (PNULL != s_music_mark_array)
    {
        result = MMIAPLAYERARRAY_Read(s_music_mark_array,index,mark_info_ptr);
    }

    return result;
}


/*****************************************************************************/
//  Description : Generate a new mark info by the current playing file info.
//  Global resource dependence :
//  Author:xingdong.li
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GenNewMarkInfo(
    MMIMP3_MUSIC_MARK_DETAIL_INFO_T *mark_info_ptr
)
{
    BOOLEAN result = TRUE;
    MMISRVAUD_PLAY_INFO_T   mp3_play_info = {0};
    MMISRVAUD_CONTENT_INFO_T  mp3_file_info = {0};
    MMIMP3_LIST_FILE_BODY_T cur_play_file_info = {0};
    wchar   file_name[FMM_SEARCH_FILENAME_MAXLEN + 1] = {0};
    uint16  file_name_len = MMIFMM_FULL_FILENAME_LEN;
    uint8   mark_time[MMIMP3_DISPLAY_TIME_LEN] = {0};
    wchar  mark_time_ucs[MMIMP3_DISPLAY_TIME_LEN] = {0};
    uint16  mark_time_ucs_len = 0;

    SCI_MEMSET(mark_info_ptr, 0, sizeof(MMIMP3_MUSIC_MARK_DETAIL_INFO_T));

    //所在列表

    mark_info_ptr->list_name_len = FMM_SEARCH_FILENAME_MAXLEN;
    MMIMP3_GetCurPlayListFullPath(mark_info_ptr->list_name, &(mark_info_ptr->list_name_len));


    //所在文件
    cur_play_file_info.full_path_len = FMM_SEARCH_FILENAME_MAXLEN;
    if(!MMIMP3_GetCurPlayFileFullPath(cur_play_file_info.full_path_name, &(cur_play_file_info.full_path_len)))
    {
        return FALSE;
    }

    MMI_WSTRNCPY(mark_info_ptr->music_file_name, FMM_SEARCH_FILENAME_MAXLEN,
                 cur_play_file_info.full_path_name, cur_play_file_info.full_path_len, cur_play_file_info.full_path_len);
    mark_info_ptr->music_file_name_len = cur_play_file_info.full_path_len;

    //书签时间
    switch(MMIMP3_GetAudioState())
    {
    case MMIMP3_AUDIO_PLAYING:
    case MMIMP3_AUDIO_PAUSED:
        MMISRVAUD_GetPlayingInfo(MMIAPIMP3_GetBGPlayerHandle(), &mp3_play_info);

        mark_info_ptr->mark_offset_in_time  = mp3_play_info.cur_time;
        mark_info_ptr->music_total_time     = mp3_play_info.total_time;
        mark_info_ptr->mark_offset_in_byte  = mp3_play_info.cur_data_offset;
        break;

    case MMIMP3_AUDIO_STOPPED:
        if(MMISRVAUD_GetFileContentInfo(
                    cur_play_file_info.full_path_name,
                    cur_play_file_info.full_path_len,
                    &mp3_file_info
                ))
        {
            mark_info_ptr->mark_offset_in_time  = 0;
            mark_info_ptr->music_total_time     = mp3_file_info.total_time;
            mark_info_ptr->mark_offset_in_byte  = 0;
        }

        break;

    default:
        break;
    }


    sprintf((char*)mark_time, " %02ld:%02ld", (uint32)(mark_info_ptr->mark_offset_in_time / 60), (mark_info_ptr->mark_offset_in_time % 60));
    mark_time_ucs_len = GUI_GBToWstr(mark_time_ucs, mark_time, (uint16)SCI_STRLEN((char*)mark_time));

    MMIAPIFMM_SplitFullPathExt(
        cur_play_file_info.full_path_name,
        cur_play_file_info.full_path_len,
        PNULL, PNULL,
        file_name, &file_name_len,
        PNULL, PNULL);
    file_name_len = MIN(file_name_len, (MMIMP3_MARK_NAME_MAX_LEN) - mark_time_ucs_len);
    MMI_WSTRNCPY(mark_info_ptr->mark_name, MMIMP3_MARK_NAME_MAX_LEN + 1,
                 file_name, file_name_len, file_name_len);
    MMI_WSTRNCPY(mark_info_ptr->mark_name + file_name_len, mark_time_ucs_len,
                 mark_time_ucs, mark_time_ucs_len, mark_time_ucs_len);
    mark_info_ptr->mark_name_len = file_name_len + mark_time_ucs_len;

    //mark_info_ptr->mark_create_time =
    return result;
}

/*****************************************************************************/
//  Description : load all music mark brief info.
//  Global resource dependence : s_music_mark_info_ptr
//  Author:xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMP3_LoadAllMusicMarkInfo(void)
{
    BOOLEAN result = FALSE;
    wchar  full_path[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
    uint16 full_path_len = MMIFILE_FULL_PATH_MAX_LEN;

    if (PNULL != s_music_mark_array)
    {
        MMIAPLAYERARRAY_DestroyArray(&s_music_mark_array);
    }

    s_music_mark_array = MMIAPLAYERARRAY_CreateArray(sizeof(MMIMP3_MUSIC_MARK_DETAIL_INFO_T),_D_FILE_NAME,_D_FILE_LINE);

    if (PNULL != s_music_mark_array)
    {
        if(CombineMarkFileNameForRead(full_path,&full_path_len))
        {
            if (MMIAPIFMM_IsFileExist(full_path,full_path_len))
            {
                MMIAPLAYERARRAY_LoadFromFile(s_music_mark_array,full_path,full_path_len);
            }
        }
        result = TRUE;
    }

    return result;
}

/*****************************************************************************/
//  Description : Combine the music mark info file name.
//  Global resource dependence :
//  Author:xingdong.li
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CombineMarkFileNameForRead(
    wchar           *full_path_name_ptr,    //[OUT]
    uint16          *full_path_len_ptr      //[OUT]
)
{

    *full_path_len_ptr = FMM_SEARCH_FILENAME_MAXLEN + 1;

    return MMIAPIFMM_GetSysFileFullPathForRead(FALSE,
            MMIMP3_RESERVED_DIR,
            MMIMP3_RESERVED_DIR_LEN,
            MMIMP3_MUSIC_MARK_FILE_NAME,
            MMIMP3_MUSIC_MARK_FILE_NAME_LEN,
            full_path_name_ptr,
            full_path_len_ptr);
}

/*****************************************************************************/
//  Description : Combine the music mark info file name.
//  Global resource dependence :
//  Author:xingdong.li
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CombineMarkFileNameForWrite(
    wchar           *full_path_name_ptr,    //[OUT]
    uint16          *full_path_len_ptr      //[OUT]
)
{

    *full_path_len_ptr = FMM_SEARCH_FILENAME_MAXLEN + 1;

    return MMIAPIFMM_GetSysFileFullPathForWrite(FALSE,
            MMIMP3_RESERVED_DIR,
            MMIMP3_RESERVED_DIR_LEN,
            MMIMP3_MUSIC_MARK_FILE_NAME,
            MMIMP3_MUSIC_MARK_FILE_NAME_LEN,
            sizeof(MMIMP3_MUSIC_MARK_DETAIL_INFO_T),
            full_path_name_ptr,
            full_path_len_ptr);
}

/*****************************************************************************/
//  Description : Add a new music mark.
//  Global resource dependence :
//  Author:xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC MMIFILE_ERROR_E MMIMP3_AddMusicMark(uint16 *cur_list_selection_ptr)
{
    MMIFILE_ERROR_E result = SFS_ERROR_SYSTEM;
    MMIMP3_MUSIC_MARK_DETAIL_INFO_T  mark_info = {0};

    if(MMIMP3_MUSIC_MARK_MAX_NUM <= MMIMP3_GetMusicMarkNum())
    {
        result = SFS_ERROR_TOOMANYFILE;
        return result;
    }

    if (PNULL != s_music_mark_array)
    {
        GenNewMarkInfo(&mark_info);
        if(MMIAPLAYERARRAY_Add(s_music_mark_array,&mark_info))
        {
            SaveMusicMark();
            if (PNULL != cur_list_selection_ptr)
            {
                *cur_list_selection_ptr = MMIMP3_GetMusicMarkNum()-1;
            }
            result = SFS_NO_ERROR;
        }
    }

    return result;
}

/*****************************************************************************/
//  Description : Delete music mark.
//  Global resource dependence :
//  Author:xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMP3_DelMusicMark(
    uint16      *mark_index_ptr,        //[IN]
    uint16      index_num,           //[IN]
    uint16      *new_list_selection_ptr
)
{
    BOOLEAN result = FALSE;
    int32 i = 0;


    if (PNULL != mark_index_ptr
            && PNULL != s_music_mark_array
            && index_num > 0
       )
    {
        for( i = index_num - 1; i >= 0; i--)
        {
            if (!MMIAPLAYERARRAY_Delete(s_music_mark_array,mark_index_ptr[i]))
            {
                break;
            }
            else if (PNULL != new_list_selection_ptr
                     && *new_list_selection_ptr > mark_index_ptr[i])
            {
                (*new_list_selection_ptr)--;
            }
        }

        SaveMusicMark();
        if (i == -1)
        {
            result = TRUE;
        }
    }

    return result;
}


/*****************************************************************************/
//  Description : Update music mark info.
//  Global resource dependence :
//  Author:xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMP3_UpdateMusicMarkName(
    uint32              mark_index,         //[IN]mark index
    MMI_STRING_T        mark_name_str
)
{
    BOOLEAN result = TRUE;
    MMIMP3_MUSIC_MARK_DETAIL_INFO_T mark_info = {0};

    if (PNULL != mark_name_str.wstr_ptr)
    {
        MMIMP3_GetMusicMarkInfo(mark_index, &mark_info);
        SCI_MEMSET(mark_info.mark_name, 0, sizeof(mark_info.mark_name));
        MMI_WSTRNCPY(mark_info.mark_name, FMM_SEARCH_FILENAME_MAXLEN,
                     mark_name_str.wstr_ptr, mark_name_str.wstr_len, mark_name_str.wstr_len);
        mark_info.mark_name_len = mark_name_str.wstr_len ;

        result = MMIAPLAYERARRAY_Modify(s_music_mark_array,mark_index,&mark_info);
        SaveMusicMark();
    }
    return result;

}

/*****************************************************************************/
//  Description : resize music mark buffer size
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIAP_ResizeMusicMarkBufferSize(uint32 buffer_size)
{
    SCI_TRACE_LOW("[MMIAP] MMIAP_ResizeMusicMarkBufferSize array=0x%X, buffer_size=%d",
                  s_music_mark_array, buffer_size);

    if (PNULL != s_music_mark_array)
    {
        MMIAPlayer_LIST_SetBufferSize(s_music_mark_array, buffer_size);
    }
}

#endif //MMIAP_MUSICMARK_SUPPORT
