/*****************************************************************************
** File Name:                    videoplayer.c                               *
** Author:                                                                   *
** Date:                                                                     *
** Copyright:      Spreadtrum, Incoporated. All Rights Reserved.             *
** Description:    This file is used to describe             *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
**								   Create
******************************************************************************/

#include "mmi_app_wre_boot_trc.h"
#ifdef WRE_SUPPORT

#include "sci_types.h"
#include "mmi_module.h"
#include "os_api.h"
#include "tasks_id.h"
#include "mmifmm_export.h"
#include "../../../../../../wre/wrekernel/include/sdk/videoplayer.h"
#include "sig_code.h"
#include "mmipdp_export.h"
#include "mmiconnection_export.h"

#include "mmiudisk_export.h"
//#include "mmieng.h"
#include "mmieng_export.h"
#include "mmiset_display.h"
#include "mmisd_export.h"

#include "strm_api.h"
#include "in_message.h"

#ifdef DRM_SUPPORT
#include "mmidrm_export.h" 
#endif

#include "mmi_appmsg.h"
#include "dal_player.h"
#include "mmiaudio_ctrl.h"
#include "mmicc_export.h"
#include "mmk_app.h"

#ifdef VIDEO_PLAYER_SUPPORT
#include "mmivp_export.h"
#endif
#include "mmiwre.h"
#include "mmiwre_adaptor.h"
#include "mmiwre_other.h"
/*---------------------------------------------------------------------------*/
/*                          MACRO DEFINITION                                 */
/*---------------------------------------------------------------------------*/
#define WMP_TRACE_LOW  SCI_TRACE_LOW

#define  HTTP_URL_HEAD_ASIC       "http://"
#define  HTTP_URL_HEAD_LEN        7
#define  RTSP_URL_HEAD_ASIC       "rtsp://"
#define  RTSP_URL_HEAD_LEN        7
#define  SEC_TO_MSEL              1000             //1s = 1000ms
#define  MAX_INVALID_VALUE        0xffffffff       //无效最大值    
#ifdef MMI_BLACK_COLOR
#undef MMI_BLACK_COLOR
#endif	// changed by yifei
#define  MMI_BLACK_COLOR		  0x0000

/**--------------------------------------------------------------------------*
**                         TYPE AND CONSTANT                                *
**--------------------------------------------------------------------------*/  
typedef unsigned long VIDEOHANDLE; 
typedef DPLAYER_DISPLAY_PARAM_T LCD_VIDEO_DISPLAY_PARAM ;
extern uint32 WRE_MEDIA_CURRENT_VOLUME ;//20120613
//PUBLIC BOOLEAN SetVideoPlayerVolume(HVIDEOHANDLE hVideo,unsigned long  dwVolume);
//状态机接收的消息
typedef enum
{
        FSM_INIT_REQ = VIDEO_NOTIFY_MAX,   //请求初始化 
        FSM_STRM_INIT_CNF,           //streaming初始化请求应答
        FSM_RELEASE_REQ,             //请求释放
        FSM_PLAY_REQ,                //请求播放
        FSM_STRM_PLAY_CNF,           //streaming播放请求应答
        FSM_STOP_REQ,                //请求停止
        FSM_STRM_STOP_CNF,           //streaming停止请求应答
        FSM_PAUSE_REQ,               //请求暂停
        FSM_STRM_PAUSE_CNF,          //streaming暂停请求应答
        FSM_RESUME_REQ,              //请求恢复播放
        FSM_STRM_RESUME_CNF,         //streaming恢复播放请求应答
        FSM_SEEK_REQ,                //请求定位播放
        FSM_STRM_SEEK_CNF,           //streaming定位请求应答
        FSM_AUTH_RSP,                //请求鉴权
        FSM_AUTHIND_REQ,             //请求获取鉴权信息
        FSM_STRM_CLOSE_CNF,          //streaming关闭请求应答
        FSM_STRM_BUFFERING_IND,      //streaming缓冲开始
        FSM_STRM_BUFFER_END_IND,     //缓冲结束
        FSM_STRM_DATA_TIMEOUT_IND,   //streaming获取数据超时
        FSM_STRM_GOODBYE_IND,        //streaming播放结束
        FSM_CALLBACK_END_IND,        //dplayer播放结束
        FSM_FRAME_END_IND,           //dplayer一帧结束
        FSM_PDP_ACTIVE_CNF,          //pdp激活应答
        FSM_PDP_DEACTIVE_IND,        //网络主动断开
        FSM_STRM_SERVER_CLOSE_IND,   //streaming服务器关闭
        FSM_ENTRY_REQ,                       
        FSM_EXIT_REQ,                //
        FSM_PLAY_NEXT_FILE,          //
        FSM_START_SET_RATE,          //
        FSM_STOP_SET_RATE,           //
        FSM_BUFFER_PERCENT,          //
		FSM_OPEN_REQ,                //打开文件请求
        FSM_REQ_MSG_MAX
} FSM_MSG_E; 

//流媒体网址信息
typedef struct 
{
    unsigned short         szTitle[MAX_NAME_LEN+1];       //流媒体网址名称
    unsigned long  int     nTitleLen;                     //流媒体网址名称长度
    char                   szUrl[MAX_URL_LEN+1];          //流媒体网址
}VIDEO_NET_INFO; 

//本地文件信息
typedef struct 
{
	unsigned short             strFilePath[MAX_NAME_LEN +1];     //文件的绝对路径
    unsigned short             uFilePathLen;                     //文件的绝对路径长度
    unsigned long int          nCreateTime;                      //文件创建时间
    unsigned long int          nWidth;                           //视频图像宽度
    unsigned long int          nHeight;                          //视频图像高度
    unsigned long int          nMediaLen;                        //视频持续时长
    unsigned long int          nFileSize;                        //视频文件大小
}VIDEO_FILE_INFO;

//播放设置参数
typedef struct
{
    unsigned char           uVolume;       //播放音量
    BOOLEAN                 bMute;         //是否静音
    unsigned char           ubrightness;   //播放亮度   
    unsigned short          uLowestPort;   //RTP/RTCP最低端口
    unsigned short          uHighestPort;  //RTP/RTCP最高端口    
    unsigned char           szAgentIp[MAX_IP_LENGTH+1];       //代理IP
    unsigned char           uAgentIpLenth;
    unsigned short          uAgentPort;   //代理端口
    BOOLEAN                 bAgent;                       //是否使用代理
}VIDEO_SETTING;

//控制层的消息结构
typedef struct 
{
    HVIDEOHANDLE          hVideo;     //状态机
    FSM_MSG_E           msg_id;         //消息id
    uint32              msg_size;       //消息大小
    void                *msg_body;      //消息体
}VIDEO_FSM_MSG;

//状态机信息结构
typedef struct  
{
	DPLAYER_HANDLE              dplayer_handle;             //dplayer handle
	MMISRV_HANDLE_T             srv_handle;             //audio service handle  //added by leichi
	STRM_HANDLE                 strm_handle;                //streaming handle
	uint32                      task_id;                    //playing task id
	VIDEO_STATE                 state;                      //状态机状态
	VIDEO_PLAY_TYPE             playType;                   //播放方式
	VIDEO_SUPPORTED_TYPE        support_type;               //支持程度
	VideoProc                   vpCallBack;                 //ui回调函数	
	uint32                      process_time;               //当前播放时间
	uint32                      total_time;                 //总时间
	wchar*                      strFilePath;                //媒体名字(本地文件为全路径)
	uint16                      uFilePathLen;               //媒体名字长度 
	uint8                       *url_ptr;                   //网络播放的url
	LCD_VIDEO_DISPLAY_PARAM     LcdDisplayParam;			//播放显示参数,显示坐标为逻辑坐标
	BOOLEAN                     is_scal_up;                 //是否拉伸
	uint8                       vol;                        //音量
	unsigned long               netID;                      //网络播放时使用的网络ID
	BOOLEAN                     is_to_pause;                //强制暂停标志
	BOOLEAN                     is_to_resume;               //强制恢复标志
	BOOLEAN                     is_to_exit;                 //强制停止释放
	VIDEO_PLAY_END_RESULT       has_play_end_reason;        //已经结束的原因（用于已经结束，但是还没有来的及处理）
	BOOLEAN                     is_not_notify_ui;           //是否通知ui刷新 false为通知，true为不通知
	uint32                      buf_percent;                //缓冲百分比
	BOOLEAN                     is_bt_support;
	BOOLEAN                     bSupportA2dp;
	BOOLEAN                     is_share_play;				//边下载边播放 
	VIDEO_SETTING               setting_info;
}VIDEO_PLAYER;

//状态机列表 用于扩展多状态机，但是目前支持单个状态机
typedef struct VIDEO_PLAYER_LIST_tag
{
    VIDEO_PLAYER                   *fsm_info_ptr;       //状态机信息
    struct VIDEO_PLAYER_LIST_tag   *next_fsm_ptr;     //下一个状态机
}VIDEO_PLAYER_LIST;

typedef BOOLEAN    (*VIDEO_FSM_DISPATCH)(VIDEO_PLAYER *fsm_ptr, VIDEO_FSM_MSG *msg_ptr);       //状态机消息分配函数

#define WRE_VIDEOPLAYER  "Wre videoplayer"
/*---------------------------------------------------------------------------*/
/*                         GLOBAL DEFINITION                                 */
/*---------------------------------------------------------------------------*/
extern int strnicmp (const char *a, const char *b, size_t c);

/**-------------------------------------------------------------------------*
**                         LOCAL FUNCTION DECLARE                           *
**--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : 创建状态机
//  Global resource dependence :  none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL HVIDEOHANDLE CreateFSM(VideoProc vpCallBack);

/*****************************************************************************/
//  Description : 销毁状态机
//  Global resource dependence :  none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void DestroyFSM(HVIDEOHANDLE hVideo);

/*****************************************************************************/
//  Description : 判断状态机是否有效
//  Global resource dependence :  none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsValidFsm(VIDEO_PLAYER *fsm_ptr);

/*****************************************************************************/
//  Description : 分发状态机消息
//  Global resource dependence :  none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN FsmDispatch(HVIDEOHANDLE   hVideo,  FSM_MSG_E msg_id, void *msg_body_ptr, uint32 msg_body_size);

/*****************************************************************************/
//  Description : 转换状态
//  Global resource dependence :  none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void FsmStateTrans(VIDEO_PLAYER *fsm_ptr, VIDEO_STATE state);

/*****************************************************************************/
//  Description : create mutex 
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void CreateFsmMutex(void);

/*****************************************************************************/
//  Description : destroy mutex 
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void DestroyFsmMutex(void);

/*****************************************************************************/
//  Description : frame end notify call back
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void CallbackFrameEndNotify(int32 frame_num);

/*****************************************************************************/
//  Description : end notify回调函数
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void CallbackEndNotify(int32 i_type);

/*****************************************************************************/
//  Description : 创建状态机消息
//  Global resource dependence :  none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL VIDEO_FSM_MSG* CreatMsg(HVIDEOHANDLE hVideo, FSM_MSG_E msg_id, void *msg_body_ptr, uint32 msg_body_size);

/*****************************************************************************/
//  Description : 释放状态机消息
//  Global resource dependence :  none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN DestroyMsg(VIDEO_FSM_MSG *msg_ptr);

/*****************************************************************************/
//  Description : 给ui层发送VIDEO_NOTIFY_PREPARE
//  Global resource dependence :  none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void SendPrepareIndToUI(VIDEO_PLAYER *fsm_ptr);

/*****************************************************************************/
//  Description : 给ui层发送VIDEO_NOTIFY_AUTH
//  Global resource dependence :  none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void SendAUTHIndToUI(VIDEO_PLAYER *fsm_ptr);

/*****************************************************************************/
//  Description : 给ui层发送VIDEO_NOTIFY_PLAY_PROCESS
//  Global resource dependence :  none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void SendProcessIndToUI(VIDEO_PLAYER *fsm_ptr);

/*****************************************************************************/
//  Description : 给ui层发送VIDEO_NOTIFY_BUFFERING
//  Global resource dependence :  none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void SendBufferingIndToUI(VIDEO_PLAYER *fsm_ptr);

/*****************************************************************************/
//  Description : 给ui层发送WMP_BUFFER_PROCESS_IND
//  Global resource dependence :  none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void SendBufferPercentToUI(VIDEO_PLAYER *fsm_ptr);

/*****************************************************************************/
//  Description : 给ui层发送VIDEO_NOTIFY_BUFFER_END
//  Global resource dependence :  none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void SendBufferEndIndToUI(VIDEO_PLAYER *fsm_ptr);

/*****************************************************************************/
//  Description : 给ui层发送VIDEO_NOTIFY_PLAY_END
//  Global resource dependence :  none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void SendPlayEndToUI(VIDEO_PLAYER *fsm_ptr, VIDEO_PLAY_END_RESULT result);

/*****************************************************************************/
//  Description : 给ui层发送VIDEO_NOTIFY_STOP
//  Global resource dependence :  none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void SendStopIndToUI(VIDEO_PLAYER *fsm_ptr);
/*****************************************************************************/
//  Description : 给ui层发送VIDEO_NOTIFY_FATAL_ERROR
//  Global resource dependence :  none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void SendFatalErrorIndTOUI(VIDEO_PLAYER *fsm_ptr,VIDEO_ERROR_TYPE errorType);

/*****************************************************************************/
//  Description : 给ui层发送VIDEO_NOTIFY_PLAY
//  Global resource dependence :  none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void SendPlayCnfToUI(VIDEO_PLAYER *fsm_ptr,VIDEO_ERROR_TYPE errorType);

/*****************************************************************************/
//  Description : 给ui层发送VIDEO_NOTIFY_PAUSE
//  Global resource dependence :  none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void SendPauseCnfToUI(VIDEO_PLAYER *fsm_ptr,VIDEO_ERROR_TYPE errorType);

/*****************************************************************************/
//  Description : 给ui层发送VIDEO_NOTIFY_SEEK
//  Global resource dependence :  none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void SendSeekCnfToUI(VIDEO_PLAYER *fsm_ptr,VIDEO_ERROR_TYPE errorType);

/*****************************************************************************/
//  Description : 通过frame获得对应的time
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL uint32 FrameToTime(DPLAYER_HANDLE video_handle, uint32 frame);

/*****************************************************************************/
//  Description : 开始播放本地文件
//  Global resource dependence :  none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL VIDEO_ERROR_TYPE PlayLocalFile(VIDEO_PLAYER *fsm_ptr, BOOLEAN is_need_fill);

/*****************************************************************************/
//  Description : 开始播放本地buf
//  Global resource dependence :  none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL VIDEO_ERROR_TYPE PlayLocalBuf(VIDEO_PLAYER *fsm_ptr, VIDEO_PLAY_PARAM  *play_param_ptr);

/*****************************************************************************/
//  Description : 开始播放网络文件
//  Global resource dependence :  none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL VIDEO_ERROR_TYPE PlayFromNet(VIDEO_PLAYER *fsm_ptr,DPLAYER_STRM_CFG_T strm_cfg);

/*****************************************************************************/
//  Description : 处理pdp消息
//  Global resource dependence :  none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void HandlePDPMsg(MMIPDP_CNF_INFO_T *msg_ptr);

/*****************************************************************************/
//  Description : 激活pdp
//  Global resource dependence :  none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN ActivePdp(VIDEO_PLAYER *fsm_ptr);

/*****************************************************************************/
//  Description : PDP激活中处理
//  Global resource dependence :  none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN FsmPdpActiving(VIDEO_PLAYER *fsm_ptr, VIDEO_FSM_MSG *msg_ptr);

/*****************************************************************************/
//  Description : 初始化状态机信息
//  Global resource dependence :  none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void InitFsmInfo(VIDEO_PLAYER *fsm_ptr, 
                       VIDEO_STATE      state, 
                       VideoProc     vpCallBack);

/*****************************************************************************/
//  Description : 更新状态机信息
//  Global resource dependence :  none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN UpdateFsmInfo(VIDEO_PLAYER *fsm_ptr, VIDEO_PLAY_PARAM* play_param_ptr);

/*****************************************************************************/
//  Description : 空闲状态的处理
//  Global resource dependence :  none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN FsmIdle(VIDEO_PLAYER *fsm_ptr, VIDEO_FSM_MSG *msg_ptr);

/*****************************************************************************/
//  Description : 准备好状态的处理
//  Global resource dependence :  none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN FsmReady(VIDEO_PLAYER *fsm_ptr, VIDEO_FSM_MSG *msg_ptr);

/*****************************************************************************/
//  Description : PDP激活中处理
//  Global resource dependence :  none
//  Author: 
//  Note: 
/*****************************************************************************/
#ifdef MMI_STREAMING_PLAYER_SUPPORT //20120612

LOCAL BOOLEAN InitStreaming(VIDEO_PLAYER *fsm_ptr, uint32 net_id);

#endif
/*****************************************************************************/
//  Description : 已初始化状态
//  Global resource dependence :  none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN FsmInit(VIDEO_PLAYER *fsm_ptr, VIDEO_FSM_MSG *msg_ptr);

/*****************************************************************************/
//  Description : 已初始化状态
//  Global resource dependence :  none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN FsmInit_PauseEx(VIDEO_PLAYER *fsm_ptr, VIDEO_FSM_MSG *msg_ptr);

/*****************************************************************************/
//  Description : 认证中
//  Global resource dependence :  none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN FsmInitAuth(VIDEO_PLAYER *fsm_ptr, VIDEO_FSM_MSG *msg_ptr);

/*****************************************************************************/
//  Description : 播放准备中
//  Global resource dependence :  none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN FsmPlayPeding(VIDEO_PLAYER *fsm_ptr, VIDEO_FSM_MSG *msg_ptr);

/*****************************************************************************/
//  Description : 认证中
//  Global resource dependence :  none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN FsmPlayAuth(VIDEO_PLAYER *fsm_ptr, VIDEO_FSM_MSG *msg_ptr);

/*****************************************************************************/
//  Description : 播放中
//  Global resource dependence :  none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN FsmPlaying(VIDEO_PLAYER *fsm_ptr, VIDEO_FSM_MSG *msg_ptr);

/*****************************************************************************/
//  Description : 缓冲中
//  Global resource dependence :  none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN FsmBuffering(VIDEO_PLAYER *fsm_ptr, VIDEO_FSM_MSG *msg_ptr);

/*****************************************************************************/
//  Description : 播放定位中
//  Global resource dependence :  none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN FsmSeeking(VIDEO_PLAYER *fsm_ptr, VIDEO_FSM_MSG *msg_ptr);

/*****************************************************************************/
//  Description : 暂停中
//  Global resource dependence :  none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN FsmPausing(VIDEO_PLAYER *fsm_ptr, VIDEO_FSM_MSG *msg_ptr);

/*****************************************************************************/
//  Description : 暂停
//  Global resource dependence :  none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN FsmPaused(VIDEO_PLAYER *fsm_ptr, VIDEO_FSM_MSG *msg_ptr);

/*****************************************************************************/
//  Description : 暂停后的player停止，streaming暂停
//  Global resource dependence :  none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN FsmPausedEx(VIDEO_PLAYER *fsm_ptr, VIDEO_FSM_MSG *msg_ptr);

/*****************************************************************************/
//  Description : 暂停中播放定位中
//  Global resource dependence :  none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN FsmPausedSeeking(VIDEO_PLAYER *fsm_ptr, VIDEO_FSM_MSG *msg_ptr);

/*****************************************************************************/
//  Description : 恢复定位中
//  Global resource dependence :  none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN FsmResuming(VIDEO_PLAYER *fsm_ptr, VIDEO_FSM_MSG *msg_ptr);

/*****************************************************************************/
//  Description : 停止中
//  Global resource dependence :  none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN FsmStopping(VIDEO_PLAYER *fsm_ptr, VIDEO_FSM_MSG *msg_ptr);

/*****************************************************************************/
//  Description : 停止
//  Global resource dependence :  none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN FsmStopped(VIDEO_PLAYER *fsm_ptr, VIDEO_FSM_MSG *msg_ptr);

/*****************************************************************************/
//  Description : 关闭中
//  Global resource dependence :  none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN FsmClosing(VIDEO_PLAYER *fsm_ptr, VIDEO_FSM_MSG *msg_ptr);

/*****************************************************************************/
//  Description : 处理streaming init cnf消息
//  Global resource dependence :  none
//  Author: 
//  Note: 
/*****************************************************************************/
#ifdef MMI_STREAMING_PLAYER_SUPPORT //20120612

LOCAL STRM_RESULT_E HandleInitCnf(VIDEO_PLAYER * fsm_ptr, STRM_INIT_CNF_T* strm_init_ptr, VIDEO_ERROR_TYPE *is_play_success);
#endif
/*****************************************************************************/
//  Description : 处理update frame请求消息
//  Global resource dependence :  none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void HandleUpdateFrameReq(VIDEO_PLAYER * fsm_ptr);

/*****************************************************************************/
//  Description : 处理pause请求消息
//  Global resource dependence :  none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN HandlePauseReq(VIDEO_PLAYER * fsm_ptr);

/*****************************************************************************/
//  Description : 处理resume请求消息
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN HandleResumeReq(VIDEO_PLAYER* fsm_ptr);

/*****************************************************************************/
//  Description : 处理seek请求消息
//  Global resource dependence :  none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN HandleSeekReq(VIDEO_PLAYER * fsm_ptr, VIDEO_SEEK_PARAM *seek_param);

/*****************************************************************************/
//  Description : 处理停止消息
//  Global resource dependence :  none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN HandleStopReq(VIDEO_PLAYER* fsm_ptr,BOOLEAN is_begin_play);

/*****************************************************************************/
//  Description : special pause
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN HandlePauseReqEx(BOOLEAN is_pause_streaming,
                               VIDEO_PLAYER* fsm_ptr);
/*****************************************************************************/
//  Description : special resume
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN HandleResumeReqEx(BOOLEAN is_resume_streaming,
                                VIDEO_PLAYER* fsm_ptr);

/*****************************************************************************/
//  Description : SendSignalTo current playing task
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void SendSignalToCurPlayingTask(uint16 signalType, uint16 data, uint32 task_id);

/*****************************************************************************/
//  Description : 切换显示模式（全屏，半屏，TVOUT)
//  Global resource dependence :  none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN SwitchDisplayMode(HVIDEOHANDLE  hVideo,LCD_VIDEO_DISPLAY_PARAM  *display_param_ptr,BOOLEAN is_only_rect);

#ifdef DRM_SUPPORT
/*****************************************************************************/
//  Description : consume drm right
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void ConsumeDrmRight(const wchar *name_path_ptr,uint16       name_len);
#endif

/*****************************************************************************/
//  Description : SettingAsScreensaver
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
//#ifdef SCREENSAVER_SUPPORT
//LOCAL BOOLEAN SettingAsScreensaver(wchar *full_name_ptr, uint16 full_name_len);
//#endif

/*****************************************************************************/
//  Description : SettingAsPowerOnOffAnimation
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
//LOCAL BOOLEAN SettingAsPowerOnOffAnimation(BOOLEAN is_power_on, wchar *full_name_ptr, uint16 full_name_len);

/*****************************************************************************/
//  Description : 用于是否可以显示全屏的相关信息
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
//LOCAL BOOLEAN IsDisplayFullScreenReferInfoEnable(void);

/*****************************************************************************/
//  Description : GetCurBufPercent
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL uint32 GetCurBufPercent(HVIDEOHANDLE hVideo);

/*****************************************************************************/
//  Description : UpdateCurrentFrame
//  Global resource dependence : 
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL void UpdateCurrentFrame(HVIDEOHANDLE hVideo);

/*****************************************************************************/
//  Description : 设置osd和透明色
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void SetBlockType(uint32 alpha);

/*****************************************************************************/
//  Description : 把展示参数从逻辑转到物理
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN TranslateDisplayParamLogicToPhysical(LCD_VIDEO_DISPLAY_PARAM *disp_para, BOOLEAN is_need_fill);

/*****************************************************************************/
//  Description : 发起鉴权认证
//  Global resource dependence :  none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN AuthRsp(HVIDEOHANDLE   hVideo,VIDEO_AUTH_INFO  *auth_info_ptr);

/*****************************************************************************/
//  Description : 切换播放设备
//  Global resource dependence :  none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN SwitchPlayDevice(HVIDEOHANDLE  hVideo,
								BOOLEAN    bSupportA2dp,
								BOOLEAN    is_bt_support);

/*****************************************************************************/
//  Description : 执行退出流程，并销毁状态机。
//  Global resource dependence :  none
//  Author: 
//  Note: 满足一键退出的需求。
/*****************************************************************************/
LOCAL void ExitVideo(HVIDEOHANDLE  hVideo);

/*****************************************************************************/
//  Description : GetTransparenceColor
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL GUI_COLOR_T GetTransparenceColor(void);

/*****************************************************************************/
//  Description : 用于处理其他task给jvm发送的消息
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E DispatchSignalToFsmMsg(PWND app_ptr, uint16 msg_id, DPARAM param);

/*****************************************************************************/
//  Description : 通过全路径名获得媒体文件信息
//  Global resource dependence :  none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void GetMediaInfoFromFullPathName(
										 const wchar            *full_file_name, 
										 uint16                  full_name_len,
										 DPLAYER_MEDIA_INFO_T    *media_ptr
										 );

/*****************************************************************************/
//  Description : 通过buf中获得媒体文件信息
//  Global resource dependence :  none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void GetMediaInfoFromBuf(
								uint8                   *video_buf,
								uint32                  video_length,
								VIDEO_FILE_TYPE             video_type,
								DPLAYER_MEDIA_INFO_T   *media_ptr
								);

/*****************************************************************************/
//  Description : 获得媒体信息
//  Global resource dependence :  none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void GetMediaInfo(HVIDEOHANDLE  hVideo,DPLAYER_MEDIA_INFO_T    *media_info_ptr);

/*****************************************************************************/
//  Description : Get the supported_type of a movie.
//  Global resource dependence :  none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL VIDEO_SUPPORTED_TYPE CheckSupportedTypeFromMediaInfo(DPLAYER_MEDIA_INFO_T *media_ptr);

/*****************************************************************************/
//  Description : 填充透明色
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN FillRectWithTransparenceColor(
											 unsigned long int *width_ptr, //[IN/OUT]
											 unsigned long int *height_ptr, //[IN/OUT]
											 unsigned long int *x_offset_ptr, //[IN/OUT]
											 unsigned long int * y_offset_ptr, //[IN/OUT]
											 unsigned long int RotateAngle,
											 BOOLEAN is_need_fill                                  
                                           );

/*****************************************************************************/
//  Description : align 4
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL uint32 GetAlign4Offset(uint32 x);

/*****************************************************************************/
//  Description : main lcd is landscape
//  Global resource dependence : 
//  Author: 
//  Note: 主屏是否横屏
/*****************************************************************************/
LOCAL BOOLEAN IsMainScreenLandscape(void);

/*****************************************************************************/
//  Description : 用于判断视频播放器对该视频文件的支持程度
//  Global resource dependence :  none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL  VIDEO_SUPPORTED_TYPE GetSupportedStatus(unsigned short  * strFileName, unsigned short  nFileNameLen);

//#include "mmk_regapp.def"
//#include "mmk_ext_app.h"
PUBLIC MMI_APPLICATION_T  g_wre_videoplayer_app = {DispatchSignalToFsmMsg, CT_APPLICATION, PNULL}; 
/*---------------------------------------------------------------------------*/
/*                         STATIC DEFINITION                                 */
/*---------------------------------------------------------------------------*/
LOCAL   uint32                  s_wmp_fsm_list      = 0;  //state machine list; 扩展多状态机使用
LOCAL   SCI_MUTEX_PTR           s_fsm_list_mutex    = 0; //mutex to protect s_wmp_fsm_list;
LOCAL   VIDEO_PLAYER          * s_current_fsm       = PNULL;  //记录当前fsm

PUBLIC BOOLEAN MMIWRE_IsWREVideoPlayerActive(void)//defined by leichi for sidekey function
{
	SCI_TRACE_LOW("wre_video MMIWRE_IsWREVideoPlayerActive s_current_fsm= 0x%x",s_current_fsm);
	if(PNULL != s_current_fsm)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}


LOCAL const   VIDEO_FSM_DISPATCH  video_fsm_dispatch[VIDEO_STATE_MAX]    = {         //状态机的分配处理函数
	FsmIdle,
        FsmReady,
        FsmPdpActiving,
        FsmInit,
        FsmInit_PauseEx,
        FsmInitAuth,
        FsmPlayPeding,
        FsmPlayAuth,
        FsmPlaying,
        FsmBuffering,
        FsmSeeking,
        FsmPausing,
        FsmPaused,
        FsmPausedEx,
        FsmPausedSeeking,
        FsmResuming,
        FsmStopping,
        FsmStopped,
        FsmClosing
};

/*---------------------------------------------------------------------------*/
/*                          FUNCTION DEFINITION                              */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : 创建CreateMediaPlayer状态机 
//  Global resource dependence :  none
//  Author: 
//  Note: 不要在callback中调用
/*****************************************************************************/
PUBLIC  HVIDEOHANDLE  CreateVideoPlayer(VideoProc vpCallBack)
{
	HVIDEOHANDLE               hVideo       =   0; 
	VIDEO_PLAYER   *fsm_ptr = NULL;//added by leichi 20120316

	CreateFsmMutex();

    if (MMIAPIUDISK_UdiskIsRun())
    {
        return 0;
    }
    else if(MMIAPISD_IsCardLogOn())
    {
        return 0;
    }
    else if (MMIAPIENG_GetIQModeStatus())
    {
        return 0;
    }
    else if (MMIAPICC_IsInState(CC_IN_CALL_STATE))
    {
        return 0;
    }

    //参数检查
    if(PNULL == vpCallBack)
    {
        //WMP_TRACE_LOW:"[Wre Player] CreateMediaPlayer ui callback is null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_769_112_2_18_3_11_6_238,(uint8*)"");
        return 0;
    }
    
    //创建状态机，并将状态机指针做为handle
    hVideo = CreateFSM(vpCallBack);
    if (0 == hVideo)
    {
        //WMP_TRACE_LOW:"[Wre Player] CreateMediaPlayer create  fail"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_777_112_2_18_3_11_6_239,(uint8*)"");
        return 0;
    }    
    //WMP_TRACE_LOW:"[Wre Player] CreateMediaPlayer create handle = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_780_112_2_18_3_11_6_240,(uint8*)"d",hVideo);
    //向 control发送消息
    if(!FsmDispatch(hVideo,FSM_INIT_REQ,PNULL,0))
    {
        //WMP_TRACE_LOW:"[Wre Player] CreateMediaPlayer dispath msg fail"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_784_112_2_18_3_11_6_241,(uint8*)"");
        DestroyFSM(hVideo);
        hVideo = 0;
    }    
    s_current_fsm = (VIDEO_PLAYER *)hVideo;  
	
    SetBlockType(WMP_HALF_TRANSPARENCE);      
    //停止背景播放
    //MMIAUDIO_PauseBgPlay(MMIBGPLAY_MODULE_MPEG4);
    //     MMIDEFAULT_TurnOnBackLight();
    //     MMIDEFAULT_AllowTurnOffBackLight(FALSE); 
    //MMIAUDIO_EnableKeyRing(MMIAUDIO_KEY_ALL, MMIBGPLAY_MODULE_MPEG4, FALSE);    
 #ifdef MMISRV_AUDIO_SUPPORT  //modified  by leichi 20120316
	fsm_ptr = (VIDEO_PLAYER *)hVideo;
	if (fsm_ptr)
	{
		fsm_ptr->srv_handle = MMISRVAUD_ReqVirtualHandle(WRE_VIDEOPLAYER, MMISRVAUD_PRI_NORMAL);//added by leichi 20120316
	}
#else//the following code is invalide
	//MMISRVAUD_ReqVirtualHandle(WRE_VIDEOPLAYER, MMISRVAUD_PRI_NORMAL);
	MMIAPISET_StopAllRing(FALSE);
#endif
	SetVideoPlayerVolume(hVideo,WRE_MEDIA_CURRENT_VOLUME);//added 20120613
       SetVideoPlayerBrightness(hVideo,MMIAPISET_GetMainLCDContrast());
    //MMIAPIBT_A2dpSetAppInfo(GetBtParam());//@andy.he  cr234137 20110623
    //返回 handle
    return hVideo;    
}

/*****************************************************************************/
//  Description : 去活PDP；销毁播放器状态机
//  Global resource dependence :  none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void DestroyVideoPlayer(HVIDEOHANDLE   hVideo)
{
    //恢复背景播放
    //MMIAUDIO_ResumeBgPlay(MMIBGPLAY_MODULE_MPEG4);
	//     MMIDEFAULT_AllowTurnOffBackLight(TRUE); 
    //MMIAUDIO_EnableKeyRing(MMIAUDIO_KEY_ALL, MMIBGPLAY_MODULE_MPEG4, TRUE);

    //WMP_TRACE_LOW:"[Wre Player] DestroyVideoPlayer hVideo = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_815_112_2_18_3_11_6_242,(uint8*)"d",hVideo);
    //检查hVideo的有效性；    
    if (0 == hVideo || !IsValidFsm((VIDEO_PLAYER *)hVideo))
    {
        //WMP_TRACE_LOW:"[Wre Player] DestroyVideoPlayer some wrong with hVideo = %x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_819_112_2_18_3_11_6_243,(uint8*)"d",hVideo);
        return; 
    }
#ifdef MMI_GPRS_SUPPORT
    //去活pdp;
    //MMIAPIPDP_Deactive(hVideo);   
#endif   
    
    //向 control发送消息
    if(!FsmDispatch(hVideo,FSM_RELEASE_REQ, PNULL,0))
    {
        ExitVideo(hVideo);
    }

    MMIAPIBT_A2dpStopDevice(MMIBT_A2DP_VIDEO_PLAYER);
    MMIAPIBT_A2dpClearAppInfo(MMIBT_A2DP_VIDEO_PLAYER);	
    GUIBLOCK_ResetMainType();
    //停止背景播放
    //MMIAUDIO_ResumeBgPlay(MMIBGPLAY_MODULE_MPEG4);
    MMIDEFAULT_AllowTurnOffBackLight(TRUE); 
    //MMIAUDIO_EnableKeyRing(MMIAUDIO_KEY_ALL, MMIBGPLAY_MODULE_MPEG4, TRUE);
    MMISRVAUD_FreeVirtualHandle(WRE_VIDEOPLAYER);//added by leichi 20120319
    MMIAPISET_ResetContrast(GUI_MAIN_LCD_ID);   
    //DestroyFsmMutex();
}

/*****************************************************************************/
//  Description : 发起视频播放（本地或网络）
//  Global resource dependence :  none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN PlayVideo(HVIDEOHANDLE   hVideo, VIDEO_PLAY_PARAM  *playParam)
{   
    BOOLEAN                 result          =   FALSE;
	//检查hVideo的有效性；	
    if (0 == hVideo || !IsValidFsm((VIDEO_PLAYER *)hVideo) || PNULL == playParam)
    {
        //WMP_TRACE_LOW:"[Wre Player] PlayVideo some wrong with hVideo = %x,playParam = %x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_855_112_2_18_3_11_6_244,(uint8*)"dd",hVideo,playParam);
        return FALSE; 
    }

	//WMP_TRACE_LOW:"[Wre Player] PlayVideo begin"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_859_112_2_18_3_11_6_245,(uint8*)"");
	//向 control发送消息
	result = FsmDispatch(hVideo,FSM_PLAY_REQ, playParam,sizeof(VIDEO_PLAY_PARAM));
	//WMP_TRACE_LOW:"[Wre Player] PlayVideo result = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_862_112_2_18_3_11_6_246,(uint8*)"d",result);
	if (!result)
	{
		SendPlayCnfToUI((VIDEO_PLAYER *)hVideo,VIDEO_ERROR_OTHERS);
	}
	return result;
	    
}

/*****************************************************************************/
//  Description : 终止视频播放（本地或网络）。
//  Global resource dependence :  none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN StopVideo(HVIDEOHANDLE   hVideo)
{  
    BOOLEAN              result         =   FALSE;
    VIDEO_PLAYER           *fsm_ptr        = (VIDEO_PLAYER *)hVideo;
    
    //检查hVideo的有效性；
    if (0 == hVideo || !IsValidFsm(fsm_ptr))
    {
        //WMP_TRACE_LOW:"[Wre Player] StopVideo some wrong with hVideo = %x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_883_112_2_18_3_11_6_247,(uint8*)"d",hVideo);
        return FALSE; 
    } 

    //WMP_TRACE_LOW:"[Wre Player] StopVideo begin"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_887_112_2_18_3_11_6_248,(uint8*)"");
    result = FsmDispatch(hVideo,FSM_STOP_REQ, PNULL,0);    
    //WMP_TRACE_LOW:"[Wre Player] StopVideo result = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_889_112_2_18_3_11_6_249,(uint8*)"d",result);
    
    return result;
}

/*****************************************************************************/
//  Description : 暂停视频播放（本地或网络）。
//  Global resource dependence :  none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN PauseVideo(HVIDEOHANDLE   hVideo)
{   
	BOOLEAN               result          =   FALSE;
	VIDEO_PLAYER             *fsm_ptr       = (VIDEO_PLAYER *)hVideo;

	//检查fsm_handler的有效性；
	if (0 == hVideo || !IsValidFsm(fsm_ptr))
	{
		WMP_TRACE_LOW("[Wre Player] PauseVideo some wrong with hVideo = %x",hVideo);
		return FALSE; 
	}  
	MMISRVAUD_FreeVirtualHandle(WRE_VIDEOPLAYER);//added by leichi 20120316
	//向 control发送消息
	result = FsmDispatch(hVideo,FSM_PAUSE_REQ, PNULL,0);     

	WMP_TRACE_LOW("[Wre Player] PauseVideo result = %d",result);
	if (!result && !fsm_ptr->is_to_pause)
	{
		SendPauseCnfToUI(fsm_ptr,VIDEO_ERROR_PAUSE_ERROR);
	}
	return result;
}

/*****************************************************************************/
//  Description : 恢复视频播放（本地或网络）
//  Global resource dependence :  none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN ResumeVideo(HVIDEOHANDLE  hVideo)
{   
	BOOLEAN                 result          =   FALSE;
	VIDEO_PLAYER             *fsm_ptr       = (VIDEO_PLAYER *)hVideo;
	unsigned long pdwVolume = 0;

	//检查hVideo的有效性；
	if (0 == hVideo || !IsValidFsm(fsm_ptr))
	{
		WMP_TRACE_LOW("[Wre Player] ResumeVideo some wrong with hVideo = %x",hVideo);
		return FALSE; 
	}
	fsm_ptr->srv_handle = MMISRVAUD_ReqVirtualHandle(WRE_VIDEOPLAYER, MMISRVAUD_PRI_NORMAL);//added by leichi :20120316
	if(GetVideoPlayerVolume(hVideo,  &pdwVolume))//added by leichi 20120315,it's necessory
	{
		SetVideoPlayerVolume(hVideo,pdwVolume);
	}

	WMP_TRACE_LOW("[Wre Player] ResumeVideo begin");   
	//向 control发送消息
	result = FsmDispatch(hVideo,FSM_RESUME_REQ, PNULL,0);   
	WMP_TRACE_LOW("[Wre Player] ResumeVideo result = %d",result);
	if (!result && !fsm_ptr->is_to_resume)
	{
		SendPlayCnfToUI(fsm_ptr,VIDEO_ERROR_RESUME_ERROR);
	}
	return result;
}

/*****************************************************************************/
//  Description : 进行定位视频播放（本地或网络）。
//  Global resource dependence :  none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN SeekVideo(HVIDEOHANDLE   hVideo,VIDEO_SEEK_PARAM  *seek_param_ptr)
{
    BOOLEAN                 result          = FALSE;
  
    //检查hVideo的有效性；
    if (0 == hVideo || !IsValidFsm((VIDEO_PLAYER *)hVideo) || PNULL == seek_param_ptr)
    {
        //WMP_TRACE_LOW:"[Wre Player] SeekVideo some wrong with hVideo = %x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_959_112_2_18_3_11_6_256,(uint8*)"d",hVideo);
        return FALSE; 
    }    
    //WMP_TRACE_LOW:"[Wre Player] SeekVideo begin"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_962_112_2_18_3_11_6_257,(uint8*)"");
    //向 control发送消息
    result = FsmDispatch(hVideo,FSM_SEEK_REQ, seek_param_ptr,sizeof(VIDEO_SEEK_PARAM));
    
    //WMP_TRACE_LOW:"[Wre Player] SeekVideo result = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_966_112_2_18_3_11_6_258,(uint8*)"d",result);
    if (!result)
    {
        SendSeekCnfToUI((VIDEO_PLAYER *)hVideo,VIDEO_ERROR_SEEK_ERROR);
    }
    return result;
}
/*****************************************************************************/
//  Description : 执行退出流程，并销毁状态机。
//  Global resource dependence :  none
//  Author: 
//  Note: 满足一键退出的需求。
/*****************************************************************************/
LOCAL void ExitVideo(HVIDEOHANDLE  hVideo)
{
    VIDEO_PLAYER * fsm_ptr = (VIDEO_PLAYER *)hVideo;
    //检查hVideo的有效性；
    if (PNULL == hVideo || !IsValidFsm(fsm_ptr))
    {
        //WMP_TRACE_LOW:"[Wre Player] ExitVideo the fsm is wrong hVideo = %x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_983_112_2_18_3_11_7_259,(uint8*)"d",hVideo);
        return;
    }    
    //设置强制退出标志
    fsm_ptr->is_to_exit = TRUE;
    //停止当前播放
    StopVideo(hVideo);      
}

/*****************************************************************************/
//  Description : 强制暂停
//  Global resource dependence :  none
//  Author: 
//  Note: 满足lose focus暂停的需求。
/*****************************************************************************/
PUBLIC void CompulsivePauseVideo(HVIDEOHANDLE  hVideo)
{
   VIDEO_PLAYER * fsm_ptr = (VIDEO_PLAYER *)hVideo;
    //检查hVideo的有效性；
    if (PNULL == hVideo || !IsValidFsm(fsm_ptr))
    {
		WMP_TRACE_LOW("[Wre Player] CompulsivePauseVideo the fsm is wrong hVideo = %x",hVideo);
		return;
	}
	MMISRVAUD_FreeVirtualHandle(WRE_VIDEOPLAYER); //added by leichi 20120316
	WMP_TRACE_LOW("[Wre Player] CompulsivePauseVideo begin fsm_ptr->is_to_resume = %d",fsm_ptr->is_to_resume);
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_1006_112_2_18_3_11_7_261,(uint8*)"d",fsm_ptr->is_to_resume);
    if(fsm_ptr->is_to_resume)
    {
       fsm_ptr->is_to_resume = FALSE;
    }
    else if(MAX_INVALID_VALUE == fsm_ptr->total_time && VIDEO_STATE_PLAYPENDING <= fsm_ptr->state)
    {        
        LCD_VIDEO_DISPLAY_PARAM dis_param = {0};
        SCI_MEMCPY(&dis_param,&(fsm_ptr->LcdDisplayParam),sizeof(LCD_VIDEO_DISPLAY_PARAM));
        dis_param.videoSupport = 0;
        SwitchDisplayMode(hVideo,&dis_param,FALSE);        
    }
    else
    {
        fsm_ptr->is_to_pause = TRUE;        
        PauseVideo(hVideo);
    }   
    //WMP_TRACE_LOW:"[Wre Player] CompulsivePauseVideo end fsm_ptr->is_to_pause = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_1023_112_2_18_3_11_7_262,(uint8*)"d",fsm_ptr->is_to_pause);
}

/*****************************************************************************/
//  Description : 强制恢复
//  Global resource dependence :  none
//  Author: 
//  Note: 满足get focus恢复的需求。
/*****************************************************************************/
PUBLIC void CompulsiveResumeVideo(HVIDEOHANDLE  hVideo)
{
    VIDEO_PLAYER * fsm_ptr = (VIDEO_PLAYER *)hVideo;
    //GUI_RECT_T lcd_rect = {0};
    //GUI_LCD_DEV_INFO const *lcd_dev_info_ptr = (const GUI_LCD_DEV_INFO*)MMITHEME_GetDefaultLcdDev();
    BOOLEAN is_need_fill = TRUE;
    DPLAYER_RETURN_E  dplayer_result=SCI_SUCCESS;//andy.he cr240364 20110620

    //检查hVideo的有效性；
    if (PNULL == hVideo || !IsValidFsm(fsm_ptr))
    {
        WMP_TRACE_LOW("[Wre Player] CompulsiveResumeVideo the fsm is wrong hVideo = %x",hVideo);
        return;
    }
	fsm_ptr->srv_handle = MMISRVAUD_ReqVirtualHandle(WRE_VIDEOPLAYER,  MMISRVAUD_PRI_NORMAL);//added by leichi 20120316
    WMP_TRACE_LOW("[Wre Player] CompulsiveResumeVideo begin fsm_ptr->is_to_pause = %d",fsm_ptr->is_to_pause);
    if(fsm_ptr->is_to_pause)
    {
        fsm_ptr->is_to_pause = FALSE;  
        if (VIDEO_STATE_PLAYPENDING == fsm_ptr->state
            || VIDEO_STATE_SEEKING == fsm_ptr->state
            || VIDEO_STATE_PAUSEDSEEKING == fsm_ptr->state)
        {
            MMIAPISET_InitAudioDevice(fsm_ptr->vol);               
            dplayer_result=DPLAYER_ResumeEx(fsm_ptr->dplayer_handle); //andy.he cr240364 20110620  
            SwitchDisplayMode((HVIDEOHANDLE)fsm_ptr,&(fsm_ptr->LcdDisplayParam),TRUE);    
            SwitchPlayDevice((HVIDEOHANDLE)fsm_ptr,fsm_ptr->bSupportA2dp,fsm_ptr->is_bt_support);
            if(SCI_SUCCESS!=dplayer_result)//andy.he cr240364 20110620
            {
                SendFatalErrorIndTOUI(fsm_ptr,VIDEO_ERROR_OTHERS);
                StopVideo((HVIDEOHANDLE)fsm_ptr);  
                return;
            }
        }
    }
    else if(MAX_INVALID_VALUE == fsm_ptr->total_time && VIDEO_STATE_PLAYPENDING <= fsm_ptr->state)
    {
        LCD_VIDEO_DISPLAY_PARAM display_param = {0};
        SCI_MEMCPY(&display_param,&(fsm_ptr->LcdDisplayParam),sizeof(LCD_VIDEO_DISPLAY_PARAM));
        fsm_ptr->LcdDisplayParam.videoSupport = 1;

        if (VIDEO_SUPPORTED_NEITHER == fsm_ptr->support_type|| VIDEO_SUPPORTED_AUDIO_ONLY == fsm_ptr->support_type)
        {
             is_need_fill = FALSE;
        }

        if (VIDEO_STATE_READY != fsm_ptr->state && VIDEO_STATE_PAUSING != fsm_ptr->state && VIDEO_STATE_PAUSED != fsm_ptr->state)
        {
            DPLAYER_Pause(fsm_ptr->dplayer_handle);
            SwitchPlayDevice((HVIDEOHANDLE)fsm_ptr,fsm_ptr->bSupportA2dp,fsm_ptr->is_bt_support);
        }         

        TranslateDisplayParamLogicToPhysical(&display_param, is_need_fill);

        if(DPLAYER_SUCCESS == DPLAYER_SetDisplayParam(fsm_ptr->dplayer_handle, &display_param))
        {   
          //  DPLAYER_UpdateCurrentFrame(DPLAYER_NORMAL_HANDLE);
        } 
        if (VIDEO_STATE_READY != fsm_ptr->state && VIDEO_STATE_PAUSING != fsm_ptr->state && VIDEO_STATE_PAUSED != fsm_ptr->state)
        {
            MMIAPISET_InitAudioDevice(fsm_ptr->vol);  
            SwitchPlayDevice((HVIDEOHANDLE)fsm_ptr,fsm_ptr->bSupportA2dp,fsm_ptr->is_bt_support);
            DPLAYER_Resume(fsm_ptr->dplayer_handle);            
        }   
        HandleUpdateFrameReq(fsm_ptr);
    }
    else
    {
        fsm_ptr->is_to_resume = TRUE; 
        ResumeVideo(hVideo);       
    }
    //WMP_TRACE_LOW:"[Wre Player] CompulsiveResumeVideo end fsm_ptr->is_to_resume = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_1100_112_2_18_3_11_7_265,(uint8*)"d",fsm_ptr->is_to_resume);

	if (VIDEO_STATE_READY != fsm_ptr->state)
	{  
		MMIDEFAULT_TurnOnBackLight();
		MMIDEFAULT_AllowTurnOffBackLight(FALSE);           
	}
}

/*****************************************************************************/
//  Description : 获取当前播放状态
//  Global resource dependence :  none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC VIDEO_STATE GetVideoPlayerState(HVIDEOHANDLE    hVideo)
{
    VIDEO_PLAYER       *fsm_ptr = (VIDEO_PLAYER *)hVideo;
    VIDEO_STATE        state   = VIDEO_STATE_MAX;
    
    if (PNULL != fsm_ptr && IsValidFsm(fsm_ptr))
    {
        state =  fsm_ptr->state;
    }
    
    //WMP_TRACE_LOW:"[Wre Player] GetMediaPlayerState state = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_1123_112_2_18_3_11_7_266,(uint8*)"d",state);
    return state;
    
}

/*****************************************************************************/
//  Description : 停止进行加快速率视频播放本地或网络
//  Global resource dependence :  none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN StopSetRate(HVIDEOHANDLE   hVideo,BOOLEAN is_backward)
{   
    BOOLEAN                 result          = FALSE;    
    
    //检查hVideo的有效性；
    if (0 == hVideo || !IsValidFsm((VIDEO_PLAYER *)hVideo))
    {
        //WMP_TRACE_LOW:"[Wre Player] StopSetRate some wrong with hVideo = %x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_1139_112_2_18_3_11_7_267,(uint8*)"d",hVideo);
        return FALSE; 
    }    
    //WMP_TRACE_LOW:"[Wre Player] StopSetRate begin"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_1142_112_2_18_3_11_7_268,(uint8*)"");
    //向 control发送消息
    result = FsmDispatch(hVideo,FSM_STOP_SET_RATE, &is_backward,sizeof(BOOLEAN));
    
    //WMP_TRACE_LOW:"[Wre Player] StopSetRate result = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_1146_112_2_18_3_11_7_269,(uint8*)"d",result);
    return result;
}

/*****************************************************************************/
//  Description : 进行加快速率视频播放本地或网络
//  Global resource dependence :  none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN StartSetRate(HVIDEOHANDLE   hVideo,BOOLEAN   is_backward)
{
   
    BOOLEAN                 result          = FALSE;
    //检查hVideo的有效性；
    if (0 == hVideo || !IsValidFsm((VIDEO_PLAYER *)hVideo))
    {
        //WMP_TRACE_LOW:"[Wre Player] StartSetRate some wrong with hVideo = %x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_1161_112_2_18_3_11_7_270,(uint8*)"d",hVideo);
        return FALSE; 
    }    
    //WMP_TRACE_LOW:"[Wre Player] StartSetRate begin"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_1164_112_2_18_3_11_7_271,(uint8*)"");
    //向 control发送消息
    result = FsmDispatch(hVideo,FSM_START_SET_RATE, &is_backward,sizeof(BOOLEAN));
    
    //WMP_TRACE_LOW:"[Wre Player] StartSetRate result = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_1168_112_2_18_3_11_7_272,(uint8*)"d",result);
    return result;
}

/*****************************************************************************/
//  Description : 设置音量
//  Global resource dependence :  none
//  Author: 
//  Note: 
/*****************************************************************************/
 
PUBLIC BOOLEAN SetVideoPlayerVolume(HVIDEOHANDLE hVideo,unsigned long  dwVolume)
{	
	BOOL bRet = FALSE;

	VIDEO_PLAYER   *fsm_ptr = (VIDEO_PLAYER *)hVideo;
	if (PNULL == fsm_ptr || !IsValidFsm(fsm_ptr))
	{
		return  FALSE;
	}
	SCI_TRACE_LOW("leichi : setvideoPlayerVolume22  = %d ;fsm_ptr->srv_handle = 0x%x",dwVolume,fsm_ptr->srv_handle);
	
	if(NULL == fsm_ptr->srv_handle)
	{
		fsm_ptr->srv_handle = MMISRVAUD_ReqVirtualHandle(WRE_VIDEOPLAYER,  MMISRVAUD_PRI_NORMAL);//added by leichi 20120612 
	}
	bRet = MMISRVAUD_SetVolume(fsm_ptr->srv_handle, dwVolume);
	if(bRet)
	{
		fsm_ptr->vol = dwVolume;		
		WRE_MEDIA_CURRENT_VOLUME = dwVolume;
		MMI_WriteNVItem(MMIWRE_NV_MEDIA_VOLUME,&WRE_MEDIA_CURRENT_VOLUME);
	}
	SCI_TRACE_LOW("leichi : setvideoPlayerVolume33  = %d  bRet = %d",dwVolume,bRet);
	return bRet;

}

/*****************************************************************************/
//  Description : 音量
//  Global resource dependence :  none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN GetVideoPlayerVolume(HVIDEOHANDLE hVideo,unsigned long *pdwVolume)
{
	VIDEO_PLAYER   *fsm_ptr  = (VIDEO_PLAYER *)hVideo;
	if ((PNULL == fsm_ptr) || (!IsValidFsm(fsm_ptr)))
	{
		return FALSE;
	}
	if(NULL == fsm_ptr->srv_handle)
	{
		fsm_ptr->srv_handle = MMISRVAUD_ReqVirtualHandle(WRE_VIDEOPLAYER,  MMISRVAUD_PRI_NORMAL);//added by leichi 20120612 
	}
	//*pdwVolume = MMISRVAUD_GetVolume(fsm_ptr->dplayer_handle);//del leichi
	//fsm_ptr->vol = *pdwVolume;//del leichi
	*pdwVolume = (unsigned long)fsm_ptr->vol;//add leichi
	SCI_TRACE_LOW("leichi fsm_ptr->vol = %d",fsm_ptr->vol);
	return TRUE;
}

/*****************************************************************************/
//  Description : 设置亮度
//  Global resource dependence :  none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN SetVideoPlayerBrightness(HVIDEOHANDLE hVideo,unsigned long dwBright)
{
    VIDEO_PLAYER   *fsm_ptr  = (VIDEO_PLAYER *)hVideo;
    
    if (PNULL == fsm_ptr || !IsValidFsm(fsm_ptr))
    {
        return FALSE;
	}
    //WmpSetting setting_info = {0};
    //setting_info = GetNVSettingInfo();
    //setting_info.brightness = brightness;
    //SetNVSettingInfo(setting_info);

	MMIAPISET_UpdateLCDContrast(GUI_MAIN_LCD_ID,dwBright); 
	return TRUE;
}

/*****************************************************************************/
// 	Description : 设置代理
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN SetVideoPlayerAgent(HVIDEOHANDLE hVideo, BOOLEAN bAgent,    //是否使用代理)
									unsigned short          uLowestPort,   //RTP/RTCP最低端口
									unsigned short          uHighestPort,  //RTP/RTCP最高端口    
									unsigned char         * strAgentIp,     //代理IP
									unsigned char           uAgentIpLenth,
									unsigned short          uAgentPort)    //代理端口
									 
{
    VIDEO_PLAYER   *fsm_ptr  = (VIDEO_PLAYER *)hVideo;
    
    if (PNULL == fsm_ptr || !IsValidFsm(fsm_ptr))
    {
        return FALSE;
	}

	fsm_ptr->setting_info.bAgent = bAgent;
	fsm_ptr->setting_info.uLowestPort = uLowestPort;
	fsm_ptr->setting_info.uHighestPort = uHighestPort;
	SCI_MEMCPY(fsm_ptr->setting_info.szAgentIp,strAgentIp,uAgentIpLenth);
	fsm_ptr->setting_info.uAgentIpLenth = uAgentIpLenth;
	fsm_ptr->setting_info.uAgentPort  =  uAgentPort;
	
	return TRUE;
}
/*****************************************************************************/
// 	Description : 取得亮度
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN GetVideoPlayerBrightness(HVIDEOHANDLE hVideo,unsigned long *pdwBright)
{
    VIDEO_PLAYER   *fsm_ptr  = (VIDEO_PLAYER *)hVideo;
    
    if (PNULL == fsm_ptr || !IsValidFsm(fsm_ptr))
    {
        return FALSE;
	}

	*pdwBright = MMIAPISET_GetMainLCDContrast();
	return TRUE; 
}

/*****************************************************************************/
//  Description : GetScreenAngle
//  Global resource dependence : 
//  Author: 
//  Note: 获得平台全局角度
/*****************************************************************************/
PUBLIC SCREEN_ANGLE GetScreenAngle( void )
{
	SCREEN_ANGLE screen_angle = 0;
	LCD_ANGLE_E  lcd_angle = 0;
	lcd_angle = MMK_GetScreenAngle();

	switch(lcd_angle)
	{
		case LCD_ANGLE_0:
			screen_angle = SCREEN_ANGLE_0;
			break;
		case LCD_ANGLE_90:
			screen_angle = SCREEN_ANGLE_90;
			break;
		case LCD_ANGLE_180:
			screen_angle = SCREEN_ANGLE_180;
			break;
		case LCD_ANGLE_270:
			screen_angle = SCREEN_ANGLE_270;
			break;
	}
	
    return screen_angle;
}

/*****************************************************************************/
//  Description : 获得视频播放的当前时间
//  Global resource dependence :  none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN GetVideoTimeOffset(HVIDEOHANDLE   hVideo,unsigned long *pdwCurTime)
{
    VIDEO_PLAYER       *fsm_ptr = (VIDEO_PLAYER *)hVideo;
    
    if (PNULL != fsm_ptr && IsValidFsm(fsm_ptr))
    {
        if (0 != fsm_ptr->dplayer_handle)
        {
            *pdwCurTime = DPLAYER_GetTime(fsm_ptr->dplayer_handle);
            fsm_ptr->process_time = *pdwCurTime;
			return TRUE;
        }
    }
	 //WMP_TRACE_LOW:"[Wre Player] GetVideoTimeOffset = %x"
	 SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_1320_112_2_18_3_11_7_273,(uint8*)"d",*pdwCurTime);
    return FALSE;
}

/*****************************************************************************/
//  Description : 获得视频播放的当前帧数。
//  Global resource dependence :  none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN GetVideoFrameOffset(HVIDEOHANDLE  hVideo,unsigned long *pdwFrameIndex)
{
    VIDEO_PLAYER                *fsm_ptr = (VIDEO_PLAYER *)hVideo;
    uint32                  time_offset = 0;
    //uint32                  frame_index = 0;
    DPLAYER_MEDIA_INFO_T    media_info = {0};
    DPLAYER_RETURN_E        info_result = DPLAYER_SUCCESS;
    
    if(PNULL != fsm_ptr && IsValidFsm(fsm_ptr))
    {
        if(0 < fsm_ptr->dplayer_handle)
        {
            info_result = DPLAYER_GetMovieInfo(fsm_ptr->dplayer_handle, &media_info);            
            if(DPLAYER_SUCCESS == info_result && 0 != media_info.video_info.frame_rate_base)
            {
                time_offset = DPLAYER_GetTime(fsm_ptr->dplayer_handle);
                *pdwFrameIndex = (media_info.video_info.frame_rate)*time_offset/SEC_TO_MSEL/media_info.video_info.frame_rate_base;
				return TRUE;
            }
        }
    }
    //WMP_TRACE_LOW:"[Wre Player] GetFrameOffset info_result = %d, *dwFrameIndex = %d, time_offset = %d, media_info.video_info.frame_rate = %d, base = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_1349_112_2_18_3_11_7_274,(uint8*)"ddddd",info_result,*pdwFrameIndex,time_offset,media_info.video_info.frame_rate,media_info.video_info.frame_rate_base);
    return FALSE;    
}

/*****************************************************************************/
//  Description : 切换屏幕
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN SwitchVideoPlayerScreen(HVIDEOHANDLE  hVideo,SCREEN_ANGLE screenangle,BOOLEAN bScaleUp,VIDEO_DISPLAY_RECT displayrect)
{
    LCD_VIDEO_DISPLAY_PARAM display_param = {0};  
	VIDEO_PLAYER        *  fsm_ptr    = (VIDEO_PLAYER *)hVideo;
	GUI_RECT_T      display_rect  = {0};   
	
	//int i = GUILCD_GetLogicAngle(GUI_MAIN_LCD_ID);
	//GUILCD_SetLogicAngle(GUI_MAIN_LCD_ID,screenangle);
	//display_rect = MMITHEME_GetFullScreenRect();  
	
    //检查hVideo的有效性；
    if (PNULL == fsm_ptr || !IsValidFsm(fsm_ptr))
    {
        //WMP_TRACE_LOW:"[Wre Player] SwitchWmpScreen the fsm is wrong fsm_ptr = %x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_1370_112_2_18_3_11_7_275,(uint8*)"d",fsm_ptr);
        return FALSE;
    } 
	
	display_param.RotateAngle = screenangle;   
	display_rect.left =  displayrect.x;
	display_rect.right=  displayrect.x+displayrect.dx;
	display_rect.top  =  displayrect.y;
	display_rect.bottom = displayrect.y+displayrect.dy;
	
    fsm_ptr->is_scal_up = bScaleUp;        
	display_param.disp_rect.dy = display_rect.bottom - display_rect.top + 1;
	display_param.disp_rect.dx = display_rect.right - display_rect.left + 1;            
	display_param.disp_rect.x = display_rect.left;
	display_param.disp_rect.y = display_rect.top;  
	
    display_param.target_rect.x = display_param.disp_rect.x;
    display_param.target_rect.y = display_param.disp_rect.y;
    display_param.target_rect.dx = display_param.disp_rect.dx;
    display_param.target_rect.dy = display_param.disp_rect.dy; 
	
    return SwitchDisplayMode(hVideo,&display_param,TRUE); 
}

/*****************************************************************************/
//  Description : 切换播放设备
//  Global resource dependence :  none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN SwitchPlayDevice(HVIDEOHANDLE  hVideo,
                                   BOOLEAN    bSupportA2dp,
                                   BOOLEAN    is_bt_support)
{
    // BOOLEAN                 result          =   TRUE;
    VIDEO_PLAYER                *fsm_ptr        = (VIDEO_PLAYER *)hVideo;
    DPLAYER_AUDIO_PARAM_T    audio_param    = {0};


    //检查hVideo的有效性；
    if (PNULL == fsm_ptr || !IsValidFsm(fsm_ptr))
    {
        //SCI_TRACE_LOW:"[Wre Player] SwitchPlayDevice the fsm is wrong fsm_ptr = %x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_1410_112_2_18_3_11_7_276,(uint8*)"d",fsm_ptr);
        return FALSE;
    }

    //SCI_TRACE_LOW:"[Wre Player] SwitchPlayDevice is_bt_support = %d, bSupportA2dp = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_1414_112_2_18_3_11_7_277,(uint8*)"dd", is_bt_support,bSupportA2dp);
        
    if (is_bt_support)
    {
        audio_param.output_device = DPLAYER_AUD_DEV_BT;
    }
    else
    {
        audio_param.output_device = DPLAYER_AUD_DEV_SPEAKER;
    }
    audio_param.output_device_info.bt_info.b_a2dp_support = bSupportA2dp;

    fsm_ptr->bSupportA2dp = bSupportA2dp;
    fsm_ptr->is_bt_support = is_bt_support;

    if (0 != fsm_ptr->dplayer_handle)
    {
        DPLAYER_SetAudioParam(fsm_ptr->dplayer_handle, &audio_param);
    }

    return TRUE;
}

/*****************************************************************************/
//  Description : 发起鉴权认证
//  Global resource dependence :  none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN AuthRsp(HVIDEOHANDLE   hVideo,
					  VIDEO_AUTH_INFO  *auth_info_ptr)
{
	
    BOOLEAN              result         =   FALSE;
    VIDEO_PLAYER             *fsm_ptr       = (VIDEO_PLAYER *)hVideo;
    
    //检查hVideo的有效性；
    if (0 == hVideo || !IsValidFsm(fsm_ptr) || PNULL == auth_info_ptr)
    {
        //WMP_TRACE_LOW:"[Wre Player] AuthRsp some wrong with hVideo = %x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_1451_112_2_18_3_11_8_278,(uint8*)"d",hVideo);
        return FALSE; 
    }
    
    //向 control发送消息
    result = FsmDispatch(hVideo,FSM_AUTH_RSP, auth_info_ptr,sizeof(VIDEO_AUTH_INFO));   
    //WMP_TRACE_LOW:"[Wre Player] AuthRsp result = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_1457_112_2_18_3_11_8_279,(uint8*)"d",result);
    return result;
}

/*****************************************************************************/
//  Description : 获得视频信息
//  Global resource dependence :  none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void GetMediaInfo(HVIDEOHANDLE  hVideo,DPLAYER_MEDIA_INFO_T    *media_info_ptr)
{
    VIDEO_PLAYER       *fsm_ptr = (VIDEO_PLAYER *)hVideo;
    
    if (PNULL == fsm_ptr || !IsValidFsm(fsm_ptr) || PNULL == media_info_ptr)
    {
        //WMP_TRACE_LOW:"[Wre Player] GetMediaInfo param is error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_1471_112_2_18_3_11_8_280,(uint8*)"");
        return;
    }

    if(0 < fsm_ptr->dplayer_handle)
    {
        DPLAYER_GetMovieInfo(fsm_ptr->dplayer_handle, media_info_ptr);
    }    
}

/*****************************************************************************/
// Description : 获取播放媒体信息
//	Global resource dependence : none
// Author: 
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN GetCurrentMediaInfo(HVIDEOHANDLE hVideo,MEDIA_INFO *pMediaInfo)
{
	DPLAYER_MEDIA_INFO_T media_info = {0};
    VIDEO_PLAYER       *fsm_ptr = (VIDEO_PLAYER *)hVideo;
    
    if (PNULL == fsm_ptr || !IsValidFsm(fsm_ptr) || PNULL == pMediaInfo)
    {
        //WMP_TRACE_LOW:"[Wre Player] GetMediaInfo param is error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_1492_112_2_18_3_11_8_281,(uint8*)"");
        return FALSE;
    }
	
    if(0 < fsm_ptr->dplayer_handle)
    {
        if(DPLAYER_SUCCESS == DPLAYER_GetMovieInfo(fsm_ptr->dplayer_handle, &media_info))
		{
            switch(media_info.file_type)
			{
				case	DPLAYER_FILETYPE_3GP:
					pMediaInfo->fileType = VIDEO_FILE_3GP;
					break;
				case	DPLAYER_FILETYPE_MP4:
					pMediaInfo->fileType = VIDEO_FILE_MP4;
					break;
				case	DPLAYER_FILETYPE_AVI:
					pMediaInfo->fileType = VIDEO_FILE_AVI;
					break;
				case	DPLAYER_FILETYPE_FLV:
					pMediaInfo->fileType = VIDEO_FILE_FLV;
					break;
				case	DPLAYER_FILETYPE_RMVB:
                    pMediaInfo->fileType = VIDEO_FILE_RMVB;
                    break;
				case	DPLAYER_FILETYPE_MAX:
					pMediaInfo->fileType = VIDEO_FILE_MAX;
					break;
				default:
					pMediaInfo->fileType = VIDEO_FILE_MAX;
					break;
			}
			
			pMediaInfo->nMediaLength   = media_info.media_length;
			pMediaInfo->nStreamBitrate = media_info.stream_bitrate;

			switch(media_info.audio_info.audio_type)
			{
				case 	AUDIO_TYPE_AMR:
					pMediaInfo->audioInfo.audioType = AUDIO_DECODE_AMR;
					break;
				case	AUDIO_TYPE_PCM:
					pMediaInfo->audioInfo.audioType = AUDIO_DECODE_PCM;
					break;
				case	AUDIO_TYPE_IMAADPCM:
					pMediaInfo->audioInfo.audioType = AUDIO_DECODE_IMAADPCM;
					break;
				case	AUDIO_TYPE_MP3:
					pMediaInfo->audioInfo.audioType = AUDIO_DECODE_MP3;
					break;
				case	AUDIO_TYPE_DRA:
					pMediaInfo->audioInfo.audioType = AUDIO_DECODE_DRA;
					break;
				case	AUDIO_TYPE_AAC:
					pMediaInfo->audioInfo.audioType = AUDIO_DECODE_ACC;
					break;
				case	AUDIO_TYPE_MAX:
					pMediaInfo->audioInfo.audioType = AUDIO_DECODE_MAX;
					break;
				default:
					pMediaInfo->audioInfo.audioType = AUDIO_DECODE_MAX;
					break;
			}
			pMediaInfo->audioInfo.nAudioSupport = media_info.audio_info.audio_support;
			pMediaInfo->audioInfo.nBitrate      = media_info.audio_info.bitrate;
			pMediaInfo->audioInfo.nChannels     = media_info.audio_info.channels;
			pMediaInfo->audioInfo.nSampleRate   = media_info.audio_info.sample_rate;

			switch(media_info.video_info.video_type)
			{
			case 	VIDEO_TYPE_H263:
				pMediaInfo->videoInfo.videoType = VIDEO_DECODE_H263;
				break;
			case	VIDEO_TYPE_MP4V:
				pMediaInfo->videoInfo.videoType = VIDEO_DECODE_MP4;
				break;
			case	VIDEO_TYPE_MJPEG:
				pMediaInfo->videoInfo.videoType = VIDEO_DECODE_MJPEG;
				break;
			case	VIDEO_TYPE_H264:
				pMediaInfo->videoInfo.videoType = VIDEO_DECODE_H264;
				break;
			case	VIDEO_TYPE_MAX:
				pMediaInfo->videoInfo.videoType = VIDEO_DECODE_MAX;
				break;
			default:
				pMediaInfo->videoInfo.videoType = VIDEO_DECODE_MAX;
				break;
			}
			pMediaInfo->videoInfo.nBitrate      = media_info.video_info.bitrate;
			pMediaInfo->videoInfo.nFrameRate    = media_info.video_info.frame_rate;
			pMediaInfo->videoInfo.nFrameRateBase= media_info.video_info.frame_rate_base;
			pMediaInfo->videoInfo.nHeight       = media_info.video_info.height;
			pMediaInfo->videoInfo.nWidth        = media_info.video_info.width;
			pMediaInfo->videoInfo.nVideoSupport = media_info.video_info.video_support;

			return  TRUE;
		}
    }    
	
	return FALSE;
}

/*****************************************************************************/
//  Description : 通过全路径名获得媒体文件信息
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL void GetMediaInfoFromFullPathName(
										 const wchar            *full_file_name, 
										 uint16                  full_name_len,
										 DPLAYER_MEDIA_INFO_T    *media_ptr
										 )
{
    DPLAYER_RETURN_E    ret         = DPLAYER_SUCCESS;
    void                *argv[1]    = {0}; 
    DPLAYER_HANDLE       dplayer_handle = 0;

    if (PNULL == full_file_name || PNULL == media_ptr || 0 == full_name_len)
    {
        //WMP_TRACE_LOW:"[Wre Player]: GetMediaInfoFromFullPathName() param error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_1611_112_2_18_3_11_8_282,(uint8*)"");
        return;
    }   
    
    argv[0] = (void*)(full_file_name);
    DPLAYER_Init();
    dplayer_handle = DPLAYER_Open(DPLAYER_OPEN_FROM_FILE, 1, argv);
    
    if(0 >= dplayer_handle)
    {   
        //WMP_TRACE_LOW:"[Wre Player]: GetMediaInfoFromFullPathName: open dplayer fail! "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_1621_112_2_18_3_11_8_283,(uint8*)"");
        return;
    }

    
    ret = DPLAYER_GetMovieInfo(dplayer_handle, media_ptr);      
    DPLAYER_Close(dplayer_handle);
    DPLAYER_Release();
    //WMP_TRACE_LOW:"[Wre Player]: GetMediaInfoFromFullPathName() return = 0x%.2x, full_name_len = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_1629_112_2_18_3_11_8_284,(uint8*)"dd", ret,full_name_len);
    //WMP_TRACE_LOW:"[Wre Player]: GetMediaInfoFromFullPathName():ntfile_type = %d,ntlen = %d,ntv_spt = %d,ntf_rate = %d,ntheight = %d,ntwidth = %d,ntvideo_type = %d,ntaudio_type = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_1640_112_2_18_3_11_8_285,(uint8*)"dddddddd",media_ptr->file_type, media_ptr->media_length,media_ptr->video_info.video_support,media_ptr->video_info.frame_rate,media_ptr->video_info.height,media_ptr->video_info.width,media_ptr->video_info.video_type, media_ptr->audio_info.audio_type);

}

/*****************************************************************************/
//  Description : 通过视频buf中获得媒体文件信息
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL void GetMediaInfoFromBuf(
                     uint8                * video_buf,
                     uint32                  video_length,
                     VIDEO_FILE_TYPE             video_type,
                     DPLAYER_MEDIA_INFO_T   *media_ptr
                )
{
    DPLAYER_HANDLE          video_handle              = NULL;
    void                    *argv[3]            = {0}; 
    DPLAYER_FILE_TYPE_E     ref_file_type       = DPLAYER_FILETYPE_MAX;


    if (PNULL == video_buf || PNULL == media_ptr)
    {
        //WMP_TRACE_LOW:"[Wre Player]: GetMediaInfoFromBuf param error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_1662_112_2_18_3_11_8_286,(uint8*)"");
        return;
    }

    //WMP_TRACE_LOW:"[Wre Player]: GetMediaInfoFromBuf: video_type = %d, video_length = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_1666_112_2_18_3_11_8_287,(uint8*)"dd",video_type, video_length);
    switch(video_type)
    {

		case VIDEO_FILE_AVI:
            ref_file_type = DPLAYER_FILETYPE_AVI;
            break;
		case VIDEO_FILE_3GP:
            ref_file_type = DPLAYER_FILETYPE_3GP;
			break;
		case VIDEO_FILE_MP4:
			ref_file_type = DPLAYER_FILETYPE_MP4;
			break;
		case VIDEO_FILE_FLV:
			ref_file_type = DPLAYER_FILETYPE_FLV;
			break;
        case VIDEO_FILE_RMVB:
            ref_file_type = DPLAYER_FILETYPE_RMVB;
            break;
		default:
			ref_file_type = DPLAYER_FILETYPE_MAX;
			break;
    }
    
    if(ref_file_type < DPLAYER_FILETYPE_MAX)
    {
        argv[0] = (void*)(video_buf);   
        argv[1] = (void*)video_length;
        argv[2] = &ref_file_type;
        DPLAYER_Init();
        video_handle = DPLAYER_Open(DPLAYER_OPEN_FROM_MEM, 3, argv);
        
        if(0 == video_handle)
        {   
            return;
        }
    }
    else
    {
        return;
    }

    DPLAYER_GetMovieInfo(video_handle,media_ptr);   
    DPLAYER_Close(video_handle);    
    DPLAYER_Release();
}

/*****************************************************************************/
//  Description : Get the supported_type of a movie.
//  Global resource dependence :  none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC VIDEO_SUPPORTED_TYPE CheckSupportedTypeFromMediaInfo(DPLAYER_MEDIA_INFO_T *media_ptr)
{
    VIDEO_SUPPORTED_TYPE      supported_type  = VIDEO_SUPPORTED_TYPE_MAX;
    
    if (PNULL == media_ptr)
    {
        //WMP_TRACE_LOW:"[Wre Player] CheckSupportedTypeFromMediaInfo param is null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_1723_112_2_18_3_11_8_288,(uint8*)"");
        return VIDEO_SUPPORTED_NEITHER;
    }
    //WMP_TRACE_LOW:"[Wre Player] CheckSupportedTypeFromMediaInfo video_support = %d, audio_support = %d , video_type = %d, audio_type = %d, height = %d, width = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_1732_112_2_18_3_11_8_289,(uint8*)"dddddd", media_ptr->video_info.video_support,media_ptr->audio_info.audio_support,media_ptr->video_info.video_type,media_ptr->audio_info.audio_type,media_ptr->video_info.height,media_ptr->video_info.width);
	
    if (1 == media_ptr->audio_info.audio_support
        && 1 == media_ptr->video_info.video_support)
    {
        supported_type = VIDEO_SUPPORTED_BOTH;
    }
    else if (1 == media_ptr->audio_info.audio_support)
    {
        supported_type = VIDEO_SUPPORTED_AUDIO_ONLY;
    }
    else if(1 == media_ptr->video_info.video_support)
    {
        supported_type = VIDEO_SUPPORTED_VIDEO_ONLY;
    }
    else
    {
        supported_type = VIDEO_SUPPORTED_NEITHER;
    }   
    
    return supported_type;
}

/*****************************************************************************/
//  Description : 用于判断播放器对该文件的支持程度
//  Global resource dependence :  none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL VIDEO_SUPPORTED_TYPE GetSupportedStatus(unsigned short  * strFileName, unsigned short  nFileNameLen)
{
    DPLAYER_MEDIA_INFO_T media_info = {0};
    
    if (PNULL == strFileName || 0 == nFileNameLen)
    {
        //WMP_TRACE_LOW:"[Wre Player] GetSupportedStatus param is null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_1765_112_2_18_3_11_8_290,(uint8*)"");
        return VIDEO_SUPPORTED_NEITHER;
    }
    
    if (PNULL != s_current_fsm 
        && VIDEO_STATE_READY != s_current_fsm->state
        && nFileNameLen == s_current_fsm->uFilePathLen 
        && PNULL != s_current_fsm->strFilePath
        && 0 == MMIAPICOM_Wstrcmp(strFileName,s_current_fsm->strFilePath))
    {
        return s_current_fsm->support_type;
    }

    GetMediaInfoFromFullPathName(strFileName, nFileNameLen, &media_info);
    
    return CheckSupportedTypeFromMediaInfo(&media_info);
}

/*****************************************************************************/
//  Description : GetTransparenceColor
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL GUI_COLOR_T GetTransparenceColor(void)
{
	// if(MMISET_THEME_DEFAULT_TYPE == MMIAPISET_GetCurrentThemeType())
	// {
	return MMI_BLACK_COLOR;
	// }
	// else
	// {
    //    return MMI_WHITE_COLOR;
    //}    
}

/*****************************************************************************/
//  Description : 填充透明色
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN FillRectWithTransparenceColor(
                                           uint32 *width_ptr, //[IN/OUT]
                                           uint32 *height_ptr, //[IN/OUT]
                                           uint32 *x_offset_ptr, //[IN/OUT]
                                           uint32 * y_offset_ptr, //[IN/OUT]
                                           uint32 RotateAngle,
                                           BOOLEAN is_need_fill                                  
                                           )
{
    GUI_RECT_T lcd_rect = {0};
    GUI_LCD_DEV_INFO const* lcd_dev_info_ptr = (const GUI_LCD_DEV_INFO*)MMITHEME_GetDefaultLcdDev();
    GUILCD_COORDINATE_PARAM_T logic_coordinate = {0};
    GUILCD_COORDINATE_PARAM_T physical_coordinate = {0};
    uint16 temp = 0;
    LCD_ANGLE_E  logic_angle = GUILCD_GetLogicAngle(GUI_MAIN_LCD_ID);
   
    if (PNULL == width_ptr || PNULL == height_ptr || PNULL == x_offset_ptr || PNULL == y_offset_ptr)
    {
        return FALSE;
    }

    //WMP_TRACE_LOW:"[Wre Player] FillRectWithTransparenceColor before x= %d, y=%d,width = %d, height = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_1828_112_2_18_3_11_8_291,(uint8*)"dddd",*x_offset_ptr,*y_offset_ptr,*width_ptr,*height_ptr);
        
    *width_ptr = *width_ptr - GetAlign4Offset(*x_offset_ptr);        
    *width_ptr = *width_ptr - (*width_ptr % 4);
    *x_offset_ptr = *x_offset_ptr + GetAlign4Offset(*x_offset_ptr);
    *height_ptr = *height_ptr - GetAlign4Offset(*y_offset_ptr);
    *height_ptr = *height_ptr - (*height_ptr % 4);    
    *y_offset_ptr = *y_offset_ptr + GetAlign4Offset(*y_offset_ptr);
        
	//WMP_TRACE_LOW:"[Wre Player] FillRectWithTransparenceColor after x= %d, y=%d,width = %d, height = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_1841_112_2_18_3_11_8_292,(uint8*)"dddd",*x_offset_ptr,*y_offset_ptr,*width_ptr,*height_ptr);
    
    if (is_need_fill)
    {
        if (RotateAngle != logic_angle
             && !((LCD_ANGLE_0 == RotateAngle && LCD_ANGLE_180 == logic_angle)
            ||(LCD_ANGLE_180 == RotateAngle && LCD_ANGLE_0 ==logic_angle)))
        {
            lcd_rect.left = *y_offset_ptr;
            lcd_rect.top = *x_offset_ptr;        
            lcd_rect.right = *y_offset_ptr + *height_ptr;
            lcd_rect.bottom = *x_offset_ptr + *width_ptr; 
            
        }
        else
        {
            lcd_rect.left = *x_offset_ptr;
            lcd_rect.top = *y_offset_ptr;        
            lcd_rect.right = *x_offset_ptr + *width_ptr;
            lcd_rect.bottom = *y_offset_ptr + *height_ptr; 
            
        }
        GUI_FillRect(lcd_dev_info_ptr, lcd_rect,GetTransparenceColor()); 
    }
    
    logic_coordinate.angle = RotateAngle;
    logic_coordinate.lcd_height = *height_ptr;
    logic_coordinate.lcd_width = *width_ptr;
    logic_coordinate.rect.left = *x_offset_ptr;
    logic_coordinate.rect.right = *x_offset_ptr + *width_ptr-1;
    logic_coordinate.rect.top = *y_offset_ptr;
    logic_coordinate.rect.bottom = *y_offset_ptr + *height_ptr-1;  
    
    GUILCD_GetLogicWidthHeight(lcd_dev_info_ptr->lcd_id,&(logic_coordinate.lcd_width),&(logic_coordinate.lcd_height));
    
    if(RotateAngle != logic_angle
        && !((LCD_ANGLE_0 == RotateAngle && LCD_ANGLE_180 == logic_angle)
            ||(LCD_ANGLE_180 == RotateAngle && LCD_ANGLE_0 ==logic_angle))
            )
    {
        temp = logic_coordinate.lcd_width;
        logic_coordinate.lcd_width = logic_coordinate.lcd_height;
        logic_coordinate.lcd_height = temp;
    }

    GUILCD_ConvertLogicToPhysicalCoordinate(lcd_dev_info_ptr->lcd_id,
        &logic_coordinate,
        &physical_coordinate);

    *width_ptr = physical_coordinate.rect.right - physical_coordinate.rect.left + 1;
    *height_ptr = physical_coordinate.rect.bottom - physical_coordinate.rect.top + 1;
    *x_offset_ptr = physical_coordinate.rect.left;
    *y_offset_ptr = physical_coordinate.rect.top;
  

    //WMP_TRACE_LOW:"[Wre Player] FillRectWithTransparenceColor after x= %d, y=%d,width = %d, height = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_1900_112_2_18_3_11_8_293,(uint8*)"dddd",*x_offset_ptr,*y_offset_ptr,*width_ptr,*height_ptr);

    return TRUE;
}

/*****************************************************************************/
//  Description : 填充透明色
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN TranslateDisplayParamLogicToPhysical(LCD_VIDEO_DISPLAY_PARAM *disp_para, BOOLEAN is_need_fill)
{
    GUI_LCD_DEV_INFO const* lcd_dev_info_ptr = (const GUI_LCD_DEV_INFO*)MMITHEME_GetDefaultLcdDev();
    GUILCD_COORDINATE_PARAM_T logic_coordinate = {0};
    GUILCD_COORDINATE_PARAM_T physical_coordinate = {0};

    logic_coordinate.angle = disp_para->RotateAngle;
    logic_coordinate.lcd_height = disp_para->disp_rect.dy;
    logic_coordinate.lcd_width = disp_para->disp_rect.dx;

    FillRectWithTransparenceColor(&(disp_para->disp_rect.dx), &(disp_para->disp_rect.dy), &(disp_para->disp_rect.x), &(disp_para->disp_rect.y), disp_para->RotateAngle, is_need_fill);
    FillRectWithTransparenceColor(&(disp_para->target_rect.dx), &(disp_para->target_rect.dy), &(disp_para->target_rect.x), &(disp_para->target_rect.y), disp_para->RotateAngle, is_need_fill);
    GUILCD_ConvertLogicToPhysicalCoordinate(lcd_dev_info_ptr->lcd_id,
        &logic_coordinate,
        &physical_coordinate);
    
    disp_para->RotateAngle = physical_coordinate.angle;
    return TRUE;

}

/*****************************************************************************/
//  Description : GetCurBufPercent
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL uint32 GetCurBufPercent(HVIDEOHANDLE hVideo)
{
    VIDEO_PLAYER   *fsm_ptr  = (VIDEO_PLAYER *)hVideo;
    
    if (PNULL == fsm_ptr || !IsValidFsm(fsm_ptr))
    {
        return 100;
    }
    else
    {
        return fsm_ptr->buf_percent;
    }

}

#ifdef DRM_SUPPORT
/*****************************************************************************/
//  Description : consume drm right
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void ConsumeDrmRight(const wchar *name_path_ptr, 
                           uint16       name_len)
{
    wchar filename[MMIFILE_FULL_PATH_MAX_LEN+2] = {0}; 

    if(name_len > MMIFILE_FULL_PATH_MAX_LEN)
    {
        return;
    }

    MMIAPICOM_Wstrncpy(filename,name_path_ptr,name_len);
    MMIAPIDRM_ConsumeRights(SFS_INVALID_HANDLE, filename);
}
#endif

/*****************************************************************************/
//  Description : 用于处理其他task给jvm发送的消息
//  Global resource dependence :  none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  DispatchSignalToFsmMsg(PWND app_ptr, uint16 msg_id, DPARAM param)
{     
    MMI_RESULT_E            result              = MMI_RESULT_TRUE;
    void                   *signal_ptr          =  (void*)((uint32)param);
    FSM_MSG_E               req_msg_id              = 0;
    void                    *msg_body_ptr       = PNULL;
    uint32                  msg_body_size       = 0;
    VIDEO_PLAY_END_RESULT        play_end_result     = 0;    
	
   
    if (PNULL == s_current_fsm)
    {
        return MMI_RESULT_FALSE;
    }

    switch(msg_id) 
    {
#ifdef STREAMING_SUPPORT    
    case STRM_INIT_CNF:  
        //WMP_TRACE_LOW:"[Wre Player] DispatchSignalToFsmMsg STRM_INIT_CNF"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_1992_112_2_18_3_11_9_294,(uint8*)"");
        req_msg_id = FSM_STRM_INIT_CNF;
        msg_body_size = sizeof(STRM_INIT_CNF_T);
        msg_body_ptr = signal_ptr;  
        
        break;
        
    case STRM_DESCRIBE_AUTH_IND:
    case STRM_SETUP_AUTH_IND:
    case STRM_PLAY_AUTH_IND:        
        req_msg_id = FSM_AUTHIND_REQ;
        msg_body_size = 0;
        msg_body_ptr = PNULL;              
        break;
        
    case STRM_PLAY_CNF:
        {
            STRM_PLAY_CNF_T  *strm_play_cnf_ptr = (STRM_PLAY_CNF_T*)signal_ptr;
            //WMP_TRACE_LOW:"[Wre Player] DispatchSignalToFsmMsg STRM_PLAY_CNF"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_2010_112_2_18_3_11_9_295,(uint8*)"");
            if (PNULL != strm_play_cnf_ptr)
            {
                req_msg_id = FSM_STRM_PLAY_CNF;
                msg_body_size = sizeof(STRM_RESULT_E);
                msg_body_ptr = &(strm_play_cnf_ptr->result);
                
            }
            else
            {
                result = MMI_RESULT_FALSE;
            }
        }        
        break;
        
    case STRM_BUFFERING_IND:
        //WMP_TRACE_LOW:"[Wre Player] DispatchSignalToFsmMsg STRM_BUFFERING_IND"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_2026_112_2_18_3_11_9_296,(uint8*)"");
        req_msg_id = FSM_STRM_BUFFERING_IND;
        msg_body_size = 0;
        msg_body_ptr = PNULL;     
        s_current_fsm->buf_percent = 0;
        break;
        
    case STRM_BUFFER_END_IND:
        //WMP_TRACE_LOW:"[Wre Player] DispatchSignalToFsmMsg STRM_BUFFER_END_IND"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_2034_112_2_18_3_11_9_297,(uint8*)"");
        req_msg_id = FSM_STRM_BUFFER_END_IND;
        msg_body_size = 0;
        msg_body_ptr = PNULL;  
        s_current_fsm->buf_percent = 100;
        break;
        
    case STRM_DATA_TIMEOUT_IND:
        //WMP_TRACE_LOW:"[Wre Player] DispatchSignalToFsmMsg STRM_DATA_TIMEOUT_IND"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_2042_112_2_18_3_11_9_298,(uint8*)"");
        req_msg_id = FSM_STRM_DATA_TIMEOUT_IND;
        msg_body_size = 0;
        msg_body_ptr = PNULL;            
        break;       
   
    case STRM_BUFFERING_PROGRESS_IND:
        {
            STRM_BUFFERING_PROGRESS_IND_T *strm_buf_process_ptr = (STRM_BUFFERING_PROGRESS_IND_T*)signal_ptr;
            //WMP_TRACE_LOW:"[Wre Player] DispatchSignalToFsmMsg strm_buf_process_ptr = %x"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_2051_112_2_18_3_11_9_299,(uint8*)"d",strm_buf_process_ptr);
            if (PNULL != strm_buf_process_ptr
            && 0 != strm_buf_process_ptr->strm_buf_total)
            {
                //WMP_TRACE_LOW:"[Wre Player] DispatchSignalToFsmMsg process = %d, total = %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_2057_112_2_18_3_11_9_300,(uint8*)"dd",strm_buf_process_ptr->strm_buf_progress,strm_buf_process_ptr->strm_buf_total);
                s_current_fsm->buf_percent = strm_buf_process_ptr->strm_buf_progress * 100 /strm_buf_process_ptr->strm_buf_total;
            }
            req_msg_id = FSM_BUFFER_PERCENT;
            msg_body_size = 0;
            msg_body_ptr = PNULL;      
            //WMP_TRACE_LOW:"[Wre Player] DispatchSignalToFsmMsg percent = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_2064_112_2_18_3_11_9_301,(uint8*)"d",s_current_fsm->buf_percent);
        }
        break;

    case STRM_GOODBYE_IND:
        //WMP_TRACE_LOW:"[Wre Player] DispatchSignalToFsmMsg STRM_GOODBYE_IND"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_2069_112_2_18_3_11_9_302,(uint8*)"");
        req_msg_id = FSM_STRM_GOODBYE_IND;
        msg_body_size = 0;
        msg_body_ptr = PNULL;        
        break;
        
    case STRM_SEEK_CNF:
        {
            STRM_SEEK_CNF_T  *strm_seek_cnf_ptr = (STRM_SEEK_CNF_T*)signal_ptr;
            //WMP_TRACE_LOW:"[Wre Player] DispatchSignalToFsmMsg STRM_SEEK_CNF"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_2078_112_2_18_3_11_9_303,(uint8*)"");
            if (PNULL != strm_seek_cnf_ptr)
            {
                req_msg_id = FSM_STRM_SEEK_CNF;
                msg_body_size = sizeof(STRM_RESULT_E);
                msg_body_ptr = &(strm_seek_cnf_ptr->result);                
            }
            else
            {
                result = MMI_RESULT_FALSE;
            }
            
        }
        break;
        
    case STRM_PAUSE_CNF:
        {
            STRM_PAUSE_CNF_T  *strm_pause_cnf_ptr = (STRM_PAUSE_CNF_T*)signal_ptr;
            //WMP_TRACE_LOW:"[Wre Player] DispatchSignalToFsmMsg STRM_PAUSE_CNF"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_2096_112_2_18_3_11_9_304,(uint8*)"");
            if (PNULL != strm_pause_cnf_ptr)
            {
                req_msg_id = FSM_STRM_PAUSE_CNF;
                msg_body_size = sizeof(STRM_RESULT_E);
                msg_body_ptr = &(strm_pause_cnf_ptr->result); 
            }
            else
            {
                result = MMI_RESULT_FALSE;
            }
                
        }        
        break;
        
    case STRM_RESUME_CNF:
        {
            STRM_RESUME_CNF_T  *strm_resume_cnf_ptr = (STRM_RESUME_CNF_T*)signal_ptr;
            //WMP_TRACE_LOW:"[Wre Player] DispatchSignalToFsmMsg STRM_RESUME_CNF"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_2114_112_2_18_3_11_9_305,(uint8*)"");
            if (PNULL != strm_resume_cnf_ptr)
            {
                req_msg_id = FSM_STRM_RESUME_CNF;
                msg_body_size = sizeof(STRM_RESULT_E);
                msg_body_ptr = &(strm_resume_cnf_ptr->result); 
            }
            else
            {
                result = MMI_RESULT_FALSE;
            }   
        }           
        break;

    case STRM_STOP_CNF:
        {
            STRM_STOP_CNF_T  *strm_stop_cnf_ptr = (STRM_STOP_CNF_T*)signal_ptr;
            //WMP_TRACE_LOW:"[Wre Player] DispatchSignalToFsmMsg STRM_STOP_CNF"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_2131_112_2_18_3_11_9_306,(uint8*)"");
            if (PNULL != strm_stop_cnf_ptr)
            {
                req_msg_id = FSM_STRM_STOP_CNF;
                msg_body_size = sizeof(STRM_RESULT_E);
                msg_body_ptr = &(strm_stop_cnf_ptr->result); 
            }
            else
            {
                result = MMI_RESULT_FALSE;
            }
        }        
        break;

    case STRM_CLOSE_CNF:
        {
            STRM_CLOSE_CNF_T  *strm_close_cnf_ptr = (STRM_CLOSE_CNF_T*)signal_ptr;
            //WMP_TRACE_LOW:"[Wre Player] DispatchSignalToFsmMsg STRM_CLOSE_CNF"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_2148_112_2_18_3_11_9_307,(uint8*)"");
            if (PNULL != strm_close_cnf_ptr)
            {
                req_msg_id = FSM_STRM_CLOSE_CNF;
                msg_body_size = sizeof(STRM_RESULT_E);
                msg_body_ptr = &(strm_close_cnf_ptr->result); 
            }
            else
            {
                result = MMI_RESULT_FALSE;
            }
        }
        break;
#endif
    case  APP_WRE_VIDEO_FRAME_END_IND:
        //WMP_TRACE_LOW:"[Wre Player] DispatchSignalToFsmMsg APP_WMP_FRAME_END_IND"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_2163_112_2_18_3_11_9_308,(uint8*)"");
        req_msg_id = FSM_FRAME_END_IND;
        msg_body_size = 0;
        msg_body_ptr = PNULL;        
        break;
		
    case APP_WRE_VIDEO_END_IND:
        {
            MmiAppCopyMsgS *sendSignal = (MmiAppCopyMsgS *)signal_ptr;
            //WMP_TRACE_LOW:"[Wre Player] DispatchSignalToFsmMsg APP_WMP_END_IND"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_2172_112_2_18_3_11_9_309,(uint8*)"");
            if (PNULL != sendSignal)
            {
                play_end_result = (VIDEO_PLAY_END_RESULT)(sendSignal->data);
            }
            req_msg_id = FSM_CALLBACK_END_IND;
            msg_body_size = sizeof(VIDEO_PLAY_END_RESULT);
            msg_body_ptr = &play_end_result;              
        }
        break;
/*
    case APP_WMP_PLAY_NEXT_FILE_REQ:
        {
            MmiAppCopyMsgS *sendSignal = (MmiAppCopyMsgS *)signal_ptr;
            //WMP_TRACE_LOW:"[Wre Player] DispatchSignalToFsmMsg APP_WMP_PLAY_NEXT_FILE_REQ"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_2185_112_2_18_3_11_9_310,(uint8*)"");
            if (PNULL != sendSignal)
            {
                play_end_result = (VIDEO_PLAY_END_RESULT)(sendSignal->data);
            }
            req_msg_id    = FSM_PLAY_NEXT_FILE;
            msg_body_size = sizeof(VIDEO_PLAY_END_RESULT);
            msg_body_ptr  = &play_end_result;              
        }
        break;
*/
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    if (MMI_RESULT_TRUE == result)
    {
        //向 control发送消息
        result = FsmDispatch((HVIDEOHANDLE)s_current_fsm,req_msg_id,msg_body_ptr,msg_body_size);           
    }    
    //WMP_TRACE_LOW("[Wre Player] DispatchSignalToFsmMsg result = %d",result);
    return result;
}
/*****************************************************************************/
//  Description : 设置osd和透明色
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void SetBlockType(uint32 alpha)
{    
    GUI_COLOR_T             color_key       =    MMI_BLACK_COLOR;//GetTransparenceColor();
    GUIBLOCK_SetType(alpha,color_key,GUIBLOCK_TYPE_OSD,(GUI_LCD_DEV_INFO *)MMITHEME_GetDefaultLcdDev());    
}

/*****************************************************************************/
//  Description : UpdateCurrentFrame
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void UpdateCurrentFrame(HVIDEOHANDLE hVideo)
{
    VIDEO_PLAYER   *fsm_ptr  = (VIDEO_PLAYER *)hVideo;
    
    if (PNULL == fsm_ptr || !IsValidFsm(fsm_ptr))
    {
        return;
    }
    else if(VIDEO_STATE_READY < fsm_ptr->state)
    {
        //DPLAYER_UpdateCurrentFrame(DPLAYER_NORMAL_HANDLE);//leichi
              DPLAYER_UpdateCurrentFrame(fsm_ptr->dplayer_handle);//leichi
    }
}

/*****************************************************************************/
//  Description : main lcd is landscape
//  Global resource dependence : 
//  Author: 
//  Note: 主屏是否横屏
/*****************************************************************************/
LOCAL BOOLEAN IsMainScreenLandscape(void)
{
    BOOLEAN     result = FALSE;	
    result = GUILCD_IsLandscape(GUI_MAIN_LCD_ID);	
    return (result);
}

/*****************************************************************************/
//  Description : 用于是否可以显示全屏的相关信息
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
/*
LOCAL BOOLEAN IsDisplayFullScreenReferInfoEnable(void)
{   
    BOOLEAN result = FALSE;
	
#if defined(MOTION_SENSOR_TYPE) && defined(MMI_RES_ORIENT_BOTH)
	if(!MMISET_IsSensorOpen(MMISET_SENSOR_TYPE_LANDSCAPE_90) 
        && !MMISET_IsSensorOpen(MMISET_SENSOR_TYPE_LANDSCAPE_270))
    {
        result = TRUE;
    }
    else
    {
        result = FALSE;
    }
#else
    if ((LCD_ANGLE_90 == GUILCD_GetInitAngle(GUI_MAIN_LCD_ID)) ||
        (LCD_ANGLE_270 == GUILCD_GetInitAngle(GUI_MAIN_LCD_ID)))
    {
        result = FALSE;
    }
    else
    {
        result = TRUE; 
    }
#endif
	
    return result;
}
*/
/*****************************************************************************/
//  Description : SettingAsScreensaver
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
/*
#ifdef SCREENSAVER_SUPPORT
LOCAL BOOLEAN SettingAsScreensaver(wchar *full_name_ptr, uint16 full_name_len)
{
    MMISET_SCREENSAVER_INFO_T ssaver_info = {0};    
    VIDEO_SUPPORTED_TYPE support_type = 0;
	
    //WMP_TRACE_LOW:"[Wre Player] SettingAsScreensaver full_name_ptr = %x, full_name_len = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_2292_112_2_18_3_11_9_311,(uint8*)"dd",full_name_ptr,full_name_len);
	
    if ((PNULL == full_name_ptr) || (0 == full_name_len))
    {
        return FALSE;
    }
#ifdef DRM_SUPPORT
    if (!MMIAPIDRM_CanDrmFileSetToWSPC(full_name_ptr,full_name_len, DRMFILE_SETTING_SCREENSAVER))        
    {
        //不可以设置此DRM文件
       	//MMIPUB_OpenAlertWarningWin(TXT_DRM_NOT_SET_SCREENSAVER);
        return FALSE;
    }
#endif
	
    support_type =  GetSupportedStatus(full_name_ptr,full_name_len);
	
    if(VIDEO_SUPPORTED_BOTH       == support_type ||
		VIDEO_SUPPORTED_VIDEO_ONLY == support_type)
    {
        ssaver_info.is_open = TRUE;
        ssaver_info.ssaver_type = MMISET_SCREENSAVER_MORE;
        ssaver_info.ssaver_more_select = MMISET_SCREENSAVER_MORE_ANIMATION;//added by mary
        ssaver_info.more_saver_info.name_wstr_len = MIN(full_name_len,MMISET_SCREENSAVER_NAME_MAX_LEN);
        MMIAPICOM_Wstrncpy(ssaver_info.more_saver_info.name_wstr,full_name_ptr,ssaver_info.more_saver_info.name_wstr_len);
        
        MMIAPISET_SetScreenSaverInfo(&ssaver_info);
        //MMIPUB_OpenAlertSuccessWin(TXT_SUCCESS);
        return TRUE;
    }
    else
    {
        //MMIPUB_OpenAlertWarningWin(TXT_COMMON_NO_SUPPORT);
        return FALSE;
    }
}
#endif
*/

/*****************************************************************************/
//  Description : SettingAsPowerOnOffAnimation
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
/*
LOCAL BOOLEAN SettingAsPowerOnOffAnimation(BOOLEAN is_power_on, wchar *full_name_ptr, uint16 full_name_len)
{
    MMISET_POWER_ON_OFF_ANIM_INFO_T power_on_off_anim_info = {0};  
    VIDEO_SUPPORTED_TYPE support_type = 0;
	
    //WMP_TRACE_LOW:"[Wre Player] SettingAsPowerOnOffAnimation is_power_on = %d, full_name_ptr = %x,full_name_len = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_2341_112_2_18_3_11_9_312,(uint8*)"ddd",is_power_on,full_name_ptr,full_name_len);
    
    if (PNULL == full_name_ptr || 0 == full_name_len)
    {
        return FALSE;
    }
	
#ifdef DRM_SUPPORT
    if (!MMIAPIDRM_CanDrmFileSetToWSPC(full_name_ptr,full_name_len, DRMFILE_SETTING_POWERONOFFANIM))        
    {
        //不可以设置此DRM文件
        return FALSE;
    }
#endif
	
    support_type =  GetSupportedStatus(full_name_ptr,full_name_len);
	
    if(VIDEO_SUPPORTED_BOTH       == support_type ||
		VIDEO_SUPPORTED_VIDEO_ONLY == support_type)
    {
        power_on_off_anim_info.is_open                 = TRUE;
        power_on_off_anim_info.anim_type               = MMISET_POWER_ON_OFF_ANIM_MORE;
        power_on_off_anim_info.more_anim_info.name_wstr_len = MIN(full_name_len,MMISET_POWER_ON_OFF_ANIM_NAME_MAX_LEN);
        MMIAPICOM_Wstrncpy(power_on_off_anim_info.more_anim_info.name_wstr, full_name_ptr,power_on_off_anim_info.more_anim_info.name_wstr_len);  
        MMIAPISET_SetPowerOnOffAnimInfo(&power_on_off_anim_info, is_power_on);
        return TRUE;
    }
    else
    {  
        return FALSE;
    }    
}
*/

/*****************************************************************************/
//  Description : create mutex 
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void CreateFsmMutex(void)
{
    if(s_fsm_list_mutex == 0)
    {
        s_fsm_list_mutex = SCI_CreateMutex(
            "FSM_LIST_MUTEX",
            SCI_INHERIT
            );		
    }
}

/*****************************************************************************/
//  Description : destroy mutex 
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void DestroyFsmMutex(void)
{
	if(s_fsm_list_mutex != 0)
	{
		SCI_DeleteMutex(s_fsm_list_mutex);
		s_fsm_list_mutex = PNULL;		
	}	
}

/*****************************************************************************/
//  Description : 创建状态机消息
//  Global resource dependence :  none
//  Author: 
//  Note: 返回的指针需要调用DestroyMsg释放
/*****************************************************************************/
LOCAL VIDEO_FSM_MSG* CreatMsg(HVIDEOHANDLE hVideo, FSM_MSG_E msg_id, void *msg_body_ptr, uint32 msg_body_size)
{
    VIDEO_FSM_MSG    *msg_ptr        =   PNULL;
    
   // WMP_TRACE_LOW("[Wre Player] CreatMsg hVideo = %x, msg_id = %x,  msg_body_ptr = %x, msg_body_size = %d",hVideo,msg_id,msg_body_ptr,msg_body_size);
    //为状态机消息分配连续空间，便于后续释放
    switch(msg_id) 
    {
    case FSM_PLAY_REQ:
        {
            VIDEO_PLAY_PARAM *play_param_ptr = (VIDEO_PLAY_PARAM *)msg_body_ptr;
            VIDEO_PLAY_PARAM *play_param2_ptr = PNULL;
            if (PNULL == play_param_ptr || msg_body_size != sizeof(VIDEO_PLAY_PARAM))
            {
                break;
            }
            //获取序列化的长度
            //WMP_TRACE_LOW:"[Wre Player] CreatMsg play_param_ptr->playType = %d, msg_body_size = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_2424_112_2_18_3_11_9_313,(uint8*)"dd",play_param_ptr->playType,msg_body_size);
            switch(play_param_ptr->playType) 
            {

            case VIDEO_PLAY_FROM_FILE:
                if (PNULL == play_param_ptr->strFilePath)
                {
                    //WMP_TRACE_LOW:"[Wre Player] CreatMsg full path ptr is null"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_2431_112_2_18_3_11_9_314,(uint8*)"");
                    return PNULL;
                }
                //4对齐
                msg_body_size = msg_body_size + ((play_param_ptr->uFilePathLen+1) * sizeof(wchar)) + sizeof(void *);
                break;
            case VIDEO_PLAY_FROM_MEM:
                if (PNULL == play_param_ptr->videoBufferInfo.pVideoBuffer)
                {
                    //WMP_TRACE_LOW:"[Wre Player] CreatMsg buffer is null"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_2440_112_2_18_3_11_10_315,(uint8*)"");
                    return PNULL;
                }
                //msg_body_size += play_param_ptr->video_buf_info.video_buf_len + 1;
                break;
            case VIDEO_PLAY_FROM_NET:
                if (PNULL == play_param_ptr->strUrl)
                {
                    //WMP_TRACE_LOW:"[Wre Player] CreatMsg url is null"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_2448_112_2_18_3_11_10_316,(uint8*)"");
                    return PNULL;
                }
                //WMP_TRACE_LOW:"[Wre Player] CreatMsg msg_body_size = %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_2451_112_2_18_3_11_10_317,(uint8*)"d",msg_body_size);
                msg_body_size = msg_body_size + strlen(play_param_ptr->strUrl) + 1;
                //WMP_TRACE_LOW:"[Wre Player] CreatMsg msg_body_size = %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_2453_112_2_18_3_11_10_318,(uint8*)"d",msg_body_size);
                if (0 != play_param_ptr->uFilePathLen && PNULL != play_param_ptr->strFilePath)
                {
                    //4对齐
                    msg_body_size = msg_body_size + ((play_param_ptr->uFilePathLen+1) * sizeof(wchar)) + sizeof(void *);
                }
                break;
            default:
                return msg_ptr;
            } 

            //序列化
            msg_ptr = (VIDEO_FSM_MSG*)SCI_ALLOCA(sizeof(VIDEO_FSM_MSG)+msg_body_size);   
            //WMP_TRACE_LOW:"[Wre Player] msg_ptr = %x, len = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_2466_112_2_18_3_11_10_319,(uint8*)"dd",msg_ptr,sizeof(VIDEO_FSM_MSG)+msg_body_size);
            if(PNULL != msg_ptr)
            {
                //消息头
                SCI_MEMSET(msg_ptr,0,(sizeof(VIDEO_FSM_MSG)+ msg_body_size));
                msg_ptr->hVideo = hVideo;
                msg_ptr->msg_id = msg_id;
                msg_ptr->msg_size = sizeof(VIDEO_FSM_MSG);
                msg_ptr->msg_body = (void *)((uint32)msg_ptr + msg_ptr->msg_size);
                //参数结构序列化
                SCI_MEMCPY(msg_ptr->msg_body,msg_body_ptr,sizeof(VIDEO_PLAY_PARAM));
                play_param2_ptr = (VIDEO_PLAY_PARAM *)(msg_ptr->msg_body);
                //WMP_TRACE_LOW:" play_param2_ptr = %x, msg_body_size =%d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_2478_112_2_18_3_11_10_320,(uint8*)"dd",play_param2_ptr, msg_body_size);
                switch(play_param_ptr->playType) 
                {

                case VIDEO_PLAY_FROM_FILE:                    
                    play_param2_ptr->strFilePath = (wchar *)((uint32)msg_ptr+sizeof(VIDEO_FSM_MSG) + sizeof(VIDEO_PLAY_PARAM));
                    play_param2_ptr->strFilePath = (wchar *)((uint32)(play_param2_ptr->strFilePath) + GetAlign4Offset((uint32)(play_param2_ptr->strFilePath)));
                    play_param2_ptr->strFilePath = MMIAPICOM_Wstrncpy(play_param2_ptr->strFilePath, play_param_ptr->strFilePath,play_param_ptr->uFilePathLen);
                    break;                
                case VIDEO_PLAY_FROM_NET:
                    play_param2_ptr->strUrl = (char*)((uint32)msg_ptr+sizeof(VIDEO_FSM_MSG) + sizeof(VIDEO_PLAY_PARAM));
                    SCI_MEMCPY(play_param2_ptr->strUrl,play_param_ptr->strUrl,(strlen(play_param_ptr->strUrl)));/*lint !e666*/
                    //WMP_TRACE_LOW:"[Wre Player] CreatMsg play_param2_ptr->strUrl = %x, len = %x"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_2490_112_2_18_3_11_10_321,(uint8*)"dd",play_param2_ptr->strUrl,strlen(play_param_ptr->strUrl));
                    if (0 != play_param_ptr->uFilePathLen && PNULL != play_param_ptr->strFilePath)
                    {
                        play_param2_ptr->strFilePath = (wchar *)((uint32)play_param2_ptr->strUrl + strlen(play_param_ptr->strUrl) + 1);
                        //WMP_TRACE_LOW:" play_param2_ptr->strFilePath = %x"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_2494_112_2_18_3_11_10_322,(uint8*)"d",play_param2_ptr->strFilePath);
                        play_param2_ptr->strFilePath = (wchar *)((uint32)(play_param2_ptr->strFilePath) + GetAlign4Offset((uint32)(play_param2_ptr->strFilePath)));
                        //WMP_TRACE_LOW:" play_param2_ptr->strFilePath = %x"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_2496_112_2_18_3_11_10_323,(uint8*)"d",play_param2_ptr->strFilePath);
                        //WMP_TRACE_LOW:" play_param_ptr->strFilePath = %x, play_param_ptr->uFilePathLen = %d"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_2497_112_2_18_3_11_10_324,(uint8*)"dd",play_param_ptr->strFilePath,play_param_ptr->uFilePathLen);
                        play_param2_ptr->strFilePath = MMIAPICOM_Wstrncpy(play_param2_ptr->strFilePath, play_param_ptr->strFilePath,play_param_ptr->uFilePathLen);
                        //WMP_TRACE_LOW:" play_param2_ptr->strFilePath = %x"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_2499_112_2_18_3_11_10_325,(uint8*)"d",play_param2_ptr->strFilePath);
                    }
                    else
                    {
                        play_param2_ptr->strFilePath = PNULL;
                        play_param2_ptr->uFilePathLen = 0;
                    }
                    break;
                default:
                    break;
                }
                
            }    
        }
        break;
        
    case FSM_AUTH_RSP:
        {
            VIDEO_AUTH_INFO    *auth_info_ptr = (VIDEO_AUTH_INFO*)msg_body_ptr;
            VIDEO_AUTH_INFO    *auth_info2_ptr = PNULL;
            if (PNULL == msg_body_ptr || msg_body_size != sizeof(VIDEO_AUTH_INFO))
            {
                break;
            }
            if (PNULL != auth_info_ptr->strPassword)
            {
                msg_body_size = msg_body_size + strlen(auth_info_ptr->strPassword) + 1;
            }
            if (PNULL != auth_info_ptr->strUserName)
            {
                msg_body_size = msg_body_size + strlen(auth_info_ptr->strUserName) + 1;
            }            
            msg_ptr = (VIDEO_FSM_MSG*)SCI_ALLOCA(sizeof(VIDEO_FSM_MSG)+msg_body_size);
            if(PNULL != msg_ptr)
            {
                SCI_MEMSET(msg_ptr,0,(sizeof(VIDEO_FSM_MSG)+ msg_body_size));
                msg_ptr->hVideo = hVideo;
                msg_ptr->msg_id = msg_id;
                msg_ptr->msg_size = sizeof(VIDEO_FSM_MSG);
                msg_ptr->msg_body = (void *)((uint32)msg_ptr+sizeof(VIDEO_FSM_MSG));
                if (PNULL != msg_ptr->msg_body)
                {                
                    SCI_MEMCPY(msg_ptr->msg_body,msg_body_ptr,sizeof(VIDEO_AUTH_INFO));
                    auth_info2_ptr = (VIDEO_AUTH_INFO *)(msg_ptr->msg_body);                                
                    auth_info2_ptr->strPassword = (char*)((uint32)(msg_ptr->msg_body) + sizeof(VIDEO_AUTH_INFO));
                    if (PNULL != auth_info_ptr->strPassword && PNULL != auth_info2_ptr->strPassword)
                    {               
                        SCI_MEMCPY(auth_info2_ptr->strPassword,auth_info_ptr->strPassword,(strlen(auth_info_ptr->strPassword)));/*lint !e666*/
                        auth_info2_ptr->strUserName = (char*)((uint32)(auth_info2_ptr->strPassword) + strlen(auth_info_ptr->strPassword)) + 1;
                    }
                    else
                    {
                        auth_info2_ptr->strPassword = PNULL;
                        auth_info2_ptr->strUserName = (char*)((uint32)(msg_ptr->msg_body) + sizeof(VIDEO_AUTH_INFO));
                    }
                    if (PNULL != auth_info_ptr->strUserName && PNULL != auth_info2_ptr->strUserName)
                    {
                        SCI_MEMCPY(auth_info2_ptr->strUserName,auth_info_ptr->strUserName,strlen(auth_info_ptr->strUserName));/*lint !e666*/
                    }
                    else
                    {
                        auth_info2_ptr->strUserName = PNULL;
                    }
                
                }
            }    
        }
        break;
        
    case FSM_STRM_INIT_CNF:
        {
            STRM_INIT_CNF_T  *strm_init_info_ptr = (STRM_INIT_CNF_T*)msg_body_ptr;
            STRM_INIT_CNF_T  *strm_init_info2_ptr = PNULL;
            uint8            *temp_ptr  = PNULL;
            if (PNULL == msg_body_ptr || msg_body_size != sizeof(STRM_INIT_CNF_T))
            {
                break;
            }
            //WMP_TRACE_LOW:"[Wre Player] CreatMsg frame height = %d, rate = %d, base = %d,width = %d, cfg_len = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_2581_112_2_18_3_11_10_326,(uint8*)"ddddd",strm_init_info_ptr->sdp_info.video_param.frame_height,strm_init_info_ptr->sdp_info.video_param.frame_rate,strm_init_info_ptr->sdp_info.video_param.frame_rate_base,strm_init_info_ptr->sdp_info.video_param.frame_width,strm_init_info_ptr->sdp_info.video_param.config_len);
            if (PNULL != strm_init_info_ptr->sdp_info.audio_param.audio_type_name_ptr)
            {
                msg_body_size = msg_body_size + strlen(strm_init_info_ptr->sdp_info.audio_param.audio_type_name_ptr) + 1;
            }
            if (0 != strm_init_info_ptr->sdp_info.audio_param.config_len && PNULL != strm_init_info_ptr->sdp_info.audio_param.config_ptr)
            {
                msg_body_size = msg_body_size + strm_init_info_ptr->sdp_info.audio_param.config_len + 1;
            }
            if (PNULL != strm_init_info_ptr->sdp_info.audio_param.url_ptr)
            {
                msg_body_size = msg_body_size + strlen(strm_init_info_ptr->sdp_info.audio_param.url_ptr) + 1;
            }
            if (PNULL != strm_init_info_ptr->sdp_info.video_param.video_type_name_ptr)
            {
                msg_body_size = msg_body_size + strlen(strm_init_info_ptr->sdp_info.video_param.video_type_name_ptr) + 1;
            }
            //WMP_TRACE_LOW:"[Wre Player] CreatMsg strm_init_info_ptr->sdp_info.video_param.config_len = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_2598_112_2_18_3_11_10_327,(uint8*)"d",strm_init_info_ptr->sdp_info.video_param.config_len);
            if (0 != strm_init_info_ptr->sdp_info.video_param.config_len && PNULL != strm_init_info_ptr->sdp_info.video_param.config_ptr)
            {
                msg_body_size = msg_body_size + strm_init_info_ptr->sdp_info.video_param.config_len + 1;
            }
            if (PNULL != strm_init_info_ptr->sdp_info.video_param.url_ptr)
            {
                msg_body_size = msg_body_size + strlen(strm_init_info_ptr->sdp_info.video_param.url_ptr) + 1;
            }
            if (PNULL != strm_init_info_ptr->sdp_info.media_name_ptr)
            {
                msg_body_size = msg_body_size  + strlen(strm_init_info_ptr->sdp_info.media_name_ptr) + 1;
            }
            if (PNULL != strm_init_info_ptr->sdp_info.session_info_ptr)
            {
                msg_body_size = msg_body_size + strlen(strm_init_info_ptr->sdp_info.session_info_ptr) + 1;
            }
            //WMP_TRACE_LOW:"[Wre Player] CreatMsg sprop_param_sets_num = %d, enc_type = %d, sprop_sets_arr = %x"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_2618_112_2_18_3_11_10_328,(uint8*)"ddd",strm_init_info_ptr->sdp_info.video_fmtp_info.fmtp_param.h264_param.sprop_sets_len,strm_init_info_ptr->sdp_info.video_fmtp_info.enc_type,strm_init_info_ptr->sdp_info.video_fmtp_info.fmtp_param.h264_param.sprop_sets_ptr);
            if (STRM_SDP_ENC_H264 == strm_init_info_ptr->sdp_info.video_fmtp_info.enc_type 
                && PNULL != strm_init_info_ptr->sdp_info.video_fmtp_info.fmtp_param.h264_param.sprop_sets_ptr
                && 0 != strm_init_info_ptr->sdp_info.video_fmtp_info.fmtp_param.h264_param.sprop_sets_len)
            {
                msg_body_size = msg_body_size + strm_init_info_ptr->sdp_info.video_fmtp_info.fmtp_param.h264_param.sprop_sets_len +1;
            }

            msg_ptr = (VIDEO_FSM_MSG*)SCI_ALLOCA(sizeof(VIDEO_FSM_MSG)+msg_body_size);
           
            if(PNULL != msg_ptr)
            {
                SCI_MEMSET(msg_ptr,0,(sizeof(VIDEO_FSM_MSG)+ msg_body_size));
                msg_ptr->hVideo = hVideo;
                msg_ptr->msg_id = msg_id;
                msg_ptr->msg_size = sizeof(VIDEO_FSM_MSG);
                msg_ptr->msg_body = (void *)((uint32)msg_ptr+sizeof(VIDEO_FSM_MSG));
                if (PNULL != msg_ptr->msg_body)
                {                
                    SCI_MEMCPY(msg_ptr->msg_body,msg_body_ptr,sizeof(STRM_INIT_CNF_T));
                    strm_init_info2_ptr = (STRM_INIT_CNF_T*)msg_ptr->msg_body;
                    strm_init_info2_ptr->sdp_info.audio_param.audio_type_name_ptr = (char*)((uint32)(msg_ptr->msg_body) + sizeof(STRM_INIT_CNF_T));
                    if (PNULL != strm_init_info_ptr->sdp_info.audio_param.audio_type_name_ptr
                        && PNULL != strm_init_info2_ptr->sdp_info.audio_param.audio_type_name_ptr)                
                    {                    
                        SCI_MEMCPY(strm_init_info2_ptr->sdp_info.audio_param.audio_type_name_ptr,
                            strm_init_info_ptr->sdp_info.audio_param.audio_type_name_ptr,
                            (strlen(strm_init_info_ptr->sdp_info.audio_param.audio_type_name_ptr)));/*lint !e666*/
                        strm_init_info2_ptr->sdp_info.audio_param.config_ptr = (uint8 *)((uint32)(strm_init_info2_ptr->sdp_info.audio_param.audio_type_name_ptr) + strlen(strm_init_info_ptr->sdp_info.audio_param.audio_type_name_ptr) + 1);
                    }
                    else
                    {
                        strm_init_info2_ptr->sdp_info.audio_param.config_ptr =  (uint8*)(strm_init_info2_ptr->sdp_info.audio_param.audio_type_name_ptr);
                        strm_init_info2_ptr->sdp_info.audio_param.audio_type_name_ptr = PNULL;  
                    }
                    if (0 != strm_init_info_ptr->sdp_info.audio_param.config_len 
                        && PNULL != strm_init_info_ptr->sdp_info.audio_param.config_ptr
                        && PNULL != strm_init_info2_ptr->sdp_info.audio_param.config_ptr)
                    {                
                        SCI_MEMCPY(strm_init_info2_ptr->sdp_info.audio_param.config_ptr,
                            strm_init_info_ptr->sdp_info.audio_param.config_ptr,
                            strm_init_info_ptr->sdp_info.audio_param.config_len);
                        strm_init_info2_ptr->sdp_info.audio_param.url_ptr = (char *)((uint32)(strm_init_info2_ptr->sdp_info.audio_param.config_ptr) + strm_init_info_ptr->sdp_info.audio_param.config_len + 1);
                    }
                    else
                    {
                        strm_init_info2_ptr->sdp_info.audio_param.url_ptr = (char *)(strm_init_info2_ptr->sdp_info.audio_param.config_ptr);
                        strm_init_info2_ptr->sdp_info.audio_param.config_ptr = PNULL;
                    }
                    if (PNULL != strm_init_info_ptr->sdp_info.audio_param.url_ptr
                        && PNULL != strm_init_info2_ptr->sdp_info.audio_param.url_ptr)
                    {               
                        SCI_MEMCPY(strm_init_info2_ptr->sdp_info.audio_param.url_ptr,
                            strm_init_info_ptr->sdp_info.audio_param.url_ptr,
                            (strlen(strm_init_info_ptr->sdp_info.audio_param.url_ptr)));/*lint !e666*/
                        strm_init_info2_ptr->sdp_info.video_param.video_type_name_ptr = (char *)((uint32)(strm_init_info2_ptr->sdp_info.audio_param.url_ptr) + strlen(strm_init_info_ptr->sdp_info.audio_param.url_ptr) +1);
                    }
                    else
                    {
                        strm_init_info2_ptr->sdp_info.video_param.video_type_name_ptr = strm_init_info2_ptr->sdp_info.audio_param.url_ptr;
                        strm_init_info2_ptr->sdp_info.audio_param.url_ptr = PNULL;
                    }
                
                    if(PNULL != strm_init_info_ptr->sdp_info.video_param.video_type_name_ptr
                        && PNULL != strm_init_info2_ptr->sdp_info.video_param.video_type_name_ptr)
                    {
                        SCI_MEMCPY(strm_init_info2_ptr->sdp_info.video_param.video_type_name_ptr,
                            strm_init_info_ptr->sdp_info.video_param.video_type_name_ptr,
                            (strlen(strm_init_info_ptr->sdp_info.video_param.video_type_name_ptr)));/*lint !e666*/
                        strm_init_info2_ptr->sdp_info.video_param.config_ptr = (uint8 *)((uint32)(strm_init_info2_ptr->sdp_info.video_param.video_type_name_ptr) + strlen(strm_init_info_ptr->sdp_info.video_param.video_type_name_ptr) + 1);
                    }
                    else
                    {
                        strm_init_info2_ptr->sdp_info.video_param.config_ptr = (uint8 *)(strm_init_info2_ptr->sdp_info.video_param.video_type_name_ptr);
                        strm_init_info2_ptr->sdp_info.video_param.video_type_name_ptr = PNULL;
                    }

                    if (0 != strm_init_info_ptr->sdp_info.video_param.config_len 
                        && PNULL != strm_init_info_ptr->sdp_info.video_param.config_ptr
                        && PNULL != strm_init_info2_ptr->sdp_info.video_param.config_ptr)
                    {                
                        SCI_MEMCPY(strm_init_info2_ptr->sdp_info.video_param.config_ptr,
                            strm_init_info_ptr->sdp_info.video_param.config_ptr,
                            strm_init_info_ptr->sdp_info.video_param.config_len);
                        strm_init_info2_ptr->sdp_info.video_param.url_ptr = (char *)((uint32)(strm_init_info2_ptr->sdp_info.video_param.config_ptr) + strm_init_info_ptr->sdp_info.video_param.config_len + 1);
                    }
                    else
                    {
                        strm_init_info2_ptr->sdp_info.video_param.url_ptr = (char *)(strm_init_info2_ptr->sdp_info.video_param.config_ptr);
                        strm_init_info2_ptr->sdp_info.video_param.config_ptr = PNULL;
                    }

                    if (PNULL != strm_init_info_ptr->sdp_info.video_param.url_ptr
                        && PNULL != strm_init_info2_ptr->sdp_info.video_param.url_ptr)
                    {                
                        SCI_MEMCPY(strm_init_info2_ptr->sdp_info.video_param.url_ptr,
                            strm_init_info_ptr->sdp_info.video_param.url_ptr,
                            (strlen(strm_init_info_ptr->sdp_info.video_param.url_ptr)));/*lint !e666*/
                        strm_init_info2_ptr->sdp_info.media_name_ptr = (char *)((uint32)(strm_init_info2_ptr->sdp_info.video_param.url_ptr) + strlen(strm_init_info_ptr->sdp_info.video_param.url_ptr) + 1);
                    }
                    else
                    {
                        strm_init_info2_ptr->sdp_info.media_name_ptr = strm_init_info2_ptr->sdp_info.video_param.url_ptr;
                        strm_init_info2_ptr->sdp_info.video_param.url_ptr = PNULL;
                    }

                    if (PNULL != strm_init_info_ptr->sdp_info.media_name_ptr
                        && PNULL != strm_init_info2_ptr->sdp_info.media_name_ptr)
                    {                
                        SCI_MEMCPY(strm_init_info2_ptr->sdp_info.media_name_ptr,
                            strm_init_info_ptr->sdp_info.media_name_ptr,
                            (strlen(strm_init_info_ptr->sdp_info.media_name_ptr)));/*lint !e666*/
                        strm_init_info2_ptr->sdp_info.session_info_ptr = (char *)((uint32)(strm_init_info2_ptr->sdp_info.media_name_ptr) + strlen(strm_init_info_ptr->sdp_info.media_name_ptr) + 1);
                    }
                    else
                    {
                        strm_init_info2_ptr->sdp_info.session_info_ptr = strm_init_info2_ptr->sdp_info.media_name_ptr;
                        strm_init_info2_ptr->sdp_info.media_name_ptr = PNULL;
                    }

                    if (PNULL != strm_init_info_ptr->sdp_info.session_info_ptr
                        && PNULL != strm_init_info2_ptr->sdp_info.session_info_ptr)
                    {                
                        SCI_MEMCPY(strm_init_info2_ptr->sdp_info.session_info_ptr,
                            strm_init_info_ptr->sdp_info.session_info_ptr,
                            (strlen(strm_init_info_ptr->sdp_info.session_info_ptr)));/*lint !e666*/
                        temp_ptr = (uint8 *)((uint32)strm_init_info2_ptr->sdp_info.session_info_ptr + strlen(strm_init_info_ptr->sdp_info.session_info_ptr) + 1);
                    }
                    else
                    {
                        temp_ptr = (uint8 *)(strm_init_info2_ptr->sdp_info.session_info_ptr);
                        strm_init_info2_ptr->sdp_info.session_info_ptr = PNULL;                     
                    }

                    if (STRM_SDP_ENC_H264 == strm_init_info_ptr->sdp_info.video_fmtp_info.enc_type 
                    && PNULL != strm_init_info_ptr->sdp_info.video_fmtp_info.fmtp_param.h264_param.sprop_sets_ptr
                    && 0 != strm_init_info_ptr->sdp_info.video_fmtp_info.fmtp_param.h264_param.sprop_sets_len
                    && PNULL != temp_ptr)
                    {
                        strm_init_info2_ptr->sdp_info.video_fmtp_info.fmtp_param.h264_param.sprop_sets_ptr = temp_ptr;
                        SCI_MEMCPY(temp_ptr,
                            strm_init_info_ptr->sdp_info.video_fmtp_info.fmtp_param.h264_param.sprop_sets_ptr,
                            strm_init_info_ptr->sdp_info.video_fmtp_info.fmtp_param.h264_param.sprop_sets_len);
                    
                    }
                    
                }
            }

        }
        break;

    default:
        msg_ptr = (VIDEO_FSM_MSG*)SCI_ALLOCA(sizeof(VIDEO_FSM_MSG)+msg_body_size);
       
        if(PNULL != msg_ptr)
        {
            SCI_MEMSET(msg_ptr,0,(sizeof(VIDEO_FSM_MSG)+ msg_body_size));
            msg_ptr->hVideo = hVideo;
            msg_ptr->msg_id = msg_id;
            msg_ptr->msg_size = sizeof(VIDEO_FSM_MSG);
            if (0 == msg_body_size)
            {
                msg_ptr->msg_body = PNULL;
            }
            else
            {
                msg_ptr->msg_body = (void *)((uint32)msg_ptr+sizeof(VIDEO_FSM_MSG));
                if (PNULL != msg_ptr->msg_body)
                {                
                    SCI_MEMCPY(msg_ptr->msg_body,msg_body_ptr,msg_body_size);
                }
            }
            
        }    
        break;
    }
    
    return msg_ptr;
}

/*****************************************************************************/
//  Description : 释放状态机消息
//  Global resource dependence :  none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN DestroyMsg(VIDEO_FSM_MSG *msg_ptr)
{
    if (PNULL != msg_ptr)
    {
        SCI_FREE(msg_ptr);
        msg_ptr = PNULL;
    }
    return TRUE;
}

/*****************************************************************************/
//  Description : 创建状态机
//  Global resource dependence :  none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL HVIDEOHANDLE CreateFSM(VideoProc vpCallBack)
{
    
    VIDEO_PLAYER              *fsm_ptr              = PNULL;
    VIDEO_PLAYER_LIST         *cur_fsm_node_ptr     = PNULL;
    
    //目前不支持多个状态机
    if(0 != s_wmp_fsm_list)
    {
        //WMP_TRACE_LOW:"[Wre Player] CreateFSM there is a fsm"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_2826_112_2_18_3_11_10_329,(uint8*)"");
        return 0;
    }

    //为状态机分配空间
    fsm_ptr = (VIDEO_PLAYER*)SCI_ALLOCA(sizeof(VIDEO_PLAYER));
    if (PNULL == fsm_ptr)
    {
        //WMP_TRACE_LOW:"[Wre Player] CreateFSM no space for fsm"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_2834_112_2_18_3_11_10_330,(uint8*)"");
        return 0;
    }
    SCI_MEMSET(fsm_ptr,0,sizeof(VIDEO_PLAYER));

    //初始化状态机信息    
    fsm_ptr->dplayer_handle = PNULL;
    fsm_ptr->uFilePathLen = 0;
    fsm_ptr->strFilePath = PNULL;
    fsm_ptr->process_time = 0;
    fsm_ptr->state = VIDEO_STATE_IDLE;
    fsm_ptr->total_time = 0;
    fsm_ptr->vpCallBack = vpCallBack;


    fsm_ptr->strm_handle = 0;
    fsm_ptr->url_ptr = PNULL;
    fsm_ptr->netID   = 0;
    fsm_ptr->playType = VIDEO_PLAY_FROM_MAX;
    fsm_ptr->support_type = VIDEO_SUPPORTED_BOTH;
    fsm_ptr->is_to_exit = FALSE;
    fsm_ptr->is_to_pause = FALSE;
    fsm_ptr->is_to_resume = FALSE;

    fsm_ptr->task_id = P_APP;
    
    //给s_wmp_fsm_list加锁    
    SCI_GetMutex(s_fsm_list_mutex, SCI_WAIT_FOREVER);
    
    //将当前状态机插入到状态机列表中
    if(s_wmp_fsm_list == 0)
    {
        cur_fsm_node_ptr = (VIDEO_PLAYER_LIST* )SCI_ALLOCA(sizeof(VIDEO_PLAYER_LIST));
        if (PNULL != cur_fsm_node_ptr)
        {
            cur_fsm_node_ptr->fsm_info_ptr = fsm_ptr;
            cur_fsm_node_ptr->next_fsm_ptr = PNULL;
            s_wmp_fsm_list = (uint32)cur_fsm_node_ptr;
            //WMP_TRACE_LOW:"[Wre Player] CreateFSM cur fsm is %x"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_2872_112_2_18_3_11_10_331,(uint8*)"d",cur_fsm_node_ptr);
        }
        else
        {
            SCI_FREE(fsm_ptr);
            fsm_ptr = PNULL;
            
            
        }
    }
    else
    {
        cur_fsm_node_ptr = (VIDEO_PLAYER_LIST* )s_wmp_fsm_list;
        while (PNULL != cur_fsm_node_ptr->next_fsm_ptr)
        {
            cur_fsm_node_ptr = cur_fsm_node_ptr->next_fsm_ptr;
        }
        
        cur_fsm_node_ptr->next_fsm_ptr = (VIDEO_PLAYER_LIST*)SCI_ALLOCA(sizeof(VIDEO_PLAYER_LIST));
        if(PNULL != cur_fsm_node_ptr->next_fsm_ptr)
        {
            cur_fsm_node_ptr->next_fsm_ptr->fsm_info_ptr = fsm_ptr;
            cur_fsm_node_ptr->next_fsm_ptr->next_fsm_ptr = PNULL;
            //WMP_TRACE_LOW:"[Wre Player] CreateFSM cur fsm is %x"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_2895_112_2_18_3_11_10_332,(uint8*)"d",cur_fsm_node_ptr->next_fsm_ptr);
        }
        else
        {
            SCI_FREE(fsm_ptr);
            fsm_ptr = PNULL;
        }
    }
    
    //给s_wmp_fsm_list解锁
    SCI_PutMutex(s_fsm_list_mutex);
    
    return (HVIDEOHANDLE)fsm_ptr;
}

/*****************************************************************************/
//  Description : 销毁状态机
//  Global resource dependence :  none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void DestroyFSM(HVIDEOHANDLE hVideo)
{
    VIDEO_PLAYER_LIST        *current_fsm_ptr = PNULL;
    VIDEO_PLAYER_LIST        *pre_fsm_ptr = PNULL;
    
    if (s_wmp_fsm_list == PNULL)
    {
        //WMP_TRACE_LOW:"[Wre Player] DestroyFSM there is no fsm list"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_2921_112_2_18_3_11_10_333,(uint8*)"");
        return;
    }
   
    SCI_GetMutex(s_fsm_list_mutex, SCI_WAIT_FOREVER);

    //从状态机列表中查找该状态机
    current_fsm_ptr = (VIDEO_PLAYER_LIST *)s_wmp_fsm_list;
    //WMP_TRACE_LOW:"[Wre Player] DestroyFSM current_fsm_ptr = %x, current_fsm_ptr->fsm_info_ptr = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_2929_112_2_18_3_11_11_334,(uint8*)"dd",current_fsm_ptr,current_fsm_ptr->fsm_info_ptr);
    if (PNULL != current_fsm_ptr && (VIDEO_PLAYER *)hVideo == current_fsm_ptr->fsm_info_ptr)
    {
        //从状态机列表中删除该状态机
        s_wmp_fsm_list = (uint32)(current_fsm_ptr->next_fsm_ptr);
        //释放该状态机
        if (PNULL != current_fsm_ptr->fsm_info_ptr->strFilePath)
        {
            SCI_FREE(current_fsm_ptr->fsm_info_ptr->strFilePath);
            current_fsm_ptr->fsm_info_ptr->strFilePath = PNULL;
        }
        if (PNULL != current_fsm_ptr->fsm_info_ptr->url_ptr)
        {
            SCI_FREE(current_fsm_ptr->fsm_info_ptr->url_ptr);
            current_fsm_ptr->fsm_info_ptr->url_ptr = PNULL;
        }
        SCI_FREE(current_fsm_ptr->fsm_info_ptr);
        current_fsm_ptr->fsm_info_ptr = PNULL;
        SCI_FREE(current_fsm_ptr);
        current_fsm_ptr = PNULL;       
    }
    else if(PNULL != current_fsm_ptr)
    {
        pre_fsm_ptr = current_fsm_ptr;
        current_fsm_ptr = current_fsm_ptr->next_fsm_ptr;
        ////从状态机列表中查找该状态机
        while (PNULL != current_fsm_ptr)
        {
            if ((VIDEO_PLAYER *)hVideo == current_fsm_ptr->fsm_info_ptr)
            {
                //从状态机列表中删除该状态机
                pre_fsm_ptr->next_fsm_ptr = current_fsm_ptr->next_fsm_ptr;
                //释放该状态机
                if (PNULL != current_fsm_ptr->fsm_info_ptr->strFilePath)
                {
                    SCI_FREE(current_fsm_ptr->fsm_info_ptr->strFilePath);
                    current_fsm_ptr->fsm_info_ptr->strFilePath = PNULL;
                }
                if (PNULL != current_fsm_ptr->fsm_info_ptr->url_ptr)
                {
                    SCI_FREE(current_fsm_ptr->fsm_info_ptr->url_ptr);
                    current_fsm_ptr->fsm_info_ptr->url_ptr = PNULL;
                }
                SCI_FREE(current_fsm_ptr->fsm_info_ptr);
                current_fsm_ptr->fsm_info_ptr = PNULL;
                SCI_FREE(current_fsm_ptr);
                current_fsm_ptr= PNULL;        
                break;
            }
            else
            {
                pre_fsm_ptr = current_fsm_ptr;
                current_fsm_ptr = current_fsm_ptr->next_fsm_ptr;
            }
        }
    }
    //WMP_TRACE_LOW:"[Wre Player] DestroyFSM  fsm hVideo = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_2985_112_2_18_3_11_11_335,(uint8*)"d",hVideo);
     //给s_wmp_fsm_list解锁
    SCI_PutMutex(s_fsm_list_mutex);
    s_current_fsm = PNULL;
}
/*****************************************************************************/
//  Description : 判断状态机是否有效
//  Global resource dependence :  none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsValidFsm(VIDEO_PLAYER        *fsm_ptr)
{
    VIDEO_PLAYER_LIST        *current_fsm_ptr = PNULL;
    BOOLEAN              result           = FALSE;
    
   
    SCI_GetMutex(s_fsm_list_mutex, SCI_WAIT_FOREVER);

    
    //在状态机列表中查找该状态机是否存在
    current_fsm_ptr = (VIDEO_PLAYER_LIST*)s_wmp_fsm_list;
    while (PNULL != current_fsm_ptr)
    {
        if (fsm_ptr == current_fsm_ptr->fsm_info_ptr)
        {
            result = TRUE;
            break;
        }
        current_fsm_ptr = current_fsm_ptr->next_fsm_ptr;
    }

    //给s_wmp_fsm_list解锁
    SCI_PutMutex(s_fsm_list_mutex);
    return result;
}

/*****************************************************************************/
//  Description : 分发请求消息
//  Global resource dependence :  none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN FsmDispatch(HVIDEOHANDLE   hVideo,  FSM_MSG_E msg_id, void *msg_body_ptr, uint32 msg_body_size)
{
    VIDEO_PLAYER        *fsm_ptr    =  (VIDEO_PLAYER *)hVideo;
    BOOLEAN         result      = FALSE;
    VIDEO_FSM_MSG    *req_msg_ptr = PNULL;
    
    //参数检查 
    //判断状态是否有效
    if(PNULL == fsm_ptr || !IsValidFsm(fsm_ptr) || (PNULL == msg_body_ptr && 0 != msg_body_size))
    {
        //WMP_TRACE_LOW:"[Wre Player] FsmDispatch something wrong with fsm_ptr = %x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_3034_112_2_18_3_11_11_336,(uint8*)"d",fsm_ptr);
        return FALSE;
    }

   // WMP_TRACE_LOW("[Wre Player] FsmDispatch msg_id = %d, msg_body_ptr = %x, msg_body_size = %d, fsm_ptr->state =%d",msg_id,msg_body_ptr,msg_body_size,fsm_ptr->state);
    req_msg_ptr = CreatMsg(hVideo,msg_id, msg_body_ptr,msg_body_size);    
    if (PNULL != req_msg_ptr && fsm_ptr->state < VIDEO_STATE_MAX && PNULL != video_fsm_dispatch[fsm_ptr->state])
    {
        result = (*video_fsm_dispatch[fsm_ptr->state])(fsm_ptr,req_msg_ptr);      
        DestroyMsg(req_msg_ptr); 
    } 
    return result;
}

/*****************************************************************************/
//  Description : 转换状态
//  Global resource dependence :  none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void FsmStateTrans(VIDEO_PLAYER *fsm_ptr, VIDEO_STATE state)
{    
    //WMP_TRACE_LOW:"[Wre Player] FsmStateTrans fsm_ptr = %x, state = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_3054_112_2_18_3_11_11_337,(uint8*)"dd",fsm_ptr,state);
    //参数检查
    if(fsm_ptr == PNULL || !IsValidFsm(fsm_ptr) || state >= VIDEO_STATE_MAX)
    {
        return;
    }    

    //向 control发送exit消息
    FsmDispatch((HVIDEOHANDLE)fsm_ptr,FSM_EXIT_REQ, PNULL,0);       
    //转换状态
    fsm_ptr->state = state;    

    //向 control发送entry消息
    FsmDispatch((HVIDEOHANDLE)fsm_ptr,FSM_ENTRY_REQ, PNULL,0);    
    
}

/*****************************************************************************/
//  Description : 给ui层发送VIDEO_NOTIFY_PREPARE
//  Global resource dependence :  none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void SendPrepareIndToUI(VIDEO_PLAYER *fsm_ptr)
{
    CALLBACKPARAM        msg_to_ui       = {0};
    VIDEO_PREPARE        prepare_info    = {0};
    
    if (PNULL == fsm_ptr || !IsValidFsm(fsm_ptr) || PNULL == fsm_ptr->vpCallBack)
    {
        //WMP_TRACE_LOW:"[Wre Player] SendPrepareIndToUI param is error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_3082_112_2_18_3_11_11_338,(uint8*)"");
        return;
    }
    
    //创建VIDEO_NOTIFY_PREPARE消息
    msg_to_ui.hVideo = (HVIDEOHANDLE)fsm_ptr;
    msg_to_ui.nNotifyID = VIDEO_NOTIFY_PREPARE;
    msg_to_ui.pNotifyParam = &prepare_info;
    prepare_info.uFileNameLen = fsm_ptr->uFilePathLen;
    prepare_info.strFileName = fsm_ptr->strFilePath;
    prepare_info.nStarttime = fsm_ptr->process_time;
    prepare_info.nTotalTime = fsm_ptr->total_time;
    prepare_info.hVideo = (HVIDEOHANDLE)fsm_ptr;
    prepare_info.supportType = fsm_ptr->support_type;
    prepare_info.playType = fsm_ptr->playType;
    
    //WMP_TRACE_LOW:"[Wre Player] SendPrepareIndToUI send prepare ind to ui"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_3098_112_2_18_3_11_11_339,(uint8*)"");
    //给ui层发送VIDEO_NOTIFY_PREPARE
    fsm_ptr->vpCallBack(&msg_to_ui);
}

/*****************************************************************************/
//  Description : 给ui层发送VIDEO_NOTIFY_AUTH
//  Global resource dependence :  none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void SendAUTHIndToUI(VIDEO_PLAYER *fsm_ptr)
{
#ifdef MMI_STREAMING_PLAYER_SUPPORT //20120612
    CALLBACKPARAM    msg_to_ui   = {0};
    VIDEO_AUTH       auth_info   = {0};
    
    if (PNULL == fsm_ptr || !IsValidFsm(fsm_ptr) || PNULL == fsm_ptr->vpCallBack)
    {
        //WMP_TRACE_LOW:"[Wre Player] SendAUTHIndToUI param is error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_3114_112_2_18_3_11_11_340,(uint8*)"");
        return;
    }
    
    //创建VIDEO_NOTIFY_AUTH消息
    msg_to_ui.hVideo = (HVIDEOHANDLE)fsm_ptr;
    msg_to_ui.nNotifyID = VIDEO_NOTIFY_AUTH;
    msg_to_ui.pNotifyParam  = &auth_info;
    auth_info.hVideo = (HVIDEOHANDLE)fsm_ptr;
    //WMP_TRACE_LOW:"[Wre Player] SendAUTHIndToUI send auth ind to ui"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_3123_112_2_18_3_11_11_341,(uint8*)"");
    //给ui层发送VIDEO_NOTIFY_AUTH
    fsm_ptr->vpCallBack(&msg_to_ui);
#endif
}

/*****************************************************************************/
//  Description : 给ui层发送VIDEO_NOTIFY_FATAL_ERROR
//  Global resource dependence :  none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void SendFatalErrorIndTOUI(VIDEO_PLAYER *fsm_ptr,VIDEO_ERROR_TYPE errorType)
{
    CALLBACKPARAM           msg_to_ui           = {0};
    VIDEO_FATAL_ERROR    fatal_error_info    = {0};
    
    if (PNULL == fsm_ptr || !IsValidFsm(fsm_ptr) || PNULL == fsm_ptr->vpCallBack)
    {
        //WMP_TRACE_LOW:"[Wre Player] SendFatalErrorIndTOUI param is error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_3139_112_2_18_3_11_11_342,(uint8*)"");
        return;
    }
    
    //创建VIDEO_NOTIFY_FATAL_ERROR消息
    msg_to_ui.hVideo = (HVIDEOHANDLE)fsm_ptr;
    msg_to_ui.nNotifyID = VIDEO_NOTIFY_FATAL_ERROR;
    msg_to_ui.pNotifyParam = &fatal_error_info;
    fatal_error_info.hVideo = (HVIDEOHANDLE)fsm_ptr;
    fatal_error_info.errorType = errorType;
    //WMP_TRACE_LOW:"[Wre Player] SendFatalErrorIndTOUI send fatal error to ui errorType = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_3149_112_2_18_3_11_11_343,(uint8*)"d",errorType);

    //给ui层发送VIDEO_NOTIFY_FATAL_ERROR
    fsm_ptr->vpCallBack(&msg_to_ui);
}

/*****************************************************************************/
//  Description : 给ui层发送VIDEO_NOTIFY_PLAY_PROCESS
//  Global resource dependence :  none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void SendProcessIndToUI(VIDEO_PLAYER *fsm_ptr)
{
    CALLBACKPARAM              msg_to_ui       = {0};
    VIDEO_PLAY_PROCESS         process_ind   = {0};
    
    if (PNULL == fsm_ptr || !IsValidFsm(fsm_ptr) || PNULL == fsm_ptr->vpCallBack)
    {
        //WMP_TRACE_LOW:"[Wre Player] SendProcessIndToUI param is error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_3166_112_2_18_3_11_11_344,(uint8*)"");
        return;
    }
    
    //创建VIDEO_NOTIFY_PLAY_PROCESS消息
    msg_to_ui.hVideo = (HVIDEOHANDLE)fsm_ptr;
    msg_to_ui.nNotifyID = VIDEO_NOTIFY_PLAY_PROCESS;
    msg_to_ui.pNotifyParam  = &process_ind;
    process_ind.hVideo = (HVIDEOHANDLE)fsm_ptr;
    process_ind.nProcessTime = fsm_ptr->process_time;
    process_ind.nTotalTime = fsm_ptr->total_time;  
    //给ui层发送VIDEO_NOTIFY_PLAY_PROCESS
    fsm_ptr->vpCallBack(&msg_to_ui);
}

/*****************************************************************************/
//  Description : 给ui层发送VIDEO_NOTIFY_BUFFERING
//  Global resource dependence :  none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void SendBufferingIndToUI(VIDEO_PLAYER *fsm_ptr)
{
#ifdef MMI_STREAMING_PLAYER_SUPPORT //20120612
    CALLBACKPARAM       msg_to_ui   = {0};
    VIDEO_BUFFERING    buffering_ind   = {0};
    
    if (PNULL == fsm_ptr || !IsValidFsm(fsm_ptr) || PNULL == fsm_ptr->vpCallBack)
    {
        //WMP_TRACE_LOW:"[Wre Player] SendBufferingIndToUI param is error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_3192_112_2_18_3_11_11_345,(uint8*)"");
        return;
    }
    
    //创建VIDEO_NOTIFY_BUFFERING消息
    msg_to_ui.hVideo = (HVIDEOHANDLE)fsm_ptr;
    msg_to_ui.nNotifyID = VIDEO_NOTIFY_BUFFERING;
    msg_to_ui.pNotifyParam  = &buffering_ind;
    buffering_ind.hVideo = (HVIDEOHANDLE)fsm_ptr;
    //WMP_TRACE_LOW:"[Wre Player] SendBufferingIndToUI send VIDEO_NOTIFY_BUFFERING to ui"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_3201_112_2_18_3_11_11_346,(uint8*)"");
    //给ui层发送VIDEO_NOTIFY_BUFFERING
    fsm_ptr->vpCallBack(&msg_to_ui);
#endif
}

/*****************************************************************************/
//  Description : 给ui层发送VIDEO_NOTIFY_BUFFER_PROCESS
//  Global resource dependence :  none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void SendBufferPercentToUI(VIDEO_PLAYER *fsm_ptr)
{
#ifdef MMI_STREAMING_PLAYER_SUPPORT //20120612
    CALLBACKPARAM       msg_to_ui   = {0};
    VIDEO_BUFFERING_PROCESS       buffering_process_info   = {0};
    
    if (PNULL == fsm_ptr || !IsValidFsm(fsm_ptr) || PNULL == fsm_ptr->vpCallBack)
    {
        //WMP_TRACE_LOW:"[Wre Player] SendBufferPercentToUI param is error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_3217_112_2_18_3_11_11_347,(uint8*)"");
        return;
    }
    
    //创建VIDEO_NOTIFY_BUFFER_PROCESS消息
    buffering_process_info.nBufferPercent = fsm_ptr->buf_percent;
    msg_to_ui.hVideo = (HVIDEOHANDLE)fsm_ptr;
    msg_to_ui.nNotifyID = VIDEO_NOTIFY_BUFFER_PROCESS;
    msg_to_ui.pNotifyParam  = &buffering_process_info;
    buffering_process_info.hVideo = (HVIDEOHANDLE)fsm_ptr;
    //WMP_TRACE_LOW:"[Wre Player]  send VIDEO_NOTIFY_BUFFER_END to ui"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_3227_112_2_18_3_11_11_348,(uint8*)"");
    //给ui层发送VIDEO_NOTIFY_BUFFER_PROCESS
    fsm_ptr->vpCallBack(&msg_to_ui);
#endif
}

/*****************************************************************************/
//  Description : 给ui层发送VIDEO_NOTIFY_BUFFER_END
//  Global resource dependence :  none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void SendBufferEndIndToUI(VIDEO_PLAYER *fsm_ptr)
{
#ifdef MMI_STREAMING_PLAYER_SUPPORT //20120612

    CALLBACKPARAM       msg_to_ui   = {0};
    VIDEO_BUFFER_END       buffering_end_info   = {0};
    
    if (PNULL == fsm_ptr || !IsValidFsm(fsm_ptr) || PNULL == fsm_ptr->vpCallBack)
    {
        //WMP_TRACE_LOW:"[Wre Player] SendBufferEndIndToUI param is error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_3243_112_2_18_3_11_11_349,(uint8*)"");
        return;
    }
    
    //创建VIDEO_NOTIFY_BUFFER_END消息
    msg_to_ui.hVideo = (HVIDEOHANDLE)fsm_ptr;
    msg_to_ui.nNotifyID = VIDEO_NOTIFY_BUFFER_END;
    msg_to_ui.pNotifyParam  = &buffering_end_info;
    buffering_end_info.hVideo = (HVIDEOHANDLE)fsm_ptr;
    //WMP_TRACE_LOW:"[Wre Player] send VIDEO_NOTIFY_BUFFER_END to ui"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_3252_112_2_18_3_11_11_350,(uint8*)"");
    //给ui层发送VIDEO_NOTIFY_BUFFER_END
    fsm_ptr->vpCallBack(&msg_to_ui);
#endif
}

/*****************************************************************************/
//  Description : 给ui层发送VIDEO_NOTIFY_PLAY_END
//  Global resource dependence :  none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void SendPlayEndToUI(VIDEO_PLAYER *fsm_ptr, VIDEO_PLAY_END_RESULT result)
{
    CALLBACKPARAM       msg_to_ui   = {0};
    VIDEO_PALY_END         play_end   = {0};
    
    if (PNULL == fsm_ptr || !IsValidFsm(fsm_ptr) || PNULL == fsm_ptr->vpCallBack)
    {
        //WMP_TRACE_LOW:"[Wre Player] SendPlayEndToUI param is error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_3268_112_2_18_3_11_11_351,(uint8*)"");
        return;
    }
    
    //创建VIDEO_NOTIFY_PLAY_END消息
    msg_to_ui.hVideo = (HVIDEOHANDLE)fsm_ptr;
    msg_to_ui.nNotifyID = VIDEO_NOTIFY_PLAY_END;
    msg_to_ui.pNotifyParam  = &play_end;
    play_end.hVideo = (HVIDEOHANDLE)fsm_ptr;
    play_end.result = result;
    //WMP_TRACE_LOW:"[Wre Player] SendAUTHIndToUI send VIDEO_NOTIFY_PLAY_END to ui"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_3278_112_2_18_3_11_11_352,(uint8*)"");
    //给ui层发送VIDEO_NOTIFY_PLAY_END
    fsm_ptr->vpCallBack(&msg_to_ui);
}

/*****************************************************************************/
//  Description : 给ui层发送VIDEO_NOTIFY_STOP
//  Global resource dependence :  none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void SendStopIndToUI(VIDEO_PLAYER *fsm_ptr)
{
    CALLBACKPARAM       msg_to_ui   = {0};
    VIDEO_STOP       stop_ind    = {0};
    
    if (PNULL == fsm_ptr || !IsValidFsm(fsm_ptr) || PNULL == fsm_ptr->vpCallBack)
    {
        //WMP_TRACE_LOW:"[Wre Player] SendStopIndToUI param is error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_3294_112_2_18_3_11_11_353,(uint8*)"");
        return;
    }
    
    //创建VIDEO_NOTIFY_STOP消息
    msg_to_ui.hVideo = (HVIDEOHANDLE)fsm_ptr;
    msg_to_ui.nNotifyID = VIDEO_NOTIFY_STOP;
    msg_to_ui.pNotifyParam  = &stop_ind;
    stop_ind.hVideo = (HVIDEOHANDLE)fsm_ptr;
    //WMP_TRACE_LOW:"[Wre Player] SendAUTHIndToUI send VIDEO_NOTIFY_STOP to ui"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_3303_112_2_18_3_11_11_354,(uint8*)"");
    //给ui层发送VIDEO_NOTIFY_STOP
    fsm_ptr->vpCallBack(&msg_to_ui);
}

/*****************************************************************************/
//  Description : 给ui层发送VIDEO_NOTIFY_PLAY
//  Global resource dependence :  none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void SendPlayCnfToUI(VIDEO_PLAYER *fsm_ptr,VIDEO_ERROR_TYPE errorType)
{
    CALLBACKPARAM       msg_to_ui   = {0};
    VIDEO_PLAY      play_cnf   = {0};
    
    if (PNULL == fsm_ptr || !IsValidFsm(fsm_ptr) || PNULL == fsm_ptr->vpCallBack)
    {
        //WMP_TRACE_LOW:"[Wre Player] SendPlayCnfToUI param is error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_3319_112_2_18_3_11_11_355,(uint8*)"");
        return;
    }
    
    //创建VIDEO_NOTIFY_PLAY消息
    msg_to_ui.hVideo = (HVIDEOHANDLE)fsm_ptr;
    msg_to_ui.nNotifyID = VIDEO_NOTIFY_PLAY;
    msg_to_ui.pNotifyParam  = &play_cnf;
    play_cnf.hVideo = (HVIDEOHANDLE)fsm_ptr;
    play_cnf.errorType = errorType;
    //WMP_TRACE_LOW:"[Wre Player] SendPlayCnfToUI send WMP_PLAY_CNFto ui errorType = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_3329_112_2_18_3_11_11_356,(uint8*)"d",errorType);
    //给ui层发送VIDEO_NOTIFY_PLAY
    fsm_ptr->vpCallBack(&msg_to_ui);
}

/*****************************************************************************/
//  Description : 给ui层发送VIDEO_NOTIFY_PAUSE
//  Global resource dependence :  none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void SendPauseCnfToUI(VIDEO_PLAYER *fsm_ptr,VIDEO_ERROR_TYPE errorType)
{
    CALLBACKPARAM       msg_to_ui   = {0};
    VIDEO_PAUSE         pause_cnf   = {0};
    
    if (PNULL == fsm_ptr || !IsValidFsm(fsm_ptr) || PNULL == fsm_ptr->vpCallBack)
    {
        //WMP_TRACE_LOW:"[Wre Player] SendPauseCnfToUI param is error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_3345_112_2_18_3_11_11_357,(uint8*)"");
        return;
    }
    
    //创建VIDEO_NOTIFY_PAUSE消息
    msg_to_ui.hVideo = (HVIDEOHANDLE)fsm_ptr;
    msg_to_ui.nNotifyID = VIDEO_NOTIFY_PAUSE;
    msg_to_ui.pNotifyParam  = &pause_cnf;
    pause_cnf.hVideo = (HVIDEOHANDLE)fsm_ptr;
    pause_cnf.errorType = errorType;
    //WMP_TRACE_LOW:"[Wre Player] SendAUTHIndToUI send VIDEO_NOTIFY_PAUSE to ui"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_3355_112_2_18_3_11_11_358,(uint8*)"");
    //给ui层发送VIDEO_NOTIFY_PAUSE
    fsm_ptr->vpCallBack(&msg_to_ui);
}

/*****************************************************************************/
//  Description : 给ui层发送VIDEO_NOTIFY_SEEK
//  Global resource dependence :  none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void SendSeekCnfToUI(VIDEO_PLAYER *fsm_ptr,VIDEO_ERROR_TYPE errorType)
{
    CALLBACKPARAM       msg_to_ui   = {0};
    VIDEO_SEEK          seek_cnf    = {0};
    
    if (PNULL == fsm_ptr || !IsValidFsm(fsm_ptr) || PNULL == fsm_ptr->vpCallBack)
    {
        //WMP_TRACE_LOW:"[Wre Player] SendSeekCnfToUI param is error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_3371_112_2_18_3_11_11_359,(uint8*)"");
        return;
    }
    
    //创建VIDEO_NOTIFY_SEEK消息
    msg_to_ui.hVideo = (HVIDEOHANDLE)fsm_ptr;
    msg_to_ui.nNotifyID = VIDEO_NOTIFY_SEEK;
    msg_to_ui.pNotifyParam  = &seek_cnf;
    seek_cnf.hVideo = (HVIDEOHANDLE)fsm_ptr;
    seek_cnf.nCurtime = fsm_ptr->process_time;
    seek_cnf.nTotalTime = fsm_ptr->total_time;
    seek_cnf.errorType = errorType;
    //WMP_TRACE_LOW:"[Wre Player] SendAUTHIndToUI send VIDEO_NOTIFY_SEEK to ui"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_3383_112_2_18_3_11_11_360,(uint8*)"");
    //给ui层发送VIDEO_NOTIFY_SEEK
    fsm_ptr->vpCallBack(&msg_to_ui);
}


/*****************************************************************************/
//  Description : 通过frame获得对应的time
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL uint32 FrameToTime(DPLAYER_HANDLE video_handle, uint32 frame)
{
    DPLAYER_MEDIA_INFO_T        media_info          = {0};
    DPLAYER_RETURN_E            info_result         = DPLAYER_SUCCESS;
    uint32                      time_lenth          = 0;
    uint32                      total_frame         = 0;
    uint32                      time                = 0;

    
    if(0 < video_handle)
    {
        //获得视频文件信息
        info_result = DPLAYER_GetMovieInfo(video_handle, &media_info);
        //获得视频文件总的时长
        time_lenth = DPLAYER_GetLength(video_handle);
        
        if(DPLAYER_SUCCESS == info_result && time_lenth > 0 && 0 != media_info.video_info.frame_rate_base)
        {
            //计算视频文件总的帧数
            total_frame = (media_info.video_info.frame_rate)*time_lenth/SEC_TO_MSEL/media_info.video_info.frame_rate_base;
            //计算对应的time
            if((frame < total_frame) && (0 != media_info.video_info.frame_rate))
            {
                time = frame * SEC_TO_MSEL*media_info.video_info.frame_rate_base /media_info.video_info.frame_rate;
                if(time > time_lenth)
                {
                    time = time_lenth - 1;
                }
            }
            else if(frame >= total_frame)
            {
                time = time_lenth - 1;
            }
        }
    }

    //WMP_TRACE_LOW:"[VP]FrameToTime handle = 0x%x, info_result = %d,frame_index = %d, time_offset = 0x%x, time_lenth = %d,base = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_3429_112_2_18_3_11_12_361,(uint8*)"dddddd",video_handle,info_result,frame,time,time_lenth,media_info.video_info.frame_rate_base);
    return time;
}

/*****************************************************************************/
//  Description : frame end notify call back
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void CallbackFrameEndNotify(int32 frame_num)
{
	
	// WMP_TRACE_LOW("[Wre Player] CallbackFrameEndNotify frame_num = %d, s_current_fsm_ptr = %x, s_current_fsm_ptr->process_time =%d",frame_num,s_current_fsm_ptr,s_current_fsm_ptr->process_time);
    if (PNULL != s_current_fsm)
    {       
        SendSignalToCurPlayingTask(APP_WRE_VIDEO_FRAME_END_IND, (uint16)frame_num, s_current_fsm->task_id );   
    }   
}

/*****************************************************************************/
//  Description : end notify回调函数
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void CallbackEndNotify(int32 i_type)
{
     VIDEO_PLAY_END_RESULT playend_result = VIDEO_PLAY_END_NONE;	 
	
    //WMP_TRACE_LOW:"[Wre Player] CallbackEndNotify i_type = %d, s_current_fsm_ptr->task_id = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_3455_112_2_18_3_11_12_362,(uint8*)"dd",i_type,s_current_fsm->task_id);
	
    if (0 == i_type)
    {
        playend_result = VIDEO_PLAY_END_SUCCESS;
    }
    else if (0 > i_type)
    {
        if (-11 == i_type)
        {
            playend_result = VIDEO_PLAY_END_NOT_SUPPORT;
        }
        else
        {
            playend_result = VIDEO_PLAY_END_ERROR;
        }
    }
    else
    {
        playend_result = VIDEO_PLAY_END_STOPPED_BY_USER;
    }  
    
    if (PNULL != s_current_fsm)
    {
        SendSignalToCurPlayingTask(APP_WRE_VIDEO_END_IND, (uint16)playend_result, s_current_fsm->task_id );   
    }      
}

/*****************************************************************************/
//  Description : 开始播放本地文件
//  Global resource dependence :  none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL VIDEO_ERROR_TYPE PlayLocalFile(VIDEO_PLAYER *fsm_ptr, BOOLEAN is_need_fill)
{
    void                    *argv[2]    = {0}; 
    DPLAYER_MEDIA_INFO_T    p_info      = {0};  
    VIDEO_ERROR_TYPE                 result      = VIDEO_ERROR_OTHERS;
    DPLAYER_RETURN_E        dplayer_result      = DPLAYER_SUCCESS;
   // GUI_RECT_T lcd_rect = {0};
  //  GUI_LCD_DEV_INFO const *lcd_dev_info_ptr = (const GUI_LCD_DEV_INFO*)MMITHEME_GetDefaultLcdDev();
    LCD_VIDEO_DISPLAY_PARAM  display_param = {0};
    uint32 file_size = 0;
#ifdef DRM_SUPPORT
    DRMFILE_PRE_CHECK_STATUS_E drm_status = DRMFILE_PRE_CHECK_NORMAL;
#endif

    if(PNULL == fsm_ptr || !IsValidFsm(fsm_ptr))
    { 
        //WMP_TRACE_LOW:"[Wre Player] PlayLocalFile fsm is error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_3503_112_2_18_3_11_12_363,(uint8*)"");
        return result;
    }
#ifdef  VIDEOTHUMBNAIL_SUPPORT  
	 MMIAPIVP_ThumbnailClose();//add leicni
#endif

    MMIAPIFMM_GetFileInfo(fsm_ptr->strFilePath,fsm_ptr->uFilePathLen,&file_size,PNULL,PNULL);

    //WMP_TRACE_LOW:"[Wre Player] PlayLocalFile uFilePathLen = %d, strFilePath = %x, %x, %x, %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_3509_112_2_18_3_11_12_364,(uint8*)"ddddd",fsm_ptr->uFilePathLen,fsm_ptr->strFilePath[0],fsm_ptr->strFilePath[1],fsm_ptr->strFilePath[2],fsm_ptr->strFilePath[3]);
    if (!MMIAPIFMM_IsFileExist(fsm_ptr->strFilePath,fsm_ptr->uFilePathLen))
    {
        fsm_ptr->support_type = VIDEO_SUPPORTED_NEITHER;
        fsm_ptr->process_time = 0;
        fsm_ptr->total_time = 0;        
        SendPrepareIndToUI(fsm_ptr);
        result = VIDEO_ERROR_NOT_FOUND;
        return result;
    }
    else if (0 == file_size)
    {
        fsm_ptr->support_type = VIDEO_SUPPORTED_NEITHER;
        fsm_ptr->process_time = 0;
        fsm_ptr->total_time = 0;        
        SendPrepareIndToUI(fsm_ptr);
        result = VIDEO_ERROR_NULL_FILE;
        return result;
    }
#ifdef DRM_SUPPORT
    else
    {
        if(MMIAPIDRM_PreCheckFileStatusEX(fsm_ptr->strFilePath, DRM_PERMISSION_PLAY,&drm_status))
        {
            //StopVideo((HVIDEOHANDLE)fsm_ptr); 
            return result;
        }
        else
        {
            if(DRMFILE_PRE_CHECK_NO_SYS_NITZ == drm_status)
            {
                result = VIDEO_ERROR_DRM_NO_SYS_NITZ;
                return result;
            }
            else if(DRMFILE_PRE_CHECK_NO_RIGHTS == drm_status)
            {
                result = VIDEO_ERROR_DRM_RIGHT_INVALID;
                return result;
            }
        }
    }
#endif
    //打开本地媒体文件
    argv[0] = (void*)(fsm_ptr->strFilePath);
    if(fsm_ptr->is_share_play)
    {
        argv[1] = (void *)((uint32)DPLAYER_FA_MOD_ABPLUSE);
    }
    else
    {
        argv[1] = (void *)((uint32)DPLAYER_FA_MOD_RB);
    }
   
    DPLAYER_Init();
    fsm_ptr->dplayer_handle = DPLAYER_Open(DPLAYER_OPEN_FROM_FILE, 2, argv);    
    //WMP_TRACE_LOW:"[Wre Player] PlayLocalFile fsm_ptr->dplayer_handle = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_3564_112_2_18_3_11_12_365,(uint8*)"d",fsm_ptr->dplayer_handle);
    
    if(0 < fsm_ptr->dplayer_handle)
    {
        //获取当前媒体文件总的播放时间和当前的播放时间
        fsm_ptr->total_time = DPLAYER_GetLength(fsm_ptr->dplayer_handle);
        
        fsm_ptr->process_time = 0;
        dplayer_result = DPLAYER_GetMovieInfo(fsm_ptr->dplayer_handle,&p_info);
        if (DPLAYER_ERROR_UNSUPPORTED_FORMAT == dplayer_result)
        {
            fsm_ptr->support_type = VIDEO_SUPPORTED_NEITHER;
        }
        else 
        {
            fsm_ptr->support_type = CheckSupportedTypeFromMediaInfo(&p_info);
        }
        fsm_ptr->LcdDisplayParam.LcdBlkId = 0;
        fsm_ptr->LcdDisplayParam.LcdId = 0;
       // fsm_ptr->display_param.disp_mode = DPLAYER_DISP_FULLVIEW;//DPLAYER_DISP_CUSTOMIZE;
        
        //WMP_TRACE_LOW:"[Wre Player] PlayLocalFile play_param_ptr->is_scal_up = %d, fsm_ptr->display_param.Width = %d, p_info.video_info.width = %d, fsm_ptr->display_param.Height = %d, p_info.video_info.height = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_3586_112_2_18_3_11_12_366,(uint8*)"ddddd",fsm_ptr->is_scal_up,fsm_ptr->LcdDisplayParam.disp_rect.dx,p_info.video_info.width,fsm_ptr->LcdDisplayParam.disp_rect.dy,p_info.video_info.height);
     
        
        //给ui发送VIDEO_NOTIFY_PREPARE消息
        SendPrepareIndToUI(fsm_ptr);

        if (VIDEO_SUPPORTED_NEITHER == fsm_ptr->support_type)
        {
            result = VIDEO_ERROR_NOT_SUPPORT;           
        }
        //WMP_TRACE_LOW:"[Wre Player] PlayLocalFile total_time = %d, process_time = %d, support_type = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_3596_112_2_18_3_11_12_367,(uint8*)"ddd",fsm_ptr->total_time,fsm_ptr->process_time,fsm_ptr->support_type);
        //设置每帧显示完毕后的回调函数
        DPLAYER_SetFrameEndNotify(fsm_ptr->dplayer_handle, CallbackFrameEndNotify);
        
        //设置播放结束的回调函数
        DPLAYER_SetEndNotify(fsm_ptr->dplayer_handle, CallbackEndNotify);       
        
        //设置播放显示参数      
        if (is_need_fill
            && (VIDEO_SUPPORTED_NEITHER == fsm_ptr->support_type
            || VIDEO_SUPPORTED_AUDIO_ONLY == fsm_ptr->support_type))
        {
             is_need_fill = FALSE;
        }

        SCI_MEMCPY(&display_param,&(fsm_ptr->LcdDisplayParam),sizeof(LCD_VIDEO_DISPLAY_PARAM));
        TranslateDisplayParamLogicToPhysical(&display_param, is_need_fill);

        dplayer_result      = DPLAYER_SetDisplayParam(fsm_ptr->dplayer_handle,&display_param);

        //开始播放
        if(DPLAYER_SUCCESS == dplayer_result)            
        {
            if(0 != fsm_ptr->LcdDisplayParam.audioSupport)
            {
                MMIAPISET_InitAudioDevice(fsm_ptr->vol);
                SwitchPlayDevice((HVIDEOHANDLE)fsm_ptr,fsm_ptr->bSupportA2dp,fsm_ptr->is_bt_support);
            }
            dplayer_result      = DPLAYER_Play(fsm_ptr->dplayer_handle);
            //保存历史记录  
            //WMP_TRACE_LOW:"[Wre Player] PlayLocalFile  dplayer_result = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_3626_112_2_18_3_11_12_368,(uint8*)"d",dplayer_result);
            switch(dplayer_result) 
            {
            case DPLAYER_SUCCESS:                
#ifdef DRM_SUPPORT
                ConsumeDrmRight(fsm_ptr->strFilePath,fsm_ptr->uFilePathLen);
#endif
                result = VIDEO_ERROR_NONE;
                break;
            case DPLAYER_ERROR_FILE_NOT_FOUND:
                result = VIDEO_ERROR_NOT_FOUND;
                break;
            case DPLAYER_ERROR_UNSUPPORTED_IMAGE_SIZE:
            case DPLAYER_ERROR_UNSUPPORTED_FORMAT:
                result = VIDEO_ERROR_NOT_SUPPORT;
                break;

            default:
                break;
            }
            
        }
        if(result != VIDEO_ERROR_NONE)
        {
            //WMP_TRACE_LOW:"[Wre Player] PlayLocalFile play error"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_3650_112_2_18_3_11_12_369,(uint8*)"");
            DPLAYER_Close(fsm_ptr->dplayer_handle);
            fsm_ptr->dplayer_handle = 0;           
            DPLAYER_Release();
        }
    }
    else
    {
        fsm_ptr->support_type = VIDEO_SUPPORTED_NEITHER;
        fsm_ptr->process_time = 0;
        fsm_ptr->total_time = 0;        
        SendPrepareIndToUI(fsm_ptr);
        result = VIDEO_ERROR_NOT_SUPPORT;
    }
    return result;
}

/*****************************************************************************/
//  Description : 开始播放本地buf
//  Global resource dependence :  none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL VIDEO_ERROR_TYPE PlayLocalBuf(VIDEO_PLAYER *fsm_ptr, VIDEO_PLAY_PARAM  *play_param_ptr)
{
    void                    *argv[3]            = {0}; 
    DPLAYER_FILE_TYPE_E     file_type           = DPLAYER_FILETYPE_3GP;
    DPLAYER_MEDIA_INFO_T    p_info              = {0};
    VIDEO_ERROR_TYPE                 result              = VIDEO_ERROR_OTHERS;
    DPLAYER_RETURN_E        dplayer_result      = DPLAYER_SUCCESS;
   // GUI_RECT_T lcd_rect = {0};
  //  GUI_LCD_DEV_INFO const *lcd_dev_info_ptr = (const GUI_LCD_DEV_INFO*)MMITHEME_GetDefaultLcdDev();
    BOOLEAN                  is_need_fill       = TRUE;    
    LCD_VIDEO_DISPLAY_PARAM          play_param         = {0};

    //参数检查
    if ((PNULL == fsm_ptr) || !IsValidFsm(fsm_ptr) || (PNULL == play_param_ptr))
    {
        //SCI_TRACE_LOW:"[Wre Player] PlayLocalBuf param is error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_3686_112_2_18_3_11_12_370,(uint8*)"");
        return result;
    }
    
    /* open mplayer */
    argv[0] = (void*)(play_param_ptr->videoBufferInfo.pVideoBuffer);
    argv[1] = (void*)play_param_ptr->videoBufferInfo.nVideoBufferLen;

    //SCI_TRACE_LOW:"[Wre Player] PlayLocalBuf play_param_ptr->videoBufferInfo.fileYype = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_3694_112_2_18_3_11_12_371,(uint8*)"d",play_param_ptr->videoBufferInfo.fileType);
 
   
    switch(play_param_ptr->videoBufferInfo.fileType)
    {
    case VIDEO_FILE_AVI:
        file_type = DPLAYER_FILETYPE_AVI;
        break;
    case VIDEO_FILE_3GP:
        file_type = DPLAYER_FILETYPE_3GP;
        break;
    case VIDEO_FILE_MP4:
        file_type = DPLAYER_FILETYPE_MP4;
        break;
    case VIDEO_FILE_FLV:
        file_type = DPLAYER_FILETYPE_FLV;
        break;
    case VIDEO_FILE_RMVB:
        file_type = DPLAYER_FILETYPE_RMVB;
        break;        
    default:
        return VIDEO_ERROR_NOT_SUPPORT;
    }
    
    argv[2] = &file_type;
    DPLAYER_Init();
    fsm_ptr->dplayer_handle = DPLAYER_Open(DPLAYER_OPEN_FROM_MEM, 3, argv);
    //SCI_TRACE_LOW:"[Wre Player] PlayLocalBuf fsm_ptr->dplayer_handle = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_3721_112_2_18_3_11_12_372,(uint8*)"d",fsm_ptr->dplayer_handle);
    if(0 < fsm_ptr->dplayer_handle)
    {
        //获取当前媒体文件总的播放时间和当前的播放时间
        fsm_ptr->total_time = DPLAYER_GetLength(fsm_ptr->dplayer_handle);
        fsm_ptr->process_time = DPLAYER_GetTime(fsm_ptr->dplayer_handle);
        dplayer_result = DPLAYER_GetMovieInfo(fsm_ptr->dplayer_handle,&p_info);
        if (DPLAYER_ERROR_UNSUPPORTED_FORMAT == dplayer_result)
        {
            fsm_ptr->support_type = VIDEO_SUPPORTED_NEITHER;
        }        
        else 
        {
            fsm_ptr->support_type = CheckSupportedTypeFromMediaInfo(&p_info);
        }      
        //WMP_TRACE_LOW:"[Wre Player] PlayLocalBuf vidot_type = %d, vidoe_support = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_3736_112_2_18_3_11_12_373,(uint8*)"dd",p_info.video_info.video_type,p_info.video_info.video_support);
        fsm_ptr->LcdDisplayParam.LcdBlkId = 0;
        fsm_ptr->LcdDisplayParam.LcdId = 0;
    //    fsm_ptr->display_param.disp_mode = DPLAYER_DISP_CUSTOMIZE;
        //WMP_TRACE_LOW:"[Wre Player] PlayLocalBuf play_param_ptr->bScalUp = %d, fsm_ptr->display_param.nWidth = %d, p_info.video_info.nWidth = %d, fsm_ptr->display_param.Height = %d, p_info.video_info.height = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_3741_112_2_18_3_11_12_374,(uint8*)"ddddd",play_param_ptr->bScalUp,fsm_ptr->LcdDisplayParam.disp_rect.dx,p_info.video_info.width,fsm_ptr->LcdDisplayParam.disp_rect.dy,p_info.video_info.height);
      /*  if (!(play_param_ptr->is_scal_up) && fsm_ptr->display_param.Width >= p_info.video_info.width && fsm_ptr->display_param.Height >= p_info.video_info.height)
        {
            fsm_ptr->display_param.xOffset = fsm_ptr->display_param.xOffset + ((fsm_ptr->display_param.Width - p_info.video_info.width) >> 1);
            fsm_ptr->display_param.yOffset = fsm_ptr->display_param.yOffset + ((fsm_ptr->display_param.Height - p_info.video_info.height) >> 1);
            fsm_ptr->display_param.Width = p_info.video_info.width;
            fsm_ptr->display_param.Height = p_info.video_info.height;
        }*/

        //给ui发送VIDEO_NOTIFY_PREPARE消息
        SendPrepareIndToUI(fsm_ptr);        
        
        //WMP_TRACE_LOW:"[Wre Player] PlayLocalBuf total_time = %d, process_time = %d, support_type = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_3753_112_2_18_3_11_12_375,(uint8*)"ddd",fsm_ptr->total_time,fsm_ptr->process_time,fsm_ptr->support_type);
        
        //设置每帧显示完毕后的回调函数
        DPLAYER_SetFrameEndNotify(fsm_ptr->dplayer_handle, CallbackFrameEndNotify);
        
        //设置播放结束的回调函数
        DPLAYER_SetEndNotify(fsm_ptr->dplayer_handle, CallbackEndNotify);        

        if (VIDEO_SUPPORTED_NEITHER == fsm_ptr->support_type
            || VIDEO_SUPPORTED_AUDIO_ONLY == fsm_ptr->support_type)
        {
             is_need_fill = FALSE;
        }

        SCI_MEMCPY(&play_param,&(fsm_ptr->LcdDisplayParam),sizeof(LCD_VIDEO_DISPLAY_PARAM));
        TranslateDisplayParamLogicToPhysical(&play_param, is_need_fill);

        dplayer_result = DPLAYER_SetDisplayParam(fsm_ptr->dplayer_handle,&play_param);
        //开始播放
        if (DPLAYER_SUCCESS == dplayer_result)
        { 
            if(0 != fsm_ptr->LcdDisplayParam.audioSupport)
            {
            	#ifdef MMISRV_AUDIO_SUPPORT  //leichi
	    	MMISRVAUD_SetVolume((MMISRV_HANDLE_T)fsm_ptr->dplayer_handle, (MMISRVAUD_VOLUME_T)fsm_ptr->vol);
		#else
                MMIAPISET_InitAudioDevice(fsm_ptr->vol);
		#endif
                SwitchPlayDevice((HVIDEOHANDLE)fsm_ptr,fsm_ptr->bSupportA2dp,fsm_ptr->is_bt_support);
            }
            dplayer_result = DPLAYER_Play(fsm_ptr->dplayer_handle);
            //WMP_TRACE_LOW:"[Wre Player] PlayLocalBuf dplayer_result = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_3780_112_2_18_3_11_12_376,(uint8*)"d",dplayer_result);
            switch(dplayer_result) 
            {
            case DPLAYER_SUCCESS:
                result = VIDEO_ERROR_NONE;
                break;
            case DPLAYER_ERROR_UNSUPPORTED_FORMAT:
                result = VIDEO_ERROR_NOT_SUPPORT;
                break;
            default:
                break;
            }
        }
        if (result != VIDEO_ERROR_NONE)
        {        
            //WMP_TRACE_LOW:"[Wre Player] PlayLocalBuf play error"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_3795_112_2_18_3_11_12_377,(uint8*)"");
            DPLAYER_Close(fsm_ptr->dplayer_handle);
            fsm_ptr->dplayer_handle = 0;    
            DPLAYER_Release();
        }
    }
    else
    {
        SendPrepareIndToUI(fsm_ptr);
        result = VIDEO_ERROR_NOT_SUPPORT;
    }
    return result;
}

/*****************************************************************************/
//  Description : 开始播放网络文件
//  Global resource dependence :  none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL VIDEO_ERROR_TYPE PlayFromNet(VIDEO_PLAYER *fsm_ptr,DPLAYER_STRM_CFG_T strm_cfg)
{
    void                *argv[1]        = {0};    
    DPLAYER_MEDIA_INFO_T p_info         = {0}; 
    VIDEO_ERROR_TYPE              result         = VIDEO_ERROR_OTHERS;
#ifdef MMI_STREAMING_PLAYER_SUPPORT //20120612

    DPLAYER_RETURN_E     dplayer_result = DPLAYER_SUCCESS;
   // GUI_RECT_T lcd_rect = {0};
   // GUI_LCD_DEV_INFO const *lcd_dev_info_ptr = (const GUI_LCD_DEV_INFO*)MMITHEME_GetDefaultLcdDev();
    BOOLEAN    is_need_fill = TRUE;
    LCD_VIDEO_DISPLAY_PARAM play_param = {0};

    if (PNULL == fsm_ptr || !IsValidFsm(fsm_ptr))
    {
        //WMP_TRACE_LOW:"[Wre Player] PlayFromNet param is error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_3826_112_2_18_3_11_12_378,(uint8*)"");
        return result;
    }

    //create dplayer handle
    argv[0] = (void*)(&strm_cfg);
    DPLAYER_Init();
    fsm_ptr->dplayer_handle = DPLAYER_Open(DPLAYER_OPEN_FROM_NET, 1, argv);    
    //WMP_TRACE_LOW:"[Wre Player] PlayFromNet fsm_ptr->dplayer_handle = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_3834_112_2_18_3_11_12_379,(uint8*)"d",fsm_ptr->dplayer_handle);
    if(0 < fsm_ptr->dplayer_handle)
    {
        //获取当前媒体文件总的播放时间和当前的播放时间
        fsm_ptr->total_time = DPLAYER_GetLength(fsm_ptr->dplayer_handle);
        if (0 == fsm_ptr->total_time)
        {
            fsm_ptr->total_time = MAX_INVALID_VALUE;
        }
        fsm_ptr->process_time = 0;
        dplayer_result = DPLAYER_GetMovieInfo(fsm_ptr->dplayer_handle,&p_info);
        if (DPLAYER_ERROR_UNSUPPORTED_FORMAT == dplayer_result)
        {
            fsm_ptr->support_type = VIDEO_SUPPORTED_NEITHER;
        }        
        else 
        {
            fsm_ptr->support_type = CheckSupportedTypeFromMediaInfo(&p_info);
        }
      
        fsm_ptr->LcdDisplayParam.LcdBlkId = 0;
        fsm_ptr->LcdDisplayParam.LcdId = 0;
      //  fsm_ptr->display_param.disp_mode = DPLAYER_DISP_FULLVIEW;//DPLAYER_DISP_CUSTOMIZE;
        
        //WMP_TRACE_LOW:"[Wre Player] PlayFromNet play_param_ptr->is_scal_up = %d, fsm_ptr->display_param.Width = %d, p_info.video_info.width = %d, fsm_ptr->display_param.Height = %d, p_info.video_info.height = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_3859_112_2_18_3_11_12_380,(uint8*)"ddddd",fsm_ptr->is_scal_up,fsm_ptr->LcdDisplayParam.disp_rect.dx,p_info.video_info.width,fsm_ptr->LcdDisplayParam.disp_rect.dy,p_info.video_info.height);
      /*  if (!(fsm_ptr->is_scal_up) && fsm_ptr->display_param.Width >= p_info.video_info.width && fsm_ptr->display_param.Height >= p_info.video_info.height)
        {
            fsm_ptr->display_param.xOffset = fsm_ptr->display_param.xOffset + ((fsm_ptr->display_param.Width - p_info.video_info.width) >> 1);
            fsm_ptr->display_param.yOffset = fsm_ptr->display_param.yOffset + ((fsm_ptr->display_param.Height - p_info.video_info.height) >> 1);
            fsm_ptr->display_param.Width = p_info.video_info.width;
            fsm_ptr->display_param.Height = p_info.video_info.height;
        }*/
        
        //给ui发送VIDEO_NOTIFY_PREPARE消息
        SendPrepareIndToUI(fsm_ptr);

        //WMP_TRACE_LOW:"[Wre Player] PlayFromNet total_time = %d, process_time = %d, support_type = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_3871_112_2_18_3_11_12_381,(uint8*)"ddd",fsm_ptr->total_time,fsm_ptr->process_time,fsm_ptr->support_type);
                
        //设置每帧显示完毕后的回调函数
        DPLAYER_SetFrameEndNotify(fsm_ptr->dplayer_handle, CallbackFrameEndNotify);
        
        //设置播放结束的回调函数
        DPLAYER_SetEndNotify(fsm_ptr->dplayer_handle, CallbackEndNotify);
      
        
        //设置播放显示参数
        //开始播放
        if (!fsm_ptr->is_to_pause)
        {   
            if (VIDEO_SUPPORTED_NEITHER == fsm_ptr->support_type
            || VIDEO_SUPPORTED_AUDIO_ONLY == fsm_ptr->support_type)
            {
                 is_need_fill = FALSE;
            }

            SCI_MEMCPY(&play_param,&(fsm_ptr->LcdDisplayParam),sizeof(LCD_VIDEO_DISPLAY_PARAM));
            TranslateDisplayParamLogicToPhysical(&play_param, FALSE);
        
            MMIAPISET_InitAudioDevice(fsm_ptr->vol);     
            SwitchPlayDevice((HVIDEOHANDLE)fsm_ptr,fsm_ptr->bSupportA2dp,fsm_ptr->is_bt_support);
            
            if(DPLAYER_SUCCESS != DPLAYER_SetDisplayParam(fsm_ptr->dplayer_handle,&play_param) 
                ||DPLAYER_SUCCESS != DPLAYER_Play(fsm_ptr->dplayer_handle) 
                || !STRM_Play(fsm_ptr->strm_handle,fsm_ptr->dplayer_handle))
            {
                //WMP_TRACE_LOW:"[Wre Player] PlayFromNet play error"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_3900_112_2_18_3_11_13_382,(uint8*)"");
                DPLAYER_Stop(fsm_ptr->dplayer_handle);
                DPLAYER_Close(fsm_ptr->dplayer_handle);
                fsm_ptr->dplayer_handle = 0;    
                DPLAYER_Release();
                return result;
            }            
        }

        result = VIDEO_ERROR_NONE;
    }
    else
    {
        SendPrepareIndToUI(fsm_ptr);
        result = VIDEO_ERROR_NOT_SUPPORT;
    }
#endif
    return result;
}

/*****************************************************************************/
//  Description : 处理pdp消息
//  Global resource dependence :  none
//  Author: 
//  Note: 
/*****************************************************************************/

LOCAL void HandlePDPMsg(MMIPDP_CNF_INFO_T *msg_ptr)
{   
#ifdef MMI_STREAMING_PLAYER_SUPPORT //20120612

    FSM_MSG_E        msg_id              = 0;
    void                *msg_body_ptr       = PNULL;
    uint32              msg_body_size       = 0;
    
    if (PNULL == s_current_fsm || PNULL == msg_ptr)
    {
        //WMP_TRACE_LOW:"[Wre Player] HandlePDPMsg param is error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_3932_112_2_18_3_11_13_383,(uint8*)"");
        return;
    }
    
    //WMP_TRACE_LOW:"[Wre Player] HandlePDPMsg msg_ptr->msg_id = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_3936_112_2_18_3_11_13_384,(uint8*)"d",msg_ptr->msg_id);
    switch(msg_ptr->msg_id) 
    {
    case MMIPDP_ACTIVE_CNF:
        msg_id = FSM_PDP_ACTIVE_CNF;
        msg_body_size = sizeof(MMIPDP_CNF_INFO_T);
        msg_body_ptr = msg_ptr;       
        break;
        
    case MMIPDP_DEACTIVE_IND:
        SendFatalErrorIndTOUI(s_current_fsm,VIDEO_ERROR_SERVER_CLOSE);
        msg_id = FSM_PDP_DEACTIVE_IND;
        msg_body_size = 0;
        msg_body_ptr = PNULL;            
        break;
        
    default:
        return;
    }

    //向 control发送消息
    FsmDispatch((HVIDEOHANDLE)s_current_fsm,msg_id,msg_body_ptr,msg_body_size);     
  #endif  
}


/*****************************************************************************/
//  Description : 激活pdp
//  Global resource dependence :  none
//  Author: 
//  Note: 
/*****************************************************************************/

LOCAL BOOLEAN ActivePdp(VIDEO_PLAYER *fsm_ptr)
{
	/*
#ifdef MMI_GPRS_SUPPORT
    MMIPDP_ACTIVE_INFO_T                    app_info            = {0};
    MN_DUAL_SYS_E                           dual_sys            = 0;
    uint8                                   setting_index       = 0;
    MMICONNECTION_LINKSETTING_DETAIL_T      *setting_item_ptr  = PNULL;
    
    if (PNULL == fsm_ptr || !IsValidFsm(fsm_ptr))
    {
        //WMP_TRACE_LOW:"[Wre Player] ActivePdp param is error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_3978_112_2_18_3_11_13_385,(uint8*)"");
        return FALSE;
    }
   
    //dual_sys = fsm_ptr->dual_sys;
    //WMP_TRACE_LOW:"[Wre Player] ActivePdp dual_sys = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_3983_112_2_18_3_11_13_386,(uint8*)"d",dual_sys);
    	
    //app_info.app_handler = MMI_MODULE_VP;

    if (MMI_DUAL_SYS_MAX != dual_sys)
    {    
        if (!MMIAPIPHONE_IsSimAvailable(dual_sys))
        {
            //WMP_TRACE_LOW:"[Wre Player] ActivePdp sim casd is not ok dual_sys = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_3991_112_2_18_3_11_13_387,(uint8*)"d",dual_sys);
            return FALSE;
        }
    
        //setting_index = VP_Setting_GetNetIndex(dual_sys);    
        setting_item_ptr = MMIAPICONNECTION_GetLinkSettingItemByIndex(dual_sys, setting_index);
    
        if (PNULL == setting_item_ptr)
        {
            //WMP_TRACE_LOW:"[Wre Player] ActivePdp param is setting_index = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_4000_112_2_18_3_11_13_388,(uint8*)"d",setting_index);
            return FALSE;
        }

        
        app_info.auth_type = setting_item_ptr->auth_type;
        app_info.apn_ptr = (char*)setting_item_ptr->apn;
        
        app_info.user_name_ptr = (char*)setting_item_ptr->username;
        app_info.psw_ptr = (char*)setting_item_ptr->password;
        app_info.dual_sys = dual_sys;
        

         MNGPRS_ReadStorageExtReqQosEx(dual_sys,MN_GPRS_STORAGE_ALL,STREAMING_E,&app_info.qos_info);
    }
    else
    {
#ifdef WIFI_SUPPORT
        app_info.apn_ptr = PNULL;
        app_info.ps_interface = MMIPDP_INTERFACE_WIFI;
#else
        if (!MMIAPIPHONE_IsSimAvailable(dual_sys))
        {
            //WMP_TRACE_LOW:"[Wre Player] ActivePdp sim casd is not ok dual_sys = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_4023_112_2_18_3_11_13_389,(uint8*)"d",dual_sys);
            return FALSE;
        }        
#endif
    }
    app_info.priority = 3;    
    app_info.handle_msg_callback = HandlePDPMsg;   
    app_info.ps_service_rat = MN_TD_PREFER;
    s_current_fsm = fsm_ptr;

    if (!MMIAPIPDP_Active(&app_info))
    {
        return FALSE;
    }
    return TRUE;
#else
    return FALSE;
#endif
	*/
	return TRUE;
}


/*****************************************************************************/
//  Description : 空闲状态的处理
//  Global resource dependence :  none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN FsmIdle(VIDEO_PLAYER *fsm_ptr, VIDEO_FSM_MSG *msg_ptr)
{
    BOOLEAN    result = TRUE;
    //参数检查
    if(PNULL == fsm_ptr || !IsValidFsm(fsm_ptr) || PNULL == msg_ptr)
    {
        //WMP_TRACE_LOW:"[Wre Player] FsmIdle param is error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_4056_112_2_18_3_11_13_390,(uint8*)"");
        return FALSE;
    }
    
    //WMP_TRACE_LOW:"[Wre Player] FsmIdle msg_ptr->msg_id = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_4060_112_2_18_3_11_13_391,(uint8*)"d",msg_ptr->msg_id);
    switch(msg_ptr->msg_id)
    {
    case FSM_INIT_REQ:
        FsmStateTrans(fsm_ptr,VIDEO_STATE_READY);        
        break;
        
    default:
        //WMP_TRACE_LOW:"IDLE, got wrong msg"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_4068_112_2_18_3_11_13_392,(uint8*)"");
        result =  FALSE;
        break;
    }
    return result;
}

/*****************************************************************************/
//  Description : 初始化状态机信息
//  Global resource dependence :  none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void InitFsmInfo(VIDEO_PLAYER *fsm_ptr, VIDEO_STATE  state, VideoProc  vpCallBack)
{   
    uint32 task_id = 0;
    BOOLEAN is_exit = FALSE;
    BOOLEAN is_pause = FALSE;
    BOOLEAN is_resume = FALSE;

    if (PNULL == fsm_ptr || !IsValidFsm(fsm_ptr))
    {
        //WMP_TRACE_LOW:"[Wre Player] InitFsmInfo param is null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_4088_112_2_18_3_11_13_393,(uint8*)"");
        return;
    }
    //WMP_TRACE_LOW:"[Wre Player] InitFsmInfo state = %d, vpCallBack = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_4091_112_2_18_3_11_13_394,(uint8*)"dd",state,vpCallBack);
    
    if (PNULL != fsm_ptr->strFilePath)
    {
        SCI_FREE(fsm_ptr->strFilePath);
        fsm_ptr->strFilePath = PNULL;
    }

    if (PNULL != fsm_ptr->url_ptr)
    {
        SCI_FREE(fsm_ptr->url_ptr);
        fsm_ptr->url_ptr = PNULL;
    }
    task_id = fsm_ptr->task_id;
    is_exit = fsm_ptr->is_to_exit;
    is_pause = fsm_ptr->is_to_pause;
    is_resume = fsm_ptr->is_to_resume;
    SCI_MEMSET(fsm_ptr,0,sizeof(VIDEO_PLAYER));
    fsm_ptr->is_to_exit = is_exit;
    fsm_ptr->is_to_pause = is_pause;
    fsm_ptr->is_to_resume = is_resume;
    fsm_ptr->state = state;
    fsm_ptr->vpCallBack = vpCallBack;
    fsm_ptr->task_id = task_id;
}

/*****************************************************************************/
//  Description : 更新状态机信息
//  Global resource dependence :  none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN UpdateFsmInfo(VIDEO_PLAYER *fsm_ptr, VIDEO_PLAY_PARAM* play_param_ptr)
{
    BOOLEAN result = TRUE;

    if (PNULL == fsm_ptr || !IsValidFsm(fsm_ptr) || PNULL == play_param_ptr)
    {
        //WMP_TRACE_LOW:"[Wre Player] UpdateFsmInfo param is null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_4127_112_2_18_3_11_13_395,(uint8*)"");
        return FALSE;
    }
    
    if (VIDEO_PLAY_FROM_NET == play_param_ptr->playType && PNULL != play_param_ptr->strUrl)
    {        
        fsm_ptr->url_ptr = (uint8 *)SCI_ALLOCA(strlen(play_param_ptr->strUrl)+1);
        if (PNULL != fsm_ptr->url_ptr)
        {
            SCI_MEMSET(fsm_ptr->url_ptr,0,(strlen(play_param_ptr->strUrl)+1));/*lint !e666*/
            if(0 == strnicmp(play_param_ptr->strUrl,HTTP_URL_HEAD_ASIC,HTTP_URL_HEAD_LEN))
            {            
                SCI_MEMCPY(fsm_ptr->url_ptr,HTTP_URL_HEAD_ASIC,HTTP_URL_HEAD_LEN);  
                SCI_MEMCPY((fsm_ptr->url_ptr+HTTP_URL_HEAD_LEN),(play_param_ptr->strUrl+HTTP_URL_HEAD_LEN),(strlen(play_param_ptr->strUrl)-HTTP_URL_HEAD_LEN));     /*lint !e666*/
            }
            else if(0 == strnicmp(play_param_ptr->strUrl,RTSP_URL_HEAD_ASIC,RTSP_URL_HEAD_LEN))
            {
                SCI_MEMCPY(fsm_ptr->url_ptr,RTSP_URL_HEAD_ASIC,RTSP_URL_HEAD_LEN);  
                SCI_MEMCPY((fsm_ptr->url_ptr+RTSP_URL_HEAD_LEN),(play_param_ptr->strUrl+RTSP_URL_HEAD_LEN),(strlen(play_param_ptr->strUrl)-RTSP_URL_HEAD_LEN));     /*lint !e666*/
            }
            else
            {
                SCI_FREE(fsm_ptr->url_ptr);
                fsm_ptr->url_ptr = PNULL;
                return FALSE;
            }
        }
        else
        {
            return FALSE;
        }
    }
    
    if (PNULL != play_param_ptr->strFilePath && 0 != play_param_ptr->uFilePathLen)
    {        
        fsm_ptr->strFilePath = (wchar *)SCI_ALLOCA((play_param_ptr->uFilePathLen + 1)*sizeof(wchar));
        if (PNULL != fsm_ptr->strFilePath)
        {
            SCI_MEMSET(fsm_ptr->strFilePath,0,((play_param_ptr->uFilePathLen + 1)*sizeof(wchar)));
            //WMP_TRACE_LOW:"[Wre Player] UpdateFsmInfo fsm_ptr->strFilePath = %x, play_param_ptr->strFilePath = %x"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_4166_112_2_18_3_11_13_396,(uint8*)"dd",fsm_ptr->strFilePath,play_param_ptr->strFilePath);
            fsm_ptr->strFilePath = MMIAPICOM_Wstrncpy(fsm_ptr->strFilePath,play_param_ptr->strFilePath,play_param_ptr->uFilePathLen);
            fsm_ptr->uFilePathLen = play_param_ptr->uFilePathLen;
        }       
        else
        {
            result = FALSE;
        }
    }
    else
    {
        result = FALSE;
    }

    if (!result && (VIDEO_PLAY_FROM_FILE == play_param_ptr->playType))
    {
        return FALSE;
    }

    //SCI_MEMCPY(&(fsm_ptr->display_param),&(play_param_ptr->displayParam),sizeof(VIDEO_DISPLAY_PARAM));   
    switch(play_param_ptr->displayParam.dispMode)
    {
	    case VIDEO_DISP_FULLVIEW:
            fsm_ptr->LcdDisplayParam.disp_mode = DPLAYER_DISP_FULLVIEW;
            break;
        case VIDEO_DISP_FULLSCREEN:
            fsm_ptr->LcdDisplayParam.disp_mode = DPLAYER_DISP_FULLSCREEN;
            break;
        case VIDEO_DISP_CUSTOMIZE:
            fsm_ptr->LcdDisplayParam.disp_mode = DPLAYER_DISP_CUSTOMIZE;
            break;
        case VIDEO_DISP_MODE_MAX:
            fsm_ptr->LcdDisplayParam.disp_mode = DPLAYER_DISP_MODE_MAX;
            break;
    }

	fsm_ptr->LcdDisplayParam.audioSupport = (unsigned long int)play_param_ptr->displayParam.audioSupport;
	fsm_ptr->LcdDisplayParam.videoSupport = (unsigned long int)play_param_ptr->displayParam.videoSupport;


	fsm_ptr->LcdDisplayParam.target_rect.dx = play_param_ptr->displayParam.targetRect.dx;
	fsm_ptr->LcdDisplayParam.target_rect.dy = play_param_ptr->displayParam.targetRect.dy;
	fsm_ptr->LcdDisplayParam.target_rect.x  = play_param_ptr->displayParam.targetRect.x;
	fsm_ptr->LcdDisplayParam.target_rect.y  = play_param_ptr->displayParam.targetRect.y;

	fsm_ptr->LcdDisplayParam.disp_rect.dx = play_param_ptr->displayParam.dispRect.dx;
	fsm_ptr->LcdDisplayParam.disp_rect.dy = play_param_ptr->displayParam.dispRect.dy;
	fsm_ptr->LcdDisplayParam.disp_rect.x  = play_param_ptr->displayParam.dispRect.x;
	fsm_ptr->LcdDisplayParam.disp_rect.y  = play_param_ptr->displayParam.dispRect.y;
		
	fsm_ptr->LcdDisplayParam.RotateAngle = play_param_ptr->displayParam.RotateAngle;
	fsm_ptr->LcdDisplayParam.LcdBlkId = 0;
	fsm_ptr->LcdDisplayParam.LcdId    = 0;

    fsm_ptr->is_scal_up = play_param_ptr->bScalUp; //fsm_ptr->dual_sys = play_param_ptr->NetID;
    fsm_ptr->netID = play_param_ptr->uNetID;    
    fsm_ptr->playType = play_param_ptr->playType;
    fsm_ptr->vol = play_param_ptr->uVol;
    fsm_ptr->is_bt_support = play_param_ptr->bSupportBT;
    fsm_ptr->bSupportA2dp = play_param_ptr->bSupportA2DP;
    fsm_ptr->is_share_play = play_param_ptr->bSharePlay;	
    return TRUE;
}

/*****************************************************************************/
//  Description : 准备好状态的处理
//  Global resource dependence :  none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN FsmReady(VIDEO_PLAYER *fsm_ptr, VIDEO_FSM_MSG *msg_ptr)
{    
    BOOLEAN             result         = TRUE;    
    VIDEO_PLAY_PARAM     * play_param_ptr = PNULL;
    VIDEO_ERROR_TYPE          errorType = VIDEO_ERROR_OTHERS;
    
    //参数检查
    if(PNULL == fsm_ptr || !IsValidFsm(fsm_ptr) || PNULL == msg_ptr ||  PNULL == fsm_ptr->vpCallBack)
    {
        //WMP_TRACE_LOW:"[Wre Player] FsmReady play param is null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_4243_112_2_18_3_11_13_397,(uint8*)"");
        return FALSE;
    }

    //WMP_TRACE_LOW:"[Wre Player] FsmReady msg_ptr->msg_id = %x, fsm_ptr->has_play_end_reason = %d, fsm_ptr->is_to_exit = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_4247_112_2_18_3_11_13_398,(uint8*)"ddd",msg_ptr->msg_id,fsm_ptr->has_play_end_reason,fsm_ptr->is_to_exit);
    switch(msg_ptr->msg_id)
    {
    case FSM_ENTRY_REQ:
        fsm_ptr->is_to_pause = FALSE;
        fsm_ptr->is_to_resume = FALSE;
        fsm_ptr->has_play_end_reason = VIDEO_PLAY_END_NONE;
        if(fsm_ptr->is_to_exit)
        {
            //清除强制退出标志；
            fsm_ptr->is_to_exit = FALSE;
            SendStopIndToUI(fsm_ptr);       
            DestroyVideoPlayer((HVIDEOHANDLE)fsm_ptr);
        }
        break;

    case FSM_PLAY_REQ:
        InitFsmInfo(fsm_ptr,VIDEO_STATE_READY,fsm_ptr->vpCallBack);
        play_param_ptr = (VIDEO_PLAY_PARAM  *)msg_ptr->msg_body;
        result = UpdateFsmInfo(fsm_ptr,play_param_ptr);
        if(result)
        {
        
            //WMP_TRACE_LOW:"[Wre Player] FsmReady play_param_ptr->playType = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_4270_112_2_18_3_11_13_399,(uint8*)"d",play_param_ptr->playType);
            switch(play_param_ptr->playType) 
            {

            case VIDEO_PLAY_FROM_FILE: 
                //播放本地文件
                errorType = PlayLocalFile(fsm_ptr,TRUE);
                if(VIDEO_ERROR_NONE != errorType)
                {  
                    SendPlayCnfToUI(fsm_ptr,errorType); 
//                    DelaySwitchVideoPlayer((uint32)(fsm_ptr));
                    //NotifyPlayNextFile(fsm_ptr,VIDEO_PLAY_END_ERROR);                    
                }
                else
                {
                    //播放成功，进入playing状态
                    SendPlayCnfToUI(fsm_ptr,VIDEO_ERROR_NONE);
                    FsmStateTrans(fsm_ptr,VIDEO_STATE_PLAYING); 
                }
                break;
            
            case VIDEO_PLAY_FROM_MEM:
                //播放本地buf
                errorType = PlayLocalBuf(fsm_ptr,play_param_ptr);
                SendPlayCnfToUI(fsm_ptr,errorType);
                if(VIDEO_ERROR_NONE == errorType)
                {
                    //播放成功，进入playing状态
                    FsmStateTrans(fsm_ptr,VIDEO_STATE_PLAYING);  
                }    
                else
                {
                    result = FALSE;
                }                
                break;
#ifdef MMI_STREAMING_PLAYER_SUPPORT //20120612
            
            case VIDEO_PLAY_FROM_NET:  
				//此时pdp激活成功，进入init状态
				
				if(InitStreaming(fsm_ptr, play_param_ptr->uNetID))
				{					
					FsmStateTrans(fsm_ptr,VIDEO_STATE_INIT);            
				}
				/*
                //进入pdp activing状态
                FsmStateTrans(fsm_ptr,wmpStatePdpActiving);
                result = ActivePdp(fsm_ptr);  
                if (!result)
                {
                    SendPlayCnfToUI(fsm_ptr,VIDEO_ERROR_OTHERS);
                    FsmStateTrans(fsm_ptr,VIDEO_STATE_READY);                    
                }
				*/
                break;
  #endif          
            default:           
                result = FALSE;
                SendPlayCnfToUI(fsm_ptr,VIDEO_ERROR_OTHERS);
                break;
            }
        }
        
        if (!result)
        {            
            SendStopIndToUI(fsm_ptr);
        }
        result = TRUE; 
        break;
        
    case FSM_PLAY_NEXT_FILE:
		/*
        if (PNULL != msg_ptr->msg_body)
        {
            last_play_result = *(VIDEO_PLAY_END_RESULT *)msg_ptr->msg_body;
        }
        else
        {
            last_play_result = VIDEO_PLAY_END_ERROR;
        }
        if (!fsm_ptr->is_to_pause)
        {
            errorType = PlayNextFile(fsm_ptr, last_play_result); 
            if (VIDEO_ERROR_NONE != errorType)
            {
                SendStopIndToUI(fsm_ptr);
            } 
        }   
        else
        {
            fsm_ptr->is_to_pause = FALSE;
        }
		*/
        break;
        
    case FSM_RESUME_REQ:
        /*
		if (fsm_ptr->is_to_resume)
        {
            fsm_ptr->is_to_resume = FALSE;
            errorType = PlayNextFile(fsm_ptr, last_play_result); 
            if (VIDEO_ERROR_NONE != errorType)
            {
                SendStopIndToUI(fsm_ptr);
            } 
        }*/
        break;

    case FSM_RELEASE_REQ:
        //销毁状态机
        DestroyFSM((HVIDEOHANDLE)fsm_ptr);       
        break;
        
    case FSM_STOP_REQ:        
        FsmStateTrans(fsm_ptr,VIDEO_STATE_READY); 
        SendStopIndToUI(fsm_ptr);
        break;

    default:
        //WMP_TRACE_LOW:"[Wre Player] FsmReady ready, got wrong msg"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_4388_112_2_18_3_11_14_400,(uint8*)"");
        result = FALSE;
        break;
    }
    return result;
}
#ifdef MMI_STREAMING_PLAYER_SUPPORT //20120612

/*****************************************************************************/
//  Description : InitStreaming
//  Global resource dependence :  none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN InitStreaming(VIDEO_PLAYER *fsm_ptr,uint32 net_id)
{
    STRM_INIT_PARAM_T  strm_ini_para = {0};
    MMIFILE_HANDLE      sdp_file_handle = 0;
    uint32              readed = 0;   
    BOOLEAN             result = TRUE;
    VIDEO_SETTING       setting_info={0};  

    if (PNULL == fsm_ptr || !IsValidFsm(fsm_ptr))
    {
        //WMP_TRACE_LOW:"[Wre Player] InitStreaming param is error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_4409_112_2_18_3_11_14_401,(uint8*)"");
        return FALSE;
    }
    //获取网络配置信息
    strm_ini_para.app_thread_id = P_APP;
    setting_info = fsm_ptr->setting_info;
    
	setting_info.bAgent        = fsm_ptr->setting_info.bAgent;
	setting_info.uLowestPort   = fsm_ptr->setting_info.uLowestPort;
	setting_info.uHighestPort  = fsm_ptr->setting_info.uHighestPort;
	setting_info.uAgentIpLenth = fsm_ptr->setting_info.uAgentIpLenth;
	setting_info.uAgentPort    = fsm_ptr->setting_info.uAgentPort;
	SCI_MEMCPY(setting_info.szAgentIp,fsm_ptr->setting_info.szAgentIp,fsm_ptr->setting_info.uAgentIpLenth);


    if (!setting_info.bAgent)
    {
        strm_ini_para.proxy.host_ptr = PNULL;
        strm_ini_para.proxy.port = 0;
    }
    else
    {
        strm_ini_para.proxy.host_ptr = (char *)SCI_ALLOCA(MAX_IP_LENGTH+1);
        if (PNULL == strm_ini_para.proxy.host_ptr)
        {
            return FALSE;
        }
        SCI_MEMSET(strm_ini_para.proxy.host_ptr,0,(MAX_IP_LENGTH+1));
        SCI_MEMCPY(strm_ini_para.proxy.host_ptr,(char *)(setting_info.szAgentIp),setting_info.uAgentIpLenth);
        
        strm_ini_para.proxy.port = setting_info.uAgentPort;
    }
    
    strm_ini_para.proxy.username_ptr = PNULL;
    strm_ini_para.proxy.password_ptr = PNULL;
    strm_ini_para.auth_info.password_ptr = PNULL;
    strm_ini_para.auth_info.username_ptr = PNULL;
    strm_ini_para.url_ptr = (char *)(fsm_ptr->url_ptr);

    //读取sdp文按信息
    if (0 != fsm_ptr->uFilePathLen && PNULL != fsm_ptr->strFilePath)
    {
        sdp_file_handle = MMIAPIFMM_CreateFile(fsm_ptr->strFilePath,
            SFS_MODE_WRITE|SFS_MODE_OPEN_EXISTING, /*lint !e655*/
            NULL, 
            NULL);
        if (0 != sdp_file_handle)
        {
            strm_ini_para.sdp_len = MMIAPIFMM_GetFileSize(sdp_file_handle);
            strm_ini_para.sdp_buf_ptr = (uint8 *)SCI_ALLOCA(strm_ini_para.sdp_len +1);
            if (PNULL != strm_ini_para.sdp_buf_ptr)
            {
                SCI_MEMSET(strm_ini_para.sdp_buf_ptr,0,(strm_ini_para.sdp_len));
                if(SFS_ERROR_NONE != MMIAPIFMM_ReadFile(sdp_file_handle,strm_ini_para.sdp_buf_ptr,strm_ini_para.sdp_len,&readed,PNULL))
                {                    
                    SCI_FREE(strm_ini_para.sdp_buf_ptr);                    
                    result = FALSE;
                }              
            }
            else
            {                
                
                result = FALSE;
            }
            MMIAPIFMM_CloseFile(sdp_file_handle); 
            
        }        
    }
    else
    {
        strm_ini_para.sdp_len = 0;
        strm_ini_para.sdp_buf_ptr = PNULL;
    }

    if (result)
    {
        strm_ini_para.uaprof_ptr = PNULL;
        strm_ini_para.rtp_lowest_port = setting_info.uLowestPort;
        strm_ini_para.rtp_highest_port = setting_info.uHighestPort;       
    
        strm_ini_para.net_id = net_id; //VPlayer/1.0 (org=SS; model=13; terversion=11; VodSwVer=00; min=6191234567; lcd=0; cl=2; vo=1; HwClass=1; version=0x0200; visualpro=0x000003; audiopro=0x000001; MIDIType=0)";
        strm_ini_para.user_agent_ptr = "WrePlayer/1.0 (org=SS; model=13; terversion=11; VodSwVer=00; min=6191234567; lcd=0; cl=2; vo=1; HwClass=1; version=0x0200; visualpro=0x000003; audiopro=0x000001; MIDIType=0)";
            //"HSVPlayer/1.0 (org=SS; model=13; terversion=11; VodSwVer=00; min=6191234567; lcd=0; cl=2; vo=1; HwClass=1; version=0x0200; visualpro=0x000003; audiopro=0x000001; MIDIType=0)";
            //"SVPlayer/1.0 (org=SS; model=13; terversion=11; VodSwVer=00; min=6191234567; lcd=0; cl=2; vo=1; HwClass=1; version=0x0200; visualpro=0x000003; audiopro=0x000001; MIDIType=0)";
        //初始化streaming部分
        fsm_ptr->strm_handle = STRM_Init(&strm_ini_para);
    }
    
    if (PNULL != strm_ini_para.proxy.host_ptr)
    {
        SCI_FREE(strm_ini_para.proxy.host_ptr);
        strm_ini_para.proxy.host_ptr = PNULL;
    }
    if (PNULL != strm_ini_para.sdp_buf_ptr)
    {
        SCI_FREE(strm_ini_para.sdp_buf_ptr);
    }
    if (0 == fsm_ptr->strm_handle)
    {
       result = FALSE;
    }
    else
    {
       result = TRUE;
    }
    return result;
}
#endif
/*****************************************************************************/
//  Description : PDP激活中处理
//  Global resource dependence :  none
//  Author: 
//  Note: 
/*****************************************************************************/

LOCAL BOOLEAN FsmPdpActiving(VIDEO_PLAYER *fsm_ptr, VIDEO_FSM_MSG *msg_ptr)
{
    BOOLEAN             result       = TRUE;
/*
    
    //参数检查
    if(PNULL == fsm_ptr || !IsValidFsm(fsm_ptr) || PNULL == fsm_ptr->vpCallBack || PNULL == msg_ptr)
    {
        //WMP_TRACE_LOW:"[Wre Player] FsmPdpActiving fsm or msg error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_4528_112_2_18_3_11_14_402,(uint8*)"");
        return FALSE;
    }

    //WMP_TRACE_LOW:"[Wre Player] FsmPdpActiving msg_ptr->msg_id = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_4532_112_2_18_3_11_14_403,(uint8*)"d",msg_ptr->msg_id);
    switch(msg_ptr->msg_id)
    {
    case FSM_PDP_ACTIVE_CNF:
        pdp_msg_ptr = (MMIPDP_CNF_INFO_T *)msg_ptr->msg_body;
        //判断pdp激活的结果
        if (PNULL == pdp_msg_ptr || MMIPDP_RESULT_SUCC != pdp_msg_ptr->result)
        {
            //WMP_TRACE_LOW:"[Wre Player] FsmPdpActiving pdp active error"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_4540_112_2_18_3_11_14_404,(uint8*)"");
            //通知ui激活失败，停止状态机
            //SendFatalErrorIndTOUI(fsm_ptr,VIDEO_ERROR_PDP_ACTIVE_ERROR);
            StopVideo((HVIDEOHANDLE)fsm_ptr);            
        }
        else if (InitStreaming(fsm_ptr, (uint32)(pdp_msg_ptr->nsapi)))
        {
            //pdp激活成功，进入init状态
            FsmStateTrans(fsm_ptr,VIDEO_STATE_INIT);            
        }
        else
        {
            //通知ui激活失败，停止状态机     
            SendFatalErrorIndTOUI(fsm_ptr,VIDEO_ERROR_OTHERS);
            StopVideo((HVIDEOHANDLE)fsm_ptr);           
        }
        break;
        
    case FSM_PDP_DEACTIVE_IND:
        //通知ui激活失败，停止状态机
        //SendFatalErrorIndTOUI(fsm_ptr,VIDEO_ERROR_PDP_ACTIVE_ERROR);
        StopVideo((HVIDEOHANDLE)fsm_ptr);        
        break;
        
    case FSM_STOP_REQ:                       
        HandleStopReq(fsm_ptr,FALSE);           
        FsmStateTrans(fsm_ptr,VIDEO_STATE_READY);   
        SendStopIndToUI(fsm_ptr);
        break;
        
    default:
        result = FALSE;
        break;
    }
*/
    return result;    
}


/*****************************************************************************/
//  Description : 已初始化状态
//  Global resource dependence :  none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN FsmInit_PauseEx(VIDEO_PLAYER *fsm_ptr, VIDEO_FSM_MSG *msg_ptr)
{ 
#ifdef MMI_STREAMING_PLAYER_SUPPORT //20120612
    BOOLEAN                     result              = TRUE;    
  //  GUI_RECT_T                  lcd_rect = {0};
    BOOLEAN is_need_fill = TRUE;
    LCD_VIDEO_DISPLAY_PARAM  display_param = {0};

    
    
    //参数检查
    if(PNULL == fsm_ptr || !IsValidFsm(fsm_ptr) || PNULL == fsm_ptr->vpCallBack || PNULL == msg_ptr)
    {
        //WMP_TRACE_LOW:"[Wre Player] FsmInit param is error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_4595_112_2_18_3_11_14_405,(uint8*)"");
        return FALSE;
    }

    //WMP_TRACE_LOW:"[Wre Player] FsmInit msg_ptr->msg_id = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_4599_112_2_18_3_11_14_406,(uint8*)"d",msg_ptr->msg_id);
    switch(msg_ptr->msg_id)
    {
    case FSM_RESUME_REQ:    
        if (fsm_ptr->is_to_resume)
        {     
            fsm_ptr->is_to_resume = FALSE;
            
            if (VIDEO_SUPPORTED_NEITHER == fsm_ptr->support_type
            || VIDEO_SUPPORTED_AUDIO_ONLY == fsm_ptr->support_type)
            {
                 is_need_fill = FALSE;
            }

			SCI_MEMCPY(&(display_param),&(fsm_ptr->LcdDisplayParam),sizeof(LCD_VIDEO_DISPLAY_PARAM)); 

/*
			display_param.audioSupport = fsm_ptr->LcdDisplayParam.audioSupport;
			display_param.videoSupport = fsm_ptr->LcdDisplayParam.videoSupport;
			display_param.RotateAngle  = fsm_ptr->LcdDisplayParam.RotateAngle;
			display_param.LcdId        = fsm_ptr->LcdDisplayParam.LcdId;
			display_param.LcdBlkId     = fsm_ptr->LcdDisplayParam.LcdBlkId;
			display_param.b_support_seek = fsm_ptr->LcdDisplayParam.b_support_seek;
			display_param.disp_mode     = fsm_ptr->LcdDisplayParam.dispMode;

			display_param.disp_rect.dx =  fsm_ptr->LcdDisplayParam.dispRect.dx;
			display_param.disp_rect.dy =  fsm_ptr->LcdDisplayParam.dispRect.dy;
			display_param.disp_rect.x  =  fsm_ptr->LcdDisplayParam.dispRect.x;
			display_param.disp_rect.y  =  fsm_ptr->LcdDisplayParam.dispRect.y;

			display_param.target_rect.dx =  fsm_ptr->LcdDisplayParam.targetRect.dx;
			display_param.target_rect.dy =  fsm_ptr->LcdDisplayParam.targetRect.dy;
			display_param.target_rect.x  =  fsm_ptr->LcdDisplayParam.targetRect.x;
			display_param.target_rect.y  =  fsm_ptr->LcdDisplayParam.targetRect.y;
*/

            TranslateDisplayParamLogicToPhysical(&display_param, is_need_fill);

            MMIAPISET_InitAudioDevice(fsm_ptr->vol);     
            SwitchPlayDevice((HVIDEOHANDLE)fsm_ptr,fsm_ptr->bSupportA2dp,fsm_ptr->is_bt_support);
            if(DPLAYER_SUCCESS != DPLAYER_SetDisplayParam(fsm_ptr->dplayer_handle,&display_param) 
                ||DPLAYER_SUCCESS != DPLAYER_Play(fsm_ptr->dplayer_handle) 
                || !STRM_Play(fsm_ptr->strm_handle,fsm_ptr->dplayer_handle))
            {               

                //WMP_TRACE_LOW:"[Wre Player] PlayFromNet play error"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_4643_112_2_18_3_11_14_407,(uint8*)"");
                SendPlayCnfToUI(fsm_ptr,VIDEO_ERROR_OTHERS);
                StopVideo((HVIDEOHANDLE)fsm_ptr);
            }
            else
            {
         //       SwitchPlayDevice((HVIDEOHANDLE)fsm_ptr,fsm_ptr->bSupportA2dp,fsm_ptr->is_bt_support);
                FsmStateTrans(fsm_ptr,VIDEO_STATE_PLAYPENDING);
            }
        }
         break;
        
    case FSM_PDP_DEACTIVE_IND:
    case FSM_STOP_REQ: 
        if(FSM_PDP_DEACTIVE_IND == msg_ptr->msg_id)
        {
            //SendFatalErrorIndTOUI(fsm_ptr,VIDEO_ERROR_PDP_DEACTIVE);
        }        
        DPLAYER_Close(fsm_ptr->dplayer_handle);
        fsm_ptr->dplayer_handle = 0;    
        DPLAYER_Release();
        HandleStopReq(fsm_ptr,FALSE);
        FsmStateTrans(fsm_ptr,VIDEO_STATE_CLOSING);        
        break;     
        
    default:
        result = FALSE;
        break;
    }

    return result;
#else 
	return FALSE;
#endif
}


/*****************************************************************************/
//  Description : 已初始化状态
//  Global resource dependence :  none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN FsmInit(VIDEO_PLAYER *fsm_ptr, VIDEO_FSM_MSG *msg_ptr)
{ 
#ifdef MMI_STREAMING_PLAYER_SUPPORT //20120612
    BOOLEAN                     result              = TRUE;    
    STRM_RESULT_E               str_ini_result      = STRM_RESULT_SUCCESS;
    VIDEO_ERROR_TYPE                  errorType          = 0;
    
    //参数检查
    if(PNULL == fsm_ptr || !IsValidFsm(fsm_ptr) || PNULL == fsm_ptr->vpCallBack || PNULL == msg_ptr)
    {
        //WMP_TRACE_LOW:"[Wre Player] FsmInit param is error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_4690_112_2_18_3_11_14_408,(uint8*)"");
        return FALSE;
    }

    //WMP_TRACE_LOW:"[Wre Player] FsmInit msg_ptr->msg_id = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_4694_112_2_18_3_11_14_409,(uint8*)"d",msg_ptr->msg_id);
    switch(msg_ptr->msg_id)
    {
    case FSM_STRM_INIT_CNF:       
         str_ini_result = HandleInitCnf(fsm_ptr,(STRM_INIT_CNF_T*)msg_ptr->msg_body, &errorType);
         //WMP_TRACE_LOW:"[Wre Player] FsmInit str_ini_result = %d, is_success_play = %d"
         SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_4699_112_2_18_3_11_14_410,(uint8*)"dd",str_ini_result,errorType);
         switch(str_ini_result) 
         {
         case STRM_RESULT_SUCCESS: 
            if (VIDEO_ERROR_NONE == errorType)
            {
                if (fsm_ptr->is_to_pause)
                {
                    fsm_ptr->is_to_pause = FALSE;
                    FsmStateTrans(fsm_ptr,VIDEO_STATE_INITPAUSEDEX);
                }
                else
                {
                    FsmStateTrans(fsm_ptr,VIDEO_STATE_PLAYPENDING);
                }
                
            }
            else
            {
                result = FALSE;                
            }
            
            break;
            
        case STRM_RESULT_UNSUPPORTED_MEDIA_TYPE:
        case STRM_RESULT_UNSUPPORTED:
            result = FALSE;
            errorType = VIDEO_ERROR_NOT_SUPPORT;
            break;
            
        case STRM_RESULT_INTERNAL_SERVER_ERROR:
            result = FALSE;
            errorType = VIDEO_ERROR_INTERNAL_SERVER_ERROR;
            break;
            
        case STRM_RESULT_BAD_REQUEST: 
            result = FALSE;
            errorType = VIDEO_ERROR_BAD_REQUEST;
            break;
        case STRM_RESULT_UNAUTHORIZED:
            result = FALSE;
            errorType = VIDEO_ERROR_UNAUTHORIZED;
            break;
            
        case STRM_RESULT_NOT_FOUND:
            result = FALSE;
            errorType = VIDEO_ERROR_NOT_FOUND;
            break; 

        case STRM_RESULT_UNKNOWN:
            result = FALSE;
            errorType = VIDEO_ERROR_CONNECT_TIMEOUT;
            break;

         default:
             result = FALSE;
             errorType = VIDEO_ERROR_STRM_INIT_ERROR;            
         }   

         if (!result)
         {
             SendPlayCnfToUI(fsm_ptr,errorType);
             FsmStateTrans(fsm_ptr,VIDEO_STATE_READY);             
             StopVideo((HVIDEOHANDLE)fsm_ptr);
             result = TRUE;
         }
        break;
        
    case FSM_PDP_DEACTIVE_IND:
    case FSM_STOP_REQ: 
        if(FSM_PDP_DEACTIVE_IND == msg_ptr->msg_id)
        {
            //SendFatalErrorIndTOUI(fsm_ptr,VIDEO_ERROR_PDP_DEACTIVE);
        }        
        
        HandleStopReq(fsm_ptr,FALSE);
        FsmStateTrans(fsm_ptr,VIDEO_STATE_CLOSING);        
        break;
        
    case FSM_AUTHIND_REQ:        
        //转入VIDEO_STATE_INITAUTH状态，等待ui的鉴权信息
        FsmStateTrans(fsm_ptr,VIDEO_STATE_INITAUTH);
        //发送消息给ui，请求输入鉴权信息
        SendAUTHIndToUI(fsm_ptr);
        break;
        
    default:
        result = FALSE;
        break;
    }

    return result;
#else
	return FALSE;
#endif
}

/*****************************************************************************/
//  Description : 认证中
//  Global resource dependence :  none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN FsmInitAuth(VIDEO_PLAYER *fsm_ptr, VIDEO_FSM_MSG *msg_ptr)
{
    
#ifdef MMI_STREAMING_PLAYER_SUPPORT //20120612
    VIDEO_AUTH_INFO          *auth_info_ptr      = PNULL;
    STRM_AUTH_INFO_T        strm_auth_info      = {0};
    BOOLEAN                 result              = TRUE;

    //参数检查
    if(PNULL == fsm_ptr || !IsValidFsm(fsm_ptr) || PNULL == fsm_ptr->vpCallBack || PNULL == msg_ptr)
    {
        //WMP_TRACE_LOW:"[Wre Player] FsmInitAuth param is error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_4807_112_2_18_3_11_14_411,(uint8*)"");
        return FALSE;
    }

    //WMP_TRACE_LOW:"[Wre Player] FsmInitAuth msg_ptr->msg_id = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_4811_112_2_18_3_11_14_412,(uint8*)"d",msg_ptr->msg_id);
    switch(msg_ptr->msg_id)
    {
    case FSM_AUTH_RSP:
        //获取鉴权信息
        auth_info_ptr = (VIDEO_AUTH_INFO *)msg_ptr->msg_body;
        if (PNULL == auth_info_ptr)
        {
            result = FALSE;
        }
        else
        {
            strm_auth_info.password_ptr = auth_info_ptr->strPassword;
            strm_auth_info.username_ptr = auth_info_ptr->strUserName;
            //将鉴权信息传给streaming处理
            result = STRM_DescribeAuthRsp(fsm_ptr->strm_handle,&strm_auth_info);
        }
        if (result)
        {
            FsmStateTrans(fsm_ptr,VIDEO_STATE_INIT);
        }
        else
        {
            SendPlayCnfToUI(fsm_ptr,VIDEO_ERROR_UNAUTHORIZED);
            StopVideo((HVIDEOHANDLE)fsm_ptr);
        }
        result = TRUE;
        break;
        
    case FSM_PDP_DEACTIVE_IND:
    case FSM_STOP_REQ:
        if(FSM_PDP_DEACTIVE_IND == msg_ptr->msg_id)
        {
//            SendFatalErrorIndTOUI(fsm_ptr,VIDEO_ERROR_PDP_DEACTIVE);
        }       
        
        HandleStopReq(fsm_ptr,FALSE);    
        FsmStateTrans(fsm_ptr,VIDEO_STATE_CLOSING);
        break;
        
    default:
        result = FALSE;
        break;
    }
    return result;
#else
	return FALSE;
#endif
}

/*****************************************************************************/
//  Description : 播放准备中
//  Global resource dependence :  none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN FsmPlayPeding(VIDEO_PLAYER *fsm_ptr, VIDEO_FSM_MSG *msg_ptr)
{
#ifdef MMI_STREAMING_PLAYER_SUPPORT //20120612

    STRM_RESULT_E       strm_result     = 0;
    BOOLEAN             result          = TRUE;
    VIDEO_ERROR_TYPE          errorType = 0;
    BOOLEAN             is_need_fill = TRUE;
    LCD_VIDEO_DISPLAY_PARAM display_param = {0};
    
    //参数检查
    if(PNULL == fsm_ptr || !IsValidFsm(fsm_ptr) || PNULL == fsm_ptr->vpCallBack || PNULL == msg_ptr)
    {
        //WMP_TRACE_LOW:"[Wre Player] FsmPlayPeding param is error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_4873_112_2_18_3_11_15_413,(uint8*)"");
        return FALSE;
    }

    //WMP_TRACE_LOW:"[Wre Player] FsmPlayPeding msg_ptr->msg_id = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_4877_112_2_18_3_11_15_414,(uint8*)"d",msg_ptr->msg_id);
    switch(msg_ptr->msg_id)
    {
    case FSM_STRM_PLAY_CNF:
        strm_result = *(STRM_RESULT_E *)msg_ptr->msg_body;
        //WMP_TRACE_LOW:"[Wre Player] FsmPlayPeding strm_result = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_4882_112_2_18_3_11_15_415,(uint8*)"d",strm_result);
        switch(strm_result) 
        {
        case STRM_RESULT_SUCCESS:
            errorType = VIDEO_ERROR_NONE;   
            break;
            
        case STRM_RESULT_UNSUPPORTED_MEDIA_TYPE:
        case STRM_RESULT_UNSUPPORTED:
            errorType = VIDEO_ERROR_NOT_SUPPORT;
            break;
            
        case STRM_RESULT_INTERNAL_SERVER_ERROR:
            errorType = VIDEO_ERROR_INTERNAL_SERVER_ERROR;
            break;
            
        case STRM_RESULT_BAD_REQUEST: 
            errorType = VIDEO_ERROR_BAD_REQUEST;
            break;
        case STRM_RESULT_UNAUTHORIZED:
            errorType = VIDEO_ERROR_UNAUTHORIZED;
            break;
            
        case STRM_RESULT_NOT_FOUND:
            errorType = VIDEO_ERROR_NOT_FOUND;
            break;            
        default:
            errorType = VIDEO_ERROR_OTHERS;
            break;
        }

        
        if (STRM_RESULT_SUCCESS == strm_result)
        {
            if (VIDEO_SUPPORTED_NEITHER == fsm_ptr->support_type
            || VIDEO_SUPPORTED_AUDIO_ONLY == fsm_ptr->support_type
            || fsm_ptr->is_to_pause
            || fsm_ptr->is_to_exit)
            {
                 is_need_fill = FALSE;
            }        
            
            //以下内容主要用于给播放区域刷新color key,将默认图标刷去，不可以去除
            SCI_MEMCPY(&display_param,&(fsm_ptr->LcdDisplayParam),sizeof(LCD_VIDEO_DISPLAY_PARAM));
            TranslateDisplayParamLogicToPhysical(&display_param, is_need_fill);
            SendPlayCnfToUI(fsm_ptr, errorType);
            FsmStateTrans(fsm_ptr,VIDEO_STATE_PLAYING);
            
        }
        else
        {            
            SendPlayCnfToUI(fsm_ptr, errorType);
            StopVideo((HVIDEOHANDLE)fsm_ptr);
        }        
        break;
       
    case FSM_PAUSE_REQ:
        if (fsm_ptr->is_to_pause)  //此时需要先停止底层播放，防止占用外面空间
        {
            //DPLAYER_PauseEx(fsm_ptr->dplayer_handle);//del by leichi
             MMISRVAUD_Pause(fsm_ptr->srv_handle);//added by leichi 

            SwitchPlayDevice((HVIDEOHANDLE)fsm_ptr,fsm_ptr->bSupportA2dp,fsm_ptr->is_bt_support);
        }
        result = FALSE;
        break;

    case FSM_STRM_SERVER_CLOSE_IND:    
    case FSM_PDP_DEACTIVE_IND:
    case FSM_STOP_REQ:   
        if(FSM_PDP_DEACTIVE_IND == msg_ptr->msg_id)
        {
            //SendFatalErrorIndTOUI(fsm_ptr,VIDEO_ERROR_PDP_DEACTIVE);
        }
        else if(FSM_STRM_SERVER_CLOSE_IND == msg_ptr->msg_id)
        {
            SendFatalErrorIndTOUI(fsm_ptr,VIDEO_ERROR_SERVER_CLOSE);            
        }
        
        HandleStopReq(fsm_ptr,TRUE);    
        FsmStateTrans(fsm_ptr,VIDEO_STATE_STOPING);
        break;    
    

    case FSM_STRM_DATA_TIMEOUT_IND:
        //给ui发送数据超时的消息
        SendFatalErrorIndTOUI(fsm_ptr,VIDEO_ERROR_DATA_TIMEOUT);
        //停止当前播放
        StopVideo((HVIDEOHANDLE)fsm_ptr);       
        break;
        
    case FSM_AUTHIND_REQ:        
        //转入VIDEO_STATE_PLAYAUTH状态，等待ui的鉴权信息
        FsmStateTrans(fsm_ptr,VIDEO_STATE_PLAYAUTH);
        //发送消息给ui，请求输入鉴权信息
        SendAUTHIndToUI(fsm_ptr);        
        break;

    default:
        result = FALSE;
        break;
    }
    return result;
#else
	return FALSE;
#endif
}

/*****************************************************************************/
//  Description : 认证中
//  Global resource dependence :  none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN FsmPlayAuth(VIDEO_PLAYER *fsm_ptr, VIDEO_FSM_MSG *msg_ptr)
{
#ifdef MMI_STREAMING_PLAYER_SUPPORT //20120612
    VIDEO_AUTH_INFO          *auth_info_ptr  = PNULL;
    STRM_AUTH_INFO_T        auth_info       = {0};
    BOOLEAN                 result          = TRUE;

    
    //参数检查
    if(PNULL == fsm_ptr || !IsValidFsm(fsm_ptr) || PNULL == fsm_ptr->vpCallBack || PNULL == msg_ptr)
    {
        //WMP_TRACE_LOW:"[Wre Player] FsmPlayAuth param is error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_4999_112_2_18_3_11_15_416,(uint8*)"");
        return FALSE;
    }

    
    //WMP_TRACE_LOW:"[Wre Player] FsmPlayAuth msg_ptr->msg_id = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_5004_112_2_18_3_11_15_417,(uint8*)"d",msg_ptr->msg_id);
    switch(msg_ptr->msg_id)
    {
    case FSM_AUTH_RSP:
        //获取鉴权信息
        auth_info_ptr = (VIDEO_AUTH_INFO *)msg_ptr->msg_body;
        if (PNULL == auth_info_ptr)
        {
            result = FALSE;
        }
        else
        {
            auth_info.password_ptr = auth_info_ptr->strPassword;
            auth_info.username_ptr = auth_info_ptr->strUserName;
            //WMP_TRACE_LOW:"[Wre Player] FsmPlayAuth auth_info.password_ptr= %x, auth_info.username_ptr = %x"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_5020_112_2_18_3_11_15_418,(uint8*)"dd",auth_info.password_ptr,auth_info.username_ptr);
            //将鉴权信息传给streaming处理
            result = STRM_SetupAuthRsp(fsm_ptr->strm_handle,&auth_info);
        }
        if (result)
        {
            FsmStateTrans(fsm_ptr,VIDEO_STATE_PLAYPENDING);
        }
        else
        {
            SendPlayCnfToUI(fsm_ptr,VIDEO_ERROR_UNAUTHORIZED);
            StopVideo((HVIDEOHANDLE)fsm_ptr);
            result = TRUE;
        }    
        break;
        
    case FSM_STRM_SERVER_CLOSE_IND:
    case FSM_PDP_DEACTIVE_IND:
    case FSM_STOP_REQ:
        if(FSM_PDP_DEACTIVE_IND == msg_ptr->msg_id)
        {
            //SendFatalErrorIndTOUI(fsm_ptr,VIDEO_ERROR_PDP_DEACTIVE);
        }
        else if(FSM_STRM_SERVER_CLOSE_IND == msg_ptr->msg_id)
        {
            SendFatalErrorIndTOUI(fsm_ptr,VIDEO_ERROR_SERVER_CLOSE);            
        }
       
        HandleStopReq(fsm_ptr,TRUE);
        FsmStateTrans(fsm_ptr,VIDEO_STATE_STOPING);
        break;
        
    default:
        result = FALSE;
        break;
    }
    return result;
#else
	return FALSE;
#endif
}

/*****************************************************************************/
//  Description : 播放中
//  Global resource dependence :  none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN FsmPlaying(VIDEO_PLAYER *fsm_ptr, VIDEO_FSM_MSG *msg_ptr)
{    
    BOOLEAN             result      =   TRUE; 
    VIDEO_PLAY_END_RESULT               i_type = VIDEO_PLAY_END_SUCCESS;
   

    //参数检查
    if(PNULL == fsm_ptr || !IsValidFsm(fsm_ptr) || PNULL == fsm_ptr->vpCallBack || PNULL == msg_ptr)
    {
        //WMP_TRACE_LOW:"[Wre Player] FsmPlaying param is error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_5072_112_2_18_3_11_15_419,(uint8*)"");
        return FALSE;
    }
    
    
    switch(msg_ptr->msg_id)
    {
    case FSM_ENTRY_REQ:
      //  CallbackFrameEndNotify(1);
        //WMP_TRACE_LOW:"[Wre Player] FsmPlaying FSM_ENTRY_REQ fsm_ptr->is_to_pause %x,fsm_ptr->has_play_end_reason = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_5081_112_2_18_3_11_15_420,(uint8*)"dd",fsm_ptr->is_to_pause,fsm_ptr->has_play_end_reason);
        if(fsm_ptr->is_to_pause)
        {
            fsm_ptr->is_to_pause = FALSE;             
            if(HandlePauseReqEx(TRUE, fsm_ptr))
            {
                if (VIDEO_PLAY_FROM_NET == fsm_ptr->playType)
                {
                    FsmStateTrans(fsm_ptr,VIDEO_STATE_PAUSING);
                }
                else
                {
                    FsmStateTrans(fsm_ptr,VIDEO_STATE_PAUSED);                    
                }
            }
        }
        else if (VIDEO_PLAY_END_NONE != fsm_ptr->has_play_end_reason)
        { 
            FsmDispatch((HVIDEOHANDLE)fsm_ptr,FSM_CALLBACK_END_IND,&(fsm_ptr->has_play_end_reason),sizeof(VIDEO_PLAY_END_RESULT));
            
        }
        break;    
#ifdef MMI_STREAMING_PLAYER_SUPPORT //20120612

    case FSM_STRM_BUFFERING_IND:
        //WMP_TRACE_LOW:"[Wre Player] FsmPlaying WMP_STRM_BUFFERING_IND"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_5105_112_2_18_3_11_15_421,(uint8*)"");
        SendBufferingIndToUI(fsm_ptr);   
        FsmStateTrans(fsm_ptr,VIDEO_STATE_BUFFERING);             
        break;
#endif
    case FSM_CALLBACK_END_IND:
        //WMP_TRACE_LOW:"[Wre Player] FsmPlaying FSM_CALLBACK_END_IND"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_5111_112_2_18_3_11_15_422,(uint8*)"");
        fsm_ptr->has_play_end_reason = VIDEO_PLAY_END_NONE;

        if (PNULL != msg_ptr->msg_body)
        {
            i_type = *(VIDEO_PLAY_END_RESULT *)msg_ptr->msg_body;
        }
        if (VIDEO_PLAY_END_STOPPED_BY_USER == i_type)
        {
            break;
        }
        
        //给ui发送VIDEO_NOTIFY_PLAY_END消息   
       // DPLAYER_UpdateCurrentFrame(DPLAYER_NORMAL_HANDLE);
             

        //WMP_TRACE_LOW:"[Wre Player] FsmPlaying fsm_ptr->playType = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_5127_112_2_18_3_11_15_423,(uint8*)"d",fsm_ptr->playType);
        if (VIDEO_PLAY_FROM_MEM == fsm_ptr->playType)
        {
            //关闭dplayer,进入ready状态
            DPLAYER_Close(fsm_ptr->dplayer_handle);
            fsm_ptr->dplayer_handle = 0;
            DPLAYER_Release();
            SendPlayEndToUI(fsm_ptr,i_type);   
            FsmStateTrans(fsm_ptr,VIDEO_STATE_READY);
            //给ui发送VIDEO_NOTIFY_STOP消息        
            SendStopIndToUI(fsm_ptr);
        }
	#ifdef MMI_STREAMING_PLAYER_SUPPORT	
        else if(VIDEO_PLAY_FROM_NET == fsm_ptr->playType)
        {
            //停止当前播放
            DPLAYER_Close(fsm_ptr->dplayer_handle);
            fsm_ptr->dplayer_handle = 0;
            DPLAYER_Release();
            SendPlayEndToUI(fsm_ptr,i_type);   
            STRM_Stop(fsm_ptr->strm_handle);   
            FsmStateTrans(fsm_ptr,VIDEO_STATE_STOPING);
            
        }
	#endif
        else if (VIDEO_PLAY_FROM_FILE == fsm_ptr->playType)
        {
            DPLAYER_Close(fsm_ptr->dplayer_handle);
            fsm_ptr->dplayer_handle = 0;
            DPLAYER_Release();
            SendPlayEndToUI(fsm_ptr,i_type);   
			FsmStateTrans(fsm_ptr,VIDEO_STATE_READY);
			//给ui发送VIDEO_NOTIFY_STOP消息        
			SendStopIndToUI(fsm_ptr);

/*            if ( (VIDEO_PLAY_END_SUCCESS != i_type) )//|| (VIDEO_ERROR_NONE != PlayLocalFile(fsm_ptr,FALSE)) )
            {
            }
            else
            {
                SendPlayCnfToUI(fsm_ptr,VIDEO_ERROR_NONE);
            }
  */    }
        else 
        {
            DPLAYER_Close(fsm_ptr->dplayer_handle);
            fsm_ptr->dplayer_handle = 0;
            DPLAYER_Release();
            SendPlayEndToUI(fsm_ptr,i_type);                        
        }        
        break;
        
    case FSM_FRAME_END_IND:       
        HandleUpdateFrameReq(fsm_ptr);        
        break; 
        
    case FSM_PAUSE_REQ:
        //WMP_TRACE_LOW:"[Wre Player] FsmPlaying FSM_PAUSE_REQ, fsm_ptr->is_not_notify_ui = %d, fsm_ptr->is_to_pause = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_5181_112_2_18_3_11_15_424,(uint8*)"dd",fsm_ptr->is_not_notify_ui,fsm_ptr->is_to_pause);
        if (fsm_ptr->is_to_pause)
        {
            fsm_ptr->is_to_pause = FALSE;
            if(HandlePauseReqEx(TRUE, fsm_ptr))
            {
            
#ifdef MMI_STREAMING_PLAYER_SUPPORT //20120612
                if (VIDEO_PLAY_FROM_NET == fsm_ptr->playType)
                {
                    fsm_ptr->is_not_notify_ui = TRUE;
                    FsmStateTrans(fsm_ptr,VIDEO_STATE_PAUSING);
                }
                else
#endif
                {
                    FsmStateTrans(fsm_ptr,VIDEO_STATE_PAUSED);                    
                }
            }
        }
        else if(result = HandlePauseReq(fsm_ptr))  /*lint !e720*/
        {
            fsm_ptr->is_not_notify_ui = FALSE;
			
#ifdef MMI_STREAMING_PLAYER_SUPPORT //20120612
            if (VIDEO_PLAY_FROM_NET == fsm_ptr->playType)
            {
                FsmStateTrans(fsm_ptr,VIDEO_STATE_PAUSING);
            }
            else
#endif
            {
                SendPauseCnfToUI(fsm_ptr,VIDEO_ERROR_NONE);
                FsmStateTrans(fsm_ptr,VIDEO_STATE_PAUSED);                
            }
        }
        break;
        
    case FSM_SEEK_REQ:
        //WMP_TRACE_LOW:"[Wre Player] FsmPlaying FSM_SEEK_REQ"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_5214_112_2_18_3_11_15_425,(uint8*)"");
        if(HandleSeekReq(fsm_ptr, (VIDEO_SEEK_PARAM*)(msg_ptr->msg_body)))
        {
            if (VIDEO_PLAY_FROM_NET != fsm_ptr->playType)
            {
                SendSeekCnfToUI(fsm_ptr,VIDEO_ERROR_NONE); 
                FsmStateTrans(fsm_ptr,VIDEO_STATE_PLAYING);
                               
            }
#ifdef MMI_STREAMING_PLAYER_SUPPORT //20120612
            else
            {
                //通知streaming seek,进入seeking状态                   
                FsmStateTrans(fsm_ptr,VIDEO_STATE_SEEKING);               
            }
#endif
        }
        break;
        
    case FSM_STRM_SERVER_CLOSE_IND:
    case FSM_PDP_DEACTIVE_IND:
    case FSM_STRM_GOODBYE_IND:
    case FSM_STOP_REQ:
        fsm_ptr->has_play_end_reason = VIDEO_PLAY_END_NONE;
        if(FSM_STRM_GOODBYE_IND ==  msg_ptr->msg_id)
        {        
            SendPlayEndToUI(fsm_ptr,VIDEO_PLAY_END_SUCCESS);
        }
        else if(FSM_PDP_DEACTIVE_IND == msg_ptr->msg_id)
        {
            //SendFatalErrorIndTOUI(fsm_ptr,VIDEO_ERROR_PDP_DEACTIVE);
        }
        else if(FSM_STRM_SERVER_CLOSE_IND == msg_ptr->msg_id)
        {
            SendFatalErrorIndTOUI(fsm_ptr,VIDEO_ERROR_SERVER_CLOSE);            
        }
        
        //WMP_TRACE_LOW:"[Wre Player] FsmPlaying FSM_STOP_REQ"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_5250_112_2_18_3_11_15_426,(uint8*)"");
        HandleStopReq(fsm_ptr,TRUE);
	
#ifdef MMI_STREAMING_PLAYER_SUPPORT //20120612
        if (VIDEO_PLAY_FROM_NET == fsm_ptr->playType)
        {
            FsmStateTrans(fsm_ptr,VIDEO_STATE_STOPING);
        }
        else
#endif
        {
            FsmStateTrans(fsm_ptr,VIDEO_STATE_READY);
            SendStopIndToUI(fsm_ptr);
        }
        break;
#ifdef MMI_STREAMING_PLAYER_SUPPORT //20120612

   case FSM_STRM_DATA_TIMEOUT_IND:
       //WMP_TRACE_LOW:"[Wre Player] FsmPlaying FSM_STRM_DATA_TIMEOUT_IND"
       SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_5264_112_2_18_3_11_15_427,(uint8*)"");
        //给ui发送数据超时的消息
        SendFatalErrorIndTOUI(fsm_ptr,VIDEO_ERROR_DATA_TIMEOUT);
        //停止当前播放
        StopVideo((HVIDEOHANDLE)fsm_ptr);       
        break;
#endif
    case FSM_RESUME_REQ:
        //WMP_TRACE_LOW:"[Wre Player] FsmPlaying FSM_RESUME_REQ"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_5272_112_2_18_3_11_15_428,(uint8*)"");
        SendPlayCnfToUI(fsm_ptr,VIDEO_ERROR_NONE);
        break;

    case FSM_START_SET_RATE:
        if(PNULL != msg_ptr->msg_body)
        {
            if (*(BOOLEAN *) msg_ptr->msg_body)
            {
                DPLAYER_SetPlayRate(fsm_ptr->dplayer_handle,DPLAYER_PLAY_RATE_BACKWARD_X4);
            }
            else
            {
                DPLAYER_SetPlayRate(fsm_ptr->dplayer_handle,DPLAYER_PLAY_RATE_X4);
            }
        }        
        break;

    case FSM_STOP_SET_RATE:
        DPLAYER_SetPlayRate(fsm_ptr->dplayer_handle,DPLAYER_PLAY_RATE_NORMAL);
        break;

    default:
        result = FALSE;
        break;
    }
    return result;
}

/*****************************************************************************/
//  Description : 缓冲中
//  Global resource dependence :  none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN FsmBuffering(VIDEO_PLAYER *fsm_ptr, VIDEO_FSM_MSG *msg_ptr)
{
#ifdef MMI_STREAMING_PLAYER_SUPPORT //20120612

    BOOLEAN              result     = TRUE;
    

    //参数检查
    if(PNULL == fsm_ptr || !IsValidFsm(fsm_ptr) || PNULL == fsm_ptr->vpCallBack || PNULL == msg_ptr)
    {
        //WMP_TRACE_LOW:"[Wre Player] FsmBuffering param is error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_5313_112_2_18_3_11_15_429,(uint8*)"");
        return FALSE;
    }

    
    //WMP_TRACE_LOW:"[Wre Player] FsmBuffering msg_ptr->msg_id = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_5318_112_2_18_3_11_15_430,(uint8*)"d",msg_ptr->msg_id);
    switch(msg_ptr->msg_id)
    {
    case FSM_ENTRY_REQ:
        if(fsm_ptr->is_to_pause)
        {
            fsm_ptr->is_to_pause = FALSE;
            if(HandlePauseReqEx(TRUE, fsm_ptr))
            {
                FsmStateTrans(fsm_ptr,VIDEO_STATE_PAUSING);
            }           
            
        }
        else if (VIDEO_PLAY_END_NONE != fsm_ptr->has_play_end_reason)
        {
            fsm_ptr->has_play_end_reason = VIDEO_PLAY_END_NONE;
            SendPlayEndToUI(fsm_ptr,VIDEO_PLAY_END_SUCCESS);
            HandleStopReq(fsm_ptr,TRUE);
            FsmStateTrans(fsm_ptr,VIDEO_STATE_STOPING);
        }
        break;

    case FSM_STRM_BUFFER_END_IND:
        //进入playing状态
        SendBufferEndIndToUI(fsm_ptr); 
        FsmStateTrans(fsm_ptr,VIDEO_STATE_PLAYING);             
        break;
        
    case FSM_BUFFER_PERCENT:
        SendBufferPercentToUI(fsm_ptr);      
        break;
        
    case FSM_STRM_DATA_TIMEOUT_IND:
        //给ui发送数据超时的消息
        SendFatalErrorIndTOUI(fsm_ptr,VIDEO_ERROR_DATA_TIMEOUT);
        //停止当前播放
        StopVideo((HVIDEOHANDLE)fsm_ptr);       
        break;
        
    case FSM_STRM_SERVER_CLOSE_IND:
    case FSM_PDP_DEACTIVE_IND:
    case FSM_STRM_GOODBYE_IND:
    case FSM_STOP_REQ:
        fsm_ptr->has_play_end_reason = VIDEO_PLAY_END_NONE;
        if(FSM_STRM_GOODBYE_IND ==  msg_ptr->msg_id)
        {        
            SendPlayEndToUI(fsm_ptr,VIDEO_PLAY_END_SUCCESS);
        }
        else if(FSM_PDP_DEACTIVE_IND == msg_ptr->msg_id)
        {
            //SendFatalErrorIndTOUI(fsm_ptr,VIDEO_ERROR_PDP_DEACTIVE);
        }
        else if(FSM_STRM_SERVER_CLOSE_IND == msg_ptr->msg_id)
        {
            SendFatalErrorIndTOUI(fsm_ptr,VIDEO_ERROR_SERVER_CLOSE);            
        }
        
        HandleStopReq(fsm_ptr,TRUE);
        FsmStateTrans(fsm_ptr,VIDEO_STATE_STOPING);
        break;

     case FSM_FRAME_END_IND:               
        HandleUpdateFrameReq(fsm_ptr);  
        break;
        
    case FSM_PAUSE_REQ:
        if (fsm_ptr->is_to_pause)
        {
            fsm_ptr->is_to_pause = FALSE;
            if(HandlePauseReqEx(TRUE, fsm_ptr))
            {
                fsm_ptr->is_not_notify_ui = TRUE;
                FsmStateTrans(fsm_ptr,VIDEO_STATE_PAUSING);
            }
        }
        else if(HandlePauseReq(fsm_ptr))
        {
            fsm_ptr->is_not_notify_ui = FALSE;
            FsmStateTrans(fsm_ptr,VIDEO_STATE_PAUSING);
        }            
        break;

    case FSM_SEEK_REQ:
        if(HandleSeekReq(fsm_ptr, (VIDEO_SEEK_PARAM*)(msg_ptr->msg_body)))
        {
            if (VIDEO_PLAY_FROM_NET != fsm_ptr->playType)
            {
                SendSeekCnfToUI(fsm_ptr,VIDEO_ERROR_NONE);      
                FsmStateTrans(fsm_ptr,VIDEO_STATE_PLAYING);                          
            }
            else
            {
                //通知streaming seek,进入seeking状态                   
                FsmStateTrans(fsm_ptr,VIDEO_STATE_SEEKING);               
            }

        }
        break;

   case FSM_CALLBACK_END_IND:     
        if (PNULL != msg_ptr->msg_body)
        {
            fsm_ptr->has_play_end_reason = *(VIDEO_PLAY_END_RESULT *)msg_ptr->msg_body;
        }
        break;
      
    default:
        result = FALSE;
        break;
    }
    return result;
#else
		return FALSE;
#endif

}

/*****************************************************************************/
//  Description : 播放定位中
//  Global resource dependence :  none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN FsmSeeking(VIDEO_PLAYER *fsm_ptr, VIDEO_FSM_MSG *msg_ptr)
{
#ifdef MMI_STREAMING_PLAYER_SUPPORT //20120612
    STRM_RESULT_E       strm_result     = 0;
    BOOLEAN             result          = TRUE;
    
    
    

    //参数检查
    if(PNULL == fsm_ptr || !IsValidFsm(fsm_ptr) || PNULL == fsm_ptr->vpCallBack || PNULL == msg_ptr)
    {
        //WMP_TRACE_LOW:"[Wre Player] FsmSeeking param is error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_5446_112_2_18_3_11_16_431,(uint8*)"");
        return FALSE;
    }

    //WMP_TRACE_LOW:"[Wre Player] FsmSeeking  msg_ptr->msg_id = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_5450_112_2_18_3_11_16_432,(uint8*)"d",msg_ptr->msg_id);
    switch(msg_ptr->msg_id)
    {
    case FSM_STRM_SEEK_CNF:
        if (PNULL != msg_ptr->msg_body)
        {
            strm_result = *(STRM_RESULT_E *)msg_ptr->msg_body;
        }
        else
        {
            strm_result = STRM_RESULT_UNKNOWN;
        }

        //WMP_TRACE_LOW:"[Wre Player] FsmSeeking strm_result = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_5463_112_2_18_3_11_16_433,(uint8*)"d",strm_result);
        
        switch(strm_result) 
        {
        case STRM_RESULT_SUCCESS:
            //成功seeking，进入playing状态  
            HandleUpdateFrameReq(fsm_ptr);       
            SendSeekCnfToUI(fsm_ptr,VIDEO_ERROR_NONE);            
            break;            
        
        default:
            //给ui发送其他错误，停止播放
            SendSeekCnfToUI(fsm_ptr,VIDEO_ERROR_SEEK_ERROR);   
            break;
        }     
        FsmStateTrans(fsm_ptr,VIDEO_STATE_PLAYING);
        break;
        
    case FSM_STRM_SERVER_CLOSE_IND:
    case FSM_PDP_DEACTIVE_IND:  
    case FSM_STOP_REQ:
        if(FSM_PDP_DEACTIVE_IND == msg_ptr->msg_id)
        {
            //SendFatalErrorIndTOUI(fsm_ptr,VIDEO_ERROR_PDP_DEACTIVE);
        }
        else if(FSM_STRM_SERVER_CLOSE_IND == msg_ptr->msg_id)
        {
            SendFatalErrorIndTOUI(fsm_ptr,VIDEO_ERROR_SERVER_CLOSE);            
        }
        HandleStopReq(fsm_ptr,TRUE);
        FsmStateTrans(fsm_ptr,VIDEO_STATE_STOPING);
        break;

   case FSM_PAUSE_REQ:
        if (fsm_ptr->is_to_pause)  //此时需要先停止底层播放，防止占用外面空间
        {
            DPLAYER_PauseEx(fsm_ptr->dplayer_handle);  
        }
        result = FALSE;
        break;

    case FSM_STRM_DATA_TIMEOUT_IND:
        //给ui发送数据超时的消息
        SendFatalErrorIndTOUI(fsm_ptr,VIDEO_ERROR_DATA_TIMEOUT);
        //停止当前播放
        StopVideo((HVIDEOHANDLE)fsm_ptr);       
        break;

    case FSM_STRM_GOODBYE_IND:    
    case FSM_CALLBACK_END_IND:     
        if (PNULL != msg_ptr->msg_body)
        {
            fsm_ptr->has_play_end_reason = *(VIDEO_PLAY_END_RESULT *)msg_ptr->msg_body;
        }
        else
        {
            fsm_ptr->has_play_end_reason = VIDEO_PLAY_END_SUCCESS;
        }
        break;

    default:
        result = FALSE;
        break;
    }
    return result;
#else
		return FALSE;
#endif

}

/*****************************************************************************/
//  Description : 暂停中
//  Global resource dependence :  none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN FsmPausing(VIDEO_PLAYER *fsm_ptr, VIDEO_FSM_MSG *msg_ptr)
{
#ifdef MMI_STREAMING_PLAYER_SUPPORT //20120612

    STRM_RESULT_E       strm_result     = 0;
    BOOLEAN             result          = TRUE;
    
    //参数检查
    if(PNULL == fsm_ptr || !IsValidFsm(fsm_ptr) || PNULL == fsm_ptr->vpCallBack || PNULL == msg_ptr)
    {
        //WMP_TRACE_LOW:"[Wre Player] FsmPausing param is error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_5542_112_2_18_3_11_16_434,(uint8*)"");
        return FALSE;
    }
    
    //WMP_TRACE_LOW:"[Wre Player] FsmPausing msg_ptr->msg_id = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_5546_112_2_18_3_11_16_435,(uint8*)"d",msg_ptr->msg_id);
    switch(msg_ptr->msg_id)
    {
    case FSM_STRM_PAUSE_CNF:
        if (PNULL == msg_ptr->msg_body)
        {
            strm_result = STRM_RESULT_UNKNOWN;
        }
        else
        {
            strm_result = *(STRM_RESULT_E *)msg_ptr->msg_body;
        }

        //WMP_TRACE_LOW:"[Wre Player] FsmPausing strm_result = %d, fsm_ptr->is_not_notify_ui = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_5559_112_2_18_3_11_16_436,(uint8*)"dd",strm_result,fsm_ptr->is_not_notify_ui);
        switch(strm_result) 
        {
        case STRM_RESULT_SUCCESS:
            //成功pause，进入paused状态
            fsm_ptr->process_time = DPLAYER_GetTime(fsm_ptr->dplayer_handle);            
            if (!fsm_ptr->is_not_notify_ui)
            {        
                fsm_ptr->is_not_notify_ui= FALSE;
                //WMP_TRACE_LOW:"[Wre Player] FsmPausing send pause cnf"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_5568_112_2_18_3_11_16_437,(uint8*)"");
                SendPauseCnfToUI(fsm_ptr,VIDEO_ERROR_NONE);          
            }
            FsmStateTrans(fsm_ptr,VIDEO_STATE_PAUSED);
            break;            
            
        default:
            fsm_ptr->is_not_notify_ui = FALSE;
            //给ui发送其他错误，停止播放    
            SendFatalErrorIndTOUI(fsm_ptr,VIDEO_ERROR_OTHERS);
            StopVideo((HVIDEOHANDLE)fsm_ptr);            
            break;
        }
        break;
        
    case FSM_STRM_SERVER_CLOSE_IND:
    case FSM_PDP_DEACTIVE_IND:   
    case FSM_STOP_REQ:
         fsm_ptr->is_not_notify_ui = FALSE;

         if(FSM_PDP_DEACTIVE_IND == msg_ptr->msg_id)
        {
            //SendFatalErrorIndTOUI(fsm_ptr,VIDEO_ERROR_PDP_DEACTIVE);
        }
        else if(FSM_STRM_SERVER_CLOSE_IND == msg_ptr->msg_id)
        {
            SendFatalErrorIndTOUI(fsm_ptr,VIDEO_ERROR_SERVER_CLOSE);            
        }
        
        HandleStopReq(fsm_ptr,TRUE);    
        FsmStateTrans(fsm_ptr,VIDEO_STATE_STOPING);
        break;        
   
    case FSM_STRM_GOODBYE_IND:    
    case FSM_CALLBACK_END_IND:     
        if (PNULL != msg_ptr->msg_body)
        {
            fsm_ptr->has_play_end_reason = *(VIDEO_PLAY_END_RESULT *)msg_ptr->msg_body;
        }
        else
        {
            fsm_ptr->has_play_end_reason = VIDEO_PLAY_END_SUCCESS;
        }
        break;
        
    default:
        result = FALSE;
        break;
    }
    return result;
#else
	return FALSE;
#endif
}

/*****************************************************************************/
//  Description : 暂停
//  Global resource dependence :  none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN FsmPaused(VIDEO_PLAYER *fsm_ptr, VIDEO_FSM_MSG *msg_ptr)
{
    BOOLEAN                 result = TRUE;   

    //参数检查
    if(PNULL == fsm_ptr || !IsValidFsm(fsm_ptr) || PNULL == fsm_ptr->vpCallBack || PNULL == msg_ptr)
    {
        //WMP_TRACE_LOW:"[Wre Player] FsmPaused param is error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_5631_112_2_18_3_11_16_438,(uint8*)"");
        return FALSE;
    }
    
    //WMP_TRACE_LOW:"[Wre Player] FsmPaused msg_ptr->msg_id = %x, is_to_pause = %d, is_to_resume = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_5635_112_2_18_3_11_16_439,(uint8*)"ddd",msg_ptr->msg_id,fsm_ptr->is_to_pause,fsm_ptr->is_to_resume);
    switch(msg_ptr->msg_id)
    {
    case FSM_ENTRY_REQ:       
        if(fsm_ptr->is_to_pause)
        {
            fsm_ptr->is_to_pause = FALSE;
            if(HandlePauseReqEx(FALSE, fsm_ptr))
            {
                FsmStateTrans(fsm_ptr,VIDEO_STATE_PAUSEDEX);
            }    
        }
        else if(fsm_ptr->is_to_resume)
        {
            //清除强制恢复标志
            fsm_ptr->is_to_resume = FALSE;      
            if(HandleResumeReqEx(TRUE,fsm_ptr))
            {
            #ifdef MMI_STREAMING_PLAYER_SUPPORT //20120612
                if (VIDEO_PLAY_FROM_NET == fsm_ptr->playType)
                {
                    //网络部分恢复streaming播放，进入resuming状态               
                    FsmStateTrans(fsm_ptr,VIDEO_STATE_RESUMING);                
                }
                else
		#endif
                {
                    //本地播放进入playing状态
                    FsmStateTrans(fsm_ptr,VIDEO_STATE_PLAYING);               
                }       
            }
        }
        break;

    case FSM_PAUSE_REQ:
        if(fsm_ptr->is_to_pause)
        {
            fsm_ptr->is_to_pause = FALSE;
            if (HandlePauseReqEx(FALSE,fsm_ptr))
            {
                FsmStateTrans(fsm_ptr,VIDEO_STATE_PAUSEDEX);
            }
            
        }
        break;        
    

    case FSM_SEEK_REQ:
        //WMP_TRACE_LOW:"[Wre Player] FsmPaused FSM_SEEK_REQ"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_5681_112_2_18_3_11_16_440,(uint8*)"");
        DPLAYER_Resume(fsm_ptr->dplayer_handle);
        if(HandleSeekReq(fsm_ptr, (VIDEO_SEEK_PARAM*)(msg_ptr->msg_body)))
        {
            FsmStateTrans(fsm_ptr,VIDEO_STATE_PAUSEDSEEKING);       
        }
        break;

    case FSM_RESUME_REQ:
        if(fsm_ptr->is_to_resume)
        {
            fsm_ptr->is_to_resume = FALSE;
            if (HandleResumeReqEx(TRUE,fsm_ptr))
            {    
            #ifdef MMI_STREAMING_PLAYER_SUPPORT //20120612
                if (VIDEO_PLAY_FROM_NET == fsm_ptr->playType)
                {
                    //网络部分恢复streaming播放，进入resuming状态               
                    FsmStateTrans(fsm_ptr,VIDEO_STATE_RESUMING);                
                }
                else
		#endif
                {
                    //本地播放进入playing状态
                    FsmStateTrans(fsm_ptr,VIDEO_STATE_PLAYING);               
                }   
            }
        }
        else if(HandleResumeReq(fsm_ptr))
        {
        #ifdef MMI_STREAMING_PLAYER_SUPPORT //20120612
            if (VIDEO_PLAY_FROM_NET == fsm_ptr->playType)
            {
                //网络部分恢复streaming播放，进入resuming状态               
                FsmStateTrans(fsm_ptr,VIDEO_STATE_RESUMING);                
            }
            else
	#endif
            {
                //本地播放进入playing状态
                SendPlayCnfToUI(fsm_ptr,VIDEO_ERROR_NONE);
                FsmStateTrans(fsm_ptr,VIDEO_STATE_PLAYING);
                
            }       
        }
        break;
        
    case FSM_STRM_SERVER_CLOSE_IND:
    case FSM_PDP_DEACTIVE_IND:
    case FSM_STOP_REQ:
        if(FSM_PDP_DEACTIVE_IND == msg_ptr->msg_id)
        {
            //SendFatalErrorIndTOUI(fsm_ptr,VIDEO_ERROR_PDP_DEACTIVE);
        }
        else if(FSM_STRM_SERVER_CLOSE_IND == msg_ptr->msg_id)
        {
            SendFatalErrorIndTOUI(fsm_ptr,VIDEO_ERROR_SERVER_CLOSE);            
        }
         HandleStopReq(fsm_ptr,TRUE); 
	 #ifdef MMI_STREAMING_PLAYER_SUPPORT //20120612
         if (VIDEO_PLAY_FROM_NET == fsm_ptr->playType)
         {
             FsmStateTrans(fsm_ptr,VIDEO_STATE_STOPING);
         }
         else
	#endif
         {
            FsmStateTrans(fsm_ptr,VIDEO_STATE_READY);
            SendStopIndToUI(fsm_ptr);
         }
        break;  

    
    case FSM_STRM_GOODBYE_IND:    
    case FSM_CALLBACK_END_IND:     
        if (PNULL != msg_ptr->msg_body)
        {
            fsm_ptr->has_play_end_reason = *(VIDEO_PLAY_END_RESULT *)msg_ptr->msg_body;
        }
        else
        {
            fsm_ptr->has_play_end_reason = VIDEO_PLAY_END_SUCCESS;
        }
        break;
        
    default:
        result = FALSE;
        break;
    }
    return result;
}

/*****************************************************************************/
//  Description : 暂停中播放定位中
//  Global resource dependence :  none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN FsmPausedSeeking(VIDEO_PLAYER *fsm_ptr, VIDEO_FSM_MSG *msg_ptr)
{
#ifdef MMI_STREAMING_PLAYER_SUPPORT //20120612
    STRM_RESULT_E       strm_result     = 0;
#endif
    BOOLEAN             result          = TRUE;
    
    
    

    //参数检查
    if(PNULL == fsm_ptr || !IsValidFsm(fsm_ptr) || PNULL == fsm_ptr->vpCallBack || PNULL == msg_ptr)
    {
        //WMP_TRACE_LOW:"[Wre Player] FsmPausedSeeking param is error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_5782_112_2_18_3_11_16_441,(uint8*)"");
        return FALSE;
    }

    //WMP_TRACE_LOW:"[Wre Player] FsmPausedSeeking  msg_ptr->msg_id = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_5786_112_2_18_3_11_16_442,(uint8*)"d",msg_ptr->msg_id);
    switch(msg_ptr->msg_id)
    {
    case FSM_ENTRY_REQ:
        if (VIDEO_PLAY_FROM_NET != fsm_ptr->playType)
        {            
            HandleUpdateFrameReq(fsm_ptr);       
            SendSeekCnfToUI(fsm_ptr,VIDEO_ERROR_NONE);  
            FsmStateTrans(fsm_ptr,VIDEO_STATE_PLAYING);
        }
        break;
#ifdef MMI_STREAMING_PLAYER_SUPPORT //20120612
    case FSM_STRM_SEEK_CNF:
        if (PNULL != msg_ptr->msg_body)
        {
            strm_result = *(STRM_RESULT_E *)msg_ptr->msg_body;
        }
        else
        {
            strm_result = STRM_RESULT_UNKNOWN;
        }

        //WMP_TRACE_LOW:"[Wre Player] FsmSeeking strm_result = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_5808_112_2_18_3_11_16_443,(uint8*)"d",strm_result);
        
        switch(strm_result) 
        {
        case STRM_RESULT_SUCCESS:
            //成功seeking，进入playing状态       
            HandleUpdateFrameReq(fsm_ptr);       
            SendSeekCnfToUI(fsm_ptr,VIDEO_ERROR_NONE);            
            break;            
        
        default:
            //给ui发送其他错误，停止播放
            SendSeekCnfToUI(fsm_ptr,VIDEO_ERROR_SEEK_ERROR);   
            break;
        }  
        FsmStateTrans(fsm_ptr,VIDEO_STATE_PLAYING);
        break;
     #endif  
   case FSM_PAUSE_REQ:
        if (fsm_ptr->is_to_pause)  //此时需要先停止底层播放，防止占用外面空间
        {
            DPLAYER_PauseEx(fsm_ptr->dplayer_handle);  //leichi 
        }
        result = FALSE;
        break;

    case FSM_STRM_SERVER_CLOSE_IND:
    case FSM_PDP_DEACTIVE_IND:  
    case FSM_STOP_REQ:
#ifdef MMI_STREAMING_PLAYER_SUPPORT //20120612
        if(FSM_PDP_DEACTIVE_IND == msg_ptr->msg_id)
        {
            //SendFatalErrorIndTOUI(fsm_ptr,VIDEO_ERROR_PDP_DEACTIVE);
        }
        else if(FSM_STRM_SERVER_CLOSE_IND == msg_ptr->msg_id)
        {
            SendFatalErrorIndTOUI(fsm_ptr,VIDEO_ERROR_SERVER_CLOSE);            
        }
#endif
        HandleStopReq(fsm_ptr,TRUE);
        FsmStateTrans(fsm_ptr,VIDEO_STATE_STOPING);
        break;
#ifdef MMI_STREAMING_PLAYER_SUPPORT //20120612

    case FSM_STRM_DATA_TIMEOUT_IND:
        //给ui发送数据超时的消息
        SendFatalErrorIndTOUI(fsm_ptr,VIDEO_ERROR_DATA_TIMEOUT);
        //停止当前播放
        StopVideo((HVIDEOHANDLE)fsm_ptr);       
        break;
#endif
    case FSM_STRM_GOODBYE_IND:    
    case FSM_CALLBACK_END_IND:     
        if (PNULL != msg_ptr->msg_body)
        {
            fsm_ptr->has_play_end_reason = *(VIDEO_PLAY_END_RESULT *)msg_ptr->msg_body;
        }
        else
        {
            fsm_ptr->has_play_end_reason = VIDEO_PLAY_END_SUCCESS;
        }
        break;

    default:
        result = FALSE;
        break;
    }
    return result;
}
/*****************************************************************************/
//  Description : 暂停后的player停止，streaming暂停
//  Global resource dependence :  none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN FsmPausedEx(VIDEO_PLAYER *fsm_ptr, VIDEO_FSM_MSG *msg_ptr)
{
    BOOLEAN                 result = TRUE;   

    //参数检查
    if(PNULL == fsm_ptr || !IsValidFsm(fsm_ptr) || PNULL == fsm_ptr->vpCallBack || PNULL == msg_ptr)
    {
        //WMP_TRACE_LOW:"[Wre Player] FsmPaused param is error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_5885_112_2_18_3_11_17_444,(uint8*)"");
        return FALSE;
    }
    
    //WMP_TRACE_LOW:"[Wre Player] FsmPaused msg_ptr->msg_id = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_5889_112_2_18_3_11_17_445,(uint8*)"d",msg_ptr->msg_id);
    switch(msg_ptr->msg_id)
    {
    case FSM_RESUME_REQ:
        fsm_ptr->is_to_resume = FALSE;
        if(HandleResumeReqEx(FALSE,fsm_ptr))
        {            
            FsmStateTrans(fsm_ptr,VIDEO_STATE_PAUSED);                   
        }
        break;
        
    case FSM_STRM_SERVER_CLOSE_IND:
    case FSM_PDP_DEACTIVE_IND:
    case FSM_STOP_REQ:
	#ifdef MMI_STREAMING_PLAYER_SUPPORT //20120612
        if(FSM_PDP_DEACTIVE_IND == msg_ptr->msg_id)
        {
            //SendFatalErrorIndTOUI(fsm_ptr,VIDEO_ERROR_PDP_DEACTIVE);
        }
        else if(FSM_STRM_SERVER_CLOSE_IND == msg_ptr->msg_id)
        {
            SendFatalErrorIndTOUI(fsm_ptr,VIDEO_ERROR_SERVER_CLOSE);            
        }
	#endif
         HandleStopReq(fsm_ptr,TRUE); 
	
#ifdef MMI_STREAMING_PLAYER_SUPPORT //20120612
         if (VIDEO_PLAY_FROM_NET == fsm_ptr->playType)
         {
             FsmStateTrans(fsm_ptr,VIDEO_STATE_STOPING);
         }
         else
#endif
         {
            FsmStateTrans(fsm_ptr,VIDEO_STATE_READY);
            SendStopIndToUI(fsm_ptr);
         }
        break;  

    case FSM_STRM_GOODBYE_IND:    
    case FSM_CALLBACK_END_IND:     
        if (PNULL != msg_ptr->msg_body)
        {
            fsm_ptr->has_play_end_reason = *(VIDEO_PLAY_END_RESULT *)msg_ptr->msg_body;
        }
        else
        {
            fsm_ptr->has_play_end_reason = VIDEO_PLAY_END_SUCCESS;
        }
        break;   
        
    default:
        result = FALSE;
        break;
    }
    return result;
}

/*****************************************************************************/
//  Description : 恢复中
//  Global resource dependence :  none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN FsmResuming(VIDEO_PLAYER *fsm_ptr, VIDEO_FSM_MSG *msg_ptr)
{
#ifdef MMI_STREAMING_PLAYER_SUPPORT //20120612

    STRM_RESULT_E   strm_result     = 0;
    BOOLEAN         result          = TRUE;    

    //参数检查
    if(PNULL == fsm_ptr || !IsValidFsm(fsm_ptr) || PNULL == fsm_ptr->vpCallBack || PNULL == msg_ptr)
    {
        return FALSE;
    }
    
    //WMP_TRACE_LOW:"[Wre Player] FsmResuming msg_ptr->msg_id = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_5957_112_2_18_3_11_17_446,(uint8*)"d",msg_ptr->msg_id);
    switch(msg_ptr->msg_id)
    {
    case FSM_STRM_RESUME_CNF:
        if (PNULL == msg_ptr->msg_body)
        {
            strm_result = STRM_RESULT_UNKNOWN;
        }
        else
        {
            strm_result = *(STRM_RESULT_E *)msg_ptr->msg_body;
        }
        //WMP_TRACE_LOW:"[Wre Player] FsmResuming strm_result = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_5969_112_2_18_3_11_17_447,(uint8*)"d",strm_result);
        switch(strm_result) 
        {
        case STRM_RESULT_SUCCESS:
            //成功resule，进入playing状态
            HandleUpdateFrameReq(fsm_ptr);       
            SendPlayCnfToUI(fsm_ptr,VIDEO_ERROR_NONE); 
            FsmStateTrans(fsm_ptr,VIDEO_STATE_PLAYING);                         
            break;            
        
        default:      
            SendPlayCnfToUI(fsm_ptr,VIDEO_ERROR_CONNECT_TIMEOUT); 
            StopVideo((HVIDEOHANDLE)fsm_ptr);           
            break;
        }
        break;
            
    case FSM_STRM_SERVER_CLOSE_IND:   
    case FSM_PDP_DEACTIVE_IND:
    case FSM_STOP_REQ:
        if(FSM_PDP_DEACTIVE_IND == msg_ptr->msg_id)
        {
            //SendFatalErrorIndTOUI(fsm_ptr,VIDEO_ERROR_PDP_DEACTIVE);
        }
        else if(FSM_STRM_SERVER_CLOSE_IND == msg_ptr->msg_id)
        {
            SendFatalErrorIndTOUI(fsm_ptr,VIDEO_ERROR_SERVER_CLOSE);            
        }
        HandleStopReq(fsm_ptr,TRUE);
        FsmStateTrans(fsm_ptr,VIDEO_STATE_STOPING);
        break;

    case FSM_STRM_DATA_TIMEOUT_IND:
        //给ui发送数据超时的消息
        SendFatalErrorIndTOUI(fsm_ptr,VIDEO_ERROR_DATA_TIMEOUT);
        //停止当前播放
        StopVideo((HVIDEOHANDLE)fsm_ptr);       
        break;

    case FSM_RESUME_REQ:
        break;

    case FSM_STRM_GOODBYE_IND:    
    case FSM_CALLBACK_END_IND:     
        if (PNULL != msg_ptr->msg_body)
        {
            fsm_ptr->has_play_end_reason = *(VIDEO_PLAY_END_RESULT *)msg_ptr->msg_body;
        }
        else
        {
            fsm_ptr->has_play_end_reason = VIDEO_PLAY_END_SUCCESS;
        }
        break;
        
    default:
        result = FALSE;
        break;
    }
    return result;
#else
	return FALSE;
#endif
}
#ifdef MMI_STREAMING_PLAYER_SUPPORT //20120612

/*****************************************************************************/
//  Description : 处理streaming init cnf消息
//  Global resource dependence :  none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL STRM_RESULT_E HandleInitCnf(VIDEO_PLAYER * fsm_ptr, STRM_INIT_CNF_T* strm_init_ptr, VIDEO_ERROR_TYPE *is_play_success)
{
    
    DPLAYER_STRM_CFG_T strm_cfg = {0};
    
    if (PNULL == fsm_ptr || !IsValidFsm(fsm_ptr) || PNULL == strm_init_ptr || PNULL == is_play_success)
    {
        //WMP_TRACE_LOW:"[Wre Player] HandleInitCnf param is null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_6041_112_2_18_3_11_17_448,(uint8*)"");
        return FALSE;
    }        
    //WMP_TRACE_LOW:"[Wre Player] HandleInitCnf strm_init_ptr->result = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_6044_112_2_18_3_11_17_449,(uint8*)"d",strm_init_ptr->result);
    switch(strm_init_ptr->result) 
    {
    case STRM_RESULT_SUCCESS:        
        //audio param
        if (PNULL != strm_init_ptr->sdp_info.audio_param.audio_type_name_ptr)
        {
            strm_cfg.audio_cfg.audio_codec_type_ptr = (uint16 *)SCI_ALLOCA((strlen(strm_init_ptr->sdp_info.audio_param.audio_type_name_ptr)+1)*sizeof(wchar));
            if (PNULL != strm_cfg.audio_cfg.audio_codec_type_ptr)
            {
                SCI_MEMSET(strm_cfg.audio_cfg.audio_codec_type_ptr,0,((strlen(strm_init_ptr->sdp_info.audio_param.audio_type_name_ptr)+1)*sizeof(wchar)));/*lint !e666*/
                MMIAPICOM_StrToWstr(((uint8 *)(strm_init_ptr->sdp_info.audio_param.audio_type_name_ptr)),((wchar *)(strm_cfg.audio_cfg.audio_codec_type_ptr)));
            }
        }
        
        strm_cfg.audio_cfg.sample_rate = strm_init_ptr->sdp_info.audio_param.sample_rate;
        strm_cfg.audio_cfg.channel_num = strm_init_ptr->sdp_info.audio_param.channel;
        strm_cfg.audio_cfg.cfg_len = strm_init_ptr->sdp_info.audio_param.config_len;
        strm_cfg.audio_cfg.cfg_ptr = strm_init_ptr->sdp_info.audio_param.config_ptr;
        strm_cfg.audio_cfg.duration = strm_init_ptr->sdp_info.audio_param.time_range.end_before_time-strm_init_ptr->sdp_info.audio_param.time_range.start_time +1;

        //video param
        if (PNULL != strm_init_ptr->sdp_info.video_param.video_type_name_ptr)
        {
            strm_cfg.video_cfg.video_codec_type_ptr = (uint16 *)SCI_ALLOCA((strlen(strm_init_ptr->sdp_info.video_param.video_type_name_ptr)+1)*sizeof(wchar));
            if (PNULL != strm_cfg.video_cfg.video_codec_type_ptr)
            {
                SCI_MEMSET(strm_cfg.video_cfg.video_codec_type_ptr,0,((strlen(strm_init_ptr->sdp_info.video_param.video_type_name_ptr)+1)*sizeof(wchar)));/*lint !e666*/
                MMIAPICOM_StrToWstr(((uint8 *)(strm_init_ptr->sdp_info.video_param.video_type_name_ptr)),((wchar *)(strm_cfg.video_cfg.video_codec_type_ptr)));
            }
        }
        
        strm_cfg.video_cfg.sample_rate = strm_init_ptr->sdp_info.video_param.sample_rate;       
        strm_cfg.video_cfg.duration = strm_init_ptr->sdp_info.video_param.time_range.end_before_time-strm_init_ptr->sdp_info.time_range.start_time+1;
        strm_cfg.video_cfg.frame_height = strm_init_ptr->sdp_info.video_param.frame_height;
        strm_cfg.video_cfg.frame_rate = strm_init_ptr->sdp_info.video_param.frame_rate;
        strm_cfg.video_cfg.frame_rate_base = strm_init_ptr->sdp_info.video_param.frame_rate_base;
        strm_cfg.video_cfg.frame_width = strm_init_ptr->sdp_info.video_param.frame_width;
        strm_cfg.video_cfg.sample_rate = strm_init_ptr->sdp_info.video_param.sample_rate;   
        strm_cfg.video_cfg.cfg_enhance_ptr = PNULL;
        strm_cfg.video_cfg.cfg_enhance_len = 0;
        strm_cfg.video_cfg.nal_header_length_size = 0;
        if (STRM_SDP_ENC_H264 == strm_init_ptr->sdp_info.video_fmtp_info.enc_type)
        {
            strm_cfg.video_cfg.cfg_len = strm_init_ptr->sdp_info.video_fmtp_info.fmtp_param.h264_param.sprop_sets_len;
            strm_cfg.video_cfg.cfg_ptr = strm_init_ptr->sdp_info.video_fmtp_info.fmtp_param.h264_param.sprop_sets_ptr;
        }
        else
        {
            strm_cfg.video_cfg.cfg_len = strm_init_ptr->sdp_info.video_param.config_len;
            strm_cfg.video_cfg.cfg_ptr = strm_init_ptr->sdp_info.video_param.config_ptr;
        }
        
        
        //time range
        strm_cfg.media_length = strm_init_ptr->sdp_info.time_range.end_before_time - strm_init_ptr->sdp_info.time_range.start_time +1;
        fsm_ptr->process_time = strm_init_ptr->sdp_info.time_range.start_time;
        fsm_ptr->total_time = strm_cfg.media_length;        
        strm_cfg.file_type = 0;
        
        
        //media name
        if (PNULL != fsm_ptr->strFilePath)
        {
            SCI_FREE(fsm_ptr->strFilePath);
            fsm_ptr->strFilePath = PNULL;
        }
        if (PNULL != strm_init_ptr->sdp_info.media_name_ptr)
        {
            fsm_ptr->strFilePath = (wchar *)SCI_ALLOCA((strlen(strm_init_ptr->sdp_info.media_name_ptr)+1) * sizeof(wchar));
            if(PNULL != fsm_ptr->strFilePath)
            {
                SCI_MEMSET(fsm_ptr->strFilePath,0,((strlen(strm_init_ptr->sdp_info.media_name_ptr)+1) * sizeof(wchar)));/*lint !e666*/
                MMIAPICOM_StrToWstr(((uint8 *)(strm_init_ptr->sdp_info.media_name_ptr)),fsm_ptr->strFilePath);
                fsm_ptr->uFilePathLen = (uint16)strlen(strm_init_ptr->sdp_info.media_name_ptr);
            }
            else
            {
                fsm_ptr->uFilePathLen = 0;
            } 
        }
                               
        //WMP_TRACE_LOW:"[Wre Player] HandleInitCnf frame height = %d, rate = %d, base = %d,width = %d, cfg_len = %d, fsm_ptr->total_time= %d, strm_cfg.video_cfg.cfg_ptr = %x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_6133_112_2_18_3_11_17_450,(uint8*)"ddddddd",strm_cfg.video_cfg.frame_height,strm_cfg.video_cfg.frame_rate,strm_cfg.video_cfg.frame_rate_base,strm_cfg.video_cfg.frame_width,strm_cfg.video_cfg.cfg_len,fsm_ptr->total_time,strm_cfg.video_cfg.cfg_ptr);
        
        //开始播放
        *is_play_success = PlayFromNet(fsm_ptr,strm_cfg);
        if (PNULL != strm_cfg.audio_cfg.audio_codec_type_ptr)
        {
            SCI_FREE(strm_cfg.audio_cfg.audio_codec_type_ptr);
            strm_cfg.audio_cfg.audio_codec_type_ptr = PNULL;
        }
        if (PNULL != strm_cfg.video_cfg.video_codec_type_ptr)
        {
            SCI_FREE(strm_cfg.video_cfg.video_codec_type_ptr);
            strm_cfg.video_cfg.video_codec_type_ptr = PNULL;
        }        
        break;           

    default:                
        fsm_ptr->strm_handle = PNULL;        
        break;
    }

    return strm_init_ptr->result;
}
#endif
/*****************************************************************************/
//  Description : 处理update frame请求消息
//  Global resource dependence :  none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void HandleUpdateFrameReq(VIDEO_PLAYER * fsm_ptr)
{
    if (PNULL == fsm_ptr || !IsValidFsm(fsm_ptr))
    {
        //WMP_TRACE_LOW:"[Wre Player] HandleUpdateFrame param is null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_6164_112_2_18_3_11_17_451,(uint8*)"");
        return;
    }

   // WMP_TRACE_LOW("[Wre Player] HandleUpdateFrameReq fsm_ptr->is_to_exit = %d, fsm_ptr->is_to_pause = %d, fsm_ptr->display_param.videoSupport = %d",fsm_ptr->is_to_exit,fsm_ptr->is_to_pause,fsm_ptr->display_param.videoSupport);
    if (!fsm_ptr->is_to_exit && !fsm_ptr->is_to_pause && 1 == fsm_ptr->LcdDisplayParam.videoSupport)
    {  
      //  DPLAYER_UpdateNextFrame(DPLAYER_NORMAL_HANDLE); //deleted by leichi
	DPLAYER_UpdateNextFrame(fsm_ptr->dplayer_handle);//added by leichi
	MMITHEME_UpdateRect();
    } 
}
/*****************************************************************************/
//  Description : 处理pause请求消息
//  Global resource dependence :  none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN HandlePauseReq(VIDEO_PLAYER * fsm_ptr)
{
    BOOLEAN    result = FALSE;
    if (PNULL == fsm_ptr || !IsValidFsm(fsm_ptr))
    {
        //WMP_TRACE_LOW:"[Wre Player] HandlePauseReq param is null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_6184_112_2_18_3_11_17_452,(uint8*)"");
        return FALSE;
    }

    fsm_ptr->process_time = DPLAYER_GetTime(fsm_ptr->dplayer_handle);
    
    if(DPLAYER_Pause(fsm_ptr->dplayer_handle))
    {
        return FALSE;
    }
    
    SwitchPlayDevice((HVIDEOHANDLE)fsm_ptr,fsm_ptr->bSupportA2dp,fsm_ptr->is_bt_support);

    if (VIDEO_PLAY_FROM_NET != fsm_ptr->playType)
    {                    
        result =  TRUE;
        return result;
    }
#ifdef MMI_STREAMING_PLAYER_SUPPORT //20120612

    else if (STRM_Pause(fsm_ptr->strm_handle))
    {
        result =  TRUE;      
    }
#endif
    else
    {
        //WMP_TRACE_LOW:"[Wre Player] HandlePauseReq can not pause, so stop"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_6208_112_2_18_3_11_17_453,(uint8*)"");
        //steaming暂停失败，stop     
        SendFatalErrorIndTOUI(fsm_ptr,VIDEO_ERROR_OTHERS);
        StopVideo((HVIDEOHANDLE)fsm_ptr);       
    } 
    

    return result;
}

/*****************************************************************************/
//  Description : 处理resume请求消息
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN HandleResumeReq(VIDEO_PLAYER* fsm_ptr)
{
    BOOLEAN result = TRUE;
#ifdef MMI_STREAMING_PLAYER_SUPPORT //20120612
    STRM_RESUME_PARAM_T strm_resume_param = {0};
#endif

    if (PNULL == fsm_ptr || !IsValidFsm(fsm_ptr))
    {
        //WMP_TRACE_LOW:"[Wre Player] HandleResumeReq param is null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_6229_112_2_18_3_11_17_454,(uint8*)"");
        return FALSE;
    }
    
    MMIAPISET_InitAudioDevice(fsm_ptr->vol);     
    SwitchPlayDevice((HVIDEOHANDLE)fsm_ptr,fsm_ptr->bSupportA2dp,fsm_ptr->is_bt_support);
    DPLAYER_Resume(fsm_ptr->dplayer_handle);    
#ifdef MMI_STREAMING_PLAYER_SUPPORT //20120612

    if (VIDEO_PLAY_FROM_NET == fsm_ptr->playType)
    {
        //网络部分恢复streaming播放，进入resuming状态
        strm_resume_param.start_time = MAX_INVALID_VALUE;
        if(!STRM_Resume(fsm_ptr->strm_handle,&strm_resume_param))            
        {
            SendFatalErrorIndTOUI(fsm_ptr,VIDEO_ERROR_OTHERS);
            StopVideo((HVIDEOHANDLE)fsm_ptr);
            result = FALSE;
        }
    }        
   #endif
    return result;
}
/*****************************************************************************/
//  Description : 处理seek请求消息
//  Global resource dependence :  none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN HandleSeekReq(VIDEO_PLAYER * fsm_ptr, VIDEO_SEEK_PARAM *seek_param)
{    
    STRM_SEEK_PARAM_T   strm_seek_param = {0};
    uint32              seek_time = 0;//@andy.he  andyhe_testwaring  20110620

    if (PNULL == fsm_ptr || !IsValidFsm(fsm_ptr) || PNULL == seek_param)
    {
        //WMP_TRACE_LOW:"[Wre Player] HandleSeekReq param is null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_6261_112_2_18_3_11_17_455,(uint8*)"");
        return FALSE;
    }
    seek_time = seek_param->nSeekTime;//@andy.he  andyhe_testwaring  20110620
     
    //WMP_TRACE_LOW:"[Wre Player] FsmPlaying seek_time = %x, seek_param->nSeekFrame = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_6266_112_2_18_3_11_17_456,(uint8*)"dd",seek_time,seek_param->nSeekFrame);
    if (MAX_INVALID_VALUE == seek_time )
    {
        //seek_time无效，通过frame计算seek_time
        if (MAX_INVALID_VALUE == seek_param->nSeekFrame)
        {
            return FALSE;
        }
        seek_time = FrameToTime(fsm_ptr->dplayer_handle,seek_param->nSeekFrame);
    }            
    //设置dplayer时间
    DPLAYER_SetTime(fsm_ptr->dplayer_handle,seek_time);
    strm_seek_param.seek_to_time = seek_time;
    
    if (VIDEO_PLAY_FROM_NET != fsm_ptr->playType)
    {
        fsm_ptr->process_time = seek_time;
        return TRUE;       
    }
#ifdef MMI_STREAMING_PLAYER_SUPPORT //20120612

    else if (STRM_Seek(fsm_ptr->strm_handle,&strm_seek_param))
    {
        return TRUE;       
    }
#endif
    else
    {        
        //WMP_TRACE_LOW:"[Wre Player] HandleSeekReq streaming see error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_6291_112_2_18_3_11_17_457,(uint8*)"");
        return FALSE;
    }
}
/*****************************************************************************/
//  Description : 处理停止请求消息
//  Global resource dependence :  none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN HandleStopReq(VIDEO_PLAYER* fsm_ptr, BOOLEAN is_begin_play)
{
  //  uint32          last_play_point = 0;    
    
    if (PNULL == fsm_ptr || !IsValidFsm(fsm_ptr))
    {
        //WMP_TRACE_LOW:"[Wre Player] HandleStopReq param is null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_6305_112_2_18_3_11_17_458,(uint8*)"");
        return FALSE;
    }

    //WMP_TRACE_LOW:"[Wre Player] HandleStopReq is_begin_play = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_6309_112_2_18_3_11_17_459,(uint8*)"d", is_begin_play);
    
    if(is_begin_play)
    {        
        DPLAYER_Stop(fsm_ptr->dplayer_handle);
       /* while(!s_is_callback_called)
        {
            SCI_Sleep(200);
        }*/
        DPLAYER_Close(fsm_ptr->dplayer_handle);
        fsm_ptr->dplayer_handle = 0;
        DPLAYER_Release();
	#ifdef MMI_STREAMING_PLAYER_SUPPORT //20120612
        if (VIDEO_PLAY_FROM_NET == fsm_ptr->playType)
        {
            STRM_Stop(fsm_ptr->strm_handle);                
        }
	#endif
    }
#ifdef MMI_STREAMING_PLAYER_SUPPORT //20120612
    else if (VIDEO_PLAY_FROM_NET == fsm_ptr->playType && 0 != fsm_ptr->strm_handle)
    {
        STRM_Close(fsm_ptr->strm_handle);
    }
#endif
    return TRUE;
}
/*****************************************************************************/
//  Description : 停止中
//  Global resource dependence :  none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN FsmStopping(VIDEO_PLAYER *fsm_ptr, VIDEO_FSM_MSG *msg_ptr)
{
    BOOLEAN             result          = TRUE;

    //参数检查
    if(PNULL == fsm_ptr || !IsValidFsm(fsm_ptr) || PNULL == fsm_ptr->vpCallBack || PNULL == msg_ptr)
    {
        //WMP_TRACE_LOW:"[Wre Player] FsmStopping param is error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_6344_112_2_18_3_11_18_460,(uint8*)"");
        return FALSE;
    }
    
    //WMP_TRACE_LOW:"[Wre Player] FsmStopping msg_ptr->msg_id = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_6348_112_2_18_3_11_18_461,(uint8*)"d",msg_ptr->msg_id);
    switch(msg_ptr->msg_id)
    {
    case FSM_ENTRY_REQ:    
        if (fsm_ptr->is_to_exit)
        {
            FsmDispatch((HVIDEOHANDLE)fsm_ptr,FSM_STRM_STOP_CNF,PNULL,PNULL);   //立即发stop cnf
        }        
        break;
#ifdef MMI_STREAMING_PLAYER_SUPPORT //20120612
    case FSM_STRM_STOP_CNF:
        FsmStateTrans(fsm_ptr,VIDEO_STATE_STOPED);        
        break;
#endif
    default:
        result = FALSE;
        break;
    }
    return result;
}

/*****************************************************************************/
//  Description : 停止
//  Global resource dependence :  none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN FsmStopped(VIDEO_PLAYER *fsm_ptr, VIDEO_FSM_MSG *msg_ptr)
{
    BOOLEAN             result          = TRUE;
    //参数检查
    if(PNULL == fsm_ptr || !IsValidFsm(fsm_ptr) || PNULL == fsm_ptr->vpCallBack || PNULL == msg_ptr)
    {
        //WMP_TRACE_LOW:"[Wre Player] FsmStopped param is error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_6379_112_2_18_3_11_18_462,(uint8*)"");
        return FALSE;
    }

    switch(msg_ptr->msg_id)
    {
    case FSM_ENTRY_REQ:   
	
#ifdef MMI_STREAMING_PLAYER_SUPPORT		//20120612
        if (STRM_Close(fsm_ptr->strm_handle))
        {       
            FsmStateTrans(fsm_ptr,VIDEO_STATE_CLOSING);
        }
        else
#endif
        {
            FsmStateTrans(fsm_ptr,VIDEO_STATE_READY);
            SendStopIndToUI(fsm_ptr);
        }       
        break;

    default:
        result = FALSE;
        break;
    }
    
    return result;
}

/*****************************************************************************/
//  Description : 关闭中
//  Global resource dependence :  none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN FsmClosing(VIDEO_PLAYER *fsm_ptr, VIDEO_FSM_MSG *msg_ptr)
{
    BOOLEAN                result = TRUE;
    

    //参数检查
    if(PNULL == fsm_ptr || !IsValidFsm(fsm_ptr) || PNULL == fsm_ptr->vpCallBack || PNULL == msg_ptr)
    {
        //WMP_TRACE_LOW:"[Wre Player] FsmClosing param is error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_6417_112_2_18_3_11_18_463,(uint8*)"");
        return FALSE;
    }

    //WMP_TRACE_LOW:"[Wre Player] FsmClosing msg_ptr->msg_id = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_6421_112_2_18_3_11_18_464,(uint8*)"d",msg_ptr->msg_id);
    switch(msg_ptr->msg_id)
    {
    case FSM_ENTRY_REQ:    
        if (fsm_ptr->is_to_exit)
        {
            FsmDispatch((HVIDEOHANDLE)fsm_ptr,FSM_STRM_CLOSE_CNF,PNULL,PNULL);
        }        
        break;
#ifdef MMI_STREAMING_PLAYER_SUPPORT //20120612
    case FSM_STRM_CLOSE_CNF:
        //给ui发送VIDEO_NOTIFY_STOP，通知stop   
        fsm_ptr->strm_handle = 0;
        FsmStateTrans(fsm_ptr,VIDEO_STATE_READY);
        SendStopIndToUI(fsm_ptr);       
        break;
#endif
    default:
        result = FALSE;
        break;
    }    
    return result;
}

/*****************************************************************************/
//  Description : special resume
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN HandleResumeReqEx(BOOLEAN is_resume_streaming,
                                VIDEO_PLAYER* fsm_ptr)
{
    BOOLEAN result = TRUE;
    STRM_RESUME_PARAM_T strm_resume_param = {0};   
    DPLAYER_RETURN_E  dplayer_result=SCI_SUCCESS;//andy.he cr240364 20110620
	
    if (PNULL == fsm_ptr || !IsValidFsm(fsm_ptr))
    {
        return FALSE;
    }
	
    MMIAPISET_InitAudioDevice(fsm_ptr->vol);         
    dplayer_result=DPLAYER_ResumeEx(fsm_ptr->dplayer_handle); //andy.he cr240364 20110620
    SwitchPlayDevice((HVIDEOHANDLE)fsm_ptr,fsm_ptr->bSupportA2dp,fsm_ptr->is_bt_support);
    SwitchDisplayMode((HVIDEOHANDLE)fsm_ptr,&(fsm_ptr->LcdDisplayParam),TRUE);    
    
    if(SCI_SUCCESS!=dplayer_result)//andy.he cr240364 20110620
    {
        SendFatalErrorIndTOUI(fsm_ptr,VIDEO_ERROR_OTHERS);
        StopVideo((HVIDEOHANDLE)fsm_ptr);  
        return FALSE;
    }
#ifdef MMI_STREAMING_PLAYER_SUPPORT //20120612	
    if (is_resume_streaming)
    {   
        if (VIDEO_PLAY_FROM_NET == fsm_ptr->playType && MAX_INVALID_VALUE != fsm_ptr->total_time)
        {  
			strm_resume_param.start_time = MAX_INVALID_VALUE;  
			//result = STRM_Resume(fsm_ptr->dplayer_handle,&strm_resume_param);//del by leichi 20120316
			result = STRM_Resume(fsm_ptr->strm_handle,&strm_resume_param);//added by leichi 20120316

        }
    }
    else
#endif
	{
        DPLAYER_Pause(fsm_ptr->dplayer_handle);
        SwitchPlayDevice((HVIDEOHANDLE)fsm_ptr,fsm_ptr->bSupportA2dp,fsm_ptr->is_bt_support);
    }
#ifdef MMI_STREAMING_PLAYER_SUPPORT	
    if(!result)
    {  
		SendFatalErrorIndTOUI(fsm_ptr,VIDEO_ERROR_OTHERS);
		StopVideo((HVIDEOHANDLE)fsm_ptr);       
    } 
#endif	
    return result;
}

/*****************************************************************************/
//  Description : special pause
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN HandlePauseReqEx(BOOLEAN is_pause_streaming,
                               VIDEO_PLAYER* fsm_ptr)
{
	BOOLEAN result = TRUE;

	if (PNULL == fsm_ptr || !IsValidFsm(fsm_ptr))
	{
		return FALSE;
	}

	DPLAYER_PauseEx(fsm_ptr->dplayer_handle);  

	SwitchPlayDevice((HVIDEOHANDLE)fsm_ptr,fsm_ptr->bSupportA2dp,fsm_ptr->is_bt_support);
	DPLAYER_Pause(fsm_ptr->dplayer_handle);  
	#ifdef MMI_STREAMING_PLAYER_SUPPORT //20120612
	if (is_pause_streaming && VIDEO_PLAY_FROM_NET == fsm_ptr->playType && MAX_INVALID_VALUE != fsm_ptr->total_time)
	{  
		result = STRM_Pause(fsm_ptr->strm_handle);
    }
	
    if(!result)
    {          
		StopVideo((HVIDEOHANDLE)fsm_ptr);       
    } 
	#endif
    return result;
}

/*****************************************************************************/
//  Description : SendSignalToAPP
//  Global resource dependence : 
//  Author: .zhang
//  Note: 
/*****************************************************************************/
LOCAL void SendSignalToCurPlayingTask(uint16 signalType, uint16 data, uint32 task_id)
{
    MmiAppCopyMsgS *sendSignal = PNULL;    
	//WMP_TRACE_LOW:"[Wre Player] SendSignalToCurPlayingTask signalType = %d, data = %d, task_id = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_6533_112_2_18_3_11_18_465,(uint8*)"ddd",signalType,data,task_id);
	
    MmiCreateSignal(signalType, sizeof(MmiAppCopyMsgS), (MmiSignalS**)&sendSignal);   
    sendSignal->data = data;
    MmiSendSignal(task_id, (MmiSignalS*)sendSignal);	
}

/*****************************************************************************/
//  Description : align 4
//  Global resource dependence : 
//  Author: bruce.chi
//  Note: 
/*****************************************************************************/
LOCAL uint32 GetAlign4Offset(uint32 x)
{
    uint32 z = 0;
    uint32 offset = 0;
	
    z = x % 4;
    if (z == 0) //do not need align
    {
        offset = 0;
    }
    else
    {
        offset = 4 - z;  //test: if x==7, offset = 4 - 3 = 1
    }
	
    return offset;
}

/*****************************************************************************/
//  Description : 切换显示模式（全屏，半屏，TVOUT)
//  Global resource dependence :  none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN SwitchDisplayMode(HVIDEOHANDLE  hVideo,LCD_VIDEO_DISPLAY_PARAM  *display_param_ptr,BOOLEAN is_only_rect)
{
    BOOLEAN                 result          =   TRUE;
    VIDEO_PLAYER                *fsm_ptr        = (VIDEO_PLAYER *)hVideo;
	// DPLAYER_MEDIA_INFO_T    p_info          = {0};
    LCD_VIDEO_DISPLAY_PARAM      display_param   = {0};
    
    //检查hVideo的有效性；
    if (PNULL == fsm_ptr || !IsValidFsm(fsm_ptr) || PNULL == display_param_ptr)
    {
        //WMP_TRACE_LOW:"[Wre Player] SwitchDisplayMode the fsm is wrong fsm_ptr = %x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_6576_112_2_18_3_11_18_466,(uint8*)"d",fsm_ptr);
        return FALSE;
    }    
    
    //WMP_TRACE_LOW:"[Wre Player] SwitchDisplayMode fsm_ptr->state = %d, is_only_rect = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_6580_112_2_18_3_11_18_467,(uint8*)"dd",fsm_ptr->state,is_only_rect);
//    WMP_TRACE_LOW("[Wre Player] SwitchDisplayMode lcd_id = %d, block_id = %d, x= %d",display_param_ptr->LcdId,display_param_ptr->LcdBlkId,display_param_ptr->dispRect.x);
    if (is_only_rect)
    {
        fsm_ptr->LcdDisplayParam.disp_rect.y = display_param_ptr->disp_rect.y;
        fsm_ptr->LcdDisplayParam.disp_rect.x = display_param_ptr->disp_rect.x;
        fsm_ptr->LcdDisplayParam.disp_rect.dx = display_param_ptr->disp_rect.dx;
        fsm_ptr->LcdDisplayParam.disp_rect.dy = display_param_ptr->disp_rect.dy;
        fsm_ptr->LcdDisplayParam.target_rect.y = display_param_ptr->target_rect.y;
        fsm_ptr->LcdDisplayParam.target_rect.x = display_param_ptr->target_rect.x;
        fsm_ptr->LcdDisplayParam.target_rect.dx = display_param_ptr->target_rect.dx;
        fsm_ptr->LcdDisplayParam.target_rect.dy = display_param_ptr->target_rect.dy;
        fsm_ptr->LcdDisplayParam.RotateAngle = display_param_ptr->RotateAngle;        
    }
    else
    {
        SCI_MEMCPY(&(fsm_ptr->LcdDisplayParam),display_param_ptr,sizeof(LCD_VIDEO_DISPLAY_PARAM));
    }
	
    if (PNULL != fsm_ptr->dplayer_handle)
    {    
        if (VIDEO_STATE_READY != fsm_ptr->state && VIDEO_STATE_PAUSING != fsm_ptr->state && VIDEO_STATE_PAUSED != fsm_ptr->state)
        {
            //DPLAYER_Pause(fsm_ptr->dplayer_handle);
            SwitchPlayDevice((HVIDEOHANDLE)fsm_ptr,fsm_ptr->bSupportA2dp,fsm_ptr->is_bt_support);
        } 
		
        SCI_MEMCPY(&display_param,&(fsm_ptr->LcdDisplayParam),sizeof(LCD_VIDEO_DISPLAY_PARAM));
        TranslateDisplayParamLogicToPhysical(&display_param, FALSE);
		
        if(DPLAYER_SUCCESS != DPLAYER_SetDisplayParam(fsm_ptr->dplayer_handle, &display_param))
        {
            result =  FALSE;
        } 
		
      //  DPLAYER_UpdateCurrentFrame(DPLAYER_NORMAL_HANDLE);//leichi
                DPLAYER_UpdateCurrentFrame(fsm_ptr->dplayer_handle);//leichi


		
        if (VIDEO_STATE_READY != fsm_ptr->state && VIDEO_STATE_PAUSING != fsm_ptr->state && VIDEO_STATE_PAUSED != fsm_ptr->state)
        {
            MMIAPISET_InitAudioDevice(fsm_ptr->vol);     
            SwitchPlayDevice((HVIDEOHANDLE)fsm_ptr,fsm_ptr->bSupportA2dp,fsm_ptr->is_bt_support);
            //DPLAYER_Resume(fsm_ptr->dplayer_handle);            
        }    
    }
    
    //WMP_TRACE_LOW:"[Wre Player] SwitchDisplayMode result = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_VIDEOPLAYER_6625_112_2_18_3_11_18_468,(uint8*)"d",result);
    return result;
}


#ifdef VIDEO_PLAYER_SUPPORT
/*****************************************************************************/
//  Description : 
//  Global resource dependence :  none
//  Author: 
//  Note: 
/*****************************************************************************/
void Adapt_EntryVideoPlayer(
							uint32 nVideoType,
							uint16 * strFileName,
							uint32  nFileNameLen,
							uint8  *  strUrl 
							)
{
#ifdef MMIWRE_MEM_CONFLICT_SOLUTION
    SCI_TRACE_LOW("Adapt_EntryVideoPlayer 11111");
    if(wretomocorapp.AppType == WRETOMORCOR_NONE)
    {
        PWRETOMOCOR_VIDEO_PARAM lpParam = NULL;
  
        wretomocorapp.AppType = WRETOMORCOR_VIDEOPLAYER;
        WreToMocorApp_Realse();
        wretomocorapp.lpParam = (unsigned short *)SCI_ALLOCA(sizeof(WRETOMOCOR_VIDEO_PARAM));
        memset((char *)wretomocorapp.lpParam, 0, sizeof(WRETOMOCOR_VIDEO_PARAM));
        lpParam = (PWRETOMOCOR_VIDEO_PARAM) wretomocorapp.lpParam;
        MMIAPICOM_Wstrcpy((wchar *) lpParam->strFileName, strFileName);
        lpParam->nFileNameLen = nFileNameLen;
        lpParam->VideoType = nVideoType;
        if(strUrl != NULL)
        {
            SCI_STRCPY(lpParam->strUrl, strUrl);
        }
        else
        {
            lpParam->strUrl = NULL;
        }
        MMIWRE_CloseWin();
        SCI_TRACE_LOW("Adapt_EntryVideoPlayer, malloc memory");
        return;
    }
#endif
    SCI_TRACE_LOW("Adapt_EntryVideoPlayer 2222");
	if(nVideoType == VIDEO_PLAY_FROM_FILE)
		MMIAPIVP_PlayVideoFromVideoPlayer(VP_ENTRY_FROM_FILE,strFileName,nFileNameLen,strUrl);
	#ifdef MMI_STREAMING_PLAYER_SUPPORT //20120612
	else if(nVideoType == VIDEO_PLAY_FROM_NET)
		MMIAPIVP_PlayVideoFromVideoPlayer(VP_ENTRY_FROM_NET,strFileName,nFileNameLen,strUrl);
	#endif

#ifdef MMIWRE_MEM_CONFLICT_SOLUTION
    WreToMocorApp_Realse();
#endif
}
#else
void Adapt_EntryVideoPlayer(
							uint32 nVideoType,
							uint16 * strFileName,
							uint32  nFileNameLen,
							uint8  *  strUrl 
							)
{
	return;
}
#endif
#endif


