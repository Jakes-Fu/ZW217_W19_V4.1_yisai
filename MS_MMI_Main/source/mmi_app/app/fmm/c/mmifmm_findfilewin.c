/*****************************************************************************
** File Name:      mmifmm_findfilewin.c                                      *
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

#define _MMIFMM_FINDFILEWIN_C_


/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmifmm_findfilewin.h"
//#include "mmifmm_export.h"
#include "sci_types.h"
#include "mmk_type.h"
#include "window_parse.h"
#include "mmifmm_id.h"
#include "mmifmm_text.h"
#include "mmipub.h"
#include "mmifmm_comfunc.h"
#include "sfs.h"
#include "mmifmm_app.h"
#include "mmifmm_mem.h"
#include "mmifmm_sort.h"
#include "mmifmm_export.h"
#include "mmifmm_mainwin.h"
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/

#define MMIFMM_FIND_FILE_NAME_MAX_LEN       255  //字符
/**--------------------------------------------------------------------------*
 **                         TYPE DEFINITION                                  *
 **--------------------------------------------------------------------------*/
typedef struct
{
    MMIFMM_FINDFILE_WIN_PARAM_T     *find_file_p;
    MMIFMM_PATH_DATA_INFO_T         *find_path_stack_ptr;
    uint8                           wait_timer_id;
} MMIFMM_FINDFILE_INFO_T;

/**-------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/
//LOCAL MMIFMM_PATH_DATA_INFO_T         *s_find_path_stack_ptr =PNULL;
/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
#ifdef MMI_FMM_SEARCH_SUPPORT
/*****************************************************************************/
// Description : 查找文件窗口
// Global resource dependence :
// Author:Ming.Song 2007-4-12
// Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleFindFileWinMsg (
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
LOCAL MMI_RESULT_E FindFileOpenCtrlFunc (MMI_WIN_ID_T win_id);

/*****************************************************************************/
// Description : close ctrl func
// Global resource dependence :
// Author:  jian.ma
// Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E FindFileCloseCtrlFunc (MMI_WIN_ID_T win_id);


/*****************************************************************************/
// Description : 在指定目录下查找文件，把子目录放入堆栈
// Global resource dependence :
// Author: Ming.Song 2007-4-13
// Note:
/*****************************************************************************/
LOCAL void FmmFindFile (
    const wchar               *find_path_ptr,
    MMIFMM_DATA_INFO_T   *result_data_ptr,
    wchar               *find_string_ptr,
    MMIFMM_PATH_DATA_INFO_T * find_path_stack_ptr
);

/*****************************************************************************/
//  Description : 字符串是否是通配符格式
//  Global resource dependence :
//  Author: jian.ma
//  Note:
/*****************************************************************************/
BOOLEAN SetStrWildcard (
    wchar *str
);

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
//find file win
WINDOW_TABLE (MMIFMM_FIND_FILE_WIN_TAB) =
{
    WIN_FUNC ( (uint32) HandleFindFileWinMsg),
    WIN_ID (MMIFMM_FIND_FILE_WIN_ID),
    WIN_TITLE (TXT_COMM_SEARCH),
#ifdef MMI_PDA_SUPPORT
    CREATE_FORM_CTRL (GUIFORM_LAYOUT_ORDER, MMIFMM_FIND_FORM_CTRL_ID),
    CHILD_EDIT_TEXT_CTRL (TRUE, MMIFMM_FIND_FILE_NAME_MAX_LEN, MMIFMM_FIND_FILE_EDITBOX_CTRL_ID, MMIFMM_FIND_FORM_CTRL_ID),
    CHILD_SOFTKEY_CTRL (TXT_COMMON_OK, TXT_NULL, STXT_RETURN, MMIFMM_FIND_SOFTKEY_CTRL_ID, MMIFMM_FIND_FORM_CTRL_ID),
#else
    WIN_SOFTKEY (TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_EDIT_TEXT_CTRL (MMIFMM_FIND_FILE_NAME_MAX_LEN, MMIFMM_FIND_FILE_EDITBOX_CTRL_ID),
#endif
    END_WIN
};
/*****************************************************************************/
// Description : enter find file win
// Global resource dependence :
// Author: jian.ma
// RETRUN:
// Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIFMM_EnterFindFileWin (MMIFMM_FINDFILE_WIN_PARAM_T *find_file_p)
{
    MMIFMM_FINDFILE_WIN_PARAM_T* param_to_findfile_win = PNULL;
    MMI_RESULT_E                result = MMI_RESULT_FALSE;
    MMI_HANDLE_T                win_handle = 0;

    if (PNULL == find_file_p)
    {
        return MMI_RESULT_FALSE;
    }

    if (MMK_IsOpenWin (MMIFMM_FIND_FILE_WIN_ID))
    {
        MMK_CloseWin (MMIFMM_FIND_FILE_WIN_ID);
    }

    param_to_findfile_win = (MMIFMM_FINDFILE_WIN_PARAM_T*) SCI_ALLOC_APPZ (sizeof (MMIFMM_FINDFILE_WIN_PARAM_T));
    param_to_findfile_win->find_path_ptr = find_file_p->find_path_ptr;
    param_to_findfile_win->result_data_ptr = find_file_p->result_data_ptr;
    param_to_findfile_win->max_result_file_num = find_file_p->max_result_file_num;
    param_to_findfile_win->win_id = find_file_p->win_id;
    param_to_findfile_win->msg_id = find_file_p->msg_id;
    win_handle = MMK_CreateWin ( (uint32 *) MMIFMM_FIND_FILE_WIN_TAB , (ADD_DATA) param_to_findfile_win);

    if (!win_handle || (MMI_INVALID_ID == win_handle))
    {
        SCI_FREE (param_to_findfile_win);
        result =  MMI_RESULT_FALSE;
    }
    else
    {
        result = MMI_RESULT_TRUE;
    }

    return result;

}
/*****************************************************************************/
// Description : 查找文件窗口
// Global resource dependence :
// Author:Ming.Song 2007-4-12
// Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleFindFileWinMsg (
    MMI_WIN_ID_T         win_id,
    MMI_MESSAGE_ID_E     msg_id,
    DPARAM               param
)
{
    MMI_RESULT_E                    result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T                   ctrl_id = MMIFMM_FIND_FILE_EDITBOX_CTRL_ID;
    MMI_STRING_T                    name_str = {0};
    MMIFMM_FINDFILE_INFO_T          *find_param = PNULL;
    wchar                           *find_name_ptr = PNULL;


    find_param = (MMIFMM_FINDFILE_INFO_T*) MMK_GetWinUserData (win_id);


    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
#ifdef MID_SOFTKEY_USE_IMG_SUPPORT
		GUIWIN_SeSoftkeytButtonIconId(win_id, IMAGE_NULL, 1, TRUE);
#endif
        if (MMI_RESULT_FALSE == FindFileOpenCtrlFunc (win_id))
        {
            MMK_CloseWin (win_id);
            break;
        }

        //set left softkey
        GUIEDIT_SetSoftkey (ctrl_id, 0, 1, TXT_NULL, TXT_COMMON_OK, PNULL);
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
                GUIEDIT_GetString (ctrl_id, &name_str);

                if (0 == name_str.wstr_len)
                {
                    break;
                }

                //文件名是否有效
                if (MMIFMM_FindStringIsValid (&name_str))
                {
					if (PNULL == find_param)//coverity12701
					{
						SCI_TRACE_LOW("[mmifmm_findfilewin.c] [HandleFindFileWinMsg] PNULL == find_param");
						break;
    				}
                    find_param->wait_timer_id = MMK_CreateTimer (200, FALSE);
                    MMIPUB_OpenAlertWinByTextId (PNULL, TXT_COMMON_WAITING, TXT_NULL, IMAGE_PUBWIN_WAIT, PNULL, PNULL, MMIPUB_SOFTKEY_NONE, PNULL);
                }
                else
                {
                    MMIPUB_OpenAlertFailWin (TXT_DC_NAMEINVALID);//ID TXT_DC_NAMEINVALID
                }

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

        //文件名是否有效
        if (MMIFMM_FindStringIsValid (&name_str))
        {
			if (PNULL == find_param)//coverity12701
			{
				SCI_TRACE_LOW("[mmifmm_findfilewin.c] [HandleFindFileWinMsg] PNULL == find_param");
				break;
    		}
            find_param->wait_timer_id = MMK_CreateTimer (200, FALSE);
            MMIPUB_OpenAlertWinByTextId (PNULL, TXT_COMMON_WAITING, TXT_NULL, IMAGE_PUBWIN_WAIT, PNULL, PNULL, MMIPUB_SOFTKEY_NONE, PNULL);
        }
        else
        {
            MMIPUB_OpenAlertFailWin (TXT_DC_NAMEINVALID);
        }

        break;

    case MSG_TIMER:
        if (PNULL == find_param)
        {
            break;
        }

        if (0 != find_param->wait_timer_id && find_param->wait_timer_id == * (uint8*) param)
        {

            MMK_StopTimer (find_param->wait_timer_id);
            find_param->wait_timer_id = 0;

            GUIEDIT_GetString (ctrl_id, &name_str);
            //MMIFMM_FreeFileInfoBuffer(FALSE);
            MMIFMM_FreeFileInfo (find_param->find_file_p->result_data_ptr, FALSE);
            SCI_MEMSET (find_param->find_file_p->result_data_ptr, 0, sizeof (MMIFMM_DATA_INFO_T));
            find_param->find_file_p->result_data_ptr->sort = MMIFMM_GeFileSortType();
            // MMI_WSTRNCPY( ucs2_find_string_name, MMIFMM_FIND_FILE_NAME_MAX_LEN, name_str.wstr_ptr, name_str.wstr_len, name_str.wstr_len );
            find_name_ptr = SCI_ALLOCA ( (MMIFMM_FULL_FILENAME_LEN + 3) * sizeof (wchar));
            SCI_MEMSET (find_name_ptr, 0x00, (MMIFMM_FULL_FILENAME_LEN + 3) *sizeof (wchar));
            SCI_MEMCPY (find_name_ptr, name_str.wstr_ptr, name_str.wstr_len * sizeof (wchar));
            SetStrWildcard (find_name_ptr);

            FmmFindFile (find_param->find_file_p->find_path_ptr, find_param->find_file_p->result_data_ptr, find_name_ptr, find_param->find_path_stack_ptr);

            while (find_param->find_path_stack_ptr->begin_pos != find_param->find_path_stack_ptr->end_pos)
            {
                FmmFindFile (find_param->find_path_stack_ptr->path_info[find_param->find_path_stack_ptr->begin_pos].pathname,
                             find_param->find_file_p->result_data_ptr,
                             find_name_ptr,
                             find_param->find_path_stack_ptr);
                find_param->find_path_stack_ptr->begin_pos++;
            }

            SCI_FREE (find_name_ptr);
            MMIFMM_FilePreSort (find_param->find_file_p->result_data_ptr);
            MMIAPIFMM_FileDataSort (find_param->find_file_p->result_data_ptr, find_param->find_file_p->result_data_ptr->sort);
            MMK_PostMsg ( (MMI_WIN_ID_T) find_param->find_file_p->win_id, (MMI_MESSAGE_ID_E) find_param->find_file_p->msg_id, PNULL, 0);
        }

        break;

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin (win_id);
        break;

    case MSG_CLOSE_WINDOW:
        FindFileCloseCtrlFunc (win_id);
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
LOCAL MMI_RESULT_E FindFileOpenCtrlFunc (MMI_WIN_ID_T win_id)
{
    MMIFMM_FINDFILE_WIN_PARAM_T *findfile_data = (MMIFMM_FINDFILE_WIN_PARAM_T*) MMK_GetWinAddDataPtr (win_id);
    MMIFMM_FINDFILE_INFO_T *find_file_info_p = (MMIFMM_FINDFILE_INFO_T *) SCI_ALLOC_APPZ (sizeof (MMIFMM_FINDFILE_INFO_T));
    find_file_info_p->find_file_p = findfile_data;
    find_file_info_p->find_path_stack_ptr = (MMIFMM_PATH_DATA_INFO_T *) SCI_ALLOC_APPZ (sizeof (MMIFMM_PATH_DATA_INFO_T));
    find_file_info_p->wait_timer_id = 0;

    MMK_SetWinUserData (win_id, (void *) find_file_info_p);

    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
// Description : close ctrl func
// Global resource dependence :
// Author:  jian.ma
// Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E FindFileCloseCtrlFunc (MMI_WIN_ID_T win_id)
{
    MMIFMM_FINDFILE_INFO_T *win_data_p = (MMIFMM_FINDFILE_INFO_T*) MMK_GetWinUserData (win_id);

    if (PNULL != win_data_p)
    {
        SCI_FREE (win_data_p->find_path_stack_ptr);

        if (0 != win_data_p->wait_timer_id)
        {
            MMK_StopTimer (win_data_p->wait_timer_id);
            win_data_p->wait_timer_id = 0;
        }

        SCI_FREE (win_data_p);
        win_data_p = PNULL;
    }

    return MMI_RESULT_TRUE;
}


/*****************************************************************************/
// Description : 在指定目录下查找文件，把子目录放入堆栈
// Global resource dependence :
// Author: Ming.Song 2007-4-13
// Note:
/*****************************************************************************/
LOCAL void FmmFindFile (
    const wchar               *find_path_ptr,
    MMIFMM_DATA_INFO_T   *result_data_ptr,
    wchar               *find_string_ptr,
    MMIFMM_PATH_DATA_INFO_T * find_path_stack_ptr
)
{
    uint32              find_path_len = 0;
    uint32              find_string_len = 0;
    MMIFILE_HANDLE      file_handle = 0;
    MMIFILE_FIND_DATA_T find_data = {0};
    uint16              filename_len = 0;
    uint16              suffix_len = 0;
    uint16              suffix_name[MMIFMM_FILENAME_LEN+1] = {0};
    //uint16              i = 0;
    uint16              path_depth = 0;
    uint16              index = 0;
    uint16                  ctlStr[] = L"/A:+NF-H-S";
    uint16      ctrlStr1[] = L"/A:+NF-F-H-S";
    uint16                  filter[] = L"*";
    SFS_ERROR_E             result = SFS_ERROR_NONE;

    if (PNULL == find_path_ptr || PNULL == result_data_ptr || PNULL == find_string_ptr || PNULL== find_path_stack_ptr)
    {
        return;
    }
    find_path_len = MMIAPICOM_Wstrlen (find_path_ptr);
    find_string_len = MMIAPICOM_Wstrlen (find_string_ptr);

    // 查找第一个数据
    result = SFS_FindFirstFileCtrl (find_path_ptr, find_string_ptr, ctlStr, &file_handle, &find_data, PNULL);

    // 查找下一个数据
    if ( (SFS_INVALID_HANDLE != file_handle) && (SFS_ERROR_NONE == result))
    {
        index = result_data_ptr->file_num + result_data_ptr->folder_num;

        do
        {
            if (result_data_ptr->file_num + result_data_ptr->folder_num >= MMIFMM_FILE_NUM)
            {
                break;
            }

            filename_len = (uint16) MMIAPICOM_Wstrlen (find_data.name);

            // 全路径文件名不能超过MMIFMM_FULL_FILENAME_LEN
            if ( (find_path_len + filename_len + 1) <= MMIFMM_FULL_FILENAME_LEN && (filename_len > 0))
            {
                if (PNULL == result_data_ptr->data[index])
                {
                    result_data_ptr->data[index] = MMIFMM_GetFileInfoBuffer();//SCI_ALLOCA(sizeof(MMIFMM_FILE_INFO_T));

                    if (PNULL == result_data_ptr->data[index])
                    {
                        break;
                    }
                }

                SCI_MEMSET (result_data_ptr->data[index], 0x00, sizeof (MMIFMM_FILE_INFO_T));

                if (PNULL != result_data_ptr->data[index]->filename_ptr)
                {
                    SCI_FREE (result_data_ptr->data[index]->filename_ptr);
                }

                result_data_ptr->data[index]->filename_len = find_path_len + filename_len + 1;
                result_data_ptr->data[index]->filename_ptr = SCI_ALLOCA ( (sizeof (wchar) * result_data_ptr->data[index]->filename_len));

                if (PNULL == result_data_ptr->data[index]->filename_ptr)
                {
                    SCI_FREE (result_data_ptr->data[index]);
                    result_data_ptr->data[index] = PNULL;
                    break;
                }

                SCI_MEMSET (result_data_ptr->data[index]->filename_ptr, 0x00, (sizeof (wchar) *result_data_ptr->data[index]->filename_len));

                if (! (MMIFMM_FileIsFolder (find_data.attr)))
                {
                    // 文件数据
                    result_data_ptr->data[index]->type = MMIFMM_FILE_TYPE_NORMAL;
                    // 普通文件需要进一步解析文件类型
                    SCI_MEMSET (suffix_name , 0, ( (MMIFMM_FILENAME_LEN + 1) *sizeof (wchar)));
                    suffix_len = MMIFMM_FILENAME_LEN;

                    if (MMIAPIFMM_SplitFileName (find_data.name, filename_len,
                                                 PNULL, PNULL,
                                                 suffix_name, &suffix_len))
                    {
                        result_data_ptr->data[index]->type = MMIAPIFMM_ConvertFileType (suffix_name, suffix_len);
                    }

                    result_data_ptr->data[index]->file_size = find_data.length;

                    if (!MMIFMM_IsFileNeedDisplay (MMIFMM_GetDisplayFileType(), find_data.name, filename_len)) //suffix_name,suffix_len)) cr120695
                    {
                        SCI_FREE (result_data_ptr->data[index]->filename_ptr);
                        result_data_ptr->data[index]->filename_ptr = PNULL;
                        continue;
                    }

                    result_data_ptr->file_num++;
                }
                else
                {
                    //folder
                    result_data_ptr->data[index]->type = MMIFMM_FILE_TYPE_FOLDER;
                    result_data_ptr->data[index]->file_size = 0;
                    result_data_ptr->folder_num ++;
                }

                result_data_ptr->data[index]->time = MMIAPICOM_Tm2Second (find_data.modify_time.sec, find_data.modify_time.min, find_data.modify_time.hour,
                                                     find_data.modify_Date.mday, find_data.modify_Date.mon, find_data.modify_Date.year);

                // 包含路径
                MMI_WSTRNCPY (result_data_ptr->data[index]->filename_ptr,
                              result_data_ptr->data[index]->filename_len,
                              find_path_ptr, find_path_len, find_path_len);
                result_data_ptr->data[index]->filename_ptr[find_path_len] = MMIFILE_SLASH;
                MMI_WSTRNCPY (&result_data_ptr->data[index]->filename_ptr[find_path_len+1],
                              result_data_ptr->data[index]->filename_len - 1 - find_path_len,
                              find_data.name, filename_len, filename_len);
                index ++;
            }
        }
        while ( (SFS_NO_ERROR == MMIAPIFMM_FindNextFile (file_handle, &find_data)));

        MMIAPIFMM_FindClose (file_handle);
    }

    path_depth = (uint16) MMIFMM_GetFullPathDepth (find_path_ptr, find_path_len);

    if ( (index < MMIFMM_FILE_NUM) && (MMIFMM_PATH_DEPTH > path_depth))
    {

        result = SFS_FindFirstFileCtrl (find_path_ptr, filter, &ctrlStr1, &file_handle, &find_data, PNULL);/*lint !e64*/

        // 查找下一个数据
        if ( (SFS_INVALID_HANDLE != file_handle) && (SFS_ERROR_NONE == result))
        {
            do
            {
                if (find_path_stack_ptr->num >= MMIFMM_STORE_PATH_NUM)
                {
                    break;
                }

                filename_len = (uint16) MMIAPICOM_Wstrlen (find_data.name);

                // 全路径文件名不能超过MMIFMM_FULL_FILENAME_LEN
                if ( (find_path_len + filename_len + 1) <= MMIFMM_FULL_FILENAME_LEN)
                {
                    if (MMIFMM_FileIsFolder (find_data.attr))
                    {
                        // 目录数据,包含路径
                        find_path_stack_ptr->path_info[find_path_stack_ptr->num].pathname_len = (uint16) MMIFMM_CombineToFileName (find_path_ptr, (uint16) find_path_len, find_data.name, (uint16) filename_len,
                                find_path_stack_ptr->path_info[find_path_stack_ptr->num].pathname, (uint16) MMIFMM_PATHNAME_LEN);

                        find_path_stack_ptr->end_pos++;
                        find_path_stack_ptr->num++;
                    }
                }
            }
            while ( (find_path_stack_ptr->num < MMIFMM_STORE_PATH_NUM) && (SFS_NO_ERROR == MMIAPIFMM_FindNextFile (file_handle, &find_data)));

            MMIAPIFMM_FindClose (file_handle);
        }
    }

}




/*****************************************************************************/
//  Description : 字符串是否是通配符格式
//  Global resource dependence :
//  Author: jian.ma
//  Note:
/*****************************************************************************/
BOOLEAN SetStrWildcard (
    wchar *str
)
{
    uint16  str_len = 0;
    int32   i = 0;

    uint16  star = 0x002a;
    uint16  quest = 0x003f;

    //SCI_ASSERT(NULL != str);
    if (PNULL == str)
    {
        SCI_TRACE_LOW ("MMIFMM:[SetStrWildcard]PNULL == str");
        return FALSE;
    }

    str_len = (uint16) MMIAPICOM_Wstrlen (str);

    for (i = 0; i < str_len; i++)
    {
        if (star == str[i] || quest == str[i])
        {
            //找到'*','?'
            break;
        }
    }

    if (i >= str_len)
    {
        //在str前后各加一个'*'
        for (i = 0; i < str_len; i++)
        {
            str[str_len-i] = str[str_len-i-1];
        }

        str[0] = star;
        str[str_len+1] = star;
    }

    return TRUE;
}
#endif