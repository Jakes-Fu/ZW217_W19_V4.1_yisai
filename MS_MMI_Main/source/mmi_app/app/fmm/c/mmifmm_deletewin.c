/*****************************************************************************
** File Name:      mmifmm_deletewin.c                                        *
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

#define _MMIFMM_DELETEWIN_C_


/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_app_fmm_trc.h"
#include "mmifmm_deletewin.h"
#include "mmifmm_mainwin.h"
#include "mmifmm_id.h"
#include "mmifmm_text.h"
#include "mmi_filetask.h"
#include "mmifmm_comwin.h"
#include "mmipub.h"
#include "mmifmm_comfunc.h"
#ifdef DRM_SUPPORT
#include "mmidrm_export.h"
#endif
#include "mmi_appmsg.h"
#include "window_parse.h"
#ifdef  VIDEOTHUMBNAIL_SUPPORT
#include "mmifmm_vthumbnail.h"
#endif
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#define MMI_FMM_DELETE_ALLFILES_FLAG    (MMIFMM_FILE_MAX_NUM + 1)

/**--------------------------------------------------------------------------*
 **                         TYPE DEFINITION                                  *
 **--------------------------------------------------------------------------*/

typedef struct
{
    MMIFMM_DATA_INFO_T* list_data_ptr;
    MMIFMM_PATH_INFO_T* current_path_ptr;
    uint16              mark_num;
    wchar               cur_pathname [MMIFMM_PATHNAME_LEN +1];
} MMIFMM_DELQUERY_DATA_T;


typedef struct
{
    MMIFMM_DATA_INFO_T* list_data_ptr;
    MMIFMM_PATH_INFO_T* current_path_ptr;
    MMIFMM_COPY_DEL_PATH_DATA_INFO_T *s_copy_del_path_data_ptr;
    APP_DELETE_HANDLE_T   *s_delete_handle_ptr;
    uint32                  s_resoved_num;
    BOOLEAN                 s_need_update;
    BOOLEAN                 s_user_stop_del;
    BOOLEAN                 s_user_stop_copy;
    uint32                  s_copydel_opt_state;
} MMIFMM_DELWIN_DATA_T;
/**-------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : handle fmm file del window message
//  Global resource dependence :
//  Author: xinrui.wang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDelFileFolderWinMsg (
    MMI_WIN_ID_T      win_id,
    MMI_MESSAGE_ID_E  msg_id,
    DPARAM            param
);

/*****************************************************************************/
//  Description : wait deleting window message
//  Global resource dependence :
//  Author: robert.lu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleWaitDeleteWinMsg (
    MMI_WIN_ID_T      win_id,
    MMI_MESSAGE_ID_E  msg_id,
    DPARAM            param
);
/*****************************************************************************/
//  Description : HandleFmmQueryWin
//  Global resource dependence :
//      Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDelQueryWin (MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
#ifdef DRM_SUPPORT
/*****************************************************************************/
//  Description : is delete drm file
//  Global resource dependence :
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsDeleteDrmFile (
    MMIFMM_DATA_INFO_T  *list_data_ptr,
    MMIFMM_PATH_INFO_T  *current_path_ptr,
    uint16 mark_num,
    wchar *cur_pathname
);
#endif
/*****************************************************************************/
// Description : open ctrl func
// Global resource dependence :
// Author:  jian.ma
// Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E DelQueryOpenCtrlFunc (
    MMI_WIN_ID_T win_id,
    MMIFMM_DELQUERY_DATA_T *query_data
);

/*****************************************************************************/
// Description : close ctrl func
// Global resource dependence :
// Author:  jian.ma
// Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E DelQueryCloseCtrlFunc (MMI_WIN_ID_T win_id);


/*****************************************************************************/
// Description : open ctrl func
// Global resource dependence :
// Author:  jian.ma
// Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E DelWinOpenCtrlFunc (MMI_WIN_ID_T win_id);

/*****************************************************************************/
// Description : close ctrl func
// Global resource dependence :
// Author:  jian.ma
// Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E DelWinCloseCtrlFunc (MMI_WIN_ID_T win_id);


/*****************************************************************************/
//  Description : copy del exit
//  Global resource dependence :
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL void FmmCopyDelExit (MMI_WIN_ID_T win_id);

// the del file window for FMM
WINDOW_TABLE (MMIFMM_DEL_SELECTFILE_WIN_TAB) =
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC ( (uint32) HandleDelFileFolderWinMsg),
    WIN_ID (MMIFMM_DEL_SELECT_FILE_WIN_ID),
    WIN_STYLE (WS_HAS_TRANSPARENT),
    WIN_MOVE_STYLE (MOVE_FORBIDDEN),
    WIN_SOFTKEY (TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};


/*****************************************************************************/
//  Description : handle fmm file del window message
//  Global resource dependence :
//  Author: xinrui.wang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDelFileFolderWinMsg (
    MMI_WIN_ID_T      win_id,
    MMI_MESSAGE_ID_E  msg_id,
    DPARAM            param
)
{
    MMI_RESULT_E            result = MMI_RESULT_TRUE;
    uint32                  full_path_len = 0;
    uint16                  i = 0;
    uint16                  file_ret = SFS_ERROR_NONE;
    MMIFILE_DEVICE_E        filedev = MMI_DEVICE_UDISK;
    MMIFMM_DELWIN_DATA_T    *win_data_p = (MMIFMM_DELWIN_DATA_T*) MMK_GetWinUserData (win_id);

    //SCI_TRACE_LOW:"MMIFMM: HandleDelFileFolderWinMsg, win_id =%d, msg_id = 0x%x."
    SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_8260_112_2_18_2_20_52_475, (uint8*) "dd", win_id, msg_id);

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
#ifdef MID_SOFTKEY_USE_IMG_SUPPORT
		GUIWIN_SeSoftkeytButtonIconId(win_id, IMAGE_NULL, 1, TRUE);
#endif
        if (MMI_RESULT_FALSE == DelWinOpenCtrlFunc (win_id))
        {
            MMK_CloseWin (win_id);
            break;
        }

        win_data_p = (MMIFMM_DELWIN_DATA_T*) MMK_GetWinUserData (win_id);
        win_data_p->s_resoved_num = 0;
        win_data_p->s_need_update = FALSE;
        APPFILE_CreateTask();
        /* the reason of that MMK_SendMsg instead of MMK_PostMsg is avoid of screen pattern glint.
        certainly, the mmk_postmsg is OK, too. */
        MMK_SendMsg (win_id, MSG_FMM_OPEN_WIN_CHOOSE_PATH, PNULL);
        break;

    case MSG_FMM_OPEN_WIN_CHOOSE_PATH:
        MMIFMM_OpenComWaitWin (HandleWaitDeleteWinMsg);

        if (win_data_p->s_copy_del_path_data_ptr->mark_flag > 0)
        {
            //get a mask file or folder to delete
            if (MMI_FMM_DELETE_ALLFILES_FLAG == win_data_p->s_copy_del_path_data_ptr->mark_flag)
            {
                //delete current file or folder
                MMK_PostMsg (win_id, MSG_FMM_OPERATE_FILE_BEGIN, PNULL, 0);
            }
            else
            {
                MMK_PostMsg (win_id, MSG_FMM_SELECT_MARK_FILE, PNULL, 0);
            }
        }
        else
        {
            //delete current file or folder
            MMK_PostMsg (win_id, MSG_FMM_OPERATE_FILE_BEGIN, PNULL, 0);
        }

        break;

    case MSG_FMM_SELECT_MARK_FILE:

        //get a mask file or folder to delete
        if (win_data_p->s_user_stop_del)
        {
            MMIPUB_CloseWaitWin (MMIFMM_FILE_WAITING_WIN_ID);
            MMK_CloseWin (win_id);
            break;
        }

        for (i = win_data_p->s_copy_del_path_data_ptr->cur_mark_num; i < (uint16) (win_data_p->list_data_ptr->file_num + win_data_p->list_data_ptr->folder_num); i++)
        {
            if (MMIFMM_FILE_STATE_UNCHECKED != win_data_p->list_data_ptr->data[i]->state)
            {
                break;
            }
        }

        if (i >= (uint16) (win_data_p->list_data_ptr->file_num + win_data_p->list_data_ptr->folder_num))
        {
            MMIPUB_CloseWaitWin (MMIFMM_FILE_WAITING_WIN_ID);

            if (win_data_p->s_resoved_num > 0)
            {
                MMIPUB_OpenAlertWarningWin (TXT_FMM_RESERVE_FOLDER);
            }

            //fixed by liyan.zhu for bugzilla 2760
            MMK_CloseWin (win_id);
        }
        else
        {
            win_data_p->s_copy_del_path_data_ptr->cur_mark_num = (uint16) (i + 1);
            SCI_MEMSET (win_data_p->s_copy_del_path_data_ptr->cur_pathname, 0, (sizeof (wchar) * (MMIFMM_PATHNAME_LEN + 1)));
            SCI_MEMSET (win_data_p->s_copy_del_path_data_ptr->copyfile_name, 0, (sizeof (wchar) * (MMIFMM_PATHNAME_LEN + 1)));

            if (win_data_p->list_data_ptr->path_is_valid > 0)
            {
                MMIAPICOM_Wstrncpy (win_data_p->s_copy_del_path_data_ptr->cur_pathname, win_data_p->list_data_ptr->data[i]->filename_ptr,
                                    win_data_p->list_data_ptr->data[i]->filename_len);
            }
            else
            {
                MMIFMM_CombineToFileName (win_data_p->current_path_ptr->pathname, win_data_p->current_path_ptr->pathname_len,
                                          win_data_p->list_data_ptr->data[i]->filename_ptr, win_data_p->list_data_ptr->data[i]->filename_len,
                                          win_data_p->s_copy_del_path_data_ptr->cur_pathname, MMIFMM_PATHNAME_LEN);

            }

            //send begin delete msg
            MMK_PostMsg (win_id, MSG_FMM_OPERATE_FILE_BEGIN, PNULL, 0);
        }

        break;

    case MSG_FMM_OPERATE_FILE_BEGIN:
        //begin to delete
        full_path_len = MMIAPICOM_Wstrlen (win_data_p->s_copy_del_path_data_ptr->cur_pathname);
        win_data_p->s_delete_handle_ptr = MMIFILE_DeleteOpen (win_data_p->s_copy_del_path_data_ptr->cur_pathname, full_path_len);

        if (PNULL == win_data_p->s_delete_handle_ptr)
        {
            //move fail
            SCI_TRACE_LOW ("[mmifmm]:deletefilefolder win_data_p->s_delete_handle_ptr == PNULL");
            MMIPUB_CloseWaitWin (MMIFMM_FILE_WAITING_WIN_ID);
            MMIPUB_OpenAlertFailWin (TXT_ERROR);
            MMK_CloseWin (win_id);
        }

        break;
    case MSG_FMM_DELETE_END_CNF:
        //delete one circle ok
        win_data_p->s_need_update = TRUE;//cr237690
        win_data_p->s_delete_handle_ptr = PNULL;
        file_ret  = * ( (uint16 *) param);

        if (SFS_ERROR_NONE == file_ret || SFS_ERROR_NOT_EXIST == file_ret)  //MS00194050
        {
#ifdef  VIDEOTHUMBNAIL_SUPPORT//cr130686
			wchar           dir_name[MMIFILE_FULL_PATH_MAX_LEN+1] = {0};
			uint16          dir_name_len = 0;
			if(MMIAPIFMM_SplitFullPath(win_data_p->s_copy_del_path_data_ptr->cur_pathname, 
				full_path_len,
				PNULL, PNULL,
				dir_name, &dir_name_len,
				NULL, NULL))
			{
				if (MMIFMM_IsDefaultVideoFolder(dir_name,dir_name_len))
				{
					MMIFMM_VideoThumbnailDeleteRelatedFile(win_data_p->s_copy_del_path_data_ptr->cur_pathname);
				}
			}
#endif
            //win_data_p->s_need_update = TRUE;
            if (0 == win_data_p->s_copy_del_path_data_ptr->mark_flag)
            {
                //not file or folder to delete

                if (!win_data_p->s_user_stop_del)
                {
                    if (win_data_p->s_resoved_num > 0)
                    {
                        MMIPUB_OpenAlertWarningWin (TXT_FMM_RESERVE_FOLDER);
                    }

//arvin zhang fixed for cr221394---start---
#ifdef MMI_FMM_MINI_SUPPORT
                    MMIPUB_OpenAlertSuccessWin (TXT_DELETED);
#endif
                }

#ifdef MMI_FMM_MINI_SUPPORT
                else//user stop deleting file operation
                {
                    MMIPUB_OpenAlertSuccessWin (TXT_USER_CANCEL);
                }

#endif
//arvin zhang fixed for cr221394---end---
                MMIPUB_CloseWaitWin (MMIFMM_FILE_WAITING_WIN_ID);
                MMK_CloseWin (win_id);
            }
            else if (MMI_FMM_DELETE_ALLFILES_FLAG == win_data_p->s_copy_del_path_data_ptr->mark_flag)
            {
                MMIAPIFMM_CreateDirectory (win_data_p->s_copy_del_path_data_ptr->cur_pathname);
                //fixed by liyan.zhu for bugzilla 2760
#if 0
                FmmAlertCopyDelResult (MMIFMM_OPT_DEL);
#endif
                MMIPUB_CloseWaitWin (MMIFMM_FILE_WAITING_WIN_ID);
                MMK_CloseWin (win_id);
            }
            else
            {
                //delete next masked file or folder
                MMK_PostMsg (win_id, MSG_FMM_SELECT_MARK_FILE, PNULL, 0);
            }
        }
        else
        {
            //delete error, stop delete
            //SCI_TRACE_LOW:"FMM_DEL: MSG_FMM_DELETE_END_CNF file_ret = %d"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_8379_112_2_18_2_20_52_476, (uint8*) "d", file_ret);

            if (SFS_ERROR_ABORT != file_ret)
            {
                filedev = MMIAPIFMM_GetDeviceTypeByPath (win_data_p->current_path_ptr->pathname, MMIFILE_DEVICE_LEN);

                MMIAPIFMM_PromptFileErrorType (PNULL, (uint32) file_ret, filedev);
                //MMIPUB_OpenAlertFailWin(TXT_ERROR);
            }

            MMIPUB_CloseWaitWin (MMIFMM_FILE_WAITING_WIN_ID);
            MMK_CloseWin (win_id);
        }

        break;
    case MSG_CLOSE_WINDOW:
        FmmCopyDelExit (win_id);
        DelWinCloseCtrlFunc (win_id);
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }

    return (result);
}


/*****************************************************************************/
//  Description : wait deleting window message
//  Global resource dependence :
//  Author: robert.lu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleWaitDeleteWinMsg (
    MMI_WIN_ID_T      win_id,
    MMI_MESSAGE_ID_E  msg_id,
    DPARAM            param
)
{
    MMI_RESULT_E                result = MMI_RESULT_TRUE;
    MMIFMM_DELWIN_DATA_T       *win_data_p = (MMIFMM_DELWIN_DATA_T*) MMK_GetWinUserData (MMIFMM_DEL_SELECT_FILE_WIN_ID);
    SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_8195_112_2_18_2_20_52_473, (uint8*) "dd", win_id, msg_id);

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        win_data_p->s_user_stop_del = FALSE;
        result = MMIPUB_HandleWaitWinMsg (win_id, msg_id, param);
        break;
    case MSG_CTL_OK:
    case MSG_APP_OK:
        // 不处理
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:

        if (!win_data_p->s_user_stop_del)
        {
            win_data_p->s_user_stop_del = TRUE;
            MMIFILE_AbortDelete (win_data_p->s_delete_handle_ptr);
        }

        break;
    case MSG_APP_RED:
        if (!win_data_p->s_user_stop_del)//CR200474
        {
            win_data_p->s_user_stop_del = TRUE;
            MMIFILE_AbortDelete (win_data_p->s_delete_handle_ptr);
        }
        //result = MMIPUB_HandleWaitWinMsg (win_id, msg_id, param);
        break;
    case MSG_CLOSE_WINDOW:
        win_data_p->s_user_stop_del = TRUE;

        //SCI_TRACE_LOW:"[FMM]HandleWaitDeleteWinMsg abort!! Delete"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_8230_112_2_18_2_20_52_474, (uint8*) "");
        break;
    default:
        result = MMIPUB_HandleWaitWinMsg (win_id, msg_id, param);
        break;
    }

    return (result);
}



/*****************************************************************************/
//  Description : HandleFmmQueryWin
//  Global resource dependence :
//      Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDelQueryWin (MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        result = MMIPUB_HandleQueryWinMsg (win_id, msg_id, param);
        break;
    case MSG_APP_OK:
#if defined TOUCH_PANEL_SUPPORT
    case MSG_CTL_PENOK:
#endif
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
        {
            MMIFMM_DELQUERY_DATA_T *win_data_p = (MMIFMM_DELQUERY_DATA_T*) MMK_GetWinUserData (win_id);

            if (PNULL != win_data_p)
            {
                MMIFMM_EnterDeleteWin (win_data_p->list_data_ptr, win_data_p->current_path_ptr, win_data_p->cur_pathname);
            }

            MMK_CloseWin (win_id);
        }
        break;

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        //enter fmm explorer
        MMK_CloseWin (win_id);
        break;
    case MSG_CLOSE_WINDOW:
        DelQueryCloseCtrlFunc (win_id);
        result = MMIPUB_HandleQueryWinMsg (win_id, msg_id, param);
        break;
    default:
        result = MMIPUB_HandleQueryWinMsg (win_id, msg_id, param);
        break;
    }

    return result;
}

/*****************************************************************************/
// Description : open ctrl func
// Global resource dependence :
// Author:  jian.ma
// Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E DelQueryOpenCtrlFunc (
    MMI_WIN_ID_T win_id,
    MMIFMM_DELQUERY_DATA_T *query_data
)
{
    MMIFMM_DELQUERY_DATA_T *del_query_ptr = PNULL;

    if (PNULL == query_data)
    {
        return MMI_RESULT_FALSE;
    }

    del_query_ptr = (MMIFMM_DELQUERY_DATA_T *) SCI_ALLOC_APPZ (sizeof (MMIFMM_DELQUERY_DATA_T));
    del_query_ptr->current_path_ptr = query_data->current_path_ptr;
    del_query_ptr->list_data_ptr = query_data->list_data_ptr;
    del_query_ptr->mark_num = query_data->mark_num;
    MMIAPICOM_Wstrncpy (del_query_ptr->cur_pathname, query_data->cur_pathname, MMIAPICOM_Wstrlen (query_data->cur_pathname));

    MMK_SetWinUserData (win_id, (void *) del_query_ptr);

    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
// Description : close ctrl func
// Global resource dependence :
// Author:  jian.ma
// Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E DelQueryCloseCtrlFunc (MMI_WIN_ID_T win_id)
{
    MMIFMM_DELQUERY_DATA_T *win_data_p = (MMIFMM_DELQUERY_DATA_T*) MMK_GetWinUserData (win_id);

    SCI_FREE (win_data_p);

    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
//  Description : del file menu function
//  Global resource dependence :
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC void MMIFMM_DeleteSelectFile (
    MMIFMM_DATA_INFO_T  *list_data_ptr,
    MMI_CTRL_ID_T       ctrl_id,
    MMIFMM_PATH_INFO_T  *current_path_ptr
)
{
    uint16  current_index = 0;
    uint32  file_len = 0;
    MMI_WIN_ID_T            query_win_id = MMIFMM_QUERY_WIN_ID;
    uint16  mark_flag = 0;
    wchar cur_pathname [MMIFMM_PATHNAME_LEN +1] = {0};

    //SCI_TRACE_LOW:"MMIFMM: DeleteSelectFile"
    SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_7786_112_2_18_2_20_51_467, (uint8*) "");

    if (0 == list_data_ptr->mark_num)
    {
        current_index = GUILIST_GetCurItemIndex (ctrl_id);
        file_len = MMIFMM_CombineFullFileName (list_data_ptr, current_path_ptr, current_index, cur_pathname, MMIFMM_PATHNAME_LEN);
    }
    //CR161515
    // 判断是外部模块通过filter访问fmm还是从主入口main访问（）区别在于前者mark all是部分文件而不是全部文件
    if ((MMIFMM_FILE_ALL != MMIFMM_GetDisplayFileType())||(MMIFMM_FMI_STATE_LOOKUP_FILE == list_data_ptr->state))
    {
        mark_flag = list_data_ptr->mark_num;
    }
    else
    {
        if (list_data_ptr->mark_num == list_data_ptr->file_num + list_data_ptr->folder_num)
        {
            mark_flag = MMI_FMM_DELETE_ALLFILES_FLAG;
        }
        else
        {
            mark_flag = list_data_ptr->mark_num;
        }
    }

#ifdef DRM_SUPPORT

    if (IsDeleteDrmFile (list_data_ptr, current_path_ptr, mark_flag, cur_pathname))
    {
        //if (1 < s_copy_del_path_data_ptr->mark_flag)
        if (1 < mark_flag)
        {
            //如果是多个文件
            MMIPUB_OpenQueryWinByTextId (TXT_DRM_RIGHTS_VALID_WHETHER_DELETE_ALL, IMAGE_PUBWIN_QUERY, &query_win_id, HandleDelQueryWin);
        }
        else
        {
            //如果就是单个文件
            MMIPUB_OpenQueryWinByTextId (TXT_DRM_RIGHTS_VALID_WHETHER_DELETE, IMAGE_PUBWIN_QUERY, &query_win_id, HandleDelQueryWin);
        }
    }
    else
#endif
    {
        MMIPUB_OpenQueryWinByTextId (TXT_DELETE, IMAGE_PUBWIN_QUERY, &query_win_id, HandleDelQueryWin);
    }

    {
        MMIFMM_DELQUERY_DATA_T del_query_data = {0};
        del_query_data.current_path_ptr = current_path_ptr;
        del_query_data.list_data_ptr = list_data_ptr;
        del_query_data.mark_num = mark_flag;
        MMIAPICOM_Wstrncpy (del_query_data.cur_pathname, cur_pathname, MMIAPICOM_Wstrlen (cur_pathname));
        DelQueryOpenCtrlFunc (query_win_id, &del_query_data);
    }
}

#ifdef DRM_SUPPORT
/*****************************************************************************/
//  Description : is delete drm file
//  Global resource dependence :
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsDeleteDrmFile (
    MMIFMM_DATA_INFO_T  *list_data_ptr,
    MMIFMM_PATH_INFO_T  *current_path_ptr,
    uint16 mark_num,
    wchar *cur_pathname
)
{
    BOOLEAN result = FALSE;
    BOOLEAN is_one_drmfile = FALSE;
    uint16 i = 0;
    wchar full_path_name[MMIFMM_FULL_FILENAME_LEN+1] = {0};

    DRM_PERMISSION_MODE_E drm_permission = DRM_PERMISSION_NONE;

    if (PNULL == list_data_ptr || PNULL == current_path_ptr)
    {
        return result;
    }

    if (0 < mark_num)
    {
        //如果只选中了一个，且为文件，则需要检查一下，是否是DRM，若是，直接退出
        for (i = 0; i < (uint16) (list_data_ptr->file_num + list_data_ptr->folder_num); i++)
        {
            if (MMIFMM_FILE_STATE_UNCHECKED != list_data_ptr->data[i]->state)
            {
                if (MMIFMM_FILE_TYPE_DRM == list_data_ptr->data[i]->type)
                {
                    SCI_MEMSET (full_path_name, 0, ( (MMIFMM_FULL_FILENAME_LEN + 1) *sizeof (wchar)));
                    MMIFMM_CombineToFileName (current_path_ptr->pathname, current_path_ptr->pathname_len,
                                              list_data_ptr->data[i]->filename_ptr, list_data_ptr->data[i]->filename_len,
                                              full_path_name, MMIFMM_PATHNAME_LEN);
                    drm_permission = MMIAPIDRM_GetFilePermission (SFS_INVALID_HANDLE, full_path_name);
                    is_one_drmfile = MMIAPIDRM_IsRightsValid (SFS_INVALID_HANDLE, full_path_name, TRUE, drm_permission);

                    if (is_one_drmfile)
                    {
                        break;
                    }
                }
            }
        }

        result = is_one_drmfile;
    }
    else
    {
        //如果当前无标记，选择高亮文件
        MMIFILE_HANDLE file_handle = SFS_INVALID_HANDLE;
        file_handle = SFS_CreateFile (cur_pathname, SFS_MODE_READ | SFS_MODE_OPEN_EXISTING, 0, 0);

        if (SFS_INVALID_HANDLE != file_handle)
        {
            if (MMIAPIDRM_IsDRMFile (file_handle, PNULL))
            {
                drm_permission = MMIAPIDRM_GetFilePermission (file_handle, PNULL);
                result = MMIAPIDRM_IsRightsValid (file_handle, PNULL, TRUE, drm_permission);
            }
        }

        SFS_CloseFile (file_handle);

    }

    return result;
}
#endif
/*****************************************************************************/
// Description : enter delete file win
// Global resource dependence :
// Author:  jian.ma
// RETRUN:
// Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIFMM_EnterDeleteWin (
    MMIFMM_DATA_INFO_T  *list_data_ptr,
    MMIFMM_PATH_INFO_T  *current_path_ptr,
    wchar* cur_pathname
)
{

    MMI_RESULT_E                result = MMI_RESULT_FALSE;
    MMI_HANDLE_T                win_handle = 0;
    MMIFMM_DELWIN_DATA_T        *del_win_data_ptr = PNULL;

    if (PNULL == list_data_ptr || PNULL == current_path_ptr)
    {
        return MMI_RESULT_FALSE;
    }

    if (MMK_IsOpenWin (MMIFMM_DEL_SELECT_FILE_WIN_ID))
    {
        MMK_CloseWin (MMIFMM_DEL_SELECT_FILE_WIN_ID);
    }

    del_win_data_ptr = (MMIFMM_DELWIN_DATA_T*) SCI_ALLOC_APPZ (sizeof (MMIFMM_DELWIN_DATA_T));

    del_win_data_ptr->s_copy_del_path_data_ptr = (MMIFMM_COPY_DEL_PATH_DATA_INFO_T *) SCI_ALLOC_APPZ (sizeof (MMIFMM_COPY_DEL_PATH_DATA_INFO_T));

    if (0 == list_data_ptr->mark_num)
    {
        //current_index = GUILIST_GetCurItemIndex(ctrl_id);

        MMIAPICOM_Wstrncpy (del_win_data_ptr->s_copy_del_path_data_ptr->cur_pathname, cur_pathname, MMIAPICOM_Wstrlen (cur_pathname));
    }

    //CR161515
    // 判断是外部模块通过filter访问fmm还是从主入口main访问（）区别在于前者mark all是部分文件而不是全部文件
    if ((MMIFMM_FILE_ALL != MMIFMM_GetDisplayFileType())||(MMIFMM_FMI_STATE_LOOKUP_FILE == list_data_ptr->state))
    {
        del_win_data_ptr->s_copy_del_path_data_ptr->mark_flag = list_data_ptr->mark_num ;
    }
    else
    {
        if (list_data_ptr->mark_num == list_data_ptr->file_num + list_data_ptr->folder_num)
        {
            MMIAPICOM_Wstrncpy (del_win_data_ptr->s_copy_del_path_data_ptr->cur_pathname, current_path_ptr->pathname, current_path_ptr->pathname_len);
            del_win_data_ptr->s_copy_del_path_data_ptr->mark_flag = MMI_FMM_DELETE_ALLFILES_FLAG;
        }
        else
        {
            del_win_data_ptr->s_copy_del_path_data_ptr->mark_flag = list_data_ptr->mark_num ;
        }
    }


    del_win_data_ptr->current_path_ptr = current_path_ptr;
    del_win_data_ptr->list_data_ptr = list_data_ptr;
    del_win_data_ptr->s_delete_handle_ptr = PNULL;
    del_win_data_ptr->s_need_update = FALSE;
    del_win_data_ptr->s_user_stop_copy = FALSE;
    del_win_data_ptr->s_user_stop_del = FALSE;
    del_win_data_ptr->s_resoved_num = 0;
    del_win_data_ptr->s_copydel_opt_state = 0;

    win_handle = MMK_CreateWin ( (uint32*) MMIFMM_DEL_SELECTFILE_WIN_TAB, (ADD_DATA) del_win_data_ptr);

    if (!win_handle || (MMI_INVALID_ID == win_handle))
    {
        SCI_FREE (del_win_data_ptr->s_copy_del_path_data_ptr);
        SCI_FREE (del_win_data_ptr);
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
LOCAL MMI_RESULT_E DelWinOpenCtrlFunc (MMI_WIN_ID_T win_id)
{
    MMIFMM_DELWIN_DATA_T *del_win_ptr = (MMIFMM_DELWIN_DATA_T *) MMK_GetWinAddDataPtr (win_id);

    if (PNULL == del_win_ptr)
    {
        return MMI_RESULT_FALSE;
    }

    MMK_SetWinUserData (win_id, (void *) del_win_ptr);

    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
// Description : close ctrl func
// Global resource dependence :
// Author:  jian.ma
// Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E DelWinCloseCtrlFunc (MMI_WIN_ID_T win_id)
{
    MMIFMM_DELWIN_DATA_T *win_data_p = (MMIFMM_DELWIN_DATA_T*) MMK_GetWinUserData (win_id);

    if (PNULL != win_data_p)
    {
        SCI_FREE (win_data_p->s_copy_del_path_data_ptr);
        SCI_FREE (win_data_p);
    }

    return MMI_RESULT_TRUE;
}


/*****************************************************************************/
//  Description : copy del exit
//  Global resource dependence :
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL void FmmCopyDelExit (MMI_WIN_ID_T win_id)
{
    MMIFMM_DELWIN_DATA_T       *delwin_data_p = (MMIFMM_DELWIN_DATA_T*) MMK_GetWinUserData (win_id);

    if (PNULL != delwin_data_p)
    {
        SFS_FindClose (delwin_data_p->s_copy_del_path_data_ptr->sfs_find_handle);
        delwin_data_p->s_copy_del_path_data_ptr->sfs_find_handle = 0;

        if (delwin_data_p->s_need_update)
        {
#ifdef MMI_PDA_SUPPORT
            GUIFORM_SetChildDisplay (MMIFMM_UDISK_FORM_CTRL_ID, MMICOMMON_SOFTKEY_CTRL_ID, GUIFORM_CHILD_DISP_HIDE);
#endif
            MMK_PostMsg (MMIFMM_MAIN_WIN_ID, MSG_FMM_DELETE_END_CNF, PNULL, 0);
        }
    }
}


/*****************************************************************************/
//  Description : handle del task callback
//  Global resource dependence :
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIFMM_DelCallBack (uint16     result)
{
    MMIFMM_DELWIN_DATA_T       *delwin_data_p = (MMIFMM_DELWIN_DATA_T*) MMK_GetWinUserData (MMIFMM_DEL_SELECT_FILE_WIN_ID);

    if (MMK_IsOpenWin (MMIFMM_DEL_SELECT_FILE_WIN_ID))
    {
        if (PNULL != delwin_data_p && PNULL != delwin_data_p->s_delete_handle_ptr)
        {
            MMIFILE_DeleteClose (delwin_data_p->s_delete_handle_ptr);
            delwin_data_p->s_delete_handle_ptr = PNULL;
        }

        MMK_PostMsg (MMIFMM_DEL_SELECT_FILE_WIN_ID, MSG_FMM_DELETE_END_CNF, &result, sizeof (result));
    }
    else if (MMK_IsOpenWin (MMIFMM_COPY_SELECT_FILE_WIN_ID))
    {
        MMK_PostMsg (MMIFMM_COPY_SELECT_FILE_WIN_ID, MSG_FMM_DELETE_END_CNF, &result, sizeof (result));
    }
    else if (MMK_IsOpenWin (MMIFMM_MOVE_FILE_WIN_ID))
    {
        MMK_PostMsg (MMIFMM_MOVE_FILE_WIN_ID, MSG_FMM_DELETE_END_CNF, &result, sizeof (result));
    }
    else if (SFS_ERROR_NONE != result && SFS_ERROR_NOT_EXIST != result) //MS00194050
    {
        if (PNULL != delwin_data_p && PNULL != delwin_data_p->s_delete_handle_ptr)
        {
            MMIFILE_DeleteClose (delwin_data_p->s_delete_handle_ptr);
            delwin_data_p->s_delete_handle_ptr = PNULL;
        }

        MMIPUB_CloseWaitWin (MMIFMM_FILE_WAITING_WIN_ID);
    }
}
/*****************************************************************************/
//  Description : get del state
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIFMM_GetDelState (void)
{
    MMIFMM_DELWIN_DATA_T       *delwin_data_p = (MMIFMM_DELWIN_DATA_T*) MMK_GetWinUserData (MMIFMM_DEL_SELECT_FILE_WIN_ID);

    if (PNULL != delwin_data_p)
    {
        return delwin_data_p->s_copydel_opt_state;
    }
    else
    {
        return 0;
    }
}
