/*****************************************************************************
** File Name:      mmifmm_movewin.c                                          *
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

#define _MMIFMM_MOVEWIN_C_


/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_app_fmm_trc.h"
#include "mmifmm_movewin.h"
#include "sci_types.h"
#include "mmk_type.h"
#include "window_parse.h"
#include "mmifmm_id.h"
#include "mmifmm_text.h"
#include "mmi_filetask.h"
#include "mmifmm_comfunc.h"
#ifdef DRM_SUPPORT
#include "mmidrm_export.h"
#endif
#include "mmipub.h"
#include "mmicc_export.h"
#include "mmk_app.h"
#include "mmifmm_comwin.h"
#include "mmifmm_app.h"
#include "mmifmm_copywin.h"
#include "mmi_appmsg.h"
#ifdef  VIDEOTHUMBNAIL_SUPPORT
#include "mmifmm_vthumbnail.h"
#endif
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/

typedef struct
{
    wchar    file_name[MMIFMM_FULL_FILENAME_LEN +1];
    wchar    path_name[MMIFMM_PATHNAME_LEN + 1];
} MMIFMM_MOVE_DATA_INFO_T;

typedef struct
{
    MMIFMM_DATA_INFO_T      *list_data_ptr;
    MMIFMM_PATH_INFO_T      *current_path_ptr;
    MMIFMM_MOVE_DATA_INFO_T *s_src_data_ptr;
    APP_MOVE_HANDLE_T       *s_move_handle_ptr;
    APP_COPY_HANDLE_T       *s_copy_handle_ptr;
    APP_DELETE_HANDLE_T     *s_delete_handle_ptr;
    wchar                   *src_full_path_ptr;
    wchar                   *s_move_dst_path_ptr;
    uint16                  src_full_pathlen;
    uint16                  mark_num;
    uint16                  s_move_index;
    BOOLEAN                 s_is_copy_from_move;
} MMIFMM_MOVEWIN_DATA_INFO_T;



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
// 	Description : handle move file win msg
//	Global resource dependence :
//  Author: juan.wu
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMoveFileWinMsg (
    MMI_WIN_ID_T        win_id,
    MMI_MESSAGE_ID_E    msg_id,
    DPARAM              param
);

/*****************************************************************************/
//  Description : wait move window message
//  Global resource dependence :
//  Author: juan.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleWaitMoveWinMsg (
    MMI_WIN_ID_T        win_id,
    MMI_MESSAGE_ID_E    msg_id,
    DPARAM              param
);

/*****************************************************************************/
//  Description : get move file target path callback
//  Global resource dependence : none
//  Author: juan.wu
//  Note:
/*****************************************************************************/
LOCAL void GetMoveTargetPathCallBack (
    BOOLEAN   is_success,
    FILEARRAY file_array
);

/*****************************************************************************/
// Description : open ctrl func
// Global resource dependence :
// Author:  jian.ma
// Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E MoveOpenCtrlFunc (MMI_WIN_ID_T win_id);

/*****************************************************************************/
// Description : close ctrl func
// Global resource dependence :
// Author:  jian.ma
// Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E MoveCloseCtrlFunc (MMI_WIN_ID_T win_id);


WINDOW_TABLE (MMIFMM_MOVE_FILE_WIN_TAB) =
{
    WIN_FUNC ( (uint32) HandleMoveFileWinMsg),
    WIN_ID (MMIFMM_MOVE_FILE_WIN_ID),
    WIN_STYLE (WS_HAS_TRANSPARENT),
    WIN_SOFTKEY (TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};

/*****************************************************************************/
// Description : preview current video file
// Global resource dependence :
// Author:  murphy.xie
// RETRUN:
// Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIFMM_EnterMoveWin (
    MMIFMM_DATA_INFO_T* list_data_ptr,
    MMIFMM_PATH_INFO_T* current_path_ptr
)
{
    MMI_RESULT_E                result = MMI_RESULT_FALSE;
    MMI_HANDLE_T                win_handle = 0;
    MMIFMM_MOVEWIN_DATA_INFO_T *win_data_p = PNULL;

    if (PNULL == list_data_ptr || PNULL == current_path_ptr)
    {
        return MMI_RESULT_FALSE;
    }

    if (MMK_IsOpenWin (MMIFMM_MOVE_FILE_WIN_ID))
    {
        MMK_CloseWin (MMIFMM_MOVE_FILE_WIN_ID);
    }


    win_data_p = SCI_ALLOC_APPZ (sizeof (MMIFMM_MOVEWIN_DATA_INFO_T));
    win_data_p->list_data_ptr = list_data_ptr;
    win_data_p->current_path_ptr = current_path_ptr;
    win_data_p->s_src_data_ptr = SCI_ALLOC_APPZ (sizeof (MMIFMM_MOVE_DATA_INFO_T));
    win_data_p->src_full_path_ptr = SCI_ALLOC_APPZ (sizeof (wchar) * (MMIFMM_FULL_FILENAME_LEN + 1));
    win_data_p->s_move_dst_path_ptr = SCI_ALLOC_APPZ (sizeof (wchar) * (MMIFMM_PATHNAME_LEN + 1));
    win_data_p->s_is_copy_from_move = FALSE;

    win_handle = MMK_CreateWin ( (uint32 *) MMIFMM_MOVE_FILE_WIN_TAB , PNULL);

    if (!win_handle || (MMI_INVALID_ID == win_handle))
    {
        SCI_FREE (win_data_p->s_src_data_ptr);
        SCI_FREE (win_data_p->src_full_path_ptr);
        SCI_FREE (win_data_p->s_move_dst_path_ptr);
        SCI_FREE (win_data_p);
        result =  MMI_RESULT_FALSE;
    }
    else
    {
        if (MMK_SetWinUserData (win_handle, win_data_p))
        {
            result = MMI_RESULT_TRUE;
        }
        else
        {
            SCI_FREE (win_data_p->s_src_data_ptr);
            SCI_FREE (win_data_p->src_full_path_ptr);
            SCI_FREE (win_data_p->s_move_dst_path_ptr);
            SCI_FREE (win_data_p);
            result =  MMI_RESULT_FALSE;
        }
    }

    return result;
}
/*****************************************************************************/
//  Description : get move file target path callback
//  Global resource dependence : none
//  Author: juan.wu
//  Note:
/*****************************************************************************/
LOCAL void GetMoveTargetPathCallBack (
    BOOLEAN   is_success,
    FILEARRAY file_array
)
{
    FILEARRAY_DATA_T          file_array_data = {0};
    uint16                    pathname_len = 0;
    MMIFMM_MOVEWIN_DATA_INFO_T*   move_data_ptr = (MMIFMM_MOVEWIN_DATA_INFO_T*) MMK_GetWinUserData (MMIFMM_MOVE_FILE_WIN_ID);

    if (is_success)
    {
        if (PNULL != move_data_ptr && 0 < MMIAPIFILEARRAY_GetArraySize (file_array) && MMIAPIFILEARRAY_Read (file_array, 0, &file_array_data))
        {
            //read user selected dir
            SCI_MEMSET (move_data_ptr->s_move_dst_path_ptr, 0, sizeof (wchar) * (MMIFMM_PATHNAME_LEN + 1));
            MMIAPICOM_Wstrncpy (move_data_ptr->s_move_dst_path_ptr, file_array_data.filename, file_array_data.name_len);
            pathname_len = file_array_data.name_len;
            MMK_SendMsg (MMIFMM_MOVE_FILE_WIN_ID, MSG_FMM_COPY_CHOOSE_PATH_RETURN, PNULL);
        }
        else
        {
            if (MMK_IsOpenWin (MMIFMM_MOVE_FILE_WIN_ID))
            {
                MMK_CloseWin (MMIFMM_MOVE_FILE_WIN_ID);
            }

            //SCI_TRACE_LOW:"[MMIFMM]:GetMoveTargetPathCallBack array size == 0"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_14302_112_2_18_2_21_5_582, (uint8*) "");
        }
    }
    else
    {
        if (MMK_IsOpenWin (MMIFMM_MOVE_FILE_WIN_ID))
        {
            MMK_CloseWin (MMIFMM_MOVE_FILE_WIN_ID);
        }

        //SCI_TRACE_LOW:"[MMIFMM]:GetMoveTargetPathCallBack failed"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_14311_112_2_18_2_21_5_583, (uint8*) "");
    }

    //释放列表
    MMIAPIFILEARRAY_Destroy (&file_array);
    file_array = PNULL;
}
/*****************************************************************************/
//  Description : handle move file win msg
//  Global resource dependence : none
//  Author: juan.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMoveFileWinMsg (
    MMI_WIN_ID_T        win_id,
    MMI_MESSAGE_ID_E    msg_id,
    DPARAM              param
)
{
    MMI_RESULT_E                   result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T                 ctrl_id = MMIFMM_UDISK_LISTBOX_CTRL_ID;
    uint16                       file_ret = SFS_NO_ERROR;
    static uint16               cur_index = 0;
    MMI_STRING_T              prompt_str1 = {0};
    MMI_STRING_T              prompt_str2 = {0};
    MMI_WIN_ID_T             query_win_id = MMIFMM_MOVE_FILE_QUERY_WIN_ID;
    MMIFMM_MOVEWIN_DATA_INFO_T*   move_data_ptr = (MMIFMM_MOVEWIN_DATA_INFO_T*) MMK_GetWinUserData (win_id);/*lint !e64 */

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
#ifdef MID_SOFTKEY_USE_IMG_SUPPORT
		GUIWIN_SeSoftkeytButtonIconId(win_id, IMAGE_NULL, 1, TRUE);
#endif
//        if (MMI_RESULT_FALSE == MoveOpenCtrlFunc (win_id))
//        {
//            MMK_CloseWin (win_id);
//            break;
//         }

        MMK_SendMsg (win_id, MSG_FMM_OPEN_WIN_CHOOSE_PATH, PNULL);
        break;

    case MSG_FMM_OPEN_WIN_CHOOSE_PATH:
        MMIFMM_GetSelectTargetPath (GetMoveTargetPathCallBack);
        break;

    case MSG_PROMPTWIN_CANCEL:       //取消当前文件移动，到下一个文件
        MMIPUB_CloseQuerytWin (&query_win_id);

        if (PNULL != move_data_ptr->s_move_handle_ptr)
        {
            APPFILE_MoveClose (move_data_ptr->s_move_handle_ptr);
            move_data_ptr->s_move_handle_ptr = PNULL;
        }

        if (0 == move_data_ptr->list_data_ptr->mark_num)
        {
            MMK_CloseWin (win_id);
            MMIPUB_CloseWaitWin (MMIFMM_FILE_WAITING_WIN_ID);
        }
        else
        {
            MMK_PostMsg (win_id, MSG_FMM_MOVE_MARK_FILE, PNULL, 0);
        }

        break;

    case MSG_PROMPTWIN_OK:    //有需要覆盖的文件或文件夹，走copy & delete流程
        move_data_ptr->s_is_copy_from_move = TRUE;

        if (PNULL == move_data_ptr->s_move_dst_path_ptr)
        {
            //SCI_TRACE_LOW:"[MMIFMM]:HandleMoveFileWinMsg MSG_PROMPTWIN_OK PNULL == s_move_dst_full_path_ptr!"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_14394_112_2_18_2_21_5_586, (uint8*) "");
            break;
        }

        move_data_ptr->s_copy_handle_ptr = MMIFILE_CopyOpen (move_data_ptr->src_full_path_ptr,
                                           move_data_ptr->src_full_pathlen,
                                           move_data_ptr->s_move_dst_path_ptr,
                                           (uint16) MMIAPICOM_Wstrlen (move_data_ptr->s_move_dst_path_ptr));
        MMIPUB_CloseQuerytWin (&query_win_id);

        if (PNULL == move_data_ptr->s_copy_handle_ptr)
        {
            //SCI_TRACE_LOW:"[MMIFMM]:HandleMoveFileWinMsg PNULL == move_data_ptr->s_copy_handle_ptr"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_14404_112_2_18_2_21_5_587, (uint8*) "");
            MMIPUB_CloseWaitWin (MMIFMM_FILE_WAITING_WIN_ID);
            MMIPUB_OpenAlertFailWin (TXT_ERROR);
            MMK_CloseWin (win_id);
            break;
        }

        //overwrite
        MMIFILE_CopyOverwrite (move_data_ptr->s_copy_handle_ptr);
        break;

    case MSG_FMM_COPY_CHOOSE_PATH_RETURN:
        {
            MMIFILE_DEVICE_E dst_dev =  0;
            MMIFILE_DEVICE_E src_dev = MMIAPIFMM_GetDeviceTypeByPath (move_data_ptr->list_data_ptr->pathname, MMIFMM_DEVICE_NAME_LEN);

            if (PNULL == move_data_ptr->s_move_dst_path_ptr)
            {
                //SCI_TRACE_LOW:"[MMIFMM]:HandleMoveFileWinMsg MSG_FMM_COPY_CHOOSE_PATH_RETURN PNULL == s_move_dst_full_path_ptr!"
                SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_14418_112_2_18_2_21_5_588, (uint8*) "");
                break;
            }

#if defined(DRM_SUPPORT) || !defined(MMIFMM_XMOVE_SUPPORT)//如果DRM开启，或XMOVE功能未开启，走COPY & DELETE流程
            {
                move_data_ptr->s_is_copy_from_move = TRUE;
                MMIFMM_CopySelectFileToFolder (move_data_ptr->list_data_ptr, GUILIST_GetCurItemIndex (ctrl_id), move_data_ptr->current_path_ptr);
                MMK_CloseWin (win_id);
                break;
            }
#endif
            dst_dev = MMIAPIFMM_GetDeviceTypeByPath (move_data_ptr->s_move_dst_path_ptr, MMIFMM_DEVICE_NAME_LEN);/*lint !e527*/

            if (src_dev != dst_dev) //不同盘符，走copy&delete流程
            {
                move_data_ptr->s_is_copy_from_move = TRUE;
                MMIFMM_CopySelectFileToFolder (move_data_ptr->list_data_ptr, GUILIST_GetCurItemIndex (ctrl_id), move_data_ptr->current_path_ptr);
                MMK_CloseWin (win_id);
            }
            else
            {
                APPFILE_CreateTask();

                if (0 == move_data_ptr->list_data_ptr->mark_num) //单文件剪切
                {
                    MMK_SendMsg (win_id, MSG_FMM_OPERATE_FILE_BEGIN, PNULL);
                }
                else//有mark文件，走mark流程
                {
                    MMK_SendMsg (win_id, MSG_FMM_MOVE_MARK_FILE, PNULL);
                }
            }
        }
        break;

    case MSG_FMM_FILE_EXIST://文件已经存在， 是否覆盖

        if (MMIAPICC_IsInState (CC_MT_CONNECTING_STATE) || MMIAPICC_IsInState (CC_MO_CONNECTING_STATE))
        {
            MMIAPIFMM_ExitFileOption();
            break;
        }

        prompt_str1.wstr_ptr = move_data_ptr->s_src_data_ptr->file_name;
        prompt_str1.wstr_len = (uint16) MMIAPICOM_Wstrlen (move_data_ptr->s_src_data_ptr->file_name);
        MMI_GetLabelTextByLang (TXT_FMM_REPEAT_REPLACE, &prompt_str2);
        MMIPUB_OpenQueryWinByTextPtr (&prompt_str1, &prompt_str2, IMAGE_PUBWIN_QUERY, &query_win_id, HandleOverWriteQueryWin);
        {
            MMIFMM_QUERYWIN_DATA_T query_win = {0};
            query_win.win_id = MMIFMM_MOVE_FILE_WIN_ID;
            MMIFMM_QueryAddCtrlFunc (query_win_id, &query_win);
        }

        break;

    case MSG_FMM_OPERATE_FILE_BEGIN:
        {
            uint16 max_len = 0;
            uint16 dst_pathlen = 0;
            wchar dst_full_path[MMIFMM_FILE_FULL_PATH_MAX_LEN+1] = {0};
            uint16 dst_full_pathlen = 0;
            uint16 full_path_max_len = 0;

            if (PNULL == move_data_ptr->s_move_dst_path_ptr)
            {
                //SCI_TRACE_LOW:"[MMIFMM]:HandleMoveFileWinMsg MSG_FMM_OPERATE_FILE_BEGIN PNULL == s_move_dst_full_path_ptr!"
                SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_14470_112_2_18_2_21_5_589, (uint8*) "");
                break;
            }

            if (0 == MMIAPICOM_Wstrlen (move_data_ptr->s_move_dst_path_ptr))
            {
                //SCI_TRACE_LOW:"[MMIFMM]:HandleMoveFileWinMsg s_move_dst_full_path_ptr len == 0!"
                SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_14476_112_2_18_2_21_5_590, (uint8*) "");
                break;
            }

            //start move
            MMIFMM_OpenComWaitWin (HandleWaitMoveWinMsg);

            if (0 == move_data_ptr->list_data_ptr->mark_num)
            {
                cur_index = GUILIST_GetCurItemIndex (ctrl_id);
            }
            else
            {
                cur_index = move_data_ptr->s_move_index;
                move_data_ptr->s_move_index++;
            }

            if (cur_index >= GUILIST_GetTotalItemNum (ctrl_id))
            {
                //SCI_TRACE_LOW:"[MMIFMM]:HandleMoveFileWinMsg cur_index = %d"
                SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_14492_112_2_18_2_21_5_591, (uint8*) "d", cur_index);
                break;
            }

            if (move_data_ptr->list_data_ptr->pathname_len + move_data_ptr->list_data_ptr->data[cur_index]->filename_len + 1 > MMIFMM_FULL_FILENAME_LEN)
            {
                //SCI_TRACE_LOW:"[MMIFMM]:total name too long %d"
                SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_14497_112_2_18_2_21_5_592, (uint8*) "d", move_data_ptr->list_data_ptr->pathname_len + move_data_ptr->list_data_ptr->data[cur_index]->filename_len + 1);
                MMK_CloseWin (win_id);
                MMIPUB_CloseWaitWin (MMIFMM_FILE_WAITING_WIN_ID);
                break;
            }

            SCI_MEMSET (move_data_ptr->src_full_path_ptr, 0, sizeof (wchar) * (MMIFMM_FULL_FILENAME_LEN + 1));
            MMIAPICOM_Wstrncpy (move_data_ptr->src_full_path_ptr, move_data_ptr->list_data_ptr->pathname, move_data_ptr->list_data_ptr->pathname_len);
            MMIAPICOM_Wstrncat (move_data_ptr->src_full_path_ptr, L"\\", 1);
            MMIAPICOM_Wstrncat (move_data_ptr->src_full_path_ptr, move_data_ptr->list_data_ptr->data[cur_index]->filename_ptr, move_data_ptr->list_data_ptr->data[cur_index]->filename_len);
            move_data_ptr->src_full_pathlen = (uint16) MMIAPICOM_Wstrlen (move_data_ptr->src_full_path_ptr);
            dst_pathlen = (uint16) MMIAPICOM_Wstrlen (move_data_ptr->s_move_dst_path_ptr);
            MMIAPICOM_Wstrncpy (dst_full_path, move_data_ptr->s_move_dst_path_ptr, dst_pathlen);
            MMIAPICOM_Wstrncat (dst_full_path, L"\\", 1);
            MMIAPICOM_Wstrncat (dst_full_path, move_data_ptr->list_data_ptr->data[cur_index]->filename_ptr, move_data_ptr->list_data_ptr->data[cur_index]->filename_len);
            dst_full_pathlen = (uint16) MMIAPICOM_Wstrlen (dst_full_path);

            full_path_max_len = (uint16) MAX (move_data_ptr->src_full_pathlen, dst_full_pathlen);
            max_len = (uint16) MAX (move_data_ptr->src_full_pathlen, dst_pathlen);

            /*判断源文件路径和目标路径是否合法
            以下两种情况不能move:
            1.源路径等于目标路径比如:把d:\photos move到 d:\photos下
            2.源文件是目标路径中的子文件 比如:d:\1.txt move 到 d:下*/
            if ( (0 == MMIAPICOM_Wstrncmp (move_data_ptr->src_full_path_ptr, move_data_ptr->s_move_dst_path_ptr, max_len))
                    || (0 == MMIAPICOM_Wstrncmp (move_data_ptr->src_full_path_ptr, dst_full_path, full_path_max_len)))
            {
                MMIPUB_OpenAlertWarningWin (TXT_FMM_MOVE_SOURCE_EQU_DEST);
                //if(move_data_ptr->list_data_ptr->mark_num > 0)
                //{
                //    MMK_SendMsg(win_id, MSG_FMM_MOVE_MARK_FILE, PNULL);
                //}
                //else
                {
                    MMK_CloseWin (win_id);
                    MMIPUB_CloseWaitWin (MMIFMM_FILE_WAITING_WIN_ID);
                }
                break;
            }
            //目标文件是源文件的子文件
            else if (MMIFMM_IsPath1ContainPath2 (move_data_ptr->s_move_dst_path_ptr,
                                                 dst_pathlen,
                                                 move_data_ptr->src_full_path_ptr,
                                                 move_data_ptr->src_full_pathlen))
            {
                MMIPUB_OpenAlertWarningWin (TXT_FMM_MOVE_SOURCE_EQU_DEST);
                //if(move_data_ptr->list_data_ptr->mark_num > 0)
                //{
                //    MMK_SendMsg(win_id, MSG_FMM_MOVE_MARK_FILE, PNULL);
                //}
                //else
                {
                    MMK_CloseWin (win_id);
                    MMIPUB_CloseWaitWin (MMIFMM_FILE_WAITING_WIN_ID);
                }
                break;
            }

#ifdef DRM_SUPPORT
            else if (MMIAPIDRM_IsDRMFile (SFS_INVALID_HANDLE, move_data_ptr->src_full_path_ptr))
            {
                MMK_PostMsg (win_id, MSG_FMM_MOVE_MARK_FILE, PNULL, 0);
                break;
            }

#endif
            else
            {
                SCI_MEMSET (move_data_ptr->s_src_data_ptr, 0, sizeof (MMIFMM_MOVE_DATA_INFO_T));
                MMIAPICOM_Wstrncpy (move_data_ptr->s_src_data_ptr->path_name,
                                    move_data_ptr->list_data_ptr->pathname,
                                    move_data_ptr->list_data_ptr->pathname_len
                                   );
                MMIAPICOM_Wstrncpy (move_data_ptr->s_src_data_ptr->file_name,
                                    move_data_ptr->list_data_ptr->data[cur_index]->filename_ptr,
                                    move_data_ptr->list_data_ptr->data[cur_index]->filename_len
                                   );
#ifdef MMIFMM_XMOVE_SUPPORT
                move_data_ptr->s_move_handle_ptr = MMIFILE_MoveOpen (move_data_ptr->s_src_data_ptr->path_name,
                                                   move_data_ptr->s_src_data_ptr->file_name,
                                                   move_data_ptr->s_move_dst_path_ptr,
                                                   move_data_ptr->s_src_data_ptr->file_name,
                                                   move_data_ptr->list_data_ptr->data[cur_index]->type);
#else
                SCI_TRACE_LOW ("HandleMoveFileWinMsg error! not support but in xmove flow");
#endif
                MMIDEFAULT_AllowTurnOffBackLight (FALSE);//剪切过程不允许锁屏

                if (PNULL == move_data_ptr->s_move_handle_ptr)
                {
                    //move fail
                    MMIPUB_CloseWaitWin (MMIFMM_FILE_WAITING_WIN_ID);
                    MMIPUB_OpenAlertFailWin (TXT_ERROR);
                    MMK_CloseWin (win_id);
                }
            }
        }
        break;

    case MSG_FMM_OPERATE_ONE_FILE_RETURN:
        move_data_ptr->s_is_copy_from_move = FALSE;
        file_ret  = * ( (uint16 *) param);

        if (cur_index >= GUILIST_GetTotalItemNum (ctrl_id))
        {
            //SCI_TRACE_LOW:"[MMIFMM]:HandleMoveFileWinMsg cur_index = %d"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_14582_112_2_18_2_21_6_593, (uint8*) "d", cur_index);
            break;
        }

        if (SFS_ERROR_NONE == file_ret)
        {
            if (PNULL != move_data_ptr->s_copy_handle_ptr)
            {
                move_data_ptr->s_delete_handle_ptr = MMIFILE_DeleteOpen (move_data_ptr->src_full_path_ptr, move_data_ptr->src_full_pathlen);

                if (PNULL == move_data_ptr->s_delete_handle_ptr)
                {
                    //move fail
                    SCI_TRACE_LOW ("[mmifmm]:handlemovefile move_data_ptr->s_delete_handle_ptr == PNULL");
                    MMIPUB_CloseWaitWin (MMIFMM_FILE_WAITING_WIN_ID);
                    MMIPUB_OpenAlertFailWin (TXT_ERROR);
                    MMK_CloseWin (win_id);
                }

                APPFILE_CopyClose (move_data_ptr->s_copy_handle_ptr);
                move_data_ptr->s_copy_handle_ptr = PNULL;
            }
            else
            {
                if (move_data_ptr->mark_num == move_data_ptr->list_data_ptr->mark_num) //没有找到下一个文件，结束操作
                {
                    move_data_ptr->mark_num = 0;

                    if (PNULL != move_data_ptr->s_move_handle_ptr)
                    {
                        APPFILE_MoveClose (move_data_ptr->s_move_handle_ptr);
                        move_data_ptr->s_move_handle_ptr = PNULL;
                    }

                    MMK_PostMsg (MMIFMM_MAIN_WIN_ID, MSG_FMM_MOVE_END_CNF, PNULL, 0);
                    MMK_CloseWin (win_id);
                    MMIPUB_CloseWaitWin (MMIFMM_FILE_WAITING_WIN_ID);
                }
                else
                {
                    //move success, get next to move
                    MMK_PostMsg (win_id, MSG_FMM_MOVE_MARK_FILE, PNULL, 0);
                }
            }
        }
        else if (SFS_ERROR_ABORT == file_ret)
        {
            //SCI_TRACE_LOW:"[MMIFMM]:HandleMoveFileWinMsg stopped by user!"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_14617_112_2_18_2_21_6_594, (uint8*) "");
            MMK_CloseWin (win_id);
            MMIPUB_CloseWaitWin (MMIFMM_FILE_WAITING_WIN_ID);
        }
        else //if(SFS_ERROR_LONG_DEPTH == file_ret)
        {
            MMIFILE_DEVICE_E filedev = MMIAPIFMM_GetDeviceTypeByPath (move_data_ptr->s_move_dst_path_ptr, MMIFMM_DEVICE_NAME_LEN);

            MMIAPIFMM_PromptFileErrorType (PNULL, (uint32) file_ret, filedev);
            MMK_CloseWin (win_id);
            MMIPUB_CloseWaitWin (MMIFMM_FILE_WAITING_WIN_ID);
        }

        break;

    case MSG_FMM_MOVE_MARK_FILE:
        {
            uint16  total_num = GUILIST_GetTotalItemNum (ctrl_id);
            uint16  i         = 0;


            if (0 == move_data_ptr->list_data_ptr->mark_num || i >= (move_data_ptr->list_data_ptr->file_num + move_data_ptr->list_data_ptr->folder_num))
            {
                //SCI_TRACE_LOW:"[MMIFMM]:HandleMoveFileWinMsg case MSG_FMM_MOVE_MARK_FILE 0 == move_data_ptr->list_data_ptr->mark_num"
                SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_14639_112_2_18_2_21_6_595, (uint8*) "");
                break;
            }

            //找完了所有的mark文件,有可能会不移动任何一个文件，则收不到MSG_FMM_OPERATE_ONE_FILE_RETURN消息，这里也要处理一下
            if (move_data_ptr->mark_num == move_data_ptr->list_data_ptr->mark_num)
            {
                move_data_ptr->mark_num = 0;

                if (PNULL != move_data_ptr->s_move_handle_ptr)
                {
                    APPFILE_MoveClose (move_data_ptr->s_move_handle_ptr);
                    move_data_ptr->s_move_handle_ptr = PNULL;
                }

                MMK_PostMsg (MMIFMM_MAIN_WIN_ID, MSG_FMM_MOVE_END_CNF, PNULL, 0);
                MMK_CloseWin (win_id);
                MMIPUB_CloseWaitWin (MMIFMM_FILE_WAITING_WIN_ID);
                break;
            }

            for (i = move_data_ptr->s_move_index; i < total_num; i++)
            {
                if (MMIFMM_FILE_STATE_CHECKED == move_data_ptr->list_data_ptr->data[i]->state)
                {
                    //找到一个，记录位置,开始操作
                    move_data_ptr->s_move_index = i;
                    move_data_ptr->mark_num++;
                    MMK_SendMsg (win_id, MSG_FMM_OPERATE_FILE_BEGIN, PNULL);
                    break;
                }
            }
        }
        break;

    case MSG_FMM_DELETE_END_CNF:
#ifdef  VIDEOTHUMBNAIL_SUPPORT//cr130686
		{
			wchar           dir_name[MMIFILE_FULL_PATH_MAX_LEN+1] = {0};
			uint16          dir_name_len = 0;
			if(MMIAPIFMM_SplitFullPath(move_data_ptr->s_delete_handle_ptr->delete_full_path, 
				move_data_ptr->s_delete_handle_ptr->delete_path_len,
				PNULL, PNULL,
				dir_name, &dir_name_len,
				NULL, NULL))
			{
				if (MMIFMM_IsDefaultVideoFolder(dir_name,dir_name_len))
				{
					MMIFMM_VideoThumbnailDeleteRelatedFile(move_data_ptr->s_delete_handle_ptr->delete_full_path);
				}
			}
		}
#endif
        if (move_data_ptr->mark_num == move_data_ptr->list_data_ptr->mark_num)
        {
            MMIFILE_DeleteClose (move_data_ptr->s_delete_handle_ptr);
            move_data_ptr->s_delete_handle_ptr = PNULL;
            MMK_PostMsg (MMIFMM_MAIN_WIN_ID, MSG_FMM_MOVE_END_CNF, PNULL, 0);
            MMIPUB_CloseWaitWin (MMIFMM_FILE_WAITING_WIN_ID);
            MMK_CloseWin (win_id);
        }
        else
        {
            MMK_PostMsg (win_id, MSG_FMM_MOVE_MARK_FILE, PNULL, 0);
        }

        break;

    case MSG_FMM_ABORT:

        if (PNULL != move_data_ptr->s_move_handle_ptr)
        {
            APPFILE_AbortMove (move_data_ptr->s_move_handle_ptr);
            move_data_ptr->s_move_handle_ptr = PNULL;
        }

        if (PNULL != move_data_ptr->s_copy_handle_ptr)
        {
            APPFILE_AbortCopy (move_data_ptr->s_copy_handle_ptr);
            move_data_ptr->s_copy_handle_ptr = PNULL;
        }

        break;

    case MSG_APP_CANCEL:
        MMIPUB_CloseWaitWin (MMIFMM_FILE_WAITING_WIN_ID);
        MMK_CloseWin (MMIFMM_QUERY_WIN_ID);
        MMK_CloseWin (win_id);
        break;

    case MSG_CLOSE_WINDOW:
        cur_index = 0;
        MMIDEFAULT_AllowTurnOffBackLight (TRUE);

        if (PNULL != move_data_ptr->s_copy_handle_ptr)
        {
            APPFILE_CopyClose (move_data_ptr->s_copy_handle_ptr);
            move_data_ptr->s_copy_handle_ptr = PNULL;
        }

        if (PNULL != move_data_ptr->s_move_handle_ptr)
        {
            APPFILE_MoveClose (move_data_ptr->s_move_handle_ptr);
            move_data_ptr->s_move_handle_ptr = PNULL;
        }

        if (PNULL != move_data_ptr->s_delete_handle_ptr)
        {
            MMIFILE_DeleteClose (move_data_ptr->s_delete_handle_ptr);
            move_data_ptr->s_delete_handle_ptr = PNULL;
        }

        MoveCloseCtrlFunc (win_id);
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }

    return result;
}
/*****************************************************************************/
//  Description : move file call back
//  Global resource dependence : none
//  Author: juan.wu
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIFMM_MoveCallBack (uint16 result)
{
    if (MMK_IsOpenWin (MMIFMM_MOVE_FILE_WIN_ID))
    {
        MMK_PostMsg (MMIFMM_MOVE_FILE_WIN_ID, MSG_FMM_OPERATE_ONE_FILE_RETURN, &result, sizeof (result));
    }
    else
    {
        MMK_PostMsg (MMIFMM_MAIN_WIN_ID, MSG_FMM_MOVE_END_CNF, PNULL, 0);
    }
}
/*****************************************************************************/
//  Description : has exist file when move
//  Global resource dependence : none
//  Author: juan.wu
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIFMM_MoveFileExist (void)
{
    //SCI_TRACE_LOW:"[MMIFMM] MMIAPIFMM_MoveFileExist enter"
    SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_14783_112_2_18_2_21_6_596, (uint8*) "");

    if (MMK_IsOpenWin (MMIFMM_MOVE_FILE_WIN_ID))
    {
        MMK_PostMsg (MMIFMM_MOVE_FILE_WIN_ID, MSG_FMM_FILE_EXIST, PNULL, 0);
    }

}
/*****************************************************************************/
//  Description : copy is from move
//  Global resource dependence : none
//  Author: juan.wu
//  Note:如果从move呼叫copy,则为TRUE
/*****************************************************************************/
PUBLIC BOOLEAN MMIFMM_IsCopyFromMove (void)
{
    MMIFMM_MOVEWIN_DATA_INFO_T *move_data_ptr = (MMIFMM_MOVEWIN_DATA_INFO_T*) MMK_GetWinUserData (MMIFMM_MOVE_FILE_WIN_ID);

    if (PNULL != move_data_ptr)
    {
        return move_data_ptr->s_is_copy_from_move;
    }
    else
    {
        return FALSE;
    }
}

/*****************************************************************************/
//  Description : set move index
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
//PUBLIC void MMIFMM_SetMoveIndex(uint16 move_index)
//{
//    MMIFMM_MOVEWIN_DATA_INFO_T *move_data_ptr = (MMIFMM_MOVEWIN_DATA_INFO_T*) MMK_GetWinAddDataPtr (MMIFMM_MOVE_FILE_WIN_ID);
//    if (PNULL != move_data_ptr)
//    {
//        move_data_ptr->s_move_index = move_index;
//    }
// }

/*****************************************************************************/
//  Description : set target path name
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIFMM_SetTargetPathName (uint16 current_index, MMIFMM_PATH_INFO_T *path_info)
{
    if (PNULL != path_info)
    {
        MMIFMM_MOVEWIN_DATA_INFO_T *move_data_ptr = (MMIFMM_MOVEWIN_DATA_INFO_T*) MMK_GetWinUserData (MMIFMM_MOVE_FILE_WIN_ID);

        if (PNULL != move_data_ptr)
        {
            move_data_ptr->s_move_index = current_index;
            MMIAPICOM_Wstrncpy (path_info->pathname, move_data_ptr->s_move_dst_path_ptr, MMIAPICOM_Wstrlen (move_data_ptr->s_move_dst_path_ptr));
            path_info->pathname_len = (uint16) MMIAPICOM_Wstrlen (path_info->pathname);
        }
    }
}
/*****************************************************************************/
//  Description : wait move window message
//  Global resource dependence :
//  Author: juan.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleWaitMoveWinMsg (
    MMI_WIN_ID_T        win_id,
    MMI_MESSAGE_ID_E    msg_id,
    DPARAM              param
)
{
    MMI_RESULT_E                result = MMI_RESULT_TRUE;

    //SCI_TRACE_LOW:"[MMIFMM]: HandleWaitMoveWinMsg, win_id =%d, msg_id = 0x%x."
    SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_14811_112_2_18_2_21_6_597, (uint8*) "dd", win_id, msg_id);

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        result = MMIPUB_HandleWaitWinMsg (win_id, msg_id, param);
        break;

    case MSG_CTL_OK:
    case MSG_APP_OK:
        // 不处理
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin (win_id);
        MMK_PostMsg (MMIFMM_MOVE_FILE_WIN_ID, MSG_FMM_ABORT, PNULL, 0);
        break;
    case MSG_CLOSE_WINDOW:
        //SCI_TRACE_LOW:"[MMIFMM]:HandleWaitMoveWinMsg abort!! moveS"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_14829_112_2_18_2_21_6_598, (uint8*) "");
        break;
    default:
        result = MMIPUB_HandleWaitWinMsg (win_id, msg_id, param);
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
LOCAL MMI_RESULT_E MoveOpenCtrlFunc (MMI_WIN_ID_T win_id)
{
    MMIFMM_MOVEWIN_DATA_INFO_T *win_data_p = (MMIFMM_MOVEWIN_DATA_INFO_T*) MMK_GetWinAddDataPtr (win_id);

    MMK_SetWinUserData (win_id, (void *) win_data_p);

    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
// Description : close ctrl func
// Global resource dependence :
// Author:  jian.ma
// Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E MoveCloseCtrlFunc (MMI_WIN_ID_T win_id)
{
    MMIFMM_MOVEWIN_DATA_INFO_T *win_data_p = (MMIFMM_MOVEWIN_DATA_INFO_T*) MMK_GetWinUserData (win_id);

    if (PNULL != win_data_p)
    {
        SCI_FREE (win_data_p->s_src_data_ptr);
        SCI_FREE (win_data_p->src_full_path_ptr);
        SCI_FREE (win_data_p->s_move_dst_path_ptr);
        SCI_FREE (win_data_p);
    }

    return MMI_RESULT_TRUE;
}
//#endif