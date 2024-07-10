/*****************************************************************************
** File Name:        audioplayer_demoapp_play_win_view.c                         *
** Author:           zirui.li                                                *
** Date:             12/13/2021                                              *
** Copyright:                                                                *
** Description: This file is used to define audioplayer play view function   *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE                 NAME                  DESCRIPTION                    *
** 12/13/2021           zirui.li              Create                         *
******************************************************************************/
#include "window_parse.h"
#include "mmimp3_image.h"
#include "mmimp3_id.h"
#include "mmitheme_prgbox.h"
#include "guiblock.h"
#include "mmidisplay_color.h"
#include "guilabel.h"
#include "guibutton.h"
#include "audioplayer_demoapp_flow.h"
#include "audioplayer_demoapp_play_win_view.h"
#include "audioplayer_demoapp_play_win_data.h"

#define AP_SINGER_RECT      DP2PX_RECT(60,16,180,32)
#define AP_TITLE_RECT       DP2PX_RECT(36,40,208,60)
#define AP_PROGRESS_RECT    DP2PX_RECT(76,64,164,69)
#define AP_PREV_RECT        DP2PX_RECT(20,102,64,138)
#define AP_PLAY_RECT        DP2PX_RECT(76,76,164,164)
#define AP_PAUSE_RECT       DP2PX_RECT(76,76,164,164)
#define AP_NEXT_RECT        DP2PX_RECT(184,102,220,138)
#define AP_FAV_RECT         DP2PX_RECT(58,178,94,214)
#define AP_VOLUME_RECT      DP2PX_RECT(102,178,138,214)
#define AP_DOWNLOAD_RECT    DP2PX_RECT(146,178,182,214)
#define AP_LISTIND_RECT     DP2PX_RECT(122,224,130,232)
#define AP_MUSIND_RECT      DP2PX_RECT(110,224,118,232)

#define AUDIOPLAYER_PRGBAR_TOTAL_COUNT      100
#define AUDIOPLAYER_PRGBAR_BG_COLOR         0x8410
#define AUDIOPLAYER_PRGBAR_FG_COLOR         0x8000
#define AUDIO_SOURCE_TAG_LEN    32  //artist,album描述一般长度，与AUDIO_ID3_TAG_T对应

/*****************************************************************************/
//  Description: play和pause button的显隐
//  Parameter: [In] status //音频播放器的状态
//             [Out] None
//             [Return] None
//  Author: zirui.li
//  Note:
/*****************************************************************************/
LOCAL void AudioPlayer_View_SetBtnVisibleStatus(AP_STATUS_E status)
{
    if (AP_STATUS_PLAYING == status)
    {
        GUIBUTTON_SetVisible(AP_PAUSE_BUTTON_CTRL_ID, TRUE,  FALSE);
        GUIBUTTON_SetVisible(AP_PLAY_BUTTON_CTRL_ID,  FALSE, FALSE);
    }
    else if(AP_STATUS_PAUSE == status)
    {
        GUIBUTTON_SetVisible(AP_PAUSE_BUTTON_CTRL_ID, FALSE, FALSE);
        GUIBUTTON_SetVisible(AP_PLAY_BUTTON_CTRL_ID,  TRUE,  FALSE);
    }
    else if(AP_STATUS_STOP == status)
    {
        GUIBUTTON_SetVisible(AP_PAUSE_BUTTON_CTRL_ID, FALSE, FALSE);
        GUIBUTTON_SetVisible(AP_PLAY_BUTTON_CTRL_ID,  TRUE,  FALSE);
    }
    else
    {
        return;
    }
}

/*****************************************************************************/
//  Description: Audioplayer test play win open window
//  Parameter: [In] win_id   //窗口id
//             [Out] None
//             [Return] None
//  Author: zirui.li
//  Note:
/*****************************************************************************/
LOCAL void Audioplayer_View_PlayWin_OpenWindow(MMI_WIN_ID_T win_id)
{
    GUI_RECT_T              rectsinger      = AP_SINGER_RECT;
    GUI_RECT_T              rectname        = AP_TITLE_RECT;
    GUI_RECT_T              rectplay        = AP_PLAY_RECT;
    GUI_RECT_T              rectpause       = AP_PAUSE_RECT;
    GUI_RECT_T              rectprev        = AP_PREV_RECT;
    GUI_RECT_T              rectnext        = AP_NEXT_RECT;
    GUI_RECT_T              rectfav         = AP_FAV_RECT;
    GUI_RECT_T              rectspeaker     = AP_VOLUME_RECT;
    GUI_RECT_T              rectdl          = AP_DOWNLOAD_RECT;
    GUI_RECT_T              rectprogrebar   = AP_PROGRESS_RECT;
    GUI_RECT_T              rectlistind     = AP_LISTIND_RECT;
    GUI_RECT_T              rectind         = AP_MUSIND_RECT;
    MMI_CONTROL_CREATE_T    create          = {0};
    GUIPRGBOX_INIT_DATA_T   init_data       = {0};
    MMI_CTRL_ID_T           ctrl_id         = AP_PRGBAR_CTRL_ID;
    IGUICTRL_T*             p_ctrl          = PNULL;
    MMI_HANDLE_T            progress_handle = NULL;
    AP_STATUS_E             status          = AP_STATUS_MAX;
    // display background
    WATCHCOM_DisplayBackground(win_id);
    // singer
    GUILABEL_SetRect(AP_SINGER_LABEL_CTRL_ID, &rectsinger, FALSE);
    GUILABEL_SetFont(AP_SINGER_LABEL_CTRL_ID, DP_FONT_16, 0xffff);
    GUILABEL_SetIsAutoScroll(AP_SINGER_LABEL_CTRL_ID, TRUE);
    // audio name
    GUILABEL_SetRect(AP_NAME_LABEL_CTRL_ID, &rectname, FALSE);
    GUILABEL_SetFont(AP_NAME_LABEL_CTRL_ID, DP_FONT_16, 0xffff);
    GUILABEL_SetIsAutoScroll(AP_NAME_LABEL_CTRL_ID, TRUE);
    // play button
    GUIBUTTON_SetRect(AP_PLAY_BUTTON_CTRL_ID, &rectplay);
    // pause button
    GUIBUTTON_SetRect(AP_PAUSE_BUTTON_CTRL_ID, &rectpause);
    // prev button
    GUIBUTTON_SetRect(AP_PREV_BUTTON_CTRL_ID, &rectprev);
    // next button
    GUIBUTTON_SetRect(AP_NEXT_BUTTON_CTRL_ID, &rectnext);
    // fav button
    GUIBUTTON_SetRect(AP_FAV_BUTTON_CTRL_ID, &rectfav);
    // speaker button
    GUIBUTTON_SetRect(AP_SPEAKER_BUTTON_CTRL_ID, &rectspeaker);
    //download button
    GUIBUTTON_SetRect(AP_DL_BUTTON_CTRL_ID, &rectdl);
    // progress bar
    init_data.is_forbid_paint = FALSE;
    init_data.theme_type = MMITHEME_PRGBOX_TYPE_WIDEBAR_PROGRESS;
    init_data.lcd_id = GUI_MAIN_LCD_ID;

    create.ctrl_id = ctrl_id;
    create.guid = SPRD_GUI_PRGBOX_ID;
    create.parent_win_handle = win_id;
    create.parent_ctrl_handle = NULL;
    create.init_data_ptr = &init_data;
    p_ctrl = MMK_CreateControl(&create);
    if (PNULL != p_ctrl)
    {
        progress_handle = GUICTRL_GetCtrlHandle((IGUICTRL_T *)p_ctrl);;
        MMK_SetActiveCtrl(progress_handle, TRUE);
        GUIAPICTRL_SetState (progress_handle, GUICTRL_STATE_TOPMOST_1, TRUE);
    }
    GUIPRGBOX_SetParamByCtrl(progress_handle, AUDIOPLAYER_PRGBAR_TOTAL_COUNT, 1);
    CTRLPROGRESSBAR_SetIsTpEnable(progress_handle, TRUE);
    CTRLPROGRESSBAR_SetType(progress_handle, GUIPRGBOX_STYLE_HORIZONTAL_PROCESS);
    CTRLPROGRESSBAR_SetBgColor(progress_handle, AUDIOPLAYER_PRGBAR_BG_COLOR);
    CTRLPROGRESSBAR_SetFgColor(progress_handle, AUDIOPLAYER_PRGBAR_FG_COLOR);
    CTRLPROGRESSBAR_SetRect(progress_handle, &rectprogrebar);
    //list indicator
    GUIBUTTON_SetRect(AP_LISTIND_BUTTON_CTRL_ID, &rectlistind);
    //indicator
    GUIBUTTON_SetRect(AP_IND_BUTTON_CTRL_ID, &rectind);
    //button callback
    CTRLBUTTON_SetCallBackFunc(AP_PAUSE_BUTTON_CTRL_ID, AudioPlayer_Flow_Pause);
    CTRLBUTTON_SetCallBackFunc(AP_PLAY_BUTTON_CTRL_ID, AudioPlayer_Flow_Resume);
    CTRLBUTTON_SetCallBackFunc(AP_PREV_BUTTON_CTRL_ID, AudioPlayer_Flow_Prev);
    CTRLBUTTON_SetCallBackFunc(AP_NEXT_BUTTON_CTRL_ID, AudioPlayer_Flow_Next);
    //volume callback
    //CTRLBUTTON_SetCallBackFunc(AP_TEST_SPEAKER_BUTTON_CTRL_ID, AudioPlayer_Volume_callback);
}

/*****************************************************************************/
//  Description: Audioplayer test play win full paint
//  Parameter: [In] win_id    //窗口id
//             [Out] None
//             [Return] None
//  Author: zirui.li
//  Note:
/*****************************************************************************/
LOCAL void Audioplayer_View_PlayWin_FullPaint(MMI_WIN_ID_T win_id)
{
    MMI_STRING_T                 strartist  = {0};
    MMI_STRING_T                 strtitle   = {0};
    AP_STATUS_E                  status     = AP_STATUS_MAX;
    uint32                       percent    = 0;
    //display background
    WATCHCOM_DisplayBackground(win_id);
    //set button visible
    status = Audioplayer_Data_Get_Apstatus();
    SCI_TRACE_LOW("[Audioplayer Test]audioplayer status is:%d", status);
    AudioPlayer_View_SetBtnVisibleStatus(status);
    // display artist
    strartist = Audioplayer_Data_Get_Artist();
    SCI_TRACE_LOW("[Audioplayer Test]artist is:%ls", strartist.wstr_ptr);
    GUILABEL_SetText(AP_SINGER_LABEL_CTRL_ID, &strartist, TRUE);
    //display title
    strtitle = Audioplayer_Data_Get_Title();
    SCI_TRACE_LOW("[Audioplayer Test]title is:%ls", strtitle.wstr_ptr);
    GUILABEL_SetText(AP_NAME_LABEL_CTRL_ID, &strtitle, TRUE);
    //display progress bar
    percent = Audioplayer_Data_Get_Play_Progress();
    GUIPRGBOX_SetPosByCtrl(TRUE, percent, 0, AP_PRGBAR_CTRL_ID);
}

#ifdef UAL_AUDIOPLAYER_SUPPORT
/*****************************************************************************/
//  Description: Audioplayer play win view register to audioplayer flow callback
//  Parameter: [In] win_id   //view对应的win id
//             [In] msg_id   //消息
//             [In] p_param  //消息携带的参数
//             [In] param_size  //参数大小
//             [Out] None
//             [Return] boolean
//  Author: zirui.li
//  Note:
/*****************************************************************************/
LOCAL boolean AudioPlayer_View_Callback(MMI_WIN_ID_T win_id, uint32 msg_id, void* p_param, uint32 param_size)
{
    boolean                ret      = TRUE;
    ual_audioplayer_media_info_t* p_media_info = (ual_audioplayer_media_info_t*)p_param;
    ual_audioplayer_play_progress_info_t* p_progress_info = (ual_audioplayer_play_progress_info_t*)p_param;
    SCI_TRACE_LOW("[Audioplayer Test]Enter audioplayer play win view callback, msg_id is:%d", msg_id);
    if (AP_PLAY_WIN_ID != win_id)
    {
        SCI_TRACE_LOW("[Audioplayer Test]AudioPlayer_View_Callback,win_id error!!!");
        return FALSE;
    }
    switch(msg_id)
    {
        case UAL_AUDIOPLAYER_MSG_AUDIO_DATA_IND:
        {
            break;
        }
        case UAL_AUDIOPLAYER_MSG_DOWNLOAD_PROGRESS:
        {
            break;
        }
        case UAL_AUDIOPLAYER_MSG_UPDATE_MEDIA_INFO:
        {
            if (PNULL == p_param || 0 == param_size)
            {
                SCI_TRACE_LOW("[Audioplayer Test]UAL_AUDIOPLAYER_MSG_UPDATE_MEDIA_INFO,p_param or param_size is 0!!!");
                ret = FALSE;
                break;
            }
            Audioplayer_Data_Set_Artist(p_media_info->artist);
            Audioplayer_Data_Set_Title(p_media_info->title);
            if (MMK_IsFocusWin(win_id))
            {
                MMK_SendMsg(win_id, MSG_FULL_PAINT, 0);
            }
            break;
        }
        case UAL_AUDIOPLAYER_MSG_AUDIO_SUSPEND:
        {
            Audioplayer_Data_Set_Apstatus(AP_STATUS_PAUSE);
            if (MMK_IsFocusWin(win_id))
            {
                MMK_SendMsg(win_id, MSG_FULL_PAINT, 0);
            }
            break;
        }
        case UAL_AUDIOPLAYER_MSG_AUDIO_RESUME:
        {
            Audioplayer_Data_Set_Apstatus(AP_STATUS_PLAYING);
            if (MMK_IsFocusWin(win_id))
            {
                MMK_SendMsg(win_id, MSG_FULL_PAINT, 0);
            }
            break;
        }
        case UAL_AUDIOPLAYER_MSG_AUDIO_ROUTE_CHANGED:
        {
            break;
        }
        case UAL_AUDIOPLAYER_MSG_PLAY_IND:
        {
            Audioplayer_Data_Set_Apstatus(AP_STATUS_PLAYING);
            if (MMK_IsFocusWin(win_id))
            {
                MMK_SendMsg(win_id, MSG_FULL_PAINT, 0);
            }
            break;
        }
        case UAL_AUDIOPLAYER_MSG_PLAY_END:
        {
            Audioplayer_Data_Set_Apstatus(AP_STATUS_STOP);
            if (MMK_IsFocusWin(win_id))
            {
                MMK_SendMsg(win_id, MSG_FULL_PAINT, 0);
            }
            break;
        }
        case UAL_AUDIOPLAYER_MSG_PLAY_PROGRESS:
        {
            if (PNULL == p_param || 0 == param_size)
            {
                SCI_TRACE_LOW("[Audioplayer Test]UAL_AUDIOPLAYER_MSG_PLAY_PROGRESS,p_param or param_size is 0!!!");
                ret = FALSE;
                break;
            }
            Audioplayer_Data_Set_Play_Progress(p_progress_info->cur_time, p_progress_info->total_time);
            SCI_TRACE_LOW("[Audioplayer Test]cur_time:%d, total_time:%d", p_progress_info->cur_time, p_progress_info->total_time);
            if (MMK_IsFocusWin(win_id))
            {
                MMK_SendMsg(win_id, MSG_FULL_PAINT, 0);
            }
            break;
        }
        case UAL_AUDIOPLAYER_MSG_PAUSE_IND:
        {
            Audioplayer_Data_Set_Apstatus(AP_STATUS_PAUSE);
            if (MMK_IsFocusWin(win_id))
            {
                MMK_SendMsg(win_id, MSG_FULL_PAINT, 0);
            }
            break;
        }
        case UAL_AUDIOPLAYER_MSG_BUFFERING_IND:
        {
            break;
        }
        case UAL_AUDIOPLAYER_MSG_BUFFERING_END:
        {
            break;
        }
        case UAL_AUDIOPLAYER_MSG_STOP_IND:
        {
            Audioplayer_Data_Set_Apstatus(AP_STATUS_STOP);
            if (MMK_IsFocusWin(win_id))
            {
                MMK_SendMsg(win_id, MSG_FULL_PAINT, 0);
            }
            break;
        }
        case UAL_AUDIOPLAYER_MSG_RESUME_IND:   //bug1899021
        {
            Audioplayer_Data_Set_Apstatus(AP_STATUS_PLAYING);
            if (MMK_IsFocusWin(win_id))
            {
                MMK_SendMsg(win_id, MSG_FULL_PAINT, 0);
            }
            break;
        }
        case UAL_AUDIOPLAYER_MSG_ERR:
        case UAL_AUDIOPLAYER_MSG_NET_NOT_CONNECTED:
        case UAL_AUDIOPLAYER_MSG_NET_TIMEOUT:
        case UAL_AUDIOPLAYER_MSG_NET_CONNECT_ERR:
        {
            break;
        }
        default:
            ret = FALSE;
            break;
    }
    SCI_TRACE_LOW("ret:%d",ret);
    return ret;
}
#endif

/*****************************************************************************/
//  Description: Audioplayer test play win handle msg
//  Parameter: [In] win_id   //窗口id
//             [In] msg_id   //消息id
//             [In] param    //参数
//             [Out] None
//             [Return] MMI_RESULT_E //窗口处理结果
//  Author: zirui.li
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E Audioplayer_View_PlayWinHandleMsg(MMI_WIN_ID_T win_id,
                                               MMI_MESSAGE_ID_E msg_id,
                                               DPARAM param)
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    SCI_TRACE_LOW("[Audioplayer Test]Enter Audioplayer test play win, msg_id is:%d", msg_id);
    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
            Audioplayer_View_PlayWin_OpenWindow(win_id);
#ifdef UAL_AUDIOPLAYER_SUPPORT
            AudioPlayer_Flow_Reg_Callback(win_id,AudioPlayer_View_Callback);
#endif
            break;
        case MSG_FULL_PAINT:
            Audioplayer_View_PlayWin_FullPaint(win_id);
            break;
        case MSG_APP_CANCEL:
            MMK_CloseWin(win_id);
            break;
        case MSG_CLOSE_WINDOW:
            recode = Audioplayer_Flow_Destroy_Service();
            Audioplayer_Data_Reset_Info();
            break;
        default:
            recode = MMI_RESULT_FALSE;
            break;
    }
    return recode;
}

WINDOW_TABLE(AP_PLAY_WIN_TAB) =
{
    WIN_HIDE_STATUS,
    WIN_FUNC((uint32)Audioplayer_View_PlayWinHandleMsg),
    WIN_ID(AP_PLAY_WIN_ID),
    CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, AP_SINGER_LABEL_CTRL_ID),
    CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, AP_NAME_LABEL_CTRL_ID),
    CREATE_BUTTON_CTRL(res_audioplayer_play, AP_PLAY_BUTTON_CTRL_ID),
    CREATE_BUTTON_CTRL(res_audioplayer_pause, AP_PAUSE_BUTTON_CTRL_ID),
    CREATE_BUTTON_CTRL(res_audioplayer_prev, AP_PREV_BUTTON_CTRL_ID),
    CREATE_BUTTON_CTRL(res_audioplayer_next, AP_NEXT_BUTTON_CTRL_ID),
    CREATE_BUTTON_CTRL(res_audioplayer_speaker,AP_SPEAKER_BUTTON_CTRL_ID),
    CREATE_BUTTON_CTRL(res_audioplayer_fav,AP_FAV_BUTTON_CTRL_ID),
    CREATE_BUTTON_CTRL(res_audioplayer_download,AP_DL_BUTTON_CTRL_ID),
    CREATE_BUTTON_CTRL(res_audioplayer_listind,AP_LISTIND_BUTTON_CTRL_ID),
    CREATE_BUTTON_CTRL(res_audioplayer_ind,AP_IND_BUTTON_CTRL_ID),
    END_WIN
};

/*****************************************************************************/
//  Description: Audioplayer test play win enter
//  Parameter: [In] None
//             [Out] None
//             [Return] None
//  Author: zirui.li
//  Note:
/*****************************************************************************/
PUBLIC void Audioplayer_View_PlayWin_Enter(void)
{
    MMK_CreateWin((uint32*)AP_PLAY_WIN_TAB, PNULL);
}
