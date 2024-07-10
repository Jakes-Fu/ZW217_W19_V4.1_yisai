/*****************************************************************************
** File Name:      mmifmm_detailwin.c                                        *
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

#define _MMIFMM_DETAILWIN_C_


/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_app_fmm_trc.h"
#include "mmifmm_detailwin.h"
#include "mmifmm_id.h"
#include "mmifmm_text.h"
#include "guirichtext.h"
#include "mmk_app.h"
#include "mmi_anim.h"
#include "mmipub.h"
#include "mmimp3_text.h"
#ifdef VIDEO_PLAYER_SUPPORT
#include "mmivp_export.h"
#include "mmivp_text.h"
#endif
#include "mmifmm_interface.h"
#ifdef DRM_SUPPORT
#include "mmidrm_export.h"
#include "mmimp3_export.h"
#endif
#include "window_parse.h"
#include "guires.h"
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         TYPE DEFINITION                                  *
 **--------------------------------------------------------------------------*/
// 文件详情数据
typedef struct mmifmm_detailwin_data_tag
{
    uint32              time;                           // 修改时间
    uint32              file_size;                      // 文件大小
    MMIFMM_FILE_TYPE_E  type;                           // 文件类型，
    uint16              filename_len;                   // 文件名长度，字符数
    wchar               filename[MMIFMM_FILENAME_LEN+1];// 文件名
    uint16              resolution_x;                   // 水平分辨率，用于图片
    uint16              resolution_y;                   // 垂直分辨率，用于图片
    uint32              file_num;                       // 文件数目
    uint32              folder_num;                     // 目录数目
#ifdef DRM_SUPPORT
    DRM_RIGHTS_T        drm_rights;                     //drm 版权信息
#endif

} MMIFMM_DETAILWIN_DATA_T;
/**-------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
// Description : to handle the fmm folder detail window message
// Global resource dependence :
// Author: jian.ma
// Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleFmmFileDetailWinMsg (
    MMI_WIN_ID_T        win_id,
    MMI_MESSAGE_ID_E    msg_id,
    DPARAM              param
);

/*****************************************************************************/
//  Description : fmm detail information waiting win msg hanlement
//  Global resource dependence : none
//  Author:     ying.xu
//  Note:       20100920
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleFolderDetailWaitWinMsg (
    MMI_WIN_ID_T              win_id,
    MMI_MESSAGE_ID_E      msg_id,
    DPARAM                        param
);

/*****************************************************************************/
// Description : open ctrl func
// Global resource dependence :
// Author: jian.ma
// Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E DetailWinOpenCtrlFunc (MMI_WIN_ID_T win_id);

/*****************************************************************************/
// Description : close ctrl func
// Global resource dependence :
// Author: jian.ma
// Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E DetailWinCloseCtrlFunc (MMI_WIN_ID_T win_id);

/*****************************************************************************/
// Description : MMIAPIFMM_SetFolderDetail
// Global resource dependence :
// Author: ying.xu
// Note:
/*****************************************************************************/
LOCAL void MMIFMM_SetFolderDetail (
    MMIFMM_DETAILWIN_DATA_T       *detail_data,
    MMI_CTRL_ID_T              ctrl_id ,
    MMI_WIN_ID_T            win_id
);

/*****************************************************************************/
// Description : setting folder detail cnf
// Global resource dependence :
// Author: YING.XU
// Note:
/*****************************************************************************/
LOCAL void MMIFMM_SetFolderDetailCNF (
    MMIFMM_DETAILWIN_DATA_T  *detail_data,
    MMI_CTRL_ID_T              ctrl_id,
    MMI_WIN_ID_T            win_id
);
/*****************************************************************************/
// Description : setting file detail
// Global resource dependence :
// Author:jian.ma
// Note:
/*****************************************************************************/
LOCAL void MMIFMM_SetFileDetail (
    MMIFMM_DETAILWIN_DATA_T       *detail_data,
    MMI_CTRL_ID_T              ctrl_id,
    MMI_WIN_ID_T            win_id
);

/*****************************************************************************/
//  Description : add one detail item
//  Global resource dependence :
//  Author:jian.ma
//  Note:
/*****************************************************************************/
LOCAL void AddOneDetailItem (
    MMI_CTRL_ID_T   ctrl_id,
    MMI_TEXT_ID_T   label_title,
    MMI_TEXT_ID_T   label_name,
    uint16          *index_ptr,
    wchar           *detail_ptr,
    uint32          detail_len
);
/*****************************************************************************/
// Description : set common character
// Global resource dependence :
// Author:jian.ma
// Note:
/*****************************************************************************/
LOCAL void SetCommonCharacter (GUIRICHTEXT_ITEM_T* p_item);

//the folder detail window
WINDOW_TABLE (MMIFMM_FILE_DETAIL_WIN_TAB) =
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC ( (uint32) HandleFmmFileDetailWinMsg),
    WIN_ID (MMIFMM_FOLDER_DETAIL_WIN_ID),
    WIN_TITLE (TXT_FMM_FILE_DETAIL),
    CREATE_RICHTEXT_CTRL (MMIFMM_FILE_DETAIL_TEXT_CTRL_ID),
    WIN_SOFTKEY (TXT_NULL, TXT_NULL, STXT_RETURN),
    END_WIN
};

/*****************************************************************************/
// Description : to handle the fmm folder detail window message
// Global resource dependence :
// Author: jian.ma
// Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleFmmFileDetailWinMsg (
    MMI_WIN_ID_T        win_id,
    MMI_MESSAGE_ID_E    msg_id,
    DPARAM              param
)
{
    MMI_RESULT_E                result = MMI_RESULT_TRUE;
    GUI_LCD_DEV_INFO            lcd_dev_info = {0};
    //MMIFMM_DETAIL_DATA_T        *detail_data=PNULL;
    MMI_CTRL_ID_T               ctrl_id = MMIFMM_FILE_DETAIL_TEXT_CTRL_ID;
    GUI_RECT_T                  d_rect = MMITHEME_GetClientRect();
    MMIFMM_DETAILWIN_DATA_T *detail_data = MMK_GetWinUserData (win_id);
    lcd_dev_info.lcd_id = GUI_MAIN_LCD_ID;
    lcd_dev_info.block_id = GUI_BLOCK_MAIN;

    //UNUSED_PARAM(param);
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
#ifdef MID_SOFTKEY_USE_IMG_SUPPORT
		GUIWIN_SeSoftkeytButtonIconId(win_id, IMAGE_NULL, 1, TRUE);
#endif
        DetailWinOpenCtrlFunc (win_id);
        MMK_SetAtvCtrl (win_id, ctrl_id);
        detail_data = MMK_GetWinUserData (win_id);

        if (PNULL != detail_data &&MMIFMM_FILE_TYPE_FOLDER == detail_data->type)//coverity 10347
        {
            MMIFMM_SetFolderDetail (detail_data, ctrl_id, win_id);
        }
        else
        {
            MMIFMM_SetFileDetail (detail_data, ctrl_id, win_id);
#ifdef DRM_SUPPORT

            //change the softkey whether is drm file window
            if (PNULL != detail_data && MMIFMM_FILE_TYPE_DRM == detail_data->type)
            {
                //get drm rights info
                DRM_RIGHTS_T drm_rights = {0};

                MMIAPIDRM_GetRightsinfo (SFS_INVALID_HANDLE, detail_data->filename, &drm_rights);

                if (DRM_LEVEL_SD == drm_rights.method_level)
                {
#ifdef MMI_PDA_SUPPORT
                    GUIWIN_SetTitleButtonState (win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
                    GUIWIN_SetTitleButtonTextId (win_id, GUITITLE_BUTTON_RIGHT, TXT_DRM_DOWNLOAD_RIGHTS, FALSE);
                    GUIWIN_SetTitleButtonHandleMsgInfo (win_id, GUITITLE_BUTTON_RIGHT, MSG_NOTIFY_TITLE_MENU_ICON, ctrl_id);
#endif
#ifdef MID_SOFTKEY_USE_IMG_SUPPORT
					GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_NULL, STXT_RETURN, FALSE);
					GUIWIN_SeSoftkeytButtonIconId(win_id, IMAGE_NULL, 1, TRUE);
#else
					GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_NULL, STXT_RETURN, FALSE);   
#endif
                }

#ifdef MMI_PDA_SUPPORT
                else
                {
                    GUIWIN_SetTitleButtonState (win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_INVISIBLE, FALSE);
                }

#endif
            }

#endif
        }

        GUIRICHTEXT_SetFocusMode (ctrl_id, GUIRICHTEXT_FOCUS_SHIFT_NONE);
        break;

    case MSG_FMM_SEARCH_CNF:
        MMIFMM_SetFolderDetailCNF (detail_data, ctrl_id, win_id);
        break;

    case MSG_FULL_PAINT:
        GUIRES_DisplayImg (PNULL,
                           &d_rect,
                           &d_rect,
                           win_id,
                           IMAGE_COMMON_BG,
                           &lcd_dev_info);
        break;

#ifdef DRM_SUPPORT
#if defined TOUCH_PANEL_SUPPORT
    case MSG_CTL_PENOK:
#endif
    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
    case MSG_APP_MENU:

        //change the softkey whether is drm file window
        if (PNULL != detail_data && MMIFMM_FILE_TYPE_DRM == detail_data->type)
        {
            //get drm rights info
            DRM_RIGHTS_T drm_rights = {0};

            MMIAPIDRM_GetRightsinfo (SFS_INVALID_HANDLE, detail_data->filename, &drm_rights);

            if (DRM_LEVEL_SD == drm_rights.method_level)
            {
                //SCI_TRACE_LOW:"MMIFMM: HandleFmmFileDetailWinMsg, open option"
                SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_7157_112_2_18_2_20_49_460, (uint8*) "");
                MMIAPIFMM_DrmOpenDownloadMenuWin (detail_data->filename);
            }
        }

        break;

#ifdef MMI_PDA_SUPPORT
    case MSG_NOTIFY_TITLE_MENU_ICON:

        if (PNULL != detail_data && MMIFMM_FILE_TYPE_DRM == detail_data->type)
        {
            //get drm rights info
            DRM_RIGHTS_T drm_rights = {0};

            MMIAPIDRM_GetRightsinfo (SFS_INVALID_HANDLE, detail_data->filename, &drm_rights);

            if (DRM_LEVEL_SD == drm_rights.method_level)
            {
                //SCI_TRACE_LOW:"MMIFMM: HandleFmmFileDetailWinMsg, Download Rights File"
                SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_7174_112_2_18_2_20_49_461, (uint8*) "");
                MMIAPIDRM_DownloadRightsFile (SFS_INVALID_HANDLE, detail_data->filename);
                MMK_CloseWin (win_id);
            }
        }

        break;
#endif
#endif

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin (win_id);
        break;
    case MSG_CLOSE_WINDOW:
        DetailWinCloseCtrlFunc (win_id);
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }

    return (result);
}


/*****************************************************************************/
// Description : enter file detail win
// Global resource dependence :
// Author:  jian.ma
// Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIFMM_EnterFileDetailWin (
    MMIFMM_DETAIL_DATA_T* detail_data  // 数据
)

{
    MMIFMM_DETAIL_DATA_T            *detail_data_param = PNULL;
    MMI_HANDLE_T                     win_handle = 0;
    MMI_RESULT_E                     result = MMI_RESULT_FALSE;

    if (MMK_IsOpenWin (MMIFMM_FOLDER_DETAIL_WIN_ID))
    {
        MMK_CloseWin (MMIFMM_FOLDER_DETAIL_WIN_ID);
    }

    detail_data_param = (MMIFMM_DETAIL_DATA_T *) SCI_ALLOC_APPZ (sizeof (MMIFMM_DETAIL_DATA_T));

    if (PNULL == detail_data_param)
    {
        SCI_TRACE_LOW ("MMIFMM: MMIFMM_EnterFileDetailWin, no memory!");
        return MMI_RESULT_FALSE;
    }

    SCI_MEMCPY (detail_data_param, detail_data, sizeof (MMIFMM_DETAIL_DATA_T));

    win_handle = MMK_CreateWin ( (uint32 *) MMIFMM_FILE_DETAIL_WIN_TAB , (ADD_DATA) detail_data_param);

    if (!win_handle || (MMI_INVALID_ID == win_handle))
    {
        SCI_TRACE_LOW ("[MMIFMM] MMIFMM_EnterFileDetailWin failed !");
        result =  MMI_RESULT_FALSE;
        SCI_FREE (detail_data_param);
    }
    else
    {
        result = MMI_RESULT_TRUE;
    }


    return MMI_RESULT_TRUE;
}
/*****************************************************************************/
// Description : open ctrl func
// Global resource dependence :
// Author:  jian.ma
// Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E DetailWinOpenCtrlFunc (MMI_WIN_ID_T win_id)
{
    MMIFMM_DETAIL_DATA_T *detail_data = (MMIFMM_DETAIL_DATA_T*) MMK_GetWinAddDataPtr (win_id);

    if (detail_data == PNULL)
    {
        //MMIFMM_RaiseError(1);
        return MMI_RESULT_FALSE;
    }

    MMK_SetWinUserData (win_id, (void *) detail_data);

    //MMICL_RegisterDateChangeEvent(win_id, MMICL_CALLTIMELIST_WIN_FLAG);
    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
// Description : close ctrl func
// Global resource dependence :
// Author:  jian.ma
// Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E DetailWinCloseCtrlFunc (MMI_WIN_ID_T win_id)
{

    MMIFMM_DETAILWIN_DATA_T *win_data_p = MMK_GetWinUserData (win_id);

    SCI_FREE (win_data_p);

    //MMICL_UnRegisterDateChangeEvent(MMICL_CALLTIMELIST_WIN_FLAG);

    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
// Description : MMIAPIFMM_SetFolderDetail
// Global resource dependence :
// Author: ying.xu
// Note:
/*****************************************************************************/
LOCAL void MMIFMM_SetFolderDetail (
    MMIFMM_DETAILWIN_DATA_T       *detail_data,
    MMI_CTRL_ID_T              ctrl_id ,
    MMI_WIN_ID_T            win_id
)
{
    SCI_TRACE_LOW ("MMIFMM: MMIAPIFMM_SetFolderDetail, detail_data =%x", detail_data);

    if (PNULL == detail_data)
    {
        return ;
    }

    if (MMIFMM_FILE_TYPE_FOLDER == detail_data->type)
    {
        MMI_STRING_T                wait_text = {0};
        MMI_GetLabelTextByLang (TXT_COMMON_WAITING, &wait_text);
        MMIPUB_OpenWaitWinEx (MMK_GetFirstAppletHandle(),
                              1,
                              &wait_text,
                              PNULL,
                              PNULL,
                              MMIFMM_FOLDER_DETAIL_INFOMATION_WAITING_WIN_ID,
                              IMAGE_NULL,
                              ANIM_PUBWIN_WAIT,
                              WIN_ONE_LEVEL,
                              MMIPUB_SOFTKEY_ONE,
                              (MMIPUB_HANDLE_FUNC) HandleFolderDetailWaitWinMsg,
                              (uint32) detail_data);
    }

    return ;
}


/*****************************************************************************/
//  Description : fmm detail information waiting win msg hanlement
//  Global resource dependence : none
//  Author:     ying.xu
//  Note:       20100920
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleFolderDetailWaitWinMsg (
    MMI_WIN_ID_T              win_id,
    MMI_MESSAGE_ID_E      msg_id,
    DPARAM                        param
)
{
    MMI_RESULT_E                result = MMI_RESULT_TRUE;
    static FILEARRAY            file_array = PNULL;
    MMIPUB_INFO_T*              add_data  =  MMIPUB_GetWinAddDataPtr (win_id);
	MMIFMM_DETAIL_DATA_T*     detail_data = PNULL;

	//coverity 12646
	if (PNULL == add_data)
	{
		SCI_TRACE_LOW("[mmifmm_detailwin.c] [HandleFolderDetailWaitWinMsg] PNULL == add_data");
		return MMI_RESULT_FALSE;
	}

    detail_data = (MMIFMM_DETAIL_DATA_T*) (add_data->user_data);
	if (PNULL == detail_data)
	{
		SCI_TRACE_LOW("[mmifmm_detailwin.c] [HandleFolderDetailWaitWinMsg] PNULL == detail_data");
		return MMI_RESULT_FALSE;
	}

    //SCI_TRACE_LOW:"MMIFMM: HandleFolderDetailWaitWinMsg, win_id =%d, msg_id = 0x%x."
    SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_5441_112_2_18_2_20_16_260, (uint8*) "dd", win_id, msg_id);

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            MMIFMM_FILTER_T             filter = {"*"};
            file_array = MMIAPIFILEARRAY_Create();
            // MMIAPIFMM_SearchFileInPath(detail_data->filename,detail_data->filename_len,&filter,TRUE,FUNC_FIND_FILE_AND_FOLDER,file_array,0,0);
            MMIFMM_SearchFileInPathNoSort (detail_data->filename, detail_data->filename_len, &filter,
                                           TRUE, FUNC_FIND_FILE_AND_FOLDER, file_array, win_id, MSG_FMM_SEARCH_CNF);
        }
        result = MMIPUB_HandleWaitWinMsg (win_id, msg_id, param);
        break;


    case MSG_FMM_SEARCH_CNF:
        {
            uint16 total_num = 0;
            uint16 i = 0;
            FILEARRAY_DATA_T    f_array_data = {0} ;
            total_num = (uint16) MMIAPIFILEARRAY_GetArraySize (file_array);

            for (i = 0; i < total_num; i++)
            {
                SCI_MEMSET (&f_array_data, 0, sizeof (f_array_data));

                if (MMIAPIFILEARRAY_Read (file_array, i, &f_array_data))
                {
                    if (FILEARRAY_TYPE_FOLDER == f_array_data.type)
                    {
                        detail_data->folder_num++;
                    }
                    else
                    {
                        detail_data->file_num++;
                    }

                    detail_data->file_size += f_array_data.size;
                }
            }

            MMIAPIFILEARRAY_Destroy (&file_array);
        }
        MMK_PostMsg (MMIFMM_FOLDER_DETAIL_WIN_ID, MSG_FMM_SEARCH_CNF, PNULL, 0);
        MMK_CloseWin (win_id);
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMIAPIFMM_SearchFileStop();
        MMK_SendMsg (MMIFMM_FOLDER_DETAIL_WIN_ID, msg_id, PNULL);
        MMK_CloseWin (win_id);
        //MMK_PostMsg(MMIFMM_FILE_DETAIL_WIN_TAB, MSG_CLOSE_WINDOW, PNULL, 0);
        result = MMIPUB_HandleWaitWinMsg (win_id, msg_id, param);
        break;

    case MSG_CLOSE_WINDOW:
        MMIAPIFILEARRAY_Destroy (&file_array);
        MMIAPIFMM_SearchFileStop();
        result = MMIPUB_HandleWaitWinMsg (win_id, msg_id, param);
        break;
    default:
        result = MMIPUB_HandleWaitWinMsg (win_id, msg_id, param);
        break;
    }

    return (result);
}

/*****************************************************************************/
// Description : MMIAPIFMM_SetFolderDetailCNF
// Global resource dependence :
// Author: YING.XU
// Note:
/*****************************************************************************/
LOCAL void MMIFMM_SetFolderDetailCNF (
    MMIFMM_DETAILWIN_DATA_T       *detail_data,
    MMI_CTRL_ID_T              ctrl_id ,
    MMI_WIN_ID_T            win_id
)
{
    MMI_TM_T                    tm = {0};
    int32                       i = 0;
    uint8                       detail_arr[MMIFMM_FULL_FILENAME_LEN] = {0};
    wchar                       wchar_detail_arr[MMIFMM_FULL_FILENAME_LEN + 1] = {0};
    uint16                      nIndex = 0;
    uint16                      string_len = 0;

    //SCI_TRACE_LOW:"MMIFMM: MMIAPIFMM_SetFolderDetailCNF, detail_data =%x"
    SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_5527_112_2_18_2_20_17_261, (uint8*) "d", detail_data);

    if (PNULL == detail_data)
    {
        return ;
    }

//set name title
    SCI_MEMSET (detail_arr, 0, sizeof (detail_arr));
    SCI_MEMSET (wchar_detail_arr, 0, sizeof (wchar_detail_arr));
    string_len = (uint16) MMIAPICOM_Wstrlen (detail_data->filename);

    for (i = string_len - 1; MMIFILE_SLASH != detail_data->filename[i] && i > 0; i--)
    {
        NULL;/*lint !e522 */
    }

    AddOneDetailItem (ctrl_id, TXT_INPUT_FILE_NAME, PNULL, &nIndex, &detail_data->filename[i+1], (string_len - i - 1));

//set date
    tm = MMIAPICOM_Second2Tm (detail_data->time);
    MMIAPISET_FormatDateStrByDateStyle (tm.tm_year, tm.tm_mon, tm.tm_mday, '.', detail_arr, MMIFMM_FULL_FILENAME_LEN);

    MMIAPICOM_StrToWstr (detail_arr, wchar_detail_arr);
    AddOneDetailItem (ctrl_id, TXT_COMMON_DATE, PNULL, &nIndex, wchar_detail_arr, (uint16) strlen ( (char*) detail_arr));

//set time
    SCI_MEMSET (detail_arr, 0, sizeof (detail_arr));
    SCI_MEMSET (wchar_detail_arr, 0, sizeof (wchar_detail_arr));

    MMIAPISET_FormatTimeStrByTime (tm.tm_hour, tm.tm_min, detail_arr, MMIFMM_FULL_FILENAME_LEN);
    MMIAPICOM_StrToWstr (detail_arr, wchar_detail_arr);
    AddOneDetailItem (ctrl_id, TXT_COMMON_TIME, PNULL, &nIndex, wchar_detail_arr, (uint16) strlen ( (char*) detail_arr));

//set size
    //SCI_MEMSET(detail_arr,0,sizeof(detail_arr));
    SCI_MEMSET (wchar_detail_arr, 0, sizeof (wchar_detail_arr));
    //sprintf((char *)detail_arr, "%ld.%03ldK", (detail_data->file_size /1024), ((detail_data->file_size %1024)*1000/1024));
    MMIAPIFMM_GetFileSizeString (detail_data->file_size, wchar_detail_arr, MMIFMM_SIZE_STR_LEN, FALSE);
    string_len = (uint16) MMIAPICOM_Wstrlen (wchar_detail_arr);
    //MMIAPICOM_StrToWstr((char*) detail_arr,wchar_detail_arr );
    AddOneDetailItem (ctrl_id, TXT_DETAIL_SIZE, PNULL, &nIndex, wchar_detail_arr, string_len);

//set location

    AddOneDetailItem (ctrl_id,
                      TXT_LOCATION ,
                      MMIAPIFMM_GetDeviceName (MMIAPIFMM_GetDeviceTypeByPath (&detail_data->filename[0], MMIFILE_DEVICE_LEN)),
                      &nIndex, PNULL, 0);

    //set folder
    SCI_MEMSET (detail_arr, 0, sizeof (detail_arr));
    SCI_MEMSET (wchar_detail_arr, 0, sizeof (wchar_detail_arr));
    sprintf ( (char *) detail_arr, "%ld", detail_data->folder_num);
    MMIAPICOM_StrToWstr (detail_arr, wchar_detail_arr);
    AddOneDetailItem (ctrl_id, TXT_FMM_SUB_FOLDER, PNULL, &nIndex, wchar_detail_arr, (uint16) strlen ( (char*) detail_arr));

    //set sub file num
    SCI_MEMSET (detail_arr, 0, sizeof (detail_arr));
    SCI_MEMSET (wchar_detail_arr, 0, sizeof (wchar_detail_arr));
    sprintf ( (char *) detail_arr, "%ld", detail_data->file_num);
    MMIAPICOM_StrToWstr (detail_arr, wchar_detail_arr);
    AddOneDetailItem (ctrl_id, TXT_FMM_SUB_FILE_NUM, PNULL, &nIndex, wchar_detail_arr, (uint16) strlen ( (char*) detail_arr));
}


/*****************************************************************************/
// Description : setting file detail
// Global resource dependence :
// Author:jian.ma
// Note:
/*****************************************************************************/
LOCAL void MMIFMM_SetFileDetail (
    MMIFMM_DETAILWIN_DATA_T       *detail_data,
    MMI_CTRL_ID_T              ctrl_id,
    MMI_WIN_ID_T            win_id
)
{
    MMI_TM_T                    tm = {0};
    int32                       i = 0;
    uint8                       detail_arr[MMIFMM_FULL_FILENAME_LEN] = {0};
    wchar                       wchar_detail_arr[MMIFMM_FULL_FILENAME_LEN + 1] = {0};
    uint16                      nIndex = 0;
    uint16                      string_len = 0;

	if (PNULL == detail_data)
	{
		return;
	}

//set name title
    SCI_MEMSET (detail_arr, 0, sizeof (detail_arr));
    SCI_MEMSET (wchar_detail_arr, 0, sizeof (wchar_detail_arr));
    string_len = (uint16) MMIAPICOM_Wstrlen (detail_data->filename);

    for (i = string_len - 1; MMIFILE_SLASH != detail_data->filename[i] && i > 0; i--)
    {
        NULL;/*lint !e522 */
    }

    AddOneDetailItem (ctrl_id, TXT_INPUT_FILE_NAME, PNULL, &nIndex, &detail_data->filename[i+1], (string_len - i - 1));

//set date
    tm = MMIAPICOM_Second2Tm (detail_data->time);
    MMIAPISET_FormatDateStrByDateStyle (tm.tm_year, tm.tm_mon, tm.tm_mday, '.', detail_arr, MMIFMM_FULL_FILENAME_LEN);

    MMIAPICOM_StrToWstr (detail_arr, wchar_detail_arr);
    AddOneDetailItem (ctrl_id, TXT_COMMON_DATE, PNULL, &nIndex, wchar_detail_arr, (uint16) strlen ( (char*) detail_arr));

//set time
    SCI_MEMSET (detail_arr, 0, sizeof (detail_arr));
    SCI_MEMSET (wchar_detail_arr, 0, sizeof (wchar_detail_arr));

    MMIAPISET_FormatTimeStrByTime (tm.tm_hour, tm.tm_min, detail_arr, MMIFMM_FULL_FILENAME_LEN);
    MMIAPICOM_StrToWstr (detail_arr, wchar_detail_arr);
    AddOneDetailItem (ctrl_id, TXT_COMMON_TIME, PNULL, &nIndex, wchar_detail_arr, (uint16) strlen ( (char*) detail_arr));

//set size
    //SCI_MEMSET(detail_arr,0,sizeof(detail_arr));
    SCI_MEMSET (wchar_detail_arr, 0, sizeof (wchar_detail_arr));
    //sprintf((char *)detail_arr, "%ld.%03ldK", (detail_data->file_size /1024), ((detail_data->file_size %1024)*1000/1024));
    MMIAPIFMM_GetFileSizeString (detail_data->file_size, wchar_detail_arr, MMIFMM_SIZE_STR_LEN, FALSE);
    string_len = (uint16) MMIAPICOM_Wstrlen (wchar_detail_arr);
    //MMIAPICOM_StrToWstr((char*) detail_arr,wchar_detail_arr );
    AddOneDetailItem (ctrl_id, TXT_DETAIL_SIZE, PNULL, &nIndex, wchar_detail_arr, string_len);

//set location
    AddOneDetailItem (ctrl_id,
                      TXT_LOCATION ,
                      MMIAPIFMM_GetDeviceName (MMIAPIFMM_GetDeviceTypeByPath (&detail_data->filename[0], MMIFILE_DEVICE_LEN)),
                      &nIndex, PNULL, 0);

    if (MMIFMM_FILE_TYPE_PICTURE == detail_data->type)
    {
        SCI_MEMSET (detail_arr, 0, sizeof (detail_arr));
        SCI_MEMSET (wchar_detail_arr, 0, sizeof (wchar_detail_arr));

        if (0 < detail_data->file_size)
        {
            uint16                      pic_width = 0;
            uint16                      pic_height = 0;
            MMIAPIFMM_GetPictureWidthHeight (
                detail_data->filename, detail_data->filename_len,
                detail_data->file_size,
                &pic_width,
                &pic_height);

            if (0 != pic_width && 0 != pic_height)
            {
                sprintf ( (char *) detail_arr, "%dX%d", (uint16) pic_width, (uint16) pic_height);
                MMIAPICOM_StrToWstr (detail_arr, wchar_detail_arr);
                AddOneDetailItem (ctrl_id, TXT_COMM_RESOLUTION, PNULL, &nIndex, wchar_detail_arr, (uint16) strlen ( (char*) detail_arr));
            }
            else
            {
                AddOneDetailItem (ctrl_id, TXT_COMM_RESOLUTION, TXT_COMMON_NO_SUPPORT, &nIndex, PNULL, 0);
            }
        }
        else
        {
            AddOneDetailItem (ctrl_id, TXT_COMM_RESOLUTION, TXT_EMPTY_FILE, &nIndex, PNULL, 0);
        }
    }

#ifdef MMI_AUDIO_PLAYER_SUPPORT

    //get time duration,song and singer
    if (MMIFMM_FILE_TYPE_MUSIC == detail_data->type)
    {
        MMISRVAUD_RING_FMT_E             audio_type = MMISRVAUD_RING_FMT_MAX;
        MMISRVAUD_CONTENT_INFO_T          mp3_file_info = {0};
#ifdef DRM_SUPPORT

        if (!MMIAPIMP3_IsDrmFile (
                    detail_data->filename,
                    detail_data->filename_len,
                    &audio_type, PNULL, PNULL))
#endif
        {
            audio_type = MMIAPICOM_GetMusicType (detail_data->filename, detail_data->filename_len);
        }

        switch (audio_type)
        {
        case MMISRVAUD_RING_FMT_MP3:
        case MMISRVAUD_RING_FMT_AAC:
        case MMISRVAUD_RING_FMT_M4A:
        case MMISRVAUD_RING_FMT_WMA:
        case MMISRVAUD_RING_FMT_WAVE:
        case MMISRVAUD_RING_FMT_MIDI:
        case MMISRVAUD_RING_FMT_AMR:
            MMISRVAUD_GetFileContentInfo (
                detail_data->filename,
                MMIAPICOM_Wstrlen (detail_data->filename),
                &mp3_file_info
            );
            {
                //set duration
                SCI_MEMSET (detail_arr, 0, sizeof (detail_arr));
                SCI_MEMSET (wchar_detail_arr, 0, sizeof (wchar_detail_arr));
                sprintf ( (char*) detail_arr, "%.2ld:%.2ld:%.2ld", ( (mp3_file_info.total_time) / (60*60)), ( ( (mp3_file_info.total_time) / 60) % 60), ( (mp3_file_info.total_time) % 60));
                MMIAPICOM_StrToWstr (detail_arr, wchar_detail_arr);
                AddOneDetailItem (ctrl_id, TXT_TOTAL_TIME, PNULL, &nIndex, wchar_detail_arr, (uint16) strlen ( (char*) detail_arr));
                //SCI_TRACE_LOW:"[MMIFMM]: tag_code_type=%d"
                SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_2379_112_2_18_2_20_10_230, (uint8*) "d", mp3_file_info.id3_tag.tag_code_type);

                //song name title
                if ( (1 == mp3_file_info.id3_tag.tag_code_type && MMIAPICOM_Wstrlen ( (wchar*) mp3_file_info.id3_tag.title) > 0)
                        || (0 == mp3_file_info.id3_tag.tag_code_type && SCI_STRLEN (mp3_file_info.id3_tag.title) > 0))
                {
                    //SCI_TRACE_LOW:"[MMIFMM]: SONG title DETAIL!"
                    SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_2384_112_2_18_2_20_10_231, (uint8*) "");
                    SCI_MEMSET (wchar_detail_arr, 0, sizeof (wchar_detail_arr));

                    if (1 == mp3_file_info.id3_tag.tag_code_type) //unicode;
                    {
                        string_len = (sizeof (mp3_file_info.id3_tag.title)) / (sizeof (wchar));
                        //这里不能用strlen(),否则遇到包含0x00的unicode会被截断，导致显示不全；
                        string_len = (uint16) MIN (string_len, MMIFMM_FULL_FILENAME_LEN);
                        MMIAPICOM_Wstrncpy (wchar_detail_arr, (wchar*) mp3_file_info.id3_tag.title, string_len);
                    }
                    else
                    {
                        string_len = (uint16) strlen (mp3_file_info.id3_tag.title);
                        string_len = (uint16) MIN (string_len, MMIFMM_FULL_FILENAME_LEN);
                        string_len = (uint16) GUI_GBToWstr (wchar_detail_arr, (uint8*) mp3_file_info.id3_tag.title, string_len);
                    }

                    AddOneDetailItem (ctrl_id, TXT_MP3_SONG, PNULL, &nIndex, wchar_detail_arr, string_len);

                }

                //artist name title
                if ( (1 == mp3_file_info.id3_tag.tag_code_type && MMIAPICOM_Wstrlen ( (wchar*) mp3_file_info.id3_tag.artist) > 0)
                        || (0 == mp3_file_info.id3_tag.tag_code_type && SCI_STRLEN (mp3_file_info.id3_tag.artist) > 0))
                {
                    //SCI_TRACE_LOW:"[MMIFMM]: artist artist DETAIL!"
                    SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_2406_112_2_18_2_20_10_232, (uint8*) "");
                    SCI_MEMSET (wchar_detail_arr, 0, sizeof (wchar_detail_arr));

                    if (1 == mp3_file_info.id3_tag.tag_code_type) //unicode;
                    {
                        string_len = (sizeof (mp3_file_info.id3_tag.artist)) / (sizeof (wchar));
                        //这里不能用strlen(),否则遇到包含0x00的unicode会被截断，导致显示不全；
                        string_len = (uint16) MIN (string_len, MMIFMM_FULL_FILENAME_LEN);
                        MMIAPICOM_Wstrncpy (wchar_detail_arr, (wchar*) mp3_file_info.id3_tag.artist, string_len);
                    }
                    else
                    {
                        string_len = (uint16) strlen (mp3_file_info.id3_tag.artist);
                        string_len = (uint16) MIN (string_len, MMIFMM_FULL_FILENAME_LEN);
                        string_len = (uint16) GUI_GBToWstr (wchar_detail_arr, (uint8*) mp3_file_info.id3_tag.artist, string_len);
                    }

                    AddOneDetailItem (ctrl_id, TXT_MP3_SINGER, PNULL, &nIndex, wchar_detail_arr, string_len);

                }
            }
            break;

        default:
            break;
        }
    }

#endif

#ifdef VIDEO_PLAYER_SUPPORT

    if (MMIFMM_FILE_TYPE_MOVIE == detail_data->type)
    {
        //set pixel title
        DPLAYER_MEDIA_INFO_T media_info = {0};
        //MMIAUDIO_PauseBgPlay(MMIBGPLAY_MODULE_FMM);//video interface need

        MMIAPIVP_GetInfoFromFullPathName (detail_data->filename, detail_data->filename_len, &media_info);

        SCI_MEMSET (detail_arr, 0, sizeof (detail_arr));
        SCI_MEMSET (wchar_detail_arr, 0, sizeof (wchar_detail_arr));
        sprintf ( (char*) detail_arr, "%ldX%ld", media_info.video_info.width, media_info.video_info.height);
        MMIAPICOM_StrToWstr (detail_arr, wchar_detail_arr);
        AddOneDetailItem (ctrl_id, TXT_COMM_RESOLUTION, PNULL, &nIndex, wchar_detail_arr, (uint16) strlen ( (char*) detail_arr));

        //set duration title
        SCI_MEMSET (detail_arr, 0, sizeof (detail_arr));
        SCI_MEMSET (wchar_detail_arr, 0, sizeof (wchar_detail_arr));
        sprintf ( (char*) detail_arr, "%.2ld:%.2ld:%.2ld", ( (media_info.media_length) / (1000*3600)), ( ( (media_info.media_length) / (1000*60)) % 60), ( ( (media_info.media_length) / 1000) % 60));
        MMIAPICOM_StrToWstr (detail_arr, wchar_detail_arr);
        AddOneDetailItem (ctrl_id, TXT_TOTAL_TIME, PNULL, &nIndex, wchar_detail_arr, (uint16) strlen ( (char*) detail_arr));

        //MMIAUDIO_ResumeBgPlay(MMIBGPLAY_MODULE_FMM);
    }

#endif

#ifdef DRM_SUPPORT

    if (MMIFMM_FILE_TYPE_DRM == detail_data->type && 0 < detail_data->filename_len)
    {
        MMIAPIFMM_AddDrmFileDetail (detail_data->filename, SFS_MODE_READ | SFS_MODE_OPEN_EXISTING, 0, 0, ctrl_id);
    }

#endif

}


/*****************************************************************************/
//  Description : add one detail item
//  Global resource dependence :
//  Author:jian.ma
//  Note:
/*****************************************************************************/
LOCAL void AddOneDetailItem (
    MMI_CTRL_ID_T   ctrl_id,
    MMI_TEXT_ID_T   label_title,
    MMI_TEXT_ID_T   label_name,
    uint16          *index_ptr,
    wchar           *detail_ptr,
    uint32          detail_len
)
{
    GUIRICHTEXT_ITEM_T item_data = {0};/*lint !e64*/
    MMI_STRING_T detail_str = {0};/*lint !e64*/

    //set XXX title
    SetCommonCharacter (&item_data);
    MMI_GetLabelTextByLang (label_title, &detail_str);
    item_data.text_data.buf.len = detail_str.wstr_len;
    item_data.text_data.buf.str_ptr = detail_str.wstr_ptr;
    GUIRICHTEXT_AddItem (ctrl_id, &item_data, index_ptr);

    //set XXX name
    if (PNULL == label_name && PNULL != detail_ptr)
    {
        SCI_MEMSET (&item_data, 0, sizeof (item_data));
        SetCommonCharacter (&item_data);
        item_data.text_data.buf.len = (uint16) detail_len;
        item_data.text_data.buf.str_ptr = detail_ptr;
        GUIRICHTEXT_AddItem (ctrl_id, &item_data, index_ptr);
    }
    else if (PNULL != label_name)
    {
        SCI_MEMSET (&item_data, 0, sizeof (item_data));
        SCI_MEMSET (&detail_str, 0, sizeof (detail_str));
        SetCommonCharacter (&item_data);
        MMI_GetLabelTextByLang (label_name, &detail_str);
        item_data.text_data.buf.len = detail_str.wstr_len;
        item_data.text_data.buf.str_ptr = detail_str.wstr_ptr;
        GUIRICHTEXT_AddItem (ctrl_id, &item_data, index_ptr);
    }
    else
    {
        //SCI_TRACE_LOW:"AddOneDetailItem fail "
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_2512_112_2_18_2_20_11_233, (uint8*) "");
    }

}

/*****************************************************************************/
// Description : set common character
// Global resource dependence :
// Author:jian.ma
// Note:
/*****************************************************************************/
LOCAL void SetCommonCharacter (GUIRICHTEXT_ITEM_T* p_item)
{
    //SCI_PASSERT(PNULL != p_item,("[MMIMPEG4]:SetCommonCharacter,param error"));
    if (PNULL == p_item)
    {
        //SCI_TRACE_LOW:"[MMIFMM]:SetCommonCharacter,param error:PNULL == p_item"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_2243_112_2_18_2_20_10_229, (uint8*) "");
        return;
    }

    p_item->img_type = GUIRICHTEXT_IMAGE_NONE;
    p_item->text_type = GUIRICHTEXT_TEXT_BUF;
}

/*****************************************************************************/
//  Description : help other modual to close win when sd plug
//  Global resource dependence : 
//  Author:liyan.zhu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIFMM_CloseOptionWin(void)
{
    //其他模块使用到的窗口关?
	if (MMK_IsOpenWin(MMIFMM_FOLDER_DETAIL_WIN_ID))
	{
		MMK_CloseWin(MMIFMM_FOLDER_DETAIL_WIN_ID);
	}
}
