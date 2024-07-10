/*****************************************************************************
** File Name:        audioplayer_demoapp_flow.c                                  *
** Author:           zirui.li                                                *
** Date:             12/13/2021                                              *
** Copyright:                                                                *
** Description: This file is used to define audioplayer flow function        *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE                 NAME                  DESCRIPTION                    *
** 12/13/2021           zirui.li              Create                         *
******************************************************************************/
#include "mmimp3_id.h"
#include "mmk_app.h"
#include "mmk_msg.h"
#include "audioplayer_demoapp_flow.h"

#define AUDIOPLAYER_AUDIOSOURCE_LEN         200
#define AUDIOPLAYER_AUDIO_VOLUME            5

#ifdef UAL_AUDIOPLAYER_SUPPORT
LOCAL ual_audioplayer_handle       s_ap_handle = 0;   //audioplayer handle
LOCAL ual_audioplayer_play_param_t s_ap_play_list[] =
{
    //{L"http://music.163.com/song/media/outer/url?id=25638810.mp3",AUDIOPLAYER_AUDIOSOURCE_LEN,0},
    {L"http://music.163.com/song/media/outer/url?id=26305541.mp3",AUDIOPLAYER_AUDIOSOURCE_LEN,0},
    {L"http://music.163.com/song/media/outer/url?id=4872533.mp3",AUDIOPLAYER_AUDIOSOURCE_LEN,0},
    {L"http://music.163.com/song/media/outer/url?id=32835565.mp3",AUDIOPLAYER_AUDIOSOURCE_LEN,0},
    {L"http://music.163.com/song/media/outer/url?id=298317.mp3",AUDIOPLAYER_AUDIOSOURCE_LEN,0},
};
LOCAL ual_audioplayer_play_param_t* s_p_play_list = s_ap_play_list;
#endif
LOCAL uint32 s_i = 0;   //index of audio

typedef struct
{
    MMI_WIN_ID_T win_id;
    handle_events_callback p_callback;
}AUDIOPLAYER_FLOW_CB_T;
LOCAL   AUDIOPLAYER_FLOW_CB_T   s_flow_cb = {0};

#ifdef UAL_AUDIOPLAYER_SUPPORT
/*****************************************************************************/
//  Description: Audioplayer test callback
//  Parameter: [In] param    //ual audioplayer发送给app的消息
//             [Out] None
//             [Return] BOOLEAN
//  Author: zirui.li
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN Audioplayer_Flow_callback(ual_cms_msg_t param)
{
    uint32                 msg_id   = 0;
    boolean                ret      = TRUE;
    ual_audioplayer_err_e  err_code = UAL_AUDIOPLAYER_ERR_NONE;
    msg_id = param.msg_id;
    SCI_TRACE_LOW("[Audioplayer Test]Enter audioplayer test callback, msg_id is:%d", msg_id);
    switch(msg_id)
    {
        case UAL_AUDIOPLAYER_MSG_AUDIO_DATA_IND:
        {
            SCI_TRACE_LOW("[Audioplayer Test]UAL_AUDIOPLAYER_MSG_AUDIO_DATA_IND");
            SCI_TRACE_LOW("[Audioplayer Test]param.p_body:%0x, param.body_size:%d",param.p_body, param.body_size);
            if (PNULL != s_flow_cb.p_callback)
            {
                SCI_TRACE_LOW("[Audioplayer Test]event to callback");
                s_flow_cb.p_callback(s_flow_cb.win_id,UAL_AUDIOPLAYER_MSG_AUDIO_DATA_IND,param.p_body,param.body_size);
            }
            else
            {
                SCI_TRACE_LOW("[Audioplayer Test]handle event callback is PNULL!!!");
                ret = FALSE;
            }
            break;
        }
        case UAL_AUDIOPLAYER_MSG_DOWNLOAD_PROGRESS:
        {
            ual_audioplayer_download_progress_info_t progress_info = {0};
            err_code = ual_audioplayer_get_download_progress_info(s_ap_handle, &progress_info);
            SCI_TRACE_LOW("[Audioplayer Test]UAL_AUDIOPLAYER_MSG_DOWNLOAD_PROGRESS");
            SCI_TRACE_LOW("[Audioplayer Test]cur_recv_data_length:%d, total_data_length:%d, download_time:%d",progress_info.cur_recv_data_length, \
                progress_info.total_data_length, progress_info.download_time);
            if (UAL_AUDIOPLAYER_ERR_NONE != err_code)
            {
                SCI_TRACE_LOW("[Audioplayer Test]get download progress fail!");
                ret = FALSE;
            }
            if (PNULL != s_flow_cb.p_callback)
            {
                SCI_TRACE_LOW("[Audioplayer Test]event to callback");
                s_flow_cb.p_callback(s_flow_cb.win_id,UAL_AUDIOPLAYER_MSG_DOWNLOAD_PROGRESS,&progress_info,sizeof(ual_audioplayer_download_progress_info_t));
            }
            else
            {
                SCI_TRACE_LOW("[Audioplayer Test]handle event callback is PNULL!!!");
                ret = FALSE;
            }
            break;
        }
        case UAL_AUDIOPLAYER_MSG_UPDATE_MEDIA_INFO:
        {
            ual_audioplayer_media_info_t media_info = {0};
            SCI_TRACE_LOW("[Audioplayer Test]UAL_AUDIOPLAYER_MSG_UPDATE_MEDIA_INFO");
            err_code = ual_audioplayer_get_media_info(s_ap_handle,&media_info);
            if (UAL_AUDIOPLAYER_ERR_NONE != err_code)
            {
                SCI_TRACE_LOW("[Audioplayer Test]get media info fail!");
                ret = FALSE;
            }
            if (PNULL != s_flow_cb.p_callback)
            {
                SCI_TRACE_LOW("[Audioplayer Test]event to callback");
                s_flow_cb.p_callback(s_flow_cb.win_id,UAL_AUDIOPLAYER_MSG_UPDATE_MEDIA_INFO,&media_info,sizeof(ual_audioplayer_media_info_t));
            }
            else
            {
                SCI_TRACE_LOW("[Audioplayer Test]handle event callback is PNULL!!!");
                ret = FALSE;
            }
            break;
        }
        case UAL_AUDIOPLAYER_MSG_AUDIO_SUSPEND:
        {
            SCI_TRACE_LOW("[Audioplayer Test]UAL_AUDIOPLAYER_MSG_AUDIO_SUSPEND");
            if (PNULL != s_flow_cb.p_callback)
            {
                SCI_TRACE_LOW("[Audioplayer Test]event to callback");
                s_flow_cb.p_callback(s_flow_cb.win_id,UAL_AUDIOPLAYER_MSG_AUDIO_SUSPEND,PNULL,0);
            }
            else
            {
                SCI_TRACE_LOW("[Audioplayer Test]handle event callback is PNULL!!!");
                ret = FALSE;
            }
            break;
        }
        case UAL_AUDIOPLAYER_MSG_AUDIO_RESUME:
        {
            SCI_TRACE_LOW("[Audioplayer Test]UAL_AUDIOPLAYER_MSG_AUDIO_RESUME");
            if (PNULL != s_flow_cb.p_callback)
            {
                SCI_TRACE_LOW("[Audioplayer Test]event to callback");
                s_flow_cb.p_callback(s_flow_cb.win_id,UAL_AUDIOPLAYER_MSG_AUDIO_RESUME,PNULL,0);
            }
            else
            {
                SCI_TRACE_LOW("[Audioplayer Test]handle event callback is PNULL!!!");
                ret = FALSE;
            }
            break;
        }
        case UAL_AUDIOPLAYER_MSG_AUDIO_ROUTE_CHANGED:
        {
            SCI_TRACE_LOW("[Audioplayer Test]UAL_AUDIOPLAYER_MSG_AUDIO_ROUTE_CHANGED");
            if (PNULL != s_flow_cb.p_callback)
            {
                SCI_TRACE_LOW("[Audioplayer Test]event to callback");
                s_flow_cb.p_callback(s_flow_cb.win_id,UAL_AUDIOPLAYER_MSG_AUDIO_ROUTE_CHANGED,PNULL,0);
            }
            else
            {
                SCI_TRACE_LOW("[Audioplayer Test]handle event callback is PNULL!!!");
                ret = FALSE;
            }
            break;
        }
        case UAL_AUDIOPLAYER_MSG_PLAY_IND:
        {
            SCI_TRACE_LOW("[Audioplayer Test]UAL_AUDIOPLAYER_MSG_PLAY_IND");
            if (PNULL != s_flow_cb.p_callback)
            {
                SCI_TRACE_LOW("[Audioplayer Test]event to callback");
                s_flow_cb.p_callback(s_flow_cb.win_id,UAL_AUDIOPLAYER_MSG_PLAY_IND,PNULL,0);
            }
            else
            {
                SCI_TRACE_LOW("[Audioplayer Test]handle event callback is PNULL!!!");
                ret = FALSE;
            }
            break;
        }
        case UAL_AUDIOPLAYER_MSG_PLAY_END:
        {
            ual_audioplayer_play_param_t   play_param = {0};
            MMI_RESULT_E                   recode     = MMI_RESULT_TRUE;
            SCI_TRACE_LOW("[Audioplayer Test]UAL_AUDIOPLAYER_MSG_PLAY_END");
            if (PNULL != s_flow_cb.p_callback)
            {
                SCI_TRACE_LOW("[Audioplayer Test]event to callback");
                s_flow_cb.p_callback(s_flow_cb.win_id,UAL_AUDIOPLAYER_MSG_PLAY_END,PNULL,0);
            }
            else
            {
                SCI_TRACE_LOW("[Audioplayer Test]handle event callback is PNULL!!!");
                break;
            }
            recode = AudioPlayer_Flow_Next();
            if (MMI_RESULT_TRUE != recode)
            {
                SCI_TRACE_LOW("[Audioplayer Test]change to next audio fail!");
                ret = FALSE;
            }
            break;
        }
        case UAL_AUDIOPLAYER_MSG_PLAY_PROGRESS:
        {
            ual_audioplayer_play_progress_info_t progress_info = {0};
            SCI_TRACE_LOW("[Audioplayer Test]UAL_AUDIOPLAYER_MSG_PLAY_PROGRESS");
            err_code = ual_audioplayer_get_play_progress_info(s_ap_handle,&progress_info);
            if (UAL_AUDIOPLAYER_ERR_NONE != err_code)
            {
                SCI_TRACE_LOW("[Audioplayer Test]get play progress info fail!");
                ret = FALSE;
            }
            SCI_TRACE_LOW("[Audioplayer Test]cur_time:%d, total_time:%d", progress_info.cur_time, progress_info.total_time);
            if (PNULL != s_flow_cb.p_callback)
            {
                SCI_TRACE_LOW("[Audioplayer Test]event to callback");
                s_flow_cb.p_callback(s_flow_cb.win_id,UAL_AUDIOPLAYER_MSG_PLAY_PROGRESS,&progress_info,sizeof(ual_audioplayer_play_progress_info_t));
            }
            else
            {
                SCI_TRACE_LOW("[Audioplayer Test]handle event callback is PNULL!!!");
                ret = FALSE;
            }
            break;
        }
        case UAL_AUDIOPLAYER_MSG_PAUSE_IND:
        {
            SCI_TRACE_LOW("[Audioplayer Test]UAL_AUDIOPLAYER_MSG_PAUSE_IND");
            if (PNULL != s_flow_cb.p_callback)
            {
                SCI_TRACE_LOW("[Audioplayer Test]event to callback");
                s_flow_cb.p_callback(s_flow_cb.win_id,UAL_AUDIOPLAYER_MSG_PAUSE_IND,PNULL,0);
            }
            else
            {
                SCI_TRACE_LOW("[Audioplayer Test]handle event callback is PNULL!!!");
                ret = FALSE;
            }
            break;
        }
        case UAL_AUDIOPLAYER_MSG_BUFFERING_IND:
        {
            SCI_TRACE_LOW("[Audioplayer Test]UAL_AUDIOPLAYER_MSG_BUFFERING_IND");
            if (PNULL != s_flow_cb.p_callback)
            {
                SCI_TRACE_LOW("[Audioplayer Test]event to callback");
                s_flow_cb.p_callback(s_flow_cb.win_id,UAL_AUDIOPLAYER_MSG_BUFFERING_IND,PNULL,0);
            }
            else
            {
                SCI_TRACE_LOW("[Audioplayer Test]handle event callback is PNULL!!!");
                ret = FALSE;
            }
            break;
        }
        case UAL_AUDIOPLAYER_MSG_BUFFERING_END:
        {
            SCI_TRACE_LOW("[Audioplayer Test]UAL_AUDIOPLAYER_MSG_BUFFERING_END");
            if (PNULL != s_flow_cb.p_callback)
            {
                SCI_TRACE_LOW("[Audioplayer Test]event to callback");
                s_flow_cb.p_callback(s_flow_cb.win_id,UAL_AUDIOPLAYER_MSG_BUFFERING_END,PNULL,0);
            }
            else
            {
                SCI_TRACE_LOW("[Audioplayer Test]handle event callback is PNULL!!!");
                ret = FALSE;
            }
            break;
        }
        case UAL_AUDIOPLAYER_MSG_STOP_IND:
        {
            SCI_TRACE_LOW("[Audioplayer Test]UAL_AUDIOPLAYER_MSG_STOP_IND");
            if (PNULL != s_flow_cb.p_callback)
            {
                SCI_TRACE_LOW("[Audioplayer Test]event to callback");
                s_flow_cb.p_callback(s_flow_cb.win_id,UAL_AUDIOPLAYER_MSG_STOP_IND,PNULL,0);
            }
            else
            {
                SCI_TRACE_LOW("[Audioplayer Test]handle event callback is PNULL!!!");
                ret = FALSE;
            }
            break;
        }
        case UAL_AUDIOPLAYER_MSG_RESUME_IND:   //bug1899021
        {
            SCI_TRACE_LOW("[Audioplayer Test]UAL_AUDIOPLAYER_MSG_RESUME_IND");
            if (PNULL != s_flow_cb.p_callback)
            {
                SCI_TRACE_LOW("[Audioplayer Test]event to callback");
                s_flow_cb.p_callback(s_flow_cb.win_id,UAL_AUDIOPLAYER_MSG_RESUME_IND,PNULL,0);
            }
            else
            {
                SCI_TRACE_LOW("[Audioplayer Test]handle event callback is PNULL!!!");
                ret = FALSE;
            }
            break;
        }
        case UAL_AUDIOPLAYER_MSG_ERR:
        case UAL_AUDIOPLAYER_MSG_NET_NOT_CONNECTED:
        case UAL_AUDIOPLAYER_MSG_NET_TIMEOUT:
        case UAL_AUDIOPLAYER_MSG_NET_CONNECT_ERR:
        {
            SCI_TRACE_LOW("[Audioplayer Test]ERROR!");
            if (PNULL != s_flow_cb.p_callback)
            {
                SCI_TRACE_LOW("[Audioplayer Test]event to callback");
                s_flow_cb.p_callback(s_flow_cb.win_id,msg_id,PNULL,0);
            }
            else
            {
                SCI_TRACE_LOW("[Audioplayer Test]handle event callback is PNULL!!!");
                ret = FALSE;
            }
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
//  Description: Audioplayer test app enter
//  Parameter: [In] None
//             [Out] None
//             [Return] None
//  Author: zirui.li
//  Note:
/*****************************************************************************/
PUBLIC void AudioPlayer_Enter(void)
{
#ifdef UAL_AUDIOPLAYER_SUPPORT
    ual_audioplayer_play_param_t  play_param = {0};
#endif
    SCI_TRACE_LOW("[Audioplayer Test]Enter network music");
    // 1.enter win
    Audioplayer_View_PlayWin_Enter();
#ifdef UAL_AUDIOPLAYER_SUPPORT
    // 2.create audioplayer service
    ual_audioplayer_create(Audioplayer_Flow_callback, &s_ap_handle);
    // 3.set volume and play audio
    play_param.p_audio_source = s_p_play_list[s_i].p_audio_source;
    play_param.audio_source_len = s_p_play_list[s_i].audio_source_len;
    SCI_TRACE_LOW("[Audioplayer Test]audio_source is:%ls", play_param.p_audio_source);
    ual_audioplayer_set_volume(s_ap_handle, AUDIOPLAYER_AUDIO_VOLUME);
    ual_audioplayer_play(s_ap_handle, &play_param);
#endif
}

/*****************************************************************************/
//  Description: Audioplayer demoapp flow register callback
//  Parameter: [In] win_id  //view模块对应的win id
//             [In] p_callback
//             [Out] None
//             [Return] BOOLEAN
//  Author: zirui.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN AudioPlayer_Flow_Reg_Callback(MMI_WIN_ID_T win_id, handle_events_callback p_callback)
{
    BOOLEAN   ret = TRUE;
    SCI_TRACE_LOW("[Audioplayer Test]regsiter callback,win_id:%d",win_id);
    if (PNULL == p_callback)
    {
        SCI_TRACE_LOW("[Audioplayer Test]p_callback is PNULL!!!");
        return FALSE;
    }
    s_flow_cb.p_callback = p_callback;
    s_flow_cb.win_id = win_id;
    return ret;
}

/*****************************************************************************/
//  Description: Audioplayer test pause button callback
//  Parameter: [In] None
//             [Out] None
//             [Return] MMI_RESULT_E
//  Author: zirui.li
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E AudioPlayer_Flow_Pause(void)
{
    MMI_RESULT_E            recode   = MMI_RESULT_TRUE;
#ifdef UAL_AUDIOPLAYER_SUPPORT
    ual_audioplayer_err_e   err_code = UAL_AUDIOPLAYER_ERR_NONE;
    SCI_TRACE_LOW("[Audioplayer Test]Enter pause button callback");
    err_code = ual_audioplayer_pause(s_ap_handle);
    if (UAL_AUDIOPLAYER_ERR_NONE != err_code)
    {
        SCI_TRACE_LOW("[Audioplayer Test]ual_audioplayer_pause fail!");
        recode = MMI_RESULT_FALSE;
    }
#endif
    return recode;
}

/*****************************************************************************/
//  Description: Audioplayer test resume button callback
//  Parameter: [In] None
//             [Out] None
//             [Return] MMI_RESULT_E
//  Author: zirui.li
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E AudioPlayer_Flow_Resume(void)
{
    MMI_RESULT_E            recode   = MMI_RESULT_TRUE;
#ifdef UAL_AUDIOPLAYER_SUPPORT
    ual_audioplayer_err_e   err_code = UAL_AUDIOPLAYER_ERR_NONE;
    SCI_TRACE_LOW("[Audioplayer Test]Enter resume button callback");
    err_code = ual_audioplayer_resume(s_ap_handle);
    if (UAL_AUDIOPLAYER_ERR_NONE != err_code)
    {
        SCI_TRACE_LOW("[Audioplayer Test]ual_audioplayer_resume fail!");
        recode = MMI_RESULT_FALSE;
    }
#endif
    return recode;
}

/*****************************************************************************/
//  Description: Audioplayer test prev button callback
//  Parameter: [In] None
//             [Out] None
//             [Return] MMI_RESULT_E
//  Author: zirui.li
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E AudioPlayer_Flow_Prev(void)
{
    MMI_RESULT_E                   recode     = MMI_RESULT_TRUE;
    uint32                         total_num  = 0;
#ifdef UAL_AUDIOPLAYER_SUPPORT
    ual_audioplayer_play_param_t   play_param = {0};
    ual_audioplayer_err_e          err_code   = UAL_AUDIOPLAYER_ERR_NONE;
    SCI_TRACE_LOW("[Audioplayer Test]Enter prev button callback");
    total_num = sizeof(s_ap_play_list)/sizeof(ual_audioplayer_play_param_t);
    SCI_TRACE_LOW("[Audioplayer Test]total num of audio list is:%d", total_num);
    if (0 == s_i)
    {
        s_i = total_num-1;
    }
    else
    {
        s_i = s_i - 1;
    }
    play_param.p_audio_source = s_p_play_list[s_i].p_audio_source;
    play_param.audio_source_len = s_p_play_list[s_i].audio_source_len;
    SCI_TRACE_LOW("[Audioplayer Test]play_param.p_audio_source:%ls", play_param.p_audio_source);
    err_code = ual_audioplayer_play(s_ap_handle, &play_param);
    if (UAL_AUDIOPLAYER_ERR_NONE != err_code)
    {
        SCI_TRACE_LOW("[Audioplayer Test]play prev audio fail!");
        recode = MMI_RESULT_FALSE;
    }
#endif
    return recode;
}

/*****************************************************************************/
//  Description: Audioplayer test next button callback
//  Parameter: [In] None
//             [Out] None
//             [Return] MMI_RESULT_E
//  Author: zirui.li
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E AudioPlayer_Flow_Next(void)
{
    MMI_RESULT_E                   recode     = MMI_RESULT_TRUE;
    uint32                         total_num  = 0;
    uint32                         last_index = 0;
#ifdef UAL_AUDIOPLAYER_SUPPORT
    ual_audioplayer_play_param_t   play_param = {0};
    ual_audioplayer_err_e          err_code   = UAL_AUDIOPLAYER_ERR_NONE;
    SCI_TRACE_LOW("[Audioplayer Test]Enter next button callback");
    total_num = sizeof(s_ap_play_list)/sizeof(ual_audioplayer_play_param_t);
    SCI_TRACE_LOW("[Audioplayer Test]total num of audio list is:%d", total_num);
    last_index = total_num - 1;
    if (last_index == s_i)
    {
        s_i = 0;
    }
    else
    {
        s_i = s_i+1;
    }
    play_param.p_audio_source = s_p_play_list[s_i].p_audio_source;
    play_param.audio_source_len = s_p_play_list[s_i].audio_source_len;
    SCI_TRACE_LOW("[Audioplayer Test]play_param.p_audio_source:%ls", play_param.p_audio_source);
    err_code = ual_audioplayer_play(s_ap_handle,&play_param);
    if (UAL_AUDIOPLAYER_ERR_NONE != err_code)
    {
        SCI_TRACE_LOW("[Audioplayer Test]play next audio fail!");
        recode = MMI_RESULT_FALSE;
    }
#endif
    return recode;
}

/*****************************************************************************/
//  Description: Audioplayer test destroy ual audioplayer service
//  Parameter: [In] None
//             [Out] None
//             [Return] MMI_RESULT_E
//  Author: zirui.li
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E Audioplayer_Flow_Destroy_Service(void)
{
    MMI_RESULT_E            recode   = MMI_RESULT_TRUE;
#ifdef UAL_AUDIOPLAYER_SUPPORT
    ual_audioplayer_err_e   err_code = UAL_AUDIOPLAYER_ERR_NONE;
    SCI_TRACE_LOW("[Audioplayer Test]Enter Destroy audioplayer service");
    err_code = ual_audioplayer_destroy(s_ap_handle);
    if (UAL_AUDIOPLAYER_ERR_NONE != err_code)
    {
        recode = MMI_RESULT_FALSE;
    }
    s_ap_handle = 0;
#endif
    SCI_TraceLow("[Audioplayer Test]recode:%d",recode);
    return recode;
}
