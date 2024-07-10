/*****************************************************************************
** File Name:      mmiphone_wintab.c                                         *
** Author:                                                                   *
** Date:           12/2003                                                   *
** Copyright:      2003 Spreadtrum, Incorporated. All Rights Reserved.         *
** Description:    This file is used to describe call log                    *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 03/2004       Jassmine.Meng     Creat
******************************************************************************/

#define _MMIPHONE_WINTAB_C_

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "mmi_app_phone_trc.h"
#include "window_parse.h"
#include "guilistbox.h"
#include "guilabel.h"
#include "mmk_timer.h"
#include "mmi_image.h"
#include "mmiphone_internal.h"
#include "mmiphone_export.h"
#include "tb_dal.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmidisplay_data.h"
#include "mmiset_export.h"
#include "mmiidle_export.h"
#include "mmi_default.h"
#include "mmicl_export.h"
#include "mmicc_export.h"
#include "mmi_menutable.h"
#ifndef WIN32
#include "umem_main.h"
#endif
#include "guisoftkey.h"
#include "guimsgbox.h"
#include "mmi_appmsg.h"
#include "mmiidle_subwintab.h"
#include "mmimp3_export.h"
#include "mmi_textfun.h"
#include "mmidc_export.h"
#include "mmiudisk_export.h"
#include "mmienvset_export.h"
#include "mmipub.h"
#include "mmiset_image.h"
#include "mmi_anim.h"
#include "mmipb_export.h" 
#include "mmisd_export.h"
#include "mn_api.h"
#ifdef KURO_SUPPORT
#include "mmikur.h"
#endif
#include "mmisms_export.h"
#include "gpio_prod_api.h"
#if defined(MMI_GUI_STYLE_TYPICAL ) || defined(MMI_SIM_LOCK_SUPPORT) || !defined(MMI_PDA_SUPPORT)
#include "guires.h"
#endif
#include "guiedit.h"
#ifdef VIDEO_PLAYER_SUPPORT
#include "mmivp_export.h"
#endif
#include "mmieng_export.h"
#include "guirichtext.h"
#include "mmiset_text.h"
#include "guictrl_api.h"
#ifdef FLASH_SUPPORT
#include "mmiacc_id.h"
#endif
#include "mmiidle_statusbar.h"
#ifdef MMI_WIFI_SUPPORT
#include "mmiwifi_export.h"
#endif

#if defined(BT_MAP_SUPPORT)
#include "mmisms_id.h"
#include "mmisms_app.h"
#include "mmibt_id.h"
#include "mmiset_id.h"
#endif

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
//TEMP 需要根据实际情况调整


#define PHONE_NETWORKSEARCHING_DURING_TIME 	4000    // 以毫秒为单位的值
#define PHONE_RINGING_DURING_TIME 	        7500    // 以毫秒为单位的值 (闹铃时间不超过1分钟，否则有可能有缺陷)

#define PHONE_OPEN_RINGING_SLEEP_TIME 	        150      // 以毫秒为单位的值 (为了让MIDI初始化成功，在开机播放铃声时候应该Sleep,是midi初始化成功)

#define MMIPHONE_EMERGENCY_NUM              10   //紧急呼叫所允许输入的电话号码个数(包括imei等)
#if defined (MAINLCD_SIZE_176X220) || defined (MAINLCD_SIZE_128X160)
#define MMIPHONE_PIN_PUK_REMAIN_TOP_MARGIN  1
#define MMIPHONE_PIN_PUK_REMAIN_HEIGHT      12
#else
#define MMIPHONE_PIN_PUK_REMAIN_TOP_MARGIN  5
#define MMIPHONE_PIN_PUK_REMAIN_HEIGHT      22
#endif






#define MMIPHONE_MAX_PIN_PUK_REMAIN_TIMES 5
#ifdef MAINLCD_SIZE_128X64 //added by taiping.lai 2011/01/03
#define MMIPHONE_PASSWORD_EDIT_LABEL_RECT       {0, 16, 127, 31}  // 编辑输入密码时的提示语区域
#define MMIPHONE_PASSWORD_EDIT_RECT_WITH_LABEL       {0, 32, 127, 47}  // 编辑输入密码时的编辑区域 需要LABEL
#define MMIPHONE_PASSWORD_EDIT_RECT       {0, 24, 127, 40}  // 编辑输入密码时的编辑区域 不需要LABEL
#define MMIPHONE_PASSWORD_TITLE_RECT      {0, 0, 127, 15}; // 编辑输入密码时的标题区域
#endif 

#ifdef MMI_SIM_LOCK_SUPPORT
//define for Countdown timer
#define COUNTDOWN_TIME_STRING_LENGTH               8       // XX:XX:XX
#define COUNTDOWN_MAX_HOUR 23
#define COUNTDOWN_MAX_MINUTE 59
#define COUNTDOWN_TEXT_FONT	SONG_FONT_36

typedef uint8 SIMLOCK_COUNTDOWN_TIME_T[COUNTDOWN_TIME_STRING_LENGTH + 1];
#endif

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/
typedef enum{
    LOSE_FOCUS_BY_ALARM,
        LOSE_FOCUS_BY_NONE
}PHONE_FOCUS_STATUS_E;

/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/




LOCAL MMI_OPERATE_PIN_T   s_operate_pin2_blocked_info;//用于pin2被锁后传递参数


#ifdef FLASH_SUPPORT
LOCAL BOOLEAN s_openIdleFlag = FALSE;//标志变量， 【TRUE】表示正常的开机动画播放完成后没有进入IDLE，其他APP将负责启动IDLE
scrDelCallBackType s_callbackarray[MAX_SUPPORT_SCR_NUMS] = {0};
#endif
#ifdef MMI_SIM_LOCK_SUPPORT
LOCAL SCI_TIME_T s_countdown_set_time = {0,0,0};//进入设置界面时看到的时间，记录为最近设置的时间值 
LOCAL SCI_TIME_T s_countdown_current_time = {0};//启动后记录的倒计时当前时间
LOCAL uint8 s_countdown_time_id = 0;//倒记时的timer
///#ifdef WIN32
LOCAL BOOLEAN s_sim_unlocked = FALSE;//模拟器上是否已经解锁
LOCAL BOOLEAN s_phone_need_restart[MMI_DUAL_SYS_MAX] = {0};//模拟器上是否已经解锁
//#endif
LOCAL BOOLEAN s_is_need_enter_unlock_win = FALSE;//是否需要回到解锁界面
#endif

/**--------------------------------------------------------------------------*
**                         EXTERNAL DECLARE                                 *
**--------------------------------------------------------------------------*/
#ifdef MMI_SIM_LOCK_SUPPORT
extern MN_BLOCK_T   g_pin_value[MMI_DUAL_SYS_MAX];
#endif
/**--------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/
uint32   g_operate_pin_win_tab;
BOOLEAN g_is_operate_pin_special_func = FALSE;

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
// 	Description : 处理PIN INPUT的消息
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePinInputWindow (
                                         MMI_WIN_ID_T     win_id, 	// PIN INPUTING窗口的ID
                                         MMI_MESSAGE_ID_E 	msg_id, 	// 窗口的内部消息ID
                                         DPARAM 		        param		// 相应消息的参数
                                         );

/*****************************************************************************/
// 	Description : PIN输入界面，alert界面\不允许red key,flip返回
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePinAlertWindow(
                                        MMI_WIN_ID_T     win_id, 	// 窗口的ID
                                        MMI_MESSAGE_ID_E 	msg_id, 	// 窗口的内部消息ID
                                        DPARAM 		        param		// 相应消息的参数
                                        );

#ifndef MMI_PDA_SUPPORT
/*****************************************************************************/
// 	Description : 设置输入密码窗口的背景色，image 和title
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL void SetPasswordBackground(
                                 MMI_WIN_ID_T    win_id, //窗口的ID
                                 MMI_CTRL_ID_T   ctrl_id,   //the id of control
                                 BOOLEAN is_need_label
                                 );
#endif

/*****************************************************************************/
// 	Description : 处理输入的PIN码
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL void HandlePinInputingString(
                                   MMI_STRING_T *pin_ptr
                                   );

/*****************************************************************************/
// 	Description : 处理PUK INPUTING的消息
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePukInputWindow (
                                         MMI_WIN_ID_T     win_id, 	// PUK INPUTING窗口的ID
                                         MMI_MESSAGE_ID_E 	msg_id, 	// 窗口的内部消息ID
                                         DPARAM 		        param		// 相应消息的参数
                                         );


/*****************************************************************************/
// 	Description : HandleExitPinBlockedExceptPhone
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL void HandleExitPinBlockedExceptPhone(void);

/*****************************************************************************/
// 	Description : 处理输入的Puk modify pin码
//  **05*PUK*NEW_PIN*NEW_PIN# to change pin   **052*PUK2*NEW_PIN2*NEW_PIN2# to change pin2
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL void HandlePukModifyPin(
                              uint8 *puk_pin_str
                              );

/*****************************************************************************/
// 	Description : PUK输入界面打开时，等待界面不允许返回，pin，new pin界面不允许red key,flip返回
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePukWaitingWindow (
                                           MMI_WIN_ID_T     win_id, 	// 窗口的ID
                                           MMI_MESSAGE_ID_E    msg_id, 	// 窗口的内部消息ID
                                           DPARAM 		        param		// 相应消息的参数
                                           );

/*****************************************************************************/
// 	Description : PUK输入界面打开时，alert界面\不允许red key,flip返回
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePukAlertWindow(
                                        MMI_WIN_ID_T     win_id, 	// 窗口的ID
                                        MMI_MESSAGE_ID_E 	msg_id, 	// 窗口的内部消息ID
                                        DPARAM 		        param		// 相应消息的参数
                                        );

/*****************************************************************************/
// 	Description : 处理new pin INPUTING的消息
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleNewPinInputWindow (
                                            MMI_WIN_ID_T     win_id, 	// new pin input窗口的ID
                                            MMI_MESSAGE_ID_E 	msg_id, 	// 窗口的内部消息ID
                                            DPARAM 		        param		// 相应消息的参数
                                            );

/*****************************************************************************/
// 	Description : 处理new pin again INPUTING的消息
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleNewPinAgainInputWindow (
                                                 MMI_WIN_ID_T    win_id, 	// new pin input窗口的ID
                                                 MMI_MESSAGE_ID_E 	msg_id, 	// 窗口的内部消息ID
                                                 DPARAM 	        param		// 相应消息的参数
                                                 );

/*****************************************************************************/
// 	Description : 处理输入的Puk and new pin码
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL void HandlePukAndNewPin(
                              MMI_STRING_T *puk_ptr,
                              MMI_STRING_T *pin_ptr
                              );

/*****************************************************************************/
// 	Description : pin2 blocked的情况下关闭窗口，则启动输入puk的窗口
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePin2BlockedExceptPhone(
                                                MMI_WIN_ID_T     win_id, 	// Pin boloked窗口的ID
                                                MMI_MESSAGE_ID_E 	msg_id, 	// 窗口的内部消息ID
                                                DPARAM 		        param		// 相应消息的参数
                                                );

/*****************************************************************************/
// 	Description : 处理所有PIN INPUT的消息，除去开关机reponse PIN的情况
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePinInputExceptPhone(
                                             MMI_WIN_ID_T    win_id, 	// PIN2 INPUTING窗口的ID
                                             MMI_MESSAGE_ID_E 	msg_id, 	// 窗口的内部消息ID
                                             DPARAM 		    param		// 相应消息的参数
                                             );

/*****************************************************************************/
// 	Description : 处理输入的PIN码，除去开关机reponse PIN的情况
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL void HandleOperatePin(
                            MMI_OPERATE_PIN_T   *operate_pin_info,
                            MMI_STRING_T       *pin_ptr
                            );

/*****************************************************************************/
// 	Description : 处理输入的PUK码，除去开关机reponse PIN的情况
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePukInputExceptPhone (
                                              MMI_WIN_ID_T   win_id, 	// PUK INPUTING窗口的ID
                                              MMI_MESSAGE_ID_E 	msg_id, 	// 窗口的内部消息ID
                                              DPARAM 		    param		// 相应消息的参数
                                              );

/*****************************************************************************/
// 	Description : 处理输入的Puk modify pin码(开机之后)
//  **05*PUK*NEW_PIN*NEW_PIN# to change pin   **052*PUK2*NEW_PIN2*NEW_PIN2# to change pin2
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL void HandlePukModifyPinExceptPhone(
                                         uint8                      *puk_pin_str,
                                         MNSIM_PIN_INDICATION_E     pin_num
                                         );

/*****************************************************************************/
// 	Description : PUK输入界面打开时，alert界面\不允许red key,flip返回(除去开关机reponse PIN的情况)
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePukAlertWindowExceptPhone(
                                                   MMI_WIN_ID_T     win_id, 	// 窗口的ID
                                                   MMI_MESSAGE_ID_E 	msg_id, 	// 窗口的内部消息ID
                                                   DPARAM 		        param		// 相应消息的参数
                                                   );

/*****************************************************************************/
// 	Description : 处理new pin INPUTING的消息,除去开关机reponse PIN的情况
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleNewPinInputExceptPhone (
                                                 MMI_WIN_ID_T    win_id, 	// new pin input窗口的ID
                                                 MMI_MESSAGE_ID_E 	msg_id, 	// 窗口的内部消息ID
                                                 DPARAM 		    param		// 相应消息的参数
                                                 );

/*****************************************************************************/
// 	Description : 处理new pin again INPUTING的消息,除去开关机reponse PIN的情况
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleNewPinAgainInputExceptPhone (
                                                      MMI_WIN_ID_T   win_id, 	// new pin input窗口的ID
                                                      MMI_MESSAGE_ID_E 	msg_id, 	// 窗口的内部消息ID
                                                      DPARAM            param		// 相应消息的参数
                                                      );

/*****************************************************************************/
// 	Description : handle display imei when no sim or sim error
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDisplayImei (
                                      MMI_WIN_ID_T   win_id, 	// 窗口的ID
                                      MMI_MESSAGE_ID_E 	msg_id, 	// 窗口的内部消息ID
                                      DPARAM            param		// 相应消息的参数
                                      );

/*****************************************************************************/
//  Description : Append IMEI Detail
//  Global resource dependence : none
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL void AppendIMEIDetail(MMI_CTRL_ID_T ctrl_id);

/*****************************************************************************/
// 	Description : 处理输入的Puk and new pin码，除去开关机reponse PIN的情况
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL void HandleOperatePukAndNewPin(
                                     MMI_OPERATE_PIN_T  *operate_pin_info,
                                     MMI_STRING_T      *puk_ptr,
                                     MMI_STRING_T      *pin_ptr
                                     );

/*****************************************************************************/
// 	Description : PIN修改PIN，除去开关机reponse PIN的情况
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL void HandleOperatePinAndNewPin(
                                     MMI_OPERATE_PIN_T  *operate_pin_info,
                                     MMI_STRING_T      *old_pin_ptr,
                                     MMI_STRING_T      *new_pin_ptr
                                     );





/*****************************************************************************/
// 	Description : HandlePoweroffQueryWin
//	Global resource dependence :                                
//      Author: wancan.you
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePoweroffQueryWin(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);



/*****************************************************************************/
// 	Description : GetPinPukEditRect
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL GUI_BOTH_RECT_T GetPinPukEditRect(MMI_WIN_ID_T win_id, BOOLEAN is_need_label);

/*****************************************************************************/
// 	Description : PIN/PUK input window softkey
//	Global resource dependence : 
//  Author: Jassmine
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN PinPukInputWinSoftkey(
                                      wchar         *str_ptr,   //in
                                      uint16        str_len,    //in
                                      MMI_TEXT_ID_T *text_id_ptr//in/out:
                                      );

/*****************************************************************************/
// 	Description : Make Emergency Call
//	Global resource dependence : 
//  Author: Jassmine
//	Note: 
/*****************************************************************************/
LOCAL void MakeEmergencyCall(MMI_STRING_T *str_info);

#ifdef MMI_SIM_LOCK_SUPPORT
/*****************************************************************************/
// 	Description : SIM Lock的情况下输入密码窗口
//	Global resource dependence : 
//  Author: dave.ruan
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSIMLockedPasswordInuptWin(
                                                MMI_WIN_ID_T     win_id, 	// Pin boloked窗口的ID
                                                MMI_MESSAGE_ID_E 	msg_id, 	// 窗口的内部消息ID
                                                DPARAM 		        param		// 相应消息的参数
                                                );

/*****************************************************************************/
// 	Description : GetPinPukEditRect
//	Global resource dependence : 
//  Author: dave.ruan
//	Note:
/*****************************************************************************/
LOCAL GUI_BOTH_RECT_T GetSIMUnlockEditRect(MMI_WIN_ID_T win_id, BOOLEAN is_need_label);

/*****************************************************************************/
// 	Description : Get SIM lock Remain string
//	Global resource dependence : 
//  Author: dave.ruan
//	Note:
/*****************************************************************************/
LOCAL void GetSIMLockRemainStr(MN_DUAL_SYS_E dual_sys,
                                MMI_STRING_T *sim_lock_str_ptr,
                                uint32 unlock_type      //IN: Network/NetworkSubset/SP/CORPORATE/USER
                                );
                                
/*****************************************************************************/
// 	Description : Get SIM lock input Password Title
//	Global resource dependence : 
//  Author: dave.ruan
//	Note:
/*****************************************************************************/
LOCAL void GetSIMLockInputPasswordTitle(
                                uint32 unlock_type,      //IN: Network/NetworkSubset/SP/CORPORATE/USER
                                MMI_TEXT_ID_T *unlock_title      //out: sim unlock title
                                );

/*****************************************************************************/
// 	Description : SIM unlock 密码输入界面，alert界面\不允许red key,flip返回
//	Global resource dependence : 
//  Author: dave.ruan
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSIMUnlockingAlertWindow(
                                        MMI_WIN_ID_T     win_id, 	// 窗口的ID
                                        MMI_MESSAGE_ID_E 	msg_id, 	// 窗口的内部消息ID
                                        DPARAM 		        param		// 相应消息的参数
                                        );
 
/*****************************************************************************/
// 	Description : SIM unlock input window softkey
//	Global resource dependence : 
//  Author: dave.ruan
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN SIMUnlockInputWinSoftkey(
                                      wchar         *str_ptr,   //in
                                      uint16        str_len,    //in
                                      MMI_TEXT_ID_T *text_id_ptr//in/out:
                                      );
                                      
/*****************************************************************************/
// 	Description : SIM unlock 密码输入界面，输入错误提示handle函数
//	Global resource dependence : 
//  Author: dave.ruan
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSIMUnlockingAlertErrorWindow(
                                        MMI_WIN_ID_T     win_id, 	// 窗口的ID
                                        MMI_MESSAGE_ID_E 	msg_id, 	// 窗口的内部消息ID
                                        DPARAM 		        param		// 相应消息的参数
                                        );

/*****************************************************************************/
// 	Description : 处理输入的sim lockpassword
//	Global resource dependence : 
//  Author: dave.ruan
//	Note:
/*****************************************************************************/
LOCAL void HandleSIMUnlockingInputingString(
                                                MMI_OPERATE_SIM_LOCK_T *operate_sim_lock_ptr,    //是否为解锁
                                                MMI_STRING_T *sim_unlocking_ptr
                                               );

 /*****************************************************************************/
// 	Description : 在SIM Lock 解锁次数达到限制的情况下弹出"SIM Restricted"窗口
//	Global resource dependence : 
//  Author: dave.ruan
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSIMLocked(
                                    MMI_WIN_ID_T     win_id, 	// Pin boloked窗口的ID
                                    MMI_MESSAGE_ID_E 	msg_id, 	// 窗口的内部消息ID
                                    DPARAM 		        param		// 相应消息的参数
                                    );
 
 /*****************************************************************************/
// 	Description : SIM unlock输入界面打开时，等待界面不允许返回,界面不允许red key,flip返回
//	Global resource dependence : 
//  Author: dave.ruan
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSIMUnlockingWaitingWindow (
                                           MMI_WIN_ID_T      win_id, 	// 窗口的ID
                                           MMI_MESSAGE_ID_E     msg_id, 	// 窗口的内部消息ID
                                           DPARAM 		        param		// 相应消息的参数
                                           );

/*****************************************************************************/
//  Description : the process message function of the Countdown running win
//  Global resource dependence : 
//  Author:        dave.ruan
//  Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleCountdownRunningWinMsg(
                                                                MMI_WIN_ID_T win_id,
                                                                MMI_MESSAGE_ID_E msg_id,
                                                                DPARAM param
                                                            );

/*****************************************************************************/
//  Description : when pen ok, handle the message
//  Global resource dependence : 
//  Author:   dave.ruan
//  Note:   
/*****************************************************************************/
LOCAL void DisplayCountdownBG(MMI_WIN_ID_T win_id);

/*****************************************************************************/
//  Description : display the background pic of time running
//  Global resource dependence : 
//  Author:        dave.ruan
//  Note:   
/*****************************************************************************/
LOCAL void DisplayCountdownTimeBg(MMI_WIN_ID_T win_id);

/*****************************************************************************/
//  Description : 设置输入的时间为倒记时开始的时间
//  Global resource dependence : 
//  Author: dave.ruan
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SetCountdownStartTime(uint32 unlock_type);

/*****************************************************************************/
//  Description : get curr time
//  Global resource dependence : none
//  Author: dave.ruan
//  Note:
/*****************************************************************************/
LOCAL SIMLOCK_COUNTDOWN_TIME_T *GetCurCountdownTime( void );

/*****************************************************************************/
//  Description : Display Cur Countdown Time
//  Global resource dependence : none
//  Author: dave.ruan
//  Note:
/*****************************************************************************/
LOCAL void DisplayCurCountdownTime(void);

/*****************************************************************************/
//  Description : Get SIM Lock Trial Counter
//  Global resource dependence : none
//  Author: dave.ruan
//  Note:
/*****************************************************************************/
LOCAL uint32 GetSIMLockTrialCounter(uint32 simlock_type);

/*****************************************************************************/
//  Description : handle counted time timer over msg
//  Global resource dependence : none
//  Author: dave.ruan
//  Note:
/*****************************************************************************/
LOCAL void CountdownHandleTimer(uint8 timer_id, uint32 param);

/*****************************************************************************/
//  Description : Send Msg to Disp Countdown
//  Global resource dependence : 
//  Author:       dave.ruan
//  RETRUN:  
//  Note:    
/*****************************************************************************/
LOCAL BOOLEAN SendMsgtoDispCountdown(void);
/*****************************************************************************/
// 	Description : 处理所有SIM Lock INPUT的消息，除去开关机
//	Global resource dependence : 
//  Author: dave.ruan
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSIMLockInputExceptPhone(
                                             MMI_WIN_ID_T    win_id, 	// PIN2 INPUTING窗口的ID
                                             MMI_MESSAGE_ID_E 	msg_id, 	// 窗口的内部消息ID
                                             DPARAM 		    param		// 相应消息的参数
                                             );

/*****************************************************************************/
//     Description : 处理倒计时界面SOS拨号的窗口
//    Global resource dependence : 
//  Author: dave.ruan
//    Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleCountDownSOSWindow(
                                                     MMI_WIN_ID_T        win_id, //IN: 窗口的ID
                                                     MMI_MESSAGE_ID_E     msg_id, //IN: 窗口的内部消息ID
                                                     DPARAM                 param    //IN: 相应消息的参数
                                                     );

/*****************************************************************************/
// 	Description :
//	Global resource dependence :                                
//      Author: dave.ruan
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSIMLockShutoffWinMsg(
                                           MMI_WIN_ID_T win_id, 
                                           MMI_MESSAGE_ID_E msg_id, 
                                           DPARAM param
                                           );

/*****************************************************************************/
// 	Description : Handle SIM Lock Alert Win Msg
//	Global resource dependence : 
//  Author: dave.ruan
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSIMLockedAlertWinMsg(
                                              MMI_WIN_ID_T win_id,
                                              MMI_MESSAGE_ID_E 	msg_id,
                                              DPARAM param
                                              );
                                              
#endif                                           
/**--------------------------------------------------------------------------*
**                         CONSTANT VARIABLES                               *
**--------------------------------------------------------------------------*/


// the window for input pin,startup
WINDOW_TABLE( PHONE_PIN_INPUTING_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandlePinInputWindow ),    
    WIN_ID( PHONE_PIN_INPUT_WIN_ID ),
    WIN_HIDE_STATUS,
    WIN_TITLE(TXT_NULL),
    CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE,MMIPHONE_PIN_REMAIN_LABEL_CTRL_ID),
    CREATE_EDIT_PASSWORD_CTRL(MMI_PIN_MAX_LENGTH,PHONE_PIN_EDITBOX_CTRL_ID),
    WIN_STYLE(WS_DISABLE_HWICON | WS_DISABLE_RETURN_WIN),
#ifdef MMI_PDA_SUPPORT
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, TXT_NULL),                        
#else
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),                        
#endif
    END_WIN
};

// the window for input puk,startup
WINDOW_TABLE( PHONE_PUK_INPUTING_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandlePukInputWindow ),    
    WIN_ID( PHONE_PUK_INPUT_WIN_ID ),
    WIN_HIDE_STATUS,
    WIN_TITLE(TXT_NULL),
    CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE,MMIPHONE_PUK_REMAIN_LABEL_CTRL_ID),
    CREATE_EDIT_PASSWORD_CTRL(MMI_PIN_MAX_LENGTH,PHONE_PUK_EDITBOX_CTRL_ID),
    WIN_STYLE(WS_DISABLE_HWICON | WS_DISABLE_RETURN_WIN),
#ifdef MMI_PDA_SUPPORT
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, TXT_NULL),                        
#else
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),                        
#endif
    END_WIN
};

// the window for input new pin,startup
WINDOW_TABLE( PHONE_NEW_PIN_INPUTING_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleNewPinInputWindow ),    
    WIN_ID( PHONE_NEW_PIN_INPUT_WIN_ID ),
    WIN_HIDE_STATUS,
    WIN_TITLE( TXT_ENTER_NEW_PIN ),
    CREATE_EDIT_PASSWORD_CTRL(MMI_PIN_MAX_LENGTH,PHONE_NEW_PIN_EDITBOX_CTRL_ID),
    WIN_STYLE(WS_DISABLE_HWICON | WS_DISABLE_RETURN_WIN),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),                    
    END_WIN
};

// the window for input new pin again,startup
WINDOW_TABLE( PHONE_NEW_PIN_AGAIN_INPUTING_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleNewPinAgainInputWindow ),    
    WIN_ID( PHONE_NEW_PIN_AGAIN_INPUT_WIN_ID ),
    WIN_HIDE_STATUS,
    WIN_TITLE( TXT_ENTER_PIN_CNF ),
    CREATE_EDIT_PASSWORD_CTRL(MMI_PIN_MAX_LENGTH,PHONE_NEW_PIN_AGAIN_EDITBOX_CTRL_ID),
    WIN_STYLE(WS_DISABLE_HWICON | WS_DISABLE_RETURN_WIN),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};

// the window for input pin
WINDOW_TABLE( MMI_PIN_INPUTING_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandlePinInputExceptPhone ),    
    WIN_ID( MMI_PIN_INPUTING_WIN_ID ),
    WIN_HIDE_STATUS,
    WIN_TITLE(TXT_NULL),
    CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE,MMIPHONE_PIN_INPUT_REMAIN_LABEL_CTRL_ID),
    CREATE_EDIT_PASSWORD_CTRL(MMI_PIN_MAX_LENGTH,MMI_PIN_INPUT_EDITBOX_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};

// the window for input puk
WINDOW_TABLE( MMI_PUK_INPUTING_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandlePukInputExceptPhone ),    
    WIN_ID( MMI_PUK_INPUTING_WIN_ID ),
    WIN_HIDE_STATUS,
    WIN_TITLE(TXT_NULL),
    CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE,MMIPHONE_PUK_INPUT_REMAIN_LABEL_CTRL_ID),
    CREATE_EDIT_PASSWORD_CTRL(MMI_PIN_MAX_LENGTH,MMI_PUK_INPUT_EDITBOX_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};

// the window for input new pin
WINDOW_TABLE( MMI_NEW_PIN_INPUTING_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleNewPinInputExceptPhone ),    
    WIN_ID( MMI_NEW_PIN_INPUTING_WIN_ID ),
    WIN_TITLE(TXT_NULL),
    CREATE_EDIT_PASSWORD_CTRL(MMI_PIN_MAX_LENGTH,MMI_NEW_PIN_INPUT_EDITBOX_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};

// the window for input new pin again
WINDOW_TABLE( MMI_NEW_PIN_AGAIN_INPUTING_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleNewPinAgainInputExceptPhone ),    
    WIN_ID( MMI_NEW_PIN_AGAIN_INPUTING_WIN_ID ),
    WIN_TITLE(TXT_NULL),
    CREATE_EDIT_PASSWORD_CTRL(MMI_PIN_MAX_LENGTH,MMI_NEW_PIN_AGAIN_INPUT_EDITBOX_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN), 
    END_WIN
};

// the window for display imei when no sim or sim error
WINDOW_TABLE( MMIPHONE_DISPLAY_IMEI_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleDisplayImei ),
    WIN_ID( MMIPHONE_DISPLAY_IMEI_WIN_ID ),
    WIN_HIDE_STATUS,    
    WIN_TITLE(TXT_PHONE_IMEI),
#ifndef ADULT_WATCH_SUPPORT //bug1586158
    WIN_BACKGROUND_ID(IMAGE_COMMON_BG),
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_RETURN),
#endif
    CREATE_RICHTEXT_CTRL(MMIPHONE_DISPLAY_IMEI_CTRL_ID),
    END_WIN
};

#ifdef MMI_SIM_LOCK_SUPPORT
// the window for input SIM Lock Password,startup
WINDOW_TABLE(SIM_LOCK_PASSWORD_INPUTING_WIN_TAB) = 
{
    //WIN_PRIO(WIN_LOWEST_LEVEL),
    WIN_FUNC( (uint32)HandleSIMLockedPasswordInuptWin),    
    WIN_ID( PHONE_SIM_LOCK_PASSWORD_INPUTING_WIN_ID),
    WIN_TITLE(TXT_NULL),
    WIN_HIDE_STATUS,
    CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE,MMIPHONE_SIM_LOCK_PASSWORD_LABEL_CTRL_ID),
    CREATE_EDIT_PASSWORD_CTRL(SIM_LOCK_KEY_MAX_LEN,PHONE_SIM_LOCK_PASSWORD_EDITBOX_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};

WINDOW_TABLE(SIM_LOCK_COUNTDOWN_RUN_WIN_TAB) = 
{
    WIN_FUNC((uint32) HandleCountdownRunningWinMsg),    
    WIN_ID(PHONE_SIM_LOCK_COUNTDOWN_WIN_ID),
    WIN_HIDE_STATUS,
    WIN_TITLE(TXT_NULL),//SIM unlock倒记时
    WIN_SOFTKEY(TXT_SOS, TXT_NULL, TXT_NULL),
    END_WIN
};

// the window for input SIM Lock in setting
WINDOW_TABLE( MMI_SIM_LOCK_INPUTING_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32)HandleSIMLockInputExceptPhone),    
    WIN_ID( MMI_SIM_LOCK_PASSWORD_INPUTING_WIN_ID ),
    WIN_HIDE_STATUS,
    WIN_TITLE(TXT_NULL),
    CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE,MMIPHONE_SIM_LOCK_INPUT_REMAIN_LABEL_CTRL_ID),
    CREATE_EDIT_PASSWORD_CTRL(SIM_LOCK_KEY_MAX_LEN,MMI_SIM_LOCK_PASSWORD_EDITBOX_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};

WINDOW_TABLE(SIM_LOCK_COUNTDOWN_SOS_WIN_TAB ) = 
{
    //WIN_PRIO(WIN_THREE_LEVEL),
    WIN_FUNC((uint32)  HandleCountDownSOSWindow),  
    WIN_HIDE_STATUS,
    WIN_TITLE(TXT_SOS),
    WIN_ID( PHONE_SIM_LOCK_SOS_WIN_ID ),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_EDIT_DIGITAL_CTRL(SIM_LOCK_KEY_MAX_LEN,MMI_SIM_LOCK_SOS_EDITBOX_CTRL_ID),
    END_WIN
};
WINDOW_TABLE( MMI_SIM_LOCKED_SHUTOFF_WIN_TAB) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleSIMLockShutoffWinMsg), 
    WIN_ID( MMIPHONE_SIMLOCK_SHUTOFF_WIN_ID),
    WIN_HIDE_STATUS,
    WIN_TITLE(TXT_SIM_LOCKED),
    END_WIN
};

#endif
/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/
#ifdef MAINLCD_SIZE_128X64 //added by taiping.lai 2011/01/03
/*****************************************************************/
//  Description :获取密码编辑LABEL 提示区域
//  Global resource dependence : none
//  Author: Taiping Lai
//  Note: 
/*****************************************************************/
LOCAL GUI_BOTH_RECT_T GetPasswordLabelBothRect(void)
{
	GUI_BOTH_RECT_T both_rect = {0};
	CAF_RECT_T label_rect = MMIPHONE_PASSWORD_EDIT_LABEL_RECT;

	both_rect.h_rect =	 label_rect;
	both_rect.v_rect =	 label_rect;
	return both_rect;
}
/*****************************************************************/
//  Description :获取密码编辑框区域
//  Global resource dependence : none
//  Author: Taiping Lai
//  Note: 需要label 时编辑区域在第三行 否则在第二三行
/*****************************************************************/
LOCAL GUI_BOTH_RECT_T GetPasswordEditBothRect(BOOLEAN is_need_label)
{
	GUI_BOTH_RECT_T both_rect = {0};
	CAF_RECT_T edit_rect_with_label = MMIPHONE_PASSWORD_EDIT_RECT_WITH_LABEL;
	CAF_RECT_T edit_rect = MMIPHONE_PASSWORD_EDIT_RECT;

	if (is_need_label)
	{
		both_rect.h_rect =	 edit_rect_with_label;
		both_rect.v_rect =	 edit_rect_with_label;
	}
	else
	{
		both_rect.h_rect =	 edit_rect;
		both_rect.v_rect =	 edit_rect;
	}
	return both_rect;
}
/*****************************************************************/
//  Description :获取密码编辑框标题区域
//  Global resource dependence : none
//  Author: Taiping Lai
//  Note: 
/*****************************************************************/
LOCAL GUI_RECT_T GetPasswordTitleRect(void)
{
	GUI_RECT_T title_rect = MMIPHONE_PASSWORD_TITLE_RECT;
	
	return title_rect;
}
#endif 


/*****************************************************************************/
// 	Description : 处理PIN INPUT的消息
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePinInputWindow (
                                         MMI_WIN_ID_T     win_id, 	// PIN INPUTING窗口的ID
                                         MMI_MESSAGE_ID_E 	msg_id, 	// 窗口的内部消息ID
                                         DPARAM 		        param		// 相应消息的参数
                                         )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    MMI_STRING_T pin_value = {0};   
    MMI_STRING_T pin_puk_remain_str = {0};
    GUI_BOTH_RECT_T edit_both_rect = {0};
    BOOLEAN is_input_emergency_call = FALSE;
    uint16 emergency_call_len = 0;
    
    #ifndef MMI_MULTI_SIM_SYS_SINGLE
    MMI_TEXT_ID_T text_id[] =
        {
            TXT_ENTER_SIM1_PIN,
            TXT_ENTER_SIM2_PIN,
            #if defined(MMI_MULTI_SIM_SYS_TRI)||defined(MMI_MULTI_SIM_SYS_QUAD)
            TXT_ENTER_SIM3_PIN,
            #endif
            #if defined(MMI_MULTI_SIM_SYS_QUAD)
            TXT_ENTER_SIM4_PIN,
            #endif
        };
    uint32 i = MN_DUAL_SYS_1;    
    #endif

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMIDEFAULT_AllowOpenKeylocWin(FALSE);
#ifdef MAINLCD_SIZE_128X64 //added by taiping.lai 2011/01/03
        {
            GUI_BOTH_RECT_T both_rect = GetPasswordLabelBothRect();
		 	  GUI_RECT_T title_rect = GetPasswordTitleRect();

		     GUIWIN_SetTitleRect(win_id, title_rect);	
            GUIAPICTRL_SetBothRect(MMIPHONE_PIN_INPUT_REMAIN_LABEL_CTRL_ID, &both_rect);
		}
#else					
        {
            GUI_BOTH_RECT_T both_rect = MMITHEME_GetWinClientBothRect(win_id);

            both_rect.h_rect.top += MMIPHONE_PIN_PUK_REMAIN_TOP_MARGIN;
            both_rect.v_rect.top += MMIPHONE_PIN_PUK_REMAIN_TOP_MARGIN;
            both_rect.h_rect.bottom = both_rect.h_rect.top + MMIPHONE_PIN_PUK_REMAIN_HEIGHT;
            both_rect.v_rect.bottom = both_rect.v_rect.top + MMIPHONE_PIN_PUK_REMAIN_HEIGHT;

            GUIAPICTRL_SetBothRect(MMIPHONE_PIN_REMAIN_LABEL_CTRL_ID, &both_rect);
        }
#endif
        IGUICTRL_SetState(MMK_GetCtrlPtr(MMIPHONE_PIN_REMAIN_LABEL_CTRL_ID), GUICTRL_STATE_DISABLE_ACTIVE, TRUE);

        edit_both_rect = GetPinPukEditRect(win_id, TRUE);
        GUIAPICTRL_SetBothRect(PHONE_PIN_EDITBOX_CTRL_ID, &edit_both_rect);

        GUIEDIT_SetPasswordStyle(PHONE_PIN_EDITBOX_CTRL_ID,GUIEDIT_PASSWORD_STYLE_DIGITAL_PIN);
        
#ifdef MMI_MULTI_SIM_SYS_SINGLE
        GUIWIN_SetTitleTextId(win_id,TXT_ENTER_PIN,FALSE);
#else
        i = MMIAPIPHONE_GetCurResponsePinSimID();

        if( i < MMI_DUAL_SYS_MAX )
        {
            GUIWIN_SetTitleTextId(win_id,text_id[i],FALSE);
        }
        else
        {
            GUIWIN_SetTitleTextId(win_id,TXT_ENTER_PIN,FALSE);
        }
#endif

        GUIEDIT_SetSoftkey(PHONE_PIN_EDITBOX_CTRL_ID, 0, MMI_PIN_MIN_LENGTH,TXT_NULL,TXT_COMMON_OK, PinPukInputWinSoftkey);

#if defined(MMI_PDA_SUPPORT) && defined(MMIPUBWIN_PDASTYLE_SUPPORT)
        // PUBEDIT负责softkey的显示，PDA下需要禁止右键
        GUIEDIT_SetSoftkey(PHONE_PIN_EDITBOX_CTRL_ID, 2, 0,TXT_NULL,TXT_NULL, PNULL);
#endif

        SCI_MEMSET(&pin_puk_remain_str, 0x00, sizeof(MMI_STRING_T));
        MMIPHONE_GetPinPukRemainStr(MMIAPIPHONE_GetCurResponsePinSimID(), &pin_puk_remain_str, MNSIM_PIN1, FALSE);

        if (PNULL != pin_puk_remain_str.wstr_ptr)
        {
            GUILABEL_SetText(MMIPHONE_PIN_REMAIN_LABEL_CTRL_ID,&pin_puk_remain_str,FALSE);

            SCI_FREE(pin_puk_remain_str.wstr_ptr);
            pin_puk_remain_str.wstr_ptr = PNULL;
        }

        MMK_SetWinDisplayStyleState(win_id, WS_DISABLE_RETURN_WIN, TRUE);
        
        MMK_SetAtvCtrl(win_id,PHONE_PIN_EDITBOX_CTRL_ID);
        break;

#if 0        
#ifdef MMI_PDA_SUPPORT
    case MSG_NOTIFY_EDITBOX_UPDATE_STRNUM:
        {
            MMI_STRING_T str_info = {0};

            GUIEDIT_GetString(PHONE_PIN_EDITBOX_CTRL_ID, &str_info);	
        
            is_input_emergency_call = MMIAPICC_IsInputEmergencyCall(&str_info, &emergency_call_len, MMIAPIPHONE_GetCurResponsePinSimID());
            if ((is_input_emergency_call) &&
                (str_info.wstr_len == emergency_call_len))
            {    
                MMIPUB_SetWinSoftkey(win_id, TXT_SOS, TXT_NULL,TRUE);
            }
            else
            {
                MMIPUB_SetWinSoftkey(win_id, TXT_COMMON_OK, TXT_NULL,TRUE);
            }
        }
        break;
#endif
#endif

    case MSG_FULL_PAINT:
#ifndef MMI_PDA_SUPPORT
        SetPasswordBackground(win_id,PHONE_PIN_EDITBOX_CTRL_ID,TRUE);
#endif
#ifndef SUBLCD_SIZE_NONE       
        //刷新小屏的显示，输入PIN码
        MMISUB_IsPermitUpdate(TRUE);
        MMISUB_SetSubLcdDisplay(FALSE,TRUE,SUB_CONTENT_PIN,PNULL);
#endif
        break;
        
    case MSG_GET_FOCUS:
        MMIDEFAULT_AllowOpenKeylocWin(FALSE);
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_APP_GREEN:
    case MSG_APP_CALL2:
    case MSG_APP_CALL3:
    case MSG_APP_CALL4:
        // get the value about PIN
        GUIEDIT_GetString(PHONE_PIN_EDITBOX_CTRL_ID,&pin_value);

        is_input_emergency_call = MMIAPICC_IsInputEmergencyCall(&pin_value, &emergency_call_len, MMIAPIPHONE_GetCurResponsePinSimID());

        SCI_TRACE_LOW(" HandlePinInputWindow is_input_emergency_call = %d, pin_value.wstr_len = %d, emergency_call_len = %d",is_input_emergency_call,pin_value.wstr_len,emergency_call_len);

        if ((is_input_emergency_call) &&
            (pin_value.wstr_len == emergency_call_len))
        {
            MakeEmergencyCall(&pin_value);
        }
        else if (PIN_LEN_IS_IN_RANGE(pin_value.wstr_len) 
                     && MSG_APP_GREEN != msg_id 
                     && MSG_APP_CALL2 != msg_id
                     && MSG_APP_CALL3 != msg_id
                     && MSG_APP_CALL4 != msg_id)
        {
            HandlePinInputingString(&pin_value);
        }
        break;
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
    case MSG_APP_RED://red and flip不允许返回
        MMIPUB_OpenAlertWinByTextId(PNULL,TXT_PLS_INPUT_PIN,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,HandlePinAlertWindow);
        break;

#ifdef FLIP_PHONE_SUPPORT  //close flip ring codes, just open for flip phone.        
    case MSG_APP_FLIP:
        MMIDEFAULT_HandleFlipKey(FALSE);
        break;
#endif
        
    case MSG_CLOSE_WINDOW:
        MMIDEFAULT_AllowOpenKeylocWin(TRUE);
#ifndef SUBLCD_SIZE_NONE
        //刷新小屏的显示，关闭输入PIN码
        MMISUB_SetSubLcdDisplay(FALSE,FALSE,SUB_CONTENT_PIN,PNULL);
#endif
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return (result);
}

/*****************************************************************************/
// 	Description : PIN输入界面，alert界面\不允许red key,flip返回
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePinAlertWindow(
                                        MMI_WIN_ID_T     win_id, 	// 窗口的ID
                                        MMI_MESSAGE_ID_E 	msg_id, 	// 窗口的内部消息ID
                                        DPARAM 		        param		// 相应消息的参数
                                        )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    
    switch (msg_id)
    {
    case MSG_APP_RED://red and flip不允许返回
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;

#ifdef FLIP_PHONE_SUPPORT  //close flip ring codes, just open for flip phone.
    case MSG_APP_FLIP:
        MMK_CloseWin(win_id);
        MMIDEFAULT_HandleFlipKey(FALSE);
        break;
#endif        

    default:
        result = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
        break;
    }
    
    return (result);
}

/*****************************************************************************/
// 	Description : GetPinPukEditRect
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL GUI_BOTH_RECT_T GetPinPukEditRect(MMI_WIN_ID_T win_id, BOOLEAN is_need_label)
{
#ifdef MAINLCD_SIZE_128X64 //added by taiping.lai 2011/01/03
	return GetPasswordEditBothRect(is_need_label);
#else
    GUI_BOTH_RECT_T both_rect = MMITHEME_GetWinClientBothRect(win_id);

    both_rect.h_rect.left = both_rect.v_rect.left = MMI_PASSWORD_EDIT_LEFT;
    if (is_need_label)
    {
        both_rect.h_rect.top += MMIPHONE_PIN_PUK_REMAIN_TOP_MARGIN + MMIPHONE_PIN_PUK_REMAIN_HEIGHT + MMI_PASSWORD_EDIT_TOP_MARGIN;
        both_rect.v_rect.top += MMIPHONE_PIN_PUK_REMAIN_TOP_MARGIN + MMIPHONE_PIN_PUK_REMAIN_HEIGHT + MMI_PASSWORD_EDIT_TOP_MARGIN;
    }
    else
    {
        both_rect.h_rect.top += MMI_PASSWORD_EDIT_TOP_MARGIN;
        both_rect.v_rect.top += MMI_PASSWORD_EDIT_TOP_MARGIN;
    }

    both_rect.h_rect.bottom = both_rect.h_rect.top + MMI_PASSWORD_EDIT_HEIGHT;
    both_rect.h_rect.right = both_rect.h_rect.right - MMI_PASSWORD_EDIT_LEFT;
    both_rect.v_rect.bottom = both_rect.v_rect.top + MMI_PASSWORD_EDIT_HEIGHT;
    both_rect.v_rect.right = both_rect.v_rect.right - MMI_PASSWORD_EDIT_LEFT;

    return both_rect;
#endif 
		
}

#ifndef MMI_PDA_SUPPORT
/*****************************************************************************/
// 	Description : 设置输入密码窗口的背景色，image 和title
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL void SetPasswordBackground(
                                 MMI_WIN_ID_T    win_id, //窗口的ID
                                 MMI_CTRL_ID_T   ctrl_id,   //the id of control
                                 BOOLEAN is_need_label
                                 )
{
    GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    GUI_RECT_T bg_rect = MMITHEME_GetClientRect();
    GUI_BOTH_RECT_T  both_client_rect = MMITHEME_GetWinClientBothRect(win_id);
#ifdef MONOCHROME_LCD_SUPPORT 
#else
    GUI_POINT_T dis_point = {0};
    uint16 icon_width = 0;
    uint16 icon_height = 0;	
    //GUI_BOTH_RECT_T  both_rect = {0};
    GUI_BOTH_RECT_T edit_both_rect = {0};
#endif 
    
    lcd_dev_info.lcd_id = GUI_MAIN_LCD_ID;
    lcd_dev_info.block_id = GUI_BLOCK_MAIN;
    
    if(MMITHEME_IsMainScreenLandscape())
    {
        bg_rect = both_client_rect.h_rect;
    }
    else
    {
        bg_rect = both_client_rect.v_rect;
    }

#ifdef MONOCHROME_LCD_SUPPORT 
    LCD_FillRect(&lcd_dev_info, bg_rect, MMI_WHITE_COLOR);
#else
    LCD_FillRect(&lcd_dev_info, bg_rect, MMI_BLACK_COLOR);
    GUIRES_GetImgWidthHeight(&icon_width, &icon_height, IMAGE_NUMBERINPUT_PIN, win_id);

    //设置image id
    IMG_EnableTransparentColor(TRUE);
    GUIRES_DisplayImg(PNULL,
            &bg_rect,
            &bg_rect,
            win_id,
            IMAGE_COMMON_BG,
            &lcd_dev_info);

    edit_both_rect = GetPinPukEditRect(win_id, is_need_label);

    dis_point.x = (bg_rect.right + bg_rect.left - icon_width) / 2;
    dis_point.y = (bg_rect.bottom + edit_both_rect.v_rect.bottom - icon_height) / 2;
    GUIRES_DisplayImg(&dis_point,
            PNULL,
            PNULL,
            win_id,
            IMAGE_NUMBERINPUT_PIN,
            &lcd_dev_info);
    
    IMG_EnableTransparentColor(FALSE); 
#endif 
}
#endif

/*****************************************************************************/
// 	Description : 处理输入的PIN码
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL void HandlePinInputingString(
                                   MMI_STRING_T *pin_ptr
                                   )
{
    MN_BLOCK_T      pin_value = {0};
    MMI_STRING_T    wait_text = {0};
    
    if(PNULL == pin_ptr)
    {
        //SCI_TRACE_LOW:"HandlePinInputingString pin_ptr is NULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_WINTAB_1209_112_2_18_2_45_30_230,(uint8*)"");
        return;
    }
    
    SCI_MEMSET(&pin_value, 0, sizeof(MN_BLOCK_T));
    
    //转换PIN的结构
    pin_value.blocklen = pin_ptr->wstr_len;

    MMI_WSTRNTOSTR((uint8 *)pin_value.blockbuf,
        MN_MAX_BLOCK_LEN,
        pin_ptr->wstr_ptr,
        pin_value.blocklen,
        pin_value.blocklen);
        
#ifdef MMI_SIM_LOCK_SUPPORT
    SCI_MEMCPY(&g_pin_value[MMIAPIPHONE_GetCurResponsePinSimID()],
                         &pin_value,
                         sizeof(MN_BLOCK_T)); //记录新的pin1码
#endif

    //response pin before receive signal of sim is ready
    MNPHONE_ResponsePinEx(MMIAPIPHONE_GetCurResponsePinSimID(), MNSIM_PIN1, FALSE, &pin_value, PNULL);
    
    MMK_CloseWin(PHONE_PIN_WAIT_WIN_ID);
    
    //进入等待界面
    MMI_GetLabelTextByLang(TXT_COMMON_WAITING, &wait_text);
    MMIPUB_OpenWaitWin(1,&wait_text,PNULL,PNULL,PHONE_PIN_WAIT_WIN_ID,IMAGE_NULL,
        ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_NONE,HandlePukWaitingWindow);

    MMK_SetWinDisplayStyleState(PHONE_PIN_WAIT_WIN_ID, WS_DISABLE_RETURN_WIN, TRUE);
}

/*****************************************************************************/
// 	Description : 处理PUK INPUTING的消息
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePukInputWindow (
                                         MMI_WIN_ID_T     win_id, 	// PUK INPUTING窗口的ID
                                         MMI_MESSAGE_ID_E 	msg_id, 	// 窗口的内部消息ID
                                         DPARAM 		        param		// 相应消息的参数
                                         )
{
    //wchar		    star_value[]= {'*'};
    //wchar		    hash_value[]= {'#'};
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    MMI_STRING_T    puk_value = {0};
    MMI_STRING_T    pin_puk_remain_str = {0};
    BOOLEAN is_input_emergency_call = FALSE;
    uint16 emergency_call_len = 0;
    GUI_BOTH_RECT_T edit_both_rect = {0};
    
#ifndef MMI_MULTI_SIM_SYS_SINGLE
    uint32 i = MN_DUAL_SYS_1;
    MMI_TEXT_ID_T text_id[] =
        {
            TXT_ENTER_SIM1_PUK,
            TXT_ENTER_SIM2_PUK,
            #if defined(MMI_MULTI_SIM_SYS_TRI)||defined(MMI_MULTI_SIM_SYS_QUAD)
            TXT_ENTER_SIM3_PUK,
            #endif
            #if defined(MMI_MULTI_SIM_SYS_QUAD)
            TXT_ENTER_SIM4_PUK,
            #endif
        };
#endif

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMIDEFAULT_AllowOpenKeylocWin(FALSE);
#ifdef MAINLCD_SIZE_128X64 //added by taiping.lai 2011/01/03
        {
            GUI_BOTH_RECT_T both_rect = GetPasswordLabelBothRect();
		 	  GUI_RECT_T title_rect = GetPasswordTitleRect();

		     GUIWIN_SetTitleRect(win_id, title_rect);	
            GUIAPICTRL_SetBothRect(MMIPHONE_PIN_INPUT_REMAIN_LABEL_CTRL_ID, &both_rect);
		}
#else			
        {
            GUI_BOTH_RECT_T both_rect = MMITHEME_GetWinClientBothRect(win_id);

            both_rect.h_rect.top += MMIPHONE_PIN_PUK_REMAIN_TOP_MARGIN;
            both_rect.v_rect.top += MMIPHONE_PIN_PUK_REMAIN_TOP_MARGIN;
            both_rect.h_rect.bottom = both_rect.h_rect.top + MMIPHONE_PIN_PUK_REMAIN_HEIGHT;
            both_rect.v_rect.bottom = both_rect.v_rect.top + MMIPHONE_PIN_PUK_REMAIN_HEIGHT;

            GUIAPICTRL_SetBothRect(MMIPHONE_PUK_REMAIN_LABEL_CTRL_ID, &both_rect);
        }
#endif
        IGUICTRL_SetState(MMK_GetCtrlPtr(MMIPHONE_PUK_REMAIN_LABEL_CTRL_ID), GUICTRL_STATE_DISABLE_ACTIVE, TRUE);

        edit_both_rect = GetPinPukEditRect(win_id, TRUE);
        GUIAPICTRL_SetBothRect(PHONE_PUK_EDITBOX_CTRL_ID, &edit_both_rect);

        GUIEDIT_SetPasswordStyle(PHONE_PUK_EDITBOX_CTRL_ID,GUIEDIT_PASSWORD_STYLE_DIGITAL_PIN);

        //set left softkey
        GUIEDIT_SetSoftkey(PHONE_PUK_EDITBOX_CTRL_ID, 0, MMI_PIN_MIN_LENGTH,TXT_NULL,TXT_COMMON_OK, PinPukInputWinSoftkey);

#if defined(MMI_PDA_SUPPORT) && defined(MMIPUBWIN_PDASTYLE_SUPPORT)
        // PUBEDIT负责softkey的显示，PDA下需要禁止右键
        GUIEDIT_SetSoftkey(PHONE_PUK_EDITBOX_CTRL_ID, 2, 0,TXT_NULL,TXT_NULL, PNULL);
#endif

        SCI_MEMSET(&pin_puk_remain_str, 0x00, sizeof(MMI_STRING_T));
        MMIPHONE_GetPinPukRemainStr(MMIAPIPHONE_GetCurResponsePinSimID(), &pin_puk_remain_str, MNSIM_PIN1, TRUE);

        if (PNULL != pin_puk_remain_str.wstr_ptr)
        {
            GUILABEL_SetText(MMIPHONE_PUK_REMAIN_LABEL_CTRL_ID,&pin_puk_remain_str,FALSE);

            SCI_FREE(pin_puk_remain_str.wstr_ptr);
            pin_puk_remain_str.wstr_ptr = PNULL;
        }

#ifdef MMI_MULTI_SIM_SYS_SINGLE
        GUIWIN_SetTitleTextId(win_id,TXT_ENTER_PUK,FALSE);
#else
        i = MMIAPIPHONE_GetCurResponsePinSimID();

        if( i < MMI_DUAL_SYS_MAX)
        {
            GUIWIN_SetTitleTextId(win_id,text_id[i],FALSE);
        }
        else
        {
            GUIWIN_SetTitleTextId(win_id,TXT_ENTER_PUK,FALSE);
        }
#endif

        MMK_SetWinDisplayStyleState(win_id, WS_DISABLE_RETURN_WIN, TRUE);

        MMK_SetAtvCtrl(win_id,PHONE_PUK_EDITBOX_CTRL_ID);
        break;

#ifdef MMI_PDA_SUPPORT
    case MSG_NOTIFY_EDITBOX_UPDATE_STRNUM:
        {
            MMI_STRING_T str_info = {0};
            GUIEDIT_GetString(PHONE_PUK_EDITBOX_CTRL_ID, &str_info);	
        
            is_input_emergency_call = MMIAPICC_IsInputEmergencyCall(&str_info, &emergency_call_len, MMIAPIPHONE_GetCurResponsePinSimID());
            if ((is_input_emergency_call) &&
                (str_info.wstr_len == emergency_call_len))
            {    
                MMIPUB_SetWinSoftkey(win_id, TXT_SOS, TXT_NULL,TRUE);
            }
            else
            {
                MMIPUB_SetWinSoftkey(win_id, TXT_COMMON_OK, TXT_NULL,TRUE);
            }
        }
        break;
#endif
        
    case MSG_FULL_PAINT:
#ifndef MMI_PDA_SUPPORT
        SetPasswordBackground(win_id,PHONE_PUK_EDITBOX_CTRL_ID, TRUE);
#endif
#ifndef SUBLCD_SIZE_NONE
        //刷新小屏的显示，输入PUK码
        MMISUB_IsPermitUpdate(TRUE);
        MMISUB_SetSubLcdDisplay(FALSE,TRUE,SUB_CONTENT_PUK,PNULL);
#endif
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_APP_GREEN:
    case MSG_APP_CALL2:
     case MSG_APP_CALL3:
      case MSG_APP_CALL4:
        {
            uint8 temp_str[MN_MAX_BLOCK_LEN] = {0};
            
            // get the value about PUK
            GUIEDIT_GetString(PHONE_PUK_EDITBOX_CTRL_ID,&puk_value);

            MMI_WSTRNTOSTR( (uint8 *)temp_str, MN_MAX_BLOCK_LEN, puk_value.wstr_ptr, puk_value.wstr_len, puk_value.wstr_len );

            is_input_emergency_call = MMIAPICC_IsInputEmergencyCall(&puk_value, &emergency_call_len, MMIAPIPHONE_GetCurResponsePinSimID());

            if ((is_input_emergency_call) &&
                (puk_value.wstr_len == emergency_call_len))
            {
                MakeEmergencyCall(&puk_value);
            }
            else if (MSG_APP_GREEN != msg_id && MSG_APP_CALL2 != msg_id
                        && MSG_APP_CALL3!= msg_id && MSG_APP_CALL3 != msg_id )
            {
                if(0 == strncmp( (char*)temp_str,"**05*",5))//input **05*PUK*NEW_PIN*NEW_PIN#
                {
                    if (PUK_MODIFY_PIN_LEN_IS_IN_RANGE(puk_value.wstr_len))
                    {
                        HandlePukModifyPin( temp_str );
                    }
                }
                else
                {
                    if (PIN_LEN_IS_IN_RANGE(puk_value.wstr_len))
                    {
                        MMK_CreatePubEditWin((uint32*)PHONE_NEW_PIN_INPUTING_WIN_TAB,PNULL);
                    }
                } 
            }
        }
        break;
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
    case MSG_APP_RED://puk输入界面不允许返回
        MMIPUB_OpenAlertWinByTextId(PNULL,TXT_PLS_INPUT_PUK,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,HandlePinAlertWindow);
        break;

#ifdef FLIP_PHONE_SUPPORT  //close flip ring codes, just open for flip phone.
    case MSG_APP_FLIP:
        MMIDEFAULT_HandleFlipKey(FALSE);
        break;
#endif

    case MSG_GET_FOCUS:
        MMIDEFAULT_AllowOpenKeylocWin(FALSE);
        break;
#ifndef SUBLCD_SIZE_NONE
    case MSG_LOSE_FOCUS:
        //刷新小屏的显示，关闭输入PUK码
        MMISUB_SetSubLcdDisplay(FALSE,FALSE,SUB_CONTENT_PUK,PNULL);
        break;
#endif
    case MSG_CLOSE_WINDOW:
        MMIDEFAULT_AllowOpenKeylocWin(TRUE);
#ifndef SUBLCD_SIZE_NONE
        //刷新小屏的显示，关闭输入PUK码
        MMISUB_SetSubLcdDisplay(FALSE,FALSE,SUB_CONTENT_PUK,PNULL);
#endif
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return (result);
}

/*****************************************************************************/
// 	Description : 处理输入的Puk modify pin码
//  **05*PUK*NEW_PIN*NEW_PIN# to change pin   **052*PUK2*NEW_PIN2*NEW_PIN2# to change pin2
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL void HandlePukModifyPin(
                              uint8 *puk_pin_str
                              )
{
    MMI_PIN_INFO_T  pin_info = {0};
    MMI_STRING_T    wait_text = {0};
    
    SCI_MEMSET(((void*)&pin_info), 0, sizeof(MMI_PIN_INFO_T));
    
    //parse input str
    if (PARS_RESULT_SUCCESS == MMIAPIPARSE_ParsePukAndPinStr((char *)puk_pin_str,&pin_info))
    {
        //response pin before receive signal of sim is ready
        MNPHONE_ResponsePinEx(MMIAPIPHONE_GetCurResponsePinSimID(), MNSIM_PIN1, TRUE, &(pin_info.pin_value), &(pin_info.ext_pin_value));
        
        MMK_CloseWin(PHONE_PIN_WAIT_WIN_ID);
        
        //进入等待界面
        MMI_GetLabelTextByLang(TXT_COMMON_WAITING, &wait_text);
        MMIPUB_OpenWaitWin(1,&wait_text,PNULL,PNULL,PHONE_PIN_WAIT_WIN_ID,IMAGE_NULL,
            ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_NONE,HandlePukWaitingWindow);

        MMK_SetWinDisplayStyleState(PHONE_PIN_WAIT_WIN_ID, WS_DISABLE_RETURN_WIN, TRUE);
    }
    else
    {
        MMIPUB_OpenAlertWinByTextId(PNULL,TXT_INPUT_ERROR,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,HandlePukAlertWindow);
        
        //清空password
        GUIEDIT_ClearAllStr(PHONE_PUK_EDITBOX_CTRL_ID);
    }
}

/*****************************************************************************/
// 	Description : PUK输入界面打开时，等待界面不允许返回，pin，new pin界面不允许red key,flip返回
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePukWaitingWindow (
                                           MMI_WIN_ID_T      win_id, 	// 窗口的ID
                                           MMI_MESSAGE_ID_E     msg_id, 	// 窗口的内部消息ID
                                           DPARAM 		        param		// 相应消息的参数
                                           )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    
    switch (msg_id)
    {
    case MSG_APP_RED://puk输入界面存在,wait window不允许返回
        break;

#ifdef FLIP_PHONE_SUPPORT  //close flip ring codes, just open for flip phone.        
    case MSG_APP_FLIP:
        MMIDEFAULT_HandleFlipKey(FALSE);
        break;
#endif

    case MSG_OPEN_WINDOW:
        MMK_SetWinDisplayStyleState(win_id, WS_DISABLE_RETURN_WIN, TRUE);
        result = MMIPUB_HandleWaitWinMsg(win_id, msg_id, param);
        break;
        
    default:
        result = MMIPUB_HandleWaitWinMsg(win_id, msg_id, param);
        break;
    }
    
    return (result);
}

/*****************************************************************************/
// 	Description : PUK输入界面打开时，alert界面.不允许red key,flip返回(启动过程)
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePukAlertWindow(
                                        MMI_WIN_ID_T     win_id, 	// 窗口的ID
                                        MMI_MESSAGE_ID_E 	msg_id, 	// 窗口的内部消息ID
                                        DPARAM 		        param		// 相应消息的参数
                                        )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    
    switch (msg_id)
    {
    case MSG_APP_RED://puk界面存在，red and flip不允许返回
        if (MMK_IsOpenWin(PHONE_PUK_INPUT_WIN_ID))
        {
            MMK_CloseWin(win_id);
        }
        else
        {
            result = MMI_RESULT_FALSE;
        }
        break;

#ifdef FLIP_PHONE_SUPPORT  //close flip ring codes, just open for flip phone.
    case MSG_APP_FLIP:
        if (MMK_IsOpenWin(PHONE_PUK_INPUT_WIN_ID))
        {
            MMIDEFAULT_HandleFlipKey(FALSE);
            MMK_CloseWin(win_id);
        }
        else
        {
            result = MMI_RESULT_FALSE;
        }
        break;
#endif        
        
    default:
        result = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
        break;
    }
    
    return (result);
}

/*****************************************************************************/
// 	Description : 处理new pin INPUTING的消息
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleNewPinInputWindow (
                                            MMI_WIN_ID_T     win_id, 	// new pin input窗口的ID
                                            MMI_MESSAGE_ID_E 	msg_id, 	// 窗口的内部消息ID
                                            DPARAM 		        param		// 相应消息的参数
                                            )
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    MMI_STRING_T   puk_value = {0};
    MMI_STRING_T   new_pin_value = {0};
    MMI_STRING_T   new_pin_again_value = {0};
    GUI_BOTH_RECT_T edit_both_rect = {0};
    BOOLEAN is_input_emergency_call = FALSE;
    uint16 emergency_call_len = 0;
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMIDEFAULT_AllowOpenKeylocWin(FALSE);
#ifdef MAINLCD_SIZE_128X64 //added by taiping.lai 2011/01/03
		{
	 	    GUI_RECT_T title_rect = GetPasswordTitleRect();

	       GUIWIN_SetTitleRect(win_id, title_rect);	
	
		}
        edit_both_rect = GetPinPukEditRect(win_id, FALSE);
#else
        edit_both_rect = GetPinPukEditRect(win_id, FALSE);
#endif 
        GUIAPICTRL_SetBothRect(PHONE_NEW_PIN_EDITBOX_CTRL_ID, &edit_both_rect);

        GUIEDIT_SetPasswordStyle(PHONE_NEW_PIN_EDITBOX_CTRL_ID,GUIEDIT_PASSWORD_STYLE_DIGITAL_PIN);

        //set left softkey
        GUIEDIT_SetSoftkey(PHONE_NEW_PIN_EDITBOX_CTRL_ID, 0, MMI_PIN_MIN_LENGTH,TXT_NULL,TXT_COMMON_OK, PinPukInputWinSoftkey);
        MMK_SetAtvCtrl(win_id,PHONE_NEW_PIN_EDITBOX_CTRL_ID);
        break;

#if 0
#ifdef MMI_PDA_SUPPORT
    case MSG_NOTIFY_EDITBOX_UPDATE_STRNUM:
        {
            MMI_STRING_T str_info = {0};
            GUIEDIT_GetString(PHONE_NEW_PIN_EDITBOX_CTRL_ID, &str_info);	
        
            is_input_emergency_call = MMIAPICC_IsInputEmergencyCall(&str_info, &emergency_call_len, MMIAPIPHONE_GetCurResponsePinSimID());
            if ((is_input_emergency_call) &&
                (str_info.wstr_len == emergency_call_len))
            {    
                MMIPUB_SetWinSoftkey(win_id, TXT_SOS, STXT_RETURN,TRUE);
            }
            else
            {
                MMIPUB_SetWinSoftkey(win_id, TXT_COMMON_OK, STXT_RETURN,TRUE);
            }
        }
        break;
#endif
#endif
        
#ifndef MMI_PDA_SUPPORT
    case MSG_FULL_PAINT:
        SetPasswordBackground(win_id,PHONE_NEW_PIN_EDITBOX_CTRL_ID,FALSE);
        break;
#endif

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_APP_GREEN:
    case MSG_APP_CALL2:
     case MSG_APP_CALL3:
     case MSG_APP_CALL4:   
        // get the value about new PIN
        GUIEDIT_GetString(PHONE_NEW_PIN_EDITBOX_CTRL_ID,&new_pin_value);
        
        is_input_emergency_call = MMIAPICC_IsInputEmergencyCall(&new_pin_value, &emergency_call_len, MMIAPIPHONE_GetCurResponsePinSimID());

        if ((is_input_emergency_call) &&
            (new_pin_value.wstr_len == emergency_call_len))
        {
            MakeEmergencyCall(&new_pin_value);
        }
        else if (PIN_LEN_IS_IN_RANGE(new_pin_value.wstr_len) 
                    && MSG_APP_GREEN != msg_id 
                    && MSG_APP_CALL2 != msg_id
                    && MSG_APP_CALL3 != msg_id
                    && MSG_APP_CALL4 != msg_id)
        {
            //open the window of input new pin again
            MMK_CreatePubEditWin((uint32*)PHONE_NEW_PIN_AGAIN_INPUTING_WIN_TAB,PNULL);
        }
        break;
        
    case MSG_SET_NEW_PIN_AGAIN:
        // get the value about new PIN again
        GUIEDIT_GetString(PHONE_NEW_PIN_AGAIN_EDITBOX_CTRL_ID,&new_pin_again_value);
        
        // get the value about new PIN
        GUIEDIT_GetString(PHONE_NEW_PIN_EDITBOX_CTRL_ID,&new_pin_value);
        
        //比较两个PIN是否相同
        if( MMIAPICOM_StrCompare( &new_pin_value, &new_pin_again_value ) )
        //if (MMIAPICOM_CompareTwoTeleNum( new_pin_again_value.wstr_ptr, new_pin_again_value.wstr_len, new_pin_value.wstr_ptr, new_pin_value.wstr_len))
        {
            // get the value about PUK
            GUIEDIT_GetString(PHONE_PUK_EDITBOX_CTRL_ID,&puk_value);
            
            HandlePukAndNewPin(&puk_value,&new_pin_value);
            
            //close input new pin and input new pin again window
            MMK_CloseWin(PHONE_NEW_PIN_INPUT_WIN_ID);
            MMK_CloseWin(PHONE_NEW_PIN_AGAIN_INPUT_WIN_ID);
        }
        else
        {
            //提示错误窗口,两次输入的PIN不一致
            #ifdef MMI_PDA_SUPPORT
            MMIPUB_OpenAlertWinByTextId(PNULL,TXT_INPUT_CONFLICT,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,HandlePukAlertWindow);            
            #else
            MMIPUB_OpenAlertWinByTextId(PNULL,TXT_MSGBOX_PIN_CONFLICT,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,HandlePukAlertWindow);
            #endif
            //清空password
            GUIEDIT_ClearAllStr(PHONE_NEW_PIN_EDITBOX_CTRL_ID);
            
            //close input new pin again window
            MMK_CloseWin(PHONE_NEW_PIN_AGAIN_INPUT_WIN_ID);
            
            MMK_SetAtvCtrl(win_id,PHONE_NEW_PIN_EDITBOX_CTRL_ID);
        }
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin( win_id );
        break;
        
    case MSG_APP_RED://puk界面存在，red and flip不允许返回
        if (!MMK_IsOpenWin(PHONE_PUK_INPUT_WIN_ID))
        {
            result = MMI_RESULT_FALSE;
        }
        break;

#ifdef FLIP_PHONE_SUPPORT  //close flip ring codes, just open for flip phone.        
    case MSG_APP_FLIP:
        if (!MMK_IsOpenWin(PHONE_PUK_INPUT_WIN_ID))
        {
            result = MMI_RESULT_FALSE;
        }
        else
        {
            MMIDEFAULT_HandleFlipKey(FALSE);
        }
        break;
#endif        

    case MSG_GET_FOCUS:
        MMIDEFAULT_AllowOpenKeylocWin(FALSE);
        break;

    case MSG_CLOSE_WINDOW:
        MMIDEFAULT_AllowOpenKeylocWin(TRUE);
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return (result);
}

/*****************************************************************************/
// 	Description : 处理new pin again INPUTING的消息
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleNewPinAgainInputWindow (
                                                 MMI_WIN_ID_T    win_id, 	// new pin input窗口的ID
                                                 MMI_MESSAGE_ID_E 	msg_id, 	// 窗口的内部消息ID
                                                 DPARAM 	        param		// 相应消息的参数
                                                 )
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    MMI_STRING_T   new_pin_again_value = {0};   
    GUI_BOTH_RECT_T edit_both_rect = {0};
    BOOLEAN is_input_emergency_call = FALSE;
    uint16 emergency_call_len = 0;
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMIDEFAULT_AllowOpenKeylocWin(FALSE);
#ifdef MAINLCD_SIZE_128X64 //added by taiping.lai 2011/01/03
		{
		 	GUI_RECT_T title_rect = GetPasswordTitleRect();

		    GUIWIN_SetTitleRect(win_id, title_rect);	
		}
        edit_both_rect = GetPinPukEditRect(win_id, FALSE);
#else
        edit_both_rect = GetPinPukEditRect(win_id, TRUE);
#endif 			
        GUIAPICTRL_SetBothRect(PHONE_NEW_PIN_AGAIN_EDITBOX_CTRL_ID, &edit_both_rect);

        GUIEDIT_SetPasswordStyle(PHONE_NEW_PIN_AGAIN_EDITBOX_CTRL_ID,GUIEDIT_PASSWORD_STYLE_DIGITAL_PIN);

        //set left softkey
        GUIEDIT_SetSoftkey(PHONE_NEW_PIN_AGAIN_EDITBOX_CTRL_ID, 0, MMI_PIN_MIN_LENGTH,TXT_NULL,TXT_COMMON_OK, PinPukInputWinSoftkey);

        MMK_SetAtvCtrl(win_id,PHONE_NEW_PIN_AGAIN_EDITBOX_CTRL_ID);
        break;

#if 0
#ifdef MMI_PDA_SUPPORT
    case MSG_NOTIFY_EDITBOX_UPDATE_STRNUM:
        {
            MMI_STRING_T str_info = {0};
            GUIEDIT_GetString(PHONE_NEW_PIN_AGAIN_EDITBOX_CTRL_ID, &str_info);	
        
            is_input_emergency_call = MMIAPICC_IsInputEmergencyCall(&str_info, &emergency_call_len, MMIAPIPHONE_GetCurResponsePinSimID());
            if ((is_input_emergency_call) &&
                (str_info.wstr_len == emergency_call_len))
            {    
                MMIPUB_SetWinSoftkey(win_id, TXT_SOS, STXT_RETURN,TRUE);
            }
            else
            {
                MMIPUB_SetWinSoftkey(win_id, TXT_COMMON_OK, STXT_RETURN,TRUE);
            }
        }
        break;
#endif
#endif
        
#ifndef MMI_PDA_SUPPORT
    case MSG_FULL_PAINT:
        SetPasswordBackground(win_id,PHONE_NEW_PIN_AGAIN_EDITBOX_CTRL_ID,FALSE);
        break;
#endif
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_APP_GREEN:
    case MSG_APP_CALL2:
    case MSG_APP_CALL3:
    case MSG_APP_CALL4:    
        // get the value about new PIN again
        GUIEDIT_GetString(PHONE_NEW_PIN_AGAIN_EDITBOX_CTRL_ID,&new_pin_again_value);
        
        is_input_emergency_call = MMIAPICC_IsInputEmergencyCall(&new_pin_again_value, &emergency_call_len, MMIAPIPHONE_GetCurResponsePinSimID());

        if ((is_input_emergency_call) &&
            (new_pin_again_value.wstr_len == emergency_call_len))
        {
            MakeEmergencyCall(&new_pin_again_value);
        }
        else if (PIN_LEN_IS_IN_RANGE(new_pin_again_value.wstr_len) 
               && MSG_APP_GREEN != msg_id 
               && MSG_APP_CALL2 != msg_id
               && MSG_APP_CALL3 != msg_id
               && MSG_APP_CALL4 != msg_id)
        {
            //send the new pin again to input new pin window
            MMK_SendMsg(PHONE_NEW_PIN_INPUT_WIN_ID,MSG_SET_NEW_PIN_AGAIN,PNULL);
        }
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin( win_id );
        break;
        
    case MSG_APP_RED://puk界面存在，red and flip不允许返回
        if (!MMK_IsOpenWin(PHONE_PUK_INPUT_WIN_ID))
        {
            result = MMI_RESULT_FALSE;
        }
        break;

#ifdef FLIP_PHONE_SUPPORT  //close flip ring codes, just open for flip phone.        
    case MSG_APP_FLIP:
        if (!MMK_IsOpenWin(PHONE_PUK_INPUT_WIN_ID))
        {
            result = MMI_RESULT_FALSE;
        }
        else
        {
            MMIDEFAULT_HandleFlipKey(FALSE);
        }
        break;
#endif        

    case MSG_GET_FOCUS:
        MMIDEFAULT_AllowOpenKeylocWin(FALSE);
        break;

    case MSG_CLOSE_WINDOW:
        MMIDEFAULT_AllowOpenKeylocWin(TRUE);
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return (result);
}

/*****************************************************************************/
// 	Description : 处理输入的Puk and new pin码
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL void HandlePukAndNewPin(
                              MMI_STRING_T *puk_ptr,
                              MMI_STRING_T *pin_ptr
                              )
{
    MN_BLOCK_T  puk_value = {0};
    MN_BLOCK_T  pin_value = {0};
    MMI_STRING_T    wait_text = {0};
    
    if(PNULL == pin_ptr ||PNULL == puk_ptr)
    {
        //SCI_TRACE_LOW:"HandlePinInputingString pin_ptr=%d ||puk_ptr=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_WINTAB_1899_112_2_18_2_45_32_231,(uint8*)"dd", pin_ptr, puk_ptr);
        return;
    }
    
    //转换PUK的结构
    puk_value.blocklen = puk_ptr->wstr_len;

    MMI_WSTRNTOSTR((uint8 *)puk_value.blockbuf,
        MN_MAX_BLOCK_LEN,
        puk_ptr->wstr_ptr,
        puk_value.blocklen,
        puk_value.blocklen);
    
    //转换PIN的结构
    pin_value.blocklen = pin_ptr->wstr_len;

    MMI_WSTRNTOSTR((uint8 *)pin_value.blockbuf,
        MN_MAX_BLOCK_LEN,
        pin_ptr->wstr_ptr,
        pin_value.blocklen,
        pin_value.blocklen);

    //response pin before receive signal of sim is ready
    MNPHONE_ResponsePinEx(MMIAPIPHONE_GetCurResponsePinSimID(), MNSIM_PIN1, TRUE, &pin_value, &puk_value);
    
    MMK_CloseWin(PHONE_PIN_WAIT_WIN_ID);
    
    //进入等待界面
    MMI_GetLabelTextByLang(TXT_COMMON_WAITING, &wait_text);
    MMIPUB_OpenWaitWin(1,&wait_text,PNULL,PNULL,PHONE_PIN_WAIT_WIN_ID,IMAGE_NULL,
        ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_NONE,HandlePukWaitingWindow);

    MMK_SetWinDisplayStyleState(PHONE_PIN_WAIT_WIN_ID, WS_DISABLE_RETURN_WIN, TRUE);
}


/*****************************************************************************/
// 	Description : handle pin and puk flow
//	Global resource dependence : 
//  Author:jassmine.meng
//	Note: 
/*****************************************************************************/
void MMIAPIPHONE_HandlePinAndPukFlow(
                             MNSIM_PIN_REQ_TYPE_E       operate_type,   //PIN的操作标识
                             MNSIM_PIN_INDICATION_E     pin_num,        //MNSIM_PIN1 or MNSIM_PIN2
                             uint32                     win_tab,
                             BOOLEAN is_special_func
                             )
{
    MNSIM_PIN_STATUS_E  pin_status = MNSIM_PIN_READY;
    MMI_OPERATE_PIN_T   *opreate_pin_ptr = SCI_ALLOC_APPZ(sizeof(MMI_OPERATE_PIN_T));
    SIM_STATUS_E sim_status = SIM_STATUS_OK;
    
    opreate_pin_ptr->operate_type = operate_type;
    opreate_pin_ptr->pin_num = pin_num;
    g_operate_pin_win_tab = win_tab;
    g_is_operate_pin_special_func = is_special_func;
    
    //get the status of PIN
    pin_status = MNSIM_GetPinStatusEx(MMIAPIPHONE_GetCurHandlingPinSimID(), pin_num);
    if (MNSIM_PIN_BLOCKED == pin_status)//PUK被锁
    {
        sim_status = MMIAPIPHONE_GetSimStatus(MMIAPIPHONE_GetCurHandlingPinSimID());
        
        if (SIM_STATUS_NO_SIM == sim_status)
        {
            MMIPUB_OpenAlertWarningWin(TXT_NO_SIM);
        }
        else if((SIM_STATUS_REGISTRATION_FAILED == sim_status) || (SIM_STATUS_REJECTED == sim_status))
        {
#ifndef MMI_IDLE_MINI_SUPPORT
            MMIPUB_OpenAlertWarningWin(TXT_SIM_REGISTRATION_FAILED);
#else
            MMIPUB_OpenAlertWarningWin(TXT_SIM_REGISTRATION_FAILED_MINI);
#endif

        }
        else
        {
            if (MNSIM_PIN1 == pin_num)//PIN1
            {
                MMIPUB_OpenAlertWarningWin(TXT_PUK_BLOCKED);
            }
            else//PIN2
            {
                MMIPUB_OpenAlertWarningWin(TXT_PUK2_BLOCKED);
            }
        }
    }
    else
    {
        if (MNSIM_PIN_PUK == pin_status)//PIN被锁，要求输入PUK
        {
            if (MNSIM_PIN1 == pin_num)//PIN1
            {
                MMK_CloseWin(MMI_PUK_INPUTING_WIN_ID);

                opreate_pin_ptr->operate_type = MNSIM_PIN_UNBLOCK;
                // 打开输入PUK的界面
                MMK_CreatePubEditWin((uint32*)MMI_PUK_INPUTING_WIN_TAB,(ADD_DATA)opreate_pin_ptr);
            }
            else//PIN2被锁sim仍然可以用
            {
                SCI_MEMSET(&s_operate_pin2_blocked_info,0,sizeof(MMI_OPERATE_PIN_T));
                s_operate_pin2_blocked_info.operate_type = operate_type;
                s_operate_pin2_blocked_info.pin_num = pin_num;
                MMIPUB_CloseAlertWin();
                MMIPUB_OpenAlertWinByTextId(PNULL,TXT_PIN2_BLOCKED,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,HandlePin2BlockedExceptPhone);

                SCI_FREE(opreate_pin_ptr);
                opreate_pin_ptr = PNULL;
            }
        }
        else//PIN2没有被锁，要求输入PIN2
        {
            MMK_CloseWin(MMI_PIN_INPUTING_WIN_ID);

            // 打开输入PIN的界面
            MMK_CreatePubEditWin((uint32*)MMI_PIN_INPUTING_WIN_TAB,(ADD_DATA)opreate_pin_ptr);
        }
    }
}

/*****************************************************************************/
// 	Description : pin2 blocked的情况下关闭窗口，则启动输入puk的窗口
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePin2BlockedExceptPhone(
                                                MMI_WIN_ID_T     win_id, 	// Pin boloked窗口的ID
                                                MMI_MESSAGE_ID_E 	msg_id, 	// 窗口的内部消息ID
                                                DPARAM 		        param		// 相应消息的参数
                                                )
{
    MMI_OPERATE_PIN_T   *operate_pin_ptr = &s_operate_pin2_blocked_info;
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    
    switch (msg_id)
    {
    case MSG_CLOSE_WINDOW:
        {
            MMI_OPERATE_PIN_T   *new_opreate_pin_ptr = SCI_ALLOC_APPZ(sizeof(MMI_OPERATE_PIN_T));
            
            MMI_MEMCPY(new_opreate_pin_ptr,
                sizeof(MMI_OPERATE_PIN_T),
                operate_pin_ptr,
                sizeof(MMI_OPERATE_PIN_T),
                sizeof(MMI_OPERATE_PIN_T));
            
            result = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);

            MMK_CloseWin(MMI_PUK_INPUTING_WIN_ID);
            
            new_opreate_pin_ptr->operate_type = MNSIM_PIN_UNBLOCK;
            // 打开输入PUK的界面
            MMK_CreatePubEditWin((uint32*)MMI_PUK_INPUTING_WIN_TAB,(ADD_DATA)new_opreate_pin_ptr);
        }
        break;
        
    default:
        result = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
        break;
    }
    
    return (result);
}

/*****************************************************************************/
// 	Description : 处理所有PIN INPUT的消息，除去开关机reponse PIN的情况
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePinInputExceptPhone(
                                             MMI_WIN_ID_T    win_id, 	// PIN2 INPUTING窗口的ID
                                             MMI_MESSAGE_ID_E 	msg_id, 	// 窗口的内部消息ID
                                             DPARAM 		    param		// 相应消息的参数
                                             )
{
    MMI_OPERATE_PIN_T   *operate_pin_ptr = (MMI_OPERATE_PIN_T *)MMK_GetWinAddDataPtr(win_id);
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    MMI_STRING_T   pin_value = {0};  
    GUI_BOTH_RECT_T edit_both_rect = {0};
    MMI_STRING_T    pin_puk_remain_str = {0};
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
#ifdef MAINLCD_SIZE_128X64 //added by taiping.lai 2011/01/03
        {
            GUI_BOTH_RECT_T both_rect = GetPasswordLabelBothRect();
		 	  GUI_RECT_T title_rect = GetPasswordTitleRect();

		     GUIWIN_SetTitleRect(win_id, title_rect);	
            GUIAPICTRL_SetBothRect(MMIPHONE_PIN_INPUT_REMAIN_LABEL_CTRL_ID, &both_rect);
		}
#else			
        {
            GUI_BOTH_RECT_T both_rect = MMITHEME_GetWinClientBothRect(win_id);

            both_rect.h_rect.top += MMIPHONE_PIN_PUK_REMAIN_TOP_MARGIN;
            both_rect.v_rect.top += MMIPHONE_PIN_PUK_REMAIN_TOP_MARGIN;
            both_rect.h_rect.bottom = both_rect.h_rect.top + MMIPHONE_PIN_PUK_REMAIN_HEIGHT;
            both_rect.v_rect.bottom = both_rect.v_rect.top + MMIPHONE_PIN_PUK_REMAIN_HEIGHT;

            GUIAPICTRL_SetBothRect(MMIPHONE_PIN_INPUT_REMAIN_LABEL_CTRL_ID, &both_rect);
        }
#endif 
        IGUICTRL_SetState(MMK_GetCtrlPtr(MMIPHONE_PIN_INPUT_REMAIN_LABEL_CTRL_ID), GUICTRL_STATE_DISABLE_ACTIVE, TRUE);

        SCI_MEMSET(&pin_puk_remain_str, 0x00, sizeof(MMI_STRING_T));
        MMIPHONE_GetPinPukRemainStr(MMIAPIPHONE_GetCurHandlingPinSimID(), &pin_puk_remain_str, operate_pin_ptr->pin_num, FALSE);

        if (PNULL != pin_puk_remain_str.wstr_ptr)
        {
            GUILABEL_SetText(MMIPHONE_PIN_INPUT_REMAIN_LABEL_CTRL_ID,&pin_puk_remain_str,FALSE);

            SCI_FREE(pin_puk_remain_str.wstr_ptr);
            pin_puk_remain_str.wstr_ptr = PNULL;
        }

        edit_both_rect = GetPinPukEditRect(win_id, TRUE);
        GUIAPICTRL_SetBothRect(MMI_PIN_INPUT_EDITBOX_CTRL_ID, &edit_both_rect);
        
        GUIEDIT_SetPasswordStyle(MMI_PIN_INPUT_EDITBOX_CTRL_ID,GUIEDIT_PASSWORD_STYLE_DIGITAL_PIN);

        //set left softkey
        GUIEDIT_SetSoftkey(MMI_PIN_INPUT_EDITBOX_CTRL_ID,0,MMI_PIN_MIN_LENGTH,TXT_NULL,TXT_COMMON_OK,PNULL);
        MMK_SetAtvCtrl(win_id,MMI_PIN_INPUT_EDITBOX_CTRL_ID);
        break;

    case MSG_GET_FOCUS:
        SCI_MEMSET(&pin_puk_remain_str, 0x00, sizeof(MMI_STRING_T));
        MMIPHONE_GetPinPukRemainStr(MMIAPIPHONE_GetCurHandlingPinSimID(), &pin_puk_remain_str, operate_pin_ptr->pin_num, FALSE);

        if (PNULL != pin_puk_remain_str.wstr_ptr)
        {
            GUILABEL_SetText(MMIPHONE_PIN_INPUT_REMAIN_LABEL_CTRL_ID,&pin_puk_remain_str,FALSE);

            SCI_FREE(pin_puk_remain_str.wstr_ptr);
            pin_puk_remain_str.wstr_ptr = PNULL;
        }
        break;
        
    case MSG_FULL_PAINT:
        //set title
        if (MNSIM_PIN1 == operate_pin_ptr->pin_num)
        {
            GUIWIN_SetTitleTextId(MMI_PIN_INPUTING_WIN_ID,TXT_ENTER_PIN,FALSE);
        }
        else
        {
            GUIWIN_SetTitleTextId(MMI_PIN_INPUTING_WIN_ID,TXT_ENTER_PIN2,FALSE);
        }
#if !defined(MMI_PDA_SUPPORT) && !defined(MMIPUBWIN_PDASTYLE_SUPPORT)
        //MMIAPISET_SetPwBg(win_id, IMAGE_NUMBERINPUT_PIN);
        SetPasswordBackground(win_id,MMI_PIN_INPUT_EDITBOX_CTRL_ID,TRUE);
#endif
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        // get the value about PIN
        GUIEDIT_GetString(MMI_PIN_INPUT_EDITBOX_CTRL_ID,&pin_value);
        
        if (PIN_LEN_IS_IN_RANGE(pin_value.wstr_len))
        {
            if (MNSIM_PIN_CHANGE == operate_pin_ptr->operate_type)
            {
                //放在此处分配空间，可以避免多次Alloc
                MMI_OPERATE_PIN_T   *new_opreate_pin_ptr = SCI_ALLOC_APPZ(sizeof(MMI_OPERATE_PIN_T));
                MMI_MEMCPY(new_opreate_pin_ptr,
                    sizeof(MMI_OPERATE_PIN_T),
                    operate_pin_ptr,
                    sizeof(MMI_OPERATE_PIN_T),
                    sizeof(MMI_OPERATE_PIN_T));
                MMK_CreatePubEditWin((uint32*)MMI_NEW_PIN_INPUTING_WIN_TAB,(ADD_DATA)new_opreate_pin_ptr);
            }
            else
            {
                HandleOperatePin(operate_pin_ptr,&pin_value);
            }
        }
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    case MSG_CLOSE_WINDOW:
        MMK_FreeWinAddData(MMI_PIN_INPUTING_WIN_ID);
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return (result);
}

/*****************************************************************************/
// 	Description : 处理输入的PIN码，除去开关机reponse PIN的情况
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL void HandleOperatePin(
                            MMI_OPERATE_PIN_T   *operate_pin_info,
                            MMI_STRING_T       *pin_ptr
                            )
{
    MN_BLOCK_T      pin_value = {0};
    MMI_STRING_T    wait_text = {0};
    
    if(PNULL == pin_ptr)
    {
        //SCI_TRACE_LOW:"HandlePinInputingString pin_ptr "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_WINTAB_2208_112_2_18_2_45_32_232,(uint8*)"");
        return;
    }
    
    //SCI_MEMSET(&pin_value, 0, sizeof(MN_BLOCK_T));
    
    //转换PIN的结构
    pin_value.blocklen = pin_ptr->wstr_len;

    MMI_WSTRNTOSTR((uint8 *)pin_value.blockbuf,
        MN_MAX_BLOCK_LEN,
        pin_ptr->wstr_ptr,
        pin_value.blocklen,
        pin_value.blocklen);
    //SCI_TRACE_LOW:"HandleOperatePin:operate_type is %d,pin_num is %d "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_WINTAB_2222_112_2_18_2_45_32_233,(uint8*)"dd",operate_pin_info->operate_type,operate_pin_info->pin_num);
    
    //operate pin
    MNPHONE_OperatePinEx(MMIAPIPHONE_GetCurHandlingPinSimID(), operate_pin_info->operate_type,operate_pin_info->pin_num,&pin_value, PNULL);
    
    MMK_CloseWin(PHONE_PIN_WAIT_WIN_ID);
    
    //进入等待界面
    MMI_GetLabelTextByLang(TXT_COMMON_WAITING, &wait_text);
    MMIPUB_OpenWaitWin(1,&wait_text,PNULL,PNULL,PHONE_PIN_WAIT_WIN_ID,IMAGE_NULL,
        ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_NONE,PNULL);
}

/*****************************************************************************/
// 	Description : HandleExitPinBlockedExceptPhone
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL void HandleExitPinBlockedExceptPhone(void)
{
    MN_DUAL_SYS_E dual_sys = MMIAPIPHONE_GetCurHandlingPinSimID();
    
    MMIAPICC_SetSimCallReady(dual_sys, FALSE);
    MMIAPISET_SetIsQueryCfu(FALSE,dual_sys);
    MAIN_SetIdleForwardState(dual_sys, FALSE);
}

/*****************************************************************************/
// 	Description : 处理输入的PUK码，除去开关机reponse PIN的情况
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePukInputExceptPhone (
                                              MMI_WIN_ID_T   win_id, 	// PUK INPUTING窗口的ID
                                              MMI_MESSAGE_ID_E 	msg_id, 	// 窗口的内部消息ID
                                              DPARAM 		    param		// 相应消息的参数
                                              )
{
    //wchar		        star_value[]= {'*'};
    //wchar		        hash_value[]= {'#'};
    MMI_OPERATE_PIN_T   *operate_pin_ptr = (MMI_OPERATE_PIN_T *)MMK_GetWinAddDataPtr(win_id);
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    MMI_STRING_T        puk_value = {0};
    GUI_BOTH_RECT_T edit_both_rect = {0};
    MMI_STRING_T    pin_puk_remain_str = {0};
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
#ifdef MAINLCD_SIZE_128X64 //added by taiping.lai 2011/01/03
        {
            GUI_BOTH_RECT_T both_rect = GetPasswordLabelBothRect();
		 	  GUI_RECT_T title_rect = GetPasswordTitleRect();

		     GUIWIN_SetTitleRect(win_id, title_rect);	
            GUIAPICTRL_SetBothRect(MMIPHONE_PIN_INPUT_REMAIN_LABEL_CTRL_ID, &both_rect);
		}
#else						
        {
            GUI_BOTH_RECT_T both_rect = MMITHEME_GetWinClientBothRect(win_id);

            both_rect.h_rect.top += MMIPHONE_PIN_PUK_REMAIN_TOP_MARGIN;
            both_rect.v_rect.top += MMIPHONE_PIN_PUK_REMAIN_TOP_MARGIN;
            both_rect.h_rect.bottom = both_rect.h_rect.top + MMIPHONE_PIN_PUK_REMAIN_HEIGHT;
            both_rect.v_rect.bottom = both_rect.v_rect.top + MMIPHONE_PIN_PUK_REMAIN_HEIGHT;

            GUIAPICTRL_SetBothRect(MMIPHONE_PUK_INPUT_REMAIN_LABEL_CTRL_ID, &both_rect);
        }
#endif
        IGUICTRL_SetState(MMK_GetCtrlPtr(MMIPHONE_PUK_INPUT_REMAIN_LABEL_CTRL_ID), GUICTRL_STATE_DISABLE_ACTIVE, TRUE);

        SCI_MEMSET(&pin_puk_remain_str, 0x00, sizeof(MMI_STRING_T));
        MMIPHONE_GetPinPukRemainStr(MMIAPIPHONE_GetCurHandlingPinSimID(), &pin_puk_remain_str, operate_pin_ptr->pin_num, TRUE);

        if (PNULL != pin_puk_remain_str.wstr_ptr)
        {
            GUILABEL_SetText(MMIPHONE_PUK_INPUT_REMAIN_LABEL_CTRL_ID,&pin_puk_remain_str,FALSE);

            SCI_FREE(pin_puk_remain_str.wstr_ptr);
            pin_puk_remain_str.wstr_ptr = PNULL;
        }

        edit_both_rect = GetPinPukEditRect(win_id, TRUE);
        GUIAPICTRL_SetBothRect(MMI_PUK_INPUT_EDITBOX_CTRL_ID, &edit_both_rect);

        GUIEDIT_SetPasswordStyle(MMI_PUK_INPUT_EDITBOX_CTRL_ID,GUIEDIT_PASSWORD_STYLE_DIGITAL_PIN);
        
        //set left softkey
        GUIEDIT_SetSoftkey(MMI_PUK_INPUT_EDITBOX_CTRL_ID,0,MMI_PIN_MIN_LENGTH,TXT_NULL,TXT_COMMON_OK,PNULL);
        MMK_SetAtvCtrl(win_id,MMI_PUK_INPUT_EDITBOX_CTRL_ID);
        break;

    case MSG_GET_FOCUS:
        SCI_MEMSET(&pin_puk_remain_str, 0x00, sizeof(MMI_STRING_T));
        MMIPHONE_GetPinPukRemainStr(MMIAPIPHONE_GetCurHandlingPinSimID(), &pin_puk_remain_str, operate_pin_ptr->pin_num, TRUE);

        if (PNULL != pin_puk_remain_str.wstr_ptr)
        {
            GUILABEL_SetText(MMIPHONE_PUK_INPUT_REMAIN_LABEL_CTRL_ID,&pin_puk_remain_str,FALSE);

            SCI_FREE(pin_puk_remain_str.wstr_ptr);
            pin_puk_remain_str.wstr_ptr = PNULL;
        }
        break;
        
    case MSG_FULL_PAINT:
        //set title
        if (MNSIM_PIN1 == operate_pin_ptr->pin_num)
        {
            GUIWIN_SetTitleTextId(MMI_PUK_INPUTING_WIN_ID,TXT_ENTER_PUK,FALSE);
        }
        else
        {
            GUIWIN_SetTitleTextId(MMI_PUK_INPUTING_WIN_ID,TXT_ENTER_PUK2,FALSE);
        }
#ifndef MMI_PDA_SUPPORT
        SetPasswordBackground(win_id,MMI_PUK_INPUT_EDITBOX_CTRL_ID,TRUE);
#endif
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        {
            uint8 temp_str[MN_MAX_BLOCK_LEN] = {0};

            // get the value about PUK
            GUIEDIT_GetString(MMI_PUK_INPUT_EDITBOX_CTRL_ID,&puk_value);

            MMI_WSTRNTOSTR( (uint8 *)temp_str, MN_MAX_BLOCK_LEN, puk_value.wstr_ptr, puk_value.wstr_len, puk_value.wstr_len );
            
            if(0 == strncmp( (char*)temp_str,"**05*",5))//input **05*PUK*NEW_PIN*NEW_PIN#
            {
                if (PUK_MODIFY_PIN_LEN_IS_IN_RANGE(puk_value.wstr_len))
                {
                    HandlePukModifyPinExceptPhone( temp_str, MNSIM_PIN1 );
                }
            }
            else if (0 == strncmp( (char*)temp_str,"**052*",6))//input **052*PUK2*NEW_PIN2*NEW_PIN2# to change pin2
            {
                if (PUK2_MODIFY_PIN2_LEN_IS_IN_RANGE(puk_value.wstr_len))
                {
                    HandlePukModifyPinExceptPhone( temp_str,MNSIM_PIN2);
                }
            }
            else
            {
                if (PIN_LEN_IS_IN_RANGE(puk_value.wstr_len))
                {
                    //放在此处分配空间，可以避免多次Alloc
                    MMI_OPERATE_PIN_T   *new_opreate_pin_ptr = SCI_ALLOC_APPZ(sizeof(MMI_OPERATE_PIN_T));
                    MMI_MEMCPY(new_opreate_pin_ptr,
                        sizeof(MMI_OPERATE_PIN_T),
                        operate_pin_ptr,
                        sizeof(MMI_OPERATE_PIN_T),
                        sizeof(MMI_OPERATE_PIN_T));
                    
                    MMK_CreatePubEditWin((uint32*)MMI_NEW_PIN_INPUTING_WIN_TAB,(ADD_DATA)new_opreate_pin_ptr);
                }
            }
        }
        break;

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL://puk输入界面不允许返回
    case MSG_APP_RED:
        {
// Bug 1339417 start
            if (MNSIM_PIN1 == operate_pin_ptr->pin_num)
            {
                HandleExitPinBlockedExceptPhone();
                MMIPUB_OpenAlertWinByTextId(PNULL,TXT_PLS_INPUT_PUK,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,HandlePinAlertWindow);
            }
            else
            {
                MMK_CloseWin(win_id);
            }
// Bug 1339417 end
        }
        break;
    case MSG_APP_FLIP:
        if (MNSIM_PIN1 == operate_pin_ptr->pin_num)
        {
            HandleExitPinBlockedExceptPhone();
            
            MMK_CloseWin(win_id);
        }
        else
        {
            MMK_CloseWin(win_id);
        }

#ifdef FLIP_PHONE_SUPPORT  //close flip ring codes, just open for flip phone.        
        if(MSG_APP_FLIP == msg_id)
        {
            MMIDEFAULT_HandleFlipKey(FALSE);
        }
#endif        
        break;
        
    case MSG_CLOSE_WINDOW:
        MMK_FreeWinAddData(MMI_PUK_INPUTING_WIN_ID);
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return (result);
}

/*****************************************************************************/
// 	Description : 处理输入的Puk modify pin码(开机之后)
//  **05*PUK*NEW_PIN*NEW_PIN# to change pin   **052*PUK2*NEW_PIN2*NEW_PIN2# to change pin2
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL void HandlePukModifyPinExceptPhone(
                                         uint8                      *puk_pin_str,
                                         MNSIM_PIN_INDICATION_E     pin_num
                                         )
{
    MMI_PIN_INFO_T  pin_info = {0};
    MMI_STRING_T    wait_text = {0};
    
    SCI_MEMSET(((void*)&pin_info), 0, sizeof(MMI_PIN_INFO_T));
    
    //parse input str
    if (PARS_RESULT_SUCCESS == MMIAPIPARSE_ParsePukAndPinStr((char *)puk_pin_str,&pin_info))
    {
        //operate pin
        MNPHONE_OperatePinEx(MMIAPIPHONE_GetCurHandlingPinSimID(), MNSIM_PIN_UNBLOCK,pin_num,&pin_info.ext_pin_value,&pin_info.pin_value);
        
        MMK_CloseWin(PHONE_PIN_WAIT_WIN_ID);
        
        //进入等待界面
        MMI_GetLabelTextByLang(TXT_COMMON_WAITING, &wait_text);
        MMIPUB_OpenWaitWin(1,&wait_text,PNULL,PNULL,PHONE_PIN_WAIT_WIN_ID,IMAGE_NULL,
            ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_NONE,HandlePukWaitingWindow);
    }
    else
    {
        if (MNSIM_PIN1 == pin_num)
        {
            MMIPUB_OpenAlertWinByTextId(PNULL,TXT_INPUT_ERROR,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,HandlePukAlertWindowExceptPhone);
        }
        else
        {
            MMIPUB_OpenAlertWarningWin(TXT_INPUT_ERROR);
        }
        
        //清空password
        GUIEDIT_ClearAllStr(MMI_PUK_INPUT_EDITBOX_CTRL_ID);
    }
}

/*****************************************************************************/
// 	Description : PUK输入界面打开时，alert界面\不允许red key,flip返回(除去开关机reponse PIN的情况)
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePukAlertWindowExceptPhone(
                                                   MMI_WIN_ID_T     win_id, 	// 窗口的ID
                                                   MMI_MESSAGE_ID_E 	msg_id, 	// 窗口的内部消息ID
                                                   DPARAM 		        param		// 相应消息的参数
                                                   )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    
    switch (msg_id)
    {
    case MSG_APP_RED://puk界面存在，red and flip不允许返回
        if (MMK_IsOpenWin(MMI_PUK_INPUTING_WIN_ID))
        {
            MMK_CloseWin(win_id);
        }
        else
        {
            result = MMI_RESULT_FALSE;
        }
        break;

#ifdef FLIP_PHONE_SUPPORT  //close flip ring codes, just open for flip phone.
    case MSG_APP_FLIP:
        if (MMK_IsOpenWin(MMI_PUK_INPUTING_WIN_ID))
        {
            MMIDEFAULT_HandleFlipKey(FALSE);
            MMK_CloseWin(win_id);
        }
        else
        {
            result = MMI_RESULT_FALSE;
        }
        break;
#endif
        
    default:
        result = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
        break;
    }
    
    return (result);
}

/*****************************************************************************/
// 	Description : 处理new pin INPUTING的消息,除去开关机reponse PIN的情况
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleNewPinInputExceptPhone (
                                                 MMI_WIN_ID_T    win_id, 	// new pin input窗口的ID
                                                 MMI_MESSAGE_ID_E 	msg_id, 	// 窗口的内部消息ID
                                                 DPARAM 		    param		// 相应消息的参数
                                                 )
{
    MMI_OPERATE_PIN_T   *operate_pin_ptr = (MMI_OPERATE_PIN_T *)MMK_GetWinAddDataPtr(win_id);
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    MMI_STRING_T   puk_value = {0};
    MMI_STRING_T   old_pin_value = {0};
    MMI_STRING_T   new_pin_value = {0};
    MMI_STRING_T   new_pin_again_value = {0};
    GUI_BOTH_RECT_T edit_both_rect = {0};

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
#ifdef MAINLCD_SIZE_128X64 //added by taiping.lai 2011/01/03
		{
			GUI_RECT_T title_rect = GetPasswordTitleRect();

			GUIWIN_SetTitleRect(win_id, title_rect);	
		}
        edit_both_rect = GetPinPukEditRect(win_id, FALSE);
#else
        edit_both_rect = GetPinPukEditRect(win_id, TRUE);     
#endif 
        GUIAPICTRL_SetBothRect(MMI_NEW_PIN_INPUT_EDITBOX_CTRL_ID, &edit_both_rect);

        GUIEDIT_SetPasswordStyle(MMI_NEW_PIN_INPUT_EDITBOX_CTRL_ID,GUIEDIT_PASSWORD_STYLE_DIGITAL_PIN);
       //set left softkey
        GUIEDIT_SetSoftkey(MMI_NEW_PIN_INPUT_EDITBOX_CTRL_ID,0,MMI_PIN_MIN_LENGTH,TXT_NULL,TXT_COMMON_OK,PNULL);
	  MMK_SetAtvCtrl(win_id,MMI_NEW_PIN_INPUT_EDITBOX_CTRL_ID);
        break;
        
    case MSG_FULL_PAINT:
        //set title
        if (MNSIM_PIN1 == operate_pin_ptr->pin_num)
        {
            GUIWIN_SetTitleTextId(MMI_NEW_PIN_INPUTING_WIN_ID,TXT_ENTER_NEW_PIN,FALSE);
        }
        else
        {
            GUIWIN_SetTitleTextId(MMI_NEW_PIN_INPUTING_WIN_ID,TXT_ENTER_NEW_PIN2,FALSE);
        }
#if !defined(MMI_PDA_SUPPORT) && !defined(MMIPUBWIN_PDASTYLE_SUPPORT)
#if !defined(MAINLCD_SIZE_176X220) && !defined(MAINLCD_SIZE_128X160)
        SetPasswordBackground(win_id,MMI_NEW_PIN_INPUT_EDITBOX_CTRL_ID,FALSE);
#else
        SetPasswordBackground(win_id,MMI_NEW_PIN_INPUT_EDITBOX_CTRL_ID,TRUE);
#endif
#endif
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        // get the value about new PIN
        GUIEDIT_GetString(MMI_NEW_PIN_INPUT_EDITBOX_CTRL_ID,&new_pin_value);
        
        if (PIN_LEN_IS_IN_RANGE(new_pin_value.wstr_len))
        {
            MMI_OPERATE_PIN_T   *new_opreate_pin_ptr = SCI_ALLOC_APPZ(sizeof(MMI_OPERATE_PIN_T));
            MMI_MEMCPY(new_opreate_pin_ptr,sizeof(MMI_OPERATE_PIN_T),
                operate_pin_ptr,sizeof(MMI_OPERATE_PIN_T),
                sizeof(MMI_OPERATE_PIN_T));
            
            //open the window of input new pin again
            MMK_CreatePubEditWin((uint32*)MMI_NEW_PIN_AGAIN_INPUTING_WIN_TAB,(ADD_DATA)new_opreate_pin_ptr);
        }
        break;
        
    case MSG_SET_NEW_PIN_AGAIN:
        // get the value about new PIN again
        GUIEDIT_GetString(MMI_NEW_PIN_AGAIN_INPUT_EDITBOX_CTRL_ID,&new_pin_again_value);
        
        // get the value about new PIN
        GUIEDIT_GetString(MMI_NEW_PIN_INPUT_EDITBOX_CTRL_ID,&new_pin_value);
        
        //比较两个PIN是否相同
        
        if( MMIAPICOM_StrCompare( &new_pin_value, &new_pin_again_value ) )
        //if (MMIAPICOM_CompareTwoTeleNum( new_pin_again_value.str_ptr, new_pin_again_value.str_len, new_pin_value.str_ptr, new_pin_value.str_len))
        {
            if (MNSIM_PIN_CHANGE == operate_pin_ptr->operate_type)//pin modify pin
            {
                // get the value about old PIN
                GUIEDIT_GetString(MMI_PIN_INPUT_EDITBOX_CTRL_ID,&old_pin_value);
                HandleOperatePinAndNewPin(operate_pin_ptr,&old_pin_value,&new_pin_value);
            }
            else//puk modify pin
            {
                // get the value about PUK
                GUIEDIT_GetString(MMI_PUK_INPUT_EDITBOX_CTRL_ID,&puk_value);
                HandleOperatePukAndNewPin(operate_pin_ptr,&puk_value,&new_pin_value);
            }
            
            //close input new pin and input new pin again window
            MMK_CloseWin(MMI_NEW_PIN_INPUTING_WIN_ID);
            MMK_CloseWin(MMI_NEW_PIN_AGAIN_INPUTING_WIN_ID);
        }
        else
        {
            if (MNSIM_PIN1 == operate_pin_ptr->pin_num)
            {
                //提示错误窗口,两次输入的PIN不一致
                #ifdef MMI_PDA_SUPPORT
                MMIPUB_OpenAlertWinByTextId(PNULL,TXT_INPUT_CONFLICT,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,HandlePukAlertWindowExceptPhone);
                #else
                MMIPUB_OpenAlertWinByTextId(PNULL,TXT_MSGBOX_PIN_CONFLICT,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,HandlePukAlertWindowExceptPhone);
                #endif
            }
            else
            {
                #ifdef MMI_PDA_SUPPORT 
                MMIPUB_OpenAlertWarningWin(TXT_INPUT_CONFLICT);
                #else
                MMIPUB_OpenAlertWarningWin(TXT_MSGBOX_PIN2_CONFLICT);
                #endif
            }
            
            //清空password
            GUIEDIT_ClearAllStr(MMI_NEW_PIN_INPUT_EDITBOX_CTRL_ID);
            
            //close input new pin again window
            MMK_CloseWin(MMI_NEW_PIN_AGAIN_INPUTING_WIN_ID);
            
            MMK_SetAtvCtrl(win_id,MMI_NEW_PIN_INPUT_EDITBOX_CTRL_ID);
        }
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin( win_id );
        break;
        
    case MSG_APP_RED://puk界面存在，red and flip不允许返回
        if (MNSIM_PIN1 == operate_pin_ptr->pin_num)
        {
            if (!MMK_IsOpenWin(MMI_PUK_INPUTING_WIN_ID))
            {
                result = MMI_RESULT_FALSE;
            }
        }
        else
        {
            result = MMI_RESULT_FALSE;
        }
        break;

#ifdef FLIP_PHONE_SUPPORT  //close flip ring codes, just open for flip phone.        
    case MSG_APP_FLIP:
        if (MNSIM_PIN1 == operate_pin_ptr->pin_num)
        {
            if (!MMK_IsOpenWin(MMI_PUK_INPUTING_WIN_ID))
            {
                result = MMI_RESULT_FALSE;
            }
            else
            {
                MMIDEFAULT_HandleFlipKey(FALSE);
            }
        }
        else
        {
            result = MMI_RESULT_FALSE;
        }
        break;
#endif        
        
    case MSG_CLOSE_WINDOW:
        MMK_FreeWinAddData(MMI_NEW_PIN_INPUTING_WIN_ID);
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return (result);
}

/*****************************************************************************/
// 	Description : 处理new pin again INPUTING的消息,除去开关机reponse PIN的情况
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleNewPinAgainInputExceptPhone (
                                                      MMI_WIN_ID_T   win_id, 	// new pin input窗口的ID
                                                      MMI_MESSAGE_ID_E 	msg_id, 	// 窗口的内部消息ID
                                                      DPARAM            param		// 相应消息的参数
                                                      )
{
    MMI_OPERATE_PIN_T   *operate_pin_ptr = (MMI_OPERATE_PIN_T *)MMK_GetWinAddDataPtr(win_id);
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    MMI_STRING_T       new_pin_again_value = {0};
    GUI_BOTH_RECT_T edit_both_rect = {0};
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
#ifdef MAINLCD_SIZE_128X64 //added by taiping.lai 2011/01/03
		{
			GUI_RECT_T title_rect = GetPasswordTitleRect();

			GUIWIN_SetTitleRect(win_id, title_rect);					
		}
        edit_both_rect = GetPinPukEditRect(win_id, FALSE);
#else
        edit_both_rect = GetPinPukEditRect(win_id, TRUE);
#endif
        GUIAPICTRL_SetBothRect(MMI_NEW_PIN_AGAIN_INPUT_EDITBOX_CTRL_ID, &edit_both_rect);

        GUIEDIT_SetPasswordStyle(MMI_NEW_PIN_AGAIN_INPUT_EDITBOX_CTRL_ID,GUIEDIT_PASSWORD_STYLE_DIGITAL_PIN);

        //set left softkey
        GUIEDIT_SetSoftkey(MMI_NEW_PIN_AGAIN_INPUT_EDITBOX_CTRL_ID,0,MMI_PIN_MIN_LENGTH,TXT_NULL,TXT_COMMON_OK,PNULL);
        MMK_SetAtvCtrl(win_id,MMI_NEW_PIN_AGAIN_INPUT_EDITBOX_CTRL_ID);
        break;
        
    case MSG_FULL_PAINT:
        //set title
        if (MNSIM_PIN1 == operate_pin_ptr->pin_num)
        {
            GUIWIN_SetTitleTextId(MMI_NEW_PIN_AGAIN_INPUTING_WIN_ID,TXT_ENTER_PIN_CNF,FALSE);
        }
        else
        {
            GUIWIN_SetTitleTextId(MMI_NEW_PIN_AGAIN_INPUTING_WIN_ID,TXT_ENTER_PIN2_CNF,FALSE);
        }
#if !defined(MMI_PDA_SUPPORT) && !defined(MMIPUBWIN_PDASTYLE_SUPPORT)
#if !defined(MAINLCD_SIZE_176X220) && !defined(MAINLCD_SIZE_128X160)
        SetPasswordBackground(win_id,MMI_NEW_PIN_AGAIN_INPUT_EDITBOX_CTRL_ID,FALSE);
#else
        SetPasswordBackground(win_id,MMI_NEW_PIN_AGAIN_INPUT_EDITBOX_CTRL_ID,TRUE);
#endif
#endif
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        // get the value about new PIN again
        GUIEDIT_GetString(MMI_NEW_PIN_AGAIN_INPUT_EDITBOX_CTRL_ID,&new_pin_again_value);
        
        if (PIN_LEN_IS_IN_RANGE(new_pin_again_value.wstr_len))
        {
            //send the new pin again to input new pin window
            MMK_SendMsg(MMI_NEW_PIN_INPUTING_WIN_ID,MSG_SET_NEW_PIN_AGAIN,PNULL);
        }
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin( win_id );
        break;
        
    case MSG_APP_RED://puk界面存在，red and flip不允许返回
        if (MNSIM_PIN1 == operate_pin_ptr->pin_num)
        {
            if (!MMK_IsOpenWin(MMI_PUK_INPUTING_WIN_ID))
            {
                result = MMI_RESULT_FALSE;
            }
        }
        else
        {
            result = MMI_RESULT_FALSE;
        }
        break;

#ifdef FLIP_PHONE_SUPPORT  //close flip ring codes, just open for flip phone.        
    case MSG_APP_FLIP:
        if (MNSIM_PIN1 == operate_pin_ptr->pin_num)
        {
            if (!MMK_IsOpenWin(MMI_PUK_INPUTING_WIN_ID))
            {
                result = MMI_RESULT_FALSE;
            }
            else
            {
                MMIDEFAULT_HandleFlipKey(FALSE);
            }
        }
        else
        {
            result = MMI_RESULT_FALSE;
        }
        break;
#endif        
        
    case MSG_CLOSE_WINDOW:
        MMK_FreeWinAddData(MMI_NEW_PIN_AGAIN_INPUTING_WIN_ID);
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return (result);
}

/*****************************************************************************/
// 	Description : handle display imei when no sim or sim error
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDisplayImei (
                                      MMI_WIN_ID_T   win_id, 	// 窗口的ID
                                      MMI_MESSAGE_ID_E 	msg_id, 	// 窗口的内部消息ID
                                      DPARAM            param		// 相应消息的参数
                                      )
{
    MMI_RESULT_E  recode = MMI_RESULT_TRUE;
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        AppendIMEIDetail(MMIPHONE_DISPLAY_IMEI_CTRL_ID);
#ifndef ADULT_WATCH_SUPPORT
        //set softkey
        GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_NULL, STXT_RETURN,FALSE);
#endif
        MMK_SetAtvCtrl(win_id, MMIPHONE_DISPLAY_IMEI_CTRL_ID);
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}

/*****************************************************************************/
// 	Description : 处理输入的Puk and new pin码，除去开关机reponse PIN的情况
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL void HandleOperatePukAndNewPin(
                                     MMI_OPERATE_PIN_T  *operate_pin_info,
                                     MMI_STRING_T      *puk_ptr,
                                     MMI_STRING_T      *pin_ptr
                                     )
{
    MN_BLOCK_T      puk_value = {0};
    MN_BLOCK_T      pin_value = {0};
    MMI_STRING_T    wait_text = {0};
    
    if(PNULL == pin_ptr ||PNULL == puk_ptr ||PNULL == operate_pin_info)
    {
        //SCI_TRACE_LOW:"HandleOperatePukAndNewPin pin_ptr=%d, puk_ptr= %d , operate_pin_info=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_WINTAB_2865_112_2_18_2_45_34_234,(uint8*)"ddd", pin_ptr, puk_ptr, operate_pin_info);
        return;
    }
    
    //转换PUK的结构
    puk_value.blocklen = puk_ptr->wstr_len;

    MMI_WSTRNTOSTR((uint8 *)puk_value.blockbuf,
        MN_MAX_BLOCK_LEN,
        puk_ptr->wstr_ptr,
        puk_value.blocklen,
        puk_value.blocklen);

    //转换PIN的结构
    pin_value.blocklen = pin_ptr->wstr_len;

    MMI_WSTRNTOSTR((uint8 *)pin_value.blockbuf,
        MN_MAX_BLOCK_LEN,
        pin_ptr->wstr_ptr,
        pin_value.blocklen,
        pin_value.blocklen);

    //operate pin
    MNPHONE_OperatePinEx(MMIAPIPHONE_GetCurHandlingPinSimID(), MNSIM_PIN_UNBLOCK,operate_pin_info->pin_num,&puk_value,&pin_value);
    
    MMK_CloseWin(PHONE_PIN_WAIT_WIN_ID);
    
    //进入等待界面
    MMI_GetLabelTextByLang(TXT_COMMON_WAITING, &wait_text);
    MMIPUB_OpenWaitWin(1,&wait_text,PNULL,PNULL,PHONE_PIN_WAIT_WIN_ID,IMAGE_NULL,
        ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_NONE,HandlePukWaitingWindow);
}

/*****************************************************************************/
// 	Description : PIN修改PIN，除去开关机reponse PIN的情况
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL void HandleOperatePinAndNewPin(
                                     MMI_OPERATE_PIN_T  *operate_pin_info,
                                     MMI_STRING_T      *old_pin_ptr,
                                     MMI_STRING_T      *new_pin_ptr
                                     )
{
    MN_BLOCK_T      old_pin_value = {0};
    MN_BLOCK_T      new_pin_value = {0};
    MMI_STRING_T    wait_text = {0};
    
    if(PNULL == old_pin_ptr ||PNULL == new_pin_ptr ||PNULL == operate_pin_info)
    {
        //SCI_TRACE_LOW:"HandleOperatePinAndNewPin pin_ptr=%d, puk_ptr= %d , operate_pin_info=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_WINTAB_2914_112_2_18_2_45_34_235,(uint8*)"ddd", old_pin_ptr, new_pin_ptr, operate_pin_info);
        return;
    }
    
    //转换old PIN的结构
    old_pin_value.blocklen = old_pin_ptr->wstr_len;

    MMI_WSTRNTOSTR((uint8 *)old_pin_value.blockbuf,
        MN_MAX_BLOCK_LEN,
        old_pin_ptr->wstr_ptr,
        old_pin_value.blocklen,
        old_pin_value.blocklen);

    //转换new PIN的结构
    new_pin_value.blocklen = new_pin_ptr->wstr_len;

    MMI_WSTRNTOSTR((uint8 *)new_pin_value.blockbuf,
        MN_MAX_BLOCK_LEN,
        new_pin_ptr->wstr_ptr,
        new_pin_value.blocklen,
        new_pin_value.blocklen);
        
#ifdef MMI_SIM_LOCK_SUPPORT
    SCI_MEMCPY(&g_pin_value[MMIAPIPHONE_GetCurHandlingPinSimID()],
                         &new_pin_value,
                         sizeof(MN_BLOCK_T)); //记录新的pin1码
#endif    
    //operate pin
    MNPHONE_OperatePinEx(MMIAPIPHONE_GetCurHandlingPinSimID(), MNSIM_PIN_CHANGE,operate_pin_info->pin_num,&old_pin_value,&new_pin_value);
    
    MMK_CloseWin(PHONE_PIN_WAIT_WIN_ID);
    
    //进入等待界面
    MMI_GetLabelTextByLang(TXT_COMMON_WAITING, &wait_text);
    MMIPUB_OpenWaitWin(1,&wait_text,PNULL,PNULL,PHONE_PIN_WAIT_WIN_ID,IMAGE_NULL,
        ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_NONE,PNULL);
}
/*****************************************************************************/
// 	Description : Get Pin/PUK Remain string
//	Global resource dependence : 
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC void MMIPHONE_GetPinPukRemainStr(MN_DUAL_SYS_E dual_sys,
                                                                                    MMI_STRING_T *pin_puk_str_ptr,
                                                                                    MNSIM_PIN_INDICATION_E pin_ind,
                                                                                    BOOLEAN chv_is_blocked)
{
    uint16 pin_puk_remain = 0;
    MMI_STRING_T remain_str = {0};
    MMI_STRING_T times_str = {0};
    uint8 pin_puk_remain_ptr[MMIPHONE_MAX_PIN_PUK_REMAIN_TIMES + 3] = {0};
    uint16 length = 0;

    if(PNULL == pin_puk_str_ptr)
    {
        //SCI_TRACE_LOW:"HandleOperatePukAndNewPin  pin_puk_str_ptr is NULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_WINTAB_2967_112_2_18_2_45_34_236,(uint8*)"");
        return;
    }

    pin_puk_remain = MNSIM_GetPinOrPukFalseRemainEx(dual_sys, pin_ind, chv_is_blocked);

    //SCI_TRACE_LOW:"MMI_PHONE: pin_puk_remain=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_WINTAB_2973_112_2_18_2_45_34_237,(uint8*)"d", pin_puk_remain);

    MMI_GetLabelTextByLang(TXT_PIN_PUK_REMAIN, &remain_str);
    MMI_GetLabelTextByLang(TXT_PIN_PUK_TIMES, &times_str);

    sprintf((char *)pin_puk_remain_ptr, " %d ", pin_puk_remain);

    length = strlen( (char *)pin_puk_remain_ptr );

    pin_puk_str_ptr->wstr_ptr = SCI_ALLOC_APPZ((remain_str.wstr_len + length + times_str.wstr_len) * sizeof(wchar));
    //SCI_MEMSET(pin_puk_str_ptr->wstr_ptr, 0x00, ((remain_str.wstr_len + length + times_str.wstr_len) * sizeof(wchar)));

    MMI_WSTRNCPY(pin_puk_str_ptr->wstr_ptr, remain_str.wstr_len, remain_str.wstr_ptr, remain_str.wstr_len, remain_str.wstr_len);
    pin_puk_str_ptr->wstr_len = remain_str.wstr_len;
    
    MMI_STRNTOWSTR( pin_puk_str_ptr->wstr_ptr+pin_puk_str_ptr->wstr_len, length, (uint8 *)pin_puk_remain_ptr, MMIPHONE_MAX_PIN_PUK_REMAIN_TIMES, length );
    pin_puk_str_ptr->wstr_len += length;
    
    MMI_WSTRNCPY((pin_puk_str_ptr->wstr_ptr+pin_puk_str_ptr->wstr_len), times_str.wstr_len, times_str.wstr_ptr, times_str.wstr_len, times_str.wstr_len);
    pin_puk_str_ptr->wstr_len += times_str.wstr_len;
}

/*****************************************************************************/
// 	Description : HandlePoweroffQueryWin
//	Global resource dependence :                                
//      Author: wancan.you
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePoweroffQueryWin(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;

    switch(msg_id)
    {
    case MSG_APP_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
        MMIPHONE_PowerOff();
        break;

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;

    default:
        result = MMIPUB_HandleQueryWinMsg(win_id,msg_id,param);
        break;
    }

    return result;
}

/*****************************************************************************/
// 	Description : open query power off 
//	Global resource dependence : 
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC void MMIPHONE_OpenQueryPowerOff(MMIPHONE_QUERY_POWER_OFF_TYPE_E query_type)
{
    MMI_TEXT_ID_T text_id = TXT_NULL;
    MMI_WIN_ID_T win_id = MMIPHONE_QUERY_POWEROFF_WIN_ID;

    //SCI_TRACE_LOW:"MMIPHONE_OpenQueryPoweroff=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_WINTAB_3035_112_2_18_2_45_34_238,(uint8*)"d", query_type);

    if (MMIPHONE_QUERY_POWER_OFF_DOWNLOAD == query_type)
    {
        text_id = TXT_QUERY_POWEROFF_DOWNLOAD;
    }

    if (TXT_NULL != text_id)
    {
        MMK_CloseWin(win_id);
        MMIPUB_OpenQueryWinByTextId(text_id, IMAGE_PUBWIN_QUERY, &win_id, HandlePoweroffQueryWin);
    }
    else
    {
        MMIPHONE_PowerOff();
    }
}

/*****************************************************************************/
// 	Description : Open IMEI Display Win
//	Global resource dependence : 
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_OpenIMEIDisplayWin(void)
{
    MMK_CreateWin((uint32*)MMIPHONE_DISPLAY_IMEI_WIN_TAB, PNULL);
}

/*****************************************************************************/
//  Description : Append IMEI Detail
//  Global resource dependence : none
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL void AppendIMEIDetail(MMI_CTRL_ID_T ctrl_id)
{
    uint16 index = 0;
    GUIRICHTEXT_ITEM_T item_data = {0};/*lint !e64*/
    MMI_STRING_T string = {0};
    wchar item_text[GUIRICHTEXT_TEXT_MAX_LEN + 1] = {0};
    MN_IMEISVN_T imeisvn = {0};/*lint !e64*/
    uint8 imei_str[(MN_MAX_IMEI_LENGTH<<1) + 1] = {0};
    // wchar imei_wstr[(MN_MAX_IMEI_LENGTH<<1) + 1] = {0};
    uint16 imei_len = 0;
    uint8 svn_str[(MN_MAX_IMEI_LENGTH<<1) + 1] = {0};
    wchar svn_wstr[(MN_MAX_IMEI_LENGTH<<1) + 1] = {0};
    wchar imei_string[(MN_MAX_IMEI_LENGTH<<1) + 1] = {0};
    uint16 svn_len = 0;
    uint32 i = MN_DUAL_SYS_1;

#ifdef ADULT_WATCH_SUPPORT
    GUIRICHTEXT_SetBgColor(ctrl_id,MMI_WHITE_COLOR);
#endif
    item_data.img_type = GUIRICHTEXT_IMAGE_NONE;
    item_data.text_type = GUIRICHTEXT_TEXT_BUF; 

    //IMEI:
    for(i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
    {
        if (MNNV_GetIMEISVNEx(i, &imeisvn))
        {
            MMI_GetLabelTextByLang(TXT_IMEI_DISPLAY, &string);

            item_data.text_data.buf.len = MIN(GUIRICHTEXT_TEXT_MAX_LEN, string.wstr_len);
            item_data.text_data.buf.str_ptr = string.wstr_ptr;

            GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);

            MMIAPICOM_BcdToStr(PACKED_LSB_FIRST, (uint8 *)imeisvn.imei, MN_MAX_IMEI_LENGTH<<1, (char*)imei_str);
            
            imei_len = (uint16)strlen((char *)imei_str) - 1;

            item_data.text_data.buf.len = MIN(GUIRICHTEXT_TEXT_MAX_LEN, imei_len);

            MMI_STRNTOWSTR(imei_string,
                           GUIRICHTEXT_TEXT_MAX_LEN,
                           (uint8 *)&imei_str[1],
                           imei_len,
                           item_data.text_data.buf.len);

            item_data.text_data.buf.str_ptr = imei_string;

            GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);
        }
        else
        {
            //SCI_TRACE_LOW:"AppendIMEIDetail Read MN_DUAL_SYS_%d MNNV_GetIMEISVNEx Error!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_WINTAB_3115_112_2_18_2_45_34_239,(uint8*)"d", i + 1);
        }
    }
    
    //IMEISV:
    if (MNNV_GetIMEISVNEx(MN_DUAL_SYS_1, &imeisvn))
    {
        if (imeisvn.svn_flag)
        {
            item_data.text_frame_type = GUIRICHTEXT_FRAME_NONE;

            MMI_GetLabelTextByLang(TXT_IMEISVN_DISPLAY, &string);

            SCI_MEMSET(item_text, 0, sizeof(item_text));
            item_data.text_data.buf.len = MMIAPICOM_CopyStr2Buf(item_text, &string, GUIRICHTEXT_TEXT_MAX_LEN);
            item_data.text_data.buf.str_ptr = item_text;

            GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);

            item_data.text_frame_type = GUIRICHTEXT_FRAME_NONE;

            sprintf((char *)svn_str, "%d", imeisvn.svn);
            
            svn_len = (uint16)strlen((char *)svn_str);

            MMI_STRNTOWSTR( svn_wstr,
                                        (MN_MAX_IMEI_LENGTH<<1),
                                        (uint8 *)svn_str,
                                        svn_len,
                                        svn_len );

            item_data.text_data.buf.str_ptr = (wchar *)svn_wstr;
            item_data.text_data.buf.len = svn_len;

            GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);
        }
        else
        {
            //SCI_TRACE_LOW:"AppendIMEIDetail SVN Not Exist!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_WINTAB_3153_112_2_18_2_45_35_240,(uint8*)"");
        }
    }
    else
    {
        //SCI_TRACE_LOW:"AppendIMEIDetail Read MN_DUAL_SYS_1MNNV_GetIMEISVNEx Error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_WINTAB_3158_112_2_18_2_45_35_241,(uint8*)"");
    }
}


/*****************************************************************************/
// 	Description : handle udisk exception
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_HandleUdiskException(
                                          BOOLEAN   is_assert
                                          )
{
#ifdef MMI_UDISK_MEM_ENABLE
    uint16              str_rect_height = 0;
    GUI_RECT_T          rect = MMITHEME_GetFullScreenRect();
    GUI_RECT_T          str_rect = MMITHEME_GetFullScreenRect();
    MMI_STRING_T        prompt_str = {0};
    GUISTR_STYLE_T      str_style  = {0};
    GUISTR_STATE_T      str_state  = GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE;
    const GUI_LCD_DEV_INFO  *lcd_dev_ptr = MMITHEME_GetDefaultLcdDev();

    //judge need erase
#ifdef WIN32
    if (0)
#else
    if (Umem_GetExceptionState())
#endif
    {
        MMIAPISET_UpdateLCDContrast(GUI_MAIN_LCD_ID, MMISET_CONTRAST_FIVE);
        
        if (is_assert)
        {
            //open lcd backlight
            MMIDEFAULT_InitDefault(FALSE);
            MMIDEFAULT_TurnOnLCDBackLight();
        }
        
        //MMIAPISET_InitLanguageSetting();

        //fill color
        GUI_FillRect(lcd_dev_ptr,
            rect,
            MMI_BLACK_COLOR);

        //ge string rect height
        str_rect_height = (int16)((rect.bottom+1) / 3);

        //set string rect
        str_rect.bottom = (int16)(str_rect_height - 1);
        
        //set string style
        str_style.font       =   MMI_DEFAULT_TEXT_FONT;
        str_style.font_color =   MMI_WHITE_COLOR;
        str_style.align      =   ALIGN_HVMIDDLE;

        //get string
        MMI_GetLabelTextByLang(TXT_STARTUP_ERASE_WAITING1,&prompt_str);

        //display string
        GUISTR_DrawTextToLCDInRect(lcd_dev_ptr, &str_rect, &str_rect, &prompt_str, &str_style, str_state, GUISTR_TEXT_DIR_AUTO);

        //set string rect
        str_rect.top    = (int16)(str_rect.bottom + 1);
        str_rect.bottom = (int16)(str_rect.top + str_rect_height - 1);

        //get string
        MMI_GetLabelTextByLang(TXT_STARTUP_ERASE_WAITING2,&prompt_str);

        //display string
        GUISTR_DrawTextToLCDInRect(lcd_dev_ptr, &str_rect, &str_rect, &prompt_str, &str_style, str_state, GUISTR_TEXT_DIR_AUTO);

        //set string rect
        str_rect.top    = (int16)(str_rect.bottom + 1);
        str_rect.bottom = MMI_MAINSCREEN_BOTTOM_MAX_PIXEL;

        //get string
        MMI_GetLabelTextByLang(TXT_COMMON_WAITING,&prompt_str);

        //display string
        GUISTR_DrawTextToLCDInRect(lcd_dev_ptr, &str_rect, &str_rect, &prompt_str, &str_style, str_state, GUISTR_TEXT_DIR_AUTO);

        MMITHEME_UpdateRect();
        
        //do erase ,after display waiting window
#ifndef WIN32
        Umem_ExceptionHandler();
#endif
    }
#endif
}

/*****************************************************************************/
// 	Description : PIN/PUK input window softkey
//	Global resource dependence : 
//  Author: Jassmine
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN PinPukInputWinSoftkey(
                                      wchar         *str_ptr,   //in
                                      uint16        str_len,    //in
                                      MMI_TEXT_ID_T *text_id_ptr//in/out:
                                      )
{
    BOOLEAN  result = FALSE;
    BOOLEAN is_input_emergency_call = FALSE;
    uint16 emergency_call_len = 0;
    MMI_STRING_T    str_info = {0};

    //set string info
    str_info.wstr_ptr = str_ptr;
    str_info.wstr_len = str_len;

    is_input_emergency_call = MMIAPICC_IsInputEmergencyCall(&str_info, &emergency_call_len, MMIAPIPHONE_GetCurResponsePinSimID());
    if ((is_input_emergency_call) &&
        (str_len == emergency_call_len))
    {
        *text_id_ptr = TXT_SOS;
        result = TRUE;
    }
    else if (str_len >= MMI_PIN_MIN_LENGTH)
    {
        *text_id_ptr = TXT_COMMON_OK;
        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
// 	Description : Make Emergency Call
//	Global resource dependence : 
//  Author: Jassmine
//	Note: 
/*****************************************************************************/
LOCAL void MakeEmergencyCall(MMI_STRING_T *str_info)
{
    uint8 *emergency_call_ptr = PNULL;
    CC_RESULT_E cc_result = CC_RESULT_SUCCESS;

    SCI_TRACE_LOW("MakeEmergencyCall Enter");

    if (PNULL == str_info || PNULL == str_info->wstr_ptr || 0 == str_info->wstr_len)
    {
        return;
    }

    emergency_call_ptr = (uint8 *)SCI_ALLOCAZ(str_info->wstr_len + 1);
    if (PNULL == emergency_call_ptr)
    {
        return;
    }

    //SCI_MEMSET(emergency_call_ptr, 0x00, (str_info->wstr_len + 1));

    MMI_WSTRNTOSTR(emergency_call_ptr,
                                str_info->wstr_len,
                                str_info->wstr_ptr,
                                str_info->wstr_len,
                                str_info->wstr_len);

    cc_result = MMIAPICC_MakeCall(
                                        MMIAPIPHONE_GetCurResponsePinSimID(),
                                        emergency_call_ptr,
                                        (uint8)str_info->wstr_len,
                                        PNULL,
                                        PNULL,
                                        MMIAPICC_GetCallSimType(MMIAPIPHONE_GetCurResponsePinSimID()),
                                        CC_CALL_NORMAL_CALL,
                                        PNULL
                                        );

    SCI_FREE(emergency_call_ptr);
    emergency_call_ptr = PNULL;

    SCI_TRACE_LOW("MakeEmergencyCall exit, cc_result = %d",cc_result);
}
#ifdef FLASH_SUPPORT
/*****************************************************************************/
// 	Description : 设置一个标志变量，表示正常的开机动画播放完成后没有进入IDLE，其他APP将负责启动IDLE
//	Global resource dependence : 
//  Author: dayong.yang
//	Note:  
/*****************************************************************************/
void MMIAPIPHONE_SetOpenIdleFlag(BOOLEAN flag)
{
	s_openIdleFlag = flag;
}
/*****************************************************************************/
// 	Description : 查询上面函数设置的标志变量
//	Global resource dependence : 
//  Author: dayong.yang
//	Note:  
/*****************************************************************************/
BOOLEAN MMIAPIPHONE_GetOpenIdleFlag(void)
{
   return s_openIdleFlag;
}
/*****************************************************************************/
// 	Description : 设置屏幕删除回调函数
//	Global resource dependence : 
//  Author: dayong.yang
//	Note:  
/*****************************************************************************/
void MMIAPIPHONE_SetScrDelCallBack(MMI_HANDLE_T win_id,pFunc func)
{
   uint16 i;

   win_id = MMK_ConvertIdToHandle( win_id );
   for(i = 0;i < MAX_SUPPORT_SCR_NUMS;i++)//查找原先注册过的回调函数并替换
   {
      if(s_callbackarray[i].win_id == win_id)
      {
         s_callbackarray[i].func = func;
		 s_callbackarray[i].is_valid = TRUE;
		 break;
      }
   }
   
   if(i == MAX_SUPPORT_SCR_NUMS)//新注册一个
   {
      for(i = 0;i < MAX_SUPPORT_SCR_NUMS;i++)
      {
         if(!s_callbackarray[i].is_valid)
         {
            s_callbackarray[i].win_id = win_id;
			s_callbackarray[i].func = func;
		 	s_callbackarray[i].is_valid = TRUE;
			break;
         }
      }
   }

   if(i == MAX_SUPPORT_SCR_NUMS)//溢出
   {
      //SCI_TRACE_LOW:"MMIAPIPHONE_SetScrDelCallBack OVERFLOW"
      SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_WINTAB_3378_112_2_18_2_45_35_242,(uint8*)"");
      //SCI_ASSERT(0); /*assert verified*/
   }
   
}
/*****************************************************************************/
// 	Description : 执行屏幕删除回调函数
//	Global resource dependence : 
//  Author: dayong.yang
//	Note:  
/*****************************************************************************/
void MMIAPIPHONE_ScrDelCallBack(MMI_HANDLE_T win_id)
{
  uint16 i;

  win_id = MMK_ConvertIdToHandle( win_id );
  for(i = 0;i < MAX_SUPPORT_SCR_NUMS;i++)//查找原先注册过的回调函数并执行
  {
     if(s_callbackarray[i].win_id == win_id)
      {
         if(s_callbackarray[i].func)
         {
           (*s_callbackarray[i].func)();
         }
		 s_callbackarray[i].is_valid = FALSE;
		 s_callbackarray[i].func = PNULL;
		 break;
      }
  }
  
}
/*****************************************************************************/
// 	Description : 尝试进入idle
//	Global resource dependence : 
//  Author: dayong.yang
//	Note:  
/*****************************************************************************/
void MMIAPIPHONE_AtemptEnterIdle(void)
{
	if(!MMK_IsOpenWin(PHONE_PIN_INPUT_WIN_ID)&&//pin 码
	!MMK_IsOpenWin(PHONE_PIN_ALERT_WIN_ID)&&//Pin Block
	!MMK_IsOpenWin(PHONE_PUK_INPUT_WIN_ID)&&//puk 码
	!MMK_IsOpenWin(MMIMSD_INIT_INPUT_PSW_WIN_ID)&&//防盗
	!MMK_IsOpenWin(MMIUDISK_USB_OPER_SELECT_WIN_ID)//USB
#ifdef MMI_SIM_LOCK_SUPPORT
      &&!MMK_IsOpenWin(PHONE_SIM_LOCK_PASSWORD_INPUTING_WIN_ID)
      &&!MMK_IsOpenWin(PHONE_SIM_LOCK_COUNTDOWN_WIN_ID)
#endif	   
	)
	{
		MMIAPIIDLE_OpenIdleWin();//进入IDLE
	}			
}
/*****************************************************************************/
// 	Description : 尝试进入idle ---用于usb
//	Global resource dependence : 
//  Author: dayong.yang
//	Note:  
/*****************************************************************************/
void MMIAPIPHONE_AtemptEnterIdleForUsb(void)
{
	if(!MMK_IsOpenWin(PHONE_PIN_INPUT_WIN_ID)&&//pin 码
	!MMK_IsOpenWin(PHONE_PIN_ALERT_WIN_ID)&&//Pin Block
	!MMK_IsOpenWin(PHONE_PUK_INPUT_WIN_ID)&&//puk 码
	!MMK_IsOpenWin(MMIMSD_INIT_INPUT_PSW_WIN_ID)//防盗
#ifdef MMI_SIM_LOCK_SUPPORT
      &&!MMK_IsOpenWin(PHONE_SIM_LOCK_PASSWORD_INPUTING_WIN_ID)
      &&!MMK_IsOpenWin(PHONE_SIM_LOCK_COUNTDOWN_WIN_ID)
#endif	   
	)
	{
		MAIN_OpenIdleWin();//进入IDLE
	}			
}
/*****************************************************************************/
// 	Description : 尝试进入idle ---用于防盗
//	Global resource dependence : 
//  Author: dayong.yang
//	Note:  
/*****************************************************************************/
void MMIAPIPHONE_AtemptEnterIdleForLock(void)
{
	if(!MMK_IsOpenWin(PHONE_PIN_INPUT_WIN_ID)&&//pin 码
	!MMK_IsOpenWin(PHONE_PIN_ALERT_WIN_ID)&&//Pin Block
	!MMK_IsOpenWin(PHONE_PUK_INPUT_WIN_ID)&&//puk 码
	!MMK_IsOpenWin(MMIUDISK_USB_OPER_SELECT_WIN_ID)//USB
#ifdef MMI_SIM_LOCK_SUPPORT
      &&!MMK_IsOpenWin(PHONE_SIM_LOCK_PASSWORD_INPUTING_WIN_ID)
      &&!MMK_IsOpenWin(PHONE_SIM_LOCK_COUNTDOWN_WIN_ID)
#endif	   
	)
	{
		MMIAPIIDLE_OpenIdleWin();//进入IDLE
	}			
}
#endif

#ifdef MMI_SIM_LOCK_SUPPORT
/*****************************************************************************/
// 	Description : SIM Lock的情况下输入密码窗口
//	Global resource dependence : 
//  Author: dave.ruan
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSIMLockedPasswordInuptWin(
                                                MMI_WIN_ID_T     win_id, 	// Pin boloked窗口的ID
                                                MMI_MESSAGE_ID_E 	msg_id, 	// 窗口的内部消息ID
                                                DPARAM 		        param		// 相应消息的参数
                                                )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    MMI_STRING_T sim_unlocking_value = {0};   
    MMI_STRING_T sim_unlocking_remain_str = {0};
    GUI_BOTH_RECT_T edit_both_rect = {0};
    BOOLEAN is_input_emergency_call = FALSE;
    uint16 emergency_call_len = 0;
    MMI_OPERATE_SIM_LOCK_T   *operate_sim_lock_ptr = (MMI_OPERATE_SIM_LOCK_T* )MMK_GetWinAddDataPtr(win_id);
    MMI_TEXT_ID_T   title_text_id = TXT_NULL;
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMIDEFAULT_AllowOpenKeylocWin(FALSE);
#ifdef MAINLCD_SIZE_128X64 
        {
            GUI_BOTH_RECT_T both_rect = GetPasswordLabelBothRect();
            GUI_RECT_T title_rect = GetPasswordTitleRect();

            GUIWIN_SetTitleRect(win_id, title_rect);	
            GUIAPICTRL_SetBothRect(MMIPHONE_SIM_LOCK_PASSWORD_LABEL_CTRL_ID, &both_rect);
        }
#else					
        {
            GUI_BOTH_RECT_T both_rect = MMITHEME_GetWinClientBothRect(win_id);

            both_rect.h_rect.top += MMIPHONE_PIN_PUK_REMAIN_TOP_MARGIN;
            both_rect.v_rect.top += MMIPHONE_PIN_PUK_REMAIN_TOP_MARGIN;
            both_rect.h_rect.bottom = both_rect.h_rect.top + MMIPHONE_PIN_PUK_REMAIN_HEIGHT;
            both_rect.v_rect.bottom = both_rect.v_rect.top + MMIPHONE_PIN_PUK_REMAIN_HEIGHT;

            GUIAPICTRL_SetBothRect(MMIPHONE_SIM_LOCK_PASSWORD_LABEL_CTRL_ID, &both_rect);
        }
#endif
        IGUICTRL_SetState(MMK_GetCtrlPtr(MMIPHONE_SIM_LOCK_PASSWORD_LABEL_CTRL_ID), GUICTRL_STATE_DISABLE_ACTIVE, TRUE);

        edit_both_rect = GetSIMUnlockEditRect(win_id, TRUE);
        GUIAPICTRL_SetBothRect(PHONE_SIM_LOCK_PASSWORD_EDITBOX_CTRL_ID, &edit_both_rect);

        GUIEDIT_SetPasswordStyle(PHONE_SIM_LOCK_PASSWORD_EDITBOX_CTRL_ID,GUIEDIT_PASSWORD_STYLE_DIGITAL);
        /*set title*/
        GetSIMLockInputPasswordTitle(operate_sim_lock_ptr->lock_type,&title_text_id);
        
        GUIWIN_SetTitleTextId(win_id,title_text_id,FALSE);

        //set left softkey
        GUIEDIT_SetSoftkey(PHONE_SIM_LOCK_PASSWORD_EDITBOX_CTRL_ID, 0, MMIPHONE_SIM_UNLOCK_PCK_MIN_KEY_LEN, TXT_NULL, TXT_COMMON_OK, SIMUnlockInputWinSoftkey);

        SCI_MEMSET(&sim_unlocking_remain_str, 0x00, sizeof(MMI_STRING_T));

        GetSIMLockRemainStr(operate_sim_lock_ptr->dual_sys, &sim_unlocking_remain_str, operate_sim_lock_ptr->lock_type);

        if(PNULL != sim_unlocking_remain_str.wstr_ptr)
        {
            GUILABEL_SetText(MMIPHONE_SIM_LOCK_PASSWORD_LABEL_CTRL_ID,&sim_unlocking_remain_str,FALSE);
            /*sim_unlocking_remain_str.wstr_ptr is alloced in GetSIMLockRemainStr*/
            SCI_FREE(sim_unlocking_remain_str.wstr_ptr);
        }

        MMK_SetWinDisplayStyleState(win_id, WS_DISABLE_RETURN_WIN, TRUE);
        
        MMK_SetAtvCtrl(win_id,PHONE_SIM_LOCK_PASSWORD_EDITBOX_CTRL_ID);
        break;
        
    case MSG_FULL_PAINT:
#ifndef MMI_PDA_SUPPORT
        SetPasswordBackground(win_id,PHONE_SIM_LOCK_PASSWORD_EDITBOX_CTRL_ID,TRUE);
#endif
        SCI_MEMSET(&sim_unlocking_remain_str, 0x00, sizeof(MMI_STRING_T));
        GetSIMLockRemainStr(operate_sim_lock_ptr->dual_sys, &sim_unlocking_remain_str, operate_sim_lock_ptr->lock_type);
        if(PNULL != sim_unlocking_remain_str.wstr_ptr)
        {
            GUILABEL_SetText(MMIPHONE_SIM_LOCK_PASSWORD_LABEL_CTRL_ID,&sim_unlocking_remain_str,FALSE);
            /*sim_unlocking_remain_str.wstr_ptr is alloced in GetSIMLockRemainStr*/
            SCI_FREE(sim_unlocking_remain_str.wstr_ptr);
        } 
#ifndef SUBLCD_SIZE_NONE        
        //刷新小屏的显示
        MMISUB_IsPermitUpdate(TRUE);
        MMISUB_SetSubLcdDisplay(FALSE,TRUE,SUB_CONTENT_SIM_LOCK,PNULL);
#endif
        break;
        
    case MSG_GET_FOCUS:
        MMIDEFAULT_AllowOpenKeylocWin(FALSE);
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_APP_GREEN:
    case MSG_APP_CALL2:
    case MSG_APP_CALL3:
    case MSG_APP_CALL4:
        {            
            // get the value about SIM Lock
            GUIEDIT_GetString(PHONE_SIM_LOCK_PASSWORD_EDITBOX_CTRL_ID,&sim_unlocking_value);

            is_input_emergency_call = MMIAPICC_IsInputEmergencyCall(&sim_unlocking_value, &emergency_call_len, operate_sim_lock_ptr->dual_sys);

            if ((is_input_emergency_call) &&
                (sim_unlocking_value.wstr_len == emergency_call_len))
            {
                MakeEmergencyCall(&sim_unlocking_value);
                //清空password
                GUIEDIT_ClearAllStr(PHONE_SIM_LOCK_PASSWORD_EDITBOX_CTRL_ID);
            }
            else if (SIM_UNLOCK_LEN_IS_IN_RANGE(sim_unlocking_value.wstr_len) 
                         && MSG_APP_GREEN != msg_id 
                         && MSG_APP_CALL2 != msg_id
                         && MSG_APP_CALL3 != msg_id
                         && MSG_APP_CALL4 != msg_id)
            {
                HandleSIMUnlockingInputingString(operate_sim_lock_ptr,&sim_unlocking_value);
            }
            else
            {
                /*防止拨号键没有被屏蔽到,删除代码*/
                /*do nothing*/
            }
        }
        break;
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
    case MSG_APP_RED://red and flip不允许返回
        MMIPUB_OpenAlertWinByTextId(PNULL,
                                    TXT_INPUT_SIM_UNLOCKING_CODE,
                                    TXT_NULL,
                                    IMAGE_PUBWIN_WARNING,
                                    PNULL,
                                    PNULL,
                                    MMIPUB_SOFTKEY_ONE,
                                    HandleSIMUnlockingAlertWindow
                                    );
        break;

#ifdef FLIP_PHONE_SUPPORT  //close flip ring codes, just open for flip phone.        
    case MSG_APP_FLIP:
        MMIDEFAULT_HandleFlipKey(FALSE);
        break;
#endif        
        
    case MSG_CLOSE_WINDOW:
        MMIDEFAULT_AllowOpenKeylocWin(TRUE);
#ifndef SUBLCD_SIZE_NONE        
       //刷新小屏的显示，关闭输入PIN码
        MMISUB_SetSubLcdDisplay(FALSE,FALSE,SUB_CONTENT_SIM_LOCK,PNULL);
#endif
        MMK_FreeWinAddData(win_id);
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return (result);
}

/*****************************************************************************/
// 	Description : GetPinPukEditRect
//	Global resource dependence : 
//  Author: dave.ruan
//	Note:
/*****************************************************************************/
LOCAL GUI_BOTH_RECT_T GetSIMUnlockEditRect(MMI_WIN_ID_T win_id, BOOLEAN is_need_label)
{
#ifdef MAINLCD_SIZE_128X64 
	return GetPasswordEditBothRect(is_need_label);
#else
    GUI_BOTH_RECT_T both_rect = MMITHEME_GetWinClientBothRect(win_id);

    both_rect.h_rect.left = both_rect.v_rect.left = MMI_PASSWORD_EDIT_LEFT;
    if (is_need_label)
    {
        both_rect.h_rect.top += MMIPHONE_PIN_PUK_REMAIN_TOP_MARGIN + MMIPHONE_PIN_PUK_REMAIN_HEIGHT + MMI_PASSWORD_EDIT_TOP_MARGIN;
        both_rect.v_rect.top += MMIPHONE_PIN_PUK_REMAIN_TOP_MARGIN + MMIPHONE_PIN_PUK_REMAIN_HEIGHT + MMI_PASSWORD_EDIT_TOP_MARGIN;
    }
    else
    {
        both_rect.h_rect.top += MMI_PASSWORD_EDIT_TOP_MARGIN;
        both_rect.v_rect.top += MMI_PASSWORD_EDIT_TOP_MARGIN;
    }

    both_rect.h_rect.bottom = both_rect.h_rect.top + MMI_PASSWORD_EDIT_HEIGHT;
    both_rect.h_rect.right = both_rect.h_rect.right - MMI_PASSWORD_EDIT_LEFT;
    both_rect.v_rect.bottom = both_rect.v_rect.top + MMI_PASSWORD_EDIT_HEIGHT;
    both_rect.v_rect.right = both_rect.v_rect.right - MMI_PASSWORD_EDIT_LEFT;

    return both_rect;
#endif 
		
}

/*****************************************************************************/
// 	Description : Get SIM lock Remain string
//	Global resource dependence : 
//  Author: dave.ruan
//	Note:
/*****************************************************************************/
LOCAL void GetSIMLockRemainStr(MN_DUAL_SYS_E dual_sys,
                                MMI_STRING_T *sim_lock_str_ptr,
                                uint32 unlock_type      //IN: Network/NetworkSubset/SP/CORPORATE/USER
                                )
{
    MMI_STRING_T remain_str = {0};
    MMI_STRING_T times_str = {0};
    uint8 sim_unlock_remain_ptr[MMIPHONE_SIM_UNLOCK_CODE_MAX_LEN ] = {0};
    uint16 length = 0;
    MN_SIM_LOCK_STATUS_T *sim_unlock_data = MMIAPIPHONE_GetSIMUnlockConfigDataPtr();
    uint32 sim_unlock_trials_remain = 0;
    uint32 unlock_timer = 0;
    int32  left_trials = 0;
    
    if(PNULL == sim_lock_str_ptr)
    {
        return;
    }

    //SCI_TRACE_LOW:"MMIPHONE:GetSIMLockRemainStr  max_num_trials=%d,time_interval= %d,time_multiplier= %d,cck_trials=%d,nck_trials=%d,nsck_trials=%d,pck_trials=%d,spck_trials=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_WINTAB_3699_112_2_18_2_45_36_243,(uint8*)"dddddddd", sim_unlock_data->max_num_trials,sim_unlock_data->time_interval,sim_unlock_data->time_multiplier,      sim_unlock_data->cck_trials,sim_unlock_data->nck_trials,sim_unlock_data->nsck_trials,sim_unlock_data->pck_trials,sim_unlock_data->spck_trials);
   
    MMI_GetLabelTextByLang(TXT_PIN_PUK_REMAIN, &remain_str);
    MMI_GetLabelTextByLang(TXT_PIN_PUK_TIMES, &times_str);
    /*get trials and unlock time value*/
    MMIAPIPHONE_GetSIMLockRemainTrialsTimer(unlock_type,&sim_unlock_trials_remain,&unlock_timer);
    left_trials = sim_unlock_data->max_num_trials - sim_unlock_trials_remain;
    
    sprintf((char *)sim_unlock_remain_ptr, " %d ", left_trials);

    length = strlen( (char *)sim_unlock_remain_ptr);

    sim_lock_str_ptr->wstr_ptr = SCI_ALLOC_APPZ((remain_str.wstr_len + length + times_str.wstr_len) * sizeof(wchar));
    //SCI_MEMSET(sim_lock_str_ptr->wstr_ptr, 0x00, ((remain_str.wstr_len + length + times_str.wstr_len) * sizeof(wchar)));

    MMI_WSTRNCPY(sim_lock_str_ptr->wstr_ptr, remain_str.wstr_len, remain_str.wstr_ptr, remain_str.wstr_len, remain_str.wstr_len);
    sim_lock_str_ptr->wstr_len = remain_str.wstr_len;
    
    MMI_STRNTOWSTR(sim_lock_str_ptr->wstr_ptr+sim_lock_str_ptr->wstr_len, length, (uint8 *)sim_unlock_remain_ptr, MMIPHONE_MAX_PIN_PUK_REMAIN_TIMES, length);
    sim_lock_str_ptr->wstr_len += length;
    
    MMI_WSTRNCPY((sim_lock_str_ptr->wstr_ptr+sim_lock_str_ptr->wstr_len), times_str.wstr_len, times_str.wstr_ptr, times_str.wstr_len, times_str.wstr_len);
    sim_lock_str_ptr->wstr_len += times_str.wstr_len;
}

/*****************************************************************************/
// 	Description : Get SIM lock input Password Title
//	Global resource dependence : 
//  Author: dave.ruan
//	Note:
/*****************************************************************************/
LOCAL void GetSIMLockInputPasswordTitle(
                                uint32 unlock_type,      //IN: Network/NetworkSubset/SP/CORPORATE/USER
                                MMI_TEXT_ID_T *unlock_title      //out: sim unlock title
                                )
{
    if(PNULL == unlock_title)
    {
        //SCI_TRACE_LOW:"[MMIPHONE]: GetSIMLockInputPasswordTitle unlock_title = NULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_WINTAB_3735_112_2_18_2_45_36_244,(uint8*)"");
        return;
    }

    if(unlock_type&MMI_SIM_NETWORK_LOCK)
    {
        *unlock_title = TXT_SIM_LOCK_ENTER_NETWORK;
    }
    else if(unlock_type&MMI_SIM_NETWORK_SUBSET_LOCK)
    {
        *unlock_title = TXT_SIM_LOCK_ENTER_NETWORK_SUBSET;
    }
    else if(unlock_type&MMI_SIM_SP_LOCK)
    {
        *unlock_title = TXT_SIM_LOCK_ENTER_SP;
    }
    else if(unlock_type&MMI_SIM_CORPORATE_LOCK)
    {
        *unlock_title = TXT_SIM_LOCK_ENTER_CORPORATE;
    }
    else if(unlock_type&MMI_SIM_USER_LOCK)
    {
        *unlock_title = TXT_SIM_LOCK_ENTER_USER;
    }
    else
    {
        *unlock_title = TXT_SIM_LOCK_ENTER_NETWORK;
    }
    
}

/*****************************************************************************/
// 	Description : Get SIM lock Remain trials and timers
//	Global resource dependence : 
//  Author: dave.ruan
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_GetSIMLockRemainTrialsTimer(
                                uint32 unlock_type,      //IN: Network/NetworkSubset/SP/CORPORATE/USER
                                uint32 *unlock_trails,      //out: sim unlock trails
                                uint32  *unlock_timer       //out:sim unlock timer
                                )
{
    MN_SIM_LOCK_STATUS_T *sim_unlock_data = MMIAPIPHONE_GetSIMUnlockConfigDataPtr();
    
    if(PNULL == unlock_trails ||PNULL == unlock_timer)
    {
        return;
    }

    //SCI_TRACE_LOW:"MMIPHONE:MMIAPIPHONE_GetSIMLockRemainTrialsTimer  max_num_trials=%d,time_interval= %d,time_multiplier= %d,cck_trials=%d,nck_trials=%d,nsck_trials=%d,pck_trials=%d,spck_trials=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_WINTAB_3791_112_2_18_2_45_36_245,(uint8*)"dddddddd", sim_unlock_data->max_num_trials,sim_unlock_data->time_interval,sim_unlock_data->time_multiplier,      sim_unlock_data->cck_trials,sim_unlock_data->nck_trials,sim_unlock_data->nsck_trials,sim_unlock_data->pck_trials,sim_unlock_data->spck_trials);
   
    switch(unlock_type)
    {       
        case MMI_SIM_NETWORK_LOCK:
        {
            *unlock_trails =sim_unlock_data->nck_trials;
            *unlock_timer =sim_unlock_data->nck_unlock_time;
        }
        break;
        
        case MMI_SIM_NETWORK_SUBSET_LOCK:
        {
            *unlock_trails =sim_unlock_data->nsck_trials;
            *unlock_timer =sim_unlock_data->nsck_unlock_time;
        }
        break;
        
        case MMI_SIM_SP_LOCK:                
        {
            *unlock_trails =sim_unlock_data->spck_trials;
            *unlock_timer =sim_unlock_data->spck_unlock_time;
        }
        break;
        
        case MMI_SIM_CORPORATE_LOCK:
        {
            *unlock_trails =sim_unlock_data->cck_trials;
            *unlock_timer =sim_unlock_data->cck_unlock_time;
        }
        break;
        
        case MMI_SIM_USER_LOCK:
        {
            *unlock_trails =sim_unlock_data->pck_trials;
            *unlock_timer =sim_unlock_data->pck_unlock_time;
        }
        break;
        
        default:        
        break;
    }
    
}

/*****************************************************************************/
// 	Description : SIM unlock 密码输入界面，alert界面\不允许red key,flip返回
//	Global resource dependence : 
//  Author: dave.ruan
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSIMUnlockingAlertWindow(
                                        MMI_WIN_ID_T     win_id, 	// 窗口的ID
                                        MMI_MESSAGE_ID_E 	msg_id, 	// 窗口的内部消息ID
                                        DPARAM 		        param		// 相应消息的参数
                                        )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    
    switch (msg_id)
    {
    case MSG_APP_RED://red and flip不允许返回
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;

#ifdef FLIP_PHONE_SUPPORT  //close flip ring codes, just open for flip phone.
    case MSG_APP_FLIP:
        MMK_CloseWin(win_id);
        MMIDEFAULT_HandleFlipKey(FALSE);
        break;
#endif

    default:
        result = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
        break;
    }
    
    return (result);
}

/*****************************************************************************/
// 	Description : SIM unlock input window softkey
//	Global resource dependence : 
//  Author: dave.ruan
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN SIMUnlockInputWinSoftkey(
                                      wchar         *str_ptr,   //in
                                      uint16        str_len,    //in
                                      MMI_TEXT_ID_T *text_id_ptr//in/out:
                                      )
{
    BOOLEAN  result = FALSE;
    BOOLEAN is_input_emergency_call = FALSE;
    uint16 emergency_call_len = 0;
    MMI_STRING_T    str_info = {0};
    MMI_OPERATE_SIM_LOCK_T   *operate_sim_lock_ptr = PNULL;
    MMI_WIN_ID_T sim_lock_win_id = 0;
    
    if(MMK_IsOpenWin(PHONE_SIM_LOCK_PASSWORD_INPUTING_WIN_ID))
    {
        sim_lock_win_id =PHONE_SIM_LOCK_PASSWORD_INPUTING_WIN_ID;
    }
    else
    {
        sim_lock_win_id = MMI_SIM_LOCK_PASSWORD_INPUTING_WIN_ID;
    }
    operate_sim_lock_ptr = (MMI_OPERATE_SIM_LOCK_T* )MMK_GetWinAddDataPtr(sim_lock_win_id);
    if(PNULL == operate_sim_lock_ptr)
    {
        //SCI_TRACE_LOW:"SIMUnlockInputWinSoftkey operate_sim_lock_ptr is NULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_WINTAB_3898_112_2_18_2_45_36_246,(uint8*)"");
        return FALSE;    
    }
    
    if(PNULL == str_ptr || PNULL == text_id_ptr)
    {
        //SCI_TRACE_LOW:"SIMUnlockInputWinSoftkey str_ptr || text_id_ptr is NULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_WINTAB_3904_112_2_18_2_45_36_247,(uint8*)"");
        return FALSE;
    }
    //set string info
    str_info.wstr_ptr = str_ptr;
    str_info.wstr_len = str_len;

    is_input_emergency_call = MMIAPICC_IsInputEmergencyCall(&str_info, &emergency_call_len, operate_sim_lock_ptr->dual_sys);

    if ((is_input_emergency_call) &&
        (str_len == emergency_call_len)
        &&(MMK_IsOpenWin(PHONE_SIM_LOCK_PASSWORD_INPUTING_WIN_ID)))
    {
        *text_id_ptr = TXT_SOS;
        result = TRUE;
    }
    else if ((str_len >= MMIPHONE_SIM_UNLOCK_PCK_MIN_KEY_LEN)&&(operate_sim_lock_ptr->lock_type&MMI_SIM_USER_LOCK))
    {
        *text_id_ptr = TXT_COMMON_OK;
        result = TRUE;
    }
    else if (str_len >=MMIPHONE_SIM_UNLOCK_OTHER_KEY_MIN_LEN)
    {
        *text_id_ptr = TXT_COMMON_OK;
        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
// 	Description : SIM unlock 密码输入界面，输入错误提示handle函数
//	Global resource dependence : 
//  Author: dave.ruan
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSIMUnlockingAlertErrorWindow(
                                        MMI_WIN_ID_T     win_id, 	// 窗口的ID
                                        MMI_MESSAGE_ID_E 	msg_id, 	// 窗口的内部消息ID
                                        DPARAM 		        param		// 相应消息的参数
                                        )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    
    switch (msg_id)
    {
    case MSG_APP_RED://red and flip不允许返回
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;

#ifdef FLIP_PHONE_SUPPORT  //close flip ring codes, just open for flip phone.
    case MSG_APP_FLIP:
        MMK_CloseWin(win_id);
        MMIDEFAULT_HandleFlipKey(FALSE);
        break;
#endif        
        
    case MSG_CLOSE_WINDOW:
        {
            MMI_OPERATE_SIM_LOCK_T *operate_sim_lock_ptr = PNULL;
            MN_SIM_LOCK_STATUS_T *sim_unlock_data = MMIAPIPHONE_GetSIMUnlockConfigDataPtr();
            MMIPUB_INFO_T *win_info_ptr = PNULL;
            uint32 time_multipler = 1;
            uint32 trials_counter = 0;
            uint32 time_out_value = 0;
            
            win_info_ptr = (MMIPUB_INFO_T *)MMIPUB_GetWinAddDataPtr(win_id);
            if ((PNULL != win_info_ptr) && (PNULL != win_info_ptr->user_data))
            {
                operate_sim_lock_ptr = (MMI_OPERATE_SIM_LOCK_T *)win_info_ptr->user_data;
            }
            else
            {
                //SCI_TRACE_LOW:"HandleSIMUnlockingAlertErrorWindow win_info_ptr =NULL ||win_info_ptr->user_data = NULL"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_WINTAB_3978_112_2_18_2_45_36_248,(uint8*)"");
                break;
            }
            /*first update NV data to PS,then update NV data to MMI from PS */
            MMIAPIPHONE_GetSIMLockRemainTrialsTimer(operate_sim_lock_ptr->lock_type,&trials_counter,&time_out_value);
              
            /*根据输入次数计算倒计时倍数*/
            if(0 !=sim_unlock_data->time_multiplier)
            {
                uint32 i = 0;
                for(i=0;i < trials_counter; i++)
                {
                    time_multipler *=  sim_unlock_data->time_multiplier;
                }
            }
            trials_counter ++;
            
            /*计算下次输入倒计时*/
            if(0 !=sim_unlock_data->time_interval)
            {
                time_out_value = sim_unlock_data->time_interval * time_multipler;
            }
            else
            {
                /*time_out_value is 0*/
            }

            SCI_TRACE_LOW("MMIPHONE_WINTAB: HandleSIMUnlockingAlertErrorWindow, max_num_trials = %d, trials_counter = %d", sim_unlock_data->max_num_trials, trials_counter);
            
            MNPHONE_UpdateSIMUnlockRetryData(operate_sim_lock_ptr->lock_type, trials_counter, time_out_value);
            
            MMIAPIPHONE_UpdateSIMUnlockData();

            if(sim_unlock_data->max_num_trials <= trials_counter)
            {
                if(MMK_IsOpenWin(PHONE_SIM_LOCK_PASSWORD_INPUTING_WIN_ID))
                {
                    MMI_WIN_ID_T alert_win_id = PHONE_SIM_LOCK_ALERT_WIN_ID;
                    //uint32 alert_time = 0;
                    /*次数达到最大限制*/
                    MMK_CloseWin(PHONE_SIM_LOCK_PASSWORD_INPUTING_WIN_ID);
                    MMK_CloseWin(PHONE_PIN_INPUT_WIN_ID);
                    MMK_CloseWin(PHONE_PIN_WAIT_WIN_ID);
                    SCI_TRACE_LOW("MMIPHONE_WIN: alert_win_id1 = %d", alert_win_id);
                    MMK_SetWinDisplayStyleState(alert_win_id, WS_DISABLE_RETURN_WIN, TRUE);
                    MMIPUB_OpenAlertWinByTextId(PNULL,TXT_SIM_LOCKED,TXT_NULL,
                                                                    IMAGE_PUBWIN_WARNING,&alert_win_id,
                                                                    PNULL,MMIPUB_SOFTKEY_NONE,HandleSIMLocked);
                    SCI_TRACE_LOW("MMIPHONE_WIN: alert_win_id2 = %d", alert_win_id);
                }
                else
                {
                    MMK_CloseWin(MMI_SIM_LOCK_PASSWORD_INPUTING_WIN_ID);
                    /*如果在输入设置SIM Lock界面，提示SIM Locked, 返回*/
                    MMIPUB_OpenAlertWinByTextId(PNULL,TXT_SIM_LOCKED,TXT_NULL,
                                                                    IMAGE_PUBWIN_WARNING,PNULL,
                                                                    PNULL,MMIPUB_SOFTKEY_NONE,PNULL);
                }
                SCI_FREE(operate_sim_lock_ptr);
                break;// result;
            }
            
            if(MMK_IsOpenWin(PHONE_SIM_LOCK_PASSWORD_INPUTING_WIN_ID))
            {
                if(0 !=time_out_value)
                {
                    /*user data memory released when close win,we need realloc*/
                    /*close win after operate_sim_lock value set*/
                    MMK_CloseWin(PHONE_SIM_LOCK_PASSWORD_INPUTING_WIN_ID);
                    MMK_CreateWin((uint32*)SIM_LOCK_COUNTDOWN_RUN_WIN_TAB,(ADD_DATA)operate_sim_lock_ptr);
                }
                else
                {
                    MMK_SendMsg(PHONE_SIM_LOCK_PASSWORD_INPUTING_WIN_ID,MSG_FULL_PAINT,PNULL);
                }
            }
        }
        break;

    default:
        result = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
        break;
    }
    
    return (result);
}

/*****************************************************************************/
// 	Description : 处理输入的sim lockpassword
//	Global resource dependence : 
//  Author: dave.ruan
//	Note:
/*****************************************************************************/
LOCAL void HandleSIMUnlockingInputingString(
                                                MMI_OPERATE_SIM_LOCK_T *operate_sim_lock_ptr,    //是否为解锁
                                                MMI_STRING_T *sim_unlocking_ptr
                                               )
{
    SIM_LOCK_KEY_T      sim_lock_key_value = {0};
    MN_SIM_LOCK_ERROR_CODE_E sim_unlock_result =MN_SIM_LOCK_DATA_INTEGRITY_FAIL;//MN_SIM_UNLOCK_NO_ERROR;
    MN_SIM_LOCK_STATUS_T *sim_unlock_data = MMIAPIPHONE_GetSIMUnlockConfigDataPtr();
    MN_SIM_LOCK_STATUS_T sim_unlock_status = {0};
    MNSIM_ERROR_E sim_error_type = MNSIM_ERROR_SIM_NO_ERROR;
    MMI_STRING_T    wait_text = {0};
    uint32 i = 0;
    
    if(PNULL == sim_unlocking_ptr
        ||PNULL == sim_unlocking_ptr->wstr_ptr 
        ||PNULL == operate_sim_lock_ptr
        ||PNULL == sim_unlock_data
    )
    {
        //SCI_TRACE_LOW:"HandleSIMUnlockingInputingString sim_unlocking_ptr||sim_unlocking_ptr->wstr_ptr|operate_sim_lock_ptr||sim_unlock_datais null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_WINTAB_4082_112_2_18_2_45_36_249,(uint8*)"");
        return;
    }    

    SCI_MEMSET(&sim_lock_key_value, 0, sizeof(SIM_LOCK_KEY_T));
    
    //转换SIM Lock key的结构
    sim_lock_key_value.key_len = sim_unlocking_ptr->wstr_len;

    MMI_WSTRNTOSTR((uint8 *)sim_lock_key_value.keys,
                                    SIM_LOCK_KEY_MAX_LEN,
                                    sim_unlocking_ptr->wstr_ptr,
                                    sim_lock_key_value.key_len,
                                    sim_lock_key_value.key_len);

    /*if locked, unlock it*/
    if(operate_sim_lock_ptr->is_enable)
    {
        if(MMK_IsOpenWin( MMI_SIM_LOCK_PASSWORD_INPUTING_WIN_ID))
        {
            //设置界面不检查下一个
            sim_unlock_result = MNPHONE_SIMUnlockEx(operate_sim_lock_ptr->lock_type,
                                                                            sim_lock_key_value,
                                                                            PNULL,
                                                                            operate_sim_lock_ptr->dual_sys);
        }
        else
        {
            sim_unlock_result = MNPHONE_SIMUnlockEx(operate_sim_lock_ptr->lock_type,
                                                                            sim_lock_key_value,
                                                                            PNULL,//&sim_error_type,//bug 191818
                                                                            operate_sim_lock_ptr->dual_sys);
            //bug 191818
			//MMIAPIPHONE_UpdateSIMUnlockData();
            //sim_unlock_data = MMIAPIPHONE_GetSIMUnlockConfigDataPtr();
            MNPHONE_GetSIMLockStatus(&sim_unlock_status);
            if(sim_unlock_status.user_locked)
            {
                sim_error_type = MNSIM_ERROR_SIM_USER_LOCKED;
            }
            else if(sim_unlock_status.corporate_locked)
            {
                sim_error_type = MNSIM_ERROR_SIM_CORPORATE_LOCKED;
            }
            else if(sim_unlock_status.sp_locked)
            {
                sim_error_type = MNSIM_ERROR_SIM_SP_LOCKED;
            }
            else if(sim_unlock_status.network_subset_locked)
            {
                sim_error_type = MNSIM_ERROR_SIM_NETWORT_SUBSET_LOCKED;
            }
            else if(sim_unlock_status.network_locked)
            {
                sim_error_type = MNSIM_ERROR_SIM_NETWORT_LOCKED;
            }                       
        }
    }
    else
    {
        sim_unlock_result = MNPHONE_SIMLockEx(operate_sim_lock_ptr->lock_type,sim_lock_key_value,operate_sim_lock_ptr->dual_sys);
    }
    
    SCI_TRACE_LOW("HandleSIMUnlockingInputingString sim_unlock_result=%d,sim_error_type=%d,network_locked=%d,network_subset_locked=%d ,sp_locked=%d,corporate_locked=%d,user_locked=%d",sim_unlock_result,sim_error_type,
        sim_unlock_status.network_locked,sim_unlock_status.network_subset_locked,sim_unlock_status.sp_locked,sim_unlock_status.corporate_locked,sim_unlock_status.user_locked);

    SCI_TRACE_LOW("MMIPHONE_WINTAB: HandleSIMUnlockingInputingString, operate_sim_lock_ptr->is_enable = %d, sim_unlock_result = %d, sim_error_type = %d", operate_sim_lock_ptr->is_enable, sim_unlock_result, sim_error_type);

    if((MN_SIM_LOCK_NO_ERROR == sim_unlock_result) && (sim_error_type == MNSIM_ERROR_SIM_NO_ERROR))
    {
        if(MMK_IsOpenWin(PHONE_SIM_LOCK_PASSWORD_INPUTING_WIN_ID))
        {
       #if 0 //def  MMI_MULTI_SIM_SYS_SINGLE //reserved for multi SIM CARD,and USE "ifNdef  MMI_MULTI_SIM_SYS_SINGLE"
            MMIPHONE_HANDLE_SIMLOCK_STATUS_T *handle_sim_lock_status = MMIPHONE_GetHandleSIMLockStatus();
            BOOLEAN is_handling_simlock = FALSE;

            if(PNULL == handle_sim_lock_status)
            {
                return;
            }

            //add for dual sys
            handle_sim_lock_status->need_handle_simlock[operate_sim_lock_ptr->dual_sys] = FALSE;
            handle_sim_lock_status->is_handling_simlock[operate_sim_lock_ptr->dual_sys] = FALSE;

            for (i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
            {
                if (handle_sim_lock_status->is_handling_simlock[i])
                {
                    is_handling_simlock = TRUE;
                    break;
                }
            }
            
            if (!is_handling_simlock)
            {
                for (i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
                {
                    if (i != operate_sim_lock_ptr->dual_sys)
                    {
                        if (handle_sim_lock_status->need_handle_simlock[i])
                        {
                            //set s_handle_sim_lock_status
                            handle_sim_lock_status->cur_handling_sim_id = (MN_DUAL_SYS_E)i;
                            handle_sim_lock_status->is_handling_simlock[i] = TRUE;
                            
                            MMK_CloseWin(PHONE_SIM_LOCK_PASSWORD_INPUTING_WIN_ID);
                            MMK_CloseWin(PHONE_SIM_LOCK_WAIT_WIN_ID);
                            
                            //create input SIM lock window
                            MMIPHONE_CreateInputSimLockWindow(handle_sim_lock_status->sim_not_ready_ind[i]);
                            return;
                        }    
                    }
                }
            }
        #endif
        
            MMK_CloseWin(PHONE_SIM_LOCK_PASSWORD_INPUTING_WIN_ID);
            MMK_CloseWin(PHONE_SIM_LOCK_WAIT_WIN_ID);
            /*PIN active confirm, close PIN input win directly,phone will restart SIM(multi sim ignored )*/
            MMK_CloseWin(PHONE_PIN_INPUT_WIN_ID);

#ifdef MMI_RESET_PHONE_AUTO_PIN
            MMIAPIPHONE_SetUsePinInNV(TRUE);
#endif
            /*SIM Lock PASS后进入idle,防止被flash UI 冲掉输入界面*/
            //MMIAPIIDLE_OpenIdleWin();
            for (i = 0; i < MMI_DUAL_SYS_MAX; i++)
            {
                /*先deactive PS, 再重新开机*/
                //MMIAPIPHONE_DeactivePS((MN_DUAL_SYS_E)i);
                MMIAPIPHONE_PoweroffPsOrSim((MN_DUAL_SYS_E)i);
                MMIPHONE_SetPhoneNeedRestart((MN_DUAL_SYS_E)i , TRUE);
            }
            
            //进入等待界面
            MMI_GetLabelTextByLang(TXT_COMMON_WAITING, &wait_text);
            MMIPUB_OpenWaitWin(1,&wait_text,PNULL,PNULL,PHONE_SIM_LOCK_WAIT_WIN_ID,IMAGE_NULL,
                ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_NONE,HandleSIMUnlockingWaitingWindow);

            MMK_SetWinDisplayStyleState(PHONE_SIM_LOCK_WAIT_WIN_ID, WS_DISABLE_RETURN_WIN, TRUE);
        }
        else
        {
            MMK_CloseWin(MMI_SIM_LOCK_PASSWORD_INPUTING_WIN_ID);
            MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
        }
        MMIAPIPHONE_UpdateSIMUnlockData();
    }
    else if((MN_SIM_LOCK_NO_ERROR == sim_unlock_result) && (sim_error_type != MNSIM_ERROR_SIM_NO_ERROR))//存在其他的SIM Lock,enter pass word window
    {
        MMI_OPERATE_SIM_LOCK_T   *sim_lock_ptr =SCI_ALLOCAZ(sizeof(MMI_OPERATE_SIM_LOCK_T));
        if(PNULL == sim_lock_ptr)
        {
            //SCI_TRACE_LOW:"[MMIPHONE] sim_lock_ptr alloc memory fail"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_WINTAB_4204_112_2_18_2_45_37_250,(uint8*)"");
            return;
        }
        SCI_MEMSET(sim_lock_ptr,0x00,sizeof(MMI_OPERATE_SIM_LOCK_T));
        sim_lock_ptr->dual_sys = operate_sim_lock_ptr->dual_sys;
        sim_lock_ptr->lock_type = MMI_SIM_NETWORK_LOCK<<((uint32)sim_error_type - (uint32)MNSIM_ERROR_SIM_NETWORT_LOCKED);
        sim_lock_ptr->is_enable = TRUE;//这里只可能是解锁
        
        MMIAPIPHONE_UpdateSIMUnlockData();
        if(MMK_IsOpenWin(PHONE_SIM_LOCK_PASSWORD_INPUTING_WIN_ID))
        {
            MMK_CloseWin(PHONE_SIM_LOCK_PASSWORD_INPUTING_WIN_ID);
        }
        MMK_CreatePubEditWin((uint32*)SIM_LOCK_PASSWORD_INPUTING_WIN_TAB,(ADD_DATA)sim_lock_ptr);

    }
    else//密码错误,不论有没有其他Lock,提示错误
    {   
        //MMI_WIN_ID_T alert_win_id = PHONE_SIM_LOCK_ALERT_WIN_ID;
        MMI_TEXT_ID_T  sim_lock_error_alert_txt_id = TXT_NULL;
        MMI_OPERATE_SIM_LOCK_T *operate_sim_lock = SCI_ALLOCAZ(sizeof(MMI_OPERATE_SIM_LOCK_T));
        
        if(PNULL == operate_sim_lock)
        {
            //SCI_TRACE_LOW:"HandleSIMUnlockingInputingString operate_sim_lock is null"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_WINTAB_4228_112_2_18_2_45_37_251,(uint8*)"");
            return;
        }

        //清空password
        if(MMK_IsOpenWin( MMI_SIM_LOCK_PASSWORD_INPUTING_WIN_ID))
        {
            GUIEDIT_ClearAllStr(MMI_SIM_LOCK_PASSWORD_EDITBOX_CTRL_ID);
        }
        else
        {
            GUIEDIT_ClearAllStr(PHONE_SIM_LOCK_PASSWORD_EDITBOX_CTRL_ID);
        }

        switch(sim_unlock_result)
        {
            case MN_SIM_LOCK_DATA_INTEGRITY_FAIL:          // The SIM lock data not pass Integrity check
            case MN_SIM_LOCK_PARAMETER_ERROR:               // (de-)personalisation parameter error
            case MN_SIM_LOCK_UPDATE_NV_ERROR:              // (de-)personalisation update nv data error
            case MN_SIM_LOCK_ERROR_ALREADY_LOCKED:          // do personalisation but this item already locked
                sim_lock_error_alert_txt_id = TXT_SIM_LOCK_SYS_PARAM_ERROR;
                break;

            case MN_SIM_LOCK_ERROR_SIM_NOT_READY:          // SIM not ready, can't get SIM info
                sim_lock_error_alert_txt_id = TXT_SIM_UNAVAILABLE;
                break;
                
            case MN_SIM_LOCK_ERROR_GID_NOT_SUPPORT:          // do SP/CORPORATE personalisation but GID not support
                sim_lock_error_alert_txt_id = TXT_SIM_LOCK_GID_NOT_SUPPORT;
                break;
                
            case MN_SIM_LOCK_ERROR_NETWORK_FAIL:                // Network (de-)personalisation fail
            case MN_SIM_LOCK_ERROR_NETWORK_SUBSET_FAIL:   //  Network subset (de-)personalisation fail
            case MN_SIM_LOCK_ERROR_SP_FAIL:                       // Service Provider (de-)personalisation fail
            case MN_SIM_LOCK_ERROR_CORPORATE_FAIL:          // Corporate (de-)personalisation fail
            case MN_SIM_LOCK_ERROR_USER_FAIL:                  // (U)SIM (de-)personalisation fail
                sim_lock_error_alert_txt_id = TXT_KL_PSW_WRONG_PSW;
                break;
                
            default:
                sim_lock_error_alert_txt_id = TXT_KL_PSW_WRONG_PSW;
                break;
        }
        
        //SCI_MEMSET(operate_sim_lock, 0x00,sizeof(MMI_OPERATE_SIM_LOCK_T));
        operate_sim_lock->is_enable = operate_sim_lock_ptr->is_enable;
        operate_sim_lock->lock_type = operate_sim_lock_ptr->lock_type;
        operate_sim_lock->dual_sys  = operate_sim_lock_ptr->dual_sys;//no use temp
        

        MMIPUB_OpenAlertWinByTextIdEx(VIRTUAL_WIN_ID,
                                                            PNULL,
                                                            sim_lock_error_alert_txt_id,
                                                            TXT_NULL,
                                                            IMAGE_PUBWIN_WARNING,
                                                            PNULL,
                                                            PNULL,
                                                            MMIPUB_SOFTKEY_ONE,
                                                            HandleSIMUnlockingAlertErrorWindow,
                                                            (uint32)operate_sim_lock);

    }
}

/*****************************************************************************/
// 	Description : 在SIM Lock 解锁次数达到限制的情况下弹出"SIM Restricted"窗口
//	Global resource dependence : 
//  Author: dave.ruan
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSIMLocked(
                                    MMI_WIN_ID_T     win_id, 	// Pin boloked窗口的ID
                                    MMI_MESSAGE_ID_E 	msg_id, 	// 窗口的内部消息ID
                                    DPARAM 		        param		// 相应消息的参数
                                    )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    GUI_POINT_T         dis_point = {0};
    GUI_LCD_DEV_INFO	lcd_dev_info = {0};
    
    lcd_dev_info.lcd_id = GUI_MAIN_LCD_ID;
    lcd_dev_info.block_id = GUI_BLOCK_MAIN;
    
    switch (msg_id)
    {
    case MSG_APP_CANCEL://pin被锁输入界面不允许返回
    case MSG_APP_RED:
        MMK_CloseWin(win_id);
        break;

#ifdef FLIP_PHONE_SUPPORT  //close flip ring codes, just open for flip phone.        
    case MSG_APP_FLIP:
        MMK_CloseWin(win_id);
        MMIDEFAULT_HandleFlipKey(FALSE);
        break;
#endif

    case MSG_LOSE_FOCUS:
        break;

    case MSG_FULL_PAINT:
        GUIRES_DisplayImg(&dis_point,
            PNULL,
            PNULL,
            win_id,
            IMAGE_COMMON_BG,
            &lcd_dev_info);
        result = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
        break;
        
    default:
        result = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
        break;
    }
    
    return (result);
}

/*****************************************************************************/
// 	Description : is SIM unlocked?
//	Global resource dependence : 
//  Author: dave.ruan
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPHONE_GetIsSIMUnlocked(void)
{
    return s_sim_unlocked;
}

/*****************************************************************************/
// 	Description : is SIM unlocked?
//	Global resource dependence : 
//  Author: dave.ruan
//	Note:
/*****************************************************************************/
PUBLIC void MMIPHONE_SetIsSIMUnlocked(BOOLEAN is_locked)
{
    s_sim_unlocked = is_locked;
}
/*****************************************************************************/
// 	Description : is phone need restart?
//	Global resource dependence : 
//  Author: dave.ruan
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPHONE_GetPhoneNeedRestart(MN_DUAL_SYS_E dual_sys)
{
    if(MMI_DUAL_SYS_MAX <= dual_sys)
    {
        //SCI_TRACE_LOW:"[MMIPHONE] get MMI_DUAL_SYS_MAX <= dual_sys fail"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_WINTAB_4367_112_2_18_2_45_37_252,(uint8*)"");
        return FALSE;
    }
    return s_phone_need_restart[dual_sys];
}

/*****************************************************************************/
// 	Description : set is phone need restart
//	Global resource dependence : 
//  Author: dave.ruan
//	Note:
/*****************************************************************************/
PUBLIC void MMIPHONE_SetPhoneNeedRestart(MN_DUAL_SYS_E dual_sys, BOOLEAN is_need_restart)
{
    if(MMI_DUAL_SYS_MAX <= dual_sys)
    {
        //SCI_TRACE_LOW:"[MMIPHONE] set dual_sys = %d is_need_restart=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_WINTAB_4381_112_2_18_2_45_37_253,(uint8*)"dd",dual_sys,is_need_restart);
        return;
    }
    s_phone_need_restart[dual_sys] = is_need_restart;
}

/*****************************************************************************/
// 	Description : SIM unlock输入界面打开时，等待界面不允许返回,界面不允许red key,flip返回
//	Global resource dependence : 
//  Author: dave.ruan
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSIMUnlockingWaitingWindow (
                                           MMI_WIN_ID_T      win_id, 	// 窗口的ID
                                           MMI_MESSAGE_ID_E     msg_id, 	// 窗口的内部消息ID
                                           DPARAM 		        param		// 相应消息的参数
                                           )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    
    switch (msg_id)
    {
    case MSG_APP_RED://SIM Unlocking输入界面存在,wait window不允许返回
        break;

#ifdef FLIP_PHONE_SUPPORT  //close flip ring codes, just open for flip phone.        
    case MSG_APP_FLIP:
        MMIDEFAULT_HandleFlipKey(FALSE);
        break;
#endif

    case MSG_OPEN_WINDOW:
        MMK_SetWinDisplayStyleState(win_id, WS_DISABLE_RETURN_WIN, TRUE);
        result = MMIPUB_HandleWaitWinMsg(win_id, msg_id, param);
        break;
    case MSG_FULL_PAINT:
        {
            GUI_POINT_T         dis_point = {0};
            GUI_LCD_DEV_INFO	lcd_dev_info = {0};          
            
            GUIRES_DisplayImg(&dis_point,
                PNULL,
                PNULL,
                win_id,
                IMAGE_COMMON_BG,
                &lcd_dev_info);
            result = MMIPUB_HandleWaitWinMsg(win_id,msg_id,param);
        }
        break;
        
    default:
        result = MMIPUB_HandleWaitWinMsg(win_id, msg_id, param);
        break;
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : the process message function of the Countdown running win
//  Global resource dependence : 
//  Author:        dave.ruan
//  Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleCountdownRunningWinMsg(
                                                                MMI_WIN_ID_T win_id,
                                                                MMI_MESSAGE_ID_E msg_id,
                                                                DPARAM param
                                                            )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_OPERATE_SIM_LOCK_T  *opreate_sim_lock_ptr  = (MMI_OPERATE_SIM_LOCK_T* )MMK_GetWinAddDataPtr(win_id);

    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
            GUIWIN_SetTitleTextId(win_id,TXT_SIM_UNLOCKING_COUNTDOWN,FALSE);
            SetCountdownStartTime(opreate_sim_lock_ptr->lock_type);
            MMK_SetWinDisplayStyleState(win_id, WS_DISABLE_RETURN_WIN, TRUE);
            if (0 != s_countdown_time_id)
            {
                MMK_StopTimer(s_countdown_time_id);//stop timer
                s_countdown_time_id = 0;
            }
            s_countdown_time_id = MMK_CreateTimerCallback(1000,  CountdownHandleTimer, (uint32)opreate_sim_lock_ptr, TRUE);     
            break;

        case MSG_FULL_PAINT:
            //显示背景图片
           // DisplayCountdownBG(win_id);
            //显示时间文本背景
            DisplayCountdownTimeBg(win_id);
            //显示倒计时时间信息
            //GetCurCountdownTime();
            DisplayCurCountdownTime();
            break;
            
        case MSG_GET_FOCUS:
            if(s_is_need_enter_unlock_win)
            {
                s_is_need_enter_unlock_win = FALSE;
                MMK_CreatePubEditWin((uint32*)SIM_LOCK_PASSWORD_INPUTING_WIN_TAB,(ADD_DATA)opreate_sim_lock_ptr);
                MMK_CloseWin(win_id);
            }
            break;
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_MIDSK:
        case MSG_CTL_OK:
        case MSG_APP_OK:
        case MSG_APP_WEB:
            MMK_CreateWin((uint32*)SIM_LOCK_COUNTDOWN_SOS_WIN_TAB, (ADD_DATA)opreate_sim_lock_ptr);
            break;

        case MSG_CLOSE_WINDOW:
            //MMK_FreeWinAddData(win_id);
            break;
          
        default:
            recode = MMI_RESULT_FALSE;
            break;
    }

    return recode;
}

/*****************************************************************************/
//  Description : when pen ok, handle the message
//  Global resource dependence : 
//  Author:   dave.ruan
//  Note:   
/*****************************************************************************/
LOCAL void DisplayCountdownBG(MMI_WIN_ID_T win_id)
{
    GUI_POINT_T         dis_point = {0};
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};

    // display background
    GUIRES_DisplayImg(&dis_point,
        PNULL,
        PNULL,
        win_id,
        IMAGE_COMMON_BG,
        &lcd_dev_info);
}

/*****************************************************************************/
//  Description : display the background pic of time running
//  Global resource dependence : 
//  Author:        dave.ruan
//  Note:   
/*****************************************************************************/
LOCAL void DisplayCountdownTimeBg(MMI_WIN_ID_T win_id)
{
    /*GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID,GUI_BLOCK_MAIN};
    // display background
    LCD_FillRect(&lcd_dev_info, MMITHEME_GetClientRect(), MMI_WINDOW_BACKGROUND_COLOR); */
    
//     GUI_POINT_T         dis_point = {0};
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    GUI_RECT_T rect=MMITHEME_GetClientRect();

    GUIRES_DisplayImg(PNULL,
        &rect,
        &rect,
        win_id,
        IMAGE_COMMON_BG,
        &lcd_dev_info);
}

/*****************************************************************************/
//  Description : 设置输入的时间为倒记时开始的时间
//  Global resource dependence : 
//  Author: dave.ruan
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SetCountdownStartTime(uint32 unlock_type)
{
    BOOLEAN         result = TRUE;
    uint32  unlock_time = 0;
    uint32  unlock_trials = 0;
    
    MMIAPIPHONE_GetSIMLockRemainTrialsTimer(unlock_type,&unlock_trials,&unlock_time);

    if(0 == unlock_time)//in seconds
    {
        return FALSE;
    }
    SCI_MEMSET(&s_countdown_set_time,0x00,sizeof(SCI_TIME_T));
    
    if(0 == s_countdown_set_time.hour && 0 ==s_countdown_set_time.min && 0 == s_countdown_set_time.sec)
    {
        s_countdown_set_time.hour = (uint8)(unlock_time /3600);//unlock time in "hour"
        s_countdown_set_time.min = (uint8)(unlock_time /60 - s_countdown_set_time.hour*60);	//unlcok time in "minute"
        s_countdown_set_time.sec = (uint8)(unlock_time - s_countdown_set_time.min *60 - s_countdown_set_time.hour * 3600);
    }
    //check time if is valid
    if(60 > s_countdown_set_time.min && 60 > s_countdown_set_time.sec)
    {
        s_countdown_current_time.sec = s_countdown_set_time.sec;
        s_countdown_current_time.min = s_countdown_set_time.min;
        s_countdown_current_time.hour =  s_countdown_set_time.hour;
    }
    else
    {
        result = FALSE;
    }
    //SCI_TRACE_LOW:"[MMIPHONE] SetCountdownStartTime result=%d,hour%2d,min%2d,sec%2d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_WINTAB_4586_112_2_18_2_45_37_254,(uint8*)"dddd",result,s_countdown_current_time.hour,s_countdown_current_time.min,s_countdown_current_time.sec);
    
    return (result);
}

/*****************************************************************************/
//  Description : get curr time
//  Global resource dependence : none
//  Author: dave.ruan
//  Note:
/*****************************************************************************/
LOCAL SIMLOCK_COUNTDOWN_TIME_T *GetCurCountdownTime( void )
{
    static  SIMLOCK_COUNTDOWN_TIME_T time ={0};
    SCI_TIME_T curtime = s_countdown_current_time;
    uint8    hour_value =0; 

    /*for hour max show 99*/
    if(99 < curtime.hour)
    {
        hour_value = 99;
    }
    else
    {
        hour_value = curtime.hour;
    }
    sprintf((char*)time, "%02d:%02d:%02d", hour_value, curtime.min, curtime.sec);
    
    return( time ); /*lint !e64*/
}

/*****************************************************************************/
//  Description : mmi phone get curr countdown time to restore in NV
//  Global resource dependence : none
//  Author: dave.ruan
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIPHONE_GetCurCountdownTime( void )
{
    uint32  time_value = 0;

    time_value = s_countdown_current_time.hour *3600 + s_countdown_current_time.min *60 + s_countdown_current_time.sec;
    return time_value;
}

/*****************************************************************************/
//  Description : Display Cur Countdown Time
//  Global resource dependence : none
//  Author: dave.ruan
//  Note:
/*****************************************************************************/
LOCAL void DisplayCurCountdownTime(void)
{
    wchar            time_temp[COUNTDOWN_TIME_STRING_LENGTH+1]  = {0};
    SIMLOCK_COUNTDOWN_TIME_T *time = {0};
    MMI_STRING_T        cur_time     = {0};
    MMISET_LANGUAGE_TYPE_E  lang    = MMISET_MAX_LANGUAGE;//language    
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    GUI_RECT_T          time_rect = {0};
    GUISTR_STYLE_T      text_style      = {0}; /*lint !e64*/
    GUISTR_STATE_T      state =         GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE; 
    
    time = GetCurCountdownTime();
    
    MMIAPISET_GetLanguageType(&lang);
    time_rect = MMITHEME_GetClientRect();
    MMI_STRNTOWSTR(time_temp,COUNTDOWN_TIME_STRING_LENGTH,(uint8*)time,COUNTDOWN_TIME_STRING_LENGTH,COUNTDOWN_TIME_STRING_LENGTH);
    cur_time.wstr_ptr      = time_temp;
    cur_time.wstr_len = COUNTDOWN_TIME_STRING_LENGTH;
    
    text_style.align = ALIGN_HVMIDDLE;
    text_style.font = COUNTDOWN_TEXT_FONT;
    text_style.font_color = MMI_BLACK_COLOR;
    
    // display
    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_RECT_T      *)&time_rect,       //the fixed display area
        (const GUI_RECT_T      *)&time_rect,       //用户要剪切的实际区域
        (const MMI_STRING_T    *)&cur_time,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        ); 
}

/*****************************************************************************/
//  Description : Get SIM Lock Trial Counter
//  Global resource dependence : none
//  Author: dave.ruan
//  Note:
/*****************************************************************************/
LOCAL uint32 GetSIMLockTrialCounter(uint32 simlock_type)
{
    MN_SIM_LOCK_STATUS_T *sim_unlock_data = MMIAPIPHONE_GetSIMUnlockConfigDataPtr();
    uint32 trial_counters = 0;
    
    if(simlock_type&MMI_SIM_NETWORK_LOCK)
    {
        trial_counters = sim_unlock_data->nck_trials;
    }
    else if(simlock_type&MMI_SIM_NETWORK_SUBSET_LOCK)
    {
        trial_counters = sim_unlock_data->nsck_trials;
    }
    else if(simlock_type&MMI_SIM_SP_LOCK)
    {
        trial_counters = sim_unlock_data->spck_trials;
    }
    else if(simlock_type&MMI_SIM_CORPORATE_LOCK)
    {
        trial_counters = sim_unlock_data->cck_trials;
    }
    else if(simlock_type&MMI_SIM_USER_LOCK)
    {
        trial_counters = sim_unlock_data->pck_trials;
    }
    else
    {
        trial_counters = sim_unlock_data->nck_trials;
    }
    
    return trial_counters;
}

/*****************************************************************************/
//  Description : handle counted time timer over msg
//  Global resource dependence : none
//  Author: dave.ruan
//  Note:
/*****************************************************************************/
LOCAL void CountdownHandleTimer(uint8 timer_id, uint32 param)
{
    //SCI_TRACE_LOW:"CountdownHandleTimer timer_id = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_WINTAB_4709_112_2_18_2_45_38_255,(uint8*)"d",timer_id);
    if (s_countdown_time_id == timer_id)
    {
        //时间减1处理
        if (s_countdown_current_time.sec <60)
        {
            s_countdown_current_time.sec -- ;
            if (((s_countdown_current_time.sec == 0)||(s_countdown_current_time.sec == 0xff) ) && (s_countdown_current_time.min == 0) && (s_countdown_current_time.hour == 0))//时间到
            {   
                s_countdown_current_time.sec=0;
                if((PHONE_SIM_LOCK_COUNTDOWN_WIN_ID == MMK_GetFocusWinId()))
                {
                    uint32 time_out_value =0;
                    MMI_OPERATE_SIM_LOCK_T  *opreate_sim_lock_ptr =(MMI_OPERATE_SIM_LOCK_T *)param;
                    
                    //倒计时到,清除存在NV里面的倒计时为0，防止开机再次倒计时
                    MNPHONE_UpdateSIMUnlockRetryData(opreate_sim_lock_ptr->lock_type, 
                                                                        GetSIMLockTrialCounter(opreate_sim_lock_ptr->lock_type), 
                                                                        time_out_value);
                    MMIAPIPHONE_UpdateSIMUnlockData();//立即更新到MMI
                    
                    s_is_need_enter_unlock_win = FALSE;
                    SendMsgtoDispCountdown();
                    MMK_CloseWin(PHONE_SIM_LOCK_COUNTDOWN_WIN_ID);
                    MMK_CreatePubEditWin((uint32*)SIM_LOCK_PASSWORD_INPUTING_WIN_TAB,(ADD_DATA)param);
                }
                else
                {
                    s_is_need_enter_unlock_win = TRUE;//当前在其他界面，需要延迟create password win
                }

                if (0 != s_countdown_time_id)
                {
                    MMK_StopTimer(s_countdown_time_id);//倒计时到，停止计时器
                    s_countdown_time_id = 0;
                }
                return;                    
            }
            else if ((s_countdown_current_time.sec == 0xff) && (s_countdown_current_time.min<=COUNTDOWN_MAX_MINUTE))
            {
                s_countdown_current_time.sec = 59;
                s_countdown_current_time.min --;
                if ((s_countdown_current_time.min == 0xff) && (s_countdown_current_time.hour>0) && (s_countdown_current_time.hour <= COUNTDOWN_MAX_HOUR))
                {
                    s_countdown_current_time.min = 59;
                    s_countdown_current_time.hour --;
                }
            }
            else
            {
                /*do nothing*/
            }
            //发消息到倒计时界面显示
            SendMsgtoDispCountdown();
        }
    }
}

/*****************************************************************************/
//  Description : Send Msg to Disp Countdown
//  Global resource dependence : 
//  Author:       dave.ruan
//  RETRUN:  
//  Note:    
/*****************************************************************************/
LOCAL BOOLEAN SendMsgtoDispCountdown(void)
{
    if (PHONE_SIM_LOCK_COUNTDOWN_WIN_ID == MMK_GetFocusWinId())
    {
        if (MMK_IsOpenWin(PHONE_SIM_LOCK_COUNTDOWN_WIN_ID))
        {
            return MMK_SendMsg(PHONE_SIM_LOCK_COUNTDOWN_WIN_ID,MSG_FULL_PAINT,PNULL);
        }
    }
    return TRUE;
}

/*****************************************************************************/
// 	Description : handle SIM Lock set menu Flow
//	Global resource dependence : 
//  Author:dave.ruan
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_HandleSIMLockSettingMenuFlow(
                             uint32       lock_type,   //SIM Lock Type
                             MN_DUAL_SYS_E    dual_sys,        //dual sim card reserved
                             BOOLEAN    is_enable   //enable or disable the lock Type
                             )
{
    MN_SIM_LOCK_STATUS_T *sim_unlock_data = MMIAPIPHONE_GetSIMUnlockConfigDataPtr();
    MMI_OPERATE_SIM_LOCK_T  *opreate_sim_lock_ptr = SCI_ALLOCAZ(sizeof(MMI_OPERATE_SIM_LOCK_T));
    uint32 trials_counter = 0;
    uint32 time_out_value = 0;
    
    if(PNULL == opreate_sim_lock_ptr)
    {
        //SCI_TRACE_LOW:"[MMIPHONE] MMIAPIPHONE_HandleSIMLockSettingMenuFlow opreate_sim_lock_ptr alloc fail!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_WINTAB_4801_112_2_18_2_45_38_256,(uint8*)"");
        return;
    }
    
    MMIAPIPHONE_GetSIMLockRemainTrialsTimer(lock_type,&trials_counter,&time_out_value);
    
    if(sim_unlock_data->max_num_trials <= trials_counter)
    {
        /*如果在输入设置SIM Lock界面，提示SIM Locked, 返回*/
        MMIPUB_OpenAlertWinByTextId(PNULL,TXT_SIM_LOCKED,TXT_NULL,
                                                        IMAGE_PUBWIN_WARNING,PNULL,
                                                        PNULL,MMIPUB_SOFTKEY_NONE,PNULL);
        SCI_FREE(opreate_sim_lock_ptr);                
        return;                                                        
    }
    //SCI_MEMSET(opreate_sim_lock_ptr,0x00,sizeof(MMI_OPERATE_SIM_LOCK_T));
    
    opreate_sim_lock_ptr->is_enable= is_enable;
    opreate_sim_lock_ptr->lock_type = lock_type;
    opreate_sim_lock_ptr->dual_sys= dual_sys;
    
    MMK_CloseWin(MMI_SIM_LOCK_PASSWORD_INPUTING_WIN_ID);

    // 打开输入SIM Lock PSWD的界面
    MMK_CreatePubEditWin((uint32*)MMI_SIM_LOCK_INPUTING_WIN_TAB,(ADD_DATA)opreate_sim_lock_ptr);

}

/*****************************************************************************/
// 	Description : 处理所有SIM Lock INPUT的消息，除去开关机
//	Global resource dependence : 
//  Author: dave.ruan
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSIMLockInputExceptPhone(
                                             MMI_WIN_ID_T    win_id, 	// PIN2 INPUTING窗口的ID
                                             MMI_MESSAGE_ID_E 	msg_id, 	// 窗口的内部消息ID
                                             DPARAM 		    param		// 相应消息的参数
                                             )
{
    MMI_OPERATE_SIM_LOCK_T   *opreate_sim_lock_ptr = (MMI_OPERATE_SIM_LOCK_T *)MMK_GetWinAddDataPtr(win_id);
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    GUI_BOTH_RECT_T edit_both_rect = {0};
    MMI_STRING_T sim_unlocking_value = {0};   
    MMI_STRING_T sim_unlocking_remain_str = {0};
    MMI_TEXT_ID_T   title_text_id = TXT_NULL;

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
#ifdef MAINLCD_SIZE_128X64 
        {
            GUI_BOTH_RECT_T both_rect = GetPasswordLabelBothRect();
            GUI_RECT_T title_rect = GetPasswordTitleRect();

            GUIWIN_SetTitleRect(win_id, title_rect);	
            GUIAPICTRL_SetBothRect(MMIPHONE_SIM_LOCK_INPUT_REMAIN_LABEL_CTRL_ID, &both_rect);
        }
#else			
        {
            GUI_BOTH_RECT_T both_rect = MMITHEME_GetWinClientBothRect(win_id);

            both_rect.h_rect.top += MMIPHONE_PIN_PUK_REMAIN_TOP_MARGIN;
            both_rect.v_rect.top += MMIPHONE_PIN_PUK_REMAIN_TOP_MARGIN;
            both_rect.h_rect.bottom = both_rect.h_rect.top + MMIPHONE_PIN_PUK_REMAIN_HEIGHT;
            both_rect.v_rect.bottom = both_rect.v_rect.top + MMIPHONE_PIN_PUK_REMAIN_HEIGHT;

            GUIAPICTRL_SetBothRect(MMIPHONE_SIM_LOCK_INPUT_REMAIN_LABEL_CTRL_ID, &both_rect);
        }
#endif 
        IGUICTRL_SetState(MMK_GetCtrlPtr(MMIPHONE_SIM_LOCK_INPUT_REMAIN_LABEL_CTRL_ID), GUICTRL_STATE_DISABLE_ACTIVE, TRUE);

        SCI_MEMSET(&sim_unlocking_remain_str, 0x00, sizeof(MMI_STRING_T));

        GetSIMLockRemainStr(opreate_sim_lock_ptr->dual_sys, &sim_unlocking_remain_str, opreate_sim_lock_ptr->lock_type);

        if (PNULL != sim_unlocking_remain_str.wstr_ptr)
        {
            GUILABEL_SetText(MMIPHONE_SIM_LOCK_INPUT_REMAIN_LABEL_CTRL_ID,&sim_unlocking_remain_str,FALSE);

            SCI_FREE(sim_unlocking_remain_str.wstr_ptr);
        }

        edit_both_rect = GetSIMUnlockEditRect(win_id, TRUE);
        GUIAPICTRL_SetBothRect(MMI_SIM_LOCK_PASSWORD_EDITBOX_CTRL_ID, &edit_both_rect);
        
        GUIEDIT_SetPasswordStyle(MMI_SIM_LOCK_PASSWORD_EDITBOX_CTRL_ID,GUIEDIT_PASSWORD_STYLE_DIGITAL_PIN);

        /*set title*/
        GetSIMLockInputPasswordTitle(opreate_sim_lock_ptr->lock_type,&title_text_id);
        
        GUIWIN_SetTitleTextId(win_id,title_text_id,FALSE);

        //set left softkey
        GUIEDIT_SetSoftkey(MMI_SIM_LOCK_PASSWORD_EDITBOX_CTRL_ID, 0,MMIPHONE_SIM_UNLOCK_PCK_MIN_KEY_LEN,TXT_NULL,TXT_COMMON_OK, SIMUnlockInputWinSoftkey);

        MMK_SetAtvCtrl(win_id,MMI_SIM_LOCK_PASSWORD_EDITBOX_CTRL_ID);
        break;
        
    case MSG_FULL_PAINT:
        SCI_MEMSET(&sim_unlocking_remain_str, 0x00, sizeof(MMI_STRING_T));
        GetSIMLockRemainStr(opreate_sim_lock_ptr->dual_sys, &sim_unlocking_remain_str, opreate_sim_lock_ptr->lock_type);

        if (PNULL != sim_unlocking_remain_str.wstr_ptr)
        {
            GUILABEL_SetText(MMIPHONE_SIM_LOCK_INPUT_REMAIN_LABEL_CTRL_ID,&sim_unlocking_remain_str,FALSE);

            SCI_FREE(sim_unlocking_remain_str.wstr_ptr);
        }
#ifndef MMI_PDA_SUPPORT
        SetPasswordBackground(win_id,MMI_SIM_LOCK_PASSWORD_EDITBOX_CTRL_ID,FALSE);
#endif
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        // get the value of SIM Lock PSWd
        GUIEDIT_GetString(MMI_SIM_LOCK_PASSWORD_EDITBOX_CTRL_ID,&sim_unlocking_value);
        
        if (SIM_UNLOCK_LEN_IS_IN_RANGE(sim_unlocking_value.wstr_len))
        {
            HandleSIMUnlockingInputingString(opreate_sim_lock_ptr, &sim_unlocking_value);
        }
        else
        {
            uint32 trials_counter = 0;
            uint32 time_out_value = 0;
            
            MMIAPIPHONE_GetSIMLockRemainTrialsTimer(opreate_sim_lock_ptr->lock_type,&trials_counter,&time_out_value);
            
            trials_counter ++;

            MMIPUB_OpenAlertWinByTextId(PNULL,TXT_KL_PSW_WRONG_PSW,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,HandlePukAlertWindow);
            MNPHONE_UpdateSIMUnlockRetryData(opreate_sim_lock_ptr->lock_type, trials_counter, time_out_value);
            MMIAPIPHONE_UpdateSIMUnlockData();
            //清空password
            GUIEDIT_ClearAllStr(MMI_SIM_LOCK_PASSWORD_EDITBOX_CTRL_ID);
        }
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    case MSG_CLOSE_WINDOW:
        MMK_FreeWinAddData(win_id);
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return (result);
}

/*****************************************************************************/
//     Description : 处理倒计时界面SOS拨号的窗口
//    Global resource dependence : 
//  Author: dave.ruan
//    Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleCountDownSOSWindow(
                                                     MMI_WIN_ID_T        win_id, //IN: 窗口的ID
                                                     MMI_MESSAGE_ID_E     msg_id, //IN: 窗口的内部消息ID
                                                     DPARAM                 param    //IN: 相应消息的参数
                                                     )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMI_SIM_LOCK_SOS_EDITBOX_CTRL_ID;
    MMI_STRING_T edit_string = {0};
    MMI_OPERATE_SIM_LOCK_T   *operate_sim_lock_ptr = (MMI_OPERATE_SIM_LOCK_T* )MMK_GetWinAddDataPtr(win_id);
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            GUI_BOTH_RECT_T  edit_rect = MMITHEME_GetWinClientBothRect(MMK_ConvertIdToHandle(win_id));   
            GUIAPICTRL_SetBothRect(MMK_ConvertIdToHandle(ctrl_id), &edit_rect);
        }
        GUIEDIT_SetFont(MMI_SIM_LOCK_SOS_EDITBOX_CTRL_ID,MMI_DEFAULT_BIG_FONT);
        GUIEDIT_SetStyle(ctrl_id, GUIEDIT_STYLE_SINGLE);
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        {
            BOOLEAN is_input_emergency_call = FALSE;
            uint16 emergency_call_len = 0;
            
            // get the value of SOS
            GUIEDIT_GetString(ctrl_id, &edit_string);

            is_input_emergency_call = MMIAPICC_IsInputEmergencyCall(&edit_string, &emergency_call_len, operate_sim_lock_ptr->dual_sys);

            if ((is_input_emergency_call) &&
                (edit_string.wstr_len == emergency_call_len))
            {
                MakeEmergencyCall(&edit_string);
                MMK_CloseWin(win_id);
            }
            else
            {
                MMI_WIN_PRIORITY_E win_priority_ptr = WIN_THREE_LEVEL;//the same as PHONE_SIM_LOCK_SOS_WIN_ID，防止提示无法弹出
                //提示输入错误
                MMIPUB_OpenAlertWinByTextId(PNULL,TXT_INPUT_ERROR,TXT_NULL,IMAGE_PUBWIN_FAIL,PNULL,&win_priority_ptr,MMIPUB_SOFTKEY_ONE,PNULL);  
                //清空password
                GUIEDIT_ClearAllStr(ctrl_id);
            }
        }
        break;
        
    case MSG_CTL_CANCEL:        
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return (recode);
}

/*****************************************************************************/
// 	Description :Handle SIM Locked Shut off WinMsg
//	Global resource dependence :                                
//      Author: dave.ruan
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSIMLockShutoffWinMsg(
                                           MMI_WIN_ID_T win_id, 
                                           MMI_MESSAGE_ID_E msg_id, 
                                           DPARAM param
                                           )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    GUI_RECT_T rect = MMITHEME_GetClientRect();
    GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    
    lcd_dev_info.lcd_id = GUI_MAIN_LCD_ID;
    lcd_dev_info.block_id = GUI_BLOCK_MAIN;
    switch(msg_id)
    {
    case MSG_FULL_PAINT:
        LCD_FillRect(&lcd_dev_info, rect, MMI_WINDOW_BACKGROUND_COLOR);
        GUIWIN_SetSoftkeyTextId(win_id,  (MMI_TEXT_ID_T)TXT_COMMON_OK, (MMI_TEXT_ID_T)TXT_NULL, (MMI_TEXT_ID_T)STXT_CANCEL, FALSE);
        MMIPUB_OpenAlertWinByTextId(PNULL,TXT_SIM_LOCKED,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,HandleSIMLockedAlertWinMsg);
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    return recode;
}

/*****************************************************************************/
// 	Description : Handle SIM Lock Alert Win Msg ,when close window,power off ME
//	Global resource dependence : 
//  Author: dave.ruan
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSIMLockedAlertWinMsg(
                                                                                              MMI_WIN_ID_T win_id,
                                                                                              MMI_MESSAGE_ID_E 	msg_id,
                                                                                              DPARAM param
                                                                                              )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    
    switch (msg_id)
    {
    case MSG_CLOSE_WINDOW:
        MMIAPIPHONE_PowerOff(); //初始化失败，直接关机
        result = MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);
        break;
        
    case MSG_APP_CANCEL:
    case MSG_APP_RED:
        MMK_CloseWin(win_id);
        break;
        
    default:
        result = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
        break;
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : MMIPHONE SIM Locked Open Shut Off Win
//  Global resource dependence :                                
//      Author: dave.ruan
//  Note:
/*****************************************************************************/
PUBLIC void MMIPHONE_SIMLockedOpenShutOffWin (void)
{
    MMK_CreateWin((uint32*)MMI_SIM_LOCKED_SHUTOFF_WIN_TAB, PNULL);
}
#endif


#include "mmiidle_statusbar.h"

#ifdef MMIPUBWIN_PDASTYLE_SUPPORT
extern PUBLIC void MMIPUBLayout( MMIPUB_INFO_T *win_info_ptr);
extern PUBLIC MMI_RESULT_E MMIPUBHandleButtonMsg(
                                    MMIPUB_INFO_T*    win_info_ptr,
                                    MMI_MESSAGE_ID_E  msg_id, 
                                    DPARAM            param
                                    );
extern PUBLIC void MMIPUBCreatSoftkeyCtrl(  MMI_HANDLE_T    win_handle);
extern PUBLIC void MMIPUBSetButton(MMIPUB_INFO_T *win_info_ptr, BOOLEAN is_fresh);
extern PUBLIC void MMIPUBDisplayWin(MMIPUB_INFO_T    *win_info_ptr);
extern PUBLIC void MMIPUB_HandleLcdSwitch(MMIPUB_INFO_T* win_info_ptr);
extern PUBLIC void MMIPUB_ReleaseLayer(MMIPUB_INFO_T* win_info_ptr);
extern PUBLIC void MMIPUBClearClientRectBg(MMIPUB_INFO_T* win_info_ptr,GUI_RECT_T* rect_ptr);
#endif
extern PUBLIC void MMIPUB_DestroyWinData( MMIPUB_INFO_T* win_info_ptr);

/*****************************************************************************/
//  Description : handle message of sim list window 
//  Global resource dependence : none
//  Author: Sam.Hua
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSIMSelectCommonFuncWinMsg(
                                    MMI_WIN_ID_T        win_id, 
                                    MMI_MESSAGE_ID_E    msg_id, 
                                    DPARAM              param
);

 

/*****************************************************************************/
//  Description : set list rect
//  Global resource dependence : 
//  Author: Sam.hua
//  Note:
/*****************************************************************************/
LOCAL void PUBLIST_SetListRect(
                       MMIPUB_INFO_T *win_info_ptr
                       )
{
    GUILIST_SetRect( win_info_ptr->ctrl_handle, &win_info_ptr->client_rect );
}


#if defined(MMI_WIFI_SUPPORT)
/*****************************************************************************/
//  Description :append wifi item to list  
//  Global resource dependence : none
//  Author: Sam.Hua
//  Note: 
/*****************************************************************************/
LOCAL void  SelectSIMList_AppendWifiItem(MMI_CTRL_ID_T  ctrl_id)
{
    GUILIST_ITEM_T      item_t = {0};
    GUILIST_ITEM_DATA_T item_data = {0};

    SCI_MEMSET(&item_t, 0x00, sizeof(item_t));
    SCI_MEMSET(&item_data, 0x00, sizeof(item_data));

    item_t.item_style = GUIITEM_STYLE_TWO_LINE_ANIM_TEXT_AND_TEXT_2;
    item_t.item_data_ptr = &item_data;
    item_t.user_data = MMI_DUAL_SYS_MAX;//WIFI

    item_data.item_content[0].item_data_type = GUIITEM_DATA_IMAGE_ID;
    item_data.item_content[0].item_data.image_id = MMIAPIWIFI_GetWifiStatusIcon();

    item_data.item_content[1].item_data_type    = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[1].item_data.text_id  = TXT_WIFI;
    
    item_data.item_content[2].item_data_type    = GUIITEM_DATA_TEXT_ID;
    if (MMIAPIWIFI_GetStatus() >0)
    {
        item_data.item_content[2].item_data.text_id  = TXT_COMMON_ON;
    }
    else
    {
        item_data.item_content[2].item_data.text_id  = TXT_COMMON_OFF;
    }

    GUILIST_AppendItem(ctrl_id, &item_t);

}//End of SelectSIMList_AppendWifiItem
#endif

#if defined(BT_DIALER_SUPPORT)
/*****************************************************************************/
//  Description :append wifi item to list  
//  Global resource dependence : none
//  Author: Sam.Hua
//  Note: 
/*****************************************************************************/
PUBLIC void  MMIAPISET_AppendBtItem(MMI_CTRL_ID_T  ctrl_id)
{
    GUILIST_ITEM_T      item_t = {0};
    GUILIST_ITEM_DATA_T item_data = {0};
    wchar               file_name[BT_DEVICE_NAME_SIZE+1] = {0};
    uint16              file_name_len = 0;
	
    SCI_MEMSET(&item_t, 0x00, sizeof(item_t));
    SCI_MEMSET(&item_data, 0x00, sizeof(item_data));
	
	item_t.item_style = GUIITEM_STYLE_TWO_LINE_ANIM_TEXT_AND_TEXT_2;
    item_t.item_data_ptr = &item_data;
    item_t.user_data = MMI_DUAL_SYS_MAX;

    item_data.item_content[0].item_data_type = GUIITEM_DATA_IMAGE_ID;
    item_data.item_content[0].item_data.image_id = IMAGE_BT_HANFREE_PAIRE;

    item_data.item_content[1].item_data_type    = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[1].item_data.text_id  = TXT_BT;
    
    if (MMIAPIBT_GetConnectedDeviceName(file_name,&file_name_len))
    {
        item_data.item_content[2].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
        item_data.item_content[2].item_data.text_buffer.wstr_len = file_name_len;
        item_data.item_content[2].item_data.text_buffer.wstr_ptr = file_name;
    }
    else
    {
    item_data.item_content[2].item_data_type    = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[2].item_data.text_id  = PNULL;
    }
	
    GUILIST_AppendItem(ctrl_id, &item_t);
	
}//End of SelectSIMList_AppendBtItem
#endif

/*****************************************************************************/
//  Description : handle message of sim list pub window 
//  Global resource dependence : none
//  Author: Sam.Hua
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSIMSelectCommonFuncWinMsg(
                                    MMI_WIN_ID_T        win_id, 
                                    MMI_MESSAGE_ID_E    msg_id, 
                                    DPARAM              param
                                    )
{
    MMI_RESULT_E  retcode = MMI_RESULT_FALSE;
    MMI_CTRL_ID_T   ctrl_id = 0;
    MMIPUB_INFO_T   *win_info_ptr = MMIPUB_GetWinAddDataPtr(win_id);
    MMIPHONE_SELECT_SIM_DATA_T* selectSim_data_ptr;
    MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_1;
    uint16 cur_index = 0;
    uint16 top_index = 0;

    //coverity 12954
    if ( PNULL == win_info_ptr )
    {
        //Error //but how to back?? 
        return MMI_RESULT_FALSE;
    }
	
	if ( PNULL==win_info_ptr->extra_data_ptr )
    {
        //Error //but how to back?? 
        return MMI_RESULT_FALSE;
    }
	ctrl_id = win_info_ptr->ctrl_handle;
#ifdef MMIPUBWIN_PDASTYLE_SUPPORT
    //转义消息
    if ( MMI_RESULT_TRUE == ( retcode = MMIPUBHandleButtonMsg( win_info_ptr, msg_id, param ) ) )
    {
        return retcode;
    }
#endif
    if ( !MMK_IsOpenWin( win_id ) )
    {
        return retcode;
    }

    selectSim_data_ptr =(MMIPHONE_SELECT_SIM_DATA_T*)win_info_ptr->extra_data_ptr;

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMK_SetAtvCtrl(win_id, ctrl_id);
        MMIAPIPHONE_SetNeedUpdateNetworkStatusWin(win_id);//
        break;

    case MSG_FULL_PAINT:
        cur_index = GUILIST_GetCurItemIndex(ctrl_id);
        top_index = GUILIST_GetTopItemIndex(ctrl_id);

        GUILIST_SetMaxItem(ctrl_id, MMI_DUAL_SYS_MAX + 1, FALSE );//max item 
        if (MMIPHONE_SELECTION_SIM_ONLY & selectSim_data_ptr->select_type)/*lint !e655*/
        {
            GUIWIN_SetTitleTextId(win_id, TXT_SIM_SEL , FALSE);
            MMIAPISET_AppendMultiSimName(ctrl_id, TXT_COMMON_OK, MMI_DUAL_SYS_MAX, (MMISET_APPEND_SIM_TYPE_E)selectSim_data_ptr->append_type);
         }
#if defined(MMI_WIFI_SUPPORT)
        if (MMIPHONE_SELECTION_WLAN_ONLY & selectSim_data_ptr->select_type)/*lint !e655*/
        {
            GUIWIN_SetTitleTextId(win_id, TXT_NETWORK_SELECT , FALSE);
            //MMIAPISET_AppendMultiSimName(ctrl_id, STXT_OK, MMI_DUAL_SYS_MAX, (MMISET_APPEND_SIM_TYPE_E)selectSim_data_ptr->append_type);
            SelectSIMList_AppendWifiItem(ctrl_id);
        }
#endif
        
#if defined(BT_DIALER_SUPPORT)
        
		if (!(MMIPHONE_SELECTION_SIM_NO_BT & selectSim_data_ptr->select_type))
        {
            MMIAPISET_AppendBtItem(ctrl_id);
		
			if(MMIAPIBT_IsConnectDevice())
			{
				GUILIST_SetItemGray(ctrl_id, GUILIST_GetTotalItemNum(ctrl_id) - 1, FALSE);
				GUILIST_SetItemInvalid(ctrl_id, GUILIST_GetTotalItemNum(ctrl_id) - 1, FALSE);
			}
			else
			{
				GUILIST_SetItemGray(ctrl_id, GUILIST_GetTotalItemNum(ctrl_id) - 1, TRUE);
				GUILIST_SetItemInvalid(ctrl_id, GUILIST_GetTotalItemNum(ctrl_id) - 1, TRUE);
			}
		}
#endif		

#ifdef MMIPUBWIN_PDASTYLE_SUPPORT
        if ( !win_info_ptr->is_layout )
        {
            win_info_ptr->line_num = GUILIST_GetTotalItemNum( win_info_ptr->ctrl_handle );

            MMIPUB_ReleaseLayer( win_info_ptr );

            MMIPUBLayout( win_info_ptr );
            PUBLIST_SetListRect( win_info_ptr );
            MMIPUBSetButton( win_info_ptr, FALSE );
        }

        MMIPUBDisplayWin( win_info_ptr );
#endif

        GUILIST_SetCurItemIndex(ctrl_id, cur_index);
        GUILIST_SetTopItemIndex(ctrl_id, top_index);
        break;

    case MSG_GET_FOCUS:
        MMIAPIPHONE_SetNeedUpdateNetworkStatusWin(win_id);
        break;

#ifdef MMIPUBWIN_PDASTYLE_SUPPORT
    case MSG_LCD_SWITCH:
        MMIPUB_HandleLcdSwitch( win_info_ptr );
        break;
    case MSG_NOTIFY_UPDATE:
        MMIPUBClearClientRectBg( win_info_ptr, param );
        break;
#endif

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        {
            uint16 cur_selection = 0;
            uint32 item_data = 0;

            cur_selection = GUILIST_GetCurItemIndex(ctrl_id);
            if(GUILIST_IsCurrItemInvalid(ctrl_id))
            {
                break;
            }

#if defined(BT_DIALER_SUPPORT) && defined(BT_MAP_SUPPORT)
			
            if(MMIAPISMS_GetCurOperIsPush())
            {
                if((cur_selection == GUILIST_GetTotalItemNum(ctrl_id) - 1) 
                    && !(MMK_IsOpenWin(MMISET_SET_SECURITY_WIN_ID)))//BUG683374
                {
                    if(MMIAPIBT_IsConnectDevice())
                    {
                        if(MMK_IsOpenWin(MMISMS_EDIT_SMS_WIN_ID))
                        {
                            MMISMS_BtSendItem(FALSE);
                        }
                        else
                        {
                            if(!MMK_IsOpenWin(MMISET_COM_SELECT_SIM_WIN_ID))
                            {
                                MMISMS_BtResendCallback(MMI_DUAL_SYS_MAX);
                            }
                        }
                        MMK_CloseWin(win_id);
                        break;
                    }
                }
            }
#endif
			
            GUILIST_GetItemData(ctrl_id, cur_selection, &item_data);        
#if defined(MMI_WIFI_SUPPORT)
            if( item_data <MMI_DUAL_SYS_MAX+1)
#else
            if( item_data <MMI_DUAL_SYS_MAX)
#endif
            {       
               dual_sys = item_data ;  
            }
            else        //Error,call user_func and close win,
            {   
                //SCI_TRACE_LOW:"SelectSIM COM WIN,dual_sys[%d]>=MMI_DUAL_SYS_MAX/MMI_DUAL_SYS_MAX+1 in wifi "
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_WINTAB_5307_112_2_18_2_45_39_257,(uint8*)"d",item_data);
                retcode = selectSim_data_ptr->callback_func(MMI_DUAL_SYS_MAX, FALSE,selectSim_data_ptr->extra_data);
                MMK_CloseWin(win_id);
                break;
            }

            //call use func to do it again
            retcode = selectSim_data_ptr->callback_func( dual_sys, TRUE,selectSim_data_ptr->extra_data);
            if (retcode)    //if callback return ture, means should close the window
            {
                MMK_CloseWin(win_id);
            }
        }
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        retcode = selectSim_data_ptr->callback_func(MMI_DUAL_SYS_MAX, FALSE,PNULL);
        MMK_CloseWin(win_id);
        break;

    case MSG_CLOSE_WINDOW:
        //Free    extra_data_ptr
/*            if (PNULL != selectSim_data_ptr->extra_data)    //help caller to free it ??
        {
            SCI_FREE(selectSim_data_ptr->extra_data);
        }
*/            
        if (PNULL !=win_info_ptr->extra_data_ptr)
        {
            SCI_FREE(win_info_ptr->extra_data_ptr);
        }
        MMIPUB_DestroyWinData( win_info_ptr );
        break;

    default:
        retcode = MMI_RESULT_FALSE;
        break;
    }

    return retcode;
}//End of HandleSIMSelectCommonWinMsg

/*****************************************************************************/
//  Description : creat and open Select SIM window by parameters
//  Global resource dependence : 
//  Author: sam.hua
//  Note:
/*****************************************************************************/
PUBLIC  MMI_RESULT_E MMIAPIPHONE_OpenSelectSimWin(MMIPHONE_SELECT_SIM_DATA_T* selectsim_input_ptr)
{
    BOOLEAN             result = FALSE;
    MMI_WIN_ID_T        win_id = MMIPUB_LIST_WIN_ID;
    MMIPUB_INFO_T       *win_info_ptr = PNULL;
    MMIAPIPHONE_SELECTSIM_FUNC     handle_function = PNULL;//(PROCESSMSG_FUNC)HandleDefaultWinMsg;
    MMI_WINDOW_CREATE_T win_create = {0};
    MMI_HANDLE_T        win_handle = 0;
    MMIPHONE_SELECT_SIM_DATA_T* temp_data_ptr=PNULL;

    //Return FALSE if input win_id and handler id is PNULL
    if (selectsim_input_ptr == PNULL)
    {
        //SCI_TRACE_LOW:"MMIAPIPHONE_OpenSelectSimWin:select sim_input_ptr couldn't be PNULL !"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_WINTAB_5366_112_2_18_2_45_39_258,(uint8*)"");
        return MMI_RESULT_FALSE;
    }


    if( (PNULL == selectsim_input_ptr->win_id_ptr) ||
        (PNULL == selectsim_input_ptr->callback_func)||
        selectsim_input_ptr->publist_id==NULL)
    {
        //SCI_TRACE_LOW:"MMIAPIPHONE_OpenSelectSimWin:win_id_ptr or  handle_func couldn't be PNULL !"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_WINTAB_5375_112_2_18_2_45_39_259,(uint8*)"");
        return MMI_RESULT_FALSE;
    }

    win_id = *(selectsim_input_ptr->win_id_ptr);

    //judge window is already open
    if ( !MMK_IsOpenWin( MMK_GetWinHandle(MMK_GetFirstAppletHandle(), win_id ) ) )
    {
        //set call back function
        handle_function = selectsim_input_ptr->callback_func;

        //MMIPUB_InitPubListWin

        //set window parameter
        win_info_ptr = SCI_ALLOC_APPZ(sizeof(MMIPUB_INFO_T));
        //SCI_MEMSET(win_info_ptr,0,sizeof(MMIPUB_INFO_T));

        temp_data_ptr = SCI_ALLOC_APPZ(sizeof(MMIPHONE_SELECT_SIM_DATA_T));
        //Set it 
        SCI_MEMCPY(temp_data_ptr,selectsim_input_ptr,sizeof(MMIPHONE_SELECT_SIM_DATA_T));

        win_info_ptr->extra_data_ptr =temp_data_ptr;
        //set user data
        win_info_ptr->user_data       = PNULL;
        //set prompt image id
#ifdef DPHONE_SUPPORT 
        win_info_ptr->prompt_image_id = IMAGE_NULL;
#else        
#ifdef MMIPUBWIN_PDASTYLE_SUPPORT
		win_info_ptr->prompt_image_id = IMAGE_PUBWIN_DOWNARROW;
#else 
		win_info_ptr->prompt_image_id = IMAGE_NULL;
#endif
#endif//syy modify 2010.11.30 -end        

        //set softkey style
        win_info_ptr->softkey_style = MMIPUB_SOFTKEY_CUSTOMER;
        win_info_ptr->win_id = win_id;
        //win_info_ptr->type = MMIPUBWIN_LIST;
        MMIPUB_SetPubListWinType(win_info_ptr);
        //win_info_ptr->user_func = handle_function;
        //get pubwin theme
        MMITHEME_GetPubTheme( win_info_ptr->type, &win_info_ptr->pubwin_theme );

        win_create.applet_handle = MMK_GetFirstAppletHandle();
        win_create.win_id = win_id;
        win_create.func = HandleSIMSelectCommonFuncWinMsg;
        win_create.win_priority = WIN_ONE_LEVEL;
        win_create.add_data_ex_ptr = win_info_ptr;
        win_create.add_data_ptr= selectsim_input_ptr->user_data;    //user_call_data

        //   No use if MMI_PDA_SUPPORT == FALSE
        win_create.window_style = WS_HAS_TRANSPARENT|WS_HAS_PROMPT_WIN;
        //  set style because MMIPUB_GetWinAddDataPtr need check it.
        win_create.window_move_style = MOVE_NOT_FULL_SCREEN_WINDOW;

        if( 0 != ( win_handle= win_info_ptr->win_handle = MMK_CreateWindow( &win_create ) ) )
        {
            result = TRUE;
        }
        if (result)
        {
            GUILIST_INIT_DATA_T  init_data   = {0};
            MMI_CONTROL_CREATE_T create      = {0};
            MMI_CTRL_ID_T        ctrl_id     = selectsim_input_ptr->publist_id;
            GUI_BOTH_RECT_T rect = {0};   

#ifdef MMIPUBWIN_PDASTYLE_SUPPORT
            MMIPUBCreatSoftkeyCtrl( win_handle );
    #ifdef MMI_PDA_SUPPORT
            MMIPUB_SetWinSoftkey(win_id,TXT_NULL,STXT_RETURN,FALSE);
    #else
            MMIPUB_SetWinSoftkey(win_id,TXT_COMMON_OK,STXT_RETURN,FALSE);
    #endif

#else
            GUIWIN_CreateSoftkeyDyn(win_handle,TXT_COMMON_OK,TXT_NULL,STXT_RETURN);
            GUIWIN_CreateTitleDyna(win_handle, TXT_SIM_SEL);
#endif
            GUIWIN_SetTitleTextAlign(win_handle,ALIGN_LVMIDDLE);
            //Create List Ctrl
            rect = MMITHEME_GetWinClientBothRect(win_handle);
            init_data.type           = GUILIST_TEXTLIST_E;
            init_data.both_rect = rect;
            create.ctrl_id           = ctrl_id;
            create.guid              =SPRD_GUI_LIST_ID;
            create.parent_win_handle = win_info_ptr->win_handle;
            create.init_data_ptr     = &init_data;

            MMK_CreateControl( &create );

            //Save publist ctrl_id to win_info
            win_info_ptr->ctrl_handle = MMK_GetCtrlHandleByWin( win_info_ptr->win_handle, ctrl_id );
#ifdef MMIPUBWIN_PDASTYLE_SUPPORT
            if( 0 != win_info_ptr->ctrl_handle )    //Set  list properties
            {
                GUILIST_SetTextFont(
                        win_info_ptr->ctrl_handle,
                        win_info_ptr->pubwin_theme.normal_font,
                        win_info_ptr->pubwin_theme.layout.button_font.color);

                GUILIST_SetBgColor(
                        win_info_ptr->ctrl_handle,
                        win_info_ptr->pubwin_theme.layout.ctrl_bg_color);
            }
#endif
        }

        if (!result)
        {
            if (!MMK_IsOpenWin( win_handle ))   //Create fail,free memory
            {
                if (PNULL !=win_info_ptr->extra_data_ptr)
                SCI_FREE(win_info_ptr->extra_data_ptr);
                SCI_FREE(win_info_ptr);
            }
        }
    }
    else
    {
        //SCI_TRACE_LOW:"MMISET_OpenSelectSimWin:win_id %d is already open!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_WINTAB_5488_112_2_18_2_45_39_260,(uint8*)"d", win_id);
    }
    return MMI_RESULT_TRUE;
}//End of MMICOM_OpenSelectSimWin 





/*Edit by script, ignore 14 case. Fri Apr 27 09:38:53 2012*/ //IGNORE9527
