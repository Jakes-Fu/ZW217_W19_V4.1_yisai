/*****************************************************************************
** File Name:      mmifmm_picwin.c                                           *
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

#define _MMIFMM_PICWIN_C_


/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "window_parse.h"
#include "mmiset_id.h"
#include "mmiset_export.h"
#include "mmi_appmsg.h"
#include "mmifmm_id.h"
#include "mmifmm_text.h"
#include "mmifilearray_export.h"
#include "mmidisplay_data.h"
#include "mmifmm_picwin.h"
#ifdef DRM_SUPPORT
#include "mmidrm_export.h"
#endif
#include "mmipub.h"
#include "guianim.h"
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
//#ifdef SEARCH_SUPPORT
/*****************************************************************************/
// 	Description : handle preview win msg
//	Global resource dependence :
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleFilePicurePreview (
    MMI_WIN_ID_T        win_id,
    MMI_MESSAGE_ID_E    msg_id,
    DPARAM              param
);
/*****************************************************************************/
//  Description : set anim parameter
//  Global resource dependence :
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SearchSetAnimParam (
    BOOLEAN          is_update,  //是否立即刷新
    MMI_WIN_ID_T        win_id,
    MMI_CTRL_ID_T    ctrl_id
);
/*****************************************************************************/
// Description : open ctrl func
// Global resource dependence :
// Author:  jian.ma
// Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E PicPreviewOpenCtrlFunc (MMI_WIN_ID_T win_id);

/*****************************************************************************/
// Description : close ctrl func
// Global resource dependence :
// Author:  jian.ma
// Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E PicPreviewCloseCtrlFunc (MMI_WIN_ID_T win_id);

/*****************************************************************************/
//  Description : handle preview win msg
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSelPicurePreview (
    MMI_WIN_ID_T        win_id,
    MMI_MESSAGE_ID_E    msg_id,
    DPARAM              param
);
/*****************************************************************************/
// Description : ok button cbk
// Global resource dependence :
// Author:
// Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E OKButtonCbk (void);

//the pictures preview window in search
WINDOW_TABLE (MMIFMM_PICTURE_PREVIEW_WIN_TAB) =
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC ( (uint32) HandleFilePicurePreview),
    WIN_ID (MMIFMM_PICTURE_PREVIEW_WIN_ID),
    WIN_TITLE (STXT_DETAIL),
    WIN_STYLE (WS_HAS_ANIM_BG),
    CREATE_ANIM_CTRL (MMIFMM_PIC_PREVIEW_CTRL_ID, MMIFMM_PICTURE_PREVIEW_WIN_ID),
    WIN_SOFTKEY (TXT_NULL, TXT_NULL, STXT_RETURN),
    END_WIN
};

//the pictures preview window
WINDOW_TABLE (MMIFMM_SEL_PIC_PREVIEW_WIN_TAB) =
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_TITLE (TXT_NULL),
    WIN_FUNC ( (uint32) HandleSelPicurePreview),
    WIN_ID (MMIFMM_SEL_PICTURE_PREVIEW_WIN_ID),
    WIN_STYLE (WS_DISABLE_COMMON_BG),
    CREATE_ANIM_CTRL (MMIFMM_SEL_PICTURE_PREVIEW_CTRL_ID, MMIFMM_SEL_PICTURE_PREVIEW_WIN_ID),
    //WIN_SUPPORT_ANGLE (WIN_SUPPORT_ANGLE_CUR),//CR163796
    WIN_SOFTKEY (TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};


/*****************************************************************************/
//  Description : open pic preview win
//  Global resource dependence :
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIFMM_EnterSearchPreviewWin (MMIFMM_PIC_PERVIEW_WIN_DATA_T *file_info_ptr)
{
    FILEARRAY_DATA_T            *file_info = PNULL;
    MMI_RESULT_E                result = MMI_RESULT_FALSE;
    MMI_HANDLE_T                win_handle = 0;

    if (PNULL == file_info_ptr)
    {
        return MMI_RESULT_FALSE;
    }

    if (MMK_IsOpenWin (MMIFMM_SEL_VIDEO_PREVIEW_WIN_ID))
    {
        MMK_CloseWin (MMIFMM_SEL_VIDEO_PREVIEW_WIN_ID);
    }

    file_info = SCI_ALLOC_APPZ (sizeof (MMIFMM_PIC_PERVIEW_WIN_DATA_T));
    SCI_MEMCPY (file_info, file_info_ptr, sizeof (MMIFMM_PIC_PERVIEW_WIN_DATA_T));

    win_handle = MMK_CreateWin ( (uint32*) MMIFMM_PICTURE_PREVIEW_WIN_TAB, (ADD_DATA) file_info);

    if (!win_handle || (MMI_INVALID_ID == win_handle))
    {
        SCI_FREE (file_info);
        result =  MMI_RESULT_FALSE;
    }
    else
    {
        result = MMI_RESULT_TRUE;
    }

    return result;
}
/*****************************************************************************/
//  Description : set anim parameter
//  Global resource dependence :
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SearchSetAnimParam (
    BOOLEAN          is_update,  //是否立即刷新
    MMI_WIN_ID_T        win_id,
    MMI_CTRL_ID_T    ctrl_id
)
{
    BOOLEAN                 result = FALSE;
    GUIANIM_RESULT_E        anim_result = GUIANIM_RESULT_SUCC;
    GUIANIM_CTRL_INFO_T     control_info = {0};
    GUIANIM_FILE_INFO_T     anim_info = {0};
    GUIANIM_DISPLAY_INFO_T  display_info = {0};
    MMIFMM_PIC_PERVIEW_WIN_DATA_T*       file_info = (MMIFMM_PIC_PERVIEW_WIN_DATA_T*) MMK_GetWinUserData (win_id);

    //get picture info
    if (PNULL != file_info)
    {
        //set anim auto adapt display
        //GUIANIM_SetAutoAdaptDisplay(TRUE,ctrl_id);

        //set anim min period
        GUIANIM_SetPeriod (MMITHEME_ANIM_MIN_PERIOD, ctrl_id);

        //set anim param
        control_info.is_ctrl_id = TRUE;
        control_info.ctrl_id = ctrl_id;

        anim_info.full_path_wstr_ptr = file_info->filename;
        anim_info.full_path_wstr_len = (uint16) file_info->name_len;

        display_info.align_style = GUIANIM_ALIGN_HVMIDDLE;
        display_info.is_play_once = FALSE;
        display_info.is_update = is_update;
        display_info.bg.bg_type = GUI_BG_COLOR;
        display_info.bg.color   = MMI_WINDOW_BACKGROUND_COLOR;

        anim_result = GUIANIM_SetParam (&control_info, PNULL, &anim_info, &display_info);

        if ( (GUIANIM_RESULT_SUCC == anim_result) ||
                (GUIANIM_RESULT_WAIT == anim_result))
        {
            result = TRUE;
        }
    }

    return (result);
}


/*****************************************************************************/
// 	Description : handle preview win msg
//	Global resource dependence :
//  Author: jian.ma
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleFilePicurePreview (
    MMI_WIN_ID_T        win_id,
    MMI_MESSAGE_ID_E    msg_id,
    DPARAM              param
)
{
    MMI_RESULT_E            result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T           ctrl_id = MMIFMM_PIC_PREVIEW_CTRL_ID;

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
#ifdef MID_SOFTKEY_USE_IMG_SUPPORT
		GUIWIN_SeSoftkeytButtonIconId(win_id, IMAGE_NULL, 1, TRUE);
#endif
        if (MMI_RESULT_FALSE == PicPreviewOpenCtrlFunc (win_id))
        {
            MMK_CloseWin (win_id);
            break;
        }

        //set anim param
        SearchSetAnimParam (FALSE, win_id, ctrl_id);
        MMK_SetAtvCtrl (win_id, ctrl_id);
        break;

    case MSG_APP_OK:
        break;

    case MSG_APP_CANCEL:
        MMK_CloseWin (win_id);
        break;
    case MSG_CLOSE_WINDOW:
        PicPreviewCloseCtrlFunc (win_id);
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
LOCAL MMI_RESULT_E PicPreviewOpenCtrlFunc (MMI_WIN_ID_T win_id)
{
    MMIFMM_PIC_PERVIEW_WIN_DATA_T *detail_data = (MMIFMM_PIC_PERVIEW_WIN_DATA_T*) MMK_GetWinAddDataPtr (win_id);

    MMK_SetWinUserData (win_id, (void *) detail_data);

    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
// Description : close ctrl func
// Global resource dependence :
// Author:  jian.ma
// Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E PicPreviewCloseCtrlFunc (MMI_WIN_ID_T win_id)
{
    MMIFMM_PIC_PERVIEW_WIN_DATA_T *win_data_p = (MMIFMM_PIC_PERVIEW_WIN_DATA_T*) MMK_GetWinUserData (win_id);

    SCI_FREE (win_data_p);

    return MMI_RESULT_TRUE;
}

//#endif

/*****************************************************************************/
//  Description : handle preview win msg
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSelPicurePreview (
    MMI_WIN_ID_T        win_id,
    MMI_MESSAGE_ID_E    msg_id,
    DPARAM              param
)
{
    MMI_RESULT_E  result = MMI_RESULT_TRUE;
    wchar         *full_path_name_ptr = (uint32) MMK_GetWinAddDataPtr (win_id);/*lint !e64 */
    uint16        full_path_len = 0;

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
#ifdef MID_SOFTKEY_USE_IMG_SUPPORT
		GUIWIN_SeSoftkeytButtonIconId(win_id, IMAGE_NULL, 1, TRUE);
#endif
        //set anim param
        if (full_path_name_ptr != PNULL)
        {
            full_path_len = (uint16) MMIAPICOM_Wstrlen (full_path_name_ptr);
        }

        GUIWIN_SetTitleText (win_id, full_path_name_ptr, full_path_len, FALSE);
#ifdef MMI_PDA_PREVIEW_BUTTON_SUPPORT

      //  if (MMIAPISET_IsWallpaperCall()) //如果是设置墙纸，需要加入button
      //  {
            MMIAPISET_CreatSetButton (win_id, MMIFMM_SEL_PICTURE_PREVIEW_BUTTON_CTRL_ID, (GUIBUTTON_CALLBACK_FUNC) OKButtonCbk, (GUIBUTTON_CALLBACK_FUNC) MMIAPISET_SetCancelButtonCallback);
            MMIAPISET_CreateSetButtonLayer (win_id, MMIFMM_SEL_PICTURE_PREVIEW_BUTTON_CTRL_ID);
      //  }

#endif
        //set anim param
        result = MMIAPIFMM_SetPreviewPicParam (FALSE, full_path_name_ptr, full_path_len, MMIFMM_SEL_PICTURE_PREVIEW_CTRL_ID);
        MMK_SetAtvCtrl (win_id, MMIFMM_SEL_PICTURE_PREVIEW_CTRL_ID);
        break;

    case MSG_FULL_PAINT:
#ifdef MMI_PDA_PREVIEW_BUTTON_SUPPORT

      //  if (MMIAPISET_IsWallpaperCall())
      //  {
            MMIAPISET_AppendSetButtonLayer();//锁屏后，button不能显示的问题
      //  }

#endif
        break;

    case MSG_APP_LEFT:
    case MSG_APP_RIGHT:
    case MSG_APP_DOWN:
    case MSG_APP_UP:
#ifdef MMI_PDA_SUPPORT

        if (MMITHEME_TRIGGER_MODE_TP == MMITHEME_GetPreTiggerMode()
                && MMITHEME_TRIGGER_MODE_KEY == MMITHEME_GetTiggerMode()
           )
        {
            MMK_SetAtvCtrl (win_id, MMIFMM_SEL_PICTURE_PREVIEW_BUTTON_CTRL_ID);
        }
        else
#endif
        {
            if (MSG_APP_RIGHT == msg_id)
            {

                MMK_SetAtvCtrl (win_id, MMISET_SET_CANCEL_BUTTON);
            }
            else if (MSG_APP_LEFT == msg_id)
            {
                MMK_SetAtvCtrl (win_id, MMIFMM_SEL_PICTURE_PREVIEW_BUTTON_CTRL_ID);
            }

        }

        break;
#if defined TOUCH_PANEL_SUPPORT
    case MSG_CTL_PENOK:
#endif
    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
#ifdef MMI_PDA_SUPPORT

        if (MMITHEME_TRIGGER_MODE_TP == MMITHEME_GetPreTiggerMode()
                && MMITHEME_TRIGGER_MODE_KEY == MMITHEME_GetTiggerMode()
           )
        {
            MMK_SetAtvCtrl (win_id, MMIFMM_SEL_PICTURE_PREVIEW_BUTTON_CTRL_ID);
            break;
        }

#endif
        if(GUIANIM_IsSupport(MMIFMM_SEL_PICTURE_PREVIEW_CTRL_ID))
		{
			if (MMIFMM_OPEN_FILE_WIN_ID == MMK_GetPrevWinId (win_id))
			{
				MMK_SendMsg (MMIFMM_OPEN_FILE_BYLAYER_WIN_ID, MSG_APP_OK, PNULL);
			}

#ifndef MMI_PDA_PREVIEW_BUTTON_SUPPORT

        //if (MMIAPISET_IsWallpaperCall()) //如果是设置墙纸，需要加入button
       // {
#ifdef DRM_SUPPORT
			if(full_path_name_ptr != PNULL)
			{
				full_path_len = MMIAPICOM_Wstrlen(full_path_name_ptr);
			}
			else
			{
				SCI_TRACE_LOW("[mmifmm_wintab.c][HandleSelPicurePreview]:full_path_name_ptr == PNULL");
				return MMI_RESULT_FALSE;
			}
			if (!MMIAPIDRM_CanDrmFileSetToWSPC(full_path_name_ptr, full_path_len, DRMFILE_SETTING_WALLPAPER))        
			{
				//不可以设置此DRM文件为壁纸
				MMIPUB_OpenAlertWarningWin(TXT_DRM_COPYRIGHTS_PROTECTION_NOT_OPERATE);
				return MMI_RESULT_FALSE;
			}
#endif
            OKButtonCbk();
      //  }

#else

			if (MMIFMM_SEL_PICTURE_PREVIEW_BUTTON_CTRL_ID == MMK_GetActiveCtrlId (win_id))
			{
#ifdef DRM_SUPPORT
				if(full_path_name_ptr != PNULL)
				{
					full_path_len = MMIAPICOM_Wstrlen(full_path_name_ptr);
				}
				else
				{
					SCI_TRACE_LOW("[mmifmm_wintab.c][HandleSelPicurePreview]:full_path_name_ptr == PNULL");
					return MMI_RESULT_FALSE;
				}
				if (!MMIAPIDRM_CanDrmFileSetToWSPC(full_path_name_ptr, full_path_len, DRMFILE_SETTING_WALLPAPER))        
				{
					//不可以设置此DRM文件为壁纸
					MMIPUB_OpenAlertWarningWin(TXT_DRM_COPYRIGHTS_PROTECTION_NOT_OPERATE);
					return MMI_RESULT_FALSE;
				}
#endif
				OKButtonCbk();
			}
			else
			{
				MMIAPISET_SetCancelButtonCallback();
			}

#endif
		}//end anim_ctrl_ptr->is_support
		else
		{//提示该图片设置失败,普通preview无设置不提示。
			SCI_TRACE_LOW("[mmifmm_wintab.c] HandleSelPicurePreview anim_ctrl_ptr->is_support == FALSE");  
#if defined MMI_WALLPAPER_SUPPORT
			if(MMIAPISET_IsWallpaperCall()
#ifndef MMI_DISPLAYSET_MINI_SUPPORT
				||MMIAPISET_IsPoweronCall()||MMIAPISET_IsPoweroffCall()
#endif
#ifdef SCREENSAVER_SUPPORT
				||MMIAPISET_IsScreensaverCall()
#endif
            )//CR205003
			{
				MMIPUB_OpenAlertFailWin(TXT_ERROR);
			}
#endif
			if (MMK_IsOpenWin(MMIFMM_SEL_PICTURE_PREVIEW_WIN_ID))
			{
				MMK_CloseWin(MMIFMM_SEL_PICTURE_PREVIEW_WIN_ID);
			}
			if(MMK_IsOpenWin(MMIFMM_OPEN_FILE_WIN_ID))
			{
				MMK_CloseWin(MMIFMM_OPEN_FILE_WIN_ID);
			}
		}
        MMK_CloseWin (win_id);
        break;
        
    #ifdef MMI_PDA_PREVIEW_BUTTON_SUPPORT    
    case MSG_NOTIFY_UPDATE:
        if(PNULL != param)
        {
            GUI_RECT_T			*display_rect_ptr = PNULL;
            display_rect_ptr = (GUI_RECT_T*)param;
            
        	if (UILAYER_IsLayerActive(MMIAPISET_GetButtonLayer()))      /*lint !e746 */
        	{
        		UILAYER_ClearRect(MMIAPISET_GetButtonLayer(), *display_rect_ptr);
        	}
        }
        break;
     #endif   
    case MSG_APP_CANCEL:
#ifdef MMI_PDA_PREVIEW_BUTTON_SUPPORT

       // if (MMIAPISET_IsWallpaperCall()) //如果是设置墙纸，需要加入button
       // {
            MMIAPISET_SetCancelButtonCallback();
        //}

#endif
        MMK_CloseWin (win_id);
        break;
    case MSG_CLOSE_WINDOW:
        MMK_FreeWinAddData (win_id);
        break;

    case MSG_CTL_ANIM_DISPLAY_IND:
        {
#ifdef DRM_SUPPORT
            GUIANIM_DISPLAY_IND_T	display_ind = * (GUIANIM_DISPLAY_IND_T *) param;

            if (GUIANIM_RESULT_SUCC == display_ind.result)
            {
                if (MMIAPIDRM_IsDRMFile (SFS_INVALID_HANDLE, full_path_name_ptr))
                {
                    MMIAPIDRM_ConsumeRights (SFS_INVALID_HANDLE, full_path_name_ptr);
                }
            }

#endif
        }
        break;
    default:
        result = MMI_RESULT_FALSE;
        break;
    }

    return (result);
}

/*****************************************************************************/
//  Description : set preview picture anim control param
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
BOOLEAN MMIAPIFMM_SetPreviewPicParam (
    BOOLEAN        is_update,          //in:是否立即刷新
    wchar          *full_path_wstr_ptr,//in
    uint16         full_path_wstr_len, //in:字节数
    MMI_CTRL_ID_T  ctrl_id
)
{
    BOOLEAN                 result = FALSE;
    GUIANIM_RESULT_E        anim_result = GUIANIM_RESULT_SUCC;
    GUIANIM_CTRL_INFO_T     control_info = {0};
    GUIANIM_FILE_INFO_T     file_info = {0};
    GUIANIM_DISPLAY_INFO_T  display_info = {0};

    control_info.is_ctrl_id = TRUE;
    control_info.ctrl_id = ctrl_id;
    control_info.ctrl_ptr = PNULL;

    file_info.full_path_wstr_ptr = full_path_wstr_ptr;
    file_info.full_path_wstr_len = full_path_wstr_len;

    display_info.align_style = GUIANIM_ALIGN_HVMIDDLE;
    display_info.is_play_once = FALSE;
    display_info.is_update = is_update;
    display_info.bg.bg_type = GUI_BG_COLOR;
    display_info.bg.color   = MMI_WINDOW_BACKGROUND_COLOR;

    anim_result = GUIANIM_SetParam (&control_info, PNULL, &file_info, &display_info);

    if ( (GUIANIM_RESULT_SUCC == anim_result) ||
            (GUIANIM_RESULT_WAIT == anim_result))
    {
        result = TRUE;
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
PUBLIC void MMIFMM_EnterPreviewPicture (wchar *full_path_name_ptr)
{
    wchar  *filename_ptr = PNULL;

    filename_ptr = (wchar *) SCI_ALLOCA ( (MMIFILE_FILE_NAME_MAX_LEN + 1) * sizeof (wchar));

    if (filename_ptr != PNULL)
    {
        SCI_MEMSET (filename_ptr, 0x00, ( (MMIFILE_FILE_NAME_MAX_LEN + 1) * sizeof (wchar)));
        MMIAPICOM_Wstrcpy (filename_ptr, full_path_name_ptr);
        MMK_CreateWin ( (uint32*) MMIFMM_SEL_PIC_PREVIEW_WIN_TAB, (ADD_DATA) filename_ptr);
    }
}
/*****************************************************************************/
// Description : ok button cbk
// Global resource dependence :
// Author:
// Note:liyan.zhu modified in 2012.10.31 to format wallpaper,screensaver,poweron/off process
/*****************************************************************************/
LOCAL MMI_RESULT_E OKButtonCbk (void)
{
    MMIFMM_SELECT_RETURN_T          selected_info = {0};
    MMI_WIN_ID_T win_id  = MMIFMM_SEL_PICTURE_PREVIEW_WIN_ID;
    MMI_WIN_ID_T                    set_win_id  = 0;
    wchar         *full_path_name_ptr = (wchar *) MMK_GetWinAddDataPtr (win_id);

    if (PNULL != full_path_name_ptr)
    {
        selected_info.file_name_len = (uint16) MMIAPICOM_Wstrlen (full_path_name_ptr);
        MMIAPICOM_Wstrncpy (selected_info.file_name_ptr, full_path_name_ptr, selected_info.file_name_len);
        selected_info.file_type = MMIFMM_FILE_TYPE_PICTURE;
    }
#if defined MMI_WALLPAPER_SUPPORT
    if (MMIAPISET_IsWallpaperCall())
    {
        SCI_TRACE_LOW("[mmifmm_wintab.c] OKButtonCbk MMIAPISET_IsWallpaperCall");
        set_win_id = MMISET_SET_WALLPAPER_WIN_ID;
    }
#endif
#ifndef MMI_DISPLAYSET_MINI_SUPPORT
    else if (MMIAPISET_IsPoweronCall())
    {
        SCI_TRACE_LOW("[mmifmm_wintab.c] OKButtonCbk MMIAPISET_IsPoweronCall");
        set_win_id = MMISET_SET_POWER_ON_ANIM_WIN_ID;
    }
    else if(MMIAPISET_IsPoweroffCall())
    {
        SCI_TRACE_LOW("[mmifmm_wintab.c] OKButtonCbk MMIAPISET_IsPoweroffCall");
        set_win_id = MMISET_SET_POWER_OFF_ANIM_WIN_ID;
    }
#endif
#ifdef SCREENSAVER_SUPPORT	
    else if (MMIAPISET_IsScreensaverCall())
    {
        SCI_TRACE_LOW("[mmifmm_wintab.c] OKButtonCbk MMIAPISET_IsScreensaverCall");
        set_win_id = MMISET_SET_SCREENSAVER_WIN_ID;
    }
#endif

    if (0 != set_win_id)
    {
        if (MMK_IsOpenWin(MMIFMM_SEL_PICTURE_PREVIEW_WIN_ID))
        {
            MMK_CloseWin(MMIFMM_SEL_PICTURE_PREVIEW_WIN_ID);
        }
        if(MMK_IsOpenWin(MMIFMM_OPEN_FILE_WIN_ID))
        {
            MMK_CloseWin(MMIFMM_OPEN_FILE_WIN_ID);
            MMK_CloseParentWin(MMIFMM_OPEN_FILE_WIN_ID);
        }
        MMK_SendMsg(set_win_id, MSG_MULTIM_SELECTED_RETURN, (void*)&selected_info);
        MMK_CloseWin(win_id);
        return MMI_RESULT_TRUE;
    }
    else
    {
        SCI_TRACE_LOW(" [mmifmm_wintab.c] OKButtonCbk 0 != set_win_id");
        MMK_CloseWin(win_id);
        return MMI_RESULT_FALSE;
    }
}
