/*****************************************************************************
** File Name:      mmifmm_lsearch.c                                          *
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

#define _MMIFMM_LSEARCH_C_


/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_app_fmm_trc.h"
#include "mmifmm_lsearch.h"
#ifdef VIDEO_PLAYER_SUPPORT
#include "mmivp_export.h"
#endif
#ifdef BROWSER_SUPPORT
#include "mmibrowser_export.h"
#endif
#include "mmipub.h"
#if defined(DRM_SUPPORT)
#include "mmidrm_export.h"
#endif
#ifdef EBOOK_SUPPORT
#include "mmiebook_export.h"
#endif
#include "mmifmm_image.h"
#include "gui_ucs2b_converter.h"
#include "mmimp3_export.h"
#include "mmifmm_app.h"
#include "mmifmm_interface.h"
#include "mmifmmsrv_api.h"
#include "mmi_image.h"
#if defined (JAVA_SUPPORT_MYRIAD) || defined JAVA_SUPPORT_IA
#include "mmijava_export.h"
#endif
#ifdef SEARCH_SUPPORT
#include "mmisearch_export.h"
#endif
#ifdef SEARCH_SUPPORT
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         TYPE DEFINITION                                  *
 **--------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/
LOCAL FILEARRAY_DATA_T  s_file_data_info = {0};//store file data info(name,path.etc) for open privacy protect
LOCAL FILEARRAY         s_file_list_array = PNULL;

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : play music file
//  Global resource dependence :
//  Author: robert.lu
//  Note:
/*****************************************************************************/
LOCAL void SearchPlayMusicFile (uint32  intem_index);

#ifdef VIDEO_PLAYER_SUPPORT
/*****************************************************************************/
//  Description : play movie file
//  Global resource dependence :
//  Author: robert.lu
//  Note:
/*****************************************************************************/
LOCAL void SearchPlayMovieFile (uint16 item_index);
#endif

#ifdef BROWSER_SUPPORT
/*****************************************************************************/
//  Description : open www file
//  Global resource dependence :
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL void SearchOpenWWWFile (uint16 item_index);
#endif

/*****************************************************************************/
//  Description : get picture file info by index
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MMISEARCH_GetFileInfo (
    uint32             index,      //in
    FILEARRAY_DATA_T   *file_info_ptr  //in/out
);

#ifdef BROWSER_SUPPORT
/*****************************************************************************/
//  Description : open www file
//  Global resource dependence :
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL void SearchOpenWWWFile (uint16 item_index)
{
    uint8 *url_ptr = PNULL;
    uint16 str_len = 0;
    uint16 header_len = 0;
    // uint16 index  = 0;
    const char * file_header = "file:///";
    wchar full_file_name [MMIFMM_FILENAME_LEN + 1] = {0};
    MMIBROWSER_ENTRY_PARAM entry_param = {0};/*lint !e64*/
    FILEARRAY_DATA_T file_data_info = {0};

    //get file info
    if (!MMISEARCH_GetFileInfo (item_index, &file_data_info))
    {
        return;
    }

    header_len = (uint16) SCI_STRLEN (file_header);
    str_len = (uint16) (MMIAPICOM_Wstrlen (full_file_name) * 3 + header_len + 1);
    url_ptr = (uint8 *) SCI_ALLOCA (str_len * sizeof (uint8));

    if (PNULL == url_ptr)
    {
        return;
    }

    SCI_MEMSET (url_ptr, 0x00, (str_len * sizeof (uint8)));
    SCI_MEMCPY (url_ptr, file_header, header_len);
    str_len -= header_len;
    GUI_WstrToUTF8 (url_ptr + header_len, str_len, full_file_name, MMIAPICOM_Wstrlen (full_file_name));
    entry_param.type = MMIBROWSER_ACCESS_URL;
    entry_param.dual_sys = MN_DUAL_SYS_MAX;
    entry_param.url_ptr = (char *) url_ptr;
    entry_param.user_agent_ptr = PNULL;
    MMIAPIBROWSER_Entry (&entry_param);
    SCI_FREE (url_ptr);

    return;
}
#endif

#ifdef VIDEO_PLAYER_SUPPORT
/*****************************************************************************/
//  Description : play movie file
//  Global resource dependence :
//  Author: robert.lu
//  Note:
/*****************************************************************************/
LOCAL void SearchPlayMovieFile (uint16 item_index)
{
    FILEARRAY_DATA_T    file_info = {0};

    //SCI_TRACE_LOW:"SearchPlayMovieFile, item_index = %d"
    SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_INTERFACE_4498_112_2_18_2_20_28_332, (uint8*) "d", item_index);

    if (!MMISEARCH_GetFileInfo (item_index, &file_info))
    {
        //SCI_TRACE_LOW:"SearchPlayMovieFile: GET FILE ERROR, item_index = %d"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_INTERFACE_4502_112_2_18_2_20_28_333, (uint8*) "d", item_index);
        return;
    }

    if (0 == file_info.size)
    {
        MMIPUB_OpenAlertWinByTextId (PNULL, TXT_EMPTY_FILE, TXT_NULL, IMAGE_PUBWIN_WARNING, PNULL, PNULL, MMIPUB_SOFTKEY_ONE, PNULL);
        return;
    }

#ifdef MMI_VIDEOPLAYER_MINI_FUNCTION
    MMIAPIVP_MiniFunction_PlayVideo (file_info.filename, file_info.name_len);
#else
    MMIAPIVP_PlayVideoFromVideoPlayer (VP_ENTRY_FROM_FILE, file_info.filename, file_info.name_len, PNULL);
#endif
}

/*****************************************************************************/
// 	Description :
//	Global resource dependence :
//  Author:tao.xue
//	Note:
/*****************************************************************************/
LOCAL void ValidatePrivacyPlayMovie (BOOLEAN is_permitted, DPARAM param_ptr)
{
    uint16  item_index = 0;

    if (is_permitted)
    {
        if (PNULL != param_ptr)
        {
            item_index = * (uint16*) param_ptr;
        }

        SearchPlayMovieFile (item_index);
    }
}
#endif

/*****************************************************************************/
//  Description : play music file
//  Global resource dependence :
//  Author: robert.lu
//  Note:
/*****************************************************************************/
LOCAL void SearchPlayMusicFile (uint32  item_index)
{
    FILEARRAY_DATA_T               file_info = {0};
#ifdef  MUSIC_PLAYER_SUPPORT
    MMIFILE_FILE_INFO_T               new_file_info = {0};
#endif

    //SCI_TRACE_LOW:"SearchPlayMusicFile, item_index = %d"
    SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_INTERFACE_4545_112_2_18_2_20_28_334, (uint8*) "d", item_index);

    if (!MMISEARCH_GetFileInfo (item_index, &file_info))
    {
        //SCI_TRACE_LOW:"SearchPlayMusicFile: GET FILE ERROR, item_index = %d"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_INTERFACE_4549_112_2_18_2_20_28_335, (uint8*) "d", item_index);
        return;
    }

    if (0 == file_info.size)
    {
        MMIPUB_OpenAlertWinByTextId (PNULL, TXT_EMPTY_FILE, TXT_NULL, IMAGE_PUBWIN_WARNING, PNULL, PNULL, MMIPUB_SOFTKEY_ONE, PNULL);
        return;
    }

#ifdef  MUSIC_PLAYER_SUPPORT
    MMIAPIFMM_GetFileInfoFormFullPath (file_info.filename, file_info.name_len, &new_file_info);

    MMIAPIMP3_PlayMyDocumentMusic (&new_file_info);
#elif defined(MMI_AUDIO_PLAYER_SUPPORT)
    MMIAPIMP3_PlayFile (file_info.filename, file_info.name_len);
#endif
}

/*****************************************************************************/
// 	Description :
//	Global resource dependence :
//  Author:tao.xue
//	Note:
/*****************************************************************************/
LOCAL void ValidatePrivacyPlayMusic (BOOLEAN is_permitted, DPARAM param_ptr)
{
    uint16  item_index = 0;

    if (is_permitted)
    {
        if (PNULL != param_ptr)
        {
            item_index = * (uint16*) param_ptr;
        }

        SearchPlayMusicFile (item_index);
    }
}

/*****************************************************************************/
//  Description : get picture file info by index
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
//PUBLIC BOOLEAN MMISEARCH_GetFileInfo(
//                                         uint32             index,      //in
//                                         FILEARRAY_DATA_T   *file_info_ptr  //in/out
//                                         )
//{
//    BOOLEAN     result = FALSE;
//
//    if(PNULL == file_info_ptr)
//    {
//        //SCI_TRACE_LOW:"MMISEARCH_GetFileInfo:PNULL == file_info_ptr"
//        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIFMM_INTERFACE_4599_112_2_18_2_20_28_336,(uint8*)"");
//        return FALSE;
//    }
//
//    result = MMIAPIFILEARRAY_Read(s_file_list_array, index, file_info_ptr);
//    //SCI_TRACE_LOW:"MMIPICVIEW_GetPicFileInfo:file_size is %d,name len %d"
//    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIFMM_INTERFACE_4604_112_2_18_2_20_28_337,(uint8*)"dd",file_info_ptr->size,file_info_ptr->name_len);
//
//    if (0 == file_info_ptr->name_len)
//    {
//        result = FALSE;
//    }
//
//    return (result);
// }

/*****************************************************************************/
//  Description : 文件搜索结果的打开函数
//  Global resource dependence :
//  Author: hermann liu
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIFMM_OpenMatchedItem (uint32 user_data)
{
    MMIFMM_FILE_TYPE_E      file_type = MMIFMM_FILE_TYPE_NORMAL;
    MMIFILE_FILE_INFO_T     file_info = {0};



    //get file info
    SCI_MEMSET (&s_file_data_info, 0x00, sizeof (FILEARRAY_DATA_T));

    if (!MMISEARCH_GetFileInfo (user_data, &s_file_data_info))
    {
        return;
    }

    MMIAPIFMM_GetFileInfoFormFullPath (s_file_data_info.filename,
                                       s_file_data_info.name_len, &file_info);

    file_type = MMIAPIFMM_GetFileTypeByFileName (s_file_data_info.filename,
                s_file_data_info.name_len);

    // 如果是文件夹，进入文件夹
    if (FILEARRAY_TYPE_FOLDER == s_file_data_info.type)
    {
        MMIPUB_OpenAlertWinByTextId (PNULL, TXT_COMMON_WAITING, TXT_NULL, IMAGE_PUBWIN_WAIT, PNULL, PNULL, MMIPUB_SOFTKEY_NONE, PNULL);
        MMIAPIFMM_OpenExplorerWithoutProtect (s_file_data_info.filename,
                                              s_file_data_info.name_len, PNULL, 0, FALSE, MMIFMM_FILE_ALL);
    }
    else if (!MMIAPIFMM_IsFileExist (s_file_data_info.filename, s_file_data_info.name_len))
    {
        //文件不存在
        MMIPUB_OpenAlertWinByTextId (PNULL, TXT_COM_FILE_NO_EXIST, TXT_NULL, IMAGE_PUBWIN_WARNING, PNULL, PNULL, MMIPUB_SOFTKEY_ONE, PNULL);
    }
    //  如果是文件，根据文件的后缀，查找播放器程序
    else
    {
        //SCI_TRACE_LOW:"MMIAPIFMM_OpenMatchedItem, call relative application!"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_INTERFACE_4653_112_2_18_2_20_28_338, (uint8*) "");

#ifdef DRM_SUPPORT

        //  如果是DRM文件，进一步分析是何种媒体文件
        if (MMIFMM_FILE_TYPE_DRM == file_type)
        {
            DRM_PERMISSION_MODE_E drm_permission = 0;
            DRMFILE_PRE_CHECK_STATUS_E pre_check_drmfile_status = DRMFILE_PRE_CHECK_NORMAL;

            //FmmCombineFullFileName(&s_fmm_list_data,&s_fmm_current_path,index,s_full_file_name,MMIFMM_FULL_FILENAME_LEN);
            file_type = MMIAPIDRM_GetMediaFileType (SFS_INVALID_HANDLE, s_file_data_info.filename);

            switch (file_type)
            {
            case MMIFMM_FILE_TYPE_PICTURE:
            case MMIFMM_FILE_TYPE_EBOOK:
#if defined MMI_VCARD_SUPPORT
            case MMIFMM_FILE_TYPE_VCARD:
#endif
                drm_permission = DRM_PERMISSION_DISPLAY;
                break;

            case MMIFMM_FILE_TYPE_MUSIC :
            case MMIFMM_FILE_TYPE_MOVIE :
                drm_permission = DRM_PERMISSION_PLAY;
                break;

            case MMIFMM_FILE_TYPE_JAVA :
                drm_permission = DRM_PERMISSION_EXECUTE;
                break;

            default:
                break;
            }

            if (0 == drm_permission)
            {
                MMIPUB_OpenAlertWinByTextId (PNULL, TXT_COMMON_NO_SUPPORT, TXT_NULL, IMAGE_PUBWIN_WARNING, PNULL, PNULL, MMIPUB_SOFTKEY_ONE, PNULL);
                return;
            }

            pre_check_drmfile_status = MMIAPIDRM_PreCheckFileStatus (s_file_data_info.filename, drm_permission);

            if (DRMFILE_PRE_CHECK_NORMAL != pre_check_drmfile_status)
            {
                if (DRMFILE_PRE_CHECK_NO_RIGHTS == pre_check_drmfile_status)
                {
                    //如果是无效的，则需要提示guilist去刷新本行
                    //MMIPUB_OpenAlertWinByTextId(PNULL,TXT_DRM_COPYRIGHTS_PROTECTION_NOT_OPERATE,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
                }

                return;
            }
        }

#endif

        switch (file_type)
        {
        case MMIFMM_FILE_TYPE_PICTURE:
            // 生成图像文件数据
            //SetupPicIndexData(ctrl_id, &s_fmm_list_data, &s_pic_index_data);
            // 打开图像浏览窗口
            MMIFMM_SearchOpenPicturePreviewWin (user_data, s_file_list_array);
            break;

#if defined(MMI_AUDIO_PLAYER_SUPPORT)
        case MMIFMM_FILE_TYPE_MUSIC:
            MMIAPISET_ValidatePrivacy (MMISET_PROTECT_AUDIO_PLAYER,  ValidatePrivacyPlayMusic, &user_data, sizeof (user_data));
            break;
#endif

#ifdef VIDEO_PLAYER_SUPPORT
        case MMIFMM_FILE_TYPE_MOVIE:
            MMIAPISET_ValidatePrivacy (MMISET_PROTECT_VIDEO_PLAYER, ValidatePrivacyPlayMovie, &user_data, sizeof (user_data));
            break;
#endif

#ifdef EBOOK_SUPPORT
        case MMIFMM_FILE_TYPE_EBOOK:
            {
                MMIFILE_DEVICE_E        device       = MMI_DEVICE_SYSTEM;
                uint16                  file_name_len = MMIFMM_FILENAME_LEN;
                wchar                   file_name [MMIFMM_FILENAME_LEN+1] = {0};

                // 显示电子书的内容
                if (MMIAPISET_IsOpenPrivacyProtect (MMISET_PROTECT_EBOOK))
                {
                    MMIAPISET_ValidatePrivacyAppEntry (MMISET_PROTECT_EBOOK, MMIAPIEBOOK_ValidatePrivacyAppEntry);
                }
                else
                {
                    //FmmCombineFullFileName(list_data_ptr,current_path_ptr,index,s_full_file_name,MMIFMM_FULL_FILENAME_LEN);
                    if (0 == s_file_data_info.size)
                    {
                        MMIPUB_OpenAlertWinByTextId (PNULL, TXT_EMPTY_FILE, TXT_NULL, IMAGE_PUBWIN_WARNING, PNULL, PNULL, MMIPUB_SOFTKEY_ONE, PNULL);
                        return ;
                    }

                    device = MMIAPIFMM_GetDeviceTypeByPath (s_file_data_info.filename, MMIFILE_DEVICE_LEN);

                    MMIAPICOM_GetFileName (s_file_data_info.filename, s_file_data_info.name_len, file_name, &file_name_len);
                    MMIAPIEBOOK_ManagerWinMsg_4Fmm ( (uint8) device, file_name, file_name_len,
                                                     s_file_data_info.size, s_file_data_info.filename, s_file_data_info.name_len);
                }

                break;
            }
#endif
#if defined MMI_VCARD_SUPPORT
        case MMIFMM_FILE_TYPE_VCARD:
            // 打开VCARD文件
            //MMIAPIPB_OpenVcardFileContactWin( s_file_data_info.filename);
            MMIAPIFMM_ShowTxtContent (s_file_data_info.filename);
            break;
#endif
#ifdef DYNAMIC_MODULE_SUPPORT
        case MMIFMM_FILE_TYPE_SZIP:
            MMIDYNA_InstallApp (s_file_data_info.filename);
            break;
#endif

#ifdef JAVA_SUPPORT
        case MMIFMM_FILE_TYPE_JAVA:
            {
                uint16          path_name_len = MMIFMM_PATHNAME_LEN;
                wchar           path_name[MMIFMM_PATHNAME_LEN+1] = {0};
                uint16          suffix_len = MMISEARCH_SUFFIX_MAX_LEN;
                wchar           suffix[MMISEARCH_SUFFIX_MAX_LEN+1] = {0};
                uint16          file_name_len = MMIFMM_FILENAME_LEN;
                wchar           file_name [MMIFMM_FILENAME_LEN+1] = {0};

                if (0 == s_file_data_info.size)
                {
                    MMIPUB_OpenAlertWinByTextId (PNULL, TXT_EMPTY_FILE, TXT_NULL, IMAGE_PUBWIN_WARNING, PNULL, PNULL, MMIPUB_SOFTKEY_ONE, PNULL);
                }
                else
                {
                    //split file name
                    MMIAPIFMM_SplitFullPathExt (s_file_data_info.filename, s_file_data_info.name_len,
                                                path_name, &path_name_len,
                                                file_name, &file_name_len,
                                                suffix, &suffix_len);

                    MMI_WSTRNCPY (file_name + file_name_len,
                                  (MMIFMM_FILENAME_LEN - file_name_len),
                                  suffix, suffix_len, suffix_len);

                    file_name_len += suffix_len;

                    if ('\\' == path_name[path_name_len - 1])
                    {
                        path_name[path_name_len - 1] = 0;
                        path_name_len = path_name_len - 1;
                    }

#ifdef JAVA_SUPPORT_SUN //需要统一接口
                    MMIAPIJAVA_InstallFromFilesystem (path_name, path_name_len,
                                                      file_name, file_name_len);
#elif JAVA_SUPPORT_IA /*lint !e553*/
                    MMIAPIJAVA_InstallFromFilesystem (s_file_data_info.filename, s_file_data_info.name_len);
#elif defined (JAVA_SUPPORT_MYRIAD)
                MMIAPIJAVA_Install (s_file_data_info.filename,  s_file_data_info.name_len);
#endif

                }
            }
            break;
#endif

        case  MMIFMM_FILE_TYPE_BROWSER:
#ifdef BROWSER_SUPPORT
            SearchOpenWWWFile (user_data);
#endif
            break;

        default:
            // 提示格式不支持
            MMIPUB_OpenAlertWinByTextId (PNULL, TXT_COMMON_NO_SUPPORT, TXT_NULL, IMAGE_PUBWIN_WARNING, PNULL, PNULL, MMIPUB_SOFTKEY_ONE, PNULL);
            break;
        }
    }

    return;
}
/*****************************************************************************/
// 	Description : 文件搜索结果的显示回调函数
//	Global resource dependence :
//    Author:hermann liu
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIFMM_SetMatchItemData (
    GUILIST_NEED_ITEM_DATA_T *need_item_data_ptr,
    uint32 user_data
)
{
    uint16 index = user_data;
    FILEARRAY_DATA_T f_array_data = {0};
//   MMI_CTRL_ID_T	 ctrl_id = MMISEARCH_RESULT_LISTBOX_CTRL_ID;
    GUILIST_ITEM_DATA_T item_data = {0};
    GUIITEM_STYLE_E item_style = 0;

    SCI_MEMSET (&f_array_data, 0, sizeof (f_array_data));

    if (!MMIAPIFILEARRAY_Read (s_file_list_array, index, &f_array_data))
    {
        //SCI_TRACE_LOW:"MMISEARCH_LoadFileRstToList: READ file array error!"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_INTERFACE_4846_112_2_18_2_20_29_339, (uint8*) "");
        return;
    }

//    item_style = GUIITEM_STYLE_TWO_LINE_ANIM_ICON_TEXT_AND_ICON_2TEXT;
    item_style = GUIITEM_STYLE_TWO_LINE_ANIM_TWO_TEXT_AND_TEXT_2;

    item_data.softkey_id[LEFT_BUTTON]      = TXT_NULL;
    item_data.softkey_id[MIDDLE_BUTTON]    = TXT_VIEW;
    item_data.softkey_id[RIGHT_BUTTON]     = STXT_RETURN;

    item_data.item_content[0].item_data_type = GUIITEM_DATA_IMAGE_ID;

    if (FILEARRAY_TYPE_FOLDER == f_array_data.type)
    {
        item_data.item_content[0].item_data.image_id = IMAGE_FMM_FOLDER_ICON;
    }
    else
    {
        item_data.item_content[0].item_data.image_id = MMIFMM_GetIconIdByFileName (f_array_data.filename, f_array_data.name_len);
    }

    {
        wchar *str_ptr = PNULL;
        wchar  str_ptr2[1] = {L'\\'};
        uint16 file_name_len = 0;

        while ( (uint16) f_array_data.name_len > file_name_len)
        {
            if (0 == MMIAPICOM_CompareTwoWstr (str_ptr2, 1, (f_array_data.filename + (uint16) f_array_data.name_len - file_name_len - 1), 1)) //L'/' == *(f_array_data.filename+(uint16)f_array_data.name_len - file_name_len -1))
            {
                str_ptr = f_array_data.filename + (uint16) f_array_data.name_len - file_name_len;
                break;
            }

            file_name_len++;
        }

        if (str_ptr == PNULL)
        {
            file_name_len = 0;
        }

        item_data.item_content[1].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
        item_data.item_content[1].item_data.text_buffer.wstr_ptr = str_ptr;
        item_data.item_content[1].item_data.text_buffer.wstr_len = file_name_len;
    }


    item_data.item_content[3].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[3].item_data.text_buffer.wstr_ptr = f_array_data.filename;
    item_data.item_content[3].item_data.text_buffer.wstr_len = (uint16) f_array_data.name_len;

    item_data.softkey_id[LEFT_BUTTON]      = TXT_NULL;
    item_data.softkey_id[MIDDLE_BUTTON]    = TXT_VIEW;
    item_data.softkey_id[RIGHT_BUTTON]     = STXT_RETURN;

    GUILIST_SetItemStyle (need_item_data_ptr->ctrl_id, item_style, need_item_data_ptr->item_index);
    GUILIST_SetItemData (need_item_data_ptr->ctrl_id, &item_data, need_item_data_ptr->item_index);
    //get file name
}

/*****************************************************************************/
// 	Description : start search for local search
//	Global resource dependence :
//    Author:hermann liu
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIFMM_StartLocalSearch (
    MMI_STRING_T  *keyword_str_ptr,
    MMI_WIN_ID_T        notice_win_id,         //如果win_id和msg_id都为0，则采用同步方式搜索文件
    MMI_MESSAGE_ID_E    notice_msg_id)
{
    if (PNULL != s_file_list_array)
    {
        MMIAPIFILEARRAY_Destroy (&s_file_list_array);
        s_file_list_array = PNULL;
    }

    s_file_list_array = MMIAPIFILEARRAY_Create();

    MMK_CheckAllocatedMemInfo( _D_FILE_NAME, _D_FILE_LINE );
    MMIFMM_SearchFileInPathByString (
        PNULL, 0,
        keyword_str_ptr->wstr_ptr, TRUE,
        FILEARRAY_SORT_NAME_ASCEND,
        FUNC_FIND_FILE_AND_ALLFOLER,
        s_file_list_array,
        notice_win_id,
        notice_msg_id);
    MMK_CheckAllocatedMemInfo( _D_FILE_NAME, _D_FILE_LINE );
}
/*****************************************************************************/
//  Description : free file search resource
//  Global resource dependence :
//  Author: hermann liu
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIFMM_FreeLocalSearchResource (void)
{
    if (PNULL != s_file_list_array)
    {
        MMIAPIFILEARRAY_Destroy (&s_file_list_array);
        s_file_list_array = PNULL;
    }

    MMIAPISEARCH_ClearCurrFileMatchNum();
}
/*****************************************************************************/
//  Description : get file search matched num
//  Global resource dependence :
//  Author: hermann liu
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIAPIFMM_GetFileSearchMatchedNum (void)
{
    return MMIAPIFILEARRAY_GetArrayCurSize(s_file_list_array);
}

/*****************************************************************************/
//  Description : get picture file info by index
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MMISEARCH_GetFileInfo (
    uint32             index,      //in
    FILEARRAY_DATA_T   *file_info_ptr  //in/out
)
{
    BOOLEAN     result = FALSE;

    if (PNULL == file_info_ptr)
    {
        //SCI_TRACE_LOW:"MMISEARCH_GetFileInfo:PNULL == file_info_ptr"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_INTERFACE_4599_112_2_18_2_20_28_336, (uint8*) "");
        return FALSE;
    }

    result = MMIAPIFILEARRAY_Read (s_file_list_array, index, file_info_ptr);
    //SCI_TRACE_LOW:"MMIPICVIEW_GetPicFileInfo:file_size is %d,name len %d"
    SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_INTERFACE_4604_112_2_18_2_20_28_337, (uint8*) "dd", file_info_ptr->size, file_info_ptr->name_len);

    if (0 == file_info_ptr->name_len)
    {
        result = FALSE;
    }

    return (result);
}
#endif
