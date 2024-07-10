/*****************************************************************************
** File Name:      mmisrvap_idown.c                                          *
** Author:                                                                   *
** Date:           11/07/2012                                                *
** Copyright:      2012 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to audio file srv inferface for down    *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 07/2012       robert.wang       Create                                    *
******************************************************************************/

#define _MMISRVAP_IDOWN_C_
/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sig_code.h"
#include "mmk_type.h"

//other module
#include "mmisrv_fsm.h"

//local module
#include "mmisrvap_iup.h"
#include "mmisrvap_fsm.h"
#include "mmisrvap_idown.h"

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


/**--------------------------------------------------------------------------*
 **                         LOCAL DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                               *
**--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : 用于处理其他task给srvap发送的消息
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E AP_DispatchSignalToAPSrv(
    PWND app_ptr, //in
    uint16 msg_id, //in
    DPARAM param //in
)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;


    MMISRV_HANDLE_T audio_handle = PNULL;
    MMISRVAP_FSM_MSG_E req_msg_id = 0;
    void *msg_body_ptr = PNULL;
    uint32 msg_body_size = 0;
    MMISRVAP_TASK_MSG_T msg_info = {0};

    uint32 notify_info = 0;



    MMISRVAP_TRACE_LOW("[MMISRVAP] AP_DispatchSignalToAPSrv msg_id=0x%X", \
                       msg_id);


    switch(msg_id)
    {
    case APP_SRVAP_EVENT_FROM_AUDIO_SRV:
    {
        MMISRVAP_TASK_MSG_T *sendSignal_ptr = (MMISRVAP_TASK_MSG_T *)param;


        if(PNULL != sendSignal_ptr)
        {
            MMI_MEMCPY(&msg_info, sizeof(msg_info),
                       sendSignal_ptr, sizeof(*sendSignal_ptr),
                       sizeof(*sendSignal_ptr)
                      );
        }

        notify_info = msg_info.notify_info;
        audio_handle = msg_info.hAudio;

        req_msg_id = msg_info.type;
        msg_body_size = sizeof(notify_info);
        msg_body_ptr = &notify_info;
    }
    break;
    default:
        result = MMI_RESULT_FALSE;
        break;
    }



    AP_Lock();

    if(MMI_RESULT_TRUE == result)
    {
        BOOLEAN ret = FALSE;
        MMISRVAP_HANDLE ap_handle = PNULL;


        //遍历查找到audio handle，对应的ap handle
        //并发送消息到对应的task id.
        ret = AP_GetAPInfoByAudioHandle(audio_handle, &ap_handle);
        if (ret)
        {
            //检查ap hanlde的有效性
            //内部不再检查有效性
            if(AP_IsValidHandle(ap_handle))
            {
                //向videoplayer control发送消息
                result = AP_FsmDispatch(ap_handle,
                                        req_msg_id,
                                        msg_body_ptr,
                                        msg_body_size
                                       );
            }
        }
    }


    MMISRVAP_TRACE_LOW("[MMISRVAP] AP_DispatchSignalToAPSrv result=%d", \
                       result);
    AP_UnLock();

    return result;
}

/*****************************************************************************/
//  Description : 处理audio srv回调的数据
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN APHandleAudioSrvReport(
    MMISRVAUD_REPORT_T *report_ptr, //in
    MMISRVAP_TASK_MSG_T *msg_ptr //in
)
{
    BOOLEAN ret = TRUE;
    MMISRVAP_FSM_MSG_E  event_id = 0;
    MMISRVAP_ERROR_E error_code = MMISRVAP_ERROR_NONE;



    if(PNULL == report_ptr
            || PNULL == msg_ptr
      )
    {
        return ret;
    }

    MMISRVAP_TRACE_LOW("[MMISRVAP] APHandleAudioSrvReport report=%d,data=%d", \
                       report_ptr->report, report_ptr->data1);

    switch(report_ptr->report)
    {
    case MMISRVAUD_REPORT_END:
        event_id = MMISRVAP_EVENT_PLAY_END_REQ;
        break;

    case MMISRVAUD_REPORT_PLAY:
        event_id = MMISRVAP_EVENT_PLAYING_REQ;

        switch(report_ptr->data1)
        {
        case MMISRVAUD_RET_OK:
            error_code = MMISRVAP_ERROR_NONE;
            break;

        case MMISRVAUD_RET_ERROR:
            error_code = MMISRVAP_ERROR_UNKNOWN;
            break;

        default:
            ret = FALSE;
            break;
        }

        break;
	case MMISRVAUD_REPORT_EARPHONE_EVENT:
    case MMISRVAUD_REPORT_BT_EVENT:
        event_id = MMISRVAP_EVENT_EXT_REQ;
        error_code = report_ptr->data1;
        break;

    default:
        ret = FALSE;
        break;
    }


    if(ret)
    {
        msg_ptr->type = event_id;
        msg_ptr->notify_info = error_code;
    }

    MMISRVAP_TRACE_LOW("[MMISRVAP] APHandleAudioSrvReport event_id=%d", \
                       event_id);
    MMISRVAP_TRACE_LOW("[MMISRVAP] APHandleAudioSrvReport error_code=%d", \
                       error_code);


    return ret;
}

/*****************************************************************************/
//  Description : Send Signal To task
//  Global resource dependence :
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL void APSendSignalToTask(
    APP_MULTIMEDIA_SIGNALCODE_E signalType, //in
    uint32 task_id, //in
    MMISRVAP_TASK_MSG_T *msg_ptr //in
)
{
    MMISRVAP_TASK_MSG_T *sendSignal_ptr = PNULL;


    MmiCreateSignal(signalType, sizeof(MMISRVAP_TASK_MSG_T), (MmiSignalS **)&sendSignal_ptr);

    if(PNULL != msg_ptr
            && PNULL != sendSignal_ptr
      )
    {
        sendSignal_ptr->hAudio = msg_ptr->hAudio;
        sendSignal_ptr->notify_info = msg_ptr->notify_info;
        sendSignal_ptr->affix_info = msg_ptr->affix_info;
        sendSignal_ptr->type = msg_ptr->type;

        MmiSendSignal(task_id, (MmiSignalS *)sendSignal_ptr);
    }

    MMISRVAP_TRACE_LOW("[MMISRVAP] APSendSignalToTask signalType=%d", \
                       signalType);
    MMISRVAP_TRACE_LOW("[MMISRVAP] APSendSignalToTask task_id=%d", \
                       task_id);
}

/*****************************************************************************/
//  Description : audio srv call back
//  Global resource dependence :
//  Author:
//  Note:
//
//typedef struct
//{
//    uint32 hAudio; == handle
//    uint32 notify_info; == data
//    uint32 apfix_info; == reserve
//    uint32 type; == event id
//}MMISRVAP_TASK_MSG_T;

/*****************************************************************************/
PUBLIC BOOLEAN AP_AudioSrvCallBack(
    MMISRV_HANDLE_T audio_handle, //in
    MMISRVMGR_NOTIFY_PARAM_T *param //in
)
{
    BOOLEAN result = TRUE;
    MMISRVAUD_REPORT_T *report_ptr = PNULL;

    APP_MULTIMEDIA_SIGNALCODE_E sig_type = 0;
    MMISRVAP_TASK_MSG_T msg_info = {0};

    MMISRVAP_ERROR_E error_code = MMISRVAP_ERROR_NONE;


    MMISRVAP_TRACE_LOW("[MMISRVAP] AP_AudioSrvCallBack handle=0x%X", \
                       audio_handle);

    if(PNULL != audio_handle
            && PNULL != param
      )
    {
        MMISRVAP_TRACE_LOW("[MMISRVAP] AP_AudioSrvCallBack event=%d", \
                           param->event);

        sig_type = APP_SRVAP_EVENT_FROM_AUDIO_SRV;

        switch(param->event)
        {
        case MMISRVMGR_NOTIFY_SUSPEND:
            msg_info.type = MMISRVAP_EVENT_PAUSE_REQ;

            msg_info.notify_info = error_code;
            break;

        case MMISRVMGR_NOTIFY_RESUME:
            msg_info.type = MMISRVAP_EVENT_RESUME_REQ;

            msg_info.notify_info = error_code;
            break;

        case MMISRVMGR_NOTIFY_EXT:
            report_ptr = (MMISRVAUD_REPORT_T *)param->data;

            if(PNULL != report_ptr)
            {
                result = APHandleAudioSrvReport(report_ptr,
                                                &msg_info
                                               );
            }

            break;

        default:
            result = FALSE;
            break;
        }
    }



    //发送消息到对应的task id.
    if (PNULL != param)
    {
        msg_info.hAudio = audio_handle;

        APSendSignalToTask(sig_type,
                           param->ind_data,
                           &msg_info
                          );
    }



    return result;

}

