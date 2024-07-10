/*****************************************************************************
** File Name:      mmifmm_createwin.c                                        *
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

#define _MMIFMM_CREATEWIN_C_


/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_app_fmm_trc.h"
#include "mmifmm_createwin.h"
#include "window_parse.h"
#include "mmifmm_id.h"
#include "mmifmm_text.h"
#include "guiform.h"
#include "mmi_filemgr.h"
#include "mmifmm_export.h"
#include "mmifmm_comfunc.h"
#include "mmipub.h"
#include "mmi_common.h"
#include "mmi_nv.h"
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         TYPE DEFINITION                                  *
 **--------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
// Description : 创建目录窗口
// Global resource dependence :
// Author:
// Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleCreateFolderWinMsg (
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
LOCAL MMI_RESULT_E CreateFolderWinOpenCtrlFunc (MMI_WIN_ID_T win_id);

/*****************************************************************************/
// Description : close ctrl func
// Global resource dependence :
// Author:  jian.ma
// Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E CreateFolderWinCloseCtrlFunc (MMI_WIN_ID_T win_id);

//create folder win
WINDOW_TABLE (MMIFMM_CREATE_FOLDER_WIN_TAB) =
{
    WIN_FUNC ( (uint32) HandleCreateFolderWinMsg),
    WIN_ID (MMIFMM_CREATE_FOLDER_WIN_ID),
    WIN_TITLE (TXT_FMM_NEW_FOLDER),
#ifdef MMI_PDA_SUPPORT
    CREATE_FORM_CTRL (GUIFORM_LAYOUT_ORDER, MMIFMM_CREATE_FOLDER_FORM_CTRL_ID),
    CHILD_EDIT_TEXT_CTRL (TRUE, 0, MMIFMM_CREATE_FOLDER_EDITBOX_CTRL_ID, MMIFMM_CREATE_FOLDER_FORM_CTRL_ID),
    CHILD_SOFTKEY_CTRL (TXT_COMMON_OK, TXT_NULL, STXT_RETURN, MMIFMM_CREATE_FOLDER_SOFTKEY_CTRL_ID, MMIFMM_CREATE_FOLDER_FORM_CTRL_ID),
#else
    WIN_SOFTKEY (TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_EDIT_TEXT_CTRL (0, MMIFMM_CREATE_FOLDER_EDITBOX_CTRL_ID),
#endif
    END_WIN
};
/*****************************************************************************/
// Description : 创建目录窗口
// Global resource dependence :
// Author:
// Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleCreateFolderWinMsg (
    MMI_WIN_ID_T         win_id,
    MMI_MESSAGE_ID_E     msg_id,
    DPARAM               param
)
{
    MMI_RESULT_E                    result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T                   ctrl_id = MMIFMM_CREATE_FOLDER_EDITBOX_CTRL_ID;
    MMI_STRING_T                    name_str = {0};
    MMIFMM_CREATEFOLDER_WIN_PARAM_T *create_folder_param = PNULL;
    MMI_STRING_T                    string = {0};
    wchar                           new_folder_name[MMIFMM_FULL_FILENAME_LEN+1] = {0};
    wchar                           new_folder_full_path_name[MMIFMM_FULL_FILENAME_LEN+1] = {0};
    uint16                          i = 0;
    uint8                           init_num[5] = {0};
    uint16                          string_len = 0;
    uint16                          num_len = 0;
    uint16                          path_len = 0;
    uint16                          full_path_len = 0;
    MMIFILE_DEVICE_E                filedev = MMI_DEVICE_UDISK;
    MMI_WIN_ID_T                    alert_win_id = MMIFMM_MEM_FULL_ALERT_WIN;

    create_folder_param = (MMIFMM_CREATEFOLDER_WIN_PARAM_T*) MMK_GetWinUserData (win_id);

    //UNUSED_PARAM(param);
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
#ifdef MID_SOFTKEY_USE_IMG_SUPPORT
		GUIWIN_SeSoftkeytButtonIconId(win_id, IMAGE_NULL, 1, TRUE);
#endif
        if (MMI_RESULT_FALSE == CreateFolderWinOpenCtrlFunc (win_id))
        {
            MMK_CloseWin (win_id);
            break;
        }

        create_folder_param = (MMIFMM_CREATEFOLDER_WIN_PARAM_T*) MMK_GetWinUserData (win_id);
        //set left softkey
        GUIEDIT_SetSoftkey (ctrl_id, 0, 1, TXT_NULL, TXT_COMMON_OK, PNULL);

        GUIEDIT_SetTextMaxLen (ctrl_id, (uint16) create_folder_param->new_folder_max_len, (uint16) create_folder_param->new_folder_max_len);

        MMI_GetLabelTextByLang (TXT_FMM_NEW_FOLDER, &string);

        if (create_folder_param->new_folder_max_len <= string.wstr_len)
        {
            // 如果文件夹名的长度小于 " 新文件夹"字符的长度，没有默认名
            MMK_SetAtvCtrl (win_id, ctrl_id);
            break;
        }

        for (i = 1; i <= MMIFMM_FILE_NUM; i++)
        {
            sprintf ( (char*) init_num, " %d", i);
            num_len = (uint16) strlen ( (char*) init_num);

            SCI_MEMSET (new_folder_name, 0, ( (MMIFMM_FULL_FILENAME_LEN + 1) *sizeof (wchar)));
            string_len = string.wstr_len;

            MMI_WSTRNCPY (new_folder_name, MMIFMM_FULL_FILENAME_LEN,
                          string.wstr_ptr, string.wstr_len, string.wstr_len);

            MMI_STRNTOWSTR (&new_folder_name[string_len], MMIFMM_FULL_FILENAME_LEN - string_len,
                            init_num, num_len, num_len);

            path_len = (uint16) MMIAPICOM_Wstrlen (create_folder_param->full_path_ptr);

            SCI_MEMSET (new_folder_full_path_name, 0, ( (MMIFMM_FULL_FILENAME_LEN + 1) *sizeof (wchar)));
            full_path_len = (uint16) MMIFMM_CombineToFileName (create_folder_param->full_path_ptr, (uint16) path_len, new_folder_name, (uint16) (string_len + num_len),
                            new_folder_full_path_name, (uint16) MMIFMM_FULL_FILENAME_LEN);

            if (!MMIAPIFMM_IsFolderExist (new_folder_full_path_name, (uint16) full_path_len))
            {
                break;
            }
        }

        GUIEDIT_SetString (ctrl_id, new_folder_name, (uint16) (string_len + num_len));
//		GUIEDIT_SetHandleRedKey(FALSE, ctrl_id);
        MMK_SetAtvCtrl (win_id, ctrl_id);

        break;

#if defined MMI_PDA_SUPPORT && defined TOUCH_PANEL_SUPPORT
    case MSG_CTL_PENOK:

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

        break;
#endif

    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_CTL_MIDSK:
        GUIEDIT_GetString (ctrl_id, &name_str);

        if (0 == name_str.wstr_len)
        {
            break;
        }

        filedev = MMIAPIFMM_GetDeviceTypeByPath (create_folder_param->full_path_ptr, MMIFILE_DEVICE_LEN);

        //目录名字是否有效
        if (MMIAPIFMM_CheckMemoryCard (filedev))
        {
            if (MMIFMM_NewFolderNameStringIsValid (&name_str, create_folder_param->new_folder_max_len))
            {
                string_len = (uint16) MIN (name_str.wstr_len, MMIFMM_FULL_FILENAME_LEN);
                MMIAPICOM_Wstrncpy (new_folder_name, name_str.wstr_ptr, string_len);
                new_folder_name[string_len] = 0;
                MMIFMM_CorrectFolderName (new_folder_name);
                string_len = (uint16) MMIAPICOM_Wstrlen (new_folder_name);
                path_len = (uint16) MMIAPICOM_Wstrlen (create_folder_param->full_path_ptr);
                SCI_MEMSET (new_folder_full_path_name, 0, ( (MMIFMM_FULL_FILENAME_LEN + 1) *sizeof (wchar)));
                MMI_WSTRNCPY (new_folder_full_path_name, MMIFMM_FULL_FILENAME_LEN,
                              create_folder_param->full_path_ptr, path_len, path_len);
                new_folder_full_path_name[path_len] = MMIFILE_SLASH;

                if (!MMIAPICOM_GetValidFileName (TRUE, new_folder_name, string_len,
                                                 &new_folder_full_path_name[path_len+1], (MMIFMM_FULL_FILENAME_LEN - path_len - 1)))
                {
                    MMIPUB_OpenAlertFailWin (TXT_DC_NAMEINVALID);
                    break;
                }

                *create_folder_param->new_folder_len_ptr = (uint16) MMIAPICOM_Wstrlen (&new_folder_full_path_name[path_len+1]);
                MMI_WSTRNCPY (create_folder_param->new_folder_ptr, *create_folder_param->new_folder_len_ptr,
                              &new_folder_full_path_name[path_len+1], *create_folder_param->new_folder_len_ptr, *create_folder_param->new_folder_len_ptr);

                full_path_len = (uint16) MMIAPICOM_Wstrlen (new_folder_full_path_name);

                if (!MMIAPIFMM_IsFolderExist (new_folder_full_path_name, full_path_len))
                {
                    SFS_ERROR_E  create_result = SFS_NO_ERROR;
                    create_result = SFS_CreateDirectory (new_folder_full_path_name);
                    //SCI_TRACE_LOW:"HandleCreateFolderWinMsg: create_result = %d"
                    SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_6255_112_2_18_2_20_47_439, (uint8*) "d", create_result);

                    MMIFMM_PromptFileErrorType (alert_win_id, create_result, filedev, FALSE);

                    if (SFS_ERROR_NONE == create_result)
                    {
                        //MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
                        MMK_PostMsg ( (MMI_WIN_ID_T) create_folder_param->win_id, (MMI_MESSAGE_ID_E) create_folder_param->msg_id, PNULL, 0);
                        MMK_CloseWin (win_id);
                    }
                    else if (SFS_ERROR_INVALID_PARAM == create_result)
                    {
                        //name invalid
                        MMIPUB_OpenAlertFailWin (TXT_DC_NAMEINVALID);
                    }
                }
                else
                {
                    MMIFMM_PromptFileErrorType (alert_win_id, SFS_ERROR_HAS_EXIST, filedev, FALSE);
                }
            }
            else
            {
                MMIFMM_PromptFileErrorType (alert_win_id, SFS_ERROR_INVALID_PARAM, filedev, FALSE);
            }
        }

        break;

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin (win_id);
        break;

    case MSG_CLOSE_WINDOW:
        CreateFolderWinCloseCtrlFunc (win_id);
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }

    return (result);
}


/*****************************************************************************/
// Description : create folder win
// Global resource dependence :
// Author: jian.ma
// RETRUN:
// Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIFMM_CreateFolderWin (MMIFMM_CREATEFOLDER_WIN_PARAM_T * folder_win_param)
{

    MMI_RESULT_E                     result = MMI_RESULT_FALSE;
    MMI_HANDLE_T                win_handle = 0;
    MMIFMM_CREATEFOLDER_WIN_PARAM_T * param_to_createfolder_win = PNULL;

    if (PNULL == folder_win_param)
    {
        return MMI_RESULT_FALSE;
    }

    if (MMK_IsOpenWin (MMIFMM_CREATE_FOLDER_WIN_ID))
    {
        MMK_CloseWin (MMIFMM_CREATE_FOLDER_WIN_ID);
    }

    param_to_createfolder_win = (MMIFMM_CREATEFOLDER_WIN_PARAM_T *) SCI_ALLOC_APP (sizeof (MMIFMM_CREATEFOLDER_WIN_PARAM_T));
    param_to_createfolder_win->full_path_ptr = folder_win_param->full_path_ptr;
    param_to_createfolder_win->new_folder_max_len = folder_win_param->new_folder_max_len;
    param_to_createfolder_win->new_folder_len_ptr = folder_win_param->new_folder_len_ptr;
    param_to_createfolder_win->new_folder_ptr = folder_win_param->new_folder_ptr;
    param_to_createfolder_win->win_id = folder_win_param->win_id;
    param_to_createfolder_win->msg_id = folder_win_param->msg_id;
    win_handle = MMK_CreateWin ( (uint32 *) MMIFMM_CREATE_FOLDER_WIN_TAB , (ADD_DATA) param_to_createfolder_win);

    if (!win_handle || (MMI_INVALID_ID == win_handle))
    {
        SCI_FREE (param_to_createfolder_win);
        result =  MMI_RESULT_FALSE;
    }
    else
    {
        result = MMI_RESULT_TRUE;
    }

    return result;
}


/*****************************************************************************/
// Description : open ctrl func
// Global resource dependence :
// Author:  jian.ma
// Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E CreateFolderWinOpenCtrlFunc (MMI_WIN_ID_T win_id)
{
    MMIFMM_CREATEFOLDER_WIN_PARAM_T *win_data_p = (MMIFMM_CREATEFOLDER_WIN_PARAM_T*) MMK_GetWinAddDataPtr (win_id);

    MMK_SetWinUserData (win_id, (void *) win_data_p);

    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
// Description : close ctrl func
// Global resource dependence :
// Author:  jian.ma
// Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E CreateFolderWinCloseCtrlFunc (MMI_WIN_ID_T win_id)
{

    MMIFMM_CREATEFOLDER_WIN_PARAM_T *win_data_p = (MMIFMM_CREATEFOLDER_WIN_PARAM_T*) MMK_GetWinUserData (win_id);

    SCI_FREE (win_data_p);

    return MMI_RESULT_TRUE;
}
