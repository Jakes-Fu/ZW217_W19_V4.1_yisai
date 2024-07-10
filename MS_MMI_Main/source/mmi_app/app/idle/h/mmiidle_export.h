/*****************************************************************************
** File Name:      mmiidle_export.h                                         *
** Author:         murphy.xie                                                    *
** Date:           02/11/2012                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe t9 text input               *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2/2012         murphy.xie       Create
******************************************************************************/

#ifndef _MMIIDLE_EXPORT_H_
#define _MMIIDLE_EXPORT_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmk_type.h"
#include "guicommon.h"
#include "mmi_text.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmimp3_export.h"
#include "mmi_id.h"
#include "mmipdp_export.h"
#ifdef MMI_DUAL_BATTERY_SUPPORT
#include "dualbat_drvapi.h"
#endif
#ifdef MMI_STATUSBAR_SCROLLMSG_SUPPORT
#include "guistatusbar_scrollmsg.h"
#include "mmisms_app.h"
#endif
#include "mmiidle_func.h"
#include "guiform.h"
#ifdef LIVE_WALLPAPER_FRAMEWORK_SUPPORT
#include "mmilivewallpaper_export.h"
#endif
#include "mmiidle_display.h"
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
/**---------------------------------------------------------------------------*
 **                         Macro Declaration                                 *
 **---------------------------------------------------------------------------*/
// 短信的状态标示
//typedef uint8 SMS_STATE_TYPE;
//#define NORMAL_SMS    0 // 不显示短信ICON
#define NEW_SMS       1 // 显示有短信未读的ICON
//#define FULL_SMS      2 // 连续闪烁ICON,表示SMS已满

// 来电的状态标示
//typedef uint8 CALL_STATE_TYPE;
//#define NORMAL_CALL    0 // 不显示CALL ICON,标示没有CALL
//#define NEW_CALL       1 // 连续闪烁ICON,表示有新SMS

typedef uint8 TIMER_OPERATE_RESULT; //停止idle timer的结果
#define TIMER_INVALIDATE    0   // 无效值
#define TIMER_START_SUCCESS 1   // Timer由为启动到启动成功
#define TIMER_START_FAILURE 2   // Timer启动失败，不满足启动的条件
#define TIMER_HAVE_STARTED  3   // Timer已经启动，没有必要再启动
#define TIMER_STOP_SUCCESS  4   // Timer由启动到关闭成功
#define TIMER_STOP_FAILURE  5   // Timer关闭失败，不满足关闭的条件
#define TIMER_HAVE_STOPED   6   // Timer已经关闭，没有必要再关闭

//background play type(mp3 and fm)
typedef enum
{
    MMIBGPLAY_TYPE_NONE,    //none
    MMIBGPLAY_TYPE_MP3,     //mp3
    MMIBGPLAY_TYPE_FM,      //fm
    MMIBGPLAY_TYPE_KURO,    //KURO
    MMIBGPLAY_TYPE_MUSIC,   //KURO
    MMIBGPLAY_TYPE_JAVA_BG, //Java BG
    MMIBGPLAY_TYPE_BT,      //bt
#ifdef WRE_SUPPORT
    MMIBGPLAY_TYPE_WRE_BG, //WRE BG
#endif
    MMIBGPLAY_TYPE_ALL,     //all
} MMIBGPLAY_TYPE_E;
/**--------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         EXTERNAL DEFINITION                              *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
 /*****************************************************************************/
//  Description : update idle 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIIDLE_MAIN_UpdateIdle(void);

/*****************************************************************************/
//  Description : 点亮或关闭背光时应用需要做的处理
//  Global resource dependence : 
//  Author: Great.Tian
//  Note:
/*****************************************************************************/
void MMIAPIIDLE_HandleLCDBacklight(
    BOOLEAN is_light // [IN] TRUE: 点亮背光; FALSE: 关闭背光
     );
     
/*****************************************************************************/
//  Description : display the main and sub idle soft icon 
//  Global resource dependence : 
//  Author: Great.Tian
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIIDLE_IdleWinIsOpen(
                            void
                            );
/*****************************************************************************/
//  Description : init the main applicaiton
//  Global resource dependence : 
//  Author: Great.Tian
//  Note: 2003-12-09
/*****************************************************************************/
void MMIAPIIDLE_init(void);
                            

/*****************************************************************************/
//  Description : open idle win
//  Global resource dependence : 
//  Author: bruce.chi
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIIDLE_OpenIdleWin(void);

/*****************************************************************************/
//  Description : open dial window without any parm
//  Global resource dependence : 
//  Author: tao.xue
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIIDLE_EnterDialWin(void);    

/*****************************************************************************/
//  Description : open dial window
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIIDLE_OpenDialWin(
                                MMI_MESSAGE_ID_E    msg_id,     //in:
                                BOOLEAN             is_dial_key //in:
                                );        


/*****************************************************************************/
//  Description : close dial window
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIIDLE_CloseDialWin(void);

/*****************************************************************************/
//  Description : reset dial num in edit
//  Global resource dependence : 
//  Author: tao.xue
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIIDLE_ResetDialNum(void);

/*****************************************************************************/
//  Description : close dial menu window
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIIDLE_CloseDialMenuWin(void);

#ifndef TOUCH_PANEL_HWICON_SUPPORT_NONE
/*****************************************************************************/
//  Description : start Hard icon for tp
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIIDLE_StartHdShortCutBarIcon(void);
#endif

#ifdef WRE_SUPPORT
/*****************************************************************************/
//  Description : edit dial window
//  Global resource dependence : 
//  Author: qingjun.yu
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIIDLE_EditDialWin(uint32 simNo,BOOLEAN is_dial_key, char * tele_num, BOOLEAN is_dial_str);
#endif


#if defined(TOUCH_PANEL_SUPPORT)
/*****************************************************************************/
// 	Description : MMICL_CreateDialWin
//	Global resource dependence : 
//  Author:tao.xue
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPIIDLE_CreateDialWin(MMI_WIN_ID_T	parent_win_id);
#endif


#ifdef TOUCH_PANEL_SUPPORT
/*****************************************************************************/
//  Description :create  a dial panel including "1,2,3,4,5,6,7,8,9,*,0,#"
//  Global resource dependence : 
//  Author: tao.xue
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIIDLE_ConstructDialPanel(MMI_WIN_ID_T win_id, MMI_HANDLE_T parent_handle, GUIFORM_INIT_DATA_T form_init);
#endif

/*****************************************************************************/
//  Description : MMIAPIIDLE_IsChargeConnected
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIIDLE_IsChargeConnected(void);

/*****************************************************************************/
//  Description : Is Prohibit Handle HW Icon
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIIDLE_IsProhibitHandleHWIcon(void);


#ifdef LIVE_WALLPAPER_FRAMEWORK_SUPPORT
/*****************************************************************************/
//  Description : MAINAPP_GetMainAppFgInfo
//  Global resource dependence : 
//  Author:arvin.wu
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIIDLE_GetMainAppFgInfo(LW_FG_INFO *lw_fg_info_ptr);
#endif

/*****************************************************************************/
//  Description : handle idle plmn message.
//  Global resource dependence : 
//  Author: Michael.shi
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIIDLE_OpenSelNetWorkWin(MN_DUAL_SYS_E dual_sys );

#ifdef CALENDAR_SUPPORT
/*****************************************************************************/
//  Description : open schedule window
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIIDLE_OpenScheduleWin(
                                    MMI_WIN_ID_T    win_id
                                    );
#endif
#ifdef MMI_ENABLE_DCD
/*****************************************************************************/
//  Description : 把当前IDLE的焦点在DCD上
//  Global resource dependence : 
//  Author: jun.hu
//  Note:  这里加上是因为用户在IDLE界面触摸笔点击DCD时，TAB键处理了事件，并发了个MSG_NOTIFY_TAB_SWITCH
//         消息上来，idle界面没有截获触摸笔消息，故在这个消息里面处理....
/*****************************************************************************/
PUBLIC  void MMIAPIIDLE_FocusToDCDLine(void);

/*****************************************************************************/
//  Description : 如果当前的焦点在DCD上，则切换到SHORTCUT1上去，此仅供外部调用.
//  Global resource dependence : 
//  Author: jun.hu
//  Note:
/*****************************************************************************/
PUBLIC  void MMIAPIIDLE_DCDFocusToShortcut(void);

/*****************************************************************************/
//  Description : active or deactive dcd ctrl
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIIDLE_ActiveDcdCtrl(
                                  MMI_WIN_ID_T     win_id,
                                  BOOLEAN          is_active   //激活or去激活
                                  );
#endif

/*****************************************************************************/
// 	Description : need show soft icon and network name according to setting
//	Global resource dependence : 
//  Author: bruce.chi
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIIDLE_NeedShowIconAccordingToDualSysSetting(MN_DUAL_SYS_E dual_sys);

//  Description : draw stk set idle mode text
//  Global resource dependence : 
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
#ifdef MMI_IDLE_STKTEXT_SUPPORT 
PUBLIC void MMIAPIIDLE_MAIN_SetIdleModeText(MN_DUAL_SYS_E dual_sys, MMI_STRING_T *stk_idle_str);
#endif
#if defined(MMI_DIALPANEL_DTMF_SUPPORT)
/*****************************************************************************/
// 	Description : Dial panel Play DTMF Tone
//	Global resource dependence : none
//	Author:
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIIDLE_PlayDialTone(MMI_MESSAGE_ID_E msg_id, BOOLEAN is_keypress);

/*****************************************************************************/
// 	Description : Stop panel Play DTMF Tone
//	Global resource dependence : none
//	Author:
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIIDLE_StopDialTone(void);
#endif

/*****************************************************************************/
//  Description : 全键盘下，某些界面按下数字/字母共享键，转为对应的数字消息
//  Global resource dependence : 
//  Author: tao.xue
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMIAPIIDLE_GetNumMsgId(uint16 orign_msg_id);

#ifdef PDA_UI_DROPDOWN_WIN
/*****************************************************************************/
//  Description :Create Stk Idle Text Win
//  Global resource dependence : none
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIIDLE_CreateStkIdleTextWin(void);
#endif

#if defined(TOUCH_PANEL_SUPPORT)
/*****************************************************************************/
//  Description : 创建DTMF键盘
//  Global resource dependence : 
//  Author: tao.xue
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIIDLE_CreateDtmfPanel(MMI_WIN_ID_T win_id);

/*****************************************************************************/
//  Description : 显示DTMF键盘
//  Global resource dependence : 
//  Author: tao.xue
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIIDLE_ShowDtmfPanel(MMI_WIN_ID_T win_id);

/*****************************************************************************/
//  Description : 隐藏DTMF键盘
//  Global resource dependence : 
//  Author: tao.xue
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIIDLE_HideDtmfPanel(MMI_WIN_ID_T win_id);
/*****************************************************************************/
//  Description : 设置DTMF键盘中的编辑框
//  Global resource dependence : 
//  Author: tao.xue
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIIDLE_SetDtmfEditFormParam(MMI_WIN_ID_T win_id);

#ifndef MAINLCD_SIZE_320X480
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: samboo.shen
//  Note:
/*****************************************************************************/
PUBLIC GUI_LCD_DEV_INFO MMIAPIIDLE_CreateDtmfPanelLayer(MMI_WIN_ID_T win_id);

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: samboo.shen
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIIDLE_SetDtmfPanelLayerPosition(MMI_WIN_ID_T win_id, GUI_LCD_DEV_INFO dtmf_panel_layer );

#endif
#endif

#if defined(MMI_DIALPANEL_DTMF_SUPPORT)
#ifdef TOUCH_PANEL_SUPPORT
/*****************************************************************************/
// 	Description : Is Press In Dial Panel
//	Global resource dependence : none
//	Author:
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIIDLE_IsPressInDialPanel(GUI_POINT_T point);
#endif
#endif

#ifdef MMI_BATTERY_PERCENT_IN_STATUSBAR
/*****************************************************************************/
//  Description :MMIAPIIDLE_GetBatteryPercentStbTxt
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 获得status bar上电池电量百分比的字串，同时也记录该百分比
/*****************************************************************************/
PUBLIC GUIWIN_STBDATA_TEXT_T  MMIAPIIDLE_GetBatteryPercentStbTxt(void);
#endif

/**---------------------------------------------------------------------------*
 **                    Old Interface  Macro Declaration    				   *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  如下宏定义用以实现对外接口更名前后的兼容
/*****************************************************************************/
#ifdef MMI_FUNC_COMPATIBLE_SUPPORT

#define MAIN_UpdateIdle				         MMIAPIIDLE_MAIN_UpdateIdle
#define MMI_HandleLCDBacklight			     MMIAPIIDLE_HandleLCDBacklight
#define MMIMAIN_IdleWinIsOpen					 MMIAPIIDLE_IdleWinIsOpen
#define main_app_init					        MMIAPIIDLE_init
#define MAIN_OpenIdleWin					    MMIAPIIDLE_OpenIdleWin
#define MMIIDLE_EnterDialWin					 MMIAPIIDLE_EnterDialWin
#define MMIIDLE_OpenDialWin					 MMIAPIIDLE_OpenDialWin
#define MMIIDLE_CloseDialWin					 MMIAPIIDLE_CloseDialWin
#define MMIIDLE_CloseDialMenuWin				MMIAPIIDLE_CloseDialMenuWin
#ifndef TOUCH_PANEL_HWICON_SUPPORT_NONE
#define MMIIDLE_StartHdShortCutBarIcon		MMIAPIIDLE_StartHdShortCutBarIcon
#endif
#ifdef WRE_SUPPORT
#define MMIIDLE_EditDialWin				    MMIAPIIDLE_EditDialWin
#endif
#if defined(MMI_PDA_SUPPORT) && defined(TOUCH_PANEL_SUPPORT)
#define MMIIDLE_CreateDialWin				    MMIAPIIDLE_CreateDialWin
#endif

#ifdef TOUCH_PANEL_SUPPORT
#define MMIIDLE_ConstructDialPane		    MMIAPIIDLE_ConstructDialPanel
#endif

#define MMIIDLE_IsProhibitHandleHWIcon		 MMIAPIIDLE_IsProhibitHandleHWIcon
#ifdef LIVE_WALLPAPER_FRAMEWORK_SUPPORT
#define MAINAPP_GetMainAppFgInfo		        MMIAPIIDLE_GetMainAppFgInfo
#endif

#define MMIIDLE_OpenSelNetWorkWin		    MMIAPIIDLE_OpenSelNetWorkWin
#ifdef CALENDAR_SUPPORT
#define MMIIDLE_OpenScheduleWin		        MMIAPIIDLE_OpenScheduleWin
#endif
#ifdef MMI_ENABLE_DCD
#define MMIIDLE_FocusToDCDLine		       MMIAPIIDLE_FocusToDCDLine
#define MMIIDLE_DCDFocusToShortcut		   MMIAPIIDLE_DCDFocusToShortcut
#define MMIIDLE_ActiveDcdCtrl		           MMIAPIIDLE_ActiveDcdCtrl
#endif

#define MMIIDLE_NeedShowIconAccordingToDualSysSetting		       MMIAPIIDLE_NeedShowIconAccordingToDualSysSetting
#ifdef MMI_IDLE_STKTEXT_SUPPORT 
#define MAIN_SetIdleModeText		            MMIAPIIDLE_MAIN_SetIdleModeText
#endif
#endif //MMI_FUNC_COMPATIBLE_SUPPORT

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
