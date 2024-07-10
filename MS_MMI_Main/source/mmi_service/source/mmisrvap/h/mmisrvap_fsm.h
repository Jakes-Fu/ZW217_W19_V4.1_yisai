/*****************************************************************************
** File Name:      mmisrvap_fsm.h                                            *
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
#ifndef _MMISRVAP_FSM_H_
#define _MMISRVAP_FSM_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmk_app.h"
#include "mmisrv_fsm.h"
#include "mmisrvaud.h"

#include "mmisrvap_export.h"

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C"
{
#endif

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define MMISRVAP_FSM_MSG_T FSMSRV_MSG_T

#define MMISRVAP_DEFAULT_PERIOD_TIME   700
/**--------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/

typedef enum
{
    MMISRVAP_STATE_IDLE,
    MMISRVAP_STATE_READY,
    MMISRVAP_STATE_PLAYING,
    MMISRVAP_STATE_PAUSE,
    MMISRVAP_STATE_MAX
}MMISRVAP_STATE_E;

typedef enum
{
    MMISRVAP_RELEASE_REQ = FSMSRV_USER_REQ, //from caller
    MMISRVAP_PLAY_REQ, //from callber
    MMISRVAP_PAUSE_REQ, //from callber
    MMISRVAP_RESUME_REQ, //from caller    
    MMISRVAP_SEEK_REQ, //from caller
    MMISRVAP_STOP_REQ, //from caller
    MMISRVAP_REVIEW_START_REQ, //from caller
    MMISRVAP_REVIEW_STOP_REQ, //from caller
    MMISRVAP_MGR_RESUME_REQ, //from caller
    MMISRVAP_EVENT_PAUSE_REQ, //from audio srv
    MMISRVAP_EVENT_RESUME_REQ, //from audio srv
    MMISRVAP_EVENT_PLAYING_REQ, //from audio srv
    MMISRVAP_EVENT_PLAY_END_REQ, //from audio srv
    MMISRVAP_EVENT_EXT_REQ, //from audio srv
#ifdef BT_DIALER_SUPPORT
    MMISRVAP_EVENT_EARPHONE_REQ, //from audio srv
#endif	
    MMISRVAP_EVENT_REVIEW_PLAY_END_REQ, //from ap srv
    MMISRVAP_RELEASE_AUDIO_HANDLE_REQ, //from caller
    MMISRVAP_REQUEST_AUDIO_HANDLE_REQ, //from caller or ap srv
    MMISRVAP_MSG_MAX
}MMISRVAP_FSM_MSG_E;

/*
复读结构体
*/
typedef struct
{
    BOOLEAN is_support_review; // 是否开启review服务
    BOOLEAN is_end_file; //是否文件结束
    uint32 start_point; //起始位置
    uint32 end_point; //结束位置
    uint32 total_time; //文件总时间
}MMISRVAP_REVIEW_INFO_T;

typedef struct
{
    wchar* full_path_ptr; //媒体名字(本地文件为全路径)
    uint16 full_path_len; //媒体名字长度
    MMISRVAUD_VOLUME_T vol;           //音量
    BOOLEAN is_support_bt;
    BOOLEAN is_enable_seek; //是否可以seek

    MMISRVAP_REVIEW_INFO_T review_info;
    uint32 offset_point; //播放起点
    MMISRVAUD_RING_FMT_E audio_type; //audio type, maybe drm audio type

    MMISRVAP_PLAYING_STATE_E play_ret; //播放结果
}MMISRVAP_PLAY_INFO_T;


/*
周期timer
*/
typedef struct
{
    BOOLEAN is_support_timer; //是否开启timer
    uint8 timer_id; //timer id
    uint32 period_time; //周期时间
}MMISRVAP_PERIOD_TIMER_T;

/*
扩展结构，用于BT数据传递
*/
typedef struct
{
    uint32 data; //
}MMISRVAP_EXT_T;


typedef struct
{
    MMISRV_HANDLE_T audiosrv_handle; //audio service handle
    MMISRVAP_HANDLE ap_handle; //audio file handle
    MMISRVAP_FSM_MSG_E msg_id;
    MMISRVAP_STATE_E state; //状态机状态
    
    BOOLEAN is_do_caller_callback;  //是否执行caller callback
    MMISRVAP_CALLBACK caller_callback; //ui回调函数
    
    MMISRVAP_ERROR_E error_code; //错误码

    MMISRVAP_PLAY_INFO_T play_info;

    MMISRVAP_PERIOD_TIMER_T period_time_info;

    MMISRVAP_EXT_T ext_info;

    uint32 task_id;

    BOOLEAN is_paused_by_audsrv;//是否被audio srv暂停
}MMISRVAP_FSM_T;

typedef struct
{
    uint32 msg_id;
    uint32 event_id;
}MMISRVAP_EVENT_COMBO_T;
/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
 
/*****************************************************************************/
//  Description : 转换状态
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void AP_FsmTransState(
    MMISRVAP_FSM_T *fsm_ptr, //in
    MMISRVAP_STATE_E state //in
);

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
);

/*****************************************************************************/
//  Description : 获得idle处理函数
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC FSM_STATE_FUNC AP_GetIdleStateFunc(void);

/*****************************************************************************/
//  Description : 开启周期timer
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void AP_StartPeriodTimer(
    MMISRVAP_FSM_T *fsm_ptr //in
);

/*****************************************************************************/
//  Description : 停止周期timer
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void AP_StopPeriodTimer(
    MMISRVAP_FSM_T *fsm_ptr //in
);

/*****************************************************************************/
//  Description : get review info pointer
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC MMISRVAP_REVIEW_INFO_T* AP_GetReviewInfoPtr(
    MMISRVAP_FSM_T *fsm_ptr //in
);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif

#endif



