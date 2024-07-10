/*****************************************************************************
** File Name:      mmisrvap_export.h                                         *
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
#ifndef _MMISRVAP_EXPORT_H_
#define _MMISRVAP_EXPORT_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmk_app.h"

#include "mmisrvaud_api.h"
#include "mmisrv_fsm.h"

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
#define MMISRVAP_TRACE_LOW SCI_TRACE_LOW



/**--------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/
typedef FSMSRV_HANDLE MMISRVAP_HANDLE;

typedef enum
{
    MMISRVAP_ERROR_NONE,
    MMISRVAP_ERROR_PARAM,
    MMISRVAP_ERROR_NOT_ENOUGH_MEMORY,
    MMISRVAP_ERROR_FILE_FORMAT_NOT_SUPPORT,               //格式不支持
    MMISRVAP_ERROR_REQ, //请求失败
    MMISRVAP_ERROR_UNKNOWN, //未知错误
    MMISRVAP_ERROR_MAX
}MMISRVAP_ERROR_E;

typedef enum
{
    MMISRVAP_EVENT_OPEN,
    MMISRVAP_EVENT_PLAYING,
    MMISRVAP_EVENT_PLAY_END,
    MMISRVAP_EVENT_PAUSE,
    MMISRVAP_EVENT_RESUME,
    MMISRVAP_EVENT_SEEK,
    MMISRVAP_EVENT_STOP,
    MMISRVAP_EVENT_EXT, //such as BT event
#ifdef BT_DIALER_SUPPORT
    MMISRVAP_EVENT_EARPHONE, // earphone event
#endif
    MMISRVAP_EVENT_REVIEW_START,
    MMISRVAP_EVENT_REVIEW_STOP,
    MMISRVAP_EVENT_REVIEW_PLAY_END, //review 服务的播放结束事件
    MMISRVAP_EVENT_PERIOD_TIMER,
    MMISRVAP_EVENT_TO_DO_PAUSE, //notify caller to do
    MMISRVAP_EVENT_TO_DO_RESUME, //notify caller to do
    MMISRVAP_EVENT_MGR_RESUME,
    MMISRVAP_EVENT_UPDATE_AUDIO_HANDLE, //update audio handle
    MMISRVAP_EVENT_MAX
}MMISRVAP_EVENT_E;

typedef enum
{
    MMISRVAP_PLAYING_NONE, //未播放
    MMISRVAP_PLAYING_OK, //播放成功
    MMISRVAP_PLAYING_PENDING, //播放阻塞
    MMISRVAP_PLAYING_MAX
}MMISRVAP_PLAYING_STATE_E;

typedef struct
{
    uint32 start_point; //起始位置,可以为0
    uint32 end_point; //结束位置, 可以为0
}MMISRVAP_REQ_REVIEW_PARAM_T;

typedef struct
{
    wchar*                  full_path_ptr;      //播放文件的绝对路径(从本地文件播放时有效)
    uint16                  full_path_len;      //播放文件的绝对路径长度(从本地文件播放时有效)
    MMISRVAUD_VOLUME_T      vol;                 //音量
    BOOLEAN                 is_support_bt; //是否支持蓝牙播放
    uint32                  period_time; //周期时间
    uint32                  offset_point; //播放起点
    MMISRVAUD_RING_FMT_E audio_type; //audio type, maybe drm audio type

    MMISRVAP_REQ_REVIEW_PARAM_T review_param; //review
}MMISRVAP_REQ_PLAY_PARAM_T;

typedef struct
{
    uint32 offset;
}MMISRVAP_REQ_SEEK_PARAM_T;


typedef struct
{
    BOOLEAN is_end_file; //是否是文件结束
    uint32 total_time; //文件总时间
    uint32 total_length; //文件总长度
}MMISRVAP_CALLBACK_REVIEW_PLAY_END_PARAM_T;

typedef struct
{
    MMISRVAUD_BT_EVENT_E bt_event;
}MMISRVAP_CALLBACK_BT_PARAM_T;

typedef struct
{
    MMISRVAP_PLAYING_STATE_E play_ret; //播放结果
}MMISRVAP_CALLBACK_PLAYING_PARAM_T;

//回调函数的参数结构
typedef struct
{
    MMISRVAP_EVENT_E event_id; //事件ID
    MMISRVAP_HANDLE handle;
    MMISRVAP_ERROR_E error_code;
    void *param_ptr;
    uint32 size_of_param;
} MMISRVAP_CALLBACK_PARAM_T;

typedef void (*MMISRVAP_CALLBACK) (MMISRVAP_CALLBACK_PARAM_T *callback_param_ptr, uint32 size_of_callback_param);//状态机给ui发送消息的回调函数

typedef struct
{
    MMISRVAUD_VOLUME_T      vol;                 //音量
    BOOLEAN                 is_support_bt; //是否支持蓝牙播放

    MMISRVAP_CALLBACK callback_func; //ap srv 通知 caller的回调函数
}MMISRVAP_OPEN_PARAM_T;


/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : init module
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMISRVAP_InitModule(void);

/*****************************************************************************/
//  Description : 开启audio file srv
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC MMISRVAP_HANDLE MMISRVAP_Open(
    MMISRVAP_OPEN_PARAM_T *open_param_ptr //in
);
/*****************************************************************************/
//  Description : MMISRVAP_ResetReviewInfo
//  Global resource dependence :  none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMISRVAP_ResetReviewInfo(
	MMISRVAP_HANDLE ap_handle //in
);
/*****************************************************************************/
//  Description : 关闭audio file srv
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMISRVAP_Close(
    MMISRVAP_HANDLE ap_handle //in
);

/*****************************************************************************/
//  Description : 播放文件
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC MMISRVAP_ERROR_E MMISRVAP_Play(
    MMISRVAP_HANDLE ap_handle, //in
    MMISRVAP_REQ_PLAY_PARAM_T *play_param_ptr, //in
    uint32 size_of_param //in
);

/*****************************************************************************/
//  Description : 暂停播放
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC MMISRVAP_ERROR_E MMISRVAP_Pause(
    MMISRVAP_HANDLE ap_handle //in
);

/*****************************************************************************/
//  Description : 恢复播放
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC MMISRVAP_ERROR_E MMISRVAP_Resume(
    MMISRVAP_HANDLE ap_handle //in
);

/*****************************************************************************/
//  Description : 搜索
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC MMISRVAP_ERROR_E MMISRVAP_SeekTo(
    MMISRVAP_HANDLE ap_handle, //in
    MMISRVAP_REQ_SEEK_PARAM_T *seek_param_ptr, //in
    uint32 size_of_param //in
);

/*****************************************************************************/
//  Description : 停止播放
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC MMISRVAP_ERROR_E MMISRVAP_Stop(
    MMISRVAP_HANDLE ap_handle //in
);

/*****************************************************************************/
//  Description : 获得播放中的信息
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC MMISRVAP_ERROR_E MMISRVAP_GetPlayingInfo(
    MMISRVAP_HANDLE ap_handle, //in
    MMISRVAUD_PLAY_INFO_T *playing_info_ptr //out
);

/*****************************************************************************/
//  Description : 设置音量
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC MMISRVAP_ERROR_E MMISRVAP_SetVolume(
    MMISRVAP_HANDLE ap_handle, //in
    MMISRVAUD_VOLUME_T vol //in
);

/*****************************************************************************/
//  Description : 设置复读位置
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC MMISRVAP_ERROR_E MMISRVAP_SetReviewPoint(
    MMISRVAP_HANDLE ap_handle, //in
    MMISRVAP_REQ_REVIEW_PARAM_T *review_param_ptr //in
);

/*****************************************************************************/
//  Description : 启动复读
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC MMISRVAP_ERROR_E MMISRVAP_StartReview(
    MMISRVAP_HANDLE ap_handle //in
);

/*****************************************************************************/
//  Description : 停止复读
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC MMISRVAP_ERROR_E MMISRVAP_StopReview(
    MMISRVAP_HANDLE ap_handle //in
);

/*****************************************************************************/
//  Description : mgr resume
//  Global resource dependence :  none
//  Author: robert.wang
//  Note: 让其他handle处于非激活状态，
//       让本handle处于激活状态
//    1) 若是被audio srv暂停的,会有resume动作
//    2) 若未被audio srv暂停，保持原来状态
/*****************************************************************************/
PUBLIC MMISRVAP_ERROR_E MMISRVAP_MGR_Resume(
    MMISRVAP_HANDLE ap_handle //in
);

/*****************************************************************************/
//  Description : request audio handle
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC MMISRVAP_ERROR_E MMISRVAP_RequestAudioHandle(
    MMISRVAP_HANDLE ap_handle //in
);

/*****************************************************************************/
//  Description : release audio handle
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC MMISRVAP_ERROR_E MMISRVAP_ReleaseAudioHandle(
    MMISRVAP_HANDLE ap_handle //in
);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif




