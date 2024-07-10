/*****************************************************************************
** File Name:      mmisrvap_fsm.c                                            *
** Author:                                                                   *
** Date:           11/07/2012                                                *
** Copyright:      2012 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to audio file srv fsm                   *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 07/2012       robert.wang       Create                                    *
******************************************************************************/

#define _MMISRVAP_FSM_C_
/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_common.h"
#include "mmk_timer.h"

//other module


//local module
#include "mmisrvap_iup.h"
#include "mmisrvap_idown.h"
#include "mmisrvap_fsm.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
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

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : 处理开始复读的回调
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL MMISRVAP_ERROR_E APHandleReviewStartCallback(
    MMISRVAP_FSM_T *fsm_ptr //in

);

/*****************************************************************************/
//  Description : 处理搜索请求
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL  MMISRVAP_ERROR_E APHandleSeekReq(
    MMISRVAP_FSM_T *fsm_ptr, //in
    MMISRVAP_FSM_MSG_T *msg_ptr //in
);

/*****************************************************************************/
//  Description : 处理event ex req
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL  MMISRVAP_ERROR_E APHandleEventExtReq(
    MMISRVAP_FSM_T *fsm_ptr, //in
    MMISRVAP_FSM_MSG_T *msg_ptr //in
);

/*****************************************************************************/
//  Description : 更新消息
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL void APUpdateMsgID(
    MMISRVAP_FSM_T *fsm_ptr, //in
    MMISRVAP_FSM_MSG_E msg_id //in
);

/*****************************************************************************/
//  Description :执行回调函数
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL void APDoCallerCallback(
    MMISRVAP_FSM_T *fsm_ptr //in
);

/**--------------------------------------------------------------------------*
 **                         LOCAL DEFINITION                                 *
 **--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                               *
**--------------------------------------------------------------------------*/


/*****************************************************************************/
//  Description : get period timer info pointer
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL MMISRVAP_PERIOD_TIMER_T* APGetPeriodTimerInfoPtr(
    MMISRVAP_FSM_T *fsm_ptr //in
)
{
    MMISRVAP_PERIOD_TIMER_T  *period_ptr = PNULL;

    if (PNULL != fsm_ptr)
    {
        period_ptr = &fsm_ptr->period_time_info;
    }

    return period_ptr;
}

/*****************************************************************************/
//  Description : get review info pointer
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC MMISRVAP_REVIEW_INFO_T* AP_GetReviewInfoPtr(
    MMISRVAP_FSM_T *fsm_ptr //in
)
{
    MMISRVAP_REVIEW_INFO_T  *info_ptr = PNULL;

    if (PNULL != fsm_ptr)
    {
        info_ptr = &fsm_ptr->play_info.review_info;
    }

    return info_ptr;
}

/*****************************************************************************/
//  Description : get play info pointer
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC MMISRVAP_PLAY_INFO_T* APGetPlayInfoPtr(
    MMISRVAP_FSM_T *fsm_ptr //in
)
{
    MMISRVAP_PLAY_INFO_T  *info_ptr = PNULL;

    if (PNULL != fsm_ptr)
    {
        info_ptr = &fsm_ptr->play_info;
    }

    return info_ptr;
}

/*****************************************************************************/
//  Description : 是否支持review
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN APIsSupportReview(
    MMISRVAP_FSM_T *fsm_ptr //in
)
{
    BOOLEAN is_support = FALSE;
    MMISRVAP_REVIEW_INFO_T *info_ptr = PNULL;


    if (PNULL == fsm_ptr)
    {
        return is_support;
    }

    info_ptr = AP_GetReviewInfoPtr(fsm_ptr);
    if (PNULL != info_ptr)
    {
        is_support = info_ptr->is_support_review;
    }

    SCI_TRACE_LOW("[MMISRVAP] APIsSupportReview is_support = %d", \
                  is_support);

    return is_support;
}

/*****************************************************************************/
//  Description : 是否支持timer
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN APIsSupportPeriodTimer(
    MMISRVAP_FSM_T *fsm_ptr //in
)
{
    BOOLEAN is_support = FALSE;
    MMISRVAP_PERIOD_TIMER_T *period_ptr = PNULL;


    if (PNULL == fsm_ptr)
    {
        return is_support;
    }

    period_ptr = APGetPeriodTimerInfoPtr(fsm_ptr);
    if (PNULL != period_ptr)
    {
        is_support = period_ptr->is_support_timer;
    }

    SCI_TRACE_LOW("[MMISRVAP] APIsSupportPeriodTimer is_support = %d", \
                  is_support);
    return is_support;
}

/*****************************************************************************/
//  Description : get ext info pointer
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL MMISRVAP_EXT_T* APGetExtInfoPtr(
    MMISRVAP_FSM_T *fsm_ptr //in
)
{
    MMISRVAP_EXT_T  *info_ptr = PNULL;

    if (PNULL != fsm_ptr)
    {
        info_ptr = &fsm_ptr->ext_info;
    }

    return info_ptr;
}

/*****************************************************************************/
//  Description : is paused by audio srv
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN APIsPausedByAudioSrv(
    MMISRVAP_FSM_T *fsm_ptr //in
)
{
    BOOLEAN  is_paused = FALSE;


    if (PNULL != fsm_ptr)
    {
        is_paused = fsm_ptr->is_paused_by_audsrv;
    }


    MMISRVAP_TRACE_LOW("[MMISRVAP] APIsPausedByAudioSrv is_paused=%d", \
                       is_paused);

    return is_paused;
}

/*****************************************************************************/
//  Description : set paused by audio srv flag
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL void APSetPausedByAudioSrvFlag(
    MMISRVAP_FSM_T *fsm_ptr, //in
    BOOLEAN is_paused_by_audsrv
)
{

    if (PNULL != fsm_ptr)
    {
        fsm_ptr->is_paused_by_audsrv = is_paused_by_audsrv;
    }


    MMISRVAP_TRACE_LOW("[MMISRVAP] APSetPausedByAudioSrvFlag is_paused_by_audsrv=%d", \
                       is_paused_by_audsrv);
}

/*****************************************************************************/
//  Description : 创建虚拟audio handle
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL MMISRV_HANDLE_T APRequestVirtualAudioHandle(
    MMISRVAP_FSM_T *fsm_ptr //in
)
{
    MMISRVMGR_SERVICE_REQ_T req = {0};
    MMISRVAUD_TYPE_T        audio_srv = {0};
    MMISRV_HANDLE_T audio_handle = PNULL;

    MMISRVAUD_ROUTE_T all_support_route = 0;
    BOOLEAN is_support_bt = FALSE;
    MMISRVAUD_VOLUME_T vol = 0;
    uint32 task_id = 0;

    if(PNULL == fsm_ptr)
    {
        return audio_handle;
    }

    is_support_bt = fsm_ptr->play_info.is_support_bt;
    vol = fsm_ptr->play_info.vol;
    task_id = SCI_IdentifyThread();
    fsm_ptr->task_id = task_id;


    MMISRVAP_TRACE_LOW("[MMISRVAP] APRequestVirtualAudioHandle task_id=%d", \
                       task_id);

#ifdef BT_DIALER_SUPPORT
    req.pri = MMISRVAUD_PRI_NORMAL_N10;
#else
    req.pri = MMISRVAUD_PRI_NORMAL;
#endif
    req.notify = AP_AudioSrvCallBack;
    req.ind_data = task_id;
    audio_srv.volume =  vol;
    audio_srv.volume_type = MMISRVAUD_VOLUME_TYPE_MED;
    audio_srv.info.type = MMISRVAUD_TYPE_VIRTUAL;
    audio_srv.play_times = 1;


    if(!is_support_bt)
    {
        all_support_route = MMISRVAUD_ROUTE_AUTO_NO_BLUETOOTH;
    }
    else
    {
        all_support_route = MMISRVAUD_ROUTE_AUTO;
    }

    audio_srv.all_support_route = all_support_route;
    audio_handle = MMISRVMGR_Request(STR_SRV_AUD_NAME, &req, &audio_srv);


    MMISRVAP_TRACE_LOW("[MMISRVAP] APRequestVirtualAudioHandle audio_handle=0x%X", \
                       audio_handle);

    return audio_handle;
}

/*****************************************************************************/
//  Description : request play to audio srv
//  Global resource dependence :
//  Author:robert.wang
//  Note:
/*****************************************************************************/
PUBLIC MMISRVAP_ERROR_E APReqPlayToAudioSrv(
    MMISRVAP_FSM_T *fsm_ptr //in
)
{
    MMISRVAP_ERROR_E error_code = MMISRVAP_ERROR_PARAM;

    MMISRVAUD_TYPE_U audio_info = {0};
    MMISRVAUD_RING_FMT_E audio_type = 0;
    MMISRVAUD_ROUTE_T all_support_route = 0;



    MMISRVAP_PLAY_INFO_T *play_info_ptr = PNULL;
    MMISRVAUD_RET_E audio_ret = 0;
    MMISRV_HANDLE_T audio_handle = PNULL;
    BOOLEAN is_support_bt = FALSE;
    wchar *file_name_ptr = PNULL;
    uint32 file_name_len = 0;
    uint32 offset_point = 0;
    MMISRVAP_PLAYING_STATE_E play_ret = 0;
    MMISRVAUD_VOLUME_T volume = 0;



    if(PNULL == fsm_ptr)
    {
        return error_code;
    }


    audio_handle = fsm_ptr->audiosrv_handle;
    play_info_ptr = APGetPlayInfoPtr(fsm_ptr);

    do
    {
        if (PNULL == play_info_ptr)
        {
            break;
        }

        is_support_bt = play_info_ptr->is_support_bt;
        file_name_ptr = play_info_ptr->full_path_ptr;
        file_name_len = play_info_ptr->full_path_len;
        volume = MMIAPISET_GetMultimVolume();//play_info_ptr->vol;

        if(PNULL == file_name_ptr
                || 0 == file_name_len
          )
        {
            break;
        }

        audio_type = play_info_ptr->audio_type;

        if(PNULL != audio_handle)
        {
            audio_info.type = MMISRVAUD_TYPE_RING_FILE;
            audio_info.ring_file.name = file_name_ptr; /*lint !e605*/
            audio_info.ring_file.name_len = file_name_len;
            audio_info.ring_file.fmt = audio_type;
            audio_info.ring_file.type = MMISRVAUD_TYPE_RING_FILE;

            if(!MMISRVAUD_SetAudioInfo(audio_handle, &audio_info))
            {
                MMISRVAUD_Stop(audio_handle);
                error_code = MMISRVAP_ERROR_REQ;
                break;
            }

            if(!is_support_bt)
            {
                all_support_route = MMISRVAUD_ROUTE_AUTO_NO_BLUETOOTH;
            }
            else
            {
                all_support_route = MMISRVAUD_ROUTE_AUTO;
            }
            MMISRVAUD_SetAllSupportRoute(audio_handle, all_support_route);
            MMISRVAUD_SetVolume(audio_handle, volume);

        }
        else
        {
            MMISRVMGR_SERVICE_REQ_T req = {0};
            MMISRVAUD_TYPE_T audio_srv = {0};
            uint32 task_id = 0;


            task_id = fsm_ptr->task_id;

            MMISRVAP_TRACE_LOW("[MMISRVAP] APReqPlayToAudioSrv task_id=%d", \
                               task_id);

#ifdef BT_DIALER_SUPPORT
            req.pri = MMISRVAUD_PRI_NORMAL_N10;
#else
            req.pri = MMISRVAUD_PRI_NORMAL;
#endif
            req.notify = AP_AudioSrvCallBack;
            req.ind_data = task_id;


            audio_srv.info.ring_file.type = MMISRVAUD_TYPE_RING_FILE;
            audio_srv.volume = volume;
            audio_srv.volume_type = MMISRVAUD_VOLUME_TYPE_MED;
            audio_srv.info.ring_file.name = file_name_ptr; /*lint !e605*/
            audio_srv.info.ring_file.name_len = file_name_len;
            audio_srv.info.ring_file.fmt = audio_type;
            audio_srv.play_times = 1;

            if(!is_support_bt)
            {
                all_support_route = MMISRVAUD_ROUTE_AUTO_NO_BLUETOOTH;
            }
            else
            {
                all_support_route = MMISRVAUD_ROUTE_AUTO;
            }

            audio_srv.all_support_route = all_support_route;

            audio_handle = MMISRVMGR_Request(STR_SRV_AUD_NAME, &req, &audio_srv);

            if(PNULL == audio_handle)
            {
                error_code = MMISRVAP_ERROR_REQ;
                break;
            }

            fsm_ptr->audiosrv_handle = audio_handle;

            MMISRVAP_TRACE_LOW("[MMISRVAP] APReqPlayToAudioSrv audio_handle=0x%X", \
                               audio_handle);


            //将audio handle传给caller.
            {
                MMISRVAP_FSM_MSG_E msg_id = 0;


                msg_id = fsm_ptr->msg_id;

                APUpdateMsgID(fsm_ptr, MMISRVAP_REQUEST_AUDIO_HANDLE_REQ);
                AP_SetErrorCode(fsm_ptr, MMISRVAP_ERROR_NONE);
                APDoCallerCallback(fsm_ptr);

                fsm_ptr->msg_id = msg_id;
            }
        }


        offset_point = play_info_ptr->offset_point;
        audio_ret = MMISRVAUD_Play(audio_handle, offset_point);

        if((MMISRVAUD_RET_OK == audio_ret)
                || (MMISRVAUD_RET_PENDING == audio_ret)
          )
        {
            error_code = MMISRVAP_ERROR_NONE;

            switch(audio_ret)
            {
            case MMISRVAUD_RET_OK:
                play_ret = MMISRVAP_PLAYING_OK;
                break;
            case MMISRVAUD_RET_PENDING:
                play_ret = MMISRVAP_PLAYING_PENDING;
                break;
            default:
                break;
            }

            play_info_ptr->play_ret = play_ret;
        }
        else
        {
            error_code = MMISRVAP_ERROR_REQ;
        }
    }
    while(0);


    MMISRVAP_TRACE_LOW("[MMISRVAP] APReqPlayToAudioSrv error_code=%d", \
                       error_code);

    return error_code;
}

/*****************************************************************************/
//  Description : 更新消息
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL void APUpdateMsgID(
    MMISRVAP_FSM_T *fsm_ptr, //in
    MMISRVAP_FSM_MSG_E msg_id //in
)
{
    if(PNULL != fsm_ptr)
    {
        fsm_ptr->msg_id = msg_id;
    }
}

/*****************************************************************************/
//  Description : 初始化状态机信息
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL void APInitReqInfo(
    MMISRVAP_FSM_T *fsm_ptr, //in
    MMISRVAP_FSM_MSG_T *msg_ptr //in
)
{

    if(PNULL == fsm_ptr
            || PNULL == msg_ptr
      )
    {
        return;
    }

    MMISRVAP_TRACE_LOW("[MMISRVAP] APInitReqInfo msg_id=%d", \
                       msg_ptr->msg_id);

    switch(msg_ptr->msg_id)
    {
    case MMISRVAP_PLAY_REQ:
        AP_StopPeriodTimer(fsm_ptr);

        if(PNULL != fsm_ptr->play_info.full_path_ptr)
        {
            SCI_FREE(fsm_ptr->play_info.full_path_ptr);
            fsm_ptr->play_info.full_path_ptr = PNULL;
        }

        //初始化play info
        SCI_MEMSET(&fsm_ptr->play_info, 0, sizeof(fsm_ptr->play_info));


        SCI_MEMSET(&fsm_ptr->period_time_info, 0, sizeof(fsm_ptr->period_time_info));
        break;

    default:
        break;
    }
}

/*****************************************************************************/
//  Description : 更新播放信息
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL MMISRVAP_ERROR_E APUpdatePlayInfo(
    MMISRVAP_FSM_T *fsm_ptr, //in
    MMISRVAP_FSM_MSG_T *msg_ptr //in
)
{
    MMISRVAP_ERROR_E error_code = MMISRVAP_ERROR_PARAM;
    MMISRVAP_REQ_PLAY_PARAM_T *play_param_ptr = PNULL;
    uint32 size_of_full_path = 0;


    MMISRVAP_PLAY_INFO_T *play_info_ptr = PNULL;


    if(PNULL == fsm_ptr
            || PNULL == msg_ptr
      )
    {
        return error_code;
    }



    do
    {
        //反序列化
        play_param_ptr = (MMISRVAP_REQ_PLAY_PARAM_T*)msg_ptr->msg_body;
        play_info_ptr = APGetPlayInfoPtr(fsm_ptr);

        if(PNULL == play_param_ptr
                || PNULL == play_param_ptr->full_path_ptr
                || 0 == play_param_ptr->full_path_len
                || PNULL == play_info_ptr)
        {
            break;
        }

        //文件名称
        play_param_ptr->full_path_ptr = (wchar *)((uint32)msg_ptr->msg_body + sizeof(MMISRVAP_REQ_PLAY_PARAM_T));
        play_param_ptr->full_path_ptr = (wchar *)((uint32)(play_param_ptr->full_path_ptr) \
                                        + MMIAPICOM_GetAlign4Offset((uint32)(play_param_ptr->full_path_ptr)));


        size_of_full_path = (play_param_ptr->full_path_len + 1) * sizeof(wchar);


        play_info_ptr->full_path_ptr = (wchar *)SCI_ALLOCA(size_of_full_path);

        if(PNULL == play_info_ptr->full_path_ptr)
        {
            error_code = MMISRVAP_ERROR_NOT_ENOUGH_MEMORY;
            break;
        }


        SCI_MEMSET(play_info_ptr->full_path_ptr, 0, size_of_full_path);
        MMIAPICOM_Wstrncpy(play_info_ptr->full_path_ptr, play_param_ptr->full_path_ptr, play_param_ptr->full_path_len);
        play_info_ptr->full_path_len = play_param_ptr->full_path_len;


        //音量,BT
        play_info_ptr->vol = play_param_ptr->vol;
        play_info_ptr->is_support_bt = play_param_ptr->is_support_bt;
        play_info_ptr->is_enable_seek = TRUE;
        play_info_ptr->audio_type = play_param_ptr->audio_type;

        //period timer
        if (0 != play_param_ptr->period_time)
        {
            MMISRVAP_PERIOD_TIMER_T *period_ptr = PNULL;

            period_ptr = APGetPeriodTimerInfoPtr(fsm_ptr);
            if (PNULL != period_ptr)
            {
                period_ptr->is_support_timer = TRUE;
                period_ptr->period_time = play_param_ptr->period_time;

                AP_StartPeriodTimer(fsm_ptr);
            }
        }
        //offset point
        play_info_ptr->offset_point = play_param_ptr->offset_point;

        //review info
        if (0 != play_param_ptr->review_param.start_point)
        {
            MMISRVAP_REVIEW_INFO_T *review_ptr = PNULL;

            review_ptr = AP_GetReviewInfoPtr(fsm_ptr);
            if (PNULL != review_ptr)
            {
                review_ptr->is_support_review = TRUE;
                review_ptr->start_point = play_param_ptr->review_param.start_point;
                review_ptr->end_point = play_param_ptr->review_param.end_point;
            }
        }

        error_code = MMISRVAP_ERROR_NONE;
    }
    while(0);


    MMISRVAP_TRACE_LOW("[MMISRVAP] APUpdatePlayInfo error_code=%d", \
                       error_code);

    return error_code;
}

/*****************************************************************************/
//  Description : 更新状态机信息
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL MMISRVAP_ERROR_E APUpdateInfo(
    MMISRVAP_FSM_T *fsm_ptr, //in
    MMISRVAP_FSM_MSG_T *msg_ptr //in
)
{
    MMISRVAP_ERROR_E error_code = MMISRVAP_ERROR_PARAM;

    MMISRVAP_FSM_MSG_E msg_id = 0;


    if(PNULL == fsm_ptr
            || PNULL == msg_ptr
      )
    {
        return error_code;
    }

    msg_id = msg_ptr->msg_id;
    APUpdateMsgID(fsm_ptr, msg_id);

    MMISRVAP_TRACE_LOW("MMISRVAP APUpdateInfo msg_id=%d", \
                       msg_id);

    switch(msg_id)
    {
    case MMISRVAP_PLAY_REQ:
        error_code = APUpdatePlayInfo(fsm_ptr, msg_ptr);
        break;

    default:
        error_code = MMISRVAP_ERROR_NONE;
        break;
    }


    MMISRVAP_TRACE_LOW("MMISRVAP APUpdateInfo error_code=%d", \
                       error_code);

    return error_code;
}

/*****************************************************************************/
//  Description : 处理播放请求
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL MMISRVAP_ERROR_E APHandlePlayReq(
    MMISRVAP_FSM_T *fsm_ptr //in
)
{
    MMISRVAP_ERROR_E error_code = MMISRVAP_ERROR_PARAM;


    if(PNULL == fsm_ptr)
    {
        return error_code;
    }


    error_code = APReqPlayToAudioSrv(fsm_ptr);

    MMISRVAP_TRACE_LOW("[MMISRVAP] HanldePlay error_code=%d",
                       error_code);

    return error_code;
}

/*****************************************************************************/
//  Description :设置回调标记
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL void APSetCallerCallbackFlag(
    BOOLEAN is_do, //in
    MMISRVAP_FSM_T *fsm_ptr //in
)
{
    if(PNULL != fsm_ptr)
    {
        fsm_ptr->is_do_caller_callback = is_do;
    }


    MMISRVAP_TRACE_LOW("[MMISRVAP] APSetCallerCallbackFlag is_do=%d", \
                       is_do);
}

/*****************************************************************************/
//  Description :获得回调标记
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN APGetCallerCallbackFlag(
    MMISRVAP_FSM_T *fsm_ptr //in
)
{
    BOOLEAN is_do = FALSE;


    if(PNULL != fsm_ptr)
    {
        is_do = fsm_ptr->is_do_caller_callback;
    }


    MMISRVAP_TRACE_LOW("[MMISRVAP] APGetCallerCallbackFlag is_do=%d", \
                       is_do);

    return is_do;
}

/*****************************************************************************/
//  Description : 根据msg id获得event id
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL MMISRVAP_EVENT_E ConvertMsgIDToEventID(
    MMISRVAP_FSM_MSG_E msg_id //in
)
{
    MMISRVAP_EVENT_COMBO_T array[]=
    {
        {FSMSRV_INIT_REQ, MMISRVAP_EVENT_OPEN},
        {MMISRVAP_PLAY_REQ, MMISRVAP_EVENT_PLAYING},
        {MMISRVAP_EVENT_PLAY_END_REQ, MMISRVAP_EVENT_PLAY_END},
        {MMISRVAP_PAUSE_REQ, MMISRVAP_EVENT_PAUSE},
        {MMISRVAP_RESUME_REQ, MMISRVAP_EVENT_RESUME},
        {MMISRVAP_SEEK_REQ, MMISRVAP_EVENT_SEEK},
        {MMISRVAP_STOP_REQ, MMISRVAP_EVENT_STOP},
        {MMISRVAP_MGR_RESUME_REQ, MMISRVAP_EVENT_MGR_RESUME},
        {MMISRVAP_EVENT_PAUSE_REQ, MMISRVAP_EVENT_TO_DO_PAUSE},
        {MMISRVAP_EVENT_RESUME_REQ, MMISRVAP_EVENT_TO_DO_RESUME},
        {MMISRVAP_EVENT_EXT_REQ, MMISRVAP_EVENT_EXT},
#ifdef BT_DIALER_SUPPORT
        {MMISRVAP_EVENT_EARPHONE_REQ, MMISRVAP_EVENT_EARPHONE},
#endif
        {MMISRVAP_REVIEW_START_REQ, MMISRVAP_EVENT_REVIEW_START},
        {MMISRVAP_REVIEW_STOP_REQ, MMISRVAP_EVENT_REVIEW_STOP},
        {MMISRVAP_EVENT_REVIEW_PLAY_END_REQ, MMISRVAP_EVENT_REVIEW_PLAY_END},
        {MMISRVAP_RELEASE_AUDIO_HANDLE_REQ, MMISRVAP_EVENT_UPDATE_AUDIO_HANDLE},
        {MMISRVAP_REQUEST_AUDIO_HANDLE_REQ, MMISRVAP_EVENT_UPDATE_AUDIO_HANDLE}

    };
    uint32 i = 0;
    uint32 total_num = ARR_SIZE(array);
    MMISRVAP_EVENT_E event_id = MMISRVAP_EVENT_MAX;

    for (i = 0; i < total_num; i++)
    {
        if (msg_id == array[i].msg_id)
        {
            event_id = array[i].event_id;
            break;
        }
    }

    MMISRVAP_TRACE_LOW("[MMISRVAP] ConvertMsgIDToEventID event_id=%d", \
                       event_id);
    return event_id;
}
/*****************************************************************************/
//  Description : 获得回调的缺省参数
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL void APGetDefaultCallbackParam(
    MMISRVAP_FSM_T *fsm_ptr, //in
    MMISRVAP_CALLBACK_PARAM_T *param_ptr //in/out
)
{

    if(PNULL == fsm_ptr
            || PNULL == param_ptr
      )
    {
        return;
    }


    param_ptr->handle = fsm_ptr->ap_handle;
    param_ptr->event_id = ConvertMsgIDToEventID(fsm_ptr->msg_id);
    param_ptr->error_code = AP_GetErrorCode(fsm_ptr);


    MMISRVAP_TRACE_LOW("[MMISRVAP] APGetDefaultCallbackParam event_id=%d", \
                       param_ptr->event_id);
}

/*****************************************************************************/
//  Description :执行回调函数
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL void APDoCallerCallback(
    MMISRVAP_FSM_T *fsm_ptr //in
)
{
    MMISRVAP_CALLBACK_PARAM_T callback_param = {0};
    MMISRV_HANDLE_T audio_handle  = PNULL;
    MMISRVAP_CALLBACK_REVIEW_PLAY_END_PARAM_T review_play_end_param = {0};
    MMISRVAP_CALLBACK_BT_PARAM_T bt_param = {0};
    MMISRVAP_CALLBACK_PLAYING_PARAM_T  play_param = {0};



    if(PNULL == fsm_ptr
      )
    {
        return;
    }

    MMISRVAP_TRACE_LOW("[MMISRVAP] APDoCallerCallback msg_id=%d", \
                       fsm_ptr->msg_id);

    switch(fsm_ptr->msg_id)
    {
    case FSMSRV_INIT_REQ: /*lint !e408*/
    case MMISRVAP_RELEASE_AUDIO_HANDLE_REQ:
    case MMISRVAP_REQUEST_AUDIO_HANDLE_REQ:
        audio_handle = fsm_ptr->audiosrv_handle;

        callback_param.param_ptr = (void*)&audio_handle;
        callback_param.size_of_param = sizeof(audio_handle);
        break;
    case MMISRVAP_PLAY_REQ:
    {
        MMISRVAP_PLAY_INFO_T *play_info_ptr = PNULL;


        play_info_ptr = APGetPlayInfoPtr(fsm_ptr);
        if (PNULL != play_info_ptr)
        {
            play_param.play_ret = play_info_ptr->play_ret;
        }

        callback_param.param_ptr = (void*)&play_param;
        callback_param.size_of_param = sizeof(play_param);
    }
    break;
    case MMISRVAP_EVENT_REVIEW_PLAY_END_REQ:
    {
        MMISRVAP_REVIEW_INFO_T *review_info_ptr = PNULL;


        review_info_ptr = AP_GetReviewInfoPtr(fsm_ptr);
        if (PNULL != review_info_ptr)
        {
            review_play_end_param.is_end_file = review_info_ptr->is_end_file;
            review_play_end_param.total_length = review_info_ptr->end_point;
            review_play_end_param.total_time = review_info_ptr->total_time;
        }

        callback_param.param_ptr = (void*)&review_play_end_param;
        callback_param.size_of_param = sizeof(review_play_end_param);
    }
    break;
    case MMISRVAP_EVENT_EXT_REQ:
    {
        MMISRVAP_EXT_T *ext_ptr = PNULL;


        ext_ptr = APGetExtInfoPtr(fsm_ptr);
        if (PNULL != ext_ptr)
        {
            bt_param.bt_event = ext_ptr->data;
        }
        callback_param.param_ptr = (void*)&bt_param;
        callback_param.size_of_param = sizeof(bt_param);
    }
    break;
    default:
        break;
    }

    APGetDefaultCallbackParam(fsm_ptr, &callback_param);


    //do callback
    if(PNULL != fsm_ptr->caller_callback)
    {
        fsm_ptr->caller_callback(&callback_param, sizeof(callback_param));
    }

}

/*****************************************************************************/
//  Description :调用回调函数
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL void APNotifyCaller(
    MMISRVAP_FSM_T *fsm_ptr //in
)
{
    BOOLEAN is_do_callback = FALSE;


    if(PNULL == fsm_ptr)
    {
        return;
    }

    is_do_callback = APGetCallerCallbackFlag(fsm_ptr);


    if(is_do_callback)
    {
        APSetCallerCallbackFlag(FALSE, fsm_ptr);

        APDoCallerCallback(fsm_ptr);
    }

    MMISRVAP_TRACE_LOW("[MMISRVAP] APNotifyCaller is_do_callback=%d", \
                       is_do_callback);
}

/*****************************************************************************/
//  Description : 检查复读
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL MMISRVAP_ERROR_E APCheckNotEndPointReview(
    MMISRVAP_FSM_T *fsm_ptr //in

)
{
    MMISRVAP_ERROR_E error_code = MMISRVAP_ERROR_PARAM;
    MMISRVAP_REVIEW_INFO_T *review_ptr = PNULL;
    uint32 cur_offset = 0;
    BOOLEAN ret = FALSE;
    MMISRVAUD_PLAY_INFO_T play_info = {0};


    do
    {
        if (PNULL == fsm_ptr)
        {
            break;
        }

        review_ptr = AP_GetReviewInfoPtr(fsm_ptr);


        //如果支持复读
        if (PNULL != review_ptr
                &&   review_ptr->is_support_review)
        {
            ret = MMISRVAUD_GetPlayingInfo(fsm_ptr->audiosrv_handle, &play_info);
            if (!ret)
            {
                error_code = MMISRVAP_ERROR_MAX;
                break;
            }
            cur_offset = play_info.cur_data_offset;

            //播放位置大于停止位置，并且不是播放结束
            if(
                cur_offset >= review_ptr->end_point
                && 0 != review_ptr->end_point
                && !review_ptr->is_end_file
                && MMISRVAP_STATE_PLAYING == fsm_ptr->state
            )
            {
                error_code = APHandleReviewStartCallback(fsm_ptr);
            }
        }
        else
        {
            error_code = MMISRVAP_ERROR_MAX;
        }
    }
    while(0);

    MMISRVAP_TRACE_LOW("[MMISRVAP] APCheckReview error_code=%d", \
                       error_code);

    return error_code;
}

/*****************************************************************************/
//  Description : 处理开始复读的回调
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL MMISRVAP_ERROR_E APHandleReviewStartCallback(
    MMISRVAP_FSM_T *fsm_ptr //in

)
{
    MMISRVAP_ERROR_E error_code = MMISRVAP_ERROR_PARAM;
    MMISRVAP_REVIEW_INFO_T *review_ptr = PNULL;
    MMISRVAP_FSM_MSG_T msg_info = {0};

    MMISRVAP_REQ_SEEK_PARAM_T seek_info = {0};

    BOOLEAN is_do_callback = FALSE;

    do
    {
        if (PNULL == fsm_ptr)
        {
            break;
        }

        review_ptr = AP_GetReviewInfoPtr(fsm_ptr);


        //如果支持复读
        if (PNULL != review_ptr
                &&   review_ptr->is_support_review)
        {
            is_do_callback = TRUE;

            //先停止timer
            AP_StopPeriodTimer(fsm_ptr);

            //seek to start point
            seek_info.offset = review_ptr->start_point;

            msg_info.msg_id = MMISRVAP_SEEK_REQ;
            msg_info.msg_size = sizeof(msg_info);
            msg_info.msg_body = &seek_info;
            error_code = APHandleSeekReq(fsm_ptr, &msg_info);
            if (MMISRVAP_ERROR_NONE != error_code)
            {
                break;
            }

            error_code = MMISRVAP_ERROR_NONE;

        }
    }
    while(0);


    if (is_do_callback)
    {
        APUpdateMsgID(fsm_ptr, MMISRVAP_REVIEW_START_REQ);

        AP_SetErrorCode(fsm_ptr, error_code);
        APSetCallerCallbackFlag(TRUE, fsm_ptr);
        APNotifyCaller(fsm_ptr);
    }
    else
    {
        error_code = MMISRVAP_ERROR_MAX;
    }

    MMISRVAP_TRACE_LOW("[MMISRVAP] HandleReview error_code=%d", \
                       error_code);

    return error_code;
}

/*****************************************************************************/
//  Description : 检查周期timer
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void APCheckPeriodTimer(
    MMISRVAP_FSM_T *fsm_ptr //in
)
{
    BOOLEAN is_start = FALSE;
    BOOLEAN is_support_timer = FALSE;
    BOOLEAN is_support_review = FALSE;

    if (PNULL == fsm_ptr)
    {
        return;
    }

    do
    {
        //check timer srv
        is_support_timer = APIsSupportPeriodTimer(fsm_ptr);
        if (is_support_timer)
        {
            is_start = TRUE;
            break;
        }

        //check review

        is_support_review = APIsSupportReview(fsm_ptr);
        if (is_support_review)
        {
            is_start = TRUE;
        }

    }
    while(0);

    if (is_start)
    {
        AP_StartPeriodTimer(fsm_ptr);
    }
}

/*****************************************************************************/
//  Description : 处理request audio handle
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL  MMISRVAP_ERROR_E APHandleRequestAudioHandleReq(
    MMISRVAP_FSM_T *fsm_ptr, //in
    MMISRVAP_FSM_MSG_T *msg_ptr //in
)
{
    MMISRVAP_ERROR_E error_code = MMISRVAP_ERROR_PARAM;


    if(PNULL == fsm_ptr
            || PNULL == msg_ptr
      )
    {
        return error_code;
    }

    do
    {
        //更新msg ID
        APUpdateMsgID(fsm_ptr, msg_ptr->msg_id);

        //创建虚拟audio handle
        fsm_ptr->audiosrv_handle = APRequestVirtualAudioHandle(fsm_ptr);

        error_code = MMISRVAP_ERROR_NONE;
    }
    while(0);


    AP_SetErrorCode(fsm_ptr, error_code);
    APSetCallerCallbackFlag(TRUE, fsm_ptr);


    SCI_TRACE_LOW("[MMISRVAP] APHandleRequestAudioHandleReq error_code = %d", \
                  error_code);

    return error_code;
}

/*****************************************************************************/
//  Description : 空闲状态的处理
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN APIdle(
    MMISRVAP_HANDLE ap_handle, //in
    MMISRVAP_FSM_MSG_T *msg_ptr //in
)
{
    BOOLEAN    result = TRUE;
    MMISRVAP_FSM_T *fsm_ptr = (MMISRVAP_FSM_T*)FSMSRV_GetObject((FSMSRV_HANDLE)ap_handle);


    //参数检查
    if(PNULL == fsm_ptr
            || PNULL == msg_ptr
      )
    {
        return FALSE;
    }

    SCI_TRACE_LOW("[MMISRVAP] APIdle msg_id = %d", \
                  msg_ptr->msg_id);


    APSetCallerCallbackFlag(FALSE, fsm_ptr);
    switch(msg_ptr->msg_id)
    {
    case FSMSRV_INIT_REQ:
        //更新信息
        fsm_ptr->ap_handle = ap_handle;

        AP_FsmTransState(fsm_ptr, MMISRVAP_STATE_READY);

        APHandleRequestAudioHandleReq(fsm_ptr, msg_ptr);
        break;

    case MMISRVAP_RELEASE_REQ:
        break;

    default:
        result =  FALSE;
        break;
    }


    APNotifyCaller(fsm_ptr);

    return result;
}

/*****************************************************************************/
//  Description : 处理release audio handle
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL  MMISRVAP_ERROR_E APHandleReleaseAudioHandleReq(
    MMISRVAP_FSM_T *fsm_ptr, //in
    MMISRVAP_FSM_MSG_T *msg_ptr //in
)
{
    MMISRVAP_ERROR_E error_code = MMISRVAP_ERROR_PARAM;


    if(PNULL == fsm_ptr
            || PNULL == msg_ptr
      )
    {
        return error_code;
    }

    do
    {
        //更新msg ID
        APUpdateMsgID(fsm_ptr, msg_ptr->msg_id);

        if (PNULL != fsm_ptr->audiosrv_handle)
        {
            MMISRVMGR_Free(fsm_ptr->audiosrv_handle);
            fsm_ptr->audiosrv_handle = PNULL;
        }

        error_code = MMISRVAP_ERROR_NONE;
    }
    while(0);


    AP_SetErrorCode(fsm_ptr, error_code);
    APSetCallerCallbackFlag(TRUE, fsm_ptr);


    SCI_TRACE_LOW("[MMISRVAP] APHandleReleaseAudioHandleReq error_code = %d", \
                  error_code);

    return error_code;
}


/*****************************************************************************/
//  Description : ready状态的处理
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN APReady(
    MMISRVAP_HANDLE ap_handle, //in
    MMISRVAP_FSM_MSG_T *msg_ptr //in
)
{
    BOOLEAN    result = TRUE;
    MMISRVAP_ERROR_E error_code = MMISRVAP_ERROR_NONE;
    MMISRVAP_FSM_T *fsm_ptr = (MMISRVAP_FSM_T*)FSMSRV_GetObject((FSMSRV_HANDLE)ap_handle);


    //参数检查
    if(PNULL == msg_ptr
            || PNULL == fsm_ptr
      )
    {
        return FALSE;
    }

    SCI_TRACE_LOW("[MMISRVAP] AP_FsmReady msg_ptr->msg_id = %d", \
                  msg_ptr->msg_id);


    APSetCallerCallbackFlag(FALSE, fsm_ptr);

    switch(msg_ptr->msg_id)
    {
    case MMISRVAP_PLAY_REQ:
        APInitReqInfo(fsm_ptr, msg_ptr);

        error_code = APUpdateInfo(fsm_ptr, msg_ptr);

        if(MMISRVAP_ERROR_NONE == error_code)
        {
            //播放本地文件
            error_code = APHandlePlayReq(fsm_ptr);

            if(MMISRVAP_ERROR_NONE == error_code)
            {
                AP_FsmTransState(fsm_ptr, MMISRVAP_STATE_PLAYING);
            }
        }

        APSetCallerCallbackFlag(TRUE, fsm_ptr);
        AP_SetErrorCode(fsm_ptr, error_code);
        break;

    case MMISRVAP_RELEASE_REQ:
        break;

    case MMISRVAP_EVENT_EXT_REQ:
#ifdef BT_DIALER_SUPPORT
    case MMISRVAP_EVENT_EARPHONE_REQ:
#endif
        APHandleEventExtReq(fsm_ptr, msg_ptr);
        break;
    case MMISRVAP_RELEASE_AUDIO_HANDLE_REQ:
        APHandleReleaseAudioHandleReq(fsm_ptr, msg_ptr);
        break;
    case MMISRVAP_REQUEST_AUDIO_HANDLE_REQ:
        APHandleRequestAudioHandleReq(fsm_ptr, msg_ptr);
        break;
    default:
        result =  FALSE;
        break;
    }


    APNotifyCaller(fsm_ptr);

    return result;
}

/*****************************************************************************/
//  Description : 处理停止请求
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL  MMISRVAP_ERROR_E APHandleStopReq(
    MMISRVAP_FSM_T *fsm_ptr, //in
    MMISRVAP_FSM_MSG_T *msg_ptr //in

)
{
    MMISRVAP_ERROR_E error_code = MMISRVAP_ERROR_PARAM;

    BOOLEAN result = FALSE;

    if(PNULL == fsm_ptr
            || PNULL == msg_ptr
      )
    {
        return error_code;
    }


    //先停止timer
    AP_StopPeriodTimer(fsm_ptr);

    //更新msg ID
    APUpdateMsgID(fsm_ptr, msg_ptr->msg_id);

    result = MMISRVAUD_Stop(fsm_ptr->audiosrv_handle);

    if(result)
    {
        error_code = MMISRVAP_ERROR_NONE;
    }
    else
    {
        error_code = MMISRVAP_ERROR_REQ;
    }

    AP_FsmTransState(fsm_ptr, MMISRVAP_STATE_READY);

    AP_SetErrorCode(fsm_ptr, error_code);
    APSetCallerCallbackFlag(TRUE, fsm_ptr);

    SCI_TRACE_LOW("[MMISRVAP] APHandleStopReq error_code = %d", \
                  error_code);

    return error_code;
}

/*****************************************************************************/
//  Description : 处理搜索请求
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL  MMISRVAP_ERROR_E APHandleSeekReq(
    MMISRVAP_FSM_T *fsm_ptr, //in
    MMISRVAP_FSM_MSG_T *msg_ptr //in
)
{
    MMISRVAP_ERROR_E error_code = MMISRVAP_ERROR_PARAM;
    MMISRVAP_REQ_SEEK_PARAM_T *seek_ptr = PNULL;

    BOOLEAN result = FALSE;


    if(PNULL == fsm_ptr
            || PNULL == msg_ptr
            || PNULL == msg_ptr->msg_body
      )
    {
        return error_code;
    }

    //先停止tiemr
    AP_StopPeriodTimer(fsm_ptr);

    //更新msg ID
    APUpdateMsgID(fsm_ptr, msg_ptr->msg_id);

    seek_ptr = (MMISRVAP_REQ_SEEK_PARAM_T*)msg_ptr->msg_body;

    result = MMISRVAUD_Seek(fsm_ptr->audiosrv_handle, seek_ptr->offset);

    if(result)
    {
        error_code = MMISRVAP_ERROR_NONE;
    }
    else
    {
        error_code = MMISRVAP_ERROR_REQ;
    }

    AP_SetErrorCode(fsm_ptr, error_code);
    APSetCallerCallbackFlag(TRUE, fsm_ptr);


    APCheckPeriodTimer(fsm_ptr);


    SCI_TRACE_LOW("[MMISRVAP] APHandleSeekReq error_code = %d", \
                  error_code);

    return error_code;
}

/*****************************************************************************/
//  Description : 检查播放结束时复读的状态
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL MMISRVAP_ERROR_E APCheckPlayEndReview(
    MMISRVAP_FSM_T *fsm_ptr //in

)
{
    MMISRVAP_ERROR_E error_code = MMISRVAP_ERROR_MAX;
    MMISRVAP_REVIEW_INFO_T *review_ptr = PNULL;

    MMISRVAUD_CONTENT_INFO_T file_info = {0};
    BOOLEAN ret = FALSE;
    BOOLEAN is_do_review  = FALSE;


    do
    {
        if (PNULL == fsm_ptr)
        {
            break;
        }

        review_ptr = AP_GetReviewInfoPtr(fsm_ptr);


        //如果支持复读
        if (PNULL != review_ptr
                &&   review_ptr->is_support_review)
        {
            ret = MMISRVAUD_GetContentInfo(fsm_ptr->audiosrv_handle, &file_info);
            if (!ret)
            {
                break;
            }
            review_ptr->is_end_file = TRUE;
            review_ptr->end_point = file_info.total_data_length;
            review_ptr->total_time = file_info.total_time;

            //执行review play end callback
            APUpdateMsgID(fsm_ptr, MMISRVAP_EVENT_REVIEW_PLAY_END_REQ);
            error_code = MMISRVAP_ERROR_NONE;
            AP_SetErrorCode(fsm_ptr, error_code);
            APSetCallerCallbackFlag(TRUE, fsm_ptr);
            APNotifyCaller(fsm_ptr);


            //开始复读，并执行review start callback
            fsm_ptr->play_info.offset_point = review_ptr->start_point;

            error_code = APReqPlayToAudioSrv(fsm_ptr);
            if (MMISRVAP_ERROR_NONE != error_code)
            {
                break;
            }

            //启动timer
            AP_StartPeriodTimer(fsm_ptr);

            APUpdateMsgID(fsm_ptr, MMISRVAP_REVIEW_START_REQ);
            AP_SetErrorCode(fsm_ptr, error_code);
            APSetCallerCallbackFlag(TRUE, fsm_ptr);
            APNotifyCaller(fsm_ptr);

            is_do_review = TRUE;
        }
    }
    while(0);


    if (!is_do_review)
    {
        error_code = MMISRVAP_ERROR_MAX;
    }


    MMISRVAP_TRACE_LOW("[MMISRVAP] APCheckPlayEndReview is_do_review=%d", \
                       is_do_review);
    MMISRVAP_TRACE_LOW("[MMISRVAP] APCheckPlayEndReview error_code=%d", \
                       error_code);

    return error_code;
}

/*****************************************************************************/
//  Description : 处理播放结束请求
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL  MMISRVAP_ERROR_E APHandlePlayEndReq(
    MMISRVAP_FSM_T *fsm_ptr, //in
    MMISRVAP_FSM_MSG_T *msg_ptr //in
)
{
    MMISRVAP_ERROR_E error_code = MMISRVAP_ERROR_PARAM;



    if(PNULL == fsm_ptr
            || PNULL == msg_ptr
      )
    {
        return error_code;
    }


    //先停止timer
    AP_StopPeriodTimer(fsm_ptr);

    //先检查复读
    error_code = APCheckPlayEndReview(fsm_ptr);
    if (MMISRVAP_ERROR_MAX == error_code
       )
    {
        //更新msg ID
        APUpdateMsgID(fsm_ptr, msg_ptr->msg_id);
        AP_SetErrorCode(fsm_ptr, MMISRVAP_ERROR_NONE);
        APSetCallerCallbackFlag(TRUE, fsm_ptr);
    }


    SCI_TRACE_LOW("[MMISRVAP] APHandlePlayEndReq error_code = %d", \
                  error_code);

    return error_code;
}

/*****************************************************************************/
//  Description : 处理复读
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL  MMISRVAP_ERROR_E APHandleReviewReq(
    MMISRVAP_FSM_T *fsm_ptr, //in
    MMISRVAP_FSM_MSG_T *msg_ptr //in
)
{
    MMISRVAP_ERROR_E error_code = MMISRVAP_ERROR_PARAM;
    MMISRVAP_REVIEW_INFO_T *review_ptr = PNULL;

    BOOLEAN result = FALSE;

    BOOLEAN is_support_review = FALSE;
    uint32 offset = 0;

    if(PNULL == fsm_ptr
            || PNULL == msg_ptr
      )
    {
        return error_code;
    }

    do
    {
        review_ptr = AP_GetReviewInfoPtr(fsm_ptr);
        if (PNULL == review_ptr)
        {
            break;
        }

        is_support_review = APIsSupportReview(fsm_ptr);
        if (!is_support_review)
        {
            break;
        }


        error_code = MMISRVAP_ERROR_NONE;

        //先停止tiemr
        AP_StopPeriodTimer(fsm_ptr);

        //更新msg ID
        APUpdateMsgID(fsm_ptr, msg_ptr->msg_id);


        switch(fsm_ptr->state)
        {
        case MMISRVAP_STATE_PAUSE:
            result = MMISRVAUD_Resume(fsm_ptr->audiosrv_handle);
            break;
        default:
            result = TRUE;
            break;
        }

        if (!result)
        {
            error_code = MMISRVAP_ERROR_REQ;
            break;
        }

        //进入播放状态
        AP_FsmTransState(fsm_ptr, MMISRVAP_STATE_PLAYING);

        if (MMISRVAP_REVIEW_START_REQ == msg_ptr->msg_id)
        {
            offset = review_ptr->start_point;
        }
        else
        {
            offset = review_ptr->end_point;
            SCI_MEMSET(review_ptr, 0, sizeof(*review_ptr));
        }

        result = MMISRVAUD_Seek(fsm_ptr->audiosrv_handle, offset);
        if(!result)
        {
            error_code = MMISRVAP_ERROR_REQ;
        }


        APCheckPeriodTimer(fsm_ptr);
    }
    while(0);


    AP_SetErrorCode(fsm_ptr, error_code);
    APSetCallerCallbackFlag(TRUE, fsm_ptr);


    SCI_TRACE_LOW("[MMISRVAP] APHandleStopReviewReq error_code = %d", \
                  error_code);

    return error_code;
}

/*****************************************************************************/
//  Description : 处理event ex req
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL  MMISRVAP_ERROR_E APHandleEventPlayingReq(
    MMISRVAP_FSM_T *fsm_ptr, //in
    MMISRVAP_FSM_MSG_T *msg_ptr //in
)
{
    MMISRVAP_ERROR_E error_code = MMISRVAP_ERROR_PARAM;
    MMISRVAP_ERROR_E playing_ret = 0;
    MMISRVAP_PLAY_INFO_T* play_info_ptr = PNULL;




    if(PNULL == fsm_ptr
            || PNULL == msg_ptr
            || PNULL == msg_ptr->msg_body
      )
    {
        return error_code;
    }

    play_info_ptr = APGetPlayInfoPtr(fsm_ptr);
    do
    {
        if (PNULL == play_info_ptr)
        {
            break;
        }

        //更新msg ID
        APUpdateMsgID(fsm_ptr, MMISRVAP_PLAY_REQ);

        playing_ret = *((MMISRVAP_ERROR_E*)msg_ptr->msg_body);

        if (MMISRVAP_ERROR_NONE == playing_ret)
        {
            play_info_ptr->play_ret = MMISRVAP_PLAYING_OK;
        }
        else
        {
            play_info_ptr->play_ret = MMISRVAP_PLAYING_MAX;
        }

        error_code = MMISRVAP_ERROR_NONE;
    }
    while(0);


    AP_SetErrorCode(fsm_ptr, error_code);
    APSetCallerCallbackFlag(TRUE, fsm_ptr);


    SCI_TRACE_LOW("[MMISRVAP] APHandleEventPlayingReq error_code = %d", \
                  error_code);

    return error_code;
}

/*****************************************************************************/
//  Description : 处理pause req
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL  MMISRVAP_ERROR_E APHandlePauseReq(
    MMISRVAP_FSM_T *fsm_ptr, //in
    MMISRVAP_FSM_MSG_T *msg_ptr //in
)
{
    MMISRVAP_ERROR_E error_code = MMISRVAP_ERROR_PARAM;
    BOOLEAN is_trans_state = FALSE;
    BOOLEAN is_do_pause = FALSE;

    if(PNULL == fsm_ptr
            || PNULL == msg_ptr
      )
    {
        return error_code;
    }

    do
    {
        error_code = MMISRVAP_ERROR_NONE;

        //停止timer
        AP_StopPeriodTimer(fsm_ptr);


        //更新msg ID
        APUpdateMsgID(fsm_ptr, msg_ptr->msg_id);

        switch(msg_ptr->msg_id)
        {
        case MMISRVAP_PAUSE_REQ:
            is_do_pause = TRUE;
            break;
        case MMISRVAP_EVENT_PAUSE_REQ:
            /*
            *MMISRVAP_EVENT_PAUSE_REQ是通知消息,audio srv已经操作resume
            */
            APSetPausedByAudioSrvFlag(fsm_ptr, TRUE);
            is_trans_state = TRUE;
            break;
        default:
            break;
        }


        if (is_do_pause)
        {
            is_trans_state = MMISRVAUD_Pause(fsm_ptr->audiosrv_handle);
            if (!is_trans_state)
            {
                error_code = MMISRVAP_ERROR_REQ;
            }
        }


        if(is_trans_state)
        {
            AP_FsmTransState(fsm_ptr, MMISRVAP_STATE_PAUSE);
        }
    }
    while(0);


    AP_SetErrorCode(fsm_ptr, error_code);
    APSetCallerCallbackFlag(TRUE, fsm_ptr);


    SCI_TRACE_LOW("[MMISRVAP] APHandlePauseReq error_code = %d", \
                  error_code);

    return error_code;
}

/*****************************************************************************/
//  Description : 处理resume req
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL  MMISRVAP_ERROR_E APHandleResumeReq(
    MMISRVAP_FSM_T *fsm_ptr, //in
    MMISRVAP_FSM_MSG_T *msg_ptr //in
)
{
    MMISRVAP_ERROR_E error_code = MMISRVAP_ERROR_PARAM;
    BOOLEAN is_do_resume = FALSE;
    BOOLEAN is_trans_state = FALSE;


    if(PNULL == fsm_ptr
            || PNULL == msg_ptr
      )
    {
        return error_code;
    }

    do
    {
        error_code = MMISRVAP_ERROR_NONE;


        //更新msg ID
        APUpdateMsgID(fsm_ptr, msg_ptr->msg_id);

        switch(msg_ptr->msg_id)
        {
        case MMISRVAP_RESUME_REQ:
            is_do_resume = TRUE;
            break;
        case MMISRVAP_EVENT_RESUME_REQ:
            /*
            *MMISRVAP_EVENT_RESUME_REQ是通知消息,audio srv已经操作resume
            */
            if (APIsPausedByAudioSrv(fsm_ptr))
            {
                is_trans_state = TRUE;
                APSetPausedByAudioSrvFlag(fsm_ptr, FALSE);
            }
            break;
        case MMISRVAP_MGR_RESUME_REQ:
            MMISRVMGR_Resume(fsm_ptr->audiosrv_handle);

            if (APIsPausedByAudioSrv(fsm_ptr))
            {
                //is_do_resume = TRUE;
                is_trans_state = TRUE;
                APSetPausedByAudioSrvFlag(fsm_ptr, FALSE);
            }
            break;
        default:
            break;
        }

        if (is_do_resume)
        {
            is_trans_state = MMISRVAUD_Resume(fsm_ptr->audiosrv_handle);
            if (!is_trans_state)
            {
                error_code = MMISRVAP_ERROR_REQ;
            }
        }


        if(is_trans_state)
        {
            AP_FsmTransState(fsm_ptr, MMISRVAP_STATE_PLAYING);

            //开启timer
            APCheckPeriodTimer(fsm_ptr);
        }

    }
    while(0);


    AP_SetErrorCode(fsm_ptr, error_code);
    APSetCallerCallbackFlag(TRUE, fsm_ptr);


    SCI_TRACE_LOW("[MMISRVAP] APHandleEventPlayingReq error_code = %d", \
                  error_code);

    return error_code;
}

/*****************************************************************************/
//  Description : 处理event ex req
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL  MMISRVAP_ERROR_E APHandleEventExtReq(
    MMISRVAP_FSM_T *fsm_ptr, //in
    MMISRVAP_FSM_MSG_T *msg_ptr //in
)
{
    MMISRVAP_ERROR_E error_code = MMISRVAP_ERROR_PARAM;
    MMISRVAP_EXT_T *ext_ptr = PNULL;


    if(PNULL == fsm_ptr
            || PNULL == msg_ptr
            || PNULL == msg_ptr->msg_body
      )
    {
        return error_code;
    }


    do
    {
        //更新msg ID
        APUpdateMsgID(fsm_ptr, msg_ptr->msg_id);

        ext_ptr = APGetExtInfoPtr(fsm_ptr);
        if (PNULL == ext_ptr)
        {
            break;
        }
        ext_ptr->data = *((uint32*)msg_ptr->msg_body);
        error_code = MMISRVAP_ERROR_NONE;
    }
    while(0);


    AP_SetErrorCode(fsm_ptr, error_code);
    APSetCallerCallbackFlag(TRUE, fsm_ptr);


    SCI_TRACE_LOW("[MMISRVAP] APHandleEventExtReq error_code = %d", \
                  error_code);

    return error_code;
}

/*****************************************************************************/
//  Description : playing状态的处理
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN APPlaying(
    MMISRVAP_HANDLE ap_handle, //in
    MMISRVAP_FSM_MSG_T *msg_ptr //in
)
{
    BOOLEAN    result = TRUE;
    MMISRVAP_FSM_T *fsm_ptr = (MMISRVAP_FSM_T*)FSMSRV_GetObject((FSMSRV_HANDLE)ap_handle);



    //参数检查
    if(PNULL == fsm_ptr
            || PNULL == msg_ptr
      )
    {
        return FALSE;
    }

    SCI_TRACE_LOW("[MMISRVAP] APPlaying msg_ptr->msg_id = %d", \
                  msg_ptr->msg_id);


    APSetCallerCallbackFlag(FALSE, fsm_ptr);

    switch(msg_ptr->msg_id)
    {
    case FSMSRV_ENTRY_REQ:
        break;

    case FSMSRV_EXIT_REQ:
        break;

    case MMISRVAP_SEEK_REQ:
        APHandleSeekReq(fsm_ptr, msg_ptr);
        break;

    case MMISRVAP_PAUSE_REQ:
    case MMISRVAP_EVENT_PAUSE_REQ:
        APHandlePauseReq(fsm_ptr, msg_ptr);
        break;

    case MMISRVAP_STOP_REQ:
        APHandleStopReq(fsm_ptr, msg_ptr);
        break;
    case MMISRVAP_EVENT_PLAY_END_REQ:
        APHandlePlayEndReq(fsm_ptr, msg_ptr);
        break;
    case MMISRVAP_REVIEW_START_REQ:
    case MMISRVAP_REVIEW_STOP_REQ:
        APHandleReviewReq(fsm_ptr, msg_ptr);
        break;
    case MMISRVAP_EVENT_EXT_REQ:
#ifdef BT_DIALER_SUPPORT
    case MMISRVAP_EVENT_EARPHONE_REQ:
#endif
        APHandleEventExtReq(fsm_ptr, msg_ptr);
        break;
    case MMISRVAP_EVENT_PLAYING_REQ:
        APHandleEventPlayingReq(fsm_ptr, msg_ptr);
        break;
    case MMISRVAP_MGR_RESUME_REQ:
        APHandleResumeReq(fsm_ptr, msg_ptr);
        break;
    default:

        result =  FALSE;
        break;
    }


    APNotifyCaller(fsm_ptr);

    return result;
}

/*****************************************************************************/
//  Description : pause状态的处理
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN APPause(
    MMISRVAP_HANDLE ap_handle, //in
    MMISRVAP_FSM_MSG_T *msg_ptr //in
)
{
    BOOLEAN    result = TRUE;
    MMISRVAP_FSM_T *fsm_ptr = (MMISRVAP_FSM_T*)FSMSRV_GetObject((FSMSRV_HANDLE)ap_handle);



    //参数检查
    if(PNULL == fsm_ptr
            || PNULL == msg_ptr
      )
    {
        return FALSE;
    }

    SCI_TRACE_LOW("[MMISRVAP] APPause msg_ptr->msg_id = %d", \
                  msg_ptr->msg_id);


    APSetCallerCallbackFlag(FALSE, fsm_ptr);

    switch(msg_ptr->msg_id)
    {
    case MMISRVAP_RESUME_REQ:
    case MMISRVAP_EVENT_RESUME_REQ:
    case MMISRVAP_MGR_RESUME_REQ:
        APHandleResumeReq(fsm_ptr, msg_ptr);
        break;

    case MMISRVAP_SEEK_REQ:
        APHandleSeekReq(fsm_ptr, msg_ptr);
        break;

    case MMISRVAP_STOP_REQ:
        APHandleStopReq(fsm_ptr, msg_ptr);
        break;
    case MMISRVAP_REVIEW_START_REQ:
    case MMISRVAP_REVIEW_STOP_REQ:
        APHandleReviewReq(fsm_ptr, msg_ptr);
        break;
    case MMISRVAP_EVENT_EXT_REQ:
#ifdef BT_DIALER_SUPPORT
    case MMISRVAP_EVENT_EARPHONE_REQ:
#endif
        APHandleEventExtReq(fsm_ptr, msg_ptr);
        break;
    case MMISRVAP_EVENT_PLAYING_REQ:
        APHandleEventPlayingReq(fsm_ptr, msg_ptr);
        break;
    default:

        result =  FALSE;
        break;
    }


    APNotifyCaller(fsm_ptr);

    return result;
}

/*****************************************************************************/
//  Description : 分发请求消息
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN AP_FsmDispatch(
    MMISRVAP_HANDLE   ap_handle, //in
    MMISRVAP_FSM_MSG_E msg_id, //in
    void *msg_body_ptr, //in
    uint32 msg_body_size //in
)
{
    BOOLEAN result = FALSE;


    //参数检查
    if(PNULL == ap_handle)
    {
        return result;
    }

    MMISRVAP_TRACE_LOW("[MMISRVAP] AP_FsmDispatch msg_id = %d, msg_body_ptr = 0x%X, msg_body_size = %d", \
                       msg_id, msg_body_ptr, msg_body_size);

    result = FSMSRV_Dispatch((FSMSRV_HANDLE)ap_handle, \
                             (uint32)msg_id, \
                             (void *)msg_body_ptr, \
                             (uint32)msg_body_size \
                            );

    MMISRVAP_TRACE_LOW("[MMISRVAP] AP_FsmDispatch result = %d", \
                       result);

    return result;
}

/*****************************************************************************/
//  Description : 转换状态
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void AP_FsmTransState(
    MMISRVAP_FSM_T *fsm_ptr, //in
    MMISRVAP_STATE_E state //in
)
{
    FSM_STATE_FUNC state_func = PNULL;
    MMISRVAP_HANDLE   ap_handle = PNULL;
    FSM_STATE_FUNC func_array[] =
    {
        APIdle,
        APReady,
        APPlaying,
        APPause
    };


    //参数检查
    if((PNULL == fsm_ptr)
            || (PNULL == fsm_ptr->ap_handle)
            || state >= MMISRVAP_STATE_MAX
      )
    {
        return ;
    }


    ap_handle = fsm_ptr->ap_handle;

    MMISRVAP_TRACE_LOW("[MMISRVAP] AP_FsmTransState state = %d", \
                       state);


    state_func = func_array[state];

    fsm_ptr->state = state;

    FSMSRV_StateTrans((FSMSRV_HANDLE)ap_handle, \
                      state_func
                     );

}

/*****************************************************************************/
//  Description : 获得idle处理函数
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC FSM_STATE_FUNC AP_GetIdleStateFunc(void)
{
    FSM_STATE_FUNC state_func = PNULL;

    state_func = (FSM_STATE_FUNC)APIdle;

    return state_func;
}

/*****************************************************************************/
//  Description : 处理周期timer callback
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL void APHandlePeriodTimerCallback(
    MMISRVAP_FSM_T *fsm_ptr //in
)
{
    MMISRVAP_CALLBACK_PARAM_T callback_param = {0};
    MMISRVAP_EVENT_E event_id = 0; //事件ID



    if (PNULL == fsm_ptr)
    {
        return;
    }


    event_id = MMISRVAP_EVENT_PERIOD_TIMER;

    callback_param.handle = fsm_ptr->ap_handle;
    callback_param.event_id = event_id;
    callback_param.error_code = AP_GetErrorCode(fsm_ptr);


    MMISRVAP_TRACE_LOW("[MMISRVAP] APHandlePeriodTimerCallback event_id=%d", \
                       event_id);

    //do callback
    if(PNULL != fsm_ptr->caller_callback)
    {
        fsm_ptr->caller_callback(&callback_param, sizeof(callback_param));
    }
}

/*****************************************************************************/
//  Description : 处理周期timer函数
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL void APHandlePeriodTimer(
    uint8 timer_id,
    uint32 param
)
{
    MMISRVAP_FSM_T *fsm_ptr = PNULL;


    if (PNULL == param)
    {
        return;
    }


    MMISRVAP_TRACE_LOW("[MMISRVAP] APHandlePeriodTimer timer_id=%d", \
                       timer_id);

    MMISRVAP_TRACE_LOW("[MMISRVAP] APHandlePeriodTimer param=%d", \
                       param);

    fsm_ptr = (MMISRVAP_FSM_T*)FSMSRV_GetObject((FSMSRV_HANDLE)param);

    /* 待时间到时，有可能fsm已经被销毁
    * 故这里检查handle是否相等
    */
    if (PNULL != fsm_ptr
            && fsm_ptr->ap_handle == (FSMSRV_HANDLE)param
       )
    {
        APCheckNotEndPointReview(fsm_ptr);


        //do callback
        if (APIsSupportPeriodTimer(fsm_ptr))
        {
            APHandlePeriodTimerCallback(fsm_ptr);
        }

        AP_StartPeriodTimer(fsm_ptr);
    }
}

/*****************************************************************************/
//  Description : 开启周期timer
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void AP_StartPeriodTimer(
    MMISRVAP_FSM_T *fsm_ptr //in
)
{
    MMISRVAP_PERIOD_TIMER_T *period_ptr = PNULL;
    uint32 time_out = 0;

    if (PNULL == fsm_ptr)
    {
        return;
    }

    period_ptr = APGetPeriodTimerInfoPtr(fsm_ptr);
    if (PNULL != period_ptr)
    {
        time_out = period_ptr->period_time;
        if (0 == time_out)
        {
            time_out = MMISRVAP_DEFAULT_PERIOD_TIME;
        }

        AP_StopPeriodTimer(fsm_ptr);

        period_ptr->timer_id = MMK_CreateTimerCallback(time_out,
                               APHandlePeriodTimer,
                               (uint32)fsm_ptr->ap_handle,
                               FALSE
                                                      );

        MMISRVAP_TRACE_LOW("[MMISRVAP] AP_StartPeriodTimer time_out=%d", \
                           time_out);
    }
}

/*****************************************************************************/
//  Description : 停止周期timer
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void AP_StopPeriodTimer(
    MMISRVAP_FSM_T *fsm_ptr //in
)
{
    MMISRVAP_PERIOD_TIMER_T *period_ptr = PNULL;

    if (PNULL == fsm_ptr)
    {
        return;
    }

    period_ptr = APGetPeriodTimerInfoPtr(fsm_ptr);
    if (PNULL != period_ptr
            && 0 != period_ptr->timer_id
       )
    {
        MMK_StopTimer(period_ptr->timer_id);
        period_ptr->timer_id = 0;

        MMISRVAP_TRACE_LOW("[MMISRVAP] AP_StopPeriodTimer");
    }
}

