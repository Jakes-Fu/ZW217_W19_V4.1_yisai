/*****************************************************************************
** File Name:      mmivp_export.h                                                   *
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

#ifndef _MMIVP_EXPORT_H_
#define _MMIVP_EXPORT_H_

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "dal_player.h"
#include "ffs.h"
#include "mmk_type.h"
#include "mn_type.h"
#include "mmi_custom_define.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmi_appmsg.h"
#include "mmifmm_export.h"

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
    
#define VP_MAX_IP_LENGTH            15                       //ip地址的最大长度
#define VP_DEFAULT_DIR              MMIMULTIM_DIR_MOVIE    //默认文件夹
#define VP_DEFAULT_DIR_LEN          MMIAPICOM_Wstrlen(MMIMULTIM_DIR_MOVIE)  //默认文件夹名称长度

#define KM_DEFAULT_DIR              MMIMULTIM_DIR_KINGMOVIE    //电影王文件夹
#define KM_DEFAULT_DIR_LEN          MMIAPICOM_Wstrlen(MMIMULTIM_DIR_KINGMOVIE)  //电影王文件夹名称长度

////文件的后处理操作 
typedef void (*VP_POST_PROGRESS)(wchar*  full_path_name,uint16 file_full_path_len,int32 process_time);


/**--------------------------------------------------------------------------*
**                         TYPE AND CONSTANT                                *
**--------------------------------------------------------------------------*/   
//定义了视频播放器对视频文件支持的程度类型。
typedef enum
{
    VP_SUPPORTED_AUDIO_ONLY,            //仅支持音频解码播放
        VP_SUPPORTED_VIDEO_ONLY,        //仅支持图像解码播放
        VP_SUPPORTED_BOTH,              //可正常播放，图像解码和音频解码均支持
        VP_SUPPORTED_NEITHER,           //无法播放，图像解码和音频解码均不支持
        VP_SUPPORTED_OVER_SCALED,       //片源规格（最大分辨率）超出平台的支持范围 片源规格（最大分辨率）：640*480
        VP_SUPPORTED_STATUS_TYPE_MAX    //系统保留，不可使用
} VP_SUPPORTED_STATUS_TYPE_E;  

//视频播放器控制层状态机的状态
typedef enum
{
    VP_STATE_IDLE,              //空闲
        VP_STATE_READY,         //准备好
        VP_STATE_PDP_ACTIVING,  //PDP激活中
        VP_STATE_INIT,          //已初始化
        VP_STATE_INIT_PAUSED_EX,  //已初始化完成时进入特殊暂停状态
        VP_STATE_INIT_AUTH,     //认证中
        VP_STATE_PLAY_PENDING,  //播放准备中
        VP_STATE_PLAY_AUTH,     //认证中
        VP_STATE_PLAYING,       //播放中
        VP_STATE_BUFFERING,     //缓冲中
        VP_STATE_SEEKING,       //播放定位中
        VP_STATE_PAUSING,       //暂停中
        VP_STATE_PAUSED,        //已暂停
        VP_STATE_PAUSED_EX,     //暂停后进入streaming暂停，player停止
        VP_STATE_PAUSED_SEEKING, //暂停中定位播放
        VP_STATE_RESUMING,      //恢复中
        VP_STATE_STOPING,       //停止中
        VP_STATE_STOPED,        //已停止
        VP_STATE_CLOSING,       //关闭中
        VP_STATE_MAX            //保留
} VP_STATE_E; 

//控制层给UI层发送的消息类型
typedef enum
{
    VP_BT_STATE_NONE,                   //不可用
    VP_BT_STATE_OPEN_WATTING,           //等待打开
    VP_BT_STATE_PLAY,                   //播放中
    VP_BT_STATE_PAUSE,                   //播放中
    VP_BT_STATE_PAUSE_WATTING,         //等待pause
    VP_BT_STATE_RESUME,                 //恢复
    VP_BT_STATE_RESUME_WATTING,         //等待RESUME
    VP_BT_STATE_MAX                     //保留
} VP_BT_STATE_E; 

//控制层给UI层发送的消息类型
typedef enum
{
    VP_PREPARE_IND = MSG_VP_INTERNAL_MSG_BEGIN,         //0x4120播放连接准备完毕。UI层可依据返回参数显示片源名，时长等信息
        VP_PLAY_PROCESS_IND,    //0x4121报告当前的播放进度（每播放一帧都会报该消息）
        VP_BUFFERING_IND,       //缓冲开始
        VP_BUFFER_END_IND,      //缓冲结束
        VP_BUFFER_PROCESS_IND,  //报告当前缓冲进度
        VP_FATAL_ERROR_IND,     //0x4125播放过程中出现致命错误
        VP_AUTH_IND,            //需要授权认证
        VP_PLAY_END_IND,        //播放完毕结束
        VP_STOP_IND,            //播放已停止
        VP_PLAY_CNF,            //开始播放
        VP_PAUSE_CNF,           //0x412a暂停
        VP_SEEK_CNF,            //定位恢复
        VP_MSG_MAX              //0x412c系统保留，不可使用
} VP_MSG_E; 


//播放方式
typedef enum
{
    VP_PLAY_FROM_NONE = 0,           //nothing
    VP_PLAY_FROM_FILE ,              //从本地文件播放
        VP_PLAY_FROM_BUFFER,        //从本地BUFFER播放
        VP_PLAY_FROM_NET,           //从网络播放
        VP_PLAY_FROM_LOCALLIST,     //从本地播放列表中播放
        VP_PLAY_FROM_MAX            //保留
} VP_PLAY_TYPE_E;

//循环播放模式类型
typedef enum
{
    VP_REPEAT_PLAY_MODE_CLOSE = 0,      //关闭循环播放
        VP_REPEAT_PLAY_MODE_ALL,        //全部曲目循环播放
        VP_REPEAT_PLAY_MODE_ONE,        //单个曲目循环播放
        VP_REPEAT_PLAY_MODE_MAX         //保留
} VP_REPEAT_PLAY_MODE_E;

#ifdef MMI_VIDEO_PLAYER_MINI_SUPPORT //6530_MINI
typedef enum
{
    VP_SHUFFLE_PLAY_MODE_OFF = 0,       //关闭乱序播放
    VP_SHUFFLE_PLAY_MODE_ON,            //打开乱序播放
    VP_SHUFFLE_PLAY_MODE_MAX            //保留
} VP_SHUFFLE_PLAY_MODE_E;
#endif
typedef enum
{
    VP_PLAY_END_NONE,
    VP_PLAY_END_END_SUCCESS,            //正常结束
        VP_PLAY_END_ERROR,              //发生错误结束
        VP_PLAY_END_NOT_SUPPORT,        //解码错误，格式不支持
        VP_PLAY_END_STOPPED_BY_USER,    //用户取消
        VP_PLAY_END_RESULT_MAX          //保留
} VP_PLAY_END_RESULT_E;

typedef enum
{
    VP_ENTRY_FROM_PLAYER,           //播放器正常播放
        VP_ENTRY_FROM_FILE,         //直接播放文件
        VP_ENTRY_FROM_NET,
        VP_ENTRY_FROM_MAX           //保留
} VP_ENTRY_E;


//本地视频播放方式类型
typedef enum
{
    VP_PLAYBACK_MODE_AUDIO_VIDEO_SYN = 0, //音视频同步优先
	VP_PLAYBACK_MODE_SCREEN_FLOW,        //画面流畅性优先
    VP_PLAYBACK_MODE_MAX         //保留
} VP_PLAYBACK_MODE_E;

typedef DPLAYER_DISPLAY_PARAM_T VP_DISPLAY_PARAM_T  ;       //显示参数
typedef uint32 VP_HANDLE;                               //Videoplayer control Init后返回给UI的handler

//回调函数的参数结构
typedef struct
{
    uint16                   msg_id; //消息ID
    uint32                  handle; //句柄（init时返回给UI的handle）    
    void*                   para;   //消息所带的参数
} VP_CALLBACK_T;

//播放结束
typedef struct
{
    VP_HANDLE           vp_handler; //Videoplayer controller handler
    VP_PLAY_END_RESULT_E  result;   //videoplayer end reason
} VP_PLAY_END_IND_T;

//buffer播放时的数据参数
typedef struct
{
    uint8*                  video_buf_ptr;  //视频BUFFER数据的指针
    uint32                  video_buf_len;  //视频BUFFER数据的长度
    MMICOM_VIDEO_TYPE_E         type;           //视频BUFFER数据的格式
} VP_PLAY_BUF_INFO_T;


//播放参数
typedef struct
{
    VP_PLAY_TYPE_E          play_type;          //播放方式
    VP_REPEAT_PLAY_MODE_E   mode;               //播放模式    
    MN_DUAL_SYS_E           dual_sys;           //sim卡
    wchar*                  full_path_ptr;      //播放文件的绝对路径(从本地文件播放时有效)
    uint16                  full_path_len;      //播放文件的绝对路径长度(从本地文件播放时有效)
    VP_PLAY_BUF_INFO_T      video_buf_info;     //视频BUFFER数据参数(从本地BUFFER播放时有效)    
    VP_DISPLAY_PARAM_T      display_param;      //播放显示参数    
    BOOLEAN                 is_scal_up;         //是否拉伸
    BOOLEAN                 is_save_history;    //是否保存历史记录
    BOOLEAN                 is_random_play ;    //是否开启随机播放
    char                    *url_ptr;           //网络播放的URL地址(从网络播放时有效)
    uint8                   vol;                 //音量
    BOOLEAN                 is_bt_support;
    BOOLEAN                 is_a2dp_support;
    BOOLEAN                 is_share_play;      //边下载边播放       
    BOOLEAN                 is_cr_video_layer;          //是否创建了图像层
    BOOLEAN			        is_audio_coexist;          
    //GUI_LCD_DEV_INFO        dev_layer_handle;           //图像层的handle
} VP_PLAY_PARAM_T;

//定位播放参数 seek_time和seek_frame两者设置一个即可。全'F'表示设置无效
typedef struct
{
    uint32       seek_time; //需要seek到的时间点
    uint32       seek_frame; //需要seek到的帧点
} VP_SEEK_PARAM_T;


typedef void (*VP_CALLBACK) (VP_CALLBACK_T *msg_ptr);       //状态机给ui发送消息的回调函数

//播放设置参数
typedef struct
{
    uint8                   brightness;                         //播放亮度   
    uint8                   net_index[MMI_DUAL_SYS_MAX];         //网络连接设置的index
    uint16                  lowest_port;  //RTP/RTCP最低端口
    uint16                  highest_port; //RTP/RTCP最高端口    
    uint8                   agent_ip[VP_MAX_IP_LENGTH+1];       //代理IP
    uint8                   agent_ip_length;
    uint16                  agent_port;   //代理端口
    BOOLEAN                 is_use_agent;                       //是否使用代理
    BOOLEAN                 is_random_play;                     //是否开启随机播放
    VP_REPEAT_PLAY_MODE_E   mode;                               //播放模式
	VP_PLAYBACK_MODE_E      playback_mode;                      //本地文件播放方式
} VP_NV_SETTING_T;
/*---------------------------------------------------------------------------*/
/*                          FUNCTION DECLARE                                 */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
//  如下宏定义用以实现对外接口更名前后的兼容
/*****************************************************************************/
#ifdef MMI_FUNC_COMPATIBLE_SUPPORT

#define VP_Init                 MMIAPIVP_FSMInit
#define VP_Release              MMIAPIVP_FSMRelease
#define VP_Play                 MMIAPIVP_FSMPlay
#define VP_Stop                 MMIAPIVP_FSMStop
#define VP_Pause                MMIAPIVP_FSMPause
#define VP_Resume               MMIAPIVP_FSMResume
#define VP_CompulsivePause      MMIAPIVP_FSMCompulsivePause
#define VP_CompulsiveResume     MMIAPIVP_FSMCompulsiveResume
#define VP_Seek                 MMIAPIVP_FSMSeek
#define VP_SwitchPlayDevice     MMIAPIVP_FSMSwitchPlayDevice
#define VP_SwitchDisplayMode    MMIAPIVP_FSMSwitchDisplayMode
#define VP_SwitchAudioSupport   MMIAPIVP_FSMSwitchAudioSupport
#define VP_GetState             MMIAPIVP_FSMGetState
#define VP_Exit                 MMIAPIVP_FSMExit
#define VP_GetTimeOffset        MMIAPIVP_FSMGetTimeOffset
#define VP_GetFilerInfo         MMIAPIVP_GetFilerInfo
#ifdef JAVA_SUPPORT_SUN         //norflashcut
#define VP_GetFrameOffset       MMIAPIVP_FSMGetFrameOffset
#endif
#define VP_UpdateCurrentVol     MMIAPIVP_FSMUpdateCurrentVol
#define VP_FSMUpdatePlayMode    MMIAPIVP_FSMUpdatePlayMode
#ifdef MOBILE_VIDEO_SUPPORT     //norflashcut
#define VP_FSMGetCurBufPercent  MMIAPIVP_FSMGetCurBufPercent
#endif
#define MMIVP_TranslateDisplayParamLogicToPhysical  MMIAPIVP_TranslateDisplayParamLogicToPhysical
#define MMIVP_GetDefaultFolderPath                  MMIAPIVP_GetDefaultFolderPath
#define MMIVP_GetTransparenceColor                  MMIAPIVP_GetTransparenceColor
#define MMIVP_FillRectWithTransparenceColor         MMIAPIVP_FillRectWithTransparenceColor

#ifdef VIDEOTHUMBNAIL_SUPPORT
#define VP_ThumbnailOpen            MMIAPIVP_ThumbnailOpen
#define VP_ThumbnailClose           MMIAPIVP_ThumbnailClose
#define VP_ThumbnailGetVideo        MMIAPIVP_ThumbnailGetVideo
#define VP_ThumbnailGetAngle        MMIAPIVP_ThumbnailGetAngle
#define VP_IsThumbnailRunning       MMIAPIVP_IsThumbnailRunning
#define VP_IsThumbnailOpen          MMIAPIVP_IsThumbnailOpen
#define VP_ThumbnailResume          MMIAPIVP_ThumbnailResume
#define VP_ThumbnailVideoAdd        MMIAPIVP_ThumbnailVideoAdd
#define VP_ThumbnailStop            MMIAPIVP_ThumbnailStop
#define VP_ThumbnailSetAngle        MMIAPIVP_ThumbnailSetAngle
#define VP_ThumbnailSetDPLAYERParam MMIAPIVP_ThumbnailSetDPLAYERParam
#endif

#endif //MMI_FUNC_COMPATIBLE_SUPPORT

/*****************************************************************************/
//  Description : 创建videoplayer状态机
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC  VP_HANDLE  MMIAPIVP_FSMInit(VP_CALLBACK vpcallback);

/*****************************************************************************/
//  Description : 去活PDP；销毁videoplayer状态机
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIVP_FSMRelease(VP_HANDLE   vp_handle);

/*****************************************************************************/
//  Description : 发起视频播放（本地或网络）
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIVP_FSMPlay(VP_HANDLE   vp_handle, VP_PLAY_PARAM_T  *play_param_ptr);

/*****************************************************************************/
//  Description : 终止视频播放（本地或网络）。
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIVP_FSMStop(VP_HANDLE   vp_handle);

/*****************************************************************************/
//  Description : 暂停视频播放（本地或网络）。
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIVP_FSMPause(VP_HANDLE   vp_handle);

/*****************************************************************************/
//  Description : 恢复视频播放（本地或网络）
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIVP_FSMResume(VP_HANDLE  vp_handle);

/*****************************************************************************/
//  Description : 强制暂停
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 满足lose focus暂停的需求。
/*****************************************************************************/
PUBLIC void MMIAPIVP_FSMCompulsivePause(VP_HANDLE  vp_handle);

/*****************************************************************************/
//  Description : 强制恢复
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 满足get focus恢复的需求。
/*****************************************************************************/
PUBLIC void MMIAPIVP_FSMCompulsiveResume(VP_HANDLE  vp_handle);

/*****************************************************************************/
//  Description : 进行定位视频播放（本地或网络）。
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIVP_FSMSeek(VP_HANDLE   vp_handle,
                        VP_SEEK_PARAM_T  *seek_param_ptr);

/*****************************************************************************/
//  Description : 切换播放设备
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIVP_FSMSwitchPlayDevice(VP_HANDLE  vp_handle,
                                   BOOLEAN    is_a2dp_support,
                                   BOOLEAN    is_bt_support);

/*****************************************************************************/
//  Description : 切换显示模式（全屏，半屏，TVOUT)
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIVP_FSMSwitchDisplayMode(VP_HANDLE  vp_handle,
                                    VP_DISPLAY_PARAM_T  *display_param_ptr,
                                    BOOLEAN is_only_rect);

/*****************************************************************************/
//  Description : 切换声音模式 
//  Global resource dependence :  none
//  Author:  
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIVP_FSMSwitchAudioSupport(VP_HANDLE  vp_handle,
                                    BOOLEAN is_support);

/*****************************************************************************/
//  Description : 获取当前播放状态
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC VP_STATE_E MMIAPIVP_FSMGetState(VP_HANDLE    vp_handle);

/*****************************************************************************/
//  Description : 执行退出流程，并销毁状态机。
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 满足一键退出的需求。
/*****************************************************************************/
PUBLIC void MMIAPIVP_FSMExit(VP_HANDLE  vp_handle);

/*****************************************************************************/
//  Description : 获得视频播放的当前时间
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMIAPIVP_FSMGetTimeOffset(VP_HANDLE   vp_handle);

/*****************************************************************************/
//  Description : 恢复初始设置
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIVP_ResetSettingInfo(void);


#ifdef JAVA_SUPPORT_SUN//norflashcut
/*****************************************************************************/
//  Description : 获得视频播放的当前帧数。
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMIAPIVP_FSMGetFrameOffset(VP_HANDLE  vp_handle);
#endif

/*****************************************************************************/
//  Description : 播放视频文件
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIVP_Play (
                                VP_PLAY_PARAM_T  *play_param_ptr,
                                VP_CALLBACK        call_back_func,   
                                uint32                    start_time
                                );

/*****************************************************************************/
//  Description : 退出播放视频文件
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIVP_ExitPlay(void);

/*****************************************************************************/
//  Description : 通过全路径名获得媒体文件信息
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIVP_GetInfoFromFullPathName(
                                            const wchar            *full_file_name, 
                                            uint16                  full_name_len,
                                            DPLAYER_MEDIA_INFO_T    *media_ptr
                                            );

/*****************************************************************************/
//  Description : 通过视频buf中获得媒体文件信息
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIVP_GetMediaInfoFromBuf(
                                        uint8                   *video_buf,
                                        uint32                  video_length,
                                        MMICOM_VIDEO_TYPE_E         video_type,
                                        DPLAYER_MEDIA_INFO_T   *media_ptr
                                        );

/*****************************************************************************/
//  Description : 用于判断视频播放器对该视频文件的支持程度
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC VP_SUPPORTED_STATUS_TYPE_E MMIAPIVP_GetSupportedStatus(
                                                        wchar   *full_file_name, 
                                                        uint16    full_name_len
                                                        );

/*****************************************************************************/
//  Description : init vp module
//  Global resource dependence : 
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIVP_InitModule(void);
/*****************************************************************************/
//  Description : init vp
//  Global resource dependence : none
//  Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIVP_Init(void);
/*****************************************************************************/
//  Description : 进入视频播放器
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIVP_MainEntry(void);

/*****************************************************************************/
//  Description : 退出视频播放器
//  Global resource dependence : 
//  Author: 
/*****************************************************************************/
PUBLIC void MMIAPIVP_ExitVideoPlayer(void);

/*****************************************************************************/
//  Description : 播放视频文件
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIVP_PlayVideoFromVideoPlayer(VP_ENTRY_E vp_entry,
                                  wchar *full_path_name_ptr,
                                  uint16 full_path_name_len,
                                  uint8 *url_ptr
                                  );
/*****************************************************************************/
//  Description : 播放视频文件
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIVP_PlayVideoFromVideoPlayerEx(VP_ENTRY_E vp_entry,
                                    wchar* full_path_name_ptr,
                                  uint16 full_path_name_len,
                                  uint8 *url_ptr,
                                  uint32 progress_time, //从某一个时间点开始播放
                                  VP_POST_PROGRESS post_progrecss  //后处理接口 
                                  );

/*****************************************************************************/
//  Description : MMIAPIVP_IsOpenPlayerWin
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIVP_IsOpenPlayerWin(void);

#if 0//norflashcut
/*****************************************************************************/
//  Description : MMIAPIVP_IsFocusPlayerWin
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIVP_IsFocusPlayerWin(void);
#endif

/*****************************************************************************/
//  Description : notify vp stop
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIVP_NotifyVPStop(void);

#if 0//norflashcut
/*****************************************************************************/
//  Description : MMIAPIVP_HandleFileChange
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIVP_HandleFileChange(void);
#endif

//add CR233033
/*****************************************************************************/
//  Description : get full path name of playing file
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIVP_GetPlayingFilename(wchar * full_path_name_ptr);

#ifdef JAVA_SUPPORT_SUN//norflashcut
/*****************************************************************************/
//  Description : register current task for playing video
//  Global resource dependence : 
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIVP_RegTaskID(VP_HANDLE vp_handle, uint32 task_id);
/*****************************************************************************/
//  Description : unregister current task for playing video
//  Global resource dependence : 
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIVP_UnRegTaskID(VP_HANDLE vp_handle, uint32 task_id);
#endif


/*****************************************************************************/
//  Description : MMIAPIVP_IsLiving
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIVP_IsLiving(void);

/*****************************************************************************/
//  Description : MMIAPIVP_IsLiving
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIVP_IsPlayWinFocus(void);

/*****************************************************************************/
//  Description : MMIAPIVP_FSMUpdateCurrentVol
//  Global resource dependence : 
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIVP_FSMUpdateCurrentVol(VP_HANDLE vp_handle, uint8 vol);

/*****************************************************************************/
//  Description : MMIAPIVP_FSMUpdatePlayMode
//  Global resource dependence : 
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIVP_FSMUpdatePlayMode(VP_HANDLE vp_handle, 
                              VP_REPEAT_PLAY_MODE_E mode,
                              BOOLEAN is_random_play
                              );

/*****************************************************************************/
//  Description : MMIAPIVP_FSMUpdatePlayBackMode
//  Global resource dependence : 
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIVP_FSMUpdatePlayBackMode(VP_HANDLE vp_handle, 
                              VP_PLAYBACK_MODE_E mode
                              );


#ifdef MOBILE_VIDEO_SUPPORT//norflashcut
/*****************************************************************************/
//  Description : MMIAPIVP_FSMGetCurBufPercent
//  Global resource dependence : 
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMIAPIVP_FSMGetCurBufPercent(VP_HANDLE vp_handle);
#endif

#ifdef MMI_STREAMING_PLAYER_SUPPORT//norflashcut
/*****************************************************************************/
//  Description : 用于将设置信息保存到nv中
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIVP_SetNVSettingInfo(VP_NV_SETTING_T nv_setting_info);

/*****************************************************************************/
//  Description : 从nv中获取设置信息
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC VP_NV_SETTING_T MMIAPIVP_GetNVSettingInfo(void);

/*****************************************************************************/
//  Description : get  vp net win id
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC MMI_WIN_ID_T MMIAPIVP_GetVPNetWinID(void);

#endif
#ifdef UI_MULTILAYER_SUPPORT
/*****************************************************************************/
//  Description : CreateVideoLayer
//  Global resource dependence : 
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIVP_CreateVideoLayer(void);
#endif
/*****************************************************************************/
//  Description : destroy video layer
//  Global resource dependence : 
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIVP_DestroyVideoLayer(void);

/*****************************************************************************/
//  Description : 禁止或恢复音频
//  Global resource dependence : 
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIVP_CurrentVideoEnableAudio(BOOLEAN is_enable);

/*****************************************************************************/
//  Description : switch current video
//  Global resource dependence : 
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIVP_CurrentVideoLcdSwitch(void);

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIVP_SetBgVideoAudioSupport(BOOLEAN is_audio_support);


/*****************************************************************************/
//  Description : 暂停视频播放
//  Global resource dependence :  none
//  Author: aoke.hu
//  Note: vp_handle为0时暂停当前播放的视频
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIVP_Pause(VP_HANDLE vp_handle);

/*****************************************************************************/
//  Description : 恢复视频播放
//  Global resource dependence :  none
//  Author: aoke.hu
//  Note: vp_handle为0时恢复当前播放视频文件
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIVP_Resume(VP_HANDLE vp_handle);

/*****************************************************************************/
//  Description : 把展示参数从逻辑转到物理
//  Global resource dependence : 
//  Author: fuyanzhang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIVP_TranslateDisplayParamLogicToPhysical(VP_DISPLAY_PARAM_T *disp_para, BOOLEAN is_need_fill);

/*****************************************************************************/
//  Description : 获取默认存放视频文件的文件夹路径
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIVP_GetDefaultFolderPath(
                                   wchar          *full_path_name_ptr,
                                   uint16          *full_path_len_ptr
                                   );

/*****************************************************************************/
//  Description : 获取透明色
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
PUBLIC GUI_COLOR_T MMIAPIVP_GetTransparenceColor(void);

/*****************************************************************************/
//  Description : 填充透明色
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIVP_FillRectWithTransparenceColor(
                                           uint32 *width_ptr, //[IN/OUT]
                                           uint32 *height_ptr, //[IN/OUT]
                                           uint32 *x_offset_ptr, //[IN/OUT]
                                           uint32 * y_offset_ptr, //[IN/OUT]
                                           uint32 RotateAngle,
                                           BOOLEAN is_need_fill                                  
                                           );

/*****************************************************************************/
//  Description : 获取筛选视频文件的筛选参数
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIVP_GetFilerInfo(MMIFMM_FILTER_T * filter);


 #ifdef  VIDEOTHUMBNAIL_SUPPORT  
/*****************************************************************************/
//  Description : VP_ThumbnailOpenDPLAYER , 开启缩略图获取功能 ，打开成功s_thumbnail_info.is_open置为true
//  Global resource dependence : 
//  Author:  
//  Note: 
/*****************************************************************************/  
PUBLIC BOOLEAN MMIAPIVP_ThumbnailOpen(
                                      wchar          *full_path_name_ptr,    
                                      uint16            full_path_len,      
                                      uint32            seektime,      //所要获取的缩略图时间点
                                      BOOLEAN  (*vp_callback)(DPLAYER_IMAGE_DATA_T *image_data),//ui的回调函数 
                                      DPLAYER_DISPLAY_PARAM_T *display_param,
                                      BOOLEAN   is_videoqueue) ;//是否为视频队列
/*****************************************************************************/
//  Description : MMIAPIVP_ThumbnailOpenByHandle , 开启缩略图获取功能 ，打开成功s_thumbnail_info.is_open置为true
//  Global resource dependence : 
//  Author:  
//  Note: 
/*****************************************************************************/  
PUBLIC BOOLEAN MMIAPIVP_ThumbnailOpenByHandle(
                                      VP_HANDLE  vphandle,
                                      uint32            seektime,      //in所要获取缩略图的本地视频文件播放时间点
                                      BOOLEAN  (*vp_callback)(DPLAYER_IMAGE_DATA_T *image_data),//in,是否为视频列表连续获取
                                      DPLAYER_DISPLAY_PARAM_T *display_param);//in,DPLAYER播放参数                                       
/*****************************************************************************/
//  Description : 关闭缩略图获取功能 ，停止dplyer，重置状态，如果是视频队列，则释放列表分配资源
//  Global resource dependence : 
//  Author:  
//  Note: 
/*****************************************************************************/
PUBLIC  void  MMIAPIVP_ThumbnailClose(void);
/*****************************************************************************/
//  Description : MMIAPIVP_ThumbnailGetVideo
//  Global resource dependence : 
//  Author:  
//  Note: 
/*****************************************************************************/
PUBLIC  wchar*  MMIAPIVP_ThumbnailGetVideo(void);
/*****************************************************************************/
//  Description : 获取  当前lcd 角度，避免横竖屏快速切换导致获取的缩略图异步出错
//  Global resource dependence : 
//  Author:  
//  Note: 
/*****************************************************************************/
PUBLIC LCD_ANGLE_E  MMIAPIVP_ThumbnailGetAngle(void);
/*****************************************************************************/
//  Description :     是否 正在进行一个视频缩略图获取  ，即DPLAYER_Play  正在播放一个视频 
//  Global resource dependence : 
//  Author:  
//  Note: 
/*****************************************************************************/
PUBLIC  BOOLEAN  MMIAPIVP_IsThumbnailRunning(void);

/*****************************************************************************/
//  Description :缩略图获取是否已经被打开,MMIAPIVP_ThumbnailOpen 调用成功后为true,VP_ThumbnailClose后为false
//  Global resource dependence : 
//  Author:  
//  Note: 
/*****************************************************************************/
PUBLIC  BOOLEAN  MMIAPIVP_IsThumbnailOpen(void);

/*****************************************************************************/
//  Description : 视频队列连续获取时，恢复 视频队列连续获取
//  Global resource dependence : 
//  Author:  
//  Note: 
/*****************************************************************************/  
PUBLIC BOOLEAN MMIAPIVP_ThumbnailResume(void) ;
/*****************************************************************************/
//  Description :  视频队列连续获取时，向列表尾中添加一个视频
//  Global resource dependence : 
//  Author:  
//  Note: 
/*****************************************************************************/
PUBLIC  BOOLEAN  MMIAPIVP_ThumbnailVideoAdd(wchar  *video_fullpath_name);
/*****************************************************************************/
//  Description : 视频队列连续获取时，暂时打断  视频队列连续获取，通过VP_ThumbnailResume 恢复 
//  Global resource dependence : 
//  Author:  
//  Note: 
/*****************************************************************************/  
PUBLIC BOOLEAN MMIAPIVP_ThumbnailStop(void);
/*****************************************************************************/
//  Description : 获取  当前视频队列中待解析的视频个数
//  Global resource dependence : 
//  Author:  
//  Note: 
/*****************************************************************************/
PUBLIC uint8 MMIAPIVP_ThumbnailGetVideoQueueNum(void);

/*****************************************************************************/
//  Description : 设置当前lcd 角度，避免横竖屏快速切换导致获取的缩略图异步出错
//  Global resource dependence : 
//  Author:  
//  Note: 
/*****************************************************************************/
PUBLIC void  MMIAPIVP_ThumbnailSetAngle(LCD_ANGLE_E lcd_angle);
/*****************************************************************************/
//  Description : 设置DPLAYER 播放参数
//  Global resource dependence : 
//  Author:  
//  Note: 
/*****************************************************************************/
PUBLIC void  MMIAPIVP_ThumbnailSetDPLAYERParam(DPLAYER_DISPLAY_PARAM_T *display_param);

#endif
/*****************************************************************************/
//  Description :设置VPWin支持所有角度窗口显示 
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/

PUBLIC void MMIAPIVP_SetVPWinSupportAllAngle(void);

/*****************************************************************************/
//  Description : is VP playing
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIVP_IsVPPlaying(void);

#ifdef MMI_VIDEOPLAYER_MINI_FUNCTION
/*****************************************************************************/
//  Description : 播放视频文件
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIVP_MiniFunction_PlayVideo(wchar *full_path_name_ptr,
                                                                                            uint16 full_path_name_len);
/*****************************************************************************/
//  Description : 播放视频文件
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIVP_MiniFunction_ExitVideoPlayer(void);

/*****************************************************************************/
//  Description : post message to the main win 
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIVP_SDCardPlugOut(void);

#endif
/*****************************************************************************/
//  Description : 显示OSD
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIVP_ResetSetSeekTime(void);

#ifdef MMI_STREAMING_PLAYER_SUPPORT
/*****************************************************************************/
//  Description : MMIAPIVP_IsSetSpecialPause
//  Global resource dependence : s_is_special_pause
//  Author: yanyan.an
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIVP_IsSetSpecialPause(BOOLEAN is_set);
#endif


#ifdef DC_WATCH_UI_SUPPORT

/*****************************************************************************/
//  Description: get vp handle for video play
//  Parameter: [In] None
//             [Out] None
//             [Return] vp_handle: uint32, vp handle.
//  Author: fulu.song
//  Note: API to operate vp fsm from other module without vp ui.
/*****************************************************************************/
PUBLIC uint32 WatchVP_GetHandle_ForDv(void);

/*****************************************************************************/
//  Description: init vp for video play
//  Parameter: [In] VP_CALLBACK: function callback for caller to handle vp msg.
//             [Out] None
//             [Return] vp_handle: uint32, vp handle.
//  Author: fulu.song
//  Note: API to operate vp fsm from other module without vp ui.
/*****************************************************************************/
PUBLIC uint32 WatchVP_Init_ForDv(VP_CALLBACK vpcallback);

/*****************************************************************************/
//  Description: start play for video play
//  Parameter: [In] filepath: video file path,  fileLen: file length.
//             [Out] None
//             [Return] BOOLEAN: execution result. TRUE if OK.
//  Author: fulu.song
//  Note: API to operate vp fsm from other module without vp ui.
/*****************************************************************************/
PUBLIC BOOLEAN WatchVP_Play_ForDv(const wchar *filePath, uint32 fileLen);

/*****************************************************************************/
//  Description: pause play for video play
//  Parameter: [In] None
//             [Out] None
//             [Return] BOOLEAN: execution result. TRUE if OK.
//  Author: fulu.song
//  Note: API to operate vp fsm from other module without vp ui.
/*****************************************************************************/
PUBLIC BOOLEAN WatchVP_Pause_ForDv(void);

/*****************************************************************************/
//  Description: resume play for video play
//  Parameter: [In] None
//             [Out] None
//             [Return] BOOLEAN: execution result. TRUE if OK.
//  Author: fulu.song
//  Note: API to operate vp fsm from other module without vp ui.
/*****************************************************************************/
PUBLIC BOOLEAN WatchVP_Resume_ForDv(void);

/*****************************************************************************/
//  Description: stop play for video play
//  Parameter: [In] None
//             [Out] None
//             [Return] BOOLEAN, stop result. TRUE if OK.
//  Author: fulu.song
//  Note: API to operate vp fsm from other module without vp ui. 
/*****************************************************************************/
PUBLIC BOOLEAN WatchVP_Stop_ForDv(void);

#endif

/**--------------------------------------------------------------------------*
**                         Compiler Flag                                    *
**--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
    
#endif


