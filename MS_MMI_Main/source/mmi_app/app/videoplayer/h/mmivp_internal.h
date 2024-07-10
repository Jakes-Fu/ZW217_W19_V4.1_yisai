/*****************************************************************************
** File Name:      mmivp_internal.h                                                   *
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

#ifndef _MMIVP_INTERNAL_H_
#define _MMIVP_INTERNAL_H_

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "mmivp_export.h"
#include "mmifmm_export.h"
#include "mmivp_locallist.h"
#include "mmicom_panel.h"

#include "mmisrvaud_api.h"
#include "mmisrvmgr.h"
#ifdef KING_MOVIE_SUPPORT
#include "video_security.h"
#endif
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
#define VP_STRACE_LOW  SCI_TRACE_LOW
#ifdef AVI_DEC_SUPPORT
#define VP_IS_SUPPORT_AVI TRUE
#else
#define VP_IS_SUPPORT_AVI FALSE
#endif

#ifdef FLV_DEC_SUPPORT
#define VP_IS_FLV_SUPPORT            TRUE
#else
#define VP_IS_FLV_SUPPORT            FALSE
#endif

#ifdef MMI_RMVB_SUPPORT
#define VP_IS_RMVB_SUPPORT            TRUE
#else
#define VP_IS_RMVB_SUPPORT            FALSE
#endif

#define VP_MAX_PORT_LENGTH          5                        //端口的最大长度

#ifdef MMI_VIDEO_PLAYER_MINI_SUPPORT //6530_MINI
#define VP_MAX_URL_LEN              1                      //url的最大长度
#else
#define VP_MAX_URL_LEN              1024                      //url的最大长度
#endif
#define VP_MAX_NAME_LEN             MMIFMM_FILE_FILE_NAME_MAX_LEN    //名称的最大长度
#define VP_AUTH_MAX_PASSWORD_LEN    20 
#define VP_AUTH_MAX_NAME_LEN        512

#define VP_DEFAULT_VOLUME           3            //默认播放音量
#define VP_DEFAULT_BRIGHTNESS       MMISET_EDEFAULT_CONTRAST            //默认的亮度
#define VP_DEFAULT_REPEAT_MODE      0            //默认关闭循环模式
#define VP_DEFAULT_RANDOM_PLAY      FALSE        //默认关闭随机播放
#define VP_DEFAULT_NEXT_INDEX       0            //默认的网络索引
    
#define VP_BOOKMARK_MAX_ITEM        50                         //允许的书签的最大个数

#define VP_MAX_INVALID_VALUE              0xffffffff      //无效最大值    

#define VP_SEC_TO_MSEL         1000        //1s = 1000ms

#define  HTTP_URL_HEAD_ASIC "http://"
#define  HTTP_URL_HEAD_LEN 7
#define  RTSP_URL_HEAD_ASIC "rtsp://"
#define RTSP_URL_HEAD_LEN 7
/**--------------------------------------------------------------------------*
**                         TYPE AND CONSTANT                                *
**--------------------------------------------------------------------------*/

//按钮的处理函数
typedef void (*VP_BUTTON_CALL_BACK)(BOOLEAN is_open);


//错误类型
typedef enum
{
        VP_ERROR_NONE = 0,                      //无错误
        VP_ERROR_NULL_FILE,                 //空文件
        VP_ERROR_NOT_SUPPORT,               //格式不支持
        VP_ERROR_INTERNAL_SERVER_ERROR,     //服务器内部错误
        VP_ERROR_BAD_REQUEST,               //错误的请求
        VP_ERROR_UNAUTHORIZED,              //认证失败
        VP_ERROR_NOT_FOUND,                 //未找到媒体
        VP_ERROR_PDP_ACTIVE_ERROR,          //Pdp激活失败
        VP_ERROR_PDP_DEACTIVE_IND,          //PDP被服务器断开
        VP_ERROR_SERVER_CLOSE_IND,          //服务器主动关闭连接
        VP_ERROR_STRM_INIT_ERROR,           //Streaming层初始化出错
        VP_ERROR_PAUSE_ERROR,               //暂停播放出错
        VP_ERROR_RESUME_ERROR,              //恢复播放出错
        VP_ERROR_SEEK_ERROR,                //定位播放出错
        VP_ERROR_DATA_TIMEOUT,              //streaming等待数据超时
        VP_ERROR_CONNECT_TIMEOUT,
        VP_ERROR_DRM_RIGHT_INVALID,         //drm版权无效
        VP_ERROR_DRM_NO_SYS_NITZ,         //drm版权无效
        VP_ERROR_OTHERS,                    //其它错误
        VP_ERROR_MAX                        //保留
} VP_ERROR_E; 

//操作列表出错原因
typedef enum
{
    VP_LIST_ERROR_NONE = 0,         //成功
        VP_LIST_ERROR_TOO_MUCH,     //超出限制（如添加的太多）
        VP_LIST_ERROR_NO_SPACE,     //没有空间存放
        VP_LIST_ERROR_OTHER,        //其他错误
        VP_PLAY_MODE_MAX            //保留
} VP_LIST_ERROR_E;

//按钮的状态
typedef enum
{
    VP_BUTTON_NONE,             //不显示
        VP_BUTTON_NORMAL,       //正常显示
        VP_BUTTON_FOCUS,        //获得焦点
        VP_BUTTON_GREY,         //灰化显示
        VP_BUTTON_STATE_MAX     //保留
}VP_BUTTON_DISPLAY_MODE_E;

//按钮类型
typedef enum
{
    VP_BUTTON_FILE_ICON,            //文件选择icon
        VP_BUTTON_REPEAT_MODE,      //循环播放选择icon
        VP_BUTTON_RANDOM_MODE,      //随机播放选择icon
        VP_BUTTON_PROCESS,          //进度条        
        VP_BUTTON_VOLUME,           //音量icon
        VP_BUTTON_BACKWARD,         //后退/下一首icon
        VP_BUTTON_PAUSE_PLAY,       //播放/暂停 icon
        VP_BUTTON_FORWARD,          //前进/上一首icon
        VP_BUTTON_SWITCH_SCREEN,    //切换屏幕icon      
        VP_BUTTON_CONTROL_PANEL_INC, //控制面板的inc icon
        VP_BUTTON_CONTROL_PANEL_DEC, //控制面板的dec icon
        VP_BUTTON_CONTROL_PANEL_CLOSE, //控制面板的close icon
        VP_BUTTON_MAX               //保留
}VP_BUTTON_TYPE_E;


//按钮信息
typedef struct 
{    
    GUI_RECT_T                      rect;       //按钮区域
    VP_BUTTON_DISPLAY_MODE_E        mode;       //显示模式
    BOOLEAN                         is_tp_down;
    MMI_MESSAGE_ID_E                keydown_msg;   //对应的keydown消息
    MMI_MESSAGE_ID_E                keyup_msg;   //对应的keyup消息
    MMI_MESSAGE_ID_E                keylong_msg; //对应的长按消息；
    MMI_MESSAGE_ID_E                keylongup_msg;//对应的长按结束消息
    MMI_IMAGE_ID_T                  image_id;   //按钮图片id
}BUTTON_INFO_T;

typedef enum
{
    VP_TIMER_NONE_TYPE,

    VP_TIMER_VOL_INC_TYPE,
    VP_TIMER_VOL_DEC_TYPE,

    VP_TIMER_BRIGHT_INC_TYPE,
    VP_TIMER_BRIGHT_DEC_TYPE,
    VP_TIMER_FORWARD_TYPE,
    VP_TIMER_BACKWARD_TYPE,   
 
}VP_TIMER_TYPE_E;

typedef struct  
{
    VP_TIMER_TYPE_E  timer_type;
    uint8          timer_id;
}VP_TIMER_T;

//鉴权信息
typedef struct
{
    char                *username_ptr;      //用户名
    char                *password_ptr;      //密码
} VP_AUTH_INFO_T;

//流媒体网址信息
typedef struct
{
    wchar       title_arr[VP_MAX_NAME_LEN+1];       //流媒体网址名称
    uint32      title_len;                          //流媒体网址名称长度
    char        url_ptr[VP_MAX_URL_LEN+1];            //流媒体网址
} VP_NET_MOVIE_INFO_T;


//本地文件信息
typedef struct
{
    wchar           full_path_ptr[VP_MAX_NAME_LEN +1];  //文件的绝对路径
    uint16          full_path_len;                      //文件的绝对路径长度
    uint32          create_time;                        //文件创建时间
    uint32          width;                              //视频图像宽度
    uint32          height;                             //视频图像高度
    uint32          media_length;                       //视频持续时长
    uint32          file_size;                          //视频文件大小
} VP_LOCAL_MOVIE_FILE_INFO_T;

//播放连接准备完毕。UI层可依据返回参数显示片源名，时长等信息。
typedef struct
{
    VP_HANDLE                   vp_handler;     //Videoplayer controller handler
    uint32                      start_time;     //播放开始时间
    uint32                      total_time;     //片源总时间（全F代表直播）
    wchar*                      file_name_ptr;  //片源名称,如果是本地文件，应该是全路径
    uint16                      file_name_len;  //片源名称长度
    VP_PLAY_TYPE_E              play_type;      //播放类型
    VP_SUPPORTED_STATUS_TYPE_E  support_type;   //支持的程度
} VP_PREPARE_IND_T;

//报告当前的播放进度（每播放一帧都会报该消息）
typedef struct
{
    VP_HANDLE               vp_handler;     //Videoplayer controller handler
    uint32                  process_time;   //当前播放的时间
    uint32                  total_time;     //片源总时间（全F代表直播）
} VP_PLAY_PROCESS_IND_T;

//报告当前的缓冲进度（每播放一帧都会报该消息）
typedef struct
{
    VP_HANDLE           vp_handler;     //Videoplayer controller handler
    uint32              buffer_percent;
} VP_BUFFERING_PROCESS_IND_T;

//缓冲结束。
typedef struct
{
    VP_HANDLE           vp_handler;     //Videoplayer controller handler
} VP_BUFFERING_IND_T;

//缓冲结束。
typedef struct
{
    VP_HANDLE           vp_handler;     //Videoplayer controller handler
} VP_BUFFER_END_IND_T;

//播放过程中出现致命错误。
typedef struct
{
    VP_HANDLE           vp_handler; //Videoplayer controller handler
    VP_ERROR_E          error_code; //播放错误类型
} VP_FATAL_ERROR_IND_T;

//需要授权认证
typedef struct
{
    VP_HANDLE           vp_handler; //Videoplayer controller handler
}VP_AUTH_IND_T;



//播放已停止
typedef struct
{
    VP_HANDLE           vp_handler; //Videoplayer controller handler 
} VP_STOP_IND_T;

typedef struct
{
    VP_HANDLE           vp_handler; //Videoplayer controller handler
    VP_ERROR_E          error_code; //播放错误类型
} VP_PAUSE_CNF_T;

typedef struct
{
    VP_HANDLE           vp_handler; //Videoplayer controller handler
    VP_ERROR_E          error_code; //播放错误类型
} VP_PLAY_CNF_T;

typedef struct
{
    VP_HANDLE           vp_handler; //Videoplayer controller handler
    VP_ERROR_E          error_code; //播放错误类型
    uint32              cur_time;   //当前播放时间
    uint32              total_time; //总的播放时间
} VP_SEEK_CNF_T;

typedef enum
{
    VP_TO_DO_NONE,
    VP_TO_DO_PLAY_FOR_STOP,
    VP_TO_DO_PLAY_FOR_FULL_PAINT,
    VP_TO_DO_PLAY_FOR_LOAD_FILE,
    VP_TO_DO_SEEK,
    VP_TO_DO_STOP,
    VP_TO_DO_MAX
}VP_TO_DO_TYPE_E;

#ifndef MMI_VIDEOPLAYER_MINI_FUNCTION
//视频播放器信息
typedef struct  
{
    VP_HANDLE           vp_handle;                              //状态机handle
    VP_ENTRY_E          vp_entry;                               //进入播放器的方式类型
    VP_STATE_E          state;                                  //播放器ui状态
    VP_PLAY_TYPE_E      play_type;                              //播放类型  
    uint32              total_time;                             //总时间
    uint32              process_time;                           //播放进度  
    uint32              buffer_percent;                         //缓冲比例
    VP_SUPPORTED_STATUS_TYPE_E support_type;                    //支持的程度   
    VP_TO_DO_TYPE_E     to_do_type;                             //需要做的事情的类型
    uint32              seek_time;                              //    
    uint8               display_state_timer_id;                 //状态显示timer id
    uint8               url[VP_MAX_URL_LEN +1];                 //网络播放的url
    wchar               full_path_name[VP_MAX_NAME_LEN +1];     //本地播放时为全路径，网络播放时为片源名称
    uint16              full_path_len;                          //名称长度    
    LCD_ANGLE_E         cur_angle;                             //当前视频窗口角度
    BOOLEAN             is_display_win;                         //全屏状态下是否显示控制面板
#ifdef KING_MOVIE_SUPPORT
    BOOLEAN             is_kmv_file;
#endif
    VP_POST_PROGRESS    post_progrecss;//文件的后处理操作 
    uint8               display_win_timer_id;                   //全屏状态下控制面板显示timer
    VP_TIMER_T          timer;                                  //控制timer_id
    MMI_TEXT_ID_T       station_text_id;                        //状态显示
    MN_DUAL_SYS_E       dual_sys;
    VP_BT_STATE_E       bt_state;                                //蓝牙状态
	BOOLEAN             is_adjust_bright;
    //GUI_LCD_DEV_INFO    dev_layer_handle;                        //video层的handle
    MMISRV_HANDLE_T     srv_handle;
}VP_INTERFACE_PLAYING_INFO_T;

//历史记录信息
typedef struct VP_HISTORY_ITEM_tag
{
    VP_PLAY_TYPE_E                      type;                       //记录中视频播放方式类型（网络，本地）
    VP_NET_MOVIE_INFO_T                 *net_movie_info_ptr;        //流媒体网址信息
    VP_LOCAL_MOVIE_FILE_INFO_T          *local_movie_file_info_ptr; //本地网络信息
    uint32                              last_play_point;            //该记录上一次播放时的中断处（直播为0）
    uint32                              total_time;                 //总时间
    int32                               locallist_index;
    VP_LOCALLIST_TYPE_E                 locallist_type;
    struct VP_HISTORY_ITEM_tag          *next_item_ptr;             //下一个历史记录
} VP_HISTORY_ITEM_T;

#ifndef MMI_GUI_STYLE_TYPICAL
typedef enum
{
#ifdef MMI_PDA_SUPPORT
    MMIVP_PDA_SETTINGS_REPEAT,
    MMIVP_PDA_SETTINGS_RANDOM,
#endif    
	MMIVP_PDA_SETTINGS_PLAY_METHOD,
    MMIVP_PDA_SETTINGS_NET1,
    MMIVP_PDA_SETTINGS_NET2,
    MMIVP_PDA_SETTINGS_NET3,
    MMIVP_PDA_SETTINGS_NET4,
    MMIVP_PDA_SETTINGS_RTCP_RTP,
    MMIVP_PDA_SETTINGS_AGENT,
    MMIVP_PDA_SETTINGS_FLOWRATE,
    MMIVP_PDA_SETTINGS_RESET,
#if defined MMI_VIDEOPLAYER_HELP_SUPPORT 
    MMIVP_PDA_SETTINGS_HELP,
#endif
    
    MMIVP_PDA_SETTINGS_MAX
}MMIVP_PDA_SETTINGS_TYPE_E;

typedef enum
{
    VP_RANDOM_PLAY_MODE_OPEN,
    VP_RANDOM_PLAY_MODE_CLOSE,

    VP_RANDOM_PLAY_MODE_MAX
}VP_RANDOM_PLAY_MODE_E;

#endif //MMI_PDA_SUPPORT

 #ifdef  VIDEOTHUMBNAIL_SUPPORT  
#define THUMBNAIL_QUEUENUM  7  //视频队列的大小 

typedef BOOLEAN  (*THUMBNAIL_CALLBACK)(DPLAYER_IMAGE_DATA_T *image_data); 
typedef BOOLEAN  (*STOPCNF_CALLBACK)(void); 
typedef struct
{
    wchar  thumbnail_name [THUMBNAIL_QUEUENUM][MMIFMM_FULL_FILENAME_LEN+1];
    uint8    videonum;
}VP_THUMBNAIL_VIDEOQUEUE;

// 缩略图信息结构体 
typedef struct  
{
    wchar                        full_path_ptr[MMIFMM_FILE_FILE_NAME_MAX_LEN];              //媒体名字(本地文件为全路径)
    uint16                         full_path_len;              //媒体名字长度 
    BOOLEAN                    is_dplayerhandle;        //是否通过 dphandle来获取缩略图full_path_ptr 为NULL时有效
    BOOLEAN                    is_open;                      //MMIAPIVP_ThumbnailOpen 调用成功后为true,VP_ThumbnailClose后为false
    BOOLEAN                    is_loading;                   // 是否 正在运行一个处理  
    LCD_ANGLE_E             lcd_angle;                          //当前lcd 角度，避免横竖屏快速切换导致获取的缩略图异步出错
    uint32                          seektime;                   // 需要获取缩略图的时间点
    DPLAYER_HANDLE        dphandle;                   //dplayer handle
    THUMBNAIL_CALLBACK                 tn_callback;              //ui的回调函数
    DPLAYER_IMAGE_DATA_T *image_data;        //图像数据,out
    DPLAYER_DISPLAY_PARAM_T  display_param;//dplay参数 ,in
    uint32                                    taskid;  //默认P_APP
#ifdef KING_MOVIE_SUPPORT
    VPLAYER_SECURITY_HANDLE         sec_handle;        //SECURITY_HANDLE  VPLAYER_SECURITY_HANDLE
    BOOLEAN  b_sprd_kmv;
#endif

    BOOLEAN                               is_videoqueue;
    VP_THUMBNAIL_VIDEOQUEUE     *videoqueue;
}VP_THUMBNAIL_T;  
#endif


/*****************************************************************************/
//  Description : 获得RTP/RTCP最低最高端口。
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC void VP_Setting_GetRTPPort(uint16 *lowest_port_ptr,
                                  uint16 *highest_port_ptr);

/*****************************************************************************/
//  Description : 设置代理参数。
//  Global resource dependence :  none
//  Author: apple.zhang
/*****************************************************************************/
PUBLIC void VP_Setting_SetAgent(BOOLEAN  is_use_agent,
                                uint8 *ip_ptr,
                                uint8  ip_length,
                                uint16 port);

/*****************************************************************************/
//  Description : 获得代理参数。
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC void VP_Setting_GetAgent(BOOLEAN  *is_use_agent_ptr,
                                uint8 *ip_ptr,
                                uint8 ip_max_length,
                                uint16 *port_ptr);
                                
/*****************************************************************************/
//  Description : 获取是否开启随机播放模式
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN VP_Setting_GetIsRandomPlayMode(void);

/*****************************************************************************/
//  Description : 设置是否开启随机播放模式
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC void VP_Setting_SetIsRandomPlayMode(BOOLEAN is_open);

/*****************************************************************************/
//  Description : 获取网络连接设置的index
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC uint8 VP_Setting_GetNetIndex(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
//  Description : 设置网络连接设置的index。
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC void VP_Setting_SetNetIndex(MN_DUAL_SYS_E dual_sys,
                                    uint8         index);
/*****************************************************************************/
//  Description : 设置RTP/RTCP最低最高端口。
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC void VP_Setting_SetRTPPort(uint16 lowest_port,
                                  uint16 highest_port);
                                    
/*****************************************************************************/
//  Description : 获取亮度大小
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC uint8 VP_Setting_GetBrightness(void);

/*****************************************************************************/
//  Description : 设置亮度
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC void VP_Setting_SetBrightness(uint8 brightness);

/*****************************************************************************/
//  Description : 获取本地文件播放方式
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC VP_PLAYBACK_MODE_E VP_Setting_GetPlaybackMode(void);

/*****************************************************************************/
//  Description : 设置本地文件播放方式
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC void VP_Setting_SetPlaybackMode(VP_PLAYBACK_MODE_E playback_mode);

/*****************************************************************************/
//  Description : 获取循环播放模式类型
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC VP_REPEAT_PLAY_MODE_E VP_Setting_GetRepeatPlayMode(void);


/*****************************************************************************/
//  Description : 设置循环播放模式类型
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC void VP_Setting_SetRepeatPlayMode(VP_REPEAT_PLAY_MODE_E mode);

/*****************************************************************************/
//  Description : 设置循环播放模式类型
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC void VP_Setting_SetRepeatPlayMode(VP_REPEAT_PLAY_MODE_E mode);

/*****************************************************************************/
//  Description : 恢复初始设置
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 返回的指针需要调用DestroyMsg释放
/*****************************************************************************/
PUBLIC void VP_Setting_ResetSettingInfo(void);

/*****************************************************************************/
//  Description : 获取设置信息
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 返回的指针需要调用DestroyMsg释放
/*****************************************************************************/
PUBLIC VP_NV_SETTING_T VP_Setting_GetSettingInfo(void);

/*****************************************************************************/
//  Description : 获得视频信息
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN VP_GetMediaInfo(VP_HANDLE  vp_handle,
                            DPLAYER_MEDIA_INFO_T    *media_info_ptr);

/*****************************************************************************/
//  Description : 进行加快速率视频播放本地或网络
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN VP_StartSetRate(VP_HANDLE   vp_handle,
                               BOOLEAN   is_backward);

/*****************************************************************************/
//  Description : 停止进行加快速率视频播放本地或网络
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN VP_StopSetRate(VP_HANDLE   vp_handle,
                               BOOLEAN   is_backward);

/*****************************************************************************/
//  Description : 发起鉴权认证
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
#ifdef MMI_STREAMING_PLAYER_SUPPORT 
PUBLIC BOOLEAN VP_AuthRsp(VP_HANDLE   vp_handle,
                        VP_AUTH_INFO_T  *auth_info_ptr);
#endif
/*****************************************************************************/
//  Description : 用于处理其他task给jvm发送的消息
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIVP_DispatchSignalToFsmMsg(PWND app_ptr, uint16 msg_id, DPARAM param);
/*****************************************************************************/
//  Description : create mutex 
//  Global resource dependence : 
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC void VP_CreateFsmMutex(void);
#if  0//norflashcut
/*****************************************************************************/
//  Description : VP_UpdateCurrentFrame
//  Global resource dependence : 
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC void VP_UpdateCurrentFrame(VP_HANDLE vp_handle);
#endif
/*****************************************************************************/
//  Description : align 4
//  Global resource dependence : 
//  Author: bruce.chi
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMIVP_GetAlign4Offset(uint32 x);

/*****************************************************************************/
//  Description : DelaySwitchVideoPlayer
//  Global resource dependence : 
//  Author: fuyanzhang
//  Note: 
/*****************************************************************************/
PUBLIC void DelaySwitchVideoPlayer(uint32 param);

/*****************************************************************************/
//  Description : HandleDelaySwitchVideoPlayerTimer
//  Global resource dependence : 
//  Author: fuyanzhang
//  Note: 
/*****************************************************************************/
PUBLIC void HandleDelaySwitchVideoPlayerTimer(uint8 timer_id, uint32 param);

/*****************************************************************************/
//  Description : StopDelaySwitchVideoPlayerTimer
//  Global resource dependence : 
//  Author: fuyanzhang
//  Note: 
/*****************************************************************************/
PUBLIC void StopDelaySwitchVideoPlayerTimer(void);

/*****************************************************************************/
//  Description : API of create audio file handle
//  Global resource dependence : none
//  Author: 
//  Note: SUCCESS - return the audio handle created
//        FAILURE - return SCI_NULL
/*****************************************************************************/
PUBLIC MMISRV_HANDLE_T MMIvp_RequestHandle(void);

/*****************************************************************************/
//  Description : Set current play handle
//  Global resource dependence : none
//  Author: yaye.jiang
//  Note: 
/*****************************************************************************/
PUBLIC MMISRV_HANDLE_T MMIvp_GetAudioHandle(void);

/*****************************************************************************/
//  Description : Set current record handle
//  Global resource dependence : none
//  Author: yaye.jiang
//  Note: 
/*****************************************************************************/
PUBLIC MMISRV_HANDLE_T MMIvp_SetAudioHandle(MMISRV_HANDLE_T handle);

#ifdef SCREENSAVER_SUPPORT
/*****************************************************************************/
//  Description : 设置osd和透明色，某些情况下 需要再每次播放时需要设置一次，如屏保界面
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void VP_SetBlockTypeForSS(void);
#endif

#ifdef MMI_AMRBT_VPSUPPORT 
/*****************************************************************************/
//  Description : bt play next file
//  Global resource dependence : none
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
PUBLIC void VP_EventNext(void);

/*****************************************************************************/
//  Description : bt play pre file
//  Global resource dependence : none
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
PUBLIC void VP_EventPrev(void);

/*****************************************************************************/
//  Description : bt notify video player play or pause 
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void VP_EventPlayOrPause(void);
#endif
/*****************************************************************************/
//  Description : 调节音量窗口
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIVP_TurnVol(MMICOM_PANEL_OPEN_TYPE_E turn_vol_type);

/*****************************************************************************/
//  Description : 获得s_current_fsm_ptr
//  Global resource dependence :  none
//  Author:  
//  Note: 
/*****************************************************************************/
PUBLIC VP_HANDLE VP_GetCurrentFSMHandle(void);
/*****************************************************************************/
//  Description : 获得dphandle
//  Global resource dependence :  none
//  Author:  
//  Note: 
/*****************************************************************************/
PUBLIC DPLAYER_HANDLE VP_GetDPLAYERhandlebyVPhandle(VP_HANDLE vphandle);


 #ifdef  VIDEOTHUMBNAIL_SUPPORT 
/*****************************************************************************/
//  Description :收到第一侦 播放成功消息, 获取缩略图并做UI回调处理  
//  Global resource dependence : 
//  Author:  
//  Note: 
/*****************************************************************************/
PUBLIC void HandleGetThumbnaiData(void);
/*****************************************************************************/
//  Description : HandleOpenNextVideo
//  Global resource dependence : 
//  Author:  
//  Note: 
/*****************************************************************************/
PUBLIC void HandleOpenNextVideo(void);
#endif
#ifdef KING_MOVIE_SUPPORT
/*****************************************************************************/
//  Description : VP_dplayerOpenSecurty
//  Global resource dependence : 
//  Author: tonny.chen
//  Note: 
/*****************************************************************************/
PUBLIC VPLAYER_SECURITY_HANDLE VP_dplayerOpenSecurty(void * pvoid,BOOLEAN *b_sprd_kmv, VDEC_SECURITY_PLUG_INFO_T** sec_info_ptr);//andy.he_kingmv

/*****************************************************************************/
//  Description : VP_dplayerCloseSecurty
//  Global resource dependence : 
//  Author: tonny.chen
//  Note: 
/*****************************************************************************/
PUBLIC void VP_dplayerCloseSecurty(void * pvoid,VPLAYER_SECURITY_HANDLE sec_handle,BOOLEAN b_sprd_kmv, VDEC_SECURITY_PLUG_INFO_T** sec_info_ptr);

/*****************************************************************************/
//  Description : VP_dplayerExtCtlAddSecurty
//  Global resource dependence : 
//  Author: tonny.chen
//  Note: 
/*****************************************************************************/
PUBLIC void VP_dplayerExtCtlAddSecurty(void * pvoid,DPLAYER_HANDLE player_handle);

/*****************************************************************************/
//  Description : VP_dplayerExtCtlDeltSecurty
//  Global resource dependence : 
//  Author: tonny.chen
//  Note: 
/*****************************************************************************/
PUBLIC void VP_dplayerExtCtlDeltSecurty(void * pvoid,DPLAYER_HANDLE player_handle, VPLAYER_SECURITY_HANDLE sec_handle);
#endif
#else
    /*****************************************************************************/
//  Description : 用于处理其他task给jvm发送的消息
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIVP_DispatchSignalToFsmMsg(PWND app_ptr, uint16 msg_id, DPARAM param);
/*****************************************************************************/
//  Description : 调节音量窗口
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIVP_TurnVol(MMICOM_PANEL_OPEN_TYPE_E turn_vol_type);
/*****************************************************************************/
//  Description : align 4
//  Global resource dependence : 
//  Author: bruce.chi
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMIVP_GetAlign4Offset(uint32 x);
#ifdef KING_MOVIE_SUPPORT
/*****************************************************************************/
//  Description : VP_dplayerOpenSecurty
//  Global resource dependence : 
//  Author: tonny.chen
//  Note: 
/*****************************************************************************/
PUBLIC VPLAYER_SECURITY_HANDLE VP_dplayerOpenSecurty(void * pvoid,BOOLEAN *b_sprd_kmv);//andy.he_kingmv

/*****************************************************************************/
//  Description : VP_dplayerCloseSecurty
//  Global resource dependence : 
//  Author: tonny.chen
//  Note: 
/*****************************************************************************/
PUBLIC void VP_dplayerCloseSecurty(void * pvoid,VPLAYER_SECURITY_HANDLE sec_handle,BOOLEAN b_sprd_kmv, VDEC_SECURITY_PLUG_INFO_T* sec_info_ptr);

/*****************************************************************************/
//  Description : VP_dplayerExtCtlAddSecurty
//  Global resource dependence : 
//  Author: tonny.chen
//  Note: 
/*****************************************************************************/
PUBLIC void VP_dplayerExtCtlAddSecurty(void * pvoid,DPLAYER_HANDLE player_handle);

/*****************************************************************************/
//  Description : VP_dplayerExtCtlDeltSecurty
//  Global resource dependence : 
//  Author: tonny.chen
//  Note: 
/*****************************************************************************/
PUBLIC void VP_dplayerExtCtlDeltSecurty(void * pvoid,DPLAYER_HANDLE player_handle, VPLAYER_SECURITY_HANDLE sec_handle);
#endif


#endif //MMI_VIDEOPLAYER_MINI_FUNCTION
/**--------------------------------------------------------------------------*
**                         Compiler Flag                                    *
**--------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif


#endif
