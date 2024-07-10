/*****************************************************************************
** File Name:                    mivp_fsm.c                                  *
** Author:                                                                   *
** Date:           2010/3/11                                                 *
** Copyright:      2010 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe vp state machine            *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2010/3/11      apple.zhang      Create
******************************************************************************/

#define MMIVP_FSM_C
/*---------------------------------------------------------------------------*/
/*                          Include Files                                    */
/*---------------------------------------------------------------------------*/
#include "mmi_app_video_player_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include <string.h>
#include "mmivp_fsm.h"
#include "mmivp_export.h"
#include "os_api.h"
#include "mmipdp_export.h"
#include "dal_player.h"
#include "sig_code.h"
#include "mmiconnection_export.h"
#ifdef MMI_STREAMING_PLAYER_SUPPORT
#include "strm_api.h"
#include "in_message.h"
#else
#include "mmivp_dummy.h"
#endif
#include "mmiphone_export.h"
#ifndef MMI_VIDEOPLAYER_MINI_FUNCTION
#include "mmivp_locallist.h"
#endif
#include "mmivp_id.h"
//#include "mmiset.h"
#include "mmiset_export.h"
#include "mmi_theme.h"
#include "mmi_default.h"
#include "mmivp_historylist.h"
#include "mmivp_internal.h"
#include "mmidisplay_data.h"
#include "guiblock.h"
#include "mmibt_export.h"

#ifdef VT_SUPPORT
#include "mmivt_export.h"
#endif
#ifdef DRM_SUPPORT
#include "mmidrm_export.h" 
#endif
#include "mmiset_export.h"


#ifdef MMI_KING_MOVIE_SUPPORT
#include "mmikm_export.h"
#include "video_security.h"
#ifdef MMI_KING_MOVIE_SUPPORT_EXTEND
#include "video_security_kmv.h"
#endif
#ifdef MMI_KING_MOVIE_SUPPORT_HMV
#include "video_security_hmv.h"
#endif
#endif
#ifdef MMI_KING_MOVIE_SUPPORT
//LOCAL  VPLAYER_SECURITY_HANDLE s_vplayer_sec_handle = PNULL;
LOCAL  void *s_vdec_sec_argv[2] = {NULL, NULL};
LOCAL  VDEC_SECURITY_PLUG_INFO_T *s_vdec_sec_info_ptr = PNULL;
#endif

LOCAL uint8  s_switch_timer_id = 0;
LOCAL GUI_LCD_DEV_INFO s_video_layer = {0};
LOCAL BOOLEAN  s_is_audio_coexist = FALSE;
#ifdef MMI_STREAMING_PLAYER_SUPPORT
LOCAL BOOLEAN  s_is_special_pause = FALSE;
#endif
/*---------------------------------------------------------------------------*/
/*                          MACRO DEFINITION                                 */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*                         GLOBAL DEFINITION                                 */
/*---------------------------------------------------------------------------*/
extern int strnicmp (const char *a, const char *b, size_t c);

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
#ifdef MMI_STREAMING_PLAYER_SUPPORT
/*****************************************************************************/
//  Description : whether video palyer collison or not
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsVideoPlayerCollision(void);
#endif
/*****************************************************************************/
//  Description : 创建videoplayer状态机消息
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL VP_FSM_MSG_T* CreatMsg(VP_HANDLE fsm_handle, VP_FSM_MSG_E msg_id, void *msg_body_ptr, uint32 msg_body_size);

/*****************************************************************************/
//  Description : 释放状态机消息
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN DestroyMsg(VP_FSM_MSG_T *msg_ptr);

/*****************************************************************************/
//  Description : 创建状态机
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL VP_HANDLE CreateFSM(VP_CALLBACK vpcallback);

/*****************************************************************************/
//  Description : 销毁状态机
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL void DestroyFSM(VP_HANDLE vp_handle);

/*****************************************************************************/
//  Description : 判断状态机是否有效
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsValidFsm(VP_FSM_T        *fsm_ptr);

/*****************************************************************************/
//  Description : 分发状态机消息
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN FsmDispatch(VP_HANDLE   vp_handle,  VP_FSM_MSG_E msg_id, void *msg_body_ptr, uint32 msg_body_size);

/*****************************************************************************/
//  Description : 转换状态
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL void FsmStateTrans(VP_FSM_T *fsm_ptr, VP_STATE_E state);

/*****************************************************************************/
//  Description : 给ui层发送VP_PREPARE_IND
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL void SendPrepareIndToUI(VP_FSM_T *fsm_ptr);

/*****************************************************************************/
//  Description : 给ui层发送VP_AUTH_IND
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL void SendAUTHIndToUI(VP_FSM_T *fsm_ptr);

/*****************************************************************************/
//  Description : 给ui层发送VP_FATAL_ERROR_IND
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL void SendFatalErrorIndTOUI(VP_FSM_T *fsm_ptr,VP_ERROR_E error_code);

/*****************************************************************************/
//  Description : 给ui层发送VP_PLAY_PROCESS_IND
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL void SendProcessIndToUI(VP_FSM_T *fsm_ptr);

/*****************************************************************************/
//  Description : 给ui层发送VP_BUFFERING_IND
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL void SendBufferingIndToUI(VP_FSM_T *fsm_ptr);

/*****************************************************************************/
//  Description : 给ui层发送VP_BUFFER_PROCESS_IND
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL void SendBufferPercentToUI(VP_FSM_T *fsm_ptr);

/*****************************************************************************/
//  Description : 给ui层发送VP_BUFFER_END_IND
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL void SendBufferEndIndToUI(VP_FSM_T *fsm_ptr);

/*****************************************************************************/
//  Description : 给ui层发送VP_PLAY_END_IND
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL void SendPlayEndToUI(VP_FSM_T *fsm_ptr, VP_PLAY_END_RESULT_E result);

/*****************************************************************************/
//  Description : 给ui层发送VP_STOP_IND
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL void SendStopIndToUI(VP_FSM_T *fsm_ptr);

/*****************************************************************************/
//  Description : 给ui层发送VP_PLAY_CNF
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL void SendPlayCnfToUI(VP_FSM_T *fsm_ptr,VP_ERROR_E error_code);

/*****************************************************************************/
//  Description : 给ui层发送VP_PAUSE_CNF
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL void SendPauseCnfToUI(VP_FSM_T *fsm_ptr,VP_ERROR_E error_code);

/*****************************************************************************/
//  Description : 给ui层发送VP_SEEK_CNF
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL void SendSeekCnfToUI(VP_FSM_T *fsm_ptr,VP_ERROR_E error_code);

/*****************************************************************************/
//  Description : SendSignalTo current playing task
//  Global resource dependence : 
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL void SendSignalToCurPlayingTask(uint16 signalType, uint16 data, uint32 task_id);

/*****************************************************************************/
//  Description : Get frame data call back
//  Global resource dependence : 
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
LOCAL DPLAYER_RETURN_E GetFrameDataCallback(DPLAYER_IMAGE_DATA_T *src_frame_ptr);

/*****************************************************************************/
//  Description : frame end notify call back
//  Global resource dependence : 
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL void CallbackFrameEndNotify(int32 frame_num);

/*****************************************************************************/
//  Description : end notify回调函数
//  Global resource dependence : 
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
//LOCAL void CallbackEndNotify(int32 i_type);

/*****************************************************************************/
//  Description : 通过frame获得对应的time
//  Global resource dependence : 
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL uint32 FrameToTime(DPLAYER_HANDLE video_handle, uint32 frame);

/*****************************************************************************/
//  Description : 开始播放本地文件
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL VP_ERROR_E PlayLocalFile(VP_FSM_T *fsm_ptr, BOOLEAN is_need_fill);

/*****************************************************************************/
//  Description : 开始播放本地buf
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL VP_ERROR_E PlayLocalBuf(VP_FSM_T *fsm_ptr, VP_PLAY_PARAM_T  *play_param_ptr);

/*****************************************************************************/
//  Description : 开始播放网络文件
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL VP_ERROR_E PlayFromNet(VP_FSM_T *fsm_ptr,DPLAYER_STRM_CFG_T strm_cfg);

/*****************************************************************************/
//  Description : 处理pdp消息
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL void HandlePDPMsg(MMIPDP_CNF_INFO_T *msg_ptr);

/*****************************************************************************/
//  Description : 激活pdp
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN ActivePdp(VP_FSM_T *fsm_ptr);

/*****************************************************************************/
//  Description : 初始化状态机信息
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL void InitFsmInfo(VP_FSM_T *fsm_ptr, 
                       VP_STATE_E      state, 
                       VP_CALLBACK     vpcallback);

/*****************************************************************************/
//  Description : 更新状态机信息
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN UpdateFsmInfo(VP_FSM_T *fsm_ptr, VP_PLAY_PARAM_T* play_param_ptr);

/*****************************************************************************/
//  Description : 空闲状态的处理
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN FsmIdle(VP_FSM_T *fsm_ptr, VP_FSM_MSG_T *msg_ptr);

/*****************************************************************************/
//  Description : 准备好状态的处理
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN FsmReady(VP_FSM_T *fsm_ptr, VP_FSM_MSG_T *msg_ptr);

#ifdef MMI_STREAMING_PLAYER_SUPPORT
/*****************************************************************************/
//  Description : PDP激活中处理
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN InitStreaming(VP_FSM_T *fsm_ptr, uint32 net_id);
#endif
/*****************************************************************************/
//  Description : PDP激活中处理
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN FsmPdpActiving(VP_FSM_T *fsm_ptr, VP_FSM_MSG_T *msg_ptr);

/*****************************************************************************/
//  Description : 已初始化状态
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN FsmInit(VP_FSM_T *fsm_ptr, VP_FSM_MSG_T *msg_ptr);

/*****************************************************************************/
//  Description : 已初始化状态
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN FsmInit_PauseEx(VP_FSM_T *fsm_ptr, VP_FSM_MSG_T *msg_ptr);

/*****************************************************************************/
//  Description : 认证中
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN FsmInitAuth(VP_FSM_T *fsm_ptr, VP_FSM_MSG_T *msg_ptr);

/*****************************************************************************/
//  Description : 播放准备中
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN FsmPlayPeding(VP_FSM_T *fsm_ptr, VP_FSM_MSG_T *msg_ptr);

/*****************************************************************************/
//  Description : 认证中
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN FsmPlayAuth(VP_FSM_T *fsm_ptr, VP_FSM_MSG_T *msg_ptr);

/*****************************************************************************/
//  Description : 播放中
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN FsmPlaying(VP_FSM_T *fsm_ptr, VP_FSM_MSG_T *msg_ptr);

/*****************************************************************************/
//  Description : 缓冲中
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN FsmBuffering(VP_FSM_T *fsm_ptr, VP_FSM_MSG_T *msg_ptr);

/*****************************************************************************/
//  Description : 播放定位中
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN FsmSeeking(VP_FSM_T *fsm_ptr, VP_FSM_MSG_T *msg_ptr);

/*****************************************************************************/
//  Description : 暂停中
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN FsmPausing(VP_FSM_T *fsm_ptr, VP_FSM_MSG_T *msg_ptr);

/*****************************************************************************/
//  Description : 暂停
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN FsmPaused(VP_FSM_T *fsm_ptr, VP_FSM_MSG_T *msg_ptr);

/*****************************************************************************/
//  Description : 暂停后的player停止，streaming暂停
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN FsmPausedEx(VP_FSM_T *fsm_ptr, VP_FSM_MSG_T *msg_ptr);

/*****************************************************************************/
//  Description : 暂停中播放定位中
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN FsmPausedSeeking(VP_FSM_T *fsm_ptr, VP_FSM_MSG_T *msg_ptr);

/*****************************************************************************/
//  Description : 恢复定位中
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN FsmResuming(VP_FSM_T *fsm_ptr, VP_FSM_MSG_T *msg_ptr);

/*****************************************************************************/
//  Description : 停止中
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN FsmStopping(VP_FSM_T *fsm_ptr, VP_FSM_MSG_T *msg_ptr);

/*****************************************************************************/
//  Description : 停止
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN FsmStopped(VP_FSM_T *fsm_ptr, VP_FSM_MSG_T *msg_ptr);

/*****************************************************************************/
//  Description : 关闭中
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN FsmClosing(VP_FSM_T *fsm_ptr, VP_FSM_MSG_T *msg_ptr);

/*****************************************************************************/
//  Description : Get the supported_type of a movie.
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL VP_SUPPORTED_STATUS_TYPE_E CheckSupportedTypeFromMediaInfo(DPLAYER_MEDIA_INFO_T *media_ptr);

/*****************************************************************************/
//  Description : 处理streaming init cnf消息
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
#ifdef MMI_STREAMING_PLAYER_SUPPORT 
LOCAL STRM_RESULT_E HandleInitCnf(VP_FSM_T * fsm_ptr, STRM_INIT_CNF_T* strm_init_ptr, VP_ERROR_E *is_play_success);
#endif
/*****************************************************************************/
//  Description : FRAME数据准备好后，获取scaling好的数据
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL void HandleGetFrameData(VP_FSM_T * fsm_ptr);

/*****************************************************************************/
//  Description : 处理update frame请求消息
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL void HandleUpdateFrameReq(VP_FSM_T * fsm_ptr);

/*****************************************************************************/
//  Description : 处理pause请求消息
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN HandlePauseReq(VP_FSM_T * fsm_ptr);

/*****************************************************************************/
//  Description : 处理resume请求消息
//  Global resource dependence : 
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN HandleResumeReq(VP_FSM_T* fsm_ptr);

/*****************************************************************************/
//  Description : 处理seek请求消息
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN HandleSeekReq(VP_FSM_T * fsm_ptr, VP_SEEK_PARAM_T *seek_param);

/*****************************************************************************/
//  Description : 处理停止消息
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN HandleStopReq(VP_FSM_T* fsm_ptr,BOOLEAN is_begin_play);

#ifndef MMI_VIDEOPLAYER_MINI_FUNCTION
/*****************************************************************************/
//  Description : 通知播放下一首
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL void NotifyPlayNextFile(VP_FSM_T *fsm_ptr, VP_PLAY_END_RESULT_E last_play_result);

/*****************************************************************************/
//  Description : 播放下一首
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL VP_ERROR_E PlayNextFile(VP_FSM_T *fsm_ptr, VP_PLAY_END_RESULT_E last_play_result);
#endif

/*****************************************************************************/
//  Description : special pause
//  Global resource dependence : 
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN HandlePauseReqEx(BOOLEAN is_pause_streaming,
                               VP_FSM_T* fsm_ptr);
/*****************************************************************************/
//  Description : special resume
//  Global resource dependence : 
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN HandleResumeReqEx(BOOLEAN is_resume_streaming,
                                VP_FSM_T* fsm_ptr);

/*****************************************************************************/
//  Description : SetPlaybackMode
//  Global resource dependence : 
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL void SetPlaybackMode(VP_FSM_T   *fsm_ptr, VP_PLAYBACK_MODE_E playback_mode);

#ifdef DRM_SUPPORT
/*****************************************************************************/
//  Description : consume drm right
//  Global resource dependence : 
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
LOCAL void ConsumeDrmRight(const wchar *name_path_ptr, 
                           uint16       name_len);
#endif


/*****************************************************************************/
//  Description : video player srv play call back function.
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN VPPlayCallBack(MMISRV_HANDLE_T handle, MMISRVMGR_NOTIFY_PARAM_T *param);

/*****************************************************************************/
//  Description : 
//  Global resource dependence :  none
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN ReqPlaySrv(VP_FSM_T *fsm_ptr);

/*---------------------------------------------------------------------------*/
/*                         STATIC DEFINITION                                 */
/*---------------------------------------------------------------------------*/
LOCAL   uint32                  s_vp_fsm_list       = 0;  //vp state machine list; 扩展多状态机使用
LOCAL   SCI_MUTEX_PTR           s_fsm_list_mutex    = 0; //mutex to protect s_vp_fsm_list;
LOCAL   VP_FSM_T                *s_current_fsm_ptr  = PNULL;  //记录当前fsm
const   FSM_DISPATCH  s_fsm_dispatch[VP_STATE_MAX]    = {         //状态机的分配处理函数
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
#ifdef MMI_STREAMING_PLAYER_SUPPORT 
/*****************************************************************************/
//  Description : whether video palyer collison or not,for fix bug  MS00247685
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsVideoPlayerCollision(void)
{
    BOOLEAN result = FALSE;

#ifdef VT_SUPPORT		
    if(MMIAPIVT_IsVtCalling())
    	{
					result = TRUE;
    	}
#endif

		//SCI_TRACE_LOW:"IsVideoPlayerCollision ret = %d"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_539_112_2_18_3_5_4_18,(uint8*)"d" , result);
		return result;  
}
#endif
/*****************************************************************************/
//  Description : 创建videoplayer状态机消息
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 返回的指针需要调用DestroyMsg释放
/*****************************************************************************/
LOCAL VP_FSM_MSG_T* CreatMsg(VP_HANDLE fsm_handle, VP_FSM_MSG_E msg_id, void *msg_body_ptr, uint32 msg_body_size)
{
    VP_FSM_MSG_T    *msg_ptr        =   PNULL;
    
   // VP_STRACE_LOW("[VP] CreatMsg fsm_handle = %x, msg_id = %x,  msg_body_ptr = %x, msg_body_size = %d",fsm_handle,msg_id,msg_body_ptr,msg_body_size);
    //为状态机消息分配连续空间，便于后续释放
    switch(msg_id) 
    {
    case VP_PLAY_REQ:
        {
            VP_PLAY_PARAM_T *play_param_ptr = (VP_PLAY_PARAM_T *)msg_body_ptr;
            VP_PLAY_PARAM_T *play_param2_ptr = PNULL;
            if (PNULL == play_param_ptr || msg_body_size != sizeof(VP_PLAY_PARAM_T))
            {
                break;
            }
            //获取序列化的长度
            //VP_STRACE_LOW:"[VP] CreatMsg play_param_ptr->play_type = %d, msg_body_size = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_563_112_2_18_3_5_4_19,(uint8*)"dd",play_param_ptr->play_type,msg_body_size);
            switch(play_param_ptr->play_type) 
            {
            case VP_PLAY_FROM_LOCALLIST:
            case VP_PLAY_FROM_FILE:
                if (PNULL == play_param_ptr->full_path_ptr)
                {
                    //VP_STRACE_LOW:"[VP] CreatMsg full path ptr is null"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_570_112_2_18_3_5_4_20,(uint8*)"");
                    return PNULL;
                }
                //4对齐
                msg_body_size = msg_body_size + ((play_param_ptr->full_path_len+1) * sizeof(wchar)) + sizeof(void *);
                break;
            case VP_PLAY_FROM_BUFFER:
                if (PNULL == play_param_ptr->video_buf_info.video_buf_ptr)
                {
                    //VP_STRACE_LOW:"[VP] CreatMsg buffer is null"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_579_112_2_18_3_5_4_21,(uint8*)"");
                    return PNULL;
                }
                //msg_body_size += play_param_ptr->video_buf_info.video_buf_len + 1;
                break;
#ifdef MMI_STREAMING_PLAYER_SUPPORT
            case VP_PLAY_FROM_NET:
                if (PNULL == play_param_ptr->url_ptr)
                {
                    //VP_STRACE_LOW:"[VP] CreatMsg url is null"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_588_112_2_18_3_5_4_22,(uint8*)"");
                    return PNULL;
                }
                //VP_STRACE_LOW:"[VP] CreatMsg msg_body_size = %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_591_112_2_18_3_5_4_23,(uint8*)"d",msg_body_size);
                msg_body_size = msg_body_size + strlen(play_param_ptr->url_ptr) + 1;
                //VP_STRACE_LOW:"[VP] CreatMsg msg_body_size = %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_593_112_2_18_3_5_4_24,(uint8*)"d",msg_body_size);
                if (0 != play_param_ptr->full_path_len && PNULL != play_param_ptr->full_path_ptr)
                {
                    //4对齐
                    msg_body_size = msg_body_size + ((play_param_ptr->full_path_len+1) * sizeof(wchar)) + sizeof(void *);
                }
                break;
#endif
            default:
                return msg_ptr;
            } 

            //序列化
            msg_ptr = (VP_FSM_MSG_T*)SCI_ALLOCA(sizeof(VP_FSM_MSG_T)+msg_body_size);   
            //VP_STRACE_LOW:"[VP] msg_ptr = %x, len = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_607_112_2_18_3_5_4_25,(uint8*)"dd",msg_ptr,sizeof(VP_FSM_MSG_T)+msg_body_size);
            if(PNULL != msg_ptr)
            {
                //消息头
                SCI_MEMSET(msg_ptr,0,(sizeof(VP_FSM_MSG_T)+ msg_body_size));
                msg_ptr->fsm_handle = fsm_handle;
                msg_ptr->msg_id = msg_id;
                msg_ptr->msg_size = sizeof(VP_FSM_MSG_T);
                msg_ptr->msg_body = (void *)((uint32)msg_ptr + msg_ptr->msg_size);
                //参数结构序列化
                SCI_MEMCPY(msg_ptr->msg_body,msg_body_ptr,sizeof(VP_PLAY_PARAM_T));
                play_param2_ptr = (VP_PLAY_PARAM_T *)(msg_ptr->msg_body);
                //VP_STRACE_LOW:"apple play_param2_ptr = %x, msg_body_size =%d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_619_112_2_18_3_5_4_26,(uint8*)"dd",play_param2_ptr, msg_body_size);
                switch(play_param_ptr->play_type) 
                {
                case VP_PLAY_FROM_LOCALLIST:
                case VP_PLAY_FROM_FILE:                    
                    play_param2_ptr->full_path_ptr = (wchar *)((uint32)msg_ptr+sizeof(VP_FSM_MSG_T) + sizeof(VP_PLAY_PARAM_T));
                    play_param2_ptr->full_path_ptr = (wchar *)((uint32)(play_param2_ptr->full_path_ptr) + MMIVP_GetAlign4Offset((uint32)(play_param2_ptr->full_path_ptr)));
                    play_param2_ptr->full_path_ptr = MMIAPICOM_Wstrncpy(play_param2_ptr->full_path_ptr, play_param_ptr->full_path_ptr,play_param_ptr->full_path_len);
                    break;   
#ifdef MMI_STREAMING_PLAYER_SUPPORT
                case VP_PLAY_FROM_NET:
                    play_param2_ptr->url_ptr = (char*)((uint32)msg_ptr+sizeof(VP_FSM_MSG_T) + sizeof(VP_PLAY_PARAM_T));
                    SCI_MEMCPY(play_param2_ptr->url_ptr,play_param_ptr->url_ptr,(strlen(play_param_ptr->url_ptr)));/*lint !e666*/
                    //VP_STRACE_LOW:"[VP] CreatMsg play_param2_ptr->url_ptr = %x, len = %x"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_632_112_2_18_3_5_4_27,(uint8*)"dd",play_param2_ptr->url_ptr,strlen(play_param_ptr->url_ptr));
                    if (0 != play_param_ptr->full_path_len && PNULL != play_param_ptr->full_path_ptr)
                    {
                        play_param2_ptr->full_path_ptr = (wchar *)((uint32)play_param2_ptr->url_ptr + strlen(play_param_ptr->url_ptr) + 1);
                        //VP_STRACE_LOW:"apple play_param2_ptr->full_path_ptr = %x"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_636_112_2_18_3_5_4_28,(uint8*)"d",play_param2_ptr->full_path_ptr);
                        play_param2_ptr->full_path_ptr = (wchar *)((uint32)(play_param2_ptr->full_path_ptr) + MMIVP_GetAlign4Offset((uint32)(play_param2_ptr->full_path_ptr)));
                        //VP_STRACE_LOW:"apple play_param2_ptr->full_path_ptr = %x"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_638_112_2_18_3_5_4_29,(uint8*)"d",play_param2_ptr->full_path_ptr);
                        //VP_STRACE_LOW:"apple play_param_ptr->full_path_ptr = %x, play_param_ptr->full_path_len = %d"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_639_112_2_18_3_5_4_30,(uint8*)"dd",play_param_ptr->full_path_ptr,play_param_ptr->full_path_len);
                        play_param2_ptr->full_path_ptr = MMIAPICOM_Wstrncpy(play_param2_ptr->full_path_ptr, play_param_ptr->full_path_ptr,play_param_ptr->full_path_len);
                        //VP_STRACE_LOW:"apple play_param2_ptr->full_path_ptr = %x"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_641_112_2_18_3_5_4_31,(uint8*)"d",play_param2_ptr->full_path_ptr);
                    }
                    else
                    {
                        play_param2_ptr->full_path_ptr = PNULL;
                        play_param2_ptr->full_path_len = 0;
                    }
                    break;
#endif
                default:
                    break;
                }
                
            }    
        }
        break;
 #ifdef MMI_STREAMING_PLAYER_SUPPORT       
    case VP_AUTH_RSP:
        {
            VP_AUTH_INFO_T    *auth_info_ptr = (VP_AUTH_INFO_T*)msg_body_ptr;
            VP_AUTH_INFO_T    *auth_info2_ptr = PNULL;
            if (PNULL == msg_body_ptr || msg_body_size != sizeof(VP_AUTH_INFO_T))
            {
                break;
            }
            if (PNULL != auth_info_ptr->password_ptr)
            {
                msg_body_size = msg_body_size + strlen(auth_info_ptr->password_ptr) + 1;
            }
            if (PNULL != auth_info_ptr->username_ptr)
            {
                msg_body_size = msg_body_size + strlen(auth_info_ptr->username_ptr) + 1;
            }            
            msg_ptr = (VP_FSM_MSG_T*)SCI_ALLOCA(sizeof(VP_FSM_MSG_T)+msg_body_size);
            if(PNULL != msg_ptr)
            {
                SCI_MEMSET(msg_ptr,0,(sizeof(VP_FSM_MSG_T)+ msg_body_size));
                msg_ptr->fsm_handle = fsm_handle;
                msg_ptr->msg_id = msg_id;
                msg_ptr->msg_size = sizeof(VP_FSM_MSG_T);
                msg_ptr->msg_body = (void *)((uint32)msg_ptr+sizeof(VP_FSM_MSG_T));
                if (PNULL != msg_ptr->msg_body)
                {                
                    SCI_MEMCPY(msg_ptr->msg_body,msg_body_ptr,sizeof(VP_AUTH_INFO_T));
                    auth_info2_ptr = (VP_AUTH_INFO_T *)(msg_ptr->msg_body);                                
                    auth_info2_ptr->password_ptr = (char*)((uint32)(msg_ptr->msg_body) + sizeof(VP_AUTH_INFO_T));
                    if (PNULL != auth_info_ptr->password_ptr && PNULL != auth_info2_ptr->password_ptr)
                    {               
                        SCI_MEMCPY(auth_info2_ptr->password_ptr,auth_info_ptr->password_ptr,(strlen(auth_info_ptr->password_ptr)));/*lint !e666*/
                        auth_info2_ptr->username_ptr = (char*)((uint32)(auth_info2_ptr->password_ptr) + strlen(auth_info_ptr->password_ptr)) + 1;
                    }
                    else
                    {
                        auth_info2_ptr->password_ptr = PNULL;
                        auth_info2_ptr->username_ptr = (char*)((uint32)(msg_ptr->msg_body) + sizeof(VP_AUTH_INFO_T));
                    }
                    if (PNULL != auth_info_ptr->username_ptr && PNULL != auth_info2_ptr->username_ptr)
                    {
                        SCI_MEMCPY(auth_info2_ptr->username_ptr,auth_info_ptr->username_ptr,strlen(auth_info_ptr->username_ptr));/*lint !e666*/
                    }
                    else
                    {
                        auth_info2_ptr->username_ptr = PNULL;
                    }
                
                }
            }    
        }
        break;
        
    case VP_STRM_INIT_CNF:
        {
            STRM_INIT_CNF_T  *strm_init_info_ptr = (STRM_INIT_CNF_T*)msg_body_ptr;
            STRM_INIT_CNF_T  *strm_init_info2_ptr = PNULL;
            uint8            *temp_ptr  = PNULL;
            if (PNULL == msg_body_ptr || msg_body_size != sizeof(STRM_INIT_CNF_T))
            {
                break;
            }
            //VP_STRACE_LOW:"[VP] CreatMsg frame height = %d, rate = %d, base = %d,width = %d, cfg_len = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_724_112_2_18_3_5_4_32,(uint8*)"ddddd",strm_init_info_ptr->sdp_info.video_param.frame_height,strm_init_info_ptr->sdp_info.video_param.frame_rate,strm_init_info_ptr->sdp_info.video_param.frame_rate_base,strm_init_info_ptr->sdp_info.video_param.frame_width,strm_init_info_ptr->sdp_info.video_param.config_len);
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
            //VP_STRACE_LOW:"[VP] CreatMsg strm_init_info_ptr->sdp_info.video_param.config_len = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_741_112_2_18_3_5_4_33,(uint8*)"d",strm_init_info_ptr->sdp_info.video_param.config_len);
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
            //VP_STRACE_LOW:"[VP] CreatMsg sprop_param_sets_num = %d, enc_type = %d, sprop_sets_arr = %x"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_761_112_2_18_3_5_4_34,(uint8*)"ddd",strm_init_info_ptr->sdp_info.video_fmtp_info.fmtp_param.h264_param.sprop_sets_len,strm_init_info_ptr->sdp_info.video_fmtp_info.enc_type,strm_init_info_ptr->sdp_info.video_fmtp_info.fmtp_param.h264_param.sprop_sets_ptr);
            if (STRM_SDP_ENC_H264 == strm_init_info_ptr->sdp_info.video_fmtp_info.enc_type 
                && PNULL != strm_init_info_ptr->sdp_info.video_fmtp_info.fmtp_param.h264_param.sprop_sets_ptr
                && 0 != strm_init_info_ptr->sdp_info.video_fmtp_info.fmtp_param.h264_param.sprop_sets_len)
            {
                msg_body_size = msg_body_size + strm_init_info_ptr->sdp_info.video_fmtp_info.fmtp_param.h264_param.sprop_sets_len +1;
            }

            msg_ptr = (VP_FSM_MSG_T*)SCI_ALLOCA(sizeof(VP_FSM_MSG_T)+msg_body_size);
           
            if(PNULL != msg_ptr)
            {
                SCI_MEMSET(msg_ptr,0,(sizeof(VP_FSM_MSG_T)+ msg_body_size));
                msg_ptr->fsm_handle = fsm_handle;
                msg_ptr->msg_id = msg_id;
                msg_ptr->msg_size = sizeof(VP_FSM_MSG_T);
                msg_ptr->msg_body = (void *)((uint32)msg_ptr+sizeof(VP_FSM_MSG_T));
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
#endif
    default:
        msg_ptr = (VP_FSM_MSG_T*)SCI_ALLOCA(sizeof(VP_FSM_MSG_T)+msg_body_size);
       
        if(PNULL != msg_ptr)
        {
            SCI_MEMSET(msg_ptr,0,(sizeof(VP_FSM_MSG_T)+ msg_body_size));
            msg_ptr->fsm_handle = fsm_handle;
            msg_ptr->msg_id = msg_id;
            msg_ptr->msg_size = sizeof(VP_FSM_MSG_T);
            if (0 == msg_body_size)
            {
                msg_ptr->msg_body = PNULL;
            }
            else
            {
                msg_ptr->msg_body = (void *)((uint32)msg_ptr+sizeof(VP_FSM_MSG_T));
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
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN DestroyMsg(VP_FSM_MSG_T *msg_ptr)
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
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL VP_HANDLE CreateFSM(VP_CALLBACK vpcallback)
{
    
    VP_FSM_T              *fsm_ptr              = PNULL;
    VP_FSM_LIST_T         *cur_fsm_node_ptr     = PNULL;
    
    //目前不支持多个状态机
    if(0 != s_vp_fsm_list)
    {
        //VP_STRACE_LOW:"[VP] CreateFSM there is a fsm"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_969_112_2_18_3_5_4_35,(uint8*)"");
        return 0;
    }

    //为状态机分配空间
    fsm_ptr = (VP_FSM_T*)SCI_ALLOCA(sizeof(VP_FSM_T));
    if (PNULL == fsm_ptr)
    {
        //VP_STRACE_LOW:"[VP] CreateFSM no space for fsm"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_977_112_2_18_3_5_5_36,(uint8*)"");
        return 0;
    }
    SCI_MEMSET(fsm_ptr,0,sizeof(VP_FSM_T));

    //初始化状态机信息    
    fsm_ptr->dplayer_handle = PNULL;
    fsm_ptr->full_path_len = 0;
    fsm_ptr->full_path_ptr = PNULL;
    fsm_ptr->process_time = 0;
    fsm_ptr->state = VP_STATE_IDLE;
    fsm_ptr->total_time = 0;
    fsm_ptr->vpcallback = vpcallback;
    fsm_ptr->is_random_play = FALSE;
    fsm_ptr->mode = VP_REPEAT_PLAY_MODE_CLOSE;
    fsm_ptr->strm_handle = 0;
    fsm_ptr->url_ptr = PNULL;
    fsm_ptr->dual_sys = MN_DUAL_SYS_1;
    fsm_ptr->play_type = VP_PLAY_FROM_MAX;
    fsm_ptr->support_type = VP_SUPPORTED_BOTH;
    fsm_ptr->is_to_exit = FALSE;
    fsm_ptr->is_to_pause = FALSE;
    fsm_ptr->is_to_resume = FALSE;
    fsm_ptr->is_save_history = FALSE;
    fsm_ptr->task_id = P_APP;
	fsm_ptr->is_seek_able = TRUE;	
	fsm_ptr->is_audio_coexist = s_is_audio_coexist;
	fsm_ptr->vol = MMIAPISET_GetMultimVolume();
	

    //给s_vp_fsm_list加锁    
    SCI_GetMutex(s_fsm_list_mutex, SCI_WAIT_FOREVER);
    
    //将当前状态机插入到状态机列表中
    if(s_vp_fsm_list == 0)
    {
        cur_fsm_node_ptr = (VP_FSM_LIST_T* )SCI_ALLOCA(sizeof(VP_FSM_LIST_T));
        if (PNULL != cur_fsm_node_ptr)
        {
            cur_fsm_node_ptr->fsm_info_ptr = fsm_ptr;
            cur_fsm_node_ptr->next_fsm_ptr = PNULL;
            s_vp_fsm_list = (uint32)cur_fsm_node_ptr;
            //VP_STRACE_LOW:"[VP] CreateFSM cur fsm is %x"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_1015_112_2_18_3_5_5_37,(uint8*)"d",cur_fsm_node_ptr);
        }
        else
        {
            SCI_FREE(fsm_ptr);
            fsm_ptr = PNULL;
            
            
        }
    }
    else
    {
        cur_fsm_node_ptr = (VP_FSM_LIST_T* )s_vp_fsm_list;
        while (PNULL != cur_fsm_node_ptr->next_fsm_ptr)
        {
            cur_fsm_node_ptr = cur_fsm_node_ptr->next_fsm_ptr;
        }
        
        cur_fsm_node_ptr->next_fsm_ptr = (VP_FSM_LIST_T*)SCI_ALLOCA(sizeof(VP_FSM_LIST_T));
        if(PNULL != cur_fsm_node_ptr->next_fsm_ptr)
        {
            cur_fsm_node_ptr->next_fsm_ptr->fsm_info_ptr = fsm_ptr;
            cur_fsm_node_ptr->next_fsm_ptr->next_fsm_ptr = PNULL;
            //VP_STRACE_LOW:"[VP] CreateFSM cur fsm is %x"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_1038_112_2_18_3_5_5_38,(uint8*)"d",cur_fsm_node_ptr->next_fsm_ptr);
        }
        else
        {
            SCI_FREE(fsm_ptr);
            fsm_ptr = PNULL;
            
            
        }
    }
    
    //给s_vp_fsm_list解锁
    SCI_PutMutex(s_fsm_list_mutex);
	s_current_fsm_ptr = fsm_ptr;
	SCI_TRACE_LOW("[VP] CreateFSM req play srv");
	MMIAPISET_StopAppRing();//按键音和视频声音相互影响，导致声音异常,bug 115475
	ReqPlaySrv(fsm_ptr);
    
    return (VP_HANDLE)fsm_ptr;
}

/*****************************************************************************/
//  Description : 销毁状态机
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL void DestroyFSM(VP_HANDLE vp_handle)
{
    VP_FSM_LIST_T        *current_fsm_ptr = PNULL;
    VP_FSM_LIST_T        *pre_fsm_ptr = PNULL;
    
    if (s_vp_fsm_list == PNULL)
    {
        //VP_STRACE_LOW:"[VP] DestroyFSM there is no fsm list"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_1066_112_2_18_3_5_5_39,(uint8*)"");
        return;
    }
   
    SCI_GetMutex(s_fsm_list_mutex, SCI_WAIT_FOREVER);

    //从状态机列表中查找该状态机
    current_fsm_ptr = (VP_FSM_LIST_T *)s_vp_fsm_list;
    //VP_STRACE_LOW:"[VP] DestroyFSM current_fsm_ptr = %x, current_fsm_ptr->fsm_info_ptr = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_1074_112_2_18_3_5_5_40,(uint8*)"dd",current_fsm_ptr,current_fsm_ptr->fsm_info_ptr);
    if (PNULL != current_fsm_ptr && (VP_FSM_T *)vp_handle == current_fsm_ptr->fsm_info_ptr)
    {
        //从状态机列表中删除该状态机
        s_vp_fsm_list = (uint32)(current_fsm_ptr->next_fsm_ptr);
        //释放该状态机
        if (PNULL != current_fsm_ptr->fsm_info_ptr->full_path_ptr)
        {
            SCI_FREE(current_fsm_ptr->fsm_info_ptr->full_path_ptr);
            current_fsm_ptr->fsm_info_ptr->full_path_ptr = PNULL;
        }
        if (PNULL != current_fsm_ptr->fsm_info_ptr->url_ptr)
        {
            SCI_FREE(current_fsm_ptr->fsm_info_ptr->url_ptr);
            current_fsm_ptr->fsm_info_ptr->url_ptr = PNULL;
        }
        if(0 != current_fsm_ptr->fsm_info_ptr->srv_handle)
        {
            MMISRVMGR_Free(current_fsm_ptr->fsm_info_ptr->srv_handle);
            current_fsm_ptr->fsm_info_ptr->srv_handle = 0;
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
            if ((VP_FSM_T *)vp_handle == current_fsm_ptr->fsm_info_ptr)
            {
                //从状态机列表中删除该状态机
                pre_fsm_ptr->next_fsm_ptr = current_fsm_ptr->next_fsm_ptr;
                //释放该状态机
                if (PNULL != current_fsm_ptr->fsm_info_ptr->full_path_ptr)
                {
                    SCI_FREE(current_fsm_ptr->fsm_info_ptr->full_path_ptr);
                    current_fsm_ptr->fsm_info_ptr->full_path_ptr = PNULL;
                }
                if (PNULL != current_fsm_ptr->fsm_info_ptr->url_ptr)
                {
                    SCI_FREE(current_fsm_ptr->fsm_info_ptr->url_ptr);
                    current_fsm_ptr->fsm_info_ptr->url_ptr = PNULL;
                }
                if(0 != current_fsm_ptr->fsm_info_ptr->srv_handle)
                {
                    MMISRVMGR_Free(current_fsm_ptr->fsm_info_ptr->srv_handle);
                    current_fsm_ptr->fsm_info_ptr->srv_handle = 0;
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
    //VP_STRACE_LOW:"[VP] DestroyFSM  fsm vp_handle = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_1130_112_2_18_3_5_5_41,(uint8*)"d",vp_handle);
     //给s_vp_fsm_list解锁
    SCI_PutMutex(s_fsm_list_mutex);
    s_current_fsm_ptr = PNULL;
}
/*****************************************************************************/
//  Description : 判断状态机是否有效
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsValidFsm(VP_FSM_T        *fsm_ptr)
{
    VP_FSM_LIST_T        *current_fsm_ptr = PNULL;
    BOOLEAN              result           = FALSE;
    
   
    SCI_GetMutex(s_fsm_list_mutex, SCI_WAIT_FOREVER);

    
    //在状态机列表中查找该状态机是否存在
    current_fsm_ptr = (VP_FSM_LIST_T*)s_vp_fsm_list;
    while (PNULL != current_fsm_ptr)
    {
        if (fsm_ptr == current_fsm_ptr->fsm_info_ptr)
        {
            result = TRUE;
            break;
        }
        current_fsm_ptr = current_fsm_ptr->next_fsm_ptr;
    }

    //给s_vp_fsm_list解锁
    SCI_PutMutex(s_fsm_list_mutex);
    return result;
}

/*****************************************************************************/
//  Description : 分发请求消息
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN FsmDispatch(VP_HANDLE   vp_handle,  VP_FSM_MSG_E msg_id, void *msg_body_ptr, uint32 msg_body_size)
{
    VP_FSM_T        *fsm_ptr    =  (VP_FSM_T *)vp_handle;
    BOOLEAN         result      = FALSE;
    VP_FSM_MSG_T    *req_msg_ptr = PNULL;
    
    //参数检查 
    //判断状态是否有效
    if(PNULL == fsm_ptr || !IsValidFsm(fsm_ptr) || (PNULL == msg_body_ptr && 0 != msg_body_size))
    {
        //VP_STRACE_LOW:"[VP] FsmDispatch something wrong with fsm_ptr = %x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_1179_112_2_18_3_5_5_42,(uint8*)"d",fsm_ptr);
        return FALSE;
    }

    VP_STRACE_LOW("[VP] FsmDispatch msg_id = %x, msg_body_ptr = %x, msg_body_size = %d, fsm_ptr->state =%d",msg_id,msg_body_ptr,msg_body_size,fsm_ptr->state);
    req_msg_ptr = CreatMsg(vp_handle,msg_id, msg_body_ptr,msg_body_size);    
    if (PNULL != req_msg_ptr && fsm_ptr->state < VP_STATE_MAX && PNULL != s_fsm_dispatch[fsm_ptr->state])
    {
        result = (*s_fsm_dispatch[fsm_ptr->state])(fsm_ptr,req_msg_ptr);      
        DestroyMsg(req_msg_ptr); 
    } 
    return result;
}

/*****************************************************************************/
//  Description : 转换状态
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL void FsmStateTrans(VP_FSM_T *fsm_ptr, VP_STATE_E state)
{    
    //VP_STRACE_LOW:"[VP] FsmStateTrans fsm_ptr = %x, state = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_1199_112_2_18_3_5_5_43,(uint8*)"dd",fsm_ptr,state);
    //参数检查
    if(fsm_ptr == PNULL || !IsValidFsm(fsm_ptr) || state >= VP_STATE_MAX)
    {
        return;
    }    

    //向videoplayer control发送exit消息
    FsmDispatch((VP_HANDLE)fsm_ptr,VP_EXIT_REQ, PNULL,0);       
    //转换状态
    fsm_ptr->state = state;    

    //向videoplayer control发送entry消息
    FsmDispatch((VP_HANDLE)fsm_ptr,VP_ENTRY_REQ, PNULL,0);    
    
}

/*****************************************************************************/
//  Description : 给ui层发送VP_PREPARE_IND
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL void SendPrepareIndToUI(VP_FSM_T *fsm_ptr)
{
    VP_CALLBACK_T           msg_to_ui       = {0};
    VP_PREPARE_IND_T        prepare_info    = {0};
    
    if (PNULL == fsm_ptr || !IsValidFsm(fsm_ptr) || PNULL == fsm_ptr->vpcallback)
    {
        //VP_STRACE_LOW:"[VP] SendPrepareIndToUI param is error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_1227_112_2_18_3_5_5_44,(uint8*)"");
        return;
    }
    
    //创建VP_PREPARE_IND消息
    msg_to_ui.handle = (VP_HANDLE)fsm_ptr;
    msg_to_ui.msg_id = VP_PREPARE_IND;
    msg_to_ui.para = &prepare_info;
    prepare_info.file_name_len = fsm_ptr->full_path_len;
    prepare_info.file_name_ptr = fsm_ptr->full_path_ptr;
    prepare_info.start_time = fsm_ptr->process_time;
    prepare_info.total_time = fsm_ptr->total_time;
    prepare_info.vp_handler = (VP_HANDLE)fsm_ptr;
    prepare_info.support_type = fsm_ptr->support_type;
    prepare_info.play_type = fsm_ptr->play_type;
    
    //VP_STRACE_LOW:"[VP] SendPrepareIndToUI send prepare ind to ui"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_1243_112_2_18_3_5_5_45,(uint8*)"");
    //给ui层发送VP_PREPARE_IND
    fsm_ptr->vpcallback(&msg_to_ui);
}

/*****************************************************************************/
//  Description : 给ui层发送VP_AUTH_IND
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL void SendAUTHIndToUI(VP_FSM_T *fsm_ptr)
{
#ifdef MMI_STREAMING_PLAYER_SUPPORT
    VP_CALLBACK_T       msg_to_ui   = {0};
    VP_AUTH_IND_T       auth_info   = {0};
    
    if (PNULL == fsm_ptr || !IsValidFsm(fsm_ptr) || PNULL == fsm_ptr->vpcallback)
    {
        //VP_STRACE_LOW:"[VP] SendAUTHIndToUI param is error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_1260_112_2_18_3_5_5_46,(uint8*)"");
        return;
    }
    
    //创建VP_AUTH_IND消息
    msg_to_ui.handle = (VP_HANDLE)fsm_ptr;
    msg_to_ui.msg_id = VP_AUTH_IND;
    msg_to_ui.para  = &auth_info;
    auth_info.vp_handler = (VP_HANDLE)fsm_ptr;
    //VP_STRACE_LOW:"[VP] SendAUTHIndToUI send auth ind to ui"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_1269_112_2_18_3_5_5_47,(uint8*)"");
    //给ui层发送VP_AUTH_IND
    fsm_ptr->vpcallback(&msg_to_ui);
#endif
}

/*****************************************************************************/
//  Description : 给ui层发送VP_FATAL_ERROR_IND
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL void SendFatalErrorIndTOUI(VP_FSM_T *fsm_ptr,VP_ERROR_E error_code)
{
    VP_CALLBACK_T           msg_to_ui           = {0};
    VP_FATAL_ERROR_IND_T    fatal_error_info    = {0};
    
    if (PNULL == fsm_ptr || !IsValidFsm(fsm_ptr) || PNULL == fsm_ptr->vpcallback)
    {
        //VP_STRACE_LOW:"[VP] SendFatalErrorIndTOUI param is error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_1286_112_2_18_3_5_5_48,(uint8*)"");
        return;
    }
    
    //创建VP_FATAL_ERROR_IND消息
    msg_to_ui.handle = (VP_HANDLE)fsm_ptr;
    msg_to_ui.msg_id = VP_FATAL_ERROR_IND;
    msg_to_ui.para = &fatal_error_info;
    fatal_error_info.vp_handler = (VP_HANDLE)fsm_ptr;
    fatal_error_info.error_code = error_code;
    //VP_STRACE_LOW:"[VP] SendFatalErrorIndTOUI send fatal error to ui error_code = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_1296_112_2_18_3_5_5_49,(uint8*)"d",error_code);

    //给ui层发送VP_FATAL_ERROR_IND
    fsm_ptr->vpcallback(&msg_to_ui);
}

/*****************************************************************************/
//  Description : 给ui层发送VP_PLAY_PROCESS_IND
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL void SendProcessIndToUI(VP_FSM_T *fsm_ptr)
{
    VP_CALLBACK_T               msg_to_ui       = {0};
    VP_PLAY_PROCESS_IND_T       process_ind   = {0};
    
    if (PNULL == fsm_ptr || !IsValidFsm(fsm_ptr) || PNULL == fsm_ptr->vpcallback)
    {
        //VP_STRACE_LOW:"[VP] SendProcessIndToUI param is error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_1313_112_2_18_3_5_5_50,(uint8*)"");
        return;
    }
    
    //创建VP_PLAY_PROCESS_IND消息
    msg_to_ui.handle = (VP_HANDLE)fsm_ptr;
    msg_to_ui.msg_id = VP_PLAY_PROCESS_IND;
    msg_to_ui.para  = &process_ind;
    process_ind.vp_handler = (VP_HANDLE)fsm_ptr;
    process_ind.process_time = fsm_ptr->process_time;
    process_ind.total_time = fsm_ptr->total_time;  
    //给ui层发送VP_PLAY_PROCESS_IND
    fsm_ptr->vpcallback(&msg_to_ui);
}

/*****************************************************************************/
//  Description : 给ui层发送VP_BUFFERING_IND
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL void SendBufferingIndToUI(VP_FSM_T *fsm_ptr)
{
#ifdef MMI_STREAMING_PLAYER_SUPPORT
    VP_CALLBACK_T       msg_to_ui   = {0};
    VP_BUFFERING_IND_T       buffering_ind   = {0};
    
    if (PNULL == fsm_ptr || !IsValidFsm(fsm_ptr) || PNULL == fsm_ptr->vpcallback)
    {
        //VP_STRACE_LOW:"[VP] SendBufferingIndToUI param is error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_1340_112_2_18_3_5_5_51,(uint8*)"");
        return;
    }
    
    //创建VP_BUFFERING_IND消息
    msg_to_ui.handle = (VP_HANDLE)fsm_ptr;
    msg_to_ui.msg_id = VP_BUFFERING_IND;
    msg_to_ui.para  = &buffering_ind;
    buffering_ind.vp_handler = (VP_HANDLE)fsm_ptr;
    //VP_STRACE_LOW:"[VP] SendBufferingIndToUI send VP_BUFFERING_IND to ui"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_1349_112_2_18_3_5_5_52,(uint8*)"");
    //给ui层发送VP_BUFFERING_IND
    fsm_ptr->vpcallback(&msg_to_ui);
#endif
}

/*****************************************************************************/
//  Description : 给ui层发送VP_BUFFER_PROCESS_IND
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL void SendBufferPercentToUI(VP_FSM_T *fsm_ptr)
{
#ifdef MMI_STREAMING_PLAYER_SUPPORT	
    VP_CALLBACK_T       msg_to_ui   = {0};
    VP_BUFFERING_PROCESS_IND_T       buffering_process_info   = {0};
    
    if (PNULL == fsm_ptr || !IsValidFsm(fsm_ptr) || PNULL == fsm_ptr->vpcallback)
    {
        //VP_STRACE_LOW:"[VP] SendBufferEndIndToUI param is error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_1367_112_2_18_3_5_5_53,(uint8*)"");
        return;
    }
    
    //创建VP_BUFFER_END_IND消息
    buffering_process_info.buffer_percent = fsm_ptr->buf_percent;
    msg_to_ui.handle = (VP_HANDLE)fsm_ptr;
    msg_to_ui.msg_id = VP_BUFFER_PROCESS_IND;
    msg_to_ui.para  = &buffering_process_info;
    buffering_process_info.vp_handler = (VP_HANDLE)fsm_ptr;
    //VP_STRACE_LOW:"[VP] SendAUTHIndToUI send VP_BUFFER_END_IND to ui"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_1377_112_2_18_3_5_5_54,(uint8*)"");
    //给ui层发送VP_BUFFER_END_IND
    fsm_ptr->vpcallback(&msg_to_ui);
#endif
}

/*****************************************************************************/
//  Description : 给ui层发送VP_BUFFER_END_IND
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL void SendBufferEndIndToUI(VP_FSM_T *fsm_ptr)
{
#ifdef MMI_STREAMING_PLAYER_SUPPORT
    VP_CALLBACK_T       msg_to_ui   = {0};
    VP_BUFFER_END_IND_T       buffering_end_info   = {0};
    
    if (PNULL == fsm_ptr || !IsValidFsm(fsm_ptr) || PNULL == fsm_ptr->vpcallback)
    {
        //VP_STRACE_LOW:"[VP] SendBufferEndIndToUI param is error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_1395_112_2_18_3_5_5_55,(uint8*)"");
        return;
    }
    
    //创建VP_BUFFER_END_IND消息
    msg_to_ui.handle = (VP_HANDLE)fsm_ptr;
    msg_to_ui.msg_id = VP_BUFFER_END_IND;
    msg_to_ui.para  = &buffering_end_info;
    buffering_end_info.vp_handler = (VP_HANDLE)fsm_ptr;
    //VP_STRACE_LOW:"[VP] SendAUTHIndToUI send VP_BUFFER_END_IND to ui"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_1404_112_2_18_3_5_5_56,(uint8*)"");
    //给ui层发送VP_BUFFER_END_IND
    fsm_ptr->vpcallback(&msg_to_ui);
#endif
}

/*****************************************************************************/
//  Description : 给ui层发送VP_PLAY_END_IND
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL void SendPlayEndToUI(VP_FSM_T *fsm_ptr, VP_PLAY_END_RESULT_E result)
{
    VP_CALLBACK_T       msg_to_ui   = {0};
    VP_PLAY_END_IND_T       play_end   = {0};
    
    if (PNULL == fsm_ptr || !IsValidFsm(fsm_ptr) || PNULL == fsm_ptr->vpcallback)
    {
        //VP_STRACE_LOW:"[VP] SendPlayEndToUI param is error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_1421_112_2_18_3_5_5_57,(uint8*)"");
        return;
    }
    
    //创建VP_PLAY_END_IND消息
    msg_to_ui.handle = (VP_HANDLE)fsm_ptr;
    msg_to_ui.msg_id = VP_PLAY_END_IND;
    msg_to_ui.para  = &play_end;
    play_end.vp_handler = (VP_HANDLE)fsm_ptr;
    play_end.result = result;
    //VP_STRACE_LOW:"[VP] SendAUTHIndToUI send VP_PLAY_END_IND to ui"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_1431_112_2_18_3_5_6_58,(uint8*)"");
    //给ui层发送VP_PLAY_END_IND
    fsm_ptr->vpcallback(&msg_to_ui);
}

/*****************************************************************************/
//  Description : 给ui层发送VP_STOP_IND
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL void SendStopIndToUI(VP_FSM_T *fsm_ptr)
{
    VP_CALLBACK_T       msg_to_ui   = {0};
    VP_STOP_IND_T       stop_ind    = {0};
    
    if (PNULL == fsm_ptr || !IsValidFsm(fsm_ptr) || PNULL == fsm_ptr->vpcallback)
    {
        //VP_STRACE_LOW:"[VP] SendStopIndToUI param is error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_1447_112_2_18_3_5_6_59,(uint8*)"");
        return;
    }

//    if(0 != fsm_ptr->srv_handle)
//    {
//        MMISRVMGR_Free(fsm_ptr->srv_handle);
//        fsm_ptr->srv_handle = 0;
//    }

    //创建VP_STOP_IND消息
    msg_to_ui.handle = (VP_HANDLE)fsm_ptr;
    msg_to_ui.msg_id = VP_STOP_IND;
    msg_to_ui.para  = &stop_ind;
    stop_ind.vp_handler = (VP_HANDLE)fsm_ptr;
    //VP_STRACE_LOW:"[VP] SendAUTHIndToUI send VP_STOP_IND to ui"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_1462_112_2_18_3_5_6_60,(uint8*)"");
    //给ui层发送VP_STOP_IND
    fsm_ptr->vpcallback(&msg_to_ui);
}

/*****************************************************************************/
//  Description : 给ui层发送VP_PLAY_CNF
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL void SendPlayCnfToUI(VP_FSM_T *fsm_ptr,VP_ERROR_E error_code)
{
    VP_CALLBACK_T       msg_to_ui   = {0};
    VP_PLAY_CNF_T       play_cnf   = {0};
    
    if (PNULL == fsm_ptr || !IsValidFsm(fsm_ptr) || PNULL == fsm_ptr->vpcallback)
    {
        //VP_STRACE_LOW:"[VP] SendPlayCnfToUI param is error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_1478_112_2_18_3_5_6_61,(uint8*)"");
        return;
    }
    
    //创建VP_PLAY_CNF消息
    msg_to_ui.handle = (VP_HANDLE)fsm_ptr;
    msg_to_ui.msg_id = VP_PLAY_CNF;
    msg_to_ui.para  = &play_cnf;
    play_cnf.vp_handler = (VP_HANDLE)fsm_ptr;
    play_cnf.error_code = error_code;
    //VP_STRACE_LOW:"[VP] SendPlayCnfToUI send VP_PLAY_CNFto ui error_code = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_1488_112_2_18_3_5_6_62,(uint8*)"d",error_code);
    //给ui层发送VP_PLAY_CNF
    fsm_ptr->vpcallback(&msg_to_ui);
}

/*****************************************************************************/
//  Description : 给ui层发送VP_PAUSE_CNF
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL void SendPauseCnfToUI(VP_FSM_T *fsm_ptr,VP_ERROR_E error_code)
{
    VP_CALLBACK_T       msg_to_ui   = {0};
    VP_PAUSE_CNF_T      pause_cnf   = {0};
    
    if (PNULL == fsm_ptr || !IsValidFsm(fsm_ptr) || PNULL == fsm_ptr->vpcallback)
    {
        //VP_STRACE_LOW:"[VP] SendPauseCnfToUI param is error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_1504_112_2_18_3_5_6_63,(uint8*)"");
        return;
    }
    
    //创建VP_PAUSE_CNF消息
    msg_to_ui.handle = (VP_HANDLE)fsm_ptr;
    msg_to_ui.msg_id = VP_PAUSE_CNF;
    msg_to_ui.para  = &pause_cnf;
    pause_cnf.vp_handler = (VP_HANDLE)fsm_ptr;
    pause_cnf.error_code = error_code;
    //VP_STRACE_LOW:"[VP] SendAUTHIndToUI send VP_PAUSE_CNF to ui"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_1514_112_2_18_3_5_6_64,(uint8*)"");
    //给ui层发送VP_PAUSE_CNF
    fsm_ptr->vpcallback(&msg_to_ui);
}

/*****************************************************************************/
//  Description : 给ui层发送VP_SEEK_CNF
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL void SendSeekCnfToUI(VP_FSM_T *fsm_ptr,VP_ERROR_E error_code)
{
    VP_CALLBACK_T       msg_to_ui   = {0};
    VP_SEEK_CNF_T       seek_cnf    = {0};
    
    if (PNULL == fsm_ptr || !IsValidFsm(fsm_ptr) || PNULL == fsm_ptr->vpcallback)
    {
        //VP_STRACE_LOW:"[VP] SendSeekCnfToUI param is error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_1530_112_2_18_3_5_6_65,(uint8*)"");
        return;
    }
    
    //创建VP_SEEK_CNF消息
    msg_to_ui.handle = (VP_HANDLE)fsm_ptr;
    msg_to_ui.msg_id = VP_SEEK_CNF;
    msg_to_ui.para  = &seek_cnf;
    seek_cnf.vp_handler = (VP_HANDLE)fsm_ptr;
    seek_cnf.cur_time = fsm_ptr->process_time;
    seek_cnf.total_time = fsm_ptr->total_time;
    seek_cnf.error_code = error_code;
    //VP_STRACE_LOW:"[VP] SendAUTHIndToUI send VP_SEEK_CNF to ui"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_1542_112_2_18_3_5_6_66,(uint8*)"");
    //给ui层发送VP_SEEK_CNF
    fsm_ptr->vpcallback(&msg_to_ui);
}

/*****************************************************************************/
//  Description : SendSignalToAPP
//  Global resource dependence : 
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL void SendSignalToCurPlayingTask(uint16 signalType, uint16 data, uint32 task_id)
{
    MmiAppCopyMsgS *sendSignal = PNULL;    
  //  VP_STRACE_LOW("[VP] SendSignalToCurPlayingTask signalType = %d, data = %d, task_id = %d",signalType,data,task_id);

    MmiCreateSignal(signalType, sizeof(MmiAppCopyMsgS), (MmiSignalS**)&sendSignal);   
    sendSignal->data = data;
    MmiSendSignal(task_id, (MmiSignalS*)sendSignal);

   
}

/*****************************************************************************/
//  Description : Get frame data call back
//  Global resource dependence : 
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
LOCAL DPLAYER_RETURN_E GetFrameDataCallback(DPLAYER_IMAGE_DATA_T *src_frame_ptr)
{
    DPLAYER_RETURN_E result = DPLAYER_SUCCESS;

    if(PNULL == s_current_fsm_ptr || PNULL == src_frame_ptr)
    {
        return DPLAYER_ERROR;
    }

    //SCI_TRACE_LOW:"[VP]:GetFrameDataCallback enter!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_1576_112_2_18_3_5_6_67,(uint8*)"");
    SCI_MEMCPY(&s_current_fsm_ptr->image_data,src_frame_ptr,sizeof(DPLAYER_IMAGE_DATA_T));
    SendSignalToCurPlayingTask(APP_VP_FRAME_END_IND, 0, s_current_fsm_ptr->task_id );

    return result;
}

/*****************************************************************************/
//  Description : frame end notify call back
//  Global resource dependence : 
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL void CallbackFrameEndNotify(int32 frame_num)
{
    if (PNULL != s_current_fsm_ptr)
    {
        if(s_current_fsm_ptr->is_cr_video_layer)
        {
            SendSignalToCurPlayingTask(APP_VP_FRAME_END_NEED_DATA, (uint16)frame_num, s_current_fsm_ptr->task_id );   
        }
        else
        {
            SendSignalToCurPlayingTask(APP_VP_FRAME_END_IND, (uint16)frame_num, s_current_fsm_ptr->task_id );   
        }  
    }   
}

/*****************************************************************************/
//  Description : seek end notify call back
//  Global resource dependence : 
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL void CallbackSeekEndNotify(void)
{
    if (PNULL != s_current_fsm_ptr)
    {
        MMIVP_ResetSetSeekTime();
    }
}

/*****************************************************************************/
//  Description : end notify回调函数
//  Global resource dependence : 
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
//LOCAL void CallbackEndNotify(int32 i_type)
//{
//    VP_PLAY_END_RESULT_E  playend_result = VP_PLAY_END_NONE;
//   
//    VP_STRACE_LOW("[VP] CallbackEndNotify i_type = %d, s_current_fsm_ptr->task_id = %d",i_type,s_current_fsm_ptr->task_id);
//  
//    if (0 == i_type)
//    {
//        playend_result = VP_PLAY_END_END_SUCCESS;
//    }
//    else if (0 > i_type)
//    {
//        if (-11 == i_type)
//        {
//            playend_result = VP_PLAY_END_NOT_SUPPORT;
//        }
//        else
//        {
//            playend_result = VP_PLAY_END_ERROR;
//        }
//    }
//    else
//    {
//        playend_result = VP_PLAY_END_STOPPED_BY_USER;
//    }  
//    
//    if (PNULL != s_current_fsm_ptr)
//    {
//        SendSignalToCurPlayingTask(APP_VP_END_IND, (uint16)playend_result, s_current_fsm_ptr->task_id );   
//    }      
//}

/*****************************************************************************/
//  Description : 通过frame获得对应的time
//  Global resource dependence : 
//  Author: apple.zhang
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
            total_frame = (media_info.video_info.frame_rate)*time_lenth/VP_SEC_TO_MSEL/media_info.video_info.frame_rate_base;
            //计算对应的time
            if((frame < total_frame) && (0 != media_info.video_info.frame_rate))
            {
                time = frame * VP_SEC_TO_MSEL*media_info.video_info.frame_rate_base /media_info.video_info.frame_rate;
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

    //VP_STRACE_LOW:"[VP]FrameToTime handle = 0x%x, info_result = %d,frame_index = %d, time_offset = 0x%x, time_lenth = %d,base = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_1678_112_2_18_3_5_6_68,(uint8*)"dddddd",video_handle,info_result,frame,time,time_lenth,media_info.video_info.frame_rate_base);
    return time;
}
/*****************************************************************************/
//  Description : 开始播放本地文件
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL VP_ERROR_E PlayLocalFile(VP_FSM_T *fsm_ptr, BOOLEAN is_need_fill)
{
    void                    *argv[2]        = {0}; 
    DPLAYER_MEDIA_INFO_T    p_info      = {0};  
    VP_ERROR_E                 result      = VP_ERROR_OTHERS;
    DPLAYER_RETURN_E        dplayer_result      = DPLAYER_SUCCESS;
   // GUI_RECT_T lcd_rect = {0};
  //  GUI_LCD_DEV_INFO const *lcd_dev_info_ptr = (const GUI_LCD_DEV_INFO*)MMITHEME_GetDefaultLcdDev();
    VP_DISPLAY_PARAM_T  display_param = {0};
    uint32 file_size = 0;
#ifdef DRM_SUPPORT
    DRMFILE_PRE_CHECK_STATUS_E drm_status = DRMFILE_PRE_CHECK_NORMAL;
#endif
    MMISRVAUD_RET_E ret = MMISRVAUD_RET_ERROR;
    
    if(PNULL == fsm_ptr || !IsValidFsm(fsm_ptr))
    { 
        //VP_STRACE_LOW:"[VP] PlayLocalFile fsm is error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_1701_112_2_18_3_5_6_69,(uint8*)"");
        return result;
    }
 #ifdef  VIDEOTHUMBNAIL_SUPPORT  
     MMIAPIVP_ThumbnailClose();
#endif
    MMIAPIFMM_GetFileInfo(fsm_ptr->full_path_ptr,fsm_ptr->full_path_len,&file_size,PNULL,PNULL);

    //VP_STRACE_LOW:"[VP] PlayLocalFile full_path_len = %d, full_path_ptr = %x, %x, %x, %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_1709_112_2_18_3_5_6_70,(uint8*)"ddddd",fsm_ptr->full_path_len,fsm_ptr->full_path_ptr[0],fsm_ptr->full_path_ptr[1],fsm_ptr->full_path_ptr[2],fsm_ptr->full_path_ptr[3]);
    if (!MMIAPIFMM_IsFileExist(fsm_ptr->full_path_ptr,fsm_ptr->full_path_len))
    {
        fsm_ptr->support_type = VP_SUPPORTED_NEITHER;
        fsm_ptr->process_time = 0;
        fsm_ptr->total_time = 0;        
        SendPrepareIndToUI(fsm_ptr);
        result = VP_ERROR_NOT_FOUND;
        return result;
    }
    else if (0 == file_size)
    {
        fsm_ptr->support_type = VP_SUPPORTED_NEITHER;
        fsm_ptr->process_time = 0;
        fsm_ptr->total_time = 0;        
        SendPrepareIndToUI(fsm_ptr);
        result = VP_ERROR_NULL_FILE;
        return result;
    }
#ifdef DRM_SUPPORT
    else
    {
        if(MMIAPIDRM_PreCheckFileStatusEX(fsm_ptr->full_path_ptr, DRM_PERMISSION_PLAY,&drm_status))
        {
            //MMIAPIVP_FSMStop((VP_HANDLE)fsm_ptr); 
            return result;
        }
        else
        {
            if(DRMFILE_PRE_CHECK_NO_SYS_NITZ == drm_status)
            {
                result = VP_ERROR_DRM_NO_SYS_NITZ;
                return result;
            }
            else if(DRMFILE_PRE_CHECK_NO_RIGHTS == drm_status)
            {
                result = VP_ERROR_DRM_RIGHT_INVALID;
                return result;
            }
        }
    }
#endif
    //打开本地媒体文件
    argv[0] = (void*)(fsm_ptr->full_path_ptr);
    if(fsm_ptr->is_share_play)
    {
        argv[1] = (void *)((uint32)DPLAYER_FA_MOD_ABPLUSE);
    }
    else
    {
        argv[1] = (void *)((uint32)DPLAYER_FA_MOD_RB);
    }
   
    DPLAYER_Init();

#ifdef MMI_KING_MOVIE_SUPPORT
    fsm_ptr->sec_handle = VP_dplayerOpenSecurty(argv[0],&(fsm_ptr->b_sprd_kmv), PNULL);//andy.he_kingmv
#endif
    fsm_ptr->dplayer_handle = DPLAYER_Open(DPLAYER_OPEN_FROM_FILE, 2, argv);    
    //VP_STRACE_LOW:"[VP] PlayLocalFile fsm_ptr->dplayer_handle = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_1768_112_2_18_3_5_6_71,(uint8*)"d",fsm_ptr->dplayer_handle);
    
    if(0 < fsm_ptr->dplayer_handle)
    {
#ifdef MMI_KING_MOVIE_SUPPORT   
        VP_dplayerExtCtlAddSecurty(fsm_ptr ->full_path_ptr,fsm_ptr->dplayer_handle);
#endif        
        //获取当前媒体文件总的播放时间和当前的播放时间
        fsm_ptr->total_time = DPLAYER_GetLength(fsm_ptr->dplayer_handle);
#ifdef MMI_VIDEOPLAYER_MINI_FUNCTION
         fsm_ptr->playback_mode = VP_PLAYBACK_MODE_AUDIO_VIDEO_SYN;
#else
        fsm_ptr->playback_mode = VP_Setting_GetPlaybackMode();
#endif
        fsm_ptr->process_time = 0;
        dplayer_result = DPLAYER_GetMovieInfo(fsm_ptr->dplayer_handle,&p_info);
        if (DPLAYER_ERROR_UNSUPPORTED_FORMAT == dplayer_result)
        {
            fsm_ptr->support_type = VP_SUPPORTED_NEITHER;
        }
        else 
        {
            fsm_ptr->support_type = CheckSupportedTypeFromMediaInfo(&p_info);
        }
        fsm_ptr->is_seek_able = p_info.b_is_seekable;
        fsm_ptr->display_param.LcdBlkId = 0;
        fsm_ptr->display_param.LcdId = 0;
        if(fsm_ptr->is_cr_video_layer)
        {
            fsm_ptr->display_param.disp_data_op = DPLAYER_DISP_DATA_GET;
            fsm_ptr->display_param.b_no_osd = 1;
        }
        else
        {
            fsm_ptr->display_param.disp_data_op = DPLAYER_DISP_DATA_OP_NONE;
#ifdef MMI_VIDEOPLAYER_MINI_FUNCTION
            fsm_ptr->display_param.b_no_osd = 1;
#else
            fsm_ptr->display_param.b_no_osd = 0;
#endif
        }
       // fsm_ptr->display_param.disp_mode = DPLAYER_DISP_FULLVIEW;//DPLAYER_DISP_CUSTOMIZE;
        
        VP_STRACE_LOW("[VP] PlayLocalFile play_param_ptr->is_scal_up = %d, fsm_ptr->display_param.Width = %d, p_info.video_info.width = %d, fsm_ptr->display_param.Height = %d, p_info.video_info.height = %d, seek = %d",fsm_ptr->is_scal_up,fsm_ptr->display_param.disp_rect.dx,p_info.video_info.width,fsm_ptr->display_param.disp_rect.dy,p_info.video_info.height,fsm_ptr->is_seek_able);
        //SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_1793_112_2_18_3_5_6_72,(uint8*)"dddddd",fsm_ptr->is_scal_up,fsm_ptr->display_param.disp_rect.dx,p_info.video_info.width,fsm_ptr->display_param.disp_rect.dy,p_info.video_info.height,fsm_ptr->is_seek_able);
     
        
        //给ui发送VP_PREPARE_IND消息
        SendPrepareIndToUI(fsm_ptr);

        if (VP_SUPPORTED_NEITHER == fsm_ptr->support_type)
        {
            result = VP_ERROR_NOT_SUPPORT;           
        }
        //VP_STRACE_LOW:"[VP] PlayLocalFile total_time = %d, process_time = %d, support_type = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_1803_112_2_18_3_5_6_73,(uint8*)"ddd",fsm_ptr->total_time,fsm_ptr->process_time,fsm_ptr->support_type);
        //设置每帧显示完毕后的回调函数
        DPLAYER_SetFrameEndNotify(fsm_ptr->dplayer_handle, CallbackFrameEndNotify);
        SetPlaybackMode(fsm_ptr,fsm_ptr->playback_mode);
        //设置播放结束的回调函数
        //DPLAYER_SetEndNotify(fsm_ptr->dplayer_handle, CallbackEndNotify);       
        
        //设置播放显示参数      
        if (is_need_fill
            && (VP_SUPPORTED_NEITHER == fsm_ptr->support_type
            || VP_SUPPORTED_AUDIO_ONLY == fsm_ptr->support_type))
        {
             is_need_fill = FALSE;
        }

        SCI_MEMCPY(&display_param,&(fsm_ptr->display_param),sizeof(VP_DISPLAY_PARAM_T));
        
        //根据屏幕自适应角度
        if(fsm_ptr->is_cr_video_layer)
        {
            //SCI_TRACE_LOW:"[VP] PlayLocalFile is_cr_video_layer"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_1823_112_2_18_3_5_6_74,(uint8*)"");
            }
            else
            {
            MMIAPIVP_TranslateDisplayParamLogicToPhysical(&display_param, is_need_fill);
        }
        dplayer_result      = DPLAYER_SetDisplayParam(fsm_ptr->dplayer_handle,&display_param);
        
		//开始播放
        if(DPLAYER_SUCCESS == dplayer_result)            
        {
            ReqPlaySrv(fsm_ptr);
            ret = MMISRVAUD_Play(fsm_ptr->srv_handle,0);
            if(ret)
            {
                //VP_STRACE_LOW:"[VP] PlayLocalFile fsm_ptr->is_save_history = %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_1837_112_2_18_3_5_6_75,(uint8*)"d",fsm_ptr->is_save_history);
#ifdef DRM_SUPPORT
                if(MMISRVAUD_RET_OK== ret)
                {
                    ConsumeDrmRight(s_current_fsm_ptr->full_path_ptr,s_current_fsm_ptr->full_path_len);
                }
#endif        
#ifdef MMI_VIDEOPLAYER_HISTORYLIST_SUPPORT 
				if (fsm_ptr->is_save_history)
                {        
/*#ifdef MMI_KING_MOVIE_SUPPORT
                    if(!MMIAPIKM_IsSupportfile() && !MMIAPISM_IsSupportfile() //smv_support
#ifdef MMI_KING_MOVIE_SUPPORT_HMV
                        && !MMIAPIKM_IsHmvfile()
#endif
                        )
#endif*/
                    {

                    VP_HistoryList_ModifyItem(                                
                            fsm_ptr->process_time,
                            fsm_ptr->total_time,
                            p_info.video_info.height,
                            p_info.video_info.width,
                            p_info.media_length,
                            PNULL,
                            0);   
                    }  
                }
#endif //MMI_VIDEOPLAYER_HISTORYLIST_SUPPORT
                result = VP_ERROR_NONE;
            }
        }
        if(result != VP_ERROR_NONE)
        {
            //VP_STRACE_LOW:"[VP] PlayLocalFile play error"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_1865_112_2_18_3_5_6_76,(uint8*)"");
#ifdef  MMI_KING_MOVIE_SUPPORT 
            VP_dplayerExtCtlDeltSecurty(fsm_ptr ->full_path_ptr,fsm_ptr->dplayer_handle, fsm_ptr->sec_handle);
#endif
            DPLAYER_Close(fsm_ptr->dplayer_handle);
#ifdef  MMI_KING_MOVIE_SUPPORT 
            VP_dplayerCloseSecurty(fsm_ptr ->full_path_ptr,fsm_ptr->sec_handle,fsm_ptr->b_sprd_kmv, PNULL);
            fsm_ptr->sec_handle = PNULL;
#endif

            fsm_ptr->dplayer_handle = 0;           
            DPLAYER_Release();
        }
    }
    else
    {
        fsm_ptr->support_type = VP_SUPPORTED_NEITHER;
        fsm_ptr->process_time = 0;
        fsm_ptr->total_time = 0;        
        SendPrepareIndToUI(fsm_ptr);
        result = VP_ERROR_NOT_SUPPORT;
#ifdef  MMI_KING_MOVIE_SUPPORT 
        VP_dplayerCloseSecurty(fsm_ptr ->full_path_ptr,fsm_ptr->sec_handle,fsm_ptr->b_sprd_kmv, PNULL);
        fsm_ptr->sec_handle = PNULL;
#endif
    }
    return result;
}

/*****************************************************************************/
//  Description : 开始播放本地buf
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL VP_ERROR_E PlayLocalBuf(VP_FSM_T *fsm_ptr, VP_PLAY_PARAM_T  *play_param_ptr)
{
    void                    *argv[3]            = {0}; 
    DPLAYER_FILE_TYPE_E     file_type           = DPLAYER_FILETYPE_3GP;
    DPLAYER_MEDIA_INFO_T    p_info              = {0};
    VP_ERROR_E                 result              = VP_ERROR_OTHERS;
    DPLAYER_RETURN_E        dplayer_result      = DPLAYER_SUCCESS;
   // GUI_RECT_T lcd_rect = {0};
  //  GUI_LCD_DEV_INFO const *lcd_dev_info_ptr = (const GUI_LCD_DEV_INFO*)MMITHEME_GetDefaultLcdDev();
    BOOLEAN                  is_need_fill       = TRUE;    
    VP_DISPLAY_PARAM_T          play_param         = {0};

    //参数检查
    if ((PNULL == fsm_ptr) || !IsValidFsm(fsm_ptr) || (PNULL == play_param_ptr))
    {
        //VP_STRACE_LOW:"[VP] PlayLocalBuf param is error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_1913_112_2_18_3_5_6_77,(uint8*)"");
        return result;
    }
 #ifdef  VIDEOTHUMBNAIL_SUPPORT      
     MMIAPIVP_ThumbnailClose();
#endif    
    /* open mplayer */
    argv[0] = (void*)(play_param_ptr->video_buf_info.video_buf_ptr);
    argv[1] = (void*)play_param_ptr->video_buf_info.video_buf_len;

    //VP_STRACE_LOW:"[VP] PlayLocalBuf play_param_ptr->video_buf_info.type = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_1923_112_2_18_3_5_7_78,(uint8*)"d",play_param_ptr->video_buf_info.type);
 
   
    switch(play_param_ptr->video_buf_info.type)
    {
    case MMICOM_VIDEO_TYPE_AVI:
        file_type = DPLAYER_FILETYPE_AVI;
        break;
    case MMICOM_VIDEO_TYPE_3GP:
        file_type = DPLAYER_FILETYPE_3GP;
        break;
    case MMICOM_VIDEO_TYPE_MP4:
        file_type = DPLAYER_FILETYPE_MP4;
        break;
    case MMICOM_VIDEO_TYPE_FLV:
        file_type = DPLAYER_FILETYPE_FLV;
        break;
//    case MMICOM_VIDEO_TYPE_RM:
    case MMICOM_VIDEO_TYPE_RMVB:
        file_type = DPLAYER_FILETYPE_RMVB;
        break;
        
    default:
        return VP_ERROR_NOT_SUPPORT;
    }
    
    argv[2] = &file_type;
    DPLAYER_Init();
    fsm_ptr->dplayer_handle = DPLAYER_Open(DPLAYER_OPEN_FROM_MEM, 3, argv);
    //VP_STRACE_LOW:"[VP] PlayLocalBuf fsm_ptr->dplayer_handle = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_1952_112_2_18_3_5_7_79,(uint8*)"d",fsm_ptr->dplayer_handle);
    if(0 < fsm_ptr->dplayer_handle)
    {
        //获取当前媒体文件总的播放时间和当前的播放时间
        fsm_ptr->total_time = DPLAYER_GetLength(fsm_ptr->dplayer_handle);
        fsm_ptr->process_time = DPLAYER_GetTime(fsm_ptr->dplayer_handle);
        dplayer_result = DPLAYER_GetMovieInfo(fsm_ptr->dplayer_handle,&p_info);
        if (DPLAYER_ERROR_UNSUPPORTED_FORMAT == dplayer_result)
        {
            fsm_ptr->support_type = VP_SUPPORTED_NEITHER;
        }        
        else 
        {
            fsm_ptr->support_type = CheckSupportedTypeFromMediaInfo(&p_info);
        }      
        fsm_ptr->is_seek_able = p_info.b_is_seekable;
        //VP_STRACE_LOW:"[VP] PlayLocalBuf vidot_type = %d, vidoe_support = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_1967_112_2_18_3_5_7_80,(uint8*)"dd",p_info.video_info.video_type,p_info.video_info.video_support,fsm_ptr->is_seek_able);

        fsm_ptr->display_param.LcdBlkId = 0;
        fsm_ptr->display_param.LcdId = 0;
        if(fsm_ptr->is_cr_video_layer)
        {
            fsm_ptr->display_param.disp_data_op = DPLAYER_DISP_DATA_GET;
            fsm_ptr->display_param.b_no_osd = 1;
        }
        else
        {
            fsm_ptr->display_param.disp_data_op = DPLAYER_DISP_DATA_OP_NONE;
#ifdef MMI_VIDEOPLAYER_MINI_FUNCTION
            fsm_ptr->display_param.b_no_osd = 1;
#else
            fsm_ptr->display_param.b_no_osd = 0;
#endif

        }
    //    fsm_ptr->display_param.disp_mode = DPLAYER_DISP_CUSTOMIZE;
        //VP_STRACE_LOW:"[VP] PlayLocalBuf play_param_ptr->is_scal_up = %d, fsm_ptr->display_param.Width = %d, p_info.video_info.width = %d, fsm_ptr->display_param.Height = %d, p_info.video_info.height = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_1972_112_2_18_3_5_7_81,(uint8*)"ddddd",play_param_ptr->is_scal_up,fsm_ptr->display_param.disp_rect.dx,p_info.video_info.width,fsm_ptr->display_param.disp_rect.dy,p_info.video_info.height);
      /*  if (!(play_param_ptr->is_scal_up) && fsm_ptr->display_param.Width >= p_info.video_info.width && fsm_ptr->display_param.Height >= p_info.video_info.height)
        {
            fsm_ptr->display_param.xOffset = fsm_ptr->display_param.xOffset + ((fsm_ptr->display_param.Width - p_info.video_info.width) >> 1);
            fsm_ptr->display_param.yOffset = fsm_ptr->display_param.yOffset + ((fsm_ptr->display_param.Height - p_info.video_info.height) >> 1);
            fsm_ptr->display_param.Width = p_info.video_info.width;
            fsm_ptr->display_param.Height = p_info.video_info.height;
        }*/

        //给ui发送VP_PREPARE_IND消息
        SendPrepareIndToUI(fsm_ptr);        
        
        //VP_STRACE_LOW:"[VP] PlayLocalBuf total_time = %d, process_time = %d, support_type = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_1984_112_2_18_3_5_7_82,(uint8*)"ddd",fsm_ptr->total_time,fsm_ptr->process_time,fsm_ptr->support_type);
        
        //设置每帧显示完毕后的回调函数
        DPLAYER_SetFrameEndNotify(fsm_ptr->dplayer_handle, CallbackFrameEndNotify);
        
        //设置播放结束的回调函数
        //DPLAYER_SetEndNotify(fsm_ptr->dplayer_handle, CallbackEndNotify);        

        if (VP_SUPPORTED_NEITHER == fsm_ptr->support_type
            || VP_SUPPORTED_AUDIO_ONLY == fsm_ptr->support_type)
        {
             is_need_fill = FALSE;
        }

        SCI_MEMCPY(&play_param,&(fsm_ptr->display_param),sizeof(VP_DISPLAY_PARAM_T));
        MMIAPIVP_TranslateDisplayParamLogicToPhysical(&play_param, is_need_fill);

        dplayer_result = DPLAYER_SetDisplayParam(fsm_ptr->dplayer_handle,&play_param);
        //开始播放
        if (DPLAYER_SUCCESS == dplayer_result)
        {
            ReqPlaySrv(fsm_ptr);
            if(MMISRVAUD_Play(fsm_ptr->srv_handle,0))
            {
                //VP_STRACE_LOW:"[VP] PlayLocalBuf play success"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_2008_112_2_18_3_5_7_83,(uint8*)"");
                result = VP_ERROR_NONE;
            }
        }
        if (result != VP_ERROR_NONE)
        {
            //MMISRVMGR_Free(fsm_ptr->srv_handle);
            //fsm_ptr->srv_handle = 0;
            //VP_STRACE_LOW:"[VP] PlayLocalBuf play error"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_2016_112_2_18_3_5_7_84,(uint8*)"");
            DPLAYER_Close(fsm_ptr->dplayer_handle);
            fsm_ptr->dplayer_handle = 0;    
            DPLAYER_Release();
        }
    }
    else
    {
        SendPrepareIndToUI(fsm_ptr);
        result = VP_ERROR_NOT_SUPPORT;
    }
    return result;
}

/*****************************************************************************/
//  Description : 开始播放网络文件
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL VP_ERROR_E PlayFromNet(VP_FSM_T *fsm_ptr,DPLAYER_STRM_CFG_T strm_cfg)
{
	VP_ERROR_E              result         = VP_ERROR_OTHERS;
#ifdef MMI_STREAMING_PLAYER_SUPPORT
    void                *argv[1]        = {0};    
    DPLAYER_MEDIA_INFO_T p_info         = {0};     
    DPLAYER_RETURN_E     dplayer_result = DPLAYER_SUCCESS;
   // GUI_RECT_T lcd_rect = {0};
   // GUI_LCD_DEV_INFO const *lcd_dev_info_ptr = (const GUI_LCD_DEV_INFO*)MMITHEME_GetDefaultLcdDev();
    BOOLEAN    is_need_fill = TRUE;
    VP_DISPLAY_PARAM_T play_param = {0};

    if (PNULL == fsm_ptr || !IsValidFsm(fsm_ptr))
    {
        //VP_STRACE_LOW:"[VP] PlayFromNet param is error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_2048_112_2_18_3_5_7_85,(uint8*)"");
        return result;
    }
 #ifdef  VIDEOTHUMBNAIL_SUPPORT      
     MMIAPIVP_ThumbnailClose();
 #endif
    //create dplayer handle
    argv[0] = (void*)(&strm_cfg);
    DPLAYER_Init();
    fsm_ptr->dplayer_handle = DPLAYER_Open(DPLAYER_OPEN_FROM_NET, 1, argv);    
    //VP_STRACE_LOW:"[VP] PlayFromNet fsm_ptr->dplayer_handle = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_2061_112_2_18_3_5_7_86,(uint8*)"d",fsm_ptr->dplayer_handle);
    if(0 < fsm_ptr->dplayer_handle)
    {
        //获取当前媒体文件总的播放时间和当前的播放时间
        fsm_ptr->total_time = DPLAYER_GetLength(fsm_ptr->dplayer_handle);
        if (0 == fsm_ptr->total_time)
        {
            fsm_ptr->total_time = VP_MAX_INVALID_VALUE;
        }
        fsm_ptr->process_time = 0;
        dplayer_result = DPLAYER_GetMovieInfo(fsm_ptr->dplayer_handle,&p_info);
        if (DPLAYER_ERROR_UNSUPPORTED_FORMAT == dplayer_result)
        {
            fsm_ptr->support_type = VP_SUPPORTED_NEITHER;
        }        
        else 
        {
            fsm_ptr->support_type = CheckSupportedTypeFromMediaInfo(&p_info);
        }
      
        fsm_ptr->display_param.LcdBlkId = 0;
        fsm_ptr->display_param.LcdId = 0;
        if(fsm_ptr->is_cr_video_layer)
        {
            fsm_ptr->display_param.disp_data_op = DPLAYER_DISP_DATA_GET;
             fsm_ptr->display_param.b_no_osd = 1;
        }
        else
        {
            fsm_ptr->display_param.disp_data_op = DPLAYER_DISP_DATA_OP_NONE;
#ifdef MMI_VIDEOPLAYER_MINI_FUNCTION
            fsm_ptr->display_param.b_no_osd = 1;
#else
            fsm_ptr->display_param.b_no_osd = 0;
#endif

        }
        fsm_ptr->is_seek_able = p_info.b_is_seekable;
        
      //  fsm_ptr->display_param.disp_mode = DPLAYER_DISP_FULLVIEW;//DPLAYER_DISP_CUSTOMIZE;
        
        VP_STRACE_LOW("[VP] PlayFromNet play_param_ptr->is_scal_up = %d, fsm_ptr->display_param.Width = %d, p_info.video_info.width = %d, fsm_ptr->display_param.Height = %d, p_info.video_info.height = %d, seek = %d",fsm_ptr->is_scal_up,fsm_ptr->display_param.disp_rect.dx,p_info.video_info.width,fsm_ptr->display_param.disp_rect.dy,p_info.video_info.height,fsm_ptr->is_seek_able);
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_2089_112_2_18_3_5_7_87,(uint8*)"dddddd",fsm_ptr->is_scal_up,fsm_ptr->display_param.disp_rect.dx,p_info.video_info.width,fsm_ptr->display_param.disp_rect.dy,p_info.video_info.height,fsm_ptr->is_seek_able);
      /*  if (!(fsm_ptr->is_scal_up) && fsm_ptr->display_param.Width >= p_info.video_info.width && fsm_ptr->display_param.Height >= p_info.video_info.height)
        {
            fsm_ptr->display_param.xOffset = fsm_ptr->display_param.xOffset + ((fsm_ptr->display_param.Width - p_info.video_info.width) >> 1);
            fsm_ptr->display_param.yOffset = fsm_ptr->display_param.yOffset + ((fsm_ptr->display_param.Height - p_info.video_info.height) >> 1);
            fsm_ptr->display_param.Width = p_info.video_info.width;
            fsm_ptr->display_param.Height = p_info.video_info.height;
        }*/
        
        //给ui发送VP_PREPARE_IND消息
        SendPrepareIndToUI(fsm_ptr);

        //VP_STRACE_LOW:"[VP] PlayFromNet total_time = %d, process_time = %d, support_type = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_2101_112_2_18_3_5_7_88,(uint8*)"ddd",fsm_ptr->total_time,fsm_ptr->process_time,fsm_ptr->support_type);
                
        //设置每帧显示完毕后的回调函数
        DPLAYER_SetFrameEndNotify(fsm_ptr->dplayer_handle, CallbackFrameEndNotify);
        
        //设置播放结束的回调函数
        //DPLAYER_SetEndNotify(fsm_ptr->dplayer_handle, CallbackEndNotify);
      
        
        //设置播放显示参数
        //开始播放
        if (!fsm_ptr->is_to_pause)
        {   
            if (VP_SUPPORTED_NEITHER == fsm_ptr->support_type
            || VP_SUPPORTED_AUDIO_ONLY == fsm_ptr->support_type)
            {
                 is_need_fill = FALSE;
            }

            SCI_MEMCPY(&play_param,&(fsm_ptr->display_param),sizeof(VP_DISPLAY_PARAM_T));
            MMIAPIVP_TranslateDisplayParamLogicToPhysical(&play_param, FALSE);

            if(DPLAYER_SUCCESS != DPLAYER_SetDisplayParam(fsm_ptr->dplayer_handle,&play_param)
                ||!ReqPlaySrv(fsm_ptr)
                ||!MMISRVAUD_Play(fsm_ptr->srv_handle,0)
                || !STRM_Play(fsm_ptr->strm_handle,fsm_ptr->dplayer_handle))
            {
                //VP_STRACE_LOW:"[VP] PlayFromNet play error"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_2128_112_2_18_3_5_7_89,(uint8*)"");
                MMISRVAUD_Stop(fsm_ptr->srv_handle);
                //MMISRVMGR_Free(fsm_ptr->srv_handle);
                //fsm_ptr->srv_handle = 0;
                DPLAYER_Close(fsm_ptr->dplayer_handle);
                fsm_ptr->dplayer_handle = 0;    
                DPLAYER_Release();
                return result;
            }            
        }
#ifdef MMI_VIDEOPLAYER_HISTORYLIST_SUPPORT
        if(fsm_ptr->is_save_history)
        {            
#ifdef MMI_KING_MOVIE_SUPPORT
            if(!MMIAPIKM_IsSupportfile() && !MMIAPISM_IsSupportfile() 
#ifdef MMI_KING_MOVIE_SUPPORT_HMV
                && !MMIAPIKM_IsHmvfile()
#endif
                )//smv_support
#endif
            {
            VP_HistoryList_ModifyItem(                
                fsm_ptr->process_time,
                fsm_ptr->total_time,
                p_info.video_info.height,
                p_info.video_info.width,
                p_info.media_length,
                fsm_ptr->full_path_len,
                fsm_ptr->full_path_ptr); 
            }  
              
        }
#endif //MMI_VIDEOPLAYER_HISTORYLIST_SUPPORT
        result = VP_ERROR_NONE;
    }
    else
    {
        SendPrepareIndToUI(fsm_ptr);
        result = VP_ERROR_NOT_SUPPORT;
    }
#endif
    return result;
}

/*****************************************************************************/
//  Description : 处理pdp消息
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL void HandlePDPMsg(MMIPDP_CNF_INFO_T *msg_ptr)
{   
#ifdef MMI_STREAMING_PLAYER_SUPPORT
    VP_FSM_MSG_E        msg_id              = 0;
    void                *msg_body_ptr       = PNULL;
    uint32              msg_body_size       = 0;
    
    if (PNULL == s_current_fsm_ptr || PNULL == msg_ptr)
    {
        //VP_STRACE_LOW:"[VP] HandlePDPMsg param is error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_2196_112_2_18_3_5_7_90,(uint8*)"");
        return;
    }
    
    //VP_STRACE_LOW:"[VP] HandlePDPMsg msg_ptr->msg_id = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_2200_112_2_18_3_5_7_91,(uint8*)"d",msg_ptr->msg_id);
    switch(msg_ptr->msg_id) 
    {
    case MMIPDP_ACTIVE_CNF:
        msg_id = VP_PDP_ACTIVE_CNF;
        msg_body_size = sizeof(MMIPDP_CNF_INFO_T);
        msg_body_ptr = msg_ptr;       
        break;
        
    case MMIPDP_DEACTIVE_IND:
        SendFatalErrorIndTOUI(s_current_fsm_ptr,VP_ERROR_SERVER_CLOSE_IND);
        msg_id = VP_PDP_DEACTIVE_IND;
        msg_body_size = 0;
        msg_body_ptr = PNULL;            
        break;
        
    default:
        return;
    }

    //向videoplayer control发送消息
    FsmDispatch((VP_HANDLE)s_current_fsm_ptr,msg_id,msg_body_ptr,msg_body_size);     
#endif
}

/*****************************************************************************/
//  Description : 激活pdp
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN ActivePdp(VP_FSM_T *fsm_ptr)
{
#ifdef MMI_STREAMING_PLAYER_SUPPORT
#ifdef MMI_GPRS_SUPPORT
    MMIPDP_ACTIVE_INFO_T                    app_info            = {0};
    MN_DUAL_SYS_E                           dual_sys            = 0;
    uint8                                   setting_index       = 0;
    MMICONNECTION_LINKSETTING_DETAIL_T      *setting_item_ptr  = PNULL;
    
    if (PNULL == fsm_ptr || !IsValidFsm(fsm_ptr))
    {
        //VP_STRACE_LOW:"[VP] ActivePdp param is error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_2240_112_2_18_3_5_7_92,(uint8*)"");
        return FALSE;
    }
   
    dual_sys = fsm_ptr->dual_sys;
    //VP_STRACE_LOW:"[VP] ActivePdp dual_sys = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_2245_112_2_18_3_5_7_93,(uint8*)"d",dual_sys);
    
    app_info.app_handler = MMI_MODULE_VP;

    if (MMI_DUAL_SYS_MAX != dual_sys)
    {    
        if (!MMIAPIPHONE_IsSimAvailable(dual_sys))
        {
            //SCI_TRACE_LOW:"[VP] ActivePdp sim casd is not ok dual_sys = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_2253_112_2_18_3_5_7_94,(uint8*)"d",dual_sys);
            return FALSE;
        }
    
        setting_index = MMIAPICONNECTION_GetAccountDefaultIndex((MN_DUAL_SYS_E)dual_sys, MMICONNECTION_ACCOUNT_BROWSER);//VP_Setting_GetNetIndex(dual_sys)  
        setting_item_ptr = MMIAPICONNECTION_GetLinkSettingItemByIndex(dual_sys, setting_index);
    
        if (PNULL == setting_item_ptr)
        {
            //SCI_TRACE_LOW:"[VP] ActivePdp param is setting_index = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_2262_112_2_18_3_5_7_95,(uint8*)"d",setting_index);
            return FALSE;
        }

        
        app_info.auth_type = setting_item_ptr->auth_type;
        app_info.apn_ptr = (char*)setting_item_ptr->apn;
        
        app_info.user_name_ptr = (char*)setting_item_ptr->username;
        app_info.psw_ptr = (char*)setting_item_ptr->password;
        app_info.dual_sys = dual_sys;
        app_info.ps_service_type = STREAMING_E;
        app_info.storage = MN_GPRS_STORAGE_ALL;
    }
    else
    {
#ifdef MMI_WIFI_SUPPORT
        app_info.apn_ptr = PNULL;
        app_info.ps_interface = MMIPDP_INTERFACE_WIFI;
#else
        if (!MMIAPIPHONE_IsSimAvailable(dual_sys))
        {
            //SCI_TRACE_LOW:"[VP] ActivePdp sim casd is not ok dual_sys = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_2285_112_2_18_3_5_7_96,(uint8*)"d",dual_sys);
            return FALSE;
        }        
#endif
    }
    app_info.priority = 3;    
    app_info.handle_msg_callback = HandlePDPMsg;   
    app_info.ps_service_rat = MN_TD_PREFER;
    s_current_fsm_ptr = fsm_ptr;
    if (!MMIAPIPDP_Active(&app_info))
    {
        return FALSE;
    }
    return TRUE;
#else
    return FALSE;
#endif
#else
	return FALSE;
#endif
}

/*****************************************************************************/
//  Description : 空闲状态的处理
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN FsmIdle(VP_FSM_T *fsm_ptr, VP_FSM_MSG_T *msg_ptr)
{
    BOOLEAN    result = TRUE;
    //参数检查
    if(PNULL == fsm_ptr || !IsValidFsm(fsm_ptr) || PNULL == msg_ptr)
    {
        //VP_STRACE_LOW:"[VP] FsmIdle param is error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_2317_112_2_18_3_5_7_97,(uint8*)"");
        return FALSE;
    }
    
    //VP_STRACE_LOW:"[VP] FsmIdle msg_ptr->msg_id = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_2321_112_2_18_3_5_7_98,(uint8*)"d",msg_ptr->msg_id);
    switch(msg_ptr->msg_id)
    {
    case VP_INIT_REQ:
        FsmStateTrans(fsm_ptr,VP_STATE_READY);        
        break;
        
    default:
        //VP_STRACE_LOW:"IDLE, got wrong msg"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_2329_112_2_18_3_5_7_99,(uint8*)"");
        result =  FALSE;
        break;
    }
    return result;
}

/*****************************************************************************/
//  Description : 初始化状态机信息
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL void InitFsmInfo(VP_FSM_T *fsm_ptr, VP_STATE_E  state, VP_CALLBACK  vpcallback)
{   
    uint32 task_id = 0;
    BOOLEAN is_exit = FALSE;
    BOOLEAN is_pause = FALSE;
    BOOLEAN is_resume = FALSE;
    MMISRV_HANDLE_T  srv_handle = 0;

    if (PNULL == fsm_ptr || !IsValidFsm(fsm_ptr))
    {
        //VP_STRACE_LOW:"[VP] InitFsmInfo param is null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_2350_112_2_18_3_5_7_100,(uint8*)"");
        return;
    }
    //VP_STRACE_LOW:"[VP] InitFsmInfo state = %d, vpcallback = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_2353_112_2_18_3_5_7_101,(uint8*)"dd",state,vpcallback);
    
    if (PNULL != fsm_ptr->full_path_ptr)
    {
        SCI_FREE(fsm_ptr->full_path_ptr);
        fsm_ptr->full_path_ptr = PNULL;
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
    srv_handle = fsm_ptr->srv_handle;
    SCI_MEMSET(fsm_ptr,0,sizeof(VP_FSM_T));
    fsm_ptr->is_to_exit = is_exit;
    fsm_ptr->is_to_pause = is_pause;
    fsm_ptr->is_to_resume = is_resume;
    fsm_ptr->state = state;
    fsm_ptr->vpcallback = vpcallback;
    fsm_ptr->task_id = task_id;
    fsm_ptr->srv_handle = srv_handle;
	fsm_ptr->is_seek_able = TRUE;
}

/*****************************************************************************/
//  Description : 更新状态机信息
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN UpdateFsmInfo(VP_FSM_T *fsm_ptr, VP_PLAY_PARAM_T* play_param_ptr)
{
    BOOLEAN result = TRUE;

    if (PNULL == fsm_ptr || !IsValidFsm(fsm_ptr) || PNULL == play_param_ptr)
    {
        //VP_STRACE_LOW:"[VP] UpdateFsmInfo param is null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_2391_112_2_18_3_5_7_102,(uint8*)"");
        return FALSE;
    }
    
    if (VP_PLAY_FROM_NET == play_param_ptr->play_type && PNULL != play_param_ptr->url_ptr)
    {        
        fsm_ptr->url_ptr = (uint8 *)SCI_ALLOCA(strlen(play_param_ptr->url_ptr)+1);
        if (PNULL != fsm_ptr->url_ptr)
        {
            SCI_MEMSET(fsm_ptr->url_ptr,0,(strlen(play_param_ptr->url_ptr)+1));/*lint !e666*/
            if(0 == strnicmp(play_param_ptr->url_ptr,HTTP_URL_HEAD_ASIC,HTTP_URL_HEAD_LEN))
            {            
                SCI_MEMCPY(fsm_ptr->url_ptr,HTTP_URL_HEAD_ASIC,HTTP_URL_HEAD_LEN);  
                SCI_MEMCPY((fsm_ptr->url_ptr+HTTP_URL_HEAD_LEN),(play_param_ptr->url_ptr+HTTP_URL_HEAD_LEN),(strlen(play_param_ptr->url_ptr)-HTTP_URL_HEAD_LEN));     /*lint !e666*/
            }
            else if(0 == strnicmp(play_param_ptr->url_ptr,RTSP_URL_HEAD_ASIC,RTSP_URL_HEAD_LEN))
            {
                SCI_MEMCPY(fsm_ptr->url_ptr,RTSP_URL_HEAD_ASIC,RTSP_URL_HEAD_LEN);  
                SCI_MEMCPY((fsm_ptr->url_ptr+RTSP_URL_HEAD_LEN),(play_param_ptr->url_ptr+RTSP_URL_HEAD_LEN),(strlen(play_param_ptr->url_ptr)-RTSP_URL_HEAD_LEN));     /*lint !e666*/
            }
            else
            {
                SCI_FREE(fsm_ptr->url_ptr);
                fsm_ptr->url_ptr = PNULL;
                return FALSE;
            }
            if (play_param_ptr->dual_sys != fsm_ptr->dual_sys)
            {
#ifdef MMI_GPRS_SUPPORT
                MMIAPIPDP_Deactive(MMI_MODULE_VP);  
#endif
            }
        }
        else
        {
            return FALSE;
        }
    }
    
    if (PNULL != play_param_ptr->full_path_ptr && 0 != play_param_ptr->full_path_len)
    {        
        fsm_ptr->full_path_ptr = (wchar *)SCI_ALLOCA((play_param_ptr->full_path_len + 1)*sizeof(wchar));
        if (PNULL != fsm_ptr->full_path_ptr)
        {
            SCI_MEMSET(fsm_ptr->full_path_ptr,0,((play_param_ptr->full_path_len + 1)*sizeof(wchar)));
            //VP_STRACE_LOW:"[VP] UpdateFsmInfo fsm_ptr->full_path_ptr = %x, play_param_ptr->full_path_ptr = %x"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_2436_112_2_18_3_5_8_103,(uint8*)"dd",fsm_ptr->full_path_ptr,play_param_ptr->full_path_ptr);
            fsm_ptr->full_path_ptr = MMIAPICOM_Wstrncpy(fsm_ptr->full_path_ptr,play_param_ptr->full_path_ptr,play_param_ptr->full_path_len);
            fsm_ptr->full_path_len = play_param_ptr->full_path_len;
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

    if (!result && (VP_PLAY_FROM_FILE == play_param_ptr->play_type || VP_PLAY_FROM_LOCALLIST == play_param_ptr->play_type))
    {
        return FALSE;
    }
    
    SCI_MEMCPY(&(fsm_ptr->display_param),&(play_param_ptr->display_param),sizeof(VP_DISPLAY_PARAM_T));    
    fsm_ptr->is_scal_up = play_param_ptr->is_scal_up;
    fsm_ptr->dual_sys = play_param_ptr->dual_sys;    
    fsm_ptr->is_random_play = play_param_ptr->is_random_play;
    fsm_ptr->is_save_history = play_param_ptr->is_save_history;
    fsm_ptr->mode = play_param_ptr->mode;
    fsm_ptr->play_type = play_param_ptr->play_type;
    fsm_ptr->vol = play_param_ptr->vol;
    fsm_ptr->is_bt_support = play_param_ptr->is_bt_support;
    fsm_ptr->is_a2dp_support = play_param_ptr->is_a2dp_support;
    fsm_ptr->is_cr_video_layer = play_param_ptr->is_cr_video_layer;
    fsm_ptr->is_audio_coexist = play_param_ptr->is_audio_coexist;
    //fsm_ptr->dev_layer_handle = play_param_ptr->dev_layer_handle;
    fsm_ptr->is_share_play = play_param_ptr->is_share_play;
    return TRUE;
}

#ifndef MMI_VIDEOPLAYER_MINI_FUNCTION
/*****************************************************************************/
//  Description : 通知播放下一首
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL void NotifyPlayNextFile(VP_FSM_T *fsm_ptr, VP_PLAY_END_RESULT_E last_play_result)
{
    if (PNULL != fsm_ptr && IsValidFsm(fsm_ptr))
    {    
        SendSignalToCurPlayingTask(APP_VP_PLAY_NEXT_FILE_REQ, (uint16)last_play_result, fsm_ptr->task_id);  
    }
}

/*****************************************************************************/
//  Description : 播放下一首
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL  VP_ERROR_E  PlayNextFile(VP_FSM_T *fsm_ptr, VP_PLAY_END_RESULT_E last_play_result)
{
    wchar full_path [VP_MAX_NAME_LEN + 1] = {0};
    uint16 full_path_len = VP_MAX_NAME_LEN;
    VP_PLAY_PARAM_T play_param = {0};
    VP_ERROR_E  result = VP_ERROR_OTHERS;
#ifdef MMI_VIDEOPLAYER_HISTORYLIST_SUPPORT
    VP_HISTORY_ITEM_T history_list_item = {0};
    SFS_DATE_T          modify_date = {0};
    SFS_TIME_T          modify_time = {0};
#endif
    if (PNULL == fsm_ptr || !IsValidFsm(fsm_ptr))
    {
        //VP_STRACE_LOW:"[VP] PlayNextFile param is null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_2500_112_2_18_3_5_8_104,(uint8*)"");
        return result;
    }
    //VP_STRACE_LOW:"[VP] PlayNextFile i_type = %d, fsm_ptr->mode = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_2503_112_2_18_3_5_8_105,(uint8*)"dd",last_play_result,fsm_ptr->mode);
    SCI_TRACE_LOW("[VP] PlayNextFile is_random_play=%d, last_play_result=%d",fsm_ptr->is_random_play, last_play_result);
	if (VP_PLAY_END_END_SUCCESS == last_play_result)
    {
        VP_LocalList_SetIsCurPlayListItemOK(TRUE);        
    }
    else if(VP_PLAY_END_ERROR == last_play_result)
    {
        VP_LocalList_SetIsCurPlayListItemOK(FALSE);
        if (VP_REPEAT_PLAY_MODE_ONE == fsm_ptr->mode)
        {
            result = VP_ERROR_OTHERS;
            VP_LocalList_DestroyPlayList();
            return result;
        }
    }

            
    if(VP_LocalList_GetNextByMode(full_path, &full_path_len,fsm_ptr->mode,fsm_ptr->is_random_play))
    {  
#ifdef MMI_VIDEOPLAYER_HISTORYLIST_SUPPORT
        if (fsm_ptr->is_save_history)
        {
            SCI_MEMSET(&history_list_item,0,sizeof(VP_HISTORY_ITEM_T));  
            history_list_item.locallist_type = VP_LocalList_GetCurListType();
            history_list_item.locallist_index = VP_LocalList_GetCurrentIndex(history_list_item.locallist_type); 
            history_list_item.type = VP_PLAY_FROM_LOCALLIST;
            history_list_item.local_movie_file_info_ptr = (VP_LOCAL_MOVIE_FILE_INFO_T*)SCI_ALLOCA(sizeof(VP_LOCAL_MOVIE_FILE_INFO_T));
            if (PNULL != history_list_item.local_movie_file_info_ptr)
            {
                SCI_MEMSET(history_list_item.local_movie_file_info_ptr,0,sizeof(VP_LOCAL_MOVIE_FILE_INFO_T));
                history_list_item.local_movie_file_info_ptr->full_path_len = MIN(full_path_len,VP_MAX_NAME_LEN);
                SCI_MEMSET(history_list_item.local_movie_file_info_ptr->full_path_ptr,0,((VP_MAX_NAME_LEN +1)*sizeof(wchar)));
                MMIAPICOM_Wstrncpy(history_list_item.local_movie_file_info_ptr->full_path_ptr,
                    (wchar *)full_path,
                    history_list_item.local_movie_file_info_ptr->full_path_len);
                MMIAPIFMM_GetFileInfo((wchar *)full_path,full_path_len,&(history_list_item.local_movie_file_info_ptr->file_size),&modify_date,&modify_time);
                history_list_item.local_movie_file_info_ptr->create_time = MMIAPICOM_TansferSFSTime(modify_date,modify_time);
				VP_HistoryList_AddItem(history_list_item);
                SCI_FREE(history_list_item.local_movie_file_info_ptr);
            }
            
        }
#endif //MMI_VIDEOPLAYER_HISTORYLIST_SUPPORT
        //设置播放参数；播放视频
        play_param.dual_sys = fsm_ptr->dual_sys;
        play_param.full_path_len = full_path_len;
        play_param.full_path_ptr = full_path;
        play_param.is_random_play = fsm_ptr->is_random_play;
        play_param.mode = fsm_ptr->mode;
        play_param.play_type = fsm_ptr->play_type;
        play_param.url_ptr = PNULL;
        play_param.video_buf_info.type = 0;
        play_param.video_buf_info.video_buf_len = 0;
        play_param.video_buf_info.video_buf_ptr = PNULL;        
        play_param.is_save_history = fsm_ptr->is_save_history;
        play_param.is_scal_up = fsm_ptr->is_scal_up;  
        play_param.vol = fsm_ptr->vol;
        play_param.is_a2dp_support = fsm_ptr->is_a2dp_support;
        play_param.is_bt_support = fsm_ptr->is_bt_support;
        play_param.is_share_play = fsm_ptr->is_share_play;
        
        SCI_MEMCPY(&(play_param.display_param),&(fsm_ptr->display_param),sizeof(VP_DISPLAY_PARAM_T));        
        if(MMIAPIVP_FSMPlay((VP_HANDLE)fsm_ptr,&play_param))
        {
            result = VP_ERROR_NONE;
        }
    }
    
    //VP_STRACE_LOW:"[VP] PlayNextFile result = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_2571_112_2_18_3_5_8_106,(uint8*)"d",result);
    
    if (VP_ERROR_NONE != result)
    {
        VP_LocalList_DestroyPlayList();
    }
    return result;

}
#endif

/*****************************************************************************/
//  Description : 准备好状态的处理
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN FsmReady(VP_FSM_T *fsm_ptr, VP_FSM_MSG_T *msg_ptr)
{    
    BOOLEAN             result         = TRUE;    
    VP_PLAY_PARAM_T     * play_param_ptr = PNULL;
    VP_ERROR_E          error_code = VP_ERROR_OTHERS;
#ifndef MMI_VIDEOPLAYER_MINI_FUNCTION     
    static VP_PLAY_END_RESULT_E last_play_result = VP_PLAY_END_END_SUCCESS;   
#endif

    //参数检查
    if(PNULL == fsm_ptr || !IsValidFsm(fsm_ptr) || PNULL == msg_ptr ||  PNULL == fsm_ptr->vpcallback)
    {
        //VP_STRACE_LOW:"[VP] FsmReady play param is null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_2594_112_2_18_3_5_8_107,(uint8*)"");
        return FALSE;
    }

    //VP_STRACE_LOW:"[VP] FsmReady msg_ptr->msg_id = %x, fsm_ptr->has_play_end_reason = %d, fsm_ptr->is_to_exit = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_2598_112_2_18_3_5_8_108,(uint8*)"ddd",msg_ptr->msg_id,fsm_ptr->has_play_end_reason,fsm_ptr->is_to_exit);
    switch(msg_ptr->msg_id)
    {
    case VP_ENTRY_REQ:
        fsm_ptr->is_to_pause = FALSE;
        fsm_ptr->is_to_resume = FALSE;
        fsm_ptr->has_play_end_reason = VP_PLAY_END_NONE;
        if(fsm_ptr->is_to_exit)
        {
            //清除强制退出标志；
            fsm_ptr->is_to_exit = FALSE;
            SendStopIndToUI(fsm_ptr);       
            MMIAPIVP_FSMRelease((VP_HANDLE)fsm_ptr);
        }
        break;

    case VP_PLAY_REQ:
        InitFsmInfo(fsm_ptr,VP_STATE_READY,fsm_ptr->vpcallback);
        play_param_ptr = (VP_PLAY_PARAM_T  *)msg_ptr->msg_body;
        result = UpdateFsmInfo(fsm_ptr,play_param_ptr);
        if(result)
        {
        
            //VP_STRACE_LOW:"[VP] FsmReady play_param_ptr->play_type = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_2621_112_2_18_3_5_8_109,(uint8*)"d",play_param_ptr->play_type);
            switch(play_param_ptr->play_type) 
            {
            case VP_PLAY_FROM_LOCALLIST:
            case VP_PLAY_FROM_FILE: 
                //播放本地文件
                error_code = PlayLocalFile(fsm_ptr,TRUE);
                if(VP_ERROR_NONE != error_code)
                {  
                    SendPlayCnfToUI(fsm_ptr,error_code); 
#ifndef MMI_VIDEOPLAYER_MINI_FUNCTION                    
                    DelaySwitchVideoPlayer((uint32)(fsm_ptr));
#endif
                    //NotifyPlayNextFile(fsm_ptr,VP_PLAY_END_ERROR);                    
                }
                else
                {
                    //播放成功，进入playing状态
                    SendPlayCnfToUI(fsm_ptr,VP_ERROR_NONE);
                    FsmStateTrans(fsm_ptr,VP_STATE_PLAYING); 
                }
                break;
            
            case VP_PLAY_FROM_BUFFER:
                //播放本地buf
                error_code = PlayLocalBuf(fsm_ptr,play_param_ptr);
                SendPlayCnfToUI(fsm_ptr,error_code);
                if(VP_ERROR_NONE == error_code)
                {
                    //播放成功，进入playing状态
                    FsmStateTrans(fsm_ptr,VP_STATE_PLAYING);  
                }    
                else
                {
                    result = FALSE;
                }                
                break;
#ifdef MMI_STREAMING_PLAYER_SUPPORT             
            case VP_PLAY_FROM_NET:   
                //进入pdp activing状态
                FsmStateTrans(fsm_ptr,VP_STATE_PDP_ACTIVING);
                result = ActivePdp(fsm_ptr);  
                if (!result)
                {
                    SendPlayCnfToUI(fsm_ptr,VP_ERROR_OTHERS);
                    FsmStateTrans(fsm_ptr,VP_STATE_READY);                    
                }
                break;
#endif
            default:           
                result = FALSE;
                SendPlayCnfToUI(fsm_ptr,VP_ERROR_OTHERS);
                break;
            }
        }
        
        if (!result)
        {            
            SendStopIndToUI(fsm_ptr);
        }
        result = TRUE; 
        break;
#ifndef MMI_VIDEOPLAYER_MINI_FUNCTION        
    case VP_PLAY_NEXT_FILE:
        if (PNULL != msg_ptr->msg_body)
        {
            last_play_result = *(VP_PLAY_END_RESULT_E *)msg_ptr->msg_body;
        }
        else
        {
            last_play_result = VP_PLAY_END_ERROR;
        }
        if (!fsm_ptr->is_to_pause)
        {
            error_code = PlayNextFile(fsm_ptr, last_play_result); 
            if (VP_ERROR_NONE != error_code)
            {
                SendStopIndToUI(fsm_ptr);
            } 
        }   
        else
        {
            fsm_ptr->is_to_pause = FALSE;
        }
        break;
        
    case VP_RESUME_REQ:
        if (fsm_ptr->is_to_resume)
        {
            fsm_ptr->is_to_resume = FALSE;
            error_code = PlayNextFile(fsm_ptr, last_play_result); 
            if (VP_ERROR_NONE != error_code)
            {
                SendStopIndToUI(fsm_ptr);
            } 
        }
        break;
#endif   
    case VP_RELEASE_REQ:
        //销毁状态机
        DestroyFSM((VP_HANDLE)fsm_ptr);       
        break;
        
    case VP_STOP_REQ:        
        FsmStateTrans(fsm_ptr,VP_STATE_READY); 
        SendStopIndToUI(fsm_ptr);
        break;

    default:
        //VP_STRACE_LOW:"[VP] FsmReady ready, got wrong msg"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_2728_112_2_18_3_5_8_110,(uint8*)"");
        result = FALSE;
        break;
    }
    return result;
}
#ifdef MMI_STREAMING_PLAYER_SUPPORT
/*****************************************************************************/
//  Description : InitStreaming
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN InitStreaming(VP_FSM_T *fsm_ptr,uint32 net_id)
{
    STRM_INIT_PARAM_T  strm_ini_para = {0};
    VP_NV_SETTING_T     setting_info = {0};
    MMIFILE_HANDLE      sdp_file_handle = 0;
    uint32              readed = 0;   
    BOOLEAN             result = TRUE;
    

    if (PNULL == fsm_ptr || !IsValidFsm(fsm_ptr))
    {
        //VP_STRACE_LOW:"[VP] InitStreaming param is error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_2750_112_2_18_3_5_8_111,(uint8*)"");
        return FALSE;
    }
    //获取网络配置信息
    strm_ini_para.app_thread_id = P_APP;
    setting_info = VP_Setting_GetSettingInfo();
    if (!setting_info.is_use_agent)
    {
        strm_ini_para.proxy.host_ptr = PNULL;
        strm_ini_para.proxy.port = 0;
    }
    else
    {
        strm_ini_para.proxy.host_ptr = (char *)SCI_ALLOCA(VP_MAX_IP_LENGTH+1);
        if (PNULL == strm_ini_para.proxy.host_ptr)
        {
            return FALSE;
        }
        SCI_MEMSET(strm_ini_para.proxy.host_ptr,0,(VP_MAX_IP_LENGTH+1));
        SCI_MEMCPY(strm_ini_para.proxy.host_ptr,(char *)(setting_info.agent_ip),setting_info.agent_ip_length);
        
        strm_ini_para.proxy.port = setting_info.agent_port;
    }
    
    strm_ini_para.proxy.username_ptr = PNULL;
    strm_ini_para.proxy.password_ptr = PNULL;
    strm_ini_para.auth_info.password_ptr = PNULL;
    strm_ini_para.auth_info.username_ptr = PNULL;
    strm_ini_para.url_ptr = (char *)(fsm_ptr->url_ptr);

    //读取sdp文按信息
    if (0 != fsm_ptr->full_path_len && PNULL != fsm_ptr->full_path_ptr)
    {
        sdp_file_handle = MMIAPIFMM_CreateFile(fsm_ptr->full_path_ptr,
            SFS_MODE_READ|SFS_MODE_OPEN_EXISTING, /*lint !e655*/
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
		result = FALSE;
    }

    if (result)
    {
        strm_ini_para.uaprof_ptr = PNULL;
        strm_ini_para.rtp_lowest_port = setting_info.lowest_port;
        strm_ini_para.rtp_highest_port = setting_info.highest_port;       
    
        strm_ini_para.net_id = net_id;
        strm_ini_para.user_agent_ptr = "HSVPlayer/1.0 (org=SS; model=13; terversion=11; VodSwVer=00; min=6191234567; lcd=0; cl=2; vo=1; HwClass=1; version=0x0200; visualpro=0x000003; audiopro=0x000001; MIDIType=0)";
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
		strm_ini_para.sdp_buf_ptr = PNULL;
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
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN FsmPdpActiving(VP_FSM_T *fsm_ptr, VP_FSM_MSG_T *msg_ptr)
{
#ifdef MMI_STREAMING_PLAYER_SUPPORT    
    MMIPDP_CNF_INFO_T  *pdp_msg_ptr = PNULL;    
    BOOLEAN             result       = TRUE;

    
    //参数检查
    if(PNULL == fsm_ptr || !IsValidFsm(fsm_ptr) || PNULL == fsm_ptr->vpcallback || PNULL == msg_ptr)
    {
        //VP_STRACE_LOW:"[VP] FsmPdpActiving fsm or msg error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_2863_112_2_18_3_5_8_112,(uint8*)"");
        return FALSE;
    }

    //VP_STRACE_LOW:"[VP] FsmPdpActiving msg_ptr->msg_id = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_2867_112_2_18_3_5_8_113,(uint8*)"d",msg_ptr->msg_id);
    switch(msg_ptr->msg_id)
    {
    case VP_PDP_ACTIVE_CNF:
        pdp_msg_ptr = (MMIPDP_CNF_INFO_T *)msg_ptr->msg_body;
        //判断pdp激活的结果
        if (PNULL == pdp_msg_ptr || MMIPDP_RESULT_SUCC != pdp_msg_ptr->result)
        {
            //VP_STRACE_LOW:"[VP] FsmPdpActiving pdp active error"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_2875_112_2_18_3_5_8_114,(uint8*)"");
            //通知ui激活失败，停止状态机
            SendFatalErrorIndTOUI(fsm_ptr,VP_ERROR_PDP_ACTIVE_ERROR);
            MMIAPIVP_FSMStop((VP_HANDLE)fsm_ptr);            
        }
        else if (InitStreaming(fsm_ptr, (uint32)(pdp_msg_ptr->nsapi)))
        {
            //pdp激活成功，进入init状态
            FsmStateTrans(fsm_ptr,VP_STATE_INIT);            
        }
        else
        {
            //通知ui激活失败，停止状态机     
            SendFatalErrorIndTOUI(fsm_ptr,VP_ERROR_OTHERS);
            MMIAPIVP_FSMStop((VP_HANDLE)fsm_ptr);           
        }
        break;
        
    case VP_PDP_DEACTIVE_IND:
        //通知ui激活失败，停止状态机
        SendFatalErrorIndTOUI(fsm_ptr,VP_ERROR_PDP_ACTIVE_ERROR);
        MMIAPIVP_FSMStop((VP_HANDLE)fsm_ptr);        
        break;
        
    case VP_STOP_REQ:                       
        HandleStopReq(fsm_ptr,FALSE);           
        FsmStateTrans(fsm_ptr,VP_STATE_READY);   
        SendStopIndToUI(fsm_ptr);
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
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN FsmInit_PauseEx(VP_FSM_T *fsm_ptr, VP_FSM_MSG_T *msg_ptr)
{ 
#ifdef MMI_STREAMING_PLAYER_SUPPORT 
    BOOLEAN                     result              = TRUE;    
  //  GUI_RECT_T                  lcd_rect = {0};
    BOOLEAN is_need_fill = TRUE;
    VP_DISPLAY_PARAM_T  play_param = {0};
    
    //参数检查
    if(PNULL == fsm_ptr || !IsValidFsm(fsm_ptr) || PNULL == fsm_ptr->vpcallback || PNULL == msg_ptr)
    {
        //VP_STRACE_LOW:"[VP] FsmInit param is error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_2931_112_2_18_3_5_9_115,(uint8*)"");
        return FALSE;
    }

    //VP_STRACE_LOW:"[VP] FsmInit msg_ptr->msg_id = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_2935_112_2_18_3_5_9_116,(uint8*)"d",msg_ptr->msg_id);
    switch(msg_ptr->msg_id)
    {
    case VP_RESUME_REQ:    
        if (fsm_ptr->is_to_resume)
        {     
            fsm_ptr->is_to_resume = FALSE;
            
            if (VP_SUPPORTED_NEITHER == fsm_ptr->support_type
            || VP_SUPPORTED_AUDIO_ONLY == fsm_ptr->support_type)
            {
                 is_need_fill = FALSE;
            }
            SCI_MEMCPY(&play_param, &(fsm_ptr->display_param),sizeof(VP_DISPLAY_PARAM_T));
            MMIAPIVP_TranslateDisplayParamLogicToPhysical(&play_param, is_need_fill);

            if(DPLAYER_SUCCESS != DPLAYER_SetDisplayParam(fsm_ptr->dplayer_handle,&play_param) 
                ||MMISRVAUD_RET_ERROR == MMISRVAUD_Play(fsm_ptr->srv_handle,0)
                || !STRM_Play(fsm_ptr->strm_handle,fsm_ptr->dplayer_handle))
            {               

                //VP_STRACE_LOW:"[VP] PlayFromNet play error"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_2956_112_2_18_3_5_9_117,(uint8*)"");
                SendPlayCnfToUI(fsm_ptr,VP_ERROR_OTHERS);
                MMIAPIVP_FSMStop((VP_HANDLE)fsm_ptr);
            }
            else
            {
                FsmStateTrans(fsm_ptr,VP_STATE_PLAY_PENDING);
            }
        }
         break;
        
    case VP_PDP_DEACTIVE_IND:
    case VP_STOP_REQ: 
        if(VP_PDP_DEACTIVE_IND == msg_ptr->msg_id)
        {
            SendFatalErrorIndTOUI(fsm_ptr,VP_ERROR_PDP_DEACTIVE_IND);
        }
        DPLAYER_Close(fsm_ptr->dplayer_handle);
        fsm_ptr->dplayer_handle = 0;    
        DPLAYER_Release();
        HandleStopReq(fsm_ptr,FALSE);
        FsmStateTrans(fsm_ptr,VP_STATE_CLOSING);        
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
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN FsmInit(VP_FSM_T *fsm_ptr, VP_FSM_MSG_T *msg_ptr)
{ 
#ifdef MMI_STREAMING_PLAYER_SUPPORT
    BOOLEAN                     result              = TRUE;    
    STRM_RESULT_E               str_ini_result      = STRM_RESULT_SUCCESS;
    VP_ERROR_E                  error_code          = 0;
    
    //参数检查
    if(PNULL == fsm_ptr || !IsValidFsm(fsm_ptr) || PNULL == fsm_ptr->vpcallback || PNULL == msg_ptr)
    {
        //VP_STRACE_LOW:"[VP] FsmInit param is error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_3013_112_2_18_3_5_9_118,(uint8*)"");
        return FALSE;
    }

    //VP_STRACE_LOW:"[VP] FsmInit msg_ptr->msg_id = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_3017_112_2_18_3_5_9_119,(uint8*)"d",msg_ptr->msg_id);
    switch(msg_ptr->msg_id)
    {
    case VP_STRM_INIT_CNF:       
         str_ini_result = HandleInitCnf(fsm_ptr,(STRM_INIT_CNF_T*)msg_ptr->msg_body, &error_code);
         //VP_STRACE_LOW:"[VP] FsmInit str_ini_result = %d, is_success_play = %d"
         SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_3022_112_2_18_3_5_9_120,(uint8*)"dd",str_ini_result,error_code);
         switch(str_ini_result) 
         {
         case STRM_RESULT_SUCCESS: 
            if (VP_ERROR_NONE == error_code)
            {
                if (fsm_ptr->is_to_pause)
                {
                    fsm_ptr->is_to_pause = FALSE;
                    FsmStateTrans(fsm_ptr,VP_STATE_INIT_PAUSED_EX);
                }
                else
                {
                    FsmStateTrans(fsm_ptr,VP_STATE_PLAY_PENDING);
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
            error_code = VP_ERROR_NOT_SUPPORT;
            break;
            
        case STRM_RESULT_INTERNAL_SERVER_ERROR:
            result = FALSE;
            error_code = VP_ERROR_INTERNAL_SERVER_ERROR;
            break;
            
        case STRM_RESULT_BAD_REQUEST: 
            result = FALSE;
            error_code = VP_ERROR_BAD_REQUEST;
            break;
        case STRM_RESULT_UNAUTHORIZED:
            result = FALSE;
            error_code = VP_ERROR_UNAUTHORIZED;
            break;
            
        case STRM_RESULT_NOT_FOUND:
            result = FALSE;
            error_code = VP_ERROR_NOT_FOUND;
            break; 

        case STRM_RESULT_UNKNOWN:
            result = FALSE;
            error_code = VP_ERROR_CONNECT_TIMEOUT;
            break;

         default:
             result = FALSE;
             error_code = VP_ERROR_STRM_INIT_ERROR;            
         }   

         if (!result)
         {
             SendPlayCnfToUI(fsm_ptr,error_code);
            // FsmStateTrans(fsm_ptr,VP_STATE_READY);             
             MMIAPIVP_FSMStop((VP_HANDLE)fsm_ptr);
             result = TRUE;
         }
        break;
        
    case VP_PDP_DEACTIVE_IND:
    case VP_STOP_REQ: 
        if(VP_PDP_DEACTIVE_IND == msg_ptr->msg_id)
        {
            SendFatalErrorIndTOUI(fsm_ptr,VP_ERROR_PDP_DEACTIVE_IND);
        }        
        
        HandleStopReq(fsm_ptr,FALSE);
        FsmStateTrans(fsm_ptr,VP_STATE_CLOSING);        
        break;
        
    case VP_AUTHIND_REQ:        
        //转入VP_STATE_INIT_AUTH状态，等待ui的鉴权信息
        FsmStateTrans(fsm_ptr,VP_STATE_INIT_AUTH);
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
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN FsmInitAuth(VP_FSM_T *fsm_ptr, VP_FSM_MSG_T *msg_ptr)
{
#ifdef MMI_STREAMING_PLAYER_SUPPORT
    VP_AUTH_INFO_T          *auth_info_ptr      = PNULL;
    STRM_AUTH_INFO_T        strm_auth_info      = {0};
    BOOLEAN                 result              = TRUE;

    //参数检查
    if(PNULL == fsm_ptr || !IsValidFsm(fsm_ptr) || PNULL == fsm_ptr->vpcallback || PNULL == msg_ptr)
    {
        //VP_STRACE_LOW:"[VP] FsmInitAuth param is error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_3133_112_2_18_3_5_9_121,(uint8*)"");
        return FALSE;
    }

    //VP_STRACE_LOW:"[VP] FsmInitAuth msg_ptr->msg_id = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_3137_112_2_18_3_5_9_122,(uint8*)"d",msg_ptr->msg_id);
    switch(msg_ptr->msg_id)
    {
    case VP_AUTH_RSP:
        //获取鉴权信息
        auth_info_ptr = (VP_AUTH_INFO_T *)msg_ptr->msg_body;
        if (PNULL == auth_info_ptr)
        {
            result = FALSE;
        }
        else
        {
            strm_auth_info.password_ptr = auth_info_ptr->password_ptr;
            strm_auth_info.username_ptr = auth_info_ptr->username_ptr;
            //将鉴权信息传给streaming处理
            result = STRM_DescribeAuthRsp(fsm_ptr->strm_handle,&strm_auth_info);
        }
        if (result)
        {
            FsmStateTrans(fsm_ptr,VP_STATE_INIT);
        }
        else
        {
            SendPlayCnfToUI(fsm_ptr,VP_ERROR_UNAUTHORIZED);
            MMIAPIVP_FSMStop((VP_HANDLE)fsm_ptr);
        }
        result = TRUE;
        break;
        
    case VP_PDP_DEACTIVE_IND:
    case VP_STOP_REQ:
        if(VP_PDP_DEACTIVE_IND == msg_ptr->msg_id)
        {
            SendFatalErrorIndTOUI(fsm_ptr,VP_ERROR_PDP_DEACTIVE_IND);
        }       
        
        HandleStopReq(fsm_ptr,FALSE);    
        FsmStateTrans(fsm_ptr,VP_STATE_CLOSING);
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
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN FsmPlayPeding(VP_FSM_T *fsm_ptr, VP_FSM_MSG_T *msg_ptr)
{
#ifdef MMI_STREAMING_PLAYER_SUPPORT
    STRM_RESULT_E       strm_result     = 0;
    BOOLEAN             result          = TRUE;
    VP_ERROR_E          error_code = 0;
    BOOLEAN             is_need_fill = TRUE;
    VP_DISPLAY_PARAM_T display_param = {0};
    
    //参数检查
    if(PNULL == fsm_ptr || !IsValidFsm(fsm_ptr) || PNULL == fsm_ptr->vpcallback || PNULL == msg_ptr)
    {
        //VP_STRACE_LOW:"[VP] FsmPlayPeding param is error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_3203_112_2_18_3_5_9_123,(uint8*)"");
        return FALSE;
    }

    //VP_STRACE_LOW:"[VP] FsmPlayPeding msg_ptr->msg_id = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_3207_112_2_18_3_5_9_124,(uint8*)"d",msg_ptr->msg_id);
    switch(msg_ptr->msg_id)
    {
    case VP_STRM_PLAY_CNF:
        strm_result = *(STRM_RESULT_E *)msg_ptr->msg_body;
        //VP_STRACE_LOW:"[VP] FsmPlayPeding strm_result = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_3212_112_2_18_3_5_9_125,(uint8*)"d",strm_result);
        switch(strm_result) 
        {
        case STRM_RESULT_SUCCESS:
            error_code = VP_ERROR_NONE;   
            break;
            
        case STRM_RESULT_UNSUPPORTED_MEDIA_TYPE:
        case STRM_RESULT_UNSUPPORTED:
            error_code = VP_ERROR_NOT_SUPPORT;
            break;
            
        case STRM_RESULT_INTERNAL_SERVER_ERROR:
            error_code = VP_ERROR_INTERNAL_SERVER_ERROR;
            break;
            
        case STRM_RESULT_BAD_REQUEST: 
            error_code = VP_ERROR_BAD_REQUEST;
            break;
        case STRM_RESULT_UNAUTHORIZED:
            error_code = VP_ERROR_UNAUTHORIZED;
            break;
            
        case STRM_RESULT_NOT_FOUND:
            error_code = VP_ERROR_NOT_FOUND;
            break;            
        default:
            error_code = VP_ERROR_OTHERS;
            break;
        }

        
        if (STRM_RESULT_SUCCESS == strm_result)
        {
            if (VP_SUPPORTED_NEITHER == fsm_ptr->support_type
            || VP_SUPPORTED_AUDIO_ONLY == fsm_ptr->support_type
            || fsm_ptr->is_to_pause
            || fsm_ptr->is_to_exit)
            {
                 is_need_fill = FALSE;
            }        
            
            //以下内容主要用于给播放区域刷新color key,将默认图标刷去，不可以去除
            SCI_MEMCPY(&display_param,&(fsm_ptr->display_param),sizeof(VP_DISPLAY_PARAM_T));
            MMIAPIVP_TranslateDisplayParamLogicToPhysical(&display_param, is_need_fill);
            SendPlayCnfToUI(fsm_ptr, error_code);
            FsmStateTrans(fsm_ptr,VP_STATE_PLAYING);
            
        }
        else
        {            
            SendPlayCnfToUI(fsm_ptr, error_code);
            MMIAPIVP_FSMStop((VP_HANDLE)fsm_ptr);
        }        
        break;
       
    case VP_PAUSE_REQ:
        if (fsm_ptr->is_to_pause)  //此时需要先停止底层播放，防止占用外面空间
        {
            MMISRVAUD_Pause(fsm_ptr->srv_handle);
        }
        result = FALSE;
        break;

    case VP_STRM_SERVER_CLOSE_IND:    
    case VP_PDP_DEACTIVE_IND:
    case VP_STOP_REQ:   
        if(VP_PDP_DEACTIVE_IND == msg_ptr->msg_id)
        {
            SendFatalErrorIndTOUI(fsm_ptr,VP_ERROR_PDP_DEACTIVE_IND);
        }
        else if(VP_STRM_SERVER_CLOSE_IND == msg_ptr->msg_id)
        {
            SendFatalErrorIndTOUI(fsm_ptr,VP_ERROR_SERVER_CLOSE_IND);            
        }
        
        HandleStopReq(fsm_ptr,TRUE);    
        FsmStateTrans(fsm_ptr,VP_STATE_STOPING);
        break;    
    

    case VP_STRM_DATA_TIMEOUT_IND:
        //给ui发送数据超时的消息
        SendFatalErrorIndTOUI(fsm_ptr,VP_ERROR_DATA_TIMEOUT);
        //停止当前播放
        MMIAPIVP_FSMStop((VP_HANDLE)fsm_ptr);       
        break;
        
    case VP_AUTHIND_REQ:        
        //转入VP_STATE_PLAY_AUTH状态，等待ui的鉴权信息
        FsmStateTrans(fsm_ptr,VP_STATE_PLAY_AUTH);
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
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN FsmPlayAuth(VP_FSM_T *fsm_ptr, VP_FSM_MSG_T *msg_ptr)
{
#ifdef MMI_STREAMING_PLAYER_SUPPORT
    VP_AUTH_INFO_T          *auth_info_ptr  = PNULL;
    STRM_AUTH_INFO_T        auth_info       = {0};
    BOOLEAN                 result          = TRUE;

    
    //参数检查
    if(PNULL == fsm_ptr || !IsValidFsm(fsm_ptr) || PNULL == fsm_ptr->vpcallback || PNULL == msg_ptr)
    {
        //VP_STRACE_LOW:"[VP] FsmPlayAuth param is error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_3332_112_2_18_3_5_10_126,(uint8*)"");
        return FALSE;
    }

    
    //VP_STRACE_LOW:"[VP] FsmPlayAuth msg_ptr->msg_id = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_3337_112_2_18_3_5_10_127,(uint8*)"d",msg_ptr->msg_id);
    switch(msg_ptr->msg_id)
    {
    case VP_AUTH_RSP:
        //获取鉴权信息
        auth_info_ptr = (VP_AUTH_INFO_T *)msg_ptr->msg_body;
        if (PNULL == auth_info_ptr)
        {
            result = FALSE;
        }
        else
        {
            auth_info.password_ptr = auth_info_ptr->password_ptr;
            auth_info.username_ptr = auth_info_ptr->username_ptr;
            //VP_STRACE_LOW:"[VP] FsmPlayAuth auth_info.password_ptr= %x, auth_info.username_ptr = %x"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_3353_112_2_18_3_5_10_128,(uint8*)"dd",auth_info.password_ptr,auth_info.username_ptr);
            //将鉴权信息传给streaming处理
            result = STRM_SetupAuthRsp(fsm_ptr->strm_handle,&auth_info);
        }
        if (result)
        {
            FsmStateTrans(fsm_ptr,VP_STATE_PLAY_PENDING);
        }
        else
        {
            SendPlayCnfToUI(fsm_ptr,VP_ERROR_UNAUTHORIZED);
            MMIAPIVP_FSMStop((VP_HANDLE)fsm_ptr);
            result = TRUE;
        }    
        break;
        
    case VP_STRM_SERVER_CLOSE_IND:
    case VP_PDP_DEACTIVE_IND:
    case VP_STOP_REQ:
        if(VP_PDP_DEACTIVE_IND == msg_ptr->msg_id)
        {
            SendFatalErrorIndTOUI(fsm_ptr,VP_ERROR_PDP_DEACTIVE_IND);
        }
        else if(VP_STRM_SERVER_CLOSE_IND == msg_ptr->msg_id)
        {
            SendFatalErrorIndTOUI(fsm_ptr,VP_ERROR_SERVER_CLOSE_IND);            
        }
       
        HandleStopReq(fsm_ptr,TRUE);
        FsmStateTrans(fsm_ptr,VP_STATE_STOPING);
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
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN FsmPlaying(VP_FSM_T *fsm_ptr, VP_FSM_MSG_T *msg_ptr)
{    
    BOOLEAN             result      =   TRUE; 
    VP_PLAY_END_RESULT_E               i_type = VP_PLAY_END_END_SUCCESS;
#ifndef MMI_VIDEOPLAYER_MINI_FUNCTION    
    VP_ERROR_E         Play_result = VP_ERROR_NONE;   
#endif


    //参数检查
    if(PNULL == fsm_ptr || !IsValidFsm(fsm_ptr) || PNULL == fsm_ptr->vpcallback || PNULL == msg_ptr)
    {
        //VP_STRACE_LOW:"[VP] FsmPlaying param is error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_3409_112_2_18_3_5_10_129,(uint8*)"");
        return FALSE;
    }
    
    
    switch(msg_ptr->msg_id)
    {
    case VP_ENTRY_REQ:
      //  CallbackFrameEndNotify(1);
        //VP_STRACE_LOW:"[VP] FsmPlaying VP_ENTRY_REQ fsm_ptr->is_to_pause %x,fsm_ptr->has_play_end_reason = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_3418_112_2_18_3_5_10_130,(uint8*)"dd",fsm_ptr->is_to_pause,fsm_ptr->has_play_end_reason);
        if(fsm_ptr->is_to_pause)
        {
            fsm_ptr->is_to_pause = FALSE;             
            if(HandlePauseReqEx(TRUE, fsm_ptr))
            {
                if (VP_PLAY_FROM_NET == fsm_ptr->play_type)
                {
                    FsmStateTrans(fsm_ptr,VP_STATE_PAUSING);
                }
                else
                {
                    FsmStateTrans(fsm_ptr,VP_STATE_PAUSED);                    
                }
            }
        }
        else if (VP_PLAY_END_NONE != fsm_ptr->has_play_end_reason)
        { 
            FsmDispatch((VP_HANDLE)fsm_ptr,VP_CALLBACK_END_IND,&(fsm_ptr->has_play_end_reason),sizeof(VP_PLAY_END_RESULT_E));
            
        }
        break;    

#ifdef MMI_STREAMING_PLAYER_SUPPORT 
    case VP_STRM_BUFFERING_IND:
        //VP_STRACE_LOW:"[VP] FsmPlaying VP_STRM_BUFFERING_IND"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_3443_112_2_18_3_5_10_131,(uint8*)"");
        SendBufferingIndToUI(fsm_ptr);   
        FsmStateTrans(fsm_ptr,VP_STATE_BUFFERING);             
        break;
#endif

    case VP_CALLBACK_END_IND:
        //VP_STRACE_LOW:"[VP] FsmPlaying VP_CALLBACK_END_IND"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_3450_112_2_18_3_5_10_132,(uint8*)"");
        fsm_ptr->has_play_end_reason = VP_PLAY_END_NONE;

        if (PNULL != msg_ptr->msg_body)
        {
            i_type = *(VP_PLAY_END_RESULT_E *)msg_ptr->msg_body;
        }
        if (VP_PLAY_END_STOPPED_BY_USER == i_type)
        {
            break;
        }
        
        //给ui发送VP_PLAY_END_IND消息   
       // DPLAYER_UpdateCurrentFrame(fsm_ptr->dplayer_handle);
             

        //VP_STRACE_LOW:"[VP] FsmPlaying fsm_ptr->play_type = %d , fsm_ptr->mode = %d, fsm_ptr->is_random_play = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_3466_112_2_18_3_5_10_133,(uint8*)"ddd",fsm_ptr->play_type,fsm_ptr->mode,fsm_ptr->is_random_play);
        if (VP_PLAY_FROM_BUFFER == fsm_ptr->play_type)
        {
            //关闭dplayer,进入ready状态
            DPLAYER_Close(fsm_ptr->dplayer_handle);
            fsm_ptr->dplayer_handle = 0;
            DPLAYER_Release();
            SendPlayEndToUI(fsm_ptr,i_type);   
            FsmStateTrans(fsm_ptr,VP_STATE_READY);
            //给ui发送VP_STOP_IND消息        
            SendStopIndToUI(fsm_ptr);
        }

#ifdef MMI_STREAMING_PLAYER_SUPPORT 
        else if(VP_PLAY_FROM_NET == fsm_ptr->play_type)
        {
            //停止当前播放
            DPLAYER_Close(fsm_ptr->dplayer_handle);
            fsm_ptr->dplayer_handle = 0;
            DPLAYER_Release();
            SendPlayEndToUI(fsm_ptr,i_type);   
            STRM_Stop(fsm_ptr->strm_handle);   
            FsmStateTrans(fsm_ptr,VP_STATE_STOPING);
            
        }
#endif
        else if (VP_PLAY_FROM_FILE == fsm_ptr->play_type)
        {
#ifdef  MMI_KING_MOVIE_SUPPORT 
            VP_dplayerExtCtlDeltSecurty(fsm_ptr ->full_path_ptr,fsm_ptr->dplayer_handle, fsm_ptr->sec_handle);
#endif
            DPLAYER_Close(fsm_ptr->dplayer_handle);
#ifdef  MMI_KING_MOVIE_SUPPORT 
            VP_dplayerCloseSecurty(fsm_ptr->full_path_ptr,fsm_ptr->sec_handle,fsm_ptr->b_sprd_kmv, PNULL);
            fsm_ptr->sec_handle = PNULL;
#endif
            fsm_ptr->dplayer_handle = 0;
            DPLAYER_Release();
            SendPlayEndToUI(fsm_ptr,i_type);   
            if ((VP_PLAY_END_END_SUCCESS != i_type) || (VP_REPEAT_PLAY_MODE_CLOSE == fsm_ptr->mode) || (VP_ERROR_NONE != PlayLocalFile(fsm_ptr,FALSE)))
            {
                FsmStateTrans(fsm_ptr,VP_STATE_READY);
                //给ui发送VP_STOP_IND消息        
                SendStopIndToUI(fsm_ptr);
            }
            else
            {
                SendPlayCnfToUI(fsm_ptr,VP_ERROR_NONE);
            }
        }
        else 
        {
#ifdef  MMI_KING_MOVIE_SUPPORT 
            VP_dplayerExtCtlDeltSecurty(fsm_ptr ->full_path_ptr,fsm_ptr->dplayer_handle, fsm_ptr->sec_handle);
#endif
            DPLAYER_Close(fsm_ptr->dplayer_handle);
#ifdef  MMI_KING_MOVIE_SUPPORT 
            VP_dplayerCloseSecurty(fsm_ptr->full_path_ptr,fsm_ptr->sec_handle,fsm_ptr->b_sprd_kmv, PNULL);
            fsm_ptr->sec_handle = PNULL;
#endif
            fsm_ptr->dplayer_handle = 0;
            DPLAYER_Release();
            SendPlayEndToUI(fsm_ptr,i_type);   
            //fsm_ptr->mode = VP_Setting_GetRepeatPlayMode();
            //fsm_ptr->is_random_play = VP_Setting_GetIsRandomPlayMode();
            #ifndef MMI_VIDEOPLAYER_MINI_FUNCTION
            if (VP_REPEAT_PLAY_MODE_ONE != fsm_ptr->mode)
            {
                //销毁正在播放列表                
                FsmStateTrans(fsm_ptr,VP_STATE_READY);
                NotifyPlayNextFile(fsm_ptr,VP_PLAY_END_END_SUCCESS);
            }
            else 
            {
                VP_LocalList_SetIsCurPlayListItemOK(TRUE);       
                            
                Play_result = PlayLocalFile(fsm_ptr,TRUE);
                if(VP_ERROR_NONE != Play_result)
                {  
                    VP_LocalList_DestroyPlayList(); 
                    SendPlayCnfToUI(fsm_ptr,Play_result);
                    //给ui发送VP_STOP_IND消息     
                    FsmStateTrans(fsm_ptr,VP_STATE_READY);
                    SendStopIndToUI(fsm_ptr);
                }
                else
                {  
                    VP_LocalList_DestroyPlayList(); 
                    SendPlayCnfToUI(fsm_ptr,VP_ERROR_NONE);
                }                
            }   
            #endif
        }        
        break;

    case VP_FRAME_END_NEED_DATA:
        HandleGetFrameData(fsm_ptr);
        break;
        
    case VP_FRAME_END_IND:       
        HandleUpdateFrameReq(fsm_ptr);        
        break; 
        
    case VP_PAUSE_REQ:
        //VP_STRACE_LOW:"[VP] FsmPlaying VP_PAUSE_REQ, fsm_ptr->is_not_notify_ui = %d, fsm_ptr->is_to_pause = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_3582_112_2_18_3_5_10_134,(uint8*)"dd",fsm_ptr->is_not_notify_ui,fsm_ptr->is_to_pause);
        if (fsm_ptr->is_to_pause)
        {
            fsm_ptr->is_to_pause = FALSE;
            if(HandlePauseReqEx(TRUE, fsm_ptr))
            {
#ifdef MMI_STREAMING_PLAYER_SUPPORT 
                if (VP_PLAY_FROM_NET == fsm_ptr->play_type)
                {
                    fsm_ptr->is_not_notify_ui = TRUE;
                    FsmStateTrans(fsm_ptr,VP_STATE_PAUSING);
                }
                else
#endif
                {
                    FsmStateTrans(fsm_ptr,VP_STATE_PAUSED);                    
                }
            }
        }
        else if(result = HandlePauseReq(fsm_ptr))  /*lint !e720*/
        {
            fsm_ptr->is_not_notify_ui = FALSE;
#ifdef MMI_STREAMING_PLAYER_SUPPORT 
            if (VP_PLAY_FROM_NET == fsm_ptr->play_type)
            {
                FsmStateTrans(fsm_ptr,VP_STATE_PAUSING);
            }
            else
#endif
            {
                SendPauseCnfToUI(fsm_ptr,VP_ERROR_NONE);
                FsmStateTrans(fsm_ptr,VP_STATE_PAUSED);                
            }
        }
        break;
        
    case VP_SEEK_REQ:
        //VP_STRACE_LOW:"[VP] FsmPlaying VP_SEEK_REQ"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_3619_112_2_18_3_5_10_135,(uint8*)"");
        if(HandleSeekReq(fsm_ptr, (VP_SEEK_PARAM_T*)(msg_ptr->msg_body)))
        {
            if (VP_PLAY_FROM_NET != fsm_ptr->play_type
				|| !fsm_ptr->is_seek_able)
            {
                SendSeekCnfToUI(fsm_ptr,VP_ERROR_NONE); 
                FsmStateTrans(fsm_ptr,VP_STATE_PLAYING);
                               
            }
#ifdef MMI_STREAMING_PLAYER_SUPPORT 
            else
            {
                //通知streaming seek,进入seeking状态                   
                FsmStateTrans(fsm_ptr,VP_STATE_SEEKING);               
            }
#endif

        }
        break;
        
    case VP_STRM_SERVER_CLOSE_IND:
    case VP_PDP_DEACTIVE_IND:
    case VP_STRM_GOODBYE_IND:
    case VP_STOP_REQ:
        fsm_ptr->has_play_end_reason = VP_PLAY_END_NONE;
        if(VP_STRM_GOODBYE_IND ==  msg_ptr->msg_id)
        {        
            SendPlayEndToUI(fsm_ptr,VP_PLAY_END_END_SUCCESS);
        }
        else if(VP_PDP_DEACTIVE_IND == msg_ptr->msg_id)
        {
            SendFatalErrorIndTOUI(fsm_ptr,VP_ERROR_PDP_DEACTIVE_IND);
        }
        else if(VP_STRM_SERVER_CLOSE_IND == msg_ptr->msg_id)
        {
            SendFatalErrorIndTOUI(fsm_ptr,VP_ERROR_SERVER_CLOSE_IND);            
        }
        
        //VP_STRACE_LOW:"[VP] FsmPlaying VP_STOP_REQ"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_3657_112_2_18_3_5_10_136,(uint8*)"");
        HandleStopReq(fsm_ptr,TRUE);
#ifdef MMI_STREAMING_PLAYER_SUPPORT 
        if (VP_PLAY_FROM_NET == fsm_ptr->play_type)
        {
            FsmStateTrans(fsm_ptr,VP_STATE_STOPING);
        }
        else
#endif
        {
            FsmStateTrans(fsm_ptr,VP_STATE_READY);
            SendStopIndToUI(fsm_ptr);
        }
        break;
#ifdef MMI_STREAMING_PLAYER_SUPPORT 
   case VP_STRM_DATA_TIMEOUT_IND:
       //VP_STRACE_LOW:"[VP] FsmPlaying VP_STRM_DATA_TIMEOUT_IND"
       SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_3673_112_2_18_3_5_10_137,(uint8*)"");
        //给ui发送数据超时的消息
        SendFatalErrorIndTOUI(fsm_ptr,VP_ERROR_DATA_TIMEOUT);
        //停止当前播放
        MMIAPIVP_FSMStop((VP_HANDLE)fsm_ptr);       
        break;
#endif

    case VP_RESUME_REQ:
        //VP_STRACE_LOW:"[VP] FsmPlaying VP_RESUME_REQ"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_3682_112_2_18_3_5_10_138,(uint8*)"");
        SendPlayCnfToUI(fsm_ptr,VP_ERROR_NONE);
        break;

    case VP_START_SET_RATE:
        if(PNULL != msg_ptr->msg_body)
        {
            if (*(BOOLEAN *) msg_ptr->msg_body)
            {
                DPLAYER_SetPlayRate(fsm_ptr->dplayer_handle,DPLAYER_PLAY_RATE_BACKWARD_X8);
            }
            else
            {
                DPLAYER_SetPlayRate(fsm_ptr->dplayer_handle,DPLAYER_PLAY_RATE_X8);
            }
        }        
        break;

    case VP_STOP_SET_RATE:
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
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN FsmBuffering(VP_FSM_T *fsm_ptr, VP_FSM_MSG_T *msg_ptr)
{
#ifdef MMI_STREAMING_PLAYER_SUPPORT
    BOOLEAN              result     = TRUE;
    

    //参数检查
    if(PNULL == fsm_ptr || !IsValidFsm(fsm_ptr) || PNULL == fsm_ptr->vpcallback || PNULL == msg_ptr)
    {
        //VP_STRACE_LOW:"[VP] FsmBuffering param is error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_3724_112_2_18_3_5_10_139,(uint8*)"");
        return FALSE;
    }

    
    //VP_STRACE_LOW:"[VP] FsmBuffering msg_ptr->msg_id = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_3729_112_2_18_3_5_10_140,(uint8*)"d",msg_ptr->msg_id);
    switch(msg_ptr->msg_id)
    {
    case VP_ENTRY_REQ:        
        if(fsm_ptr->is_to_pause)
        {
            fsm_ptr->is_to_pause = FALSE;
            if(HandlePauseReqEx(TRUE, fsm_ptr))
            {
                FsmStateTrans(fsm_ptr,VP_STATE_PAUSING);
            }           
            
        }
        else if (VP_PLAY_END_NONE != fsm_ptr->has_play_end_reason)
        {
            fsm_ptr->has_play_end_reason = VP_PLAY_END_NONE;
            SendPlayEndToUI(fsm_ptr,VP_PLAY_END_END_SUCCESS);
            HandleStopReq(fsm_ptr,TRUE);
            FsmStateTrans(fsm_ptr,VP_STATE_STOPING);
        }
        break;

    case VP_STRM_BUFFER_END_IND:
        //进入playing状态
        SendBufferEndIndToUI(fsm_ptr); 
        FsmStateTrans(fsm_ptr,VP_STATE_PLAYING);             
        break;
        
    case VP_BUFFER_PERCENT:
        SendBufferPercentToUI(fsm_ptr);      
        break;
        
    case VP_STRM_DATA_TIMEOUT_IND:
        //给ui发送数据超时的消息
        SendFatalErrorIndTOUI(fsm_ptr,VP_ERROR_DATA_TIMEOUT);
        //停止当前播放
        MMIAPIVP_FSMStop((VP_HANDLE)fsm_ptr);       
        break;
        
    case VP_STRM_SERVER_CLOSE_IND:
    case VP_PDP_DEACTIVE_IND:
    case VP_STRM_GOODBYE_IND:
    case VP_STOP_REQ:
        fsm_ptr->has_play_end_reason = VP_PLAY_END_NONE;
        if(VP_STRM_GOODBYE_IND ==  msg_ptr->msg_id)
        {        
            SendPlayEndToUI(fsm_ptr,VP_PLAY_END_END_SUCCESS);
        }
        else if(VP_PDP_DEACTIVE_IND == msg_ptr->msg_id)
        {
            SendFatalErrorIndTOUI(fsm_ptr,VP_ERROR_PDP_DEACTIVE_IND);
        }
        else if(VP_STRM_SERVER_CLOSE_IND == msg_ptr->msg_id)
        {
            SendFatalErrorIndTOUI(fsm_ptr,VP_ERROR_SERVER_CLOSE_IND);            
        }
        
        HandleStopReq(fsm_ptr,TRUE);
        FsmStateTrans(fsm_ptr,VP_STATE_STOPING);
        break;

     case VP_FRAME_END_NEED_DATA:
        HandleGetFrameData(fsm_ptr);
        break;

     case VP_FRAME_END_IND:               
        HandleUpdateFrameReq(fsm_ptr);  
        break;
        
    case VP_PAUSE_REQ:
        if (fsm_ptr->is_to_pause)
        {
            fsm_ptr->is_to_pause = FALSE;
            if(HandlePauseReqEx(TRUE, fsm_ptr))
            {
                fsm_ptr->is_not_notify_ui = TRUE;
                FsmStateTrans(fsm_ptr,VP_STATE_PAUSING);
            }
        }
        else if(HandlePauseReq(fsm_ptr))
        {
            fsm_ptr->is_not_notify_ui = FALSE;
            FsmStateTrans(fsm_ptr,VP_STATE_PAUSING);
        }            
        break;

    case VP_SEEK_REQ:
        if(HandleSeekReq(fsm_ptr, (VP_SEEK_PARAM_T*)(msg_ptr->msg_body)))
        {
            if (VP_PLAY_FROM_NET != fsm_ptr->play_type
				|| !fsm_ptr->is_seek_able)
            {
                SendSeekCnfToUI(fsm_ptr,VP_ERROR_NONE);      
                FsmStateTrans(fsm_ptr,VP_STATE_PLAYING);                          
            }
            else
            {
                //通知streaming seek,进入seeking状态                   
                FsmStateTrans(fsm_ptr,VP_STATE_SEEKING);               
            }

        }
        break;

   case VP_CALLBACK_END_IND:     
        if (PNULL != msg_ptr->msg_body)
        {
            fsm_ptr->has_play_end_reason = *(VP_PLAY_END_RESULT_E *)msg_ptr->msg_body;
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
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN FsmSeeking(VP_FSM_T *fsm_ptr, VP_FSM_MSG_T *msg_ptr)
{
#ifdef MMI_STREAMING_PLAYER_SUPPORT
    STRM_RESULT_E       strm_result     = 0;
    BOOLEAN             result          = TRUE;
    
    
    

    //参数检查
    if(PNULL == fsm_ptr || !IsValidFsm(fsm_ptr) || PNULL == fsm_ptr->vpcallback || PNULL == msg_ptr)
    {
        //VP_STRACE_LOW:"[VP] FsmSeeking param is error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_3865_112_2_18_3_5_11_141,(uint8*)"");
        return FALSE;
    }

    //VP_STRACE_LOW:"[VP] FsmSeeking  msg_ptr->msg_id = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_3869_112_2_18_3_5_11_142,(uint8*)"d",msg_ptr->msg_id);
    switch(msg_ptr->msg_id)
    {
    case VP_STRM_SEEK_CNF:
        if (PNULL != msg_ptr->msg_body)
        {
            strm_result = *(STRM_RESULT_E *)msg_ptr->msg_body;
        }
        else
        {
            strm_result = STRM_RESULT_UNKNOWN;
        }

        //VP_STRACE_LOW:"[VP] FsmSeeking strm_result = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_3882_112_2_18_3_5_11_143,(uint8*)"d",strm_result);
        
        switch(strm_result) 
        {
        case STRM_RESULT_SUCCESS:
            //成功seeking，进入playing状态  
            HandleUpdateFrameReq(fsm_ptr);       
            SendSeekCnfToUI(fsm_ptr,VP_ERROR_NONE);            
            break;            
        
        default:
            //给ui发送其他错误，停止播放
            SendSeekCnfToUI(fsm_ptr,VP_ERROR_SEEK_ERROR);   
            break;
        }     
        FsmStateTrans(fsm_ptr,VP_STATE_PLAYING);
        break;
        
    case VP_STRM_SERVER_CLOSE_IND:
    case VP_PDP_DEACTIVE_IND:  
    case VP_STOP_REQ:
        if(VP_PDP_DEACTIVE_IND == msg_ptr->msg_id)
        {
            SendFatalErrorIndTOUI(fsm_ptr,VP_ERROR_PDP_DEACTIVE_IND);
        }
        else if(VP_STRM_SERVER_CLOSE_IND == msg_ptr->msg_id)
        {
            SendFatalErrorIndTOUI(fsm_ptr,VP_ERROR_SERVER_CLOSE_IND);            
        }
        HandleStopReq(fsm_ptr,TRUE);
        FsmStateTrans(fsm_ptr,VP_STATE_STOPING);
        break;

   case VP_PAUSE_REQ:
        if (fsm_ptr->is_to_pause)  //此时需要先停止底层播放，防止占用外面空间
        {
            MMISRVAUD_Pause(fsm_ptr->srv_handle); 
        }
        result = FALSE;
        break;

    case VP_STRM_DATA_TIMEOUT_IND:
        //给ui发送数据超时的消息
        SendFatalErrorIndTOUI(fsm_ptr,VP_ERROR_DATA_TIMEOUT);
        //停止当前播放
        MMIAPIVP_FSMStop((VP_HANDLE)fsm_ptr);       
        break;

    case VP_STRM_GOODBYE_IND:    
    case VP_CALLBACK_END_IND:     
        if (PNULL != msg_ptr->msg_body)
        {
            fsm_ptr->has_play_end_reason = *(VP_PLAY_END_RESULT_E *)msg_ptr->msg_body;
        }
        else
        {
            fsm_ptr->has_play_end_reason = VP_PLAY_END_END_SUCCESS;
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
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN FsmPausing(VP_FSM_T *fsm_ptr, VP_FSM_MSG_T *msg_ptr)
{
#ifdef MMI_STREAMING_PLAYER_SUPPORT
    STRM_RESULT_E       strm_result     = 0;
    BOOLEAN             result          = TRUE;
    
    //参数检查
    if(PNULL == fsm_ptr || !IsValidFsm(fsm_ptr) || PNULL == fsm_ptr->vpcallback || PNULL == msg_ptr)
    {
        //VP_STRACE_LOW:"[VP] FsmPausing param is error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_3965_112_2_18_3_5_11_144,(uint8*)"");
        return FALSE;
    }
    
    //VP_STRACE_LOW:"[VP] FsmPausing msg_ptr->msg_id = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_3969_112_2_18_3_5_11_145,(uint8*)"d",msg_ptr->msg_id);
    switch(msg_ptr->msg_id)
    {
    case VP_STRM_PAUSE_CNF:
        if (PNULL == msg_ptr->msg_body)
        {
            strm_result = STRM_RESULT_UNKNOWN;
        }
        else
        {
            strm_result = *(STRM_RESULT_E *)msg_ptr->msg_body;
        }

        //VP_STRACE_LOW:"[VP] FsmPausing strm_result = %d, fsm_ptr->is_not_notify_ui = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_3982_112_2_18_3_5_11_146,(uint8*)"dd",strm_result,fsm_ptr->is_not_notify_ui);
        switch(strm_result) 
        {
        case STRM_RESULT_SUCCESS:
            //成功pause，进入paused状态
            fsm_ptr->process_time = DPLAYER_GetTime(fsm_ptr->dplayer_handle);            
            if (!fsm_ptr->is_not_notify_ui)
            {        
                fsm_ptr->is_not_notify_ui= FALSE;
                //VP_STRACE_LOW:"[VP] FsmPausing send pause cnf"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_3991_112_2_18_3_5_11_147,(uint8*)"");
                SendPauseCnfToUI(fsm_ptr,VP_ERROR_NONE);          
            }
            FsmStateTrans(fsm_ptr,VP_STATE_PAUSED);
            break;            
            
        default:
            fsm_ptr->is_not_notify_ui = FALSE;
            //给ui发送其他错误，停止播放    
            SendFatalErrorIndTOUI(fsm_ptr,VP_ERROR_OTHERS);
            MMIAPIVP_FSMStop((VP_HANDLE)fsm_ptr);            
            break;
        }
        break;
        
    case VP_STRM_SERVER_CLOSE_IND:
    case VP_PDP_DEACTIVE_IND:   
    case VP_STOP_REQ:
         fsm_ptr->is_not_notify_ui = FALSE;
#ifdef MMI_STREAMING_PLAYER_SUPPORT 
         if(VP_PDP_DEACTIVE_IND == msg_ptr->msg_id)
        {
            SendFatalErrorIndTOUI(fsm_ptr,VP_ERROR_PDP_DEACTIVE_IND);
        }
        else if(VP_STRM_SERVER_CLOSE_IND == msg_ptr->msg_id)
        {
            SendFatalErrorIndTOUI(fsm_ptr,VP_ERROR_SERVER_CLOSE_IND);            
        }
		else if(!strm_result)
		{
            HandleStopReq(fsm_ptr, TRUE);
            STRM_Close(fsm_ptr->strm_handle);
			FsmStateTrans(fsm_ptr, VP_STATE_READY);
            SendStopIndToUI(fsm_ptr);
			SCI_TRACE_LOW("[VP] FsmPausing strm_result error");
			break;
		}
#endif
        HandleStopReq(fsm_ptr,TRUE);    
        FsmStateTrans(fsm_ptr,VP_STATE_STOPING);
        break;        
   
    case VP_STRM_GOODBYE_IND:    
    case VP_CALLBACK_END_IND:     
        if (PNULL != msg_ptr->msg_body)
        {
            fsm_ptr->has_play_end_reason = *(VP_PLAY_END_RESULT_E *)msg_ptr->msg_body;
        }
        else
        {
            fsm_ptr->has_play_end_reason = VP_PLAY_END_END_SUCCESS;
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
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN FsmPaused(VP_FSM_T *fsm_ptr, VP_FSM_MSG_T *msg_ptr)
{
    BOOLEAN                 result = TRUE;   

    //参数检查
    if(PNULL == fsm_ptr || !IsValidFsm(fsm_ptr) || PNULL == fsm_ptr->vpcallback || PNULL == msg_ptr)
    {
        //VP_STRACE_LOW:"[VP] FsmPaused param is error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_4057_112_2_18_3_5_11_148,(uint8*)"");
        return FALSE;
    }
    
    //VP_STRACE_LOW:"[VP] FsmPaused msg_ptr->msg_id = %x, is_to_pause = %d, is_to_resume = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_4061_112_2_18_3_5_11_149,(uint8*)"ddd",msg_ptr->msg_id,fsm_ptr->is_to_pause,fsm_ptr->is_to_resume);
    switch(msg_ptr->msg_id)
    {
    case VP_ENTRY_REQ:       
        if(fsm_ptr->is_to_pause)
        {
            fsm_ptr->is_to_pause = FALSE;
            if(HandlePauseReqEx(FALSE, fsm_ptr))
            {
                FsmStateTrans(fsm_ptr,VP_STATE_PAUSED_EX);
            }    
        }
        else if(fsm_ptr->is_to_resume)
        {
            //清除强制恢复标志
            fsm_ptr->is_to_resume = FALSE;      
            if(HandleResumeReqEx(TRUE,fsm_ptr))
            {
#ifdef MMI_STREAMING_PLAYER_SUPPORT 
                if (VP_PLAY_FROM_NET == fsm_ptr->play_type)
                {
                    //网络部分恢复streaming播放，进入resuming状态               
                    FsmStateTrans(fsm_ptr,VP_STATE_RESUMING);                
                }
                else
#endif
                {
                    //本地播放进入playing状态
                    FsmStateTrans(fsm_ptr,VP_STATE_PLAYING);               
                }       
            }
        }
        break;

    case VP_PAUSE_REQ:
        if(fsm_ptr->is_to_pause)
        {
            fsm_ptr->is_to_pause = FALSE;
            if (HandlePauseReqEx(FALSE,fsm_ptr))
            {
                FsmStateTrans(fsm_ptr,VP_STATE_PAUSED_EX);
            }
            
        }
        break;        
    

    case VP_SEEK_REQ:
        //VP_STRACE_LOW:"[VP] FsmPaused VP_SEEK_REQ"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_4109_112_2_18_3_5_11_150,(uint8*)"");
// #ifdef  CAMERA_SUPPORT
        MMISRVAUD_ResumeSlightly(fsm_ptr->srv_handle);
// #endif
        if(HandleSeekReq(fsm_ptr, (VP_SEEK_PARAM_T*)(msg_ptr->msg_body)))
        {
            FsmStateTrans(fsm_ptr,VP_STATE_PAUSED_SEEKING);       
        }
        break;

    case VP_RESUME_REQ:
        if(fsm_ptr->is_to_resume)
        {
            fsm_ptr->is_to_resume = FALSE;
            if (HandleResumeReqEx(TRUE,fsm_ptr))
            {      
#ifdef MMI_STREAMING_PLAYER_SUPPORT 
                if (VP_PLAY_FROM_NET == fsm_ptr->play_type)
                {
                    //网络部分恢复streaming播放，进入resuming状态               
                    FsmStateTrans(fsm_ptr,VP_STATE_RESUMING);                
                }
                else
#endif
                {
                    //本地播放进入playing状态
                    FsmStateTrans(fsm_ptr,VP_STATE_PLAYING);               
                }   
            }
        }
        else if(HandleResumeReq(fsm_ptr))
        {
#ifdef MMI_STREAMING_PLAYER_SUPPORT 
            if (VP_PLAY_FROM_NET == fsm_ptr->play_type)
            {
                //网络部分恢复streaming播放，进入resuming状态               
                FsmStateTrans(fsm_ptr,VP_STATE_RESUMING);                
            }
            else
#endif
            {
                //本地播放进入playing状态
                SendPlayCnfToUI(fsm_ptr,VP_ERROR_NONE);
                FsmStateTrans(fsm_ptr,VP_STATE_PLAYING);
                
            }       
        }
        break;
        
    case VP_STRM_SERVER_CLOSE_IND:
    case VP_PDP_DEACTIVE_IND:
    case VP_STOP_REQ:
        if(VP_PDP_DEACTIVE_IND == msg_ptr->msg_id)
        {
            SendFatalErrorIndTOUI(fsm_ptr,VP_ERROR_PDP_DEACTIVE_IND);
        }
        else if(VP_STRM_SERVER_CLOSE_IND == msg_ptr->msg_id)
        {
            SendFatalErrorIndTOUI(fsm_ptr,VP_ERROR_SERVER_CLOSE_IND);            
        }
         HandleStopReq(fsm_ptr,TRUE); 
#ifdef MMI_STREAMING_PLAYER_SUPPORT 
         if (VP_PLAY_FROM_NET == fsm_ptr->play_type)
         {
             FsmStateTrans(fsm_ptr,VP_STATE_STOPING);
         }
         else
#endif
         {
            FsmStateTrans(fsm_ptr,VP_STATE_READY);
            SendStopIndToUI(fsm_ptr);
         }
        break;  
        
    case VP_STOP_SET_RATE:
        DPLAYER_SetPlayRate(fsm_ptr->dplayer_handle,DPLAYER_PLAY_RATE_NORMAL);
        break;
    
    case VP_STRM_GOODBYE_IND:    
    case VP_CALLBACK_END_IND:     
        if (PNULL != msg_ptr->msg_body)
        {
            fsm_ptr->has_play_end_reason = *(VP_PLAY_END_RESULT_E *)msg_ptr->msg_body;
        }
        else
        {
            fsm_ptr->has_play_end_reason = VP_PLAY_END_END_SUCCESS;
        }
        break;
        
    default:
        SCI_TRACE_LOW("[VP] FsmPaused msg_ptr->msg_id = %x wrong msg!", msg_ptr->msg_id);
        result = FALSE;
        break;
    }
    return result;
}

/*****************************************************************************/
//  Description : 暂停中播放定位中
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN FsmPausedSeeking(VP_FSM_T *fsm_ptr, VP_FSM_MSG_T *msg_ptr)
{
#ifdef MMI_STREAMING_PLAYER_SUPPORT
    STRM_RESULT_E       strm_result     = 0;
#endif
    BOOLEAN             result          = TRUE;
    
    
    

    //参数检查
    if(PNULL == fsm_ptr || !IsValidFsm(fsm_ptr) || PNULL == fsm_ptr->vpcallback || PNULL == msg_ptr)
    {
        //VP_STRACE_LOW:"[VP] FsmPausedSeeking param is error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_4218_112_2_18_3_5_11_151,(uint8*)"");
        return FALSE;
    }

    //VP_STRACE_LOW:"[VP] FsmPausedSeeking  msg_ptr->msg_id = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_4222_112_2_18_3_5_11_152,(uint8*)"d",msg_ptr->msg_id);
    switch(msg_ptr->msg_id)
    {
    case VP_ENTRY_REQ:
        if (VP_PLAY_FROM_NET != fsm_ptr->play_type
			|| !fsm_ptr->is_seek_able)
        {            
            HandleUpdateFrameReq(fsm_ptr);       
            SendSeekCnfToUI(fsm_ptr,VP_ERROR_NONE);  
            FsmStateTrans(fsm_ptr,VP_STATE_PLAYING);
        }
        break;
#ifdef MMI_STREAMING_PLAYER_SUPPORT
    case VP_STRM_SEEK_CNF:
        if (PNULL != msg_ptr->msg_body)
        {
            strm_result = *(STRM_RESULT_E *)msg_ptr->msg_body;
        }
        else
        {
            strm_result = STRM_RESULT_UNKNOWN;
        }

        //VP_STRACE_LOW:"[VP] FsmSeeking strm_result = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_4244_112_2_18_3_5_11_153,(uint8*)"d",strm_result);
        
        switch(strm_result) 
        {
        case STRM_RESULT_SUCCESS:
            //成功seeking，进入playing状态       
            HandleUpdateFrameReq(fsm_ptr);       
            SendSeekCnfToUI(fsm_ptr,VP_ERROR_NONE);            
            break;            
        
        default:
            //给ui发送其他错误，停止播放
            SendSeekCnfToUI(fsm_ptr,VP_ERROR_SEEK_ERROR);   
            break;
        }  
        FsmStateTrans(fsm_ptr,VP_STATE_PLAYING);
        break;
#endif      
   case VP_PAUSE_REQ:
        if (fsm_ptr->is_to_pause)  //此时需要先停止底层播放，防止占用外面空间
        {
            MMISRVAUD_Pause(fsm_ptr->srv_handle);  
        }
        result = FALSE;
        break;

    case VP_STRM_SERVER_CLOSE_IND:
    case VP_PDP_DEACTIVE_IND:  
    case VP_STOP_REQ:
#ifdef MMI_STREAMING_PLAYER_SUPPORT		
        if(VP_PDP_DEACTIVE_IND == msg_ptr->msg_id)
        {
            SendFatalErrorIndTOUI(fsm_ptr,VP_ERROR_PDP_DEACTIVE_IND);
        }
        else if(VP_STRM_SERVER_CLOSE_IND == msg_ptr->msg_id)
        {
            SendFatalErrorIndTOUI(fsm_ptr,VP_ERROR_SERVER_CLOSE_IND);            
        }
#endif
        HandleStopReq(fsm_ptr,TRUE);
        FsmStateTrans(fsm_ptr,VP_STATE_STOPING);
        break;
#ifdef MMI_STREAMING_PLAYER_SUPPORT
    case VP_STRM_DATA_TIMEOUT_IND:
        //给ui发送数据超时的消息
        SendFatalErrorIndTOUI(fsm_ptr,VP_ERROR_DATA_TIMEOUT);
        //停止当前播放
        MMIAPIVP_FSMStop((VP_HANDLE)fsm_ptr);       
        break;
#endif

    case VP_STRM_GOODBYE_IND:    
    case VP_CALLBACK_END_IND:     
        if (PNULL != msg_ptr->msg_body)
        {
            fsm_ptr->has_play_end_reason = *(VP_PLAY_END_RESULT_E *)msg_ptr->msg_body;
        }
        else
        {
            fsm_ptr->has_play_end_reason = VP_PLAY_END_END_SUCCESS;
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
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN FsmPausedEx(VP_FSM_T *fsm_ptr, VP_FSM_MSG_T *msg_ptr)
{
    BOOLEAN                 result = TRUE;   

    //参数检查
    if(PNULL == fsm_ptr || !IsValidFsm(fsm_ptr) || PNULL == fsm_ptr->vpcallback || PNULL == msg_ptr)
    {
        //VP_STRACE_LOW:"[VP] FsmPaused param is error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_4325_112_2_18_3_5_12_154,(uint8*)"");
        return FALSE;
    }
    
    //VP_STRACE_LOW:"[VP] FsmPaused msg_ptr->msg_id = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_4329_112_2_18_3_5_12_155,(uint8*)"d",msg_ptr->msg_id);
    switch(msg_ptr->msg_id)
    {
    case VP_RESUME_REQ:
        fsm_ptr->is_to_resume = FALSE;
        if(HandleResumeReqEx(FALSE,fsm_ptr))
        {            
            FsmStateTrans(fsm_ptr,VP_STATE_PAUSED);                   
        }
        break;
        
    case VP_STRM_SERVER_CLOSE_IND:
    case VP_PDP_DEACTIVE_IND:
    case VP_STOP_REQ:
#ifdef MMI_STREAMING_PLAYER_SUPPORT
        if(VP_PDP_DEACTIVE_IND == msg_ptr->msg_id)
        {
            SendFatalErrorIndTOUI(fsm_ptr,VP_ERROR_PDP_DEACTIVE_IND);
        }
        else if(VP_STRM_SERVER_CLOSE_IND == msg_ptr->msg_id)
        {
            SendFatalErrorIndTOUI(fsm_ptr,VP_ERROR_SERVER_CLOSE_IND);            
        }
#endif
         HandleStopReq(fsm_ptr,TRUE); 
#ifdef MMI_STREAMING_PLAYER_SUPPORT
         if (VP_PLAY_FROM_NET == fsm_ptr->play_type)
         {
             FsmStateTrans(fsm_ptr,VP_STATE_STOPING);
         }
         else
#endif
         {
            FsmStateTrans(fsm_ptr,VP_STATE_READY);
            SendStopIndToUI(fsm_ptr);
         }
        break;  

    case VP_STRM_GOODBYE_IND:    
    case VP_CALLBACK_END_IND:     
        if (PNULL != msg_ptr->msg_body)
        {
            fsm_ptr->has_play_end_reason = *(VP_PLAY_END_RESULT_E *)msg_ptr->msg_body;
        }
        else
        {
            fsm_ptr->has_play_end_reason = VP_PLAY_END_END_SUCCESS;
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
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN FsmResuming(VP_FSM_T *fsm_ptr, VP_FSM_MSG_T *msg_ptr)
{
#ifdef MMI_STREAMING_PLAYER_SUPPORT
    STRM_RESULT_E   strm_result     = 0;
    BOOLEAN         result          = TRUE;    

    //参数检查
    if(PNULL == fsm_ptr || !IsValidFsm(fsm_ptr) || PNULL == fsm_ptr->vpcallback || PNULL == msg_ptr)
    {
        return FALSE;
    }
    
    //VP_STRACE_LOW:"[VP] FsmResuming msg_ptr->msg_id = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_4402_112_2_18_3_5_12_156,(uint8*)"d",msg_ptr->msg_id);
    switch(msg_ptr->msg_id)
    {
    case VP_STRM_RESUME_CNF:
        if (PNULL == msg_ptr->msg_body)
        {
            strm_result = STRM_RESULT_UNKNOWN;
        }
        else
        {
            strm_result = *(STRM_RESULT_E *)msg_ptr->msg_body;
        }
        //VP_STRACE_LOW:"[VP] FsmResuming strm_result = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_4414_112_2_18_3_5_12_157,(uint8*)"d",strm_result);
        switch(strm_result) 
        {
        case STRM_RESULT_SUCCESS:
            //成功resule，进入playing状态
            HandleUpdateFrameReq(fsm_ptr);       
            SendPlayCnfToUI(fsm_ptr,VP_ERROR_NONE); 
            FsmStateTrans(fsm_ptr,VP_STATE_PLAYING);                         
            break;            
        
        default:      
            SendPlayCnfToUI(fsm_ptr,VP_ERROR_CONNECT_TIMEOUT); 
            MMIAPIVP_FSMStop((VP_HANDLE)fsm_ptr);           
            break;
        }
        break;
            
    case VP_STRM_SERVER_CLOSE_IND:   
    case VP_PDP_DEACTIVE_IND:
    case VP_STOP_REQ:
        if(VP_PDP_DEACTIVE_IND == msg_ptr->msg_id)
        {
            SendFatalErrorIndTOUI(fsm_ptr,VP_ERROR_PDP_DEACTIVE_IND);
        }
        else if(VP_STRM_SERVER_CLOSE_IND == msg_ptr->msg_id)
        {
            SendFatalErrorIndTOUI(fsm_ptr,VP_ERROR_SERVER_CLOSE_IND);            
        }
        HandleStopReq(fsm_ptr,TRUE);
        FsmStateTrans(fsm_ptr,VP_STATE_STOPING);
        break;

    case VP_STRM_DATA_TIMEOUT_IND:
        //给ui发送数据超时的消息
        SendFatalErrorIndTOUI(fsm_ptr,VP_ERROR_DATA_TIMEOUT);
        //停止当前播放
        MMIAPIVP_FSMStop((VP_HANDLE)fsm_ptr);       
        break;

    case VP_RESUME_REQ:
        break;

    case VP_STRM_GOODBYE_IND:    
    case VP_CALLBACK_END_IND:     
        if (PNULL != msg_ptr->msg_body)
        {
            fsm_ptr->has_play_end_reason = *(VP_PLAY_END_RESULT_E *)msg_ptr->msg_body;
        }
        else
        {
            fsm_ptr->has_play_end_reason = VP_PLAY_END_END_SUCCESS;
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
//  Description : 处理streaming init cnf消息
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
#ifdef MMI_STREAMING_PLAYER_SUPPORT 
LOCAL STRM_RESULT_E HandleInitCnf(VP_FSM_T * fsm_ptr, STRM_INIT_CNF_T* strm_init_ptr, VP_ERROR_E *is_play_success)
{
  
    DPLAYER_STRM_CFG_T strm_cfg = {0};
    /*cr247685 start*/   
    if (IsVideoPlayerCollision() || PNULL == fsm_ptr || !IsValidFsm(fsm_ptr) || PNULL == strm_init_ptr || PNULL == is_play_success)
    {
        //VP_STRACE_LOW:"[VP] HandleInitCnf param is null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_4490_112_2_18_3_5_12_158,(uint8*)"");
        return STRM_RESULT_INVALID_PARAMETER; //modify by NEWMS00109918.       
    }     
    /*cr247685 end*/   
    
    //VP_STRACE_LOW:"[VP] HandleInitCnf strm_init_ptr->result = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_4495_112_2_18_3_5_12_159,(uint8*)"d",strm_init_ptr->result);
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
        if (PNULL != fsm_ptr->full_path_ptr)
        {
            SCI_FREE(fsm_ptr->full_path_ptr);
            fsm_ptr->full_path_ptr = PNULL;
        }
        if (PNULL != strm_init_ptr->sdp_info.media_name_ptr)
        {
            fsm_ptr->full_path_ptr = (wchar *)SCI_ALLOCA((strlen(strm_init_ptr->sdp_info.media_name_ptr)+1) * sizeof(wchar));
            if(PNULL != fsm_ptr->full_path_ptr)
            {
                SCI_MEMSET(fsm_ptr->full_path_ptr,0,((strlen(strm_init_ptr->sdp_info.media_name_ptr)+1) * sizeof(wchar)));/*lint !e666*/
                MMIAPICOM_StrToWstr(((uint8 *)(strm_init_ptr->sdp_info.media_name_ptr)),fsm_ptr->full_path_ptr);
                fsm_ptr->full_path_len = (uint16)strlen(strm_init_ptr->sdp_info.media_name_ptr);
            }
            else
            {
                fsm_ptr->full_path_len = 0;
            } 
        }
                               
        //VP_STRACE_LOW:"[VP] HandleInitCnf frame height = %d, rate = %d, base = %d,width = %d, cfg_len = %d, fsm_ptr->total_time= %d, strm_cfg.video_cfg.cfg_ptr = %x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_4584_112_2_18_3_5_12_160,(uint8*)"ddddddd",strm_cfg.video_cfg.frame_height,strm_cfg.video_cfg.frame_rate,strm_cfg.video_cfg.frame_rate_base,strm_cfg.video_cfg.frame_width,strm_cfg.video_cfg.cfg_len,fsm_ptr->total_time,strm_cfg.video_cfg.cfg_ptr);
        
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
//  Description : FRAME数据准备好后，获取scaling好的数据
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL void HandleGetFrameData(VP_FSM_T * fsm_ptr)
{
    DPLAYER_IMAGE_DATA_T image_data = {0};

    if (PNULL != fsm_ptr)
    {
        if(fsm_ptr->is_cr_video_layer)
        {
            if (IsValidFsm(fsm_ptr) && !fsm_ptr->is_to_exit && !fsm_ptr->is_to_pause && 1 == fsm_ptr->display_param.videoSupport)
            {
                DPLAYER_GetNextFrameData(fsm_ptr->dplayer_handle,PNULL,&image_data,GetFrameDataCallback);
            }
        }
        else
        {
            //SCI_TRACE_LOW:"[VP]:HandleGetFrameData no video_layer!!!!!!!!!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_4627_112_2_18_3_5_12_161,(uint8*)"");
        }
        
    }

}

/*****************************************************************************/
//  Description : 处理update frame请求消息
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL void HandleUpdateFrameReq(VP_FSM_T * fsm_ptr)
{
#ifdef UI_MULTILAYER_SUPPORT
    UILAYER_INFO_T  layer_info = {0};
    uint8       *layer_buf_ptr = PNULL;  /*lint !e529*/       
    uint16           *dest_buf_ptr = PNULL;/*lint !e529*/
    uint16           *src_buf_ptr = PNULL;/*lint !e529*/
    uint32          i = 0; /*lint !e529*/
    uint32          j = 0;/*lint !e529*/
    uint16          dx=0;/*lint !e529*/
    uint16          dy=0;/*lint !e529*/
    uint16         height = 0;
    uint16         width = 0;
    UILAYER_COPYBUF_PARAM_T     copy_param = {0};
    UILAYER_COPYBUF_T           copy_buf = {0};
#endif
    if (PNULL == fsm_ptr || !IsValidFsm(fsm_ptr))
    {
        //VP_STRACE_LOW:"[VP] HandleUpdateFrameReq param is null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_4653_112_2_18_3_5_12_162,(uint8*)"");
        return;
    }

   // VP_STRACE_LOW("[VP] HandleUpdateFrameReq fsm_ptr->is_to_exit = %d, fsm_ptr->is_to_pause = %d, fsm_ptr->display_param.videoSupport = %d",fsm_ptr->is_to_exit,fsm_ptr->is_to_pause,fsm_ptr->display_param.videoSupport);
    if (!fsm_ptr->is_to_exit && !fsm_ptr->is_to_pause && 1 == fsm_ptr->display_param.videoSupport)
    {
        
        if(fsm_ptr->is_cr_video_layer)
        {
#ifdef UI_MULTILAYER_SUPPORT         
        #if 1
            GUI_LCD_DEV_INFO* s_mainlcd_layer=MMITHEME_GetDefaultLcdDev();/*lint !e605*/
        #else
             GUI_LCD_DEV_INFO* s_mainlcd_layer=&s_video_layer;
        #endif
            height = s_current_fsm_ptr->image_data.coordinate.trim_h;
            width = s_current_fsm_ptr->image_data.coordinate.trim_w;
            //SCI_TRACE_MID:"[VP]:GetFrameDataCallback height=%d,width=%d!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_4670_112_2_18_3_5_12_163,(uint8*)"dd",height,width);

            UILAYER_GetLayerInfo(s_mainlcd_layer, &layer_info);
//#ifdef MMI_VIDEOWALLPAPER_SUPPORT_LOW_MEMORY
            if((layer_info.layer_height >= layer_info.layer_width 
                && height>=width) 
                ||(layer_info.layer_height <layer_info.layer_width 
                && height<width) )
            {
                if(layer_info.layer_height < height
                    ||layer_info.layer_width < width)
                {
                    //SCI_TRACE_LOW:"[VP]HandleUpdateFrameReq,ERROR BUFFER1!!"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_4681_112_2_18_3_5_12_164,(uint8*)"");
                    return;
                }
                
                GUI_FillRect(s_mainlcd_layer, UILAYER_GetLayerRect(s_mainlcd_layer), 0x0000);
                copy_param.rect = UILAYER_GetLayerRect(s_mainlcd_layer);
                copy_param.rect.left += (layer_info.layer_width-width)/2;
                copy_param.rect.top += (layer_info.layer_height-height)/2;                    
                copy_param.rect.right = (copy_param.rect.left + width-1);
                copy_param.rect.bottom = (copy_param.rect.top + height-1);
                
                copy_buf.buf_ptr = (uint8*)s_current_fsm_ptr->image_data.buf.oneplane.p_data;
                copy_buf.data_type = DATA_TYPE_RGB565;
                copy_buf.width = width;
                copy_buf.height = height;
                UILAYER_CopyBufferToLayer(s_mainlcd_layer, &copy_param, &copy_buf);
            } 
            else
            {
                //SCI_TRACE_MID:"[VP]:GetFrameDataCallback l_height=%d,l_width=%d!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_4700_112_2_18_3_5_12_165,(uint8*)"dd",layer_info.layer_height,layer_info.layer_width);
                layer_buf_ptr = UILAYER_GetLayerBufferPtr(s_mainlcd_layer);
                if(PNULL == layer_buf_ptr
                    ||layer_info.layer_height < width
                    ||layer_info.layer_width < height)
                {
                    //SCI_TRACE_LOW:"[VP]HandleUpdateFrameReq,ERROR BUFFER2!!"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_4706_112_2_18_3_5_12_166,(uint8*)"");
                    return;
                }

               
                dx= (layer_info.layer_width - height) /2;
                dy=(layer_info.layer_height - width)/2;
                GUI_FillRect(s_mainlcd_layer, UILAYER_GetLayerRect(s_mainlcd_layer), 0x0000);
                
                dest_buf_ptr = (uint16*)layer_buf_ptr;
                src_buf_ptr = (uint16*)s_current_fsm_ptr->image_data.buf.oneplane.p_data;
                for(i=0;i<width;i++)
                {
                    for(j=0;j<height;j++)
                    {
                        *(dest_buf_ptr + (i+dy)*layer_info.layer_width + layer_info.layer_width-1-(j+dx)) = *(src_buf_ptr + j*width + i);
                    }
                }
            }
            MMITHEME_StoreUpdateRect(MMITHEME_GetDefaultLcdDev(),MMITHEME_GetFullScreenRect());
            MMITHEME_UpdateRect();
//#else
#if 0
            if(layer_info.layer_height >=height
                &&layer_info.layer_width >= width)
            {
                UILAYER_COPYBUF_PARAM_T     copy_param = {0};
                UILAYER_COPYBUF_T           copy_buf = {0};

                copy_param.rect = UILAYER_GetLayerRect(&s_video_layer);
                copy_param.rect.left += (layer_info.layer_width-width)/2;
                copy_param.rect.top += (layer_info.layer_height-height)/2;                    
                copy_param.rect.right = (copy_param.rect.left + width-1);
                copy_param.rect.bottom = (copy_param.rect.top + height-1);
    
                copy_buf.buf_ptr = (uint8*)s_current_fsm_ptr->image_data.buf.oneplane.p_data;
                copy_buf.data_type = DATA_TYPE_RGB565;
                copy_buf.width = width;
                copy_buf.height = height;
                UILAYER_CopyBufferToLayer(&s_video_layer, &copy_param, &copy_buf);
                MMITHEME_StoreUpdateRect(MMITHEME_GetDefaultLcdDev(),MMITHEME_GetFullScreenRect());
                MMITHEME_UpdateRect();
            }
            else
            {
                //SCI_TRACE_LOW:"[VP]:HandleUpdateFrameReq ERROR!!!!! height=%d,width=%d,layer_height=%d,layer_width=%d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_4746_112_2_18_3_5_13_167,(uint8*)"dddd",height,width,layer_info.layer_height,layer_info.layer_width);
            }   
#endif

#endif  //UI_MULTILAYER_SUPPORT

        }
        else
        {
            MMIAPIVP_FSMGetTimeOffset(fsm_ptr);
            SendProcessIndToUI(fsm_ptr);
            //SCI_TRACE_LOW:"[VP]:HandleUpdateFrameReq DPLAYER_UpdateNextFrame"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_4752_112_2_18_3_5_13_168,(uint8*)"");
            DPLAYER_UpdateNextFrame(fsm_ptr->dplayer_handle);
            MMITHEME_UpdateRect();
        }  
    } 
}
/*****************************************************************************/
//  Description : 处理pause请求消息
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN HandlePauseReq(VP_FSM_T * fsm_ptr)
{
    BOOLEAN    result = FALSE;
    if (PNULL == fsm_ptr || !IsValidFsm(fsm_ptr))
    {
        //VP_STRACE_LOW:"[VP] HandlePauseReq param is null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_4767_112_2_18_3_5_13_169,(uint8*)"");
        return FALSE;
    }

    fsm_ptr->process_time = DPLAYER_GetTime(fsm_ptr->dplayer_handle);
// #ifdef  CAMERA_SUPPORT      
    if(!MMISRVAUD_PauseSlightly(fsm_ptr->srv_handle))
    {
        return FALSE;
    }
// #endif
    if (VP_PLAY_FROM_NET != fsm_ptr->play_type)
    {                    
        result =  TRUE;
        return result;
    }
    else if (STRM_Pause(fsm_ptr->strm_handle))
    {
        result =  TRUE;      
    }
    else
    {
        //VP_STRACE_LOW:"[VP] HandlePauseReq can not pause, so stop"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_4789_112_2_18_3_5_13_170,(uint8*)"");
        //steaming暂停失败，stop     
        SendFatalErrorIndTOUI(fsm_ptr,VP_ERROR_OTHERS);
        MMIAPIVP_FSMStop((VP_HANDLE)fsm_ptr);       
    } 
    

    return result;
}

/*****************************************************************************/
//  Description : 处理resume请求消息
//  Global resource dependence : 
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN HandleResumeReq(VP_FSM_T* fsm_ptr)
{
    BOOLEAN result = TRUE;
#ifdef MMI_STREAMING_PLAYER_SUPPORT
    STRM_RESUME_PARAM_T strm_resume_param = {0};
#endif

    if (PNULL == fsm_ptr || !IsValidFsm(fsm_ptr))
    {
        //VP_STRACE_LOW:"[VP] HandleResumeReq param is null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_4812_112_2_18_3_5_13_171,(uint8*)"");
        return FALSE;
    }
// #ifdef  CAMERA_SUPPORT   
    MMIAPISET_StopAppRing(); 
    MMISRVAUD_SetAudioPauseStateByHandle(fsm_ptr->srv_handle);
    MMISRVAUD_ResumeSlightly(fsm_ptr->srv_handle);   
// #endif
#ifdef MMI_STREAMING_PLAYER_SUPPORT
    if (VP_PLAY_FROM_NET == fsm_ptr->play_type)
    {
        //网络部分恢复streaming播放，进入resuming状态
        strm_resume_param.start_time = VP_MAX_INVALID_VALUE;
        if(!STRM_Resume(fsm_ptr->strm_handle,&strm_resume_param))            
        {
            SendFatalErrorIndTOUI(fsm_ptr,VP_ERROR_OTHERS);
            MMIAPIVP_FSMStop((VP_HANDLE)fsm_ptr);
            result = FALSE;
        }
    }     
#endif
    
    return result;
}
/*****************************************************************************/
//  Description : 处理seek请求消息
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN HandleSeekReq(VP_FSM_T * fsm_ptr, VP_SEEK_PARAM_T *seek_param)
{    
    STRM_SEEK_PARAM_T   strm_seek_param = {0};
    uint32              seek_time = 0;//@andy.he  andyhe_testwaring  20110620
#ifdef MMI_VIDEOPLAYER_HISTORYLIST_SUPPORT
    uint32 cur_player_seek_time = 0;
    BOOLEAN is_play_last_file = FALSE;
#endif
    if (PNULL == fsm_ptr || !IsValidFsm(fsm_ptr) || PNULL == seek_param)
    {
        //VP_STRACE_LOW:"[VP] HandleSeekReq param is null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_4844_112_2_18_3_5_13_172,(uint8*)"");
        return FALSE;
    }
#ifdef MMI_VIDEOPLAYER_HISTORYLIST_SUPPORT  
    if (!fsm_ptr->is_seek_able)  
    {
        cur_player_seek_time = MMIVP_GetSeekTime();
        /*对于不能seek的文件,若是按历史记录会从上次位置
        开始播放,改为从头重新播放,避免bug181417 */
        if(0 != cur_player_seek_time
            && seek_param->seek_time == cur_player_seek_time  
            )
        {
            is_play_last_file = TRUE;
            seek_param->seek_time = 0; 
            seek_param->seek_frame = 0;
        }
    }
#endif
#ifdef MMI_VIDEOPLAYER_HISTORYLIST_SUPPORT
    if(!is_play_last_file)
#endif
    {
        if (!fsm_ptr->is_seek_able)
        {
            return TRUE;
        }
    }

    seek_time = seek_param->seek_time;//@andy.he  andyhe_testwaring  20110620
     
    //VP_STRACE_LOW:"[VP] FsmPlaying seek_time = %x, seek_param->seek_frame = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_4849_112_2_18_3_5_13_173,(uint8*)"dd",seek_time,seek_param->seek_frame);
    if (VP_MAX_INVALID_VALUE == seek_time )
    {
        //seek_time无效，通过frame计算seek_time
        if (VP_MAX_INVALID_VALUE == seek_param->seek_frame)
        {
            return FALSE;
        }
        seek_time = FrameToTime(fsm_ptr->dplayer_handle,seek_param->seek_frame);
    }            
    //设置dplayer时间
    DPLAYER_SetTime(fsm_ptr->dplayer_handle,seek_time);
#if defined (PLATFORM_ANTISW3)
    DPLAYER_SetSeekEndNotify(fsm_ptr->dplayer_handle, CallbackSeekEndNotify);
    #endif
    strm_seek_param.seek_to_time = seek_time;
    
    if (VP_PLAY_FROM_NET != fsm_ptr->play_type)
    {
        fsm_ptr->process_time = seek_time;
        return TRUE;       
    }
#ifdef MMI_STREAMING_PLAYER_SUPPORT
    else if (STRM_Seek(fsm_ptr->strm_handle,&strm_seek_param))
    {
        return TRUE;       
    }
#endif
    else
    {        
        //VP_STRACE_LOW:"[VP] HandleSeekReq streaming see error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_4876_112_2_18_3_5_13_174,(uint8*)"");
        return FALSE;
    }
}
/*****************************************************************************/
//  Description : 处理停止请求消息
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN HandleStopReq(VP_FSM_T* fsm_ptr, BOOLEAN is_begin_play)
{
  //  uint32          last_play_point = 0;    
    
    if (PNULL == fsm_ptr || !IsValidFsm(fsm_ptr))
    {
        //VP_STRACE_LOW:"[VP] HandleStopReq param is null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_4890_112_2_18_3_5_13_175,(uint8*)"");
        return FALSE;
    }
    //VP_STRACE_LOW:"[VP] HandleStopReq is_begin_play = %d, fsm_ptr->is_save_history = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_4893_112_2_18_3_5_13_176,(uint8*)"dd",is_begin_play,fsm_ptr->is_save_history);
    
    if(is_begin_play)
    {
        MMISRVAUD_Stop(fsm_ptr->srv_handle);
       /* while(!s_is_callback_called)
        {
            SCI_Sleep(200);
        }*/
#ifdef  MMI_KING_MOVIE_SUPPORT 
        VP_dplayerExtCtlDeltSecurty(fsm_ptr ->full_path_ptr,fsm_ptr->dplayer_handle, fsm_ptr->sec_handle);
#endif
        DPLAYER_Close(fsm_ptr->dplayer_handle);
#ifdef  MMI_KING_MOVIE_SUPPORT 
        VP_dplayerCloseSecurty(fsm_ptr->full_path_ptr,fsm_ptr->sec_handle,fsm_ptr->b_sprd_kmv, PNULL);
        fsm_ptr->sec_handle = PNULL;
#endif
        fsm_ptr->dplayer_handle = 0;
        DPLAYER_Release();
#ifdef MMI_STREAMING_PLAYER_SUPPORT        
        if (VP_PLAY_FROM_NET == fsm_ptr->play_type)
        {
            STRM_Stop(fsm_ptr->strm_handle);                
        }
#endif
    }
#ifdef MMI_STREAMING_PLAYER_SUPPORT
    else if (VP_PLAY_FROM_NET == fsm_ptr->play_type
        && 0 != fsm_ptr->strm_handle)
    {
        STRM_Close(fsm_ptr->strm_handle);
    }
#endif
    return TRUE;
}
/*****************************************************************************/
//  Description : 停止中
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN FsmStopping(VP_FSM_T *fsm_ptr, VP_FSM_MSG_T *msg_ptr)
{
    BOOLEAN             result          = TRUE;

    //参数检查
    if(PNULL == fsm_ptr || !IsValidFsm(fsm_ptr) || PNULL == fsm_ptr->vpcallback || PNULL == msg_ptr)
    {
        //VP_STRACE_LOW:"[VP] FsmStopping param is error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_4939_112_2_18_3_5_13_177,(uint8*)"");
        return FALSE;
    }
    
    //VP_STRACE_LOW:"[VP] FsmStopping msg_ptr->msg_id = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_4943_112_2_18_3_5_13_178,(uint8*)"d",msg_ptr->msg_id);
    switch(msg_ptr->msg_id)
    {
    case VP_ENTRY_REQ:    
        if (fsm_ptr->is_to_exit)
        {
            FsmDispatch((VP_HANDLE)fsm_ptr,VP_STRM_STOP_CNF,PNULL,PNULL);   //立即发stop cnf
        }        
        break;
#ifdef MMI_STREAMING_PLAYER_SUPPORT
    case VP_STRM_STOP_CNF:
        FsmStateTrans(fsm_ptr,VP_STATE_STOPED);        
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
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN FsmStopped(VP_FSM_T *fsm_ptr, VP_FSM_MSG_T *msg_ptr)
{
    BOOLEAN             result          = TRUE;
    //参数检查
    if(PNULL == fsm_ptr || !IsValidFsm(fsm_ptr) || PNULL == fsm_ptr->vpcallback || PNULL == msg_ptr)
    {
        //VP_STRACE_LOW:"[VP] FsmStopped param is error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_4975_112_2_18_3_5_13_179,(uint8*)"");
        return FALSE;
    }

    switch(msg_ptr->msg_id)
    {
    case VP_ENTRY_REQ:  
#ifdef MMI_STREAMING_PLAYER_SUPPORT		
        if (STRM_Close(fsm_ptr->strm_handle))
        {       
            FsmStateTrans(fsm_ptr,VP_STATE_CLOSING);
        }
        else
#endif
        {
            FsmStateTrans(fsm_ptr,VP_STATE_READY);
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
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN FsmClosing(VP_FSM_T *fsm_ptr, VP_FSM_MSG_T *msg_ptr)
{
    BOOLEAN                result = TRUE;
    

    //参数检查
    if(PNULL == fsm_ptr || !IsValidFsm(fsm_ptr) || PNULL == fsm_ptr->vpcallback || PNULL == msg_ptr)
    {
        //VP_STRACE_LOW:"[VP] FsmClosing param is error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_5015_112_2_18_3_5_13_180,(uint8*)"");
        return FALSE;
    }

    //VP_STRACE_LOW:"[VP] FsmClosing msg_ptr->msg_id = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_5019_112_2_18_3_5_13_181,(uint8*)"d",msg_ptr->msg_id);
    switch(msg_ptr->msg_id)
    {
    case VP_ENTRY_REQ:    
        if (fsm_ptr->is_to_exit)
        {
            FsmDispatch((VP_HANDLE)fsm_ptr,VP_STRM_CLOSE_CNF,PNULL,PNULL);
        }        
        break;
#ifdef MMI_STREAMING_PLAYER_SUPPORT
    case VP_STRM_CLOSE_CNF:
        //给ui发送VP_STOP_IND，通知stop   
        fsm_ptr->strm_handle = 0;
        FsmStateTrans(fsm_ptr,VP_STATE_READY);
        SendStopIndToUI(fsm_ptr);       
        break;
#endif
 //add by NEWMS00109918 begin.       
    case VP_RELEASE_REQ:
        //销毁状态机
        DestroyFSM((VP_HANDLE)fsm_ptr);       
        break;
 //add by NEWMS00109918 end.    
        
    default:
        result = FALSE;
        break;
    }    
    return result;
}

/*****************************************************************************/
//  Description : 用于处理其他task给jvm发送的消息
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIVP_DispatchSignalToFsmMsg(PWND app_ptr, uint16 msg_id, DPARAM param)
{     
    MMI_RESULT_E            result              = MMI_RESULT_TRUE;
    void                   *signal_ptr          =  (void*)((uint32)param);
    VP_FSM_MSG_E            req_msg_id              = 0;
    void                    *msg_body_ptr       = PNULL;
    uint32                  msg_body_size       = 0;
    VP_PLAY_END_RESULT_E   play_end_result      = 0;    
#ifdef  VIDEOTHUMBNAIL_SUPPORT  
    if(MMIAPIVP_IsThumbnailOpen())
    {
        switch(msg_id)
        {
            case APP_VP_ONE_FRAME_REARCH:
                    //VP_STRACE_LOW:"[VP] MMIVP_DispatchSignalToFsmMsg   APP_VP_ONE_FRAME_REARCH"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_5068_112_2_18_3_5_13_182,(uint8*)"");
                    HandleGetThumbnaiData();
                break;
             case APP_VP_ONE_FRAME_END:
                    //VP_STRACE_LOW:"[VP] MMIVP_DispatchSignalToFsmMsg   APP_VP_ONE_FRAME_END"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_5072_112_2_18_3_5_13_183,(uint8*)"");
                    HandleOpenNextVideo();
                    break;
            default:
                 result = MMI_RESULT_FALSE;
                break;
        }
        return result;
    }
#endif   

    if (PNULL == s_current_fsm_ptr)
    {
        return result;
    }

    switch(msg_id) 
    {
#ifdef MMI_STREAMING_PLAYER_SUPPORT    
    case STRM_INIT_CNF:  
        //VP_STRACE_LOW:"[VP] MMIVP_DispatchSignalToFsmMsg STRM_INIT_CNF"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_5092_112_2_18_3_5_13_184,(uint8*)"");
        req_msg_id = VP_STRM_INIT_CNF;
        msg_body_size = sizeof(STRM_INIT_CNF_T);
        msg_body_ptr = signal_ptr;  
        
        break;
        
    case STRM_DESCRIBE_AUTH_IND:
    case STRM_SETUP_AUTH_IND:
    case STRM_PLAY_AUTH_IND:        
        req_msg_id = VP_AUTHIND_REQ;
        msg_body_size = 0;
        msg_body_ptr = PNULL;              
        break;
        
    case STRM_PLAY_CNF:
        {
            STRM_PLAY_CNF_T  *strm_play_cnf_ptr = (STRM_PLAY_CNF_T*)signal_ptr;
            //VP_STRACE_LOW:"[VP] MMIVP_DispatchSignalToFsmMsg STRM_PLAY_CNF"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_5110_112_2_18_3_5_13_185,(uint8*)"");
            if (PNULL != strm_play_cnf_ptr)
            {
                req_msg_id = VP_STRM_PLAY_CNF;
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
        //VP_STRACE_LOW:"[VP] MMIVP_DispatchSignalToFsmMsg STRM_BUFFERING_IND"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_5126_112_2_18_3_5_13_186,(uint8*)"");
        req_msg_id = VP_STRM_BUFFERING_IND;
        msg_body_size = 0;
        msg_body_ptr = PNULL;     
        s_current_fsm_ptr->buf_percent = 0;
        break;
        
    case STRM_BUFFER_END_IND:
        //VP_STRACE_LOW:"[VP] MMIVP_DispatchSignalToFsmMsg STRM_BUFFER_END_IND"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_5134_112_2_18_3_5_13_187,(uint8*)"");
        req_msg_id = VP_STRM_BUFFER_END_IND;
        msg_body_size = 0;
        msg_body_ptr = PNULL;  
        s_current_fsm_ptr->buf_percent = 100;
        break;
        
    case STRM_DATA_TIMEOUT_IND:
        //VP_STRACE_LOW:"[VP] MMIVP_DispatchSignalToFsmMsg STRM_DATA_TIMEOUT_IND"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_5142_112_2_18_3_5_13_188,(uint8*)"");
        req_msg_id = VP_STRM_DATA_TIMEOUT_IND;
        msg_body_size = 0;
        msg_body_ptr = PNULL;            
        break;       
   
    case STRM_BUFFERING_PROGRESS_IND:
        {
            STRM_BUFFERING_PROGRESS_IND_T *strm_buf_process_ptr = (STRM_BUFFERING_PROGRESS_IND_T*)signal_ptr;
            //VP_STRACE_LOW:"[VP] MMIVP_DispatchSignalToFsmMsg strm_buf_process_ptr = %x"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_5151_112_2_18_3_5_13_189,(uint8*)"d",strm_buf_process_ptr);
            if (PNULL != strm_buf_process_ptr
            && 0 != strm_buf_process_ptr->strm_buf_total)
            {
                //VP_STRACE_LOW:"[VP] MMIVP_DispatchSignalToFsmMsg process = %d, total = %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_5157_112_2_18_3_5_13_190,(uint8*)"dd",strm_buf_process_ptr->strm_buf_progress,strm_buf_process_ptr->strm_buf_total);
                s_current_fsm_ptr->buf_percent = strm_buf_process_ptr->strm_buf_progress * 100 /strm_buf_process_ptr->strm_buf_total;
            }
            req_msg_id = VP_BUFFER_PERCENT;
            msg_body_size = 0;
            msg_body_ptr = PNULL;      
            //VP_STRACE_LOW:"[VP] MMIVP_DispatchSignalToFsmMsg percent = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_5164_112_2_18_3_5_13_191,(uint8*)"d",s_current_fsm_ptr->buf_percent);
        }
        break;

    case STRM_GOODBYE_IND:
        //VP_STRACE_LOW:"[VP] MMIVP_DispatchSignalToFsmMsg STRM_GOODBYE_IND"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_5169_112_2_18_3_5_13_192,(uint8*)"");
        req_msg_id = VP_STRM_GOODBYE_IND;
        msg_body_size = 0;
        msg_body_ptr = PNULL;        
        break;
        
    case STRM_SEEK_CNF:
        {
            STRM_SEEK_CNF_T  *strm_seek_cnf_ptr = (STRM_SEEK_CNF_T*)signal_ptr;
            //VP_STRACE_LOW:"[VP] MMIVP_DispatchSignalToFsmMsg STRM_SEEK_CNF"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_5178_112_2_18_3_5_13_193,(uint8*)"");
            if (PNULL != strm_seek_cnf_ptr)
            {
                req_msg_id = VP_STRM_SEEK_CNF;
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
            //VP_STRACE_LOW:"[VP] MMIVP_DispatchSignalToFsmMsg STRM_PAUSE_CNF"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_5196_112_2_18_3_5_13_194,(uint8*)"");
            if (PNULL != strm_pause_cnf_ptr)
            {
                req_msg_id = VP_STRM_PAUSE_CNF;
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
            //VP_STRACE_LOW:"[VP] MMIVP_DispatchSignalToFsmMsg STRM_RESUME_CNF"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_5214_112_2_18_3_5_14_195,(uint8*)"");
            if (PNULL != strm_resume_cnf_ptr)
            {
                req_msg_id = VP_STRM_RESUME_CNF;
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
            //VP_STRACE_LOW:"[VP] MMIVP_DispatchSignalToFsmMsg STRM_STOP_CNF"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_5233_112_2_18_3_5_14_196,(uint8*)"");
            if (PNULL != strm_stop_cnf_ptr)
            {
                req_msg_id = VP_STRM_STOP_CNF;
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
            //VP_STRACE_LOW:"[VP] MMIVP_DispatchSignalToFsmMsg STRM_CLOSE_CNF"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_5250_112_2_18_3_5_14_197,(uint8*)"");
            if (PNULL != strm_close_cnf_ptr)
            {
                req_msg_id = VP_STRM_CLOSE_CNF;
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
    case APP_VP_FRAME_END_IND:
     //   VP_STRACE_LOW("[VP] MMIVP_DispatchSignalToFsmMsg APP_VP_FRAME_END_IND");
        req_msg_id = VP_FRAME_END_IND;
        msg_body_size = 0;
        msg_body_ptr = PNULL;        
        break;

    case APP_VP_FRAME_END_NEED_DATA:
     //   VP_STRACE_LOW("[VP] MMIVP_DispatchSignalToFsmMsg APP_VP_FRAME_END_NEED_DATA");
        req_msg_id = VP_FRAME_END_NEED_DATA;
        msg_body_size = 0;
        msg_body_ptr = PNULL;        
        break;

    case APP_VP_END_IND:
        {
            MmiAppCopyMsgS *sendSignal = (MmiAppCopyMsgS *)signal_ptr;
            //VP_STRACE_LOW:"[VP] MMIVP_DispatchSignalToFsmMsg APP_VP_END_IND"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_5281_112_2_18_3_5_14_198,(uint8*)"");
            if (PNULL != sendSignal)
            {
                play_end_result = (VP_PLAY_END_RESULT_E)((MmiAppCopyMsgS*)sendSignal->data);/*lint !e549*/
            }
            req_msg_id = VP_CALLBACK_END_IND;
            msg_body_size = sizeof(VP_PLAY_END_RESULT_E);
            msg_body_ptr = &play_end_result;              
        }
        break;

    case APP_VP_PLAY_NEXT_FILE_REQ:
        {
            MmiAppCopyMsgS *sendSignal = (MmiAppCopyMsgS *)signal_ptr;
            //VP_STRACE_LOW:"[VP] MMIVP_DispatchSignalToFsmMsg APP_VP_PLAY_NEXT_FILE_REQ"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_5295_112_2_18_3_5_14_199,(uint8*)"");
            if (PNULL != sendSignal)
            {
                play_end_result = (VP_PLAY_END_RESULT_E)((MmiAppCopyMsgS*)sendSignal->data);/*lint !e549*/
            }
            req_msg_id = VP_PLAY_NEXT_FILE;
            msg_body_size = sizeof(VP_PLAY_END_RESULT_E);
            msg_body_ptr = &play_end_result;              
        }
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    if (MMI_RESULT_TRUE == result)
    {
        //向videoplayer control发送消息
        result = FsmDispatch((VP_HANDLE)s_current_fsm_ptr,req_msg_id,msg_body_ptr,msg_body_size);           
    }    
  //  VP_STRACE_LOW("[VP] MMIVP_DispatchSignalToFsmMsg result = %d",result);
    return result;
}


/*****************************************************************************/
//  Description : 创建videoplayer状态机
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 不要在callback中调用vp fsm函数
/*****************************************************************************/
PUBLIC  VP_HANDLE  MMIAPIVP_FSMInit(VP_CALLBACK vpcallback)
{
    VP_HANDLE               vp_handle       =   0; 
    
    //参数检查
    if(PNULL == vpcallback)
    {
        //VP_STRACE_LOW:"[VP] MMIAPIVP_FSMInit ui callback is null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_5331_112_2_18_3_5_14_200,(uint8*)"");
        return 0;
    }
    
    //创建videoplayer 状态机，并将状态机指针做为handle
    vp_handle = CreateFSM(vpcallback);
    if (0 == vp_handle)
    {
        //VP_STRACE_LOW:"[VP] MMIAPIVP_FSMInit create fsm fail"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_5339_112_2_18_3_5_14_201,(uint8*)"");
        return 0;
    }    
    //VP_STRACE_LOW:"[VP ] MMIAPIVP_FSMInit create fsm handle = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_5342_112_2_18_3_5_14_202,(uint8*)"d",vp_handle);
    //向videoplayer control发送消息
    if(!FsmDispatch(vp_handle,VP_INIT_REQ,PNULL,0))
    {
        //VP_STRACE_LOW:"[VP] MMIAPIVP_FSMInit dispath msg fail"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_5346_112_2_18_3_5_14_203,(uint8*)"");
        DestroyFSM(vp_handle);
        vp_handle = 0;
    }    
    s_current_fsm_ptr = (VP_FSM_T *)vp_handle;   
    //返回videoplayer handle
    return vp_handle;
    
}

/*****************************************************************************/
//  Description : 去活PDP；销毁videoplayer状态机
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIVP_FSMRelease(VP_HANDLE   vp_handle)
{
    
    //VP_STRACE_LOW:"[VP] MMIAPIVP_FSMRelease vp_handle = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_5363_112_2_18_3_5_14_204,(uint8*)"d",vp_handle);
    //检查vp_handler的有效性；    
    if (0 == vp_handle || !IsValidFsm((VP_FSM_T *)vp_handle))
    {
        //VP_STRACE_LOW:"[VP] MMIAPIVP_FSMRelease some wrong with vp_handle = %x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_5367_112_2_18_3_5_14_205,(uint8*)"d",vp_handle);
        return; 
    }
#ifdef MMI_GPRS_SUPPORT
    //去活pdp;
    MMIAPIPDP_Deactive(MMI_MODULE_VP);   
#endif   
    
    //向videoplayer control发送消息
    if(!FsmDispatch(vp_handle,VP_RELEASE_REQ, PNULL,0))
    {
        MMIAPIVP_FSMExit(vp_handle);
    }
    
}

/*****************************************************************************/
//  Description : 发起视频播放（本地或网络）
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIVP_FSMPlay(VP_HANDLE   vp_handle, VP_PLAY_PARAM_T  *play_param_ptr)
{   
    BOOLEAN                 result          =   FALSE;
    //检查vp_handler的有效性；
    
    if (0 == vp_handle || !IsValidFsm((VP_FSM_T *)vp_handle) || PNULL == play_param_ptr)
    {
        //VP_STRACE_LOW:"[VP] MMIAPIVP_FSMPlay some wrong with vp_handle = %x,play_param_ptr = %x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_5394_112_2_18_3_5_14_206,(uint8*)"dd",vp_handle,play_param_ptr);
        return FALSE; 
    }
    #ifndef MMI_VIDEOPLAYER_MINI_FUNCTION
    //本地播放创建随机播放列表
    if (VP_PLAY_FROM_LOCALLIST == play_param_ptr->play_type)
    { 
        if (VP_REPEAT_PLAY_MODE_ALL == play_param_ptr->mode 
            || (VP_REPEAT_PLAY_MODE_CLOSE == play_param_ptr->mode && play_param_ptr->is_random_play))
        {
            VP_LocalList_CreateCurPlayList(play_param_ptr->is_random_play);
        }
    }
    #endif
    //VP_STRACE_LOW:"[VP] MMIAPIVP_FSMPlay begin"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_5406_112_2_18_3_5_14_207,(uint8*)"");
    //向videoplayer control发送消息
    MMIAPISET_StopAppRing();//按键音和视频声音相互影响，导致声音异常,bug 115475 132913
    
    result = FsmDispatch(vp_handle,VP_PLAY_REQ, play_param_ptr,sizeof(VP_PLAY_PARAM_T));
    //VP_STRACE_LOW:"[VP] MMIAPIVP_FSMPlay result = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_5409_112_2_18_3_5_14_208,(uint8*)"d",result);
    if (!result)
    {
        SendPlayCnfToUI((VP_FSM_T *)vp_handle,VP_ERROR_OTHERS);
    }
    return result;
    
}

/*****************************************************************************/
//  Description : 终止视频播放（本地或网络）。
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIVP_FSMStop(VP_HANDLE   vp_handle)
{  
    BOOLEAN                  result         =   FALSE;
    VP_FSM_T           *fsm_ptr          = (VP_FSM_T *)vp_handle;
    
    //检查vp_handler的有效性；
    if (0 == vp_handle || !IsValidFsm(fsm_ptr))
    {
        //VP_STRACE_LOW:"[VP] MMIAPIVP_FSMStop some wrong with vp_handle = %x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_5430_112_2_18_3_5_14_209,(uint8*)"d",vp_handle);
        return FALSE; 
    } 
#ifndef MMI_VIDEOPLAYER_MINI_FUNCTION
    //销毁正在播放列表
    VP_LocalList_DestroyPlayList();    
#endif
    //VP_STRACE_LOW:"[VP] MMIAPIVP_FSMStop begin"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_5436_112_2_18_3_5_14_210,(uint8*)"");
    result = FsmDispatch(vp_handle,VP_STOP_REQ, PNULL,0);    
    //VP_STRACE_LOW:"[VP] MMIAPIVP_FSMStop result = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_5438_112_2_18_3_5_14_211,(uint8*)"d",result);
    
    return result;
}

/*****************************************************************************/
//  Description : 暂停视频播放（本地或网络）。
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIVP_FSMPause(VP_HANDLE   vp_handle)
{   
    BOOLEAN               result          =   FALSE;
    VP_FSM_T             *fsm_ptr       = (VP_FSM_T *)vp_handle;
    
    //检查vp_handler的有效性；
    if (0 == vp_handle || !IsValidFsm(fsm_ptr))
    {
        //VP_STRACE_LOW:"[VP] MMIAPIVP_FSMPause some wrong with vp_handle = %x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_5455_112_2_18_3_5_14_212,(uint8*)"d",vp_handle);
        return FALSE; 
    }  
    //VP_STRACE_LOW:"[VP] MMIAPIVP_FSMPause begin"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_5458_112_2_18_3_5_14_213,(uint8*)"");
    //向videoplayer control发送消息
    result = FsmDispatch(vp_handle,VP_PAUSE_REQ, PNULL,0);     
    
    //VP_STRACE_LOW:"[VP] MMIAPIVP_FSMPause result = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_5462_112_2_18_3_5_14_214,(uint8*)"d",result);
    if (!result && !fsm_ptr->is_to_pause)
    {
        SendPauseCnfToUI(fsm_ptr,VP_ERROR_PAUSE_ERROR);
    }
    return result;
}

/*****************************************************************************/
//  Description : 恢复视频播放（本地或网络）
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIVP_FSMResume(VP_HANDLE  vp_handle)
{   
    BOOLEAN                 result          =   FALSE;
    VP_FSM_T             *fsm_ptr       = (VP_FSM_T *)vp_handle;
    
    
    //检查vp_handler的有效性；
    if (0 == vp_handle || !IsValidFsm(fsm_ptr))
    {
        //VP_STRACE_LOW:"[VP] MMIAPIVP_FSMResume some wrong with vp_handle = %x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_5483_112_2_18_3_5_14_215,(uint8*)"d",vp_handle);
        return FALSE; 
    }
    //VP_STRACE_LOW:"[VP] MMIAPIVP_FSMResume begin"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_5486_112_2_18_3_5_14_216,(uint8*)"");
    //向videoplayer control发送消息
    result = FsmDispatch(vp_handle,VP_RESUME_REQ, PNULL,0);   
    //VP_STRACE_LOW:"[VP] MMIAPIVP_FSMResume result = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_5489_112_2_18_3_5_14_217,(uint8*)"d",result);
    if (!result && !fsm_ptr->is_to_resume)
    {
        SendPlayCnfToUI(fsm_ptr,VP_ERROR_RESUME_ERROR);
    }
    return result;
}

/*****************************************************************************/
//  Description : 进行定位视频播放（本地或网络）。
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIVP_FSMSeek(VP_HANDLE   vp_handle,
                       VP_SEEK_PARAM_T  *seek_param_ptr)
{
   
    BOOLEAN                 result          = FALSE;
    
    
    //检查vp_handler的有效性；
    if (0 == vp_handle || !IsValidFsm((VP_FSM_T *)vp_handle) || PNULL == seek_param_ptr)
    {
        //VP_STRACE_LOW:"[VP] MMIAPIVP_FSMSeek some wrong with vp_handle = %x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_5511_112_2_18_3_5_14_218,(uint8*)"d",vp_handle);
        return FALSE; 
    }    
    //VP_STRACE_LOW:"[VP] MMIAPIVP_FSMSeek begin"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_5514_112_2_18_3_5_14_219,(uint8*)"");
    //向videoplayer control发送消息
    result = FsmDispatch(vp_handle,VP_SEEK_REQ, seek_param_ptr,sizeof(VP_SEEK_PARAM_T));
    
    //VP_STRACE_LOW:"[VP] MMIAPIVP_FSMSeek result = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_5518_112_2_18_3_5_14_220,(uint8*)"d",result);
    if (!result)
    {
        SendSeekCnfToUI((VP_FSM_T *)vp_handle,VP_ERROR_SEEK_ERROR);
    }
    return result;
}

/*****************************************************************************/
//  Description : 停止进行加快速率视频播放本地或网络
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN VP_StopSetRate(VP_HANDLE   vp_handle,
                               BOOLEAN   is_backward)
{
   
    BOOLEAN                 result          = FALSE;
    
    
    //检查vp_handler的有效性；
    if (0 == vp_handle || !IsValidFsm((VP_FSM_T *)vp_handle))
    {
        //VP_STRACE_LOW:"[VP] VP_StopSetRate some wrong with vp_handle = %x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_5540_112_2_18_3_5_14_221,(uint8*)"d",vp_handle);
        return FALSE; 
    }    
    //VP_STRACE_LOW:"[VP] VP_StopSetRate begin"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_5543_112_2_18_3_5_14_222,(uint8*)"");
    //向videoplayer control发送消息
    result = FsmDispatch(vp_handle,VP_STOP_SET_RATE, &is_backward,sizeof(BOOLEAN));
    
    //VP_STRACE_LOW:"[VP] VP_StopSetRate result = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_5547_112_2_18_3_5_14_223,(uint8*)"d",result);
    return result;
}

/*****************************************************************************/
//  Description : 进行加快速率视频播放本地或网络
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN VP_StartSetRate(VP_HANDLE   vp_handle,
                               BOOLEAN   is_backward)
{
   
    BOOLEAN                 result          = FALSE;
    
    
    //检查vp_handler的有效性；
    if (0 == vp_handle || !IsValidFsm((VP_FSM_T *)vp_handle))
    {
        //VP_STRACE_LOW:"[VP] VP_StartSetRate some wrong with vp_handle = %x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_5565_112_2_18_3_5_14_224,(uint8*)"d",vp_handle);
        return FALSE; 
    }    
    //VP_STRACE_LOW:"[VP] VP_StartSetRate begin"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_5568_112_2_18_3_5_14_225,(uint8*)"");
    //向videoplayer control发送消息
    result = FsmDispatch(vp_handle,VP_START_SET_RATE, &is_backward,sizeof(BOOLEAN));
    
    //VP_STRACE_LOW:"[VP] VP_StartSetRate result = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_5572_112_2_18_3_5_14_226,(uint8*)"d",result);
    return result;
}

/*****************************************************************************/
//  Description : 发起鉴权认证
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
#ifdef MMI_STREAMING_PLAYER_SUPPORT 
PUBLIC BOOLEAN VP_AuthRsp(VP_HANDLE   vp_handle,
                          VP_AUTH_INFO_T  *auth_info_ptr)
{
   
    BOOLEAN              result         =   FALSE;
    VP_FSM_T             *fsm_ptr       = (VP_FSM_T *)vp_handle;
    
    //检查vp_handler的有效性；
    if (0 == vp_handle || !IsValidFsm(fsm_ptr) || PNULL == auth_info_ptr)
    {
        //VP_STRACE_LOW:"[VP] VP_AuthRsp some wrong with vp_handle = %x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_5591_112_2_18_3_5_14_227,(uint8*)"d",vp_handle);
        return FALSE; 
    }
    
    //向videoplayer control发送消息
    result = FsmDispatch(vp_handle,VP_AUTH_RSP, auth_info_ptr,sizeof(VP_AUTH_INFO_T));   
    //VP_STRACE_LOW:"[VP] VP_AuthRsp result = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_5597_112_2_18_3_5_14_228,(uint8*)"d",result);
    return result;
}
#endif

/*****************************************************************************/
//  Description : 切换播放设备
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIVP_FSMSwitchPlayDevice(VP_HANDLE  vp_handle,
                                   BOOLEAN    is_a2dp_support,
                                   BOOLEAN    is_bt_support)
{
    // BOOLEAN                 result          =   TRUE;
    VP_FSM_T                *fsm_ptr        = (VP_FSM_T *)vp_handle;
    DPLAYER_AUDIO_PARAM_T    audio_param    = {0};


    //检查vp_handler的有效性；
    if (PNULL == fsm_ptr || !IsValidFsm(fsm_ptr))
    {
        //SCI_TRACE_LOW:"[VP] MMIAPIVP_FSMSwitchPlayDevice the fsm is wrong fsm_ptr = %x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_5618_112_2_18_3_5_14_229,(uint8*)"d",fsm_ptr);
        return FALSE;
    }

    //SCI_TRACE_LOW:"[VP] MMIAPIVP_FSMSwitchPlayDevice is_bt_support = %d, is_a2dp_support = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_5622_112_2_18_3_5_14_230,(uint8*)"dd", is_bt_support,is_a2dp_support);
        
    if (is_bt_support)
    {
        audio_param.output_device = DPLAYER_AUD_DEV_BT;
    }
    else
    {
        audio_param.output_device = DPLAYER_AUD_DEV_SPEAKER;
    }
    audio_param.output_device_info.bt_info.b_a2dp_support = is_a2dp_support;

    fsm_ptr->is_a2dp_support = is_a2dp_support;
    fsm_ptr->is_bt_support = is_bt_support;

    if (0 != fsm_ptr->dplayer_handle)
    {
        DPLAYER_SetAudioParam(fsm_ptr->dplayer_handle, &audio_param);
    }

    return TRUE;
}
/*****************************************************************************/
//  Description : 切换显示模式（全屏，半屏，TVOUT)
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIVP_FSMSwitchDisplayMode(VP_HANDLE  vp_handle,
                                    VP_DISPLAY_PARAM_T  *display_param_ptr,
                                    BOOLEAN is_only_rect)
{
    BOOLEAN                 result          =   TRUE;
    VP_FSM_T                *fsm_ptr        = (VP_FSM_T *)vp_handle;
   // DPLAYER_MEDIA_INFO_T    p_info          = {0};
    VP_DISPLAY_PARAM_T      display_param   = {0};
    
    //检查vp_handler的有效性；
    if (PNULL == fsm_ptr || !IsValidFsm(fsm_ptr) || PNULL == display_param_ptr)
    {
        //VP_STRACE_LOW:"[VP] MMIAPIVP_FSMSwitchDisplayMode the fsm is wrong fsm_ptr = %x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_5660_112_2_18_3_5_14_231,(uint8*)"d",fsm_ptr);
        return FALSE;
    }
    
    
    //VP_STRACE_LOW:"[VP] MMIAPIVP_FSMSwitchDisplayMode fsm_ptr->state = %d, is_only_rect = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_5665_112_2_18_3_5_14_232,(uint8*)"dd",fsm_ptr->state,is_only_rect);
   
    
    
    //VP_STRACE_LOW:"[VP] MMIAPIVP_FSMSwitchDisplayMode lcd_id = %d, block_id = %d, x= %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_5669_112_2_18_3_5_15_233,(uint8*)"ddd",display_param_ptr->LcdId,display_param_ptr->LcdBlkId,display_param_ptr->disp_rect.x);
    if (is_only_rect)
    {
        fsm_ptr->display_param.disp_rect.y = display_param_ptr->disp_rect.y;
        fsm_ptr->display_param.disp_rect.x = display_param_ptr->disp_rect.x;
        fsm_ptr->display_param.disp_rect.dx = display_param_ptr->disp_rect.dx;
        fsm_ptr->display_param.disp_rect.dy = display_param_ptr->disp_rect.dy;
        fsm_ptr->display_param.target_rect.y = display_param_ptr->target_rect.y;
        fsm_ptr->display_param.target_rect.x = display_param_ptr->target_rect.x;
        fsm_ptr->display_param.target_rect.dx = display_param_ptr->target_rect.dx;
        fsm_ptr->display_param.target_rect.dy = display_param_ptr->target_rect.dy;
        fsm_ptr->display_param.RotateAngle = display_param_ptr->RotateAngle;        
    }
    else
    {
        SCI_MEMCPY(&(fsm_ptr->display_param),display_param_ptr,sizeof(VP_DISPLAY_PARAM_T));
    }

    if (PNULL != fsm_ptr->dplayer_handle)
    {
        SCI_MEMCPY(&display_param,&(fsm_ptr->display_param),sizeof(VP_DISPLAY_PARAM_T));
        MMIAPIVP_TranslateDisplayParamLogicToPhysical(&display_param, FALSE);

        if(DPLAYER_SUCCESS != DPLAYER_SetDisplayParam(fsm_ptr->dplayer_handle, &display_param))
        {
            result =  FALSE;
        } 

        DPLAYER_UpdateCurrentFrame(fsm_ptr->dplayer_handle); 
    }
    
    
    //VP_STRACE_LOW:"[VP] MMIAPIVP_FSMSwitchDisplayMode result = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_5701_112_2_18_3_5_15_234,(uint8*)"d",result);
    return result;
}
/*****************************************************************************/
//  Description : 切换声音模式 
//  Global resource dependence :  none
//  Author:  
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIVP_FSMSwitchAudioSupport(VP_HANDLE  vp_handle,
                                    BOOLEAN is_support)
{
    BOOLEAN                 result          =   TRUE;
    VP_FSM_T                *fsm_ptr        = (VP_FSM_T *)vp_handle;
    
    //检查vp_handler的有效性；
    if (PNULL == fsm_ptr || !IsValidFsm(fsm_ptr))
    {
        //VP_STRACE_LOW:"[VP] MMIAPIVP_FSMSwitchDisplayMode the fsm is wrong fsm_ptr = %x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_5660_112_2_18_3_5_14_231,(uint8*)"d",fsm_ptr);
        return FALSE;
    }
    
    


    if(fsm_ptr->display_param.audioSupport != is_support)
    {
        fsm_ptr->display_param.audioSupport = is_support;
    }
        
    if (PNULL != fsm_ptr->dplayer_handle)
    {

        if(DPLAYER_SUCCESS != DPLAYER_SetDisplayParam(fsm_ptr->dplayer_handle, &(fsm_ptr->display_param)))
        {
            result =  FALSE;
        } 

    }
    
    
    //VP_STRACE_LOW:"[VP] MMIAPIVP_FSMSwitchDisplayMode result = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_5701_112_2_18_3_5_15_234,(uint8*)"d",result);
    return result;
}


/*****************************************************************************/
//  Description : 获取当前播放状态
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC VP_STATE_E MMIAPIVP_FSMGetState(VP_HANDLE    vp_handle)
{
    VP_FSM_T       *fsm_ptr = (VP_FSM_T *)vp_handle;
    VP_STATE_E      state   = VP_STATE_MAX;
    
    if (PNULL != fsm_ptr && IsValidFsm(fsm_ptr))
    {
        state =  fsm_ptr->state;
    }
    
    //VP_STRACE_LOW:"[VP] MMIAPIVP_FSMGetState state = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_5719_112_2_18_3_5_15_235,(uint8*)"d",state);
    return state;
    
}

/*****************************************************************************/
//  Description : 执行退出流程，并销毁状态机。
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 满足一键退出的需求。
/*****************************************************************************/
PUBLIC void MMIAPIVP_FSMExit(VP_HANDLE  vp_handle)
{
    VP_FSM_T * fsm_ptr = (VP_FSM_T *)vp_handle;
    //检查vp_handler的有效性；
    if (PNULL == vp_handle || !IsValidFsm(fsm_ptr))
    {
        //VP_STRACE_LOW:"[VP] MMIAPIVP_FSMExit the fsm is wrong vp_handle = %x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_5734_112_2_18_3_5_15_236,(uint8*)"d",vp_handle);
        return;
    }    
    //设置强制退出标志
    fsm_ptr->is_to_exit = TRUE;
    //停止当前播放
    MMIAPIVP_FSMStop(vp_handle);  

    
}

/*****************************************************************************/
//  Description : 强制暂停
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 满足lose focus暂停的需求。
/*****************************************************************************/
PUBLIC void MMIAPIVP_FSMCompulsivePause(VP_HANDLE  vp_handle)
{
   VP_FSM_T * fsm_ptr = (VP_FSM_T *)vp_handle;
    //检查vp_handler的有效性；
    if (PNULL == vp_handle || !IsValidFsm(fsm_ptr))
    {
        //VP_STRACE_LOW:"[VP] MMIAPIVP_FSMCompulsivePause the fsm is wrong vp_handle = %x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_5755_112_2_18_3_5_15_237,(uint8*)"d",vp_handle);
        return;
    }

    if(VP_STATE_PAUSED == MMIAPIVP_FSMGetState((VP_HANDLE)s_current_fsm_ptr))
    {
        MMISRVAUD_SetAudioPlayStateByHandle(fsm_ptr->srv_handle);
    }
    //VP_STRACE_LOW:"[VP] MMIAPIVP_FSMCompulsivePause begin fsm_ptr->is_to_resume = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_5759_112_2_18_3_5_15_238,(uint8*)"d",fsm_ptr->is_to_resume);

	SCI_TRACE_LOW("[VP] MMIAPIVP_FSMCompulsivePause current state = %d", fsm_ptr->state);
    if(fsm_ptr->is_to_resume)
    {
       fsm_ptr->is_to_resume = FALSE;
    }
    else if(VP_MAX_INVALID_VALUE == fsm_ptr->total_time
        && VP_STATE_PLAY_PENDING <= fsm_ptr->state)
    {        
        VP_DISPLAY_PARAM_T dis_param = {0};
        SCI_MEMCPY(&dis_param,&(fsm_ptr->display_param),sizeof(VP_DISPLAY_PARAM_T));
        dis_param.videoSupport = 0;
        MMIAPIVP_FSMSwitchDisplayMode(vp_handle,&dis_param,FALSE);
        
    }
#ifdef MMI_STREAMING_PLAYER_SUPPORT 
    else if ((VP_STATE_PLAY_PENDING > fsm_ptr->state && VP_STATE_READY < fsm_ptr->state)
           &&VP_PLAY_FROM_NET == fsm_ptr->play_type)
    {
        STRM_Close(fsm_ptr->strm_handle);
        FsmStateTrans(fsm_ptr, VP_STATE_READY);
        s_is_special_pause = TRUE;
		SCI_TRACE_LOW("[VP] MMIAPIVP_FSMCompulsivePause special paused");
    }
#endif
    else
    {
        fsm_ptr->is_to_pause = TRUE;        
        MMIAPIVP_FSMPause(vp_handle);
    }   
    //VP_STRACE_LOW:"[VP] MMIAPIVP_FSMCompulsivePause end fsm_ptr->is_to_pause = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_5778_112_2_18_3_5_15_239,(uint8*)"d",fsm_ptr->is_to_pause);
}

/*****************************************************************************/
//  Description : 强制恢复
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 满足get focus恢复的需求。
/*****************************************************************************/
PUBLIC void MMIAPIVP_FSMCompulsiveResume(VP_HANDLE  vp_handle)
{
   VP_FSM_T * fsm_ptr = (VP_FSM_T *)vp_handle;
   //GUI_RECT_T lcd_rect = {0};
   //GUI_LCD_DEV_INFO const *lcd_dev_info_ptr = (const GUI_LCD_DEV_INFO*)MMITHEME_GetDefaultLcdDev();
   BOOLEAN is_need_fill = TRUE;
    BOOLEAN resume_result = FALSE;
   
    //检查vp_handler的有效性；
    if (PNULL == vp_handle || !IsValidFsm(fsm_ptr))
    {
        //VP_STRACE_LOW:"[VP] MMIAPIVP_FSMCompulsiveResume the fsm is wrong vp_handle = %x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_5796_112_2_18_3_5_15_240,(uint8*)"d",vp_handle);
        return;
    }
    //VP_STRACE_LOW:"[VP] MMIAPIVP_FSMCompulsiveResume begin fsm_ptr->is_to_pause = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_5799_112_2_18_3_5_15_241,(uint8*)"d",fsm_ptr->is_to_pause);

	SCI_TRACE_LOW("[VP] MMIAPIVP_FSMCompulsiveResume current state = %d", fsm_ptr->state);
    if(fsm_ptr->is_to_pause)
    {
        fsm_ptr->is_to_pause = FALSE;  
        if (VP_STATE_PLAY_PENDING == fsm_ptr->state
            || VP_STATE_SEEKING == fsm_ptr->state
            || VP_STATE_PAUSED_SEEKING == fsm_ptr->state)
        {
            resume_result = MMISRVAUD_Resume(fsm_ptr->srv_handle); 
            MMIAPIVP_FSMSwitchDisplayMode((VP_HANDLE)fsm_ptr,&(fsm_ptr->display_param),TRUE);
            if(!resume_result)//andy.he cr240364 20110620
            {
                SendFatalErrorIndTOUI(fsm_ptr,VP_ERROR_OTHERS);
                MMIAPIVP_FSMStop((VP_HANDLE)fsm_ptr);  
                return;
            }
        }
    }
    else if(VP_MAX_INVALID_VALUE == fsm_ptr->total_time
        && VP_STATE_PLAY_PENDING <= fsm_ptr->state)
    {
        VP_DISPLAY_PARAM_T dis_param = {0};
        SCI_MEMCPY(&dis_param,&(fsm_ptr->display_param),sizeof(VP_DISPLAY_PARAM_T));
        dis_param.videoSupport = 1;
        fsm_ptr->display_param.videoSupport = 1;
        if (VP_SUPPORTED_NEITHER == fsm_ptr->support_type
            || VP_SUPPORTED_AUDIO_ONLY == fsm_ptr->support_type)
        {
             is_need_fill = FALSE;
        }

//        if (VP_STATE_READY != fsm_ptr->state && VP_STATE_PAUSING != fsm_ptr->state && VP_STATE_PAUSED != fsm_ptr->state)
//        {
//            DPLAYER_Pause(fsm_ptr->dplayer_handle);
//        }         

        MMIAPIVP_TranslateDisplayParamLogicToPhysical(&dis_param, is_need_fill);

        if(DPLAYER_SUCCESS == DPLAYER_SetDisplayParam(fsm_ptr->dplayer_handle, &dis_param))
        {   
          //  DPLAYER_UpdateCurrentFrame(fsm_ptr->dplayer_handle);
        } 
//        if (VP_STATE_READY != fsm_ptr->state && VP_STATE_PAUSING != fsm_ptr->state && VP_STATE_PAUSED != fsm_ptr->state)
//        {
//            DPLAYER_Resume(fsm_ptr->dplayer_handle);            
//        }   
        HandleUpdateFrameReq(fsm_ptr);
    }
#ifdef MMI_STREAMING_PLAYER_SUPPORT 
    else if (s_is_special_pause
           &&VP_PLAY_FROM_NET == fsm_ptr->play_type)
    {
        s_is_special_pause = FALSE;
        FsmStateTrans(fsm_ptr,VP_STATE_PDP_ACTIVING); 
        if (!ActivePdp(fsm_ptr))
        {
            SendPlayCnfToUI(fsm_ptr,VP_ERROR_OTHERS);
            FsmStateTrans(fsm_ptr,VP_STATE_READY);                    
        }
        SCI_TRACE_LOW("[VP] MMIAPIVP_FSMCompulsiveResume resume special pause");
    }
#endif
    else
    {
        fsm_ptr->is_to_resume = TRUE; 
        MMIAPIVP_FSMResume(vp_handle);     
    }

    if(VP_STATE_PAUSED == MMIAPIVP_FSMGetState((VP_HANDLE)s_current_fsm_ptr))
    {
        MMISRVAUD_SetAudioPauseStateByHandle(fsm_ptr->srv_handle);
    }
    //VP_STRACE_LOW:"[VP] MMIAPIVP_FSMCompulsiveResume end fsm_ptr->is_to_resume = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_5852_112_2_18_3_5_15_242,(uint8*)"d",fsm_ptr->is_to_resume);
}


/*****************************************************************************/
//  Description : 获得视频播放的当前时间
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMIAPIVP_FSMGetTimeOffset(VP_HANDLE   vp_handle)
{
    VP_FSM_T       *fsm_ptr = (VP_FSM_T *)vp_handle;
    uint32          cur_time = 0;
    
    if (PNULL != fsm_ptr && IsValidFsm(fsm_ptr))
    {
        if (0 != fsm_ptr->dplayer_handle)
        {
            cur_time = DPLAYER_GetTime(fsm_ptr->dplayer_handle);
            fsm_ptr->process_time = cur_time;
        }
    }
   // VP_STRACE_LOW("[VP] MMIAPIVP_FSMGetTimeOffset cur_time = %x",cur_time);
    return cur_time;
}

#ifdef JAVA_SUPPORT_SUN//norflashcut
/*****************************************************************************/
//  Description : 获得视频播放的当前帧数。
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMIAPIVP_FSMGetFrameOffset(VP_HANDLE  vp_handle)
{
    VP_FSM_T                *fsm_ptr = (VP_FSM_T *)vp_handle;
    uint32                  time_offset = 0;
    uint32                  frame_index = 0;
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
                frame_index = (media_info.video_info.frame_rate)*time_offset/VP_SEC_TO_MSEL/media_info.video_info.frame_rate_base;
            }
        }
    }
    //VP_STRACE_LOW:"[VP] MMIAPIVP_FSMGetFrameOffset info_result = %d, frame_index = %d, time_offset = %d, media_info.video_info.frame_rate = %d, base = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_5902_112_2_18_3_5_15_243,(uint8*)"ddddd",info_result,frame_index,time_offset,media_info.video_info.frame_rate,media_info.video_info.frame_rate_base);
    return frame_index;
    
}
#endif




/*****************************************************************************/
//  Description : 获得视频信息
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN VP_GetMediaInfo(VP_HANDLE  vp_handle,
                            DPLAYER_MEDIA_INFO_T    *media_info_ptr)
{
    VP_FSM_T       *fsm_ptr = (VP_FSM_T *)vp_handle;
    DPLAYER_RETURN_E dplayer_result=DPLAYER_ERROR;
    BOOLEAN result=FALSE;
        
    if (PNULL == fsm_ptr || !IsValidFsm(fsm_ptr) || PNULL == media_info_ptr)
    {
        //VP_STRACE_LOW:"[VP] VP_GetMediaInfo param is error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_5924_112_2_18_3_5_15_244,(uint8*)"");
        return FALSE;
    }

    if(0 < fsm_ptr->dplayer_handle)
    {
        dplayer_result=DPLAYER_GetMovieInfo(fsm_ptr->dplayer_handle, media_info_ptr);
    }    

    if(dplayer_result==DPLAYER_SUCCESS)
    {
        result=TRUE;
    }
    return result;
}
/*****************************************************************************/
//  Description : 获得s_current_fsm_ptr
//  Global resource dependence :  none
//  Author:  
//  Note: 
/*****************************************************************************/
PUBLIC VP_HANDLE VP_GetCurrentFSMHandle(void)
{
   return  (VP_HANDLE)s_current_fsm_ptr;
}

/*****************************************************************************/
//  Description : 获得DPLAYERhandle
//  Global resource dependence :  none
//  Author:  
//  Note: 
/*****************************************************************************/
PUBLIC DPLAYER_HANDLE VP_GetDPLAYERhandlebyVPhandle(VP_HANDLE vphandle)
{
    if(vphandle == NULL)
    {
        VP_STRACE_LOW("[VP]: VP_GetDPLAYERhandlebyVPhandle vphandle is null");
        return NULL;
    }
   return    ((VP_FSM_T*)vphandle)->dplayer_handle ;
}
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
                )
{
    VP_SEEK_PARAM_T  seek_param = {0};

    if (PNULL == call_back_func || PNULL  == play_param_ptr)
    {
        //VP_STRACE_LOW:"[VP] MMIAPIVP_Play param is null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_5962_112_2_18_3_5_15_245,(uint8*)"");
        return FALSE;
    }

    //VP_STRACE_LOW:"[VP] MMIAPIVP_Play  start_time = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_5966_112_2_18_3_5_15_246,(uint8*)"d",start_time);
    //VP_STRACE_LOW:"[VP] MMIAPIVP_Play play_param_ptr = %x, full_path_len = %d, full_path_ptr = %x, is_random_play = %d, mode = %d, play_type = %d, url_ptr = %x, video_buf_info.type = %d, video_buf_len = %d, video_buf_ptr = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_5978_112_2_18_3_5_15_247,(uint8*)"dddddddddd",play_param_ptr, play_param_ptr->full_path_len,play_param_ptr->full_path_ptr,play_param_ptr->is_random_play,play_param_ptr->mode,play_param_ptr->play_type,play_param_ptr->url_ptr,play_param_ptr->video_buf_info.type,play_param_ptr->video_buf_info.video_buf_len,play_param_ptr->video_buf_info.video_buf_ptr);

     //VP_STRACE_LOW:"[VP] MMIAPIVP_Play audioSupport = %d, Height = %d, RotateAngle = %d, videoSupport = %d,Width = %d,xOffset = %d, yOffset = %d"
     SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_5987_112_2_18_3_5_15_248,(uint8*)"ddddddd",play_param_ptr->display_param.audioSupport,play_param_ptr->display_param.disp_rect.dy,play_param_ptr->display_param.RotateAngle,play_param_ptr->display_param.videoSupport,play_param_ptr->display_param.disp_rect.dx,play_param_ptr->display_param.disp_rect.x,play_param_ptr->display_param.disp_rect.y);
   

   
    if (PNULL == s_current_fsm_ptr)
    {         
		s_is_audio_coexist = play_param_ptr->is_audio_coexist;
        s_current_fsm_ptr = (VP_FSM_T *)MMIAPIVP_FSMInit(call_back_func);
		s_is_audio_coexist = FALSE;
    }
    else 
    {
        //VP_STRACE_LOW:"[VP] MMIAPIVP_Play fsm is not ready now, player wait"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_5997_112_2_18_3_5_15_249,(uint8*)"");
        return FALSE;
    }
    
    GUIBLOCK_SetType(255,MMIAPIVP_GetTransparenceColor(),GUIBLOCK_TYPE_OSD,(GUI_LCD_DEV_INFO *)MMITHEME_GetDefaultLcdDev());

#ifdef MMI_KING_MOVIE_SUPPORT
    MMIAPIKM_CkeckSupportfile( play_param_ptr->full_path_ptr);
#endif
    //VP_STRACE_LOW:"[VP] MMIAPIVP_Play call_back_func = %x ,start_time = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_6003_112_2_18_3_5_15_250,(uint8*)"dd",call_back_func,start_time);
    if (PNULL != s_current_fsm_ptr)
    {
        if(MMIAPIVP_FSMPlay((VP_HANDLE)s_current_fsm_ptr,play_param_ptr))
        {
            if (0 < start_time)
            {
                seek_param.seek_frame = VP_MAX_INVALID_VALUE;
                seek_param.seek_time = start_time;
                MMIAPIVP_FSMSeek((VP_HANDLE)s_current_fsm_ptr,&seek_param);                
            }
            return TRUE;
            
        }
        MMIAPIVP_FSMRelease((VP_HANDLE)s_current_fsm_ptr);
    }
    GUIBLOCK_ResetMainType();
    return FALSE;
}

/*****************************************************************************/
//  Description : 退出播放视频文件
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIVP_ExitPlay(void)
{
    //VP_STRACE_LOW:"[VP] MMIAPIVP_ExitPlay s_current_fsm_ptr = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_6029_112_2_18_3_5_15_251,(uint8*)"d",s_current_fsm_ptr);
    GUIBLOCK_ResetMainType();

    if (PNULL != s_current_fsm_ptr)
    {
        if(VP_STATE_READY == MMIAPIVP_FSMGetState((VP_HANDLE)s_current_fsm_ptr))
        {
            MMIAPIVP_FSMRelease((VP_HANDLE)s_current_fsm_ptr);
        }
        else
        {
            MMIAPIVP_FSMExit((VP_HANDLE)s_current_fsm_ptr);
        }        
    }  

}

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
                )
{
    DPLAYER_RETURN_E    ret         = DPLAYER_SUCCESS;
    void                *argv[1]    = {0}; 
    DPLAYER_HANDLE       dplayer_handle = 0;
#ifdef MMI_KING_MOVIE_SUPPORT
    VPLAYER_SECURITY_HANDLE         sec_handle=PNULL;
    BOOLEAN  b_sprdkmv=TRUE;
#endif

    if (PNULL == full_file_name || PNULL == media_ptr || 0 == full_name_len)
    {
        //VP_STRACE_LOW:"[VP]: MMIAPIVP_GetInfoFromFullPathName() param error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_6066_112_2_18_3_5_15_252,(uint8*)"");
        return FALSE;
    }   
    
    argv[0] = (void*)(full_file_name);
    DPLAYER_Init();

 
   dplayer_handle = DPLAYER_Open(DPLAYER_OPEN_FROM_FILE, 1, argv);
    
    if(0 >= dplayer_handle)
    {   
        //VP_STRACE_LOW:"[VP]: MMIAPIVP_GetInfoFromFullPathName: open dplayer fail! "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_6078_112_2_18_3_5_15_253,(uint8*)"");
        return FALSE;
    }

    ret = DPLAYER_GetMovieInfo(dplayer_handle, media_ptr); 
#ifdef MMI_KING_MOVIE_SUPPORT
    if(MMICOM_VIDEO_TYPE_KMV==MMIAPICOM_GetMovieType(full_file_name, full_name_len)) 
    {
        VDEC_SECURITY_PLUG_INFO_T **temp_data = SCI_ALLOC_APP(sizeof(VDEC_SECURITY_PLUG_INFO_T *));
        VP_STRACE_LOW("[VP]: MMIAPIVP_GetInfoFromFullPathName() s_vdec_sec_info_ptr:0x%x ,E",temp_data);
        sec_handle = VP_dplayerOpenSecurty(argv[0],&b_sprdkmv, temp_data);
        if(PNULL==sec_handle)
            {
                media_ptr->video_info.video_support=0;
                media_ptr->audio_info.audio_support=0;
            }
        VP_dplayerCloseSecurty(argv[0],sec_handle,b_sprdkmv, temp_data);
        SCI_FREE(temp_data);
        VP_STRACE_LOW("[VP]: MMIAPIVP_GetInfoFromFullPathName() s_vdec_sec_info_ptr:0x%x ,X",temp_data);
         
    }
#endif
   
    DPLAYER_Close(dplayer_handle);

    DPLAYER_Release();
    //VP_STRACE_LOW:"[VP]: MMIAPIVP_GetInfoFromFullPathName() return = 0x%.2x, full_name_len = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_6100_112_2_18_3_5_15_254,(uint8*)"dd", ret,full_name_len);
    //VP_STRACE_LOW:"[VP]: MMIAPIVP_GetInfoFromFullPathName():ntfile_type = %d,ntlen = %d,ntv_spt = %d,ntf_rate = %d,ntheight = %d,ntwidth = %d,ntvideo_type = %d,ntaudio_type = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_6111_112_2_18_3_5_15_255,(uint8*)"dddddddd",media_ptr->file_type, media_ptr->media_length,media_ptr->video_info.video_support,media_ptr->video_info.frame_rate,media_ptr->video_info.height,media_ptr->video_info.width,media_ptr->video_info.video_type, media_ptr->audio_info.audio_type);
    
    if(DPLAYER_SUCCESS == ret)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

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
                )
{
    DPLAYER_HANDLE          video_handle              = NULL;
    void                    *argv[3]            = {0}; 
    DPLAYER_FILE_TYPE_E     ref_file_type       = DPLAYER_FILETYPE_MAX;


    if (PNULL == video_buf || PNULL == media_ptr)
    {
        //VP_STRACE_LOW:"[VP]: MMIAPIVP_GetMediaInfoFromBuf param error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_6141_112_2_18_3_5_15_256,(uint8*)"");
        return;
    }

    //VP_STRACE_LOW:"[VP]: MMIAPIVP_GetMediaInfoFromBuf: video_type = %d, video_length = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_6145_112_2_18_3_5_15_257,(uint8*)"dd",video_type, video_length);

    switch(video_type)
    {
    case MMICOM_VIDEO_TYPE_AVI:
        ref_file_type = DPLAYER_FILETYPE_AVI;
        break;
    case MMICOM_VIDEO_TYPE_3GP:
        ref_file_type = DPLAYER_FILETYPE_3GP;
        break;
    case MMICOM_VIDEO_TYPE_MP4:
        ref_file_type = DPLAYER_FILETYPE_MP4;
        break;
    case MMICOM_VIDEO_TYPE_FLV:
        ref_file_type = DPLAYER_FILETYPE_FLV;
        break;
    case MMICOM_VIDEO_TYPE_RMVB:
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
            //VP_STRACE_LOW:"[VP]: MMIAPIVP_GetMediaInfoFromBuf: open dplayer fail! "
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_6180_112_2_18_3_5_16_258,(uint8*)"");
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
//  Description : 用于判断视频播放器对该视频文件的支持程度
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC VP_SUPPORTED_STATUS_TYPE_E MMIAPIVP_GetSupportedStatus(
                                    wchar   *full_file_name, 
                                    uint16    full_name_len
                                    )
{
    DPLAYER_MEDIA_INFO_T media_info = {0};
    
    if (PNULL == full_file_name || 0 == full_name_len)
    {
        //VP_STRACE_LOW:"[VP] MMIAPIVP_GetSupportedStatus param is null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_6207_112_2_18_3_5_16_259,(uint8*)"");
        return VP_SUPPORTED_NEITHER;
    }
    
    if (PNULL != s_current_fsm_ptr 
        && VP_STATE_READY != s_current_fsm_ptr->state
        && full_name_len == s_current_fsm_ptr->full_path_len 
        && PNULL != s_current_fsm_ptr->full_path_ptr
        && 0 == MMIAPICOM_Wstrcmp(full_file_name,s_current_fsm_ptr->full_path_ptr))
    {
        return s_current_fsm_ptr->support_type;
    }

    MMIAPIVP_GetInfoFromFullPathName(full_file_name, full_name_len, &media_info);
    
    return CheckSupportedTypeFromMediaInfo(&media_info);
}

/*****************************************************************************/
//  Description : Get the supported_type of a movie.
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL VP_SUPPORTED_STATUS_TYPE_E CheckSupportedTypeFromMediaInfo(DPLAYER_MEDIA_INFO_T *media_ptr)
{
    VP_SUPPORTED_STATUS_TYPE_E      supported_type  = VP_SUPPORTED_STATUS_TYPE_MAX;
    
    if (PNULL == media_ptr)
    {
        //VP_STRACE_LOW:"[VP] CheckSupportedTypeFromMediaInfo param is null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_6235_112_2_18_3_5_16_260,(uint8*)"");
        return VP_SUPPORTED_NEITHER;
    }
    //VP_STRACE_LOW:"[VP] CheckSupportedTypeFromMediaInfo video_support = %d, audio_support = %d , video_type = %d, audio_type = %d, height = %d, width = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_6244_112_2_18_3_5_16_261,(uint8*)"dddddd", media_ptr->video_info.video_support,media_ptr->audio_info.audio_support,media_ptr->video_info.video_type,media_ptr->audio_info.audio_type,media_ptr->video_info.height,media_ptr->video_info.width);

    if (1 == media_ptr->audio_info.audio_support
        && 1 == media_ptr->video_info.video_support)
    {
        supported_type = VP_SUPPORTED_BOTH;
    }
    else if (1 == media_ptr->audio_info.audio_support)
    {
        supported_type = VP_SUPPORTED_AUDIO_ONLY;
    }
    else if(1 == media_ptr->video_info.video_support)
    {
        supported_type = VP_SUPPORTED_VIDEO_ONLY;
    }
    else
    {
        supported_type = VP_SUPPORTED_NEITHER;
    }   
    
    return supported_type;
}

/*****************************************************************************/
//  Description : align 4
//  Global resource dependence : 
//  Author: bruce.chi
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMIVP_GetAlign4Offset(uint32 x)
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
#ifdef JAVA_SUPPORT_SUN//norflashcut
/*****************************************************************************/
//  Description : register current task for playing video
//  Global resource dependence : 
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIVP_RegTaskID(VP_HANDLE vp_handle, uint32 task_id)
{
    VP_FSM_T *fsm_ptr = (VP_FSM_T *)vp_handle;

    //VP_STRACE_LOW:"[VP] MMIVP_RegTaskID vp_handle = %x, task_id = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_6297_112_2_18_3_5_16_262,(uint8*)"dd",vp_handle,task_id);
    if (PNULL == fsm_ptr || !IsValidFsm(fsm_ptr))
    {
        return FALSE;
    }
    
    fsm_ptr->task_id = task_id;
    return TRUE;
}

/*****************************************************************************/
//  Description : unregister current task for playing video
//  Global resource dependence : 
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIVP_UnRegTaskID(VP_HANDLE vp_handle, uint32 task_id)
{
    VP_FSM_T *fsm_ptr = (VP_FSM_T *)vp_handle;

    //VP_STRACE_LOW:"[VP] MMIVP_UnRegTaskID vp_handle = %x, task_id = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_6315_112_2_18_3_5_16_263,(uint8*)"dd",vp_handle,task_id);
    if (PNULL == fsm_ptr || !IsValidFsm(fsm_ptr))
    {
        return;
    }
    
    fsm_ptr->task_id = P_APP;
}
#endif
/*****************************************************************************/
//  Description : create mutex 
//  Global resource dependence : 
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC void VP_CreateFsmMutex(void)
{
    if(s_fsm_list_mutex == 0)
    {
        s_fsm_list_mutex = SCI_CreateMutex(
            "VP_FSM_LIST_MUTEX",
            SCI_INHERIT
            );
    }
}

/*****************************************************************************/
//  Description : special pause
//  Global resource dependence : 
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN HandlePauseReqEx(BOOLEAN is_pause_streaming,
                               VP_FSM_T* fsm_ptr)
{
    BOOLEAN result = TRUE;

    if (PNULL == fsm_ptr || !IsValidFsm(fsm_ptr))
    {
        return FALSE;
    }

    MMISRVAUD_Pause(fsm_ptr->srv_handle); 
    if (is_pause_streaming && VP_PLAY_FROM_NET == fsm_ptr->play_type && VP_MAX_INVALID_VALUE != fsm_ptr->total_time)
    {  
       result = STRM_Pause(fsm_ptr->strm_handle);
    }
        
    if(!result)
    {          
        
       MMIAPIVP_FSMStop((VP_HANDLE)fsm_ptr);       
    } 

    return result;
}



/*****************************************************************************/
//  Description : special resume
//  Global resource dependence : 
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN HandleResumeReqEx(BOOLEAN is_resume_streaming,
                                VP_FSM_T* fsm_ptr)
{
    BOOLEAN result = TRUE;
    STRM_RESUME_PARAM_T strm_resume_param = {0};   
    BOOLEAN resume_result = FALSE;

    if (PNULL == fsm_ptr || !IsValidFsm(fsm_ptr))
    {
        return FALSE;
    }

    resume_result = MMISRVAUD_Resume(fsm_ptr->srv_handle);
    MMIAPIVP_FSMSwitchDisplayMode((VP_HANDLE)fsm_ptr,&(fsm_ptr->display_param),TRUE);    
    
    if(!resume_result)
    {
        SendFatalErrorIndTOUI(fsm_ptr,VP_ERROR_OTHERS);
        MMIAPIVP_FSMStop((VP_HANDLE)fsm_ptr);  
        return FALSE;
    }
        
    if (is_resume_streaming)
    {   
        if (VP_PLAY_FROM_NET == fsm_ptr->play_type && VP_MAX_INVALID_VALUE != fsm_ptr->total_time)
        {  
           strm_resume_param.start_time = VP_MAX_INVALID_VALUE;  
           result = STRM_Resume(fsm_ptr->strm_handle,&strm_resume_param);
        }
    }
    else
    {
// #ifdef  CAMERA_SUPPORT  
        MMISRVAUD_PauseSlightly(fsm_ptr->srv_handle);
// #endif
    }
        
    if(!result)
    {  
       SendFatalErrorIndTOUI(fsm_ptr,VP_ERROR_OTHERS);
       MMIAPIVP_FSMStop((VP_HANDLE)fsm_ptr);       
    } 

    return result;
}

#if  0//norflashcut
/*****************************************************************************/
//  Description : VP_UpdateCurrentFrame
//  Global resource dependence : 
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC void VP_UpdateCurrentFrame(VP_HANDLE vp_handle)
{
    VP_FSM_T   *fsm_ptr  = (VP_FSM_T *)vp_handle;
    
    if (PNULL == fsm_ptr || !IsValidFsm(fsm_ptr))
    {
        return;
    }
    else if(VP_STATE_READY < fsm_ptr->state)
    {
        DPLAYER_UpdateCurrentFrame(fsm_ptr->dplayer_handle);
    }
}
#endif

/*****************************************************************************/
//  Description : MMIAPIVP_FSMUpdatePlayBackMode
//  Global resource dependence : 
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIVP_FSMUpdatePlayBackMode(VP_HANDLE vp_handle, 
                              VP_PLAYBACK_MODE_E mode
                              )
{
	VP_FSM_T   *fsm_ptr  = (VP_FSM_T *)vp_handle;
     if (PNULL != fsm_ptr && IsValidFsm(fsm_ptr))
     {
         fsm_ptr->playback_mode = mode; 
		 SetPlaybackMode(fsm_ptr,fsm_ptr->playback_mode);	 
     }
}

/*****************************************************************************/
//  Description : SetPlaybackMode
//  Global resource dependence : 
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL void SetPlaybackMode(VP_FSM_T   *fsm_ptr, VP_PLAYBACK_MODE_E playback_mode)
{

	DPLAYER_PLAYBACK_MODE_E dplayer_playback_mode = (DPLAYER_PLAYBACK_MODE_E)playback_mode;
	
	if (PNULL != fsm_ptr && PNULL != fsm_ptr->dplayer_handle)
	{
		DPLAYER_SetPlaybackMode(fsm_ptr->dplayer_handle,dplayer_playback_mode);
	}	

}

/*****************************************************************************/
//  Description : MMIAPIVP_FSMUpdateCurrentVol
//  Global resource dependence : 
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIVP_FSMUpdateCurrentVol(VP_HANDLE vp_handle, uint8 vol)
{
    VP_FSM_T   *fsm_ptr  = (VP_FSM_T *)vp_handle;
    
    if (PNULL == fsm_ptr || !IsValidFsm(fsm_ptr))
    {
        return;
    }
    fsm_ptr->vol = vol;
    if(0 != fsm_ptr->srv_handle)
    {
        MMISRVAUD_SetVolume(fsm_ptr->srv_handle, vol);
    }
}

#ifdef MOBILE_VIDEO_SUPPORT//norflashcut
/*****************************************************************************/
//  Description : MMIAPIVP_FSMGetCurBufPercent
//  Global resource dependence : 
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMIAPIVP_FSMGetCurBufPercent(VP_HANDLE vp_handle)
{
    VP_FSM_T   *fsm_ptr  = (VP_FSM_T *)vp_handle;
    
    if (PNULL == fsm_ptr || !IsValidFsm(fsm_ptr))
    {
        return 100;
    }
    else
    {
        return fsm_ptr->buf_percent;
    }

}
#endif


/*****************************************************************************/
//  Description : MMIAPIVP_FSMUpdatePlayMode
//  Global resource dependence : 
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIVP_FSMUpdatePlayMode(VP_HANDLE vp_handle, 
                              VP_REPEAT_PLAY_MODE_E mode,
                              BOOLEAN is_random_play
                              )
{
     VP_FSM_T   *fsm_ptr  = (VP_FSM_T *)vp_handle;
     if (PNULL != fsm_ptr && IsValidFsm(fsm_ptr))
     {
         fsm_ptr->mode = mode;
         fsm_ptr->is_random_play = is_random_play;
     }
}
#ifndef MMI_VIDEOPLAYER_MINI_FUNCTION 
PUBLIC void DelaySwitchVideoPlayer(uint32 param)
{
    if (s_switch_timer_id!= 0)
    {
        MMK_StopTimer(s_switch_timer_id);
        s_switch_timer_id = 0;
    }
#ifdef BLUETOOTH_SUPPORT
#ifdef MMI_AMRBT_VPSUPPORT  
    if(MMIAPIBT_GetActiveBTHeadset())
    {
        s_switch_timer_id = MMK_CreateTimerCallback(MMI_2SECONDS, HandleDelaySwitchVideoPlayerTimer, param, FALSE);
    }
    else
    {
        s_switch_timer_id = MMK_CreateTimerCallback(MMI_1SECONDS, HandleDelaySwitchVideoPlayerTimer, param, FALSE);
    }
#else
    s_switch_timer_id = MMK_CreateTimerCallback(MMI_1SECONDS, HandleDelaySwitchVideoPlayerTimer, param, FALSE);
#endif
#else
    s_switch_timer_id = MMK_CreateTimerCallback(MMI_1SECONDS, HandleDelaySwitchVideoPlayerTimer, param, FALSE);
#endif
    //SCI_TRACE_LOW:"[VP] s_switch_timer_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_6547_112_2_18_3_5_16_264,(uint8*)"d",s_switch_timer_id);
}

PUBLIC void HandleDelaySwitchVideoPlayerTimer(
                   uint8 timer_id,
                   uint32 param
                    )
{    
    VP_FSM_T * fsm_ptr = (VP_FSM_T *)param;
    StopDelaySwitchVideoPlayerTimer();
    NotifyPlayNextFile(fsm_ptr,VP_PLAY_END_ERROR);
}

PUBLIC void StopDelaySwitchVideoPlayerTimer(
                    void
                    )
{
    //SCI_TRACE_LOW:"[VP] StopDelayPlayVideoPlayerTimer Stop Timer"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_6564_112_2_18_3_5_16_265,(uint8*)"");
    if (s_switch_timer_id != 0)
    {
        MMK_StopTimer(s_switch_timer_id);
        s_switch_timer_id = 0;
    }
}
#endif
#ifdef DRM_SUPPORT
/*****************************************************************************/
//  Description : consume drm right
//  Global resource dependence : 
//  Author: aoke.hu
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

#ifdef MMI_KING_MOVIE_SUPPORT
/*****************************************************************************/
PUBLIC USER_SECKEY_PROCESS_FUN userkey_process_callback(uint8 * user_secbuff, uint8 * org_seckey,uint16 keylen)
{

   SCI_MEMCPY(org_seckey, user_secbuff, keylen );

}/*lint !e533*/
//  Description : VP_dplayerOpenSecurty
//  Global resource dependence : 
//  Author: tonny.chen
//  Note: 
/*****************************************************************************/
PUBLIC VPLAYER_SECURITY_HANDLE VP_dplayerOpenSecurty(void * pvoid,BOOLEAN *b_sprd_kmv, VDEC_SECURITY_PLUG_INFO_T** sec_info_ptr)
{
    uint32 vdec_sec_argc = 1;
    VPLAYER_SECURITY_HANDLE sec_handle = PNULL;
    MMICOM_VIDEO_TYPE_E videotype = MMIAPICOM_GetMovieType((wchar * )(pvoid),MMIAPICOM_Wstrlen((wchar * )(pvoid)));
    BOOLEAN is_kmv=FALSE;
#ifdef MMI_KING_MOVIE_SUPPORT_HMV        
    BOOLEAN is_hmv=FALSE;
#endif
    VDEC_SECURITY_PLUG_INFO_T** temp_ptr = PNULL;
#ifdef MMI_KING_MOVIE_SUPPORT
      SPRDMV_UserSecFun_RegCallback(userkey_process_callback);
#endif

    if(PNULL != sec_info_ptr)
    {
        temp_ptr = sec_info_ptr;
    }
    else
    {
        temp_ptr = &s_vdec_sec_info_ptr;
    }
    
     if( MMICOM_VIDEO_TYPE_KMV == videotype)
    {
        is_kmv = TRUE;
    }
#ifdef MMI_KING_MOVIE_SUPPORT_HMV    
    else if( MMICOM_VIDEO_TYPE_HMV == videotype)
    {
        is_hmv = TRUE;
    }
    SCI_TRACE_LOW("[VP] VP_dplayerOpenSecurty enter is_kmv=%d, is_hmv=%d", is_kmv, is_hmv);
#endif	

    SCI_TRACE_LOW("[VP] VP_dplayerOpenSecurty enter sec_info_ptr=0x%x, s_vdec_sec_info_ptr=0x%x", sec_info_ptr,s_vdec_sec_info_ptr);
    
    if(is_kmv)
    {
        s_vdec_sec_argv[0] = pvoid;
        *b_sprd_kmv=TRUE;
        sec_handle = VPlayerSecurity_Open(vdec_sec_argc,(void *)s_vdec_sec_argv);
#ifdef MMI_KING_MOVIE_SUPPORT_EXTEND 
        if(PNULL == sec_handle)
        {
            sec_handle=VPlayerSecurity_Open_Kmv(vdec_sec_argc,(void *)s_vdec_sec_argv);
            if(PNULL != sec_handle)
            {
                 *b_sprd_kmv=FALSE;
            }
        }
#endif        
        //VP_STRACE_LOW:"[VP] VP_dplayerOpenSecurty handle=0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_6616_112_2_18_3_5_16_266,(uint8*)"d",sec_handle);
        if(sec_handle)
        {   
            if(*b_sprd_kmv)
            {
                VPlayerSecurity_GetVDecSecPlugInfo(sec_handle,temp_ptr);
            }
#ifdef MMI_KING_MOVIE_SUPPORT_EXTEND     
            else
            {
                VPlayerSecurity_GetVDecSecPlugInfo_Kmv(sec_handle,temp_ptr);
            }
#endif
            //VP_STRACE_LOW:"VP_dplayerOpenSecurty, VPlayerSecurity_GetVDecSecPlugInfo,FID=%d,plug_handle=0x%x"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_6630_112_2_18_3_5_16_267,(uint8*)"dd",(*temp_ptr)->FID, (*temp_ptr)->vdec_sec_handle);
               
        }
    }
#ifdef MMI_KING_MOVIE_SUPPORT_HMV
    else if(is_hmv)
    {
        s_vdec_sec_argv[0] = pvoid;
        *b_sprd_kmv=FALSE;
        sec_handle = VPlayerSecurity_Open_Hmv(vdec_sec_argc,(void *)s_vdec_sec_argv);        
        //VP_STRACE_LOW:"[VP] VP_dplayerOpenSecurty handle=0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_6640_112_2_18_3_5_16_268,(uint8*)"d",sec_handle);
        if(sec_handle)
        {
            VPlayerSecurity_GetVDecSecPlugInfo_Hmv(sec_handle,temp_ptr);
            //VP_STRACE_LOW:"VP_dplayerOpenSecurty, VPlayerSecurity_GetVDecSecPlugInfo,FID=%d,plug_handle=0x%x"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_6645_112_2_18_3_5_16_269,(uint8*)"dd",(*temp_ptr)->FID, (*temp_ptr)->vdec_sec_handle);
            
        }
    }
#endif
    SCI_TRACE_LOW("[VP] VP_dplayerOpenSecurty exit sec_info_ptr=0x%x, s_vdec_sec_info_ptr=0x%x", sec_info_ptr,s_vdec_sec_info_ptr);
    SCI_TRACE_LOW("[VP] VP_dplayerOpenSecurty exit");
    return sec_handle;
}

/*****************************************************************************/
//  Description : VP_dplayerCloseSecurty
//  Global resource dependence : 
//  Author: tonny.chen
//  Note: 
/*****************************************************************************/
PUBLIC void VP_dplayerCloseSecurty(void * pvoid,VPLAYER_SECURITY_HANDLE sec_handle,BOOLEAN b_sprd_kmv, VDEC_SECURITY_PLUG_INFO_T** sec_info_ptr)
{
    MMICOM_VIDEO_TYPE_E videotype = MMIAPICOM_GetMovieType((wchar * )(pvoid),MMIAPICOM_Wstrlen((wchar * )(pvoid)));
    BOOLEAN is_kmv=FALSE;
#ifdef MMI_KING_MOVIE_SUPPORT_HMV        
    BOOLEAN is_hmv=FALSE;
#endif
    VDEC_SECURITY_PLUG_INFO_T** temp_ptr = PNULL;

    if(PNULL != sec_info_ptr)
    {
        temp_ptr = sec_info_ptr;
    }
    else
    {
        temp_ptr = &s_vdec_sec_info_ptr;
    }
    
     if( MMICOM_VIDEO_TYPE_KMV == videotype)
    {
        is_kmv = TRUE;
    }
#ifdef MMI_KING_MOVIE_SUPPORT_HMV    
    else if( MMICOM_VIDEO_TYPE_HMV == videotype)
    {
        is_hmv = TRUE;
    }
#endif

    SPRDMV_UserSecFun_UnRegCallback( );
#ifdef MMI_KING_MOVIE_SUPPORT_HMV 
    SCI_TRACE_LOW("[VP] VP_dplayerCloseSecurty enter is_kmv=%d, is_hmv=%d", is_kmv, is_hmv);
#endif	
    if(is_kmv)
    {
        if(sec_handle)
        {   
            SCI_TRACE_LOW("[VP] VP_dplayerCloseSecurty enter sec_info_ptr=0x%x, s_vdec_sec_info_ptr=0x%x", sec_info_ptr,s_vdec_sec_info_ptr);
            //VP_STRACE_LOW:"VP_dplayerCloseSecurty,FID=%d,handle=0x%x,plug_handle=0x%x"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_6664_112_2_18_3_5_16_270,(uint8*)"ddd",(*temp_ptr)->FID,sec_handle, (*temp_ptr)->vdec_sec_handle);
            if(b_sprd_kmv)
            {
                VPlayerSecurity_Close(sec_handle);
            }
#ifdef MMI_KING_MOVIE_SUPPORT_EXTEND     
            else
            {
                VPlayerSecurity_Close_Kmv(sec_handle);
            }
#endif
            sec_handle = PNULL;
            *temp_ptr = PNULL;
        }
    }
    
#ifdef MMI_KING_MOVIE_SUPPORT_HMV
    else if(is_hmv)
    {
        if(sec_handle)
        {   
            SCI_TRACE_LOW("[VP] VP_dplayerCloseSecurty enter sec_info_ptr=0x%x, s_vdec_sec_info_ptr=0x%x", sec_info_ptr,s_vdec_sec_info_ptr);
            //VP_STRACE_LOW:"VP_dplayerCloseSecurty,FID=%d,handle=0x%x,plug_handle=0x%x"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_6686_112_2_18_3_5_17_271,(uint8*)"ddd",(*temp_ptr)->FID,sec_handle, (*temp_ptr)->vdec_sec_handle);
            VPlayerSecurity_Close_Hmv(sec_handle);
            
            sec_handle = PNULL;
            *temp_ptr = PNULL;
        }
    }
#endif
    SCI_TRACE_LOW("[VP] VP_dplayerCloseSecurty exit sec_info_ptr=0x%x, s_vdec_sec_info_ptr=0x%x", sec_info_ptr,s_vdec_sec_info_ptr);
    SCI_TRACE_LOW("[VP] VP_dplayerCloseSecurty exit");    
}

/*****************************************************************************/
//  Description : VP_dplayerExtCtlAddtSecurty
//  Global resource dependence : 
//  Author: tonny.chen
//  Note: 
/*****************************************************************************/
PUBLIC void VP_dplayerExtCtlAddSecurty(void * pvoid,DPLAYER_HANDLE player_handle)
{   
    uint32 vdec_sec_argc = 1;
    MMICOM_VIDEO_TYPE_E videotype = MMIAPICOM_GetMovieType((wchar * )(pvoid),MMIAPICOM_Wstrlen((wchar * )(pvoid)));
    BOOLEAN is_kmv=FALSE;
#ifdef MMI_KING_MOVIE_SUPPORT_HMV        
    BOOLEAN is_hmv=FALSE;
#endif
     if( MMICOM_VIDEO_TYPE_KMV == videotype)
    {
        is_kmv = TRUE;
    }
#ifdef MMI_KING_MOVIE_SUPPORT_HMV    
    else if( MMICOM_VIDEO_TYPE_HMV == videotype)
    {
        is_hmv = TRUE;
    }
    SCI_TRACE_LOW("[VP] VP_dplayerExtCtlAddSecurty enter is_kmv=%d, is_hmv=%d", is_kmv, is_hmv);
#endif
    if(is_kmv
        #ifdef MMI_KING_MOVIE_SUPPORT_HMV    
        || is_hmv
        #endif
        )
    {
        s_vdec_sec_argv[0] = (void *)s_vdec_sec_info_ptr;
        if(s_vdec_sec_info_ptr)
        {
            //VP_STRACE_LOW:"VP_dplayerExtCtlAddSecurty entry,s_vdec_sec_argv[0]=0x%x"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_6713_112_2_18_3_5_17_272,(uint8*)"d",s_vdec_sec_argv[0]);
                           
            DPLAYER_ExternalPlugControl(player_handle,DPLAYER_EXTPLUG_CTL_ADD_DEC_SECURITY,vdec_sec_argc,(void **)s_vdec_sec_argv );
        }
    }
    SCI_TRACE_LOW("[VP] VP_dplayerExtCtlAddSecurty exit ");
}

/*****************************************************************************/
//  Description : VP_dplayerExtCtlDeltSecurty
//  Global resource dependence : 
//  Author: tonny.chen
//  Note: 
/*****************************************************************************/
PUBLIC void VP_dplayerExtCtlDeltSecurty(void * pvoid,DPLAYER_HANDLE player_handle, VPLAYER_SECURITY_HANDLE sec_handle)
{
    uint32 vdec_sec_argc = 1;
    MMICOM_VIDEO_TYPE_E videotype = MMIAPICOM_GetMovieType((wchar * )(pvoid),MMIAPICOM_Wstrlen((wchar * )(pvoid)));
    BOOLEAN is_kmv=FALSE;
#ifdef MMI_KING_MOVIE_SUPPORT_HMV        
    BOOLEAN is_hmv=FALSE;
#endif
     if( MMICOM_VIDEO_TYPE_KMV == videotype)
    {
        is_kmv = TRUE;
    }
#ifdef MMI_KING_MOVIE_SUPPORT_HMV    
    else if( MMICOM_VIDEO_TYPE_HMV == videotype)
    {
        is_hmv = TRUE;
    }
    SCI_TRACE_LOW("[VP] VP_dplayerExtCtlDeltSecurty enter is_kmv=%d, is_hmv=%d", is_kmv, is_hmv);
#endif	
    if(is_kmv
        #ifdef MMI_KING_MOVIE_SUPPORT_HMV    
        || is_hmv
        #endif
        )
    {
        if(sec_handle)
        {
            //VP_STRACE_LOW:"VP_dplayerExtCtlDeltSecurty handle = 0x%x,s_vdec_sec_argv[0]=0x%x"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_6735_112_2_18_3_5_17_273,(uint8*)"dd", sec_handle,s_vdec_sec_argv[0]);
            DPLAYER_ExternalPlugControl(player_handle,DPLAYER_EXTPLUG_CTL_DEL_DEC_SECURITY,vdec_sec_argc,(void **)s_vdec_sec_argv );
        }
    }  
    SCI_TRACE_LOW("[VP] VP_dplayerExtCtlDeltSecurty exit");
}
#endif //MMI_KING_MOVIE_SUPPORT
#ifdef UI_MULTILAYER_SUPPORT
/*****************************************************************************/
//  Description : CreateVideoLayer
//  Global resource dependence : 
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIVP_CreateVideoLayer(void)
{
    BOOLEAN result = FALSE;
    UILAYER_RESULT_E layer_result  = UILAYER_RESULT_SUCCESS;
    uint16 lcd_width = 0;
    uint16 lcd_height = 0;
    UILAYER_CREATE_T create_info = {0}; 
    UILAYER_APPEND_BLT_T append_layer = {0};

    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);
    if ((UILAYER_HANDLE_MULTI == UILAYER_GetHandleType(&s_video_layer))
        && UILAYER_IsLayerActive(&s_video_layer))    
    {
        //已经创建
        result = TRUE;
    }
    else
    {       
        create_info.lcd_id          = LCD_ID_0;
        create_info.owner_handle    = 0;//MMIDC_MAIN_WIN_ID;//win_id;
        create_info.offset_x        = 0;//main_rect.left;
        create_info.offset_y        = 0;//main_rect.top;
        create_info.width           = lcd_width;//(main_rect.right+1-main_rect.left);
        create_info.height          = lcd_height;//(main_rect.bottom+1-main_rect.top);
        create_info.is_bg_layer     = TRUE;
        create_info.is_static_layer = TRUE;//FALSE;   
        create_info.format = UILAYER_MEM_IMMUTABLE;
        
        layer_result = UILAYER_CreateLayer(&create_info, &s_video_layer);

        if(UILAYER_RESULT_SUCCESS == layer_result)
        {
            append_layer.lcd_dev_info = s_video_layer;
            append_layer.layer_level = UILAYER_LEVEL_LOW;
            UILAYER_AppendBltLayer(&append_layer); 
            //UILAYER_SetLayerColorKey(dev_info_ptr, TRUE, UILAYER_TRANSPARENT_COLOR);
            //UILAYER_SetLayerAlpha(&s_panel_para.dev_layer_handle, 255*2/3);
            if (UILAYER_IsLayerActive(&s_video_layer))
            {
                UILAYER_Clear(&s_video_layer);
            }
            result = TRUE;
        }
        else
        {
            result = FALSE;
        }
    }
    
    return result;
}
#endif //UI_MULTILAYER_SUPPORT
/*****************************************************************************/
//  Description : destroy video layer
//  Global resource dependence : 
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIVP_DestroyVideoLayer(void)
{
#ifdef UI_MULTILAYER_SUPPORT
    UILAYER_RELEASELAYER(&(s_video_layer));   /*lint !e506 !e774*/
//    UILAYER_ReleaseLayer(&(s_video_layer));
    SCI_MEMSET(&s_video_layer, 0x00, sizeof(GUI_LCD_DEV_INFO));
#endif
}

/*****************************************************************************/
//  Description : switch current video/ 只转角度，不改变坐标区域
//  Global resource dependence : 
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIVP_CurrentVideoLcdSwitch(void)
{
    BOOLEAN result = FALSE;
    DPLAYER_MEDIA_INFO_T p_info = {0};
    UILAYER_INFO_T layer_info = {0};

    if(PNULL != s_current_fsm_ptr
        && s_current_fsm_ptr->is_cr_video_layer)
    {
        DPLAYER_GetMovieInfo(s_current_fsm_ptr->dplayer_handle,&p_info);
#ifdef UI_MULTILAYER_SUPPORT
        UILAYER_GetLayerInfo(&s_video_layer, &layer_info);
#endif
        if((layer_info.layer_height >=layer_info.layer_width
            &&p_info.video_info.height>=p_info.video_info.width)
            ||(layer_info.layer_height <layer_info.layer_width
            &&p_info.video_info.height<p_info.video_info.width))
        {
            if(LCD_ANGLE_0 != s_current_fsm_ptr->display_param.RotateAngle)
            {
                s_current_fsm_ptr->display_param.RotateAngle = LCD_ANGLE_0;
            }
        }
        else
        {
            if(LCD_ANGLE_90 != s_current_fsm_ptr->display_param.RotateAngle)
            {
                s_current_fsm_ptr->display_param.RotateAngle = LCD_ANGLE_90;
            }
        }
        s_current_fsm_ptr->display_param.disp_rect.dx = layer_info.layer_width;           
		s_current_fsm_ptr->display_param.disp_rect.dy = layer_info.layer_height;
        s_current_fsm_ptr->display_param.target_rect.dx = layer_info.layer_width;
		s_current_fsm_ptr->display_param.target_rect.dy = layer_info.layer_height;
        if(DPLAYER_SUCCESS == DPLAYER_SetDisplayParam(s_current_fsm_ptr->dplayer_handle, &s_current_fsm_ptr->display_param))
        {
            result = TRUE;
        }
        else
        {
            result = FALSE;
        }
    }

    return result;

    /*
    BOOLEAN result = FALSE;
    LCD_ANGLE_E lcd_angle = LCD_ANGLE_0;

    if(PNULL != s_current_fsm_ptr)
    {
        if(LCD_ANGLE_0 == lcd_angle || LCD_ANGLE_90 == lcd_angle)
        {
            if(s_current_fsm_ptr->display_param.RotateAngle > LCD_ANGLE_90)
            {
                result = TRUE;
                s_current_fsm_ptr->display_param.RotateAngle -= LCD_ANGLE_90;
            }
        }
        else
        {
            if(s_current_fsm_ptr->display_param.RotateAngle <= LCD_ANGLE_90)
            {
                result = TRUE;
                s_current_fsm_ptr->display_param.RotateAngle += LCD_ANGLE_90;
            }
        }
//        GUILCD_ConvertRectAndAngle(GUI_MAIN_LCD_ID,PNULL,&logic_angle,PNULL,&phy_angle);
//        s_current_fsm_ptr->display_param.RotateAngle = angle;
        if(result && DPLAYER_SUCCESS == DPLAYER_SetDisplayParam(s_current_fsm_ptr->dplayer_handle, &s_current_fsm_ptr->display_param))
        {
            result = TRUE;
        }
        else
        {
            result = FALSE;
        }
    }
    //SCI_TRACE_LOW:"[VP]:MMIAPIVP_CurrentVideoLcdSwitch result=%d,Angle=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_6890_112_2_18_3_5_17_274,(uint8*)"dd",result,s_current_fsm_ptr->display_param.RotateAngle);

    return result;
    */
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIVP_SetBgVideoAudioSupport(BOOLEAN is_audio_support)
{
    BOOLEAN result = FALSE;
    uint32 audioSupport = is_audio_support;
	VP_DISPLAY_PARAM_T display_param = {0};

    if(PNULL != s_current_fsm_ptr
        && s_current_fsm_ptr->is_audio_coexist )
    {
        //VP_STRACE_LOW:"[VP]:MMIAPIVP_SetBgVideoAudioSupport audioSupport=%d,audioSupport=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_6908_112_2_18_3_5_17_275,(uint8*)"dd",audioSupport,s_current_fsm_ptr->display_param.audioSupport);
        if(s_current_fsm_ptr->display_param.audioSupport != 0)
        {
            SCI_MEMCPY(&display_param,&(s_current_fsm_ptr->display_param),sizeof(VP_DISPLAY_PARAM_T));
            if(s_current_fsm_ptr->is_cr_video_layer)
            {
                SCI_TRACE_LOW("[VP]:MMIAPIVP_SetBgVideoAudioSupport is_cr_video_layer");
            }
            else
            {
                MMIAPIVP_TranslateDisplayParamLogicToPhysical(&display_param, FALSE);
            }
            display_param.audioSupport=audioSupport;
            if(DPLAYER_SUCCESS == DPLAYER_SetDisplayParam(s_current_fsm_ptr->dplayer_handle, &display_param))
            {
                result = TRUE;
            }
            else
            {
                result = FALSE;
            }
        }
    }
    
    return result;
}


/*****************************************************************************/
//  Description : 暂停视频播放
//  Global resource dependence :  none
//  Author: aoke.hu
//  Note: vp_handle为0时暂停当前播放的视频
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIVP_Pause(VP_HANDLE vp_handle)
{
    if(0 == vp_handle)
    {
        vp_handle = (VP_HANDLE)s_current_fsm_ptr;
        //SCI_TRACE_LOW:"[VP]:MMIAPIVP_Pause pause current video"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_6936_112_2_18_3_5_17_276,(uint8*)"");
    }
    return MMIAPIVP_FSMPause(vp_handle);
}

/*****************************************************************************/
//  Description : 恢复视频播放
//  Global resource dependence :  none
//  Author: aoke.hu
//  Note: vp_handle为0时恢复当前播放视频文件
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIVP_Resume(VP_HANDLE vp_handle)
{
    if(0 == vp_handle)
    {
        vp_handle = (VP_HANDLE)s_current_fsm_ptr;
        //SCI_TRACE_LOW:"[VP]:MMIAPIVP_Resume resume current video"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_6950_112_2_18_3_5_17_277,(uint8*)"");
    }
    return MMIAPIVP_FSMResume(vp_handle);
}

/*****************************************************************************/
//  Description : video player srv play call back function.
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN VPPlayCallBack(MMISRV_HANDLE_T handle, MMISRVMGR_NOTIFY_PARAM_T *param)   
{
    MMISRVAUD_REPORT_T *report_ptr = PNULL;
    VP_PLAY_END_RESULT_E  playend_result = VP_PLAY_END_NONE;
    
    if(PNULL != s_current_fsm_ptr
        && handle == s_current_fsm_ptr->srv_handle 
        && PNULL != param)
    {
        //SCI_TRACE_LOW:"[VP]:VPPlayCallBack event=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_6968_112_2_18_3_5_17_278,(uint8*)"d",param->event);
        switch(param->event)
        {
        case MMISRVMGR_NOTIFY_SUSPEND:
            SendPauseCnfToUI(s_current_fsm_ptr,VP_ERROR_NONE);
            break;
        case MMISRVMGR_NOTIFY_RESUME:
            MMISRVAUD_SetVolume(handle, (MMISRVAUD_VOLUME_T)MMIAPISET_GetMultimVolume());
            if(MMISRVAUD_IsAudPlaying(handle))
            {
                SendPlayCnfToUI(s_current_fsm_ptr,VP_ERROR_NONE);
            }
            break;
        case MMISRVMGR_NOTIFY_EXT:
            report_ptr = (MMISRVAUD_REPORT_T *)param->data;
            if(PNULL != report_ptr)
            {
                //SCI_TRACE_LOW:"[VP]:VPPlayCallBack report=%d,data=%d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_6985_112_2_18_3_5_17_279,(uint8*)"dd",report_ptr->report,report_ptr->data1);
                switch(report_ptr->report)
                {
                case MMISRVAUD_REPORT_END:
                    MMISRVAUD_Stop(handle);
                    switch(report_ptr->data1) 
                    {
                    case MMISRVAUD_REPORT_RESULT_NOT_SUPPORT:
                        playend_result = VP_PLAY_END_NOT_SUPPORT;
                    	break;
                    case MMISRVAUD_REPORT_RESULT_ERROR_UNKNOWN:
                    case MMISRVAUD_REPORT_RESULT_ERROR:
                        playend_result = VP_PLAY_END_ERROR;
                    	break;
                    case MMISRVAUD_REPORT_RESULT_STOP:
                        playend_result = VP_PLAY_END_STOPPED_BY_USER;
                    	break;
                    default:
                        playend_result = VP_PLAY_END_END_SUCCESS;
                        break;
                    }
                    SendSignalToCurPlayingTask(APP_VP_END_IND, (uint16)playend_result, s_current_fsm_ptr->task_id);   
                    break;

                case MMISRVAUD_REPORT_PLAY:
                    switch(report_ptr->data1) 
                    {
                    case MMISRVAUD_RET_OK:
#ifdef DRM_SUPPORT
                        ConsumeDrmRight(s_current_fsm_ptr->full_path_ptr,s_current_fsm_ptr->full_path_len);
#endif
                        break;

                    case MMISRVAUD_RET_ERROR:
                        playend_result = VP_PLAY_END_NOT_SUPPORT;
                        SendSignalToCurPlayingTask(APP_VP_END_IND, (uint16)playend_result, s_current_fsm_ptr->task_id);   
                        break;
                       
                    default:
                        break;
                    }
                    break;
#ifndef MMI_VIDEOPLAYER_MINI_FUNCTION                    
#ifdef MMI_AMRBT_VPSUPPORT
                case MMISRVAUD_REPORT_BT_EVENT:
                    switch(report_ptr->data1) 
                    {
                    case MMISRVAUD_BT_EVENT_KEY_PAUSE:
                    case MMISRVAUD_BT_EVENT_KEY_RESUME:
                        VP_EventPlayOrPause();
                    	break;

                    case MMISRVAUD_BT_EVENT_KEY_PREV:
                        VP_EventPrev();
                    	break;
                    case MMISRVAUD_BT_EVENT_KEY_NEXT:
                        VP_EventNext();
                    	break;

                    case MMISRVAUD_BT_EVENT_KEY_VOL_UP:
                        MMIAPIVP_TurnVol(MMICOM_PANEL_OPEN_INC);
                    	break;

                    case MMISRVAUD_BT_EVENT_KEY_VOL_DOWN:
                        MMIAPIVP_TurnVol(MMICOM_PANEL_OPEN_DEC);
                    	break;
                        
                    default:
                        break;
                    }
                    break;
#endif   
#endif
                case MMISRVAUD_REPORT_EARPHONE_EVENT:
                    switch(report_ptr->data1) 
                    {
                    case (MMISRVAUD_EARPHONE_EVENT_CLK + KEY_HEADSET_BUTTIN):
                        VP_EventPlayOrPause();
                    	break;

                    case (MMISRVAUD_EARPHONE_EVENT_LONG + KEY_HEADSET_BUTTIN):
                        VP_EventNext();
                    	break;

                    default:
                        break;
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
    }
    return TRUE;

}

/*****************************************************************************/
//  Description : 
//  Global resource dependence :  none
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN ReqPlaySrv(VP_FSM_T *fsm_ptr)
{
    MMISRVMGR_SERVICE_REQ_T req = {0};
    MMISRVAUD_TYPE_T audio_srv = {0};
    MMISRV_HANDLE_T handle = 0;
    BOOLEAN result = FALSE;
    MMISRVAUD_TYPE_U audio_info = {0};

    if (PNULL == fsm_ptr || !IsValidFsm(fsm_ptr))
    {
        return FALSE;
    }

    if(0 != fsm_ptr->srv_handle)
    {
        //SCI_TRACE_LOW:"[VP]:ReqPlaySrv handle is already exist!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_7088_112_2_18_3_5_17_280,(uint8*)"");
        if(fsm_ptr->is_audio_coexist)
        {
            audio_info.type = MMISRVAUD_TYPE_VIDEO_WP;
        }
        else
        {
            audio_info.type = MMISRVAUD_TYPE_VIDEO;
        }
        audio_info.video.data =  fsm_ptr->dplayer_handle;
        if(MMISRVAUD_SetAudioInfo(fsm_ptr->srv_handle,&audio_info))
        {
			MMISRVAUD_SetVolume(fsm_ptr->srv_handle, fsm_ptr->vol);
            result = TRUE;
        }
        else
        {
            //SCI_TRACE_LOW:"[VP]: MMISRVAUD_SetAudioInfo error!!!!!!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_7104_112_2_18_3_5_17_281,(uint8*)"");
            MMISRVAUD_Stop(fsm_ptr->srv_handle);
            result = FALSE;
        }
    }
    else
    {
        req.pri = MMISRVAUD_PRI_NORMAL;
        req.notify = VPPlayCallBack;
        if(fsm_ptr->is_audio_coexist)
        {
            audio_srv.info.type = MMISRVAUD_TYPE_VIDEO_WP;
        }
        else
        {
            audio_srv.info.type = MMISRVAUD_TYPE_VIDEO;
        }
        if (fsm_ptr->dplayer_handle > 0)
        {
            audio_srv.info.video.data = fsm_ptr->dplayer_handle;
        }
        else
        {
            audio_srv.info.video.data = 0x01;
        }
        
        audio_srv.volume = fsm_ptr->vol;
        audio_srv.volume_type = MMISRVAUD_VOLUME_TYPE_MED;
  #ifdef MMI_AMRBT_VPSUPPORT
        audio_srv.all_support_route = MMISRVAUD_ROUTE_AUTO;
  #else
        audio_srv.all_support_route = MMISRVAUD_ROUTE_SPEAKER| MMISRVAUD_ROUTE_EARPHONE;
  #endif
        handle = MMISRVMGR_Request(STR_SRV_AUD_NAME, &req, &audio_srv);
        if(handle > 0)
        {
            fsm_ptr->srv_handle = handle;
            result = TRUE;
        }
    }
    //SCI_TRACE_LOW:"[VP]:ReqPlaySrv handle = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_FSM_7135_112_2_18_3_5_17_282,(uint8*)"d",fsm_ptr->srv_handle);

    return result;
}

/*****************************************************************************/
//  Description : is VP playing
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIVP_IsVPPlaying(void)
{
    return ( VP_STATE_PLAYING == MMIAPIVP_FSMGetState((VP_HANDLE)s_current_fsm_ptr));
}
#ifdef MMI_STREAMING_PLAYER_SUPPORT
/*****************************************************************************/
//  Description : MMIAPIVP_IsSetSpecialPause
//  Global resource dependence : s_is_special_pause
//  Author: yanyan.an
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIVP_IsSetSpecialPause(BOOLEAN is_set)
{
    s_is_special_pause = is_set;
}
#endif
