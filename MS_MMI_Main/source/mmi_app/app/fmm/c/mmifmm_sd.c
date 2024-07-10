/*****************************************************************************
** File Name:      mmifmm_sd.c                                               *
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

#define _MMIFMM_SD_C_


/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmifmm_sd.h"
#include "mmi_app_fmm_trc.h"
#include "mmifmm_id.h"
#include "mmifmm_text.h"
#include "mmipub.h"
#include "mmisd_export.h"
#include "mmiudisk_export.h"
#include "mmiacc_id.h"
#ifndef VIDEO_PLAYER_SUPPORT
#include "mmi_appmsg.h"
#endif
#ifdef VIDEO_PLAYER_SUPPORT
#include "mmivp_export.h"
#ifdef  VIDEOTHUMBNAIL_SUPPORT
#include "mmivp_internal.h"
#endif
#endif
#include "guitab.h"
#include "gui_ucs2b_converter.h"
#include "mmi_filemgr.h"
#include "mmifmm_comfunc.h"
#include "window_parse.h"
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         TYPE DEFINITION                                  *
 **--------------------------------------------------------------------------*/

typedef struct
{
    MMIFILE_DEVICE_E device_type;      //文件类型
} MMIFMM_MEMCARD_RENAME_WIN_PARAM_T;
/**-------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : HandleFmmQueryWin
//  Global resource dependence :
//      Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleFmmQueryWin (MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);


/*****************************************************************************/
// Description : T卡改名
// Global resource dependence :
// Author:
// Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMemCardRenameWinMsg (
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
LOCAL MMI_RESULT_E MemCardRenameOpenCtrlFunc (MMI_WIN_ID_T win_id);

/*****************************************************************************/
// Description : close ctrl func
// Global resource dependence :
// Author:  jian.ma
// Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E MemCardRenameCloseCtrlFunc (MMI_WIN_ID_T win_id);

WINDOW_TABLE (MMIFMM_MEMCARD_RENAME_WIN_TAB) =
{
    WIN_FUNC ( (uint32) HandleMemCardRenameWinMsg),
    WIN_ID (MMIFMM_MEMCARD_RENAME_WIN_ID),
    WIN_TITLE (TXT_COMMON_RENAME),
#ifdef MMI_PDA_SUPPORT
    CREATE_FORM_CTRL (GUIFORM_LAYOUT_ORDER, MMIFMM_MEMCARD_RENAME_FORM_CTRL_ID),
    CHILD_EDIT_TEXT_CTRL (TRUE, 0, MMIFMM_MEMCARD_RENAME_EDITBOX_CTRL_ID, MMIFMM_MEMCARD_RENAME_FORM_CTRL_ID),
    CHILD_SOFTKEY_CTRL (TXT_COMMON_OK, TXT_NULL, STXT_RETURN, MMIFMM_MEMCARD_RENAME_SOFTKEY_CTRL_ID, MMIFMM_MEMCARD_RENAME_FORM_CTRL_ID),
#else
    WIN_SOFTKEY (TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_EDIT_TEXT_CTRL (0, MMIFMM_MEMCARD_RENAME_EDITBOX_CTRL_ID),
#endif
    END_WIN
};

/*****************************************************************************/
// Description : memory format
// Global resource dependence :
// Author:  jian.ma
// RETRUN:
// Note:
/*****************************************************************************/
PUBLIC void MMIFMM_FmmMemoryFormat (uint32 device, MMI_TEXT_ID_T query_text_id)
{

    MMI_WIN_ID_T  query_win_id = MMIFMM_FORMAT_WIN_ID;
    MMIPUB_OpenQueryWinByTextIdEx (MMK_GetFirstAppletHandle(),
                                   query_text_id,
                                   IMAGE_PUBWIN_QUERY,
                                   &query_win_id,
                                   HandleFmmQueryWin,
                                   (uint32) device);
    //MMIAPIUDISK_OpenFormatWin(device);
}


/*****************************************************************************/
//  Description : HandleFmmQueryWin
//  Global resource dependence :
//      Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleFmmQueryWin (MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    MMI_WIN_ID_T    alert_win_id = MMISDCARD_FORMATTING_WIN_ID;
    MMIPUB_INFO_T   *win_info_ptr = PNULL;
	MMI_STRING_T    str_text = {0};
	GUITAB_TEXT_T   tab_text_str = {0};
    uint32          time_out = 0;
	uint16          cur_tab_index = 0;
    MMIFILE_DEVICE_E device_type = MMI_DEVICE_UDISK;
	MMIFILE_DEVICE_E fmm_dev = MMI_DEVICE_UDISK;

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
        //waiting win
        win_info_ptr = MMIPUB_GetWinAddDataPtr (win_id);

        if (PNULL == win_info_ptr)
        {
            //SCI_TRACE_LOW:"[MMIFMM] HandleFmmQueryWin win_info_ptr == PNULL !"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_7232_112_2_18_2_20_49_462, (uint8*) "");
            break;
        }

        device_type = (MMIFILE_DEVICE_E) win_info_ptr->user_data;
		//CR188283
		MMIPUB_OpenAlertWinByTextIdEx(MMK_GetFirstAppletHandle(),&time_out,TXT_COMMON_WAITING,TXT_NULL,
			IMAGE_PUBWIN_WAIT,&alert_win_id,PNULL,MMIPUB_SOFTKEY_NONE,MMIAPISD_NotInterruptFfs,win_id);
	    break;
       // MMIPUB_OpenAlertWinByTextId (&time_out, TXT_COMMON_WAITING, TXT_NULL, IMAGE_PUBWIN_WAIT, &alert_win_id, PNULL, MMIPUB_SOFTKEY_NONE, MMIAPISD_NotInterruptFfs);

	case MSG_FMM_FORMAT:
		win_info_ptr = MMIPUB_GetWinAddDataPtr(win_id);
		if(PNULL == win_info_ptr)
		{
			//SCI_TRACE_LOW:"[MMIFMM] CASE MSG_FMM_FORMAT:HandleFmmQueryWin win_info_ptr == PNULL !"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIFMM_WINTAB_7232_112_2_18_2_20_49_462,(uint8*)"");
			break;
		}
		device_type = (MMIFILE_DEVICE_E)win_info_ptr->user_data;
#ifndef MAINLCD_SIZE_128X64
        MMIAPISD_HandleBeforeSDCardFormat();
#endif

        if (!MMIAPIFMM_FormatDeviceAsyn (
                    MMIAPIFMM_GetDevicePath (device_type),
                    MMIAPIFMM_GetDevicePathLen (device_type),
                    MMIFILE_FORMAT_AUTO,
                    win_id,
                    MSG_UDISK_FORMAT_CNF
                ))
        {
            //格式化出错
            MMIAPIFMM_UpdateFmmList (device_type, TRUE);
            MMIPUB_OpenAlertWinByTextId (PNULL, MMIAPIFMM_GetDeviceName (device_type), TXT_COMMON_FORMAT_ERROR, IMAGE_PUBWIN_WARNING, PNULL, PNULL, MMIPUB_SOFTKEY_ONE, PNULL);
            MMK_CloseWin (win_id);
            MMK_CloseWin (alert_win_id);
        }
        else
        {
			for(; fmm_dev<MMI_DEVICE_NUM; fmm_dev++)
			{
				if(MMIAPIFMM_GetDeviceTypeStatus(fmm_dev))
				{
					if(device_type == fmm_dev)
					{
						MMI_GetLabelTextByLang(MMIAPIFMM_GetDeviceName(device_type), &str_text); //lint !e661
						tab_text_str.wstr_len = MIN(str_text.wstr_len,GUITAB_MAX_ITEM_NAME_LENGTH);
						MMIAPICOM_Wstrncpy(tab_text_str.wstr, str_text.wstr_ptr, tab_text_str.wstr_len);
						GUITAB_SetItemText(MMIFMM_TAB_CTRL_ID,&tab_text_str,(uint32)cur_tab_index);
						fmm_dev = MMI_DEVICE_NUM;//找到对应tab则退出查找
					}
					cur_tab_index++;
				}
			}
        }
        break;

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin (win_id);
        break;

    case MSG_UDISK_FORMAT_CNF:
        MMIAPIUdisk_HandleFormatCnf (* (BOOLEAN *) param);
        win_info_ptr = MMIPUB_GetWinAddDataPtr (win_id);

        if (PNULL != win_info_ptr)
        {
            device_type = (MMIFILE_DEVICE_E) win_info_ptr->user_data;
            MMIAPIFMM_UpdateFmmList (device_type, TRUE);
        }

        MMK_CloseWin (alert_win_id);
        
#ifdef TCARD_PB_SUPPORT

        if (PNULL != win_info_ptr)
        {            
            device_type = (MMIFILE_DEVICE_E) win_info_ptr->user_data;
            
            if(device_type == MMIAPIFMM_GetFirstValidSD())
            {            
                if(MMIAPIPB_IsTCardContactExist())
                {
                    MMIAPIPB_ResortPB();
                }
            }
        }

#endif
        //Bug 141190 
        MMK_CloseWin (win_id);

        break;
    default:
        result = MMIPUB_HandleQueryWinMsg (win_id, msg_id, param);
        break;
    }

    return result;
}




/*****************************************************************************/
//  Description : stop current operation when sd plug
//  Global resource dependence :
//  Author:robert.lu
//  Note:
/*****************************************************************************/
PUBLIC void MMIFMM_StopOperaSDPlug (void)
{
    MMI_WIN_ID_T            query_win_id = MMIFMM_QUERY_WIN_ID;

    if (MMK_IsOpenWin (MMIFMM_MAIN_WIN_ID))
    {
        //SCI_TRACE_LOW:"MMIFMM:MMIAPIFMM_StopOperaSDPlug close relative windows."
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_8412_112_2_18_2_20_52_477, (uint8*) "");

        //close all prompt win
        MMIPUB_CloseAlertWin();
        MMIPUB_CloseQuerytWin (&query_win_id);

        // close copy window
        if (MMK_IsOpenWin (MMIFMM_COPY_SELECT_FILE_WIN_ID))
        {
            // close select path window
            if (MMK_IsOpenWin (MMIFMM_CHOOSE_PATH_WIN_ID))
            {
                MMK_CloseWin (MMIFMM_CHOOSE_PATH_WIN_ID);
            }

            // close wait window
            if (MMK_IsOpenWin (MMIFMM_FILE_WAITING_WIN_ID))
            {
                MMIPUB_CloseWaitWin (MMIFMM_FILE_WAITING_WIN_ID);
            }

            MMK_CloseWin (MMIFMM_COPY_SELECT_FILE_WIN_ID);
        }

        // close delete window
        if (MMK_IsOpenWin (MMIFMM_DEL_SELECT_FILE_WIN_ID))
        {
            // close wait window
            if (MMK_IsOpenWin (MMIFMM_FILE_WAITING_WIN_ID))
            {
                MMIPUB_CloseWaitWin (MMIFMM_FILE_WAITING_WIN_ID);
            }

            MMK_CloseWin (MMIFMM_DEL_SELECT_FILE_WIN_ID);
        }

#ifdef VIDEO_PLAYER_SUPPORT
#ifdef MMI_VIDEOPLAYER_MINI_FUNCTION
        MMIAPIVP_MiniFunction_ExitVideoPlayer();
#else
        MMIAPIVP_ExitVideoPlayer();
#endif
#endif

#ifdef  MUSIC_PLAYER_SUPPORT
        // close all music win
        MMIAPIMP3_CloseMyDocumentMusicPlay();
#endif

        if (MMK_IsOpenWin (MMIFMM_PIC_PREVIEW_OPT_MENU_WIN_ID))
        {
            MMK_CloseWin (MMIFMM_PIC_PREVIEW_OPT_MENU_WIN_ID);
        }

        if (MMK_IsOpenWin (MMIFMM_PICTURE_PREVIEW_WIN_ID))
        {
            MMK_CloseWin (MMIFMM_PICTURE_PREVIEW_WIN_ID);
        }

        // close create folder window
        if (MMK_IsOpenWin (MMIFMM_CREATE_FOLDER_WIN_ID))
        {
            MMK_CloseWin (MMIFMM_CREATE_FOLDER_WIN_ID);
        }

        // close rename window
        if (MMK_IsOpenWin (MMIFMM_RENAME_WIN_ID))
        {
            MMK_CloseWin (MMIFMM_RENAME_WIN_ID);
        }

        // close find window
        if (MMK_IsOpenWin (MMIFMM_FIND_FILE_WIN_ID))
        {
            MMK_CloseWin (MMIFMM_FIND_FILE_WIN_ID);
        }

        if (MMK_IsOpenWin (MMIFMM_MEMORY_DETAIL_WIN_ID))
        {
            MMK_CloseWin (MMIFMM_MEMORY_DETAIL_WIN_ID);
        }

        if (MMK_IsOpenWin (MMIFMM_FOLDER_DETAIL_WIN_ID))
        {
            MMK_CloseWin (MMIFMM_FOLDER_DETAIL_WIN_ID);
        }

        // close relative menu window
        if (MMK_IsOpenWin (MMIFMM_SEND_OPT_MENU_WIN_ID))
        {
            MMK_CloseWin (MMIFMM_SEND_OPT_MENU_WIN_ID);
        }

        if (MMK_IsOpenWin (MMIFMM_SORT_OPT_MENU_WIN_ID))
        {
            MMK_CloseWin (MMIFMM_SORT_OPT_MENU_WIN_ID);
        }

        if (MMK_IsOpenWin (MMIFMM_EMPTY_FOLDER_MENU_WIN_ID))
        {
            MMK_CloseWin (MMIFMM_EMPTY_FOLDER_MENU_WIN_ID);
        }

        if (MMK_IsOpenWin (MMIFMM_MOVIE_OPT_MENU_WIN_ID))
        {
            MMK_CloseWin (MMIFMM_MOVIE_OPT_MENU_WIN_ID);
        }

        if (MMK_IsOpenWin (MMIFMM_MUSIC_OPT_MENU_WIN_ID))
        {
            MMK_CloseWin (MMIFMM_MUSIC_OPT_MENU_WIN_ID);
        }

        if (MMK_IsOpenWin (MMIFMM_PIC_OPT_MENU_WIN_ID))
        {
            MMK_CloseWin (MMIFMM_PIC_OPT_MENU_WIN_ID);
        }

        if (MMK_IsOpenWin (MMIFMM_NORMAL_OPT_MENU_WIN_ID))
        {
            MMK_CloseWin (MMIFMM_NORMAL_OPT_MENU_WIN_ID);
        }

        // close fmm main window
        MMK_CloseParentWin (MMIFMM_MAIN_WIN_ID);
    }

    if (MMK_IsOpenWin (MMIFMM_GETFILE_WIN_ID))
    {
        MMK_CloseWin (MMIFMM_GETFILE_WIN_ID);
    }
}
/*****************************************************************************/
// Description : enter rename win
// Global resource dependence :
// Author:  jian.ma
// Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIFMM_EnterMemCardReNameWin (
    MMIFILE_DEVICE_E device_type// 数据
)
{
    MMIFMM_MEMCARD_RENAME_WIN_PARAM_T  *memcard_rename_win = PNULL;

    if (MMK_IsOpenWin (MMIFMM_MEMCARD_RENAME_WIN_ID))
    {
        MMK_CloseWin (MMIFMM_MEMCARD_RENAME_WIN_ID);
    }

    memcard_rename_win = SCI_ALLOC_APPZ (sizeof (MMIFMM_MEMCARD_RENAME_WIN_PARAM_T));
    memcard_rename_win->device_type = device_type;

    MMK_CreateWin ( (uint32 *) MMIFMM_MEMCARD_RENAME_WIN_TAB , (ADD_DATA) memcard_rename_win);

    return MMI_RESULT_TRUE;
}
/*****************************************************************************/
// Description :
// Global resource dependence :
// Author:
// Note:
/*****************************************************************************/
LOCAL BOOLEAN  IsIncludeInvlidChar(MMI_STRING_T *new_name_ptr)
{
    uint8 i = 0;
    BOOLEAN result = FALSE;
    const uint8 invalid_char[] = {'.',','};
    int32 j = 0;
    uint8 invalid_char_num  = sizeof(invalid_char) ;
    uint8 str_len = 0;

    if (new_name_ptr == PNULL )/* !e774 */
    {
        return TRUE;
    }    
    str_len = MIN(new_name_ptr->wstr_len,MMIFILE_NEW_DEVICE_NAME_MAX_LEN);//coverity 12977

    if(0 == str_len)
    {
        return TRUE;
    }
    for(i = 0; i < str_len; i++)
    {
        for (j=0; j< invalid_char_num; j++)
        {
            if(new_name_ptr->wstr_ptr[i] == invalid_char[j])
            {
                result = TRUE;
                break;
            }
        }
    }
    return result;
}
/*****************************************************************************/
// Description : T卡改名
// Global resource dependence :
// Author:
// Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMemCardRenameWinMsg (
    MMI_WIN_ID_T         win_id,
    MMI_MESSAGE_ID_E     msg_id,
    DPARAM               param
)
{
    MMI_RESULT_E                result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T               ctrl_id = MMIFMM_MEMCARD_RENAME_EDITBOX_CTRL_ID;
    MMI_STRING_T                name_str = {0};
    GUITAB_TEXT_T               item_text = {0};
    char                        buffer_name[MMIFILE_NEW_DEVICE_NAME_MAX_LEN*3+1] = {0};
    wchar                       get_name[MMIFMM_FULL_FILENAME_LEN+1] = {0};
    wchar                       *dev_path = PNULL;
    uint8                       temp_len = 0;
	//memcard rename分开
	MMIFILE_DEVICE_E           fmm_dev = MMI_DEVICE_UDISK;
	uint16                      cur_tab_index = 0;
    
    MMIFMM_MEMCARD_RENAME_WIN_PARAM_T *win_d  = (MMIFMM_MEMCARD_RENAME_WIN_PARAM_T *) MMK_GetWinUserData (win_id);
    uint16 temp_wstr_len = 0;//pclint bug 2099193

    SCI_TRACE_LOW ("MMIFMM: HandleMemCardRenameWinMsg  msg_id = 0x%x", msg_id);

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
#ifdef MID_SOFTKEY_USE_IMG_SUPPORT
		GUIWIN_SeSoftkeytButtonIconId(win_id, IMAGE_NULL, 1, TRUE);
#endif 
        if (MMI_RESULT_FALSE == MemCardRenameOpenCtrlFunc (win_id))
        {
            MMK_CloseWin (win_id);
            break;
        }

        win_d = (MMIFMM_MEMCARD_RENAME_WIN_PARAM_T *) MMK_GetWinUserData (win_id);

        if (PNULL == win_d)
        {
            MMK_CloseWin (win_id);
            break;
        }

        dev_path = MMIAPIFMM_GetDevicePath (win_d->device_type);
        MMIAPIFMM_GetDeviceLabel (dev_path, buffer_name);

        SCI_TRACE_LOW ("HandleMemCardRenameWinMsg, buffer_ptr = %s", buffer_name);
        //set left softkey
        GUIEDIT_SetSoftkey (ctrl_id, 0, 1, TXT_NULL, TXT_COMMON_OK, PNULL);
        GUIEDIT_SetTextMaxLen (ctrl_id, MMIFILE_NEW_DEVICE_NAME_MAX_LEN , MMIFILE_NEW_DEVICE_NAME_MAX_LEN);

		GUI_OtherCodeToWstr(get_name,MMIFMM_FULL_FILENAME_LEN,GUI_GetCodeType(buffer_name,strlen (buffer_name)),(uint8 *)buffer_name,strlen(buffer_name));
        //GUI_UTF8ToWstr (get_name, MMIFILE_NEW_DEVICE_NAME_MAX_LEN, (uint8 *) buffer_name, strlen (buffer_name));
        //GUI_OtherCodeToWstr(get_name,MMIFILE_NEW_DEVICE_NAME_MAX_LEN,GUI_CHAR_CODE_ANSI,  (uint8 *)buffer_name,strlen(buffer_name));
        temp_wstr_len = (uint16)MMIAPICOM_Wstrlen (get_name);
        temp_len = MIN(temp_wstr_len,MMIFILE_NEW_DEVICE_NAME_MAX_LEN);
        GUIEDIT_SetString (ctrl_id, get_name, temp_len);
        MMK_SetAtvCtrl (win_id, ctrl_id);
        break;

    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_CTL_MIDSK:
        GUIEDIT_GetString (ctrl_id, &name_str);

        if (0 == name_str.wstr_len || MMIFILE_NEW_DEVICE_NAME_MAX_LEN < name_str.wstr_len)
        {
            MMIPUB_OpenAlertWinByTextId (PNULL, TXT_DC_NAMEINVALID, TXT_NULL, IMAGE_PUBWIN_WARNING , PNULL, PNULL, MMIPUB_SOFTKEY_ONE, PNULL);
            break;
        }

        if (MMIFMM_NewFolderNameStringIsValid (&name_str, MMIFILE_NEW_DEVICE_NAME_MAX_LEN)
            && !IsIncludeInvlidChar(&name_str))
        {
            GUI_WstrToUTF8 ( (uint8 *) buffer_name, MMIFILE_NEW_DEVICE_NAME_MAX_LEN*3, name_str.wstr_ptr, name_str.wstr_len);
            //GUI_WstrToOtherCode(GUI_CHAR_CODE_ANSI, (uint8 *)buffer_name, MMIFILE_NEW_DEVICE_NAME_MAX_LEN*2, (const wchar *)name_str.wstr_ptr,name_str.wstr_len);
            dev_path = MMIAPIFMM_GetDevicePath (win_d->device_type);

            if (MMIAPIFMM_SetDeviceLabel (dev_path, buffer_name, strlen (buffer_name)))
            {
                SCI_MEMSET (buffer_name, 0x00, sizeof( buffer_name));

                MMIAPIFMM_GetDeviceLabel (dev_path, buffer_name);
                //GUI_OtherCodeToWstr( get_name,MMIFILE_NEW_DEVICE_NAME_MAX_LEN,GUI_CHAR_CODE_ANSI,  (uint8 *)buffer_name,strlen(buffer_name));
				GUI_UTF8ToWstr (get_name, MMIFILE_NEW_DEVICE_NAME_MAX_LEN, (uint8 *) buffer_name, strlen (buffer_name));
                MMIAPICOM_Wstrncpy (item_text.wstr, get_name, MMIAPICOM_Wstrlen (get_name));
                item_text.wstr_len = (uint16) MMIAPICOM_Wstrlen (item_text.wstr);
               // GUITAB_SetItemText (MMIFMM_TAB_CTRL_ID, &item_text, GUITAB_GetCurSel (MMIFMM_TAB_CTRL_ID));
				for(; fmm_dev<MMI_DEVICE_NUM; fmm_dev++)
				{
					if(MMIAPIFMM_GetDeviceTypeStatus(fmm_dev))
					{
						if(win_d->device_type == fmm_dev)
						{
							GUITAB_SetItemText(MMIFMM_TAB_CTRL_ID,&item_text,(uint32)cur_tab_index);
							fmm_dev = MMI_DEVICE_NUM;//找到对应tab则退出查找
						}
						cur_tab_index++;
					}
				}
            }
            else
            {
                MMIPUB_OpenAlertWinByTextId (PNULL, TXT_COMMON_SETTING_FAIL, TXT_NULL, IMAGE_PUBWIN_WARNING , PNULL, PNULL, MMIPUB_SOFTKEY_ONE, PNULL);
                break;
            }
        }
        else
        {
            MMIFMM_PromptFileErrorType (PNULL, SFS_ERROR_INVALID_PARAM, win_d->device_type, TRUE);
        }

        MMK_CloseWin (win_id);
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
        MemCardRenameCloseCtrlFunc (win_id);
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
LOCAL MMI_RESULT_E MemCardRenameOpenCtrlFunc (MMI_WIN_ID_T win_id)
{
    MMIFMM_MEMCARD_RENAME_WIN_PARAM_T *win_data_p = (MMIFMM_MEMCARD_RENAME_WIN_PARAM_T*) MMK_GetWinAddDataPtr (win_id);

    MMK_SetWinUserData (win_id, (void *) win_data_p);

    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
// Description : close ctrl func
// Global resource dependence :
// Author:  jian.ma
// Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E MemCardRenameCloseCtrlFunc (MMI_WIN_ID_T win_id)
{

    MMIFMM_MEMCARD_RENAME_WIN_PARAM_T *win_data_p = (MMIFMM_MEMCARD_RENAME_WIN_PARAM_T*) MMK_GetWinUserData (win_id);
    SCI_FREE (win_data_p);
    //MMICL_UnRegisterDateChangeEvent(MMICL_CALLTIMELIST_WIN_FLAG);

    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
//  Description : check memory card status
//  Return:
//  Global resource dependence :
//  Author:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFMM_CheckMemoryCard (MMIFILE_DEVICE_E file_dev)
{
    BOOLEAN result = FALSE;
    MMI_TEXT_ID_T alert_text_id = TXT_NULL;
    MMI_TEXT_ID_T device_text_id = TXT_NULL;

    if (MMIAPIUDISK_UdiskIsRun())
    {
        //u disk runing
        alert_text_id = TXT_COMMON_UDISK_USING;
        result = FALSE;
    }
    else if (file_dev >= MMI_DEVICE_NUM)
    {
        //no sd card
        /* Modify by michael on 2/27/2012 for NEWMS00172443 : 统一修改弹出信息*/
        //alert_text_id = TXT_SD_NO_EXIST;
        alert_text_id = TXT_PLEASE_INSERT_SD;
        /************************Modify over*************************/
        result = FALSE;
    }
    else if ( (MMI_DEVICE_SDCARD == file_dev) && MMIAPISD_IsCardLogOn())
    {
        //card log runging
        device_text_id = MMIAPIFMM_GetDeviceName (file_dev);
        alert_text_id = TXT_COMMON_CARDLOG_USING;
        result = FALSE;
    }
    else
    {
        BOOLEAN need_format = FALSE;
        //device_type = MMIFILE_GetDeviceTypeByFileDev(file_dev);
        device_text_id = MMIAPIFMM_GetDeviceName (file_dev);

        if (MMIAPIFMM_GetDeviceStatus ( (uint16 *) MMIAPIFMM_GetDevicePath (file_dev), MMIAPIFMM_GetDevicePathLen (file_dev)))
        {
            result = TRUE;
        }
        else
        {
            MMIFILE_ERROR_E device_error = SFS_NO_ERROR;
            device_error = MMIAPIFMM_RegisterDevice ( (uint16 *) MMIAPIFMM_GetDevicePath (file_dev), MMIAPIFMM_GetDevicePathLen (file_dev));

            if (SFS_NO_ERROR == device_error)
            {
                result = TRUE;
            }

            if (SFS_ERROR_INVALID_FORMAT == device_error)
            {
                need_format = TRUE;
            }
        }

        if (!result && (!need_format))
        {
            //device not exist
            alert_text_id = TXT_COMMON_NOT_EXIST;
            //result = FALSE;
        }
        else if (!result)
        {
            alert_text_id = TXT_COMMON_FORMAT_ERROR;
        }
    }

    if (!result)
    {
        //show error message
        MMIPUB_OpenAlertWinByTextId (PNULL, device_text_id, alert_text_id, IMAGE_PUBWIN_WARNING, PNULL, PNULL, MMIPUB_SOFTKEY_ONE, PNULL);
    }

    //SCI_TRACE_LOW("[baokun] MMIAPIFMM_CheckMemoryCard file_dev %d result %d", file_dev, result);
    return result;
}
