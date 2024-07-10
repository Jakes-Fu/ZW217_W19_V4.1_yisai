/*****************************************************************************
** File Name:      mmifmm_videowin.c                                         *
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

#define _MMIFMM_VIDEOVIWWIN_C_


/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_app_fmm_trc.h"
#include "mmifmm_videowin.h"
#include "window_parse.h"
#include "mmifmm_id.h"
#include "mmifmm_text.h"
#include "mmiset_id.h"
#include "mmi_appmsg.h"
#include "mmi_filemgr.h"
#ifdef VIDEO_PLAYER_SUPPORT
#include "mmivp_export.h"
#else
#include "mmi_common.h"
#endif
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
//  Description : handle preview video win msg
//  Global resource dependence :
//  Author: murphy.xie
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleVideoPreviewWinMsg (
    MMI_WIN_ID_T        win_id,
    MMI_MESSAGE_ID_E    msg_id,
    DPARAM              param
);

/*****************************************************************************/
// Description : open ctrl func
// Global resource dependence :
// Author:  jian.ma
// Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E VideoPreviewOpenCtrlFunc (MMI_WIN_ID_T win_id);

/*****************************************************************************/
// Description : close ctrl func
// Global resource dependence :
// Author:  jian.ma
// Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E VideoPreviewCloseCtrlFunc (MMI_WIN_ID_T win_id);
#ifdef VIDEO_PLAYER_SUPPORT
/*****************************************************************************/
// 	Description : HandleFmmVideoPreviewCallback
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
LOCAL void HandleFmmVideoPreviewCallback (VP_CALLBACK_T *msg_ptr);
#endif
// the window for preview video
WINDOW_TABLE (MMIFMM_SEL_VIDEO_PREVIEW_WIN_TAB) =
{
    //CLEAR_LCD,
    WIN_FUNC ( (uint32) HandleVideoPreviewWinMsg),
    WIN_ID (MMIFMM_SEL_VIDEO_PREVIEW_WIN_ID),
    WIN_MOVE_STYLE (MOVE_FORBIDDEN),
    WIN_SUPPORT_ANGLE (WIN_SUPPORT_ANGLE_INIT),
    WIN_SOFTKEY (TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};
/*****************************************************************************/
// 	Description : to handle the screen saver window message
//	Global resource dependence :
//  Author:murphy.xie
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleVideoPreviewWinMsg (
    MMI_WIN_ID_T         win_id,
    MMI_MESSAGE_ID_E     msg_id,
    DPARAM               param
)
{
    MMI_RESULT_E                        result = MMI_RESULT_TRUE;
#ifdef VIDEO_PLAYER_SUPPORT
    VP_PLAY_PARAM_T                     play_param  = {0};
    GUI_RECT_T                          full_rect = {0};
#endif
	//coverity 13030
    wchar*                              full_path_name_ptr = PNULL;//(wchar*) MMK_GetWinUserData (win_id);/*lint !e64 */
    uint16                              full_path_len = 0;
    IGUICTRL_T* ctrl_handle = PNULL;

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
#ifdef MID_SOFTKEY_USE_IMG_SUPPORT
		GUIWIN_SeSoftkeytButtonIconId(win_id, IMAGE_NULL, 1, TRUE);
#endif
        if (MMI_RESULT_FALSE == VideoPreviewOpenCtrlFunc (win_id))
        {
            MMK_CloseWin (win_id);
            break;
        }

        MMIDEFAULT_StopAutoKeylockTimer();
        MMIDEFAULT_StopLcdBackLightTimer();
        full_path_name_ptr = (wchar*) MMK_GetWinUserData (win_id);

        //set anim param
        if (full_path_name_ptr != PNULL)
        {
            full_path_len = (uint16) MMIAPICOM_Wstrlen (full_path_name_ptr);
        }

        //设置状态栏不可见
        ctrl_handle = MMK_GetCtrlPtrByWin (MMK_GetWinHandle (MMK_GetFirstAppletHandle(), win_id), MMITHEME_GetStatusBarCtrlId());

        if (0 != ctrl_handle)
        {
            IGUICTRL_SetState (ctrl_handle, GUICTRL_STATE_DISABLE_ACTIVE | GUICTRL_STATE_INVISIBLE | GUICTRL_STATE_DISABLE_TP, TRUE);
        }

#ifdef VIDEO_PLAYER_SUPPORT
        {
            play_param.dual_sys = MN_DUAL_SYS_1;
            play_param.full_path_len = full_path_len;
            play_param.full_path_ptr = full_path_name_ptr;
            play_param.is_random_play = FALSE;
            play_param.is_scal_up = TRUE;
            play_param.mode = VP_REPEAT_PLAY_MODE_ONE;
            play_param.play_type = VP_PLAY_FROM_FILE;
            play_param.url_ptr = PNULL;
            play_param.video_buf_info.type = 0;
            play_param.video_buf_info.video_buf_len = 0;
            play_param.video_buf_info.video_buf_ptr = PNULL;
            play_param.display_param.audioSupport = 1;
            play_param.display_param.videoSupport = 1;

            play_param.display_param.RotateAngle = GUILCD_GetLogicAngle (GUI_MAIN_LCD_ID);

            full_rect = MMITHEME_GetFullScreenRect();

            if (LCD_ANGLE_0 == play_param.display_param.RotateAngle)
            {
                play_param.display_param.RotateAngle = LCD_ANGLE_90;
                play_param.display_param.disp_rect.dy = full_rect.right - full_rect.left + 1;
                play_param.display_param.disp_rect.dx = full_rect.bottom - full_rect.top + 1;
            }
            else if (LCD_ANGLE_180 == play_param.display_param.RotateAngle)
            {
                play_param.display_param.RotateAngle = LCD_ANGLE_270;
                play_param.display_param.disp_rect.dy = full_rect.right - full_rect.left + 1;
                play_param.display_param.disp_rect.dx = full_rect.bottom - full_rect.top + 1;
            }
            else
            {
                play_param.display_param.disp_rect.dx = full_rect.right - full_rect.left + 1;
                play_param.display_param.disp_rect.dy = full_rect.bottom - full_rect.top + 1;
            }

            play_param.display_param.target_rect.dx = play_param.display_param.disp_rect.dx;
            play_param.display_param.target_rect.dy = play_param.display_param.disp_rect.dy;
            play_param.display_param.target_rect.x = play_param.display_param.disp_rect.x;
            play_param.display_param.target_rect.y = play_param.display_param.disp_rect.y;
            play_param.display_param.disp_mode = DPLAYER_DISP_CUSTOMIZE;

            play_param.vol = 3;

            if (!MMIAPIVP_Play (&play_param, HandleFmmVideoPreviewCallback, 0))
            {
                MMK_CloseWin (win_id);

            }
        }
#endif
        break;

    case MSG_MPEG4_END_CALLBACK:
        MMK_CloseWin (win_id);

        break;

    case MSG_BACKLIGHT_TURN_OFF:
    case MSG_LOSE_FOCUS:
#if defined TOUCH_PANEL_SUPPORT
    case MSG_TP_PRESS_UP:
#endif
        MMK_CloseWin (win_id);
        break;

#ifdef SCREENSAVER_SUPPORT
#if defined TOUCH_PANEL_SUPPORT
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:

        if (MMK_IsOpenWin (MMISET_SET_SCREENSAVER_WIN_ID) && (MMIFMM_OPEN_FILE_WIN_ID == MMK_GetPrevWinId (win_id)))
        {
            MMK_SendMsg (MMIFMM_OPEN_FILE_BYLAYER_WIN_ID, MSG_APP_OK, PNULL);
        }

        MMK_CloseWin (win_id);
        break;
#endif
    case MSG_CLOSE_WINDOW:
        VideoPreviewCloseCtrlFunc (win_id);
#ifdef VIDEO_PLAYER_SUPPORT
        MMIAPIVP_ExitPlay();
#endif
        MMIDEFAULT_StartAutoKeylockTimer();
        MMIDEFAULT_StartSettingLCDBackLightTimer();
        break;

    default:

        if ( ( (msg_id >= MSG_KEYDOWN_UP) &&
                (msg_id <= MSG_KEYDOWN_MIDDLE))
                || (MSG_KEYUP_SLIDE == msg_id)
                || (MSG_KEYDOWN_TV == msg_id)
           )
        {
#ifdef LCD_SLIDE_SUPPORT

            if (MSG_KEYUP_SLIDE == msg_id)
            {
                MMIDEFAULT_HandleSlideKey (MSG_KEYUP_SLIDE);
            }

#endif
#ifdef VIDEO_PLAYER_SUPPORT
            MMIAPIVP_ExitPlay();
#endif
            MMK_CloseWin (win_id);
        }
        else
        {
            result = MMI_RESULT_FALSE;
        }

        break;
    }

    return (result);
}


/*****************************************************************************/
// Description : preview current video file
// Global resource dependence :
// Author:  murphy.xie
// RETRUN:
// Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIFMM_EnterVideoPreviewWin (wchar *full_path_name_ptr)
{
    wchar                       *filename_ptr = PNULL;
    MMI_RESULT_E                result = MMI_RESULT_FALSE;
    MMI_HANDLE_T                win_handle = 0;

    if (PNULL == full_path_name_ptr)
    {
        return MMI_RESULT_FALSE;
    }

    if (MMK_IsOpenWin (MMIFMM_SEL_VIDEO_PREVIEW_WIN_ID))
    {
        MMK_CloseWin (MMIFMM_SEL_VIDEO_PREVIEW_WIN_ID);
    }

    filename_ptr = (wchar *) SCI_ALLOCA ( (MMIFILE_FILE_NAME_MAX_LEN + 1) * sizeof (wchar));

    if (filename_ptr != PNULL)
    {
        SCI_MEMSET (filename_ptr, 0x00, ( (MMIFILE_FILE_NAME_MAX_LEN + 1) * sizeof (wchar)));
        MMIAPICOM_Wstrcpy (filename_ptr, full_path_name_ptr);
        win_handle = MMK_CreateWin ( (uint32*) MMIFMM_SEL_VIDEO_PREVIEW_WIN_TAB, (ADD_DATA) filename_ptr);

        if (!win_handle || (MMI_INVALID_ID == win_handle))
        {
            SCI_FREE (filename_ptr);
            result =  MMI_RESULT_FALSE;
        }
        else
        {
            result = MMI_RESULT_TRUE;
        }
    }

    return result;
}

/*****************************************************************************/
// Description : open ctrl func
// Global resource dependence :
// Author:  jian.ma
// Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E VideoPreviewOpenCtrlFunc (MMI_WIN_ID_T win_id)
{
    wchar *detail_data = (wchar*) MMK_GetWinAddDataPtr (win_id);

    MMK_SetWinUserData (win_id, (void *) detail_data);

    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
// Description : close ctrl func
// Global resource dependence :
// Author:  jian.ma
// Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E VideoPreviewCloseCtrlFunc (MMI_WIN_ID_T win_id)
{
    wchar *win_data_p = (wchar*) MMK_GetWinUserData (win_id);

    SCI_FREE (win_data_p);

    return MMI_RESULT_TRUE;
}

#ifdef VIDEO_PLAYER_SUPPORT
/*****************************************************************************/
// 	Description : HandleFmmVideoPreviewCallback
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
LOCAL void HandleFmmVideoPreviewCallback (VP_CALLBACK_T *msg_ptr)
{
    VP_PLAY_END_IND_T  *play_end_ind_ptr = PNULL;
    VP_PLAY_END_RESULT_E play_end_result = 0;

    if (PNULL == msg_ptr)
    {
        return;
    }


    //SCI_TRACE_LOW:"[VP] HandleVideoScreenSaverMsg msg_ptr->msg_id = %d"
    SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_WINTAB_4517_112_2_18_2_20_44_404, (uint8*) "d", msg_ptr->msg_id);

    switch (msg_ptr->msg_id)
    {
    case VP_PLAY_END_IND:
        play_end_ind_ptr = (VP_PLAY_END_IND_T *) msg_ptr->para;
        play_end_result = play_end_ind_ptr->result;
        break;

    case VP_STOP_IND:
        play_end_result = VP_PLAY_END_STOPPED_BY_USER;
        break;

    case VP_PLAY_PROCESS_IND:
        //MMITHEME_UpdateRect();
        return;

    default:
        return;
    }

    MMK_PostMsg (MMIFMM_SEL_VIDEO_PREVIEW_WIN_ID, MSG_MPEG4_END_CALLBACK, &play_end_result, sizeof (VP_PLAY_END_RESULT_E));

}
#endif