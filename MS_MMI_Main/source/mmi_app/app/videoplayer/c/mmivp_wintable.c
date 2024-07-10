/*****************************************************************************
** File Name:      mmiVP_wintab.c                                         *
** Author:                                                                   *
** Date:           2010/3/26                                                 *
** Copyright:      2010 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe call log                    *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2010/3/26      apple.zhang        Create                                    *
******************************************************************************/
#ifndef MMI_VIDEOPLAYER_MINI_FUNCTION
/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "mmi_app_video_player_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "mmivp_export.h"
#include "mmiudisk_export.h"
#include "mmisd_export.h"
//#include "mmieng.h"
#include "mmieng_export.h"
#include "mmipub.h"
#include "mmi_default.h"
#include "mmi_text.h"
#include "mmi_image.h"
#include "mmivp_position.h"
#include "mmi_appmsg.h"
#include "mmivp_internal.h"
#include "mmivp_id.h"
#include "window_parse.h"
#include "guilabel.h"
#include "guires.h"
#include "mmivp_image.h"
#include "mmidisplay_data.h"
#include "mmivp_menutable.h"
#include "mmivp_text.h"
#include "mmiconnection_export.h"
#include "mmivp_internal.h"
#include "guirichtext.h"
#include "mmk_app.h"
#include "guiedit.h"
#include "mmivp_bookmarklist.h"
#include "mmivp_locallist.h"
#include "mmivp_historylist.h"
#include "mmi_resource.h"
#include "mmifilearray_export.h"
#include "mmicom_time.h"
#include "mmiidle_export.h"

#ifdef BROWSER_SUPPORT 
#include "mmibrowser_export.h"
#endif
#include "mmiset_display.h"
#include "guidropdownlist.h"
#include "guisetlist.h"
#include "guitext.h"
#include "mmiphone_export.h"
//#include "mmiset.h"
#include "mmiset_export.h"
#include "guiform.h"
#include "guistatusbar.h"
#include "guiblock.h"
#include "guitab.h"
#include "mmicc_export.h"
#include "guiownerdraw.h"
#include "guidc.h"
#include "guibutton.h"
#include "guisoftkey.h"
#include "guiownerdraw.h"
#include "guiform.h"
#include "guilcd.h"
#include "mmk_gsensor.h"
#ifdef JAVA_SUPPORT
#include "mmijava_export.h"
#endif
#include "mmimultim_text.h"
#include "mmivp_text_id.h"
#include "mmitheme_button.h"
#include "mmibt_export.h"
#include "mmifmm_export.h"
//#ifdef MOTION_SENSOR_TYPE   
//#include "mmiset_func.h"
//#endif

#ifdef MMI_GPRS_SUPPORT
#include "mmicl_export.h"
#endif

#ifdef DRM_SUPPORT
#include "mmidrm_export.h" 
#endif

#ifdef COUNTEDTIME_SUPPORT
#include "mmicountedtime_export.h"
#endif


#ifdef MMI_VIDEOWALLPAPER_SUPPORT
#include "mmiset_videowallpaper.h"
#endif

#include "mmimultim_image.h"
#include "mmifmm_internal.h"

#ifdef UI_MULTILAYER_SUPPORT
#include "ui_layer.h"
#endif
#include "mmiidle_statusbar.h"
#ifdef CMCC_UI_STYLE
#include "mmialarm_export.h"
#endif
#include "mmisms_export.h"  
#include "mmicom_trace.h"
#ifdef WATCH_LEFT_RIGHT_WEAR_SUPPORT
#include "dal_display.h"
#endif
/*---------------------------------------------------------------------------*/
/*                          MACRO DEFINITION                                 */
/*---------------------------------------------------------------------------*/
#define  VP_SEEK_STEP 5000    //seek间隔
#define VP_VOLUME_MIN MMISRVAUD_VOLUME_LEVEL_0
#define VP_VOLUME_MAX  MMISRVAUD_VOLUME_LEVEL_MAX
#define MMIVP_PLAYER_WIN_ID  MMIVP_MAIN_PLAY_WIN_ID //播放器主窗口id
#define VP_DISPLAY_TIME_LENGTH 32          //时间显示长度
#define VP_MIN_TO_SEC           60         //1min = 60s
#define VP_HOUR_TO_MIN           60        //1hour = 1min
#define MMIVP_LONG_REPEAT_TIME_OUT    500 //长按后repeat timer
#define MMIVP_TP_LONG_TIMER_OUT 1500 //tp long press timer
//#define MMIVP_VOL_BRIGHT_TIME_OUT 300 //音量或者亮度自动增加的time out
#define VP_PROCESS_TIME_OUT       300
#define VP_MAX_PORT              0xffff
#define VP_NONE_TRANSPARENCE     255
#define VP_HALF_TRANSPARENCE     230
#define VP_MAX_LOCALLIST_ITEMNUM 10000
#define VP_LOCALLIST_DELNUM_ONECIRCLE  100//400

 #ifdef  VIDEOTHUMBNAIL_SUPPORT  
LOCAL GUI_LCD_DEV_INFO s_thumbnail_layer = {0};//有时候横屏时所创建的缩略图的显示层  
#endif
#ifdef MMI_PDA_SUPPORT
    LOCAL BOOLEAN       s_is_markall = FALSE;
#endif /* MMI_PDA_SUPPORT */

/*---------------------------------------------------------------------------*/
/*                         GLOBAL DEFINITION                                 */
/*---------------------------------------------------------------------------*/
typedef enum
{
     VP_APPEND_LIST_ITEM_TYPE,          //append
     VP_REPLACE_LIST_ITEM_TYPE,         //replace
     VP_NEED_LIST_ITEM_DATA_TYPE,       //need data
     VP_NEED_LIST_ITEM_CONTENT_TYPE    //need content
} VP_LIST_ITEM_TYPE_E;



typedef void (*GetListItemContent)(uint16 list_index,
                                    wchar *item_content_ptr,
                                    uint16 *item_content_len_ptr,
                                    MMI_IMAGE_ID_T *image_id_ptr
                                    );
#ifdef MMI_STREAMING_PLAYER_SUPPORT
extern  int strnicmp (const char *a, const char *b, size_t c); /*lint !e752 !e526 */
#endif
#ifdef  VIDEOTHUMBNAIL_SUPPORT  
LOCAL  BOOLEAN  s_is_get_focus=FALSE;

typedef struct
{
    BOOLEAN is_getthumbnail_by_dpHandle;
    void (*pf_frame_end_notify)(int32 frame_num);  
 }DPHANDLE_GETTHUMBNAIL_T;


LOCAL DPHANDLE_GETTHUMBNAIL_T  s_vp_dphandle_getthumbnail = {0};  //备份dphandle的播放参数 ，获取缩略图后 恢复此播放参数 

/*****************************************************************************/
//  Description : 视频播放器载入显示祯功能，准备参数并开始缩略图获取
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN NotifyDPLAYERGetTumbnail(BOOLEAN is_dphandle);
/*****************************************************************************/
//  Description : 销毁横屏的缩略图显示层 ，并重置主层的参数
//  Global resource dependence : 
//  Author:  
//  Note: 
/*****************************************************************************/
LOCAL void DestroyLandscapeThumbnailLayer(void);
/*****************************************************************************/
//  Description :  设置主层的参数，实现叠加效果
//  Global resource dependence : 
//  Author:  
//  Note: 
/*****************************************************************************/
LOCAL void VPThumbnailSetMainLayer(void);
/*****************************************************************************/
//  Description :  重置主层的参数
//  Global resource dependence : 
//  Author:  
//  Note: 
/*****************************************************************************/
LOCAL void VPThumbnailRestoreMainLayer(void);
/*****************************************************************************/
//  Description :  设置主层的参数，实现叠加效果
//  Global resource dependence : 
//  Author:  
//  Note: 
/*****************************************************************************/
LOCAL void VPThumbnailResetDPLAYERParam(void);
/*****************************************************************************/
//  Description : IsAbleToFresh
//  Global resource dependence : 
//  Author: applezhang
//  Note: 处于focus状态，且当前 状态满足 
/*****************************************************************************/
LOCAL BOOLEAN IsAbleToGetThumbnail(void);
#endif
/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : 设置osd和透明色
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
LOCAL void SetBlockType(uint32 alpha);

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void VPRequestVirtualHandle(void);

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void VPReleaseVirtualHandle(void);

/*****************************************************************************/
//  Description : 提示出错信息
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void DisplayErrorCode(VP_ERROR_E error_code);

/*****************************************************************************/
//  Description : StartProcessTimer
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void StartProcessTimer(void);

/*****************************************************************************/
//  Description : PauseProcessTimer
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void PauseProcessTimer(void);

/*****************************************************************************/
//  Description : ResumeProcessTimer
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void ResumeProcessTimer(void);

/*****************************************************************************/
//  Description : StopProcessTimer
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void StopProcessTimer(void);

/*****************************************************************************/
//  Description : HandleProcessTimer
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void HandleProcessTimer(void);

/*****************************************************************************/
//  Description : HandleFsmMsg
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void HandleFsmMsg(VP_CALLBACK_T *msg_ptr);

/*****************************************************************************/
//  Description : SaveHistory
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SaveHistory(wchar *full_path_name_ptr,
                          uint16 full_path_name_len,
                          uint8 *url_ptr,
                          uint32 process_time,
                          VP_PLAY_TYPE_E play_type,                     
                          uint32 total_time);

/*****************************************************************************/
//  Description : NotifyPlayerForPlayFile
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN NotifyPlayerForPlayFile(wchar *full_path_name_ptr,
                          uint16 full_path_name_len,                       
                          uint32 process_time,
                          uint32 total_time,
                          VP_PLAY_TYPE_E play_type     
						 );
#ifdef MMI_STREAMING_PLAYER_SUPPORT
/*****************************************************************************/
//  Description : PlayFromNet
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN PlayFromNet(
                          uint32 process_time,
                          uint32 total_time
                          );
/*****************************************************************************/
//  Description : NotifyPlayerForPlay
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN NotifyPlayerForPlayNet(wchar *full_path_name_ptr,
                          uint16 full_path_name_len,
                          uint8 *url_ptr,
                          uint32 process_time,
                          uint32 total_time
                          );
#endif
/*****************************************************************************/
//  Description : 更新屏幕内容
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void UpdateVPWin(BOOLEAN is_fresh);

/*****************************************************************************/
//  Description :  加载本地播放列表等待窗口处理函数
//  Global resource dependence : 
//  Author:apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleVPSearchWaitingWin(
                         MMI_WIN_ID_T       win_id,     
                         MMI_MESSAGE_ID_E   msg_id, 
                         DPARAM             param
                         );

/*****************************************************************************/
//  Description : 进度条seek
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
LOCAL void OpenSeekWin(MMI_WIN_ID_T win_Id ,
                       GUI_POINT_T point,
                       BOOLEAN is_seek_now);

/*****************************************************************************/
//  Description : 打开文件选择窗口
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
LOCAL void OpenFileOptionWin(void);

/*****************************************************************************/
//  Description : 打开设置循环模式窗口
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
LOCAL void OpenRepeatModeWin(void);

/*****************************************************************************/
//  Description : 打开设置随机播放窗口
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
LOCAL void OpenRandomModeWin(void);

/*****************************************************************************/
//  Description : 打开音量调节窗口
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
LOCAL void OpenControlPanel(BOOLEAN is_vol_panel);

/*****************************************************************************/
//  Description : 播放或者暂停
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
LOCAL void PlayOrPause(void);

/*****************************************************************************/
//  Description : 切换屏幕
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
LOCAL void SwitchScreen(BOOLEAN is_full);

/*****************************************************************************/
//  Description : 使视频播放器主界面消失
//  Global resource dependence : 
//  Author: apple.zhang
//  Note:
/*****************************************************************************/
LOCAL void DisappearVPWin(MMI_WIN_ID_T  win_id,
                          BOOLEAN is_disappear,
                          BOOLEAN is_fresh);


/*****************************************************************************/
//  Description : 获取最后一次播放的信息
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN LoadLastPlayingFile(void);

/*****************************************************************************/
//  Description : 获取第一个本地播放列表信息
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
LOCAL void LoadFirstLocalListFile(void);

/*****************************************************************************/
//  Description : 设置播放速率
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
LOCAL void VideoPlayer_SetRate(BOOLEAN is_start, BOOLEAN is_backward);

/*****************************************************************************/
//  Description : 定位
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
LOCAL void VideoPlayerSeek(MMI_WIN_ID_T       win_id, uint32 seek_time);

/*****************************************************************************/
//  Description : 停止
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
LOCAL void VideoPlayerStop(MMI_WIN_ID_T       win_id);

/*****************************************************************************/
//  Description : 恢复
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
LOCAL void VideoPlayerResume(MMI_WIN_ID_T       win_id);

/*****************************************************************************/
//  Description : 恢复
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
LOCAL void VideoPlayerCompulsiveResume(MMI_WIN_ID_T       win_id);

/*****************************************************************************/
//  Description : 暂停
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
LOCAL void VideoPlayerCompulsivePause(MMI_WIN_ID_T       win_id);

/*****************************************************************************/
//  Description : 暂停
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN VideoPlayerPause(MMI_WIN_ID_T       win_id);
#ifdef MMI_STREAMING_PLAYER_SUPPORT
/*****************************************************************************/
//  Description : 选择sim卡
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN GetSimCardForPlay(MN_DUAL_SYS_E *dual_sys_ptr);
#endif
/*****************************************************************************/
//  Description : 播放
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
LOCAL void VideoPlayerPlay(MMI_WIN_ID_T       win_id);

/*****************************************************************************/
//  Description : 通知fsm播放
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN NotifyFsmPlay(void);

/*****************************************************************************/
//  Description : IsLastestHistory
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsLastestHistory(void);

/*****************************************************************************/
//  Description : 上一首
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
LOCAL void PlayPreFile(void);

/*****************************************************************************/
//  Description : 后退
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
LOCAL void Backward(BOOLEAN is_start,
                    BOOLEAN is_seek);

/*****************************************************************************/
//  Description : 下一首
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
LOCAL void PlayNextFile(void);

/*****************************************************************************/
//  Description : 前进
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
LOCAL void Forward(BOOLEAN is_start,
                   BOOLEAN is_seek);

/*****************************************************************************/
//  Description : 音量调节
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
//LOCAL uint8 AdjustVol(BOOLEAN is_up);

/*****************************************************************************/
//  Description : 音量调节
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
LOCAL void AdjustVolCallBack(MMICOM_PANEL_CALLBACK_T *para_ptr);

/*****************************************************************************/
//  Description : 亮度调节
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
//LOCAL uint8 AdjustBright(BOOLEAN is_up);

/*****************************************************************************/
//  Description : 亮度调节
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
LOCAL void AdjustBrightCallBack(MMICOM_PANEL_CALLBACK_T *para_ptr);

/*****************************************************************************/
//  Description : 启动vp win control pandle display timer
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
LOCAL void StartVPWinDisplayerTimer(void);

/*****************************************************************************/
//  Description : 停止vp win control pandle display timer
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
LOCAL void StopVPWinDisplayerTimer(void);

/*****************************************************************************/
//  Description : 启动vp timer
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
LOCAL void StartVPTimer(VP_TIMER_TYPE_E timer_type);

/*****************************************************************************/
//  Description : 停止vp timer
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
LOCAL void StopVPTimer(void);

/*****************************************************************************/
//  Description : vp timer
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN HandleVPTimerMsg(void);

/*****************************************************************************/
//  Description : 全屏状态下方向键处理函数.
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
//LOCAL void HandleDirectionKeyMsgForFullScreen(MMI_MESSAGE_ID_E msg_id);


/*****************************************************************************/
//  Description : 半屏状态下方向键处理函数.
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
LOCAL void HandleDirectionKeyMsgForHalfScreen(MMI_MESSAGE_ID_E msg_id);



/*****************************************************************************/
//  Description : 快捷键处理
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
LOCAL void HandleShortCutMsg(MMI_MESSAGE_ID_E msg_id);

#ifdef TOUCH_PANEL_SUPPORT /*pclint-650*/
/*****************************************************************************/
//  Description : down触笔处理
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
LOCAL void HandleTPDownMsg(DPARAM  param);
/*****************************************************************************/
//  Description : up触笔处理
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
LOCAL void HandleTPUpMsg(DPARAM  param);
/*****************************************************************************/
//  Description : Move触笔处理
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
LOCAL void HandleTPMoveMsg(DPARAM  param);
/*****************************************************************************/
//  Description : 触笔处理
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
LOCAL void HandleTPMsg(MMI_MESSAGE_ID_E msg_id, DPARAM  param);

/*****************************************************************************/
//  Description : 触笔处理
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
LOCAL void HandleCtrlTpMsg(DPARAM param);
#endif //end of TOUCH_PANEL_SUPPORT /*pclint-650*/
/*****************************************************************************/
//  Description : 设置播放器角度,禁止旋转
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
LOCAL void SetVpWinAngle(LCD_ANGLE_E angle);

/*****************************************************************************/
//  Description : 停止播放器素所有的timr
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
LOCAL void StopPlayerAllTimer(void);

/*****************************************************************************/
//  Description : 视频播放器主界面消息处理.
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleVPWinMsg(
                                     MMI_WIN_ID_T       win_id,     
                                     MMI_MESSAGE_ID_E    msg_id, 
                                     DPARAM              param
                                     );

/*****************************************************************************/
//  Description : OpenLocalFileWin
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN OpenLocalFileWin(MMI_WIN_ID_T       win_id);
#ifdef MMI_STREAMING_PLAYER_SUPPORT 
/*****************************************************************************/
//  Description : SaveBookMark
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN SaveBookMark(void);
#endif
/*****************************************************************************/
//  Description : 显示选项窗口
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
LOCAL void DisplayOptMenWin(MMI_WIN_ID_T       win_id, MMI_CTRL_ID_T    ctrl_id);


#ifdef MMI_STREAMING_PLAYER_SUPPORT  //norflashcut
/*****************************************************************************/
//  Description : 网络选择窗口选项消息处理.
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleNetOptMenuWinMsg(
                                     MMI_WIN_ID_T       win_id,     
                                     MMI_MESSAGE_ID_E    msg_id, 
                                     DPARAM              param
                                     );
#endif

/*****************************************************************************/
//  Description : 视频播放器选项消息处理.
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleOptMenuWinMsg(
                                     MMI_WIN_ID_T       win_id,     
                                     MMI_MESSAGE_ID_E    msg_id, 
                                     DPARAM              param
                                     );
#ifdef MMI_VIDEOPLAYER_HELP_SUPPORT
/*****************************************************************************/
//  Description : the window for vp help information
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleVPHelpWinMsg(
                                      MMI_WIN_ID_T win_id,
                                      MMI_MESSAGE_ID_E msg_id,
                                      DPARAM param
                                      );
#endif

#ifdef MMI_GPRS_SUPPORT
    /*****************************************************************************/
    //  Description : update net setting when the index list item is selected  
    //  Global resource dependence : none
    //  Author: jixin.xu
    //  Note:
    /*****************************************************************************/
LOCAL void MMIVP_ConnectionCallback(MN_DUAL_SYS_E dual_sys, uint32 selected_index);
#endif

#ifdef MMI_GUI_STYLE_TYPICAL //norflashcut
/*****************************************************************************/
//  Description : the window for vp setting information
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSettingMenuWinMsg(
                                      MMI_WIN_ID_T win_id,
                                      MMI_MESSAGE_ID_E msg_id,
                                      DPARAM param
                                      );
#endif

#ifdef MMI_STREAMING_PLAYER_SUPPORT  //norflashcut 
/*****************************************************************************/
//  Description :setting link list
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleLinkListWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/****************************************************************************/    
//  Description : SetInputRTCPPortDetailEntry
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void SetInputRTCPPortDetailEntry(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id);

/*****************************************************************************/
//  Description :HandleRTPOrRTCPWinMsg
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleRTPOrRTCPWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/****************************************************************************/    
//  Description : SetInputAgentDetailEntry
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void SetInputAgentDetailEntry(void);

/*****************************************************************************/
//  Description :HandleAgentWinMsg
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleAgentWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
#endif

/*****************************************************************************/
//  Description : 添加文件开始播放
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL void AddFileToPlayerCallBack(
                                      BOOLEAN     is_success,
                                      FILEARRAY   file_array
                                      );



/*****************************************************************************/
//  Description : 显示视频播放器主界面文件名称
//  Global resource dependence : 
//  Author: apple.zhang
//  Note:
/*****************************************************************************/
LOCAL void DisplayVPWinTitle(BOOLEAN is_fresh);

/*****************************************************************************/
//  Description : 停止显示视频播放器主界面状态名称消失timer
//  Global resource dependence : 
//  Author: apple.zhang
//  Note:
/*****************************************************************************/
LOCAL void StopVPDisplayStationTimer(void);

/*****************************************************************************/
//  Description : 启动显示视频播放器主界面状态名称消失timer
//  Global resource dependence : 
//  Author: apple.zhang
//  Note:
/*****************************************************************************/
LOCAL void StartVPDisplayStationTimer(void);

/*****************************************************************************/
//  Description : 处理显示视频播放器主界面状态名称消失timer
//  Global resource dependence : 
//  Author: apple.zhang
//  Note:
/*****************************************************************************/
LOCAL void HandleVPDisplayStationTimer(void);

/*****************************************************************************/
//  Description : 显示视频播放器主界面状态名称
//  Global resource dependence : 
//  Author: apple.zhang
//  Note:
/*****************************************************************************/
LOCAL void DisplayVPWinStation(MMI_TEXT_ID_T text_id,
                               BOOLEAN is_fresh,
                                BOOLEAN is_keep_last);

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: apple.zhang
//  Note:
/*****************************************************************************/
LOCAL void DisplayVPWinFileIcon(BOOLEAN is_fresh);

/*****************************************************************************/
//  Description : 显示视频播放器主界面循环播放图标
//  Global resource dependence : 
//  Author: apple.zhang
//  Note:
/*****************************************************************************/
LOCAL void DisplayVPWinRepeatModeIcon(BOOLEAN is_fresh);

/*****************************************************************************/
//  Description : 显示视频播放器主界面随机播放图标
//  Global resource dependence : 
//  Author: apple.zhang
//  Note:
/*****************************************************************************/
LOCAL void DisplayVPWinRandomModeIcon(BOOLEAN is_fresh);

/*****************************************************************************/
//  Description : 显示视频播放器主界面音量图标
//  Global resource dependence : 
//  Author: apple.zhang
//  Note:
/*****************************************************************************/
LOCAL void DisplayVPWinVolumeIcon(BOOLEAN is_fresh);

#ifdef MMI_PDA_SUPPORT//istyle_return
/*****************************************************************************/
//  Description : 显示视频播放器主界面返回 图标
//  Global resource dependence : 
//  Author: apple.zhang
//  Note:
/*****************************************************************************/
LOCAL void DisplayVPWinReturnIcion(BOOLEAN is_fresh);
#endif


/*****************************************************************************/
//  Description : 显示视频播放器主界面音量图标
//  Global resource dependence : 
//  Author: apple.zhang
//  Note:
/*****************************************************************************/
//LOCAL void DisplayVPWinVolumeIconEx(GUIBUTTON_OWNER_DRAW_T    *owner_draw_ptr);

/*****************************************************************************/
//  Description : 显示视频播放器主界面
//  Global resource dependence : 
//  Author: apple.zhang
//  Note:
/*****************************************************************************/
LOCAL void DisplayVPWinBackwardOrForwardIcon(BOOLEAN is_fresh);

/*****************************************************************************/
//  Description : 显示视频播放器主界面暂停或者播放图标
//  Global resource dependence : 
//  Author: apple.zhang
//  Note:
/*****************************************************************************/
LOCAL void DisplayVPWinPlayOrPauseIcon(BOOLEAN is_fresh);



/*****************************************************************************/
//  Description : 显示视频播放器主界面进度条和时间地显示
//  Global resource dependence : 
//  Author: apple.zhang
//  Note:
/*****************************************************************************/
LOCAL void DisplayVPWinProgressEx(GUIOWNDRAW_INFO_T *owner_draw_ptr);

/*****************************************************************************/
//  Description : 显示视频播放器主界面进度条和时间地显示
//  Global resource dependence : 
//  Author: apple.zhang
//  Note:
/*****************************************************************************/
 LOCAL void DisplayVPWinProgress(BOOLEAN is_fresh);

 /*****************************************************************************/
//  Description : 显示默认图片
//  Global resource dependence : 
//  Author: apple.zhang
//  Note:
/*****************************************************************************/
LOCAL void DisplayVpWinDefaultPic(void);

/*****************************************************************************/
//  Description : 显示视频播放器主界面进度条和时间地显示
//  Global resource dependence : 
//  Author: apple.zhang
//  Note:
/*****************************************************************************/
LOCAL void DisplayVpWinVideoArea(GUIOWNDRAW_INFO_T *owner_draw_ptr);

/*****************************************************************************/
//  Description : 显示视频播放器主界面时间显示
//  Global resource dependence : 
//  Author: apple.zhang
//  Note:
/*****************************************************************************/
LOCAL void DisplayVPWinProcessTime(GUI_RECT_T progress_rect);
/*****************************************************************************/
//  Description : 显示视频播放器切换屏幕图标
//  Global resource dependence : 
//  Author: apple.zhang
//  Note:
/*****************************************************************************/
LOCAL void DisplayVPWinSwitchScreenIcon(BOOLEAN is_fresh);

#ifdef MMI_STREAMING_PLAYER_SUPPORT
/*****************************************************************************/
//  Description :HandleAuthWinMsg
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleAuthWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/****************************************************************************/    
//  Description : SetInputAuthDetailEntry
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void SetInputAuthDetailEntry(MMI_CTRL_ID_T ctrl_id);
#endif
/****************************************************************************/    
//  Description :add item to richtext
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void RichText_AddItem(MMI_CTRL_ID_T ctrl_id, 
                            MMI_STRING_T * item_string, 
                            GUIRICHTEXT_FRAME_TYPE_E frame_type, 
                            GUIRICHTEXT_TAG_TYPE_E tag_type);



/*****************************************************************************/
//  Description :OpenBrowerWin
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void OpenBrowerWin(uint8 *url_ptr);
#ifdef MMI_STREAMING_PLAYER_SUPPORT
/*****************************************************************************/
//  Description :HandleBookmarkListWinMsg
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleBookmarkListWinMsg(MMI_WIN_ID_T win_id, 
                                    MMI_MESSAGE_ID_E msg_id, 
                                    DPARAM param);
#endif
/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author: apple.zhang
//  Note:
/*****************************************************************************/
LOCAL void InitListData(
                          MMI_CTRL_ID_T          ctrl_id,
                          uint32          total_num);
/*****************************************************************************/
//  Description : handle list append data
//  Global resource dependence : 
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL void HandleListAppendData(
                                   MMI_WIN_ID_T           win_id,    
                                   DPARAM param,
                                   MMI_HANDLE_T ctrl_handle,
                                   VP_LIST_ITEM_TYPE_E type,
                                   uint16 item_content_len,
                                   wchar *item_content_ptr,
                                   MMI_IMAGE_ID_T image_id
                                   );
#ifdef MMI_STREAMING_PLAYER_SUPPORT
/*****************************************************************************/
//  Description : 获取书签数据
//  Global resource dependence :
//  Author: apple.zhang
//  Note:
/*****************************************************************************/
LOCAL void GetBookmarkListItemContent(uint16 bookmark_list_index,
                                      wchar *item_content_ptr,
                                      uint16 *item_content_len_ptr,
                                      MMI_IMAGE_ID_T *image_id_ptr);
#endif
/*****************************************************************************/
//  Description : 更新列表数据
//  Global resource dependence :
//  Author: apple.zhang
//  Note:
/*****************************************************************************/
LOCAL void UpdateListData(
                          MMI_WIN_ID_T           win_id,     
                          MMI_CTRL_ID_T          ctrl_id,
                          uint32                 total_num,
                          GetListItemContent     getlistitemconten
                          );

/*****************************************************************************/
//  Description : 获取历史记录数据
//  Global resource dependence :
//  Author: apple.zhang
//  Note:
/*****************************************************************************/
LOCAL void GetHistoryListItemContent(uint16 historylist_index,
                                      wchar *item_content_ptr,
                                      uint16 *item_content_len_ptr,
                                      MMI_IMAGE_ID_T *image_id_ptr);


#ifdef MMI_VIDEOPLAYER_HISTORYLIST_SUPPORT
/*****************************************************************************/
//  Description :PlayCurrentHistoryVIdeo
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN PlayCurrentHistoryVIdeo(MMI_CTRL_ID_T list_ctrl_id, BOOLEAN is_go_on, BOOLEAN *is_query_ptr);

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : Del all History
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDelAllHistoryWinMsg(
                                             MMI_WIN_ID_T win_id,
                                             MMI_MESSAGE_ID_E msg_id,
                                             DPARAM param
                                             );
#endif

/*****************************************************************************/
//  Description :HandleHistoryListWinMsg
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleHistoryListWinMsg(MMI_WIN_ID_T win_id, 
                                    MMI_MESSAGE_ID_E msg_id, 
                                    DPARAM param);
#endif
#ifdef MMI_VIDEOPLAYER_SHORTCUTMENU_SUPPORT
/*****************************************************************************/
//  Description : CreateShortcutMenu
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
LOCAL void CreateShortcutMenu(MMI_CTRL_ID_T   ctrl_id);

/*****************************************************************************/
//  Description : 视频播放器快捷选项消息处理.
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleShortcutMenuWinMsg(
                                     MMI_WIN_ID_T       win_id,     
                                     MMI_MESSAGE_ID_E    msg_id, 
                                     DPARAM              param
                                     );
#endif

#ifdef MMI_STREAMING_PLAYER_SUPPORT  //norflashcut
/*****************************************************************************/
//  Description :HandleNetWinMsg
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleNetWinMsg(MMI_WIN_ID_T win_id, 
                                    MMI_MESSAGE_ID_E msg_id, 
                                    DPARAM param);
#endif

/*****************************************************************************/
//  Description : 获取本地播放列表数据
//  Global resource dependence :
//  Author: apple.zhang
//  Note:
/*****************************************************************************/
LOCAL void GetLocalListItemContent(uint16 locallist_index,
                                      wchar *item_content_ptr,
                                      uint16 *item_content_len_ptr,
                                      MMI_IMAGE_ID_T *image_id_ptr);

/*****************************************************************************/
//  Description :HandleLocalListWinMsg
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleLocalListWinMsg(MMI_WIN_ID_T win_id, 
                                    MMI_MESSAGE_ID_E msg_id, 
                                    DPARAM param);

/*****************************************************************************/
//  Description :HandleLocalListWinMsg
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDefaultLocalListWinMsg(MMI_WIN_ID_T win_id, 
                                    MMI_MESSAGE_ID_E msg_id, 
                                    DPARAM param);
#ifndef MMI_VIDEO_PLAYER_LOCALLIST_MINI_SUPPORT //6530_MINI
/*****************************************************************************/
//  Description :HandleUserLocalListWinMsg
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleUserLocalListWinMsg(MMI_WIN_ID_T win_id, 
                                    MMI_MESSAGE_ID_E msg_id, 
                                    DPARAM param);
#endif
 #if 0//norflashcut
/*****************************************************************************/
//  Description :HandleVPFileDetailWinMsg
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleVPFileDetailWinMsg(MMI_WIN_ID_T win_id, 
                                    MMI_MESSAGE_ID_E msg_id, 
                                    DPARAM param);
#endif
#ifdef MMI_STREAMING_PLAYER_SUPPORT
/*****************************************************************************/
//  Description :HandleVPNetDetailWinMsg
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleVPNetDetailWinMsg(MMI_WIN_ID_T win_id, 
                                    MMI_MESSAGE_ID_E msg_id, 
                                    DPARAM param);
/****************************************************************************/    
//  Description : SetNetDetailEntry
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void SetNetDetailEntry(MMI_CTRL_ID_T ctrl_id, VP_NET_MOVIE_INFO_T *net_movie_info_ptr);


#if  0  //norflashcut
/****************************************************************************/    
//  Description : SetFileDetailEntry
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void SetFileDetailEntry(MMI_CTRL_ID_T ctrl_id, VP_LOCAL_MOVIE_FILE_INFO_T* local_movie_file_info_ptr);
#endif

/*****************************************************************************/
//  Description :HandleAddBookmarkWinMsg
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleAddBookmarkWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/****************************************************************************/    
//  Description : SetInputBookmarkDetailEntry
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void SetInputBookmarkDetailEntry(MMI_CTRL_ID_T ctrl_id, VP_NET_MOVIE_INFO_T *bookmark_data_ptr);

/*****************************************************************************/
//  Description : 视频播放器书签选项消息处理.
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleBookmarkOptMenuWinMsg(
                                     MMI_WIN_ID_T       win_id,     
                                     MMI_MESSAGE_ID_E    msg_id, 
                                     DPARAM              param
                                     );
#endif
#ifdef MMI_VIDEOPLAYER_HISTORYLIST_SUPPORT
/*****************************************************************************/
//  Description : 视频播放器历史记录选项消息处理.
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleHistoryOptMenuWinMsg(
                                     MMI_WIN_ID_T       win_id,     
                                     MMI_MESSAGE_ID_E    msg_id, 
                                     DPARAM              param
                                     );
#endif //MMI_VIDEOPLAYER_HISTORYLIST_SUPPORT

/*****************************************************************************/
//  Description : AddLocalListCallBack
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
LOCAL void AddLocalListCallBack( VP_OP_LIST_RESULT_E    result, 
                                VP_LOCALLIST_TYPE_E locallist_type);
#ifndef MMI_VIDEO_PLAYER_MINI_SUPPORT //6530_MINI
/*****************************************************************************/
//  Description : 视频播放器本地列表选项消息处理.
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleLocalListOptMenuWinMsg(
                                     MMI_WIN_ID_T       win_id,     
                                     MMI_MESSAGE_ID_E    msg_id, 
                                     DPARAM              param
                                     );
#endif //end of MMI_VIDEO_PLAYER_MINI_SUPPORT //6530_MINI
/*****************************************************************************/
//  Description : SettingAsScreensaver
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
#ifdef SCREENSAVER_SUPPORT
LOCAL BOOLEAN SettingAsScreensaver(wchar *full_name_ptr, uint16 full_name_len);
#endif

#ifdef MMI_VIDEOWALLPAPER_SUPPORT
/*****************************************************************************/
//  Description : SettingAsScreensaver
//  Global resource dependence : 
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN SettingAsVideoWallpaper(wchar *full_name_ptr, uint16 full_name_len, uint32 file_size);
#endif

/*****************************************************************************/
//  Description : SettingAsPowerOnOffAnimation
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN SettingAsPowerOnOffAnimation(BOOLEAN is_power_on, wchar *full_name_ptr, uint16 full_name_len);
/*****************************************************************************/
//  Description : HandleFileOptionWinMsg
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleFileOptionWinMsg(
                                      MMI_WIN_ID_T win_id,
                                      MMI_MESSAGE_ID_E msg_id,
                                      DPARAM param
                                      );
/*****************************************************************************/
//  Description : 判断是否是否是有效的本地播放列表信息
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN CheckHistoryListItemIsHasValidLocalListInfo(VP_LOCALLIST_TYPE_E locallist_type,
                                                          int32 locallist_index, 
                                                          wchar * full_path_ptr
                                                          );
/*****************************************************************************/
//  Description : display player win soft key
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void DisplayVPWinSoftkey(BOOLEAN is_fresh);

/*****************************************************************************/
//  Description : open local list win
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void OpenLocalListWin(void);

/*****************************************************************************/
//  Description : IsSaveLastPlayPoint
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsSaveLastPlayPoint(void);

/*****************************************************************************/
//  Description : IsOpenOtherWinEnable
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsOpenOtherWinEnable(void);

/*****************************************************************************/
//  Description : IsAdjustBrightEnable
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsAdjustBrightEnable(void);

/*****************************************************************************/
//  Description : IsSeekEnable
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsSeekEnable(void);

/*****************************************************************************/
//  Description : 是否需要显示控制面板
//  Global resource dependence : 
//  Author: 
//  Note:  
/*****************************************************************************/
LOCAL BOOLEAN IsNeedDisplayControlPanel(void);

/*****************************************************************************/
//  Description : IsDisplayFullScreenReferInfoEnable
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsDisplayFullScreenReferInfoEnable(void);

/*****************************************************************************/
//  Description : IsSwitchFullScreenEnable
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsSwitchFullScreenEnable(void);


/*****************************************************************************/
//  Description : handle sim select window message
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
#ifdef MMI_STREAMING_PLAYER_SUPPORT
#if defined(MMI_WIFI_SUPPORT) || !defined(MMI_MULTI_SIM_SYS_SINGLE) 
LOCAL MMI_RESULT_E HandleSIMSelectWinMsg(
                                           MMI_WIN_ID_T win_id, 
                                           MMI_MESSAGE_ID_E msg_id, 
                                           DPARAM param
                                         );
#endif
#endif
#ifdef MMI_VIDEO_PLAYER_MINI_SUPPORT //6530_MINI
/*****************************************************************************/
//  Description : SetVPLayoutParam
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void SetVPLayoutParam(void);

#else
/*****************************************************************************/
//  Description : SetFormParam
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
LOCAL void SetFormParam(void);
#endif
/*****************************************************************************/
//  Description : SetFormParamV
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
LOCAL void SetFormParamV(void);
/*****************************************************************************/
//  Description : SetFormParamH
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
LOCAL void SetFormParamH(void);

#ifdef MMI_STREAMING_PLAYER_SUPPORT
/****************************************************************************/    
//  Description : SetNetViewDetailEntry
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void SetNetViewDetailEntry(MMI_CTRL_ID_T ctrl_id);
#endif
/*****************************************************************************/
//  Description : IsAbleToFresh
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsAbleToFresh(void);
//PDA style UI
#ifndef MMI_GUI_STYLE_TYPICAL
#ifdef MMI_PDA_SUPPORT
#ifdef MMI_STREAMING_PLAYER_SUPPORT
/*****************************************************************************/
//  Description : 书签列表 长按弹出选项消息处理.
//  Global resource dependence : 
//  Author: xin.li
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleBookmarkLongOkOptMenuWinMsg(
                                     MMI_WIN_ID_T       win_id,     
                                     MMI_MESSAGE_ID_E    msg_id, 
                                     DPARAM              param
                                     );
#endif
#ifdef MMI_VIDEOPLAYER_HISTORYLIST_SUPPORT
/*****************************************************************************/
//  Description : 历史记录列表 长按选项消息处理.
//  Global resource dependence : 
//  Author: xin.li
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleHistoryLongOkOptMenuWinMsg(
                                     MMI_WIN_ID_T       win_id,     
                                     MMI_MESSAGE_ID_E    msg_id, 
                                     DPARAM              param
                                     );
#endif
/*****************************************************************************/
//  Description : 本地列表 长按选项消息处理.
//  Global resource dependence : 
//  Author: xin.li
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleLocalListLongOkOptMenuWinMsg(
                                     MMI_WIN_ID_T       win_id,     
                                     MMI_MESSAGE_ID_E    msg_id, 
                                     DPARAM              param
                                     );
#ifdef MMI_STREAMING_PLAYER_SUPPORT
/*****************************************************************************/
// 	Description : handle message of input URL window of VP
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleVPInputNetUrlWinMsg(
                                             MMI_WIN_ID_T	    win_id, 
                                             MMI_MESSAGE_ID_E	msg_id, 
                                             DPARAM				param
                                             );

// 	Description : 网址浏览窗口 连接按钮callback
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E NetLinkButton1Callback(void);

/*****************************************************************************/
// 	Description :CMCC 窗口 连接按钮callback
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E NetLinkButton2Callback(void);

#endif
/*****************************************************************************/
// 	Description : 获取指定LIST项的文本信息
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN GetListItemTextInfo(
                                  MMI_CTRL_ID_T ctrl_id,    //[in]  控件ID
                                  uint16        index,      //[in]  索引
                                  MMI_STRING_T  *main_text,  //[out] 主标题
                                  MMI_STRING_T  *sub_text    //[out] 副标题
                                  );
#endif //MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : handle PDA style settings window message
//  Global resource dependence : 
//  Author: xin.li
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePdaSettingsWinMsg(
                                        MMI_WIN_ID_T        win_id,     
                                        MMI_MESSAGE_ID_E    msg_id, 
                                        DPARAM              param
                                        );


/*****************************************************************************/
//  Description : init pda style settings list ctrl
//  Global resource dependence : 
//  Author: xin.li
//  Note: 
/*****************************************************************************/
LOCAL void InitVpPdaSettingsListCtrl(void);

/*****************************************************************************/
//  Description : append settings item
//  Global resource dependence : 
//  Author: xin.li
//  Note: 
/*****************************************************************************/
LOCAL void AppendVpPdaSettingsItem(
                                 MMI_CTRL_ID_T  ctrl_id,    //控件ID
                                 MMI_TEXT_ID_T  text_1,     //主文本
                                 MMI_TEXT_ID_T  text_2,     //副文本
                                 MMI_IMAGE_ID_T image_id,   //图标ID
                                 uint32         user_data   //用户数据
                                 );

/*****************************************************************************/
//  Description : append settings item
//  Global resource dependence : 
//  Author: xin.li
//  Note: 
/*****************************************************************************/
LOCAL void AppendVpPdaSettingsItemEx(
                                 MMI_CTRL_ID_T  ctrl_id,            //控件ID
                                 MMI_TEXT_ID_T  text_1,             //主文本
                                 wchar          *text2_buf_ptr,     //副文本数据
                                 uint16         text2_buf_len,      //副文本长度
                                 MMI_IMAGE_ID_T image_id,           //图标ID
                                 uint32         user_data           //用户数据
                                 );



#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : get text_id by Repeat mode
//  Global resource dependence : 
//  Author: xin.li
//  Note: 
/*****************************************************************************/
LOCAL MMI_TEXT_ID_T GetTextIdByRepeatMode(VP_REPEAT_PLAY_MODE_E repeat_mode);

/*****************************************************************************/
//  Description : handle repeat set window message
//  Global resource dependence : 
//  Author: xin.li
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleVpRepeatSetWin(
                                     MMI_WIN_ID_T       win_id,     
                                     MMI_MESSAGE_ID_E   msg_id, 
                                     DPARAM             param
                                     );



/*****************************************************************************/
//  Description : append repeat list item
//  Global resource dependence : 
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL void AppendRepeatListItem(void);

/*****************************************************************************/
// 	Description : enter bookmark list marked status
//	Global resource dependence : none
//  Author: fen.xie
//	Note:
/*****************************************************************************/
LOCAL void VPEnterBookmarkListMarked(
                               MMI_CTRL_ID_T  list_ctrl_id
                               );//NEWMS00125002

/*****************************************************************************/
// 	Description : exit bookmark list marked status
//	Global resource dependence : none
//  Author: fen.xie
//	Note:
/*****************************************************************************/
LOCAL void VPExitBookmarkListMarked(
                               MMI_CTRL_ID_T  list_ctrl_id
                               );     
                       
#endif //MMI_PDA_SUPPORT
#endif //MMI_GUI_STYLE_TYPICAL

/*****************************************************************************/
//  Description : append palyback mode list item
//  Global resource dependence : 
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL void AppendPlaybackModeListItem(void);

/*****************************************************************************/
//  Description : get text_id by Playback Mode
//  Global resource dependence : 
//  Author: xin.li
//  Note: 
/*****************************************************************************/
LOCAL MMI_TEXT_ID_T GetTextIdByPlaybackMode(VP_PLAYBACK_MODE_E playback_mode);

/*****************************************************************************/
//  Description : handle playback mode set window message
//  Global resource dependence : 
//  Author: xin.li
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleVpPlaybackModeSetWin(
                                     MMI_WIN_ID_T       win_id,     
                                     MMI_MESSAGE_ID_E   msg_id, 
                                     DPARAM             param
                                     );

/*****************************************************************************/
//  Description : 转屏
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
LOCAL void HandleSwitchScreenMsg(void);
#ifdef TOUCH_PANEL_SUPPORT
/*****************************************************************************/
//  Description : double触笔处理
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
LOCAL void HandleTPDoubleMsg(DPARAM  param);
#endif

#ifdef MMI_VIDEO_PLAYER_MINI_SUPPORT //6530_MINI
/*****************************************************************************/
//  Description : handle repeat mode set window message
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleVPrepeatModeWin(
                        MMI_WIN_ID_T        win_id,     
                        MMI_MESSAGE_ID_E    msg_id, 
                        DPARAM              param
                        );
/*****************************************************************************/
//  Description : handle shuffle mode set window message
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/                        
LOCAL MMI_RESULT_E HandleVPShuffleModeWin(
                        MMI_WIN_ID_T        win_id,     
                        MMI_MESSAGE_ID_E    msg_id, 
                        DPARAM              param
                        );
/*****************************************************************************/
//  Description : AppendVPRepeatModeListItem
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/                        
LOCAL void AppendVPRepeatModeListItem(
                    MMI_CTRL_ID_T       ctrl_id
                    );
/*****************************************************************************/
//  Description : AppendVPShuffleModeListItem
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/                    
LOCAL void AppendVPShuffleModeListItem(
                    MMI_CTRL_ID_T       ctrl_id
                    );


/*****************************************************************************/
//  Description : 亮度调节
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void AdjustBrightness(MMICOM_PANEL_OPEN_TYPE_E turn_bright_type);

/*****************************************************************************/
//  Description : 显示OSD
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void DisplayOSD(void);
#endif
#ifdef MMI_VIDEOPLAYER_FIRST_SEARCH
/*****************************************************************************/
//  Description : 加载相关列表文件
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/

LOCAL void VP_LocalList_Load(void);
#endif

/*****************************************************************************/
//  Description : 获取下一个本地播放列表视频
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN  GetNextLocalListFileName(wchar  *full_path_name,uint16   *full_path_len);
/*****************************************************************************/
//  Description : 获取前一个本地播放列表视频
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN  GetPreLocalListFileName(wchar  *full_path_name,uint16   *full_path_len);

/*---------------------------------------------------------------------------*/
/*                         STATIC DEFINITION                                 */
/*---------------------------------------------------------------------------*/
const wchar  s_http_url_head[] = L"http://";
const wchar s_rtsp_url_head[] = L"rtsp://";
LOCAL VP_INTERFACE_PLAYING_INFO_T s_video_player_control_info = {0};
LOCAL uint8  s_process_timer_id = 0;
LOCAL MMI_CTRL_ID_T s_tp_down_ctrl_id = 0;
LOCAL GUI_RECT_T    s_tp_down_video_rect = {0};
LOCAL MMI_CTRL_ID_T s_key_down_msg_id = 0;
LOCAL BOOLEAN   s_is_lcd_switch = FALSE;
LOCAL BOOLEAN   s_is_refresh_switch = FALSE;
#ifdef MMI_STREAMING_PLAYER_SUPPORT
LOCAL BOOLEAN          s_is_need_auth = FALSE;   
#endif
LOCAL BOOLEAN  s_is_full_paint = FALSE;
LOCAL BOOLEAN  s_is_complusive_pause = FALSE;




/**--------------------------------------------------------------------------*
**                         CONSTANT VARIABLES                               *
**--------------------------------------------------------------------------*/
//视频播放器主界面
WINDOW_TABLE( MMIVP_MAIN_PLAY_WIN_TAB ) = 
{  
    
    WIN_FUNC( (uint32)HandleVPWinMsg ),      
    WIN_ID( MMIVP_MAIN_PLAY_WIN_ID ),
    WIN_STYLE(WS_DISPATCH_TO_CHILDWIN | WS_NEED_DBLCLK),
    WIN_MOVE_STYLE(MOVE_FORBIDDEN),
    END_WIN
};

//视频播放器主界面
WINDOW_TABLE( MMIVP_MAIN_PLAY_WIN_TAB_V ) = 
{      
    WIN_STATUSBAR,
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER, VP_PLAY_PANEL_FORM_CTRL_ID),
    CHILD_FORM_CTRL(FALSE,GUIFORM_LAYOUT_SBS,VP_PLAY_PANEL_TITLE_FORM_CTRL_ID,VP_PLAY_PANEL_FORM_CTRL_ID),
    CHILD_BUTTON_CTRL(FALSE,IMAGE_NULL,VP_PLAY_PANEL_RANDOM_BUTTON_CTRL_ID,VP_PLAY_PANEL_TITLE_FORM_CTRL_ID),
    CHILD_BUTTON_CTRL(FALSE,IMAGE_NULL,VP_PLAY_PANEL_REPEAT_BUTTON_CTRL_ID,VP_PLAY_PANEL_TITLE_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE,FALSE,VP_PLAY_NAME_LABEL_CTRL_ID,VP_PLAY_PANEL_TITLE_FORM_CTRL_ID),
    CHILD_BUTTON_CTRL(FALSE,IMAGE_NULL,VP_PLAY_PANEL_FILE_BUTTON_CTRL_ID,VP_PLAY_PANEL_TITLE_FORM_CTRL_ID),  
    CHILD_OWNDRAW_CTRL(FALSE,VP_PLAY_PANEL_SPACE_OWNDRAW_CTRL_ID,VP_PLAY_PANEL_TITLE_FORM_CTRL_ID,PNULL),


    CHILD_OWNDRAW_CTRL(FALSE,VP_PLAY_PANEL_VIDEO_OWNDRAW_CTRL_ID,VP_PLAY_PANEL_FORM_CTRL_ID,DisplayVpWinVideoArea),/*lint !e64 */

    CHILD_OWNDRAW_CTRL(FALSE,VP_PLAY_PANEL_PROCESS_OWNDRAW_CTRL_ID,VP_PLAY_PANEL_FORM_CTRL_ID,DisplayVPWinProgressEx),/*lint !e64 */

  
    CHILD_FORM_CTRL(FALSE,GUIFORM_LAYOUT_SBS,VP_PLAY_PANEL_BUTTON_FORM_CTRL_ID,VP_PLAY_PANEL_FORM_CTRL_ID),    
    CHILD_BUTTON_CTRL(FALSE,IMAGE_NULL,VP_PLAY_PANEL_VOL_BUTTON_CTRL_ID,VP_PLAY_PANEL_BUTTON_FORM_CTRL_ID),
    CHILD_BUTTON_CTRL(FALSE,IMAGE_NULL,VP_PLAY_PANEL_PRE_BUTTON_CTRL_ID,VP_PLAY_PANEL_BUTTON_FORM_CTRL_ID),
    CHILD_BUTTON_CTRL(FALSE,IMAGE_NULL,VP_PLAY_PANEL_PLAY_BUTTON_CTRL_ID,VP_PLAY_PANEL_BUTTON_FORM_CTRL_ID),
    CHILD_BUTTON_CTRL(FALSE,IMAGE_NULL,VP_PLAY_PANEL_NEXT_BUTTON_CTRL_ID,VP_PLAY_PANEL_BUTTON_FORM_CTRL_ID),
    CHILD_BUTTON_CTRL(FALSE,IMAGE_NULL,VP_PLAY_PANEL_FULLSCREEN_BUTTON_CTRL_ID,VP_PLAY_PANEL_BUTTON_FORM_CTRL_ID),
#ifdef MMI_PDA_SUPPORT//@andy.he_MP4PDA
    CHILD_OWNDRAW_CTRL(FALSE,VP_PLAY_PANEL_BTN_SPACE_OWNDRAW_CTRL_ID,VP_PLAY_PANEL_BUTTON_FORM_CTRL_ID,PNULL),
#endif
#ifndef MMI_PDA_SUPPORT
    WIN_SOFTKEY(STXT_OPTION, TXT_NULL, STXT_EXIT),
#endif
    END_WIN
};
#ifdef MMI_PDA_SUPPORT//istyle_return
//视频播放器主界面
WINDOW_TABLE( MMIVP_MAIN_PLAY_WIN_TAB_V_ISTYLE ) = 
{      
    WIN_STATUSBAR,
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER, VP_PLAY_PANEL_FORM_CTRL_ID),
    CHILD_FORM_CTRL(FALSE,GUIFORM_LAYOUT_SBS,VP_PLAY_PANEL_TITLE_FORM_CTRL_ID,VP_PLAY_PANEL_FORM_CTRL_ID),

    CHILD_BUTTON_CTRL(FALSE,IMAGE_NULL,VP_PLAY_PANEL_RETURN_BUTTON_CTRL_ID,VP_PLAY_PANEL_TITLE_FORM_CTRL_ID),
    CHILD_BUTTON_CTRL(FALSE,IMAGE_NULL,VP_PLAY_PANEL_REPEAT_BUTTON_CTRL_ID,VP_PLAY_PANEL_TITLE_FORM_CTRL_ID),
     CHILD_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE,FALSE,VP_PLAY_NAME_LABEL_CTRL_ID,VP_PLAY_PANEL_TITLE_FORM_CTRL_ID),
    CHILD_BUTTON_CTRL(FALSE,IMAGE_NULL,VP_PLAY_PANEL_RANDOM_BUTTON_CTRL_ID,VP_PLAY_PANEL_TITLE_FORM_CTRL_ID),
    CHILD_BUTTON_CTRL(FALSE,IMAGE_NULL,VP_PLAY_PANEL_FILE_BUTTON_CTRL_ID,VP_PLAY_PANEL_TITLE_FORM_CTRL_ID),  
    CHILD_OWNDRAW_CTRL(FALSE,VP_PLAY_PANEL_SPACE_OWNDRAW_CTRL_ID,VP_PLAY_PANEL_TITLE_FORM_CTRL_ID,PNULL),


    CHILD_OWNDRAW_CTRL(FALSE,VP_PLAY_PANEL_VIDEO_OWNDRAW_CTRL_ID,VP_PLAY_PANEL_FORM_CTRL_ID,DisplayVpWinVideoArea),/*lint !e64 */

    CHILD_OWNDRAW_CTRL(FALSE,VP_PLAY_PANEL_PROCESS_OWNDRAW_CTRL_ID,VP_PLAY_PANEL_FORM_CTRL_ID,DisplayVPWinProgressEx),/*lint !e64 */

  
    CHILD_FORM_CTRL(FALSE,GUIFORM_LAYOUT_SBS,VP_PLAY_PANEL_BUTTON_FORM_CTRL_ID,VP_PLAY_PANEL_FORM_CTRL_ID),    
    CHILD_BUTTON_CTRL(FALSE,IMAGE_NULL,VP_PLAY_PANEL_VOL_BUTTON_CTRL_ID,VP_PLAY_PANEL_BUTTON_FORM_CTRL_ID),
    CHILD_BUTTON_CTRL(FALSE,IMAGE_NULL,VP_PLAY_PANEL_PRE_BUTTON_CTRL_ID,VP_PLAY_PANEL_BUTTON_FORM_CTRL_ID),
    CHILD_BUTTON_CTRL(FALSE,IMAGE_NULL,VP_PLAY_PANEL_PLAY_BUTTON_CTRL_ID,VP_PLAY_PANEL_BUTTON_FORM_CTRL_ID),
    CHILD_BUTTON_CTRL(FALSE,IMAGE_NULL,VP_PLAY_PANEL_NEXT_BUTTON_CTRL_ID,VP_PLAY_PANEL_BUTTON_FORM_CTRL_ID),
    CHILD_BUTTON_CTRL(FALSE,IMAGE_NULL,VP_PLAY_PANEL_FULLSCREEN_BUTTON_CTRL_ID,VP_PLAY_PANEL_BUTTON_FORM_CTRL_ID),
    CHILD_OWNDRAW_CTRL(FALSE,VP_PLAY_PANEL_BTN_SPACE_OWNDRAW_CTRL_ID,VP_PLAY_PANEL_BUTTON_FORM_CTRL_ID,PNULL),
    END_WIN
};
#endif


//视频播放器主界面
#ifdef MMI_VIDEO_PLAYER_MINI_LAYOUT  //6530_MINI & 6530_MINI 全功能版
WINDOW_TABLE( MMIVP_MAIN_PLAY_WIN_TAB_H ) =      //220X176 176X220 128X160
{   

    CREATE_BUTTON_CTRL(IMAGE_NULL, VP_PLAY_PANEL_RANDOM_BUTTON_CTRL_ID),
    CREATE_BUTTON_CTRL(IMAGE_NULL, VP_PLAY_PANEL_REPEAT_BUTTON_CTRL_ID),
    CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, VP_PLAY_NAME_LABEL_CTRL_ID),
    CREATE_OWNDRAW_CTRL(VP_PLAY_PANEL_VIDEO_OWNDRAW_CTRL_ID, DisplayVpWinVideoArea),
    CREATE_BUTTON_CTRL(IMAGE_NULL, VP_PLAY_PANEL_PRE_BUTTON_CTRL_ID),
    CREATE_BUTTON_CTRL(IMAGE_NULL, VP_PLAY_PANEL_PLAY_BUTTON_CTRL_ID),
    CREATE_BUTTON_CTRL(IMAGE_NULL, VP_PLAY_PANEL_NEXT_BUTTON_CTRL_ID),
#if (defined MAINLCD_DEV_SIZE_220X176) || (defined MAINLCD_DEV_SIZE_160X128)
    CREATE_BUTTON_CTRL(IMAGE_NULL, VP_PLAY_PANEL_OPT_BUTTON_CTRL_ID),
    CREATE_BUTTON_CTRL(IMAGE_NULL, VP_PLAY_PANEL_RETURN_BUTTON_CTRL_ID),
#endif

    CREATE_OWNDRAW_CTRL(VP_PLAY_PANEL_PROCESS_OWNDRAW_CTRL_ID, DisplayVPWinProgressEx),
           
    END_WIN
};
#ifdef MMI_VIDEO_PLAYER_MINI_SUPPORT //6530_MINI
WINDOW_TABLE( MMIVP_PLAY_RANDOM_MODE_WIN_TAB ) = 
{
    WIN_TITLE( TXT_SET_SHUFFLE ),
    WIN_FUNC( (uint32)HandleVPShuffleModeWin ),    
    WIN_ID( MMIVP_SHUFFLE_MODE_WIN_ID ),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E,VP_PLAY_MODE_SHUFFLE_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};

WINDOW_TABLE( MMIVP_PLAY_REPEAT_MODE_WIN_TAB ) = 
{
    WIN_TITLE( TXT_COMMON_REPEAT ),
    WIN_FUNC( (uint32)HandleVPrepeatModeWin ),    
    WIN_ID( MMIVP_REPEAT_MODE_WIN_ID ),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E,VP_PLAY_MODE_REPEAT_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};
#endif  //MMI_VIDEO_PLAYER_MINI_SUPPORT
#else  //6530
WINDOW_TABLE( MMIVP_MAIN_PLAY_WIN_TAB_H ) = 
{   
    WIN_STATUSBAR,   
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER, VP_PLAY_PANEL_FORM_CTRL_ID),
    CHILD_FORM_CTRL(FALSE,GUIFORM_LAYOUT_SBS,VP_PLAY_PANEL_TITLE_FORM_CTRL_ID,VP_PLAY_PANEL_FORM_CTRL_ID),
    CHILD_BUTTON_CTRL(FALSE,IMAGE_NULL,VP_PLAY_PANEL_RANDOM_BUTTON_CTRL_ID,VP_PLAY_PANEL_TITLE_FORM_CTRL_ID),
    CHILD_BUTTON_CTRL(FALSE,IMAGE_NULL,VP_PLAY_PANEL_REPEAT_BUTTON_CTRL_ID,VP_PLAY_PANEL_TITLE_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE,FALSE,VP_PLAY_NAME_LABEL_CTRL_ID,VP_PLAY_PANEL_TITLE_FORM_CTRL_ID),
    CHILD_BUTTON_CTRL(FALSE,IMAGE_NULL,VP_PLAY_PANEL_FILE_BUTTON_CTRL_ID,VP_PLAY_PANEL_TITLE_FORM_CTRL_ID),  
    CHILD_OWNDRAW_CTRL(FALSE,VP_PLAY_PANEL_SPACE_OWNDRAW_CTRL_ID,VP_PLAY_PANEL_TITLE_FORM_CTRL_ID,PNULL),

    
    CHILD_OWNDRAW_CTRL(FALSE,VP_PLAY_PANEL_VIDEO_OWNDRAW_CTRL_ID,VP_PLAY_PANEL_FORM_CTRL_ID,DisplayVpWinVideoArea),/*lint !e64 */

    CHILD_OWNDRAW_CTRL(FALSE,VP_PLAY_PANEL_PROCESS_OWNDRAW_CTRL_ID,VP_PLAY_PANEL_FORM_CTRL_ID,DisplayVPWinProgressEx),/*lint !e64 */

   // CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,VP_PLAY_PANEL_STATION_LABEL_CTRL_ID,VP_PLAY_PANEL_FORM_CTRL_ID),
    CHILD_FORM_CTRL(FALSE,GUIFORM_LAYOUT_SBS,VP_PLAY_PANEL_BUTTON_FORM_CTRL_ID,VP_PLAY_PANEL_FORM_CTRL_ID),
    CHILD_BUTTON_CTRL(FALSE,IMAGE_NULL,VP_PLAY_PANEL_OPT_BUTTON_CTRL_ID,VP_PLAY_PANEL_BUTTON_FORM_CTRL_ID),
    CHILD_BUTTON_CTRL(FALSE,IMAGE_NULL,VP_PLAY_PANEL_VOL_BUTTON_CTRL_ID,VP_PLAY_PANEL_BUTTON_FORM_CTRL_ID),
    CHILD_BUTTON_CTRL(FALSE,IMAGE_NULL,VP_PLAY_PANEL_PRE_BUTTON_CTRL_ID,VP_PLAY_PANEL_BUTTON_FORM_CTRL_ID),
    CHILD_BUTTON_CTRL(FALSE,IMAGE_NULL,VP_PLAY_PANEL_PLAY_BUTTON_CTRL_ID,VP_PLAY_PANEL_BUTTON_FORM_CTRL_ID),
    CHILD_BUTTON_CTRL(FALSE,IMAGE_NULL,VP_PLAY_PANEL_NEXT_BUTTON_CTRL_ID,VP_PLAY_PANEL_BUTTON_FORM_CTRL_ID),
    CHILD_BUTTON_CTRL(FALSE,IMAGE_NULL,VP_PLAY_PANEL_FULLSCREEN_BUTTON_CTRL_ID,VP_PLAY_PANEL_BUTTON_FORM_CTRL_ID),
    CHILD_BUTTON_CTRL(FALSE,IMAGE_NULL,VP_PLAY_PANEL_RETURN_BUTTON_CTRL_ID,VP_PLAY_PANEL_BUTTON_FORM_CTRL_ID),
#ifdef MMI_PDA_SUPPORT//@andy.he_MP4PDA
    CHILD_OWNDRAW_CTRL(FALSE,VP_PLAY_PANEL_BTN_SPACE_OWNDRAW_CTRL_ID,VP_PLAY_PANEL_BUTTON_FORM_CTRL_ID,PNULL),
#endif
    END_WIN
};
#endif

#ifdef MMI_STREAMING_PLAYER_SUPPORT  //norflashcut
WINDOW_TABLE( MMIVP_NET_URL_OPT_MENU_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleNetOptMenuWinMsg),
    WIN_ID(MMIVP_OPT_MENU_WIN_ID),
    WIN_STYLE( WS_HAS_TRANSPARENT),
    CREATE_POPMENU_CTRL(MENU_VP_NET_URL_OPT,VP_NET_OPT_MENU_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),
    END_WIN
};
#endif


//选项窗口
WINDOW_TABLE( MMIVP_OPT_MENU_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleOptMenuWinMsg),
    WIN_ID(MMIVP_OPT_MENU_WIN_ID),
#ifdef MMI_VIDEO_PLAYER_MINI_LAYOUT//6530 MINI 界面布局
    WIN_TITLE(STXT_OPTION),
#else
    WIN_STYLE( WS_HAS_TRANSPARENT),
#endif
   // CREATE_POPMENU_CTRL(MENU_VP_OPT,VP_OPT_MENU_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),
    END_WIN
};

//本地文件播放时选项窗口
//WINDOW_TABLE( MMIVP_PLAYING_FILE_OPT_MENU_WIN_TAB ) = 
//{
//    WIN_FUNC( (uint32)HandleOptMenuWinMsg),
//    WIN_ID(MMIVP_PLAYING_FILE_OPT_MENU_WIN_ID),
//    WIN_STYLE( WS_HAS_TRANSPARENT),
//    CREATE_POPMENU_CTRL(MENU_VP_PLAYING_FILE_OPT,VP_OPT_MENU_CTRL_ID),
//    WIN_SOFTKEY(STXT_OK,TXT_NULL,STXT_RETURN),
//    END_WIN
// };

//网络文件播放时选项窗口
//WINDOW_TABLE( MMIVP_PLAYING_NET_FILE_OPT_MENU_WIN_TAB ) = 
//{
//    WIN_FUNC( (uint32)HandleOptMenuWinMsg),
//    WIN_ID(MMIVP_PLAYING_FILE_OPT_MENU_WIN_ID),
//    WIN_STYLE( WS_HAS_TRANSPARENT),
//    CREATE_POPMENU_CTRL(MENU_VP_PLAYING_NET_FILE_OPT,VP_OPT_MENU_CTRL_ID),
//    WIN_SOFTKEY(STXT_OK,TXT_NULL,STXT_RETURN),
//    END_WIN
//};

#ifdef MMI_VIDEOPLAYER_SHORTCUTMENU_SUPPORT
//快捷键窗口
WINDOW_TABLE( MMIVP_SHORTCUT_POPWIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleShortcutMenuWinMsg),
    WIN_ID(MMIVP_SHOUTCUT_OPT_MENU_WIN_ID),
#ifdef MMI_VIDEO_PLAYER_MINI_LAYOUT// MINI 界面布局
    WIN_TITLE(TXT_HELP),
#else
    WIN_STYLE( WS_HAS_TRANSPARENT),
#endif
    WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),
    END_WIN
};
#endif
#ifdef MMI_VIDEOPLAYER_HELP_SUPPORT
//帮助窗口
WINDOW_TABLE( MMIVP_HELP_INFO_WIN_TAB ) = 
{
    WIN_FUNC((uint32) HandleVPHelpWinMsg ),    
    WIN_ID( MMIVP_HELP_INFO_WIN_ID),
#ifndef MMI_ONLY_IDLE_DISP_STATUSBAR //6530_low_memory 不需要statusbar
    WIN_STATUSBAR,
#endif
    WIN_TITLE(TXT_HELP),    
    CREATE_TEXT_CTRL(VP_HELP_TEXT_CTRL_ID),
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_RETURN),
    END_WIN
}; 
#endif

#ifdef MMI_GUI_STYLE_TYPICAL //norflashcut
//设置窗口
WINDOW_TABLE( MMIVP_SETTING_WIN_TAB ) = 
{
    WIN_FUNC((uint32)HandleSettingMenuWinMsg),
    WIN_ID(MMIVP_SETTINGMENU_WIN_ID),
#ifndef MMI_ONLY_IDLE_DISP_STATUSBAR //MINI界面不需要statusbar
    WIN_STATUSBAR,
#endif
    WIN_TITLE(STXT_MAIN_SETTING),    
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_MENU_CTRL(MENU_VP_SET_OPT, VP_MENU_SET_OPT_CTRL_ID),
    END_WIN
};
#endif

#ifdef MMI_STREAMING_PLAYER_SUPPORT  //norflashcut 
//网络连接设置窗口
WINDOW_TABLE(MMIVP_LINKLIST_WIN_TAB ) = 
{
    WIN_FUNC((uint32)HandleLinkListWinMsg),    
    WIN_ID(MMIVP_LINKLIST_WIN_ID),
#ifndef MMI_ONLY_IDLE_DISP_STATUSBAR //MINI界面不需要statusbar
    WIN_STATUSBAR,
#endif
    WIN_TITLE(TXT_NETWORK_CONNECT_SET),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, VP_LISTBOX_LINK_SETTINGCTRL_ID),
    END_WIN
};

//rtp/rtcp端口设置
WINDOW_TABLE(MMIVP_RTP_RTCP_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleRTPOrRTCPWinMsg),    
    WIN_ID(MMIVP_RTCP_WIN_ID),
#ifndef MMI_ONLY_IDLE_DISP_STATUSBAR //MINI界面不需要statusbar
    WIN_STATUSBAR,
#endif
    WIN_TITLE(TXT_VP_SET_RTCP),
#ifndef MMI_PDA_SUPPORT
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
#else
    WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
    WIN_SOFTKEY(TXT_NULL, TXT_COMMON_OK, TXT_NULL),
#endif
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER, VP_RTCP_PORT_INPUT_FORM_CTRL_ID),
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,VP_RTCP_LOWEST_PORT_FORM_CTRL_ID,VP_RTCP_PORT_INPUT_FORM_CTRL_ID),
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,VP_RTCP_LOWEST_PORT_LABEL_CTRL_ID,VP_RTCP_LOWEST_PORT_FORM_CTRL_ID),
            CHILD_EDIT_DIGITAL_CTRL(TRUE,VP_MAX_PORT_LENGTH,VP_RTCP_LOWEST_PORT_INPUT_CTRL_ID,VP_RTCP_LOWEST_PORT_FORM_CTRL_ID),
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,VP_RTCP_HIGHEST_PORT_FROM_CTRL_ID,VP_RTCP_PORT_INPUT_FORM_CTRL_ID),
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,VP_RTCP_HIGHEST_PORT_LABEL_CTRL_ID,VP_RTCP_HIGHEST_PORT_FROM_CTRL_ID),
            CHILD_EDIT_DIGITAL_CTRL(TRUE,VP_MAX_PORT_LENGTH,VP_RTCP_HIGHEST_PORT_INPUT_CTRL_ID,VP_RTCP_HIGHEST_PORT_FROM_CTRL_ID),
    END_WIN
};

//代理设置
WINDOW_TABLE(MMIVP_AGENT_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleAgentWinMsg),    
    WIN_ID(MMIVP_AGENT_WIN_ID),
#ifndef MMI_ONLY_IDLE_DISP_STATUSBAR //MINI界面不需要statusbar
    WIN_STATUSBAR,
#endif
    WIN_TITLE(TXT_COMM_PROXY),
#ifndef MMI_PDA_SUPPORT
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
#else
    WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
    WIN_SOFTKEY(TXT_NULL, TXT_COMMON_OK, TXT_NULL),
#endif
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER, VP_AGENT_FORM_CTRL_ID),

        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,VP_AGENT_OPEN_FORM_CTRL_ID,VP_AGENT_FORM_CTRL_ID),
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,VP_AGENT_LABEL_CTRL_ID,VP_AGENT_OPEN_FORM_CTRL_ID),
            CHILD_SETLIST_CTRL(TRUE,VP_AGENT_INPUT_SETLIST_CTRL_ID,VP_AGENT_OPEN_FORM_CTRL_ID),

        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,VP_AGENT_IP_FORM_CTRL_ID,VP_AGENT_FORM_CTRL_ID),
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,VP_AGENT_IP_LABEL_CTRL_ID,VP_AGENT_IP_FORM_CTRL_ID),    
            CHILD_EDIT_IP_CTRL(TRUE,VP_AGENT_IP_INPUT_CTRL_ID,VP_AGENT_IP_FORM_CTRL_ID),

        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,VP_AGENT_PORT_FORM_CTRL_ID,VP_AGENT_FORM_CTRL_ID),
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,VP_AGENT_PORT_LABEL_CTRL_ID,VP_AGENT_PORT_FORM_CTRL_ID),
            CHILD_EDIT_DIGITAL_CTRL(TRUE,VP_MAX_PORT_LENGTH,VP_AGENT_PORT_INPUT_CTRL_ID,VP_AGENT_PORT_FORM_CTRL_ID),
    END_WIN
};
#endif

#ifdef MMI_STREAMING_PLAYER_SUPPORT
//授权窗口
WINDOW_TABLE(MMIVP_AUTH_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleAuthWinMsg),    
    WIN_ID(MMIVP_AUTH_WIN_ID),
#ifndef MMI_ONLY_IDLE_DISP_STATUSBAR //MINI界面不需要statusbar
    WIN_STATUSBAR,
#endif
    WIN_TITLE(TXT_VP_AUTH),
#ifndef MMI_PDA_SUPPORT
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
#endif
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER, VP_AUTH_INPUT_FORM_CTRL_ID),
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,VP_AUTH_USERNAME_FORM_CTRL_ID,VP_AUTH_INPUT_FORM_CTRL_ID),
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,VP_AUTH_USERNAME_LABEL_CTRL_ID,VP_AUTH_USERNAME_FORM_CTRL_ID),
            CHILD_EDIT_TEXT_CTRL(TRUE,VP_MAX_NAME_LEN,VP_AUTH_USERNAME_INPUT_CTRL_ID,VP_AUTH_USERNAME_FORM_CTRL_ID),
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,VP_AUTH_PASSWORD_FORM_CTRL_ID,VP_AUTH_INPUT_FORM_CTRL_ID),
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,VP_AUTH_PASSWORD_LABEL_CTRL_ID,VP_AUTH_PASSWORD_FORM_CTRL_ID),
            CHILD_EDIT_PASSWORD_CTRL(TRUE,VP_AUTH_MAX_PASSWORD_LEN,VP_AUTH_PASSWORD_INPUT_CTRL_ID,VP_AUTH_PASSWORD_FORM_CTRL_ID),

#ifdef MMI_PDA_SUPPORT
    CHILD_SOFTKEY_CTRL(TXT_COMMON_OK, TXT_NULL, STXT_RETURN,MMICOMMON_SOFTKEY_CTRL_ID,VP_AUTH_INPUT_FORM_CTRL_ID),
#endif

    END_WIN
};

//书签窗口
WINDOW_TABLE(MMIVP_BOOKMARKLIST_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleBookmarkListWinMsg),    
    WIN_ID(MMIVP_BOOKMARKLIST_WIN_ID),
#ifndef MMI_ONLY_IDLE_DISP_STATUSBAR //MINI界面不需要statusbar
    WIN_STATUSBAR,
#endif
#ifdef CMCC_UI_STYLE
    WIN_TITLE(TXT_COMMON_BOOKMARKS),
#else
    WIN_TITLE(TXT_COMMON_FAVORITE),
#endif
    WIN_SOFTKEY(STXT_OPTION, TXT_COMMON_PLAY, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E,VP_BOOKMARK_LIST_CTRL_ID),
    END_WIN
};

//添加书签
WINDOW_TABLE(MMIVP_ADD_BOOKMARK_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleAddBookmarkWinMsg),    
    WIN_ID(MMIVP_ADD_BOOKMARK_WIN_ID),
#ifndef MMI_ONLY_IDLE_DISP_STATUSBAR //MINI界面不需要statusbar
    WIN_STATUSBAR,
#endif
#ifdef CMCC_UI_STYLE
    WIN_TITLE(TXT_COMMON_BOOKMARKS),
#else
    WIN_TITLE(TXT_VP_SAVE_BOOKMARK_ID),
#endif
#ifndef MMI_PDA_SUPPORT
    WIN_SOFTKEY(STXT_SAVE, TXT_NULL, STXT_RETURN),
#else
    WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
    WIN_SOFTKEY(TXT_NULL, STXT_SAVE, TXT_NULL),
#endif
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER, VP_ADD_BOOKMARK_FORM_CTRL_ID),
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,VP_BOOKMARK_NAME_FORM_CTRL_ID,VP_ADD_BOOKMARK_FORM_CTRL_ID),
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,VP_BOOKMARK_NAME_LABEL_CTRL_ID,VP_BOOKMARK_NAME_FORM_CTRL_ID),
            CHILD_EDIT_TEXT_CTRL(TRUE,VP_MAX_NAME_LEN,VP_BOOKMARK_NAME_INPUT_CTRL_ID,VP_BOOKMARK_NAME_FORM_CTRL_ID),
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,VP_BOOKMARK_URL_FORM_CTRL_ID,VP_ADD_BOOKMARK_FORM_CTRL_ID),
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,VP_BOOKMARK_URL_LABEL_CTRL_ID,VP_BOOKMARK_URL_FORM_CTRL_ID),
            CHILD_EDIT_TEXT_CTRL(TRUE,VP_MAX_URL_LEN,VP_BOOKMARK_URL_INPUT_CTRL_ID,VP_BOOKMARK_URL_FORM_CTRL_ID),
    END_WIN
};


//书签选项窗口
WINDOW_TABLE( MMIVP_BOOKMARK_POPMENU_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleBookmarkOptMenuWinMsg),
    WIN_ID(MMIVP_BOOKMARK_OPT_MENU_WIN_ID),
    WIN_STYLE( WS_HAS_TRANSPARENT),
    CREATE_POPMENU_CTRL(MENU_VP_BOOKMARK_OPT,VP_BOOKMARK_OPT_MENU_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),
    END_WIN
};
#endif
#ifdef MMI_VIDEOPLAYER_HISTORYLIST_SUPPORT
//历史记录
WINDOW_TABLE(MMIVP_HISTORYLIST_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleHistoryListWinMsg),    
    WIN_ID(MMIVP_HISTORYLIST_WIN_ID),
#ifndef MMI_ONLY_IDLE_DISP_STATUSBAR //MINI界面不需要statusbar
    WIN_STATUSBAR,
#endif
    WIN_TITLE(TXT_VP_HISTORY_LIST),
    WIN_SOFTKEY(STXT_OPTION, TXT_COMMON_PLAY, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, VP_HISTORY_LIST_CTRL_ID),
    END_WIN
};
#endif
#ifdef MMI_VIDEOPLAYER_HISTORYLIST_SUPPORT
//历史记录选项窗口
WINDOW_TABLE( MMIVP_HISTORY_POPMENU_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleHistoryOptMenuWinMsg),
    WIN_ID(MMIVP_HISTORY_OPT_MENU_WIN_ID),
    WIN_STYLE( WS_HAS_TRANSPARENT),
    CREATE_POPMENU_CTRL(MENU_VP_HISTORY_OPT,VP_HISTORY_OPT_MENU_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),
    END_WIN
};
#endif //MMI_VIDEOPLAYER_HISTORYLIST_SUPPORT
#ifdef MMI_STREAMING_PLAYER_SUPPORT  //norflashcut
//网址浏览窗口
#ifndef MMI_PDA_SUPPORT
WINDOW_TABLE(MMIVP_NET_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleNetWinMsg),    
    WIN_ID(MMIVP_NET_WIN_ID),
#ifndef MMI_ONLY_IDLE_DISP_STATUSBAR //MINI界面不需要statusbar
    WIN_STATUSBAR,
#endif
    WIN_TITLE(TXT_VP_NET_VIEW),
    WIN_SOFTKEY(STXT_OPTION, TXT_COMMON_CONNECT_COMN, STXT_RETURN),
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER, VP_NET_FORM_CTRL_ID),  
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,VP_URL_INPUT_NAME_FORM_CTRL_ID,VP_NET_FORM_CTRL_ID),
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,VP_URL_INPUT_NAME_LABEL_CTRL_ID,VP_URL_INPUT_NAME_FORM_CTRL_ID),
            CHILD_EDIT_TEXT_CTRL(TRUE,VP_MAX_URL_LEN,VP_NET_URL_INPUT_CTRL_ID,VP_URL_INPUT_NAME_FORM_CTRL_ID),
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,VP_CMCC_NAME_FORM_CTRL_ID,VP_NET_FORM_CTRL_ID),
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,VP_CMCC_NAME_LABEL_CTRL_ID,VP_CMCC_NAME_FORM_CTRL_ID),
            CHILD_TEXT_CTRL(TRUE,VP_NET_CMCC_INPUT_CTRL_ID,VP_CMCC_NAME_FORM_CTRL_ID),    
    END_WIN
};
#else //PDA style
WINDOW_TABLE(MMIVP_NET_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleNetWinMsg),    
    WIN_ID(MMIVP_NET_WIN_ID),
    WIN_STATUSBAR,
    WIN_TITLE(TXT_VP_NET_VIEW),
    WIN_SOFTKEY(STXT_OPTION, TXT_COMMON_CONNECT_COMN, STXT_RETURN),
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER, VP_NET_FORM_CTRL_ID),  
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,VP_URL_INPUT_NAME_FORM_CTRL_ID,VP_NET_FORM_CTRL_ID),
            CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_SBS,VP_URL_INPUT_NAME_TITLE_FORM_CTRL_ID,VP_URL_INPUT_NAME_FORM_CTRL_ID),
                CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,VP_URL_INPUT_NAME_LABEL_CTRL_ID,VP_URL_INPUT_NAME_TITLE_FORM_CTRL_ID),
                CHILD_BUTTON_CTRL(TRUE,IMAGE_COMMON_RIGHT_ARROW,VP_URL_LINK_BUTTON1_CTRL_ID,VP_URL_INPUT_NAME_TITLE_FORM_CTRL_ID),
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,VP_NET_URL_INPUT_CTRL_ID,VP_URL_INPUT_NAME_FORM_CTRL_ID),
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,VP_CMCC_NAME_FORM_CTRL_ID,VP_NET_FORM_CTRL_ID),
            CHILD_FORM_CTRL(FALSE,GUIFORM_LAYOUT_SBS,VP_CMCC_NAME_TITLE_FORM_CTRL_ID,VP_CMCC_NAME_FORM_CTRL_ID),
                CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,TRUE,VP_CMCC_NAME_LABEL_CTRL_ID,VP_CMCC_NAME_TITLE_FORM_CTRL_ID),
                CHILD_BUTTON_CTRL(FALSE,IMAGE_COMMON_RIGHT_ARROW,VP_URL_LINK_BUTTON2_CTRL_ID,VP_CMCC_NAME_TITLE_FORM_CTRL_ID),
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,TRUE,VP_NET_CMCC_INPUT_CTRL_ID,VP_CMCC_NAME_FORM_CTRL_ID),
    END_WIN
};

//the window for url input    
WINDOW_TABLE(MMIVP_NET_URL_INPUT_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleVPInputNetUrlWinMsg),    
    WIN_ID(MMIVP_NET_URL_INPUT_WIN_ID),
     WIN_TITLE(TXT_VP_NET_VIEW),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_EDIT_TEXT_CTRL(VP_MAX_URL_LEN ,VP_NET_EDIT_INPUT_URL_CTRL_ID),
    END_WIN
};
#endif
#endif //MMI_PDA_SUPPORT

#ifdef MMI_VIDEO_PLAYER_MINI_SUPPORT //6530_MINI //6530_MINI 只有locallist
//本地播放列表窗口 6530_low_memory
WINDOW_TABLE( MMIVP_LOCALLIST_WIN_TAB ) = 
{    
    WIN_FUNC( (uint32) HandleDefaultLocalListWinMsg),    
    WIN_ID( MMIVP_DEFAULT_LOCALLIST_WIN_ID),
    WIN_TITLE(TXT_VP_LOCALLIST),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, VP_DEFAULT_LOCALLIST_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_PLAY, TXT_NULL, STXT_RETURN),
    END_WIN
};
#else
//本地播放列表窗口
WINDOW_TABLE( MMIVP_LOCALLIST_WIN_TAB ) = 
{    
    WIN_FUNC( (uint32) HandleLocalListWinMsg),    
    WIN_ID( MMIVP_LOCALLIST_WIN_ID),
#ifndef MMI_ONLY_IDLE_DISP_STATUSBAR //6530_MINI界面不需要statusbar
    WIN_STATUSBAR,
#endif
#if (defined MMI_GUI_STYLE_TYPICAL) || (defined MMI_GUI_STYLE_MINISCREEN)
    CREATE_TAB_CTRL(GUI_TAB_ITEM_LEFT,GUITAB_STATE_NONEED_SCROLL, VP_LOCALLIST_CTRL_ID),
#else
	CREATE_TAB_CTRL(GUI_TAB_ITEM_LEFT, GUITAB_STATE_NONEED_SCROLL|GUITAB_STATE_SINGLE_LINE, VP_LOCALLIST_CTRL_ID),
#endif
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};

//默认本地播放列表窗口
WINDOW_TABLE(MMIVP_DEFAULT_LOCALLIST_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleDefaultLocalListWinMsg),    
    WIN_ID(MMIVP_DEFAULT_LOCALLIST_WIN_ID),
#ifndef MMI_ONLY_IDLE_DISP_STATUSBAR //6530_MINI界面不需要statusbar
    WIN_STATUSBAR,
#endif
    WIN_SOFTKEY(STXT_OPTION, TXT_COMMON_PLAY, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, VP_DEFAULT_LOCALLIST_CTRL_ID),
    END_WIN
};

//用户自定义本地播放列表窗口
WINDOW_TABLE(MMIVP_USER_LOCALLIST_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleUserLocalListWinMsg),    
    WIN_ID(MMIVP_USER_LOCALLIST_WIN_ID),   
#ifndef MMI_ONLY_IDLE_DISP_STATUSBAR //6530_MINI界面不需要statusbar
    WIN_STATUSBAR,
#endif
#ifdef MMI_PDA_SUPPORT//NEWMS00125002
    WIN_STYLE(WS_DISABLE_SOFTKEY_TO_TITLE),
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER, VP_USER_LOCALLIST_FORM_CTRL_ID),
    CHILD_SOFTKEY_CTRL(TXT_MARK_ALL, TXT_DELETE, STXT_CANCEL, MMICOMMON_SOFTKEY_CTRL_ID, VP_USER_LOCALLIST_FORM_CTRL_ID),
    CHILD_LIST_CTRL(TRUE, GUILIST_TEXTLIST_E, VP_USER_LOCALLIST_CTRL_ID, VP_USER_LOCALLIST_FORM_CTRL_ID),
#else     
    WIN_SOFTKEY(STXT_OPTION, TXT_COMMON_PLAY, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, VP_USER_LOCALLIST_CTRL_ID),
#endif
    END_WIN
};
#endif //end of MMI_VIDEO_PLAYER__MINI_SUPPORT  //6530_MINI 只有locallist

#ifndef MMI_VIDEO_PLAYER_MINI_SUPPORT //6530_MINI
//默认本地播放列表选项窗口
WINDOW_TABLE( MMIVP_DEFAULT_LOCALLIST_POPMENU_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleLocalListOptMenuWinMsg),
    WIN_ID(MMIVP_DEFAULT_LOCALLIST_OPT_MENU_WIN_ID),
    WIN_STYLE( WS_HAS_TRANSPARENT),
    CREATE_POPMENU_CTRL(MENU_VP_DEFAULT_LOCALLIST_OPT,VP_DEFAULT_LOCALLIST_OPT_MENU_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),
    END_WIN
};

//用户自定义本地播放列表选项窗口
WINDOW_TABLE( MMIVP_USER_LOCALLIST_POPMENU_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleLocalListOptMenuWinMsg),
    WIN_ID(MMIVP_USER_LOCALLIST_OPT_MENU_WIN_ID),
    WIN_STYLE( WS_HAS_TRANSPARENT),
    CREATE_POPMENU_CTRL(MENU_VP_USER_LOCALLIST_OPT,VP_USER_LOCALLIST_OPT_MENU_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),
    END_WIN
};
#endif //end of  MMI_VIDEO_PLAYER_MINI_SUPPORT //6530_MINI
 #if 0//norflashcut
//本地文件属性
WINDOW_TABLE(MMIVP_VIEW_FILE_DETAIL_WIN_TAB) =
{    
    WIN_FUNC( (uint32)HandleVPFileDetailWinMsg ), 
    WIN_ID( MMIVP_FILE_DETAIL_WIN_ID ),
    WIN_STATUSBAR,
    WIN_TITLE(TXT_VP_FILEATTR),
    CREATE_RICHTEXT_CTRL(VP_FILE_DETAIL_TEXT_CTRL_ID),
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_RETURN),
    END_WIN
};
#endif
#ifdef MMI_STREAMING_PLAYER_SUPPORT
//网络文件属性
WINDOW_TABLE(MMIVP_VIEW_NET_DETAIL_WIN_TAB) =
{    
    WIN_FUNC( (uint32)HandleVPNetDetailWinMsg ), 
    WIN_ID( MMIVP_NET_DETAIL_WIN_ID ),
#ifndef MMI_ONLY_IDLE_DISP_STATUSBAR //MINI界面不需要statusbar
    WIN_STATUSBAR,
#endif
    WIN_TITLE(TXT_VP_FILEATTR),
    CREATE_RICHTEXT_CTRL(VP_NET_DETAIL_TEXT_CTRL_ID),
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_RETURN),
    END_WIN
};
#endif
#ifdef TOUCH_PANEL_SUPPORT //6530_MINI
//文件选择窗口
WINDOW_TABLE( MMIVP_FILE_OPTION_WIN_TAB ) = 
{
    WIN_FUNC((uint32)HandleFileOptionWinMsg),
    WIN_ID(MMIVP_FILE_OPTION_WIN_ID),
#ifndef MMI_ONLY_IDLE_DISP_STATUSBAR //6530_MINI界面不需要statusbar
    WIN_STATUSBAR,
#endif
    WIN_TITLE(STXT_OPTION),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_MENU_CTRL(MENU_VP_FILE_OPT, VP_MENU_FILE_OPT_CTRL_ID),
    END_WIN
};
#endif

#ifdef MMI_STREAMING_PLAYER_SUPPORT
#if defined(MMI_WIFI_SUPPORT) || !defined(MMI_MULTI_SIM_SYS_SINGLE) 
WINDOW_TABLE( MMIVP_SELECT_SIM_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleSIMSelectWinMsg ),    
    WIN_ID( MMIVP_SELECT_SIM_WIN_ID ),
#ifndef MMI_ONLY_IDLE_DISP_STATUSBAR //MINI界面不需要statusbar
    WIN_STATUSBAR,
#endif
    WIN_TITLE( TXT_SIM_SEL ),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E,VP_SELECT_SIM_LIST_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};
#endif
#endif

#ifndef MMI_GUI_STYLE_TYPICAL
//PDA风格UI
#ifdef MMI_PDA_SUPPORT
#ifdef MMI_STREAMING_PLAYER_SUPPORT
//书签列表 长按选项窗口
WINDOW_TABLE( MMIVP_BOOKMARK_LONGOK_POPMENU_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleBookmarkLongOkOptMenuWinMsg),
    WIN_ID(MMIVP_BOOKMARK_LONGOK_OPT_MENU_WIN_ID),
    WIN_STYLE( WS_HAS_TRANSPARENT),
    CREATE_POPMENU_CTRL(MENU_VP_BOOKMARK_LONGOK_OPT,VP_BOOKMARK_LONGOK_OPT_MENU_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),
    END_WIN
};
#endif
#ifdef MMI_VIDEOPLAYER_HISTORYLIST_SUPPORT
//历史记录列表 长按选项窗口
WINDOW_TABLE( MMIVP_HISTORY_LONGOK_POPMENU_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleHistoryLongOkOptMenuWinMsg),
    WIN_ID(MMIVP_HISTORY_LONGOK_OPT_MENU_WIN_ID),
    WIN_STYLE( WS_HAS_TRANSPARENT),
    CREATE_POPMENU_CTRL(MENU_VP_HISTORY_LONGOK_OPT,VP_HISTORY_LONGOK_OPT_MENU_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),
    END_WIN
};
#endif
//默认本地播放列表 长按选项窗口
WINDOW_TABLE( MMIVP_DEFAULT_LOCALLIST_LONGOK_POPMENU_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleLocalListLongOkOptMenuWinMsg),
    WIN_ID(MMIVP_DEFAULT_LOCALLIST_LONGOK_OPT_MENU_WIN_ID),
    WIN_STYLE( WS_HAS_TRANSPARENT),
    CREATE_POPMENU_CTRL(MENU_VP_DEFAULT_LOCALLIST_LONGOK_OPT,VP_DEFAULT_LOCALLIST_LONGOK_OPT_MENU_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),
    END_WIN
};

//用户自定义本地播放列表 长按选项窗口
WINDOW_TABLE( MMIVP_USER_LOCALLIST_LONGOK_POPMENU_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleLocalListLongOkOptMenuWinMsg),
    WIN_ID(MMIVP_USER_LOCALLIST_LONGOK_OPT_MENU_WIN_ID),
    WIN_STYLE( WS_HAS_TRANSPARENT),
    CREATE_POPMENU_CTRL(MENU_VP_USER_LOCALLIST_LONGOK_OPT,VP_USER_LOCALLIST_LONGOK_OPT_MENU_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),
    END_WIN
};
#endif //MMI_PDA_SUPPORT
//PDA style 设置窗口
LOCAL WINDOW_TABLE( MMIVP_PDA_SETTINGS_WIN_TAB ) = 
{
#ifndef MMI_ONLY_IDLE_DISP_STATUSBAR //6530_MINI without statusbar
    WIN_STATUSBAR,
#endif
    WIN_TITLE(STXT_MAIN_SETTING),
    WIN_FUNC((uint32)HandlePdaSettingsWinMsg),
    WIN_ID(MMIVP_PDA_SETTINGS_WIN_ID),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E,MMIVP_PDA_SETTINGS_LIST_CTRL_ID),
 //   WIN_STYLE(WS_HAS_TRANSPARENT),
    WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),
    END_WIN
};


#ifdef MMI_PDA_SUPPORT

LOCAL WINDOW_TABLE( MMIVP_SETTINGS_REPEAT_WIN_TAB ) = 
{
    WIN_STATUSBAR,
    WIN_TITLE( TXT_COMMON_REPEAT ),
    WIN_FUNC( (uint32)HandleVpRepeatSetWin),    
    WIN_ID( MMIVP_SETTINGS_REPEAT_WIN_ID ),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E,MMIVP_REPEAT_SET_LIST_CTRL_ID),
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_CANCEL),
    END_WIN
};

#endif //MMI_PDA_SUPPORT
#endif //MMI_GUI_STYLE_TYPICAL

LOCAL WINDOW_TABLE( MMIVP_SETTINGS_PLAYBACK_MODE_WIN_TAB ) = 
{
#ifndef MMI_ONLY_IDLE_DISP_STATUSBAR //6530_MINI界面不需要statusbar
    WIN_STATUSBAR,
#endif
    WIN_TITLE( TXT_VP_SET_PLAY_METHOD ),
    WIN_FUNC( (uint32)HandleVpPlaybackModeSetWin),    
    WIN_ID( MMIVP_SETTINGS_PLAYBACK_MODE_WIN_ID ),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E,MMIVP_PLAYBACK_MODE_SET_LIST_CTRL_ID),
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_CANCEL),
    END_WIN
};

/*---------------------------------------------------------------------------*/
/*                         FUNCTION DEFINITION                               */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : 提示出错信息
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void DisplayErrorCode(VP_ERROR_E error_code)
{
    MMI_TEXT_ID_T text_id = TXT_NULL;

    switch(error_code) 
    {
    case VP_ERROR_NOT_SUPPORT:
        text_id = TXT_COMMON_NO_SUPPORT;
        break;
    case VP_ERROR_INTERNAL_SERVER_ERROR:
        text_id = TXT_VP_INTERNAL_SERVER_ERROR;
        break;
    case VP_ERROR_BAD_REQUEST:
        text_id = TXT_COMM_BAD_REQUEST;
        break;
    case VP_ERROR_UNAUTHORIZED:
        text_id = TXT_UNAUTHORIZED;
        break;
    case VP_ERROR_NOT_FOUND:
        text_id = TXT_COM_FILE_NO_EXIST;
        break;
    
    case VP_ERROR_SERVER_CLOSE_IND:
        text_id = TXT_VP_SERVER_CLOSE_IND;
        break;
 
    case VP_ERROR_DATA_TIMEOUT:
        text_id = TXT_VP_DATA_TIMEOUT;
        break;

    case VP_ERROR_CONNECT_TIMEOUT:
        text_id = TXT_VP_CONNECT_TIMEOUT;
        break;

    case VP_ERROR_NULL_FILE:
        text_id = TXT_EMPTY_FILE;
        break;    

    case VP_ERROR_PDP_ACTIVE_ERROR:
        text_id = TXT_VP_PDP_ACTIVE_ERROR;
        break;

    case VP_ERROR_PDP_DEACTIVE_IND:
        text_id = TXT_VP_PDP_DEACTIVE_IND;
        break;

#ifdef DRM_SUPPORT
    case VP_ERROR_DRM_RIGHT_INVALID:
        text_id = TXT_DRM_COPYRIGHTS_INVLIAD_NOT_OPERATE;
        break;

    case VP_ERROR_DRM_NO_SYS_NITZ:
        text_id = TXT_DRM_NOT_SYS_NITZ;
        break;
#endif

    default:
        text_id = TXT_COMM_PLAY_ERROR;
        break;
    }
    DisplayVPWinStation(text_id,TRUE,FALSE);  
}

/*****************************************************************************/
//  Description : StartProcessTimer
//  Global resource dependence : 
//  Author: 
//  Note: 启动进度条显示timer
/*****************************************************************************/
LOCAL void StartProcessTimer(void)
{   
    //SCI_TRACE_LOW:"[VP] StartProcessTimer before s_process_timer_id = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_2091_112_2_18_3_5_32_407,(uint8*)"d",s_process_timer_id);

    if (0 != s_process_timer_id)
    {
        MMK_StopTimer(s_process_timer_id);
        s_process_timer_id = 0;
    } 
    
    if (VP_STATE_READY != s_video_player_control_info.state)
    {
        s_process_timer_id = MMK_CreateWinTimer(MMIVP_PLAYER_WIN_ID, VP_PROCESS_TIME_OUT, FALSE);
        MMK_StartWinTimer(MMIVP_PLAYER_WIN_ID, s_process_timer_id, VP_PROCESS_TIME_OUT, FALSE);  
    }
    
    //VP_STRACE_LOW:"[VP] StartProcessTimer s_process_timer_id = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_2105_112_2_18_3_5_32_408,(uint8*)"d",s_process_timer_id);
}

/*****************************************************************************/
//  Description : PauseProcessTimer
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void PauseProcessTimer(void)
{
    StopProcessTimer();   
}

/*****************************************************************************/
//  Description : ResumeProcessTimer
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void ResumeProcessTimer(void)
{
  
   HandleProcessTimer();   
}

/*****************************************************************************/
//  Description : StopProcessTimer
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void StopProcessTimer(void)
{   
    //SCI_TRACE_LOW:"[VP] StopProcessTimer s_process_timer_id = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_2133_112_2_18_3_5_32_409,(uint8*)"d",s_process_timer_id);

    if (0 != s_process_timer_id)
    {
        MMK_StopTimer(s_process_timer_id);
        s_process_timer_id = 0;
    }       
    
}

/*****************************************************************************/
//  Description : HandleProcessTimer
//  Global resource dependence : 
//  Author: 
//  Note: 进度条timer到时，更新进度条
/*****************************************************************************/
LOCAL void HandleProcessTimer(void)
{
    
    VP_PLAY_PROCESS_IND_T       process_ind   = {0};

    StopProcessTimer();       
    if (0 == s_video_player_control_info.seek_time)
    {            
        process_ind.process_time = MMIAPIVP_FSMGetTimeOffset(s_video_player_control_info.vp_handle); 
        s_video_player_control_info.process_time = process_ind.process_time;                     
    }
    DisplayVPWinProgress(TRUE);
    StartProcessTimer();
}

/*****************************************************************************/
//  Description : HandleFsmMsg
//  Global resource dependence : 
//  Author: 
//  Note: 处理状态机给ui发送的消息
/*****************************************************************************/
LOCAL void HandleFsmMsg(VP_CALLBACK_T *msg_ptr)
{   
    MMI_WIN_ID_T        win_id = MMIVP_PLAYER_WIN_ID;   
    MMI_TEXT_ID_T       text_id    = TXT_NULL;
    
   

    if (PNULL == msg_ptr || PNULL == msg_ptr->para)
    {
        //VP_STRACE_LOW:"[VP] HandleFsmMsg param is error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_2175_112_2_18_3_5_32_410,(uint8*)"");
        return;
    }
    //VP_STRACE_LOW:"[VP] HandleFsmMsg msg_ptr->msg_id = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_2178_112_2_18_3_5_32_411,(uint8*)"d",msg_ptr->msg_id);
    switch(msg_ptr->msg_id) 
    {
    case VP_PREPARE_IND:   //从媒体中获得显示信息
        {
            VP_PREPARE_IND_T prepare_info = *(VP_PREPARE_IND_T *)msg_ptr->para;                   

            if (prepare_info.vp_handler == s_video_player_control_info.vp_handle)
            {
                s_video_player_control_info.play_type = prepare_info.play_type;                
                s_video_player_control_info.full_path_len = (uint16)MIN(prepare_info.file_name_len,VP_MAX_NAME_LEN);
                SCI_MEMSET(s_video_player_control_info.full_path_name,0,((VP_MAX_NAME_LEN +1)*sizeof(wchar)));
                MMIAPICOM_Wstrncpy(s_video_player_control_info.full_path_name,
                    prepare_info.file_name_ptr,
                    s_video_player_control_info.full_path_len);               
                s_video_player_control_info.total_time = prepare_info.total_time;
                s_video_player_control_info.support_type = prepare_info.support_type; 
                if (VP_TO_DO_SEEK != s_video_player_control_info.to_do_type)
                {
                    s_video_player_control_info.process_time = prepare_info.start_time;
                }                
                //VP_STRACE_LOW:"[VP] HandleFsmMsg play_type = %d, total_time = %d,support_type = %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_2199_112_2_18_3_5_32_412,(uint8*)"ddd",prepare_info.play_type,s_video_player_control_info.total_time, s_video_player_control_info.support_type);
                                
                MMIDEFAULT_TurnOnBackLight();
                MMIDEFAULT_AllowTurnOffBackLight(FALSE); 
                if (VP_PLAY_FROM_NET == prepare_info.play_type)   //网络播放显示初始化，本地播放根据当前状态显示
                {                      
                    DisplayVPWinStation(TXT_VP_READY,TRUE,FALSE);                     
                    if (MMIAPIVP_IsLiving()) //直播
                    {     
                        DisplayVPWinSoftkey(TRUE);   //直播不显示选项，灰化文件列表icon
                        DisplayVPWinFileIcon(TRUE); 
                    }
                }
                else
                {
                    DisplayVPWinStation(TXT_NULL,TRUE,TRUE);
                }
                DisplayVPWinProgress(TRUE);      //更新进度条显示             
                DisplayVPWinTitle(TRUE);    //更新窗口标题
            } 
        }
        break;   

    case VP_BUFFERING_IND: //开始缓冲
        {
            VP_BUFFERING_IND_T buffering_ind = *(VP_BUFFERING_IND_T *)msg_ptr->para;
            if (buffering_ind.vp_handler == s_video_player_control_info.vp_handle)
            {               
                //VP_STRACE_LOW:"[VP] HandleFsmMsg buffering s_video_player_control_info.state = %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_2227_112_2_18_3_5_32_413,(uint8*)"d",s_video_player_control_info.state);
                s_video_player_control_info.state = VP_STATE_BUFFERING;      
                s_video_player_control_info.buffer_percent = 0;
                DisplayVPWinStation(TXT_VP_BUFFRING,TRUE,FALSE);       //显示缓冲中                
                if (IsNeedDisplayControlPanel()) //全屏进入缓冲状态，显示控制面板
                {                    
                    DisappearVPWin(win_id,FALSE,TRUE);
                }                
            }
        }       
        break;

    case VP_BUFFER_PROCESS_IND:
        {
            VP_BUFFERING_PROCESS_IND_T buffering_ind = *(VP_BUFFERING_PROCESS_IND_T *)msg_ptr->para;
            if (buffering_ind.vp_handler == s_video_player_control_info.vp_handle)
            {                
                   s_video_player_control_info.buffer_percent = buffering_ind.buffer_percent;   
                   DisplayVPWinStation(TXT_VP_BUFFRING,TRUE,FALSE);       //显示缓冲中      
            }
        } 
        break;
        
    case VP_BUFFER_END_IND: //缓冲完成
        {
            VP_BUFFER_END_IND_T buffer_end_ind = *(VP_BUFFER_END_IND_T *)msg_ptr->para;
            if (buffer_end_ind.vp_handler == s_video_player_control_info.vp_handle)
            {           
                //VP_STRACE_LOW:"[VP] HandleFsmMsg buffer end s_video_player_control_info.state = %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_2255_112_2_18_3_5_32_414,(uint8*)"d",s_video_player_control_info.state);
                s_video_player_control_info.state = VP_STATE_PLAYING;         
                s_video_player_control_info.buffer_percent = 100;
                DisplayVPWinStation(TXT_VP_BUFFRING,TRUE,FALSE); 
                if (VP_SUPPORTED_AUDIO_ONLY == s_video_player_control_info.support_type)
                {
#ifdef MMI_VIDEO_PLAYER_MINI_LAYOUT//6530 MINI 界面布局
                    DisplayVPWinStation(TXT_VP_VIDIO_NOT_SUPPORT_MINI,TRUE,FALSE);  //视频图像不支持
#else
                    DisplayVPWinStation(TXT_VP_VIDIO_NOT_SUPPORT,TRUE,FALSE);  //显示播放中
#endif
                }
                else 
                {
                    DisplayVPWinStation(TXT_VP_PLAYING,TRUE,FALSE);  //显示播放中  
                    StartVPDisplayStationTimer();
                }                

                //缓冲结束进入播放状态，全屏播放状态下启动控制面板消失timer                
                StartVPWinDisplayerTimer();   
                
            }
        }        
        break;

         
    case VP_PLAY_CNF: //开始播放
        {
            VP_PLAY_CNF_T play_result = *(VP_PLAY_CNF_T *)msg_ptr->para;          
            if (play_result.vp_handler == s_video_player_control_info.vp_handle)
            {
                //VP_STRACE_LOW:"[VP] HandleFsmMsg play_result.error_code = %d, s_video_player_control_info.to_do_type = %d, s_video_player_control_info.process_time = %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_2285_112_2_18_3_5_32_415,(uint8*)"ddd",play_result.error_code,s_video_player_control_info.to_do_type,s_video_player_control_info.process_time);
                

                if(VP_ERROR_NONE == play_result.error_code) //开始播放成功
                {  
                    //VP_STRACE_LOW:"[VP] HandleFsmMsg play cnf s_video_player_control_info.state = %d"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_2290_112_2_18_3_5_32_416,(uint8*)"d",s_video_player_control_info.state);
                                            
                    s_video_player_control_info.state = VP_STATE_PLAYING;
                    if (MMITHEME_IsMainScreenLandscape()
                        && s_video_player_control_info.is_display_win)  //必须重新显示一次，因为开始播放的时候回全屏刷新一次
                    {
                        DisappearVPWin(win_id,FALSE,TRUE);
                        StartVPWinDisplayerTimer(); //全屏播放，进入播放状态，启动控制面板消失timer    
                    }
                    DisplayVPWinSoftkey(TRUE);
                    StartProcessTimer();  //启动进度条更新timer                    
                    DisplayVPWinBackwardOrForwardIcon(TRUE); //本地文件和网络播放快进快退button不灰化
                    DisplayVPWinPlayOrPauseIcon(TRUE);    //显示暂停button      
                    DisplayVPWinSwitchScreenIcon(TRUE);
#ifdef CMCC_TEST_FLAG
                    MMIDEFAULT_AllowTurnOffBackLight(FALSE);
#endif

                    if (VP_SUPPORTED_AUDIO_ONLY == s_video_player_control_info.support_type)
                    {
#ifdef MMI_VIDEO_PLAYER_MINI_LAYOUT//6530 MINI 界面布局
                        DisplayVPWinStation(TXT_VP_VIDIO_NOT_SUPPORT_MINI,TRUE,FALSE);  //视频图像不支持
#else
                        DisplayVPWinStation(TXT_VP_VIDIO_NOT_SUPPORT,TRUE,FALSE);  //显示播放中
#endif                    
                    }
                    else 
                    {
                        DisplayVPWinStation(TXT_VP_PLAYING,TRUE,FALSE);  //显示播放中  
                        StartVPDisplayStationTimer();
                    }
                    
                    //VP_STRACE_LOW:"[VP] HandleFsmMsg s_tp_down_ctrl_id = %d, s_key_down_msg_id = %d"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_2314_112_2_18_3_5_32_417,(uint8*)"dd",s_tp_down_ctrl_id,s_key_down_msg_id);
                    if (VP_TO_DO_SEEK == s_video_player_control_info.to_do_type) //从某一断点处开始播放
                    {
                        s_video_player_control_info.to_do_type = VP_TO_DO_NONE;
                        MMK_PostMsg(win_id,MSG_VP_SEEK,PNULL,PNULL);                                
                    } 
                    else if(0 != s_tp_down_ctrl_id)
                    {
                        MMI_MESSAGE_ID_E msg_id = 0;
                        MMI_MESSAGE_ID_E down_msg_id = 0;                        
                        switch(s_tp_down_ctrl_id) 
                        {
                        case VP_PLAY_PANEL_PRE_BUTTON_CTRL_ID:
                            msg_id = MSG_KEYLONG_LEFT;   
                            down_msg_id = MSG_KEYDOWN_LEFT;
    	                    break;

                        case VP_PLAY_PANEL_NEXT_BUTTON_CTRL_ID:
                            msg_id = MSG_KEYLONG_RIGHT;     
                            down_msg_id = MSG_KEYDOWN_RIGHT;
                            break;

                        default:        
                            break;
                        }

                        if (0 != msg_id)
                        {                            
                            MMK_PostMsg(win_id,down_msg_id,PNULL,PNULL);
                            MMK_PostMsg(win_id,msg_id,PNULL,PNULL);
                        }

                    }
                    else if (0 != s_key_down_msg_id)
                    {
                        MMK_PostMsg(win_id,s_key_down_msg_id,PNULL,PNULL);
                        //@CR241032 2011.05.26                      
                        StopVPWinDisplayerTimer();  
                        if (IsNeedDisplayControlPanel())
                        {
                            DisappearVPWin(MMIVP_PLAYER_WIN_ID,FALSE,TRUE);            
                        }  
                        //@CR241032 2011.05.26
                    }
                }
                else
                {   
                    //VP_STRACE_LOW:"[VP] HandleFsmMsg play cnf error s_video_player_control_info.state = %d"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_2361_112_2_18_3_5_32_418,(uint8*)"d",s_video_player_control_info.state);
#ifdef MMI_VIDEOPLAYER_HISTORYLIST_SUPPORT
					if (IsSaveLastPlayPoint()) //add for newms111640
                    {                        
						VP_HistoryList_SetLastPlayPoint(s_video_player_control_info.process_time);            
                    }
#endif //MMI_VIDEOPLAYER_HISTORYLIST_SUPPORT
                    s_video_player_control_info.state = VP_STATE_READY; //播放失败，进入ready状态   

                   // if (MMITHEME_IsMainScreenLandscape()
                   //     && s_video_player_control_info.is_display_win)  //必须重新显示一次，因为开始播放的时候回全屏刷新一次
                   // {
                   //     StopVPWinDisplayerTimer();
                   //     DisappearVPWin(win_id,FALSE,TRUE);
                  //  }

                    DisplayVPWinSwitchScreenIcon(TRUE);
                    
                    if (MMK_IsFocusWin(MMIVP_PLAYER_WIN_ID)  //显示默认图片
                      && s_is_full_paint)
                    {                                  
                       
                        DisplayVpWinDefaultPic();
                    }

                                     
                    DisplayErrorCode(play_result.error_code);  //显示错误信息
                }                         
            }            
            
        }        
        break;

    case VP_PAUSE_CNF://暂停
        {
            VP_PAUSE_CNF_T pause_result = *(VP_PAUSE_CNF_T *)msg_ptr->para;
            if (pause_result.vp_handler == s_video_player_control_info.vp_handle)
            {
                //VP_STRACE_LOW:"[VP] HandleFsmMsg pause_result.error_code = %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_2399_112_2_18_3_5_33_419,(uint8*)"d",pause_result.error_code);
                if (VP_ERROR_NONE == pause_result.error_code)  //暂停成功
                {    
                    //VP_STRACE_LOW:"[VP] HandleFsmMsg pause cnf s_video_player_control_info.state = %d"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_2402_112_2_18_3_5_33_420,(uint8*)"d",s_video_player_control_info.state);

                    if (VP_STATE_PAUSED == s_video_player_control_info.state
                        || VP_STATE_PAUSING == s_video_player_control_info.state)
                    {                        
                        s_video_player_control_info.state = VP_STATE_PAUSED;  //进入暂停状态，显示播放图标，显示暂停成功提示
                        DisplayVPWinSoftkey(TRUE);
                        DisplayVPWinPlayOrPauseIcon(TRUE);   
                        DisplayVPWinStation(TXT_VP_PAUSED,TRUE,FALSE);
                        
#ifdef CMCC_TEST_FLAG
                        MMIDEFAULT_AllowTurnOffBackLight(TRUE);
#endif
                    }                                 
                    
                }
            }
            
        }
        break;    

    case VP_SEEK_CNF: //seek
        {
            VP_SEEK_CNF_T seek_result = *(VP_SEEK_CNF_T *)msg_ptr->para;
            if (seek_result.vp_handler == s_video_player_control_info.vp_handle) 
            {
                //VP_STRACE_LOW:"[VP] HandleFsmMsg seek_result.error_code = %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_2424_112_2_18_3_5_33_421,(uint8*)"d",seek_result.error_code);
                s_video_player_control_info.state = VP_STATE_PLAYING; //seek成功，进入播放状态              
                DisplayVPWinBackwardOrForwardIcon(TRUE);              //网络播放时快进快退不灰化                               
                DisplayVPWinPlayOrPauseIcon(TRUE);                    //显示播放图标
                DisplayVPWinSoftkey(TRUE);
                //s_video_player_control_info.seek_time = 0;
                if (VP_SUPPORTED_AUDIO_ONLY == s_video_player_control_info.support_type)
                {
#ifdef MMI_VIDEO_PLAYER_MINI_LAYOUT//6530 MINI 界面布局
                    DisplayVPWinStation(TXT_VP_VIDIO_NOT_SUPPORT_MINI,TRUE,FALSE);  //视频图像不支持
#else
                    DisplayVPWinStation(TXT_VP_VIDIO_NOT_SUPPORT,TRUE,FALSE);  //显示播放中
#endif                    
                }
                else 
                {
                    DisplayVPWinStation(TXT_VP_PLAYING,TRUE,FALSE);  //显示播放中  
                    StartVPDisplayStationTimer(); 
                }
                DisplayVPWinProgress(TRUE);
                StartVPWinDisplayerTimer();                           //全屏播放时，启动控制面板消失的timer
                
            }            
        }
        break;

    case VP_FATAL_ERROR_IND:
        {
            VP_FATAL_ERROR_IND_T fatal_error = *(VP_FATAL_ERROR_IND_T *)msg_ptr->para;           

            if (fatal_error.vp_handler == s_video_player_control_info.vp_handle)
            {
                //VP_STRACE_LOW:"[VP] HandleFsmMsg fatal_error.error_code = %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_2452_112_2_18_3_5_33_422,(uint8*)"d",fatal_error.error_code);
#ifdef MMI_VIDEOPLAYER_HISTORYLIST_SUPPORT
                if (IsSaveLastPlayPoint()) //add for newms111640
                {                        
					VP_HistoryList_SetLastPlayPoint(s_video_player_control_info.process_time);            
                }
#endif //MMI_VIDEOPLAYER_HISTORYLIST_SUPPORT
                s_video_player_control_info.state = VP_STATE_READY;  //发生异常错误，提示出错类型，禁止弹出授权窗口
#ifdef MMI_STREAMING_PLAYER_SUPPORT
                s_is_need_auth = FALSE;
#endif
                if (MMK_IsFocusWin(MMIVP_PLAYER_WIN_ID)  //显示默认图片
                      && s_is_full_paint)
                {                                  
                   
                    DisplayVpWinDefaultPic();
                }
				
                //显示错误信息必须放在最后，否则不能显示出错误信息。for cr241938
                DisplayErrorCode(fatal_error.error_code);  //显示错误信息
            }           
           
        }
        break;

    case VP_AUTH_IND:
        {
            VP_AUTH_IND_T auth_ind = *(VP_AUTH_IND_T *)msg_ptr->para;
            if (auth_ind.vp_handler == s_video_player_control_info.vp_handle)
            {
                MMK_PostMsg(win_id,MSG_VP_AUTH_IND,PNULL,PNULL);  //需要授权
            }
        }
        
        break;

    case VP_PLAY_END_IND://播放结束，循环播放等，每首播放结束都可以收到该消息
        {
            VP_PLAY_END_IND_T play_end_ind = *(VP_PLAY_END_IND_T *)msg_ptr->para;
           
     

            if (play_end_ind.vp_handler == s_video_player_control_info.vp_handle)
            {
                //VP_STRACE_LOW:"[VP] HandleFsmMsg play_end_ind.result = %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_2496_112_2_18_3_5_33_423,(uint8*)"d",play_end_ind.result);
#ifdef MMI_STREAMING_PLAYER_SUPPORT				
                s_is_need_auth = FALSE;                  //关闭并禁止弹出授权窗口
                MMK_CloseWin(MMIVP_AUTH_WIN_ID); 
#endif
                switch(play_end_ind.result) 
                {
                case VP_PLAY_END_END_SUCCESS:
                    text_id = TXT_VP_PLAY_END;
                    break;
                    
                case VP_PLAY_END_ERROR:
                    text_id = TXT_COMM_PLAY_ERROR;
                    break;
                case VP_PLAY_END_STOPPED_BY_USER:
                    text_id = TXT_VP_STOPPED_BY_USER;
                    break;
                case VP_PLAY_END_NOT_SUPPORT:
                    text_id = TXT_COMMON_NO_SUPPORT;
                    break;
                default:
                    text_id = TXT_VP_PLAY_END;
                    break;
                } 
                StopProcessTimer();                  //停止进度条更新显示timer
                
                if (!MMIAPIVP_IsLiving()) //非直播情况下，更新进度条到最大
                {             
                    s_video_player_control_info.process_time = s_video_player_control_info.total_time;  
                    DisplayVPWinProgress(TRUE);                    
                }  
                
                if (MMK_IsFocusWin(MMIVP_PLAYER_WIN_ID)  //显示默认图片
                      && s_is_full_paint)
                {                                  
                       
                    DisplayVpWinDefaultPic();
                    if (MMITHEME_IsMainScreenLandscape()) //全屏播放显示控制面板
                    {   
                        StopVPWinDisplayerTimer();
                        DisappearVPWin(win_id,FALSE,TRUE);  
                    }           
                    if (TXT_NULL != text_id)
                    {
                        DisplayVPWinStation(text_id,TRUE,FALSE);  
                        if (TXT_VP_PLAY_END != text_id)
                        {
                            s_video_player_control_info.state = VP_STATE_READY;
                        } 
                    }
                                      
                }
                
            }
			else
			{
				SCI_TRACE_LOW("[VP] HandleFsmMsg error!! vp_handler != vp_handle");
			}
        }        
        break;

    case VP_STOP_IND: //全部停止播放
        {
            VP_STOP_IND_T stop_ind = *(VP_STOP_IND_T *)msg_ptr->para;      
            
            
            if (MMICOM_PANEL_BRIGHTNESS == MMIAPICOM_GetPanelType())
            {
                MMIAPICOM_ClosePanelChildWin();
            } 
            
            if (stop_ind.vp_handler == s_video_player_control_info.vp_handle && MMK_IsOpenWin(win_id))
            {   
                //VP_STRACE_LOW:"[VP] HandleFsmMsg stop cnf s_video_player_control_info.state = %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_2565_112_2_18_3_5_33_424,(uint8*)"d",s_video_player_control_info.state);
                if (VP_STATE_READY != s_video_player_control_info.state)  //如果是正常结束，则提示停止，否则显示停止出错提示信息
                {
                    s_video_player_control_info.state = VP_STATE_READY;
                    text_id = STXT_STOP;
                }

                if (MMITHEME_IsMainScreenLandscape()) //全屏播放显示控制面板
                {
                    StopVPWinDisplayerTimer();
                    DisappearVPWin(win_id,FALSE,TRUE);   
                }
#ifdef  VIDEOTHUMBNAIL_SUPPORT     

                if(IsAbleToGetThumbnail())   
                {
                    NotifyDPLAYERGetTumbnail(FALSE);
                }

#endif                
                DisplayVpWinDefaultPic(); 
                if (TXT_NULL != text_id
					&& VP_TO_DO_NONE == s_video_player_control_info.to_do_type
					&& VP_PLAY_FROM_NET == s_video_player_control_info.play_type)
                {
                    DisplayVPWinStation(text_id,TRUE,FALSE); 
                }    
                else
                {
                    DisplayVPWinStation(text_id,TRUE,TRUE); 
                }
                StopProcessTimer();            // 停止进度条timer；  
                s_video_player_control_info.process_time = 0;       //当前播放时间归零           
                

                //VP_STRACE_LOW:"[VP] VP_STOP_IND s_video_player_control_info.to_do_type = %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_2600_112_2_18_3_5_33_425,(uint8*)"d",s_video_player_control_info.to_do_type);

                if (VP_TO_DO_PLAY_FOR_STOP == s_video_player_control_info.to_do_type) //已经准备好下一首开时播放
                {    
                    
                    if (MMK_IsFocusWin(win_id))  //如果当前窗口已经处于focus状态，则直接通知播放，否则，等待窗口focus后再继续播放
                    {
                        MMK_PostMsg(win_id,MSG_VP_BEGIN_PLAY,PNULL,PNULL);   
                        s_video_player_control_info.to_do_type = VP_TO_DO_NONE;
                    }
                    else
                    {
                        s_video_player_control_info.to_do_type = VP_TO_DO_PLAY_FOR_FULL_PAINT;
                    }                    
                    
                }
                else if(VP_TO_DO_PLAY_FOR_FULL_PAINT == s_video_player_control_info.to_do_type)
                {
                    
                }
                else  //彻底结束，不再播放
                {
                    MMIDEFAULT_AllowTurnOffBackLight(TRUE); 
                    s_video_player_control_info.to_do_type = VP_TO_DO_NONE;
                    s_video_player_control_info.support_type = VP_SUPPORTED_BOTH;                        
                    s_video_player_control_info.seek_time = 0;   
                    StopPlayerAllTimer();   //停止所有timer   
#ifdef MMI_STREAMING_PLAYER_SUPPORT
                    s_is_need_auth = FALSE;   //关闭并禁止授权窗口
                    MMK_CloseWin(MMIVP_AUTH_WIN_ID);  
#endif
                    DisplayVPWinFileIcon(TRUE);     //更新文件列表显示icon                                                                                        
                    //VP_STRACE_LOW:"[VP] VP_STOP_IND full_path_len = %d"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_2633_112_2_18_3_5_33_426,(uint8*)"d",s_video_player_control_info.full_path_len);
                    if (0 == s_video_player_control_info.full_path_len 
                        && 0 == strlen((char *)(s_video_player_control_info.url))) //清除了当前播放信息的，需要重新设置
                    {         
                        s_video_player_control_info.state = VP_STATE_READY;
                        s_video_player_control_info.play_type = 0;   
                        s_video_player_control_info.total_time = 0; 
                        s_video_player_control_info.process_time = 0;                       
                        VP_LocalList_SetCurListType(VP_LOCALLIST_MAX);
                        VP_LocalList_SetCurrentIndex(-1);
                        DisplayVPWinTitle(TRUE);
                        text_id = TXT_VP_NO_FILE;
                    }  
                    DisplayVPWinPlayOrPauseIcon(TRUE);  //根据状态更新播放/暂停图标
                    DisplayVPWinBackwardOrForwardIcon(TRUE);   //本地文件和网络播放需要灰化快进快退                    
                    DisplayVPWinProgress(TRUE);              //更新进度条显示，当前播放时间为0
                    if (TXT_NULL != text_id)
                    {
                        DisplayVPWinStation(text_id,TRUE,FALSE);    
                    } 
                    else
                    {
                        DisplayVPWinStation(text_id,TRUE,TRUE); 
                    }
                    DisplayVPWinSoftkey(TRUE);               //更新softkey显示，停止变退出     
                    DisplayVPWinSwitchScreenIcon(TRUE);
                }
            }
            
        }
        break;

    default:
        break;
    }
    
}

/*****************************************************************************/
//  Description : SaveHistory
//  Global resource dependence : 
//  Author: 
//  Note:保存历史记录
/*****************************************************************************/
LOCAL BOOLEAN SaveHistory(wchar *full_path_name_ptr,
                          uint16 full_path_name_len,
                          uint8 *url_ptr,
                          uint32 process_time,
                          VP_PLAY_TYPE_E play_type,               
                          uint32 total_time)
{
#ifdef MMI_VIDEOPLAYER_HISTORYLIST_SUPPORT
    VP_HISTORY_ITEM_T   history_list_item = {0};
    SFS_DATE_T          modify_date = {0};
    SFS_TIME_T          modify_time = {0};
    DPLAYER_MEDIA_INFO_T media_info = {0};
    VP_LOCALLIST_TYPE_E locallist_type = VP_LocalList_GetCurListType();
    int32               locallist_index = VP_LocalList_GetCurrentIndex(locallist_type);

    //VP_STRACE_LOW:"[VP] SaveHistory locallist_type = %d, locallist_index = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_2701_112_2_18_3_5_33_427,(uint8*)"dd",locallist_type,locallist_index);
    if (VP_PLAY_FROM_NET ==  play_type)
    {
        history_list_item.local_movie_file_info_ptr = PNULL;
        history_list_item.net_movie_info_ptr = (VP_NET_MOVIE_INFO_T*)SCI_ALLOCA(sizeof(VP_NET_MOVIE_INFO_T));
        if (PNULL != history_list_item.net_movie_info_ptr)
        {
            SCI_MEMSET(history_list_item.net_movie_info_ptr, 0, sizeof(VP_NET_MOVIE_INFO_T));
            SCI_MEMSET(history_list_item.net_movie_info_ptr->url_ptr,0,(VP_MAX_URL_LEN+1));
            SCI_MEMCPY((char *)(history_list_item.net_movie_info_ptr->url_ptr),(char *)url_ptr,(MIN(VP_MAX_URL_LEN,strlen((char *)url_ptr)))); /*lint !e666*/
            SCI_MEMSET(history_list_item.net_movie_info_ptr->title_arr,0,((VP_MAX_NAME_LEN+1)*sizeof(wchar)));
            if (0 != s_video_player_control_info.full_path_len)
            {                
                MMIAPICOM_Wstrncpy(history_list_item.net_movie_info_ptr->title_arr,s_video_player_control_info.full_path_name,s_video_player_control_info.full_path_len);
                history_list_item.net_movie_info_ptr->title_len = s_video_player_control_info.full_path_len;
            }           
            
        }
    }
    else 
    {
        history_list_item.net_movie_info_ptr = PNULL;
        history_list_item.local_movie_file_info_ptr = (VP_LOCAL_MOVIE_FILE_INFO_T*)SCI_ALLOCA(sizeof(VP_LOCAL_MOVIE_FILE_INFO_T));
        if (PNULL != history_list_item.local_movie_file_info_ptr)
        {
            SCI_MEMSET(history_list_item.local_movie_file_info_ptr,0,sizeof(VP_LOCAL_MOVIE_FILE_INFO_T));
            history_list_item.local_movie_file_info_ptr->full_path_len = (uint16)MIN(full_path_name_len,VP_MAX_NAME_LEN);
            SCI_MEMSET(history_list_item.local_movie_file_info_ptr->full_path_ptr,0,((VP_MAX_NAME_LEN +1)*sizeof(wchar)));
            MMIAPICOM_Wstrncpy(history_list_item.local_movie_file_info_ptr->full_path_ptr,
                full_path_name_ptr,
                history_list_item.local_movie_file_info_ptr->full_path_len);
            MMIAPIFMM_GetFileInfo(full_path_name_ptr,full_path_name_len,&(history_list_item.local_movie_file_info_ptr->file_size),&modify_date,&modify_time);
            history_list_item.local_movie_file_info_ptr->create_time = MMIAPICOM_TansferSFSTime(modify_date,modify_time);
            VP_GetMediaInfo(s_video_player_control_info.vp_handle,&media_info);
            history_list_item.local_movie_file_info_ptr->height = media_info.video_info.height;
            history_list_item.local_movie_file_info_ptr->width = media_info.video_info.width;
            history_list_item.local_movie_file_info_ptr->media_length = media_info.media_length;
        }
    }
    
    history_list_item.last_play_point = 0;
    history_list_item.type = play_type;
    history_list_item.next_item_ptr = PNULL;
    history_list_item.locallist_type = locallist_type;
    history_list_item.locallist_index = locallist_index;
    history_list_item.last_play_point = process_time;    
    history_list_item.total_time = total_time;    
    VP_HistoryList_AddItem(history_list_item);
    if (PNULL != history_list_item.local_movie_file_info_ptr)
    {
        SCI_FREE(history_list_item.local_movie_file_info_ptr);
    }
    if (PNULL != history_list_item.net_movie_info_ptr)
    {
        SCI_FREE(history_list_item.net_movie_info_ptr);
    }
#endif //MMI_VIDEOPLAYER_HISTORYLIST_SUPPORT
    return TRUE;

}

/*****************************************************************************/
//  Description : NotifyPlayerForPlayFile
//  Global resource dependence : 
//  Author: 
//  Note:   通知播放器开始播放本地文件或者本地播放列表中的文件
/*****************************************************************************/
LOCAL BOOLEAN NotifyPlayerForPlayFile(wchar *full_path_name_ptr,
                          uint16 full_path_name_len,                       
                          uint32 process_time,
                          uint32 total_time,
                          VP_PLAY_TYPE_E play_type)
{
    
    
    //VP_STRACE_LOW:"[VP] NotifyPlayerForPlayFile full_path_name_ptr = %x, process_time = %d, play_type = %d, full_path_name_len = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_2774_112_2_18_3_5_33_428,(uint8*)"dddd",full_path_name_ptr,process_time,play_type, full_path_name_len);
    if (PNULL == full_path_name_ptr)
    {
        return FALSE;
    }

    if (VP_PLAY_FROM_FILE == play_type)
    {    
        VP_LocalList_SetCurListType(VP_LOCALLIST_MAX);
        VP_LocalList_SetCurrentIndex(-1);
    }

    SCI_MEMSET(s_video_player_control_info.full_path_name,0,((VP_MAX_NAME_LEN +1)*sizeof(wchar)));
    SCI_MEMSET(s_video_player_control_info.url,0,(VP_MAX_URL_LEN +1));
    s_video_player_control_info.full_path_len = (uint16)MIN(full_path_name_len,VP_MAX_NAME_LEN); 
    MMIAPICOM_Wstrncpy(s_video_player_control_info.full_path_name,
        full_path_name_ptr,
        s_video_player_control_info.full_path_len);      
    s_video_player_control_info.play_type = play_type;       
    s_video_player_control_info.process_time = process_time;   
    s_video_player_control_info.seek_time = process_time;
    s_video_player_control_info.total_time = total_time; 
    s_video_player_control_info.station_text_id = TXT_NULL;
    #ifdef KING_MOVIE_SUPPORT
    s_video_player_control_info.is_kmv_file = MMIAPICOM_IsKingMovieType(s_video_player_control_info.full_path_name,s_video_player_control_info.full_path_len);
    #endif
    SaveHistory(full_path_name_ptr,full_path_name_len,PNULL,process_time,play_type,total_time); //保存历史记录
    if (MMK_IsFocusWin(MMIVP_PLAYER_WIN_ID)
        && s_is_full_paint)    //通知播放器播放
    {    
        MMK_PostMsg(MMIVP_PLAYER_WIN_ID,MSG_VP_BEGIN_PLAY,PNULL,PNULL);
    }
    else
    {
        s_video_player_control_info.to_do_type = VP_TO_DO_PLAY_FOR_FULL_PAINT;
    }
    return TRUE;
}
#ifdef MMI_STREAMING_PLAYER_SUPPORT
/*****************************************************************************/
//  Description : PlayFromNet
//  Global resource dependence : 
//  Author: 
//  Note: 播放器直接播放当前网络文件
/*****************************************************************************/
LOCAL BOOLEAN PlayFromNet(
                          uint32 process_time,
                          uint32 total_time
                          )
{
    MMI_STRING_T undefine_string = {0};

    //VP_STRACE_LOW:"[VP] PlayFromNet process_time = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_2820_112_2_18_3_5_33_429,(uint8*)"d",process_time);
    s_video_player_control_info.process_time = process_time;   
    s_video_player_control_info.total_time = total_time;
    
    //@CR242015 2011.05.18 begin, when playing from start point, s_video_player_control_info.seek_time also need being setting zero;
    if ((0 == s_video_player_control_info.process_time)
        && (0 != s_video_player_control_info.seek_time))
    {
        s_video_player_control_info.seek_time = 0;
    }
    //@CR242015 2011.05.18 end, when playing from start point, s_video_player_control_info.seek_time also need being setting zero;

    if (0 == s_video_player_control_info.full_path_len)  //没有文件名，显示未命名
    {
        MMI_GetLabelTextByLang(TXT_VP_UNDEFINE,&undefine_string);
        s_video_player_control_info.full_path_len = (uint16)MIN(VP_MAX_NAME_LEN,undefine_string.wstr_len);
        MMIAPICOM_Wstrncpy(s_video_player_control_info.full_path_name,
            undefine_string.wstr_ptr,
            s_video_player_control_info.full_path_len);
    }

    SaveHistory(s_video_player_control_info.full_path_name,  //保存历史记录
        s_video_player_control_info.full_path_len,
        s_video_player_control_info.url,
        process_time,
        VP_PLAY_FROM_NET,       
        total_time);

    if (VP_PLAY_FROM_NET == s_video_player_control_info.play_type)
    {
#ifdef BROWSER_SUPPORT
    if(!MMIAPIBROWSER_IsRunning())
#endif
    {
        if (!GetSimCardForPlay(&(s_video_player_control_info.dual_sys)))
        {
            return TRUE;
        }   
    }
    }

    if (MMK_IsFocusWin(MMIVP_PLAYER_WIN_ID)
        && s_is_full_paint)  //通知播放器播放网络文件
    {    
        MMK_PostMsg(MMIVP_PLAYER_WIN_ID,MSG_VP_BEGIN_PLAY,PNULL,PNULL);
    }
    else
    {
        s_video_player_control_info.to_do_type = VP_TO_DO_PLAY_FOR_FULL_PAINT;
    }
    return TRUE;
}

/*****************************************************************************/
//  Description : NotifyPlayerForPlayNet
//  Global resource dependence : 
//  Author: 
//  Note: 通知播放器播放网络文件
/*****************************************************************************/
LOCAL BOOLEAN NotifyPlayerForPlayNet(wchar *full_path_name_ptr,
                          uint16 full_path_name_len,
                          uint8 *url_ptr,
                          uint32 process_time,
                          uint32 total_time
                          )
{
    MMI_STRING_T undefine_string = {0};
#ifndef MMI_WIFI_SUPPORT
    uint16 sim_sys  = MN_DUAL_SYS_1;
#endif
    //VP_STRACE_LOW:"[VP] NotifyPlayerForPlayNet full_path_name_ptr = %x, url_ptr=%x, process_time = %d, full_path_name_len = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_2887_112_2_18_3_5_34_430,(uint8*)"dddd",full_path_name_ptr,url_ptr,process_time,full_path_name_len);
    if (PNULL == url_ptr
        || (0 != strnicmp((char*)url_ptr,(char*)HTTP_URL_HEAD_ASIC,(int)HTTP_URL_HEAD_LEN)
        && 0 != strnicmp((char*)url_ptr,(char*)RTSP_URL_HEAD_ASIC,(int)RTSP_URL_HEAD_LEN))
        )

    {
        return FALSE;
    }
    if (MMIAPISET_GetFlyMode()
        #ifndef MMI_WIFI_SUPPORT
        || 0 == MMIAPIPHONE_GetSimAvailableNum(&sim_sys, 1)
        #endif
		)
    {
        MMIAPIPHONE_AlertSimNorOKStatus();      
        s_video_player_control_info.to_do_type = VP_TO_DO_STOP;
        return TRUE;
    }

    VP_LocalList_SetCurListType(VP_LOCALLIST_MAX);
    VP_LocalList_SetCurrentIndex(-1);
    SCI_MEMSET(s_video_player_control_info.full_path_name,0,((VP_MAX_NAME_LEN +1)*sizeof(wchar)));
    SCI_MEMSET(s_video_player_control_info.url,0,(VP_MAX_URL_LEN +1));
    s_video_player_control_info.full_path_len = (uint16)MIN(full_path_name_len,VP_MAX_NAME_LEN);

    if (0 != full_path_name_len && PNULL != full_path_name_ptr)  //没有文件名，显示未命名
    {
        MMIAPICOM_Wstrncpy(s_video_player_control_info.full_path_name,
            full_path_name_ptr,
            s_video_player_control_info.full_path_len);        
    }
    else 
    {
        MMI_GetLabelTextByLang(TXT_VP_UNDEFINE,&undefine_string);
        s_video_player_control_info.full_path_len = MIN(VP_MAX_NAME_LEN,undefine_string.wstr_len);
        MMIAPICOM_Wstrncpy(s_video_player_control_info.full_path_name,
            undefine_string.wstr_ptr,
            s_video_player_control_info.full_path_len);
    }
    
    s_video_player_control_info.play_type = VP_PLAY_FROM_NET;
    SCI_MEMCPY((char *)(s_video_player_control_info.url),(char *)url_ptr,(MIN(VP_MAX_URL_LEN,(strlen((char *)url_ptr)))));  /*lint !e666*/     
   // s_video_player_control_info.process_time = process_time;
    s_video_player_control_info.seek_time = process_time;
    s_video_player_control_info.total_time = total_time;

   // VP_STRACE_LOW("[VP] NotifyPlayerForPlayNet full_path_name_ptr = %x",*full_path_name_ptr);
    SaveHistory(full_path_name_ptr,full_path_name_len,url_ptr,process_time,VP_PLAY_FROM_NET,total_time); //保存历史记录

    if (VP_PLAY_FROM_NET == s_video_player_control_info.play_type)
    {
    	if (!GetSimCardForPlay(&(s_video_player_control_info.dual_sys)))
	    {
            s_video_player_control_info.to_do_type = VP_TO_DO_STOP;
	        return TRUE;
	    }
	    else//NEWMS00164417
	    {
        	  if(MMK_IsOpenWin( MMIVP_NET_WIN_ID)) 
                {
                    MMK_CloseWin(MMIVP_NET_WIN_ID);
                }
	    }
    }

    if (MMK_IsFocusWin(MMIVP_PLAYER_WIN_ID)
        && s_is_full_paint)  //通知播放器播放网络文件
    {    
        MMK_PostMsg(MMIVP_PLAYER_WIN_ID,MSG_VP_BEGIN_PLAY,PNULL,PNULL);
    }
    else
    {
        s_video_player_control_info.to_do_type = VP_TO_DO_PLAY_FOR_FULL_PAINT;
    }
    return TRUE;
}
#endif
/*****************************************************************************/
//  Description : 更新屏幕内容
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void UpdateVPWin(BOOLEAN is_fresh)
{

	SCI_TRACE_LOW("[VP] UpdateVPWin is_fresh=%d",is_fresh);
#if defined MMI_PDA_SUPPORT && defined(MMI_ISTYLE_SUPPORT)//istyle_return
    if (MMITHEME_IsIstyle() &&  !MMITHEME_IsMainScreenLandscape() )
    {
        DisplayVPWinReturnIcion(is_fresh);
    }
#endif
    //文件选择
    DisplayVPWinFileIcon(is_fresh);
    
    DisplayVPWinRepeatModeIcon(is_fresh);  //会立即刷新
    DisplayVPWinRandomModeIcon(is_fresh);
    DisplayVPWinTitle(is_fresh);
    //显示播放区域
    if(is_fresh)
    {    
        DisplayVpWinVideoArea(PNULL);
        DisplayVPWinProgress(is_fresh);
    }
    DisplayVPWinBackwardOrForwardIcon(is_fresh);
    DisplayVPWinPlayOrPauseIcon(is_fresh);
    DisplayVPWinSwitchScreenIcon(is_fresh);    
    DisplayVPWinStation(TXT_NULL,is_fresh,TRUE); 
    
    DisplayVPWinVolumeIcon(is_fresh);
    //softkey
    DisplayVPWinSoftkey(is_fresh);    
    DisappearVPWin(MMIVP_PLAYER_WIN_ID,(BOOLEAN)!(s_video_player_control_info.is_display_win),is_fresh);
    
}
  
/*****************************************************************************/
//  Description : enter video player main screen.
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void MMIAPIVP_EnterVdoPly(void)
{
#ifdef UI_P3D_SUPPORT
    MMI_Disable3DEffect(MMI_3D_EFFECT_VDOPLY);
#endif
    MMK_CreateWin((uint32 *)MMIVP_MAIN_PLAY_WIN_TAB, PNULL); 
}

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
                                  )
{
   return  MMIAPIVP_PlayVideoFromVideoPlayerEx(vp_entry,full_path_name_ptr,full_path_name_len,url_ptr,0,PNULL);
}

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
                                  )
{  
 
    if (MMIAPIUDISK_UdiskIsRun())
    {
        MMIPUB_OpenAlertWarningWin(TXT_COMMON_UDISK_USING);
        return FALSE;
    }
    else if(MMIAPISD_IsCardLogOn())
    {
        MMIPUB_OpenAlertWarningWin(TXT_COMMON_CARDLOG_USING);
        return FALSE;
    }
    else if (MMIAPIENG_GetIQModeStatus())
    {
        MMIPUB_OpenAlertWarningWin(TXT_IQ_DATA_PROCESSING);
        return FALSE;
    }
    else if (MMIAPICC_IsInState(CC_IN_CALL_STATE))
    {
        MMIPUB_OpenAlertWarningWin(TXT_CALL_USING);
        return FALSE;
    }
    
    if (VP_ENTRY_FROM_PLAYER != vp_entry && PNULL == full_path_name_ptr && PNULL == url_ptr)
    {
        MMIPUB_OpenAlertWarningWin(TXT_COM_FILE_NO_EXIST);
        return FALSE;
    }
    
        //JAVA 后台运行,内存不足，提示退出
#ifdef JAVA_SUPPORT
    //VP_STRACE_LOW:"MMIAPIVP_PlayVideoFromVideoPlayer java running =%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_3045_112_2_18_3_5_34_431,(uint8*)"d", MMIAPIJAVA_IsJavaRuning());
    if (MMIAPIJAVA_IsJavaRuning())
    {
        MMIPUB_OpenAlertWarningWin( TXT_EXIT_BACKGROUND_JAVA);
        return FALSE;
    }
#endif

    //VP_STRACE_LOW:"[VP] MMIAPIVP_PlayVideoFromVideoPlayer vp_entry = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_3053_112_2_18_3_5_34_432,(uint8*)"d",vp_entry);

#if defined(MMISTRM_APP_CONFLICT)
    if(VP_ENTRY_FROM_NET == vp_entry)
    {
    #ifdef BROWSER_SUPPORT
        if (MMIAPIBROWSER_IsRunning())
        { 
            //VP_STRACE_LOW:"[VP] MMIAPIVP_PlayVideoFromVideoPlayer MMIAPIBROWSER_IsRunning()"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_3061_112_2_18_3_5_34_433,(uint8*)"");
            MMIAPIBROWSER_Close();
        }
    #endif
    }
#endif    
#ifdef MMI_VIDEOWALLPAPER_SUPPORT
    MMIAPISETVWP_StopVideoWallpaper();
#endif
    if (!MMK_IsOpenWin(MMIVP_PLAYER_WIN_ID))
    {        
        SCI_MEMSET(&s_video_player_control_info,0,sizeof(VP_INTERFACE_PLAYING_INFO_T));    
        s_video_player_control_info.vp_entry = vp_entry;
        s_video_player_control_info.station_text_id = TXT_NULL;
        s_video_player_control_info.support_type = VP_SUPPORTED_BOTH;        
        s_video_player_control_info.vp_handle = MMIAPIVP_FSMInit(HandleFsmMsg);  //创建状态机
        s_key_down_msg_id = 0;
        s_tp_down_ctrl_id = 0;
        
        if (VP_ENTRY_FROM_PLAYER != vp_entry && PNULL != s_video_player_control_info.vp_handle) //从本地文件或者网络中直接播放
        {
            //需要立即进入不加载本地播放列表中的信息
#ifdef MMI_VIDEOPLAYER_HISTORYLIST_SUPPORT
             //加载历史记录列表
            VP_HistoryList_Load();
#endif
#ifdef MMI_STREAMING_PLAYER_SUPPORT
            //加载书签列表
            VP_BookmarkList_Load();  
#endif
            //打开播放器
            MMIAPIVP_EnterVdoPly();
            //MMK_CreateWin((uint32 *)MMIVP_MAIN_PLAY_WIN_TAB, PNULL);   
            
#ifdef MMI_PDA_SUPPORT//istyle_return
            MMK_CreateCtrlByWinTabEx( MMIVP_MAIN_PLAY_WIN_ID,  (uint32 *)(MMITHEME_IsMainScreenLandscape() ? MMIVP_MAIN_PLAY_WIN_TAB_H : ((MMITHEME_IsIstyle() ? MMIVP_MAIN_PLAY_WIN_TAB_V_ISTYLE : MMIVP_MAIN_PLAY_WIN_TAB_V))) );            
#else
            MMK_CreateCtrlByWinTabEx( MMIVP_MAIN_PLAY_WIN_ID,  (uint32 *)(MMITHEME_IsMainScreenLandscape() ? MMIVP_MAIN_PLAY_WIN_TAB_H : MMIVP_MAIN_PLAY_WIN_TAB_V) );
#endif
        }
        else if (VP_ENTRY_FROM_PLAYER == vp_entry && PNULL != s_video_player_control_info.vp_handle)
        {
            //打开播放器
            MMIAPIVP_EnterVdoPly();
#ifdef MMI_PDA_SUPPORT//istyle_return
            MMK_CreateCtrlByWinTabEx( MMIVP_MAIN_PLAY_WIN_ID,  (uint32 *)(MMITHEME_IsMainScreenLandscape() ? MMIVP_MAIN_PLAY_WIN_TAB_H : ((MMITHEME_IsIstyle() ? MMIVP_MAIN_PLAY_WIN_TAB_V_ISTYLE : MMIVP_MAIN_PLAY_WIN_TAB_V))) );            
#else
            MMK_CreateCtrlByWinTabEx( MMIVP_MAIN_PLAY_WIN_ID,  (uint32 *)(MMITHEME_IsMainScreenLandscape() ? MMIVP_MAIN_PLAY_WIN_TAB_H : MMIVP_MAIN_PLAY_WIN_TAB_V) );
#endif

        }
        else  //创建状态机失败
        {
           
            //VP_STRACE_LOW:"[VP] MMIAPIVP_MainEntry cannot creat fsm"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_3113_112_2_18_3_5_34_434,(uint8*)"");
            MMIPUB_OpenAlertWarningWin(TXT_VP_BUSY);
            return FALSE;
        }
    }
    else  //播放器窗口已经打开，设置获得focus
    {        
        MMK_WinGrabFocus(MMIVP_PLAYER_WIN_ID);
    }

        s_video_player_control_info.post_progrecss = post_progrecss;
    //通知播放器开始播放
    if (VP_ENTRY_FROM_PLAYER != vp_entry)
    { 
 #ifdef MMI_STREAMING_PLAYER_SUPPORT       
        if (VP_ENTRY_FROM_NET == vp_entry)
        {
            NotifyPlayerForPlayNet(full_path_name_ptr,full_path_name_len,url_ptr,progress_time,0);           
        }
        else
#endif
        {            
            NotifyPlayerForPlayFile(full_path_name_ptr,full_path_name_len,progress_time,0,VP_PLAY_FROM_FILE);            
        }
        
    }
    return TRUE;
    
}


/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author:tao.xue
//	Note: 
/*****************************************************************************/
LOCAL void VP_ValidatePrivacyAppEntry(void)
{
    MMIAPIVP_PlayVideoFromVideoPlayer(VP_ENTRY_FROM_PLAYER,PNULL,0,PNULL);
}

/*****************************************************************************/
//  Description : 进入视频播放器
//  Global resource dependence : 
//  Author: 
/*****************************************************************************/
PUBLIC void MMIAPIVP_MainEntry(void)
{
    MMIAPISET_ValidatePrivacyAppEntry(MMISET_PROTECT_VIDEO_PLAYER, VP_ValidatePrivacyAppEntry);   
}

/*****************************************************************************/
//  Description : 退出视频播放器
//  Global resource dependence : 
//  Author: 
/*****************************************************************************/
PUBLIC void MMIAPIVP_ExitVideoPlayer(void)
{
    //VP_STRACE_LOW:"[VP] MMIAPIVP_ExitVideoPlayer"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_3168_112_2_18_3_5_34_435,(uint8*)"");
#ifdef MMI_VIDEOPLAYER_HISTORYLIST_SUPPORT
    if (IsSaveLastPlayPoint())
    {        
        VP_HistoryList_SetLastPlayPoint(s_video_player_control_info.process_time);
    }
#endif
    MMIAPIVP_FSMExit(s_video_player_control_info.vp_handle);
    if(MMK_IsOpenWin(MMIVP_MAIN_PLAY_WIN_ID))
    {
        MMIPUB_CloseQuerytWin(PNULL);   //必须关掉期间开启的询问窗口
#ifdef MMI_GPRS_SUPPORT
    	MMIAPICL_CloseGprsFlowRateWin();
#endif
    }
    MMK_CloseMiddleWin(MMIVP_MAIN_PLAY_WIN_ID, MMIVP_END_WIN_ID);

    MMIAPIFMM_CloseOptionWin();//close file details win

    //开启3d效果
    //MMI_Enable3DMMI(TRUE);
    
}

/*****************************************************************************/
//  Description :  加载本地播放列表等待窗口处理函数
//  Global resource dependence : 
//  Author:apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleVPSearchWaitingWin(
                         MMI_WIN_ID_T       win_id,     
                         MMI_MESSAGE_ID_E   msg_id, 
                         DPARAM             param
                         )
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;      
    static  BOOLEAN  is_open_player_win = FALSE;
    static VP_LOCALLIST_TYPE_E   locallist_type = VP_LOCALLIST_DEFAULT;
    static MMIFILE_DEVICE_E      file_dev_type = MMI_DEVICE_NUM;
    MMI_WIN_ID_T    locallist_win_id  = 0;
    MMI_CTRL_ID_T   locallist_ctrl_id = 0;
    MMIFILE_ERROR_E   file_error = SFS_ERROR_NONE;
    static BOOLEAN   is_stop_by_user = FALSE;  //1.用于判断是否是用户主动停止搜索 2.用于记录是否自己关掉的窗口。
    
    
    
    //VP_STRACE_LOW:"[VP] HandleVPSearchWaitingWin msg_id = %x, is_open_player_win = %d, locallist_type = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_3210_112_2_18_3_5_34_436,(uint8*)"ddd",msg_id,is_open_player_win,locallist_type);
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        is_open_player_win = FALSE;    
        locallist_type = VP_LOCALLIST_DEFAULT;
        file_dev_type=  MMI_DEVICE_UDISK;//如果平台没有U盘，这个初始值必须根据FILE_DEV_E_T定义做相应修改；
        is_stop_by_user = FALSE;
////////////进入后全屏
#ifdef MMI_VIDEO_PLAYER_MINI_LAYOUT//6530 MINI 界面布局 进入视频播放器直接全屏 将主界面和搜索界面转为横屏
        if(LCD_ANGLE_0 == MMK_GetScreenAngle())
        {
            MMK_SetWinSupportAngle(MMIVP_MAIN_PLAY_WIN_ID, WIN_SUPPORT_ANGLE_90);
            MMK_SetWinAngle(MMIVP_MAIN_PLAY_WIN_ID, LCD_ANGLE_90, TRUE);
            MMK_SetWinSupportAngle(win_id, WIN_SUPPORT_ANGLE_90);
            MMK_SetWinAngle(win_id, LCD_ANGLE_90, TRUE);
        }
#endif
        result = MMIPUB_HandleWaitWinMsg(win_id,msg_id,param);        
        break;


    case MSG_KEYDOWN_CANCEL:    
        is_stop_by_user = TRUE;
        MMIAPIFMM_SearchFileStop();
        break;

    case MSG_APP_RED:
        is_stop_by_user = TRUE;
        MMIAPIFMM_SearchFileStop();
        result = MMI_RESULT_FALSE;
        break;

    case MSG_VP_UPDATE_BEGIN:  //开始更新本地播放列表
        if(PNULL != param)
        {
            locallist_type = *(VP_LOCALLIST_TYPE_E *)param;
        }
        else
        {
            is_stop_by_user = TRUE;
            MMIPUB_OpenAlertFailWin(TXT_UPDATE_FAIL);
            MMIPUB_CloseWaitWin(win_id);
            break;
        }

        //VP_STRACE_LOW:"[VP] HandleVPSearchWaitingWin list_index = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_3242_112_2_18_3_5_34_437,(uint8*)"d",locallist_type);
        
        if(!VP_LocalList_BeginUpdate(win_id, MSG_VP_UPDATE_END,locallist_type))
        {
            is_stop_by_user = TRUE;
            MMIPUB_OpenAlertFailWin(TXT_UPDATE_FAIL);
            MMIPUB_CloseWaitWin(win_id);
        }
        break;

    case MSG_VP_UPDATE_END:  //更新完成
        if (VP_LOCALLIST_DEFAULT == locallist_type)
        {
            locallist_win_id = MMIVP_DEFAULT_LOCALLIST_WIN_ID;
            locallist_ctrl_id = VP_DEFAULT_LOCALLIST_CTRL_ID;
        }
        else
        {
            locallist_win_id = MMIVP_USER_LOCALLIST_WIN_ID;
            locallist_ctrl_id = VP_USER_LOCALLIST_CTRL_ID;
        }
      
        file_error = VP_LocalList_EndUpdate(locallist_type); 

        //更新列表，给出提示
        if (SFS_ERROR_NO_SPACE == file_error)
        {
            UpdateListData(locallist_win_id,locallist_ctrl_id,VP_LocalList_GetNum(locallist_type),GetLocalListItemContent);
            if(MMI_DEVICE_NUM <= MMIAPIFMM_GetValidateDevice(MMI_DEVICE_UDISK))
            {
                MMIPUB_OpenAlertWinByTextId(PNULL,TXT_NO_SD_CARD_ALERT,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_NONE,PNULL);
            }
            else
            {
                MMIPUB_OpenAlertWinByTextId(PNULL,TXT_NO_SPACE,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_NONE,PNULL);
            }
        } 
        else if (SFS_ERROR_NONE != file_error)
        {
            UpdateListData(locallist_win_id,locallist_ctrl_id,VP_LocalList_GetNum(locallist_type),GetLocalListItemContent);
            MMIPUB_OpenAlertFailWin(TXT_UPDATE_FAIL);
        }         
        else
        {
            UpdateListData(locallist_win_id,locallist_ctrl_id,VP_LocalList_GetNum(locallist_type),GetLocalListItemContent);
        }
        is_stop_by_user = TRUE;
        MMIPUB_CloseWaitWin(win_id);        
        break;
#ifdef MMI_VIDEOPLAYER_FIRST_SEARCH        
    case MSG_VP_CHECK_NEED_RESEARCH:
        if(VP_LocalList_IsNeedReSearchLocallist())
        {
            MMK_PostMsg(win_id,MSG_VP_LOADING_BEGIN,PNULL,PNULL); 
        }
        else
        {
            VP_LocalList_Load();     
            //打开播放器窗口
            if(MMK_IsFocusWin(win_id))
            {      
                SetBlockType(VP_HALF_TRANSPARENCE);      
                is_stop_by_user = TRUE;
                MMIPUB_CloseWaitWin(win_id);
            } 
            else
            {
                is_open_player_win = TRUE;
            }
        }
        break;
#endif        
    case MSG_VP_LOADING_BEGIN:  //开始加载默认播放列表，包括U盘和SD卡(可能多卡)下的video文件夹中的视频文件
        locallist_type = VP_LOCALLIST_DEFAULT;
        if(!VP_LocalList_BeginLoad(win_id, MSG_VP_LOADING_END,locallist_type,file_dev_type)) //开始加载U盘的默认播放列表
        { 
            MMK_PostMsg(win_id,MSG_VP_LOADING_END,PNULL,PNULL);  //继续加载其他file device type下默认文件夹下面的视频文件
        }
        break;  
        
    case MSG_VP_LOADING_END:  //加载结束
        if (!is_stop_by_user && VP_LOCALLIST_DEFAULT == locallist_type)
        {   
            if (MMI_DEVICE_NUM != file_dev_type)
            {
                file_dev_type++;
                MMK_PostMsg(win_id,MSG_VP_LOADING_BEGIN,PNULL,PNULL);  //开始加载其他file device type下默认文件夹下面的视频文件
            }
            else
            {
                 file_error = VP_LocalList_EndLoad(locallist_type); //默认播放列表加载完毕

                if (SFS_ERROR_NO_SPACE == file_error)
                {       
                    if(MMI_DEVICE_NUM <= MMIAPIFMM_GetValidateDevice(MMI_DEVICE_UDISK))
                    {
                        /* Modify by michael on 2/27/2012 for NEWMS00172443 : 统一修改弹出信息*/
                        MMIPUB_OpenAlertWinByTextId(PNULL,TXT_PLEASE_INSERT_SD,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_NONE,PNULL);
                        /************************Modify over*************************/
                    }
                    else
                    {
                        MMIPUB_OpenAlertWinByTextId(PNULL,TXT_NO_SPACE,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_NONE,PNULL);
                    }
                }  

                locallist_type = VP_LOCALLIST_USER;
                if(!VP_LocalList_BeginLoad(win_id, MSG_VP_LOADING_END,locallist_type,file_dev_type)) //开始加载用户自定义播放列表
                {  
                    MMIPUB_OpenAlertWinByTextId(PNULL,TXT_VP_LOADING_ERROR,TXT_NULL,IMAGE_PUBWIN_FAIL,PNULL,PNULL,MMIPUB_SOFTKEY_NONE,PNULL);
                    MMK_PostMsg(win_id,MSG_VP_LOADING_END,PNULL,PNULL);
                }
            }
        }
        else  //用户自定义播放列表加载完毕
        {            
            file_error = VP_LocalList_EndLoad(locallist_type);
            if (SFS_ERROR_NO_SPACE == file_error)
            {    
                MMIPUB_OpenAlertWinByTextId(PNULL,TXT_NO_SPACE,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_NONE,PNULL);
            }      
            else if(SFS_ERROR_NONE != file_error)
            {
                MMIPUB_OpenAlertWinByTextId(PNULL,TXT_VP_LOADING_ERROR,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_NONE,PNULL);//@andy.he 11-06-07 cr245369
            }
       
#ifdef MMI_VIDEOPLAYER_HISTORYLIST_SUPPORT             
            //加载历史记录列表
            VP_HistoryList_Load();
#endif
#ifdef MMI_STREAMING_PLAYER_SUPPORT
            //加载书签列表
            VP_BookmarkList_Load();
#endif
             if (VP_ENTRY_FROM_PLAYER == s_video_player_control_info.vp_entry)
            {
                //获取最后一次播放的信息
                if(!LoadLastPlayingFile())
                {
                    LoadFirstLocalListFile();
                }
                #ifdef KING_MOVIE_SUPPORT
                s_video_player_control_info.is_kmv_file = MMIAPICOM_IsKingMovieType(s_video_player_control_info.full_path_name,s_video_player_control_info.full_path_len);
                #endif
             }   
            //打开播放器窗口
            if(MMK_IsFocusWin(win_id))
            {      
                SetBlockType(VP_HALF_TRANSPARENCE);      
                is_stop_by_user = TRUE;
                MMIPUB_CloseWaitWin(win_id);
            } 
            else
            {
                is_open_player_win = TRUE;
            }
        }
        break;

    case MSG_GET_FOCUS:
        if (is_open_player_win)  //lose focus状态下加载完毕
        {

            SetBlockType(VP_HALF_TRANSPARENCE);   
            is_stop_by_user = TRUE;
            MMIPUB_CloseWaitWin(win_id);
        }
        break;

    case MSG_CLOSE_WINDOW:
        if (!is_stop_by_user)
        {
            is_stop_by_user = TRUE;
            //MMIAPIFMM_SearchFileStop();
        }
        MMIAPIFMM_SearchFileStop();
        if (!MMK_IsOpenWin(MMIVP_PLAYER_WIN_ID))  //打开播放器出错，释放相关资源，退出播放器
        {
            //释放状态机  
            MMIAPIVP_FSMRelease(s_video_player_control_info.vp_handle);
            SCI_MEMSET(&s_video_player_control_info,0,sizeof( VP_INTERFACE_PLAYING_INFO_T));
#ifdef MMI_STREAMING_PLAYER_SUPPORT
            //释放书签资源
            VP_BookmarkList_Release();
#endif
#ifdef MMI_VIDEOPLAYER_HISTORYLIST_SUPPORT
            //释放历史记录
            VP_HistoryList_Release();
#endif
            //释放本地播放列表
            VP_LocalList_Release(VP_LOCALLIST_DEFAULT);
            VP_LocalList_Release(VP_LOCALLIST_USER);
            //开启3d效果
            //MMI_Enable3DMMI(TRUE);
        }
        
        break;

    default:
        result = MMIPUB_HandleWaitWinMsg(win_id,msg_id,param);
        break;
    }

    return result;
}

/*****************************************************************************/
//  Description : 显示视频播放器主界面文件名称
//  Global resource dependence : 
//  Author: apple.zhang
//  Note:
/*****************************************************************************/
LOCAL void DisplayVPWinTitle(BOOLEAN is_fresh)
{
    
    wchar                file_name[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
    uint16               file_name_len = MMIFILE_FULL_PATH_MAX_LEN;
    MMI_STRING_T         title = {0};
#if (defined MMI_VIDEO_PLAYER_MINI_LAYOUT) && (defined MMI_STREAMING_PLAYER_SUPPORT)
    GUI_RECT_T           name_label_rect = VP_NAME_LABEL_RECT_H;
#endif
    
    if (!MMK_IsOpenWin(MMIVP_PLAYER_WIN_ID))
    {
        return;
    }

    if (!IsAbleToFresh())
    {
        is_fresh = FALSE;
    }

    //本地文件和本地列表文件需要从记录的全路径中分离文件名，网路文件直接使用，没有文件，不显示
    if ((VP_PLAY_FROM_FILE == s_video_player_control_info.play_type) || (VP_PLAY_FROM_LOCALLIST == s_video_player_control_info.play_type))
    {
        MMIAPIFMM_SplitFullPath(
                s_video_player_control_info.full_path_name, 
                s_video_player_control_info.full_path_len, 
                PNULL, PNULL, PNULL, PNULL, 
                file_name, &file_name_len);
    }
    else if (VP_PLAY_FROM_NONE == s_video_player_control_info.play_type)
    {
         title.wstr_ptr = PNULL;
        title.wstr_len = 0;
    }
    else
    {
        file_name_len = (uint16)MIN(s_video_player_control_info.full_path_len,MMIFILE_FULL_PATH_MAX_LEN);     
        MMIAPICOM_Wstrncpy(file_name,s_video_player_control_info.full_path_name,file_name_len);
               
    }
    title.wstr_ptr = file_name;
    title.wstr_len = file_name_len;      
   
////CR NEWMS00207628   leimingma
#ifdef MMI_VIDEO_PLAYER_MINI_LAYOUT //6530_MINI  
    MMK_SetActiveCtrl(VP_PLAY_NAME_LABEL_CTRL_ID, is_fresh); 
    GUIFORM_PermitChildBorder(VP_PLAY_PANEL_TITLE_FORM_CTRL_ID, FALSE);
#endif
////CR NEWMS00207628  leimingma

#if (defined MMI_VIDEO_PLAYER_MINI_LAYOUT) && (defined MMI_STREAMING_PLAYER_SUPPORT)
    if (VP_PLAY_FROM_NET == s_video_player_control_info.play_type)
    {
        name_label_rect.left  = VP_NAME_LABEL_NET_RECT_H_LEFT;
    }

    GUILABEL_SetRect(VP_PLAY_NAME_LABEL_CTRL_ID, &name_label_rect, FALSE);
#endif//MMI_VIDEO_PLAYER_MINI_LAYOUT
    GUILABEL_SetText(VP_PLAY_NAME_LABEL_CTRL_ID, &title, is_fresh);   

}

/*****************************************************************************/
//  Description : 停止显示视频播放器主界面状态名称消失timer
//  Global resource dependence : 
//  Author: apple.zhang
//  Note:
/*****************************************************************************/
LOCAL void StopVPDisplayStationTimer(void)
{
    if (0 != s_video_player_control_info.display_state_timer_id)
    {
        MMK_StopTimer(s_video_player_control_info.display_state_timer_id);
        s_video_player_control_info.display_state_timer_id = 0;
        s_video_player_control_info.station_text_id = TXT_NULL;
    }  
}

/*****************************************************************************/
//  Description : 启动显示视频播放器主界面状态名称消失timer
//  Global resource dependence : 
//  Author: apple.zhang
//  Note:
/*****************************************************************************/
LOCAL void StartVPDisplayStationTimer(void)
{
    if (0 != s_video_player_control_info.display_state_timer_id)
    {
        MMK_StopTimer(s_video_player_control_info.display_state_timer_id);
        s_video_player_control_info.display_state_timer_id = 0;
    }   
    s_video_player_control_info.display_state_timer_id = MMK_CreateWinTimer(MMIVP_PLAYER_WIN_ID, MMI_3SECONDS, FALSE);
    MMK_StartWinTimer(MMIVP_PLAYER_WIN_ID, s_video_player_control_info.display_state_timer_id, MMI_3SECONDS, FALSE);
}

/*****************************************************************************/
//  Description : 处理显示视频播放器主界面状态名称消失timer
//  Global resource dependence : 
//  Author: apple.zhang
//  Note:
/*****************************************************************************/
LOCAL void HandleVPDisplayStationTimer(void)
{
    MMI_TEXT_ID_T text_id = TXT_NULL;

    switch(s_video_player_control_info.state) 
    { 
    case VP_STATE_READY:
        if (0 == s_video_player_control_info.full_path_len) //当前没有文件
        {
            text_id = TXT_VP_NO_FILE;
        }
        break;

    case VP_STATE_PLAY_PENDING:
        if (VP_STATE_INIT == MMIAPIVP_FSMGetState(s_video_player_control_info.vp_handle)
            || VP_STATE_PDP_ACTIVING == MMIAPIVP_FSMGetState(s_video_player_control_info.vp_handle))
        {
            text_id = TXT_VP_INIT;
        }
        else
        {
            text_id = TXT_VP_READY;
        }
        break;

    case VP_STATE_PLAYING:
        if (VP_SUPPORTED_AUDIO_ONLY == s_video_player_control_info.support_type)
        {
#ifdef MMI_VIDEO_PLAYER_MINI_LAYOUT//6530 MINI 界面布局
            text_id = TXT_VP_VIDIO_NOT_SUPPORT_MINI;
#else
            text_id = TXT_VP_VIDIO_NOT_SUPPORT;
#endif
        }               
        break;
    
    case VP_STATE_PAUSING:
        text_id = TXT_VP_PAUSING;
        break;

    case VP_STATE_PAUSED:
        text_id = TXT_VP_PAUSED;
        break;

    case VP_STATE_RESUMING:
        text_id = TXT_VP_RESUMING;
        break;

    case VP_STATE_SEEKING:
        text_id = TXT_VP_SEEKING;
        break;

    case VP_STATE_STOPING:
        text_id = TXT_VP_STOPPING;
        break;

    case VP_STATE_BUFFERING:
        text_id = TXT_VP_BUFFRING;
        break;

    default:      
        break;
    }       

    {
        DisplayVPWinStation(text_id,TRUE,FALSE); 
    }     

}
/*****************************************************************************/
//  Description : 显示视频播放器主界面状态名称
//  Global resource dependence : 
//  Author: apple.zhang
//  Note: 如果text_id != TXT_NULL则根据text_id显示，停止显示timer
//       如果text_id == TXT_NULL且is_keep_last为true，则显示上一次显示的结果
//       如果text_id == TXT_NULL is_keep_last == false则清空当前显示，停止显示timer
/*****************************************************************************/
LOCAL void DisplayVPWinStation(MMI_TEXT_ID_T text_id, BOOLEAN is_fresh, BOOLEAN is_keep_last)
{         
    MMI_STRING_T            display_station = {0};     
    GUI_RECT_T              display_rect = {0};
    GUI_RECT_T              fill_rect = {0};
    GUISTR_STYLE_T          text_style = {0};
    uint16                  state_width = 0; 
    GUISTR_STATE_T          state = GUISTR_STATE_ALIGN|GUISTR_STATE_SINGLE_LINE|GUISTR_STATE_ELLIPSIS;
    GUI_LCD_DEV_INFO        dev_info = {0};
    GUI_BORDER_T            boder_info = {0};
    

    if (VP_PLAY_FROM_NET != s_video_player_control_info.play_type
        && (TXT_VP_NO_FILE == text_id
        || TXT_VP_PLAYING == text_id
        || TXT_VP_PAUSED == text_id
        || STXT_STOP == text_id
        || TXT_VP_PLAY_END == text_id || (TXT_NULL== text_id && (VP_SUPPORTED_BOTH == s_video_player_control_info.support_type))))
    {
        return;
    }

    if (!s_is_full_paint
        || !MMK_IsFocusWin(MMIVP_PLAYER_WIN_ID)
         )
    {
        is_fresh = FALSE;
    }    
    
    if (TXT_NULL == text_id && is_keep_last)    //根据当前状态显示
    {        
       text_id = s_video_player_control_info.station_text_id;        
       if ((TXT_VP_PLAYING == text_id
           && 0 == s_video_player_control_info.display_state_timer_id)
           || (TXT_VP_TOO_MUCH == text_id)
           || (TXT_VP_SAVE_BOOKMARK_SUCC == text_id)
           || (TXT_ERROR == text_id))
       {
           StartVPDisplayStationTimer();
       }
    }    
    else
    {
        StopVPDisplayStationTimer();
    }
    
    s_video_player_control_info.station_text_id = text_id;    

   
    if (!s_video_player_control_info.is_display_win)
    {
        display_rect = MMITHEME_GetFullScreenRect();
    }
    else
    {
        GUIOWNDRAW_GetDisplayRect(VP_PLAY_PANEL_VIDEO_OWNDRAW_CTRL_ID,&display_rect);
    }

    dev_info.lcd_id = GUI_MAIN_LCD_ID;
    dev_info.block_id = GUI_BLOCK_MAIN; 
#ifdef MMI_VIDEO_PLAYER_MINI_LAYOUT //6530_MINI
    display_rect.left = VP_VIDEO_AREA_LEFT;
    display_rect.right = VP_VIDEO_AREA_WIDTH_H;
    display_rect.bottom = display_rect.bottom  - VP_STATION_ADJUST;  //leiming ma
    display_rect.top = display_rect.bottom - VP_STATION_FONT  - VP_STATION_ADJUST ;//- VP_DEFAULTPIC_STATION_DISTANCE;   //leiming ma
#else
    display_rect.bottom = display_rect.bottom - 5;  
    display_rect.top = display_rect.bottom - VP_STATION_FONT -2;   
#endif

    if (is_fresh)
    {
        //清除上一次痕迹
        fill_rect.left = 0;
        fill_rect.top = display_rect.top;
        fill_rect.bottom = display_rect.bottom;
#ifdef MMI_VIDEO_PLAYER_MINI_LAYOUT //6530_MINI 防止覆盖右侧button
        fill_rect.right = MMITHEME_GetMainScreenRightPixel() - VP_BUTTON_RIGHT;
#else
        fill_rect.right = MMITHEME_GetMainScreenRightPixel();
#endif
        GUI_FillRect(&dev_info,fill_rect,MMIAPIVP_GetTransparenceColor());     
    }
    
        
    if (TXT_NULL != text_id)
    {    
        MMI_STRING_T text_string = {0};

        MMI_GetLabelTextByLang(text_id,&text_string);  

        if (TXT_VP_BUFFRING == text_id)
        {
            char percent_arr[10] = {0};
            wchar percent_warr[10] = {0};
            SCI_MEMSET(percent_warr,0,10*sizeof(wchar));
            SCI_MEMSET(percent_arr,0,10);
            sprintf((char*)percent_arr, " %d%%",s_video_player_control_info.buffer_percent);
            MMIAPICOM_StrToWstr((uint8*)percent_arr,percent_warr);
            display_station.wstr_len = text_string.wstr_len + strlen(percent_arr);
            //VP_STRACE_LOW:"[VP] DisplayVPWinStation display_station.wstr_len = %d,text_string.wstr_len = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_3646_112_2_18_3_5_35_438,(uint8*)"dd",display_station.wstr_len,text_string.wstr_len);
            display_station.wstr_ptr = SCI_ALLOCA((display_station.wstr_len+1) * sizeof(wchar));
            if (PNULL != display_station.wstr_ptr)
            {
                SCI_MEMSET(display_station.wstr_ptr,0,((display_station.wstr_len+1) * sizeof(wchar)));
                MMIAPICOM_Wstrncpy(display_station.wstr_ptr,text_string.wstr_ptr,text_string.wstr_len);
                MMIAPICOM_Wstrncpy((display_station.wstr_ptr + text_string.wstr_len),percent_warr,strlen(percent_arr));
            }
            else
            {
                return;
            }
        }
        else
        {
            display_station.wstr_ptr = SCI_ALLOCA((text_string.wstr_len+1) * sizeof(wchar));
            if (PNULL != display_station.wstr_ptr)
            {
                SCI_MEMSET(display_station.wstr_ptr,0,((text_string.wstr_len+1) * sizeof(wchar)));
                MMIAPICOM_Wstrncpy(display_station.wstr_ptr,text_string.wstr_ptr,text_string.wstr_len);            
                display_station.wstr_len = text_string.wstr_len;
            }
            else
            {
                return;
            }
        }
        text_style.align = ALIGN_HVMIDDLE;
        text_style.angle = ANGLE_0;
        text_style.effect = FONT_EFFECT_NONE;
        text_style.font =  VP_STATION_FONT;
        if (MMI_WHITE_COLOR == MMIAPIVP_GetTransparenceColor())
        {
            text_style.edge_color = MMI_GRAY_WHITE_COLOR;        
            text_style.font_color = MMI_GRAY_WHITE_COLOR;
            text_style.bg_color = MMI_BACKGROUND_COLOR;
        }
        else
        {
            text_style.edge_color = MMI_BACKGROUND_COLOR;        
            text_style.font_color = MMI_BACKGROUND_COLOR;
            text_style.bg_color = MMI_GRAY_WHITE_COLOR;
        }    
        
        
        if (is_fresh)
        {
            

            //获取提示宽度，计算显示区域
            state_width =  GUI_GetStringWidth(VP_STATION_FONT,display_station.wstr_ptr,display_station.wstr_len);      
             
            if (state_width > (fill_rect.right - fill_rect.left +1))
            {
                display_rect.left = fill_rect.left + VP_STATION_MARGIN_H;
                display_rect.right = fill_rect.right - VP_STATION_MARGIN_H;
                
            }
            else
            {
#ifdef MMI_VIDEO_PLAYER_MINI_LAYOUT //6530_MINI
                if (state_width > MMITHEME_GetMainScreenRightPixel() - 2*VP_BUTTON_RIGHT)
                {
                    display_rect.left = 0;//(MMITHEME_GetMainScreenRightPixel()  - state_width)/2 - VP_STATION_MARGIN_H; 
                    display_rect.right = MMITHEME_GetMainScreenRightPixel() - VP_BUTTON_RIGHT;//display_rect.left + state_width + VP_STATION_MARGIN_H;
                } 
                else
                {
                    display_rect.left = (MMITHEME_GetMainScreenRightPixel()  - state_width)/2 - VP_STATION_MARGIN_H; 
                    display_rect.right = display_rect.left + state_width + VP_STATION_MARGIN_H;
                }
#else
                display_rect.left = display_rect.left + (display_rect.right - display_rect.left + 1 - state_width)/2 - VP_STATION_MARGIN_H;
                display_rect.right = display_rect.left + state_width + VP_STATION_MARGIN_H + VP_STATION_MARGIN_H;
#endif
            }
            
            //填充文字显示区域背景
            GUI_FillRect(&dev_info,display_rect,text_style.bg_color);
            boder_info.color = text_style.font_color;
            boder_info.type = GUI_BORDER_SOLID;
            boder_info.width = 1;
            GUI_DisplayBorder(display_rect,display_rect,&boder_info,&dev_info);           

           

            //显示文字
            GUISTR_DrawTextToLCDInRect( 
                &dev_info,
                (const GUI_RECT_T      *)&display_rect,       //the fixed display area
                (const GUI_RECT_T      *)&display_rect,       //用户要剪切的实际区域
                (const MMI_STRING_T    *)&display_station,
                &text_style,
                state,
                GUISTR_TEXT_DIR_AUTO
                );   

        }
        if (PNULL != display_station.wstr_ptr)
        {
            SCI_FREE(display_station.wstr_ptr);
        }
    }
    
       
}



/*****************************************************************************/
//  Description : 显示视频播放器主界面循环播放图标
//  Global resource dependence : 
//  Author: apple.zhang
//  Note:
/*****************************************************************************/
LOCAL void DisplayVPWinRepeatModeIcon(BOOLEAN is_fresh)
{   
    VP_REPEAT_PLAY_MODE_E   repeat_mode = VP_REPEAT_PLAY_MODE_CLOSE;
    MMI_IMAGE_ID_T          image_id   = IMAGE_NULL;      
    GUI_BG_T                fg_info = {0};   
    GUIFORM_CHILD_DISPLAY_E display_type = GUIFORM_CHILD_DISP_NORMAL;
    MMI_HANDLE_T            form_handle = VP_PLAY_PANEL_MODE_FORM_CTRL_ID;
#ifdef TOUCH_PANEL_SUPPORT //@andy.he_MP4PDA
   MMI_IMAGE_ID_T  impressed_id  = IMAGE_NULL;
#endif

    if (!MMK_IsOpenWin(MMIVP_PLAYER_WIN_ID))
    {
        return;
    }

    if (!IsAbleToFresh())
    {
        is_fresh = FALSE;
    }
  
    //SCI_TRACE_LOW("[VP] DisplayVPWinRepeatModeIcon is_fresh = %d");
    //VP_STRACE_LOW:"[VP] DisplayVPWinRepeatModeIcon display_type = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_3767_112_2_18_3_5_35_439,(uint8*)"d",display_type);
    
    form_handle = VP_PLAY_PANEL_TITLE_FORM_CTRL_ID;
    
    GUIFORM_GetChildDisplay(form_handle,VP_PLAY_PANEL_REPEAT_BUTTON_CTRL_ID,&display_type);

    if (GUIFORM_CHILD_DISP_HIDE != display_type
        && (VP_PLAY_FROM_FILE != s_video_player_control_info.play_type
        && VP_PLAY_FROM_LOCALLIST != s_video_player_control_info.play_type))
    {
        display_type = GUIFORM_CHILD_DISP_HIDE;
    }
    else if (GUIFORM_CHILD_DISP_HIDE == display_type
        && (VP_PLAY_FROM_FILE == s_video_player_control_info.play_type
        || VP_PLAY_FROM_LOCALLIST == s_video_player_control_info.play_type))
    {
        display_type = GUIFORM_CHILD_DISP_NORMAL;        
    }

    if (is_fresh)
    {   
        
        GUIFORM_SetChildDisplay(form_handle,VP_PLAY_PANEL_REPEAT_BUTTON_CTRL_ID,display_type);        
    }

    if (GUIFORM_CHILD_DISP_HIDE == display_type )
    {
        return;
    }

    if (VP_ENTRY_FROM_PLAYER != s_video_player_control_info.vp_entry)
    {
        image_id = IMAGE_COMMON_BTN_REPEAT_OFF_GREY;   
    }  
    else
    {    
            repeat_mode = VP_Setting_GetRepeatPlayMode();
            switch(repeat_mode) 
            {
            case VP_REPEAT_PLAY_MODE_CLOSE:
                image_id = IMAGE_COMMON_BTN_REPEAT_OFF;
        #ifdef TOUCH_PANEL_SUPPORT //@andy.he_MP4PDA
                impressed_id  = IMAGE_COMMON_BTN_REPEAT_OFF_FOCUS;
        #endif
                break;
            case VP_REPEAT_PLAY_MODE_ALL:
                image_id = IMAGE_COMMON_BTN_REPEAT_ON;
        #ifdef TOUCH_PANEL_SUPPORT //@andy.he_MP4PDA
                impressed_id  = IMAGE_COMMON_BTN_REPEAT_ON_FOCUS;
        #endif        
                break;
            case VP_REPEAT_PLAY_MODE_ONE:
                image_id = IMAGE_COMMON_BTN_REPEAT_ONE;
        #ifdef TOUCH_PANEL_SUPPORT //@andy.he_MP4PDA
                impressed_id  = IMAGE_COMMON_BTN_REPEAT_ONE_FOCUS;
        #endif
                break;
            default:
                image_id = IMAGE_COMMON_BTN_REPEAT_OFF;
        #ifdef TOUCH_PANEL_SUPPORT //@andy.he_MP4PDA
                impressed_id  = IMAGE_COMMON_BTN_REPEAT_OFF_FOCUS;
        #endif

                break;
            }    
    } 
    fg_info.bg_type = GUI_BG_IMG;
    fg_info.img_id = image_id;
#ifdef TOUCH_PANEL_SUPPORT //@andy.he_MP4PDA
    if (VP_ENTRY_FROM_PLAYER != s_video_player_control_info.vp_entry)
    {
        GUIBUTTON_SetGrayed(VP_PLAY_PANEL_REPEAT_BUTTON_CTRL_ID, TRUE, &fg_info, FALSE);
    } 
    else
    {
        GUIBUTTON_SetFg(VP_PLAY_PANEL_REPEAT_BUTTON_CTRL_ID,&fg_info);  
        fg_info.img_id = impressed_id;
        GUIBUTTON_SetPressedFg(VP_PLAY_PANEL_REPEAT_BUTTON_CTRL_ID,&fg_info);
    }
#else
    GUIBUTTON_SetPressedFg(VP_PLAY_PANEL_REPEAT_BUTTON_CTRL_ID,&fg_info);
    GUIBUTTON_SetFg(VP_PLAY_PANEL_REPEAT_BUTTON_CTRL_ID,&fg_info);    
#endif
    if (is_fresh)
    {
        GUIBUTTON_Update(VP_PLAY_PANEL_REPEAT_BUTTON_CTRL_ID);
    }
  
}

/*****************************************************************************/
//  Description : 显示视频播放器主界面随机播放图标
//  Global resource dependence : 
//  Author: apple.zhang
//  Note:
/*****************************************************************************/
LOCAL void DisplayVPWinRandomModeIcon(BOOLEAN is_fresh)
{
    MMI_IMAGE_ID_T         image_id   = IMAGE_NULL;    
    GUI_BG_T               fg_info = {0};     
    GUIFORM_CHILD_DISPLAY_E display_type = GUIFORM_CHILD_DISP_NORMAL;
    MMI_HANDLE_T            form_handle = VP_PLAY_PANEL_MODE_FORM_CTRL_ID;
#ifdef TOUCH_PANEL_SUPPORT //@andy.he_MP4PDA
   MMI_IMAGE_ID_T  impressed_id  = IMAGE_NULL;
#endif
  
    if (!MMK_IsOpenWin(MMIVP_PLAYER_WIN_ID))
    {
        return;
    }

    if (!IsAbleToFresh())
    {
        is_fresh = FALSE;
    }

   
    //VP_STRACE_LOW:"[VP] DisplayVPWinRandomModeIcon display_type = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_3869_112_2_18_3_5_36_440,(uint8*)"d",display_type);

    form_handle = VP_PLAY_PANEL_TITLE_FORM_CTRL_ID;    
    GUIFORM_GetChildDisplay(form_handle,VP_PLAY_PANEL_RANDOM_BUTTON_CTRL_ID,&display_type);

    if (GUIFORM_CHILD_DISP_HIDE != display_type
        && (VP_PLAY_FROM_FILE != s_video_player_control_info.play_type
        && VP_PLAY_FROM_LOCALLIST != s_video_player_control_info.play_type))
    {
        display_type = GUIFORM_CHILD_DISP_HIDE;
    }
    else if (GUIFORM_CHILD_DISP_HIDE == display_type
        && (VP_PLAY_FROM_FILE == s_video_player_control_info.play_type
        || VP_PLAY_FROM_LOCALLIST == s_video_player_control_info.play_type))
    {
        display_type = GUIFORM_CHILD_DISP_NORMAL;        
    }

    if (is_fresh)
    {  
        GUIFORM_SetChildDisplay(form_handle,VP_PLAY_PANEL_RANDOM_BUTTON_CTRL_ID,display_type);    
    }
    

    if (GUIFORM_CHILD_DISP_HIDE == display_type)
    {
        return;
    }

    if (VP_ENTRY_FROM_PLAYER != s_video_player_control_info.vp_entry)
    {
        image_id = IMAGE_COMMON_BTN_SHUFFLE_OFF_GREY;
    }  
    else if (VP_Setting_GetIsRandomPlayMode())
    {
        image_id = IMAGE_COMMON_BTN_SHUFFLE_ON;
#ifdef TOUCH_PANEL_SUPPORT //@andy.he_MP4PDA
        impressed_id=IMAGE_COMMON_BTN_SHUFFLE_ON_FOCUS;
#endif
    }
    else
    {
        image_id = IMAGE_COMMON_BTN_SHUFFLE_OFF;
#ifdef TOUCH_PANEL_SUPPORT //@andy.he_MP4PDA
        impressed_id  = IMAGE_COMMON_BTN_SHUFFLE_OFF_FOCUS;
#endif
    }      
   
    fg_info.bg_type = GUI_BG_IMG;
    fg_info.img_id = image_id;
    #ifdef TOUCH_PANEL_SUPPORT //@andy.he_MP4PDA
    if (VP_ENTRY_FROM_PLAYER != s_video_player_control_info.vp_entry)
    {
        GUIBUTTON_SetGrayed(VP_PLAY_PANEL_RANDOM_BUTTON_CTRL_ID, TRUE, &fg_info, FALSE);
    } 
    else
    {
        GUIBUTTON_SetFg(VP_PLAY_PANEL_RANDOM_BUTTON_CTRL_ID,&fg_info);  
        fg_info.img_id = impressed_id;
        GUIBUTTON_SetPressedFg(VP_PLAY_PANEL_RANDOM_BUTTON_CTRL_ID,&fg_info);
    }
    #else
        GUIBUTTON_SetPressedFg(VP_PLAY_PANEL_RANDOM_BUTTON_CTRL_ID,&fg_info);
        GUIBUTTON_SetFg(VP_PLAY_PANEL_RANDOM_BUTTON_CTRL_ID,&fg_info);
    #endif
    if (is_fresh)
    {
        GUIBUTTON_Update(VP_PLAY_PANEL_RANDOM_BUTTON_CTRL_ID);
    }
    
}


#ifdef MMI_PDA_SUPPORT//istyle_return
/*****************************************************************************/
//  Description : 显示视频播放器主界面返回 图标
//  Global resource dependence : 
//  Author: apple.zhang
//  Note:
/*****************************************************************************/
LOCAL void DisplayVPWinReturnIcion(BOOLEAN is_fresh)
{
    
    MMI_IMAGE_ID_T  image_id = IMAGE_NULL;
    GUI_BG_T               fg_info = {0};     
   MMI_IMAGE_ID_T  impressed_id  = IMAGE_NULL;
    
    if (!MMK_IsOpenWin(MMIVP_PLAYER_WIN_ID))
    {
        return;
    }

    if (!IsAbleToFresh())
    {
        is_fresh = FALSE;
    }

    image_id = IMAGE_COMMON_BTN_RETURN;
    impressed_id = IMAGE_COMMON_BTN_RETURN_FOCUS;

    fg_info.bg_type = GUI_BG_IMG;
    fg_info.img_id = impressed_id;
    GUIBUTTON_SetPressedFg(VP_PLAY_PANEL_RETURN_BUTTON_CTRL_ID,&fg_info);
    fg_info.img_id = image_id;
    GUIBUTTON_SetFg(VP_PLAY_PANEL_RETURN_BUTTON_CTRL_ID,&fg_info);      

    if (is_fresh)
    {
        GUIBUTTON_Update(VP_PLAY_PANEL_RETURN_BUTTON_CTRL_ID);
    }   
    
}

#endif

/*****************************************************************************/
//  Description : 显示视频播放器主界面音量图标
//  Global resource dependence : 
//  Author: apple.zhang
//  Note:
/*****************************************************************************/
LOCAL void DisplayVPWinVolumeIcon(BOOLEAN is_fresh)
{    

    MMI_IMAGE_ID_T  image_id = IMAGE_NULL;
    GUI_BG_T               fg_info = {0};     
#ifdef TOUCH_PANEL_SUPPORT //@andy.he_MP4PDA
   MMI_IMAGE_ID_T  impressed_id  = IMAGE_NULL;
#endif

    if (!MMK_IsOpenWin(MMIVP_PLAYER_WIN_ID))
    {
        return;
    }
    
    if (!IsAbleToFresh())
    {
        is_fresh = FALSE;
    }
     
    if (MMISET_VOL_ZERO == MMIAPISET_GetMultimVolume())
    {
#ifndef MMI_RES_LOW_COST        
        image_id = IMAGE_COMMON_PLAYER_BTN_VOL_MUTE;
#endif
#ifdef TOUCH_PANEL_SUPPORT //@andy.he_MP4PDA
        impressed_id = IMAGE_COMMON_PLAYER_BTN_VOL_MUTE_FOCUS;
#endif
    }
    else
    {
#ifndef MMI_RES_LOW_COST        
        image_id = IMAGE_COMMON_PLAYER_BTN_VOL;
#endif
#ifdef TOUCH_PANEL_SUPPORT //@andy.he_MP4PDA
        impressed_id = IMAGE_COMMON_PLAYER_BTN_VOL_FOCUS;
#endif
    }   

#ifdef TOUCH_PANEL_SUPPORT //@andy.he_MP4PDA
    fg_info.bg_type = GUI_BG_IMG;
    fg_info.img_id = impressed_id;
    GUIBUTTON_SetPressedFg(VP_PLAY_PANEL_VOL_BUTTON_CTRL_ID,&fg_info);
    fg_info.img_id = image_id;
    GUIBUTTON_SetFg(VP_PLAY_PANEL_VOL_BUTTON_CTRL_ID,&fg_info);      
#else
    fg_info.bg_type = GUI_BG_IMG;
    fg_info.img_id = image_id;
    GUIBUTTON_SetPressedFg(VP_PLAY_PANEL_VOL_BUTTON_CTRL_ID,&fg_info);
    GUIBUTTON_SetFg(VP_PLAY_PANEL_VOL_BUTTON_CTRL_ID,&fg_info);    
#endif

    if (is_fresh)
    {
        GUIBUTTON_Update(VP_PLAY_PANEL_VOL_BUTTON_CTRL_ID);
    }   
        
}

/*****************************************************************************/
//  Description : 显示视频播放器主界面快进快退图标
//  Global resource dependence : 
//  Author: apple.zhang
//  Note:
/*****************************************************************************/
LOCAL void DisplayVPWinBackwardOrForwardIcon(BOOLEAN is_fresh)
{
    GUI_BG_T           fg_info         = {0};
    BOOLEAN            is_gray         = TRUE; 
    
    if (!MMK_IsOpenWin(MMIVP_PLAYER_WIN_ID))
    {
        return;
    }

    if (!IsAbleToFresh())
    {
        is_fresh = FALSE;
    }    
    
    //VP_STRACE_LOW:"[VP] DisplayVPWinBackwardOrForwardIcon s_video_player_control_info.state = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_4048_112_2_18_3_5_36_441,(uint8*)"d",s_video_player_control_info.state);
   
    if (IsSeekEnable() 
        || VP_PLAY_FROM_LOCALLIST == s_video_player_control_info.play_type)
    {
        is_gray = FALSE;
    }    
    
    fg_info.bg_type = GUI_BG_IMG;

    if (is_gray)
    {
        fg_info.img_id = IMAGE_COMMON_BTN_PREV_GREY; 
#ifdef MMI_VIDEO_PLAYER_MINI_LAYOUT//6530 MINI 界面布局
        GUIBUTTON_SetFg(VP_PLAY_PANEL_PRE_BUTTON_CTRL_ID,&fg_info);
        GUIBUTTON_SetGrayedFg(VP_PLAY_PANEL_PRE_BUTTON_CTRL_ID,FALSE,PNULL,is_fresh);
#else
        GUIBUTTON_SetGrayedFg(VP_PLAY_PANEL_PRE_BUTTON_CTRL_ID,TRUE,&fg_info,is_fresh); 
#endif
        
        fg_info.img_id = IMAGE_COMMON_BTN_NEXT_GREY;
#ifdef MMI_VIDEO_PLAYER_MINI_LAYOUT//6530 MINI 界面布局
        GUIBUTTON_SetFg(VP_PLAY_PANEL_NEXT_BUTTON_CTRL_ID,&fg_info);
        GUIBUTTON_SetGrayedFg(VP_PLAY_PANEL_NEXT_BUTTON_CTRL_ID,FALSE,PNULL,is_fresh);
#else
        GUIBUTTON_SetGrayedFg(VP_PLAY_PANEL_NEXT_BUTTON_CTRL_ID,TRUE,&fg_info,is_fresh);
#endif

    }
    else
    {           
        fg_info.img_id = IMAGE_COMMON_BTN_PREV;
        GUIBUTTON_SetFg(VP_PLAY_PANEL_PRE_BUTTON_CTRL_ID,&fg_info);
#ifdef TOUCH_PANEL_SUPPORT //@andy.he_MP4PDA
        fg_info.img_id = IMAGE_COMMON_BTN_PREV_FOCUS;
        GUIBUTTON_SetPressedFg(VP_PLAY_PANEL_PRE_BUTTON_CTRL_ID,&fg_info);
#else
        GUIBUTTON_SetPressedFg(VP_PLAY_PANEL_PRE_BUTTON_CTRL_ID,&fg_info);
#endif
        GUIBUTTON_SetGrayedFg(VP_PLAY_PANEL_PRE_BUTTON_CTRL_ID,FALSE,PNULL,is_fresh);
        fg_info.img_id = IMAGE_COMMON_BTN_NEXT;
        GUIBUTTON_SetFg(VP_PLAY_PANEL_NEXT_BUTTON_CTRL_ID,&fg_info);
#ifdef TOUCH_PANEL_SUPPORT //@andy.he_MP4PDA
        fg_info.img_id = IMAGE_COMMON_BTN_NEXT_FOCUS;
        GUIBUTTON_SetPressedFg(VP_PLAY_PANEL_NEXT_BUTTON_CTRL_ID,&fg_info);
#else
        GUIBUTTON_SetPressedFg(VP_PLAY_PANEL_NEXT_BUTTON_CTRL_ID,&fg_info);
#endif       
        GUIBUTTON_SetGrayedFg(VP_PLAY_PANEL_NEXT_BUTTON_CTRL_ID,FALSE,PNULL,is_fresh);
        
    }   
}

/*****************************************************************************/
//  Description : 显示视频播放器主界面暂停或者播放图标
//  Global resource dependence : 
//  Author: apple.zhang
//  Note:
/*****************************************************************************/
LOCAL void DisplayVPWinPlayOrPauseIcon(BOOLEAN is_fresh)
{    
    GUI_BG_T    fg_info     = {0};
    BOOLEAN     is_gray     = FALSE;
#ifdef TOUCH_PANEL_SUPPORT //@andy.he_MP4PDA
   MMI_IMAGE_ID_T  impressed_id  = IMAGE_NULL;
#endif

    if (!MMK_IsOpenWin(MMIVP_PLAYER_WIN_ID))
    {
        return;
    }

    if (!IsAbleToFresh())
    {
        is_fresh = FALSE;
    }
     
    //VP_STRACE_LOW:"[VP] DisplayVPWinPlayOrPauseIcon s_video_player_control_info.state = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_4111_112_2_18_3_5_36_442,(uint8*)"d",s_video_player_control_info.state);

    switch(s_video_player_control_info.state) 
    { 
    case VP_STATE_READY:
    case VP_STATE_PAUSED:
        if (0 == s_video_player_control_info.full_path_len
           || MMIAPIVP_IsLiving())
        {
            is_gray = TRUE;
            fg_info.img_id = IMAGE_COMMON_BTN_PLAY_GREY;
        }
        else
        {
            fg_info.img_id = IMAGE_COMMON_BTN_PLAY;
#ifdef TOUCH_PANEL_SUPPORT //@andy.he_MP4PDA
            impressed_id  = IMAGE_COMMON_BTN_PLAY_FOCUS;
#endif     
        }
        break;

    case VP_STATE_SEEKING:
    case VP_STATE_RESUMING:
    case VP_STATE_PLAY_PENDING:
        is_gray = TRUE;
        fg_info.img_id = IMAGE_COMMON_BTN_PAUSE_GREY;
        break;
        
    case VP_STATE_BUFFERING:
    case VP_STATE_PLAYING:
        if (MMIAPIVP_IsLiving())
        {
            is_gray = TRUE;
            fg_info.img_id = IMAGE_COMMON_BTN_PAUSE_GREY;
        }
        else
        {
            fg_info.img_id = IMAGE_COMMON_BTN_PAUSE;
#ifdef TOUCH_PANEL_SUPPORT //@andy.he_MP4PDA
            impressed_id  = IMAGE_COMMON_BTN_PAUSE_FOCUS;
#endif
        }
        break;
 
    case VP_STATE_STOPING:
    case VP_STATE_PAUSING:
        is_gray = TRUE;
        fg_info.img_id = IMAGE_COMMON_BTN_PLAY_GREY;
        break;
 

    default:   
        //VP_STRACE_LOW:"[vp] DisplayVPWinPlayOrPauseIcon error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_4163_112_2_18_3_5_36_443,(uint8*)"");
        break;
    }       
    

    fg_info.bg_type = GUI_BG_IMG;

    if (!is_gray)
    {
#ifdef TOUCH_PANEL_SUPPORT //@andy.he_MP4PDA
        GUIBUTTON_SetFg(VP_PLAY_PANEL_PLAY_BUTTON_CTRL_ID,&fg_info); 
        fg_info.img_id = impressed_id;
        GUIBUTTON_SetPressedFg(VP_PLAY_PANEL_PLAY_BUTTON_CTRL_ID,&fg_info);
#else
        GUIBUTTON_SetPressedFg(VP_PLAY_PANEL_PLAY_BUTTON_CTRL_ID,&fg_info);
        GUIBUTTON_SetFg(VP_PLAY_PANEL_PLAY_BUTTON_CTRL_ID,&fg_info); 
#endif
    }    
#ifdef MMI_VIDEO_PLAYER_MINI_LAYOUT//6530 MINI 界面布局
    {
        GUIBUTTON_SetFg(VP_PLAY_PANEL_PLAY_BUTTON_CTRL_ID,&fg_info);
        GUIBUTTON_SetGrayedFg(VP_PLAY_PANEL_PLAY_BUTTON_CTRL_ID,FALSE,&fg_info,is_fresh);
    }
#else
     GUIBUTTON_SetGrayedFg(VP_PLAY_PANEL_PLAY_BUTTON_CTRL_ID,is_gray,&fg_info,is_fresh);
#endif

#ifdef MMI_VIDEO_PLAYER_MINI_LAYOUT//6530 MINI 界面布局
#if !( (defined MAINLCD_DEV_SIZE_220X176)||(defined MAINLCD_DEV_SIZE_160X128) ) /*MMI_VIDEO_PLAYER_SCREEN_H_SUPPORT*/ //6530_MINI 220X176 不需要刷新
     GUIBUTTON_SetBg(VP_PLAY_PANEL_PLAY_BUTTON_CTRL_ID, &fg_info);
     if (is_fresh)
     {
         MMK_SendMsg(MMIVP_PLAYER_WIN_ID, MSG_FULL_PAINT, PNULL);
     }
#endif
#endif

}

/*****************************************************************************/
//  Description : 显示默认图片
//  Global resource dependence : 
//  Author: apple.zhang
//  Note:
/*****************************************************************************/
LOCAL void DisplayVpWinDefaultPic(void)
{
    uint16   width = 0;
    uint16   height = 0;
    GUI_POINT_T dis_point = {0};
    GUI_RECT_T display_rect = {0};

    if (!s_is_full_paint)
    {
        return;
    }   
    
    if (s_video_player_control_info.is_display_win)
    {
        GUIOWNDRAW_GetDisplayRect(VP_PLAY_PANEL_VIDEO_OWNDRAW_CTRL_ID,&display_rect);        
    }
    else if(VP_SUPPORTED_AUDIO_ONLY == s_video_player_control_info.support_type)
    {
        display_rect = MMITHEME_GetFullScreenRect();
#ifdef CMCC_TEST_FLAG
        display_rect.top += MMITHEME_GetStatusBarHeight();
#endif
    }
    else
    {
        //VP_STRACE_LOW:"[VP] DisplayVpWinDefaultPic not display default pic"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_4211_112_2_18_3_5_36_444,(uint8*)"");
        return;
    }

    GUI_FillRect(MMITHEME_GetDefaultLcdDev(),display_rect,MMIAPIVP_GetTransparenceColor());

    if (VP_SUPPORTED_AUDIO_ONLY == s_video_player_control_info.support_type
        || VP_SUPPORTED_NEITHER == s_video_player_control_info.support_type
        || VP_STATE_READY == s_video_player_control_info.state
        || VP_STATE_PLAY_PENDING == s_video_player_control_info.state
        )
    {    
        GUIRES_GetImgWidthHeight(&width,&height,IMAGE_VP_DEFAULT_PIC,MMIVP_PLAYER_WIN_ID);
        
#ifdef MMI_VIDEO_PLAYER_MINI_LAYOUT //6530_MINI
//         dis_point.x = 50;
//         dis_point.y = 43;
        dis_point.x = (MMITHEME_GetMainScreenRightPixel() - width)/2;
#else
        dis_point.x = display_rect.left + (display_rect.right - display_rect.left + 1 - width)/2;
#endif

        dis_point.y = display_rect.top + (display_rect.bottom - display_rect.top + 1 - height)/2 ; 

        GUIRES_DisplayImg(&dis_point, PNULL, PNULL, MMIVP_PLAYER_WIN_ID, IMAGE_VP_DEFAULT_PIC, MMITHEME_GetDefaultLcdDev());
    }
}

/*****************************************************************************/
//  Description : 显示播放区域
//  Global resource dependence : 
//  Author: apple.zhang
//  Note:
/*****************************************************************************/
LOCAL void DisplayVpWinVideoArea(GUIOWNDRAW_INFO_T *owner_draw_ptr)
{
   
    GUI_RECT_T display_rect = {0};
    VP_DISPLAY_PARAM_T display_param = {0};
    BOOLEAN    is_full_screen = TRUE;
   
    
       
   
    if (PNULL == owner_draw_ptr)
    {
        GUIOWNDRAW_GetDisplayRect(VP_PLAY_PANEL_VIDEO_OWNDRAW_CTRL_ID,&display_rect);
        if (display_rect.right == display_rect.left)
        {
            return;
        }
    }
    else
    {
        display_rect.bottom = owner_draw_ptr->display_rect.bottom;
        display_rect.top = owner_draw_ptr->display_rect.top;
        display_rect.left = owner_draw_ptr->display_rect.left;
        display_rect.right = owner_draw_ptr->display_rect.right;
    }   
    
    //VP_STRACE_LOW:"[VP] DisplayVpWinVideoArea s_video_player_control_info.state = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_4261_112_2_18_3_5_36_445,(uint8*)"d",s_video_player_control_info.state);

    display_param.RotateAngle = GUILCD_GetLogicAngle(GUI_MAIN_LCD_ID);  
     
    if(s_is_lcd_switch && s_video_player_control_info.cur_angle != display_param.RotateAngle)
    {
        if (!MMITHEME_IsMainScreenLandscape())
        {   
            is_full_screen = FALSE;
        }            
        else if (LCD_ANGLE_90 == display_param.RotateAngle
            || LCD_ANGLE_270 == display_param.RotateAngle
            )
        {
            is_full_screen = TRUE;
        }
        else
        {
            is_full_screen = FALSE;
        }
    }
    
    if (VP_STATE_READY == s_video_player_control_info.state)       
    {   
        if(s_is_lcd_switch)
        {
            s_is_lcd_switch = FALSE;            
            if (s_video_player_control_info.cur_angle != display_param.RotateAngle)
            {
                //SwitchScreen(is_full_screen);
                DisappearVPWin(MMIVP_PLAYER_WIN_ID,FALSE,TRUE);  //切换屏幕，每次都需要显示控制面板
                s_is_refresh_switch = TRUE;
                if(s_video_player_control_info.is_display_win)
                {
                    DisappearVPWin(MMIVP_PLAYER_WIN_ID,FALSE,TRUE);
                }
            }
            s_video_player_control_info.cur_angle = display_param.RotateAngle;
        }
#ifdef  VIDEOTHUMBNAIL_SUPPORT  

        if(MMK_IsFocusWin(MMIVP_LOADING_LOCALLIST_WAIT_WIN_ID))
        {
             DisplayVpWinDefaultPic();	
        }
        else if(MMK_IsFocusWin(MMIVP_PLAYER_WIN_ID) )//|| MMK_IsFocusWin(MMIVP_OPT_MENU_WIN_ID)
        {
            if(!UILAYER_IsBltLayer(&s_thumbnail_layer))
            {
                DisplayVpWinDefaultPic();	
            }

            if (IsAbleToGetThumbnail() && s_video_player_control_info.vp_entry == VP_ENTRY_FROM_PLAYER )//VP当前有其他事件要处理，就不调用dplayer去获取thumbnail，避免冲突 
            {
                NotifyDPLAYERGetTumbnail(FALSE); 	
            }
        }
        s_is_get_focus=FALSE;
#else
         DisplayVpWinDefaultPic();	
#endif    

    }   
    else
    {   
        if(s_is_lcd_switch)
        {       
            s_is_lcd_switch = FALSE;        
            if(s_video_player_control_info.cur_angle != display_param.RotateAngle)
            {
                s_video_player_control_info.cur_angle = display_param.RotateAngle;
                //SwitchScreen(is_full_screen);
                s_is_refresh_switch = TRUE;
                DisappearVPWin(MMIVP_PLAYER_WIN_ID,FALSE,TRUE);  //切换屏幕，每次都需要显示控制面板
                StartVPWinDisplayerTimer();                
            }
        }  
        else
        {
            DisplayVpWinDefaultPic();
        }
    #ifdef  VIDEOTHUMBNAIL_SUPPORT
        if(VP_TO_DO_NONE == s_video_player_control_info.to_do_type && VP_STATE_PAUSED == s_video_player_control_info.state && MMK_IsFocusWin(MMIVP_PLAYER_WIN_ID) && s_is_get_focus && !s_is_refresh_switch)
        {
            NotifyDPLAYERGetTumbnail(TRUE); 
            s_is_get_focus=FALSE;
        }
    #endif

    }
    DisplayVPWinStation(TXT_NULL,TRUE,TRUE);
}


/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: apple.zhang
//  Note:
/*****************************************************************************/
LOCAL void DisplayVPWinFileIcon(BOOLEAN is_fresh)
{ 
#ifdef TOUCH_PANEL_SUPPORT  
    GUI_BG_T fg_info = {0};  
    
    if (!MMK_IsOpenWin(MMIVP_PLAYER_WIN_ID))
    {
        return;
    }
    
    if (!IsAbleToFresh())
    {
        is_fresh = FALSE;
    }

    fg_info.bg_type = GUI_BG_IMG;
    if (MMIAPIVP_IsLiving())
    {        
#if defined MMI_PDA_SUPPORT && defined(MMI_ISTYLE_SUPPORT)
        if (MMITHEME_IsIstyle() && !MMITHEME_IsMainScreenLandscape())
        {
            fg_info.img_id = IMAGE_COMMON_BTN_OPTION_GREY;
        }
        else
#endif
        {
#ifndef MMI_RES_LOW_COST            
            fg_info.img_id = IMAGE_COMMON_BTN_LIST_GREY;
#endif        
        }
        GUIBUTTON_SetGrayedFg(VP_PLAY_PANEL_FILE_BUTTON_CTRL_ID,TRUE,&fg_info,is_fresh);
    }
    else
    {        
#if defined MMI_PDA_SUPPORT && defined(MMI_ISTYLE_SUPPORT)
        if (MMITHEME_IsIstyle() && !MMITHEME_IsMainScreenLandscape())
        {
            fg_info.img_id = IMAGE_COMMON_BTN_OPTION;
        }
        else
#endif
        {
#ifndef MMI_RES_LOW_COST            
            fg_info.img_id = IMAGE_COMMON_BTN_LIST;
#endif        
        }

        GUIBUTTON_SetFg(VP_PLAY_PANEL_FILE_BUTTON_CTRL_ID,&fg_info);
#ifdef MMI_PDA_SUPPORT //@andy.he_MP4PDA
#if defined(MMI_ISTYLE_SUPPORT)
        if (MMITHEME_IsIstyle() && !MMITHEME_IsMainScreenLandscape())
        {
            fg_info.img_id = IMAGE_COMMON_BTN_OPTION_FOCUS;
        }
        else
#endif
        {
            fg_info.img_id = IMAGE_COMMON_BTN_LIST_FOCUS;
        }
        GUIBUTTON_SetPressedFg(VP_PLAY_PANEL_FILE_BUTTON_CTRL_ID,&fg_info);
#elif TOUCH_PANEL_SUPPORT    
        fg_info.img_id = IMAGE_COMMON_BTN_LIST_FOCUS;
        GUIBUTTON_SetPressedFg(VP_PLAY_PANEL_FILE_BUTTON_CTRL_ID,&fg_info);
#else          
        GUIBUTTON_SetPressedFg(VP_PLAY_PANEL_FILE_BUTTON_CTRL_ID,&fg_info);
#endif

        GUIBUTTON_SetGrayedFg(VP_PLAY_PANEL_FILE_BUTTON_CTRL_ID,FALSE,PNULL,is_fresh);  
    }
#endif

}

/*****************************************************************************/
//  Description : 显示视频播放器主界面进度条和时间地显示
//  Global resource dependence : 
//  Author: apple.zhang
//  Note:
/*****************************************************************************/
LOCAL void DisplayVPWinProgress(BOOLEAN is_fresh)
{ 

    GUI_BG_T        bg_info = {0};

    if (!MMK_IsFocusWin(MMIVP_PLAYER_WIN_ID))  
    {
        return;
    }

    bg_info.bg_type = GUI_BG_IMG;
    bg_info.img_id = IMAGE_VP_PROCESSBAR_BG;
    GUIOWNDRAW_SetBg(VP_PLAY_PANEL_PROCESS_OWNDRAW_CTRL_ID, &bg_info);
    GUIOWNDRAW_Update(VP_PLAY_PANEL_PROCESS_OWNDRAW_CTRL_ID);

#if (!defined MMI_VIDEO_PLAYER_MINI_LAYOUT)&&(defined VIDEOTHUMBNAIL_SUPPORT)
    bg_info.bg_type = GUI_BG_IMG;
    bg_info.img_id = IMAGE_VP_BTN_BG;
    GUIFORM_SetBg(VP_PLAY_PANEL_BUTTON_FORM_CTRL_ID, &bg_info);
#endif

}

/*****************************************************************************/
//  Description : 显示视频播放器主界面进度条和时间地显示
//  Global resource dependence : 
//  Author: apple.zhang
//  Note:
/*****************************************************************************/
LOCAL void DisplayVPWinProgressEx(GUIOWNDRAW_INFO_T *owner_draw_ptr)
{
    GUI_RECT_T          image_rect = {0};   
    uint32              process = 0;
    GUI_POINT_T         dis_point = {0};   
    uint16              process_height = 0;
    uint16              process_width = 0;
    uint16              slide_height = 0;
    uint16              slide_width = 0;
    GUI_RECT_T          display_rect = {0};
    
 
   if (!IsAbleToFresh() && !MMK_IsFocusWin(MMIVP_LOADING_LOCALLIST_WAIT_WIN_ID))
    {
        return;
    }

    //VP_STRACE_LOW("[VP] DisplayVPWinProgressEx state = %d", s_video_player_control_info.state);
    if (PNULL == owner_draw_ptr)
    {
        GUIOWNDRAW_GetDisplayRect(VP_PLAY_PANEL_PROCESS_OWNDRAW_CTRL_ID ,&display_rect);
        if (display_rect.right == display_rect.left)
        {
            return;
        }
    }   
    else
    {
        display_rect.bottom = owner_draw_ptr->display_rect.bottom;
        display_rect.top = owner_draw_ptr->display_rect.top;
        display_rect.left = owner_draw_ptr->display_rect.left;
        display_rect.right = owner_draw_ptr->display_rect.right;
    }      


        display_rect.top   += VP_OWNERDRAW_PROCESS_TOPMARGIN;
        display_rect.left  += VP_OWNERDRAW_PROCESS_LEFTMARGIN;
        display_rect.right -= VP_OWNERDRAW_PROCESS_LEFTMARGIN;


    //VP_STRACE_LOW:"[VP] DisplayVPWinProgressEx process_time = %d, total_time = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_4481_112_2_18_3_5_37_446,(uint8*)"dd",s_video_player_control_info.process_time,s_video_player_control_info.total_time);
   
    if (s_video_player_control_info.process_time > s_video_player_control_info.total_time)
    {
        s_video_player_control_info.process_time = s_video_player_control_info.total_time;
    }

   
    GUIRES_GetImgWidthHeight(&process_width,&process_height,IMAGE_COMMON_PLAYER_PROCESS_GROOVE,MMIVP_PLAYER_WIN_ID);
    GUIRES_GetImgWidthHeight(&slide_width,&slide_height,IMAGE_COMMON_PLAYER_PROCESS_SLIDE,MMIVP_PLAYER_WIN_ID);

    
    process_width = display_rect.right - display_rect.left - slide_width+ 1;
    


    if((1 * VP_SEC_TO_MSEL) <= s_video_player_control_info.total_time && !MMIAPIVP_IsLiving())
    {     
        process = ((s_video_player_control_info.process_time/VP_SEC_TO_MSEL) * (uint32)process_width) / (s_video_player_control_info.total_time/VP_SEC_TO_MSEL);
    } 
    else if (0 < s_video_player_control_info.process_time && !MMIAPIVP_IsLiving())
    {
        process = process_width;
    }

    /** show process bg slot **/      
   // VP_STRACE_LOW("[VP] DisplayVPWinProgress process = %d, width = %d, right = %d, left = %d",process,width,display_rect.right,display_rect.left);
    
    display_rect.top = display_rect.top + (slide_height - process_height)/2;
    display_rect.bottom = display_rect.top + process_height;
    GUIRES_DisplayImg(PNULL, &display_rect, PNULL, MMIVP_PLAYER_WIN_ID, IMAGE_COMMON_PLAYER_PROCESS_GROOVE, MMITHEME_GetDefaultLcdDev());
    
    /** show process **/  
    if(process > 0)
    {           
        image_rect.left = display_rect.left;
        image_rect.right = MIN(image_rect.left + (uint16)process, display_rect.right);
        image_rect.top = display_rect.top;
        image_rect.bottom = display_rect.bottom;
        GUIRES_DisplayImg(PNULL, &image_rect, PNULL, MMIVP_PLAYER_WIN_ID, IMAGE_COMMON_PLAYER_PROCESS, MMITHEME_GetDefaultLcdDev());

    }

    display_rect.top = display_rect.top - (slide_height - process_height)/2;
    display_rect.bottom = display_rect.top + slide_height;
    /** show slide **/   
    if (!MMIAPIVP_IsLiving())  //直播不显示slide
    {   
        //GUIRES_GetImgWidthHeight(&process_width,&process_height,IMAGE_COMMON_PLAYER_PROCESS_SLIDE,MMIVP_PLAYER_WIN_ID);
        //display_rect.left = display_rect.left;// - process_width/2;
        //display_rect.right = display_rect.right;        

        dis_point.x = MIN((uint16)process + display_rect.left, display_rect.right - slide_width+1);    
        dis_point.y = display_rect.top;
        //display_rect.left = display_rect.left; //+ process_width/2;
        GUIRES_DisplayImg(&dis_point, PNULL, PNULL, MMIVP_PLAYER_WIN_ID, IMAGE_COMMON_PLAYER_PROCESS_SLIDE, MMITHEME_GetDefaultLcdDev());    
   
    }

    display_rect.top = display_rect.bottom + VP_TIME_PROCESS_DISTANCE;
    display_rect.bottom = display_rect.top + VP_TIME_FONT;
    DisplayVPWinProcessTime(display_rect);    
}


/*****************************************************************************/
//  Description : 显示视频播放器主界面时间显示
//  Global resource dependence : 
//  Author: apple.zhang
//  Note:
/*****************************************************************************/
LOCAL void DisplayVPWinProcessTime(GUI_RECT_T progress_rect)
{
    uint8 cur_time_str[VP_DISPLAY_TIME_LENGTH] = {0};   
    wchar wchar_cur_time_str[VP_DISPLAY_TIME_LENGTH] = {0}; 
    uint8 total_time_str[VP_DISPLAY_TIME_LENGTH] = {0}; 
    wchar wchar_total_time_str[VP_DISPLAY_TIME_LENGTH] = {0};   
    MMI_STRING_T cur_str_t = {0};
    MMI_STRING_T total_str_t = {0};
    GUISTR_STYLE_T      text_style      = {0};  /*lint !e64*/
    GUISTR_STATE_T      state =         GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE|GUISTR_STATE_EFFECT; 
    GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    GUI_RECT_T   cur_time_rect = {0};
    GUI_RECT_T   total_time_rect = {0};
    uint32       total_time      = 0;
    GUISTR_INFO_T str_info = {0};
    

    if (VP_MAX_INVALID_VALUE != s_video_player_control_info.total_time )
    {
        total_time  = s_video_player_control_info.total_time;
    }

    if(total_time < VP_SEC_TO_MSEL && total_time > 0)
    {
        //cur_str_t.wstr_len = sprintf((char*)cur_time_str, "%.2ld:%.2ld", 
         //   (s_video_player_control_info.process_time/VP_SEC_TO_MSEL)/VP_MIN_TO_SEC, 
         //   (s_video_player_control_info.process_time /VP_SEC_TO_MSEL)%VP_HOUR_TO_MIN);
        if(s_video_player_control_info.process_time != 0)
        {
            cur_str_t.wstr_len = sprintf((char*)cur_time_str, "%.2ld:%.2ld",0, 1);
        }
        else
        {
            cur_str_t.wstr_len = sprintf((char*)cur_time_str, "%.2ld:%.2ld",0, 0);
        }
        total_str_t.wstr_len = sprintf((char*)total_time_str, "%.2ld:%.2ld",0, 1);
    }
    else if((total_time/VP_SEC_TO_MSEL)/VP_MIN_TO_SEC < VP_HOUR_TO_MIN 
        && (s_video_player_control_info.process_time/VP_SEC_TO_MSEL)/VP_MIN_TO_SEC < VP_HOUR_TO_MIN )
    {   
        cur_str_t.wstr_len = sprintf((char*)cur_time_str, "%.2ld:%.2ld", 
            (s_video_player_control_info.process_time/VP_SEC_TO_MSEL)/VP_MIN_TO_SEC, 
            (s_video_player_control_info.process_time /VP_SEC_TO_MSEL)%VP_HOUR_TO_MIN);
        total_str_t.wstr_len = sprintf((char*)total_time_str, "%.2ld:%.2ld", 
            (total_time/VP_SEC_TO_MSEL)/VP_MIN_TO_SEC, 
            (total_time/VP_SEC_TO_MSEL)%VP_HOUR_TO_MIN);       
    }
    else
    {
        cur_str_t.wstr_len = sprintf((char*)cur_time_str, "%.2ld:%.2ld:%.2ld", 
            ((s_video_player_control_info.process_time/VP_SEC_TO_MSEL)/VP_MIN_TO_SEC)/VP_HOUR_TO_MIN, 
            ((s_video_player_control_info.process_time/VP_SEC_TO_MSEL)/VP_MIN_TO_SEC)%VP_HOUR_TO_MIN, 
            (s_video_player_control_info.process_time/VP_SEC_TO_MSEL)%VP_HOUR_TO_MIN);
        total_str_t.wstr_len = sprintf((char*)total_time_str, "%.2ld:%.2ld:%.2ld", 
            ((total_time/VP_SEC_TO_MSEL)/VP_MIN_TO_SEC)/VP_HOUR_TO_MIN,
            ((total_time/VP_SEC_TO_MSEL)/VP_MIN_TO_SEC)%VP_HOUR_TO_MIN, 
            (total_time/VP_SEC_TO_MSEL)%VP_HOUR_TO_MIN);      
    }   
    MMIAPICOM_StrToWstr((uint8*)cur_time_str,(wchar *)wchar_cur_time_str );
    cur_str_t.wstr_ptr = wchar_cur_time_str;           
    MMIAPICOM_StrToWstr((uint8*)total_time_str,(wchar *)wchar_total_time_str );
    total_str_t.wstr_ptr = wchar_total_time_str;           

    text_style.align = ALIGN_LEFT;
    text_style.font = VP_TIME_FONT;
    text_style.effect = FONT_EFFECT_REVERSE_EDGE;
    
    if (MMI_WHITE_COLOR == MMIAPIVP_GetTransparenceColor())
    {
        text_style.font_color = MMI_BACKGROUND_COLOR;
        text_style.edge_color = MMI_GRAY_WHITE_COLOR;
    }
    else
    {
        text_style.font_color = MMI_GRAY_WHITE_COLOR;
        text_style.edge_color = MMI_BACKGROUND_COLOR;
    }
    
    GUISTR_GetStringInfo(&text_style,&cur_str_t,state,&str_info);
    cur_time_rect.left = progress_rect.left;
    
#ifdef MMI_VIDEO_PLAYER_MINI_LAYOUT
    cur_time_rect.top = progress_rect.top - VP_TIME_ADJUST; //0 leiming ma
    cur_time_rect.bottom = cur_time_rect.top + VP_TIME_FONT;
#else
    cur_time_rect.top = progress_rect.top;
    cur_time_rect.bottom = cur_time_rect.top + str_info.height - 1;
#endif
    cur_time_rect.right = cur_time_rect.left + str_info.width - 1;//NEWMS00176580
    cur_time_rect.right = cur_time_rect.left + GUISTR_GetStringWidth(&text_style,&cur_str_t,state);//NEWMS00176580
   
    // display   
    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_RECT_T      *)&cur_time_rect,       //the fixed display area
        (const GUI_RECT_T      *)&cur_time_rect,       //用户要剪切的实际区域
        (const MMI_STRING_T    *)&cur_str_t,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        ); 
    
    text_style.align = ALIGN_RIGHT;      
    total_time_rect.right = progress_rect.right;
    total_time_rect.left = total_time_rect.right - GUISTR_GetStringWidth(&text_style,&total_str_t,state);//NEWMS00176580
    total_time_rect.top = cur_time_rect.top;
    total_time_rect.bottom = cur_time_rect.bottom;
 
    //GUI_FillRect(MMITHEME_GetDefaultLcdDev(),total_time_rect,MMIAPIVP_GetTransparenceColor());
    // display

    if (!MMIAPIVP_IsLiving())  //直播不显示总时间
    {    
        GUISTR_DrawTextToLCDInRect( 
            (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
            (const GUI_RECT_T      *)&total_time_rect,       //the fixed display area
            (const GUI_RECT_T      *)&total_time_rect,       //用户要剪切的实际区域
            (const MMI_STRING_T    *)&total_str_t,
            &text_style,
            state,
            GUISTR_TEXT_DIR_AUTO
            ); 
    }
    if( (s_video_player_control_info.process_time == s_video_player_control_info.total_time) && (s_video_player_control_info.total_time != 0))
    {
        MMITHEME_UpdateRect();
    }
    
}

/*****************************************************************************/
//  Description : 显示视频播放器切换屏幕图标
//  Global resource dependence : 
//  Author: apple.zhang
//  Note:
/*****************************************************************************/
LOCAL void DisplayVPWinSwitchScreenIcon(BOOLEAN is_fresh)
{ 

    if (IsDisplayFullScreenReferInfoEnable())
    {

#ifdef TOUCH_PANEL_SUPPORT
        GUI_BG_T   fg_info = {0};
       // BOOLEAN    is_gray = !IsSwitchFullScreenEnable();andy.he_crNEWMS00112870

        MMI_IMAGE_ID_T  impressed_id  = IMAGE_NULL;
 
   

        if (!MMK_IsOpenWin(MMIVP_PLAYER_WIN_ID))
        {
            return;
        }

        if (!IsAbleToFresh())
        {
            is_fresh = FALSE;
        }

        fg_info.bg_type = GUI_BG_IMG;    
    
        if(MMK_IsWindowLandscape(MMIVP_PLAYER_WIN_ID))
        {
#ifndef MMI_RES_LOW_COST            
            fg_info.img_id = IMAGE_COMMON_BTN_FULLSCREEN_OFF; 
#endif            
            #ifdef   MMI_PDA_SUPPORT  
            impressed_id=IMAGE_COMMON_BTN_FULLSCREEN_OFF_FOCUS;
            #endif  
        }
        else
        {
#ifndef MMI_RES_LOW_COST            
            fg_info.img_id = IMAGE_COMMON_BTN_FULLSCREEN; 
#endif            
            #ifdef   MMI_PDA_SUPPORT  
            impressed_id=IMAGE_COMMON_BTN_FULLSCREEN_FOCUS;
            #endif   
        }

            GUIBUTTON_SetFg(VP_PLAY_PANEL_FULLSCREEN_BUTTON_CTRL_ID,&fg_info);
            #ifdef   TOUCH_PANEL_SUPPORT
            fg_info.img_id=impressed_id;
            GUIBUTTON_SetPressedFg(VP_PLAY_PANEL_FULLSCREEN_BUTTON_CTRL_ID,&fg_info);
            #else
            GUIBUTTON_SetPressedFg(VP_PLAY_PANEL_FULLSCREEN_BUTTON_CTRL_ID,&fg_info);
            #endif
            GUIBUTTON_SetGrayedFg(VP_PLAY_PANEL_FULLSCREEN_BUTTON_CTRL_ID,FALSE,PNULL,is_fresh);

#endif
    }
}

/*****************************************************************************/
//  Description : 使视频播放器主界面消失
//  Global resource dependence : 
//  Author: apple.zhang
//  Note:
/*****************************************************************************/
LOCAL void DisappearVPWin(MMI_WIN_ID_T  win_id,
                          BOOLEAN is_disappear,
                          BOOLEAN is_fresh)
{
    GUI_RECT_T lcd_rect = {0};
    
    IGUICTRL_T* ctrl_handle = PNULL;    
    
    
   
    //VP_STRACE_LOW:"[VP] DisappearVPWin is_disappear = %d, s_video_player_control_info.is_display_win = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_4739_112_2_18_3_5_37_447,(uint8*)"dd",is_disappear, s_video_player_control_info.is_display_win);
    
    if (!MMK_IsFocusWin(MMIVP_PLAYER_WIN_ID))
    {
        return;
    }    

    if (!s_is_full_paint)
    {
        is_fresh = FALSE;
    }
    
    if (is_disappear)
    {       
        StopPlayerAllTimer();       //停止所有timer     
        if (s_video_player_control_info.is_display_win)  //如果有显示变为不显示，设置可见
        {
            s_video_player_control_info.is_display_win = FALSE;
#ifdef MMI_VIDEO_PLAYER_MINI_LAYOUT//6530 MINI 界面布局 //6530_MINI 去掉了form，改用button
        MMK_PostMsg(VP_PLAY_NAME_LABEL_CTRL_ID, MSG_CTL_GET_ACTIVE, PNULL, PNULL);
        GUIBUTTON_SetVisible(VP_PLAY_PANEL_RANDOM_BUTTON_CTRL_ID, FALSE, is_fresh);
        GUIBUTTON_SetVisible(VP_PLAY_PANEL_REPEAT_BUTTON_CTRL_ID, FALSE, is_fresh);
        GUIBUTTON_SetVisible(VP_PLAY_PANEL_PRE_BUTTON_CTRL_ID, FALSE, is_fresh);
        GUIBUTTON_SetVisible(VP_PLAY_PANEL_PLAY_BUTTON_CTRL_ID, FALSE, is_fresh);
        GUIBUTTON_SetVisible(VP_PLAY_PANEL_NEXT_BUTTON_CTRL_ID, FALSE, is_fresh);
        GUILABEL_SetVisible(VP_PLAY_NAME_LABEL_CTRL_ID, FALSE, is_fresh);
        GUIOWNDRAW_SetVisible(VP_PLAY_PANEL_PROCESS_OWNDRAW_CTRL_ID, FALSE, is_fresh);
        GUIOWNDRAW_SetVisible(VP_PLAY_PANEL_VIDEO_OWNDRAW_CTRL_ID, FALSE, is_fresh);
#if (defined MAINLCD_DEV_SIZE_220X176) || (defined MAINLCD_DEV_SIZE_160X128) //6530_MINI 220X176
        GUIBUTTON_SetVisible(VP_PLAY_PANEL_OPT_BUTTON_CTRL_ID, FALSE, is_fresh);
        GUIBUTTON_SetVisible(VP_PLAY_PANEL_RETURN_BUTTON_CTRL_ID, FALSE, is_fresh);
#endif 

        if (is_fresh)
        {
            MMK_SendMsg(MMIVP_PLAYER_WIN_ID,MSG_FULL_PAINT,PNULL);
        }
        
#else
            GUIFORM_SetVisible(VP_PLAY_PANEL_FORM_CTRL_ID,FALSE,is_fresh);            

#endif  //MMI_VIDEO_PLAYER_MINI_SUPPORT //6530_MINI 去掉了form，改用button
        }        
        if (!MMITHEME_IsMainScreenLandscape())   //竖屏需要设置softkey
        {     
            GUIWIN_SetSoftkeyTPEnable(win_id, FALSE);
            GUIWIN_SetSoftkeyVisible(win_id,FALSE);    
            
        }    

        //设置状态栏不可见
        ctrl_handle = MMK_GetCtrlPtrByWin(MMK_GetWinHandle(MMK_GetFirstAppletHandle(), win_id), MMITHEME_GetStatusBarCtrlId());
        if(0 != ctrl_handle)
        {
#ifndef CMCC_TEST_FLAG
            IGUICTRL_SetState(ctrl_handle, GUICTRL_STATE_DISABLE_ACTIVE | GUICTRL_STATE_INVISIBLE | GUICTRL_STATE_DISABLE_TP, TRUE);
#endif
        }
        
        lcd_rect = MMITHEME_GetFullScreenRect();  
#ifdef CMCC_TEST_FLAG
        lcd_rect.top += MMITHEME_GetStatusBarHeight();
#endif
        GUI_FillRect(MMITHEME_GetDefaultLcdDev(), lcd_rect, MMIAPIVP_GetTransparenceColor());
        
        //SCI_TRACE_LOW:"[VP] DisappearVPWin s_video_player_control_info.support_type = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_4776_112_2_18_3_5_37_448,(uint8*)"d",s_video_player_control_info.support_type);
        if (VP_SUPPORTED_AUDIO_ONLY == s_video_player_control_info.support_type
            || VP_SUPPORTED_NEITHER == s_video_player_control_info.support_type)
        {        
            DisplayVpWinDefaultPic(); 
        }
    }
    else 
    {
        s_video_player_control_info.is_display_win = TRUE;          
        HandleProcessTimer(); //立即更新进度条，防止反复操作切屏进度条不更新
        if (!MMITHEME_IsMainScreenLandscape())
        {            
            GUIWIN_SetSoftkeyTPEnable(win_id, TRUE);
            GUIWIN_SetSoftkeyVisible(win_id,TRUE);
        }
        
#ifdef MMI_VIDEO_PLAYER_MINI_LAYOUT//6530 MINI 界面布局 //6530_MINI 去掉了form，改用button
        MMK_PostMsg(VP_PLAY_NAME_LABEL_CTRL_ID, MSG_CTL_GET_ACTIVE, PNULL, PNULL);
        GUIBUTTON_SetVisible(VP_PLAY_PANEL_RANDOM_BUTTON_CTRL_ID, TRUE, is_fresh);
        GUIBUTTON_SetVisible(VP_PLAY_PANEL_REPEAT_BUTTON_CTRL_ID, TRUE, is_fresh);
        GUIBUTTON_SetVisible(VP_PLAY_PANEL_PRE_BUTTON_CTRL_ID, TRUE, is_fresh);
        GUIBUTTON_SetVisible(VP_PLAY_PANEL_PLAY_BUTTON_CTRL_ID, TRUE, is_fresh);
        GUIBUTTON_SetVisible(VP_PLAY_PANEL_NEXT_BUTTON_CTRL_ID, TRUE, is_fresh);
        GUILABEL_SetVisible(VP_PLAY_NAME_LABEL_CTRL_ID, TRUE, is_fresh);
        GUIOWNDRAW_SetVisible(VP_PLAY_PANEL_PROCESS_OWNDRAW_CTRL_ID, TRUE, is_fresh);
        GUIOWNDRAW_SetVisible(VP_PLAY_PANEL_VIDEO_OWNDRAW_CTRL_ID, TRUE, is_fresh);
#if (defined MAINLCD_DEV_SIZE_220X176) || (defined MAINLCD_DEV_SIZE_160X128) //6530_MINI 220X176
        GUIBUTTON_SetVisible(VP_PLAY_PANEL_OPT_BUTTON_CTRL_ID, TRUE, is_fresh);
        GUIBUTTON_SetVisible(VP_PLAY_PANEL_RETURN_BUTTON_CTRL_ID, TRUE, is_fresh);
#endif  

        if (is_fresh)
        {
            MMK_SendMsg(MMIVP_PLAYER_WIN_ID,MSG_FULL_PAINT,PNULL);
        }
#else
        GUIFORM_SetVisible(VP_PLAY_PANEL_FORM_CTRL_ID,TRUE,is_fresh);

#endif  //MMI_VIDEO_PLAYER_MINI_LAYOUT //6530_MINI 去掉了form，改用button

        ctrl_handle = MMK_GetCtrlPtrByWin(MMK_GetWinHandle(MMK_GetFirstAppletHandle(), win_id), MMITHEME_GetStatusBarCtrlId());
        if(0 != ctrl_handle)
        {
            GUICTRL_SetState(ctrl_handle, GUICTRL_STATE_DISABLE_ACTIVE | GUICTRL_STATE_INVISIBLE | GUICTRL_STATE_DISABLE_TP, FALSE);
        }
       
        if (is_fresh)
        {
            GUIWIN_ReDrawStbCtrl(win_id,TRUE);  
            if (!MMITHEME_IsMainScreenLandscape())
            {
                GUISOFTKEY_Update(win_id,MMITHEME_GetSoftkeyCtrlId());
            }
            DisplayVPWinStation(TXT_NULL,TRUE,TRUE);
        }
        
    }

    if (is_fresh)
    {
        MMIAPICOM_UpdatePanel();  //如果打开音量/亮度控制面板，则需要更新
    }    
}


/*****************************************************************************/
//  Description : 获取最后一次播放的信息
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN LoadLastPlayingFile(void)
{   
#ifdef MMI_VIDEOPLAYER_HISTORYLIST_SUPPORT
    const VP_HISTORY_ITEM_T   *history_info_ptr = PNULL;    

    history_info_ptr = VP_HistoryList_GetInfo(0);        
    if (PNULL != history_info_ptr && (PNULL != history_info_ptr->local_movie_file_info_ptr || PNULL != history_info_ptr->net_movie_info_ptr))
    {  
        
        SCI_MEMSET(s_video_player_control_info.full_path_name,0,sizeof((VP_MAX_NAME_LEN +1)*sizeof(wchar)));
        SCI_MEMSET(s_video_player_control_info.url,0,VP_MAX_URL_LEN +1);
        //VP_STRACE_LOW:"[VP] LoadLastPlayingFile history_info_ptr->locallist_type = %d, history_info_ptr->locallist_index = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_4835_112_2_18_3_5_37_449,(uint8*)"dd", history_info_ptr->locallist_type,history_info_ptr->locallist_index);
            
        if (VP_PLAY_FROM_LOCALLIST == history_info_ptr->type && PNULL != history_info_ptr->local_movie_file_info_ptr)
        {      
            //本地播放列表文件，查找在本地播放列表的相应位置上是否依然存在
            if (!CheckHistoryListItemIsHasValidLocalListInfo(history_info_ptr->locallist_type,history_info_ptr->locallist_index,history_info_ptr->local_movie_file_info_ptr->full_path_ptr))
            {
                return FALSE;             
            } 
            s_video_player_control_info.full_path_len = MIN(VP_MAX_NAME_LEN,history_info_ptr->local_movie_file_info_ptr->full_path_len);
            MMIAPICOM_Wstrncpy(s_video_player_control_info.full_path_name,
                history_info_ptr->local_movie_file_info_ptr->full_path_ptr,
                s_video_player_control_info.full_path_len);
        } 
        else if (VP_PLAY_FROM_NET == history_info_ptr->type && PNULL != history_info_ptr->net_movie_info_ptr)
        {            
            if (0 != history_info_ptr->net_movie_info_ptr->title_len)
            {
                s_video_player_control_info.full_path_len = MIN(VP_MAX_NAME_LEN,history_info_ptr->net_movie_info_ptr->title_len);
                MMIAPICOM_Wstrncpy(s_video_player_control_info.full_path_name,
                    history_info_ptr->net_movie_info_ptr->title_arr,
                    s_video_player_control_info.full_path_len);
            }                        
            SCI_MEMCPY(s_video_player_control_info.url, history_info_ptr->net_movie_info_ptr->url_ptr,VP_MAX_URL_LEN);
        }
        else if(PNULL != history_info_ptr->local_movie_file_info_ptr)
        {              
            if (MMIAPIFMM_IsFileExist(history_info_ptr->local_movie_file_info_ptr->full_path_ptr, history_info_ptr->local_movie_file_info_ptr->full_path_len))
            {
                s_video_player_control_info.full_path_len = MIN(history_info_ptr->local_movie_file_info_ptr->full_path_len,VP_MAX_NAME_LEN);
                MMIAPICOM_Wstrncpy(s_video_player_control_info.full_path_name,
                    history_info_ptr->local_movie_file_info_ptr->full_path_ptr,
                    s_video_player_control_info.full_path_len);
            } 
            else
            {
                SCI_TRACE_LOW("[VP] LoadLastPlayingFile, file does not exists!");
                return FALSE;
            }
        }            
        else
        {
            return FALSE;
        }

        VP_LocalList_SetCurListType(history_info_ptr->locallist_type);
        VP_LocalList_SetCurrentIndex(history_info_ptr->locallist_index);
        s_video_player_control_info.process_time = history_info_ptr->last_play_point; 
        s_video_player_control_info.seek_time = history_info_ptr->last_play_point;
        s_video_player_control_info.total_time = history_info_ptr->total_time;
        s_video_player_control_info.play_type = history_info_ptr->type;

        //如果上次结束时是在已经播放完毕，则设置当前播放时间为0
        if (s_video_player_control_info.total_time == s_video_player_control_info.process_time)
        {
            s_video_player_control_info.process_time = 0;
            s_video_player_control_info.seek_time = 0;
        }
        //VP_STRACE_LOW:"[VP] LoadLastPlayingFile process = %d, total_time = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_4885_112_2_18_3_5_38_450,(uint8*)"dd",s_video_player_control_info.process_time, s_video_player_control_info.total_time);
        DisplayVPWinRepeatModeIcon(TRUE);
        DisplayVPWinRandomModeIcon(TRUE);
        DisplayVPWinTitle(TRUE); 
        DisplayVPWinBackwardOrForwardIcon(TRUE);    
        if (0 == s_video_player_control_info.full_path_len) //当前没有文件
        {
            DisplayVPWinStation(TXT_VP_NO_FILE,TRUE,FALSE);         
        }
        else
        {
            DisplayVPWinStation(TXT_NULL,TRUE,FALSE);
        }
        DisplayVPWinPlayOrPauseIcon(TRUE);
        DisplayVPWinSoftkey(TRUE);
        #ifdef MMI_PDA_SUPPORT//istyle_return
        DisplayVPWinReturnIcion(TRUE);
        #endif
        return TRUE;
    }
#endif
    return FALSE;

}


/*****************************************************************************/
//  Description : 获取前一个本地播放列表视频
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN  GetPreLocalListFileName(wchar  *full_path_name,uint16   *full_path_len)
{   
    BOOLEAN found_result = FALSE;

    if(full_path_name == NULL ||  full_path_len == NULL)
    {
        return found_result;
    }
    
    if(VP_PLAY_FROM_LOCALLIST == s_video_player_control_info.play_type)
    {    
        SCI_MEMSET(full_path_name,0,((VP_MAX_NAME_LEN +1)*sizeof(wchar)));
        if (!VP_Setting_GetIsRandomPlayMode())
        {
            //play video file according to sequence when you pres left key.
            found_result = VP_LocalList_GetPrev(full_path_name,full_path_len);

        }
        else
        {
            //play video file by random when you press left key.
            found_result = VP_LocalList_GetPreByMode(full_path_name, full_path_len,VP_Setting_GetRepeatPlayMode(),TRUE);
            if(!found_result)
            {   
                VP_LocalList_DestroyPlayList();
            } 
        }    
    }

        return found_result;

}



/*****************************************************************************/
//  Description : 获取下一个本地播放列表视频
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN  GetNextLocalListFileName(wchar  *full_path_name,uint16   *full_path_len)
{   
    BOOLEAN found_result = FALSE;

    if(full_path_name == NULL  || full_path_len == NULL)
    {
        return found_result;
    }
    
    if(VP_PLAY_FROM_LOCALLIST == s_video_player_control_info.play_type)
    {    
        SCI_MEMSET(full_path_name,0,((VP_MAX_NAME_LEN +1)*sizeof(wchar)));
        if (!VP_Setting_GetIsRandomPlayMode())
        {
            //play video file according to sequence when you pres left key.
            found_result = VP_LocalList_GetNext(full_path_name,full_path_len);

        }
        else
        {
            //play video file by random when you press left key.
            found_result = VP_LocalList_GetNextByMode(full_path_name, full_path_len,VP_Setting_GetRepeatPlayMode(),TRUE);
            if(!found_result)
            {   
                VP_LocalList_DestroyPlayList();
            } 
        }    
    }

        return found_result;

}

/*****************************************************************************/
//  Description : 获取前一个本地播放列表信息
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void LoadPreLocalListFile(void)
{   
    DPLAYER_MEDIA_INFO_T media_info = {0};        
    uint16          full_path_len = VP_MAX_NAME_LEN;
    wchar           full_path_name [VP_MAX_NAME_LEN +1] = {0}; 
    BOOLEAN found_next = FALSE;
    
     found_next = GetPreLocalListFileName(full_path_name,&full_path_len);   

    if (found_next)
    {  
        SCI_MEMSET(s_video_player_control_info.full_path_name,0,((VP_MAX_NAME_LEN +1)*sizeof(wchar)));
        SCI_MEMSET(s_video_player_control_info.url,0,VP_MAX_URL_LEN +1);   
        
        s_video_player_control_info.full_path_len = full_path_len;
        MMIAPICOM_Wstrncpy(s_video_player_control_info.full_path_name,
                full_path_name,
                s_video_player_control_info.full_path_len);
        
        s_video_player_control_info.process_time = 0; 

    if(VP_STATE_READY == s_video_player_control_info.state 
           && (VP_PLAY_FROM_FILE == s_video_player_control_info.play_type 
           || VP_PLAY_FROM_LOCALLIST == s_video_player_control_info.play_type))
        {
            MMIAPIVP_GetInfoFromFullPathName(s_video_player_control_info.full_path_name, s_video_player_control_info.full_path_len, &media_info);
            s_video_player_control_info.total_time = media_info.media_length;   
        }
        else
        {
            s_video_player_control_info.total_time = 0;   
        }

        s_video_player_control_info.seek_time = 0;
        s_video_player_control_info.station_text_id= 0;
        s_video_player_control_info.support_type = VP_SUPPORTED_BOTH;
 #ifdef  VIDEOTHUMBNAIL_SUPPORT          
         if(UILAYER_IsBltLayer(&s_thumbnail_layer))
       {
              DestroyLandscapeThumbnailLayer(); 
        }
#endif         
        UpdateVPWin(TRUE);
    }    

}

/*****************************************************************************/
//  Description : 获取下一个本地播放列表信息
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void LoadNextLocalListFile(void)
{   
    DPLAYER_MEDIA_INFO_T media_info = {0};        
    uint16          full_path_len = VP_MAX_NAME_LEN;
    wchar           full_path_name [VP_MAX_NAME_LEN +1] = {0}; 
    BOOLEAN found_next = FALSE;
    
     found_next = GetNextLocalListFileName(full_path_name,&full_path_len); 

    if (found_next)
    {  
        SCI_MEMSET(s_video_player_control_info.full_path_name,0,((VP_MAX_NAME_LEN +1)*sizeof(wchar)));
        SCI_MEMSET(s_video_player_control_info.url,0,VP_MAX_URL_LEN +1);   
        
        s_video_player_control_info.full_path_len = full_path_len;
        MMIAPICOM_Wstrncpy(s_video_player_control_info.full_path_name,
                full_path_name,
                s_video_player_control_info.full_path_len);
        
        s_video_player_control_info.process_time = 0; 

        if(VP_STATE_READY == s_video_player_control_info.state 
           && (VP_PLAY_FROM_FILE == s_video_player_control_info.play_type 
           || VP_PLAY_FROM_LOCALLIST == s_video_player_control_info.play_type))
        {
            MMIAPIVP_GetInfoFromFullPathName(s_video_player_control_info.full_path_name, s_video_player_control_info.full_path_len, &media_info);
            s_video_player_control_info.total_time = media_info.media_length;   
        }
        else
        {
            s_video_player_control_info.total_time = 0;   
        }

        s_video_player_control_info.seek_time = 0;
        s_video_player_control_info.station_text_id= 0;
         s_video_player_control_info.support_type = VP_SUPPORTED_BOTH;
#ifdef  VIDEOTHUMBNAIL_SUPPORT          
         if(UILAYER_IsBltLayer(&s_thumbnail_layer))
       {
              DestroyLandscapeThumbnailLayer(); 
        }
#endif         
        UpdateVPWin(TRUE);
   
    }    

}


/*****************************************************************************/
//  Description : 获取第一个本地播放列表信息
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
LOCAL void LoadFirstLocalListFile(void)
{   
   
    FILEARRAY_DATA_T f_array_data = {0};
    DPLAYER_MEDIA_INFO_T media_info = {0};        
          
    if (VP_LocalList_GetInfo(0,&f_array_data,VP_LOCALLIST_DEFAULT))
    {  
        VP_LocalList_SetCurListType(VP_LOCALLIST_DEFAULT);
        VP_LocalList_SetCurrentIndex(0);    
        s_video_player_control_info.play_type = VP_PLAY_FROM_LOCALLIST;
        SCI_MEMSET(s_video_player_control_info.full_path_name,0,(VP_MAX_NAME_LEN +1)*sizeof(wchar));
        SCI_MEMSET(s_video_player_control_info.url,0,VP_MAX_URL_LEN +1);   
        
        s_video_player_control_info.full_path_len = MIN(VP_MAX_NAME_LEN,f_array_data.name_len);
        MMIAPICOM_Wstrncpy(s_video_player_control_info.full_path_name,
                f_array_data.filename,
                s_video_player_control_info.full_path_len);
        
        s_video_player_control_info.process_time = 0; 

        if(VP_STATE_READY == s_video_player_control_info.state 
           && (VP_PLAY_FROM_FILE == s_video_player_control_info.play_type 
           || VP_PLAY_FROM_LOCALLIST == s_video_player_control_info.play_type))
        {
            MMIAPIVP_GetInfoFromFullPathName(s_video_player_control_info.full_path_name, s_video_player_control_info.full_path_len, &media_info);
            s_video_player_control_info.total_time = media_info.media_length;   
        }
        else
        {
            s_video_player_control_info.total_time = 0;   
        }

        s_video_player_control_info.seek_time = 0;
        DisplayVPWinRepeatModeIcon(TRUE);
        DisplayVPWinRandomModeIcon(TRUE);
        DisplayVPWinTitle(TRUE);
#ifdef MMI_PDA_SUPPORT//istyle_return
        DisplayVPWinReturnIcion(TRUE);
#endif
        if (0 == s_video_player_control_info.full_path_len) //当前没有文件
        {
            DisplayVPWinStation(TXT_VP_NO_FILE,TRUE,FALSE);         
        }
        else
        {
            DisplayVPWinStation(TXT_NULL,TRUE,FALSE);
        }
        DisplayVPWinBackwardOrForwardIcon(TRUE);        
        DisplayVPWinPlayOrPauseIcon(TRUE);
        DisplayVPWinSoftkey(TRUE);
    }    
    else
    {
        s_video_player_control_info.play_type = VP_PLAY_FROM_FILE;
        DisplayVPWinStation(TXT_VP_NO_FILE,TRUE,FALSE);
    }
}



/*****************************************************************************/
//  Description : 设置播放速率
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
LOCAL void VideoPlayer_SetRate(BOOLEAN is_start, BOOLEAN is_backward)
{
    if (is_start)
    {
        VP_StartSetRate(s_video_player_control_info.vp_handle, is_backward);
    }
    else
    {
        VP_StopSetRate(s_video_player_control_info.vp_handle, is_backward);
    }
    
}

/*****************************************************************************/
//  Description : 定位
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
LOCAL void VideoPlayerSeek(MMI_WIN_ID_T  win_id, uint32 seek_time)
{
    VP_SEEK_PARAM_T seek_param = {0};

#ifdef  VIDEOTHUMBNAIL_SUPPORT  
     if(VP_STATE_PAUSED == s_video_player_control_info.state && s_vp_dphandle_getthumbnail.is_getthumbnail_by_dpHandle )
    {
        VPThumbnailResetDPLAYERParam();
        MMIAPIVP_ThumbnailClose();
    }
#endif    

    seek_param.seek_frame = VP_MAX_INVALID_VALUE;
    seek_param.seek_time = seek_time;   
    if (VP_PLAY_FROM_NET == s_video_player_control_info.play_type)
    {         
        s_video_player_control_info.state = VP_STATE_SEEKING;
//        StopVPWinDisplayerTimer();
//        if (IsNeedDisplayControlPanel())  //全屏播放，进入非播放状态，显示控制面板
//        { 
//            DisappearVPWin(win_id,FALSE,TRUE);   
//        }
        DisplayVPWinBackwardOrForwardIcon(TRUE);
        DisplayVPWinPlayOrPauseIcon(TRUE);   
        DisplayVPWinSoftkey(TRUE);
        DisplayVPWinStation(TXT_VP_SEEKING,TRUE,FALSE); 
    }      
   

    MMIAPIVP_FSMSeek(s_video_player_control_info.vp_handle,&seek_param);     
    s_video_player_control_info.seek_time = seek_time; //bug220953
    s_video_player_control_info.process_time = seek_time;
    

}

/*****************************************************************************/
//  Description : 停止
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
LOCAL void VideoPlayerStop(MMI_WIN_ID_T       win_id)
{
#ifdef  VIDEOTHUMBNAIL_SUPPORT  
     if(VP_STATE_PAUSED == s_video_player_control_info.state && s_vp_dphandle_getthumbnail.is_getthumbnail_by_dpHandle )
    {
        VPThumbnailResetDPLAYERParam();
        MMIAPIVP_ThumbnailClose();
    }
#endif       

    StopVPWinDisplayerTimer();    

#ifdef  VIDEOTHUMBNAIL_SUPPORT  //@bug 120428
    if(VP_STATE_PAUSED != s_video_player_control_info.state)
#endif
    {
        if (MMITHEME_IsMainScreenLandscape()
            && s_video_player_control_info.is_display_win)  //全屏播放，进入非播放状态，显示控制面板
        {        
            DisappearVPWin(win_id,FALSE,TRUE);   
        }
    }

    if (VP_PLAY_FROM_NET == s_video_player_control_info.play_type)
    { 
        s_video_player_control_info.state = VP_STATE_STOPING;        
        DisplayVPWinBackwardOrForwardIcon(TRUE);
        DisplayVPWinPlayOrPauseIcon(TRUE); 
        DisplayVPWinSoftkey(TRUE);
        DisplayVPWinStation(TXT_VP_STOPPING,TRUE,FALSE); 
        DisplayVPWinSwitchScreenIcon(TRUE);

#ifdef MMI_GPRS_SUPPORT
    //去活pdp;
    MMIAPIPDP_Deactive(MMI_MODULE_VP);   
#endif 
    } 
 #ifdef MMI_STREAMING_PLAYER_SUPPORT   
    //StopProcessTimer();   
    s_is_need_auth = FALSE;     
#endif
    MMIAPIVP_FSMStop(s_video_player_control_info.vp_handle);

    //change the play mode
    MMIAPIVP_FSMUpdatePlayMode(s_video_player_control_info.vp_handle,	 VP_REPEAT_PLAY_MODE_MAX, FALSE);
}
/*****************************************************************************/
//  Description : 恢复
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
LOCAL void VideoPlayerResume(MMI_WIN_ID_T       win_id)
{
#ifdef  VIDEOTHUMBNAIL_SUPPORT  
     if(VP_STATE_PAUSED == s_video_player_control_info.state && s_vp_dphandle_getthumbnail.is_getthumbnail_by_dpHandle )
    {
        VPThumbnailResetDPLAYERParam();
        MMIAPIVP_ThumbnailClose();
    }
#endif    
   
    s_video_player_control_info.state = VP_STATE_PLAYING;

    //VP_STRACE_LOW:"[VP] VideoPlayerResume s_video_player_control_info.state = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_5066_112_2_18_3_5_38_451,(uint8*)"d",s_video_player_control_info.state);

    if (!IsLastestHistory())
    {
        SaveHistory(s_video_player_control_info.full_path_name,
            s_video_player_control_info.full_path_len,
            s_video_player_control_info.url,
            s_video_player_control_info.process_time,
            s_video_player_control_info.play_type,                   
            s_video_player_control_info.total_time);
    }  

    if (VP_PLAY_FROM_NET == s_video_player_control_info.play_type)
    {    
        s_video_player_control_info.state = VP_STATE_RESUMING;
//        if (IsNeedDisplayControlPanel())  //全屏播放，进入非播放状态，显示控制面板
//        {
//            StopVPWinDisplayerTimer();
//            DisappearVPWin(win_id,FALSE,TRUE);   
//        }
        DisplayVPWinStation(TXT_VP_RESUMING,TRUE,FALSE);
    }    
    
    if (VP_PLAY_FROM_FILE == s_video_player_control_info.play_type)
    {
        DisplayVPWinBackwardOrForwardIcon(TRUE);
    }
    DisplayVPWinPlayOrPauseIcon(TRUE);  
    DisplayVPWinSoftkey(TRUE);
 //   ResumeProcessTimer(); //masked this for cr254473，只有当 UI 收到 VP_PLAY_CNF 时，表示resume成功，才调用StartProcessTimer()刷新进度条。
    MMIAPIVP_FSMResume(s_video_player_control_info.vp_handle);
}

/*****************************************************************************/
//  Description : 恢复
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
LOCAL void VideoPlayerCompulsiveResume(MMI_WIN_ID_T       win_id)
{
    s_is_complusive_pause = FALSE;

    //if(VP_TO_DO_PLAY_FOR_FULL_PAINT != s_video_player_control_info.to_do_type)
    {
        MMIAPIVP_FSMCompulsiveResume(s_video_player_control_info.vp_handle);    //状态改变，显示应该位于该函数之下
    }
    //add for newms108638 begin;
    //当streaming 处于VP_STATE_RESUMING 状态下，pause　icon　灰显，用户不可以选择；
    if (VP_PLAY_FROM_NET == s_video_player_control_info.play_type
        && (VP_STATE_RESUMING == MMIAPIVP_FSMGetState(s_video_player_control_info.vp_handle)||VP_STATE_PAUSING== MMIAPIVP_FSMGetState(s_video_player_control_info.vp_handle)))  
    {    
        s_video_player_control_info.state = VP_STATE_RESUMING;
//        if (IsNeedDisplayControlPanel())  //全屏播放，进入非播放状态，显示控制面板
//        {
//            StopVPWinDisplayerTimer();
//            DisappearVPWin(win_id,FALSE,TRUE);   
//        }
  //      DisplayVPWinStation(TXT_VP_RESUMING,TRUE,FALSE);
        DisplayVPWinPlayOrPauseIcon(TRUE);  
    }    
    //add for newms108638 end;
    
    if (VP_STATE_READY != s_video_player_control_info.state)
    {  
        MMIDEFAULT_TurnOnBackLight();
        MMIDEFAULT_AllowTurnOffBackLight(FALSE); 
        ResumeProcessTimer();
        DisappearVPWin(win_id,FALSE,TRUE);             
        StartVPWinDisplayerTimer();
        if (!IsLastestHistory()
            && (VP_STATE_PLAYING == s_video_player_control_info.state
            || VP_STATE_BUFFERING == s_video_player_control_info.state))
        {
            SaveHistory(s_video_player_control_info.full_path_name,
                s_video_player_control_info.full_path_len,
                s_video_player_control_info.url,
                s_video_player_control_info.process_time,
                s_video_player_control_info.play_type,                   
                s_video_player_control_info.total_time);
        }            
    }
}

/*****************************************************************************/
//  Description : 暂停
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
LOCAL void VideoPlayerCompulsivePause(MMI_WIN_ID_T       win_id)
{
    if (!s_is_complusive_pause)
    {
        s_is_complusive_pause = TRUE;
        MMIAPIVP_FSMCompulsivePause(s_video_player_control_info.vp_handle);
        //这里停止后再恢复需要重连，清除BUFFER相关显示信息
        if(TXT_VP_BUFFRING == s_video_player_control_info.station_text_id)
        {
            s_video_player_control_info.station_text_id = TXT_NULL;
        }
    }    
}

/*****************************************************************************/
//  Description : 暂停
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN VideoPlayerPause(MMI_WIN_ID_T       win_id)
{
    BOOLEAN result = TRUE;
    s_video_player_control_info.state = VP_STATE_PAUSED;     
    //VP_STRACE_LOW:"[VP] VideoPlayerPause s_video_player_control_info.state = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_5171_112_2_18_3_5_38_452,(uint8*)"d",s_video_player_control_info.state);

//    if (IsNeedDisplayControlPanel()) //全屏播放，进入非播放状态，显示控制面板
//    {
//        StopVPWinDisplayerTimer();
//        DisappearVPWin(win_id,FALSE,TRUE);   
//    }

    if(VP_PLAY_FROM_NET == s_video_player_control_info.play_type)
    {
        s_video_player_control_info.state = VP_STATE_PAUSING;
        DisplayVPWinStation(TXT_VP_PAUSING,TRUE,FALSE); 
    }
//    else
//    {
//        DisplayVPWinStation(TXT_VP_PAUSING,TRUE,FALSE); 
//    }

    //SCI_TRACE_LOW:"[VP] VideoPlayerPause s_video_player_control_info.state = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_5189_112_2_18_3_5_38_453,(uint8*)"d",s_video_player_control_info.state);
    
    if (VP_PLAY_FROM_LOCALLIST != s_video_player_control_info.play_type)
    {     
        DisplayVPWinBackwardOrForwardIcon(TRUE);  
    }

    DisplayVPWinPlayOrPauseIcon(TRUE);  
    DisplayVPWinSoftkey(TRUE);
    PauseProcessTimer();

    if(!MMIAPIVP_FSMPause(s_video_player_control_info.vp_handle)) //暂停失败，恢复状态
    {
        //VP_STRACE_LOW:"[VP] VideoPlayerPause PAUSE FAIL s_video_player_control_info.state = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_5202_112_2_18_3_5_38_454,(uint8*)"d",s_video_player_control_info.state);
        s_video_player_control_info.state = VP_STATE_PLAYING;
        DisplayVPWinStation(TXT_VP_PAUSE_ERROR,TRUE,FALSE); 
        StartVPDisplayStationTimer();
        if (VP_PLAY_FROM_LOCALLIST != s_video_player_control_info.play_type)
        {     
            DisplayVPWinBackwardOrForwardIcon(TRUE);  
        }
        DisplayVPWinPlayOrPauseIcon(TRUE);  
        DisplayVPWinSoftkey(TRUE);
        ResumeProcessTimer();
        result = FALSE;
    }

    return result;
}

/*****************************************************************************/
//  Description : 通知fsm播放
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN NotifyFsmPlay(void)
{
    VP_PLAY_PARAM_T    *play_param_ptr = PNULL;
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    GUI_RECT_T display_rect = {0};
    

    play_param_ptr = (VP_PLAY_PARAM_T*)SCI_ALLOCA(sizeof(VP_PLAY_PARAM_T));
    
    if (PNULL == play_param_ptr)
    {
        return FALSE;
    }

    SCI_MEMSET(play_param_ptr,0,sizeof(VP_PLAY_PARAM_T));
   

    if (VP_PLAY_FROM_NET == s_video_player_control_info.play_type)
    {
        s_video_player_control_info.state = VP_STATE_PLAY_PENDING;
        DisplayVPWinStation(TXT_VP_INIT,TRUE,PNULL);  
    }
    else
    {
        s_video_player_control_info.state = VP_STATE_PLAYING;
#ifndef MMI_VIDEO_PLAYER_MINI_LAYOUT//6530 MINI 界面布局
        if (VP_SUPPORTED_AUDIO_ONLY == s_video_player_control_info.support_type)
        {
            DisplayVPWinStation(TXT_VP_VIDIO_NOT_SUPPORT,TRUE,FALSE);          
        }
        else
        {
            DisplayVPWinStation(TXT_VP_PLAYING,TRUE,FALSE);        
            StartVPDisplayStationTimer();
        }
#endif
    }
    
    DisplayVPWinBackwardOrForwardIcon(TRUE);    
    DisplayVPWinRepeatModeIcon(TRUE); 
    DisplayVPWinRandomModeIcon(TRUE);
    DisplayVPWinTitle(TRUE); 
    DisplayVPWinFileIcon(TRUE);
    DisplayVPWinSwitchScreenIcon(TRUE);
#ifdef MMI_PDA_SUPPORT//istyle_return
    DisplayVPWinReturnIcion(TRUE);
#endif


    if (0 != s_video_player_control_info.seek_time)  //上次结束是非正常播放完毕结束，则设置断点，从断点出播放
    {
        s_video_player_control_info.to_do_type = VP_TO_DO_SEEK;
        s_video_player_control_info.process_time = s_video_player_control_info.seek_time;
    }

    //SCI_TRACE_LOW:"[VP] NotifyFsmPlay is_bt_support = %d, is_a2dp_support = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_5290_112_2_18_3_5_38_455,(uint8*)"dd",play_param_ptr->is_bt_support,play_param_ptr->is_a2dp_support);
        
    play_param_ptr->dual_sys = s_video_player_control_info.dual_sys;
    play_param_ptr->display_param.audioSupport = 1;
    play_param_ptr->display_param.videoSupport = 1;
    play_param_ptr->display_param.b_support_seek = 1;
    play_param_ptr->display_param.disp_mode = DPLAYER_DISP_FULLVIEW;    
    play_param_ptr->display_param.RotateAngle = s_video_player_control_info.cur_angle; 

    if (MMITHEME_IsMainScreenLandscape())
    {
        display_rect = MMITHEME_GetFullScreenRect();    
        play_param_ptr->is_scal_up = TRUE;
 #ifdef  VIDEOTHUMBNAIL_SUPPORT          
         if(UILAYER_IsBltLayer(&s_thumbnail_layer))
       {
              DestroyLandscapeThumbnailLayer(); 
        }
#endif         
    }
    else
    {   
        GUIOWNDRAW_GetDisplayRect(VP_PLAY_PANEL_VIDEO_OWNDRAW_CTRL_ID,&display_rect);      
        play_param_ptr->is_scal_up = FALSE;
    }
    
    play_param_ptr->display_param.disp_rect.dy = display_rect.bottom - display_rect.top + 1;
    play_param_ptr->display_param.disp_rect.dx = display_rect.right - display_rect.left + 1;       
    play_param_ptr->display_param.disp_rect.x = display_rect.left;
    play_param_ptr->display_param.disp_rect.y = display_rect.top;   

    play_param_ptr->display_param.target_rect.x = play_param_ptr->display_param.disp_rect.x;
    play_param_ptr->display_param.target_rect.y = play_param_ptr->display_param.disp_rect.y;
    play_param_ptr->display_param.target_rect.dx = play_param_ptr->display_param.disp_rect.dx;
    play_param_ptr->display_param.target_rect.dy = play_param_ptr->display_param.disp_rect.dy;

    

    play_param_ptr->video_buf_info.type = 0;
    play_param_ptr->video_buf_info.video_buf_len = 0;
    play_param_ptr->video_buf_info.video_buf_ptr = PNULL;
    play_param_ptr->play_type = s_video_player_control_info.play_type;
    if (VP_PLAY_FROM_BUFFER != s_video_player_control_info.play_type)
    {
        play_param_ptr->is_save_history = TRUE;
    }
    
    if (VP_PLAY_FROM_FILE == s_video_player_control_info.play_type
        || VP_PLAY_FROM_LOCALLIST == s_video_player_control_info.play_type)
    {
        play_param_ptr->full_path_len = s_video_player_control_info.full_path_len;
        play_param_ptr->full_path_ptr = s_video_player_control_info.full_path_name;        
        play_param_ptr->url_ptr = PNULL;
        play_param_ptr->mode = VP_Setting_GetRepeatPlayMode();
        play_param_ptr->is_random_play = VP_Setting_GetIsRandomPlayMode();       
    }
    else
    {
        play_param_ptr->is_random_play = FALSE;
        play_param_ptr->mode = VP_REPEAT_PLAY_MODE_CLOSE;
        if (VP_PLAY_FROM_FILE == s_video_player_control_info.play_type)
        {
            play_param_ptr->full_path_len = s_video_player_control_info.full_path_len;
            play_param_ptr->full_path_ptr = s_video_player_control_info.full_path_name;           
            play_param_ptr->url_ptr = PNULL;            
        }
        else if (VP_PLAY_FROM_NET == s_video_player_control_info.play_type)
        {
            play_param_ptr->full_path_len = s_video_player_control_info.full_path_len;
            if (0 != s_video_player_control_info.full_path_len)
            {
                play_param_ptr->full_path_ptr = s_video_player_control_info.full_path_name;
            }
            else
            {
                play_param_ptr->full_path_ptr = PNULL;      
            }
            play_param_ptr->url_ptr = (char *)(s_video_player_control_info.url);            
        }       
    } 
    play_param_ptr->vol = MMIAPISET_GetMultimVolume();
    result = MMIAPIVP_FSMPlay(s_video_player_control_info.vp_handle,play_param_ptr); 
    //在调用VP_Play()之前和之后，s_video_player_control_info.state 可能被改变,可能对应 play or pause icon;
    //正常情况下,play or pause icon 的设置应该和视频播放器主界面中间的软键对应起来，
    //所以DisplayVPWinPlayOrPauseIcon(TRUE)和DisplayVPWinSoftkey(TRUE)两行应该放在一起设置；
    DisplayVPWinPlayOrPauseIcon(TRUE);
    DisplayVPWinSoftkey(TRUE);   //softkey变为停止
    SCI_FREE(play_param_ptr);
    play_param_ptr = PNULL;
    
    if (MMI_RESULT_TRUE == result)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
   
}
#ifdef MMI_STREAMING_PLAYER_SUPPORT
/*****************************************************************************/
//  Description : 选择sim卡
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN GetSimCardForPlay(MN_DUAL_SYS_E *dual_sys_ptr)
{   
#ifndef MMI_WIFI_SUPPORT
    uint16 sim_sys  = MN_DUAL_SYS_1;
    uint32 sim_num = 0;

    if (PNULL == dual_sys_ptr)
    {
        return FALSE;
    }
    
    if (MMIAPISET_GetFlyMode())
    {
        MMIAPIPHONE_AlertSimNorOKStatus();       
        return FALSE;
    }
    
    sim_num = MMIAPIPHONE_GetSimAvailableNum(&sim_sys, 1);

    if (0 == sim_num)
    {
        //no sim
        MMIAPIPHONE_AlertSimNorOKStatus();

        return FALSE;
    }
    else if (1 == sim_num)
    {
        *dual_sys_ptr = (MN_DUAL_SYS_E)sim_sys;
    }
#ifndef MMI_MULTI_SIM_SYS_SINGLE
    else
    {
        uint32 major_sim_type = MMIAPISET_GetMajorSim(MMISET_MAJOR_SIM_SEL_FUNC_STREAMING);

        if (major_sim_type < MMISET_MAJOR_SIM_SEL_MANUAL_SEL
            && MMIAPIPHONE_IsSimAvailable(MN_DUAL_SYS_1+(major_sim_type-MMISET_MAJOR_SIM_SEL_SIM1)))
        {
            *dual_sys_ptr = (MN_DUAL_SYS_E)(MN_DUAL_SYS_1+(major_sim_type-MMISET_MAJOR_SIM_SEL_SIM1));
        }
        else
        {
            MMK_CreateWin((uint32*)MMIVP_SELECT_SIM_WIN_TAB, PNULL);
            return FALSE;
        }
    }
#endif

    return TRUE;
#else  //wifi
    uint16 sim_sys  = MN_DUAL_SYS_1;
    uint32 sim_num = 0;

    if (PNULL == dual_sys_ptr)
    {
        return FALSE;
    }

    if (MMIAPISET_GetFlyMode())
    {
        MMIAPIPHONE_AlertSimNorOKStatus();
        return FALSE;
    }

    sim_num = MMIAPIPHONE_GetSimAvailableNum(&sim_sys, 1);

    if (0 == sim_num)
    {
        *dual_sys_ptr = MMI_DUAL_SYS_MAX;
    }
    else
    {
        MMK_CreateWin((uint32*)MMIVP_SELECT_SIM_WIN_TAB,PNULL);
        return FALSE;
    }

    return TRUE;
#endif
}
#endif
/*****************************************************************************/
//  Description : 播放
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
LOCAL void VideoPlayerPlay(MMI_WIN_ID_T       win_id)
{     
    
    if (VP_STATE_READY != MMIAPIVP_FSMGetState(s_video_player_control_info.vp_handle))  //当前文件还没有停止，停止播放当前
    {        
        s_video_player_control_info.to_do_type = VP_TO_DO_PLAY_FOR_STOP; 
#ifdef MMI_VIDEOPLAYER_HISTORYLIST_SUPPORT
        if (IsSaveLastPlayPoint())         //记录停止播放断点
        {
            VP_HistoryList_SetLastPlayPoint(s_video_player_control_info.process_time);
        }        
#endif //MMI_VIDEOPLAYER_HISTORYLIST_SUPPORT
        VideoPlayerStop(win_id);    
        return;
    }  

#ifndef MMI_WIFI_SUPPORT    
    if (MMI_DUAL_SYS_MAX != s_video_player_control_info.dual_sys)
#endif
    {   
        NotifyFsmPlay(); 
    }  
    
}

/*****************************************************************************/
//  Description : 停止播放器素所有的timr
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
LOCAL void StopPlayerAllTimer(void)
{      
    StopVPDisplayStationTimer();
    StopVPTimer();
    StopVPWinDisplayerTimer();
    StopProcessTimer();
}

/*****************************************************************************/
//  Description : IsLastestHistory
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsLastestHistory(void)
{
    
    BOOLEAN            result           = FALSE;
#ifdef MMI_VIDEOPLAYER_HISTORYLIST_SUPPORT
    const VP_HISTORY_ITEM_T* history_item_ptr = VP_HistoryList_GetInfo(0);

    if (PNULL != history_item_ptr && history_item_ptr->type == s_video_player_control_info.play_type)
    {
        if (VP_PLAY_FROM_NET == history_item_ptr->type)
        {
            if (PNULL != history_item_ptr->net_movie_info_ptr 
                && 0 == strcmp(history_item_ptr->net_movie_info_ptr->url_ptr,
                                    (char *)(s_video_player_control_info.url)))
            {
                result = TRUE;
            }
        }
        else if (PNULL != history_item_ptr->local_movie_file_info_ptr)
        {
            if (0 == MMIAPICOM_Wstrcmp(history_item_ptr->local_movie_file_info_ptr->full_path_ptr,s_video_player_control_info.full_path_name))
            {
                result = TRUE;
            }
        }
    }
#endif
    return result;
    
}

/*****************************************************************************/
//  Description : 上一首
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
LOCAL void PlayPreFile(void)
{ 
    uint16          full_path_len = VP_MAX_NAME_LEN;
    wchar           full_path_name [VP_MAX_NAME_LEN +1] = {0}; 
   

    if(GetPreLocalListFileName(full_path_name,&full_path_len))
    {
        NotifyPlayerForPlayFile(full_path_name,full_path_len,0,0,VP_PLAY_FROM_LOCALLIST);
    }    
}


/*****************************************************************************/
//  Description : 后退
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
LOCAL void Backward(BOOLEAN is_start,
                    BOOLEAN is_seek)
{
    uint32          seek_time = 0;   
    MMI_WIN_ID_T    win_id = MMIVP_PLAYER_WIN_ID;
    
    MMIAPISET_StopAppRing();
   
    if(IsSeekEnable())
    {
        //VP_STRACE_LOW:"[VP] Backward process_time =%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_5601_112_2_18_3_5_39_457,(uint8*)"d",s_video_player_control_info.process_time);
        
        if (VP_PLAY_FROM_NET == s_video_player_control_info.play_type)  //网络端seek，长按时，只是进度条see,释放按键时，通知播放器开始seek
        {
            if (s_video_player_control_info.process_time < VP_SEEK_STEP)
            {
                seek_time = 0;
            }
            else
            {
                seek_time = s_video_player_control_info.process_time - VP_SEEK_STEP;
            }
            if (is_start)
            {                
                StopProcessTimer();
                StartVPTimer(VP_TIMER_BACKWARD_TYPE);
                s_video_player_control_info.process_time = seek_time;
                DisplayVPWinProgress(TRUE);
            }
            else
            {
                StopVPTimer();
                VideoPlayerSeek(win_id,seek_time);  
                StartProcessTimer();
            }
        }
        else if(is_seek)
        {      
            if (s_video_player_control_info.process_time < VP_SEEK_STEP)
            {
                seek_time = 0;
            }
            else
            {
                seek_time = s_video_player_control_info.process_time - VP_SEEK_STEP;
            }
            VideoPlayerSeek(win_id,seek_time); 
            StartProcessTimer();
        }
        else
        {
//            if (s_video_player_control_info.process_time < VP_SEEK_STEP)
//            {
//                seek_time = 0;
//            }
//            else
//            {
//                seek_time = s_video_player_control_info.process_time - VP_SEEK_STEP;
//            }
//            if (is_start)
//            {                
//                StopProcessTimer();
//                StartVPTimer(VP_TIMER_BACKWARD_TYPE);
//                s_video_player_control_info.process_time = seek_time;
//                DisplayVPWinProgress(TRUE);                
//            }
//            else
//            {
//                StopVPTimer();
//                VideoPlayerSeek(win_id,seek_time);  
//                StartProcessTimer();
//            }
            VideoPlayer_SetRate(is_start, TRUE);
        }
               
    }
}

/*****************************************************************************/
//  Description : 下一首
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
LOCAL void PlayNextFile(void)
{
    uint16          full_path_len = VP_MAX_NAME_LEN;
    wchar           full_path_name [VP_MAX_NAME_LEN +1] = {0};
   // GUIFORM_CHILD_DISPLAY_E display_type = GUIFORM_CHILD_DISP_NORMAL;

    if(GetNextLocalListFileName(full_path_name,&full_path_len))
    {
        NotifyPlayerForPlayFile(full_path_name,full_path_len,0,0,VP_PLAY_FROM_LOCALLIST);
    }
       
}

/*****************************************************************************/
//  Description : 前进
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
LOCAL void Forward(BOOLEAN is_start,
                   BOOLEAN is_seek)
{   
    uint32          seek_time = 0;
    MMI_WIN_ID_T    win_id = MMIVP_PLAYER_WIN_ID;      

    MMIAPISET_StopAppRing();

    if(IsSeekEnable())
    {  
                
        //VP_STRACE_LOW:"[VP] BackWardOrPreFile seek_time = %d, s_video_player_control_info.process_time = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_5719_112_2_18_3_5_39_459,(uint8*)"dd",seek_time,s_video_player_control_info.process_time);
        if (VP_PLAY_FROM_NET == s_video_player_control_info.play_type)
        {
            if (s_video_player_control_info.process_time + VP_SEEK_STEP > s_video_player_control_info.total_time)
            {
                seek_time = s_video_player_control_info.total_time;
            }
            else
            {
                seek_time = s_video_player_control_info.process_time + VP_SEEK_STEP;
            } 
            if (is_start)
            {
                StopProcessTimer();
                StartVPTimer(VP_TIMER_FORWARD_TYPE);
                s_video_player_control_info.process_time = seek_time;
                DisplayVPWinProgress(TRUE);
            }
            else
            {
                StopVPTimer();
                VideoPlayerSeek(win_id,seek_time);  
                StartProcessTimer();
            }
        }
        else if (is_seek)
        {
            if (s_video_player_control_info.process_time + VP_SEEK_STEP > s_video_player_control_info.total_time)
            {
                seek_time = s_video_player_control_info.total_time;
            }
            else
            {
                seek_time = s_video_player_control_info.process_time + VP_SEEK_STEP;
            }
            VideoPlayerSeek(win_id,seek_time);  
            StartProcessTimer();
        }
        else
        {           
//            if (s_video_player_control_info.process_time + VP_SEEK_STEP > s_video_player_control_info.total_time)
//            {
//                seek_time = s_video_player_control_info.total_time;
//            }
//            else
//            {
//                seek_time = s_video_player_control_info.process_time + VP_SEEK_STEP;
//            } 
//            if (is_start)
//            {                
//                StopProcessTimer();
//                StartVPTimer(VP_TIMER_FORWARD_TYPE);
//                s_video_player_control_info.process_time = seek_time;
//                DisplayVPWinProgress(TRUE);
//            }
//            else
//            {
//                StopVPTimer();
//                VideoPlayerSeek(win_id,seek_time);  
//                StartProcessTimer();
//            }
            VideoPlayer_SetRate(is_start, FALSE);
        }       
        
    }
   
    
}

/*****************************************************************************/
//  Description : 音量调节
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
LOCAL void AdjustVolCallBack(MMICOM_PANEL_CALLBACK_T *para_ptr) 
{
    if(PNULL != para_ptr)
    {
        //VP_STRACE_LOW:"[VP] AdjustVolCallBack volue = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_5796_112_2_18_3_5_39_460,(uint8*)"d", para_ptr->cur_value);
        MMIAPISET_SetMultimVolume(para_ptr->cur_value);
        MMIAPIVP_FSMUpdateCurrentVol(s_video_player_control_info.vp_handle, para_ptr->cur_value);
    }  
    DisplayVPWinVolumeIcon(TRUE);
}

/*****************************************************************************/
//  Description : 亮度调节
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
LOCAL void AdjustBrightCallBack(MMICOM_PANEL_CALLBACK_T *para_ptr) 
{
    if(PNULL != para_ptr)
    {
        //VP_STRACE_LOW:"[VP] AdjustBrightCallBack volue = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_5811_112_2_18_3_5_39_461,(uint8*)"d",para_ptr->cur_value);
        VP_Setting_SetBrightness(para_ptr->cur_value);
        MMIAPISET_UpdateLCDContrast(GUI_MAIN_LCD_ID,para_ptr->cur_value);    
    }
}

/*****************************************************************************/
//  Description : 亮度调节
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
//LOCAL uint8 AdjustBright(BOOLEAN is_up)
//{
//    uint8 bright = VP_Setting_GetBrightness();
//
//    if (is_up)
//    {
//        if (bright<MMISET_CONTRAST_SEVEN)
//        {
//            bright++;
//        }
//        else
//        {
//            bright = MMISET_CONTRAST_SEVEN;
//        }       
//    }
//    else if(bright>0)
//    {
//        bright--;    
//    }
//    else
//    {
//        bright = 0;
//    }
//    VP_STRACE_LOW("[VP] AdjustBright bright = %d, is_up = %d",bright,is_up);
//    AdjustBrightCallBack(bright);
//    return bright;
//}

/*****************************************************************************/
//  Description : 启动vp win control pandle display timer
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
LOCAL void StartVPWinDisplayerTimer(void)
{
    //VP_STRACE_LOW:"[VP] StartVPWinDisplayerTimer state = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_5855_112_2_18_3_5_39_462,(uint8*)"d",s_video_player_control_info.state);
    if(MMITHEME_IsMainScreenLandscape()
        && s_video_player_control_info.is_display_win
        && VP_STATE_PLAYING == s_video_player_control_info.state
        && MMK_IsFocusWin(MMIVP_PLAYER_WIN_ID))
    {
        if (0 != s_video_player_control_info.display_win_timer_id)
        {
            MMK_StopTimer(s_video_player_control_info.display_win_timer_id);
        }
              
        s_video_player_control_info.display_win_timer_id = MMK_CreateWinTimer(MMIVP_PLAYER_WIN_ID, MMI_2SECONDS, FALSE);
        MMK_StartWinTimer(MMIVP_PLAYER_WIN_ID, s_video_player_control_info.display_win_timer_id, MMI_2SECONDS, FALSE);
       
    }
}

/*****************************************************************************/
//  Description : 停止vp win control pandle display timer
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
LOCAL void StopVPWinDisplayerTimer(void)
{
    if (0 != s_video_player_control_info.display_win_timer_id)
    {
        MMK_StopTimer(s_video_player_control_info.display_win_timer_id);
        s_video_player_control_info.display_win_timer_id = 0;
    }
}

/*****************************************************************************/
//  Description : 启动vp timer
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
LOCAL void StartVPTimer(VP_TIMER_TYPE_E timer_type)
{
    BOOLEAN is_start_timer = FALSE;
    uint32 time_out = MMI_3SECONDS;
 

    //VP_STRACE_LOW:"[VP] StartVPTimer timer_type = %d, s_video_player_control_info.timer.timer_type  = %d, s_video_player_control_info.timer.timer_id = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_5896_112_2_18_3_5_40_463,(uint8*)"ddd",timer_type,s_video_player_control_info.timer.timer_type,s_video_player_control_info.timer.timer_id );
    
    switch(timer_type) 
    {  
    case VP_TIMER_BACKWARD_TYPE:        
    case VP_TIMER_FORWARD_TYPE:
        is_start_timer = TRUE;
        time_out = MMIVP_LONG_REPEAT_TIME_OUT;
        break;    
    
    default:
        break;
    }

    if (0 != s_video_player_control_info.timer.timer_id)
    {
        MMK_StopTimer(s_video_player_control_info.timer.timer_id);
    }
    
    if (is_start_timer)
    {        
        s_video_player_control_info.timer.timer_type = timer_type;        
        s_video_player_control_info.timer.timer_id = MMK_CreateWinTimer(MMIVP_PLAYER_WIN_ID, time_out, FALSE);
        //VP_STRACE_LOW:"[VP] StartVPTimer s_video_player_control_info.timer.timer_id = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_5919_112_2_18_3_5_40_464,(uint8*)"d",s_video_player_control_info.timer.timer_id);
        MMK_StartWinTimer(MMIVP_PLAYER_WIN_ID, s_video_player_control_info.timer.timer_id, time_out, FALSE);
    }
    else
    {
        s_video_player_control_info.timer.timer_id = 0;       
    }
}

/*****************************************************************************/
//  Description : 停止vp timer
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
LOCAL void StopVPTimer(void)
{
    if (0 != s_video_player_control_info.timer.timer_id)
    {
        MMK_StopTimer(s_video_player_control_info.timer.timer_id);     
        s_video_player_control_info.timer.timer_id = 0;
    }
}

/*****************************************************************************/
//  Description : vp timer
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN HandleVPTimerMsg(void)
{
    BOOLEAN result = TRUE;
   
   

    //VP_STRACE_LOW:"[VP] HandleVPTimerMsg s_video_player_control_info.timer.timer_type = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_5951_112_2_18_3_5_40_465,(uint8*)"d",s_video_player_control_info.timer.timer_type);
    switch(s_video_player_control_info.timer.timer_type) 
    {    
    case VP_TIMER_BACKWARD_TYPE:
        s_video_player_control_info.timer.timer_id = 0;
        Backward(TRUE,FALSE);
        break;

    case VP_TIMER_FORWARD_TYPE:
        s_video_player_control_info.timer.timer_id = 0;
        Forward(TRUE,FALSE);
        break;
    
    default:
        result = FALSE;
        break;
    }

    

    return result;
}


/*****************************************************************************/
//  Description : 半屏状态下方向键处理函数.
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
LOCAL void HandleDirectionKeyMsgForHalfScreen(MMI_MESSAGE_ID_E msg_id)
{  
    
    
    
    //VP_STRACE_LOW:"[VP] HandleDirectionKeyMsgForHalfScreen msg_id = %x, s_video_player_control_info.timer.timer_type = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_5984_112_2_18_3_5_40_466,(uint8*)"dd",msg_id,s_video_player_control_info.timer.timer_type);
    SCI_TRACE_LOW("[VP] HandleDirectionKeyMsgForHalfScreen play_type=%d, state=%d", s_video_player_control_info.play_type, s_video_player_control_info.state);
    switch(msg_id) 
    {
    case MSG_KEYDOWN_UP:        
    case MSG_APP_UPSIDE:
        MMIAPIVP_TurnVol(MMICOM_PANEL_OPEN_INC);
        break;
        
    case MSG_KEYDOWN_DOWN:         
    case MSG_APP_DOWNSIDE:
//        if (IsNeedDisplayControlPanel())
//        {
//            DisappearVPWin(MMIVP_PLAYER_WIN_ID,FALSE,TRUE);
//            StartVPWinDisplayerTimer();
//        }
        MMIAPIVP_TurnVol(MMICOM_PANEL_OPEN_DEC);
        break;           

    case MSG_KEYDOWN_LEFT:
    case MSG_KEYDOWN_RIGHT:        
        StopVPWinDisplayerTimer();
        if (IsNeedDisplayControlPanel())
        {
            DisappearVPWin(MMIVP_PLAYER_WIN_ID,FALSE,TRUE);            
        }        
        break;

    case MSG_KEYUP_LEFT:         
        StartVPWinDisplayerTimer();
        if (VP_PLAY_FROM_LOCALLIST == s_video_player_control_info.play_type)
        {
            StopDelaySwitchVideoPlayerTimer();
            if(VP_STATE_READY == s_video_player_control_info.state)
            {
                LoadPreLocalListFile();
            }
            else if(VP_STATE_PAUSED == s_video_player_control_info.state)
            {
#ifdef MMI_VIDEOPLAYER_HISTORYLIST_SUPPORT
                if (IsSaveLastPlayPoint())         //记录停止播放断点
                {
                    VP_HistoryList_SetLastPlayPoint(s_video_player_control_info.process_time);
                }        
#endif //MMI_VIDEOPLAYER_HISTORYLIST_SUPPORT
                s_video_player_control_info.to_do_type = VP_TO_DO_PLAY_FOR_LOAD_FILE;
                VideoPlayerStop(MMIVP_PLAYER_WIN_ID);  
                s_video_player_control_info.to_do_type = VP_TO_DO_NONE;
                s_video_player_control_info.state = VP_STATE_READY;
                LoadPreLocalListFile();
            }
            else
            {
                PlayPreFile();
            }
     
        }
        else
        {
            Backward(FALSE,TRUE);
        }
        break;

    case MSG_KEYUP_RIGHT:       
        StartVPWinDisplayerTimer();
        if (VP_PLAY_FROM_LOCALLIST == s_video_player_control_info.play_type)
        {
            StopDelaySwitchVideoPlayerTimer();
            if(VP_STATE_READY == s_video_player_control_info.state)
            {
                LoadNextLocalListFile();
            }
            else if(VP_STATE_PAUSED == s_video_player_control_info.state)
            {
#ifdef MMI_VIDEOPLAYER_HISTORYLIST_SUPPORT
                if (IsSaveLastPlayPoint())         //记录停止播放断点
                {
                VP_HistoryList_SetLastPlayPoint(s_video_player_control_info.process_time);
                }        
#endif //MMI_VIDEOPLAYER_HISTORYLIST_SUPPORT
                s_video_player_control_info.to_do_type = VP_TO_DO_PLAY_FOR_LOAD_FILE;
                VideoPlayerStop(MMIVP_PLAYER_WIN_ID);  
                s_video_player_control_info.to_do_type = VP_TO_DO_NONE;
                s_video_player_control_info.state = VP_STATE_READY;
                LoadNextLocalListFile();
            }
            else
            {
                 PlayNextFile();
            }
           
        }
        else
        {
            Forward(FALSE,TRUE);
        }
        break;     
        
    case MSG_KEYLONG_LEFT:  
        s_key_down_msg_id = msg_id;
        Backward(TRUE,FALSE);                     
        break;
    
    case MSG_KEYLONG_RIGHT:  
        s_key_down_msg_id = msg_id;
        Forward(TRUE,FALSE);        
        break;    
        
    case MSG_KEYPRESSUP_LEFT:
        s_key_down_msg_id = 0;
        StartVPWinDisplayerTimer();
        Backward(FALSE,FALSE);                     
        break;

    case MSG_KEYPRESSUP_RIGHT:  
        s_key_down_msg_id = 0;
        StartVPWinDisplayerTimer();
        Forward(FALSE,FALSE);       
        break;
        
    default:
        break;
    }
    

}

/*****************************************************************************/
//  Description : 快捷键处理
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
LOCAL void HandleShortCutMsg(MMI_MESSAGE_ID_E msg_id)
{
    uint8 vol = MMISET_VOL_ZERO;
    //VP_STRACE_LOW:"[VP] HandleShortCutMsg msg_id = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_6070_112_2_18_3_5_40_467,(uint8*)"d",msg_id);
    
    switch(msg_id)
    {    
    case MSG_KEYDOWN_3:
#ifdef MMI_VIDEO_PLAYER_MINI_SUPPORT //6530_MINI   
#ifdef KEYPAD_TYPE_QWERTY_KEYPAD //6530_MINI NEWMS00207601
    case MSG_KEYDOWN_Y:
#endif 
        DisplayOSD();
#else
#ifdef MMI_STREAMING_PLAYER_SUPPORT
        if (IsOpenOtherWinEnable())
        {
            MMK_CreateWin((uint32 *)MMIVP_BOOKMARKLIST_WIN_TAB,PNULL);
        }      
#endif
#endif  // MMI_VIDEO_PLAYER_MINI_SUPPORT //6530_MINI NEWMS00207601
        break;
     
    case MSG_KEYDOWN_2: 
#ifdef MMI_VIDEO_PLAYER_MINI_LAYOUT//6530 MINI 界面布局没有转屏
#ifdef KEYPAD_TYPE_QWERTY_KEYPAD //6530_MINI NEWMS00207601
    case MSG_KEYDOWN_T:
#endif
#ifdef MMI_VIDEO_PLAYER_MINI_SUPPORT //6530_MINI
        DisplayOSD();
#endif
#else
        SetBlockType(VP_NONE_TRANSPARENCE);  //bug 876087
        HandleSwitchScreenMsg();
#endif //MMI_VIDEO_PLAYER_MINI_SUPPORT //6530_MINI NEWMS00207601
        break;

        
    case MSG_KEYDOWN_1:
#ifdef MMI_VIDEO_PLAYER_MINI_SUPPORT //6530_MINI 
#ifdef KEYPAD_TYPE_QWERTY_KEYPAD //6530_MINI NEWMS00207601
    case MSG_KEYDOWN_R:
#endif
        DisplayOSD();
#else
#ifdef MMI_STREAMING_PLAYER_SUPPORT
#ifdef MMI_VIDEOPLAYER_HISTORYLIST_SUPPORT
        if (IsOpenOtherWinEnable())
        {
            MMK_CreateWin((uint32 *)MMIVP_HISTORYLIST_WIN_TAB,PNULL);
        }        
#endif
#endif
#endif //MMI_VIDEO_PLAYER_MINI_SUPPORT //6530_MINI NEWMS00207601
        break;
        
    case MSG_KEYDOWN_5:
#ifdef MMI_VIDEO_PLAYER_MINI_SUPPORT //6530_MINI NEWMS00207601
#ifdef KEYPAD_TYPE_QWERTY_KEYPAD //6530_MINI NEWMS00207601
    case MSG_KEYDOWN_G:
#endif
#endif
//        if (IsNeedDisplayControlPanel())
//        {
//            DisappearVPWin(MMIVP_PLAYER_WIN_ID,FALSE,TRUE);
//            StartVPWinDisplayerTimer();
//        }
        OpenControlPanel(FALSE); 
        break;
        
    case MSG_KEYDOWN_8: 
#ifdef MMI_VIDEO_PLAYER_MINI_SUPPORT //6530_MINI
#ifdef KEYPAD_TYPE_QWERTY_KEYPAD //6530_MINI NEWMS00207601
    case MSG_KEYDOWN_B:  ////CR NEWMS00207601  leimingma
#endif
#endif
#ifdef MMI_STREAMING_PLAYER_SUPPORT      
        if (IsNeedDisplayControlPanel() && (VP_PLAY_FROM_NET == s_video_player_control_info.play_type)) 
        {
            DisappearVPWin(MMIVP_PLAYER_WIN_ID,FALSE,TRUE);
            StartVPWinDisplayerTimer();
        }
        SaveBookMark();        
#else 
#ifdef MMI_VIDEO_PLAYER_MINI_SUPPORT //6530_MINI
        DisplayOSD(); ////CR NEWMS00207601  leimingma
#endif 
#endif //MMI_STREAMING_PLAYER_SUPPORT  
        break;
#ifdef MMI_VIDEOPLAYER_SHORTCUTMENU_SUPPORT    
    case MSG_KEYDOWN_HASH: 
        if (IsOpenOtherWinEnable())
        {
            MMK_CreateWin((uint32 *)MMIVP_SHORTCUT_POPWIN_TAB,PNULL);
            GUIMENU_CreatDynamic(PNULL,MMIVP_SHOUTCUT_OPT_MENU_WIN_ID,VP_SHOUTCUT_OPT_MENU_CTRL_ID,GUIMENU_STYLE_POPUP);   
        }        
        break;
#endif
    case MSG_KEYDOWN_STAR:
#ifdef MMI_VIDEO_PLAYER_MINI_SUPPORT //6530_MINI 
#ifdef KEYPAD_TYPE_QWERTY_KEYPAD //6530_MINI NEWMS00207601
    case MSG_KEYDOWN_U:
#endif
#endif
        if (!MMIAPICOM_IsPanelOpened())
        {
         //   if (IsNeedDisplayControlPanel())  //masked @CR242129 2011.05.18 , if user press * key continuously , LCD always displays OSD.
            {
                DisappearVPWin(MMIVP_PLAYER_WIN_ID,FALSE,TRUE);

                StartVPWinDisplayerTimer();
            } 
            vol = MMIAPISET_MultimMuteSwitch();
            MMIAPIVP_FSMUpdateCurrentVol(s_video_player_control_info.vp_handle,vol);
            DisplayVPWinVolumeIcon(TRUE);
#ifdef MMI_VIDEO_PLAYER_MINI_LAYOUT //6530 MINI 界面布局
            MMIAPIVP_TurnVol(MMICOM_PANEL_OPEN_NONE);
#endif
        }    
        else
        {
            MMIAPICOM_SwitchPanelValue(MMICOM_PANEL_VOLUME);
        }
        break;

#ifdef MMI_VIDEO_PLAYER_MINI_SUPPORT //6530_MINI 支持4 6 直接调亮度 添加无快捷键数字键显示OSD
    case MSG_KEYDOWN_4:
#ifdef KEYPAD_TYPE_QWERTY_KEYPAD
    case MSG_KEYDOWN_F:
#endif
        AdjustBrightness(MMICOM_PANEL_OPEN_DEC);
        break;


    case MSG_KEYDOWN_6:
#ifdef KEYPAD_TYPE_QWERTY_KEYPAD
    case MSG_KEYDOWN_H:
#endif
        AdjustBrightness(MMICOM_PANEL_OPEN_INC);
        break;

    case MSG_KEYDOWN_7:
    case MSG_KEYDOWN_9:
    case MSG_KEYDOWN_0:
#ifdef KEYPAD_TYPE_QWERTY_KEYPAD
    case MSG_KEYDOWN_V:
    case MSG_KEYDOWN_N:
    case MSG_KEYDOWN_M:  
#endif 
        DisplayOSD();
        break;
#endif //MMI_VIDEO_PLAYER_MINI_SUPPORT //6530_MINI 支持4 6 直接调亮度 添加无快捷键数字键显示OSD

    default:
        break;
    }
}
#ifdef MMI_VIDEO_PLAYER_MINI_LAYOUT//6530 MINI 界面布局
/*****************************************************************************/
//  Description : SetVPLayoutParam
//  Global resource dependence : 
//  Author: leiming.ma
//  Note: 
/*****************************************************************************/
LOCAL void SetVPLayoutParam(void)
{
//    GUI_RECT_T screen_rect = {0};
    GUI_BG_T   bg_info     = {0};
//    GUI_BG_T   fg_info     = {0};

    GUI_RECT_T vp_random_button_rect_h   = VP_RANDOM_BUTTON_RECT_H;
    GUI_RECT_T vp_repeat_button_rect_h   = VP_REPEAT_BUTTON_RECT_H;
    GUI_RECT_T vp_name_label_rect_h      = VP_NAME_LABEL_RECT_H;
    GUI_RECT_T vp_video_owndraw_rect_h   = VP_VIDEO_OWNDRAW_RECT_H;
    GUI_RECT_T vp_pre_button_rect_h      = VP_PRE_BUTTON_RECT_H;
    GUI_RECT_T vp_play_button_rect_h     = VP_PLAY_BUTTON_RECT_H;
    GUI_RECT_T vp_next_button_rect_h     = VP_NEXT_BUTTON_RECT_H;
    GUI_RECT_T vp_process_owndraw_rect_h = VP_PROCESS_OWNDRAW_RECT_H;
#if (defined MAINLCD_DEV_SIZE_220X176) || (defined MAINLCD_DEV_SIZE_160X128) //6530_MINI 220X176
    GUI_RECT_T vp_opt_button_rect_h      = VP_OPT_BUTTON_RECT_H;
    GUI_RECT_T vp_return_button_rect_h   = VP_RETURN_BUTTON_RECT_H;
#endif


#if (defined MAINLCD_DEV_SIZE_220X176) || (defined MAINLCD_DEV_SIZE_160X128)  //6530_MINI 220X176  
 ////////////设置220X176全键盘机的布局
//title 
    bg_info.bg_type = GUI_BG_IMG;
    bg_info.img_id = IMAGE_VP_TITLE_BAR;
    //random
    GUIBUTTON_SetRect(VP_PLAY_PANEL_RANDOM_BUTTON_CTRL_ID, &vp_random_button_rect_h);
    GUIBUTTON_SetBg(VP_PLAY_PANEL_RANDOM_BUTTON_CTRL_ID, &bg_info);
    //repeat
    GUIBUTTON_SetRect(VP_PLAY_PANEL_REPEAT_BUTTON_CTRL_ID, &vp_repeat_button_rect_h);
    GUIBUTTON_SetBg(VP_PLAY_PANEL_REPEAT_BUTTON_CTRL_ID, &bg_info);
    //name
    GUILABEL_SetRect(VP_PLAY_NAME_LABEL_CTRL_ID, &vp_name_label_rect_h, FALSE);
    GUILABEL_SetBg(VP_PLAY_NAME_LABEL_CTRL_ID, &bg_info);
    GUILABEL_SetFont(VP_PLAY_NAME_LABEL_CTRL_ID, VP_NAME_FONT, MMI_WHITE_COLOR);

//video
    //video
    GUIOWNDRAW_SetRect(VP_PLAY_PANEL_VIDEO_OWNDRAW_CTRL_ID, &vp_video_owndraw_rect_h);


//process &button
    bg_info.img_id = IMAGE_VP_PROCESSBAR_BG;
    //process
    GUIOWNDRAW_SetRect(VP_PLAY_PANEL_PROCESS_OWNDRAW_CTRL_ID, &vp_process_owndraw_rect_h);
    GUIOWNDRAW_SetBg(VP_PLAY_PANEL_PROCESS_OWNDRAW_CTRL_ID, &bg_info);
    //options
//     GUILABEL_SetRect(VP_PLAY_PANEL_OPTIONS_LABEL_CTRL_ID, &vp_option_label_rect_h, FALSE);
//     GUILABEL_SetTextById(VP_PLAY_PANEL_OPTIONS_LABEL_CTRL_ID, STXT_OPTION, FALSE);
//     GUILABEL_SetFont(VP_PLAY_PANEL_OPTIONS_LABEL_CTRL_ID, VP_NAME_FONT, MMI_WHITE_COLOR);
//     GUILABEL_SetBg(VP_PLAY_PANEL_OPTIONS_LABEL_CTRL_ID, &bg_info);

    //button play pre next opt return 
    GUIBUTTON_SetRect(VP_PLAY_PANEL_PRE_BUTTON_CTRL_ID, &vp_pre_button_rect_h);
    GUIBUTTON_SetBg(VP_PLAY_PANEL_PRE_BUTTON_CTRL_ID, &bg_info);
    GUIBUTTON_SetHandleLong(VP_PLAY_PANEL_PRE_BUTTON_CTRL_ID,TRUE);

    GUIBUTTON_SetRect(VP_PLAY_PANEL_PLAY_BUTTON_CTRL_ID, &vp_play_button_rect_h);
    GUIBUTTON_SetBg(VP_PLAY_PANEL_PLAY_BUTTON_CTRL_ID, &bg_info);

    GUIBUTTON_SetRect(VP_PLAY_PANEL_NEXT_BUTTON_CTRL_ID, &vp_next_button_rect_h);
    GUIBUTTON_SetBg(VP_PLAY_PANEL_NEXT_BUTTON_CTRL_ID, &bg_info);
    GUIBUTTON_SetHandleLong(VP_PLAY_PANEL_NEXT_BUTTON_CTRL_ID,TRUE);

    GUIBUTTON_SetRect(VP_PLAY_PANEL_OPT_BUTTON_CTRL_ID, &vp_opt_button_rect_h);
    GUIBUTTON_SetBg(VP_PLAY_PANEL_OPT_BUTTON_CTRL_ID, &bg_info);

    GUIBUTTON_SetRect(VP_PLAY_PANEL_RETURN_BUTTON_CTRL_ID, &vp_return_button_rect_h);
    GUIBUTTON_SetBg(VP_PLAY_PANEL_RETURN_BUTTON_CTRL_ID, &bg_info);


#else //176X220   128X160

//title 
    bg_info.bg_type = GUI_BG_IMG;
    bg_info.img_id = IMAGE_VP_TITLE_BAR;
    //random
    GUIBUTTON_SetRect(VP_PLAY_PANEL_RANDOM_BUTTON_CTRL_ID, &vp_random_button_rect_h);
    GUIBUTTON_SetBg(VP_PLAY_PANEL_RANDOM_BUTTON_CTRL_ID, &bg_info);
    //repeat
    GUIBUTTON_SetRect(VP_PLAY_PANEL_REPEAT_BUTTON_CTRL_ID, &vp_repeat_button_rect_h);
    GUIBUTTON_SetBg(VP_PLAY_PANEL_REPEAT_BUTTON_CTRL_ID, &bg_info);
    //back
// #ifndef MAINLCD_DEV_SIZE_128X160 
//     GUILABEL_SetRect(VP_PLAY_PANEL_BACK_LABEL_CTRL_ID, &vp_back_label_rect_h, FALSE);
//     GUILABEL_SetTextById(VP_PLAY_PANEL_BACK_LABEL_CTRL_ID, TXT_COMMON_COMMON_BACK, FALSE);
//     GUILABEL_SetFont(VP_PLAY_PANEL_BACK_LABEL_CTRL_ID, VP_NAME_FONT, MMI_WHITE_COLOR);
//     GUILABEL_SetBg(VP_PLAY_PANEL_BACK_LABEL_CTRL_ID, &bg_info);
// #endif
    //name
    GUILABEL_SetRect(VP_PLAY_NAME_LABEL_CTRL_ID, &vp_name_label_rect_h, FALSE);
    GUILABEL_SetBg(VP_PLAY_NAME_LABEL_CTRL_ID, &bg_info);
    GUILABEL_SetFont(VP_PLAY_NAME_LABEL_CTRL_ID, VP_NAME_FONT, MMI_WHITE_COLOR);

//video & button
    //video
    GUIOWNDRAW_SetRect(VP_PLAY_PANEL_VIDEO_OWNDRAW_CTRL_ID, &vp_video_owndraw_rect_h);
    //play pre next
    GUIBUTTON_SetRect(VP_PLAY_PANEL_PRE_BUTTON_CTRL_ID, &vp_pre_button_rect_h);
    GUIBUTTON_SetRect(VP_PLAY_PANEL_PLAY_BUTTON_CTRL_ID, &vp_play_button_rect_h);
    GUIBUTTON_SetRect(VP_PLAY_PANEL_NEXT_BUTTON_CTRL_ID, &vp_next_button_rect_h);

//process
    bg_info.img_id = IMAGE_VP_PROCESSBAR_BG;
    //process
    GUIOWNDRAW_SetRect(VP_PLAY_PANEL_PROCESS_OWNDRAW_CTRL_ID, &vp_process_owndraw_rect_h);
    GUIOWNDRAW_SetBg(VP_PLAY_PANEL_PROCESS_OWNDRAW_CTRL_ID, &bg_info);
    //options
// #ifndef MAINLCD_DEV_SIZE_128X160 
//     GUILABEL_SetRect(VP_PLAY_PANEL_OPTIONS_LABEL_CTRL_ID, &vp_option_label_rect_h, FALSE);
//     GUILABEL_SetTextById(VP_PLAY_PANEL_OPTIONS_LABEL_CTRL_ID, STXT_OPTION, FALSE);
//     GUILABEL_SetFont(VP_PLAY_PANEL_OPTIONS_LABEL_CTRL_ID, VP_NAME_FONT, MMI_WHITE_COLOR);
//     GUILABEL_SetBg(VP_PLAY_PANEL_OPTIONS_LABEL_CTRL_ID, &bg_info);
// #endif
#endif  //MMI_VIDEO_PLAYER_SCREEN_H_SUPPORT //6530_MINI 220X176
}

#else  //6530 form 设置

/*****************************************************************************/
//  Description : SetFormParam
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
LOCAL void SetFormParam(void)
{
    GUI_RECT_T       screen_rect = {0};  
    uint16           height = 0;
    uint16           h_space = VP_BUTTON_SPACE;    
    uint16           v_space = 0;
    GUIFORM_CHILD_WIDTH_T sbs = {0};
    GUI_BG_T        bg_info = {0}; 
    GUI_COLOR_T font_color = 0;
    uint16         title_height = 0;
    uint16         mode_height = 0;
#ifdef MMI_PDA_SUPPORT//@andy.he_MP4PDA
    uint16         prev_height = 0;
    uint16         btn_height = 0;
#endif    
    //set form not support slide
    GUIFORM_IsSlide(VP_PLAY_PANEL_FORM_CTRL_ID,FALSE);

    screen_rect = MMITHEME_GetFullScreenRect();
    screen_rect.top = screen_rect.top + MMI_STATUSBAR_HEIGHT;
    if(MMITHEME_IsMainScreenLandscape())
    {          
        SetFormParamH();
    }
    else
    {
#ifdef  VIDEOTHUMBNAIL_SUPPORT  
        if(UILAYER_IsBltLayer(&s_thumbnail_layer))
        {
            DestroyLandscapeThumbnailLayer(); 
        }
#endif    
#ifndef MMI_PDA_SUPPORT
        screen_rect.bottom = screen_rect.bottom - (int16)MMITHEME_GetSoftkeyHeight();
#endif
        SetFormParamV();
    }
    GUIFORM_SetRect(VP_PLAY_PANEL_FORM_CTRL_ID,&screen_rect); 

    //设置整个form的margin为0  
    GUIFORM_SetMargin(VP_PLAY_PANEL_FORM_CTRL_ID,0);  
    
    //设置form的背景是透明色
    bg_info.bg_type = GUI_BG_COLOR;
    //bg_info.color = MMIAPIVP_GetTransparenceColor();
    bg_info.color = MMI_BACKGROUND_COLOR;  //bug876087
    GUIFORM_SetBg(VP_PLAY_PANEL_FORM_CTRL_ID,&bg_info);
    //设置starusbar的背景与form的背景相同
    //bug 866721
    bg_info.color = MMI_BACKGROUND_COLOR;
    //bug 866721
    GUIWIN_SetStbBg(MMIVP_PLAYER_WIN_ID,bg_info);
    /*-----------------------------------------------------title栏部分begin-----------------------------------------*/

    //设置title栏的背景，margin为VP_TITLE_MARGIN，h_space和v_space为0
    bg_info.bg_type = GUI_BG_IMG;
    bg_info.img_id = IMAGE_VP_TITLE_BAR;

    GUIFORM_SetBg(VP_PLAY_PANEL_TITLE_FORM_CTRL_ID, &bg_info);
    GUIFORM_PermitChildFont(VP_PLAY_PANEL_TITLE_FORM_CTRL_ID,FALSE);
    v_space = VP_TITLE_MARGIN;
    h_space = 0;
    GUIFORM_SetChildSpace(VP_PLAY_PANEL_FORM_CTRL_ID,VP_PLAY_PANEL_TITLE_FORM_CTRL_ID,&h_space,&v_space);   


#ifdef MMI_PDA_SUPPORT//istyle_return
        //istyle返回图标固定宽度，没有光影效果，设置按下的背景图片以及正常没有背景图片
        sbs.type = GUIFORM_CHILD_WIDTH_FIXED;
        GUIRES_GetImgWidthHeight(&sbs.add_data,PNULL,IMAGE_COMMON_BTN_RETURN,MMIVP_PLAYER_WIN_ID);
        GUIFORM_SetChildWidth(VP_PLAY_PANEL_TITLE_FORM_CTRL_ID,VP_PLAY_PANEL_RETURN_BUTTON_CTRL_ID,&sbs);    
        GUIBUTTON_SetRunSheen(VP_PLAY_PANEL_RETURN_BUTTON_CTRL_ID,FALSE); 
        bg_info.bg_type = GUI_BG_NONE;
        bg_info.img_id = IMAGE_COMMON_BTN_RETURN;//IMAGE_COMMON_BTN_RETURN;
        GUIBUTTON_SetPressedBg(VP_PLAY_PANEL_RETURN_BUTTON_CTRL_ID,&bg_info);     
        bg_info.bg_type = GUI_BG_NONE;
        GUIBUTTON_SetBg(VP_PLAY_PANEL_RETURN_BUTTON_CTRL_ID,&bg_info); 
#endif

    //循环图标固定宽度，没有光影效果，设置按下的背景图片以及正常没有背景图片
    sbs.type = GUIFORM_CHILD_WIDTH_FIXED;
    GUIRES_GetImgWidthHeight(&sbs.add_data,PNULL,IMAGE_COMMON_MODE_BTN_PRESS,MMIVP_PLAYER_WIN_ID);
    GUIFORM_SetChildWidth(VP_PLAY_PANEL_TITLE_FORM_CTRL_ID,VP_PLAY_PANEL_REPEAT_BUTTON_CTRL_ID,&sbs);    
    GUIBUTTON_SetRunSheen(VP_PLAY_PANEL_REPEAT_BUTTON_CTRL_ID,FALSE); 
#ifdef TOUCH_PANEL_SUPPORT //@andy.he_MP4PDA
    bg_info.bg_type = GUI_BG_NONE;
#else
    bg_info.bg_type = GUI_BG_IMG;
#endif
    bg_info.img_id = IMAGE_COMMON_MODE_BTN_PRESS;
    GUIBUTTON_SetPressedBg(VP_PLAY_PANEL_REPEAT_BUTTON_CTRL_ID,&bg_info);     
    bg_info.bg_type = GUI_BG_NONE;
    GUIBUTTON_SetBg(VP_PLAY_PANEL_REPEAT_BUTTON_CTRL_ID,&bg_info);

    //随机图标固定宽度，没有光影效果，设置按下的背景图片以及正常没有背景图片
    sbs.type = GUIFORM_CHILD_WIDTH_FIXED;
    GUIRES_GetImgWidthHeight(&sbs.add_data,PNULL,IMAGE_COMMON_MODE_BTN_PRESS,MMIVP_PLAYER_WIN_ID);
    GUIFORM_SetChildWidth(VP_PLAY_PANEL_TITLE_FORM_CTRL_ID,VP_PLAY_PANEL_RANDOM_BUTTON_CTRL_ID,&sbs);  
    GUIBUTTON_SetRunSheen(VP_PLAY_PANEL_RANDOM_BUTTON_CTRL_ID,FALSE);
#ifdef TOUCH_PANEL_SUPPORT //@andy.he_MP4PDA
    bg_info.bg_type = GUI_BG_NONE;
#else
    bg_info.bg_type = GUI_BG_IMG;
#endif
    bg_info.img_id = IMAGE_COMMON_MODE_BTN_PRESS;
    GUIBUTTON_SetPressedBg(VP_PLAY_PANEL_RANDOM_BUTTON_CTRL_ID,&bg_info);   
    bg_info.bg_type = GUI_BG_NONE;
    GUIBUTTON_SetBg(VP_PLAY_PANEL_RANDOM_BUTTON_CTRL_ID,&bg_info);

    //设置title文件显示的颜色
    if (MMI_WHITE_COLOR == MMIAPIVP_GetTransparenceColor())
    {
        font_color = MMI_BLACK_COLOR;
    }
    else
    {
        font_color = MMI_WHITE_COLOR;
    }
    GUILABEL_SetFont(VP_PLAY_NAME_LABEL_CTRL_ID, VP_NAME_FONT, font_color);


    //文件选择图标固定宽度，没有光影效果，设置按下的背景图片以及正常没有背景图片
    sbs.type = GUIFORM_CHILD_WIDTH_FIXED;
#if defined MMI_PDA_SUPPORT && defined(MMI_ISTYLE_SUPPORT)
    if (MMITHEME_IsIstyle() && !MMITHEME_IsMainScreenLandscape())
    {
        GUIRES_GetImgWidthHeight(&sbs.add_data,&height,IMAGE_COMMON_BTN_OPTION,MMIVP_PLAYER_WIN_ID);   
    }
    else
#endif
    {
        GUIRES_GetImgWidthHeight(&sbs.add_data,&height,IMAGE_COMMON_TITLE_BTN_PRESS,MMIVP_PLAYER_WIN_ID);   
    }

#ifdef TOUCH_PANEL_SUPPORT      
    GUIFORM_SetChildWidth(VP_PLAY_PANEL_TITLE_FORM_CTRL_ID,VP_PLAY_PANEL_FILE_BUTTON_CTRL_ID,&sbs); 
    h_space = VP_TITLE_MARGIN;
    GUIFORM_SetChildSpace(VP_PLAY_PANEL_TITLE_FORM_CTRL_ID,VP_PLAY_PANEL_FILE_BUTTON_CTRL_ID,&h_space,PNULL);
    GUIBUTTON_SetRunSheen(VP_PLAY_PANEL_FILE_BUTTON_CTRL_ID,FALSE);
#ifdef TOUCH_PANEL_SUPPORT //@andy.he_MP4PDA
    bg_info.bg_type = GUI_BG_NONE;
#else
    bg_info.bg_type = GUI_BG_IMG;
#endif
    bg_info.img_id = IMAGE_COMMON_TITLE_BTN_PRESS;
    GUIBUTTON_SetPressedBg(VP_PLAY_PANEL_FILE_BUTTON_CTRL_ID,&bg_info);       
    bg_info.bg_type = GUI_BG_NONE;
    GUIBUTTON_SetBg(VP_PLAY_PANEL_FILE_BUTTON_CTRL_ID,&bg_info);
#endif
    
    sbs.add_data = 1;
    GUIFORM_SetChildWidth(VP_PLAY_PANEL_TITLE_FORM_CTRL_ID,VP_PLAY_PANEL_SPACE_OWNDRAW_CTRL_ID,&sbs);  

    GUIRES_GetImgWidthHeight(&sbs.add_data,&height,IMAGE_COMMON_TITLE_BTN_PRESS,MMIVP_PLAYER_WIN_ID);   
    GUIRES_GetImgWidthHeight(PNULL,&title_height,IMAGE_VP_TITLE_BAR,MMIVP_PLAYER_WIN_ID);

    height = MAX(height, title_height);
    GUIRES_GetImgWidthHeight(PNULL,&mode_height,IMAGE_COMMON_MODE_BTN_PRESS,MMIVP_PLAYER_WIN_ID);
    title_height = MAX(height,mode_height);

    #if defined MMI_PDA_SUPPORT && defined(MMI_ISTYLE_SUPPORT)//istyle_return
   if (MMITHEME_IsIstyle() && !MMITHEME_IsMainScreenLandscape())
    {
        GUIRES_GetImgWidthHeight(PNULL,&height,IMAGE_COMMON_BTN_RETURN,MMIVP_PLAYER_WIN_ID);
         title_height = MAX(height,title_height);
    }
    #endif
    //get line height    
    GUIOWNDRAW_SetHeight(VP_PLAY_PANEL_TITLE_FORM_CTRL_ID,title_height);       

    
    /*-------------------------------------------------title栏部分end------------------------------------------------*/

    /*-------------------------------------------------button栏部分begin---------------------------------------------*/
    //设置progress栏的背景
    bg_info.bg_type = GUI_BG_IMG;
    bg_info.img_id = IMAGE_VP_PROCESSBAR_BG;
    GUIOWNDRAW_SetBg(VP_PLAY_PANEL_PROCESS_OWNDRAW_CTRL_ID, &bg_info);

    //设置button栏的背景，margin为0，h_space和v_space为0，所有图标居中显示
    bg_info.bg_type = GUI_BG_IMG;
    bg_info.img_id = IMAGE_VP_BTN_BG;

    GUIFORM_SetBg(VP_PLAY_PANEL_BUTTON_FORM_CTRL_ID, &bg_info);
    GUIFORM_SetMargin(VP_PLAY_PANEL_BUTTON_FORM_CTRL_ID,0);   
    v_space = 0;
    h_space = 0;
    GUIFORM_SetChildSpace(VP_PLAY_PANEL_FORM_CTRL_ID,VP_PLAY_PANEL_BUTTON_FORM_CTRL_ID,&h_space,&v_space);    
    GUIFORM_SetAlign(VP_PLAY_PANEL_BUTTON_FORM_CTRL_ID,GUIFORM_CHILD_ALIGN_HMIDDLE);
    
    
    //音量图标固定宽度，固定间隔，没有光影效果，设置按下的背景图片以及正常没有背景图片   
   // h_space = VP_BUTTON_SPACE; 
    //GUIFORM_SetChildSpace(VP_PLAY_PANEL_BUTTON_FORM_CTRL_ID,VP_PLAY_PANEL_VOL_BUTTON_CTRL_ID,&h_space,PNULL);    
    sbs.type = GUIFORM_CHILD_WIDTH_FIXED;
    GUIRES_GetImgWidthHeight(&sbs.add_data,PNULL,IMAGE_COMMON_PLAYER_BTN_VOL,MMIVP_PLAYER_WIN_ID);
    GUIFORM_SetChildWidth(VP_PLAY_PANEL_BUTTON_FORM_CTRL_ID,VP_PLAY_PANEL_VOL_BUTTON_CTRL_ID,&sbs);  
    GUIBUTTON_SetRunSheen(VP_PLAY_PANEL_VOL_BUTTON_CTRL_ID,FALSE);    

#ifdef TOUCH_PANEL_SUPPORT //@andy.he_MP4PDA
    bg_info.bg_type = GUI_BG_NONE;
#else
    bg_info.bg_type = GUI_BG_IMG;
    bg_info.img_id = IMAGE_COMMON_BTN_PRESS;
#endif
    GUIBUTTON_SetPressedBg(VP_PLAY_PANEL_VOL_BUTTON_CTRL_ID,&bg_info); 
    bg_info.bg_type = GUI_BG_NONE;
    GUIBUTTON_SetBg(VP_PLAY_PANEL_VOL_BUTTON_CTRL_ID,&bg_info);
    

    //上一首图标固定宽度，固定间隔，没有光影效果，设置按下的背景图片以及正常没有背景图片，处理长按消息   
    sbs.type = GUIFORM_CHILD_WIDTH_FIXED;
    GUIRES_GetImgWidthHeight(&sbs.add_data,PNULL,IMAGE_COMMON_BTN_PREV,MMIVP_PLAYER_WIN_ID);
    GUIFORM_SetChildWidth(VP_PLAY_PANEL_BUTTON_FORM_CTRL_ID,VP_PLAY_PANEL_PRE_BUTTON_CTRL_ID,&sbs); 
    GUIBUTTON_SetRunSheen(VP_PLAY_PANEL_PRE_BUTTON_CTRL_ID,FALSE);   
   // h_space = VP_BUTTON_SPACE; 
  //  GUIFORM_SetChildSpace(VP_PLAY_PANEL_BUTTON_FORM_CTRL_ID,VP_PLAY_PANEL_PRE_BUTTON_CTRL_ID,&h_space,PNULL);
#ifdef TOUCH_PANEL_SUPPORT //@andy.he_MP4PDA
    bg_info.bg_type = GUI_BG_NONE;
#else
    bg_info.bg_type = GUI_BG_IMG;
#endif
    bg_info.img_id = IMAGE_COMMON_BTN_PRESS;
    GUIBUTTON_SetPressedBg(VP_PLAY_PANEL_PRE_BUTTON_CTRL_ID,&bg_info);  
    bg_info.bg_type = GUI_BG_NONE;
    GUIBUTTON_SetBg(VP_PLAY_PANEL_PRE_BUTTON_CTRL_ID,&bg_info);
    GUIBUTTON_SetHandleLong(VP_PLAY_PANEL_PRE_BUTTON_CTRL_ID,TRUE);
   

    //播放图标固定宽度，固定间隔，没有光影效果，设置按下的背景图片以及正常没有背景图片   
    sbs.type = GUIFORM_CHILD_WIDTH_FIXED;
    GUIRES_GetImgWidthHeight(&sbs.add_data,PNULL,IMAGE_COMMON_BTN_PLAY,MMIVP_PLAYER_WIN_ID);
    GUIFORM_SetChildWidth(VP_PLAY_PANEL_BUTTON_FORM_CTRL_ID,VP_PLAY_PANEL_PLAY_BUTTON_CTRL_ID,&sbs); 
    GUIBUTTON_SetRunSheen(VP_PLAY_PANEL_PLAY_BUTTON_CTRL_ID,FALSE);  
   // h_space = VP_BUTTON_SPACE; 
    //GUIFORM_SetChildSpace(VP_PLAY_PANEL_BUTTON_FORM_CTRL_ID,VP_PLAY_PANEL_PLAY_BUTTON_CTRL_ID,&h_space,PNULL);
#ifdef TOUCH_PANEL_SUPPORT //@andy.he_MP4PDA
    bg_info.bg_type = GUI_BG_NONE;
#else
    bg_info.bg_type = GUI_BG_IMG;
#endif
    bg_info.img_id = IMAGE_COMMON_BTN_PRESS;
    GUIBUTTON_SetPressedBg(VP_PLAY_PANEL_PLAY_BUTTON_CTRL_ID,&bg_info);  
    bg_info.bg_type = GUI_BG_NONE;
    GUIBUTTON_SetBg(VP_PLAY_PANEL_PLAY_BUTTON_CTRL_ID,&bg_info);
   

    //下一首图标固定宽度，固定间隔，没有光影效果，设置按下的背景图片以及正常没有背景图片，处理长按消息   
    sbs.type = GUIFORM_CHILD_WIDTH_FIXED;
    GUIRES_GetImgWidthHeight(&sbs.add_data,PNULL,IMAGE_COMMON_BTN_NEXT,MMIVP_PLAYER_WIN_ID);
    GUIFORM_SetChildWidth(VP_PLAY_PANEL_BUTTON_FORM_CTRL_ID,VP_PLAY_PANEL_NEXT_BUTTON_CTRL_ID,&sbs); 
    GUIBUTTON_SetRunSheen(VP_PLAY_PANEL_NEXT_BUTTON_CTRL_ID,FALSE);  
    //h_space = VP_BUTTON_SPACE; 
   // GUIFORM_SetChildSpace(VP_PLAY_PANEL_BUTTON_FORM_CTRL_ID,VP_PLAY_PANEL_NEXT_BUTTON_CTRL_ID,&h_space,PNULL);
#ifdef TOUCH_PANEL_SUPPORT //@andy.he_MP4PDA
    bg_info.bg_type = GUI_BG_NONE;
#else
    bg_info.bg_type = GUI_BG_IMG;
#endif
    bg_info.img_id = IMAGE_COMMON_BTN_PRESS;
    GUIBUTTON_SetPressedBg(VP_PLAY_PANEL_NEXT_BUTTON_CTRL_ID,&bg_info);  
    bg_info.bg_type = GUI_BG_NONE;
    GUIBUTTON_SetBg(VP_PLAY_PANEL_NEXT_BUTTON_CTRL_ID,&bg_info);
    GUIBUTTON_SetHandleLong(VP_PLAY_PANEL_NEXT_BUTTON_CTRL_ID,TRUE);

#ifdef MMI_PDA_SUPPORT//@andy.he_MP4PDA
{
    sbs.add_data = 1;
    GUIFORM_SetChildWidth(VP_PLAY_PANEL_BUTTON_FORM_CTRL_ID,VP_PLAY_PANEL_BTN_SPACE_OWNDRAW_CTRL_ID,&sbs);  
    GUIRES_GetImgWidthHeight(&sbs.add_data,&height,IMAGE_COMMON_PLAYER_BTN_VOL,MMIVP_PLAYER_WIN_ID);   
    GUIRES_GetImgWidthHeight(PNULL,&prev_height,IMAGE_COMMON_BTN_PREV,MMIVP_PLAYER_WIN_ID);
    height = MAX(height, prev_height);                     
    GUIRES_GetImgWidthHeight(PNULL,&btn_height,IMAGE_VP_BTN_BG,MMIVP_PLAYER_WIN_ID);
    btn_height = MAX(height,btn_height);
    //get line height    
    GUIOWNDRAW_SetHeight(VP_PLAY_PANEL_BTN_SPACE_OWNDRAW_CTRL_ID,btn_height);       
}
#endif

  
   //全屏播放 图标固定宽度，固定间隔，没有光影效果，设置按下的背景图片以及正常没有背景图片   
   if (IsDisplayFullScreenReferInfoEnable())
   {   
#ifdef TOUCH_PANEL_SUPPORT         
        sbs.type = GUIFORM_CHILD_WIDTH_FIXED;
        GUIRES_GetImgWidthHeight(&sbs.add_data,PNULL,IMAGE_COMMON_BTN_FULLSCREEN,MMIVP_PLAYER_WIN_ID);
        GUIFORM_SetChildWidth(VP_PLAY_PANEL_BUTTON_FORM_CTRL_ID,VP_PLAY_PANEL_FULLSCREEN_BUTTON_CTRL_ID,&sbs); 
        GUIBUTTON_SetRunSheen(VP_PLAY_PANEL_FULLSCREEN_BUTTON_CTRL_ID,FALSE);  
        //h_space = VP_BUTTON_SPACE; 
        //GUIFORM_SetChildSpace(VP_PLAY_PANEL_BUTTON_FORM_CTRL_ID,VP_PLAY_PANEL_FULLSCREEN_BUTTON_CTRL_ID,&h_space,PNULL);
#ifdef TOUCH_PANEL_SUPPORT //@andy.he_MP4PDA
        bg_info.bg_type = GUI_BG_NONE;
#else
        bg_info.bg_type = GUI_BG_IMG;
#endif
        bg_info.img_id = IMAGE_COMMON_BTN_PRESS;
        GUIBUTTON_SetPressedBg(VP_PLAY_PANEL_FULLSCREEN_BUTTON_CTRL_ID,&bg_info);  
        bg_info.bg_type = GUI_BG_NONE;
        GUIBUTTON_SetBg(VP_PLAY_PANEL_FULLSCREEN_BUTTON_CTRL_ID,&bg_info);
   
#else
        GUIFORM_SetChildDisplay(VP_PLAY_PANEL_BUTTON_FORM_CTRL_ID,VP_PLAY_PANEL_FULLSCREEN_BUTTON_CTRL_ID,GUIFORM_CHILD_DISP_HIDE);
#endif
   }
   else
   {
        GUIFORM_SetChildDisplay(VP_PLAY_PANEL_BUTTON_FORM_CTRL_ID,VP_PLAY_PANEL_FULLSCREEN_BUTTON_CTRL_ID,GUIFORM_CHILD_DISP_HIDE);
   }          

   /*-------------------------------------------button栏部分end-----------------------------------------------------------*/

   //非本地列表文件隐藏
    if ((VP_PLAY_FROM_FILE != s_video_player_control_info.play_type
        && VP_PLAY_FROM_LOCALLIST != s_video_player_control_info.play_type))
    {       
        GUIFORM_SetChildDisplay(VP_PLAY_PANEL_TITLE_FORM_CTRL_ID,VP_PLAY_PANEL_REPEAT_BUTTON_CTRL_ID,GUIFORM_CHILD_DISP_HIDE);
        GUIFORM_SetChildDisplay(VP_PLAY_PANEL_TITLE_FORM_CTRL_ID,VP_PLAY_PANEL_RANDOM_BUTTON_CTRL_ID,GUIFORM_CHILD_DISP_HIDE);    
    }
    
#ifndef TOUCH_PANEL_SUPPORT
    //文件选择图标隐藏，需要占位控件的高度
    GUIFORM_SetChildDisplay(VP_PLAY_PANEL_TITLE_FORM_CTRL_ID,VP_PLAY_PANEL_FILE_BUTTON_CTRL_ID,GUIFORM_CHILD_DISP_HIDE);
#endif
}

/*****************************************************************************/
//  Description : SetFormParamV
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
LOCAL void SetFormParamV(void)
{  
    GUIFORM_CHILD_WIDTH_T    sbs = {0};    
    uint16 height_button = 0; 
    uint16 height = 0;
    uint16 height_title = 0;
    uint16 v_space = 0;
    uint16 h_space = 0;
    GUI_RECT_T screen_rect = {0};
    uint16 button_num = 0;
    uint16 width_button = 0;
    uint16 button_space = 0;
    uint16 lcd_width = 0;
    uint16 lcd_height = 0;
    GUIFORM_CHILD_HEIGHT_T child_height = {0};

    //图像显示区域 
    screen_rect = MMITHEME_GetFullScreenRect();
    screen_rect.top = screen_rect.top + MMI_STATUSBAR_HEIGHT;

#if defined MMI_PDA_SUPPORT && defined(MMI_ISTYLE_SUPPORT)
    if (MMITHEME_IsIstyle() && !MMITHEME_IsMainScreenLandscape())
    {
        GUIRES_GetImgWidthHeight(PNULL,&height,IMAGE_COMMON_BTN_OPTION,MMIVP_PLAYER_WIN_ID);   
    } 
    else
#endif
    GUIRES_GetImgWidthHeight(PNULL,&height,IMAGE_COMMON_TITLE_BTN_PRESS,MMIVP_PLAYER_WIN_ID);   
    GUIRES_GetImgWidthHeight(PNULL,&height_title,IMAGE_VP_TITLE_BAR,MMIVP_PLAYER_WIN_ID);

    height_title = MAX(height, height_title);
    GUIRES_GetImgWidthHeight(PNULL,&height,IMAGE_COMMON_MODE_BTN_PRESS,MMIVP_PLAYER_WIN_ID);
    height_title = MAX(height,height_title);
   
    GUIRES_GetImgWidthHeight(PNULL,&height,IMAGE_COMMON_BTN_PRESS,MMIVP_PLAYER_WIN_ID);
    GUIRES_GetImgWidthHeight(PNULL,&height_button,IMAGE_COMMON_BTN_PLAY,MMIVP_PLAYER_WIN_ID);
    height_button = MAX(height_button, height);

//#ifdef MMI_PDA_SUPPORT//@andy.he_MP4PDA
    GUIRES_GetImgWidthHeight(PNULL,&height,IMAGE_VP_BTN_BG,MMIVP_PLAYER_WIN_ID);
    height_button = MAX(height_button, height);
//#endif
  
    v_space = VP_TITLE_MARGIN;   
    GUIFORM_SetChildSpace(VP_PLAY_PANEL_FORM_CTRL_ID,VP_PLAY_PANEL_VIDEO_OWNDRAW_CTRL_ID,PNULL,&v_space);
#ifndef MMI_PDA_SUPPORT
    height = (uint16)((uint16)(screen_rect.bottom - screen_rect.top + 1 - height_button) - height_title - VP_PROCESS_HEIGHT - MMITHEME_GetSoftkeyHeight() - 2*VP_TITLE_MARGIN);
#else
    //PDA has no softkey
    height = (uint16)((uint16)(screen_rect.bottom - screen_rect.top + 1 - height_button) - height_title - VP_PROCESS_HEIGHT - 2*VP_TITLE_MARGIN);
#endif
    GUIOWNDRAW_SetHeight(VP_PLAY_PANEL_VIDEO_OWNDRAW_CTRL_ID,height);    
    GUIFORM_SetChildAlign(VP_PLAY_PANEL_FORM_CTRL_ID,VP_PLAY_PANEL_VIDEO_OWNDRAW_CTRL_ID,GUIFORM_CHILD_ALIGN_HMIDDLE);  
    
    child_height.type = GUIFORM_CHILD_HEIGHT_FIXED;
    child_height.add_data = height_title;
    GUIFORM_SetChildHeight(VP_PLAY_PANEL_FORM_CTRL_ID, VP_PLAY_PANEL_TITLE_FORM_CTRL_ID, &child_height);
    child_height.type = GUIFORM_CHILD_HEIGHT_LEFT_MIN;
    GUIFORM_SetChildHeight(VP_PLAY_PANEL_TITLE_FORM_CTRL_ID, VP_PLAY_NAME_LABEL_CTRL_ID, &child_height);
    
    //进度条区域
    sbs.type = GUIFORM_CHILD_WIDTH_FIXED; 

    GUIRES_GetImgWidthHeight(PNULL,&height,IMAGE_VP_PROCESSBAR_BG,MMIVP_PLAYER_WIN_ID);   
    //height = VP_PROCESS_HEIGHT;
    sbs.add_data = VP_PROCESS_WIDTH;//VP_PROCESS_WIDTH;
    GUIOWNDRAW_SetHeight(VP_PLAY_PANEL_PROCESS_OWNDRAW_CTRL_ID,height);   
    GUIFORM_SetChildWidth(VP_PLAY_PANEL_FORM_CTRL_ID,VP_PLAY_PANEL_PROCESS_OWNDRAW_CTRL_ID,&sbs);
    GUIFORM_SetChildAlign(VP_PLAY_PANEL_FORM_CTRL_ID,VP_PLAY_PANEL_PROCESS_OWNDRAW_CTRL_ID,GUIFORM_CHILD_ALIGN_HMIDDLE);

    v_space = VP_TITLE_MARGIN;
#if defined MAINLCD_SIZE_240X320 && !defined TOUCH_PANEL_SUPPORT
    v_space = VP_TITLE_MARGIN + 1; //目的是为了使参数四对齐 bug148786
#endif
    h_space = 0;
    GUIFORM_SetChildSpace(VP_PLAY_PANEL_FORM_CTRL_ID,VP_PLAY_PANEL_PROCESS_OWNDRAW_CTRL_ID,&h_space,&v_space);     
    
    child_height.type = GUIFORM_CHILD_HEIGHT_FIXED;
    child_height.add_data = height_button;
    GUIFORM_SetChildHeight(VP_PLAY_PANEL_FORM_CTRL_ID, VP_PLAY_PANEL_BUTTON_FORM_CTRL_ID, &child_height);   
    GUIFORM_SetChildHeight(VP_PLAY_PANEL_BUTTON_FORM_CTRL_ID, VP_PLAY_PANEL_VOL_BUTTON_CTRL_ID, &child_height);   
    GUIFORM_SetChildHeight(VP_PLAY_PANEL_BUTTON_FORM_CTRL_ID, VP_PLAY_PANEL_PRE_BUTTON_CTRL_ID, &child_height);   
    GUIFORM_SetChildHeight(VP_PLAY_PANEL_BUTTON_FORM_CTRL_ID, VP_PLAY_PANEL_PLAY_BUTTON_CTRL_ID, &child_height);   
    GUIFORM_SetChildHeight(VP_PLAY_PANEL_BUTTON_FORM_CTRL_ID, VP_PLAY_PANEL_NEXT_BUTTON_CTRL_ID, &child_height);   
    GUIFORM_SetChildHeight(VP_PLAY_PANEL_BUTTON_FORM_CTRL_ID, VP_PLAY_PANEL_FULLSCREEN_BUTTON_CTRL_ID, &child_height);   

    button_num = 4; 
#ifdef TOUCH_PANEL_SUPPORT         
   if (IsDisplayFullScreenReferInfoEnable())
   { 
        button_num ++; 
   }
#endif

    GUIRES_GetImgWidthHeight(&width_button,PNULL,IMAGE_COMMON_BTN_PLAY,MMIVP_PLAYER_WIN_ID);
    GUILCD_GetLogicWidthHeight(MAIN_LCD_ID,&lcd_width,&lcd_height);
    button_space = (lcd_width - (width_button*button_num))/(button_num)-1;

    GUIFORM_SetChildSpace(VP_PLAY_PANEL_BUTTON_FORM_CTRL_ID,VP_PLAY_PANEL_VOL_BUTTON_CTRL_ID,&button_space,PNULL);    
    GUIFORM_SetChildSpace(VP_PLAY_PANEL_BUTTON_FORM_CTRL_ID,VP_PLAY_PANEL_PRE_BUTTON_CTRL_ID,&button_space,PNULL);
    GUIFORM_SetChildSpace(VP_PLAY_PANEL_BUTTON_FORM_CTRL_ID,VP_PLAY_PANEL_PLAY_BUTTON_CTRL_ID,&button_space,PNULL);
    GUIFORM_SetChildSpace(VP_PLAY_PANEL_BUTTON_FORM_CTRL_ID,VP_PLAY_PANEL_NEXT_BUTTON_CTRL_ID,&button_space,PNULL);
    GUIFORM_SetChildSpace(VP_PLAY_PANEL_BUTTON_FORM_CTRL_ID,VP_PLAY_PANEL_FULLSCREEN_BUTTON_CTRL_ID,&button_space,PNULL);
    
 }

/*****************************************************************************/
//  Description : SetFormParamH
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
LOCAL void SetFormParamH(void)
{
    GUIFORM_CHILD_WIDTH_T    sbs = {0}; 
    uint16  h_space = VP_BUTTON_SPACE;
    uint16 height_button = 0; 
    uint16 height = 0;
    uint16 height_title = 0;
    GUI_BG_T bg_info = {0};
    GUI_RECT_T screen_rect = {0};
    uint16 v_space = 0;
    uint16 button_num = 0;
    uint16 width_button = 0;
    uint16 button_space = 0;
    uint16 lcd_width;
    uint16 lcd_height;
    GUIFORM_CHILD_HEIGHT_T child_height = {0};
  
    //图像显示区域 
    screen_rect = MMITHEME_GetFullScreenRect();
    screen_rect.top = screen_rect.top + MMI_STATUSBAR_HEIGHT;
    v_space = VP_TITLE_MARGIN;   
    GUIFORM_SetChildSpace(VP_PLAY_PANEL_FORM_CTRL_ID,VP_PLAY_PANEL_VIDEO_OWNDRAW_CTRL_ID,PNULL,&v_space);

    GUIRES_GetImgWidthHeight(PNULL,&height,IMAGE_COMMON_TITLE_BTN_PRESS,MMIVP_PLAYER_WIN_ID);   
    GUIRES_GetImgWidthHeight(PNULL,&height_title,IMAGE_VP_TITLE_BAR,MMIVP_PLAYER_WIN_ID);

    height_title = MAX(height, height_title);
    GUIRES_GetImgWidthHeight(PNULL,&height,IMAGE_COMMON_MODE_BTN_PRESS,MMIVP_PLAYER_WIN_ID);
    height_title = MAX(height,height_title);    

    GUIRES_GetImgWidthHeight(PNULL,&height,IMAGE_COMMON_BTN_PRESS,MMIVP_PLAYER_WIN_ID);
    GUIRES_GetImgWidthHeight(PNULL,&height_button,IMAGE_COMMON_BTN_PLAY,MMIVP_PLAYER_WIN_ID);
    height_button = MAX(height_button, height);

//#ifdef MMI_PDA_SUPPORT//@andy.he_MP4PDA
    GUIRES_GetImgWidthHeight(PNULL,&height,IMAGE_VP_BTN_BG,MMIVP_PLAYER_WIN_ID);
    height_button = MAX(height_button, height);
//#endif
    height = screen_rect.bottom - screen_rect.top + 1 - height_title - height_button - VP_PROCESS_HEIGHT_H - 2*VP_TITLE_MARGIN;
    GUIOWNDRAW_SetHeight(VP_PLAY_PANEL_VIDEO_OWNDRAW_CTRL_ID,height);    
    GUIFORM_SetChildAlign(VP_PLAY_PANEL_FORM_CTRL_ID,VP_PLAY_PANEL_VIDEO_OWNDRAW_CTRL_ID,GUIFORM_CHILD_ALIGN_HMIDDLE);  

    child_height.type = GUIFORM_CHILD_HEIGHT_FIXED;
    child_height.add_data = height_title;
    GUIFORM_SetChildHeight(VP_PLAY_PANEL_FORM_CTRL_ID, VP_PLAY_PANEL_TITLE_FORM_CTRL_ID, &child_height);
    child_height.type = GUIFORM_CHILD_HEIGHT_LEFT_MIN;
    GUIFORM_SetChildHeight(VP_PLAY_PANEL_TITLE_FORM_CTRL_ID, VP_PLAY_NAME_LABEL_CTRL_ID, &child_height);
    
    //进度条区域
    sbs.type = GUIFORM_CHILD_WIDTH_FIXED;    
    height = VP_PROCESS_HEIGHT_H;
    sbs.add_data = VP_PROCESS_WIDTH_H;
    GUIOWNDRAW_SetHeight(VP_PLAY_PANEL_PROCESS_OWNDRAW_CTRL_ID,height);   
    GUIFORM_SetChildWidth(VP_PLAY_PANEL_FORM_CTRL_ID,VP_PLAY_PANEL_PROCESS_OWNDRAW_CTRL_ID,&sbs);
    GUIFORM_SetChildAlign(VP_PLAY_PANEL_FORM_CTRL_ID,VP_PLAY_PANEL_PROCESS_OWNDRAW_CTRL_ID,GUIFORM_CHILD_ALIGN_HMIDDLE);
    v_space = VP_TITLE_MARGIN;
    GUIFORM_SetChildSpace(VP_PLAY_PANEL_FORM_CTRL_ID,VP_PLAY_PANEL_PROCESS_OWNDRAW_CTRL_ID,PNULL,&v_space);   
    

    child_height.type = GUIFORM_CHILD_HEIGHT_FIXED;
    child_height.add_data = height_button;
    GUIFORM_SetChildHeight(VP_PLAY_PANEL_FORM_CTRL_ID, VP_PLAY_PANEL_BUTTON_FORM_CTRL_ID, &child_height);   
    GUIFORM_SetChildHeight(VP_PLAY_PANEL_BUTTON_FORM_CTRL_ID, VP_PLAY_PANEL_OPT_BUTTON_CTRL_ID, &child_height);   
    GUIFORM_SetChildHeight(VP_PLAY_PANEL_BUTTON_FORM_CTRL_ID, VP_PLAY_PANEL_VOL_BUTTON_CTRL_ID, &child_height);   
    GUIFORM_SetChildHeight(VP_PLAY_PANEL_BUTTON_FORM_CTRL_ID, VP_PLAY_PANEL_PRE_BUTTON_CTRL_ID, &child_height);   
    GUIFORM_SetChildHeight(VP_PLAY_PANEL_BUTTON_FORM_CTRL_ID, VP_PLAY_PANEL_PLAY_BUTTON_CTRL_ID, &child_height);   
    GUIFORM_SetChildHeight(VP_PLAY_PANEL_BUTTON_FORM_CTRL_ID, VP_PLAY_PANEL_NEXT_BUTTON_CTRL_ID, &child_height);   
    GUIFORM_SetChildHeight(VP_PLAY_PANEL_BUTTON_FORM_CTRL_ID, VP_PLAY_PANEL_FULLSCREEN_BUTTON_CTRL_ID, &child_height);   
    GUIFORM_SetChildHeight(VP_PLAY_PANEL_BUTTON_FORM_CTRL_ID, VP_PLAY_PANEL_RETURN_BUTTON_CTRL_ID, &child_height);   
    

    //选项图标固定宽度，固定间隔，没有光影效果，设置按下的背景图片以及正常没有背景图片  
    sbs.type = GUIFORM_CHILD_WIDTH_FIXED;
    GUIRES_GetImgWidthHeight(&sbs.add_data,PNULL,IMAGE_COMMON_BTN_OPTION,MMIVP_PLAYER_WIN_ID);
    GUIFORM_SetChildWidth(VP_PLAY_PANEL_BUTTON_FORM_CTRL_ID,VP_PLAY_PANEL_OPT_BUTTON_CTRL_ID,&sbs); 
    GUIBUTTON_SetRunSheen(VP_PLAY_PANEL_OPT_BUTTON_CTRL_ID,FALSE);
    h_space = VP_BUTTON_SPACE;
    GUIFORM_SetChildSpace(VP_PLAY_PANEL_BUTTON_FORM_CTRL_ID,VP_PLAY_PANEL_OPT_BUTTON_CTRL_ID,&h_space,PNULL);
#ifdef TOUCH_PANEL_SUPPORT //@andy.he_MP4PDA
    bg_info.bg_type = GUI_BG_NONE;
#else
    bg_info.bg_type = GUI_BG_IMG;
#endif
    bg_info.img_id = IMAGE_COMMON_BTN_PRESS;
    GUIBUTTON_SetPressedBg(VP_PLAY_PANEL_OPT_BUTTON_CTRL_ID,&bg_info);  
    bg_info.bg_type = GUI_BG_NONE;
    GUIBUTTON_SetBg(VP_PLAY_PANEL_OPT_BUTTON_CTRL_ID,&bg_info);

    //返回图标固定宽度，固定间隔，没有光影效果，设置按下的背景图片以及正常没有背景图片  
    sbs.type = GUIFORM_CHILD_WIDTH_FIXED;
    GUIRES_GetImgWidthHeight(&sbs.add_data,PNULL,IMAGE_COMMON_BTN_RETURN,MMIVP_PLAYER_WIN_ID);
    GUIFORM_SetChildWidth(VP_PLAY_PANEL_BUTTON_FORM_CTRL_ID,VP_PLAY_PANEL_RETURN_BUTTON_CTRL_ID,&sbs); 
    GUIBUTTON_SetRunSheen(VP_PLAY_PANEL_RETURN_BUTTON_CTRL_ID,FALSE);  
    //h_space = VP_BUTTON_SPACE;
   // GUIFORM_SetChildSpace(VP_PLAY_PANEL_BUTTON_FORM_CTRL_ID,VP_PLAY_PANEL_RETURN_BUTTON_CTRL_ID,&h_space,PNULL);
#ifdef TOUCH_PANEL_SUPPORT //@andy.he_MP4PDA
    bg_info.bg_type = GUI_BG_NONE;
#else
    bg_info.bg_type = GUI_BG_IMG;
#endif
    bg_info.img_id = IMAGE_COMMON_BTN_PRESS;
    GUIBUTTON_SetPressedBg(VP_PLAY_PANEL_RETURN_BUTTON_CTRL_ID,&bg_info);  
    bg_info.bg_type = GUI_BG_NONE;
    GUIBUTTON_SetBg(VP_PLAY_PANEL_RETURN_BUTTON_CTRL_ID,&bg_info);

    button_num = 6; 
#ifdef TOUCH_PANEL_SUPPORT         
   if (IsDisplayFullScreenReferInfoEnable())
   { 
        button_num ++; 
   }
#endif

    GUIRES_GetImgWidthHeight(&width_button,PNULL,IMAGE_COMMON_BTN_PLAY,MMIVP_PLAYER_WIN_ID);
    GUILCD_GetLogicWidthHeight(MAIN_LCD_ID,&lcd_width,&lcd_height);
    button_space = (lcd_width - (width_button*button_num))/(button_num)-1;

    GUIFORM_SetChildSpace(VP_PLAY_PANEL_BUTTON_FORM_CTRL_ID,VP_PLAY_PANEL_OPT_BUTTON_CTRL_ID,&button_space,PNULL);
    GUIFORM_SetChildSpace(VP_PLAY_PANEL_BUTTON_FORM_CTRL_ID,VP_PLAY_PANEL_VOL_BUTTON_CTRL_ID,&button_space,PNULL);    
    GUIFORM_SetChildSpace(VP_PLAY_PANEL_BUTTON_FORM_CTRL_ID,VP_PLAY_PANEL_PRE_BUTTON_CTRL_ID,&button_space,PNULL);
    GUIFORM_SetChildSpace(VP_PLAY_PANEL_BUTTON_FORM_CTRL_ID,VP_PLAY_PANEL_PLAY_BUTTON_CTRL_ID,&button_space,PNULL);
    GUIFORM_SetChildSpace(VP_PLAY_PANEL_BUTTON_FORM_CTRL_ID,VP_PLAY_PANEL_NEXT_BUTTON_CTRL_ID,&button_space,PNULL);
    GUIFORM_SetChildSpace(VP_PLAY_PANEL_BUTTON_FORM_CTRL_ID,VP_PLAY_PANEL_FULLSCREEN_BUTTON_CTRL_ID,&button_space,PNULL);
    GUIFORM_SetChildSpace(VP_PLAY_PANEL_BUTTON_FORM_CTRL_ID,VP_PLAY_PANEL_RETURN_BUTTON_CTRL_ID,&button_space,PNULL);
    
}
#endif //MMI_VIDEO_PLAYER_MINI_LAYOUT //6530_MINI
/*****************************************************************************/
//  Description : 设置osd和透明色
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
LOCAL void SetBlockType(uint32 alpha)
{    
   GUI_COLOR_T             color_key       =    MMIAPIVP_GetTransparenceColor();
   GUIBLOCK_SetType(alpha,color_key,GUIBLOCK_TYPE_OSD,(GUI_LCD_DEV_INFO *)MMITHEME_GetDefaultLcdDev());    
}
#ifdef SCREENSAVER_SUPPORT

/*****************************************************************************/
//  Description : 设置osd和透明色，某些情况下 需要再每次播放时需要设置一次，如屏保界面
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void VP_SetBlockTypeForSS(void)
{
    if(MMK_IsFocusWin(MMIIDLE_SCREENSAVER_WIN_ID))
    {
          GUIBLOCK_SetType(255,MMIAPIVP_GetTransparenceColor(),GUIBLOCK_TYPE_OSD,(GUI_LCD_DEV_INFO *)MMITHEME_GetDefaultLcdDev());
    }
 }
#endif
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void VPRequestVirtualHandle(void)
{
    MMISRVMGR_SERVICE_REQ_T req = {0};
    MMISRVAUD_TYPE_T audio_srv = {0};

    if(0 == s_video_player_control_info.srv_handle)
    {
        req.pri = MMISRVAUD_PRI_NORMAL;
        audio_srv.volume =  MMIAPISET_GetMultimVolume();
        audio_srv.volume_type = MMISRVAUD_VOLUME_TYPE_MED;        
        audio_srv.info.type = MMISRVAUD_TYPE_VIRTUAL;
        
        s_video_player_control_info.srv_handle = MMISRVMGR_Request(STR_SRV_AUD_NAME, &req, &audio_srv);
    }
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void VPReleaseVirtualHandle(void)
{
    if(0 != s_video_player_control_info.srv_handle)
    {
        MMISRVMGR_Free(s_video_player_control_info.srv_handle);
        s_video_player_control_info.srv_handle = 0;
    }
}

#ifdef MMI_VIDEOPLAYER_FIRST_SEARCH
/*****************************************************************************/
//  Description : 加载相关列表文件
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/

LOCAL void VP_LocalList_Load(void)
{
    VP_LocalList_LoadUserLocalList();
#ifdef MMI_VIDEOPLAYER_HISTORYLIST_SUPPORT             
    //加载历史记录列表
    VP_HistoryList_Load();
#endif
#ifdef MMI_STREAMING_PLAYER_SUPPORT
    //加载书签列表
    VP_BookmarkList_Load();
#endif
     if (VP_ENTRY_FROM_PLAYER == s_video_player_control_info.vp_entry)
    {
        //获取最后一次播放的信息
        if(!LoadLastPlayingFile())
        {
            LoadFirstLocalListFile();
        }
        #ifdef KING_MOVIE_SUPPORT
        s_video_player_control_info.is_kmv_file = MMIAPICOM_IsKingMovieType(s_video_player_control_info.full_path_name,s_video_player_control_info.full_path_len);    
        #endif
     }   

 }
#endif

/*****************************************************************************/
//  Description : 视频播放器主界面消息处理.
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleVPWinMsg(
                                     MMI_WIN_ID_T       win_id,     
                                     MMI_MESSAGE_ID_E    msg_id, 
                                     DPARAM              param
                                     )
{
    MMI_RESULT_E            recode  = MMI_RESULT_TRUE; 
    BOOLEAN is_full_screen = TRUE;
    MMI_STRING_T prompt_str = {0};
    uint32 process_time = 0;
    BOOLEAN  is_save_processtime=FALSE;
    
    msg_id = MMIAPICOM_ConvertKeyMsg(msg_id);
    //VP_STRACE_LOW:"[VP]: HandleVPWinMsg(), msg_id = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_6762_112_2_18_3_5_41_468,(uint8*)"d", msg_id);
    
    switch(msg_id)
    {    
    case MSG_OPEN_WINDOW:  
        s_is_full_paint = FALSE;
        s_is_complusive_pause = FALSE;
        s_video_player_control_info.state = VP_STATE_READY;
        s_video_player_control_info.is_display_win = TRUE;
        s_video_player_control_info.cur_angle = MMK_GetScreenAngle();
       // VPRequestVirtualHandle();
        //SetBlockType(VP_HALF_TRANSPARENCE);       
#ifdef PDA_UI_DROPDOWN_WIN
        GUIWIN_SetStbState(win_id, GUISTATUSBAR_STATE_INVALID, TRUE); 
#endif
        VP_Setting_SetBrightness(MMIAPISET_GetMainLCDContrast());                            
#ifdef MMI_STREAMING_PLAYER_SUPPORT
        s_is_need_auth = FALSE;     
        MMIAPIVP_IsSetSpecialPause(FALSE);
#endif
   
#ifdef MMI_VIDEO_PLAYER_MINI_LAYOUT//6530 MINI 界面布局
        SetVPLayoutParam();
#else
        SetFormParam(); 
#endif

        if (VP_ENTRY_FROM_PLAYER == s_video_player_control_info.vp_entry)
        {
            s_is_full_paint=TRUE;
            DisplayVpWinDefaultPic();
            DisplayVPWinProgressEx(PNULL); 
            //MMK_UpdateScreen();
            s_is_full_paint=FALSE;
#ifdef MMI_VIDEOPLAYER_FIRST_SEARCH
            MMI_GetLabelTextByLang(TXT_COMMON_WAITING, &prompt_str);
#else
            MMI_GetLabelTextByLang(TXT_COMMON_SEARCHING, &prompt_str);
#endif

            //开始加载本地播放列表,历史记录列表和书签
            MMIPUB_OpenWaitWin(1,&prompt_str,PNULL,PNULL,MMIVP_LOADING_LOCALLIST_WAIT_WIN_ID,PNULL,ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_NONE,HandleVPSearchWaitingWin);  
#ifdef MMI_VIDEOPLAYER_FIRST_SEARCH
            MMK_PostMsg(MMIVP_LOADING_LOCALLIST_WAIT_WIN_ID,MSG_VP_CHECK_NEED_RESEARCH,PNULL,PNULL);
#else
            MMK_PostMsg(MMIVP_LOADING_LOCALLIST_WAIT_WIN_ID,MSG_VP_LOADING_BEGIN,PNULL,PNULL);
#endif

            // MMK_SendMsg(MMIVP_LOADING_LOCALLIST_WAIT_WIN_ID,MSG_VP_LOADING_BEGIN,PNULL);
        }
        else
        {   
            SetBlockType(VP_HALF_TRANSPARENCE); 
#ifdef MMI_VIDEO_PLAYER_MINI_LAYOUT//6530 MINI 界面布局 //6530_MINI 进入视频播放器直接全屏
        if(LCD_ANGLE_0 == MMK_GetScreenAngle())
        {
            MMK_SetWinSupportAngle(MMIVP_MAIN_PLAY_WIN_ID, WIN_SUPPORT_ANGLE_90);
            MMK_SetWinAngle(MMIVP_MAIN_PLAY_WIN_ID, LCD_ANGLE_90, TRUE);
        }
#endif
        }
#ifdef UI_MULTILAYER_SUPPORT
    UILAYER_SetMainLayerMemFormat(UILAYER_MEM_NONE);
#endif       
        break;

    case MSG_LCD_SWITCH:      
#ifdef TOUCH_PANEL_SUPPORT
    if(VP_PLAY_PANEL_PRE_BUTTON_CTRL_ID == s_tp_down_ctrl_id   || VP_PLAY_PANEL_NEXT_BUTTON_CTRL_ID == s_tp_down_ctrl_id)
    {
        MMK_PostMsg(win_id, MSG_TP_PRESS_UP, PNULL, 0);
    }
#endif       
        if(MMIAPICOM_IsPanelOpened())
        {
            MMIAPICOM_ClosePanelChildWin();
        }
        s_is_full_paint = FALSE;
#ifdef MMI_PDA_SUPPORT//istyle_return
        MMK_CreateCtrlByWinTabEx( win_id,  (uint32 *)(MMITHEME_IsMainScreenLandscape() ? MMIVP_MAIN_PLAY_WIN_TAB_H : ((MMITHEME_IsIstyle() ? MMIVP_MAIN_PLAY_WIN_TAB_V_ISTYLE : MMIVP_MAIN_PLAY_WIN_TAB_V))) );
#else
        MMK_CreateCtrlByWinTabEx( win_id,  (uint32 *)(MMITHEME_IsMainScreenLandscape() ? MMIVP_MAIN_PLAY_WIN_TAB_H : MMIVP_MAIN_PLAY_WIN_TAB_V) );
#endif
#ifdef PDA_UI_DROPDOWN_WIN
        GUIWIN_SetStbState(win_id, GUISTATUSBAR_STATE_INVALID, TRUE); 
#endif
#ifdef  VIDEOTHUMBNAIL_SUPPORT  
     if(VP_STATE_PAUSED == s_video_player_control_info.state && s_vp_dphandle_getthumbnail.is_getthumbnail_by_dpHandle )
    {
        VPThumbnailResetDPLAYERParam();
        MMIAPIVP_ThumbnailClose();
    }
#endif    
#ifdef MMI_VIDEO_PLAYER_MINI_LAYOUT//6530 MINI 界面布局
        SetVPLayoutParam();
#else
        SetFormParam(); 
#endif
        s_is_lcd_switch = TRUE; //s_is_lcd_switch = TRUE must be added after SetFormParam();
        break;
        
    case MSG_END_FULL_PAINT:
        if(s_is_refresh_switch)
        {
            if (!MMITHEME_IsMainScreenLandscape())
            {   
                is_full_screen = FALSE;
            }            
            else if (LCD_ANGLE_90 == GUILCD_GetLogicAngle(GUI_MAIN_LCD_ID)
                || LCD_ANGLE_270 == GUILCD_GetLogicAngle(GUI_MAIN_LCD_ID)
                )
            {
                is_full_screen = TRUE;
            }
            else
            {
                is_full_screen = FALSE;
            }
            SwitchScreen(is_full_screen);
            s_is_refresh_switch = FALSE;
    #ifdef  VIDEOTHUMBNAIL_SUPPORT
        if(VP_TO_DO_NONE == s_video_player_control_info.to_do_type && VP_STATE_PAUSED == s_video_player_control_info.state && MMK_IsFocusWin(MMIVP_PLAYER_WIN_ID) && s_is_get_focus && !s_is_refresh_switch)
        {
            NotifyDPLAYERGetTumbnail(TRUE); 
        }
        s_is_get_focus=FALSE;
    #endif     
        }
        break;
 
    case MSG_FULL_PAINT:
        UpdateVPWin(FALSE);        
        if (MMK_IsFocusWin(win_id)
            && VP_TO_DO_PLAY_FOR_FULL_PAINT == s_video_player_control_info.to_do_type)
        {
            MMK_PostMsg(win_id,MSG_VP_BEGIN_PLAY,PNULL,PNULL);
            s_video_player_control_info.to_do_type = VP_TO_DO_NONE;
        }
        else if(MMK_IsFocusWin(win_id)
            && VP_TO_DO_STOP == s_video_player_control_info.to_do_type)
        {
            s_video_player_control_info.to_do_type = VP_TO_DO_NONE;
            VideoPlayerStop(win_id);    
        }
        s_is_full_paint = TRUE;
        DisplayVPWinRepeatModeIcon(TRUE);  //会立即刷新
        DisplayVPWinRandomModeIcon(TRUE);       
        if (!MMK_IsFocusWin(win_id)
            && !s_video_player_control_info.is_display_win)
        {           
            GUI_FillRect(MMITHEME_GetDefaultLcdDev(), MMITHEME_GetFullScreenRect(), MMIAPIVP_GetTransparenceColor());
        }
        if(MMK_IsFocusWin(win_id)
            && s_video_player_control_info.is_adjust_bright)
        {
            MMK_PostMsg(MMIVP_PLAYER_WIN_ID,MSG_VP_OPEN_BRIGHT_CONTROL_PANE,PNULL,PNULL);  
            s_video_player_control_info.is_adjust_bright = FALSE;
        }
        break;   

    case MSG_TIMER:
        //VP_STRACE_LOW:"[VP] HandleVPWinMsg param = %d, s_video_player_control_info.timer.timer_id= %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_6857_112_2_18_3_5_41_469,(uint8*)"dd",*((uint8 *)param),s_video_player_control_info.timer.timer_id);
        if (s_video_player_control_info.display_state_timer_id == *((uint8 *)param))
        {           
            s_video_player_control_info.display_state_timer_id = 0;    
            HandleVPDisplayStationTimer();                  
        } 
        else if (s_video_player_control_info.timer.timer_id == *((uint8 *)param))
        {
            HandleVPTimerMsg();
        }      
        else if (s_process_timer_id == *((uint8 *)param))
        {
            HandleProcessTimer();
        }
        else if(s_video_player_control_info.display_win_timer_id == *((uint8 *)param))
        {
            if (MMITHEME_IsMainScreenLandscape()
                && s_video_player_control_info.is_display_win
                && VP_STATE_PLAYING == s_video_player_control_info.state
                && MMK_IsFocusWin(win_id))  //全屏播放状态下才可以消失
            {           
                DisappearVPWin(win_id,TRUE,TRUE);
#ifdef MMI_VIDEO_PLAYER_MINI_LAYOUT//6530 MINI 界面布局 LABEL控件不失焦时会常显
        MMK_PostMsg(VP_PLAY_NAME_LABEL_CTRL_ID, MSG_CTL_LOSE_ACTIVE, 0, 0);
#endif

            }
            s_video_player_control_info.display_win_timer_id = 0;
        }

        break;
       
    case MSG_APP_RED:
#ifdef  VIDEOTHUMBNAIL_SUPPORT  
     if(VP_STATE_PAUSED == s_video_player_control_info.state && s_vp_dphandle_getthumbnail.is_getthumbnail_by_dpHandle )
    {
        VPThumbnailResetDPLAYERParam();
        MMIAPIVP_ThumbnailClose();
    }
#endif
//        MMIAPIVP_FSMExit(s_video_player_control_info.vp_handle);        
        recode = MMI_RESULT_FALSE;
        break;

    case MSG_CLOSE_WINDOW:
#ifdef MMI_STREAMING_PLAYER_SUPPORT
        s_is_need_auth = FALSE;
        MMIAPIVP_IsSetSpecialPause(FALSE);
#endif
        s_is_full_paint = FALSE;
        s_is_complusive_pause = FALSE;
#ifdef MMI_STREAMING_PLAYER_SUPPORT
        MMK_CloseWin(MMIVP_AUTH_WIN_ID);
#endif
        StopPlayerAllTimer();  
        is_save_processtime = IsSaveLastPlayPoint();
        process_time = s_video_player_control_info.process_time;
       // VPReleaseVirtualHandle();
        MMIAPIVP_FSMRelease(s_video_player_control_info.vp_handle);  
  
#ifdef MMI_VIDEOPLAYER_HISTORYLIST_SUPPORT
        if (is_save_processtime)
        {   
            VP_HistoryList_SetLastPlayPoint(process_time);
        }
#endif
#ifdef KING_MOVIE_SUPPORT
     if(s_video_player_control_info.is_kmv_file) 
    {
         if (is_save_processtime && PNULL != s_video_player_control_info.post_progrecss)
         {
             s_video_player_control_info.post_progrecss(s_video_player_control_info.full_path_name,s_video_player_control_info.full_path_len,process_time);
         }
    }
#endif

        SCI_MEMSET(&s_video_player_control_info,0,sizeof( VP_INTERFACE_PLAYING_INFO_T));

#ifdef MMI_STREAMING_PLAYER_SUPPORT
        //释放书签资源
        VP_BookmarkList_Release();
#endif
#ifdef MMI_VIDEOPLAYER_HISTORYLIST_SUPPORT
        VP_HistoryList_Release();
#endif
        VP_LocalList_Release(VP_LOCALLIST_DEFAULT);
        VP_LocalList_Release(VP_LOCALLIST_USER);
        GUIBLOCK_ResetMainType();
        MMIDEFAULT_AllowTurnOffBackLight(TRUE); 
        MMIAPISET_ResetContrast(GUI_MAIN_LCD_ID);
        //关闭3d效果
        //MMI_Enable3DMMI(TRUE);
#ifdef UI_P3D_SUPPORT
        MMI_Enable3DEffect(MMI_3D_EFFECT_VDOPLY);
#endif        
#ifdef COUNTEDTIME_SUPPORT
        MMIAPICT_CheckandStartDelayedCoutimeEvent();//弹出延迟的倒计时到提示框
#endif
#ifdef  VIDEOTHUMBNAIL_SUPPORT  
        DestroyLandscapeThumbnailLayer();
#endif
#ifdef  VIDEOTHUMBNAIL_SUPPORT  
        MMIAPIVP_ThumbnailClose();
#endif
#ifdef CMCC_UI_STYLE
        if (!MMIAPIALM_CheckAndStartDelayedAlarm())// 弹出被延迟的闹钟或者日程提示
        {
#ifdef COUNTEDTIME_SUPPORT
            MMIAPICT_CheckandStartDelayedCoutimeEvent();//弹出延迟的倒计时到提示
#endif
        }

        MMIAPISMS_CheckAndShowNewMsg();
#endif
#ifdef UI_MULTILAYER_SUPPORT
     UILAYER_SetMainLayerMemFormat(UILAYER_MEM_DOUBLE_COPY);
#endif
        break;       
        
        
    case MSG_APP_WEB:  
    case MSG_CTL_MIDSK:
        //VP_STRACE_LOW:"[VP] handle vp win web key s_video_player_control_info.state = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_6963_112_2_18_3_5_42_470,(uint8*)"d",s_video_player_control_info.state);
        if (IsNeedDisplayControlPanel())   //显示控制面板
        {
            DisappearVPWin(win_id,FALSE,TRUE);
            StartVPWinDisplayerTimer();
        }
        PlayOrPause();                             
        break;
        
        
    case MSG_APP_MENU:
    case MSG_APP_OK:    
    case MSG_CTL_OK:        
        if (IsOpenOtherWinEnable())        
        {   
            MMK_CreateWin((uint32 *)MMIVP_OPT_MENU_WIN_TAB,PNULL);     
            GUIMENU_CreatDynamic(PNULL,MMIVP_OPT_MENU_WIN_ID,VP_OPT_MENU_CTRL_ID,GUIMENU_STYLE_POPUP);   
#ifdef MMI_VIDEO_PLAYER_MINI_LAYOUT//6530 MINI 界面布局 //6530_MINI option菜单title刷新
            GUIMENU_SetState(VP_OPT_MENU_CTRL_ID,GUIMENU_STATE_CUSTOMER_TITLE,TRUE);
#endif 
        }
        break;
        
        
    case MSG_APP_CANCEL:    //停止播放      
    case MSG_CTL_CANCEL: 
        if (VP_STATE_READY == s_video_player_control_info.state)
        {           
            MMK_CloseWin(win_id);
        } 
        else
        {
            process_time = s_video_player_control_info.process_time;
            is_save_processtime = IsSaveLastPlayPoint();
            VideoPlayerStop(win_id);       
#ifdef MMI_VIDEOPLAYER_HISTORYLIST_SUPPORT
            if (is_save_processtime)
            {
                //vp记录当前进度
                VP_HistoryList_SetLastPlayPoint(process_time);            
            }
#endif
#ifdef KING_MOVIE_SUPPORT
            if(s_video_player_control_info.is_kmv_file)
            {
                //km记录当前进度
                if(is_save_processtime && PNULL != s_video_player_control_info.post_progrecss)
                {
                    s_video_player_control_info.post_progrecss(s_video_player_control_info.full_path_name,s_video_player_control_info.full_path_len,process_time);
                }
            }
#endif
                   
        }
        break;        

    case MSG_KEYUP_SDCARD_DETECT:    //停止播放      
        if (VP_STATE_READY != s_video_player_control_info.state)
        {           
            VideoPlayerStop(win_id);
        }
        MMK_CloseWin(win_id);
        break;
        
    case MSG_LOSE_FOCUS:      
 #ifdef  VIDEOTHUMBNAIL_SUPPORT  
         if(VP_STATE_PAUSED == s_video_player_control_info.state && s_vp_dphandle_getthumbnail.is_getthumbnail_by_dpHandle )
        {
            VPThumbnailResetDPLAYERParam();
            MMIAPIVP_ThumbnailClose();
        }
 #endif       

        if(s_key_down_msg_id == MSG_KEYLONG_RIGHT  || s_key_down_msg_id == MSG_KEYLONG_LEFT)
        {
            Forward(FALSE,FALSE);       
        }

        s_tp_down_ctrl_id = 0; 
        s_key_down_msg_id = 0;
        s_is_full_paint = FALSE;
        StopPlayerAllTimer();        
        VideoPlayerCompulsivePause(win_id);
        PauseProcessTimer();   
        GUIBLOCK_ResetMainType();//NEWMS00183010
        MMIDEFAULT_AllowTurnOffBackLight(TRUE);        
        MMIAPISET_ResetContrast(GUI_MAIN_LCD_ID);
 #ifdef  VIDEOTHUMBNAIL_SUPPORT      
        VPThumbnailRestoreMainLayer();
 #endif
#ifdef UI_MULTILAYER_SUPPORT
        UILAYER_SetMainLayerMemFormat(UILAYER_MEM_DOUBLE_COPY);
#endif
        //MMI_Enable3DMMI(TRUE);
        break;
        
    case MSG_GET_FOCUS: 
#ifdef UI_MULTILAYER_SUPPORT
        UILAYER_SetMainLayerMemFormat(UILAYER_MEM_NONE);
#endif
        s_is_full_paint = FALSE;       
        SetBlockType(VP_HALF_TRANSPARENCE);//NEWMS00183010
        MMIAPISET_UpdateLCDContrast(GUI_MAIN_LCD_ID,VP_Setting_GetBrightness());        
#ifdef  VIDEOTHUMBNAIL_SUPPORT    
        s_is_get_focus=TRUE;
        VPThumbnailSetMainLayer();
#endif

        VideoPlayerCompulsiveResume(win_id);   
#ifdef MMI_STREAMING_PLAYER_SUPPORT
        if (s_is_need_auth)
        {          
            s_is_need_auth = FALSE;
            if (VP_TO_DO_PLAY_FOR_FULL_PAINT != s_video_player_control_info.to_do_type
                && VP_STATE_READY != s_video_player_control_info.state)
            {            
                MMK_CreateWin((uint32 *)MMIVP_AUTH_WIN_TAB,PNULL);
            }
        }       
#endif
#if defined VIDEOTHUMBNAIL_SUPPORT && defined MMI_VIDEO_PLAYER_MINI_LAYOUT //@bug 145711
        GUI_FillRect(MMITHEME_GetDefaultLcdDev(), MMITHEME_GetFullScreenRect(), MMIAPIVP_GetTransparenceColor());
#endif

        break;  
        
    case MSG_KEYUP_DOWN:   
    case MSG_KEYLONG_DOWN: 
    case MSG_KEYUP_RIGHT:  
    case MSG_KEYLONG_RIGHT:       
    case MSG_KEYUP_LEFT: 
    case MSG_KEYLONG_LEFT:
    case MSG_KEYUP_UP:        
    case MSG_KEYLONG_UP:  
    case MSG_KEYPRESSUP_DOWN:
    case MSG_KEYPRESSUP_UP:
    case MSG_KEYPRESSUP_LEFT:
    case MSG_KEYPRESSUP_RIGHT:
    case MSG_KEYDOWN_UP:
    case MSG_KEYDOWN_DOWN:
    case MSG_KEYDOWN_LEFT:
    case MSG_KEYDOWN_RIGHT:
    case MSG_KEYREPEAT_DOWN:
    case MSG_KEYREPEAT_UP:
    case MSG_KEYREPEAT_LEFT:
    case MSG_KEYREPEAT_RIGHT:
    case MSG_APP_UPSIDE:
    case MSG_APP_DOWNSIDE:        
        //VP_STRACE_LOW:"[VP]: HandleVPWinMsg(), key msg_id = %x, s_video_player_control_info.is_display_win =%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_7080_112_2_18_3_5_42_471,(uint8*)"dd", msg_id,s_video_player_control_info.is_display_win);
      
        HandleDirectionKeyMsgForHalfScreen(msg_id);       
        break;

    case MSG_KEYDOWN_3:   
    case MSG_KEYDOWN_2:        
    case MSG_KEYDOWN_1:  
    case MSG_KEYDOWN_5:
    case MSG_KEYDOWN_8:    
    case MSG_KEYDOWN_HASH: 
    case MSG_KEYDOWN_STAR:
#ifdef MMI_VIDEO_PLAYER_MINI_SUPPORT
    case MSG_KEYDOWN_4:
    case MSG_KEYDOWN_6:
    case MSG_KEYDOWN_7:
    case MSG_KEYDOWN_9:
    case MSG_KEYDOWN_0:  //0
#ifdef KEYPAD_TYPE_QWERTY_KEYPAD  ////CR NEWMS00207601 leimingma
    case MSG_KEYDOWN_R:  //1
    case MSG_KEYDOWN_T:  //2
    case MSG_KEYDOWN_Y:  //3 
    case MSG_KEYDOWN_B:  //8        
    case MSG_KEYDOWN_G:  //5
    case MSG_KEYDOWN_F:  //4
    case MSG_KEYDOWN_H:  //6
    case MSG_KEYDOWN_J:  //#
    case MSG_KEYDOWN_U:  //* 
    case MSG_KEYDOWN_N:  //9
    case MSG_KEYDOWN_V:  //7
    case MSG_KEYDOWN_M:
#endif ////CR NEWMS00207601 leimingma
#endif //MMI_VIDEO_PLAYER_MINI_SUPPORT
        HandleShortCutMsg(msg_id);
        break;   
     
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_DOWN: 
    case MSG_TP_PRESS_UP:
    case MSG_TP_PRESS_MOVE:    
    case MSG_CTL_PENOK:
    case MSG_CTL_PENLONGOK:
    case MSG_TP_PRESS_DOUBLE:
        HandleTPMsg(msg_id,param);
        break;            
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527    

        
         
 #ifdef MMI_STREAMING_PLAYER_SUPPORT       
    case MSG_PROMPTWIN_OK:  
        MMIPUB_CloseQuerytWin(PNULL);
        PlayFromNet(s_video_player_control_info.process_time,s_video_player_control_info.total_time); 
        break;
        
    case MSG_PROMPTWIN_CANCEL:   
        MMIPUB_CloseQuerytWin(PNULL);
        PlayFromNet(0,s_video_player_control_info.total_time);
        break;
#endif

    case MSG_VP_BEGIN_PLAY: 
#ifdef MMI_STREAMING_PLAYER_SUPPORT
        s_is_need_auth = FALSE;
#endif
        VideoPlayerPlay(win_id);               
        break;    
    
    case MSG_VP_SEEK:
        VideoPlayerSeek(win_id,s_video_player_control_info.seek_time);   
        break;
#ifdef MMI_STREAMING_PLAYER_SUPPORT
    case MSG_VP_AUTH_IND:
        if (MMK_IsFocusWin(win_id))
        {
            MMK_CreateWin((uint32 *)MMIVP_AUTH_WIN_TAB,PNULL);
        }
        else
        {
            s_is_need_auth = TRUE;
        }
        break;  
#endif

    case MSG_VP_FILE_OPTION:
        OpenFileOptionWin();
        break;

    case MSG_VP_REPEAT_SET:
        OpenRepeatModeWin();
        break;
    case MSG_VP_RANDOM_SET:
        OpenRandomModeWin();
        break;    

    case MSG_VP_OPEN_BRIGHT_CONTROL_PANE:
        OpenControlPanel(FALSE); 
        break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }

    
    return recode;
}

/*****************************************************************************/
//  Description : 进度条seek
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
LOCAL void OpenSeekWin(MMI_WIN_ID_T win_Id ,
                       GUI_POINT_T point,
                       BOOLEAN is_seek_now)
{
    uint32   seek_time = 0;
    uint16 height = 0;    
    uint16 slide_width = 0;
    GUI_RECT_T display_rect = {0};
    uint32  space = 0;
    uint32  process_width = 0;

    //VP_STRACE_LOW:"[VP] OpenSeekWin x = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_7198_112_2_18_3_5_42_472,(uint8*)"d",point.x);
    
    if(IsSeekEnable() || (!MMIAPIVP_IsLiving()
        &&(VP_STATE_READY == s_video_player_control_info.state) && 0 != s_video_player_control_info.full_path_len))
    {        
        GUIRES_GetImgWidthHeight(PNULL,&height,IMAGE_COMMON_PLAYER_PROCESS_GROOVE,MMIVP_PLAYER_WIN_ID);
        GUIOWNDRAW_GetDisplayRect(VP_PLAY_PANEL_PROCESS_OWNDRAW_CTRL_ID,&display_rect);
        
        if(GUI_PointIsInRect(point,display_rect))
        {
			if(point.x <= display_rect.left+VP_OWNERDRAW_PROCESS_LEFTMARGIN)
			{
				seek_time=0;
			}
			else if(display_rect.right -VP_OWNERDRAW_PROCESS_LEFTMARGIN <= point.x )
			{
				seek_time=s_video_player_control_info.total_time;
			}
			else
			{
				space = (uint32)(point.x - display_rect.left+1);   /*lint !e571*/                                 
				process_width = (uint32)(display_rect.right-display_rect.left + 1 - slide_width - 2*VP_OWNERDRAW_PROCESS_LEFTMARGIN); /*lint !e571*/    
				GUIRES_GetImgWidthHeight(&slide_width, NULL,IMAGE_COMMON_PLAYER_PROCESS_SLIDE,MMIVP_PLAYER_WIN_ID);
				seek_time =  (uint32)(space * (s_video_player_control_info.total_time * 1.0 / process_width));
			}
            if (!is_seek_now)   //没有释放的时候，只是进度条在动，释放的时候通知状态机seek
            {
                StopProcessTimer();
                s_video_player_control_info.process_time = seek_time;
                DisplayVPWinProgress(TRUE);
            }
            else
            {
                if((VP_STATE_READY == s_video_player_control_info.state))
                {
                    PlayOrPause();
                }
                else
                {
                    VideoPlayerSeek(win_Id, seek_time);
                    StartProcessTimer();
                }
            }
        }
        else if(s_tp_down_ctrl_id == VP_PLAY_PANEL_PROCESS_OWNDRAW_CTRL_ID)
        {
            //VideoPlayerSeek(win_Id, s_video_player_control_info.process_time);
            StartProcessTimer();
        }
    }    
}

/*****************************************************************************/
//  Description : 打开文件选择窗口
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
LOCAL void OpenFileOptionWin(void)
{
#ifdef TOUCH_PANEL_SUPPORT 
    if(IsOpenOtherWinEnable())    
    {
        MMK_CreateWin((uint32 *)MMIVP_FILE_OPTION_WIN_TAB,PNULL);
    }
#endif
}

/*****************************************************************************/
//  Description : 打开设置循环模式窗口
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
LOCAL void OpenRepeatModeWin(void)
{
    VP_REPEAT_PLAY_MODE_E repeat_mode = 0;      
    MMI_CTRL_ID_T          ctrl_id = 0;
    
    if(VP_PLAY_FROM_FILE == s_video_player_control_info.play_type
        || VP_PLAY_FROM_LOCALLIST == s_video_player_control_info.play_type)
    {    
        ctrl_id = VP_PLAY_PANEL_TITLE_FORM_CTRL_ID;        

        repeat_mode = VP_Setting_GetRepeatPlayMode();
        if (repeat_mode == VP_REPEAT_PLAY_MODE_ONE)
        {
            repeat_mode = VP_REPEAT_PLAY_MODE_CLOSE;
        }
        else
        {
            repeat_mode++;
        }
        //VP_STRACE_LOW:"[VP] OpenRepeatModeWin repeat_mode = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_7269_112_2_18_3_5_42_473,(uint8*)"d",repeat_mode);
        VP_Setting_SetRepeatPlayMode(repeat_mode);
        MMIAPIVP_FSMUpdatePlayMode(s_video_player_control_info.vp_handle,
            repeat_mode,
            VP_Setting_GetIsRandomPlayMode());
        DisplayVPWinRepeatModeIcon(TRUE);  
    }
}

/*****************************************************************************/
//  Description : 打开设置随机播放窗口
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
LOCAL void OpenRandomModeWin(void)
{
    BOOLEAN  is_random = FALSE;   
    MMI_CTRL_ID_T ctrl_id = 0;  
    

    if(VP_PLAY_FROM_FILE == s_video_player_control_info.play_type
        || VP_PLAY_FROM_LOCALLIST == s_video_player_control_info.play_type)
     {
        ctrl_id = VP_PLAY_PANEL_TITLE_FORM_CTRL_ID;
        
     
        is_random = VP_Setting_GetIsRandomPlayMode();
        if (is_random)
        {
            is_random = FALSE;
        }
        else
        {
            is_random = TRUE;
        }
        //VP_STRACE_LOW:"[VP] OpenRandomModeWin is_random = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_7303_112_2_18_3_5_42_474,(uint8*)"d",is_random);
        VP_Setting_SetIsRandomPlayMode(is_random);
        MMIAPIVP_FSMUpdatePlayMode(s_video_player_control_info.vp_handle,
                VP_Setting_GetRepeatPlayMode(),
                is_random);
        DisplayVPWinRandomModeIcon(TRUE);        
     }
        
}

/*****************************************************************************/
//  Description : 打开调节窗口
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
LOCAL void OpenControlPanel(BOOLEAN is_vol_panel)
{  
    MMICOM_PANEL_INFO_T panel_info = {0};

    if (!MMK_IsFocusWin(MMIVP_PLAYER_WIN_ID))
    {
        //VP_STRACE_LOW:"[VP] OpenControlPanel is not focus and not open control panel"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_7323_112_2_18_3_5_42_475,(uint8*)"");
        return;
    }
    
    if (is_vol_panel)
    {   
        panel_info.cur_value = MMIAPISET_GetMultimVolume();
        panel_info.last_value = MMIAPISET_GetLastMultimVolume();
        panel_info.min_value = VP_VOLUME_MIN;   
        panel_info.max_value = VP_VOLUME_MAX;
        panel_info.panel_type = MMICOM_PANEL_VOLUME;
        panel_info.set_callback = AdjustVolCallBack;    
        
    }
    else if (!IsAdjustBrightEnable())
    {
        return;
    }
    else 
    {
        panel_info.cur_value = VP_Setting_GetBrightness();
        panel_info.min_value = 0;
        panel_info.max_value = MMISET_CONTRAST_SEVEN;
        panel_info.panel_type = MMICOM_PANEL_BRIGHTNESS;
        panel_info.set_callback = AdjustBrightCallBack;       
    }
    panel_info.x = -1;
    panel_info.y = -1;
    MMIAPICOM_OpenPanelChildWin(MMIVP_PLAYER_WIN_ID,&panel_info);
     
}

/*****************************************************************************/
//  Description : 调节音量窗口
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIVP_TurnVol(MMICOM_PANEL_OPEN_TYPE_E turn_vol_type)
{
    MMICOM_PANEL_INFO_T panel_info = {0};
    
    panel_info.cur_value = MMIAPISET_GetMultimVolume();
    panel_info.last_value = MMIAPISET_GetLastMultimVolume();
    panel_info.min_value = VP_VOLUME_MIN;   
    panel_info.max_value = VP_VOLUME_MAX;
    panel_info.panel_type = MMICOM_PANEL_VOLUME;
    panel_info.open_type = turn_vol_type;
    panel_info.set_callback = AdjustVolCallBack; 
    panel_info.x = -1;
    panel_info.y = -1;
    MMIAPICOM_OpenPanelChildWin(MMK_GetFocusWinHandle(),&panel_info); 
}

/*****************************************************************************/
//  Description : 播放或者暂停
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
LOCAL void PlayOrPause(void)
{
    MMI_WIN_ID_T  win_id = MMIVP_PLAYER_WIN_ID;
    uint16 full_path_len = VP_MAX_NAME_LEN;
    wchar full_path_name [VP_MAX_NAME_LEN +1] = {0};
    

    //VP_STRACE_LOW:"[VP] PlayOrPause s_video_player_control_info.state = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_7386_112_2_18_3_5_43_476,(uint8*)"d",s_video_player_control_info.state);
   
    if (VP_STATE_READY == s_video_player_control_info.state
        && 0 != s_video_player_control_info.full_path_len)   //ready状态且当前有文件可以播放
    {
            
        SCI_MEMSET(full_path_name,0,((VP_MAX_NAME_LEN +1)*sizeof(wchar)));
        full_path_len = MIN(VP_MAX_NAME_LEN,s_video_player_control_info.full_path_len);
        MMIAPICOM_Wstrncpy(full_path_name,s_video_player_control_info.full_path_name,full_path_len);          
#ifdef MMI_STREAMING_PLAYER_SUPPORT
        if (VP_PLAY_FROM_NET == s_video_player_control_info.play_type)
        {  
            //如果出现获取数据timeout或者连接timeout的情况，获取最后一次播放的信息
            if (0 == s_video_player_control_info.process_time) //add for newms111640 
            {
            	LoadLastPlayingFile();            
            }              
            if (0 != s_video_player_control_info.process_time
                && VP_MAX_INVALID_VALUE != s_video_player_control_info.total_time) //点播上次断点非起点，需要询问是否继续播放
            {            
                MMIPUB_OpenQueryWinByTextId(TXT_VP_IS_CONTINUE,IMAGE_PUBWIN_QUERY,PNULL,PNULL);
            }
            else
            {
                PlayFromNet(s_video_player_control_info.process_time,
                    s_video_player_control_info.total_time);                   
            }
        }
        else
#endif
        {
            //正常结束时按play,此时process_time = total time,应该从开始播放
            if (s_video_player_control_info.process_time == s_video_player_control_info.total_time)
            {
               s_video_player_control_info.process_time = 0;
            }

            NotifyPlayerForPlayFile(full_path_name,
                full_path_len,
                s_video_player_control_info.process_time,
                s_video_player_control_info.total_time,
                s_video_player_control_info.play_type);
        }
        
    }
    else if (VP_STATE_PAUSED == s_video_player_control_info.state
        && !MMIAPIVP_IsLiving()) //非直播暂停状态
    {
        VideoPlayerResume(win_id);  
    }
    else if ((VP_STATE_PLAYING == s_video_player_control_info.state 
        || VP_STATE_BUFFERING == s_video_player_control_info.state)
        && !MMIAPIVP_IsLiving())
    {        
        VideoPlayerPause(win_id);     
    }   
}

/*****************************************************************************/
//  Description : 切换屏幕
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
LOCAL void SwitchScreen(BOOLEAN is_full)
{
    VP_DISPLAY_PARAM_T display_param = {0};    
   // GUI_RECT_T lcd_rect = {0};    
   // IGUICTRL_T* ctrl_handle = PNULL;
    MMI_WIN_ID_T win_id = MMIVP_PLAYER_WIN_ID;
    BOOLEAN is_scal_up = FALSE;
    GUI_RECT_T display_rect = {0};   
    //GUI_POINT_T dis_point = {0};
   
    
    if (!MMK_IsOpenWin(win_id))
    {
        return;
    }
    display_param.RotateAngle = GUILCD_GetLogicAngle(GUI_MAIN_LCD_ID);      

    //VP_STRACE_LOW:"[VP] SwitchScreen is_full = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_7468_112_2_18_3_5_43_477,(uint8*)"d",is_full);
    
    

    if (is_full)
    {             
        display_rect = MMITHEME_GetFullScreenRect();  
        is_scal_up = TRUE;
    }
    else
    {  
          
              
        GUIOWNDRAW_GetDisplayRect(VP_PLAY_PANEL_VIDEO_OWNDRAW_CTRL_ID,&display_rect);        
        StartVPWinDisplayerTimer();
        is_scal_up = FALSE;        
    }       

    display_param.disp_rect.dy = display_rect.bottom - display_rect.top + 1;
    display_param.disp_rect.dx = display_rect.right - display_rect.left + 1;            
    display_param.disp_rect.x = display_rect.left;
    display_param.disp_rect.y = display_rect.top;  

    display_param.target_rect.x = display_param.disp_rect.x;
    display_param.target_rect.y = display_param.disp_rect.y;
    display_param.target_rect.dx = display_param.disp_rect.dx;
    display_param.target_rect.dy = display_param.disp_rect.dy;
        
    
    MMIAPIVP_FSMSwitchDisplayMode(s_video_player_control_info.vp_handle,&display_param,TRUE);        
    
}

/*****************************************************************************/
//  Description : 添加文件开始播放
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL void AddFileToPlayerCallBack(
                                      BOOLEAN     is_success,
                                      FILEARRAY   file_array
                                      )
{
    uint32              file_num = 0;
    FILEARRAY_DATA_T    file_data = {0};

    //VP_STRACE_LOW:"[VP] AddFileToPlayerCallBack is_success = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_7513_112_2_18_3_5_43_478,(uint8*)"d",is_success);
    if (is_success)
    {
       file_num = MMIAPIFILEARRAY_GetArraySize(file_array);
       if (file_num > 0 && MMIAPIFILEARRAY_Read(file_array,0,&file_data))
       {  
           
           NotifyPlayerForPlayFile(file_data.filename,file_data.name_len,0,0,VP_PLAY_FROM_FILE);
           
       }
    }
    if (PNULL != file_array)
    {
        MMIAPIFILEARRAY_Destroy(&file_array);
        file_array = PNULL;
    }
}

/*****************************************************************************/
//  Description : OpenLocalFileWin
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN OpenLocalFileWin(MMI_WIN_ID_T       win_id)
{
    MMIFMM_FILTER_T             filter = {0};
    FILEARRAY                   ret_array = PNULL;
    wchar                   abs_default_path[VP_MAX_NAME_LEN+1]={0};
    uint16                  abs_default_path_length = VP_MAX_NAME_LEN;

	if(MMI_DEVICE_NUM <= MMIAPIFMM_GetValidateDevice(MMI_DEVICE_UDISK))
    {
        MMIPUB_OpenAlertWinByTextId(PNULL,TXT_NO_SD_CARD_ALERT,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_NONE,PNULL);
		MMK_CloseWin(win_id);
		return FALSE;
	}

    MMIAPIVP_GetFilerInfo(&filter);
    ret_array = MMIAPIFILEARRAY_Create();    
    if(!MMIAPIVP_GetDefaultFolderPath(abs_default_path,&abs_default_path_length)
		|| FMM_OPENWIN_SUCCESS != MMIAPIFMM_OpenFileWinByLayer(
                                                abs_default_path,
                                                abs_default_path_length,
                                                &filter,
                                                FUNC_FIND_FILE_AND_ALLFOLER,
                                                FALSE,
                                                PNULL,
                                                ret_array,
                                                AddFileToPlayerCallBack,
                                                FALSE
                                                ))
    {
        MMIAPIFILEARRAY_Destroy(&ret_array);
        MMK_CloseWin(win_id);
        MMIPUB_OpenAlertFailWin(TXT_ERROR);
        return FALSE;
    }  
    else
    {
        MMIAPIFMM_SetOpenFileWinSoftkey(TXT_COMMON_PLAY, TXT_NULL, STXT_RETURN);
        MMK_CloseWin(win_id);
        return TRUE;
    }
}
#ifdef MMI_STREAMING_PLAYER_SUPPORT
/*****************************************************************************/
//  Description : SaveBookMark
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN SaveBookMark(void)
{
    VP_NET_MOVIE_INFO_T         bookmark_data = {0};
    VP_LIST_ERROR_E             add_result = 0;
    MMI_TEXT_ID_T               text_id   = TXT_NULL;
    
    if (VP_PLAY_FROM_NET == s_video_player_control_info.play_type)
    {       
        SCI_MEMSET(bookmark_data.url_ptr,0,(VP_MAX_URL_LEN+1));
        SCI_MEMCPY((char *)(bookmark_data.url_ptr),(char *)(s_video_player_control_info.url),(MIN(VP_MAX_URL_LEN,(strlen((char *)(s_video_player_control_info.url))))));/*lint !e666*/
        SCI_MEMSET(bookmark_data.title_arr,0,((VP_MAX_NAME_LEN+1)*sizeof(wchar)));
        bookmark_data.title_len = MIN(VP_MAX_NAME_LEN,s_video_player_control_info.full_path_len);
        MMIAPICOM_Wstrncpy(bookmark_data.title_arr,s_video_player_control_info.full_path_name,bookmark_data.title_len);        
                    
        add_result = VP_BookmarkList_AddItem(bookmark_data);      
        //VP_STRACE_LOW:"[VP] SaveBookMark add_result = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_7596_112_2_18_3_5_43_479,(uint8*)"d",add_result);
       
        if (VP_LIST_ERROR_TOO_MUCH == add_result)
        {
            text_id = TXT_VP_TOO_MUCH;
        }
        else if (VP_LIST_ERROR_NONE == add_result)
        {
            text_id = TXT_VP_SAVE_BOOKMARK_SUCC;
        }
        //@CR239633 2011.05.18
        else if(VP_LIST_ERROR_NO_SPACE == add_result)
        {
            text_id = TXT_NO_SPACE;
        }
        else
        {
            if(MMI_DEVICE_NUM <= MMIAPIFMM_GetFirstValidDevice())
            {
                text_id = TXT_NO_SD_CARD_ALERT;
            }
            else
            {
                text_id = TXT_ERROR;
            } 
        }
        //@CR239633 2011.05.18
        
        DisplayVPWinStation(text_id,TRUE,FALSE);  
        StartVPDisplayStationTimer();
        return TRUE;
    }
    return FALSE;
}
#endif
/*****************************************************************************/
//  Description : 显示选项窗口
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
LOCAL void DisplayOptMenWin(MMI_WIN_ID_T       win_id, MMI_CTRL_ID_T    ctrl_id)
{  
   // MMI_MENU_GROUP_ID_T         group_id = 0;  
    MMI_STRING_T            kstring = {0};
    GUIMENU_DYNA_ITEM_T     node_item = {0};
    uint32                  index = 0; 
    
#ifndef MMI_PDA_SUPPORT
#ifndef MMI_VIDEO_PLAYER_MINI_SUPPORT  //6530_MINI
    GUIMENU_POP_SEL_INFO_T      pop_item_info = {0};  /*pclint-529*/
    uint32                      j = 0;                /*pclint-529*/
#endif //end of MMI_VIDEO_PLAYER_MINI_SUPPORT

#ifdef MMI_VIDEO_PLAYER_MINI_LAYOUT//6530 MINI 界面布局
    
#ifdef MMI_VIDEO_PLAYER_MINI_SUPPORT  //6530_MINI 32X16 option

    MMI_GetLabelTextByLang(TXT_VP_LOCALLIST, &kstring);
    node_item.item_text_ptr = &kstring;
    if (VP_ENTRY_FROM_PLAYER != s_video_player_control_info.vp_entry)
    {
         node_item.is_grayed = TRUE;
    }   
    else
    {
        node_item.is_grayed = FALSE;
    }
    GUIMENU_InsertNode(index, ID_VP_OPEN_LOCALLIST, 0, &node_item, ctrl_id);
#else
    MMI_GetLabelTextByLang(TXT_COMM_OPEN, &kstring);
    node_item.item_text_ptr = &kstring;
    node_item.is_grayed = FALSE;
    GUIMENU_InsertNode(index, ID_VP_OPEN, 0, &node_item, ctrl_id);
    
    MMI_GetLabelTextByLang(TXT_VP_LOCALFILE_VIEW, &kstring);
    node_item.item_text_ptr = &kstring;
    node_item.is_grayed = FALSE;
    GUIMENU_InsertNode(j, ID_VP_OPEN_LOCALFILE, ID_VP_OPEN, &node_item, ctrl_id);
    j++; 
#ifdef MMI_STREAMING_PLAYER_SUPPORT
    MMI_GetLabelTextByLang(TXT_VP_NET_VIEW, &kstring);
    node_item.item_text_ptr = &kstring;
    node_item.is_grayed = FALSE;
    GUIMENU_InsertNode(j, ID_VP_OPEN_NET, ID_VP_OPEN, &node_item, ctrl_id);
    j++; 
#endif    
    
    
    MMI_GetLabelTextByLang(TXT_VP_LOCALLIST, &kstring);
    node_item.item_text_ptr = &kstring;
    if (VP_ENTRY_FROM_PLAYER != s_video_player_control_info.vp_entry)
    {
        node_item.is_grayed = TRUE;
    }   
    else
    {
        node_item.is_grayed = FALSE;
    }
    GUIMENU_InsertNode(j, ID_VP_OPEN_LOCALLIST, ID_VP_OPEN, &node_item, ctrl_id);
 
#endif //MMI_VIDEO_PLAYER_MINI_SUPPORT  //6530_MINI 32X16 option
    index++;
#ifdef MMI_VIDEO_PLAYER_MINI_SUPPORT //6530_MINI
    MMI_GetLabelTextByLang(TXT_SET_SHUFFLE, &kstring);
    node_item.item_text_ptr = &kstring;
    if (VP_ENTRY_FROM_PLAYER != s_video_player_control_info.vp_entry)
    {
        node_item.is_grayed = TRUE;
    }   
    else
    {
        node_item.is_grayed = FALSE;
    }
    GUIMENU_InsertNode(index, ID_VP_SET_RANDOM_MODE, 0, &node_item, ctrl_id);
    GUIMENU_SetSubMenuStyle(ctrl_id,ID_VP_SET_RANDOM_MODE,GUIMENU_STYLE_POPUP_RADIO);
    index++;
    
    MMI_GetLabelTextByLang(TXT_COMMON_REPEAT, &kstring);
    node_item.item_text_ptr = &kstring;
    if (VP_ENTRY_FROM_PLAYER != s_video_player_control_info.vp_entry)
    {
        node_item.is_grayed = TRUE;
    }   
    else
    {
        node_item.is_grayed = FALSE;
    }
    GUIMENU_InsertNode(index, ID_VP_SET_REPEATE_MODE, 0, &node_item, ctrl_id);
    index++; 
#else
    MMI_GetLabelTextByLang(TXT_COMMON_REPEAT, &kstring);
    node_item.item_text_ptr = &kstring;
    if (VP_ENTRY_FROM_PLAYER != s_video_player_control_info.vp_entry)
    {
        node_item.is_grayed = TRUE;
    }   
    else
    {
        node_item.is_grayed = FALSE;
    }
    GUIMENU_InsertNode(index, ID_VP_SET_REPEATE_MODE, 0, &node_item, ctrl_id);
    GUIMENU_SetSubMenuStyle(ctrl_id,ID_VP_SET_REPEATE_MODE,GUIMENU_STYLE_POPUP_RADIO);
    
    MMI_GetLabelTextByLang(TXT_CLOSE, &kstring);
    node_item.item_text_ptr = &kstring;
    node_item.is_grayed = FALSE;    
    GUIMENU_InsertNode(j, ID_VP_REPEAT_CLOSE, ID_VP_SET_REPEATE_MODE, &node_item, ctrl_id);
    j++;
    
    MMI_GetLabelTextByLang(TXT_COMMON_SET_REPEAT_ALL, &kstring);
    node_item.item_text_ptr = &kstring;
    node_item.is_grayed = FALSE;
    GUIMENU_InsertNode(j, ID_VP_REPEAT_ALL, ID_VP_SET_REPEATE_MODE, &node_item, ctrl_id);
    j++;
    
    MMI_GetLabelTextByLang(TXT_VP_SINGLE, &kstring);
    node_item.item_text_ptr = &kstring;
    node_item.is_grayed = FALSE;    
    GUIMENU_InsertNode(j, ID_VP_REPEAT_ONE, ID_VP_SET_REPEATE_MODE, &node_item, ctrl_id);
    j++;
    
    pop_item_info.is_static = FALSE;
    pop_item_info.ctrl_id = ctrl_id;
    switch(VP_Setting_GetRepeatPlayMode()) 
    {
    case VP_REPEAT_PLAY_MODE_ALL:
        pop_item_info.node_id = ID_VP_REPEAT_ALL;
        break;
    case VP_REPEAT_PLAY_MODE_ONE:
        pop_item_info.node_id = ID_VP_REPEAT_ONE;
        break;
    default:
        pop_item_info.node_id = ID_VP_REPEAT_CLOSE;
        break;
    } 
    GUIMENU_SetPopItemStatus(TRUE,&pop_item_info);
    index++;   
    j = 0;    
    
    MMI_GetLabelTextByLang(TXT_SET_SHUFFLE, &kstring);
    node_item.item_text_ptr = &kstring;
    if (VP_ENTRY_FROM_PLAYER != s_video_player_control_info.vp_entry)
    {
        node_item.is_grayed = TRUE;
    }   
    else
    {
        node_item.is_grayed = FALSE;
    }
    GUIMENU_InsertNode(index, ID_VP_SET_RANDOM_MODE, 0, &node_item, ctrl_id);
    GUIMENU_SetSubMenuStyle(ctrl_id,ID_VP_SET_RANDOM_MODE,GUIMENU_STYLE_POPUP_RADIO);
    
    MMI_GetLabelTextByLang(TXT_CLOSE, &kstring);
    node_item.item_text_ptr = &kstring;
    node_item.is_grayed = FALSE;
    GUIMENU_InsertNode(j, ID_VP_RANDOM_CLOSE, ID_VP_SET_RANDOM_MODE, &node_item, ctrl_id);
    j++;
    
    MMI_GetLabelTextByLang(TXT_OPEN, &kstring);
    node_item.item_text_ptr = &kstring;
    node_item.is_grayed = FALSE;
    GUIMENU_InsertNode(j, ID_VP_RANDOM_OPEN, ID_VP_SET_RANDOM_MODE, &node_item, ctrl_id);
    j++;   
    
    
    pop_item_info.is_static = FALSE;
    pop_item_info.ctrl_id = ctrl_id;   
    if (VP_Setting_GetIsRandomPlayMode())
    {
        pop_item_info.node_id = ID_VP_RANDOM_OPEN;
    }
    else
    {
        pop_item_info.node_id = ID_VP_RANDOM_CLOSE;
    }     
    GUIMENU_SetPopItemStatus(TRUE,&pop_item_info);
    index++;  
    j = 0;

#ifdef MMI_STREAMING_PLAYER_SUPPORT
    MMI_GetLabelTextByLang(TXT_VIEW, &kstring);
    node_item.item_text_ptr = &kstring;
    node_item.is_grayed = FALSE;
    GUIMENU_InsertNode(index, ID_VP_VIEW, 0, &node_item, ctrl_id);
#ifdef MMI_VIDEOPLAYER_HISTORYLIST_SUPPORT
    MMI_GetLabelTextByLang(TXT_VP_HISTORYLIST_ID, &kstring);
    node_item.item_text_ptr = &kstring;
    node_item.is_grayed = FALSE;
    GUIMENU_InsertNode(j, ID_VP_VIEW_HISTORYLIST, ID_VP_VIEW, &node_item, ctrl_id);
    j++; 
#endif

    MMI_GetLabelTextByLang(TXT_VP_BOOKMARK_ID, &kstring);

    node_item.item_text_ptr = &kstring;
    node_item.is_grayed = FALSE;
    GUIMENU_InsertNode(j, ID_VP_VIEW_BOOKMARK, ID_VP_VIEW, &node_item, ctrl_id);
    j++; 
    
    index++;    
    j = 0;
#endif

#endif  //MMI_VIDEO_PLAYER_MINI_SUPPORT //6530_MINI

    MMI_GetLabelTextByLang(STXT_MAIN_SETTING, &kstring);
    node_item.item_text_ptr = &kstring;
    node_item.is_grayed = FALSE;
    GUIMENU_InsertNode(index, ID_VP_SETTING, 0, &node_item, ctrl_id);  
    index++;  
#ifndef MMI_VIDEO_PLAYER_MINI_SUPPORT //6530_MINI
    if (VP_PLAY_FROM_FILE == s_video_player_control_info.play_type
        || VP_PLAY_FROM_LOCALLIST == s_video_player_control_info.play_type)
    {
        MMI_GetLabelTextByLang(TXT_COMMON_SETTING_AS, &kstring);
        node_item.item_text_ptr = &kstring;
        node_item.is_grayed = FALSE;
        GUIMENU_InsertNode(index, ID_VP_SETTING_AS, 0, &node_item, ctrl_id);
        
#ifdef SCREENSAVER_SUPPORT
        MMI_GetLabelTextByLang(TXT_COMMON_SCREENSAVE, &kstring);
        node_item.item_text_ptr = &kstring;
        node_item.is_grayed = ((0==s_video_player_control_info.full_path_len)? TRUE:FALSE);
        GUIMENU_InsertNode(j, ID_VP_SCEENSAVE, ID_VP_SETTING_AS, &node_item, ctrl_id);
        j++;
#endif
        MMI_GetLabelTextByLang(TXT_COMMON_POWER_ON_ANIMATION, &kstring);
        node_item.item_text_ptr = &kstring;
        node_item.is_grayed = ((0==s_video_player_control_info.full_path_len)? TRUE:FALSE);
        GUIMENU_InsertNode(j, ID_VP_POWER_ON_SET, ID_VP_SETTING_AS, &node_item, ctrl_id);
        j++;
        
        MMI_GetLabelTextByLang(TXT_COMMON_POWER_OFF_ANIMATION, &kstring);
        node_item.item_text_ptr = &kstring;
        node_item.is_grayed = ((0==s_video_player_control_info.full_path_len)? TRUE:FALSE);
        GUIMENU_InsertNode(j, ID_VP_POWER_OFF_SET, ID_VP_SETTING_AS, &node_item, ctrl_id);
        j++;
        
#ifdef MMI_VIDEOWALLPAPER_SUPPORT
        MMI_GetLabelTextByLang(TXT_VIDEO_WALLPAPER, &kstring);
        node_item.item_text_ptr = &kstring;
        node_item.is_grayed = ((0==s_video_player_control_info.full_path_len)? TRUE:FALSE);
        GUIMENU_InsertNode(j, ID_VP_VIDEO_WALLPAPER, ID_VP_SETTING_AS, &node_item, ctrl_id);
#endif        
        index++;     	
        j = 0;
    }
    else if(VP_PLAY_FROM_NET == s_video_player_control_info.play_type)
    {
        MMI_GetLabelTextByLang(TXT_VP_SAVE_BOOKMARK_ID, &kstring);
        node_item.item_text_ptr = &kstring;
        node_item.is_grayed = FALSE;
        GUIMENU_InsertNode(index, ID_VP_SAVE_BOOKMAR, 0, &node_item, ctrl_id);
        index++;       
    }

#endif  //MMI_VIDEO_PLAYER_MINI_SUPPORT //6530_MINI
    if (VP_PLAY_FROM_FILE == s_video_player_control_info.play_type
        || VP_PLAY_FROM_LOCALLIST == s_video_player_control_info.play_type
        || VP_PLAY_FROM_NET == s_video_player_control_info.play_type)
                
    {
        MMI_GetLabelTextByLang(STXT_DETAIL, &kstring);
        node_item.item_text_ptr = &kstring;
        node_item.is_grayed = ((0==s_video_player_control_info.full_path_len)? TRUE:FALSE);
        GUIMENU_InsertNode(index, ID_VP_FILEATTR, 0, &node_item, ctrl_id);
        index++;        
    }

    MMI_GetLabelTextByLang(TXT_VP_EXIT, &kstring);
    node_item.item_text_ptr = &kstring;
    node_item.is_grayed = FALSE;
    GUIMENU_InsertNode(index, ID_VP_EXIT, 0, &node_item, ctrl_id);


#else  //6530 非PDA

    MMI_GetLabelTextByLang(TXT_COMM_OPEN, &kstring);
    node_item.item_text_ptr = &kstring;
    node_item.is_grayed = FALSE;
    GUIMENU_InsertNode(index, ID_VP_OPEN, 0, &node_item, ctrl_id);

    MMI_GetLabelTextByLang(TXT_VP_LOCALFILE_VIEW, &kstring);
    node_item.item_text_ptr = &kstring;
    node_item.is_grayed = FALSE;
    GUIMENU_InsertNode(j, ID_VP_OPEN_LOCALFILE, ID_VP_OPEN, &node_item, ctrl_id);
    j++; 

#ifdef MMI_STREAMING_PLAYER_SUPPORT
    MMI_GetLabelTextByLang(TXT_VP_NET_VIEW, &kstring);
    node_item.item_text_ptr = &kstring;
    node_item.is_grayed = FALSE;
    GUIMENU_InsertNode(j, ID_VP_OPEN_NET, ID_VP_OPEN, &node_item, ctrl_id);
    j++; 
#endif

    MMI_GetLabelTextByLang(TXT_VP_LOCALLIST, &kstring);
    node_item.item_text_ptr = &kstring;
    if (VP_ENTRY_FROM_PLAYER != s_video_player_control_info.vp_entry)
    {
         node_item.is_grayed = TRUE;
    }   
    else
    {
        node_item.is_grayed = FALSE;
    }
    GUIMENU_InsertNode(j, ID_VP_OPEN_LOCALLIST, ID_VP_OPEN, &node_item, ctrl_id);
    j++;    

    index++; 
    j = 0;

    if(IsDisplayFullScreenReferInfoEnable())
    {
        if (LCD_ANGLE_90 == s_video_player_control_info.cur_angle
            || LCD_ANGLE_270 == s_video_player_control_info.cur_angle)
        {
            MMI_GetLabelTextByLang(TXT_VP_HALFSCREEN_ID, &kstring);
        }
        else
        {
            MMI_GetLabelTextByLang(TXT_VP_FULLSCREEN_ID, &kstring);
        }
        node_item.item_text_ptr = &kstring;
        node_item.is_grayed =FALSE;// !IsSwitchFullScreenEnable()
        GUIMENU_InsertNode(index, ID_VP_FULLSCREEN, 0, &node_item, ctrl_id);
        index++;   
    } 

    
    MMI_GetLabelTextByLang(TXT_VP_BRIGHTNESS_ID, &kstring);
    node_item.item_text_ptr = &kstring;
    node_item.is_grayed = !IsAdjustBrightEnable();
    GUIMENU_InsertNode(index, ID_VP_BRIGHTNESS, 0, &node_item, ctrl_id);
    index++;     
   

#ifdef MMI_STREAMING_PLAYER_SUPPORT
    MMI_GetLabelTextByLang(TXT_VIEW, &kstring);
    node_item.item_text_ptr = &kstring;
    node_item.is_grayed = FALSE;
    GUIMENU_InsertNode(index, ID_VP_VIEW, 0, &node_item, ctrl_id);
#ifdef MMI_VIDEOPLAYER_HISTORYLIST_SUPPORT
    MMI_GetLabelTextByLang(TXT_VP_HISTORYLIST_ID, &kstring);
    node_item.item_text_ptr = &kstring;
    node_item.is_grayed = FALSE;
    GUIMENU_InsertNode(j, ID_VP_VIEW_HISTORYLIST, ID_VP_VIEW, &node_item, ctrl_id);
    j++; 
#endif
#ifdef CMCC_UI_STYLE
    MMI_GetLabelTextByLang(TXT_COMMON_BOOKMARKS, &kstring);
#else
    MMI_GetLabelTextByLang(TXT_VP_BOOKMARK_ID, &kstring);
#endif
    node_item.item_text_ptr = &kstring;
    node_item.is_grayed = FALSE;
    GUIMENU_InsertNode(j, ID_VP_VIEW_BOOKMARK, ID_VP_VIEW, &node_item, ctrl_id);
    j++; 

    index++;    
    j = 0;
#endif
    
    MMI_GetLabelTextByLang(TXT_COMMON_REPEAT, &kstring);
    node_item.item_text_ptr = &kstring;
    if (VP_ENTRY_FROM_PLAYER != s_video_player_control_info.vp_entry)
    {
        node_item.is_grayed = TRUE;
    }   
    else
    {
        node_item.is_grayed = FALSE;
    }
    GUIMENU_InsertNode(index, ID_VP_SET_REPEATE_MODE, 0, &node_item, ctrl_id);
    GUIMENU_SetSubMenuStyle(ctrl_id,ID_VP_SET_REPEATE_MODE,GUIMENU_STYLE_POPUP_RADIO);

    MMI_GetLabelTextByLang(TXT_CLOSE, &kstring);
    node_item.item_text_ptr = &kstring;
    node_item.is_grayed = FALSE;    
    GUIMENU_InsertNode(j, ID_VP_REPEAT_CLOSE, ID_VP_SET_REPEATE_MODE, &node_item, ctrl_id);
    j++;

    MMI_GetLabelTextByLang(TXT_COMMON_SET_REPEAT_ALL, &kstring);
    node_item.item_text_ptr = &kstring;
    node_item.is_grayed = FALSE;
    GUIMENU_InsertNode(j, ID_VP_REPEAT_ALL, ID_VP_SET_REPEATE_MODE, &node_item, ctrl_id);
    j++;

    MMI_GetLabelTextByLang(TXT_VP_SINGLE, &kstring);
    node_item.item_text_ptr = &kstring;
    node_item.is_grayed = FALSE;    
    GUIMENU_InsertNode(j, ID_VP_REPEAT_ONE, ID_VP_SET_REPEATE_MODE, &node_item, ctrl_id);
    j++;
    
    pop_item_info.is_static = FALSE;
    pop_item_info.ctrl_id = ctrl_id;
    switch(VP_Setting_GetRepeatPlayMode()) 
    {
    case VP_REPEAT_PLAY_MODE_ALL:
        pop_item_info.node_id = ID_VP_REPEAT_ALL;
        break;
    case VP_REPEAT_PLAY_MODE_ONE:
        pop_item_info.node_id = ID_VP_REPEAT_ONE;
        break;
    default:
        pop_item_info.node_id = ID_VP_REPEAT_CLOSE;
        break;
    } 
    GUIMENU_SetPopItemStatus(TRUE,&pop_item_info);
    index++;   
    j = 0;

    MMI_GetLabelTextByLang(TXT_SET_SHUFFLE, &kstring);
    node_item.item_text_ptr = &kstring;
    if (VP_ENTRY_FROM_PLAYER != s_video_player_control_info.vp_entry)
    {
        node_item.is_grayed = TRUE;
    }   
    else
    {
        node_item.is_grayed = FALSE;
    }
    GUIMENU_InsertNode(index, ID_VP_SET_RANDOM_MODE, 0, &node_item, ctrl_id);
    GUIMENU_SetSubMenuStyle(ctrl_id,ID_VP_SET_RANDOM_MODE,GUIMENU_STYLE_POPUP_RADIO);

    MMI_GetLabelTextByLang(TXT_CLOSE, &kstring);
    node_item.item_text_ptr = &kstring;
    node_item.is_grayed = FALSE;
    GUIMENU_InsertNode(j, ID_VP_RANDOM_CLOSE, ID_VP_SET_RANDOM_MODE, &node_item, ctrl_id);
    j++;

    MMI_GetLabelTextByLang(TXT_OPEN, &kstring);
    node_item.item_text_ptr = &kstring;
    node_item.is_grayed = FALSE;
    GUIMENU_InsertNode(j, ID_VP_RANDOM_OPEN, ID_VP_SET_RANDOM_MODE, &node_item, ctrl_id);
    j++;
    
    

    pop_item_info.is_static = FALSE;
    pop_item_info.ctrl_id = ctrl_id;   
    if (VP_Setting_GetIsRandomPlayMode())
    {
        pop_item_info.node_id = ID_VP_RANDOM_OPEN;
    }
    else
    {
        pop_item_info.node_id = ID_VP_RANDOM_CLOSE;
    }     
    GUIMENU_SetPopItemStatus(TRUE,&pop_item_info);
    index++;  
    j = 0;
    

    if (VP_PLAY_FROM_FILE == s_video_player_control_info.play_type
        || VP_PLAY_FROM_LOCALLIST == s_video_player_control_info.play_type)
    {
        MMI_GetLabelTextByLang(TXT_COMMON_SETTING_AS, &kstring);
        node_item.item_text_ptr = &kstring;
        node_item.is_grayed = FALSE;
        GUIMENU_InsertNode(index, ID_VP_SETTING_AS, 0, &node_item, ctrl_id);
        
#ifdef SCREENSAVER_SUPPORT
        MMI_GetLabelTextByLang(TXT_COMMON_SCREENSAVE, &kstring);
        node_item.item_text_ptr = &kstring;
        node_item.is_grayed = ((0==s_video_player_control_info.full_path_len)? TRUE:FALSE);
        GUIMENU_InsertNode(j, ID_VP_SCEENSAVE, ID_VP_SETTING_AS, &node_item, ctrl_id);
        j++;
#endif
        MMI_GetLabelTextByLang(TXT_COMMON_POWER_ON_ANIMATION, &kstring);
        node_item.item_text_ptr = &kstring;
        node_item.is_grayed = ((0==s_video_player_control_info.full_path_len)? TRUE:FALSE);
        GUIMENU_InsertNode(j, ID_VP_POWER_ON_SET, ID_VP_SETTING_AS, &node_item, ctrl_id);
        j++;

        MMI_GetLabelTextByLang(TXT_COMMON_POWER_OFF_ANIMATION, &kstring);
        node_item.item_text_ptr = &kstring;
        node_item.is_grayed = ((0==s_video_player_control_info.full_path_len)? TRUE:FALSE);
        GUIMENU_InsertNode(j, ID_VP_POWER_OFF_SET, ID_VP_SETTING_AS, &node_item, ctrl_id);
        j++;

#ifdef MMI_VIDEOWALLPAPER_SUPPORT
        MMI_GetLabelTextByLang(TXT_VIDEO_WALLPAPER, &kstring);
        node_item.item_text_ptr = &kstring;
        node_item.is_grayed = ((0==s_video_player_control_info.full_path_len)? TRUE:FALSE);
        GUIMENU_InsertNode(j, ID_VP_VIDEO_WALLPAPER, ID_VP_SETTING_AS, &node_item, ctrl_id);
        j++;
#endif

        index++;     	
        j = 0;
    }
    else if(VP_PLAY_FROM_NET == s_video_player_control_info.play_type)
    {
        MMI_GetLabelTextByLang(TXT_VP_SAVE_BOOKMARK_ID, &kstring);
        node_item.item_text_ptr = &kstring;
        node_item.is_grayed = FALSE;
        GUIMENU_InsertNode(index, ID_VP_SAVE_BOOKMAR, 0, &node_item, ctrl_id);
        index++;       
    }

#ifdef MMI_STREAMING_PLAYER_SUPPORT
    MMI_GetLabelTextByLang(STXT_MAIN_SETTING, &kstring);
    node_item.item_text_ptr = &kstring;
    node_item.is_grayed = FALSE;
    GUIMENU_InsertNode(index, ID_VP_SETTING, 0, &node_item, ctrl_id);
    index++;  
#else
    MMI_GetLabelTextByLang(TXT_VP_SET_PLAY_METHOD, &kstring);
    node_item.item_text_ptr = &kstring;
    node_item.is_grayed = FALSE;
    GUIMENU_InsertNode(index, ID_VP_SET_PLAYBACK_MODE, 0, &node_item, ctrl_id);
    GUIMENU_SetSubMenuStyle(ctrl_id,ID_VP_SET_PLAYBACK_MODE,GUIMENU_STYLE_POPUP_RADIO);

    MMI_GetLabelTextByLang(TXT_VP_AUDIO_VIDEO_SYN, &kstring);
    node_item.item_text_ptr = &kstring;
    node_item.is_grayed = FALSE;
    GUIMENU_InsertNode(j, ID_VP_AUDIO_VIDEO_SYN, ID_VP_SET_PLAYBACK_MODE, &node_item, ctrl_id);
    j++;

    MMI_GetLabelTextByLang(TXT_VP_SCREEN_FLOW, &kstring);
    node_item.item_text_ptr = &kstring;
    node_item.is_grayed = FALSE;
    GUIMENU_InsertNode(j, ID_VP_SCREEN_FLOW, ID_VP_SET_PLAYBACK_MODE, &node_item, ctrl_id);
    j++;   
    

    pop_item_info.is_static = FALSE;
    pop_item_info.ctrl_id = ctrl_id;   
    if (VP_PLAYBACK_MODE_AUDIO_VIDEO_SYN == VP_Setting_GetPlaybackMode())
    {
        pop_item_info.node_id = ID_VP_AUDIO_VIDEO_SYN;
    }
    else
    {
        pop_item_info.node_id = ID_VP_SCREEN_FLOW;
    }     
    GUIMENU_SetPopItemStatus(TRUE,&pop_item_info);
    index++;  
    j = 0;
#endif
    
    if (VP_PLAY_FROM_FILE == s_video_player_control_info.play_type
        || VP_PLAY_FROM_LOCALLIST == s_video_player_control_info.play_type
        || VP_PLAY_FROM_NET == s_video_player_control_info.play_type)
                
    {
        MMI_GetLabelTextByLang(STXT_DETAIL/*TXT_VP_FILEATTR*/, &kstring);
        node_item.item_text_ptr = &kstring;
        node_item.is_grayed = ((0==s_video_player_control_info.full_path_len)? TRUE:FALSE);
        GUIMENU_InsertNode(index, ID_VP_FILEATTR, 0, &node_item, ctrl_id);
        index++;        
    }

#ifndef MMI_STREAMING_PLAYER_SUPPORT
    MMI_GetLabelTextByLang(TXT_COMMON_SET_RESET, &kstring);
    node_item.item_text_ptr = &kstring;
    node_item.is_grayed = FALSE;
    GUIMENU_InsertNode(index, ID_VP_SET_RESET, 0, &node_item, ctrl_id);
    index++; 
#ifdef MMI_VIDEOPLAYER_HELP_SUPPORT 
    MMI_GetLabelTextByLang(TXT_HELP, &kstring);
    node_item.item_text_ptr = &kstring;
    node_item.is_grayed = FALSE;
    GUIMENU_InsertNode(index, ID_VP_HELP, 0, &node_item, ctrl_id);
    index++; 
    #endif
#endif
    
    MMI_GetLabelTextByLang(TXT_VP_EXIT, &kstring);
    node_item.item_text_ptr = &kstring;
    node_item.is_grayed = FALSE;
    GUIMENU_InsertNode(index, ID_VP_EXIT, 0, &node_item, ctrl_id);
    index++;
#endif  //MMI_VIDEO_PLAYER_MINI_SUPPORT  //6530_MINI
      
#else //PDA style pop menu
    
    //本地播放列表
    MMI_GetLabelTextByLang(TXT_VP_LOCALLIST, &kstring);
    node_item.item_text_ptr = &kstring;
    if (VP_ENTRY_FROM_PLAYER != s_video_player_control_info.vp_entry)
    {
        node_item.is_grayed = TRUE;
    }   
    else
    {
        node_item.is_grayed = FALSE;
    }
    node_item.select_icon_id = IMAGE_VP_MENU_LOCAL_PLAY_LIST;
    GUIMENU_InsertNode(index, ID_VP_OPEN_LOCALLIST, 0, &node_item, ctrl_id);
    index++;
    
    //网络浏览
#ifdef MMI_STREAMING_PLAYER_SUPPORT
    MMI_GetLabelTextByLang(TXT_VP_NET_VIEW, &kstring);
    node_item.item_text_ptr = &kstring;
    node_item.is_grayed = FALSE;
    node_item.select_icon_id = IMAGE_VP_MENU_OPEN_NET;
    GUIMENU_InsertNode(index, ID_VP_OPEN_NET, 0, &node_item, ctrl_id);
    index++;
#endif
    
    //本地浏览
    MMI_GetLabelTextByLang(TXT_VP_LOCALFILE_VIEW, &kstring);
    node_item.item_text_ptr = &kstring;
    node_item.is_grayed = FALSE;
    node_item.select_icon_id = IMAGE_VP_MENU_OPEN_LOCAL_LIST;
    GUIMENU_InsertNode(index, ID_VP_OPEN_LOCALFILE, 0, &node_item, ctrl_id);
    index++;
    
    //历史播放记录
#ifdef MMI_VIDEOPLAYER_HISTORYLIST_SUPPORT
    MMI_GetLabelTextByLang(TXT_VP_HISTORYLIST_ID, &kstring);
    node_item.item_text_ptr = &kstring;
    node_item.is_grayed = FALSE;
    node_item.select_icon_id = IMAGE_VP_MENU_HISTORY_PLAY_LIST;
    GUIMENU_InsertNode(index, ID_VP_VIEW_HISTORYLIST, 0, &node_item, ctrl_id);
    index++;
#endif
    
    //设置
//#ifdef MMI_STREAMING_PLAYER_SUPPORT
    MMI_GetLabelTextByLang(STXT_MAIN_SETTING, &kstring);
    node_item.item_text_ptr = &kstring;
    node_item.is_grayed = FALSE;
    node_item.select_icon_id = IMAGE_COMMON_OPTMENU_SET;
    GUIMENU_InsertNode(index, ID_VP_SETTING, 0, &node_item, ctrl_id);
    index++;    
//#endif
    
    
    if (VP_PLAY_FROM_FILE == s_video_player_control_info.play_type|| VP_PLAY_FROM_LOCALLIST == s_video_player_control_info.play_type)
    {
        //设为屏保
#ifdef SCREENSAVER_SUPPORT
        MMI_GetLabelTextByLang(TXT_VP_PDA_SCREENSAVE, &kstring);
        node_item.item_text_ptr = &kstring;
        node_item.is_grayed =((0==s_video_player_control_info.full_path_len)? TRUE:FALSE);
        node_item.select_icon_id = IMAGE_NULL;
        GUIMENU_InsertNode(index, ID_VP_SCEENSAVE, 0, &node_item, ctrl_id);
        index++;
#endif
        
        //设为开机动画
        MMI_GetLabelTextByLang(TXT_VP_PDA_POWER_ON_ANIMATION, &kstring);
        node_item.item_text_ptr = &kstring;
        node_item.is_grayed = ((0==s_video_player_control_info.full_path_len)? TRUE:FALSE);
        node_item.select_icon_id = IMAGE_NULL;
        GUIMENU_InsertNode(index, ID_VP_POWER_ON_SET, 0, &node_item, ctrl_id);
        index++;
        
        //设为关机动画
        MMI_GetLabelTextByLang(TXT_VP_PDA_POWER_OFF_ANIMATION, &kstring);
        node_item.item_text_ptr = &kstring;
        node_item.is_grayed = ((0==s_video_player_control_info.full_path_len)? TRUE:FALSE);
        node_item.select_icon_id = IMAGE_NULL;
        GUIMENU_InsertNode(index, ID_VP_POWER_OFF_SET, 0, &node_item, ctrl_id);
        index++;

        //设为视频墙纸
#ifdef MMI_VIDEOWALLPAPER_SUPPORT
        MMI_GetLabelTextByLang(TXT_VP_PDA_VIDEO_WALLPAPER, &kstring);
        node_item.item_text_ptr = &kstring;
        node_item.is_grayed =((0==s_video_player_control_info.full_path_len)? TRUE:FALSE);
        node_item.select_icon_id = IMAGE_NULL;
        GUIMENU_InsertNode(index, ID_VP_VIDEO_WALLPAPER, 0, &node_item, ctrl_id);
        index++;
#endif
    }
    
    //亮度调节
    MMI_GetLabelTextByLang(TXT_VP_BRIGHTNESS_ID, &kstring);
    node_item.item_text_ptr = &kstring;
    node_item.is_grayed = !IsAdjustBrightEnable();
    node_item.select_icon_id = IMAGE_NULL;
    GUIMENU_InsertNode(index, ID_VP_BRIGHTNESS, 0, &node_item, ctrl_id);
    index++;
    
    //文件属性
    if (VP_PLAY_FROM_FILE == s_video_player_control_info.play_type
        || VP_PLAY_FROM_LOCALLIST == s_video_player_control_info.play_type
        || VP_PLAY_FROM_NET == s_video_player_control_info.play_type)
        
    {
        MMI_GetLabelTextByLang(STXT_DETAIL/*TXT_VP_FILEATTR*/, &kstring);
        node_item.item_text_ptr = &kstring;
        node_item.is_grayed = ((0==s_video_player_control_info.full_path_len)? TRUE:FALSE);
        node_item.select_icon_id = IMAGE_NULL;
        GUIMENU_InsertNode(index, ID_VP_FILEATTR, 0, &node_item, ctrl_id);
        index++;        
    }
#ifdef MMI_STREAMING_PLAYER_SUPPORT //norflashcut 
    //我的收藏夹
    MMI_GetLabelTextByLang(TXT_VP_BOOKMARK_ID, &kstring);
    node_item.item_text_ptr = &kstring;
    node_item.is_grayed = FALSE;
    node_item.select_icon_id = IMAGE_NULL;
    GUIMENU_InsertNode(index, ID_VP_VIEW_BOOKMARK, 0, &node_item, ctrl_id);
    index++;
    
    //保存至收藏夹
    if(VP_PLAY_FROM_NET == s_video_player_control_info.play_type)
    {
        MMI_GetLabelTextByLang(TXT_VP_SAVE_BOOKMARK_ID, &kstring);
        node_item.item_text_ptr = &kstring;
        node_item.is_grayed = FALSE;
        node_item.select_icon_id = IMAGE_NULL;
        GUIMENU_InsertNode(index, ID_VP_SAVE_BOOKMAR, 0, &node_item, ctrl_id);
        index++;       
    }
#endif
   /* 
#ifndef MMI_STREAMING_PLAYER_SUPPORT
    //恢复出厂设置
    MMI_GetLabelTextByLang(TXT_COMMON_SET_RESET, &kstring);
    node_item.item_text_ptr = &kstring;
    node_item.is_grayed = FALSE;
    node_item.select_icon_id = IMAGE_NULL;
    GUIMENU_InsertNode(index, ID_VP_SET_RESET, 0, &node_item, ctrl_id);
    index++; 
    
    //帮助
    MMI_GetLabelTextByLang(TXT_HELP, &kstring);
    node_item.item_text_ptr = &kstring;
    node_item.is_grayed = FALSE;
    node_item.select_icon_id = IMAGE_NULL;
    GUIMENU_InsertNode(index, ID_VP_HELP, 0, &node_item, ctrl_id);
    index++; 
#endif
*/
    //退出播放器
    MMI_GetLabelTextByLang(TXT_VP_EXIT, &kstring);
    node_item.item_text_ptr = &kstring;
    node_item.is_grayed = FALSE;
    node_item.select_icon_id = IMAGE_NULL;
    GUIMENU_InsertNode(index, ID_VP_EXIT, 0, &node_item, ctrl_id);
    index++;
    
#endif //MMI_PDA_SUPPORT
}



#ifdef MMI_STREAMING_PLAYER_SUPPORT  //norflashcut
/*****************************************************************************/
//  Description : 网络选择窗口选项消息处理.
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleNetOptMenuWinMsg(
                                     MMI_WIN_ID_T       win_id,     
                                     MMI_MESSAGE_ID_E    msg_id, 
                                     DPARAM              param
                                     )
{
    MMI_RESULT_E                result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T               ctrl_id = VP_NET_OPT_MENU_CTRL_ID;
    MMI_MENU_ID_T               menu_id = 0;
    MMI_MENU_GROUP_ID_T         group_id = 0;   
    VP_NET_MOVIE_INFO_T         *net_movie_info_ptr = PNULL;
	
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMIAPICOM_EnableGrayed(win_id,MENU_VP_OPEN_OPT,ID_VP_OPEN_NET,TRUE);
        if (VP_NET_URL_INPUT_CTRL_ID == MMK_GetActiveCtrlId(MMIVP_NET_WIN_ID))
        {
            MMIAPICOM_EnableGrayed(win_id,MENU_VP_NET_URL_OPT,ID_VP_FILEATTR,TRUE);
        }     
        if (VP_ENTRY_FROM_PLAYER != s_video_player_control_info.vp_entry)
        {
            MMIAPICOM_EnableGrayed(win_id,MENU_VP_OPEN_OPT,ID_VP_OPEN_LOCALLIST,TRUE);
        }
        MMK_SetAtvCtrl(win_id,ctrl_id);
        break;

    case MSG_CTL_MIDSK:  
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_WEB:
    case MSG_APP_OK:
        GUIMENU_GetId(ctrl_id,&group_id,&menu_id);
        //VP_STRACE_LOW:"[VP] HandleNetOptMenuWinMsg menu_id = %x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_8131_112_2_18_3_5_44_480,(uint8*)"d",menu_id);
        switch (menu_id)
        {   
        case ID_VP_OPEN_LOCALFILE:
            if (OpenLocalFileWin(win_id))
            {
                MMK_CloseWin(win_id);
                MMK_CloseWin(MMIVP_NET_WIN_ID);
            }           
            break;

        case ID_VP_OPEN_LOCALLIST:
            OpenLocalListWin();
            MMK_CloseWin(win_id);
            MMK_CloseWin(MMIVP_NET_WIN_ID);                       
            break;
#ifdef MMI_VIDEOPLAYER_HISTORYLIST_SUPPORT
        case ID_VP_VIEW_HISTORYLIST:
            MMK_CreateWin((uint32 *)MMIVP_HISTORYLIST_WIN_TAB,PNULL);
            MMK_CloseWin(win_id);
            MMK_CloseWin(MMIVP_NET_WIN_ID);             
            break;
#endif

        case ID_VP_VIEW_BOOKMARK:
            MMK_CreateWin((uint32 *)MMIVP_BOOKMARKLIST_WIN_TAB,PNULL);
            MMK_CloseWin(win_id);
            MMK_CloseWin(MMIVP_NET_WIN_ID);            
            break;    
       
        case ID_VP_FILEATTR: 
            net_movie_info_ptr = (VP_NET_MOVIE_INFO_T*)SCI_ALLOCA(sizeof(VP_NET_MOVIE_INFO_T));
            if (PNULL != net_movie_info_ptr)
            {
                SCI_MEMSET(net_movie_info_ptr,0,sizeof(VP_NET_MOVIE_INFO_T));
                *net_movie_info_ptr = VP_GetCMCCAddressInfo();
                MMK_CreateWin((uint32 *)MMIVP_VIEW_NET_DETAIL_WIN_TAB,(ADD_DATA)net_movie_info_ptr);
            }              
            else
            {
                MMIPUB_OpenAlertFailWin(TXT_ERROR);
            }
            MMK_CloseWin(win_id);
            break;

        default:
            break;
        }
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}
#endif

/*****************************************************************************/
//  Description : 视频播放器选项消息处理.
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleOptMenuWinMsg(
                                     MMI_WIN_ID_T       win_id,     
                                     MMI_MESSAGE_ID_E    msg_id, 
                                     DPARAM              param
                                     )
{
    MMI_RESULT_E                result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T               ctrl_id = VP_OPT_MENU_CTRL_ID;
    MMI_MENU_ID_T               menu_id = 0;
   // MMI_MENU_GROUP_ID_T         group_id = 0;  
#ifdef MMI_STREAMING_PLAYER_SUPPORT
    VP_NET_MOVIE_INFO_T         *net_movie_info_ptr = PNULL;
#endif
    SFS_DATE_T                  date = {0};
    SFS_TIME_T                  time = {0};
    uint32  file_size = 0;
    MMIFMM_DETAIL_DATA_T        detail_data={0};

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        
        //++++CR242519
        #ifdef MMI_RES_ORIENT_PORTRAIT
#ifndef MMI_LOW_MEMORY_RESOURCE        
        if(MMITHEME_IsMainScreenLandscape())
        {
            //GUIWIN_SetSoftkeyCommonBg(win_id, IMAGE_COMMON_BG_H);
            GUIMENU_SetMenuBgColor(ctrl_id, MMI_WHITE_COLOR);
            GUIWIN_SetSoftkeyPressedStyle(win_id,
                                                                MMITHEME_GetCurThemeFontColor(MMI_THEME_SOFTKEY_FONT), 
                                                                GUI_SOFTKEY_BG_IMAGE_ONLY, 
                                                                0, 
                                                                IMAGE_SOFTKEY_BG_H_DOWN);
        }
#endif        
        #endif
#ifdef MMI_VIDEO_PLAYER_MINI_LAYOUT//6530 MINI 界面布局
#if !((defined MAINLCD_DEV_SIZE_220X176) || (defined MAINLCD_DEV_SIZE_160X128)) //6530_MINI 横屏不转屏
        MMK_SetWinSupportAngle(MMIVP_OPT_MENU_WIN_ID, WIN_SUPPORT_ANGLE_0);
        MMK_SetWinAngle(MMIVP_OPT_MENU_WIN_ID, LCD_ANGLE_0, TRUE);
#endif
#endif //MMI_VIDEO_PLAYER_MINI_SUPPORT //6530_MINI


        //+++++CR242519
        DisplayOptMenWin(win_id,ctrl_id);
        MMK_SetAtvCtrl(win_id,ctrl_id);
		GUIMENU_SetMenuBgColor(ctrl_id, MMI_WHITE_COLOR);
        break;

    case MSG_CTL_MIDSK:  
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_WEB:
    case MSG_APP_OK:
        menu_id = GUIMENU_GetCurNodeId(ctrl_id);
        //VP_STRACE_LOW:"[VP] HandleOptMenuWinMsg menu_id = %x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_8246_112_2_18_3_5_44_481,(uint8*)"d",menu_id);
        switch (menu_id)
        {
        case ID_VP_SETTING:           
#ifdef MMI_GUI_STYLE_TYPICAL
            MMK_CreateWin((uint32 *)MMIVP_SETTING_WIN_TAB,PNULL);
#else
            //PDA sytle
            MMK_CreateWin((uint32 *)MMIVP_PDA_SETTINGS_WIN_TAB,PNULL);
#endif
            MMK_CloseWin(win_id);
            break;
        case ID_VP_FLOWRATE:
#ifdef BROWSER_SUPPORT
            MMIAPICL_OpenGprsFlowRateWin();
#endif
            MMK_CloseWin(win_id);
            break;
#ifdef MMI_VIDEOPLAYER_HELP_SUPPORT
        case ID_VP_HELP:
            MMK_CreateWin((uint32 *)MMIVP_HELP_INFO_WIN_TAB,PNULL);  
            MMK_CloseWin(win_id);
            break;
#endif
        case ID_VP_OPEN_LOCALFILE:
            OpenLocalFileWin(win_id);
            break;

#ifdef MMI_STREAMING_PLAYER_SUPPORT  //norflashcut
        case ID_VP_OPEN_NET:
            MMK_CreateWin((uint32 *)MMIVP_NET_WIN_TAB,PNULL);  
            MMK_CloseWin(win_id);            
            break;  
#endif
        case ID_VP_OPEN_LOCALLIST:
            OpenLocalListWin();
            MMK_CloseWin(win_id);                       
            break;
#ifdef MMI_VIDEOPLAYER_HISTORYLIST_SUPPORT
        case ID_VP_VIEW_HISTORYLIST:
            MMK_CreateWin((uint32 *)MMIVP_HISTORYLIST_WIN_TAB,PNULL);
            MMK_CloseWin(win_id);            
            break;
#endif

#ifdef MMI_STREAMING_PLAYER_SUPPORT
        case ID_VP_VIEW_BOOKMARK:
            MMK_CreateWin((uint32 *)MMIVP_BOOKMARKLIST_WIN_TAB,PNULL);
            MMK_CloseWin(win_id);            
            break;    
#endif
        case ID_VP_FULLSCREEN:   
            MMK_CloseWin(win_id);  
            MMK_PostMsg(MMIVP_PLAYER_WIN_ID,MSG_KEYDOWN_2,PNULL,PNULL);                       
            break;

        case ID_VP_BRIGHTNESS:          
             MMK_CloseWin(win_id);   
			 s_video_player_control_info.is_adjust_bright = TRUE;
            // MMK_PostMsg(MMIVP_PLAYER_WIN_ID,MSG_VP_OPEN_BRIGHT_CONTROL_PANE,PNULL,PNULL);       
            break;
#ifdef MMI_STREAMING_PLAYER_SUPPORT
        case ID_VP_SAVE_BOOKMAR:
            SaveBookMark();
            MMK_CloseWin(win_id);
            break;
#endif
        case ID_VP_FILEATTR:
#ifdef MMI_STREAMING_PLAYER_SUPPORT
            if(VP_PLAY_FROM_NET == s_video_player_control_info.play_type ) 
            {
                net_movie_info_ptr = (VP_NET_MOVIE_INFO_T*)SCI_ALLOCA(sizeof(VP_NET_MOVIE_INFO_T));
                if (PNULL != net_movie_info_ptr)
                {                
                    SCI_MEMSET(net_movie_info_ptr,0,sizeof(VP_NET_MOVIE_INFO_T));
                    net_movie_info_ptr->title_len = s_video_player_control_info.full_path_len;
                    MMIAPICOM_Wstrncpy(net_movie_info_ptr->title_arr,s_video_player_control_info.full_path_name,s_video_player_control_info.full_path_len);
                    SCI_MEMCPY(net_movie_info_ptr->url_ptr,s_video_player_control_info.url,VP_MAX_URL_LEN);
                    MMK_CreateWin((uint32 *)MMIVP_VIEW_NET_DETAIL_WIN_TAB,(ADD_DATA)net_movie_info_ptr);
                }
                else
                {
                    //VP_STRACE_LOW:"[VP] HandleLocalListOptMenuWinMsg net alloca memory error"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_8327_112_2_18_3_5_45_482,(uint8*)"");
                    MMIPUB_OpenAlertFailWin(TXT_ERROR);
                }
            }
            else
#endif
            if (!MMIAPIFMM_IsFileExist(s_video_player_control_info.full_path_name,s_video_player_control_info.full_path_len))
            {
                MMIPUB_OpenAlertWarningWin(TXT_COM_FILE_NO_EXIST);
            }
            else
            {
                MMIAPIFMM_GetFileInfo(s_video_player_control_info.full_path_name,
                    s_video_player_control_info.full_path_len,
                    &(detail_data.file_size),
                    &date,
                    &time);
                detail_data.time = MMIAPICOM_Tm2Second(time.sec, time.min, time.hour, date.mday, date.mon, date.year);
                detail_data.filename_len = MIN(MMIFMM_FILENAME_LEN,s_video_player_control_info.full_path_len);
                MMIAPICOM_Wstrncpy(detail_data.filename,s_video_player_control_info.full_path_name,detail_data.filename_len);
#ifdef DRM_SUPPORT
                if(MMIAPIDRM_IsDRMFile(PNULL,detail_data.filename))
                {
                    detail_data.type = MMIFMM_FILE_TYPE_DRM;
                }
                else
#endif
                {
                    detail_data.type = MMIFMM_FILE_TYPE_MOVIE;
                }
                MMIAPIFMM_DetailFileData(&detail_data);
            }   
            MMK_CloseWin(win_id);
            break;

        case ID_VP_EXIT:
#ifdef MMI_VIDEOPLAYER_HISTORYLIST_SUPPORT
            if (IsSaveLastPlayPoint())
            { 
                VP_HistoryList_SetLastPlayPoint(s_video_player_control_info.process_time);
            }
#endif //MMI_VIDEOPLAYER_HISTORYLIST_SUPPORT
            MMIAPIVP_FSMExit(s_video_player_control_info.vp_handle);            
            MMK_CloseWin(win_id);            
            MMK_CloseWin(MMIVP_PLAYER_WIN_ID);
            break;

        case ID_VP_SCEENSAVE:
#ifdef SCREENSAVER_SUPPORT
            MMIAPIFMM_GetFileInfo(s_video_player_control_info.full_path_name,s_video_player_control_info.full_path_len,&file_size,PNULL,PNULL);

            if(!MMIAPIFMM_IsFileExist(s_video_player_control_info.full_path_name,s_video_player_control_info.full_path_len))
            {
                MMIPUB_OpenAlertWarningWin(TXT_COM_FILE_NO_EXIST);
            }
            else if (0 == file_size)
            {
                MMIPUB_OpenAlertWarningWin(TXT_EMPTY_FILE);
            }
            else
            {
                SettingAsScreensaver(s_video_player_control_info.full_path_name,s_video_player_control_info.full_path_len);
            }
#endif
            MMK_CloseWin(win_id);
            break;

#ifdef MMI_VIDEOWALLPAPER_SUPPORT
        case ID_VP_VIDEO_WALLPAPER:
            MMIAPIFMM_GetFileInfo(s_video_player_control_info.full_path_name,s_video_player_control_info.full_path_len,&file_size,PNULL,PNULL);
            SettingAsVideoWallpaper(s_video_player_control_info.full_path_name,s_video_player_control_info.full_path_len,file_size);
            MMK_CloseWin(win_id);
            break;
#endif

        case ID_VP_POWER_ON_SET:
            MMIAPIFMM_GetFileInfo(s_video_player_control_info.full_path_name,s_video_player_control_info.full_path_len,&file_size,PNULL,PNULL);
            
            if(!MMIAPIFMM_IsFileExist(s_video_player_control_info.full_path_name,s_video_player_control_info.full_path_len))
            {
                MMIPUB_OpenAlertWarningWin(TXT_COM_FILE_NO_EXIST);
            } 
             else if (0 == file_size)//@andy.he cr240708 2011/06/09
            {
                MMIPUB_OpenAlertWarningWin(TXT_EMPTY_FILE);
            }  
            else
            {
                SettingAsPowerOnOffAnimation(TRUE,s_video_player_control_info.full_path_name,s_video_player_control_info.full_path_len);
            }
            MMK_CloseWin(win_id);
            break;

        case ID_VP_POWER_OFF_SET:
            MMIAPIFMM_GetFileInfo(s_video_player_control_info.full_path_name,s_video_player_control_info.full_path_len,&file_size,PNULL,PNULL);
            
            if(!MMIAPIFMM_IsFileExist(s_video_player_control_info.full_path_name,s_video_player_control_info.full_path_len))
            {
                MMIPUB_OpenAlertWarningWin(TXT_COM_FILE_NO_EXIST);
            }   
            else if (0 == file_size)//@andy.he cr240708 2011/06/09
            {
                MMIPUB_OpenAlertWarningWin(TXT_EMPTY_FILE);
            } 
            else
            {
                SettingAsPowerOnOffAnimation(FALSE,s_video_player_control_info.full_path_name,s_video_player_control_info.full_path_len);
            }
            MMK_CloseWin(win_id);
            break;

        case ID_VP_RANDOM_OPEN:
            VP_Setting_SetIsRandomPlayMode(TRUE);            
            MMIAPIVP_FSMUpdatePlayMode(s_video_player_control_info.vp_handle,
                VP_Setting_GetRepeatPlayMode(),
                TRUE);
            MMK_CloseWin(win_id);
            break;

        case ID_VP_RANDOM_CLOSE:
            VP_Setting_SetIsRandomPlayMode(FALSE);
            MMIAPIVP_FSMUpdatePlayMode(s_video_player_control_info.vp_handle,
                VP_Setting_GetRepeatPlayMode(),
                FALSE);
            MMK_CloseWin(win_id);
            break;

        case ID_VP_REPEAT_CLOSE:
            VP_Setting_SetRepeatPlayMode(VP_REPEAT_PLAY_MODE_CLOSE);
            MMIAPIVP_FSMUpdatePlayMode(s_video_player_control_info.vp_handle,
                VP_REPEAT_PLAY_MODE_CLOSE,
                VP_Setting_GetIsRandomPlayMode());
            MMK_CloseWin(win_id);
            break;

        case ID_VP_REPEAT_ALL:
            VP_Setting_SetRepeatPlayMode(VP_REPEAT_PLAY_MODE_ALL);
            MMIAPIVP_FSMUpdatePlayMode(s_video_player_control_info.vp_handle,
                VP_REPEAT_PLAY_MODE_ALL,
                VP_Setting_GetIsRandomPlayMode());
            MMK_CloseWin(win_id);
            break;

        case ID_VP_REPEAT_ONE:
            VP_Setting_SetRepeatPlayMode(VP_REPEAT_PLAY_MODE_ONE);  
            MMIAPIVP_FSMUpdatePlayMode(s_video_player_control_info.vp_handle,
                VP_REPEAT_PLAY_MODE_ONE,
                VP_Setting_GetIsRandomPlayMode());
            MMK_CloseWin(win_id);
            break;


        case ID_VP_SET_RESET:
            MMIPUB_OpenQueryWinByTextId(TXT_RESET_QUERY,IMAGE_PUBWIN_QUERY,PNULL,PNULL);
            break;

		case ID_VP_SCREEN_FLOW:
			VP_Setting_SetPlaybackMode(VP_PLAYBACK_MODE_SCREEN_FLOW);  
			MMIAPIVP_FSMUpdatePlayBackMode(
				s_video_player_control_info.vp_handle,
				VP_PLAYBACK_MODE_SCREEN_FLOW);
			MMK_CloseWin(win_id);
			break;

		case ID_VP_AUDIO_VIDEO_SYN:
			VP_Setting_SetPlaybackMode(VP_PLAYBACK_MODE_AUDIO_VIDEO_SYN);  
			MMIAPIVP_FSMUpdatePlayBackMode(
				s_video_player_control_info.vp_handle,
				VP_PLAYBACK_MODE_AUDIO_VIDEO_SYN);
			MMK_CloseWin(win_id);
			break;
#if defined MMI_VIDEO_PLAYER_MINI_SUPPORT && defined (MMI_VIDEO_PLAYER_MINI_LAYOUT) //6530_MINI 
        case ID_VP_SET_REPEATE_MODE:
            MMK_CreatePubListWin((uint32 *)MMIVP_PLAY_REPEAT_MODE_WIN_TAB, PNULL);
            break; 

        case ID_VP_SET_RANDOM_MODE:
            MMK_CreatePubListWin((uint32 *)MMIVP_PLAY_RANDOM_MODE_WIN_TAB, PNULL);
            break;
#endif
        default:
            break;
        }
        break;

    case MSG_PROMPTWIN_OK:        
        MMIPUB_CloseQuerytWin(PNULL);
        MMIAPIVP_ResetSettingInfo(); 
        //need update current vol about  fsm_ptr after reset settings.
        //MMIAPIVP_FSMUpdateCurrentVol(s_video_player_control_info.vp_handle,MMIAPISET_GetMultimVolume());

        //MMIPUB_OpenAlertSuccessWin( TXT_SUCCESS);
        MMK_CloseWin(win_id);
        break;

    case MSG_PROMPTWIN_CANCEL:
        MMIPUB_CloseQuerytWin(PNULL);
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}


#ifdef MMI_VIDEOPLAYER_HELP_SUPPORT
/*****************************************************************************/
//  Description : the window for vp help information
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleVPHelpWinMsg(
                                      MMI_WIN_ID_T win_id,
                                      MMI_MESSAGE_ID_E msg_id,
                                      DPARAM param
                                      )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_STRING_T help_string = {0};
        
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
       // GUIWIN_SetStbBgImageId(win_id,IMAGE_COMMON_BG);
        #ifdef MMI_VIDEO_PLAYER_MINI_LAYOUT
        #if defined MMI_STREAMING_PLAYER_SUPPORT && defined MMI_VIDEOPLAYER_HISTORYLIST_SUPPORT && defined MMI_VIDEOPLAYER_SHORTCUTMENU_SUPPORT  //6530_MINI         
            MMI_GetLabelTextByLang(TXT_VP_HELP_CONTENT_SENSOR, &help_string);
        #else
            MMI_GetLabelTextByLang(TXT_VP_HELP_LOW_MEMORY, &help_string);
        #endif
        #else
        MMI_GetLabelTextByLang(TXT_VP_HELP_CONTENT_ID, &help_string); 
        #endif
        GUITEXT_SetString(VP_HELP_TEXT_CTRL_ID , help_string.wstr_ptr, help_string.wstr_len, FALSE);  
        MMK_SetAtvCtrl(win_id, VP_HELP_TEXT_CTRL_ID);
        break;      

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
      
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }

    return recode;
}
#endif
#ifdef MMI_GPRS_SUPPORT
    /*****************************************************************************/
    //  Description : update net setting when the index list item is selected  
    //  Global resource dependence : none
    //  Author: jixin.xu
    //  Note:
    /*****************************************************************************/
LOCAL void MMIVP_ConnectionCallback(MN_DUAL_SYS_E dual_sys, uint32 selected_index)
    {
        VP_Setting_SetNetIndex(dual_sys, selected_index);
    }
#endif



#ifdef MMI_GUI_STYLE_TYPICAL  //norflashcut
/*****************************************************************************/
//  Description : the window for vp setting information
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSettingMenuWinMsg(
                                      MMI_WIN_ID_T win_id,
                                      MMI_MESSAGE_ID_E msg_id,
                                      DPARAM param
                                      )
{
    MMI_RESULT_E          result = MMI_RESULT_TRUE;
    MMI_MENU_ID_T        menu_id = 0;
    MMI_MENU_GROUP_ID_T group_id = 0;
    uint16            list_index = 0;
#ifndef MMI_MULTI_SIM_SYS_SINGLE
    uint32 i = MN_DUAL_SYS_1;
    MMI_MENU_ID_T vp_set_net[] =
    {
        ID_VP_SET_NET1,
        ID_VP_SET_NET2,
        ID_VP_SET_NET3,
        ID_VP_SET_NET4
    };
#endif

    switch(msg_id) 
    {
    case MSG_OPEN_WINDOW:
#ifndef MMI_MULTI_SIM_SYS_SINGLE
        for(i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
        {
            MMIAPICOM_EnableGrayed(win_id,MENU_VP_SET_OPT,vp_set_net[i],(BOOLEAN)(!MMIAPIPHONE_IsSimAvailable(i)));
        }        
#else
        MMIAPICOM_EnableGrayed(win_id,MENU_VP_SET_OPT,ID_VP_SET_NET,(BOOLEAN)!(MMIAPIPHONE_IsSimAvailable(MN_DUAL_SYS_1)));
#endif
        MMK_SetAtvCtrl(win_id,VP_MENU_SET_OPT_CTRL_ID);
        break;

    case MSG_APP_WEB:
    case MSG_APP_OK:
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        GUIMENU_GetId(MMK_GetCtrlHandleByWin(win_id, VP_MENU_SET_OPT_CTRL_ID),&group_id,&menu_id);
        switch( menu_id )
        { 
#ifdef MMI_STREAMING_PLAYER_SUPPORT  //norflashcut       
#ifndef MMI_MULTI_SIM_SYS_SINGLE
        case ID_VP_SET_NET1:
        case ID_VP_SET_NET2:
        case ID_VP_SET_NET3:
        case ID_VP_SET_NET4:
            list_index = MMIAPICONNECTION_GetListIndexByAPNIndex((MN_DUAL_SYS_1+(menu_id - ID_VP_SET_NET1)),
                                                                 VP_Setting_GetNetIndex((MN_DUAL_SYS_E)(MN_DUAL_SYS_1+(menu_id - ID_VP_SET_NET1))));
            MMIAPICONNECTION_CreateLinklistWin(MMICONNECTION_ACCOUNT_BROWSER,list_index,
                                               (MN_DUAL_SYS_1+(menu_id - ID_VP_SET_NET1)), MMIVP_ConnectionCallback);
                break;       
#else
        case ID_VP_SET_NET:
            list_index = MMIAPICONNECTION_GetListIndexByAPNIndex(MN_DUAL_SYS_1,
                                                                 VP_Setting_GetNetIndex((MN_DUAL_SYS_E)(MN_DUAL_SYS_1)));
            MMIAPICONNECTION_CreateLinklistWin(MMICONNECTION_ACCOUNT_BROWSER, list_index,
                                               MN_DUAL_SYS_1, MMIVP_ConnectionCallback);
            break;
#endif
		case ID_VP_SET_PLAYBACK_MODE:
			MMK_CreateWin((uint32 *) MMIVP_SETTINGS_PLAYBACK_MODE_WIN_TAB, PNULL);
			break;

        case ID_VP_SET_RTCP_RTP:
            MMK_CreateWin((uint32 *) MMIVP_RTP_RTCP_WIN_TAB, PNULL);
            break;
            
        case ID_VP_SET_AGENT:
            MMK_CreateWin((uint32 *) MMIVP_AGENT_WIN_TAB,PNULL);
            break;
#endif  //MMI_STREAMING_PLAYER_SUPPORT    
#ifdef BROWSER_SUPPORT
         case ID_VP_FLOWRATE:
            MMIAPICL_OpenGprsFlowRateWin();           
            break;
#endif

        case ID_VP_SET_RESET:
            MMIPUB_OpenQueryWinByTextId(TXT_RESET_QUERY,IMAGE_PUBWIN_QUERY,PNULL,PNULL);
            break;
#ifdef MMI_VIDEOPLAYER_HELP_SUPPORT
        case ID_VP_HELP:
            MMK_CreateWin((uint32 *)MMIVP_HELP_INFO_WIN_TAB,PNULL);  
            break;
#endif
        default:
            break;
        }
        break;

    case MSG_PROMPTWIN_OK:
        MMIPUB_CloseQuerytWin(PNULL);
        MMIAPIVP_ResetSettingInfo(); 
        //need update current vol about  fsm_ptr after reset settings.
        //MMIAPIVP_FSMUpdateCurrentVol(s_video_player_control_info.vp_handle,MMIAPISET_GetMultimVolume());

        MMIPUB_OpenAlertSuccessWin( TXT_SUCCESS);
        break;

    case MSG_PROMPTWIN_CANCEL:
        MMIPUB_CloseQuerytWin(PNULL);
        break;

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return result;
}
#endif

#ifdef MMI_STREAMING_PLAYER_SUPPORT  //norflashcut 
/*****************************************************************************/
//  Description :setting link list
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleLinkListWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E        err_code = MMI_RESULT_TRUE;
    uint8               link_index = 0;
    MN_DUAL_SYS_E       dual_sim = MN_DUAL_SYS_MAX;   
    MMICONNECTION_LINKSETTING_DETAIL_T* link_setting_ptr = PNULL;
    GUILIST_ITEM_T      item_t = {0};
    GUILIST_ITEM_DATA_T item_data = {0};
    MMI_CTRL_ID_T       ctrl_id = VP_LISTBOX_LINK_SETTINGCTRL_ID;
#ifndef MMI_MULTI_SIM_SYS_SINGLE 
    MMI_TEXT_ID_T		text_vp_set_net_sim[] =
    {
        TXT_COMMON_SET_NET_SIM1,
        TXT_COMMON_SET_NET_SIM2,
        #if defined(MMI_MULTI_SIM_SYS_TRI)||defined(MMI_MULTI_SIM_SYS_QUAD)
        TXT_COMMON_SET_NET_SIM3,
        #endif
        #if defined(MMI_MULTI_SIM_SYS_QUAD)
        TXT_COMMON_SET_NET_SIM4
        #endif
    };
#endif

    dual_sim = (uint32)MMK_GetWinAddDataPtr(win_id);
    
    //VP_STRACE_LOW:"[VP]HandleLinkListWinMsg() msg_id=0x%04x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_8729_112_2_18_3_5_46_483,(uint8*)"d", msg_id);

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUILIST_SetMaxItem(ctrl_id, MMICONNECTION_MAX_SETTING_NUM, FALSE );
        for(link_index = 0; link_index < MMIAPICONNECTION_GetLinkSettingNum(dual_sim); link_index++)
        {
            link_setting_ptr = MMIAPICONNECTION_GetLinkSettingItemByIndex(dual_sim, link_index);            
            item_t.item_style = GUIITEM_STYLE_ONE_LINE_RADIO;
            item_t.item_data_ptr = &item_data;
            item_data.softkey_id[0] = TXT_COMMON_OK;
            item_data.softkey_id[1] = TXT_NULL;
            item_data.softkey_id[2] = STXT_RETURN;
            item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
            item_data.item_content[0].item_data.text_buffer.wstr_len = link_setting_ptr->name_len;
            item_data.item_content[0].item_data.text_buffer.wstr_ptr = link_setting_ptr->name;
    
            GUILIST_AppendItem(ctrl_id, &item_t);
        }
#ifndef MMI_MULTI_SIM_SYS_SINGLE        
        GUITITLE_SetTextId(win_id,MMITHEME_GetTitleCtrlId(),text_vp_set_net_sim[dual_sim],FALSE);      
#endif
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;

    case MSG_FULL_PAINT:
        link_index = VP_Setting_GetNetIndex(dual_sim);
        GUILIST_SetSelectedItem(ctrl_id,link_index,TRUE);
        GUILIST_SetCurItemIndex(ctrl_id,link_index);
        break;

    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
    case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_OK:
        link_index = (uint8)GUILIST_GetCurItemIndex(ctrl_id);
        GUILIST_SetSelectedItem(ctrl_id, link_index, TRUE);
        VP_Setting_SetNetIndex(dual_sim,link_index);
        MMIPUB_OpenAlertSuccessWin( TXT_SUCCESS);
        MMK_CloseWin(win_id);
        break;

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;

    default :
        err_code = MMI_RESULT_FALSE;
        break;
    }    
    
    return err_code;
}

/****************************************************************************/    
//  Description : SetInputRTCPPortDetailEntry
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void SetInputRTCPPortDetailEntry(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id)
{
    uint16 lowest_port = 0;
    uint16 highest_port = 0;
    uint8  port_str[VP_MAX_PORT_LENGTH +1] = {0};
    wchar  port_wstr[VP_MAX_PORT_LENGTH +1] = {0};
    uint16 port_len = 0;
    
    GUIFORM_SetStyle(VP_RTCP_LOWEST_PORT_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
    GUIFORM_SetStyle(VP_RTCP_HIGHEST_PORT_FROM_CTRL_ID,GUIFORM_STYLE_UNIT);

    VP_Setting_GetRTPPort(&lowest_port,&highest_port);

    GUILABEL_SetTextById(VP_RTCP_LOWEST_PORT_LABEL_CTRL_ID,TXT_VP_SET_RTCP_LOWEST_PORT,FALSE);       
    
    SCI_MEMSET(port_str,0,(VP_MAX_PORT_LENGTH +1));
    _snprintf((char*)port_str, (VP_MAX_PORT_LENGTH+1),"%d", lowest_port);
    port_len = strlen((char*)port_str);
    SCI_MEMSET(port_wstr,0,((VP_MAX_PORT_LENGTH +1)*sizeof(wchar)));
    MMIAPICOM_StrToWstr((uint8*)port_str, (wchar *)port_wstr);
    GUIEDIT_SetString(VP_RTCP_LOWEST_PORT_INPUT_CTRL_ID,port_wstr,port_len);
    

    GUILABEL_SetTextById(VP_RTCP_HIGHEST_PORT_LABEL_CTRL_ID,TXT_VP_SET_RTCP_HIGHEST_PORT,FALSE);   
    SCI_MEMSET(port_str,0,(VP_MAX_PORT_LENGTH +1));
    _snprintf((char*)port_str, (VP_MAX_PORT_LENGTH+1),"%d", highest_port);
    port_len = strlen((char*)port_str);
    SCI_MEMSET(port_wstr,0,((VP_MAX_PORT_LENGTH +1)*sizeof(wchar)));
    MMIAPICOM_StrToWstr((uint8*)port_str,(wchar *)port_wstr);
    GUIEDIT_SetString(VP_RTCP_HIGHEST_PORT_INPUT_CTRL_ID,port_wstr,port_len);
  
     
}

/*****************************************************************************/
//  Description :HandleRTPOrRTCPWinMsg
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleRTPOrRTCPWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E                recode          = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T               ctrl_id = VP_RTCP_PORT_INPUT_FORM_CTRL_ID;
    uint32                      lowest_port = 0;
    uint8                       lowest_port_ptr[VP_MAX_PORT_LENGTH +1] = {0};
    uint32                      highest_port = 0;
    uint8                       highest_port_ptr[VP_MAX_PORT_LENGTH +1] = {0};
    MMI_STRING_T                string = {0};



    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
      //  GUIWIN_SetStbBgImageId(win_id,IMAGE_COMMON_BG);
        SetInputRTCPPortDetailEntry(win_id, ctrl_id);       
#ifndef MMI_GUI_STYLE_TYPICAL
        GUIFORM_SetType(VP_RTCP_PORT_INPUT_FORM_CTRL_ID, GUIFORM_TYPE_TP);
#endif	
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;   
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:          
        MMK_CloseWin(win_id);     
        break;        
   
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#ifdef MMI_PDA_SUPPORT
        if ((PNULL != param) && ((MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID == ((MMI_NOTIFY_T*)param)->src_id)
            || (MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID == ((MMI_NOTIFY_T*)param)->src_id)
            || (MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID == ((MMI_NOTIFY_T*)param)->src_id)))
        {
            if ((MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID == ((MMI_NOTIFY_T*)param)->src_id))
            {
                //left softkey处理
            }
            else if ((MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID == ((MMI_NOTIFY_T*)param)->src_id))
            {
                //middle softkey处理
                MMK_PostMsg(win_id, MSG_APP_OK, PNULL, NULL);
            }
            else if ((MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID == ((MMI_NOTIFY_T*)param)->src_id))
            {
                //right softkey处理
            }
        }
#endif //MMI_PDA_SUPPORT
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

    case MSG_APP_OK:
    case MSG_CTL_OK:    
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        GUIEDIT_GetString(VP_RTCP_LOWEST_PORT_INPUT_CTRL_ID,&string);      
        SCI_MEMSET(lowest_port_ptr,0,(VP_MAX_PORT_LENGTH+1));
        MMIAPICOM_WstrToStr(string.wstr_ptr, (uint8*)lowest_port_ptr);
        lowest_port = atoi((char*)lowest_port_ptr);       

        if(VP_MAX_PORT < lowest_port
            || 0 == string.wstr_len)
        {
            MMIPUB_OpenAlertWarningWin(TXT_VP_LOWEST_PORT_INVAILD_NUMBER); 
            break;
        }
        GUIEDIT_GetString(VP_RTCP_HIGHEST_PORT_INPUT_CTRL_ID,&string);
        SCI_MEMSET(highest_port_ptr,0,(VP_MAX_PORT_LENGTH+1));
        MMIAPICOM_WstrToStr(string.wstr_ptr, (uint8*)highest_port_ptr);
        highest_port = atoi((char*)highest_port_ptr);   

        if(VP_MAX_PORT < highest_port
            || 0 == string.wstr_len)
        {           
            MMIPUB_OpenAlertWarningWin(TXT_VP_HIGHEST_PORT_INVAILD_NUMBER); 
        }
        else if (highest_port < lowest_port)
        {
            MMIPUB_OpenAlertWarningWin(TXT_VP_PORT_INVALID); 
        }
        else
        {        
            VP_Setting_SetRTPPort((uint16)lowest_port, (uint16)highest_port);   
		    //xin.li_clean_popwin			
            //MMIPUB_OpenAlertSuccessWin(TXT_COMM_SAVE_SUCCESS);
            MMK_CloseWin(win_id);   
        }
        break;  
    
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
    
}

/****************************************************************************/    
//  Description : SetInputAgentDetailEntry
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void SetInputAgentDetailEntry(void)
{
    BOOLEAN                 is_use_agent = FALSE;
    uint8                   cur_index = 0;
    uint8                   ip_str[VP_MAX_IP_LENGTH +1] = {0};
    uint16                  port = 0;
    uint8                   port_str[VP_MAX_PORT_LENGTH +1] = {0};
    wchar                   port_wstr[VP_MAX_PORT_LENGTH + 1] = {0};
    uint32                  ip_address = 0;
    uint16                  port_len = 0;
  

    GUIFORM_SetStyle(VP_AGENT_OPEN_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
    GUIFORM_SetStyle(VP_AGENT_IP_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
    GUIFORM_SetStyle(VP_AGENT_PORT_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);

    /* 是否使用代理*/
    GUILABEL_SetTextById(VP_AGENT_LABEL_CTRL_ID,TXT_PROXY_USE_QUERY,FALSE);   

    GUISETLIST_AddItemById(VP_AGENT_INPUT_SETLIST_CTRL_ID, TXT_YES);
    GUISETLIST_AddItemById(VP_AGENT_INPUT_SETLIST_CTRL_ID, TXT_COMM_NO);
    GUISETLIST_SetTitleTextId(VP_AGENT_INPUT_SETLIST_CTRL_ID, TXT_PROXY_USE_QUERY);

    //get current method index
    VP_Setting_GetAgent(&is_use_agent,ip_str,VP_MAX_IP_LENGTH,&port);   

    if (is_use_agent)
    {
        cur_index = 0;
    }
    else
    {
        cur_index = 1;
    }
     //set select method item
    GUISETLIST_SetCurIndex(VP_AGENT_INPUT_SETLIST_CTRL_ID, cur_index);

    GUILABEL_SetTextById(VP_AGENT_IP_LABEL_CTRL_ID,TXT_VP_AGENT_IP,FALSE);      
    MMIAPICONNECTION_IpStrToNum((uint8 *)ip_str,&ip_address);
    GUIEDIT_SetIP(VP_AGENT_IP_INPUT_CTRL_ID, ip_address);   

    GUILABEL_SetTextById(VP_AGENT_PORT_LABEL_CTRL_ID,TXT_VP_AGENT_PORT,FALSE);      

    
    SCI_MEMSET(port_str,0,(VP_MAX_PORT_LENGTH +1));
    _snprintf((char*)port_str, (VP_MAX_PORT_LENGTH+1),"%d", port);
    port_len = strlen((char*)port_str);
    SCI_MEMSET(port_wstr,0,((VP_MAX_PORT_LENGTH +1)*sizeof(wchar)));
    MMIAPICOM_StrToWstr((uint8*)port_str, (wchar *)port_wstr);

    GUIEDIT_SetString(VP_AGENT_PORT_INPUT_CTRL_ID,port_wstr,port_len); 
    
    if (!is_use_agent)
    {
        GUIFORM_SetChildDisplay(VP_AGENT_FORM_CTRL_ID,
            VP_AGENT_IP_FORM_CTRL_ID,
            GUIFORM_CHILD_DISP_GRAY
            );

        GUIFORM_SetChildDisplay(VP_AGENT_FORM_CTRL_ID,
            VP_AGENT_PORT_FORM_CTRL_ID,
            GUIFORM_CHILD_DISP_GRAY
            );
    }
    
}

/*****************************************************************************/
//  Description :HandleAgentWinMsg
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleAgentWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E                recode          = MMI_RESULT_TRUE;
    uint32                      focus_index = 0;
    BOOLEAN                     is_use_agent = FALSE;
    uint8                       ip_string[VP_MAX_IP_LENGTH +1] = {0}; 
    MMI_STRING_T                port_string = {0};
    uint32                      ip_address = 0;
    uint8                       port_str[VP_MAX_PORT_LENGTH +1] = {0};
    uint32                      port = 0;


    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
      //  GUIWIN_SetStbBgImageId(win_id,IMAGE_COMMON_BG);
        SetInputAgentDetailEntry();
#ifndef MMI_GUI_STYLE_TYPICAL
        GUIFORM_SetType(VP_AGENT_FORM_CTRL_ID, GUIFORM_TYPE_TP);
#endif	
        GUISETLIST_SetFontAlign(VP_AGENT_INPUT_SETLIST_CTRL_ID, ALIGN_LVMIDDLE);
        MMK_SetAtvCtrl(win_id, VP_AGENT_FORM_CTRL_ID);
        break;    

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:          
        MMK_CloseWin(win_id);     
        break;        
   
   case MSG_CTL_SETLIST_SWITCH:    
        focus_index = GUISETLIST_GetCurIndex(VP_AGENT_INPUT_SETLIST_CTRL_ID);
        if (1 == focus_index)
        {
            GUIFORM_SetChildDisplay(VP_AGENT_FORM_CTRL_ID,
                VP_AGENT_IP_FORM_CTRL_ID,
                GUIFORM_CHILD_DISP_GRAY
                );

            GUIFORM_SetChildDisplay(VP_AGENT_FORM_CTRL_ID,
                VP_AGENT_PORT_FORM_CTRL_ID,
                GUIFORM_CHILD_DISP_GRAY
                );
        }
        else
        {
            GUIFORM_SetChildDisplay(VP_AGENT_FORM_CTRL_ID,
                VP_AGENT_IP_FORM_CTRL_ID,
                GUIFORM_CHILD_DISP_NORMAL
                );

            GUIFORM_SetChildDisplay(VP_AGENT_FORM_CTRL_ID,
                VP_AGENT_PORT_FORM_CTRL_ID,
                GUIFORM_CHILD_DISP_NORMAL
                );
        }
        MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, 0);//@andy.he cr245534 2011/06/08
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
   case MSG_CTL_PENOK:
#ifdef MMI_PDA_SUPPORT
       if ((PNULL != param) && ((MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID == ((MMI_NOTIFY_T*)param)->src_id)
           || (MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID == ((MMI_NOTIFY_T*)param)->src_id)
           || (MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID == ((MMI_NOTIFY_T*)param)->src_id)))
       {
           if ((MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID == ((MMI_NOTIFY_T*)param)->src_id))
           {
               //left softkey处理
           }
           else if ((MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID == ((MMI_NOTIFY_T*)param)->src_id))
           {
               //middle softkey处理
               MMK_PostMsg(win_id, MSG_APP_OK, PNULL, NULL);
           }
           else if ((MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID == ((MMI_NOTIFY_T*)param)->src_id))
           {
               //right softkey处理
           } 
       }
#endif //MMI_PDA_SUPPORT
       break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

    case MSG_APP_OK:
    case MSG_CTL_OK:  
    case MSG_CTL_MIDSK:          
    case MSG_APP_WEB:    
        focus_index = GUISETLIST_GetCurIndex(VP_AGENT_INPUT_SETLIST_CTRL_ID);
        if (0 == focus_index)
        {
            is_use_agent = TRUE;
            ip_address = GUIEDIT_GetIP(VP_AGENT_IP_INPUT_CTRL_ID);
            SCI_MEMSET(ip_string,0,(VP_MAX_IP_LENGTH +1));
            MMIAPICONNECTION_IpNumToStr(ip_address, (uint8 *)ip_string);            
            GUIEDIT_GetString(VP_AGENT_PORT_INPUT_CTRL_ID,&port_string);
            SCI_MEMSET(port_str,0,(VP_MAX_PORT_LENGTH +1));
            MMIAPICOM_WstrToStr(port_string.wstr_ptr, (uint8*)port_str);//只允许输入数字，可以用这个函数
            port = atoi((char*)port_str);
            
        }
        else
        {
            is_use_agent = FALSE;
        }

        if(is_use_agent && (VP_MAX_PORT < port || 0 == port_string.wstr_len))
        {
            MMIPUB_OpenAlertWarningWin(TXT_VP_AGENT_PORT_INVALID); 
           
        }
        else
        {
            VP_Setting_SetAgent(is_use_agent,(uint8 *)ip_string,VP_MAX_IP_LENGTH,(uint16)port);
		    //xin.li_clean_popwin            
			//MMIPUB_OpenAlertSuccessWin(TXT_COMM_SAVE_SUCCESS);
            MMK_CloseWin(win_id);     
        }
        break;        

   
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
    
}
#endif
/****************************************************************************/    
//  Description :add item to richtext
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void RichText_AddItem(MMI_CTRL_ID_T ctrl_id, 
                            MMI_STRING_T * item_string, 
                            GUIRICHTEXT_FRAME_TYPE_E frame_type, 
                            GUIRICHTEXT_TAG_TYPE_E tag_type)
{
    GUIRICHTEXT_ITEM_T  item_data = {0};/*lint !e64*/
    uint16 index = 0;

    if (PNULL == item_string)
    {
        //VP_STRACE_LOW:"[VP] RichText_AddItem param is error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_9142_112_2_18_3_5_47_484,(uint8*)"");
        return;
    }
   

    item_data.img_type = GUIRICHTEXT_IMAGE_NONE;
    item_data.text_type = GUIRICHTEXT_TEXT_BUF;
    item_data.text_data.buf.len = item_string->wstr_len;
    item_data.text_data.buf.str_ptr = item_string->wstr_ptr;
    item_data.frame_type = frame_type;
    item_data.tag_type = tag_type;
    
    GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);
}

#ifdef MMI_STREAMING_PLAYER_SUPPORT
/****************************************************************************/    
//  Description : SetInputAuthDetailEntry
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void SetInputAuthDetailEntry(MMI_CTRL_ID_T ctrl_id)
{      
    wchar                       star_value[]= {'.'};
    BOOLEAN                     is_disp_im_icon = TRUE;
    BOOLEAN                     is_disp_num_info = TRUE;

    GUIFORM_SetStyle(VP_AUTH_USERNAME_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
    GUIFORM_SetStyle(VP_AUTH_PASSWORD_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);

    GUILABEL_SetTextById(VP_AUTH_USERNAME_LABEL_CTRL_ID,TXT_COMMON_USERNAME,FALSE);   
    GUILABEL_SetTextById(VP_AUTH_PASSWORD_LABEL_CTRL_ID,TXT_COMMON_PASSWORD,FALSE);    
    GUIEDIT_SetIm(VP_AUTH_PASSWORD_INPUT_CTRL_ID, GUIIM_TYPE_ABC | GUIIM_TYPE_DIGITAL,GUIIM_TYPE_ABC);      /*lint !e655*/
    GUIEDIT_ConfigImCustomKey(VP_AUTH_PASSWORD_INPUT_CTRL_ID,GUIIM_TYPE_DIGITAL,GUIIM_CUSTOM_KEY_STAR,star_value,1);
    GUIEDIT_SetDispImIcon(VP_AUTH_PASSWORD_INPUT_CTRL_ID,&is_disp_im_icon,&is_disp_num_info);
}


/*****************************************************************************/
//  Description :HandleAuthWinMsg
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleAuthWinMsg(MMI_WIN_ID_T win_id, 
                                    MMI_MESSAGE_ID_E msg_id, 
                                    DPARAM param)
{
    MMI_RESULT_E                recode          = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T               ctrl_id = VP_AUTH_INPUT_FORM_CTRL_ID;
    VP_AUTH_INFO_T              auth_info = {0};
    MMI_STRING_T                edit_string = {0};
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        //GUIWIN_SetStbBgImageId(win_id,IMAGE_COMMON_BG);
        SetInputAuthDetailEntry(ctrl_id);
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;       
   
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:  
        auth_info.password_ptr = PNULL;
        auth_info.username_ptr = PNULL;
        VP_AuthRsp(s_video_player_control_info.vp_handle,&auth_info);
        MMK_CloseWin(win_id);     
        break; 
      
    case MSG_FULL_PAINT:
    #ifdef MMI_PDA_SUPPORT//andy.he_cr120378      
    {
        if(GUIEDIT_IsImOpen(VP_AUTH_USERNAME_INPUT_CTRL_ID) || GUIEDIT_IsImOpen(VP_AUTH_PASSWORD_INPUT_CTRL_ID))
        {
            GUIFORM_SetChildDisplay(VP_AUTH_INPUT_FORM_CTRL_ID,MMICOMMON_SOFTKEY_CTRL_ID,GUIFORM_CHILD_DISP_HIDE);
        }
        else
        {
            GUIFORM_SetChildDisplay(VP_AUTH_INPUT_FORM_CTRL_ID,MMICOMMON_SOFTKEY_CTRL_ID,GUIFORM_CHILD_DISP_NORMAL);        
        } 
    }
    #endif  
        break;       
    case MSG_APP_OK:
    case MSG_CTL_OK: 
     case MSG_CTL_MIDSK:          
    case MSG_APP_WEB:
#ifndef MMI_PDA_SUPPORT    
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#endif        
        GUIEDIT_GetString(VP_AUTH_USERNAME_INPUT_CTRL_ID,&edit_string);
        if (0 != edit_string.wstr_len && PNULL != edit_string.wstr_ptr)
        {
            auth_info.username_ptr = (char *)SCI_ALLOCA(edit_string.wstr_len +1);
            if (PNULL != auth_info.username_ptr)
            {
                SCI_MEMSET(auth_info.username_ptr,0,(edit_string.wstr_len +1));
                MMIAPICOM_WstrToStr(edit_string.wstr_ptr,((uint8 *)(auth_info.username_ptr)));
            }
        }
        GUIEDIT_GetString(VP_AUTH_PASSWORD_INPUT_CTRL_ID,&edit_string);
        if (0 != edit_string.wstr_len && PNULL != edit_string.wstr_ptr)
        {
            auth_info.password_ptr = (char *)SCI_ALLOCA(edit_string.wstr_len +1);
            if (PNULL != auth_info.password_ptr)
            {
                SCI_MEMSET(auth_info.password_ptr,0,(edit_string.wstr_len +1));
                MMIAPICOM_WstrToStr(edit_string.wstr_ptr,((uint8 *)(auth_info.password_ptr)));
            }
        }
        
        VP_AuthRsp(s_video_player_control_info.vp_handle,&auth_info);
        if (PNULL != auth_info.username_ptr)
        {
            SCI_FREE(auth_info.username_ptr);
        }
        if (PNULL != auth_info.password_ptr)
        {
            SCI_FREE(auth_info.password_ptr);
        }
        MMK_CloseWin(win_id);     
        break;       

#ifdef MMI_PDA_SUPPORT
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
    ctrl_id= ((MMI_NOTIFY_T*) param)->src_id;
    switch(ctrl_id)
    {
    case MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID:
        MMK_SendMsg(win_id, MSG_APP_OK,PNULL);
        break;
    case MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID:
        MMK_SendMsg(win_id, MSG_APP_CANCEL,PNULL);
        break;
   default:
        break;
    }
    break;
#endif   
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
       default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}
#endif
/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author: apple.zhang
//  Note:
/*****************************************************************************/
LOCAL void InitListData(
                          MMI_CTRL_ID_T          ctrl_id,
                          uint32          total_num)
{
    uint32                               i = 0;
    GUILIST_ITEM_T                      item = {0};
    uint32  min_totalnum=0;
        
    GUILIST_SetTitleIndexType( ctrl_id, GUILIST_TITLE_INDEX_DEFAULT );
//    GUILIST_SetMaxItem(ctrl_id, VP_MAX_LOCALLIST_ITEMNUM, TRUE);   
    item.item_style = GUIITEM_STYLE_ONE_LINE_ICON_TEXT; 

    min_totalnum = (total_num < VP_MAX_LOCALLIST_ITEMNUM) ?  total_num:(VP_MAX_LOCALLIST_ITEMNUM);
    GUILIST_SetMaxItem(ctrl_id, min_totalnum, TRUE);  
   
    // initialize the ListBox, delet all items here
    for ( i = 0; i < min_totalnum; i++ )
    {
        GUILIST_AppendItem( ctrl_id, &item );
    }

    GUILIST_SetCurItemIndex( ctrl_id, 0 );
}

/*****************************************************************************/
//  Description : handle list append data
//  Global resource dependence : 
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL void HandleListAppendData(
                                   MMI_WIN_ID_T           win_id,    
                                   DPARAM param,
                                   MMI_HANDLE_T ctrl_handle,
                                   VP_LIST_ITEM_TYPE_E type,
                                   uint16 item_content_len,
                                   wchar *item_content_ptr,
                                   MMI_IMAGE_ID_T image_id
                                   )
{
    GUILIST_NEED_ITEM_DATA_T *need_item_data_ptr = PNULL;
    GUILIST_NEED_ITEM_CONTENT_T *need_item_content_ptr = PNULL;
    GUILIST_ITEM_T      item_t = {0};
    GUILIST_ITEM_DATA_T  item_data = {0};
    uint16              index = 0;
    
    
    //VP_STRACE_LOW:"[VP] HandleListAppendData type = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_9337_112_2_18_3_5_48_485,(uint8*)"d",type);
    item_t.item_style = GUIITEM_STYLE_ONE_LINE_TEXT;
    if (VP_NEED_LIST_ITEM_DATA_TYPE == type)
    {
        need_item_data_ptr = (GUILIST_NEED_ITEM_DATA_T*)param;
        if (PNULL == need_item_data_ptr)
        {
            //VP_STRACE_LOW:"[VP] HandleListAppendData need_item_data_ptr is null"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_9344_112_2_18_3_5_48_486,(uint8*)"");
            MMK_CloseWin(win_id);
            return;
        }
        index = need_item_data_ptr->item_index;
       
    }
    else if (VP_NEED_LIST_ITEM_CONTENT_TYPE == type)
    {
        need_item_content_ptr = (GUILIST_NEED_ITEM_CONTENT_T*)param;
        if (PNULL == need_item_content_ptr)
        {
            //VP_STRACE_LOW:"[VP] HandleListAppendData need_item_content_ptr is null"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_9356_112_2_18_3_5_48_487,(uint8*)"");
            MMK_CloseWin(win_id);
            return;
        }
        index = need_item_content_ptr->item_index;   
        
    }    
  
    item_t.item_style = GUIITEM_STYLE_ONE_LINE_ICON_TEXT;
    item_data.item_content[0].item_data.image_id = image_id;    

    if( type == VP_APPEND_LIST_ITEM_TYPE )
    {
        GUILIST_AppendItem( ctrl_handle, &item_t );
    }
    else if( type == VP_REPLACE_LIST_ITEM_TYPE )
    {
        GUILIST_ReplaceItem( ctrl_handle, &item_t, index );
    }
    else
    {
        item_t.item_data_ptr = &item_data;
#ifdef MMI_VIDEO_PLAYER_MINI_SUPPORT //6530_MINI
        item_data.softkey_id[0] = TXT_COMMON_PLAY;
        item_data.softkey_id[1] = TXT_NULL;
#else
        item_data.softkey_id[0] = STXT_OPTION;
        item_data.softkey_id[1] = TXT_COMMON_PLAY;
#endif
        item_data.softkey_id[2] = STXT_RETURN;       
         /* text 1 */
        item_data.item_content[1].item_data_type = GUIITEM_DATA_TEXT_BUFFER;        
        item_data.item_content[1].item_data.text_buffer.wstr_len = item_content_len;
        item_data.item_content[1].item_data.text_buffer.wstr_ptr = item_content_ptr;        

        if( type == VP_NEED_LIST_ITEM_DATA_TYPE )
        {
            item_data.item_content[0].item_data_type = GUIITEM_DATA_IMAGE_ID;
            GUILIST_SetItemData( ctrl_handle, &item_data, index );  
           
        }
        else if( type == VP_NEED_LIST_ITEM_CONTENT_TYPE )
        {
            GUILIST_SetItemContent( ctrl_handle, &item_data.item_content[0], index, 0 );
        }
    }
     
    
}
#ifdef MMI_STREAMING_PLAYER_SUPPORT
/*****************************************************************************/
//  Description : 获取书签数据
//  Global resource dependence :
//  Author: apple.zhang
//  Note:
/*****************************************************************************/
LOCAL void GetBookmarkListItemContent(uint16 bookmark_list_index,
                                      wchar *item_content_ptr,
                                      uint16 *item_content_len_ptr,
                                      MMI_IMAGE_ID_T *image_id_ptr)
{ 
    const VP_NET_MOVIE_INFO_T* bookmar_item_ptr = PNULL; 
    uint32                   bookmark_num = VP_BookmarkList_GetNum();
    wchar                    *title_ptr = PNULL;

    if (PNULL == item_content_len_ptr || PNULL == item_content_ptr || PNULL == image_id_ptr)
    {
        //VP_STRACE_LOW:"[VP] GetBookmarkListItemContent param error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_9416_112_2_18_3_5_48_488,(uint8*)"");
        return;
    }
    bookmar_item_ptr = VP_BookmarkList_GetInfo(bookmark_num - 1 - bookmark_list_index);    
    if (PNULL != bookmar_item_ptr)
    {
        *image_id_ptr = IMAGE_VP_NET_LIST_ICON;    
        title_ptr = (wchar *)(bookmar_item_ptr->title_arr);
        item_content_ptr = MMIAPICOM_Wstrncpy(item_content_ptr, title_ptr,bookmar_item_ptr->title_len);       
        *item_content_len_ptr = (uint16)(bookmar_item_ptr->title_len);
    }
   
}
#endif
/*****************************************************************************/
//  Description : 更新列表数据
//  Global resource dependence :
//  Author: apple.zhang
//  Note:
/*****************************************************************************/
LOCAL void UpdateListData(
                          MMI_WIN_ID_T           win_id,     
                          MMI_CTRL_ID_T          ctrl_id,
                          uint32                 total_num,
                          GetListItemContent     getlistitemconten
                          )
{
    uint16                  max_num = 0;
    uint32                   i = 0; 
    wchar                   item_content[VP_MAX_NAME_LEN + 1] = {0};
    uint16                  item_content_len = 0;
    MMI_IMAGE_ID_T          image_id = IMAGE_NULL;
    uint32 min_totalnum =0 ;
    
    max_num = GUILIST_GetTotalItemNum(ctrl_id);
    if (max_num > 0)
    {       
        GUILIST_RemoveAllItems(ctrl_id);
    }  
    //VP_STRACE_LOW:"[VP] UpdateListData total_num = %d, max_num = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_9452_112_2_18_3_5_48_489,(uint8*)"dd",total_num,max_num);
    if (0 == total_num)
    {
        return;
    }    
    GUILIST_SetTitleIndexType( ctrl_id, GUILIST_TITLE_INDEX_DEFAULT );
  
//    GUILIST_SetMaxItem(ctrl_id, VP_MAX_LOCALLIST_ITEMNUM, TRUE);  

    min_totalnum = (total_num < VP_MAX_LOCALLIST_ITEMNUM) ?  total_num:(VP_MAX_LOCALLIST_ITEMNUM);
    GUILIST_SetMaxItem(ctrl_id, min_totalnum, TRUE);  
    
    // initialize the ListBox, delet all items here
    for ( i = 0; i < min_totalnum; i++ )
    {
        SCI_MEMSET(item_content,0,((VP_MAX_NAME_LEN + 1)*sizeof(wchar)));
        getlistitemconten(i,item_content,&item_content_len,&image_id);
        HandleListAppendData(win_id,PNULL,ctrl_id, VP_APPEND_LIST_ITEM_TYPE,item_content_len, item_content,image_id);    
    }     
   
}

/*****************************************************************************/
//  Description :OpenBrowerWin
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void OpenBrowerWin(uint8 *url_ptr)
{
#ifdef BROWSER_SUPPORT    
    MMIBROWSER_ENTRY_PARAM entry_param = {0};

    if (PNULL == url_ptr)
    {
        return;
    }

    entry_param.type = MMIBROWSER_ACCESS_URL;
#ifndef MMI_MULTI_SIM_SYS_SINGLE
    entry_param.dual_sys = MMI_DUAL_SYS_MAX;     
#else
    entry_param.dual_sys = MN_DUAL_SYS_1;     
#endif        
    entry_param.url_ptr = (char *)url_ptr;
    entry_param.user_agent_ptr = PNULL;

    MMIAPIBROWSER_Entry(&entry_param);      
#endif

}
#ifdef MMI_STREAMING_PLAYER_SUPPORT
/*****************************************************************************/
//  Description :HandleBookmarkListWinMsg
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleBookmarkListWinMsg(MMI_WIN_ID_T win_id, 
                                    MMI_MESSAGE_ID_E msg_id, 
                                    DPARAM param)
{
    MMI_RESULT_E            result=MMI_RESULT_TRUE;
    MMI_CTRL_ID_T           list_ctrl_id = VP_BOOKMARK_LIST_CTRL_ID;
    uint32                  list_num = 0;   
    uint16                  bookmark_list_index = 0;
    MMI_IMAGE_ID_T          image_id = IMAGE_NULL;
    wchar                   item_content[VP_MAX_NAME_LEN + 1] = {0};
    uint16                  item_content_len = 0;
    GUILIST_NEED_ITEM_DATA_T*need_item_data_ptr = PNULL;
    GUILIST_NEED_ITEM_CONTENT_T*need_item_content_ptr = PNULL;
    const VP_NET_MOVIE_INFO_T       *bookmark_item_ptr = PNULL;
    
    switch(msg_id)
    {
     case MSG_OPEN_WINDOW:
         list_num = VP_BookmarkList_GetNum();
         InitListData(list_ctrl_id,list_num);     
         if (0 == list_num)
         {
             GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_NULL, STXT_RETURN, FALSE); 
         }
         else
         {
             GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_COMMON_PLAY, STXT_RETURN, FALSE); 
         }
#ifdef MMI_PDA_SUPPORT
        GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
#endif
         MMK_SetAtvCtrl(win_id,list_ctrl_id);   
         break;       
     

     case MSG_CTL_LIST_NEED_ITEM_DATA:        
        need_item_data_ptr = (GUILIST_NEED_ITEM_DATA_T*)param;
        if (PNULL == need_item_data_ptr)
        {
            //VP_STRACE_LOW:"[VP] HandleListAppendData need_item_data_ptr is null"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_9541_112_2_18_3_5_48_490,(uint8*)"");
            MMK_CloseWin(win_id);
            break;
        }
        bookmark_list_index = need_item_data_ptr->item_index;   
        GetBookmarkListItemContent(bookmark_list_index,item_content,&item_content_len,&image_id);
        HandleListAppendData(win_id,param,list_ctrl_id,VP_NEED_LIST_ITEM_DATA_TYPE,item_content_len,item_content,image_id);
        break;
        
   case MSG_CTL_LIST_NEED_ITEM_CONTENT:      
       need_item_content_ptr = (GUILIST_NEED_ITEM_CONTENT_T*)param;
        if (PNULL == need_item_content_ptr)
        {
            //VP_STRACE_LOW:"[VP] HandleListAppendData need_item_content_ptr is null"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_9554_112_2_18_3_5_49_491,(uint8*)"");
            MMK_CloseWin(win_id);
            break;
        }
        bookmark_list_index = need_item_content_ptr->item_index;  
        GetBookmarkListItemContent(bookmark_list_index,item_content,&item_content_len,&image_id);
        HandleListAppendData(win_id,param,list_ctrl_id,VP_NEED_LIST_ITEM_CONTENT_TYPE,item_content_len,item_content,image_id);
        break;
 

    case MSG_GET_FOCUS:        
        if (0 != VP_BookmarkList_GetNum())        
        { 
            GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_COMMON_PLAY, STXT_RETURN, FALSE);        
        }
        else
        {
            GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_NULL, STXT_RETURN, FALSE); 
        }
        break;
        

    case MSG_APP_MENU:
    case MSG_CTL_OK:
    case MSG_APP_OK:        
        MMK_CreateWin((uint32*)MMIVP_BOOKMARK_POPMENU_WIN_TAB,PNULL);         
        break;
        
#ifdef MMI_PDA_SUPPORT
#ifndef MMI_GUI_STYLE_TYPICAL
    case MSG_CTL_LIST_LONGOK:
        //长按List弹出context menu菜单
        MMK_CreateWin((uint32*)MMIVP_BOOKMARK_LONGOK_POPMENU_WIN_TAB,PNULL);
        break;
#endif
#endif
        
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        list_num = VP_BookmarkList_GetNum();
        bookmark_list_index = GUILIST_GetCurItemIndex(list_ctrl_id);
        //VP_STRACE_LOW:"[VP] HandleBookmarkListWinMsg list_num = %d, list_index = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_9594_112_2_18_3_5_49_492,(uint8*)"dd",list_num,bookmark_list_index );
        bookmark_item_ptr = VP_BookmarkList_GetInfo(list_num - 1 - bookmark_list_index);        
        if (PNULL != bookmark_item_ptr && 0 == strnicmp(bookmark_item_ptr->url_ptr,HTTP_URL_HEAD_ASIC,HTTP_URL_HEAD_LEN))
        {            
            OpenBrowerWin((uint8 *)(bookmark_item_ptr->url_ptr)); 
            MMK_CloseWin(win_id);
        }
        else if (PNULL != bookmark_item_ptr && 0 == strnicmp(bookmark_item_ptr->url_ptr,RTSP_URL_HEAD_ASIC,RTSP_URL_HEAD_LEN))
        {                 
            //VP_STRACE_LOW:"[VP] HandleBookmarkListWinMsg bookmark_item_ptr->title_arr = %x"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_9603_112_2_18_3_5_49_493,(uint8*)"d",*(bookmark_item_ptr->title_arr));
           
            NotifyPlayerForPlayNet((wchar *)(bookmark_item_ptr->title_arr),bookmark_item_ptr->title_len,(uint8 *)(bookmark_item_ptr->url_ptr),0,0);
            MMK_CloseWin(win_id);  
            //MMITHEME_SetUpdateDelayCount(0);    
        }
        else
        {
            MMK_CreateWin((uint32*)MMIVP_BOOKMARK_POPMENU_WIN_TAB,PNULL);   
        }
        break;

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return result;
}
#endif
/*****************************************************************************/
//  Description : 获取历史记录数据
//  Global resource dependence :
//  Author: apple.zhang
//  Note:
/*****************************************************************************/
LOCAL void GetHistoryListItemContent(uint16 historylist_index,
                                      wchar *item_content_ptr,
                                      uint16 *item_content_len_ptr,
                                      MMI_IMAGE_ID_T *image_id_ptr)
{ 
#ifdef MMI_VIDEOPLAYER_HISTORYLIST_SUPPORT
    const VP_HISTORY_ITEM_T      *history_info_ptr = PNULL;    
    wchar                  device_ptr[MMIFILE_DEVICE_LEN + 1] = {0};
    uint16                  device_len   = MMIFILE_DEVICE_LEN;
#ifdef DRM_SUPPORT
    MMIFMM_FILE_TYPE_E   file_type = MMIFMM_FILE_TYPE_NORMAL;
#endif

    if (PNULL == item_content_ptr || PNULL == item_content_len_ptr || PNULL == image_id_ptr)
    {
        //VP_STRACE_LOW:"[VP] GetHistoryListItemContent param is error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_9646_112_2_18_3_5_49_494,(uint8*)"");
        return;
    }
    history_info_ptr = VP_HistoryList_GetInfo(historylist_index);
    if (PNULL != history_info_ptr)
    {
 #ifdef MMI_STREAMING_PLAYER_SUPPORT  
        if(VP_PLAY_FROM_NET == history_info_ptr->type && PNULL != history_info_ptr->net_movie_info_ptr)
        {
            *image_id_ptr = IMAGE_VP_NET_LIST_ICON;
            *item_content_len_ptr = history_info_ptr->net_movie_info_ptr->title_len;
            item_content_ptr = MMIAPICOM_Wstrncpy(item_content_ptr,history_info_ptr->net_movie_info_ptr->title_arr,history_info_ptr->net_movie_info_ptr->title_len);       
        }
        else if(PNULL != history_info_ptr->local_movie_file_info_ptr)
#else
            if(PNULL != history_info_ptr->local_movie_file_info_ptr)
#endif
        {

            SCI_MEMSET(device_ptr,0,((MMIFILE_DEVICE_LEN + 1)*sizeof(wchar)));
            MMIAPIFMM_SplitFullPath(
                            history_info_ptr->local_movie_file_info_ptr->full_path_ptr, 
                            history_info_ptr->local_movie_file_info_ptr->full_path_len, 
                            device_ptr, &device_len, PNULL, PNULL, 
                            item_content_ptr, item_content_len_ptr);
#ifdef DRM_SUPPORT
            file_type = MMIAPIFMM_GetFileTypeByFileName(history_info_ptr->local_movie_file_info_ptr->full_path_ptr,
                history_info_ptr->local_movie_file_info_ptr->full_path_len);
            if(MMIFMM_FILE_TYPE_DRM == file_type)
            {
                *image_id_ptr = MMIAPIFMM_GetDrmIconId(history_info_ptr->local_movie_file_info_ptr->full_path_ptr);
            }
            else
#endif
            if(MMI_DEVICE_UDISK < MMIAPIFMM_GetDeviceTypeByPath(device_ptr,device_len))
            {
                *image_id_ptr = IMAGE_COMMON_MOVIE_SD_ICON;    
            }
            else
            {
                *image_id_ptr = IMAGE_COMMON_MOVIE_UDISK_ICON;
            }
         
       
        }
    }
#endif
}

#ifdef MMI_VIDEOPLAYER_HISTORYLIST_SUPPORT
/*****************************************************************************/
//  Description :PlayCurrentHistoryVIdeo
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN PlayCurrentHistoryVIdeo(MMI_CTRL_ID_T list_ctrl_id, BOOLEAN is_go_on, BOOLEAN *is_query_ptr)
{
    const VP_HISTORY_ITEM_T           *history_info_ptr = PNULL;
    uint16                      historylist_index = 0;
    uint32                      process_time = 0;

    historylist_index = GUILIST_GetCurItemIndex(list_ctrl_id);
    history_info_ptr = VP_HistoryList_GetInfo(historylist_index);  
    //VP_STRACE_LOW:"[VP] PlayCurrentHistoryVIdeo history_info_ptr = %x,  is_query_ptr = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_9704_112_2_18_3_5_49_495,(uint8*)"dd",history_info_ptr,is_query_ptr);
    if (PNULL != history_info_ptr)
    { 
        //VP_STRACE_LOW:"[VP] PlayCurrentHistoryVIdeo history_info_ptr->last_play_point = %d, history_info_ptr->total_time = %x, history_info_ptr->type = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_9710_112_2_18_3_5_49_496,(uint8*)"ddd",history_info_ptr->last_play_point,history_info_ptr->total_time,history_info_ptr->type);
        
        if (PNULL != is_query_ptr 
            && 0 != history_info_ptr->last_play_point 
            && VP_MAX_INVALID_VALUE != s_video_player_control_info.total_time 
            && VP_PLAY_FROM_NET == history_info_ptr->type)
        {
            *is_query_ptr = TRUE;
            return TRUE;
        }
        if (is_go_on)
        {
            process_time = history_info_ptr->last_play_point;
        }
        else
        {
            process_time = 0;
        }
        
        //VP_STRACE_LOW:"[VP] PlayCurrentHistoryVIdeo history_info_ptr->type = %d, process_time = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_9729_112_2_18_3_5_49_497,(uint8*)"dd",history_info_ptr->type,process_time);
 #ifdef MMI_STREAMING_PLAYER_SUPPORT      
		if (VP_PLAY_FROM_NET == history_info_ptr->type)
        {
            if (PNULL == history_info_ptr->net_movie_info_ptr)
            {
                //VP_STRACE_LOW:"[VP] PlayCurrentHistoryVIdeo net info pnull"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_9735_112_2_18_3_5_49_498,(uint8*)"");
                return FALSE;
            }
            NotifyPlayerForPlayNet(history_info_ptr->net_movie_info_ptr->title_arr,
                history_info_ptr->net_movie_info_ptr->title_len,
                (uint8 *)(history_info_ptr->net_movie_info_ptr->url_ptr),
                process_time,
                history_info_ptr->total_time);
        }
        else
#endif
        {
            if (PNULL == history_info_ptr->local_movie_file_info_ptr)
            {
                return FALSE;
            }            
            NotifyPlayerForPlayFile(history_info_ptr->local_movie_file_info_ptr->full_path_ptr,
                history_info_ptr->local_movie_file_info_ptr->full_path_len,
                process_time,
                history_info_ptr->total_time,
                VP_PLAY_FROM_FILE);
        }
        return TRUE;        
    }
    else
    {
        return FALSE;
    }


}
#endif


#ifdef MMI_VIDEOPLAYER_HISTORYLIST_SUPPORT
#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : Del all History
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDelAllHistoryWinMsg(
                                             MMI_WIN_ID_T win_id,
                                             MMI_MESSAGE_ID_E msg_id,
                                             DPARAM param
                                             )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    
    switch (msg_id)
    {
    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_CTL_OK:
        {
            BOOLEAN delete_result = TRUE;
            uint16 cur_historylist_index = 0;

            MMIPUB_CloseQuerytWin(PNULL);

            delete_result = VP_HistoryList_DelAllItem();
            cur_historylist_index = 0;

            UpdateListData(MMIVP_HISTORYLIST_WIN_ID, VP_HISTORY_LIST_CTRL_ID, VP_HistoryList_GetNum(), GetHistoryListItemContent);
            GUILIST_SetCurItemIndex(VP_HISTORY_LIST_CTRL_ID, cur_historylist_index);
            if (delete_result)
            {
                MMIPUB_OpenAlertSuccessWin(TXT_DELETED);
            }
            else
            {
                MMIPUB_OpenAlertFailWin(TXT_COMMON_DEL_FAIL);
            }
        }
        MMK_CloseWin(win_id);
        break;

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin( win_id );
        break;
    
    default:
        result = MMIPUB_HandleQueryWinMsg(win_id, msg_id, param );
        break;
    }

    return (result);
}
#endif
/*****************************************************************************/
//  Description :HandleHistoryListWinMsg
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleHistoryListWinMsg(MMI_WIN_ID_T win_id, 
                                    MMI_MESSAGE_ID_E msg_id, 
                                    DPARAM param)
{
    MMI_RESULT_E                result=MMI_RESULT_TRUE;
    MMI_CTRL_ID_T               list_ctrl_id = VP_HISTORY_LIST_CTRL_ID;
    uint32                      list_num = 0;   
    uint16                      historylist_index = 0;
    MMI_IMAGE_ID_T              image_id = IMAGE_NULL;
    wchar                       item_content[VP_MAX_NAME_LEN + 1] = {0};
    uint16                      item_content_len = 0;
    GUILIST_NEED_ITEM_DATA_T    *need_item_data_ptr = PNULL;
    GUILIST_NEED_ITEM_CONTENT_T *need_item_content_ptr = PNULL;    
    BOOLEAN                     is_query = FALSE;
    
    switch(msg_id) 
    {
     case MSG_OPEN_WINDOW:
         list_num = VP_HistoryList_GetNum();
         InitListData(list_ctrl_id,list_num); 
         if (0 == list_num)
         {
             GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_NULL, STXT_RETURN, FALSE); 
         }
         else
         {
             GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_COMMON_PLAY, STXT_RETURN, FALSE); 
         }         
#ifdef MMI_PDA_SUPPORT
        if (0 < VP_HistoryList_GetNum())
        {
            GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
        }
        GUIWIN_SetTitleButtonTextId(win_id, GUITITLE_BUTTON_RIGHT, TXT_DELALL, FALSE);
        GUIWIN_SetTitleButtonHandleMsgInfo(win_id, GUITITLE_BUTTON_RIGHT, MSG_NOTIFY_TITLE_MENU_ICON, list_ctrl_id);
#endif
         MMK_SetAtvCtrl(win_id,list_ctrl_id);  
         break;        

     case MSG_CTL_LIST_NEED_ITEM_DATA:        
        need_item_data_ptr = (GUILIST_NEED_ITEM_DATA_T*)param;
        if (PNULL == need_item_data_ptr)
        {
            //VP_STRACE_LOW:"[VP] HandleHistoryListWinMsg need_item_data_ptr is null"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_9871_112_2_18_3_5_49_499,(uint8*)"");
            MMK_CloseWin(win_id);
            break;
        }
        historylist_index = need_item_data_ptr->item_index;   
        GetHistoryListItemContent(historylist_index,item_content,&item_content_len,&image_id);
        HandleListAppendData(win_id,param,list_ctrl_id,VP_NEED_LIST_ITEM_DATA_TYPE,item_content_len,item_content,image_id);
        break;
        
   case MSG_CTL_LIST_NEED_ITEM_CONTENT:      
       need_item_content_ptr = (GUILIST_NEED_ITEM_CONTENT_T*)param;
        if (PNULL == need_item_content_ptr)
        {
            //VP_STRACE_LOW:"[VP] HandleHistoryListWinMsg need_item_content_ptr is null"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_9884_112_2_18_3_5_49_500,(uint8*)"");
            MMK_CloseWin(win_id);
            break;
        }
        historylist_index = need_item_content_ptr->item_index;  
        GetHistoryListItemContent(historylist_index,item_content,&item_content_len,&image_id);
        HandleListAppendData(win_id,param,list_ctrl_id,VP_NEED_LIST_ITEM_CONTENT_TYPE,item_content_len,item_content,image_id);
        break;

   case MSG_GET_FOCUS:
       if (0 != VP_HistoryList_GetNum())        
        { 
            GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_COMMON_PLAY, STXT_RETURN, FALSE);        
        }
        else
        {
            GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_NULL, STXT_RETURN, FALSE); 
        }

#ifdef MMI_PDA_SUPPORT
        if (0 < VP_HistoryList_GetNum())
        {
            GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
        }
        else
        {
            GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_INVISIBLE, FALSE);
        }
#endif
        break;
       
   case MSG_APP_MENU:
    case MSG_CTL_OK:
    case MSG_APP_OK:       
        MMK_CreateWin((uint32*)MMIVP_HISTORY_POPMENU_WIN_TAB,PNULL);          
        break;
        
#ifdef MMI_PDA_SUPPORT
    case MSG_NOTIFY_TITLE_MENU_ICON:
        MMIPUB_CloseQuerytWin(PNULL);
        MMIPUB_OpenQueryWinByTextId(TXT_DELALL, IMAGE_PUBWIN_QUERY, PNULL, HandleDelAllHistoryWinMsg);
        break;
    case MSG_CTL_LIST_LONGOK:
        //长按List弹出context menu菜单
        MMK_CreateWin((uint32*)MMIVP_HISTORY_LONGOK_POPMENU_WIN_TAB,PNULL);
        break;
#endif
   
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK: 
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        if (0 != VP_HistoryList_GetNum())  
        {        
            is_query = FALSE;
            PlayCurrentHistoryVIdeo(list_ctrl_id,FALSE,&is_query);
            if (is_query)
            {
                MMIPUB_OpenQueryWinByTextId(TXT_VP_IS_CONTINUE,IMAGE_PUBWIN_QUERY,PNULL,PNULL);
            }
            else
            {
                MMK_CloseWin(win_id);
            }
        }
        else
        {
            MMK_CreateWin((uint32*)MMIVP_HISTORY_POPMENU_WIN_TAB,PNULL); 
        }
        break;

    case MSG_PROMPTWIN_OK:
        MMIPUB_CloseQuerytWin(PNULL);
        PlayCurrentHistoryVIdeo(list_ctrl_id,TRUE, PNULL); 
        MMK_CloseWin(win_id);
        break;

    case MSG_PROMPTWIN_CANCEL:
        MMIPUB_CloseQuerytWin(PNULL);
        PlayCurrentHistoryVIdeo(list_ctrl_id,FALSE, PNULL); 
        MMK_CloseWin(win_id);
        break;

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
    case MSG_CLOSE_WINDOW:
        break;
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return result;
}
#endif

#ifdef MMI_VIDEOPLAYER_SHORTCUTMENU_SUPPORT
/*****************************************************************************/
//  Description : CreateShortcutMenu
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
LOCAL void CreateShortcutMenu(MMI_CTRL_ID_T   ctrl_id)
{
    MMI_STRING_T            kstring = {0};
    GUIMENU_DYNA_ITEM_T     node_item = {0};
    uint32                  index = 0;  

#ifdef MMI_STREAMING_PLAYER_SUPPORT
#ifdef MMI_VIDEOPLAYER_HISTORYLIST_SUPPORT
    MMI_GetLabelTextByLang(TXT_VP_HISTORYLIST_ID, &kstring);
    node_item.item_text_ptr = &kstring;
    GUIMENU_InsertNode(index, ID_VP_VIEW_HISTORYLIST, 0, &node_item, ctrl_id);
    index++;    
#endif
#endif
#ifndef MMI_VIDEO_PLAYER_MINI_LAYOUT   
   if(IsDisplayFullScreenReferInfoEnable())
    {
        if (LCD_ANGLE_90 == s_video_player_control_info.cur_angle
            || LCD_ANGLE_270 == s_video_player_control_info.cur_angle)
        {
            MMI_GetLabelTextByLang(TXT_VP_HALFSCREEN_ID, &kstring);
        }
        else
        {
            MMI_GetLabelTextByLang(TXT_VP_FULLSCREEN_ID, &kstring);
        }
        node_item.item_text_ptr = &kstring;
        node_item.is_grayed = !IsSwitchFullScreenEnable();
        GUIMENU_InsertNode(index, ID_VP_FULLSCREEN, 0, &node_item, ctrl_id);
        index++;   
    }
#endif
    
#ifdef MMI_STREAMING_PLAYER_SUPPORT
#ifdef CMCC_UI_STYLE
    MMI_GetLabelTextByLang(TXT_COMMON_BOOKMARKS, &kstring);
#else
    MMI_GetLabelTextByLang(TXT_VP_BOOKMARK_ID, &kstring);
#endif
    node_item.item_text_ptr = &kstring;
    GUIMENU_InsertNode(index, ID_VP_VIEW_BOOKMARK, 0, &node_item, ctrl_id);
    index++;             
#endif    
    
    if (IsAdjustBrightEnable())
    {
        MMI_GetLabelTextByLang(TXT_VP_BRIGHTNESS_ID, &kstring);
        node_item.item_text_ptr = &kstring;
        GUIMENU_InsertNode(index, ID_VP_BRIGHTNESS, 0, &node_item, ctrl_id);
        index++;     
    }
    
#ifdef MMI_STREAMING_PLAYER_SUPPORT
    if (VP_PLAY_FROM_NET == s_video_player_control_info.play_type)
    {
        MMI_GetLabelTextByLang(TXT_VP_SAVE_BOOKMARK_ID, &kstring);
        node_item.item_text_ptr = &kstring;
        GUIMENU_InsertNode(index, ID_VP_SAVE_BOOKMAR, 0, &node_item, ctrl_id);
        index++;     
    }  
#endif
    if (MMISET_VOL_ZERO == MMIAPISET_GetMultimVolume())
    {
        MMI_GetLabelTextByLang(TXT_VP_CANCEL_MUTE_ID, &kstring);
    }
    else
    {
        MMI_GetLabelTextByLang(TXT_VP_MUTE_ID, &kstring);
    }    
    node_item.item_text_ptr = &kstring;
    GUIMENU_InsertNode(index, ID_VP_MUTE, 0, &node_item, ctrl_id);   
}

/*****************************************************************************/
//  Description : 视频播放器快捷选项消息处理.
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleShortcutMenuWinMsg(
                                     MMI_WIN_ID_T       win_id,     
                                     MMI_MESSAGE_ID_E    msg_id, 
                                     DPARAM              param
                                     )
{
    MMI_RESULT_E                result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T               ctrl_id = VP_SHOUTCUT_OPT_MENU_CTRL_ID;
    MMI_MENU_ID_T               menu_id = 0;
    

   

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
#ifdef MMI_VIDEO_PLAYER_MINI_LAYOUT
        GUIMENU_SetMenuBgColor(ctrl_id, MMI_WHITE_COLOR);
#if !((defined MAINLCD_DEV_SIZE_220X176) || (defined MAINLCD_DEV_SIZE_160X128)) //6530_MINI 横屏不转屏
         MMK_SetWinSupportAngle(MMIVP_SHOUTCUT_OPT_MENU_WIN_ID, WIN_SUPPORT_ANGLE_0);
         MMK_SetWinAngle(MMIVP_SHOUTCUT_OPT_MENU_WIN_ID, LCD_ANGLE_0, TRUE);
#endif
#endif //MMI_VIDEO_PLAYER_MINI_LAYOUT
        CreateShortcutMenu(ctrl_id);
#ifdef MMI_GUI_STYLE_MINISCREEN
        GUIMENU_SetState(ctrl_id,GUIMENU_STATE_CUSTOMER_TITLE,TRUE);
#endif
        MMK_SetAtvCtrl(win_id,ctrl_id);
        break;

    case MSG_CTL_MIDSK:  
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_WEB:
    case MSG_APP_OK:
        menu_id = GUIMENU_GetCurNodeId(ctrl_id);	
        //VP_STRACE_LOW:"[VP] HandleShortcutMenuWinMsg menu_id = %x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_10083_112_2_18_3_5_50_501,(uint8*)"d",menu_id);
        switch (menu_id)
        {  
#ifdef MMI_VIDEOPLAYER_HISTORYLIST_SUPPORT
        case ID_VP_VIEW_HISTORYLIST:
            MMK_CreateWin((uint32 *)MMIVP_HISTORYLIST_WIN_TAB,PNULL);
            MMK_CloseWin(win_id);
            break;
#endif

#ifdef MMI_STREAMING_PLAYER_SUPPORT
        case ID_VP_VIEW_BOOKMARK:
            MMK_CreateWin((uint32 *)MMIVP_BOOKMARKLIST_WIN_TAB,PNULL);
            MMK_CloseWin(win_id);
            break;    
#endif
        case ID_VP_FULLSCREEN:   
            MMK_CloseWin(win_id);
            MMK_PostMsg(MMIVP_PLAYER_WIN_ID,MSG_KEYDOWN_2,PNULL,PNULL); 
            break;

        case ID_VP_BRIGHTNESS:
            MMK_CloseWin(win_id);
			s_video_player_control_info.is_adjust_bright = TRUE;             
           // MMK_PostMsg(MMIVP_PLAYER_WIN_ID,MSG_VP_OPEN_BRIGHT_CONTROL_PANE,PNULL,PNULL);       
            break;
#ifdef MMI_STREAMING_PLAYER_SUPPORT
        case ID_VP_SAVE_BOOKMAR:
            MMK_CloseWin(win_id);
            SaveBookMark();            
            break;
#endif
        case ID_VP_MUTE:
            MMIAPIVP_FSMUpdateCurrentVol(s_video_player_control_info.vp_handle,MMIAPISET_MultimMuteSwitch());
            MMK_CloseWin(win_id);
            break;

        default:
            break;
        }
 
        
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}
#endif

#ifdef MMI_STREAMING_PLAYER_SUPPORT
/****************************************************************************/    
//  Description : SetNetViewDetailEntry
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void SetNetViewDetailEntry(MMI_CTRL_ID_T ctrl_id)
{  
    wchar   url_wchar[VP_MAX_URL_LEN + 1] = {0};
#ifndef MMI_PDA_SUPPORT
    wchar   star_value[]= {'.'};
#endif
    VP_NET_MOVIE_INFO_T         net_movie_info          = {0};
#ifdef MMI_PDA_SUPPORT
    MMI_STRING_T url_string={0};
#endif

    GUIFORM_SetStyle(VP_URL_INPUT_NAME_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
    GUIFORM_SetStyle(VP_CMCC_NAME_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);

    //input url
    GUILABEL_SetTextById(VP_URL_INPUT_NAME_LABEL_CTRL_ID,TXT_VP_INPUT_NET_URL,FALSE);

#ifdef MMI_PDA_SUPPORT 
    url_string.wstr_ptr=(wchar *)s_rtsp_url_head;
    url_string.wstr_len=MMIAPICOM_Wstrlen(s_rtsp_url_head);
    GUILABEL_SetText(VP_NET_URL_INPUT_CTRL_ID,&url_string,FALSE);
#else
    GUIEDIT_SetIm(VP_NET_URL_INPUT_CTRL_ID,(uint32)GUIIM_TYPE_ENGLISH | (uint32)GUIIM_TYPE_DIGITAL | (uint32)GUIIM_TYPE_ABC,GUIIM_TYPE_ABC); /*lint !e655*/
    //GUIEDIT_ConfigImCustomKey(VP_NET_URL_INPUT_CTRL_ID,GUIIM_TYPE_DIGITAL,GUIIM_CUSTOM_KEY_STAR,star_value,1);
    GUIEDIT_SetImTag(VP_NET_URL_INPUT_CTRL_ID, GUIIM_TAG_URL);

    GUIEDIT_SetString(VP_NET_URL_INPUT_CTRL_ID ,(wchar *)s_rtsp_url_head,RTSP_URL_HEAD_LEN);
#endif

    //cmcc url
    GUILABEL_SetTextById(VP_CMCC_NAME_LABEL_CTRL_ID,TXT_VP_CMMC_URL,FALSE);

    net_movie_info = VP_GetCMCCAddressInfo();
    SCI_MEMSET(url_wchar,0,((VP_MAX_URL_LEN+1)*sizeof(wchar)));
    MMIAPICOM_StrToWstr(((uint8 *)(net_movie_info.url_ptr)),(wchar *)url_wchar);
    
#ifdef MMI_PDA_SUPPORT
   url_string.wstr_ptr=url_wchar;
   url_string.wstr_len=strlen(net_movie_info.url_ptr);
   GUILABEL_SetText(VP_NET_CMCC_INPUT_CTRL_ID,&url_string,FALSE);
 #else
    GUITEXT_SetString(VP_NET_CMCC_INPUT_CTRL_ID,url_wchar,strlen(net_movie_info.url_ptr),FALSE); 
 #endif
 
}

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
// 	Description : handle message of input URL window of VP
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleVPInputNetUrlWinMsg(
                                             MMI_WIN_ID_T	    win_id, 
                                             MMI_MESSAGE_ID_E	msg_id, 
                                             DPARAM				param
                                             )
{
    MMI_RESULT_E	recode = MMI_RESULT_TRUE;
    MMI_STRING_T string_info = {0};
    MMI_CTRL_ID_T ctrl_id = VP_NET_EDIT_INPUT_URL_CTRL_ID;
    GUIIM_TYPE_T allow_im = GUIIM_TYPE_NONE;
    GUIIM_TYPE_T init_im = GUIIM_TYPE_NONE;
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        //set im
        allow_im = GUIIM_TYPE_ENGLISH | GUIIM_TYPE_ABC | GUIIM_TYPE_DIGITAL;/*lint !e655*/
        init_im = GUIIM_TYPE_ABC;
        GUIEDIT_SetIm(ctrl_id,allow_im,init_im);
        GUIEDIT_SetImTag(ctrl_id, GUIIM_TAG_URL);
        GUILABEL_GetText(VP_NET_URL_INPUT_CTRL_ID, &string_info);
        GUIEDIT_SetString(ctrl_id,(wchar *)(string_info.wstr_ptr),string_info.wstr_len);
        MMK_SetAtvCtrl(win_id,ctrl_id);
        break;
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
    case MSG_APP_OK:
    case MSG_CTL_OK:
        GUIEDIT_GetString(ctrl_id,&string_info);
        GUILABEL_SetText(VP_NET_URL_INPUT_CTRL_ID,&string_info,FALSE);
        MMK_CloseWin(win_id);
        NetLinkButton1Callback();//NEWMS00164399
        break;    
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);//返回上一级
        break;
        
    default:	
        recode = MMI_RESULT_FALSE;
        break;
    }
    return recode;
    
}
#endif


/*****************************************************************************/
//  Description :HandleNetWinMsg
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleNetWinMsg(MMI_WIN_ID_T win_id, 
                                    MMI_MESSAGE_ID_E msg_id, 
                                    DPARAM param)
{
    MMI_RESULT_E                recode              = MMI_RESULT_TRUE;    
    MMI_CTRL_ID_T               ctrl_id             = VP_NET_FORM_CTRL_ID;    
    MMI_STRING_T                string              = {0}; 
    uint8                       url_ptr[VP_MAX_URL_LEN + 1] = {0}; 
    VP_NET_MOVIE_INFO_T         net_movie_info          = {0};

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
#ifdef MID_SOFTKEY_USE_IMG_SUPPORT 
        GUIEDIT_SetSoftkeyEx(VP_NET_URL_INPUT_CTRL_ID, 
                                 1, 
                                 0,
                                 TXT_COMMON_CONNECT_COMN,
                                 TXT_COMMON_CONNECT_COMN,
                                 IMAGE_COMMON_SOFTKEY_CONNECT_ICON, 
                                 EDIT_BUTTON_IMAGE_ID, 
                                 PNULL
                             );
#endif
       // GUIWIN_SetStbBgImageId(win_id,IMAGE_COMMON_BG);
        SetNetViewDetailEntry(ctrl_id); 
#ifdef  MMI_PDA_SUPPORT       
        GUIFORM_SetType(VP_NET_FORM_CTRL_ID, GUIFORM_TYPE_TP);
#endif        
#ifdef MMI_PDA_SUPPORT
        {
            GUIFORM_CHILD_WIDTH_T button_width = {0};
            
            //set form pda style
            GUIFORM_SetType(VP_NET_FORM_CTRL_ID, GUIFORM_TYPE_TP);
            
            //set button callback
            GUIBUTTON_SetCallBackFunc(VP_URL_LINK_BUTTON1_CTRL_ID, NetLinkButton1Callback);
            GUIBUTTON_SetCallBackFunc(VP_URL_LINK_BUTTON2_CTRL_ID, NetLinkButton2Callback);
            
            //set layout percent
            button_width.type = GUIFORM_CHILD_WIDTH_PERCENT;
            button_width.add_data = 85;
            GUIFORM_SetChildWidth(VP_URL_INPUT_NAME_TITLE_FORM_CTRL_ID, VP_URL_INPUT_NAME_LABEL_CTRL_ID, &button_width);
            GUIFORM_SetChildWidth(VP_CMCC_NAME_TITLE_FORM_CTRL_ID,      VP_CMCC_NAME_LABEL_CTRL_ID, &button_width);
            GUILABEL_SetFont(VP_NET_CMCC_INPUT_CTRL_ID, MMI_DEFAULT_SMALL_FONT, MMI_BLACK_COLOR);
        }
#endif

        MMK_SetAtvCtrl(win_id, ctrl_id);

        break;          

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:         
        MMK_CloseWin(win_id);     
        break;  
    
        
    case MSG_APP_MENU:
    case MSG_APP_OK:
    case MSG_CTL_OK:    
#ifndef MMI_PDA_SUPPORT
        MMK_CreateWin((uint32 *)MMIVP_NET_URL_OPT_MENU_WIN_TAB,PNULL);
#endif
        break;   

#ifdef MMI_PDA_SUPPORT//andy.he_crNEWMS00112388
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
     case MSG_CTL_PENOK:
        if (PNULL != param)
        {
          uint32 src_id = ((MMI_NOTIFY_T*)param)->src_id; 
          if (VP_CMCC_NAME_LABEL_CTRL_ID == src_id  ||VP_NET_CMCC_INPUT_CTRL_ID==src_id)
            {        
                net_movie_info = VP_GetCMCCAddressInfo();
                
                if (0 == strnicmp(net_movie_info.url_ptr,HTTP_URL_HEAD_ASIC,HTTP_URL_HEAD_LEN))
                {                
                    OpenBrowerWin((uint8 *)(net_movie_info.url_ptr));       
                    //MMK_CloseWin(win_id);//NEWMS00164417
                }
                else if (0 == strnicmp(net_movie_info.url_ptr,RTSP_URL_HEAD_ASIC,RTSP_URL_HEAD_LEN))
                {               
                   
                    NotifyPlayerForPlayNet((wchar *)(net_movie_info.title_arr),net_movie_info.title_len,(uint8 *)(net_movie_info.url_ptr),0,0);
                   // MMK_CloseWin(win_id); //NEWMS00164417
                }
                else
                {
                    MMIPUB_OpenAlertWarningWin(TXT_INVALID_URL);
            
                }            
            }
         else if(VP_NET_URL_INPUT_CTRL_ID == src_id || VP_URL_INPUT_NAME_LABEL_CTRL_ID==src_id) 
            {
                    MMK_CreatePubEditWin((uint32*)MMIVP_NET_URL_INPUT_WIN_TAB,PNULL);
            }
        }
        break;
#endif//TOUCH_PANEL_SUPPORT //IGNORE9527
#endif 
    case MSG_CTL_MIDSK:          
    case MSG_APP_WEB:
        ctrl_id = MMK_GetActiveCtrlId(win_id);
        if (VP_NET_URL_INPUT_CTRL_ID == ctrl_id)
        {
        #ifdef MMI_PDA_SUPPORT
            GUILABEL_GetText(ctrl_id,&string);
        #else
            GUIEDIT_GetString(ctrl_id,&string);          
        #endif
            if (0 != string.wstr_len && PNULL != string.wstr_ptr && 0 == MMIAPICOM_CompareTwoWstrExt(string.wstr_ptr,RTSP_URL_HEAD_LEN,s_rtsp_url_head,RTSP_URL_HEAD_LEN,FALSE))
            {
                MMIAPICOM_WstrToStr(string.wstr_ptr,(uint8 *)url_ptr);                
                NotifyPlayerForPlayNet(PNULL,0,url_ptr,0,0);
                //MMK_CloseWin(win_id); //NEWMS00164417
            }
            else if (0 != string.wstr_len && PNULL != string.wstr_ptr)
            {
                MMIAPICOM_WstrToStr(string.wstr_ptr,(uint8 *)url_ptr);
                OpenBrowerWin(url_ptr);       
                //MMK_CloseWin(win_id);//NEWMS00164417
            }
            else
            {
                MMIPUB_OpenAlertWarningWin(TXT_COMM_URL_NULL);

            }
        }
        else
        {        
            net_movie_info = VP_GetCMCCAddressInfo();
            
            if (0 == strnicmp(net_movie_info.url_ptr,HTTP_URL_HEAD_ASIC,HTTP_URL_HEAD_LEN))
            {                
                OpenBrowerWin((uint8 *)(net_movie_info.url_ptr));       
                //MMK_CloseWin(win_id);//NEWMS00164417
            }
            else if (0 == strnicmp(net_movie_info.url_ptr,RTSP_URL_HEAD_ASIC,RTSP_URL_HEAD_LEN))
            {               
               
                NotifyPlayerForPlayNet((wchar *)(net_movie_info.title_arr),net_movie_info.title_len,(uint8 *)(net_movie_info.url_ptr),0,0);
               // MMK_CloseWin(win_id);//NEWMS00164417               
            }
            else
            {
                MMIPUB_OpenAlertWarningWin(TXT_INVALID_URL);
        
            }            
        }
        break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}
#endif

/*****************************************************************************/
//  Description : 获取本地播放列表数据
//  Global resource dependence :
//  Author: apple.zhang
//  Note:
/*****************************************************************************/
LOCAL void GetLocalListItemContent(uint16 locallist_index,
                                      wchar *item_content_ptr,
                                      uint16 *item_content_len_ptr,
                                      MMI_IMAGE_ID_T *image_id_ptr)
{    
    FILEARRAY_DATA_T       array_data   = {0};
    wchar device_ptr [MMIFILE_DEVICE_LEN + 1] = {0};
    uint16 device_len = MMIFILE_DEVICE_LEN;
    VP_LOCALLIST_TYPE_E locallist_type = VP_LOCALLIST_DEFAULT;
#ifdef DRM_SUPPORT
    MMIFMM_FILE_TYPE_E   file_type = MMIFMM_FILE_TYPE_NORMAL;
#endif

    if (PNULL == item_content_ptr || PNULL == item_content_len_ptr || PNULL == image_id_ptr)
    {
        //VP_STRACE_LOW:"[VP] GetLocalListItemContent param is error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_10416_112_2_18_3_5_50_502,(uint8*)"");
        return;
    }

    if(1 == GUITAB_GetCurSel(VP_LOCALLIST_CTRL_ID))
    {
        locallist_type = VP_LOCALLIST_USER;
    }
    
    if(VP_LocalList_GetInfo((int32)locallist_index,&array_data,locallist_type))
    {
        SCI_MEMSET(device_ptr,0,((MMIFILE_DEVICE_LEN + 1)*sizeof(wchar)));
        MMIAPIFMM_SplitFullPath(
                        array_data.filename, 
                        array_data.name_len, 
                        device_ptr, &device_len, PNULL, PNULL, 
                        item_content_ptr, item_content_len_ptr); 
#ifdef DRM_SUPPORT
        file_type = MMIAPIFMM_GetFileTypeByFileName(array_data.filename, array_data.name_len);
        if(MMIFMM_FILE_TYPE_DRM == file_type)
        {
            *image_id_ptr = MMIAPIFMM_GetDrmIconId(array_data.filename);
        }
        else
#endif
        if(MMI_DEVICE_UDISK < MMIAPIFMM_GetDeviceTypeByPath(device_ptr, device_len))
        {
            *image_id_ptr = IMAGE_COMMON_MOVIE_SD_ICON;    
        }
        else
        {
            *image_id_ptr = IMAGE_COMMON_MOVIE_UDISK_ICON;
        }       
    }

    
  
}

/*****************************************************************************/
//  Description : handle local list window message
//  Global resource dependence :
//  Author: apple.zhang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleLocalListWinMsg(
                                       MMI_WIN_ID_T         win_id,     
                                       MMI_MESSAGE_ID_E     msg_id, 
                                       DPARAM               param
                                       )
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;  
    MMI_STRING_T    str_text = {0};
    uint16          img_width = 0; 
    MMI_CTRL_ID_T   ctrl_id   = VP_LOCALLIST_CTRL_ID;

    switch(msg_id)
    {
    case MSG_LCD_SWITCH:
        GUIRES_GetImgWidthHeight(&img_width,PNULL,IMAGE_TAB_SELECT2,win_id);
//        GUITAB_SetItemWidth(ctrl_id, img_width);
        break;

    case MSG_OPEN_WINDOW:
        {
#if !((defined MMI_VIDEO_PLAYER_MINI_SUPPORT)||(defined MMI_MINI_QVGA_UI_SUPPORT)) //6530_MINI 32X32QVGA //只有本地列表
            MMI_IMAGE_ID_T vp_pda_tab[][2] = 
                {{IMAGE_VP_MY_VIDEO_FOCUS_ICON, IMAGE_VP_MY_VIDEO_ICON},
                {IMAGE_VP_VIDEO_LIST_FOCUS_ICON, IMAGE_VP_VIDEO_LIST_ICON}};

#if defined MMI_PDA_SUPPORT && defined MMI_ISTYLE_SUPPORT
            MMI_IMAGE_ID_T istyle_vp_pda_tab[][2] = 
                {{ISTYLE_IMAGE_VP_MY_VIDEO_FOCUS_ICON, ISTYLE_IMAGE_VP_MY_VIDEO_ICON},
                {ISTYLE_IMAGE_VP_VIDEO_LIST_FOCUS_ICON, ISTYLE_IMAGE_VP_VIDEO_LIST_ICON}};
#endif

            MMI_IMAGE_ID_T (*pda_tab_ptr)[2] = {PNULL};

#if defined MMI_PDA_SUPPORT && defined MMI_ISTYLE_SUPPORT
            if (MMITHEME_IsIstyle())
            {
                pda_tab_ptr = istyle_vp_pda_tab;
            }
            else
#endif
            {
                pda_tab_ptr = vp_pda_tab;
            }

            GUITAB_SetMaxItemNum(ctrl_id, VP_LOCALLIST_MAX);
            GUIRES_GetImgWidthHeight(&img_width,PNULL,IMAGE_TAB_SELECT2,win_id);
//            GUITAB_SetItemWidth(ctrl_id, img_width);

            MMI_GetLabelTextByLang(TXT_VP_MY_VIDEO, &str_text);
    	
            GUITAB_AppendSimpleItem(
                    ctrl_id,
                    &str_text,
                    pda_tab_ptr[VP_LOCALLIST_DEFAULT][0],
                    pda_tab_ptr[VP_LOCALLIST_DEFAULT][1]);  
        
            GUITAB_AddChildWindow(
                ctrl_id,
                MMIVP_DEFAULT_LOCALLIST_WIN_ID, 
                VP_LOCALLIST_DEFAULT);

    		MMI_GetLabelTextByLang(TXT_VP_CUSTOMIZED_LIST, &str_text);
            GUITAB_AppendSimpleItem(
                    ctrl_id,
                    &str_text,
                    pda_tab_ptr[VP_LOCALLIST_USER][0],
                    pda_tab_ptr[VP_LOCALLIST_USER][1]);  

            GUITAB_AddChildWindow(
                    ctrl_id,
                    MMIVP_USER_LOCALLIST_WIN_ID, 
                    VP_LOCALLIST_USER);  
#endif //MMI_MINI_QVGA_UI_SUPPORT
             //@CR241504 2011.05.13 根据当前播放视频设置list的默认项
            if(VP_LOCALLIST_DEFAULT==VP_LocalList_GetCurListType()&&0<VP_LocalList_GetNum(VP_LOCALLIST_DEFAULT))
            {
                GUILIST_SetCurItemIndex(VP_DEFAULT_LOCALLIST_CTRL_ID, VP_LocalList_GetCurrentIndex(VP_LOCALLIST_DEFAULT));
                GUITAB_SetCurSel(ctrl_id,VP_LOCALLIST_DEFAULT); 
                MMK_SetChildWinFocus(MMIVP_LOCALLIST_WIN_ID, MMIVP_DEFAULT_LOCALLIST_WIN_ID);
            }
            else if(VP_LOCALLIST_USER==VP_LocalList_GetCurListType()&&0<VP_LocalList_GetNum(VP_LOCALLIST_USER))
            {
                GUILIST_SetCurItemIndex(VP_USER_LOCALLIST_CTRL_ID, VP_LocalList_GetCurrentIndex(VP_LOCALLIST_USER));
                GUITAB_SetCurSel(ctrl_id,VP_LOCALLIST_USER); 
                MMK_SetChildWinFocus(MMIVP_LOCALLIST_WIN_ID, MMIVP_USER_LOCALLIST_WIN_ID);
            }else
            {
                GUITAB_SetCurSel(ctrl_id,VP_LOCALLIST_DEFAULT); 
                MMK_SetChildWinFocus(MMIVP_LOCALLIST_WIN_ID, MMIVP_DEFAULT_LOCALLIST_WIN_ID);
            }
            //@CR241504 2011.05.13
            
            MMK_SetAtvCtrl(win_id, ctrl_id);
        }
        break; 

    case MSG_CTL_CANCEL:
        MMK_CloseParentWin(win_id);
        break;   

    default:
        result = MMI_RESULT_FALSE;
        break;
    }

    return (result);
}

/*****************************************************************************/
//  Description :HandleLocalListWinMsg
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDefaultLocalListWinMsg(MMI_WIN_ID_T win_id, 
                                    MMI_MESSAGE_ID_E msg_id, 
                                    DPARAM param)
{
    MMI_RESULT_E                result                  = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T               list_ctrl_id            = VP_DEFAULT_LOCALLIST_CTRL_ID;
    uint32                      list_num                = 0;   
    uint16                      list_item_index         = 0;
    MMI_IMAGE_ID_T              image_id                = IMAGE_NULL;
    wchar                       item_content[VP_MAX_NAME_LEN + 1] = {0};
    uint16                      item_content_len        = 0;
    GUILIST_NEED_ITEM_DATA_T    *need_item_data_ptr     = PNULL;
    GUILIST_NEED_ITEM_CONTENT_T*need_item_content_ptr   = PNULL;
    FILEARRAY_DATA_T            array_data              = {0};
    VP_LOCALLIST_TYPE_E         locallist_type          = VP_LOCALLIST_DEFAULT;
   
        
    switch(msg_id) 
    {
     case MSG_OPEN_WINDOW: 
         
         list_num = VP_LocalList_GetNum(locallist_type);
         InitListData(list_ctrl_id,list_num); 
         if (0 == list_num)
         {
            #ifdef MMI_VIDEO_PLAYER_MINI_SUPPORT //6530_MINI
                GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_NULL, STXT_RETURN, FALSE);
            #else
                GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_NULL, STXT_RETURN, FALSE);                
            #endif //MMI_VIDEO_PLAYER_MINI_SUPPORT //6530_MINI
         }                 

         MMK_SetAtvCtrl(win_id,list_ctrl_id); 
         break;        

     case MSG_CTL_LIST_NEED_ITEM_DATA:        
        need_item_data_ptr = (GUILIST_NEED_ITEM_DATA_T*)param;
        if (PNULL == need_item_data_ptr)
        {
            //VP_STRACE_LOW:"[VP] HandleLocalListWinMsg need_item_data_ptr is null"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_10605_112_2_18_3_5_51_503,(uint8*)"");
#ifdef MMI_VIDEO_PLAYER_MINI_SUPPORT //6530_MINI 直接关闭默认本地列表
            MMK_CloseWin(MMIVP_DEFAULT_LOCALLIST_WIN_ID);
#else
            MMK_CloseParentWin(MMIVP_LOCALLIST_WIN_ID);
#endif
            break;
        }
        list_item_index = need_item_data_ptr->item_index;   
        GetLocalListItemContent(list_item_index,item_content,&item_content_len,&image_id);
        HandleListAppendData(win_id,param,list_ctrl_id,VP_NEED_LIST_ITEM_DATA_TYPE,item_content_len,item_content,image_id);
        break;
        
   case MSG_CTL_LIST_NEED_ITEM_CONTENT:      
       need_item_content_ptr = (GUILIST_NEED_ITEM_CONTENT_T*)param;
        if (PNULL == need_item_content_ptr)
        {
            //VP_STRACE_LOW:"[VP] HandleLocalListWinMsg need_item_content_ptr is null"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_10618_112_2_18_3_5_51_504,(uint8*)"");
#ifdef MMI_VIDEO_PLAYER_MINI_SUPPORT //6530_MINI
            MMK_CloseWin(MMIVP_DEFAULT_LOCALLIST_WIN_ID);
#else
            MMK_CloseParentWin(MMIVP_LOCALLIST_WIN_ID);
#endif
            break;
        }
        list_item_index = need_item_content_ptr->item_index;  
        GetLocalListItemContent(list_item_index,item_content,&item_content_len,&image_id);
        HandleListAppendData(win_id,param,list_ctrl_id,VP_NEED_LIST_ITEM_CONTENT_TYPE,item_content_len,item_content,image_id);
        break;

    case MSG_CTL_LIST_MARK_ITEM:        //get mark num
        if (0 == GUILIST_GetSelectedItemIndex(list_ctrl_id,PNULL,0))
        {
            //set list enable mark
            GUILIST_SetTextListMarkable(list_ctrl_id,FALSE);
        }
        break;


   case MSG_GET_FOCUS:
       list_num = VP_LocalList_GetNum(locallist_type);
       if(!GUILIST_GetListStateById(list_ctrl_id,GUILIST_STATE_ENABLE_MARK))
       {            
            if (0 == list_num)
            {
                    GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_NULL, STXT_RETURN, FALSE);                
                    GUILIST_SetTextListMarkable(list_ctrl_id,FALSE);  
            }
            else
            {                
                #ifdef MMI_VIDEO_PLAYER_MINI_SUPPORT //6530_MINI
                    GUIWIN_SetSoftkeyTextId(win_id, TXT_COMMON_PLAY, TXT_NULL, STXT_RETURN, FALSE);
                #else
                    GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_COMMON_PLAY, STXT_RETURN, FALSE);                
                #endif          

            }
       }   
#ifdef MMI_PDA_SUPPORT
        GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_INVISIBLE, FALSE);
#endif
       break;
       
   case MSG_APP_MENU:
    case MSG_CTL_OK:
    case MSG_APP_OK: 
#ifdef MMI_VIDEO_PLAYER_MINI_SUPPORT //6530_MINI
        list_item_index = GUILIST_GetCurItemIndex(list_ctrl_id);
        if(VP_LocalList_GetInfo(list_item_index,&array_data,locallist_type))    
        {           
            //if (PNULL != array_data.filename)
            {
                VP_LocalList_SetCurListType(locallist_type);
                VP_LocalList_SetCurrentIndex((int32)list_item_index);
                NotifyPlayerForPlayFile(array_data.filename,array_data.name_len,0,0,VP_PLAY_FROM_LOCALLIST);
            }
            MMK_CloseWin(MMIVP_DEFAULT_LOCALLIST_WIN_ID); 
        }

#else
    #ifndef MMI_PDA_SUPPORT
        MMK_CreateWin((uint32*)MMIVP_DEFAULT_LOCALLIST_POPMENU_WIN_TAB,PNULL);
    #else
        #ifdef MMI_VIDEOPLAYER_FIRST_SEARCH
            MMK_CreateWin((uint32*)MMIVP_DEFAULT_LOCALLIST_POPMENU_WIN_TAB,PNULL);
        #endif
    #endif
#endif //MMI_VIDEO_PLAYER_MINI_SUPPORT //6530_MINI
        break;

#ifdef MMI_PDA_SUPPORT
    case MSG_CTL_LIST_LONGOK:
        //长按List弹出context menu菜单
        MMK_CreateWin((uint32*)MMIVP_DEFAULT_LOCALLIST_LONGOK_POPMENU_WIN_TAB,PNULL);
        break;
#endif
   
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        list_item_index = GUILIST_GetCurItemIndex(list_ctrl_id);
        if(VP_LocalList_GetInfo(list_item_index,&array_data,locallist_type))    
        {           
            //if (PNULL != array_data.filename)
            {
                VP_LocalList_SetCurListType(locallist_type);
                VP_LocalList_SetCurrentIndex((int32)list_item_index);
                NotifyPlayerForPlayFile(array_data.filename,array_data.name_len,0,0,VP_PLAY_FROM_LOCALLIST);
            }
#ifdef MMI_VIDEO_PLAYER_MINI_SUPPORT //6530_MINI 
            MMK_CloseWin(MMIVP_DEFAULT_LOCALLIST_WIN_ID);
#else
            MMK_CloseParentWin(MMIVP_LOCALLIST_WIN_ID); //MS00197683
#endif
        }
        else
        {
#ifndef MMI_PDA_SUPPORT
#ifndef MMI_VIDEO_PLAYER_MINI_SUPPORT //6530_MINI
            MMK_CreateWin((uint32*)MMIVP_DEFAULT_LOCALLIST_POPMENU_WIN_TAB,PNULL);
#endif 
#endif
        }
        break;
    
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
#ifdef MMI_VIDEO_PLAYER_MINI_SUPPORT //6530_MINI
        MMK_CloseWin(MMIVP_DEFAULT_LOCALLIST_WIN_ID);
#else
        MMK_CloseParentWin(MMIVP_LOCALLIST_WIN_ID);
#endif
        break;   

    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return result;
}

#ifdef MMI_PDA_SUPPORT        
/*****************************************************************************/
// 	Description : 等待窗口处理
//	Global resource dependence : 
//  Author:dave.ruan
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleVPDeletingLocaListWaitingWin(
                                                 MMI_WIN_ID_T win_id,
                                                 MMI_MESSAGE_ID_E msg_id,
                                                 DPARAM param
                                                 )
{
    MMI_RESULT_E        result    = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T               list_ctrl_id            = VP_USER_LOCALLIST_CTRL_ID;
    uint16                      cur_locallist_index             = 0;  
    uint16                  mark_num = 0;
    MMIFILE_ERROR_E             file_error                      = SFS_ERROR_NONE;
    uint16  del_item_num=0;
    VP_LOCALLIST_TYPE_E         locallist_type              = VP_LOCALLIST_USER;
    static BOOLEAN user_stop=FALSE;
    static uint16                      * select_item_ptr               = PNULL;
    uint16                      marked_locallist_index             = 0;  
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        user_stop=FALSE;
        cur_locallist_index = GUILIST_GetCurItemIndex(list_ctrl_id);
        select_item_ptr = (uint16 *)SCI_ALLOCA((sizeof(uint16)*GUILIST_GetTotalItemNum(list_ctrl_id)));
        VP_STRACE_LOW("[VP]: HandleVPDeletingLocaListWaitingWin(),delete begin!");
        if (PNULL != select_item_ptr)
        {
            SCI_MEMSET(select_item_ptr,0,(sizeof(uint16)*GUILIST_GetTotalItemNum(list_ctrl_id)));/*lint !e666*/
            GUILIST_GetSelectedItemIndex(list_ctrl_id,select_item_ptr,mark_num);
           
            for (marked_locallist_index = 0; marked_locallist_index < mark_num; marked_locallist_index++)/*lint !e681*/
            {                    
                //如果删除的是当前播放文件，则停止播放
                if (locallist_type == VP_LocalList_GetCurListType() && select_item_ptr[marked_locallist_index] == VP_LocalList_GetCurrentIndex(locallist_type))

                {
#ifdef MMI_VIDEOPLAYER_HISTORYLIST_SUPPORT
                    if(IsSaveLastPlayPoint())
                    {
                        VP_HistoryList_SetLastPlayPoint(s_video_player_control_info.process_time);
                    }
#endif //MMI_VIDEOPLAYER_HISTORYLIST_SUPPORT
                    SCI_MEMSET(s_video_player_control_info.full_path_name,0,((VP_MAX_NAME_LEN +1)*sizeof(wchar)));
                    s_video_player_control_info.full_path_len = 0;
                    SCI_MEMSET(s_video_player_control_info.url,0,(VP_MAX_URL_LEN +1));
                    VideoPlayerStop(MMIVP_PLAYER_WIN_ID);
                }

                if(0 != cur_locallist_index && select_item_ptr[marked_locallist_index] <= cur_locallist_index)
                {
                    cur_locallist_index--;
                }
                
            }
            }
            
         result = MMIPUB_HandleWaitWinMsg(win_id,msg_id,param); 
        break;
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
    case MSG_APP_RED:
        {
            user_stop=TRUE;
        }
        break;
    case MSG_VP_DEL_MARKITEM_INFO:
        if(!user_stop && PNULL != select_item_ptr)
        {
            mark_num = GUILIST_GetSelectedItemIndex(list_ctrl_id,PNULL,0);
          //开始删除
            while(mark_num>0 &&  del_item_num <= VP_LOCALLIST_DELNUM_ONECIRCLE)
            {
                GUILIST_GetSelectedItemIndex(list_ctrl_id,select_item_ptr,mark_num);
                
                if (VP_LocalList_DelItemWithouSaveFile(select_item_ptr[0],locallist_type))
                {                           
                    del_item_num++;
                    GUILIST_RemoveItem(list_ctrl_id,select_item_ptr[0]);
                }
                else
                {

                    SCI_FREE(select_item_ptr);
                    select_item_ptr=PNULL;
                    //VP_STRACE_LOW:"[VP] HandleLocalListLongOkOptMenuWinMsg delete FAIL"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_10974_112_2_18_3_5_51_507,(uint8*)"");
                    break;
                }
                mark_num = GUILIST_GetSelectedItemIndex(list_ctrl_id,PNULL,0);
            }
            VP_STRACE_LOW("[VP]: HandleVPDeletingLocaListWaitingWin(),MMIAPICOM_OtherTaskToMMI marknum:%d!",mark_num);
            if(mark_num >0)
            {
                MMIAPICOM_OtherTaskToMMI(win_id,MSG_VP_DEL_MARKITEM_INFO,PNULL,0);
                break;
            }
            else
            {
                file_error = VP_Locallist_SaveToFile(locallist_type);
                if (SFS_ERROR_NONE != file_error)
                {
                    if (locallist_type == VP_LocalList_GetCurListType())
                    {            
#ifdef MMI_VIDEOPLAYER_HISTORYLIST_SUPPORT
                        VP_HistoryList_SetLastPlayPoint(s_video_player_control_info.process_time);
#endif //MMI_VIDEOPLAYER_HISTORYLIST_SUPPORT
    					SCI_MEMSET(s_video_player_control_info.full_path_name,0,((VP_MAX_NAME_LEN +1)*sizeof(wchar)));
                        s_video_player_control_info.full_path_len = 0;
                        SCI_MEMSET(s_video_player_control_info.url,0,(VP_MAX_URL_LEN +1));    
                        VideoPlayerStop(MMIVP_PLAYER_WIN_ID);
                    }
                }
                if(select_item_ptr != PNULL)
                {
                    SCI_FREE(select_item_ptr);
                    select_item_ptr=PNULL;
                }
                user_stop=FALSE;
                MMK_CloseWin(win_id);
            }    
            
        }
        else
        {
            user_stop=FALSE;
            if(select_item_ptr != PNULL)
            {
                SCI_FREE(select_item_ptr);
                select_item_ptr=PNULL;
            }
            MMK_CloseWin(win_id);
        }
        break;

case MSG_CLOSE_WINDOW:
    if(select_item_ptr != PNULL)
    {
        SCI_FREE(select_item_ptr);
        select_item_ptr=PNULL;
    }
    GUILIST_SetTextListMarkable(list_ctrl_id,FALSE);
    UpdateListData(win_id,list_ctrl_id,VP_LocalList_GetNum(locallist_type),GetLocalListItemContent);
    GUILIST_SetCurItemIndex(list_ctrl_id,cur_locallist_index); 
    //MMK_PostMsg(MMIVP_USER_LOCALLIST_WIN_ID, MSG_VP_DEL_MARKITEM_UPDATEWIN, PNULL, 0);
    result = MMIPUB_HandleWaitWinMsg(win_id,msg_id,param);
    break;
    
    default:
        result = MMIPUB_HandleWaitWinMsg(win_id,msg_id,param);
        break;
    }
    
    return result;
}
#endif
#ifndef MMI_VIDEO_PLAYER_MINI_SUPPORT //6530_MINI
/*****************************************************************************/
//  Description :HandleUserLocalListWinMsg
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleUserLocalListWinMsg(MMI_WIN_ID_T win_id, 
                                    MMI_MESSAGE_ID_E msg_id, 
                                    DPARAM param)
{
    MMI_RESULT_E                result                  = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T               list_ctrl_id            = VP_USER_LOCALLIST_CTRL_ID;
    uint32                      list_num                = 0;   
    uint16                      list_item_index              = 0;
    MMI_IMAGE_ID_T              image_id                = IMAGE_NULL;
    wchar                       item_content[VP_MAX_NAME_LEN + 1] = {0};
    uint16                      item_content_len        = 0;
    GUILIST_NEED_ITEM_DATA_T    *need_item_data_ptr     = PNULL;
    GUILIST_NEED_ITEM_CONTENT_T*need_item_content_ptr   = PNULL;
    FILEARRAY_DATA_T            array_data              = {0};
    VP_LOCALLIST_TYPE_E         locallist_type              = VP_LOCALLIST_USER;
#ifdef MMI_PDA_SUPPORT
    GUIFORM_CHILD_HEIGHT_T child_height={0};
    uint16                  mark_num = 0;
    uint16                      cur_locallist_index             = 0;  
    GUIFORM_CHILD_DISPLAY_E   display_type=GUIFORM_CHILD_DISP_MAX;
#endif
        
    switch(msg_id) 
    {
     case MSG_OPEN_WINDOW:     
       //  list_rect.left   = 0;
      //   list_rect.top    = MMI_TAB_CHILD_WINDOW_TOP;
      //   if (MMK_IsIncludeStatusBar(win_id))
     //    {
      //       list_rect.top += MMI_STATUSBAR_HEIGHT;
     //    } 
      //   list_rect.right  = MMI_MAINSCREEN_RIGHT_MAX_PIXEL;
      //   list_rect.bottom = MMI_CLIENT_RECT_BOTTOM;
      //   GUILIST_SetRect(list_ctrl_id,&list_rect);

         list_num = VP_LocalList_GetNum(locallist_type);
         InitListData(list_ctrl_id,list_num); 
         if (0 == list_num)
         {
              GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_NULL, STXT_RETURN, FALSE);                
         }                 

#ifdef MMI_PDA_SUPPORT//NEWMS00125002
        s_is_markall = FALSE;
        child_height.type = GUIFORM_CHILD_HEIGHT_LEFT;
        GUILIST_PermitBorder(list_ctrl_id, FALSE);
        GUIFORM_IsSlide(VP_USER_LOCALLIST_FORM_CTRL_ID, FALSE);
        GUIFORM_SetChildDisplay(VP_USER_LOCALLIST_FORM_CTRL_ID, MMICOMMON_SOFTKEY_CTRL_ID, GUIFORM_CHILD_DISP_HIDE);
        GUIFORM_SetChildHeight(VP_USER_LOCALLIST_FORM_CTRL_ID, list_ctrl_id, &child_height);
        GUIFORM_PermitChildBg(VP_USER_LOCALLIST_FORM_CTRL_ID, FALSE);
        GUILIST_SetUserBg(list_ctrl_id, FALSE);
#endif /* MMI_PDA_SUPPORT */


#ifdef MMI_PDA_SUPPORT
        GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
#endif
         MMK_SetAtvCtrl(win_id,list_ctrl_id); 
         break;        

     case MSG_CTL_LIST_NEED_ITEM_DATA:        
        need_item_data_ptr = (GUILIST_NEED_ITEM_DATA_T*)param;
        if (PNULL == need_item_data_ptr)
        {
            //VP_STRACE_LOW:"[VP] HandleLocalListWinMsg need_item_data_ptr is null"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_10776_112_2_18_3_5_51_505,(uint8*)"");
            MMK_CloseParentWin(MMIVP_LOCALLIST_WIN_ID);
            break;
        }
        list_item_index = need_item_data_ptr->item_index;   
        GetLocalListItemContent(list_item_index,item_content,&item_content_len,&image_id);
        HandleListAppendData(win_id,param,list_ctrl_id,VP_NEED_LIST_ITEM_DATA_TYPE,item_content_len,item_content,image_id);
        break;
        
   case MSG_CTL_LIST_NEED_ITEM_CONTENT:      
       need_item_content_ptr = (GUILIST_NEED_ITEM_CONTENT_T*)param;
        if (PNULL == need_item_content_ptr)
        {
            //VP_STRACE_LOW:"[VP] HandleLocalListWinMsg need_item_content_ptr is null"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_10789_112_2_18_3_5_51_506,(uint8*)"");
            MMK_CloseParentWin(MMIVP_LOCALLIST_WIN_ID);
            break;
        }
        list_item_index = need_item_content_ptr->item_index;  
        GetLocalListItemContent(list_item_index,item_content,&item_content_len,&image_id);
        HandleListAppendData(win_id,param,list_ctrl_id,VP_NEED_LIST_ITEM_CONTENT_TYPE,item_content_len,item_content,image_id);
        break;

    case MSG_CTL_LIST_MARK_ITEM:        //get mark num
#ifdef MMI_PDA_SUPPORT
        mark_num = GUILIST_GetSelectedItemIndex(list_ctrl_id, PNULL, 0);
        if (0 == mark_num)
        {
            GUISOFTKEY_SetTextId(win_id, MMICOMMON_SOFTKEY_CTRL_ID, TXT_MARK_ALL, TXT_DELETE, STXT_CANCEL, TRUE);
            s_is_markall = FALSE;
        }

        if (GUILIST_GetTotalItemNum(list_ctrl_id) == mark_num)
        {
            GUISOFTKEY_SetTextId(win_id, MMICOMMON_SOFTKEY_CTRL_ID, TXT_CANCEL_MARK_ALL, TXT_DELETE, STXT_CANCEL, TRUE);
            s_is_markall = TRUE;
        }
#else
        if (0 == GUILIST_GetSelectedItemIndex(list_ctrl_id,PNULL,0))
        {
            //set list enable mark
            GUILIST_SetTextListMarkable(list_ctrl_id,FALSE);
        }
#endif        
        break;


   case MSG_GET_FOCUS:
       list_num = VP_LocalList_GetNum(locallist_type);
       if(!GUILIST_GetListStateById(list_ctrl_id,GUILIST_STATE_ENABLE_MARK))
       {  
#ifndef MMI_PDA_SUPPORT
            if (0 == list_num)
            {
                GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_NULL, STXT_RETURN, FALSE);   
                GUILIST_SetTextListMarkable(list_ctrl_id,FALSE);  
            }
            else
            {               
                GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_COMMON_PLAY, STXT_RETURN, FALSE);
            }
#else

			GUIFORM_SetChildDisplay(VP_USER_LOCALLIST_FORM_CTRL_ID, MMICOMMON_SOFTKEY_CTRL_ID, GUIFORM_CHILD_DISP_HIDE);
#endif
       }   
#ifdef MMI_PDA_SUPPORT
        if (0 == list_num)//NEWMS00171354
        {
            GUIFORM_SetChildDisplay(VP_USER_LOCALLIST_FORM_CTRL_ID, MMICOMMON_SOFTKEY_CTRL_ID, GUIFORM_CHILD_DISP_HIDE);
        }
        GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
#endif
       break;
       
   case MSG_APP_MENU:
    case MSG_CTL_OK:
    case MSG_APP_OK: 
        #ifdef MMI_PDA_SUPPORT
        GUIFORM_GetChildDisplay(VP_USER_LOCALLIST_FORM_CTRL_ID, MMICOMMON_SOFTKEY_CTRL_ID, &display_type);
        if(GUIFORM_CHILD_DISP_NORMAL==display_type)
        {
            //do-nothing
        }
        else
        {
            MMK_CreateWin((uint32*)MMIVP_USER_LOCALLIST_POPMENU_WIN_TAB,PNULL); 
        }
        #else
            MMK_CreateWin((uint32*)MMIVP_USER_LOCALLIST_POPMENU_WIN_TAB,PNULL);          
        #endif
        break;
        
#ifdef MMI_PDA_SUPPORT
    case MSG_CTL_LIST_LONGOK:
        //长按List弹出context menu菜单
        MMK_CreateWin((uint32*)MMIVP_USER_LOCALLIST_LONGOK_POPMENU_WIN_TAB,PNULL);
        break;
#endif
        
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#ifdef MMI_PDA_SUPPORT//NEWMS00125002
        {
            MMI_CTRL_ID_T       ctrl_id = 0;

            if (PNULL != param)
            {
                ctrl_id = ((MMI_NOTIFY_T*) param)->src_id;
                if (ctrl_id == MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID)
                {

                    s_is_markall = !s_is_markall;

                    if (s_is_markall)
                    {
                        GUISOFTKEY_SetTextId(win_id, MMICOMMON_SOFTKEY_CTRL_ID, TXT_CANCEL_MARK_ALL, TXT_DELETE, STXT_CANCEL, FALSE );
                        if (0 == GUILIST_GetSelectedItemIndex(list_ctrl_id,PNULL,0))
                        {
                            //set list enable mark
                            GUILIST_SetTextListMarkable(list_ctrl_id,TRUE);
                            //set mark max number
                            GUILIST_SetMaxSelectedItem(list_ctrl_id,(uint16)VP_LocalList_GetNum(locallist_type));
                        }
                        //mark all item
                        GUILIST_SetAllSelected(list_ctrl_id,TRUE);            
                    }
                    else
                    {
                        GUISOFTKEY_SetTextId(win_id, MMICOMMON_SOFTKEY_CTRL_ID, TXT_MARK_ALL, TXT_DELETE, STXT_CANCEL, FALSE );
                        GUILIST_SetAllSelected(list_ctrl_id,FALSE);
                    }

                    MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, 0);
                    break;


                }
                else if (ctrl_id == MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID)
                {
               // #ifdef MMI_STREAMING_PLAYER_SUPPORT 
                    VPExitBookmarkListMarked(list_ctrl_id);        
		//#endif				
                    MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, 0);
                    break;
                }
                else if (ctrl_id == MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID)
                {
                    mark_num = GUILIST_GetSelectedItemIndex(list_ctrl_id, PNULL, 0);
        
                    if(0 == mark_num)
                    {
                         MMIPUB_OpenAlertWarningWin(TXT_SMS_SELECT_ONT_PROMPT);
                    }
                    else
                    {
                        MMI_STRING_T    str = {0};
                        MMI_GetLabelTextByLang(TXT_COMMON_WAITING, &str);
                        cur_locallist_index = GUILIST_GetCurItemIndex(list_ctrl_id);
                       
                        {
                                MMIPUB_OpenWaitWin(1,&str,PNULL,PNULL,MMIVP_DELETING_LOCALLIST_WAIT_WIN_ID,
                                    PNULL,ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_ONE,HandleVPDeletingLocaListWaitingWin); 
                                MMK_PostMsg(MMIVP_DELETING_LOCALLIST_WAIT_WIN_ID, MSG_VP_DEL_MARKITEM_INFO, PNULL, 0);   
                            
                        }      
                    }
                    break;
                }
            }
        }
#endif /* MMI_PDA_SUPPORT */
    
        list_item_index = GUILIST_GetCurItemIndex(list_ctrl_id);
        if(VP_LocalList_GetInfo(list_item_index,&array_data,locallist_type))    
        {          
            //if (PNULL != array_data.filename)
            {
                VP_LocalList_SetCurListType(locallist_type);
                VP_LocalList_SetCurrentIndex((int32)list_item_index);
                NotifyPlayerForPlayFile(array_data.filename,array_data.name_len,0,0,VP_PLAY_FROM_LOCALLIST);
            }           
            MMK_CloseParentWin(MMIVP_LOCALLIST_WIN_ID);//MS00197683
            
        }
        else
        {
            MMK_CreateWin((uint32*)MMIVP_USER_LOCALLIST_POPMENU_WIN_TAB,PNULL);
        }
        break;
    
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseParentWin(MMIVP_LOCALLIST_WIN_ID);
        break;   

    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return result;
}

#endif
 #if 0//norflashcut
/*****************************************************************************/
//  Description :HandleVPFileDetailWinMsg
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleVPFileDetailWinMsg(MMI_WIN_ID_T win_id, 
                                    MMI_MESSAGE_ID_E msg_id, 
                                    DPARAM param)
{
    MMI_RESULT_E                result = MMI_RESULT_TRUE;
    GUI_LCD_DEV_INFO            lcd_dev_info = {0};  
    MMI_CTRL_ID_T               ctrl_id = VP_FILE_DETAIL_TEXT_CTRL_ID;
    GUI_RECT_T                  d_rect = MMITHEME_GetClientRect();
    VP_LOCAL_MOVIE_FILE_INFO_T* local_movie_file_info_ptr = (VP_LOCAL_MOVIE_FILE_INFO_T*)MMK_GetWinAddDataPtr(win_id);


    lcd_dev_info.lcd_id = GUI_MAIN_LCD_ID;
    lcd_dev_info.block_id = GUI_BLOCK_MAIN;
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUIWIN_SetStbBgImageId(win_id,IMAGE_COMMON_BG);
        SetFileDetailEntry(ctrl_id,local_movie_file_info_ptr); 
        if (PNULL != local_movie_file_info_ptr)
        {
            SCI_FREE(local_movie_file_info_ptr);
            local_movie_file_info_ptr = PNULL;
        }
        GUIRICHTEXT_SetFocusMode(ctrl_id, GUIRICHTEXT_FOCUS_SHIFT_NONE);
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;

    case MSG_FULL_PAINT:
        GUIRES_DisplayImg(PNULL,
            &d_rect,
            &d_rect,
            win_id,
            IMAGE_COMMON_BG,
            &lcd_dev_info);
        break;    

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;  
    
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}
#endif
#ifdef MMI_STREAMING_PLAYER_SUPPORT
/*****************************************************************************/
//  Description :HandleVPNetDetailWinMsg
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleVPNetDetailWinMsg(MMI_WIN_ID_T win_id, 
                                    MMI_MESSAGE_ID_E msg_id, 
                                    DPARAM param)
{
    MMI_RESULT_E                result = MMI_RESULT_TRUE;
    GUI_LCD_DEV_INFO            lcd_dev_info = {0};  
    MMI_CTRL_ID_T               ctrl_id = VP_NET_DETAIL_TEXT_CTRL_ID;
    GUI_RECT_T                  d_rect = MMITHEME_GetClientRect();
    VP_NET_MOVIE_INFO_T         *net_movie_info_ptr = (VP_NET_MOVIE_INFO_T *)MMK_GetWinAddDataPtr(win_id);
 
    lcd_dev_info.lcd_id = GUI_MAIN_LCD_ID;
    lcd_dev_info.block_id = GUI_BLOCK_MAIN;

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        //GUIWIN_SetStbBgImageId(win_id,IMAGE_COMMON_BG);
        SetNetDetailEntry(ctrl_id, net_movie_info_ptr);  
        if (PNULL != net_movie_info_ptr)
        {
            SCI_FREE(net_movie_info_ptr);
            net_movie_info_ptr = PNULL;
        }
        GUIRICHTEXT_SetFocusMode(ctrl_id, GUIRICHTEXT_FOCUS_SHIFT_NONE);
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;

    case MSG_FULL_PAINT:
        GUIRES_DisplayImg(PNULL,
            &d_rect,
            &d_rect,
            win_id,
            IMAGE_COMMON_BG,
            &lcd_dev_info);
        break;
    

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;  
    

    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}

/****************************************************************************/    
//  Description : SetNetDetailEntry
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void SetNetDetailEntry(MMI_CTRL_ID_T ctrl_id, VP_NET_MOVIE_INFO_T *net_movie_info_ptr)
{
    MMI_STRING_T string = {0};    
   
    /* name */
    SCI_MEMSET(&string, 0, sizeof(MMI_STRING_T));
    MMIRES_GetText(TXT_INPUT_NAME, ctrl_id, &string);
    RichText_AddItem(ctrl_id, &string, GUIRICHTEXT_FRAME_NONE, GUIRICHTEXT_TAG_NONE);

    SCI_MEMSET(&string, 0, sizeof(MMI_STRING_T)); 
    if (PNULL != net_movie_info_ptr && 0 != net_movie_info_ptr->title_len)
    {
        string.wstr_len = net_movie_info_ptr->title_len;
        string.wstr_ptr = net_movie_info_ptr->title_arr; 
    }       
    RichText_AddItem(ctrl_id, &string, GUIRICHTEXT_FRAME_NONE, GUIRICHTEXT_TAG_NONE);
   
    /* url*/
    SCI_MEMSET(&string, 0, sizeof(MMI_STRING_T));
    MMIRES_GetText(TXT_INPUT_URL_ADRESS, ctrl_id, &string);
    RichText_AddItem(ctrl_id, &string, GUIRICHTEXT_FRAME_NONE, GUIRICHTEXT_TAG_NONE);

    SCI_MEMSET(&string, 0, sizeof(MMI_STRING_T));
    if (PNULL != net_movie_info_ptr)
    {    
        string.wstr_len = strlen(net_movie_info_ptr->url_ptr);
        string.wstr_ptr = (wchar*)SCI_ALLOCA((string.wstr_len +1) * sizeof(wchar));
        if (PNULL != string.wstr_ptr)
        {
            SCI_MEMSET(string.wstr_ptr,0,((string.wstr_len+1) * sizeof(wchar)));
            MMIAPICOM_StrToWstr(((uint8 *)(net_movie_info_ptr->url_ptr)),string.wstr_ptr);
        }
    }
    RichText_AddItem(ctrl_id, &string, GUIRICHTEXT_FRAME_NONE, GUIRICHTEXT_TAG_NONE);    
    if (PNULL != string.wstr_ptr)
    {
        SCI_FREE(string.wstr_ptr);
    }
}
#endif

#if  0  //norflashcut
/****************************************************************************/    
//  Description : SetFileDetailEntry
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void SetFileDetailEntry(MMI_CTRL_ID_T ctrl_id, VP_LOCAL_MOVIE_FILE_INFO_T* local_movie_file_info_ptr)
{
    MMI_STRING_T                string                          = {0};
    MMI_TM_T                    tm                              = {0}; 
    uint8                       detail_arr[VP_MAX_NAME_LEN + 1]     = {0};
    wchar                       wchar_detail_arr[VP_MAX_NAME_LEN + 1] = {0};
    MMIFILE_DEVICE_E            dev = MMI_DEVICE_NUM;
    
    /* filename */
    SCI_MEMSET(&string, 0, sizeof(MMI_STRING_T));
    MMIRES_GetText(TXT_INPUT_NAME, ctrl_id, &string);
    RichText_AddItem(ctrl_id, &string, GUIRICHTEXT_FRAME_NONE, GUIRICHTEXT_TAG_NONE);

    SCI_MEMSET(&string, 0, sizeof(MMI_STRING_T));    
    if (PNULL != local_movie_file_info_ptr)
    {
        string.wstr_len = VP_MAX_NAME_LEN;
        MMIAPIFMM_SplitFullPath(local_movie_file_info_ptr->full_path_ptr,
            local_movie_file_info_ptr->full_path_len,
            PNULL,
            PNULL,
            PNULL,
            PNULL,
            wchar_detail_arr,
            &(string.wstr_len));   
        string.wstr_ptr = wchar_detail_arr;
          
    }    
    RichText_AddItem(ctrl_id, &string, GUIRICHTEXT_FRAME_NONE, GUIRICHTEXT_TAG_NONE);
   
    /* date */
    SCI_MEMSET(&string, 0, sizeof(MMI_STRING_T));
    MMIRES_GetText(TXT_COMMON_DATE, ctrl_id, &string);
    RichText_AddItem(ctrl_id, &string, GUIRICHTEXT_FRAME_NONE, GUIRICHTEXT_TAG_NONE);

    SCI_MEMSET(&string, 0, sizeof(MMI_STRING_T));
    if (PNULL != local_movie_file_info_ptr)
    {
        tm = MMIAPICOM_FatData2Tm(local_movie_file_info_ptr->create_time); 
        MMIAPISET_FormatDateStrByDateStyle(tm.tm_year, tm.tm_mon, tm.tm_mday,
            '.',detail_arr,VP_MAX_NAME_LEN + 1);    
    
        MMIAPICOM_StrToWstr((uint8*) detail_arr, (wchar *)wchar_detail_arr );
        string.wstr_ptr = wchar_detail_arr;
        string.wstr_len = (uint16)strlen((char *)detail_arr);
    }   
    RichText_AddItem(ctrl_id, &string, GUIRICHTEXT_FRAME_NONE, GUIRICHTEXT_TAG_NONE);   
    
     /* time */
    SCI_MEMSET(&string, 0, sizeof(MMI_STRING_T));
    MMIRES_GetText(TXT_COMMON_TIME, ctrl_id, &string);
    RichText_AddItem(ctrl_id, &string, GUIRICHTEXT_FRAME_NONE, GUIRICHTEXT_TAG_NONE);

    SCI_MEMSET(&string, 0, sizeof(MMI_STRING_T));
    if (PNULL != local_movie_file_info_ptr)
    {
        SCI_MEMSET(detail_arr,0,sizeof(detail_arr));
        SCI_MEMSET(wchar_detail_arr,0,sizeof(wchar_detail_arr));

        MMIAPISET_FormatTimeStrByTime(tm.tm_hour, tm.tm_min,detail_arr,VP_MAX_NAME_LEN + 1);
        MMIAPICOM_StrToWstr((uint8*) detail_arr, (wchar *)wchar_detail_arr );
        string.wstr_ptr = wchar_detail_arr;
        string.wstr_len = (uint16)strlen((char *)detail_arr);
    }   
    RichText_AddItem(ctrl_id, &string, GUIRICHTEXT_FRAME_NONE, GUIRICHTEXT_TAG_NONE);   

    /* size*/
    SCI_MEMSET(&string, 0, sizeof(MMI_STRING_T));
    MMIRES_GetText(TXT_DETAIL_SIZE, ctrl_id, &string);
    RichText_AddItem(ctrl_id, &string, GUIRICHTEXT_FRAME_NONE, GUIRICHTEXT_TAG_NONE);

    SCI_MEMSET(&string, 0, sizeof(MMI_STRING_T));
    if (PNULL != local_movie_file_info_ptr)
    {
        SCI_MEMSET(wchar_detail_arr,0,sizeof(wchar_detail_arr));
        MMIAPIFMM_GetFileSizeString(local_movie_file_info_ptr->file_size, wchar_detail_arr, MMIFMM_SIZE_STR_LEN, FALSE);        
        string.wstr_ptr = wchar_detail_arr;
        string.wstr_len = MMIAPICOM_Wstrlen((wchar *)wchar_detail_arr);
    }   
    RichText_AddItem(ctrl_id, &string, GUIRICHTEXT_FRAME_NONE, GUIRICHTEXT_TAG_NONE);   

   /* pos*/
    SCI_MEMSET(&string, 0, sizeof(MMI_STRING_T));
    MMIRES_GetText(TXT_COMMON_DETAIL_FILE_POS, ctrl_id, &string);
    RichText_AddItem(ctrl_id, &string, GUIRICHTEXT_FRAME_NONE, GUIRICHTEXT_TAG_NONE);

    SCI_MEMSET(&string, 0, sizeof(MMI_STRING_T));
    if (PNULL != local_movie_file_info_ptr)
    {
        dev = MMIAPIFMM_GetDeviceTypeByPath(local_movie_file_info_ptr->full_path_ptr, MMIFILE_DEVICE_LEN);
        MMIRES_GetText(MMIAPIFMM_GetDeviceName(dev), ctrl_id, &string);   
    }   
    
    RichText_AddItem(ctrl_id, &string, GUIRICHTEXT_FRAME_NONE, GUIRICHTEXT_TAG_NONE);   
    
     /* pixel*/
    SCI_MEMSET(&string, 0, sizeof(MMI_STRING_T));
    MMIRES_GetText(TXT_COMM_RESOLUTION, ctrl_id, &string);
    RichText_AddItem(ctrl_id, &string, GUIRICHTEXT_FRAME_NONE, GUIRICHTEXT_TAG_NONE);

    SCI_MEMSET(&string, 0, sizeof(MMI_STRING_T));
    if (PNULL != local_movie_file_info_ptr)
    {
        SCI_MEMSET(detail_arr,0,sizeof(detail_arr));
        SCI_MEMSET(wchar_detail_arr,0,sizeof(wchar_detail_arr));
        sprintf((char*)detail_arr, "%ldX%ld",local_movie_file_info_ptr->width,local_movie_file_info_ptr->height);
        MMIAPICOM_StrToWstr( (uint8*)detail_arr,(wchar *)wchar_detail_arr );     
        string.wstr_ptr = wchar_detail_arr;
        string.wstr_len = (uint16)strlen((char *)detail_arr);        
    }   
    RichText_AddItem(ctrl_id, &string, GUIRICHTEXT_FRAME_NONE, GUIRICHTEXT_TAG_NONE);   

     /* duration*/
    SCI_MEMSET(&string, 0, sizeof(MMI_STRING_T));
    MMIRES_GetText(TXT_TOTAL_TIME, ctrl_id, &string);
    RichText_AddItem(ctrl_id, &string, GUIRICHTEXT_FRAME_NONE, GUIRICHTEXT_TAG_NONE);

    SCI_MEMSET(&string, 0, sizeof(MMI_STRING_T));
    if (PNULL != local_movie_file_info_ptr)
    {
        SCI_MEMSET(detail_arr,0,sizeof(detail_arr));
        SCI_MEMSET(wchar_detail_arr,0,sizeof(wchar_detail_arr));
        sprintf((char*)detail_arr, "%.2ld:%.2ld:%.2ld", ((local_movie_file_info_ptr->media_length)/(VP_SEC_TO_MSEL*VP_HOUR_TO_MIN*VP_MIN_TO_SEC)), (((local_movie_file_info_ptr->media_length)/(VP_SEC_TO_MSEL*VP_HOUR_TO_MIN))%VP_MIN_TO_SEC), (((local_movie_file_info_ptr->media_length)/VP_SEC_TO_MSEL)%VP_MIN_TO_SEC));
        MMIAPICOM_StrToWstr((uint8*) detail_arr,(wchar *)wchar_detail_arr );
        string.wstr_ptr = wchar_detail_arr;
        string.wstr_len = (uint16)strlen((char *)detail_arr);        
    }   
    RichText_AddItem(ctrl_id, &string, GUIRICHTEXT_FRAME_NONE, GUIRICHTEXT_TAG_NONE);   

#ifdef DRM_SUPPORT
    if(PNULL != local_movie_file_info_ptr
        && MMIAPIDRM_IsDRMFile(PNULL,local_movie_file_info_ptr->full_path_ptr))
    {
        MMIAPIFMM_AddDrmFileDetail(local_movie_file_info_ptr->full_path_ptr, SFS_MODE_READ|SFS_MODE_OPEN_EXISTING, 0, 0, ctrl_id);
    }    
#endif
     

}
#endif
#ifdef MMI_STREAMING_PLAYER_SUPPORT
/*****************************************************************************/
//  Description :HandleAddBookmarkWinMsg
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleAddBookmarkWinMsg(MMI_WIN_ID_T win_id, 
                                    MMI_MESSAGE_ID_E msg_id, 
                                    DPARAM param)
{
    MMI_RESULT_E                recode          = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T               ctrl_id         = VP_ADD_BOOKMARK_FORM_CTRL_ID;
    const VP_NET_MOVIE_INFO_T          *bookmark_data_ptr = PNULL;
    VP_LIST_ERROR_E             add_bookmark_result = 0; 
    MMI_STRING_T                undefine_string          = {0};
    uint16                      bookmark_list_index      = (uint32)MMK_GetWinAddDataPtr(win_id);
    VP_NET_MOVIE_INFO_T         bookmark_data   = {0};
    MMI_STRING_T               url_string = {0};
    MMI_STRING_T               name_string = {0};

    
    //VP_STRACE_LOW:"[VP] HandleAddBookmarkWinMsg msg_id = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_11357_112_2_18_3_5_52_508,(uint8*)"d",msg_id);
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
       //GUIWIN_SetStbBgImageId(win_id,IMAGE_COMMON_BG);
        GUIFORM_SetStyle(VP_BOOKMARK_NAME_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
        GUIFORM_SetStyle(VP_BOOKMARK_URL_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);

        if (bookmark_list_index == VP_BookmarkList_GetNum())
        {
            //添加新书签
#ifdef CMCC_UI_STYLE
            GUIWIN_SetTitleTextId(win_id,STXT_ADD,FALSE);
#else
            GUIWIN_SetTitleTextId(win_id,TXT_VP_SAVE_BOOKMARK_TITLE,FALSE);
#endif
            SetInputBookmarkDetailEntry(ctrl_id,PNULL);           
            
        }
        else
        {
            //修改书签
            GUIWIN_SetTitleTextId(win_id,TXT_MODIFY ,FALSE);
            bookmark_data_ptr = VP_BookmarkList_GetInfo(VP_BookmarkList_GetNum() - 1 - bookmark_list_index);
            SetInputBookmarkDetailEntry(ctrl_id,(VP_NET_MOVIE_INFO_T *)bookmark_data_ptr);            
        }        
    
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;       
   
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:          
        MMK_CloseWin(win_id);     
        break;        
   
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#ifdef MMI_PDA_SUPPORT
        if ((PNULL != param) && ((MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID == ((MMI_NOTIFY_T*)param)->src_id)
            || (MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID == ((MMI_NOTIFY_T*)param)->src_id)
            || (MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID == ((MMI_NOTIFY_T*)param)->src_id)))
        {
            if ((MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID == ((MMI_NOTIFY_T*)param)->src_id))
            {
                //left softkey处理
            }
            else if ((MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID == ((MMI_NOTIFY_T*)param)->src_id))
            {
                //middle softkey处理
                MMK_PostMsg(win_id, MSG_APP_OK, PNULL, NULL);
            }
            else if ((MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID == ((MMI_NOTIFY_T*)param)->src_id))
            {
                //right softkey处理
            }
        }
#endif //MMI_PDA_SUPPORT
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

    case MSG_APP_OK:
    case MSG_CTL_OK:   
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        GUIEDIT_GetString(VP_BOOKMARK_URL_INPUT_CTRL_ID,&url_string);

        if ((PNULL == url_string.wstr_ptr) || (0 == url_string.wstr_len))
        {
            MMIPUB_OpenAlertWarningWin(TXT_COMM_URL_NULL);

        }
        else if (0 == MMIAPICOM_CompareTwoWstrExt(s_http_url_head,HTTP_URL_HEAD_LEN,url_string.wstr_ptr,HTTP_URL_HEAD_LEN,FALSE) 
                || 0 == MMIAPICOM_CompareTwoWstrExt(s_rtsp_url_head,RTSP_URL_HEAD_LEN,url_string.wstr_ptr,RTSP_URL_HEAD_LEN,FALSE))
        {
            SCI_MEMSET(bookmark_data.url_ptr,0,(VP_MAX_URL_LEN+1));
            MMIAPICOM_WstrToStr(url_string.wstr_ptr, ((uint8 *)(bookmark_data.url_ptr)));
            GUIEDIT_GetString(VP_BOOKMARK_NAME_INPUT_CTRL_ID,&name_string);            
            SCI_MEMSET(bookmark_data.title_arr,0,((VP_MAX_NAME_LEN+1)*sizeof(wchar)));
            if (PNULL == name_string.wstr_ptr || 0 == name_string.wstr_len)
            {
                MMI_GetLabelTextByLang(TXT_VP_UNDEFINE,&undefine_string);
                bookmark_data.title_len = MIN(VP_MAX_NAME_LEN,undefine_string.wstr_len);   
                MMIAPICOM_Wstrncpy(bookmark_data.title_arr, undefine_string.wstr_ptr,bookmark_data.title_len);          
                             
            }
            else
            {
                bookmark_data.title_len = MIN(VP_MAX_NAME_LEN,name_string.wstr_len);
                MMIAPICOM_Wstrncpy(bookmark_data.title_arr,name_string.wstr_ptr,bookmark_data.title_len);                              
            }

            //VP_STRACE_LOW:"[VP] bookmark_data.title_arr = %x, list_index = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_11443_112_2_18_3_5_52_509,(uint8*)"dd",bookmark_data.title_arr,bookmark_list_index);
            if (bookmark_list_index == VP_BookmarkList_GetNum())
            {
                add_bookmark_result = VP_BookmarkList_AddItem(bookmark_data);
                bookmark_list_index = 0;
                
            }
            else
            {
                add_bookmark_result = VP_BookmarkList_ModifyItem((VP_BookmarkList_GetNum() - 1 - bookmark_list_index),bookmark_data);
            }
            UpdateListData(MMIVP_BOOKMARKLIST_WIN_ID,VP_BOOKMARK_LIST_CTRL_ID,VP_BookmarkList_GetNum(),GetBookmarkListItemContent);
         
            GUILIST_SetCurItemIndex(VP_BOOKMARK_LIST_CTRL_ID,bookmark_list_index);
            if (VP_LIST_ERROR_TOO_MUCH == add_bookmark_result)
            {
                MMIPUB_OpenAlertWarningWin(TXT_VP_TOO_MUCH);
            }
            else if (VP_LIST_ERROR_NONE == add_bookmark_result)
            { 
				//xin.li_clean_popwin
                //MMIPUB_OpenAlertSuccessWin(TXT_VP_SAVE_BOOKMARK_SUCC);

            }
            else if(VP_LIST_ERROR_NO_SPACE == add_bookmark_result)
            {
                MMIPUB_OpenAlertWarningWin(TXT_NO_DISK_SPACE);
            }
            else
            {
                MMIPUB_OpenAlertFailWin(TXT_ERROR);
            }
            MMK_CloseWin(win_id);    
        }
        else
        {
            MMIPUB_OpenAlertWarningWin(TXT_INVALID_URL);
        }
        break;       

   
    
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}

/****************************************************************************/    
//  Description : SetInputBookmarkDetailEntry
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void SetInputBookmarkDetailEntry(MMI_CTRL_ID_T ctrl_id,VP_NET_MOVIE_INFO_T *bookmark_data_ptr)
{        

    wchar  *url_wstr_ptr = PNULL;
    wchar                       star_value[]= {'.'};

    GUILABEL_SetTextById(VP_BOOKMARK_NAME_LABEL_CTRL_ID,TXT_INPUT_NAME,FALSE);
    if (PNULL != bookmark_data_ptr && 0 != bookmark_data_ptr->title_len)
    {
        GUIEDIT_SetString(VP_BOOKMARK_NAME_INPUT_CTRL_ID,bookmark_data_ptr->title_arr,bookmark_data_ptr->title_len);
    }
    GUILABEL_SetTextById(VP_BOOKMARK_URL_LABEL_CTRL_ID,TXT_INPUT_URL_ADRESS,FALSE);
    GUIEDIT_SetIm(VP_BOOKMARK_URL_INPUT_CTRL_ID,GUIIM_TYPE_ENGLISH | GUIIM_TYPE_ABC | GUIIM_TYPE_DIGITAL, GUIIM_TYPE_ABC); /*lint !e655*/
    GUIEDIT_ConfigImCustomKey(VP_BOOKMARK_URL_INPUT_CTRL_ID,GUIIM_TYPE_DIGITAL,GUIIM_CUSTOM_KEY_STAR,star_value,1);
    GUIEDIT_SetImTag(VP_BOOKMARK_URL_INPUT_CTRL_ID, GUIIM_TAG_URL);
   if (PNULL != bookmark_data_ptr)
   {
       url_wstr_ptr = (wchar*)SCI_ALLOCA((strlen((char *)bookmark_data_ptr->url_ptr) + 1) * sizeof(wchar));
        if (PNULL != url_wstr_ptr)
        {
            SCI_MEMSET(url_wstr_ptr,0,((strlen((char *)bookmark_data_ptr->url_ptr) + 1) * sizeof(wchar)));/*lint !e666*/
            MMIAPICOM_StrToWstr(((uint8 *)(bookmark_data_ptr->url_ptr)),url_wstr_ptr);
            GUIEDIT_SetString(VP_BOOKMARK_URL_INPUT_CTRL_ID,url_wstr_ptr,strlen((char *)bookmark_data_ptr->url_ptr));
        } 
        else
        {
            GUIEDIT_SetString(VP_BOOKMARK_URL_INPUT_CTRL_ID,(wchar *)s_rtsp_url_head,RTSP_URL_HEAD_LEN);
        }
   }
   else
   {
        
       GUIEDIT_SetString(VP_BOOKMARK_URL_INPUT_CTRL_ID,(wchar *)s_rtsp_url_head,RTSP_URL_HEAD_LEN);
   } 
}
/*****************************************************************************/
//  Description : 视频播放器书签选项消息处理.
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleBookmarkOptMenuWinMsg(
                                     MMI_WIN_ID_T       win_id,     
                                     MMI_MESSAGE_ID_E    msg_id, 
                                     DPARAM              param
                                     )
{
    MMI_RESULT_E                result              = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T               ctrl_id             = VP_BOOKMARK_OPT_MENU_CTRL_ID;
    MMI_MENU_ID_T               menu_id             = 0;
    MMI_MENU_GROUP_ID_T         group_id            = 0;  
    BOOLEAN                     delete_result       = TRUE;
    uint16                      cur_bookmark_list_index      = 0;
    const VP_NET_MOVIE_INFO_T         *bookmark_data_ptr  = PNULL;
    VP_NET_MOVIE_INFO_T         *net_info_ptr = PNULL;

    switch (msg_id)
    {
    case MSG_FULL_PAINT: 
        if (VP_ENTRY_FROM_PLAYER != s_video_player_control_info.vp_entry)
        {
            MMIAPICOM_EnableGrayed(win_id,MENU_VP_OPEN_OPT,ID_VP_OPEN_LOCALLIST,TRUE);
        }
        MMIAPICOM_EnableGrayed(win_id,MENU_VP_VIEW_OPT,ID_VP_VIEW_BOOKMARK,TRUE);
        if (0 == VP_BookmarkList_GetNum())
        {
            MMIAPICOM_EnableGrayed(win_id,MENU_VP_BOOKMARK_OPT,ID_VP_DELETE_CUR,TRUE);
            MMIAPICOM_EnableGrayed(win_id,MENU_VP_BOOKMARK_OPT,ID_VP_DELETE_ALL,TRUE);
            MMIAPICOM_EnableGrayed(win_id,MENU_VP_OPERATER_OPT,ID_VP_MODIFY,TRUE);
            MMIAPICOM_EnableGrayed(win_id,MENU_VP_BOOKMARK_OPT,ID_VP_FILEATTR,TRUE);
        }
        else if (VP_BOOKMARK_MAX_ITEM == VP_BookmarkList_GetNum())
        {
            MMIAPICOM_EnableGrayed(win_id,MENU_VP_OPERATER_OPT,ID_VP_ADD,TRUE);
        }
        MMK_SetAtvCtrl(win_id,ctrl_id);
        break;

    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        GUIMENU_GetId(ctrl_id,&group_id,&menu_id);
        //VP_STRACE_LOW:"[VP] HandleBookmarkOptMenuWinMsg menu_id = %x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_11579_112_2_18_3_5_53_510,(uint8*)"d",menu_id);
        switch (menu_id)
        { 
        case ID_VP_OPEN_LOCALFILE:
            if (OpenLocalFileWin(win_id))
            {
                MMK_CloseWin(MMIVP_BOOKMARKLIST_WIN_ID);
            }
            break;
#ifdef MMI_STREAMING_PLAYER_SUPPORT  //norflashcut
        case ID_VP_OPEN_NET:
            MMK_CreateWin((uint32 *)MMIVP_NET_WIN_TAB,PNULL);      
            MMK_CloseWin(win_id);
            MMK_CloseWin(MMIVP_BOOKMARKLIST_WIN_ID);
            break;
#endif
        case ID_VP_OPEN_LOCALLIST:
            OpenLocalListWin();
            MMK_CloseWin(win_id);
            MMK_CloseWin(MMIVP_BOOKMARKLIST_WIN_ID);
            break;
#ifdef MMI_VIDEOPLAYER_HISTORYLIST_SUPPORT
        case ID_VP_VIEW_HISTORYLIST:
            MMK_CreateWin((uint32 *)MMIVP_HISTORYLIST_WIN_TAB,PNULL);
            MMK_CloseWin(win_id);
            MMK_CloseWin(MMIVP_BOOKMARKLIST_WIN_ID);
            break;  
#endif
      
        case ID_VP_FILEATTR:
            cur_bookmark_list_index = GUILIST_GetCurItemIndex(VP_BOOKMARK_LIST_CTRL_ID);
            //VP_STRACE_LOW:"[VP] HandleBookmarkOptMenuWinMsg cur_list_index = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_11610_112_2_18_3_5_53_511,(uint8*)"d",cur_bookmark_list_index);
            bookmark_data_ptr = VP_BookmarkList_GetInfo(VP_BookmarkList_GetNum() - 1 - cur_bookmark_list_index);          
            if (PNULL != bookmark_data_ptr)
            {
                net_info_ptr = (VP_NET_MOVIE_INFO_T *)SCI_ALLOCA(sizeof(VP_NET_MOVIE_INFO_T));
                if (PNULL != net_info_ptr)
                {
                    SCI_MEMSET(net_info_ptr,0,sizeof(VP_NET_MOVIE_INFO_T));
                    SCI_MEMCPY(net_info_ptr,bookmark_data_ptr,sizeof(VP_NET_MOVIE_INFO_T));                
                    MMK_CreateWin((uint32 *)MMIVP_VIEW_NET_DETAIL_WIN_TAB,(ADD_DATA)net_info_ptr);
                }
                else
                {
                    //VP_STRACE_LOW:"[VP] HandleBookmarkOptMenuWinMsg no memory"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_11623_112_2_18_3_5_53_512,(uint8*)"");
                    MMIPUB_OpenAlertFailWin(TXT_ERROR);
                }
            }
            else
            {
                MMIPUB_OpenAlertFailWin(TXT_ERROR);
            }
            MMK_CloseWin(win_id);
           
            break;
            
        case ID_VP_DELETE_CUR:
            MMIPUB_OpenQueryWinByTextId(TXT_DELETEQUERY,IMAGE_PUBWIN_QUERY,PNULL,PNULL);
            break;
            
        case ID_VP_DELETE_ALL:
            MMIPUB_OpenQueryWinByTextId(TXT_DELALL,IMAGE_PUBWIN_QUERY,PNULL,PNULL);
            break;
            
        case ID_VP_MODIFY:
            cur_bookmark_list_index = GUILIST_GetCurItemIndex(VP_BOOKMARK_LIST_CTRL_ID);
            MMK_CreateWin((uint32 *) MMIVP_ADD_BOOKMARK_WIN_TAB,(ADD_DATA)cur_bookmark_list_index);
            MMK_CloseWin(win_id);
            break;

        case ID_VP_ADD:
            cur_bookmark_list_index = VP_BookmarkList_GetNum();
            MMK_CreateWin((uint32 *) MMIVP_ADD_BOOKMARK_WIN_TAB,(ADD_DATA)cur_bookmark_list_index);
            MMK_CloseWin(win_id);
            break;
        default:
            break;
        } 
        
        break;

    case MSG_PROMPTWIN_OK:
        MMIPUB_CloseQuerytWin(PNULL);
        GUIMENU_GetId(ctrl_id,&group_id,&menu_id);
        //VP_STRACE_LOW:"[VP] HandleBookmarkOptMenuWinMsg menu_id = %x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_11663_112_2_18_3_5_53_513,(uint8*)"d",menu_id);
        switch(menu_id) 
        {
        case ID_VP_DELETE_CUR:  
            cur_bookmark_list_index = GUILIST_GetCurItemIndex(VP_BOOKMARK_LIST_CTRL_ID);
            delete_result = VP_BookmarkList_DelItem(VP_BookmarkList_GetNum() - 1 - cur_bookmark_list_index);
            break;
        case ID_VP_DELETE_ALL:
            delete_result = VP_BookmarkList_DelAllItem();
            break;
        default:
            break;
        }  
        UpdateListData(MMIVP_BOOKMARKLIST_WIN_ID,VP_BOOKMARK_LIST_CTRL_ID,VP_BookmarkList_GetNum(),GetBookmarkListItemContent);
        GUILIST_SetCurItemIndex(VP_BOOKMARK_LIST_CTRL_ID,cur_bookmark_list_index);
        if (delete_result)
        {
            //MMIPUB_OpenAlertSuccessWin(TXT_DELETED);
        }
        else
        {
            MMIPUB_OpenAlertFailWin(TXT_COMMON_DEL_FAIL);
        }
        MMK_CloseWin(win_id);
        break;

    case MSG_PROMPTWIN_CANCEL:
        MMIPUB_CloseQuerytWin(PNULL);
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}
#endif


#ifdef MMI_VIDEOPLAYER_HISTORYLIST_SUPPORT
/*****************************************************************************/
//  Description : 视频播放器历史记录选项消息处理.
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleHistoryOptMenuWinMsg(
                                     MMI_WIN_ID_T       win_id,     
                                     MMI_MESSAGE_ID_E    msg_id, 
                                     DPARAM              param
                                     )
{
    MMI_RESULT_E            result              = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T           ctrl_id             = VP_HISTORY_OPT_MENU_CTRL_ID;
    MMI_MENU_ID_T           menu_id             = 0;
    MMI_MENU_GROUP_ID_T     group_id            = 0;
    BOOLEAN                 delete_result       = TRUE;
    uint16                  cur_historylist_index      = 0;
    const VP_HISTORY_ITEM_T       *history_info_ptr   = PNULL;
    VP_LOCAL_MOVIE_FILE_INFO_T* local_movie_file_info_ptr = PNULL;
    MMIFMM_DETAIL_DATA_T    detail_data ={0};
    MMI_TM_T                tm = {0};
#ifdef MMI_STREAMING_PLAYER_SUPPORT
   VP_NET_MOVIE_INFO_T         *net_file_info_ptr = PNULL;
#endif
    //VP_STRACE_LOW:"[VP] HandleHistoryOptMenuWinMsg msg_id = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_11730_112_2_18_3_5_53_514,(uint8*)"d",msg_id);
    switch (msg_id)
    {
    case MSG_FULL_PAINT:
        if (VP_ENTRY_FROM_PLAYER != s_video_player_control_info.vp_entry)
        {
            MMIAPICOM_EnableGrayed(win_id,MENU_VP_OPEN_OPT,ID_VP_OPEN_LOCALLIST,TRUE);
        }
        MMIAPICOM_EnableGrayed(win_id,MENU_VP_VIEW_OPT,ID_VP_VIEW_HISTORYLIST,TRUE);
        if (0 == VP_HistoryList_GetNum())
        {
            MMIAPICOM_EnableGrayed(win_id,MENU_VP_HISTORY_OPT,ID_VP_DELETE_CUR,TRUE);      
            MMIAPICOM_EnableGrayed(win_id,MENU_VP_HISTORY_OPT,ID_VP_DELETE_ALL,TRUE);      
            MMIAPICOM_EnableGrayed(win_id,MENU_VP_HISTORY_OPT,ID_VP_FILEATTR,TRUE);
        }
        
        MMK_SetAtvCtrl(win_id,ctrl_id);
        break;

    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        GUIMENU_GetId(ctrl_id,&group_id,&menu_id);
        //VP_STRACE_LOW:"[VP] HandleHistoryOptMenuWinMsg menu_id = %x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_11755_112_2_18_3_5_53_515,(uint8*)"d",menu_id);
        switch (menu_id)
        {
        case ID_VP_OPEN_LOCALFILE:
            if(OpenLocalFileWin(win_id))
            {                
                MMK_CloseWin(MMIVP_HISTORYLIST_WIN_ID);
            }
            break;
#ifdef MMI_STREAMING_PLAYER_SUPPORT  //norflashcut
        case ID_VP_OPEN_NET:
            MMK_CreateWin((uint32 *)MMIVP_NET_WIN_TAB,PNULL);      
            MMK_CloseWin(win_id);
            MMK_CloseWin(MMIVP_HISTORYLIST_WIN_ID);
            break;
#endif
        case ID_VP_OPEN_LOCALLIST:
            OpenLocalListWin();
            MMK_CloseWin(win_id);
            MMK_CloseWin(MMIVP_HISTORYLIST_WIN_ID);
            break;
#ifdef MMI_STREAMING_PLAYER_SUPPORT
         case ID_VP_VIEW_BOOKMARK:
            MMK_CreateWin((uint32 *)MMIVP_BOOKMARKLIST_WIN_TAB,PNULL);
            MMK_CloseWin(win_id);
            MMK_CloseWin(MMIVP_HISTORYLIST_WIN_ID);
            break;     
#endif      
        case ID_VP_FILEATTR:
            cur_historylist_index = GUILIST_GetCurItemIndex(VP_HISTORY_LIST_CTRL_ID);
            history_info_ptr = VP_HistoryList_GetInfo(cur_historylist_index);
            if (PNULL != history_info_ptr)
            {
#ifdef MMI_STREAMING_PLAYER_SUPPORT
                if (VP_PLAY_FROM_NET == history_info_ptr->type)
                {
                    net_file_info_ptr = (VP_NET_MOVIE_INFO_T *)SCI_ALLOCA(sizeof(VP_NET_MOVIE_INFO_T));
                    if (PNULL != net_file_info_ptr && PNULL != history_info_ptr->net_movie_info_ptr)
                    {
                        SCI_MEMSET(net_file_info_ptr,0,sizeof(VP_NET_MOVIE_INFO_T));
                        SCI_MEMCPY(net_file_info_ptr,history_info_ptr->net_movie_info_ptr,sizeof(VP_NET_MOVIE_INFO_T));
                        MMK_CreateWin((uint32 *)MMIVP_VIEW_NET_DETAIL_WIN_TAB,(ADD_DATA)net_file_info_ptr);
                    }
                    else
                    {
                        //VP_STRACE_LOW:"[VP] HandleHistoryOptMenuWinMsg no memory"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_11800_112_2_18_3_5_53_516,(uint8*)"");
                        MMIPUB_OpenAlertFailWin(TXT_ERROR);

                    }
                }
                else
#endif
					if (PNULL != history_info_ptr->local_movie_file_info_ptr &&
                    !MMIAPIFMM_IsFileExist(history_info_ptr->local_movie_file_info_ptr->full_path_ptr,history_info_ptr->local_movie_file_info_ptr->full_path_len))
                {
                    MMIPUB_OpenAlertWarningWin(TXT_COM_FILE_NO_EXIST);
                }
                else
                {
                    local_movie_file_info_ptr = history_info_ptr->local_movie_file_info_ptr;
                    if (PNULL != local_movie_file_info_ptr)
                    {
                        tm = MMIAPICOM_FatData2Tm(local_movie_file_info_ptr->create_time); 
                        detail_data.time = MMIAPICOM_Tm2Second(tm.tm_sec,tm.tm_min,tm.tm_hour,tm.tm_mday,tm.tm_mon,tm.tm_year);
                        detail_data.file_size = local_movie_file_info_ptr->file_size;
                        detail_data.filename_len = MIN(MMIFMM_FILENAME_LEN,local_movie_file_info_ptr->full_path_len);
                        MMIAPICOM_Wstrncpy(detail_data.filename,local_movie_file_info_ptr->full_path_ptr,detail_data.filename_len);
#ifdef DRM_SUPPORT
                        if(MMIAPIDRM_IsDRMFile(PNULL,detail_data.filename))
                        {
                            detail_data.type = MMIFMM_FILE_TYPE_DRM;
                        }
                        else
#endif
                        {
                            detail_data.type = MMIFMM_FILE_TYPE_MOVIE;
                        }
                        MMIAPIFMM_DetailFileData(&detail_data);
                    }
                    else
                    {
                        MMIPUB_OpenAlertFailWin(TXT_ERROR);
                    }
                }
            }
            else
            {
                MMIPUB_OpenAlertFailWin(TXT_ERROR);
            }
            MMK_CloseWin(win_id);
            break;
            
        case ID_VP_DELETE_CUR:
            MMIPUB_OpenQueryWinByTextId(TXT_DELETEQUERY,IMAGE_PUBWIN_QUERY,PNULL,PNULL);
            break;
            
        case ID_VP_DELETE_ALL:
            MMIPUB_OpenQueryWinByTextId(TXT_DELALL,IMAGE_PUBWIN_QUERY,PNULL,PNULL);
            break;            
       
        default:
            break;
        } 
        
        break;

    case MSG_PROMPTWIN_OK:
        MMIPUB_CloseQuerytWin(PNULL);
        GUIMENU_GetId(ctrl_id,&group_id,&menu_id);
        //VP_STRACE_LOW:"[VP] HandleHistoryOptMenuWinMsg menu_id = %x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_11864_112_2_18_3_5_53_517,(uint8*)"d",menu_id);
        switch(menu_id) 
        {
        case ID_VP_DELETE_CUR:
            cur_historylist_index = GUILIST_GetCurItemIndex(VP_HISTORY_LIST_CTRL_ID);
            delete_result = VP_HistoryList_DelItem(cur_historylist_index);
            break;
        case ID_VP_DELETE_ALL:
            delete_result = VP_HistoryList_DelAllItem();
            cur_historylist_index = 0;
            break;
        default:
            break;
        }  
        UpdateListData(MMIVP_HISTORYLIST_WIN_ID,VP_HISTORY_LIST_CTRL_ID,VP_HistoryList_GetNum(),GetHistoryListItemContent);
        GUILIST_SetCurItemIndex(VP_HISTORY_LIST_CTRL_ID,cur_historylist_index);
        if (delete_result)
        {
		    //xin.li_clean_popwin
            //MMIPUB_OpenAlertSuccessWin(TXT_DELETED);
                        
        }
        else
        {
            MMIPUB_OpenAlertFailWin(TXT_COMMON_DEL_FAIL);
        }       
        MMK_CloseWin(win_id);
        
        break;

    case MSG_PROMPTWIN_CANCEL:
        MMIPUB_CloseQuerytWin(PNULL);
        MMK_CloseWin(win_id);   //NEWMS00164270
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}
#endif
/*****************************************************************************/
//  Description : AddLocalListCallBack
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
LOCAL void AddLocalListCallBack( VP_OP_LIST_RESULT_E    result, 
                                VP_LOCALLIST_TYPE_E locallist_type)
{
    uint16 list_index = GUILIST_GetCurItemIndex(VP_USER_LOCALLIST_CTRL_ID);

    //VP_STRACE_LOW:"[VP] AddLocalListCallBack locallist_type = %d, list_index = %d, is_success = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_11920_112_2_18_3_5_53_518,(uint8*)"ddd",locallist_type,list_index,result);
        
    UpdateListData(MMIVP_USER_LOCALLIST_WIN_ID,VP_USER_LOCALLIST_CTRL_ID,VP_LocalList_GetNum(locallist_type),GetLocalListItemContent);   
    GUILIST_SetCurItemIndex(VP_USER_LOCALLIST_CTRL_ID,list_index);

    if (VP_OP_LIST_SUCCESS != result)
    {
        if (VP_OP_LIST_SAVE_ERROR == result
            && VP_PLAY_FROM_LOCALLIST == s_video_player_control_info.play_type
            && VP_LOCALLIST_USER == VP_LocalList_GetCurListType())
        {       
#ifdef MMI_VIDEOPLAYER_HISTORYLIST_SUPPORT
            VP_HistoryList_SetLastPlayPoint(s_video_player_control_info.process_time);
#endif //MMI_VIDEOPLAYER_HISTORYLIST_SUPPORT 
			SCI_MEMSET(s_video_player_control_info.full_path_name,0,((VP_MAX_NAME_LEN +1)*sizeof(wchar)));
            s_video_player_control_info.full_path_len = 0;
            SCI_MEMSET(s_video_player_control_info.url,0,(VP_MAX_URL_LEN +1));    
            VideoPlayerStop(MMIVP_PLAYER_WIN_ID);
        }
        if (VP_OP_LIST_NO_SPACE == result)
        {
            if(MMI_DEVICE_NUM <= MMIAPIFMM_GetValidateDevice(MMI_DEVICE_UDISK))
            {
                MMIPUB_OpenAlertWinByTextId(PNULL,TXT_NO_SD_CARD_ALERT,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_NONE,PNULL);
            }
            else
            {
                MMIPUB_OpenAlertWinByTextId(PNULL,TXT_NO_SPACE,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_NONE,PNULL);
            }
        }
        else
        {
            MMIPUB_OpenAlertFailWin(TXT_ERROR);
    
        }
    }
}
#ifndef MMI_VIDEO_PLAYER_MINI_SUPPORT //6530_MINI
/*****************************************************************************/
//  Description : 视频播放器本地列表选项消息处理.
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleLocalListOptMenuWinMsg(
                                     MMI_WIN_ID_T       win_id,     
                                     MMI_MESSAGE_ID_E    msg_id, 
                                     DPARAM              param
                                     )
{
    MMI_RESULT_E                result                          = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T               ctrl_id                         = VP_DEFAULT_LOCALLIST_OPT_MENU_CTRL_ID;
    MMI_CTRL_ID_T               list_ctrl_id                    = VP_DEFAULT_LOCALLIST_CTRL_ID;
    MMI_MENU_ID_T               menu_id                         = 0;
    MMI_MENU_GROUP_ID_T         group_id                        = MENU_VP_DEFAULT_LOCALLIST_OPT;      
    uint16                      cur_locallist_index             = 0;  
    uint16                      marked_locallist_index             = 0;  
    FILEARRAY_DATA_T            array_data                      = {0};   
    MMI_STRING_T                prompt_str                      = {0};
    uint16                      mark_num                        = 0;
    uint16                      * select_item_ptr               = PNULL;
    uint32                      locallist_num                   = 0;
    GUILIST_ITEM_T              *item_ptr                       = PNULL;
    VP_LOCALLIST_TYPE_E         locallist_type                  = VP_LOCALLIST_DEFAULT;
    MMI_WIN_ID_T                list_win_id                     = MMIVP_DEFAULT_LOCALLIST_WIN_ID;
    MMIFILE_ERROR_E             file_error                      = SFS_ERROR_NONE;
    MMIFMM_DETAIL_DATA_T        detail_data                     = {0};

    if (MMIVP_USER_LOCALLIST_OPT_MENU_WIN_ID == win_id)
    {
        ctrl_id = VP_USER_LOCALLIST_OPT_MENU_CTRL_ID;
        list_ctrl_id = VP_USER_LOCALLIST_CTRL_ID;
        locallist_type = VP_LOCALLIST_USER;
        list_win_id = MMIVP_USER_LOCALLIST_WIN_ID;
#ifndef MMI_VIDEO_PLAYER_MINI_SUPPORT //6530_MINI
        group_id   = MENU_VP_USER_LOCALLIST_OPT;
#endif
    }

    //VP_STRACE_LOW:"[VP] HandleLocalListOptMenuWinMsg msg_id = %x, locallist_type = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_11995_112_2_18_3_5_53_519,(uint8*)"dd",msg_id,locallist_type);
    switch (msg_id)
    {
    case MSG_FULL_PAINT:
        locallist_num = VP_LocalList_GetNum(locallist_type);        
#ifndef MMI_VIDEO_PLAYER_MINI_SUPPORT //6530_MINI 没有该菜单       
        MMIAPICOM_EnableGrayed(win_id,MENU_VP_OPEN_OPT,ID_VP_OPEN_LOCALLIST,TRUE);
#endif
        if (0 == locallist_num)
        {
            MMIAPICOM_EnableGrayed(win_id,group_id,ID_VP_DELETE,TRUE);
            MMIAPICOM_EnableGrayed(win_id,group_id,ID_VP_FILEATTR,TRUE);
            MMIAPICOM_EnableGrayed(win_id,group_id,ID_VP_MARK_OPT,TRUE);
            MMIAPICOM_EnableGrayed(win_id,group_id,ID_VP_SETTING_AS,TRUE);
        }
        else
        {
// #ifdef MMI_STREAMING_PLAYER_SUPPORT
            mark_num = GUILIST_GetSelectedItemIndex(list_ctrl_id,PNULL,0);
            if (0 == mark_num)
            {
                MMIAPICOM_EnableGrayed(win_id,MENU_VP_MARK_OPT,ID_VP_CANCEL_MARK,TRUE);
                MMIAPICOM_EnableGrayed(win_id,MENU_VP_MARK_OPT,ID_VP_CANCEL_ALL_MARK,TRUE);
            }
            else if (mark_num > 0)
            {
                MMIAPICOM_EnableGrayed(win_id,group_id,ID_VP_SETTING_AS,TRUE);
                MMIAPICOM_EnableGrayed(win_id,group_id,ID_VP_FILEATTR,TRUE);
                if(locallist_num == mark_num)
                {
                    MMIAPICOM_EnableGrayed(win_id,MENU_VP_MARK_OPT,ID_VP_MARK_ALL,TRUE);
                    MMIAPICOM_EnableGrayed(win_id,MENU_VP_MARK_OPT,ID_VP_SUB_MARK,TRUE);                
                }  
                else 
                {
                    cur_locallist_index = GUILIST_GetCurItemIndex(list_ctrl_id);          
                    item_ptr = (GUILIST_ITEM_T*)GUILIST_GetItemPtrByIndex(list_ctrl_id,cur_locallist_index);                
                    if(PNULL != item_ptr && GUILIST_GetItemState(item_ptr,GUIITEM_STATE_SELECTED) )
                    {
                        MMIAPICOM_EnableGrayed(win_id,MENU_VP_MARK_OPT,ID_VP_SUB_MARK,TRUE);
                    }
                    else
                    {
                        MMIAPICOM_EnableGrayed(win_id,MENU_VP_MARK_OPT,ID_VP_CANCEL_MARK,TRUE);
                    }
                
                }
            }
// #endif
        }
        MMK_SetAtvCtrl(win_id,ctrl_id);
        break;

    case MSG_CTL_MIDSK:  
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_WEB:
        GUIMENU_GetId(ctrl_id,&group_id,&menu_id);
        
        //VP_STRACE_LOW:"[VP] HandleLocalListOptMenuWinMsg menu_id = %x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_12050_112_2_18_3_5_54_520,(uint8*)"d",menu_id);
        switch (menu_id)
        {  
        case ID_VP_OPEN_LOCALFILE:
            if(OpenLocalFileWin(win_id))
            {                
                MMK_CloseParentWin(MMIVP_LOCALLIST_WIN_ID);
            }
            break;
#ifdef MMI_STREAMING_PLAYER_SUPPORT  //norflashcut
        case ID_VP_OPEN_NET:
            MMK_CreateWin((uint32 *)MMIVP_NET_WIN_TAB,PNULL);
            MMK_CloseWin(win_id);
            MMK_CloseParentWin(MMIVP_LOCALLIST_WIN_ID);
            break;  
#endif      
        case ID_VP_FILEATTR:
            cur_locallist_index = GUILIST_GetCurItemIndex(list_ctrl_id);
            //VP_STRACE_LOW:"[VP] HandleLocalListOptMenuWinMsg cur_list_index = %d, locallist_type = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_12068_112_2_18_3_5_54_521,(uint8*)"dd",cur_locallist_index,locallist_type);
            if(VP_LocalList_GetInfo(cur_locallist_index,&array_data,locallist_type))
            {
                if (!MMIAPIFMM_IsFileExist(array_data.filename,array_data.name_len))
                {
                    MMIPUB_OpenAlertWarningWin(TXT_COM_FILE_NO_EXIST);
                }
                else
                {
                    detail_data.time = MMIAPICOM_Tm2Second(array_data.modify_time.sec,
                                                            array_data.modify_time.min,
                                                            array_data.modify_time.hour,
                                                            array_data.modify_date.mday,
                                                            array_data.modify_date.mon,
                                                            array_data.modify_date.year);
                    detail_data.file_size = array_data.size;
                    detail_data.filename_len = MIN(MMIFMM_FILENAME_LEN,array_data.name_len);
                    MMIAPICOM_Wstrncpy(detail_data.filename,array_data.filename,detail_data.filename_len);
#ifdef DRM_SUPPORT
                    if(MMIAPIDRM_IsDRMFile(PNULL,detail_data.filename))
                    {
                        detail_data.type = MMIFMM_FILE_TYPE_DRM;
                    }
                    else
#endif
                    {
                        detail_data.type = MMIFMM_FILE_TYPE_MOVIE;
                    }
                    MMIAPIFMM_DetailFileData(&detail_data);
                }
                
            }
            else
            {
                MMIPUB_OpenAlertFailWin(TXT_ERROR);
                //VP_STRACE_LOW:"[VP] HandleLocalListOptMenuWinMsg get file info error"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_12103_112_2_18_3_5_54_522,(uint8*)"");
            }
            MMK_CloseWin(win_id);
            break;
            
        case ID_VP_DELETE:
            mark_num = GUILIST_GetSelectedItemIndex(list_ctrl_id,PNULL,0);
            cur_locallist_index = GUILIST_GetCurItemIndex(list_ctrl_id);
            if (0 == mark_num)  //删除当前
            {
                //ger current index
                //VP_STRACE_LOW:"[VP] HandleLocalListOptMenuWinMsg cur_list_index = %d, locallist_type = %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_12114_112_2_18_3_5_54_523,(uint8*)"dd",cur_locallist_index,locallist_type);
                //如果删除的是当前播放文件，则停止播放
                if (locallist_type == VP_LocalList_GetCurListType() && cur_locallist_index == VP_LocalList_GetCurrentIndex(locallist_type))
                { 
#ifdef MMI_VIDEOPLAYER_HISTORYLIST_SUPPORT
                    if (IsSaveLastPlayPoint())
                    {
                        VP_HistoryList_SetLastPlayPoint(s_video_player_control_info.process_time);
                    }          
#endif //MMI_VIDEOPLAYER_HISTORYLIST_SUPPORT
                    SCI_MEMSET(s_video_player_control_info.full_path_name,0,((VP_MAX_NAME_LEN +1)*sizeof(wchar)));
                    s_video_player_control_info.full_path_len = 0;
                    SCI_MEMSET(s_video_player_control_info.url,0,(VP_MAX_URL_LEN +1));    
                    VideoPlayerStop(MMIVP_PLAYER_WIN_ID);
                }
                //删除本地播放列表
                if (VP_LocalList_DelItemWithouSaveFile(cur_locallist_index,locallist_type))
                {
                    GUILIST_RemoveItem(list_ctrl_id,cur_locallist_index);
                    file_error = VP_Locallist_SaveToFile(locallist_type);
                    if (SFS_ERROR_NONE != file_error)
                    {
                        if (locallist_type == VP_LocalList_GetCurListType())
                        {    
#ifdef MMI_VIDEOPLAYER_HISTORYLIST_SUPPORT
                            VP_HistoryList_SetLastPlayPoint(s_video_player_control_info.process_time);
#endif //MMI_VIDEOPLAYER_HISTORYLIST_SUPPORT
							SCI_MEMSET(s_video_player_control_info.full_path_name,0,((VP_MAX_NAME_LEN +1)*sizeof(wchar)));
                            s_video_player_control_info.full_path_len = 0;
                            SCI_MEMSET(s_video_player_control_info.url,0,(VP_MAX_URL_LEN +1));    
                            VideoPlayerStop(MMIVP_PLAYER_WIN_ID);
                        }
                    }
                }
            }
            else //删除标记
            {
                select_item_ptr = (uint16 *)SCI_ALLOCA((sizeof(uint16)*GUILIST_GetTotalItemNum(list_ctrl_id)));
                if (PNULL != select_item_ptr)
                {
                    MMIPUB_OpenAlertWinByTextId(PNULL,TXT_COMMON_WAITING,TXT_NULL,IMAGE_PUBWIN_WAIT,PNULL,PNULL,MMIPUB_SOFTKEY_NONE,PNULL);

                    SCI_MEMSET(select_item_ptr,0,(sizeof(uint16)*GUILIST_GetTotalItemNum(list_ctrl_id)));/*lint !e666*/
                    GUILIST_GetSelectedItemIndex(list_ctrl_id,select_item_ptr,mark_num);
                   
                    for (marked_locallist_index = 0; marked_locallist_index < mark_num; marked_locallist_index++)
                    {                    
                        //如果删除的是当前播放文件，则停止播放
                        if (locallist_type == VP_LocalList_GetCurListType() && select_item_ptr[marked_locallist_index] == VP_LocalList_GetCurrentIndex(locallist_type))

                        {
#ifdef MMI_VIDEOPLAYER_HISTORYLIST_SUPPORT
                            if(IsSaveLastPlayPoint())
                            {
                                VP_HistoryList_SetLastPlayPoint(s_video_player_control_info.process_time);
                            }
#endif //MMI_VIDEOPLAYER_HISTORYLIST_SUPPORT
                            SCI_MEMSET(s_video_player_control_info.full_path_name,0,((VP_MAX_NAME_LEN +1)*sizeof(wchar)));
                            s_video_player_control_info.full_path_len = 0;
                            SCI_MEMSET(s_video_player_control_info.url,0,(VP_MAX_URL_LEN +1));
                            VideoPlayerStop(MMIVP_PLAYER_WIN_ID);
                        }

                        if(0 != cur_locallist_index && select_item_ptr[marked_locallist_index] <= cur_locallist_index)
                        {
                            cur_locallist_index--;
                        }
                        
                    }

                    //开始删除
                    while(mark_num>0)
                    {
                        GUILIST_GetSelectedItemIndex(list_ctrl_id,select_item_ptr,mark_num);
                        
                        if (VP_LocalList_DelItemWithouSaveFile(select_item_ptr[0],locallist_type))
                        {                           
                            GUILIST_RemoveItem(list_ctrl_id,select_item_ptr[0]);
                        }
                        else
                        {

                            SCI_FREE(select_item_ptr);
                            //VP_STRACE_LOW:"[VP] HandleLocalListOptMenuWinMsg delete FAIL"
                            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_12197_112_2_18_3_5_54_524,(uint8*)"");
                            break;
                        }
                        mark_num = GUILIST_GetSelectedItemIndex(list_ctrl_id,PNULL,0);
                    }
                    MMIPUB_CloseAlertWin();               
                    file_error = VP_Locallist_SaveToFile(locallist_type);
                    if (SFS_ERROR_NONE != file_error)
                    {
                        if (locallist_type == VP_LocalList_GetCurListType())
                        {       
#ifdef MMI_VIDEOPLAYER_HISTORYLIST_SUPPORT
                            VP_HistoryList_SetLastPlayPoint(s_video_player_control_info.process_time);
#endif //MMI_VIDEOPLAYER_HISTORYLIST_SUPPORT
							SCI_MEMSET(s_video_player_control_info.full_path_name,0,((VP_MAX_NAME_LEN +1)*sizeof(wchar)));
                            s_video_player_control_info.full_path_len = 0;
                            SCI_MEMSET(s_video_player_control_info.url,0,(VP_MAX_URL_LEN +1));    
                            VideoPlayerStop(MMIVP_PLAYER_WIN_ID);
                        }
                    }

                    SCI_FREE(select_item_ptr);
                    //marked_locallist_index = 0;
                    
                }
            }
            GUILIST_SetTextListMarkable(list_ctrl_id,FALSE);
            UpdateListData(list_win_id,list_ctrl_id,VP_LocalList_GetNum(locallist_type),GetLocalListItemContent);
            GUILIST_SetCurItemIndex(list_ctrl_id,cur_locallist_index);
            if(VP_STATE_READY == MMIAPIVP_FSMGetState(s_video_player_control_info.vp_handle))
            {
                cur_locallist_index = (cur_locallist_index >= VP_LocalList_GetNum(locallist_type))?0:cur_locallist_index;
                if(VP_LocalList_GetInfo(cur_locallist_index,&array_data,locallist_type))
                {
                    VP_LocalList_SetCurrentIndex(cur_locallist_index);
                    NotifyPlayerForPlayFile(array_data.filename, array_data.name_len, 0, 0, VP_PLAY_FROM_LOCALLIST);
                    s_video_player_control_info.to_do_type = VP_TO_DO_NONE;
                }
            }
            MMK_CloseWin(win_id);
            break;
            
        case ID_VP_ADD:
            VP_LocalList_AddItem(AddLocalListCallBack,locallist_type);     
            GUILIST_SetTextListMarkable(list_ctrl_id,FALSE);
            MMK_CloseWin(win_id);
            break;

        case ID_VP_UPDATE:

            //先检查是否有合适的存储器且存储器空间足够；add by sandy for cr246623.
            if(SFS_ERROR_NO_SPACE == VP_LocalList_GetSuitableDeviceState())  
            {
                MMIPUB_OpenAlertWinByTextId(PNULL,TXT_NO_SPACE,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_NONE,PNULL);
                MMK_CloseWin(win_id);
                break;
            }
            
            MMI_GetLabelTextByLang(TXT_COMMON_UPDATING, &prompt_str);
            MMIPUB_OpenWaitWin(1,&prompt_str,PNULL,PNULL,MMIVP_LOADING_LOCALLIST_WAIT_WIN_ID,
                PNULL,ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_ONE,HandleVPSearchWaitingWin); 
            if (VP_PLAY_FROM_LOCALLIST == s_video_player_control_info.play_type && locallist_type == VP_LocalList_GetCurListType())
            {
#ifdef MMI_VIDEOPLAYER_HISTORYLIST_SUPPORT
                if (IsSaveLastPlayPoint())
                {
                    VP_HistoryList_SetLastPlayPoint(s_video_player_control_info.process_time);
                }   
#endif //MMI_VIDEOPLAYER_HISTORYLIST_SUPPORT
                SCI_MEMSET(s_video_player_control_info.full_path_name,0,((VP_MAX_NAME_LEN +1)*sizeof(wchar)));
                s_video_player_control_info.full_path_len = 0;
                SCI_MEMSET(s_video_player_control_info.url,0,(VP_MAX_URL_LEN +1));
                VideoPlayerStop(MMIVP_PLAYER_WIN_ID);
            }            
            MMK_PostMsg(MMIVP_LOADING_LOCALLIST_WAIT_WIN_ID,MSG_VP_UPDATE_BEGIN,(DPARAM)&locallist_type,sizeof(VP_LOCALLIST_TYPE_E));
            GUILIST_SetTextListMarkable(list_ctrl_id,FALSE);
            MMK_CloseWin(win_id);
            break;
#ifdef MMI_PDA_SUPPORT//NEWMS00125002       
//#ifdef MMI_STREAMING_PLAYER_SUPPORT
        case ID_VP_MARK_OPT:  
            VPEnterBookmarkListMarked(list_ctrl_id);
            MMK_CloseWin(win_id);
            break;
//#endif			
#endif
        case ID_VP_SUB_MARK:
            //set list enable mark
            GUILIST_SetTextListMarkable(list_ctrl_id,TRUE);
            //set mark max number
            GUILIST_SetMaxSelectedItem(list_ctrl_id,(uint16)VP_LocalList_GetNum(locallist_type));
            //get item index
            cur_locallist_index = GUILIST_GetCurItemIndex(list_ctrl_id);
            //mark item
            GUILIST_SetSelectedItem(list_ctrl_id,cur_locallist_index,TRUE);
            MMK_CloseWin(win_id);
            break;

        case ID_VP_CANCEL_MARK:
            cur_locallist_index = GUILIST_GetCurItemIndex(list_ctrl_id);
            //cancel mark item
            GUILIST_SetSelectedItem(list_ctrl_id,cur_locallist_index,FALSE);
            //get mark num       
            if (0 == GUILIST_GetSelectedItemIndex(list_ctrl_id,PNULL,0))
            {
                //set list enable mark
                GUILIST_SetTextListMarkable(list_ctrl_id,FALSE);
            }
            MMK_CloseWin(win_id);
            break;

        case ID_VP_MARK_ALL:          
            if (0 == GUILIST_GetSelectedItemIndex(list_ctrl_id,PNULL,0))
            {
                //set list enable mark
                GUILIST_SetTextListMarkable(list_ctrl_id,TRUE);
                //set mark max number
                GUILIST_SetMaxSelectedItem(list_ctrl_id,(uint16)VP_LocalList_GetNum(locallist_type));
            }
            //mark all item
            GUILIST_SetAllSelected(list_ctrl_id,TRUE);
            MMK_CloseWin(win_id);
            break;

        case ID_VP_CANCEL_ALL_MARK:
            GUILIST_SetAllSelected(list_ctrl_id,FALSE);
            //set list enable mark
            GUILIST_SetTextListMarkable(list_ctrl_id,FALSE);
            MMK_CloseWin(win_id);
            break;

        case ID_VP_SCEENSAVE:
#ifdef SCREENSAVER_SUPPORT
            cur_locallist_index = GUILIST_GetCurItemIndex(list_ctrl_id);
            VP_LocalList_GetInfo(cur_locallist_index,&array_data,locallist_type);

            if(!MMIAPIFMM_IsFileExist(array_data.filename,array_data.name_len))
            {
                MMIPUB_OpenAlertWarningWin(TXT_COM_FILE_NO_EXIST);
            }    
            else if (0 == array_data.size)
            {
                MMIPUB_OpenAlertWarningWin(TXT_EMPTY_FILE);
            }
            else
            {
                SettingAsScreensaver(array_data.filename,array_data.name_len);
            }
#endif
            MMK_CloseWin(win_id);
            break;

#ifdef MMI_VIDEOWALLPAPER_SUPPORT
        case ID_VP_VIDEO_WALLPAPER:
            cur_locallist_index = GUILIST_GetCurItemIndex(list_ctrl_id);
            VP_LocalList_GetInfo(cur_locallist_index,&array_data,locallist_type);
            SettingAsVideoWallpaper(array_data.filename,array_data.name_len,array_data.size);
            MMK_CloseWin(win_id);
            break;
#endif

        case ID_VP_POWER_ON_SET:
            cur_locallist_index = GUILIST_GetCurItemIndex(list_ctrl_id);
            VP_LocalList_GetInfo(cur_locallist_index,&array_data,locallist_type);
            if(!MMIAPIFMM_IsFileExist(array_data.filename,array_data.name_len))
            {
                MMIPUB_OpenAlertWarningWin(TXT_COM_FILE_NO_EXIST);
            }    
            else if (0 == array_data.size)//@andy.he cr240708 2011/06/09
            {
                MMIPUB_OpenAlertWarningWin(TXT_EMPTY_FILE);
            }  
            else
            {
                SettingAsPowerOnOffAnimation(TRUE,array_data.filename,array_data.name_len);
            }
            MMK_CloseWin(win_id);
            break;

        case ID_VP_POWER_OFF_SET:
            cur_locallist_index = GUILIST_GetCurItemIndex(list_ctrl_id);
            VP_LocalList_GetInfo(cur_locallist_index,&array_data,locallist_type);
            if(!MMIAPIFMM_IsFileExist(array_data.filename,array_data.name_len))
            {
                MMIPUB_OpenAlertWarningWin(TXT_COM_FILE_NO_EXIST);
            }
            else if (0 == array_data.size)//@andy.he cr240708 2011/06/09
            {
                MMIPUB_OpenAlertWarningWin(TXT_EMPTY_FILE);
            }  
            else
            {
                SettingAsPowerOnOffAnimation(FALSE,array_data.filename,array_data.name_len);
            }
            MMK_CloseWin(win_id);
            break;

        default:
            break;
        } 
        
        break;
    

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}
#endif // MMI_VIDEO_PLAYER_MINI_SUPPORT //6530_MINI
/*****************************************************************************/
//  Description : SettingAsScreensaver
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
#ifdef SCREENSAVER_SUPPORT
LOCAL BOOLEAN SettingAsScreensaver(wchar *full_name_ptr, uint16 full_name_len)
{
    MMISET_SCREENSAVER_INFO_T ssaver_info = {0};    
    VP_SUPPORTED_STATUS_TYPE_E support_type = 0;

    //VP_STRACE_LOW:"[VP] SettingAsScreensaver full_name_ptr = %x, full_name_len = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_12413_112_2_18_3_5_54_525,(uint8*)"dd",full_name_ptr,full_name_len);

    if ((PNULL == full_name_ptr) || (0 == full_name_len))
    {
        return FALSE;
    }
#ifdef DRM_SUPPORT
    if (!MMIAPIDRM_CanDrmFileSetToWSPC(full_name_ptr,full_name_len, DRMFILE_SETTING_SCREENSAVER))        
    {
        //不可以设置此DRM文件
       	MMIPUB_OpenAlertWarningWin(TXT_DRM_NOT_SET_SCREENSAVER);
        return FALSE;
    }
#endif

    support_type =  MMIAPIVP_GetSupportedStatus(full_name_ptr, 
                                        full_name_len);

    if(VP_SUPPORTED_BOTH       == support_type ||
       VP_SUPPORTED_VIDEO_ONLY == support_type)
    {
        ssaver_info.is_open = TRUE;
        ssaver_info.ssaver_type = MMISET_SCREENSAVER_MORE_ANIMATION;
        ssaver_info.more_ani_saver_info.name_wstr_len = MIN(full_name_len,MMISET_SCREENSAVER_NAME_MAX_LEN);
        MMIAPICOM_Wstrncpy(ssaver_info.more_ani_saver_info.name_wstr,full_name_ptr,ssaver_info.more_ani_saver_info.name_wstr_len);
        
        MMIAPISET_SetScreenSaverInfo(&ssaver_info);
        MMIPUB_OpenAlertSuccessWin(TXT_SUCCESS);
        return TRUE;
    }
    else
    {
        MMIPUB_OpenAlertWarningWin(TXT_COMMON_NO_SUPPORT);
        return FALSE;
    }
    
    
}
#endif

#ifdef MMI_VIDEOWALLPAPER_SUPPORT
/*****************************************************************************/
//  Description : SettingAsScreensaver
//  Global resource dependence : 
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN SettingAsVideoWallpaper(wchar *full_name_ptr, uint16 full_name_len, uint32 file_size)
{  
    VP_SUPPORTED_STATUS_TYPE_E support_type = 0;

    //VP_STRACE_LOW:"[VP] SettingAsVideoWallpaper full_name_ptr = %x, full_name_len = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_12462_112_2_18_3_5_54_526,(uint8*)"dd",full_name_ptr,full_name_len);

    if ((PNULL == full_name_ptr) || (0 == full_name_len))
    {
        return FALSE;
    }

    if(!MMIAPIFMM_IsFileExist(full_name_ptr,full_name_len))
    {
        MMIPUB_OpenAlertWarningWin(TXT_COM_FILE_NO_EXIST);
        return FALSE;
    }
    else if (0 == file_size)
    {
        MMIPUB_OpenAlertWarningWin(TXT_EMPTY_FILE);
        return FALSE;
    }
#ifdef DRM_SUPPORT
    if (!MMIAPIDRM_CanDrmFileSetToWSPC(full_name_ptr,full_name_len, DRMFILE_SETTING_SCREENSAVER))        
    {
        //不可以设置此DRM文件
       	MMIPUB_OpenAlertWarningWin(TXT_DRM_NOT_SET_VIDEOWALLPAPER);
        return FALSE;
    }
#endif

    support_type =  MMIAPIVP_GetSupportedStatus(full_name_ptr, 
                                        full_name_len);

    if(VP_SUPPORTED_BOTH       == support_type ||
       VP_SUPPORTED_VIDEO_ONLY == support_type)
    {
        MMISET_WALLPAPER_SET_T      wallpaper_info  =   {0};//直接打开电影桌面生效
#ifdef MMI_POWER_SAVING_MODE_SUPPORT
        MMISET_POWER_SETTING_ITEM_T power_saving_setting =  MMIAPISET_GetPowerSavingSetting();
#endif
        MMIAPISETVWP_AddToVideoWallpaperList(full_name_ptr,full_name_len,file_size); 
#ifdef MMI_POWER_SAVING_MODE_SUPPORT
        if(!(power_saving_setting.is_fb_anim_wallpaper && power_saving_setting.is_active)
#ifndef MMI_VIDEO_PLAYER_MINI_LAYOUT		
            && (MMISET_WALLPAPER_VIDEO == wallpaper_info.wallpaper_type)
#endif			
			)

        {			
            SCI_MEMSET(&wallpaper_info, 0, sizeof(MMISET_WALLPAPER_SET_T));
            wallpaper_info.wallpaper_type = MMISET_WALLPAPER_VIDEO;
            wallpaper_info.wallpaper_id = (uint16)(MMISET_WALLPAPER_LIST_VIDEO + 1);
            MMIAPISET_SetWallpaperSetInfo(wallpaper_info);
            return TRUE;
        }
        else
        {
            MMIPUB_OpenAlertWarningWin(TXT_VP_VIDEO_WALLPAPER_POWERSAVING_ON);
            return FALSE;
        } 
#else
#ifndef MMI_VIDEO_PLAYER_MINI_LAYOUT
        if(MMISET_WALLPAPER_VIDEO == wallpaper_info.wallpaper_type)
#endif
        {		
            SCI_MEMSET(&wallpaper_info, 0, sizeof(MMISET_WALLPAPER_SET_T));
            wallpaper_info.wallpaper_type = MMISET_WALLPAPER_VIDEO;
            wallpaper_info.wallpaper_id = (uint16)(MMISET_WALLPAPER_LIST_VIDEO + 1);
            MMIAPISET_SetWallpaperSetInfo(wallpaper_info);
            return TRUE;
        }
#endif        
    }
    else
    {
        MMIPUB_OpenAlertWarningWin(TXT_COMMON_NO_SUPPORT);
        return FALSE;
    } 
}
#endif

/*****************************************************************************/
//  Description : SettingAsPowerOnOffAnimation
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN SettingAsPowerOnOffAnimation(BOOLEAN is_power_on, wchar *full_name_ptr, uint16 full_name_len)
{
    MMISET_POWER_ON_OFF_ANIM_INFO_T power_on_off_anim_info = {0};  
    VP_SUPPORTED_STATUS_TYPE_E support_type = 0;

    //VP_STRACE_LOW:"[VP] SettingAsPowerOnOffAnimation is_power_on = %d, full_name_ptr = %x,full_name_len = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_12525_112_2_18_3_5_55_527,(uint8*)"ddd",is_power_on,full_name_ptr,full_name_len);
    
    if (PNULL == full_name_ptr || 0 == full_name_len)
    {
        return FALSE;
    }

#ifdef DRM_SUPPORT
    if (!MMIAPIDRM_CanDrmFileSetToWSPC(full_name_ptr,full_name_len, DRMFILE_SETTING_POWERONOFFANIM))        
    {
        //不可以设置此DRM文件
       	MMIPUB_OpenAlertWarningWin(TXT_DRM_NOT_SET_POWERANIM);
        return FALSE;
    }
#endif

    support_type =  MMIAPIVP_GetSupportedStatus(full_name_ptr, 
                                        full_name_len);

    if(VP_SUPPORTED_BOTH       == support_type ||
       VP_SUPPORTED_VIDEO_ONLY == support_type)
    {
        power_on_off_anim_info.is_open                 = TRUE;
        power_on_off_anim_info.anim_type               = MMISET_POWER_ON_OFF_ANIM_MORE;
        power_on_off_anim_info.more_anim_info.name_wstr_len = MIN(full_name_len,MMISET_POWER_ON_OFF_ANIM_NAME_MAX_LEN);
        MMIAPICOM_Wstrncpy(power_on_off_anim_info.more_anim_info.name_wstr, full_name_ptr,power_on_off_anim_info.more_anim_info.name_wstr_len);  
        MMIAPISET_SetPowerOnOffAnimInfo(&power_on_off_anim_info, is_power_on);
        MMIPUB_OpenAlertSuccessWin(TXT_SUCCESS);
        return TRUE;
    }
    else
    {  
        MMIPUB_OpenAlertWarningWin(TXT_COMMON_NO_SUPPORT);
        return FALSE;
    }    
   
}

/*****************************************************************************/
//  Description : HandleFileOptionWinMsg
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleFileOptionWinMsg(
                                      MMI_WIN_ID_T win_id,
                                      MMI_MESSAGE_ID_E msg_id,
                                      DPARAM param
                                      )
{
    MMI_RESULT_E                        result = MMI_RESULT_TRUE;
    MMI_MENU_ID_T                        menu_id = 0;
    MMI_MENU_GROUP_ID_T                 group_id = 0;
    MMI_CTRL_ID_T                       ctrl_id = VP_MENU_FILE_OPT_CTRL_ID;
    switch(msg_id) 
    {
    case MSG_OPEN_WINDOW:
        MMK_SetAtvCtrl(win_id,ctrl_id);
        if (VP_ENTRY_FROM_PLAYER != s_video_player_control_info.vp_entry)
        {
            MMIAPICOM_EnableGrayed(win_id,MENU_VP_FILE_OPT,ID_VP_OPEN_LOCALLIST,TRUE);
        }
        break;

    case MSG_APP_WEB:
    case MSG_APP_OK:
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        GUIMENU_GetId(MMK_GetCtrlHandleByWin(win_id, ctrl_id),&group_id,&menu_id);
        //VP_STRACE_LOW:"[VP] HandleFileOptionWinMsg menu_id = %x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_12593_112_2_18_3_5_55_528,(uint8*)"d",menu_id);
        switch( menu_id )
        {
        case ID_VP_OPEN_LOCALFILE:
            OpenLocalFileWin(win_id);                       
            break;
#ifdef MMI_STREAMING_PLAYER_SUPPORT  //norflashcut
        case ID_VP_OPEN_NET:
            MMK_CreateWin((uint32 *)MMIVP_NET_WIN_TAB,PNULL);  
            MMK_CloseWin(win_id);            
            break;
#endif
        case ID_VP_OPEN_LOCALLIST:
            OpenLocalListWin();
            MMK_CloseWin(win_id);
            break;
        default:
            break;
        }
        break;


    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return result;
}

/*****************************************************************************/
//  Description : MMIAPIVP_IsOpenPlayerWin
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIVP_IsOpenPlayerWin(void)
{
    return MMK_IsOpenWin(MMIVP_PLAYER_WIN_ID);

}

#if 0//norflashcut
/*****************************************************************************/
//  Description : MMIAPIVP_IsFocusPlayerWin
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIVP_IsFocusPlayerWin(void)
{
    return MMK_IsFocusWin(MMIVP_PLAYER_WIN_ID);

}
#endif


/*****************************************************************************/
//  Description : MMIAPIVP_IsLiving
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIVP_IsLiving(void)
{
    BOOLEAN result = FALSE;
#ifdef MMI_STREAMING_PLAYER_SUPPORT
    if (VP_PLAY_FROM_NET == s_video_player_control_info.play_type
        && VP_MAX_INVALID_VALUE == s_video_player_control_info.total_time 
        && VP_STATE_READY != s_video_player_control_info.state)
    {
        result = TRUE;
    }
    //VP_STRACE_LOW:"[VP] MMIAPIVP_IsLiving result = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_12664_112_2_18_3_5_55_529,(uint8*)"d", result);
#endif
    return result;
}

/*****************************************************************************/
//  Description : MMIAPIVP_IsLiving
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIVP_IsPlayWinFocus(void)
{
    return MMK_IsFocusWin(MMIVP_MAIN_PLAY_WIN_ID);
}

/*****************************************************************************/
//  Description : 判断是否是否是有效的本地播放列表信息
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN CheckHistoryListItemIsHasValidLocalListInfo(VP_LOCALLIST_TYPE_E locallist_type,
                                                          int32 locallist_index, 
                                                          wchar * full_path_ptr
                                                          )
{    
    BOOLEAN             result      = FALSE;
    FILEARRAY_DATA_T    array_data  = {0};

   if (PNULL != full_path_ptr)
   { 
       if (MMIAPIFMM_IsFileExist(full_path_ptr,MMIAPICOM_Wstrlen(full_path_ptr)))
       {
           if(VP_LocalList_GetInfo(locallist_index,&array_data,locallist_type) && 0 == MMIAPICOM_Wstrcmp(full_path_ptr,array_data.filename))
           {
               result = TRUE;
           }
       }
       else
       {
           SCI_TRACE_LOW("[VP] CheckHistoryListItemIsHasValidLocalListInfo, file does not exists!");
           return FALSE;
       }
    
   }
   return result;

}
/*****************************************************************************/
//  Description : display player win soft key
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void DisplayVPWinSoftkey(BOOLEAN       is_refresh)
{
    MMI_TEXT_ID_T leftsoft_id = TXT_NULL;
    MMI_TEXT_ID_T middlesoft_id = TXT_NULL;
    MMI_TEXT_ID_T rightsoft_id = TXT_NULL;
    GUI_BG_T fg_info = {0};
#ifdef TOUCH_PANEL_SUPPORT //@andy.he_MP4PDA
   MMI_IMAGE_ID_T  impressed_id  = IMAGE_NULL;
#endif
    

    if (!IsAbleToFresh())
    {
        is_refresh = FALSE;
    }
    //VP_STRACE_LOW:"[VP] DisplayVPWinSoftkey s_video_player_control_info.total_time = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_12711_112_2_18_3_5_55_530,(uint8*)"d",s_video_player_control_info.total_time);
    
    if (!MMIAPIVP_IsLiving())
    {
        leftsoft_id = STXT_OPTION;
    }
    
    if (VP_STATE_READY == s_video_player_control_info.state)
    {
        rightsoft_id = STXT_EXIT;
    }
    else
    {
        rightsoft_id = STXT_STOP;
    }    
    

    if (MMITHEME_IsMainScreenLandscape())
    {
        fg_info.bg_type = GUI_BG_IMG;

        if (STXT_OPTION == leftsoft_id)
        {
            fg_info.img_id = IMAGE_COMMON_BTN_OPTION;
            GUIBUTTON_SetFg(VP_PLAY_PANEL_OPT_BUTTON_CTRL_ID,&fg_info);
            #ifdef TOUCH_PANEL_SUPPORT //@andy.he_MP4PDA
                fg_info.img_id = IMAGE_COMMON_BTN_OPTION_FOCUS;
                GUIBUTTON_SetPressedFg(VP_PLAY_PANEL_OPT_BUTTON_CTRL_ID,&fg_info);
            #else
                GUIBUTTON_SetPressedFg(VP_PLAY_PANEL_OPT_BUTTON_CTRL_ID,&fg_info);
            #endif
            GUIBUTTON_SetGrayedFg(VP_PLAY_PANEL_OPT_BUTTON_CTRL_ID,FALSE,PNULL,is_refresh);        
        }
        else
        {
#ifndef MMI_RES_LOW_COST            
            fg_info.img_id = IMAGE_COMMON_BTN_OPTION_GREY;
#endif            
            GUIBUTTON_SetGrayedFg(VP_PLAY_PANEL_OPT_BUTTON_CTRL_ID,TRUE,&fg_info,is_refresh);           
        }
        
        if (STXT_STOP == rightsoft_id)
        {
            fg_info.img_id = IMAGE_COMMON_BTN_STOP;
#ifdef TOUCH_PANEL_SUPPORT //@andy.he_MP4PDA
            impressed_id  = IMAGE_COMMON_BTN_STOP_FOCUS;
#endif
        }
        else
        {
            fg_info.img_id = IMAGE_COMMON_BTN_RETURN;
#ifdef TOUCH_PANEL_SUPPORT //@andy.he_MP4PDA
            impressed_id  = IMAGE_COMMON_BTN_RETURN_FOCUS;
#endif            
        }    
        GUIBUTTON_SetFg(VP_PLAY_PANEL_RETURN_BUTTON_CTRL_ID,&fg_info);
#ifdef TOUCH_PANEL_SUPPORT //@andy.he_MP4PDA
        fg_info.img_id = impressed_id;
        GUIBUTTON_SetPressedFg(VP_PLAY_PANEL_RETURN_BUTTON_CTRL_ID,&fg_info);       
#else
        GUIBUTTON_SetPressedFg(VP_PLAY_PANEL_RETURN_BUTTON_CTRL_ID,&fg_info);       
#endif
        if (is_refresh)
        {
            GUIBUTTON_Update(VP_PLAY_PANEL_RETURN_BUTTON_CTRL_ID);
        }
    }
    else
    {
        if(!MMIAPIVP_IsLiving())
        {
            if(0 != s_video_player_control_info.full_path_len && (VP_STATE_READY == s_video_player_control_info.state || VP_STATE_PAUSED == s_video_player_control_info.state))
            {
                middlesoft_id = TXT_COMMON_PLAY;
            }
            else if((VP_STATE_BUFFERING == s_video_player_control_info.state || VP_STATE_PLAYING == s_video_player_control_info.state))
            {
                middlesoft_id = TXT_PAUSE;
            }
        }
	
        GUIWIN_SetSoftkeyTextId(MMIVP_PLAYER_WIN_ID, leftsoft_id,middlesoft_id,rightsoft_id,is_refresh);  
    }
}

/*****************************************************************************/
//  Description : notify vp stop
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIVP_NotifyVPStop(void)
{
    if(MMK_IsOpenWin(MMIVP_PLAYER_WIN_ID)        
        && MMIAPIVP_IsLiving()
      )
    {
#ifdef MMI_VIDEOPLAYER_HISTORYLIST_SUPPORT 
        if (IsSaveLastPlayPoint())
        {
            VP_HistoryList_SetLastPlayPoint(s_video_player_control_info.process_time);
        }        
#endif //MMI_VIDEOPLAYER_HISTORYLIST_SUPPORT
        VideoPlayerStop(MMIVP_PLAYER_WIN_ID);  
    }
}

/*****************************************************************************/
//  Description : open local list win
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void OpenLocalListWin(void)
{
    
#ifdef MMI_VIDEO_PLAYER_MINI_SUPPORT //6530_MINI
    MMK_CreateWin((uint32 *)MMIVP_LOCALLIST_WIN_TAB,PNULL);
#else
    MMK_CreateParentWin((uint32 *)MMIVP_LOCALLIST_WIN_TAB,PNULL);
    MMK_CreateChildWin(MMIVP_LOCALLIST_WIN_ID, (uint32*)MMIVP_DEFAULT_LOCALLIST_WIN_TAB, NULL);
    MMK_CreateChildWin(MMIVP_LOCALLIST_WIN_ID, (uint32*)MMIVP_USER_LOCALLIST_WIN_TAB, NULL);
#endif   
    //@CR241504 2011.05.13
	//MMK_SetChildWinFocus(MMIVP_LOCALLIST_WIN_ID, MMIVP_DEFAULT_LOCALLIST_WIN_ID);
    //@CR241504 2011.05.13
	
}

/*****************************************************************************/
//  Description : IsSaveLastPlayPoint
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsSaveLastPlayPoint(void)
{
    if (VP_STATE_READY == s_video_player_control_info.state
        || VP_STATE_PLAY_PENDING == s_video_player_control_info.state
        || VP_STATE_STOPING == s_video_player_control_info.state)
    {
        return FALSE;
    }
#ifdef MMI_STREAMING_PLAYER_SUPPORT
    else if (MMIAPIVP_IsLiving()
        || s_video_player_control_info.process_time == s_video_player_control_info.total_time)
    {
        return FALSE;
    }
#endif
    else
    {
        return TRUE;
    }
}

/*****************************************************************************/
//  Description : IsOpenOtherWinEnable
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsOpenOtherWinEnable(void)
{
    if (MMIAPIVP_IsLiving())
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}
/*****************************************************************************/
//  Description : IsAdjustBrightEnable
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsAdjustBrightEnable(void)
{
    if (VP_STATE_READY != s_video_player_control_info.state
        && VP_STATE_STOPING != s_video_player_control_info.state)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }

}
/*****************************************************************************/
//  Description : IsSeekEnable
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsSeekEnable(void)
{       
   
    if(!MMIAPIVP_IsLiving()
        &&(VP_STATE_BUFFERING == s_video_player_control_info.state
        || VP_STATE_PLAYING == s_video_player_control_info.state
        ||VP_STATE_PAUSED == s_video_player_control_info.state))            
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}


/*****************************************************************************/
//  Description : 是否需要显示控制面板
//  Global resource dependence : 
//  Author: 
//  Note:  横屏情况非播放状态，以及竖屏情况下，需要显示控制面板
/*****************************************************************************/
LOCAL BOOLEAN IsNeedDisplayControlPanel(void)
{
    if (MMITHEME_IsMainScreenLandscape()
        && !s_video_player_control_info.is_display_win
       )
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
    
}
/*****************************************************************************/
//  Description : 用于是否可以显示全屏的相关信息
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsDisplayFullScreenReferInfoEnable(void)
{   
    BOOLEAN result = FALSE;
	if ((LCD_ANGLE_90 == GUILCD_GetInitAngle(GUI_MAIN_LCD_ID)) ||
        (LCD_ANGLE_270 == GUILCD_GetInitAngle(GUI_MAIN_LCD_ID)))
    {
        result = FALSE;
    }
    else
    {
        result = TRUE; 
    }
    return result;
    #if 0
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
#endif
}


/*****************************************************************************/
//  Description : 用于是否可以切换半全屏
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsSwitchFullScreenEnable(void)
{   

    BOOLEAN result = FALSE;

    /*the following two sentence:don't support MSG_KEYDOWN_2 when there is not frame image in the video area*/
    if(IsDisplayFullScreenReferInfoEnable())
    {
    /*
        if(VP_STATE_READY != s_video_player_control_info.state && VP_STATE_STOPING != s_video_player_control_info.state)
        {
            result = TRUE;
        }
        else
        {
            result = FALSE;
        }*/
        result=TRUE;
    }

    return result;
}

#ifdef MMI_STREAMING_PLAYER_SUPPORT
/*****************************************************************************/
//  Description : append sim name and wifi to the list
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void AppendSimNameToList(MMI_CTRL_ID_T ctrl_id)
{
#if defined(MMI_MULTI_SIM_SYS_SINGLE) || defined(MMI_WIFI_SUPPORT)
    MMI_STRING_T sim_str = {0};
    GUILIST_ITEM_T      item_t = {0};
    GUILIST_ITEM_DATA_T item_data = {0};
#endif

#ifdef MMI_WIFI_SUPPORT
    GUILIST_SetMaxItem(ctrl_id, MMI_DUAL_SYS_MAX + 1, FALSE );
#else
    GUILIST_SetMaxItem(ctrl_id, MMI_DUAL_SYS_MAX, FALSE );
#endif
    
#ifndef MMI_MULTI_SIM_SYS_SINGLE 
    MMIAPISET_AppendMultiSimName(ctrl_id, TXT_COMMON_OK, MMI_DUAL_SYS_MAX, MMISET_APPEND_SYS_OK);
#else
    item_t.item_style = GUIITEM_STYLE_ONE_LINE_RADIO;
    item_t.item_data_ptr = &item_data;
    item_t.user_data = MN_DUAL_SYS_1;

    MMI_GetLabelTextByLang(TXT_GPRS,&sim_str);
    
    item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[0].item_data.text_buffer.wstr_len = sim_str.wstr_len;
    
    item_data.item_content[0].item_data.text_buffer.wstr_ptr = sim_str.wstr_ptr;

    //set softkey
    item_data.softkey_id[0] = TXT_COMMON_OK;
    item_data.softkey_id[1] = TXT_NULL;
    item_data.softkey_id[2] = STXT_RETURN;
    
    GUILIST_AppendItem(ctrl_id, &item_t);
#endif

#ifdef MMI_WIFI_SUPPORT
    SCI_MEMSET(&item_t, 0x00, sizeof(item_t));
    SCI_MEMSET(&item_data, 0x00, sizeof(item_data));

    item_t.item_style = GUIITEM_STYLE_ONE_LINE_ICON_TEXT;
    item_t.item_data_ptr = &item_data;
    item_t.user_data = MMI_DUAL_SYS_MAX;//WIFI
    MMI_GetLabelTextByLang(TXT_WIFI,&sim_str);
    item_data.item_content[0].item_data_type = GUIITEM_DATA_IMAGE_ID;
    item_data.item_content[0].item_data.image_id = MAIN_GetIdleWifiIcon();
                              
    item_data.item_content[1].item_data_type    = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[1].item_data.text_buffer.wstr_len = sim_str.wstr_len;
    item_data.item_content[1].item_data.text_buffer.wstr_ptr = sim_str.wstr_ptr;
    //set softkey
    item_data.softkey_id[0] = TXT_COMMON_OK;
    item_data.softkey_id[1] = TXT_NULL;
    item_data.softkey_id[2] = STXT_RETURN;
    
    GUILIST_AppendItem(ctrl_id, &item_t);
#endif

    GUILIST_SetOwnSofterKey(ctrl_id, TRUE);
}

#if defined(MMI_WIFI_SUPPORT) || !defined(MMI_MULTI_SIM_SYS_SINGLE) 
/*****************************************************************************/
//  Description : handle sim select window message
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSIMSelectWinMsg(
                                           MMI_WIN_ID_T win_id, 
                                           MMI_MESSAGE_ID_E msg_id, 
                                           DPARAM param
                                         )
{
    MMI_RESULT_E        result          = MMI_RESULT_TRUE;
    uint16              list_index      = 0;
    MMI_CTRL_ID_T       ctrl_id         = VP_SELECT_SIM_LIST_CTRL_ID;
    MN_DUAL_SYS_E  dual_sys        = MN_DUAL_SYS_1;
    uint32 item_data = 0;

    //SCI_TRACE_LOW:"[VP] HandleSIMSelectWinMsg msg_id= 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_13070_112_2_18_3_5_56_531,(uint8*)"d", msg_id);

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
#ifndef MMI_WIFI_SUPPORT
        GUITITLE_SetTextId(win_id,MMITHEME_GetTitleCtrlId(),TXT_SIM_SEL,FALSE);
#else
        GUITITLE_SetTextId(win_id,MMITHEME_GetTitleCtrlId(),TXT_NETWORK_SELECT,FALSE);
#endif
#if !defined(MMI_MULTI_SIM_SYS_SINGLE) 
        MMIAPIPHONE_SetNeedUpdateNetworkStatusWin(win_id);
#endif
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;
        
    case MSG_FULL_PAINT:
        {
            uint16 cur_selection = 0;
            cur_selection = GUILIST_GetCurItemIndex(ctrl_id);
            //动态添加LIST ITEM   
            AppendSimNameToList(ctrl_id);
            GUILIST_SetCurItemIndex(ctrl_id, cur_selection);
        }
        break;

#if !defined(MMI_MULTI_SIM_SYS_SINGLE) 
    case MSG_GET_FOCUS:
        MMIAPIPHONE_SetNeedUpdateNetworkStatusWin(win_id);
        break;
#endif          
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_WEB:    
    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_CTL_MIDSK:
        list_index = GUILIST_GetCurItemIndex(ctrl_id);

        if(GUILIST_IsCurrItemInvalid(ctrl_id))
        {
            break;
        }
        
        GUILIST_GetItemData(ctrl_id, list_index, &item_data);

        if (item_data < MMI_DUAL_SYS_MAX)
        {
            dual_sys = (MN_DUAL_SYS_E)item_data;
        }
        else
        {
            dual_sys = MMI_DUAL_SYS_MAX;
        }

        s_video_player_control_info.dual_sys = dual_sys;
        s_video_player_control_info.to_do_type = VP_TO_DO_PLAY_FOR_FULL_PAINT;
        MMK_CloseWin(win_id);//返回上一级,需要player win先获得focus,禁止按键音等，然后开始播放 
        if(MMK_IsOpenWin( MMIVP_NET_WIN_ID)) //NEWMS00164417
        {
            MMK_CloseWin(MMIVP_NET_WIN_ID);
        }
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:        
        s_video_player_control_info.to_do_type = VP_TO_DO_STOP;
        MMK_CloseWin(win_id);//返回上一级
        break;   

    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return result;
}
#endif
#endif

/*****************************************************************************/
//  Description : 转屏
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
LOCAL void HandleSwitchScreenMsg(void)
{
	if (IsSwitchFullScreenEnable())
    {                    
        StopPlayerAllTimer();   
		SCI_TRACE_LOW("[VP] HandleSwitchScreenMsg s_video_player_control_info.cur_angle = %d",s_video_player_control_info.cur_angle);
        switch(s_video_player_control_info.cur_angle) 
        {
        case LCD_ANGLE_0:
            SetVpWinAngle(LCD_ANGLE_90);               
            break;
        case LCD_ANGLE_90:
            SetVpWinAngle(LCD_ANGLE_0);                
            break;
        case LCD_ANGLE_180:
            SetVpWinAngle(LCD_ANGLE_270);               
            break;
        case LCD_ANGLE_270:
            SetVpWinAngle(LCD_ANGLE_180);            
            break;
        default:
            break;
        }          
              
    }
}
#ifdef TOUCH_PANEL_SUPPORT  /*pclint-650*/
/*****************************************************************************/
//  Description : double触笔处理
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
LOCAL void HandleTPDoubleMsg(DPARAM  param)
{
	GUI_POINT_T  point = {0}; 
    
    point.x = MMK_GET_TP_X(param);
    point.y = MMK_GET_TP_Y(param);    
    
	  
    VP_STRACE_LOW("[VP] HandleTPDoubleMsg x= %d, y = %d, rect.left = %d,right = %d,top = %d,bottom = %d",point.x,point.y,s_tp_down_video_rect.left,s_tp_down_video_rect.right,s_tp_down_video_rect.top,s_tp_down_video_rect.bottom);
    
    if (GUI_PointIsInRect(point,s_tp_down_video_rect))
	{
		VP_STRACE_LOW("[VP] HandleTPDoubleMsg switch screen");
		HandleSwitchScreenMsg();
	}
}

/*****************************************************************************/
//  Description : down触笔处理
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
LOCAL void HandleTPDownMsg(DPARAM  param)
{
    GUI_POINT_T point = {0}; 
    GUI_RECT_T   rect = {0};
    
    point.x = MMK_GET_TP_X(param);
    point.y = MMK_GET_TP_Y(param);    
    
    GUIOWNDRAW_GetDisplayRect(VP_PLAY_PANEL_PROCESS_OWNDRAW_CTRL_ID,&rect);
    //VP_STRACE_LOW:"[VP] HandleTPDownMsg x= %d, y = %d, rect.left = %d,right = %d,top = %d,bottom = %d, s_tp_down_ctrl_id = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_13163_112_2_18_3_5_56_532,(uint8*)"ddddddd",point.x,point.y,rect.left,rect.right,rect.top,rect.bottom,s_tp_down_ctrl_id);
    
    if (GUI_PointIsInRect(point,rect))
    {
        s_tp_down_ctrl_id = VP_PLAY_PANEL_PROCESS_OWNDRAW_CTRL_ID;
        //VP_STRACE_LOW:"[VP] HandleTPDownMsg s_tp_down_ctrl_id = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_13168_112_2_18_3_5_56_533,(uint8*)"d",s_tp_down_ctrl_id);
        OpenSeekWin(MMIVP_PLAYER_WIN_ID,point,FALSE);
    }     
    else
    {
        s_tp_down_ctrl_id = 0;
    }
}

/*****************************************************************************/
//  Description : up触笔处理
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
LOCAL void HandleTPUpMsg(DPARAM  param)
{
    GUI_POINT_T point = {0};   
    MMI_MESSAGE_ID_E msg_id  = 0;
    MMI_WIN_ID_T win_id = MMIVP_PLAYER_WIN_ID;


    point.x = MMK_GET_TP_X(param);
    point.y = MMK_GET_TP_Y(param);
    
    //VP_STRACE_LOW:"[VP] HandleTPUpMsg s_tp_down_ctrl_id = %d, point.x = %d, y= %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_13191_112_2_18_3_5_56_534,(uint8*)"ddd",s_tp_down_ctrl_id,point.x, point.y);
    switch(s_tp_down_ctrl_id) 
    {
    case VP_PLAY_PANEL_PRE_BUTTON_CTRL_ID:
        msg_id = MSG_KEYPRESSUP_LEFT;        
    	break;

    case VP_PLAY_PANEL_NEXT_BUTTON_CTRL_ID:
        msg_id = MSG_KEYPRESSUP_RIGHT;               
        
        break;

    case VP_PLAY_PANEL_PROCESS_OWNDRAW_CTRL_ID:
        OpenSeekWin(MMIVP_PLAYER_WIN_ID,point,TRUE);
        break;
        
    default:
        break;
    }

    if (0 != msg_id)
    {
        MMK_SendMsg(win_id, msg_id,PNULL);    
    }  
    s_tp_down_ctrl_id = 0;
}

/*****************************************************************************/
//  Description : 触笔处理
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
LOCAL void HandleTPMoveMsg(DPARAM  param)
{
    GUI_POINT_T point = {0};    
    point.x = MMK_GET_TP_X(param);
    point.y = MMK_GET_TP_Y(param);

    if (s_tp_down_ctrl_id == VP_PLAY_PANEL_PROCESS_OWNDRAW_CTRL_ID)
    {
        OpenSeekWin(MMIVP_PLAYER_WIN_ID,point,FALSE);
    }   
}

/*****************************************************************************/
//  Description : 触笔处理
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
LOCAL void HandleCtrlTpLongMsg(DPARAM param)
{
    MMI_CTRL_ID_T ctrl_id = 0;
    MMI_WIN_ID_T win_id = MMIVP_PLAYER_WIN_ID;
    MMI_MESSAGE_ID_E msg_id = 0;

    if (PNULL == param)
    {
        return;
    }

    ctrl_id = ((MMI_NOTIFY_T*) param)->src_id;  
    //VP_STRACE_LOW:"[VP] HandleCtrlTpLongMsg ctrl_id = %d, s_tp_down_ctrl_id = %D"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_13250_112_2_18_3_5_56_535,(uint8*)"dd",ctrl_id,s_tp_down_ctrl_id);
   

    switch(ctrl_id) 
    {
    case VP_PLAY_PANEL_PRE_BUTTON_CTRL_ID:
        msg_id = MSG_KEYLONG_LEFT;        
    	break;

    case VP_PLAY_PANEL_NEXT_BUTTON_CTRL_ID:
        msg_id = MSG_KEYLONG_RIGHT;        
        break;

    default:        
        break;
    }

    if (0 != msg_id)
    {
        s_tp_down_ctrl_id = ctrl_id;
        MMK_SendMsg(win_id,msg_id,PNULL);
    }
}
#endif //TOUCH_PANEL_SUPPORT /*pclint-650*/
/*****************************************************************************/
//  Description : 设置播放器角度,禁止旋转
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
LOCAL void SetVpWinAngle(LCD_ANGLE_E angle)
{
    MMI_WIN_ID_T win_id = MMIVP_PLAYER_WIN_ID;
    WINDOW_SUPPORT_ANGLE support_angel = WIN_SUPPORT_ANGLE_ALL;
    
    switch( angle )
    {
    case LCD_ANGLE_0:
        support_angel = WIN_SUPPORT_ANGLE_0;
        break;
    case LCD_ANGLE_90:
        support_angel = WIN_SUPPORT_ANGLE_90;
        break;
    case LCD_ANGLE_180:
        support_angel = WIN_SUPPORT_ANGLE_180;
        break;
    case LCD_ANGLE_270:
        support_angel = WIN_SUPPORT_ANGLE_270;
        break;
    default:
        break;
    }
    MMK_SetWinSupportAngle(win_id,support_angel);
    MMK_SetWinAngle(win_id,angle,TRUE);
}

/*****************************************************************************/
//  Description :设置VPWin支持所有角度窗口显示 
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/

PUBLIC void MMIAPIVP_SetVPWinSupportAllAngle(void)
{
    MMK_SetWinSupportAngle(MMIVP_PLAYER_WIN_ID,WIN_SUPPORT_ANGLE_ALL);
}

/*****************************************************************************/
//  Description : 触笔处理
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
LOCAL void HandleCtrlTpMsg(DPARAM param)
{
    MMI_CTRL_ID_T ctrl_id = 0;
    MMI_WIN_ID_T win_id = MMIVP_PLAYER_WIN_ID;
    MMI_MESSAGE_ID_E msg_id = 0;
   
    if (PNULL == param)
    {
        return;
    }

    ctrl_id = ((MMI_NOTIFY_T*) param)->src_id;  
    //VP_STRACE_LOW:"[VP] HandleCtrlTpMsg ctrl_id = %d, s_tp_down_ctrl_id = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_13319_112_2_18_3_5_56_536,(uint8*)"dd",ctrl_id,s_tp_down_ctrl_id);
    
    
    switch(ctrl_id) 
    {
    case VP_PLAY_PANEL_VIDEO_OWNDRAW_CTRL_ID:
        if (s_video_player_control_info.is_display_win)
        {
            if (MMITHEME_IsMainScreenLandscape()
                && VP_STATE_PLAYING == s_video_player_control_info.state)
            {
                StopVPWinDisplayerTimer();
                DisappearVPWin(MMIVP_PLAYER_WIN_ID,TRUE,TRUE);
            }

        }    
        break;

    case VP_PLAY_PANEL_VOL_BUTTON_CTRL_ID:
        MMIAPIVP_TurnVol(MMICOM_PANEL_OPEN_NONE);              
        break;

    case VP_PLAY_PANEL_PRE_BUTTON_CTRL_ID:        
        msg_id = MSG_KEYUP_LEFT;                 
        break;

    case VP_PLAY_PANEL_PLAY_BUTTON_CTRL_ID:
        msg_id = MSG_APP_WEB;   
        break;

    case VP_PLAY_PANEL_NEXT_BUTTON_CTRL_ID:        
        msg_id = MSG_KEYUP_RIGHT;         
        break;

    case VP_PLAY_PANEL_FULLSCREEN_BUTTON_CTRL_ID:  
        msg_id = MSG_KEYDOWN_2;                       
        break;

    case VP_PLAY_PANEL_FILE_BUTTON_CTRL_ID:
#if defined MMI_PDA_SUPPORT && defined(MMI_ISTYLE_SUPPORT)
        if (MMITHEME_IsIstyle() && !MMITHEME_IsMainScreenLandscape())
        {
            msg_id = MSG_APP_MENU;
        }
        else
#endif
        {
            msg_id = MSG_VP_FILE_OPTION;
        }
        break;

    case VP_PLAY_PANEL_RANDOM_BUTTON_CTRL_ID:
        msg_id = MSG_VP_RANDOM_SET;      
        break;

    case VP_PLAY_PANEL_REPEAT_BUTTON_CTRL_ID:
        msg_id = MSG_VP_REPEAT_SET;
        break;

    case VP_PLAY_PANEL_OPT_BUTTON_CTRL_ID:
        msg_id = MSG_APP_OK;
        break;

    case VP_PLAY_PANEL_RETURN_BUTTON_CTRL_ID:
        msg_id = MSG_APP_CANCEL;
        break;

    case VP_PLAY_PANEL_PROCESS_OWNDRAW_CTRL_ID:
        break;

    default:
        if (MMITHEME_IsMainScreenLandscape()
            && VP_STATE_PLAYING == s_video_player_control_info.state
            && s_video_player_control_info.is_display_win)
        {
            StopVPWinDisplayerTimer();
            DisappearVPWin(MMIVP_PLAYER_WIN_ID,TRUE,TRUE);
        }
        break;
    }

    if (0 != msg_id)
    {
       // s_tp_down_ctrl_id = ctrl_id;
        s_tp_down_ctrl_id = 0;
        MMK_SendMsg(win_id,msg_id,PNULL);
    }
        
}
#ifdef TOUCH_PANEL_SUPPORT /*pclint-650*/
/*****************************************************************************/
//  Description : 触笔处理
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
LOCAL void HandleTPMsg(MMI_MESSAGE_ID_E msg_id, DPARAM  param)
{   
    //VP_STRACE_LOW:"[VP] HandleTPMsg msg_id = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_13414_112_2_18_3_5_56_537,(uint8*)"d",msg_id);
    

    switch(msg_id) 
    {
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_DOWN:     
        s_tp_down_video_rect = MMITHEME_GetFullScreenRect();
        if (s_video_player_control_info.is_display_win)
        {
            GUIOWNDRAW_GetDisplayRect(VP_PLAY_PANEL_VIDEO_OWNDRAW_CTRL_ID,&s_tp_down_video_rect);
        } 

    VP_STRACE_LOW("[VP] HandleTPMsg rect.left = %d,right = %d,top = %d,bottom = %d",s_tp_down_video_rect.left,s_tp_down_video_rect.right,s_tp_down_video_rect.top,s_tp_down_video_rect.bottom);
    StopVPWinDisplayerTimer();         
    if (IsNeedDisplayControlPanel())
    {
        DisappearVPWin(MMIVP_PLAYER_WIN_ID,FALSE,TRUE);                                  
    }
    else
    {           
        HandleTPDownMsg(param);   
    }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_UP:     
        StartVPWinDisplayerTimer();       
        HandleTPUpMsg(param);   
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_MOVE:
        HandleTPMoveMsg(param);
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
        HandleCtrlTpMsg(param);
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENLONGOK:
        HandleCtrlTpLongMsg(param);
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_DOUBLE:
#ifndef MMI_VIDEO_PLAYER_MINI_LAYOUT//6530 MINI 界面布局 不转屏
        HandleTPDoubleMsg(param);
#endif
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

    default:         
        break;
    }
}
#endif //end of TOUCH_PANEL_SUPPORT /*pclint-650*/
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

    //VP_STRACE_LOW:"[VP] MMIAPIVP_FillRectWithTransparenceColor before x= %d, y=%d,width = %d, height = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_13482_112_2_18_3_5_56_538,(uint8*)"dddd",*x_offset_ptr,*y_offset_ptr,*width_ptr,*height_ptr);
        
    *width_ptr = *width_ptr - MMIVP_GetAlign4Offset(*x_offset_ptr);        
    *width_ptr = *width_ptr - (*width_ptr % 4);
    *x_offset_ptr = *x_offset_ptr + MMIVP_GetAlign4Offset(*x_offset_ptr);
    *height_ptr = *height_ptr - MMIVP_GetAlign4Offset(*y_offset_ptr);
    *height_ptr = *height_ptr - (*height_ptr % 4);    
    *y_offset_ptr = *y_offset_ptr + MMIVP_GetAlign4Offset(*y_offset_ptr);
        
	//VP_STRACE_LOW:"[VP] MMIAPIVP_FillRectWithTransparenceColor after x= %d, y=%d,width = %d, height = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_13495_112_2_18_3_5_56_539,(uint8*)"dddd",*x_offset_ptr,*y_offset_ptr,*width_ptr,*height_ptr);
    
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
        GUI_FillRect(lcd_dev_info_ptr, lcd_rect, MMIAPIVP_GetTransparenceColor()); 
    }
    *width_ptr = *width_ptr - MMIVP_GetAlign4Offset(*x_offset_ptr);        
    *width_ptr = *width_ptr - (*width_ptr % 4);
    *x_offset_ptr = *x_offset_ptr + MMIVP_GetAlign4Offset(*x_offset_ptr);
    *height_ptr = *height_ptr - MMIVP_GetAlign4Offset(*y_offset_ptr);
    *height_ptr = *height_ptr - (*height_ptr % 4);    
    *y_offset_ptr = *y_offset_ptr + MMIVP_GetAlign4Offset(*y_offset_ptr);
        
	//VP_STRACE_LOW:"[VP] MMIAPIVP_FillRectWithTransparenceColor after x= %d, y=%d,width = %d, height = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_13495_112_2_18_3_5_56_539,(uint8*)"dddd",*x_offset_ptr,*y_offset_ptr,*width_ptr,*height_ptr);
    
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
  

    //VP_STRACE_LOW:"[VP] MMIAPIVP_FillRectWithTransparenceColor after x= %d, y=%d,width = %d, height = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_13554_112_2_18_3_5_57_540,(uint8*)"dddd",*x_offset_ptr,*y_offset_ptr,*width_ptr,*height_ptr);

    return TRUE;
}

/*****************************************************************************/
//  Description : 填充透明色
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIVP_TranslateDisplayParamLogicToPhysical(VP_DISPLAY_PARAM_T *disp_para, BOOLEAN is_need_fill)
{
    GUI_LCD_DEV_INFO const* lcd_dev_info_ptr = (const GUI_LCD_DEV_INFO*)MMITHEME_GetDefaultLcdDev();
    GUILCD_COORDINATE_PARAM_T logic_coordinate = {0};
    GUILCD_COORDINATE_PARAM_T physical_coordinate = {0};

    logic_coordinate.angle = disp_para->RotateAngle;
    logic_coordinate.lcd_height = disp_para->disp_rect.dy;
    logic_coordinate.lcd_width = disp_para->disp_rect.dx;

    MMIAPIVP_FillRectWithTransparenceColor(&(disp_para->disp_rect.dx), &(disp_para->disp_rect.dy), &(disp_para->disp_rect.x), &(disp_para->disp_rect.y), disp_para->RotateAngle, is_need_fill);
    MMIAPIVP_FillRectWithTransparenceColor(&(disp_para->target_rect.dx), &(disp_para->target_rect.dy), &(disp_para->target_rect.x), &(disp_para->target_rect.y), disp_para->RotateAngle, is_need_fill);
    GUILCD_ConvertLogicToPhysicalCoordinate(lcd_dev_info_ptr->lcd_id,
        &logic_coordinate,
        &physical_coordinate);
    
    disp_para->RotateAngle = physical_coordinate.angle;
    return TRUE;

}


/*****************************************************************************/
//  Description : IsAbleToFresh
//  Global resource dependence : 
//  Author: applezhang
//  Note: 处于focus状态，且控制面板显示的情况下可以刷新
/*****************************************************************************/
LOCAL BOOLEAN IsAbleToFresh(void)
{
    if (s_video_player_control_info.is_display_win
        && MMK_IsFocusWin(MMIVP_PLAYER_WIN_ID)
        && s_is_full_paint)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*****************************************************************************/
//  Description : MMIAPIVP_GetTransparenceColor
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
PUBLIC GUI_COLOR_T MMIAPIVP_GetTransparenceColor(void)
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
//  Description : bt play next file
//  Global resource dependence : none
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
PUBLIC void VP_EventNext(void)
{
    //SCI_TRACE_LOW:"[VP] VP_EventNext enter"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_13625_112_2_18_3_5_57_541,(uint8*)"");
    MMK_SendMsg(MMIVP_PLAYER_WIN_ID, MSG_KEYDOWN_RIGHT, PNULL);
    MMK_SendMsg(MMIVP_PLAYER_WIN_ID, MSG_KEYUP_RIGHT, PNULL);
}

/*****************************************************************************/
//  Description : bt play pre file
//  Global resource dependence : none
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
PUBLIC void VP_EventPrev(void)
{
    //SCI_TRACE_LOW:"[VP] VP_EventPrev enter"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_13636_112_2_18_3_5_57_542,(uint8*)"");
    MMK_SendMsg(MMIVP_PLAYER_WIN_ID, MSG_KEYDOWN_LEFT, PNULL);
    MMK_SendMsg(MMIVP_PLAYER_WIN_ID, MSG_KEYUP_LEFT, PNULL);
}

/*****************************************************************************/
//  Description : earphone notify video player play or pause 
//  Global resource dependence : none
//  Author: apple.zhang
//  Note: 耳机端发起播放/暂停/恢复通知手机端
/*****************************************************************************/
PUBLIC void VP_EventPlayOrPause(void)
{ 
    //SCI_TRACE_LOW:"[VP] VP_EventPlayOrPause enter"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_13647_112_2_18_3_5_57_543,(uint8*)"");
    MMK_SendMsg(MMIVP_PLAYER_WIN_ID, MSG_APP_WEB, PNULL);    
}


#if 0//norflashcut
/*****************************************************************************/
//  Description : MMIAPIVP_HandleFileChange
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIVP_HandleFileChange(void)
{
    //SCI_TRACE_LOW:"[VP] MMIAPIVP_HandleFileChange s_video_player_control_info.play_type = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_13660_112_2_18_3_5_57_544,(uint8*)"d",s_video_player_control_info.play_type);
     if(MMK_IsOpenWin(MMIVP_PLAYER_WIN_ID)        
      && VP_PLAY_FROM_NET != s_video_player_control_info.play_type
      )
    {       
        VideoPlayerStop(MMIVP_PLAYER_WIN_ID);  
        s_video_player_control_info.total_time = 0;
    }
    
}
#endif

//add CR233033
/*****************************************************************************/
//  Description : get full path name of playing file
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIVP_GetPlayingFilename(wchar * full_path_name_ptr)
{
    if(VP_STATE_PLAYING == s_video_player_control_info.state || VP_STATE_PAUSED == s_video_player_control_info.state)
    {
        MMIAPICOM_Wstrncpy( full_path_name_ptr,
                                                s_video_player_control_info.full_path_name,s_video_player_control_info.full_path_len);
        //SCI_TRACE_LOW:"[VP]MMIAPIVP_GetPlayingFilename playingname = %x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_13683_112_2_18_3_5_57_545,(uint8*)"d",full_path_name_ptr );
    }

}

 #ifdef  VIDEOTHUMBNAIL_SUPPORT  

/*****************************************************************************/
//  Description : IsAbleToFresh
//  Global resource dependence : 
//  Author: applezhang
//  Note: 处于focus状态，且当前 状态满足 
/*****************************************************************************/
LOCAL BOOLEAN IsAbleToGetThumbnail(void)
{
    if(MMK_IsFocusWin(MMIVP_PLAYER_WIN_ID)
        && (VP_TO_DO_NONE == s_video_player_control_info.to_do_type
         &&  VP_PLAY_FROM_NET != s_video_player_control_info.play_type &&  VP_PLAY_FROM_BUFFER != s_video_player_control_info.play_type))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
 
/*****************************************************************************/
//  Description :视频播放器载入显示祯功能，横屏时，创建横屏来显示图像，实现叠加效果 
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL  BOOLEAN  CreateLandscapeThumbnailLayer(void)
{
    BOOLEAN result = FALSE;
    UILAYER_RESULT_E layer_result  = UILAYER_RESULT_SUCCESS;
    uint16 lcd_width = 0;
    uint16 lcd_height = 0;
    UILAYER_CREATE_T create_info = {0}; 
    UILAYER_APPEND_BLT_T append_layer = {0};

     //SCI_TRACE_LOW:"[VP]:CreateLandscapeThumbnailLayer "
     SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_13702_112_2_18_3_5_57_546,(uint8*)"" );
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);
    if ((UILAYER_HANDLE_MULTI == UILAYER_GetHandleType(&s_thumbnail_layer))
        && UILAYER_IsLayerActive(&s_thumbnail_layer))    
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
        
        layer_result = UILAYER_CreateLayer(&create_info, &s_thumbnail_layer);

        if(UILAYER_RESULT_SUCCESS == layer_result)
        {
            //SCI_TRACE_LOW:"[VP]:CreateLandscapeThumbnailLayer success "
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_13726_112_2_18_3_5_57_547,(uint8*)"" );
            UILAYER_Clear(&s_thumbnail_layer);
            append_layer.lcd_dev_info = s_thumbnail_layer;
            append_layer.layer_level = UILAYER_LEVEL_LOW;
            UILAYER_AppendBltLayer(&append_layer);   
            result = TRUE;
        }
        else
        {
            result = FALSE;
        }
    }
    
    return result;
}


/*****************************************************************************/
//  Description : 销毁横屏的缩略图显示层 ，并重置主层的参数
//  Global resource dependence : 
//  Author:  
//  Note: 
/*****************************************************************************/
LOCAL void DestroyLandscapeThumbnailLayer(void)
{
    //SCI_TRACE_LOW:"[VP]:DestroyLandscapeThumbnailLayer  "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_13749_112_2_18_3_5_57_548,(uint8*)"" );

    //UILAYER_RestoreMainLayer();
    //UILAYER_RemoveBltLayer(&(s_thumbnail_layer));
    UILAYER_SetLayerColorKey((GUI_LCD_DEV_INFO *)MMITHEME_GetDefaultLcdDev(), FALSE, MMI_BLACK_COLOR);
    UILAYER_SetLayerAlpha((GUI_LCD_DEV_INFO *)MMITHEME_GetDefaultLcdDev(), 0xFF);
    UILAYER_RELEASELAYER(&(s_thumbnail_layer));   /*lint !e506 !e774*/
  //  SCI_MEMSET(&s_thumbnail_layer, 0x00, sizeof(GUI_LCD_DEV_INFO));
}
/*****************************************************************************/
//  Description :  重置主层的参数
//  Global resource dependence : 
//  Author:  
//  Note: 
/*****************************************************************************/
LOCAL void VPThumbnailRestoreMainLayer(void)
{
    if (MMITHEME_IsMainScreenLandscape())
    {
         if(UILAYER_IsBltLayer(&s_thumbnail_layer))
       {
            //SCI_TRACE_LOW:"[VP]:VPThumbnailResetMainLayer  "
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_13768_112_2_18_3_5_57_549,(uint8*)"" );
            UILAYER_SetLayerColorKey((GUI_LCD_DEV_INFO *)MMITHEME_GetDefaultLcdDev(), FALSE, MMI_BLACK_COLOR);
            UILAYER_SetLayerAlpha((GUI_LCD_DEV_INFO *)MMITHEME_GetDefaultLcdDev(), 0xFF);
        }
    }
 }
/*****************************************************************************/
//  Description :  设置主层的参数，实现叠加效果
//  Global resource dependence : 
//  Author:  
//  Note: 
/*****************************************************************************/
LOCAL void VPThumbnailSetMainLayer(void)
{
    if (MMITHEME_IsMainScreenLandscape())
    {
         if(UILAYER_IsBltLayer(&s_thumbnail_layer))
       {
            //SCI_TRACE_LOW:"[VP]:VPThumbnailSetMainLayer  "
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_13784_112_2_18_3_5_57_550,(uint8*)"" );
            UILAYER_SetLayerColorKey((GUI_LCD_DEV_INFO *)MMITHEME_GetDefaultLcdDev(), TRUE, MMI_BLACK_COLOR);
            UILAYER_SetLayerAlpha((GUI_LCD_DEV_INFO *)MMITHEME_GetDefaultLcdDev(), VP_HALF_TRANSPARENCE);
        }
    }
}
/*****************************************************************************/
//  Description :  设置主层的参数，实现叠加效果
//  Global resource dependence : 
//  Author:  
//  Note: 
/*****************************************************************************/
LOCAL void VPThumbnailSetDPLAYERParam(void)
{
    VP_DISPLAY_PARAM_T  display_param={0};
    GUI_RECT_T  display_rect={0};
    display_param.audioSupport =0;
    display_param.videoSupport = 1;
    display_param.b_support_seek = 1;
    display_param.disp_mode = DPLAYER_DISP_FULLVIEW;    
    display_param.RotateAngle = 0;//GUILCD_GetLogicAngle(GUI_MAIN_LCD_ID);//s_video_player_control_info.cur_angle; 

    //SCI_TRACE_LOW:"[VP]:VPThumbnailSetDPLAYERParam   RotateAngle:%d,s_video_player_control_info.cur_angle:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_13804_112_2_18_3_5_57_551,(uint8*)"dd",display_param.RotateAngle,s_video_player_control_info.cur_angle);

    if (MMITHEME_IsMainScreenLandscape())
    {
        display_rect = MMITHEME_GetFullScreenRect();    
    }
    else
    {   
        GUIOWNDRAW_GetDisplayRect(VP_PLAY_PANEL_VIDEO_OWNDRAW_CTRL_ID,&display_rect);      
    }
    
    display_param.disp_rect.dy = display_rect.bottom - display_rect.top + 1;
    display_param.disp_rect.dx = display_rect.right - display_rect.left + 1;       
    display_param.disp_rect.x = display_rect.left;
    display_param.disp_rect.y = display_rect.top;   

    display_param.target_rect.x = display_param.disp_rect.x;
    display_param.target_rect.y = display_param.disp_rect.y;
    display_param.target_rect.dx = display_param.disp_rect.dx;
    display_param.target_rect.dy = display_param.disp_rect.dy;

    MMIAPIVP_TranslateDisplayParamLogicToPhysical(&display_param, FALSE);
    MMIAPIVP_ThumbnailSetDPLAYERParam(&display_param);
}


/*****************************************************************************/
//  Description : 获取缩略图成功的UI 回调函数，主要是显示得到的缩略图
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN DisplayThumbnail(DPLAYER_IMAGE_DATA_T *image_data)
{
    UILAYER_COPYBUF_PARAM_T copy_param={0};
    UILAYER_COPYBUF_T copy_buf={0};
    int16  image_width=0;
    int16  image_height=0;
    BOOLEAN result=FALSE;
    VP_STRACE_LOW("[VP]:DisplayThumbnail");


     if(s_vp_dphandle_getthumbnail.is_getthumbnail_by_dpHandle)
    {
        VPThumbnailResetDPLAYERParam();
        return TRUE;
    }
    
    if(image_data == PNULL ||image_data->coordinate.trim_w==0 ||image_data->coordinate.trim_h==0)
    {
       MMIAPIVP_ThumbnailClose();//获取异常  
        //SCI_TRACE_LOW("[VP]:DisplayThumbnail  error image data ");
        //MMIAPIVP_ThumbnailVideoAdd(s_video_player_control_info.full_path_name);
        //MMIAPIVP_ThumbnailSetAngle(GUILCD_GetLogicAngle(GUI_MAIN_LCD_ID));
        //VPThumbnailSetDPLAYERParam();
        return FALSE;
    }
    if(!MMK_IsFocusWin(MMIVP_PLAYER_WIN_ID))//&& !MMK_IsFocusWin(MMIVP_OPT_MENU_WIN_ID) 
    {
         MMIAPIVP_ThumbnailClose();
        return FALSE;
    }
    if(MMIAPIVP_ThumbnailGetAngle() != GUILCD_GetLogicAngle(GUI_MAIN_LCD_ID))//异步获取的角度不正确，重新获取
    {
        //SCI_TRACE_LOW:"[VP]:DisplayThumbnail MMIAPIVP_ThumbnailGetAngle !=GUILCD_GetLogicAngle "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_13857_112_2_18_3_5_57_552,(uint8*)"");
        //MMIAPIVP_ThumbnailClose();
        //NotifyDPLAYERGetTumbnail();
        //MMK_PostMsg(MMIVP_PLAYER_WIN_ID, MSG_FULL_PAINT, PNULL, NULL);
        MMIAPIVP_ThumbnailVideoAdd(s_video_player_control_info.full_path_name);
        MMIAPIVP_ThumbnailSetAngle(GUILCD_GetLogicAngle(GUI_MAIN_LCD_ID));
        VPThumbnailSetDPLAYERParam();
        return FALSE;
    }
    if(MMIAPICOM_Wstrcmp(MMIAPIVP_ThumbnailGetVideo(),s_video_player_control_info.full_path_name ) != 0)
    {
                //SCI_TRACE_LOW:"[VP]:DisplayThumbnail MMIAPIVP_ThumbnailGetAngle !=GUILCD_GetLogicAngle "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_13857_112_2_18_3_5_57_552,(uint8*)"");
        //MMIAPIVP_ThumbnailClose();
        //NotifyDPLAYERGetTumbnail();
        //MMK_PostMsg(MMIVP_PLAYER_WIN_ID, MSG_FULL_PAINT, PNULL, NULL);
        MMIAPIVP_ThumbnailVideoAdd(s_video_player_control_info.full_path_name);
        MMIAPIVP_ThumbnailSetAngle(GUILCD_GetLogicAngle(GUI_MAIN_LCD_ID));
        VPThumbnailSetDPLAYERParam();
        return FALSE;
    }

    //SCI_TRACE_LOW:"[VP]:DisplayThumbnail   image_data->coordinate.w:%d,image_data->coordinate.h:%d,image_data->coordinate.trim_x:%d,image_data->coordinate.trim_y:%d,image_data->coordinate.trim_w:%d,image_data->coordinate.trim_h:%d ,rotate:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_13868_112_2_18_3_5_57_553,(uint8*)"ddddddd",image_data->coordinate.w,image_data->coordinate.h,image_data->coordinate.trim_x,image_data->coordinate.trim_y,image_data->coordinate.trim_w,image_data->coordinate.trim_h,image_data->coordinate.rotate_angle);
    image_width=image_data->coordinate.trim_w;
    image_height=image_data->coordinate.trim_h;
    if (MMITHEME_IsMainScreenLandscape())
    {
        copy_param.rect= MMITHEME_GetFullScreenRect();  
    }
    else
    {   
        DestroyLandscapeThumbnailLayer();
        GUIOWNDRAW_GetDisplayRect(VP_PLAY_PANEL_VIDEO_OWNDRAW_CTRL_ID,&(copy_param.rect));      
    }

        copy_buf.data_type=DATA_TYPE_RGB565;
        copy_buf.width=image_width;
        copy_buf.height=image_height;
        copy_buf.buf_ptr=image_data->buf.oneplane.p_data;
 
    if((copy_param.rect.right-copy_param.rect.left+1)>=image_width && (copy_param.rect.bottom-copy_param.rect.top+1)>=image_height)
    {
            copy_param.rect.left += (copy_param.rect.right -copy_param.rect.left+1-image_width)/2;
            copy_param.rect.top += (copy_param.rect.bottom-copy_param.rect.top+1-image_height)/2;                    
            copy_param.rect.right = (copy_param.rect.left + image_width-1);
            copy_param.rect.bottom = (copy_param.rect.top + image_height-1);

             if (MMITHEME_IsMainScreenLandscape())
            {//横屏时需要创建一个缩略图层，跟主层实现叠加效果
                if(CreateLandscapeThumbnailLayer())
                {
                    GUI_RECT_T display_rect={0};
                    UILAYER_SetLayerColorKey((GUI_LCD_DEV_INFO *)MMITHEME_GetDefaultLcdDev(), TRUE, MMI_BLACK_COLOR);//设置主层的透明色 和alpha值
                    UILAYER_SetLayerAlpha((GUI_LCD_DEV_INFO *)MMITHEME_GetDefaultLcdDev(), VP_HALF_TRANSPARENCE);

                    UILAYER_CopyBufferToLayer(&s_thumbnail_layer,&copy_param,&copy_buf);  //图像buffer拷贝至 层 
                    MMITHEME_StoreUpdateRect(&s_thumbnail_layer,MMITHEME_GetFullScreenRect());

                    GUIFORM_SetVisible(VP_PLAY_PANEL_FORM_CTRL_ID,TRUE,TRUE); //MSG_FULL_PAINT为了使alpha值立即生效 
                    GUIOWNDRAW_GetDisplayRect(VP_PLAY_PANEL_VIDEO_OWNDRAW_CTRL_ID,&display_rect);
                    GUI_FillRect(MMITHEME_GetDefaultLcdDev(),display_rect,MMIAPIVP_GetTransparenceColor());  //清除 遗留的字符串 和默认图片
                    MMITHEME_UpdateRect();
                    result=TRUE;
                }
            }
             else
            {//竖屏时，图像区域没有遮挡 ，可以直接拷贝至主层显示  
                GUI_RECT_T display_rect={0};
                GUIOWNDRAW_GetDisplayRect(VP_PLAY_PANEL_VIDEO_OWNDRAW_CTRL_ID,&display_rect);
                GUI_FillRect(MMITHEME_GetDefaultLcdDev(),display_rect,MMIAPIVP_GetTransparenceColor()); //清除 遗留的字符串 

                UILAYER_CopyBufferToLayer((GUI_LCD_DEV_INFO *)MMITHEME_GetDefaultLcdDev(),&copy_param,&copy_buf);
                MMITHEME_StoreUpdateRect(MMITHEME_GetDefaultLcdDev(),MMITHEME_GetFullScreenRect());
                MMITHEME_UpdateRect();
                result=TRUE;
            } 
        }
        MMIAPIVP_ThumbnailClose();
         return result;
}

 
/*****************************************************************************/
//  Description : 视频播放器载入显示祯功能，准备参数并开始缩略图获取
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN NotifyDPLAYERGetTumbnail(BOOLEAN is_dphandle) 
{
    VP_DISPLAY_PARAM_T  display_param={0};
    GUI_RECT_T  display_rect={0};
    display_param.audioSupport =0;
    display_param.videoSupport = 1;
    display_param.b_support_seek = 1;
    display_param.disp_mode = DPLAYER_DISP_FULLVIEW;    
    display_param.RotateAngle = 0;//GUILCD_GetLogicAngle(GUI_MAIN_LCD_ID);//s_video_player_control_info.cur_angle; 
    display_param.disp_data_op = DPLAYER_DISP_DATA_GET;
    display_param.b_no_osd = 1;

    //SCI_TRACE_LOW:"[VP]:NotifyDPLAYERGetTumbnail   RotateAngle:%d,s_video_player_control_info.cur_angle:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_13942_112_2_18_3_5_57_554,(uint8*)"dd",display_param.RotateAngle,s_video_player_control_info.cur_angle);

	SCI_TRACE_LOW("[VP]:NotifyDPLAYERGetTumbnail is_dphandle=%d", is_dphandle);
    if (MMITHEME_IsMainScreenLandscape())
    {
        display_rect = MMITHEME_GetFullScreenRect();    
    }
    else
    {   
        GUIOWNDRAW_GetDisplayRect(VP_PLAY_PANEL_VIDEO_OWNDRAW_CTRL_ID,&display_rect);      
    }
    
    display_param.disp_rect.dy = display_rect.bottom - display_rect.top + 1;
    display_param.disp_rect.dx = display_rect.right - display_rect.left + 1;       
    display_param.disp_rect.x = display_rect.left;
    display_param.disp_rect.y = display_rect.top;   

    display_param.target_rect.x = display_param.disp_rect.x;
    display_param.target_rect.y = display_param.disp_rect.y;
    display_param.target_rect.dx = display_param.disp_rect.dx;
    display_param.target_rect.dy = display_param.disp_rect.dy;

    MMIAPIVP_TranslateDisplayParamLogicToPhysical(&display_param, FALSE);
    //SCI_TRACE_LOW("[VP]:NotifyDPLAYERGetTumbnail   RotateAngle:%d,s_video_player_control_info.cur_angle:%d",display_param.RotateAngle,s_video_player_control_info.cur_angle);

    if(!is_dphandle)
    {
        if(MMIAPIVP_ThumbnailOpen(s_video_player_control_info.full_path_name, s_video_player_control_info.full_path_len, s_video_player_control_info.seek_time,DisplayThumbnail, &display_param,TRUE))
        {
            return TRUE;
        }
        else if(MMIAPIVP_IsThumbnailOpen() && MMIAPIVP_ThumbnailGetVideoQueueNum() == 0)
        {
            if(MMIAPIVP_ThumbnailVideoAdd(s_video_player_control_info.full_path_name))
            {
                MMIAPIVP_ThumbnailSetAngle(GUILCD_GetLogicAngle(GUI_MAIN_LCD_ID));
                VPThumbnailSetDPLAYERParam();
                MMIAPIVP_ThumbnailResume();
                return TRUE;
            }
            else
            {
                return FALSE;
            }
        }
        else
        {
            //SCI_TRACE_LOW:"[VP]:NotifyDPLAYERGetTumbnail   fail !"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_13973_112_2_18_3_5_57_555,(uint8*)"");
           // MMIAPIVP_ThumbnailClose();
            return FALSE;
        }
    }
    else
        {   
                DPLAYER_HANDLE dphandle = VP_GetDPLAYERhandlebyVPhandle(s_video_player_control_info.vp_handle);        
                DPLAYER_GetFrameEndNotify(dphandle, &(s_vp_dphandle_getthumbnail.pf_frame_end_notify));
                s_vp_dphandle_getthumbnail.is_getthumbnail_by_dpHandle = TRUE;

                if(s_video_player_control_info.support_type == VP_SUPPORTED_BOTH ||s_video_player_control_info.support_type == VP_SUPPORTED_VIDEO_ONLY)        
                {
                    if(MMIAPIVP_ThumbnailOpenByHandle(s_video_player_control_info.vp_handle, s_video_player_control_info.process_time, DisplayThumbnail, &display_param))
                    {
                        return TRUE;
                    }
                    else
                    {
                        SCI_TRACE_LOW("[VP]:NotifyDPLAYERGetTumbnail   fail2 !");
                        VPThumbnailResetDPLAYERParam();
                        return FALSE;
                    }
                }

        }		
        return FALSE;
}
/*****************************************************************************/
//  Description :  停止 DPHANDLE 获取缩略图，并还原DPHANDLE的播放参数
//  Global resource dependence : 
//  Author:  
//  Note: 
/*****************************************************************************/
LOCAL void VPThumbnailResetDPLAYERParam(void)
{
    DPLAYER_HANDLE dphandle = VP_GetDPLAYERhandlebyVPhandle(s_video_player_control_info.vp_handle);  
    VP_DISPLAY_PARAM_T  display_param={0};
    GUI_RECT_T  display_rect= {0};
    BOOLEAN is_full_screen = TRUE;


    VP_STRACE_LOW("[VP]:VPThumbnailResetDPLAYERParam");
        
    if(dphandle == NULL)
    {
        VP_STRACE_LOW("[VP]:VPThumbnailResetDPLAYERParam dphandle is null");
        return ;
    }
    DPLAYER_Pause(dphandle);
    DPLAYER_SetFrameEndNotify(dphandle, s_vp_dphandle_getthumbnail.pf_frame_end_notify);
    SCI_MEMSET((void *)&s_vp_dphandle_getthumbnail,0x00,sizeof(DPHANDLE_GETTHUMBNAIL_T));

    display_param.RotateAngle = GUILCD_GetLogicAngle(GUI_MAIN_LCD_ID);      

    if (!MMITHEME_IsMainScreenLandscape())
    {   
        is_full_screen = FALSE;
    }            
    else if (LCD_ANGLE_90 == GUILCD_GetLogicAngle(GUI_MAIN_LCD_ID)
    || LCD_ANGLE_270 == GUILCD_GetLogicAngle(GUI_MAIN_LCD_ID)
    )
    {
        is_full_screen = TRUE;
    }
    else
    {
        is_full_screen = FALSE;
    }


    if (is_full_screen)
    {             
        display_rect = MMITHEME_GetFullScreenRect();  
    }
    else
    {   
        GUIOWNDRAW_GetDisplayRect(VP_PLAY_PANEL_VIDEO_OWNDRAW_CTRL_ID,&display_rect);        
        StartVPWinDisplayerTimer();      
    }       

    display_param.disp_rect.dy = display_rect.bottom - display_rect.top + 1;
    display_param.disp_rect.dx = display_rect.right - display_rect.left + 1;            
    display_param.disp_rect.x = display_rect.left;
    display_param.disp_rect.y = display_rect.top;  

    display_param.target_rect.x = display_param.disp_rect.x;
    display_param.target_rect.y = display_param.disp_rect.y;
    display_param.target_rect.dx = display_param.disp_rect.dx;
    display_param.target_rect.dy = display_param.disp_rect.dy;

    MMIAPIVP_FSMSwitchAudioSupport(s_video_player_control_info.vp_handle,TRUE);    
    MMIAPIVP_FSMSwitchDisplayMode(s_video_player_control_info.vp_handle,&display_param,TRUE);           
}

#endif
#ifndef MMI_GUI_STYLE_TYPICAL
#ifdef MMI_PDA_SUPPORT
#ifdef MMI_STREAMING_PLAYER_SUPPORT
/*****************************************************************************/
//  Description : 书签列表 长按弹出选项消息处理.
//  Global resource dependence : 
//  Author: xin.li
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleBookmarkLongOkOptMenuWinMsg(
                                     MMI_WIN_ID_T       win_id,     
                                     MMI_MESSAGE_ID_E    msg_id, 
                                     DPARAM              param
                                     )
{
    MMI_RESULT_E                result              = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T               ctrl_id             = VP_BOOKMARK_LONGOK_OPT_MENU_CTRL_ID;
    MMI_MENU_ID_T               menu_id             = 0;
    MMI_MENU_GROUP_ID_T         group_id            = 0;  
    BOOLEAN                     delete_result       = TRUE;
    uint16                      cur_bookmark_list_index      = 0;
    const VP_NET_MOVIE_INFO_T   *bookmark_data_ptr  = PNULL;
    VP_NET_MOVIE_INFO_T         *net_info_ptr = PNULL;
    GUIMENU_TITLE_INFO_T        title_info = {0};
    MMI_STRING_T                main_title = {0};
    MMI_STRING_T                sub_title = {0};
    MMI_CTRL_ID_T               list_ctrl_id = VP_BOOKMARK_LIST_CTRL_ID;
    
    switch (msg_id)
    {
    case MSG_FULL_PAINT: 
        if (0 == VP_BookmarkList_GetNum())
        {
            MMIAPICOM_EnableGrayed(win_id,MENU_VP_BOOKMARK_LONGOK_OPT,ID_VP_DELETE_CUR,TRUE);
            MMIAPICOM_EnableGrayed(win_id,MENU_VP_BOOKMARK_LONGOK_OPT,ID_VP_MODIFY,TRUE);
            MMIAPICOM_EnableGrayed(win_id,MENU_VP_BOOKMARK_LONGOK_OPT,ID_VP_FILEATTR,TRUE);
        }

        //set title
        GetListItemTextInfo(list_ctrl_id, GUILIST_GetCurItemIndex(list_ctrl_id), &main_title, &sub_title);
        title_info.is_static    = TRUE;
        title_info.group_id     = MENU_VP_BOOKMARK_LONGOK_OPT;
        title_info.title_ptr    = &main_title;
        title_info.sub_title_ptr = &sub_title; 
        GUIMENU_SetMenuTitleEx(ctrl_id, &title_info);
        


        MMK_SetAtvCtrl(win_id,ctrl_id);
        break;
        
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        GUIMENU_GetId(ctrl_id,&group_id,&menu_id);
        //VP_STRACE_LOW:"[VP] HandleBookmarkLongOkOptMenuWinMsg menu_id = %x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_14034_112_2_18_3_5_58_556,(uint8*)"d",menu_id);
        switch (menu_id)
        {
        case ID_VP_FILEATTR:
            cur_bookmark_list_index = GUILIST_GetCurItemIndex(VP_BOOKMARK_LIST_CTRL_ID);
            //VP_STRACE_LOW:"[VP] HandleBookmarkLongOkOptMenuWinMsg cur_list_index = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_14039_112_2_18_3_5_58_557,(uint8*)"d",cur_bookmark_list_index);
            bookmark_data_ptr = VP_BookmarkList_GetInfo(VP_BookmarkList_GetNum() - 1 - cur_bookmark_list_index);          
            if (PNULL != bookmark_data_ptr)
            {
                net_info_ptr = (VP_NET_MOVIE_INFO_T *)SCI_ALLOCA(sizeof(VP_NET_MOVIE_INFO_T));
                if (PNULL != net_info_ptr)
                {
                    SCI_MEMSET(net_info_ptr,0,sizeof(VP_NET_MOVIE_INFO_T));
                    SCI_MEMCPY(net_info_ptr,bookmark_data_ptr,sizeof(VP_NET_MOVIE_INFO_T));                
                    MMK_CreateWin((uint32 *)MMIVP_VIEW_NET_DETAIL_WIN_TAB,(ADD_DATA)net_info_ptr);
                }
                else
                {
                    //VP_STRACE_LOW:"[VP] HandleBookmarkLongOkOptMenuWinMsg no memory"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_14052_112_2_18_3_5_58_558,(uint8*)"");
                    MMIPUB_OpenAlertFailWin(TXT_ERROR);
                }
            }
            else
            {
                MMIPUB_OpenAlertFailWin(TXT_ERROR);
            }
            MMK_CloseWin(win_id);
           
            break;
            
        case ID_VP_DELETE_CUR:
            MMIPUB_OpenQueryWinByTextId(TXT_DELETEQUERY,IMAGE_PUBWIN_QUERY,PNULL,PNULL);
            break;
            
        case ID_VP_DELETE_ALL:
            MMIPUB_OpenQueryWinByTextId(TXT_DELALL,IMAGE_PUBWIN_QUERY,PNULL,PNULL);
            break;
            
        case ID_VP_MODIFY:
            cur_bookmark_list_index = GUILIST_GetCurItemIndex(VP_BOOKMARK_LIST_CTRL_ID);
            MMK_CreateWin((uint32 *) MMIVP_ADD_BOOKMARK_WIN_TAB,(ADD_DATA)cur_bookmark_list_index);
            MMK_CloseWin(win_id);
            break;

        case ID_VP_ADD:
            cur_bookmark_list_index = VP_BookmarkList_GetNum();
            MMK_CreateWin((uint32 *) MMIVP_ADD_BOOKMARK_WIN_TAB,(ADD_DATA)cur_bookmark_list_index);
            MMK_CloseWin(win_id);
            break;
        default:
            break;
        } 
        
        break;

    case MSG_PROMPTWIN_OK:
        MMIPUB_CloseQuerytWin(PNULL);
        GUIMENU_GetId(ctrl_id,&group_id,&menu_id);
        //VP_STRACE_LOW:"[VP] HandleBookmarkLongOkOptMenuWinMsg menu_id = %x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_14092_112_2_18_3_5_58_559,(uint8*)"d",menu_id);
        switch(menu_id) 
        {
        case ID_VP_DELETE_CUR:  
            cur_bookmark_list_index = GUILIST_GetCurItemIndex(VP_BOOKMARK_LIST_CTRL_ID);
            delete_result = VP_BookmarkList_DelItem(VP_BookmarkList_GetNum() - 1 - cur_bookmark_list_index);
            break;
        case ID_VP_DELETE_ALL:
            delete_result = VP_BookmarkList_DelAllItem();
            break;
        default:
            break;
        }  
        UpdateListData(MMIVP_BOOKMARKLIST_WIN_ID,VP_BOOKMARK_LIST_CTRL_ID,VP_BookmarkList_GetNum(),GetBookmarkListItemContent);
        GUILIST_SetCurItemIndex(VP_BOOKMARK_LIST_CTRL_ID,cur_bookmark_list_index);
        if (delete_result)
        {
		    //xin.li_clean_popwin
            //MMIPUB_OpenAlertSuccessWin(TXT_DELETED);
        }
        else
        {
            MMIPUB_OpenAlertFailWin(TXT_COMMON_DEL_FAIL);
        }
        MMK_CloseWin(win_id);
        break;

    case MSG_PROMPTWIN_CANCEL:
        MMIPUB_CloseQuerytWin(PNULL);
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}
#endif
#ifdef MMI_VIDEOPLAYER_HISTORYLIST_SUPPORT
/*****************************************************************************/
//  Description : 历史记录列表 长按选项消息处理.
//  Global resource dependence : 
//  Author: xin.li
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleHistoryLongOkOptMenuWinMsg(
                                     MMI_WIN_ID_T       win_id,     
                                     MMI_MESSAGE_ID_E    msg_id, 
                                     DPARAM              param
                                     )
{
    MMI_RESULT_E                result              = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T               ctrl_id             = VP_HISTORY_LONGOK_OPT_MENU_CTRL_ID;
    MMI_MENU_ID_T               menu_id             = 0;
    MMI_MENU_GROUP_ID_T         group_id            = 0;
    BOOLEAN                     delete_result       = TRUE;
    uint16                      cur_historylist_index      = 0;
    const VP_HISTORY_ITEM_T     *history_info_ptr   = PNULL;
    VP_LOCAL_MOVIE_FILE_INFO_T  *local_movie_file_info_ptr = PNULL;
    GUIMENU_TITLE_INFO_T        title_info = {0};
    MMI_STRING_T                main_title = {0};
    MMI_STRING_T                sub_title = {0};
    MMI_CTRL_ID_T               list_ctrl_id = VP_HISTORY_LIST_CTRL_ID;
    MMIFMM_DETAIL_DATA_T        detail_data ={0};
    MMI_TM_T                    tm = {0};
#ifdef MMI_STREAMING_PLAYER_SUPPORT
   VP_NET_MOVIE_INFO_T         *net_file_info_ptr = PNULL;
#endif

    //VP_STRACE_LOW:"[VP] HandleHistoryLongOkOptMenuWinMsg msg_id = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_14162_112_2_18_3_5_58_560,(uint8*)"d",msg_id);
    switch (msg_id)
    {
    case MSG_FULL_PAINT:
        if (0 == VP_HistoryList_GetNum())
        {
            MMIAPICOM_EnableGrayed(win_id,MENU_VP_HISTORY_LONGOK_OPT,ID_VP_DELETE_CUR,TRUE);    
            MMIAPICOM_EnableGrayed(win_id,MENU_VP_HISTORY_LONGOK_OPT,ID_VP_FILEATTR,TRUE);
        }
        
        //set title
        GetListItemTextInfo(list_ctrl_id, GUILIST_GetCurItemIndex(list_ctrl_id), &main_title, &sub_title);
        title_info.is_static    = TRUE;
        title_info.group_id     = MENU_VP_HISTORY_LONGOK_OPT;
        title_info.title_ptr    = &main_title;
        title_info.sub_title_ptr = &sub_title; 
        GUIMENU_SetMenuTitleEx(ctrl_id, &title_info);



        MMK_SetAtvCtrl(win_id,ctrl_id);
        break;
        
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        GUIMENU_GetId(ctrl_id,&group_id,&menu_id);
        //VP_STRACE_LOW:"[VP] HandleHistoryLongOkOptMenuWinMsg menu_id = %x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_14191_112_2_18_3_5_58_561,(uint8*)"d",menu_id);
        switch (menu_id)
        {
        case ID_VP_FILEATTR:
            cur_historylist_index = GUILIST_GetCurItemIndex(VP_HISTORY_LIST_CTRL_ID);
            history_info_ptr = VP_HistoryList_GetInfo(cur_historylist_index);
            if (PNULL != history_info_ptr)
            {
#ifdef MMI_STREAMING_PLAYER_SUPPORT
                if (VP_PLAY_FROM_NET == history_info_ptr->type)
                {
                    net_file_info_ptr = (VP_NET_MOVIE_INFO_T *)SCI_ALLOCA(sizeof(VP_NET_MOVIE_INFO_T));
                    if (PNULL != net_file_info_ptr && PNULL != history_info_ptr->net_movie_info_ptr)
                    {
                        SCI_MEMSET(net_file_info_ptr,0,sizeof(VP_NET_MOVIE_INFO_T));
                        SCI_MEMCPY(net_file_info_ptr,history_info_ptr->net_movie_info_ptr,sizeof(VP_NET_MOVIE_INFO_T));
                        MMK_CreateWin((uint32 *)MMIVP_VIEW_NET_DETAIL_WIN_TAB,(ADD_DATA)net_file_info_ptr);
                    }
                    else
                    {
                        //VP_STRACE_LOW:"[VP] HandleHistoryLongOkOptMenuWinMsg no memory"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_14211_112_2_18_3_5_58_562,(uint8*)"");
                        MMIPUB_OpenAlertFailWin(TXT_ERROR);

                    }
                }
                else
#endif
					if (PNULL != history_info_ptr->local_movie_file_info_ptr &&
                    !MMIAPIFMM_IsFileExist(history_info_ptr->local_movie_file_info_ptr->full_path_ptr,history_info_ptr->local_movie_file_info_ptr->full_path_len))
                {
                    MMIPUB_OpenAlertWarningWin(TXT_COM_FILE_NO_EXIST);
                }
                else
                {
                    local_movie_file_info_ptr = history_info_ptr->local_movie_file_info_ptr;
                    if (PNULL != local_movie_file_info_ptr)
                    {
                        tm = MMIAPICOM_FatData2Tm(local_movie_file_info_ptr->create_time); 
                        detail_data.time = MMIAPICOM_Tm2Second(tm.tm_sec,tm.tm_min,tm.tm_hour,tm.tm_mday,tm.tm_mon,tm.tm_year);
                        detail_data.file_size = local_movie_file_info_ptr->file_size;
                        detail_data.filename_len = MIN(MMIFMM_FILENAME_LEN,local_movie_file_info_ptr->full_path_len);
                        MMIAPICOM_Wstrncpy(detail_data.filename,local_movie_file_info_ptr->full_path_ptr,detail_data.filename_len);
#ifdef DRM_SUPPORT
                        if(MMIAPIDRM_IsDRMFile(PNULL,detail_data.filename))
                        {
                            detail_data.type = MMIFMM_FILE_TYPE_DRM;
                        }
                        else
#endif
                        {
                            detail_data.type = MMIFMM_FILE_TYPE_MOVIE;
                        }
                        MMIAPIFMM_DetailFileData(&detail_data);
                    }
                    else
                    {
                        MMIPUB_OpenAlertFailWin(TXT_ERROR);
                    }
                }
            }
            else
            {
                MMIPUB_OpenAlertFailWin(TXT_ERROR);
            }
            MMK_CloseWin(win_id);
            break;
            
        case ID_VP_DELETE_CUR:
            MMIPUB_OpenQueryWinByTextId(TXT_DELETEQUERY,IMAGE_PUBWIN_QUERY,PNULL,PNULL);
            break;
            
        case ID_VP_DELETE_ALL:
            MMIPUB_OpenQueryWinByTextId(TXT_DELALL,IMAGE_PUBWIN_QUERY,PNULL,PNULL);
            break;            
       
        default:
            break;
        } 
        
        break;

    case MSG_PROMPTWIN_OK:
        MMIPUB_CloseQuerytWin(PNULL);
        GUIMENU_GetId(ctrl_id,&group_id,&menu_id);
        //VP_STRACE_LOW:"[VP] HandleHistoryLongOkOptMenuWinMsg menu_id = %x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_14275_112_2_18_3_5_58_563,(uint8*)"d",menu_id);
        switch(menu_id) 
        {
        case ID_VP_DELETE_CUR:
            cur_historylist_index = GUILIST_GetCurItemIndex(VP_HISTORY_LIST_CTRL_ID);
            delete_result = VP_HistoryList_DelItem(cur_historylist_index);
            break;
        case ID_VP_DELETE_ALL:
            delete_result = VP_HistoryList_DelAllItem();
            cur_historylist_index = 0;
            break;
        default:
            break;
        }  
        UpdateListData(MMIVP_HISTORYLIST_WIN_ID,VP_HISTORY_LIST_CTRL_ID,VP_HistoryList_GetNum(),GetHistoryListItemContent);
        GUILIST_SetCurItemIndex(VP_HISTORY_LIST_CTRL_ID,cur_historylist_index);
        if (delete_result)
        {
			//xin.li_clean_popwin
            //MMIPUB_OpenAlertSuccessWin(TXT_DELETED);
                        
        }
        else
        {
            MMIPUB_OpenAlertFailWin(TXT_COMMON_DEL_FAIL);
        }       
        MMK_CloseWin(win_id);
        
        break;

    case MSG_PROMPTWIN_CANCEL:
        MMIPUB_CloseQuerytWin(PNULL);
        MMK_CloseWin(win_id);   //NEWMS00164270
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}
#endif //MMI_VIDEOPLAYER_HISTORYLIST_SUPPORT
/*****************************************************************************/
//  Description : 本地列表 长按选项消息处理.
//  Global resource dependence : 
//  Author: xin.li
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleLocalListLongOkOptMenuWinMsg(
                                     MMI_WIN_ID_T       win_id,     
                                     MMI_MESSAGE_ID_E    msg_id, 
                                     DPARAM              param
                                     )
{
    MMI_RESULT_E                result                          = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T               ctrl_id                         = VP_DEFAULT_LOCALLIST_LONGOK_OPT_MENU_CTRL_ID;
    MMI_CTRL_ID_T               list_ctrl_id                    = VP_DEFAULT_LOCALLIST_CTRL_ID;
    MMI_MENU_ID_T               menu_id                         = 0;
    MMI_MENU_GROUP_ID_T         group_id                        = MENU_VP_DEFAULT_LOCALLIST_LONGOK_OPT;      
    uint16                      cur_locallist_index             = 0;  
    uint16                      marked_locallist_index             = 0;  
    FILEARRAY_DATA_T            array_data                      = {0};   
    uint16                      mark_num                        = 0;
    uint16                      * select_item_ptr               = PNULL;
    uint32                      locallist_num                   = 0;
    VP_LOCALLIST_TYPE_E         locallist_type                  = VP_LOCALLIST_DEFAULT;
    MMI_WIN_ID_T                list_win_id                     = MMIVP_DEFAULT_LOCALLIST_WIN_ID;
    MMIFILE_ERROR_E             file_error                      = SFS_ERROR_NONE;
    GUIMENU_TITLE_INFO_T        title_info                      = {0};
    MMI_STRING_T                main_title                      = {0};
    MMI_STRING_T                sub_title                       = {0};
    MMIFMM_DETAIL_DATA_T        detail_data                     = {0};
    
    if (MMIVP_USER_LOCALLIST_LONGOK_OPT_MENU_WIN_ID == win_id)
    {
        ctrl_id         = VP_USER_LOCALLIST_LONGOK_OPT_MENU_CTRL_ID;
        list_ctrl_id    = VP_USER_LOCALLIST_CTRL_ID;
        locallist_type  = VP_LOCALLIST_USER;
        list_win_id     = MMIVP_USER_LOCALLIST_WIN_ID;
        group_id        = MENU_VP_USER_LOCALLIST_LONGOK_OPT;
    }
    
    //VP_STRACE_LOW:"[VP] HandleLocalListLongOkOptMenuWinMsg msg_id = %x, locallist_type = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_14360_112_2_18_3_5_58_564,(uint8*)"dd",msg_id,locallist_type);
    switch (msg_id)
    {
    case MSG_FULL_PAINT:
        locallist_num = VP_LocalList_GetNum(locallist_type);        
        if (0 == locallist_num)
        {
            MMIAPICOM_EnableGrayed(win_id,group_id,ID_VP_DELETE,TRUE);
            MMIAPICOM_EnableGrayed(win_id,group_id,ID_VP_FILEATTR,TRUE);
            MMIAPICOM_EnableGrayed(win_id,group_id,ID_VP_SCEENSAVE,TRUE);
            MMIAPICOM_EnableGrayed(win_id,group_id,ID_VP_POWER_ON_SET,TRUE);
            MMIAPICOM_EnableGrayed(win_id,group_id,ID_VP_POWER_OFF_SET,TRUE);
            MMIAPICOM_EnableGrayed(win_id,group_id,ID_VP_VIDEO_WALLPAPER,TRUE);
        }
        else
        {
            mark_num = GUILIST_GetSelectedItemIndex(list_ctrl_id,PNULL,0);
            if (mark_num > 0)
            {
                MMIAPICOM_EnableGrayed(win_id,group_id,ID_VP_SCEENSAVE,TRUE);               
                MMIAPICOM_EnableGrayed(win_id,group_id,ID_VP_POWER_ON_SET,TRUE);
                MMIAPICOM_EnableGrayed(win_id,group_id,ID_VP_POWER_OFF_SET,TRUE);
                MMIAPICOM_EnableGrayed(win_id,group_id,ID_VP_VIDEO_WALLPAPER,TRUE);
                MMIAPICOM_EnableGrayed(win_id,group_id,ID_VP_FILEATTR,TRUE);
            }
        }
        
        //set title
        GetListItemTextInfo(list_ctrl_id, GUILIST_GetCurItemIndex(list_ctrl_id), &main_title, &sub_title);
        title_info.is_static    = TRUE;
        title_info.group_id     = group_id;
        title_info.title_ptr    = &main_title;
        title_info.sub_title_ptr = &sub_title; 
        GUIMENU_SetMenuTitleEx(ctrl_id, &title_info);
        


        MMK_SetAtvCtrl(win_id,ctrl_id);
        break;

    case MSG_CTL_MIDSK:  
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_WEB:
        GUIMENU_GetId(ctrl_id,&group_id,&menu_id);
        
        //VP_STRACE_LOW:"[VP] HandleLocalListLongOkOptMenuWinMsg menu_id = %x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_14406_112_2_18_3_5_58_565,(uint8*)"d",menu_id);
        switch (menu_id)
        {
        case ID_VP_FILEATTR:
            cur_locallist_index = GUILIST_GetCurItemIndex(list_ctrl_id);
            //VP_STRACE_LOW:"[VP] HandleLocalListLongOkOptMenuWinMsg cur_list_index = %d, locallist_type = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_14411_112_2_18_3_5_58_566,(uint8*)"dd",cur_locallist_index,locallist_type);
            if(VP_LocalList_GetInfo(cur_locallist_index,&array_data,locallist_type))
            {
                if (!MMIAPIFMM_IsFileExist(array_data.filename,array_data.name_len))
                {
                    MMIPUB_OpenAlertWarningWin(TXT_COM_FILE_NO_EXIST);
                }
                else
                {
                    detail_data.time = MMIAPICOM_Tm2Second(array_data.modify_time.sec,
                                                            array_data.modify_time.min,
                                                            array_data.modify_time.hour,
                                                            array_data.modify_date.mday,
                                                            array_data.modify_date.mon,
                                                            array_data.modify_date.year);
                    detail_data.file_size = array_data.size;
                    detail_data.filename_len = MIN(MMIFMM_FILENAME_LEN,array_data.name_len);
                    MMIAPICOM_Wstrncpy(detail_data.filename,array_data.filename,detail_data.filename_len);
#ifdef DRM_SUPPORT
                    if(MMIAPIDRM_IsDRMFile(PNULL,detail_data.filename))
                    {
                        detail_data.type = MMIFMM_FILE_TYPE_DRM;
                    }
                    else
#endif
                    {
                        detail_data.type = MMIFMM_FILE_TYPE_MOVIE;
                    }
                    MMIAPIFMM_DetailFileData(&detail_data);
                }
            }
            else
            {
                MMIPUB_OpenAlertFailWin(TXT_ERROR);
                //VP_STRACE_LOW:"[VP] HandleLocalListLongOkOptMenuWinMsg get file info error"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_14445_112_2_18_3_5_58_567,(uint8*)"");
            }
            MMK_CloseWin(win_id);
            break;
            
        case ID_VP_DELETE:
            mark_num = GUILIST_GetSelectedItemIndex(list_ctrl_id,PNULL,0);
            cur_locallist_index = GUILIST_GetCurItemIndex(list_ctrl_id);
            if (0 == mark_num)  //删除当前
            {
                //ger current index
                //VP_STRACE_LOW:"[VP] HandleLocalListLongOkOptMenuWinMsg cur_list_index = %d, locallist_type = %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_14456_112_2_18_3_5_58_568,(uint8*)"dd",cur_locallist_index,locallist_type);
                //如果删除的是当前播放文件，则停止播放
                if (locallist_type == VP_LocalList_GetCurListType() && cur_locallist_index == VP_LocalList_GetCurrentIndex(locallist_type))
                { 
#ifdef MMI_VIDEOPLAYER_HISTORYLIST_SUPPORT
                    if (IsSaveLastPlayPoint())
                    {
                        VP_HistoryList_SetLastPlayPoint(s_video_player_control_info.process_time);
                    }  
#endif //MMI_VIDEOPLAYER_HISTORYLIST_SUPPORT
                    SCI_MEMSET(s_video_player_control_info.full_path_name,0,((VP_MAX_NAME_LEN +1)*sizeof(wchar)));
                    s_video_player_control_info.full_path_len = 0;
                    SCI_MEMSET(s_video_player_control_info.url,0,(VP_MAX_URL_LEN +1));    
                    VideoPlayerStop(MMIVP_PLAYER_WIN_ID);
                }
                //删除本地播放列表
                if (VP_LocalList_DelItemWithouSaveFile(cur_locallist_index,locallist_type))
                {
                    GUILIST_RemoveItem(list_ctrl_id,cur_locallist_index);
                    file_error = VP_Locallist_SaveToFile(locallist_type);
                    if (SFS_ERROR_NONE != file_error)
                    {
                        if (locallist_type == VP_LocalList_GetCurListType())
                        { 
#ifdef MMI_VIDEOPLAYER_HISTORYLIST_SUPPORT
                            VP_HistoryList_SetLastPlayPoint(s_video_player_control_info.process_time);
#endif //MMI_VIDEOPLAYER_HISTORYLIST_SUPPORT
							SCI_MEMSET(s_video_player_control_info.full_path_name,0,((VP_MAX_NAME_LEN +1)*sizeof(wchar)));
                            s_video_player_control_info.full_path_len = 0;
                            SCI_MEMSET(s_video_player_control_info.url,0,(VP_MAX_URL_LEN +1));    
                            VideoPlayerStop(MMIVP_PLAYER_WIN_ID);
                        }
                    }
                }
            }
            else //删除标记
            {
                select_item_ptr = (uint16 *)SCI_ALLOCA((sizeof(uint16)*GUILIST_GetTotalItemNum(list_ctrl_id)));
                if (PNULL != select_item_ptr)
                {
                    MMIPUB_OpenAlertWinByTextId(PNULL,TXT_COMMON_WAITING,TXT_NULL,IMAGE_PUBWIN_WAIT,PNULL,PNULL,MMIPUB_SOFTKEY_NONE,PNULL);

                    SCI_MEMSET(select_item_ptr,0,(sizeof(uint16)*GUILIST_GetTotalItemNum(list_ctrl_id)));/*lint !e666*/
                    GUILIST_GetSelectedItemIndex(list_ctrl_id,select_item_ptr,mark_num);
                   
                    for (marked_locallist_index = 0; marked_locallist_index < mark_num; marked_locallist_index++)
                    {                    
                        //如果删除的是当前播放文件，则停止播放
                        if (locallist_type == VP_LocalList_GetCurListType() && select_item_ptr[marked_locallist_index] == VP_LocalList_GetCurrentIndex(locallist_type))

                        {
#ifdef MMI_VIDEOPLAYER_HISTORYLIST_SUPPORT
                            if(IsSaveLastPlayPoint())
                            {
                                VP_HistoryList_SetLastPlayPoint(s_video_player_control_info.process_time);
                            }
#endif //MMI_VIDEOPLAYER_HISTORYLIST_SUPPORT
                            SCI_MEMSET(s_video_player_control_info.full_path_name,0,((VP_MAX_NAME_LEN +1)*sizeof(wchar)));
                            s_video_player_control_info.full_path_len = 0;
                            SCI_MEMSET(s_video_player_control_info.url,0,(VP_MAX_URL_LEN +1));
                            VideoPlayerStop(MMIVP_PLAYER_WIN_ID);
                        }

                        if(0 != cur_locallist_index && select_item_ptr[marked_locallist_index] <= cur_locallist_index)
                        {
                            cur_locallist_index--;
                        }
                        
                    }

                    //开始删除
                    while(mark_num>0)
                    {
                        GUILIST_GetSelectedItemIndex(list_ctrl_id,select_item_ptr,mark_num);
                        
                        if (VP_LocalList_DelItemWithouSaveFile(select_item_ptr[0],locallist_type))
                        {                           
                            GUILIST_RemoveItem(list_ctrl_id,select_item_ptr[0]);
                        }
                        else
                        {

                            SCI_FREE(select_item_ptr);
                            //VP_STRACE_LOW:"[VP] HandleLocalListLongOkOptMenuWinMsg delete FAIL"
                            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_14539_112_2_18_3_5_59_569,(uint8*)"");
                            break;
                        }
                        mark_num = GUILIST_GetSelectedItemIndex(list_ctrl_id,PNULL,0);
                    }
                    MMIPUB_CloseAlertWin();               
                    file_error = VP_Locallist_SaveToFile(locallist_type);
                    if (SFS_ERROR_NONE != file_error)
                    {
                        if (locallist_type == VP_LocalList_GetCurListType())
                        {  
#ifdef MMI_VIDEOPLAYER_HISTORYLIST_SUPPORT
                            VP_HistoryList_SetLastPlayPoint(s_video_player_control_info.process_time);
#endif //MMI_VIDEOPLAYER_HISTORYLIST_SUPPORT
							SCI_MEMSET(s_video_player_control_info.full_path_name,0,((VP_MAX_NAME_LEN +1)*sizeof(wchar)));
                            s_video_player_control_info.full_path_len = 0;
                            SCI_MEMSET(s_video_player_control_info.url,0,(VP_MAX_URL_LEN +1));    
                            VideoPlayerStop(MMIVP_PLAYER_WIN_ID);
                        }
                    }

                    SCI_FREE(select_item_ptr);
                    //marked_locallist_index = 0;
                    
                }
            }
            GUILIST_SetTextListMarkable(list_ctrl_id,FALSE);
            UpdateListData(list_win_id,list_ctrl_id,VP_LocalList_GetNum(locallist_type),GetLocalListItemContent);
            GUILIST_SetCurItemIndex(list_ctrl_id,cur_locallist_index);
            MMK_CloseWin(win_id);
            break;
            
        case ID_VP_SCEENSAVE:
#ifdef SCREENSAVER_SUPPORT
            cur_locallist_index = GUILIST_GetCurItemIndex(list_ctrl_id);
            VP_LocalList_GetInfo(cur_locallist_index,&array_data,locallist_type);
            
            if(!MMIAPIFMM_IsFileExist(array_data.filename,array_data.name_len))
            {
                MMIPUB_OpenAlertWarningWin(TXT_COM_FILE_NO_EXIST);
            }    
            else if (0 == array_data.size)
            {
                MMIPUB_OpenAlertWarningWin(TXT_EMPTY_FILE);
            }
            else
            {
                SettingAsScreensaver(array_data.filename,array_data.name_len);
            }
#endif
            MMK_CloseWin(win_id);
            break;

#ifdef MMI_VIDEOWALLPAPER_SUPPORT
        case ID_VP_VIDEO_WALLPAPER:
            cur_locallist_index = GUILIST_GetCurItemIndex(list_ctrl_id);
            VP_LocalList_GetInfo(cur_locallist_index,&array_data,locallist_type);
            SettingAsVideoWallpaper(array_data.filename,array_data.name_len,array_data.size);
            MMK_CloseWin(win_id);
            break;
#endif

        case ID_VP_POWER_ON_SET:
            cur_locallist_index = GUILIST_GetCurItemIndex(list_ctrl_id);
            VP_LocalList_GetInfo(cur_locallist_index,&array_data,locallist_type);
            if(!MMIAPIFMM_IsFileExist(array_data.filename,array_data.name_len))
            {
                MMIPUB_OpenAlertWarningWin(TXT_COM_FILE_NO_EXIST);
            }
            else if (0 == array_data.size)
            {
                MMIPUB_OpenAlertWarningWin(TXT_EMPTY_FILE);
            }
            else
            {
                SettingAsPowerOnOffAnimation(TRUE,array_data.filename,array_data.name_len);
            }
            MMK_CloseWin(win_id);
            break;

        case ID_VP_POWER_OFF_SET:
            cur_locallist_index = GUILIST_GetCurItemIndex(list_ctrl_id);
            VP_LocalList_GetInfo(cur_locallist_index,&array_data,locallist_type);
            if(!MMIAPIFMM_IsFileExist(array_data.filename,array_data.name_len))
            {
                MMIPUB_OpenAlertWarningWin(TXT_COM_FILE_NO_EXIST);
            }
            else if (0 == array_data.size)
            {
                MMIPUB_OpenAlertWarningWin(TXT_EMPTY_FILE);
            }
            else
            {
                SettingAsPowerOnOffAnimation(FALSE,array_data.filename,array_data.name_len);
            }
            MMK_CloseWin(win_id);
            break;
            
        default:
            break;
        } 
        
        break;
        
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}
#ifdef MMI_STREAMING_PLAYER_SUPPORT
/*****************************************************************************/
// 	Description : 网址浏览窗口 连接按钮callback
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E NetLinkButton1Callback(void)
{
        MMI_CTRL_ID_T           ctrl_id = VP_NET_URL_INPUT_CTRL_ID;    
        MMI_STRING_T            string  = {0}; 
        uint8                   url_ptr[VP_MAX_URL_LEN + 1] = {0}; 
        MMI_RESULT_E            return_code = MMI_RESULT_TRUE;
        
        //SCI_TRACE_LOW:"[VP]:NetLinkButtonCallback"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_14668_112_2_18_3_5_59_570,(uint8*)"");
        GUILABEL_GetText(ctrl_id,&string);

        if (0 != string.wstr_len && PNULL != string.wstr_ptr && 0 == MMIAPICOM_CompareTwoWstrExt(string.wstr_ptr,RTSP_URL_HEAD_LEN,s_rtsp_url_head,RTSP_URL_HEAD_LEN,FALSE))
        {
            MMIAPICOM_WstrToStr(string.wstr_ptr,(uint8 *)url_ptr);                
            NotifyPlayerForPlayNet(PNULL,0,url_ptr,0,0);
            //MMK_CloseWin(win_id);//NEWMS00164417
        }
        else if (0 != string.wstr_len && PNULL != string.wstr_ptr)
        {
            MMIAPICOM_WstrToStr(string.wstr_ptr,(uint8 *)url_ptr);
            OpenBrowerWin(url_ptr);       
            //MMK_CloseWin(win_id);
        }
        else
        {
            MMIPUB_OpenAlertWarningWin(TXT_COMM_URL_NULL);
            
        }   
    return return_code;
}

/*****************************************************************************/
// 	Description : CMCC 窗口 连接按钮callback
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E NetLinkButton2Callback(void)
{
    MMI_WIN_ID_T            win_id = MMIVP_NET_WIN_ID;
    VP_NET_MOVIE_INFO_T     net_movie_info = {0};
    MMI_RESULT_E            return_code = MMI_RESULT_TRUE;
    
    //SCI_TRACE_LOW:"[VP]:NetLinkButtonCallback"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_14701_112_2_18_3_5_59_571,(uint8*)"");
           
        net_movie_info = VP_GetCMCCAddressInfo();
        
        if (0 == strnicmp(net_movie_info.url_ptr,HTTP_URL_HEAD_ASIC,HTTP_URL_HEAD_LEN))
        {                
            OpenBrowerWin((uint8 *)(net_movie_info.url_ptr));       
            //MMK_CloseWin(win_id);//NEWMS00164417
        }
        else if (0 == strnicmp(net_movie_info.url_ptr,RTSP_URL_HEAD_ASIC,RTSP_URL_HEAD_LEN))
        {               
            
            NotifyPlayerForPlayNet((wchar *)(net_movie_info.title_arr),net_movie_info.title_len,(uint8 *)(net_movie_info.url_ptr),0,0);
            //MMK_CloseWin(win_id);//NEWMS00164417
        }
        else
        {
            MMIPUB_OpenAlertWarningWin(TXT_INVALID_URL);
            
        }            

    return return_code;
}
#endif
/*****************************************************************************/
// 	Description : 获取指定LIST项的文本信息
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN GetListItemTextInfo(
                                  MMI_CTRL_ID_T ctrl_id,    //[in]  控件ID
                                  uint16        index,      //[in]  索引
                                  MMI_STRING_T  *main_text,  //[out] 主标题
                                  MMI_STRING_T  *sub_text    //[out] 副标题
                                  )
{
    BOOLEAN             return_code = FALSE;
    const GUILIST_ITEM_T      *item_t_ptr = PNULL;
    const GUIITEM_STYLE_T     *style_t_ptr = PNULL;
    int16               main_index = 0;
    int16               sub_index = 0;
    
    item_t_ptr = GUILIST_GetItemPtrByIndex(ctrl_id, index);
    if(PNULL != item_t_ptr)
    {
        //通过style获取文本数据的索引位置
        style_t_ptr = MMITHEME_GetItemStyle(item_t_ptr->item_style);
        if(PNULL != style_t_ptr)
        {
            //主文本信息
            main_index = style_t_ptr->main_index;
            if(main_index >= 0)
            {
                main_text->wstr_ptr = item_t_ptr->item_data_ptr->item_content[main_index].item_data.text_buffer.wstr_ptr;
                main_text->wstr_len = item_t_ptr->item_data_ptr->item_content[main_index].item_data.text_buffer.wstr_len;
                
                return_code = TRUE;
            }
            
            //副文本信息
            sub_index = style_t_ptr->auxiliary_index;
            if(sub_index >= 0)
            {
                sub_text->wstr_ptr = item_t_ptr->item_data_ptr->item_content[sub_index].item_data.text_buffer.wstr_ptr;
                sub_text->wstr_len = item_t_ptr->item_data_ptr->item_content[sub_index].item_data.text_buffer.wstr_len;
            }
        }
    }
    
    return return_code;
}
#endif  //MMI_PDA_SUPPORT

/*****************************************************************************/
//  Description : handle PDA style settings window message
//  Global resource dependence : 
//  Author: xin.li
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePdaSettingsWinMsg(
                                        MMI_WIN_ID_T        win_id,     
                                        MMI_MESSAGE_ID_E    msg_id, 
                                        DPARAM              param
                                        )
{
    MMI_CTRL_ID_T       ctrl_id = MMIVP_PDA_SETTINGS_LIST_CTRL_ID;
    MMI_RESULT_E        result  = MMI_RESULT_TRUE;
    //MMI_WIN_ID_T alert_win_id = MMIVP_PDA_SETTINGS_WIN_ID;
    uint16                index = 0;
    uint32            user_data = 0;
    static int32  list_offset_y = 0;
    uint16           list_index = 0;
        
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
#ifdef PDA_UI_DROPDOWN_WIN
        GUIWIN_SetStbState(win_id, GUISTATUSBAR_STATE_INVALID, TRUE); 
#endif
        list_offset_y = 0;
        InitVpPdaSettingsListCtrl();
        MMK_SetAtvCtrl(win_id,ctrl_id);
        break;
        
    case MSG_LOSE_FOCUS:
        GUILIST_GetTopItemOffset(ctrl_id, &list_offset_y);
        break;
        
    case MSG_GET_FOCUS:
    case MSG_VP_UPDATE_BEGIN:
        index = GUILIST_GetCurItemIndex(ctrl_id);
        GUILIST_RemoveAllItems(ctrl_id);
        InitVpPdaSettingsListCtrl();
        //set list offset
        GUILIST_SetTopItemOffset(ctrl_id, list_offset_y);
        GUILIST_SetCurItemIndex(ctrl_id, index);
        break;
    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_WEB:
        index = GUILIST_GetCurItemIndex(ctrl_id);
        GUILIST_GetItemData(ctrl_id, index, &user_data);
        
        switch((MMIVP_PDA_SETTINGS_TYPE_E)user_data)
        {
#ifdef MMI_PDA_SUPPORT        
        case MMIVP_PDA_SETTINGS_REPEAT:
            MMK_CreatePubListWin((uint32 *)MMIVP_SETTINGS_REPEAT_WIN_TAB, PNULL);
            break;
            
        case MMIVP_PDA_SETTINGS_RANDOM:
            if(VP_Setting_GetIsRandomPlayMode())
            {
                VP_Setting_SetIsRandomPlayMode(FALSE);
                MMIAPIVP_FSMUpdatePlayMode(
                    s_video_player_control_info.vp_handle,
                    VP_Setting_GetRepeatPlayMode(),
                    FALSE);
            }
            else
            {
                VP_Setting_SetIsRandomPlayMode(TRUE);            
                MMIAPIVP_FSMUpdatePlayMode(
                    s_video_player_control_info.vp_handle,
                    VP_Setting_GetRepeatPlayMode(),
                    TRUE);
            }
            MMK_PostMsg(win_id, MSG_VP_UPDATE_BEGIN, PNULL, NULL);
            MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, NULL);
            break;
 #endif           
			
		case MMIVP_PDA_SETTINGS_PLAY_METHOD:
			MMK_CreatePubListWin((uint32 *)MMIVP_SETTINGS_PLAYBACK_MODE_WIN_TAB, PNULL);
			break;


 #ifdef MMI_STREAMING_PLAYER_SUPPORT//norflashcut           
        case MMIVP_PDA_SETTINGS_NET1:
        case MMIVP_PDA_SETTINGS_NET2:
        case MMIVP_PDA_SETTINGS_NET3:
        case MMIVP_PDA_SETTINGS_NET4:
            list_index = MMIAPICONNECTION_GetListIndexByAPNIndex((MN_DUAL_SYS_1+(user_data - MMIVP_PDA_SETTINGS_NET1)),
                                                                 VP_Setting_GetNetIndex((MN_DUAL_SYS_E)(MN_DUAL_SYS_1+(user_data - MMIVP_PDA_SETTINGS_NET1))));
            MMIAPICONNECTION_CreateLinklistWin(MMICONNECTION_ACCOUNT_BROWSER, list_index,
                                               (MN_DUAL_SYS_1+(user_data - MMIVP_PDA_SETTINGS_NET1)), MMIVP_ConnectionCallback);
            break;
            
        case MMIVP_PDA_SETTINGS_RTCP_RTP:
            MMK_CreateWin((uint32 *) MMIVP_RTP_RTCP_WIN_TAB, PNULL);
            break;
            
        case MMIVP_PDA_SETTINGS_AGENT:
            MMK_CreateWin((uint32 *) MMIVP_AGENT_WIN_TAB,PNULL);
            break;


#ifdef BROWSER_SUPPORT
        case MMIVP_PDA_SETTINGS_FLOWRATE:
            MMIAPICL_OpenGprsFlowRateWin();
            break;
#endif
#endif
            
        case MMIVP_PDA_SETTINGS_RESET:
            MMIPUB_OpenQueryWinByTextId(TXT_RESET_QUERY,IMAGE_PUBWIN_QUERY,PNULL,PNULL);
            break;
    #if defined MMI_VIDEOPLAYER_HELP_SUPPORT    
        case MMIVP_PDA_SETTINGS_HELP:
            MMK_CreateWin((uint32 *)MMIVP_HELP_INFO_WIN_TAB, PNULL);
            break;
    #endif        
        default:
            break;
        }
        break;
        
    case MSG_PROMPTWIN_OK:
        MMIPUB_CloseQuerytWin(PNULL);
        MMIAPIVP_ResetSettingInfo(); 
        MMIAPISET_SetMultimVolume(MMISET_VOL_DEFAULT_MULTIM);//NEWMS00121246
        MMIAPISET_InitAudioDevice(MMISET_VOL_DEFAULT_MULTIM); 
        MMIAPIVP_FSMUpdateCurrentVol(s_video_player_control_info.vp_handle,MMISET_VOL_DEFAULT_MULTIM);
        index = GUILIST_GetCurItemIndex(ctrl_id);//NEWMS00161440 begin
        GUILIST_RemoveAllItems(ctrl_id);
        InitVpPdaSettingsListCtrl();
        GUILIST_SetTopItemOffset(ctrl_id, list_offset_y);
        GUILIST_SetCurItemIndex(ctrl_id, index);//NEWMS00161440 end

        //need update current vol about  fsm_ptr after reset settings.
        //MMIAPIVP_FSMUpdateCurrentVol(s_video_player_control_info.vp_handle,MMIAPISET_GetMultimVolume());
        //xin.li_clean_popwin
        MMIPUB_OpenAlertSuccessWin(TXT_SUCCESS);
        break;
        
    case MSG_PROMPTWIN_CANCEL:
        MMIPUB_CloseQuerytWin(PNULL);
        break;
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    default:
        result = MMI_RESULT_FALSE;
    break;
    }
    
    return (result);
}



/*****************************************************************************/
//  Description : init pda style settings list ctrl
//  Global resource dependence : 
//  Author: xin.li
//  Note: 
/*****************************************************************************/
LOCAL void InitVpPdaSettingsListCtrl(void)
{
    MMI_CTRL_ID_T       ctrl_id = MMIVP_PDA_SETTINGS_LIST_CTRL_ID;
    #ifdef MMI_PDA_SUPPORT 
    MMI_IMAGE_ID_T      image_id = IMAGE_NULL;
    MMI_TEXT_ID_T       text2_id = TXT_NULL;
    #endif
#ifdef MMI_STREAMING_PLAYER_SUPPORT
    uint8               link_index = 0;
    MMICONNECTION_LINKSETTING_DETAIL_T* link_setting_ptr = PNULL;
#endif
    //set max item
    GUILIST_SetMaxItem(ctrl_id, MMIVP_PDA_SETTINGS_MAX, FALSE);
#ifdef MMI_PDA_SUPPORT
    if (VP_PLAY_FROM_LOCALLIST == s_video_player_control_info.play_type)//those works only when file in locallist 
    {
        //REPEAT
        AppendVpPdaSettingsItem(
            ctrl_id,
            TXT_COMMON_REPEAT,
            GetTextIdByRepeatMode(VP_Setting_GetRepeatPlayMode()),
            IMAGE_COMMON_RIGHT_ARROW,
            (uint32)MMIVP_PDA_SETTINGS_REPEAT);
    
        //RANDOM
        if(VP_Setting_GetIsRandomPlayMode())
        {
            text2_id = TXT_OPEN;
            image_id = IMAGE_CHECK_SELECTED_ICON;
        }
        else
        {
            text2_id = TXT_CLOSE;
            image_id = IMAGE_CHECK_UNSELECTED_ICON;
        }
        AppendVpPdaSettingsItem(
            ctrl_id,
            TXT_SET_SHUFFLE,
            text2_id,
            image_id,
        (uint32)MMIVP_PDA_SETTINGS_RANDOM);
    }
#endif   
	AppendVpPdaSettingsItem(
        ctrl_id,
        TXT_VP_SET_PLAY_METHOD,
        GetTextIdByPlaybackMode(VP_Setting_GetPlaybackMode()),
        IMAGE_COMMON_RIGHT_ARROW,
        (uint32)MMIVP_PDA_SETTINGS_PLAY_METHOD);

#ifdef MMI_STREAMING_PLAYER_SUPPORT
#ifndef MMI_MULTI_SIM_SYS_SINGLE

    //sim 1
    link_index = VP_Setting_GetNetIndex(MN_DUAL_SYS_1);
    link_setting_ptr = MMIAPICONNECTION_GetLinkSettingItemByIndex(MN_DUAL_SYS_1, link_index);

    AppendVpPdaSettingsItemEx(
        ctrl_id,
        TXT_COMMON_SET_NET_SIM1,
        link_setting_ptr->name,
        link_setting_ptr->name_len,
        IMAGE_COMMON_RIGHT_ARROW,
        (uint32)MMIVP_PDA_SETTINGS_NET1);

    GUILIST_SetItemGray(ctrl_id, MMIVP_PDA_SETTINGS_NET1, (BOOLEAN)(!MMIAPIPHONE_IsSimAvailable(MN_DUAL_SYS_1)));
    
    //sim 2
    link_index = VP_Setting_GetNetIndex(MN_DUAL_SYS_2);
    link_setting_ptr = MMIAPICONNECTION_GetLinkSettingItemByIndex(MN_DUAL_SYS_2, link_index);

    AppendVpPdaSettingsItemEx(
        ctrl_id,
        TXT_COMMON_SET_NET_SIM2,
        link_setting_ptr->name,
        link_setting_ptr->name_len,
        IMAGE_COMMON_RIGHT_ARROW,
        (uint32)MMIVP_PDA_SETTINGS_NET2);

    GUILIST_SetItemGray(ctrl_id, MMIVP_PDA_SETTINGS_NET2, (BOOLEAN)(!MMIAPIPHONE_IsSimAvailable(MN_DUAL_SYS_2)));
    
#if defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
    //sim 3
    link_index = VP_Setting_GetNetIndex(MN_DUAL_SYS_3);
    link_setting_ptr = MMIAPICONNECTION_GetLinkSettingItemByIndex(MN_DUAL_SYS_3, link_index);

    AppendVpPdaSettingsItemEx(
        ctrl_id,
        TXT_COMMON_SET_NET_SIM3,
        link_setting_ptr->name,
        link_setting_ptr->name_len,
        IMAGE_COMMON_RIGHT_ARROW,
        (uint32)MMIVP_PDA_SETTINGS_NET3);

    GUILIST_SetItemGray(ctrl_id, MMIVP_PDA_SETTINGS_NET3, (BOOLEAN)(!MMIAPIPHONE_IsSimAvailable(MN_DUAL_SYS_3)));
#endif
#if defined(MMI_MULTI_SIM_SYS_QUAD)
    //sim 4
    link_index = VP_Setting_GetNetIndex(MN_DUAL_SYS_4);
    link_setting_ptr = MMIAPICONNECTION_GetLinkSettingItemByIndex(MN_DUAL_SYS_4, link_index);

    AppendVpPdaSettingsItemEx(
        ctrl_id,
        TXT_COMMON_SET_NET_SIM4,
        link_setting_ptr->name,
        link_setting_ptr->name_len,
        IMAGE_COMMON_RIGHT_ARROW,
        (uint32)MMIVP_PDA_SETTINGS_NET4);

    GUILIST_SetItemGray(ctrl_id, MMIVP_PDA_SETTINGS_NET4, (BOOLEAN)(!MMIAPIPHONE_IsSimAvailable(MN_DUAL_SYS_4)));
#endif
#else
    AppendVpPdaSettingsItem(
        ctrl_id,
        TXT_NETWORK_CONNECT_SET,
        TXT_NULL, //@xin to do
        IMAGE_COMMON_RIGHT_ARROW,
        (uint32)MMIVP_PDA_SETTINGS_NET1);
#endif
    
    //RTCP_RTP
    AppendVpPdaSettingsItem(
        ctrl_id,
        TXT_VP_SET_RTCP,
        TXT_NULL,
        IMAGE_NULL,
        (uint32)MMIVP_PDA_SETTINGS_RTCP_RTP);
    
    //AGENT
    AppendVpPdaSettingsItem(
        ctrl_id,
        TXT_COMM_PROXY,
        TXT_NULL,
        IMAGE_NULL,
        (uint32)MMIVP_PDA_SETTINGS_AGENT);
    
#ifdef BROWSER_SUPPORT
    //FLOWRATE
    AppendVpPdaSettingsItem(
        ctrl_id,
        TXT_COMMON_FLOW_STATISTICS,
        TXT_NULL,
        IMAGE_COMMON_RIGHT_ARROW,
        (uint32)MMIVP_PDA_SETTINGS_FLOWRATE);
#endif
#endif //MMI_STREAMING_PLAYER_SUPPORT
    
    //RESET
    AppendVpPdaSettingsItem(
        ctrl_id,
        TXT_COMMON_SET_RESET,
        TXT_NULL,
        IMAGE_COMMON_RIGHT_ARROW,
        (uint32)MMIVP_PDA_SETTINGS_RESET);

#if defined MMI_VIDEOPLAYER_HELP_SUPPORT    
    //HELP
    AppendVpPdaSettingsItem(
        ctrl_id,
        TXT_HELP,
        TXT_NULL,
        IMAGE_COMMON_RIGHT_ARROW,
        (uint32)MMIVP_PDA_SETTINGS_HELP);
#endif
    
}

/*****************************************************************************/
//  Description : append settings item
//  Global resource dependence : 
//  Author: xin.li
//  Note: 
/*****************************************************************************/
LOCAL void AppendVpPdaSettingsItem(
                                 MMI_CTRL_ID_T  ctrl_id,    //控件ID
                                 MMI_TEXT_ID_T  text_1,     //主文本
                                 MMI_TEXT_ID_T  text_2,     //副文本
                                 MMI_IMAGE_ID_T image_id,   //图标ID
                                 uint32         user_data   //用户数据
                                 )
{
    GUILIST_ITEM_T      item_t = {0};
    GUILIST_ITEM_DATA_T item_data = {0};
    
    item_t.item_data_ptr = &item_data;
    item_t.user_data     = user_data;
    
    if(TXT_NULL != text_2)
    {
#ifdef TOUCH_PANEL_SUPPORT
        item_t.item_style    = GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_ANIM;
#else
        item_t.item_style    = GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT;
#endif
        
        //text 1
        item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;
        item_data.item_content[0].item_data.text_id = text_1;
        //text 2
#ifdef TOUCH_PANEL_SUPPORT        
        {
            item_data.item_content[2].item_data_type    = GUIITEM_DATA_TEXT_ID;
            item_data.item_content[2].item_data.text_id = text_2;
        }
        
        //anim
        item_data.item_content[1].item_data_type    = GUIITEM_DATA_IMAGE_ID;
        item_data.item_content[1].item_data.image_id = image_id;
#else
        item_data.item_content[1].item_data_type    = GUIITEM_DATA_TEXT_ID;
        item_data.item_content[1].item_data.text_id = text_2;
#endif
    }
    else
    {
        //single line style
        item_t.item_style    = GUIITEM_STYLE_ONE_LINE_TEXT_ICON;
        
        item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;
        item_data.item_content[0].item_data.text_id = text_1;
        
        //anim
        //item_data.item_content[1].item_data_type    = GUIITEM_DATA_IMAGE_ID;
        //item_data.item_content[1].item_data.image_id = image_id;
    }
    
    GUILIST_AppendItem(ctrl_id, &item_t);
}

/*****************************************************************************/
//  Description : append settings item
//  Global resource dependence : 
//  Author: xin.li
//  Note: 
/*****************************************************************************/
LOCAL void AppendVpPdaSettingsItemEx(
                                 MMI_CTRL_ID_T  ctrl_id,            //控件ID
                                 MMI_TEXT_ID_T  text_1,             //主文本
                                 wchar          *text2_buf_ptr,     //副文本数据
                                 uint16         text2_buf_len,      //副文本长度
                                 MMI_IMAGE_ID_T image_id,           //图标ID
                                 uint32         user_data           //用户数据
                                 )
{
    GUILIST_ITEM_T      item_t = {0};
    GUILIST_ITEM_DATA_T item_data = {0};
    
    item_t.item_style    = GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_ANIM;
    item_t.item_data_ptr = &item_data;
    item_t.user_data     = user_data;
    
    //text 1
    item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[0].item_data.text_id = text_1;
    //text 2
    if(PNULL != text2_buf_ptr)
    {
        item_data.item_content[2].item_data_type    = GUIITEM_DATA_TEXT_BUFFER;
        item_data.item_content[2].item_data.text_buffer.wstr_ptr = text2_buf_ptr;
        item_data.item_content[2].item_data.text_buffer.wstr_len = text2_buf_len;
    }
    
    //anim
    item_data.item_content[1].item_data_type    = GUIITEM_DATA_IMAGE_ID;
    item_data.item_content[1].item_data.text_id = image_id;
    
    GUILIST_AppendItem(ctrl_id, &item_t);
}



#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : get text_id by Repeat mode
//  Global resource dependence : 
//  Author: xin.li
//  Note: 
/*****************************************************************************/
LOCAL MMI_TEXT_ID_T GetTextIdByRepeatMode(VP_REPEAT_PLAY_MODE_E repeat_mode)
{
    MMI_TEXT_ID_T   text_id = TXT_NULL;
    
    switch(repeat_mode)
    {
    case VP_REPEAT_PLAY_MODE_ALL:
        text_id = TXT_COMMON_SET_REPEAT_ALL;
        break;
        
    case VP_REPEAT_PLAY_MODE_ONE:
        text_id = TXT_VP_SINGLE;
        break;
        
    case VP_REPEAT_PLAY_MODE_CLOSE:
        text_id = TXT_CLOSE;
        break;
        
    default:
        text_id = TXT_CLOSE;
        break;
    }
    
    return text_id;
}



/*****************************************************************************/
//  Description : handle repeat set window message
//  Global resource dependence : 
//  Author: xin.li
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleVpRepeatSetWin(
                                     MMI_WIN_ID_T       win_id,     
                                     MMI_MESSAGE_ID_E   msg_id, 
                                     DPARAM             param
                                     )
{
    uint16          index = 0;
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T   ctrl_id = MMIVP_REPEAT_SET_LIST_CTRL_ID;
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        AppendRepeatListItem();
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;
        
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_WEB:
    case MSG_APP_OK:
        GUILIST_GetSelectedItemIndex(ctrl_id, &index, 1);
        
        VP_Setting_SetRepeatPlayMode((VP_REPEAT_PLAY_MODE_E)index);
        MMIAPIVP_FSMUpdatePlayMode(
            s_video_player_control_info.vp_handle,
            (VP_REPEAT_PLAY_MODE_E)index,
            VP_Setting_GetIsRandomPlayMode());
        
        MMK_CloseWin(win_id);
        break;
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return (result);
}



/*****************************************************************************/
//  Description : append repeat list item
//  Global resource dependence : 
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL void AppendRepeatListItem(void)
{
    uint16          cur_selection = 0;
    MMI_CTRL_ID_T   ctrl_id = MMIVP_REPEAT_SET_LIST_CTRL_ID;
    
    GUILIST_SetMaxItem(ctrl_id, VP_REPEAT_PLAY_MODE_MAX, FALSE);
    
    MMIAPISET_AppendListItemByTextId(TXT_CLOSE, STXT_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
    MMIAPISET_AppendListItemByTextId(TXT_COMMON_SET_REPEAT_ALL, STXT_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
    MMIAPISET_AppendListItemByTextId(TXT_VP_SINGLE, STXT_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
    
    //get current lyric set
    switch(VP_Setting_GetRepeatPlayMode())
    {
    case VP_REPEAT_PLAY_MODE_CLOSE:
        cur_selection = VP_REPEAT_PLAY_MODE_CLOSE;
        break;
        
    case VP_REPEAT_PLAY_MODE_ALL:
        cur_selection = VP_REPEAT_PLAY_MODE_ALL;
        break;
        
    case VP_REPEAT_PLAY_MODE_ONE:
        cur_selection = VP_REPEAT_PLAY_MODE_ONE;
        break;
        
    default:
        break;
    }
    //set select item
    GUILIST_SetSelectedItem(ctrl_id, cur_selection, TRUE);
    
    //set current item
    GUILIST_SetCurItemIndex(ctrl_id, cur_selection);
}

/*****************************************************************************/
// 	Description : enter bookmark list marked status
//	Global resource dependence : none
//  Author: fen.xie
//	Note:
/*****************************************************************************/
LOCAL void VPEnterBookmarkListMarked(
                               MMI_CTRL_ID_T  list_ctrl_id
                               )//NEWMS00125002
{
    if (0 == GUILIST_GetSelectedItemIndex(list_ctrl_id,PNULL,0))
    {
        GUILIST_SetTextListMarkable(list_ctrl_id,TRUE);//set list enable mark
        GUILIST_SetMaxSelectedItem(list_ctrl_id, VP_LocalList_GetNum(VP_LOCALLIST_USER));//set mark max number
    }
     s_is_markall = FALSE;
    GUIFORM_SetChildDisplay(VP_USER_LOCALLIST_FORM_CTRL_ID, MMICOMMON_SOFTKEY_CTRL_ID, GUIFORM_CHILD_DISP_NORMAL);
    GUIWIN_SetTitleButtonState(MMIVP_USER_LOCALLIST_WIN_ID, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_INVISIBLE, FALSE);
    GUISOFTKEY_SetTextId(MMIVP_USER_LOCALLIST_WIN_ID, MMICOMMON_SOFTKEY_CTRL_ID, TXT_MARK_ALL, TXT_DELETE, STXT_CANCEL, FALSE );
    GUILIST_SetAllSelected(list_ctrl_id,FALSE);
}
/*****************************************************************************/
// 	Description : exit bookmark list marked status
//	Global resource dependence : none
//  Author: fen.xie
//	Note:
/*****************************************************************************/
LOCAL void VPExitBookmarkListMarked(
                               MMI_CTRL_ID_T  list_ctrl_id
                               )
{
    s_is_markall = FALSE;
    GUIFORM_SetChildDisplay(VP_USER_LOCALLIST_FORM_CTRL_ID, MMICOMMON_SOFTKEY_CTRL_ID, GUIFORM_CHILD_DISP_HIDE);
    GUIWIN_SetTitleButtonState(MMIVP_USER_LOCALLIST_WIN_ID, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
    GUILIST_SetAllSelected(list_ctrl_id,FALSE);
    GUILIST_SetTextListMarkable(list_ctrl_id,FALSE);
}
#endif//MMI_PDA_SUPPORT
#endif
// #endif //MMI_GUI_STYLE_TYPICAL  leimingma


/*****************************************************************************/
//  Description : get text_id by Playback Mode
//  Global resource dependence : 
//  Author: xin.li
//  Note: 
/*****************************************************************************/
LOCAL MMI_TEXT_ID_T GetTextIdByPlaybackMode(VP_PLAYBACK_MODE_E playback_mode)
{
    MMI_TEXT_ID_T   text_id = TXT_NULL;
    
    switch(playback_mode)
    {
    case VP_PLAYBACK_MODE_AUDIO_VIDEO_SYN:
        text_id = TXT_VP_AUDIO_VIDEO_SYN;
        break;
        
    case VP_PLAYBACK_MODE_SCREEN_FLOW:
        text_id = TXT_VP_SCREEN_FLOW;
        break;    
        
    default:
        text_id = TXT_VP_AUDIO_VIDEO_SYN;
        break;
    }
    
    return text_id;
}

/*****************************************************************************/
//  Description : handle playback mode set window message
//  Global resource dependence : 
//  Author: xin.li
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleVpPlaybackModeSetWin(
                                     MMI_WIN_ID_T       win_id,     
                                     MMI_MESSAGE_ID_E   msg_id, 
                                     DPARAM             param
                                     )
{
    uint16          index = 0;
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T   ctrl_id = MMIVP_PLAYBACK_MODE_SET_LIST_CTRL_ID;
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        AppendPlaybackModeListItem();
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;
        
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_WEB:
    case MSG_APP_OK:
        GUILIST_GetSelectedItemIndex(ctrl_id, &index, 1);
        VP_Setting_SetPlaybackMode((VP_PLAYBACK_MODE_E)index);  
		MMIAPIVP_FSMUpdatePlayBackMode(
            s_video_player_control_info.vp_handle,
            (VP_PLAYBACK_MODE_E)index);
        MMK_CloseWin(win_id);
        break;
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return (result);
}


/*****************************************************************************/
//  Description : append palyback mode list item
//  Global resource dependence : 
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL void AppendPlaybackModeListItem(void)
{
    uint16          cur_selection = 0;
    MMI_CTRL_ID_T   ctrl_id = MMIVP_PLAYBACK_MODE_SET_LIST_CTRL_ID;
    
    GUILIST_SetMaxItem(ctrl_id, VP_PLAYBACK_MODE_MAX, FALSE);
    
    MMIAPISET_AppendListItemByTextId(TXT_VP_AUDIO_VIDEO_SYN, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
    MMIAPISET_AppendListItemByTextId(TXT_VP_SCREEN_FLOW, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);    
    
    //get current lyric set
    switch(VP_Setting_GetPlaybackMode())
    {
    case VP_PLAYBACK_MODE_AUDIO_VIDEO_SYN:
        cur_selection = VP_PLAYBACK_MODE_AUDIO_VIDEO_SYN;
        break;
        
    case VP_PLAYBACK_MODE_SCREEN_FLOW:
        cur_selection = VP_PLAYBACK_MODE_SCREEN_FLOW;
        break;       
   
        
    default:
        break;
    }
    //set select item
    GUILIST_SetSelectedItem(ctrl_id, cur_selection, TRUE);
    
    //set current item
    GUILIST_SetCurItemIndex(ctrl_id, cur_selection);
}

#if defined (MMI_VIDEO_PLAYER_MINI_SUPPORT) 
#if defined(MMI_VIDEO_PLAYER_MINI_LAYOUT)
/*****************************************************************************/
//  Description : handle repeat mode set window message
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/

LOCAL MMI_RESULT_E HandleVPrepeatModeWin(
                        MMI_WIN_ID_T        win_id,     
                        MMI_MESSAGE_ID_E    msg_id, 
                        DPARAM              param
                        )
{
    MMI_RESULT_E            result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T           ctrl_id = VP_PLAY_MODE_REPEAT_CTRL_ID;
    VP_REPEAT_PLAY_MODE_E   cur_selection = VP_REPEAT_PLAY_MODE_MAX;


    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        AppendVPRepeatModeListItem(ctrl_id);
        MMK_SetAtvCtrl(win_id,ctrl_id);

        break;

    case MSG_CTL_MIDSK:
    case MSG_CTL_PENOK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
    case MSG_APP_OK:
        //get the selected item
        GUILIST_GetSelectedItemIndex(ctrl_id,&cur_selection,1);
        
        VP_Setting_SetRepeatPlayMode((VP_REPEAT_PLAY_MODE_E)cur_selection);

        MMIAPIVP_FSMUpdatePlayMode(
            s_video_player_control_info.vp_handle,
            (VP_REPEAT_PLAY_MODE_E)cur_selection,
            VP_Setting_GetIsRandomPlayMode());
        


        MMK_CloseWin( win_id );
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin( win_id );
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }

    return (result);
}
/*****************************************************************************/
//  Description : handle shuffle mode set window message
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleVPShuffleModeWin(
                        MMI_WIN_ID_T        win_id,     
                        MMI_MESSAGE_ID_E    msg_id, 
                        DPARAM              param
                        )
{
    MMI_RESULT_E                result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T               ctrl_id = VP_PLAY_MODE_SHUFFLE_CTRL_ID;
    VP_SHUFFLE_PLAY_MODE_E      cur_selection = VP_SHUFFLE_PLAY_MODE_MAX;
    

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        AppendVPShuffleModeListItem(ctrl_id);
        MMK_SetAtvCtrl(win_id,ctrl_id);
        break;

    case MSG_CTL_MIDSK:
    case MSG_CTL_PENOK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
    case MSG_APP_OK:
        //get the selected item
        GUILIST_GetSelectedItemIndex(ctrl_id,&cur_selection,1);
        
        VP_Setting_SetIsRandomPlayMode((VP_SHUFFLE_PLAY_MODE_E)cur_selection);

        //success!
        //MMIPUB_OpenAlertWinByTextId(PNULL,TXT_COMPLETE,TXT_NULL,IMAGE_PUBWIN_SUCCESS,&alert_win_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);

        MMK_CloseWin( win_id );
        break;

    case MSG_CTL_CANCEL:
        MMK_CloseWin( win_id );
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }

    return (result);
}
#endif
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void AppendVPRepeatModeListItem(
                    MMI_CTRL_ID_T       ctrl_id
                    )
{
    VP_REPEAT_PLAY_MODE_E  cur_selection = VP_Setting_GetRepeatPlayMode();
    //UINT16                 cur_index = 0;
    GUILIST_SetMaxItem(ctrl_id,VP_REPEAT_PLAY_MODE_MAX, FALSE );

    MMIAPISET_AppendListItemByTextId(TXT_CLOSE,STXT_OK,ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
    MMIAPISET_AppendListItemByTextId(TXT_COMMON_SET_REPEAT_ALL,STXT_OK,ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
    MMIAPISET_AppendListItemByTextId(TXT_COMM_REPEAT_ONE,STXT_OK,ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);


    //set select item
    GUILIST_SetSelectedItem(ctrl_id,cur_selection,TRUE);

    //set current item
    GUILIST_SetCurItemIndex(ctrl_id,cur_selection);
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void AppendVPShuffleModeListItem(
                            MMI_CTRL_ID_T       ctrl_id
                            )
{   
    VP_SHUFFLE_PLAY_MODE_E  cur_selection = VP_Setting_GetIsRandomPlayMode();

    GUILIST_SetMaxItem(ctrl_id,VP_SHUFFLE_PLAY_MODE_MAX, FALSE );

    MMIAPISET_AppendListItemByTextId(TXT_CLOSE,TXT_COMMON_OK,ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
    MMIAPISET_AppendListItemByTextId(TXT_OPEN,TXT_COMMON_OK,ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);


    //set select item
    GUILIST_SetSelectedItem(ctrl_id,cur_selection,TRUE);

    //set current item
    GUILIST_SetCurItemIndex(ctrl_id,cur_selection);
}

/*****************************************************************************/
//  Description : 亮度调节
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void AdjustBrightness(MMICOM_PANEL_OPEN_TYPE_E turn_bright_type)
{
    MMICOM_PANEL_INFO_T bright_panel = {0};   


    bright_panel.cur_value = VP_Setting_GetBrightness();
    bright_panel.min_value = 0;
    bright_panel.max_value = MMISET_CONTRAST_SEVEN;
    bright_panel.panel_type = MMICOM_PANEL_BRIGHTNESS;
    bright_panel.set_callback = AdjustBrightCallBack;
    bright_panel.open_type = turn_bright_type;
    bright_panel.x = -1;
    bright_panel.y = -1;
    MMIAPICOM_OpenPanelChildWin(MMIVP_PLAYER_WIN_ID,&bright_panel);

}
/*****************************************************************************/
//  Description : 显示OSD
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void DisplayOSD(void)
{
    if (!MMIAPICOM_IsPanelOpened())
    {
        DisappearVPWin(MMIVP_PLAYER_WIN_ID,FALSE,TRUE);
        StartVPWinDisplayerTimer();
    }
}

#endif  //MMI_VIDEO_PLAYER_MINI_SUPPORT //6530_MINI
/*****************************************************************************/
//  Description : 显示OSD
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIVP_ResetSetSeekTime(void)
{
	s_video_player_control_info.seek_time = 0;
}

/*****************************************************************************/
//  Description : post message to the main win 
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIVP_SDCardPlugOut(void)
{
    MMK_PostMsg(MMIVP_MAIN_PLAY_WIN_ID, MSG_KEYUP_SDCARD_DETECT, PNULL,0);
}
/*****************************************************************************/
//  Description : get seek time
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMIVP_GetSeekTime(void)
{
	return s_video_player_control_info.seek_time;	
}

#ifdef DC_WATCH_UI_SUPPORT

LOCAL VP_INTERFACE_PLAYING_INFO_T *s_video_player_control_info_fordv = &s_video_player_control_info;

/*****************************************************************************/
//  Description: get vp handle for video play
//  Parameter: [In] None
//             [Out] None
//             [Return] vp_handle: uint32, vp handle.
//  Author: fulu.song
//  Note: API to operate vp fsm from other module without vp ui.
/*****************************************************************************/
PUBLIC uint32 WatchVP_GetHandle_ForDv(void)
{
    return s_video_player_control_info_fordv->vp_handle;
}

/*****************************************************************************/
//  Description: init vp for video play
//  Parameter: [In] VP_CALLBACK: function callback for caller to handle vp msg.
//             [Out] None
//             [Return] vp_handle: uint32, vp handle.
//  Author: fulu.song
//  Note: API to operate vp fsm from other module without vp ui.
/*****************************************************************************/
PUBLIC uint32 WatchVP_Init_ForDv(VP_CALLBACK vpcallback)
{
    SCI_MEMSET(s_video_player_control_info_fordv,0,sizeof(VP_INTERFACE_PLAYING_INFO_T));
    s_video_player_control_info_fordv->vp_entry = VP_ENTRY_FROM_FILE;
    s_video_player_control_info_fordv->station_text_id = TXT_NULL;
    s_video_player_control_info_fordv->support_type = VP_SUPPORTED_BOTH;
    s_video_player_control_info_fordv->vp_handle = MMIAPIVP_FSMInit(vpcallback);

    return s_video_player_control_info_fordv->vp_handle;
}

/*****************************************************************************/
//  Description: start play for video play
//  Parameter: [In] filepath: video file path,  fileLen: file length.
//             [Out] None
//             [Return] BOOLEAN: execution result. TRUE if OK.
//  Author: fulu.song
//  Note: API to operate vp fsm from other module without vp ui.
/*****************************************************************************/
PUBLIC BOOLEAN WatchVP_Play_ForDv(const wchar *filePath, uint32 fileLen)
{
    if (filePath == NULL || fileLen == 0)
        return FALSE;
    do
    {
        VP_PLAY_PARAM_T play_param = {0};
        GUI_RECT_T full_rect = {0};
        play_param.dual_sys = 0;
        play_param.is_random_play = FALSE;
        play_param.mode = VP_REPEAT_PLAY_MODE_CLOSE;
        play_param.url_ptr = PNULL;
        play_param.display_param.audioSupport = 1;
        play_param.display_param.videoSupport = 1;
        play_param.is_scal_up = TRUE;
        play_param.display_param.RotateAngle = LCD_ANGLE_0;
        play_param.vol = 7;
        play_param.play_type = VP_PLAY_FROM_FILE;
        play_param.full_path_ptr = filePath;
        play_param.full_path_len = fileLen;
        play_param.is_scal_up = FALSE;

        full_rect = MMITHEME_GetFullScreenRect();

        if (play_param.display_param.RotateAngle == 1)
        {
            full_rect.bottom = MMITHEME_GetFullScreenRect().right;
            full_rect.right = MMITHEME_GetFullScreenRect().bottom;
        }
        play_param.display_param.disp_rect.x = 0;
        play_param.display_param.disp_rect.y = 0;
        play_param.display_param.disp_rect.dx = full_rect.right - full_rect.left + 1;
        play_param.display_param.disp_rect.dy = full_rect.bottom - full_rect.top + 1;

        play_param.display_param.target_rect.x = play_param.display_param.disp_rect.x;
        play_param.display_param.target_rect.y = play_param.display_param.disp_rect.y;
        play_param.display_param.target_rect.dx = play_param.display_param.disp_rect.dx;
        play_param.display_param.target_rect.dy = play_param.display_param.disp_rect.dy;
        play_param.display_param.disp_mode = DPLAYER_DISP_FULLSCREEN;
        TRACE_APP_VIDEOS("WatchVP_Play_ForDv  play_param.display_param.RotateAngle = %d",play_param.display_param.RotateAngle);
        TRACE_APP_VIDEOS("WatchVP_Play_ForDv  full_rect = %d, %d, %d, %d", full_rect.left, full_rect.top, full_rect.right, full_rect.bottom);
        TRACE_APP_VIDEOS("WatchVP_Play_ForDv  begin play.");
        TRACE_APP_VIDEOS("WatchVP_Play_ForDv  disp_rect  = %d, %d, %d, %d.", 
        play_param.display_param.disp_rect.x,
        play_param.display_param.disp_rect.y,
        play_param.display_param.disp_rect.dx,
        play_param.display_param.disp_rect.dy);
        TRACE_APP_VIDEOS("WatchVP_Play_ForDv  target_rect  = %d, %d, %d, %d.", 
        play_param.display_param.target_rect.x,
        play_param.display_param.target_rect.y,
        play_param.display_param.target_rect.dx,
        play_param.display_param.target_rect.dy);
        TRACE_APP_VIDEOS("WatchVP_Play_ForDv  disp_mode  = %d.", play_param.display_param.disp_mode);
        TRACE_APP_VIDEOS("WatchVP_Play_ForDv  RotateAngle  = %d.", play_param.display_param.RotateAngle);
        TRACE_APP_VIDEOS("WatchVP_Play_ForDv  audioSupport  = %d.", play_param.display_param.audioSupport);
        TRACE_APP_VIDEOS("WatchVP_Play_ForDv  videoSupport  = %d.", play_param.display_param.videoSupport);
        TRACE_APP_VIDEOS("WatchVP_Play_ForDv  LcdId  = %d.", play_param.display_param.LcdId);
        TRACE_APP_VIDEOS("WatchVP_Play_ForDv  LcdBlkId  = %d.", play_param.display_param.LcdBlkId);
        TRACE_APP_VIDEOS("WatchVP_Play_ForDv  b_support_seek  = %d.", play_param.display_param.b_support_seek);
        TRACE_APP_VIDEOS("WatchVP_Play_ForDv  disp_data_op  = %d.", play_param.display_param.disp_data_op);
        TRACE_APP_VIDEOS("WatchVP_Play_ForDv  b_no_osd  = %d.", play_param.display_param.b_no_osd);
        TRACE_APP_VIDEOS("WatchVP_Play_ForDv  end play.");

        if(!MMIAPIVP_FSMPlay((VP_HANDLE)WatchVP_GetHandle_ForDv(),&play_param))
        {
            return FALSE;
        }
    } while(0);

    return TRUE;
}

/*****************************************************************************/
//  Description: pause play for video play
//  Parameter: [In] None
//             [Out] None
//             [Return] BOOLEAN: execution result. TRUE if OK.
//  Author: fulu.song
//  Note: API to operate vp fsm from other module without vp ui.
/*****************************************************************************/
PUBLIC BOOLEAN WatchVP_Pause_ForDv(void)
{
    if (WatchVP_GetHandle_ForDv() == 0)
        return FALSE;
    return MMIAPIVP_Pause(WatchVP_GetHandle_ForDv());
}

/*****************************************************************************/
//  Description: resume play for video play
//  Parameter: [In] None
//             [Out] None
//             [Return] BOOLEAN: execution result. TRUE if OK.
//  Author: fulu.song
//  Note: API to operate vp fsm from other module without vp ui.
/*****************************************************************************/
PUBLIC BOOLEAN WatchVP_Resume_ForDv(void)
{
    if (WatchVP_GetHandle_ForDv() == 0)
        return FALSE;
    return MMIAPIVP_Resume(WatchVP_GetHandle_ForDv());
}

/*****************************************************************************/
//  Description: stop play for video play
//  Parameter: [In] None
//             [Out] None
//             [Return] BOOLEAN, execution result. TRUE if OK.
//  Author: fulu.song
//  Note: API to operate vp fsm from other module without vp ui.
/*****************************************************************************/
PUBLIC BOOLEAN WatchVP_Stop_ForDv(void)
{
    uint32 handle = WatchVP_GetHandle_ForDv();
    TRACE_APP_VIDEOS("WatchVP_Stop_ForDv begin");
    if (handle== 0)
        return FALSE;
    SCI_MEMSET(s_video_player_control_info_fordv, 0, sizeof(VP_INTERFACE_PLAYING_INFO_T));
    MMIAPIVP_FSMExit(handle);
    TRACE_APP_VIDEOS("WatchVP_Stop_ForDv end");
    return TRUE;
}
#endif //end #ifdef DC_WATCH_UI_SUPPORT

#endif //MINI_FUNCTION
