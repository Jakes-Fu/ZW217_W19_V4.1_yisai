/*****************************************************************************
** File Name:      mmivt_wintab.c                                                   *
** Author:                                                                   *
** Date:           2/7/2010                                                *
** Copyright:      2010 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:                                                              *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 12/7/2005       bin.ji       Create
******************************************************************************/

//#ifndef _MMIVT_WINTAB_C_
//#define _MMIVT_WINTAB_C_


/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
//#include "std_header.h"

//#include "mmivt_export.h"
#include "mmi_app_vt_trc.h"
#ifdef VT_SUPPORT
#include "mmivt_ui.h"
#include "mmivt_id.h"
#include "mmivt_nv.h"
#include "mmivt_kernel_refAdapter.h"
#include "mmivt_kernel_324Adapter.h"
//临时
#include "mmicc_anim.h"
#include "mmicc_text.h"
#include "mmicc_image.h"
#include "mmicc_audio.h"
#include "mmivt_position.h"
#include "mmivt_anim.h"
#include "mmivt_text.h"
#include "mmivt_image.h"

#include "mmk_type.h"
#include "mmi_id.h"
#include "mmi_image.h"
#include "mmidisplay_data.h"
//#include "mmivt_export.h"
#include "window_parse.h"
#include "mmi_menutable.h"
#include "mmipub.h"
#include "mmi_default.h"
#include "mmicc_export.h"
#include "mmicc_audio.h"
#include "mmi_nv.h"
//#include "mmiset.h"
#include "mmiset_export.h"
#include "guicommon.h"
#include "guilabel.h"
#include "guirichtext.h"
#include "guiedit.h"
#include "guires.h"
#include "mmi_font.h"
#include "mmiidle_export.h"
//#include "mmiset_display.h"

#include "mmisms_export.h"
#include "mmicl_export.h"
#include "mmk_timer.h"

#include "mmiphone_export.h"
#include "mn_type.h"

#include "mmiaudio.h"
#include "mmidc_save.h"
#include "mmidc_flow.h"
#include "mmisd_export.h"
#include "mmiudisk_export.h"
#include "mmimtv_export.h"

#include "guibutton.h"
//#include "mmiset_call.h"
#include "mmiset_call_export.h"
#ifdef STREAMING_HS_SUPPORT // chenxiang 20100415
#include "mmismp_export.h"
#endif // STREAMING_HS_SUPPORT
//#include "mmieng.h"
#include "mmieng_export.h"
//#include "mmienvset.h"
#include "mmienvset_export.h"
#include "guictrl_api.h"
#include "mmidm_export.h"
#ifdef BLUETOOTH_SUPPORT    
#include "mmibt_text.h"
#endif
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#define VT_DISPLAY_TIME_LEN 9 //时间显示字串长度
#define VT_HOUR_TO_SECOND 3600 //小时换算为秒
#define VT_MINUTE_TO_SECOND 60 //分钟换算为秒

#define VT_ONE_SECOND_TIMEOUT 1000 //1秒计时
#define VT_THREE_SECONDS_TIMEOUT 3000 //3秒计时
#define VT_FIVE_SECONDS_TIMEOUT 5000 //5秒计时
#define VT_FALLBACK_WAIT_TIMEOUT 15000 //回落等待时间15秒

#define VT_VOLUME_NUM_LENGTH_MAX 10 //音量显示的最大长度

#define VT_VIDEO_OPTION_PARAM_LENGTH_MAX 10 //视频参数显示最大长度

#define VT_USERINPUT_LENTH_MAX 40 //user input 显示最大长度

#define VT_TIMEOUT_FOREVER  0xFFFFFF//timeout forever

#define VT_NT_RING_CON_ID 3 //本地振铃音ID

#define VT_MAIN_PLAY_BG 0x39A7 //呼入呼出时，动画背景色：56，52，56

#define VT_NO_NUM_AUTODIAL_MAX_TIME 1

#define VT_NON_VALID_DEST_AUTODIAL_MAX_TIME 10

#define VT_RECORD_NEED_SPACE_MIN MMI_DC_MIN_SPACE_SIZE //录制要求空闲空间最小值

#define VT_RECORD_FILE_ID_MAX 100000000 //录音文件id号最大值

#define VT_FILE_NAME_LEN_MAX 255 //录制文件名最大长度

#define VT_TRANSPARENT_COLOR MMI_OSD_TRANSPARENT_COLOR

#define VT_FILE_CREAT_FILE_COUNT_MAX  50// 创建文件重试次数

//position
#define BG_BANK_BOTH_RECT {{OPTION_BG_BANK_RECT_LEFT, OPTION_BG_BANK_RECT_TOP, OPTION_BG_BANK_RECT_RIGHT, OPTION_BG_BANK_RECT_BOTTOM}, \
                                                  {OPTION_BG_BANK_RECT_LEFT_H, OPTION_BG_BANK_RECT_TOP_H, OPTION_BG_BANK_RECT_RIGHT_H, OPTION_BG_BANK_RECT_BOTTOM_H}}
#define MAIN_PLAY_BOTH_RECT {{MAIN_PLAY_RECT_LEFT, MAIN_PLAY_RECT_TOP, MAIN_PLAY_RECT_RIGHT, MAIN_PLAY_RECT_BOTTOM}, \
                                                      {MAIN_PLAY_RECT_LEFT_H, MAIN_PLAY_RECT_TOP_H, MAIN_PLAY_RECT_RIGHT_H, MAIN_PLAY_RECT_BOTTOM_H}}
#define PHOTO_BOTH_RECT {{PHOTO_RECT_LEFT, PHOTO_RECT_TOP, PHOTO_RECT_RIGHT, PHOTO_RECT_BOTTOM}, \
                                              {PHOTO_RECT_LEFT_H, PHOTO_RECT_TOP_H, PHOTO_RECT_RIGHT_H, PHOTO_RECT_BOTTOM_H}}
#define AUDIO_OPTION_BUTTON_BOTH_RECT {{AUDIO_OPTION_BUTTON_LEFT, AUDIO_OPTION_BUTTON_TOP, AUDIO_OPTION_BUTTON_RIGHT, AUDIO_OPTION_BUTTON_BOTTOM}, \
                                                                           {AUDIO_OPTION_BUTTON_LEFT_H, AUDIO_OPTION_BUTTON_TOP_H, AUDIO_OPTION_BUTTON_RIGHT_H, AUDIO_OPTION_BUTTON_BOTTOM_H}}
#define DISPLAY_OPTION_BUTTON_BOTH_RECT {{DISPLAY_OPTION_BUTTON_LEFT, DISPLAY_OPTION_BUTTON_TOP, DISPLAY_OPTION_BUTTON_RIGHT, DISPLAY_OPTION_BUTTON_BOTTOM}, \
                                                                              {DISPLAY_OPTION_BUTTON_LEFT_H, DISPLAY_OPTION_BUTTON_TOP_H, DISPLAY_OPTION_BUTTON_RIGHT_H, DISPLAY_OPTION_BUTTON_BOTTOM_H}}
#define CAMERA_OPTION_BUTTON_BOTH_RECT {{CAMERA_OPTION_BUTTON_LEFT, CAMERA_OPTION_BUTTON_TOP, CAMERA_OPTION_BUTTON_RIGHT, CAMERA_OPTION_BUTTON_BOTTOM}, \
                                                                             {CAMERA_OPTION_BUTTON_LEFT_H, CAMERA_OPTION_BUTTON_TOP_H, CAMERA_OPTION_BUTTON_RIGHT_H, CAMERA_OPTION_BUTTON_BOTTOM_H}}
#define DISPMODE_OPTION_BUTTON_BOTH_RECT {{DISPMODE_OPTION_BUTTON_LEFT, DISPMODE_OPTION_BUTTON_TOP, DISPMODE_OPTION_BUTTON_RIGHT, DISPMODE_OPTION_BUTTON_BOTTOM}, \
                                                                                 {DISPMODE_OPTION_BUTTON_LEFT_H, DISPMODE_OPTION_BUTTON_TOP_H, DISPMODE_OPTION_BUTTON_RIGHT_H, DISPMODE_OPTION_BUTTON_BOTTOM_H}}
#define TWO_LINE_LABEL1_BOTH_RECT {{TWO_LINE_LABEL1_LEFT, TWO_LINE_LABEL1_TOP, TWO_LINE_LABEL1_RIGHT, TWO_LINE_LABEL1_BOTTOM}, \
                                                                  {TWO_LINE_LABEL1_LEFT_H, TWO_LINE_LABEL1_TOP_H, TWO_LINE_LABEL1_RIGHT_H, TWO_LINE_LABEL1_BOTTOM_H}}
#define TWO_LINE_LABEL2_BOTH_RECT {{TWO_LINE_LABEL2_LEFT, TWO_LINE_LABEL2_TOP, TWO_LINE_LABEL2_RIGHT, TWO_LINE_LABEL2_BOTTOM}, \
                                                                  {TWO_LINE_LABEL2_LEFT_H, TWO_LINE_LABEL2_TOP_H, TWO_LINE_LABEL2_RIGHT_H, TWO_LINE_LABEL2_BOTTOM_H}}
#define MT_ANIM_BOTH_RECT {{MT_ANIM_RECT_LEFT, MT_ANIM_RECT_TOP, MT_ANIM_RECT_RIGHT, MT_ANIM_RECT_BOTTOM}, \
                                                  {MT_ANIM_RECT_LEFT_H, MT_ANIM_RECT_TOP_H, MT_ANIM_RECT_RIGHT_H, MT_ANIM_RECT_BOTTOM_H}}

#define MT_BG_BANK_BOTH_RECT {{MT_BG_BANK_RECT_LEFT, MT_BG_BANK_RECT_TOP, MT_BG_BANK_RECT_RIGHT, MT_BG_BANK_RECT_BOTTOM},\
                                                         {MT_BG_BANK_RECT_LEFT_H, MT_BG_BANK_RECT_TOP_H, MT_BG_BANK_RECT_RIGHT_H, MT_BG_BANK_RECT_BOTTOM_H}}

#define MT_NAME_LABEL_BOTH_RECT {{INCOMING_CALL_NAME_LABEL_LEFT, INCOMING_CALL_NAME_LABEL_TOP, INCOMING_CALL_NAME_LABEL_RIGHT, INCOMING_CALL_NAME_LABEL_BOTTOM}, \
                                                                {INCOMING_CALL_NAME_LABEL_LEFT_H, INCOMING_CALL_NAME_LABEL_TOP_H, INCOMING_CALL_NAME_LABEL_RIGHT_H, INCOMING_CALL_NAME_LABEL_BOTTOM_H}}

#define MT_STATE_LABEL_BOTH_RECT {{INCOMING_CALL_STATE_LABEL_LEFT, INCOMING_CALL_STATE_LABEL_TOP, INCOMING_CALL_STATE_LABEL_RIGHT, INCOMING_CALL_STATE_LABEL_BOTTOM}, \
                                                                {INCOMING_CALL_STATE_LABEL_LEFT_H, INCOMING_CALL_STATE_LABEL_TOP_H, INCOMING_CALL_STATE_LABEL_RIGHT_H, INCOMING_CALL_STATE_LABEL_BOTTOM_H}}
#define CONNECTING_ANIM_BOTH_RECT MT_ANIM_BOTH_RECT 
#define CONNECTING_BG_BANK_BOTH_RECT MT_BG_BANK_BOTH_RECT 
#define CONNECTING_NAME_LABEL_BOTH_RECT MT_NAME_LABEL_BOTH_RECT 
#define CONNECTING_STATE_LABEL_BOTH_RECT MT_STATE_LABEL_BOTH_RECT
#define DISCONNECTING_ANIM_BOTH_RECT MT_ANIM_BOTH_RECT 
#define DISCONNECTING_BG_BANK_BOTH_RECT MT_BG_BANK_BOTH_RECT 
#define DISCONNECTING_NAME_LABEL_BOTH_RECT MT_NAME_LABEL_BOTH_RECT 
#define DISCONNECTING_STATE_LABEL_BOTH_RECT MT_STATE_LABEL_BOTH_RECT
#define LCD_BRIGHTNESS_BUTTON_BOTH_RECT {{VIDEO_OPTION_LCD_BTIGHTNESS_LEFT, VIDEO_OPTION_LCD_BTIGHTNESS_TOP, VIDEO_OPTION_LCD_BTIGHTNESS_RIGHT, VIDEO_OPTION_LCD_BTIGHTNESS_BOTTOM}, \
                                                                           {VIDEO_OPTION_LCD_BTIGHTNESS_LEFT_H, VIDEO_OPTION_LCD_BTIGHTNESS_TOP_H, VIDEO_OPTION_LCD_BTIGHTNESS_RIGHT_H, VIDEO_OPTION_LCD_BTIGHTNESS_BOTTOM_H}}
#define CONTRAST_BUTTON_BOTH_RECT {{VIDEO_OPTION_CONTRAST_LEFT, VIDEO_OPTION_CONTRAST_TOP, VIDEO_OPTION_CONTRAST_RIGHT, VIDEO_OPTION_CONTRAST_BOTTOM}, \
                                                                    {VIDEO_OPTION_CONTRAST_LEFT_H, VIDEO_OPTION_CONTRAST_TOP_H, VIDEO_OPTION_CONTRAST_RIGHT_H, VIDEO_OPTION_CONTRAST_BOTTOM_H}}
#define BRIGHTNESS_BUTTON_BOTH_RECT {{VIDEO_OPTION_BRIGHTNESS_LEFT, VIDEO_OPTION_BRIGHTNESS_TOP, VIDEO_OPTION_BRIGHTNESS_RIGHT, VIDEO_OPTION_BRIGHTNESS_BOTTOM}, \
                                                                        {VIDEO_OPTION_BRIGHTNESS_LEFT_H, VIDEO_OPTION_BRIGHTNESS_TOP_H, VIDEO_OPTION_BRIGHTNESS_RIGHT_H, VIDEO_OPTION_BRIGHTNESS_BOTTOM_H}}
#define ROTATE_BUTTON_BOTH_RECT {{VIDEO_OPTION_ROTATE_LEFT, VIDEO_OPTION_ROTATE_TOP, VIDEO_OPTION_ROTATE_RIGHT, VIDEO_OPTION_ROTATE_BOTTOM}, \
                                                                {VIDEO_OPTION_ROTATE_LEFT_H, VIDEO_OPTION_ROTATE_TOP_H, VIDEO_OPTION_ROTATE_RIGHT_H, VIDEO_OPTION_ROTATE_BOTTOM_H}}
#define MINUS_BUTTON_BOTH_RECT {{ADJUST_MINUS_BUTTON_LEFT, ADJUST_MINUS_BUTTON_TOP, ADJUST_MINUS_BUTTON_RIGHT, ADJUST_MINUS_BUTTON_BOTTOM}, \
                                                                {ADJUST_MINUS_BUTTON_LEFT_H, ADJUST_MINUS_BUTTON_TOP_H, ADJUST_MINUS_BUTTON_RIGHT_H, ADJUST_MINUS_BUTTON_BOTTOM_H}}
#define ADD_BUTTON_BOTH_RECT {{ADJUST_ADD_BUTTON_LEFT, ADJUST_ADD_BUTTON_TOP, ADJUST_ADD_BUTTON_RIGHT, ADJUST_ADD_BUTTON_BOTTOM}, \
                                                                {ADJUST_ADD_BUTTON_LEFT_H, ADJUST_ADD_BUTTON_TOP_H, ADJUST_ADD_BUTTON_RIGHT_H, ADJUST_ADD_BUTTON_BOTTOM_H}}
#define ROTATE_90_BUTTON_BOTH_RECT {{VIDEO_OPTION_ROTATE_90_LEFT, VIDEO_OPTION_ROTATE_90_TOP, VIDEO_OPTION_ROTATE_90_RIGHT, VIDEO_OPTION_ROTATE_90_BOTTOM}, \
                                                                        {VIDEO_OPTION_ROTATE_90_LEFT_H, VIDEO_OPTION_ROTATE_90_TOP_H, VIDEO_OPTION_ROTATE_90_RIGHT_H, VIDEO_OPTION_ROTATE_90_BOTTOM_H}}
#define ROTATE_180_BUTTON_BOTH_RECT {{VIDEO_OPTION_ROTATE_180_LEFT, VIDEO_OPTION_ROTATE_180_TOP, VIDEO_OPTION_ROTATE_180_RIGHT, VIDEO_OPTION_ROTATE_180_BOTTOM}, \
                                                                        {VIDEO_OPTION_ROTATE_180_LEFT_H, VIDEO_OPTION_ROTATE_180_TOP_H, VIDEO_OPTION_ROTATE_180_RIGHT_H, VIDEO_OPTION_ROTATE_180_BOTTOM_H}}
#define SUB_OPTION_BG_BOTH_RECT {{VIDEO_SUBOPTION_BG_LEFT, VIDEO_SUBOPTION_BG_TOP, VIDEO_SUBOPTION_BG_RIGHT, VIDEO_SUBOPTION_BG_BOTTOM}, \
                                                                {VIDEO_SUBOPTION_BG_LEFT_H, VIDEO_SUBOPTION_BG_TOP_H, VIDEO_SUBOPTION_BG_RIGHT_H, VIDEO_SUBOPTION_BG_BOTTOM_H}}
#define PROMPT_BOTH_RECT {{VIDEO_OPTION_ADJUST_TYPE_LEFT, VIDEO_OPTION_ADJUST_TYPE_TOP, VIDEO_OPTION_ADJUST_TYPE_RIGHT, VIDEO_OPTION_ADJUST_TYPE_BOTTOM}, \
                                                {VIDEO_OPTION_ADJUST_TYPE_LEFT_H, VIDEO_OPTION_ADJUST_TYPE_TOP_H, VIDEO_OPTION_ADJUST_TYPE_RIGHT_H, VIDEO_OPTION_ADJUST_TYPE_BOTTOM_H}}
#define VIDEO_PARAM_BOTH_RECT {{ADJUST_LEVEL_LEFT, ADJUST_LEVEL_TOP, ADJUST_LEVEL_RIGHT, ADJUST_LEVEL_BOTTOM}, \
                                                        {ADJUST_LEVEL_LEFT_H, ADJUST_LEVEL_TOP_H, ADJUST_LEVEL_RIGHT_H, ADJUST_LEVEL_BOTTOM_H}}
#define VOLUME_PARAM_BOTH_RECT {{VIDEO_OPTION_ADJUST_TYPE_LEFT, VIDEO_OPTION_ADJUST_TYPE_TOP, VIDEO_OPTION_ADJUST_TYPE_RIGHT, VIDEO_OPTION_ADJUST_TYPE_BOTTOM}, \
                                                                {VIDEO_OPTION_ADJUST_TYPE_LEFT_H, VIDEO_OPTION_ADJUST_TYPE_TOP_H, VIDEO_OPTION_ADJUST_TYPE_RIGHT_H, VIDEO_OPTION_ADJUST_TYPE_BOTTOM_H}}
#define USER_INPUT_EDIT_BOTH_RECT {{ USERINPUT_EDIT_LEFT, USERINPUT_EDIT_TOP, USERINPUT_EDIT_RIGHT, USERINPUT_EDIT_BOTTOM }, \
                                                                    { USERINPUT_EDIT_LEFT_H, USERINPUT_EDIT_TOP_H, USERINPUT_EDIT_RIGHT_H, USERINPUT_EDIT_BOTTOM_H}}

/**--------------------------------------------------------------------------*
**                         TYPE AND CONSTANT                                *
**--------------------------------------------------------------------------*/
LOCAL uint8 s_vt_time_count_timer = 0; //通话计时计数器

LOCAL uint8 s_vt_volume_close_timer = 0; //音量调节画面关闭定时器（3秒钟不操作，关闭调节音量画面）

LOCAL uint8 s_vt_video_param_timer = 0; //视频参数设置界面关闭定时器

LOCAL uint8 s_vt_open_camera_timer = 0; //收到对方打开摄像头，关闭远端图画替代计时器

LOCAL BOOLEAN s_vt_resume_mp3= FALSE;

typedef enum
{
    CONNECTED_MENU_NODE_ROOT,		//root
    CONNECTED_MENU_NODE_STOP_RECORD, //停止录制
    CONNECTED_MENU_NODE_STOP_REPLACE, //停止替代
    CONNECTED_MENU_NODE_CAMERA, //关闭/打开摄像头
    CONNECTED_MENU_NODE_REMOTE_MUTE, //远端静音
    CONNECTED_MENU_NODE_MUTE, //本端静音
    CONNECTED_MENU_NODE_BT, //打开/关闭蓝牙
    CONNECTED_MENU_NODE_REPLACE, //替代画面
    CONNECTED_MENU_NODE_RECORD, //录制
    CONNECTED_MENU_NODE_SMS, //短消息
    CONNECTED_MENU_NODE_PB, //电话簿
    CONNECTED_MENU_NODE_BROWSER, //浏览器
    CONNECTED_MENU_NODE_SETTING, //设置

    CONNECTED_MENU_NODE_REPLACE_IMAGE, //替代画面：图片
    CONNECTED_MENU_NODE_REPLACE_VIDEO, //替代画面：视频

    CONNECTED_MENU_NODE_RECORD_RV_DA, //录制远端图像和混音
    CONNECTED_MENU_NODE_RECORD_DA, //录制混音
    CONNECTED_MENU_NODE_RECORD_RV //录制远端视频

}VT_CONNECTED_MENU_NODE_E; //通话窗口菜单节点


/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/

//************action handle function*****************//
/*****************************************************************************/
//  Description : HandleUIDial
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:UI处理对方拨号的操作
/*****************************************************************************/
LOCAL MMIVT_RESULT_E HandleUIDial(DPARAM param);

/*****************************************************************************/
//  Description : HandleUIRemoteConnect
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:UI处理对方拨号的操作
/*****************************************************************************/
LOCAL MMIVT_RESULT_E HandleUIRemoteConnect(void);

/*****************************************************************************/
//  Description : HandleUIRemoteIncoming
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:UI处理对方来电的操作
/*****************************************************************************/
LOCAL MMIVT_RESULT_E HandleUIRemoteIncoming(void);

/*****************************************************************************/
//  Description : HandleUIConnect
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:UI处理接听的操作
/*****************************************************************************/
LOCAL MMIVT_RESULT_E HandleUIConnect(void);

/*****************************************************************************/
//  Description : HandleUIConnectEnd
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:UI处理接听结束的操作
/*****************************************************************************/
LOCAL MMIVT_RESULT_E HandleUIConnectEnd(void);

/*****************************************************************************/
//  Description : HandleUIHangup
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:UI处理挂断的操作
/*****************************************************************************/
LOCAL MMIVT_RESULT_E HandleUIHangup(void);

/*****************************************************************************/
//  Description : HandleUIDisconnect
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:UI处理收到挂断消息的操作
/*****************************************************************************/
LOCAL MMIVT_RESULT_E HandleUIDisconnect(void);

/*****************************************************************************/
//  Description : HandleUIDiscEnd
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:UI处理挂断完成的操作
/*****************************************************************************/
LOCAL MMIVT_RESULT_E HandleUIDiscEnd(void);

/*****************************************************************************/
//  Description : HandleUIRemoteFBReq
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:UI处理收到对方回落请求的操作
/*****************************************************************************/
LOCAL MMIVT_RESULT_E HandleUIRemoteFBReq(void);

/*****************************************************************************/
//  Description : HandleUIFBCallQuery
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:UI处理接受对方回落请求的操作
/*****************************************************************************/
//LOCAL MMIVT_RESULT_E HandleUIFBAccept(DPARAM param);

/*****************************************************************************/
//  Description : HandleUIFBReject
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:UI处理拒绝对方回落请求的操作
/*****************************************************************************/
//LOCAL MMIVT_RESULT_E HandleUIFBReject(void);

/*****************************************************************************/
//  Description : HandleUIFBReq
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:UI处理发起回落请求的操作
/*****************************************************************************/
LOCAL MMIVT_RESULT_E HandleUIFBReq(void);

/*****************************************************************************/
//  Description : HandleUIQuit
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:UI处理退出VT applet的操作
/*****************************************************************************/
LOCAL MMIVT_RESULT_E HandleUIQuit(void);

/*****************************************************************************/
//  Description : HandleUIRemoteAlert
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:UI处理对方开始振铃的操作
/*****************************************************************************/
LOCAL MMIVT_RESULT_E HandleUIRemoteAlert(void);

/*****************************************************************************/
//  Description : HandleUIEnterMMR
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:UI处理进入多媒体彩铃
/*****************************************************************************/
LOCAL MMIVT_RESULT_E HandleUIEnterMMR(void);

/*****************************************************************************/
//  Description : HandleUIQuitMMR
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:UI处理退出多媒体彩铃
/*****************************************************************************/
LOCAL MMIVT_RESULT_E HandleUIQuitMMR(void);

/*****************************************************************************/
//  Description : HandleUIRemoteNoSignalStart
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:UI处理对方数据中断
/*****************************************************************************/
LOCAL MMIVT_RESULT_E HandleUIRemoteNoSignalStart(void);

/*****************************************************************************/
//  Description : HandleUIRemoteNoSignalStop
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:UI处理对方数据恢复
/*****************************************************************************/
LOCAL MMIVT_RESULT_E HandleUIRemoteNoSignalStop(void);

/*****************************************************************************/
//  Description : HandleUIRemoteCameraOff
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:UI处理对方关闭摄像头
/*****************************************************************************/
LOCAL MMIVT_RESULT_E HandleUIRemoteCameraOff(void);

/*****************************************************************************/
//  Description : HandleUIRemoteCameraOn
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:UI处理对方开启摄像头
/*****************************************************************************/
LOCAL MMIVT_RESULT_E HandleUIRemoteCameraOn(void);

/*****************************************************************************/
//  Description : HandleUIUpdateAudioDevice
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:UI处理更新声音设备
/*****************************************************************************/
LOCAL MMIVT_RESULT_E HandleUIUpdateAudioDevice(void);

/*****************************************************************************/
//  Description : QuitVTApp
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:UI退出VT应用
/*****************************************************************************/
LOCAL MMIVT_RESULT_E QuitVTApp(void);

/*****************************************************************************/
//  Description : HandleUIRedial
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:UI处理自动拨号的操作
/*****************************************************************************/
LOCAL MMIVT_RESULT_E HandleUIRedial(DPARAM param);

/*****************************************************************************/
//  Description : HandleUIRefErrorInd
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:UI处理ref 异常消息
/*****************************************************************************/
LOCAL MMIVT_RESULT_E HandleUIRefErrorInd(DPARAM param);

/*****************************************************************************/
//  Description : TranslateState
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:vt的状态切换函数
/*****************************************************************************/
LOCAL void TranslateUIState(MMIVT_UI_STATE_E cur_state, MMIVT_UI_MSG_E action);

/*****************************************************************************/
//  Description : 获得当前UI的状态
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:获得当前UI的状态
/*****************************************************************************/
LOCAL MMIVT_UI_STATE_E GetUICurState(void);

/*****************************************************************************/
//  Description : SendMsgToKernel
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:发送消息到kernel
/*****************************************************************************/
LOCAL MMIVT_RESULT_E SendMsgToKernel(MMIVT_KERNEL_MSG_E action, DPARAM param);

//************win msg handle function*****************//
/*****************************************************************************/
// 	Description : open mo calling win
//	Global resource dependence : 
//  Author:samboo.shen
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E OpenMoCallingWin(void);

/*****************************************************************************/
// 	Description : open mt calling win
//	Global resource dependence : 
//  Author:samboo.shen
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E OpenMtIncomingWin(void);

/*****************************************************************************/
// 	Description : OpenConnectingWin
//	Global resource dependence : 
//  Author:samboo.shen
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E OpenConnectingWin(void);

/*****************************************************************************/
// 	Description : OpenConnectedWin
//	Global resource dependence : 
//  Author:samboo.shen
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E OpenConnectedWin(void);

/*****************************************************************************/
// 	Description : OpenConectedMenuWin
//	Global resource dependence : 
//  Author:samboo.shen
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E OpenConectedMenuWin(MMI_HANDLE_T parent_win_handle);

/*****************************************************************************/
// 	Description : OpenDisconnectingWin
//	Global resource dependence : 
//  Author:samboo.shen
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E OpenDisconnectingWin(void);

/*****************************************************************************/
// 	Description : OpenDisconnectedShowTimeWin
//	Global resource dependence : 
//  Author:samboo.shen
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E OpenDisconnectedShowTimeWin(void);

/*****************************************************************************/
// 	Description : OpenDisconnectedCausePromptWin
//	Global resource dependence : 
//  Author:samboo.shen
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E OpenDisconnectedCausePromptWin(void);

/*****************************************************************************/
// 	Description : OpenFallbackQueryWin
//	Global resource dependence : 
//  Author:samboo.shen
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E OpenFallbackQueryWin(void);

/*****************************************************************************/
// 	Description : OpenFallbackPromptWin
//	Global resource dependence : 
//  Author:samboo.shen
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E OpenFallbackPromptWin(void);

/*****************************************************************************/
// 	Description : OpenFallbackQuitWin
//	Global resource dependence : 
//  Author:samboo.shen
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E OpenFallbackQuitWin(void);

/*****************************************************************************/
// 	Description : OpenFallbackWaitWin
//	Global resource dependence : 
//  Author:samboo.shen
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E OpenFallbackWaitWin(void);

/*****************************************************************************/
// 	Description : OpenRedialingWin
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E OpenRedialingWin(void);

/*****************************************************************************/
// 	Description : HANDLE2GQueryWinMsg
//	Global resource dependence : 
//  Author:samboo.shen
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HANDLE2GQueryWinMsg( 
                                            MMI_WIN_ID_T win_id, 
                                            MMI_MESSAGE_ID_E msg_id, 
                                            DPARAM param);

/*****************************************************************************/
// 	Description : HandleMoCallingWinMsg
//	Global resource dependence : 
//  Author:samboo.shen
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMoCallingWinMsg( 
                                            MMI_WIN_ID_T win_id, 
                                            MMI_MESSAGE_ID_E msg_id, 
                                            DPARAM param);

/*****************************************************************************/
// 	Description : HandleMtIncomingWinMsg
//	Global resource dependence : 
//  Author:samboo.shen
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMtIncomingWinMsg( 
                                             MMI_WIN_ID_T win_id, 
                                             MMI_MESSAGE_ID_E msg_id, 
                                             DPARAM param);

/*****************************************************************************/
// 	Description : HandleConnectingWinMsg
//	Global resource dependence : 
//  Author:samboo.shen
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleConnectingWinMsg( 
                                             MMI_WIN_ID_T win_id, 
                                             MMI_MESSAGE_ID_E msg_id, 
                                             DPARAM param);

/*****************************************************************************/
// 	Description : HandleConnectedWinMsg
//	Global resource dependence : 
//  Author:samboo.shen
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleConnectedWinMsg( 
                                            MMI_WIN_ID_T win_id, 
                                            MMI_MESSAGE_ID_E msg_id, 
                                            DPARAM param);

/*****************************************************************************/
// 	Description : HandleConnectedMenuWinMsg
//	Global resource dependence : 
//  Author:samboo.shen
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleConnectedMenuWinMsg( 
                                            MMI_WIN_ID_T win_id, 
                                            MMI_MESSAGE_ID_E msg_id, 
                                            DPARAM param);

/*****************************************************************************/
// 	Description : InitConectedDynamicMenu
//	Global resource dependence : 
//  Author:samboo.shen
//	Note:
/*****************************************************************************/
LOCAL void InitConectedDynamicMenu(MMI_HANDLE_T win_handle );

/*****************************************************************************/
//  Description : insert dynamenu node by label
//  Global resource dependence : 
//  Author:samboo.shen
//	Note:
/*****************************************************************************/
LOCAL void InsertNodeByLabel(
                             MMI_CTRL_ID_T      ctrl_id,        //控件id
                             MMI_TEXT_ID_T      text_id,       //插入的节点的文本
                             uint16             node_id,        //节点编号
                             uint16             parent_node_id, //父节点编号
                             uint16             index           //位置
                             );

/*****************************************************************************/
// 	Description : HandleDisconnectingWinMsg
//	Global resource dependence : 
//  Author:samboo.shen
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDisconnectingWinMsg( 
                                                MMI_WIN_ID_T win_id, 
                                                MMI_MESSAGE_ID_E msg_id, 
                                                DPARAM param);

/*****************************************************************************/
// 	Description : HandleFallbackQueryWin
//	Global resource dependence : 
//  Author:samboo.shen
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleFallbackQueryWin( 
                                                MMI_WIN_ID_T win_id, 
                                                MMI_MESSAGE_ID_E msg_id, 
                                                DPARAM param);

/*****************************************************************************/
// 	Description : HandleFallbackPromptWin
//	Global resource dependence : 
//  Author:samboo.shen
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleFallbackPromptWin( 
                                                MMI_WIN_ID_T win_id, 
                                                MMI_MESSAGE_ID_E msg_id, 
                                                DPARAM param);

/*****************************************************************************/
// 	Description : HandleFallbackQuitWin
//	Global resource dependence : 
//  Author:samboo.shen
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleFallbackQuitWin( 
                                                MMI_WIN_ID_T win_id, 
                                                MMI_MESSAGE_ID_E msg_id, 
                                                DPARAM param);

/*****************************************************************************/
// 	Description : HandleFallbackWaitWin
//	Global resource dependence : 
//  Author:samboo.shen
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleFallbackWaitWin( 
                                                MMI_WIN_ID_T win_id, 
                                                MMI_MESSAGE_ID_E msg_id, 
                                                DPARAM param);

/*****************************************************************************/
// 	Description : HandleDisconnectedShowWinMsg
//	Global resource dependence : 
//  Author:samboo.shen
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDisconnectedShowWinMsg( 
                                                MMI_WIN_ID_T win_id, 
                                                MMI_MESSAGE_ID_E msg_id, 
                                                DPARAM param);

/*****************************************************************************/
// 	Description : SetDisconnectedShowDetail
//	Global resource dependence : 
//  Author:samboo.shen
//	Note:
/*****************************************************************************/
LOCAL void SetDisconnectedShowDetail(MMI_HANDLE_T win_id, MMI_CTRL_ID_T ctrl_id);

/*****************************************************************************/
// 	Description : HandleDisconnectedCausePromptWin
//	Global resource dependence : 
//  Author:samboo.shen
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDisconnectedCausePromptWin( 
                                                MMI_WIN_ID_T win_id, 
                                                MMI_MESSAGE_ID_E msg_id, 
                                                DPARAM param);

/*****************************************************************************/
// 	Description : handle timeout msg in vt connected win
//	Global resource dependence : 
//  Author:samboo.shen
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleVTConnectedWinTimeOut(MMI_WIN_ID_T win_id,DPARAM param);

/*****************************************************************************/
// 	Description : HandleVTConnectedWinUpAndDownKey
//	Global resource dependence : 
//  Author:samboo.shen
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleVTConnectedWinUpAndDownKey(MMI_WIN_ID_T win_id,MMI_MESSAGE_ID_E msg_id);

#if 1//ndef TOUCH_PANEL_SUPPORT
/*****************************************************************************/
// 	Description : HandleVTConnectedWinLeftAndRightKey
//	Global resource dependence : 
//  Author:samboo.shen
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleVTConnectedWinLeftAndRightKey(MMI_WIN_ID_T win_id,MMI_MESSAGE_ID_E msg_id);
#endif

/*****************************************************************************/
// 	Description : HandleVTConnectedWinPhoneNumKey
//	Global resource dependence : 
//  Author:samboo.shen
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleVTConnectedWinPhoneNumKey(MMI_WIN_ID_T win_id,MMI_MESSAGE_ID_E msg_id);

/*****************************************************************************/
// 	Description : HandleVTConnectedWinWebKey
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleVTConnectedWinWebKey(MMI_WIN_ID_T win_id);

/*****************************************************************************/
// 	Description : HandleVTConnectedWinCancelKey
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleVTConnectedWinCancelKey(MMI_WIN_ID_T win_id);

/*****************************************************************************/
// 	Description : HandleVTConnectedWinOkKey
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleVTConnectedWinOkKey(MMI_WIN_ID_T win_id);

/*****************************************************************************/
// 	Description : HandleVTConnectedWinPenOk
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleVTConnectedWinPenOk(                              
                                              MMI_WIN_ID_T      win_id,         //in:
                                              MMI_CTRL_ID_T     button_ctrl_id  //in:button ctrl
                                            );
/*****************************************************************************/
// 	Description : HandleRedialWinMsg
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleRedialWinMsg( 
                                            MMI_WIN_ID_T win_id, 
                                            MMI_MESSAGE_ID_E msg_id, 
                                            DPARAM param);

//************display function*****************//
/*****************************************************************************/
// 	Description : display anim pic
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
LOCAL void DisplayImageIcon(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id, MMI_IMAGE_ID_T image_id, GUI_RECT_T* rect_ptr);


/*****************************************************************************/
// 	Description : display anim pic
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
LOCAL void DisplayLabelImage(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id, MMI_IMAGE_ID_T image_id);

/*****************************************************************************/
// 	Description : DisplayButtonImage
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
LOCAL void DisplayButtonImage(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id, GUI_BG_T* button_bg_ptr);

/*****************************************************************************/
// 	Description : SetCtrlVisible
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
LOCAL void SetCtrlVisible(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id, BOOLEAN is_visible);

/*****************************************************************************/
// 	Description : display anim pic
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
LOCAL void DisplayAnim(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id, MMI_ANIM_ID_T image_id, GUI_RECT_T* rect_ptr, GUI_BG_T* bg_ptr);

/*****************************************************************************/
// 	Description : SetMainPlayArea
//	Global resource dependence : 
//  Author:samboo.shen
//	Note:
/*****************************************************************************/
LOCAL void SetMainPlayArea(MMI_WIN_ID_T win_id, BOOLEAN is_transparent);

/*****************************************************************************/
// 	Description : SetSecondaryPlayArea
//	Global resource dependence : 
//  Author:samboo.shen
//	Note:
/*****************************************************************************/
LOCAL void SetSecondaryPlayArea(MMI_WIN_ID_T win_id, BOOLEAN is_transparent);

/*****************************************************************************/
// 	Description : DisplayCallNameLabel
//	Global resource dependence : 
//  Author:samboo.shen
//	Note:
/*****************************************************************************/
LOCAL void DisplayCallNameLabel(MMI_WIN_ID_T win_id);

/*****************************************************************************/
// 	Description : DisplayCallTimeLabel
//	Global resource dependence : 
//  Author:samboo.shen
//	Note:
/*****************************************************************************/
LOCAL void DisplayCallTimeLabel(MMI_WIN_ID_T win_id, BOOLEAN is_fresh);

/*****************************************************************************/
// 	Description : DisplayCallStateLabel
//	Global resource dependence : 
//  Author:samboo.shen
//	Note:
/*****************************************************************************/
LOCAL void DisplayCallStateLabel(MMI_WIN_ID_T win_id, BOOLEAN is_fresh);

/*****************************************************************************/
// 	Description : SetHeadAndContentInRText
//	Global resource dependence : none
//  Author: samboo.shen
//	Note:
/*****************************************************************************/
LOCAL void SetHeadAndContentInRText(MMI_TEXT_ID_T head_text_id,      //标题文字id
                               MMI_CTRL_ID_T ctrl_id,            //所在的控件容器
                               MMI_STRING_T content_str,         //显示内容
                               uint16   *index
                               );

/*****************************************************************************/
// 	Description : FreshRect
//	Global resource dependence : 
//  Author:samboo.shen
//	Note:
/*****************************************************************************/
LOCAL void FreshRect(MMI_WIN_ID_T win_id);

/*****************************************************************************/
// 	Description : HandleVTWinOpenWinMsg
//	Global resource dependence : 
//  Author:samboo.shen
//	Note:
/*****************************************************************************/
LOCAL void HandleVTWinOpenWinMsg(MMI_WIN_ID_T win_id);


/*****************************************************************************/
// 	Description : StartWinTimer
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: timer_id [IN] & [OUT]
/*****************************************************************************/
LOCAL void StartWinTimer(MMI_WIN_ID_T win_id, uint8* timer_id, uint32 time_out);

/*****************************************************************************/
// 	Description : StopWinTimer
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: timer_id [IN] & [OUT]
/*****************************************************************************/
LOCAL void StopWinTimer(uint8* timer_id);

/*****************************************************************************/
// 	Description : GetBigHeadPhotoByCalledNum
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: call_number_ptr [in]
//        photo_name_ptr [out]
//        photo_len [out] 
/*****************************************************************************/
LOCAL BOOLEAN GetBigHeadPhotoByCalledNum(MN_CALLED_NUMBER_T *call_number_ptr, 
                                        wchar* photo_name_ptr, 
                                        uint32* photo_len_ptr);

#if 1//ndef TOUCH_PANEL_SUPPORT
/*****************************************************************************/
// 	Description : SwitchVideoRotateFocus
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
LOCAL void SwitchVideoRotateFocus(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id);

/*****************************************************************************/
// 	Description : SwitchVideoOptionFocus
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
LOCAL void SwitchVideoOptionFocus(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id);

/*****************************************************************************/
// 	Description : SwitchMainOptionFocus
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
LOCAL void SwitchMainOptionFocus(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id);
#endif

/*****************************************************************************/
// 	Description : UpdateLcdBright
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
LOCAL void UpdateLcdBright(MMI_WIN_ID_T win_id, BOOLEAN is_lcd_brightness_add);

/*****************************************************************************/
// 	Description : UpdateZoom
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
LOCAL void UpdateZoom(MMI_WIN_ID_T win_id, BOOLEAN is_zoom_out);

/*****************************************************************************/
// 	Description : UpdateZoom
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
LOCAL void UpdateContrast(MMI_WIN_ID_T win_id, BOOLEAN is_contrast_add);

/*****************************************************************************/
// 	Description : UpdateZoom
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
LOCAL void UpdateBrightness(MMI_WIN_ID_T win_id, BOOLEAN is_brightness_add);

/*****************************************************************************/
// 	Description : UpdateZoom
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
LOCAL void UpdateRotateOption(MMI_WIN_ID_T win_id, MMIVT_IMAGE_ROTATE_MODE_E mode);

/*****************************************************************************/
// 	Description : UpdateVolume
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
LOCAL void UpdateVolume(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id);

/*****************************************************************************/
// 	Description : UpdateAudioOption
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E UpdateAudioOption(MMI_WIN_ID_T win_id);

/*****************************************************************************/
// 	Description : UpdateCameraOption
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
LOCAL MMIVT_REF_RET_E UpdateCameraOption(MMI_WIN_ID_T win_id);

/*****************************************************************************/
// 	Description : UpdateDisplayModeOption
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
LOCAL void UpdateDisplayModeOption(void);

/*****************************************************************************/
// 	Description : EnterMainOptionMode
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
LOCAL void EnterMainOptionMode(MMI_WIN_ID_T win_id);

/*****************************************************************************/
// 	Description : QuitMainOptionMode 
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
LOCAL void QuitMainOptionMode(MMI_WIN_ID_T win_id);

/*****************************************************************************/
// 	Description : DisplayMainOption
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
LOCAL void DisplayMainOption(MMI_WIN_ID_T win_id);

/*****************************************************************************/
// 	Description : HideMainOption 
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
LOCAL void HideMainOption(MMI_WIN_ID_T win_id);

/*****************************************************************************/
// 	Description : EnterVideoOptionMode
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
LOCAL void EnterVideoOptionMode(MMI_WIN_ID_T win_id);

/*****************************************************************************/
// 	Description : QuitVideoOptionMode
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
LOCAL void QuitVideoOptionMode(MMI_WIN_ID_T win_id);

/*****************************************************************************/
// 	Description : DisplayVideoOption
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
LOCAL void DisplayVideoOption(MMI_WIN_ID_T win_id);

/*****************************************************************************/
// 	Description : HideVideoOption 
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
LOCAL void HideVideoOption(MMI_WIN_ID_T win_id);

/*****************************************************************************/
// 	Description : EnterVideoParamMode 
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
LOCAL void EnterVideoParamMode(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T button_ctrl_id);

/*****************************************************************************/
// 	Description : QuitVideoParamMode
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
LOCAL void QuitVideoParamMode(MMI_WIN_ID_T win_id);

/*****************************************************************************/
// 	Description : DisplayVideoParam
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
LOCAL void DisplayVideoParam(MMI_WIN_ID_T win_id, MMIVT_2LEVEL_VIDEO_OPTION_E video_option);

/*****************************************************************************/
// 	Description : HideVideoParam 
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
LOCAL void HideVideoParam(MMI_WIN_ID_T win_id);

/*****************************************************************************/
// 	Description : EnterUserInputMode
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
LOCAL void EnterUserInputMode(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id);

/*****************************************************************************/
// 	Description : QuitUserInputMode
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
LOCAL void QuitUserInputMode(MMI_WIN_ID_T win_id);

/*****************************************************************************/
// 	Description : EnterVolumeMode
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
LOCAL void EnterVolumeMode(MMI_WIN_ID_T win_id);

/*****************************************************************************/
// 	Description : QuitVolumeMode
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
LOCAL void QuitVolumeMode(MMI_WIN_ID_T win_id);

/*****************************************************************************/
// 	Description : DisplayVolume
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
LOCAL void DisplayVolume(MMI_WIN_ID_T win_id);

/*****************************************************************************/
// 	Description : HideVolume
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
LOCAL void HideVolume(MMI_WIN_ID_T win_id);

/*****************************************************************************/
// 	Description : SetConnectedWinSoftKey
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
LOCAL void SetConnectedWinSoftKey(MMI_WIN_ID_T win_id, BOOLEAN is_fresh);

//#ifdef TOUCH_PANEL_SUPPORT
///*****************************************************************************/
//// 	Description : SetConnectedWinButtonVar
////	Global resource dependence : 
////  Author:samboo.shen
////	Note: 
///*****************************************************************************/
//LOCAL void SetConnectedWinButtonVar(MMI_WIN_ID_T win_id);
//#endif

/*****************************************************************************/
// 	Description : CloseVTWin
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
LOCAL void CloseVTAppletWinByID(MMI_WIN_ID_T win_id);

/*****************************************************************************/
// 	Description : GetTextPromptByCause
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
LOCAL MMI_TEXT_ID_T GetTextPromptByCause(MN_CALL_DISCONNECT_CALL_CAUSE_E cause);

/*****************************************************************************/
// 	Description : GetFallBackPromptByCause
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
LOCAL MMI_TEXT_ID_T GetFallBackPromptByCause(MN_CALL_DISCONNECT_CALL_CAUSE_E cause);

/*****************************************************************************/
// 	Description : HandleEnableCamera
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
LOCAL MMIVT_REF_RET_E HandleEnableCamera(BOOLEAN is_enable);

/*****************************************************************************/
// 	Description : StartLocalReplaceImage
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
LOCAL MMIVT_REF_RET_E StartLocalReplaceImage(BOOLEAN is_default);

/*****************************************************************************/
// 	Description : StopLocalReplaceImage
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
LOCAL void StopLocalReplaceImage(void);

/*****************************************************************************/
// 	Description : StartRemoteReplaceImage
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
LOCAL MMIVT_REF_RET_E StartRemoteReplaceImage(BOOLEAN is_default);

 /*****************************************************************************/
// 	Description : StopRemoteReplaceImage
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
LOCAL void StopRemoteReplaceImage(void);

/*****************************************************************************/
// 	Description : StartRecord
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
LOCAL MMIVT_REF_RET_E StartRecord(MMIVT_RECORD_TYPE_E record_type);

/*****************************************************************************/
// 	Description : StopRecord
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E StopRecord(void);

/*****************************************************************************/
//  Description : ErrorNotifyCallBack
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:底层错误处理函数
/*****************************************************************************/
LOCAL void ErrorNotify(MMIVT_REF_RET_E err_type);

/*****************************************************************************/
//  Description : IsAllowVtQuit
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsAllowVtQuit(void);

/*****************************************************************************/
//  Description : StartVTBlock
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:
/*****************************************************************************/
LOCAL void StartVTBlock(void);

/*****************************************************************************/
//  Description : StopVTBlock
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:
/*****************************************************************************/
LOCAL void StopVTBlock(void);

/*****************************************************************************/
//  Description : DisableGsensorRotate
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:
/*****************************************************************************/
LOCAL void DisableGsensorRotate(MMI_HANDLE_T win_handle, BOOLEAN is_disable_opera);

/**--------------------------------------------------------------------------*
**                         CONSTANT VARIABLES                               *
**--------------------------------------------------------------------------*/

//去电窗口
WINDOW_TABLE(MMIVT_MO_CALLING_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleMoCallingWinMsg), 
    WIN_ID( MMIVT_MO_CALLING_WIN_ID ),
    //WIN_TITLE(TXT_NULL),
#ifdef MMI_PDA_SUPPORT
    WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
#endif
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_CANCEL),
    CREATE_ANIM_CTRL(MMIVT_ANIM_OPTION_BG_BANK_CTRL_ID, MMIVT_MO_CALLING_WIN_ID),
    CREATE_ANIM_CTRL(MMIVT_ANIM_MO_CALLING_CTRL_ID, MMIVT_MO_CALLING_WIN_ID),

    CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, MMIVT_LABEL_PHOTO_CTRL_ID),
             
    //main option
    CREATE_BUTTON_CTRL(IMAG_VT_HANDHOLD_OFF, MMIVT_BUTTON_AUDIO_OPTION_CTRL_ID),
    CREATE_BUTTON_CTRL(IMAGE_VT_DISPLAY_OFF, MMIVT_BUTTON_VIDEO_OPTION_CTRL_ID),
    CREATE_BUTTON_CTRL(IMAG_VT_CAMERA_OFF,  MMIVT_BUTTON_CAMERA_OPTION_CTRL_ID),
    CREATE_BUTTON_CTRL(IMAG_VT_DISPMODE_OFF, MMIVT_BUTTON_DISPMODE_OPTION_CTRL_ID),

    CREATE_LABEL_CTRL(GUILABEL_ALIGN_LEFT, MMIVT_LABEL_CALLSTATE_CTRL_ID),
    CREATE_LABEL_CTRL(GUILABEL_ALIGN_LEFT, MMIVT_LABEL_CALLNAME_CTRL_ID),

    END_WIN
};

//来电窗口
WINDOW_TABLE(MMIVT_MT_INCOMING_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleMtIncomingWinMsg), 
    WIN_ID( MMIVT_MT_INCOMING_WIN_ID ),

#ifdef MMI_PDA_SUPPORT
    WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
#endif
    WIN_SOFTKEY(STXT_ANSWER, TXT_VT_ANSWER_AUDIO, STXT_REJECT),
    CREATE_ANIM_CTRL(MMIVT_ANIM_OPTION_BG_BANK_CTRL_ID, MMIVT_MT_INCOMING_WIN_ID),        
    CREATE_ANIM_CTRL(MMIVT_ANIM_INCOMING_CALL_CTRL_ID, MMIVT_MT_INCOMING_WIN_ID),

    CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, MMIVT_LABEL_CALLSTATE_CTRL_ID),
    CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, MMIVT_LABEL_CALLNAME_CTRL_ID),
    END_WIN
};

//正在连接窗口
WINDOW_TABLE(MMIVT_CONNECTING_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleConnectingWinMsg), 
    WIN_ID( MMIVT_CONNECTING_WIN_ID ),
    WIN_BACKGROUND_ID(IMAGE_COMMON_BG),
    //WIN_TITLE(TXT_CONNECTING),
#ifdef MMI_PDA_SUPPORT
    WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
#endif
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, TXT_NULL),

    CREATE_ANIM_CTRL(MMIVT_ANIM_OPTION_BG_BANK_CTRL_ID, MMIVT_CONNECTING_WIN_ID), 
    CREATE_ANIM_CTRL(MMIVT_ANIM_CALL_CONNECTING_CTRL_ID, MMIVT_CONNECTING_WIN_ID),

    CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, MMIVT_LABEL_CALLSTATE_CTRL_ID),
    CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, MMIVT_LABEL_CALLNAME_CTRL_ID),
    END_WIN
};

//通话窗口
WINDOW_TABLE(MMIVT_CONNECTED_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleConnectedWinMsg), 
        WIN_ID( MMIVT_CONNECTED_WIN_ID ),
        //WIN_TITLE(TXT_CC_COMMUN),
#ifdef MMI_PDA_SUPPORT
    WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
#endif
        WIN_SOFTKEY(STXT_OPTION, TXT_NULL, TXT_VT_HIDE_ME),

        CREATE_ANIM_CTRL(MMIVT_ANIM_OPTION_BG_BANK_CTRL_ID, MMIVT_CONNECTED_WIN_ID),
        CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, MMIVT_LABEL_PHOTO_CTRL_ID),
        CREATE_LABEL_CTRL(GUILABEL_ALIGN_LEFT, MMIVT_LABEL_CALLNAME_CTRL_ID),
        CREATE_LABEL_CTRL(GUILABEL_ALIGN_LEFT, MMIVT_LABEL_CALLTIME_CTRL_ID),

        //main option
        CREATE_BUTTON_CTRL(IMAG_VT_HANDHOLD_OFF, MMIVT_BUTTON_AUDIO_OPTION_CTRL_ID),
        CREATE_BUTTON_CTRL(IMAGE_VT_DISPLAY_OFF, MMIVT_BUTTON_VIDEO_OPTION_CTRL_ID),
        CREATE_BUTTON_CTRL(IMAG_VT_CAMERA_OFF, MMIVT_BUTTON_CAMERA_OPTION_CTRL_ID),
        CREATE_BUTTON_CTRL(IMAG_VT_DISPMODE_OFF, MMIVT_BUTTON_DISPMODE_OPTION_CTRL_ID),


        //video option
        CREATE_LABEL_CTRL(GUILABEL_ALIGN_LEFT, MMIVT_LABEL_SUBOPTION_BG_CTRL_ID),
        CREATE_BUTTON_CTRL(IMAG_VT_LCD_BRIGHTNESS_OFF, MMIVT_BUTTON_DISPLAY_LCD_BRIGHTNESS_CTRL_ID),
        CREATE_BUTTON_CTRL(IMAGE_VT_VIDEO_OPTION_CONTRAST_OFF, MMIVT_BUTTON_DISPLAY_CONTRAST_CTRL_ID),
        CREATE_BUTTON_CTRL(IMAGE_VT_VIDEO_OPTION_BRIGHTNESS_OFF, MMIVT_BUTTON_DISPLAY_BRIGHTNESS_CTRL_ID),
        CREATE_BUTTON_CTRL(IMAGE_VT_VIDEO_OPTION_ROTATE_OFF, MMIVT_BUTTON_DISPLAY_ROTATE_CTRL_ID),
     
        //video parameter controller
        CREATE_LABEL_CTRL(GUILABEL_ALIGN_LEFT, MMIVT_LABEL_PROMOPT_CTRL_ID),
        CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, MMIVT_LABEL_VIDEO_PARAM_CTRL_ID),
        CREATE_BUTTON_CTRL(IMAG_VT_ADJUST_LEFT_OFF, MMIVT_BUTTON_VIDEO_PARAM_MINUS_CTRL_ID),
        CREATE_BUTTON_CTRL(IMAG_VT_ADJUST_RIGHT_OFF, MMIVT_BUTTON_VIDEO_PARAM_ADD_CTRL_ID),
                
        //video rotate
        CREATE_BUTTON_CTRL(IMAGE_VT_VIDEO_OPTION_ROTATE_90_OFF, MMIVT_BUTTON_ROTATE_90_CTRL_ID),
        CREATE_BUTTON_CTRL(IMAGE_VT_VIDEO_OPTION_ROTATE_180_OFF, MMIVT_BUTTON_ROTATE_180_CTRL_ID),

        //audio volume
        CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, MMIVT_LABEL_VOLUME_PARAM_CTRL_ID),
        CREATE_BUTTON_CTRL(IMAG_VT_ADJUST_DOWN_OFF, MMIVT_BUTTON_VOLUME_MINUS_CTRL_ID),
        CREATE_BUTTON_CTRL(IMAG_VT_ADJUST_UP_OFF, MMIVT_BUTTON_VOLUME_ADD_CTRL_ID),

        //user input for touch panel
#ifdef TOUCH_PANEL_SUPPORT
        CREATE_BUTTON_CTRL(IMAGE_VT_BUTTON_DEL_OFF, MMIVT_BUTTON_DIAL_DEL_CTRL_ID),
        CREATE_BUTTON_CTRL(IMAGE_VT_BUTTON_ONE_OFF, MMIVT_BUTTON_DIAL_1_CTRL_ID),
        CREATE_BUTTON_CTRL(IMAGE_VT_BUTTON_TWO_OFF, MMIVT_BUTTON_DIAL_2_CTRL_ID),
        CREATE_BUTTON_CTRL(IMAGE_VT_BUTTON_THREE_OFF, MMIVT_BUTTON_DIAL_3_CTRL_ID),
        CREATE_BUTTON_CTRL(IMAGE_VT_BUTTON_FOUR_OFF, MMIVT_BUTTON_DIAL_4_CTRL_ID),
        CREATE_BUTTON_CTRL(IMAGE_VT_BUTTON_FIVE_OFF, MMIVT_BUTTON_DIAL_5_CTRL_ID),
        CREATE_BUTTON_CTRL(IMAGE_VT_BUTTON_SIX_OFF, MMIVT_BUTTON_DIAL_6_CTRL_ID),
        CREATE_BUTTON_CTRL(IMAGE_VT_BUTTON_SEVEN_OFF, MMIVT_BUTTON_DIAL_7_CTRL_ID),
        CREATE_BUTTON_CTRL(IMAGE_VT_BUTTON_EIGHT_OFF, MMIVT_BUTTON_DIAL_8_CTRL_ID),
        CREATE_BUTTON_CTRL(IMAGE_VT_BUTTON_NINE_OFF, MMIVT_BUTTON_DIAL_9_CTRL_ID),
        CREATE_BUTTON_CTRL(IMAGE_VT_BUTTON_ZERO_OFF, MMIVT_BUTTON_DIAL_0_CTRL_ID),
        CREATE_BUTTON_CTRL(IMAGE_VT_BUTTON_STAR_OFF, MMIVT_BUTTON_DIAL_STAR_CTRL_ID),
        CREATE_BUTTON_CTRL(IMAGE_VT_BUTTON_HASH_OFF, MMIVT_BUTTON_DIAL_HASH_CTRL_ID),
#endif

        END_WIN
};

//通话选项窗口
WINDOW_TABLE( MMIVT_CONNECTED_MENU_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleConnectedMenuWinMsg),
    WIN_ID(MMIVT_CONNECTED_MENU_WIN_ID),
    WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),
    END_WIN
};

//正在挂断窗口
WINDOW_TABLE(MMIVT_DISCONNECTING_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleDisconnectingWinMsg), 
    WIN_ID( MMIVT_DISCONNECTING_WIN_ID ),
    WIN_BACKGROUND_ID(IMAGE_COMMON_BG),
    //WIN_TITLE(TXT_CC_HANGING_UP),
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, TXT_NULL),
    CREATE_ANIM_CTRL(MMIVT_ANIM_OPTION_BG_BANK_CTRL_ID, MMIVT_DISCONNECTING_WIN_ID), 
    CREATE_ANIM_CTRL(MMIVT_ANIM_CALL_DISCONNECTING_CTRL_ID, MMIVT_DISCONNECTING_WIN_ID),

    CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, MMIVT_LABEL_CALLSTATE_CTRL_ID),
    CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, MMIVT_LABEL_CALLNAME_CTRL_ID),

    END_WIN
};

//通话挂断时间信息显示窗口
WINDOW_TABLE(MMIVT_DISCONNECTED_SHOWTIME_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleDisconnectedShowWinMsg ), 
        WIN_ID( MMIVT_DISCONNECTED_SHOWTIME_WIN_ID ),
        WIN_BACKGROUND_ID(IMAGE_COMMON_BG),
        //WIN_TITLE(TXT_CC_HANGING_UP),
        WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_RETURN),
        CREATE_RICHTEXT_CTRL(MMIVT_RICHTEXT_SHOWTIME_CTRL_ID),

        END_WIN
};

//自动重拨窗口

WINDOW_TABLE(MMIVT_REDIAL_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleRedialWinMsg), 
    WIN_ID( MMIVT_REDIAL_WIN_ID ),
    //WIN_TITLE(TXT_NULL),
#ifdef MMI_PDA_SUPPORT
    WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
#endif
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_CANCEL),
    CREATE_ANIM_CTRL(MMIVT_ANIM_OPTION_BG_BANK_CTRL_ID, MMIVT_REDIAL_WIN_ID),
    CREATE_ANIM_CTRL(MMIVT_ANIM_MO_CALLING_CTRL_ID, MMIVT_REDIAL_WIN_ID),

    CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, MMIVT_LABEL_PHOTO_CTRL_ID),    
    CREATE_BUTTON_CTRL(IMAG_VT_HANDHOLD_OFF, MMIVT_BUTTON_AUDIO_OPTION_CTRL_ID),
    CREATE_BUTTON_CTRL(IMAGE_VT_DISPLAY_OFF, MMIVT_BUTTON_VIDEO_OPTION_CTRL_ID),
    CREATE_BUTTON_CTRL(IMAG_VT_CAMERA_OFF, MMIVT_BUTTON_CAMERA_OPTION_CTRL_ID),
    CREATE_BUTTON_CTRL(IMAG_VT_DISPMODE_OFF, MMIVT_BUTTON_DISPMODE_OPTION_CTRL_ID),

    CREATE_LABEL_CTRL(GUILABEL_ALIGN_LEFT, MMIVT_LABEL_CALLSTATE_CTRL_ID),
    CREATE_LABEL_CTRL(GUILABEL_ALIGN_LEFT, MMIVT_LABEL_CALLNAME_CTRL_ID),

    END_WIN
};

/*****************************************************************************/
//  Description : MMIVT_HandleUIMsg
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:处理来自ps和ui的操作，进行相应的状态跃迁。
/*****************************************************************************/
PUBLIC MMIVT_RESULT_E MMIVT_HandleUIMsg(
                                 MMIVT_UI_MSG_E msg,	//需要处理的action
                                 DPARAM param	//action相关信息
                                 )
{
    MMIVT_RESULT_E result = MMIVT_RESULT_SUCCESS;

    //SCI_TRACE_LOW:"mmivt HandleUIAction state:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_UI_1267_112_2_18_3_6_13_95,(uint8*)"d", GetUICurState());
    switch(msg) 
    {
    case MMIVT_UI_MSG_DIAL:
        result = HandleUIDial(param);
        break;
        
    case MMIVT_UI_MSG_REMOTE_CONNECT:
        result = HandleUIRemoteConnect();
        break;
        
    case MMIVT_UI_MSG_REMOTE_INCOMING:
        result = HandleUIRemoteIncoming();
        break;
        
    case MMIVT_UI_MSG_CONNECT:
        result = HandleUIConnect();
        break;
        
    case MMIVT_UI_MSG_CONNECT_END:
        result = HandleUIConnectEnd();
        break;
        
    case MMIVT_UI_MSG_HANGUP:
        result = HandleUIHangup();
        break;
        
    case MMIVT_UI_MSG_DISCONNECT:
        result = HandleUIDisconnect();
        break;
        
    case MMIVT_UI_MSG_DISC_END:
        result = HandleUIDiscEnd();
        break;

    case MMIVT_UI_MSG_REMOTE_FB_REQ:
        result = HandleUIRemoteFBReq();
        break;
        
//    case MMIVT_UI_MSG_FB_ACCEPT:
//        result = HandleUIFBAccept(parm);
//        break;
//
//    case MMIVT_UI_MSG_FB_REJECT:
//        result = HandleUIFBReject(parm);
//        break;
        
    case MMIVT_UI_MSG_FB_REQ:
        result = HandleUIFBReq();
        break;


    case MMIVT_UI_MSG_QUIT:
        result = HandleUIQuit();
        break;

    case MMIVT_UI_MSG_REMOTE_ALERTING:
        result = HandleUIRemoteAlert();
        break;

    case MMIVT_UI_MSG_ENTER_MMR:
        result = HandleUIEnterMMR();
        break;

    case MMIVT_UI_MSG_QUIT_MMR:
        result = HandleUIQuitMMR();
        break;
        
    case MMIVT_UI_MSG_REDIAL:
        result = HandleUIRedial(param);
        break;

    case MMIVT_UI_MSG_REF_ERROR_IND:
        result = HandleUIRefErrorInd(param);
        break;
        
    case MMIVT_UI_MSG_REMOTE_CAMERA_OFF:
        result = HandleUIRemoteCameraOff();
        break;

    case MMIVT_UI_MSG_REMOTE_CAMERA_ON:
        result = HandleUIRemoteCameraOn();
        break;
        
    case MMIVT_UI_MSG_UPDATE_AUDIO_DEVICE:
        result = HandleUIUpdateAudioDevice();
        break;

    default:
        //donothing
        result = MMIVT_RESULT_INVALID_ACTION;
        break;
    }
    return result;
}

/*****************************************************************************/
//  Description : QuitVTApp
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:UI退出VT应用
/*****************************************************************************/
LOCAL MMIVT_RESULT_E QuitVTApp(void)
{
    return MMIVT_HandleUIMsg(MMIVT_UI_MSG_QUIT, PNULL);
}

/*****************************************************************************/
//  Description : HandleUIDial
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:UI处理拨号的操作
/*****************************************************************************/
LOCAL MMIVT_RESULT_E HandleUIDial(DPARAM param)
{

    MMIVT_RESULT_E result = MMIVT_RESULT_SUCCESS;
    MMIVT_UI_INFO_DIAL_T* dial_info_ptr = (MMIVT_UI_INFO_DIAL_T*)param;
    MMIVT_APPLET_T *vt_applet_ptr = MMIVT_GetAppletInstance();    
    //SCI_TRACE_LOW:"mmivt HandleUIDial"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_UI_1382_112_2_18_3_6_14_96,(uint8*)"");

    if (!MMIAPIPHONE_IsSimAvailable(dial_info_ptr->dual_sys))
    {
        MMIAPIPHONE_AlertSimNorOKStatus();
        return MMIVT_RESULT_FAILED;
    }

    if (MMIAPIMTV_IsMTVRunning())
    {
        MMIPUB_OpenAlertWarningWin(TXT_EXIT_MOBILETV);
        return MMIVT_RESULT_FAILED;
    }

/* stop audioplayer when vt is existed*/
	if(MMIAPIMP3_IsPlayerPlaying())	 // MMIAPIMP3_IsPlayerPlaying   MMIAPIAP_IsOpened
	{
		//MMIAPIAP_DestroyByBG();
		MMIAPIVT_SetAudioPlayResumeStatus(TRUE);
		MMIAPIMP3_PauseMp3(FALSE);
	}

    //如果已经存在applet，则判定为异常
    if (PNULL != vt_applet_ptr)
    {
        //SCI_TRACE_LOW:"mmivt err already have applet"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_UI_1399_112_2_18_3_6_14_97,(uint8*)"");
        MMIVT_StopVTApplet();
        //return MMIVT_RESULT_WRONG_APPLET;
    }
    
    MMIVT_StartVTApplet();

    MMIVT_SetDualSys(dial_info_ptr->dual_sys);
    //如果不是3G网络
    if ((MMI_GMMREG_RAT_3G != MMIAPIPHONE_GetTDOrGsm(MMIVT_GetDualSys()) 
        && MMI_GMMREG_RAT_GPRS_AND_3G != MMIAPIPHONE_GetTDOrGsm(MMIVT_GetDualSys()))
    //如果正处于语音通话中
        ||(MMIAPICC_IsInState(CC_CALL_CONNECTED_STATE) && !MMIAPICC_IsVideoCall())
        )
    {
        return MMIVT_HandleUIMsg(MMIVT_UI_MSG_REMOTE_FB_REQ, PNULL);
    }
    
    //状态迁移
    TranslateUIState(MMIVT_UI_STATE_MO_CALLING, MMIVT_UI_MSG_DIAL);

    //向kernel发送MMIVT_KERNEL_MSG_USER_DIAL
    result = SendMsgToKernel(MMIVT_KERNEL_MSG_USER_DIAL,param);

    //打开呼出画面
    if (MMIVT_RESULT_SUCCESS == result)
    {
        OpenMoCallingWin();
    }

    return result;
    
}

/*****************************************************************************/
//  Description : HandleUIRemoteConnect
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:UI处理对方接听的操作
/*****************************************************************************/
LOCAL MMIVT_RESULT_E HandleUIRemoteConnect(void)
{
    
    MMIVT_APPLET_T *vt_applet_ptr = MMIVT_GetAppletInstance();
    
    //SCI_TRACE_LOW:"mmivt HandleUIRemoteConnect"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_UI_1441_112_2_18_3_6_14_98,(uint8*)"");
    if (PNULL == vt_applet_ptr)
    {
        //SCI_TRACE_LOW:"mmivt null applet"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_UI_1444_112_2_18_3_6_14_99,(uint8*)"");
        return MMIVT_RESULT_NULL_APPLET;
    }
    
    if (MMIVT_UI_STATE_MO_CALLING !=  GetUICurState())
    {
        //SCI_TRACE_LOW:"mmivt err state:%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_UI_1450_112_2_18_3_6_14_100,(uint8*)"d", GetUICurState());
        return MMIVT_RESULT_ERROR_STATE;
    }
    
    //状态迁移
    TranslateUIState(MMIVT_UI_STATE_CONNECTED, MMIVT_UI_MSG_REMOTE_CONNECT);
    
    //打开通话界面，关闭呼出界面
    OpenConnectedWin();
    CloseVTAppletWinByID(MMIVT_MO_CALLING_WIN_ID);
    return MMIVT_RESULT_SUCCESS;
}

/*****************************************************************************/
//  Description : HandleUIRemoteIncoming
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:UI处理对方来电的操作
/*****************************************************************************/
LOCAL MMIVT_RESULT_E HandleUIRemoteIncoming(void)
{
    MMIVT_APPLET_T *vt_applet_ptr = MMIVT_GetAppletInstance();
    //SCI_TRACE_LOW:"mmivt HandleUIRemoteIncoming"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_UI_1470_112_2_18_3_6_14_101,(uint8*)"");

/* pause audioplayer when vt is existed*/
	if(MMIAPIMP3_IsPlayerPlaying())	 // MMIAPIMP3_IsPlayerPlaying   MMIAPIAP_IsOpened
	{
		MMIAPIVT_SetAudioPlayResumeStatus(TRUE);
		MMIAPIMP3_PauseMp3(FALSE);
	}
	
    if (PNULL != vt_applet_ptr)
    {
        //SCI_TRACE_LOW:"mmivt err already have applet"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_UI_1473_112_2_18_3_6_14_102,(uint8*)"");
        MMIVT_StopVTApplet();
        //return MMIVT_RESULT_WRONG_APPLET;
    }
    
    MMIVT_StartVTApplet();
    
    //状态迁移
    TranslateUIState(MMIVT_UI_STATE_MT_INCOMING, MMIVT_UI_MSG_REMOTE_INCOMING);
    
    //打开来电窗口
    OpenMtIncomingWin();
    return MMIVT_RESULT_SUCCESS;
    
}

/*****************************************************************************/
//  Description : HandleUIConnect
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:UI处理开始连接的操作
/*****************************************************************************/
LOCAL MMIVT_RESULT_E HandleUIConnect(void)
{
    MMIVT_APPLET_T *vt_applet_ptr = MMIVT_GetAppletInstance();
    MMIVT_RESULT_E result = MMIVT_RESULT_SUCCESS;
    //SCI_TRACE_LOW:"mmivt HandleUIConnect"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_UI_1497_112_2_18_3_6_14_103,(uint8*)"");
    if (PNULL == vt_applet_ptr)
    {
        //SCI_TRACE_LOW:"mmivt null applet"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_UI_1500_112_2_18_3_6_14_104,(uint8*)"");
        return MMIVT_RESULT_NULL_APPLET;
    }
    
    if (MMIVT_UI_STATE_MT_INCOMING !=  GetUICurState())
    {
        //SCI_TRACE_LOW:"mmivt err state:%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_UI_1506_112_2_18_3_6_14_105,(uint8*)"d", GetUICurState());
        return MMIVT_RESULT_ERROR_STATE;
    }
    //状态迁移
    TranslateUIState(MMIVT_UI_STATE_CONNECTING, MMIVT_UI_MSG_CONNECT);
    
    //向kernel发送MMIVT_KERNEL_MSG_CONNECT_CALL
    result = SendMsgToKernel(MMIVT_KERNEL_MSG_CONNECT_CALL, PNULL);

    //打开正在连接画面，关闭来电画面
    OpenConnectingWin();
    CloseVTAppletWinByID(MMIVT_MT_INCOMING_WIN_ID);
    

    return result;
}

/*****************************************************************************/
//  Description : HandleUIConnectEnd
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:UI处理已经接通的操作
/*****************************************************************************/
LOCAL MMIVT_RESULT_E HandleUIConnectEnd(void)
{
    MMIVT_APPLET_T *vt_applet_ptr = MMIVT_GetAppletInstance();
    //SCI_TRACE_LOW:"mmivt HandleUIConnectEnd"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_UI_1530_112_2_18_3_6_14_106,(uint8*)"");
    if (PNULL == vt_applet_ptr)
    {
        //SCI_TRACE_LOW:"mmivt null applet"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_UI_1533_112_2_18_3_6_14_107,(uint8*)"");
        return MMIVT_RESULT_NULL_APPLET;
    }
    
    if (MMIVT_UI_STATE_CONNECTING !=  GetUICurState())
    {
        //SCI_TRACE_LOW:"mmivt err state:%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_UI_1539_112_2_18_3_6_14_108,(uint8*)"d", GetUICurState());
        return MMIVT_RESULT_ERROR_STATE;
    }
    //状态迁移
    TranslateUIState(MMIVT_UI_STATE_CONNECTED, MMIVT_UI_MSG_CONNECT_END);
    
    //打开通话窗口，关闭正在连接窗口
    OpenConnectedWin();
    CloseVTAppletWinByID(MMIVT_CONNECTING_WIN_ID);
    
    return MMIVT_RESULT_SUCCESS;
}

/*****************************************************************************/
//  Description : HandleUIHangup
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:UI处理挂断的操作
/*****************************************************************************/
LOCAL MMIVT_RESULT_E HandleUIHangup(void)
{
    MMIVT_APPLET_T *vt_applet_ptr = MMIVT_GetAppletInstance();
    MMIVT_UI_STATE_E state = GetUICurState();
    MMIVT_RESULT_E result = MMIVT_RESULT_SUCCESS;
    
    
    if (PNULL == vt_applet_ptr)
    {
        //SCI_TRACE_LOW:"mmivt null applet"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_UI_1565_112_2_18_3_6_14_109,(uint8*)"");
        return MMIVT_RESULT_NULL_APPLET;
    }
    //SCI_TRACE_LOW:"mmivt HandleUIHangup"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_UI_1568_112_2_18_3_6_14_110,(uint8*)"");

    switch(state) 
    {
    case MMIVT_UI_STATE_MO_CALLING:
    case MMIVT_UI_STATE_MT_INCOMING:
    case MMIVT_UI_STATE_CONNECTING:
    case MMIVT_UI_STATE_UNCONN_DISCONNECTING:
        TranslateUIState(MMIVT_UI_STATE_UNCONN_DISCONNECTING, MMIVT_UI_MSG_HANGUP);
        break;
        
    case MMIVT_UI_STATE_CONNECTED:
        //when multimedia ring is playing, disconnect win don't show the time
        if (MMVT_KERNEL_SUBSTATE_MMR == MMIVT_GetSubState())
        {
            TranslateUIState(MMIVT_UI_STATE_UNCONN_DISCONNECTING, MMIVT_UI_MSG_HANGUP);
        }
        else
        {
            TranslateUIState(MMIVT_UI_STATE_CONN_DISCONNECTING, MMIVT_UI_MSG_HANGUP);
        }
        break;

    case MMIVT_UI_STATE_CONN_DISCONNECTING:
        TranslateUIState(MMIVT_UI_STATE_CONN_DISCONNECTING, MMIVT_UI_MSG_HANGUP);
        break;
        
    default:
        //SCI_TRACE_LOW:"mmivt HandleUIHangup err state:%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_UI_1596_112_2_18_3_6_14_111,(uint8*)"d", state);
        return MMIVT_RESULT_ERROR_STATE;
        break;/*lint !e527*/
    }

    //向kernel发送MMIVT_KERNEL_MSG_USER_HANGUP
    result = SendMsgToKernel(MMIVT_KERNEL_MSG_USER_HANGUP, PNULL);

    vt_applet_ptr->vt_instance.ui_display_info.is_hangup_by_user = TRUE;

    
    //打开正在断开窗口，关闭其他窗口
    OpenDisconnectingWin();
    CloseVTAppletWinByID(MMIVT_MO_CALLING_WIN_ID);
    CloseVTAppletWinByID(MMIVT_MT_INCOMING_WIN_ID);
    CloseVTAppletWinByID(MMIVT_CONNECTING_WIN_ID);
    CloseVTAppletWinByID(MMIVT_CONNECTED_WIN_ID);
    CloseVTAppletWinByID(MMIVT_CONNECTED_MENU_WIN_ID);
    

    return result;
}

/*****************************************************************************/
//  Description : HandleUIDisconnect
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:UI处理收到挂断消息的操作
/*****************************************************************************/
LOCAL MMIVT_RESULT_E HandleUIDisconnect(void)
{
    MMIVT_APPLET_T *vt_applet_ptr = MMIVT_GetAppletInstance();
    MMIVT_UI_STATE_E state = GetUICurState();
    //SCI_TRACE_LOW:"mmivt HandleUIDisconnect"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_UI_1627_112_2_18_3_6_14_112,(uint8*)"");
    if (PNULL == vt_applet_ptr)
    {
        //SCI_TRACE_LOW:"mmivt null applet"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_UI_1630_112_2_18_3_6_14_113,(uint8*)"");
        return MMIVT_RESULT_NULL_APPLET;
    }
    
    switch(state) 
    {
    case MMIVT_UI_STATE_MO_CALLING:
    case MMIVT_UI_STATE_MT_INCOMING:
    case MMIVT_UI_STATE_CONNECTING:
    case MMIVT_UI_STATE_CONNECTED:
        //when multimedia ring is playing, disconnect win don't show the time
        if (MMVT_KERNEL_SUBSTATE_MMR == MMIVT_GetSubState()
            && MMIVT_UI_STATE_CONNECTED == state)
        {
            TranslateUIState(MMIVT_UI_STATE_UNCONN_DISCONNECTING, MMIVT_UI_MSG_HANGUP);
        }
        else
        {
            TranslateUIState(MMIVT_UI_STATE_CONN_DISCONNECTING, MMIVT_UI_MSG_HANGUP);
        }

        vt_applet_ptr->vt_instance.ui_display_info.is_hangup_by_user = FALSE;
        //打开正在断开窗口，关闭其他窗口
        OpenDisconnectingWin();
        CloseVTAppletWinByID(MMIVT_MO_CALLING_WIN_ID);
        CloseVTAppletWinByID(MMIVT_MT_INCOMING_WIN_ID);
        CloseVTAppletWinByID(MMIVT_CONNECTING_WIN_ID);
        CloseVTAppletWinByID(MMIVT_CONNECTED_WIN_ID);
        CloseVTAppletWinByID(MMIVT_CONNECTED_MENU_WIN_ID);
        break;
        
    default:
        //SCI_TRACE_LOW:"mmivt: HandleUIDisconnect err state:%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_UI_1662_112_2_18_3_6_14_114,(uint8*)"d", state);
        return MMIVT_RESULT_ERROR_STATE;
        break;/*lint !e527*/
    }

    return MMIVT_RESULT_SUCCESS;
}

/*****************************************************************************/
//  Description : HandleUIDiscEnd
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:UI处理挂断结束的操作
/*****************************************************************************/
LOCAL MMIVT_RESULT_E HandleUIDiscEnd(void)
{
    MMIVT_APPLET_T *vt_applet_ptr = MMIVT_GetAppletInstance();
    MMIVT_UI_STATE_E state = GetUICurState();

    //SCI_TRACE_LOW:"mmivt HandleUIDiscEnd"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_UI_1679_112_2_18_3_6_14_115,(uint8*)"");
    if (PNULL == vt_applet_ptr)
    {
        //SCI_TRACE_LOW:"mmivt null applet"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_UI_1682_112_2_18_3_6_14_116,(uint8*)"");
        return MMIVT_RESULT_NULL_APPLET;
    }

    switch(state) 
    {
    case MMIVT_UI_STATE_CONN_DISCONNECTING:
        TranslateUIState(MMIVT_UI_STATE_UNCONN_DISCONNECTED, MMIVT_UI_MSG_HANGUP);
        if(MN_CAUSE_NO_CIRC_CHAN_AV == MMIVT_GetDiscCause())
        {
            //通话过程中挂断cause = 34时提示用户"连接已断开"
            if(TXT_NULL != GetTextPromptByCause(MMIVT_GetDiscCause()))
            {
                OpenDisconnectedCausePromptWin();
            }
        }
        else
        {
            OpenDisconnectedShowTimeWin();
        }
        //SCI_TRACE_LOW:"mmivt close MMIVT_DISCONNECTING_WIN_ID"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_UI_1702_112_2_18_3_6_14_117,(uint8*)"");
        CloseVTAppletWinByID(MMIVT_DISCONNECTING_WIN_ID);
        //SCI_TRACE_LOW:"mmivt close end"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_UI_1704_112_2_18_3_6_14_118,(uint8*)"");
        break;

    case MMIVT_UI_STATE_UNCONN_DISCONNECTING:
    case MMIVT_UI_STATE_MO_CALLING:
    case MMIVT_UI_STATE_MT_INCOMING:
    case MMIVT_UI_STATE_CONNECTING:
        //设置自动重拨最大次数
        if (MMIVT_UI_STATE_MO_CALLING == state && MMIAPISET_GetIsAutoRedail())
        {
            if (MMIAPICC_IsUserBusy(MMIVT_GetDiscCause()))
            {
                vt_applet_ptr->vt_instance.redial_info.redial_times_max = VT_NO_NUM_AUTODIAL_MAX_TIME;
            }
            else if (MMIAPICC_IsValideDest(MMIVT_GetDiscCause()))
            {
                vt_applet_ptr->vt_instance.redial_info.redial_times_max = VT_NON_VALID_DEST_AUTODIAL_MAX_TIME;
            }
        }
        else
        {
            vt_applet_ptr->vt_instance.redial_info.redial_times_max = 0;
        }
        
        //IF(cause值需要给用户提示)
        if(TXT_NULL != GetTextPromptByCause(MMIVT_GetDiscCause()))
        {
            //状态变为UNCONN_DISCONNECTED；
            TranslateUIState(MMIVT_UI_STATE_UNCONN_DISCONNECTED, MMIVT_UI_MSG_DISC_END);
            //打开挂断提示窗口；
            OpenDisconnectedCausePromptWin();
            //关闭正在挂断窗口
            CloseVTAppletWinByID(MMIVT_DISCONNECTING_WIN_ID);
            //关闭来电窗口
            CloseVTAppletWinByID(MMIVT_MT_INCOMING_WIN_ID);
            //关闭呼出窗口
            CloseVTAppletWinByID(MMIVT_MO_CALLING_WIN_ID);
            //关闭重拨窗口
            CloseVTAppletWinByID(MMIVT_REDIAL_WIN_ID);
        }
        else
        {
#if defined(MMI_REJECT_CALL_BY_SMS_SUPPORT)
            //处理挂断回复sms
            if (MMIVT_UI_STATE_UNCONN_DISCONNECTING == state 
                && MMIVT_UI_MSG_HANGUP == vt_applet_ptr->vt_instance.ui_state_info.msg
                && MMIVT_CALL_TYPE_MT_CALL == MMIVT_GetCallType())  //判断是对方来电，并且本方手动挂断
            {
                uint8 tele_num[MMIVT_PHONE_NUM_MAX_LEN + 2] = {0};
                uint8 tele_len = 0;
                MMIVT_GetCallNormalNumber(tele_num, &tele_len);
                
                MMIAPICC_SendSMSAfterRejectMTCall(tele_num, tele_len);
            }
#endif

            //状态变为IDLE；
            TranslateUIState(MMIVT_UI_STATE_IDLE, MMIVT_UI_MSG_DISC_END);
            //关闭VT Applet;
            MMIVT_StopVTApplet();
        }
        
        break;

    default:
        //SCI_TRACE_LOW:"mmivt err state:%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_UI_1769_112_2_18_3_6_15_119,(uint8*)"d", state);
        return MMIVT_RESULT_ERROR_STATE;
        break;/*lint !e527*/
    }

    return MMIVT_RESULT_SUCCESS;
    
}

/*****************************************************************************/
//  Description : HandleUIRemoteFBReq
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:UI处理收到对方回落请求的操作
/*****************************************************************************/
LOCAL MMIVT_RESULT_E HandleUIRemoteFBReq(void)
{
    MMIVT_APPLET_T *vt_applet_ptr = MMIVT_GetAppletInstance();
    MMIVT_UI_STATE_E state = GetUICurState();

    //SCI_TRACE_LOW:"mmivt HandleUIRemoteFBReq"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_UI_1787_112_2_18_3_6_15_120,(uint8*)"");
    if (PNULL == vt_applet_ptr)
    {
        //SCI_TRACE_LOW:"mmivt null applet"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_UI_1790_112_2_18_3_6_15_121,(uint8*)"");
        return MMIVT_RESULT_NULL_APPLET;
    }

    switch(state) 
    {
    case MMIVT_UI_STATE_MO_CALLING:
    case MMIVT_UI_STATE_IDLE: // 如果不在3g服务区，直接处理回落请求
    case MMIVT_UI_STATE_CONN_DISCONNECTING: 
    case MMIVT_UI_STATE_UNCONN_DISCONNECTING:
        {
            MMISET_VT_FALLBACK_HANDLE_MODE_E fallback_handle_mode = MMIAPISET_GetFallbackHandleMode();
            switch(fallback_handle_mode) 
            {
            case MMISET_VT_FALLBACK_QUERY:
                //状态变为FB_CALL_QUERY
                TranslateUIState(MMIVT_UI_STATE_FB_CALL_QUERY, MMIVT_UI_MSG_REMOTE_FB_REQ);
                //打开回落询问窗口
                OpenFallbackQueryWin();
            	break;

            case MMISET_VT_FALLBACK_ACCEPT:
                //状态变为FB_CALL_PROMPT
                TranslateUIState(MMIVT_UI_STATE_FB_CALL_PROMPT, MMIVT_UI_MSG_REMOTE_FB_REQ);
                //打开接受回落请求提示窗口
                OpenFallbackPromptWin();
            	break;

            case MMISET_VT_FALLBACK_QUIT:
                //状态变为FB_CALL_QUIT
                TranslateUIState(MMIVT_UI_STATE_FB_CALL_QUIT, MMIVT_UI_MSG_REMOTE_FB_REQ);
                //打开自动放弃回落提示窗口
                OpenFallbackQuitWin();
            	break;

            default:
                //SCI_TRACE_LOW:"mmivt err fallback mode:%d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_UI_1826_112_2_18_3_6_15_122,(uint8*)"d", fallback_handle_mode);
                return MMIVT_RESULT_ERROR_PARM;
                break;/*lint !e527*/
            }
            //关闭呼出窗口
            CloseVTAppletWinByID(MMIVT_MO_CALLING_WIN_ID);

        }
        break;

    default:
        //SCI_TRACE_LOW:"mmivt err state:%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_UI_1837_112_2_18_3_6_15_123,(uint8*)"d", state);
        return MMIVT_RESULT_ERROR_STATE;
        break;/*lint !e527*/
    }

    return MMIVT_RESULT_SUCCESS;    
}

/*****************************************************************************/
//  Description : HandleUIFBReq
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:UI处理发起回落请求的操作
/*****************************************************************************/
LOCAL MMIVT_RESULT_E HandleUIFBReq(void)
{
    MMIVT_APPLET_T *vt_applet_ptr = MMIVT_GetAppletInstance();
    MMIVT_UI_STATE_E state = GetUICurState();

    //SCI_TRACE_LOW:"mmivt HandleUIFBReq"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_UI_1854_112_2_18_3_6_15_124,(uint8*)"");
    if (PNULL == vt_applet_ptr)
    {
        //SCI_TRACE_LOW:"mmivt null applet"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_UI_1857_112_2_18_3_6_15_125,(uint8*)"");
        return MMIVT_RESULT_NULL_APPLET;
    }

    switch(state) 
    {
    case MMIVT_UI_STATE_MT_INCOMING:
        //状态变为MMIVT_UI_STATE_FB_WAITING
        TranslateUIState(MMIVT_UI_STATE_FB_WAITING, MMIVT_UI_MSG_FB_REQ);
        //打开回落等待窗口
        OpenFallbackWaitWin();
        //关闭来电窗口
        CloseVTAppletWinByID(MMIVT_MT_INCOMING_WIN_ID);
        //通知kernel执行HANGUP ，cause为#88
        {
            BOOLEAN is_fallback = TRUE;
            MMIVT_HandleKernelMsg(MMIVT_KERNEL_MSG_USER_HANGUP, &is_fallback);
        }
        break;

    default:
        //SCI_TRACE_LOW:"mmivt err state:%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_UI_1878_112_2_18_3_6_15_126,(uint8*)"d", state);
        return MMIVT_RESULT_ERROR_STATE;
        break;/*lint !e527*/
    }
    return MMIVT_RESULT_SUCCESS;   
}


/*****************************************************************************/
//  Description : HandleUIQuit
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:UI处理退出VT applet的操作
/*****************************************************************************/
LOCAL MMIVT_RESULT_E HandleUIQuit(void)
{
    MMIVT_APPLET_T *vt_applet_ptr = MMIVT_GetAppletInstance();
    
    //SCI_TRACE_LOW:"mmivt HandleUIQuit"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_UI_1894_112_2_18_3_6_15_127,(uint8*)"");
    if (PNULL == vt_applet_ptr)
    {
        //SCI_TRACE_LOW:"mmivt null applet"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_UI_1897_112_2_18_3_6_15_128,(uint8*)"");
        return MMIVT_RESULT_NULL_APPLET;
    }
    
    MMIVT_StopVTApplet();
    
    return MMIVT_RESULT_SUCCESS;
    
}

/*****************************************************************************/
//  Description : HandleUIRemoteAlert
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:UI处理对方对方开始振铃的操作
/*****************************************************************************/
LOCAL MMIVT_RESULT_E HandleUIRemoteAlert(void)
{

    MMIVT_APPLET_T *vt_applet_ptr = MMIVT_GetAppletInstance();
    
    //SCI_TRACE_LOW:"mmivt HandleUIRemoteAlert"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_UI_1916_112_2_18_3_6_15_129,(uint8*)"");
    if (PNULL == vt_applet_ptr)
    {
        //SCI_TRACE_LOW:"mmivt null applet"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_UI_1919_112_2_18_3_6_15_130,(uint8*)"");
        return MMIVT_RESULT_NULL_APPLET;
    }    

    if (MMIVT_UI_STATE_MO_CALLING != GetUICurState())
    {
        //SCI_TRACE_LOW:"mmivt err state:%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_UI_1925_112_2_18_3_6_15_131,(uint8*)"d", GetUICurState());
        return MMIVT_RESULT_ERROR_STATE;
    }

    {
        MMI_HANDLE_T applet_handle = PNULL;
        MMI_HANDLE_T win_handle = PNULL;

        applet_handle = vt_applet_ptr->caf_applet.app_handle;

        win_handle = MMK_GetWinHandle(applet_handle, MMIVT_MO_CALLING_WIN_ID);
        MMK_SendMsg(win_handle, MSG_VT_REMOTE_ALERTING_IND, PNULL);
        
    }   
    return MMIVT_RESULT_SUCCESS;
}

/*****************************************************************************/
//  Description : HandleUIEnterMMR
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:UI处理进入多媒体彩铃
/*****************************************************************************/
LOCAL MMIVT_RESULT_E HandleUIEnterMMR(void)
{
    MMIVT_APPLET_T *vt_applet_ptr = MMIVT_GetAppletInstance();
    
    //SCI_TRACE_LOW:"mmivt HandleUIEnterMMR"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_UI_1950_112_2_18_3_6_15_132,(uint8*)"");
    if (PNULL == vt_applet_ptr)
    {
        //SCI_TRACE_LOW:"mmivt null applet"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_UI_1953_112_2_18_3_6_15_133,(uint8*)"");
        return MMIVT_RESULT_NULL_APPLET;
    }    

    if (MMIVT_UI_STATE_CONNECTED != GetUICurState())
    {
        //SCI_TRACE_LOW:"mmivt err state:%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_UI_1959_112_2_18_3_6_15_134,(uint8*)"d", GetUICurState());
        return MMIVT_RESULT_ERROR_STATE;
    }

    {
        MMI_HANDLE_T applet_handle = PNULL;
        MMI_HANDLE_T win_handle = PNULL;

        applet_handle = vt_applet_ptr->caf_applet.app_handle;

        win_handle = MMK_GetWinHandle(applet_handle, MMIVT_CONNECTED_WIN_ID);
        DisplayCallTimeLabel(win_handle, TRUE);

        //停止计时器
        StopWinTimer(&s_vt_time_count_timer);

        //重置计时
        MMIVT_GetAppletInstance()->vt_instance.ui_call_time_count = 0;
        
    }   
    return MMIVT_RESULT_SUCCESS;
    
}

/*****************************************************************************/
//  Description : HandleUIQuitMMR
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:UI处理退出多媒体彩铃
/*****************************************************************************/
LOCAL MMIVT_RESULT_E HandleUIQuitMMR(void)
{
    MMIVT_APPLET_T *vt_applet_ptr = MMIVT_GetAppletInstance();
    
    //SCI_TRACE_LOW:"mmivt HandleUIEnterMMR"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_UI_1991_112_2_18_3_6_15_135,(uint8*)"");
    if (PNULL == vt_applet_ptr)
    {
        //SCI_TRACE_LOW:"mmivt null applet"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_UI_1994_112_2_18_3_6_15_136,(uint8*)"");
        return MMIVT_RESULT_NULL_APPLET;
    }    

    if (MMIVT_UI_STATE_CONNECTED != GetUICurState())
    {
        //SCI_TRACE_LOW:"mmivt err state:%d sub state:%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_UI_2000_112_2_18_3_6_15_137,(uint8*)"dd", GetUICurState(), MMIVT_GetSubState());
        return MMIVT_RESULT_ERROR_STATE;
    }

    {
        MMI_HANDLE_T applet_handle = PNULL;
        MMI_HANDLE_T win_handle = PNULL;

        applet_handle = vt_applet_ptr->caf_applet.app_handle;

        win_handle = MMK_GetWinHandle(applet_handle, MMIVT_CONNECTED_WIN_ID);

        //重置计时
        MMIVT_GetAppletInstance()->vt_instance.ui_call_time_count = 0;

        DisplayCallTimeLabel(win_handle, TRUE);

        //重新启动计时器
        StartWinTimer(win_handle, &s_vt_time_count_timer, VT_ONE_SECOND_TIMEOUT);
        
    }   
    
    return MMIVT_RESULT_SUCCESS;
}

/*****************************************************************************/
//  Description : HandleUIRemoteNoSignalStart
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:UI处理对方数据中断
/*****************************************************************************/
LOCAL MMIVT_RESULT_E HandleUIRemoteNoSignalStart(void)
{
    //替换远端图像
    MMIVT_REF_ReplaceStart(MMISET_VT_REPLACE_REMOTE_IMAGE, FALSE);
    return MMIVT_RESULT_SUCCESS;
}

/*****************************************************************************/
//  Description : HandleUIRemoteNoSignalStop
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:UI处理对方数据恢复
/*****************************************************************************/
LOCAL MMIVT_RESULT_E HandleUIRemoteNoSignalStop(void)
{
    //恢复远端图像
    MMIVT_REF_ReplaceStop(MMISET_VT_REPLACE_REMOTE_IMAGE);
    return MMIVT_RESULT_SUCCESS;
}

/*****************************************************************************/
//  Description : HandleUIRedial
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:UI处理自动拨号的操作
/*****************************************************************************/
LOCAL MMIVT_RESULT_E HandleUIRedial(DPARAM param)
{
    MMIVT_APPLET_T *vt_applet_ptr = MMIVT_GetAppletInstance();
    MMIVT_RESULT_E result = MMIVT_RESULT_SUCCESS;
    //SCI_TRACE_LOW:"mmivt HandleUIRedial"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_UI_2055_112_2_18_3_6_15_138,(uint8*)"");
    
    if (PNULL == vt_applet_ptr)
    {
        //SCI_TRACE_LOW:"mmivt null applet"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_UI_2059_112_2_18_3_6_15_139,(uint8*)"");
        return MMIVT_RESULT_NULL_APPLET;
    }
    
    //如果不是3G网络
    if (MMI_GMMREG_RAT_3G != MMIAPIPHONE_GetTDOrGsm(MMIVT_GetDualSys()) 
        && MMI_GMMREG_RAT_GPRS_AND_3G != MMIAPIPHONE_GetTDOrGsm(MMIVT_GetDualSys()))
    {
        uint32 timeout = VT_TIMEOUT_FOREVER;
        MMI_WIN_ID_T win_id = MMIVT_2G_QUERY_WIN_ID;
        MMI_WIN_PRIORITY_E win_priority = WIN_ONE_LEVEL;
        CloseVTAppletWinByID(MMIVT_2G_QUERY_WIN_ID);
        MMIPUB_OpenAlertWinByTextId(
                                    &timeout, 
                                    TXT_3G_NOT_AVAILABLE, 
                                    TXT_VT_QUERY_VOICE_CALL,
                                    IMAGE_PUBWIN_QUERY,
                                    &win_id,
                                    &win_priority,
                                    MMIPUB_SOFTKEY_OKEXIT,
                                    HANDLE2GQueryWinMsg
                                    );
        return MMIVT_RESULT_FAILED;
    }

    
    //状态迁移
    TranslateUIState(MMIVT_UI_STATE_MO_CALLING, MMIVT_UI_MSG_REDIAL);
    //打开重拨画面
    OpenRedialingWin();
    
    //向kernel发送MMIVT_KERNEL_MSG_USER_DIAL
    result = SendMsgToKernel(MMIVT_KERNEL_MSG_USER_DIAL,param);

    return result;
    
}

/*****************************************************************************/
//  Description : HandleUIRefErrorInd
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:UI处理ref 异常消息
/*****************************************************************************/
LOCAL MMIVT_RESULT_E HandleUIRefErrorInd(DPARAM param)
{
    uint32 data = ((APP_MN_POINT_DATA_T*)param)->data;
    
    //SCI_TRACE_LOW:"mmivt HandleUIRefErrorInd %d"
#if 1//OPEN_VT_CHANGE
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_UI_2104_112_2_18_3_6_15_140,(uint8*)"d", (MMIVT_REF_RET_E)data);
#else
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_UI_2104_112_2_18_3_6_15_140,(uint8*)"d", *err_type_ptr);
#endif
    ErrorNotify((MMIVT_REF_RET_E)data);
    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
//  Description : HandleUIRemoteCameraOff
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:UI处理对方关闭摄像头消息
/*****************************************************************************/
LOCAL MMIVT_RESULT_E HandleUIRemoteCameraOff(void)
{
    MMIVT_RESULT_E result = MMIVT_RESULT_SUCCESS;
    MMIVT_REF_RET_E ret_code = MMIVT_REF_RET_SUCCESS;
    MMIVT_APPLET_T *vt_applet_ptr = MMIVT_GetAppletInstance();
    //SCI_TRACE_LOW:"mmivt HandleUIRemoteCameraOff"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_UI_2118_112_2_18_3_6_15_141,(uint8*)"");
    
    if (PNULL == vt_applet_ptr)
    {
        //SCI_TRACE_LOW:"mmivt null applet"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_UI_2122_112_2_18_3_6_15_142,(uint8*)"");
        return MMIVT_RESULT_NULL_APPLET;
    }

    if (MMIVT_KERNEL_STATE_CALL_CONNECTED != MMIVT_GetCallState())
    {
        //SCI_TRACE_LOW:"mmivt err state"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_UI_2128_112_2_18_3_6_15_143,(uint8*)"");
        return MMIVT_RESULT_ERROR_STATE;
    }

    MMIPUB_OpenAlertWarningWin( TXT_COMM_REMOTE_CAMERA_OFF);


    if (MMISET_VT_REPLACE_REMOTE_IMG == MMIAPISET_GetVTRemoteShutdownCameraOption())
    {
        ret_code = StartRemoteReplaceImage(FALSE);
    }

    if(ret_code != MMIVT_REF_RET_SUCCESS)
    {
    	result = MMIVT_RESULT_FAILED;
    }
    
    return result;
}

/*****************************************************************************/
//  Description : HandleUIRemoteCameraOn
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:UI处理对方打开摄像头消息
/*****************************************************************************/
LOCAL MMIVT_RESULT_E HandleUIRemoteCameraOn(void)
{
    MMIVT_APPLET_T *vt_applet_ptr = MMIVT_GetAppletInstance();
    //SCI_TRACE_LOW:"mmivt HandleUIRemoteCameraOn"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_UI_2155_112_2_18_3_6_15_144,(uint8*)"");
    
    if (PNULL == vt_applet_ptr)
    {
        //SCI_TRACE_LOW:"mmivt null applet"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_UI_2159_112_2_18_3_6_15_145,(uint8*)"");
        return MMIVT_RESULT_NULL_APPLET;
    }

    if (MMIVT_KERNEL_STATE_CALL_CONNECTED != MMIVT_GetCallState())
    {
        //SCI_TRACE_LOW:"mmivt err state"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_UI_2165_112_2_18_3_6_15_146,(uint8*)"");
        return MMIVT_RESULT_ERROR_STATE;
    }

    MMIPUB_OpenAlertWarningWin( TXT_VT_REMOTE_CAMERA_ON);

    if (vt_applet_ptr->vt_instance.ui_display_info.is_remote_replacing)
    {
        //cr183021 处理远端图像显示时，可能出现闪烁，增加延时
        MMI_HANDLE_T connected_win_handle = MMK_GetWinHandle(vt_applet_ptr->caf_applet.app_handle, MMIVT_CONNECTED_WIN_ID);
        StartWinTimer(connected_win_handle, &s_vt_open_camera_timer, VT_ONE_SECOND_TIMEOUT);
    }
    return MMIVT_RESULT_SUCCESS;

}

/*****************************************************************************/
//  Description : HandleUIUpdateAudioDevice
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:UI处理更新声音设备
/*****************************************************************************/
LOCAL MMIVT_RESULT_E HandleUIUpdateAudioDevice(void)
{
    MMIVT_APPLET_T *vt_applet_ptr = MMIVT_GetAppletInstance();
    MMI_HANDLE_T connected_win_handle = MMIVT_CONNECTED_WIN_ID;

    //SCI_TRACE_LOW:"mmivt HandleUIUpdateAudioDevice"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_UI_2190_112_2_18_3_6_15_147,(uint8*)"");

    if (PNULL == vt_applet_ptr)
    {
        //SCI_TRACE_LOW:"mmivt null applet"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_UI_2194_112_2_18_3_6_15_148,(uint8*)"");
        return MMIVT_RESULT_NULL_APPLET;
    }

    if (MMICC_AUDIO_DEV_HF == MMICC_GetCurrentDev())
    {
        vt_applet_ptr->vt_instance.ui_display_info.is_handfree_show = FALSE;
    }
    else
    {
        vt_applet_ptr->vt_instance.ui_display_info.is_handfree_show = TRUE;
    }
    if (MMK_IsFocusWin(connected_win_handle)&&(MMIVT_OPTION_BANK_MODE_NORMAL == vt_applet_ptr->vt_instance.ui_display_info.option_bank_mode))
    {
        DisplayMainOption(connected_win_handle);
    }
    return MMIVT_RESULT_SUCCESS;
}

/*****************************************************************************/
//  Description : TranslateState
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:vt的状态切换函数
/*****************************************************************************/
LOCAL void TranslateUIState(MMIVT_UI_STATE_E cur_state, MMIVT_UI_MSG_E msg)
{
    MMIVT_APPLET_T *vt_applet_ptr = MMIVT_GetAppletInstance();

    if (PNULL == vt_applet_ptr)
    {
        //SCI_TRACE_LOW:"mmivt null applet"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_UI_2223_112_2_18_3_6_15_149,(uint8*)"");
        return ;
    }
    vt_applet_ptr->vt_instance.ui_state_info.last_state = vt_applet_ptr->vt_instance.ui_state_info.cur_state;
    vt_applet_ptr->vt_instance.ui_state_info.cur_state = cur_state;
    vt_applet_ptr->vt_instance.ui_state_info.msg = msg;
    
}

/*****************************************************************************/
//  Description : 获得当前UI的状态
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:获得当前UI的状态
/*****************************************************************************/
LOCAL MMIVT_UI_STATE_E GetUICurState(void)
{
    MMIVT_UI_STATE_E state = MMIVT_UI_STATE_MAX;
    MMIVT_APPLET_T *vt_applet_ptr = MMIVT_GetAppletInstance();
    if (PNULL != vt_applet_ptr)
    {
        state = vt_applet_ptr->vt_instance.ui_state_info.cur_state;
    }
    else
    {
        state = MMIVT_UI_STATE_MAX;
    }
    return state;
}

/*****************************************************************************/
//  Description : SendMsgToKernel
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:发送消息到kernel
/*****************************************************************************/
LOCAL MMIVT_RESULT_E SendMsgToKernel(MMIVT_KERNEL_MSG_E msg, DPARAM param)
{
    return MMIVT_HandleKernelMsg(msg, param);
}


/*****************************************************************************/
// 	Description : OpenMoCallingWin
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E OpenMoCallingWin(void)
{
    MMI_WINDOW_TABLE_CREATE_T win_table_create = {0};
    MMIVT_APPLET_T *vt_applet_ptr = MMIVT_GetAppletInstance();
    if (PNULL == vt_applet_ptr)
    {
        //SCI_TRACE_LOW:"mmivt null applet"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_UI_2271_112_2_18_3_6_16_150,(uint8*)"");
        return MMI_RESULT_FALSE;
    }
    
    win_table_create.applet_handle = vt_applet_ptr->caf_applet.app_handle;
    win_table_create.parent_win_handle = 0;
    win_table_create.win_table_ptr = MMIVT_MO_CALLING_WIN_TAB;
    SCI_TRACE_LOW("mmivt OpenMoCallingWin"   );
    MMK_CreateWinTable( &win_table_create );
    
    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
// 	Description : OpenMtIncomingWin
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E OpenMtIncomingWin(void)
{
    MMI_WINDOW_TABLE_CREATE_T win_table_create = {0};
    MMIVT_APPLET_T *vt_applet_ptr = MMIVT_GetAppletInstance();
    if (PNULL == vt_applet_ptr)
    {
        //SCI_TRACE_LOW:"mmivt null applet"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_UI_2294_112_2_18_3_6_16_151,(uint8*)"");
        return MMI_RESULT_FALSE;
    }    
    
    win_table_create.applet_handle = vt_applet_ptr->caf_applet.app_handle;
    win_table_create.parent_win_handle = 0;
    win_table_create.win_table_ptr = MMIVT_MT_INCOMING_WIN_TAB;
    
    MMK_CreateWinTable( &win_table_create );
    
    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
// 	Description : OpenConnectingWin
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E OpenConnectingWin(void)
{
    MMI_WINDOW_TABLE_CREATE_T win_table_create = {0};
    MMIVT_APPLET_T *vt_applet_ptr = MMIVT_GetAppletInstance();
    if (PNULL == vt_applet_ptr)
    {
        //SCI_TRACE_LOW:"mmivt null applet"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_UI_2317_112_2_18_3_6_16_152,(uint8*)"");
        return MMI_RESULT_FALSE;
    }
    
    win_table_create.applet_handle = vt_applet_ptr->caf_applet.app_handle;
    win_table_create.parent_win_handle = 0;
    win_table_create.win_table_ptr = MMIVT_CONNECTING_WIN_TAB;
    
    MMK_CreateWinTable( &win_table_create );
    
    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
// 	Description : OpenConnectedWin
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E OpenConnectedWin(void)
{
    MMI_WINDOW_TABLE_CREATE_T win_table_create = {0};
    MMIVT_APPLET_T *vt_applet_ptr = MMIVT_GetAppletInstance();
    if (PNULL == vt_applet_ptr)
    {
        //SCI_TRACE_LOW:"mmivt null applet"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_UI_2340_112_2_18_3_6_16_153,(uint8*)"");
        return MMI_RESULT_FALSE;
    }    
    
    win_table_create.applet_handle = vt_applet_ptr->caf_applet.app_handle;
    win_table_create.parent_win_handle = 0;
    win_table_create.win_table_ptr = MMIVT_CONNECTED_WIN_TAB;
    
    MMK_CreateWinTable( &win_table_create );
    
    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
// 	Description : OpenConectedMenuWin
//	Global resource dependence : 
//  Author:samboo.shen
//	Note:modify samboo.shen
/*****************************************************************************/
LOCAL MMI_RESULT_E OpenConectedMenuWin(MMI_HANDLE_T parent_win_handle)
{

    MMI_WINDOW_TABLE_CREATE_T win_table_create = {0};
    MMIVT_APPLET_T *vt_applet_ptr = MMIVT_GetAppletInstance();
    if (PNULL == vt_applet_ptr)
    {
        //SCI_TRACE_LOW:"mmivt null applet"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_UI_2364_112_2_18_3_6_16_154,(uint8*)"");
        return MMI_RESULT_FALSE;
    }

    CloseVTAppletWinByID(MMIVT_CONNECTED_MENU_WIN_ID);
    
    win_table_create.applet_handle = vt_applet_ptr->caf_applet.app_handle;
    //win_table_create.parent_win_handle = parent_win_handle;
    win_table_create.win_table_ptr = MMIVT_CONNECTED_MENU_WIN_TAB;
    win_table_create.func = HandleConnectedMenuWinMsg;
    
    MMK_CreateWinTable( &win_table_create );
    
    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
// 	Description : OpenDisconnectingWin
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E OpenDisconnectingWin(void)
{
    MMI_WINDOW_TABLE_CREATE_T win_table_create = {0};
    MMIVT_APPLET_T *vt_applet_ptr = MMIVT_GetAppletInstance();
    if (PNULL == vt_applet_ptr)
    {
        //SCI_TRACE_LOW:"mmivt null applet"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_UI_2390_112_2_18_3_6_16_155,(uint8*)"");
        return MMI_RESULT_FALSE;
    }
    
    win_table_create.applet_handle = vt_applet_ptr->caf_applet.app_handle;
    win_table_create.parent_win_handle = 0;
    win_table_create.win_table_ptr = MMIVT_DISCONNECTING_WIN_TAB;
    
    MMK_CreateWinTable( &win_table_create );
    
    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
// 	Description : OpenDisconnectedShowTimeWin
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E OpenDisconnectedShowTimeWin(void)
{
    MMI_WINDOW_TABLE_CREATE_T win_table_create = {0};
    MMIVT_APPLET_T *vt_applet_ptr = MMIVT_GetAppletInstance();
    if (PNULL == vt_applet_ptr)
    {
        //SCI_TRACE_LOW:"mmivt null applet"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_UI_2413_112_2_18_3_6_16_156,(uint8*)"");
        return MMI_RESULT_FALSE;
    }
    
    win_table_create.applet_handle = vt_applet_ptr->caf_applet.app_handle;
    win_table_create.parent_win_handle = 0;
    win_table_create.win_table_ptr = MMIVT_DISCONNECTED_SHOWTIME_WIN_TAB;
    
    MMK_CreateWinTable( &win_table_create );
    
    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
// 	Description : OpenDisconnectedCausePromptWin
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E OpenDisconnectedCausePromptWin(void)
{
    MMI_STRING_T cause_prompt_str = {0};
    MMI_TEXT_ID_T prompt_id = GetTextPromptByCause(MMIVT_GetDiscCause());

    MMIVT_APPLET_T *vt_applet_ptr = MMIVT_GetAppletInstance();
    if (PNULL == vt_applet_ptr)
    {
        //SCI_TRACE_LOW:"mmivt null applet"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_UI_2438_112_2_18_3_6_16_157,(uint8*)"");
        return MMI_RESULT_FALSE;
    }
    
    MMI_GetLabelTextByLang(prompt_id, &cause_prompt_str);

    MMIPUB_OpenWaitWinEx(vt_applet_ptr->caf_applet.app_handle, 1, &cause_prompt_str, PNULL, PNULL, MMIVT_UNDISCONNECTED_PROMPT_WIN_ID, 
        IMAGE_PUBWIN_WARNING, 0, WIN_ONE_LEVEL, MMIPUB_SOFTKEY_ONE, HandleDisconnectedCausePromptWin,0);

    return MMI_RESULT_TRUE;    
}

/*****************************************************************************/
// 	Description : OpenFallbackQueryWin
//	Global resource dependence : 
//  Author:samboo.shen
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E OpenFallbackQueryWin(void)
{
    MMI_STRING_T fallback_prompt_str = {0};
    MMI_STRING_T query_str = {0};

    MMIVT_APPLET_T *vt_applet_ptr = MMIVT_GetAppletInstance();
    if (PNULL == vt_applet_ptr)
    {
        //SCI_TRACE_LOW:"mmivt null applet"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_UI_2462_112_2_18_3_6_16_158,(uint8*)"");
        return MMI_RESULT_FALSE;
    }

    if (MMI_GMMREG_RAT_3G != MMIAPIPHONE_GetTDOrGsm(MMIVT_GetDualSys()) 
        && MMI_GMMREG_RAT_GPRS_AND_3G != MMIAPIPHONE_GetTDOrGsm(MMIVT_GetDualSys()))
    {
        MMI_GetLabelTextByLang(TXT_3G_NOT_AVAILABLE, &fallback_prompt_str);
    }
    else if( (MMIAPICC_IsInState(CC_CALL_CONNECTED_STATE) && !MMIAPICC_IsVideoCall()))
    {
        MMI_GetLabelTextByLang(TXT_VIDEO_CALL_FB_CAUSE_TEMP_UNSUPPORTED, &fallback_prompt_str);
    }
    else
    {
        MMI_GetLabelTextByLang(GetFallBackPromptByCause(MMIVT_GetDiscCause()), &fallback_prompt_str);
    }
    MMI_GetLabelTextByLang(TXT_VIDEO_CALL_FB_QUERY, &query_str);

    MMIPUB_OpenWaitWinEx(vt_applet_ptr->caf_applet.app_handle, 2, &fallback_prompt_str, &query_str, PNULL, MMIVT_FALLBACK_QUERY_WIN_ID, 
        IMAGE_PUBWIN_QUERY, 0, WIN_ONE_LEVEL, MMIPUB_SOFTKEY_OKCANCEL, HandleFallbackQueryWin,0);

    return MMI_RESULT_TRUE;
    
}

/*****************************************************************************/
// 	Description : OpenFallbackPromptWin
//	Global resource dependence : 
//  Author:samboo.shen
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E OpenFallbackPromptWin(void)
{
    MMI_STRING_T fallback_prompt_str = {0};
    MMI_STRING_T handle_prompt_str = {0};

    MMIVT_APPLET_T *vt_applet_ptr = MMIVT_GetAppletInstance();
    if (PNULL == vt_applet_ptr)
    {
        //SCI_TRACE_LOW:"mmivt null applet"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_UI_2500_112_2_18_3_6_16_159,(uint8*)"");
        return MMI_RESULT_FALSE;
    }

    if (MMI_GMMREG_RAT_3G != MMIAPIPHONE_GetTDOrGsm(MMIVT_GetDualSys()) 
        && MMI_GMMREG_RAT_GPRS_AND_3G != MMIAPIPHONE_GetTDOrGsm(MMIVT_GetDualSys()))
    {
        MMI_GetLabelTextByLang(TXT_3G_NOT_AVAILABLE, &fallback_prompt_str);
    }
    else
    {
        MMI_GetLabelTextByLang(GetFallBackPromptByCause(MMIVT_GetDiscCause()), &fallback_prompt_str);
    }
    MMI_GetLabelTextByLang(TXT_VIDEO_CALL_FB_ACCEPT, &handle_prompt_str);

    MMIPUB_OpenWaitWinEx(vt_applet_ptr->caf_applet.app_handle, 2, &fallback_prompt_str, &handle_prompt_str, PNULL, MMIVT_FALLBACK_PROMPT_WIN_ID, 
        IMAGE_PUBWIN_WARNING, 0, WIN_ONE_LEVEL, MMIPUB_SOFTKEY_ONE, HandleFallbackPromptWin, 0);

    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
// 	Description : OpenFallbackQuitWin
//	Global resource dependence : 
//  Author:samboo.shen
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E OpenFallbackQuitWin(void)
{
    MMI_STRING_T fallback_prompt_str = {0};
    MMI_STRING_T handle_prompt_str = {0};

    MMIVT_APPLET_T *vt_applet_ptr = MMIVT_GetAppletInstance();
    if (PNULL == vt_applet_ptr)
    {
        //SCI_TRACE_LOW:"mmivt null applet"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_UI_2533_112_2_18_3_6_16_160,(uint8*)"");
        return MMI_RESULT_FALSE;
    }

    if (MMI_GMMREG_RAT_3G != MMIAPIPHONE_GetTDOrGsm(MMIVT_GetDualSys()) 
        && MMI_GMMREG_RAT_GPRS_AND_3G != MMIAPIPHONE_GetTDOrGsm(MMIVT_GetDualSys()))
    {
        MMI_GetLabelTextByLang(TXT_3G_NOT_AVAILABLE, &fallback_prompt_str);
    }
    else
    {
        MMI_GetLabelTextByLang(GetFallBackPromptByCause(MMIVT_GetDiscCause()), &fallback_prompt_str);
    }
    MMI_GetLabelTextByLang(TXT_VIDEO_CALL_FB_QUIT, &handle_prompt_str);

    MMIPUB_OpenWaitWinEx(vt_applet_ptr->caf_applet.app_handle, 2, &fallback_prompt_str, &handle_prompt_str, PNULL, MMIVT_FALLBACK_QUIT_WIN_ID, 
        IMAGE_PUBWIN_WARNING, 0, WIN_ONE_LEVEL, MMIPUB_SOFTKEY_ONE, HandleFallbackQuitWin, 0);

    return MMI_RESULT_TRUE;    
}

/*****************************************************************************/
// 	Description : OpenFallbackWaitWin
//	Global resource dependence : 
//  Author:samboo.shen
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E OpenFallbackWaitWin(void)
{
    MMI_STRING_T prompt_str = {0};
    MMI_HANDLE_T app_handle = PNULL;

    MMIVT_APPLET_T *vt_applet_ptr = MMIVT_GetAppletInstance();
    if (PNULL == vt_applet_ptr)
    {
        //SCI_TRACE_LOW:"mmivt null applet"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_UI_2566_112_2_18_3_6_16_161,(uint8*)"");
        return MMI_RESULT_FALSE;
    }

    app_handle = vt_applet_ptr->caf_applet.app_handle;

    MMI_GetLabelTextByLang(TXT_VIDEO_CALL_FB_WAIT, &prompt_str);

    MMIPUB_OpenWaitWinEx(app_handle, 1, &prompt_str, PNULL, PNULL, MMIVT_FALLBACK_WAIT_WIN_ID, 
        IMAGE_PUBWIN_WARNING, 0, WIN_ONE_LEVEL, MMIPUB_SOFTKEY_ONE, HandleFallbackWaitWin, 0);

    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
// 	Description : OpenRedialingWin
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E OpenRedialingWin(void)
{
    MMI_WINDOW_TABLE_CREATE_T win_table_create = {0};
    MMIVT_APPLET_T *vt_applet_ptr = MMIVT_GetAppletInstance();
    if (PNULL == vt_applet_ptr)
    {
        //SCI_TRACE_LOW:"mmivt null applet"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_UI_2590_112_2_18_3_6_16_162,(uint8*)"");
        return MMI_RESULT_FALSE;
    }
    
    win_table_create.applet_handle = vt_applet_ptr->caf_applet.app_handle;
    win_table_create.parent_win_handle = 0;
    win_table_create.win_table_ptr = MMIVT_REDIAL_WIN_TAB;
    
    MMK_CreateWinTable( &win_table_create );
    
    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
// 	Description : HANDLE2GQueryWinMsg
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HANDLE2GQueryWinMsg( 
                                            MMI_WIN_ID_T win_id, 
                                            MMI_MESSAGE_ID_E msg_id, 
                                            DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;

	switch (msg_id)
	{

    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_CTL_OK:

        MMIVT_MakeFBCall();    

        MMK_CloseWin(win_id);
        break;

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    default:
        recode = MMIPUB_HandleAlertWinMsg( win_id, msg_id, param );
        break;
    }
    return recode;
}


/*****************************************************************************/
// 	Description : HandleMoCallingWinMsg
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMoCallingWinMsg( 
                                            MMI_WIN_ID_T win_id, 
                                            MMI_MESSAGE_ID_E msg_id, 
                                            DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    LOCAL uint8 s_mo_local_net_ring_timer = 0;
    SCI_TRACE_LOW("mmivt HandleMoCallingWinMsg msg = %d",   msg_id);

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            GUI_BOTH_RECT_T bg_bank_both_rect = BG_BANK_BOTH_RECT;
            GUI_BOTH_RECT_T main_play_both_rect = MAIN_PLAY_BOTH_RECT;
            GUI_BOTH_RECT_T photo_both_rect = PHOTO_BOTH_RECT;
            GUI_BOTH_RECT_T audio_option_button_both_rect = AUDIO_OPTION_BUTTON_BOTH_RECT;
            GUI_BOTH_RECT_T display_option_button_both_rect = DISPLAY_OPTION_BUTTON_BOTH_RECT;
            GUI_BOTH_RECT_T camera_option_button_both_rect = CAMERA_OPTION_BUTTON_BOTH_RECT;
            GUI_BOTH_RECT_T dispmode_option_button_both_rect = DISPMODE_OPTION_BUTTON_BOTH_RECT;
            GUI_BOTH_RECT_T state_label_both_rect = TWO_LINE_LABEL1_BOTH_RECT;
            GUI_BOTH_RECT_T name_label_both_rect = TWO_LINE_LABEL2_BOTH_RECT;

            GUIAPICTRL_SetBothRect(MMK_GetCtrlHandleByWin(win_id, MMIVT_LABEL_CALLSTATE_CTRL_ID), &state_label_both_rect);
            GUIAPICTRL_SetBothRect(MMK_GetCtrlHandleByWin(win_id, MMIVT_LABEL_CALLNAME_CTRL_ID), &name_label_both_rect);
            GUIAPICTRL_SetBothRect(MMK_GetCtrlHandleByWin(win_id, MMIVT_ANIM_OPTION_BG_BANK_CTRL_ID), &bg_bank_both_rect);
            GUIAPICTRL_SetBothRect(MMK_GetCtrlHandleByWin(win_id, MMIVT_ANIM_MO_CALLING_CTRL_ID), &main_play_both_rect);
            GUIAPICTRL_SetBothRect(MMK_GetCtrlHandleByWin(win_id, MMIVT_LABEL_PHOTO_CTRL_ID), &photo_both_rect);
            GUIAPICTRL_SetBothRect(MMK_GetCtrlHandleByWin(win_id, MMIVT_BUTTON_AUDIO_OPTION_CTRL_ID), &audio_option_button_both_rect);
            GUIAPICTRL_SetBothRect(MMK_GetCtrlHandleByWin(win_id, MMIVT_BUTTON_VIDEO_OPTION_CTRL_ID), &display_option_button_both_rect);
            GUIAPICTRL_SetBothRect(MMK_GetCtrlHandleByWin(win_id, MMIVT_BUTTON_CAMERA_OPTION_CTRL_ID), &camera_option_button_both_rect);
            GUIAPICTRL_SetBothRect(MMK_GetCtrlHandleByWin(win_id, MMIVT_BUTTON_DISPMODE_OPTION_CTRL_ID), &dispmode_option_button_both_rect);
        }
        //CLEAR LCD
        {
            FreshRect(win_id);
        }        
        HandleVTWinOpenWinMsg(win_id);

        {
            GUI_BG_T bg = {0};
            bg.bg_type = GUI_BG_IMG;
            bg.img_id = IMAG_VT_MAIN_PLAY_BG;
            bg.shape = GUI_SHAPE_RECT;
            DisplayAnim(win_id, MMIVT_ANIM_MO_CALLING_CTRL_ID, ANIM_MO_CALL, PNULL, &bg); 
        }

        //耳机插入的时候需要显示为免提
        if (MMICC_AUDIO_DEV_EP == MMICC_GetCurrentDev())
        {
            MMIVT_GetAppletInstance()->vt_instance.ui_display_info.is_handfree_show = TRUE;
        }
        else
        {
            MMIVT_GetAppletInstance()->vt_instance.ui_display_info.is_handfree_show = FALSE;
        }
        DisplayMainOption(win_id);
        
        DisplayCallStateLabel(win_id, FALSE);
        DisplayCallNameLabel(win_id);


        //仅用于09b 10a支持applet获得焦点时，收到resume消息
        MMIDEFAULT_SetBackLight(TRUE);		//点亮背光
	    MMIDEFAULT_AllowTurnOffBackLight(FALSE);	//背光常开   

        //给视频显示区域刷透明色
        SetSecondaryPlayArea(win_id, TRUE);   

        //设置mmi block
        StartVTBlock();
        
        //disable gsensor rotate
        DisableGsensorRotate(win_id, TRUE);
        break;

    case MSG_LOSE_FOCUS:
        SetSecondaryPlayArea(win_id, FALSE);
        StopVTBlock();
        break;

    //仅用于09b 10a支持applet获得焦点时，收到resume消息
    case MSG_GET_FOCUS:
        MMIDEFAULT_SetBackLight(TRUE);		//点亮背光
        MMIDEFAULT_AllowTurnOffBackLight(FALSE);	//背光常开   
        FreshRect(win_id);//fresh lcd for cleaning residual image of last open win

        SetSecondaryPlayArea(win_id, TRUE);
        StartVTBlock();   
        break;

    case MSG_APP_CANCEL:
    case MSG_APP_RED:
    case MSG_KEYDOWN_HEADSET_BUTTON:
        //挂断电话
        MMIVT_HandleUIMsg(MMIVT_UI_MSG_HANGUP, PNULL);
        break;

    //case MSG_APP_OK:
    case MSG_APP_WEB:
        UpdateAudioOption(win_id);
        break;

    case MSG_APP_LEFT:
    case MSG_APP_RIGHT:
    case MSG_APP_UP:
    case MSG_APP_DOWN:
		//这几个按键不需要响应
        break;


    case MSG_VT_REMOTE_ALERTING_IND://收到对方开始振铃的消息
        //显示当前状态
        DisplayCallStateLabel(win_id, TRUE);
        //开启本地振铃音
        {
#if 1//OPEN_VT_CHANGE
#ifdef MMISRV_AUDIO_SUPPORT
            MMISRVAUD_PlayToneEx(MMISRVAUD_PRI_HIGH, MMISRVAUD_TYPE_TONE_GENERIC, VT_NT_RING_CON_ID, MMI_1SECONDS,MMISRVAUD_TONE_FREQ_DEFAULT);//bug337098
            //MMISRVAUD_PlayTone(MMISRVAUD_TYPE_TONE_GENERIC, VT_NT_RING_CON_ID, MMI_1SECONDS, 2000);
#else
            MMIAUDIO_PlayTone(MMIAUDIO_GENERIC_TONE, VT_NT_RING_CON_ID, MMI_1SECONDS, MMIAUDIO_TONE_DEFAULT_FREQ);
#endif
#else
            MMIAUDIO_PlayTone(MMIAUDIO_GENERIC_TONE, VT_NT_RING_CON_ID, MMI_1SECONDS, MMIAUDIO_TONE_DEFAULT_FREQ);
#endif
            StartWinTimer(win_id, &s_mo_local_net_ring_timer, VT_THREE_SECONDS_TIMEOUT);
        }
        break;

    case MSG_TIMER:
        if (s_mo_local_net_ring_timer == *(uint8*)param)
        {
            //振动
#if 1//OPEN_VT_CHANGE
#ifdef MMISRV_AUDIO_SUPPORT
            MMISRVAUD_PlayToneEx(MMISRVAUD_PRI_HIGH, MMISRVAUD_TYPE_TONE_GENERIC, VT_NT_RING_CON_ID, MMI_1SECONDS,MMISRVAUD_TONE_FREQ_DEFAULT);//bug337098
            //MMISRVAUD_PlayTone(MMISRVAUD_TYPE_TONE_GENERIC, VT_NT_RING_CON_ID, MMI_1SECONDS, 2000);
#else
            MMIAUDIO_PlayTone(MMIAUDIO_GENERIC_TONE, VT_NT_RING_CON_ID, MMI_1SECONDS, MMIAUDIO_TONE_DEFAULT_FREQ);
#endif
#else
            MMIAUDIO_PlayTone(MMIAUDIO_GENERIC_TONE, VT_NT_RING_CON_ID, MMI_1SECONDS, MMIAUDIO_TONE_DEFAULT_FREQ);
#endif
            StartWinTimer(win_id, &s_mo_local_net_ring_timer, VT_THREE_SECONDS_TIMEOUT);                
        }
        else
        {
            recode = MMI_RESULT_FALSE;
        }
        break; 

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
        if (PNULL != param)
        {
            uint32 src_id = ((MMI_NOTIFY_T*)param)->src_id;

            if(MMIVT_BUTTON_AUDIO_OPTION_CTRL_ID == ((MMI_NOTIFY_T*)param)->src_id)
            {
                UpdateAudioOption(win_id);
                break;
            }
            
            switch (src_id)
            {
            case MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID:
                MMK_SendMsg(win_id, MSG_APP_OK, PNULL);
                break;
            case MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID:
                MMK_SendMsg(win_id, MSG_APP_CANCEL, PNULL);
                break;
            default:
                break;
            }
        }

        break;   
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

    case MSG_CLOSE_WINDOW:
        //关闭本地振铃音
        {
            StopWinTimer(&s_mo_local_net_ring_timer);
#if 1//OPEN_VT_CHANGE
#ifdef MMISRV_AUDIO_SUPPORT
            MMISRVAUD_StopTone(MMISRVAUD_TYPE_TONE_GENERIC);
#else
            MMIAUDIO_StopTone(MMIAUDIO_GENERIC_TONE);
#endif
#else
            MMIAUDIO_StopTone(MMIAUDIO_GENERIC_TONE);
#endif
        }
        StopVTBlock();

        //enable gsensor rotate
        DisableGsensorRotate(win_id, FALSE);
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    return recode;
}
 
/*****************************************************************************/
// 	Description : HandleMtIncomingWinMsg
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMtIncomingWinMsg( 
                                             MMI_WIN_ID_T win_id, 
                                             MMI_MESSAGE_ID_E msg_id, 
                                             DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMISET_CALL_RING_TYPE_E call_ring_type = MMISET_CALL_RING;
    LOCAL BOOLEAN s_is_first_silence_in_vt = FALSE;
    //任意键接听电话处理
    if ((msg_id > MSG_KEYDOWN) && (msg_id <= MSG_KEYDOWN_HEADSET_BUTTON)
        && (msg_id != MSG_APP_CANCEL)
        && (msg_id != MSG_APP_RED)
        && (msg_id != MSG_APP_WEB)
        )
    {
        MMISET_ANYKEY_REJECT_SET_T reject_set = MMIAPISET_GetAnykeyRejectMode();
        if (reject_set.is_anykey_answer)
        {
            //any key to accept call, beside RIGHT_SOFT_KEY and POWER_KEY
            msg_id = MSG_APP_OK;
        }
    }
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            GUI_BOTH_RECT_T mt_anim_both_rect = MT_ANIM_BOTH_RECT;
            GUI_BOTH_RECT_T mt_bg_bank_both_rect = MT_BG_BANK_BOTH_RECT;
            GUI_BOTH_RECT_T mt_name_label_both_rect = MT_NAME_LABEL_BOTH_RECT;
            GUI_BOTH_RECT_T mt_state_label_both_rect = MT_STATE_LABEL_BOTH_RECT;

            GUIAPICTRL_SetBothRect(MMK_GetCtrlHandleByWin(win_id, MMIVT_LABEL_CALLSTATE_CTRL_ID), &mt_state_label_both_rect);
            GUIAPICTRL_SetBothRect(MMK_GetCtrlHandleByWin(win_id, MMIVT_LABEL_CALLNAME_CTRL_ID), &mt_name_label_both_rect);
            GUIAPICTRL_SetBothRect(MMK_GetCtrlHandleByWin(win_id, MMIVT_ANIM_INCOMING_CALL_CTRL_ID), &mt_anim_both_rect);
            GUIAPICTRL_SetBothRect(MMK_GetCtrlHandleByWin(win_id, MMIVT_ANIM_OPTION_BG_BANK_CTRL_ID), &mt_bg_bank_both_rect);

        }

#ifdef STREAMING_HS_SUPPORT // chenxiang 20100415
        MMIAPISMP_CloseSMP(TRUE);
#endif // STREAMING_HS_SUPPORT
        //播放来电铃声
        call_ring_type = MMIAPICC_StartRingOrVibrate(TRUE);

        if (call_ring_type == MMISET_CALL_RING 
            || call_ring_type == MMISET_CALL_VIBRA_AND_RING
            || call_ring_type == MMISET_CALL_VIBRA_BEFORE_RING
            &&(0 != MMIAPIENVSET_GetCurModeRingVol(MMISET_RING_TYPE_CALL)))
        {
            s_is_first_silence_in_vt = TRUE;
        }
        else
        {
            s_is_first_silence_in_vt = FALSE;
        }

        //CLEAR LCD
        {
            FreshRect(win_id);
        }

        HandleVTWinOpenWinMsg(win_id);
        {
            GUI_BG_T bg = {0};
            bg.bg_type = GUI_BG_IMG;
            bg.img_id = IMAG_VT_MAIN_PLAY_BG;
            bg.shape = GUI_SHAPE_RECT;
            DisplayAnim(win_id, MMIVT_ANIM_INCOMING_CALL_CTRL_ID, ANIM_MO_CALL, PNULL, &bg); 
        }
        
        DisplayCallStateLabel(win_id, FALSE);
        DisplayCallNameLabel(win_id);

        //仅用于09b 10a支持applet获得焦点时，收到resume消息
        MMIDEFAULT_SetBackLight(TRUE);		//点亮背光
        MMIDEFAULT_AllowTurnOffBackLight(FALSE);	//背光常开   
        break;

    case MSG_FULL_PAINT:
        /*{
            GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
            GUI_POINT_T point = {MAIN_PLAY_RECT_LEFT, MAIN_PLAY_RECT_TOP};
            GUI_RECT_T display_rect = {MAIN_PLAY_RECT_LEFT, MAIN_PLAY_RECT_TOP, MAIN_PLAY_RECT_RIGHT, MAIN_PLAY_RECT_BOTTOM};
            GUI_RECT_T image_rect = {0, 0, (MAIN_PLAY_RECT_RIGHT - MAIN_PLAY_RECT_LEFT), (MAIN_PLAY_RECT_BOTTOM - MAIN_PLAY_RECT_TOP)};
            GUIRES_DisplayImg(&point, &display_rect, &image_rect, win_id, IMAG_VT_MAIN_PLAY_BG,&lcd_dev_info);
        }*/

        if (s_is_first_silence_in_vt)
        {
            GUIWIN_SeSoftkeytButtonTextId(win_id, TXT_RING_SILENT, RIGHT_BUTTON, FALSE);
        }
        else
        {
            GUIWIN_SeSoftkeytButtonTextId(win_id, STXT_REJECT, RIGHT_BUTTON, FALSE);
        }

        //处理自动接听
        if (MMIAPIENG_GetAutoAnswerCallSet())
        {
            //SCI_TRACE_LOW:"mmivt auto accept mt"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_UI_2898_112_2_18_3_6_17_163,(uint8*)"");
            MMK_PostMsg(win_id, MSG_APP_GREEN, PNULL, 0);
        }
        break;

    //仅用于09b 10a支持applet获得焦点时，收到resume消息
    case MSG_GET_FOCUS:
        MMIDEFAULT_SetBackLight(TRUE);		//点亮背光
        MMIDEFAULT_AllowTurnOffBackLight(FALSE);	//背光常开      
        break;

    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_APP_GREEN:
    case MSG_KEYDOWN_HEADSET_BUTTON:
    case MSG_BT_ACCEPT_INCOMING_CALL:
        s_is_first_silence_in_vt = FALSE;

        //停止播放铃声
        MMIAPICC_StopRingOrVibrate();
        //接听电话
        MMIVT_HandleUIMsg(MMIVT_UI_MSG_CONNECT, PNULL);
        break;

    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        s_is_first_silence_in_vt = FALSE;

        //停止播放铃声
        MMIAPICC_StopRingOrVibrate();
        //语音接听
#ifdef ASP_SUPPORT
        MMICC_SetAcceptCurCall(TRUE);
#endif        
        MMIVT_HandleUIMsg(MMIVT_UI_MSG_FB_REQ, PNULL);
        break;
        
    case MSG_APP_CANCEL:
    case MSG_APP_RED:
    case MSG_BT_CANCEL_OUTGOING_CALL: //blue tooth headset cancel outgoing call	
    case MSG_BT_REJECT_INCOMING_CALL: //blue tooth headset reject the incoming call           
        if (msg_id == MSG_APP_CANCEL)
        {
            if (s_is_first_silence_in_vt)
            {
                s_is_first_silence_in_vt = FALSE;

                MMIAPICC_StopRingOrVibrate();

                MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
                break;
            }
        }

        //停止播放铃声
        MMIAPICC_StopRingOrVibrate();
        //挂断电话
        MMIVT_HandleUIMsg(MMIVT_UI_MSG_HANGUP, PNULL);
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
        if (PNULL != param)
        {
            uint32 src_id = ((MMI_NOTIFY_T*)param)->src_id;

            switch (src_id)
            {
            case MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID:
                MMK_SendMsg(win_id, MSG_APP_OK, PNULL);
                break;
            case MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID:
                MMK_SendMsg(win_id, MSG_APP_WEB, PNULL);
                break;
            case MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID:
                MMK_SendMsg(win_id, MSG_APP_CANCEL, PNULL);
                break;
            default:
                break;
            }
        }

        break;   
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

//bug337348
    case MSG_APP_LEFT:
    case MSG_APP_RIGHT:
    case MSG_APP_UP:
    case MSG_APP_DOWN:
		//这几个按键不需要响应
        break;

    case MSG_APP_UPSIDE:
    case MSG_APP_DOWNSIDE:
    case MSG_APP_CAMERA:
        if (s_is_first_silence_in_vt)
        {
            s_is_first_silence_in_vt = FALSE;

            MMIAPICC_StopRingOrVibrate();

            MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
        }
        break;

    case MSG_CLOSE_WINDOW:
        s_is_first_silence_in_vt = FALSE;

        //停止播放铃声
        MMIAPICC_StopRingOrVibrate();
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    return recode;
}


/*****************************************************************************/
// 	Description : HandleConnectingWinMsg
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleConnectingWinMsg( 
                                             MMI_WIN_ID_T win_id, 
                                             MMI_MESSAGE_ID_E msg_id, 
                                             DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            GUI_BOTH_RECT_T connecting_anim_both_rect = CONNECTING_ANIM_BOTH_RECT;
            GUI_BOTH_RECT_T connecting_bg_bank_both_rect = CONNECTING_BG_BANK_BOTH_RECT;
            GUI_BOTH_RECT_T connecting_name_label_both_rect = CONNECTING_NAME_LABEL_BOTH_RECT;
            GUI_BOTH_RECT_T connecting_state_label_both_rect = CONNECTING_STATE_LABEL_BOTH_RECT;

            GUIAPICTRL_SetBothRect(MMK_GetCtrlHandleByWin(win_id, MMIVT_LABEL_CALLSTATE_CTRL_ID), &connecting_state_label_both_rect);
            GUIAPICTRL_SetBothRect(MMK_GetCtrlHandleByWin(win_id, MMIVT_LABEL_CALLNAME_CTRL_ID), &connecting_name_label_both_rect);
            GUIAPICTRL_SetBothRect(MMK_GetCtrlHandleByWin(win_id, MMIVT_ANIM_CALL_CONNECTING_CTRL_ID), &connecting_anim_both_rect);
            GUIAPICTRL_SetBothRect(MMK_GetCtrlHandleByWin(win_id, MMIVT_ANIM_OPTION_BG_BANK_CTRL_ID), &connecting_bg_bank_both_rect);

        }
        
        HandleVTWinOpenWinMsg(win_id);
#if 1//OPEN_VT_CHANGE
        DisplayAnim(win_id, MMIVT_ANIM_CALL_CONNECTING_CTRL_ID, ANIM_MO_CALL, PNULL, PNULL); 
#else
        DisplayAnim(win_id, MMIVT_ANIM_CALL_CONNECTING_CTRL_ID, IMAGE_CALL_COMMUN, PNULL, PNULL); 
#endif
        DisplayCallStateLabel(win_id, FALSE);
        DisplayCallNameLabel(win_id);
        //disable gsensor rotate
        DisableGsensorRotate(win_id, TRUE);        
        break;

    case MSG_FULL_PAINT:
        {
            GUI_POINT_T point = {0, 0};
            GUI_RECT_T display_rect = {0, 0, MAIN_PLAY_RECT_RIGHT, MMI_STATUSBAR_HEIGHT};
            GUI_RECT_T image_rect = MMITHEME_GetFullScreenRect();
            GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
            GUIRES_DisplayImg(&point, &display_rect, &image_rect, win_id, IMAGE_COMMON_TITLE_BAR,&lcd_dev_info);
        }
        break;
        
    case MSG_APP_CANCEL:
    case MSG_APP_RED:
        //挂断电话
        MMIVT_HandleUIMsg(MMIVT_UI_MSG_HANGUP, PNULL);
        break;

//bug339182
    case MSG_APP_LEFT:
    case MSG_APP_RIGHT:
    case MSG_APP_UP:
    case MSG_APP_DOWN:
		//这几个按键不需要响应
        break;

    case MSG_CLOSE_WINDOW:
        //disable gsensor rotate
        DisableGsensorRotate(win_id, FALSE);
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    return recode;
}

/*****************************************************************************/
// 	Description : HandleConnectedWinMsg
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleConnectedWinMsg( 
                                            MMI_WIN_ID_T win_id, 
                                            MMI_MESSAGE_ID_E msg_id, 
                                            DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    VT_DISPLAY_INFO_T display_info = MMIVT_GetAppletInstance()->vt_instance.ui_display_info;
#ifdef TOUCH_PANEL_SUPPORT
    GUI_RECT_T      key_rect1 = {DIAL_KEY_DEL_LEFT, DIAL_KEY_DEL_TOP, DIAL_KEY_DEL_RIGHT, DIAL_KEY_DEL_BOTTOM};
    GUI_RECT_T      key_rect2 = {DIAL_KEY_1_LEFT, DIAL_KEY_1_TOP, DIAL_KEY_1_RIGHT, DIAL_KEY_1_BOTTOM};
    GUI_RECT_T      key_rect3 = {DIAL_KEY_2_LEFT, DIAL_KEY_2_TOP, DIAL_KEY_2_RIGHT, DIAL_KEY_2_BOTTOM};
    GUI_RECT_T      key_rect4 = {DIAL_KEY_3_LEFT, DIAL_KEY_3_TOP, DIAL_KEY_3_RIGHT, DIAL_KEY_3_BOTTOM};
    GUI_RECT_T      key_rect5 = {DIAL_KEY_4_LEFT, DIAL_KEY_4_TOP, DIAL_KEY_4_RIGHT, DIAL_KEY_4_BOTTOM};
    GUI_RECT_T      key_rect6 = {DIAL_KEY_5_LEFT, DIAL_KEY_5_TOP, DIAL_KEY_5_RIGHT, DIAL_KEY_5_BOTTOM};
    GUI_RECT_T      key_rect7 = {DIAL_KEY_6_LEFT, DIAL_KEY_6_TOP, DIAL_KEY_6_RIGHT, DIAL_KEY_6_BOTTOM};
    GUI_RECT_T      key_rect8 = {DIAL_KEY_7_LEFT, DIAL_KEY_7_TOP, DIAL_KEY_7_RIGHT, DIAL_KEY_7_BOTTOM};
    GUI_RECT_T      key_rect9 = {DIAL_KEY_8_LEFT, DIAL_KEY_8_TOP, DIAL_KEY_8_RIGHT, DIAL_KEY_8_BOTTOM};
    GUI_RECT_T      key_rect10 = {DIAL_KEY_9_LEFT, DIAL_KEY_9_TOP, DIAL_KEY_9_RIGHT, DIAL_KEY_9_BOTTOM};
    GUI_RECT_T      key_rect11 = {DIAL_KEY_0_LEFT, DIAL_KEY_0_TOP, DIAL_KEY_0_RIGHT, DIAL_KEY_0_BOTTOM};
    GUI_RECT_T      key_rect12 = {DIAL_KEY_STAR_LEFT, DIAL_KEY_STAR_TOP, DIAL_KEY_STAR_RIGHT, DIAL_KEY_STAR_BOTTOM};
    GUI_RECT_T      key_rect13 = {DIAL_KEY_HASH_LEFT, DIAL_KEY_HASH_TOP, DIAL_KEY_HASH_RIGHT, DIAL_KEY_HASH_BOTTOM};
#endif

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
#ifdef TOUCH_PANEL_SUPPORT
        GUIBUTTON_SetRect(MMIVT_BUTTON_DIAL_DEL_CTRL_ID,&key_rect1);
        GUIBUTTON_SetRect(MMIVT_BUTTON_DIAL_1_CTRL_ID,&key_rect2);
        GUIBUTTON_SetRect(MMIVT_BUTTON_DIAL_2_CTRL_ID,&key_rect3);
        GUIBUTTON_SetRect(MMIVT_BUTTON_DIAL_3_CTRL_ID,&key_rect4);
        GUIBUTTON_SetRect(MMIVT_BUTTON_DIAL_4_CTRL_ID,&key_rect5);
        GUIBUTTON_SetRect(MMIVT_BUTTON_DIAL_5_CTRL_ID,&key_rect6);
        GUIBUTTON_SetRect(MMIVT_BUTTON_DIAL_6_CTRL_ID,&key_rect7);
        GUIBUTTON_SetRect(MMIVT_BUTTON_DIAL_7_CTRL_ID,&key_rect8);
        GUIBUTTON_SetRect(MMIVT_BUTTON_DIAL_8_CTRL_ID,&key_rect9);
        GUIBUTTON_SetRect(MMIVT_BUTTON_DIAL_9_CTRL_ID,&key_rect10);
        GUIBUTTON_SetRect(MMIVT_BUTTON_DIAL_0_CTRL_ID,&key_rect11);
        GUIBUTTON_SetRect(MMIVT_BUTTON_DIAL_STAR_CTRL_ID,&key_rect12);
        GUIBUTTON_SetRect(MMIVT_BUTTON_DIAL_HASH_CTRL_ID,&key_rect13);
#endif

        {
            GUI_BOTH_RECT_T bg_bank_both_rect = BG_BANK_BOTH_RECT;
            GUI_BOTH_RECT_T main_play_both_rect = MAIN_PLAY_BOTH_RECT;
            GUI_BOTH_RECT_T photo_both_rect = PHOTO_BOTH_RECT;
            GUI_BOTH_RECT_T audio_option_button_both_rect = AUDIO_OPTION_BUTTON_BOTH_RECT;
            GUI_BOTH_RECT_T display_option_button_both_rect = DISPLAY_OPTION_BUTTON_BOTH_RECT;
            GUI_BOTH_RECT_T camera_option_button_both_rect = CAMERA_OPTION_BUTTON_BOTH_RECT;
            GUI_BOTH_RECT_T dispmode_option_button_both_rect = DISPMODE_OPTION_BUTTON_BOTH_RECT;
            GUI_BOTH_RECT_T time_label_both_rect = TWO_LINE_LABEL1_BOTH_RECT;
            GUI_BOTH_RECT_T name_label_both_rect = TWO_LINE_LABEL2_BOTH_RECT;
            GUI_BOTH_RECT_T lcd_brightness_button_both_rect = LCD_BRIGHTNESS_BUTTON_BOTH_RECT;
            GUI_BOTH_RECT_T contrast_button_both_rect = CONTRAST_BUTTON_BOTH_RECT;
            GUI_BOTH_RECT_T brightness_button_both_rect = BRIGHTNESS_BUTTON_BOTH_RECT;
            GUI_BOTH_RECT_T rotate_button_both_rect = ROTATE_BUTTON_BOTH_RECT;
            GUI_BOTH_RECT_T minus_button_both_rect = MINUS_BUTTON_BOTH_RECT;
            GUI_BOTH_RECT_T add_button_both_rect = ADD_BUTTON_BOTH_RECT; 
            GUI_BOTH_RECT_T rotate_90_button_both_rect = ROTATE_90_BUTTON_BOTH_RECT; /*lint !e778*/
            GUI_BOTH_RECT_T rotate_180_button_both_rect = ROTATE_180_BUTTON_BOTH_RECT;/*lint !e778*/
            GUI_BOTH_RECT_T sub_option_both_rect = SUB_OPTION_BG_BOTH_RECT; 
            GUI_BOTH_RECT_T prompt_both_rect = PROMPT_BOTH_RECT;/*lint !e778*/
            GUI_BOTH_RECT_T video_param_both_rect = VIDEO_PARAM_BOTH_RECT; 
            GUI_BOTH_RECT_T volume_param_both_rect = VOLUME_PARAM_BOTH_RECT;/*lint !e778*/

            GUIAPICTRL_SetBothRect(MMK_GetCtrlHandleByWin(win_id, MMIVT_LABEL_CALLTIME_CTRL_ID), &time_label_both_rect);
            GUIAPICTRL_SetBothRect(MMK_GetCtrlHandleByWin(win_id, MMIVT_LABEL_CALLNAME_CTRL_ID), &name_label_both_rect);
            GUIAPICTRL_SetBothRect(MMK_GetCtrlHandleByWin(win_id, MMIVT_ANIM_OPTION_BG_BANK_CTRL_ID), &bg_bank_both_rect);
            GUIAPICTRL_SetBothRect(MMK_GetCtrlHandleByWin(win_id, MMIVT_ANIM_MO_CALLING_CTRL_ID), &main_play_both_rect);
            GUIAPICTRL_SetBothRect(MMK_GetCtrlHandleByWin(win_id, MMIVT_LABEL_PHOTO_CTRL_ID), &photo_both_rect);
            GUIAPICTRL_SetBothRect(MMK_GetCtrlHandleByWin(win_id, MMIVT_BUTTON_AUDIO_OPTION_CTRL_ID), &audio_option_button_both_rect);
            GUIAPICTRL_SetBothRect(MMK_GetCtrlHandleByWin(win_id, MMIVT_BUTTON_VIDEO_OPTION_CTRL_ID), &display_option_button_both_rect);
            GUIAPICTRL_SetBothRect(MMK_GetCtrlHandleByWin(win_id, MMIVT_BUTTON_CAMERA_OPTION_CTRL_ID), &camera_option_button_both_rect);
            GUIAPICTRL_SetBothRect(MMK_GetCtrlHandleByWin(win_id, MMIVT_BUTTON_DISPMODE_OPTION_CTRL_ID), &dispmode_option_button_both_rect);
            GUIAPICTRL_SetBothRect(MMK_GetCtrlHandleByWin(win_id, MMIVT_BUTTON_DISPLAY_LCD_BRIGHTNESS_CTRL_ID), &lcd_brightness_button_both_rect);
            GUIAPICTRL_SetBothRect(MMK_GetCtrlHandleByWin(win_id, MMIVT_BUTTON_DISPLAY_CONTRAST_CTRL_ID), &contrast_button_both_rect);
            GUIAPICTRL_SetBothRect(MMK_GetCtrlHandleByWin(win_id, MMIVT_BUTTON_DISPLAY_BRIGHTNESS_CTRL_ID), &brightness_button_both_rect);
            GUIAPICTRL_SetBothRect(MMK_GetCtrlHandleByWin(win_id, MMIVT_BUTTON_DISPLAY_ROTATE_CTRL_ID), &rotate_button_both_rect);
            GUIAPICTRL_SetBothRect(MMK_GetCtrlHandleByWin(win_id, MMIVT_BUTTON_VIDEO_PARAM_MINUS_CTRL_ID), &minus_button_both_rect);
            GUIAPICTRL_SetBothRect(MMK_GetCtrlHandleByWin(win_id, MMIVT_BUTTON_VIDEO_PARAM_ADD_CTRL_ID), &add_button_both_rect);
            GUIAPICTRL_SetBothRect(MMK_GetCtrlHandleByWin(win_id, MMIVT_BUTTON_ROTATE_90_CTRL_ID), &rotate_90_button_both_rect);
            GUIAPICTRL_SetBothRect(MMK_GetCtrlHandleByWin(win_id, MMIVT_BUTTON_ROTATE_180_CTRL_ID), &rotate_180_button_both_rect);
            GUIAPICTRL_SetBothRect(MMK_GetCtrlHandleByWin(win_id, MMIVT_BUTTON_VOLUME_MINUS_CTRL_ID), &minus_button_both_rect);
            GUIAPICTRL_SetBothRect(MMK_GetCtrlHandleByWin(win_id, MMIVT_BUTTON_VOLUME_ADD_CTRL_ID), &add_button_both_rect);
            GUIAPICTRL_SetBothRect(MMK_GetCtrlHandleByWin(win_id, MMIVT_LABEL_SUBOPTION_BG_CTRL_ID), &sub_option_both_rect);
            GUIAPICTRL_SetBothRect(MMK_GetCtrlHandleByWin(win_id, MMIVT_LABEL_PROMOPT_CTRL_ID), &prompt_both_rect);
            GUIAPICTRL_SetBothRect(MMK_GetCtrlHandleByWin(win_id, MMIVT_LABEL_VIDEO_PARAM_CTRL_ID), &video_param_both_rect);
            GUIAPICTRL_SetBothRect(MMK_GetCtrlHandleByWin(win_id, MMIVT_LABEL_VOLUME_PARAM_CTRL_ID), &volume_param_both_rect);            
        }

        //CLEAR LCD
        {
            FreshRect(win_id);
        }  

        HandleVTWinOpenWinMsg(win_id);
        StartWinTimer(win_id, &s_vt_time_count_timer, VT_ONE_SECOND_TIMEOUT);

        HideVideoOption(win_id);
        HideVideoParam(win_id);
        HideVolume(win_id);
        //隐藏userinput输入控件
#ifdef TOUCH_PANEL_SUPPORT
        SetCtrlVisible(win_id, MMIVT_BUTTON_DIAL_DEL_CTRL_ID, FALSE);
        SetCtrlVisible(win_id, MMIVT_BUTTON_DIAL_1_CTRL_ID, FALSE);
        SetCtrlVisible(win_id, MMIVT_BUTTON_DIAL_2_CTRL_ID, FALSE);
        SetCtrlVisible(win_id, MMIVT_BUTTON_DIAL_3_CTRL_ID, FALSE);
        SetCtrlVisible(win_id, MMIVT_BUTTON_DIAL_4_CTRL_ID, FALSE);
        SetCtrlVisible(win_id, MMIVT_BUTTON_DIAL_5_CTRL_ID, FALSE);
        SetCtrlVisible(win_id, MMIVT_BUTTON_DIAL_6_CTRL_ID, FALSE);
        SetCtrlVisible(win_id, MMIVT_BUTTON_DIAL_7_CTRL_ID, FALSE);
        SetCtrlVisible(win_id, MMIVT_BUTTON_DIAL_8_CTRL_ID, FALSE);
        SetCtrlVisible(win_id, MMIVT_BUTTON_DIAL_9_CTRL_ID, FALSE);
        SetCtrlVisible(win_id, MMIVT_BUTTON_DIAL_0_CTRL_ID, FALSE);
        SetCtrlVisible(win_id, MMIVT_BUTTON_DIAL_STAR_CTRL_ID, FALSE);
        SetCtrlVisible(win_id, MMIVT_BUTTON_DIAL_HASH_CTRL_ID, FALSE);
#endif    

        if (MMICC_AUDIO_DEV_HF == MMICC_GetCurrentDev())
        {
            MMIVT_GetAppletInstance()->vt_instance.ui_display_info.is_handfree_show = FALSE;
        }
        else
        {
            MMIVT_GetAppletInstance()->vt_instance.ui_display_info.is_handfree_show = TRUE;
        }
        EnterMainOptionMode(win_id);

        //如果是多媒体彩铃状态，不显示
        DisplayCallTimeLabel(win_id, FALSE);
        DisplayCallNameLabel(win_id);
        

        //仅用于09b 10a支持applet获得焦点时，收到resume消息
        MMIDEFAULT_SetBackLight(TRUE);		//点亮背光
		MMIDEFAULT_AllowTurnOffBackLight(FALSE);	//背光常开 
        SetConnectedWinSoftKey(win_id, FALSE);

         //给视频显示区域刷透明色
        SetMainPlayArea(win_id, TRUE);
        SetSecondaryPlayArea(win_id, TRUE);

        //设置mmi block
        StartVTBlock();

        //disable gsensor rotate
        DisableGsensorRotate(win_id, TRUE);

        //进入通话状态，显示双方视频（VT_DialStart）
        //MMIVT_REF_StartTalk(); 
        break;
        
    case MSG_FULL_PAINT://cr226858 maryxiao
        if(!MMIAPIDM_GetHandsetLock()) 
        {
            SetConnectedWinSoftKey(win_id, FALSE);
        }
        else
        {
            GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_NULL, STXT_RETURN, FALSE);    
        }
        break;

    case MSG_TIMER:
        recode = HandleVTConnectedWinTimeOut(win_id, param);
        
        break;
        

    case MSG_APP_RED:
    case MSG_KEYDOWN_HEADSET_BUTTON:
        //挂断电话
        MMIVT_HandleUIMsg(MMIVT_UI_MSG_HANGUP, PNULL);
        break;

#if 1//ndef TOUCH_PANEL_SUPPORT
    case MSG_APP_LEFT:
    case MSG_APP_RIGHT:
        recode = HandleVTConnectedWinLeftAndRightKey(win_id, msg_id);
        break;
#endif

    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        if(!MMIAPIDM_GetHandsetLock()) //added by mary cr226858
        {
            recode = HandleVTConnectedWinWebKey(win_id);
        }
        break;

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:

            recode = HandleVTConnectedWinCancelKey(win_id);

        break;

    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_APP_MENU:
        if(!MMIAPIDM_GetHandsetLock()) //added by mary cr226858
        {
            recode = HandleVTConnectedWinOkKey(win_id);
        }
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
        if(!MMIAPIDM_GetHandsetLock()) //added by mary cr226858
        {
            recode = HandleVTConnectedWinPenOk(win_id, ((MMI_NOTIFY_T*)param)->src_id);
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

    case MSG_APP_UP:
    case MSG_APP_DOWN:
    case MSG_APP_UPSIDE:
    case MSG_APP_DOWNSIDE:
        recode = HandleVTConnectedWinUpAndDownKey(win_id, msg_id);
        break;

    case MSG_KEYDOWN_1:
    case MSG_KEYDOWN_2:
    case MSG_KEYDOWN_3:
    case MSG_KEYDOWN_4:
    case MSG_KEYDOWN_5:
    case MSG_KEYDOWN_6:
    case MSG_KEYDOWN_7:
    case MSG_KEYDOWN_8:
    case MSG_KEYDOWN_9:
    case MSG_KEYDOWN_0:
    case MSG_KEYDOWN_STAR:
    case MSG_KEYDOWN_HASH:
        recode = HandleVTConnectedWinPhoneNumKey(win_id, msg_id);
        break;

    case MSG_NOTIFY_EDITBOX_UPDATE_STRNUM:
        //userinput edit string 为零时，需要设置softkey（其他时间softkey由edit ctrl处理）
        if (MMIVT_OPTION_BANK_MODE_USERINPUT == display_info.option_bank_mode)
        {
            MMI_STRING_T userinput_str = {0};
            MMI_HANDLE_T userinput_edit_handle = MMK_GetCtrlHandleByWin(win_id, MMIVT_EDIT_USERINPUT_CTRL_ID);
            GUIEDIT_GetString(userinput_edit_handle, &userinput_str);
            if (0 == userinput_str.wstr_len)
            {
                GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_NULL, STXT_RETURN, TRUE);
            }
        }
        break;

    case MSG_LOSE_FOCUS:
        //暂停音量调节界面关闭定时和视频参数调节界面关闭定时
        MMK_PauseTimer(s_vt_volume_close_timer);
        MMK_PauseTimer(s_vt_video_param_timer);
        
        StopVTBlock();
        //失去焦点时让视频区域刷上黑色
        SetMainPlayArea(win_id, FALSE);
        SetSecondaryPlayArea(win_id, FALSE);
        break;

    case MSG_GET_FOCUS:
        //仅用于09b 10a支持applet获得焦点时，收到resume消息
        MMIDEFAULT_SetBackLight(TRUE);		//点亮背光
        MMIDEFAULT_AllowTurnOffBackLight(FALSE);	//背光常开
        FreshRect(win_id);
        SetConnectedWinSoftKey(win_id, FALSE);

         //给视频显示区域刷透明色
        SetMainPlayArea(win_id, TRUE);
        SetSecondaryPlayArea(win_id, TRUE);

        StartVTBlock();
        //恢复音量调节界面关闭定时和视频参数调节界面关闭定时
        MMK_ResumeTimer(s_vt_volume_close_timer);
        MMK_ResumeTimer(s_vt_video_param_timer);
        
        break;

    case MSG_CLOSE_WINDOW:
        GUIBLOCK_ResetMainType();
        StopWinTimer(&s_vt_time_count_timer);
        StopWinTimer(&s_vt_open_camera_timer);
        //退出dial模式
        StopVTBlock();

        //enable gsensor rotate
        DisableGsensorRotate(win_id, FALSE);
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    return recode;
}



/*****************************************************************************/
// 	Description : handle timeout msg in vt connected win
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleVTConnectedWinTimeOut(MMI_WIN_ID_T win_id,DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
#ifdef TOUCH_PANEL_SUPPORT
    MMIVT_OPTION_BANK_MODE_E option_mode = MMIVT_GetAppletInstance()->vt_instance.ui_display_info.option_bank_mode;
#endif               
    if (s_vt_time_count_timer == *(uint8*)param)
    {

        if (MMVT_KERNEL_SUBSTATE_MMR != MMIVT_GetSubState())
        {
            MMIVT_GetAppletInstance()->vt_instance.ui_call_time_count++;

//触摸屏时userinput输入盘与时间显示区域重合，打开输入盘需要隐藏时间显示
#ifdef TOUCH_PANEL_SUPPORT
            if (MMIVT_OPTION_BANK_MODE_USERINPUT == option_mode)
            {
                DisplayCallTimeLabel(win_id, FALSE);
            }
            else
 #endif               
            {
                DisplayCallTimeLabel(win_id, TRUE);
            }
        
            StartWinTimer(win_id, &s_vt_time_count_timer, VT_ONE_SECOND_TIMEOUT);
        }
        else
        {
            // time label don't show time
        }

    }
    else if (s_vt_volume_close_timer == *(uint8*)param)
    {
        QuitVolumeMode(win_id);
        EnterMainOptionMode(win_id);
        MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
    }
    else if (s_vt_video_param_timer == *(uint8*)param)
    {
        QuitVideoParamMode(win_id);
        EnterVideoOptionMode(win_id);
        MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
    }
    else if (s_vt_open_camera_timer == *(uint8*)param)
    {
        StopRemoteReplaceImage();        
    }       
    else
    {
        result = MMI_RESULT_FALSE; 
    }
    return result;
}

/*****************************************************************************/
// 	Description : HandleVTConnectedWinUpAndDownKey
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleVTConnectedWinUpAndDownKey(MMI_WIN_ID_T win_id,MMI_MESSAGE_ID_E msg_id)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;

    VT_DISPLAY_INFO_T display_info = MMIVT_GetAppletInstance()->vt_instance.ui_display_info;
    
    //SCI_TRACE_LOW:"mmivt HandleVTConnectedWinUpAndDownKey mode: %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_UI_3422_112_2_18_3_6_18_164,(uint8*)"d",display_info.option_bank_mode);
    switch(display_info.option_bank_mode) 
    {
    case MMIVT_OPTION_BANK_MODE_NORMAL:
        QuitMainOptionMode(win_id);
        EnterVolumeMode(win_id);
        MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
        break;

    case MMIVT_OPTION_BANK_MODE_VIDEO_OPTION:
        QuitVideoParamMode(win_id);
        QuitVideoOptionMode(win_id);
        EnterVolumeMode(win_id);
        MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
        break;

    case MMIVT_OPTION_BANK_MODE_VOLUME:
        UpdateVolume(win_id, msg_id);
        break;

    default:
        //do nothing
        break;
    }

    return result;
}
#if 1//ndef TOUCH_PANEL_SUPPORT
/*****************************************************************************/
// 	Description : HandleVTConnectedWinLeftAndRightKey
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleVTConnectedWinLeftAndRightKey(MMI_WIN_ID_T win_id,MMI_MESSAGE_ID_E msg_id)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;

    VT_DISPLAY_INFO_T display_info = MMIVT_GetAppletInstance()->vt_instance.ui_display_info;
    
    //SCI_TRACE_LOW:"mmivt HandleVTConnectedWinLeftAndRightKey mode: %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_UI_3460_112_2_18_3_6_18_165,(uint8*)"d",display_info.option_bank_mode);
    switch(display_info.option_bank_mode) 
    {
    case MMIVT_OPTION_BANK_MODE_NORMAL:
        //变换第一级选项焦点
        SwitchMainOptionFocus(win_id, msg_id);
    	break;

    case MMIVT_OPTION_BANK_MODE_VIDEO_OPTION:
        {
            if(display_info.is_video_sub_option_active)//IF(第二级菜单已经激活)
            {
                switch(display_info.video_sub_option_focus) 
                {
                case MMIVT_2LEVEL_VIDEO_LCD_BRIGHT_OPTION:
                    //调节lcd 亮度
                    {
                        BOOLEAN is_lcd_bright_add = TRUE;
                        if (MSG_APP_LEFT == msg_id)
                        {
                            is_lcd_bright_add = FALSE;
                        }
                        UpdateLcdBright(win_id, is_lcd_bright_add);
                    }
            	    break;

                case MMIVT_2LEVEL_VIDEO_CONTRAST_OPTION:
                    //调节对比度参数
                    {
                        BOOLEAN is_contrast_add = TRUE;
                        if (MSG_APP_LEFT == msg_id)
                        {
                            is_contrast_add = FALSE;
                        }
                        UpdateContrast(win_id, is_contrast_add);
                    }
            	    break;
                    
                case MMIVT_2LEVEL_VIDEO_BRIGHT_OPTION:
                    //调节亮度参数
                    {
                        BOOLEAN is_brightness_add = TRUE;
                        if (MSG_APP_LEFT == msg_id)
                        {
                            is_brightness_add = FALSE;
                        }
                        UpdateBrightness(win_id, is_brightness_add);
                    }
            	    break;

                case MMIVT_2LEVEL_VIDEO_ROTATE_OPTION:
                    SwitchVideoRotateFocus(win_id, msg_id);
            	    break;
                    
                default:
                    //SCI_TRACE_LOW:"mmivt wrong focus %d"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_UI_3515_112_2_18_3_6_18_166,(uint8*)"d", display_info.video_sub_option_focus);
                    break;
                }
            }
            else//if(display_info.is_video_sub_option_active)//IF(第二级菜单已经激活)
            {
                SwitchVideoOptionFocus(win_id, msg_id);
            }
        }
        break;

    default:
        //do nothing
        break;
    }
    
    return result;
}
#endif
/*****************************************************************************/
// 	Description : HandleVTConnectedWinPhoneNumKey
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleVTConnectedWinPhoneNumKey(MMI_WIN_ID_T win_id,MMI_MESSAGE_ID_E msg_id)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;

    VT_DISPLAY_INFO_T display_info = MMIVT_GetAppletInstance()->vt_instance.ui_display_info;
    
    //SCI_TRACE_LOW:"mmivt HandleVTConnectedWinPhoneNumKey mode: %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_UI_3544_112_2_18_3_6_18_167,(uint8*)"d",display_info.option_bank_mode);
    switch(display_info.option_bank_mode) 
    {
    case MMIVT_OPTION_BANK_MODE_NORMAL:
        QuitMainOptionMode(win_id);
        EnterUserInputMode(win_id, msg_id);
    	break;

    case MMIVT_OPTION_BANK_MODE_VIDEO_OPTION:
        QuitVideoParamMode(win_id);
        QuitVideoOptionMode(win_id);
        EnterUserInputMode(win_id, msg_id);       
        break;
        
    case MMIVT_OPTION_BANK_MODE_VOLUME:
        QuitVolumeMode(win_id);
        EnterUserInputMode(win_id, msg_id);
        break;

    default:
        //do nothing
        break;
    }
    MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
    
    return result;
}



/*****************************************************************************/
// 	Description : HandleVTConnectedWinWebKey
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleVTConnectedWinWebKey(MMI_WIN_ID_T win_id)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;

    VT_DISPLAY_INFO_T display_info = MMIVT_GetAppletInstance()->vt_instance.ui_display_info;
    
    //SCI_TRACE_LOW:"mmivt HandleVTConnectedWinWebKey mode: %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_UI_3584_112_2_18_3_6_18_168,(uint8*)"d",display_info.option_bank_mode);
#ifdef TOUCH_PANEL_SUPPORT
    switch(display_info.option_bank_mode) 
    {
    case MMIVT_OPTION_BANK_MODE_NORMAL:
        QuitMainOptionMode(win_id);
        EnterUserInputMode(win_id, 0);
    	break;

    case MMIVT_OPTION_BANK_MODE_VIDEO_OPTION:
        QuitVideoParamMode(win_id);
        QuitVideoOptionMode(win_id);
        QuitMainOptionMode(win_id);
        EnterUserInputMode(win_id, 0);
        break;
        
    case MMIVT_OPTION_BANK_MODE_VOLUME:
        QuitVolumeMode(win_id);
        EnterUserInputMode(win_id, 0);
        break;

    default:
        //do nothing
        break;
    }
    MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
#else
    switch(display_info.option_bank_mode) 
    {
    case MMIVT_OPTION_BANK_MODE_NORMAL:
        {
            switch(display_info.option_focus) 
            {
            case MMIVT_1LEVEL_AUDIO_OPTION:
                UpdateAudioOption(win_id);
            	break;

            case MMIVT_1LEVEL_VIDEO_OPTION:
                QuitMainOptionMode(win_id);
                EnterVideoOptionMode(win_id);               
            	break;

            case MMIVT_1LEVEL_CAMERA_OPTION:
                {
                    MMIVT_REF_RET_E ret_code = UpdateCameraOption(win_id);
                    ErrorNotify(ret_code);
                }
                
            	break;

            case MMIVT_1LEVEL_DISPLAY_MODE_OPTION:
                UpdateDisplayModeOption();
            	break;
            default:
                //SCI_TRACE_LOW:"mmivt wrong focus %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_UI_3638_112_2_18_3_6_18_169,(uint8*)"d", display_info.option_focus);
                break;
            }
            MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
        }
        break;

    case MMIVT_OPTION_BANK_MODE_VIDEO_OPTION:
        if (display_info.is_video_sub_option_active)
        {
            if (MMIVT_2LEVEL_VIDEO_ROTATE_OPTION == display_info.video_sub_option_focus)
            {
                UpdateRotateOption(win_id, VT_IMAGE_ROTATE_90_DEGREE);
            }
            else
            {
                //do nothing
            }
        }
        else
        {
            QuitVideoOptionMode(win_id);
            EnterVideoParamMode(win_id, 0);
        }
        break;

    default:
        //do nothing
        break;
    }
#endif //touch_panel_support

    return result;
}

/*****************************************************************************/
// 	Description : HandleVTConnectedWinCancelKey
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleVTConnectedWinCancelKey(MMI_WIN_ID_T win_id)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;

    VT_DISPLAY_INFO_T display_info = MMIVT_GetAppletInstance()->vt_instance.ui_display_info;
    
    //SCI_TRACE_LOW:"mmivt HandleVTConnectedWinCancelKey mode: %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_UI_3683_112_2_18_3_6_18_170,(uint8*)"d",display_info.option_bank_mode);
    switch(display_info.option_bank_mode) 
    {
    case MMIVT_OPTION_BANK_MODE_NORMAL:
        if(!MMIAPIDM_GetHandsetLock()) //added by mary cr226858
        {
            {
                MMIVT_REF_RET_E ret_code = HandleEnableCamera(MMIVT_GetAppletInstance()->vt_instance.ui_display_info.is_replacing);
                ErrorNotify(ret_code);
            }
        
            SetConnectedWinSoftKey(win_id, TRUE);
         }

        break;

    case MMIVT_OPTION_BANK_MODE_VIDEO_OPTION:
        if(!MMIAPIDM_GetHandsetLock()) //added by mary cr226858
        {
            if (display_info.is_video_sub_option_active)
            {
                QuitVideoParamMode(win_id);
                EnterVideoOptionMode(win_id);
            }
            else
            {
                QuitVideoParamMode(win_id);
                QuitVideoOptionMode(win_id);
                EnterMainOptionMode(win_id);
            }
            MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
        }
        break;

    case MMIVT_OPTION_BANK_MODE_USERINPUT:
        if(!MMIAPIDM_GetHandsetLock()) //added by mary cr226858
        {
            QuitUserInputMode(win_id);
            EnterMainOptionMode(win_id);
            MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
        }
        break;

    case MMIVT_OPTION_BANK_MODE_VOLUME:
        QuitVolumeMode(win_id);
        EnterMainOptionMode(win_id);
        MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
        break;

    default:
        //do nothing
        break;
    }
    return result;
}

/*****************************************************************************/
// 	Description : HandleVTConnectedWinOkKey
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleVTConnectedWinOkKey(MMI_WIN_ID_T win_id)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;

    //开启menu窗口
    OpenConectedMenuWin(win_id);
    return result;
}

/*****************************************************************************/
// 	Description : HandleVTConnectedWinPenOk
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleVTConnectedWinPenOk(                              
                                              MMI_WIN_ID_T      win_id,         //in:
                                              MMI_CTRL_ID_T     button_ctrl_id  //in:button ctrl
                                            )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
#ifdef TOUCH_PANEL_SUPPORT
    VT_DISPLAY_INFO_T display_info = MMIVT_GetAppletInstance()->vt_instance.ui_display_info;
    switch(button_ctrl_id) 
    {
    case MMIVT_BUTTON_AUDIO_OPTION_CTRL_ID:
        UpdateAudioOption(win_id);
    	break;

    case MMIVT_BUTTON_VIDEO_OPTION_CTRL_ID:
        QuitMainOptionMode(win_id);
        EnterVideoOptionMode(win_id);
        MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
    	break;

    case MMIVT_BUTTON_CAMERA_OPTION_CTRL_ID:
        UpdateCameraOption(win_id);
    	break;

    case MMIVT_BUTTON_DISPMODE_OPTION_CTRL_ID:
        UpdateDisplayModeOption();
    	break;

    case MMIVT_BUTTON_DISPLAY_LCD_BRIGHTNESS_CTRL_ID:
    case MMIVT_BUTTON_DISPLAY_CONTRAST_CTRL_ID:
    case MMIVT_BUTTON_DISPLAY_BRIGHTNESS_CTRL_ID:
    case MMIVT_BUTTON_DISPLAY_ROTATE_CTRL_ID:
        QuitVideoOptionMode(win_id);
        EnterVideoParamMode(win_id, button_ctrl_id);
        break;

    case MMIVT_BUTTON_VIDEO_PARAM_ADD_CTRL_ID:
    case MMIVT_BUTTON_VIDEO_PARAM_MINUS_CTRL_ID:
        if(display_info.is_video_sub_option_active)//IF(第二级菜单已经激活)
        {
            BOOLEAN is_add = TRUE;
            if (MMIVT_BUTTON_VIDEO_PARAM_MINUS_CTRL_ID == button_ctrl_id)
            {
                is_add = FALSE;
            }
            switch(display_info.video_sub_option_focus) 
            {
            case MMIVT_2LEVEL_VIDEO_LCD_BRIGHT_OPTION:
                {
                    UpdateLcdBright(win_id, is_add);
                }
            	break;

            case MMIVT_2LEVEL_VIDEO_CONTRAST_OPTION:
                //调节对比度参数
                {
                    UpdateContrast(win_id, is_add);
                }
            	break;
                
            case MMIVT_2LEVEL_VIDEO_BRIGHT_OPTION:
                //调节亮度参数
                {
                    UpdateBrightness(win_id, is_add);
                }
            	break;
                
            default:
                //SCI_TRACE_LOW:"mmivt wrong focus %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_UI_3824_112_2_18_3_6_19_171,(uint8*)"d", display_info.video_sub_option_focus);
                break;
            }
        }        
        break;

    case MMIVT_BUTTON_ROTATE_90_CTRL_ID:
    case MMIVT_BUTTON_ROTATE_180_CTRL_ID:
        {
            MMIVT_IMAGE_ROTATE_MODE_E rotate_mode = VT_IMAGE_ROTATE_90_DEGREE;
            if (MMIVT_BUTTON_ROTATE_180_CTRL_ID == button_ctrl_id)
            {
                rotate_mode = VT_IMAGE_ROTATE_180_DEGREE;
            }
            UpdateRotateOption(win_id, rotate_mode);
        }
        break;

    case MMIVT_BUTTON_VOLUME_ADD_CTRL_ID:
        UpdateVolume(win_id, MSG_APP_UP);
        break;

    case MMIVT_BUTTON_VOLUME_MINUS_CTRL_ID:
        UpdateVolume(win_id, MSG_APP_DOWN);
        break;

    case MMIVT_BUTTON_DIAL_DEL_CTRL_ID:
        MMK_SendMsg(MMK_GetCtrlHandleByWin(win_id, MMIVT_EDIT_USERINPUT_CTRL_ID),MSG_APP_CANCEL,PNULL);
        break;
        
    case MMIVT_BUTTON_DIAL_1_CTRL_ID:
    case MMIVT_BUTTON_DIAL_2_CTRL_ID:
    case MMIVT_BUTTON_DIAL_3_CTRL_ID:
    case MMIVT_BUTTON_DIAL_4_CTRL_ID:
    case MMIVT_BUTTON_DIAL_5_CTRL_ID:
    case MMIVT_BUTTON_DIAL_6_CTRL_ID:
    case MMIVT_BUTTON_DIAL_7_CTRL_ID:
    case MMIVT_BUTTON_DIAL_8_CTRL_ID:
    case MMIVT_BUTTON_DIAL_9_CTRL_ID:
    case MMIVT_BUTTON_DIAL_0_CTRL_ID:
    case MMIVT_BUTTON_DIAL_STAR_CTRL_ID:
    case MMIVT_BUTTON_DIAL_HASH_CTRL_ID:
        {
            MMI_MESSAGE_ID_E keydown_msg_id = MSG_APP_1 + button_ctrl_id - MMIVT_BUTTON_DIAL_1_CTRL_ID;
            MMI_MESSAGE_ID_E keyup_msg_id = MSG_KEYUP_1 + button_ctrl_id - MMIVT_BUTTON_DIAL_1_CTRL_ID;
            MMI_HANDLE_T edit_ctrl_handle = MMK_GetCtrlHandleByWin(win_id, MMIVT_EDIT_USERINPUT_CTRL_ID);
            MMIAPIVT_SendUserInput(keydown_msg_id);

		    //handle key
		    MMK_SendMsg(edit_ctrl_handle,keydown_msg_id,PNULL);
            MMK_SendMsg(edit_ctrl_handle,keyup_msg_id,PNULL);
        }
        //send dtmf
        break;
    case MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID:
        MMK_SendMsg(win_id, MSG_APP_OK, PNULL);
        break;
    case MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID:
        MMK_SendMsg(win_id, MSG_APP_WEB, PNULL);
        break;
    case MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID:
        MMK_SendMsg(win_id, MSG_APP_CANCEL, PNULL);
        break;
        
    default:
        //SCI_TRACE_LOW:"mmivt HandleVTConnectedWinPenOk err button_id:%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_UI_3880_112_2_18_3_6_19_172,(uint8*)"d", button_ctrl_id);
        break;

    }
#endif //TOUCH_PANEL_SUPPORT
    return recode;
}

/*****************************************************************************/
// 	Description : the process message function of the Missed call application
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleConnectedMenuWinMsg( 
                                            MMI_WIN_ID_T win_id, 
                                            MMI_MESSAGE_ID_E msg_id, 
                                            DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    VT_DISPLAY_INFO_T display_info = MMIVT_GetAppletInstance()->vt_instance.ui_display_info;
    uint16 node_id = 0;
    GUI_RECT_T full_rect = {MAIN_PLAY_RECT_LEFT, MAIN_PLAY_RECT_TOP, MAIN_PLAY_RECT_RIGHT, MAIN_PLAY_RECT_BOTTOM};
    GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        LCD_FillRect(&lcd_dev_info, full_rect, MMI_BLACK_COLOR);
	MMITHEME_StoreUpdateRect(&lcd_dev_info, full_rect);
        //init dynamic menu
        InitConectedDynamicMenu(win_id);

        //disable gsensor rotate
        DisableGsensorRotate(win_id, TRUE);
        break;

    case MSG_APP_OK:
    case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        //fresh main play area
        {
            GUI_RECT_T full_rect = {MAIN_PLAY_RECT_LEFT, MAIN_PLAY_RECT_TOP, MAIN_PLAY_RECT_RIGHT, MAIN_PLAY_RECT_BOTTOM};
            GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
            LCD_FillRect(&lcd_dev_info, full_rect, MMI_WINDOW_BACKGROUND_COLOR);
        }
        node_id = GUIMENU_GetCurNodeId(MMK_GetCtrlHandleByWin(win_id, MMIVT_CONNECTED_POPMENU_CTRL_ID));
        switch(node_id) 
        {
        case CONNECTED_MENU_NODE_STOP_RECORD:
            StopRecord();
            MMIPUB_OpenAlertSuccessWin( TXT_COMMON_STOP_RECORD);            
        	break;
        case CONNECTED_MENU_NODE_STOP_REPLACE:
            StopLocalReplaceImage();
            MMIVT_GetAppletInstance()->vt_instance.ui_display_info.is_replacing = FALSE;
        	break;
        case CONNECTED_MENU_NODE_CAMERA:
            {
                MMIVT_REF_RET_E ret_code = HandleEnableCamera(display_info.is_replacing);
                ErrorNotify(ret_code);
            }            
            break;
        case CONNECTED_MENU_NODE_REMOTE_MUTE:
            {
                BOOLEAN is_remote_mute = MMICC_IsRemoteMute();
                MMICC_EnableRemoteMute(!is_remote_mute ?TRUE:FALSE);
            }
        	break;
        case CONNECTED_MENU_NODE_MUTE:
            {
                BOOLEAN is_mute = MMICC_IsMute();
                MMICC_EnableMute(!is_mute?TRUE:FALSE);
            }
        	break;
#ifdef BLUETOOTH_SUPPORT
        case CONNECTED_MENU_NODE_BT:
            {
                if (display_info.is_recording 
                    && ((MMIVT_RECORD_TYPE_RV_DA == display_info.record_type) || (MMIVT_RECORD_TYPE_DA == display_info.record_type)))
                {
                    MMIPUB_OpenAlertWarningWin(TXT_RECORD_CALL_ALERT);
                }
                else
                {
                    MMICC_AUDIO_OPER_E oper = MMICC_AUDIO_OPER_ENABLE;
                    if (MMICC_AUDIO_DEV_BT == MMICC_GetCurrentDev())
                    {
                        oper = MMICC_AUDIO_OPER_DISABLE;
                    }
                    else
                    {
                        oper = MMICC_AUDIO_OPER_ENABLE;
                    }
                    MMIAPICC_SwitchAudioDevice(oper, MMICC_AUDIO_DEV_BT);
                }
            }
        	break;
#endif
        case CONNECTED_MENU_NODE_REPLACE:
            StartLocalReplaceImage(FALSE);
        	break;
        case CONNECTED_MENU_NODE_RECORD_RV_DA:
            if(MMIAPIUDISK_UdiskIsRun())
            {
                MMIPUB_OpenAlertWinByTextId(PNULL,TXT_COMMON_UDISK_USING,TXT_COMMON_STOP_RECORD,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
            }
            else
            {
                MMIVT_REF_RET_E result = StartRecord(MMIVT_RECORD_TYPE_RV_DA);
                ErrorNotify(result);
            }           
        	break;
        case CONNECTED_MENU_NODE_RECORD_DA:
            if(MMIAPIUDISK_UdiskIsRun())
            {
                MMIPUB_OpenAlertWinByTextId(PNULL,TXT_COMMON_UDISK_USING,TXT_COMMON_STOP_RECORD,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
            }
            else
            {
                MMIVT_REF_RET_E result = StartRecord(MMIVT_RECORD_TYPE_DA);
                ErrorNotify(result);
            }   
        	break;
        case CONNECTED_MENU_NODE_RECORD_RV:
            if(MMIAPIUDISK_UdiskIsRun())
            {
                MMIPUB_OpenAlertWinByTextId(PNULL,TXT_COMMON_UDISK_USING,TXT_COMMON_STOP_RECORD,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
            }
            else
            {
                MMIVT_REF_RET_E result = StartRecord(MMIVT_RECORD_TYPE_RV);
                ErrorNotify(result);
            }   
            break;
        case CONNECTED_MENU_NODE_SMS:
            MMIAPISMS_OpenMainMenuWin();
        	break;
        case CONNECTED_MENU_NODE_PB:
            MMIAPIPB_OpenListAllGroupWin();
        	break;
#ifdef BROWSER_SUPPORT
        case CONNECTED_MENU_NODE_BROWSER:
            if (MMIAPIUDISK_UdiskIsRun()) //U盘使用中
            {
                MMIPUB_OpenAlertWarningWin(TXT_COMMON_UDISK_USING);
            }
            else
            {
                //fresh main display area
                SetMainPlayArea(win_id, FALSE);

                MMIAPICC_SwitchToBrowser();
            }
            MMK_CloseWin(win_id);
            break;
#endif
        case CONNECTED_MENU_NODE_SETTING:
            MMIAPISET_OpenVTSeting();
        	break;
        default:
            //do nothing
            break;
        }
        MMK_CloseWin(win_id);

        break;

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
    case MSG_APP_RED:
        MMK_CloseWin(win_id);
        break;


#if 0
    case MSG_GET_FOCUS:
        {
            GUI_RECT_T full_rect = {MAIN_PLAY_RECT_LEFT, MAIN_PLAY_RECT_TOP, MAIN_PLAY_RECT_RIGHT, MAIN_PLAY_RECT_BOTTOM};
            GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
            LCD_FillRect(&lcd_dev_info, full_rect, MMI_WINDOW_BACKGROUND_COLOR);
        }
        break;
#endif

    case MSG_CLOSE_WINDOW:
        //enable gsensor rotate
        DisableGsensorRotate(win_id, FALSE);
        break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    return recode;    
}

/*****************************************************************************/
//  Description : InitConectedDynamicMenu
//  Global resource dependence : 
//  Author:maggie.ren
//  Note:
/*****************************************************************************/
LOCAL void InitConectedDynamicMenu(MMI_HANDLE_T win_handle )
{
    uint32 index = 0;
    MMI_CTRL_ID_T menu_ctrl_id = MMIVT_CONNECTED_POPMENU_CTRL_ID;
    MMI_HANDLE_T menu_ctrl_handle = PNULL;
#ifdef BLUETOOTH_SUPPORT
    BT_ADDRESS      address = {0};
#endif
    MMI_TEXT_ID_T	label = TXT_NULL;
    VT_DISPLAY_INFO_T display_info = MMIVT_GetAppletInstance()->vt_instance.ui_display_info;
    
    GUIMENU_CreatDynamic(PNULL, win_handle, menu_ctrl_id, GUIMENU_STYLE_POPUP);
    menu_ctrl_handle = MMK_GetCtrlHandleByWin(win_handle, menu_ctrl_id);
    
    //停止录制
    if(display_info.is_recording)
    {
        label = TXT_COMMON_STOP_RECORD;
        InsertNodeByLabel(
            menu_ctrl_handle,//ctrl_id
            label,//text_ptr 
            CONNECTED_MENU_NODE_STOP_RECORD,//node_id
            0, //parent_node_id
            (uint16)index++//index
            ); 
    }
    //回复本地画面
//    if(display_info.is_replacing)
//    {
//        label = TXT_CONECTED_MENU_STOP_REPLACE;
//        InsertNodeByLabel(
//            menu_ctrl_handle,//ctrl_id
//            label,//text_ptr 
//            CONNECTED_MENU_NODE_STOP_REPLACE,//node_id
//            0, //parent_node_id
//            (uint16)index++//index
//            ); 
//    }
    //打开/关闭摄像头    
    {
        if(display_info.is_replacing)
        {
            label = TXT_CONECTED_MENU_TURNON_CAMERA;
        }
        else
        {
            label = TXT_CONECTED_MENU_TURNOFF_CAMERA;
        }
        
        InsertNodeByLabel(
            menu_ctrl_handle,//ctrl_id
            label,//text_ptr 
            CONNECTED_MENU_NODE_CAMERA,//node_id
            0, //parent_node_id
            (uint16)index++//index
            ); 
    }
    //打开/关闭远端静音
    {
        if (MMICC_IsRemoteMute())
        {
            label = TXT_CONECTED_MENU_TURNOFF_REMOTE_MUTE;
        }
        else
        {
            label = TXT_CONECTED_MENU_TURNON_REMOTE_MUTE;
        }
        
        InsertNodeByLabel(
            menu_ctrl_handle,//ctrl_id
            label,//text_ptr 
            CONNECTED_MENU_NODE_REMOTE_MUTE,//node_id
            0, //parent_node_id
            (uint16)index++//index
            ); 
    }
    //打开/关闭静音
    {
        if (MMICC_IsMute())
        {
            label = TXT_CONECTED_MENU_TURNOFF_MUTE;
        }
        else
        {
            label = TXT_CONECTED_MENU_TURNON_MUTE;
        }
        
        InsertNodeByLabel(
            menu_ctrl_handle,//ctrl_id
            label,//text_ptr 
            CONNECTED_MENU_NODE_MUTE,//node_id
            0, //parent_node_id
            (uint16)index++//index
            ); 
    }
#ifdef BLUETOOTH_SUPPORT
    //蓝牙通话
    if (MMIAPIBT_GetActivatedHS(&address))
    {
        if (MMICC_AUDIO_DEV_BT == MMICC_GetCurrentDev())
        {
            label = TXT_CONECTED_MENU_TURNOFF_BT;
        }
        else
        {
            label = TXT_CONECTED_MENU_TURNON_BT;
        }
        
        InsertNodeByLabel(
            menu_ctrl_handle,//ctrl_id
            label,//text_ptr 
            CONNECTED_MENU_NODE_BT,//node_id
            0, //parent_node_id
            (uint16)index++//index
            ); 
    }
#endif
    //替代画面
//    if (display_info.is_camera_on)//只有在摄像头开启的时候才能进行替代画面操作
//    {
//        label = TXT_CONECTED_MENU_REPLACE;
//        InsertNodeByLabel(
//            menu_ctrl_handle,//ctrl_id
//            label,//text_ptr 
//            CONNECTED_MENU_NODE_REPLACE,//node_id
//            0, //parent_node_id
//            (uint16)index++//index
//            );                              
//    }
    //录制
    if(!display_info.is_recording)
    {
        uint32 record_menu_index = 0;
        label = TXT_CONECTED_MENU_RECORD;
        InsertNodeByLabel(
            menu_ctrl_handle,//ctrl_id
            label,//text_ptr 
            CONNECTED_MENU_NODE_RECORD,//node_id
            0, //parent_node_id
            (uint16)index++//index
            ); 

        //子菜单项：远端视频和混音
        {
            label = TXT_CONECTED_MENU_RECORD_RV_DA;
            InsertNodeByLabel(
                menu_ctrl_handle,//ctrl_id
                label,//text_ptr 
                CONNECTED_MENU_NODE_RECORD_RV_DA,//node_id
                CONNECTED_MENU_NODE_RECORD, //parent_node_id
                (uint16)record_menu_index++//index
                ); 
        }
        //子菜单项：混音1
        {
            label = TXT_CONECTED_MENU_RECORD_DA;
            InsertNodeByLabel(
                menu_ctrl_handle,//ctrl_id
                label,//text_ptr 
                CONNECTED_MENU_NODE_RECORD_DA,//node_id
                CONNECTED_MENU_NODE_RECORD, //parent_node_id
                (uint16)record_menu_index++//index
                ); 
        }
        //子菜单项：远端视频
        {
            label = TXT_CONECTED_MENU_RECORD_RV;
            InsertNodeByLabel(
                menu_ctrl_handle,//ctrl_id
                label,//text_ptr 
                CONNECTED_MENU_NODE_RECORD_RV,//node_id
                CONNECTED_MENU_NODE_RECORD, //parent_node_id
                (uint16)record_menu_index++//index
                ); 
        }
    }
    //信息服务
    {
        label = TXT_COMMUNICATION_MESSAGE;
        InsertNodeByLabel(
            menu_ctrl_handle,//ctrl_id
            label,//text_ptr 
            CONNECTED_MENU_NODE_SMS,//node_id
            0, //parent_node_id
            (uint16)index++//index
            ); 
    }
    //通讯录
    {
        label = STXT_MAIN_PB;
        InsertNodeByLabel(
            menu_ctrl_handle,//ctrl_id
            label,//text_ptr 
            CONNECTED_MENU_NODE_PB,//node_id
            0, //parent_node_id
            (uint16)index++//index
            ); 
    }
    //浏览器
#ifdef BROWSER_SUPPORT
    {
        label = TXT_CC_TO_BROWSER;
        InsertNodeByLabel(
            menu_ctrl_handle,//ctrl_id
            label,//text_ptr 
            CONNECTED_MENU_NODE_BROWSER,//node_id
            0, //parent_node_id
            (uint16)index++//index
            ); 
    }
#endif
    //设置
    {
        label = STXT_MAIN_SETTING;
        InsertNodeByLabel(
            menu_ctrl_handle,//ctrl_id
            label,//text_ptr 
            CONNECTED_MENU_NODE_SETTING,//node_id
            0, //parent_node_id
            (uint16)index++//index
            ); 
    }
    MMK_SetAtvCtrl(win_handle, menu_ctrl_handle);    
}


/*****************************************************************************/
//  Description : insert dynamenu node by label
//  Global resource dependence : 
//  Author:maggie.ren
//  Note:
/*****************************************************************************/
LOCAL void InsertNodeByLabel(
                             MMI_CTRL_ID_T      ctrl_id,        //控件id
                             MMI_TEXT_ID_T      text_id,       //插入的节点的文本
                             uint16             node_id,        //节点编号
                             uint16             parent_node_id, //父节点编号
                             uint16             index           //位置
                             )
{
    MMI_STRING_T            kstring = {0};
    GUIMENU_DYNA_ITEM_T     node_item = {0};

    MMI_GetLabelTextByLang(text_id, &kstring);
    node_item.item_text_ptr = &kstring;
    GUIMENU_InsertNode(index, node_id, parent_node_id, &node_item, ctrl_id);
}  


/*****************************************************************************/
// 	Description : HandleDisconnectingWinMsg
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDisconnectingWinMsg( 
                                                MMI_WIN_ID_T win_id, 
                                                MMI_MESSAGE_ID_E msg_id, 
                                                DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            GUI_BOTH_RECT_T disconnecting_anim_both_rect = DISCONNECTING_ANIM_BOTH_RECT;
            GUI_BOTH_RECT_T disconnecting_bg_bank_both_rect = DISCONNECTING_BG_BANK_BOTH_RECT;
            GUI_BOTH_RECT_T disconnecting_name_label_both_rect = DISCONNECTING_NAME_LABEL_BOTH_RECT;
            GUI_BOTH_RECT_T disconnecting_state_label_both_rect = DISCONNECTING_STATE_LABEL_BOTH_RECT;

            GUIAPICTRL_SetBothRect(MMK_GetCtrlHandleByWin(win_id, MMIVT_LABEL_CALLSTATE_CTRL_ID), &disconnecting_state_label_both_rect);
            GUIAPICTRL_SetBothRect(MMK_GetCtrlHandleByWin(win_id, MMIVT_LABEL_CALLNAME_CTRL_ID), &disconnecting_name_label_both_rect);
            GUIAPICTRL_SetBothRect(MMK_GetCtrlHandleByWin(win_id, MMIVT_ANIM_CALL_DISCONNECTING_CTRL_ID), &disconnecting_anim_both_rect);
            GUIAPICTRL_SetBothRect(MMK_GetCtrlHandleByWin(win_id, MMIVT_ANIM_OPTION_BG_BANK_CTRL_ID), &disconnecting_bg_bank_both_rect);

        }
        //CLEAR LCD
        {
            FreshRect(win_id);
        }  
        HandleVTWinOpenWinMsg(win_id);
        {
            GUI_BG_T bg = {0};
            bg.bg_type = GUI_BG_IMG;
            bg.img_id = IMAG_VT_MAIN_PLAY_BG;
            bg.shape = GUI_SHAPE_RECT;
            DisplayAnim(win_id, MMIVT_ANIM_CALL_DISCONNECTING_CTRL_ID, ANIM_MO_CALL, PNULL, &bg); 
        }        
        DisplayCallStateLabel(win_id, FALSE);
        DisplayCallNameLabel(win_id);
        //disable gsensor rotate
        DisableGsensorRotate(win_id, TRUE);             
        break;

    case MSG_FULL_PAINT:
        {
            GUI_POINT_T point = {0, 0};
            GUI_RECT_T display_rect = {0, 0, MAIN_PLAY_RECT_RIGHT, MMI_STATUSBAR_HEIGHT};
            GUI_RECT_T image_rect = MMITHEME_GetFullScreenRect();
            GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
            GUIRES_DisplayImg(&point, &display_rect, &image_rect, win_id, IMAGE_COMMON_TITLE_BAR,&lcd_dev_info);
        }   
        break;

//bug339182
    case MSG_APP_LEFT:
    case MSG_APP_RIGHT:
    case MSG_APP_UP:
    case MSG_APP_DOWN:
		//这几个按键不需要响应
        break;

    case MSG_CLOSE_WINDOW:
        //disable gsensor rotate
        DisableGsensorRotate(win_id, FALSE);   
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    return recode;
}

/*****************************************************************************/
// 	Description : HandleFallbackQueryWin
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleFallbackQueryWin( 
                                                MMI_WIN_ID_T win_id, 
                                                MMI_MESSAGE_ID_E msg_id, 
                                                DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;

    switch(msg_id) 
    {
    case MSG_OPEN_WINDOW:
        //设置状态栏
        GUIWIN_CreateStbDyn(win_id);
        GUIWIN_SetStbBgImageId(win_id, IMAGE_STATUS_BAR);
        GUIWIN_SetTitleTextId( win_id, TXT_WARNING, FALSE );//bug337276
        recode = MMIPUB_HandleWaitWinMsg( win_id, msg_id, param );
    	break;

    case MSG_APP_OK:
    case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        //发起电话
        MMIVT_MakeFBCall();
        MMK_CloseWin(win_id);
        break;
        
    case MSG_APP_RED:
    case MSG_APP_CANCEL:       
        recode = MMIPUB_HandleWaitWinMsg( win_id, msg_id, param );
        if(IsAllowVtQuit())
        {
            MMK_CloseWin(win_id);
        }
    	break;

    case MSG_LOSE_FOCUS:
        recode = MMIPUB_HandleWaitWinMsg( win_id, msg_id, param );
        //MMK_CloseWin(win_id);
        break;

    case MSG_CLOSE_WINDOW:
        recode = MMIPUB_HandleWaitWinMsg( win_id, msg_id, param );
        QuitVTApp();
        break;
        
    default:
        recode = MMIPUB_HandleWaitWinMsg( win_id, msg_id, param );
        break;
    }

    return recode;
}

/*****************************************************************************/
// 	Description : HandleFallbackPromptWin
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleFallbackPromptWin( 
                                                MMI_WIN_ID_T win_id, 
                                                MMI_MESSAGE_ID_E msg_id, 
                                                DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    LOCAL uint8 s_fallback_prompt_win_close_timer = 0;

    switch(msg_id) 
    {
    case MSG_OPEN_WINDOW:
        //设置状态栏
        GUIWIN_CreateStbDyn(win_id);
        GUIWIN_SetStbBgImageId(win_id, IMAGE_STATUS_BAR);
        GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_NULL, TXT_COMMON_OK, FALSE);
        GUIWIN_SetTitleTextId( win_id, TXT_WARNING, FALSE );//bug337276
        recode = MMIPUB_HandleWaitWinMsg( win_id, msg_id, param );
        StartWinTimer(win_id, &s_fallback_prompt_win_close_timer, VT_THREE_SECONDS_TIMEOUT);
        break;

    case MSG_TIMER:
        if (s_fallback_prompt_win_close_timer == *(uint8*)param)
        {
            //发起电话
            MMIVT_MakeFBCall();
            MMK_CloseWin(win_id);
        }
        else
        {
            recode = MMI_RESULT_FALSE;
        }
        
        break; 

    case MSG_APP_RED:
        MMK_CloseWin(win_id);
        break;
        
    case MSG_APP_CANCEL:        
        if(IsAllowVtQuit())
        {
            //发起电话
            MMIVT_MakeFBCall();        
            MMK_CloseWin(win_id);
        }
    	break;

    case MSG_LOSE_FOCUS:
        recode = MMIPUB_HandleWaitWinMsg( win_id, msg_id, param );
        MMK_CloseWin(win_id);        
        break;

    case MSG_CLOSE_WINDOW:
        StopWinTimer(&s_fallback_prompt_win_close_timer);
        recode = MMIPUB_HandleWaitWinMsg( win_id, msg_id, param );
        QuitVTApp();
        break;
        
    default:
        recode = MMIPUB_HandleWaitWinMsg( win_id, msg_id, param );
        break;
    }

    return recode;    
}

/*****************************************************************************/
// 	Description : HandleFallbackQuitWin
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleFallbackQuitWin( 
                                                MMI_WIN_ID_T win_id, 
                                                MMI_MESSAGE_ID_E msg_id, 
                                                DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;  
    LOCAL uint8 s_fallback_quit_win_close_timer = 0;

    switch(msg_id) 
    {
    case MSG_OPEN_WINDOW:
        //设置状态栏
        GUIWIN_CreateStbDyn(win_id);
        GUIWIN_SetStbBgImageId(win_id, IMAGE_STATUS_BAR);
        GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_NULL, TXT_COMMON_OK, FALSE);
        GUIWIN_SetTitleTextId( win_id, TXT_WARNING, FALSE );//bug337276
        recode = MMIPUB_HandleWaitWinMsg( win_id, msg_id, param );
        StartWinTimer(win_id, &s_fallback_quit_win_close_timer, VT_THREE_SECONDS_TIMEOUT);
        break;

    case MSG_TIMER:
        if (s_fallback_quit_win_close_timer == *(uint8*)param)
        {
            MMK_CloseWin(win_id);
        }
        else
        {
            recode = MMI_RESULT_FALSE;
        }
        //recode = MMIPUB_HandleWaitWinMsg( win_id, msg_id, param );
        break; 

    case MSG_APP_RED:
    case MSG_APP_CANCEL:
        if(IsAllowVtQuit())
        {
            MMK_CloseWin(win_id);
        }
    	break;

    case MSG_LOSE_FOCUS:
        recode = MMIPUB_HandleWaitWinMsg( win_id, msg_id, param );
        MMK_CloseWin(win_id);
        break;

    case MSG_CLOSE_WINDOW:
        StopWinTimer(&s_fallback_quit_win_close_timer);
        recode = MMIPUB_HandleWaitWinMsg( win_id, msg_id, param );
        QuitVTApp();
        break;
        
    default:
        recode = MMIPUB_HandleWaitWinMsg( win_id, msg_id, param );
        break;
    }

    return recode;
}

/*****************************************************************************/
// 	Description : HandleFallbackWaitWin
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleFallbackWaitWin( 
                                                MMI_WIN_ID_T win_id, 
                                                MMI_MESSAGE_ID_E msg_id, 
                                                DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;  
    LOCAL uint8 s_fallback_wait_timer = 0;

    switch(msg_id) 
    {
    case MSG_OPEN_WINDOW:
        //设置状态栏
        GUIWIN_CreateStbDyn(win_id);
        GUIWIN_SetStbBgImageId(win_id, IMAGE_STATUS_BAR);
        GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_NULL, TXT_COMMON_OK, FALSE);
        GUIWIN_SetTitleTextId( win_id, TXT_WARNING, FALSE );//bug 336843
        recode = MMIPUB_HandleWaitWinMsg( win_id, msg_id, param );
        StartWinTimer(win_id, &s_fallback_wait_timer, VT_FALLBACK_WAIT_TIMEOUT);
        break;

    case MSG_TIMER:
        if (s_fallback_wait_timer == *(uint8*)param)
        {
            MMK_CloseWin(win_id);
        }
        else
        {
            recode = MMI_RESULT_FALSE;
        }
        
        break; 

    case MSG_LOSE_FOCUS:
        recode = MMIPUB_HandleWaitWinMsg( win_id, msg_id, param );
        MMK_CloseWin(win_id);
    	break;

    case MSG_APP_RED:
    case MSG_APP_CANCEL:
        if(IsAllowVtQuit())
        {
            MMK_CloseWin(win_id);
        }
    	break;

    case MSG_CLOSE_WINDOW:
        StopWinTimer(&s_fallback_wait_timer);
        recode = MMIPUB_HandleWaitWinMsg( win_id, msg_id, param );
        QuitVTApp();
        break;
        
    default:
        recode = MMIPUB_HandleWaitWinMsg( win_id, msg_id, param );
        break;
    }

    return recode;
}

/*****************************************************************************/
// 	Description : HandleDisconnectedShowWinMsg
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDisconnectedShowWinMsg( 
                                                MMI_WIN_ID_T win_id, 
                                                MMI_MESSAGE_ID_E msg_id, 
                                                DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    LOCAL uint8 s_vt_disc_show_win_close_timer = 0;
    MMI_CTRL_ID_T richtext_ctrl_id = MMIVT_RICHTEXT_SHOWTIME_CTRL_ID;
    GUI_BOTH_RECT_T  richtext_both_rect = {0};//{RT_DISCONNECTED_SHOWTIME_LEFT,RT_DISCONNECTED_SHOWTIME_TOP,RT_DISCONNECTED_SHOWTIME_RIGHT,RT_DISCONNECTED_SHOWTIME_BOTTOM};

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        //设置状态栏
        GUIWIN_CreateStbDyn(win_id);
        GUIWIN_SetStbBgImageId(win_id, IMAGE_STATUS_BAR);
        
        richtext_both_rect = MMITHEME_GetWinClientBothRect(win_id);
        if(0 == richtext_both_rect.v_rect.top)
        {
                richtext_both_rect.v_rect.top = MMI_STATUSBAR_HEIGHT;
        }
        GUIAPICTRL_SetBothRect(MMK_GetCtrlHandleByWin(win_id, richtext_ctrl_id), &richtext_both_rect);

        SetDisconnectedShowDetail(win_id, richtext_ctrl_id);
        StartWinTimer(win_id, &s_vt_disc_show_win_close_timer, VT_THREE_SECONDS_TIMEOUT);

        MMK_SetAtvCtrl(win_id, MMK_GetCtrlHandleByWin(win_id, richtext_ctrl_id));
        
        break;

    case MSG_FULL_PAINT:
        {
            GUI_POINT_T point = {0, 0};
            GUI_RECT_T display_rect = {0, 0, MAIN_PLAY_RECT_RIGHT, MMI_STATUSBAR_HEIGHT};
            GUI_RECT_T image_rect = MMITHEME_GetFullScreenRect();
            GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
            display_rect.right = image_rect.right;
            GUIRES_DisplayImg(&point, &display_rect, &image_rect, win_id, IMAGE_COMMON_TITLE_BAR,&lcd_dev_info);
        }
        break;

    case MSG_TIMER:
        if (s_vt_disc_show_win_close_timer == *(uint8*)param)
        {
            MMK_CloseWin(win_id);
        }
        else
        {
            recode = MMI_RESULT_FALSE;
        }
        break;

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
    case MSG_APP_RED:
        MMK_CloseWin(win_id);
        break;

    case MSG_LOSE_FOCUS:
        MMK_CloseWin(win_id);
        break;

    case MSG_CLOSE_WINDOW:
        StopWinTimer(&s_vt_disc_show_win_close_timer);
        QuitVTApp();
        break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    return recode;
}

/*****************************************************************************/
// 	Description : SetDisconnectedShowDetail
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
LOCAL void SetDisconnectedShowDetail(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id)
{
    MMI_HANDLE_T richtext_handle = NULL; //ctrl handle
    GUIRICHTEXT_ITEM_T  item_data = {0};/*lint !e64*/ //richtext item
    uint16 index = 0;   //richtext item index
    wchar photo_file_name[MMIFILE_FULL_PATH_MAX_LEN +1] = {0};  //big head photo file name
    uint32 photo_file_len = 0;  //big head photo file name len
    MN_CALLED_NUMBER_T called_number = {0}; //current call number
    BOOLEAN is_photo_file_exist = FALSE;    //custom big head photo is exist
    MMI_STRING_T call_name = {0};   // current call name
    wchar temp_name_str[MMIVT_MAX_PBNAME_LEN + 1] = {0};

    richtext_handle = MMK_GetCtrlHandleByWin(win_id, ctrl_id);
    
    //set space between items
    //GUIRICHTEXT_SetItemSpace(richtext_handle, (uint8) RICHTEXT_ITEM_SPACE);

   //big head photo

    item_data.img_frame_type = GUIRICHTEXT_FRAME_NORMAL;
    item_data.img_set_size = TRUE;
    item_data.img_setting_width = PHOTO_RECT_RIGHT - PHOTO_RECT_LEFT;
    item_data.img_setting_height = PHOTO_RECT_BOTTOM - PHOTO_RECT_TOP;

    MMIVT_GetCallBCDNumber(&called_number);
    if (GetBigHeadPhotoByCalledNum(&called_number, photo_file_name, &photo_file_len))
    {
        if (MMIAPIFMM_IsFileExist(
            (const uint16 *)photo_file_name,
            (uint16)(photo_file_len)
            ))
        {
            is_photo_file_exist = TRUE;
        }
    }

    if (is_photo_file_exist)
    {
        item_data.img_type = GUIRICHTEXT_IMAGE_FILE;
        item_data.img_data.file.filename_ptr = photo_file_name;
        item_data.img_data.file.filename_len = photo_file_len;
    }
    else
    {
        item_data.img_type = GUIRICHTEXT_IMAGE_RES;
        item_data.img_data.res.id = IMAG_VT_DEFAULT_PHOTO;
    }

    //call name
    call_name.wstr_ptr = temp_name_str;
    MMIAPIVT_GetCallName(&call_name);
    item_data.text_type = GUIRICHTEXT_TEXT_BUF;
    item_data.text_data.buf.len = call_name.wstr_len;
    item_data.text_data.buf.str_ptr = call_name.wstr_ptr;

    GUIRICHTEXT_AddItem ( richtext_handle, &item_data, &index);

    //call start time & date
    {
        uint8   time[VT_DISPLAY_TIME_LEN + 1 ] = {0};
        MMI_STRING_T    time_str = {0};
        wchar   time_wstr[VT_DISPLAY_TIME_LEN + 1] = {0};
        MMI_TM_T call_start_tm = MMIAPICOM_Second2Tm(MMIVT_GetCallStartTime());

        MMIAPISET_FormatTimeStrByTime(call_start_tm.tm_hour, call_start_tm.tm_min,time,MMIVT_MAX_PBNAME_LEN + 1);
        time_str.wstr_ptr = time_wstr;
        time_str.wstr_len = (uint16)strlen((char*)time);

        MMI_STRNTOWSTR( time_wstr, VT_DISPLAY_TIME_LEN, (uint8 *)time, time_str.wstr_len, time_str.wstr_len );
        
        SetHeadAndContentInRText(TXT_TIME, richtext_handle, time_str, &index);
    }

    //call duration
    {
        uint8   duration[VT_DISPLAY_TIME_LEN + 1 ] = {0};
        MMI_STRING_T    duration_str = {0};
        wchar   duration_wstr[VT_DISPLAY_TIME_LEN + 1] = {0};
        MMI_TM_T duration_tm = MMIAPICOM_Second2Tm(MMIVT_GetCallDuration());

        sprintf((char*)duration, "%02d:%02d:%02d", duration_tm.tm_hour, duration_tm.tm_min, duration_tm.tm_sec);
        duration_str.wstr_ptr  = duration_wstr;
        duration_str.wstr_len    = (uint16)strlen((char*)duration);
        MMI_STRNTOWSTR( duration_wstr, VT_DISPLAY_TIME_LEN, (uint8 *)duration, duration_str.wstr_len, duration_str.wstr_len );

        SetHeadAndContentInRText(TXT_CL_CALL_TIME, richtext_handle, duration_str, &index);

        
    }

}

/*****************************************************************************/
// 	Description : HandleDisconnectedCausePromptWin
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDisconnectedCausePromptWin( 
                                                MMI_WIN_ID_T win_id, 
                                                MMI_MESSAGE_ID_E msg_id, 
                                                DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    LOCAL uint8 s_disconnected_prompt_win_close_timer = 0;
    LOCAL BOOLEAN s_is_need_redial = FALSE;

    switch(msg_id) 
    {
    case MSG_OPEN_WINDOW:
        //设置状态栏
        GUIWIN_CreateStbDyn(win_id);
        GUIWIN_SetStbBgImageId(win_id, IMAGE_STATUS_BAR);
        GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_NULL, TXT_COMMON_OK, FALSE);
        GUIWIN_SetTitleTextId( win_id, TXT_WARNING, FALSE );//bug 336843
        recode = MMIPUB_HandleWaitWinMsg( win_id, msg_id, param );
        StartWinTimer(win_id, &s_disconnected_prompt_win_close_timer, VT_THREE_SECONDS_TIMEOUT);
        s_is_need_redial = FALSE;
        break;

    case MSG_TIMER:
        if (s_disconnected_prompt_win_close_timer == *(uint8*)param)
        {
            //处理自动重拨
            if (MMIAPISET_GetIsAutoRedail())
            {
                MMIVT_APPLET_T *vt_applet_ptr = MMIVT_GetAppletInstance();
                if (PNULL != vt_applet_ptr
                    && (vt_applet_ptr->vt_instance.redial_info.redialed_times < vt_applet_ptr->vt_instance.redial_info.redial_times_max))
                {
                    s_is_need_redial = TRUE;
                }
                else    
                {
                    s_is_need_redial = FALSE;
                }
            }
            MMK_CloseWin(win_id);
        }
        else
        {
            recode = MMI_RESULT_FALSE;
        }
        //recode = MMIPUB_HandleWaitWinMsg( win_id, msg_id, param );
        break; 

    case MSG_APP_RED:
    case MSG_APP_CANCEL:
        //发起电话
        recode = MMIPUB_HandleWaitWinMsg( win_id, msg_id, param );
        MMK_CloseWin(win_id);
    	break;

    case MSG_LOSE_FOCUS:
        recode = MMIPUB_HandleWaitWinMsg( win_id, msg_id, param );
        MMK_CloseWin(win_id);        
        break;

    case MSG_CLOSE_WINDOW:
        StopWinTimer(&s_disconnected_prompt_win_close_timer);
        recode = MMIPUB_HandleWaitWinMsg( win_id, msg_id, param );
        //处理自动重拨
        if (MMIAPISET_GetIsAutoRedail() && s_is_need_redial)
        {
            //发起自动重拨
            //MMIVT_HandleUIMsg(MMIVT_UI_MSG_REDIAL, PNULL);
            MMIVT_Redial();
        }
        else
        {
            //退出vt applet
            QuitVTApp();
        }
        s_is_need_redial = FALSE;
        break;
        
    default:
        recode = MMIPUB_HandleWaitWinMsg( win_id, msg_id, param );
        break;
    }

    return recode;      
}

/*****************************************************************************/
// 	Description : HandleRedialWinMsg
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleRedialWinMsg( 
                                            MMI_WIN_ID_T win_id, 
                                            MMI_MESSAGE_ID_E msg_id, 
                                            DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    LOCAL uint8 s_mo_local_net_ring_timer = 0;
    GUI_RECT_T      anim_rect1 = {OPTION_BG_BANK_RECT_LEFT, OPTION_BG_BANK_RECT_TOP, OPTION_BG_BANK_RECT_RIGHT, OPTION_BG_BANK_RECT_BOTTOM};
    GUI_RECT_T      anim_rect2 = {MAIN_PLAY_RECT_LEFT, MAIN_PLAY_RECT_TOP, MAIN_PLAY_RECT_RIGHT, MAIN_PLAY_RECT_BOTTOM};
    GUI_RECT_T      anim_rect3 = {PHOTO_RECT_LEFT, PHOTO_RECT_TOP, PHOTO_RECT_RIGHT, PHOTO_RECT_BOTTOM};
    GUI_RECT_T      button_rect1 = {AUDIO_OPTION_BUTTON_LEFT, AUDIO_OPTION_BUTTON_TOP, AUDIO_OPTION_BUTTON_RIGHT, AUDIO_OPTION_BUTTON_BOTTOM};
    GUI_RECT_T      button_rect2 = {DISPLAY_OPTION_BUTTON_LEFT, DISPLAY_OPTION_BUTTON_TOP, DISPLAY_OPTION_BUTTON_RIGHT, DISPLAY_OPTION_BUTTON_BOTTOM};
    GUI_RECT_T      button_rect3 = {CAMERA_OPTION_BUTTON_LEFT, CAMERA_OPTION_BUTTON_TOP, CAMERA_OPTION_BUTTON_RIGHT, CAMERA_OPTION_BUTTON_BOTTOM};
    GUI_RECT_T      button_rect4 = {DISPMODE_OPTION_BUTTON_LEFT, DISPMODE_OPTION_BUTTON_TOP, DISPMODE_OPTION_BUTTON_RIGHT, DISPMODE_OPTION_BUTTON_BOTTOM};

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
        	GUI_RECT_T lab_rect = {TWO_LINE_LABEL1_LEFT, TWO_LINE_LABEL1_TOP, TWO_LINE_LABEL1_RIGHT, TWO_LINE_LABEL1_BOTTOM};
			GUILABEL_SetRect(MMIVT_LABEL_CALLSTATE_CTRL_ID, &lab_rect, FALSE);
        }                
		{
        	GUI_RECT_T lab_rect = {TWO_LINE_LABEL2_LEFT, TWO_LINE_LABEL2_TOP, TWO_LINE_LABEL2_RIGHT, TWO_LINE_LABEL2_BOTTOM};
			GUILABEL_SetRect(MMIVT_LABEL_CALLNAME_CTRL_ID, &lab_rect, FALSE);
        }        
        GUIBUTTON_SetRect(MMIVT_BUTTON_AUDIO_OPTION_CTRL_ID,&button_rect1);
        GUIBUTTON_SetRect(MMIVT_BUTTON_VIDEO_OPTION_CTRL_ID,&button_rect2);
        GUIBUTTON_SetRect(MMIVT_BUTTON_CAMERA_OPTION_CTRL_ID,&button_rect3);
        GUIBUTTON_SetRect(MMIVT_BUTTON_DISPMODE_OPTION_CTRL_ID,&button_rect4);

        GUIANIM_SetCtrlRect(MMIVT_ANIM_OPTION_BG_BANK_CTRL_ID,&anim_rect1);
        GUIANIM_SetCtrlRect(MMIVT_ANIM_MO_CALLING_CTRL_ID,&anim_rect2);
        GUILABEL_SetRect(MMIVT_LABEL_PHOTO_CTRL_ID,&anim_rect3, FALSE);
        //CLEAR LCD
        {
            FreshRect(win_id);
        }        
        HandleVTWinOpenWinMsg(win_id);
        {
            GUI_BG_T bg = {0};
            bg.bg_type = GUI_BG_IMG;
            bg.img_id = IMAG_VT_MAIN_PLAY_BG;
            bg.shape = GUI_SHAPE_RECT;
            DisplayAnim(win_id, MMIVT_ANIM_MO_CALLING_CTRL_ID, ANIM_MO_CALL, PNULL, &bg); 
        }        

        //显示icon option
        EnterMainOptionMode(win_id);
        DisplayCallStateLabel(win_id, FALSE);
        DisplayCallNameLabel(win_id);

        GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_NULL, STXT_CANCEL, FALSE);

        //仅用于09b 10a支持applet获得焦点时，收到resume消息
        MMIDEFAULT_SetBackLight(TRUE);		//点亮背光
		MMIDEFAULT_AllowTurnOffBackLight(FALSE);	//背光常开    

         //给视频显示区域刷透明色
        SetSecondaryPlayArea(win_id, TRUE);
        
        //设置mmi block
        StartVTBlock();

        //disable gsensor rotate
        DisableGsensorRotate(win_id, TRUE);
        break;

    //仅用于09b 10a支持applet获得焦点时，收到resume消息
    case MSG_GET_FOCUS:
        MMIDEFAULT_SetBackLight(TRUE);		//点亮背光
        MMIDEFAULT_AllowTurnOffBackLight(FALSE);	//背光常开  
        FreshRect(win_id);//fresh lcd for cleaning residual image of last open win
        SetSecondaryPlayArea(win_id, TRUE);        
        StartVTBlock();
        break;

    case MSG_LOSE_FOCUS:
        SetSecondaryPlayArea(win_id, FALSE);
        StopVTBlock();
        break;

    case MSG_APP_CANCEL:
    case MSG_APP_RED:
    case MSG_KEYDOWN_HEADSET_BUTTON:
        //挂断电话
        MMIVT_HandleUIMsg(MMIVT_UI_MSG_HANGUP, PNULL);
        break;

    //case MSG_APP_OK:
    case MSG_APP_WEB:
        UpdateAudioOption(win_id);
        break;

    case MSG_VT_REMOTE_ALERTING_IND://收到对方开始振铃的消息
        //显示当前状态
        DisplayCallStateLabel(win_id, TRUE);
        //开启本地振铃音
        {
#if 1//OPEN_VT_CHANGE
#ifdef MMISRV_AUDIO_SUPPORT
            MMISRVAUD_PlayToneEx(MMISRVAUD_PRI_HIGH, MMISRVAUD_TYPE_TONE_GENERIC, VT_NT_RING_CON_ID, MMI_1SECONDS,MMISRVAUD_TONE_FREQ_DEFAULT);//bug337098
            //MMISRVAUD_PlayTone(MMISRVAUD_TYPE_TONE_GENERIC, VT_NT_RING_CON_ID, MMI_1SECONDS, 2000);
#else
            MMIAUDIO_PlayTone(MMIAUDIO_GENERIC_TONE, VT_NT_RING_CON_ID, MMI_1SECONDS, MMIAUDIO_TONE_DEFAULT_FREQ);
#endif
#else
            MMIAUDIO_PlayTone(MMIAUDIO_GENERIC_TONE, VT_NT_RING_CON_ID, MMI_1SECONDS, MMIAUDIO_TONE_DEFAULT_FREQ);
#endif
            StartWinTimer(win_id, &s_mo_local_net_ring_timer, VT_THREE_SECONDS_TIMEOUT);
        }
        break;

    case MSG_TIMER:
        if (s_mo_local_net_ring_timer == *(uint8*)param)
        {
            //振动
 #if 1//OPEN_VT_CHANGE
#ifdef MMISRV_AUDIO_SUPPORT
           MMISRVAUD_PlayToneEx(MMISRVAUD_PRI_HIGH, MMISRVAUD_TYPE_TONE_GENERIC, VT_NT_RING_CON_ID, MMI_1SECONDS,MMISRVAUD_TONE_FREQ_DEFAULT);//bug337098
           //MMISRVAUD_PlayTone(MMISRVAUD_TYPE_TONE_GENERIC, VT_NT_RING_CON_ID, MMI_1SECONDS, 2000);
#else
           MMIAUDIO_PlayTone(MMIAUDIO_GENERIC_TONE, VT_NT_RING_CON_ID, MMI_1SECONDS, MMIAUDIO_TONE_DEFAULT_FREQ);
#endif
#else
           MMIAUDIO_PlayTone(MMIAUDIO_GENERIC_TONE, VT_NT_RING_CON_ID, MMI_1SECONDS, MMIAUDIO_TONE_DEFAULT_FREQ);
#endif
            StartWinTimer(win_id, &s_mo_local_net_ring_timer, VT_THREE_SECONDS_TIMEOUT);                
        }
        else
        {
            recode = MMI_RESULT_FALSE;
        }
        break; 
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
        if (PNULL != param)
        {
            uint32 src_id = ((MMI_NOTIFY_T*)param)->src_id;

            if(MMIVT_BUTTON_AUDIO_OPTION_CTRL_ID == ((MMI_NOTIFY_T*)param)->src_id)
            {
                UpdateAudioOption(win_id);
                break;
            }
            
            switch (src_id)
            {
            case MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID:
                MMK_SendMsg(win_id, MSG_APP_OK, PNULL);
                break;
            case MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID:
                MMK_SendMsg(win_id, MSG_APP_CANCEL, PNULL);
                break;
            default:
                break;
            }
        }
        break;          
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

//bug339182
    case MSG_APP_LEFT:
    case MSG_APP_RIGHT:
    case MSG_APP_UP:
    case MSG_APP_DOWN:
		//这几个按键不需要响应
        break;

    case MSG_CLOSE_WINDOW:
        //关闭本地振铃音
        {
            StopWinTimer(&s_mo_local_net_ring_timer);
 #if 1//OPEN_VT_CHANGE
#ifdef MMISRV_AUDIO_SUPPORT
            MMISRVAUD_StopTone(MMISRVAUD_TYPE_TONE_GENERIC);
#else
            MMIAUDIO_StopTone(MMIAUDIO_GENERIC_TONE);
#endif
#else
            MMIAUDIO_StopTone(MMIAUDIO_GENERIC_TONE);
#endif
        }
        StopVTBlock();

        //enable gsensor rotate
        DisableGsensorRotate(win_id, FALSE);
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    return recode;
}

/*****************************************************************************/
// 	Description : DisplayImageIcon
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
LOCAL void DisplayImageIcon(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id, MMI_IMAGE_ID_T image_id, GUI_RECT_T* rect_ptr)
{
    GUIANIM_CTRL_INFO_T     ctrl_info = {0};
    GUIANIM_DATA_INFO_T     data_info = {0};
    GUIANIM_DISPLAY_INFO_T  display_info = {0};
    MMI_HANDLE_T            anim_handle = NULL;

    anim_handle = MMK_GetCtrlHandleByWin(win_id, ctrl_id);
    ctrl_info.is_ctrl_id     = TRUE;
    ctrl_info.ctrl_id        = anim_handle;
    data_info.img_id         = image_id;
    display_info.align_style = GUIANIM_ALIGN_HVMIDDLE;
    display_info.is_update = TRUE;
    GUIANIM_SetParam(&ctrl_info,&data_info,PNULL,&display_info);
    
    if (PNULL != rect_ptr)
    {
        GUIANIM_SetCtrlRect(anim_handle, rect_ptr);
    }

    return;
}

/*****************************************************************************/
// 	Description : DisplayLabelImage
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
LOCAL void DisplayLabelImage(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id, MMI_IMAGE_ID_T image_id)
{   
    MMI_HANDLE_T label_handle = MMK_GetCtrlHandleByWin(win_id, ctrl_id);
    if (IMAGE_NULL != image_id)
    {
        GUILABEL_SetIcon(label_handle, image_id);
    }
    
    GUILABEL_SetVisible(label_handle, TRUE, TRUE);//SetCtrlVisible(win_id, ctrl_id, TRUE);
    IGUICTRL_SetState(MMK_GetCtrlPtr(label_handle), GUICTRL_STATE_DISABLE_TP, TRUE);

    //MMK_SendMsg(label_handle, MSG_CTL_PAINT, PNULL);
}

/*****************************************************************************/
// 	Description : DisplayButtonImage
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
LOCAL void DisplayButtonImage(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id, GUI_BG_T* button_bg_ptr)
{   
    MMI_HANDLE_T button_handle = MMK_GetCtrlHandleByWin(win_id, ctrl_id);

    if (PNULL != button_bg_ptr)
    {
        GUIBUTTON_SetBg(button_handle, button_bg_ptr);
    }
    
    SetCtrlVisible(win_id, ctrl_id, TRUE);
    MMK_SendMsg(button_handle, MSG_CTL_PAINT, PNULL);
}

/*****************************************************************************/
// 	Description : SetCtrlVisible
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
LOCAL void SetCtrlVisible(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id, BOOLEAN is_visible)
{
    MMI_HANDLE_T ctrl_handle = MMK_GetCtrlHandleByWin(win_id, ctrl_id);
    IGUICTRL_SetState(MMK_GetCtrlPtr(ctrl_handle), GUICTRL_STATE_INVISIBLE | GUICTRL_STATE_DISABLE_TP, !is_visible?TRUE:FALSE);
}


/*****************************************************************************/
// 	Description : display anim pic
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
LOCAL void DisplayAnim(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id, MMI_ANIM_ID_T image_id, GUI_RECT_T* rect_ptr, GUI_BG_T* bg_ptr)
{
    GUIANIM_CTRL_INFO_T     ctrl_info = {0};
    GUIANIM_DATA_INFO_T     data_info = {0};
    GUIANIM_DISPLAY_INFO_T  display_info = {0};
    MMI_HANDLE_T            anim_handle = NULL;

    anim_handle = MMK_GetCtrlHandleByWin(win_id, ctrl_id);
    ctrl_info.is_ctrl_id = TRUE;
    ctrl_info.ctrl_id = anim_handle;
    data_info.anim_id = image_id;
    display_info.align_style = GUIANIM_ALIGN_HVMIDDLE;
    if(PNULL != bg_ptr)
    {
        display_info.bg = *bg_ptr;
    }
    
    GUIANIM_SetParam(&ctrl_info,&data_info,PNULL,&display_info);
    
    if (PNULL != rect_ptr)
    {
        GUIANIM_SetCtrlRect(anim_handle, rect_ptr);
    }

    return;
}

/*****************************************************************************/
// 	Description : SetMainPlayArea
//	Global resource dependence : 
//  Author:samboo.shen
//	Note:
/*****************************************************************************/
LOCAL void SetMainPlayArea(MMI_WIN_ID_T win_id, BOOLEAN is_transparent)
{
    GUI_RECT_T main_play_rect = {MAIN_PLAY_RECT_LEFT, MAIN_PLAY_RECT_TOP, MAIN_PLAY_RECT_RIGHT, MAIN_PLAY_RECT_BOTTOM};
    GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    CAF_COLOR_T color = is_transparent?VT_TRANSPARENT_COLOR:MMI_WINDOW_BACKGROUND_COLOR;

    if(MMITHEME_IsMainScreenLandscape())
    {
        main_play_rect.left = MAIN_PLAY_RECT_LEFT_H;
        main_play_rect.right = MAIN_PLAY_RECT_RIGHT_H;
        main_play_rect.top = MAIN_PLAY_RECT_TOP_H;
        main_play_rect.bottom = MAIN_PLAY_RECT_BOTTOM_H;
    }
    LCD_FillRect(&lcd_dev_info, main_play_rect, color);
}

/*****************************************************************************/
// 	Description : SetSecondaryPlayArea
//	Global resource dependence : 
//  Author:samboo.shen
//	Note:
/*****************************************************************************/
LOCAL void SetSecondaryPlayArea(MMI_WIN_ID_T win_id, BOOLEAN is_transparent)
{
    CAF_COLOR_T color = is_transparent?VT_TRANSPARENT_COLOR:MMI_WINDOW_BACKGROUND_COLOR;
    GUI_BG_T big_photo_bg = {GUI_BG_COLOR, GUI_SHAPE_RECT, IMAGE_NULL, 0, FALSE};
    big_photo_bg.color = color;
    GUILABEL_SetBg(MMK_GetCtrlHandleByWin(win_id, MMIVT_LABEL_PHOTO_CTRL_ID), &big_photo_bg);
    if (!is_transparent)
    {
        GUI_RECT_T secondary_play_rect = {SECONDARY_PLAY_RECT_LEFT, SECONDARY_PLAY_RECT_TOP, SECONDARY_PLAY_RECT_RIGHT, SECONDARY_PLAY_RECT_BOTTOM};
        GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};

        if(MMITHEME_IsMainScreenLandscape())
        {
            secondary_play_rect.left = SECONDARY_PLAY_RECT_LEFT_H;
            secondary_play_rect.right = SECONDARY_PLAY_RECT_RIGHT_H;
            secondary_play_rect.top = SECONDARY_PLAY_RECT_TOP_H;
            secondary_play_rect.bottom = SECONDARY_PLAY_RECT_BOTTOM_H;
        }
        LCD_FillRect(&lcd_dev_info, secondary_play_rect, color);
    }
}
/*****************************************************************************/
// 	Description : DisplayCallStateLabel
//	Global resource dependence : 
//  Author:samboo.shen
//	Note:
/*****************************************************************************/
LOCAL void DisplayCallStateLabel(MMI_WIN_ID_T win_id, BOOLEAN is_fresh)
{
    MMI_TEXT_ID_T text_id = TXT_NULL;
    MMIVT_KERNEL_STATE_E call_state = MMIVT_KERNEL_STATE_IDLE;
    MMIVT_CALL_TYPE_E call_type = MMIVT_CALL_TYPE_MO_CALL;
    MMI_HANDLE_T state_label_handle = MMK_GetCtrlHandleByWin(win_id, MMIVT_LABEL_CALLSTATE_CTRL_ID);

    call_state = MMIVT_GetCallState();
    call_type = MMIVT_GetCallType();

    switch(call_state) 
    {
    case MMIVT_KERNEL_STATE_PS_CONNECTING:
        if (MMIVT_CALL_TYPE_MO_CALL == call_type)
        {
            if (MMVT_KERNEL_SUBSTATE_REMOTE_ALERTING == MMIVT_GetSubState())
            {
                text_id = TXT_VIDEO_CALL_REMOTE_ALERTING;
            }
            else
            {
                text_id = TXT_VIDEO_CALL_OUT;
            } 
        }
    	break;

    case MMIVT_KERNEL_STATE_H324_CONNECTING:
        text_id = TXT_CONNECTING;//TXT_VIDEO_CALL_CONNECTING;
        break;

    case MMIVT_KERNEL_STATE_ALERTING:
        if (MMIVT_CALL_TYPE_MT_CALL == call_type)
        {
            text_id = TXT_VIDEO_CALL_IN;
        }
        break;

    case MMIVT_KERNEL_STATE_CONNECT_PENDING:
        if (MMIVT_CALL_TYPE_MT_CALL == call_type)
        {
            text_id = TXT_CONNECTING;//TXT_VIDEO_CALL_CONNECTING;
        }
        break;

    case MMIVT_KERNEL_STATE_DISCONNECTING:
        text_id = TXT_CC_RELEASING;//TXT_VIDEO_CALL_DISCONNECTING
        break;
    default:
        //do nothing
        break;
    }
    GUILABEL_SetTextById(state_label_handle, text_id, is_fresh);
}

/*****************************************************************************/
// 	Description : DisplayCallTimeLabel
//	Global resource dependence : 
//  Author:samboo.shen
//	Note:
/*****************************************************************************/
LOCAL void DisplayCallTimeLabel(MMI_WIN_ID_T win_id, BOOLEAN is_fresh)
{
    MMI_HANDLE_T time_label_handle = MMK_GetCtrlHandleByWin(win_id, MMIVT_LABEL_CALLTIME_CTRL_ID);
    MMI_STRING_T display_time_str = {0};
    wchar temp_wchar[VT_DISPLAY_TIME_LEN + 1] = {0};
    uint8 time_str[VT_DISPLAY_TIME_LEN] = {0};
    uint32 cur_time_count = MMIVT_GetAppletInstance()->vt_instance.ui_call_time_count;
    //GUI_BG_T time_label_bg = {GUI_BG_IMG, GUI_SHAPE_RECT,IMAGE_VT_TEXT_LABEL_BG, 0};

    display_time_str.wstr_ptr = temp_wchar;

    //multimedia ring is playing or when call just connected 
    if (MMVT_KERNEL_SUBSTATE_MMR == MMIVT_GetSubState()
        || 0 == cur_time_count)
    {
        MMI_TEXT_ID_T text_id = TXT_VIDEO_CALL_CONNECTING;
        GUILABEL_SetTextById(time_label_handle, text_id, is_fresh);
        return;
    }

    sprintf((char*)time_str,
        "%02d:%02d:%02d",
        (int)(cur_time_count / VT_HOUR_TO_SECOND),
        (int)((cur_time_count % VT_HOUR_TO_SECOND) / VT_MINUTE_TO_SECOND),
        (int)(cur_time_count % VT_MINUTE_TO_SECOND)
        );
    
    display_time_str.wstr_len = strlen((char*)time_str );
    MMI_STRNTOWSTR(display_time_str.wstr_ptr,
        display_time_str.wstr_len,
        (uint8 *)time_str,
        display_time_str.wstr_len,
        display_time_str.wstr_len);

    //GUILABEL_SetBg(time_label_handle, &time_label_bg);
    GUILABEL_SetText(time_label_handle, &display_time_str, is_fresh);
    
}

/*****************************************************************************/
// 	Description : DisplayCallNameLabel
//	Global resource dependence : 
//  Author:samboo.shen
//	Note:
/*****************************************************************************/
LOCAL void DisplayCallNameLabel(MMI_WIN_ID_T win_id)
{
    MMI_STRING_T call_name = {0};
    MMI_HANDLE_T name_label_handle = MMK_GetCtrlHandleByWin(win_id, MMIVT_LABEL_CALLNAME_CTRL_ID);
    MMIAPIVT_GetCallName(&call_name);
    if (0 == call_name.wstr_len)
    {
        GUILABEL_SetTextById(name_label_handle, (MMI_TEXT_ID_T)TXT_UNKNOW_NUM, FALSE);
    }
    else
    {
        GUILABEL_SetText(name_label_handle, &call_name, FALSE);
    }
    //GUILABEL_SetBg(name_label_handle, &name_label_bg);
}

/*****************************************************************************/
// 	Description : SetHeadAndContentInRText
//	Global resource dependence : none
//  Author: samboo.shen
//	Note:
/*****************************************************************************/
LOCAL void SetHeadAndContentInRText(MMI_TEXT_ID_T head_text_id,      //标题文字id
                               MMI_CTRL_ID_T ctrl_id,            //所在的控件容器
                               MMI_STRING_T content_str,         //显示内容
                               uint16   *index
                               )
{
    GUIRICHTEXT_ITEM_T item_data = {0};    /*lint !e64*/ 
    MMI_STRING_T string_prefix = {0};       //标题的内容
    MMI_STRING_T colon_string = {0};
    wchar colon_str[] = {':', 0};
    uint8 wstr_len = 0;
    wchar dst_string[GUIRICHTEXT_TEXT_MAX_LEN+1] = {0};

    if(PNULL == index)
    {
        //SCI_TRACE_LOW:"SetHeadAndContentInRText index is NULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_UI_5333_112_2_18_3_6_22_173,(uint8*)"");
        return;
    }

    if(0 == content_str.wstr_len)
    {
        return;
    }
    item_data.img_type = GUIRICHTEXT_IMAGE_NONE;
    item_data.text_type = GUIRICHTEXT_TEXT_BUF;
    
    /*display head*/
    //get content of the head
    MMI_GetLabelTextByLang(head_text_id, &string_prefix);
    wstr_len = MMIAPICOM_CopyStr2Buf(dst_string, &string_prefix, GUIRICHTEXT_TEXT_MAX_LEN);
    item_data.text_data.buf.len = wstr_len;

    //add ":"
    colon_string.wstr_ptr = (wchar *)colon_str;
    colon_string.wstr_len = 1;
    wstr_len = MMIAPICOM_CopyStr2Buf(dst_string + item_data.text_data.buf.len, &colon_string, GUIRICHTEXT_TEXT_MAX_LEN);
    item_data.text_data.buf.len += wstr_len;
    item_data.text_data.buf.str_ptr = dst_string;

    GUIRICHTEXT_AddItem(ctrl_id, &item_data, index);

    /*display content*/
    item_data.text_data.buf.len = content_str.wstr_len;
    item_data.text_data.buf.str_ptr = content_str.wstr_ptr;

    GUIRICHTEXT_AddItem(ctrl_id, &item_data, index);
}

/*****************************************************************************/
// 	Description : FreshRect
//	Global resource dependence : 
//  Author:samboo.shen
//	Note:
/*****************************************************************************/
LOCAL void FreshRect(MMI_WIN_ID_T win_id)
{
    GUI_RECT_T full_rect = MMITHEME_GetFullScreenRect();
    GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    LCD_FillRect(&lcd_dev_info, full_rect, MMI_BLACK_COLOR);
    /*{
        GUI_POINT_T point = {MAIN_PLAY_RECT_LEFT, MAIN_PLAY_RECT_TOP};
        GUI_RECT_T display_rect = {MAIN_PLAY_RECT_LEFT, MAIN_PLAY_RECT_TOP, MAIN_PLAY_RECT_RIGHT, MAIN_PLAY_RECT_BOTTOM};
        GUI_RECT_T image_rect = {0, 0, (MAIN_PLAY_RECT_RIGHT - MAIN_PLAY_RECT_LEFT), (MAIN_PLAY_RECT_BOTTOM - MAIN_PLAY_RECT_TOP)};
        GUIRES_DisplayImg(&point, &display_rect, &image_rect, win_id, IMAG_VT_MAIN_PLAY_BG,&lcd_dev_info);
    }*/
    
}

/*****************************************************************************/
// 	Description : HandleVTWinOpenWinMsg
//	Global resource dependence : 
//  Author:samboo.shen
//	Note:
/*****************************************************************************/
LOCAL void HandleVTWinOpenWinMsg(MMI_WIN_ID_T win_id)
{

    //设置状态栏
    GUIWIN_CreateStbDyn(win_id); 
    //GUIWIN_SetStbBgImageId(win_id, IMAGE_COMMON_BG);
    DisplayImageIcon(win_id, MMIVT_ANIM_OPTION_BG_BANK_CTRL_ID, IMAG_VT_BG_BANK, PNULL);
}


/*****************************************************************************/
// 	Description : StartWinTimer
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: timer_id [IN] & [OUT]
/*****************************************************************************/
LOCAL void StartWinTimer(MMI_WIN_ID_T win_id, uint8* timer_id, uint32 time_out)
{
    
    if (0 != *timer_id)
    {
        MMK_StopTimer(*timer_id);
        *timer_id = 0;        
    }
    *timer_id = MMK_CreateWinTimer( win_id, time_out, FALSE);
}

/*****************************************************************************/
// 	Description : StopWinTimer
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
LOCAL void StopWinTimer(uint8* timer_id)
{
    
    if (0 != *timer_id)
    {
        MMK_StopTimer(*timer_id);
        *timer_id = 0;        
    }
}


/*****************************************************************************/
// 	Description : GetBigHeadPhotoByBcdNum
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: call_number_ptr [in]
//        photo_name_ptr [out]
//        photo_len [out] 
/*****************************************************************************/
LOCAL BOOLEAN GetBigHeadPhotoByCalledNum(MN_CALLED_NUMBER_T *call_number_ptr, 
                                        wchar* photo_name_ptr, 
                                        uint32* photo_len_ptr)
{
    MMIPB_BCD_NUMBER_T pb_num = {0};
    uint32 photo_size = 0;
    
    if(PNULL == call_number_ptr ||PNULL == photo_name_ptr ||PNULL == photo_len_ptr)
    {
        //SCI_TRACE_LOW:"GetBigHeadPhotoByCalledNum call_number_ptr=%d, photo_name_ptr =%d, photo_len_ptr =%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_UI_5444_112_2_18_3_6_22_174,(uint8*)"ddd",call_number_ptr, photo_name_ptr, photo_len_ptr);
        return;
    }

    pb_num.npi_ton = MMIAPICOM_ChangeTypePlanToUint8(call_number_ptr->number_type, MN_NUM_PLAN_UNKNOW);
    pb_num.number_len = MIN(MN_MAX_ADDR_BCD_LEN, call_number_ptr->num_len);
    MMI_MEMCPY(
        pb_num.number,
        sizeof(pb_num.number),
        (void*)call_number_ptr->party_num,
        sizeof(call_number_ptr->party_num),
        pb_num.number_len
        );	
    
    return MMIAPIPB_GetCustomPhotoPathByNumber(&pb_num,photo_name_ptr, photo_len_ptr, &photo_size);
}

#if 1//ndef TOUCH_PANEL_SUPPORT
/*****************************************************************************/
// 	Description : SwitchVideoRotateFocus
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
LOCAL void SwitchVideoRotateFocus(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id)
{ 
    MMIVT_IMAGE_ROTATE_MODE_E rotate_mode = MMIVT_GetAppletInstance()->vt_instance.ui_display_info.image_rotate_mode;
    
    //SCI_TRACE_LOW:"mmivt SwitchVideoRotateFocus"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_UI_5470_112_2_18_3_6_22_175,(uint8*)"");
    
    if (VT_IMAGE_ROTATE_90_DEGREE == rotate_mode)
    {
        rotate_mode = VT_IMAGE_ROTATE_180_DEGREE;
    }
    else
    {
        rotate_mode = VT_IMAGE_ROTATE_90_DEGREE;
    }

    MMIVT_GetAppletInstance()->vt_instance.ui_display_info.image_rotate_mode = rotate_mode;

    DisplayVideoParam(win_id, MMIVT_2LEVEL_VIDEO_ROTATE_OPTION);

    //开启界面关闭定时器
    StartWinTimer(win_id, &s_vt_video_param_timer, VT_FIVE_SECONDS_TIMEOUT);
    
}

/*****************************************************************************/
// 	Description : SwitchVideoOptionFocus
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
LOCAL void SwitchVideoOptionFocus(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id)
{
    int8 sub_option_focus = (int8)MMIVT_GetAppletInstance()->vt_instance.ui_display_info.video_sub_option_focus;
    
    //SCI_TRACE_LOW:"mmivt SwitchVideoOptionFocus focus:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_UI_5498_112_2_18_3_6_22_176,(uint8*)"d", sub_option_focus);

    if (MSG_APP_RIGHT == msg_id)
    {
        if (sub_option_focus >= MMIVT_2LEVEL_VIDEO_OPTION_MAX - 1)
        {
            sub_option_focus = MMIVT_2LEVEL_VIDEO_LCD_BRIGHT_OPTION;
        }
        else
        {
            sub_option_focus ++;   
        }
    }
    else if (MSG_APP_LEFT == msg_id)
    {
        if (sub_option_focus <= 0 )
        {
            sub_option_focus = MMIVT_2LEVEL_VIDEO_OPTION_MAX - 1;
        }
        else
        {
            sub_option_focus --;
        }
        
    }
    MMIVT_GetAppletInstance()->vt_instance.ui_display_info.video_sub_option_focus = (MMIVT_2LEVEL_VIDEO_OPTION_E)sub_option_focus;

    DisplayVideoOption(win_id);

}

/*****************************************************************************/
// 	Description : SwitchMainOptionFocus
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
LOCAL void SwitchMainOptionFocus(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id)
{
    int8 option_focus = (int8)MMIVT_GetAppletInstance()->vt_instance.ui_display_info.option_focus;
    
    //SCI_TRACE_LOW:"mmivt SwitchMainOptionFocus focus:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_UI_5537_112_2_18_3_6_22_177,(uint8*)"d", option_focus);

    if (MSG_APP_RIGHT == msg_id)
    {
        if (option_focus >= MMIVT_1LEVEL_OPTION_FOCUS_MAX - 1)
        {
            option_focus = MMIVT_1LEVEL_AUDIO_OPTION;
        }
        else
        {
            option_focus ++;
        }
    }
    else if (MSG_APP_LEFT == msg_id)
    {
        if (option_focus <= 0 )
        {
            option_focus = MMIVT_1LEVEL_OPTION_FOCUS_MAX - 1;
        }
        else
        {
            option_focus --;
        }    
    }

    MMIVT_GetAppletInstance()->vt_instance.ui_display_info.option_focus = (MMIVT_1LEVEL_OPTION_E)option_focus;

    DisplayMainOption(win_id);
}
#endif

/*****************************************************************************/
// 	Description : UpdateLcdBright
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
LOCAL void UpdateLcdBright(MMI_WIN_ID_T win_id, BOOLEAN is_lcd_brightness_add)
{
    MMIVT_VIDEO_LEVEL_E lcd_bright_level = MMIVT_GetAppletInstance()->vt_instance.ui_display_info.lcd_bright_level;

    //SCI_TRACE_LOW:"mmivt UpdateLcdBright %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_UI_5576_112_2_18_3_6_22_178,(uint8*)"d", lcd_bright_level);

    if (is_lcd_brightness_add)
    {
        if (lcd_bright_level >= MMIVT_VIDEO_LEVEL_SEVEN)
        {
            lcd_bright_level = MMIVT_VIDEO_LEVEL_SEVEN;
        }
        else
        {
            lcd_bright_level++;
        }
               
    }
    else
    {
        if (lcd_bright_level <= MMIVT_VIDEO_LEVEL_ONE)
        {
            lcd_bright_level = MMIVT_VIDEO_LEVEL_ONE;
        }
        else
        {
            lcd_bright_level--;
        }   
    }     

    MMIVT_REF_SetLcdBrightness(lcd_bright_level);

    MMIVT_GetAppletInstance()->vt_instance.ui_display_info.lcd_bright_level = lcd_bright_level;

    //显示当前参数等级
    DisplayVideoParam(win_id, MMIVT_2LEVEL_VIDEO_LCD_BRIGHT_OPTION);

    //开启界面关闭定时器
    StartWinTimer(win_id, &s_vt_video_param_timer, VT_FIVE_SECONDS_TIMEOUT);
}

/*****************************************************************************/
// 	Description : UpdateZoom
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
LOCAL void UpdateZoom(MMI_WIN_ID_T win_id, BOOLEAN is_zoom_out)
{
    MMIVT_VIDEO_LEVEL_E zoom_level = MMIVT_GetAppletInstance()->vt_instance.ui_display_info.zoom_level;

    //SCI_TRACE_LOW:"mmivt UpdateZoom level:%d zoom_out:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_UI_5621_112_2_18_3_6_22_179,(uint8*)"dd", zoom_level, is_zoom_out);

    if (is_zoom_out)
    {
        if (zoom_level >= MMIVT_VIDEO_LEVEL_SEVEN)
        {
            zoom_level = MMIVT_VIDEO_LEVEL_SEVEN;
        }
        else
        {
            zoom_level++;
        }
               
    }
    else
    {
        if (zoom_level <= MMIVT_VIDEO_LEVEL_ONE)
        {
            zoom_level = MMIVT_VIDEO_LEVEL_ONE;
        }
        else
        {
            zoom_level--;
        }   
    }

    //MMIVT_REF_SetZoom(zoom_level);

    MMIVT_GetAppletInstance()->vt_instance.ui_display_info.zoom_level = zoom_level;

    //显示当前参数等级
    //DisplayVideoParam(win_id);
}

/*****************************************************************************/
// 	Description : UpdateContrast
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
LOCAL void UpdateContrast(MMI_WIN_ID_T win_id, BOOLEAN is_contrast_add)
{
    MMIVT_VIDEO_LEVEL_E contrast_level = MMIVT_GetAppletInstance()->vt_instance.ui_display_info.contrast_level;

    //SCI_TRACE_LOW:"mmivt UpdateContrast level:%d contrast_add:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_UI_5663_112_2_18_3_6_22_180,(uint8*)"dd", contrast_level, is_contrast_add);
    if (is_contrast_add)
    {
        if (contrast_level >= MMIVT_VIDEO_LEVEL_SEVEN)
        {
            contrast_level = MMIVT_VIDEO_LEVEL_SEVEN;
        }
        else
        {
            contrast_level++; 
        }                 
    }
    else
    {
        if (contrast_level <= MMIVT_VIDEO_LEVEL_ONE)
        {
            contrast_level = MMIVT_VIDEO_LEVEL_ONE;
        }
        else
        {
            contrast_level--;
        }
        
    }

    MMIVT_REF_SetContrast(contrast_level);

    MMIVT_GetAppletInstance()->vt_instance.ui_display_info.contrast_level = contrast_level;

    //显示当前参数等级
    DisplayVideoParam(win_id, MMIVT_2LEVEL_VIDEO_CONTRAST_OPTION);

    //开启界面关闭定时器
    StartWinTimer(win_id, &s_vt_video_param_timer, VT_FIVE_SECONDS_TIMEOUT);
}

/*****************************************************************************/
// 	Description : UpdateBrightness
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
LOCAL void UpdateBrightness(MMI_WIN_ID_T win_id, BOOLEAN is_brightness_add)
{
    MMIVT_VIDEO_LEVEL_E brightness_level = MMIVT_GetAppletInstance()->vt_instance.ui_display_info.brightness_level;

    //SCI_TRACE_LOW:"mmivt UpdateBrightness level:%d brightness_add:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_UI_5707_112_2_18_3_6_23_181,(uint8*)"dd", brightness_level, is_brightness_add);
    if (is_brightness_add)
    {
        if (brightness_level >= MMIVT_VIDEO_LEVEL_SEVEN)
        {
            brightness_level = MMIVT_VIDEO_LEVEL_SEVEN;
        }
        else
        {
            brightness_level++;
        }
    }
    else
    {

        if (brightness_level <= MMIVT_VIDEO_LEVEL_ONE)
        {
            brightness_level = MMIVT_VIDEO_LEVEL_ONE;
        }
        else
        {
            brightness_level--;
        }
    }
    MMIVT_REF_SetBrightness(brightness_level);

    MMIVT_GetAppletInstance()->vt_instance.ui_display_info.brightness_level = brightness_level;

    //显示当前参数等级
    DisplayVideoParam(win_id, MMIVT_2LEVEL_VIDEO_BRIGHT_OPTION);

    //开启界面关闭定时器
    StartWinTimer(win_id, &s_vt_video_param_timer, VT_FIVE_SECONDS_TIMEOUT);
}

/*****************************************************************************/
// 	Description : UpdateRotateOption
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: rotate_mode仅用于触摸屏
/*****************************************************************************/
LOCAL void UpdateRotateOption(MMI_WIN_ID_T win_id, MMIVT_IMAGE_ROTATE_MODE_E mode)
{
    MMIVT_REF_RET_E ret_code = MMIVT_REF_RET_SUCCESS;
    MMIVT_DISP_MODE_E disp_mode = MMIVT_GetAppletInstance()->vt_instance.ui_display_info.display_mode;
    MMIVT_IMAGE_ROTATE_MODE_E rotate_mode = MMIVT_GetAppletInstance()->vt_instance.ui_display_info.image_rotate_mode;
    MMIVT_IMAGE_ROTATE_STATUS_E rotate_status = MMIVT_GetAppletInstance()->vt_instance.ui_display_info.image_local_rotate_status;

#ifdef TOUCH_PANEL_SUPPORT
    rotate_mode = mode;
#endif

    //SCI_TRACE_LOW:"mmivt UpdateRotateOption mode:%d, status:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_UI_5757_112_2_18_3_6_23_182,(uint8*)"dd", rotate_mode, rotate_status);

    rotate_status = (rotate_status + rotate_mode + 1)%VT_IMAGE_STATUS_ROTATE_MAX;

    ret_code = MMIVT_REF_RotateLocal(rotate_status, disp_mode);

    if (MMIVT_REF_RET_SUCCESS == ret_code)
    {
        MMIVT_GetAppletInstance()->vt_instance.ui_display_info.image_local_rotate_status = rotate_status;
    }

    //开启界面关闭定时器
    StartWinTimer(win_id, &s_vt_video_param_timer, VT_FIVE_SECONDS_TIMEOUT);
}

/*****************************************************************************/
// 	Description : UpdateVolume
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: volume超出范围的判断和处理在MMIAPICC_SetCallVolume中进行处理
/*****************************************************************************/
LOCAL void UpdateVolume(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id)
{
    uint8 volume = MMIAPICC_GetCallVolume();

    if ((MSG_APP_UP == msg_id) || (MSG_APP_UPSIDE == msg_id))
    {
        volume++;
    }
    else if ((MSG_APP_DOWN == msg_id) || (MSG_APP_DOWNSIDE == msg_id))
    {
        volume--;
    }
    
    MMIAPICC_SetCallVolume(volume);

    DisplayVolume(win_id);

    StartWinTimer(win_id, &s_vt_volume_close_timer, VT_THREE_SECONDS_TIMEOUT);
}

/*****************************************************************************/
// 	Description : UpdateAudioOption
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E UpdateAudioOption(MMI_WIN_ID_T win_id)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    MMICC_AUDIO_DEV_E audio_dev = MMICC_AUDIO_DEV_HH;
    BOOLEAN is_handfree_show = MMIVT_GetAppletInstance()->vt_instance.ui_display_info.is_handfree_show;

    //SCI_TRACE_LOW:"mmivt UpdateAudioOption handfree_show:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_UI_5806_112_2_18_3_6_23_183,(uint8*)"d", is_handfree_show);

    if (is_handfree_show && (MMICC_AUDIO_DEV_HF != MMICC_GetCurrentDev()))
    {
        audio_dev = MMICC_AUDIO_DEV_HF;
    }

    MMIAPICC_SwitchAudioDevice(MMICC_AUDIO_OPER_ENABLE, audio_dev);

    if (MMICC_AUDIO_DEV_HF == MMICC_GetCurrentDev())
    {
        MMIVT_GetAppletInstance()->vt_instance.ui_display_info.is_handfree_show = FALSE;
    }
    else
    {
        MMIVT_GetAppletInstance()->vt_instance.ui_display_info.is_handfree_show = TRUE;
    }

    DisplayMainOption(win_id);

    return result;

}

/*****************************************************************************/
// 	Description : UpdateCameraOption
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
LOCAL MMIVT_REF_RET_E UpdateCameraOption(MMI_WIN_ID_T win_id)
{
    MMIVT_REF_RET_E ret_code = MMIVT_REF_RET_SUCCESS;
    MMIVT_CAMERA_ID_E camera_id = MMIVT_GetAppletInstance()->vt_instance.ui_display_info.camera_id;

    //SCI_TRACE_LOW:"mmivt UpdateCameraOption id:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_UI_5839_112_2_18_3_6_23_184,(uint8*)"d", camera_id);

    if (MMIVT_GetAppletInstance()->vt_instance.ui_display_info.is_replacing)
    {
        //SCI_TRACE_LOW:"mmivt UpdateCameraOption replacing not switch camera"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_UI_5843_112_2_18_3_6_23_185,(uint8*)"");
        return MMIVT_REF_RET_FAIL;
    }
    
    if (MMIVT_CAMERA_ID_MAIN == camera_id)
    {
        camera_id = MMIVT_CAMERA_ID_SECONDARY;
    }
    else
    {
        camera_id = MMIVT_CAMERA_ID_MAIN;
    }

    ret_code = MMIVT_REF_SwitchCamera(camera_id);

    if (MMIVT_REF_RET_SUCCESS == ret_code)
    {
        MMIVT_GetAppletInstance()->vt_instance.ui_display_info.camera_id = camera_id;

        DisplayMainOption(win_id);
    }

    return ret_code;
}

/*****************************************************************************/
// 	Description : UpdateDisplayModeOption
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
LOCAL void UpdateDisplayModeOption(void)
{
    MMIVT_DISP_MODE_E display_mode = MMIVT_GetAppletInstance()->vt_instance.ui_display_info.display_mode;
    MMIVT_IMAGE_ROTATE_STATUS_E rotate_status = MMIVT_GetAppletInstance()->vt_instance.ui_display_info.image_local_rotate_status;

    //SCI_TRACE_LOW:"mmivt UpdateDisplayModeOption mode:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_UI_5877_112_2_18_3_6_23_186,(uint8*)"d", display_mode);

    switch(display_mode) 
    {
    case MMIVT_DISP_MODE_REMOTE_LOCAL:
        display_mode = MMIVT_DISP_MODE_LOCAL_REMOTE;
    	break;

    case MMIVT_DISP_MODE_LOCAL_REMOTE:
        display_mode = MMIVT_DISP_MODE_REMOTE_LOCAL;
    	break;

//    case MMIVT_DISP_MODE_REMOTE_ONLY:
//        display_mode = MMIVT_DISP_MODE_LOCAL_ONLY;
//    	break;
//
//    case MMIVT_DISP_MODE_LOCAL_ONLY:
//        display_mode = MMIVT_DISP_MODE_REMOTE_LOCAL;
//    	break;
    default:
        display_mode = MMIVT_DISP_MODE_REMOTE_LOCAL;
        break;
    }
    MMIVT_REF_SetDispArea(rotate_status, display_mode);

    MMIVT_GetAppletInstance()->vt_instance.ui_display_info.display_mode = display_mode;

    //DisplayOptionIconArea(win_id);

    //MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
}

/*****************************************************************************/
// 	Description : EnterMainOptionMode
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
LOCAL void EnterMainOptionMode(MMI_WIN_ID_T win_id)
{
    //SCI_TRACE_LOW:"mmivt EnterMainOptionMode"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_UI_5915_112_2_18_3_6_23_187,(uint8*)"");

    MMIVT_GetAppletInstance()->vt_instance.ui_display_info.option_bank_mode = MMIVT_OPTION_BANK_MODE_NORMAL;

    DisplayMainOption(win_id);

    SetConnectedWinSoftKey(win_id, FALSE);
}

/*****************************************************************************/
// 	Description : QuitMainOptionMode 
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
LOCAL void QuitMainOptionMode(MMI_WIN_ID_T win_id)
{
    //SCI_TRACE_LOW:"mmivt QuitMainOptionMode"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_UI_5930_112_2_18_3_6_23_188,(uint8*)"");

    HideMainOption(win_id);
    //HideVideoOption(win_id);
    //HideVideoParam(win_id);

    SetConnectedWinSoftKey(win_id, FALSE);
}

/*****************************************************************************/
// 	Description : DisplayMainOption
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
LOCAL void DisplayMainOption(MMI_WIN_ID_T win_id)
{
    GUI_BG_T audio_button_bg = {GUI_BG_IMG, GUI_SHAPE_RECT, IMAG_VT_HANDHOLD_OFF, 0, FALSE};
    GUI_BG_T video_button_bg = {GUI_BG_IMG, GUI_SHAPE_RECT, IMAGE_VT_DISPLAY_OFF, 0, FALSE};
    GUI_BG_T camera_button_bg = {GUI_BG_IMG, GUI_SHAPE_RECT, IMAG_VT_CAMERA_OFF, 0, FALSE};
    GUI_BG_T dispmode_button_bg = {GUI_BG_IMG, GUI_SHAPE_RECT, IMAG_VT_DISPMODE_OFF, 0, FALSE};

    VT_DISPLAY_INFO_T display_info = {0};

    display_info = MMIVT_GetAppletInstance()->vt_instance.ui_display_info;

    //audio option 状态显示
    if (display_info.is_handfree_show)
    {
        audio_button_bg.img_id= IMAG_VT_HANDFREE_OFF;
    }
    else
    {
        audio_button_bg.img_id= IMAG_VT_HANDHOLD_OFF;
    }

    // 设置焦点
#ifndef TOUCH_PANEL_SUPPORT
    switch(display_info.option_focus) 
    {
    case MMIVT_1LEVEL_AUDIO_OPTION:
    if (display_info.is_handfree_show)
    {
        audio_button_bg.img_id= IMAG_VT_HANDFREE_ON;
    }
    else
    {
        audio_button_bg.img_id= IMAG_VT_HANDHOLD_ON;
    }
    	break;

    case MMIVT_1LEVEL_VIDEO_OPTION:
        video_button_bg.img_id= IMAGE_VT_DISPLAY_ON;
    	break;

    case MMIVT_1LEVEL_CAMERA_OPTION:
        camera_button_bg.img_id= IMAG_VT_CAMERA_ON;
    	break;

    case MMIVT_1LEVEL_DISPLAY_MODE_OPTION:
        dispmode_button_bg.img_id= IMAG_VT_DISPMODE_ON;
    	break;
    
    default:
        //SCI_TRACE_LOW:"mmivt:wrong focus %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_UI_5992_112_2_18_3_6_23_189,(uint8*)"d", display_info.option_focus);
        audio_button_bg.img_id= IMAG_VT_HANDHOLD_ON;
        display_info.option_focus = MMIVT_1LEVEL_AUDIO_OPTION;
        break; 
    }

#endif

    //呼出窗口和重拨窗口需要灰化button
    if( MMK_IsFocusWin(MMIVT_MO_CALLING_WIN_ID)//(MMK_GetWinHandle(MMIVT_GetAppletInstance()->caf_applet.app_handle, MMIVT_MO_CALLING_WIN_ID) == win_id)
        ||MMK_IsFocusWin(MMIVT_REDIAL_WIN_ID)//(MMK_GetWinHandle(MMIVT_GetAppletInstance()->caf_applet.app_handle, MMIVT_REDIAL_WIN_ID) == win_id)
        )
    {
        GUI_BG_T video_button_gray_bg = {GUI_BG_IMG, GUI_SHAPE_RECT, IMAGE_VT_DISPLAY_GRAY, 0, FALSE};
        GUI_BG_T camera_button_gray_bg = {GUI_BG_IMG, GUI_SHAPE_RECT, IMAG_VT_CAMERA_GRAY, 0, FALSE};
        GUI_BG_T dispmode_button_gray_bg = {GUI_BG_IMG, GUI_SHAPE_RECT, IMAG_VT_DISPMODE_GRAY, 0, FALSE};
        MMI_HANDLE_T video_button_handle = MMK_GetCtrlHandleByWin(win_id, MMIVT_BUTTON_VIDEO_OPTION_CTRL_ID);
        MMI_HANDLE_T camera_button_handle = MMK_GetCtrlHandleByWin(win_id, MMIVT_BUTTON_CAMERA_OPTION_CTRL_ID);
        MMI_HANDLE_T dispmode_button_handle = MMK_GetCtrlHandleByWin(win_id, MMIVT_BUTTON_DISPMODE_OPTION_CTRL_ID);
        GUIBUTTON_SetGrayed(video_button_handle, TRUE, &video_button_gray_bg, TRUE);
        GUIBUTTON_SetGrayed(camera_button_handle, TRUE, &camera_button_gray_bg, TRUE);
        GUIBUTTON_SetGrayed(dispmode_button_handle, TRUE, &dispmode_button_gray_bg, TRUE);

        DisplayButtonImage(win_id, MMIVT_BUTTON_AUDIO_OPTION_CTRL_ID, &audio_button_bg);
    }
    else
    {
        DisplayButtonImage(win_id, MMIVT_BUTTON_AUDIO_OPTION_CTRL_ID, &audio_button_bg);
        DisplayButtonImage(win_id, MMIVT_BUTTON_VIDEO_OPTION_CTRL_ID, &video_button_bg);
        DisplayButtonImage(win_id, MMIVT_BUTTON_CAMERA_OPTION_CTRL_ID, &camera_button_bg);
        DisplayButtonImage(win_id, MMIVT_BUTTON_DISPMODE_OPTION_CTRL_ID, &dispmode_button_bg);
    }
    
}

/*****************************************************************************/
// 	Description : HideMainOption 
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
LOCAL void HideMainOption(MMI_WIN_ID_T win_id)
{
    SetCtrlVisible(win_id, MMIVT_BUTTON_AUDIO_OPTION_CTRL_ID, FALSE);
    SetCtrlVisible(win_id, MMIVT_BUTTON_VIDEO_OPTION_CTRL_ID, FALSE);
    SetCtrlVisible(win_id, MMIVT_BUTTON_CAMERA_OPTION_CTRL_ID, FALSE);
    SetCtrlVisible(win_id, MMIVT_BUTTON_DISPMODE_OPTION_CTRL_ID, FALSE);
}

/*****************************************************************************/
// 	Description : EnterVideoOptionMode
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
LOCAL void EnterVideoOptionMode(MMI_WIN_ID_T win_id)
{
    //SCI_TRACE_LOW:"mmivt EnterVideoOptionMode"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_UI_6045_112_2_18_3_6_23_190,(uint8*)"");

    MMIVT_GetAppletInstance()->vt_instance.ui_display_info.option_bank_mode = MMIVT_OPTION_BANK_MODE_VIDEO_OPTION;

    DisplayVideoOption(win_id);

    SetConnectedWinSoftKey(win_id, FALSE);
}

/*****************************************************************************/
// 	Description : QuitVideoOptionMode
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
LOCAL void QuitVideoOptionMode(MMI_WIN_ID_T win_id)
{
    //SCI_TRACE_LOW:"mmivt QuitVideoOptionMode"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_UI_6060_112_2_18_3_6_23_191,(uint8*)"");

    HideVideoOption(win_id);
    //HideVideoParam(win_id);

    SetConnectedWinSoftKey(win_id, FALSE);
    
}

/*****************************************************************************/
// 	Description : DisplayVideoOption
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
LOCAL void DisplayVideoOption(MMI_WIN_ID_T win_id)
{
    GUI_BG_T lcd_brightness_button_bg = {GUI_BG_IMG, GUI_SHAPE_RECT, IMAG_VT_LCD_BRIGHTNESS_OFF, 0, FALSE};
    GUI_BG_T contrast_button_bg = {GUI_BG_IMG, GUI_SHAPE_RECT, IMAGE_VT_VIDEO_OPTION_CONTRAST_OFF, 0, FALSE};
    GUI_BG_T brightness_button_bg = {GUI_BG_IMG, GUI_SHAPE_RECT, IMAGE_VT_VIDEO_OPTION_BRIGHTNESS_OFF, 0, FALSE};
    GUI_BG_T rotate_button_bg = {GUI_BG_IMG, GUI_SHAPE_RECT, IMAGE_VT_VIDEO_OPTION_ROTATE_OFF, 0, FALSE};

    MMIVT_2LEVEL_VIDEO_OPTION_E sub_option_focus = MMIVT_GetAppletInstance()->vt_instance.ui_display_info.video_sub_option_focus;

    //SCI_TRACE_LOW:"mmivt DisplayVideoOption focus %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_UI_6082_112_2_18_3_6_23_192,(uint8*)"d", sub_option_focus);

    
#ifndef TOUCH_PANEL_SUPPORT
    //设置焦点
    switch(sub_option_focus) 
    {
    case MMIVT_2LEVEL_VIDEO_LCD_BRIGHT_OPTION:
        lcd_brightness_button_bg.img_id= IMAG_VT_LCD_BRIGHTNESS_ON;
    	break;

    case MMIVT_2LEVEL_VIDEO_CONTRAST_OPTION:
        contrast_button_bg.img_id= IMAGE_VT_VIDEO_OPTION_CONTRAST_ON;
    	break;

    case MMIVT_2LEVEL_VIDEO_BRIGHT_OPTION:
        brightness_button_bg.img_id= IMAGE_VT_VIDEO_OPTION_BRIGHTNESS_ON;
    	break;

    case MMIVT_2LEVEL_VIDEO_ROTATE_OPTION:
        rotate_button_bg.img_id= IMAGE_VT_VIDEO_OPTION_ROTATE_ON;
    	break;

    default:
        //SCI_TRACE_LOW:"mmivt DisplayVideoOption wrong sub focus"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_UI_6106_112_2_18_3_6_24_193,(uint8*)"");
        break;
    }
#endif
    DisplayLabelImage(win_id, MMIVT_LABEL_SUBOPTION_BG_CTRL_ID, IMAGE_VT_VIDEO_OPTION_BG);

    DisplayButtonImage(win_id, MMIVT_BUTTON_DISPLAY_LCD_BRIGHTNESS_CTRL_ID, &lcd_brightness_button_bg);
    DisplayButtonImage(win_id, MMIVT_BUTTON_DISPLAY_CONTRAST_CTRL_ID, &contrast_button_bg);
    DisplayButtonImage(win_id, MMIVT_BUTTON_DISPLAY_BRIGHTNESS_CTRL_ID, &brightness_button_bg);
    DisplayButtonImage(win_id, MMIVT_BUTTON_DISPLAY_ROTATE_CTRL_ID, &rotate_button_bg);
}

/*****************************************************************************/
// 	Description : HideVideoOption 
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
LOCAL void HideVideoOption(MMI_WIN_ID_T win_id)
{
    SetCtrlVisible(win_id, MMIVT_BUTTON_DISPLAY_LCD_BRIGHTNESS_CTRL_ID, FALSE);
    SetCtrlVisible(win_id, MMIVT_BUTTON_DISPLAY_CONTRAST_CTRL_ID, FALSE);
    SetCtrlVisible(win_id, MMIVT_BUTTON_DISPLAY_BRIGHTNESS_CTRL_ID, FALSE);
    SetCtrlVisible(win_id, MMIVT_BUTTON_DISPLAY_ROTATE_CTRL_ID, FALSE);
    GUILABEL_SetVisible(MMK_GetCtrlHandleByWin(win_id, MMIVT_LABEL_SUBOPTION_BG_CTRL_ID), FALSE, FALSE);//SetCtrlVisible(win_id, MMIVT_LABEL_SUBOPTION_BG_CTRL_ID, FALSE);
}

/*****************************************************************************/
// 	Description : EnterVideoParamMode 
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
LOCAL void EnterVideoParamMode(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T button_ctrl_id)
{
    MMIVT_2LEVEL_VIDEO_OPTION_E video_option = MMIVT_2LEVEL_VIDEO_LCD_BRIGHT_OPTION;

    //SCI_TRACE_LOW:"mmivt EnterVideoParamMode"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_UI_6139_112_2_18_3_6_24_194,(uint8*)"");

    MMIVT_GetAppletInstance()->vt_instance.ui_display_info.is_video_sub_option_active = TRUE;
#ifdef  TOUCH_PANEL_SUPPORT
    {
        switch(button_ctrl_id) 
        {
        case MMIVT_BUTTON_DISPLAY_LCD_BRIGHTNESS_CTRL_ID:
            video_option = MMIVT_2LEVEL_VIDEO_LCD_BRIGHT_OPTION;
            MMIVT_GetAppletInstance()->vt_instance.ui_display_info.video_sub_option_focus = MMIVT_2LEVEL_VIDEO_LCD_BRIGHT_OPTION;
        	break;

        case MMIVT_BUTTON_DISPLAY_CONTRAST_CTRL_ID:
            video_option = MMIVT_2LEVEL_VIDEO_CONTRAST_OPTION;
            MMIVT_GetAppletInstance()->vt_instance.ui_display_info.video_sub_option_focus = MMIVT_2LEVEL_VIDEO_CONTRAST_OPTION;
        	break;

        case MMIVT_BUTTON_DISPLAY_BRIGHTNESS_CTRL_ID:
            video_option = MMIVT_2LEVEL_VIDEO_BRIGHT_OPTION;
            MMIVT_GetAppletInstance()->vt_instance.ui_display_info.video_sub_option_focus = MMIVT_2LEVEL_VIDEO_BRIGHT_OPTION;
        	break;

        case MMIVT_BUTTON_DISPLAY_ROTATE_CTRL_ID:
            video_option = MMIVT_2LEVEL_VIDEO_ROTATE_OPTION;
            MMIVT_GetAppletInstance()->vt_instance.ui_display_info.video_sub_option_focus = MMIVT_2LEVEL_VIDEO_ROTATE_OPTION;
            break;
            
        default:
            //SCI_TRACE_LOW:"mmivt EnterVideoParamMode error ctrl"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_UI_6167_112_2_18_3_6_24_195,(uint8*)"");
            return;
        }
    }
#else
    video_option = MMIVT_GetAppletInstance()->vt_instance.ui_display_info.video_sub_option_focus;
#endif
    DisplayVideoParam(win_id, video_option);

    //开启界面关闭定时器
    StartWinTimer(win_id, &s_vt_video_param_timer, VT_FIVE_SECONDS_TIMEOUT);
}

/*****************************************************************************/
// 	Description : QuitVideoParamMode
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
LOCAL void QuitVideoParamMode(MMI_WIN_ID_T win_id)
{
    //SCI_TRACE_LOW:"mmivt QuitVideoParamMode"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_UI_6186_112_2_18_3_6_24_196,(uint8*)"");

    MMIVT_GetAppletInstance()->vt_instance.ui_display_info.is_video_sub_option_active = FALSE;

    HideVideoParam(win_id);

    //停止关闭定时器
    StopWinTimer(&s_vt_video_param_timer);
}

/*****************************************************************************/
// 	Description : DisplayVideoParam
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
LOCAL void DisplayVideoParam(MMI_WIN_ID_T win_id, MMIVT_2LEVEL_VIDEO_OPTION_E video_option)
{
    //SCI_TRACE_LOW:"mmivt DisplayVideoParam video option:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_UI_6202_112_2_18_3_6_24_197,(uint8*)"d", video_option);

    if (MMIVT_2LEVEL_VIDEO_ROTATE_OPTION == video_option)
    {
        //显示图像旋转设置界面
        // 显示背景，类型提示和控制按钮
        GUI_BG_T bg_90 = {GUI_BG_IMG, GUI_SHAPE_RECT, IMAGE_VT_VIDEO_OPTION_ROTATE_90_OFF, 0, FALSE};
        GUI_BG_T bg_180 = {GUI_BG_IMG, GUI_SHAPE_RECT, IMAGE_VT_VIDEO_OPTION_ROTATE_180_OFF, 0, FALSE};
#ifndef TOUCH_PANEL_SUPPORT
        switch(MMIVT_GetAppletInstance()->vt_instance.ui_display_info.image_rotate_mode) 
        {
        case VT_IMAGE_ROTATE_90_DEGREE:
            bg_90.img_id= IMAGE_VT_VIDEO_OPTION_ROTATE_90_ON;
        	break;

        case VT_IMAGE_ROTATE_180_DEGREE:
            bg_180.img_id= IMAGE_VT_VIDEO_OPTION_ROTATE_180_ON;
        	break;

        default:
            //SCI_TRACE_LOW:"mmivt DisplayVideoParam err rotate mode:%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_UI_6222_112_2_18_3_6_24_198,(uint8*)"d", MMIVT_GetAppletInstance()->vt_instance.ui_display_info.image_rotate_mode);
            return;
        }
#endif
        DisplayLabelImage(win_id, MMIVT_LABEL_SUBOPTION_BG_CTRL_ID, IMAGE_VT_VIDEO_OPTION_BG);
        DisplayLabelImage(win_id, MMIVT_LABEL_PROMOPT_CTRL_ID, IMAG_VT_ICON_ROTATE);
        DisplayButtonImage(win_id, MMIVT_BUTTON_ROTATE_90_CTRL_ID, &bg_90);
        DisplayButtonImage(win_id, MMIVT_BUTTON_ROTATE_180_CTRL_ID, &bg_180);        
        
    }
    else
    {
        //显示屏幕亮度，sensor对比度，亮度设置界面
        MMIVT_VIDEO_LEVEL_E video_level = MMIVT_VIDEO_LEVEL_ONE; //视频参数等级
        MMI_IMAGE_ID_T prompt_image = IMAGE_NULL; //视频参数类型提示图片
        switch(video_option) 
        {

        case MMIVT_2LEVEL_VIDEO_LCD_BRIGHT_OPTION:
            video_level = MMIVT_GetAppletInstance()->vt_instance.ui_display_info.lcd_bright_level;
            prompt_image = IMAG_VT_ICON_LCD_BRIGHTNESS;
    	    break;

        case MMIVT_2LEVEL_VIDEO_CONTRAST_OPTION:
            video_level = MMIVT_GetAppletInstance()->vt_instance.ui_display_info.contrast_level;
            prompt_image = IMAG_VT_ICON_CONTRAST;
    	    break;

        case MMIVT_2LEVEL_VIDEO_BRIGHT_OPTION:
            video_level = MMIVT_GetAppletInstance()->vt_instance.ui_display_info.brightness_level;
            prompt_image = IMAG_VT_ICON_BRIGHTNESS;
    	    break;

        default:
            //SCI_TRACE_LOW:"mmivt wrong video option %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_UI_6256_112_2_18_3_6_24_199,(uint8*)"d", video_option);
            return;
        }
        // 显示背景，类型提示和控制按钮
        DisplayLabelImage(win_id, MMIVT_LABEL_SUBOPTION_BG_CTRL_ID, IMAGE_VT_VIDEO_OPTION_BG);
        DisplayLabelImage(win_id, MMIVT_LABEL_PROMOPT_CTRL_ID, prompt_image);
        DisplayButtonImage(win_id, MMIVT_BUTTON_VIDEO_PARAM_MINUS_CTRL_ID, PNULL);
        DisplayButtonImage(win_id, MMIVT_BUTTON_VIDEO_PARAM_ADD_CTRL_ID, PNULL);
        
        //显示参数
        {
            MMI_STRING_T video_para_str = {0};
            wchar temp_string[VT_VIDEO_OPTION_PARAM_LENGTH_MAX] = {0};
            char num_char_str[VT_VIDEO_OPTION_PARAM_LENGTH_MAX] = {0};
            MMI_HANDLE_T video_param_label_handle = MMK_GetCtrlHandleByWin(win_id, MMIVT_LABEL_VIDEO_PARAM_CTRL_ID);

            video_para_str.wstr_ptr = temp_string;
            MMIAPICOM_Int2Str((uint32)video_level, (uint8*)num_char_str,VT_VIDEO_OPTION_PARAM_LENGTH_MAX);
            MMI_STRNTOWSTR(temp_string,VT_VIDEO_OPTION_PARAM_LENGTH_MAX,(uint8 *)num_char_str,VT_VIDEO_OPTION_PARAM_LENGTH_MAX,VT_VIDEO_OPTION_PARAM_LENGTH_MAX);
            video_para_str.wstr_len = MMIAPICOM_Wstrlen(video_para_str.wstr_ptr);
            GUILABEL_SetVisible(MMK_GetCtrlHandleByWin(win_id, MMIVT_LABEL_VIDEO_PARAM_CTRL_ID), TRUE, FALSE);//SetCtrlVisible(win_id, MMIVT_LABEL_VIDEO_PARAM_CTRL_ID, TRUE);
            GUILABEL_SetText(video_param_label_handle, &video_para_str, TRUE);
        }
    }
}

/*****************************************************************************/
// 	Description : HideVideoParam 
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
LOCAL void HideVideoParam(MMI_WIN_ID_T win_id)
{
    GUILABEL_SetVisible(MMK_GetCtrlHandleByWin(win_id, MMIVT_LABEL_SUBOPTION_BG_CTRL_ID), FALSE, FALSE);//SetCtrlVisible(win_id, MMIVT_LABEL_SUBOPTION_BG_CTRL_ID, FALSE);
    GUILABEL_SetVisible(MMK_GetCtrlHandleByWin(win_id, MMIVT_LABEL_PROMOPT_CTRL_ID), FALSE, FALSE);//SetCtrlVisible(win_id, MMIVT_LABEL_PROMOPT_CTRL_ID, FALSE);
    GUILABEL_SetVisible(MMK_GetCtrlHandleByWin(win_id, MMIVT_LABEL_VIDEO_PARAM_CTRL_ID), FALSE, FALSE);//SetCtrlVisible(win_id, MMIVT_LABEL_VIDEO_PARAM_CTRL_ID, FALSE);
    SetCtrlVisible(win_id, MMIVT_BUTTON_VIDEO_PARAM_MINUS_CTRL_ID, FALSE);
    SetCtrlVisible(win_id, MMIVT_BUTTON_VIDEO_PARAM_ADD_CTRL_ID, FALSE);
    SetCtrlVisible(win_id, MMIVT_BUTTON_ROTATE_90_CTRL_ID, FALSE);
    SetCtrlVisible(win_id, MMIVT_BUTTON_ROTATE_180_CTRL_ID, FALSE);
}

/*****************************************************************************/
// 	Description : EnterUserInputMode
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
LOCAL void EnterUserInputMode(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id)
{
    MMI_CTRL_ID_T edit_ctrl_id = MMIVT_EDIT_USERINPUT_CTRL_ID;
    MMI_HANDLE_T edit_ctrl_handle = PNULL;
    //MMI_IMAGE_ID_T edit_bg = IMAGE_VT_USERINPUT_BG;
    //const uint16 edit_boder_width = USERINPUT_EDIT_BODER_WIDTH;
    //const GUI_COLOR_T edit_boder_color = VT_TRANSPARENT_COLOR;
    GUI_BORDER_T edit_border = {USERINPUT_EDIT_BODER_WIDTH, MMI_BACKGROUND_COLOR, GUI_BORDER_SOLID};
    //const GUI_COLOR_T edit_bg_color = MMI_WHITE_COLOR;
    const GUI_COLOR_T edit_font_color = MMI_BACKGROUND_COLOR;
    GUI_BG_T edit_bg = {GUI_BG_COLOR, GUI_SHAPE_RECT, IMAGE_NULL, MMI_WHITE_COLOR, FALSE};
    
    MMI_CONTROL_CREATE_T create = {0};
    GUIEDIT_INIT_DATA_T init_data = {0};
    GUI_BOTH_RECT_T user_input_both_rect = USER_INPUT_EDIT_BOTH_RECT;
    //SCI_TRACE_LOW:"mmivt EnterUserInputMode"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_UI_6316_112_2_18_3_6_24_200,(uint8*)"");

    MMIVT_GetAppletInstance()->vt_instance.ui_display_info.option_bank_mode = MMIVT_OPTION_BANK_MODE_USERINPUT;

    //create ctrl
//     init_data.rect.left = USERINPUT_EDIT_LEFT;
//     init_data.rect.right = USERINPUT_EDIT_RIGHT;
//     init_data.rect.top = USERINPUT_EDIT_TOP;
//     init_data.rect.bottom = USERINPUT_EDIT_BOTTOM;
    init_data.both_rect = user_input_both_rect;

    init_data.type = GUIEDIT_TYPE_PHONENUM;
    init_data.str_max_len = VT_USERINPUT_LENTH_MAX;


    create.init_data_ptr = &init_data;
    create.ctrl_id = edit_ctrl_id;
    create.guid = SPRD_GUI_EDITBOX_ID;
    create.parent_win_handle = win_id;

    MMK_CreateControl(&create);

    //set ctrl parameter
    edit_ctrl_handle = MMK_GetCtrlHandleByWin(win_id, edit_ctrl_id);
    GUIEDIT_SetStyle(edit_ctrl_handle, GUIEDIT_STYLE_SINGLE);
    GUIEDIT_SetPhoneNumStyle(TRUE, edit_ctrl_handle);
    //GUIEDIT_SetFont(edit_ctrl_handle, SONG_FONT_20);
    GUIEDIT_SetFontColor(edit_ctrl_handle, edit_font_color);
    GUIEDIT_SetBorder(edit_ctrl_handle, &edit_border);
    //GUIEDIT_SetLongCancelDelAll(TRUE, edit_ctrl_handle);
    GUIEDIT_SetBg(edit_ctrl_handle, &edit_bg);
    GUIEDIT_SetImTp(edit_ctrl_handle, FALSE);
    SetConnectedWinSoftKey(win_id, TRUE);
    MMK_SetActiveCtrl(edit_ctrl_handle, TRUE);


#ifdef TOUCH_PANEL_SUPPORT
    //DisplayImageIcon(win_id, MMIVT_ANIM_OPTION_BG_BANK_CTRL_ID, IMAGE_COMMON_BG, PNULL);

    /*if (MMIVT_DISP_MODE_LOCAL_REMOTE == MMIVT_GetAppletInstance()->vt_instance.ui_display_info.display_mode)
    {
        MMIVT_REF_SetDispArea(MMIVT_GetAppletInstance()->vt_instance.ui_display_info.image_local_rotate_status, MMIVT_DISP_MODE_LOCAL_ONLY);
    }
    else if (MMIVT_DISP_MODE_REMOTE_LOCAL == MMIVT_GetAppletInstance()->vt_instance.ui_display_info.display_mode)
    {
        MMIVT_REF_SetDispArea(MMIVT_GetAppletInstance()->vt_instance.ui_display_info.image_local_rotate_status, MMIVT_DISP_MODE_REMOTE_ONLY);
    }*/

    
    SetCtrlVisible(win_id, MMIVT_BUTTON_DIAL_DEL_CTRL_ID, TRUE);
    SetCtrlVisible(win_id, MMIVT_BUTTON_DIAL_1_CTRL_ID, TRUE);
    SetCtrlVisible(win_id, MMIVT_BUTTON_DIAL_2_CTRL_ID, TRUE);
    SetCtrlVisible(win_id, MMIVT_BUTTON_DIAL_3_CTRL_ID, TRUE);
    SetCtrlVisible(win_id, MMIVT_BUTTON_DIAL_4_CTRL_ID, TRUE);
    SetCtrlVisible(win_id, MMIVT_BUTTON_DIAL_5_CTRL_ID, TRUE);
    SetCtrlVisible(win_id, MMIVT_BUTTON_DIAL_6_CTRL_ID, TRUE);
    SetCtrlVisible(win_id, MMIVT_BUTTON_DIAL_7_CTRL_ID, TRUE);
    SetCtrlVisible(win_id, MMIVT_BUTTON_DIAL_8_CTRL_ID, TRUE);
    SetCtrlVisible(win_id, MMIVT_BUTTON_DIAL_9_CTRL_ID, TRUE);
    SetCtrlVisible(win_id, MMIVT_BUTTON_DIAL_0_CTRL_ID, TRUE);
    SetCtrlVisible(win_id, MMIVT_BUTTON_DIAL_STAR_CTRL_ID, TRUE);
    SetCtrlVisible(win_id, MMIVT_BUTTON_DIAL_HASH_CTRL_ID, TRUE);

    GUILABEL_SetVisible(MMK_GetCtrlHandleByWin(win_id, MMIVT_LABEL_CALLNAME_CTRL_ID), FALSE, FALSE);//SetCtrlVisible(win_id, MMIVT_LABEL_CALLNAME_CTRL_ID, FALSE);
    GUILABEL_SetVisible(MMK_GetCtrlHandleByWin(win_id, MMIVT_LABEL_CALLTIME_CTRL_ID), FALSE, FALSE);//SetCtrlVisible(win_id, MMIVT_LABEL_CALLTIME_CTRL_ID, FALSE);
    GUILABEL_SetVisible(MMIVT_LABEL_PHOTO_CTRL_ID, FALSE, FALSE);

#endif    

    //将第一个按键消息转化为str，放到控件中 GUIEDIT_SetString
    if ((msg_id >= MSG_APP_1) && ( msg_id <= MSG_APP_HASH))
    {
        MMK_PostMsg(edit_ctrl_handle, MSG_CTL_PAINT, 0, 0);
        MMK_PostMsg(win_id, msg_id, PNULL,0);
    }

}

/*****************************************************************************/
// 	Description : QuitUserInputMode
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
LOCAL void QuitUserInputMode(MMI_WIN_ID_T win_id)
{
    MMI_HANDLE_T userinput_edit_handle = MMK_GetCtrlHandleByWin(win_id, MMIVT_EDIT_USERINPUT_CTRL_ID);
    
    //SCI_TRACE_LOW:"mmivt QuitUserInputMode"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_UI_6402_112_2_18_3_6_24_201,(uint8*)"");

    MMIVT_GetAppletInstance()->vt_instance.ui_display_info.option_bank_mode = MMIVT_OPTION_BANK_MODE_NORMAL;

    MMK_DestroyControl(userinput_edit_handle);

    SetConnectedWinSoftKey(win_id, FALSE);

#ifdef TOUCH_PANEL_SUPPORT
    DisplayImageIcon(win_id, MMIVT_ANIM_OPTION_BG_BANK_CTRL_ID, IMAG_VT_BG_BANK, PNULL);

     //给视频显示区域刷透明色
    {
        GUI_RECT_T main_paly_rect = {MAIN_PLAY_RECT_LEFT, MAIN_PLAY_RECT_TOP, MAIN_PLAY_RECT_RIGHT, MAIN_PLAY_RECT_BOTTOM};
        GUI_RECT_T secondary_play_rect = {SECONDARY_PLAY_RECT_LEFT, SECONDARY_PLAY_RECT_TOP, SECONDARY_PLAY_RECT_RIGHT, SECONDARY_PLAY_RECT_BOTTOM};
        GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
        LCD_FillRect(&lcd_dev_info, main_paly_rect, VT_TRANSPARENT_COLOR);
        LCD_FillRect(&lcd_dev_info, secondary_play_rect, VT_TRANSPARENT_COLOR);
    }

    //MMIVT_REF_SetDispArea(MMIVT_GetAppletInstance()->vt_instance.ui_display_info.image_local_rotate_status, MMIVT_GetAppletInstance()->vt_instance.ui_display_info.display_mode);

    SetCtrlVisible(win_id, MMIVT_BUTTON_DIAL_DEL_CTRL_ID, FALSE);
    SetCtrlVisible(win_id, MMIVT_BUTTON_DIAL_1_CTRL_ID, FALSE);
    SetCtrlVisible(win_id, MMIVT_BUTTON_DIAL_2_CTRL_ID, FALSE);
    SetCtrlVisible(win_id, MMIVT_BUTTON_DIAL_3_CTRL_ID, FALSE);
    SetCtrlVisible(win_id, MMIVT_BUTTON_DIAL_4_CTRL_ID, FALSE);
    SetCtrlVisible(win_id, MMIVT_BUTTON_DIAL_5_CTRL_ID, FALSE);
    SetCtrlVisible(win_id, MMIVT_BUTTON_DIAL_6_CTRL_ID, FALSE);
    SetCtrlVisible(win_id, MMIVT_BUTTON_DIAL_7_CTRL_ID, FALSE);
    SetCtrlVisible(win_id, MMIVT_BUTTON_DIAL_8_CTRL_ID, FALSE);
    SetCtrlVisible(win_id, MMIVT_BUTTON_DIAL_9_CTRL_ID, FALSE);
    SetCtrlVisible(win_id, MMIVT_BUTTON_DIAL_0_CTRL_ID, FALSE);
    SetCtrlVisible(win_id, MMIVT_BUTTON_DIAL_STAR_CTRL_ID, FALSE);
    SetCtrlVisible(win_id, MMIVT_BUTTON_DIAL_HASH_CTRL_ID, FALSE);

    GUILABEL_SetVisible(MMK_GetCtrlHandleByWin(win_id, MMIVT_LABEL_CALLNAME_CTRL_ID), TRUE, FALSE);//SetCtrlVisible(win_id, MMIVT_LABEL_CALLNAME_CTRL_ID, TRUE);
    GUILABEL_SetVisible(MMK_GetCtrlHandleByWin(win_id, MMIVT_LABEL_CALLTIME_CTRL_ID), TRUE, FALSE);//SetCtrlVisible(win_id, MMIVT_LABEL_CALLTIME_CTRL_ID, TRUE);
    GUILABEL_SetVisible(MMIVT_LABEL_PHOTO_CTRL_ID, TRUE, FALSE);
#endif    

    MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);

}

/*****************************************************************************/
// 	Description : EnterVolumeMode
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
LOCAL void EnterVolumeMode(MMI_WIN_ID_T win_id)
{
    //SCI_TRACE_LOW:"mmivt EnterVolumeMode"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_UI_6453_112_2_18_3_6_24_202,(uint8*)"");

    MMIVT_GetAppletInstance()->vt_instance.ui_display_info.option_bank_mode = MMIVT_OPTION_BANK_MODE_VOLUME;

    DisplayVolume(win_id);

    SetConnectedWinSoftKey(win_id, FALSE);

    StartWinTimer(win_id, &s_vt_volume_close_timer, VT_THREE_SECONDS_TIMEOUT);
}

/*****************************************************************************/
// 	Description : QuitVolumeMode
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
LOCAL void QuitVolumeMode(MMI_WIN_ID_T win_id)
{
    //SCI_TRACE_LOW:"mmivt QuitVolumeMode"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_UI_6470_112_2_18_3_6_24_203,(uint8*)"");

    MMIVT_GetAppletInstance()->vt_instance.ui_display_info.option_bank_mode = MMIVT_OPTION_BANK_MODE_NORMAL;
    
    HideVolume(win_id);

    SetConnectedWinSoftKey(win_id, FALSE);

    //停止关闭定时器
    StopWinTimer(&s_vt_volume_close_timer);
}
/*****************************************************************************/
// 	Description : DisplayVolume
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
LOCAL void DisplayVolume(MMI_WIN_ID_T win_id)
{
    MMI_HANDLE_T volume_display_label_handle = PNULL;
    MMI_STRING_T volume_str = {0};
    wchar temp_string[VT_VOLUME_NUM_LENGTH_MAX] = {0};
    char num_char_str[VT_VOLUME_NUM_LENGTH_MAX] = {0};
    uint8 volume = MMIAPICC_GetCallVolume();

    DisplayLabelImage(win_id, MMIVT_LABEL_SUBOPTION_BG_CTRL_ID, IMAGE_VT_VIDEO_OPTION_BG);
    DisplayLabelImage(win_id, MMIVT_LABEL_VOLUME_PARAM_CTRL_ID, IMAG_VT_ICON_VOLUME);
    DisplayButtonImage(win_id, MMIVT_BUTTON_VOLUME_MINUS_CTRL_ID, PNULL);
    DisplayButtonImage(win_id, MMIVT_BUTTON_VOLUME_ADD_CTRL_ID, PNULL);
    
    volume_str.wstr_ptr = temp_string;

    MMIAPICOM_Int2Str((uint32)volume, (uint8*)num_char_str,VT_VOLUME_NUM_LENGTH_MAX);
    MMI_STRNTOWSTR(temp_string,VT_VOLUME_NUM_LENGTH_MAX,(uint8 *)num_char_str,VT_VOLUME_NUM_LENGTH_MAX,VT_VOLUME_NUM_LENGTH_MAX);
    volume_str.wstr_len = MMIAPICOM_Wstrlen(volume_str.wstr_ptr);

    volume_display_label_handle = MMK_GetCtrlHandleByWin(win_id, MMIVT_LABEL_VIDEO_PARAM_CTRL_ID);
    GUILABEL_SetVisible(MMK_GetCtrlHandleByWin(win_id, MMIVT_LABEL_VIDEO_PARAM_CTRL_ID), TRUE, FALSE);//SetCtrlVisible(win_id, MMIVT_LABEL_VIDEO_PARAM_CTRL_ID, TRUE);
    GUILABEL_SetText(volume_display_label_handle, &volume_str, TRUE);

    
}

/*****************************************************************************/
// 	Description : DisplayVolume
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
LOCAL void HideVolume(MMI_WIN_ID_T win_id)
{
    GUILABEL_SetVisible(MMK_GetCtrlHandleByWin(win_id, MMIVT_LABEL_SUBOPTION_BG_CTRL_ID), FALSE, FALSE);//SetCtrlVisible(win_id, MMIVT_LABEL_SUBOPTION_BG_CTRL_ID, FALSE);
    GUILABEL_SetVisible(MMK_GetCtrlHandleByWin(win_id, MMIVT_LABEL_VOLUME_PARAM_CTRL_ID), FALSE, FALSE);//SetCtrlVisible(win_id, MMIVT_LABEL_VOLUME_PARAM_CTRL_ID, FALSE);
    SetCtrlVisible(win_id, MMIVT_BUTTON_VOLUME_MINUS_CTRL_ID, FALSE);
    SetCtrlVisible(win_id, MMIVT_BUTTON_VOLUME_ADD_CTRL_ID, FALSE);
    GUILABEL_SetVisible(MMK_GetCtrlHandleByWin(win_id, MMIVT_LABEL_VIDEO_PARAM_CTRL_ID), FALSE, FALSE);//SetCtrlVisible(win_id, MMIVT_LABEL_VIDEO_PARAM_CTRL_ID, FALSE);
}

/*****************************************************************************/
// 	Description : SetConnectedWinSoftKey
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
LOCAL void SetConnectedWinSoftKey(MMI_WIN_ID_T win_id, BOOLEAN is_fresh)
{
    VT_DISPLAY_INFO_T display_info = MMIVT_GetAppletInstance()->vt_instance.ui_display_info;

    switch(display_info.option_bank_mode) 
    {
    case MMIVT_OPTION_BANK_MODE_NORMAL:
        if(display_info.is_replacing)
        {
#ifdef MMI_PDA_SUPPORT
            GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_INPUTNUM, TXT_VT_SHOW_ME, is_fresh);
#else
            GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_NULL, TXT_VT_SHOW_ME, is_fresh);
#endif
        }
        else
        {
#ifdef MMI_PDA_SUPPORT
            GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_INPUTNUM, TXT_VT_HIDE_ME, is_fresh);
#else
            GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_NULL, TXT_VT_HIDE_ME, is_fresh);
#endif
        }
#ifdef TOUCH_PANEL_SUPPORT
        GUIWIN_SeSoftkeytButtonIconId(win_id, IMAGE_CALL_DIAL, MIDDLE_BUTTON, is_fresh);
#endif
    	break;

    case MMIVT_OPTION_BANK_MODE_VOLUME:
    case MMIVT_OPTION_BANK_MODE_VIDEO_OPTION:
        GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_NULL, STXT_RETURN, is_fresh);
#ifdef TOUCH_PANEL_SUPPORT
        GUIWIN_SeSoftkeytButtonIconId(win_id, IMAGE_CALL_DIAL, MIDDLE_BUTTON, is_fresh);
#endif
        break;

    case MMIVT_OPTION_BANK_MODE_USERINPUT:
        GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_NULL, STXT_RETURN, is_fresh);

        break;

    default:
        //donohing
        break;
    }    
}


/*****************************************************************************/
//  Description : MMIAPIVT_SendUserInput
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:
/*****************************************************************************/
PUBLIC MMIVT_RESULT_E MMIAPIVT_SendUserInput(uint32 msg_id)
{
    MMI_HANDLE_T applet_handle = PNULL;
    MMI_HANDLE_T win_handle = PNULL;
    MMIVT_APPLET_T *vt_applet_ptr = PNULL;
    uint8 user_input_char = MMIAPICC_MsgidToAscii(msg_id);


    vt_applet_ptr = MMIVT_GetAppletInstance();
    
    //MMI_HANDLE_T applet_handle = ((MMIVT_APPLET_T *)MMK_GetAppletPtrByWin(win_id))->caf_applet.app_handle;
    if (PNULL == vt_applet_ptr)
    {
        return MMIVT_RESULT_NULL_APPLET;
    }
    applet_handle = vt_applet_ptr->caf_applet.app_handle;

    win_handle = MMK_GetWinHandle(applet_handle, MMIVT_CONNECTED_WIN_ID);
    
    //SCI_TRACE_LOW:"mmivt.c HandleUser user_input_char: %c"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_UI_6591_112_2_18_3_6_25_204,(uint8*)"c", user_input_char);

    if (MMK_IsFocusWin(win_handle))
    {
        MMIVT_324_SendUserInput(user_input_char);

        return MMIVT_RESULT_SUCCESS;
    }
    else
    {
        //SCI_TRACE_LOW:"mmivt send userinput error state:%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_UI_6601_112_2_18_3_6_25_205,(uint8*)"d", GetUICurState());
        return MMIVT_RESULT_ERROR_STATE;
    }

}

/*****************************************************************************/
//  Description : MMIAPIVT_ReleaseVTCall
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:
/*****************************************************************************/
PUBLIC MMIVT_RESULT_E MMIAPIVT_ReleaseVTCallByRedkey (void)
{   
    return MMIVT_HandleUIMsg(MMIVT_UI_MSG_HANGUP, PNULL);
}

/*****************************************************************************/
//  Description : MMIAPIVT_IsFallBackCall
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIVT_IsFallBackCall (MN_CALLED_NUMBER_T num)
{

    BOOLEAN result = FALSE;
    MN_CALLED_NUMBER_T bcd_number = {0};
    MMIVT_GetCallBCDNumber(&bcd_number);

    if (MMIVT_UI_STATE_FB_WAITING == GetUICurState())
    {
	    result = (num.num_len ==  bcd_number.num_len)
			    //&& MatchNumberTypeAndPlan(current_addr, s_fallback_address)
			    && !memcmp(num.party_num, bcd_number.party_num, sizeof(bcd_number.party_num));
    }
    
    return result;
}

/*****************************************************************************/
// 	Description : CloseVTAppletWinByID
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
LOCAL void CloseVTAppletWinByID(MMI_WIN_ID_T win_id)
{
    MMI_HANDLE_T win_handle = PNULL;
    MMI_HANDLE_T applet_handle = PNULL;
    
    if (PNULL == MMIVT_GetAppletInstance())
    {
        return;
    }
    applet_handle = MMIVT_GetAppletInstance()->caf_applet.app_handle;
    win_handle = MMK_GetWinHandle(applet_handle, win_id);

    MMK_CloseWin(win_handle);
}

/*****************************************************************************/
// 	Description : GetTextPromptByCause
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
LOCAL MMI_TEXT_ID_T GetTextPromptByCause(MN_CALL_DISCONNECT_CALL_CAUSE_E cause)
{
    MMI_TEXT_ID_T text_id = TXT_NULL;
    MMIVT_APPLET_T *vt_applet_ptr = MMIVT_GetAppletInstance();
    if (PNULL == vt_applet_ptr)
    {
        //SCI_TRACE_LOW:"mmivt null applet"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_UI_6666_112_2_18_3_6_25_206,(uint8*)"");
        return MMIVT_RESULT_NULL_APPLET;
    }
    
    if(MMIVT_CALL_TYPE_MT_CALL != MMIVT_GetCallType() 
        && !vt_applet_ptr->vt_instance.ui_display_info.is_hangup_by_user)
    {
        switch(cause) 
        {
        case MN_CAUSE_NORMAL_CLEARING:
            text_id = TXT_NULL;
            break;
        case MN_CAUSE_UNASSIGNED_NO:
        case MN_CAUSE_NUMBER_CHANGED:
        case MN_CAUSE_INVALID_NO_FORMAT:
            text_id = TXT_CC_ERROR_NUMBER;
    	    break;
        case MN_CAUSE_NO_ROUTE_TO_DEST:
        case MN_CAUSE_CHAN_UNACCEPTABLE:
        case MN_CAUSE_NO_USER_RESPONDING:
        case MN_CAUSE_CALL_REJECTED:
        case MN_CAUSE_FACILITY_REJECTED:
        case MN_CAUSE_NET_OUT_OF_ORDER:
        case MN_CAUSE_TEMP_FAILURE:
        case MN_CAUSE_ACC_INFO_DISCARDED:
        case MN_CAUSE_QOS_UNAV:
        case MN_CAUSE_INVALID_TI:
            text_id = TXT_CC_USER_UNCONNECTED;
            break;
        case MN_CAUSE_OPER_DETERM_BARRING:
        case MN_CAUSE_CUG_INCOMING_BARRED:
            text_id = TXT_CC_CALL_BARRED;
            break;
        case MN_CAUSE_USER_BUSY:
            text_id = TXT_CC_USER_BUSY;
            break;
        case MN_CAUSE_ALERTING_NO_ANSWER:
            text_id = TXT_CC_USER_NOT_RESPOND;
            break;
        case MN_CAUSE_DEST_OUT_OF_ORDER:
            text_id = TXT_CC_USER_POWER_OFF;
            break;
        case MN_CAUSE_NO_CIRC_CHAN_AV:
        case MN_CAUSE_SWITCH_CONGESTION:
        case MN_CAUSE_REQ_CIRC_CHAN_UNAV:
            text_id = TXT_COMMON_LINK_DISCONNECTED;
            break;
        case MN_CAUSE_SERV_OPT_UNAV:
            text_id = TXT_CC_ROAMING_CALL_BARRED;
            break;
        case MN_CAUSE_SVC_OPT_NOT_IMPL:
            text_id = TXT_CC_VT_UNSUPPORT;
            break;
        default:
            text_id = TXT_CC_USER_UNCONNECTED;
            break;
        }        
    }
    else
    {
        text_id = TXT_NULL;
    }

    return text_id;
}

/*****************************************************************************/
// 	Description : GetFallBackPromptByCause
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
LOCAL MMI_TEXT_ID_T GetFallBackPromptByCause(MN_CALL_DISCONNECT_CALL_CAUSE_E cause)
{
    MMI_TEXT_ID_T text_id = TXT_VIDEO_CALL_FB_CAUSE_UNAVAILABLE; //#47 MN_CAUSE_RESOURCES_UNAV
    
    switch(cause) 
    {

    case MN_CAUSE_BEAR_CAP_NOT_AUTH: //#57
        text_id = TXT_VIDEO_CALL_FB_CAUSE_NOT_START_SERVICE;
        break;
    case MN_CAUSE_BEAR_CAP_UNAV://#58
        text_id = TXT_VIDEO_CALL_FB_CAUSE_TEMP_UNSUPPORTED;
        break;
    case MN_CAUSE_INCOMPAT_DEST://#88
        text_id = TXT_VIDEO_CALL_FB_CAUSE_USER_CHOOSE_NC;
        break;
    default:
        //do nothing #47 and others
        break;
    }
    return text_id;
}

/*****************************************************************************/
// 	Description : HandleEnableCamera
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
LOCAL MMIVT_REF_RET_E HandleEnableCamera(BOOLEAN is_enable)
{
    MMIVT_REF_RET_E ret_code = MMIVT_REF_RET_SUCCESS;
    //SCI_TRACE_LOW:"mmivt HandleEnableCamera %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_UI_6766_112_2_18_3_6_25_207,(uint8*)"d", is_enable);
    if (!is_enable)
    {
        ret_code = MMIVT_REF_ReplaceStart(MMISET_VT_REPLACE_LOCAL_IMAGE, FALSE);
    }
    else
    {
        ret_code = MMIVT_REF_ReplaceStop(MMISET_VT_REPLACE_LOCAL_IMAGE);
    }
    if (MMIVT_REF_RET_SUCCESS == ret_code
        || MMIVT_REF_RET_REPLACE_UDISK_IN_USE == ret_code
        || MMIVT_REF_RET_REPLACE_FILE_NOT_EXIST == ret_code
        )
    {
        //通知324向对方发送打开或者关闭摄像头消息
        MMIVT_324_EnableCamera(is_enable);
        MMIVT_GetAppletInstance()->vt_instance.ui_display_info.is_replacing = !is_enable;
    }
    return ret_code;
}

/*****************************************************************************/
// 	Description : StartLocalReplaceImage
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
LOCAL MMIVT_REF_RET_E StartLocalReplaceImage(BOOLEAN is_default)
{

    MMIVT_REF_RET_E result = MMIVT_REF_ReplaceStart(MMISET_VT_REPLACE_LOCAL_IMAGE, is_default);
    if (MMIVT_REF_RET_SUCCESS == result
        || MMIVT_REF_RET_REPLACE_UDISK_IN_USE == result
        || MMIVT_REF_RET_REPLACE_FILE_NOT_EXIST == result
        )
    {
        MMIVT_GetAppletInstance()->vt_instance.ui_display_info.is_replacing = TRUE;
    }
    else if(!is_default)
    {
         
    }
    return result;
    
}

/*****************************************************************************/
// 	Description : StopLocalReplaceImage
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
LOCAL void StopLocalReplaceImage(void)
{
    MMIVT_REF_ReplaceStop(MMISET_VT_REPLACE_LOCAL_IMAGE);
    MMIVT_GetAppletInstance()->vt_instance.ui_display_info.is_replacing = FALSE;
}

/*****************************************************************************/
// 	Description : StartRemoteReplaceImage
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
LOCAL MMIVT_REF_RET_E StartRemoteReplaceImage(BOOLEAN is_default)
{

    MMIVT_REF_RET_E result = MMIVT_REF_ReplaceStart(MMISET_VT_REPLACE_REMOTE_IMAGE, is_default);
    if (MMIVT_REF_RET_SUCCESS == result
        || MMIVT_REF_RET_REPLACE_UDISK_IN_USE == result
        || MMIVT_REF_RET_REPLACE_FILE_NOT_EXIST == result
        )
    {
        MMIVT_GetAppletInstance()->vt_instance.ui_display_info.is_remote_replacing = TRUE;
    }
    else if(!is_default)
    {
         
    }
    return result;
    
}

/*****************************************************************************/
// 	Description : StopRemoteReplaceImage
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
LOCAL void StopRemoteReplaceImage(void)
{
    MMIVT_REF_ReplaceStop(MMISET_VT_REPLACE_REMOTE_IMAGE);
    MMIVT_GetAppletInstance()->vt_instance.ui_display_info.is_remote_replacing = FALSE;
}



/*****************************************************************************/
//  Description : MMIAPIVT_ConnectCall
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIVT_ConnectCall (void)
{
    MMIVT_HandleUIMsg(MMIVT_UI_MSG_CONNECT, PNULL);
}

/*****************************************************************************/
// 	Description : StartRecord
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
LOCAL MMIVT_REF_RET_E StartRecord(MMIVT_RECORD_TYPE_E record_type)
{
    MMIVT_REF_RET_E result = MMIVT_REF_RET_SUCCESS;
    MMIFILE_DEVICE_E record_path = 0;
    MMI_STRING_T record_file_str = {0};
    wchar temp_record_file_str[VT_FILE_NAME_LEN_MAX] = {0};
    uint32 file_name_id = 0;
    const wchar* device_name_ptr = PNULL;
    uint16 device_len = 0;
    //MMIFILE_DEVICE_E fmm_dev = MMI_DEVICE_SDCARD;
    
    //SCI_TRACE_LOW:"MMIVT StartRecord record_type:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_UI_6879_112_2_18_3_6_25_208,(uint8*)"d", record_type);
    if (record_type >= MMIVT_RECORD_TYPE_MAX)
    {
        //SCI_TRACE_LOW:"MMIVT StartRecord err type:%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_UI_6882_112_2_18_3_6_25_209,(uint8*)"d", record_type);
        return MMIVT_REF_RET_FAIL;
    }

    if ((MMICC_AUDIO_DEV_BT == MMICC_GetCurrentDev()) 
        &&((MMIVT_RECORD_TYPE_RV_DA == record_type) || (MMIVT_RECORD_TYPE_DA == record_type)))
    {
        //SCI_TRACE_LOW:"MMIVT StartRecord bt in use"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_UI_6889_112_2_18_3_6_25_210,(uint8*)"");
        return MMIVT_REF_RET_BLUETOOTH_IN_USE;
    }

    record_file_str.wstr_ptr = temp_record_file_str;
    record_file_str.wstr_len = VT_FILE_NAME_LEN_MAX;
    
    //获取录制存储位置
    record_path = MMIAPIFMM_GetDefaultDisk();
    if (MMIAPIFMM_GetDeviceTypeStatus(record_path))
    {
        switch(record_type) 
        {
        case MMIVT_RECORD_TYPE_RV_DA:
            record_path = MMIAPISET_GetVTRecordRVDAPath();
        	break;
        case MMIVT_RECORD_TYPE_RV:
            record_path = MMIAPISET_GetVTRecordRVPath();
        	break;
        case MMIVT_RECORD_TYPE_DA:
            record_path = MMIAPISET_GetVTRecordDAPath();
        	break;
        default:
            record_path = MMI_DEVICE_UDISK;
            break;
        }
    }

    //判断存储区域是否有足够空间
    if(SFS_ERROR_NONE != MMIAPIFMM_GetSuitableFileDev(record_path, VT_RECORD_NEED_SPACE_MIN, &record_path))
    {
        //SCI_TRACE_LOW:"mmivt no enough space to record"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_UI_6919_112_2_18_3_6_25_211,(uint8*)"");
        return MMIVT_REF_RET_DISK_FULL;
    }


    //存储位置路径名
    device_name_ptr = MMIAPIFMM_GetDevicePath(record_path);
    device_len = MMIAPIFMM_GetDevicePathLen(record_path); 

    //获得file name
    {
        char temp_file_name[VT_FILE_NAME_LEN_MAX] = {0};
        wchar temp_file_name_str[VT_FILE_NAME_LEN_MAX] = {0};
        MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;
        MMIFILE_HANDLE file_handle = PNULL;
        MMIFILE_ERROR_E file_error = 0;
        uint32 times = 0;
        
        MMINV_READ(MMINV_VT_RECORD_ID, &file_name_id, return_value);
        if (MN_RETURN_SUCCESS != return_value)
        {
            file_name_id = 0;
            MMINV_WRITE(MMINV_VT_RECORD_ID, &file_name_id);        
        }
        else
        {
            file_name_id++;
        }

        do
        {
            if (file_name_id > VT_RECORD_FILE_ID_MAX)
            {
                file_name_id = 0;
            }
            MMIAPIDC_GetFileIDString(temp_file_name, "VT", file_name_id, VT_RECORD_FILE_ID_MAX, "3gp");
            MMIAPICOM_StrToWstr((uint8 *)temp_file_name, temp_file_name_str);

            SCI_MEMSET(temp_record_file_str, 0x0, sizeof(temp_record_file_str));
            record_file_str.wstr_len = VT_FILE_NAME_LEN_MAX;
            MMIAPIFMM_CombineFullPath(device_name_ptr, device_len, MMIMULTIM_DIR_MOVIE, MMIAPICOM_Wstrlen(MMIMULTIM_DIR_MOVIE), 
                temp_file_name_str, (uint16)MMIAPICOM_Wstrlen(temp_file_name_str), temp_record_file_str, &(record_file_str.wstr_len));

            file_error = MMIAPIFMM_CreateFileAsyn(temp_record_file_str, SFS_MODE_WRITE|SFS_MODE_CREATE_NEW, NULL, NULL, 0,0,&file_handle);
            //判断文件名是否有效
            if (0 != file_handle)
            {
                //创建成功，删除文件，退出循环
                MMIAPIFMM_CloseFile(file_handle);
                MMIAPIFMM_DeleteFileSyn(record_file_str.wstr_ptr, record_file_str.wstr_len);
                break;
            }
            else
            {
                times++;
                if (VT_FILE_CREAT_FILE_COUNT_MAX < times)
                {
                    //SCI_TRACE_LOW:"mmivt start record create file failed"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_UI_6976_112_2_18_3_6_25_212,(uint8*)"");
                    MMINV_WRITE(MMINV_VT_RECORD_ID, &file_name_id);
                    return MMIVT_REF_RET_FAIL;
                }
            }
            file_name_id = file_name_id + times;
        }        while (SFS_ERROR_HAS_EXIST == file_error);
        switch(file_error)
        {
            case SFS_ERROR_NO_SPACE:
            case SFS_ERROR_FOLD_FULL:
                return MMIVT_REF_RET_DISK_FULL;

            case SFS_NO_ERROR:
                //do nothing
                break;

            default:
                return MMIVT_REF_RET_FAIL;
        }
        
    }

    result = MMIVT_REF_RecordStart(record_type, &record_file_str);

    if (MMIVT_REF_RET_SUCCESS == result)
    {
        MMIVT_GetAppletInstance()->vt_instance.ui_display_info.is_recording = TRUE;
        //记录上次使用的id号
        MMINV_WRITE(MMINV_VT_RECORD_ID, &file_name_id);
    }
    else
    {
        //do nothing
    }
    return result;
        
}

/*****************************************************************************/
// 	Description : StopRecord
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E StopRecord(void)
{
    MMIVT_APPLET_T *vt_applet_ptr = MMIVT_GetAppletInstance();

    //SCI_TRACE_LOW:"MMIVT StopRecord"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_UI_7023_112_2_18_3_6_25_213,(uint8*)"");
    if (PNULL == vt_applet_ptr)
    {
        //SCI_TRACE_LOW:"mmivt null applet"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_UI_7026_112_2_18_3_6_25_214,(uint8*)"");
        return MMI_RESULT_FALSE;
    }    
    if (!vt_applet_ptr->vt_instance.ui_display_info.is_recording)
    {
        //SCI_TRACE_LOW:"MMIVT not recording"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_UI_7031_112_2_18_3_6_25_215,(uint8*)"");
        return MMI_RESULT_FALSE;
    }
    MMIVT_REF_RecordStop();
    vt_applet_ptr->vt_instance.ui_display_info.is_recording = FALSE;
    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
// 	Description : MMIVT_StopRecord
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIVT_StopRecord(void)
{
    return StopRecord();
}

/*****************************************************************************/
//  Description : ErrorNotifyCallBack
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:底层错误处理函数
/*****************************************************************************/
LOCAL void ErrorNotify(MMIVT_REF_RET_E err_type)
{
    MMI_WIN_PRIORITY_E win_priority = WIN_TWO_LEVEL;
    //SCI_TRACE_LOW:"mmivt errorNotify %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_UI_7055_112_2_18_3_6_26_216,(uint8*)"d", err_type);
    switch(err_type) 
    {
    case MMIVT_REF_RET_DISK_FULL:
        MMIPUB_OpenAlertWinByTextId(PNULL, TXT_COMM_MEMORY_FULL, TXT_COMMON_STOP_RECORD, IMAGE_PUBWIN_WARNING, PNULL, &win_priority, MMIPUB_SOFTKEY_ONE, PNULL);
        MMIVT_StopRecord();
    	break;

    case MMIVT_REF_RET_FILE_FULL:
        MMIPUB_OpenAlertWinByTextId(PNULL, TXT_VT_REF_ERROR_FILE_BIG, TXT_COMMON_STOP_RECORD, IMAGE_PUBWIN_WARNING, PNULL, &win_priority, MMIPUB_SOFTKEY_ONE, PNULL);
        MMIVT_StopRecord();        
    	break;

    case MMIVT_REF_RET_FILE_ERR:
        MMIPUB_OpenAlertWinByTextId(PNULL, TXT_VT_REF_ERROR_FILE_ERROR, TXT_COMMON_STOP_RECORD, IMAGE_PUBWIN_WARNING, PNULL, &win_priority, MMIPUB_SOFTKEY_ONE, PNULL);
        MMIVT_StopRecord();        
    	break;

    case MMIVT_REF_RET_INVALID_IMAGE:
        MMIPUB_OpenAlertWinByTextId(PNULL, TXT_COMMON_NO_SUPPORT, TXT_NULL, IMAGE_PUBWIN_WARNING, PNULL, &win_priority, MMIPUB_SOFTKEY_ONE, PNULL);
        break;

    case MMIVT_REF_RET_SENSOR_ERR:
        MMIPUB_OpenAlertWinByTextId(PNULL, TXT_VT_REF_ERROR_SENSOR_ERR, TXT_NULL, IMAGE_PUBWIN_WARNING, PNULL, &win_priority, MMIPUB_SOFTKEY_ONE, PNULL);
        break;

    case MMIVT_REF_RET_INVALID_PARAM:
        MMIPUB_OpenAlertWinByTextId(PNULL, TXT_VT_REF_ERROR_INVALID_PARAM, TXT_NULL, IMAGE_PUBWIN_WARNING, PNULL, &win_priority, MMIPUB_SOFTKEY_ONE, PNULL);
        break;

    case MMIVT_REF_RET_REPLACE_UDISK_IN_USE:
        MMIPUB_OpenAlertWinByTextId(PNULL, TXT_COMMON_UDISK_USING, TXT_VT_REF_REPLACE_BY_DEFAULT, IMAGE_PUBWIN_WARNING, PNULL, &win_priority, MMIPUB_SOFTKEY_ONE, PNULL);
        break;

    case MMIVT_REF_RET_REPLACE_FILE_NOT_EXIST:
        MMIPUB_OpenAlertWinByTextId(PNULL, TXT_COM_FILE_NO_EXIST, TXT_VT_REF_REPLACE_BY_DEFAULT, IMAGE_PUBWIN_WARNING, PNULL, &win_priority, MMIPUB_SOFTKEY_ONE, PNULL);
        break;

#ifdef BLUETOOTH_SUPPORT        
    case MMIVT_REF_RET_BLUETOOTH_IN_USE:
         MMIPUB_OpenAlertWinByTextId(PNULL,TXT_RECORD_CALL_ALERT,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,&win_priority,MMIPUB_SOFTKEY_ONE,PNULL);
        break;   
#endif

    default:
        //do nothing
        break;
    }
}

/*****************************************************************************/
//  Description : ErrorNotifyCallBack
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:底层错误处理函数
/*****************************************************************************/
PUBLIC void MMIVT_ErrorNotify(MMIVT_REF_RET_E err_type)
{
    ErrorNotify(err_type);
}

/*****************************************************************************/
//  Description : IsAllowVtQuit
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsAllowVtQuit(void)
{
    return (MMIVT_KERNEL_STATE_IDLE == MMIVT_GetCallState());
}

/*****************************************************************************/
//  Description : StartVTBlock
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:
/*****************************************************************************/
LOCAL void StartVTBlock(void)
{
    GUI_LCD_DEV_INFO lcd_dev = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    GUIBLOCK_SetType(255, VT_TRANSPARENT_COLOR, GUIBLOCK_TYPE_OSD, &lcd_dev);
}

/*****************************************************************************/
//  Description : StopVTBlock
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:
/*****************************************************************************/
LOCAL void StopVTBlock(void)
{
    GUIBLOCK_ResetMainType();
}

/*****************************************************************************/
//  Description : DisableGsensorRotate
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:
/*****************************************************************************/
LOCAL void DisableGsensorRotate(MMI_HANDLE_T win_handle, BOOLEAN is_disable_opera)
{
#ifdef MOTION_SENSOR_TYPE
    LCD_ANGLE_E  current_angle = GUILCD_GetLogicAngle((LCD_ID_E)GUI_MAIN_LCD_ID);

    //SCI_TRACE_LOW:"mmivt DisableGsensorRotate set = %d, current_angle = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_UI_7151_112_2_18_3_6_26_217,(uint8*)"dd", is_disable_opera, current_angle);

    MMK_SetWinSupportAngle(win_handle, WIN_SUPPORT_ANGLE_ALL);
    if(is_disable_opera)
    {
        MMK_SetWinAngle( win_handle, LCD_ANGLE_0, FALSE );
        MMK_SetWinSupportAngle(win_handle, WIN_SUPPORT_ANGLE_CUR);
    }
    else
    {
        //do nothing
    }
#endif
}

/*****************************************************************************/
//  Description : MMIAPIVT_GetAudioPlayStatus
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC  BOOLEAN MMIAPIVT_GetAudioPlayResumeStatus(void)
{
    return s_vt_resume_mp3;
}


/*****************************************************************************/
//  Description : MMIAPIVT_SetAudioPlayResumeStatus
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIVT_SetAudioPlayResumeStatus(BOOLEAN  status)
{
     s_vt_resume_mp3 = status ;
}



#endif



/*Edit by script, ignore 6 case. Fri Apr 27 09:38:56 2012*/ //IGNORE9527
