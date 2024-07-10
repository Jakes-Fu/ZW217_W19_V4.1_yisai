/*****************************************************************************
** File Name:      mmifmm_comfunc.c                                          *
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
#define _MMIFMM_COMFUNC_C_

#include "mmi_app_fmm_trc.h"
#include "os_api.h"
#include "mmi_osbridge.h"
#include "mmk_type.h"
#include "mmipub.h"
#ifdef DRM_SUPPORT
#include "mmidrm_export.h"
#endif
#include "mmi_image.h"
#include "mmifmm_comfunc.h"

#define SBC_CHAR_START                      (0xFF01)            //全角符号:"!"
#define SBC_CHAR_END                        (0xFF5E)            //全角符号:"~"
#define SBC_TO_DBC_STEP                     (0xFEE0)            //全角半角转换间隔
#define SBC_SPACE                           (0x3000)            //全角符号:空格
#define DBC_SPACE                           (0x20)              //半角空格

/*****************************************************************************/
//  Description : combine  filename with path
//  Global resource dependence :
//  Author: robert.lu
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIFMM_CombineToFileName (
    const wchar      *pathname_ptr, //in
    uint16      pahname_len,           //in
    const wchar      *filename_ptr,     //in
    uint16      filename_len,       //in
    wchar      *full_path_filename_ptr,//out
    uint16      full_path_filename_len_ptr//out
)
{
    uint32      full_file_name_len = 0;
    //SCI_TRACE_LOW:"MMIFMM: CombineFileName."
    SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_10569_112_2_18_2_20_57_516, (uint8*) "");

    //SCI_ASSERT(PNULL != pathname_ptr);
    //SCI_ASSERT(PNULL != filename_ptr);
    //SCI_ASSERT(PNULL != full_path_filename_ptr);
    if ( (PNULL == pathname_ptr) || (PNULL == filename_ptr) || (PNULL == full_path_filename_ptr))
    {
        //SCI_TRACE_LOW:"MMIFMM:[CombineToFileName]:param error!"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_10575_112_2_18_2_20_57_517, (uint8*) "");
        return full_file_name_len;
    }

    if (0 == pahname_len || 0 == filename_len)
    {
        return full_file_name_len;
    }
    else
    {
        //SCI_ASSERT((pahname_len+filename_len+1)<= full_path_filename_len_ptr);
        if ( (pahname_len + filename_len + 1) > full_path_filename_len_ptr)
        {
            //SCI_TRACE_LOW:"MMIFMM:[CombineToFileName]:total_len = %d"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_10588_112_2_18_2_20_57_518, (uint8*) "d", (pahname_len + filename_len + 1));
            return full_file_name_len;
        }

        MMIAPICOM_Wstrncpy (full_path_filename_ptr, pathname_ptr, pahname_len);
        full_path_filename_ptr[pahname_len] = MMIFILE_SLASH;
        MMIAPICOM_Wstrncpy (&full_path_filename_ptr[pahname_len+1], filename_ptr, filename_len);
        full_file_name_len = pahname_len + filename_len + 1;
    }

    return full_file_name_len;
}


/*****************************************************************************/
//  Description : combine full file name
//  Global resource dependence :
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIFMM_CombineFullFileName (
    const MMIFMM_DATA_INFO_T      *list_data_ptr, //in
    const MMIFMM_PATH_INFO_T      *current_path_ptr,      //in
    const uint32                   index,                 //in
    wchar                   *file_name_ptr,         //out
    const uint32                   file_name_max_len      //in
)
{
    uint32 full_file_name_len = 0;
    SCI_MEMSET (file_name_ptr , 0x00, ( (file_name_max_len) *sizeof (wchar)));

    if (list_data_ptr->path_is_valid)
    {
        MMIAPICOM_Wstrncpy (file_name_ptr,
                            list_data_ptr->data[index]->filename_ptr,
                            list_data_ptr->data[index]->filename_len
                           );
        full_file_name_len = list_data_ptr->data[index]->filename_len;
    }
    else
    {
        if (PNULL != current_path_ptr)
        {
            full_file_name_len = MMIFMM_CombineToFileName (current_path_ptr->pathname, current_path_ptr->pathname_len,
                                 list_data_ptr->data[index]->filename_ptr, list_data_ptr->data[index]->filename_len,
                                 file_name_ptr, (uint16) file_name_max_len);
        }
        else
        {
            full_file_name_len = MMIFMM_CombineToFileName (list_data_ptr->pathname, list_data_ptr->pathname_len,
                                 list_data_ptr->data[index]->filename_ptr, list_data_ptr->data[index]->filename_len,
                                 file_name_ptr, (uint16) file_name_max_len);
        }
    }

    return full_file_name_len;
}

/*****************************************************************************/
//  Description : printf error code
//  Global resource dependence :
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC void MMIFMM_RaiseError (uint32 error_index)
{
    SCI_TRACE_LOW ("MMIFMM_RaiseError, Something happens,be careful ,error code = %d", error_index);
    return;
}


/*****************************************************************************/
//  Description : crrect folder name, such as remove "."," ",from tail
//  Global resource dependence : none
//  Parameter: folder_name_ptr [IN/OUT]: orignal folder, after change,
//                                       it will return corrected folder name
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void  MMIFMM_CorrectFolderName (wchar *folder_name_ptr)
{
    uint16 name_len = 0;
    int16 i = 0;

    if (folder_name_ptr != PNULL)
    {
        name_len = (uint16) MMIAPICOM_Wstrlen (folder_name_ptr);

        if (name_len > 0)
        {
            for (i = (int16) (name_len - 1); i >= 0; i--)
            {
                if (folder_name_ptr[i] != '.' && (folder_name_ptr[i] != ' '))
                {
                    break;
                }
            }

            if (i < name_len - 1)
            {
                folder_name_ptr[i + 1] = 0;
            }
        }
    }
}

/*****************************************************************************/
// Description : 新建目录名字是否有效
// Global resource dependence :
// Author:
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIFMM_NewFolderNameStringIsValid (
    const MMI_STRING_T     *file_name_ptr, //in:
    uint32                     ucs2_max_len    //in
)
{
    BOOLEAN     result = FALSE;

    //SCI_ASSERT(NULL != file_name_ptr);
    if (PNULL == file_name_ptr)
    {
        //SCI_TRACE_LOW:"MMIFMM:[NewFolderNameStringIsValid] PNULL == file_name_ptr"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_6322_112_2_18_2_20_47_440, (uint8*) "");
        return result;
    }

    //判断名字长度
    if ( (0 < file_name_ptr->wstr_len) && (ucs2_max_len >= file_name_ptr->wstr_len))
    {
        if (!MMIAPICOM_IsIncludeInvalidChar (file_name_ptr->wstr_ptr,  file_name_ptr->wstr_len))
        {
            result = TRUE;
        }
    }

    return (result);
}
/*****************************************************************************/
//  Description : get finally slash index
//  Global resource dependence :
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMIFMM_GetFinallySlashIndex (wchar *file_name_ptr, int32 file_len)
{
    int32 i = 0;

    //SCI_ASSERT(PNULL!=file_name_ptr);
    if (PNULL == file_name_ptr)
    {
        //SCI_TRACE_LOW:"MMIFMM:[MMIFMM_GetFinallySlashIndex]PNULL == file_name_ptr"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_11735_112_2_18_2_21_0_530, (uint8*) "");
        return (uint16) i;
    }

    //SCI_ASSERT(0<file_len);

    for (i = file_len; file_name_ptr[i] != MMIFILE_SLASH && i > 0; i--)
    {
        NULL;  /*lint !e522 */
    }

    //SCI_TRACE_LOW:"MMIFMM_GetFinallySlashIndex i =%d"
    SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_11745_112_2_18_2_21_0_531, (uint8*) "d", i);
    return (uint16) i;
}

/*****************************************************************************/
//  Description : is file need display
//  Global resource dependence :
//  Author:jian.ma
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIFMM_IsFileNeedDisplay (uint32 suffix_type, wchar *name_ptr, uint16 name_len)
{
    wchar  suffix_name[MMIFMM_FILENAME_LEN + 1] = {0};
    uint16 suffix_len = MMIFMM_FILENAME_LEN;
    uint32 actual_file_type = 0;

    if (MMIFMM_FILE_ALL == suffix_type)
    {
        return TRUE;
    }

    if (MMIFMM_UNKNOW_TYPE == suffix_type)
    {
        return FALSE;
    }

    if (!MMIAPIFMM_SplitFileName (name_ptr, name_len, PNULL, PNULL, suffix_name, &suffix_len))
    {
        return FALSE;
    }

    actual_file_type = MMIAPIFMM_GetFileType (suffix_name, suffix_len);

    if ( (MMIFMM_DRM_ALL& (suffix_type)) && (MMIFMM_DRM_ALL& (actual_file_type)))
    {
        //drm file
        if (! (MMIFMM_MEDIA_TYPE (suffix_type) &MMIFMM_MEDIA_TYPE (actual_file_type)))
        {
            return FALSE;
        }
    }
    else if ( (MMIFMM_MEDIA_TYPE (suffix_type) &MMIFMM_MEDIA_TYPE (actual_file_type)) == 0//小类不相同
              || (MMIFMM_FILE_TYPE (suffix_type) != MMIFMM_FILE_TYPE (actual_file_type)) //不是drm
            )
    {
        //not drm file, not same file type or not same media type
        return FALSE;
    }

#ifdef DRM_SUPPORT

    if (MMIFMM_DRM_ALL == suffix_type || MMIFMM_DCF_FILE == suffix_type || MMIFMM_DM_FILE == suffix_type)
    {
        return TRUE;
    }

    if ( (actual_file_type & MMIFMM_DRM_ALL) != 0)
    {
        MMIFILE_HANDLE drm_handle = 0;
        DRM_RIGHTS_T rights_info = {0};
        uint32 drm_suffix_type = 0;

        drm_handle = SFS_CreateFile (name_ptr, SFS_MODE_READ | SFS_MODE_OPEN_EXISTING, 0, 0);

        if (!MMIAPIDRM_IsDRMFile (drm_handle, PNULL))
        {
            //非drm文件，则继续原有处理
            MMIAPIFMM_CloseFile (drm_handle);
            return FALSE;
        }

        /*是DRM文件，则求出此文件的mine type*/
        MMIAPIDRM_GetRightsinfo (drm_handle, PNULL, &rights_info);
        drm_suffix_type = MMIFMM_ConvertMimeTypeToSuffix (rights_info.mime_type);
        MMIAPIFMM_CloseFile (drm_handle);

        if (0 == (drm_suffix_type & suffix_type))
        {
            return FALSE;
        }
    }

#endif
    return TRUE;
}
/*****************************************************************************/
//  Description : get path depth
//  Global resource dependence :
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIFMM_GetFullPathDepth (wchar *path_ptr, uint32 path_len)
{
    uint32 i = 0;
    uint32 path_depth = 0;

    //SCI_ASSERT(PNULL!=path_ptr);
    if (PNULL == path_ptr)
    {
        //SCI_TRACE_LOW:"MMIFMM:[MMIFMM_GetFullPathDepth]PNULL == path_ptr"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_11676_112_2_18_2_21_0_526, (uint8*) "");
        return path_depth;
    }

    for (i = 0; i < path_len; i++)
    {
        if (MMIFILE_SLASH == path_ptr[i])
        {
            path_depth++;
        }
    }

    //SCI_TRACE_LOW:"MMIFMM_GetFullPathDepth path_depth =%d"
    SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_11687_112_2_18_2_21_0_527, (uint8*) "d", path_depth);
    return path_depth;
}

/*****************************************************************************
 	Description : Convert the character from upper to lower
	Global resource dependence : none
       Author:
	Note:
*****************************************************************************/
PUBLIC BOOLEAN MMIFMM_ConvertToLower (uint16 *ch)
{
    if ('A' <= *ch && *ch <= 'Z')
    {
        //upper case change to low case
        *ch = *ch + ('a' - 'A');
        return TRUE;
    }

    return FALSE;
}

/*****************************************************************************
 	Description : Convert the characer from SBC to DBC.
	Global resource dependence : none
       Author:
	Note:
*****************************************************************************/
PUBLIC BOOLEAN MMIFMM_ConvertSBC2DBC (
    uint16 *ch    //IN/OUT:全角符号转换成半角
)
{
    BOOLEAN result = FALSE;

    if ( (*ch >= SBC_CHAR_START) && (*ch <= SBC_CHAR_END))
    {
        *ch -= SBC_TO_DBC_STEP;

        result = TRUE;
    }
    else if (*ch == SBC_SPACE)
    {
        *ch = DBC_SPACE;

        result = TRUE;
    }

    return result;
}



/*****************************************************************************/
// Description : 搜索串是否有效
// Global resource dependence :
// Author: Ming.Song
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIFMM_FindStringIsValid (
    MMI_STRING_T   *file_name_ptr  //in:
)
{
    BOOLEAN     result = FALSE;

    if (PNULL == file_name_ptr)
    {
        SCI_TRACE_LOW ("MMIFMM_FindStringIsValid PNULL == full_path_wstr_ptr");
        return FALSE;
    }

    if (' ' == * (file_name_ptr->wstr_ptr))
    {
        return result;
    }

    if ( (0x00 == * (file_name_ptr->wstr_ptr)) && (' ' == * (file_name_ptr->wstr_ptr + 1)))
    {
        return result;
    }

    //判断名字长度
    if (0 < file_name_ptr->wstr_len)
    {
        {
            //uint8 invalid_char[] = {'\\', '/', ':', '"', '<', '>', '|',0x0D};
            //MS00191807
            const uint8 invalid_char[] = {'\\', '/', ':', '"', '<', '>',  0x0D};
            uint16 i = 0;
            uint16 j = 0;
            uint8 invalid_char_num  = sizeof (invalid_char) ;
            wchar *name_ptr = NULL;

            name_ptr = file_name_ptr->wstr_ptr;

            for (i = 0; i < file_name_ptr->wstr_len; i++)
            {
                for (j = 0; j < invalid_char_num; j++)
                {
                    if (* (name_ptr + i) == invalid_char[j])
                    {
                        return FALSE;
                    }
                }
            }

            return TRUE;
        }
    }

    return (result);
}
/*****************************************************************************/
// Description : show file error type
// Global resource dependence :
// Author:
// RETRUN:
// Note:
/*****************************************************************************/
PUBLIC void MMIAPIFMM_PromptFileErrorType (
    MMI_WIN_ID_T alert_win_id,
    MMIFILE_ERROR_E fs_error,
    MMIFILE_DEVICE_E filedev
)
{
    SFS_ERROR_E sfs_error = fs_error;

    if (SFS_ERROR_NONE !=  sfs_error)
    {
        MMIFMM_PromptFileErrorType (alert_win_id, sfs_error, filedev, TRUE);
    }
}

/*****************************************************************************/
// Description : show file error type
// Global resource dependence :
// Author:
// RETRUN:
// Note:
/*****************************************************************************/
PUBLIC void MMIFMM_PromptFileErrorType (MMI_WIN_ID_T alert_win_id, SFS_ERROR_E fs_error, MMIFILE_DEVICE_E filedev, BOOLEAN is_file)
{
    MMI_TEXT_ID_T storage_text_id = TXT_NULL;
    MMI_TEXT_ID_T text_id = TXT_NULL;
    MMI_IMAGE_ID_T image_id = IMAGE_PUBWIN_SUCCESS;
    MMI_WIN_ID_T   *win_id_ptr = PNULL;
    //MMIFILE_DEVICE_E device_type = MMI_DEVICE_UDISK;

    if (SFS_ERROR_ABORT == fs_error)
    {
        //user stop, need not to show its message
        return;
    }

    switch (fs_error)
    {
    case SFS_ERROR_NONE:
        //success
        //text_id = TXT_COMPLETE;
        //image_id = IMAGE_PUBWIN_SUCCESS;
        break;
    case SFS_ERROR_INVALID_PARAM:
        //invalid name
        text_id = TXT_DC_NAMEINVALID;
        image_id = IMAGE_PUBWIN_WARNING;
        break;
    case SFS_ERROR_HAS_EXIST:

        //already exist
        if (is_file)
        {
            text_id = TXT_FILE_EXISTED;
        }
        else
        {
            text_id = TXT_FOLDER_EXISTED;
        }

        image_id = IMAGE_PUBWIN_WARNING;
        break;
    case SFS_ERROR_NOT_EXIST:

        //not exist
        if (is_file)
        {
            text_id = TXT_COM_FILE_NO_EXIST;
        }
        else
        {
            text_id = TXT_FOLDER_NOT_EXIST;
        }

        image_id = IMAGE_PUBWIN_WARNING;
        break;
    case SFS_ERROR_NO_SPACE:

        //no space
        //device_type = MMIFILE_GetDeviceTypeByFileDev(filedev);
        if (MMIAPIFMM_GetDeviceStatus (MMIAPIFMM_GetDevicePath (filedev), MMIAPIFMM_GetDevicePathLen (filedev)))
        {
            storage_text_id = MMIAPIFMM_GetDeviceName (filedev);
            text_id = TXT_NO_SPACE;
        }
        else
        {
            storage_text_id =  TXT_NULL;
            text_id = TXT_NO_SD_CARD_ALERT;
        }

        image_id = IMAGE_PUBWIN_WARNING;
        break;
    case SFS_ERROR_NOT_EMPTY:
        //folder not empty
        text_id = TXT_COM_FOLDER_NOT_EMPTY;
        image_id = IMAGE_PUBWIN_WARNING;
        break;
    case SFS_ERROR_FOLD_FULL:

        //fold entry is not enough to  add new file or fold in it
        //device_type = MMIFILE_GetDeviceTypeByFileDev(filedev);
        if (MMIAPIFMM_GetDeviceStatus (MMIAPIFMM_GetDevicePath (filedev), MMIAPIFMM_GetDevicePathLen (filedev)))
        {
            storage_text_id = MMIAPIFMM_GetDeviceName (filedev);
        }

//         if(FS_UDISK == filedev)
//         {
//             storage_text_id = TXT_FMM_PHONE;
//         }
//         else
//         {
//             storage_text_id = TXT_COMMON_SD_CARD;
//         }
        text_id = TXT_NO_SPACE;
        image_id = IMAGE_PUBWIN_WARNING;
        break;
    case SFS_ERROR_ACCESS:
        //file in using
        text_id = TXT_COM_FILE_USING;
        image_id = IMAGE_PUBWIN_WARNING;
        break;
    case SFS_ERROR_LONG_DEPTH:
        //file in using
        text_id = TXT_COM_FILE_NAME_TOO_LONG;
        image_id = IMAGE_PUBWIN_WARNING;
        break;
    default:
        //other error
        text_id = TXT_ERROR;
        image_id = IMAGE_PUBWIN_FAIL;
        break;
    }

    //MMIPUB_CloseAlertWin();
    if (PNULL != alert_win_id)
    {
        win_id_ptr = &alert_win_id;
    }

    if (TXT_NULL == storage_text_id)
    {
        if (TXT_NULL != text_id)
        {
            MMIPUB_OpenAlertWinByTextId (PNULL, text_id, TXT_NULL, image_id, win_id_ptr, PNULL, MMIPUB_SOFTKEY_ONE, PNULL);
        }
    }
    else
    {
        if (TXT_NULL != text_id)
        {
            MMIPUB_OpenAlertWinByTextId (PNULL, storage_text_id, text_id, image_id, win_id_ptr, PNULL, MMIPUB_SOFTKEY_ONE, PNULL);
        }
    }
}


/*****************************************************************************/
//  Description : The file is dir
//  Global resource dependence :
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIFMM_FileIsFolder (const uint16 file_attr)
{
    BOOLEAN result = FALSE;

    if (file_attr & SFS_ATTR_DIR)
    {
        result = TRUE;
    }

    //SCI_TRACE_LOW:"FmmFileIsFolder IsFolder =%d"
    SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_11761_112_2_18_2_21_0_532, (uint8*) "d", result);
    return result;
}

/*****************************************************************************/
//  Description : convert suffix type
//  Global resource dependence :
//  Author: haiwu.chen`
/*****************************************************************************/
PUBLIC uint32 MMIFMM_ConvertMimeTypeToSuffix (
    MIME_TYPE_E mime_type
)
{
    uint32 suffix_type = 0;

    switch (mime_type)
    {
    case MIME_TYPE_TEXT_PLAIN:
        suffix_type = MMIFMM_TXT_ALL;
        break;

    case MIME_TYPE_IMAGE_ANY:                // image start
        suffix_type = MMIFMM_PIC_ALL;
        break;

    case MIME_TYPE_IMAGE_GIF:
        suffix_type = MMIFMM_PICTURE_GIF;
        break;

    case MIME_TYPE_IMAGE_JPEG:
        suffix_type = MMIFMM_PICTURE_JPG;
        break;

//    case MIME_TYPE_IMAGE_TIFF:
//        suffix_type = MMIFMM_PICTURE_GIF;
//        break;

    case MIME_TYPE_IMAGE_PNG:
        suffix_type = MMIFMM_PICTURE_PNG;
        break;

    case MIME_TYPE_IMAGE_VND_WAP_WBMP:
        suffix_type = MMIFMM_PICTURE_WBMP;
        break;

//    case MIME_TYPE_IMAGE_SVG:
//        suffix_type = MMIFMM_PIC_ALL;
//        break;

    case MIME_TYPE_IMAGE_BMP:
        suffix_type = MMIFMM_PICTURE_BMP;
        break;

//    case MIME_TYPE_IMAGE_X_UP_WPNG:
//        suffix_type = MMIFMM_PIC_ALL;
//       break;

    case MIME_TYPE_TEXT_ANY:         // "text/*"
        suffix_type = MMIFMM_TXT_ALL;
        break;

//    case MIME_TYPE_TEXT_HTML:        // "text/html"
//        suffix_type = MMIFMM_PIC_ALL;
//        break;

//    case MIME_TYPE_TEXT_PLAIN:       // "text/plain"
//        suffix_type = MMIFMM_PIC_ALL;
//         break;

//    case MIME_TYPE_TEXT_X_VCALENDAR: // "text/x-vcalendar
//        suffix_type = MMIFMM_PIC_ALL;
//        break;

//    case MIME_TYPE_TEXT_X_VCARD:     // "text/x-vcard"
//        suffix_type = MMIFMM_PIC_ALL;
//        break;

//    case MIME_TYPE_TEXT_XML:         // "text/xml"
//        suffix_type = MMIFMM_PIC_ALL;
//        break;

#ifdef JAVA_SUPPORT
//    case MIME_TYPE_APPLICATION_JAVA_VM:
//        suffix_type = MMIFMM_PIC_ALL;
//        break;

    case MIME_TYPE_APPLICATION_JAVA_JAR:
        suffix_type = MMIFMM_JAVA_JAR;
        break;

    case MIME_TYPE_APPLICATION_JAVA_JAD:
        suffix_type = MMIFMM_JAVA_JAD;
        break;
#endif

    case MIME_TYPE_AUDIO_ANY:                // audio start
        suffix_type = MMIFMM_MUSIC_ALL;
        break;

    case MIME_TYPE_AUDIO_MID:
        suffix_type = MMIFMM_MUSIC_MID;
        break;

    case MIME_TYPE_AUDIO_MPEG:
        suffix_type = MMIFMM_MUSIC_MP3;
        break;

    case MIME_TYPE_AUDIO_X_WAV:
        suffix_type = MMIFMM_MUSIC_WAV;
        break;

    case MIME_TYPE_AUDIO_AMR:
        suffix_type = MMIFMM_MUSIC_AMR;
        break;

    case MIME_TYPE_AUDIO_X_AAC:
        suffix_type = MMIFMM_MUSIC_ACC;
        break;

    case MIME_TYPE_AUDIO_WMA:
        suffix_type = MMIFMM_MUSIC_WMA;
        break;

    case MIME_TYPE_AUDIO_MIDI:
    case MIME_TYPE_AUDIO_X_MIDI:
    case MIME_TYPE_AUDIO_SP_MIDI:
        suffix_type = MMIFMM_MUSIC_MIDI;
        break;

    case MIME_TYPE_VIDEO_MP4:
    case MIME_TYPE_VIDEO_MPEG:
        suffix_type = MMIFMM_MOVIE_MP4;
        break;

    case MIME_TYPE_VIDEO_3GPP:
        suffix_type = MMIFMM_MOVIE_3GP;
        break;
#ifdef MMI_KING_MOVIE_SUPPORT
    case MIME_TYPE_VIDEO_KMV:
        suffix_type = MMIFMM_MOVIE_KMV;
        break;
    case MIME_TYPE_VIDEO_SMV:    //smv_support
        suffix_type = MMIFMM_MOVIE_SMV;
        break;
#ifdef MMI_KING_MOVIE_SUPPORT_HMV
    case MIME_TYPE_VIDEO_HMV:    //hmv_support
        suffix_type = MMIFMM_MOVIE_HMV;
        break;
#endif
#endif
    case MIME_TYPE_VIDEO_AVI:
        suffix_type = MMIFMM_MOVIE_AVI;
        break;

    default:
        break;

    }

    return suffix_type;
}



/*****************************************************************************/
//  Description : check copy file path before copying
//  Global resource dependence :
//  Author: xinrui.wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIFMM_IsPath1ContainPath2 (
    uint16   *pathname_1,
    uint16   pathname_1_len,
    uint16   *pathname_2,
    uint16   pathname_2_len
)
{
    BOOLEAN         is_contain = FALSE;
    uint16          i = 0;

    if (NULL != pathname_1 && NULL != pathname_2
            && 0 != pathname_1_len && 0 != pathname_2_len
       )
    {
        if (pathname_1_len >= pathname_2_len)
        {
            for (i = 0; i < pathname_2_len; i++)
            {
                if (pathname_1[i] != pathname_2[i])
                {
                    break;
                }
            }

            if (i >= pathname_2_len)
            {
                if (pathname_1_len == pathname_2_len || (MMIFILE_SLASH == pathname_1[i]))
                {
                    //same folder or path2 is path1's sub folder
                    is_contain = TRUE;
                }
            }
        }
    }

    return is_contain;
}


/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:liqing.peng
//  Modify: xinrui.wang
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMIFMM_GetPathDepth (
    const wchar *path_ptr,
    uint16   path_len
)
{
    int32   i = 0;
    uint16  depth = 1;

    for (i = 0; i < path_len; i++)
    {
        if (MMIFILE_SLASH == path_ptr[i])
        {
            depth++;
        }
    }

    return depth;
}


/*****************************************************************************/
//  Description : whether file is in default picture folder
//  Global resource dependence :
//  Author: robert.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIFMM_IsDefaultPicFolder (
    const wchar      *file_name,
    uint16      name_len
)
{
    BOOLEAN     result = FALSE;
    uint16     offset = 0;
    wchar       pic_name [MMIMULTIM_DIR_PIC_LEN+MMIFMM_PATH_NAME_OFFSET+1] = {0};
    uint16      pic_len = 0;

    if (file_name == PNULL || name_len == 0)
    {
        return FALSE;
    }

    pic_len = MMIAPICOM_Wstrlen (file_name);
	offset = MMIFMM_PATH_NAME_OFFSET;
	if(pic_len-offset == MMIAPICOM_Wstrlen(MMIMULTIM_DIR_PICTURE))
	{
		MMIAPICOM_Wstrncpy(pic_name,file_name,pic_len);
	}
	else
	{
		return FALSE;
	}

    if (offset > name_len)
    {
        return FALSE;
    }

    if ( (MMIAPICOM_Wstrlen (MMIMULTIM_DIR_PICTURE) == (pic_len - offset))
#ifdef CMCC_UI_STYLE
            || ( (MMIAPICOM_Wstrlen (MMIMULTIM_DIR_PHOTOS) == (pic_len - offset)))
#endif
       )
    {
        // 判断是否是Picture文件夹
        if (0 == MMIAPICOM_CompareTwoWstrExt (&pic_name[offset], MMIAPICOM_Wstrlen (MMIMULTIM_DIR_PICTURE),
                                              MMIMULTIM_DIR_PICTURE, MMIAPICOM_Wstrlen (MMIMULTIM_DIR_PICTURE), FALSE))
        {
            result = TRUE;
        }

        // 判断是否是Picture文件夹
#ifdef CMCC_UI_STYLE
        else if (0 == MMIAPICOM_CompareTwoWstrExt (&pic_name[offset], MMIAPICOM_Wstrlen (MMIMULTIM_DIR_PHOTOS),
                 MMIMULTIM_DIR_PHOTOS, MMIAPICOM_Wstrlen (MMIMULTIM_DIR_PHOTOS), FALSE))
        {
            result = TRUE;
        }

#endif
    }
    else
    {
        result = FALSE;
    }

    return result;
}


/*****************************************************************************/
//  Description : get file info form fullpath
//  Global resource dependence :
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFMM_GetFileInfoFormFullPath (
    const wchar *full_file_name,       //in
    const uint32 filename_len,          //in
    MMIFILE_FILE_INFO_T *file_info//out
)
{
    //SCI_ASSERT(PNULL!=file_info);
    if (PNULL == file_info)
    {
        //SCI_TRACE_LOW:"MMIFMM:[MMIAPIFMM_GetFileInfoFormFullPath]PNULL ==file_info"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_10540_112_2_18_2_20_57_515, (uint8*) "");
        return FALSE;
    }

    MMIAPIFMM_SplitFullPath (full_file_name, (uint16) filename_len,
                             file_info->device_name, &file_info->device_name_len,
                             file_info->dir_name, &file_info->dir_name_len,
                             file_info->file_name, &file_info->file_name_len);

	//coverity 12580
 //   file_info->device_name_len = file_info->device_name_len;
 //   file_info->dir_name_len = file_info->dir_name_len;
  //  file_info->file_name_len = file_info->file_name_len;

    return MMIAPIFMM_GetFileInfo (full_file_name, (uint16) filename_len, &file_info->file_size, PNULL, PNULL);
}


/*****************************************************************************/
//  Description : get file name from data info
//  Global resource dependence :
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIFMM_FmmGetFileName (MMIFMM_DATA_INFO_T      *list_data_ptr,        //in
                                     uint32                   index,                 //in
                                     wchar                   *file_name_ptr,         //out
                                     uint32                   file_name_max_len      //in
                                    )
{
    uint16 i = 0;
    uint32 file_name_len = 0;
    SCI_MEMSET (file_name_ptr, 0, ( (file_name_max_len) *sizeof (wchar)));

    if (list_data_ptr->path_is_valid)
    {
        i = MMIFMM_GetFinallySlashIndex (list_data_ptr->data[index]->filename_ptr, (list_data_ptr->data[index]->filename_len - 1));
        //for (i  = list_data_ptr->data[index]->filename_len -1; list_data_ptr->data[index]->filename[i] != MMIFILE_SLASH && i >0; i--);
        file_name_len = list_data_ptr->data[index]->filename_len - i - 1;
        MMIAPICOM_Wstrncpy (file_name_ptr,  &list_data_ptr->data[index]->filename_ptr[i+1], file_name_len);
    }
    else
    {
        file_name_len = list_data_ptr->data[index]->filename_len;
        MMIAPICOM_Wstrncpy (file_name_ptr,  list_data_ptr->data[index]->filename_ptr, file_name_len);
    }

    return file_name_len;
}

/*****************************************************************************/
//  Description : get dev info and check if have enough space
//  Global resource dependence :
//  Author:jian.ma
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFMM_IsDevEnoughSpace (uint32 size, MMIFILE_DEVICE_E dev)
{
    uint32                  free_space_low = 0;
    uint32                  free_space_high = 0;
    BOOLEAN                 result = TRUE;

    if (dev < MMI_DEVICE_NUM)
    {
        MMIAPIFMM_GetDeviceFreeSpace (MMIAPIFMM_GetDevicePath (dev), MMIAPIFMM_GetDevicePathLen (dev),
                                      &free_space_high, &free_space_low);
    }
    else
    {
        return FALSE;
    }

    if (0 == free_space_high && size > free_space_low)
    {
        //SCI_TRACE_LOW:"MMIAPIFMM_IsDevEnoughSpace: free spaced high value is = %d free space low value is = %d"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_2139_112_2_18_2_20_10_228, (uint8*) "dd", free_space_high, free_space_low);
        result = FALSE;
    }

    return result;
}

/*****************************************************************************/
//  Description : get pic data for pic detail info
//  Global resource dependence :
//  Author:liqing.peng
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFMM_GetPictureWidthHeight (
    const wchar       *full_path_ptr,
    const uint16      full_path_len,
    uint32            file_size,
    uint16            *width_ptr,
    uint16            *height_ptr
)
{
    BOOLEAN     result = FALSE;

    if ( (PNULL == full_path_ptr) || (0 == full_path_len) || (PNULL == width_ptr) || (PNULL == height_ptr))
    {
        //SCI_TRACE_LOW:"MMIAPIFMM_GetPictureWidthHeight param error!"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_2538_112_2_18_2_20_11_234, (uint8*) "");
        return FALSE;
    }

    if (0 != file_size)
    {
        result = GUIANIM_GetImgWidthHeight (width_ptr, height_ptr, full_path_ptr, full_path_len);/*lint !e605*/
    }

    return result;
}

