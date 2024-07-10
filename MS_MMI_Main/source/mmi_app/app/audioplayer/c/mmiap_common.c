/*****************************************************************************
** File Name:      mmiap_common.c                                            *
** Author:                                                                   *
** Date:           23/08/2012                                                *
** Copyright:      2012 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to audio file srv inferface for down    *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 08/2012       robert.wang       Create                                    *
******************************************************************************/

#define _MMIAP_COMMON_C_
/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_app_audioplayer_trc.h"
#include "mmipub.h"
#include "mmi_text.h"


//other module
#ifdef DRM_SUPPORT
#include "mmidrm_export.h"
#endif

//local module
#include "mmimp3_export.h"
#include "mmiap_control.h"
#include "mmiap_setting.h"

#ifdef MMIAP_SPECTRUM_SUPPORT
#include "mmiap_spectrum.h"
#endif

#ifdef MMIAP_LYRIC_SUPPORT
#include "mmiap_lyric_internal.h"
#endif

#include "mmiap_list.h"
#include "mmisrvfmm_export.h"
#include "mmiap_list_control.h"
#include "mmiap_appmain.h"
#include "mmiap_common.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
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

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         LOCAL DEFINITION                                 *
 **--------------------------------------------------------------------------*/
LOCAL const uint8   s_mp3_all_suffix[][MMIMP3_DEFAULT_SUFFIX_LEN] =
{
#ifdef MP3_BACKGROUND_SUPPORT
    {MMIMP3_SUFFIX_MP3},            //Mp3文件后缀名
    //@CR241798 2011.05.30
    {MMIMP3_SUFFIX_MP2},            //Mp2文件后缀名
    {MMIMP3_SUFFIX_MP1},            //Mp1文件后缀名
    //@CR241798 2011.05.30
#endif
#ifdef WMA_BACKGROUND_SUPPORT
    {MMIMP3_SUFFIX_WMA},            //WMA文件后缀名
#endif
#ifdef AAC_BACKGROUND_SUPPORT
    {MMIMP3_SUFFIX_AAC},            //AAC文件后缀名
    {MMIMP3_SUFFIX_M4A},            //M4A文件后缀名
#endif
#ifdef AMR_BACKGROUND_SUPPORT
    {MMIMP3_SUFFIX_AMR},            //AMR文件后缀名
#endif
#if defined MIDI_BACKGROUND_SUPPORT && !defined MMI_SOUNDBANK_MODE_COMPACT
    {MMIMP3_SUFFIX_MID},            //MID文件后缀名
    {MMIMP3_SUFFIX_MIDI},           //MIDI文件后缀名
#endif
#ifdef WAV_BACKGROUND_SUPPORT
    {MMIMP3_SUFFIX_WAV},            //WAV文件后缀名
#endif
#ifdef DRM_SUPPORT
    {MMIMP3_SUFFIX_DRM},            //DRM文件后缀名
    {MMIMP3_SUFFIX_DCF},            //DCF文件后缀名
#endif
    {PNULL}
};
const wchar g_mmimp3_reserved_dir[]           = { 'M', 'p', '3', '_', 'r', 'e', 's' ,0 };

const wchar g_mmimp3_play_list_suffix_wstr[]  = { '.', 'l', 's', 't', 0 };
const wchar g_mmimp3_mark_file_name_wstr[]    = { 'm', 'p', '3', '.', 'd', 'a', 't' ,0 };//MMIMP3_MUSIC_MARK_FILE_NAME

/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                               *
**--------------------------------------------------------------------------*/


/*****************************************************************************/
//  Description : Get the audio folder.
//  Global resource dependence :
//  Author:xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMP3_GetDefaultAudioFolder(
    wchar *folder_name_ptr,
    uint16 *folder_name_len_ptr
)
{
    BOOLEAN          result = FALSE;
    MMIFILE_DEVICE_E dev    = MMI_DEVICE_UDISK;

    if(PNULL == folder_name_ptr || PNULL == folder_name_len_ptr)
    {
        return FALSE;
    }
    dev = MMIAPIFMM_GetFirstValidDevice();
    if(dev < MMI_DEVICE_NUM)
    {

        result = MMIAPIFMM_CombineFullPath(
                     MMIAPIFMM_GetDevicePath(dev),
                     MMIAPIFMM_GetDevicePathLen(dev),
                     MMIMULTIM_DIR_MUSIC,
                     MMIAPICOM_Wstrlen(MMIMULTIM_DIR_MUSIC),
                     PNULL,
                     NULL,
                     folder_name_ptr,
                     folder_name_len_ptr
                 );

    }

    return result;
}

/*****************************************************************************/
//  Description : Initialize the mp3 file loading filter.
//  Global resource dependence :
//  Author:xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIAP_InitFileFilter(
    void
)
{
    uint32 i = 0;
    uint32 suffix_len = SCI_STRLEN(MMIMP3_SUFFIX_MP3);
    uint32 filter_len = 0;
    MMIFMM_FILTER_T *filter_ptr = MMIAP_GetMusicFilterPtr();


    
    if (PNULL == filter_ptr)
    {
        return;
    }
    
    SCI_MEMSET(filter_ptr->filter_str, 0, sizeof(filter_ptr->filter_str));

    /*lint -save -e685 -e568 -e681 */

    for(i = 0; i < MMIMP3_TYPE_MAX; i++)
    {
        filter_len = SCI_STRLEN((char*)filter_ptr->filter_str);
        filter_ptr->filter_str[filter_len] = ' ';
        filter_ptr->filter_str[filter_len + 1] = '*';
        suffix_len = SCI_STRLEN((char*)s_mp3_all_suffix[i]);
        MMI_MEMCPY(&filter_ptr->filter_str[filter_len + 2], sizeof(filter_ptr->filter_str) - filter_len - 2,
                   s_mp3_all_suffix[i], suffix_len, suffix_len);
    }

    /*lint -restore */
    SCI_TRACE_LOW("[MMIMP3]: InitMp3FileFilter, filter = %s", filter_ptr->filter_str);
}


/*****************************************************************************/
//  Description : Get the music filter.
//  Global resource dependence :
//  Author:xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIMP3_GetMusicFilter(
    MMIFMM_FILTER_T * filter_ptr
)
{
    MMIFMM_FILTER_T *music_filter_ptr = MMIAP_GetMusicFilterPtr();

    
    if (PNULL == filter_ptr
        || PNULL == music_filter_ptr
       )
    {
        return;
    }


    //return &s_music_filter;
    MMI_MEMCPY(filter_ptr, sizeof(MMIFMM_FILTER_T), music_filter_ptr,
               sizeof(MMIFMM_FILTER_T), sizeof(MMIFMM_FILTER_T));
    //SCI_TRACE_LOW:"[MMIMP3]: MMIMP3_GetMusicFilter, filter = %s, orig_filter = %s"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMP3_11010_112_2_18_2_4_12_207, (uint8*)"ss",\
                   filter_ptr->filter_str, music_filter_ptr->filter_str);
}

/*****************************************************************************/
//  Description : Whether the file can be deleted.
//  Global resource dependence :
//  Author:xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMP3_IsFileUsed(
    uint16              *file_path_ptr,
    uint16              file_path_len
)
{
    BOOLEAN result = FALSE;

    if(MMIAPIMP3_IsBgPlayMp3(file_path_ptr, file_path_len)    ||
#ifdef MMIAP_LYRIC_SUPPORT
            MMIAPIMP3_IsBgPlayLyric(file_path_ptr, file_path_len)  ||
#endif
            MMIAPIMP3_IsInternalDataFile(file_path_ptr, file_path_len)
      )
    {
        result = TRUE;
    }

    return result;
}

/*****************************************************************************/
//  Description : Whether the file is in the folder of MMIMP3_RESERVED_DIR.
//  Global resource dependence :
//  Author:xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMP3_IsInternalDataFile(
    uint16              *file_path_ptr,
    uint16              file_path_len
)
{
    BOOLEAN     result = FALSE;
    uint16      dir_path[FMM_SEARCH_FILENAME_MAXLEN + 1] = {0};
    uint16      dir_len = FMM_SEARCH_FILENAME_MAXLEN;
    uint16      reserve_dir_path[MMIMULTIM_DIR_SYSTEM_LEN + MMIMP3_RESERVED_DIR_LEN + 2] = {0};
    uint16      reserve_dir_len = MMIMULTIM_DIR_SYSTEM_LEN + MMIMP3_RESERVED_DIR_LEN + 1;

    MMI_WSTRNCPY(reserve_dir_path, reserve_dir_len, MMIMULTIM_DIR_SYSTEM, MMIMULTIM_DIR_SYSTEM_LEN, MMIMULTIM_DIR_SYSTEM_LEN);
    reserve_dir_path[MMIMULTIM_DIR_SYSTEM_LEN] = MMIFILE_SLASH;
    MMI_WSTRNCPY(reserve_dir_path + MMIMULTIM_DIR_SYSTEM_LEN + 1, reserve_dir_len - MMIMULTIM_DIR_SYSTEM_LEN, MMIMP3_RESERVED_DIR, MMIMP3_RESERVED_DIR_LEN, MMIMP3_RESERVED_DIR_LEN);

    if(MMIAPIFMM_SplitFullPath(
                file_path_ptr, file_path_len,
                PNULL, NULL,
                dir_path, &dir_len,
                PNULL, NULL
            ))
    {
        if(0 == MMIAPICOM_CompareTwoWstrExt(
                    dir_path, dir_len,
                    reserve_dir_path, reserve_dir_len,
                    FALSE
                ))
        {
            result = TRUE;
        }
    }

    return result;
}

/*****************************************************************************/
//  Description : To judge whether the file is the type which is loaded by mp3 player.
//  Global resource dependence :
//  Author: xingdong.li
//  Note: 依据后缀判断
/*****************************************************************************/
PUBLIC BOOLEAN MMIAP_IsPlayerLoadType(
    MMISRVAUD_RING_FMT_E audio_type
)
{
    BOOLEAN result = FALSE;

    switch(audio_type)
    {
    case MMISRVAUD_RING_FMT_MP3:
#ifdef MP3_BACKGROUND_SUPPORT
        result = TRUE;
#endif
        break;

    case MMISRVAUD_RING_FMT_WMA:
#ifdef WMA_BACKGROUND_SUPPORT
        result = TRUE;
#endif
        break;

    case MMISRVAUD_RING_FMT_AAC:
    case MMISRVAUD_RING_FMT_M4A:
#ifdef AAC_BACKGROUND_SUPPORT
        result = TRUE;
#endif
        break;

    case MMISRVAUD_RING_FMT_AMR:
#ifdef AMR_BACKGROUND_SUPPORT
        result = TRUE;
#endif
        break;

    case MMISRVAUD_RING_FMT_MIDI:
#if defined MIDI_BACKGROUND_SUPPORT && !defined MMI_SOUNDBANK_MODE_COMPACT
        result = TRUE;
#endif
        break;

    case MMISRVAUD_RING_FMT_WAVE:
#ifdef WAV_BACKGROUND_SUPPORT
        result = TRUE;
#endif
        break;

    default:
        break;
    }

    return result;
}

/*****************************************************************************/
//  Description : To judge whether the file is the type which is loaded by mp3 player.
//  Global resource dependence :
//  Author: xingdong.li
//  Note: 依据后缀判断
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMP3_IsMp3PlayerLoadType(
    const wchar *full_path_ptr,     //in
    uint16      full_path_len       //in，以双字节计算
)
{
    BOOLEAN            result = FALSE;
    MMISRVAUD_RING_FMT_E    audio_type = MMISRVAUD_RING_FMT_MAX;

    if(NULL == full_path_ptr ||
            0 == full_path_len    ||
            MMIFILE_FILE_NAME_MAX_LEN < full_path_len)
    {
        //SCI_TRACE_LOW:"[MMIMP3]:MMIAPIMP3_IsMp3PlayerLoadType, full_path_ptr = %x,full_path_len = %d!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMP3_3853_112_2_18_2_3_55_63, (uint8*)"dd", full_path_ptr, full_path_len);
        return result;
    }

#ifdef DRM_SUPPORT

    if(!MMIAPIMP3_IsDrmFile(
                full_path_ptr,
                full_path_len,
                &audio_type, PNULL, PNULL))
#endif
    {
        audio_type  = MMIAPICOM_GetMusicType(full_path_ptr, full_path_len);
    }

    result = MMIAP_IsPlayerLoadType(audio_type);

    return result;
}

#ifdef DRM_SUPPORT
/*****************************************************************************/
//  Description : consume drm right
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL void ConsumeDrmRight(void)
{
    MMIMP3_LIST_FILE_BODY_T cur_play_info = {0};

    cur_play_info.full_path_len = FMM_SEARCH_FILENAME_MAXLEN;
    if(MMIMP3_GetCurPlayFileFullPath(cur_play_info.full_path_name, &(cur_play_info.full_path_len)))
    {
        MMIAPIDRM_ConsumeRights(SFS_INVALID_HANDLE, cur_play_info.full_path_name);
        //SCI_TRACE_LOW:"[MMIMP3]: ConsumeDrmRight index = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMP3_1646_112_2_18_2_3_51_16, (uint8*)"d",\
                     MMIMP3_GetCurPlayFileIndex());
    }
}

/*****************************************************************************/
//  Description : consume drm right
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIMP3_ConsumeDrmRight(void)
{
    ConsumeDrmRight();
}

/*****************************************************************************/
//  Description : Is Drm File Suffix
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsDrmFileSuffix(
    const wchar    *file_name_ptr,
    uint16          name_len
)
{
    BOOLEAN             result = FALSE;
    MMI_STRING_T        suffix_str = {0};
    uint8               suffix_asci[MMIMP3_DEFAULT_SUFFIX_LEN + 1] = {0};

    if(PNULL == file_name_ptr || 0 == name_len)
    {
        return FALSE;
    }

    suffix_str.wstr_ptr = SCI_ALLOC_APPZ( ( MMIMP3_DEFAULT_SUFFIX_LEN + 1 ) * sizeof(wchar) );

    if (PNULL != suffix_str.wstr_ptr)
    {
        MMIAPICOM_GetNameAndSuffix(file_name_ptr, name_len, PNULL, &suffix_str);
        MMIAPICOM_Wstrlower(suffix_str.wstr_ptr);
        MMIAPICOM_WstrToStr(suffix_str.wstr_ptr, suffix_asci);

        if(0 == MMIAPICOM_Stricmp(suffix_asci, MMIMP3_SUFFIX_DCF_NO_DOT) /*lint !e64*/
                || 0 == MMIAPICOM_Stricmp(suffix_asci, MMIMP3_SUFFIX_DRM_NO_DOT)) /*lint !e64*/
        {
            result = TRUE;
        }

        SCI_FREE(suffix_str.wstr_ptr);
    }

    return result;
}

/*****************************************************************************/
//  Description : Is Drm File Type
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMP3_IsDrmFile(
    const wchar    *file_name_ptr,      //[IN]
    uint16          name_len,           //[IN]
    MMISRVAUD_RING_FMT_E *audio_type_ptr,    //[OUT]
    BOOLEAN         *is_audio_drm_ptr,  //[OUT]
    BOOLEAN         *is_drm_valid_ptr   //[OUT]
)
{
    BOOLEAN                 result = FALSE;
    BOOLEAN                 is_audio_drm_file = FALSE;
    BOOLEAN                 is_drm_valid = FALSE;
    MMISRVAUD_RING_FMT_E    drm_audio_type = MMISRVAUD_RING_FMT_MAX;
    DRM_RIGHTS_T            drm_right =  {0};
    SFS_HANDLE              drm_handle = PNULL;
    SFS_ERROR_E             error = SFS_NO_ERROR;

    if(IsDrmFileSuffix(file_name_ptr, name_len))
    {
        drm_handle = SFS_CreateFile(file_name_ptr,
                                    SFS_MODE_READ | SFS_MODE_OPEN_EXISTING, 0, 0);

        is_drm_valid = (MMIAPIDRM_IsDRMFile(drm_handle, PNULL)
                        && MMIAPIDRM_IsRightsValid(drm_handle, PNULL, TRUE, DRM_PERMISSION_PLAY));

        error = DRM_GetRightsInfo(drm_handle, &drm_right);

        drm_audio_type = MMIAPIDRM_ConvertAudioMimeType(drm_right.mime_type);

        SFS_CloseFile(drm_handle);


        if(MMISRVAUD_RING_FMT_MAX != drm_audio_type)
        {
            is_audio_drm_file = TRUE;
        }

        if(PNULL != audio_type_ptr)
        {
            *audio_type_ptr = drm_audio_type;
        }

        if(PNULL != is_audio_drm_ptr)
        {
            *is_audio_drm_ptr = TRUE;
        }

        if(PNULL != is_drm_valid_ptr)
        {
            *is_drm_valid_ptr = is_drm_valid;
        }

        result = TRUE;
    }

    //SCI_TRACE_LOW:"[MMIMP3 DRM] MMIAPIMP3_IsDrmFile :drm_audio_type = %d,is_audio_drm_file = %d, is_drm_valid = %d, result = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMP3_1907_112_2_18_2_3_51_18, (uint8*)"dddd", drm_audio_type, is_audio_drm_file, is_drm_valid, result);

    return result;
}
#endif  //#ifdef DRM_SUPPORT

/*****************************************************************************/
//  Description : get system file full path for read
//  Author:robert.wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAP_GetSysFileFullPathForRead(
    MMIFILE_DEVICE_E default_dev,          //[IN]
    const wchar    *path_name_ptr,         //[IN]
    uint16         path_name_len,          //[IN]
    const wchar    *file_name_ptr,         //[IN]
    uint16         file_name_len,          //[IN]
    wchar          *full_path_name_ptr,    //[OUT]
    uint16         *full_path_len_ptr      //[OUT]
)
{
    BOOLEAN result_val = FALSE;
    MMIFILE_DEVICE_E device_type = MMI_DEVICE_UDISK;
    //MMIFILE_DEVICE_E first_valid_dev = MMI_DEVICE_NUM;

    BOOLEAN is_ok = FALSE;



    if (PNULL == path_name_ptr
        || 0 == path_name_len
        || PNULL == file_name_ptr
        || 0 == file_name_len
        || PNULL == full_path_name_ptr
        || PNULL == full_path_len_ptr
       )
    {
        return is_ok;
    }






    do
    {
        device_type = default_dev;

        //check default dev
        if((device_type < MMI_DEVICE_NUM)
                && MMIAPIFMM_GetDeviceTypeStatus(device_type)
          )
        {
            result_val = MMIAPIFMM_CombineSysPath(device_type,
                                                  path_name_ptr,
                                                  path_name_len,
                                                  file_name_ptr,
                                                  file_name_len,
                                                  full_path_name_ptr,
                                                  full_path_len_ptr
                                                 );


            if(result_val && MMIAPIFMM_IsFileExist(full_path_name_ptr, *full_path_len_ptr))
            {
                is_ok = TRUE;
                break;
            }

        }

#if 0

        //check from E to C
        for(device_type = MMI_DEVICE_NUM - 1; device_type >= MMI_DEVICE_SYSTEM; device_type--)
        {
            if(default_dev == device_type)
            {
                continue;
            }

            if(MMIAPIFMM_GetDeviceTypeStatus(device_type))
            {
                if(MMI_DEVICE_NUM <= first_valid_dev)
                {
                    first_valid_dev = device_type;
                }


                result_val = MMIAPIFMM_CombineSysPath(device_type,
                                                      path_name_ptr,
                                                      path_name_len,
                                                      file_name_ptr,
                                                      file_name_len,
                                                      full_path_name_ptr,
                                                      full_path_len_ptr
                                                     );

                if(result_val && MMIAPIFMM_IsFileExist(full_path_name_ptr, *full_path_len_ptr))
                {
                    is_ok = TRUE;
                    break;
                }
            }
        }

        if(!is_ok)
        {
            //must return a valid full path
            if(MMI_DEVICE_NUM <= first_valid_dev)
            {
                first_valid_dev = MMI_DEVICE_SDCARD;
            }

            result_val = MMIAPIFMM_CombineSysPath(first_valid_dev,
                                                  path_name_ptr,
                                                  path_name_len,
                                                  file_name_ptr,
                                                  file_name_len,
                                                  full_path_name_ptr,
                                                  full_path_len_ptr
                                                 );

            if(result_val)
            {
                is_ok = TRUE;
            }
        }

#endif
    }
    while(0);


    SCI_TRACE_LOW("[MMIMP3] GetSysFileFullPathForRead is_ok=%d",
                  is_ok);

    return is_ok;
}

