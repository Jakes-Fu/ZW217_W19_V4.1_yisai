/*****************************************************************************
** File Name:      mmifmm_renamewin.c                                        *
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

#define _MMIFMM_RENAMEWIN_C_


/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_app_fmm_trc.h"
#include "mmi_appmsg.h"
#include "mmifmm_renamewin.h"
#include "mmifmm_id.h"
#include "mmifmm_text.h"
#include "mmipub.h"
#include "mmiset_export.h"
#include "mmifmm_comfunc.h"
#include "mmienvset_export.h"
#include "window_parse.h"
#include "mmi_wallpaper_export.h"
#ifdef  VIDEOTHUMBNAIL_SUPPORT
#include "mmifmm_vthumbnail.h"
#endif
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         TYPE DEFINITION                                  *
 **--------------------------------------------------------------------------*/
typedef struct
{
    MMIFMM_RENAME_WIN_PARAM_T *rename_papam;
    wchar       *rename_buffer_ptr;
    uint32      max_file_name_len; //目录名字可以改到的最大长度，ucs2 len
    uint32      file_name_ext_len; //"."加扩展名长度，ucs2 len
} MMIFMM_RENAMEWIN_INFO_T;

/**-------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
// Description : 改名窗口，目录和文件改名都用此窗口
// Global resource dependence :
// Author:Ming.Song 2007-4-19
// Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleFmmRenameWinMsg (
    MMI_WIN_ID_T         win_id,
    MMI_MESSAGE_ID_E     msg_id,
    DPARAM               param
);

/*****************************************************************************/
// Description : open ctrl func
// Global resource dependence :
// Author:  jian.ma
// Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E RenameOpenCtrlFunc (MMI_WIN_ID_T win_id);

/*****************************************************************************/
// Description : close ctrl func
// Global resource dependence :
// Author:  jian.ma
// Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E RenameCloseCtrlFunc (MMI_WIN_ID_T win_id);

/*****************************************************************************/
// Description : 目录改名，拆分目录名字
// Global resource dependence :
// Author: Ming.Song 2007-4-19
// Note:
/*****************************************************************************/
LOCAL BOOLEAN FmmRenameSplitFolder (
    const wchar   *full_path_ptr,             //in 全路径: "d:\abc\def\mp3"
    wchar   *before_the_last_path_ptr,  //out 除去最后一级目录的全路径加"\": "d:\abc\def\"
    wchar   *the_last_path_ptr,         //out 最后一级目录，即要改的目录名字: "mp3"
    uint32   *max_folder_name_len        //out 目录名字可以改到的最大长度，ucs2 len
);

/*****************************************************************************/
// Description : 文件改名，拆分文件名字
// Global resource dependence :
// Author: Ming.Song 2007-4-19
// Note:
/*****************************************************************************/
LOCAL BOOLEAN FmmRenameSplitFile (
    const wchar *full_path_ptr,                 //in 全路径: "d:\abc\def\mp3\a.mp3"
    wchar *the_path_ptr,                  //out 路径加"\": "d:\abc\def\mp3\"
    wchar *file_name_without_ext,         //out 要改的文件名字，不包括扩展名: "a"
    uint32 *max_file_name_without_ext_len, //out ucs2 len
    wchar *file_name_ext,                 //out "."加扩展名: ".mp3"
    uint32 *file_name_ext_len              //out "."加扩展名长度，ucs2 len
);
/*****************************************************************************/
// Description : check is call ring info
// Global resource dependence :
// Author: jian.ma
// Note:
/*****************************************************************************/
LOCAL BOOLEAN CheckIsCallringInfo (
    BOOLEAN *is_call_ring_info,
    const wchar  *full_path_wstr_ptr,
    uint16  old_name_len
);

#ifdef SCREENSAVER_SUPPORT
/*****************************************************************************/
// Description : check is screen saver info
// Global resource dependence :
// Author: jian.ma
// Note:
/*****************************************************************************/
LOCAL BOOLEAN CheckIsScreensaverInfo (
    MMISET_SCREENSAVER_INFO_T *ssaver_info,
    const wchar *full_path_wstr_ptr,
    uint16 old_name_len
);
#endif


/*****************************************************************************/
// Description : check is power on/off anim info
// Global resource dependence :
// Author: jian.ma
// Note:
/*****************************************************************************/
LOCAL BOOLEAN CheckIsPowerOnOffAnimInfo (
    MMISET_POWER_ON_OFF_ANIM_INFO_T *power_onoff_anim_info,
    const wchar *full_path_wstr_ptr,
    uint16 old_name_len
);
//rename win
WINDOW_TABLE (MMIFMM_RENAME_WIN_TAB) =
{
    WIN_FUNC ( (uint32) HandleFmmRenameWinMsg),
    WIN_ID (MMIFMM_RENAME_WIN_ID),
    WIN_TITLE (TXT_COMMON_RENAME),
#ifdef MMI_PDA_SUPPORT
    CREATE_FORM_CTRL (GUIFORM_LAYOUT_ORDER, MMIFMM_RENAME_FORM_CTRL_ID),
    CHILD_EDIT_TEXT_CTRL (TRUE, 0, MMIFMM_RENAME_EDITBOX_CTRL_ID, MMIFMM_RENAME_FORM_CTRL_ID),
    CHILD_SOFTKEY_CTRL (TXT_COMMON_OK, TXT_NULL, STXT_RETURN, MMIFMM_RENAME_SOFTKEY_CTRL_ID, MMIFMM_RENAME_FORM_CTRL_ID),
#else
    WIN_SOFTKEY (TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_EDIT_TEXT_CTRL (0, MMIFMM_RENAME_EDITBOX_CTRL_ID),
#endif
    END_WIN
};
/*****************************************************************************/
// Description : enter rename win
// Global resource dependence :
// Author:  jian.ma
// Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIFMM_EnterReNameWin (
    MMIFMM_RENAME_WIN_PARAM_T*  rename_param// 数据
)
{
    BOOLEAN                     result = MMI_RESULT_FALSE;
    MMIFMM_RENAME_WIN_PARAM_T   *param_to_rename_win_ptr = PNULL;
    size_t                      name_len = 0;
    MMI_HANDLE_T                win_handle = 0;

    if (PNULL == rename_param)
    {
        return MMI_RESULT_FALSE;
    }

    if (MMK_IsOpenWin (MMIFMM_RENAME_WIN_ID))
    {
        MMK_CloseWin (MMIFMM_RENAME_WIN_ID);
    }

    param_to_rename_win_ptr = (MMIFMM_RENAME_WIN_PARAM_T *) SCI_ALLOC_APPZ (sizeof (MMIFMM_RENAME_WIN_PARAM_T));

    if (PNULL != param_to_rename_win_ptr)
    {
        name_len = MMIAPICOM_Wstrlen (rename_param->full_path_ptr);
        param_to_rename_win_ptr->full_path_ptr = (wchar *) SCI_ALLOC_APPZ ( (MMIFILE_FULL_PATH_MAX_LEN + 1) * sizeof (wchar));
        SCI_MEMSET (param_to_rename_win_ptr->full_path_ptr, 0x00, (MMIFILE_FULL_PATH_MAX_LEN + 1) *sizeof (wchar));
        MMIAPICOM_Wstrncpy (param_to_rename_win_ptr->full_path_ptr, rename_param->full_path_ptr, MIN (MMIFILE_FULL_PATH_MAX_LEN, name_len));
        param_to_rename_win_ptr->type = rename_param->type;
        param_to_rename_win_ptr->win_id = rename_param->win_id;
        param_to_rename_win_ptr->msg_id = rename_param->msg_id;
        param_to_rename_win_ptr->max_file_lan = rename_param->max_file_lan;
        win_handle = MMK_CreateWin ( (uint32 *) MMIFMM_RENAME_WIN_TAB , (ADD_DATA) param_to_rename_win_ptr);

        if (!win_handle || (MMI_INVALID_ID == win_handle))
        {
            SCI_TRACE_LOW ("[MMIFMM] MMIFMM_EnterReNameWin create MMIFMM_RENAME_WIN_TAB failed !");
            result =  MMI_RESULT_FALSE;
            SCI_FREE (param_to_rename_win_ptr->full_path_ptr);
            SCI_FREE (param_to_rename_win_ptr);
        }
        else
        {
            result = MMI_RESULT_TRUE;
        }
    }
    else
    {
        SCI_TRACE_LOW ("MMIFMM: MMIFMM_EnterReNameWin, no memory!");
    }

    return result;
}

/*****************************************************************************/
// Description : 改名窗口，目录和文件改名都用此窗口
// Global resource dependence :
// Author:Ming.Song 2007-4-19
// Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleFmmRenameWinMsg (
    MMI_WIN_ID_T         win_id,
    MMI_MESSAGE_ID_E     msg_id,
    DPARAM               param
)
{
    BOOLEAN                     is_complete_name = TRUE;
    MMI_RESULT_E                result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T               ctrl_id = MMIFMM_RENAME_EDITBOX_CTRL_ID;
    MMI_STRING_T                name_str = {0};
    MMIFMM_RENAMEWIN_INFO_T     *rename_param_ptr = PNULL;
    uint16                      string_len = 0;
    uint16                      old_name_len = 0;
    uint16                      new_name_len = 0;
    //全路径，"d:\abc\def\mp3\a.mp3"
    wchar                       *path_buf_ptr = PNULL; //file:路径加"\": "d:\abc\def\mp3\" //folder:除去最后一级目录的全路径加"\": "d:\abc\def\"
    wchar                       *file_buf_ptr = PNULL; //file:要改的文件名字，不包括扩展名: "a"folder:最后一级目录，即要改的目录名字: "mp3"
    wchar                       *ext_buf_ptr  = PNULL; //file:"."加扩展名: ".mp3"
    wchar                       *new_name_ptr = PNULL;
    SFS_ERROR_E                 rename_result = SFS_NO_ERROR;
    MMIFILE_DEVICE_E            filedev = MMI_DEVICE_UDISK;

    rename_param_ptr = (MMIFMM_RENAMEWIN_INFO_T*) MMK_GetWinUserData (win_id);

    //SCI_TRACE_LOW:"MMIFMM: HandleFmmRenameWinMsg, win_id =0x%x, msg_id = 0x%x."
    SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_6423_112_2_18_2_20_48_443, (uint8*) "dd", win_id, msg_id);

    //UNUSED_PARAM(param);
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
#ifdef MID_SOFTKEY_USE_IMG_SUPPORT
		GUIWIN_SeSoftkeytButtonIconId(win_id, IMAGE_NULL, 1, TRUE);
#endif
        if (MMI_RESULT_FALSE == RenameOpenCtrlFunc (win_id))
        {
            MMK_CloseWin (win_id);
            break;
        }

        rename_param_ptr = (MMIFMM_RENAMEWIN_INFO_T*) MMK_GetWinUserData (win_id);

        if (PNULL == rename_param_ptr)
        {
            //SCI_TRACE_LOW:"[MMIFMM] HandleFmmRenameWinMsg MSG_OPEN_WINDOW PNULL == rename_param_ptr"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_6439_112_2_18_2_20_48_444, (uint8*) "");
            MMK_CloseWin (win_id);
            break;
        }

        path_buf_ptr = rename_param_ptr->rename_buffer_ptr;
        file_buf_ptr = rename_param_ptr->rename_buffer_ptr + MMIFMM_FULL_FILENAME_LEN + 1;
        ext_buf_ptr  = rename_param_ptr->rename_buffer_ptr + (MMIFMM_FULL_FILENAME_LEN + 1) * 2;
        new_name_ptr = rename_param_ptr->rename_buffer_ptr + (MMIFMM_FULL_FILENAME_LEN + 1) * 3;

        if (MMIFMM_FILE_TYPE_FOLDER == rename_param_ptr->rename_papam->type)
        {
            if (!FmmRenameSplitFolder (rename_param_ptr->rename_papam->full_path_ptr, path_buf_ptr, file_buf_ptr, &rename_param_ptr->max_file_name_len))
            {
                MMK_CloseWin (win_id);
                return result;
            }
        }
        else
        {
            if (!FmmRenameSplitFile (rename_param_ptr->rename_papam->full_path_ptr, path_buf_ptr, file_buf_ptr, &rename_param_ptr->max_file_name_len, ext_buf_ptr, &rename_param_ptr->file_name_ext_len))
            {
                MMK_CloseWin (win_id);
                return result;
            }
        }

        //set left softkey
        GUIEDIT_SetSoftkey (ctrl_id, 0, 1, TXT_NULL, TXT_COMMON_OK, PNULL);

        if (rename_param_ptr->max_file_name_len > rename_param_ptr->rename_papam->max_file_lan)
        {
            rename_param_ptr->max_file_name_len = (uint16) (rename_param_ptr->max_file_name_len - rename_param_ptr->rename_papam->max_file_lan);
        }
        else
        {
            rename_param_ptr->max_file_name_len = 0;
        }

        //SCI_TRACE_LOW:"HandleFmmRenameWinMsg, max_file_name_len = %d."
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_6471_112_2_18_2_20_48_445, (uint8*) "d", rename_param_ptr->max_file_name_len);
        GUIEDIT_SetTextMaxLen (ctrl_id, (uint16) rename_param_ptr->max_file_name_len, (uint16) rename_param_ptr->max_file_name_len);
        string_len = (uint16) MMIAPICOM_Wstrlen (file_buf_ptr);
        GUIEDIT_SetString (ctrl_id, file_buf_ptr, string_len);
		//回退红键处理
		//GUIEDIT_SetHandleRedKey(FALSE, ctrl_id);
        MMK_SetAtvCtrl (win_id, ctrl_id);
        break;

    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_CTL_MIDSK:
        GUIEDIT_GetString (ctrl_id, &name_str);

        path_buf_ptr = rename_param_ptr->rename_buffer_ptr;
        file_buf_ptr = rename_param_ptr->rename_buffer_ptr + MMIFMM_FULL_FILENAME_LEN + 1;
        ext_buf_ptr  = rename_param_ptr->rename_buffer_ptr + (MMIFMM_FULL_FILENAME_LEN + 1) * 2;
        new_name_ptr = rename_param_ptr->rename_buffer_ptr + (MMIFMM_FULL_FILENAME_LEN + 1) * 3;

        SCI_MEMSET (new_name_ptr, 0x00, (sizeof (wchar) * (MMIFMM_FULL_FILENAME_LEN + 1)));

        if (0 == name_str.wstr_len)
        {

            MMIPUB_OpenAlertWinByTextId (PNULL, TXT_DC_NAMEINVALID, TXT_NULL, IMAGE_PUBWIN_WARNING , PNULL, PNULL, MMIPUB_SOFTKEY_ONE, PNULL);
            break;
        }

        //名字是否有效
        filedev = MMIAPIFMM_GetDeviceTypeByPath (rename_param_ptr->rename_papam->full_path_ptr, MMIFILE_DEVICE_LEN);

        if (MMIFMM_NewFolderNameStringIsValid (&name_str, rename_param_ptr->max_file_name_len)&&(L'.' != name_str.wstr_ptr[0]))
        {//CR208889
            if (PNULL != path_buf_ptr)
            {
                string_len = (uint16) MMIAPICOM_Wstrlen (path_buf_ptr);
                MMIAPICOM_Wstrncpy (new_name_ptr, path_buf_ptr, string_len);
            }

            if (MMIFMM_FILE_TYPE_FOLDER != rename_param_ptr->rename_papam->type)
            {
                if (0 != rename_param_ptr->file_name_ext_len && PNULL != ext_buf_ptr) /*lint !e774 */
                {
                    is_complete_name = FALSE;
                }
            }

            if (!MMIAPICOM_GetValidFileName (is_complete_name, name_str.wstr_ptr, name_str.wstr_len,
                                             &new_name_ptr[string_len], (MMIFMM_FULL_FILENAME_LEN - string_len)))
            {
                MMIFMM_PromptFileErrorType (PNULL, SFS_ERROR_INVALID_PARAM, filedev, TRUE);
                //MMIPUB_OpenAlertFailWin(TXT_DC_NAMEINVALID);//TXT_DC_NAMEINVALID
                break;
            }

            if (MMIFMM_FILE_TYPE_FOLDER != rename_param_ptr->rename_papam->type)
            {
                if (0 != rename_param_ptr->file_name_ext_len && PNULL != ext_buf_ptr) /*lint !e774 */
                {
                    string_len = (uint16) MMIAPICOM_Wstrlen (new_name_ptr);
                    MMIAPICOM_Wstrncpy (&new_name_ptr[string_len], ext_buf_ptr, rename_param_ptr->file_name_ext_len);
                }
            }
            else
            {
                //crrect folder name, such as remove "."," ",from tail
                MMIFMM_CorrectFolderName (new_name_ptr);
            }

            old_name_len = (uint16) MMIAPICOM_Wstrlen (rename_param_ptr->rename_papam->full_path_ptr);
            new_name_len = (uint16) MMIAPICOM_Wstrlen (new_name_ptr);

            if (MMIFMM_FILE_TYPE_FOLDER == rename_param_ptr->rename_papam->type)
            {
				SCI_TRACE_BUF(" mmifmm_renamewin.c new_name_ptr",new_name_ptr,new_name_len);
                rename_result = SFS_RenameFile (rename_param_ptr->rename_papam->full_path_ptr, new_name_ptr, NULL);

                if (SFS_ERROR_HAS_EXIST == rename_result)
                {
                    MMIPUB_OpenAlertWinByTextId (PNULL, TXT_FMM_NAME_DUPLICATE, TXT_NULL, IMAGE_PUBWIN_WARNING , PNULL, PNULL, MMIPUB_SOFTKEY_ONE, PNULL);
                }
                else
                {
                    MMIFMM_PromptFileErrorType (PNULL, rename_result, filedev, FALSE);
                    //SCI_TRACE_LOW:"HandleFmmRenameWinMsg: rename_result = %d"
                    SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_6561_112_2_18_2_20_48_446, (uint8*) "d", rename_result);

                    if (SFS_ERROR_NONE == rename_result)
                    {
                        MMIFMM_RENAME_RETURN_T rename_return = {0};
                        MMIAPICOM_Wstrncpy (rename_return.file_name_ptr, new_name_ptr, new_name_len);
                        rename_return.file_name_len = new_name_len;
                        MMK_SendMsg ( (MMI_WIN_ID_T) rename_param_ptr->rename_papam->win_id, (MMI_MESSAGE_ID_E) rename_param_ptr->rename_papam->msg_id, (void*) &rename_return);
                        MMK_CloseWin (win_id);
                    }
                }
            }
            else
            {
                //if(!MMIAPIFMM_IsFileExist(new_name_ptr,new_name_len))
                {
                    MMISET_POWER_ON_OFF_ANIM_INFO_T  power_on_anim_info     = {0};
                    MMISET_POWER_ON_OFF_ANIM_INFO_T  power_off_anim_info    = {0};
#ifdef SCREENSAVER_SUPPORT
                    MMISET_SCREENSAVER_INFO_T        ssaver_info            = {0};
                    BOOLEAN                          file_is_ssaver         = FALSE;
#endif
                    BOOLEAN                          file_is_power_on_anim  = FALSE;
                    BOOLEAN                          file_is_power_off_anim = FALSE;
                    BOOLEAN                          file_is_call_info[MMI_DUAL_SYS_MAX]   = {0};
                    BOOLEAN                          is_success             = TRUE;

                    MMIAPISET_GetPowerOnOffAnimInfo (&power_on_anim_info, TRUE);
                    MMIAPISET_GetPowerOnOffAnimInfo (&power_off_anim_info, FALSE);

                    file_is_power_on_anim  = CheckIsPowerOnOffAnimInfo (&power_on_anim_info, rename_param_ptr->rename_papam->full_path_ptr, old_name_len);
                    file_is_power_off_anim = CheckIsPowerOnOffAnimInfo (&power_off_anim_info, rename_param_ptr->rename_papam->full_path_ptr, old_name_len);
                    CheckIsCallringInfo (file_is_call_info, rename_param_ptr->rename_papam->full_path_ptr, old_name_len);

                    rename_result = SFS_RenameFile (rename_param_ptr->rename_papam->full_path_ptr, new_name_ptr, NULL);

                    if (SFS_ERROR_NONE == rename_result)
                    {
#ifdef  VIDEOTHUMBNAIL_SUPPORT//cr130686
						wchar           dir_name[MMIFILE_FULL_PATH_MAX_LEN+1] = {0};
						uint16          dir_name_len = 0;
						if(MMIAPIFMM_SplitFullPath(rename_param_ptr->rename_papam->full_path_ptr, 
							old_name_len,
							PNULL, PNULL,
							dir_name, &dir_name_len,
                            NULL, NULL))
						{
							if (MMIFMM_IsDefaultVideoFolder(dir_name,dir_name_len))
							{
								MMIFMM_VideoThumbnailDeleteRelatedFile(rename_param_ptr->rename_papam->full_path_ptr);
							}
						}
#endif
                        if (MMIFMM_FILE_TYPE_MOVIE == rename_param_ptr->rename_papam->type)
                        {

                            //SCI_TRACE_LOW:"HandleFmmRenameWinMsg: file_is_power_on_anim = %d"
                            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_6601_112_2_18_2_20_48_447, (uint8*) "d", file_is_power_on_anim);
                            //SCI_TRACE_LOW:"HandleFmmRenameWinMsg: file_is_power_off_anim = %d"
                            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_6602_112_2_18_2_20_48_448, (uint8*) "d", file_is_power_off_anim);
#ifdef SCREENSAVER_SUPPORT
#ifdef VIDEO_PLAYER_SUPPORT

                            MMIAPISET_GetScreensaverInfo (&ssaver_info);
                            file_is_ssaver = CheckIsScreensaverInfo (&ssaver_info, rename_param_ptr->rename_papam->full_path_ptr, old_name_len);
                            //SCI_TRACE_LOW:"HandleFmmRenameWinMsg: file_is_ssaver = %d"
                            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_6608_112_2_18_2_20_48_449, (uint8*) "d", file_is_ssaver);

                            if (file_is_ssaver) //rename file is ssaver
                            {
                                ssaver_info.more_ani_saver_info.name_wstr_len = (uint16) MIN (new_name_len, MMISET_SCREENSAVER_NAME_MAX_LEN);

                                SCI_MEMSET (ssaver_info.more_ani_saver_info.name_wstr, 0, sizeof (ssaver_info.more_ani_saver_info.name_wstr));

                                MMIAPICOM_Wstrncpy (ssaver_info.more_ani_saver_info.name_wstr, new_name_ptr, ssaver_info.more_ani_saver_info.name_wstr_len);

                                ssaver_info.is_open     = TRUE;
                                ssaver_info.ssaver_type = MMISET_SCREENSAVER_MORE_ANIMATION;
                                MMIAPISET_SetScreenSaverInfo (&ssaver_info);
                            }

#endif
#endif

                            if (file_is_power_on_anim) //rename file is power on anim
                            {
                                power_on_anim_info.more_anim_info.name_wstr_len = (uint16) MIN (new_name_len, MMISET_POWER_ON_OFF_ANIM_NAME_MAX_LEN);

                                SCI_MEMSET (power_on_anim_info.more_anim_info.name_wstr, 0, sizeof (power_on_anim_info.more_anim_info.name_wstr));

                                MMIAPICOM_Wstrncpy (power_on_anim_info.more_anim_info.name_wstr,
                                                    new_name_ptr, power_on_anim_info.more_anim_info.name_wstr_len);

                                power_on_anim_info.is_open = TRUE;
                                power_on_anim_info.anim_type = MMISET_POWER_ON_OFF_ANIM_MORE;
                                MMIAPISET_SetPowerOnOffAnimInfo (&power_on_anim_info, TRUE);
                            }

                            if (file_is_power_off_anim) //rename file is power off anim
                            {
                                power_off_anim_info.more_anim_info.name_wstr_len = (uint16) MIN (new_name_len, MMISET_POWER_ON_OFF_ANIM_NAME_MAX_LEN);
                                SCI_MEMSET (power_off_anim_info.more_anim_info.name_wstr, 0, sizeof (power_off_anim_info.more_anim_info.name_wstr));

                                MMIAPICOM_Wstrncpy (power_off_anim_info.more_anim_info.name_wstr,
                                                    new_name_ptr, power_on_anim_info.more_anim_info.name_wstr_len);
                                power_off_anim_info.is_open = TRUE;
                                power_off_anim_info.anim_type = MMISET_POWER_ON_OFF_ANIM_MORE;
                                MMIAPISET_SetPowerOnOffAnimInfo (&power_off_anim_info, FALSE);
                            }

                        }
                        else  if (MMIFMM_FILE_TYPE_PICTURE == rename_param_ptr->rename_papam->type)
                        {
#ifdef SCREENSAVER_SUPPORT
                            MMIAPISET_GetScreensaverInfo (&ssaver_info);
                            file_is_ssaver = CheckIsScreensaverInfo (&ssaver_info, rename_param_ptr->rename_papam->full_path_ptr, old_name_len);
                            //SCI_TRACE_LOW:"HandleFmmRenameWinMsg: file_is_ssaver = %d"
                            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_6656_112_2_18_2_20_48_450, (uint8*) "d", file_is_ssaver);

                            if (file_is_ssaver) //rename file is ssaver
                            {
                                ssaver_info.more_pic_saver_info.name_wstr_len = (uint16) MIN (new_name_len, MMISET_SCREENSAVER_NAME_MAX_LEN);

                                SCI_MEMSET (ssaver_info.more_pic_saver_info.name_wstr, 0, sizeof (ssaver_info.more_pic_saver_info.name_wstr));

                                MMIAPICOM_Wstrncpy (ssaver_info.more_pic_saver_info.name_wstr, new_name_ptr, ssaver_info.more_pic_saver_info.name_wstr_len);

                                ssaver_info.is_open     = TRUE;
                                ssaver_info.ssaver_type = MMISET_SCREENSAVER_MORE_PICTURE;
                                MMIAPISET_SetScreenSaverInfo (&ssaver_info);
                            }

#endif

                            //如果重命名的文件正好是墙纸图片，则修改墙纸设置
#if defined MMI_WALLPAPER_SUPPORT
                            if (MMIAPIFMM_IsWallPaperFile (rename_param_ptr->rename_papam->full_path_ptr, old_name_len))
                            {
                                MMIAPIFMM_ChangeWallPaperFileName (new_name_ptr, new_name_len);
                                //update wallpaper
                                MMIAPIIDLE_SetWallpaper (TRUE);
                            }
#endif
                            if (file_is_power_on_anim) //rename file is power on anim
                            {
                                power_on_anim_info.more_anim_info.name_wstr_len = (uint16) MIN (new_name_len, MMISET_POWER_ON_OFF_ANIM_NAME_MAX_LEN);

                                SCI_MEMSET (power_on_anim_info.more_anim_info.name_wstr, 0, sizeof (power_on_anim_info.more_anim_info.name_wstr));

                                MMIAPICOM_Wstrncpy (power_on_anim_info.more_anim_info.name_wstr,
                                                    new_name_ptr, power_on_anim_info.more_anim_info.name_wstr_len);

                                power_on_anim_info.is_open = TRUE;
                                power_on_anim_info.anim_type = MMISET_POWER_ON_OFF_PIC_MORE;
                                MMIAPISET_SetPowerOnOffAnimInfo (&power_on_anim_info, TRUE);
                            }

                            if (file_is_power_off_anim) //rename file is power off anim
                            {
                                power_off_anim_info.more_anim_info.name_wstr_len = (uint16) MIN (new_name_len, MMISET_POWER_ON_OFF_ANIM_NAME_MAX_LEN);
                                SCI_MEMSET (power_off_anim_info.more_anim_info.name_wstr, 0, sizeof (power_off_anim_info.more_anim_info.name_wstr));

                                MMIAPICOM_Wstrncpy (power_off_anim_info.more_anim_info.name_wstr,
                                                    new_name_ptr, power_on_anim_info.more_anim_info.name_wstr_len);
                                power_off_anim_info.is_open = TRUE;
                                power_off_anim_info.anim_type = MMISET_POWER_ON_OFF_PIC_MORE;
                                MMIAPISET_SetPowerOnOffAnimInfo (&power_off_anim_info, FALSE);
                            }

                            //delete picture miniature
                            GUIANIM_DeleteMiniatureFile (rename_param_ptr->rename_papam->full_path_ptr, old_name_len);
                        }
                        else  if (MMIFMM_FILE_TYPE_MUSIC == rename_param_ptr->rename_papam->type)
                        {
                            MMISET_CALL_MORE_RING_T     call_ring_info = {0};
                            MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_1;


                            MMI_WSTRNCPY (call_ring_info.name_wstr, MMISET_CALL_RING_NAME_MAX_LEN,
                                          new_name_ptr, new_name_len, new_name_len);
                            call_ring_info.name_wstr_len = (uint16) MIN (new_name_len, MMISET_CALL_RING_NAME_MAX_LEN);

                            for (dual_sys = MN_DUAL_SYS_1; dual_sys < MMI_DUAL_SYS_MAX; dual_sys++)
                            {
                                //任意一个sim card 的操作失败即提示失败??
                                if (file_is_call_info[dual_sys])
                                {
                                    if (!MMIAPIENVSET_SetMoreCallRingInfo (dual_sys, call_ring_info, MMIAPIENVSET_GetActiveModeId()))
                                    {
                                        is_success = FALSE;
                                        break;
                                    }
                                }
                            }
                        }

                        if (is_success)
                        {
                            MMIFMM_RENAME_RETURN_T rename_return = {0};
                            MMIAPICOM_Wstrncpy (rename_return.file_name_ptr, new_name_ptr, new_name_len);
                            rename_return.file_name_len = new_name_len;
                            MMK_SendMsg ( (MMI_WIN_ID_T) rename_param_ptr->rename_papam->win_id, (MMI_MESSAGE_ID_E) rename_param_ptr->rename_papam->msg_id, (void*) &rename_return);
                            MMK_SendMsg((MMI_WIN_ID_T) MMIFMM_UDISK_CHILD_WIN_ID,MSG_FMM_RENAME_END_CNF, (void*) &rename_return);//CR171976
							SCI_TRACE_BUF(" mmifmm_renamewin.c rename_return = ", rename_return.file_name_ptr,rename_return.file_name_len);
							MMK_CloseWin (win_id);
                        }
                        else
                        {
                            MMIPUB_OpenAlertFailWin (TXT_ERROR);
                        }
                    }
                    else
                    {
                        if (SFS_ERROR_HAS_EXIST == rename_result)
                        {
                            MMIPUB_OpenAlertWinByTextId (PNULL, TXT_FMM_NAME_DUPLICATE, TXT_NULL, IMAGE_PUBWIN_WARNING , PNULL, PNULL, MMIPUB_SOFTKEY_ONE, PNULL);
                        }
                        else
                        {
                            MMIFMM_PromptFileErrorType (PNULL, rename_result, filedev, TRUE);
							//某些需要关闭rename窗口的错误类型 bug71235
							if (SFS_ERROR_ACCESS == rename_result)
							{
								MMK_CloseWin(win_id);
							}
                        }
                    }
                }
            }
        }
        else
        {
            MMIFMM_PromptFileErrorType (PNULL, SFS_ERROR_INVALID_PARAM, filedev, TRUE);
        }

        break;

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin (win_id);
        break;
#if defined TOUCH_PANEL_SUPPORT
    case MSG_CTL_PENOK:
#ifdef MMI_PDA_SUPPORT

        if (PNULL != param)
        {
            uint32 src_id = ( (MMI_NOTIFY_T*) param)->src_id;

            switch (src_id)
            {
            case MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID:
                MMK_SendMsg (win_id, MSG_APP_OK, PNULL);
                break;
            case MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID:
                MMK_CloseWin (win_id);
                break;
            default:
                break;
            }
        }

#endif
        break;
#endif

    case MSG_CLOSE_WINDOW:
        RenameCloseCtrlFunc (win_id);
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }

    return (result);
}

/*****************************************************************************/
// Description : open ctrl func
// Global resource dependence :
// Author:  jian.ma
// Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E RenameOpenCtrlFunc (MMI_WIN_ID_T win_id)
{
    MMIFMM_RENAME_WIN_PARAM_T *detail_data = (MMIFMM_RENAME_WIN_PARAM_T*) MMK_GetWinAddDataPtr (win_id);
    MMIFMM_RENAMEWIN_INFO_T *win_data_p = PNULL;

    if (detail_data == PNULL)
    {
        //MMIFMM_RaiseError(1);
        return MMI_RESULT_FALSE;
    }

    win_data_p = (MMIFMM_RENAMEWIN_INFO_T*) SCI_ALLOC_APPZ (sizeof (MMIFMM_RENAMEWIN_INFO_T));

    if (win_data_p == PNULL)
    {
        //MMIFMM_RaiseError(1);
        return MMI_RESULT_FALSE;
    }

    win_data_p->rename_papam = detail_data;
    win_data_p->rename_buffer_ptr = (wchar *) SCI_ALLOC_APPZ (sizeof (wchar) * (MMIFMM_FULL_FILENAME_LEN + 1) * 4);
    win_data_p->file_name_ext_len = 0;
    win_data_p->max_file_name_len = 0;
    MMK_SetWinUserData (win_id, (void *) win_data_p);

    //MMICL_RegisterDateChangeEvent(win_id, MMICL_CALLTIMELIST_WIN_FLAG);
    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
// Description : close ctrl func
// Global resource dependence :
// Author:  jian.ma
// Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E RenameCloseCtrlFunc (MMI_WIN_ID_T win_id)
{

    MMIFMM_RENAMEWIN_INFO_T *win_data_p = (MMIFMM_RENAMEWIN_INFO_T*) MMK_GetWinUserData (win_id);

    if (PNULL != win_data_p)
    {
        SCI_FREE (win_data_p->rename_papam->full_path_ptr);
        SCI_FREE (win_data_p->rename_papam);
        SCI_FREE (win_data_p->rename_buffer_ptr);
        SCI_FREE (win_data_p);
    }

    //MMICL_UnRegisterDateChangeEvent(MMICL_CALLTIMELIST_WIN_FLAG);

    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
// Description : 目录改名，拆分目录名字
// Global resource dependence :
// Author: Ming.Song 2007-4-19
// Note:
/*****************************************************************************/
LOCAL BOOLEAN FmmRenameSplitFolder (
    const wchar   *full_path_ptr,             //in 全路径: "d:\abc\def\mp3"
    wchar   *before_the_last_path_ptr,  //out 除去最后一级目录的全路径加"\": "d:\abc\def\"
    wchar   *the_last_path_ptr,         //out 最后一级目录，即要改的目录名字: "mp3"
    uint32   *max_folder_name_len        //out 目录名字可以改到的最大长度，ucs2 len
)
{
    BOOLEAN     result = TRUE;
    uint16      string_len = 0;
    uint16      i = 0;

    //SCI_ASSERT(NULL != full_path_ptr);
    if (PNULL == full_path_ptr)
    {
        //SCI_TRACE_LOW:"MMIFMM:[FmmRenameSplitFolder]PNULL == full_path_ptr"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_6947_112_2_18_2_20_49_455, (uint8*) "");
        return FALSE;
    }

    string_len = (uint16) MMIAPICOM_Wstrlen (full_path_ptr);

    if (MMIFMM_FULL_FILENAME_LEN < string_len)
    {
        //SCI_TRACE_LOW:"FmmRenameSplitFolder string_len =%d"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_6954_112_2_18_2_20_49_456, (uint8*) "d", string_len);
        return FALSE;
    }

    i = MMIFMM_GetFinallySlashIndex (full_path_ptr, (string_len - 1));
//    for(i=string_len-1;MMIFILE_SLASH!=full_path_ptr[i] && i>0;i--);

    SCI_MEMSET (before_the_last_path_ptr, 0, ( (MMIFMM_FULL_FILENAME_LEN + 1) *sizeof (wchar)));
    SCI_MEMSET (the_last_path_ptr, 0, ( (MMIFMM_FULL_FILENAME_LEN + 1) *sizeof (wchar)));
    MMIAPICOM_Wstrncpy (before_the_last_path_ptr, full_path_ptr, i + 1);
    MMIAPICOM_Wstrncpy (the_last_path_ptr, &full_path_ptr[i+1], string_len - 1 - i);
    *max_folder_name_len = (uint16) (MMIFMM_FULL_FILENAME_LEN - i - 1);

    return (result);
}

/*****************************************************************************/
// Description : 文件改名，拆分文件名字
// Global resource dependence :
// Author: Ming.Song 2007-4-19
// Note:
/*****************************************************************************/
LOCAL BOOLEAN FmmRenameSplitFile (
    const wchar *full_path_ptr,                 //in 全路径: "d:\abc\def\mp3\a.mp3"
    wchar *the_path_ptr,                  //out 路径加"\": "d:\abc\def\mp3\"
    wchar *file_name_without_ext,         //out 要改的文件名字，不包括扩展名: "a"
    uint32 *max_file_name_without_ext_len, //out ucs2 len
    wchar *file_name_ext,                 //out "."加扩展名: ".mp3"
    uint32 *file_name_ext_len              //out "."加扩展名长度，ucs2 len
)
{
    BOOLEAN     result = TRUE;
    uint16      string_len = 0;
    int16       i = 0;
    int16       j = 0;

    //SCI_ASSERT(NULL != full_path_ptr);
    if (PNULL == full_path_ptr)
    {
        //SCI_TRACE_LOW:"MMIFMM:[FmmRenameSplitFile]PNULL == full_path_ptr"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_6991_112_2_18_2_20_49_457, (uint8*) "");
        return FALSE;
    }

    string_len = (uint16) MMIAPICOM_Wstrlen (full_path_ptr);

    if (MMIFMM_FULL_FILENAME_LEN < string_len)
    {
        //SCI_TRACE_LOW:"FmmRenameSplitFile string_len =%d"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_6998_112_2_18_2_20_49_458, (uint8*) "d", string_len);
        return FALSE;
    }

    for (i = (uint16) (string_len - 1); i >= 0; i--)
    {
        if (MMIFILE_SLASH == full_path_ptr[i])
        {
            break;
        }
    }

    for (j = (uint16) (string_len - 1); j >= 0; j--)
    {
        if (MMIFILE_DOT == full_path_ptr[j])
        {
            break;
        }
    }

    SCI_MEMSET (the_path_ptr, 0, ( (MMIFMM_FULL_FILENAME_LEN + 1) *sizeof (wchar)));
    SCI_MEMSET (file_name_without_ext, 0, ( (MMIFMM_FULL_FILENAME_LEN + 1) *sizeof (wchar)));
    SCI_MEMSET (file_name_ext, 0, ( (MMIFMM_FULL_FILENAME_LEN + 1) *sizeof (wchar)));
    MMIAPICOM_Wstrncpy (the_path_ptr, full_path_ptr, i + 1);

    if ( (i < j) && (j != (string_len - 1)))
    {
        MMIAPICOM_Wstrncpy (file_name_without_ext, &full_path_ptr[i+1], j - 1 - i);
        *max_file_name_without_ext_len = (uint16) (MMIFMM_FULL_FILENAME_LEN - i - 1 - string_len + j);
        MMIAPICOM_Wstrncpy (file_name_ext, &full_path_ptr[j], string_len - j);
        *file_name_ext_len = (uint16) (string_len - j);
    }
    else
    {
        MMIAPICOM_Wstrncpy (file_name_without_ext, &full_path_ptr[i+1], string_len - 1 - i);
        *max_file_name_without_ext_len = (uint16) (MMIFMM_FULL_FILENAME_LEN - i - 1);
        *file_name_ext_len = 0;
    }

    return (result);
}

#ifdef SCREENSAVER_SUPPORT
/*****************************************************************************/
// Description : check is screen saver info
// Global resource dependence :
// Author: jian.ma
// Note:
/*****************************************************************************/
LOCAL BOOLEAN CheckIsScreensaverInfo (MMISET_SCREENSAVER_INFO_T *ssaver_info, const wchar *full_path_ptr, uint16 old_name_len)
{

    BOOLEAN result = FALSE;

    if (PNULL != ssaver_info && PNULL != full_path_ptr && 0 != old_name_len)
    {
        //SCI_TRACE_LOW:"CheckIsScreensaverInfo: ssaver_info->more_pic_saver_info.name_len = %d"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_6833_112_2_18_2_20_48_451, (uint8*) "d", ssaver_info->more_pic_saver_info.name_wstr_len);
        //SCI_TRACE_LOW:"CheckIsScreensaverInfo: old_name_len = %d"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_6834_112_2_18_2_20_48_452, (uint8*) "d", old_name_len);

        if (ssaver_info->is_open)
        {
            if (MMISET_SCREENSAVER_MORE_PICTURE == ssaver_info->ssaver_type
                    && 0 < ssaver_info->more_pic_saver_info.name_wstr_len
                    && (0 == MMIAPICOM_CompareTwoWstr (full_path_ptr, old_name_len, ssaver_info->more_pic_saver_info.name_wstr, ssaver_info->more_pic_saver_info.name_wstr_len)))

            {
                result = TRUE;
            }

#ifdef VIDEO_PLAYER_SUPPORT
            else if (MMISET_SCREENSAVER_MORE_ANIMATION == ssaver_info->ssaver_type && 0 < ssaver_info->more_ani_saver_info.name_wstr_len && (0 == MMIAPICOM_CompareTwoWstr (full_path_ptr, old_name_len,
                     ssaver_info->more_ani_saver_info.name_wstr, ssaver_info->more_ani_saver_info.name_wstr_len)))
            {
                result = TRUE;
            }

#endif
        }
    }

    return result;

}
#endif

/*****************************************************************************/
// Description : check is call ring info
// Global resource dependence :
// Author: jian.ma
// Note:
/*****************************************************************************/
LOCAL BOOLEAN CheckIsCallringInfo (
    BOOLEAN *is_call_ring_info,
    const wchar  *full_path_ptr,
    uint16  old_name_len
)
{
    BOOLEAN result = FALSE;
    MMISET_CALL_RING_T          ring_info = {MMISET_CALL_RING_FIXED, 0, 0};
    int32   i = 0;

    if (PNULL != is_call_ring_info && PNULL != full_path_ptr && 0 != old_name_len)
    {

        for (i = 0; i < MMI_DUAL_SYS_MAX; i++)
        {
            ring_info = MMIAPISET_GetCallRingInfo ( (MN_DUAL_SYS_E) i, MMIAPIENVSET_GetActiveModeId());

            if (MMISET_CALL_RING_MORE_RING == ring_info.call_ring_type
                    && 0 < ring_info.more_ring_info.name_wstr_len
                    && 0 == MMIAPICOM_CompareTwoWstr (full_path_ptr, old_name_len,/*lint !e506 */
                                                      ring_info.more_ring_info.name_wstr,
                                                      ring_info.more_ring_info.name_wstr_len)
               )
            {
                is_call_ring_info[i] = TRUE;
            }
        }

    }

    return result;
}



/*****************************************************************************/
// Description : check is power on anim info
// Global resource dependence :
// Author: jian.ma
// Note:
/*****************************************************************************/
LOCAL BOOLEAN CheckIsPowerOnOffAnimInfo (
    MMISET_POWER_ON_OFF_ANIM_INFO_T *power_onoff_anim_info,
    const wchar *full_path_ptr,
    uint16 old_name_len
)
{

    BOOLEAN result = FALSE;
    BOOLEAN is_more = FALSE;

    //is more power on off pic or anim
	if (PNULL != power_onoff_anim_info)//coverity 12699
	{
		is_more = (MMISET_POWER_ON_OFF_ANIM_MORE == power_onoff_anim_info->anim_type ||
				   MMISET_POWER_ON_OFF_PIC_MORE == power_onoff_anim_info->anim_type);

		if (PNULL != full_path_ptr && 0 != old_name_len)
		{
			//SCI_TRACE_LOW:"CheckIsPowerOnOffAnimInfo: power_onoff_anim_info->more_anim_info.name_len = %d"
			SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_6879_112_2_18_2_20_49_453, (uint8*) "d", power_onoff_anim_info->more_anim_info.name_wstr_len);
			//SCI_TRACE_LOW:"CheckIsPowerOnOffAnimInfo: old_name_len = %d"
			SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_6880_112_2_18_2_20_49_454, (uint8*) "d", old_name_len);

			if (power_onoff_anim_info->is_open &&
					is_more  &&
					0 < power_onoff_anim_info->more_anim_info.name_wstr_len &&
					(0 == MMIAPICOM_CompareTwoWstr (full_path_ptr, old_name_len, power_onoff_anim_info->more_anim_info.name_wstr,
													power_onoff_anim_info->more_anim_info.name_wstr_len))) /*lint !e506 */
			{
				result = TRUE;
			}
		}
	}

    return result;
}

