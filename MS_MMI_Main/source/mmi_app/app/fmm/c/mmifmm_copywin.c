/*****************************************************************************
** File Name:      mmifmm_copywin.c                                          *
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

#define _MMIFMM_COPYWIN_C_


/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_app_fmm_trc.h"
#include "mmifmm_copywin.h"
#include "mmifmm_id.h"
#include "mmifmm_text.h"
#include "mmi_filetask.h"
#include "mmifmm_comwin.h"
#include "mmicc_export.h"
#include "mmifmm_comwin.h"
#include "mmifmm_movewin.h"
#include "mmifmm_comfunc.h"
#ifdef DRM_SUPPORT
#include "mmidrm_export.h"
#endif
#include "mmifmm_app.h"
#include "mmi_appmsg.h"
#include "window_parse.h"
#include "mmi_image.h"
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         TYPE DEFINITION                                  *
 **--------------------------------------------------------------------------*/
//复制文件夹状态控制
//typedef enum
//{
//     MMIFMM_COPYDEL_NONE,                //初始状态
//     //MMIFMM_COPYDEL_QUERY,               //询问是否覆盖所有
//     //MMIFMM_COPYDEL_OK,                  //确认覆盖所有
//     MMIFMM_COPYDEL_CANCEL             //取消覆盖
//     //MMIFMM_COPYDEL_RENAME              //需要重命名
//} MMIFMM_COPYDEL_OPT_STATE_E;

typedef enum
{
    MMIFMM_OPT_COPY,
    MMIFMM_OPT_DEL
} MMIFMM_COPY_DEL_TYPE_E;

typedef struct
{
    MMIFMM_DATA_INFO_T* list_data_ptr;
    MMIFMM_PATH_INFO_T* current_path_ptr;
    MMIFMM_COPY_DEL_PATH_DATA_INFO_T * s_copy_del_path_data_ptr;
    APP_COPY_HANDLE_T*      s_copy_handle_ptr;
    BOOLEAN                 s_need_update;
    uint32                  s_copydel_opt_state;
    BOOLEAN                 s_user_stop_copy;
#ifdef MMI_FMM_MOVE_SUPPORT
    BOOLEAN                 s_is_copy_from_move;
    APP_DELETE_HANDLE_T*    s_delete_handle_ptr;
#endif
} MMIFMM_COPYWIN_INFO_T;

/**-------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/
//LOCAL MMIFMM_COPY_DEL_PATH_DATA_INFO_T  *s_copy_del_path_data_ptr= PNULL;
//LOCAL APP_COPY_HANDLE_T *s_copy_handle_ptr = PNULL;

//LOCAL MMIFMM_COPYDEL_OPT_STATE_E s_copydel_opt_state = MMIFMM_COPYDEL_NONE;  //copy and del used
//LOCAL BOOLEAN               s_user_stop_copy = FALSE;
/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : handle fmm file copy window message
//  Global resource dependence :
//  Author: xinrui.wang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleCopyFileToFolderWinMsg (
    MMI_WIN_ID_T      win_id,
    MMI_MESSAGE_ID_E  msg_id,
    DPARAM            param
);

/*****************************************************************************/
//  Description : get target path callback
//  Global resource dependence : none
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL void GetTargetPathCallBack (
    BOOLEAN   is_success,
    FILEARRAY file_array);
/*****************************************************************************/
// Description : open ctrl func
// Global resource dependence :
// Author:  jian.ma
// Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E CopyWinOpenCtrlFunc (MMI_WIN_ID_T win_id);

/*****************************************************************************/
// Description : close ctrl func
// Global resource dependence :
// Author:  jian.ma
// Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E CopyWinCloseCtrlFunc (MMI_WIN_ID_T win_id);

/*****************************************************************************/
//  Description : wait deleting window message
//
//  Global resource dependence :
//  Author: robert.lu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleWaitCopyWinMsg (
    MMI_WIN_ID_T        win_id,
    MMI_MESSAGE_ID_E    msg_id,
    DPARAM              param
);


/*****************************************************************************/
//  Description : close copy del handle
//  Global resource dependence :
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL void FmmCloseCopyDelHandle (MMI_WIN_ID_T win_id);

/*****************************************************************************/
//  Description : copy del exit
//  Global resource dependence :
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL void FmmCopyDelExit (MMI_WIN_ID_T win_id);




// the copy file window for FMM
WINDOW_TABLE (MMIFMM_COPY_SELECTFILE_TOFOLDER_WIN_TAB) =
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC ( (uint32) HandleCopyFileToFolderWinMsg),
    WIN_ID (MMIFMM_COPY_SELECT_FILE_WIN_ID),
    WIN_STYLE (WS_HAS_TRANSPARENT),
    WIN_SOFTKEY (TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};

/*****************************************************************************/
//  Description : handle fmm file copy window message
//  Global resource dependence :
//  Author: xinrui.wang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleCopyFileToFolderWinMsg (
    MMI_WIN_ID_T      win_id,
    MMI_MESSAGE_ID_E  msg_id,
    DPARAM            param
)
{
    MMI_RESULT_E            result = MMI_RESULT_TRUE;
    uint32                  i =  0;
    int32                   j = 0;
    uint16                  file_ret = SFS_NO_ERROR;
    BOOLEAN                 is_file = TRUE;
    MMI_STRING_T            prompt_str1 = {0};
    MMI_STRING_T            prompt_str2 = {0};
    MMI_WIN_ID_T            query_win_id = MMIFMM_QUERY_WIN_ID;
    MMIFILE_DEVICE_E        fs_dev = MMI_DEVICE_UDISK;
#ifdef MMI_FMM_MOVE_SUPPORT
    uint16                  move_dst_len = 0;
    uint16                  move_src_len = 0;
    uint16                  max_len      = 0;
#endif
    MMIFMM_COPYWIN_INFO_T *copy_win_info = (MMIFMM_COPYWIN_INFO_T*) MMK_GetWinUserData (win_id);

    //SCI_TRACE_LOW:"MMIFMM: HandleCopyFileToFolderWinMsg, win_id =%d, msg_id = 0x%x"
    SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_7930_112_2_18_2_20_51_472, (uint8*) "dd", win_id, msg_id);

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
#ifdef MID_SOFTKEY_USE_IMG_SUPPORT
		GUIWIN_SeSoftkeytButtonIconId(win_id, IMAGE_NULL, 1, TRUE);
#endif
        if (MMI_RESULT_FALSE == CopyWinOpenCtrlFunc (win_id))
        {
            MMK_CloseWin (win_id);
            break;
        }
        copy_win_info = (MMIFMM_COPYWIN_INFO_T*) MMK_GetWinUserData (win_id);
        if (PNULL == copy_win_info)
        {
            MMK_CloseWin (win_id);
            break;
        }
        copy_win_info->s_copy_handle_ptr = PNULL;
        copy_win_info->s_need_update = FALSE;
#ifdef MMI_FMM_MOVE_SUPPORT

        if (copy_win_info->s_is_copy_from_move) //如果是从move动作过渡到copy,则不需要去选择目标路径了
        {
            MMK_SendMsg (win_id, MSG_FMM_COPY_CHOOSE_PATH_RETURN, PNULL);
        }
        else
#endif
        {
            MMK_SendMsg (win_id, MSG_FMM_OPEN_WIN_CHOOSE_PATH, PNULL);
        }

        break;

    case MSG_PROMPTWIN_CANCEL:       //取消当前文件复制，到下一个文件
        MMIPUB_CloseQuerytWin (&query_win_id);
        MMIFMM_OpenComWaitWin (HandleWaitCopyWinMsg);
        APPFILE_CopyClose (copy_win_info->s_copy_handle_ptr);
        copy_win_info->s_copy_handle_ptr = PNULL;
        MMK_PostMsg (win_id, MSG_FMM_SELECT_MARK_FILE, PNULL, 0);
        break;

    case MSG_PROMPTWIN_OK:      //确定覆盖已有文件
        MMIPUB_CloseQuerytWin (&query_win_id);
        //overwrite
        MMIFILE_CopyOverwrite (copy_win_info->s_copy_handle_ptr);
        MMIFMM_OpenComWaitWin (HandleWaitCopyWinMsg);
        break;

    case MSG_FMM_SELECT_MARK_FILE://标记状态复制
        //查找标记的文件
        for (i = copy_win_info->s_copy_del_path_data_ptr->cur_mark_num; i < (uint16) (copy_win_info->list_data_ptr->file_num + copy_win_info->list_data_ptr->folder_num); i++)
        {
            if (MMIFMM_FILE_STATE_UNCHECKED != copy_win_info->list_data_ptr->data[i]->state)
            {
                break;
            }
        }

        //错误保护，如果超过最大文件数退出复制
        if (i >= (uint16) (copy_win_info->list_data_ptr->file_num + copy_win_info->list_data_ptr->folder_num))
        {
            MMIPUB_CloseWaitWin (MMIFMM_FILE_WAITING_WIN_ID);
            MMK_CloseWin (win_id);
        }
        else
        {
            copy_win_info->s_copy_del_path_data_ptr->cur_mark_num = (uint16) (i + 1);
            SCI_MEMSET (copy_win_info->s_copy_del_path_data_ptr->cur_pathname, 0, (sizeof (wchar) * (MMIFMM_PATHNAME_LEN + 1)));
            SCI_MEMSET (copy_win_info->s_copy_del_path_data_ptr->copyfile_name, 0, (sizeof (wchar) * (MMIFMM_PATHNAME_LEN + 1)));

//#ifdef MMI_FMM_MOVE_SUPPORT
//            if(copy_win_info->s_is_copy_from_move)
//            {
//                MMIFMM_SetMoveIndex((uint16)i);
//            }
//#endif
            if (copy_win_info->list_data_ptr->path_is_valid > 0)
            {
                SCI_MEMSET (copy_win_info->s_copy_del_path_data_ptr->user_scr_path, 0, (sizeof (wchar) * (MMIFMM_PATHNAME_LEN + 1)));
                MMIAPICOM_Wstrncpy (copy_win_info->s_copy_del_path_data_ptr->user_scr_path, copy_win_info->list_data_ptr->data[i]->filename_ptr,
                                    copy_win_info->list_data_ptr->data[i]->filename_len);

                //去掉后面的文件名，只保留路径
                for (j = copy_win_info->list_data_ptr->data[i]->filename_len; j > 0 && copy_win_info->s_copy_del_path_data_ptr->user_scr_path[j-1] != MMIFILE_SLASH; j--)
                {
                    copy_win_info->s_copy_del_path_data_ptr->user_scr_path[j-1] = 0x0000;
                }

                MMIAPICOM_Wstrncpy (copy_win_info->s_copy_del_path_data_ptr->cur_pathname, copy_win_info->list_data_ptr->data[i]->filename_ptr,
                                    copy_win_info->list_data_ptr->data[i]->filename_len);
            }
            else
            {
                MMIFMM_CombineToFileName (copy_win_info->current_path_ptr->pathname, copy_win_info->current_path_ptr->pathname_len, copy_win_info->list_data_ptr->data[i]->filename_ptr, copy_win_info->list_data_ptr->data[i]->filename_len,
                                          copy_win_info->s_copy_del_path_data_ptr->cur_pathname, MMIFMM_PATHNAME_LEN);
            }

#ifdef DRM_SUPPORT

            if (MMIAPIDRM_IsDRMFile (SFS_INVALID_HANDLE, copy_win_info->s_copy_del_path_data_ptr->cur_pathname))
            {
                MMK_PostMsg (win_id, MSG_FMM_SELECT_MARK_FILE, PNULL, 0);
                break;
            }

#endif

            MMK_PostMsg (win_id, MSG_FMM_OPERATE_FILE_BEGIN, PNULL, 0);
        }

        break;

    case MSG_FMM_OPEN_WIN_CHOOSE_PATH://选择复制路径

        MMIFMM_GetSelectTargetPath (GetTargetPathCallBack);
        //MMIAPIFMM_GetSelectivePathName(win_id, MSG_FMM_COPY_CHOOSE_PATH_RETURN, copy_win_info->s_copy_del_path_data_ptr->target_path_info.pathname);
        break;

    case MSG_FMM_COPY_CHOOSE_PATH_RETURN://选择路径返回
        MMIFMM_OpenComWaitWin (HandleWaitCopyWinMsg);
        APPFILE_CreateTask();

        if (copy_win_info->s_copy_del_path_data_ptr->mark_flag > 0)
        {
            //selected marked files
            MMK_PostMsg (win_id, MSG_FMM_SELECT_MARK_FILE, PNULL, 0);
        }
        else
        {
            //no mark, copy current file
            MMK_PostMsg (win_id, MSG_FMM_OPERATE_FILE_BEGIN, PNULL, 0);
        }

        break;

    case MSG_FMM_OPERATE_FILE_BEGIN://开始复制操作
        //检查复制文件夹时，是否源文件夹和目标文件夹嵌套
        if (MMIFMM_IsPath1ContainPath2 (copy_win_info->s_copy_del_path_data_ptr->target_path_info.pathname,
                                        copy_win_info->s_copy_del_path_data_ptr->target_path_info.pathname_len,
                                        copy_win_info->s_copy_del_path_data_ptr->cur_pathname,
                                        MMIAPICOM_Wstrlen (copy_win_info->s_copy_del_path_data_ptr->cur_pathname)
                                       ))
        {
            //源文件夹和目标文件夹嵌套,不能复制
            copy_win_info->s_need_update = TRUE;
            MMIPUB_CloseWaitWin (MMIFMM_FILE_WAITING_WIN_ID);
            MMIPUB_OpenAlertWarningWin (TXT_FMM_CANNOT_COPY);
            MMK_CloseWin (win_id);
            break;
        }
        else
        {
            //start copy
            copy_win_info->s_copy_handle_ptr = MMIFILE_CopyOpen (copy_win_info->s_copy_del_path_data_ptr->cur_pathname,
                                               MMIAPICOM_Wstrlen (copy_win_info->s_copy_del_path_data_ptr->cur_pathname),
                                               copy_win_info->s_copy_del_path_data_ptr->target_path_info.pathname,
                                               copy_win_info->s_copy_del_path_data_ptr->target_path_info.pathname_len);

            MMIDEFAULT_AllowTurnOffBackLight (FALSE);//复制过程不允许锁屏

            if (PNULL == copy_win_info->s_copy_handle_ptr)
            {
                //copy fail
                MMIPUB_CloseWaitWin (MMIFMM_FILE_WAITING_WIN_ID);
                MMIPUB_OpenAlertFailWin (TXT_ERROR);
                MMK_CloseWin (win_id);
            }
        }

        break;
    case MSG_FMM_FILE_EXIST:
        //query overwrite or not ?
        MMIPUB_CloseWaitWin (MMIFMM_FILE_WAITING_WIN_ID);
#ifdef MMI_FMM_MOVE_SUPPORT
        move_dst_len = copy_win_info->s_copy_del_path_data_ptr->target_path_info.pathname_len;
        move_src_len = copy_win_info->current_path_ptr->pathname_len;
        max_len = (uint16) MAX (move_dst_len, move_src_len);

        if (copy_win_info->s_is_copy_from_move
                && (0 == MMIAPICOM_Wstrncmp (copy_win_info->s_copy_del_path_data_ptr->target_path_info.pathname,
                                             copy_win_info->s_copy_del_path_data_ptr->user_scr_path,
                                             max_len))) //从move过渡过来，提示不能移动，目标文件和源文件相同
        {
            MMIPUB_OpenAlertWarningWin (TXT_FMM_MOVE_SOURCE_EQU_DEST);
            MMK_CloseWin (win_id);
        }
        else
#endif
        {
            if (MMIAPICC_IsInState (CC_MT_CONNECTING_STATE) || MMIAPICC_IsInState (CC_MO_CONNECTING_STATE))
            {
                MMIAPIFMM_ExitFileOption();
                break;
            }

            prompt_str1.wstr_ptr = (wchar *) SCI_ALLOC_APP ( (MMIFMM_FULL_FILENAME_LEN + 1) * sizeof (wchar));
            SCI_MEMSET (prompt_str1.wstr_ptr, 0x00, ( (MMIFMM_FULL_FILENAME_LEN + 1) *sizeof (wchar)));
            prompt_str1.wstr_len = MMIFMM_FULL_FILENAME_LEN;
            MMIAPICOM_GetFileName (copy_win_info->s_copy_handle_ptr->dest_full_path,
                                   MIN (MMIFMM_FULL_FILENAME_LEN, copy_win_info->s_copy_handle_ptr->dest_full_path_len),
                                   prompt_str1.wstr_ptr,
                                   &prompt_str1.wstr_len);
            //MMIAPICOM_Wstrncpy(prompt_str1.wstr_ptr, copy_win_info->s_copy_handle_ptr->dest_full_path, MIN(MMIFMM_FULL_FILENAME_LEN, copy_win_info->s_copy_handle_ptr->dest_full_path_len));
            MMI_GetLabelTextByLang (TXT_FMM_REPEAT_REPLACE, &prompt_str2);
            //if(!MMIAPICC_IsInState(CC_IN_CALL_STATE))
            {
                MMIPUB_OpenQueryWinByTextPtr (&prompt_str1, &prompt_str2, IMAGE_PUBWIN_QUERY, &query_win_id, HandleOverWriteQueryWin);
                {
                    MMIFMM_QUERYWIN_DATA_T query_win = {0};
                    query_win.win_id = MMIFMM_COPY_SELECT_FILE_WIN_ID;
                    MMIFMM_QueryAddCtrlFunc (query_win_id, &query_win);
                }
            }

            if (PNULL != prompt_str1.wstr_ptr)
            {
                SCI_FREE (prompt_str1.wstr_ptr);
                prompt_str1.wstr_ptr = PNULL;
            }
        }

        break;
    case MSG_FMM_OPERATE_ONE_FILE_RETURN:
        APPFILE_CopyClose (copy_win_info->s_copy_handle_ptr);
        copy_win_info->s_copy_handle_ptr = PNULL;
        file_ret  = * ( (uint16 *) param);

        if (SFS_ERROR_NONE == file_ret)
        {
            copy_win_info->s_need_update = TRUE;
#ifdef MMI_FMM_MOVE_SUPPORT

            if (copy_win_info->s_is_copy_from_move) //如果是从move过渡到copy，copy完一个，要把source文件删除
            {
                copy_win_info->s_delete_handle_ptr = MMIFILE_DeleteOpen (copy_win_info->s_copy_del_path_data_ptr->cur_pathname,
                                                     MMIAPICOM_Wstrlen (copy_win_info->s_copy_del_path_data_ptr->cur_pathname));

                if (PNULL == copy_win_info->s_delete_handle_ptr)
                {
                    //delete fail
                    SCI_TRACE_LOW ("[mmifmm]:copy from move copy_win_info->s_delete_handle_ptr == PNULL");
                    MMIPUB_CloseWaitWin (MMIFMM_FILE_WAITING_WIN_ID);
                    MMIPUB_OpenAlertFailWin (TXT_ERROR);
                    MMK_CloseWin (win_id);
                }
            }
            else
#endif
            {
                //copy success, get next to copy
                MMK_PostMsg (win_id, MSG_FMM_SELECT_MARK_FILE, PNULL, 0);
            }
        }
        else
        {
            fs_dev = MMIAPIFMM_GetDeviceTypeByPath (copy_win_info->s_copy_del_path_data_ptr->target_path_info.pathname, MMIFILE_DEVICE_LEN);

            if (MMIAPIFMM_IsFolderExist (copy_win_info->s_copy_del_path_data_ptr->cur_pathname, MMIAPICOM_Wstrlen (copy_win_info->s_copy_del_path_data_ptr->cur_pathname)))
            {
                is_file = FALSE;
            }

            MMIFMM_PromptFileErrorType (PNULL, (uint32) file_ret, fs_dev, is_file);
            MMK_CloseWin (win_id);
            MMIPUB_CloseWaitWin (MMIFMM_FILE_WAITING_WIN_ID);
            MMK_CloseWin (MMIFMM_QUERY_WIN_ID);
            MMK_PostMsg (MMIFMM_MAIN_WIN_ID, MSG_FMM_COPY_END_CNF, PNULL, 0);
        }

        break;

#ifdef MMI_FMM_MOVE_SUPPORT
    case MSG_FMM_DELETE_END_CNF:

        if (PNULL != copy_win_info->s_delete_handle_ptr && copy_win_info->s_is_copy_from_move)
        {
            MMIFILE_DeleteClose (copy_win_info->s_delete_handle_ptr);
            copy_win_info->s_delete_handle_ptr = PNULL;
            MMK_PostMsg (win_id, MSG_FMM_SELECT_MARK_FILE, PNULL, 0);
        }

        break;
#endif

    case MSG_FMM_ABORT:
        APPFILE_AbortCopy (copy_win_info->s_copy_handle_ptr);
        break;
    case MSG_APP_CANCEL:
        APPFILE_AbortCopy (copy_win_info->s_copy_handle_ptr);
        MMIPUB_CloseWaitWin (MMIFMM_FILE_WAITING_WIN_ID);
        MMK_CloseWin (MMIFMM_QUERY_WIN_ID);
        MMK_CloseWin (win_id);
        break;
    case MSG_CLOSE_WINDOW:
        MMK_CloseWin (MMIFMM_OPEN_FILE_BYLAYER_WIN_ID);
        MMK_CloseWin (MMIFMM_OPEN_FILE_BYLAYER_WAIT_WIN_ID);
        MMK_CloseWin (MMIFMM_OPEN_FILE_WIN_ID);

        if (PNULL != copy_win_info->s_copy_handle_ptr)
        {
            APPFILE_CopyClose (copy_win_info->s_copy_handle_ptr);
            copy_win_info->s_copy_handle_ptr = PNULL;
        }

#ifdef MMI_FMM_MOVE_SUPPORT

        if (PNULL != copy_win_info->s_delete_handle_ptr)
        {
            MMIFILE_DeleteClose (copy_win_info->s_delete_handle_ptr);
            copy_win_info->s_delete_handle_ptr = PNULL;
        }

#endif
        FmmCopyDelExit (win_id);
        MMIDEFAULT_AllowTurnOffBackLight (TRUE);//复制完，关闭窗口，允许锁屏
        copy_win_info->s_copydel_opt_state = 0;
        CopyWinCloseCtrlFunc (win_id);
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }

    return (result);
}

/*****************************************************************************/
// Description : enter copy win
// Global resource dependence :
// Author:  jian.ma
// RETRUN:
// Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIFMM_EnterCopyWin (
    MMIFMM_DATA_INFO_T    *list_data_ptr,
    MMIFMM_PATH_INFO_T    *current_path_ptr,
    wchar *user_scr_path,
    wchar *cur_pathname,
    uint16 current_index
)
{
    MMI_RESULT_E                result = MMI_RESULT_FALSE;
    MMI_HANDLE_T                win_handle = 0;
    MMIFMM_COPYWIN_INFO_T       * copywin_data = PNULL;

    if (PNULL == list_data_ptr)
    {
        return MMI_RESULT_FALSE;
    }

    if (MMK_IsOpenWin (MMIFMM_COPY_SELECT_FILE_WIN_ID))
    {
        MMK_CloseWin (MMIFMM_COPY_SELECT_FILE_WIN_ID);
    }

    copywin_data = (MMIFMM_COPYWIN_INFO_T*) SCI_ALLOC_APPZ (sizeof (MMIFMM_COPYWIN_INFO_T));
    copywin_data->list_data_ptr = list_data_ptr;
    copywin_data->current_path_ptr = current_path_ptr;
    copywin_data->s_copy_del_path_data_ptr = (MMIFMM_COPY_DEL_PATH_DATA_INFO_T *) SCI_ALLOC_APPZ (sizeof (MMIFMM_COPY_DEL_PATH_DATA_INFO_T));
    copywin_data->s_need_update = FALSE;

    if (PNULL == copywin_data->s_copy_del_path_data_ptr)
    {
        //SCI_TRACE_LOW:"CopySelectFileToFolder alloc s_copy_del_path_data_ptr fail!"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_7679_112_2_18_2_20_50_466, (uint8*) "");
        SCI_FREE (copywin_data);
        return result;
    }

    MMIAPICOM_Wstrncpy (copywin_data->s_copy_del_path_data_ptr->user_scr_path, user_scr_path, MMIAPICOM_Wstrlen (user_scr_path));
    MMIAPICOM_Wstrncpy (copywin_data->s_copy_del_path_data_ptr->cur_pathname, cur_pathname, MMIAPICOM_Wstrlen (cur_pathname));
    copywin_data->s_copy_del_path_data_ptr->mark_flag = list_data_ptr->mark_num;

#ifdef MMI_FMM_MOVE_SUPPORT

    if (MMIFMM_IsCopyFromMove())
    {
        MMIFMM_SetTargetPathName (current_index, &copywin_data->s_copy_del_path_data_ptr->target_path_info);
        copywin_data->s_is_copy_from_move = TRUE;
    }

#endif
    win_handle = MMK_CreateWin ( (uint32*) MMIFMM_COPY_SELECTFILE_TOFOLDER_WIN_TAB, (ADD_DATA) copywin_data);

    if (!win_handle || (MMI_INVALID_ID == win_handle))
    {
        SCI_FREE (copywin_data->s_copy_del_path_data_ptr);
        SCI_FREE (copywin_data);
        result =  MMI_RESULT_FALSE;
    }
    else
    {
        result = MMI_RESULT_TRUE;
    }

    return result;
}


/*****************************************************************************/
//  Description : get target path callback
//  Global resource dependence : none
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL void GetTargetPathCallBack (
    BOOLEAN   is_success,
    FILEARRAY file_array
)
{
    FILEARRAY_DATA_T          file_array_data = {0};

    if (is_success)
    {
        MMIFMM_COPYWIN_INFO_T *win_data_p = (MMIFMM_COPYWIN_INFO_T*) MMK_GetWinUserData (MMIFMM_COPY_SELECT_FILE_WIN_ID);

        if (PNULL != win_data_p && 0 < MMIAPIFILEARRAY_GetArraySize (file_array) && MMIAPIFILEARRAY_Read (file_array, 0, &file_array_data))
        {

            //read user selected dir
            MMIAPICOM_Wstrncpy (win_data_p->s_copy_del_path_data_ptr->target_path_info.pathname, file_array_data.filename, file_array_data.name_len);
            win_data_p->s_copy_del_path_data_ptr->target_path_info.pathname_len = file_array_data.name_len;
            MMK_PostMsg (MMIFMM_COPY_SELECT_FILE_WIN_ID, MSG_FMM_COPY_CHOOSE_PATH_RETURN, PNULL, 0);
        }
        else
        {
            if (MMK_IsOpenWin (MMIFMM_COPY_SELECT_FILE_WIN_ID))
            {
                MMK_CloseWin (MMIFMM_COPY_SELECT_FILE_WIN_ID);
            }

            //SCI_TRACE_LOW:"GetTargetPathCallBack array size == 0"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_12385_112_2_18_2_21_1_544, (uint8*) "");
        }
    }
    else
    {
        if (MMK_IsOpenWin (MMIFMM_COPY_SELECT_FILE_WIN_ID))
        {
            MMK_CloseWin (MMIFMM_COPY_SELECT_FILE_WIN_ID);
        }

        //SCI_TRACE_LOW:"GetTargetPathCallBack failed"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_12394_112_2_18_2_21_1_545, (uint8*) "");
    }

    //释放列表
    MMIAPIFILEARRAY_Destroy (&file_array);
    file_array = PNULL;
}

/*****************************************************************************/
// Description : open ctrl func
// Global resource dependence :
// Author:  jian.ma
// Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E CopyWinOpenCtrlFunc (MMI_WIN_ID_T win_id)
{
    MMIFMM_COPYWIN_INFO_T *detail_data = (MMIFMM_COPYWIN_INFO_T*) MMK_GetWinAddDataPtr (win_id);

    MMK_SetWinUserData (win_id, (void *) detail_data);

    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
// Description : close ctrl func
// Global resource dependence :
// Author:  jian.ma
// Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E CopyWinCloseCtrlFunc (MMI_WIN_ID_T win_id)
{
    MMIFMM_COPYWIN_INFO_T *win_data_p = (MMIFMM_COPYWIN_INFO_T*) MMK_GetWinUserData (win_id);


    if (PNULL != win_data_p && PNULL != win_data_p->s_copy_del_path_data_ptr)
    {
        SCI_FREE (win_data_p->s_copy_del_path_data_ptr);
        win_data_p->s_copy_del_path_data_ptr = PNULL;
    }

    SCI_FREE (win_data_p);
    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
//  Description : wait deleting window message
//
//  Global resource dependence :
//  Author: robert.lu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleWaitCopyWinMsg (
    MMI_WIN_ID_T        win_id,
    MMI_MESSAGE_ID_E    msg_id,
    DPARAM              param
)
{
    MMI_RESULT_E                result = MMI_RESULT_TRUE;
    MMIFMM_COPYWIN_INFO_T       *win_data_p = (MMIFMM_COPYWIN_INFO_T*) MMK_GetWinUserData (MMIFMM_COPY_SELECT_FILE_WIN_ID);

    //SCI_TRACE_LOW:"MMIFMM: HandleWaitCopyWinMsg, win_id =%d, msg_id = 0x%x."
    SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_7856_112_2_18_2_20_51_470, (uint8*) "dd", win_id, msg_id);

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:

        if (PNULL != win_data_p)
        {
            win_data_p->s_user_stop_copy = FALSE;
        }

        result = MMIPUB_HandleWaitWinMsg (win_id, msg_id, param);
        break;
    case MSG_CTL_OK:
    case MSG_APP_OK:
        // 不处理
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:

        if (PNULL != win_data_p && !win_data_p->s_user_stop_copy)
        {
            win_data_p->s_user_stop_copy = TRUE;
            MMK_PostMsg (MMIFMM_COPY_SELECT_FILE_WIN_ID, MSG_FMM_ABORT, PNULL, 0);
        }

        break;
    case MSG_CLOSE_WINDOW:

        if (PNULL != win_data_p)
        {
            if (PNULL != win_data_p->s_copy_handle_ptr && (!win_data_p->s_user_stop_copy))
            {
                APPFILE_AbortCopy (win_data_p->s_copy_handle_ptr);
            }

            win_data_p->s_user_stop_copy = TRUE;
        }

        MMK_CloseWin (MMIFMM_QUERY_WIN_ID);
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_7892_112_2_18_2_20_51_471, (uint8*) "");
        break;
    default:
        result = MMIPUB_HandleWaitWinMsg (win_id, msg_id, param);
        break;
    }

    return (result);
}

/*****************************************************************************/
//  Description : copy del exit
//  Global resource dependence :
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL void FmmCopyDelExit (MMI_WIN_ID_T win_id)
{
    MMIFMM_COPYWIN_INFO_T       *copywin_data_p = (MMIFMM_COPYWIN_INFO_T*) MMK_GetWinUserData (win_id);

    FmmCloseCopyDelHandle (win_id);

    if (PNULL != copywin_data_p && copywin_data_p->s_need_update)
    {
#ifdef MMI_PDA_SUPPORT
        GUIFORM_SetChildDisplay (MMIFMM_UDISK_FORM_CTRL_ID, MMICOMMON_SOFTKEY_CTRL_ID, GUIFORM_CHILD_DISP_HIDE);
#endif
        //stop copy now
        MMK_PostMsg (MMIFMM_MAIN_WIN_ID, MSG_FMM_COPY_END_CNF, PNULL, 0);
    }
}


/*****************************************************************************/
//  Description : close copy del handle
//  Global resource dependence :
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL void FmmCloseCopyDelHandle (MMI_WIN_ID_T win_id)
{
    MMIFMM_COPYWIN_INFO_T       *copywin_data_p = (MMIFMM_COPYWIN_INFO_T*) MMK_GetWinUserData (win_id);

    if (PNULL != copywin_data_p
            && PNULL != copywin_data_p->s_copy_del_path_data_ptr
            && 0 != copywin_data_p->s_copy_del_path_data_ptr->sfs_find_handle)
    {
        SFS_FindClose (copywin_data_p->s_copy_del_path_data_ptr->sfs_find_handle);
        copywin_data_p->s_copy_del_path_data_ptr->sfs_find_handle = 0;
    }
}



/*****************************************************************************/
// Description : copy next
// Global resource dependence :
// Author:
// RETRUN:
// Note:
/*****************************************************************************/
PUBLIC void MMIAPIFMM_CopyNext (void)
{
    MMIFMM_COPYWIN_INFO_T       *win_data_p = (MMIFMM_COPYWIN_INFO_T*) MMK_GetWinUserData (MMIFMM_COPY_SELECT_FILE_WIN_ID);
    
    if (PNULL != win_data_p) 
    {
		//coverity 12975
		//SCI_TRACE_LOW:"[MMIFMM] MMIAPIFMM_CopyNext s_user_stop_copy %d enter"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_12890_112_2_18_2_21_2_549, (uint8*) "d", win_data_p->s_user_stop_copy);
		if (!win_data_p->s_user_stop_copy && MMK_IsOpenWin (MMIFMM_COPY_SELECT_FILE_WIN_ID))
		{
			 MMK_PostMsg (MMIFMM_COPY_SELECT_FILE_WIN_ID, MSG_FMM_COPY_FILE_CIRCLE, PNULL, 0);
		}
    }
}


/*****************************************************************************/
//  Description : handle copy task callback
//  Global resource dependence :
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
void MMIAPIFMM_CopyFileExistInform (void)
{
    //SCI_TRACE_LOW:"[MMIFMM] MMIAPIFMM_CopyFileExistInform enter"
    SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_8612_112_2_18_2_20_53_478, (uint8*) "");

    if (MMK_IsOpenWin (MMIFMM_COPY_SELECT_FILE_WIN_ID))
    {
        MMK_PostMsg (MMIFMM_COPY_SELECT_FILE_WIN_ID, MSG_FMM_FILE_EXIST, PNULL, 0);
    }
}


/*****************************************************************************/
//  Description : handle copy task callback
//  Global resource dependence :
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
void MMIAPIFMM_CopyCallBack (
    uint16     result
)
{
    if (MMK_IsOpenWin (MMIFMM_COPY_SELECT_FILE_WIN_ID))
    {
        MMK_PostMsg (MMIFMM_COPY_SELECT_FILE_WIN_ID, MSG_FMM_OPERATE_ONE_FILE_RETURN, &result, sizeof (result));
    }
    else if (MMK_IsOpenWin (MMIFMM_MOVE_FILE_WIN_ID))
    {
        MMK_PostMsg (MMIFMM_MOVE_FILE_WIN_ID, MSG_FMM_OPERATE_ONE_FILE_RETURN, &result, sizeof (result));
    }
    else
    {
        MMK_PostMsg (MMIFMM_MAIN_WIN_ID, MSG_FMM_COPY_END_CNF, PNULL, 0);
    }
}