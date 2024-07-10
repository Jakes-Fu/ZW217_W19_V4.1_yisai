/*****************************************************************************
** File Name:               mmiap_play.c                                     *
** Author:                                                                   *
** Date:           2012/7/19                                                 *
** Copyright:      2012 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe ap control                  *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME              DESCRIPTION                              *
** 2012/7/19      aoke.hu           Create                                   *
******************************************************************************/

#define _MMIAP_PLAY_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_app_audioplayer_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif

#include "mmipub.h"
#include "mmi_appmsg.h"
#include "mmk_timer.h"
#include "guifont.h"


//other module
#include "mmifmm_interface.h"
#include "mmifmm_export.h"
#include "mmiidle_export.h"
#include "mmiidle_subwintab.h"
#include "mmisd_export.h"

#include "mmicc_export.h"
#include "mmi_default.h"
#include "mmisrvaud_api.h"
#include "mmisrvmgr.h"
#include "mp3_adp.h"
#include "mmisrvap_export.h"
#include "mmiidle_cstyle.h"


#ifdef PDA_UI_DROPDOWN_WIN
#include "mmidropdownwin_export.h"
#endif

//local module
#include "mmimp3_export.h"
#include "mmimp3_id.h"
#include "mmimp3_text.h"
#include "mmiap_common.h"
#include "mmiap_applet.h"
#include "mmiapwin_main.h"
#include "mmimp3_nv.h"


#include "mmiapwin_set.h"
#include "mmiap_setting.h"
#include "mmiap_appmain.h"

#ifdef MMIAP_MUSICMARK_SUPPORT
#include "mmiap_musicmark.h"
#endif

#ifdef MMIAP_SPECTRUM_SUPPORT
#include "mmiap_spectrum.h"
#endif

#ifdef MMIAP_LYRIC_SUPPORT
#include "mmiap_lyric_internal.h"
#endif

#ifdef MMIAP_F_HISTORY_PLAYLIST
#include "mmiap_lastinfo_ctrl.h"
#endif

#ifdef BLUETOOTH_SUPPORT
#include "mmimp3_earphone.h"
#endif

#include "mmiapwin_list.h"
#include "mmiap_list.h"
#include "mmiap_list_manager.h"
#include "mmiap_list_control.h"

#include "mmiap_play.h"

/*lint -e655*/
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define MMIMP3_PROGRESS_SCROLL_NUM      (300)
#define MMIMP3_PROGRESS_SCROLL_MIN_TIME 100 //滚动的timer最小间隔
#define MMIMP3_PROGRESS_SCROLL_MAX_TIME 500 //滚动的timer最大间隔

#define MMIMP3_FORWARD_BACKWARD_MIN_TIME    5           //mp3快进快退最小时间单位 5s
#define MMIMP3_FORWARD_BACKWARD_MAX_NUM     20          //mp3快进快退,将整首曲子平均分为20分
#define MMIMP3_FORWARD_BACKWARD_MIN_NUM     2           //mp3快进快退,将整首曲子平均分为2分

/**--------------------------------------------------------------------------*
 **                         LOCAL DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

LOCAL MMIAP_PLAY_CTRL_INFO_T *s_mmiap_play_ctrl_info_ptr = PNULL;
LOCAL BOOLEAN s_review_process_sucessful = TRUE;
/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : Get NV settings, and initialize the virables.
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:   //Mp3PrepareToPlay
/*****************************************************************************/
LOCAL void InitMp3ModuleVariables(void);

/*****************************************************************************/
//  Description : get mp3 current offset,play position
//  Global resource dependence :
//  Author:jassmine.meng
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetMp3CurrentOffset(
    BOOLEAN       is_forward,     //forward or backward
    uint32        *mp3_offset_ptr //in/out:从Mp3 pause置算起
);

/*****************************************************************************/
//  Description : get mp3 current total offset,from 0 start
//  Global resource dependence :
//  Author:jassmine.meng
//  Note:
/*****************************************************************************/
LOCAL uint32 GetMp3CurrentTotalOffset(
    BOOLEAN           is_forward,
    uint32            forward_backward_offset//从Mp3 pause置算起
);

/*****************************************************************************/
//  Description : Stop the audio.
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL void StopMp3(void);

/*****************************************************************************/
//  Description : whether the current playing file are all not supported
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsCurPlayListAllNotSupported(void);
/*==========================================================
 * author    :
 * function  :
 * decr      :
 * parameter :
 * return    :
 * effect    : NO
 * history   :created  9/3/2009
 ==========================================================*/

LOCAL BOOLEAN GetReviewMp3CurrentOffset(
    BOOLEAN       is_forward,     //forward or backward
    uint32        *mp3_offset_ptr //in/out:从Mp3 pause置算起
);


/*****************************************************************************/
//  Description : mp3 alert call back
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL void Mp3AlertCallback(
    void
);

/*****************************************************************************/
//  Description : mmi ap callback to ap srv
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL void SRVAP_CALLBACK(
    MMISRVAP_CALLBACK_PARAM_T *callback_param_ptr,
    uint32 size_of_callback_param
);

/*****************************************************************************/
//  Description : start timer for play mp3
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
LOCAL uint32 GetAPPeriodTime(
    uint32    mp3_time
);

/*****************************************************************************/
//  Description : Stop play state
//  Global resource dependence :
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL void StopPlayState(void);

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : get ap play ctrl info ptr
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL MMIAP_PLAY_CTRL_INFO_T *GetApPlayCtrlInfoPtr(void)
{
    SCI_TRACE_LOW("[MMIMP3] GetApPlayCtrlInfoPtr s_mmiap_play_ctrl_info_ptr = 0x%x", s_mmiap_play_ctrl_info_ptr);
    return s_mmiap_play_ctrl_info_ptr;
}

/*****************************************************************************/
//  Description : get ap play info ptr
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL MMIMP3_PLAY_INFO_T *GetApPlayInfoPtr(void)
{
    MMIAP_PLAY_CTRL_INFO_T *info_ptr = GetApPlayCtrlInfoPtr();
    MMIMP3_PLAY_INFO_T *play_ptr = PNULL;


    if (PNULL != info_ptr)
    {
        play_ptr = &info_ptr->play_info;
    }

    return play_ptr;
}

/*****************************************************************************/
//  Description : get ap review info ptr
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL MMIMP3_REVIEW_INFO_T *GetApReviewInfoPtr(void)
{
    MMIAP_PLAY_CTRL_INFO_T *info_ptr = GetApPlayCtrlInfoPtr();
    MMIMP3_REVIEW_INFO_T *review_ptr = PNULL;


    if (PNULL != info_ptr)
    {
        review_ptr = &info_ptr->review_info;
    }

    return review_ptr;
}

/*****************************************************************************/
//  Description : create ap play ctrl info
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CreatePlayCtrlInfo(void)
{
    BOOLEAN ret = FALSE;
    MMIAP_PLAY_CTRL_INFO_T *info_ptr = GetApPlayCtrlInfoPtr();
    uint32 info_size = sizeof(*info_ptr);

    SCI_TRACE_LOW("[MMIMP3] CreatePlayCtrlInfo info_ptr = 0x%x", info_ptr);

    if (PNULL == info_ptr)
    {
        info_ptr = SCI_ALLOCA(info_size);
    }

    if (PNULL != info_ptr)
    {
        s_mmiap_play_ctrl_info_ptr = info_ptr;

        SCI_MEMSET(info_ptr, 0, info_size);

        ret = TRUE;
    }

    SCI_TRACE_LOW("[MMIMP3] CreatePlayCtrlInfo ret = %d", ret);
    return ret;
}

/*****************************************************************************/
//  Description : destroy ap play ctrl info
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAP_DestroyAPPlayCtrlInfo(
    void
)
{

    SCI_TRACE_LOW("[MMIMP3] MMIAP_DestroyAPPlayCtrlInfo");

    if (PNULL != s_mmiap_play_ctrl_info_ptr)
    {
        SCI_FREE(s_mmiap_play_ctrl_info_ptr);
    }
}

/*****************************************************************************/
//  Description : get review info
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAP_GetReviewInfo(
    MMIMP3_REVIEW_INFO_T *review_ptr //out
)
{
    MMIMP3_REVIEW_INFO_T *info_ptr = GetApReviewInfoPtr();


    if (PNULL == review_ptr
            || PNULL == info_ptr
       )
    {
        return;
    }


    *review_ptr = *info_ptr;
}

/*****************************************************************************/
//  Description : set review info
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAP_SetReviewInfo(
    MMIMP3_REVIEW_INFO_T *review_ptr //in
)
{
    MMIMP3_REVIEW_INFO_T *info_ptr = GetApReviewInfoPtr();


    if (PNULL == review_ptr
            || PNULL == info_ptr
       )
    {
        return;
    }


    *info_ptr = *review_ptr;
}

/*****************************************************************************/
//  Description : get no support number
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL uint16 GetNoSupportNumber(
    void
)
{
    MMIAP_PLAY_CTRL_INFO_T *info_ptr = GetApPlayCtrlInfoPtr();
    uint16 num = 0;

    if (PNULL == info_ptr
       )
    {
        return num;
    }


    num = info_ptr->no_support_num;

    SCI_TRACE_LOW("GetNoSupportNumber num=%d",\
                  num);

    return num;
}

/*****************************************************************************/
//  Description : set no support number
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void SetNoSupportNumber(
    uint16 num
)
{
    MMIAP_PLAY_CTRL_INFO_T *info_ptr = GetApPlayCtrlInfoPtr();


    if (PNULL == info_ptr
       )
    {
        return;
    }


    info_ptr->no_support_num = num;

    SCI_TRACE_LOW("SetNoSupportNumber num=%d",\
                  num);
}

/*****************************************************************************/
//  Description : get ap srv handle
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC MMISRVAP_HANDLE GetAPSrvHandle(void)
{
    MMIAP_PLAY_CTRL_INFO_T *info_ptr = GetApPlayCtrlInfoPtr();
    MMISRVAP_HANDLE handle = PNULL;

    if (PNULL == info_ptr
       )
    {
        return handle;
    }

    handle = info_ptr->srvap_handle;

    SCI_TRACE_LOW("[MMIMP3] GetAPSrvHandle srvap_handle=0x%X",\
                  handle);

    return handle;
}

/*****************************************************************************/
//  Description : set ap srv handle
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL void SetAPSrvHandle(
    MMISRVAP_HANDLE handle //in
)
{
    MMIAP_PLAY_CTRL_INFO_T *info_ptr = GetApPlayCtrlInfoPtr();


    if (PNULL == info_ptr
       )
    {
        return ;
    }

    info_ptr->srvap_handle = handle;

    SCI_TRACE_LOW("[MMIMP3] SetAPSrvHandle srvap_handle=0x%X",\
                  handle);
}


/*****************************************************************************/
//  Description : get dyn playing info
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL void GetDynPlayingInfo(
    MMIMP3_PLAYING_DYN_INFO_T *dyn_ptr //out
)
{
    MMIMP3_PLAY_INFO_T *info_ptr = GetApPlayInfoPtr();


    if (PNULL == dyn_ptr
            || PNULL == info_ptr
       )
    {
        return;
    }


    *dyn_ptr = info_ptr->dyn_playing_info;
}

/*****************************************************************************/
//  Description : set dyn playing info
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL void SetDynPlayingInfo(
    MMIMP3_PLAYING_DYN_INFO_T *dyn_ptr //in
)
{
    MMIMP3_PLAY_INFO_T *info_ptr = GetApPlayInfoPtr();


    if (PNULL == dyn_ptr
            || PNULL == info_ptr
       )
    {
        return;
    }


    info_ptr->dyn_playing_info = *dyn_ptr;
}

/*****************************************************************************/
//  Description : get switch delay timer id
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL uint8 GetSwitchDelayTimerID(
    void
)
{
    MMIMP3_PLAY_INFO_T *info_ptr = GetApPlayInfoPtr();
    uint8 timer_id = 0;


    if (PNULL == info_ptr
       )
    {
        return timer_id;
    }


    timer_id = info_ptr->switch_delay_timer;

    SCI_TRACE_LOW("GetSwitchDelayTimerID timer_id=%d",\
                  timer_id);

    return timer_id;
}

/*****************************************************************************/
//  Description : set switch delay timer id
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void SetSwitchDelayTimerID(
    uint8 timer_id
)
{
    MMIMP3_PLAY_INFO_T *info_ptr = GetApPlayInfoPtr();


    if (PNULL == info_ptr
       )
    {
        return;
    }

    info_ptr->switch_delay_timer = timer_id;

    SCI_TRACE_LOW("SetSwitchDelayTimerID timer_id=%d",\
                  timer_id);
}

/*****************************************************************************/
//  Description : get switch delay offset
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL uint32 GetSwitchDelayOffset(
    void
)
{
    MMIMP3_PLAY_INFO_T *info_ptr = GetApPlayInfoPtr();
    uint32 offset = 0;


    if (PNULL == info_ptr
       )
    {
        return offset;
    }


    offset = info_ptr->switch_delay_offset;

    SCI_TRACE_LOW("GetSwitchDelayOffset offset=%d",\
                  offset);

    return offset;
}

/*****************************************************************************/
//  Description : set switch delay offset
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void SetSwitchDelayOffset(
    uint32 offset
)
{
    MMIMP3_PLAY_INFO_T *info_ptr = GetApPlayInfoPtr();


    if (PNULL == info_ptr
       )
    {
        return;
    }

    info_ptr->switch_delay_offset = offset;

    SCI_TRACE_LOW("SetSwitchDelayOffset offset=%d",\
                  offset);
}

/*****************************************************************************/
//  Description : get ap audio srv handle
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC MMISRV_HANDLE_T MMIAP_GetAPAudioSrvHandle(
    void
)
{
    MMIAP_PLAY_CTRL_INFO_T *info_ptr = GetApPlayCtrlInfoPtr();
    MMISRV_HANDLE_T handle = PNULL;

    if (PNULL == info_ptr
       )
    {
        return handle;
    }

    handle = info_ptr->audio_srv_handle;

    SCI_TRACE_LOW("GetAPAudioSrvHandle handle=0x%X",\
                  handle);

    return handle;
}

/*****************************************************************************/
//  Description : set ap audio srv handle
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL void SetAPAudioSrvHandle(
    MMISRV_HANDLE_T handle //in
)
{
    MMIAP_PLAY_CTRL_INFO_T *info_ptr = GetApPlayCtrlInfoPtr();


    if (PNULL == info_ptr
       )
    {
        return ;
    }

    info_ptr->audio_srv_handle = handle;

    SCI_TRACE_LOW("SetAPAudioSrvHandle handle=0x%X",\
                  handle);
}

/*****************************************************************************/
//  Description : seek to
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAP_SeekTo(
    MMISRVAP_HANDLE af_handle, //in
    uint32 offset
)
{
    MMISRVAP_REQ_SEEK_PARAM_T seek_param = {0};
    MMISRVAP_ERROR_E error_code = 0;
    BOOLEAN ret = FALSE;

    seek_param.offset = offset;

    error_code = MMISRVAP_SeekTo(af_handle,
                                 &seek_param,
                                 sizeof(seek_param)
                                );
    if (MMISRVAP_ERROR_NONE == error_code)
    {
        ret = TRUE;
    }

    SCI_TRACE_LOW("[MMIAP] MMIAP_SeekTo ret=%d",\
                  ret);

    return ret;
}

/*****************************************************************************/
//  Description : Get NV settings, and initialize the virables.
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL void InitMp3ModuleVariables(void)
{
    MMIMP3_PLAY_INFO_T *info_ptr = GetApPlayInfoPtr();


    if (PNULL != info_ptr)
    {
        SCI_MEMSET(info_ptr, 0, sizeof(MMIMP3_PLAY_INFO_T));
    }


    SetNoSupportNumber(0);

    MMIMP3_SetAudioState(MMIMP3_AUDIO_STOPPED);
    MMIAPIMP3_SetPlayerState(MMIMP3_PLAYER_STATE_NONE);

    MMIMP3_SetFirstInit(TRUE);
    MMIMP3_SetPausedByService(FALSE);
}

/*****************************************************************************/
//  Description : get mp3bg handle
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC MMISRV_HANDLE_T MMIAPIMP3_GetBGPlayerHandle(void)
{
    return MMIAP_GetAPAudioSrvHandle();
}

/*****************************************************************************/
//  Description : Request mp3 play handle.
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMP3_ReqPlayHandle(void)
{
    BOOLEAN result = FALSE;
    MMISRVAP_OPEN_PARAM_T open_param = {0};
    MMISRVAP_HANDLE handle = PNULL;

    SCI_TRACE_LOW("[MMIMP3] MMIMP3_ReqPlayHandle");
    
#ifdef BT_AV_SNK_SUPPORT
    if(MMIAPIBT_IsA2dpSinkConnected())
    {
        SCI_TRACE_LOW("BT_AV, MMIMP3_ReqPlayHandle, in bt music mode, return");
        return TRUE;
    }
#endif

    open_param.vol = MMIAPISET_GetMultimVolume();

#ifdef BLUETOOTH_SUPPORT
    if(MMIMP3_BT_MODE_ON == MMIMP3_GetBtSettingMode() || (MMIMP3_BT_MODE_NONE == MMIMP3_GetBtSettingMode() ))
    {
        open_param.is_support_bt = TRUE;
    }
#endif
    open_param.callback_func = SRVAP_CALLBACK;


    handle = MMISRVAP_Open(&open_param);

    if (PNULL != handle)
    {
        result = TRUE;
    }

    SetAPSrvHandle(handle);

    return result;
}

/*****************************************************************************/
//  Description : Release mp3 play handle.
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIMP3_ReleasePlayHandle(void)
{
    SCI_TRACE_LOW("[MMIMP3] MMIMP3_ReleasePlayHandle");
    MMISRVAP_Close(GetAPSrvHandle());

    SetAPSrvHandle(PNULL);
    SetAPAudioSrvHandle(PNULL);
}

/*****************************************************************************/
//  Description : get current file content
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMP3_GetCurFileContent(void)
{
    BOOLEAN ret = FALSE;
    MMISRVAUD_CONTENT_INFO_T mp3_file_info = {0};

    MMIMP3_LIST_FILE_BODY_T cur_play_file_info = {0};
    MMIMP3_PLAYING_DYN_INFO_T dyn_info = {0};


    cur_play_file_info.full_path_len = FMM_SEARCH_FILENAME_MAXLEN;
    ret = MMIMP3_GetCurPlayFileFullPath(cur_play_file_info.full_path_name ,&(cur_play_file_info.full_path_len));
    if (ret)
    {
        if(MMIAPIFMM_IsFileExist(cur_play_file_info.full_path_name,
                                 cur_play_file_info.full_path_len
                                )
          )
        {
            if(MMISRVAUD_GetFileContentInfo(
                        cur_play_file_info.full_path_name,  /*lint !e605*/
                        cur_play_file_info.full_path_len,
                        &mp3_file_info
                    )
              )
            {
                GetDynPlayingInfo(&dyn_info);

                dyn_info.total_length   = mp3_file_info.total_data_length;
                dyn_info.total_time     = mp3_file_info.total_time;
                dyn_info.bit_rate       = mp3_file_info.bitrate;

                SetDynPlayingInfo(&dyn_info);

                ret = TRUE;
            }
        }
    }

    return ret;
}

/*****************************************************************************/
//  Description : Request mp3 play sevice.
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMP3_ReqPlaySrv(const wchar * file_name_ptr,
                                 uint16 file_name_len,
                                 MMISRVAUD_RING_FMT_E audio_type,
                                 uint32 offset
                                )
{
    BOOLEAN result = FALSE;

    MMISRVAP_REQ_PLAY_PARAM_T play_param = {0};
    MMISRVAP_ERROR_E error_code =0;
    uint32 period_time = 0;

    MMIMP3_REVIEW_INFO_T review_info = {0};
    MMIMP3_PLAYING_DYN_INFO_T dyn_info = {0};

    SCI_TRACE_LOW("[MMIMP3] MMIMP3_ReqPlaySrv");

#ifdef BT_AV_SNK_SUPPORT
    if(MMIAPIBT_IsA2dpSinkConnected())
    {
        SCI_TRACE_LOW("BT_AV, MMIMP3_ReqPlaySrv, in bt music mode, return");
        return TRUE;
    }
#endif

    if (PNULL == file_name_ptr
            || 0 == file_name_len
       )
    {
        return result;
    }

    GetDynPlayingInfo(&dyn_info);
    period_time = GetAPPeriodTime(dyn_info.total_time);


    play_param.audio_type = audio_type;
    play_param.vol = MMIAPISET_GetMultimVolume();

#ifdef BLUETOOTH_SUPPORT
    if(MMIMP3_BT_MODE_ON == MMIMP3_GetBtSettingMode() || (MMIMP3_BT_MODE_NONE == MMIMP3_GetBtSettingMode() ))
    {
        play_param.is_support_bt = TRUE;
    }
#endif

    play_param.period_time = period_time;
    play_param.offset_point = offset;

    play_param.full_path_ptr = (wchar*)file_name_ptr;
    play_param.full_path_len = file_name_len;


    MMIAP_GetReviewInfo(&review_info);
    if (0 != review_info.start_point)
    {
        play_param.review_param.start_point = review_info.start_point;
        play_param.review_param.end_point = review_info.end_point;
    }


    error_code = MMISRVAP_Play(GetAPSrvHandle(),
                               &play_param,
                               sizeof(play_param));

    if (MMISRVAP_ERROR_NONE == error_code)
    {
        result = TRUE;
    }

    return result;
}

/*****************************************************************************/
//  Description : Play mp3 in a delayed manner, start delay timer
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIMP3_DelayPlayMp3(
    uint32    mp3_offset
)
{
    uint32 time_out = 0;
    uint8 timer_id = 0;



    timer_id = GetSwitchDelayTimerID();
    if(timer_id != 0)
    {
        MMK_StopTimer(timer_id);
        SetSwitchDelayTimerID(0);
    }

    SetSwitchDelayOffset(mp3_offset);

    /*
    if(MMIMP3_PLAYER_SWITCH_PAUSED == MMIAPIMP3_GetPlayerState())
    {
        //SCI_TRACE_LOW:"[MMIMP3] MMIMP3_DelayPlayMp3 encounter MMIMP3_PLAYER_SWITCH_PAUSED"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMP3_1534_112_2_18_2_3_51_10,(uint8*)"");
    }
    else*/
    {
        MMIAPIMP3_SetPlayerState(MMIMP3_PLAYER_MUSIC_SWITCH);

        {
            MMIAP_PLAY_WIN_CALLBACK_PARAM_T win_param = {0};


            win_param.flag = MMIAPWIN_PLAY_FLAG_ALL_PANEL;
            MMIAP_DoWinCallback(MMIAP_WIN_TYPE_MAIN,
                                MMIAPWIN_FLAG_FUNC_PLAY,
                                &win_param,
                                sizeof(win_param)
                               );
        }

#ifdef BLUETOOTH_SUPPORT

        if(MMIAPIBT_GetActiveBTHeadset())
        {
            time_out = MMI_1SECONDS;
        }
        else
#endif
        {
            time_out = 500;
        }

        timer_id = MMK_CreateTimerCallback(time_out, MMIAPIMP3_HandleDelayPlayTimer, NULL, FALSE);
        SetSwitchDelayTimerID(timer_id);
    }

    //SCI_TRACE_LOW:"[MMIMP3] MMIMP3_DelayPlayMp3 Start Timer = %d, offset = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMP3_1553_112_2_18_2_3_51_11, (uint8*)"dd",\
                 timer_id, mp3_offset);
}

/*****************************************************************************/
//  Description : Get the delay timer, for playing mp3 in a delayed manner.
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMIAPIMP3_GetDelayPlayTimer(
    void
)
{
    return GetSwitchDelayTimerID();
}

/*****************************************************************************/
//  Description : Stop the delay timer, for playing mp3 in a delayed manner.
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIMP3_StopDelayPlayTimer(
    void
)
{
    uint8 timer_id = 0;


    //SCI_TRACE_LOW:"[MMIMP3] MMIMP3_StopDelayPlayTimer Stop Timer"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMP3_1575_112_2_18_2_3_51_12, (uint8*)"");

    timer_id = GetSwitchDelayTimerID();
    if(timer_id != 0)
    {
        MMK_StopTimer(timer_id);
        SetSwitchDelayTimerID(0);
    }
}

/*****************************************************************************/
//  Description : Play mp3 in a delayed manner, handle delay timer,begin to play.
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIMP3_HandleDelayPlayTimer(
    uint8 timer_id,
    uint32 param
)
{
    uint32 offset = 0;
    BOOLEAN result = FALSE;


    MMIMP3_StopDelayPlayTimer();

    if(MMIMP3_PLAYER_SWITCH_PAUSED == MMIAPIMP3_GetPlayerState())
    {
        //SCI_TRACE_LOW:"[MMIMP3] MMIAPIMP3_HandleDelayPlayTimer encounter MMIMP3_PLAYER_SWITCH_PAUSED"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMP3_1602_112_2_18_2_3_51_13, (uint8*)"");
    }
    else if(MMIMP3_PLAYER_MUSIC_SWITCH == MMIAPIMP3_GetPlayerState())
    {
        offset = GetSwitchDelayOffset();
        result = MMIAPIMP3_PlayAudioPlayer(offset);
        SetSwitchDelayOffset(MMIMP3_ZERO_OFFSET);
    }

    //SCI_TRACE_LOW:"[MMIMP3] MMIAPIMP3_HandleDelayPlayTimer result = %d, p_state = %d, a_state = %d!!!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMP3_1610_112_2_18_2_3_51_14, (uint8*)"ddd",\
                 result, MMIAPIMP3_GetPlayerState(), MMIMP3_GetAudioState());
}

/*****************************************************************************/
//  Description : Start play audio player
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMP3_PlayAudioPlayer(
    uint32    mp3_offset
)
{
    BOOLEAN result = FALSE;


#ifdef BLUETOOTH_SUPPORT
    MMIMP3_CheckBtSetting();
#endif
    result = MMIMP3_PlayMp3(mp3_offset);

    return result;
}

/*****************************************************************************/
//  Description : Stop audio player
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIMP3_StopAudioPlayer(
    void
)
{
    MMIMP3_StopMp3();
}

/*****************************************************************************/
//  Description : mp3 alert call back
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL void Mp3AlertCallback(
    void
)
{
    //SCI_TRACE_LOW:"[MMIMP3] Mp3AlertCallback enter"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMP3_1696_112_2_18_2_3_51_17, (uint8*)"");
    MMIMP3_CallBackAfterPlay(MMISRVAP_ERROR_NONE, NULL);
}

/*****************************************************************************/
//  Description : process file not exist for play
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL void ProcessFileNotExistForPlay(
    void
)
{
    SCI_TRACE_LOW("[MMIAP] ProcessFileNotExistForPlay");


    MMIAPIMP3_SetPlayerState(MMIMP3_PLAYER_STATE_NONE);



    {
        MMIAP_PLAY_WIN_CALLBACK_PARAM_T win_param = {0};


        win_param.flag = MMIAPWIN_PLAY_FLAG_ALERT_MSG;
        win_param.alert_msg = MMIMP3_MUSIC_FILE_NOT_EXIST;
        MMIAP_DoWinCallback(MMIAP_WIN_TYPE_MAIN,
                            MMIAPWIN_FLAG_FUNC_PLAY,
                            &win_param,
                            sizeof(win_param)
                           );
    }

    MAIN_SetIdleBgPlayName(TRUE, MMIBGPLAY_TYPE_MP3);
#ifndef SUBLCD_SIZE_NONE
    MMISUB_SetSubLcdDisplay(FALSE, TRUE, SUB_CONTENT_MP3, PNULL);
#endif
}

/*****************************************************************************/
//  Description : process req play fail
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL void ProcessReqPlayFail(
    void
)
{
    SCI_TRACE_LOW("[MMIAP] ProcessReqPlayFail");

    MMIAPIMP3_SetPlayerState(MMIMP3_PLAYER_MUSIC_SWITCH);
    MMIMP3_SetAudioState(MMIMP3_AUDIO_STOPPED);
    MMIMP3_NoSupportNumInc();

    //not display mp3 name
    MAIN_SetIdleBgPlayName(TRUE, MMIBGPLAY_TYPE_MP3);
#ifndef SUBLCD_SIZE_NONE
    MMISUB_SetSubLcdDisplay(FALSE, TRUE, SUB_CONTENT_MP3, PNULL);
#endif

    {
        MMIAP_WIN_CALLBACK_RET_E cb_ret = 0;
        MMIAP_PLAY_WIN_CALLBACK_PARAM_T win_param = {0};


        win_param.flag = MMIAPWIN_PLAY_FLAG_ALERT_MSG;
        win_param.alert_msg = MMIMP3_FORMAT_NOT_SUPPORT;
        cb_ret = MMIAP_DoWinCallback(MMIAP_WIN_TYPE_MAIN,
                                     MMIAPWIN_FLAG_FUNC_PLAY,
                                     &win_param,
                                     sizeof(win_param)
                                    );

        if(MMIAP_WIN_CALLBACK_RET_DONE_BY_CB != cb_ret)
        {
            Mp3AlertCallback();
        }
    }
}
/*****************************************************************************/
//  Description : process req play success
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL void ProcessReqPlaySuccess(
    void
)
{
    MMISRVAUD_EQ_MODE_E      eq_mode = MMIAPSETWIN_GetCurrentEqMode();
    MMIMP3_PLAYING_DYN_INFO_T dyn_info = {0};
    MMISRVAUD_CONTENT_INFO_T mp3_content_info = {0};


    MMIAPIMP3_SetPlayerState(MMIMP3_PLAYER_AUDIO_ACTIVE);
    MMIMP3_SetAudioState(MMIMP3_AUDIO_PLAYING);

    MMIAPIMP3_EnableEq(eq_mode);
#ifdef MMIAP_LYRIC_SUPPORT
    MMIMP3_OpenLyric();
#endif

    MMIMP3_NoSupportNumClear();

	GetDynPlayingInfo(&dyn_info);
	
    MMISRVAUD_GetContentInfo(MMIAP_GetAPAudioSrvHandle(), &mp3_content_info);
    dyn_info.total_length   = mp3_content_info.total_data_length;
    dyn_info.total_time     = mp3_content_info.total_time;
    dyn_info.bit_rate       = mp3_content_info.bitrate;
    SetDynPlayingInfo(&dyn_info);


    MMIMP3_GetPlayingDynInfo(PNULL, TRUE);

    //SCI_TRACE_LOW:"[MMIMP3]:MMIMP3_PlayMp3: uiTotalTime = %d, total_length = %d, bit_rate = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMP3_2070_112_2_18_2_3_52_22, (uint8*)"ddd", mp3_content_info.total_time, mp3_content_info.total_data_length, mp3_content_info.bitrate);

    {
        MMIAP_PLAY_WIN_CALLBACK_PARAM_T win_param = {0};


        win_param.flag = MMIAPWIN_PLAY_FLAG_START_SPECTRUM;
        MMIAP_DoWinCallback(MMIAP_WIN_TYPE_MAIN,
                            MMIAPWIN_FLAG_FUNC_PLAY,
                            &win_param,
                            sizeof(win_param)
                           );
    }


    MAIN_SetIdleBgPlayName(TRUE, MMIBGPLAY_TYPE_MP3);
#ifdef PDA_UI_DROPDOWN_WIN
    MMIDROPDOWNWIN_UpdateMainWin();
#endif
#ifndef SUBLCD_SIZE_NONE
    MMISUB_SetSubLcdDisplay(FALSE, TRUE, SUB_CONTENT_MP3, PNULL);
#endif
}

/*****************************************************************************/
//  Description : Start play mp3
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMP3_PlayMp3(
    uint32    mp3_offset
)
{
    BOOLEAN                 result = FALSE;
    MMISRVAUD_RING_FMT_E         audio_type  = MMISRVAUD_RING_FMT_MAX;
    uint32                  active_index = MMIMP3_GetCurPlayFileIndex();
    BOOLEAN                 is_file_exist = FALSE;
    BOOLEAN                 is_file_support_a2dp = FALSE;
    BOOLEAN                 is_active_a2dp = FALSE;
    uint32                  cur_file_size = 0;
#ifdef DRM_SUPPORT
    BOOLEAN                 is_drm_file = FALSE;
    BOOLEAN                 is_audio_drm_file = FALSE;
    BOOLEAN                 is_drm_valid = FALSE;
    MMISRVAUD_RING_FMT_E         drm_audio_type = MMISRVAUD_RING_FMT_MAX;
    DRMFILE_PRE_CHECK_STATUS_E drm_status = DRMFILE_PRE_CHECK_NORMAL;
#endif
    wchar             full_path_name[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
    uint16            full_path_len = MMIFILE_FULL_PATH_MAX_LEN;

    uint32 total_num = MMIMP3_GetCurPlayListTotalNum();


    //SCI_TRACE_LOW:"[MMIMP3] MMIMP3_PlayMp3 active_index = %d, total_num = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMP3_1940_112_2_18_2_3_51_19, (uint8*)"dd",\
                 active_index, total_num);

#ifdef BT_AV_SNK_SUPPORT
    if(MMIAPIBT_IsA2dpSinkConnected())
    {
        SCI_TRACE_LOW("BT_AV, MMIMP3_PlayMp3, in bt music mode, return");
        return TRUE;
    }
#endif

    MMIMP3_StopDelayPlayTimer();
    MMIAPIMP3_SetPlayerState(MMIMP3_PLAYER_MUSIC_SWITCH);


    do
    {

        result = MMIMP3_GetCurPlayFileFullPath(full_path_name,&full_path_len);
        if (!result)
        {
            ProcessFileNotExistForPlay();
            break;
        }

        if(active_index >= total_num)
        {
            SCI_TRACE_LOW("[MMIAP] MMIMP3_PlayMp3 error active_index=%d",\
                          active_index);

            MMIAPIMP3_SetPlayerState(MMIMP3_PLAYER_STATE_NONE);
            MMIAPIMP3_StopApplet(FALSE);

            result = FALSE;
            break;
        }

        MMIAPIFMM_GetFileInfo(full_path_name, full_path_len, &cur_file_size, PNULL, PNULL);
        is_file_exist = MMIAPIFMM_IsFileExist(full_path_name, full_path_len);
        is_file_support_a2dp = MMIAPIMP3_IsFileSupportA2DP(full_path_name, full_path_len);
#ifdef BLUETOOTH_SUPPORT
        is_active_a2dp = MMIAPIMP3_IsThroughBt();
#endif
#ifdef DRM_SUPPORT

        if(MMIAPIMP3_IsDrmFile(
                    full_path_name, full_path_len,
                    &drm_audio_type, &is_audio_drm_file, &is_drm_valid
                ))
        {
            is_drm_file = TRUE;
            is_audio_drm_file = MMIAP_IsPlayerLoadType(drm_audio_type);
            //SCI_TRACE_LOW:"[MMIMP3] MMIMP3_PlayMp3 is drm file"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMP3_1961_112_2_18_2_3_51_20, (uint8*)"");


            {
                MMIAP_WIN_CALLBACK_RET_E cb_ret = 0;
                MMIAP_PLAY_WIN_CALLBACK_PARAM_T win_param = {0};



                win_param.flag = MMIAPWIN_PLAY_FLAG_PRECHECK_FILE;
                win_param.full_path_name_ptr = full_path_name;
                win_param.drm_status_ptr = &drm_status;
                cb_ret = MMIAP_DoWinCallback(MMIAP_WIN_TYPE_MAIN,
                                             MMIAPWIN_FLAG_FUNC_PLAY,
                                             &win_param,
                                             sizeof(win_param)
                                            );

                if (MMIAP_WIN_CALLBACK_RET_BREAK == cb_ret)
                {
                    result = FALSE;
                    break;
                }
            }

        }

#endif
        //SCI_TRACE_LOW:"[MMIMP3] MMIMP3_PlayMp3 :is_file_exist = %d,file_size = %d, is_a2dp = %d, file_support_a2sp = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMP3_1973_112_2_18_2_3_51_21, (uint8*)"dddd", is_file_exist, cur_file_size, is_active_a2dp, is_file_support_a2dp);

        if(!is_file_exist                               ||
                0 == cur_file_size                           ||
                (!is_file_support_a2dp && is_active_a2dp)
#ifdef DRM_SUPPORT
                || (!is_drm_valid && is_audio_drm_file)
                || (!is_audio_drm_file && is_drm_file)
#endif
          )
        {
            BOOLEAN is_msg = FALSE;
            MMIMP3_ALERT_MSG_E alert_msg = 0;
            MMIAP_WIN_CALLBACK_RET_E cb_ret = 0;


            MMIMP3_NoSupportNumInc();


            if(!is_file_exist)
            {
                is_msg = TRUE;
                alert_msg = MMIMP3_MUSIC_FILE_NOT_EXIST;
            }
            else if(0 == cur_file_size)
            {
                is_msg = TRUE;
                alert_msg = MMIMP3_MUSIC_FILE_SIZE_ZERO;
            }
            else if(!is_file_support_a2dp && is_active_a2dp)
            {
                is_msg = TRUE;
                alert_msg = MMIMP3_A2DP_NOT_SUPPORT;
            }

#ifdef DRM_SUPPORT
            else if(DRMFILE_PRE_CHECK_NO_SYS_NITZ == drm_status)
            {
                is_msg = TRUE;
                alert_msg = MMIMP3_DRM_NOT_SYS_NITZ;
            }
            else if(DRMFILE_PRE_CHECK_NO_RIGHTS == drm_status)
            {
                is_msg = TRUE;
                alert_msg = MMIMP3_DRM_RIGHT_INVALID;
            }
            else if(!is_drm_valid && is_audio_drm_file)
            {
                is_msg = TRUE;
                alert_msg = MMIMP3_DRM_RIGHT_INVALID;
            }
            else if(!is_audio_drm_file && is_drm_file)
            {
                is_msg = TRUE;
                alert_msg = MMIMP3_DRM_FORMAT_NOT_AUDIO;
            }
#endif

            if (is_msg)
            {
                MMIAP_PLAY_WIN_CALLBACK_PARAM_T win_param = {0};


                win_param.flag = MMIAPWIN_PLAY_FLAG_ALERT_MSG;
                win_param.alert_msg = alert_msg;
                cb_ret = MMIAP_DoWinCallback(MMIAP_WIN_TYPE_MAIN,
                                             MMIAPWIN_FLAG_FUNC_PLAY,
                                             &win_param,
                                             sizeof(win_param)
                                            );
            }

            if(MMIAP_WIN_CALLBACK_RET_DONE_BY_CB != cb_ret)
            {
                Mp3AlertCallback();
            }


            MAIN_SetIdleBgPlayName(TRUE, MMIBGPLAY_TYPE_MP3);
#ifndef SUBLCD_SIZE_NONE
            MMISUB_SetSubLcdDisplay(FALSE, TRUE, SUB_CONTENT_MP3, PNULL);
#endif
        }
        else
        {
            //get audio type
            audio_type = MMIAPICOM_GetMusicType(
                             full_path_name,
                             full_path_len
                         );

#ifdef DRM_SUPPORT

            if(is_audio_drm_file)
            {
                audio_type = drm_audio_type;
            }

#endif

            result = MMIMP3_ReqPlaySrv(full_path_name,
                                       full_path_len,
                                       audio_type,
                                       mp3_offset
                                      );

            if (!result)
            {
                ProcessReqPlayFail();
                break;
            }

            ProcessReqPlaySuccess();
        }
    }
    while(0);

//SCI_TRACE_LOW:"[MMIMP3]:MMIMP3_PlayMp3, result = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMP3_2144_112_2_18_2_3_52_23, (uint8*)"d", result);
    return (result);
}

/*****************************************************************************/
//  Description : Pause the audio BG player
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMP3_PauseMp3(
    BOOLEAN is_notify_by_audio_service
)
{
    BOOLEAN                 result      = TRUE;
    BOOLEAN is_update = FALSE;


    SCI_TRACE_LOW("[MMIMP3] MMIAPIMP3_PauseMp3 player_state = %d, audio_state = %d !!!", \
                  MMIAPIMP3_GetPlayerState(), MMIMP3_GetAudioState());

    switch(MMIAPIMP3_GetPlayerState())
    {
    case MMIMP3_PLAYER_STATE_NONE:
        break;

    case MMIMP3_PLAYER_AUDIO_ACTIVE:
    {
        switch(MMIMP3_GetAudioState())
        {
        case MMIMP3_AUDIO_PLAYING:
            if (is_notify_by_audio_service)
            {
                is_update = TRUE;
            }
            else
            {
                if (MMISRVAP_ERROR_NONE == MMISRVAP_Pause(GetAPSrvHandle()))
                {
                    is_update = TRUE;
                }
            }

            if(is_update)
            {
#ifdef MMIAP_LYRIC_SUPPORT
                MMIMP3_PauseLyric();
#endif
                MMIMP3_SetAudioState(MMIMP3_AUDIO_PAUSED);
#ifdef MMIAP_SPECTRUM_SUPPORT
                MMIMP3_StopSpectrum(FALSE);
#endif
                MMIMP3_StopMainAnim(TRUE);
                MMIMP3_StopTitleAnim(TRUE);
                MAIN_SetIdleBgPlayName(TRUE, MMIBGPLAY_TYPE_MP3);
#ifndef SUBLCD_SIZE_NONE
                MMISUB_SetSubLcdDisplay(FALSE, TRUE, SUB_CONTENT_MP3, PNULL);
#endif
                result = TRUE;
            }
            else
            {
                result = FALSE;
                //SCI_TRACE_MID:"[MMIMP3]:MMIAPIMP3_PauseMp3:AUDIO FAILED!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMP3_2187_112_2_18_2_3_52_25, (uint8*)"");
                break;
            }

            break;

        default:
            break;
        }
    }
    break;

    case MMIMP3_PLAYER_MUSIC_SWITCH:
        MMIMP3_StopDelayPlayTimer();
        MMIAPIMP3_SetPlayerState(MMIMP3_PLAYER_SWITCH_PAUSED);
        MAIN_SetIdleBgPlayName(TRUE, MMIBGPLAY_TYPE_MP3);
#ifndef SUBLCD_SIZE_NONE
        MMISUB_SetSubLcdDisplay(FALSE, TRUE, SUB_CONTENT_MP3, PNULL);
#endif
        result = TRUE;
        break;

    case MMIMP3_PLAYER_SWITCH_PAUSED:
        MMIMP3_StopDelayPlayTimer();
        result = TRUE;
        break;

    default:
        break;
    }

    //SCI_TRACE_MID:"[MMIMP3]:MMIAPIMP3_PauseMp3: result = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMP3_2214_112_2_18_2_3_52_26, (uint8*)"d", result);


    MMIMP3_SetPausedByService(is_notify_by_audio_service);


    if(result)
    {
        MMIAPISET_ClearCurRingType(MMISET_RING_TYPE_AUDIO_PLAYER);
    }


    {
        MMIAP_PLAY_WIN_CALLBACK_PARAM_T win_param = {0};


        win_param.flag = MMIAPWIN_PLAY_FLAG_BUTTON_PROGRESS;
        MMIAP_DoWinCallback(MMIAP_WIN_TYPE_MAIN,
                            MMIAPWIN_FLAG_FUNC_PLAY,
                            &win_param,
                            sizeof(win_param)
                           );
    }


    //update mp3 widget in idle win
    {
        MMIAP_PLAY_WIN_CALLBACK_PARAM_T win_param = {0};


        win_param.flag = MMIAPWIN_PLAY_FLAG_UPDATE_MP3;
        MMIAP_DoWinCallback(MMIAP_WIN_TYPE_IDLE,
                            MMIAPWIN_FLAG_FUNC_PLAY,
                            &win_param,
                            sizeof(win_param)
                           );
    }


    return result;
}


/*****************************************************************************/
//  Description : Resume audio BG player.
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMP3_ResumeMp3(
    BOOLEAN is_notify_by_audio_service
)
{
    BOOLEAN          result = FALSE;
    BOOLEAN is_update = FALSE;


    //SCI_TRACE_LOW:"[MMIMP3] MMIAPIMP3_ResumeMp3 player_state = %d, audio_state = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMP3_2241_112_2_18_2_3_52_27, (uint8*)"dd", MMIAPIMP3_GetPlayerState(), MMIMP3_GetAudioState());

    switch(MMIAPIMP3_GetPlayerState())
    {
    case MMIMP3_PLAYER_STATE_NONE:
        break;

    case MMIMP3_PLAYER_AUDIO_ACTIVE:
    {
        switch(MMIMP3_GetAudioState())
        {
        case MMIMP3_AUDIO_PAUSED:
            if (is_notify_by_audio_service)
            {
                is_update = TRUE;
            }
            else
            {
                if (MMISRVAP_ERROR_NONE == MMISRVAP_Resume(GetAPSrvHandle()))
                {
                    is_update = TRUE;
                }
            }

            if(is_update)
            {
                MMIMP3_SetAudioState(MMIMP3_AUDIO_PLAYING);
#ifdef MMIAP_LYRIC_SUPPORT
                MMIMP3_ResumeLyric();
#endif

                MMIAPIMP3_HandleProgressTimer(0, NULL);
#ifdef MMIAP_SPECTRUM_SUPPORT
                MMIMP3_StartSpectrum();
#endif

                MMIMP3_PlayTitleAnim(TRUE);

                MAIN_SetIdleBgPlayName(TRUE, MMIBGPLAY_TYPE_MP3);
#ifndef SUBLCD_SIZE_NONE
                MMISUB_SetSubLcdDisplay(FALSE, TRUE, SUB_CONTENT_MP3, PNULL);
#endif
                result = TRUE;
            }
            else
            {
                MMIAP_WIN_CALLBACK_RET_E cb_ret = 0;


                //SCI_TRACE_MID:"[MMIMP3]:MMIAPIMP3_ResumeMp3:AUDIO FAILED!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMP3_2275_112_2_18_2_3_52_28, (uint8*)"");


                {
                    MMIAP_PLAY_WIN_CALLBACK_PARAM_T win_param = {0};
                    BOOLEAN is_need_alert = TRUE;

                    win_param.flag = MMIAPWIN_PLAY_FLAG_ALERT_MSG;
                    win_param.alert_msg = MMIMP3_FORMAT_NOT_SUPPORT;
                    win_param.param_ptr = &is_need_alert;
                    cb_ret = MMIAP_DoWinCallback(MMIAP_WIN_TYPE_MAIN,
                                                 MMIAPWIN_FLAG_FUNC_PLAY,
                                                 &win_param,
                                                 sizeof(win_param)
                                                );
                }


                MMIAPIMP3_SetPlayerState(MMIMP3_PLAYER_STATE_NONE);
                MAIN_SetIdleBgPlayName(TRUE, MMIBGPLAY_TYPE_MP3);
#ifndef SUBLCD_SIZE_NONE
                MMISUB_SetSubLcdDisplay(FALSE, TRUE, SUB_CONTENT_MP3, PNULL);
#endif
                break;
            }

            break;

        default:
            break;
        }
    }
    break;

    case MMIMP3_PLAYER_MUSIC_SWITCH:
    case MMIMP3_PLAYER_SWITCH_PAUSED:
    {
        uint32 start_point = 0;
        MMIMP3_REVIEW_INFO_T review_info = {0};


        MMIAP_GetReviewInfo(&review_info);
        if(MMIMP3_REVIEW_STATE_NONE == MMIMP3_GetReviewStatus())
        {
            start_point = MMIMP3_ZERO_OFFSET;
        }
        else
        {
            start_point = review_info.start_point;
        }
        MMIMP3_DelayPlayMp3(start_point);
    }

    result = TRUE;
    break;

    default:
        break;
    }


    {
        MMIAP_PLAY_WIN_CALLBACK_PARAM_T win_param = {0};


        win_param.flag = MMIAPWIN_PLAY_FLAG_BUTTON_PROGRESS;
        MMIAP_DoWinCallback(MMIAP_WIN_TYPE_MAIN,
                            MMIAPWIN_FLAG_FUNC_PLAY,
                            &win_param,
                            sizeof(win_param)
                           );
    }


    //update mp3 widget in idle win
    {
        MMIAP_PLAY_WIN_CALLBACK_PARAM_T win_param = {0};


        win_param.flag = MMIAPWIN_PLAY_FLAG_UPDATE_MP3;
        MMIAP_DoWinCallback(MMIAP_WIN_TYPE_IDLE,
                            MMIAPWIN_FLAG_FUNC_PLAY,
                            &win_param,
                            sizeof(win_param)
                           );
    }


    return result;
}

/*****************************************************************************/
//  Description : Stop the audio.
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL void StopMp3(void)
{

    StopPlayState();

    MMIMP3_ResetPlayingDynInfo();
    MMIMP3_ResetReviewInfo();


    {
        MMIAP_PLAY_WIN_CALLBACK_PARAM_T win_param = {0};


        win_param.flag = MMIAPWIN_PLAY_FLAG_UPDATE_BUTTON_STATE;
        MMIAP_DoWinCallback(MMIAP_WIN_TYPE_MAIN,
                            MMIAPWIN_FLAG_FUNC_PLAY,
                            &win_param,
                            sizeof(win_param)
                           );
    }
}


/*****************************************************************************/
//  Description : Stop the audio BG player.
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIMP3_StopMp3(
    void
)
{
    //SCI_TRACE_LOW:"[MMIMP3] MMIMP3_StopMp3 player_state = %d, audio_state = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMP3_2368_112_2_18_2_3_52_29, (uint8*)"dd", MMIAPIMP3_GetPlayerState(), MMIMP3_GetAudioState());

    MMIMP3_ClearMp3AlertInfo(FALSE);
#ifdef MMI_AUDIOPLAYER_REVIEW_SUPPORT
    MMIMP3_ClearReviewMenu();
#endif

    switch(MMIAPIMP3_GetPlayerState())
    {
    case MMIMP3_PLAYER_STATE_NONE:
        break;

    case MMIMP3_PLAYER_AUDIO_ACTIVE:
    case MMIMP3_PLAYER_MUSIC_SWITCH:
    case MMIMP3_PLAYER_SWITCH_PAUSED:
    {
        StopMp3();

        MAIN_SetIdleBgPlayName(FALSE, MMIBGPLAY_TYPE_MP3);
#ifndef SUBLCD_SIZE_NONE
        MMISUB_SetSubLcdDisplay(FALSE, FALSE, SUB_CONTENT_MP3, PNULL);
#endif

        {
            MMIAP_PLAY_WIN_CALLBACK_PARAM_T win_param = {0};


            win_param.flag = MMIAPWIN_PLAY_FLAG_ALL_PANEL;
            MMIAP_DoWinCallback(MMIAP_WIN_TYPE_MAIN,
                                MMIAPWIN_FLAG_FUNC_PLAY,
                                &win_param,
                                sizeof(win_param)
                               );
        }
    }
    break;

    default:
        break;
    }
}

/*****************************************************************************/
//  Description : start timer for play mp3
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
LOCAL uint32 GetAPPeriodTime(
    uint32    mp3_time
)
{
    uint32              time_out = 0;

    //获得滚动条滚动的时间跨度，每次滚动一个默认滚动条的长度
    time_out = (mp3_time * 1000) / MMIMP3_PROGRESS_SCROLL_NUM;

    //set time out
    if((0 == mp3_time) ||
            (MMIMP3_PROGRESS_SCROLL_MIN_TIME > time_out))
    {
        time_out = MMIMP3_PROGRESS_SCROLL_MIN_TIME;
    }
    else if(time_out > MMIMP3_PROGRESS_SCROLL_MAX_TIME)
    {
        time_out = MMIMP3_PROGRESS_SCROLL_MAX_TIME;
    }

    SCI_TRACE_LOW("GetAPPeriodTime:time_out = %d",\
                  time_out);

    return time_out;
}

/*****************************************************************************/
//  Description : handle progress timer msg
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIMP3_HandleProgressTimer(uint8 timer_id, uint32 param)
{
    //should update info
    MMIMP3_GetPlayingDynInfo(PNULL, TRUE);


    //display progress
    {
        MMIAP_PLAY_WIN_CALLBACK_PARAM_T win_param = {0};


        win_param.flag = MMIAPWIN_PLAY_FLAG_ONLY_PROGRESS;
        MMIAP_DoWinCallback(MMIAP_WIN_TYPE_MAIN,
                            MMIAPWIN_FLAG_FUNC_PLAY,
                            &win_param,
                            sizeof(win_param)
                           );
    }
}

/*****************************************************************************/
//  Description : Get the playing Dyn Info
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIMP3_GetPlayingDynInfo(
    MMIMP3_PLAYING_DYN_INFO_T  *dyn_info_ptr,
    BOOLEAN                    is_real_time
)
{
    MMISRVAUD_PLAY_INFO_T    mp3_play_info = {0};
    MMISRVAP_ERROR_E error_code = 0;
    MMIMP3_PLAYING_DYN_INFO_T dyn_info = {0};

	SCI_TRACE_LOW("[MMIMP3] MMIMP3_GetPlayingDynInfo is_real_time = %d", is_real_time);
	
    GetDynPlayingInfo(&dyn_info);
    if(is_real_time)
    {
         SCI_TRACE_LOW("[MMIMP3] MMIMP3_GetPlayingDynInfo MMIAPIMP3_GetPlayerState() = %d", MMIAPIMP3_GetPlayerState());
         SCI_TRACE_LOW("[MMIMP3] MMIMP3_GetPlayingDynInfo s_mmiap_play_ctrl_info_ptr->audio_srv_handle = 0x%x", s_mmiap_play_ctrl_info_ptr->audio_srv_handle);
         SCI_TRACE_LOW("[MMIMP3] MMIMP3_GetPlayingDynInfo MMISRVAUD_IsAudPlaying = %d", MMISRVAUD_IsAudPlaying(s_mmiap_play_ctrl_info_ptr->audio_srv_handle));
        if((MMIMP3_PLAYER_AUDIO_ACTIVE == MMIAPIMP3_GetPlayerState()) && (MMISRVAUD_IsAudPlaying(s_mmiap_play_ctrl_info_ptr->audio_srv_handle)))
        {
            error_code = MMISRVAP_GetPlayingInfo(GetAPSrvHandle(), &mp3_play_info);
            SCI_TRACE_LOW("[MMIMP3] MMIMP3_GetPlayingDynInfo GetAPSrvHandle() = 0x%x", GetAPSrvHandle());
            SCI_TRACE_LOW("[MMIMP3] MMIMP3_GetPlayingDynInfo error_code = %d", error_code);
            if (MMISRVAP_ERROR_NONE == error_code)
            {
                dyn_info.cur_time   = mp3_play_info.cur_time;
                dyn_info.cur_offset = mp3_play_info.cur_data_offset;

                SetDynPlayingInfo(&dyn_info);
            }
        }
    }

    if(PNULL != dyn_info_ptr)
    {
        MMI_MEMCPY(dyn_info_ptr, sizeof(MMIMP3_PLAYING_DYN_INFO_T), &dyn_info,
                   sizeof(MMIMP3_PLAYING_DYN_INFO_T), sizeof(MMIMP3_PLAYING_DYN_INFO_T));
    }
}

/*****************************************************************************/
//  Description : Get the playing Dyn Info
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIMP3_GetPlayingDynInfo(
    MMIMP3_PLAYING_DYN_INFO_T  *dyn_info_ptr,
    BOOLEAN                    is_real_time
)
{
    MMIMP3_GetPlayingDynInfo(dyn_info_ptr, is_real_time);
}

/*****************************************************************************/
//  Description : Reset the playing Dyn Info
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIMP3_ResetPlayingDynInfo(
    void
)
{
    MMIMP3_PLAYING_DYN_INFO_T dyn_info = {0};
	MMIMP3_PLAYING_DYN_INFO_T pre_dyn_info = {0};

	GetDynPlayingInfo(&pre_dyn_info);
 	dyn_info.total_time   = pre_dyn_info.total_time;
 	dyn_info.total_length   = pre_dyn_info.total_length;
    //reset dyn info
    SetDynPlayingInfo(&dyn_info);
}

/*****************************************************************************/
//  Description : Callback function to MMI AUDIO
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIMP3_CallBackAfterPlay(
    MMISRVAP_ERROR_E  result,
    DPARAM                     param
)
{
    MMIMP3_PLAYING_DYN_INFO_T dyn_info = {0};


    //SCI_TRACE_LOW:"[MMIMP3]:MMIMP3_CallBackAfterPlay:result = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMP3_2760_112_2_18_2_3_53_34, (uint8*)"d", result);


    MMIMP3_ResetReviewInfo();


    if(MMISRVAP_ERROR_NONE != result)
    {
        {
            MMIAP_PLAY_WIN_CALLBACK_PARAM_T win_param = {0};


            win_param.flag = MMIAPWIN_PLAY_FLAG_WIN_SET_ALERT;
            win_param.srvap_result = result;
            MMIAP_DoWinCallback(MMIAP_WIN_TYPE_MAIN,
                                MMIAPWIN_FLAG_FUNC_PLAY,
                                &win_param,
                                sizeof(win_param)
                               );
        }
    }

    //set dynamic info
    GetDynPlayingInfo(&dyn_info);
    dyn_info.cur_offset = dyn_info.total_length;
    dyn_info.cur_time   = dyn_info.total_time;
    SetDynPlayingInfo(&dyn_info);

#ifdef MMIAP_SPECTRUM_SUPPORT
    MMIMP3_StopSpectrum(TRUE);
#endif



    {
        MMIAP_PLAY_WIN_CALLBACK_PARAM_T win_param = {0};


        win_param.flag = MMIAPWIN_PLAY_FLAG_ONLY_PROGRESS;
        MMIAP_DoWinCallback(MMIAP_WIN_TYPE_MAIN,
                            MMIAPWIN_FLAG_FUNC_PLAY,
                            &win_param,
                            sizeof(win_param)
                           );
    }

    MMITHEME_UpdateRect();

    //update mp3 widget in idle win
    {
        MMIAP_PLAY_WIN_CALLBACK_PARAM_T win_param = {0};


        win_param.flag = MMIAPWIN_PLAY_FLAG_UPDATE_MP3;
        MMIAP_DoWinCallback(MMIAP_WIN_TYPE_IDLE,
                            MMIAPWIN_FLAG_FUNC_PLAY,
                            &win_param,
                            sizeof(win_param)
                           );
    }

    if(MMIMP3_PLAYER_SWITCH_PAUSED != MMIAPIMP3_GetPlayerState())
    {
        MMIAPIMP3_SetPlayerState(MMIMP3_PLAYER_MUSIC_SWITCH);
    }


    {
        MMIAP_PLAY_WIN_CALLBACK_PARAM_T win_param = {0};


        win_param.flag = MMIAPWIN_PLAY_FLAG_PLAY_CALLBACK;
        MMIAP_DoWinCallback(MMIAP_WIN_TYPE_VIRTUAL,
                            MMIAPWIN_FLAG_FUNC_PLAY,
                            &win_param,
                            sizeof(win_param)
                           );
    }
}

/*****************************************************************************/
//  Description : Mp3正常播放结束的回调函数。
//                根据播放模式重新显示进度条和button和是否继续播放其他mp3等
//  Global resource dependence :
//  Author:xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIMP3_PlayMp3CallBack(void)
{
    MMIMP3_REVIEW_INFO_T review_info = {0};
    MMIMP3_PLAY_MODE_E play_mode = 0;

#ifdef MMIAP_F_RELEASE_BG_AUDIO
    BOOLEAN is_last_music = FALSE;
#endif

    MMIAP_GetReviewInfo(&review_info);
    //SCI_TRACE_LOW:"[MMIMP3] MMIAPIMP3_PlayMp3CallBack cur_play_mode = %d,review_state = %d,PlayerState = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMP3_2802_112_2_18_2_3_53_35, (uint8*)"ddd",\
                 MMIMP3_GetMp3PlayMode(), \
                 review_info.state, \
                 MMIAPIMP3_GetPlayerState()
                );

    if(MMIMP3_PLAYER_SWITCH_PAUSED != MMIAPIMP3_GetPlayerState() &&
            MMIMP3_PLAYER_MUSIC_SWITCH  != MMIAPIMP3_GetPlayerState())
    {
        return;
    }

    MMIMP3_SetAudioState(MMIMP3_AUDIO_STOPPED);

#ifdef MMIAP_SPECTRUM_SUPPORT
    MMIMP3_StopSpectrum(TRUE);  //需要在AUDIO close 之前，否则无意义
#endif
    MMIMP3_StopMainAnim(TRUE);
    MMIMP3_StopTitleAnim(TRUE);
#ifdef MMIAP_LYRIC_SUPPORT
    MMIMP3_StopLyric();
#endif

    if(MMIMP3_REVIEW_STATE_NONE != MMIMP3_GetReviewStatus())
    {
        MMIMP3_StopForwardOrBackwardTimer();
    }

    switch(review_info.state)
    {
    case MMIMP3_REVIEW_STATE_NONE:
    {
        MMIMP3_ResetPlayingDynInfo();

        play_mode = MMIMP3_GetMp3PlayModeAuto();
        switch(play_mode)  //@CR241305 2011.05.24
        {
            /*
            case MMIMP3_PLAY_ONE_ONCE:
                MMIAPIMP3_StopAudioPlayer();
                break;
            */
        case MMIMP3_PLAY_ONE_REPEAT:
            if(MMIMP3_PLAYER_MUSIC_SWITCH == MMIAPIMP3_GetPlayerState())
            {
                if(!IsCurPlayListAllNotSupported())
                {
                    MMIMP3_DelayPlayMp3(MMIMP3_ZERO_OFFSET);

                }
                else
                {
                    MMIAPIMP3_StopAudioPlayer();

#ifdef MMIAP_F_RELEASE_BG_AUDIO
                    is_last_music = TRUE;
#endif
                }
            }

            break;

        case MMIMP3_PLAY_LIST_ONCE:
        case MMIMP3_PLAY_LIST_REPEAT:
        case MMIMP3_PLAY_RANDOM_REPEAT:
        case MMIMP3_PLAY_RANDOM_ONCE:
            if(!IsCurPlayListAllNotSupported()  &&
                    MMIMP3_SetCurPlayFileIndex(FALSE)
              )
            {



                {
                    MMIAP_WIN_CALLBACK_RET_E cb_ret = 0;
                    MMIAP_PLAY_WIN_CALLBACK_PARAM_T win_param = {0};


                    win_param.flag = MMIAPWIN_PLAY_FLAG_ALL_PANEL;
                    win_param.flag |= MMIAPWIN_PLAY_FLAG_SWITCH_ALBUM;
                    cb_ret = MMIAP_DoWinCallback(MMIAP_WIN_TYPE_MAIN,
                                                 MMIAPWIN_FLAG_FUNC_PLAY,
                                                 &win_param,
                                                 sizeof(win_param)
                                                );

                    if(MMIAP_WIN_CALLBACK_RET_DONE_BY_CB != cb_ret)
                    {
#ifdef MMI_AUDIOPLAYER_ALBUM_SUPPORT
                        MMIAPIMP3_UpdateAllAlbumInfo(FALSE);
#endif
                    }
                }

                //MMIMP3_UpdatelistDisplay(TRUE);

                if(MMIMP3_PLAYER_MUSIC_SWITCH == MMIAPIMP3_GetPlayerState())
                {
                    MMIMP3_DelayPlayMp3(MMIMP3_ZERO_OFFSET);
                }
            }
            else
            {
                MMIAPIMP3_StopAudioPlayer();

#ifdef MMIAP_F_RELEASE_BG_AUDIO
                is_last_music = TRUE;
#endif
            }

            break;

        default:
            break;
        }
    }
    break;

    default:
        break;
    }
	if(MMK_IsOpenWin(MMIMP3_PLAY_LIST_WIN_ID))
	{
		MMK_PostMsg(MMIMP3_PLAY_LIST_WIN_ID, MSG_MP3_SWITCH_AUTO, PNULL, 0);
	}
    {
        MMIAP_PLAY_WIN_CALLBACK_PARAM_T win_param = {0};


        win_param.flag = MMIAPWIN_PLAY_FLAG_SWITCH_MP3;
        MMIAP_DoWinCallback(MMIAP_WIN_TYPE_IDLE,
                            MMIAPWIN_FLAG_FUNC_PLAY,
                            &win_param,
                            sizeof(win_param)
                           );
    }


#ifdef MMIAP_F_RELEASE_BG_AUDIO
    if (is_last_music
            && (MMIMP3_PLAY_LIST_ONCE == play_mode
                || MMIMP3_PLAY_RANDOM_ONCE== play_mode
                || MMIMP3_PLAY_ONE_REPEAT == play_mode
               )
       )
    {
        MMIAPIAP_ReleaseAudioHandle();
    }
#endif
}

/*****************************************************************************/
//  Description : 跳转到前一首或者下一首Mp3。
//              根据播放模式重新显示进度条和button和是否继续播放其他mp3等
//  Global resource dependence :
//  Author:jassmine.meng
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIMP3_GoToPreOrNextMp3(
    BOOLEAN      is_prev
)
{
    uint32                  total_num = MMIMP3_GetCurPlayListTotalNum();


    //SCI_TRACE_LOW:"[MMIMP3] MMIAPIMP3_GoToPreOrNextMp3 cur_play_mode = %d,review_state = %d, player_state = %d,audio_state = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMP3_2925_112_2_18_2_3_53_36, (uint8*)"dddd", MMIMP3_GetMp3PlayMode(), MMIMP3_GetReviewStatus(), MMIAPIMP3_GetPlayerState(), MMIMP3_GetAudioState());

    if(0 == total_num)
    {
        //SCI_TRACE_LOW:"[MMIMP3] MMIAPIMP3_GoToPreOrNextMp3 Total_num = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMP3_2929_112_2_18_2_3_53_37, (uint8*)"d", total_num);
        return;
    }

#ifdef MMIAP_F_HISTORY_PLAYLIST

    if(MMIAP_IsHistoryInit())
    {
        //step 1
        MMIAP_SetHistoryInitFlag(FALSE);

        //step 2
        {
            MMIAP_PLAY_WIN_CALLBACK_PARAM_T win_param = {0};


            win_param.flag = MMIAPWIN_PLAY_FLAG_UPDATE_BUTTON_STATE;
            MMIAP_DoWinCallback(MMIAP_WIN_TYPE_MAIN,
                                MMIAPWIN_FLAG_FUNC_PLAY,
                                &win_param,
                                sizeof(win_param)
                               );
        }

    }

#endif


#ifdef MMIAP_SPECTRUM_SUPPORT
    MMIMP3_StopSpectrum(TRUE);
#endif
    MMIMP3_StopMainAnim(TRUE);
    MMIMP3_StopTitleAnim(TRUE);
#ifdef MMIAP_LYRIC_SUPPORT
    MMIMP3_StopLyric();
#endif
    MMIMP3_ClearMp3AlertInfo(FALSE);
#ifdef MMI_AUDIOPLAYER_REVIEW_SUPPORT
    MMIMP3_ClearReviewMenu();
#endif



    MMIMP3_ResetCurPlayFileIndex(is_prev);



    //update mp3 widget in idle win
    {
        MMIAP_PLAY_WIN_CALLBACK_PARAM_T win_param = {0};


        win_param.flag = MMIAPWIN_PLAY_FLAG_SWITCH_MP3;
        win_param.param_ptr = (void*)is_prev;
        MMIAP_DoWinCallback(MMIAP_WIN_TYPE_IDLE,
                            MMIAPWIN_FLAG_FUNC_PLAY,
                            &win_param,
                            sizeof(win_param)
                           );
    }


    //当前如果正在播放,暂停mp3,跳转到前一首或者下一首Mp3仍然播放
    switch(MMIAPIMP3_GetPlayerState())
    {
    case MMIMP3_PLAYER_STATE_NONE:
    {

        MMIMP3_ResetReviewInfo();
        MMIMP3_ResetPlayingDynInfo();

        MMIMP3_GetCurFileContent();
    }
    break;

    case MMIMP3_PLAYER_AUDIO_ACTIVE:
        switch(MMIMP3_GetAudioState())
        {
        case MMIMP3_AUDIO_PLAYING:
            MMIMP3_ResetReviewInfo();
            StopMp3();
            MMIAPIMP3_PlayAudioPlayer(MMIMP3_ZERO_OFFSET);
            break;

        case MMIMP3_AUDIO_PAUSED:
            MMIMP3_ResetReviewInfo();
            StopMp3();
            MMIMP3_GetCurFileContent();
            break;

        case MMIMP3_AUDIO_STOPPED:
            break;

        default:
            break;
        }

        break;

    case MMIMP3_PLAYER_MUSIC_SWITCH:
    case MMIMP3_PLAYER_SWITCH_PAUSED:
        MMIMP3_DelayPlayMp3(MMIMP3_ZERO_OFFSET);
        break;

    default:
        break;
    }

    if(MMIMP3_AUDIO_PLAYING != MMIMP3_GetAudioState())
    {
        MMIAP_PLAY_WIN_CALLBACK_PARAM_T win_param = {0};


        win_param.flag = MMIAPWIN_PLAY_FLAG_ALL_PANEL;
        MMIAP_DoWinCallback(MMIAP_WIN_TYPE_MAIN,
                            MMIAPWIN_FLAG_FUNC_PLAY,
                            &win_param,
                            sizeof(win_param)
                           );
    }
}

/*****************************************************************************/
//  Description : whether the current playing file are all not supported
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsCurPlayListAllNotSupported(void)
{
    BOOLEAN     result = FALSE;
    MMIMP3_PLAY_MODE_E mode = MMIMP3_GetMp3PlayModeAuto();
    uint16 no_support_num = 0;
    uint32 total_num = MMIMP3_GetCurPlayListTotalNum();

    no_support_num = GetNoSupportNumber();
    if(MMIMP3_PLAY_LIST_REPEAT   == mode ||
            MMIMP3_PLAY_RANDOM_REPEAT == mode)
    {
        if(no_support_num >= total_num &&
                total_num > 0)
        {
            result = TRUE;
        }
    }
    else if(MMIMP3_PLAY_ONE_REPEAT == mode)
    {
        if(no_support_num >= 1 &&
                total_num > 0)
        {
            result = TRUE;
        }
    }

    return result;
}

/*****************************************************************************/
//  Description : Check Mp3 No Support Num
//  Global resource dependence : s_mp3_play_info
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIMP3_NoSupportNumInc(void)
{
    uint16 num = 0;

    num = GetNoSupportNumber();
    num++;
    SetNoSupportNumber(num);
}

/*****************************************************************************/
//  Description : Check Mp3 No Support Num
//  Global resource dependence : s_mp3_play_info
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIMP3_NoSupportNumClear(void)
{
    SetNoSupportNumber(0);
}

/*****************************************************************************/
//  Description : get current mp3 name,include suffix
//  Global resource dependence :
//  Author:jassmine.meng
//  Note: only for sub screen and idle
/*****************************************************************************/
PUBLIC void MMIAPIMP3_GetCurMp3Name(
    MMI_STRING_T   *mp3_name_ptr,
    uint16         max_name_len
)
{
    MMIMP3_LIST_FILE_BODY_T     cur_play_info = {0};
    wchar                       *full_path_ptr = PNULL;
    uint16                      file_name_len = FMM_SEARCH_FILENAME_MAXLEN + 1;

    if(PNULL == mp3_name_ptr || PNULL == mp3_name_ptr->wstr_ptr)
    {
        //SCI_TRACE_LOW:"[MMIMP3]:MMIAPIMP3_GetCurMp3Name, mp3_name_ptr = %x!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMP3_3220_112_2_18_2_3_54_40, (uint8*)"d", mp3_name_ptr);
        return ;
    }

    full_path_ptr = SCI_ALLOC_APP((FMM_SEARCH_FILENAME_MAXLEN + 1) * sizeof(wchar));

    if(PNULL == full_path_ptr)
    {
        return;
    }

    SCI_MEMSET(full_path_ptr, 0x00, (FMM_SEARCH_FILENAME_MAXLEN + 1) * sizeof(wchar));

    cur_play_info.full_path_len = FMM_SEARCH_FILENAME_MAXLEN;
    if(MMIMP3_GetCurPlayFileFullPath(cur_play_info.full_path_name, &(cur_play_info.full_path_len)))  //当前文件可能未被load
    {
        if(MMIAPIFMM_SplitFullPath(
                    cur_play_info.full_path_name,
                    cur_play_info.full_path_len,
                    PNULL, PNULL, PNULL, PNULL,
                    full_path_ptr, &file_name_len
                ))
        {
            file_name_len = MIN(max_name_len, file_name_len);
            MMI_WSTRNCPY(mp3_name_ptr->wstr_ptr, file_name_len, full_path_ptr, file_name_len, file_name_len);
            mp3_name_ptr->wstr_len = file_name_len;
        }
    }

    SCI_FREE(full_path_ptr);
}

/*****************************************************************************/
//  Description : handle camera key
//  Global resource dependence :
//  Author:jassmine.meng
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIMP3_HandleCameraKey(void)
{
    /*
        if(!MMIDEFAULT_GetFlipStatus())
        {
            MMIDEFAULT_TurnOnBackLight();

            //play or pause or resume, old requirement.
        }
    */
}

/*****************************************************************************/
//  Description : when format udisk,reset global variabel
//  Global resource dependence :
//  Author:jassmine.meng
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIMP3_ResetAllMp3(void)
{
    //停止播放
    MMIAPIMP3_StopAudioPlayer();

    MMIAPIMP3_StopApplet(FALSE);//@fen.xie

    InitMp3ModuleVariables();
    MMIMP3_ResetReviewInfo();
}

/*****************************************************************************/
//  Description : if it is current mp3 background play
//  Global resource dependence :
//  Author:jassmine.meng
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMP3_IsBgPlayMp3(
    const wchar *full_name_ptr, //in
    uint16     full_name_len   //in
)
{
    BOOLEAN                 result = FALSE;
    MMIMP3_LIST_FILE_BODY_T cur_play_info = {0};

    cur_play_info.full_path_len = FMM_SEARCH_FILENAME_MAXLEN;
    if(!MMIMP3_GetCurPlayFileFullPath(cur_play_info.full_path_name,&(cur_play_info.full_path_len)))  //当前文件可能未被load
    {
        return FALSE;
    }

    if(MMIMP3_PLAYER_STATE_NONE != MMIAPIMP3_GetPlayerState())
    {
        if(0 == MMIAPICOM_CompareTwoWstrExt(full_name_ptr, full_name_len,
                                            cur_play_info.full_path_name, cur_play_info.full_path_len, FALSE))
        {
            result = TRUE;
        }
    }

    return (result);
}

/*****************************************************************************/
//  Description : forward mp3
//  Global resource dependence :
//  Author:jassmine.meng
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMP3_ForwardMp3(void)
{
    BOOLEAN             result = FALSE;
    uint32              mp3_offset = 0;
    MMIMP3_PLAYING_DYN_INFO_T dyn_play_info = {0};

    if(MMIMP3_IsCurFileSeekSupport())
    {
        if(MMIMP3_AUDIO_PLAYING  == MMIMP3_GetAudioState() ||
                MMIMP3_AUDIO_PAUSED   == MMIMP3_GetAudioState())
        {
            if(MMIMP3_REVIEW_STATE_PLAY != MMIMP3_GetReviewStatus())
            {
                //get forward length
                MMIMP3_GetPlayingDynInfo(PNULL, TRUE);
                result = GetMp3CurrentOffset(TRUE, &mp3_offset);

                if(result)
                {
                    result = MMIAP_ForwardOrBackwardMp3(FALSE, TRUE, mp3_offset, MMIMP3_GetAudioState());
                }
                else
                {
                    //SCI_TRACE_LOW:"MMIAPIMP3_ForwardMp3:get mp3 current offset result %d is error!"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMP3_3353_112_2_18_2_3_54_41, (uint8*)"d", result);
                }
            }
            /*Start of      2009.9.3 for 对Review状态的MP3的快进快退的处理 */
            else//添加复读状态下的快退
            {
                MMIMP3_GetPlayingDynInfo(&dyn_play_info, TRUE);//先获取下整个mp3的信息
                result = GetReviewMp3CurrentOffset(TRUE, &mp3_offset);

                if(MMIMP3_HandleMp3ReviewEnd(dyn_play_info.cur_offset))
                {
                    //SCI_TRACE_LOW:"[MMIMP3]:MMIMP3_ReviewForwardMp3 ReviewEnd"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMP3_3363_112_2_18_2_3_54_42, (uint8*)"");
                }
                else
                {
                    if(result)
                    {
                        result = MMIAP_ForwardOrBackwardMp3(FALSE, TRUE, mp3_offset, MMIMP3_GetAudioState());
                    }
                    else
                    {
                        //SCI_TRACE_LOW:"MMIMP3_ReviewForwardMp3:get mp3 current offset result %d is error!"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMP3_3373_112_2_18_2_3_54_43, (uint8*)"d", result);
                    }
                }
            }

            /*End   of      2009.9.3 for 对Review状态的MP3的快进快退的处理 */
        }
    }
    else
    {
        //SCI_TRACE_LOW:"MMIAPIMP3_ForwardMp3:cur_state %d don't handle!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMP3_3381_112_2_18_2_3_54_44, (uint8*)"d", MMIMP3_GetAudioState());
    }

    return (result);
}
/*==========================================================
 * author    :
 * function  :
 * decr      :
 * parameter :
 * return    :
 * effect    : NO
 * history   :created  9/3/2009
 ==========================================================*/

LOCAL BOOLEAN GetReviewMp3CurrentOffset(
    BOOLEAN       is_forward,     //forward or backward
    uint32        *mp3_offset_ptr //in/out:从Mp3 pause置算起
)
{
    BOOLEAN             result = TRUE;
    uint32              forward_backward_offset = 0;
    MMIMP3_PLAYING_DYN_INFO_T dyn_info = {0};
    uint32 total_time = 0;
    uint32 end_point = 0;
    uint32 start_point = 0;
    MMIMP3_REVIEW_INFO_T review_info = {0};



    if(PNULL == mp3_offset_ptr)
    {
        //SCI_TRACE_LOW:"[MMIMP3]:GetReviewMp3CurrentOffset, mp3_offset_ptr = %x!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMP3_3408_112_2_18_2_3_54_45, (uint8*)"d", mp3_offset_ptr);
        return FALSE;
    }

    MMIAP_GetReviewInfo(&review_info);

    MMIMP3_GetPlayingDynInfo(&dyn_info, FALSE);

    //SCI_TRACE_LOW:"GetReviewMp3CurrentOffset:file_total_time = %d,file_total_len = %d,bitrate = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMP3_3415_112_2_18_2_3_54_46, (uint8*)"ddd", dyn_info.total_time, dyn_info.total_length, dyn_info.bit_rate);

    total_time = review_info.end_time - review_info.start_time;
    MMIAPIMP3_GetReviewStartPoint(&start_point);
    MMIAPIMP3_GetReviewEndPoint(&end_point);
    //get offset from pause point

    if(MMIMP3_FORWARD_BACKWARD_MIN_TIME < (total_time / MMIMP3_FORWARD_BACKWARD_MAX_NUM))
    {
        forward_backward_offset = dyn_info.total_length / MMIMP3_FORWARD_BACKWARD_MAX_NUM;
    }
    else if(MMIMP3_FORWARD_BACKWARD_MIN_TIME > total_time)
    {
        forward_backward_offset = dyn_info.total_length / MMIMP3_FORWARD_BACKWARD_MIN_NUM;
    }
    else
    {
        forward_backward_offset = (uint32)(MMIMP3_FORWARD_BACKWARD_MIN_TIME * (uint64_t)dyn_info.total_length / dyn_info.total_time);
    }

    //SCI_TRACE_LOW:"GetReviewMp3CurrentOffset:forward_backward_offset = %ld"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMP3_3435_112_2_18_2_3_54_47, (uint8*)"d", forward_backward_offset);

    *mp3_offset_ptr = forward_backward_offset;


    return (result);
}
/*****************************************************************************/
//  Description : backward mp3
//  Global resource dependence :
//  Author:jassmine.meng
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMP3_BackwardMp3(void)
{
    BOOLEAN             result = FALSE;
    uint32              mp3_offset = 0;

    if(MMIMP3_IsCurFileSeekSupport())
    {
        if((MMIMP3_AUDIO_PLAYING == MMIMP3_GetAudioState()) ||
                (MMIMP3_AUDIO_PAUSED  == MMIMP3_GetAudioState()))
        {
            if(MMIMP3_REVIEW_STATE_PLAY != MMIMP3_GetReviewStatus())
            {
                //get backward length and time offset
                MMIMP3_GetPlayingDynInfo(PNULL, TRUE);
                result = GetMp3CurrentOffset(FALSE, &mp3_offset);

                if(result)
                {
                    result = MMIAP_ForwardOrBackwardMp3(FALSE, FALSE, mp3_offset, MMIMP3_GetAudioState());
                }
                else
                {
                    //SCI_TRACE_LOW:"MMIAPIMP3_BackwardMp3:get mp3 current offset result %d is error!"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMP3_3468_112_2_18_2_3_54_48, (uint8*)"d", result);
                }
            }
            /*Start of      2009.9.3 for 对Review状态的MP3的快进快退的处理 */
            else//添加复读状态下的快退
            {
                MMIMP3_GetPlayingDynInfo(PNULL, TRUE);//先获取下整个mp3的信息
                result = GetReviewMp3CurrentOffset(FALSE, &mp3_offset);

                if(result)
                {
                    result = MMIAP_ForwardOrBackwardMp3(FALSE, FALSE, mp3_offset, MMIMP3_GetAudioState());
                }
                else
                {
                    //SCI_TRACE_LOW:"MMIMP3_ReviewBackwardMp3:get mp3 current offset result %d is error!"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMP3_3482_112_2_18_2_3_55_49, (uint8*)"d", result);
                }
            }

            /*End   of      2009.9.3 for 对Review状态的MP3的快进快退的处理 */

        }
    }
    else
    {
        //SCI_TRACE_LOW:"MMIAPIMP3_BackwardMp3:cur_state %d don't handle!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMP3_3490_112_2_18_2_3_55_50, (uint8*)"d", MMIMP3_GetAudioState());
    }

    return (result);
}
/*****************************************************************************/
//  Description : get mp3 current offset,play position
//  Global resource dependence :
//  Author:jassmine.meng
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetMp3CurrentOffset(
    BOOLEAN       is_forward,     //forward or backward
    uint32        *mp3_offset_ptr //in/out:从Mp3 pause置算起
)
{
    BOOLEAN             result = TRUE;
    uint32              forward_backward_offset = 0;
    MMIMP3_PLAYING_DYN_INFO_T dyn_info = {0};

    if(PNULL == mp3_offset_ptr)
    {
        //SCI_TRACE_LOW:"[MMIMP3]:GetMp3CurrentOffset, mp3_offset_ptr = %x!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMP3_3509_112_2_18_2_3_55_51, (uint8*)"d", mp3_offset_ptr);
        return FALSE;
    }

    MMIMP3_GetPlayingDynInfo(&dyn_info, FALSE);

    //SCI_TRACE_LOW:"GetMp3CurrentOffset:file_total_time = %d,file_total_len = %d,bitrate = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMP3_3516_112_2_18_2_3_55_52, (uint8*)"ddd", dyn_info.total_time, dyn_info.total_length, dyn_info.bit_rate);

    //get offset from pause point
    if(MMIMP3_FORWARD_BACKWARD_MIN_TIME < (dyn_info.total_time / MMIMP3_FORWARD_BACKWARD_MAX_NUM))
    {
        forward_backward_offset = dyn_info.total_length / MMIMP3_FORWARD_BACKWARD_MAX_NUM;
    }
    else if(MMIMP3_FORWARD_BACKWARD_MIN_TIME > dyn_info.total_time)
    {
        forward_backward_offset = dyn_info.total_length / MMIMP3_FORWARD_BACKWARD_MIN_NUM;
    }
    else
    {
        forward_backward_offset = (uint32)(MMIMP3_FORWARD_BACKWARD_MIN_TIME * (uint64_t)dyn_info.total_length / dyn_info.total_time);
    }

    //SCI_TRACE_LOW:"GetMp3CurrentOffset:forward_backward_offset = %ld"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMP3_3532_112_2_18_2_3_55_53, (uint8*)"d", forward_backward_offset);

    *mp3_offset_ptr = forward_backward_offset;


    return (result);
}

/*****************************************************************************/
//  Description : forward or backward mp3
//  Global resource dependence :
//  Author:jassmine.meng
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAP_ForwardOrBackwardMp3(
    BOOLEAN              is_tp_progress, //是否是Tp进度条
    BOOLEAN              is_forward,
    uint32               mp3_offset,     //按键:Mp3 pause位置算起,Tp进度条:Mp3 0位置算起
    MMIMP3_AUDIO_STATE_E cur_state
)
{
    BOOLEAN             result = FALSE;
    uint32              mp3_total_offset = 0;
    MMISRVAP_ERROR_E error_code = MMISRVAP_ERROR_NONE;

    //SCI_TRACE_LOW:"[MMIMP3] MMIAP_ForwardOrBackwardMp3 mp3_offset = %d, cur_state = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMP3_3554_112_2_18_2_3_55_54, (uint8*)"dd", mp3_offset, cur_state);

    if(!(MMIMP3_AUDIO_PLAYING == cur_state || MMIMP3_AUDIO_PAUSED == cur_state))
    {
        return result;
    }

    //get total mp3 offset,from start
    if(is_tp_progress)
    {
        mp3_total_offset = mp3_offset;
    }
    else
    {
        mp3_total_offset = GetMp3CurrentTotalOffset(is_forward, mp3_offset);
    }

    switch(cur_state)
    {
    case MMIMP3_AUDIO_PLAYING:
        result = MMIAP_SeekTo(GetAPSrvHandle(), mp3_total_offset);

        if(result)
        {
            //seek mp3 lyric
#ifdef MMIAP_LYRIC_SUPPORT
            MMIMP3_SeekLyric(mp3_total_offset, TRUE);
#endif


            {
                MMIAP_PLAY_WIN_CALLBACK_PARAM_T win_param = {0};


                win_param.flag = MMIAPWIN_PLAY_FLAG_ONLY_PROGRESS;
                MMIAP_DoWinCallback(MMIAP_WIN_TYPE_MAIN,
                                    MMIAPWIN_FLAG_FUNC_PLAY,
                                    &win_param,
                                    sizeof(win_param)
                                   );
            }
        }
        else
        {
            //SCI_TRACE_LOW:"MMIAP_ForwardOrBackwardMp3: MMIMP3_AUDIO_PLAYING audio seek result = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMP3_3589_112_2_18_2_3_55_55, (uint8*)"d", result);
        }

        break;

    case MMIMP3_AUDIO_PAUSED:
        error_code = MMISRVAP_Resume(GetAPSrvHandle());

        if(MMISRVAP_ERROR_NONE == error_code)
        {
            MMIMP3_SetAudioState(MMIMP3_AUDIO_PLAYING);
            MMIAP_SeekTo(GetAPSrvHandle(), mp3_total_offset);
#ifdef MMIAP_LYRIC_SUPPORT
            MMIMP3_ResumeLyric();
            MMIMP3_SeekLyric(mp3_total_offset, TRUE);
#endif


            {
                MMIAP_PLAY_WIN_CALLBACK_PARAM_T win_param = {0};


                win_param.flag = MMIAPWIN_PLAY_FLAG_BUTTON_PROGRESS;
                MMIAP_DoWinCallback(MMIAP_WIN_TYPE_MAIN,
                                    MMIAPWIN_FLAG_FUNC_PLAY,
                                    &win_param,
                                    sizeof(win_param)
                                   );
            }



#ifdef MMIAP_SPECTRUM_SUPPORT
            MMIMP3_StartSpectrum();
#endif
            MMIMP3_PlayTitleAnim(TRUE);

            MAIN_SetIdleBgPlayName(TRUE, MMIBGPLAY_TYPE_MP3);
#ifndef SUBLCD_SIZE_NONE
            MMISUB_SetSubLcdDisplay(FALSE, TRUE, SUB_CONTENT_MP3, PNULL);
#endif
        }
        else
        {
            //SCI_TRACE_LOW:"MMIAP_ForwardOrBackwardMp3: MMIMP3_AUDIO_PAUSED audio seek result = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMP3_3622_112_2_18_2_3_55_56, (uint8*)"d", result);
        }

        break;

    default:
        break;
    }

    return (result);
}

/*****************************************************************************/
//  Description : get mp3 current total offset,from 0 start
//  Global resource dependence :
//  Author:jassmine.meng
//  Note:
/*****************************************************************************/
LOCAL uint32 GetMp3CurrentTotalOffset(
    BOOLEAN           is_forward,
    uint32            forward_backward_offset//从Mp3 pause置算起
)
{
    uint32              mp3_total_offset = 0;
    MMIMP3_PLAYING_DYN_INFO_T dyn_info = {0};
    uint32 start_point = 0;
    uint32 end_point = 0;

    MMIMP3_GetPlayingDynInfo(&dyn_info, FALSE);

    //set total offset
    if(MMIMP3_REVIEW_STATE_PLAY != MMIMP3_GetReviewStatus())
    {
        if(is_forward)
        {
            mp3_total_offset = dyn_info.cur_offset + forward_backward_offset;

            if(mp3_total_offset > dyn_info.total_length)
            {
                mp3_total_offset = dyn_info.total_length;
            }
        }
        else
        {
            if(dyn_info.cur_offset > forward_backward_offset)
            {
                mp3_total_offset = dyn_info.cur_offset - forward_backward_offset;
            }
            else
            {
                mp3_total_offset = 0;
            }
        }


    }
    else
    {
        MMIAPIMP3_GetReviewStartPoint(&start_point);
        MMIAPIMP3_GetReviewEndPoint(&end_point);

        if(is_forward)
        {
            mp3_total_offset = dyn_info.cur_offset + forward_backward_offset;

            if(mp3_total_offset > end_point)
            {
                mp3_total_offset = end_point - 1;
            }
        }
        else
        {
            if(start_point + forward_backward_offset < dyn_info.cur_offset)
            {
                mp3_total_offset = dyn_info.cur_offset - forward_backward_offset;
            }
            else
            {
                mp3_total_offset = start_point;
            }
        }

    }

    //SCI_TRACE_LOW:"GetMp3CurrentTotalOffset:mp3_total_offset =%ld"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMP3_3699_112_2_18_2_3_55_57, (uint8*)"d", mp3_total_offset);

    return (mp3_total_offset);
}

/*****************************************************************************/
//  Description : To judge whether the current playing file support seek.
//  Global resource dependence :
//  Author: xingdong.li
//  Note: 依据后缀判断
/*****************************************************************************/
PUBLIC BOOLEAN MMIMP3_IsCurFileSeekSupport(
    void
)
{
    BOOLEAN                 result = FALSE;
    MMIMP3_LIST_FILE_BODY_T cur_file_info = {0};


    cur_file_info.full_path_len = FMM_SEARCH_FILENAME_MAXLEN;
    if(MMIMP3_GetCurPlayFileFullPath(cur_file_info.full_path_name, &(cur_file_info.full_path_len)))  //当前文件可能未被load
    {
        if(MMIMP3_IsFileSeekSupport(
                    cur_file_info.full_path_name,
                    cur_file_info.full_path_len
                ))
        {
            result = TRUE;
        }
    }

    return result;
}

/*****************************************************************************/
//  Description : To judge whether the music is supported for seeking.
//  Global resource dependence :
//  Author: xingdong.li
//  Note: 依据后缀判断
/*****************************************************************************/
PUBLIC BOOLEAN MMIMP3_IsFileSeekSupport(
    const wchar *full_path_ptr,
    uint16 full_path_len
)
{
    BOOLEAN            result = FALSE;
    MMISRVAUD_RING_FMT_E    audio_type = MMISRVAUD_RING_FMT_MAX;

#ifdef DRM_SUPPORT

    if(!MMIAPIMP3_IsDrmFile(
                full_path_ptr,
                full_path_len,
                &audio_type, PNULL, PNULL))
#endif
    {
        audio_type  = MMIAPICOM_GetMusicType(full_path_ptr, full_path_len);
    }

    switch(audio_type)
    {
    case MMISRVAUD_RING_FMT_MP3:
#ifdef MP3_SEEK_SUPPORT
        result = TRUE;
#endif
        break;

    case MMISRVAUD_RING_FMT_WMA:
#ifdef WMA_SEEK_SUPPORT
        result = TRUE;
#endif
        break;

    case MMISRVAUD_RING_FMT_AAC:
    case MMISRVAUD_RING_FMT_M4A:
#ifdef AAC_SEEK_SUPPORT
        result = TRUE;
#endif
        break;

    case MMISRVAUD_RING_FMT_AMR:
#ifdef AMR_SEEK_SUPPORT
        result = TRUE;
#endif
        break;

    case MMISRVAUD_RING_FMT_MIDI:
#ifdef MIDI_SEEK_SUPPORT
        result = TRUE;
#endif
        break;

    case MMISRVAUD_RING_FMT_WAVE:
#ifdef WAV_SEEK_SUPPORT
        result = TRUE;
#endif
        break;

    default:
        break;
    }

    //SCI_TRACE_LOW:"[MMIMP3]: MMIMP3_IsFileSeekSupport, audio_type = %d, result = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMP3_3955_112_2_18_2_3_55_64, (uint8*)"dd", audio_type, result);

    return result;
}


/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMP3_IsFirstInit(
    void
)
{
    MMIMP3_PLAY_INFO_T *info_ptr = GetApPlayInfoPtr();
    BOOLEAN is_first_init = FALSE;


    if (PNULL != info_ptr)
    {
        is_first_init = info_ptr->is_first_init;
    }

    return is_first_init;
}

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIMP3_SetFirstInit(
    BOOLEAN is_first_init
)
{
    MMIMP3_PLAY_INFO_T *info_ptr = GetApPlayInfoPtr();


    if (PNULL != info_ptr)
    {
        info_ptr->is_first_init = is_first_init;
    }
}

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMP3_IsPausedByService(
    void
)
{
    MMIMP3_PLAY_INFO_T *info_ptr = GetApPlayInfoPtr();
    BOOLEAN is_paused_by_service = FALSE;


    if (PNULL != info_ptr)
    {
        is_paused_by_service = info_ptr->is_paused_by_service;
    }

    return is_paused_by_service;
}

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIMP3_SetPausedByService(
    BOOLEAN is_paused_by_service
)
{
    MMIMP3_PLAY_INFO_T *info_ptr = GetApPlayInfoPtr();


    if (PNULL != info_ptr)
    {
        info_ptr->is_paused_by_service = is_paused_by_service;
    }
}

/*****************************************************************************/
//  Description : Get current audio state.
//  Global resource dependence :
//  Author:xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC MMIMP3_AUDIO_STATE_E MMIMP3_GetAudioState(
    void
)
{
    MMIMP3_PLAY_INFO_T *info_ptr = GetApPlayInfoPtr();
    MMIMP3_AUDIO_STATE_E state = 0;


    if (PNULL != info_ptr)
    {
        state = info_ptr->audio_state;
    }

    return state;
}

/*****************************************************************************/
//  Description : Get current audio state.
//  Global resource dependence :
//  Author:xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC MMIMP3_AUDIO_STATE_E MMIAPIMP3_GetAudioState(
    void
)
{
    return MMIMP3_GetAudioState();
}

/*****************************************************************************/
//  Description : Set current audio state.
//  Global resource dependence :
//  Author:xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIAP_SetAudioState(
    MMIMP3_AUDIO_STATE_E cur_state
)
{
    MMIMP3_PLAY_INFO_T *info_ptr = GetApPlayInfoPtr();


    if (PNULL != info_ptr)
    {
        info_ptr->audio_state = cur_state;
    }
}


/*****************************************************************************/
//  Description : Set current audio state.
//  Global resource dependence :
//  Author:xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIMP3_SetAudioState(
    MMIMP3_AUDIO_STATE_E cur_state
)
{
    MMIAP_SetAudioState(cur_state);

    {
        MMIAP_PLAY_WIN_CALLBACK_PARAM_T win_param = {0};


        win_param.flag = MMIAPWIN_PLAY_FLAG_UPDATE_BUTTON_STATE;
        MMIAP_DoWinCallback(MMIAP_WIN_TYPE_MAIN,
                            MMIAPWIN_FLAG_FUNC_PLAY,
                            &win_param,
                            sizeof(win_param)
                           );
    }
}

/*****************************************************************************/
//  Description : Get player state.
//  Global resource dependence :
//  Author:xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC MMIMP3_PLAYER_STATE_E MMIAPIMP3_GetPlayerState(
    void
)
{
    MMIMP3_PLAY_INFO_T *info_ptr = GetApPlayInfoPtr();
    MMIMP3_PLAYER_STATE_E state = 0;


    if (PNULL != info_ptr)
    {
        state = info_ptr->player_state;
    }

    return state;
}

/*****************************************************************************/
//  Description : Set player state.
//  Global resource dependence :
//  Author:xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIAP_SetPlayerState(
    MMIMP3_PLAYER_STATE_E cur_state
)
{
    MMIMP3_PLAY_INFO_T *info_ptr = GetApPlayInfoPtr();


    if (PNULL != info_ptr)
    {
        info_ptr->player_state = cur_state;
    }
}


/*****************************************************************************/
//  Description : Set player state.
//  Global resource dependence :
//  Author:xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIMP3_SetPlayerState(
    MMIMP3_PLAYER_STATE_E cur_state
)
{
    MMIAP_SetPlayerState(cur_state);


    {
        MMIAP_PLAY_WIN_CALLBACK_PARAM_T win_param = {0};


        win_param.flag = MMIAPWIN_PLAY_FLAG_UPDATE_BUTTON_STATE;
        MMIAP_DoWinCallback(MMIAP_WIN_TYPE_MAIN,
                            MMIAPWIN_FLAG_FUNC_PLAY,
                            &win_param,
                            sizeof(win_param)
                           );
    }

}

/*****************************************************************************/
//  Description : set the volume of mp3
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMP3_SetMp3Volume(
    BOOLEAN  is_increase
)
{
    BOOLEAN         is_vol_change = FALSE;
    uint8           volume = MMIAPISET_GetMultimVolume();

    //    if(!MMIAUDIO_IsAudioPlayerBgPaused())
    {
        if(is_increase) //增加音量
        {
            if(MMISET_VOL_MAX > volume)
            {
                volume++;
                is_vol_change = TRUE;
            }
        }
        else
        {
            if(MMISET_VOL_ZERO < volume)
            {
                volume--;
                is_vol_change = TRUE;
            }
        }

        if(is_vol_change)
        {
            MMIAPISET_SetMultimVolume(volume);
            MMISRVAP_SetVolume(GetAPSrvHandle(), volume);
        }
    }

    return (is_vol_change);
}

/*****************************************************************************/
//  Description : set the volume of mp3
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMP3_SetVolume(
    uint8  volume
)
{
    MMIAPISET_SetMultimVolume(volume);
#ifdef BT_AV_SNK_SUPPORT
	if(MMIAPIBT_IsA2dpSinkConnected())
	{
		SCI_TRACE_LOW("BT_AV, MMIAPIBT_SetA2dpSinkVolume");
		MMIAPIBT_SetA2dpSinkVolume(volume);
	}
	else
	{
		MMISRVAP_SetVolume(GetAPSrvHandle(),volume);
	}
#else
	MMISRVAP_SetVolume(GetAPSrvHandle(),volume);
#endif

    return TRUE;
}

/*****************************************************************************/
//  Description : Reset mp3 review info.
//  Global resource dependence :
//  Author:xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIMP3_ResetReviewInfo(
    void
)
{
    MMIMP3_REVIEW_INFO_T review_info = {0};


    //SCI_TRACE_LOW:"[MMIMP3]:MMIMP3_ResetReviewInfo entry"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMP3_11178_112_2_18_2_4_12_209, (uint8*)"");

    review_info.is_end_file_end = FALSE;
    review_info.start_point     = 0;
    review_info.end_point       = 0;
    review_info.state           = MMIMP3_REVIEW_STATE_NONE;

    MMIAP_SetReviewInfo(&review_info);
}

/*****************************************************************************/
//  Description : Set mp3 review starting point.
//  Global resource dependence :
//  Author:xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMP3_SetReviewStartPoint(
    void
)
{
    BOOLEAN result = FALSE;
    MMIMP3_PLAYING_DYN_INFO_T dyn_info = {0};
    MMIMP3_REVIEW_INFO_T review_info = {0};


    MMIAP_GetReviewInfo(&review_info);
    MMIMP3_GetPlayingDynInfo(&dyn_info, TRUE);

    if(MMIMP3_AUDIO_PLAYING  == MMIMP3_GetAudioState() ||
            MMIMP3_AUDIO_PAUSED   == MMIMP3_GetAudioState())
    {
        if(dyn_info.cur_time + (MMIMP3_PROGRESS_SCROLL_MAX_TIME << 2) / 1000 <= dyn_info.total_time)
        {
            review_info.start_point = dyn_info.cur_offset;
            review_info.start_time  = dyn_info.cur_time;
            review_info.state = MMIMP3_REVIEW_STATE_SET;
            result = TRUE;
        }
    }

    if (result)
    {
        MMISRVAP_REQ_REVIEW_PARAM_T review_param = {0};


        review_param.start_point = review_info.start_point;
        review_param.end_point = review_info.end_point;

        MMISRVAP_SetReviewPoint(GetAPSrvHandle(), \
                                &review_param
                               );

        MMIAP_SetReviewInfo(&review_info);
    }

    //SCI_TRACE_LOW:"[MMIMP3]:MMIMP3_SetReviewStartPoint result = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMP3_11209_112_2_18_2_4_12_210, (uint8*)"d", result);
    return result;
}

/*****************************************************************************/
//  Description : Get mp3 review starting point.
//  Global resource dependence :
//  Author:xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMP3_GetReviewStartPoint(
    uint32 *start_point_ptr
)
{
    BOOLEAN result = FALSE;
    MMIMP3_REVIEW_INFO_T review_info = {0};


    MMIAP_GetReviewInfo(&review_info);
    if(MMIMP3_REVIEW_STATE_NONE != review_info.state)
    {
        *start_point_ptr = review_info.start_point;
        result = TRUE;
    }

    return result;
}


/*****************************************************************************/
//  Description : Set mp3 review end point.
//  Global resource dependence :
//  Author:xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMP3_SetReviewEndPoint(
    void
)
{
    BOOLEAN result = FALSE;
    MMIMP3_PLAYING_DYN_INFO_T dyn_info = {0};
    MMIMP3_REVIEW_INFO_T review_info = {0};


    MMIAP_GetReviewInfo(&review_info);
    MMIMP3_GetPlayingDynInfo(&dyn_info, TRUE);

    if(MMIMP3_AUDIO_PLAYING  == MMIMP3_GetAudioState() ||
            MMIMP3_AUDIO_PAUSED   == MMIMP3_GetAudioState()
#ifdef MMIAP_F_HISTORY_PLAYLIST
            || (
                MMIAP_IsHistoryInit()
                && MMIMP3_AUDIO_STOPPED  == MMIMP3_GetAudioState()
            )
#endif
      )

    {
        if(dyn_info.cur_offset > review_info.start_point &&
                dyn_info.cur_time >= review_info.start_time + (MMIMP3_PROGRESS_SCROLL_MAX_TIME << 2) / 1000)
        {
            review_info.end_point = dyn_info.cur_offset;
            review_info.end_time  = dyn_info.cur_time;
            result = TRUE;
        }
        else if(dyn_info.cur_offset < review_info.start_point &&
                dyn_info.cur_time + (MMIMP3_PROGRESS_SCROLL_MAX_TIME << 2) / 1000 <= review_info.start_time)
        {
            review_info.end_point   = review_info.start_point;
            review_info.end_time    = review_info.start_time;
            review_info.start_point = dyn_info.cur_offset;
            review_info.start_time  = dyn_info.cur_time;
            result = TRUE;
        }
        else
        {
            result = FALSE;
        }
    }


    if (result)
    {
        MMISRVAP_REQ_REVIEW_PARAM_T review_param = {0};

        review_param.start_point = review_info.start_point;
        review_param.end_point = review_info.end_point;

        MMISRVAP_SetReviewPoint(GetAPSrvHandle(), \
                                &review_param
                               );


        MMIAP_SetReviewInfo(&review_info);
    }


    //SCI_TRACE_LOW:"[MMIMP3]:MMIMP3_SetReviewEndPoint result = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMP3_11270_112_2_18_2_4_12_211, (uint8*)"d", result);
    return result;
}

/*****************************************************************************/
//  Description : Get mp3 review end point.
//  Global resource dependence :
//  Author:xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMP3_GetReviewEndPoint(
    uint32 *end_point_ptr
)
{
    BOOLEAN result = FALSE;
    MMIMP3_REVIEW_INFO_T review_info = {0};


    MMIAP_GetReviewInfo(&review_info);
    if(MMIMP3_REVIEW_STATE_NONE != review_info.state)
    {
        *end_point_ptr = review_info.end_point;
        result = TRUE;
    }

    return result;
}

/*****************************************************************************/
//  Description : Start review.
//  Global resource dependence :
//  Author:xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIMP3_StartReview(
    void
)
{
    BOOLEAN  result = TRUE;
    MMIMP3_REVIEW_INFO_T review_info = {0};


    //SCI_TRACE_LOW:"[MMIMP3]:MMIMP3_StartReview audio state = %d, player_state = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMP3_11303_112_2_18_2_4_13_212, (uint8*)"dd",\
                 MMIMP3_GetAudioState(), MMIAPIMP3_GetPlayerState());


    MMIAP_GetReviewInfo(&review_info);
    switch(MMIMP3_GetAudioState())
    {
    case MMIMP3_AUDIO_PLAYING:
    {
        review_info.state = MMIMP3_REVIEW_STATE_PLAY;
    }
    break;

    case MMIMP3_AUDIO_PAUSED:
    {
        //MMISRVAP_Stop(GetAPSrvHandle());
        review_info.state = MMIMP3_REVIEW_STATE_PLAY;
        //result = MMIMP3_PlayMp3(s_mp3_review_info.start_point);
    }

    break;

    case MMIMP3_AUDIO_STOPPED:
    {
        review_info.state = MMIMP3_REVIEW_STATE_PLAY;
        //result = MMIMP3_PlayMp3(s_mp3_review_info.start_point);
    }
    break;

    default:
        result = FALSE;
        break;
    }

    if (result)
    {
        MMIAP_SetReviewInfo(&review_info);

        MMISRVAP_StartReview(GetAPSrvHandle());
    }

}

#ifdef MMI_AUDIOPLAYER_REVIEW_SUPPORT
/*****************************************************************************/
//  Description : Exit review.
//  Global resource dependence :
//  Author:xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIMP3_ExitReview(
    void
)
{
    //SCI_TRACE_LOW:"[MMIMP3]:MMIMP3_ExitReview audio state = %d, player_state = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMP3_11364_112_2_18_2_4_13_214, (uint8*)"dd", MMIMP3_GetAudioState(), MMIAPIMP3_GetPlayerState());


    MMISRVAP_StopReview(GetAPSrvHandle());

    //reset review
    MMIMP3_ResetReviewInfo();

}
#endif
#ifdef MMI_AUDIOPLAYER_REVIEW_SUPPORT
/*****************************************************************************/
//  Description : Handle review control
//  Global resource dependence :
//  Author:xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIMP3_HandleMp3ReviewPlay(
    void
)
{
    MMIMP3_REVIEW_INFO_T review_info = {0};

    //SCI_TRACE_LOW:"[MMIMP3]:MMIMP3_HandleMp3ReviewPlay entry"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMP3_11440_112_2_18_2_4_13_217, (uint8*)"");

    MMIAP_GetReviewInfo(&review_info);
    switch(review_info.state)
    {
    case MMIMP3_REVIEW_STATE_NONE:
		if (s_review_process_sucessful)
		{
        	MMIMP3_SetReviewStartPoint();
		}
        break;

    case MMIMP3_REVIEW_STATE_SET:
        if(MMIMP3_SetReviewEndPoint())
        {
            MMIMP3_StartReview();
        }
		else
		{
			MMISRVAP_ResetReviewInfo(GetAPSrvHandle());
			MMIMP3_ResetReviewInfo();
		}

        break;

    case MMIMP3_REVIEW_STATE_PLAY:
        MMIMP3_ExitReview();
        break;

    default:
        break;
    }
}
#endif
/*****************************************************************************/
//  Description : Handle the end of the review section.
//  Global resource dependence :
//  Author:xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMP3_HandleMp3ReviewEnd(
    uint32 cur_offset
)
{
    BOOLEAN result = FALSE;
    MMISRVAUD_PLAY_INFO_T    mp3_play_info = {0};
    MMIMP3_REVIEW_INFO_T review_info = {0};


    MMIAP_GetReviewInfo(&review_info);
    if(review_info.end_point <= cur_offset                       &&
            MMIMP3_REVIEW_STATE_PLAY == review_info.state             &&
            !review_info.is_end_file_end                              &&
            !MMIMP3_IsSeeking())
    {
        if(MMIMP3_AUDIO_PLAYING  == MMIMP3_GetAudioState() &&
                MMISRVAP_ERROR_NONE == MMISRVAP_GetPlayingInfo(GetAPSrvHandle(), &mp3_play_info))
        {
            MMIMP3_StartReview();
            result = TRUE;
        }
    }

    return result;
}

/*****************************************************************************/
//  Description : Get the review state.
//  Global resource dependence :
//  Author:xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC MMIMP3_REVIEW_STATE_TYPE_E MMIMP3_GetReviewStatus(
    void
)
{
    MMIMP3_REVIEW_INFO_T review_info = {0};


    MMIAP_GetReviewInfo(&review_info);
    return review_info.state;
}

/*****************************************************************************/
//  Description : Get the review state.
//  Global resource dependence :
//  Author:xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC MMIMP3_REVIEW_STATE_TYPE_E MMIAPIMP3_GetReviewStatus(
    void
)
{
    return MMIMP3_GetReviewStatus();
}

/*****************************************************************************/
//  Description : whether the audio player is playing
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMP3_IsPlayerPlaying(void)
{
    BOOLEAN     result = FALSE;

    if(MMIMP3_PLAYER_MUSIC_SWITCH == MMIAPIMP3_GetPlayerState() ||
            MMIMP3_AUDIO_PLAYING       == MMIMP3_GetAudioState())
    {
        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : whether the audio player is paused
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMP3_IsPlayerPaused(void)
{
    BOOLEAN     result = FALSE;

    if(MMIMP3_PLAYER_SWITCH_PAUSED == MMIAPIMP3_GetPlayerState() ||
            MMIMP3_AUDIO_PAUSED         == MMIMP3_GetAudioState())
    {
        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : whether the audio player is stopped
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMP3_IsPlayerStopped(void)
{
    return (MMIMP3_PLAYER_STATE_NONE == MMIAPIMP3_GetPlayerState());
}

/*****************************************************************************/
//  Description : whether the audio player is not stopped
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMP3_IsPlayerActive(void)
{
    return (!MMIAPIMP3_IsPlayerStopped());
}

/*****************************************************************************/
//  Description : whether the audio player is opened
//  Global resource dependence :
//  Author: chunyou
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIAP_IsOpened(void)
{
    BOOLEAN ret = FALSE;
    uint32 applet_id = SPRD_MP3_APPLET_ID;


    if(MMK_GetAppletState(applet_id, MMI_APPLET_STATE_BG_HIDE_WIN)
            || MMK_GetAppletState(applet_id, MMI_APPLET_STATE_BG)
      )
    {
        ret = TRUE;
    }

    SCI_TRACE_LOW("[MMIMP3] MMIAPIAP_IsOpened ret=%d",
                  ret);

    return ret;
}

/*****************************************************************************/
//  Description : Fatal error.
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIMP3_FatalError(void)
{
    //SCI_TRACE_LOW:"[MMIMP3]: FATAL ERROR"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMP3_11868_112_2_18_2_4_14_222, (uint8*)"");

    {
        MMIAP_PLAY_WIN_CALLBACK_PARAM_T win_param = {0};


        win_param.flag = MMIAPWIN_PLAY_FLAG_FATAL_ERROR;
        MMIAP_DoWinCallback(MMIAP_WIN_TYPE_VIRTUAL,
                            MMIAPWIN_FLAG_FUNC_PLAY,
                            &win_param,
                            sizeof(win_param)
                           );
    }
}

/*****************************************************************************/
//  Description : Handle Fatal Error.
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIMP3_HandleFatalError(void)
{
    MMIPUB_OpenAlertWarningWin(TXT_ERROR);

    MMIMP3_ClearCurPlayInfo();

    MMIAPIMP3_ResetAllMp3();
}

#ifdef MMIAP_F_HISTORY_PLAYLIST
/*****************************************************************************/
//  Description : get audio player current play info
//  Global resource dependence :
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC MMIMP3_PLAY_INFO_T *MMIAP_GetAPCurPlayInfo(void)
{
    return (MMIMP3_PLAY_INFO_T*)GetApPlayInfoPtr();
}

/*****************************************************************************/
//  Description : init audio player old info
//  Global resource dependence :
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAP_InitOldInfo(void)
{
    MMIMP3_PLAY_INFO_T *info_ptr = GetApPlayInfoPtr();


    if (PNULL == info_ptr)
    {
        return;
    }

    SCI_MEMSET(&info_ptr->old_info, 0, sizeof(info_ptr->old_info));
}

/*****************************************************************************/
//  Description : get audio player old info
//  Global resource dependence :
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC MMIMP3_OLD_INFO_T *MMIAP_GetCurOldInfo(void)
{
    MMIMP3_PLAY_INFO_T *info_ptr = GetApPlayInfoPtr();
    MMIMP3_OLD_INFO_T *old_ptr = PNULL;


    if (PNULL != info_ptr)
    {
        old_ptr = &info_ptr->old_info;
    }

    return (MMIMP3_OLD_INFO_T*)old_ptr;
}
#endif //#ifdef MMIAP_F_HISTORY_PLAYLIST



#ifdef BLUETOOTH_SUPPORT
/*****************************************************************************/
//  Description : Set the mp3 bt setting mode.
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIMP3_SetBtSettingMode(
    MMIMP3_BT_SETTING_TYPE bt_mode
)
{
    MMIMP3_PLAY_INFO_T *info_ptr = GetApPlayInfoPtr();


    if (PNULL != info_ptr)
    {
        info_ptr->bt_mode = bt_mode;
    }
}

/*****************************************************************************/
//  Description : Get the mp3 bt setting mode.
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC MMIMP3_BT_SETTING_TYPE MMIMP3_GetBtSettingMode(
    void
)
{
    MMIMP3_PLAY_INFO_T *info_ptr = GetApPlayInfoPtr();
    MMIMP3_BT_SETTING_TYPE bt_mode = 0;


    if (PNULL != info_ptr)
    {
        bt_mode = info_ptr->bt_mode;
    }

    SCI_TRACE_LOW("[MMIMP3] MMIMP3_GetBtSettingMode = %d", bt_mode);
    return bt_mode;
}
#endif

/*****************************************************************************/
//  Description : handle play end
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
void HandleCallbackPlayEnd(
    MMISRVAP_HANDLE ap_handle, //in
    MMISRVAP_CALLBACK_PARAM_T *callback_param_ptr //in
)
{

    if (PNULL == callback_param_ptr
            || PNULL == ap_handle
       )
    {
        return;
    }


    MMISRVAP_Stop(ap_handle);

    MMIMP3_CallBackAfterPlay(callback_param_ptr->error_code, NULL);
}

/*****************************************************************************/
//  Description : handle playing
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
void HandleCallbackPlaying(
    MMISRVAP_HANDLE ap_handle, //in
    MMISRVAP_CALLBACK_PARAM_T *callback_param_ptr //in

)
{
    MMISRVAP_PLAYING_STATE_E play_ret = 0;
    MMISRVAP_CALLBACK_PLAYING_PARAM_T *play_param_ptr = PNULL;

    if (PNULL == ap_handle
            || PNULL == callback_param_ptr
       )
    {
        SCI_TRACE_LOW("[MMIMP3] PNULL == ap_handle or callback_param_ptr return error!");
        return;
    }


    SCI_TRACE_LOW("[MMIMP3] callback_param_ptr->error_code = %d", callback_param_ptr->error_code);
    switch(callback_param_ptr->error_code)
    {
    case MMISRVAP_ERROR_NONE:

        if (PNULL != callback_param_ptr->param_ptr)
        {
            play_param_ptr = (MMISRVAP_CALLBACK_PLAYING_PARAM_T*)callback_param_ptr->param_ptr;
            play_ret = play_param_ptr->play_ret;

            if (MMISRVAP_PLAYING_OK == play_ret)
            {
#ifdef DRM_SUPPORT
                MMIAPIMP3_ConsumeDrmRight();
#endif
            }
        }

        break;
    default:
        break;
    }
}

/*****************************************************************************/
//  Description : handle ext event for bt
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/

void HandleCallbackExtEvent(
    void *param_ptr,
    uint32 size_of_param
)
{
#if (defined BT_A2DP_SUPPORT || defined BT_HFG_SUPPORT) && (defined BLUETOOTH_SUPPORT)

    MMISRVAP_CALLBACK_BT_PARAM_T *bt_ptr = PNULL;

    if (PNULL == param_ptr)
    {
        return;
    }

    bt_ptr = (MMISRVAP_CALLBACK_BT_PARAM_T*)param_ptr;

    switch(bt_ptr->bt_event)
    {
    case MMISRVAUD_BT_EVENT_KEY_PAUSE:
        MMIMP3_EventPause();
        break;
    case MMISRVAUD_BT_EVENT_KEY_RESUME:
        MMIMP3_EventPlay();
        break;

	case MMISRVAUD_BT_EVENT_KEY_STOP:
		MMIMP3_EventStop();
		break;

    case MMISRVAUD_BT_EVENT_KEY_PREV:
        MMIMP3_EventPre();
        if(MMK_IsOpenWin(MMIMP3_PLAY_LIST_WIN_ID))
		{
			MMK_PostMsg(MMIMP3_PLAY_LIST_WIN_ID, MSG_MP3_SWITCH_BY_BT, PNULL, 0);
		}
        break;

    case MMISRVAUD_BT_EVENT_KEY_NEXT:
        MMIMP3_EventNext();
		if(MMK_IsOpenWin(MMIMP3_PLAY_LIST_WIN_ID))
		{
			MMK_PostMsg(MMIMP3_PLAY_LIST_WIN_ID, MSG_MP3_SWITCH_BY_BT, PNULL, 0);
		}
        break;

    case MMISRVAUD_BT_EVENT_KEY_VOL_UP:
        MMIAPIMP3_TurnVol(MMICOM_PANEL_OPEN_INC);
        break;

    case MMISRVAUD_BT_EVENT_KEY_VOL_DOWN:
        MMIAPIMP3_TurnVol(MMICOM_PANEL_OPEN_DEC);
        break;

    default:
        break;
    }

#endif
	if(GPIO_CheckHeadsetStatus())
	{
		MMIMP3_EventPlayOrPause();
	}

}

/*****************************************************************************/
//  Description : handle play end review
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
void HandleCallbackPlayEndReview(
    MMISRVAP_HANDLE ap_handle, //in
    MMISRVAP_CALLBACK_PARAM_T *callback_param_ptr //in
)
{
    MMISRVAP_CALLBACK_REVIEW_PLAY_END_PARAM_T *review_ptr = PNULL;
    MMIMP3_REVIEW_INFO_T review_info = {0};


    if (PNULL == ap_handle
            || PNULL == callback_param_ptr
       )
    {
        return;
    }


    review_ptr = callback_param_ptr->param_ptr;
    if (PNULL != review_ptr)
    {
        MMIAP_GetReviewInfo(&review_info);

        review_info.is_end_file_end = review_ptr->is_end_file;
        review_info.end_point = review_ptr->total_length;
        review_info.end_time = review_ptr->total_time;

        MMIAP_SetReviewInfo(&review_info);
    }

}

/*****************************************************************************/
//  Description : update review
//  Global resource dependence :
//  Author:robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAP_UpdateReviewStart(
    void
)
{
    MMIMP3_REVIEW_INFO_T review_info = {0};


    SCI_TRACE_LOW("[MMIMP3]:MMIAP_UpdateReviewStart audio state = %d, player_state = %d",\
                  MMIMP3_GetAudioState(), MMIAPIMP3_GetPlayerState());


    MMIAP_GetReviewInfo(&review_info);
    switch(MMIMP3_GetAudioState())
    {
    case MMIMP3_AUDIO_PLAYING:
    case MMIMP3_AUDIO_PAUSED:
    {

        MMIMP3_SetAudioState(MMIMP3_AUDIO_PLAYING);

        //seek mp3 lyric
#ifdef MMIAP_LYRIC_SUPPORT
        MMIMP3_SeekLyric(review_info.start_point, TRUE);
#endif

        //display progress

        {
            MMIAP_PLAY_WIN_CALLBACK_PARAM_T win_param = {0};


            win_param.flag = MMIAPWIN_PLAY_FLAG_ONLY_PROGRESS;
            MMIAP_DoWinCallback(MMIAP_WIN_TYPE_MAIN,
                                MMIAPWIN_FLAG_FUNC_PLAY,
                                &win_param,
                                sizeof(win_param)
                               );
        }

        review_info.state = MMIMP3_REVIEW_STATE_PLAY;
    }
    break;
    default:
        break;
    }



    MMIAP_SetReviewInfo(&review_info);
    {
        MMIAP_PLAY_WIN_CALLBACK_PARAM_T win_param = {0};


        win_param.flag = MMIAPWIN_PLAY_FLAG_MSG_SOFT_BAR;
        MMIAP_DoWinCallback(MMIAP_WIN_TYPE_MAIN,
                            MMIAPWIN_FLAG_FUNC_PLAY,
                            &win_param,
                            sizeof(win_param)
                           );
    }
}

/*****************************************************************************/
//  Description : Exit review.
//  Global resource dependence :
//  Author:xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIMP3_UpdateReviewStop(
    void
)
{
    MMIMP3_REVIEW_INFO_T review_info = {0};


    SCI_TRACE_LOW("[MMIMP3]:MMIMP3_UpdateReviewStop audio state = %d, player_state = %d",\
                  MMIMP3_GetAudioState(), MMIAPIMP3_GetPlayerState());

    MMIAP_GetReviewInfo(&review_info);
    switch(review_info.state)
    {
    case MMIMP3_REVIEW_STATE_NONE:
    case MMIMP3_REVIEW_STATE_SET:
        break;

    case MMIMP3_REVIEW_STATE_PLAY:
        if(review_info.is_end_file_end)
        {
            ;
        }
        else
        {
            switch(MMIMP3_GetAudioState())
            {
            case MMIMP3_AUDIO_PLAYING:
            case MMIMP3_AUDIO_PAUSED:

                MMIMP3_SetAudioState(MMIMP3_AUDIO_PLAYING);

#ifdef MMIAP_LYRIC_SUPPORT
                MMIMP3_SeekLyric(review_info.end_point, TRUE);
#endif

                {
                    MMIAP_PLAY_WIN_CALLBACK_PARAM_T win_param = {0};


                    win_param.flag = MMIAPWIN_PLAY_FLAG_SHOW_LYRIC_NOW;
                    win_param.flag |= MMIAPWIN_PLAY_FLAG_ONLY_PROGRESS;
                    MMIAP_DoWinCallback(MMIAP_WIN_TYPE_MAIN,
                                        MMIAPWIN_FLAG_FUNC_PLAY,
                                        &win_param,
                                        sizeof(win_param)
                                       );
                }
                break;
            default:
                break;
            }
        }

        break;

    default:
        break;
    }


    {
        MMIAP_PLAY_WIN_CALLBACK_PARAM_T win_param = {0};


        win_param.flag = MMIAPWIN_PLAY_FLAG_MSG_SOFT_BAR;
        MMIAP_DoWinCallback(MMIAP_WIN_TYPE_MAIN,
                            MMIAPWIN_FLAG_FUNC_PLAY,
                            &win_param,
                            sizeof(win_param)
                           );
    }
}


/*****************************************************************************/
//  Description : mmi ap callback to ap srv
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL void SRVAP_CALLBACK(
    MMISRVAP_CALLBACK_PARAM_T *callback_param_ptr, //in
    uint32 size_of_callback_param //in
)
{
    MMISRVAP_EVENT_E event_id = 0;
    MMISRVAP_HANDLE ap_handle = PNULL;

    if (PNULL == callback_param_ptr)
    {
        return;
    }


    event_id = callback_param_ptr->event_id;
    ap_handle = callback_param_ptr->handle;

    SCI_TRACE_LOW("[MMIMP3] AP_CALLBACK event_id=%d",\
                  event_id);

    SCI_TRACE_LOW("[MMIMP3] AP_CALLBACK ap_handle=0x%X",\
                  ap_handle);

    if (PNULL == ap_handle)
    {
        return;
    }

    switch(event_id)
    {
    case MMISRVAP_EVENT_UPDATE_AUDIO_HANDLE:
    case MMISRVAP_EVENT_OPEN:

        if (PNULL != callback_param_ptr->param_ptr)
        {
            MMISRV_HANDLE_T audio_handle = PNULL;

            audio_handle = *((MMISRV_HANDLE_T*)callback_param_ptr->param_ptr);
            SetAPAudioSrvHandle(audio_handle);
        }
        break;
    case MMISRVAP_EVENT_TO_DO_PAUSE:

        if (MMISRVAP_ERROR_NONE != callback_param_ptr->error_code)
        {
            break;
        }

        if(MMIAPIMP3_IsPlayerPlaying())
        {
            MMIAPIMP3_PauseMp3(TRUE);
        }

        break;

    case MMISRVAP_EVENT_TO_DO_RESUME:
    case MMISRVAP_EVENT_MGR_RESUME:

        if (MMISRVAP_ERROR_NONE != callback_param_ptr->error_code)
        {
            break;
        }

        MMISRVAP_SetVolume(GetAPSrvHandle(), (MMISRVAUD_VOLUME_T)MMIAPISET_GetMultimVolume());

        if(MMIMP3_IsPausedByService())
        {
            MMIMP3_SetPausedByService(FALSE);

            if(MMIAPIMP3_IsPlayerPaused())
            {
                MMIAPIMP3_ResumeMp3(TRUE);
            }
        }

        break;

    case MMISRVAP_EVENT_PLAY_END:

        HandleCallbackPlayEnd(ap_handle,
                              callback_param_ptr
                             );
        break;

    case MMISRVAP_EVENT_PLAYING:

        HandleCallbackPlaying(ap_handle,
                              callback_param_ptr
                             );
        break;


    case MMISRVAP_EVENT_EXT:
        HandleCallbackExtEvent(callback_param_ptr->param_ptr,
                               callback_param_ptr->size_of_param
                              );
        break;
    case MMISRVAP_EVENT_PERIOD_TIMER:

        MMIAPIMP3_HandleProgressTimer(0, PNULL);
        break;
    case MMISRVAP_EVENT_REVIEW_PLAY_END:
        HandleCallbackPlayEndReview(ap_handle,
                                    callback_param_ptr
                                   );
        break;
    case MMISRVAP_EVENT_REVIEW_START:
        if (MMISRVAP_ERROR_NONE != callback_param_ptr->error_code)
        {
            break;
        }

        MMIAP_UpdateReviewStart();
        break;
    case MMISRVAP_EVENT_REVIEW_STOP:
        if (MMISRVAP_ERROR_NONE != callback_param_ptr->error_code)
        {
            break;
        }

        MMIMP3_UpdateReviewStop();
        break;
    default:
        break;
    }

}

/*****************************************************************************/
//  Description : mp3 alert call back
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIAP_Mp3AlertCallback(
    void
)
{
    //SCI_TRACE_LOW:"[MMIMP3] Mp3AlertCallback enter"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMP3_1696_112_2_18_2_3_51_17, (uint8*)"");
    MMIMP3_CallBackAfterPlay(MMISRVAP_ERROR_NONE, NULL);
}

/*****************************************************************************/
//  Description : init play ctrl info
//  Global resource dependence :
//  Author:robert.wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAP_InitPlayCtrlInfo(
    void
)
{
    BOOLEAN ret = FALSE;


    ret = CreatePlayCtrlInfo();
    SCI_TRACE_LOW("[MMIAP] MMIAP_InitPlayCtrlInfo ret=%d",
                  ret);

    if (ret)
    {
        InitMp3ModuleVariables();
        MMIMP3_ResetReviewInfo();
    }

    return ret;
}

#ifdef MMIAP_F_FIRST_SEARCH
/*****************************************************************************/
//  Description : is first search or not
//  Global resource dependence :
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAP_IsFirstSearch(void)
{
    BOOLEAN is_first = FALSE;
    MMIAP_PLAY_CTRL_INFO_T *info_ptr = GetApPlayCtrlInfoPtr();


    if (PNULL != info_ptr)
    {
        is_first = info_ptr->is_first_search;
    }

    SCI_TRACE_LOW("[MMIAP] MMIAP_IsFirstSearch is_first=%d",
                  is_first);

    return is_first;
}

/*****************************************************************************/
//  Description : set first search flag
//  Global resource dependence :
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAP_SetFirstSearchFlag(BOOLEAN is_first)
{
    MMIAP_PLAY_CTRL_INFO_T *info_ptr = GetApPlayCtrlInfoPtr();


    if (PNULL != info_ptr)
    {
        info_ptr->is_first_search = is_first;
    }


    SCI_TRACE_LOW("[MMIAP] MMIAP_SetFirstSearchFlag is_first=%d",
                  is_first);
}

/*****************************************************************************/
//  Description : CheckSearchFlag
//  Global resource dependence :
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAP_CheckSearchFlag(void)
{

    if(MMIMP3_IsListFileExist(TRUE,0))
    {
        MMIAP_SetFirstSearchFlag(FALSE);
    }
    else
    {
        MMIAP_SetFirstSearchFlag(TRUE);
    }
}

#endif //#ifdef MMIAP_F_FIRST_SEARCH

/*****************************************************************************/
//  Description : mgr resume
//  Global resource dependence :
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAP_MGR_Resume(
    void
)
{
    BOOLEAN ret = FALSE;


    if (MMISRVAP_ERROR_NONE == MMISRVAP_MGR_Resume(GetAPSrvHandle()))
    {
        ret = TRUE;
    }

    SCI_TRACE_LOW("[MMIAP] MMIAP_MGR_Resume ret=%d",
                  ret);

    return ret;
}

/*****************************************************************************/
//  Description : Stop play state
//  Global resource dependence :
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL void StopPlayState(void)
{
    MMIAPISET_ClearCurRingType(MMISET_RING_TYPE_AUDIO_PLAYER);

    MMISRVAP_Stop(GetAPSrvHandle());

    MMIAP_SetAudioState(MMIMP3_AUDIO_STOPPED);
    MMIMP3_StopDelayPlayTimer();
    MMIAP_SetPlayerState(MMIMP3_PLAYER_STATE_NONE);


    MMIMP3_StopMainAnim(TRUE);
    MMIMP3_StopTitleAnim(TRUE);

#ifdef MMIAP_LYRIC_SUPPORT
    MMIMP3_StopLyric();
#endif

#ifdef MMIAP_SPECTRUM_SUPPORT
    MMIMP3_StopSpectrum(TRUE);  //需要在AUDIO close 之前，否则无意义
#endif
}

#ifdef MMIAP_F_RELEASE_BG_AUDIO
/*****************************************************************************/
//  Description : release audio handle
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIAP_ReleaseAudioHandle(
    void
)
{
    MMISRVAP_ERROR_E error_code = 0;
    BOOLEAN ret = FALSE;

    MMIMP3_PLAYING_DYN_INFO_T dyn_info = {0};
    MMISRV_HANDLE_T audio_handle = PNULL;

    SCI_TRACE_LOW("[MMIMP3] MMIAPIAP_ReleaseAudioHandle");

    /*非播放状态，释放audio handle
    *以方便其他声音播放，例如TTS,视频桌面
    */
    if (!MMIAPIMP3_IsPlayerPlaying()
            //&& !MMIAPIMP3_IsPlayerPaused()
            && !MMIMP3_IsPausedByService()
            && !MMK_IsActiveApplet(SPRD_MP3_APPLET_ID)
       )
    {      
        audio_handle = MMIAP_GetAPAudioSrvHandle();


        SCI_TRACE_LOW("[MMIAP] MMIAPIAP_ReleaseAudioHandle audio_handle=%x",
                     audio_handle);
        
        if (PNULL == audio_handle)
        {
            ret = TRUE;
        }
        else
        {
            SCI_TRACE_LOW("[MMIAP] MMIAPIAP_ReleaseAudioHandle start time=%d",
                          SCI_GetTickCount());

            MMIAPIAP_SetReleaseAudioHandleFlag(TRUE);

            //update playing info
            MMIMP3_GetPlayingDynInfo(&dyn_info, TRUE);
            //stop music
            StopPlayState();


            error_code = MMISRVAP_ReleaseAudioHandle(GetAPSrvHandle());
            SCI_TRACE_LOW("[MMIMP3] MMIAPIAP_ReleaseAudioHandle err");
            if (MMISRVAP_ERROR_NONE == error_code)
            {
                ret = TRUE;
            }

            SCI_TRACE_LOW("[MMIAP] MMIAPIAP_ReleaseAudioHandle end time=%d",
                          SCI_GetTickCount());
        }
    }

    SCI_TRACE_LOW("[MMIAP] MMIAPIAP_ReleaseAudioHandle ret=%d",\
                  ret);

    return ret;
}

/*****************************************************************************/
//  Description : request audio handle
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAP_RequestAudioHandle(
    void
)
{
    MMISRVAP_ERROR_E error_code = 0;
    BOOLEAN ret = FALSE;
    MMISRV_HANDLE_T audio_handle = PNULL;

    audio_handle = MMIAP_GetAPAudioSrvHandle();
    
    SCI_TRACE_LOW("[MMIMP3] MMIAP_RequestAudioHandle MMIAP_GetAPAudioSrvHandle = 0x%x", audio_handle);

    if (PNULL == audio_handle)
    {
        SCI_TRACE_LOW("[MMIAP] MMIAP_RequestAudioHandle start time=%d",
                      SCI_GetTickCount());

        error_code = MMISRVAP_RequestAudioHandle(GetAPSrvHandle());

        SCI_TRACE_LOW("[MMIMP3] MMISRVAP_RequestAudioHandle error_code = 0x%x", error_code);
        if (MMISRVAP_ERROR_NONE == error_code)
        {
            ret = TRUE;
        }

        SCI_TRACE_LOW("[MMIAP] MMIAP_RequestAudioHandle end time=%d",
                      SCI_GetTickCount());
    }

    SCI_TRACE_LOW("[MMIAP] MMIAP_RequestAudioHandle audio_handle=%X",\
                  audio_handle);

    SCI_TRACE_LOW("[MMIAP] MMIAP_RequestAudioHandle ret=%d",\
                  ret);

    return ret;
}

/*****************************************************************************/
//  Description : set release audio handle flag
//  Global resource dependence :
//  Author:robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIAP_SetReleaseAudioHandleFlag(
    BOOLEAN is_release
)
{
    MMIAP_PLAY_CTRL_INFO_T *info_ptr = GetApPlayCtrlInfoPtr();


    if (PNULL != info_ptr)
    {
        info_ptr->is_release_audio_handle = is_release;
    }
}

/*****************************************************************************/
//  Description : get release audio handle flag
//  Global resource dependence :
//  Author:robert.wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIAP_IsReleaseAudioHandle(
    void
)
{
    MMIAP_PLAY_CTRL_INFO_T *info_ptr = GetApPlayCtrlInfoPtr();
    BOOLEAN is_release = FALSE;


    if (PNULL != info_ptr)
    {
        is_release = info_ptr->is_release_audio_handle;
    }


    SCI_TRACE_LOW("[MMIAP] MMIAPIAP_IsReleaseAudioHandle is_release=%d",
                  is_release);

    return is_release;
}
#endif

/*****************************************************************************/
//  Description : get display flag
//  Global resource dependence :
//  Author:robert.wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIAP_IsDisplayForPlayStateNone(
    BOOLEAN is_over_offset_zero, //in
    BOOLEAN is_check_total_length //in
)
{
    BOOLEAN ret = FALSE;

#if defined(MMIAP_F_RELEASE_BG_AUDIO) || defined(MMIAP_F_HISTORY_PLAYLIST)

    MMIMP3_PLAYING_DYN_INFO_T dyn_info = {0};
    MMIMP3_PLAYER_STATE_E   player_state = MMIAPIMP3_GetPlayerState();




    if (
        MMIMP3_PLAYER_STATE_NONE == player_state
        &&
        (
#ifdef MMIAP_F_RELEASE_BG_AUDIO
            MMIAPIAP_IsReleaseAudioHandle()
#endif
#ifdef MMIAP_F_HISTORY_PLAYLIST
            || MMIAP_IsHistoryInit()
#endif
        )
    )
    {
        MMIMP3_GetPlayingDynInfo(&dyn_info, FALSE);

        do
        {
            if (is_over_offset_zero
                    && is_check_total_length
               )
            {

                if (dyn_info.cur_offset > 0
                        && dyn_info.total_length > 0
                   )
                {
                    ret = TRUE;
                }
                break;
            }

            if (is_check_total_length)
            {
                if (dyn_info.total_length > 0)
                {
                    ret = TRUE;
                }
                break;
            }

            if (is_over_offset_zero)
            {
                if (dyn_info.cur_offset > 0)
                {
                    ret = TRUE;
                }
                break;
            }

            ret = TRUE;
        }
        while(0);
    }

#endif

    SCI_TRACE_LOW("[MMIAP]MMIAPIAP_IsDisplayForPlayStateNone ret=%d",
                  ret);

    return ret;
}

/*****************************************************************************/
//  Description : get current play offset
//  Global resource dependence :
//  Author:robert.wang
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIAPIAP_GetCurPlayOffset(
    uint32 org_offset
)
{
    uint32 offset = 0;
    BOOLEAN is_get = FALSE;



    offset = org_offset;
    //first
#ifdef MMIAP_F_RELEASE_BG_AUDIO

    if (MMIAPIAP_IsReleaseAudioHandle())
    {
        is_get = TRUE;

        MMIAPIAP_SetReleaseAudioHandleFlag(FALSE);
    }
#endif

    //second
#ifdef MMIAP_F_HISTORY_PLAYLIST
    if(MMIAP_IsHistoryInit())
    {
        is_get = TRUE;
        MMIAP_SetHistoryInitFlag(FALSE);
    }

#endif

    if (is_get)
    {
        MMIMP3_PLAYING_DYN_INFO_T dyn_info = {0};


        MMIAPIMP3_GetPlayingDynInfo(&dyn_info, FALSE);
        offset = dyn_info.cur_offset;
    }


    SCI_TRACE_LOW("[MMIAP]MMIAPIAP_GetCurPlayOffset is_get=%d,offset=%d",
                  is_get, offset);

    return offset;
}
/*****************************************************************************/
//  Description : MMIMP3_SetReviewState
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIMP3_SetReviewState(BOOLEAN is_review_process_sucessful)
{
	s_review_process_sucessful =  is_review_process_sucessful;
}
