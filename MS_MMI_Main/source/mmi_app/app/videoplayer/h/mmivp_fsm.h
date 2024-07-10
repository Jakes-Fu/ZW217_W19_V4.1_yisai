/*****************************************************************************
** File Name:      mmivp_fsm.h                                                   *
** Author:                                                                   *
** Date:           2010/3/18                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe t9 text input               *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2010/3/18      apple.zhang       Create
******************************************************************************/

#ifndef _MMIVP_FSM_H_
#define _MMIVP_FSM_H_

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "mmivp_export.h"
#include "dal_player.h"
#include "mn_type.h"
#ifdef MMI_STREAMING_PLAYER_SUPPORT
#include "strm_api.h"
#else
#include "mmivp_dummy.h"
#endif
#ifdef MMI_KING_MOVIE_SUPPORT
#include "video_security.h"
#endif
#include "mmisrvaud_api.h"

/**--------------------------------------------------------------------------*
**                         Compiler Flag                                    *
**--------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C"
{
#endif
    
/*---------------------------------------------------------------------------*/
/*                          MACRO DEFINITION                                 */
/*---------------------------------------------------------------------------*/
#define VP_SUPPORTED_MAX_HEIGHT       480    //允许播放视频的最大高度
#define VP_SUPPORTED_MAX_WIDTH        640   //允许播放视频的最大宽度
    
/**--------------------------------------------------------------------------*
**                         TYPE AND CONSTANT                                *
**--------------------------------------------------------------------------*/   
//状态机接收的消息
typedef enum
{
    VP_INIT_REQ = VP_MSG_MAX,       //请求初始化 0x412c
        VP_STRM_INIT_CNF,           //streaming初始化请求应答0x412d
        VP_RELEASE_REQ,             //请求释放0x412e
        VP_PLAY_REQ,                //请求播放0x412f
        VP_STRM_PLAY_CNF,           //streaming播放请求应答0x4130
        VP_STOP_REQ,                //请求停止0x4131
        VP_STRM_STOP_CNF,           //streaming停止请求应答0x4132
        VP_PAUSE_REQ,               //请求暂停0x4133
        VP_STRM_PAUSE_CNF,          //streaming暂停请求应答0x4134
        VP_RESUME_REQ,              //请求恢复播放0x4135
        VP_STRM_RESUME_CNF,         //streaming恢复播放请求应答0x4136
        VP_SEEK_REQ,                //请求定位播放0x4137
        VP_STRM_SEEK_CNF,           //streaming定位请求应答0x4138
        VP_AUTH_RSP,                //请求鉴权0x4139
        VP_AUTHIND_REQ,             //请求获取鉴权信息0x413a
        VP_STRM_CLOSE_CNF,          //streaming关闭请求应答0x413b
        VP_STRM_BUFFERING_IND,      //streaming缓冲开始0x413c
        VP_STRM_BUFFER_END_IND,     //缓冲结束0x413d
        VP_STRM_DATA_TIMEOUT_IND,   //streaming获取数据超时0x413e
        VP_STRM_GOODBYE_IND,        //streaming播放结束0x413f
        VP_CALLBACK_END_IND,        //dplayer播放结束0x4140
        VP_FRAME_END_IND,           //dplayer一帧结束    0x4141
        VP_PDP_ACTIVE_CNF,          //pdp激活应答 0x4142
        VP_PDP_DEACTIVE_IND,        //网络主动断开0x4143
        VP_STRM_SERVER_CLOSE_IND,   //streaming服务器关闭0x4144
        VP_ENTRY_REQ,                       //0x4145
        VP_EXIT_REQ,                //0x4146
        VP_PLAY_NEXT_FILE,          //0x4147
        VP_START_SET_RATE,          //0x4148
        VP_STOP_SET_RATE,          //0x4149
        VP_BUFFER_PERCENT,           //0x414a
        VP_FRAME_END_NEED_DATA,     //0x414b
        VP_REQ_MSG_MAX              //0x414c
} VP_FSM_MSG_E; 

//控制层的消息结构
typedef struct 
{
    VP_HANDLE           fsm_handle;     //状态机
    VP_FSM_MSG_E        msg_id;         //消息id
    uint32              msg_size;       //消息大小
    void                *msg_body;      //消息体
}VP_FSM_MSG_T;


//状态机信息结构
typedef struct  
{
    DPLAYER_HANDLE              dplayer_handle;             //dplayer handle
    MMISRV_HANDLE_T             srv_handle;             //audio service handle
#ifdef MMI_KING_MOVIE_SUPPORT
    VPLAYER_SECURITY_HANDLE         sec_handle;        //SECURITY_HANDLE
    BOOLEAN  b_sprd_kmv;
#endif
    STRM_HANDLE                 strm_handle;                //streaming handle
    uint32                      task_id;                    //playing task id
    VP_STATE_E                  state;                      //状态机状态
    VP_PLAY_TYPE_E              play_type;                  //播放方式
    VP_SUPPORTED_STATUS_TYPE_E  support_type;               //支持程度
    VP_CALLBACK                 vpcallback;                 //ui回调函数
    uint32                      process_time;               //当前播放时间
    uint32                      total_time;                 //总时间
    wchar*                      full_path_ptr;              //媒体名字(本地文件为全路径)
    uint16                      full_path_len;              //媒体名字长度 
    uint8                       *url_ptr;                   //网络播放的url
    VP_DISPLAY_PARAM_T          display_param;              //播放显示参数,显示坐标为逻辑坐标
    BOOLEAN                     is_scal_up;                 //是否拉伸
    VP_REPEAT_PLAY_MODE_E       mode;                       //播放模式
    BOOLEAN                     is_random_play;             //是否开启随机播放
    uint8                       vol;                        //音量
    MN_DUAL_SYS_E               dual_sys;                   //网络播放时使用的sim卡
    BOOLEAN                     is_to_pause;                //强制暂停标志
    BOOLEAN                     is_to_resume;               //强制恢复标志
    BOOLEAN                     is_to_exit;                 //强制停止释放
    VP_PLAY_END_RESULT_E        has_play_end_reason;            //已经结束的原因（用于已经结束，但是还没有来的及处理）
    BOOLEAN                     is_save_history;            //是否保存历史记录
    BOOLEAN                     is_not_notify_ui;           //是否通知ui刷新 false为通知，true为不通知
    uint32                      buf_percent;                 //缓冲百分比
    BOOLEAN                     is_bt_support;
    BOOLEAN                     is_a2dp_support;
    BOOLEAN                     is_share_play;      //边下载边播放 
    BOOLEAN                     is_cr_video_layer;          //是否创建了图像层
    BOOLEAN			is_audio_coexist;   
    DPLAYER_IMAGE_DATA_T        image_data;           //图像层数据
	VP_PLAYBACK_MODE_E          playback_mode;        //本地文件播放方式
	BOOLEAN                     is_seek_able;        //是否可以seek
}VP_FSM_T;


//状态机列表 用于扩展多状态机，但是目前支持单个状态机
typedef struct VP_FSM_LIST_tag
{
    VP_FSM_T                *fsm_info_ptr;       //状态机信息
    struct VP_FSM_LIST_tag   *next_fsm_ptr;     //下一个状态机
}VP_FSM_LIST_T;



typedef BOOLEAN    (*FSM_DISPATCH)(VP_FSM_T *fsm_ptr, VP_FSM_MSG_T *msg_ptr);       //状态机消息分配函数
/**--------------------------------------------------------------------------*
**                         Compiler Flag                                    *
**--------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif

#endif
